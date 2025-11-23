/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : req.c 

	Goal : Handle Requesters

	Ver  : 1.0

****************************************/

#include <intuition/intuition.h>
#include <libraries/reqtools.h>
#include <clib/reqtools.h>

#include "includes.h"
extern char	**LocaleMsg;


struct ReqToolsBase *ReqToolsBase;

void InitRT(void)
{

void Inf(char *);

ReqToolsBase=(struct ReqToolsBase *)OpenLibrary("reqtools.library",38);
if(ReqToolsBase==NULL)
	{
	Inf("Reqtools.Library V38+ ????");
	exit(1);
	}

}

void FreeRT(void)
{
CloseLibrary(ReqToolsBase);
}

/****************************************/
/*					*/
/* Function : EasyReq			*/
/*					*/
/* Goal     : Internal Use		*/
/*					*/
/*					*/
/* Return Values : Selected Button Id 	*/
/*					*/
/****************************************/

int EasyReq(struct EasyStruct *es,int *ip,int arg1)
	{
	   return(EasyRequestArgs(NULL , es, ip,&arg1 ));
	}


/****************************************/
/*					*/
/* Function : Req			*/
/*					*/
/* Goal     : Pop Up a requester	*/
/*	      With text 'mess'		*/
/*	      and yes/no buttons	*/
/*					*/
/*					*/
/* Return Values : 1=Yes		*/
/*	    	   0=No			*/
/*					*/
/****************************************/

int Req(char *mess)
{

static ULONG tags[3] = { RTEZ_ReqTitle, (ULONG)0, TAG_END };

tags[1]=(ULONG)LocaleMsg[CA_SURE];

if(rtEZRequest(mess,LocaleMsg[CA_YES_NO],NULL,(struct TagItem *)tags)) return(1); else return(0);

}


/****************************************/
/*					*/
/* Function : Inf ASL			*/
/*					*/
/* Goal     : Pop Up a requester	*/
/*	      With text mess and a 	*/
/*	      OKAY Button		*/
/*					*/
/*					*/
/* Return Values : NONE 		*/
/*					*/
/****************************************/

void Inf(char *mess)
{
int iflags = IDCMP_DISKINSERTED;

struct EasyStruct ES = {
    sizeof (struct EasyStruct),
    0,
    LocaleMsg[CA_INFORMATION],
    "%s",
    LocaleMsg[CA_OKAY]
};

EasyReq(&ES,&iflags,(int)mess);
}

/****************************************/
/*					*/
/* Function : Info			*/
/*					*/
/* Goal     : Pop Up a requester	*/
/*	      With text mess and a 	*/
/*	      OKAY Button		*/
/*					*/
/*					*/
/* Return Values : NONE 		*/
/*					*/
/****************************************/

void Info(char *mess)
{

static ULONG tags[3] = { RTEZ_ReqTitle, (ULONG)0, TAG_END };

tags[1]=(ULONG)LocaleMsg[CA_INFORMATION];

rtEZRequest(mess,LocaleMsg[CA_OKAY],NULL,(struct TagItem *)tags);
}


/****************************************/
/*					*/
/* Function : Freq			*/
/*					*/
/* Goal     : Open an RT Requester	*/
/*					*/
/* file : file choosen by user		*/
/* mode : 0=files,1=dirs,2=volumes	*/
/* text : title of the Requester	*/
/*					*/
/* Return Values : 			*/
/*					*/
/*  -2 : Unable to alloc the requester	*/
/*  -1 : User canceled the requester	*/
/*   1 : Okay				*/
/*					*/
/****************************************/

int Freq(char *file,int mode,char *text)
{
struct rtFileRequester *req;
APTR ret;
char path[256];
char total[512];
int len;

NameFromLock(DirLock,path,255);

req=(struct rtFileRequester *)rtAllocRequestA(RT_FILEREQ,NULL);
if(req==NULL) return(-2);

if(mode!=2) rtChangeReqAttr(req,RTFI_Dir,path,TAG_DONE);
if(mode==0) ret=(APTR)rtFileRequest(req,file,text,TAG_DONE);
if(mode==1) ret=(APTR)rtFileRequest(req,NULL,text,RTFI_Flags,FREQF_NOFILES,TAG_DONE);
if(mode==2) ret=(APTR)rtFileRequest(req,NULL,text,RTFI_VolumeRequest,VREQF_ALLDISKS|VREQF_NOASSIGNS,TAG_DONE);

if(ret==NULL) {rtFreeRequest(req);return(-1);}

len=strlen(req->Dir);
total[0]=0;

if(len!=0)
{
strcpy(total,req->Dir);
if(total[len-1]!=':') strcat(total,"/");
}

if(mode==0 && strlen(file)!=0) strcat(total,file);
sprintf(file,"%c%s%c",'"',total,'"');

rtFreeRequest(req);
return(1);
}


int GetText(char *buffer,int len,char *msg)
{
if(!rtGetString (buffer, len, msg, NULL, TAG_END))
	return(-1);
else return(0);
}

