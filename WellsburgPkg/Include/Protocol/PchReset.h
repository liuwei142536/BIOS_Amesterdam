/** @file
  PCH Reset Protocol

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
#ifndef _PCH_RESET_H_
#define _PCH_RESET_H_

#define EFI_CAPSULE_VARIABLE_NAME           L"CapsuleUpdateData"

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                             gPchResetProtocolGuid;
extern EFI_GUID                             gPchResetCallbackProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _PCH_RESET_PROTOCOL          PCH_RESET_PROTOCOL;

typedef struct _PCH_RESET_CALLBACK_PROTOCOL PCH_RESET_CALLBACK_PROTOCOL;

//
// Related Definitions
//
///
/// PCH Reset Types
///
typedef enum {
  ColdReset,
  WarmReset,
  ShutdownReset,
  PowerCycleReset,
  GlobalReset,
  GlobalResetWithEc
} PCH_RESET_TYPE;

//
// Member functions
//
/**
  Execute Pch Reset from the host controller.

  @param[in] This                 Pointer to the PCH_RESET_PROTOCOL instance.
  @param[in] PchResetType         Pch Reset Types which includes ColdReset, WarmReset, ShutdownReset,
                                  PowerCycleReset, GlobalReset, GlobalResetWithEc

  @retval EFI_SUCCESS             Successfully completed.
  @retval EFI_INVALID_PARAMETER   If ResetType is invalid.
**/
typedef
EFI_STATUS
(EFIAPI *PCH_RESET) (
  IN     PCH_RESET_PROTOCOL       * This,
  IN     PCH_RESET_TYPE           PchResetType
  );

/**
  Execute call back function for Pch Reset.

  @param[in] PchResetType         Pch Reset Types which includes PowerCycle, Globalreset.

  @retval EFI_SUCCESS             The callback function has been done successfully
  @retval EFI_NOT_FOUND           Failed to find Pch Reset Callback protocol. Or, none of
                                  callback protocol is installed.
  @retval Others                  Do not do any reset from PCH
**/
typedef
EFI_STATUS
(EFIAPI *PCH_RESET_CALLBACK) (
  IN     PCH_RESET_TYPE           PchResetType
  );

///
/// Interface structure for the Pch Reset Protocol
///
struct _PCH_RESET_PROTOCOL {
  PCH_RESET Reset;
};

///
/// PCH_RESET_CALLBACK_PROTOCOL Structure Definition
///
/// This protocol is used to execute PCH Reset from the host controller.
/// The PCH Reset protocol and PCH Reset PPI implement the Intel (R) PCH Reset Interface
/// for DXE and PEI environments, respectively. If other drivers need to run their
/// callback function right before issuing the reset, they can install PCH Reset
/// Callback Protocol/PPI before PCH Reset DXE/PEI driver to achieve that.
///
struct _PCH_RESET_CALLBACK_PROTOCOL {
  PCH_RESET_CALLBACK  ResetCallback;
};

#endif
