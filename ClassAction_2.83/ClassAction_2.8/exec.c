/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : Exec.c

	Goal : Kernel of the hard core code

	Ver  : 2.00

****************************************/

#include <dos/dos.h>
#include <dos/dosextens.h>
#include <dos/dostags.h>
#include <exec/types.h>
#include <exec/exec.h>
#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <intuition/classusr.h>
#include <intuition/imageclass.h>
#include <intuition/gadgetclass.h>
#include <libraries/gadtools.h>
#include <libraries/iffparse.h>
#include <graphics/displayinfo.h>
#include <graphics/gfxbase.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/gadtools_protos.h>
#include <clib/graphics_protos.h>
#include <clib/utility_protos.h>
#include <clib/datatypes_protos.h>
#include <clib/iffparse_protos.h>
#include <workbench/workbench.h>
#include <datatypes/datatypes.h>
#include <datatypes/datatypesclass.h>

#include <string.h>
#include <fcntl.h>
#include <stdio.h>

#include "gadgets.h"
#include "includes.h"

char dirformat[30] = {"x"};
char filformat[30] = {"x"};
char Quote = 39;

void My_Free(char *buf, int len, int cr)
{
	if (cr == 1)
		FreeMem(buf, len);
	else
		free(buf);
}

/****************************************/
/*										*/
/* Function : GetClass(fullname,name)	*/
/*										*/
/* Goal     : Determine the Class of	*/
/*	      the file 'fullname'			*/
/*	      with local name 'name'		*/
/*	      if the file is Crunched		*/
/*	      we try to decrunch it			*/
/*										*/
/* Return Values : 						*/
/*										*/
/*  pointer to the determined Class		*/
/*										*/
/****************************************/

struct Class *Getclass(char *full, char *name, int affi)
{
	struct FileInfoBlock info;
	BPTR lock;
	struct Class *cla;
	char tmp[1024];
	int han;
	APTR dec;
	int len, newlen;
	char *buff;
	int cr;
	int ls;

	if (affi == 1)
		PutString(GD_Proj_File, name);
	strcpy(Cruncher, "");

	/* We search with Matchnames */

	cla = MemList.head->next;
	while (cla != NULL)
	{
		if (cla->suffix[0] != 0)
		{
			ls = MatchPatternNoCase(cla->suffix, name);
			if (ls == 1)
				return (cla);
		}
		cla = cla->next;
	}

	/* We get the file size */

	han = open(full, O_RDONLY);
	lock = Lock(full, ACCESS_READ);
	Examine(lock, &info);
	len = info.fib_Size;
	UnLock(lock);

	/* we search with offsets */

	cla = MemList.head->next;
	while (cla != NULL)
	{
		if (CheckFileOffset(cla, han, len) == 1)
		{
			close(han);
			return (cla);
		}
		cla = cla->next;
	}
	close(han);

	/* se search with offsets and decrunching */

	if (DecrunchFlag == 1)
	{
		if (affi == 1)
			PutString(GD_Proj_Class, LocaleMsg[CA_LOADING]);
		han = open(full, O_RDONLY);

		buff = (char *)malloc(len);
		if (buff == NULL)
		{
			Info(LocaleMsg[CA_MEMORY]);
			close(han);
			return (MemList.head);
		}

		read(han, buff, len);
		close(han);

		if (affi == 1)
			PutString(GD_Proj_Class, LocaleMsg[CA_DECRUNCHING]);
		dec = Decrunch(buff, len, tmp, &newlen);
		if (dec != NULL)
		{
			strcpy(Cruncher, tmp);
			free(buff);
			buff = dec;
			len = newlen;
			cr = 1;

			cla = MemList.head->next;
			while (cla != NULL)
			{
				if (CheckBufferOffset(cla, buff, len) == 1)
				{
					My_Free(buff, len, cr);
					return (cla);
				}
				cla = cla->next;
			}
		}
		else
		{
			cr = 0;
			strcpy(Cruncher, LocaleMsg[CA_UNKNOWN]);
		}

		My_Free(buff, len, cr);
	}

	/* we test if it's an ASCII file */

