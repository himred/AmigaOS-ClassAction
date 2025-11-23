/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : protos.h

	Goal : define external prototypes

	Ver  : 2.0

****************************************/

void Free_List(struct List, struct Remember *);
int Make_List(UBYTE **, struct List *, struct Remember *);
int AddList(struct List *, char *, int);
void FreeList(struct List *);
void FreeMemList(void);
int SaveMem(int);
int LoadMem(void);
void MemToList(void);
int Req(char *);
void RInfo(char *);
int Getfile(char *);
void NotifyMaster();
void InitLocale(void);
void FreeLocale(void);
int Learn(APTR, APTR[], APTR, long);
void InitRT();
void FreeRT();
