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
  MeLib.h

@brief:
  Header file for Me functionality

**/
#ifndef _ME_LIB_H_
#define _ME_LIB_H_

#include "Library/MePolicyLib.h"
#include "Library/HeciMsgLib.h"
#include "MeChipset.h"
#include "HeciRegs.h"
#include "Protocol/MeBiosPayloadData.h"

#define MBP_PRESENT_MASK  0x00000020
#define FIVE_MS_TIMEOUT   5000

extern EFI_GUID gExitPmAuthProtocolGuid;

#pragma pack(push, 1)

typedef union _MBP_HEADER {
  UINT32  Data;
  struct {
    UINT32  MbpSize : 8;    /// Byte 0 - MBP Size in DW including Header
    UINT32  NumEntries : 8; /// Byte 1 - Number of Entries (Data Items) in MBP
    UINT32  Rsvd : 16;      /// Byte 3:2 - Reserved
  } Fields;
} MBP_HEADER;

typedef union _MBP_ITEM_HEADER {
  UINT32  Data;
  struct {
    UINT32  AppId : 8;      /// Byte 0 - Application ID
    UINT32  ItemId : 8;     /// Byte 1 - Item ID
    UINT32  Length : 8;     /// Byte 2 - Length in DW
    UINT32  Rsvd : 8;       /// Byte 3 - Reserved
  } Fields;
} MBP_ITEM_HEADER;

#pragma pack(pop)

EFI_STATUS
PrepareMeBiosPayload (
  IN ME_BIOS_PAYLOAD     *MbpPtr,
  IN OUT EFI_EVENT       *MpbClearEvent
  )
/**
  @brief
  This routine returns ME-BIOS Payload information.

  @param[in]     MbpPtr          ME_INFO_HOB, ME-BIOS Payload information.
  @param[in out] MpbClearEvent   event to create on gExitPmAuthProtocolGuid
                                 to check MBP FWSTS bit.

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_DEVICE_ERROR        The circular buffer is empty
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate necessary data structures.

**/
;

EFI_STATUS
MeLibInit (
  VOID
  )
/**

  @brief

  Check if Me is enabled

  @param[in] None.


**/
;

EFI_STATUS
HeciGetMeFwInfo (
  IN OUT ME_CAP *MECapability
  )
/**

  @brief
  Host client gets Firmware update info from ME client

  @param[in] MECapability         Structure of FirmwareUpdateInfo

  @exception EFI_UNSUPPORTED      No MBP Data Protocol available

**/
;

EFI_STATUS
HeciGetFwCapsSku (
  MEFWCAPS_SKU       *FwCapsSku
  )
/**

  @brief
  Send Get Firmware SKU Request to ME

  @param[in] FwCapsSku            Return Data from Get Firmware Capabilities MKHI Request

  @retval EFI_SUCCESS             The function completed successfully.

**/
;

EFI_STATUS
HeciGetPlatformType (
  OUT PLATFORM_TYPE_RULE_DATA   *RuleData
  )
/**

  @brief
  This message is sent by the BIOS or IntelR MEBX prior to the End of Post (EOP)
  on the boot where host wants to get Ibex Peak platform type.
  One of usages is to utilize this command to determine if the platform runs in
  4M or 8M size firmware.

  @param[in] RuleData             PlatformBrand,
                                  IntelMeFwImageType,
                                  SuperSku,
                                  PlatformTargetMarketType,
                                  PlatformTargetUsageType

  @retval EFI_SUCCESS             The function completed successfully.

**/
;

EFI_STATUS
HeciGetFwVersion (
  IN OUT GEN_GET_FW_VER_ACK_DATA      *MsgGenGetFwVersionAckData
  )
/**

  @brief
  Send Get Firmware Version Request to ME

  @param[in] MsgGenGetFwVersionAckData  Return themessage of FW version

  @retval EFI_SUCCESS             The function completed successfully.

**/
;

VOID
MeEmptyEventFunction (
  IN  EFI_EVENT                   Event,
  IN  void                        *ParentImageHandle
  )
/**

  @brief
  Dummy return for Me signal event use

  @param[in] Event                The event that triggered this notification function
  @param[in] ParentImageHandle    Pointer to the notification functions context

  @retval EFI_SUCCESS             Always return EFI_SUCCESS

**/
;

EFI_STATUS
GetAtStateInfo (
  AT_STATE_INFO *AtStateInfo
  )
/**

  @brief
  Get AT State Information From Stored ME platform policy

  @param[in] AtState              Pointer to AT State Information
  @param[in] AtLastTheftTrigger   Pointer to Variable holding the cause of last AT Stolen Stae
  @param[in] AtLockState          Pointer to variable indicating whether AT is locked or not
  @param[in] AtAmPref             Pointer to variable indicating whether ATAM or PBA should be used

  @retval EFI_SUCCESS             The function completed successfully
  @exception EFI_UNSUPPORTED      No MBP Data Protocol available

**/
;

VOID
DxeMbpDebugDump (
  IN DXE_MBP_DATA_PROTOCOL  *MbpPtr
  )
/**

  Dump DXE_MBP_DATA_PROTOCOL

  @param[in] MbpData    Pointer to DXE_MBP_DATA_PROTOCOL

**/
;

#endif
