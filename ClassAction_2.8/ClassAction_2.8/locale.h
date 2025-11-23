#ifndef LOCALE_H
#define LOCALE_H


/****************************************************************************/


/* This file was created automatically by CatComp.
 * Do NOT edit by hand!
 */


#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifdef CATCOMP_ARRAY
#undef CATCOMP_NUMBERS
#undef CATCOMP_STRINGS
#define CATCOMP_NUMBERS
#define CATCOMP_STRINGS
#endif

#ifdef CATCOMP_BLOCK
#undef CATCOMP_STRINGS
#define CATCOMP_STRINGS
#endif


/****************************************************************************/


#ifdef CATCOMP_NUMBERS

#define LOCALE_NB_MSG   34

#define CA_FILE_NOT_FOUND 0
#define CA_COMMOD 1
#define CA_CONFIG_CORRUPT 2
#define CA_CONFIG_NOT_VALID 3
#define CA_GENERIC_NOT_VALID 4
#define CA_XFD_FAILED 5
#define CA_DECRUNCHING 6
#define CA_UNKNOWN 7
#define CA_READING 8
#define CA_SURE_ERROR 9
#define CA_REQ_ERROR 10
#define CA_COPYRIGHT 11
#define CA_QUIT 12
#define CA_SELECT_FILE 13
#define CA_INFO 14
#define CA_ALREADY_RUNNING 15
#define CA_APP_ERROR 16
#define CA_NO_CONFIG 17
#define CA_SHAREWARE 18
#define CA_YES_NO 19
#define CA_QUIT_BUTT 20
#define CA_ABOUT_BUTT 21
#define CA_CLASS_BUTT 22
#define CA_FILE_BUTT 23
#define CA_PARENT_BUTT 24
#define CA_INFO_BUTT 25
#define CA_NOT_CR 26
#define CA_NOT_DATA 27
#define CA_NOT_DECR 28
#define CA_INFORMATION 29
#define CA_MEMORY 30
#define CA_LOADING 31
#define CA_SURE 32
#define CA_OKAY 33

#endif /* CATCOMP_NUMBERS */


/****************************************************************************/


#ifdef CATCOMP_STRINGS

#define CA_FILE_NOT_FOUND_STR "File Not Found"
#define CA_COMMOD_STR "Recognize and handle ANY file format."
#define CA_CONFIG_CORRUPT_STR "Your Config File\n\nIs Corrupted"
#define CA_CONFIG_NOT_VALID_STR "Your Config File\n\nis not a valid\n\nClassAction Config File"
#define CA_GENERIC_NOT_VALID_STR "The Generic File is not valid."
#define CA_XFD_FAILED_STR "xfdmaster.library not found !!\nThe Auto Decrunching Function\nis now Deactivated."
#define CA_DECRUNCHING_STR "Decrunching"
#define CA_UNKNOWN_STR "Unknown"
#define CA_READING_STR "Reading & Sorting"
#define CA_SURE_ERROR_STR "Syntax Error in SURE command"
#define CA_REQ_ERROR_STR "Syntax Error in REQ command"
#define CA_COPYRIGHT_STR "%s \n\n(c) 1994-1996 by Salim Gasmi\n\nYou can contact me at :\n\nGasmi Salim\n4b rue des petits champs\nF-67300 Shiltigheim\nFrance\n\nEmail: salim@gasmi.net\nEmail: salim@sdv.fr\nWeb: http://www.gasmi.net\nIrc: Dr_Unix (#amigafr,#amiga)\n\nRegistered to : %s"
#define CA_QUIT_STR "Really Quit"
#define CA_SELECT_FILE_STR "You must select a file first"
#define CA_INFO_STR "File      : %s\nClass     : %s\nSize      : %ld Bytes"
#define CA_ALREADY_RUNNING_STR "ClassAction is already running."
#define CA_APP_ERROR_STR "Unable to create an App icon"
#define CA_NO_CONFIG_STR "Config file\nNot Found."
#define CA_SHAREWARE_STR "ClassAction is ShareWare.\n\nIf you USE it please Register.\n\nThe Registration fee is : $ 10 or 20 DM or 50 FF.\n\nMy address can be found with the 'About' button."
#define CA_YES_NO_STR "Yes|No"
#define CA_QUIT_BUTT_STR "Quit"
#define CA_ABOUT_BUTT_STR "About"
#define CA_CLASS_BUTT_STR "Class"
#define CA_FILE_BUTT_STR "File "
#define CA_PARENT_BUTT_STR "Parent"
#define CA_INFO_BUTT_STR "Info"
#define CA_NOT_CR_STR "Not Crunched"
#define CA_NOT_DATA_STR "Not a Data File"
#define CA_NOT_DECR_STR "Not Decrunched"
#define CA_INFORMATION_STR "Information"
#define CA_MEMORY_STR "Not Enough Memory \n\nto load the file"
#define CA_LOADING_STR "Loading"
#define CA_SURE_STR "Are you Sure ?"
#define CA_OKAY_STR "Okay"

