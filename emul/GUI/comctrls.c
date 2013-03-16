#include <stdlib.h>
#include <assert.h>

#include "fonts.h"
#include "comctrls.h"
#include "graphics.h"
#include "iface.h"
#include "guiserv.h"

unsigned int
	siLeft, siTop, siWidth, siHeight;

void DrawLabel(const struct CGUIConstItem *Label)
{
	assert(Label);
	assert(Label->Kind == IDK_LABEL);

	siLeft=Label->Left;
	siTop=Label->Top;
	siWidth=Label->Width;
	siHeight=Label->Height;
	if (Label->Additional->IsEnabled)
	{
		ClearBox(siLeft+1,siTop+1,siWidth-2,siHeight-2);
		if (Label->Additional->IsExtra)
			DrawString(siLeft,siTop,((struct CEditBoxItem *)(Label->Specific))->Text,guiExtraFont);
		else
			DrawString(siLeft,siTop,((struct CEditBoxItem *)(Label->Specific))->Text,guiNormalFont);
	}
	else
		if (Label->Additional->IsExtra)
			DrawString(siLeft,siTop,Label->Caption,guiExtraFont);
		else
			DrawString(siLeft,siTop,Label->Caption,guiDisabledFont);
}

void SelectLabel(const struct CGUIConstItem *Label)
{
	assert(Label->Kind == IDK_LABEL);

	if (!Label->Additional->IsEnabled)
		return;

	siWidth=GetStringLength(guiNormalFont,((struct CEditBoxItem *)(Label->Specific))->Text);
	if (siWidth>Label->Width)
		siWidth=Label->Width;
	hLineXor(Label->Left,Label->Top+Label->Height-1,siWidth);
}

////////////////// EditBox /////////////////////

void DrawEditBox(const struct CGUIConstItem *Edit)
{
	assert(Edit);
	assert(Edit->Kind == IDK_EDIT);

	siLeft=Edit->Left;
	siTop=Edit->Top;
	siWidth=Edit->Width;
	siHeight=Edit->Height;

	ClearBox(siLeft+1,siTop+1,siWidth-2,siHeight-1);
	// EditBox використовується також для промальовування комірок таблиці,
	// а для них не треба рамка
	if (Edit->Owner->Kind!=IDK_TABLE)
		RectangleSet(siLeft,siTop,siWidth+1,siHeight+1);
	if (Edit->Additional->IsEnabled)
		DrawString(siLeft+4,siTop+2,((struct CEditBoxItem *)(Edit->Specific))->Text,guiNormalFont);
	else
		DrawString(siLeft+4,siTop+2,((struct CEditBoxItem *)(Edit->Specific))->Text,guiDisabledFont);
}

void SelectEditBox(const struct CGUIConstItem *Edit)
{
	assert(Edit);
	assert(Edit->Kind == IDK_EDIT);

	if (!Edit->Additional->IsEnabled)
		return;
	vLineXor(Edit->Left+4+GetStringLength(guiNormalFont,
		((struct CEditBoxItem *)(Edit->Specific))->Text),
		Edit->Top+2,Edit->Height-3);
}

/////////// Button Functions //////////////////////

const unsigned char
	LeftUpperCorner[3] = {0x30,0x40,0x80},
	RightUpperCorner[3] = {0xc0,0x20,0x10},
	LeftLowerCorner[3] = {0x80,0x40,0x30},
	RightLowerCorner[3] = {0x10,0x20,0xc0};

void DrawButton(const struct CGUIConstItem *Button)
{
	assert(Button);
	assert(Button->Kind == IDK_BUTTON);

	siLeft=Button->Left,
	siTop=Button->Top,
	siWidth=Button->Width,
	siHeight=Button->Height;

	BitCpy(siLeft,siTop,4,3,LeftUpperCorner);
	BitCpy(siLeft+siWidth-4,siTop,4,3,RightUpperCorner);
	BitCpy(siLeft,siTop+siHeight-3,4,3,LeftLowerCorner);
	BitCpy(siLeft+siWidth-4,siTop+siHeight-3,4,3,RightLowerCorner);

	hLine(siLeft+4,siTop,siWidth-8);
	hLine(siLeft+4,siTop+siHeight-1,siWidth-8);
	vLine(siLeft,siTop+3,siHeight-6);
	vLine(siLeft+siWidth-1,siTop+3,siHeight-6);
	if (Button->Additional->IsEnabled)
		DrawString(siLeft+4,siTop+2,Button->Caption,guiNormalFont);
	else
		DrawString(siLeft+4,siTop+2,Button->Caption,guiDisabledFont);
}

void SelectButton(const struct CGUIConstItem *Button)
{
	assert(Button);
	assert(Button->Kind == IDK_BUTTON);

	siLeft=Button->Left;
	siTop=Button->Top;
	siWidth=Button->Width;
	siHeight=Button->Height;

	if (!Button->Additional->IsEnabled)
		return;

	BoxXor(siLeft+1,siTop+2,siWidth-2,siHeight-4);
	hLineXor(siLeft+2,siTop+1,siWidth-4);
	hLineXor(siLeft+2,siTop+siHeight-2,siWidth-4);
}

////////////// Check Box Functions /////////////////////
const unsigned char
	FlagChecked[8]={0xff,0xc3,0xa5,0x99,0x99,0xa5,0xc3,0xff},
	FlagUnChecked[8]={0xff,0x81,0x81,0x81,0x81,0x81,0x81,0xff};

void DrawCheckBox(const struct CGUIConstItem *CheckBox)
{
	assert(CheckBox);
	assert(CheckBox->Kind == IDK_CHECK);

	siLeft = CheckBox->Left;
	siTop = CheckBox->Top;
	siWidth = CheckBox->Width;
	siHeight = CheckBox->Height;

	if (((struct CCheckBoxItem *)(CheckBox->Specific))->IsChecked)
	{
		BitCpy(siLeft,siTop,8,8,FlagChecked);
	}
	else
	{
		BitCpy(siLeft,siTop,8,8,FlagUnChecked);
	}
	if (CheckBox->Additional->IsEnabled)
		DrawString(siLeft+12,siTop,CheckBox->Caption,guiNormalFont);
	else
		DrawString(siLeft+12,siTop,CheckBox->Caption,guiDisabledFont);
}

void SelectCheckBox(const struct CGUIConstItem *CheckBox)
{
	assert(CheckBox);
	assert(CheckBox->Kind == IDK_CHECK);

	siLeft = CheckBox->Left;
	siTop = CheckBox->Top;
	siWidth = CheckBox->Width;
	siHeight = CheckBox->Height;

	if (!CheckBox->Additional->IsEnabled)
		return;
	hLineXor(siLeft,siTop+siHeight-1,siWidth);
	vLineXor(siLeft+siWidth-1,siTop,siHeight-1);
}

///////////////////// BmpItem functions /////////////////////

void DrawBmpItem(const struct CGUIConstItem *BmpItem)
{
	assert(BmpItem);
	assert(BmpItem->Kind == IDK_BMP);

	if (BmpItem->Additional->IsExtra)
	{
		BitCpyInv(((struct CBmpItem *)(BmpItem->Specific))->Left,
			((struct CBmpItem *)(BmpItem->Specific))->Top,
			BmpItem->Width,BmpItem->Height,
			BmpItem->Caption);
	}
	else
	{
		BitCpy(((struct CBmpItem *)(BmpItem->Specific))->Left,
			((struct CBmpItem *)(BmpItem->Specific))->Top,
			BmpItem->Width,BmpItem->Height,
			BmpItem->Caption);
	}
}
