/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2011, Intel Corporation. All rights reserved.
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

--*/

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include "Protocol/AmtWrapperProtocol.h"
#include "Amt.h"
#include "AmtLib.h"
#include "AmtSetup.h"


extern EFI_GUID gEfiSetupVariableGuid;

EFI_STATUS
EFIAPI
InitializeAmtSetup (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
)
{
  SETUP_AMT_FEATURES    SetupAmtFeatures;
  EFI_STATUS            Status;
  UINTN                 VariableSize;
  UINTN                 Update = 0;
  UINT8                 GrayOut;
  AMT_WRAPPER_PROTOCOL  *pAmtWrapper = NULL;

//  EfiInitializeDriverLib (ImageHandle, SystemTable);

  Status = gBS->LocateProtocol(
                          &gEfiAmtWrapperProtocolGuid,
                          NULL,
                          &pAmtWrapper
                         );
  ASSERT_EFI_ERROR(Status);

  VariableSize = sizeof(SETUP_AMT_FEATURES);
  Status = gRT->GetVariable(
        L"SetupAmtFeatures",
        &gEfiSetupVariableGuid,
        NULL,
        &VariableSize,
        &SetupAmtFeatures
       );

  if ( pAmtWrapper->ActiveManagementEnableIdeR() || pAmtWrapper->ActiveManagementEnableSol() || pAmtWrapper->ActiveManagementEnableKvm()) {
        GrayOut = 1;
  } else {
        GrayOut = 0;
  }

  if (SetupAmtFeatures.GrayOut != GrayOut) {
    SetupAmtFeatures.GrayOut = GrayOut;
    Update = 1;
  }

  if (Update || EFI_ERROR(Status)) {
    Status = gRT->SetVariable(
      L"SetupAmtFeatures",
      &gEfiSetupVariableGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
      sizeof(SETUP_AMT_FEATURES),
      &SetupAmtFeatures
    );
    ASSERT_EFI_ERROR(Status);
  }

  return Status;
}
