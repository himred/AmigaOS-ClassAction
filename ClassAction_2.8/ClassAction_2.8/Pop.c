/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : pop.c

	Goal : Build up the Action requester

	Ver  : 2.01

****************************************/

#include <exec/types.h>
#include <exec/exec.h>
#include <intuition/intuitionbase.h>
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

#include "includes.h"

#define POPGAD 100
#define NCOM 4

struct Gadget *Mylist;
struct Window *Pop_Wnd;
struct Gadget *Pop_List;
struct Screen *Pop_Scr;
int Xm, Ym;

/****************************************/
/*										*/
/* Function : OpenPopWindow()			*/
/*										*/
/* Goal     : Open the Action requester	*/
/*	      whith height hhh				*/
/*	      for the file 'name' of		*/
/*	      class 'classname'				*/
/*										*/
/* Return Values : 						*/
/*										*/
/*	0: Okay								*/
/*	1: Unable to create context			*/
/*	4: Unable to create window			*/
/*										*/
/****************************************/

int OpenPop_Window(char *name, char *classname, int hhh)
{
	char title[1000];
	struct NewGadget ng;
	struct Gadget *g;
	ULONG Tagy = (TAG_DONE);

	PubFrontScreen(title);
	Pop_Scr = LockPubScreen(title);
	VisualInfo = GetVisualInfo(Pop_Scr, TAG_DONE);

	sprintf(title, "%s (%s)", name, classname);

	struct NewGadget NGad = {
		1, 1, (TheFont->tf_XSize + 2) * strlen(title), hhh, (UBYTE *)"", NULL, POPGAD, PLACETEXT_ABOVE, NULL, NULL};

	if (!(g = CreateContext(&Pop_List)))
		return (1L);

	CopyMem((char *)&NGad, (char *)&ng, (long)sizeof(struct NewGadget));

	ng.ng_VisualInfo = VisualInfo;
	ng.ng_TextAttr = &Dirattr;
	ng.ng_LeftEdge = 5;
	ng.ng_TopEdge = Myattr.ta_YSize + 4;

	g = CreateGadgetA((ULONG)LISTVIEW_KIND, g, &ng, (struct TagItem *)&Tagy);

	Mylist = g;

	if (!(Pop_Wnd = OpenWindowTags(NULL,
								   WA_Left, Xm - 13,
								   WA_Top, Ym - Myattr.ta_YSize - 7,
								   WA_Width, (TheFont->tf_XSize + 2) * strlen(title) + 10,
								   WA_Height, hhh + Myattr.ta_YSize + 3,
								   WA_IDCMP, LISTVIEWIDCMP | IDCMP_CLOSEWINDOW | IDCMP_REFRESHWINDOW,
								   WA_Flags, WFLG_DRAGBAR | WFLG_SMART_REFRESH | WFLG_ACTIVATE | WFLG_CLOSEGADGET,
								   WA_Gadgets, Pop_List,
								   WA_Title, title,
								   WA_ScreenTitle, "",
								   WA_PubScreen, Pop_Scr,
								   WA_PubScreenFallBack, TRUE,
								   TAG_DONE)))
		return (4L);

	GT_RefreshWindow(Pop_Wnd, NULL);

	return (0L);
}

/****************************************/
/*										*/
/* Function : ClosePopWindow			*/
/*										*/
/* Goal     : close the action requester*/
/*										*/
/*										*/
/* Return Values : NONE					*/
/*										*/
/****************************************/

void ClosePop_Window(void)
{
	if (Pop_Wnd)
	{
		CloseWindow(Pop_Wnd);
		Pop_Wnd = NULL;
	}

	if (Pop_List)
	{
		FreeGadgets(Pop_List);
		Pop_List = NULL;

		if (VisualInfo)
			FreeVisualInfo(VisualInfo);
		if (Pop_Scr)
			UnlockPubScreen(NULL, Pop_Scr);
	}
}

