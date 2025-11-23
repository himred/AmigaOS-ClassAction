
/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : xfd.c 

	Goal : Handle decrunching 

	Ver  : 1.1

****************************************/

#include "xfd.h"
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
#include <fcntl.h>
#include "gadgets.h"
#include "includes.h"

struct Library *XfdBase;

/****************************************/
/*					*/
/* Function : OpenXfdLib		*/
/*					*/
/* Goal     : Try to open the		*/
/*	      xfdmaster.library		*/
/*					*/
/* Return Values : 			*/
/*					*/
/*	 0= OKAY			*/
/*	-1= Failed			*/
/*					*/
/****************************************/

int OpenXfdLib(void)
{
XfdBase=(struct Library *)OpenLibrary("xfdmaster.library",0);
if(XfdBase==NULL) return(-1); else return(0);
}


/****************************************/
/*					*/
/* Function : CloseXfdLib()		*/
/*					*/
/* Goal     : Close the xfdmaster	*/
/*	      Library			*/
/*					*/
/* Return Values : None			*/
/*					*/
/****************************************/

void CloseXfdLib(void)
{
if(XfdBase!=NULL) CloseLibrary(XfdBase);
}



/****************************************/
/*					*/
/* Function : Decrunch			*/
/*					*/
/* Goal     :Decrunch the buffer 'buff'	*/
/*	     of len 'buflen'		*/
/*	     and return the pointer	*/
/*	     of the decrunched buffer	*/
/*	     Initialize 'cruncher'	*/
/*	    to the used decruncher	*/
/*	    and 'newlen' with the len	*/
/*	    of the decrunched buffer	*/
/*					*/
/* Return Values : 			*/
/*					*/
/*	the decrunched buffer pointer	*/
/*	or NULL if decrunching failed	*/
/*					*/
/****************************************/

APTR Decrunch(char *buff,int size,char *cruncher,int *newlen)
{
struct Buffer *xfd;
int ret;
APTR dec;

if(XfdBase==NULL) return(NULL);

xfd=xfdAllocBufferInfo();
xfd->xfdbi_SourceBuffer=buff;
xfd->xfdbi_SourceBufLen=size;
xfd->xfdbi_Special=NULL;

ret=xfdRecogBuffer(xfd);
if(ret==FALSE)
	{
	strcpy(cruncher,LocaleMsg[CA_NOT_CR]);
	xfdFreeBufferInfo(xfd);
	return(NULL);
	}


if(xfd->xfdbi_PackerFlags<2) 
	{
	strcpy(cruncher,LocaleMsg[CA_NOT_DATA]);
	xfdFreeBufferInfo(xfd);
	return(NULL);
	}

strcpy(cruncher,xfd->xfdbi_PackerName);
xfd->xfdbi_TargetBufMemType=MEMF_PUBLIC;

ret=xfdDecrunchBuffer(xfd);
if(ret==FALSE)
	{
	strcpy(cruncher,LocaleMsg[CA_NOT_DECR]);
	xfdFreeBufferInfo(xfd);
	return(NULL);
	}

dec=xfd->xfdbi_TargetBuffer;	

*newlen=xfd->xfdbi_TargetBufLen;
xfdFreeBufferInfo(xfd);
return(dec);
}

