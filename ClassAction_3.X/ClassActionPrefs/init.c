/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : init.c

	Goal : main

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

void InitAll()
{

	struct Class *cla;

	InitRT();
	InitLocale();

	MemList.head = MemList.tail = NULL;
	MemList.size = 0;

	LoadMem();
	SortList();
	LoadGen();

	/* Adding Generic class to list */

	cla = MemList.head->next;
	MemList.head->next = &Generic;
	Generic.next = cla;
	if (MemList.tail == MemList.head)
		MemList.tail = &Generic;
	MemList.size++;

	/* Adding Directory class to list */

	cla = MemList.head->next->next;
	MemList.head->next->next = &Directory;
	Directory.next = cla;
	MemList.size++;
}

void FreeAll()
{

	FreeMemList();
	FreeLocale();
	FreeRT();
}
