/****************************************

		Class Action Source

		(c) by 1994 Salim Gasmi

		File : Exec.c

		Goal : Kernel of the hard core code

		Ver  : 3.00

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

/****************************************
Function : CheckComment(fullname)
Goal     : Determine the Class of
				   the file 'fullname'
				   with its comment
				   check date validity
		Return Values :
				   pointer to the determined Class
****************************************/
struct Class *CheckComment(char *full)
{
	struct FileInfoBlock info;
	struct DateStamp dt;
	struct Class *cla;
	char ch[8];
	BPTR lock;

	lock = Lock(full, ACCESS_READ);
	if (lock == NULL)
		return (NULL);

	Examine(lock, &info);
	UnLock(lock);

	if (strlen(info.fib_Comment) <= COMMENT_L + 15)
		return (NULL);
	if (strncmp(info.fib_Comment, COMMENT, COMMENT_L) != 0)
		return (NULL);

	strncpy(ch, info.fib_Comment + COMMENT_L, 6);
	ch[6] = 0;
	dt.ds_Days = atol(ch);

	strncpy(ch, info.fib_Comment + COMMENT_L + 6, 4);
	ch[4] = 0;
	dt.ds_Minute = atol(ch);

	strncpy(ch, info.fib_Comment + COMMENT_L + 10, 4);
	ch[4] = 0;
	dt.ds_Tick = atol(ch);

	if (CompareDates(&dt, &(info.fib_Date)) > 0)
		return (NULL);

	cla = MemList.head->next;

	while (cla != NULL)
	{
		if (strcmp(info.fib_Comment + COMMENT_L + 15, cla->name) == 0)
			return (cla);
		cla = cla->next;
	}

	return (NULL);
}

/****************************************
Function : PutComment(fullname,class)
Goal     : Set the comment of the file 'fullname'
				   with the classname
				   Does not overwrite
				   Non CA comments
Return Values :
				   0 if set, 1 otherwise
****************************************/
int PutComment(char *full, struct Class *cla)
{
	struct FileInfoBlock info;
	struct DateStamp dt;
	char ch[79];
	BPTR lock;
	int l;

	if (DoComm != 1)
		return (1);

	lock = Lock(full, ACCESS_READ);
	if (lock == NULL)
		return (NULL);

	Examine(lock, &info);
	UnLock(lock);

	l = strlen(info.fib_Comment);

	if (l != 0 && l <= COMMENT_L + 15)
		return (1);
	if (l != 0 && strncmp(info.fib_Comment, COMMENT, COMMENT_L) != 0)
		return (1);

	DateStamp(&dt);

	sprintf(ch, "%s%06ld%04ld%04ld %s", COMMENT, dt.ds_Days, dt.ds_Minute, dt.ds_Tick, cla->name);
	SetComment(full, ch);
	return (0);
}

/****************************************
Function : GetClass(fullname,name)
Goal     : Determine the Class of
				   the file 'fullname'
				   with local name 'name'
				   if the file is Crunched
				   we try to decrunch it
Return Values :
				   pointer to the determined Class
****************************************/
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

	strcpy(Cruncher, "");

	/* We check with comments if comment are okay */

	if (DoComm == 1)
	{
		cla = CheckComment(full);
		if (cla != NULL)
			return (cla);
	}

	/* We search with Matchnames */

	cla = MemList.head->next;
	while (cla != NULL)
	{
		if (cla->suffix[0] != 0)
		{
			ls = MatchPatternNoCase(cla->suffix, name);
			if (ls == 1)
			{
				PutComment(full, cla);
				return (cla);
			}
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
			PutComment(full, cla);
			return (cla);
		}
		cla = cla->next;
	}
	close(han);

	/* se search with offsets and decrunching */

	if (DecrunchFlag == 1)
	{
		han = open(full, O_RDONLY);

		buff = (char *)malloc(len);
		if (buff == NULL && len != 0)
		{
			MInfo(LocaleMsg[CA_MEMORY]);
			close(han);
			return (MemList.head);
		}

		read(han, buff, len);
		close(han);

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
					PutComment(full, cla);
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
		{
			PutComment(full, cla);
			return (cla);
		}
		cla = cla->next;
	}

	/* Unknown Class */

	return (MemList.head);
}

/****************************************
Function : CheckBufferOffset
Goal     : determine if the buffer
				   'buff' has valid offsets
				   with the Class 'cl'
Return Values :
				   0 = No Valid Offsets found
				   1 = Valid Offsets found
****************************************/
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

