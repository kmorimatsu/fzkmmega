/*
   This file is provided under the LGPL license ver 2.1.
   Written by Katsumi
   http://hp.vector.co.jp/authors/VA016157/
*/

#include <xc.h>
#include "lib_video_megalopa.h"
#include "exception.h"

unsigned int g_ex_data[EXCEPTION_DATA_SIZE] __attribute__((persistent));

void _general_exception_handler_main (void);
void _general_exception_handler (void){
	//asm volatile("la $sp,%0"::"i"(&RAM[RAMSIZE-4]));
	asm volatile("j _general_exception_handler_main");
}
void _general_exception_handler_main (void){
	int i;
	// $v1 is g_ex_data
	asm volatile("la $v1,%0"::"i"(&g_ex_data[0]));
	// g_ex_data[1]=$sp
	asm volatile("sw $sp,4($v1)");
	// Prepare proper stack area before SoftReset
	asm volatile("la $sp,%0"::"i"(&g_ex_data[EXCEPTION_DATA_SIZE-1]));
	// g_ex_data[2]=Cause
	asm volatile("mfc0 $v0,$13");
	asm volatile("sw $v0,8($v1)");
	// g_ex_data[3]=EPC 
	asm volatile("mfc0 $v0,$14");
	asm volatile("sw $v0,12($v1)");
	// Exception occured
	g_ex_data[0]=1;
	// Wait until all buttons are released and reset MachiKania.
	#ifdef __DEBUG
		asm volatile("j 0xBFC00000");
	#else
		for(i=0;i<100000;i++){
			if((PORTB&(KEYUP|KEYDOWN|KEYLEFT|KEYRIGHT|KEYSTART|KEYFIRE))
				!=(KEYUP|KEYDOWN|KEYLEFT|KEYRIGHT|KEYSTART|KEYFIRE)) i=0;
		}
		RCONbits.POR=0;
		RCONbits.EXTR=0;
		asm volatile("j 0x9FC00000");
	#endif
}

void blue_screen(void){
	int i,j,sp;
	unsigned int* opos;
	if (RCONbits.POR || RCONbits.EXTR) {
		// After power on or reset. Reset flags and return.
		RCONbits.POR=0;
		RCONbits.EXTR=0;
		for(i=0;i<EXCEPTION_DATA_SIZE;i++){
			// Reset all g_ex_data[]
			g_ex_data[i]=0;
		}
		return;
	} else if (g_ex_data[0]==0) {
		// No exception found.
		return;
	}
	// Exception occured before SoftReset().
	printstr("STOP");
	printstr("\nException at ");
	printhex32(g_ex_data[3]);
	printstr("\n      Cause: ");
	printhex32(g_ex_data[2]);
	printstr("\n ");
	switch((g_ex_data[2]>>2)&0x1f){
		case 0:  printstr("(Interrupt)"); break;
		case 1:  printstr("(TLB modification)"); break;
		case 2:  printstr("(TLB load/fetch)"); break;
		case 3:  printstr("(TLB store)"); break;
		case 4:  printstr("(Address load/fetch error )"); break;
		case 5:  printstr("(Address store error)"); break;
		case 6:  printstr("(Bus fetch error)"); break;
		case 7:  printstr("(Bus load/store error)"); break;
		case 8:  printstr("(Syscall)"); break;
		case 9:  printstr("(Breakpoint)"); break;
		case 10: printstr("(Reserved instruction)"); break;
		case 11: printstr("(Coprocessor Unusable)"); break;
		case 12: printstr("(Integer Overflow)"); break;
		case 13: printstr("(Trap)"); break;
		case 23: printstr("(Reference to Watch address)"); break;
		case 24: printstr("(Machine check)"); break;
		default: printstr("(Unknown)"); break;
	}
	printstr("\n         sp: ");
	printhex32(g_ex_data[1]);
	printstr("\n");
	printstr("Reset MachiKania to contine.");
	while(1) asm("wait");
}
