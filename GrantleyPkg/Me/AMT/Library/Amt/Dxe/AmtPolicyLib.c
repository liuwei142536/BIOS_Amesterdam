/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  2006 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:
  AmtPolicyLib.c

@brief:
  Implementation file for AMT Policy functionality

**/

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include "AmtPolicyLib.h"
#include "Protocol/AmtPlatformPolicy.h"
#include "MeLib.h"

//#include EFI_PROTOCOL_CONSUMER (ActiveManagement)
//#include EFI_PROTOCOL_CONSUMER (AmtPlatformPolicy)

DXE_AMT_POLICY_PROTOCOL  *mDxePlatformAmtPolicy = NULL;

EFI_STATUS
AmtPolicyLibInit (
  VOID
  )
/**

  @brief
  Check if AMT is enabled in setup options.

  @param[in] None.

  @retval EFI_SUCCESS             AMT platform policy is initialized.
  @retval All other error conditions encountered when no AMT platform policy available.

**/
{
  EFI_STATUS  Status;

  if (mDxePlatformAmtPolicy != NULL) {
    return EFI_SUCCESS;
  }
  //
  // Get the desired platform setup policy.
  //
  Status = gBS->LocateProtocol (&gDxePlatformAmtPolicyGuid, NULL, &mDxePlatformAmtPolicy);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "No AMT Platform Policy Protocol available"));
    ASSERT_EFI_ERROR(Status);
  }
  return Status;
}

BOOLEAN
AsfSupported (
  VOID
  )
/**

  @brief
  Check if Asf is enabled in setup options.

  @param[in] None.

  @retval FALSE                   Asf is disabled.
  @retval TRUE                    Asf is enabled.

**/
{
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit();
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
  }
  ///
  /// First check if ASF support is enabled in Setup.
  ///
  if (mDxePlatformAmtPolicy->AmtConfig.AsfEnabled != 1) {
    return FALSE;
  }

  return TRUE;
}

BOOLEAN
AmtSupported (
  VOID
  )
/**

  @brief
  Check if Amt is enabled in setup options.

  @param[in] None.

  @retval FALSE                   Amt is disabled.
  @retval TRUE                    Amt is enabled.

**/
{
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit();
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
  }
  ///
  /// First check if AMT support is enabled in Setup.
  ///
  if (mDxePlatformAmtPolicy->AmtConfig.iAmtEnabled != 1) {
    return FALSE;
  }

  return TRUE;
}

BOOLEAN
AmtHotkeyPressed (
  VOID
  )
/**

  @brief

  Check if AMT BIOS Extension hotkey was pressed during BIOS boot.

  @param[in] None.

  @retval FALSE                   MEBx hotkey was not pressed.
  @retval TRUE                    MEBx hotkey was pressed.

**/
{
  BOOLEAN    Supported;
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit();
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
  }
  ///
  /// First check if AMT Setup Prompt is enabled in Setup.
  ///
  if (mDxePlatformAmtPolicy->AmtConfig.iAmtbxHotkeyPressed == 1) {
    Supported = TRUE;
  } else {
    Supported = FALSE;
  }

  return Supported;
}

BOOLEAN
AmtSelectionScreen (
  VOID
  )
/**

  @brief
  Check if AMT BIOS Extension Selection Screen is enabled in setup options.

  @param[in] None.

  @retval FALSE                   AMT Selection Screen is disabled.
  @retval TRUE                    AMT Selection Screen is enabled.

**/
{
  BOOLEAN    Supported;
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit();
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
  }
  ///
  /// First check if AMT Selection Screen is enabled in Setup.
  ///
  if (mDxePlatformAmtPolicy->AmtConfig.iAmtbxSelectionScreen == 1) {
    Supported = TRUE;
  } else {
    Supported = FALSE;
  }

  return Supported;
}

BOOLEAN
AmtWatchDog (
  VOID
  )
/**

  @brief
  Check if AMT WatchDog is enabled in setup options.

  @param[in] None.

  @retval FALSE                   AMT WatchDog is disabled.
  @retval TRUE                    AMT WatchDog is enabled.

**/
{
  BOOLEAN    Supported;
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit();
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
  }
  Supported = FALSE;
  if (ManageabilityModeSetting () != 0) {
    ///
    ///
    /// First check if AMT WatchDog is enabled in Setup.
    ///
    if (AsfSupported ()) {
      if (mDxePlatformAmtPolicy->AmtConfig.WatchDog == 1) {
        Supported = TRUE;
      }
    }
  }

  return Supported;
}

BOOLEAN
AmtWatchDogOs (
  VOID
  )
/**

  @brief
  Check if AMT WatchDogOs is enabled in setup options.

  @param[in] None.

  @retval FALSE                   AMT WatchDogOs is disabled.
  @retval TRUE                    AMT WatchDogOs is enabled.

**/
{
  BOOLEAN    Supported;
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit();
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
  }
  Supported = FALSE;
  if (ManageabilityModeSetting () != 0) {
    ///
    ///
    /// First check if AMT WatchDogOs is enabled in Setup.
    ///
    if (AsfSupported ()) {
      if (mDxePlatformAmtPolicy->AmtConfig.WatchDogOs == 1) {
        Supported = TRUE;
      }
    }
  }

  return Supported;
}

UINT16
AmtWatchDogTimerBiosGet (
  VOID
  )
