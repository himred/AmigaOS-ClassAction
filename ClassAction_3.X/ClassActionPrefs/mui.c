/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : mui.c

	Goal : main and MUI interface

	Ver  : 3.0

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
#include <utility/hooks.h>
#include <string.h>

#include "includes.h"
#include "ClassActionPrefs_rev.h"
#include "mui.h"
#include "listtree_mcc.h"

#include "listree.h"

struct Library *MUIMasterBase;

#define NCOM 13

char Ret[NCOM][8];

APTR app,window,
	LW_CLASSES,LW_ACTIONS,ST_NOMA,ST_NOMC,ST_FILTRE,ST_OFFSET,CY_OFFSET,
	BT_ADDC,BT_DELC,BT_LEARN,CY_MODE,ST_PILE,ST_DELAI,CH_RESCAN,ST_EXEC,
	BT_ADDA,BT_DELA,BT_SEL,BT_COM,BT_SAVE,BT_USE,BT_QUIT,WI_COM,LW_COM,
	BT_UP,BT_DOWN,BT_LNO,BT_LOK,ST_O[5],GA_LEARN,WI_LEARN,SL_LEARN,
	BT_LGO,WI_LGO,LW_POP,POP_NOM;

struct Class *SelectedClass=NULL;
struct Action *SelectedAction=NULL;
char Sleep=0;
char ByeBye=0;

char str[512];
void MUInfo(char *);
void RebuildPop();

ULONG __saveds __asm HookFuncAddc()
{
    int i, olds;
    struct Class *cla;

    sprintf(str, LocaleMsg[CAP_NEW_CLASS], MemList.size + 1);
    cla = (struct Class *)malloc(sizeof(struct Class));
    strcpy(cla->name, str);
    cla->nbaction = 0;
    cla->action = NULL;
    cla->actiontail = NULL;
    cla->next = NULL;
    strcpy(cla->suffix, "");

    for (i = 0; i < 5; i++)
        strcpy(cla->value[i], "");

    if (MemList.tail == NULL)
        MemList.head = MemList.tail = cla;
    else
    {
        MemList.tail->next = cla;
        MemList.tail = cla;
        (MemList.size)++;
    }

    olds = Sleep;
    Sleep = 1;
    DoMethod(LW_CLASSES, MUIM_List_InsertSingle, cla->name, MUIV_List_Insert_Bottom);
    set(LW_CLASSES, MUIA_List_Active, MemList.size - 1);
    SelectedClass = cla;
    RebuildPop();
    Sleep = olds;
}

ULONG __saveds __asm HookFuncDelc()
{

    struct Class *cla;
    struct Action *act, *tmp;
    int olds;

    if (SelectedClass == NULL)
        return (0);
    if (MuiReq(LocaleMsg[CAP_DEL_CLASS]) == 0)
        return (0);

    act = SelectedClass->action;
    while (act != NULL)
    {
        tmp = act->next;
        free(act);
        act = tmp;
    }

    cla = MemList.head;

    while (cla->next != SelectedClass)
        cla = cla->next;
    if (SelectedClass == MemList.tail)
    {
        cla->next = NULL;
        MemList.tail = cla;
    }
    else
        cla->next = SelectedClass->next;

    free(SelectedClass);
    (MemList.size)--;

    olds = Sleep;
    Sleep = 1;
    DoMethod(LW_CLASSES, MUIM_List_Remove, MUIV_List_Remove_Active);
    if (cla->next == NULL)
        cla = MemList.tail;
    else
        cla = cla->next;
    SelectedClass = cla;
    RebuildPop();
    Sleep = olds;
}

ULONG __saveds __asm HookFuncDela()
{

    struct Action *act, *next;
    int olds;

    if (SelectedAction == NULL)
        return (0);
    if (MuiReq(LocaleMsg[CAP_DEL_ACTION]) == 0)
        return (0);

    next = SelectedAction->next;

    act = SelectedClass->action;

    if (SelectedAction == SelectedClass->action)
    {
        SelectedClass->action = SelectedAction->next;
        if (SelectedClass->actiontail == SelectedAction)
            SelectedClass->actiontail = NULL;
    }
    else
    {
        while (act->next != SelectedAction)
            act = act->next;
        if (SelectedAction == SelectedClass->actiontail)
        {
            act->next = NULL;
            SelectedClass->actiontail = act;
        }
        else
            act->next = SelectedAction->next;
    }

    free(SelectedAction);
    SelectedClass->nbaction--;

    olds = Sleep;
    Sleep = 1;
    if (next == NULL)
        next = SelectedClass->actiontail;
    SelectedAction = next;
    DoMethod(LW_ACTIONS, MUIM_List_Remove, MUIV_List_Remove_Active);
    RebuildPop();
    Sleep = olds;
}

ULONG __saveds __asm HookFuncLnot()
{
    int x;

    set(WI_LEARN, MUIA_Window_Open, FALSE);
    for (x = 0; x < 5; x++)
        set(ST_O[x], MUIA_String_Contents, "");
}

ULONG __saveds __asm HookFuncLok()
{
    int i;
    char *a;
    int olds;

    if (MuiReq(LocaleMsg[CAP_L_DO_U]) == 0)
        return (0);

    for (i = 0; i < 5; i++)
    {
        get(ST_O[i], MUIA_String_Contents, &a);
        strcpy(SelectedClass->value[i], a);
    }

    strcpy(SelectedClass->suffix, "");

    olds = Sleep;
    Sleep = 1;

    set(ST_FILTRE, MUIA_String_Contents, "");
    set(ST_OFFSET, MUIA_String_Contents, SelectedClass->value[0]);
    set(CY_OFFSET, MUIA_Cycle_Active, 0);

    Sleep = olds;
    HookFuncLnot();
}

ULONG __saveds __asm HookFuncLgo()
{
    int x;
    long a;

    get(SL_LEARN, MUIA_Slider_Level, &a);
    set(WI_LGO, MUIA_Window_Open, FALSE);
    x = Learn(WI_LEARN, ST_O, GA_LEARN, a);

    if (x == 0)
        MUInfo(LocaleMsg[CAP_L_NOTHING]);
    if (x == -10)
        MUInfo(LocaleMsg[CAP_L_MEMORY]);
    if (x == -4)
        MUInfo(LocaleMsg[CAP_L_NEED]);
    if (x <= 0)
        HookFuncLnot();
}

ULONG __saveds __asm HookFuncLearn()
{

    if (Sleep == 1 || SelectedClass == NULL)
        return (0);

    set(WI_LGO, MUIA_Window_Open, TRUE);
}

