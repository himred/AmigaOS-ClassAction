/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : Global.c

	Goal : Initialize globals vars

	Ver  : 3.00

****************************************/

#include <dos/dos.h>
#include <dos/dosextens.h>
#include <exec/types.h>
#include <exec/exec.h>
#include <graphics/displayinfo.h>
#include <graphics/gfxbase.h>
#include <clib/exec_protos.h>
#include <clib/utility_protos.h>
#include <string.h>
#include <Workbench/Workbench.h>
#include <utility/hooks.h>

#include "newtypes.h"
#include "ClassAction_rev.h"

struct List Clist, Alist, Flist; /* my lists				*/
struct ClassList MemList;		 /* my list header			*/
char DecrunchFlag;				 /* =1 if decrunch succeed	*/
char Drives[MXDRV][50];			 /* drives buttons			*/
struct DosList *Doslist;		 /* ptr to doslist			*/
char Cruncher[30];				 /* name of cruncher		*/
char ProgramName[255];			 /* Name of the proggy   	*/
struct DiskObject *IconObj;		 /* Icon Object				*/
struct Library *WbBase;			 /* WB LIb Base				*/
struct Library *ConsoleDevice;	 /* Console Device Base   	*/
struct IOStdReq IOStdReq;		 /* for console device		*/
char ButPath[MXDRV][255];		 /* Path of buttons			*/
char DevString[100];			 /* device string			*/
char IconFile[100];				 /* Icon disk file			*/
struct Class Generic;			 /* ptr to generic class	*/
struct Class Directory;			 /* ptr to dir class		*/
struct Library *DataBase;		 /* datatype library		*/
char **LocaleMsg;				 /* messages array			*/
char CurrentPath[1024];			 /* current path			*/
int Active;						 /* Global var				*/
char GtLike;					 /* Use MUI Style DIR		*/
char ShowIcons;					 /* Shall we disply Icons	*/
char PrefsPath[512];			 /* Path of CAP				*/
char IconStart;					 /* =1 if start as Icon		*/
char DoComm;					 /* =1 if we do Comment		*/
char HKey[6][30];				 /* Defined Keys			*/
