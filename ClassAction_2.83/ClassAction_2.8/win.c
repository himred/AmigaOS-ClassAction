/****************************************

        Class Action Source

        (c) by 1994 Salim Gasmi

        File : win.c

        Goal : Build up the Interface

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

#include <utility/hooks.h>
#include "gadgets.h"
#include "includes.h"


extern struct Hook Callback;
int TX,TY,DX,DY;
char *j;

#define MA              4

/****************************************/
/*                                      */
/* Function : HookFunct                 */
/*                                      */
/* Goal     : Handle my own drawing     */
/*            routine for listviews     */
/*                                      */
/* Return Values : Internal             */
/*                                      */
/****************************************/

ULONG __saveds __asm HookFunc(register __a0 struct Hook         *hook,
                              register __a2 struct Node         *object,
                              register __a1 struct LVDrawMsg    *mess)
{
int color;

if(mess->lvdm_MethodID!=LV_DRAW) return(LVCB_UNKNOWN);

if(mess->lvdm_State==LVR_NORMAL) color=0; else color=mess->lvdm_DrawInfo->dri_Pens[FILLPEN];
SetAPen(mess->lvdm_RastPort,color);
RectFill(mess->lvdm_RastPort,mess->lvdm_Bounds.MinX,mess->lvdm_Bounds.MinY,mess->lvdm_Bounds.MaxX,mess->lvdm_Bounds.MaxY);

color=object->ln_Pri+1;

SetAPen(mess->lvdm_RastPort,color);

Move(mess->lvdm_RastPort,mess->lvdm_Bounds.MinX+1,mess->lvdm_Bounds.MinY+mess->lvdm_RastPort->Font->tf_Baseline);
Text(mess->lvdm_RastPort,object->ln_Name,strlen(object->ln_Name));
return(LVCB_OK);
}




/****************************************/
/*                                      */
/* Function : Setup Screen              */
/*                                      */
/* Goal     : attach a screen Scr       */
/*            to the WBScreen           */
/*                                      */
/*                                      */
/* Return Values :                      */
/*                                      */
/*                                      */
/****************************************/

int SetupScreen( void )
{

Test_Wdt=(UBYTE *)Ver;

if(UseDefFont==0 && LoadPrefs()==0)
{

TheFont=(struct TextFont *)OpenFont(&Myattr);
DirFont=(struct TextFont *)OpenFont(&Dirattr);

if(TheFont!=NULL)
{
TX=TheFont->tf_XSize;
TY=TheFont->tf_YSize;
} else {Myattr=topaz8;TX=TX=8;}

if(DirFont!=NULL && DirFont->tf_XSize<15)
{
DX=DirFont->tf_XSize;
DY=DirFont->tf_YSize;
} else {Dirattr=topaz8;DX=DY=8;}

}
else
{
TheFont=NULL;
DirFont=NULL;
Myattr=Dirattr=topaz8;
DX=DY=TX=TY=8;
}

}


/****************************************/
/*                                      */
/* Function : CloseDownScreen           */
/*                                      */
/* Goal     : free Scr screen           */
/*                                      */
/****************************************/

void CloseDownScreen( void )
{


        if ( VisualInfo ) {
                FreeVisualInfo( VisualInfo );
                VisualInfo = NULL;
        }

        if ( Scr        ) {
                UnlockPubScreen( NULL, Scr );
                Scr = NULL;
        }

}



/****************************************/
/*                                      */
/* Function : OpenTestWindow            */
/*                                      */
/* Goal     : Open the main Window      */
/*            and attach the gadgets    */
/*                                      */
/* Return Values :                      */
/*                                      */
/*                                      */
/****************************************/

int OpenTest_Window( int WinSize )
{


#define YO              4

int minsize;
int YY;
char TheScreenName[128];

Callback.h_Entry=(void *)HookFunc;
Callback.h_SubEntry=NULL;
Callback.h_Data=NULL;

YY=TY+4;
minsize=11*(YY)+10*2+2*MA+TY;
if(WinSize<minsize) WinSize=minsize;

if(PubScreen==0) Scr = LockPubScreen( "Workbench" );
else
	{
	PubFrontScreen(TheScreenName);
	Scr = LockPubScreen(TheScreenName);	
	}

VisualInfo = GetVisualInfo( Scr, TAG_DONE );


        if ( ! CreateContext( &Test_GList ))
                return( 1L );

        if(WinX==-1) WinX=Test_Left;
        if(WinY==-1) WinY=Test_Top;

        if ( ! ( Test_Wnd = OpenWindowTags( NULL,
                                WA_Left,        WinX,
                                WA_Top,         WinY,
                                WA_Width,       Test_Width,
                                WA_Height,      WinSize,
                                WA_IDCMP,       BUTTONIDCMP|LISTVIEWIDCMP|IDCMP_CLOSEWINDOW|IDCMP_NEWSIZE,
                                WA_Flags,       WFLG_DRAGBAR|WFLG_DEPTHGADGET|WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_CLOSEGADGET|WFLG_RMBTRAP|WFLG_SIZEGADGET|WFLG_SIZEBBOTTOM,
                                WA_Gadgets,     Test_GList,
                                WA_Title,       Test_Wdt,
                                WA_PubScreen,   Scr,
                                WA_MaxWidth,    Test_Width,
                                WA_MinHeight,   minsize,
                                WA_MaxHeight,   10000,
                                TAG_DONE )))
        return( 4L );

        SigGad=1 << Test_Wnd->UserPort->mp_SigBit;

        SetNewSize(WinSize,0);
        GT_RefreshWindow(Test_Wnd,NULL);
        return( 0L );
}


