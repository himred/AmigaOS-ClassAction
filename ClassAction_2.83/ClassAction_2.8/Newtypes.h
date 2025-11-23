/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : newtypes.h

	Goal : declares my own structures
		for holding classes & actions

	Ver  : 2.00

****************************************/

#define WBRUN "T:ClassAction_RunTask"

#define SFILET "ENV:ClassAction.prefs"
#define SFILE "ENVARC:ClassAction.prefs"

#define GFILET "ENV:ClassAction_Gen.prefs"
#define GFILE "ENVARC:ClassAction_Gen.prefs"

#define PREF "ENV:Sys/Font.prefs"
#define PREF2 "ENVARC:Sys/Font.prefs"

#define SHEADER "CAFS20"
#define GHEADER "CAFG20"

#define EOC "CLASS_END"
#define EOSF "FILE_END"

#define UNKNOWN "Unknown Class"

struct Action
{
	char name[20];		 /* Action Name 					*/
	char exec[250];		 /* Execute string					*/
	char wbrun;			 /* Cli/WB run Flag					*/
	char stack[8];		 /* ASCII Value of the CLI Stack	*/
	char delay[3];		 /* Delay of Cli					*/
	struct Action *next; /* Next Action						*/
};

struct Class
{
	char name[20];			   /* Class Name				*/
	char suffix[210];		   /* Parsed Matchname			*/
	char value[5][50];		   /* Value of the 5 offsets	*/
	struct Class *next;		   /* Next Class				*/
	struct Action *action;	   /* First Action of the Class	*/
	struct Action *actiontail; /* Last Action of the Class	*/
	int nbaction;			   /* # of actions defined		*/
};

struct ClassList
{
	struct Class *head; /* First Defined Class		*/
	struct Class *tail; /* Last Defined Class		*/
	int size;			/* # of Classes defined		*/
};

struct Scan
{
	char name[50];	   /* Real Local File Name 		*/
	char tmp[50];	   /* Displayed File Name		*/
	struct Scan *next; /* Next File Entry			*/
	struct Scan *prec; /* Previous File Entry		*/
};

struct ScanList
{
	struct Scan *head; /* First Displayed File		*/
	struct Scan *tail; /* Last Displayed File		*/
	int size;		   /* # of displayed files		*/
};

/* End of File */
