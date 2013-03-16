#ifndef _GUI_SERVER
#define _GUI_SERVER

#ifdef __cplusplus
#	define EXPORT_CPP extern "C"
#else
#	define EXPORT_CPP
#endif

/*****************************************************************
 *  Модуль, який реалізує функції сервера графічного інтрефейсу. *
 *  Для початку роботи треба ініціалізувати його змінні          *
 *  викликом InitGUIServer(), потім у циклі викликається         *
 *  ProcessGUIServer() з необхідним параметром-характеристикою   *
 *  події (миша або touch-screen, клавіатура).                   *
 *  Сервер промальовує ситуацію не обов"язково за один виклик,   *
 *  проте він гарантує повернення керування у основний цикл      *
 *  за скінченний час, що визначається макропараметром           *
 *  MAX_ITEMS_TO_DRAW у файлі interface.h                        *
 *****************************************************************/

// Чи можуть текстові мітки отримувати курсор?
#define LABELS_ARE_SELECTABLE
#undef LABELS_ARE_SELECTABLE

// Чи можна активізувати недоступні елементи тачскріном?
// При цьому, зрозуміло, деякі елементи можуть бути недосягненними,
// якщо є перекриття з іншими. Вибір цієї опції залежить від бажаної
// функціональності
#define GET_DISABLED_BY_TOUCHSCREEN
//#undef GET_DISABLED_BY_TOUCHSCREEN

/************************************************************************
 *  Повертає вказівник на елемент інтерфейсу, визначивши його з ItemID. *
 *  Макрос розміщений у цьому заголовку, а не в interface.h, тому що    *
 *  останній буде генеруватися компілятором ресурсів                    *
 ************************************************************************/

#define GET_ITEM_WITH_ID(ItemID) (ConstDeskTop + ItemID)

extern const unsigned char *guiNormalFont, *guiDisabledFont,*guiExtraFont;
extern unsigned char dh;
//extern const struct CGUIConstItem *Selection; // Курсор
extern const struct CGUIConstItem *ActiveDialog; // Активний елемент верхнього рівня
											// активним може бути лише один!
extern struct CDialogCtrl *ActiveDialogSpec; // А це його специфічні дані -- курсор
// зроблено 6 листопада 2003 року, для того щоб зберігати положення курсора для кожного
// діалога окремо від інших

// Типи подій
enum EventTypes
{
	EVENT_NULL,
	EVENT_TOUCHSCREEN,
	EVENT_KEYBOARD
};

// Клавіші
enum Keys
{
	K_0, K_1, K_2, K_3, K_4, K_5, K_6, K_7, K_8, K_9,
	K_LEFT, K_RIGHT, K_UP, K_DOWN,
	K_PGDOWN,K_PGUP,
	K_INC,K_DEC
};

// Опис події
struct CEventDescriptor
{
	enum EventTypes Type;
	union
	{
		unsigned int x;
		enum Keys key;
	};
	unsigned int y;
};

extern struct CDeque DrawQueue;
extern struct CDeque DialogStack;

/**********************************************************************************
 *  Ініціалізація сервера.                                                        *
 *  параметри:                                                                    *
 *      NormalFont - шрифт, який використовуватиметься для доступних елементів    *
 *      DisabledFont - шрифт, яким будуть промальовуватися недоступні елементи    *
 *  повертає                                                                      *
 *      1 у випадку успішного завершення                                          *
 *      0 інакше                                                                  *
 **********************************************************************************/
EXPORT_CPP int InitGUIServer(void);

/**********************************************************************************
 *  Промалювати частину робочого стола, використовуючи дані з DrawQueue           *
 *  та ConstDeskTop                                                               *
 **********************************************************************************/
void DrawGUI(void);

void RefreshGUIItem(const struct CGUIConstItem *Item);

/**********************************************************************************
 *  Повертає вказівник на ту деталь інтерфейсу, яка містить точку x, y            *
 *  є видима на екрані                                                            *
 **********************************************************************************/
const struct CGUIConstItem *GetGUIItem(unsigned int x, unsigned int y,
									const struct CGUIConstItem *Item);

/**********************************************************************************
 *  Візуально виділити курсором деталь інтерфейсу                                 *
 **********************************************************************************/
void SelectGUIItem(const struct CGUIConstItem *Item);

/**********************************************************************************
 *  Встановити курсор на один із елементів інтерфейсу та забезпечити його         *
 *  видимість                                                                     *
 **********************************************************************************/
void SetSelection(const struct CGUIConstItem *Item);

/**********************************************************************************
 *  Основна функція, яка або промальовує деталі інтерфейсу, або оброб-            *
 *  ляє подію, описану структурою Event                                           *
 **********************************************************************************/
EXPORT_CPP int ProcessGUIServer(const struct CEventDescriptor *Event);

/*********************************************************************************
 *  Група функцій з даними для їх роботи, які використовуються для навігації по  *
 * діалогові. Роль випливає з назви                                              *
 *********************************************************************************/
const struct CGUIConstItem *GetNearestLeftItem(const struct CGUIConstItem *Item);
const struct CGUIConstItem *GetNearestRightItem(const struct CGUIConstItem *Item);
const struct CGUIConstItem *GetNearestUpperItem(const struct CGUIConstItem *Item);
const struct CGUIConstItem *GetNearestLowerItem(const struct CGUIConstItem *Item);

#undef EXPORT_CPP

#endif
