#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "graphics.h"
#include "iface.h"
#include "fonts.h"
#include "tabctrl.h"
#include "comctrls.h"
#include "dialog.h"
#include "table.h"
#include "tools.h"
#include "action.h"
#include "fonts.h"

#include "guiserv.h"

const unsigned char *guiNormalFont, *guiDisabledFont,*guiExtraFont;
unsigned char dh;

//const struct CGUIConstItem *Selection; // Cursor
const struct CGUIConstItem *ActiveDialog;
struct CDialogCtrl *ActiveDialogSpec;

// Ініціалізація черги на промальовування DrawQueue
void *DrawQueueData[DRAW_QUEUE_MAX];
struct CDeque DrawQueue = 
	{0,0,DRAW_QUEUE_MAX,DrawQueueData};

// Ініціалізація стеку елементів верхнього рівня DialogStack
void *DialogStackData[DIALOG_STACK_MAX];
struct CDeque DialogStack =
	{0,0,DIALOG_STACK_MAX,DialogStackData};

/**********************************************************************************
 *  Ініціалізація сервера.                                                        *
 *  повертає                                                                      *
 *      1 у випадку успішного завершення                                          *
 *      0 інакше                                                                  *
 **********************************************************************************/
int InitGUIServer(void)
{
	guiNormalFont=font8x8;
	guiDisabledFont=font8x8lite;
	guiExtraFont=font8x16;

	dh = 10;

	ActiveDialog = ConstDeskTop+1;
	ActiveDialogSpec = (struct CDialogCtrl *)ActiveDialog->Specific;
	AddToDequeTail((void *)ActiveDialog,&DrawQueue);
	return 1;
}

/**********************************************************************************
 *  Промалювати частину робочого стола, використовуючи дані з DrawQueue           *
 *  та ConstDeskTop                                                               *
 **********************************************************************************/
void DrawGUI(void) 
{
	void *temp;
	const struct CGUIConstItem *Item;
	enum GUIItemKind ItemKind;
	int ItemsDrawn = 0;

	if (!GetFromDequeHead(&temp,&DrawQueue))
		return;
	Item = (const struct CGUIConstItem *)temp;
	while (((ItemKind=Item->Kind) != IDK_END 
		|| !DequeIsEmpty(&DrawQueue))
		&& ItemsDrawn<MAX_ITEMS_TO_DRAW)
	{
		if (Item->Additional->IsVisible)
		{
			switch (ItemKind)
			{
			case IDK_DIALOG:
				DrawDialog(Item);
				return;
				//break;
			case IDK_TABCONTROL:
				DrawTabControl(Item);
				break;
			case IDK_LABEL:
				DrawLabel(Item);
				break;
			case IDK_EDIT:
				DrawEditBox(Item);
				break;
			case IDK_BUTTON:
				DrawButton(Item);
				break;
			case IDK_CHECK:
				DrawCheckBox(Item);
				break;
			case IDK_BMP:
				DrawBmpItem(Item);
				break;
			case IDK_TABLE:
				DrawTable(Item);
				break;
			case IDK_END:
				GetFromDequeHead(&temp,&DrawQueue);
				Item = (const struct CGUIConstItem *)temp;
				continue;
			default:;
			}
			ItemsDrawn++;
		}
		Item++;
	}
	if (ItemsDrawn == MAX_ITEMS_TO_DRAW
		&& ItemKind != IDK_END)
		AddToDequeHead((void *)Item,&DrawQueue);
}

void RefreshGUIItem(const struct CGUIConstItem *Item)
{
	if (ActiveDialogSpec->Cursor==Item)
		SelectGUIItem(Item);
	switch (Item->Kind)
	{
	case IDK_LABEL:
		DrawLabel(Item);
		break;
	case IDK_EDIT:
		DrawEditBox(Item);
		break;
	case IDK_CHECK:
		DrawCheckBox(Item);
		break;
	case IDK_BMP:
		DrawBmpItem(Item);
		break;
	default:;
		// Other Items are not implemented yet;
	}
	if (ActiveDialogSpec->Cursor==Item)
		SelectGUIItem(Item);
}