ULONG __saveds __asm HookFuncAdda()
{
    struct Action *act;
    int olds;

    if (SelectedClass == NULL)
        return (0);

    act = (struct Action *)malloc(sizeof(struct Action));
    sprintf(str, LocaleMsg[CAP_NEW_ACTION], SelectedClass->nbaction + 1);

    strcpy(act->name, str);
    strcpy(act->exec, NOACT);
    strcpy(act->stack, "4096");
    strcpy(act->delay, "-1");
    act->wbrun = 0;
    act->next = NULL;

    if (SelectedClass->action == NULL)
    {
        SelectedClass->action = act;
        SelectedClass->actiontail = act;
    }
    else
    {
        SelectedClass->actiontail->next = act;
        SelectedClass->actiontail = act;
    }

    SelectedClass->nbaction++;

    olds = Sleep;
    Sleep = 1;
    DoMethod(LW_ACTIONS, MUIM_List_InsertSingle, act->name, MUIV_List_Insert_Bottom);
    set(LW_ACTIONS, MUIA_List_Active, SelectedClass->nbaction - 1);
    SelectedAction = act;
    RebuildPop();
    Sleep = olds;
}

ULONG __saveds __asm HookFuncSel()
{
    char file[200];
    char *x;
    long a;
    char tmp[300];

    if (SelectedAction == NULL || Getfile(file) != 1)
        return (0);

    get(ST_EXEC, MUIA_String_BufferPos, &a);
    get(ST_EXEC, MUIA_String_Contents, &x);
    strncpy(tmp, x, a);
    tmp[a] = 0;
    strcat(tmp, file);
    strcat(tmp, x + a);
    set(ST_EXEC, MUIA_String_Contents, tmp);
    strcpy(SelectedAction->exec, tmp);
}

ULONG __saveds __asm HookFuncCom()
{
    if (Sleep == 1 || SelectedAction == NULL)
        return (0);

    set(WI_COM, MUIA_Window_Open, TRUE);
}

ULONG __saveds __asm HookFuncSave()
{
    SaveMem(1);
    ByeBye = 1;
}

ULONG __saveds __asm HookFuncUse()
{
    SaveMem(0);
    ByeBye = 1;
}

ULONG __saveds __asm HookFuncQuit()
{
    ByeBye = 1;
}

ULONG __saveds __asm HookFuncLwc()
{
    long a;
    int i, olds;
    struct Class *cla;
    struct Action *act;
    int k;

    olds = Sleep;

    get(LW_CLASSES, MUIA_List_Active, &a);
    cla = MemList.head;
    for (i = 0; i < a; i++)
        cla = cla->next;
    SelectedClass = cla;
    SelectedAction = NULL;

    Sleep = 1;

    if (i < 3)
    {
        set(BT_DELC, MUIA_Disabled, TRUE);
        set(BT_LEARN, MUIA_Disabled, TRUE);
        set(ST_OFFSET, MUIA_Disabled, TRUE);
        set(ST_FILTRE, MUIA_Disabled, TRUE);
        set(CY_OFFSET, MUIA_Disabled, TRUE);
        set(ST_NOMC, MUIA_Disabled, TRUE);
        k = 2;
    }
    else
    {
        set(BT_DELC, MUIA_Disabled, FALSE);
        set(BT_LEARN, MUIA_Disabled, FALSE);
        set(ST_OFFSET, MUIA_Disabled, FALSE);
        set(ST_FILTRE, MUIA_Disabled, FALSE);
        set(CY_OFFSET, MUIA_Disabled, FALSE);
        set(ST_NOMC, MUIA_Disabled, FALSE);
        k = 0;
    }

    set(CY_OFFSET, MUIA_Cycle_Active, 0);
    set(ST_NOMC, MUIA_String_Contents, cla->name + k);
    set(ST_OFFSET, MUIA_String_Contents, cla->value[0]);
    set(ST_FILTRE, MUIA_String_Contents, cla->suffix);

    set(ST_NOMA, MUIA_String_Contents, "");
    set(ST_EXEC, MUIA_String_Contents, "");
    set(ST_DELAI, MUIA_String_Contents, "");
    set(ST_PILE, MUIA_String_Contents, "");

    act = SelectedClass->action;

    set(LW_ACTIONS, MUIA_List_Quiet, TRUE);
    DoMethod(LW_ACTIONS, MUIM_List_Clear, );

    i = 0;
    while (act != NULL)
    {
        DoMethod(LW_ACTIONS, MUIM_List_InsertSingle, act->name, MUIV_List_Insert_Bottom);
        act = act->next;
        i++;
    }

    set(LW_ACTIONS, MUIA_List_Quiet, FALSE);

    Sleep = olds;
    if (i != 0)
    {
        SelectedAction = act;
        set(LW_ACTIONS, MUIA_List_Active, 0);
    }
    else
        set(CY_MODE, MUIA_Cycle_Active, 0);
}

ULONG __saveds __asm HookFuncLwp()
{
    int CopyClass(struct Action * act);
    struct MUIS_Listtree_TreeNode *tree;
    struct Action *act;

    if (SelectedAction == NULL)
        return (0);

    get(LW_POP, MUIA_Listtree_Active, &tree);

    DoMethod(POP_NOM, MUIM_Popstring_Close, );
    act = (struct Action *)tree->tn_User;

    CopyClass(act);
}

ULONG __saveds __asm HookFuncLwa()
{
    long a;
    int i, olds, st;
    struct Action *act;

    olds = Sleep;

    get(LW_ACTIONS, MUIA_List_Active, &a);
    if (a == -1)
    {
        Sleep = 1;
        set(CY_MODE, MUIA_Cycle_Active, 0);
        set(ST_NOMA, MUIA_String_Contents, "");
        set(ST_EXEC, MUIA_String_Contents, "");
        set(ST_DELAI, MUIA_String_Contents, "");
        set(ST_PILE, MUIA_String_Contents, "");
        Sleep = olds;
        return (0);
    }

    act = SelectedClass->action;
    for (i = 0; i < a; i++)
        act = act->next;
    SelectedAction = act;

    Sleep = 1;

    i = act->wbrun;
    st = FALSE;

    if (i >= 10)
    {
        i -= 10;
        st = TRUE;
    }

    set(CH_RESCAN, MUIA_Selected, st);
    set(CY_MODE, MUIA_Cycle_Active, i);
    set(ST_NOMA, MUIA_String_Contents, act->name);
    set(ST_EXEC, MUIA_String_Contents, act->exec);
    set(ST_DELAI, MUIA_String_Contents, act->delay);
    set(ST_PILE, MUIA_String_Contents, act->stack);

    if (i == 0)
    {
        set(ST_PILE, MUIA_Disabled, FALSE);
        set(ST_DELAI, MUIA_Disabled, FALSE);
    }
    else
    {
        set(ST_PILE, MUIA_Disabled, TRUE);
        set(ST_DELAI, MUIA_Disabled, TRUE);

        set(ST_DELAI, MUIA_String_Contents, "");
        set(ST_PILE, MUIA_String_Contents, "");
    }

    Sleep = olds;
}