	cla = MemList.head->next;
	while (cla != NULL)
	{
		if (strcmp(cla->value[0], "ASCII[]") == 0 && IsAscii(full) == 1)
			return (cla);
		cla = cla->next;
	}

	/* Unknown Class */

	return (MemList.head);
}

/****************************************/
/*										*/
/* Function : CheckBufferOffset			*/
/*										*/
/* Goal     : determine if the buffer	*/
/*            'buff' has valid offsets  */
/*	      with the Class 'cl'			*/
/*										*/
/*										*/
/* Return Values : 						*/
/*										*/
/* 0 = No Valid Offsets found			*/
/* 1 = Valid Offsets found				*/
/*										*/
/****************************************/

int CheckBufferOffset(struct Class *cl, char *buff, int buflen)
{
	int good;
	int i, j;
	char tmp[1024], val[1024];
	char hex[3];
	int off;
	unsigned char c;
	char *x;
	char vax[200];
	char asc[200];

	good = 0;

	for (i = 0; i < 5; i++)
	{
		strcpy(val, "");
		strcpy(tmp, cl->value[i]);
		x = stpchr(tmp, ',');
		if (x != NULL)
		{
			*x = 0;
			off = atoi(tmp);
			strcpy(vax, x + 1);

			if (buflen < off)
				return (0);

			if (vax[0] == '"')
			{
				strcpy(asc, vax + 1);
				asc[strlen(asc) - 1] = 0;
				if (strnicmp(asc, buff + off, strlen(asc)) != 0)
					return (0);
				else
					good++;
			}

			if (vax[0] == Quote)
			{
				strcpy(asc, vax + 1);
				asc[strlen(asc) - 1] = 0;
				if (strncmp(asc, buff + off, strlen(asc)) != 0)
					return (0);
				else
					good++;
			}

			if (vax[0] != '"' && vax[0] != Quote)
			{
				for (j = 0; j < strlen(vax) / 2; j = j + 1)
				{
					c = buff[j + off];
					sprintf(hex, "%02x", c);
					strcat(val, hex);
				}
				if (stricmp(val, vax) != 0)
					return (0);
				else
					good++;
			}
		}
	}
	if (good == 0)
		return (0);
	else
		return (1);
}

/************************************/
/*									*/
/* Function : CheckFileOffset		*/
/*									*/
/* Goal     : determine if the file	*/
/*            handled by 'han' of 	*/
/*	      length 'len' has valid	*/
/*	      offsets with  Class 'cl'	*/
/*									*/
/* Return Values : 					*/
/*									*/
/* 0 = No Valid Offsets found		*/
/* 1 = Valid Offsets found			*/
/*									*/
/************************************/

int CheckFileOffset(struct Class *cl, int han, int len)
{
	int good;
	int i, j;
	char tmp[1024], val[1024];
	char hex[3];
	int off;
	unsigned char c;
	char *x;
	char vax[200];
	char asc[200];

	good = 0;

	for (i = 0; i < 5; i++)
	{
		strcpy(val, "");
		strcpy(tmp, cl->value[i]);
		x = stpchr(tmp, ',');
		if (x != NULL)
		{
			*x = 0;
			off = atoi(tmp);
			strcpy(vax, x + 1);

			if (len < off)
				return (0);

			lseek(han, 0, 0);
			lseek(han, off, 0);

			if (vax[0] == '"')
			{
				strcpy(asc, vax + 1);
				asc[strlen(asc) - 1] = 0;
				read(han, tmp, strlen(asc));
				if (strnicmp(asc, tmp, strlen(asc)) != 0)
					return (0);
				else
					good++;
			}

			if (vax[0] == Quote)
			{
				strcpy(asc, vax + 1);
				asc[strlen(asc) - 1] = 0;
				read(han, tmp, strlen(asc));
				if (strncmp(asc, tmp, strlen(asc)) != 0)
					return (0);
				else
					good++;
			}

			if (vax[0] != '"' && vax[0] != Quote)
			{
				read(han, tmp, strlen(vax) / 2);
				for (j = 0; j < strlen(vax) / 2; j = j + 1)
				{
					c = tmp[j];
					sprintf(hex, "%02x", c);
					strcat(val, hex);
				}

				if (stricmp(val, vax) != 0)
					return (0);
				else
					good++;
			}
		}
	}
	if (good == 0)
		return (0);
	else
		return (1);
}