/**********************************************************************************
 *  Повертає вказівник на ту деталь інтерфейсу, яка містить точку x, y            *
 *  є видима на екрані                                                            *
 **********************************************************************************/
const struct CGUIConstItem *GetGUIItem(unsigned int x, unsigned int y,
									const struct CGUIConstItem *Item)
{
	enum GUIItemKind ItemKind;

	assert(ActiveDialog);
	assert(ActiveDialog->Contained);

	if (Item==NULL)
		Item=ActiveDialog->Contained;
	
	if (!IS_POINT_IN_RECTANGLE(ActiveDialog->Left,ActiveDialog->Top,
		ActiveDialog->Width,ActiveDialog->Height,x,y))
	{
		// Користувач тицьнув поза межами діалога
		RestoreActiveDialogFromStack();
		return Item;
	}

	while ((ItemKind=Item->Kind) != IDK_END)
	{
		if (IS_POINT_IN_RECTANGLE(Item->Left,Item->Top,
			Item->Width,Item->Height,x,y)
			&& Item->Additional->IsVisible
#ifndef GET_DISABLED_BY_TOUCHSCREEN
			&& Item->Additional->IsEnabled
#endif
			)
		{
			switch (ItemKind)
			{
			case IDK_TABCONTROL:
				if (GetTabControlItem(x,y,&Item))
					return Item;
				--Item;
				break;
			case IDK_LABEL:
			case IDK_EDIT:
			case IDK_BUTTON:
			case IDK_CHECK:
			case IDK_BMP:
				return Item;
			case IDK_TABLE:
			case IDK_DIALOG:
				if (!Item->Contained)
					return NULL;
				else
					Item = Item->Contained - 1;
				break;
			case IDK_END:
				return (Item-1)->Owner;
			default:;
				/*
					Something new, an error has occured!
				*/
			}
		}
		++Item;
	}
	// Досягли заглушку, отже треба повернути батьківський контейнер
    return (Item-1)->Owner; 
}

/**********************************************************************************
 *  Візуально виділити курсором деталь інтерфейсу                                 *
 **********************************************************************************/
void SelectGUIItem(const struct CGUIConstItem *Item)
{
	if (Item==NULL)
		return;
	if (!Item->Additional->IsVisible)
		return;
	switch (Item->Kind)
	{
	case IDK_TAB:
		SelectTab(Item);
		break;
#ifdef LABELS_ARE_SELECTABLE
	case IDK_LABEL:
		SelectLabel(Item);
		break;
#endif
	case IDK_EDIT:
		SelectEditBox(Item);
		break;
	case IDK_BUTTON:
		SelectButton(Item);
		break;
	case IDK_CHECK:
		SelectCheckBox(Item);
		break;
	default:;
	}
}

/**********************************************************************************
 *  Встановити курсор на один із елементів інтерфейсу та забезпечити його         *
 *  видимість                                                                     *
 **********************************************************************************/
