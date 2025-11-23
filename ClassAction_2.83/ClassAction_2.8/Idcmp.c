/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : idcmp.c

	Goal : Handle IDCMP messages

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
#include <dos/dosextens.h>
#include <fcntl.h>
#include <stdio.h>

#include "gadgets.h"
#include "includes.h"
#include "ClassAction_rev.h"

/****************************************/
/*										*/
/* Function : HandleIDCMP				*/
/*										*/
/* Goal     : Obvious ....				*/
/*										*/
/*										*/
/* Return Values : NONE					*/
/*										*/
/****************************************/

int HandleIDCMP(char *username)
{

	static char ch[2024];
	static char na[1024];
	static char sta[30];
	static char FileName[1024];
	static char File[1024];
	static char *found;

	static struct Window *win;
	static struct Class *cla;
	static struct Action *act, *tmp;
	static struct Action *SelectedAction = NULL;
	static struct Class *SelectedClass = NULL;
	static struct Node *nod;
	static struct Scan *sc;
	static FILE *handler;
	static int i, out;
	static BPTR parent, lock;
	static int isdir;
	static int isgene;
	static char SelectedFile[255];
	static int nload = 0;
	struct FileInfoBlock info;
	ULONG length;
	char Quit = 0;
	static LONG lclicks = 0;
	static LONG lclickm = 0;
	LONG clicks, clickm;

	struct IntuiMessage *Msg;
	long Class, Code, ID;
	struct Gadget *SelGadget;
	BOOL CHECKGADGET = TRUE;

	if (nload == 0)
	{
		Dir();
		Refresh = 0;
		nload = 1;
	}
	if (Refresh == 3)
	{
		SelectedClass = NULL;
		SelectedAction = NULL;
		SelectedFile[0] = 0;
		NewList(&Alist);
		PutList(GD_Proj_Actions, &Alist);
		PutString(GD_Proj_File, "");
		PutString(GD_Proj_Class, "");
		Refresh = 0;
	}
	if (Refresh == 1)
	{
		Refresh = 0;
		PutList(GD_Proj_Req, &Flist);
		if (SelectedClass != NULL)
		{
			PutString(GD_Proj_File, File);
			PutString(GD_Proj_Class, SelectedClass->name);
			PutList(GD_Proj_Actions, &Alist);
		}
	}

	while (Msg = GT_GetIMsg(Test_Wnd->UserPort))
	{

		Class = Msg->Class;
		Code = Msg->Code;
		clicks = Msg->Seconds;
		clickm = Msg->Micros;

		if ((Class == IDCMP_GADGETUP) || (Class == IDCMP_GADGETDOWN))
		{

			if (SelGadget = (struct Gadget *)Msg->IAddress)
				ID = SelGadget->GadgetID;
		}
		GT_ReplyIMsg(Msg);
		if (Test_Wnd == NULL)
			return (0);

		char *res = NULL;

		switch (Class)
		{
		case IDCMP_MOUSEBUTTONS:
			if (Code == 105 && Test_Wnd->MouseX > 85 && Test_Wnd->MouseX < 85 + Number && Test_Wnd->MouseY > 10)
			{
				if (AssignFlag == 0)
					Assigns();
				else
				{
					DirLock = Lock(GetString(Test_Gadgets[GD_Proj_Dir]));
					if (DirLock != NULL)
						Dir();
				}
			}
			break;
		case IDCMP_GADGETUP:
		case IDCMP_GADGETDOWN:

			/******************************/
			/* Click on File requester    */
			/******************************/

			if (ID == GD_Proj_Req)
			{

				if (Code == 0 && ParentFlag == 1)
				{
					ID = GD_Proj_Parent;
				}
				else
				{
					sc = Dirlist.head;
					isdir = 1;

					if (ParentFlag == 1)
						Code--;
					if (Code >= Dirlist.size)
					{
						sc = Filelist.head;
						Code -= Dirlist.size;
						isdir = 0;
					}

					for (i = 0; i < Code; i++)
						sc = sc->next;
					strcpy(File, sc->name);

					if (strcmp(File, SelectedFile) != 0)
					{
						FreeList(&Alist);
						SelectedClass = NULL;
						PutList(GD_Proj_Actions, &Alist);
					}

					if (AssignFlag == 0)
					{
						strcpy(na, ((struct StringInfo *)Test_Gadgets[GD_Proj_Dir]->SpecialInfo)->Buffer);
						if (na[strlen(na) - 1] != ':')
							strcat(na, "/");
						strcat(na, File);
						strcpy(FileName, na);
					}
					else
					{
						isdir = 1;
						strcpy(FileName, File);
					}

					if (isdir == 1)
					{
						parent = Lock(FileName, ACCESS_READ);
						if (parent != NULL)
						{
							UnLock(DirLock);
							DirLock = parent;
							PutText(GD_Proj_Dir, FileName);
							Dir();
							SelectedFile[0] = 0;
						}
					}
					else
					{
						if (strcmp(File, SelectedFile) == 0 && DoubleClick(lclicks, lclickm, clicks, clickm) == TRUE)
						{
							ID = GD_Proj_Actions;
							Code = 0;
							clicks = clickm = 0;
						}
						else
						{
							strcpy(SelectedFile, File);
							cla = Getclass(FileName, File, 1);

							SelectedClass = cla;
							PutString(GD_Proj_Class, cla->name);
							act = cla->action;
							i = 0;
							NewList(&Alist);
							while (act != NULL)
							{
								AddList(&Alist, act->name, 0);
								act = act->next;
								i++;
							}

							if (Generic.action != NULL)
							{
								act = Generic.action;
								while (act != NULL)
								{
									AddList(&Alist, act->name, 1);
									act = act->next;
									i++;
								}
							}

							PutList(GD_Proj_Actions, &Alist);
						}
					}
				}
				lclickm = clickm;
				lclicks = clicks;
			}

			/******************************/
			/* Click on path gadget       */
			/******************************/

			if (ID == GD_Proj_Dir)
			{
				FreeList(&Flist);
				PutList(GD_Proj_Req, &Flist);
				FreeList(&Alist);
				PutList(GD_Proj_Actions, &Alist);

				parent = Lock(((struct StringInfo *)Test_Gadgets[GD_Proj_Dir]->SpecialInfo)->Buffer, ACCESS_READ);
				if (parent != NULL)
				{
					UnLock(DirLock);
					DirLock = parent;
					Dir();
					SelectedFile[0] = 0;
				}
			}

			/******************************/
			/* Click on parent button     */
			/******************************/

			if (ID == GD_Proj_Parent && AssignFlag == 0)
			{
				SelectedFile[0] = 0;
				parent = ParentDir(DirLock);
				if (parent != 0)
				{
					UnLock(DirLock);
					DirLock = parent;
					res = (char *)((struct StringInfo *)Test_Gadgets[GD_Proj_Dir]->SpecialInfo)->Buffer;

					FreeList(&Alist);
					SelectedClass = NULL;
					PutList(GD_Proj_Actions, &Alist);

					i = strlen(res);

					if (res[i - 1] == ':')
					{
						NameFromLock(DirLock, ch, 100);
						PutText(GD_Proj_Dir, ch);
					}
					else
					{
						*((char *)(PathPart(res))) = 0;
						PutText(GD_Proj_Dir, res);
					}
					Dir();
				}
				else
					Assigns();
			}

			/******************************/
			/* Click on Actions requester */
			/******************************/

			if (ID == GD_Proj_Actions)
			{
				isgene = 0;
				act = SelectedClass->action;
				if (Code > SelectedClass->nbaction - 1)
				{
					Code -= SelectedClass->nbaction;
					act = Generic.action;
					isgene = 1;
				}

				for (i = 0; i < Code; i++)
					act = act->next;

				Runner(act, FileName, isgene);
			}

			/******************************/
			/* Click on about button      */
			/******************************/

			if (ID == GD_Proj_About)
			{
				sprintf(ch, LocaleMsg[CA_COPYRIGHT], VSTRING, username);
				Info(ch);
			}

			/******************************/
			/* Click on driveX button     */
			/******************************/

			if (ID >= GD_Proj_Dr1)
			{
				FreeList(&Flist);
				PutList(GD_Proj_Req, &Flist);
				FreeList(&Alist);
				PutList(GD_Proj_Actions, &Alist);

				i = ID - GD_Proj_Dr1;
				parent = Lock(ButPath[i], ACCESS_READ);
				if (parent != NULL)
				{
					UnLock(DirLock);
					DirLock = parent;
					PutText(GD_Proj_Dir, ButPath[i]);
					Dir();
					SelectedFile[0] = 0;
				}
			}

			/******************************/
			/* Click on Quit button       */
			/******************************/

			if (ID == GD_Proj_Quit && Req(LocaleMsg[CA_QUIT]) == 1)
			{
				Quit = 1;
			}

			/******************************/
			/* Click on Info button       */
			/******************************/

			if (ID == GD_Proj_Info)
			{
				if (SelectedClass == NULL)
					Info(LocaleMsg[CA_SELECT_FILE]);
				else
				{
					lock = Lock(FileName, ACCESS_READ);
					Examine(lock, &info);
					length = (ULONG)info.fib_Size;
					UnLock(lock);

					sprintf(na, LocaleMsg[CA_INFO], File, SelectedClass->name, length);
					if (DecrunchFlag == 1 && Cruncher[0] != 0)
					{
						sprintf(ch, "\nCruncher  : %s", Cruncher);
						strcat(na, ch);
					}

					Info(na);
				}
			}
			break;

		case IDCMP_CLOSEWINDOW:
			Quit = 2;
			strcpy(OldPath, (char *)((struct StringInfo *)Test_Gadgets[GD_Proj_Dir]->SpecialInfo)->Buffer);
			WinX = Test_Wnd->LeftEdge;
			WinY = Test_Wnd->TopEdge;

			SelectedClass = NULL;
			SelectedAction = NULL;
			SelectedFile[0] = 0;
			NewList(&Alist);
			PutList(GD_Proj_Actions, &Alist);
			PutString(GD_Proj_File, "");
			PutString(GD_Proj_Class, "");

			break;
		case IDCMP_NEWSIZE:

			SetNewSize(Test_Wnd->Height, 1);

			break;

		default:
			break;
		}
	}
	return (Quit);
}
