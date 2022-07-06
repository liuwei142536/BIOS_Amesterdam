/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  2006 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file
  HeciMsgLib.h

@brief
  Header file for Heci Message functionality

--*/
#ifndef _HECI_MESSAGE_LIB_H_
#define _HECI_MESSAGE_LIB_H_

#include "CoreBiosMsg.h"
#include <Protocol/HeciProtocol.h>

//
// Reset Request Origin Codes.
//
#define PEI_HECI_REQ_ORIGIN_BIOS_MEMORY_INIT  0x01
#define PEI_HECI_REQ_ORIGIN_BIOS_POST         0x02
#define PEI_HECI_REQ_ORIGIN_AMTBX_LAN_DISABLE 0x03

//
// End of Post Codes
//
#define HECI_EOP_STATUS_SUCCESS       0x0
#define HECI_EOP_PERFORM_GLOBAL_RESET 0x1
//
// IFR Update states
//
#define IFR_UPDATE_ENABLE   1
#define IFR_UPDATE_DISABLE  0

#define EFI_ME_FW_SKU_VARIABLE_GUID \
  { \
    0xe1a21d94, 0x4a20, 0x4e0e, 0xae, 0x9, 0xa9, 0xa2, 0x1f, 0x24, 0xbb, 0x9e \
  }

//
// Defines the HECI request buffer format for the ICC_MPHY_WRITE_SETTINGS_CMD.
//
typedef struct _ICC_MPHY_WRITE_SETTINGS_REQ
{
   ICC_HEADER  Header;           // Standard ICC HECI Header
   UINT32      Reserved    : 4;  // Reserved for future use
   UINT32      PostedWrite : 1;  // 0-Response returned, 1-No Response returned
   UINT32      Reserved2   : 23; // Reserved for future use
}MPHY_WRITE_SETTINGS_REQ;

typedef struct {
  UINT32  MeEnabled : 1;          // [0]     ME enabled/Disabled
  UINT32  Reserved : 2;           // [2:1]   Reserved, must set to 0
  UINT32  IntelAmtFw : 1;         // [3]     Intel AMT FW support
  UINT32  IntelAmtFwStandard : 1; // [4]     Intel AMT Standard FW support
  UINT32  IntelSmallBusiness : 1; // [5]     Intel Small Business Technology support
  UINT32  Reserved1 : 7;          // [12:6]  Reserved
  UINT32  AtSupported : 1;        // [13]    AT Support
  UINT32  IntelKVM : 1;           // [14]    Intel KVM supported
  UINT32  LocalWakeupTimer : 1;   // [15]    Local Wakeup Timer support
  UINT32  Reserved2 : 16;         // [31:16] Reserved, must set to 0
  UINT32  MeMinorVer : 16;        // [47:32] ME FW Minor Version.
  UINT32  MeMajorVer : 16;        // [63:48] ME FW Major Version.
  UINT32  MeBuildNo : 16;         // [79:64] ME FW Build Number.
  UINT32  MeHotFixNo : 16;        // [95:80] ME FW Hotfix Number
} ME_CAP;

#define MAX_ASSET_TABLE_ALLOCATED_SIZE  0x3000
#define HECI_HWA_CLIENT_ID              11
#define HWA_TABLE_PUSH_CMD              0

#pragma pack(1)

typedef enum _HWAI_TABLE_TYPE_INDEX
{
  HWAI_TABLE_TYPE_INDEX_FRU_DEVICE  = 0,
  HWAI_TABLE_TYPE_INDEX_MEDIA_DEVICE,
  HWAI_TABLE_TYPE_INDEX_SMBIOS,
  HWAI_TABLE_TYPE_INDEX_ASF,
  HWAI_TABLE_TYPE_INDEX_MAX         = 4,
} HWAI_TABLE_TYPE_INDEX;

typedef struct _SINGLE_TABLE_POSITION {
  UINT16  Offset;
  UINT16  Length;
} SINGLE_TABLE_POSITION;

typedef struct _TABLE_PUSH_DATA {
  SINGLE_TABLE_POSITION Tables[HWAI_TABLE_TYPE_INDEX_MAX];
  UINT8                 TableData[1];
} TABLE_PUSH_DATA;

typedef union {
  UINT32  Data;
  struct {
    UINT32  MessageLength : 16;
    UINT32  Command : 4;          // only supported command would be HWA_TABLE_PUSH=0;
    UINT32  FRUTablePresent : 1;
    UINT32  MediaTablePresent : 1;
    UINT32  SMBIOSTablePresent : 1;
    UINT32  ASFTablePresent : 1;
    UINT32  Reserved : 8;
  } Fields;
} AU_MESSAGE_HEADER;

typedef struct _AU_TABLE_PUSH_MSG {
  AU_MESSAGE_HEADER Header;
  TABLE_PUSH_DATA   Data;
} AU_TABLE_PUSH_MSG;

