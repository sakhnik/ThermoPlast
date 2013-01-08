#ifndef _COM_CTRLS
#define _COM_CTRLS

/***************************************************************
 *  Модуль роботи з загальними елементами керування:           *
 *  текстові мітки, поле редактування тексту, командна кнопка, *
 *  перемикач                                                  *
 *                                                             *
 *	DrawXXX() -- промальовування елемента інтерфейсу           *
 *	SelectXXX() -- виділення його курсором                     *
 ***************************************************************/

#include "iface.h"

void DrawLabel(const struct CGUIConstItem *Label);
void SelectLabel(const struct CGUIConstItem *Label);

void DrawEditBox(const struct CGUIConstItem *EditBox);
void SelectEditBox(const struct CGUIConstItem *EditBox);

void DrawButton(const struct CGUIConstItem *Button);
void SelectButton(const struct CGUIConstItem *Button);

void DrawCheckBox(const struct CGUIConstItem *CheckBox);
void SelectCheckBox(const struct CGUIConstItem *CheckBox);

void DrawBmpItem(const struct CGUIConstItem *BmpItem);

#endif
