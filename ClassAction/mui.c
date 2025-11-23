/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : Mui.c

	Goal : Mui interface code

	Ver  : 3.00

****************************************/


#include <dos/dos.h>
#include <devices/inputevent.h>
#include <lib/rexx.h>
#include <exec/exec.h>
#include <exec/io.h>
#include <exec/devices.h>

#include <proto/intuition.h>
#include <intuition/intuition.h>

#include "includes.h"
#include "ClassAction_rev.h"
#include "mui.h"


/* Some local globals needed */

static const char *Onglet[3];
char MAX_STR[512];
char *FAIL_ERROR="Internal Error";
char *TheVersion=VERSTAG;
char ByeBye=0;
char DirPattern[208];

char *acts[100];
char *pacts[100];
char FullName[1024];
struct DiskObject *dobj;

char User[50];
int LastDir=-1;
struct Library *MUIMasterBase;

void MUInfo(char *);

APTR app,appop=NULL,window,
     popup,popup2,
     LST_DIR,LST_DIR2,
     LST_ACT,
     STR_PATH,STR_PATH2,
     TXT_FILENAME,
     TXT_CLASS,
     BT_PARENT,BT_PARENT2,BT_SWAP,
     BT_INFO,
     BT_VERS,
     BT_DRIVES[MXDRV],
     BT_PREFS,
     LST_VOL,LST_VOL2,
     POP_WIN,
     POP_LST,
     BT_MKDIR,
     BT_COPY,
     BT_DELETE,
     BT_RENAME,
     BT_CLONE,
     BT_MOVE,
     BT_FIN;


struct Class *CurrentClass,*OldClass;
int NbDrv=0;


/****************************************

    Function : HookFuncDspList

    Goal     : Display volumes &&
               assigns

    Return Values : Always 0

****************************************/
SAVEDS ASM LONG HookFuncDspList(REG(a2) char **array, REG(a1) struct FileInfoBlock *fi)
{
    static char buffer1[127], buffer2[20];

    LONG i;
    struct FileInfoBlock *fib;

    i = (LONG)array[-1];

    DoMethod(LST_DIR, MUIM_List_GetEntry, i, &fib);

    if (fib->fib_DirEntryType < 0)
    {
        sprintf(buffer1, "\33l%s", fib->fib_FileName);
        sprintf(buffer2, "\33r%ld", fib->fib_Size);
        array[0] = buffer1;
        array[1] = buffer2;
    }
    else
    {
        sprintf(buffer1, "\338\33l%s", fib->fib_FileName);
        array[0] = buffer1;
        array[1] = "\338\33rDir";
    }

    return (0);
}

SAVEDS ASM LONG HookFuncDspList2(REG(a2) char **array, REG(a1) struct FileInfoBlock *fi)
{

    static char buffer1[127], buffer2[20];

    LONG i;
    struct FileInfoBlock *fib;

    i = (LONG)array[-1];

    DoMethod(LST_DIR2, MUIM_List_GetEntry, i, &fib);

    if (fib->fib_DirEntryType < 0)
    {
        sprintf(buffer1, "\33l%s", fib->fib_FileName);
        sprintf(buffer2, "\33r%ld", fib->fib_Size);
        array[0] = buffer1;
        array[1] = buffer2;
    }
    else
    {
        sprintf(buffer1, "\338\33l%s", fib->fib_FileName);
        array[0] = buffer1;
        array[1] = "\338\33rDir";
    }

    return (0);
}

/****************************************

    Function : HookFuncObjStrVolList

    Goal     : Set the dir of LST_DIR
               according to assigns
               list selection

****************************************/
SAVEDS ASM VOID HookFuncObjStrVolList(REG(a2) Object *pop, REG(a1) Object *str)
{
    char *x;
    DoMethod(pop, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &x);
    DoMethod(LST_DIR, MUIM_Set, MUIA_Dirlist_Directory, x);
    DoMethod(LST_ACT, MUIM_List_Clear, );
    OldClass = NULL;
}

SAVEDS ASM VOID HookFuncObjStrVolList2(REG(a2) Object *pop,REG(a1) Object *str)
{
    char *x;
    DoMethod(pop, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &x);
    DoMethod(LST_DIR2, MUIM_Set, MUIA_Dirlist_Directory, x);
}

/****************************************

    Function : PopUp

    Goal     : Display Action List
               When in AppIcon mode

****************************************/
int PopUp(struct Class *cla, char *file)
{
    int n = 0;
    struct Action *act;

    if (cla->nbaction == 1)
    {
        Runner(cla->action, file, 0);
        return (0);
    }

    act = cla->action;
    CurrentClass = cla;
    strcpy(FullName, file);

    while (pacts[n] != NULL)
    {
        free(pacts[n]);
        pacts[n] = NULL;
        n++;
    }
    n = 0;
    DoMethod(POP_LST, MUIM_List_Clear, );
    while (act != NULL)
    {
        pacts[n] = (char *)malloc(sizeof(act->name) + 1);
        strcpy(pacts[n], act->name);
        act = act->next;
        n++;
    }

    if (CurrentClass != &Directory)
        act = Generic.action;
    else
        act = NULL;

    while (act != NULL)
    {
        pacts[n] = (char *)malloc(sizeof(act->name) + 5);
        sprintf(pacts[n], "\338%s", act->name);
        act = act->next;
        n++;
    }

    pacts[n + 1] = NULL;
    DoMethod(POP_LST, MUIM_List_Insert, &(pacts), n, MUIV_List_Insert_Bottom);

    set(POP_WIN, MUIA_Window_LeftEdge, MUIV_Window_LeftEdge_Moused);
    set(POP_WIN, MUIA_Window_TopEdge, MUIV_Window_TopEdge_Moused);
    set(POP_WIN, MUIA_Window_Open, TRUE);
}

/****************************************

	Function : HookFuncAppMessage

	Goal     : Handle App Messages

****************************************/
SAVEDS ASM LONG HookFuncAppMessage(REG(A2) APTR obj, REG(A1) struct AppMessage **x)
{
	struct AppMessage *msg = *x;
	struct WBArg *ap;
	int state;
	struct FileInfoBlock *fib = NULL;
	int ar;

	get(app, MUIA_Application_Iconified, &state);

	ap = msg->am_ArgList;
	if (state == 0)
	{
		NameFromLock(ap->wa_Lock, FullName, 1023);
		DoMethod(LST_DIR, MUIM_Set, MUIA_Dirlist_Directory, FullName);
		DoMethod(LST_ACT, MUIM_List_Clear, );
		OldClass = NULL;
		LastDir = -1;

		AddPart(FullName, ap->wa_Name, 1023);
	}
	else
		for (ar = 0; ar < 1; ar++)
		{
			ap = &(msg->am_ArgList[ar]);
			NameFromLock(ap->wa_Lock, FullName, 1023);
			if (strlen(ap->wa_Name) != 0)
			{
				AddPart(FullName, ap->wa_Name, 1023);
				CurrentClass = Getclass(FullName, ap->wa_Name, 0);
			}
			else
				CurrentClass = &Directory;

			PopUp(CurrentClass, FullName);
		}
}

/****************************************

	Function : Action2

	Goal     : We dblcliked on a file
			   Let launch the 1st action

****************************************/
void Action2(char *pat, char *fname, int pos)
{
	struct Class *cla;

	cla = Getclass(pat, fname, 0);
	Runner(cla->action, pat, 0);
}

/****************************************

	Function : HookFuncChoixDirList

	Goal     : User selected a file

****************************************/
ULONG __saveds __asm HookFuncChoixDirList()
{
	struct FileInfoBlock *fib;
	char *path;
	struct Action *act;
	int n = 0;

	DoMethod(LST_DIR, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &fib);
	if (fib)
		if (fib->fib_DirEntryType < 0)
		{
			get(LST_DIR, MUIA_Dirlist_Path, &path);
			set(TXT_FILENAME, MUIA_Text_Contents, fib->fib_FileName);
			CurrentClass = Getclass(path, fib->fib_FileName, 0);
			set(TXT_CLASS, MUIA_Text_Contents, CurrentClass->name);
		}
		else
		{
			set(TXT_CLASS, MUIA_Text_Contents, LocaleMsg[CA_DIR]);
			set(TXT_FILENAME, MUIA_Text_Contents, fib->fib_FileName);
			CurrentClass = &Directory;
		}

	if (CurrentClass != OldClass)
	{
		OldClass = CurrentClass;
		act = CurrentClass->action;
		while (acts[n] != NULL)
		{
			free(acts[n]);
			acts[n] = NULL;
			n++;
		}
		n = 0;
		DoMethod(LST_ACT, MUIM_List_Clear, );
		while (act != NULL)
		{
			acts[n] = (char *)malloc(sizeof(act->name) + 1);
			strcpy(acts[n], act->name);
			act = act->next;
			n++;
		}

		if (CurrentClass != &Generic && CurrentClass != &Directory)
		{
			act = Generic.action;
			while (act != NULL)
			{
				acts[n] = (char *)malloc(sizeof(act->name) + 5);
				sprintf(acts[n], "\338%s", act->name);
				act = act->next;
				n++;
			}
		}

		acts[n + 1] = NULL;
		set(LST_ACT, MUIA_List_Quiet, TRUE);
		DoMethod(LST_ACT, MUIM_List_Insert, &(acts), n, MUIV_List_Insert_Bottom);
		set(LST_ACT, MUIA_List_Quiet, FALSE);
	}

	set(window, MUIA_Window_ActiveObject, LST_DIR);
}

