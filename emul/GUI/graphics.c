#include "graphics.h"
#include "fonts.h"

/********************************************************************************
 *     ����� ���������� ������ ����� ���������� ������������� ����������� ��  *
 *     ���������� �������.                                                     *
 * ó���, ������ ������ ���������������� � ����� ����� �� � ����� �����,     *
 * ���� ����� ���� ����������� �������, ��� �� ���������. (�������� -- ��      *
 * ����� ��������� ������� �� ������������ ��������                            *
 ********************************************************************************/

unsigned char *Addr, *AddrEnd; // ������ �������"�� ��� ������� �������
unsigned char *AddrBegin,*AddrEnd1; // ���������������� � �������� ��������� 
									// ������������ �� ��������� �������
unsigned char 
	part_length, // �������� ����� ��� ����� ������, ���������, ���
				// ���������� ������ ����� ������ ��
	mask,		// ����� ���������� (�� ��� �� ����� �������� �� ��
				// ��������, �� �������� � �� ���������������
	buf,		// ������� �� ����� ��
	SrcBit, 
	bit_end,

	base,		// ������� ��� �����: ��� ������� �������� ������ 
				// ���� ���� 0, ��� ������ -- 4
	SrcRow,
	SrcSkip;
signed char
	bit; // �� ������ �������"��, ���� ������� ������
unsigned int
	x, y, // ���������������� ��� ��������� �������
	xMax, yMax;

/********************************************************************************
 *������� ����� ������������� ���, �������������� ������� ��������, �����     *
 *�������� ���������.                                                           *
 *   ���������:                                                                 *
 *       x,y -- ���䒺�� ���������� ����� ����� ��                        *
 *       length -- ���䒺��� ������� ��.                                       *
 ********************************************************************************/

void hLine(unsigned int x, unsigned int y, unsigned int length)
{
	// �������� �� ������������ �� � �����
	if IS_NOT_IN_SCREEN(x,y)
		return;
	if (x+length > SCREEN_WIDTH) 
		length=SCREEN_WIDTH-x; 

	Addr=&VideoRam[ADDRESS(x,y)];
	bit=BIT(x,y);
	mask=(bit<4) ? 0x0f : 0xf0;
	base = (bit>3) ? 4 : 0;
	
	part_length = (bit+1) & 3; // �� ������� ��
	// �������, �� ����� ��������� �������
	if (part_length>0) 
	{
		switch (part_length)
		{
		case 1:
			buf=0x01; // 0001
			break;
		case 2:
			buf=0x03; // 0011
			break;
		default: // 3
			buf=0x07; // 0111
		}
		*(Addr++) |= (buf << base);
	}

	// ���
	AddrEnd = Addr + ((length-part_length) >> 2); 
	for (; Addr < AddrEnd; Addr++)
		(*Addr) |= mask;

	part_length = (length-part_length) & 3;
	// ����, ��, �� ���������� ���� �������� ������� ���
	if (part_length>0)
	{
		switch (part_length)
		{
		case 1:
			buf=0x08;  // 8 == 1000
			break;
		case 2:
			buf=0x0c; // 12 == 1100
			break;
		default:
			buf=0x0e; // 14 == 1110
		}
		buf=buf << base;
		*Addr |= buf;
	}
}

/****************************************************************
 *   ������� ����� �� ����� ����������� ���                  *
 *      ���������:                                              *
 *         x,y -- ����"��� ���������� ����� ����� ��      *
 *         length -- �� ����"���� �������                      *
 ****************************************************************/
void vLine(unsigned int x, unsigned int y, unsigned int length)
{
	if IS_NOT_IN_SCREEN(x,y)
		return;
	if (y+length>SCREEN_HEIGHT)
		length = SCREEN_HEIGHT - y;
	
	Addr = VideoRam+ADDRESS(x,y);
	bit = BIT(x,y);
	AddrEnd = VideoRam+ADDRESS(x,y+length-1);  // ������ ���� �� ��
	buf = BIT(x,y+length-1);			// �� ���� �� ��������������� ��� �������� ������ � �����
	mask = 1<<bit;

	while (Addr!=AddrEnd || bit!=buf)
	{
		(*Addr) |= mask;
		Addr += SCREEN_QUATERNIONS;
		if (Addr >= VideoRam+SCREEN_TOTAL) // ������� �� ����� �������� ������
		{
			Addr -= SCREEN_TOTAL;
			bit += 4;
			mask = mask << 4;
		}
	}
	(*Addr) |= mask; // ������� ����� ����� ��������� ���������
}

