
/****************************************

	Class Action Source

	(c) by 1995 Salim Gasmi

	File : locale.c

	Goal : define localization stuff

	Ver  : 3.00

****************************************/


#include <exec/types.h>
#include <exec/libraries.h>
#include <libraries/locale.h>
#include <clib/locale_protos.h>


#define CATCOMP_NUMBERS
#define CATCOMP_BLOCK
#define CATCOMP_CODE

#include "locale.h"

struct LocaleInfo li;


#define LocaleBase li.li_LocaleBase
#define catalog    li.li_Catalog


void InitLocale(void)
{
extern char	**LocaleMsg;
int i;
char *tmp;

    LocaleBase = (struct Library *)OpenLibrary("locale.library",38);
	if(LocaleBase!=NULL) catalog = OpenCatalogA(NULL,"ClassActionPrefs.catalog",NULL);


	LocaleMsg=(char **)malloc(LOCALE_NB_MSG*sizeof(char *));
	
	for(i=0;i<LOCALE_NB_MSG;i++)
		{
		tmp=GetString(&li,i);
		LocaleMsg[i]=(char *)malloc(strlen(tmp)+1);
		strcpy(LocaleMsg[i],tmp);
		}

    if (LocaleBase)
    {
        CloseCatalog(catalog);
        CloseLibrary(LocaleBase);
    }
}



void FreeLocale(void)
{
int i;

for(i=0;i<LOCALE_NB_MSG;i++)
	free(LocaleMsg[i]);

free(LocaleMsg);

}
