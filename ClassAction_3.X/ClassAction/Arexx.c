/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : Arexx.c

	Goal : Handle Arexx commands

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
#include <workbench/startup.h>
#include <workbench/Workbench.h>
#include <lib/rexx.h>
#include "classaction_rev.h"

#include "includes.h"
#include <stdio.h>
#include <fcntl.h>

char *RexxHostName = "ClassAction";

char sep[2] = {'"', 0};

/****************************************
	Function : DoRexxCommand

	Goal : Called whengot an Arexx command
		   Handle them and does the
		   needed stuff

	Return Values : Always 0L
****************************************/
long DoRexxCommand(void *msg, struct MsgPort *port, char *res, char **pres)
{
	int IsIcon(void);
	void PopUp(struct Class *, char *);

	struct List AppList;
	struct Action *act, *run;
	struct Class *cla;
	int ret = 0;
	int nnn = 0;
	int h;
	char *full, *pp;
	char toto[512];

	*pres = "";

	if (strcmp(res, "QUIT") == 0)
	{
		Active = 0;
		return (0L);
	}

	if (strcmp(res, "VER") == 0)
	{
		*pres = VERS;
		return (0L);
	}

	if (strcmp(res, "SHOW") == 0 && IsIcon() != 0)
	{
		Active = 3;
		return (0L);
	}

	if (strncmp(res, "GETCLASS ", 9) == 0)
	{
		full = res + 9;
		if (full[0] == '"')
		{
			full++;
			full[strlen(full) - 1] = 0;
		}
		h = open(full, O_RDONLY);
		if (h < 0)
		{
			*pres = LocaleMsg[CA_FILE_NOT_FOUND];
			return (0L);
		}
		close(h);
		cla = Getclass(full, (char *)FilePart(full), 0);
		*pres = cla->name;
		return (0L);
	}

	if (strcmp(res, "STATUS") == 0)
	{
		if (IsIcon() != 0)
			*pres = "0";
		else
			*pres = "1";
		return (0L);
	}

	if (strcmp(res, "APPICONIFY") == 0 && IsIcon() == 0)
	{
		Active = 2;
		return (0L);
	}

	if (strcmp(res, "USE") == 0)
	{
		FreeMemList();
		FreeList(&Alist);
		LoadMem();
		LoadGen();
		Active = 4;
		return (0L);
	}

	if (strncmp(res, "LOAD ", 5) == 0)
	{
		full = res + 5;
		if (full[0] == '"')
		{
			full++;
			full[strlen(full) - 1] = 0;
		}
		h = open(full, O_RDONLY);
		if (h < 0)
		{
			*pres = LocaleMsg[CA_FILE_NOT_FOUND];
			return (0L);
		}

		close(h);
		cla = Getclass(full, (char *)FilePart(full), 0);
		act = cla->action;
		NewList(&AppList);
		nnn = 0;
		while (act != NULL)
		{
			AddList(&AppList, act->name, 0);
			nnn = nnn + 1;
			act = act->next;
		}

		if (nnn == 1)
			Runner(cla->action, full, 0);
		else
		{
			PopUp(cla, full);
			*pres = "OK";
			return (0L);
		}
	}

	if (strncmp(res, "ACTION ", 7) == 0)
	{
		strcpy(toto, res + 7);
		full = toto;
		if (full[0] == '"')
		{
			full++;
			pp = (char *)strstr(full, sep);
			if (pp == NULL)
			{
				*pres = LocaleMsg[CA_FILE_NOT_FOUND];
				return (0L);
			}
			*pp = 0;
			pp++;
		}
		else
		{
			pp = (char *)strstr(full, " ");
			if (pp == NULL)
			{
				*pres = LocaleMsg[CA_FILE_NOT_FOUND];
				return (0L);
			}
			*pp = 0;
			pp++;
		}

		h = open(full, O_RDONLY);
		if (h < 0)
		{
			*pres = LocaleMsg[CA_FILE_NOT_FOUND];
			return (0L);
		}
		close(h);

		cla = Getclass(full, (char *)FilePart(full), 0);
		run = NULL;
		act = cla->action;
		while (act != NULL)
		{
			if (run == NULL)
				if (strstr(act->name, pp) != NULL)
					run = act;
			act = act->next;
		}

		if (run != NULL)
			Runner(run, full, 0);

		*pres = "OK";
		return (0L);
	}
	return (0L);
}
