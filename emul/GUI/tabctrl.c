#include <stdlib.h>
#include <assert.h>

#include "tabctrl.h"
#include "graphics.h"
#include "fonts.h"
#include "iface.h"
#include "guiserv.h"
#include "tools.h"

// Глобальні змінні вводяться для того, щоб зекономити пам"ять стеку
// та час ініціалізації

unsigned char
	hasNext, // Є ще закладки, які не вмістилися у рядок?
	hasPrev; // Чи є такі закладки, які знаходяться перед пешою видимою?
unsigned int
	tcLeft, tcTop, tcWidth, tcHeight, // Координати прямокутника усього TabControl'a
	tcRight, tcDown, tcTextLevel, // Допоміжні геометричні дані
	CurrentX, // Поточна координата по осі абсцис
	xLength, // довжина символьного рядка -- надпису
	RightBound; // права межа чергової закладки
struct CTabControlItem *TabCtrlSpec; // Вказівник на специфічні дані
const struct CGUIConstItem
	*CurrentTab, // Оброблювана закладка
	*ActiveTab; // Активна закладка, тобто та, елементи якої відображаються
// Зображення стрілки ліворуч
const unsigned char PrevArrow[14]=
{
	0x10,0x00,
	0x20,0x00,
	0x7f,0xe0,
	0xff,0xe0,
	0x7f,0xe0,
	0x20,0x00,
	0x10,0x00
};
// Зображення стрілки праворуч
const unsigned char NextArrow[14]=
{
	0x00,0x80,
	0x00,0x40,
	0x7f,0xe0,
	0x7f,0xf0,
	0x7f,0xe0,
	0x00,0x40,
	0x00,0x80
};

// ширина поля для стрілок для перемикання між невидимими закладками
const unsigned int xWidth = 24;

// Перша закладка промальовується трохи не так, як інші, тому її треба виділити
char TabIsFirst;

// Набір допоміжних спрайтів
const unsigned char
	leftCorner[4]={0x30,0x40,0x80,0x80},
	middleCorner[4]={0x10,0xa0,0x40,0x40},
	rightCorner[4]={0x00,0x80,0x40,0x40},
	rightCorner1[4]={0x40,0x20,0x10,0x10};

/***************************************************************
 * Функція відображає закладки на екрані, виходячи з даних,    *
 * що розміщені в ConstDeskTop[]                               *
 ***************************************************************/
