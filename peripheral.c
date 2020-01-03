/*
   This file is provided under the LGPL license ver 2.1.
   Written by Katsumi.
   https://github.com/kmorimatsu/
*/

#include <xc.h>
#include <sys/attribs.h>
#include "z80.h"
#include "api.h"
#include "peripheral.h"
#include "monitor.h"
#include "main.h"

#define printstr(c,t) printstr_cur(c,t)
#define printhex8(c,d) printhex8_cur(c,d)
#define printhex16(c,d) printhex32_cur(c,d)
#define printhex32(c,d) printhex16_cur(c,d)

#define VIDEOHEIGHT 27

unsigned char RAM[65536] __attribute__((persistent,address(0xA0010000)));
unsigned char g_cursorchar;
unsigned char g_blinktimer;
unsigned char g_cpmkeybuff[8];
unsigned char g_cpmkeybuffwrite;
unsigned char g_cpmkeybuffread;

unsigned char g_timer8;
unsigned short g_timer8count;
unsigned char g_ram_rom;
unsigned char g_rom_a14;
unsigned char g_rom_a15;

unsigned int g_sfrvalue;

void peripheral_init(void){
	// Clear latch etc
	g_timer8=0;
	g_timer8count=0;
	g_ram_rom=0;
	g_rom_a14=0;
	g_rom_a15=0;
	// Clear cursor
	g_cursorchar=cursor[0];
	g_blinktimer=0;
	// Initialize key
	g_cpmkeybuffwrite=0;
	g_cpmkeybuffread=0;
	// Enable software intterupt for key
	IPC0bits.CS0IP=3;
	IPC0bits.CS0IS=0;
	IFS0bits.CS0IF=0;
	IEC0bits.CS0IE=1;
}

UINT8 readMemory(UINT16 addr){
	if (0x4000<=addr || g_ram_rom) {
		return RAM[addr];
	} else {
		if (g_rom_a14) addr+=0x4000;
		if (g_rom_a15) addr+=0x8000;
		return monitor[addr];
	}
}
// See macro in peripheral.h for writeMemory()

UINT8 readIO(UINT8 addrL, UINT8 addrH){
	UINT8 ret;
	unsigned int sfraddr;
	if (0x00<=addrL && addrL<=0x07) {
		// SIO
		switch(addrL&0x03){
			case 0x00:
			case 0x01:
				//ret=io.keypress;
				//io.keypress=0;
				if (cpm_const()) {
					return cpm_conin();
				} else {
					return 0;
				}
			case 0x02:
			case 0x03:
				if (g_timer8count) {
					g_timer8count--;
					g_timer8=g_timer8 ? 0:1;
				}
				return 0x06|(cpm_const() ? 1:0)|(g_timer8 ? 8:0);
			default:
				break;
		}
	} else if (0x10<=addrL && addrL<=0x17) {
		// IDE
		return ide_read(addrL&7);
	} else if (0x38<=addrL && addrL<=0x3F) {
		// Latch
	} else if (0x40<=addrL && addrL<=0x43) {
		// PIC32MX SFR bits 8-15, 16-23, and 24-31
		switch(addrL){
			case 0x41:
				return (g_sfrvalue>>8)&0xff;
			case 0x42:
				return (g_sfrvalue>>16)&0xff;
			case 0x43:
				return (g_sfrvalue>>24)&0xff;
			case 0x40:
			default:
				// Reserved 
				break;
		}
	} else if (0x80<=addrL) {
		// PIC32MX SFR
		// Calculation of SFR address from Z80 I/O address (as B and C registers) is done as follows:
		// 0xBF800000 | ((C&0x40)<<13 | (B<<8) | ((C&0x3f)<<2)
		sfraddr=addrL&0x40;
		sfraddr<<=5;
		sfraddr|=addrH;
		sfraddr<<=6;
		sfraddr|=addrL&0x3f;
		sfraddr<<=2;
		sfraddr|=0xBF800000;
		g_sfrvalue=((volatile unsigned int*)sfraddr)[0];
		return g_sfrvalue&0xff;
	}
	return 0;
}
void writeIO(UINT8 addrL, UINT8 addrH, UINT8 data){
	unsigned int sfraddr;
	if (0x00<=addrL && addrL<=0x07) {
		// SIO
		switch(addrL&0x03){
			case 0x00:
				// SIOA_D
			case 0x01:
				// SIOB_D
				cpm_conout(data);
				break;
		}
	} else if (0x10<=addrL && addrL<=0x17) {
		// IDE
		ide_write(addrL&7,data);
	} else if (0x38<=addrL && addrL<=0x3F) {
		// Latch
		switch(addrL){
			case 0x38:
				g_ram_rom=data&1;
				break;
			case 0x3E:
				g_rom_a15=data&1;
				break;
			case 0x3F:
				g_rom_a14=data&1;
				break;
		}
	} else if (0x40<=addrL && addrL<=0x43) {
		// PIC32MX SFR bits 8-15, 16-23, and 24-31
		switch(addrL){
			case 0x41:
				g_sfrvalue&=0xffff0000;
				g_sfrvalue|=(unsigned int)data<<8;
				break;
			case 0x42:
				g_sfrvalue&=0xff00ff00;
				g_sfrvalue|=(unsigned int)data<<16;
				break;
			case 0x43:
				g_sfrvalue&=0x00ffff00;
				g_sfrvalue|=(unsigned int)data<<24;
				break;
			case 0x40:
			default:
				// Reserved
				break;
		}
	} else if (0x80<=addrL) {
		// PIC32MX SFR
		// Calculation of SFR address from Z80 I/O address (as B and C registers) is done as follows:
		// 0xBF800000 | ((C&0x40)<<13 | (B<<8) | ((C&0x3f)<<2)
		sfraddr=addrL&0x40;
		sfraddr<<=5;
		sfraddr|=addrH;
		sfraddr<<=6;
		sfraddr|=addrL&0x3f;
		sfraddr<<=2;
		sfraddr|=0xBF800000;
		g_sfrvalue&=0xffffff00;
		g_sfrvalue|=data;
		((volatile unsigned int*)sfraddr)[0]=g_sfrvalue;
	}
}

