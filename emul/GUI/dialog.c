#include <assert.h>

#include "iface.h"
#include "graphics.h"
#include "tools.h"
#include "guiserv.h"
#include "dialog.h"

/*********************************************************
 *  Функція малює діалог.                                *
 *********************************************************/

// Геометричні параметри контейнерів верхнього рівня
unsigned int 
	tliLeft, tliTop, tliWidth, tliHeight; 
struct CGUIConstItem *DialogTemp;

void DrawDialog(const struct CGUIConstItem *Dialog)
{
	assert(Dialog);
	assert(Dialog->Kind == IDK_DIALOG);

	tliLeft   = Dialog->Left,
	tliTop    = Dialog->Top,
	tliWidth  = Dialog->Width,
	tliHeight = Dialog->Height;

	ClearBox(tliLeft,tliTop,tliWidth,tliHeight);
	RectangleSet(tliLeft,tliTop,tliWidth,tliHeight);
	if (Dialog->Contained)
		AddToDequeTail((void *)Dialog->Contained, &DrawQueue);
}

/*****************************************************************************
 *    Зберегти поточний активний діалог у стеку діалогів                     *
 * повертає 1 у випадку удачі, інакше -- 0                                   *
 *****************************************************************************/
int StoreActiveDialogInStack(void)
{
	if (!AddToDequeHead((void *)ActiveDialog,&DialogStack))
	{
		GetFromDequeTail((void *)&DialogTemp,
		  &DialogStack);
		AddToDequeHead((void *)ActiveDialog,&DialogStack);
		return 0;
	}
	return 1;
}

/*****************************************************************************
 *    Відновити активний діалог зі сетку діалогів                            *
 * повертає 1 у випадку удачі, інакше -- 0                                   *
 *****************************************************************************/
int RestoreActiveDialogFromStack(void)
{
	void *temp;
	if (!GetFromDequeHead(&temp,&DialogStack))
		return 0;
	ActiveDialog = (const struct CGUIConstItem *)temp;
	ActiveDialogSpec = (struct CDialogCtrl *)ActiveDialog->Specific;
	AddToDequeTail((void *)ActiveDialog,&DrawQueue);
	return 1;
}

void MakeDialogActive(const struct CGUIConstItem *Dialog)
{
	ActiveDialog=Dialog;
	ActiveDialogSpec=(struct CDialogCtrl *)ActiveDialog->Specific;
	AddToDequeTail((void *)Dialog,&DrawQueue);
}

