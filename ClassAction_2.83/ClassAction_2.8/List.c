
/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : List.c

	Goal : Handle Exec lists

	Ver  : 2.00

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

#include "includes.h"
#include "gadgets.h"

/****************************************/
/*										*/
/* Function : PutList()					*/
/*										*/
/* Goal     : attach the exec list 'li'	*/
/*	      to the listview gadget		*/
/*	      with ID='id'					*/
/*										*/
/*										*/
/* Return Values : NONE					*/
/*										*/
/****************************************/

void PutList(int id, struct List *li)
{
	GT_SetGadgetAttrs(Test_Gadgets[id], Test_Wnd, NULL,
					  GTLV_Labels, li, TAG_DONE);
}

/****************************************/
/*										*/
/* Function : AddList()					*/
/*										*/
/* Goal     : add a node named 'name'	*/
/*	      to the list 'li'				*/
/*										*/
/* Return Values : 						*/
/*										*/
/*  0 = Node Added Succesfully			*/
/* -1 = Not Enough memory !!!			*/
/*										*/
/****************************************/

int AddList(struct List *li, char *name, char pri)
{
	struct Node *nod;

	nod = (struct Node *)AllocMem(sizeof(struct Node), MEMF_CLEAR | MEMF_PUBLIC);
	if (nod == NULL)
		return (-1);

	nod->ln_Name = name;
	nod->ln_Type = NT_UNKNOWN;
	nod->ln_Pri = pri;
	AddTail(li, nod);
	return (0);
}

/****************************************/
/*										*/
/* Function : FreeList  				*/
/*										*/
/* Goal     : Free the list 'li'		*/
/*            from the memory			*/
/*										*/
/* Return Values : NONE					*/
/*										*/
/****************************************/

void FreeList(struct List *li)
{
	struct Node *nod;

	Forbid();
	while (nod = RemHead(li))
		FreeMem(nod, sizeof(struct Node));
	Permit();
}

/****************************************/
/*										*/
/* Function : FreeMemList				*/
/*										*/
/* Goal     : free ALL Classes lists	*/
/*										*/
/* Return Values : NONE 				*/
/*										*/
/****************************************/

void FreeMemList(void)
{
	struct Class *cla, *ctmp;
	struct Action *act, *atmp;

	cla = MemList.head;

	while (cla != NULL)
	{
		ctmp = cla->next;
		act = cla->action;
		while (act != NULL)
		{
			atmp = act->next;
			free(act);
			act = atmp;
		}
		free(cla);
		cla = ctmp;
	}

	act = Generic.action;
	while (act != NULL)
	{
		atmp = act->next;
		free(act);
		act = atmp;
	}
}

/****************************************/
/*										*/
/* Function : MemToList					*/
/*										*/
/* Goal     : Transform The Class List 	*/
/*	      into an exec list				*/
/*	      pointed by Alist				*/
/*										*/
/*										*/
/* Return Values : NONE 				*/
/*										*/
/****************************************/

void MemToList(void)
{
	struct Class *cla;

	NewList(&Alist);
	cla = MemList.head;

	while (cla != NULL)
	{
		AddList(&Alist, cla->name, 0);
		cla = cla->next;
	}
}

/****************************************/
/*										*/
/* Function : FreeScanList				*/
/*										*/
/* Goal     : Free the dir list 		*/
/*										*/
/* Return Values : NONE 				*/
/*										*/
/****************************************/

void FreeScanList()
{
	struct Scan *sc, *stmp;

	sc = Dirlist.head;

	while (sc != NULL)
	{
		stmp = sc->next;
		free(sc);
		sc = stmp;
	}

	sc = Filelist.head;

	while (sc != NULL)
	{
		stmp = sc->next;
		free(sc);
		sc = stmp;
	}
}

/****************************************/
/*										*/
/* Function : InitLists					*/
/*										*/
/* Goal     : initialize the used lists	*/
/*										*/
/* Return Values : NONE 				*/
/*										*/
/****************************************/

void InitLists(void)
{
	NewList(&Flist);
	NewList(&Alist);
	MemList.head = MemList.tail = NULL;
	MemList.size = 0;
	Filelist.head = Filelist.tail = NULL;
	Dirlist.head = Dirlist.tail = NULL;
	Filelist.size = 0;
	Dirlist.size = 0;
}