void cpm_conout(unsigned char ascii){
	int i;
	// Restore cursor
	if (g_blinktimer<30) {
		cursor[0]=g_cursorchar;
	}
	g_blinktimer=30;
	switch(ascii){
		case 0x00: // null
		case 0x02: // ^B
		case 0x03: // ^C
		case 0x04: // ^D
		case 0x05: // ^E
		case 0x0c: // ^L
		case 0x0e: // ^N
		case 0x0f: // ^O
		case 0x10: // ^P
		case 0x11: // ^Q
		case 0x12: // ^R
		case 0x13: // ^S
		case 0x14: // ^T
		case 0x15: // ^U
		case 0x16: // ^V
		case 0x17: // ^W
		case 0x19: // ^Y
			break;
		case 0x01: // ^A
			if (&TVRAM[0]<cursor) cursor--;
//			g_cursor--;
//			if (g_cursor<0) g_cursor=0;
			break;
		case 0x06: // ^F
			if (cursor<&TVRAM[twidth*twidthy-1]) cursor++;
//			g_cursor++;
//			if (80*24<=g_cursor) g_cursor=80*24-1;
			break;
		case 0x07: // ^G (BEL)
//			g_beep=1;
			wait_msec(500);
			cursor[0]=g_cursorchar;
//			g_beep=0;
			break;
		case 0x09: // ^I (TAB)
			i=cursor-&TVRAM[0];
			switch(twidth){
				case 30:
					if ((i%30)<20) i+=10-(i%10);
					break;
				case 36:
					if ((i%36)<27) i+=9-(i%9);
					break;
				case 40:
					if ((i%40)<30) i+=10-(i%10);
					break;
				case 48:
					if ((i%48)<40) i+=8-(i%8);
					break;
				case 64:
					if ((i%64)<56) i+=8-(i%8);
					break;
				case 80:
				default:
					if ((i%80)<70) i+=10-(i%10);
					break;
			}
			cursor=&TVRAM[i];
			break;
		case 0x08: // ^H (BS)
			if (&TVRAM[0]<cursor) {
				cursor--;
				cursor[0]=0x20;
			}
			break;
		case 0x0a: // (^J) LF
			printchar('\n');
			break;
		case 0x0b: // ^K
			if (&TVRAM[twidth]<=cursor) cursor-=twidth;
			break;
		case 0x0d: // ^M (CR)
			i=cursor-&TVRAM[0];
			i-=i%twidth;
			cursor=&TVRAM[i];
			break;
		case 0x18: // ^X
/*			for(var i=i<parseInt(g_cursor/80)*80;i<g_cursor;i++){
				this.write(i,0x20);
			}
			g_cursor=1+g_cursor/80;
			g_cursor=g_cursor*80;
*/
			break;
		case 0x7f: // DEL
			break;
		case 0x1a: // ^Z
			clearscreen();
			cursor=&TVRAM[0];
			break;
		case 0x1c: // ^\
			printchar(g_cursorchar);
			break;
		case 0x1d: // ^]
			if (&TVRAM[0]<cursor) cursor--;
			break;
		case 0x1e: // ^^
			cursor=&TVRAM[0];
			break;
		default:
			printchar(ascii);
			break;
	}
	// Update cursor
	g_cursorchar=cursor[0];
	g_blinktimer=58;
}

