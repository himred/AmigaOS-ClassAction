/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : Disk.c

	Goal : Load/Save Functions

	Ver  : 2.00

****************************************/

#include <fcntl.h>
#include <exec/types.h>
#include <exec/exec.h>
#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <intuition/classusr.h>
#include <intuition/imageclass.h>
#include <intuition/gadgetclass.h>
#include <libraries/gadtools.h>
#include <graphics/displayinfo.h>
#include <graphics/gfxbase.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/gadtools_protos.h>
#include <clib/graphics_protos.h>
#include <clib/utility_protos.h>
#include <prefs/font.h>
#include <string.h>
#include <stdio.h>

#include "includes.h"
#include "gadgets.h"

FILE *f;

/****************************************/
/*										*/
/* Function : Re(string)				*/
/*										*/
/* Goal     : Read next string from		*/
/*	      File Handler f and check		*/
/*	      all errors					*/
/*										*/
/*										*/
/* Return Values : NONE 				*/
/*										*/
/*										*/
/*										*/
/****************************************/

void Re(char *s)
{
	static fuck = 0;

	if (fgets(s, 256, f) == NULL)
	{
		strcpy(s, "???");
		if (fuck == 0)
		{
			fuck = 1;
			Info(LocaleMsg[CA_CONFIG_CORRUPT]);
		}
	}
	else
		s[strlen(s) - 1] = 0;
}

/****************************************/
/*										*/
/* Function : LoadMem()					*/
/*										*/
/* Goal     : Load the Config File		*/
/*	      Into Memory and build up		*/
/*	      all the needed lists			*/
/*										*/
/*										*/
/* Return Values : 						*/
/*										*/
/*	 0 = Everything is OK				*/
/*	-1 = No Config File Found			*/
/*	-2 = Not a valid File				*/
/*										*/
/****************************************/

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
		Info(LocaleMsg[CA_CONFIG_NOT_VALID]);
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

/****************************************/
/*										*/
/* Function : LoadGen()					*/
/*										*/
/* Goal     : Load the Generic File		*/
/*	      Into Memory and build up		*/
/*	      all the needed lists			*/
/*										*/
/*										*/
/* Return Values : 						*/
/*										*/
/*	 0 = Everything is OK				*/
/*	-1 = No Config File Found			*/
/*	-2 = Not a valid File				*/
/*										*/
/****************************************/

int LoadGen()
{
	struct Action *act;
	struct Class *cla;
	char ch[256];
	char tmp[20];
	int i, out = 0;
	char *ppp;

	Generic.name[0] = 0;
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
		Info(LocaleMsg[CA_GENERIC_NOT_VALID]);
		return (-2);
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
	return (0);
}

/****************************************/
/*										*/
/* Function : ReadToolsTypes			*/
/*										*/
/* Goal     : Get The ToolsTypes		*/
/*	      and setup global flags		*/
/*										*/
/* Return Values : 						*/
/*										*/
/*	Always 0							*/
/*										*/
/****************************************/