/****************************************/
/*										*/
/* Function : ParseArgs					*/
/*										*/
/* Goal     : interprete exec string	*/
/*            with selected file 'file' */
/*	      and fill up 'res' with		*/
/*	      the result					*/
/*										*/
/* Return Values : 						*/
/*										*/
/* -2 = Unable to parse 				*/
/* -1 = Nothing to change				*/
/*  0 = Parsed Succesfully				*/
/*										*/
/****************************************/

int ParseArgs(char *exec, char *file, char *res)
{
	char *found, *x;
	char sep[] = {'"', 0};
	char tmpe[1024];
	char tmpf[1024];
	char *suf, *mimi;

	strcpy(tmpe, exec);

	found = strstr(tmpe, "[f]");
	if (found != NULL)
	{
		*found = 0;
		strcpy(res, tmpe);
		strcat(res, sep);
		strcat(res, file);
		strcat(res, sep);
		strcat(res, found + 3);
		return (0);
	}

	found = strstr(tmpe, "[F]");
	if (found != NULL)
	{
		*found = 0;
		strcpy(res, tmpe);
		strcat(res, file);
		strcat(res, found + 3);
		return (0);
	}

	found = strstr(tmpe, "[b]");
	if (found != NULL)
	{
		*found = 0;
		strcpy(res, tmpe);
		strcat(res, sep);
		strcat(res, (char *)FilePart(file));
		strcat(res, sep);
		strcat(res, found + 3);
		return (0);
	}

	found = strstr(tmpe, "[B]");
	if (found != NULL)
	{
		*found = 0;
		strcpy(res, tmpe);
		strcat(res, (char *)FilePart(file));
		strcat(res, found + 3);
		return (0);
	}

	strcpy(tmpe, exec);
	strcpy(tmpf, file);

	found = strstr(tmpe, "[s]");
	if (found != NULL)
	{
		x = stpchr(tmpf, '.');
		if (x != NULL)
		{
			suf = x + 1;
			while ((mimi = stpchr(suf, '.')) != NULL)
				suf = mimi + 1;
			*(suf - 1) = 0;
		}

		*found = 0;
		strcpy(res, tmpe);
		strcat(res, sep);
		strcat(res, tmpf);
		strcat(res, sep);
		strcat(res, found + 3);
		return (0);
	}

	found = strstr(tmpe, "[S]");
	if (found != NULL)
	{
		x = stpchr(tmpf, '.');
		if (x != NULL)
		{
			suf = x + 1;
			while ((mimi = stpchr(suf, '.')) != NULL)
				suf = mimi + 1;
			*(suf - 1) = 0;
		}

		*found = 0;
		strcpy(res, tmpe);
		strcat(res, tmpf);
		strcat(res, found + 3);
		return (0);
	}

	found = strstr(tmpe, "[x]");
	if (found != NULL)
	{
		x = stpchr(tmpf, '.');
		if (x != NULL)
		{
			suf = x + 1;
			while ((mimi = stpchr(suf, '.')) != NULL)
				suf = mimi + 1;
			*(suf - 1) = 0;
		}

		*found = 0;
		strcpy(res, tmpe);
		strcat(res, sep);
		strcat(res, (char *)FilePart(tmpf));
		strcat(res, sep);
		strcat(res, found + 3);
		return (0);
	}

	found = strstr(tmpe, "[X]");
	if (found != NULL)
	{
		x = stpchr(tmpf, '.');
		if (x != NULL)
		{
			suf = x + 1;
			while ((mimi = stpchr(suf, '.')) != NULL)
				suf = mimi + 1;
			*(suf - 1) = 0;
		}

		*found = 0;
		strcpy(res, tmpe);
		strcat(res, (char *)FilePart(tmpf));
		strcat(res, found + 3);
		return (0);
	}

	strcpy(res, exec);
	return (-1);
}

