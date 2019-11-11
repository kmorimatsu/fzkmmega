// lib_video_megalopa.h
// �e�L�X�g�{�O���t�B�b�N�r�f�I�o�́@PIC32MX370F512H�p�w�b�_�[�t�@�C���@by K.Tanaka

#define X_RES	288 // Graphic�������𑜓x
#define X_RESW	384 // Graphic�������𑜓x�i���C�h���j
#define X_RESZ	256 // Graphic�������𑜓x�itype Z�݊����j
#define Y_RES	216 // Graphic�c�����𑜓x
#define Y_RESZ	224 // Graphic�c�����𑜓x�itype Z�݊����j
#define WIDTH_X	36 // ������������
#define WIDTH_Y	27 // �c����������
#define WIDTH_XW	48 // �������������i���C�h���j
#define WIDTH_XW2	64 // �������������i���C�h�A6�h�b�g�t�H���g���j
#define WIDTH_XBW	80 // �������������i���m�N�����j
#define WIDTH_30	30 // �������������i�݊����[�h8�h�b�g�t�H���g���j
#define WIDTH_40	40 // �������������i�݊����[�h6�h�b�g�t�H���g���j
#define ATTROFFSET	(WIDTH_X*WIDTH_Y) // TVRAM��̃J���[�p���b�g�i�[�ʒu�i�W�����j
#define ATTROFFSETW	(WIDTH_XW*WIDTH_Y) // TVRAM��̃J���[�p���b�g�i�[�ʒu�i���C�h���j
#define ATTROFFSETW2	(WIDTH_XW2*WIDTH_Y) // TVRAM��̃J���[�p���b�g�i�[�ʒu�i���C�h�A6�h�b�g�t�H���g���j
#define ATTROFFSETBW	(WIDTH_XBW*WIDTH_Y) // TVRAM��̃J���[�p���b�g�i�[�ʒu�i���m�N�����j
#define ATTROFFSET30	(WIDTH_30*WIDTH_Y) // TVRAM��̃J���[�p���b�g�i�[�ʒu�i�݊����[�h8�h�b�g�t�H���g���j
#define ATTROFFSET40	(WIDTH_40*WIDTH_Y) // TVRAM��̃J���[�p���b�g�i�[�ʒu�i�݊����[�h6�h�b�g�t�H���g���j
#define WIDTH_XMAX WIDTH_XBW // �������ő啶����

// videomode�l
#define VMODE_T30 0 // �W���e�L�X�g30�����݊����[�h
#define VMODE_STDTEXT 1 // �W���e�L�X�g36�������[�h
#define VMODE_T40 2 // �W���e�L�X�g40�����݊����[�h�i6�h�b�g�t�H���g�j
#define VMODE_WIDETEXT 3 // ���C�h�e�L�X�g48�������[�h
#define VMODE_WIDETEXT6dot 4 // ���C�h�e�L�X�g64�������[�h�i6�h�b�g�t�H���g�j
#define VMODE_MONOTEXT 5 // ���m�N���e�L�X�g80�������[�h
#define VMODE_ZOEAGRPH 16 // type Z�݊��O���t�B�b�N���[�h
#define VMODE_STDGRPH 17 // �W���O���t�B�b�N�{�e�L�X�g36�������[�h
#define VMODE_WIDEGRPH 18 // ���C�h�O���t�B�b�N�{�e�L�X�g48�������[�h

// textomode�l
#define TMODE_T30 0 // �W���e�L�X�g30�����݊����[�h
#define TMODE_STDTEXT 1 // �W���e�L�X�g36�������[�h
#define TMODE_T40 2 // �W���e�L�X�g40�����݊����[�h�i6�h�b�g�t�H���g�j
#define TMODE_WIDETEXT 3 // ���C�h�e�L�X�g48�������[�h
#define TMODE_WIDETEXT6dot 4 // ���C�h�e�L�X�g64�������[�h�i6�h�b�g�t�H���g�j
#define TMODE_MONOTEXT 5 // ���m�N���e�L�X�g80�������[�h

// graphmode�l
#define GMODE_NOGRPH 0 // �O���t�B�b�N�s�g�p
#define GMODE_ZOEAGRPH 1 // type Z�݊��O���t�B�b�N���[�h
#define GMODE_STDGRPH 2 // �W���O���t�B�b�N�{�e�L�X�g36�������[�h
#define GMODE_WIDEGRPH 3 // ���C�h�O���t�B�b�N�{�e�L�X�g48�������[�h

#define KEYPORT PORTD
#define KEYUP 0x0004
#define KEYDOWN 0x0001
#define KEYLEFT 0x0002
#define KEYRIGHT 0x0008
#define KEYSTART 0x0010
#define KEYFIRE 0x0020

extern volatile char drawing;		//�@�\�����Ԓ���-1
extern volatile unsigned short drawcount;		//�@1��ʕ\���I�����Ƃ�1�����B�A�v������0�ɂ���B
							// �Œ�1��͉�ʕ\���������Ƃ̃`�F�b�N�ƁA�A�v���̏���������ʊ��ԕK�v���̊m�F�ɗ��p�B
extern unsigned char *GVRAM; // Graphic�r�f�I������
extern unsigned char TVRAM[]; // Character�r�f�I������
extern const unsigned char FontData[]; //�t�H���g�p�^�[��(���������ɂ�������fontdata[]�ɃR�s�[)
extern const unsigned char FontData2[]; //�t�H���g�p�^�[��6�h�b�g�t�H���g
extern unsigned char videomode,textmode,graphmode; //��ʃ��[�h
extern int twidth,twidthy; //�e�L�X�g�������i���j����сi�c
extern int attroffset; // TVRAM�̃J���[���G���A�ʒu
extern int gwidth,gwidthy; // �O���t�B�b�NX�����𑜓x
extern unsigned char fontdata[]; //�Œ�t�H���g�̈�A����������FontData[]����R�s�[
extern unsigned char *Fontp; //���݂̃t�H���g�p�^�[���̐擪�A�h���X