void SetSelection(const struct CGUIConstItem *Item)
{
	const struct CGUIConstItem *Parent,*p,*r,*toRepaint=NULL;
	void *q;

	// Невидимий елемент не можна виділити, спочатку треба його зробити видимим 
	// у користувацькій частині програми
	if (!Item->Additional->IsVisible)
		return;
#ifndef LABELS_ARE_SELECTABLE
	if (Item->Kind==IDK_LABEL)
		return;
#endif
	Parent = Item->Owner;
	p=Parent;

	// Переконатися у тому, що елемент видно на робочому столі
	while (p)
	{
		switch (p->Kind)
		{
		case IDK_DIALOG:
			if (ActiveDialog!=p)
			{
				StoreActiveDialogInStack();
				ActiveDialog = p;
				toRepaint = p;
			}
			break;
		case IDK_TAB: // Елемент знаходиться на сторінці закладки
			r=p->Owner;
			q=(r->Specific);
			if (((struct CTabControlItem *)q)->Active != p)
			{
				toRepaint = r;
				((struct CTabControlItem *)q)->Active = p;
			}
			p=p->Owner;
			break;
		case IDK_TABCONTROL: // Елемент є закладкою
			q=p->Specific;
			if (((struct CTabControlItem *)q)->Active != Item)
			{
				((struct CTabControlItem *)q)->Active = Item;
				toRepaint = p;
			}
			break;
		default:;
		}
		if (p)
			p=p->Owner; // Перевірка потрібна,
	} //бо можуть бути непослідовні переходи
	
	/*
		Якщо здійснюється перемальовування, то виділяти не треба, встановиться саме,
		проте у протилежному випадку доведеться здійснити це вручну (що, власне,
		і робить наступний оператор)
	*/
	if (toRepaint)
		AddToDequeTail((void *)toRepaint, &DrawQueue);
	else
		SelectGUIItem(Item);
	ActiveDialogSpec=(struct CDialogCtrl *)ActiveDialog->Specific;
	ActiveDialogSpec->Cursor = Item;
}


char *String;
char temp_char,temp_carry;
/**********************************************************************************
 *  Основна функція, яка або промальовує деталі інтерфейсу, або оброб-            *
 *  ляє подію, описану структурою Event                                           *
 **********************************************************************************/
