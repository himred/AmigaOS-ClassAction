/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : req.c

	Goal : Handle Requesters

	Ver  : 1.0

****************************************/

#define CATCOMP_NUMBERS

#include <intuition/intuition.h>
#include <libraries/reqtools.h>
#include <clib/reqtools.h>
#include "locale.h"

extern char **LocaleMsg;

struct ReqToolsBase *ReqToolsBase;

void InitRT(void)
{

	void Inf(char *);

	ReqToolsBase = (struct ReqToolsBase *)OpenLibrary("reqtools.library", 38);
	if (ReqToolsBase == NULL)
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
/*										*/
/* Function : EasyReq					*/
/*										*/
/* Goal     : Internal Use				*/
/*										*/
/*										*/
/* Return Values : Selected Button Id 	*/
/*										*/
/****************************************/

int EasyReq(struct EasyStruct *es, int *ip, int arg1)
{
	return (EasyRequestArgs(NULL, es, ip, &arg1));
}

/****************************************/
/*										*/
/* Function : Req						*/
/*										*/
/* Goal     : Pop Up a requester		*/
/*	      With text 'mess'				*/
/*	      and yes/no buttons			*/
/*										*/
/*										*/
/* Return Values : 1=Yes				*/
/*	    	   0=No						*/
/*										*/
/****************************************/

int Req(char *mess)
{

	static ULONG tags[3] = {RTEZ_ReqTitle, (ULONG)0, TAG_END};

	tags[1] = (ULONG)LocaleMsg[CAP_ARE_U_SURE];

	if (rtEZRequest(mess, LocaleMsg[CAP_YES_NO], NULL, (struct TagItem *)tags))
		return (1);
	else
		return (0);
}

/****************************************/
/*										*/
/* Function : Inf ASL					*/
/*										*/
/* Goal     : Pop Up a requester		*/
/*	      With text mess and a 			*/
/*	      OKAY Button					*/
/*										*/
/*										*/
/* Return Values : NONE 				*/
/*										*/
/****************************************/

void Inf(char *mess)
{
	int iflags = IDCMP_DISKINSERTED;

	struct EasyStruct ES = {
		sizeof(struct EasyStruct),
		0,
		LocaleMsg[CAP_INFO],
		"%s",
		LocaleMsg[CAP_OK]};

	EasyReq(&ES, &iflags, (int)mess);
}

/****************************************/
/*										*/
/* Function : Info						*/
/*										*/
/* Goal     : Pop Up a requester		*/
/*	      With text mess and a 			*/
/*	      OKAY Button					*/
/*										*/
/*										*/
/* Return Values : NONE 				*/
/*										*/
/****************************************/

void Info(char *mess)
{

	static ULONG tags[3] = {RTEZ_ReqTitle, (ULONG)0, TAG_END};

	tags[1] = (ULONG)LocaleMsg[CAP_INFO];

	rtEZRequest(mess, LocaleMsg[CAP_OK], NULL, (struct TagItem *)tags);
}

/****************************************/
/*										*/
/* Function : Freq						*/
/*										*/
/* Goal     : Open an RT Requester		*/
/*										*/
/* file : file choosen by user			*/
/*										*/
/* Return Values : 						*/
/*										*/
/*  -2 : Unable to alloc the requester	*/
/*  -1 : User canceled the requester	*/
/*   1 : Okay							*/
/*										*/
/****************************************/

int Getfile(char *file)
{
	struct rtFileRequester *req;
	APTR ret;
	char total[512];
	int len;

	req = rtAllocRequestA(RT_FILEREQ, NULL);
	if (req == NULL)
		return (-2);

	strcpy(file, "");
	ret = (APTR)rtFileRequest(req, file, "", TAG_END);

	if (ret == NULL)
	{
		rtFreeRequest(req);
		return (-1);
	}

	len = strlen(req->Dir);
	total[0] = 0;

	if (len != 0)
	{
		strcpy(total, req->Dir);
		if (total[len - 1] != ':')
			strcat(total, "/");
	}

	if (strlen(file) != 0)
		strcat(total, file);
	strcpy(file, total);

	rtFreeRequest(req);
	return (1);
}
