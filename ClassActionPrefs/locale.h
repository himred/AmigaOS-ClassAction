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

#define LOCALE_NB_MSG 74

#define CAP_CONFIG_CORRUPT 0
#define CAP_CONFIG_NOT_VALID 1
#define CAP_GENERIC 2
#define CAP_GENERIC_NOT_VALID 3
#define CAP_DIR_NOT_VALID 4
#define CAP_ADD 5
#define CAP_DEL 6
#define CAP_SAVE 7
#define CAP_USE 8
#define CAP_CLASSES 9
#define CAP_ACTIONS 10
#define CAP_CLASSNAME 11
#define CAP_MATCH_NAME 12
#define CAP_NAME 13
#define CAP_EXEC 14
#define CAP_LOAD 15
#define CAP_STACK 16
#define CAP_COMM 17
#define CAP_COPY 18
#define CAP_UP 19
#define CAP_DOWN 20
#define CAP_CANCEL 21
#define CAP_DELAY 22
#define CAP_SELECT 23
#define CAP_NEW_CLASS 24
#define CAP_NEW_ACTION 25
#define CAP_CANT_DELETE 26
#define CAP_DEL_CLASS 27
#define CAP_DEL_ACTION 28
#define CAP_UNABLE 29
#define CAP_F_COMM 30
#define CAP_S_COMM 31
#define CAP_B_COMM 32
#define CAP_X_COMM 33
#define CAP_BF_COMM 34
#define CAP_BS_COMM 35
#define CAP_BB_COMM 36
#define CAP_BX_COMM 37
#define CAP_REQF_COMM 38
#define CAP_REQD_COMM 39
#define CAP_REQV_COMM 40
#define CAP_REQT_COMM 41
#define CAP_SURE_COMM 42
#define CAP_COMMANDS 43
#define CAP_ARE_U_SURE 44
#define CAP_YES_NO 45
#define CAP_INFO 46
#define CAP_OK 47
#define CAP_UNKNOWN 48
#define CAP_NOACT 49
#define CAP_L_NEED 50
#define CAP_L_CHOOSE 51
#define CAP_L_MEMORY 52
#define CAP_L_NOTHING 53
#define CAP_L_PROG 54
#define CAP_L_PREP 55
#define CAP_L_LEARN 56
#define CAP_L_STATUS 57
#define CAP_L_ACCEPT 58
#define CAP_L_DO_U 59
#define CAP_L_COMP 60
#define CAP_L_ACCU 61
#define CAP_DIRECTORY 62
#define CAP_H_DELC 63
#define CAP_H_DELA 64
#define CAP_H_ADDC 65
#define CAP_H_ADDA 66
#define CAP_H_LEARN 67
#define CAP_H_FILE 68
#define CAP_H_COMM 69
#define CAP_H_SAVE 70
#define CAP_H_USE 71
#define CAP_H_QUIT 72
#define CAP_SHAREWARE 73

#endif /* CATCOMP_NUMBERS */


/****************************************************************************/


#ifdef CATCOMP_STRINGS

