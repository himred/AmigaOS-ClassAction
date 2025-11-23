/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : global.c

	Goal : define global vars

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

#include "newtypes.h"
#include "ClassActionPrefs_rev.h"

#define CATCOMP_NUMBERS
#include "locale.h"


static char Soft[]=VERSTAG;


struct ClassList	MemList;
struct Class		Generic;
struct Class		Directory;
char			**LocaleMsg;


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
