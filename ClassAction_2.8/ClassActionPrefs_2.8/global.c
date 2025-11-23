/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : global.c

	Goal : define global vars

	Ver  : 1.0

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

#include "gadgets.h"
#include "newtypes.h"
#include "ClassActionPrefs_rev.h"

#define CATCOMP_NUMBERS
#include "locale.h"


static char Soft[]=VERSTAG;

struct Screen         *Scr = NULL;
UBYTE                 *PubScreenName = NULL;
APTR                   VisualInfo = NULL;
struct Window         *Test_Wnd = NULL;
struct Gadget         *Test_GList = NULL;
struct Gadget         *Test_Gadgets[Test__CNT];
UWORD                  Test_Left = 12;
UWORD                  Test_Top = 3;
UWORD                  Test_Width = 595;
UWORD                  Test_Height = 222;
UBYTE					*Ver = VSTRING;
	
UBYTE                 *Test_Wdt =VERS;


struct List			Clist,Alist;
struct ClassList	MemList;
struct Class		Generic;
struct Hook			Callback;
char				**LocaleMsg;


UBYTE *WB[] = {
	(UBYTE *)"Cli",
	(UBYTE *)"WB",
	(UBYTE *)"No Cli",
	(UBYTE *)"Arexx",
	NULL };


UBYTE *Offset_Cycle0Labels[] = {
	(UBYTE *)"Offset #1",
	(UBYTE *)"Offset #2",
	(UBYTE *)"Offset #3",
	(UBYTE *)"Offset #4",
	(UBYTE *)"Offset #5",
	NULL };

struct TextAttr topaz8 = {
	( STRPTR )"topaz.font", 8, 0x00, 0x01 };


struct NewGadget *Test_NGad ;



UWORD Test_GTypes[] = {
	CYCLE_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	LISTVIEW_KIND,
	LISTVIEW_KIND,
	STRING_KIND,
	STRING_KIND,
	STRING_KIND,
	STRING_KIND,
	STRING_KIND,
	BUTTON_KIND,
	CYCLE_KIND,
	STRING_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	STRING_KIND,
	BUTTON_KIND

};

ULONG Test_GTags[] = {
	(GTCY_Labels), (ULONG)&Offset_Cycle0Labels[ 0 ], (TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(GTLV_ShowSelected),NULL,(GTLV_CallBack),(ULONG)&Callback,(TAG_DONE),
	(GTLV_ShowSelected),NULL,(TAG_DONE),
	(GTST_MaxChars), 19, (TAG_DONE),
	(GTST_MaxChars), 49, (TAG_DONE),
	(GTST_MaxChars), 99, (TAG_DONE),
	(GTST_MaxChars), 19, (TAG_DONE),
	(GTST_MaxChars), 240, (TAG_DONE),
	(TAG_DONE),
	(GTCY_Labels), (ULONG)&WB[ 0 ], (TAG_DONE),
	(GTST_MaxChars), 6, (TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(GTST_MaxChars), 2, (TAG_DONE),
	(TAG_DONE)
};
