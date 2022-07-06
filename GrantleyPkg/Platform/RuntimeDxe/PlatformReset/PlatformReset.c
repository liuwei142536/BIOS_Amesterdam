/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  2011 - 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:

  PlatformReset.c

@brief:

  Provide the ResetSystem AP.

**/
#include "PlatformReset.h"

#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
#include <Library/HeciMsgLib.h>
#include "MeLib.h"
#include "Library/MeTypeLib.h"
#endif // ME_SUPPORT_FLAG

PCH_RESET_PROTOCOL  *mPchReset;

extern EFI_GUID gEfiEventVirtualAddressChangeGuid; 

VOID
PchResetVirtualddressChangeEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );


VOID
EFIAPI
PlatformResetSystem (
  IN EFI_RESET_TYPE   ResetType,
  IN EFI_STATUS       ResetStatus,
  IN UINTN            DataSize,
  IN CHAR16           *ResetData OPTIONAL
  )
/**

  @brief
  Reset the system.

  @param[in] ResetType            Warm or cold
  @param[in] ResetStatus          Possible cause of reset
  @param[in] DataSize             Size of ResetData in bytes
  @param[in] ResetData            Optional Unicode string

  @retval Does not return if the reset takes place.

**/
{
  EFI_STATUS                          Status;
  DXE_PCH_PLATFORM_RESET_POLICY_PROTOCOL *PchPlatformResetPolicy;
  PCH_RESET_TYPE                      PchResetType;
  PCH_EXTENDED_RESET_TYPES            Type;

  //
  // Initialize the Reset Types structure
  //
  ZeroMem(&Type, sizeof(PCH_EXTENDED_RESET_TYPES));

  PchResetType = ResetType;

 if (!EfiAtRuntime() && ((ResetType == EfiResetWarm) || (ResetType == EfiResetCold))) {
    Status = gBS->LocateProtocol (&gDxePchPlatformResetPolicyProtocolGuid, NULL, &PchPlatformResetPolicy);
    if (!EFI_ERROR (Status)) {
      PchPlatformResetPolicy->PlatformReset (&Type);
      if(Type.PowerCycleReset == 1) {
        PchResetType = PowerCycleReset;
      }
      if(Type.GlobalReset == 1) {
        PchResetType = GlobalReset;
      }
      if(Type.GlobalResetWithEc == 1) {
        PchResetType = GlobalResetWithEc;
      }
    }
  } 

#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
#if defined(AMT_SUPPORT) && AMT_SUPPORT
  if (MeTypeIsAmt()) {
    if ((PchResetType == GlobalReset) || (PchResetType == GlobalResetWithEc)) {
      //
      // Let ME do global reset if Me Fw is available
      //
      Status = HeciSendCbmResetRequest (CBM_RR_REQ_ORIGIN_BIOS_POST, CBM_HRR_GLOBAL_RESET);
      // When reset via ME is not supported use PchReset Instead
      // ME Global Reset should fail after EOP is sent.
      DEBUG((EFI_D_ERROR, "[ME] Send CBM_HRR_GLOBAL_RESET status: %r\n", Status));
      // Wait for reset execution by ME
      gBS->Stall (1000000);
      // Go to use PCH Reset
      DEBUG((EFI_D_ERROR, "[ME] Continue execution with regular PCH reset\n"));
    }
  }
#endif // AMT_SUPPORT
#endif // ME_SUPPORT_FLAG

  mPchReset->Reset (mPchReset, PchResetType);

  ASSERT (FALSE);
}

EFI_STATUS
InitializePlatformReset (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
/**

  @brief
  Entry point of Platform Reset driver.

  @param[in] ImageHandle          Standard entry point parameter
  @param[in] SystemTable          Standard entry point parameter

  @retval EFI_SUCCESS             Reset RT protocol installed
  @retval All other error conditions encountered result in an ASSERT

**/
{
  EFI_HANDLE  Handle;
  EFI_STATUS  Status;
  EFI_EVENT   Event;

  Status = gBS->LocateProtocol (&gPchResetProtocolGuid, NULL, &mPchReset);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Make sure the Reset Architectural Protocol is not already installed in the system
  ///
  ASSERT_PROTOCOL_ALREADY_INSTALLED (NULL, &gEfiResetArchProtocolGuid);

  ///
  /// Hook the runtime service table
  ///
  SystemTable->RuntimeServices->ResetSystem = PlatformResetSystem;

  ///
  /// Now install the Reset RT AP on a new handle
  ///
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEfiResetArchProtocolGuid,
                  NULL,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  PchResetVirtualddressChangeEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &Event
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

VOID
PchResetVirtualddressChangeEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
/**

  @brief
  Fixup internal data pointers so that the services can be called in virtual mode.

  @param[in] Event                 The event registered.
  @param[in] Context               Event context. Not used in this event handler.


**/
{
  gRT->ConvertPointer (EFI_INTERNAL_POINTER, (VOID *) &mPchReset);
}