/****************************************

	Function : HookFuncDoubleClickDirList

	Goal     : We have dbl clicked on a
			   file/dir on files LV

****************************************/
ULONG __saveds __asm HookFuncDoubleClickDirList()
{
	struct FileInfoBlock *fib;
	char *path;

	DoMethod(LST_DIR, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &fib);
	if (fib)
		if (fib->fib_DirEntryType < 0)
		{

			get(LST_DIR, MUIA_Dirlist_Path, &path);
			strcpy(FullName, path);
			Action2(path, fib->fib_FileName, 0);
		}
		else if (fib->fib_DirEntryType > 0)
		{
			get(LST_DIR, MUIA_Dirlist_Path, &path);
			DoMethod(LST_DIR, MUIM_Set, MUIA_Dirlist_Directory, path);
			DoMethod(LST_ACT, MUIM_List_Clear, );
			OldClass = NULL;
			LastDir = -1;
		}
}

ULONG __saveds __asm HookFuncDoubleClickDirList2()
{
	struct FileInfoBlock *fib;
	char *path;

	DoMethod(LST_DIR2, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &fib);
	if (fib)
		if (fib->fib_DirEntryType > 0)
		{
			get(LST_DIR2, MUIA_Dirlist_Path, &path);
			DoMethod(LST_DIR2, MUIM_Set, MUIA_Dirlist_Directory, path);
		}
}

/****************************************

	Function : HookFuncListeValide

	Goal     : Dir Scan Complete && OK

****************************************/
ULONG __saveds __asm HookFuncListeValide()
{
	char *path;
	BPTR lock;

	get(LST_DIR, MUIA_Dirlist_Directory, &path);

	lock = Lock(path, ACCESS_READ);
	if (lock)
	{
		NameFromLock(lock, MAX_STR, sizeof(MAX_STR));
		path = MAX_STR;
		UnLock(lock);
	}

	DoMethod(STR_PATH, MUIM_Set, MUIA_String_Contents, path);
	strcpy(CurrentPath, path);
	DoMethod(LST_ACT, MUIM_List_Clear, );
	OldClass = NULL;
}

ULONG __saveds __asm HookFuncListeValide2()
{
	char *path;
	BPTR lock;

	get(LST_DIR2, MUIA_Dirlist_Directory, &path);

	lock = Lock(path, ACCESS_READ);
	if (lock)
	{
		NameFromLock(lock, MAX_STR, sizeof(MAX_STR));
		path = MAX_STR;
		UnLock(lock);
	}

	DoMethod(STR_PATH2, MUIM_Set, MUIA_String_Contents, path);
}

/****************************************

	Function : HookFuncPrefs

	Goal     : Click on Prefs button
			   Run ClassActionPrefs

****************************************/
ULONG __saveds __asm HookFuncPrefs()
{

	struct Action prefs;

	strcpy(prefs.exec, "[f]");
	prefs.wbrun = 1;
	strcpy(prefs.stack, "16000");
	strcpy(prefs.delay, "0");
	prefs.next = NULL;

	Runner(&prefs, PrefsPath, 0);
}

/****************************************

	Function : HookFuncRequesterVers

	Goal     : Click on Version button
			   Display it

****************************************/
ULONG __saveds __asm HookFuncRequesterVers()
{
	char st[1024];

	sprintf(st, "%s\n(c) 1993-2025 Salim GASMI - salim@gasmi.net", VERS);
	strcat(st, "\n\nClassAction uses the MUI object library.\nMUI is (c) 1993-1996 by Stefan Stuntz");

	MUInfo(st);
}

/****************************************

	Function : HookFuncParent

	Goal     : Click on Parent button
			   get parent dir && rescan

****************************************/
ULONG __saveds __asm HookFuncParent()
{
	char *path;
	BPTR lockcur, lockpar;

	get(LST_DIR, MUIA_Dirlist_Directory, &path);

	lockcur = Lock(path, ACCESS_READ);
	if (lockcur)
	{
		lockpar = ParentDir(lockcur);
		if (lockpar)
		{
			NameFromLock(lockpar, MAX_STR, sizeof(MAX_STR));
			DoMethod(LST_DIR, MUIM_Set, MUIA_Dirlist_Directory, MAX_STR);
			DoMethod(LST_ACT, MUIM_List_Clear, );
			OldClass = NULL;

			UnLock(lockpar);
		}
		UnLock(lockcur);
	}
}

ULONG __saveds __asm HookFuncParent2()
{
	char *path;
	BPTR lockcur, lockpar;

	get(LST_DIR2, MUIA_Dirlist_Directory, &path);

	lockcur = Lock(path, ACCESS_READ);
	if (lockcur)
	{
		lockpar = ParentDir(lockcur);
		if (lockpar)
		{
			NameFromLock(lockpar, MAX_STR, sizeof(MAX_STR));
			DoMethod(LST_DIR2, MUIM_Set, MUIA_Dirlist_Directory, MAX_STR);
			UnLock(lockpar);
		}
		UnLock(lockcur);
	}
}

/****************************************

	Function : HookFuncAct

	Goal     : Click on an Action
			   Do all the stuff

****************************************/

ULONG __saveds __asm HookFuncAct()
{
	long a;
	int i;
	struct Action *act;
	struct FileInfoBlock *fib;
	LONG id;
	char pa[512];
	char *pat;

	act = CurrentClass->action;
	get(LST_ACT, MUIA_List_Active, &a);
	if (a > CurrentClass->nbaction - 1)
	{
		act = Generic.action;
		a -= CurrentClass->nbaction;
	}

	for (i = 0; i < a; i++)
		act = act->next;

	get(LST_DIR, MUIA_Dirlist_Directory, &pat);

	strcpy(pa, pat);

	if (pa[strlen(pa) - 1] != ':')
		strcat(pa, "/");

	id = MUIV_List_NextSelected_Start;
	DoMethod(LST_DIR, MUIM_List_NextSelected, &id);
	DoMethod(LST_DIR, MUIM_List_NextSelected, &id);

	id = MUIV_List_NextSelected_Start;

	for (;;)
	{
		DoMethod(LST_DIR, MUIM_List_NextSelected, &id);
		if (id == MUIV_List_NextSelected_End)
			break;

		DoMethod(LST_DIR, MUIM_List_GetEntry, id, &fib);
		sprintf(FullName, "%s%s", pa, fib->fib_FileName);
		Runner(act, FullName, 0);
		DoMethod(LST_DIR, MUIM_List_Select, id, MUIV_List_Select_Off, NULL);
	}

	if (act->wbrun >= 10)
		DoMethod(LST_DIR, MUIM_Dirlist_ReRead);
}

/****************************************

	Function : HookFuncPopc

	Goal     : Clicked on an action
			   of the Popper Win

****************************************/
ULONG __saveds __asm HookFuncPopc()
{
	long a;
	int i;
	struct Action *act;

	set(POP_WIN, MUIA_Window_Open, FALSE);
	act = CurrentClass->action;
	get(POP_LST, MUIA_List_Active, &a);

	if (a > CurrentClass->nbaction - 1)
	{
		act = Generic.action;
		a -= CurrentClass->nbaction;
	}

	for (i = 0; i < a; i++)
		act = act->next;

	Runner(act, FullName, 0);
	if (act->wbrun > 10)
		DoMethod(LST_DIR, MUIM_Dirlist_ReRead);
}

/****************************************

	Function : HookFuncDrive

	Goal     : Clicked on a drive button

****************************************/
ULONG __saveds __asm HookFuncDrive(register __a0 struct Hook *hook, register __a2 APTR object, register __a1 APTR message)
{
	int i, good;

	for (i = 0; i < NbDrv; i++)
		if (BT_DRIVES[i] == object)
			good = i;

	if (good == LastDir)
		DoMethod(LST_DIR, MUIM_Dirlist_ReRead);
	else
		DoMethod(LST_DIR, MUIM_Set, MUIA_Dirlist_Directory, ButPath[good]);

	LastDir = good;
	DoMethod(LST_ACT, MUIM_List_Clear, );
	OldClass = NULL;
}

