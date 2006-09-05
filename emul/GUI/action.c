// This is program-generated source file. Add your action-handlers code here
// Sakhnik, 2003

#include <assert.h>

#include "iface.h"
#include "action.h"
#include "tools.h"
#include "guiserv.h"
#include "dialog.h"
#include "comctrls.h"

void ActionPerformed(const struct CGUIConstItem *Item,
				const struct CEventDescriptor *Event)
{
	unsigned int ItemID;

	if (Item)
		ItemID = Item -> ID;
	else
		ItemID = 0;
	
	switch (ItemID)
	{
	case ID_MACHINE_BPROP:
	case ID_MACHINE_IMGPROP:
		GET_ITEM_WITH_ID (ID_MACHINE_IMG1_1)->Additional->IsVisible=
			!GET_ITEM_WITH_ID (ID_MACHINE_IMG1_1)->Additional->IsVisible;
		GET_ITEM_WITH_ID (ID_MACHINE_IMG1_1)->Additional->IsExtra=
			!GET_ITEM_WITH_ID (ID_MACHINE_IMG1_1)->Additional->IsExtra;
		DrawBmpItem (GET_ITEM_WITH_ID (ID_MACHINE_IMG1_1));
		//MakeDialogActive(GET_ITEM_WITH_ID(ID_MACHINE_FORMMACHINE));
		break;
	case ID_SPLASH_IMGSPLASH:
	case ID_SPLASH_FORMSPLASH:
		MakeDialogActive(GET_ITEM_WITH_ID(ID_MACHINE_FORMMACHINE));
		break;
	case ID_MACHINE_BINJECTION:
	case ID_MACHINE_IMGINJECTION:
		StoreActiveDialogInStack();
		MakeDialogActive(GET_ITEM_WITH_ID(ID_INJECTION_FORMINJECTION));
		break;
	case ID_MACHINE_BLOCK:
	case ID_MACHINE_IMGLOCK:
		StoreActiveDialogInStack();
		MakeDialogActive(GET_ITEM_WITH_ID(ID_LOCK_FORMLOCK));
		break;
	case ID_MACHINE_BTHERMO:
	case ID_MACHINE_IMGTHERMO:
		StoreActiveDialogInStack();
		MakeDialogActive(GET_ITEM_WITH_ID(ID_TEMPERATURE_FORMT));
		break;
	case ID_MACHINE_BTEST:
	case ID_MACHINE_IMGTEST:
		StoreActiveDialogInStack();
		MakeDialogActive(GET_ITEM_WITH_ID(ID_TEST_FORMTEST));
		break;
	case ID_INJECTION_BEXIT:
	case ID_LOCK_BEXIT:
	case ID_TEMPERATURE_BEXIT:
	case ID_TEST_BEXIT:
	case ID_INJECTION_IMGEXIT:
	case ID_LOCK_IMGEXIT:
	case ID_TEMPERATURE_IMGEXIT:
	case ID_TEST_IMGEXIT:
		RestoreActiveDialogFromStack();
		break;

	}
	
}

// End of action.c