/***************************************************************************
 *  ������� ������ �������� hLine, ����� ���� ��� ��������� �����������  *
 *  ������� �������� XOR                                                  *
 ***************************************************************************/
void hLineXor(unsigned int x, unsigned int y, unsigned int length)
{
	// �������� �� ������������ �� � �����
	if IS_NOT_IN_SCREEN(x,y)
		return;
	if (x+length > SCREEN_WIDTH) 
		length=SCREEN_WIDTH-x; 

	Addr=&VideoRam[ADDRESS(x,y)];
	bit=BIT(x,y);
	mask=(bit<4) ? 0x0f : 0xf0;
	base = (bit>3) ? 4 : 0;
	
	part_length = (bit+1) & 3; // �� ������� ��
	// �������, �� ����� ��������� �������
	if (part_length>0) 
	{
		switch (part_length)
		{
		case 1:
			buf=0x01; // 0001
			break;
		case 2:
			buf=0x03; // 0011
			break;
		default: // 3
			buf=0x07; // 0111
		}
		*(Addr++) ^= (buf << base);
	}

	// ���
	AddrEnd = Addr + ((length-part_length) >> 2); 
	for (; Addr < AddrEnd; Addr++)
		(*Addr) ^= mask;

	part_length = (length-part_length) & 3;
	// ����, ��, �� ���������� ���� �������� ������� ���
	if (part_length>0)
	{
		switch (part_length)
		{
		case 1:
			buf=0x08;  // 8 == 1000
			break;
		case 2:
			buf=0x0c; // 12 == 1100
			break;
		default:
			buf=0x0e; // 14 == 1110
		}
		buf=buf << base;
		*Addr ^= buf;
	}
}

/********************************************************************
 *     XOR-������ ������� vLine                                    *
 ********************************************************************/
void vLineXor(unsigned int x, unsigned int y, unsigned int length)
{
	if IS_NOT_IN_SCREEN(x,y)
		return;
	if (y+length>SCREEN_HEIGHT)
		length = SCREEN_HEIGHT - y;
	
	Addr = VideoRam+ADDRESS(x,y);
	bit = BIT(x,y);
	AddrEnd = VideoRam+ADDRESS(x,y+length-1);  // ������ ���� �� ��
	buf = BIT(x,y+length-1);			// �� ���� �� ��������������� ��� �������� ������ � �����
	mask = 1<<bit;

	while (Addr!=AddrEnd || bit!=buf)
	{
		(*Addr) ^= mask;
		Addr += SCREEN_QUATERNIONS;
		if (Addr >= VideoRam+SCREEN_TOTAL) // ������� �� ����� �������� ������
		{
			Addr -= SCREEN_TOTAL;
			bit += 4;
			mask = mask << 4;
		}
	}
	(*Addr) ^= mask; // ������� ����� ����� ��������� ���������
}

/*********************************************************************
 *  �����������.                                                     *
 *      ���������:                                                   *
 *          Left, Top -- ����"��� ���������� ����� ��������� ���� *
 *          Width, Height -- ����"��� ������ �� ������ ������������*
 *********************************************************************/
void RectangleSet(const unsigned int Left, const unsigned int Top,
			   const unsigned int Width, const unsigned int Height)
{
	hLine(Left,Top,Width);
	hLine(Left,Top+Height-1,Width);
	vLine(Left,Top+1,Height-2);
	vLine(Left+Width-1,Top+1,Height-2);
}

/***********************************************************************
 *    XOR-������ ������� Rectangle                                    *
 ***********************************************************************/
void RectangleXor(const unsigned int Left, const unsigned int Top,
			   const unsigned int Width, const unsigned int Height)
{
	hLineXor(Left,Top,Width);
	hLineXor(Left,Top+Height-1,Width);
	vLineXor(Left,Top+1,Height-2);
	vLineXor(Left+Width-1,Top+1,Height-2);
}

