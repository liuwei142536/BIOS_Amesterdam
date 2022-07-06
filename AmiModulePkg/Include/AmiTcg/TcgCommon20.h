/*++

   Copyright (c) 2005 Intel Corporation. All rights reserved
   This software and associated documentation (if any) is furnished
   under a license and may only be used or copied in accordance
   with the terms of the license. Except as permitted by such
   license, no part of this software or documentation may be
   reproduced, stored in a retrieval system, or transmitted in any
   form or by any means without the express written consent of
   Intel Corporation.


   Module Name:

   TcgCommon.h

   Abstract:

   Header file for TcgCommon.c

   --*/
//*************************************************************************
// $Header: /Alaska/SOURCE/Modules/TCG/TcgCommon.h 13    3/19/12 6:14p Fredericko $
//
// $Revision: 13 $
//
// $Date: 3/19/12 6:14p $
//*************************************************************************
//*************************************************************************
//<AMI_FHDR_START>
//
// Name:  TcgCommon.h
//
// Description:
//  Header file for TcgCommon.c
//
//<AMI_FHDR_END>
//*************************************************************************
#ifndef _TCG_COMMON_H_
#define _TCG_COMMON_H_

#include <Uefi.h>
#include "AmiTcg/TcgPc.h"
#include "AmiTcg/TcgEfiTpm.h"
#include "AmiTcg/sha.h"
#include "AmiTcg/Tpm20.h"
#include <Hob.h>
#include "TCGMisc.h"

#define TcgCommonN2HS( v16 ) TcgCommonH2NS( v16 )
#define TcgCommonN2HL( v32 ) TcgCommonH2NL( v32 )
#define EFI_TPL_DRIVER                 6
#define EFI_TPL_APPLICATION         4
#define EFI_TPL_CALLBACK            8
#define EFI_TPL_NOTIFY              16
#define EFI_TPL_HIGH_LEVEL          31



#define TCGGET_HOB_TYPE( Hob )     ((Hob).Header->HobType)
#define TCGGET_HOB_LENGTH( Hob )   ((Hob).Header->HobLength)
#define TCGGET_NEXT_HOB( Hob )     ((Hob).Raw + TCGGET_HOB_LENGTH( Hob ))
#define TCGEND_OF_HOB_LIST( Hob )  (TCGGET_HOB_TYPE( Hob ) ==\
                                 EFI_HOB_TYPE_END_OF_HOB_LIST)

extern
UINT16
EFIAPI TcgCommonH2NS (
    IN UINT16 Val );

extern
UINT32
EFIAPI TcgCommonH2NL (
    IN UINT32 Val );

VOID* GetHob (
    IN UINT16 Type,
    IN VOID   *HobStart  );

EFI_STATUS TcgGetNextGuidHob(
    IN OUT VOID          **HobStart,
    IN EFI_GUID          * Guid,
    OUT VOID             **Buffer,
    OUT UINTN            *BufferSize OPTIONAL );

extern
VOID
EFIAPI Tpm20TcgCommonCopyMem (
    IN VOID  *CallbackContext,
    OUT VOID *Dest,
    IN VOID  *Src,
    IN UINTN Len );

extern
EFI_STATUS
EFIAPI Tpm20SHA1HashAll (
    IN VOID            *CallbackContext,
    IN VOID            *HashData,
    IN UINTN           HashDataLen,
    OUT UINT8          *Digest
);

EFI_STATUS EfiLibGetSystemConfigurationTable(
    IN EFI_GUID *TableGuid,
    IN OUT VOID **Table );

extern
BOOLEAN
EFIAPI IsTcmSupportType (
);

#pragma pack(push,1)

typedef struct
{
    TPMI_DH_PCR              pcrHandle;
    TPML_DIGEST_VALUES       DigestValue;
} PCR_Extend_In;

typedef struct
{
    TPMI_ST_COMMAND_TAG       Tag;
    UINT32                    CommandSize;
    TPM_CC                    CommandCode;
    TPMI_DH_PCR               PcrHandle;
    UINT32                    AuthorizationSize;
    TPMS_AUTH_COMMAND         AuthSessionPcr;
    TPML_DIGEST_VALUES        DigestValues;
} TPM2_PCRExtend_cmd_t;

typedef struct
{
    TPMI_ST_COMMAND_TAG         Tag;
    UINT32                      RespondSize;
    TPM_RC                      ResponseCode;
    UINT32                      parameterSize;
    TPMS_AUTH_RESPONSE  pwapAuth;
} TPM2_PCRExtend_res_t;


typedef struct
{
    TPM_ST tag;
    UINT32 paramSize;
    TPM_CC commandCode;
} INT_TPM2_COMMAND_HEADER;

typedef struct
{
    TPM_ST tag;
    UINT32 paramSize;
    TPM_RC responseCode;
} INT_TPM2_RESPONSE_HEADER;

typedef struct
{
    INT_TPM2_COMMAND_HEADER       Header;
    TPMI_RH_HIERARCHY             AuthHandle;
    UINT32                        AuthorizationSize;
    TPMS_AUTH_COMMAND             AuthSession;
    TPMI_RH_HIERARCHY             Hierarchy;
    TPMI_YES_NO                   State;
} AMI_TPM2_HIERARCHY_CONTROL_COMMAND;

typedef struct
{
    INT_TPM2_RESPONSE_HEADER      Header;
    UINT32                        ParameterSize;
    TPMS_AUTH_RESPONSE            AuthSession;
} AMI_TPM2_HIERARCHY_CONTROL_RESPONSE;

EFI_STATUS
EFIAPI SHA2HashAll(
    IN  VOID            *CallbackContext,
    IN  VOID            *HashData,
    IN  UINTN           HashDataLen,
    OUT UINT8           *Digest
);

typedef UINT32        TCG_PCRINDEX;

#if MDE_PKG_VERSION <15
typedef struct  
{
    TCG_PCRINDEX         PCRIndex;
    UINT32               EventType;
    TPML_DIGEST_VALUES   Digests;
    UINT32               EventSize;
} TCG_PCR_EVENT2_HDR;
#endif

#pragma pack(pop)


#endif
