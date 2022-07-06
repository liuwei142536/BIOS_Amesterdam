//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2010, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
// $Archive: /Alaska/BIN/Modules/AMITSE2_0/AMITSE/PasswordEncode/PasswordEncode.h $
//
//*****************************************************************//
/** @file PasswordEncode.h
    Header file for code to handle password operations

**/

#ifndef _PASSWORDENCODE_H_
#define	_PASSWORDENCODE_H_

#ifdef TSE_FOR_APTIO_4_50
#include "Token.h"
#else
#include "tokens.h"
#endif

#if TSE_USE_EDK_LIBRARY
#include "Tiano.h" 
#else
#include "Efi.h" 
#endif


#if TSE_HASH_PASSWORD
#include "Protocol/Hash.h"
#if AMITSE_CryptoPkg_SUPPORT
#if TSE_USE_EDK_LIBRARY
#include "Library/BaseCryptLib.h"
#else
#include "CryptoPkg/Include/Library/BaseCryptLib.h"
#endif
#endif
#if AMITSE_AmiCryptoPkg_SUPPORT
#include "AmiCryptoPkg/Include/Library/BaseCryptLib.h"
#endif
#endif

///////////////////////////////////////////////////////
//		EXTERN FUNCTIONS AND VARIABLES
//////////////////////////////////////////////////////
extern BOOLEAN IsPasswordSupportNonCaseSensitive ();
extern BOOLEAN IsHashingSupported (VOID);
extern BOOLEAN EfiCompareGuid (IN EFI_GUID *Guid1,IN EFI_GUID *Guid2);
extern VOID * EfiLibAllocateZeroPool (IN UINTN AllocationSize);
extern VOID MemCopy( VOID *dest, VOID *src, UINTN size );
extern VOID MemSet( VOID *buffer, UINTN size, UINT8 value );

BOOLEAN
EFIAPI
Sha1Duplicate (
  IN   CONST VOID  *Sha1Context,
  OUT  VOID        *NewSha1Context
  );

#endif /* _PASSWORDENCODE_H_ */


//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2010, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**     5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
