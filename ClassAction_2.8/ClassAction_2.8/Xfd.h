/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : Xfd.h

	Goal : Declare Needed structure for 
		xfdmaster.library

	Ver  : 1.00 Converted from Assembler
		into C by Salim Gasmi

****************************************/

#include <exec/types.h>

struct Buffer 
{
	APTR	xfdbi_SourceBuffer	; 
	ULONG	xfdbi_SourceBufLen	; 
	APTR	xfdbi_Slave		; 
	APTR	xfdbi_PackerName	;
	UWORD	xfdbi_PackerFlags	;
	UWORD	xfdbi_Error		;
	APTR	xfdbi_TargetBuffer	;
	ULONG	xfdbi_TargetBufMemType	;
	ULONG	xfdbi_TargetBufLen	;
	ULONG	xfdbi_TargetBufSaveLen	;
	ULONG	xfdbi_DecrAddress	;
	ULONG	xfdbi_JmpAddress	;
	APTR	xfdbi_Special		;
};