typedef enum {
  SEV_NO_ERROR                      = 0,
  SEV_LOW_ERROR,
  SEV_HIGH_ERROR,
  SEV_CRITICAL_ERROR
} MDES_SEVERITY_LEVEL;

typedef enum {
  EFI_STATUS_CODE                   = 1,
  RAW_BUFFER,
  ASCII_DATA,
} MDES_BIOS_PAYLOAD_TYPE;

#pragma pack()

EFI_STATUS
HeciAssetUpdateFwMsg (
  IN  EFI_HANDLE      Handle,
  IN  TABLE_PUSH_DATA *AssetTableData,
  IN  UINT16          TableDataSize
  )
/**

  @brief
  Send Hardware Asset Tables to Firmware

  @param[in] Handle               A handle for this module
  @param[in] AssetTableData       Hardware Asset Table Data
  @param[in] TableDataSize        Size of Asset table

  @retval EFI_SUCCESS             Table sent
  @retval EFI_ABORTED             Could not allocate Memory

**/


;

/**
  Send the required system ChipsetInit Table to ME FW.

  @param[in] ChipsetInitTable     The required system ChipsetInit Table.
  @param[in] ChipsetInitTableLen  Length of the table in bytes

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
HeciChipsetInitSyncMsg (
  IN  UINT8      *ChipsetInitTable,
  IN  UINT32     ChipsetInitTableLen
  )
;

EFI_STATUS
HeciSendCbmResetRequest (
  IN  UINT8                             ResetOrigin,
  IN  UINT8                             ResetType
  )
/*++

Routine Description:

  Send Core BIOS Reset Request Message through HECI.

Arguments:

  ResetOrigin - Reset source
  ResetType   - Global or Host reset

Returns:

    EFI_STATUS

--*/
;

EFI_STATUS
HeciSendEndOfPostMessage (
  void
  )
/**

  @brief
  Send End of Post Request Message through HECI.

  @param[in]                      none

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function

**/
;

EFI_STATUS
HeciGetFwCapsSkuMsg (
  IN OUT GEN_GET_FW_CAPSKU       *MsgGenGetFwCapsSku,
  IN OUT GEN_GET_FW_CAPS_SKU_ACK *MsgGenGetFwCapsSkuAck
  )
/**

  @brief
  Send Get Firmware SKU Request to ME

  @param[in] MsgGenGetFwCapsSku     Return message for Get Firmware Capability SKU
  @param[in] MsgGenGetFwCapsSkuAck  Return message for Get Firmware Capability SKU ACK

  @exception EFI_UNSUPPORTED        Current ME mode doesn't support this function

**/
;

EFI_STATUS
HeciGetFwVersionMsg (
  IN OUT GEN_GET_FW_VER_ACK     *MsgGenGetFwVersionAck
  )
/**

  @brief
  Send Get Firmware Version Request to ME

  @param[in][out] MsgGenGetFwVersionAck   Return themessage of FW version

  @exception EFI_UNSUPPORTED        Current ME mode doesn't support this function

**/
;

EFI_STATUS
HeciHmrfpoEnable (
  IN  UINT64                          Nonce,
  OUT UINT8                           *Result
  )
/**

  @brief
  Sends a message to ME to unlock a specified SPI Flash region for writing and receiving a response message.
  It is recommended that HMRFPO_ENABLE MEI message needs to be sent after all OROMs finish their initialization.

  @param[in] Nonce                Nonce received in previous HMRFPO_ENABLE Response Message
  @param[in] Result               HMRFPO_ENABLE response

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function

**/
;

EFI_STATUS
HeciHmrfpoLock (
  OUT UINT64                          *Nonce,
  OUT UINT32                          *FactoryDefaultBase,
  OUT UINT32                          *FactoryDefaultLimit,
  OUT UINT8                           *Result
  )
/**

  @brief
  Sends a message to ME to lock a specified SPI Flash region for writing and receiving a response message.

  @param[out] Nonce               Random number generated by Ignition ME FW. When BIOS
                                  want to unlock region it should use this value
                                  in HMRFPO_ENABLE Request Message
  @param[out] FactoryDefaultBase  The base of the factory default calculated from the start of the ME region.
                                  BIOS sets a Protected Range (PR) register¡¦s "Protected Range Base" field with this value
                                  + the base address of the region.
  @param[out] FactoryDefaultLimit The length of the factory image.
                                  BIOS sets a Protected Range (PR) register¡¦s "Protected Range Limit" field with this value
  @param[out] Result              Status report

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function

**/
;

EFI_STATUS
HeciHmrfpoGetStatus (
  OUT UINT8                           *Result
  )
/**

  @brief
  System BIOS sends this message to get status for HMRFPO_LOCK message.

  @param[out] Result              HMRFPO_GET_STATUS response

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function

**/
;

