/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : Disk.c

	Goal : Load/Save Functions

	Ver  : 3.00

****************************************/

#include <fcntl.h>
#include <exec/types.h>
#include <exec/exec.h>
#include <string.h>
#include <stdio.h>

#include "includes.h"

FILE *f;

/****************************************
	Function : Re(string)

	Goal     : Read next string from
		   File Handler f and check
		   all errors

	Return Values : NONE
****************************************/
void Re(char *s)
{
	static int fuck = 0;

	if (fgets(s, 256, f) == NULL)
	{
		strcpy(s, "???");
		if (fuck == 0)
		{
			fuck = 1;
			MInfo(LocaleMsg[CA_CONFIG_CORRUPT]);
		}
	}
	else
		s[strlen(s) - 1] = 0;
}

/****************************************
	Function : LoadMem()

	Goal     : Load the Config File
		   Into Memory and build up
		   all the needed lists

	Return Values :
	 0 = Everything is OK
	-1 = No Config File Found
	-2 = Not a valid File
****************************************/
int LoadMem(void)
{
	struct Class *cla;
	struct Action *act;
	char ch[256];
	int i;
	int out;
	char tmp[20];
	char suf[100];
	char pat[102];
	char *ppp;

	/* Allocating the Unknown Class */

	cla = (struct Class *)malloc(sizeof(struct Class));

	cla->next = NULL;
	cla->action = cla->actiontail = NULL;
	cla->nbaction = 0;
	cla->suffix[0] = NULL;
	for (i = 0; i < 5; i++)
		cla->value[i][0] = NULL;

	strcpy(cla->name, UNKNOWN);

	MemList.head = MemList.tail = cla;
	MemList.size++;

	/* End of Allocation */

	f = fopen(SFILET, "r");
	if (f == NULL)
		f = fopen(SFILE, "r");
	if (f == NULL)
		return (-1);

	Re(ch);
	if (strcmp(ch, SHEADER) != 0)
	{
		MInfo(LocaleMsg[CA_CONFIG_NOT_VALID]);
		fclose(f);
		return (-2);
	}

	Re(cla->name);

	/* reading Unknown Actions */

	out = 0;
	while (out == 0)
	{
		Re(ch);
		if (strcmp(ch, EOC) != 0)
		{
			act = (struct Action *)malloc(sizeof(struct Action));
			act->next = NULL;

			if (cla->action == NULL)
				cla->action = cla->actiontail = act;
			else
			{
				cla->actiontail->next = act;
				cla->actiontail = act;
			}
			cla->nbaction++;
			strcpy(act->name, ch);
			Re(act->exec);
			Re(tmp);
			act->wbrun = atoi(tmp);
			Re(tmp);
			ppp = strstr(tmp, ",");
			if (ppp == NULL)
			{
				strcpy(act->stack, tmp);
				strcpy(act->delay, "-1");
			}
			else
			{
				*ppp = 0;
				strcpy(act->stack, tmp);
				strcpy(act->delay, ppp + 1);
			}
		}
		else
			out = 1;
	}

	/* Starting reading File */

	while (1)
	{
		Re(ch);
		if (strcmp(ch, EOSF) == 0)
		{
			fclose(f);
			return (0);
		}
		cla = (struct Class *)malloc(sizeof(struct Class));

		cla->next = NULL;
		cla->action = cla->actiontail = NULL;
		cla->nbaction = 0;

		MemList.tail->next = cla;
		MemList.tail = cla;
		MemList.size++;
		strcpy(cla->name, ch);

		Re(suf);
		if (suf[0] != 0)
		{
			sprintf(pat, "(%s)", suf);
			ParsePatternNoCase(pat, cla->suffix, 208);
		}
		else
			cla->suffix[0] = 0;

		for (i = 0; i < 5; i++)
			Re(cla->value[i]);
		out = 0;
		while (out == 0)
		{
			Re(ch);
			if (strcmp(ch, EOC) != 0)
			{
				act = (struct Action *)malloc(sizeof(struct Action));
				act->next = NULL;

				if (cla->action == NULL)
					cla->action = cla->actiontail = act;
				else
				{
					cla->actiontail->next = act;
					cla->actiontail = act;
				}
				cla->nbaction++;
				strcpy(act->name, ch);
				Re(act->exec);
				Re(tmp);
				act->wbrun = atoi(tmp);
				Re(tmp);
				ppp = strstr(tmp, ",");
				if (ppp == NULL)
				{
					strcpy(act->stack, tmp);
					strcpy(act->delay, "-1");
				}
				else
				{
					*ppp = 0;
					strcpy(act->stack, tmp);
					strcpy(act->delay, ppp + 1);
				}
			}
			else
				out = 1;
		}
	}

	return (0);
}

