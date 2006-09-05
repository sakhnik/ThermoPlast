#include <assert.h>

#include "iface.h"
#include "graphics.h"
#include "fonts.h"
#include "guiserv.h"
#include "table.h"
#include "tools.h"

unsigned int tLeft,tTop,tWidth,tHeight;
unsigned char i,j,Cols,Rows;

const struct CTableCtrl *TableCtrlSpec;
const struct CGUIConstItem *Cell;

void DrawTable(const struct CGUIConstItem *Table)
{
	assert(Table);
	assert(Table->Kind==IDK_TABLE);

	TableCtrlSpec=((struct CTableCtrl *)(Table->Specific));
	
	tLeft   = Table->Left;
	tTop    = Table->Top;
	tWidth  = Table->Width;
	tHeight = Table->Height;

	ClearBox(tLeft,tTop,tWidth,tHeight);
	RectangleSet(tLeft,tTop,tWidth,tHeight);
	hLine(tLeft+1,tTop+tHeight-2,tWidth-2);
	vLine(tLeft+tWidth-2,tTop+1,tHeight-3);


	Cols = TableCtrlSpec->ColCount;
	Rows = TableCtrlSpec->RowCount;
	
	Cell = Table->Contained;
	AddToDequeTail((void *)Cell,&DrawQueue);
	// Заголовок таблиці
	vLine(Cell->Width+Cell->Left-4,tTop+1,tHeight-3);

	for (i=1;i<Cols;i++)
	{
		++Cell;
		vLine(Cell->Left-2,tTop+1,tHeight-3);
	}

	++Cell;
	hLine(tLeft+1,Cell->Top-3,tWidth-3);

	// друк по рядках
	for (j=1;j<Rows;j++)
	{
		hLine(tLeft+1,Cell->Top-2,tWidth-3);	
		Cell+=Cols;
/*		for (i=1;i<Cols;i++)
		{
			++Cell;
			if (Cell->Additional->IsEnabled)
				DrawLimitedString(Cell->Left+4,Cell->Top+2,Cell->Width-4,Cell->Height-2,
					((struct CEditBoxItem *)(Cell->Specific))->Text,guiNormalFont);
			else
				DrawLimitedString(Cell->Left+4,Cell->Top+2,Cell->Width-4,Cell->Height-2,
					((struct CEditBoxItem *)(Cell->Specific))->Text,guiDisabledFont);
			
		}
		++Cell;*/
	}
}