EFI_STATUS
HeciQueryKvmRequest (
  IN  UINT32                         QueryType,
  OUT UINT32                         *ResponseCode
  )
/**

  @brief
  KVM support.

  @param[in] QueryType            0 - Query Request
                                  1 - Cancel Request
  @param[out] ResponseCode        1h - Continue, KVM session established.
                                  2h - Continue, KVM session cancelled.

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function

**/
;

EFI_STATUS
HeciGetLocalFwUpdate (
  OUT UINT8         *RuleData
  )
/**

  @brief
  This message is sent by the BIOS or IntelR MEBX prior to the End of Post (EOP) on the boot
  where host wants to query the local firmware update interface status.

  @param[out] RuleData            1 - local firmware update interface enable
                                  0 - local firmware update interface disable

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function

**/
;

EFI_STATUS
HeciSetLocalFwUpdate (
  IN UINT8         RuleData
  )
/**

  @brief
  This message is sent by the BIOS or IntelR MEBX prior to the End of Post (EOP) on the boot
  where host wants to enable or disable the local firmware update interface.
  The firmware allows a single update once it receives the enable command

  @param[in] RuleData             1 - local firmware update interface enable
                                  0 - local firmware update interface disable

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function

**/
;

EFI_STATUS
HeciSetMeEnableMsg (
  IN VOID
  )
/**

  @brief
  This message is sent by the BIOS or IntelR MEBX prior to the End of Post (EOP) on the boot
  where host wants to enable the ME State.
  The firmware allows a single update once it receives the enable command

  @param[in] None

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function
  @retval EFI_SUCCESS             ME enabled message sent

**/
;

EFI_STATUS
HeciSetMeDisableMsg (
  VOID
  )
/**

  @brief
  This message is sent by the BIOS or IntelR MEBX prior to the End of Post (EOP) on the boot
  where host wants to disable the ME State.
  The firmware allows a single update once it receives the disable command

  @param[in] None

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function
  @retval EFI_SUCCESS             ME is disabled

**/
;

EFI_STATUS
HeciGetPlatformTypeMsg (
  OUT PLATFORM_TYPE_RULE_DATA   *RuleData
  )
/**

  @brief
  This message is sent by the BIOS or IntelR MEBX prior to the End of Post (EOP)
  on the boot where host wants to get Ibex Peak platform type.
  One of usages is to utilize this command to determine if the platform runs in
  1.5M or 5M size firmware.

  @param[in] RuleData             PlatformBrand,
                                  IntelMeFwImageType,
                                  SuperSku,
                                  PlatformTargetMarketType,
                                  PlatformTargetUsageType

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function

**/
;

EFI_STATUS
HeciAmtBiosSynchInfo (
  OUT UINT32         *RuleData
  )
/**

  @brief
  This message is sent by the BIOS on the boot where the host wants to get the firmware provisioning state.
  The firmware will respond to AMT BIOS SYNCH INFO message even after the End of Post.

  @param[out] RuleData            Bit [2:0] Reserved
                                  Bit [4:3] Provisioning State
                                    00 - Pre -provisioning
                                    01 - In -provisioning
                                    02 - Post ¡Vprovisioning
                                  Bit [31:5] Reserved

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Firmware provisioning state returned

**/
;

EFI_STATUS
HeciGetOemTagMsg (
  OUT UINT32         *RuleData
  )
/**

  @brief
  The firmware will respond to GET OEM TAG message even after the End of Post (EOP).

  @param[in] RuleData             Default is zero. Tool can create the OEM specific OEM TAG data.

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function

**/
;

EFI_STATUS
EFIAPI
HeciSetIccClockEnables (
  IN UINT32 Enables,
  IN UINT32 EnablesMask,
  IN UINT8  ResponseMode
  )
/**
  @brief
  Enables/disables clocks. Used to turn off clocks in unused pci/pcie slots.
  @param[in] Enables              each bit means corresponding clock should be turned on (1) or off (0)
  @param[in] EnablesMask          each bit means corresponding enable bit is valid (1) or should be ignored (0)
  @param[in] ResponseMode 0       wait for response, 1 - skip

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        Wrong response
  @retval EFI_NOT_READY           ME is not ready
  @retval EFI_INVALID_PARAMETER   ResponseMode is invalid value
  @retval EFI_SUCCESS             The function completed successfully.

**/
;

EFI_STATUS
HeciLockIccRegisters (
  IN UINT8       AccessMode,
  IN UINT8       ResponseMode,
  IN OUT ICC_LOCK_REGS_INFO  *LockRegInfo
  )
