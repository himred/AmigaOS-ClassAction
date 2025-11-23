/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : learn.c

	Goal : learn new classes

	Ver  : 2.5

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
#include <clib/asl_protos.h>

#include "gadgets.h"
#include "includes.h"

#include <intuition/intuition.h>
#include <libraries/reqtools.h>
#include <clib/reqtools.h>

#include <stdio.h>
#include <fcntl.h>
#include <dos/dosextens.h>

#define GetString(g) (((struct StringInfo *)g->SpecialInfo)->Buffer)

#define GD_LE_STATUS 0
#define GD_LE_OFF1 1
#define GD_LE_OFF2 2
#define GD_LE_OFF3 3
#define GD_LE_OFF4 4
#define GD_LE_OFF5 5
#define GD_LE_OK 6
#define GD_LE_CANCEL 7
#define LearnCNT 8

#define SENS 3
#define MAXI 16

struct Screen *L_Scr = NULL;
APTR L_VisualInfo = NULL;
struct Window *LearnWnd = NULL;
struct Gadget *LearnGList = NULL;
struct Gadget *LearnGadgets[8];
UWORD LearnLeft = 13;
UWORD LearnTop = 23;
UWORD LearnWidth = 538;
UWORD LearnHeight = 140;

UWORD LearnGTypes[] = {
	TEXT_KIND,
	STRING_KIND,
	STRING_KIND,
	STRING_KIND,
	STRING_KIND,
	STRING_KIND,
	BUTTON_KIND,
	BUTTON_KIND};

LONG LearnGTags[] = {
	(GTTX_Border), TRUE, (TAG_DONE),
	(GTST_MaxChars), 49, (TAG_DONE),
	(GTST_MaxChars), 49, (TAG_DONE),
	(GTST_MaxChars), 49, (TAG_DONE),
	(GTST_MaxChars), 49, (TAG_DONE),
	(GTST_MaxChars), 256, (TAG_DONE),
	(TAG_DONE),
	(TAG_DONE)};

struct rtFileRequester *req;
struct rtFileList *head, *tmplist;
struct Class *TheClass;
char Accepted = 0;
int Nf;

extern struct ReqToolsBase *ReqToolsBase;

int OpenLearnWindow(void)
{

	UBYTE *LearnWdt = (UBYTE *)LocaleMsg[CAP_L_LEARN];

	struct NewGadget LearnNGad[] = {
		92, 6, 413, 13, (UBYTE *)LocaleMsg[CAP_L_STATUS], NULL, GD_LE_STATUS, PLACETEXT_LEFT, NULL, NULL,
		92, 28, 413, 13, (UBYTE *)"Offset #1", NULL, GD_LE_OFF1, PLACETEXT_LEFT, NULL, NULL,
		92, 45, 413, 13, (UBYTE *)"Offset #2", NULL, GD_LE_OFF2, PLACETEXT_LEFT, NULL, NULL,
		92, 62, 413, 13, (UBYTE *)"Offset #3", NULL, GD_LE_OFF3, PLACETEXT_LEFT, NULL, NULL,
		92, 79, 413, 13, (UBYTE *)"Offset #4", NULL, GD_LE_OFF4, PLACETEXT_LEFT, NULL, NULL,
		92, 96, 413, 13, (UBYTE *)"Offset #5", NULL, GD_LE_OFF5, PLACETEXT_LEFT, NULL, NULL,
		92, 119, 84, 14, (UBYTE *)LocaleMsg[CAP_L_ACCEPT], NULL, GD_LE_OK, PLACETEXT_IN, NULL, NULL,
		421, 119, 84, 14, (UBYTE *)LocaleMsg[CAP_CANCEL], NULL, GD_LE_CANCEL, PLACETEXT_IN, NULL, NULL};
	struct NewGadget ng;
	struct Gadget *g;
	UWORD lc, tc;
	UWORD offx = Scr->WBorLeft, offy = Scr->WBorTop + Scr->RastPort.TxHeight + 1;

	if (!(g = CreateContext(&LearnGList)))
		return (1L);

	for (lc = 0, tc = 0; lc < LearnCNT; lc++)
	{

		CopyMem((char *)&LearnNGad[lc], (char *)&ng, (long)sizeof(struct NewGadget));

		ng.ng_VisualInfo = VisualInfo;
		ng.ng_TextAttr = &topaz8;
		ng.ng_LeftEdge += offx;
		ng.ng_TopEdge += offy;

		LearnGadgets[lc] = g = CreateGadgetA((LONG)LearnGTypes[lc], g, &ng, (struct TagItem *)&LearnGTags[tc]);

		while (LearnGTags[tc])
			tc += 2;
		tc++;

		if (NOT g)
			return (2L);
	}

	if (!(LearnWnd = OpenWindowTags(NULL,
									WA_Left, LearnLeft,
									WA_Top, LearnTop,
									WA_Width, LearnWidth,
									WA_Height, LearnHeight + offy,
									WA_IDCMP, TEXTIDCMP | STRINGIDCMP | BUTTONIDCMP | IDCMP_CLOSEWINDOW | IDCMP_REFRESHWINDOW,
									WA_Flags, WFLG_DRAGBAR | WFLG_SMART_REFRESH | WFLG_ACTIVATE,
									WA_Gadgets, LearnGList,
									WA_Title, LearnWdt,
									WA_PubScreen, Scr,
									TAG_DONE)))
		return (4L);

	GT_RefreshWindow(LearnWnd, NULL);

	return (0L);
}

