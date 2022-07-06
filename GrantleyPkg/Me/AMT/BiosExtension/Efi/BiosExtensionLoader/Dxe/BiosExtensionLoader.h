/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  2004 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:

  BiosExtensionLoader.h

@brief:

  Include for AMT Bios Extentions Loader

**/
#ifndef _AMT_PTBX_H_
#define _AMT_PTBX_H_

#include "IndustryStandard/Acpi10.h"
#include "IndustryStandard/Acpi20.h"
#include "IndustryStandard/Acpi30.h"
#include "IndustryStandard/SmBios.h"
#include "MeAccess.h"
#include "Inventory.h"
#include "AmtLib.h"
#include "UsbProvision.h"
#include "IndustryStandard/Pci22.h"
#include "Protocol/AmtPlatformPolicy.h"

#define MEBX_RET_ACTION_CONTINUE_TO_BOOT  0x01
#define MEBX_RET_ACTION_GO_TO_SLEEP       0x02
#define MEBX_RET_ACTION_RESET             0x04
#define MEBX_RET_CODE_MASK                0x07

#define MEBX_RET_ACTION_GLOBAL_RESET      0x01

#define MEBX_RET_SUBACTION_SLP_S4         0x01
#define MEBX_RET_SUBACTION_SLP_S5         0x02

#define MEBX_RET_SUBACTION_FAILURE        0x01

#define HIWORD_RET_SUCCESS                0x0000
#define HIWORD_RET_MEBX_SETUP_ERROR       0x0001
#define HIWORD_RET_AMT_INVENTORY_ERROR    0x0002
#define HIWORD_RET_HECI_INTERFACE_ERROR   0x0003
#define HIWORD_RET_AMT_CONNECTION_ERROR   0x0004
#define HIWORD_RET_TIME_OUT_ERROR         0x0005
#define HIWORD_RET_USB_PROVISIONING_ERROR 0x0006
#define HIWORD_RET_PARAMETERS_FRAME_ERROR 0x0007

#define FW_IMAGE_TYPE_1_5MB               3
#define FW_IMAGE_TYPE_5MB                 4

#define MEBX_USER_ENTERED_RESPONSE        0x0002
#define MEBX_RA_SELECTION_MENU            0x0004
#define MEBX_HIDE_UNCONFIGURE_CONF_PROMPT 0x0040
#define MEBX_MEBX_DEBUG_MSG               0x4000
#define MEBX_UNCONFIGURE                  0x8000

#define MEBX_STF_PCI_DEV_TABLE            0x0001
#define MEBX_STF_MEDIA_DEV_TABLE          0x0002

#define MEBX_MEDIA_IN_ATA                 0x0000
#define MEBX_MEDIA_IN_ATAPI               0x0001
#define MEBX_MEDIA_IN_SATA                0x0002

#define MEBX_MEDIA_DT_HDD                 0x0000
#define MEBX_MEDIA_DT_CDROM               0x0001
#define MEBX_MEDIA_DT_DVD                 0x0002
#define MEBX_MEDIA_DT_FDD                 0x0003
#define MEBX_MEDIA_DT_MO                  0x0004

///
/// MEBX Software Class DXE Subclass Progress Code definitions.
///
#define EFI_SW_DXE_MEBX_OPROM_DONE              (EFI_OEM_SPECIFIC | 0x00000000)
#define EFI_SW_DXE_MEBX_OPROM_NOT_STARTED       (EFI_OEM_SPECIFIC | 0x00000001)
#define EFI_SW_DXE_MEBX_SLEEP_S4_NOT_TAKEN      (EFI_OEM_SPECIFIC | 0x00000002)
#define EFI_SW_DXE_MEBX_SLEEP_S5_NOT_TAKEN      (EFI_OEM_SPECIFIC | 0x00000003)
#define EFI_SW_DXE_MEBX_SLEEP_UNSPECD_NOT_TAKEN (EFI_OEM_SPECIFIC | 0x00000004)
#define EFI_SW_DXE_MEBX_ERROR_ACTION_INTF       (EFI_OEM_SPECIFIC | 0x00000005)
#define EFI_SW_DXE_MEBX_ERROR_ACTION_INV        (EFI_OEM_SPECIFIC | 0x00000006)
#define EFI_SW_DXE_MEBX_ERROR_ACTION_SETUP      (EFI_OEM_SPECIFIC | 0x00000007)
#define EFI_SW_DXE_MEBX_ERROR_ACTION_FAIL       (EFI_OEM_SPECIFIC | 0x00000008)
#define EFI_SW_DXE_MEBX_ERROR_ACTION_UNSPECD    (EFI_OEM_SPECIFIC | 0x00000009)
#define EFI_SW_DXE_MEBX_OTHER_UNSPECD           (EFI_OEM_SPECIFIC | 0x00000009)
#define EFI_SW_DXE_MEBX_RESET_ACTION            (EFI_OEM_SPECIFIC | 0x0000000a)