/********************************************************************
 *    ������ �������� ������ ������                                *
 *        � ���������� ��������� �����������, ���� ����� ��������  *
 ********************************************************************/
void ClearBox(unsigned int Left, unsigned int Top,
			  unsigned int Width, unsigned int Height)
{
	if IS_NOT_IN_SCREEN(Left,Top)
		return;
	if (Left+Width > SCREEN_WIDTH)
		Width = SCREEN_WIDTH - Left;
	if (Top+Height > SCREEN_HEIGHT)
		Height = SCREEN_HEIGHT - Top;

	Addr = ADDRESS(Left,Top)+VideoRam;
	AddrEnd = ADDRESS(Left,Top+Height-1)+VideoRam;
	bit = BIT(Left,Top);
	bit_end = BIT(Left,Top+Height-1);

	part_length = (bit+1) & 3; 
	// �������, �� ����� ��������� �������
	if (part_length>0) 
	{
		AddrBegin = Addr+1;
		switch (part_length)
		{
		case 1:
			buf=0x01; // 0001
			break;
		case 2:
			buf=0x03; // 0011
			break;
		default: // 3
			buf=0x07; // 0111
		}
		buf = Top>SCREEN_HALF ? buf << 4 : buf;
		while (Addr!=AddrEnd || bit!=bit_end)
		{
			*Addr &= ~buf;
			Addr += SCREEN_QUATERNIONS;
			if (Addr >= VideoRam+SCREEN_TOTAL) // ������� �� ����� �������� ������
			{
				Addr -= SCREEN_TOTAL;
				bit += 4;
				buf = buf << 4;
			}

		}
		*Addr &= ~buf;
	}
	else
		AddrBegin = Addr;

	Addr = ADDRESS(Left+Width-1,Top) + VideoRam;
	AddrEnd = ADDRESS(Left+Width-1,Top+Height-1) + VideoRam;
	bit = BIT(Left+Width-1,Top);
	bit_end = BIT(Left+Width-1,Top+Height-1);

	// ����
	part_length = (Width-part_length) & 3;
	// ����, ��, �� ���������� ���� �������� ������� ���
	if (part_length>0)
	{
		AddrEnd1 = Addr;
		switch (part_length)
		{
		case 1:
			buf=0x08;  // 8 == 1000
			break;
		case 2:
			buf=0x0c; // 12 == 1100
			break;
		default:
			buf=0x0e; // 14 == 1110
		}
		buf = Top>SCREEN_HALF ? buf << 4 : buf;
		while (Addr!=AddrEnd || bit!=bit_end)
		{
			*Addr &= ~buf;
			Addr += SCREEN_QUATERNIONS;
			if (Addr >= VideoRam+SCREEN_TOTAL) // ������� �� ����� �������� ������
			{
				Addr -= SCREEN_TOTAL;
				bit += 4;
				buf = buf << 4;
			}
		}
		*Addr &= ~buf;
	}
	else
	{
		AddrEnd1 = Addr+1;
		++AddrEnd; // �� ��������������� � �������������� �������� �������
	}

	if (AddrBegin<AddrEnd1)
	{
		Addr = AddrBegin;
		mask = Top<SCREEN_HALF ? 0xf0 : 0x0f;
		bit_end = (Top+Height-1 < SCREEN_HALF) ? 0xf0 : 0x0f; // � ��� mask_end
		while (Addr<AddrEnd || mask!=bit_end)
		{
			*Addr++ &= mask;
			if (Addr==AddrEnd1)
			{
				AddrBegin += SCREEN_QUATERNIONS;
				AddrEnd1 += SCREEN_QUATERNIONS;
				if (AddrBegin>=SCREEN_TOTAL+VideoRam)
				{
					if (mask==0x0f)
						return;
					mask = mask >> 4;
					AddrBegin -= SCREEN_TOTAL;
					AddrEnd1 -= SCREEN_TOTAL;
				}
				Addr = AddrBegin;
			}
		}
	}
}

/*****************************************************************
 *   ������������ ������������                                   *
 *****************************************************************/
