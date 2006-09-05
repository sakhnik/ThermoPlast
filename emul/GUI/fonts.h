#ifndef fonts_h
#define fonts_h

/*********************************************************\
	������ �� �������.
	� ����������� �������, �������, ���������� ���������
	� ���� ��������� ������ iterface.c, ������� ���� 
	������ ���������� ����������� �������
\*********************************************************/

/*********************************************************************
 *  ������� ��� ������ � ��������.                                  *
 * ������� ������������� ������������� ��������� ����������� ���- *
 * ���������� � ���� ������, �������, ���� ����� �� ������       *
 * ����������, ����� ���� ������� ���� �������, ������ �������������*
 * ��������� �������                                                 *
 *********************************************************************/

//unsigned char GetFontHeight(const unsigned char *font);
//unsigned char GetFontWidth(const unsigned char *font);

/*********************************************************************
 * ������� ������� ������� ����������� ����� � �������             *
 *********************************************************************/
unsigned int GetStringLength(const unsigned char *font, const unsigned char *s);

/********************************************************************
 * ������� ����� ���������� �����.                                  *
 * ���������:                                                       *
 *   x, y -- ����"��� ���������� ����� ��������� ���� ��������-  *
 *           ����, � ���� ����� ���������� �����                    *
 *   str  -- �������� �� ��� �����                                 *
 *   font --  �����, ���� ����� �����������                         *
 ********************************************************************/
void DrawString(unsigned int x, unsigned int y, 
				const unsigned char *str, const unsigned char *font);

/********************************************************************
 * ������� ����� ���������� �����.                                  *
 * ���������:                                                       *
 *   x, y -- ����"��� ���������� ����� ��������� ���� ��������-  *
 *           ����, � ���� ����� ���������� �����                    *
 *   str  -- �������� �� ��� �����                                 *
 *   font --  �����, ���� ����� �����������                         *
 * ��� ����������� ������ �� ��� BitCpy � ����������� ������     *
 * ��������� ��������� �������                                     *
 ********************************************************************/
void DrawStringInv(unsigned int x, unsigned int y, 
				const unsigned char *str, const unsigned char *font);

/*
	���� ������������ ������, ������� 8
*/
extern const unsigned char font8x8[256*8];
extern const unsigned char font8x16[256*16];
extern const unsigned char font8x8lite[256*8];

/*
	���������, ���� ���� ��� �� ����
*/
/*
extern const unsigned char pic_delta[34];
extern const unsigned char pic_cl_small[34];
extern const unsigned char pic_oil[34];
extern const unsigned char c1_1[50];
extern const unsigned char c1_2[50];
extern const unsigned char c1_3[50];
extern const unsigned char c1_4[50];
extern const unsigned char c1_5[50];
extern const unsigned char c1_6_2[50];
extern const unsigned char c2_1[50];
extern const unsigned char c2_2[50];
extern const unsigned char c2_3[50];
extern const unsigned char c2_4[50];
extern const unsigned char help[74];
extern const unsigned char dT_n[26];
extern const unsigned char tahom[26];
extern const unsigned char tahom_clear[26];
extern const unsigned char mold[18];
extern const unsigned char worm[18];
extern const unsigned char clear[74];
extern const unsigned char clear16_24[50];
extern const unsigned char clear24_32[114];
extern const unsigned char nozzle3_1[50];
extern const unsigned char nozzle3_2[50];

extern const unsigned char splash[3602];
*/
#endif