int DrawTabControl(const struct CGUIConstItem *TabControl)
{
	assert(TabControl);
	assert(TabControl->Kind == IDK_TABCONTROL);

	hasNext=0;
	hasPrev=0;

	tcLeft=TabControl->Left;
	tcTop=TabControl->Top;
	tcWidth=TabControl->Width;
	tcHeight=TabControl->Height;
	tcRight=tcLeft+tcWidth;
	tcDown=tcTop+tcHeight;
	tcTextLevel=tcTop+2;

	CurrentX=tcLeft;

	TabCtrlSpec = TabControl->Specific;

	CurrentTab = TabCtrlSpec->First;
	ActiveTab = TabCtrlSpec->Active;
	TabIsFirst = 1;

	ClearBox(tcLeft,tcTop,tcWidth,tcHeight);
	BitCpy(tcLeft,tcTop,4,4,leftCorner);
	vLine(tcLeft,tcTop+4,tcHeight - 4);
	hLine(tcLeft,tcTop+tcHeight-1,tcWidth-1);
	vLine(tcRight-2,tcTop+dh,tcHeight-dh);
	hLine(tcLeft+2,tcDown,tcWidth-4);
	vLine(tcRight-1,tcTop+dh+1,tcHeight-dh-2);

	hasPrev = (CurrentTab->Owner->Contained != CurrentTab);

	while (!hasNext &&
		CurrentTab->Kind != IDK_END)
	{
		if (CurrentTab->Additional->IsEnabled)
		{
			xLength = GetStringLength(guiNormalFont,CurrentTab->Caption);
			DrawString(CurrentX+4,tcTextLevel,CurrentTab->Caption,guiNormalFont);
		}
		else
		{
			xLength = GetStringLength(guiDisabledFont,CurrentTab->Caption);
			DrawString(CurrentX+4,tcTextLevel,CurrentTab->Caption,guiDisabledFont);
		}

		RightBound = (CurrentX + xLength + 4);
		if (RightBound>tcRight-xWidth-4)
		{
			hasNext=1;
			RightBound=tcRight-xWidth-4;
		}

		if (!TabIsFirst)
		{
			BitCpy(CurrentX,tcTop,4,4,middleCorner);
		}
		else
			TabIsFirst = 0;

		hLine(CurrentX+4,tcTop,RightBound-CurrentX-4);
		vLine(RightBound+1,tcTop+3,dh-3);

		if (CurrentTab != ActiveTab)
			hLine(CurrentX+1,tcTop+dh,RightBound-CurrentX+1);

		CurrentX = RightBound;
		CurrentTab++;
	}

	if (!hasNext)
	{
		BitCpy(CurrentX,tcTop,4,4,rightCorner);
		hLine(CurrentX+1,tcTop+dh,tcRight-CurrentX-2);
	}

	/*
		Next, Prev.
	*/

	if (hasNext || hasPrev)
	{
		CurrentX=tcRight-xWidth-2;
		ClearBox(CurrentX,tcTop+1,xWidth,dh-1);

		if (hasPrev)
		{
			BitCpy(CurrentX+2,tcTop+2,12,7,PrevArrow);
		}
		if (hasNext)
		{
			BitCpy(CurrentX+12,tcTop+2,12,7,NextArrow);
		}

		BitCpy(tcRight-xWidth-4,tcTop,4,4,leftCorner);
		vLine(CurrentX-2,tcTop+3,dh-3);
		hLine(tcRight-xWidth,tcTop,xWidth-2);
		BitCpy(tcRight-4,tcTop,4,4,rightCorner1);
		vLine(tcRight-1,tcTop+2,dh);
		hLine(CurrentX-2,tcTop+dh,xWidth+2);
	}

	// Нарешті, треба намалювати елементи, що містяться на активній закладці.
	// Тобто, треба просто занести перший елемент у чергу на промальовування
	if (ActiveTab->Contained)
		AddToDequeTail((void *)ActiveTab->Contained,&DrawQueue);
	return 1;
}

/*******************************************************************
 *  Кожен контейнер повинен мати функцію взяття елемента за його   *
 *  екранними координатами                                         *
 *  Повернення 0 у викликаючу функцію сигналізує про те, що пошук  *
 *  треба продовжити з елемента *Item, одиниця ж показує, що бажа- *
 *  ний елемент знаходиться за адресою *Item                       *
 *******************************************************************/
