/** @file
  PCH BIOS Write Protect Driver.

@copyright
  Copyright (c) 2011 - 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
**/
#include "PchBiosWriteProtect.h"
#include <Protocol/SmmBase2.h>

///
/// Global variables
///
EFI_SMM_ICHN_DISPATCH2_PROTOCOL        *mIchnDispatch;
EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL     *mPchIoTrap;
UINTN                                   mPciD31F0RegBase;

/**
  This hardware SMI handler will be run every time the BIOS Write Enable bit is set.

  @param[in] DispatchHandle       Not used
  @param[in] DispatchContext      Not used

  @retval None
**/
static
VOID
PchBiosWpCallback (
  IN  EFI_HANDLE                                DispatchHandle,
  IN CONST VOID  *Context         OPTIONAL,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{
  DEBUG ((DEBUG_INFO, "PchBiosWpCallback.\n"));

  ///
  /// Disable BIOSWE bit to protect BIOS
  ///
  MmioAnd8 ((UINTN) (mPciD31F0RegBase + R_PCH_LPC_BIOS_CNTL), (UINT8) ~B_PCH_LPC_BIOS_CNTL_BIOSWE);
}

/**
  Register an IchnBiosWp callback function to handle TCO BIOSWR SMI
  SMM_BWP and BLE bits will be set here

  @param[in] DispatchHandle       Not used
  @param[in] CallbackContext      Information about the IO trap that occurred

  @retval None
**/
VOID
PchBiosLockIoTrapCallback (
  IN  EFI_HANDLE                                DispatchHandle,
  IN CONST VOID  *Context         OPTIONAL,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS                       Status;
  EFI_SMM_ICHN_REGISTER_CONTEXT    IchnContext;
  EFI_HANDLE                       IchnHandle;
  
  DEBUG ((DEBUG_INFO, "PchBiosLockIoTrapCallback.\n"));
  
  if (mIchnDispatch == NULL) {
    return;
  }

  IchnHandle = NULL;

  ///
  /// Set SMM_BWP bit before registering IchnBiosWp
  ///
  MmioOr8 ((UINTN) (mPciD31F0RegBase + R_PCH_LPC_BIOS_CNTL), (UINT8) B_PCH_LPC_BIOS_CNTL_SMM_BWP);

  ///
  /// Register an IchnBiosWp callback function to handle TCO BIOSWR SMI
  ///
  IchnContext.Type = IchnBiosWp;
  Status = mIchnDispatch->Register (
                            mIchnDispatch,
                            (EFI_SMM_HANDLER_ENTRY_POINT2)PchBiosWpCallback,
                            &IchnContext,
                            &IchnHandle
                            );
  ASSERT_EFI_ERROR (Status);
}

VOID
EFIAPI
OnReadyToBoot (
  IN      EFI_EVENT                         Event,
  IN      VOID                              *Context
  )
{
  DXE_PCH_PLATFORM_POLICY_PROTOCOL          *PchPlatformPolicy;

  DEBUG ((DEBUG_ERROR | DEBUG_INFO, "PchBiosWriteProtect_ReadyToBoot.\n"));
  
  gBS->CloseEvent (Event);

  PchPlatformPolicy = (DXE_PCH_PLATFORM_POLICY_PROTOCOL *)Context;
  IoWrite32 (PchPlatformPolicy->LockDownConfig->PchBiosLockIoTrapAddress, 0x0E);

  DEBUG ((DEBUG_ERROR | DEBUG_INFO, "PchBiosWriteProtect_ReadyToBoot Finish.\n"));
}

/**
  Entry point for Pch Bios Write Protect driver.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval EFI_SUCCESS             Initialization complete.
**/
EFI_STATUS
EFIAPI
InstallPchBiosWriteProtect (
  IN EFI_HANDLE            ImageHandle,
  IN EFI_SYSTEM_TABLE      *SystemTable
  )
{
  EFI_STATUS                                Status;
  DXE_PCH_PLATFORM_POLICY_PROTOCOL          *PchPlatformPolicy;
  EFI_HANDLE                                PchIoTrapHandle;
  EFI_SMM_IO_TRAP_REGISTER_CONTEXT          PchIoTrapRegister;
  UINT64                                    BaseAddress;
  EFI_EVENT                                 OnReadyToBootEvent;
  EFI_SMM_BASE2_PROTOCOL                    *SmmBase;
  BOOLEAN                                   InSmm;

  ///
  /// Locate PCH Platform Policy protocol
  ///
  Status = gBS->LocateProtocol (&gDxePchPlatformPolicyProtocolGuid, NULL, (VOID **)&PchPlatformPolicy);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR | DEBUG_INFO, "Failed to locate PCH Policy protocol.\n"));
    return Status;
  }

  if ((PchPlatformPolicy->LockDownConfig->BiosLock == PCH_DEVICE_ENABLE)) {
    mPciD31F0RegBase = MmPciAddress (
                        0,
                        0,
                        PCI_DEVICE_NUMBER_PCH_LPC,
                        PCI_FUNCTION_NUMBER_PCH_LPC,
                        0
                        );

    ///
    /// Get the ICHn protocol
    ///
    mIchnDispatch = NULL;
    Status = gSmst->SmmLocateProtocol(&gEfiSmmIchnDispatch2ProtocolGuid, NULL, &mIchnDispatch);

    ASSERT_EFI_ERROR (Status);

    ///
    /// Locate the PCH IO TRAP Dispatch protocol
    ///
    PchIoTrapHandle = NULL;
    Status = gSmst->SmmLocateProtocol (&gEfiSmmIoTrapDispatch2ProtocolGuid, NULL, &mPchIoTrap);

    ASSERT_EFI_ERROR (Status);

    if (PchPlatformPolicy->LockDownConfig->PchBiosLockIoTrapAddress == 0)  {
        BaseAddress = 0x3000;
        Status = gDS->AllocateIoSpace (
                        EfiGcdAllocateMaxAddressSearchBottomUp,
                        EfiGcdIoTypeIo,
                        1,
                        0x10,
                        &BaseAddress,
                        ImageHandle,
                        NULL
                        );
        ASSERT_EFI_ERROR (Status);
        PchPlatformPolicy->LockDownConfig->PchBiosLockIoTrapAddress = (UINT16)BaseAddress;
    }

    ///
    /// Register BIOS Lock IO Trap SMI handler
    ///
    PchIoTrapRegister.Type         = WriteTrap;
    PchIoTrapRegister.Length       = 4;
    PchIoTrapRegister.Address    = PchPlatformPolicy->LockDownConfig->PchBiosLockIoTrapAddress;

    Status = mPchIoTrap->Register (
                          mPchIoTrap,
                          (EFI_SMM_HANDLER_ENTRY_POINT2)PchBiosLockIoTrapCallback,
                          &PchIoTrapRegister,                       
                          &PchIoTrapHandle
                          );
    ASSERT_EFI_ERROR (Status);

    DEBUG ((DEBUG_ERROR, "PchBiosLockIoTrapAddress = 0x%x\n", PchIoTrapRegister.Address));

    if ((PchPlatformPolicy->LockDownConfig->PchBiosLockIoTrapAddress == 0) &&
        (PchIoTrapRegister.Address == 0)) {
      DEBUG ((EFI_D_ERROR | EFI_D_INFO, "Invalid PchIoTrapContext.Address!!!\n"));
      ASSERT (FALSE);
    } else {
      if ((PchPlatformPolicy->LockDownConfig->PchBiosLockIoTrapAddress != 0) &&
          (PchPlatformPolicy->LockDownConfig->PchBiosLockIoTrapAddress != PchIoTrapRegister.Address)) {
        DEBUG ((DEBUG_ERROR | DEBUG_INFO, "Invalid PchIoTrapContext.Address!!!\n"));
        ASSERT (FALSE);
      } else {
        PchPlatformPolicy->LockDownConfig->PchBiosLockIoTrapAddress = PchIoTrapRegister.Address;
      }
    }

    //
    // Retrieve SMM Base Protocol
    //
    Status = gBS->LocateProtocol (
                    &gEfiSmmBase2ProtocolGuid,
                    NULL,
                    (VOID **) &SmmBase
                    );
    ASSERT_EFI_ERROR (Status);

    //
    // Check to see if we are already in SMM
    //
    SmmBase->InSmm (SmmBase, &InSmm);
    if (!InSmm) {
      DEBUG ((DEBUG_ERROR, "InstallPchBiosWriteProtect_ DXE.\n"));
      EfiCreateEventReadyToBootEx (
        TPL_NOTIFY, 
        OnReadyToBoot, 
        PchPlatformPolicy, 
        &OnReadyToBootEvent
        );
    } else {
      DEBUG ((DEBUG_ERROR, "InstallPchBiosWriteProtect_SMM.\n"));
    }
    
  }

  return EFI_SUCCESS;
}