/****************************************
	Function : LoadGen()

	Goal     : Load the Generic File
		   Into Memory and build up
		   all the needed lists

	Return Values :
	 0 = Everything is OK
	-1 = No Config File Found
	-2 = Not a valid File
****************************************/
int LoadGen()
{

	struct Action *act;
	struct Class *cla;
	char ch[256];
	char tmp[20];
	int i, out = 0;
	char *ppp;

	Generic.nbaction = 0;
	Generic.actiontail = Generic.action = NULL;
	for (i = 0; i < 5; i++)
		Generic.value[i][0] = NULL;
	Generic.suffix[0] = NULL;

	cla = &Generic;

	f = fopen(GFILET, "r");
	if (f == NULL)
		f = fopen(GFILE, "r");
	if (f == NULL)
		return (-1);

	Re(ch);
	if (strcmp(ch, GHEADER) != 0)
	{
		return (-1);
	}

	while (out == 0)
	{
		Re(ch);
		if (strcmp(ch, EOC) != 0)
		{
			act = (struct Action *)malloc(sizeof(struct Action));
			act->next = NULL;

			if (cla->action == NULL)
				cla->action = cla->actiontail = act;
			else
			{
				cla->actiontail->next = act;
				cla->actiontail = act;
			}
			cla->nbaction++;
			strcpy(act->name, ch);
			Re(act->exec);
			Re(tmp);
			act->wbrun = atoi(tmp);
			Re(tmp);
			ppp = strstr(tmp, ",");
			if (ppp == NULL)
			{
				strcpy(act->stack, tmp);
				strcpy(act->delay, "-1");
			}
			else
			{
				*ppp = 0;
				strcpy(act->stack, tmp);
				strcpy(act->delay, ppp + 1);
			}
		}
		else
			out = 1;
	}
	fclose(f);

	/* Reading Dir File */

	strcpy(Directory.name, "Directory");
	Directory.nbaction = 0;
	Directory.actiontail = Directory.action = NULL;
	for (i = 0; i < 5; i++)
		Directory.value[i][0] = NULL;
	Directory.suffix[0] = NULL;

	cla = &Directory;

	f = fopen(DFILET, "r");
	if (f == NULL)
		f = fopen(DFILE, "r");
	if (f == NULL)
		return (-1);

	Re(ch);
	if (strcmp(ch, DHEADER) != 0)
	{
		Info("Dir Prefs not valid");
		return (-1);
	}
	out = 0;

	while (out == 0)
	{
		Re(ch);
		if (strcmp(ch, EOC) != 0)
		{
			act = (struct Action *)malloc(sizeof(struct Action));
			act->next = NULL;

			if (cla->action == NULL)
				cla->action = cla->actiontail = act;
			else
			{
				cla->actiontail->next = act;
				cla->actiontail = act;
			}
			cla->nbaction++;
			strcpy(act->name, ch);
			Re(act->exec);
			Re(tmp);
			act->wbrun = atoi(tmp);
			Re(tmp);
			ppp = strstr(tmp, ",");
			if (ppp == NULL)
			{
				strcpy(act->stack, tmp);
				strcpy(act->delay, "-1");
			}
			else
			{
				*ppp = 0;
				strcpy(act->stack, tmp);
				strcpy(act->delay, ppp + 1);
			}
		}
		else
			out = 1;
	}

	fclose(f);
	return (0);
}