#pragma pack(1)

#define DEVICES_LIST_VERSION            0x0001
#define AMT_DATA_VERSION                0x0101
#define MEBX_SETUP_VERSION              0x9000
#define MEBX_VERSION                    0x9000      ///< for MEBx 9.0.0 or later
#define MEBX_SETUP_PLATFORM_MNT_DEFAULT MNT_OFF
#define MEBX_SETUP_SOL_IDER_DEFAULT     0
#define PCI_DEVICE_MAX_NUM              256
#define MEDIA_DEVICE_MAX_NUM            32

//      SMB_SIG                         0x5F4D535F
#define SMB_SIG_0                       0x5F
#define SMB_SIG_1                       0x53
#define SMB_SIG_2                       0x4D
#define SMB_SIG_3                       0x5F
#define RSDT_SIG                        0x54445352  ///< RSDT (in reverse)
#define ASF_SIG                         0x21465341  ///< ASF! (in reverse)
#define ONEMIN                          60000000
#define FIVEMIN                         300000000
#define TENMIN                          600000000

#define INVOKE_MEBX_BIT                 0x08
#define WDT_TIMEOUT                     8           ///< in seconds
typedef enum _HWAI_FRU_TYPE_
{
  HWAI_FRU_TYPE_NONE= 0,
  HWAI_FRU_TYPE_PCI
} HWAI_FRU_TYPE;

typedef struct {
  UINT16  VersionInfo;
  UINT8   DevCount;
  UINT8   Reserved1[5];
} AMT_FRU_DEVICES_HEADER;

typedef struct {
  UINT16  Vid;
  UINT16  Did;
  UINT16  Rid;
  UINT16  SubSysVid;
  UINT16  SubSysDid;
  UINT32  ClassCode;
  UINT16  BusDevFcn;
} MEBX_FRU_PCI_DEVICES;

typedef struct {
  AMT_FRU_DEVICES_HEADER  PciDevicesHeader;
  MEBX_FRU_PCI_DEVICES    PciDevInfo[PCI_DEVICE_MAX_NUM];
} AMT_PCI_FRU;

typedef struct _PCI_DEV_INFO {
  UINT16  VersionInfo;
  UINT8   PciDevCount;
  UINT8   Reserved[5];
} PCI_DEV_INFO;

typedef struct _HWAI_PCI_FRU_DATA_ {
  UINT16  VendorID;
  UINT16  DeviceID;
  UINT16  RevID;
  UINT16  SubsystemVendorID;
  UINT16  SubsystemDeviceID;
  UINT32  ClassCode;
  UINT16  BusDevFunc;
} HWAI_PCI_DATA;

typedef struct _HWAI_PCI_FRU_ENTRY {
  UINT8         SmbiosType;
  UINT8         Length;
  UINT16        SmbiosHandle;
  UINT32        FruType;
  HWAI_PCI_DATA FruData;
} HWAI_PCI_FRU;

typedef struct _HWAI_FRU_TABLE_HEADER {
  UINT16        TableByteCount;
  UINT16        StructureCount;
  HWAI_PCI_FRU  Data[1];
} HWAI_FRU_TABLE;

typedef struct _HWAI_MEDIA_DATA_ {
  UINT16  Length;
  UINT8   Interface;
  UINT8   DevType;
  UINT8   Manufacturer[40];
  UINT8   SerialNumber[20];
  UINT8   Version[8];
  UINT8   ModelNumber[40];
  UINT32  MaxMediaSizeLow;
  UINT32  MaxMediaSizeHigh;
  UINT16  Capabilities[3];
} HWAI_MEDIA_DATA;

