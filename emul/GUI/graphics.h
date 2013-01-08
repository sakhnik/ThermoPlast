#ifndef _GRAPHICS
#define _GRAPHICS


#ifdef __cplusplus
#	define EXPORT_CPP extern "C"
#else
#	define EXPORT_CPP
#endif

/********************************************************
 *  Модуль роботи з графікою LCD.                       *
 *  Екран розбитий на дві половини, таким чином         *
 *  старша тетрада кожного біта відеопам"яті відображає *
 *  послідовність із чоирьох пікселів нижньої половини  *
 *  екрана, а молодша (тетрада) -- верхньої.            *
 ********************************************************/

// Фізичні розміри екрану. Для конкретної реалізації треба поміняти
// числа у двох наступних рядках
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Службові дані про екран
#define SCREEN_QUATERNIONS (SCREEN_WIDTH>>2)
#define SCREEN_HALF (SCREEN_HEIGHT>>1)
#define SCREEN_TOTAL (SCREEN_HALF)*(SCREEN_QUATERNIONS)

// відеопам"ять
EXPORT_CPP extern unsigned char VideoRam[SCREEN_TOTAL];

// допоміжні функції та макроси
#define SET_BIT(x, y) (x|=(1<<(y)))
#define UNSET_BIT(x,y) (x &= ~(1<<y))
#define SET_XOR_BIT(x, y) (x^=(1<<(y)))
#define GET_BIT(x, y) ((x & (1<<(y))) != 0)

// Чи знаходиться точка в межах координат екрану?
// x, y мусять бути беззнаковими, бо не виконується перевірка на невід"ємність!
#define IS_NOT_IN_SCREEN(x,y) ((x>=SCREEN_WIDTH)||(y>=SCREEN_HEIGHT))

// екранні координати (x,y) -> (Address, bit) відеопам"яті
#define ADDRESS(x,y) (((y)<<6)+((y)<<4)+((x)>>2)-((y<SCREEN_HALF) ? 0 : (SCREEN_TOTAL)))
#define BIT(x,y) (3 - ((x) & 3) + ((y<SCREEN_HALF) ? 0 : 4))

// Засвічення та погашення пікселя з координатами (x,y)
#define SET_PIXEL(x,y) SET_BIT(VideoRam[ADDRESS(x,y)],BIT(x,y));
#define UNSET_PIXEL(x,y) UNSET_BIT(VideoRam[ADDRESS(x,y)],BIT(x,y));
// Чи належить точка з координатами (x,y)
// прямокутнику (left,top, width, height)?
#define IS_POINT_IN_RECTANGLE(left,top,width,height,x,y) \
	(((x)>=(left))&&((y)>=(top))&&((x)<((left)+(width)))\
	&&((y)<((top)+(height))))

/********************************************************************************
 *Функція малює горизонтальну лінію, використовуючи швидкий алгоритм, тобто     *
 *виводячи тетрадами.                                                           *
 *   Параметри:                                                                 *
 *       x,y -- невід’ємні координати першої точки лінії                        *
 *       length -- невід’ємна довжина її.                                       *
 ********************************************************************************/

void hLine(unsigned int x, unsigned int y, unsigned int length);

/****************************************************************
 *   Функція малює на екрані вертикальну лінію                  *
 *      Параметри:                                              *
 *         x,y -- невід"ємні координати першої точки лінії      *
 *         length -- її невід"ємна довжина                      *
 ****************************************************************/
void vLine(unsigned int x, unsigned int y, unsigned int length);

/***************************************************************************
 *  Функція цілком повторює hLine, тільки вона для виведення використовує  *
 *  побітову операцію XOR                                                  *
 ***************************************************************************/
void hLineXor(unsigned int x, unsigned int y, unsigned int length);

/********************************************************************
 *     XOR-двійник функції vLine                                    *
 ********************************************************************/
void vLineXor(unsigned int x, unsigned int y, unsigned int length);

/*********************************************************************
 *  Прямокутник.                                                     *
 *      Параметри:                                                   *
 *          Left, Top -- невід"ємні координати лівого верхнього кута *
 *          Width, Height -- невід"ємні ширина та висота прямокутника*
 *********************************************************************/
void RectangleSet(const unsigned int Left, const unsigned int Top,
			   const unsigned int Width, const unsigned int Height);

/***********************************************************************
 *    XOR-двійник функції Rectangle                                    *
 ***********************************************************************/
void RectangleXor(const unsigned int Left, const unsigned int Top,
			   const unsigned int Width, const unsigned int Height);

/********************************************************************
 *    Швидке очищення області екрана                                *
 *        у параметрах вказується прямокутник, який треба очистити  *
 ********************************************************************/
void ClearBox(unsigned int Left, unsigned int Top,
			  unsigned int Widht, unsigned int Height);

/*****************************************************************
 *   Інвертування прямокутника                                   *
 *****************************************************************/
void BoxXor(unsigned int Left, unsigned int Top,
			unsigned int Width, unsigned int Height);

/***************************************************************
 *   Виведення пікселя зі вказаними координатами на екран      *
 ***************************************************************/
void DrawPixel(unsigned int x, unsigned int y);

/*************************************************************
 *    Перевірка пікселя на засвіченість                      *
 *************************************************************/
EXPORT_CPP char IsPixelSet(unsigned int x, unsigned int y);

/**************************************************************
 *    Швидке очищення екрана                                  *
 **************************************************************/
EXPORT_CPP void ClearScreen(void);


//////////////////////// Bitmap & font functions //////////////////////

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
			const unsigned char *SrcData);

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
			const unsigned char *SrcData);

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
			const unsigned char *SrcData);

/*
	Операції з бітмапами поки не потрібні, тому вони не введені
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