/****************************************/
/*										*/
/* Function : GetToolsType				*/
/*										*/
/* Goal     : Get the Value of the		*/
/*	      Tooltype 'tool'				*/
/*	      in string 'result'			*/
/*										*/
/*										*/
/* Return Values : 						*/
/*										*/
/*	1= Tooltype found					*/
/*	0= Tooltype not found				*/
/*     -1= Icon not found				*/
/*										*/
/****************************************/

int GetToolsType(char *tool, char *result)
{
	char tmp[255];
	char chemin[256];
	BPTR lock;
	char *ptr;
	static struct DiskObject *icon = NULL;

	if (tool == NULL && icon != NULL)
	{
		FreeDiskObject(icon);
		return (-1);
	}

	if (icon == NULL)
	{
		lock = GetProgramDir();
		NameFromLock(lock, tmp, 100);

		if (tmp[strlen(tmp) - 1] == ':')
			sprintf(chemin, "%s%s", tmp, ProgramName);
		else
			sprintf(chemin, "%s/%s", tmp, ProgramName);

		icon = (struct DiskObject *)GetDiskObject(chemin);
		if (icon == NULL)
			return (-1);
	}

	ptr = (char *)FindToolType(icon->do_ToolTypes, tool);
	if (ptr == NULL)
		return (0);

	strcpy(result, ptr);
	return (1);
}

/****************************************/
/*										*/
/* Function : Dir						*/
/*										*/
/* Goal     : Really High Optimized		*/
/*	      Fast File Requester			*/
/*	      Just display the dir			*/
/*	      of the lock 'DirLock'			*/
/*										*/
/****************************************/

int Dir(void)
{

	struct FileInfoBlock info;
	struct Scan *sc;
	char *ptr;
	int len;
	char tmp[100];

	if (dirformat[0] == 'x')
	{
		sprintf(dirformat, "%%-%ds %%8s", MaxFile);
		sprintf(filformat, "%%-%ds %%8d", MaxFile);
	}

	FreeList(&Flist);
	PutList(GD_Proj_Req, &Flist);
	FreeScanList();
	Filelist.size = 0;
	Filelist.head = NULL;
	Dirlist.size = 0;
	Dirlist.head = NULL;

	AssignFlag = 0;
	if (ParentDir(DirLock) == 0)
		ParentFlag = 0;
	else
	{
		AddList(&Flist, "<Parent>", 1);
		ParentFlag = 1;
	}

	Examine(DirLock, &info);

	PutString(GD_Proj_File, LocaleMsg[CA_READING]);
	PutString(GD_Proj_Class, "");

	while (ExNext(DirLock, &info) == -1)
	{
		len = strlen(info.fib_FileName);
		if (len < 5)
			ptr = NULL;
		else
			ptr = info.fib_FileName + len - 5;
		if (stricmp(ptr, ".info") != 0)
		{
			sc = (struct Scan *)malloc(sizeof(struct Scan));
			strcpy(sc->name, info.fib_FileName);

			if (info.fib_DirEntryType < 0)
			{
				if (len > MaxFile - 1)
				{
					info.fib_FileName[MaxFile - 1] = 0;
				}
				sprintf(sc->tmp, filformat, info.fib_FileName, info.fib_Size);

				if (Filelist.head == NULL)
				{
					Filelist.head = Filelist.tail = sc;
					sc->prec = sc->next = NULL;
				}
				else
					InsertFileNode(sc);
				Filelist.size++;
			}
			else
			{
				if (len > MaxDir - 1)
				{
					info.fib_FileName[MaxDir - 1] = 0;
				}

				sprintf(tmp, "%s", info.fib_FileName);
				sprintf(sc->tmp, dirformat, tmp, "Drawer");

				if (Dirlist.head == NULL)
				{
					Dirlist.head = Dirlist.tail = sc;
					sc->prec = sc->next = NULL;
				}
				else
					InsertDirNode(sc);
				Dirlist.size++;
			}
		}
	}

	sc = Dirlist.head;
	while (sc != NULL)
	{
		AddList(&Flist, sc->tmp, 1);
		sc = sc->next;
	}

	sc = Filelist.head;
	while (sc != NULL)
	{
		AddList(&Flist, sc->tmp, 0);
		sc = sc->next;
	}

	PutList(GD_Proj_Req, &Flist);
	PutString(GD_Proj_File, "");
	return (0);
}