int cpm_const(void){
	return (g_cpmkeybuffread!=g_cpmkeybuffwrite);
}
unsigned char cpm_conin(void){
	unsigned char ret;
	ret=g_cpmkeybuff[g_cpmkeybuffread++];
	g_cpmkeybuffread&=7;
	return ret;
}

void __ISR(_CORE_SOFTWARE_0_VECTOR,IPL3SOFT) CS0Hanlder(void){
	// This function is called at ~60 Hz frequency 
	static unsigned char s_prevchar=0;
	static int s_next8=0;
	unsigned char curchar;
	// Drop flag
	IFS0bits.CS0IF=0;
	// Read key
	curchar=ps2readkey();
	if (vkey) {
		// Detect ENTER key etc
		if (!curchar) {
			curchar=vkey&0xff;
			if (curchar&0xe0) curchar=0;
			if (curchar==0x0d) curchar=0x0a;
		}
		// Check CTRL key
		if (vkey&0x0200) {
			curchar=vkey&0x1f;
		}
		// Update buffer if key is just pressed
		if (s_prevchar!=curchar && curchar) {
			g_cpmkeybuff[g_cpmkeybuffwrite++]=curchar;
			g_cpmkeybuffwrite&=7;
			// Interrupt
			intZ80(0xE0);
		}
	}
	// Update static data
	s_prevchar=curchar;
	// Blink cursor
	g_blinktimer++;
	if (59<g_blinktimer) {
		g_blinktimer=0;
		cursor[0]=0x87;
	} else if (30==g_blinktimer) {
		cursor[0]=g_cursorchar;
	}
	// Produce 8 hz square wave here
	// To do this, check core timer (47727267 Hz) for 16 hz, 
	// and invert flag with this frequency
	if ((int)(s_next8-coretimer())<0) {
		s_next8+=47727267/16;
		if (g_timer8count<0xffff) g_timer8count++;
	}
	if (g_timer8count) {
		// Interrupt
		intZ80(0xE0);
	}
}

/*
	Wrappers for KM-Z80 midi functions follow
*/

#undef printchar
#undef printstr
#undef printhex8
#undef printhex16
#undef printhex32

void printchar_cur(int cur,char c){
	while(80*VIDEOHEIGHT<=cur) cur-=240;
	setcursor(cur%80,cur/80,7);
	printchar(c);
}

void printstr_cur(int cur,char* str){
	while(80*VIDEOHEIGHT<=cur) cur-=240;
	setcursor(cur%80,cur/80,7);
	printstr(str);
}

void printhex8_cur(int cur,unsigned char d){
	while(80*VIDEOHEIGHT<=cur) cur-=240;
	setcursor(cur%80,cur/80,7);
	printhex8(d);
}

void printhex16_cur(int cur,unsigned short d){
	while(80*VIDEOHEIGHT<=cur) cur-=240;
	setcursor(cur%80,cur/80,7);
	printhex16(d);
}

void printhex32_cur(int cur,unsigned int d){
	while(80*VIDEOHEIGHT<=cur) cur-=240;
	setcursor(cur%80,cur/80,7);
	printhex32(d);
}

void vertical_scroll(void){
	int i;
	for(i=0;i<20*(VIDEOHEIGHT-1);i++){
		((unsigned int*)TVRAM)[i]=((unsigned int*)TVRAM)[i+20];
	}
	for(i=i;i<20*VIDEOHEIGHT;i++){
		((unsigned int*)TVRAM)[i]=0x20202020;
	}
}