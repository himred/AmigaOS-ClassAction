/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : Gadgets.c

	Goal : main

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
#include <utility/hooks.h>
#include <string.h>

#include "gadgets.h"
#include "includes.h"


extern struct Hook Callback;


/****************************************/
/*					*/
/* Function : HookFunct			*/
/*					*/
/* Goal     : Handle my own drawing	*/
/*	      routine for listviews	*/
/*					*/
/* Return Values : Internal		*/
/*					*/
/****************************************/

ULONG __saveds __asm HookFunc(register __a0 struct Hook		*hook,
			      register __a2 struct Node		*object,
			      register __a1 struct LVDrawMsg	*mess)
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



int SetupScreen( void )
{

	if ( ! ( Scr = LockPubScreen( PubScreenName )))
		return( 1L );

	if ( ! ( VisualInfo = GetVisualInfo( Scr, TAG_DONE )))
		return( 2L );


Callback.h_Entry=(void *)HookFunc;
Callback.h_SubEntry=NULL;
Callback.h_Data=NULL;

}

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

int OpenTest_Window( void )
{
	struct NewGadget	ng;
	struct Gadget	*g;
	UWORD		lc, tc;
	UWORD		offx = Scr->WBorLeft, offy = Scr->WBorTop + Scr->RastPort.TxHeight + 1;

	
struct NewGadget T_NGad[] = {
	9, 154, 102, 14, NULL, NULL, GD_Offset_Cycle, 0, NULL, NULL,
	9, 180, 53, 13, (UBYTE *)LocaleMsg[CAP_ADD], NULL, GD_Class_Add, PLACETEXT_IN, NULL, NULL,
	67, 180, 57, 13, (UBYTE *)LocaleMsg[CAP_DEL], NULL, GD_Class_Del, PLACETEXT_IN, NULL, NULL,
	300, 180, 48, 13, (UBYTE *)LocaleMsg[CAP_ADD], NULL, GD_Action_Add, PLACETEXT_IN, NULL, NULL,
	352, 180, 48, 13, (UBYTE *)LocaleMsg[CAP_DEL], NULL, GD_Action_Del, PLACETEXT_IN, NULL, NULL,
	5, 204, 146, 14, (UBYTE *)LocaleMsg[CAP_SAVE], NULL, GD_Proj_Save, PLACETEXT_IN, NULL, NULL,
	220, 204, 146, 14, (UBYTE *)LocaleMsg[CAP_USE], NULL, GD_Proj_Quit, PLACETEXT_IN, NULL, NULL,
	560, 20, 20, 13, (UBYTE *)"?", NULL, GD_Proj_About, PLACETEXT_IN, NULL, NULL,
	6, 20, 263, 91, (UBYTE *)LocaleMsg[CAP_CLASSES], NULL, GD_Class_List, PLACETEXT_ABOVE, NULL, NULL,
	300, 20, 256, 91, (UBYTE *)LocaleMsg[CAP_ACTIONS], NULL, GD_Action_List, PLACETEXT_ABOVE, NULL, NULL,
	90, 121, 179, 14, (UBYTE *)LocaleMsg[CAP_CLASSNAME], NULL, GD_Class_Name, PLACETEXT_LEFT, NULL, NULL,
	119, 154, 150, 14, NULL, NULL, GD_Class_Off, 0, NULL, NULL,
	90, 138, 179, 14, (UBYTE *)LocaleMsg[CAP_MATCH_NAME], NULL, GD_Class_Ext, 0, NULL, NULL,
	339, 138, 217, 14, (UBYTE *)LocaleMsg[CAP_NAME], NULL, GD_Action_Name, PLACETEXT_LEFT, NULL, NULL,
	339, 154, 217, 14, (UBYTE *)LocaleMsg[CAP_EXEC], NULL, GD_Action_Exec, PLACETEXT_LEFT, NULL, NULL,
	404,180,48,13, (UBYTE *)LocaleMsg[CAP_LOAD],NULL,GD_Action_Load,NULL,NULL,NULL,
	339,121,87,14,NULL,NULL,GD_Action_Wb,0,NULL,NULL,
	430,121,70,14,(UBYTE *)LocaleMsg[CAP_STACK],NULL,GD_Action_Stack,PLACETEXT_ABOVE,NULL,NULL,
	456,180,48,13,(UBYTE *)LocaleMsg[CAP_COMM],NULL,GD_Action_Com,NULL,NULL,NULL	,
	508,180,48,13,(UBYTE *)LocaleMsg[CAP_COPY],NULL,GD_Action_Copy,NULL,NULL,NULL,
	560,76,20,13,(UBYTE *)LocaleMsg[CAP_UP],NULL,GD_Action_Up,NULL,NULL,NULL,
	560,91,20,13,(UBYTE *)LocaleMsg[CAP_DOWN],NULL,GD_Action_Down,NULL,NULL,NULL,
	436, 204, 146, 14, (UBYTE *)LocaleMsg[CAP_CANCEL], NULL, GD_Proj_Cancel, PLACETEXT_IN, NULL, NULL,
	510,121,46,14,(UBYTE *)LocaleMsg[CAP_DELAY],NULL,GD_Action_Delay,PLACETEXT_ABOVE,NULL,NULL,
	129,180,100,13,(UBYTE *)LocaleMsg[CAP_L_LEARN],NULL,GD_Class_Learn,NULL,NULL,NULL	
};


	if ( ! ( g = CreateContext( &Test_GList )))
		return( 1L );

	for( lc = 0, tc = 0; lc < Test__CNT; lc++ ) {

		CopyMem((char * )&T_NGad[ lc ], (char * )&ng, (long)sizeof( struct NewGadget ));

		ng.ng_VisualInfo = VisualInfo;
		ng.ng_TextAttr   = &topaz8;
		ng.ng_LeftEdge  += offx;
		ng.ng_TopEdge   += offy;

		Test_Gadgets[ lc ] = g = CreateGadgetA((ULONG)Test_GTypes[ lc ], g, &ng, ( struct TagItem * )&Test_GTags[ tc ] );


		while( Test_GTags[ tc ] ) tc += 2;
		tc++;

		if ( NOT g )
			return( 2L );
	}

	if ( ! ( Test_Wnd = OpenWindowTags( NULL,
				WA_Left,	Test_Left,
				WA_Top,		Test_Top,
				WA_Width,	Test_Width,
				WA_Height,	Test_Height + offy,
				WA_IDCMP,	CYCLEIDCMP|BUTTONIDCMP|LISTVIEWIDCMP|STRINGIDCMP|IDCMP_CLOSEWINDOW|IDCMP_REFRESHWINDOW,
				WA_Flags,	WFLG_DRAGBAR|WFLG_DEPTHGADGET|WFLG_SMART_REFRESH|WFLG_ACTIVATE,
				WA_Gadgets,	Test_GList,
				WA_Title,	Test_Wdt,
				WA_ScreenTitle,	Test_Wdt,
				WA_PubScreen,	Scr,
				WA_MinWidth,	67,
				WA_MinHeight,	21,
				WA_MaxWidth,	640,
				WA_MaxHeight,	256,
				TAG_DONE )))
	return( 4L );

	GT_RefreshWindow( Test_Wnd, NULL );

	return( 0L );
}

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
}

