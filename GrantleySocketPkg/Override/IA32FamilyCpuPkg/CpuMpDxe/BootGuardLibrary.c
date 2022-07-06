/**
  This file contains an 'Intel Peripheral Driver' and uniquely        
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your   
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c) 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

  @file 
  BootGuardLibrary.c

  BootGuardLibrary implementation. 

**/

#include "BootGuardLibrary.h"

BOOLEAN
IsBootGuardSupported (
  VOID
)
/**

@brief

  Determine if Boot Guard is supported

  @retval TRUE  - Processor is Boot Guard capable.
  @retval FALSE - Processor is not Boot Guard capable.

**/
{
  UINT64          BootGuardBootStatus;
  UINT32          BootGuardAcmStatus;
  UINT64          BootGuardCapability;

  BootGuardBootStatus = *(UINT64 *)(UINTN)(TXT_PUBLIC_BASE + R_CPU_BOOT_GUARD_BOOTSTATUS);    //0xA0, LT_SPAD
  BootGuardAcmStatus  = *(UINT32 *)(UINTN)(TXT_PUBLIC_BASE + R_CPU_BOOT_GUARD_ACM_STATUS);    // 0x328, LT_BIOSACMCode
  BootGuardCapability = AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO) & B_BOOT_GUARD_SACM_INFO_CAPABILITY;  // Bit 32

  if (BootGuardCapability != 0) {
    DEBUG ((DEBUG_INFO, "Processor supports Boot Guard.\n"));
    return TRUE;
  } else {
    DEBUG ((DEBUG_INFO, "Processor does not support Boot Guard.\n"));
    return FALSE;
  }
}

EFI_STATUS
StopPbeTimer (
  VOID
)
/**

@brief

  Stop PBE timer if system is in Boot Guard boot

  @retval EFI_SUCCESS        - Stop PBE timer
  @retval EFI_UNSUPPORTED    - Not in Boot Guard boot mode.

**/
{
  UINT64   BootGuardBootStatus;
  UINT64   BootGuardOperationMode;

  if (IsBootGuardSupported()) {
    BootGuardBootStatus = (*(UINT64 *) (UINTN) (TXT_PUBLIC_BASE + R_CPU_BOOT_GUARD_BOOTSTATUS) & (BIT63|BIT62));
    BootGuardOperationMode = AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO) & (B_BOOT_GUARD_SACM_INFO_MEASURED_BOOT | B_BOOT_GUARD_SACM_INFO_VERIFIED_BOOT);

    //
    // Stop PBET if Verified/Measured/NEM bit is set in MSR 0x13A or
    // Boot Guard fails to launch or fails to execute successfully for avoiding brick platform
    //

    if (BootGuardBootStatus == V_CPU_BOOT_GUARD_LOAD_ACM_SUCCESS) {        // Bit 63, LT_SPAD
      if (BootGuardOperationMode == 0) {
        DEBUG ((DEBUG_INFO, "Platform is in Legacy boot mode.\n"));
        return EFI_UNSUPPORTED;
      } else {
        DEBUG ((DEBUG_INFO, "Platform in Boot Guard Boot mode.\n"));
      }
    } else {
      DEBUG ((DEBUG_ERROR, "Boot Guard ACM launch failed or ACM execution failed.\n"));
    }

    DEBUG ((DEBUG_INFO, "Disable PBET\n"));
    AsmWriteMsr64 (MSR_BC_PBEC, B_STOP_PBET);
  } else {
    DEBUG ((DEBUG_ERROR, "Boot Guard is not supported.\n"));
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

