/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : main.c

	Goal : first called function 

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

#include "gadgets.h"
#include "includes.h"
#include <stdio.h>

int main(int argc,char **argv)
{
int x;
int mode;
ULONG Got;
int registered;
char User[50];

OldPath[0]=0;
WinX=WinY=-1;


if(argc!=0) strcpy(ProgramName,argv[0]);

InitRT();
ReadToolsTypes();

InitLocale();

if(InitCx()==-1)
		{
		Info(LocaleMsg[CA_ALREADY_RUNNING]);
		if(WbBase!=NULL) CloseLibrary(WbBase);
		if(IconPort!=NULL) DeleteMsgPort(IconPort);
		FreeCx();
		return(0);
		}

InitArexxPort();
if(InitAppStuff()==-1) 
		{
		Info(LocaleMsg[CA_APP_ERROR]);
		if(WbBase!=NULL) CloseLibrary(WbBase);
		if(IconPort!=NULL) DeleteMsgPort(IconPort);
		return(0);
		}
	
InitLists();
ExtractSeg();
if(LoadMem()==-1) Info(LocaleMsg[CA_NO_CONFIG]);
LoadGen();

DataBase=(struct Library *)OpenLibrary("datatypes.library",0);

registered=GetReg(User);
if(registered!=0) strcpy(User,"NOT REGISTERED");

x=0;
mode=AppStart;
Active=1;
SetupScreen();

	if(mode==0)
		{ 
		Refresh=0;
		OpenTest_Window(WindowHeight);
		SetDrives();
		Dir();
		}

	if(mode==1) CreateAppIcon();

	if(mode==2) Test_Wnd=NULL;

	while(x!=1)
		{
		Got=Wait(SigApp|SigGad|SigRexx|SigCx);
		if((Got&SigGad))
			{ 
			x=HandleIDCMP(User);
			if(x==2) 
				{
				CloseTest_Window();
				Test_Wnd=NULL;
				CreateAppIcon();
				}
			}

		if((Got&SigApp)) 
			{
			x=HandleApp();
			if(x==2)
				{
			RemIcon();
			OpenTest_Window(WindowHeight);
			SetDrives();
			Refresh=1;
				}
			}

		if((Got&SigCx)) 
			{
			x=HandleCx();
			if(x==2)
			{
			if(Test_Wnd==NULL)
				{
				RemIcon();
				OpenTest_Window(WindowHeight);
				SetDrives();
				Refresh=1;
				}
			else
				{
				CloseTest_Window();
				Test_Wnd=NULL;	
				}
			}
			}

		if((Got&SigRexx)) 
			{
			ProcessRexxCommands(NULL);
			if(Active==0) x=1;

			if(Active==2) 
				{
				CloseTest_Window();
				Test_Wnd=NULL;
				CreateAppIcon();
				Active=1;
				}

			if(Active==3)
				{
				RemIcon();	
				OpenTest_Window(WindowHeight);
				SetDrives();
				Refresh=1;
				Active=1;
				}

			if(Active==4)
				{
				Active=1;
				Refresh=3;
				}

			}
	}

CloseTest_Window();
RemIcon();
FreeList(&Alist);
FreeMemList();
FreeScanList();
unlink(WBRUN);
CloseXfdLib();
UnLock(DirLock);
if(IconPort!=NULL) DeleteMsgPort(IconPort);
FreeCx();
FreeDiskObject(IconObj);
if(TheFont!=NULL) CloseFont(TheFont);
if(DirFont!=NULL) CloseFont(DirFont);
IsAscii(NULL);
CloseLibrary(WbBase);

if(registered!=0) Info(LocaleMsg[CA_SHAREWARE]); 
FreeLocale();
FreeRT();

return(0);

}


int wbmain(struct WBStartup *mess)
{
strcpy(ProgramName,(char *)(mess->sm_ArgList[0].wa_Name));
main(0,NULL);
}

