//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c) 2011 - 2014, Intel Corporation. All rights reserved.
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

--*/

#ifndef __AMT_PET_ALERT_H__
#define __AMT_PET_ALERT_H__

#include "Amt.h"
#include "AmtLib.h"
#include "MeLib.h"
#include <Library/BdsLib.h>
#include <Library/GenericBdsLib.h>
#include "HeciRegs.h"
#include "AmtPolicyLib.h"

// BAE device type defines
#define BAE_NETWORK_DEVICE    0x1
#define BAE_HDD_DEVICE        0x2
#define BAE_REMOVABLE_DEVICE  0x3
#define BAE_EMPTY_QUEUE       0x7F

// Based on ATA/ATAPI-6
typedef struct _IDENTIFY_DATA{

  UINT16  General_Config_0;
  UINT16  Reserved_1;
  UINT16  Special_Config_2;
  UINT16  Reserved_3;
  UINT16  Reserved_4;
  UINT16  Reserved_5;
  UINT16  Reserved_6;
  UINT16  Reserved_7;
  UINT16  Reserved_8;
  UINT16  Reserved_9;
  UINT8 Serial_Number_10[20];
  UINT16  Reserved_20;
  UINT16  Reserved_21;
  UINT16  Reserved_22;
  UINT8 Firmware_Revision_23[8];
  UINT8 Model_Number_27[40];
  UINT16  Maximum_Sector_Multiple_Command_47;
  UINT16  Reserved_48;
  UINT16  Capabilities_49;
  UINT16  Capabilities_50;
  UINT16  PIO_Mode_51;
  UINT16  Reserved_52;
  UINT16  Valid_Bits_53;
  UINT16  Reserved_54_58[5];
  UINT16  Valid_Bits_59;
  UINT32  Addressable_Sector_60;
  UINT16  SingleWord_DMA_62;
  UINT16  MultiWord_DMA_63;
  UINT16  PIO_Mode_64;
  UINT16  Min_Multiword_DMA_timing_65;
  UINT16  Manuf_Multiword_DMA_timing_66;
  UINT16  Min_PIO_Mode_timing_67;
  UINT16  Min_PIO_Mode_timing_68;
  UINT16  Reserved_69_74[6];
  UINT16  Queue_Depth_75;
  UINT16  Reserved_76_79[4];
  UINT16  Major_Revision_80;
  UINT16  Minor_Revision_81;
  UINT16  Command_Set_Supported_82;
  UINT16  Command_Set_Supported_83;
  UINT16  Command_Set_Supported_84;
  UINT16  Command_Set_Enabled_85;
  UINT16  Command_Set_Enabled_86;
  UINT16  Command_Set_Enabled_87;
  UINT16  UDMA_Mode_88;
  UINT16  Time_security_Earse_89;
  UINT16  Time_Esecurity_Earse_90;
  UINT16  Current_Power_Level_91;
  UINT16  Master_Password_Rev_92;
  UINT16  Hard_Reset_Value_93;
  UINT16  Acoustic_Level_94;
  UINT16  Reserved_95_99[5];
  UINT64  LBA_48;
  UINT16  Reserved_104_126[23];
  UINT16  Status_Notification_127;
  UINT16  Security_Status_128;
  UINT16  Reserved_129_159[31];
  UINT16  CFA_Power_Mode_160;
  UINT16  Reserved_161_175[15];
  UINT16  Media_Serial_Number_176_205[30];
  UINT16  Reserved_206_254[49];
  UINT16  Checksum_255;
} IDENTIFY_DATA;

#pragma pack(push,1)
typedef struct {
  UINT32 Attributes;
  UINT16 FilePathListLength;
} EFI_LOAD_OPTION;

// Example 1: 16 10 0F 6F 02 68 08 FF FF 00 00 40 13 XX XX XX
// Example 2: 15 10 0F 6F 02 68 10 FF FF 22 00 AA 13 03 03 02

typedef struct {
  UINT8           SubCommand;     // 0x00
  UINT8           VersionNumber;    // 0x01
  UINT8           EventSensorType;  // 0x02
  UINT8           EventType;      // 0x03
  UINT8           EventOffset;    // 0x04
  UINT8           EventSourceType;  // 0x05
  UINT8           EventSeverity;    // 0x06
  UINT8           SensorDevice;   // 0x07
  UINT8           Sensornumber;   // 0x08
  UINT8           Entity;       // 0x09
  UINT8           EntityInstance;   // 0x0A
  UINT8           EventData1;     // 0x0B
  UINT8           EventData2;     // 0x0C
  UINT8           EventData3;     // 0x0D
  UINT8           EventData4;     // 0x0E
  UINT8           EventData5;     // 0x0F
} BOOT_AUDIT_ENTRY;

typedef struct {
  UINT8           Command;
  UINT8           ByteCount;
  BOOT_AUDIT_ENTRY      Data;
} BOOT_AUDIT_ENTRY_PACK;

typedef struct {
  UINT8           BootQueue[3];
  UINT8           BootHarddriveTag[64];
} PET_ALERT_CFG;

#pragma pack(pop)

extern EFI_GUID gEfiSetupVariableGuid;

VOID
AmtPetAlertReadyToBoot(
  EFI_EVENT           Event,
    VOID                *ParentImageHandle
);

EFI_STATUS
AmtPetAlertLegacyBoot(
  EFI_EVENT           Event,
    VOID                *ParentImageHandle
);

#endif
