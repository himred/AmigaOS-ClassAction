/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : pop.c

	Goal : Build up the Interface

	Ver  : 1.0

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

#define POPGAD 100
#define NCOM 13

char *Comms[NCOM];

		
char *Ret[]={"[f]","[s]","[b]","[x]","[F]","[S]","[B]","[X]","REQF[","REQD[","REQV[","REQT[","SURE["};


struct List Xlist;
struct Gadget *Mylist;
struct Window *Pop_Wnd;
struct Gadget *Pop_List;
struct NewGadget NGad={
	1, 1, 399, 109, (UBYTE *)"", NULL, POPGAD, PLACETEXT_ABOVE, NULL, NULL};


int OpenPop_Window( void )
{
	struct NewGadget	ng;
	struct Gadget	*g;
	ULONG		Tagy=(TAG_DONE);

	UWORD		offx = Scr->WBorLeft, offy = Scr->WBorTop + Scr->RastPort.TxHeight + 1;



	if ( ! ( g = CreateContext( &Pop_List )))
		return( 1L );

		CopyMem((char * )&NGad, (char * )&ng, (long)sizeof( struct NewGadget ));

		ng.ng_VisualInfo = VisualInfo;
		ng.ng_TextAttr   = &topaz8;
		ng.ng_LeftEdge  += offx;
		ng.ng_TopEdge   += offy;

		g = CreateGadgetA((ULONG)LISTVIEW_KIND, g, &ng, ( struct TagItem * )&Tagy);


	Mylist=g;

	if ( ! ( Pop_Wnd = OpenWindowTags( NULL,
				WA_Left,	200,
				WA_Top,		100,
				WA_Width,	409,
				WA_Height,	122,
				WA_IDCMP,	LISTVIEWIDCMP|IDCMP_CLOSEWINDOW|IDCMP_REFRESHWINDOW,
				WA_Flags,	WFLG_DRAGBAR|WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_CLOSEGADGET,
				WA_Gadgets,	Pop_List,
				WA_Title,	LocaleMsg[CAP_COMMANDS],
				WA_ScreenTitle,	"",
				WA_PubScreen,	Scr,
				TAG_DONE )))
	return( 4L );

	GT_RefreshWindow( Pop_Wnd, NULL );

	return( 0L );
}

void ClosePop_Window( void )
{
	if ( Pop_Wnd        ) {
		CloseWindow( Pop_Wnd );
		Pop_Wnd = NULL;
	}

	if ( Pop_List      ) {
		FreeGadgets( Pop_List );
		Pop_List = NULL;
	}
}


void PopUp(char *sel)
{
int i;
void HandleIDCMPOP(char *);

NewList(&Xlist);

Comms[0]=LocaleMsg[CAP_F_COMM];
Comms[1]=LocaleMsg[CAP_S_COMM];
Comms[2]=LocaleMsg[CAP_B_COMM];
Comms[3]=LocaleMsg[CAP_X_COMM];
Comms[4]=LocaleMsg[CAP_BF_COMM];
Comms[5]=LocaleMsg[CAP_BS_COMM];
Comms[6]=LocaleMsg[CAP_BB_COMM];
Comms[7]=LocaleMsg[CAP_BX_COMM];
Comms[8]=LocaleMsg[CAP_REQF_COMM];
Comms[9]=LocaleMsg[CAP_REQD_COMM];
Comms[10]=LocaleMsg[CAP_REQV_COMM];
Comms[11]=LocaleMsg[CAP_REQT_COMM];
Comms[12]=LocaleMsg[CAP_SURE_COMM];	

for(i=0;i<NCOM;i++)
AddList(&Xlist,Comms[i],0);


OpenPop_Window();

GT_SetGadgetAttrs(Mylist,Pop_Wnd,NULL,
      GTLV_Labels,&Xlist,TAG_DONE);

HandleIDCMPOP(sel);

ClosePop_Window();
FreeList(&Xlist);
}

void HandleIDCMPOP(char *sel)
{

        struct IntuiMessage *Msg;    
        long Class,Code,ID;  
        struct Gadget *SelGadget;     
        BOOL ENDTEST=FALSE;           
        BOOL CHECKGADGET=TRUE;        


        while (!(ENDTEST))
        {
            WaitPort(Pop_Wnd->UserPort);
            while (Msg=GT_GetIMsg(Pop_Wnd->UserPort))
            { 

                Class = Msg->Class;
                Code  = Msg->Code;
                if ((Class == IDCMP_GADGETUP) || (Class == IDCMP_GADGETDOWN))
                { 

                    if (SelGadget=(struct Gadget *)Msg->IAddress)
                        ID =   SelGadget->GadgetID;
                }
                GT_ReplyIMsg(Msg);

                char *res=NULL;
                switch (Class)
                {
                    case IDCMP_GADGETUP:
                    case IDCMP_GADGETDOWN:


if(ID==POPGAD)
{
strcpy(sel,Ret[Code]);
ENDTEST=TRUE;
}
 

break;

                    case IDCMP_CLOSEWINDOW:
			sel[0]=0;
                        ENDTEST=TRUE;
                        break;
                    default:
                        break;
                }
            }
        }

        Forbid();

        while (Msg=GT_GetIMsg(Pop_Wnd->UserPort)) GT_ReplyIMsg(Msg);

        ModifyIDCMP(Pop_Wnd,0L);

        Permit();
    }
