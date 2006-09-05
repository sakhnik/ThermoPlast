#include "graphics.h"
#include "fonts.h"

/********************************************************************************
 *     Через обмеженість розміру стеку доводиться користуватися глобальними та  *
 *     статичними змінними.                                                     *
 * Гірше, багато змінних використовуються з різною метою та у різних ролях,     *
 * тому треба бути максимально уважним, щоб не плутатися. (геометрія -- це      *
 * уміння правильно мислити на неправильних малюнках                            *
 ********************************************************************************/

unsigned char *Addr, *AddrEnd; // Адреси відеопам"яті для прямого доступу
unsigned char *AddrBegin,*AddrEnd1; // Використовуються у функціях малювання 
									// прямокутників та виведення спрайтів
unsigned char 
	part_length, // допоміжна змінна для різних потреб, наприклад, для
				// збереження довжин різних частин лінії
	mask,		// маска заповнення (не був би екран розбитий на дві
				// половини, не довелося б її використовувати
	buf,		// початок та кінець лінії
	SrcBit, 
	bit_end,

	base,		// зміщення для маски: для верхньої половини екрану 
				// вона рівна 0, для нижньої -- 4
	SrcRow,
	SrcSkip;
signed char
	bit; // біт комірки відеопам"яті, який відповідає пікселю
unsigned int
	x, y, // Використовуються для виведення спрайтів
	xMax, yMax;

/********************************************************************************
 *Функція малює горизонтальну лінію, використовуючи швидкий алгоритм, тобто     *
 *виводячи тетрадами.                                                           *
 *   Параметри:                                                                 *
 *       x,y -- невід’ємні координати першої точки лінії                        *
 *       length -- невід’ємна довжина її.                                       *
 ********************************************************************************/

