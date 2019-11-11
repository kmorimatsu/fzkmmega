/*
   This file is provided under the LGPL license ver 2.1.
   Written by K.Tanaka
   http://www.ze.em-net.ne.jp/~kenken/index.html
*/

#define BLINKTIME 15
#define CURSORCHAR 0x87
#define CURSORCHAR2 0x80
#define CURSORCOLOR 7

void getcursorchar();
// �J�[�\���_�ŗp�Ɍ��̕����R�[�h��ޔ�

void resetcursorchar();
// �J�[�\�������̕����ɖ߂�

void blinkcursorchar();
// ����I�ɌĂяo�����ƂŃJ�[�\����_�ŕ\��������
// BLINKTIME�œ_�ŊԊu��ݒ�
// ���O��getcursorchar()���Ăяo���Ă���

int lineinput(char *s,int n);
// �L�[���͂��ĕ�����z��s�Ɋi�[
// s�ɏ�������������Ă����ƍŏ��ɕ\�����ĕ�����̍Ō�ɃJ�[�\���ړ�����
// ������������g�p���Ȃ��ꍇ��*s=0�Ƃ��Ă���
// �J�[�\���ʒu��setcursor�֐��Ŏw�肵�Ă���
// �ő啶����n�A�Ō��0���i�[����̂�n+1�o�C�g�̗̈�K�v�A������n�̍ő�l��255
// �߂�l�@Enter�ŏI�������ꍇ0�AESC�ŏI������-1�is�͉󂳂Ȃ��j

unsigned char inputchar(void);
// �L�[�{�[�h����1�L�[���͑҂�
// �߂�l �ʏ핶���̏ꍇASCII�R�[�h�A���̑���0�A�O���[�o���ϐ�vkey�ɉ��z�L�[�R�[�h

unsigned char printinputchar(void);
// �J�[�\���\�����Ȃ���L�[�{�[�h����ʏ핶���L�[���͑҂����A���͂��ꂽ������\��
// �߂�l ���͂��ꂽ������ASCII�R�[�h�A�O���[�o���ϐ�vkey�ɍŌ�ɉ����ꂽ�L�[�̉��z�L�[�R�[�h

unsigned char cursorinputchar(void);
// �J�[�\���\�����Ȃ���L�[�{�[�h����1�L�[���͑҂�
// �߂�l �ʏ핶���̏ꍇASCII�R�[�h�A���̑���0�A�O���[�o���ϐ�vkey�ɉ��z�L�[�R�[�h

extern unsigned char blinkchar,blinkcolor;
extern int blinktimer;
extern int insertmode; //�}�����[�h�F1�A�㏑�����[�h�F0
