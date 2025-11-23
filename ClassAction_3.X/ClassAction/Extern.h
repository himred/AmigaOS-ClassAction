
/****************************************

	Class Action Source

	(c) by 1995 Salim Gasmi

	File : extern.h

	Goal : define extern vars

	Ver  : 3.00

****************************************/

#include "newtypes.h"
#include <dos/dos.h>
#include <dos/dosextens.h>
#include "newtypes.h"

extern struct List Clist, Alist, Flist; /* my lists					*/
extern struct ClassList MemList;		/* my list header			*/
extern char DecrunchFlag;				/* =1 if decrunch succeed	*/
extern char Drives[MXDRV][50];			/* drives buttons			*/
extern struct DosList *Doslist;			/* ptr to doslist			*/
extern char Cruncher[30];				/* name of cruncher			*/
extern char ProgramName[255];			/* Name of the proggy   	*/
extern struct DiskObject *IconObj;		/* Icon Object				*/
extern struct Library *WbBase;			/* WB LIb Base				*/
extern char ButPath[MXDRV][255];		/* Path of buttons			*/
extern char DevString[100];				/* device string			*/
extern char IconFile[100];				/* Icon disk file			*/
extern struct Class Generic;			/* ptr to generic class		*/
extern struct Class Directory;			/* ptr to generic class		*/
extern struct Library *DataBase;		/* datatype library			*/
extern struct Library *ConsoleDevice;	/* Console Device Base  	*/
extern struct IOStdReq IOStdReq;		/* for console device  		*/
extern char **LocaleMsg;				/* messages array			*/
extern char CurrentPath[1024];			/* current path         	*/
extern int Active;						/* Global var				*/
extern char GtLike;						/* Shall we Use MDL			*/
extern char ShowIcons;					/* Shall we show icons 		*/
extern char PrefsPath[512];				/* Path of CAP          	*/
extern char IconStart;					/* =1 if start as icon		*/
extern char DoComm;						/* =1 if we do Comment  	*/
extern char HKey[6][30];				/* Defined Keys         	*/
/* END OF EXTERNS */