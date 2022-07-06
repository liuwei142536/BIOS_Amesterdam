//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license
// agreement with Intel or your vendor.  This file may be
// modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c) 2007 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  LtPeiLib.h

Abstract:

  This file contains function definitions that can determine
  the LT capabilities of a platform during PEI and perform
  certain specific platform tasks that are required for LT
  during PEI.

--*/

#ifndef _LT_PEI_LIB_H_
#define _LT_PEI_LIB_H_

#include <Guid\PlatformTxt.h>
#include <Guid/SetupVariable.h>
#include <Ppi\CpuIo.h>
#include <Ppi\Stall.h>
#include <Ppi/MpService.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <CpuBaseLib.h>
#include <Ppi/TxtSclean.h>
#include <Library/HobLib.h>
#include <Ppi/MasterBootMode.h>
#include <Ppi\PciCfg2.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/LocalApicLib.h>

#define MKF_TPM_PRESENT
#define EFI_CPUID_VERSION_INFO                0x1
#define EFI_MSR_LT_CONTROL                    0x2E7

//#define EFI_MSR_IA32_FEATURE_CONTROL    0x3A
#define FEATURE_LOCK_BIT                0x0001
#define FEATURE_VMON_IN_SMX             0x0002
#define FEATURE_VMON_OUT_SMX            0x0004
#define FEATURE_SENTER_ENABLE           0xFF00

extern EFI_GUID   gPeiPostScriptTablePpiGuid;
extern EFI_GUID   gPeiTxtMemoryUnlockedPpiGuid;
extern EFI_GUID   gSmmTxtConfigLockGuid;

typedef struct _LT_PEI_LIB_CONTEXT_ {
  CONST EFI_PEI_SERVICES           **PeiServices;
  EFI_PEI_CPU_IO_PPI               *CpuIoPpi;
  EFI_PEI_PCI_CFG2_PPI             *PciCfgPpi;
  EFI_PEI_STALL_PPI                *PeiStall;
  PEI_MP_SERVICES_PPI              *PeiMpServices;
  EFI_PLATFORM_TXT_DEVICE_MEMORY   *PlatformTxtDeviceMemory;
  EFI_PLATFORM_TXT_POLICY_DATA     *PlatformTxtPolicyData;
  UINT8                            ApVector;
  UINT8                            ApCount;
} LT_PEI_LIB_CONTEXT ;

//
// Structure used to hold Socket information for S3 resume
//
typedef struct {
  UINT8     SocketIndex;
  UINT8     SocketFoundId;
  UINT16    ApApicId;
} LT_PEI_SOCKET_INFO;

EFI_STATUS
InitializeLtPeiLib (
  IN      CONST EFI_PEI_SERVICES   **PeiServices,
  IN OUT  LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

BOOLEAN
IsLtProcessor (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

VOID
EnableTxt (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

VOID
LockLtMsr (
  IN      VOID                    *Buffer
  );

VOID
LockLtMsrOnAll(
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

BOOLEAN
IsLtSxProcessor (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

BOOLEAN
IsTpmPresent (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

BOOLEAN
IsLtEstablished (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

BOOLEAN
IsLtWakeError (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

BOOLEAN
IsLtResetStatus (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

BOOLEAN
IsLtEnabled (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

VOID
PowerCycleReset (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

EFI_STATUS
DoClearSecrets (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

EFI_STATUS
DoSclean (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );
  
EFI_STATUS
PeiTxtLockConfigForAll (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

EFI_STATUS
PeiDoLockConfigForAll (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

VOID
DoLockConfig (
  IN      VOID                    *Buffer
  );
  
VOID
EFIAPI
DoLockConfigOnAP (
  IN      VOID                    *Buffer
  );

#endif