void start_composite(void); //�J���[�R���|�W�b�g�o�͊J�n
void stop_composite(void); //�J���[�R���|�W�b�g�o�͒�~
void init_composite(void); //�J���[�R���|�W�b�g�o�͏�����
void init_palette(void); //�J���[�p���b�g������
void g_clearscreen(void); //Graphic��ʃN���A
void clearscreen(void); //Character��ʃN���A
void set_palette(unsigned char n,unsigned char b,unsigned char r,unsigned char g); //�J���[�p���b�g�ݒ�
void set_bgcolor(unsigned char b,unsigned char r,unsigned char g); // �o�b�N�O���E���h�J���[�ݒ�
void set_videomode(unsigned char m, unsigned char *gvram); //�r�f�I���[�h�̐؂�ւ�

// �ȉ��� text_graph_library.c ���C�u�������g�p���邽�߂̐錾

// �O���t�B�b�N��ʊ֘A
void g_pset(int x,int y,unsigned int c);
	// (x,y)�̈ʒu�ɃJ���[c�œ_��`��
void g_putbmpmn(int x,int y,char m,char n,const unsigned char bmp[]);
	// ��m*�cn�h�b�g�̃L�����N�^�[�����Wx,y�ɕ\��
	// unsigned char bmp[m*n]�z��ɁA�P���ɃJ���[�ԍ�����ׂ�
	// �J���[�ԍ���0�̕����͓����F�Ƃ��Ĉ���
void g_clrbmpmn(int x,int y,char m,char n);
	// �cm*��n�h�b�g�̃L�����N�^�[����
	// �J���[0�œh��Ԃ�
void g_gline(int x1,int y1,int x2,int y2,unsigned int c);
	// (x1,y1)-(x2,y2)�ɃJ���[c�Ő�����`��
void g_hline(int x1,int x2,int y,unsigned int c);
	// (x1,y)-(x2,y)�̐������C�����J���[c�ō����`��
void g_boxfill(int x1,int y1,int x2,int y2,unsigned int c);
	//���W(x1,y1),(x2,y2)��Ίp���Ƃ���J���[c�œh��ꂽ�����`��`��
	// (x1,y1),(x2,y2)��Ίp���Ƃ���J���[c�œh��ꂽ�����`��`��
void g_circle(int x0,int y0,unsigned int r,unsigned int c);
	// (x0,y0)�𒆐S�ɁA���ar�A�J���[c�̉~��`��
void g_circlefill(int x0,int y0,unsigned int r,unsigned int c);
	// (x0,y0)�𒆐S�ɁA���ar�A�J���[c�œh��ꂽ�~��`��
void g_putfont(int x,int y,unsigned int c,int bc,unsigned char n);
	//8*8�h�b�g�̃A���t�@�x�b�g�t�H���g�\��
	//���W�ix,y)�A�J���[�ԍ�c
	//bc:�o�b�N�O�����h�J���[�A�����̏ꍇ����
	//n:�����ԍ�
void g_printstr(int x,int y,unsigned int c,int bc,unsigned char *s);
	//���W(x,y)����J���[�ԍ�c�ŕ�����s��\���Abc:�o�b�N�O�����h�J���[
void g_printnum(int x,int y,unsigned char c,int bc,unsigned int n);
	//���W(x,y)�ɃJ���[�ԍ�c�Ő��ln��\���Abc:�o�b�N�O�����h�J���[
void g_printnum2(int x,int y,unsigned char c,int bc,unsigned int n,unsigned char e);
	//���W(x,y)�ɃJ���[�ԍ�c�Ő��ln��\���Abc:�o�b�N�O�����h�J���[�Ae���ŕ\��
unsigned int g_color(int x,int y);
//���W(x,y)��VRAM��̌��݂̃p���b�g�ԍ���Ԃ��A��ʊO��0��Ԃ�

//�e�L�X�g��ʊ֘A
extern unsigned char *cursor;
extern unsigned char cursorcolor;
void vramscroll(void);
	//1�s�X�N���[��
void setcursor(unsigned char x,unsigned char y,unsigned char c);
	//�J�[�\���ʒu�ƃJ���[��ݒ�
void setcursorcolor(unsigned char c);
	//�J�[�\���ʒu���̂܂܂ŃJ���[�ԍ���c�ɐݒ�
void printchar(unsigned char n);
	//�J�[�\���ʒu�Ƀe�L�X�g�R�[�hn��1�����\�����A�J�[�\����1�����i�߂�
void printstr(unsigned char *s);
	//�J�[�\���ʒu�ɕ�����s��\��
void printnum(unsigned int n);
	//�J�[�\���ʒu�ɕ����Ȃ�����n��10�i���\��
void printnum2(unsigned int n,unsigned char e);
	//�J�[�\���ʒu�ɕ����Ȃ�����n��e����10�i���\���i�O�̋󂫌������̓X�y�[�X�Ŗ��߂�j
void cls(void);
	//�e�L�X�g��ʂ�0�ŃN���A���A�J�[�\������ʐ擪�Ɉړ�
void startPCG(unsigned char *p,int a);
	// RAM�t�H���g�iPCG�j�̗��p�J�n�Ap���t�H���g�i�[�ꏊ�Aa��0�ȊO�ŃV�X�e���t�H���g���R�s�[
void stopPCG(void);
	// RAM�t�H���g�iPCG�j�̗��p��~