ULONG __saveds __asm HookFuncStNoma()
{

    char *x;
    if (Sleep == 1 || SelectedAction == NULL)
        return (0);

    get(ST_NOMA, MUIA_String_Contents, &x);
    strcpy(SelectedAction->name, x);
    DoMethod(LW_ACTIONS, MUIM_List_Redraw, MUIV_List_Redraw_Active);
    RebuildPop();
}

ULONG __saveds __asm HookFuncStNomc()
{

    char *x;

    if (Sleep == 1 || SelectedClass == NULL)
        return (0);

    get(ST_NOMC, MUIA_String_Contents, &x);
    strcpy(SelectedClass->name, x);
    DoMethod(LW_CLASSES, MUIM_List_Redraw, MUIV_List_Redraw_Active);
    RebuildPop();
}

ULONG __saveds __asm HookFuncStFiltre()
{
    char *x;

    if (Sleep == 1 || SelectedClass == NULL)
        return (0);

    get(ST_FILTRE, MUIA_String_Contents, &x);
    strcpy(SelectedClass->suffix, x);
}

ULONG __saveds __asm HookFuncStOffset()
{
    char *x;
    long a;

    if (Sleep == 1 || SelectedClass == NULL)
        return (0);

    get(CY_OFFSET, MUIA_Cycle_Active, &a);
    get(ST_OFFSET, MUIA_String_Contents, &x);
    strcpy(SelectedClass->value[a], x);
}

ULONG __saveds __asm HookFuncStPile()
{
    char *x;

    if (Sleep == 1 || SelectedAction == NULL)
        return (0);

    get(ST_PILE, MUIA_String_Contents, &x);
    strcpy(SelectedAction->stack, x);
}

ULONG __saveds __asm HookFuncStDelai()
{
    char *x;

    if (Sleep == 1 || SelectedAction == NULL)
        return (0);

    get(ST_DELAI, MUIA_String_Contents, &x);
    strcpy(SelectedAction->delay, x);
}

ULONG __saveds __asm HookFuncStExec()
{
    char *x;

    if (Sleep == 1 || SelectedAction == NULL)
        return (0);

    get(ST_EXEC, MUIA_String_Contents, &x);
    strcpy(SelectedAction->exec, x);
}

ULONG __saveds __asm HookFuncCyMode()
{
    long a;
    long st;
    int olds;
    struct Action *act;

    if (SelectedAction == NULL)
        return (0);

    act = SelectedAction;

    get(CY_MODE, MUIA_Cycle_Active, &a);
    if (a != 0)
    {
        set(ST_PILE, MUIA_Disabled, TRUE);
        set(ST_DELAI, MUIA_Disabled, TRUE);
    }
    else
    {
        set(ST_PILE, MUIA_Disabled, FALSE);
        set(ST_DELAI, MUIA_Disabled, FALSE);
        olds = Sleep;
        Sleep = 1;
        set(ST_EXEC, MUIA_String_Contents, act->exec);
        set(ST_DELAI, MUIA_String_Contents, act->delay);
        set(ST_PILE, MUIA_String_Contents, act->stack);
        Sleep = olds;
    }

    if (Sleep == 1)
        return (0);
    get(CH_RESCAN, MUIA_Selected, &st);
    SelectedAction->wbrun = (char)(a + 10 * st);
}

ULONG __saveds __asm HookFuncCyOffset()
{
    if (Sleep == 1 || SelectedClass == NULL)
        return (0);

    long a;
    int olds;
    olds = Sleep;

    get(CY_OFFSET, MUIA_Cycle_Active, &a);
    Sleep = 1;
    set(ST_OFFSET, MUIA_String_Contents, SelectedClass->value[a]);
    Sleep = olds;
}

ULONG __saveds __asm HookFuncChRescan()
{
    long a;
    if (Sleep == 1 || SelectedAction == NULL)
        return (0);

    get(CH_RESCAN, MUIA_Selected, &a);
    if (a == TRUE && SelectedAction->wbrun < 10)
        SelectedAction->wbrun += 10;
    if (a == FALSE && SelectedAction->wbrun >= 10)
        SelectedAction->wbrun -= 10;
}

ULONG __saveds __asm HookFuncWiComClose()
{
    set(WI_COM, MUIA_Window_Open, FALSE);
}

ULONG __saveds __asm HookFuncUp()
{

    struct Action *tmp, *prev;

    if (SelectedAction == NULL)
        return (0);
    if (SelectedAction == SelectedClass->action)
        return (0);

    tmp = SelectedClass->action;
    while (tmp->next != SelectedAction)
        tmp = tmp->next;

    prev = SelectedClass->action;
    if (prev != tmp)
        while (prev->next != tmp)
            prev = prev->next;

    if (prev == tmp)
    {
        SelectedClass->action = SelectedAction;
        prev->next = SelectedAction->next;
        SelectedAction->next = prev;
    }
    else
    {
        prev->next = SelectedAction;
        tmp->next = SelectedAction->next;
        SelectedAction->next = tmp;
    }

    tmp = SelectedClass->action;
    while (tmp != NULL)
    {
        if (tmp->next == NULL)
            SelectedClass->actiontail = tmp;
        tmp = tmp->next;
    }

    DoMethod(LW_ACTIONS, MUIM_List_Exchange, MUIV_List_Exchange_Active, MUIV_List_Exchange_Previous);

    set(LW_ACTIONS, MUIA_List_Active, MUIV_List_Active_Up);
}

