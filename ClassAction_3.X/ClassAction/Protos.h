/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : protos.h

	Goal : define functions prototypes

	Ver  : 3.00

****************************************/

int LoadMem(void);
int LoadPrefs(void);
int ReadToolsTypes(void);

struct Class *Getclass(char *, char *, int);

int CheckBufferOffset(struct Class *, char *, int);
int CheckFileOffset(struct Class *, int, int);
int IsAscii(char *);

int ParseArgs(char *, char *, char *);
int ParseReq(char *, char *, char *);

int Runner(struct Action *, char *, int);

int GetReg(char *);
int GetToolsType(char *, char *);
void BstrToStr(BSTR, char *);

int AddList(struct List *, char *, char);
void FreeList(struct List *);
void FreeMemList(void);
void MemToList(void);
void InitLists(void);

int EasyReq(struct EasyStruct *, int *, int);
int Freq(char *, int, char *);
void MInfo(char *);
int Req(char *);

void ExtractSeg(void);

void SetDrives(void);

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

int DiskCmp(const char *file1, const char *file2);

/* End of Prototypes */
