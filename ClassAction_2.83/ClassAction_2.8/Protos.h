/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : protos.h

	Goal : define functions prototypes

	Ver  : 2.00

****************************************/

void CreateAppIcon(void);
int InitAppStuff(void);
void RemIcon(void);

long DoRexxCommand(void *, struct MsgPort *, char *, char **);
void InitArexxPort(void);

int InitCx(void);
void FreeCx(void);
int HandleCx(void);

int LoadMem(void);
int LoadPrefs(void);
int ReadToolsTypes(void);

int Assigns(void);
int Dir(void);

struct Class *Getclass(char *, char *, int);

int InsertFileNode(struct Scan *);
int InsertDirNode(struct Scan *);
int CheckBufferOffset(struct Class *, char *, int);
int CheckFileOffset(struct Class *, int, int);
int IsAscii(char *);

int ParseArgs(char *, char *, char *);
int ParseReq(char *, char *, char *);

int Runner(struct Action *, char *, int);

int GetReg(char *);
int GetToolsType(char *, char *);
void BstrToStr(BSTR, char *);

int HandleApp(void);
int HandleIDCMP(char *);
void HandleAppWin(void);

int AddList(struct List *, char *, char);
void FreeList(struct List *);
void FreeMemList(void);
void MemToList(void);
void FreeScanList(void);
void InitLists(void);

void ClosePop_Window(void);
int HandleIDCMPOP(void);
int OpenPop_Window(char *, char *, int);
int PopUp(struct List *, char *, char *, int);
void PubFrontScreen(char *);

int EasyReq(struct EasyStruct *, int *, int);
int Freq(char *, int, char *);
void Info(char *);
int Req(char *);
int GetText(char *, int, char *);

void ExtractSeg(void);

void CloseDownScreen(void);
void CloseTest_Window(void);
int OpenTest_Window(int);
void PutString(int, char *);
void PutText(int, char *);
void SetDrives(void);
int SetupScreen(void);
int SetNewSize(int, char);

void CloseXfdLib(void);
APTR Decrunch(char *, int, char *, int *);
int OpenXfdLib(void);

struct Buffer *xfdAllocBufferInfo(void);

int xfdRecogBuffer(struct Buffer *);
int xfdDecrunchBuffer(struct Buffer *);
void xfdFreeBufferInfo(struct Buffer *);
long *MyAddAppIcon(void);

void InitLocale(void);
void FreeLocale(void);

void InitRT();
void FreeRT();

/* End of Prototypes */
