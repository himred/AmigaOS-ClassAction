/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : protos.h

	Goal : define external prototypes

	Ver  : 2.0

****************************************/

int SetupScreen(void);
void CloseDownScreen(void);
int OpenTest_Window(void);
void CloseTest_Window(void);
void Free_List(struct List, struct Remember *);
int Make_List(UBYTE **, struct List *, struct Remember *);
void PutString(int, char *);
void PutCycle(int, int);
void PutList(int, struct List *);
void InserText(int, char *, int);
int AddList(struct List *, char *, int);
void FreeList(struct List *);
void FreeMemList(void);
int SaveMem(int);
int LoadMem(void);
void MemToList(void);
int Req(char *);
void Info(char *);
int Getfile(char *);
void Status(int, int);
void DisableClass(int);
void DisableAction(int);
void PopUp(char *);
void CopyOn(void);
void CopyOff(void);
void NotifyMaster();
void Select(int, int);
void Visible(int, int);
void InitLocale(void);
void FreeLocale(void);
int Learn(struct Class *);
void InitRT();
void FreeRT();
