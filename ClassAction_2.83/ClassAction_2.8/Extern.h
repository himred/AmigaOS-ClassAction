
/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : extern.h

	Goal : define extern vars

	Ver  : 2.00

****************************************/


#include "newtypes.h"
#include <dos/dos.h>
#include <dos/dosextens.h>

extern struct IntuitionBase *IntuitionBase;	/* Intuition lib Base	*/
extern struct Library       *GadToolsBase;	/* Gadtools lib Base	*/

extern struct Screen        *Scr;		/* My used screen	*/
extern UBYTE                 *PubScreenName;	/* the Public screen	*/
extern APTR                  VisualInfo;	/* my visual info	*/
extern struct Window        *Test_Wnd;		/* my Window		*/
extern struct Gadget        *Test_GList;	/* my gadget list	*/
extern struct Gadget        *Test_Gadgets[8];	/* my gadgets		*/
extern UWORD                 Test_Left;		/* top left window X	*/
extern UWORD                 Test_Top;		/* top left window Y	*/
extern UWORD                 Test_Width;	/* window width		*/
extern UBYTE                *Test_Wdt;		/* window title		*/
extern UBYTE		    *Ver;		/* Soft Version		*/
extern struct TextAttr       topaz8;		/* used font		*/
extern UWORD                 Test_GTypes[];	/* gadgets types	*/
extern ULONG                 Test_GTags[];	/* gadgets tags		*/
extern struct List           Clist,Alist,Flist;	/* my lists		*/
extern struct ClassList	     MemList;		/* my list header	*/
extern char 		    DecrunchFlag;	/* =1 if decrunch succed*/
extern char		    ParentFlag;		/* =1 if a parent exists*/
extern char		    AssignFlag;		/* =1 if assigns shown	*/
extern struct ScanList	    Filelist,Dirlist;	/* lists of files/dirs	*/
extern BPTR		    DirLock;		/* lock of current dir	*/
extern char		    Drives[12][50];	/* drives buttons	*/
extern struct DosList	    *Doslist;		/* ptr to doslist	*/
extern struct Process	    *Proc;		/* ptr to the process	*/
extern char		     Cruncher[30];	/* name of cruncher	*/
extern int		     WindowHeight;	/* Height of the window */
extern char		     ProgramName[255];  /* Name of the proggy   */
extern struct MsgPort	    *IconPort;		/* Msg Port for AppIcon */
extern struct DiskObject    *IconObj;		/* Icon Object		*/
extern struct Library	    *WbBase;		/* WB LIb Base		*/
extern struct AppIcon	    *IconApp;		/* The AppIcon		*/
extern int 		    Number;		/* Id of action file	*/
extern char		    AppStart;		/* =1 if App Start	*/ 
extern struct TextAttr	    Myattr;		/* the used attr	*/
extern struct TextAttr	    Dirattr;		/* the used attr	*/
extern struct TextFont	   *TheFont;		/* the loaded font	*/
extern struct TextFont	   *DirFont;		/* the loaded font	*/
extern ULONG		   SigRexx;		/* Rexx mask Signal	*/
extern ULONG		   SigGad;		/* Gadtool mask signal	*/
extern ULONG		   SigApp;		/* AppIcon mask signal	*/
extern ULONG		   SigCx;		/* commodities signal	*/
extern struct MsgPort	   *ArexxPort;		/* Arexx Port		*/
extern char		   OldPath[255];	/* Previous path	*/
extern char		   Refresh;		/* =1 if we fake dir()  */
extern struct Library 	   *CxBase;		/* Commodity library	*/
extern struct MsgPort 	   *CxPort;		/* Commodity port	*/
extern char 		   Active;		/* Internal flag	*/
extern char 		   CxPri;		/* Commodity Priority	*/
extern char	        IconName[100];		/* Icon text		*/
extern int		IconX;			/* Icon X pos		*/
extern int 		IconY;			/* Icon Y pos		*/
extern char		ButPath[11][255];	/* Path of buttons	*/
extern int		MaxDir;			/* maxlength of a dir	*/
extern int		MaxFile;		/* maxlength of a file	*/
extern char		UseDefFont;		/* =1 if topaz8		*/
extern char		TFname[128];		/* Text Font name	*/
extern char		DFname[128];		/* screen font name	*/
extern char		Stopper;		/* internal forbid	*/
extern struct Hook	Callback;		/* ptr to drawing fct	*/
extern int              WinX;        		/* Window X pos		*/
extern int              WinY;        		/* Window Y pos		*/
extern char             DevString[100];		/* device string	*/
extern char		IconFile[100];		/* Icon disk file	*/
extern struct Class	Generic;		/* ptr to generic class	*/
extern struct Library 	*DataBase;		/* datatype library	*/
extern char		**LocaleMsg;		/* messages array	*/
extern char		ReqBug;			/* flag if reqs bugs	*/
extern char		PubScreen;		/* flag to use pscreens */
extern char		CAHotKey[32];		/* commod hotkey	*/

/* END OF EXTERNS */