#endif /* CATCOMP_STRINGS */


/****************************************************************************/


#ifdef CATCOMP_ARRAY

struct CatCompArrayType
{
    LONG   cca_ID;
    STRPTR cca_Str;
};

static const struct CatCompArrayType CatCompArray[] =
{
    {CA_FILE_NOT_FOUND,(STRPTR)CA_FILE_NOT_FOUND_STR},
    {CA_COMMOD,(STRPTR)CA_COMMOD_STR},
    {CA_CONFIG_CORRUPT,(STRPTR)CA_CONFIG_CORRUPT_STR},
    {CA_CONFIG_NOT_VALID,(STRPTR)CA_CONFIG_NOT_VALID_STR},
    {CA_GENERIC_NOT_VALID,(STRPTR)CA_GENERIC_NOT_VALID_STR},
    {CA_XFD_FAILED,(STRPTR)CA_XFD_FAILED_STR},
    {CA_DECRUNCHING,(STRPTR)CA_DECRUNCHING_STR},
    {CA_UNKNOWN,(STRPTR)CA_UNKNOWN_STR},
    {CA_READING,(STRPTR)CA_READING_STR},
    {CA_SURE_ERROR,(STRPTR)CA_SURE_ERROR_STR},
    {CA_REQ_ERROR,(STRPTR)CA_REQ_ERROR_STR},
    {CA_COPYRIGHT,(STRPTR)CA_COPYRIGHT_STR},
    {CA_QUIT,(STRPTR)CA_QUIT_STR},
    {CA_SELECT_FILE,(STRPTR)CA_SELECT_FILE_STR},
    {CA_INFO,(STRPTR)CA_INFO_STR},
    {CA_ALREADY_RUNNING,(STRPTR)CA_ALREADY_RUNNING_STR},
    {CA_APP_ERROR,(STRPTR)CA_APP_ERROR_STR},
    {CA_NO_CONFIG,(STRPTR)CA_NO_CONFIG_STR},
    {CA_SHAREWARE,(STRPTR)CA_SHAREWARE_STR},
    {CA_YES_NO,(STRPTR)CA_YES_NO_STR},
    {CA_QUIT_BUTT,(STRPTR)CA_QUIT_BUTT_STR},
    {CA_ABOUT_BUTT,(STRPTR)CA_ABOUT_BUTT_STR},
    {CA_CLASS_BUTT,(STRPTR)CA_CLASS_BUTT_STR},
    {CA_FILE_BUTT,(STRPTR)CA_FILE_BUTT_STR},
    {CA_PARENT_BUTT,(STRPTR)CA_PARENT_BUTT_STR},
    {CA_INFO_BUTT,(STRPTR)CA_INFO_BUTT_STR},
    {CA_NOT_CR,(STRPTR)CA_NOT_CR_STR},
    {CA_NOT_DATA,(STRPTR)CA_NOT_DATA_STR},
    {CA_NOT_DECR,(STRPTR)CA_NOT_DECR_STR},
    {CA_INFORMATION,(STRPTR)CA_INFORMATION_STR},
    {CA_MEMORY,(STRPTR)CA_MEMORY_STR},
    {CA_LOADING,(STRPTR)CA_LOADING_STR},
    {CA_SURE,(STRPTR)CA_SURE_STR},
    {CA_OKAY,(STRPTR)CA_OKAY_STR},
};

#endif /* CATCOMP_ARRAY */


/****************************************************************************/


#ifdef CATCOMP_BLOCK

