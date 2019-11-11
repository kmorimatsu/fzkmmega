/*
   This file is provided under the LGPL license ver 2.1.
   Written by K.Tanaka & Katsumi
   http://www.ze.em-net.ne.jp/~kenken/index.html
   http://hp.vector.co.jp/authors/VA016157/
*/

// Using following libraries
// ps2keyboard370f.X.a : PS/2 keyboard input
// lib_videoout_megalopa.X.a : NTFS color video
// sdfsio370fLib.a ： SD/MMC card file system

/*
	PIC32MX peripheral assignment
	
	Interrupts
		NTSC,   Timer2, vector  8, priority 5
		NTSC,   OC5,    vector 22, priority 5
		NTSC,   OC2,    vector 10, priority 5
		NTSC,   OC1,    vector  6, priority 5
		PS/2,   CNF,    vector 33, priority 6
		PS/2,   Timer5, vector 20, priority 4
		MUSIC,  CS0,    vector  1, priority 2
		SERIAL, UART,   vector 31, priority 3
		TIMER,  Timer1, vector  4, priority 3
		INT,    CS1,    vector  2, priority 1
	
	Timers
		Timer1 not used
		Timer2 NTSC
		Timer3 not used
		Timer4 not used
		Timer5 PS/2
	
	DMA
		DMA0 not used
		DMA1 not used
		DMA2 not used
		DMA3 PS/2
	
	Output compair
		OC1 NTSC
		OC2 NTSC
		OC3 not used
		OC4 not used
		OC5 NTSC
	
	UART
		UART1 not used
		UART2 not used
	
	SPI
		SPI1 not used
		SPI2 SD/MMC
	
	I2C
		I2C1 not used
		I2C2 not used
	
	ポート使用
		B0  I/O, AN0
		B1  I/O, AN1
		B2  I/O, AN2
		B3  I/O, AN3
		B4  I/O, AN4
		B5  I/O, AN5
		B6  I/O, AN6
		B7  I/O, AN7
		B8  I/O, AN8
		B9  I/O, AN9
		B10 I/O, AN10
		B11 I/O, AN11
		B12 I/O, AN12
		B13 I/O, AN13
		B14 I/O, AN14
		B15 I/O, AN15
		C12 OSC1 (Crystal)
		C13 U1TX (UART)
		C14 U1RX (UART)
		C15 OSC2 (Crystal)
		D0  SW_DOWN
		D1  SW_LEFT
		D2  SW_UP
		D3  SW_RIGHT
		D4  SW_START
		D5  SW_FIRE
		D6  
		D7  
		D8  
		D9  SPI1_CS (SPI)
		D10 PWM1
		D11 PWM2
		E0  NTSC
		E1  NTSC
		E2  NTSC
		E3  NTSC
		E4  NTSC
		E5  I/O, AN22
		E6  I/O, AN23
		E7  I/O, AN27
		F0  PS/2 DAT
		F1  PS/2 CLK
		F2  SDI1 (SPI)
		F3  SPI2_CS (MMC)
		F4  AUDIO_R
		F5  AUDIO_L
		F6  SCK1 (SPI)
		G2  SCL1 (I2C)
		G3  SDA1 (I2C)
		G6  SCK2 (MMC)
		G7  SDI2 (MMC)
		G8  SDO2 (MMC)
		G9  SDO1 (SPI)
*/

#include <xc.h>
#include "interface/keyinput.h"
#include "interface/lib_video_megalopa.h"
#include "interface/ps2keyboard.h"
#include "main.h"

// Using crystal with PLL (20/3)
// 3.579545×4＝14.31818MHz
#pragma config FSRSSEL = PRIORITY_7
#pragma config PMDL1WAY = OFF
#pragma config IOL1WAY = OFF
//#pragma config FUSBIDIO = OFF
//#pragma config FVBUSONIO = OFF
#pragma config FPLLIDIV = DIV_3
#pragma config FPLLMUL = MUL_20
//#pragma config UPLLIDIV = DIV_1
//#pragma config UPLLEN = OFF
#pragma config FPLLODIV = DIV_1
#pragma config FNOSC = PRIPLL
#pragma config FSOSCEN = OFF
#pragma config IESO = OFF
#pragma config POSCMOD = XT
#pragma config OSCIOFNC = OFF
#pragma config FPBDIV = DIV_1
#pragma config FCKSM = CSDCMD
#pragma config FWDTEN = OFF
#pragma config DEBUG = OFF
#pragma config PWP = OFF
#pragma config BWP = OFF
#pragma config CP = OFF

