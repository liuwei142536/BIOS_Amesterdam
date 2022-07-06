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

  At.h

@brief:

  Defines and prototypes for the AT driver.
  This driver implements the AT protocol.

**/
#ifndef _AT_H_
#define _AT_H_
#include "AtHi.h"
#include "MeLib.h"
#include "AtAmHelper.h"
#include "Protocol/AtProtocol.h"


//
// Used during initialization
//
// #include EFI_PROTOCOL_CONSUMER (FirmwareVolume)
// #include EFI_PROTOCOL_CONSUMER (HECI)

///
/// Driver Produced Protocols
///
// #include EFI_PROTOCOL_PRODUCER (At)

//
// extern EFI_GUID gDxePlatformAtGuid;
//
#define EFI_SIGNATURE_16(A, B)        ((A) | (B << 8))
#define EFI_SIGNATURE_32(A, B, C, D)  (EFI_SIGNATURE_16 (A, B) | (EFI_SIGNATURE_16 (C, D) << 16))
#define EFI_SIGNATURE_64(A, B, C, D, E, F, G, H) \
(EFI_SIGNATURE_32 (A, B, C, D) | ((UINT64) (EFI_SIGNATURE_32 (E, F, G, H)) << 32))

#define AT_PRIVATE_DATA_SIGNATURE  EFI_SIGNATURE_32 ('A', 'T', 'D', 'R')

#pragma pack(1)
///
/// MKHI host message header. This header is part of HECI message sent from MEBx via
/// Host Configuration Interface (HCI). ME Configuration Manager or Power Configuration
/// Manager also include this header with appropriate fields set as part of the
/// response message to the HCI.
///
typedef struct {
  UINTN             Signature;
  EFI_HANDLE        Handle;
  EFI_AT_PROTOCOL   AtProtocol;

} AT_INSTANCE;

#define AT_INSTANCE_FROM_AT_PROTOCOL(a) CR (a, AT_INSTANCE, At, AT_PRIVATE_DATA_SIGNATURE)

#pragma pack()

