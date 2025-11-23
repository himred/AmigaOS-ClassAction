
/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : handleapp.c

	Goal : Handle AppIcon messages

	Ver  : 2.01

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
#include <fcntl.h>

#define APPICON 0

/****************************************/
/*										*/
/* Function : HandleApp()				*/
/*										*/
/* Goal     : handle AppIcon messages	*/
/*										*/
/*										*/
/* Return Values : 						*/
/*										*/
/*	0: Okay message handled				*/
/*	2: Got a quit message				*/
/*										*/
/****************************************/

int HandleApp(void)
{
	int PopUp(struct List *, char *, char *, int);

	struct AppMessage *appmsg = NULL;
	char out = 0;
	char ENDTEST = FALSE;
	int i;
	char dir[500];
	char name[40];
	struct List AppList;
	struct Action *act;
	struct Class *cla;
	int x;
	int ret = 0;
	int nnn = 0;
	int ar;

	while (appmsg = (struct AppMessage *)GetMsg(IconPort))
	{
		if (!appmsg->am_NumArgs)
			ret = 2;
		else
			for (ar = 0; ar < appmsg->am_NumArgs; ar++)
			{
				strcpy(name, appmsg->am_ArgList[ar].wa_Name);
				NameFromLock(appmsg->am_ArgList[ar].wa_Lock, dir, 499);
				if (strlen(name) != 0)
				{
					if (dir[strlen(dir) - 1] != ':')
						strcat(dir, "/");
					strcat(dir, name);
					cla = Getclass(dir, name, 0);
					act = cla->action;
					NewList(&AppList);
					nnn = 0;
					while (act != NULL)
					{
						AddList(&AppList, act->name, 0);
						nnn = nnn + 1;
						act = act->next;
					}

					act = NULL;
					if (nnn == 1)
						act = cla->action;
					else
					{
						x = PopUp(&AppList, name, cla->name, nnn);
						if (x != -1)
						{
							act = cla->action;
							for (i = 0; i < x; i++)
								act = act->next;
						}
					}

					if (act != NULL)
						Runner(act, dir, ReqBug);
				}
			}

		ReplyMsg((struct Message *)appmsg);
	}
	return (ret);
}
