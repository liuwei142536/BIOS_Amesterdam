/*++

This file contains a 'Sample Driver' and is licensed as such
under the terms of your license agreement with Intel or your
vendor.  This file may be modified by the user, subject to
the additional terms of the license agreement

--*/

/*++
Copyright (c)  2008-2011 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

 MePlatformPolicyUpdateDxe.h

Abstract:

  Header file for DXE MePlatformPolicyUpdate.

--*/
#ifndef _ME_PLATFORM_POLICY_UPDATE_DXE_H_
#define _ME_PLATFORM_POLICY_UPDATE_DXE_H_

//
// #include "EfiDriverLib.h"
//
#include "Guid\SetupVariable.h"
//#include "Setup.h"
#include "MeLib.h"
#include "HeciRegs.h"
#include "MeChipset.h"
#include "PchAccess.h"
//#include "PlatformDefinitions.h"


#define AMT_BITMASK   0x00000004
#define CLEAR_AMT_BIT 0x00000000

#define MAX_FW_UPDATE_BIOS_SELECTIONS 2

#define ME_INFO_SETUP_GUID \
{ \
  0x78259433, 0x7B6D, 0x4DB3, 0x9A, 0xE8, 0x36, 0xC4, 0xC2, 0xC3, 0xA1, 0x7D \
}
#define SETUP_FWU_GUID \
{ \
  0x37466687, 0x7ea9, 0x4f23, 0x83, 0xa5, 0x3a, 0xd2, 0x10, 0xe7, 0xa6, 0x67 \
}

#define SETUP_MNG_GUID \
{ \
  0x35940cdb, 0x6e2e, 0x4078, 0xae, 0xf3, 0xc0, 0x80, 0x15, 0x40, 0x81, 0xab \
}

#define ME_STATE_CHG_GUID \
{ \
  0xbc61783c, 0x10b9, 0x43a5, 0xb7, 0x42, 0x71, 0x6d, 0xfa, 0xb2, 0xa2, 0xa7 \
}

#define EFI_NVRAM_UPDATE_GUID \
{ \
  0xd84beff0, 0x159a, 0x4b60, 0x9a, 0xb9, 0xac, 0x5c, 0x47, 0x4b, 0xd3, 0xb1 \
}

#define ME_POLICY_UPDATE_GUID \
{ \
  0x5c4b7c33, 0xee9b, 0x4eda, 0x80, 0x53, 0xc9, 0x58, 0x33, 0x12, 0xf7, 0x98 \
}

extern EFI_GUID gMeSetupInfoGuid;
extern EFI_GUID gSetupFwUpdChgGuid;
extern EFI_GUID gSetupMngChgGuid;
extern EFI_GUID gMeStateChgGuid;
extern EFI_GUID gMeSetupPolicyUpdate;

typedef struct {
  UINT8   MeFirmwareInfo;
  UINT32  MeMajor;
  UINT32  MeMinor;
  UINT32  MeHotFix;
  UINT32  MeBuildNo;
  UINT32  MeFileSystem;
  UINT8   MeImageType;
} ME_INFO_SETUP_DATA;

//
// EFI_STATUS
// IllegalCpuForUlvPchHandler(
//  IN  EFI_EVENT   Event,
//  IN  VOID        *Context
//  )
///*++
//
// Routine Description:
//
//  Perform End User messaging and halt booting because of ILLEGAL CPU-PCH configuration
//
// Arguments:
//
//  IN  EFI_EVENT   Event    - event based on CONSOLE_OUT_DEVICES_STARTED_PROTOCOL_GUID
//  IN  VOID        *Context - VOID Pointer
//
// Returns:
//
//  EFI_STATUS - EFI_SUCCESS;
//
//--*/
//;

VOID
GetMeSetupInfo (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
/*++

Routine Description:

  Function that is called only if we enter BIOS Setup

Arguments:

  Event         Pointer to the event that triggered this Callback Function
  Context       VOID Pointer required for Callback function

Returns:

  None


--*/
;

VOID
SignalFwuDataChng (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
/*++

Routine Description:

    Function that is Called if we change FW Update BIOS Setup Option

Arguments:

  Event         Pointer to the event that triggered this Callback Function
  Context       VOID Pointer required for Callback function

Returns:

  None


--*/
;

VOID
SignalMngStateChng (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
/*++

Routine Description:

    Function that is Called if we change Manageability State BIOS Setup Option

Arguments:

  Event         Pointer to the event that triggered this Callback Function
  Context       VOID Pointer required for Callback function

Returns:

  None.


--*/
;

VOID
SignalMeStateChng (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
/*++

Routine Description:

    Function that is Called if we change Manageability State BIOS Setup Option

Arguments:

  Event         Pointer to the event that triggered this Callback Function
  Context       VOID Pointer required for Callback function

Returns:

  None


--*/
;

VOID
SignalIfrStateChng (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
/*++

Routine Description:

  Function that is called when we save UI Data

Arguments:

  Event         Pointer to the event that triggered this Callback Function
  Context       VOID Pointer required for Callback function

Returns:

  None


--*/
;

EFI_STATUS
UpdateMePolicyFromSetup (
  DXE_ME_POLICY_PROTOCOL  *MePlatformPolicyInstance,
  BOOLEAN                 updateMeFwDownGrade
  )
/*++

Routine Description:

  Update Me platform policy while MePlatformProtocol is installed.

Arguments:

  MePlatformPolicyInstance - ME Platform Policy
  updateMeFwDownGrade      - Flag to updateMeFwDowngrade update

Returns:

  None

--*/
;

VOID
SignalMeUpdatePolicyFromSetup (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
/*++

Routine Description:

  Function that is called when we save UI Data

Arguments:

  Event         Pointer to the event that triggered this Callback Function
  Context       VOID Pointer required for Callback function

Returns:

  None


--*/
;
//
// VOID
// MePlatformPolicyPerformUlvCheck(
//  VOID
// )
// ;

EFI_STATUS
EFIAPI
MePlatformPolicyUpdateMeSettings (
  DXE_ME_POLICY_PROTOCOL *MePlatformPolicyInstance
)
;

EFI_STATUS
UpdateIfrState(
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
/*++

Routine Description:

  Function that is called when we save UI Data

Arguments:

  Event         Pointer to the event that triggered this Callback Function
  Context       VOID Pointer required for Callback function

Returns:

  EFI_SUCCESS           Initialization complete.

--*/
;

#endif
