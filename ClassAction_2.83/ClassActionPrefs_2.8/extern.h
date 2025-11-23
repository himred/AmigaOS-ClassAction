
/****************************************

	Class Action Source

	(c) by 1995 Salim Gasmi

	File : extern.h

	Goal : define extern vars

	Ver  : 1.00

****************************************/


#include "newtypes.h"
#include "gadgets.h"

extern struct IntuitionBase *IntuitionBase;
extern struct Library       *GadToolsBase;

extern struct Screen        *Scr;
extern UBYTE                 *PubScreenName;
extern APTR                  VisualInfo;
extern struct Window        *Test_Wnd;
extern struct Gadget        *Test_GList;
extern struct Gadget        *Test_Gadgets[Test__CNT];
extern UWORD                 Test_Left;
extern UWORD                 Test_Top;
extern UWORD                 Test_Width;
extern UWORD                 Test_Height;
extern UBYTE                *Test_Wdt;
extern UBYTE                *Offset_Cycle0Labels[];
extern UBYTE		    	*WB[];
extern UBYTE                *Class_Cyclo0Labels[];
extern UBYTE		    	*Ver;
extern struct TextAttr       topaz8;
extern UWORD                 Test_GTypes[];
extern struct NewGadget      Test_NGad[];
extern ULONG                 Test_GTags[];
extern struct List           Clist,Alist;
extern struct ClassList	     MemList;
extern struct Class	     	Generic;
extern char		     		**LocaleMsg;

