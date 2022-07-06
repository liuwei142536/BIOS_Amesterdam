//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/** @file
  All the definitions required for Chassis commands Header File.

  Copyright (c) 1999 - 2014, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.
**/

#ifndef _IPMINETFNCHASSISDEFINITIONS_H_
#define _IPMINETFNCHASSISDEFINITIONS_H_

//
// Net function definition for Chassis command
//
#define EFI_SM_NETFN_CHASSIS  0x00

/*----------------------------------------------------------------------------------------
    Definitions for Get Chassis Capabilities command    
----------------------------------------------------------------------------------------*/
#define EFI_CHASSIS_GET_CAPABILITIES  0x00

//
//  Constants and Structure definitions for "Get Chassis Capabilities" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Chassis Status command  
----------------------------------------------------------------------------------------*/
#define EFI_CHASSIS_GET_STATUS  0x01

//
//  Constants and Structure definitions for "Get Chassis Status" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Chassis Control command     
----------------------------------------------------------------------------------------*/
#define EFI_CHASSIS_CONTROL 0x02

//
//  Constants and Structure definitions for "Chassis Control" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Chassis Reset command   
----------------------------------------------------------------------------------------*/
#define EFI_CHASSIS_RESET 0x03

//
//  Constants and Structure definitions for "Chassis Reset" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Chassis Identify command    
----------------------------------------------------------------------------------------*/
#define EFI_CHASSIS_IDENTIFY  0x04

//
//  Constants and Structure definitions for "Chassis Identify" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Chassis Capabilities command    
----------------------------------------------------------------------------------------*/
#define EFI_CHASSIS_SET_CAPABILITIES  0x05

//
//  Constants and Structure definitions for "Set Chassis Capabilities" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Power Restore Policy command    
----------------------------------------------------------------------------------------*/
#define EFI_CHASSIS_SET_POWER_RESTORE_POLICY  0x06

//
//  Constants and Structure definitions for "Set Power Restore Policy" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get System Restart Cause command    
----------------------------------------------------------------------------------------*/
#define EFI_CHASSIS_GET_SYSTEM_RESTART_CAUSE  0x07

//
//  Constants and Structure definitions for "Get System Restart Cause" command to follow here
//
typedef enum {
  Unknown,
  ChassisControlCommand,
  ResetViaPushButton,
  PowerupViaPowerButton,
  WatchdogExpiration,
  Oem,
  AutoPowerOnAlwaysRestore,
  AutoPowerOnRestorePrevious,
  ResetViaPef,
  PowerCycleViaPef,
  SoftReset,
  PowerUpViaRtc
} EFI_SYSTEM_RESTART_CAUSE;

#pragma pack (1)

typedef struct {
  UINT8   Cause:4;
  UINT8   Reserved:4;
  UINT8   ChannelNumber;
} GET_SYSTEM_RESTART_CAUSE_RESPONSE;

#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Set System BOOT options command     
----------------------------------------------------------------------------------------*/
#define EFI_CHASSIS_SET_SYSTEM_BOOT_OPTIONS 0x08

//
//  Constants and Structure definitions for "Set System boot options" command to follow here
//
#pragma pack (1)

typedef struct {
  UINT8   ParameterSelector:7;
  UINT8    MarkParameterInvalid:1;
  UINT8    ParameterData[1];
} SET_BOOT_OPTIONS_REQUEST;

typedef struct {
  UINT8   CompletionCode:8;
} SET_BOOT_OPTIONS_RESPONSE;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Get System BOOT options command     
----------------------------------------------------------------------------------------*/
#define EFI_CHASSIS_GET_SYSTEM_BOOT_OPTIONS 0x09

//
//  Constants and Structure definitions for "Get System boot options" command to follow here
//
#pragma pack (1)
typedef struct {
  UINT8 Parameter;
  UINT8 Valid;
  UINT8 Data1;
  UINT8 Data2;
  UINT8 Data3;
  UINT8 Data4;
  UINT8 Data5;
} GET_THE_SYSTEM_BOOT_OPTIONS;

typedef struct {
  UINT8   ParameterVersion;
  UINT8   ParameterValid;
  UINT8   ChannelNumber;
  UINT32  SessionId;
  UINT32  TimeStamp;
  UINT8   Reserved[3];
} EFI_BOOT_INITIATOR;

typedef struct {
  UINT8    ParameterSelector;
  UINT8    SetSelector;
  UINT8    BlockSelector;
} GET_BOOT_OPTIONS_REQUEST;