/****************************************

	Function : HookFuncPath

	Goal     : User pressed Enter
			   on Path String

****************************************/
ULONG __saveds __asm HookFuncPath()
{
	char *ptr, *ptr2;

	get(STR_PATH, MUIA_String_Contents, &ptr);
	get(LST_DIR, MUIA_Dirlist_Directory, &ptr2);
	if (strcmp(ptr, ptr2) == 0)
		DoMethod(LST_DIR, MUIM_Dirlist_ReRead);
	else
		DoMethod(LST_DIR, MUIM_Set, MUIA_Dirlist_Directory, ptr);
	DoMethod(LST_ACT, MUIM_List_Clear, );
	OldClass = NULL;
}

ULONG __saveds __asm HookFuncPath2()
{
	char *ptr, *ptr2;

	get(STR_PATH2, MUIA_String_Contents, &ptr);
	get(LST_DIR2, MUIA_Dirlist_Directory, &ptr2);
	if (strcmp(ptr, ptr2) == 0)
		DoMethod(LST_DIR2, MUIM_Dirlist_ReRead);
	else
		DoMethod(LST_DIR2, MUIM_Set, MUIA_Dirlist_Directory, ptr);
}

/****************************************

	Function : HookFuncQuit

	Goal     : User clicked on Quit button

****************************************/
ULONG __saveds __asm HookFuncQuit()
{
	ByeBye = 1;
}

/****************************************

	Function : HookFuncRename

	Goal     : User clicked on Rename button

****************************************/
ULONG __saveds __asm HookFuncRename()
{
	struct FileInfoBlock *fib;
	LONG id;
	char pa[512];
	char *pat;
	char fullname[1024];
	struct Action act;
	char real[40];
	char dest[512];

	if (OldClass == NULL)
		return (0);

	act.wbrun = 2;

	get(LST_DIR, MUIA_Dirlist_Directory, &pat);

	strcpy(pa, pat);
	if (pa[strlen(pa) - 1] != ':')
		strcat(pa, "/");

	id = MUIV_List_NextSelected_Start;
	DoMethod(LST_DIR, MUIM_List_NextSelected, &id);
	DoMethod(LST_DIR, MUIM_List_NextSelected, &id);

	id = MUIV_List_NextSelected_Start;

	for (;;)
	{
		DoMethod(LST_DIR, MUIM_List_NextSelected, &id);
		if (id == MUIV_List_NextSelected_End)
			break;

		DoMethod(LST_DIR, MUIM_List_GetEntry, id, &fib);
		sprintf(fullname, "%s%s", pa, fib->fib_FileName);
		strcpy(real, fib->fib_FileName);
		if (GetText(real, 39, LocaleMsg[CA_RENAME]) != -1)
		{
			sprintf(dest, "%s%s", pa, real);
			sprintf(act.exec, "C:Rename [f] %c%s%c", '"', dest, '"');
			Runner(&act, fullname, 1);

			/* Do for the Icon as well */

			strcat(dest, ".info");
			strcat(fullname, ".info");
			sprintf(act.exec, "C:Rename [f] %c%s%c", '"', dest, '"');
			Runner(&act, fullname, 1);
		}
		DoMethod(LST_DIR, MUIM_List_Select, id, MUIV_List_Select_Off, NULL);
	}

	DoMethod(LST_DIR, MUIM_Dirlist_ReRead);
}

/****************************************

	Function : HookFuncClone

	Goal     : User clicked on Clone button

****************************************/
ULONG __saveds __asm HookFuncClone()
{
	struct FileInfoBlock *fib;
	LONG id;
	char pa[512];
	char *pat;
	char dest[512];
	char fullname[1024];
	struct Action act;
	char real[40];
	char good[1024];

	if (OldClass == NULL)
		return (0);

	act.wbrun = 2;

	get(LST_DIR, MUIA_Dirlist_Directory, &pat);

	strcpy(pa, pat);
	if (pa[strlen(pa) - 1] != ':')
		strcat(pa, "/");

	get(LST_DIR2, MUIA_Dirlist_Directory, &pat);

	strcpy(dest, pat);
	if (dest[strlen(dest) - 1] != ':')
		strcat(dest, "/");

	id = MUIV_List_NextSelected_Start;
	DoMethod(LST_DIR, MUIM_List_NextSelected, &id);
	DoMethod(LST_DIR, MUIM_List_NextSelected, &id);

	id = MUIV_List_NextSelected_Start;

	for (;;)
	{
		DoMethod(LST_DIR, MUIM_List_NextSelected, &id);
		if (id == MUIV_List_NextSelected_End)
			break;

		DoMethod(LST_DIR, MUIM_List_GetEntry, id, &fib);
		sprintf(fullname, "%s%s", pa, fib->fib_FileName);
		strcpy(real, fib->fib_FileName);
		if (GetText(real, 39, LocaleMsg[CA_CLONE]) != -1)
		{
			sprintf(good, "%s%s", dest, real);
			sprintf(act.exec, "C:Copy [f] %c%s%c ALL", '"', good, '"');
			Runner(&act, fullname, 1);

			/* Icon as well */

			strcat(good, ".info");
			strcat(fullname, ".info");
			sprintf(act.exec, "C:Copy [f] %c%s%c", '"', good, '"');
			Runner(&act, fullname, 1);
		}

		DoMethod(LST_DIR, MUIM_List_Select, id, MUIV_List_Select_Off, NULL);
	}

	DoMethod(LST_DIR2, MUIM_Dirlist_ReRead);
}

/****************************************

	Function : HookFuncCopy

	Goal     : User clicked on Copy button

****************************************/
ULONG __saveds __asm HookFuncCopy()
{

	struct FileInfoBlock *fib;
	LONG id;
	char pa[512];
	char *pat;
	char dest[512];
	char fullname[1024];
	struct Action act;
	char good[1024];

	if (OldClass == NULL)
		return (0);

	act.wbrun = 2;

	get(LST_DIR, MUIA_Dirlist_Directory, &pat);

	strcpy(pa, pat);
	if (pa[strlen(pa) - 1] != ':')
		strcat(pa, "/");

	get(LST_DIR2, MUIA_Dirlist_Directory, &pat);

	strcpy(dest, pat);
	if (dest[strlen(dest) - 1] != ':')
		strcat(dest, "/");

	id = MUIV_List_NextSelected_Start;
	DoMethod(LST_DIR, MUIM_List_NextSelected, &id);
	DoMethod(LST_DIR, MUIM_List_NextSelected, &id);

	id = MUIV_List_NextSelected_Start;

	for (;;)
	{
		DoMethod(LST_DIR, MUIM_List_NextSelected, &id);
		if (id == MUIV_List_NextSelected_End)
			break;

		DoMethod(LST_DIR, MUIM_List_GetEntry, id, &fib);
		sprintf(fullname, "%s%s", pa, fib->fib_FileName);
		sprintf(good, "%s%s", dest, fib->fib_FileName);
		sprintf(act.exec, "C:Copy [f] %c%s%c ALL", '"', good, '"');
		Runner(&act, fullname, 1);

		/* icon as well */

		strcat(fullname, ".info");
		strcat(good, ".info");
		sprintf(act.exec, "C:Copy [f] %c%s%c ALL", '"', good, '"');
		Runner(&act, fullname, 1);

		DoMethod(LST_DIR, MUIM_List_Select, id, MUIV_List_Select_Off, NULL);
	}

	DoMethod(LST_DIR2, MUIM_Dirlist_ReRead);
}

/****************************************

	Function : HookFuncDelete

	Goal     : User clicked on Delete button

****************************************/
ULONG __saveds __asm HookFuncDelete()
{
	struct FileInfoBlock *fib;
	LONG id;
	char pa[512];
	char *pat;
	char fullname[1024];
	struct Action act;

	if (OldClass == NULL)
		return (0);

	act.wbrun = 2;
	strcpy(act.exec, "C:Delete [f] ALL");

	get(LST_DIR, MUIA_Dirlist_Directory, &pat);

	strcpy(pa, pat);
	if (pa[strlen(pa) - 1] != ':')
		strcat(pa, "/");

	id = MUIV_List_NextSelected_Start;
	DoMethod(LST_DIR, MUIM_List_NextSelected, &id);
	DoMethod(LST_DIR, MUIM_List_NextSelected, &id);

	id = MUIV_List_NextSelected_Start;

	if (MuiReq(LocaleMsg[CA_DEL_OK]) != 1)
		return (0);

	for (;;)
	{
		DoMethod(LST_DIR, MUIM_List_NextSelected, &id);
		if (id == MUIV_List_NextSelected_End)
			break;

		DoMethod(LST_DIR, MUIM_List_GetEntry, id, &fib);
		sprintf(fullname, "%s%s", pa, fib->fib_FileName);
		Runner(&act, fullname, 1);

		/* Icon as well */

		strcat(fullname, ".info");
		Runner(&act, fullname, 1);

		DoMethod(LST_DIR, MUIM_List_Select, id, MUIV_List_Select_Off, NULL);
	}

	DoMethod(LST_DIR, MUIM_Dirlist_ReRead);
}