/****************************************
	Function : ReadToolsTypes

	Goal     : Get The ToolsTypes
		   and setup global flags

	Return Values : Always 0
****************************************/
int ReadToolsTypes(void)
{
	int i;
	char tmp[1024];
	char tool[100];
	char *p;

	for (i = 1; i <= MXDRV; i++)
	{
		sprintf(tmp, "DRIVE%d", i);
		if (GetToolsType(tmp, tool) == 1)
		{
			p = strstr(tool, ",");
			if (p == NULL || *(p + 1) == 0)
			{
				strcpy(Drives[i - 1], tool);
				strcpy(ButPath[i - 1], tool);
			}
			else
			{
				*p = 0;
				p++;
				strcpy(Drives[i - 1], tool);
				strcpy(ButPath[i - 1], p);
			}
		}

		else
		{
			Drives[i - 1][0] = 0;
			i = MXDRV + 10;
		}
	}

	strcpy(tool, "NO");
	if (GetToolsType("DECRUNCH", tool) != 1)
		DecrunchFlag = 0;

	if (stricmp(tool, "YES") == 0)
	{
		if (OpenXfdLib() == -1)
		{
			MInfo(LocaleMsg[CA_XFD_FAILED]);
			DecrunchFlag = 0;
		}
		else
			DecrunchFlag = 1;
	}

	i = -1;

	if (GetToolsType("KEY_PATTERN", tool) != 1)
		strcpy(HKey[0], "");
	else
		strcpy(HKey[0], tool);

	if (GetToolsType("KEY_ALL", tool) != 1)
		strcpy(HKey[1], "");
	else
		strcpy(HKey[1], tool);

	if (GetToolsType("KEY_NONE", tool) != 1)
		strcpy(HKey[2], "");
	else
		strcpy(HKey[2], tool);

	if (GetToolsType("KEY_PARENT", tool) != 1)
		strcpy(HKey[3], "");
	else
		strcpy(HKey[3], tool);

	if (GetToolsType("KEY_SELECT", tool) != 1)
		strcpy(HKey[4], "");
	else
		strcpy(HKey[4], tool);

	if (GetToolsType("KEY_TOGGLE", tool) != 1)
		strcpy(HKey[5], "");
	else
		strcpy(HKey[5], tool);

	if (GetToolsType("NOCOMMENT", tool) != 1)
		DoComm = 1;
	else
		DoComm = 0;

	if (GetToolsType("ICONSTART", tool) != 1)
		IconStart = 0;
	else
		IconStart = 1;

	if (GetToolsType("GTLIKE", tool) != 1)
		GtLike = 0;
	else
		GtLike = 1;

	if (GetToolsType("DISPINFO", tool) != 1)
		ShowIcons = 0;
	else
		ShowIcons = 1;

	DecrunchFlag = 0;
	if (GetToolsType("DECRUNCH", tool) == 1 && stricmp(tool, "YES") == 0)
		DecrunchFlag = 1;

	if (GetToolsType("CLISIZE", tool) != 1)
		strcpy(DevString, "CON:0/0/640/100/ClassAction_Output_Window");
	else
		strcpy(DevString, tool);

	if (GetToolsType("ICONFILE", tool) != 1)
		IconFile[0] = 0;
	else
		strcpy(IconFile, tool);

	if (GetToolsType("CAPREFS", tool) != 1)
		strcpy(PrefsPath, "Sys:Prefs/ClassActionPrefs");
	else
		strcpy(PrefsPath, tool);

	GetToolsType(NULL, NULL);
	return (0);
}

/****************************************
	Function : GetReg(user)

	Goal     : Check Registration

	Return Values : Always 0 (registered)
****************************************/
int GetReg(char *user)
{
	return (0);
}

/****************************************
	Function : GetRoot(lock)

	Goal     : return the root of a lock

	Return Values : the root lock
****************************************/
static BPTR GetRoot(BPTR lock)
{
	BPTR parent;

	while (lock)
	{
		parent = ParentDir(lock);
		if (!parent)
			break;

		UnLock(lock);
		lock = parent;
	}

	return lock;
}

/****************************************
	Function : DiskCmp(file1,file2)

	Goal     : 	Compare two files on disk
				to see if they are on the
				same disk

	Return Values :
	 0 = Same Disk
	 Not 0 = Different Disks
****************************************/
int DiskCmp(const char *file1, const char *file2)
{
	BPTR lock1 = 0, lock2 = 0;
	BPTR root1, root2;
	int result = 10;

	lock1 = Lock((STRPTR)file1, ACCESS_READ);
	lock2 = Lock((STRPTR)file2, ACCESS_READ);
	if (!lock1 || !lock2)
		goto cleanup;

	root1 = GetRoot(lock1);
	root2 = GetRoot(lock2);

	if (!root1 || !root2)
		goto cleanup;

	char aaaa[128];
	char bbbb[128];
	NameFromLock(root1, aaaa, sizeof(aaaa));
	NameFromLock(root2, bbbb, sizeof(bbbb));
	result = strcmp(aaaa, bbbb);

cleanup:
	if (lock1)
		UnLock(lock1);
	if (lock2)
		UnLock(lock2);

	return result;
}