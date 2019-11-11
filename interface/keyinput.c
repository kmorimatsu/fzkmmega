/*
   This file is provided under the LGPL license ver 2.1.
   Written by K.Tanaka
   http://www.ze.em-net.ne.jp/~kenken/index.html
*/
// �L�[���́A�J�[�\���\���֘A�@�\ by K.Tanaka
// PS/2�L�[�{�[�h���̓V�X�e���A�J���[�e�L�X�g�o�̓V�X�e�����p

#include "lib_video_megalopa.h"
#include "ps2keyboard.h"
#include "keyinput.h"

unsigned char lineinputbuf[256]; //lineinput�֐��p�ꎞ�o�b�t�@
unsigned char blinkchar,blinkcolor;
int blinktimer;
int insertmode; //�}�����[�h�F1�A�㏑�����[�h�F0

void getcursorchar(){
// �J�[�\���_�ŗp�Ɍ��̕����R�[�h��ޔ�
	blinkchar=*cursor;
	blinkcolor=*(cursor+attroffset);
}
void resetcursorchar(){
// �J�[�\�������̕����ɖ߂�
	*cursor=blinkchar;
	*(cursor+attroffset)=blinkcolor;
}
void blinkcursorchar(){
// ����I�ɌĂяo�����ƂŃJ�[�\����_�ŕ\��������
// BLINKTIME�œ_�ŊԊu��ݒ�
// ���O��getcursorchar()���Ăяo���Ă���
	blinktimer++;
	if(blinktimer>=BLINKTIME*2) blinktimer=0;
	if(blinktimer<BLINKTIME){
		if(insertmode) *cursor=CURSORCHAR;
		else *cursor=CURSORCHAR2;
		*(cursor+attroffset)=CURSORCOLOR;
	}
	else{
		*cursor=blinkchar;
		*(cursor+attroffset)=blinkcolor;
	}
}

unsigned char inputchar(void){
// �L�[�{�[�h����1�L�[���͑҂�
// �߂�l �ʏ핶���̏ꍇASCII�R�[�h�A���̑���0�A�O���[�o���ϐ�vkey�ɉ��z�L�[�R�[�h
	unsigned char k;
	unsigned short d;
	d=drawcount;
	while(1){
		while(d==drawcount) asm("wait"); //60����1�b�E�F�C�g
		d=drawcount;
		k=ps2readkey();  //�L�[�o�b�t�@����ǂݍ��݁Ak1:�ʏ핶�����͂̏ꍇASCII�R�[�h
		if(vkey) return k;
	}
}

unsigned char cursorinputchar(void){
// �J�[�\���\�����Ȃ���L�[�{�[�h����1�L�[���͑҂�
// �߂�l �ʏ핶���̏ꍇASCII�R�[�h�A���̑���0�A�O���[�o���ϐ�vkey�ɉ��z�L�[�R�[�h
	unsigned char k;
	unsigned short d;
	getcursorchar(); //�J�[�\���ʒu�̕�����ޔ��i�J�[�\���_�ŗp�j
	d=drawcount;
	while(1){
		while(d==drawcount) asm("wait"); //60����1�b�E�F�C�g
		d=drawcount;
		blinkcursorchar(); //�J�[�\���_�ł�����
		k=ps2readkey();  //�L�[�o�b�t�@����ǂݍ��݁Ak1:�ʏ핶�����͂̏ꍇASCII�R�[�h
		if(vkey) break;  //�L�[�������ꂽ�ꍇ���[�v���甲����
	}
	resetcursorchar(); //�J�[�\�������̕����\���ɖ߂�
	return k;
}

unsigned char printinputchar(void){
// �J�[�\���\�����Ȃ���L�[�{�[�h����ʏ핶���L�[���͑҂����A���͂��ꂽ������\��
// �߂�l ���͂��ꂽ������ASCII�R�[�h�A�O���[�o���ϐ�vkey�ɍŌ�ɉ����ꂽ�L�[�̉��z�L�[�R�[�h
	unsigned char k;
	while(1){
		k=cursorinputchar();
		if(k) break;
	}
	printchar(k);
	return k;
}

