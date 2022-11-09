//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#ifndef __JSON_CAPSULE_VERIFICATION__H__
#define __JSON_CAPSULE_VERIFICATION__H__
#ifdef __cplusplus
extern "C" {
#endif

#include "JsonConfigCapsule.h"
#include "AmiCertificate.h"
#include "Guid/WinCertificate.h"
#include "Guid/ImageAuthentication.h"
#include "Protocol/AmiDigitalSignature.h"


typedef enum { HASHALG_SHA1, HASHALG_SHA256, HASHALG_SHA384, HASHALG_SHA512, HASHALG_MAX} HASHALG;
//
// SHA-1 digest size in bytes.
//
#define SHA1_DIGEST_SIZE    20
//
// SHA-256 digest size in bytes
//
#define SHA256_DIGEST_SIZE  32
//
// SHA-384 digest size in bytes
//
#define SHA384_DIGEST_SIZE  48
//
// SHA-512 digest size in bytes
//
#define SHA512_DIGEST_SIZE  64

// Set max digest size as SHA512 Output (64 bytes) by far
//
#define MAX_DIGEST_SIZE    SHA512_DIGEST_SIZE

typedef struct {
    EFI_FFS_FILE_HEADER FfsHdr;
    EFI_COMMON_SECTION_HEADER SecHdr;
    EFI_GUID            SectionGuid;
    UINT8               FwCapHdr[0];
} AMI_FFS_COMMON_SECTION_HEADER;


typedef enum {
    CertUndefined,
    CertSha256,
    CertSha384,
    CertSha512,
    CertX509,
    CertX509Sha256,
    CertX509Sha384,
    CertX509Sha512,
    CertTypeMax
} nCertType;



extern EFI_STATUS EFIAPI AmiTimeToSec(EFI_TIME *EfiTime, UINT64 *t);
extern INTN MemCmp(VOID* pDestination, VOID* pSource, UINTN Count);
extern VOID * EFIAPI ZeroMem (  OUT VOID  *Buffer,  IN UINTN  Length  );


/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2010, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