int ReadToolsTypes(void)
{
	int i;
	char tmp[1024];
	char tool[100];
	char *p;

	for (i = 1; i <= 11; i++)
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
			Drives[i - 1][0] = 0;
	}

	strcpy(tool, "NO");
	if (GetToolsType("DECRUNCH", tool) != 1)
		DecrunchFlag = 0;

	if (stricmp(tool, "YES") == 0)
	{
		if (OpenXfdLib() == -1)
		{
			Info(LocaleMsg[CA_XFD_FAILED]);
			DecrunchFlag = 0;
		}
		else
			DecrunchFlag = 1;
	}

	strcpy(tool, "146");
	if (GetToolsType("HEIGHT", tool) != 1)
		WindowHeight = 0;
	WindowHeight = atoi(tool);

	i = -1;

	if (GetToolsType("STARTDIR", tool) != 1)
		DirLock = GetProgramDir();
	else
	{
		DirLock = Lock(tool, ACCESS_READ);
		if (DirLock == NULL)
			DirLock = GetProgramDir();
		else
			i = 0;
	}

	if (i == -1)
		NameFromLock(DirLock, tmp, 1000);
	else
		strcpy(tmp, tool);
	strcpy(Drives[11], tmp);

	strcpy(tool, "NO");
	if (GetToolsType("APPSTART", tool) != -1)
		AppStart = 0;
	if (stricmp(tool, "YES") == 0)
		AppStart = 1;
	if (stricmp(tool, "HIDE") == 0)
		AppStart = 2;

	strcpy(tool, "NO");
	if (GetToolsType("PUBSCREEN", tool) != -1)
		PubScreen = 0;
	if (stricmp(tool, "YES") == 0)
		PubScreen = 1;

	if (GetToolsType("CX_PRIORITY", tool) != -1)
		CxPri = 0;
	else
		CxPri = atoi(tool);

	if (GetToolsType("ICONX", tool) != 1)
		IconX = -1;
	else
		IconX = atoi(tool);

	if (GetToolsType("ICONY", tool) != 1)
		IconY = -1;
	else
		IconY = atoi(tool);

	if (GetToolsType("WINX", tool) != 1)
		WinX = -1;
	else
		WinX = atoi(tool);

	if (GetToolsType("WINY", tool) != 1)
		WinY = -1;
	else
		WinY = atoi(tool);

	if (GetToolsType("ICONNAME", tool) != 1)
		strcpy(IconName, "ClassAction");
	else
		strcpy(IconName, tool);

	if (GetToolsType("CX_HOTKEY", tool) != 1)
		strcpy(CAHotKey, "LALT C");
	else
		strcpy(CAHotKey, tool);

	strcpy(tool, "YES");
	if (GetToolsType("WBFONT", tool) != 1)
		UseDefFont = 0;
	if (stricmp(tool, "NO") == 0)
		UseDefFont = 1;

	strcpy(tool, "NO");
	if (GetToolsType("REQBUG", tool) != 1)
		ReqBug = 0;
	if (stricmp(tool, "YES") == 0)
		ReqBug = -1;

	if (GetToolsType("CLISIZE", tool) != 1)
		strcpy(DevString, "CON:0/0/640/100/ClassAction_Output_Window");
	else
		strcpy(DevString, tool);

	if (GetToolsType("ICONFILE", tool) != 1)
		IconFile[0] = 0;
	else
		strcpy(IconFile, tool);

	GetToolsType(NULL, NULL);
	return (0);
}

/****************************************/
/*										*/
/* Function : ReadPrefs()				*/
/*										*/
/* Goal     : Get the prefs fonts name	*/
/*	      Used in the Workbench			*/
/*										*/
/* Return Values : 						*/
/*										*/
/*	-1: Failed to get					*/
/*	 0: Okay							*/
/*										*/
/****************************************/

int LoadPrefs()
{
	struct FontPrefs *fp;
	char *buff;
	int l, i;
	int h;
	int ok = 0;

	h = open(PREF, O_RDONLY);
	if (h < 0)
		h = open(PREF2, O_RDONLY);
	if (h < 0)
		return (-1);

	l = lseek(h, 0, 2);
	lseek(h, 0, 0);

	buff = (char *)malloc(l);
	if (buff == NULL)
	{
		close(h);
		return (-1);
	}

	read(h, buff, l);
	close(h);

	for (i = 0; i < l - 4; i++)
		if (buff[i] == 'F' && buff[i + 1] == 'O' && buff[i + 2] == 'N' && buff[i + 3] == 'T')
		{
			i = i + 8;
			fp = (struct FontPrefs *)(buff + i);
			if (fp->fp_Type == FP_SYSFONT)
			{
				Dirattr = fp->fp_TextAttr;
				strcpy(DFname, fp->fp_Name);
				Dirattr.ta_Name = (STRPTR)DFname;
				ok++;
			}

			if (fp->fp_Type == FP_SCREENFONT)
			{
				Myattr = fp->fp_TextAttr;
				strcpy(TFname, fp->fp_Name);
				Myattr.ta_Name = (STRPTR)TFname;
				ok++;
			}

			i = i + sizeof(struct FontPrefs) - 1;
		}

	free(buff);
	if (ok == 2)
		return (0);
	else
		return (-1);
}

int GetReg(char *user)
{

	int h;
	int i;
	unsigned char x;
	int cs;
	int l;

	h = open("s:CA.key", O_RDONLY);
	if (h < 0)
		return (-1);

	cs = 0;
	for (i = 0; i < 200; i++)
	{
		read(h, &x, 1);
		cs = cs + x;
	}

	read(h, &x, 1);
	cs = cs % 256;
	if (x != cs)
	{
		close(h);
		return (-1);
	}

	read(h, &x, 1);
	l = x;

	cs = 0;
	for (i = 0; i < l; i++)
	{
		read(h, &x, 1);
		cs = cs + x;
		user[i] = x - 85 - i;
	}

	user[l] = 0;
	read(h, &x, 1);
	cs = cs % 256;
	if (cs != x)
	{
		close(h);
		return (-3);
	}

	cs = 0;
	for (i = 0; i < 200; i++)
	{
		read(h, &x, 1);
		cs = cs + x;
	}

	read(h, &x, 1);
	cs = cs % 256;
	if (x != cs)
	{
		close(h);
		return (-2);
	}

	close(h);
	return (0);
}