EFI_STATUS
EFIAPI
AtEntryPoint (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
/**

  Entry point for the AT Dxe Driver.

  @param[in] ImageHandle       Image handle of this driver.
  @param[in] SystemTable       Global system service table.

  @retval EFI_SUCCESS           Initialization complete.
  @exception EFI_UNSUPPORTED    The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.

**/
;

EFI_STATUS
EFIAPI
AuthenticateCredential (
  IN     EFI_AT_PROTOCOL   *This,
  IN     UINT8              *PassPhrase,
  IN     UINT32             *PassType,
  IN OUT UINT8              *IsAuthenticated
  )
/**

  This function sends a request to ME AT Services to validate AT recovery credentials. The user input is
  captured in UTF-16 format and then passed to this funtion. This function converts the User recovery password into
  a HASH by using Salt & Nonce and then send the password HASH to ME AT Services for validation. ME AT Service compares the
  Password HASH and returns either pass or fail.

  @param[in] This                 The address of protocol
  @param[in] PassPhrase           Passphrase that needs to be authenticated sent to ME
  @param[in] PassType             Password type user or server generated
@param[in][out] IsAuthenticated The return of the password match 1 for success and 0 for fail

  @retval EFI_OUT_OF_RESOURCES    Unable to allocate necessary data structures.
  @retval EFI_SUCCESS             The function completed successfully.

**/
;


EFI_STATUS
EFIAPI
ComputeHash (
  IN     EFI_AT_PROTOCOL              *This,
  IN     UINT8                        *PassPhrase,
  IN OUT UINT8                        *Hash
  )
/**

  This API compute the SHA1 hash of the user enterted password
  @param[in] This                 The address of protocol
  @param[in] PassPhrase           The passphrase for which SHA1 hash to be computed
  @param[in][out] Hash            The return value of the SHA1 hash

  @retval EFI_SUCCESS             The function completed successfully.

**/
;

EFI_STATUS
EFIAPI
GetTimerInfo (
  IN     EFI_AT_PROTOCOL    *This,
  IN OUT UINT32             *Interval,
  IN OUT UINT32             *TimeLeft
  )
/**

  This API get the AT Unlock Timer values

  @param[in] This                 The address of protocol
  @param[in] Interval             The return value of the Unlock Time Interval that was set by AT Server
  @param[in] TimeLeft             The Timeleft in the Unlock Timer

  @retval EFI_SUCCESS             The function completed successfully.

**/
;

EFI_STATUS
EFIAPI
GetNonce (
  IN     EFI_AT_PROTOCOL    *This,
  IN OUT UINT8              *Nonce
  )
/**

  This gets the ME nonce

  @param[in] This                 The address of protocol
  @param[in] Nonce                The return value of the 16 Byte nonce received from ME

  @retval EFI_SUCCESS             The function completed successfully.

**/
;


EFI_STATUS
EFIAPI
GetRecoveryString (
  IN  EFI_AT_PROTOCOL  *This,
  IN  UINT32           *StringId,
  OUT UINT8            *IsvString,
  OUT UINT32           *IsvStringLength

  )
/**

  This retrives the ISV String stored by AT Server that BIOS will display during Platform lock state

  @param[in] This                 The address of protocol
  @param[in] StringId             The String buffer ID to retrive the ISV String
  @param[out] IsvString           256 Bytes of ISV string array, the
  @param[out] IsvStringLength     The String length

  @retval EFI_SUCCESS             The function completed successfully.

**/
;


EFI_STATUS
EFIAPI
SendAssertStolen (
  IN  EFI_AT_PROTOCOL             *This,
  OUT UINT8                       *CompletionCode
  )
/**

  This send an AssertStolen Message to ME when OEM has set the AllowAssertStolen bit to be accepted by BIOS.

  @param[in] This             The address of protocol
  @param[out] CompletionCode  The return ME Firmware return code for this request

  @retval EFI_SUCCESS        The function completed successfully.

**/
;
EFI_STATUS
EFIAPI
GetIsvId (
  IN     EFI_AT_PROTOCOL              *This,
  OUT    UINT32                       *IsvId
  )
/**

  This receives the ISV ID from ME and display the ID, when the platform is in stolen state

  @param[in] This        - The address of protocol
  @param[out] IsvId        The pointer to 4 byte ISV ID

  @retval EFI_SUCCESS   The function completed successfully.

**/
;

EFI_STATUS
EFIAPI
SetSuspendState (
  IN     EFI_AT_PROTOCOL              *This,
  IN     UINT32                       TransitionState,
  IN     UINT8                        *Token
  )
/**

  This requests FW to enter or exit Suspend mode based on user input

  @param[in] This                 The address of protocol
  @param[in] TransitionState      0: Exit Suspend Mode
                                  1: Enter Suspend Mode
  @param[in] Token                SRTK generated Token

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate necessary data structures.

**/
;

EFI_STATUS
InitWWANREcov (
  IN     EFI_AT_PROTOCOL             *This
  )
/**

  This instructs FW that a WWAN recovery is
  desired and thus the Radio needs to be initialized

  This command in not supported.

**/
;

EFI_STATUS
GetWWANNicStatus (
  IN     EFI_AT_PROTOCOL              *This,
  IN OUT UINT8                        *RadioStatus,
  IN OUT UINT8                        *NetworkStatus
  )
/**

  This queries FW of the NIC Radio Status

  This command in not supported.

**/
;

EFI_STATUS
GetStateUnsigned (
  IN    EFI_AT_PROTOCOL   *This,
  OUT   AT_STATE_INFO    *StateUnsigned
  )
/**

  This queries FW of the AT Status in Unsigned mode

  @param[in] This                 The address of protocol
  @param[out] StateUnsigned       Structure retrieved from ME describing current AT state

  @retval EFI_SUCCESS             The function completed successfully.

**/
;


#endif
