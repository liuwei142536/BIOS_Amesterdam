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
  AmtLib.h

@brief
  Header file for AMT functionality

**/
#ifndef _AMT_LIB_H_
#define _AMT_LIB_H_

#include "AmtPolicyLib.h"

EFI_STATUS
AmtLibInit (
  VOID
  )
/**

  Check if Asf is enabled in setup options.

  @param[in] None.

**/
;

BOOLEAN
ActiveManagementEnableIdeR (
  IN  VOID
  )
/**

  This will return IDE Redirection Boot Option.
  True if the option is enabled.

  @param[in] None.

  @retval True                    IDE-R is enabled.
  @retval False                   IDE-R is disabled.

**/
;

BOOLEAN
ActiveManagementEnforceSecureBoot (
  IN  VOID
  )
/**

  This will return Enforce Secure Boot over IDER Boot Option.
  True if the option is enabled.

  @param[in] None.

  @retval True                    Enforce Secure Boot is enabled.
  @retval False                   Enforce Secure Boot is disabled.

**/
;

BOOLEAN
ActiveManagementPauseBoot (
  IN  VOID
  )
/**

  This will return BIOS Pause Boot Option.
  True if the option is enabled.

  @param[in] None.

  @retval True                    BIOS Pause is enabled.
  @retval False                   BIOS Pause is disabled.

**/
;

BOOLEAN
ActiveManagementEnterSetup (
  IN  VOID
  )
/**

  This will return BIOS Setup Boot Option.
  True if the option is enabled.

  @param[in] None.

  @retval True                    BIOS Setup is enabled.
  @retval False                   BIOS Setup is disabled.

**/
;

BOOLEAN
ActiveManagementConsoleLocked (
  IN  VOID
  )
/**

  This will return Serial-over-Lan Boot Option.
  True if the option is enabled.

  @param[in] None.

  @retval True                    Console Lock is enabled.
  @retval False                   Console Lock is disabled.

**/
;

BOOLEAN
ActiveManagementEnableKvm (
  IN  VOID
  )
/**

  This will return KVM Boot Option.
  True if the option is enabled.

  @param[in] None.

  @retval True                    KVM is enabled.
  @retval False                   KVM is disabled.

**/
;

BOOLEAN
ActiveManagementEnableSol (
  IN  VOID
  )
/**

  This will return Serial-over-Lan Boot Option.
  True if the option is enabled.

  @param[in] None.

  @retval True                    Serial-over-Lan is enabled.
  @retval False                   Serial-over-Lan is disabled.

**/
;

UINT8
ActiveManagementIderBootDeviceGet (
  IN VOID
  )
/**

  This will return IDE Redirection boot device index to boot

  @param[in] None.

  @retval IdeBootDevice           Return the boot device number to boot
                                  Bits 0-1: If IDER boot is selected in Perimeter 1 then Bits 1,2 define the drive on the IDER controller to be used as the boot driver.
                                  Bit 1  Bit0
                                   0    0    Primary Master Drive
                                   0    1    Primary Slave Drive
                                   1    0    Secondary Master Drive
                                   1    1    Secondary Slave Drive
                                  Bits 2-7: Reserved set to 0

**/
;

BOOLEAN
ActiveManagementFwProgress (
  IN  VOID
  )
/**

  This will return progress event Option.
  True if the option is enabled.

  @param[in] None.

  @retval True                    progress event is enabled.
  @retval False                   progress event is disabled.

**/
;

//
// ASF Message
//
#define EFI_ASF_START_WDT_VERSION                         0x10
#define EFI_ASF_STOP_WDT_VERSION                          0x10

#define EFI_ASF_MESSAGE_COMMAND_SENSE_DEVICE_SYSTEM_STATE 0x01
#define EFI_ASF_MESSAGE_COMMAND_MANAGEMENT_CONTROL        0x02
#define EFI_ASF_MESSAGE_COMMAND_ASF_CONFIGURATION         0x03
#define EFI_ASF_MESSAGE_COMMAND_MESSAGE                   0x04
#define EFI_ASF_MESSAGE_COMMAND_GETUUID                   0xC7

//
// HECI ASF Command
//
#define EFI_ASF_START_WDT_BYTE_COUNT                      0x0F
#define EFI_ASF_START_WDT_VERSION_NUMBER                  0x10
#define EFI_ASF_START_WDT_EVENT_SENSOR_TYPE               0x23
#define EFI_ASF_START_WDT_EVENT_TYPE                      0x6F
#define EFI_ASF_START_WDT_EVENT_OFFSET                    0x00
#define EFI_ASF_START_WDT_EVENT_SOURCE_TYPE_OS            0x48
#define EFI_ASF_START_WDT_EVENT_SOURCE_TYPE_BIOS          0x00
#define EFI_ASF_START_WDT_EVENT_SOURCE_TYPE               0x00  ///< 0x00 - BIOs, 0x48 - OS
#define EFI_ASF_START_WDT_EVENT_SEVERITY                  0x10  ///< critical
#define EFI_ASF_START_WDT_SENSOR_DEVICE                   0xFF  ///< unspecified
#define EFI_ASF_START_WDT_SENSOR_NUMBER                   0xFF  ///< unspecified
#define EFI_ASF_START_WDT_ENTITY                          0x00  ///< unspecified
#define EFI_ASF_START_WDT_ENTITY_INSTANCE                 0x00  ///< unspecified
#define EFI_ASF_START_WDT_EVENT_DATA_BYTE_0               0x40
#define EFI_ASF_START_WDT_EVENT_DATA_BYTE_1               0x02  ///< 0x02 BIOS POST WDT Timeout, 0x04 OS WDT timeout
#define EFI_ASF_START_WDT_EVENT_DATA_BYTE_1_BIOS_TIMEOUT  0x02
#define EFI_ASF_START_WDT_EVENT_DATA_BYTE_1_OS_TIMEOUT    0x04

#define EFI_ASF_STOP_WDT_BYTE_COUNT                       0x02

VOID
AsfStopWatchDog (
  VOID
  )
/**

  Stop ASF Watch Dog Timer

  @param[in] None.

**/
;

#define ASF_START_BIOS_WDT  0
#define ASF_START_OS_WDT    1

VOID
AsfStartWatchDog (
  IN  UINT8                       WatchDogType
  )
/**

  Start ASF Watch Dog Timer

  @param[in] WatchDogType         Which kind of WatchDog, ASF OS WatchDog Timer setting or ASF BIOS WatchDog Timer setting

**/
;

BOOLEAN
BdsKvmInitialization (
  IN  VOID
  )
/**

  Sent initialize KVM message

  @param[in] None.

  @retval True                    KVM Initialization is successful
  @retval False                   KVM is not enabled

**/
;
#endif