void BoxXor(unsigned int Left, unsigned int Top,
			unsigned int Width, unsigned int Height)
{
	if IS_NOT_IN_SCREEN(Left,Top)
		return;
	if (Left+Width > SCREEN_WIDTH)
		Width = SCREEN_WIDTH - Left;
	if (Top+Height > SCREEN_HEIGHT)
		Height = SCREEN_HEIGHT - Top;

	Addr = ADDRESS(Left,Top)+VideoRam;
	AddrEnd = ADDRESS(Left,Top+Height-1)+VideoRam;
	bit = BIT(Left,Top);
	bit_end = BIT(Left,Top+Height-1);

	part_length = (bit+1) & 3; 
	// �������, �� ����� ��������� �������
	if (part_length>0) 
	{
		AddrBegin = Addr+1;
		switch (part_length)
		{
		case 1:
			buf=0x01; // 0001
			break;
		case 2:
			buf=0x03; // 0011
			break;
		default: // 3
			buf=0x07; // 0111
		}
		buf = Top>SCREEN_HALF ? buf << 4 : buf;
		while (Addr!=AddrEnd || bit!=bit_end)
		{
			*Addr ^= buf;
			Addr += SCREEN_QUATERNIONS;
			if (Addr >= VideoRam+SCREEN_TOTAL) // ������� �� ����� �������� ������
			{
				Addr -= SCREEN_TOTAL;
				bit += 4;
				buf = buf << 4;
			}

		}
		*Addr ^= buf;
	}
	else
		AddrBegin = Addr;

	Addr = ADDRESS(Left+Width-1,Top) + VideoRam;
	AddrEnd = ADDRESS(Left+Width-1,Top+Height-1) + VideoRam;
	bit = BIT(Left+Width-1,Top);
	bit_end = BIT(Left+Width-1,Top+Height-1);

	// ����
	part_length = (Width-part_length) & 3;
	// ����, ��, �� ���������� ���� �������� ������� ���
	if (part_length>0)
	{
		AddrEnd1 = Addr;
		switch (part_length)
		{
		case 1:
			buf=0x08;  // 8 == 1000
			break;
		case 2:
			buf=0x0c; // 12 == 1100
			break;
		default:
			buf=0x0e; // 14 == 1110
		}
		buf = Top>SCREEN_HALF ? buf << 4 : buf;
		while (Addr!=AddrEnd || bit!=bit_end)
		{
			*Addr ^= buf;
			Addr += SCREEN_QUATERNIONS;
			if (Addr >= VideoRam+SCREEN_TOTAL) // ������� �� ����� �������� ������
			{
				Addr -= SCREEN_TOTAL;
				bit += 4;
				buf = buf << 4;
			}
		}
		*Addr ^= buf;
	}
	else
	{
		AddrEnd1 = Addr+1;
		++AddrEnd; // �� ��������������� � �������������� �������� �������
	}

	if (AddrBegin<AddrEnd1)
	{
		Addr = AddrBegin;
		mask = Top<SCREEN_HALF ? 0x0f : 0xf0;
		bit_end = (Top+Height-1 < SCREEN_HALF) ? 0x0f : 0xf0; // � ��� mask_end
		while (Addr<AddrEnd || mask!=bit_end)
		{
			*Addr++ ^= mask;
			if (Addr==AddrEnd1)
			{
				AddrBegin += SCREEN_QUATERNIONS;
				AddrEnd1 += SCREEN_QUATERNIONS;
				if (AddrBegin>=SCREEN_TOTAL+VideoRam)
				{
					mask = mask << 4;
					AddrBegin -= SCREEN_TOTAL;
					AddrEnd1 -= SCREEN_TOTAL;
				}
				Addr = AddrBegin;
			}
		}
	}

}

/***************************************************************
 *   ��������� ������ � ��������� ������������ �� �����      *
 ***************************************************************/
void DrawPixel(unsigned int x, unsigned int y)
{
	if IS_NOT_IN_SCREEN(x,y)
		return;
	SET_PIXEL(x,y)
}

/*************************************************************
 *    �������� ������ �� ����������                      *
 *************************************************************/
char IsPixelSet(unsigned int x, unsigned int y)
{
	if IS_NOT_IN_SCREEN(x,y)
		return (char)-1;

	return GET_BIT(VideoRam[ADDRESS(x,y)], BIT(x,y));
}

