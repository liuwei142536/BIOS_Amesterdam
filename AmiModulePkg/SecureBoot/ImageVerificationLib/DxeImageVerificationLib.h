//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
#ifndef __IMAGEVERIFICATIONLIB_H__
#define __IMAGEVERIFICATIONLIB_H__

#include <Protocol/DevicePath.h>
#include <Library/SecurityManagementLib.h>

#include <Library/MemoryAllocationLib.h>
#include <Protocol/Hash.h>

//#define EFI_CERT_TYPE_RSA2048_SHA256_SIZE 256
//#define EFI_CERT_TYPE_RSA2048_SIZE        256
//#define EFI_CERT_TYPE_SHA256_SIZE         32
#define MAX_NOTIFY_STRING_LEN             64

//
// Image type definitions
//
#define IMAGE_UNKNOWN                         0x00000000
#define IMAGE_FROM_FV                         0x00000001
#define IMAGE_FROM_OPTION_ROM                 0x00000002
#define IMAGE_FROM_REMOVABLE_MEDIA            0x00000003
#define IMAGE_FROM_FIXED_MEDIA                0x00000004

//
// Authorization policy bit definition
//
#define ALWAYS_EXECUTE                         0x00000000
#define NEVER_EXECUTE                          0x00000001
#define ALLOW_EXECUTE_ON_SECURITY_VIOLATION    0x00000002
#define DEFER_EXECUTE_ON_SECURITY_VIOLATION    0x00000003
#define DENY_EXECUTE_ON_SECURITY_VIOLATION     0x00000004
#define QUERY_USER_ON_SECURITY_VIOLATION       0x00000005

//
// Support hash types
//
//#define HASHALG_SHA1                           0x00000000
//#define HASHALG_SHA224                         0x00000001
//#define HASHALG_SHA256                         0x00000002
//#define HASHALG_SHA384                         0x00000003
//#define HASHALG_SHA512                         0x00000004
//#define HASHALG_MAX                            0x00000005
typedef enum { HASHALG_SHA1, HASHALG_SHA256, HASHALG_SHA384, HASHALG_SHA512, HASHALG_MAX} HASHALG;

#define MAX_ELEM_NUM        28 // ~28 max number of PE hdr elements to be hashed in one pass
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

#define ALIGNMENT_SIZE                    8
#define ALIGN_SIZE(a) (((a) % ALIGNMENT_SIZE) ? ALIGNMENT_SIZE - ((a) % ALIGNMENT_SIZE) : 0)

#ifndef OFFSET_OF
#define OFFSET_OF(TYPE, Field) ((UINTN) &(((TYPE *)0)->Field))
#endif

//defined for every CPU architecture
// Maximum legal IA-32 address
#ifndef EFI_MAX_ADDRESS
#define EFI_MAX_ADDRESS   0xFFFFFFFF
#endif

//
//
// PKCS7 Certificate definition
//
typedef struct _WIN_CERTIFICATE_EFI_PKCS {
  WIN_CERTIFICATE Hdr;
  UINT8           CertData[1];
} WIN_CERTIFICATE_EFI_PKCS;

#ifndef AMI_BDS_CONNECT_DRIVERS_PROTOCOL_GUID
#define AMI_BDS_CONNECT_DRIVERS_PROTOCOL_GUID \
    { 0x3aa83745, 0x9454, 0x4f7a, { 0xa7, 0xc0, 0x90, 0xdb, 0xd0, 0x2f, 0xab, 0x8e } }
#endif

//
// Definitions for Efi System table entry to store the location within DB variable 
// of the Trusted Certificate that was used to verify Efi OS BootLoader image 
// The table is consumed by AmiTcg service to extend Measurement of DB to PCR7
//6683D10C-CF6E-4914-B5B4-AB8ED7370ED7
#define AMI_VALID_BOOT_IMAGE_CERT_TBL_GUID \
    { 0x6683D10C, 0xCF6E, 0x4914, 0xB5, 0xB4, 0xAB, 0x8E, 0xD7, 0x37, 0x0E, 0xD7 }
//
//
// Data Table definition
//
typedef struct _AMI_VALID_CERT_IN_SIG_DB {
  UINT32          SigOffset;
  UINT32          SigLength;
} AMI_VALID_CERT_IN_SIG_DB;

#endif
