/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : Arexx.c

	Goal : ARexx CA Start interface

	Ver  : 3.0

****************************************/

#include <workbench/startup.h>
#include <workbench/Workbench.h>
#include <lib/rexx.h>
#include <stdio.h>

char *RexxHostName = "CAStart";

long DoRexxCommand(void *msg, struct MsgPort *port, char *res, char **pres)
{
	return (0L);
}

main(int argc, char **argv)
{
	char sys[512];

	sprintf(sys, "LOAD %s", argv[1]);
	PlaceRexxCommandDirect(NULL, "ClassAction.01", sys, NULL, NULL);
}

int wbmain(struct WBStartup *mess)
{
	char sys[1024];
	char full[1024];

	NameFromLock(mess->sm_ArgList[1].wa_Lock, full, 1023);
	AddPart(full, mess->sm_ArgList[1].wa_Name, 1023);
	sprintf(sys, "LOAD %s", full);
	PlaceRexxCommandDirect(NULL, "ClassAction.01", sys, NULL, NULL);
}