/**************************************************************
 *    ������ �������� ������                                  *
 **************************************************************/
void ClearScreen(void)
{
	unsigned char *Addr = VideoRam, *AddrEnd=VideoRam+SCREEN_TOTAL;
	for (;Addr<AddrEnd;Addr++)
		*Addr=0;
}

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
			const unsigned char *SrcData)
{
	if IS_NOT_IN_SCREEN(xDst,yDst)
		return;
	SrcRow = SrcWidth;
	if (SrcWidth + xDst >= SCREEN_WIDTH)
		SrcWidth = SCREEN_WIDTH - xDst;
	if (SrcHeight + yDst >= SCREEN_HEIGHT)
		SrcHeight = SCREEN_HEIGHT - yDst;

	SrcSkip=SrcRow-SrcWidth;

	if ((xDst & 3) == 0 && (SrcWidth & 3) == 0)
	{
		// ������� ����� ���� �� ������� ������

		Addr=VideoRam + ADDRESS(xDst,yDst);
		bit=(yDst<SCREEN_HALF) ? 0 : 4;
		base=SCREEN_QUATERNIONS - (SrcWidth >> 2);

		mask = (yDst<SCREEN_HALF) ? 0xf0 : 0x0f;
		// �� � � ������� ����, ���� �� ������� � ���� ������� �����?
		buf=((SrcWidth & 4)!=0);

		for (y=0;y<SrcHeight;y++)
		{
			// �������� �������� �� ��� ����� 
			for (x=0;x<(SrcWidth>>3);x++)
			{
				*Addr++ = (((*Addr) & mask) | (((*SrcData) >> 4) << bit));
				*Addr++ = (((*Addr) & mask) | (((*SrcData) &0x0f) << bit));
				SrcData++;
			}

			// ����� ������� ����-�������
			if (buf)
			{
				*Addr++ = (((*Addr) & mask) | (((*SrcData) >> 4) << bit));
				SrcData++;
			}

			Addr += base; // ��������� �����
			if (Addr >= VideoRam + SCREEN_TOTAL)
			{
				// ��������� ������ �������� ������
				Addr-=SCREEN_TOTAL;
				bit+=4;
				mask = mask >> 4;
			}
		}
	}
	else
	{
		// ��������� �������
		bit = 7;
		yMax=yDst+SrcHeight;
		xMax=xDst+SrcWidth;
		for (y=yDst; y<yMax; y++)
		{
			for (x=xDst; x<xMax; x++)
			{
				if (y==SCREEN_HALF)
					x=x;
				if (GET_BIT(*SrcData, bit))
					SET_PIXEL(x,y)
				else
					UNSET_PIXEL(x,y)
				bit--;
				if (bit<0)
				{
					bit=7;
					SrcData++;
				}
			}
			if (bit!=7)
			{
				SrcData++;
				bit=7;
			}
		}
	}
}

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
			const unsigned char *SrcData)
{
	if IS_NOT_IN_SCREEN(xDst,yDst)
		return;
	SrcRow = SrcWidth;
	if (SrcWidth + xDst >= SCREEN_WIDTH)
		SrcWidth = SCREEN_WIDTH - xDst;
	if (SrcHeight + yDst >= SCREEN_HEIGHT)
		SrcHeight = SCREEN_HEIGHT - yDst;

	SrcSkip=SrcRow-SrcWidth;

	if ((xDst & 3) == 0 && (SrcWidth & 3) == 0)
	{
		// ������� ����� ���� �� ������� ������

		Addr=VideoRam + ADDRESS(xDst,yDst);
		bit=(yDst<SCREEN_HALF) ? 0 : 4;
		base=SCREEN_QUATERNIONS - (SrcWidth >> 2);

		mask = (yDst<SCREEN_HALF) ? 0xf0 : 0x0f;
		// �� � � ������� ����, ���� �� ������� � ���� ������� �����?
		buf=((SrcWidth & 4)!=0);

		for (y=0;y<SrcHeight;y++)
		{
			// �������� �������� �� ��� ����� 
			for (x=0;x<(SrcWidth>>3);x++)
			{
				*Addr++ = (((*Addr) & mask) | ((((unsigned char) ~ *SrcData) >> 4) << bit));
				*Addr++ = (((*Addr) & mask) | ((((unsigned char) ~ *SrcData) &0x0f) << bit));
				SrcData++;
			}

			// ����� ������� ����-�������
			if (buf)
			{
				*Addr++ = (((*Addr) & mask) | ((((unsigned char)~ *SrcData) >> 4) << bit));
				SrcData++;
			}

			Addr += base; // ��������� �����
			if (Addr >= VideoRam + SCREEN_TOTAL)
			{
				// ��������� ������ �������� ������
				Addr-=SCREEN_TOTAL;
				bit+=4;
				mask = mask >> 4;
			}
		}
	}
	else
	{
		// ��������� �������
		bit = 7;
		yMax=yDst+SrcHeight;
		xMax=xDst+SrcWidth;
		for (y=yDst; y<yMax; y++)
		{
			for (x=xDst; x<xMax; x++)
			{
				if (y==SCREEN_HALF)
					x=x;
				if (!GET_BIT(*SrcData, bit))
					SET_PIXEL(x,y)
				else
					UNSET_PIXEL(x,y)
				bit--;
				if (bit<0)
				{
					bit=7;
					SrcData++;
				}
			}
			if (bit!=7)
			{
				SrcData++;
				bit=7;
			}
		}
	}
}

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
			const unsigned char *SrcData)
{
	if IS_NOT_IN_SCREEN(xDst,yDst)
		return;
	SrcRow = SrcWidth;
	if (SrcWidth + xDst >= SCREEN_WIDTH)
		SrcWidth = SCREEN_WIDTH - xDst;
	if (SrcHeight + yDst >= SCREEN_HEIGHT)
		SrcHeight = SCREEN_HEIGHT - yDst;

	SrcSkip=SrcRow-SrcWidth;

	if ((xDst & 3) == 0 && (SrcWidth & 3) == 0)
	{
		// ������� ����� ���� �� ������� ������

		Addr=VideoRam + ADDRESS(xDst,yDst);
		bit=(yDst<SCREEN_HALF) ? 0 : 4;
		base=SCREEN_QUATERNIONS - (SrcWidth >> 2);

		//mask = (yDst<SCREEN_HALF) ? 0xf0 : 0x0f;
		// �� � � ������� ����, ���� �� ������� � ���� ������� �����?
		buf=((SrcWidth & 4)!=0);

		for (y=0;y<SrcHeight;y++)
		{
			// �������� �������� �� ��� ����� 
			for (x=0;x<(SrcWidth>>3);x++)
			{
				*Addr++ |= ((*SrcData) << bit);
				*Addr++ |= (((*SrcData) &0x0f) << bit);
				SrcData++;
			}

			// ����� ������� ����-�������
			if (buf)
			{
				*Addr++ |= (((*SrcData) >> 4) << bit);
				SrcData++;
			}

			Addr += base; // ��������� �����
			if (Addr >= VideoRam + SCREEN_TOTAL)
			{
				// ��������� ������ �������� ������
				Addr-=SCREEN_TOTAL;
				bit+=4;
				//mask = mask >> 4;
			}
		}
	}
	else
	{
		// ��������� �������
		bit = 7;
		yMax=yDst+SrcHeight;
		xMax=xDst+SrcWidth;
		for (y=yDst; y<yMax; y++)
		{
			for (x=xDst; x<xMax; x++)
			{
				if (y==SCREEN_HALF)
					x=x;
				if (GET_BIT(*SrcData, bit))
					SET_PIXEL(x,y)
				bit--;
				if (bit<0)
				{
					bit=7;
					SrcData++;
				}
			}
			if (bit!=7)
			{
				SrcData++;
				bit=7;
			}
		}
	}
}