int lineinput(char *s,int n){
// �L�[���͂��ĕ�����z��s�Ɋi�[
// s�ɏ�������������Ă����ƍŏ��ɕ\�����ĕ�����̍Ō�ɃJ�[�\���ړ�����
// ������������g�p���Ȃ��ꍇ��*s=0�Ƃ��Ă���
// �J�[�\���ʒu��setcursor�֐��Ŏw�肵�Ă���
// �ő啶����n�A�Ō��0���i�[����̂�n+1�o�C�g�̗̈�K�v�A������n�̍ő�l��255
// �߂�l�@Enter�ŏI�������ꍇ0�AESC�ŏI������-1�is�͉󂳂Ȃ��j
//
	unsigned char *ps,*pd,*pc;
	unsigned char k1,k2;
	int i;

	if(n>255) return -1;
	ps=s;
	pd=lineinputbuf;
	i=0;
	//�����������lineinputbuf�ɃR�s�[���A��������i�ɓ����
	while(*ps!=0 && i<n){
		*pd++=*ps++;
		i++;
	}
	*pd=0;//�o�b�t�@���̕�����Ō��0
	pc=pd;//���݂̕������͈ʒu�|�C���^�i�Ō���j
	if(i>0) printstr(lineinputbuf); //����������\��
	while(1){
		k1=cursorinputchar(); //�J�[�\���\�����Ȃ���1�L�[���͑҂�
		k2=(unsigned char)vkey; //k2:���z�L�[�R�[�h
		if(k1){
			//�ʏ핶���̏ꍇ
			if(insertmode || *pc==0){
				//�}�����[�h�܂��͍Ō���̏ꍇ
				if(i==n) continue; //���͕������ő�l�̏ꍇ����
				for(pd=lineinputbuf+i;pd>=pc;pd--) *(pd+1)=*pd; //1�������}��
				i++;
			}
			*pc=k1; //���͕�����ǉ�
			printstr(pc); //���͕����ȍ~��\��
			pc++;
			for(ps=lineinputbuf+i;ps>pc;ps--) cursor--; //�J�[�\���ʒu�߂�
		}
		else switch(k2){
			//���䕶���̏ꍇ
			case VK_LEFT:
			case VK_NUMPAD4:
				//�����L�[
				if(pc>lineinputbuf){
					pc--;
					cursor--;
				}
				break;
			case VK_RIGHT:
			case VK_NUMPAD6:
				//�E���L�[
				if(*pc!=0){
					pc++;
					cursor++;
				}
				break;
			case VK_RETURN: //Enter�L�[
			case VK_SEPARATOR: //�e���L�[��Enter
				//���͗p�o�b�t�@����Ăяo�����̃o�b�t�@�ɃR�s�[���ďI��
				printchar('\n');
				ps=lineinputbuf;
				pd=s;
				while(*ps!=0) *pd++=*ps++;
				*pd=0;
				return 0;
			case VK_HOME:
			case VK_NUMPAD7:
				//Home�L�[�A������擪�ɃJ�[�\���ړ�
				while(pc>lineinputbuf){
					pc--;
					cursor--;
				}
				break;
			case VK_END:
			case VK_NUMPAD1:
				//End�L�[�A������Ō���ɃJ�[�\���ړ�
				while(*pc!=0){
					pc++;
					cursor++;
				}
				break;
			case VK_BACK:
				//Back Space�L�[�A1�������Ɉړ���Delete����
				if(pc==lineinputbuf) break;//�J�[�\�����擪�̏ꍇ�A����
				pc--;
				cursor--;
			case VK_DELETE:
			case VK_DECIMAL:
				//Delete�L�[�A�J�[�\���ʒu��1�����폜
				if(*pc==0) break;//�J�[�\�����Ō���̏ꍇ�A����
				for(pd=pc;*(pd+1)!=0;pd++) *pd=*(pd+1);
				*pd=0;
				i--;
				printstr(pc);
				printchar(0);//NULL�����\��
				for(ps=lineinputbuf+i+1;ps>pc;ps--) cursor--;
				break;
			case VK_INSERT:
			case VK_NUMPAD0:
				//Insert�L�[�A�}�����[�h�g�O������
				insertmode^=1;
				break;
			case VK_ESCAPE:
			case VK_CANCEL:
				//ESC�L�[�܂���CTRL+Break�L�[�A-1�ŏI��
				return -1;
		}
	}
}
