#ifndef _GUI_SERVER
#define _GUI_SERVER

#ifdef __cplusplus
#	define EXPORT_CPP extern "C"
#else
#	define EXPORT_CPP
#endif

/*****************************************************************
 *  ������, ���� ������ ������� ������� ���������� ����������. *
 *  ��� ������� ������ ����� ������������ ���� ����          *
 *  �������� InitGUIServer(), ���� � ���� �����������         *
 *  ProcessGUIServer() � ���������� ����������-���������������   *
 *  ��䳿 (���� ��� touch-screen, ���������).                   *
 *  ������ ���������� �������� �� ����"������ �� ���� ������,   *
 *  ����� �� ������� ���������� ��������� � �������� ����      *
 *  �� ��������� ���, �� ����������� ���������������           *
 *  MAX_ITEMS_TO_DRAW � ���� interface.h                        *
 *****************************************************************/

// �� ������ ������� ���� ���������� ������?
#define LABELS_ARE_SELECTABLE
#undef LABELS_ARE_SELECTABLE

// �� ����� ����������� ��������� �������� ���������?
// ��� �����, ��������, ���� �������� ������ ���� ��������������,
// ���� � ���������� � ������. ���� ���� ����� �������� �� ������
// ���������������
#define GET_DISABLED_BY_TOUCHSCREEN
//#undef GET_DISABLED_BY_TOUCHSCREEN

/************************************************************************
 *  ������� �������� �� ������� ����������, ���������� ���� � ItemID. *
 *  ������ ��������� � ����� ���������, � �� � interface.h, ���� ��    *
 *  ������� ���� ������������ ����������� �������                    *
 ************************************************************************/

#define GET_ITEM_WITH_ID(ItemID) (ConstDeskTop + ItemID)

extern const unsigned char *guiNormalFont, *guiDisabledFont,*guiExtraFont;
extern unsigned char dh;
//extern const struct CGUIConstItem *Selection; // ������
extern const struct CGUIConstItem *ActiveDialog; // �������� ������� ��������� ����
											// �������� ���� ���� ���� ����!
extern struct CDialogCtrl *ActiveDialogSpec; // � �� ���� ��������� ��� -- ������
// �������� 6 ��������� 2003 ����, ��� ���� ��� �������� ��������� ������� ��� �������
// ������ ������ �� �����

// ���� ����
enum EventTypes
{
	EVENT_NULL,
	EVENT_TOUCHSCREEN,
	EVENT_KEYBOARD
};

// ������
enum Keys
{
	K_0, K_1, K_2, K_3, K_4, K_5, K_6, K_7, K_8, K_9,
	K_LEFT, K_RIGHT, K_UP, K_DOWN,
	K_PGDOWN,K_PGUP,
	K_INC,K_DEC
};

// ���� ��䳿
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
 *  ����������� �������.                                                        *
 *  ���������:                                                                    *
 *      NormalFont - �����, ���� ��������������������� ��� ��������� ��������    *
 *      DisabledFont - �����, ���� ������ ���������������� ��������� ��������    *
 *  �������                                                                      *
 *      1 � ������� �������� ����������                                          *
 *      0 ������                                                                  *
 **********************************************************************************/
EXPORT_CPP int InitGUIServer(void);

/**********************************************************************************
 *  ����������� ������� �������� �����, �������������� ��� � DrawQueue           *
 *  �� ConstDeskTop                                                               *
 **********************************************************************************/
void DrawGUI(void);

void RefreshGUIItem(const struct CGUIConstItem *Item);

/**********************************************************************************
 *  ������� �������� �� �� ������ ����������, ��� ������ ����� x, y            *
 *  � ������ �� �����                                                            *
 **********************************************************************************/
const struct CGUIConstItem *GetGUIItem(unsigned int x, unsigned int y,
									const struct CGUIConstItem *Item);

/**********************************************************************************
 *  ³������� ������� �������� ������ ����������                                 *
 **********************************************************************************/
void SelectGUIItem(const struct CGUIConstItem *Item);

/**********************************************************************************
 *  ���������� ������ �� ���� �� �������� ���������� �� ����������� ����         *
 *  ��������                                                                     *
 **********************************************************************************/
void SetSelection(const struct CGUIConstItem *Item);

/**********************************************************************************
 *  ������� �������, ��� ��� ���������� ����� ����������, ��� �����-            *
 *  ��� ����, ������� ���������� Event                                           *
 **********************************************************************************/
EXPORT_CPP int ProcessGUIServer(const struct CEventDescriptor *Event);

/*********************************************************************************
 *  ����� ������� � ������ ��� �� ������, �� ���������������� ��� �������� ��  *
 * �������. ���� ������� � �����                                              *
 *********************************************************************************/
const struct CGUIConstItem *GetNearestLeftItem(const struct CGUIConstItem *Item);
const struct CGUIConstItem *GetNearestRightItem(const struct CGUIConstItem *Item);
const struct CGUIConstItem *GetNearestUpperItem(const struct CGUIConstItem *Item);
const struct CGUIConstItem *GetNearestLowerItem(const struct CGUIConstItem *Item);

#undef EXPORT_CPP

#endif