/****************************************

	Function : HookFuncMkDir

	Goal     : User clicked on MakeDir button

****************************************/
ULONG __saveds __asm HookFuncMkDir()
{
	char pa[512];
	char *pat;
	char fullname[1024];
	struct Action act;
	char real[40];

	act.wbrun = 2;
	strcpy(act.exec, "C:MakeDir [f]");

	get(LST_DIR, MUIA_Dirlist_Directory, &pat);

	strcpy(pa, pat);
	if (pa[strlen(pa) - 1] != ':')
		strcat(pa, "/");

	real[0] = 0;

	if (GetText(real, 39, LocaleMsg[CA_MKDIR]) != -1)
	{
		sprintf(fullname, "%s%s", pa, real);
		Runner(&act, fullname, 1);

		/* Create Icon as Well */
		strcpy(act.exec, "C:Copy SYS:Prefs/Env-Archive/Sys/def_drawer.info [f]");
		strcat(fullname, ".info");
		Runner(&act, fullname, 1);
	}

	DoMethod(LST_DIR, MUIM_Dirlist_ReRead);
}

/****************************************

	Function : HookFuncMove

	Goal     : User clicked on Move button

****************************************/
ULONG __saveds __asm HookFuncMove()
{

	struct FileInfoBlock *fib;
	LONG id;
	char pa[512];
	char *pat;
	char dest[512];
	char fullname[1024];
	struct Action act;
	char real[40];
	char good[1024];
	int diskcmp;

	if (OldClass == NULL)
		return (0);

	act.wbrun = 2;

	get(LST_DIR, MUIA_Dirlist_Directory, &pat);

	strcpy(pa, pat);
	if (pa[strlen(pa) - 1] != ':')
		strcat(pa, "/");

	get(LST_DIR2, MUIA_Dirlist_Directory, &pat);

	strcpy(dest, pat);
	if (dest[strlen(dest) - 1] != ':')
		strcat(dest, "/");

	id = MUIV_List_NextSelected_Start;
	DoMethod(LST_DIR, MUIM_List_NextSelected, &id);
	DoMethod(LST_DIR, MUIM_List_NextSelected, &id);

	id = MUIV_List_NextSelected_Start;
	diskcmp = DiskCmp(pa, dest);

	for (;;)
	{
		DoMethod(LST_DIR, MUIM_List_NextSelected, &id);
		if (id == MUIV_List_NextSelected_End)
			break;

		DoMethod(LST_DIR, MUIM_List_GetEntry, id, &fib);
		sprintf(fullname, "%s%s", pa, fib->fib_FileName);
		strcpy(real, fib->fib_FileName);
		sprintf(good, "%s%s", dest, real);
		if (diskcmp == 0)
			rename(fullname, good);
		else
		{
			/* Copy + Delete */
			sprintf(act.exec, "C:Copy [f] %c%s%c ALL", '"', good, '"');
			Runner(&act, fullname, 1);
			sprintf(act.exec, "C:Delete [f] ALL");
			Runner(&act, fullname, 1);
		}

		/* Icon as well */
		strcat(fullname, ".info");
		strcat(good, ".info");

		if (diskcmp == 0)
			rename(fullname, good);
		else
		{
			/* Copy + Delete */
			sprintf(act.exec, "C:Copy [f] %c%s%c ALL", '"', good, '"');
			Runner(&act, fullname, 1);
			sprintf(act.exec, "C:Delete [f] ALL");
			Runner(&act, fullname, 1);
		}

		DoMethod(LST_DIR, MUIM_List_Select, id, MUIV_List_Select_Off, NULL);
	}

	DoMethod(LST_DIR, MUIM_Dirlist_ReRead);
	DoMethod(LST_DIR2, MUIM_Dirlist_ReRead);
}

/****************************************

	Function : HookFuncSwap

	Goal     : User clicked on Swap button

****************************************/
ULONG __saveds __asm HookFuncSwap()
{
	char *s, *d;
	char ss[512], dd[512];

	get(STR_PATH, MUIA_String_Contents, &s);
	get(STR_PATH2, MUIA_String_Contents, &d);

	strcpy(ss, s);
	strcpy(dd, d);

	DoMethod(STR_PATH, MUIM_Set, MUIA_String_Contents, dd);
	DoMethod(STR_PATH2, MUIM_Set, MUIA_String_Contents, ss);
	DoMethod(LST_DIR, MUIM_Set, MUIA_Dirlist_Directory, dd);
	DoMethod(LST_DIR2, MUIM_Set, MUIA_Dirlist_Directory, ss);
}

/****************************************

	Function : HookFuncKey

	Goal     : Select file starting
			   with pressed key

****************************************/
ULONG __saveds __asm HookFuncKey(register __a0 struct Hook *hook, register __a2 APTR object, register __a1 APTR message)
{
	char ch[200];
	struct InputEvent *x;
	struct FileInfoBlock *fib;
	int i;
	long n;

	get(LST_DIR, MUIA_List_Entries, &n);

	if (n < 2)
		return (0);

	get(window, MUIA_Window_InputEvent, &x);

	RawKeyConvert(x, ch, 1, NULL);
	ch[1] = 0;

	for (i = 0; i < n; i++)
	{
		DoMethod(LST_DIR, MUIM_List_GetEntry, i, &fib);

		if (ch[0] > 'Z' && fib->fib_DirEntryType < 0 && tolower(ch[0]) == tolower(fib->fib_FileName[0]))
		{
			set(LST_DIR, MUIA_List_Active, i);
			return (0);
		}

		if (ch[0] < 'a' && fib->fib_DirEntryType > 0 && tolower(ch[0]) == tolower(fib->fib_FileName[0]))
		{
			set(LST_DIR, MUIA_List_Active, i);
			return (0);
		}
	}
}

/****************************************

	Function : HookFuncPat

	Goal     : Select files matching
			   a pattern

****************************************/
ULONG __saveds __asm HookFuncPat(register __a0 struct Hook *hook, register __a2 APTR object, register __a1 APTR message)
{
	int l;
	char pat[100];
	long n;
	int i;
	struct FileInfoBlock *fib;
	int k;

	strcpy(pat, "#?");
	l = GetText(pat, 100, "Enter Pattern");

	if (l < 0)
		return (0);

	ParsePatternNoCase(pat, DirPattern, 208);

	get(LST_DIR, MUIA_List_Entries, &n);

	k = -1;

	for (i = 0; i < n; i++)
	{
		DoMethod(LST_DIR, MUIM_List_GetEntry, i, &fib);
		if (fib->fib_DirEntryType < 0 && MatchPatternNoCase(DirPattern, fib->fib_FileName) == 1)
		{
			if (k == -1)
				k = i;
			DoMethod(LST_DIR, MUIM_List_Select, i, MUIV_List_Select_On, NULL);
		}
	}

	set(LST_DIR, MUIA_List_Active, k);
}

/****************************************

	Function : HookFuncAll

	Goal     : Select all files

****************************************/
ULONG __saveds __asm HookFuncAll(register __a0 struct Hook *hook, register __a2 APTR object, register __a1 APTR message)
{

	long n;

	set(LST_DIR, MUIA_Dirlist_MultiSelDirs, FALSE);
	DoMethod(LST_DIR, MUIM_List_Select, MUIV_List_Select_All, MUIV_List_Select_On, NULL);
	set(LST_DIR, MUIA_Dirlist_MultiSelDirs, TRUE);

	get(LST_DIR, MUIA_List_Entries, &n);
	set(LST_DIR, MUIA_List_Active, n - 1);
}

/****************************************

	Function : HookFuncNone

	Goal     : Deselect all files

****************************************/
ULONG __saveds __asm HookFuncNone(register __a0 struct Hook *hook, register __a2 APTR object, register __a1 APTR message)
{
	DoMethod(LST_DIR, MUIM_List_Select, MUIV_List_Select_All, MUIV_List_Select_Off, NULL);
}

