/*
   This file is provided under the LGPL license ver 2.1.
   Written by K.Tanaka & Katsumi
   http://www.ze.em-net.ne.jp/~kenken/index.html
   http://hp.vector.co.jp/authors/VA016157/
*/

#define MEGALOPA
#define SYSVER1 "Megalopa"
#define SYSVER2 "1.4"
#define SYSVERI 0x0140
#define BASVER "KM-1304"

#define INIFILE "MACHIKAM.INI" // �����ݒ�t�@�C��
#define HEXFILE "MACHIKAM.HEX" // ���s��HEX�t�@�C����������ƈ�v�����ꍇ�̓G�f�B�^�N��

#define FILENAME_FLASH_ADDRESS 0x9D07EFF0
#define PIC32MX_RAMSIZE 0x20000
#define PIC32MX_FLASHSIZE 0x80000

void printhex8(unsigned char d);
void printhex16(unsigned short d);
void printhex32(unsigned int d);