typedef struct _HWAI_MEDIA_ENTRY {
  UINT8           SmbiosType;
  UINT8           Length;
  UINT16          SmbiosHandle;
  HWAI_MEDIA_DATA MediaData;
} HWAI_MEDIA_ENTRY;

typedef struct _HWAI_MEDIA_DEVICE_TABLE_HEADER {
  UINT16            TableByteCount;
  UINT16            StructureCount;
  HWAI_MEDIA_ENTRY  Data[1];
} HWAI_MEDIA_TABLE;

typedef struct _MEDIA_DEV_INFO {
  UINT16  VersionInfo;
  UINT8   MediaDevCount;
  UINT8   Reserved[5];
} MEDIA_DEV_INFO;

typedef struct {
  UINT16  StructSize;
  UINT8   Interface;
  UINT8   DevType;
  UINT8   Rsvd[40];
  UINT8   SerialNo[20];
  UINT8   VersionNo[8];
  UINT8   ModelNo[40];
  UINT64  MaxMediaSize;
  UINT16  SupportedCmdSets[3];
} MEBX_FRU_MEDIA_DEVICES;

typedef struct {
  AMT_FRU_DEVICES_HEADER  MediaDevicesHeader;
  MEBX_FRU_MEDIA_DEVICES  MediaDevInfo[MEDIA_DEVICE_MAX_NUM];
} AMT_MEDIA_FRU;

#define PasswordWriteDisabled 0
#define PasswordWriteEnabled  1

///
/// Me States
///
#define ME_STATE_TEMP_DISABLED  1

typedef struct _MEBX_DEBUG_FLAGS_ {
  UINT16  Port80 : 1;   ///< Port 80h
  UINT16  SerialIO : 1; ///< Serial Port
  UINT16  Rsvd : 14;    ///< Reserved
} MEBX_DEBUG_FLAGS;

typedef struct _MEBX_OEM_RESOLUTION_SETTINGS_ {
  UINT16  MebxNonUiTextMode : 4;
  UINT16  MebxUiTextMode : 4;
  UINT16  MebxGraphicsMode : 4;
  UINT16  Rsvd : 4;
} MEBX_OEM_RESOLUTION_SETTINGS;

typedef struct {
  UINT16                        BpfVersion;
  UINT8                         CpuReplacementTimeout;
  UINT8                         Reserved[7];
  UINT8                         ActiveRemoteAssistanceProcess;
  UINT8                         CiraTimeout;
  UINT16                        OemFlags;
  MEBX_DEBUG_FLAGS              MebxDebugFlags;
  UINT32                        MeBiosSyncDataPtr;
  UINT32                        UsbKeyDataStructurePtr;
  MEBX_OEM_RESOLUTION_SETTINGS  OemResolutionSettings;
  UINT8                         Reserved3[0x2E];
} MEBX_BPF;

typedef struct {
  UINT16  VersionInfo;
  UINT8   Reserved[2];
  UINT32  PtrSmbiosTable;
  UINT32  PtrAcpiRsdt;
  UINT8   GetSpecCmd;
  UINT8   Reserved1;
  UINT16  GetSpecCmdParam;
  UINT16  SupportedTablesFlags;
  UINT32  PciDevAssertInfoPtr;
  UINT32  MediaDevAssetInfoPtr;
} AMT_DATA;

typedef union {
  UINT32  Data32;
  UINT16  Data16[2];
  UINT8   Data8[4];
} DATA32_UNION;

typedef struct {
  UINT8 SubCommand;
  UINT8 Version;
  UINT8 Data0_7;
  UINT8 Data8_15;
  UINT8 Data16_23;
  UINT8 Data24_31;
} EFI_ASF_MEBX_MESSAGE;

typedef struct _EFI_HECI_ASF_BIOS_EXTENSION_MESSAGE {
  UINT8                 Command;
  UINT8                 ByteCount;
  EFI_ASF_MEBX_MESSAGE  AsfMebxMessage;
} EFI_HECI_ASF_BIOS_EXTENSION_MESSAGE;

#pragma pack()

#define EFI_HECI_ASF_BIOS_EXTENSION_MESSAGE_LENGTH  0x08