/****************************************/
/*										*/
/* Function : PopUp						*/
/*										*/
/* Goal     : pop the actions requeter  */
/*	      and wait for a user reply		*/
/*	      and handle it					*/
/*	      Fill up the requester 		*/
/*	      with list 'li' for the		*/
/*	     file name, with nb actions		*/
/*										*/
/* Return Values : 						*/
/*										*/
/*	-1: User cancel						*/
/*       or the Id of action choosen	*/
/*										*/
/****************************************/

int PopUp(struct List *list, char *name, char *classname, int nb)
{
	int i;
	int HandleIDCMPOP(void);

	if (nb > 7)
		nb = 7;
	i = (nb + 1) * (Dirattr.ta_YSize);

	OpenPop_Window(name, classname, i);

	GT_SetGadgetAttrs(Mylist, Pop_Wnd, NULL,
					  GTLV_Labels, list, TAG_DONE);

	WindowToFront(Pop_Wnd);

	i = HandleIDCMPOP();

	ClosePop_Window();
	FreeList(list);
	return (i);
}

/****************************************/
/*										*/
/* Function : HandleIDCMPPop			*/
/*										*/
/* Goal     : handle message for pop	*/
/*										*/
/*										*/
/* Return Values : see previous fct		*/
/*										*/
/****************************************/

int HandleIDCMPOP(void)
{
	struct IntuiMessage *Msg;
	long Class, Code, ID;
	struct Gadget *SelGadget;
	BOOL ENDTEST = FALSE;
	BOOL CHECKGADGET = TRUE;
	int ret;

	while (!(ENDTEST))
	{
		WaitPort(Pop_Wnd->UserPort);
		while (Msg = GT_GetIMsg(Pop_Wnd->UserPort))
		{

			Class = Msg->Class;
			Code = Msg->Code;
			if ((Class == IDCMP_GADGETUP) || (Class == IDCMP_GADGETDOWN))
			{

				if (SelGadget = (struct Gadget *)Msg->IAddress)
					ID = SelGadget->GadgetID;
			}
			GT_ReplyIMsg(Msg);

			char *res = NULL;
			switch (Class)
			{
			case IDCMP_GADGETUP:
			case IDCMP_GADGETDOWN:

				if (ID == POPGAD)
				{
					ret = Msg->Code;
					ENDTEST = TRUE;
				}

				break;

			case IDCMP_CLOSEWINDOW:
				ENDTEST = TRUE;
				ret = -1;
				break;
			default:
				break;
			}
		}
	}

	Forbid();

	while (Msg = GT_GetIMsg(Pop_Wnd->UserPort))
		GT_ReplyIMsg(Msg);

	ModifyIDCMP(Pop_Wnd, 0L);

	Permit();
	return (ret);
}

/****************************************/
/*										*/
/* Function : PubFrontScreen			*/
/*										*/
/* Goal     : fill `scrname' with		*/
/*	      the name of the frontmost		*/
/*	      public screen					*/
/*										*/
/* Return Values : NONE					*/
/*										*/
/****************************************/

void PubFrontScreen(char *scrname)
{
	struct List *psl = LockPubScreenList();
	ULONG lock = LockIBase(0L);
	struct Screen *scr;
	struct PubScreenNode *psn;
	STRPTR name = NULL;

	for (scr = IntuitionBase->FirstScreen; !name; scr = scr->NextScreen)
		for (psn = (struct PubScreenNode *)psl->lh_Head; psn->psn_Node.ln_Succ; psn = (struct PubScreenNode *)psn->psn_Node.ln_Succ)
		{
			if (scr == psn->psn_Screen)
			{
				name = (STRPTR)psn->psn_Node.ln_Name;
				Xm = scr->MouseX;
				Ym = scr->MouseY;
				break;
			}
		}
	strcpy(scrname, name);
	UnlockIBase(lock);
	UnlockPubScreenList();
}