int ProcessGUIServer(const struct CEventDescriptor *Event)
{
	void *q;
	const struct CGUIConstItem *Item;
	unsigned char c=0;
	if (DequeIsEmpty(&DrawQueue))
	{
		//	Обробка подій
		switch (Event->Type)
		{
		case EVENT_TOUCHSCREEN:
			SelectGUIItem(ActiveDialogSpec->Cursor); // Зняття виділення
			Item = GetGUIItem(Event->x,Event->y,ActiveDialog);
			if (Item)
			{
				if (Item->Kind!=IDK_DIALOG)
					ActiveDialogSpec->Cursor = Item;
				else
					ActiveDialogSpec->Cursor = NULL;
			// Первинна реакція:
				switch (Item->Kind)
				{
				case IDK_CHECK:
					// CheckBox інвертує свій стан
					q = Item->Specific;
					if (Item->Additional->IsEnabled)
						((struct CCheckBoxItem *) q)->IsChecked = 
							!((struct CCheckBoxItem *) q)->IsChecked; 
					DrawCheckBox(Item);
				}
			}
			if (ActiveDialogSpec->Cursor)
				ActionPerformed(ActiveDialogSpec->Cursor,Event); 
			// передача керування на "прикладний рівень"
			if (DequeIsEmpty(&DrawQueue))
				SelectGUIItem(ActiveDialogSpec->Cursor); // поставити виділення, 
			return 1;
		case EVENT_KEYBOARD:
			SelectGUIItem(ActiveDialogSpec->Cursor);

			if (!ActiveDialogSpec->Cursor)
			{
				assert(ActiveDialog);
				assert(ActiveDialogSpec);
				ActiveDialogSpec->Cursor = ActiveDialog->Contained;
			}

			switch (Event->key)
			{

			case K_LEFT:
				// Ніколи вибраний елемент не може бути елементом верхнього рівня
				assert(ActiveDialogSpec->Cursor->Owner);
				if (!ActiveDialogSpec->Cursor) 
					ActiveDialogSpec->Cursor = ActiveDialog->Contained;
				// Кому треба порожній діалог?
				assert(ActiveDialogSpec->Cursor);
				if (Item=GetNearestLeftItem(ActiveDialogSpec->Cursor))
					ActiveDialogSpec->Cursor=Item;
				SelectGUIItem(ActiveDialogSpec->Cursor);
				return 1;

			case K_RIGHT:
				assert(ActiveDialogSpec->Cursor->Owner);
				if (!ActiveDialogSpec->Cursor)  
					ActiveDialogSpec->Cursor = ActiveDialog->Contained;
				assert(ActiveDialogSpec->Cursor);
				if (Item=GetNearestRightItem(ActiveDialogSpec->Cursor))
					ActiveDialogSpec->Cursor=Item;
				SelectGUIItem(ActiveDialogSpec->Cursor);
				return 1;

			case K_UP:
				assert(ActiveDialogSpec->Cursor->Owner);
				if (!ActiveDialogSpec->Cursor)  
					ActiveDialogSpec->Cursor = ActiveDialog->Contained;
				assert(ActiveDialogSpec->Cursor);
				if (Item=GetNearestUpperItem(ActiveDialogSpec->Cursor))
					ActiveDialogSpec->Cursor=Item;
				SelectGUIItem(ActiveDialogSpec->Cursor);
				return 1;

			case K_PGUP:
/*				if (ActiveDialogSpec->Cursor 
					&& ActiveDialogSpec->Cursor->Owner
					&& ActiveDialogSpec->Cursor->Owner->Kind!=IDK_DIALOG)
				{
					ActiveDialogSpec->Cursor = ActiveDialogSpec->Cursor->Owner;
				}
				SelectGUIItem(ActiveDialogSpec->Cursor);
				return 1;
*/	
				if (!RestoreActiveDialogFromStack())
					SelectGUIItem(ActiveDialogSpec->Cursor);
				return 1;
					
			case K_DOWN:
				if (!ActiveDialogSpec->Cursor || !ActiveDialogSpec->Cursor->Owner)  
					ActiveDialogSpec->Cursor = ActiveDialog->Contained;
				if (!ActiveDialogSpec->Cursor)
					break;
				if (Item=GetNearestLowerItem(ActiveDialogSpec->Cursor))
					ActiveDialogSpec->Cursor=Item;
				SelectGUIItem(ActiveDialogSpec->Cursor);
				return 1;

			case K_PGDOWN:
				if (ActiveDialogSpec->Cursor && ActiveDialogSpec->Cursor->Kind!=IDK_END)
				{
					if (ActiveDialogSpec->Cursor->Contained)
					{
						ActiveDialogSpec->Cursor = ActiveDialogSpec->Cursor->Contained;
						SetSelection(ActiveDialogSpec->Cursor);
						ActionPerformed(ActiveDialogSpec->Cursor,Event);
						return 1;
					}
					else
					{
						// курсор знаходиться на елементі нижнього рівня, отже
						// треба просто згенерувати відповідну подію
						if (ActiveDialogSpec->Cursor)
							ActionPerformed(ActiveDialogSpec->Cursor,Event);
						if (DequeIsEmpty(&DrawQueue))
							SelectGUIItem(ActiveDialogSpec->Cursor);
						return 1;
					}
				}
				else
				{
					assert(ActiveDialog);
					if (ActiveDialog)
					{
						ActiveDialogSpec->Cursor = ActiveDialog->Contained;
						while (ActiveDialogSpec->Cursor->Kind!=IDK_END
							|| !ActiveDialogSpec->Cursor->Additional->IsVisible
							&& !ActiveDialogSpec->Cursor->Additional->IsEnabled
#ifndef LABELS_ARE_SELECTABLE
							&& ActiveDialogSpec->Cursor->Kind!=IDK_LABEL
#endif
							)
							++ActiveDialogSpec->Cursor;
						if (ActiveDialogSpec->Cursor->Kind==IDK_END)
							ActiveDialogSpec->Cursor = NULL;

						if (DequeIsEmpty(&DrawQueue))
							SelectGUIItem(ActiveDialogSpec->Cursor);
						return 1;
					}
				}
				break;
			case K_0:
				c='0';
			case K_1:
				if (!c)
					c='1';
			case K_2:
				if (!c)
					c='2';
			case K_3:
				if (!c)
					c='3';
			case K_4:
				if (!c)
					c='4';
			case K_5:
				if (!c)
					c='5';
			case K_6:
				if (!c)
					c='6';
			case K_7:
				if (!c)
					c='7';
			case K_8:
				if (!c)
					c='8';
			case K_9:
				if (!c)
					c='9';
				if (ActiveDialogSpec->Cursor && ActiveDialogSpec->Cursor->Kind != IDK_EDIT)
					break;
				q = ActiveDialogSpec->Cursor->Specific;
				String=(char *)((struct CEditBoxItem *)q)->Text;
				temp_char=strlen(String);
				strcpy(String,String+1);
				String[temp_char-1]=c;
				String=strchr(String,'.');
				if (String)
				{
					String[0]=String[1];
					String[1]='.';
				}
				DrawEditBox(ActiveDialogSpec->Cursor);
				break;
			case K_INC:
				if (ActiveDialogSpec->Cursor->Kind != IDK_EDIT)
					break;
				q = ActiveDialogSpec->Cursor->Specific;
				String=(char *)((struct CEditBoxItem *)q)->Text;
				temp_carry=0;
				String+=strlen(String)-1;
				if (*String!='9')
					++(*String);
				else
				{
					temp_carry=1;
					*String='0';
				}
				while (temp_carry && (String > (char *)((struct CEditBoxItem *)q)->Text))
				{
					String--;
					if (*String=='.')
						continue;
					if (*String!='9')
					{
						temp_carry=0;
						++(*String);
					}
					else
					{
						*String='0';
						temp_carry=1;
					}
				}
				DrawEditBox(ActiveDialogSpec->Cursor);
				break;
			case K_DEC:
				if (ActiveDialogSpec->Cursor->Kind != IDK_EDIT)
					break;
				q = ActiveDialogSpec->Cursor->Specific;
				String=(char *)((struct CEditBoxItem *)q)->Text;
				temp_carry=0;
				String+=strlen(String)-1;
				if (*String!='0')
					--(*String);
				else
				{
					temp_carry=1;
					*String='9';
				}
				while (temp_carry && (String > (char *)((struct CEditBoxItem *)q)->Text))
				{
					String--;
					if (*String=='.')
						continue;
					if (*String!='0')
					{
						temp_carry=0;
						--(*String);
					}
					else
					{
						*String='9';
						temp_carry=1;
					}
				}
				DrawEditBox(ActiveDialogSpec->Cursor);
				break;

			}

			if (ActiveDialogSpec->Cursor)
				ActionPerformed(ActiveDialogSpec->Cursor,Event);
			if (DequeIsEmpty(&DrawQueue))
				SelectGUIItem(ActiveDialogSpec->Cursor);
			return 1;
		case EVENT_NULL:;
			return 1;
		}
		return 1;
	}
	else
	{
		/*
			Квант промальовування, розмір якого визначається макросом MAX_ITEMS_TO_DRAW
		*/
		DrawGUI();
		if (DequeIsEmpty(&DrawQueue)) // Промальовування закінчено
		{
			SelectGUIItem(ActiveDialogSpec->Cursor);
		}
		return 0;
	}
}