void CloseLearnWindow(void)
{
	if (LearnWnd)
	{
		CloseWindow(LearnWnd);
		LearnWnd = NULL;
	}

	if (LearnGList)
	{
		FreeGadgets(LearnGList);
		LearnGList = NULL;
	}
}

/*****************************************/

void HandleLearn(void)
{
	struct IntuiMessage *Msg;
	long Class, Code, ID;
	struct Gadget *SelGadget;
	BOOL ENDTEST = FALSE;
	BOOL CHECKGADGET = TRUE;
	int i;
	int p;
	LONG tag[] = {(LONG)GTCY_Active, (LONG)&p, TAG_DONE};

	while (!(ENDTEST))
	{
		WaitPort(LearnWnd->UserPort);
		while (Msg = GT_GetIMsg(LearnWnd->UserPort))
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

				if (ID == GD_LE_CANCEL)
					ENDTEST = TRUE;
				if (ID == GD_LE_OK && Req(LocaleMsg[CAP_L_DO_U]) == 1)
				{

					for (i = 0; i < 5; i++)
						strcpy(TheClass->value[i], GetString(LearnGadgets[i + 1]));

					strcpy(TheClass->suffix, "");

					GT_GetGadgetAttrsA(Test_Gadgets[GD_Offset_Cycle], Test_Wnd, NULL, (struct TagItem *)tag);

					GT_SetGadgetAttrs(Test_Gadgets[GD_Class_Off], Test_Wnd, NULL,
									  GTST_String, TheClass->value[p], TAG_DONE);

					GT_SetGadgetAttrs(Test_Gadgets[GD_Class_Ext], Test_Wnd, NULL,
									  GTST_String, "", TAG_DONE);

					ENDTEST = TRUE;
					Accepted = 1;
				}

				break;

			default:
				break;
			}
		}
	}

	Forbid();

	while (Msg = GT_GetIMsg(LearnWnd->UserPort))
		GT_ReplyIMsg(Msg);

	ModifyIDCMP(LearnWnd, 0L);

	Permit();
}

/****************************************/

int Learn(struct Class *cla)
{
	int Analyse(void);
	char file[108];

	TheClass = cla;

	req = rtAllocRequest(RT_FILEREQ, NULL);

	if (req == NULL)
		return (-2);

	file[0] = 0;
	head = rtFileRequest(req, file, LocaleMsg[CAP_L_CHOOSE], RTFI_Flags, FREQF_MULTISELECT, TAG_DONE);

	if (head == NULL)
	{
		rtFreeRequest(req);
		return (-1);
	}

	tmplist = head;
	Nf = 0;
	while (tmplist != NULL)
	{
		tmplist = tmplist->Next;
		Nf++;
	}

	if (Nf < 2)
	{
		Info(LocaleMsg[CAP_L_NEED]);
		rtFreeRequest(req);
		rtFreeFileList(head);
		return (-4);
	}

	OpenLearnWindow();
	if (Analyse() != 0)
		HandleLearn();
	CloseLearnWindow();
	rtFreeRequest(req);
	rtFreeFileList(head);
	return (Accepted);
}

/******/

