#ifndef _GRAPHICS
#define _GRAPHICS


#ifdef __cplusplus
#	define EXPORT_CPP extern "C"
#else
#	define EXPORT_CPP
#endif

/********************************************************
 *  ������ ������ � �������� LCD.                       *
 *  ����� �������� �� �� ��������, ����� �����         *
 *  ������ ������� ������� ��� �������"�� �������� *
 *  ����������� �� ������� ������ ������ ��������  *
 *  ������, � ������� (�������) -- �������.            *
 ********************************************************/

// Գ���� ������ ������. ��� ��������� ��������� ����� �������
// ����� � ���� ��������� ������
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// ������� ��� ��� �����
#define SCREEN_QUATERNIONS (SCREEN_WIDTH>>2)
#define SCREEN_HALF (SCREEN_HEIGHT>>1)
#define SCREEN_TOTAL (SCREEN_HALF)*(SCREEN_QUATERNIONS)

// �������"���
EXPORT_CPP extern unsigned char VideoRam[SCREEN_TOTAL];

// ������� ������� �� �������
#define SET_BIT(x, y) (x|=(1<<(y)))
#define UNSET_BIT(x,y) (x &= ~(1<<y))
#define SET_XOR_BIT(x, y) (x^=(1<<(y)))
#define GET_BIT(x, y) ((x & (1<<(y))) != 0)

// �� ����������� ����� � ����� ��������� ������?
// x, y ������ ���� ������������, �� �� ���������� �������� �� ����"������!
#define IS_NOT_IN_SCREEN(x,y) ((x>=SCREEN_WIDTH)||(y>=SCREEN_HEIGHT))

// ������ ���������� (x,y) -> (Address, bit) �������"��
#define ADDRESS(x,y) (((y)<<6)+((y)<<4)+((x)>>2)-((y<SCREEN_HALF) ? 0 : (SCREEN_TOTAL)))
#define BIT(x,y) (3 - ((x) & 3) + ((y<SCREEN_HALF) ? 0 : 4))

// ��������� �� ��������� ������ � ������������ (x,y)
#define SET_PIXEL(x,y) SET_BIT(VideoRam[ADDRESS(x,y)],BIT(x,y));
#define UNSET_PIXEL(x,y) UNSET_BIT(VideoRam[ADDRESS(x,y)],BIT(x,y));
// �� �������� ����� � ������������ (x,y)
// ������������ (left,top, width, height)?
#define IS_POINT_IN_RECTANGLE(left,top,width,height,x,y) \
	(((x)>=(left))&&((y)>=(top))&&((x)<((left)+(width)))\
	&&((y)<((top)+(height))))

/********************************************************************************
 *������� ����� ������������� ���, �������������� ������� ��������, �����     *
 *�������� ���������.                                                           *
 *   ���������:                                                                 *
 *       x,y -- ���䒺�� ���������� ����� ����� ��                        *
 *       length -- ���䒺��� ������� ��.                                       *
 ********************************************************************************/

void hLine(unsigned int x, unsigned int y, unsigned int length);

/****************************************************************
 *   ������� ����� �� ����� ����������� ���                  *
 *      ���������:                                              *
 *         x,y -- ����"��� ���������� ����� ����� ��      *
 *         length -- �� ����"���� �������                      *
 ****************************************************************/
void vLine(unsigned int x, unsigned int y, unsigned int length);

/***************************************************************************
 *  ������� ������ �������� hLine, ����� ���� ��� ��������� �����������  *
 *  ������� �������� XOR                                                  *
 ***************************************************************************/
void hLineXor(unsigned int x, unsigned int y, unsigned int length);

/********************************************************************
 *     XOR-������ ������� vLine                                    *
 ********************************************************************/
void vLineXor(unsigned int x, unsigned int y, unsigned int length);

/*********************************************************************
 *  �����������.                                                     *
 *      ���������:                                                   *
 *          Left, Top -- ����"��� ���������� ����� ��������� ���� *
 *          Width, Height -- ����"��� ������ �� ������ ������������*
 *********************************************************************/
void RectangleSet(const unsigned int Left, const unsigned int Top,
			   const unsigned int Width, const unsigned int Height);

/***********************************************************************
 *    XOR-������ ������� Rectangle                                    *
 ***********************************************************************/
void RectangleXor(const unsigned int Left, const unsigned int Top,
			   const unsigned int Width, const unsigned int Height);