#define EFI_ASF_MESSAGE_SUBCOMMAND_MEBX_VALUE       0x80
#define EFI_ASF_MESSAGE_SUBCOMMAND_MEBX_END         0x81
#define EFI_ASF_MESSAGE_VERSIONNUMBER               0x10
#define EFI_ASF_MEBX_MESSAGE_LENGTH                 0x06
#define EFI_ASF_MESSAGE_COMMAND_MANAGEMENT_CONTROL  0x02

///
/// SMBUS PCI Function Address for AMT BIOS Extension
///
#define SMBUS_PFA (PCI_DEVICE_NUMBER_PCH_SMBUS << 3 | PCI_FUNCTION_NUMBER_PCH_SMBUS)

EFI_EVENT             mExitBootServicesEvent;
EFI_EVENT             mLegacyBootEvent;
EFI_EVENT             mBlockIOProtocolEvent;
EFI_EVENT             mBuildMediaEvent;
EFI_EVENT             mBuildUsbProvisioningEvent;

extern EFI_HANDLE     gBdsImageHandle;

EFI_STATUS
AdjustAndExecuteMebxImage (
  IN  VOID                        *MebxBiosParamsBuffer
  )
/**

  Detect EFI MEBx support; Loading and execute.

  @param[in] MebxBiosParamsBuffer MebxBiosParams Flat pointer

**/
;

void
GotoS4 (
  VOID
  )
/**

  Enter the leading movements of S4

  @param[in] None


**/
;

EFI_STATUS
MebxNotifyEvent (
  VOID
  )
/**

  Need to treat iAMT BX (BIOS Extensions) 16 bit binary like a 16 bit PCI OPROM and
  match with i82573E HW device.  No Shadow RAM required and different header so need
  special code to deal with this. This routine is run at boot time.
  1) Check if need to do ptbx invocation or not based on setup etc.
  2) This driver interface abstracts PTBX from rest of tiano and creates independant
     interface for its execution.
  3) getptbxrom via above call interface.
  4) Check status of ptbx rom call and if ok do following:
     a) get ram to put PTBX 16 bit real mode binary data in based on size returned from call in #3 above.
     b) use info from call to get and copy ptbx binary code to allocated region.
     b) ram can be trashed once control returns in here after ptbx execution,
     c) execute ptbx.
     d) free up used ram for ptbx as done now.
  5) Send end of pre-boot to AMT
  6) Return to caller.

  @param[in] None.

  @retval EFI_OUT_OF_RESOURCES    Unable to allocate necessary data structure.
  @retval EFI_NOT_FOUND           1.5M FW SKU detected - there is no MEBx on 1.5 FW SKU
**/
;

VOID
AmtDeviceConfigure (
  IN  ME_BIOS_EXTENSION_SETUP  *MeBiosExtensionSetupData
  )
/**

  Check the status of Amt devices

  @param[in] MeBiosExtensionSetupData   MEBx setting data


**/
;

EFI_STATUS
AmtReadyToBoot (
  VOID
  )
/**
  Signal a event for Amt ready to boot.

  @param[in] None

  @retval EFI_SUCCESS             Mebx launched or no controller

**/
;

VOID
CheckForBiosInvokeReason (
  IN  UINT8       *InvokeMebx,
  IN  MEBX_BPF    *MebxBiosParams
  )
/**

  Detect BIOS invoke reasons.

  @param[in] InvokeMebx           Pointer to the Invoke MEBx flag
  @param[in] MebxBiosParamsBuffer MebxBiosParams Flat pointer


**/
;

VOID
DxeMebxBiosParamsDebugDump (
  IN MEBX_BPF *MebxBiosParams
  )
/**

  Dump MEBx BIOS Params

  @param[in]   MebxBiosParams - MEBx BIOS params

**/
;

extern AMT_MEDIA_FRU  mAmtMediaFru;
extern AMT_PCI_FRU    mAmtPciFru;

//
// USB Provisioning
//
extern VOID           *mUsbProvData;
extern VOID           *mUsbProvDataBackup;
extern UINTN          mUsbProvDataSize;
extern BOOLEAN        mUsbProvsionDone;
extern UINT8          mUsbKeyBus, mUsbKeyPort, mUsbKeyDevice, mUsbKeyFunction;

#ifndef EFI_DEADLOOP
#define EFI_DEADLOOP() { volatile UINTN __DeadLoopVar__ = 1; while (__DeadLoopVar__); }
#endif
#endif // _AMT_PTBX_H_