#define CAP_CONFIG_CORRUPT_STR "Your Config File\n\nIs Corrupted"
#define CAP_CONFIG_NOT_VALID_STR "Your Config File\n\nis not a valid\n\nClassAction Config File"
#define CAP_GENERIC_STR "Generic Actions"
#define CAP_GENERIC_NOT_VALID_STR "The Generic File is not valid."
#define CAP_DIR_NOT_VALID_STR "The Dir file is not valid."
#define CAP_ADD_STR "Add"
#define CAP_DEL_STR "Del"
#define CAP_SAVE_STR "Save"
#define CAP_USE_STR "Use"
#define CAP_CLASSES_STR "Classes"
#define CAP_ACTIONS_STR "Actions"
#define CAP_CLASSNAME_STR "ClassName"
#define CAP_MATCH_NAME_STR "MatchName"
#define CAP_NAME_STR "Name"
#define CAP_EXEC_STR "Exec"
#define CAP_LOAD_STR "Load"
#define CAP_STACK_STR "Stack"
#define CAP_COMM_STR "Comm"
#define CAP_COPY_STR "Copy"
#define CAP_UP_STR "U"
#define CAP_DOWN_STR "D"
#define CAP_CANCEL_STR "Cancel"
#define CAP_DELAY_STR "Delay"
#define CAP_SELECT_STR "Select the source Class"
#define CAP_NEW_CLASS_STR "<New Class #%d>"
#define CAP_NEW_ACTION_STR "<New Action #%d>"
#define CAP_CANT_DELETE_STR "You Can't Delete this Class"
#define CAP_DEL_CLASS_STR "Delete this Class ?"
#define CAP_DEL_ACTION_STR "Delete this action ?"
#define CAP_UNABLE_STR "Unable to Proceed !\nSource and Destination\nClasses are equal"
#define CAP_F_COMM_STR "[f]    ''Path+File Name''"
#define CAP_S_COMM_STR "[s]    ''Path+File Name without suffix''"
#define CAP_B_COMM_STR "[b]    ''File Name''"
#define CAP_X_COMM_STR "[x]    ''File Name without suffix'' "
#define CAP_BF_COMM_STR "[F]    Path+File Name"
#define CAP_BS_COMM_STR "[S]    Path+File Name without suffix"
#define CAP_BB_COMM_STR "[B]    File Name"
#define CAP_BX_COMM_STR "[X]    File Name without suffix"
#define CAP_REQF_COMM_STR "REQF   Request a file"
#define CAP_REQD_COMM_STR "REQD   Request a directory"
#define CAP_REQV_COMM_STR "REQV   Request a volume"
#define CAP_REQT_COMM_STR "REQT   Request a text"
#define CAP_SURE_COMM_STR "SURE   Confirm a command"
#define CAP_COMMANDS_STR "Commands"
#define CAP_ARE_U_SURE_STR "Are you sure ?"
#define CAP_YES_NO_STR "Yes|No"
#define CAP_INFO_STR "Information"
#define CAP_OK_STR "Okay"
#define CAP_UNKNOWN_STR "Unknown Class"
#define CAP_NOACT_STR "No Action"
#define CAP_L_NEED_STR "I need at least 2 files for learning."
#define CAP_L_CHOOSE_STR "Choose files to learn."
#define CAP_L_MEMORY_STR "Not Enough Memory for learning."
#define CAP_L_NOTHING_STR "Nothing found."
#define CAP_L_PROG_STR "Analysis in progress ..."
#define CAP_L_PREP_STR "Preprocessing... please wait"
#define CAP_L_LEARN_STR "Learn"
#define CAP_L_STATUS_STR "Status"
#define CAP_L_ACCEPT_STR "Accept"
#define CAP_L_DO_U_STR "Accept this analysis ?"
#define CAP_L_COMP_STR "Analysis complete."
#define CAP_L_ACCU_STR "Learning accuracy (%%)"
#define CAP_DIRECTORY_STR "Directory"
#define CAP_H_DELC_STR "Delete the selected class."
#define CAP_H_DELA_STR "Delete the selected action."
#define CAP_H_ADDC_STR "Add a new class."
#define CAP_H_ADDA_STR "Add a new action."
#define CAP_H_LEARN_STR "Automatic learn of a new class."
#define CAP_H_FILE_STR "Select a file."
#define CAP_H_COMM_STR "List of all internal commands"
#define CAP_H_SAVE_STR "Save the configuration and quit."
#define CAP_H_USE_STR "Use the configuration and quit."
#define CAP_H_QUIT_STR "Quit without any modifications"
#define CAP_SHAREWARE_STR "\33c\33bClassAction\33n is MailWare.\n\nIf you USE it please Register.\n\nThe Registration is FREE: Just connect to http://www.gasmi.net.\n\nRead the doc for more info."

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
    {CAP_CONFIG_CORRUPT,(STRPTR)CAP_CONFIG_CORRUPT_STR},
    {CAP_CONFIG_NOT_VALID,(STRPTR)CAP_CONFIG_NOT_VALID_STR},
    {CAP_GENERIC,(STRPTR)CAP_GENERIC_STR},
    {CAP_GENERIC_NOT_VALID,(STRPTR)CAP_GENERIC_NOT_VALID_STR},
    {CAP_DIR_NOT_VALID,(STRPTR)CAP_DIR_NOT_VALID_STR},
    {CAP_ADD,(STRPTR)CAP_ADD_STR},
    {CAP_DEL,(STRPTR)CAP_DEL_STR},
    {CAP_SAVE,(STRPTR)CAP_SAVE_STR},
    {CAP_USE,(STRPTR)CAP_USE_STR},
    {CAP_CLASSES,(STRPTR)CAP_CLASSES_STR},
    {CAP_ACTIONS,(STRPTR)CAP_ACTIONS_STR},
    {CAP_CLASSNAME,(STRPTR)CAP_CLASSNAME_STR},
    {CAP_MATCH_NAME,(STRPTR)CAP_MATCH_NAME_STR},
    {CAP_NAME,(STRPTR)CAP_NAME_STR},
    {CAP_EXEC,(STRPTR)CAP_EXEC_STR},
    {CAP_LOAD,(STRPTR)CAP_LOAD_STR},
    {CAP_STACK,(STRPTR)CAP_STACK_STR},
    {CAP_COMM,(STRPTR)CAP_COMM_STR},
    {CAP_COPY,(STRPTR)CAP_COPY_STR},
    {CAP_UP,(STRPTR)CAP_UP_STR},
    {CAP_DOWN,(STRPTR)CAP_DOWN_STR},
    {CAP_CANCEL,(STRPTR)CAP_CANCEL_STR},
    {CAP_DELAY,(STRPTR)CAP_DELAY_STR},
    {CAP_SELECT,(STRPTR)CAP_SELECT_STR},
    {CAP_NEW_CLASS,(STRPTR)CAP_NEW_CLASS_STR},
    {CAP_NEW_ACTION,(STRPTR)CAP_NEW_ACTION_STR},
    {CAP_CANT_DELETE,(STRPTR)CAP_CANT_DELETE_STR},
    {CAP_DEL_CLASS,(STRPTR)CAP_DEL_CLASS_STR},
    {CAP_DEL_ACTION,(STRPTR)CAP_DEL_ACTION_STR},
    {CAP_UNABLE,(STRPTR)CAP_UNABLE_STR},
    {CAP_F_COMM,(STRPTR)CAP_F_COMM_STR},
    {CAP_S_COMM,(STRPTR)CAP_S_COMM_STR},
    {CAP_B_COMM,(STRPTR)CAP_B_COMM_STR},
    {CAP_X_COMM,(STRPTR)CAP_X_COMM_STR},
    {CAP_BF_COMM,(STRPTR)CAP_BF_COMM_STR},
    {CAP_BS_COMM,(STRPTR)CAP_BS_COMM_STR},
    {CAP_BB_COMM,(STRPTR)CAP_BB_COMM_STR},
    {CAP_BX_COMM,(STRPTR)CAP_BX_COMM_STR},
    {CAP_REQF_COMM,(STRPTR)CAP_REQF_COMM_STR},
    {CAP_REQD_COMM,(STRPTR)CAP_REQD_COMM_STR},
    {CAP_REQV_COMM,(STRPTR)CAP_REQV_COMM_STR},
    {CAP_REQT_COMM,(STRPTR)CAP_REQT_COMM_STR},
    {CAP_SURE_COMM,(STRPTR)CAP_SURE_COMM_STR},
    {CAP_COMMANDS,(STRPTR)CAP_COMMANDS_STR},
    {CAP_ARE_U_SURE,(STRPTR)CAP_ARE_U_SURE_STR},
    {CAP_YES_NO,(STRPTR)CAP_YES_NO_STR},
    {CAP_INFO,(STRPTR)CAP_INFO_STR},
    {CAP_OK,(STRPTR)CAP_OK_STR},
    {CAP_UNKNOWN,(STRPTR)CAP_UNKNOWN_STR},
    {CAP_NOACT,(STRPTR)CAP_NOACT_STR},
    {CAP_L_NEED,(STRPTR)CAP_L_NEED_STR},
    {CAP_L_CHOOSE,(STRPTR)CAP_L_CHOOSE_STR},
    {CAP_L_MEMORY,(STRPTR)CAP_L_MEMORY_STR},
    {CAP_L_NOTHING,(STRPTR)CAP_L_NOTHING_STR},
    {CAP_L_PROG,(STRPTR)CAP_L_PROG_STR},
    {CAP_L_PREP,(STRPTR)CAP_L_PREP_STR},
    {CAP_L_LEARN,(STRPTR)CAP_L_LEARN_STR},
    {CAP_L_STATUS,(STRPTR)CAP_L_STATUS_STR},
    {CAP_L_ACCEPT,(STRPTR)CAP_L_ACCEPT_STR},
    {CAP_L_DO_U,(STRPTR)CAP_L_DO_U_STR},
    {CAP_L_COMP,(STRPTR)CAP_L_COMP_STR},
    {CAP_L_ACCU,(STRPTR)CAP_L_ACCU_STR},
    {CAP_DIRECTORY,(STRPTR)CAP_DIRECTORY_STR},
    {CAP_H_DELC,(STRPTR)CAP_H_DELC_STR},
    {CAP_H_DELA,(STRPTR)CAP_H_DELA_STR},
    {CAP_H_ADDC,(STRPTR)CAP_H_ADDC_STR},
    {CAP_H_ADDA,(STRPTR)CAP_H_ADDA_STR},
    {CAP_H_LEARN,(STRPTR)CAP_H_LEARN_STR},
    {CAP_H_FILE,(STRPTR)CAP_H_FILE_STR},
    {CAP_H_COMM,(STRPTR)CAP_H_COMM_STR},
    {CAP_H_SAVE,(STRPTR)CAP_H_SAVE_STR},
    {CAP_H_USE,(STRPTR)CAP_H_USE_STR},
    {CAP_H_QUIT,(STRPTR)CAP_H_QUIT_STR},
    {CAP_SHAREWARE,(STRPTR)CAP_SHAREWARE_STR},
};