/*********************************************************************************
 *  Група функцій з даними для їх роботи, які використовуються для навігації по  *
 * діалогові. Роль випливає з назви                                              *
 *********************************************************************************/
#define abs(x) ((x)<0 ? -(x) : x)

const struct CGUIConstItem *ItemToCompare,*ItemToReturn;
unsigned int min_dist, dist;

const struct CGUIConstItem *GetNearestLeftItem(const struct CGUIConstItem *Item)
{
	if (!Item || !Item->Owner)
		return NULL;

	min_dist=SCREEN_WIDTH;
	ItemToReturn=NULL;
	ItemToCompare=Item->Owner->Contained; // Це перший елемент на цьому рівні
	while (ItemToCompare->Kind!=IDK_END)
	{
		if (ItemToCompare!=Item 
			&& ItemToCompare->Additional->IsVisible
			&& ItemToCompare->Additional->IsEnabled
			&& ItemToCompare->Left < Item->Left 
#ifndef LABELS_ARE_SELECTABLE
			&& ItemToCompare->Kind!=IDK_LABEL
#endif
			&& ((dist = abs((signed int)ItemToCompare->Left - (signed int)Item->Left) +
					abs((signed int)ItemToCompare->Top - (signed int)Item->Top)) < min_dist))
		{
			min_dist=dist;
			ItemToReturn = ItemToCompare;
		}
		++ItemToCompare;
	}
	return ItemToReturn;
}