/****************************************

	Function : HookFuncTog

	Goal     : Toggle selection of all files

****************************************/
ULONG __saveds __asm HookFuncTog(register __a0 struct Hook *hook, register __a2 APTR object, register __a1 APTR message)
{
	DoMethod(LST_DIR, MUIM_List_Select, MUIV_List_Select_All, MUIV_List_Select_Toggle, NULL);
}

/****************************************

	Function : HookFuncBack

	Goal     : Go back to previous dir
****************************************/
ULONG __saveds __asm HookFuncBack(register __a0 struct Hook *hook, register __a2 APTR object, register __a1 APTR message)
{
	HookFuncParent();
}

/****************************************

	Function : HookFuncType

	Goal     : Select files of a certain type

****************************************/
ULONG __saveds __asm HookFuncType(register __a0 struct Hook *hook, register __a2 APTR object, register __a1 APTR message)
{
	char *p;
	long n;
	int i;
	struct FileInfoBlock *fib;
	struct Class *cla;
	char path[512], full[1024];

	if (CurrentClass == NULL)
		return (0);

	get(LST_DIR, MUIA_List_Entries, &n);
	get(LST_DIR, MUIA_Dirlist_Path, &p);

	strcpy(path, p);
	p = PathPart(path);
	*p = 0;

	for (i = 0; i < n; i++)
	{

		DoMethod(LST_DIR, MUIM_List_GetEntry, i, &fib);
		if (fib->fib_DirEntryType < 0)
		{
			strcpy(full, path);
			AddPart(full, fib->fib_FileName, 1024);
			cla = Getclass(full, fib->fib_FileName, 0);
			if (cla == CurrentClass)
				DoMethod(LST_DIR, MUIM_List_Select, i, MUIV_List_Select_On, NULL);
		}
	}
}

/***** The Hooks themselves *****/
struct Hook HookAppMessage         	= { {NULL, NULL}, (void *)HookFuncAppMessage,NULL,NULL};
struct Hook HookChoixDirList       	= { {NULL, NULL}, (void *)HookFuncChoixDirList,NULL,NULL};
struct Hook HookDoubleClickDirList 	= { {NULL, NULL}, (void *)HookFuncDoubleClickDirList,NULL,NULL};
struct Hook HookDoubleClickDirList2	= { {NULL, NULL}, (void *)HookFuncDoubleClickDirList2,NULL,NULL};
struct Hook HookListeValide        	= { {NULL, NULL}, (void *)HookFuncListeValide,NULL,NULL};
struct Hook HookListeValide2       	= { {NULL, NULL}, (void *)HookFuncListeValide2,NULL,NULL};
struct Hook HookPrefs	           	= { {NULL, NULL}, (void *)HookFuncPrefs,NULL,NULL};
struct Hook HookRequesterVers      	= { {NULL, NULL}, (void *)HookFuncRequesterVers,NULL,NULL};
struct Hook HookParent2            	= { {NULL, NULL}, (void *)HookFuncParent2,NULL,NULL};
struct Hook HookParent             	= { {NULL, NULL}, (void *)HookFuncParent,NULL,NULL};
struct Hook HookAct                	= { {NULL, NULL}, (void *)HookFuncAct,NULL,NULL};
struct Hook HookDrive              	= { {NULL, NULL}, (void *)HookFuncDrive,NULL,NULL};
struct Hook HookPopc               	= { {NULL, NULL}, (void *)HookFuncPopc,NULL,NULL};
struct Hook HookObjStrVolList2     	= { {NULL, NULL}, (VOID *)HookFuncObjStrVolList2,NULL,NULL };
struct Hook HookObjStrVolList      	= { {NULL, NULL}, (VOID *)HookFuncObjStrVolList,NULL,NULL };
struct Hook HookDspList      	   	= { {NULL, NULL}, (VOID *)HookFuncDspList,NULL,NULL };
struct Hook HookDspList2     	   	= { {NULL, NULL}, (VOID *)HookFuncDspList2,NULL,NULL };
struct Hook HookPath         	   	= { {NULL, NULL}, (VOID *)HookFuncPath,NULL,NULL };
struct Hook HookPath2        	   	= { {NULL, NULL}, (VOID *)HookFuncPath2,NULL,NULL };
struct Hook HookQuit        	   	= { {NULL, NULL}, (VOID *)HookFuncQuit,NULL,NULL };
struct Hook HookCopy        	   	= { {NULL, NULL}, (VOID *)HookFuncCopy,NULL,NULL };
struct Hook HookRename      	   	= { {NULL, NULL}, (VOID *)HookFuncRename,NULL,NULL };
struct Hook HookClone       	   	= { {NULL, NULL}, (VOID *)HookFuncClone,NULL,NULL };
struct Hook HookMakeDir     	   	= { {NULL, NULL}, (VOID *)HookFuncMkDir,NULL,NULL };
struct Hook HookDelete      	   	= { {NULL, NULL}, (VOID *)HookFuncDelete,NULL,NULL };
struct Hook HookMove        	   	= { {NULL, NULL}, (VOID *)HookFuncMove,NULL,NULL };
struct Hook HookSwap        	   	= { {NULL, NULL}, (VOID *)HookFuncSwap,NULL,NULL };
struct Hook HookKey         	   	= { {NULL, NULL}, (VOID *)HookFuncKey,NULL,NULL };
struct Hook HookPat         	   	= { {NULL, NULL}, (VOID *)HookFuncPat,NULL,NULL };
struct Hook HookNone         	   	= { {NULL, NULL}, (VOID *)HookFuncNone,NULL,NULL };
struct Hook HookAll          	   	= { {NULL, NULL}, (VOID *)HookFuncAll,NULL,NULL };
struct Hook HookBack         	   	= { {NULL, NULL}, (VOID *)HookFuncBack,NULL,NULL };
struct Hook HookType         	   	= { {NULL, NULL}, (VOID *)HookFuncType,NULL,NULL };
struct Hook HookTog         	   	= { {NULL, NULL}, (VOID *)HookFuncTog,NULL,NULL };

/****************************************

	Function : CreateButtonList

	Goal     : Create the path buttons

****************************************/
APTR CreateButtonList()
{
	APTR grp;
	int t;

	grp = VGroup,
	MUIA_Weight, 25,
	MUIA_Group_SameSize, TRUE,
	MUIA_ShortHelp, LocaleMsg[CA_BLIST],
	End;

	if (grp)
	{
		for (t = 1; t <= MXDRV; t++)
			if (Drives[t - 1][0] != 0)
			{

				char ctrlchar;
				NbDrv++;

				ctrlchar = t + '0';
				if (ctrlchar > '9')
					ctrlchar = '0';
				BT_DRIVES[t - 1] = TextObject,
							  ButtonFrame,
							  MUIA_InputMode, MUIV_InputMode_RelVerify,
							  MUIA_Background, MUII_ButtonBack,
							  MUIA_Text_SetVMax, FALSE,
							  MUIA_Text_Contents, Drives[t - 1],
							  MUIA_Text_PreParse, "\33c",
							  End;

				if (BT_DRIVES[t - 1])
				{
					DoMethod(grp, OM_ADDMEMBER, BT_DRIVES[t - 1]);
					DoMethod(BT_DRIVES[t - 1], MUIM_Notify, MUIA_Pressed, FALSE,
							 BT_DRIVES[t - 1], 2, MUIM_CallHook, &HookDrive);
				}
				else
					fail(BT_DRIVES[t - 1], FAIL_ERROR);
			}
	}
	else
		fail(grp, FAIL_ERROR);

	return grp;
}

/****************************************

	Function : CreateScroll

	Goal     : Create the Virtual group
			   for the path buttons

****************************************/
APTR CreateScroll()
{
{
        return
        (
        ScrollgroupObject,
		MUIA_Weight, 10,
		MUIA_Scrollgroup_FreeHoriz,FALSE,
                MUIA_Scrollgroup_Contents, VirtgroupObject,
                        VirtualFrame,
                        Child, CreateButtonList(),
    			End,
		End
        );
}
}

