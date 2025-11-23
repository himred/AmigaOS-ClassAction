/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : learn.c

	Goal : learn new classes

	Ver  : 3.0

****************************************/

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
#include <string.h>
#include <clib/asl_protos.h>

#include <libraries/mui.h>

#include "includes.h"

#include <intuition/intuition.h>
#include <libraries/reqtools.h>
#include <clib/reqtools_protos.h>

#include <stdio.h>
#include <fcntl.h>
#include <dos/dosextens.h>

#define SENS 3
#define MAXI 16

struct rtFileRequester *req;
struct rtFileList *head, *tmplist;
char Accepted = 0;
int Nf;

extern struct ReqToolsBase *ReqToolsBase;

int Learn(APTR WIN, APTR ST_O[5], APTR GA_LEARN, long accu)
{
	int Analyse(APTR, APTR[], APTR, long);
	char file[108];
	int x;

	req = rtAllocRequest(RT_FILEREQ, NULL);

	if (req == NULL)
		return (-2);

	file[0] = 0;
	head = rtFileRequest(req, file, LocaleMsg[CAP_L_CHOOSE], RTFI_Flags, FREQF_MULTISELECT, TAG_DONE);

	if (head == NULL)
	{
		rtFreeRequest(req);
		return (-1);
	}

	tmplist = head;
	Nf = 0;
	while (tmplist != NULL)
	{
		tmplist = tmplist->Next;
		Nf++;
	}

	if (Nf < 2)
	{
		rtFreeRequest(req);
		rtFreeFileList(head);
		return (-4);
	}

	set(WIN, MUIA_Window_Open, TRUE);
	x = Analyse(WIN, ST_O, GA_LEARN, accu);
	rtFreeRequest(req);
	rtFreeFileList(head);
	return (x);
}

/******/

int Analyse(APTR WI, APTR ST_O[5], APTR GA_LEARN, long accu)
{

	struct FileInfoBlock info;
	BPTR lock;
	LONG l;
	int i;
	LONG min = 9999999999;
	char *buff, *tmp;
	unsigned int *same;
	int k, h;
	int size;
	char isasc;
	int j;
	int nbz;
	int nf = 0;
	char cc[5];
	char offset[60];
	int argc;
	char filename[255], file[255];
	char dir[100];
	int s;
	int precis;

	argc = Nf;
	l = strlen(req->Dir);
	if (l == 0)
		dir[0] = 0;
	else
	{
		if (req->Dir[l - 1] != ':')
			sprintf(dir, "%s/", req->Dir);
		else
			strcpy(dir, req->Dir);
	}

	set(GA_LEARN, MUIA_Gauge_InfoText, LocaleMsg[CAP_L_PREP]);

	tmplist = head;
	for (i = 0; i < argc; i++)
	{
		sprintf(filename, "%s%s", dir, tmplist->Name);
		lock = Lock(filename, ACCESS_READ);
		Examine(lock, &info);
		l = (LONG)info.fib_Size;
		if (l < min)
		{
			min = l;
			strcpy(file, filename);
		}
		UnLock(lock);
		tmplist = tmplist->Next;
	}

	if (min < 8)
	{
		return (0);
	}

	buff = (char *)malloc(min);
	tmp = (char *)malloc(min);
	same = (int *)malloc(min * sizeof(int));

	if (buff == NULL || tmp == NULL || same == NULL)
	{
		if (buff != NULL)
			free(buff);
		if (tmp != NULL)
			free(tmp);
		if (same != NULL)
			free(same);
		return (-10);
	}

	for (i = 0; i < min; i++)
		same[i] = 0;

	h = open(file, O_RDONLY);
	read(h, buff, min);
	close(h);

	tmplist = head;
	set(GA_LEARN, MUIA_Gauge_Max, argc);

	for (i = 0; i < argc; i++)
	{
		sprintf(offset, "%s (%d/%d)", LocaleMsg[CAP_L_PROG], i + 1, argc);

		set(GA_LEARN, MUIA_Gauge_Current, i + 1);
		set(GA_LEARN, MUIA_Gauge_InfoText, offset);

		sprintf(filename, "%s%s", dir, tmplist->Name);
		if (strcmp(filename, file) != 0)
		{
			h = open(filename, O_RDONLY);
			read(h, tmp, min);
			close(h);
			for (j = 0; j < min; j++)
				if (tmp[j] == buff[j])
					same[j]++;
		}

		tmplist = tmplist->Next;
	}

	i = -1;
	nf = 0;

	precis = (argc * accu / 100) - 1;

	while ((i < min - SENS) && (nf < 5))
	{
		i++;
		if (same[i] >= precis)
		{
			k = i + 1;
			s = i;
			while (k < min && same[k] >= precis && (k - i <= MAXI))
				k++;
			while ((buff[s] == 0) && (s <= k))
				s++;
			if (k - s >= SENS)
			{
				size = k - s;
				isasc = 1;
				nbz = 0;
				for (j = 0; j < size; j++)
				{
					if (buff[s + j] == 0)
						nbz++;
					if (buff[s + j] < 32 || buff[s + j] > 126)
					{
						if (j >= SENS && isasc == 1)
						{
							size = j;
							isasc = 1;
							j = size + 1;
						}
						else
							isasc = 0;
					}
				}

				nf++;
				sprintf(offset, "%d,", s);
				if (isasc == 1)
					strcat(offset, "'");
				for (j = 0; j < size; j++)
				{
					if (isasc == 0)
						sprintf(cc, "%02x", (unsigned char)buff[s + j]);
					else
						sprintf(cc, "%c", buff[s + j]);
					strcat(offset, cc);
				}

				if (isasc == 1)
					strcat(offset, "'");

				set(ST_O[nf - 1], MUIA_String_Contents, offset);

				i = k - 1;
			}
		}
	}

	set(GA_LEARN, MUIA_Gauge_InfoText, LocaleMsg[CAP_L_COMP]);

	free(buff);
	free(same);
	free(tmp);
	return (nf);
}
