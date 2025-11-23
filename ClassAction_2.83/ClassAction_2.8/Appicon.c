/****************************************

		Class Action Source

		(c) by 1994 Salim Gasmi

		File : AppIcon.c

		Goal : Handle AppIcon Hardly

		Ver  : 2.00

****************************************/

#include <exec/types.h>
#include <exec/exec.h>
#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <intuition/classusr.h>
#include <intuition/imageclass.h>
#include <intuition/gadgetclass.h>
#include <libraries/gadtools.h>
#include <graphics/displayinfo.h>
#include <graphics/gfxbase.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/gadtools_protos.h>
#include <clib/graphics_protos.h>
#include <clib/utility_protos.h>
#include <string.h>
#include <workbench/startup.h>
#include <workbench/Workbench.h>

#include "gadgets.h"
#include "includes.h"
#include <stdio.h>

APTR XOBJ, XPORT, WBBASE, XTEXT;

/****************************************/
/*                                      */
/* Function : CreateAppIcon()           */
/*                                      */
/* Goal     : Create the AppIcon        */
/*            using my own asm call     */
/*                                      */
/* Return Values : NONE                 */
/*                                      */
/****************************************/

void CreateAppIcon()
{

	WBBASE = (APTR)WbBase;
	XPORT = (APTR)IconPort;
	XOBJ = (APTR)IconObj;
	XTEXT = (APTR)IconName;

	IconApp = (struct AppIcon *)MyAddAppIcon();
}

/****************************************/
/*                                      */
/* Function : InitAppStuff()            */
/*                                      */
/* Goal     : Initialize needed stuff   */
/*            for the AppIcon           */
/*                                      */
/* Return Values :                      */
/*                                      */
/*      0 : Okay                        */
/*     -1 : Failed                      */
/*                                      */
/****************************************/

int InitAppStuff()
{
	BPTR lock;
	char tmp[1000];
	char chemin[1000];

	IconObj = NULL;

	lock = GetProgramDir();
	NameFromLock(lock, tmp, 999);

	if (tmp[strlen(tmp) - 1] == ':')
		sprintf(chemin, "%s%s", tmp, ProgramName);
	else
		sprintf(chemin, "%s/%s", tmp, ProgramName);

	WbBase = (struct Library *)OpenLibrary("workbench.library", 0);
	if (WbBase == NULL)
		return (-1);

	if (IconFile[0] != 0)
		IconObj = (struct DiskObject *)GetDiskObject(IconFile);
	if (IconObj == NULL)
		IconObj = (struct DiskObject *)GetDiskObjectNew(chemin);

	if (IconX == -1)
		IconX = NO_ICON_POSITION;
	if (IconY == -1)
		IconY = NO_ICON_POSITION;

	IconObj->do_Type = NULL;
	IconObj->do_CurrentX = IconX;
	IconObj->do_CurrentY = IconY;

	IconPort = CreateMsgPort();
	if (IconPort == NULL)
		return (-1);
	SigApp = 1 << IconPort->mp_SigBit;
	return (0);
}

/****************************************/
/*                                      */
/* Function : RemIcon()                 */
/*                                      */
/* Goal     : Remove the AppIcon        */
/*                                      */
/* Return Values : NONE                 */
/*                                      */
/****************************************/

void RemIcon()
{

	if (IconApp != NULL)
	{
		RemoveAppIcon(IconApp);
		IconApp = NULL;
	}
}
