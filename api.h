/*
   This file is provided under the LGPL license ver 2.1.
   Written by Katsumi.
   http://hp.vector.co.jp/authors/VA016157/
   kmorimatsu@users.sourceforge.jp
*/

#ifndef BOOL
	typedef enum _BOOL { FALSE = 0, TRUE } BOOL; 
#endif

#ifndef BYTE
	#define BYTE unsigned char
#endif

#ifndef WORD
	#define WORD unsigned short
#endif

#ifndef DWORD
	#define DWORD unsigned long
#endif

#ifndef UINT16
	#define UINT16 unsigned short
#endif

#ifndef size_t
	#define size_t unsigned int
#endif

#include "interface/keyinput.h"
#include "interface/lib_video_megalopa.h"
#include "interface/ps2keyboard.h"
#include "interface/sdfsio370f.h"
#include "interface/exception.h"