/****************************************
Function : CheckFileOffset
Goal     : determine if the file
				   handled by 'han' of
				   length 'len' has valid
				   offsets with  Class 'cl'
Return Values :
				   0 = No Valid Offsets found
				   1 = Valid Offsets found
****************************************/
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

/****************************************
Function : ParseArgs
Goal     : interprete exec string
				   with selected file 'file'
				   and fill up 'res' with
				   the result
Return Values :
				   -2 = Unable to parse
				   -1 = Nothing to change
					0 = Parsed Succesfully
****************************************/
int ParseArgs(char *exec, char *file, char *res)
{
	char *found, *x;
	char sep[] = {'"', 0};
	char tmpe[1024];
	char tmpf[1024];
	char *suf, *mimi;
	char dir[512];

	void GetL2Dir(char *);

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

	found = strstr(tmpe, "[d]");
	if (found != NULL)
	{
		GetL2Dir(dir);
		*found = 0;
		strcpy(res, tmpe);
		strcat(res, sep);
		strcat(res, dir);
		strcat(res, sep);
		strcat(res, found + 3);
		return (0);
	}

	found = strstr(tmpe, "[D]");
	if (found != NULL)
	{
		GetL2Dir(dir);
		*found = 0;
		strcpy(res, tmpe);
		strcat(res, dir);
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

/****************************************
Function : GetToolsType
Goal     : Get the Value of the
				   Tooltype 'tool'
				   in string 'result'
Return Values :
				   1= Tooltype found
				   0= Tooltype not found
				  -1= Icon not found
****************************************/
int GetToolsType(char *tool, char *result)
{
	char *ptr;

	ptr = (char *)FindToolType(IconObj->do_ToolTypes, tool);
	if (ptr == NULL)
		return (0);

	strcpy(result, ptr);
	return (1);
}

/****************************************
Function : BstrToStr
Goal     : Convert a BCPL String into a C String
****************************************/
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

/****************************************
Function : ParseReq
Goal     : interprete exec string
				   with REQ Commands
				   and fill up 'res' with
				   the result
Return Values :
				   -2 = Unable to parse
				   -1 = Nothing to change
					0 = Parsed Succesfully
****************************************/
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
			MInfo(LocaleMsg[CA_SURE_ERROR]);
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
		MInfo(LocaleMsg[CA_REQ_ERROR]);
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

/****************************************
Function : Runner
Goal     : run the action act with filename
****************************************/
int Runner(struct Action *act, char *filename, int isgene)
{
	BPTR handler;
	char na[512];
	char ch[512];
	char oktodo;
	char sta[20];
	char rexxcmd[300];
	char path[300];
	char *p;
	char full[1024];
	int ss;
	char device[200];

	strcpy(path, filename);
	p = (char *)PathPart(path);
	*p = 0;
	p = (char *)FilePart(filename);

	strcpy(na, act->exec);

	if (act->wbrun >= 10)
		isgene = 1;

	while ((oktodo = ParseReq(na, p, ch)) == 0)
		strcpy(na, ch);
	if (oktodo == -2)
		return (0);
	else
		while (ParseArgs(na, filename, ch) == 0)
			strcpy(na, ch);

	if (act->wbrun == 0 || act->wbrun == 10)
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
		}

		return (0);
	}

	handler = Open("NIL:", MODE_OLDFILE);

	if (act->wbrun == 1 || act->wbrun == 11)
		sprintf(full, "%s %s", WBRUN, ch);

	if (act->wbrun == 2 || act->wbrun == 12)
		strcpy(full, ch);

	if (act->wbrun == 3 || act->wbrun == 13)
		sprintf(full, "sys:rexxc/rx %s", ch);

	if (isgene == 0)
		SystemTags(full, SYS_Input, handler, SYS_Output, NULL, SYS_Asynch, TRUE, TAG_DONE);
	else
	{
		SystemTags(full, SYS_Input, handler, SYS_Output, NULL, SYS_Asynch, FALSE, TAG_DONE);
	}

	return (0);
}

/****************************************
Function : IsAscii
Goal     : Determine if 'file' is an
				   ASCII file
				   if file==NULL then the datatype
				   library is closed (DataBase)
Return Values:
				   1: File is ASCII
				   0: File is not ASCII
				  -1: File not found
				  -2: Datatypes libs not found
				  -3: GetDatatype Failed
****************************************/
int IsAscii(char *file)
{
	struct DataTypeHeader *dth;
	struct DataType *dtn;
	BPTR lock;
	char *s;
	char ret;

	if (DataBase == NULL)
		return (-2);

	if (file == NULL)
	{
		CloseLibrary(DataBase);
		return (0);
	}

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