/*

// ������ bmp �� ����������: � ����� ��� ������� ����� ������, ���� ��� �����
// ����� ��������� ����� ������� ���������. �����, ���� ���� ������� � �� ��-
// ���������, ���� �� �����������.

void BmpCpy(unsigned int xDst, unsigned int yDst, 
			unsigned int SrcLeft, unsigned int SrcTop,
			unsigned int SrcWidth, unsigned int SrcHeight, unsigned int SrcRow,
			const unsigned char *SrcData)
{
	unsigned int 
		x, y, // Destination coords, used by SET_PIXEL
		SrcSkip, // Unused part of SrcData
		SrcBegin=SrcRow*SrcTop+SrcLeft, // First bit of SrcData to be drawn
		xMax;
	signed char 
		SrcBit=7-SrcBegin&7;// The same, but the bit in byte, not from the beginnig
		//SrcBit0=SrcBit; 

	SrcData+=(SrcBegin>>3); // Skipping the first unused part

	if (xDst+SrcWidth>=SCREEN_WIDTH)
		SrcWidth=SCREEN_WIDTH-xDst;
	if (yDst+SrcHeight>=SCREEN_HEIGHT)
		SrcHeight=SCREEN_HEIGHT-yDst;

	SrcSkip=SrcRow-SrcWidth;
	xMax=xDst+SrcWidth;
	for (y=yDst+SrcHeight; y>=yDst; y--)
	{
		for (x=xDst; x<xMax; x++)
		{
			if (GET_BIT(*SrcData, SrcBit))
				SET_PIXEL(x,y);
			SrcBit--;
			if (SrcBit<0)
			{
				SrcBit=7;
				SrcData++;
			}
		}
		// Can be optimized a little bit, but is there a reason to?
		SrcData+=(SrcSkip >> 3);
		SrcBit-=(SrcSkip & 7);
		if (SrcBit<0) 
		{
			SrcBit+=8;
			SrcData++;
		}
	}
}

*/