/**

  @brief
  Return BIOS watchdog timer

  @param[in] None.

  @retval UINT16                  BIOS ASF Watchdog Timer

**/
{
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit();
    if (EFI_ERROR(Status)) {
      return 0;
    }
  }
  return mDxePlatformAmtPolicy->AmtConfig.WatchDogTimerBios;
}

UINT16
AmtWatchDogTimerOsGet (
  VOID
  )
/**

  @brief
  Return OS watchdog timer

  @param[in] None.

  @retval UINT16                  OS ASF Watchdog Timer

**/
{
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit();
    if (EFI_ERROR(Status)) {
      return 0;
    }
  }
  return mDxePlatformAmtPolicy->AmtConfig.WatchDogTimerOs;
}

BOOLEAN
AmtCiraRequestTrigger (
  VOID
  )
/**

  @brief
  Provide CIRA request information from OEM code.

  @param[in] None.

  @retval Check if any CIRA requirement during POST

**/
{
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit();
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
  }
  return mDxePlatformAmtPolicy->AmtConfig.CiraRequest == 1;
}

UINT8
AmtCiraRequestTimeout (
  VOID
  )
/**

  @brief
  Provide CIRA request Timeout from OEM code.

  @param[in] None.

  @retval CIRA require Timeout for MPS connection to be estabilished

**/
{
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit();
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
  }
  return mDxePlatformAmtPolicy->AmtConfig.CiraTimeout;
}

UINT8
ManageabilityModeSetting (
  VOID
  )
/**

  @brief
  Provide Manageability Mode setting from MEBx BIOS Sync Data

  @param[in] None

  @retval UINT8                   Manageability Mode = MNT_AMT or MNT_ASF

**/
{
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit();
    if (EFI_ERROR(Status)) {
      return 0;
    }
  }
  return (UINT8) (mDxePlatformAmtPolicy->AmtConfig.ManageabilityMode);
}

BOOLEAN
AmtUnConfigureMe (
  VOID
  )
/**

  @brief

  Provide UnConfigure ME without password request from OEM code.

  @param[in] None.

  @retval Check if unConfigure ME without password request

**/
{
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit();
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
  }
  return mDxePlatformAmtPolicy->AmtConfig.UnConfigureMe == 1;
}

BOOLEAN
AmtHideUnConfigureMeConfPrompt (
  VOID
  )
/**

  @brief
  Provide 'Hiding the Unconfigure ME without password confirmation prompt' request from OEM code.

  @param[in] None.

  @retval Check if 'Hide unConfigure ME without password Confirmation prompt' request

**/
{
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit();
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
  }
  return mDxePlatformAmtPolicy->AmtConfig.HideUnConfigureMeConfirm == 1;
}

BOOLEAN
AmtMebxDebugMsg (
  VOID
  )
/**

  @brief
  Provide show MEBx debug message request from OEM code.

  @param[in] None.

  @retval Check show MEBx debug message request

 **/
{
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit();
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
  }
  return mDxePlatformAmtPolicy->AmtConfig.MebxDebugMsg == 1;
}

UINT32
AmtPciDeviceFilterOutTable (
  VOID
  )
/**

  @brief

  Provide on-board device list table and do not need to report them to AMT.  AMT only need to know removable PCI device
  information.

  @param[in] None.

  @retval on-board device list table pointer other than system device.

**/
{
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit();
    if (EFI_ERROR(Status)) {
      return 0;
    }
  }
  return mDxePlatformAmtPolicy->AmtConfig.PciDeviceFilterOutTable;
}

BOOLEAN
USBProvisionSupport (
  VOID
  )
/**

  @brief
  Check if USB provisioning enabled/disabled in platform policy.

  @param[in] None.

  @retval FALSE                   USB provisioning is disabled.
  @retval TRUE                    USB provisioning is enabled.

**/
{
  BOOLEAN    Supported;
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit();
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
  }
  Supported = FALSE;

  ///
  /// First check if USB Provision is enabled in Setup.
  ///
  if (mDxePlatformAmtPolicy->AmtConfig.USBProvision == 1) {
    Supported = TRUE;
  }

  return Supported;
}

BOOLEAN
FwProgressSupport (
  VOID
  )
/**

  @brief
  This will return progress event Option.
  True if the option is enabled.

  @param[in] None.

  @retval True                    progress event is enabled.
  @retval False                   progress event is disabled.

**/
{
  BOOLEAN    Supported;
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit();
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
  }
  Supported = FALSE;

  ///
  /// First check if FW Progress is enabled in Setup.
  ///
  if (mDxePlatformAmtPolicy->AmtConfig.FWProgress == 1) {
    Supported = TRUE;
  }

  return Supported;
}

BOOLEAN
AmtForcMebxSyncUp (
  VOID
  )
/**

  @brief
  Check if ForcMebxSyncUp is enabled in setup options.

  @param[in] None.

  @retval FALSE                   ForcMebxSyncUp is disabled.
  @retval TRUE                    ForcMebxSyncUp is enabled.

**/
{
  BOOLEAN    Supported;
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit();
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
  }
  if (mDxePlatformAmtPolicy->AmtConfig.ForcMebxSyncUp == 1) {
    Supported = TRUE;
  } else {
    Supported = FALSE;
  }

  return Supported;
}
