/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  2004 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:

  ActiveManagement.h

@brief:

  Header file for the Active Management Driver.

**/
#ifndef _ACTIVE_MANAGEMENT_H_
#define _ACTIVE_MANAGEMENT_H_

#include <Library/UefiBootServicesTableLib.h>
#include "Amt.h"
#include "AmtLib.h"
#include "Protocol/ActiveManagementProtocol.h"

///
/// Used during initialization
///
//#include EFI_PROTOCOL_CONSUMER (AlertStandardFormat)

///
/// Driver Produced Protocols
///
//#include EFI_PROTOCOL_PRODUCER (ActiveManagement)

///
/// Private data structure definitions for the driver
///
#define EFI_SIGNATURE_16(A, B)        ((A) | (B << 8))
#define EFI_SIGNATURE_32(A, B, C, D)  (EFI_SIGNATURE_16 (A, B) | (EFI_SIGNATURE_16 (C, D) << 16))
#define EFI_SIGNATURE_64(A, B, C, D, E, F, G, H) \
    (EFI_SIGNATURE_32 (A, B, C, D) | ((UINT64) (EFI_SIGNATURE_32 (E, F, G, H)) << 32))

#define ACTIVE_MANAGEMENT_PRIVATE_DATA_SIGNATURE  EFI_SIGNATURE_32 ('A', 'M', 'T', 'P')

typedef struct {
  UINTN                               Signature;
  EFI_HANDLE                          Handle;
  EFI_ACTIVE_MANAGEMENT_PROTOCOL      ActiveManagementProtocol;
  EFI_ALERT_STANDARD_FORMAT_PROTOCOL  *Asf;
} ACTIVE_MANAGEMENT_INSTANCE;

#define ACTIVE_MANAGEMENT_INSTANCE_FROM_ACTIVE_MANAGEMENT_PROTOCOL(a) \
  CR ( \
  a, \
  ACTIVE_MANAGEMENT_INSTANCE, \
  ActiveManagementProtocol, \
  ACTIVE_MANAGEMENT_PRIVATE_DATA_SIGNATURE \
  )

///
/// Function prototypes used by the AMT protocol.
///
EFI_STATUS
EFIAPI
GetIderState (
  IN     EFI_ACTIVE_MANAGEMENT_PROTOCOL   *This,
  IN OUT BOOLEAN                          *CurrentState
  )
/**

@brief
  This will return IDE Redirection Boot Option.
  True if the option is enabled.

  @param[in] This                 The address of protocol
  @param[in] CurrentState         Return the state of IDE Redireciton Boot Opiton

  @retval EFI_SUCCESS             The function completed successfully.

**/
;

EFI_STATUS
EFIAPI
GetEnforceSecureBootState (
  IN     EFI_ACTIVE_MANAGEMENT_PROTOCOL   *This,
  IN OUT BOOLEAN                          *CurrentState
  )
/**

  @brief
  This will return Enforce Secure Boot over IDER Boot Option.
  True if the option is enabled.

  @param[in] This                 The address of protocol
  @param[in] CurrentState         Return the state of Enforce Secure Boot over IDER Boot Option

  @retval EFI_SUCCESS             The function completed successfully.

**/
;

EFI_STATUS
EFIAPI
GetSolState (
  IN     EFI_ACTIVE_MANAGEMENT_PROTOCOL   *This,
  IN OUT BOOLEAN                          *CurrentState
  )
/**

  @brief
  This will return Serial-over-Lan Boot Option.
  True if the option is enabled.

  @param[in] This                 The address of protocol
  @param[in] CurrentState         Return the state of Serial-over-Lan Boot Opiton

  @retval EFI_SUCCESS             The function completed successfully.

**/
;

EFI_STATUS
EFIAPI
GetRemoteFlashState (
  IN     EFI_ACTIVE_MANAGEMENT_PROTOCOL   *This,
  IN OUT BOOLEAN                          *CurrentState
  )
/**

  @brief
  This will return Remote Flash Boot Option.
  True if the option is enabled.

  @param[in] This                 The address of protocol
  @param[in] CurrentState         Return the state of Remote Flash Boot Opiton

  @retval EFI_SUCCESS             The function completed successfully.

**/
;