void main()
{

struct Class *cla;
void HandleIDCMP();

InitRT();
InitLocale();

NewList(&Clist);
NewList(&Alist);
MemList.head=MemList.tail=NULL;
MemList.size=0;

LoadMem();
SortList();
LoadGen();

/* Adding Generic class to list */

cla=MemList.head->next;
MemList.head->next=&Generic;
Generic.next=cla;
if(MemList.tail==MemList.head) MemList.tail=&Generic;
MemList.size++;

MemToList();

SetupScreen();
OpenTest_Window();

PutList(GD_Class_List,&Clist);

DisableClass(TRUE);
DisableAction(TRUE);
HandleIDCMP();

CloseTest_Window();
CloseDownScreen();
FreeList(&Clist);
FreeList(&Alist);

FreeMemList();
FreeLocale();
FreeRT();
}

void PutString(int id,char *string)
{
        GT_SetGadgetAttrs(Test_Gadgets[id],Test_Wnd,NULL,
            GTST_String,string,TAG_DONE);

}

void PutCycle(int id,int string)
{
        GT_SetGadgetAttrs(Test_Gadgets[id],Test_Wnd,NULL,
            GTCY_Active,string,TAG_DONE);
}


void InserText(int id,char *st,int p)
{
char tmp[256];
char tmp2[256];
char *res;


res=((struct StringInfo *)Test_Gadgets[id]->SpecialInfo)->Buffer;
strcpy(tmp,res);

strcpy(tmp2,tmp+p);
tmp[p]=0;
strcat(tmp,st);
strcat(tmp,tmp2);
PutString(id,tmp);
}


void Status(int id,int k)
{

        GT_SetGadgetAttrs(Test_Gadgets[id],Test_Wnd,NULL,
            GA_Disabled,k,TAG_DONE);

}

int GetCycle(int id)
{
int p;
ULONG tag[]={(ULONG)GTCY_Active,(ULONG)&p,TAG_DONE};


GT_GetGadgetAttrsA(Test_Gadgets[id],Test_Wnd,NULL,(struct TagItem *)tag);
return(p);
}

void Select(int id,int k)
{
        GT_SetGadgetAttrs(Test_Gadgets[id],Test_Wnd,NULL,
            GTLV_Selected,k,TAG_DONE);

}

void Visible(int d,int k)
{
        GT_SetGadgetAttrs(Test_Gadgets[d],Test_Wnd,NULL,
            GTLV_Top,k,TAG_DONE);
}


void DisableClass(int k)
{
Status(GD_Class_Name,k);
Status(GD_Class_Ext,k);
Status(GD_Class_Off,k);
}

void DisableAction(int k)
{
Status(GD_Action_Name,k);
Status(GD_Action_Exec,k);
Status(GD_Action_Stack,k);
Status(GD_Action_Wb,k);
Status(GD_Action_Delay,k);
}

void CopyOn()
{
int i;

for(i=0;i<Test__CNT;i++)
	if(i!=GD_Class_List) Status(i,TRUE);

}

void CopyOff()
{
int i;

for(i=0;i<Test__CNT;i++)
	Status(i,FALSE);
}


int wbmain(void *wbmsg)
{
main();
}