/****************************************

	Function : main

	Goal     : Build the interface
			   assign fcts to events
			   Initializations

****************************************/
int main(int argc, char *argv[])
{
	void FreeAll(void);
	void InitAll(void);

	/* initializations */

	ULONG SigRexx = (1 << RexxSigBit);
	int registered;
	char jj[2];

	if (argc != 0)
		strcpy(ProgramName, argv[0]);

	MUIMasterBase = (struct Library *)OpenLibrary("muimaster.library", 11);
	if (MUIMasterBase == NULL)
	{
		Inf("Unable to Open MUIMaster library V11+");
		return (0);
	}

	init();
	InitAll();
	acts[0] = NULL;
	pacts[0] = NULL;
	OldClass = NULL;
	Active = -1;

	registered = GetReg(User);
	if (registered != 0)
		strcpy(User, "NOT REGISTERED");

	dobj->do_CurrentX = 500;
	dobj->do_CurrentY = 100;

	/* Build the MUI Interface */
    app = ApplicationObject,
            MUIA_Application_Author,         "Gasmi Salim",
            MUIA_Application_Base,           "ClassAction",
            MUIA_Application_Copyright,      "(c) Gasmi Salim 1997",
            MUIA_Application_Description,    LocaleMsg[CA_COMMOD],
            MUIA_Application_Title,          "ClassAction",
            MUIA_Application_Version,        VERS,
            MUIA_Application_SingleTask,     TRUE,
            MUIA_Application_UseCommodities, TRUE,
            MUIA_Application_UseRexx,        FALSE,
            MUIA_Application_DiskObject,     dobj,

            SubWindow, window = WindowObject,
                MUIA_Window_Title,     VERS,
                MUIA_Window_ID,        MAKE_ID('W','I','N','1'),
                MUIA_Window_AppWindow, TRUE,
	        MUIA_Window_DefaultObject,LST_DIR,

                WindowContents, HGroup,
                    Child, CreateScroll(),

                    Child, VGroup,
                        MUIA_Weight, 70,
                        Child, LST_DIR = ListviewObject,
                                  /* MUIA_ShortHelp, LocaleMsg[CA_MLIST], */
                                   MUIA_CycleChain, TRUE,
                                   MUIA_Listview_MultiSelect, TRUE,
                                   MUIA_Listview_List, DirlistObject,
                                       InputListFrame,
                                       MUIA_List_Format, ",MIW=100 P=\33r",
                                       MUIA_Dirlist_Directory, "SYS:",
                                       MUIA_Dirlist_RejectIcons, TRUE,
				       MUIA_Dirlist_MultiSelDirs, TRUE,
                                   End,
                               End,

                        Child, HGroup,
                            Child, popup = PopobjectObject,
                                MUIA_Popstring_Button, PopButton(MUII_PopUp),
                                MUIA_Popstring_String, STR_PATH = StringObject,
                                    MUIA_CycleChain, TRUE,
                                    /* MUIA_ShortHelp, LocaleMsg[CA_TPATH], */
                                    StringFrame,
                                End,
                                MUIA_Popobject_ObjStrHook, &HookObjStrVolList,
                                MUIA_Popobject_Object, ListviewObject,
                                MUIA_ShortHelp, LocaleMsg[CA_VOLS],
                                   MUIA_Listview_List, LST_VOL = VolumelistObject,
                                       InputListFrame,
                                       MUIA_List_Format, ",,P=\33r,P=\33r",
                                   End,
                               End,

                            End,

                            Child, BT_PARENT = TextObject,
                                ButtonFrame,
                                MUIA_Weight, 25,
                                MUIA_InputMode, MUIV_InputMode_RelVerify,
                                MUIA_Background, MUII_ButtonBack,
                                MUIA_Text_Contents, LocaleMsg[CA_PARENT_BUTT],
                                MUIA_Text_PreParse, "\33c",
                                MUIA_ShortHelp, LocaleMsg[CA_PARENT],
                            End,

			    Child, BT_SWAP = TextObject,
                                ButtonFrame,
                                MUIA_Weight,10,
                                MUIA_InputMode, MUIV_InputMode_RelVerify,
                                MUIA_Background, MUII_ButtonBack,
                                MUIA_Text_Contents, "<->",
                                MUIA_Text_PreParse, "\33c",
                                MUIA_ShortHelp, LocaleMsg[CA_PARENT],
                            End,

                        End,
                    End,
		    Child, BalanceObject,End,
		    Child, VGroup,MUIA_Weight,10,
		    Child, RegisterGroup(Onglet),
                    Child, VGroup,
                        Child, HGroup,
                            Child, VGroup, MUIA_Weight, 40,
                                Child, TextObject,
                                    MUIA_Text_Contents, LocaleMsg[CA_FILE_BUTT],
                                End,

                                Child, TextObject,
                                    MUIA_Text_Contents, LocaleMsg[CA_CLASS_BUTT],
                                End,
                            End,

                            Child, VGroup, MUIA_Weight, 100,
                                Child, TXT_FILENAME = TextObject,
                                    TextFrame,
                                    MUIA_ShortHelp, LocaleMsg[CA_FNAME],
                                    MUIA_Background, MUII_TextBack,
                                End,

                                Child, TXT_CLASS = TextObject,
                                    TextFrame,
                                    MUIA_ShortHelp, LocaleMsg[CA_CNAME],
                                    MUIA_Background, MUII_TextBack,
                                End,
                            End,
                        End,

                        Child, LST_ACT = ListviewObject,
                            /* MUIA_ShortHelp, LocaleMsg[CA_ALIST], */
                            MUIA_Listview_List, ListObject,
                               InputListFrame,
                            End,
                        End,
		    End,
		                     /*  Onglet 2 app */

		    Child, VGroup,
                    Child, VGroup,GroupFrameT(LocaleMsg[CA_DEST]),
                        MUIA_Weight, 100,
                        Child, LST_DIR2 = ListviewObject,
                                   MUIA_Listview_List, DirlistObject,
                                       InputListFrame,
                                       MUIA_List_Format, ",MIW=100 P=\33r",
                                       MUIA_Dirlist_Directory, "SYS:",
                                       MUIA_Dirlist_RejectIcons, TRUE,
				       /*MUIA_Dirlist_DrawersOnly, TRUE,*/
                                   End,
                               End,

                        Child, HGroup,
                            Child, popup2 = PopobjectObject,
                                MUIA_Popstring_Button, PopButton(MUII_PopUp),
                                MUIA_Popstring_String, STR_PATH2 = StringObject,
                                    MUIA_CycleChain, TRUE,
                                    StringFrame,
                                End,
                                MUIA_Popobject_ObjStrHook, &HookObjStrVolList2,
                                MUIA_Popobject_Object, ListviewObject,
                                MUIA_ShortHelp, LocaleMsg[CA_VOLS],
                                   MUIA_Listview_List, LST_VOL2 = VolumelistObject,
                                       InputListFrame,
                                       MUIA_List_Format, ",,P=\33r,P=\33r",
                                   End,
                               End,

                            End,

                            Child, BT_PARENT2 = TextObject,
                                ButtonFrame,
                                MUIA_Weight, 25,
				MUIA_Font,MUIV_Font_Tiny,
                                MUIA_InputMode, MUIV_InputMode_RelVerify,
                                MUIA_Background, MUII_ButtonBack,
                                MUIA_Text_Contents, LocaleMsg[CA_PARENT_BUTT],
                                MUIA_Text_PreParse, "\33c",
                                MUIA_ShortHelp, LocaleMsg[CA_PARENT],
                            End,
                        End,
                    End,
                    	Child, VGroup,GroupFrameT(LocaleMsg[CA_OPTIONS]),
                        Child, HGroup, MUIA_Group_SameSize, TRUE,
                            Child, BT_COPY = TextObject,
                                ButtonFrame,
				MUIA_Font,MUIV_Font_Tiny,
                                MUIA_InputMode, MUIV_InputMode_RelVerify,
                                MUIA_Background, MUII_ButtonBack,
                                MUIA_Text_Contents, LocaleMsg[CA_COPY],
                                MUIA_Text_PreParse, "\33c",
                            End,

                            Child, BT_RENAME = TextObject,
                                ButtonFrame,
				MUIA_Font,MUIV_Font_Tiny,
                                MUIA_InputMode, MUIV_InputMode_RelVerify,
                                MUIA_Background, MUII_ButtonBack,
                                MUIA_Text_Contents, LocaleMsg[CA_RENAME],
                                MUIA_Text_PreParse, "\33c",
                            End,

                            Child, BT_MKDIR = TextObject,
                                ButtonFrame,
				MUIA_Font,MUIV_Font_Tiny,
                                MUIA_InputMode, MUIV_InputMode_RelVerify,
                                MUIA_Background, MUII_ButtonBack,
                                MUIA_Text_Contents, LocaleMsg[CA_MKDIR],
                                MUIA_Text_PreParse, "\33c",
                            End,
                     End,
                        Child, HGroup, MUIA_Group_SameSize, TRUE,
                            Child, BT_CLONE = TextObject,
                                ButtonFrame,
				MUIA_Font,MUIV_Font_Tiny,
                                MUIA_InputMode, MUIV_InputMode_RelVerify,
                                MUIA_Background, MUII_ButtonBack,
                                MUIA_Text_Contents, LocaleMsg[CA_CLONE],
                                MUIA_Text_PreParse, "\33c",
                            End,

                            Child, BT_MOVE = TextObject,
                                ButtonFrame,
				MUIA_Font,MUIV_Font_Tiny,
                                MUIA_InputMode, MUIV_InputMode_RelVerify,
                                MUIA_Background, MUII_ButtonBack,
                                MUIA_Text_Contents, LocaleMsg[CA_MOVE],
                                MUIA_Text_PreParse, "\33c",
                            End,

                            Child, BT_DELETE = TextObject,
                                ButtonFrame,
				MUIA_Font,MUIV_Font_Tiny,
                                MUIA_InputMode, MUIV_InputMode_RelVerify,
                                MUIA_Background, MUII_ButtonBack,
                                MUIA_Text_Contents, LocaleMsg[CA_DELETE],
                                MUIA_Text_PreParse, "\33c",
                            End,
                     End,
		    End,
		    End,
	  	    End,

                        Child, HGroup, MUIA_Group_SameSize, TRUE,
                            Child, BT_INFO = TextObject,
                                ButtonFrame,
                                MUIA_InputMode, MUIV_InputMode_RelVerify,
                                MUIA_Background, MUII_ButtonBack,
                                MUIA_Text_Contents, LocaleMsg[CA_PREFS],
                                MUIA_Text_PreParse, "\33c",
                                MUIA_ShortHelp, LocaleMsg[CA_PREFSH],
                            End,

                            Child, BT_VERS = TextObject,
                                ButtonFrame,
                                MUIA_InputMode, MUIV_InputMode_RelVerify,
                                MUIA_Background, MUII_ButtonBack,
                                MUIA_Text_Contents, LocaleMsg[CA_ABOUT_BUTT],
                                MUIA_Text_PreParse, "\33c",
                                MUIA_ShortHelp, LocaleMsg[CA_ABOUTH],
                            End,

                            Child, BT_FIN  = TextObject,
                                ButtonFrame,
                                MUIA_InputMode, MUIV_InputMode_RelVerify,
                                MUIA_Background, MUII_ButtonBack,
                                MUIA_Text_Contents, LocaleMsg[CA_QUIT_BUTT],
                                MUIA_Text_PreParse, "\33c",
                                MUIA_ShortHelp, LocaleMsg[CA_QUITH],
                            End,
                     End,
                  End,
                End,
            End,
        End;

    appop = ApplicationObject,
            MUIA_Application_Author,         "Gasmi Salim",
            MUIA_Application_Base,           "ClassActionPopper",
            MUIA_Application_Copyright,      "(c) Gasmi Salim 1995",
            MUIA_Application_Description,    "ClassActionPopper",
            MUIA_Application_Title,          "ClassActionPopper",
            MUIA_Application_Version,        "3.0",
            MUIA_Application_UseCommodities, FALSE,
            MUIA_Application_UseRexx,        FALSE,

            SubWindow, POP_WIN = WindowObject,
                MUIA_Window_Title,     "PopWindow",
                MUIA_Window_ID,        MAKE_ID('W','I','N','2'),

                WindowContents, HGroup,
                Child, POP_LST = ListviewObject,
                    MUIA_ShortHelp, LocaleMsg[CA_ALIST],
                    MUIA_Listview_List, ListObject,
                       InputListFrame,
                    End,
                 End,
             End,
             End,
        End;

	if (!appop)
	{
		fail(appop, FAIL_ERROR);
	}

	if (!app)
	{
		MInfo(LocaleMsg[CA_ALREADY_RUNNING]);
		fail(app, FAIL_ERROR);
	}

	/* Assign fcts to events via Hooks */
	if (GtLike == 1)
	{
		set(LST_DIR, MUIA_List_DisplayHook, &HookDspList);
		set(LST_DIR2, MUIA_List_DisplayHook, &HookDspList2);
	}

	if (ShowIcons == 1)
	{
		set(LST_DIR, MUIA_Dirlist_RejectIcons, FALSE);
		set(LST_DIR2, MUIA_Dirlist_RejectIcons, FALSE);
	}

	set(app, MUIA_Application_DropObject, window);

	DoMethod(window, MUIM_Notify, MUIA_AppMessage, MUIV_EveryTime,
			 window, 3, MUIM_CallHook, &HookAppMessage, MUIV_TriggerValue);

	DoMethod(window, MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
			 app, 3, MUIM_Set, MUIA_Application_Iconified, TRUE);

	DoMethod(POP_WIN, MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
			 POP_WIN, 3, MUIM_Set, MUIA_Window_Open, FALSE);

	DoMethod(BT_FIN, MUIM_Notify, MUIA_Pressed, FALSE,
			 app, 2, MUIM_CallHook, &HookQuit);

	DoMethod(STR_PATH, MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime,
			 STR_PATH, 2, MUIM_CallHook, &HookPath);

	DoMethod(STR_PATH2, MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime,
			 STR_PATH2, 2, MUIM_CallHook, &HookPath2);

	DoMethod(LST_DIR, MUIM_Notify, MUIA_List_Active, MUIV_EveryTime,
			 LST_DIR, 2, MUIM_CallHook, &HookChoixDirList);

	DoMethod(LST_DIR, MUIM_Notify, MUIA_Listview_DoubleClick, TRUE,
			 LST_DIR, 2, MUIM_CallHook, &HookDoubleClickDirList);

	DoMethod(LST_DIR2, MUIM_Notify, MUIA_Listview_DoubleClick, TRUE,
			 LST_DIR2, 2, MUIM_CallHook, &HookDoubleClickDirList2);

	DoMethod(LST_DIR, MUIM_Notify, MUIA_Dirlist_Status, MUIV_Dirlist_Status_Invalid,
			 TXT_FILENAME, 3, MUIM_Set, MUIA_Text_Contents, "????");

	DoMethod(LST_DIR, MUIM_Notify, MUIA_Dirlist_Status, MUIV_Dirlist_Status_Invalid,
			 TXT_CLASS, 3, MUIM_Set, MUIA_Text_Contents, "");

	DoMethod(LST_DIR, MUIM_Notify, MUIA_Dirlist_Status, MUIV_Dirlist_Status_Reading,
			 TXT_FILENAME, 3, MUIM_Set, MUIA_Text_Contents, LocaleMsg[CA_READING]);

	DoMethod(LST_DIR, MUIM_Notify, MUIA_Dirlist_Status, MUIV_Dirlist_Status_Reading,
			 TXT_CLASS, 3, MUIM_Set, MUIA_Text_Contents, "");

	DoMethod(LST_DIR, MUIM_Notify, MUIA_Dirlist_Status, MUIV_Dirlist_Status_Valid,
			 TXT_FILENAME, 3, MUIM_Set, MUIA_Text_Contents, "");

	DoMethod(LST_DIR, MUIM_Notify, MUIA_Dirlist_Status, MUIV_Dirlist_Status_Valid,
			 TXT_CLASS, 3, MUIM_Set, MUIA_Text_Contents, "");

	DoMethod(LST_DIR2, MUIM_Notify, MUIA_Dirlist_Status, MUIV_Dirlist_Status_Valid,
			 LST_DIR2, 2, MUIM_CallHook, &HookListeValide2);

	DoMethod(LST_DIR, MUIM_Notify, MUIA_Dirlist_Status, MUIV_Dirlist_Status_Valid,
			 LST_DIR, 2, MUIM_CallHook, &HookListeValide);

	DoMethod(BT_INFO, MUIM_Notify, MUIA_Pressed, FALSE,
			 BT_INFO, 2, MUIM_CallHook, &HookPrefs);

	DoMethod(BT_SWAP, MUIM_Notify, MUIA_Pressed, FALSE,
			 BT_SWAP, 2, MUIM_CallHook, &HookSwap);

	DoMethod(BT_VERS, MUIM_Notify, MUIA_Pressed, FALSE,
			 BT_VERS, 2, MUIM_CallHook, &HookRequesterVers);

	DoMethod(BT_PARENT, MUIM_Notify, MUIA_Pressed, FALSE,
			 BT_PARENT, 2, MUIM_CallHook, &HookParent);

	DoMethod(BT_PARENT2, MUIM_Notify, MUIA_Pressed, FALSE,
			 BT_PARENT2, 2, MUIM_CallHook, &HookParent2);

	DoMethod(LST_ACT, MUIM_Notify, MUIA_Listview_DoubleClick, TRUE,
			 LST_ACT, 2, MUIM_CallHook, &HookAct);

	DoMethod(POP_LST, MUIM_Notify, MUIA_Listview_DoubleClick, TRUE,
			 POP_LST, 2, MUIM_CallHook, &HookPopc);

	DoMethod(LST_VOL, MUIM_Notify, MUIA_Listview_DoubleClick, TRUE,
			 popup, 2, MUIM_Popstring_Close, TRUE);

	DoMethod(LST_VOL2, MUIM_Notify, MUIA_Listview_DoubleClick, TRUE,
			 popup2, 2, MUIM_Popstring_Close, TRUE);

	DoMethod(BT_COPY, MUIM_Notify, MUIA_Pressed, FALSE,
			 BT_COPY, 2, MUIM_CallHook, &HookCopy);

	DoMethod(BT_RENAME, MUIM_Notify, MUIA_Pressed, FALSE,
			 BT_COPY, 2, MUIM_CallHook, &HookRename);

	DoMethod(BT_CLONE, MUIM_Notify, MUIA_Pressed, FALSE,
			 BT_COPY, 2, MUIM_CallHook, &HookClone);

	DoMethod(BT_MOVE, MUIM_Notify, MUIA_Pressed, FALSE,
			 BT_COPY, 2, MUIM_CallHook, &HookMove);

	DoMethod(BT_MKDIR, MUIM_Notify, MUIA_Pressed, FALSE,
			 BT_COPY, 2, MUIM_CallHook, &HookMakeDir);

	DoMethod(BT_DELETE, MUIM_Notify, MUIA_Pressed, FALSE,
			 BT_COPY, 2, MUIM_CallHook, &HookDelete);

	jj[1] = 0;
	for (jj[0] = 'a'; jj[0] <= 'z'; jj[0]++)
		DoMethod(window, MUIM_Notify, MUIA_Window_InputEvent, jj, window, 2, MUIM_CallHook, &HookKey);
	for (jj[0] = 'A'; jj[0] <= 'Z'; jj[0]++)
		DoMethod(window, MUIM_Notify, MUIA_Window_InputEvent, jj, window, 2, MUIM_CallHook, &HookKey);

	if (HKey[0][0] != 0)
		DoMethod(window, MUIM_Notify, MUIA_Window_InputEvent, HKey[0], window, 2, MUIM_CallHook, &HookPat);
	if (HKey[1][0] != 0)
		DoMethod(window, MUIM_Notify, MUIA_Window_InputEvent, HKey[1], window, 2, MUIM_CallHook, &HookAll);
	if (HKey[2][0] != 0)
		DoMethod(window, MUIM_Notify, MUIA_Window_InputEvent, HKey[2], window, 2, MUIM_CallHook, &HookNone);
	if (HKey[3][0] != 0)
		DoMethod(window, MUIM_Notify, MUIA_Window_InputEvent, HKey[3], window, 2, MUIM_CallHook, &HookBack);
	if (HKey[4][0] != 0)
		DoMethod(window, MUIM_Notify, MUIA_Window_InputEvent, HKey[4], window, 2, MUIM_CallHook, &HookType);
	if (HKey[5][0] != 0)
		DoMethod(window, MUIM_Notify, MUIA_Window_InputEvent, HKey[5], window, 2, MUIM_CallHook, &HookTog);

	/* On ouvre la fen�tre */
	set(window, MUIA_Window_Open, TRUE);

	/*
	if(registered!=0)
	MUI_Request(appop,NULL,0,"",LocaleMsg[CA_OKAY],LocaleMsg[CA_SHAREWARE]);
	*/

	if (IconStart == 1)
		set(app, MUIA_Application_Iconified, TRUE);

	ULONG sigs = 0;
	ULONG sigs2 = 0;

	/* Events loop */
	while (DoMethod(app, MUIM_Application_NewInput, &sigs) != MUIV_Application_ReturnID_Quit && ByeBye == 0)
	{
		DoMethod(appop, MUIM_Application_NewInput, &sigs2);
		if (sigs)
		{
			sigs = Wait(sigs | sigs2 | SigRexx | SIGBREAKF_CTRL_C);
			/* if (sigs & SIGBREAKF_CTRL_C) break; */
			if (sigs & SigRexx)
			{
				ProcessRexxCommands(NULL);
				if (Active == 0)
					break;
				if (Active == 2)
					set(app, MUIA_Application_Iconified, TRUE);
				if (Active == 3)
					set(app, MUIA_Application_Iconified, FALSE);

				if (Active == 4)
				{
					DoMethod(LST_DIR, MUIV_List_Select_Active,
							 MUIV_List_Select_Off, NULL);

					DoMethod(LST_ACT, MUIM_List_Clear, );
					set(TXT_FILENAME, MUIA_Text_Contents, "");
					set(TXT_CLASS, MUIA_Text_Contents, "");
					OldClass = NULL;
				}
			}
		}
	}

	/* We quit and free everything */
	/*
	if (registered != 0)
		MUInfo(LocaleMsg[CA_SHAREWARE]);
	*/
	set(window, MUIA_Window_Open, FALSE);
	if (appop)
		MUI_DisposeObject(appop);
	if (app)
		MUI_DisposeObject(app);
	FreeAll();
	exit(0);
}

