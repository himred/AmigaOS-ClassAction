
/****************************************

	ClassActionPrefs Source

	(c) by 1995 Salim Gasmi

	File : Arexx.c

	Goal : Handle Arexx stuff

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
#include <workbench/startup.h>
#include <workbench/Workbench.h>
#include <lib/rexx.h>

#include "includes.h"
#include <stdio.h>

char *RexxHostName = "CAP_Slave";

long DoRexxCommand(void *msg, struct MsgPort *port, char *res, char **pres)
{
	return (0L);
}

void NotifyMaster()
{
	PlaceRexxCommandDirect(NULL, "ClassAction.01", "USE", NULL, NULL);
}
