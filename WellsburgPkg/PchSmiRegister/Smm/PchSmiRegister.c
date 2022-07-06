/** @file
  PCH RC SMI Handling initialization tasks

@copyright
  Copyright (c) 2019 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

**/
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/PchPlatformLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/PchPlatformPolicy.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmIchnDispatch2.h>
#include <Protocol/SmmIchnDispatch2Ex.h>
#include "PchAccess.h"

EFI_SMM_BASE2_PROTOCOL   *mSmmBase = NULL;
EFI_SMM_ICHN_DISPATCH2_EX_PROTOCOL      *IchnExDispatch;
EFI_SMM_ICHN_EX_REGISTER_CONTEXT        *IchnExDispatchContext;

typedef struct {
  UINTN            Smi;
  UINTN            SmiType;
  BOOLEAN          SmiCondition;
} PCH_SMI_DESC;

typedef enum {
  Ichn,
  IchnEx
} PCH_SMI_TYPE;

PCH_SMI_DESC  PchSmiDesc[] = {
  {IchnExGpioUnlock, IchnEx, FALSE}
};


VOID
PchIchnExSmiCallBack (
  IN  EFI_HANDLE                                    DispatchHandle,
  IN CONST VOID   *Context         OPTIONAL,
  IN OUT VOID     *CommBuffer      OPTIONAL,
  IN OUT UINTN    *CommBufferSize  OPTIONAL
  )
{
  UINTN PciD31F0RegBase;

  //
  // ICHnEx handlers
  //
  switch (((EFI_SMM_ICHN_EX_REGISTER_CONTEXT*) Context)->Type) {
  case IchnExGpioUnlock:
    PciD31F0RegBase = MmPciAddress (0,
                                    DEFAULT_PCI_BUS_NUMBER_PCH,
                                    PCI_DEVICE_NUMBER_PCH_LPC,
                                    PCI_FUNCTION_NUMBER_PCH_LPC,
                                    0);
    ///
    /// Set GPIO Lockdown Enable bit
    ///
    MmioOr8 ((UINTN) (PciD31F0RegBase + R_PCH_LPC_GPIO_CNT), (UINT8) B_PCH_LPC_GPIO_LOCKDOWN_EN);
  default:
    break;
  }
}

EFI_STATUS
PchSmiRegisterInit () {
  UINTN                                     Index;
  EFI_STATUS                                Status;
  DXE_PCH_PLATFORM_POLICY_PROTOCOL          *PchPlatformPolicy;

  ///
  /// Locate PCH Platform Policy protocol
  ///
  Status = gBS->LocateProtocol (&gDxePchPlatformPolicyProtocolGuid, NULL, (VOID **)&PchPlatformPolicy);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR | DEBUG_INFO, "Failed to locate PCH Policy protocol.\n"));
    return Status;
  }

  ///
  /// Fill in conditions for PchSmiDesc
  ///
  for(Index=0; Index < (sizeof(PchSmiDesc)/sizeof(PCH_SMI_DESC)); Index++)
  {
    if(PchSmiDesc[Index].Smi == IchnExGpioUnlock){
      ///
      /// PCH BIOS Spec Rev 0.5.0, Section 16.4 GPIO Registers Lockdown
      /// If GPIO configurations are not used after boot, it is recommended that the GLE Lockdown Enable
      /// and the GPIO_UNLOCK_SMI_EN bits are set by BIOS prior to end of POST.
      ///
      if (PchPlatformPolicy->LockDownConfig->GpioLockDown == PCH_DEVICE_ENABLE) {
        PchSmiDesc[Index].SmiCondition = TRUE;
      }
    }
  }
  return EFI_SUCCESS;
}


EFI_STATUS
PchSmiRegisterEntry (
  IN EFI_HANDLE            ImageHandle,
  IN EFI_SYSTEM_TABLE      *SystemTable
  )
/*++

Routine Description:

  Entry point for the ConfigSmi DXE_SMM driver.

Arguments:

  ImageHandle     - Image handle of this driver.
  SystemTable     - Global system service table.

Returns:

  EFI_SUCCESS           - Initialization complete.
  EFI_UNSUPPORTED       - When executed not in SMM
--*/
{
  EFI_STATUS                                Status;
  BOOLEAN                                   InSmm;
  UINTN                                     Index;
  EFI_HANDLE                                PchSmiEntryHandle;

  DEBUG ((DEBUG_INFO, "PchSmiRegister() Start\n"));

  //
  // Locate SmmBase protocol
  //
  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, &mSmmBase);
  ASSERT_EFI_ERROR (Status);

  mSmmBase->InSmm (mSmmBase, &InSmm);

  if (!InSmm) {
    DEBUG ((DEBUG_INFO, "PchSmiRegister() End\n"));
    return EFI_UNSUPPORTED;
  } else {
    // Init SMI Source Description
    PchSmiRegisterInit ();

    //
    // Locate ICHnEx Dispatch protocol
    //
    Status = gSmst->SmmLocateProtocol (&gEfiSmmIchnDispatch2ExProtocolGuid, NULL, &IchnExDispatch);
    ASSERT_EFI_ERROR (Status);

    //
    // Register ICHn/IchnEx callbacks
    //
    for(Index=0; Index < (sizeof(PchSmiDesc)/sizeof(PCH_SMI_DESC)); Index++)
    {
      if(PchSmiDesc[Index].SmiCondition == FALSE){
        continue;
      }
      if(PchSmiDesc[Index].SmiType == IchnEx){
        IchnExDispatchContext->Type  = PchSmiDesc[Index].Smi;
        Status = IchnExDispatch->Register (
                            IchnExDispatch,
                            (EFI_SMM_HANDLER_ENTRY_POINT2)PchIchnExSmiCallBack,
                            IchnExDispatchContext,
                            &PchSmiEntryHandle
                            );
      }
      ASSERT_EFI_ERROR (Status);
    }
  }

  DEBUG ((DEBUG_INFO, "PchSmiRegister() End\n"));

  return EFI_SUCCESS;
}