const struct CGUIConstItem *GetNearestRightItem(const struct CGUIConstItem *Item)
{
	if (!Item || !Item->Owner)
		return NULL;

	min_dist=SCREEN_WIDTH;
	ItemToReturn=NULL;
	ItemToCompare=Item->Owner->Contained; // Це перший елемент на цьому рівні
	while (ItemToCompare->Kind!=IDK_END)
	{
		if (ItemToCompare!=Item 
			&& ItemToCompare->Additional->IsVisible
			&& ItemToCompare->Additional->IsEnabled
			&& ItemToCompare->Left > Item->Left 
#ifndef LABELS_ARE_SELECTABLE
			&& ItemToCompare->Kind!=IDK_LABEL
#endif
			&& ((dist = abs((signed int)ItemToCompare->Left - (signed int)Item->Left) +
					abs((signed int)ItemToCompare->Top - (signed int)Item->Top)) < min_dist))
		{
			min_dist=dist;
			ItemToReturn = ItemToCompare;
		}
		++ItemToCompare;
	}
	return ItemToReturn;
}

const struct CGUIConstItem *GetNearestUpperItem(const struct CGUIConstItem *Item)
{
	if (!Item || !Item->Owner)
		return NULL;

	min_dist=SCREEN_HEIGHT;
	ItemToReturn=NULL;
	ItemToCompare=Item->Owner->Contained; // Це перший елемент на цьому рівні
	while (ItemToCompare->Kind!=IDK_END)
	{
		if (ItemToCompare!=Item 
			&& ItemToCompare->Additional->IsVisible
			&& ItemToCompare->Additional->IsEnabled
			&& ItemToCompare->Top < Item->Top 
#ifndef LABELS_ARE_SELECTABLE
			&& ItemToCompare->Kind!=IDK_LABEL
#endif
			&& ((dist = abs((signed int)ItemToCompare->Left - (signed int)Item->Left) +
					abs((signed int)ItemToCompare->Top - (signed int)Item->Top)) < min_dist))
		{
			min_dist=dist;
			ItemToReturn = ItemToCompare;
		}
		++ItemToCompare;
	}
	return ItemToReturn;
}

const struct CGUIConstItem *GetNearestLowerItem(const struct CGUIConstItem *Item)
{
	if (!Item || !Item->Owner)
		return NULL;

	min_dist=SCREEN_HEIGHT;
	ItemToReturn=NULL;
	ItemToCompare=Item->Owner->Contained; // Це перший елемент на цьому рівні
	while (ItemToCompare->Kind!=IDK_END)
	{
		if (ItemToCompare!=Item 
			&& ItemToCompare->Additional->IsVisible
			&& ItemToCompare->Additional->IsEnabled
#ifndef LABELS_ARE_SELECTABLE
			&& ItemToCompare->Kind!=IDK_LABEL
#endif
			&& ItemToCompare->Top > Item->Top 
			&& ((dist = abs((signed int)ItemToCompare->Left - (signed int)Item->Left) +
					abs((signed int)ItemToCompare->Top - (signed int)Item->Top)) < min_dist))
		{
			min_dist=dist;
			ItemToReturn = ItemToCompare;
		}
		++ItemToCompare;
	}
	return ItemToReturn;
}
