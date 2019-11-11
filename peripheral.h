/*
   This program is provided under the LGPL license ver 2.1.
   Written by Katsumi.
   http://hp.vector.co.jp/authors/VA016157/
   kmorimatsu@users.sourceforge.jp
*/

/*
	Environment(machine)-specific basic functions follow 
*/


/*
	_getCode() macro (as inline function) that fetches a code using PC, and increment PC.
	This macro is used more frequently in time than getCode() function, 
	so faster code is required than getCode() function.
*/
extern unsigned char RAM[];
extern unsigned char* VRAM;
extern const unsigned char monitor[];
#define _getCode() readMemory(regPC++)

/*
	RAM and I/O interface functions follows.
*/

UINT8 readMemory(UINT16 addr);
#define writeMemory(a,b) do { RAM[(UINT16)(a)]=(UINT8)(b); } while(0)
UINT8 readIO(UINT8 addrL, UINT8 addrH);
void writeIO(UINT8 addrL, UINT8 addrH, UINT8 data);

/*
	Macros
*/

/*
	CP/M BIOS environment
*/

int cpm_const(void);
unsigned char cpm_conin(void);
void cpm_conout(unsigned char ascii);

/*
	Wrappers for KM-Z80 midi functions follow
*/

void printchar_cur(int cur,char c);
void printstr_cur(int cur,char* str);
void printhex8_cur(int cur,unsigned char d);
void printhex16_cur(int cur,unsigned short d);
void printhex32_cur(int cur,unsigned int d);
extern unsigned char *cursor;void vertical_scroll(void);
