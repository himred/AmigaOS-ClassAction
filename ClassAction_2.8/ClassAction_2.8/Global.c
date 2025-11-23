/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : Global.c

	Goal : Initialize globals vars

	Ver  : 2.00

****************************************/

#include <dos/dos.h>
#include <dos/dosextens.h>
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
#include <Workbench/Workbench.h>
#include <libraries/commodities.h>
#include <utility/hooks.h>

#include "gadgets.h"
#include "newtypes.h"
#include "ClassAction_rev.h"

static char Soft[] = VERSTAG;

UBYTE *Ver = VERS;

struct List Clist, Alist, Flist;
struct ClassList MemList;

struct Screen *Scr = NULL;
UBYTE *PubScreenName = NULL;
APTR VisualInfo = NULL;
struct Window *Test_Wnd = NULL;
struct Gadget *Test_GList = NULL;
struct Gadget *Test_Gadgets[Test__CNT];
UWORD Test_Left = 10;
UWORD Test_Top = 15;
UWORD Test_Width = 620;
UBYTE *Test_Wdt = Ver;
char DecrunchFlag;
char AssignFlag;
char ParentFlag;
struct DosList *Doslist;
struct ScanList Filelist, Dirlist;
BPTR DirLock;
char Drives[12][50];
char Cruncher[30];
struct Process *Proc;
int WindowHeight;
char ProgramName[255];
struct MsgPort *IconPort;
struct DiskObject *IconObj;
struct Library *WbBase;
struct AppIcon *IconApp;
int Number;
char AppStart;
struct TextAttr Myattr;
struct TextAttr Dirattr;
struct TextFont *TheFont = NULL;
struct TextFont *DirFont = NULL;
ULONG SigGad = 0;
ULONG SigRexx = 0;
ULONG SigApp = 0;
ULONG SigCx = 0;
char OldPath[255];
char Refresh;
struct MsgPort *ArexxPort;
struct Library *CxBase = NULL;
CxObj *Broker = NULL;
struct MsgPort *CxPort = NULL;
char Active = 1;
char CxPri = 0;
char IconName[100];
int IconX;
int IconY;
char ButPath[11][255];
int MaxDir;
int MaxFile;
char UseDefFont;
char TFname[128];
char DFname[128];
char Stopper = 0;
struct Hook Callback;
int WinX;
int WinY;
char DevString[100];
char IconFile[100];
struct Class Generic;
struct Library *DataBase = NULL;
char **LocaleMsg;
char ReqBug;
char PubScreen;
char CAHotKey[32];

struct TextAttr topaz8 = {
	(STRPTR) "topaz.font", 8, 0x00, 0x01};

UWORD Test_GTypes[] = {
	LISTVIEW_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	TEXT_KIND,
	TEXT_KIND,
	LISTVIEW_KIND,
	STRING_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND};

ULONG Test_GTags[] = {
	(GTLV_ShowSelected), NULL, (GTLV_CallBack), (ULONG)&Callback, (TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(GTTX_Border), TRUE, (TAG_DONE),
	(GTTX_Border), TRUE, (TAG_DONE),
	(GTLV_ShowSelected), NULL, (GTLV_CallBack), (ULONG)&Callback, (TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE)};