//
// Response Parameters for IPMI Get Boot Options
//
typedef struct {
  UINT8   SetInProgress: 2;
  UINT8   Reserved: 6;
} BOOT_OPTIONS_RESPONSE_PARAMETER_0;

typedef struct {
  UINT8   ServicePartitionSelector;
} BOOT_OPTIONS_RESPONSE_PARAMETER_1;

typedef struct {
  UINT8   ServicePartitionDiscovered:1;
  UINT8   ServicePartitionScanRequest:1;
  UINT8   Reserved: 5;
} BOOT_OPTIONS_RESPONSE_PARAMETER_2;

typedef struct {
  UINT8   BmcBootFlagValid: 5;
  UINT8   Reserved: 3;
} BOOT_OPTIONS_RESPONSE_PARAMETER_3;

typedef struct {
  UINT8   WriteMask;
  UINT8   BootInitiatorAcknowledgeData;
} BOOT_OPTIONS_RESPONSE_PARAMETER_4;

#define BOOT_OPTION_HANDLED_BY_BIOS 0x01

typedef struct {
  //
  // Data 1
  //
  UINT8   Reserved0:6;
  UINT8   PersistentOptions:1;
  UINT8   BootFlagValid:1;
  //
  // Data 2
  //
  UINT8   LockReset:1;
  UINT8   ScreenBlank:1;
  UINT8   BootDeviceSelector:4;
  UINT8   LockKeyboard:1;
  UINT8   CmosClear:1;
  //
  //
  // Data 3
  UINT8   ConsoleRedirection:2;
  UINT8   LockSleep:1;
  UINT8   UserPasswordBypass:1;
  UINT8   ForceProgressEventTrap:1;
  UINT8   BiosVerbosity:2;
  UINT8   LockPower:1;
  //
  // Data 4
  //
  UINT8   BiosMuxControlOverride:2;
  UINT8   BiosSharedModeOverride:1;
  UINT8   Reserved1:4;
} BOOT_OPTIONS_RESPONSE_PARAMETER_5;

typedef struct {
  UINT8   ChannelNumber:4;
  UINT8   Reserved:4;
  UINT8   SessionId[4];
  UINT8   BootInfoTimeStamp[4];
} BOOT_OPTIONS_RESPONSE_PARAMETER_6;

typedef struct {
  UINT8   SetSelector;
  UINT8   BlockData[16];
} BOOT_OPTIONS_RESPONSE_PARAMETER_7;

typedef union {
  BOOT_OPTIONS_RESPONSE_PARAMETER_0   Parm0;
  BOOT_OPTIONS_RESPONSE_PARAMETER_1   Parm1;
  BOOT_OPTIONS_RESPONSE_PARAMETER_2   Parm2;
  BOOT_OPTIONS_RESPONSE_PARAMETER_3   Parm3;
  BOOT_OPTIONS_RESPONSE_PARAMETER_4   Parm4;
  BOOT_OPTIONS_RESPONSE_PARAMETER_5   Parm5;
  BOOT_OPTIONS_RESPONSE_PARAMETER_6   Parm6;
  BOOT_OPTIONS_RESPONSE_PARAMETER_7   Parm7;
} BOOT_OPTIONS_PARAMETERS;

typedef struct {
  UINT8    ParameterVersion:4;
  UINT8    Reserved:4;
  UINT8    ParameterSelector:7;
  UINT8    ParameterValid:1;
  UINT8    ParameterData[1];
} GET_BOOT_OPTIONS_RESPONSE;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Set front panel button enables command  
----------------------------------------------------------------------------------------*/
#define EFI_CHASSIS_SET_FP_BUTTON_ENABLES 0x0A

#pragma pack (1)
typedef struct {
  UINT8    DisablePoweroffButton:1;
  UINT8    DisableResetButton:1;
  UINT8    DisableDiagnosticInterruptButton:1;
  UINT8    DisableStandbyButton:1;
  UINT8    Reserved:4;
} SET_FRONT_PANEL_BUTTON_ENABLES_REQUEST;

#pragma pack()

//
//  Constants and Structure definitions for "Set front panel button enables" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Power Cycle Interval command    
----------------------------------------------------------------------------------------*/
#define EFI_CHASSIS_SET_POWER_CYCLE_INTERVALS 0x0B

//
//  Constants and Structure definitions for "Set Power Cycle Interval" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get POH Counter command     
----------------------------------------------------------------------------------------*/
#define EFI_CHASSIS_GET_POH_COUNTER 0x0F

//
//  Constants and Structure definitions for "Get POH Counter" command to follow here
//

#endif