/****************************************/
/*										*/
/* Function : Assigns					*/
/*										*/
/* Goal     : Really High Optimized		*/
/*	      Fast Assign Requester			*/
/*										*/
/****************************************/

int Assigns(void)
{

	char path[100];
	struct Scan *sc;
	char tmp[100];

	FreeList(&Flist);
	PutList(GD_Proj_Req, &Flist);
	FreeScanList();
	Filelist.size = 0;
	Filelist.head = NULL;
	Dirlist.size = 0;
	Dirlist.head = NULL;

	DirLock = 0;
	AssignFlag = 1;
	ParentFlag = 0;

	PutString(GD_Proj_File, LocaleMsg[CA_READING]);
	PutString(GD_Proj_Class, "");

	Doslist = (struct DosList *)LockDosList(LDF_VOLUMES | LDF_ASSIGNS | LDF_READ);
	while ((Doslist = (struct DosList *)NextDosEntry(Doslist, LDF_VOLUMES | LDF_ASSIGNS | LDF_READ)) != NULL)
	{
		sc = (struct Scan *)malloc(sizeof(struct Scan));
		BstrToStr(Doslist->dol_Name, path);
		sprintf(tmp, "%s:", path);
		strcpy(sc->name, tmp);

		if (strlen(tmp) > MaxFile - 1)
			tmp[MaxFile - 1] = 0;

		if (Doslist->dol_Type == 2)
		{
			sprintf(sc->tmp, dirformat, tmp, "VOLUME");
			if (Dirlist.head == NULL)
			{
				Dirlist.head = Dirlist.tail = sc;
				sc->prec = sc->next = NULL;
			}
			else
				InsertDirNode(sc);
			Dirlist.size++;
		}

		if (Doslist->dol_Type == 1)
		{
			sprintf(sc->tmp, dirformat, tmp, "ASSIGN");
			if (Filelist.head == NULL)
			{
				Filelist.head = Filelist.tail = sc;
				sc->prec = sc->next = NULL;
			}
			else
				InsertFileNode(sc);
			Filelist.size++;
		}
	}

	UnLockDosList(LDF_VOLUMES | LDF_ASSIGNS | LDF_READ);

	sc = Dirlist.head;
	while (sc != NULL)
	{
		AddList(&Flist, sc->tmp, 1);
		sc = sc->next;
	}

	sc = Filelist.head;
	while (sc != NULL)
	{
		AddList(&Flist, sc->tmp, 0);
		sc = sc->next;
	}

	PutList(GD_Proj_Req, &Flist);
	PutString(GD_Proj_File, "");
	return (0);
}

/****************************************/
/*										*/
/* Function : InsertFile(Dir)Node		*/
/*										*/
/* Goal     : Insert a node in a double	*/
/*	      linked list with real			*/
/*	      time sorting					*/
/*										*/
/*										*/
/* Return Values : 						*/
/*										*/
/* Always 0								*/
/*										*/
/****************************************/

int InsertFileNode(struct Scan *x)
{
	struct Scan *ptr;

	if (stricmp(Filelist.head->name, x->name) > 0)
	{
		ptr = Filelist.head;
		ptr->prec = x;
		Filelist.head = x;
		x->next = ptr;
		x->prec = NULL;

		return (0);
	}

	if (stricmp(Filelist.tail->name, x->name) <= 0)
	{
		ptr = Filelist.tail;
		x->prec = ptr;
		x->next = NULL;
		Filelist.tail = x;
		ptr->next = x;
		return (0);
	}

	ptr = Filelist.head;

	while (ptr != NULL)
	{
		if (stricmp(ptr->name, x->name) > 0)
		{
			x->next = ptr;
			x->prec = ptr->prec;
			ptr->prec->next = x;
			ptr->prec = x;
			return (0);
		}
		else
			ptr = ptr->next;
	}

	return (0);
}