ULONG __saveds __asm HookFuncDown()
{

    struct Action *tmp, *prev;

    if (SelectedAction == NULL)
        return (0);
    if (SelectedAction == SelectedClass->actiontail)
        return (0);

    tmp = SelectedClass->action;
    if (tmp != SelectedAction)
        while (tmp->next != SelectedAction)
            tmp = tmp->next;

    prev = SelectedAction->next;

    if (tmp == SelectedAction)
    {
        prev = SelectedAction->next;
        SelectedClass->action = prev;
        tmp->next = prev->next;
        prev->next = tmp;
    }
    else
    {
        tmp->next = prev;
        SelectedAction->next = prev->next;
        prev->next = SelectedAction;
    }

    tmp = SelectedClass->action;
    while (tmp != NULL)
    {
        if (tmp->next == NULL)
            SelectedClass->actiontail = tmp;
        tmp = tmp->next;
    }

    DoMethod(LW_ACTIONS, MUIM_List_Exchange, MUIV_List_Exchange_Active, MUIV_List_Exchange_Next);

    set(LW_ACTIONS, MUIA_List_Active, MUIV_List_Active_Down);
}

ULONG __saveds __asm HookFuncLwCom()
{
    long a, r;
    char tmp[300];
    char *x;

    if (SelectedAction == NULL)
    {
        MUInfo("Oooops c'est pas Normal");
        return (0L);
    }

    get(LW_COM, MUIA_List_Active, &r);
    get(ST_EXEC, MUIA_String_BufferPos, &a);
    get(ST_EXEC, MUIA_String_Contents, &x);

    strncpy(tmp, x, a);
    tmp[a] = 0;
    strcat(tmp, Ret[r]);
    strcat(tmp, x + a);

    strcpy(SelectedAction->exec, tmp);
    set(ST_EXEC, MUIA_String_Contents, SelectedAction->exec);

    set(WI_COM, MUIA_Window_Open, FALSE);
}