static const char CatCompBlock[] =
{
    "\x00\x00\x00\x00\x00\x10"
    CA_FILE_NOT_FOUND_STR "\x00\x00"
    "\x00\x00\x00\x01\x00\x26"
    CA_COMMOD_STR "\x00"
    "\x00\x00\x00\x02\x00\x20"
    CA_CONFIG_CORRUPT_STR "\x00\x00"
    "\x00\x00\x00\x03\x00\x3A"
    CA_CONFIG_NOT_VALID_STR "\x00"
    "\x00\x00\x00\x04\x00\x20"
    CA_GENERIC_NOT_VALID_STR "\x00\x00"
    "\x00\x00\x00\x05\x00\x52"
    CA_XFD_FAILED_STR "\x00\x00"
    "\x00\x00\x00\x06\x00\x0C"
    CA_DECRUNCHING_STR "\x00"
    "\x00\x00\x00\x07\x00\x08"
    CA_UNKNOWN_STR "\x00"
    "\x00\x00\x00\x08\x00\x12"
    CA_READING_STR "\x00"
    "\x00\x00\x00\x09\x00\x1E"
    CA_SURE_ERROR_STR "\x00\x00"
    "\x00\x00\x00\x0A\x00\x1C"
    CA_REQ_ERROR_STR "\x00"
    "\x00\x00\x00\x0B\x00\xF6"
    CA_COPYRIGHT_STR "\x00\x00"
    "\x00\x00\x00\x0C\x00\x0C"
    CA_QUIT_STR "\x00"
    "\x00\x00\x00\x0D\x00\x1E"
    CA_SELECT_FILE_STR "\x00\x00"
    "\x00\x00\x00\x0E\x00\x34"
    CA_INFO_STR "\x00"
    "\x00\x00\x00\x0F\x00\x20"
    CA_ALREADY_RUNNING_STR "\x00"
    "\x00\x00\x00\x10\x00\x1E"
    CA_APP_ERROR_STR "\x00\x00"
    "\x00\x00\x00\x11\x00\x18"
    CA_NO_CONFIG_STR "\x00\x00"
    "\x00\x00\x00\x12\x00\xA0"
    CA_SHAREWARE_STR "\x00\x00"
    "\x00\x00\x00\x13\x00\x08"
    CA_YES_NO_STR "\x00\x00"
    "\x00\x00\x00\x14\x00\x06"
    CA_QUIT_BUTT_STR "\x00\x00"
    "\x00\x00\x00\x15\x00\x06"
    CA_ABOUT_BUTT_STR "\x00"
    "\x00\x00\x00\x16\x00\x06"
    CA_CLASS_BUTT_STR "\x00"
    "\x00\x00\x00\x17\x00\x06"
    CA_FILE_BUTT_STR "\x00"
    "\x00\x00\x00\x18\x00\x08"
    CA_PARENT_BUTT_STR "\x00\x00"
    "\x00\x00\x00\x19\x00\x06"
    CA_INFO_BUTT_STR "\x00\x00"
    "\x00\x00\x00\x1A\x00\x0E"
    CA_NOT_CR_STR "\x00\x00"
    "\x00\x00\x00\x1B\x00\x10"
    CA_NOT_DATA_STR "\x00"
    "\x00\x00\x00\x1C\x00\x10"
    CA_NOT_DECR_STR "\x00\x00"
    "\x00\x00\x00\x1D\x00\x0C"
    CA_INFORMATION_STR "\x00"
    "\x00\x00\x00\x1E\x00\x26"
    CA_MEMORY_STR "\x00\x00"
    "\x00\x00\x00\x1F\x00\x08"
    CA_LOADING_STR "\x00"
    "\x00\x00\x00\x20\x00\x10"
    CA_SURE_STR "\x00\x00"
    "\x00\x00\x00\x21\x00\x06"
    CA_OKAY_STR "\x00\x00"
};

#endif /* CATCOMP_BLOCK */


/****************************************************************************/


struct LocaleInfo
{
    APTR li_LocaleBase;
    APTR li_Catalog;
};


#ifdef CATCOMP_CODE

STRPTR GetString(struct LocaleInfo *li, LONG stringNum)
{
LONG   *l;
UWORD  *w;
STRPTR  builtIn;

    l = (LONG *)CatCompBlock;

    while (*l != stringNum)
    {
        w = (UWORD *)((ULONG)l + 4);
        l = (LONG *)((ULONG)l + (ULONG)*w + 6);
    }
    builtIn = (STRPTR)((ULONG)l + 6);

#define XLocaleBase LocaleBase
#define LocaleBase li->li_LocaleBase
    
    if (LocaleBase)
        return(GetCatalogStr(li->li_Catalog,stringNum,builtIn));
#define LocaleBase XLocaleBase
#undef XLocaleBase

    return(builtIn);
}


#endif /* CATCOMP_CODE */


/****************************************************************************/


#endif /* LOCALE_H */
