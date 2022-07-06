/*++
Copyright (c) 2014 - 2021 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  @file SpsSmm.h

Abstract:

  Structures and macros for SPS SMM driver
**/

#ifndef SPSSMM_H_
#define SPSSMM_H_

#include <AmiDxeLib.h> // AptioV Server Override

#define HECI_RSP_STATUS_SUCCESS       0x0

//
// HECIMBAR register definition
//
#define H_CB_WW   0x00
#define H_CSR     0x04
#define ME_CB_RW  0x08
#define ME_CSR_HA 0x0C

///
/// H_CSR - Host Control Status
///
typedef union {
  UINT32  ul;
  struct {
    UINT32  H_IE     : 1;  ///< 0 - Host Interrupt Enable ME
    UINT32  H_IS     : 1;  ///< 1 - Host Interrupt Status ME
    UINT32  H_IG     : 1;  ///< 2 - Host Interrupt Generate
    UINT32  H_RDY    : 1;  ///< 3 - Host Ready
    UINT32  H_RST    : 1;  ///< 4 - Host Reset
    UINT32  Reserved : 3;  ///< 7:5 - Reserved
    UINT32  H_CBRP   : 8;  ///< 15:8 - Host CB Read Pointer
    UINT32  H_CBWP   : 8;  ///< 23:16 - Host CB Write Pointer
    UINT32  H_CBD    : 8;  ///< 31:24 - Host Circular Buffer Depth
  } r;
} HECI_HOST_CONTROL_REGISTER;

#pragma pack(1)
typedef struct
{
  HECI_MSG_HEADER Heci;
  MKHI_MSG_HEADER Mkhi;
  UINT64          Nonce;
} HECI_MSG_HMRFPO_ENABLE_REQ;

typedef struct
{
  HECI_MSG_HEADER Heci;
  MKHI_MSG_HEADER Mkhi;
  UINT32          FactoryBase;
  UINT32          FactoryLimit;
  UINT8           Status;
  UINT8           Reserved[3];
} HECI_MSG_HMRFPO_ENABLE_RSP;
#pragma pack()

#define HECI_MSG_ENABLE_REQ_MKHI_HDR  0x00000105
#define HECI_MSG_ENABLE_RSP_MKHI_HDR  0x00008105

#pragma pack(1)
typedef struct
{
  HECI_MSG_HEADER Heci;
  MKHI_MSG_HEADER Mkhi;
  UINT64          Nonce;
} HECI_MSG_HMRFPO_MERESET_REQ;

typedef struct
{
  HECI_MSG_HEADER Heci;
  MKHI_MSG_HEADER Mkhi;
  UINT32          FactoryBase;
  UINT8           Status;
  UINT8           Reserved[3];
} HECI_MSG_HMRFPO_MERESET_RSP;
#pragma pack()

#define HECI_MSG_MERESET_REQ_MKHI_HDR  0x00000005
#define HECI_MSG_MERESET_RSP_MKHI_HDR  0x00008005

#pragma pack(1)
typedef struct
{
  HECI_MSG_HEADER Heci;
  MKHI_MSG_HEADER Mkhi;
} HECI_MSG_HMRFPO_LOCK_REQ;

typedef struct
{
  HECI_MSG_HEADER Heci;
  MKHI_MSG_HEADER Mkhi;
  UINT64          Nonce;
  UINT32          FactoryBase;
  UINT32          FactoryLimit;
  UINT8           Status;
  UINT8           Reserved[3];
} HECI_MSG_HMRFPO_LOCK_RSP;
#pragma pack()

#define HECI_MSG_LOCK_REQ_MKHI_HDR  0x00000205
#define HECI_MSG_LOCK_RSP_MKHI_HDR  0x00008205

#pragma pack(1)
typedef struct
{
  HECI_MSG_HEADER Heci;
  MKHI_MSG_HEADER Mkhi;
} HECI_MSG_MKHI_GET_FW_VERSION_REQ;

typedef struct
{
  HECI_MSG_HEADER Heci;
  MKHI_MSG_HEADER Mkhi;
  MKHI_FW_VERSION Act;
  MKHI_FW_VERSION Rcv;
  MKHI_FW_VERSION Bkp;
} HECI_MSG_MKHI_GET_FW_VERSION_RSP;
#pragma pack()

#define HECI_MSG_GET_FW_VERSION_REQ_MKHI_HDR   0x000002FF
#define HECI_MSG_GET_FW_VERSION_RSP_MKHI_HDR   0x000082FF

EFI_STATUS
HeciReq_HmrfpoLock(
  IN EFI_SMM_HECI_PROTOCOL *pSmmHeci,
  IN OUT UINT64            *Nonce
  );

EFI_STATUS
HeciReq_HmrfpoEnable(
  IN EFI_SMM_HECI_PROTOCOL *pSmmHeci,
  IN UINT64                Nonce
  );

EFI_STATUS
HeciReq_HmrfpoReset (
  IN EFI_SMM_HECI_PROTOCOL *pSmmHeci,
  IN UINT64                Nonce
  );

void
SetMeVersions(
  IN     EFI_SMM_HECI_PROTOCOL   *pSmmHeci,
  IN OUT SPS_SMM_HMRFPO_PROTOCOL *pSpsSmmHmrfpo
  );

EFI_STATUS
GetSpsConfiguration(
  IN EFI_SYSTEM_TABLE       *SystemTable,
  OUT EFI_HOB_TYPE_SPS_INFO *pHob
  );


EFI_STATUS
Hmrfpo_Lock (void);

EFI_STATUS
Hmrfpo_MeReset (void);

EFI_STATUS
Hmrfpo_Enable (void);


#endif /* SPSSMM_H_ */
