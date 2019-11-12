/*
   This file is provided under the LGPL license ver 2.1.
   Written by K.Tanaka & Katsumi
   http://www.ze.em-net.ne.jp/~kenken/index.html
   http://hp.vector.co.jp/authors/VA016157/
*/

#define FILENAME_FLASH_ADDRESS 0x9D07EFF0
#define PIC32MX_RAMSIZE 0x20000
#define PIC32MX_FLASHSIZE 0x80000

extern unsigned char RAM[65536];
extern char g_ide_filename[13];

void printhex8(unsigned char d);
void printhex16(unsigned short d);
void printhex32(unsigned int d);
int coretimer(void);
void wait_msec(int msec);
void wait60thsec(unsigned short n);
