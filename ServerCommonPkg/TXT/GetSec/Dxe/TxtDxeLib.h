//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c) 2007 - 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  TxtDxeLib.h

Abstract:

  This file contains function definitions that can determine
  the LT capabilities of a platform during DXE and perform
  certain specific platform tasks that are required for LT
  during DXE.

--*/

#ifndef _LT_DXE_LIB_H_
#define _LT_DXE_LIB_H_

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol\MpService.h>
#include <Guid\PlatformTxt.h>
#include <Guid\SetupVariable.h>
#include <Guid\HobList.h>
#include <Guid\PlatformInfo.h>
#include <Protocol/CpuIo.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/LoadedImage.h>
#include <Protocol\PciRootBridgeIo.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/LegacyBios.h>
#include <Protocol/LegacyRegion2.h>
#include <Library\S3BootScriptLib.h>
#include <Library/BaseLib.h>
#include <Library/CpuConfigLib.h>
#include <Library/PcdLib.h>
#include <Protocol\ReadyForLock.h>
#include <Protocol/LegacyRegion.h>
#include <Protocol\PciCallback.h>
#include <Protocol/IioSystem.h>
#include <Library/DxeServicesTableLib.h>

#define MKF_TPM_PRESENT

#define EFI_CPUID_VERSION_INFO                0x1
#define EFI_MSR_IA32_FEATURE_CONTROL          0x0000003A
#define EFI_MSR_LT_CONTROL                    0x2E7
#define LT_SPAD_HIGH                          0xFED300A4
#define   B_EFI_MSR_LT_CONTROL_LOCK         BIT0
//
// Chispet register
//
#define SA_DPR_REG                          0x290 
#define   SA_DPR_LOCK                       0x1
#define   SA_DPR_PRM                        0x4

#define LEGACY_BIOS_MEMORY_SEGMENT          0x0F0000
#define LEGACY_BIOS_MEMORY_LENGTH           0x010000

//AptioV server override start: dynamic MMCFG base support
#ifndef DYNAMIC_MMCFG_BASE_FLAG 
//#define PcdPciExpressBaseAddress            PCIEX_BASE_ADDRESS

#define MmPciAddress(Bus, Device, Function, Register) \
      ( \
        (UINTN) PcdGet64 (PcdPciExpressBaseAddress) + (UINTN) (Bus << 20) + (UINTN) (Device << 15) + (UINTN) \
          (Function << 12) + Register \
      )
#endif
//AptioV server override end: dynamic MMCFG base support

//
// define PCI address macro for reg > 0xFF
//
#define EFI_PCI_ADDRESS_EXT(bus, dev, func, reg) \
    ((UINT64) ((((UINTN) reg) << 32) + (((UINTN) bus) << 24) + (((UINTN) dev) << 16) + (((UINTN) func) << 8)))

//
// Define the TPM Decode and TXT Public and Private spaces:
// - TPM Decode  0xFED40000 - 0xFED44FFF
// - TXT Public  0xFED30000 - 0xFED3FFFF
// - TXT Private 0xFED20000 - 0xFED2FFFF
//
#define TXT_PUBLIC_PRIVATE_BASE  0xFED20000   // TPM Decode and TXT Public/Private reg address
#define TXT_PUBLIC_PRIVATE_SIZE  0x25000      // TPM Decode and TXT Public/Private reg size
    
extern  EFI_RUNTIME_SERVICES          *gRT;

typedef struct _LT_DXE_LIB_CONTEXT_ {    
  EFI_HANDLE                          ImageHandle;
  EFI_SYSTEM_TABLE                    *SystemTable;
  EFI_CPU_IO_PROTOCOL                 *CpuIo;
  EFI_MP_SERVICES_PROTOCOL            *MpService;
  EFI_PLATFORM_TXT_DEVICE_MEMORY      *PlatformTxtDeviceMemory;
  EFI_PLATFORM_TXT_POLICY_DATA        *PlatformTxtPolicyData;
  UINT8                               ApVector;
  UINT8                               ApCount;
  UINT32                              ApicId[MAX_CPU_NUM];
} LT_DXE_LIB_CONTEXT ;

// Aptio V Server Override Start : TXT Error and status Register message printing.
#define TXT_ACM_STATUS_LO  0xFED300A0
#define TXT_ACM_STATUS_HI  0xFED300A4
#define TXT_ACM_ERRORCODE  0xFED30030
#define TXT_BIOS_ACM_ERRORCODE  0xFED30328

// Aptio V Server Override Start : TXT Error and status Register message printing.
VOID PrintLtsxErrors(
);
// Aptio V Server Override End : TXT Error and status Register message printing.
// Aptio V Server Override Enabd: TXT Error and status Register message printing.

EFI_STATUS
InitializeLtDxeLib (        
  IN      EFI_HANDLE              ImageHandle,
  IN      EFI_SYSTEM_TABLE        *SystemTable,
  IN OUT  LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

EFI_STATUS
DisableLtsxAndReset (
  VOID
  );

BOOLEAN
IsLtEstablished ( 
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

BOOLEAN
IsLtProcessor (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

BOOLEAN
IsLtEnabled ( 
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

BOOLEAN
IsTpmPresent (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

BOOLEAN
IsLtResetStatus (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

EFI_STATUS
SetupLtDeviceMemory (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

EFI_STATUS
DoScheck (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

EFI_STATUS
DoLockConfig (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

EFI_STATUS
ClearTpmAuxIndex (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

EFI_STATUS
ResetTpmEstBit (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

EFI_STATUS
TxtLockConfigForAll (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

#endif