/***************************************************************************
 *  �������, ��� ��������������� ��� �������� ������� �������            *
 * � ����������� ���������� ���������� ����� ������, � ��� ������� �����- *
 * ������ ���� ������ ��� ������, ��, ������, ���.                     *
 * ����� ��� ����� ������ ����� ������ ������ ������ �� ������ �������-  *
 * ���                                                                     *
 ***************************************************************************/
void BmpAlignedCpy(unsigned int xDst, unsigned int yDst,
				   const unsigned char *SrcData)
{
	unsigned char SrcWidth=SrcData[0],SrcHeight=SrcData[1];
	if IS_NOT_IN_SCREEN(xDst,yDst)
		return;
	SrcRow = SrcData[0];
	++SrcData;
	++SrcData;

	if (SrcWidth + xDst >= SCREEN_WIDTH)
		SrcWidth = SCREEN_WIDTH - xDst;
	if (SrcHeight + yDst >= SCREEN_HEIGHT)
	{
		SrcData=SrcData+((SrcHeight-SCREEN_HEIGHT + yDst)*SrcRow >> 3);
		SrcHeight = SCREEN_HEIGHT - yDst;
	}

	SrcSkip=(SrcRow-SrcWidth)>>3;

	// ������� ����� ���� �� ������� ������

	Addr=VideoRam + ADDRESS(xDst,yDst+SrcHeight);
	bit=(yDst+SrcHeight<SCREEN_HALF) ? 0 : 4;
	base=SCREEN_QUATERNIONS + (SrcWidth >> 2);

	mask = (yDst+SrcHeight<SCREEN_HALF) ? 0xf0 : 0x0f;
	// �� � � ������� ����, ���� �� ������� � ���� ������� �����?
	buf=((SrcWidth & 4)!=0);

	for (y=0;y<SrcHeight;y++)
	{
		// �������� �������� �� ��� ����� 
		for (x=0;x<(unsigned)(SrcWidth>>3);x++)
		{
			*Addr++ = (((*Addr) & mask) | ((((unsigned char)~ *SrcData) >> 4) << bit));
			*Addr++ = (((*Addr) & mask) | ((((unsigned char)~ *SrcData) &0x0f) << bit));
			SrcData++;
		}

		// ����� ������� ����-�������
		if (buf)
		{
			*Addr++ = (((*Addr) & mask) | ((((unsigned char)~ *SrcData) >> 4) << bit));
			SrcData++;
		}

		Addr -= base; // ��������� �����
		if (Addr < VideoRam)
		{
			// ��������� ������ �������� ������
			Addr+=SCREEN_TOTAL;
			bit-=4;
			mask = mask << 4;
		}
		SrcData+=SrcSkip;
	}
}