int main(int argc, char **argv)
{

    long MuiReq(char *);

    char *Comms[NCOM];

    int i, n;
    struct Class *cla;

    MUIMasterBase = (struct Library *)OpenLibrary("muimaster.library", 11);
    if (MUIMasterBase == NULL)
    {
        Inf("Unable to Open MUIMaster library V11+");
        return (0);
    }

    InitAll();

    Comms[0] = LocaleMsg[CAP_F_COMM];
    Comms[1] = LocaleMsg[CAP_S_COMM];
    Comms[2] = LocaleMsg[CAP_B_COMM];
    Comms[3] = LocaleMsg[CAP_X_COMM];
    Comms[4] = LocaleMsg[CAP_BF_COMM];
    Comms[5] = LocaleMsg[CAP_BS_COMM];
    Comms[6] = LocaleMsg[CAP_BB_COMM];
    Comms[7] = LocaleMsg[CAP_BX_COMM];
    Comms[8] = LocaleMsg[CAP_REQF_COMM];
    Comms[9] = LocaleMsg[CAP_REQD_COMM];
    Comms[10] = LocaleMsg[CAP_REQV_COMM];
    Comms[11] = LocaleMsg[CAP_REQT_COMM];
    Comms[12] = LocaleMsg[CAP_SURE_COMM];

    strcpy(Ret[0], "[f]");
    strcpy(Ret[1], "[s]");
    strcpy(Ret[2], "[b]");
    strcpy(Ret[3], "[x]");
    strcpy(Ret[4], "[F]");
    strcpy(Ret[5], "[S]");
    strcpy(Ret[6], "[B]");
    strcpy(Ret[7], "[X]");
    strcpy(Ret[8], "REQF[");
    strcpy(Ret[9], "REQD[");
    strcpy(Ret[10], "REQV[");
    strcpy(Ret[11], "REQT[");
    strcpy(Ret[12], "SURE[");

    struct Hook HookUp = {{NULL, NULL}, (void *)HookFuncUp, NULL, NULL};
    struct Hook HookDown = {{NULL, NULL}, (void *)HookFuncDown, NULL, NULL};
    struct Hook HookAddc = {{NULL, NULL}, (void *)HookFuncAddc, NULL, NULL};
    struct Hook HookDelc = {{NULL, NULL}, (void *)HookFuncDelc, NULL, NULL};
    struct Hook HookLearn = {{NULL, NULL}, (void *)HookFuncLearn, NULL, NULL};
    struct Hook HookAdda = {{NULL, NULL}, (void *)HookFuncAdda, NULL, NULL};
    struct Hook HookDela = {{NULL, NULL}, (void *)HookFuncDela, NULL, NULL};
    struct Hook HookSel = {{NULL, NULL}, (void *)HookFuncSel, NULL, NULL};
    struct Hook HookCom = {{NULL, NULL}, (void *)HookFuncCom, NULL, NULL};
    struct Hook HookSave = {{NULL, NULL}, (void *)HookFuncSave, NULL, NULL};
    struct Hook HookUse = {{NULL, NULL}, (void *)HookFuncUse, NULL, NULL};
    struct Hook HookQuit = {{NULL, NULL}, (void *)HookFuncQuit, NULL, NULL};
    struct Hook HookLwc = {{NULL, NULL}, (void *)HookFuncLwc, NULL, NULL};
    struct Hook HookLwa = {{NULL, NULL}, (void *)HookFuncLwa, NULL, NULL};
    struct Hook HookLwp = {{NULL, NULL}, (void *)HookFuncLwp, NULL, NULL};
    struct Hook HookStNoma = {{NULL, NULL}, (void *)HookFuncStNoma, NULL, NULL};
    struct Hook HookStNomc = {{NULL, NULL}, (void *)HookFuncStNomc, NULL, NULL};
    struct Hook HookStFiltre = {{NULL, NULL}, (void *)HookFuncStFiltre, NULL, NULL};
    struct Hook HookStOffset = {{NULL, NULL}, (void *)HookFuncStOffset, NULL, NULL};
    struct Hook HookStPile = {{NULL, NULL}, (void *)HookFuncStPile, NULL, NULL};
    struct Hook HookStDelai = {{NULL, NULL}, (void *)HookFuncStDelai, NULL, NULL};
    struct Hook HookStExec = {{NULL, NULL}, (void *)HookFuncStExec, NULL, NULL};
    struct Hook HookCyMode = {{NULL, NULL}, (void *)HookFuncCyMode, NULL, NULL};
    struct Hook HookCyOffset = {{NULL, NULL}, (void *)HookFuncCyOffset, NULL, NULL};
    struct Hook HookChRescan = {{NULL, NULL}, (void *)HookFuncChRescan, NULL, NULL};
    struct Hook HookWiComClose = {{NULL, NULL}, (void *)HookFuncWiComClose, NULL, NULL};
    struct Hook HookLwCom = {{NULL, NULL}, (void *)HookFuncLwCom, NULL, NULL};
    struct Hook HookLok = {{NULL, NULL}, (void *)HookFuncLok, NULL, NULL};
    struct Hook HookLnot = {{NULL, NULL}, (void *)HookFuncLnot, NULL, NULL};
    struct Hook HookLgo = {{NULL, NULL}, (void *)HookFuncLgo, NULL, NULL};

    cl_ddlisttree = MUI_CreateCustomClass(NULL, MUIC_Listtree, NULL, sizeof(struct Data_DDListtree), Dispatcher_DDListtree);
    if (cl_ddlisttree == NULL)
        return (0);


    app =
        ApplicationObject,
            MUIA_Application_Author,         "Gasmi Salim",
            MUIA_Application_Base,           "ClassActionPrefs",
            MUIA_Application_Copyright,      "(c) 1994-1996 Gasmi Salim",
            MUIA_Application_Description,    "Preferences for ClassAction",
            MUIA_Application_Title,          VERS,
            MUIA_Application_Version,        VERSTAG,
            MUIA_Application_SingleTask,     TRUE,
            MUIA_Application_UseCommodities, FALSE,
            MUIA_Application_UseRexx,        FALSE,

	    SubWindow, WI_LGO = WindowObject,
                MUIA_Window_Title,     LocaleMsg[CAP_L_ACCU],
                MUIA_Window_ID,        MAKE_ID('W','I','N','4'),
                WindowContents, VGroup,
			 Child, HGroup,
                             Child, TextObject,
                                 MUIA_Text_Contents,LocaleMsg[CAP_L_ACCU] ,
                             End,
			 End,
			 Child, HGroup,
			      Child, SL_LEARN = SliderObject,
				     MUIA_Slider_Min,75,
				     MUIA_Slider_Max,100,
				     MUIA_Slider_Horiz,TRUE,
				     MUIA_Slider_Level,100,
				     MUIA_Frame,MUIV_Frame_Slider,
			      End,
			 End,
			 Child, HGroup,
                                Child, BT_LGO = TextObject,
                                    ButtonFrame,
                                    MUIA_InputMode, MUIV_InputMode_RelVerify,
                                    MUIA_Background, MUII_ButtonBack,
                                    MUIA_Text_Contents, LocaleMsg[CAP_L_ACCEPT],
                                    MUIA_Text_PreParse, "\33c",
                                End,
			End,
			End,
			End,

	    SubWindow, WI_LEARN = WindowObject,
		MUIA_Window_Title,     LocaleMsg[CAP_L_LEARN],
                MUIA_Window_ID,        MAKE_ID('W','I','N','3'),
		WindowContents, VGroup,
			 Child, HGroup,
			     Child, GA_LEARN = GaugeObject,
				      MUIA_Gauge_Current,10,
				      MUIA_Gauge_Horiz,TRUE,
				      MUIA_Gauge_Max,25,
				      MUIA_Gauge_InfoText,"Gauge",
				      MUIA_Frame,MUIV_Frame_Gauge,
			     End,
			 End,
			 Child, HGroup,
                             Child, TextObject, MUIA_Weight, 25,
                                        MUIA_Text_Contents,"Offset #1" ,
                             End,
                             Child, ST_O[0] = StringObject,
                                        MUIA_String_Format, MUIV_String_Format_Left,
                                        MUIA_String_MaxLen,50,
                                        MUIA_CycleChain, TRUE,
                                        StringFrame,
                             End,
			 End,
			 Child, HGroup,
                             Child, TextObject, MUIA_Weight, 25,
                                        MUIA_Text_Contents,"Offset #2" ,
                             End,
                             Child, ST_O[1] = StringObject,
                                        MUIA_String_Format, MUIV_String_Format_Left,
                                        MUIA_String_MaxLen,50,
                                        MUIA_CycleChain, TRUE,
                                        StringFrame,
                             End,
			 End,
			 Child, HGroup,
                             Child, TextObject, MUIA_Weight, 25,
                                        MUIA_Text_Contents,"Offset #3" ,
                             End,
                             Child, ST_O[2] = StringObject,
                                        MUIA_String_Format, MUIV_String_Format_Left,
                                        MUIA_String_MaxLen,50,
                                        MUIA_CycleChain, TRUE,
                                        StringFrame,
                             End,
			 End,
			 Child, HGroup,
                             Child, TextObject, MUIA_Weight, 25,
                                        MUIA_Text_Contents,"Offset #4" ,
                             End,
                             Child, ST_O[3] = StringObject,
                                        MUIA_String_Format, MUIV_String_Format_Left,
                                        MUIA_String_MaxLen,50,
                                        MUIA_CycleChain, TRUE,
                                        StringFrame,
                             End,
			 End,
			 Child, HGroup,
                             Child, TextObject, MUIA_Weight, 25,
                                        MUIA_Text_Contents,"Offset #5" ,
                             End,
                             Child, ST_O[4] = StringObject,
                                        MUIA_String_Format, MUIV_String_Format_Left,
                                        MUIA_String_MaxLen,50,
                                        MUIA_CycleChain, TRUE,
                                        StringFrame,
                             End,
			 End,

			 Child, HGroup,

                                Child, BT_LOK = TextObject,
                                    ButtonFrame,
                                    MUIA_InputMode, MUIV_InputMode_RelVerify,
                                    MUIA_Background, MUII_ButtonBack,
                                    MUIA_Text_Contents, LocaleMsg[CAP_L_ACCEPT],
                                    MUIA_Text_PreParse, "\33c",
                                End,

                                Child, BT_LNO = TextObject,
                                    ButtonFrame,
                                    MUIA_InputMode, MUIV_InputMode_RelVerify,
                                    MUIA_Background, MUII_ButtonBack,
                                    MUIA_Text_Contents, LocaleMsg[CAP_CANCEL],
                                    MUIA_Text_PreParse, "\33c",
                                End,
			 End,
			End,
			End,

            SubWindow, WI_COM = WindowObject,
                MUIA_Window_Title,     LocaleMsg[CAP_COMMANDS],
                MUIA_Window_ID,        MAKE_ID('W','I','N','2'),

                WindowContents, HGroup,
                Child, LW_COM = ListviewObject,
                    MUIA_Listview_List, ListObject,
                       InputListFrame,
                    End,
                 End,
             End,
	     End,


            SubWindow, window = WindowObject,
                MUIA_Window_Title,     VERS,
                MUIA_Window_ID,        MAKE_ID('W','I','N','1'),

                WindowContents, VGroup,
                    Child, HGroup, MUIA_Group_SameSize, TRUE,
                        Child, VGroup, GroupFrameT("Classes"),

                            Child, LW_CLASSES = ListviewObject,
                                MUIA_Listview_List, ListObject,
                                    InputListFrame,
                                End,
                            End,

                            Child, TextObject,
                            End,

                            Child, HGroup,
                                Child, VGroup, MUIA_Weight, 25, MUIA_Group_SameSize, TRUE,
                                    Child, TextObject,
                                        MUIA_Text_Contents, LocaleMsg[CAP_CLASSNAME],
                                    End,

                                    Child, TextObject,
                                        MUIA_Text_Contents, LocaleMsg[CAP_MATCH_NAME],
                                    End,

                                    Child, CY_OFFSET=KeyCycle(Offset_Cycle0Labels,'o'),
                                End,

                                Child, VGroup, MUIA_Group_SameSize, TRUE,
                                    Child, ST_NOMC = StringObject,
					MUIA_String_MaxLen,20,
				        MUIA_String_Format, MUIV_String_Format_Left,
                                        MUIA_CycleChain, TRUE,
                                        StringFrame,
                                    End,

                                    Child, ST_FILTRE = StringObject,
				        MUIA_String_Format, MUIV_String_Format_Left,
					MUIA_String_MaxLen,100,
                                        MUIA_CycleChain, TRUE,
                                        StringFrame,
                                    End,

                                    Child, ST_OFFSET = StringObject,
				        MUIA_String_Format, MUIV_String_Format_Left,
					MUIA_String_MaxLen,50,
                                        MUIA_CycleChain, TRUE,
                                        StringFrame,
                                    End,

                                End,
                            End,

                            Child, HGroup, MUIA_Group_SameSize, TRUE,
                                Child, BT_ADDC = TextObject,
                                    ButtonFrame,
                                    MUIA_InputMode, MUIV_InputMode_RelVerify,
                                    MUIA_Background, MUII_ButtonBack,
                                    MUIA_Text_Contents, LocaleMsg[CAP_ADD],
                                    MUIA_Text_PreParse, "\33c",
                                    MUIA_ShortHelp, LocaleMsg[CAP_H_ADDC],
                                End,

                                Child, BT_DELC = TextObject,
                                    ButtonFrame,
                                    MUIA_InputMode, MUIV_InputMode_RelVerify,
                                    MUIA_Background, MUII_ButtonBack,
                                    MUIA_Text_Contents, LocaleMsg[CAP_DEL],
                                    MUIA_Text_PreParse, "\33c",
                                    MUIA_ShortHelp, LocaleMsg[CAP_H_DELC],
                                End,

                                Child, BT_LEARN = TextObject,
                                    ButtonFrame,
                                    MUIA_InputMode, MUIV_InputMode_RelVerify,
                                    MUIA_Background, MUII_ButtonBack,
                                    MUIA_Text_Contents, LocaleMsg[CAP_L_LEARN],
                                    MUIA_Text_PreParse, "\33c",
                                    MUIA_ShortHelp, LocaleMsg[CAP_H_LEARN],
                                End,


                            End,
                        End,

			/*Child, BalanceObject,End,*/
                        Child, VGroup, GroupFrameT(LocaleMsg[CAP_ACTIONS]),
                            Child, LW_ACTIONS = ListviewObject,
                                                MUIA_Listview_List, ListObject,
                                                        InputListFrame,
                                    End,
                            End,

                            Child, VGroup,

                                Child, HGroup,
                                    Child, VGroup,
                                        Child, HGroup,
					   Child,VGroup,
						Child, TextObject,
                                                    MUIA_Text_PreParse, "\33c",
                                                End,
					   Child, HGroup,
                                                Child,
                                                 BT_UP = ImageObject,
                                                 ImageButtonFrame,
                                                 MUIA_InputMode,
                                                        MUIV_InputMode_RelVerify,
                                                 MUIA_Image_Spec, MUII_ArrowUp,
                                                 MUIA_Image_FreeVert, TRUE,
                                                 MUIA_Selected,FALSE,
                                                 MUIA_Background,MUII_ButtonBack,
                                                 MUIA_ShowSelState,TRUE,
                                                 End,
                                                Child,
                                                 BT_DOWN = ImageObject,
                                                 ImageButtonFrame,
                                                 MUIA_InputMode,
                                                        MUIV_InputMode_RelVerify,
                                                 MUIA_Image_Spec, MUII_ArrowDown,
                                                 MUIA_Image_FreeVert, TRUE,
                                                 MUIA_Selected,FALSE,
                                                 MUIA_Background,MUII_ButtonBack,
                                                 MUIA_ShowSelState,TRUE,
                                                 End,
                                                End,
					    End,


                                            Child, VGroup,
                                                Child, TextObject,
                                                    MUIA_Text_Contents, "Mode",
                                                    MUIA_Text_PreParse, "\33c",

                                                End,
                                                Child, CY_MODE = KeyCycle(WB,'m'),
                                            End,

                                            Child, VGroup,
                                                Child, TextObject,
                                                    MUIA_Text_Contents, LocaleMsg[CAP_STACK],
                                                    MUIA_Text_PreParse, "\33c",
                                                End,
                                                Child, ST_PILE = StringObject,
						    MUIA_String_Format,
							MUIV_String_Format_Left,
						    MUIA_String_MaxLen,7,
						    MUIA_String_Accept,
							"0123456789",
                                                    MUIA_CycleChain, TRUE,
                                                    StringFrame,
                                                End,
                                            End,

                                            Child, VGroup,
                                                Child, TextObject,
                                                    MUIA_Text_Contents,LocaleMsg[CAP_DELAY] ,
                                                    MUIA_Text_PreParse, "\33c",
                                                End,
                                                Child, ST_DELAI = StringObject,
						    MUIA_String_Format,
                                                        MUIV_String_Format_Left,
						    MUIA_String_Accept,
							"0123456789-",
						    MUIA_String_MaxLen,3,
                                                    MUIA_CycleChain, TRUE,
                                                    StringFrame,
                                                End,
                                            End,

                                            Child, VGroup,
                                                Child, TextObject,
                                                    MUIA_Text_Contents, "RescanDir",
                                                    MUIA_Text_PreParse, "\33c",
                                                End,
                                                Child, CH_RESCAN = CheckMark(FALSE),
                                            End,

                                        End,
                                    End,
                                End,

                                Child, HGroup,
                                    Child, TextObject, MUIA_Weight, 25,
                                        MUIA_Text_Contents,LocaleMsg[CAP_NAME] ,
                                    End,
				    Child, POP_NOM = PopobjectObject,
                                     MUIA_Popstring_Button, PopButton(MUII_PopUp),
                                     MUIA_Popstring_String, ST_NOMA = StringObject,
					MUIA_Popstring_Toggle,TRUE,
					MUIA_String_Format, MUIV_String_Format_Left,
                                        MUIA_String_MaxLen,20,
                                        MUIA_CycleChain, TRUE,
                                        StringFrame,
                                     End,
                                MUIA_Popobject_Object, ListviewObject,
				MUIA_Listview_List, LW_POP =
					NewObject(cl_ddlisttree->mcc_Class, NULL,
                            InputListFrame,
                            MUIA_Listtree_Format, ",,",
                                   End,
                               End,
				End,
				End,

                                Child, HGroup,
                                    Child, TextObject, MUIA_Weight, 25,
                                        MUIA_Text_Contents, LocaleMsg[CAP_EXEC],
                                    End,
                                    Child, ST_EXEC = StringObject,
				        MUIA_String_Format, MUIV_String_Format_Left,
					MUIA_String_MaxLen,241,
                                        MUIA_CycleChain, TRUE,
                                        StringFrame,
                                    End,
                                End,
                            End,

                            Child, HGroup, MUIA_Group_SameSize, TRUE,
                                Child, BT_ADDA = TextObject,
                                    ButtonFrame,
                                    MUIA_InputMode, MUIV_InputMode_RelVerify,
                                    MUIA_Background, MUII_ButtonBack,
                                    MUIA_Text_Contents, LocaleMsg[CAP_ADD],
                                    MUIA_Text_PreParse, "\33c",
                                    MUIA_ShortHelp, LocaleMsg[CAP_H_ADDA],
                                End,

                                Child, BT_DELA = TextObject,
                                    ButtonFrame,
                                    MUIA_InputMode, MUIV_InputMode_RelVerify,
                                    MUIA_Background, MUII_ButtonBack,
                                    MUIA_Text_Contents, LocaleMsg[CAP_DEL],
                                    MUIA_Text_PreParse, "\33c",
                                    MUIA_ShortHelp, LocaleMsg[CAP_H_DELA],
                                End,

                                Child, BT_SEL = TextObject,
                                    ButtonFrame,
                                    MUIA_InputMode, MUIV_InputMode_RelVerify,
                                    MUIA_Background, MUII_ButtonBack,
                                    MUIA_Text_Contents, LocaleMsg[CAP_LOAD],
                                    MUIA_Text_PreParse, "\33c",
                                    MUIA_ShortHelp, LocaleMsg[CAP_H_FILE],
                                End,

                                Child, BT_COM = TextObject,
                                    ButtonFrame,
                                    MUIA_InputMode, MUIV_InputMode_RelVerify,
                                    MUIA_Background, MUII_ButtonBack,
                                    MUIA_Text_Contents, LocaleMsg[CAP_COMM],
                                    MUIA_Text_PreParse, "\33c",
                                    MUIA_ShortHelp, LocaleMsg[CAP_H_COMM],
                                End,

                            End,
                        End,
                    End,

                    Child, HGroup, MUIA_Group_SameSize, TRUE,
                        Child, BT_SAVE = TextObject,
                            ButtonFrame,
                            MUIA_InputMode, MUIV_InputMode_RelVerify,
                            MUIA_Background, MUII_ButtonBack,
                            MUIA_Text_Contents, LocaleMsg[CAP_SAVE],
                            MUIA_Text_PreParse, "\33c",
                            MUIA_ShortHelp, LocaleMsg[CAP_H_SAVE],
                        End,

                        Child, BT_USE = TextObject,
                            ButtonFrame,
                            MUIA_InputMode, MUIV_InputMode_RelVerify,
                            MUIA_Background, MUII_ButtonBack,
                            MUIA_Text_Contents, LocaleMsg[CAP_USE],
                            MUIA_Text_PreParse, "\33c",
                            MUIA_ShortHelp, LocaleMsg[CAP_H_USE],
                        End,

                        Child, BT_QUIT = TextObject,
                            ButtonFrame,
                            MUIA_InputMode, MUIV_InputMode_RelVerify,
                            MUIA_Background, MUII_ButtonBack,
                            MUIA_Text_Contents, LocaleMsg[CAP_CANCEL],
                            MUIA_Text_PreParse, "\33c",
                            MUIA_ShortHelp, LocaleMsg[CAP_H_QUIT],
                        End,

                    End,

                End,
            End,
        End;

/* METHODS */

    DoMethod(window,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,app,2,MUIM_Application_ReturnID,MUIV_Application_ReturnID_Quit);
    DoMethod(WI_LEARN,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,WI_LEARN,2,MUIM_CallHook,&HookLnot);
    DoMethod(WI_COM,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,WI_COM,2,MUIM_CallHook,&HookWiComClose);
    DoMethod(BT_LGO,MUIM_Notify,MUIA_Pressed,FALSE,BT_LGO,2,MUIM_CallHook,&HookLgo);
    DoMethod(BT_LOK,MUIM_Notify,MUIA_Pressed,FALSE,BT_LOK,2,MUIM_CallHook,&HookLok);
    DoMethod(BT_LNO,MUIM_Notify,MUIA_Pressed,FALSE,BT_LNO,2,MUIM_CallHook,&HookLnot);
    DoMethod(BT_ADDC,MUIM_Notify,MUIA_Pressed,FALSE,BT_ADDC,2,MUIM_CallHook,&HookAddc);
    DoMethod(BT_DELC,MUIM_Notify,MUIA_Pressed,FALSE,BT_DELC,2,MUIM_CallHook,&HookDelc);
    DoMethod(BT_LEARN,MUIM_Notify,MUIA_Pressed,FALSE,BT_LEARN,2,MUIM_CallHook,&HookLearn);
    DoMethod(BT_ADDA,MUIM_Notify,MUIA_Pressed,FALSE,BT_ADDA,2,MUIM_CallHook,&HookAdda);
    DoMethod(BT_DELA,MUIM_Notify,MUIA_Pressed,FALSE,BT_DELA,2,MUIM_CallHook,&HookDela);
    DoMethod(BT_SEL,MUIM_Notify,MUIA_Pressed,FALSE,BT_SEL,2,MUIM_CallHook,&HookSel);
    DoMethod(BT_COM,MUIM_Notify,MUIA_Pressed,FALSE,BT_COM,2,MUIM_CallHook,&HookCom);
    DoMethod(BT_SAVE,MUIM_Notify,MUIA_Pressed,FALSE,BT_SAVE,2,MUIM_CallHook,&HookSave);
    DoMethod(BT_USE,MUIM_Notify,MUIA_Pressed,FALSE,BT_USE,2,MUIM_CallHook,&HookUse);
    DoMethod(BT_QUIT,MUIM_Notify,MUIA_Pressed,FALSE,BT_QUIT,2,MUIM_CallHook,&HookQuit);
    DoMethod(LW_POP,MUIM_Notify,MUIA_Listtree_DoubleClick,MUIV_EveryTime,LW_POP,2,MUIM_CallHook,&HookLwp);
    DoMethod(LW_CLASSES,MUIM_Notify,MUIA_List_Active,MUIV_EveryTime,LW_CLASSES,2,MUIM_CallHook,&HookLwc);
    DoMethod(LW_ACTIONS,MUIM_Notify,MUIA_List_Active,MUIV_EveryTime,LW_ACTIONS,2,MUIM_CallHook,&HookLwa);
    DoMethod(LW_COM,MUIM_Notify,MUIA_Listview_DoubleClick,TRUE,LW_COM,2,MUIM_CallHook,&HookLwCom);
    DoMethod(ST_NOMA,MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,ST_NOMA,2,MUIM_CallHook,&HookStNoma);
    DoMethod(ST_NOMC,MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,ST_NOMC,2,MUIM_CallHook,&HookStNomc);
    DoMethod(ST_FILTRE,MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,ST_FILTRE,2,MUIM_CallHook,&HookStFiltre);
    DoMethod(ST_OFFSET,MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,ST_OFFSET,2,MUIM_CallHook,&HookStOffset);
    DoMethod(ST_PILE,MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,ST_PILE,2,MUIM_CallHook,&HookStPile);
    DoMethod(ST_DELAI,MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,ST_DELAI,2,MUIM_CallHook,&HookStDelai);
    DoMethod(ST_EXEC,MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,ST_EXEC,2,MUIM_CallHook,&HookStExec);
    DoMethod(CY_MODE,MUIM_Notify,MUIA_Cycle_Active,MUIV_EveryTime,CY_MODE,2,MUIM_CallHook,&HookCyMode);
    DoMethod(CY_OFFSET,MUIM_Notify,MUIA_Cycle_Active,MUIV_EveryTime,CY_OFFSET,2,MUIM_CallHook,&HookCyOffset);
    DoMethod(CH_RESCAN,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,CH_RESCAN,2,MUIM_CallHook,&HookChRescan);
    DoMethod(BT_UP,MUIM_Notify,MUIA_Pressed,FALSE,BT_UP,2,MUIM_CallHook,&HookUp);
    DoMethod(BT_DOWN,MUIM_Notify,MUIA_Pressed,FALSE,BT_DOWN,2,MUIM_CallHook,&HookDown);
    /* Code Starts */

    cla=MemList.head;
    n=0;

    while(cla!=NULL)
        {
            sprintf(str, "\338%s", cla->name);
            if (n >= 3)
                strcpy(str, cla->name);
            strcpy(cla->name, str);
            DoMethod(LW_CLASSES, MUIM_List_InsertSingle, cla->name, MUIV_List_Insert_Bottom);
            n++;

            cla = cla->next;
        }

    for(i=0;i<NCOM;i++)
        {
            DoMethod(LW_COM, MUIM_List_InsertSingle, Comms[i], MUIV_List_Insert_Bottom);
        }


    RebuildPop();


        /* On ouvre la fen�tre */
        set(window,MUIA_Window_Open,TRUE);
        if(GetReg(str)!=0) MUInfo(LocaleMsg[CAP_SHAREWARE]);

        {
            ULONG sigs = 0;

            while (DoMethod(app, MUIM_Application_NewInput, &sigs) != MUIV_Application_ReturnID_Quit && ByeBye == 0)
            {
                if (sigs)
                {
                    sigs = Wait(sigs | SIGBREAKF_CTRL_C);
                    if (sigs & SIGBREAKF_CTRL_C)
                        break;
                }
            }
        }

    set(window,MUIA_Window_Open,FALSE);
    FreeAll();
    fail(app, 0);
    MUI_DeleteCustomClass(cl_ddlisttree);
}

/****************************************/
/*                                      */
/* Function : wbmain                    */
/*                                      */
/* Goal     : task runned from WB       */
/*                                      */
/****************************************/
int wbmain(struct WBStartup *mess)
{
    main(0, NULL);
}

void MUInfo(char *st)
{
    MUI_Request(app, window, 0, LocaleMsg[CAP_INFO], LocaleMsg[CAP_OK], st);
}

long MuiReq(char *st)
{
    return (MUI_Request(app, window, 0, LocaleMsg[CAP_INFO], LocaleMsg[CAP_YES_NO], st));
}

void RebuildPop()
{
    struct Class *cla;
    struct Action *act;
    struct MUIS_Listtree_TreeNode *node;

    set(LW_POP, MUIA_Listtree_Quiet, TRUE);
    cla = MemList.head;

    DoMethod(LW_POP, MUIM_Listtree_Remove, MUIV_Listtree_Remove_ListNode_Root, MUIV_Listtree_Remove_TreeNode_All, 0);

    while (cla != NULL)
    {
        sprintf(str, "\338%s", cla->name);
        node = (struct MUIS_Listtree_TreeNode *)DoMethod(LW_POP, MUIM_Listtree_Insert, str, NULL, MUIV_Listtree_Insert_ListNode_Root, MUIV_Listtree_Insert_PrevNode_Tail, TNF_LIST);

        act = cla->action;
        while (act != NULL)
        {
            sprintf(str, "%s", act->name);
            DoMethod(LW_POP, MUIM_Listtree_Insert, str, act, node, MUIV_Listtree_Insert_PrevNode_Tail, 0);
            act = act->next;
        }

        cla = cla->next;
    }

    set(LW_POP, MUIA_Listtree_Quiet, FALSE);
}

int CopyClass(struct Action *act)
{

    if (act != NULL)
    {
        strcpy(SelectedAction->exec, act->exec);
        strcpy(SelectedAction->name, act->name);
        strcpy(SelectedAction->delay, act->delay);
        SelectedAction->wbrun = act->wbrun;

        HookFuncLwa();
        DoMethod(LW_ACTIONS, MUIM_List_Redraw, MUIV_List_Redraw_Active);
        RebuildPop();
    }

    return (0);
}
