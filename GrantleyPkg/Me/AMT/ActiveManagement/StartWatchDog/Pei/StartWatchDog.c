/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  1999 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:

  StartWatchDog.c

@brief:



**/
#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include "AmtLibPei.h"
#include "HeciMsgLibPei.h"
#include "Library/MeTypeLib.h"

EFI_STATUS
EFIAPI
PeiInitStartWatchDog (
  IN EFI_FFS_FILE_HEADER       *FfsHeader,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
/**

  @brief
  Perform the platform spefific initializations.

  @param[in] FfsHeader            FFS file header pointer of this driver.
  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval EFI_SUCCESS             if the interface could be successfully installed.

**/
{
  EFI_STATUS    Status;
  EFI_BOOT_MODE BootMode;
  PEI_HECI_PPI  *HeciPpi;
  UINT32        HeciMemBar;
  UINT16        WaitTimerBios;
  UINT32        MeStatus;

#if defined(SPS_SUPPORT) && SPS_SUPPORT
  if (!MeTypeIsAmt()) {
    return EFI_UNSUPPORTED;
  }
#endif // SPS_SUPPORT

  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status) && (BootMode == BOOT_ON_S3_RESUME)) {
    return EFI_SUCCESS;
  }

  if (PeiAmtWatchDog ((EFI_PEI_SERVICES **)PeiServices)) {
    Status = PeiServicesLocatePpi (
              &gPeiHeciPpiGuid, /// GUID
              0,                /// INSTANCE
              NULL,             /// EFI_PEI_PPI_DESCRIPTOR
              &HeciPpi          /// PPI
              );
    ASSERT_EFI_ERROR (Status);

    Status = HeciPpi->InitializeHeci(HeciPpi, &HeciMemBar);
    if (!EFI_ERROR (Status)) {
      ///
      /// Get ME Status
      ///
      Status = HeciPpi->GetMeStatus(&MeStatus);
      ASSERT_EFI_ERROR (Status);

      ///
      /// If ME is ready, send AsfStartWatchDog message
      ///
      if (ME_STATUS_ME_STATE_ONLY (MeStatus) == ME_READY) {
        WaitTimerBios = PeiAmtWatchTimerBiosGet ((EFI_PEI_SERVICES **)PeiServices);

        Status = PeiHeciAsfStartWatchDog (
                  (EFI_PEI_SERVICES **)PeiServices,
                  HeciPpi,
                  HeciMemBar,
                  WaitTimerBios
                  );
        ASSERT_EFI_ERROR (Status);
      }
    }
  }

  return Status;
}