/****************************************/
/*                                      */
/* Function : CloseTestWindow           */
/*                                      */
/* Goal     : Close the main Window     */
/*            and free the gadgets      */
/*                                      */
/****************************************/

void CloseTest_Window( void )
{

        if ( Test_Wnd        ) {
                CloseWindow( Test_Wnd );
                Test_Wnd = NULL;
        }


        if ( Test_GList      ) {
                FreeGadgets( Test_GList );
                Test_GList = NULL;
        }

CloseDownScreen();
}


/****************************************/
/*                                      */
/* Function : PutString                 */
/*                                      */
/* Goal     : Attach the text string    */
/*            to the gadget ID          */
/*                                      */
/****************************************/

void PutString(int id,char *string)
{
        GT_SetGadgetAttrs(Test_Gadgets[id],Test_Wnd,NULL,
            GTTX_Text,string,TAG_DONE);

}

/****************************************/
/*                                      */
/* Function : PutText                   */
/*                                      */
/* Goal     : Attach the text string    */
/*            to the gadget ID          */
/*                                      */
/****************************************/

void PutText(int id,char *string)
{
        GT_SetGadgetAttrs(Test_Gadgets[id],Test_Wnd,NULL,
            GTST_String,string,TAG_DONE);

}

/****************************************/
/*                                      */
/* Function : SetDrives                 */
/*                                      */
/* Goal     : Attach the Drive mapping  */
/*            to the drive gadgets      */
/*                                      */
/****************************************/

void SetDrives(void)
{
int i;

for(i=0;i<11;i++)
if(Drives[i][0]==0)
GT_SetGadgetAttrs(Test_Gadgets[GD_Proj_Dr1+i],Test_Wnd,NULL,GA_Disabled,TRUE,TAG_DONE);

if(OldPath[0]!=0) strcpy(Drives[11],OldPath);
PutText(GD_Proj_Dir,Drives[11]);
}