EFI_STATUS
EFIAPI
GetIderBootDeviceSelectd (
  IN     EFI_ACTIVE_MANAGEMENT_PROTOCOL   *This,
  IN OUT UINT8                            *IdeBootDevice
  )
/**

  @brief
  This will return IDE Redirection boot device to boot

  @param[in] This                 The address of protocol
  @param[in] IdeBootDevice        Return the boot device number to boot
    Bits 0-1: If IDER boot is selected in Perimeter 1 then Bits 1,2 define the drive on the IDER controller to be used as the boot driver.
     Bit 1  Bit0
      0    0    Primary Master Drive
      0    1    Primary Slave Drive
      1    0    Secondary Master Drive
      1    1    Secondary Slave Drive
    Bits 2-7: Reserved set to 0

  @retval EFI_SUCCESS             The function completed successfully.

**/
;

EFI_STATUS
EFIAPI
GetBiosSetupState (
  IN     EFI_ACTIVE_MANAGEMENT_PROTOCOL   *This,
  IN OUT BOOLEAN                          *CurrentState
  )
/**

  @brief
  This will return BIOS Setup Boot Option.
  True if the option is enabled.

  @param[in] This                 The address of protocol
  @param[in] CurrentState         Return the state of BIOS Setup Boot Opiton

  @retval EFI_SUCCESS             The function completed successfully.

**/
;

EFI_STATUS
EFIAPI
GetBiosPauseState (
  IN     EFI_ACTIVE_MANAGEMENT_PROTOCOL   *This,
  IN OUT BOOLEAN                          *CurrentState
  )
/**

  @brief
  This will return BIOS Pause Boot Option.
  True if the option is enabled.

  @param[in] This                 The address of protocol
  @param[in] CurrentState         Return the state of BIOS Pause Boot Opiton

  @retval EFI_SUCCESS             The function completed successfully.

**/
;

EFI_STATUS
EFIAPI
GetConsoleLockState (
  IN     EFI_ACTIVE_MANAGEMENT_PROTOCOL   *This,
  IN OUT BOOLEAN                          *CurrentState
  )
/**

  @brief
  This will return Console Lock Boot Option.
  True if the option is enabled.

  @param[in] This                 The address of protocol
  @param[in] CurrentState         Return the state of BIOS Pause Boot Opiton

  @retval EFI_SUCCESS             The function completed successfully.

**/
;

EFI_STATUS
EFIAPI
GetKvmState (
  IN     EFI_ACTIVE_MANAGEMENT_PROTOCOL   *This,
  IN OUT BOOLEAN                          *CurrentState
  )
/**

  @brief
  This will return KVM Boot Option.
  True if the option is enabled.

  @param[in] This                 The address of protocol
  @param[in] CurrentState         Return the state of KVM Boot Opiton

  @retval EFI_SUCCESS             The function completed successfully.

**/
;

EFI_STATUS
EFIAPI
GetAsfBootOptions (
  IN     EFI_ACTIVE_MANAGEMENT_PROTOCOL   *This,
  IN OUT EFI_ASF_BOOT_OPTIONS             **AsfBootOptions
  )
/**

  @brief

  Return current ASF Boot Options

  @param[in] This                 Pointer to the EFI_ACTIVE_MANAGEMENT_PROTOCOL instance.
  @param[in] AsfBootOptions       ASF Boot Options

  @retval EFI_SUCCESS             Boot options updated

**/
;

EFI_STATUS
EFIAPI
ActiveManagementEntryPoint (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
/**

  @brief

  Entry point for the Active Management Driver.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval EFI_SUCCESS             Initialization complete.
  @exception EFI_UNSUPPORTED      The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.

**/
;

EFI_STATUS
EFIAPI
GetProgressMsgRequest (
  IN     EFI_ACTIVE_MANAGEMENT_PROTOCOL   *This,
  IN OUT BOOLEAN                          *CurrentState
  )
/**

  @brief
  This will return progress event Option.
  True if the option is enabled.

  @param[in] This                 The address of protocol
  @param[in] CurrentState         Return the state of progress event Opiton

  @retval EFI_SUCCESS             The function completed successfully.

**/
;

#endif