/**
  @brief
  Sets or reads Lock mask on ICC registers.
  @param[in] AccessMode           0 - set, 1 - get
  @param[in] ResponseMode         0 - firmware will answer, 1 - firmware will not answer
  @param[in][out] LockRegInfo     bundle count info and mask of registers to become (for 'set' mode) or are
                                  (for 'get' mode) locked. Each bit represents a register. 0=lock, 1=don't lock

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER   ResponseMode or pointer of Mask is invalid value
  @retval EFI_DEVICE_ERROR        Wrong response
  @retval EFI_NOT_READY           Heci device hasn't ready yet

**/
;

EFI_STATUS
HeciGetIccProfile (
  OUT UINT8*Profile
  )
/**
  @brief
  retrieves the number of currently used ICC clock profile
  @param[out] Profile             number of current ICC clock profile

  @exception EFI_UNSUPPORTED      ICC clock profile doesn't support
  @retval EFI_NOT_READY           Heci device hasn't ready yet

**/
;

EFI_STATUS
HeciSetIccProfile (
  IN UINT8 Profile
  )
/**
  @brief
  Sets ICC clock profile to be used on next and following boots
  @param[in] Profile              number of profile to be used

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        Wrong response
  @retval EFI_NOT_READY           Heci device hasn't ready yet

**/
;

EFI_STATUS
HeciPlatformDebugCapabilityMsg (
  IN PLATFORM_DEBUG_CAP Data,
  OUT UINT8             *Result
  )
/**

  This message is used to turn on the Intel ME firmware MDES
  capability, Intel SVT for PCH capability or both when the
  system is in a post-manufactured state. Once firmware receives
  this message, the firmware will enable selected platform debug
  capabilities . The firmware will automatically disable all
  platform debug capabilities if this message is not received
  before receiving End Of Post.

  @param[in]  Data                capabilities to be enabled
  @param[out] Result              0x00   : Enable Success
                                  Others : Enable Failure

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function

**/
;

EFI_STATUS
HeciFwFeatureStateOverride (
  IN UINT32  EnableState,
  IN UINT32  DisableState
  )
/**

  @brief
  Sends the MKHI Enable/Disable manageability message

  @param[in] EnableState          Enable Bit Mask
  @param[in] DisableState         Disable Bit Mask

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function

**/
;

EFI_STATUS
HeciGetFwFeatureStateMsg (
  OUT MEFWCAPS_SKU                *RuleData
  )
/**

  @brief
  The Get FW Feature Status message is based on MKHI interface.
  This command is used by BIOS/IntelR MEBX to get firmware runtime status.
  The GET FW RUNTIME STATUS message doesn't need to check the HFS.
  FWInitComplete value before sending the command.
  It means this message can be sent regardless of HFS.FWInitComplete.

  @param[out] RuleData            MEFWCAPS_SKU message

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function

**/
;

EFI_STATUS
HeciSendMdesStatusCode (
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN EFI_GUID                 * CallerId OPTIONAL,
  IN EFI_STATUS_CODE_DATA     * Data OPTIONAL
  )
/**

  @brief
  It creates and sends Heci messages.

  Remark:
  Functionality is available only in release mode.
  Using MDES in debug mode causes recursive calling of this function
  because debug messages are sending from Heci->SendMsg function.

  @param[in] CodeType             Indicates the type of status code being reported.
  @param[in] Value                Describes the current status of a hardware or software entity.
                                  This included information about the class and subclass that is
                                  used to classify the entity as well as an operation.
  @param[in] Instance             The enumeration of a hardware or software entity within
                                  the system. Valid instance numbers start with 1.
  @param[in] CallerId             This optional parameter may be used to identify the caller.
                                  This parameter allows the status code driver to apply different
                                  rules to different callers.
  @param[in] Data                 This optional parameter may be used to pass additional data.

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Opcode evaluation success.
  @retval Other                   Opcode evaluation failed.

**/
;

EFI_STATUS
HeciGetMdesConfig (
  OUT MDES_BIOS_FLAGS *Flags,
  OUT UINT32          *BiosEventFilters
  )
/**

  @brief
  Provides an interface to call function to send HECI message.

  @param[in] Flags                Indicates the status of the BIOS MDES.
  @param[in] BiosEventFilters     Indicates the status of the BIOS event filter group.

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function
  @retval EFI_SUCCESS             HECI sent with success.

**/
;

EFI_STATUS
HeciGetIfrUpdate (
  OUT UINT8         *RuleData
  )
/**

  @brief
  This message is sent by the BIOS when it wants to query
  the independent firmware recovery (IFR).

  @param[in] RuleData             1 - local firmware update interface enable

                                  0 - local firmware update interface disable

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function

**/
;

EFI_STATUS
HeciSetIfrUpdate (
  IN UINT8         RuleData
  )
/**

  @brief
  This message is sent by the BIOS when it wants to set
  the independent firmware recovery (IFR) state.

  @param[in] RuleData             1 - local firmware update interface enable
                                  0 - local firmware update interface disable

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function

**/
;

#endif
