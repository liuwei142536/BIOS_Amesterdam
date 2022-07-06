//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file
  AmiSecureBootLib.h: helper library for Secure Boot configuration services
  The internal header file includes the common header files, defines
  internal structure and functions used by SecureBoot module.
**/
#ifndef _SECUREBOOT_LIB_H_
#define _SECUREBOOT_LIB_H_

#include <AmiCertificate.h>
#include <Guid/GlobalVariable.h>
#include <Guid/ImageAuthentication.h>

//############################### 
//
// AMI_EFI_VARIABLE_AUTHENTICATION_2 descriptor
//
// A time-based authentication method descriptor template
//
//###############################
#pragma pack(1)
#ifndef AMI_EFI_VARIABLE_AUTHENTICATION_2
typedef struct {
    EFI_TIME                            TimeStamp;
    WIN_CERTIFICATE_UEFI_GUID_1         AuthInfo;
} AMI_EFI_VARIABLE_AUTHENTICATION_2;
#endif
typedef struct {
    AMI_EFI_VARIABLE_AUTHENTICATION_2   AuthHdr;
    EFI_SIGNATURE_LIST                  SigList;
    EFI_SIGNATURE_DATA                  SigData;
} EFI_VARIABLE_SIG_HDR_2;
#pragma pack()

#define EFI_CERT_TYPE_RSA2048_SIZE        256
#define EFI_CERT_TYPE_SHA256_SIZE         32
#define EFI_CERT_TYPE_CERT_X509_SHA256_GUID_SIZE        48
#define EFI_CERT_TYPE_CERT_X509_SHA384_GUID_SIZE        64
#define EFI_CERT_TYPE_CERT_X509_SHA512_GUID_SIZE        80

// SetVars
#define SET_SECURE_VARIABLE_DEL    1
#define RESET_NV_KEYS              1

#define SET_SECURE_VARIABLE_SET    2
#define SET_NV_DEFAULT_KEYS        2

#define SET_SECURE_VARIABLE_SET_RT 4
#define SET_RT_DEFAULT_KEYS        4

#define SET_SECURE_VARIABLE_APPEND 8

// Bad Certificate list
typedef struct {
    CHAR8 *SubjectName;
    EFI_GUID OwnerGuid;
} SDL_X509_SIG_ENTRY;

//SecureBoot Defaults provisioning

typedef enum {
    DbxVar=0,
    DbtVar,
    DbrVar,
    DbVar,
    KekVar,
    PkVar,
    MaxVarCount
} SECURE_BOOT_POLICY_VARIABLES;

#define SecureBootPolicyVariableCount MaxVarCount
#define SecureVariableSigDatabaseCount KekVar

extern EFI_GUID *SecureVariableFileGuid[];
extern CHAR16* SecureVariableFileName[];
extern CHAR16* SecureVariableFileNameDefault[];

//###############################
// Library function prototypes
//###############################
EFI_STATUS EFIAPI
AmiSetSecureBootVariable(
    IN UINT16   Action,
    IN CHAR16   *SecureVariableName,
    IN CHAR16   *SecureVariableNameDefault OPTIONAL,
    IN EFI_GUID *SecureVariableGuid,
    IN UINT8    *pVarBuffer,
    IN UINTN    VarSize
);
EFI_STATUS EFIAPI
AmiInstallSecureBootVariables (
    IN UINT16    Action,
    IN UINT16    SecureVariableSigDatabaseCount,
    IN CHAR16   *SecureVariableNameList[],
    IN CHAR16   *SecureVariableNameListDefault[] OPTIONAL,
    IN EFI_GUID *SecureVariableFileGuid[]
);
EFI_STATUS EFIAPI
AmiInstallSecureBootDefaults (
    IN UINT16    Action
);
VOID EFIAPI
AmiFillAuthenticatedHdr2(
    UINT8*  pVar,
    UINT8   VarSetMode
);
EFI_STATUS EFIAPI
AmiFillAuthenticatedVariableHdr(
    UINT8 *pVar,
    UINT8 *pCert,
    UINTN CertSize,
    UINT8 VarSetMode
);
EFI_STATUS EFIAPI
AmiFillAuthenticatedVariableHdr2(
    UINT8 *pVar,
    UINT8 *pCert,
    UINTN CertSize,
    EFI_GUID *pOwnerGuid,
    UINT8 VarSetMode
);
EFI_STATUS EFIAPI
AmiValidateSignatureList (
    IN VOID         *Data,
    IN UINTN        DataSize,
    IN OUT UINTN    *SigCertCount OPTIONAL,
    IN OUT UINTN    *SigListCount OPTIONAL
);
EFI_STATUS EFIAPI
AmiFindCertificateInSignatureList( 
    IN SDL_X509_SIG_ENTRY *FindCertEntry,
    IN VOID      *bufIn,
    IN UINTN     bufSize,
    IN OUT EFI_SIGNATURE_LIST **SigListOut OPTIONAL
);
EFI_STATUS EFIAPI
AmiRemoveCertificateFromSignatureList(
    SDL_X509_SIG_ENTRY *LookUpCertEntry,
    IN VOID         *bufIn,
    IN OUT UINTN    *bufSize
);
EFI_STATUS EFIAPI
AmiRemoveBadCertificates(
    IN SDL_X509_SIG_ENTRY *BadCertificateList OPTIONAL,
    IN VOID         *VarBuf,
    IN OUT UINTN    *VarSize
);
EFI_STATUS EFIAPI
AmiTimeToSec(EFI_TIME *EfiTime, UINT64 *t);

#endif