int InsertDirNode(struct Scan *x)
{
	struct Scan *ptr;

	if (stricmp(Dirlist.head->name, x->name) > 0)
	{
		ptr = Dirlist.head;
		ptr->prec = x;
		Dirlist.head = x;
		x->next = ptr;
		x->prec = NULL;

		return (0);
	}

	if (stricmp(Dirlist.tail->name, x->name) <= 0)
	{
		ptr = Dirlist.tail;
		x->prec = ptr;
		x->next = NULL;
		Dirlist.tail = x;
		ptr->next = x;
		return (0);
	}

	ptr = Dirlist.head;

	while (ptr != NULL)
	{
		if (stricmp(ptr->name, x->name) > 0)
		{
			x->next = ptr;
			x->prec = ptr->prec;
			ptr->prec->next = x;
			ptr->prec = x;
			return (0);
		}
		else
			ptr = ptr->next;
	}

	return (0);
}

/****************************************/
/*										*/
/* Function : BstrToStr					*/
/*										*/
/* Goal     : Convert a BCPL String		*/
/*	      into a C String				*/
/*										*/
/****************************************/

void BstrToStr(BSTR bs, char *r)
{
	ULONG *x;
	char *c;
	int i;

	x = (ULONG *)bs;
	x = x << 2;
	c = (char *)x;

	for (i = 1; i <= c[0]; i++)
		r[i - 1] = c[i];
	r[c[0]] = 0;
}

/****************************************/
/*										*/
/* Function : ParseReq					*/
/*										*/
/* Goal     : interprete exec string	*/
/*            with REQ Commands         */
/*	      and fill up 'res' with		*/
/*	      the result					*/
/*										*/
/* Return Values : 						*/
/*										*/
/* -2 = Unable to parse 				*/
/* -1 = Nothing to change				*/
/*  0 = Parsed Succesfully				*/
/*										*/
/****************************************/

int ParseReq(char *exec, char *fname, char *res)
{
	char tmp[1024];
	char *found, *end;
	int l;
	int mode = -1;
	char text[100];
	char file[256];

	strcpy(res, exec);
	strcpy(tmp, exec);

	found = strstr(tmp, "SURE[");
	if (found != NULL)
	{
		end = strstr(found, "]");
		if (end == NULL)
		{
			Info(LocaleMsg[CA_SURE_ERROR]);
			return (-2);
		}

		strcpy(text, found + 5);
		text[end - found - 5] = 0;
		mode = Req(text);
		if (mode != 1)
			return (-2);
		strcpy(res, end + 1);
		return (0);
	}

	found = strstr(tmp, "REQD[");
	if (found != NULL)
		mode = 1;

	if (mode == -1)
	{
		found = strstr(tmp, "REQF[");
		if (found != NULL)
			mode = 0;
	}

	if (mode == -1)
	{
		found = strstr(tmp, "REQV[");
		if (found != NULL)
			mode = 2;
	}

	if (mode == -1)
	{
		found = strstr(tmp, "REQT[");
		if (found != NULL)
			mode = 3;
	}

	if (mode == -1)
		return (-1);

	end = strstr(found, "]");
	if (end == NULL)
	{
		Info(LocaleMsg[CA_REQ_ERROR]);
		return (-2);
	}

	strcpy(text, found + 5);
	text[end - found - 5] = 0;

	if (mode < 3)
	{
		strcpy(file, fname);
		l = Freq(file, mode, text);
		if (l < 0)
			return (-2);
	}

	if (mode == 3)
	{
		strcpy(file, "");
		l = GetText(file, 255, text);
		if (l < 0)
			return (-2);
	}

	*found = 0;

	sprintf(res, "%s%s", tmp, file);
	if (end[1] != 0)
		strcat(res, end + 1);
	return (0);
}

/****************************************/
/*										*/
/* Function : Runner					*/
/*										*/
/* Goal     : run the action act		*/
/*            with filename	        	*/
/*										*/
/****************************************/

