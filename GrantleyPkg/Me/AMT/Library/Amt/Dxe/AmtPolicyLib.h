/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

@copyright
  Copyright (c)  2006 - 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

@file
  AmtPolicyLib.h

@brief
  Header file for AMT Policy functionality

**/
#ifndef _AMT_POLICY_LIB_H_
#define _AMT_POLICY_LIB_H_

#include "MeBiosExtensionSetup.h"

EFI_STATUS
AmtPolicyLibInit (
  VOID
  )
/**

  Check if AMT is enabled in setup options.

  @param[in] None.

  @retval EFI_SUCCESS             AMT platform policy is initialized.
  @retval All other error conditions encountered when no AMT platform policy available.

**/
;

BOOLEAN
AsfSupported (
  VOID
  )
/**

  Check if Asf is enabled in setup options.

  @param[in] None.

  @retval FALSE                   Asf is disabled.
  @retval TRUE                    Asf is enabled.

**/
;

BOOLEAN
AmtSupported (
  VOID
  )
/**

  Check if Amt is enabled in setup options.

  @param[in] None.

  @retval FALSE                   Amt is disabled.
  @retval TRUE                    Amt is enabled.

**/
;

BOOLEAN
AmtHotkeyPressed (
  VOID
  )
/**

  Check if AMT BIOS Extension hotkey was pressed during BIOS boot.

  @param[in] None.

  @retval FALSE                   MEBx hotkey was not pressed.
  @retval TRUE                    MEBx hotkey was pressed.

**/
;

BOOLEAN
AmtSelectionScreen (
  VOID
  )
/**

  Check if AMT BIOS Extension Selection Screen is enabled in setup options.

  @param[in] None.

  @retval FALSE                   AMT Selection Screen is disabled.
  @retval TRUE                    AMT Selection Screen is enabled.

**/
;

BOOLEAN
AmtWatchDog (
  VOID
  )
/**

  Check if AMT WatchDog is enabled in setup options.

  @param[in] None.

  @retval FALSE                   AMT WatchDog is disabled.
  @retval TRUE                    AMT WatchDog is enabled.

**/
;

BOOLEAN
AmtWatchDogOs (
  VOID
  )
/**

  Check if AMT WatchDogOs is enabled in setup options.

  @param[in] None.

  @retval FALSE                   AMT WatchDogOs is disabled.
  @retval TRUE                    AMT WatchDogOs is enabled.

**/
;


UINT16
AmtWatchDogTimerBiosGet (
  VOID
  )
/**

  Return BIOS watchdog timer

  @param[in] None.

  @retval UINT16                  BIOS ASF Watchdog Timer

**/
;

UINT16
AmtWatchDogTimerOsGet (
  VOID
  )
/**

  Return OS watchdog timer

  @param[in] None.

  @retval UINT16                  OS ASF Watchdog Timer

**/
;

BOOLEAN
AmtCiraRequestTrigger (
  VOID
  )
/**

  Provide CIRA request information from OEM code.

  @param[in] None.

  @retval Check if any CIRA requirement during POST

**/
;

UINT8
AmtCiraRequestTimeout (
  VOID
  )
/**

  Provide CIRA request Timeout from OEM code.

  @param[in] None.

  @retval CIRA require Timeout for MPS connection to be estabilished

**/
;

UINT8
ManageabilityModeSetting (
  VOID
  )
/**

  Provide Manageability Mode setting from MEBx BIOS Sync Data

  @param[in] None

  @retval UINT8                   Manageability Mode = MNT_AMT or MNT_ASF

**/
;

BOOLEAN
AmtUnConfigureMe (
  VOID
  )
/**

  Provide UnConfigure ME without password request from OEM code.

  @param[in] None.

  @retval Check if unConfigure ME without password request

**/
;

BOOLEAN
AmtHideUnConfigureMeConfPrompt (
  VOID
  )
/**

  Provide 'Hiding the Unconfigure ME without password confirmation prompt' request from OEM code.

  @param[in] None.

  @retval Check if 'Hide unConfigure ME without password Confirmation prompt' request

**/
;

BOOLEAN
AmtMebxDebugMsg (
  VOID
  )
/**

  Provide show MEBx debug message request from OEM code.

  @param[in] None.

  @retval Check show MEBx debug message request

 **/
;

UINT32
AmtPciDeviceFilterOutTable (
  VOID
  )
/**

  Provide on-board device list table and do not need to report them to AMT.  AMT only need to know removable PCI device
  information.

  @param[in] None.

  @retval on-board device list table pointer other than system device.

**/
;

BOOLEAN
USBProvisionSupport (
  VOID
  )
/**

  Check if USB provisioning enabled/disabled in platform policy.

  @param[in] None.

  @retval FALSE                   USB provisioning is disabled.
  @retval TRUE                    USB provisioning is enabled.

**/
;

BOOLEAN
FwProgressSupport (
  VOID
  )
/**

  This will return progress event Option.
  True if the option is enabled.

  @param[in] None.

  @retval True                    progress event is enabled.
  @retval False                   progress event is disabled.

**/
;

BOOLEAN
AmtForcMebxSyncUp (
  VOID
  )
/**

  Check if ForcMebxSyncUp is enabled in setup options.

  @param[in] None.

  @retval FALSE                   ForcMebxSyncUp is disabled.
  @retval TRUE                    ForcMebxSyncUp is enabled.

**/
;

VOID
DxeAmtPolicyDebugDump (
  VOID
  )
/**

  Dump DXE Amt Platform Policy

  @param[in] None.

**/
;

VOID
DxeMebxSetupVariableDebugDump (
  IN ME_BIOS_EXTENSION_SETUP  *MeBiosExtensionSetup OPTIONAL
  )
/**

  Dump ME_BIOS_EXTENSION_SETUP variable

  @param[in] MeBiosExtensionSetup   Pointer to ME_BIOS_EXTENSION_SETUP variable

**/
;
#endif