/****************************************

	Function : FreeAll

	Goal     : Free ALL allocated ress

****************************************/
void FreeAll(void)
{
	int i;

	FreeList(&Alist);
	FreeMemList();
	unlink(WBRUN);
	CloseXfdLib();
	IsAscii(NULL);
	CloseLibrary(WbBase);
	CloseDevice((struct IORequest *)(&IOStdReq));
	FreeLocale();
	FreeRT();
	for (i = 0; i < 100; i++)
	{
		if (acts[i] != NULL)
			free(acts[i]);
		if (pacts[i] != NULL)
			free(pacts[i]);
	}

	FreeDiskObject(dobj);
	if (MUIMasterBase)
		CloseLibrary(MUIMasterBase);
}

/****************************************

	Function : InitAll

	Goal     : Allocate ALL needed ress

****************************************/
void InitAll(void)
{
	IconObj = (struct DiskObject *)GetDiskObject(ProgramName);
	if (IconObj == NULL)
	{
		Inf("Icon failed");
		exit(0);
	}

	InitRT();
	ReadToolsTypes();
	InitLocale();
	InitLists();
	ExtractSeg();
	if (LoadMem() == -1)
		Inf(LocaleMsg[CA_NO_CONFIG]);
	LoadGen();
	DataBase = (struct Library *)OpenLibrary("datatypes.library", 0);

	dobj = (struct DiskObject *)GetDiskObject(IconFile);
	if (dobj == NULL)
		dobj = IconObj;
	else
		FreeDiskObject(IconObj);

	Onglet[0] = LocaleMsg[CA_ACTIONS];
	Onglet[1] = LocaleMsg[CA_MANAGE];
	Onglet[2] = NULL;

	OpenDevice("console.device", -1, (struct IORequest *)&IOStdReq, 0);
	ConsoleDevice = (struct Library *)(&(IOStdReq.io_Device->dd_Library));
}

