//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file hiistring.h
    Header file for code to handle the hii sting operations

**/

#ifndef _AMIHIISTRING_H_	// DO NOT CHANGE THIS TO _HII_H_ !!  That is used by the HII Protocol header
#define	_AMIHIISTRING_H_

#ifdef TSE_FOR_APTIO_4_50
#include "Token.h" 
#else
#include "tokens.h"
#endif

#define	INVALID_HANDLE	((VOID*)(UINTN)-1)
#define	INVALID_TOKEN	((UINT16)-1)


UINT16 HiiAddString( /*EFI_HII_HANDLE*/VOID* handle, CHAR16 *string );
UINT16 HiiChangeStringLanguage( /*EFI_HII_HANDLE*/VOID * handle, UINT16 token, CHAR16 *lang, CHAR16 *string );
CHAR16 *HiiGetStringLanguage( /*EFI_HII_HANDLE*/VOID * handle, UINT16 token, CHAR16 *lang );
CHAR16 *GetVariableNameByID( UINT32 VariableID );
CHAR16 *GetGUIDNameByID( UINT32 VariableID );

UINTN HiiFindStrPrintBoundary ( IN CHAR16   *String, IN UINTN PrintWidth );
UINTN IsCharWide( CHAR16 strWide);

CHAR16 *SkipEscCode(CHAR16 *String);
CHAR16 *GetLanguageVariableName();
UINTN HiiGetGlyphWidth(VOID);
UINTN HiiGetGlyphHeight(VOID);
EFI_STATUS GetUnicodeCollection2Protocol(VOID **Protocol);
EFI_STATUS GetUnicodeCollectionProtocol(VOID **Protocol);
EFI_STATUS InitUnicodeCollectionProtocol(VOID **Protocol);
BOOLEAN MetaiMatch(VOID *Protocol,IN CHAR16 *String,IN CHAR16 *Pattern);
INTN StringColl(VOID *Protocol,IN CHAR16 *String1,IN CHAR16 *String2);

#endif /* _AMIHIISTRING_H_ */