void hLine(unsigned int x, unsigned int y, unsigned int length)
{
	// Перевірки на розташування лінії в екрані
	if IS_NOT_IN_SCREEN(x,y)
		return;
	if (x+length > SCREEN_WIDTH) 
		length=SCREEN_WIDTH-x; 

	Addr=&VideoRam[ADDRESS(x,y)];
	bit=BIT(x,y);
	mask=(bit<4) ? 0x0f : 0xf0;
	base = (bit>3) ? 4 : 0;
	
	part_length = (bit+1) & 3; // це початок лінії
	// початок, до першої найближчої тетради
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

	// тіло
	AddrEnd = Addr + ((length-part_length) >> 2); 
	for (; Addr < AddrEnd; Addr++)
		(*Addr) |= mask;

	part_length = (length-part_length) & 3;
	// хвіст, те, що залишилося після останньої тетради тіла
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
 *   Функція малює на екрані вертикальну лінію                  *
 *      Параметри:                                              *
 *         x,y -- невід"ємні координати першої точки лінії      *
 *         length -- її невід"ємна довжина                      *
 ****************************************************************/
void vLine(unsigned int x, unsigned int y, unsigned int length)
{
	if IS_NOT_IN_SCREEN(x,y)
		return;
	if (y+length>SCREEN_HEIGHT)
		length = SCREEN_HEIGHT - y;
	
	Addr = VideoRam+ADDRESS(x,y);
	bit = BIT(x,y);
	AddrEnd = VideoRam+ADDRESS(x,y+length-1);  // адреса кінця лінії та
	buf = BIT(x,y+length-1);			// біт кінця лінії використовується для перевірки виходу з циклу
	mask = 1<<bit;

	while (Addr!=AddrEnd || bit!=buf)
	{
		(*Addr) |= mask;
		Addr += SCREEN_QUATERNIONS;
		if (Addr >= VideoRam+SCREEN_TOTAL) // Перехід на другу половину екрана
		{
			Addr -= SCREEN_TOTAL;
			bit += 4;
			mask = mask << 4;
		}
	}
	(*Addr) |= mask; // Останню точку треба поставити самостійно
}

/***************************************************************************
 *  Функція цілком повторює hLine, тільки вона для виведення використовує  *
 *  побітову операцію XOR                                                  *
 ***************************************************************************/
void hLineXor(unsigned int x, unsigned int y, unsigned int length)
{
	// Перевірки на розташування лінії в екрані
	if IS_NOT_IN_SCREEN(x,y)
		return;
	if (x+length > SCREEN_WIDTH) 
		length=SCREEN_WIDTH-x; 

	Addr=&VideoRam[ADDRESS(x,y)];
	bit=BIT(x,y);
	mask=(bit<4) ? 0x0f : 0xf0;
	base = (bit>3) ? 4 : 0;
	
	part_length = (bit+1) & 3; // це початок лінії
	// початок, до першої найближчої тетради
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

	// тіло
	AddrEnd = Addr + ((length-part_length) >> 2); 
	for (; Addr < AddrEnd; Addr++)
		(*Addr) ^= mask;

	part_length = (length-part_length) & 3;
	// хвіст, те, що залишилося після останньої тетради тіла
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
 *     XOR-двійник функції vLine                                    *
 ********************************************************************/
void vLineXor(unsigned int x, unsigned int y, unsigned int length)
{
	if IS_NOT_IN_SCREEN(x,y)
		return;
	if (y+length>SCREEN_HEIGHT)
		length = SCREEN_HEIGHT - y;
	
	Addr = VideoRam+ADDRESS(x,y);
	bit = BIT(x,y);
	AddrEnd = VideoRam+ADDRESS(x,y+length-1);  // адреса кінця лінії та
	buf = BIT(x,y+length-1);			// біт кінця лінії використовується для перевірки виходу з циклу
	mask = 1<<bit;

	while (Addr!=AddrEnd || bit!=buf)
	{
		(*Addr) ^= mask;
		Addr += SCREEN_QUATERNIONS;
		if (Addr >= VideoRam+SCREEN_TOTAL) // Перехід на другу половину екрана
		{
			Addr -= SCREEN_TOTAL;
			bit += 4;
			mask = mask << 4;
		}
	}
	(*Addr) ^= mask; // Останню точку треба поставити самостійно
}

/*********************************************************************
 *  Прямокутник.                                                     *
 *      Параметри:                                                   *
 *          Left, Top -- невід"ємні координати лівого верхнього кута *
 *          Width, Height -- невід"ємні ширина та висота прямокутника*
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
 *    XOR-двійник функції Rectangle                                    *
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
 *    Швидке очищення області екрана                                *
 *        у параметрах вказується прямокутник, який треба очистити  *
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
	// початок, до першої найближчої тетради
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
			if (Addr >= VideoRam+SCREEN_TOTAL) // Перехід на другу половину екрана
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

	// Хвіст
	part_length = (Width-part_length) & 3;
	// хвіст, те, що залишилося після останньої тетради тіла
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
			if (Addr >= VideoRam+SCREEN_TOTAL) // Перехід на другу половину екрана
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
		++AddrEnd; // Це використовується в промальовуванні середньої частини
	}

	if (AddrBegin<AddrEnd1)
	{
		Addr = AddrBegin;
		mask = Top<SCREEN_HALF ? 0xf0 : 0x0f;
		bit_end = (Top+Height-1 < SCREEN_HALF) ? 0xf0 : 0x0f; // У ролі mask_end
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
 *   Інвертування прямокутника                                   *
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
	// початок, до першої найближчої тетради
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
			if (Addr >= VideoRam+SCREEN_TOTAL) // Перехід на другу половину екрана
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

	// Хвіст
	part_length = (Width-part_length) & 3;
	// хвіст, те, що залишилося після останньої тетради тіла
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
			if (Addr >= VideoRam+SCREEN_TOTAL) // Перехід на другу половину екрана
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
		++AddrEnd; // Це використовується в промальовуванні середньої частини
	}

	if (AddrBegin<AddrEnd1)
	{
		Addr = AddrBegin;
		mask = Top<SCREEN_HALF ? 0x0f : 0xf0;
		bit_end = (Top+Height-1 < SCREEN_HALF) ? 0x0f : 0xf0; // У ролі mask_end
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
 *   Виведення пікселя зі вказаними координатами на екран      *
 ***************************************************************/
void DrawPixel(unsigned int x, unsigned int y)
{
	if IS_NOT_IN_SCREEN(x,y)
		return;
	SET_PIXEL(x,y)
}

/*************************************************************
 *    Перевірка пікселя на засвіченість                      *
 *************************************************************/
char IsPixelSet(unsigned int x, unsigned int y)
{
	if IS_NOT_IN_SCREEN(x,y)
		return (char)-1;

	return GET_BIT(VideoRam[ADDRESS(x,y)], BIT(x,y));
}

/**************************************************************
 *    Швидке очищення екрана                                  *
 **************************************************************/
void ClearScreen(void)
{
	unsigned char *Addr = VideoRam, *AddrEnd=VideoRam+SCREEN_TOTAL;
	for (;Addr<AddrEnd;Addr++)
		*Addr=0;
}

/*****************************************************************************
 *    Функція виводить зображення на екран.                                  *
 * Самостійно перевіряє розміщення даних, тобто у випадку розміщення по      *
 * тетрадах використовує більш швидкий алгоритм копіювання тетрадами, а      *
 * не по пікселях.                                                           *
 * Особливість: кожен новий рядок повинен починатися з нового байту в джерелі*
 * Параметри:                                                                *
 *            xDst, yDst -- невід"ємні координати лівого верхнього кута      *
 *                          спрайта на екрані                                *
 *            SrcWidth, SrcHeight -- невід"ємні розміри спрайта              *
 *            SrcData -- Адреса даних спрайту                                *
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
		// Джерело чітко лягає на тетради екрана

		Addr=VideoRam + ADDRESS(xDst,yDst);
		bit=(yDst<SCREEN_HALF) ? 0 : 4;
		base=SCREEN_QUATERNIONS - (SrcWidth >> 2);

		mask = (yDst<SCREEN_HALF) ? 0xf0 : 0x0f;
		// Чи є у джерела хвіст, який не входить у цілу кількість байтів?
		buf=((SrcWidth & 4)!=0);

		for (y=0;y<SrcHeight;y++)
		{
			// Спочатку виводимо всі цілі байти 
			for (x=0;x<(SrcWidth>>3);x++)
			{
				*Addr++ = (((*Addr) & mask) | (((*SrcData) >> 4) << bit));
				*Addr++ = (((*Addr) & mask) | (((*SrcData) &0x0f) << bit));
				SrcData++;
			}

			// тепер видимий хвіст-тетрада
			if (buf)
			{
				*Addr++ = (((*Addr) & mask) | (((*SrcData) >> 4) << bit));
				SrcData++;
			}

			Addr += base; // наступний рядок
			if (Addr >= VideoRam + SCREEN_TOTAL)
			{
				// Досягнуто нижньої половини екрана
				Addr-=SCREEN_TOTAL;
				bit+=4;
				mask = mask >> 4;
			}
		}
	}
	else
	{
		// Загальний випадок
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
 *    Функція виводить інвертоване зображення на екран.                      *
 * Самостійно перевіряє розміщення даних, тобто у випадку розміщення по      *
 * тетрадах використовує більш швидкий алгоритм копіювання тетрадами, а      *
 * не по пікселях.                                                           *
 * Особливість: кожен новий рядок повинен починатися з нового байту в джерелі*
 * Параметри:                                                                *
 *            xDst, yDst -- невід"ємні координати лівого верхнього кута      *
 *                          спрайта на екрані                                *
 *            SrcWidth, SrcHeight -- невід"ємні розміри спрайта              *
 *            SrcData -- Адреса даних спрайту                                *
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
		// Джерело чітко лягає на тетради екрана

		Addr=VideoRam + ADDRESS(xDst,yDst);
		bit=(yDst<SCREEN_HALF) ? 0 : 4;
		base=SCREEN_QUATERNIONS - (SrcWidth >> 2);

		mask = (yDst<SCREEN_HALF) ? 0xf0 : 0x0f;
		// Чи є у джерела хвіст, який не входить у цілу кількість байтів?
		buf=((SrcWidth & 4)!=0);

		for (y=0;y<SrcHeight;y++)
		{
			// Спочатку виводимо всі цілі байти 
			for (x=0;x<(SrcWidth>>3);x++)
			{
				*Addr++ = (((*Addr) & mask) | ((((unsigned char) ~ *SrcData) >> 4) << bit));
				*Addr++ = (((*Addr) & mask) | ((((unsigned char) ~ *SrcData) &0x0f) << bit));
				SrcData++;
			}

			// тепер видимий хвіст-тетрада
			if (buf)
			{
				*Addr++ = (((*Addr) & mask) | ((((unsigned char)~ *SrcData) >> 4) << bit));
				SrcData++;
			}

			Addr += base; // наступний рядок
			if (Addr >= VideoRam + SCREEN_TOTAL)
			{
				// Досягнуто нижньої половини екрана
				Addr-=SCREEN_TOTAL;
				bit+=4;
				mask = mask >> 4;
			}
		}
	}
	else
	{
		// Загальний випадок
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
 *    Функція виводить зображення на екран, використовуючи побітове АЛЕ      *
 * Самостійно перевіряє розміщення даних, тобто у випадку розміщення по      *
 * тетрадах використовує більш швидкий алгоритм копіювання тетрадами, а      *
 * не по пікселях.                                                           *
 * Особливість: кожен новий рядок повинен починатися з нового байту в джерелі*
 * Параметри:                                                                *
 *            xDst, yDst -- невід"ємні координати лівого верхнього кута      *
 *                          спрайта на екрані                                *
 *            SrcWidth, SrcHeight -- невід"ємні розміри спрайта              *
 *            SrcData -- Адреса даних спрайту                                *
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
		// Джерело чітко лягає на тетради екрана

		Addr=VideoRam + ADDRESS(xDst,yDst);
		bit=(yDst<SCREEN_HALF) ? 0 : 4;
		base=SCREEN_QUATERNIONS - (SrcWidth >> 2);

		//mask = (yDst<SCREEN_HALF) ? 0xf0 : 0x0f;
		// Чи є у джерела хвіст, який не входить у цілу кількість байтів?
		buf=((SrcWidth & 4)!=0);

		for (y=0;y<SrcHeight;y++)
		{
			// Спочатку виводимо всі цілі байти 
			for (x=0;x<(SrcWidth>>3);x++)
			{
				*Addr++ |= ((*SrcData) << bit);
				*Addr++ |= (((*SrcData) &0x0f) << bit);
				SrcData++;
			}

			// тепер видимий хвіст-тетрада
			if (buf)
			{
				*Addr++ |= (((*SrcData) >> 4) << bit);
				SrcData++;
			}

			Addr += base; // наступний рядок
			if (Addr >= VideoRam + SCREEN_TOTAL)
			{
				// Досягнуто нижньої половини екрана
				Addr-=SCREEN_TOTAL;
				bit+=4;
				//mask = mask >> 4;
			}
		}
	}
	else
	{
		// Загальний випадок
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

// Формат bmp має особливість: у ньому дані записані знизу догори, тому для нього
// раніше виділялися окремі функції малювання. Проте, поки немає потреби у їх ви-
// користанні, вони не модернізовані.

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
 *  Функція, яка використовується для підтримки старого формату            *
 * У параментрах вказуються координати точки екрана, в якій повинен знахо- *
 * дитися лівий верхній кут бітмапа, та, власне, дані.                     *
 * перші два байти масиву даних повинні містити ширину та висоту зображе-  *
 * ння                                                                     *
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

	// Джерело чітко лягає на тетради екрана

	Addr=VideoRam + ADDRESS(xDst,yDst+SrcHeight);
	bit=(yDst+SrcHeight<SCREEN_HALF) ? 0 : 4;
	base=SCREEN_QUATERNIONS + (SrcWidth >> 2);

	mask = (yDst+SrcHeight<SCREEN_HALF) ? 0xf0 : 0x0f;
	// Чи є у джерела хвіст, який не входить у цілу кількість байтів?
	buf=((SrcWidth & 4)!=0);

	for (y=0;y<SrcHeight;y++)
	{
		// Спочатку виводимо всі цілі байти 
		for (x=0;x<(unsigned)(SrcWidth>>3);x++)
		{
			*Addr++ = (((*Addr) & mask) | ((((unsigned char)~ *SrcData) >> 4) << bit));
			*Addr++ = (((*Addr) & mask) | ((((unsigned char)~ *SrcData) &0x0f) << bit));
			SrcData++;
		}

		// тепер видимий хвіст-тетрада
		if (buf)
		{
			*Addr++ = (((*Addr) & mask) | ((((unsigned char)~ *SrcData) >> 4) << bit));
			SrcData++;
		}

		Addr -= base; // наступний рядок
		if (Addr < VideoRam)
		{
			// Досягнуто нижньої половини екрана
			Addr+=SCREEN_TOTAL;
			bit-=4;
			mask = mask << 4;
		}
		SrcData+=SrcSkip;
	}
}