/********************************************************************
 *    ������ �������� ������ ������                                *
 *        � ���������� ��������� �����������, ���� ����� ��������  *
 ********************************************************************/
void ClearBox(unsigned int Left, unsigned int Top,
			  unsigned int Widht, unsigned int Height);

/*****************************************************************
 *   ������������ ������������                                   *
 *****************************************************************/
void BoxXor(unsigned int Left, unsigned int Top,
			unsigned int Width, unsigned int Height);

/***************************************************************
 *   ��������� ������ � ��������� ������������ �� �����      *
 ***************************************************************/
void DrawPixel(unsigned int x, unsigned int y);

/*************************************************************
 *    �������� ������ �� ����������                      *
 *************************************************************/
EXPORT_CPP char IsPixelSet(unsigned int x, unsigned int y);

/**************************************************************
 *    ������ �������� ������                                  *
 **************************************************************/
EXPORT_CPP void ClearScreen(void);


//////////////////////// Bitmap & font functions //////////////////////

/*****************************************************************************
 *    ������� �������� ���������� �� �����.                                  *
 * ��������� �������� ��������� �����, ����� � ������� ��������� ��      *
 * �������� ����������� ���� ������� �������� ��������� ���������, �      *
 * �� �� �������.                                                           *
 * ����������: ����� ����� ����� ������� ���������� � ������ ����� � ������*
 * ���������:                                                                *
 *            xDst, yDst -- ����"��� ���������� ����� ��������� ����      *
 *                          ������� �� �����                                *
 *            SrcWidth, SrcHeight -- ����"��� ������ �������              *
 *            SrcData -- ������ ����� �������                                *
 *****************************************************************************/
void BitCpy(unsigned int xDst, unsigned int yDst,
			unsigned int SrcWidth, unsigned int SrcHeight,
			const unsigned char *SrcData);

/*****************************************************************************
 *    ������� �������� ����������� ���������� �� �����.                      *
 * ��������� �������� ��������� �����, ����� � ������� ��������� ��      *
 * �������� ����������� ���� ������� �������� ��������� ���������, �      *
 * �� �� �������.                                                           *
 * ����������: ����� ����� ����� ������� ���������� � ������ ����� � ������*
 * ���������:                                                                *
 *            xDst, yDst -- ����"��� ���������� ����� ��������� ����      *
 *                          ������� �� �����                                *
 *            SrcWidth, SrcHeight -- ����"��� ������ �������              *
 *            SrcData -- ������ ����� �������                                *
 *****************************************************************************/
void BitCpyInv(unsigned int xDst, unsigned int yDst,
			unsigned int SrcWidth, unsigned int SrcHeight,
			const unsigned char *SrcData);

/*****************************************************************************
 *    ������� �������� ���������� �� �����, �������������� ������� ���      *
 * ��������� �������� ��������� �����, ����� � ������� ��������� ��      *
 * �������� ����������� ���� ������� �������� ��������� ���������, �      *
 * �� �� �������.                                                           *
 * ����������: ����� ����� ����� ������� ���������� � ������ ����� � ������*
 * ���������:                                                                *
 *            xDst, yDst -- ����"��� ���������� ����� ��������� ����      *
 *                          ������� �� �����                                *
 *            SrcWidth, SrcHeight -- ����"��� ������ �������              *
 *            SrcData -- ������ ����� �������                                *
 *****************************************************************************/
void BitCpyOr(unsigned int xDst, unsigned int yDst,
			unsigned int SrcWidth, unsigned int SrcHeight,
			const unsigned char *SrcData);

/*
	�������� � �������� ���� �� ������, ���� ���� �� ������
*/
/*
void BmpCpy(unsigned int xDst, unsigned int yDst,
			unsigned int SrcLeft, unsigned int SrcTop,
			unsigned int SrcWidth, unsigned int SrcHeight, unsigned int SrcRow,
			const unsigned char *SrcData);
*/
/*
	Draws quaternions-aligned bitmap reversing it along Y-axis
*/
/*void BmpAlignedCpy(unsigned int xDst, unsigned int yDst,
				   unsigned int SrcWidth, unsigned int SrcHeight,
				   const unsigned char *SrcData);
*/

void BmpAlignedCpy(unsigned int xDst, unsigned int yDst,
				   const unsigned char *SrcData);

#undef EXPORT_CPP

#endif
