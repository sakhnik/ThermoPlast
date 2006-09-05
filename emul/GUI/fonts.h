#ifndef fonts_h
#define fonts_h

/*********************************************************\
	Шрифти та малюнки.
	У майбутньому малюнки, напевне, доведеться перенести
	у файл реалізації модуля iterface.c, оскільки вони 
	повинні включатися компілятором ресурсів
\*********************************************************/

/*********************************************************************
 *  Функції для роботи зі шрифтами.                                  *
 * Оскільки передбачається низькорівневий інтерфейс прикладного про- *
 * грамування у такій системі, імовірно, вони ніколи не будуть       *
 * використані, проте якщо виникне така потреба, просто розкоментуйте*
 * необхідну функцію                                                 *
 *********************************************************************/

//unsigned char GetFontHeight(const unsigned char *font);
//unsigned char GetFontWidth(const unsigned char *font);

/*********************************************************************
 * Функція повертає довжину символьного рядка у пікселях             *
 *********************************************************************/
unsigned int GetStringLength(const unsigned char *font, const unsigned char *s);

/********************************************************************
 * Функція малює символьний рядок.                                  *
 * Параметри:                                                       *
 *   x, y -- невід"ємні координати лівого верхнього кута прямокут-  *
 *           ника, у який треба намалювати рядок                    *
 *   str  -- вказівник на сам рядок                                 *
 *   font --  шрифт, який треба використати                         *
 ********************************************************************/
void DrawString(unsigned int x, unsigned int y, 
				const unsigned char *str, const unsigned char *font);

/********************************************************************
 * Функція малює символьний рядок.                                  *
 * Параметри:                                                       *
 *   x, y -- невід"ємні координати лівого верхнього кута прямокут-  *
 *           ника, у який треба намалювати рядок                    *
 *   str  -- вказівник на сам рядок                                 *
 *   font --  шрифт, який треба використати                         *
 * для прискорення роботи всі тіла BitCpy з необхідними змінами     *
 * перенесені всередину функції                                     *
 ********************************************************************/
void DrawStringInv(unsigned int x, unsigned int y, 
				const unsigned char *str, const unsigned char *font);

/*
	Набір моноширинних шрифтів, шириною 8
*/
extern const unsigned char font8x8[256*8];
extern const unsigned char font8x16[256*16];
extern const unsigned char font8x8lite[256*8];

/*
	піктограми, яких потім тут не буде
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
