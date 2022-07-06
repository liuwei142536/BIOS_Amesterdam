/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c) 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:
  MeTypeSetLib.c
  
@brief:
  Me Type Set Lib implementation.
  It is for set ME Type in system configuration in DXE

**/
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <PiDxe.h>
#include <Library/IoLib.h>
#include <Library/PciExpressLib.h>
#include "Library/PciLib.h"
#include "Library/PchPlatformLib.h"
#include <Library/DebugLib.h>
#include <Guid/SetupVariable.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include "MeAccess.h"
#include "PchAccess.h"
#include "HeciRegs.h"
#include "Library/MeTypeLib.h"
#include "MeTypeLibInternal.h"
#include "Sps.h"

EFI_STATUS
MeTypeSet (void)
{
  UINTN                   VariableSize = sizeof (SYSTEM_CONFIGURATION);
  UINT32                  Attributes;
  SYSTEM_CONFIGURATION    SetupData;
  EFI_STATUS              Status;
  ON_BOARD_ME_TYPE        MeType = GetOnBoardMeType();

  DEBUG ((EFI_D_INFO, "[HECI] MeTypeSet Called for MeType = 0x%x\n",MeType));
  if ((MeType != ME_TYPE_SPS) && (MeType != ME_TYPE_ME9)) {
    DEBUG ((EFI_D_ERROR, "[HECI] Unknown MeType !!!\n"));
    return EFI_UNSUPPORTED;
  }

  Status = gRT->GetVariable (
                L"IntelSetup",   //AptioV server override
                &gEfiSetupVariableGuid,
                &Attributes,
                &VariableSize,
                &SetupData
                );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[HECI] Failed to read SYSTEM_CONFIGURATION!  Status = %r\n", Status));
    return EFI_NOT_READY;
  }
  DEBUG ((EFI_D_INFO, "[HECI] MeType present in setup data = 0x%x\n",SetupData.MeType));
  if (SetupData.MeType != (UINT8)MeType) {
     DEBUG ((EFI_D_INFO, "[HECI] MeType present in setup is different to detected. Updating ...\n"));
     SetupData.MeType = (UINT8)MeType;
     Status = gRT->SetVariable(
                   L"IntelSetup",  //AptioV server override
                   &gEfiSetupVariableGuid,
                   Attributes,
                   VariableSize,
                   &SetupData
                   );
     DEBUG ((EFI_D_INFO, "[HECI] MeType update result : %r\n", Status));
  }

  return EFI_SUCCESS;
}