#endif /* CATCOMP_ARRAY */


/****************************************************************************/


#ifdef CATCOMP_BLOCK

static const char CatCompBlock[] =
{
    "\x00\x00\x00\x00\x00\x20"
    CAP_CONFIG_CORRUPT_STR "\x00\x00"
    "\x00\x00\x00\x01\x00\x3A"
    CAP_CONFIG_NOT_VALID_STR "\x00"
    "\x00\x00\x00\x02\x00\x10"
    CAP_GENERIC_STR "\x00"
    "\x00\x00\x00\x03\x00\x20"
    CAP_GENERIC_NOT_VALID_STR "\x00\x00"
    "\x00\x00\x00\x04\x00\x1C"
    CAP_DIR_NOT_VALID_STR "\x00\x00"
    "\x00\x00\x00\x05\x00\x04"
    CAP_ADD_STR "\x00"
    "\x00\x00\x00\x06\x00\x04"
    CAP_DEL_STR "\x00"
    "\x00\x00\x00\x07\x00\x06"
    CAP_SAVE_STR "\x00\x00"
    "\x00\x00\x00\x08\x00\x04"
    CAP_USE_STR "\x00"
    "\x00\x00\x00\x09\x00\x08"
    CAP_CLASSES_STR "\x00"
    "\x00\x00\x00\x0A\x00\x08"
    CAP_ACTIONS_STR "\x00"
    "\x00\x00\x00\x0B\x00\x0A"
    CAP_CLASSNAME_STR "\x00"
    "\x00\x00\x00\x0C\x00\x0A"
    CAP_MATCH_NAME_STR "\x00"
    "\x00\x00\x00\x0D\x00\x06"
    CAP_NAME_STR "\x00\x00"
    "\x00\x00\x00\x0E\x00\x06"
    CAP_EXEC_STR "\x00\x00"
    "\x00\x00\x00\x0F\x00\x06"
    CAP_LOAD_STR "\x00\x00"
    "\x00\x00\x00\x10\x00\x06"
    CAP_STACK_STR "\x00"
    "\x00\x00\x00\x11\x00\x06"
    CAP_COMM_STR "\x00\x00"
    "\x00\x00\x00\x12\x00\x06"
    CAP_COPY_STR "\x00\x00"
    "\x00\x00\x00\x13\x00\x02"
    CAP_UP_STR "\x00"
    "\x00\x00\x00\x14\x00\x02"
    CAP_DOWN_STR "\x00"
    "\x00\x00\x00\x15\x00\x08"
    CAP_CANCEL_STR "\x00\x00"
    "\x00\x00\x00\x16\x00\x06"
    CAP_DELAY_STR "\x00"
    "\x00\x00\x00\x17\x00\x18"
    CAP_SELECT_STR "\x00"
    "\x00\x00\x00\x18\x00\x10"
    CAP_NEW_CLASS_STR "\x00"
    "\x00\x00\x00\x19\x00\x12"
    CAP_NEW_ACTION_STR "\x00\x00"
    "\x00\x00\x00\x1A\x00\x1C"
    CAP_CANT_DELETE_STR "\x00"
    "\x00\x00\x00\x1B\x00\x14"
    CAP_DEL_CLASS_STR "\x00"
    "\x00\x00\x00\x1C\x00\x16"
    CAP_DEL_ACTION_STR "\x00\x00"
    "\x00\x00\x00\x1D\x00\x3E"
    CAP_UNABLE_STR "\x00\x00"
    "\x00\x00\x00\x1E\x00\x1A"
    CAP_F_COMM_STR "\x00"
    "\x00\x00\x00\x1F\x00\x2A"
    CAP_S_COMM_STR "\x00\x00"
    "\x00\x00\x00\x20\x00\x16"
    CAP_B_COMM_STR "\x00\x00"
    "\x00\x00\x00\x21\x00\x26"
    CAP_X_COMM_STR "\x00\x00"
    "\x00\x00\x00\x22\x00\x16"
    CAP_BF_COMM_STR "\x00"
    "\x00\x00\x00\x23\x00\x26"
    CAP_BS_COMM_STR "\x00\x00"
    "\x00\x00\x00\x24\x00\x12"
    CAP_BB_COMM_STR "\x00\x00"
    "\x00\x00\x00\x25\x00\x20"
    CAP_BX_COMM_STR "\x00"
    "\x00\x00\x00\x26\x00\x16"
    CAP_REQF_COMM_STR "\x00"
    "\x00\x00\x00\x27\x00\x1C"
    CAP_REQD_COMM_STR "\x00\x00"
    "\x00\x00\x00\x28\x00\x18"
    CAP_REQV_COMM_STR "\x00"
    "\x00\x00\x00\x29\x00\x16"
    CAP_REQT_COMM_STR "\x00"
    "\x00\x00\x00\x2A\x00\x1A"
    CAP_SURE_COMM_STR "\x00\x00"
    "\x00\x00\x00\x2B\x00\x0A"
    CAP_COMMANDS_STR "\x00\x00"
    "\x00\x00\x00\x2C\x00\x10"
    CAP_ARE_U_SURE_STR "\x00\x00"
    "\x00\x00\x00\x2D\x00\x08"
    CAP_YES_NO_STR "\x00\x00"
    "\x00\x00\x00\x2E\x00\x0C"
    CAP_INFO_STR "\x00"
    "\x00\x00\x00\x2F\x00\x06"
    CAP_OK_STR "\x00\x00"
    "\x00\x00\x00\x30\x00\x0E"
    CAP_UNKNOWN_STR "\x00"
    "\x00\x00\x00\x31\x00\x0A"
    CAP_NOACT_STR "\x00"
    "\x00\x00\x00\x32\x00\x26"
    CAP_L_NEED_STR "\x00"
    "\x00\x00\x00\x33\x00\x18"
    CAP_L_CHOOSE_STR "\x00\x00"
    "\x00\x00\x00\x34\x00\x20"
    CAP_L_MEMORY_STR "\x00"
    "\x00\x00\x00\x35\x00\x10"
    CAP_L_NOTHING_STR "\x00\x00"
    "\x00\x00\x00\x36\x00\x1A"
    CAP_L_PROG_STR "\x00\x00"
    "\x00\x00\x00\x37\x00\x1E"
    CAP_L_PREP_STR "\x00\x00"
    "\x00\x00\x00\x38\x00\x06"
    CAP_L_LEARN_STR "\x00"
    "\x00\x00\x00\x39\x00\x08"
    CAP_L_STATUS_STR "\x00\x00"
    "\x00\x00\x00\x3A\x00\x08"
    CAP_L_ACCEPT_STR "\x00\x00"
    "\x00\x00\x00\x3B\x00\x18"
    CAP_L_DO_U_STR "\x00\x00"
    "\x00\x00\x00\x3C\x00\x14"
    CAP_L_COMP_STR "\x00\x00"
    "\x00\x00\x00\x3D\x00\x18"
    CAP_L_ACCU_STR "\x00\x00"
    "\x00\x00\x00\x3E\x00\x0A"
    CAP_DIRECTORY_STR "\x00"
    "\x00\x00\x00\x3F\x00\x1C"
    CAP_H_DELC_STR "\x00\x00"
    "\x00\x00\x00\x40\x00\x1C"
    CAP_H_DELA_STR "\x00"
    "\x00\x00\x00\x41\x00\x12"
    CAP_H_ADDC_STR "\x00\x00"
    "\x00\x00\x00\x42\x00\x12"
    CAP_H_ADDA_STR "\x00"
    "\x00\x00\x00\x43\x00\x20"
    CAP_H_LEARN_STR "\x00"
    "\x00\x00\x00\x44\x00\x10"
    CAP_H_FILE_STR "\x00\x00"
    "\x00\x00\x00\x45\x00\x1E"
    CAP_H_COMM_STR "\x00"
    "\x00\x00\x00\x46\x00\x22"
    CAP_H_SAVE_STR "\x00\x00"
    "\x00\x00\x00\x47\x00\x20"
    CAP_H_USE_STR "\x00"
    "\x00\x00\x00\x48\x00\x20"
    CAP_H_QUIT_STR "\x00\x00"
    "\x00\x00\x00\x49\x00\x9E"
    CAP_SHAREWARE_STR "\x00\x00"
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