/****************************************

	Function : wbmain

	Goal     : task runned from WB

****************************************/
int wbmain(struct WBStartup *mess)
{
	strcpy(ProgramName, (char *)(mess->sm_ArgList[0].wa_Name));
	main(0, NULL);
}

/****************************************

	Function : MUInfo

	Goal     : Display a MUI requester

****************************************/
void MUInfo(char *st)
{
	MUI_Request(app, window, 0, LocaleMsg[CA_INFORMATION], LocaleMsg[CA_OKAY], st);
}

/****************************************

	Function : IsIcon

	Goal     : Determine if our appli
			   is iconifyed or not

	Return Values : TRUE / FALSE

****************************************/
int IsIcon(void)
{
	int state;
	get(app, MUIA_Application_Iconified, &state);
	return (state);
}

/****************************************

	Function : MuiReq

	Goal     : Display a MUI requester
			   with Yes / No buttons

	Return Values : Button pressed

****************************************/
long MuiReq(char *st)
{
	return (MUI_Request(app, window, 0, LocaleMsg[CA_SURE], LocaleMsg[CA_YES_NO], st));
}

/****************************************

	Function : GetL2Dir

	Goal     : Get the path from
			   the 2nd path string

****************************************/
void GetL2Dir(char *p)
{
	char *pp;
	int l;
	char slash[2];

	slash[0] = '/';
	slash[1] = 0;

	get(STR_PATH2, MUIA_String_Contents, &pp);

	strcpy(p, pp);
	l = strlen(p);

	if (p[l - 1] != ':' && p[l - 1] != '/')
		strcat(p, slash);
}