unsigned char initialvmode;

void printhex8(unsigned char d){
	printchar("0123456789ABCDEF"[d>>4]);
	printchar("0123456789ABCDEF"[d&0x0f]);	
}

void printhex16(unsigned short d){
	printhex8(d>>8);
	printhex8(d&0x00ff);
}

void printhex32(unsigned int d){
	printhex16(d>>16);
	printhex16(d&0x0000ffff);
}

void wait60thsec(unsigned short n){
	n+=drawcount;
	while(drawcount!=n) asm("wait");
}

int main(void){
	char ascii, cursorchar, blinktimer;

	// Port initializations
	CNPUB = 0xFFFF; // PORTB all pull up (I/O)
	TRISB = 0xFFFF; // PORTB all input
	CNPUC = 0x4000; // PORTC14 pull up (U1RX)
	TRISC = 0x4000; // PORTC14 output but input for everything else
	TRISD = KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT;// input from button port
	CNPUE = 0x00E0; // PORTE5-7 pull up (I/O)
	TRISE = 0x00E0; // PORTE0-4 output 5-7 input入力
	CNPUF = 0x0004; // PORTF2 pull up (SDI1)
	TRISF = 0x0004; // PORTF2 input but output for everthing else
	TRISG = 0x0080; // PORTG7 input but output for everything else

	ANSELB = 0x0000; // all digital
	ANSELD = 0x0000; // all digital
	ANSELE = 0x0000; // all digital
	ANSELG = 0x0000; // all digital
	CNPUDSET=KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT;// pull up
	CNPUFSET=0x0004; // PORTF2 (SDI1)
	ODCF = 0x0003;	//RF0,RF1: open drain
	ODCG = 0x000c;//RG2,RG3: open drain
	LATGSET = 0x000c;

	// peripheral pin asignment
	SDI2R = 1; //RPG7: SDI2
	RPG8R = 6; //RPG8: SDO2

	init_composite(); // Initialize video
	set_videomode(VMODE_MONOTEXT,0); // 80 characters mode
	setcursor(0,0,7);

	printstr("MachiKania type M console\n");
	printstr("NTSC 80 x 27 character display and PS/2 keyboard\n");
	printstr("Copyright (c) 2015-2019 Kenken\n");
	printstr("Copyright (c) 2016-2019 Katsumi\n");
	printstr("Init File System...");
	// Initialize the File System
	if(!FSInit()){ // Initialize file system
		printstr("\nFile System Error\n");
		printstr("Insert Correct Card\n");
		printstr("And Reset\n");
		while(1) asm("wait");
	}
	printstr("OK\n");

	lockkey=2; // NumLock
	keytype=0; // JP key board
	printstr("Init PS/2...");
	wait60thsec(30); 
	if(ps2init()){ // Initialize PS/2
		printstr("Keyboard Not Found\n");
	} else {
		printstr("OK\n");
	}

	wait60thsec(60);

	// Non-blocking type example
	while(1){
		cursorchar=cursor[0];
		blinktimer=0;
		do {
			switch (blinktimer++){
				case 40:
					blinktimer=1;
				case 0:
					cursor[0]='_';
					break;
				case 20:
					cursor[0]=cursorchar;
					break;
			}
			wait60thsec(1); // 60 Hz wait
			ascii=ps2readkey();
		} while(!vkey);
		cursor[0]=cursorchar;
		if (!ascii) {
			ascii=vkey&0xff;
			if (ascii&0xe0) ascii=0;
			if (ascii==0x0d) ascii=0x0a;
		}
		if (ascii) printchar(ascii);
	}

	// Blocking type example
	while(1){
		ascii=cursorinputchar();
		if (!ascii) {
			ascii=vkey&0xff;
			if (ascii&0xe0) ascii=0;
			if (ascii==0x0d) ascii=0x0a;
		}
		if (ascii) printchar(ascii);
	}
}