int SetNewSize(int WinSize,char flag)
{

#define YO              4

int YY;
char txt[512];

WinSize-=TY+10+2*MA;            /* bottom border height */

YY=TY+4;                        /* normal button height */

int bot=WinSize-YY-MA;          /* top Y of buttons bottom */



int rs,as,ry,ay,rw;
int MaxChars;
int t1,t2,taille;
struct TextAttr *attr;



rs=bot;
rs=(rs)/DY;
rs=rs*DY;
ry=bot-rs;

as=rs-(2*YY+4+6);
as=(as)/DY;
as=as*DY;
ay=bot-as;

int HH=(int)((bot+YY-ry)/11);   /* Height of a drive button */
int XO=(bot+YY-ry-11*HH)/2+MA+ry;

MaxChars=(312-16)/DX-1;
MaxFile=MaxChars-9;
MaxDir=MaxFile-2;
rw=(DX)*(MaxChars+1)+18;
Number=rw;
t1=strlen(LocaleMsg[CA_FILE_BUTT]);
t2=strlen(LocaleMsg[CA_CLASS_BUTT]);

if(t1>=t2) taille=t1; else taille=t2;

taille=taille*DX;



struct NewGadget Test_NGad[] = {
        405, ay+MA, 201, as, (UBYTE *)"", NULL, GD_Proj_Actions, PLACETEXT_ABOVE, NULL, NULL,
        545, bot+MA, 60, YY, (UBYTE *)LocaleMsg[CA_QUIT_BUTT], NULL, GD_Proj_Quit, PLACETEXT_IN, NULL, NULL,
        475, bot+MA, 60, YY, (UBYTE *)LocaleMsg[CA_ABOUT_BUTT], NULL, GD_Proj_About, PLACETEXT_IN, NULL, NULL,
        415+taille, YY+2+MA+ry, 190-taille, YY, (UBYTE *)"", NULL, GD_Proj_Class, PLACETEXT_LEFT, NULL, NULL,
        415+taille,MA+ry,190-taille,YY,(UBYTE *)"",NULL,GD_Proj_File,PLACETEXT_LEFT,NULL,NULL,
85,ry+MA,rw,rs,(UBYTE *)"",NULL,GD_Proj_Req,PLACETEXT_ABOVE,NULL,NULL,
85,bot+MA,312-70,YY,(UBYTE *)"",NULL,GD_Proj_Dir,PLACETEXT_ABOVE,NULL,NULL,
328,bot+MA,70,YY,(UBYTE *)LocaleMsg[CA_PARENT_BUTT],NULL,GD_Proj_Parent,PLACETEXT_IN,NULL,NULL,
405,bot+MA,60,YY,(UBYTE *)LocaleMsg[CA_INFO_BUTT],NULL,GD_Proj_Info,PLACETEXT_IN,NULL,NULL,
YO,XO,70,HH-2,(UBYTE *)Drives[0],NULL,GD_Proj_Dr1,PLACETEXT_IN,NULL,NULL,
YO,XO+HH,70,HH-2,(UBYTE *)Drives[1],NULL,GD_Proj_Dr2,PLACETEXT_IN,NULL,NULL,
YO,XO+2*HH,70,HH-2,(UBYTE *)Drives[2],NULL,GD_Proj_Dr3,PLACETEXT_IN,NULL,NULL,
YO,XO+3*HH,70,HH-2,(UBYTE *)Drives[3],NULL,GD_Proj_Dr4,PLACETEXT_IN,NULL,NULL,
YO,XO+4*HH,70,HH-2,(UBYTE *)Drives[4],NULL,GD_Proj_Dr5,PLACETEXT_IN,NULL,NULL,
YO,XO+5*HH,70,HH-2,(UBYTE *)Drives[5],NULL,GD_Proj_Dr6,PLACETEXT_IN,NULL,NULL,
YO,XO+6*HH,70,HH-2,(UBYTE *)Drives[6],NULL,GD_Proj_Dr7,PLACETEXT_IN,NULL,NULL,
YO,XO+7*HH,70,HH-2,(UBYTE *)Drives[7],NULL,GD_Proj_Dr8,PLACETEXT_IN,NULL,NULL,
YO,XO+8*HH,70,HH-2,(UBYTE *)Drives[8],NULL,GD_Proj_Dr9,PLACETEXT_IN,NULL,NULL,
YO,XO+9*HH,70,HH-2,(UBYTE *)Drives[9],NULL,GD_Proj_Dr10,PLACETEXT_IN,NULL,NULL,
YO,XO+10*HH,70,HH-2,(UBYTE *)Drives[10],NULL,GD_Proj_Dr11,PLACETEXT_IN,NULL,NULL
};

if(flag==1) strcpy(txt,GetString(Test_Gadgets[GD_Proj_Dir]));

FreeGadgets( Test_GList );

        struct NewGadget        ng;
        struct Gadget   *g;
        UWORD           lc, tc;
        UWORD           offx = Scr->WBorLeft, offy = Scr->WBorTop + Scr->RastPort.TxHeight + 1;

        if ( ! ( g = CreateContext( &Test_GList )))
                return( 1L );


        for( lc = 0, tc = 0; lc < Test__CNT; lc++ ) {

                CopyMem((char * )&Test_NGad[ lc ], (char * )&ng, (long)sizeof( struct NewGadget ));

                if(lc==GD_Proj_Req || lc==GD_Proj_Actions) attr=&Dirattr; else attr=&Myattr;
                ng.ng_VisualInfo = VisualInfo;
                ng.ng_TextAttr   = attr;
                ng.ng_LeftEdge  += offx;
                ng.ng_TopEdge   += offy;

                Test_Gadgets[ lc ] = g = CreateGadgetA((ULONG)Test_GTypes[ lc ], g, &ng, ( struct TagItem * )&Test_GTags[ tc ] );


                while( Test_GTags[ tc ] ) tc += 2;
                tc++;

                if ( NOT g )
                        return( 2L );

                }


if(flag==1)
{
SetAPen(Test_Wnd->RPort,0);
RectFill(Test_Wnd->RPort,0,0,640,WinSize+TY+10+2*MA);
SetDrives();
PutText(GD_Proj_Dir,txt);
}

SetAPen(Test_Wnd->RPort,1);

Move(Test_Wnd->RPort,405+Test_Wnd->BorderLeft,Test_Wnd->BorderTop+TY+ry+MA);
Text(Test_Wnd->RPort,LocaleMsg[CA_FILE_BUTT],strlen(LocaleMsg[CA_FILE_BUTT]));
Move(Test_Wnd->RPort,405+Test_Wnd->BorderLeft,Test_Wnd->BorderTop+TY+YY+ry+MA+2);
Text(Test_Wnd->RPort,LocaleMsg[CA_CLASS_BUTT],strlen(LocaleMsg[CA_CLASS_BUTT]));

RefreshWindowFrame(Test_Wnd);
Refresh=1;
WindowHeight=WinSize+TY+10+2*MA;
return(0);

}

