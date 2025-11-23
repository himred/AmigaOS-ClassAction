
/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : List.c

	Goal : Handle Exec lists

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
#include <string.h>

#include "includes.h"

int AddList(struct List *li, char *name, int pri)
{
	struct Node *nod;

	nod = (struct Node *)AllocMem(sizeof(struct Node), MEMF_CLEAR | MEMF_PUBLIC);
	if (nod == NULL)
		return (-1);

	nod->ln_Name = name;
	nod->ln_Pri = pri;
	nod->ln_Type = NT_UNKNOWN;
	AddTail(li, nod);
	return (0);
}

void FreeList(struct List *li)
{

	struct Node *nod;

	Forbid();
	while (nod = RemHead(li))
		FreeMem(nod, sizeof(struct Node));
	Permit();
}

void FreeMemList(void)
{
	struct Class *cla, *ctmp;
	struct Action *act, *atmp;

	/* freeing generic list */

	cla = MemList.head->next->next->next;
	MemList.head->next = cla;

	act = Generic.action;
	while (act != NULL)
	{
		atmp = act->next;
		free(act);
		act = atmp;
	}

	act = Directory.action;
	while (act != NULL)
	{
		atmp = act->next;
		free(act);
		act = atmp;
	}

	/* Freeing Normal List */

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
}