int GetTabControlItem(unsigned int x, unsigned int y,
						const struct CGUIConstItem **Item)
{
	const struct CGUIConstItem *TabControl;

	assert(Item);
	assert(*Item);
	assert((*Item)->Kind == IDK_TABCONTROL);

	hasNext=0;
	hasPrev=0;
	TabControl = *Item;
	tcLeft=TabControl->Left;
	tcTop=TabControl->Top;
	tcWidth=TabControl->Width;
	tcHeight=TabControl->Height;
	tcRight=tcLeft+tcWidth;
	CurrentX=tcLeft;
	TabCtrlSpec = (struct CTabControlItem *)(TabControl->Specific);
	CurrentTab = TabCtrlSpec->First;
	ActiveTab = TabCtrlSpec->Active;

	// Тицьнули посередині активної сторінки, отже, пошук треба продовжити
	// серед її елементів
	if (IS_POINT_IN_RECTANGLE(tcLeft,tcTop+dh,tcWidth,tcHeight-dh,x,y))
	{
		if (ActiveTab->Contained)
		{
			*Item = ActiveTab->Contained;
			return 0; // це просто сигнал про те, що не знайдено, треба продовжити пошук
		}
		else
		{
			*Item = NULL;
			return 1;
		}
	}

	// Тицьнули на одну із закладок, або й на стрілку
	if (IS_POINT_IN_RECTANGLE(tcLeft,tcTop,tcWidth,dh,x,y))
	{
		hasPrev = (CurrentTab->Owner->Contained != CurrentTab);

		while (!hasNext && CurrentTab->Kind != IDK_END)
		{
			if (CurrentTab->Additional->IsEnabled)
			{
				xLength = GetStringLength(guiNormalFont,CurrentTab->Caption);
			}
			else
			{
				xLength = GetStringLength(guiDisabledFont,CurrentTab->Caption);
			}

			RightBound = (CurrentX + xLength + 4);
			if (RightBound>tcRight-xWidth-4)
			{
				hasNext=1;
				RightBound=tcRight-xWidth-4;
			}

			if (IS_POINT_IN_RECTANGLE(CurrentX+2,tcTop,RightBound-CurrentX-4,dh,x,y))
			{
				if (CurrentTab != ActiveTab && CurrentTab->Additional->IsEnabled)
				{
					TabCtrlSpec->Active = CurrentTab;
				}
				if (hasNext)
				{
					++TabCtrlSpec->First;
				}
				AddToDequeTail((void *)TabControl,&DrawQueue);
				*Item = CurrentTab;
				return 1;
			}

			CurrentX = RightBound;

			CurrentTab++;
		}

		if (hasNext || hasPrev)
		{
			CurrentX=tcRight-xWidth-1;
			if (hasPrev && IS_POINT_IN_RECTANGLE(CurrentX-3,tcTop,(xWidth>>1),dh,x,y))
			{
			// Prev
				CurrentTab=TabCtrlSpec->First;
				CurrentTab--;
				if (CurrentTab->Kind==IDK_TAB)
				{
					TabCtrlSpec->First=CurrentTab;
					AddToDequeTail((void *)TabControl,&DrawQueue);
				}
				*Item = NULL;
				return 1;
			}
			if (hasNext && IS_POINT_IN_RECTANGLE(CurrentX-2+(xWidth>>1),tcTop,(xWidth>>1),dh,x,y))
			{
			// Next
				CurrentTab=TabCtrlSpec->First;
				CurrentTab++;
				if (CurrentTab->Kind==IDK_TAB)
				{
					TabCtrlSpec->First=CurrentTab;
					AddToDequeTail((void *)TabControl,&DrawQueue);
				}
				*Item = NULL;
				return 1;
			}
		}
		*Item = NULL;
		return 1;
	}
	*Item = NULL;
	return 1;
}

/********************************************************************
 *   Контейнер не можна виділити цілком, проте його певна частина   *
 * може мати таку властивість. У даному випадку йдеться про         *
 * закладку Tab                                                     *
 ********************************************************************/
void SelectTab(const struct CGUIConstItem *Tab)
{
	const struct CGUIConstItem *TabControl;

	assert(Tab);
	TabControl = Tab->Owner;
	assert(TabControl);
	assert(TabControl->Kind == IDK_TABCONTROL);

	hasNext=0;

	tcLeft=TabControl->Left;
	tcTop=TabControl->Top;
	tcWidth=TabControl->Width;
	tcRight=tcLeft+tcWidth;
	CurrentX=tcLeft;

	TabCtrlSpec = (struct CTabControlItem *)(TabControl->Specific);
	CurrentTab = TabCtrlSpec->First;
	TabIsFirst=1;

	while (!hasNext && CurrentTab->Kind != IDK_END)
	{
		if (CurrentTab->Additional->IsEnabled)
		{
			xLength = GetStringLength(guiNormalFont,CurrentTab->Caption);
		}
		else
		{
			xLength = GetStringLength(guiDisabledFont,CurrentTab->Caption);
		}

		RightBound = (CurrentX + xLength + 4);
		if (RightBound>tcRight-xWidth-4)
		{
			hasNext=1;
			RightBound=tcRight-xWidth-4;
		}

		if (CurrentTab==Tab && CurrentTab->Additional->IsEnabled)
		{
			if (TabIsFirst)
			{
				BoxXor(CurrentX+1,tcTop+2,xLength+4,dh-2);
				hLineXor(CurrentX+2,tcTop+1,xLength+2);
			}
			else
			{
				BoxXor(CurrentX+2,tcTop+2,xLength+3,dh-2);
				hLineXor(CurrentX+3,tcTop+1,xLength+1);
			}
			return;
		}

		TabIsFirst=0;
		CurrentX = RightBound;
		CurrentTab++;
	}
}