int Analyse(void)
{

	struct FileInfoBlock info;
	BPTR lock;
	LONG l;
	int i;
	LONG min = 9999999999;
	char *buff, *tmp;
	unsigned int *same;
	int k, h;
	int size;
	char isasc;
	int j;
	int nbz;
	int nf = 0;
	char cc[5];
	char offset[60];
	int argc;
	char filename[255], file[255];
	char dir[100];
	int s;

	argc = Nf;
	l = strlen(req->Dir);
	if (l == 0)
		dir[0] = 0;
	else
	{
		if (req->Dir[l - 1] != ':')
			sprintf(dir, "%s/", req->Dir);
		else
			strcpy(dir, req->Dir);
	}

	GT_SetGadgetAttrs(LearnGadgets[0], LearnWnd, NULL,
					  GTTX_Text, LocaleMsg[CAP_L_PREP], TAG_DONE);

	tmplist = head;
	for (i = 0; i < argc; i++)
	{
		sprintf(filename, "%s%s", dir, tmplist->Name);
		lock = Lock(filename);
		Examine(lock, &info);
		l = (LONG)info.fib_Size;
		if (l < min)
		{
			min = l;
			strcpy(file, filename);
		}
		UnLock(lock);
		tmplist = tmplist->Next;
	}

	if (min < 8)
	{
		Info(LocaleMsg[CAP_L_NOTHING]);
		return (0);
	}

	buff = (char *)malloc(min);
	tmp = (char *)malloc(min);
	same = (int *)malloc(min * sizeof(int));

	if (buff == NULL || tmp == NULL || same == NULL)
	{
		Info(LocaleMsg[CAP_L_MEMORY]);
		if (buff != NULL)
			free(buff);
		if (tmp != NULL)
			free(tmp);
		if (same != NULL)
			free(same);
		return (0);
	}

	for (i = 0; i < min; i++)
		same[i] = 0;

	h = open(file, O_RDONLY);
	read(h, buff, min);
	close(h);

	tmplist = head;
	for (i = 0; i < argc; i++)
	{
		sprintf(offset, "%s (%d/%d)", LocaleMsg[CAP_L_PROG], i + 1, argc);

		GT_SetGadgetAttrs(LearnGadgets[0], LearnWnd, NULL,
						  GTTX_Text, offset, TAG_DONE);

		sprintf(filename, "%s%s", dir, tmplist->Name);
		if (strcmp(filename, file) != 0)
		{
			h = open(filename, O_RDONLY);
			read(h, tmp, min);
			close(h);
			for (j = 0; j < min; j++)
				if (tmp[j] == buff[j])
					same[j]++;
		}

		tmplist = tmplist->Next;
	}

	i = -1;
	nf = 0;

	while ((i < min - SENS) && (nf < 5))
	{
		i++;
		if (same[i] == argc - 1)
		{
			k = i + 1;
			s = i;
			while (k < min && same[k] == argc - 1 && (k - i <= MAXI))
				k++;
			while ((buff[s] == 0) && (s <= k))
				s++;
			if (k - s >= SENS)
			{
				size = k - s;
				isasc = 1;
				nbz = 0;
				for (j = 0; j < size; j++)
				{
					if (buff[s + j] == 0)
						nbz++;
					if (buff[s + j] < 32 || buff[s + j] > 126)
					{
						if (j >= SENS && isasc == 1)
						{
							size = j;
							isasc = 1;
							j = size + 1;
						}
						else
							isasc = 0;
					}
				}

				nf++;
				sprintf(offset, "%d,", s);
				if (isasc == 1)
					strcat(offset, "'");
				for (j = 0; j < size; j++)
				{
					if (isasc == 0)
						sprintf(cc, "%02x", (unsigned char)buff[s + j]);
					else
						sprintf(cc, "%c", buff[s + j]);
					strcat(offset, cc);
				}

				if (isasc == 1)
					strcat(offset, "'");

				GT_SetGadgetAttrs(LearnGadgets[nf], LearnWnd, NULL,
								  GTST_String, offset, TAG_DONE);

				i = k - 1;
			}
		}
	}

	GT_SetGadgetAttrs(LearnGadgets[0], LearnWnd, NULL,
					  GTTX_Text, LocaleMsg[CAP_L_COMP], TAG_DONE);
	free(buff);
	free(same);
	free(tmp);
	if (nf == 0)
		Info(LocaleMsg[CAP_L_NOTHING]);
	return (nf);
}
