/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : locale.c

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
#define catalog li.li_Catalog


/*****************************************
	Function : InitLocale()

	Goal     : Initialize locale
			   and load messages

	Return Values : NONE

****************************************/
void InitLocale(void)
{
	extern char **LocaleMsg;
	int i;
	char *tmp;

	LocaleBase = (struct Library *)OpenLibrary("locale.library", 38);
	if (LocaleBase != NULL)
		catalog = OpenCatalogA(NULL, "ClassAction.catalog", NULL);

	LocaleMsg = (char **)malloc(LOCALE_NB_MSG * sizeof(char *));

	for (i = 0; i < LOCALE_NB_MSG; i++)
	{
		tmp = GetString(&li, i);
		LocaleMsg[i] = (char *)malloc(strlen(tmp) + 1);
		strcpy(LocaleMsg[i], tmp);
	}

	if (LocaleBase)
	{
		CloseCatalog(catalog);
		CloseLibrary(LocaleBase);
	}
}

/*****************************************
	Function : FreeLocale()

	Goal     : Free locale messages

	Return Values : NONE

****************************************/
void FreeLocale(void)
{
	int i;

	for (i = 0; i < LOCALE_NB_MSG; i++)
		free(LocaleMsg[i]);

	free(LocaleMsg);
}