int Runner(struct Action *act, char *filename, int isgene)
{
	BPTR handler;
	char na[512];
	char ch[512];
	char oktodo;
	char sta[20];
	char rexxcmd[300];
	char path[300];
	char *p, *ptr, *ptr2, *ptr3;
	char full[1024];
	int ss, out;
	char device[200];

	strcpy(path, filename);
	p = (char *)PathPart(path);
	*p = 0;
	p = (char *)FilePart(filename);

	strcpy(na, act->exec);

	if (isgene == -1)
	{
		out = 0;
		while (out == 0)
		{
			ptr = strstr(na, "REQD[");
			if (ptr != NULL)
				*(ptr + 3) = 'T';

			ptr2 = strstr(na, "REQF[");
			if (ptr2 != NULL)
				*(ptr2 + 3) = 'T';

			ptr3 = strstr(na, "REQV[");
			if (ptr3 != NULL)
				*(ptr3 + 3) = 'T';

			if ((ptr == NULL) && (ptr2 == NULL) && (ptr3 == NULL))
				out = 1;
		}

		isgene = 0;
	}

	while ((oktodo = ParseReq(na, p, ch)) == 0)
		strcpy(na, ch);
	if (oktodo == -2)
		return (0);
	else
		while (ParseArgs(na, filename, ch) == 0)
			strcpy(na, ch);

	if (act->wbrun == 0)
	{
		ss = atoi(act->delay);
		if (ss < 0)
			sprintf(device, "%s/AUTO/WAIT/CLOSE", DevString);
		else
			sprintf(device, "%s/AUTO", DevString);

		handler = Open(device, MODE_OLDFILE);
		sprintf(sta, "stack %s", act->stack);
		sprintf(rexxcmd, "cd %c%s%c", '"', path, '"');
		if (ss > 0)
			sprintf(na, "wait %d", ss);
		else
			strcpy(na, ";");
		sprintf(full, "%s%c%s%c%s%c%s%c", sta, 10, rexxcmd, 10, ch, 10, na, 10);

		if (isgene == 0)
			SystemTags(full, SYS_Input, handler, SYS_Output, NULL, SYS_Asynch, TRUE, TAG_DONE);
		else
		{
			SystemTags(full, SYS_Input, handler, SYS_Output, NULL, SYS_Asynch, FALSE, TAG_DONE);
			Dir();
		}

		return (0);
	}

	handler = Open("NIL:", MODE_OLDFILE);

	if (act->wbrun == 1)
		sprintf(full, "%s %s", WBRUN, ch);

	if (act->wbrun == 2)
		strcpy(full, ch);

	if (act->wbrun == 3)
		sprintf(full, "sys:rexxc/rx %s", ch);

	if (isgene == 0)
		SystemTags(full, SYS_Input, handler, SYS_Output, NULL, SYS_Asynch, TRUE, TAG_DONE);
	else
	{
		SystemTags(full, SYS_Input, handler, SYS_Output, NULL, SYS_Asynch, FALSE, TAG_DONE);
		Dir();
	}

	return (0);
}

/****************************************/
/* Function : IsAscii					*/
/*										*/
/* Goal     : Determine if 'file' is an	*/
/*            ASCII file	        	*/
/*										*/
/* if file==NULL then the datatype		*/
/* library is closed (DataBase)			*/
/*										*/
/* Return Values: 						*/
/*										*/
/*	1: File is ASCII					*/
/*	0: File is not ASCII				*/
/*     -1: File not found				*/
/*     -2: Datatypes libs not found		*/
/*     -3: GetDatatype Failed			*/
/*										*/
/****************************************/

int IsAscii(char *file)
{
	struct DataTypeHeader *dth;
	struct DataType *dtn;
	BPTR lock;
	char *s;
	char ret;

	if (file == NULL)
	{
		CloseLibrary(DataBase);
		return (0);
	}

	if (DataBase == NULL)
		return (-2);

	lock = Lock(file, ACCESS_READ);
	if (lock == NULL)
		return (-1);

	dtn = ObtainDataTypeA(DTST_FILE, (APTR)lock, NULL);
	if (dtn == NULL)
	{
		UnLock(lock);
		return (-3);
	}

	dth = dtn->dtn_Header;

	s = GetDTString((dth->dth_Flags & DTF_TYPE_MASK) + DTMSG_TYPE_OFFSET);
	for (ret = 0; ret < strlen(s); ret++)
		s[ret] = tolower(s[ret]);
	if (strstr(s, "ascii") != NULL)
		ret = 1;
	else
		ret = 0;

	ReleaseDataType(dtn);
	UnLock(lock);
	return (ret);
}
