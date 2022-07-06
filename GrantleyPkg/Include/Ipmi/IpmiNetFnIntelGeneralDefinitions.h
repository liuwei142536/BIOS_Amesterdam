//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c)  1999 - 2009 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
  IpmiNetFnIntelGeneralDefinitions.h
  
Abstract:
  The file has all the definitions required for Intel Specific commands
--*/

#ifndef _IPMINETFNINTELGENERALDEFINITIONS_H_
#define _IPMINETFNINTELGENERALDEFINITIONS_H_

//
// Net function definition for Intel OEM specific command
//
#define EFI_SM_NETFN_INTEL_OEM  0x30

/*----------------------------------------------------------------------------------------
    Definitions for Get Expection Address command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_GET_EXCEPTION_ADDRESS 0x01

//
//  Constants and Structure definitions for "Get Exception Address" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Restore Configuration command   
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_RESTORE_CONFIGURATION 0x02

//
//  Constants and Structure definitions for "Restore Configuration" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get SM Signal command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_GET_SM_SIGNAL 0x14

//
//  Constants and Structure definitions for "Get SM Signal" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set SM Signal command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_SET_SM_SIGNAL 0x15

//
//  Constants and Structure definitions for "Set SM Signal" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Read Self Test command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_READ_SELF_TEST  0x16

//
//  Constants and Structure definitions for "Read Self Test" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Write BMC Memory command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_WRITE_BMC_MEMORY  0x1A

//
//  Constants and Structure definitions for "Write BMC Memory" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Read BMC Memory command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_READ_BMC_MEMORY 0x1B

//
//  Constants and Structure definitions for "Read BMC Memory" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Secure Mode Options command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_SET_SECURE_MODE_OPTIONS 0x1E

//
//  Constants and Structure definitions for "Set Secure Mode Options" command to follow here
//
#pragma pack(1)
typedef union {
  UINT8 AsUint8;
  struct {
    UINT8 BlankVideo : 1;
    UINT8 WriteProtectDiskette : 1;
    UINT8 LockNmiButton : 1;
    UINT8 LockSleepButton : 1;
    UINT8 LockResetButton : 1;
    UINT8 LockPowerButton : 1;
    UINT8 LockFrontPanel : 1;
    UINT8 ForceSecureMode : 1;
  } s;
} SECURE_MODE_OPTIONS;

typedef struct {
  SECURE_MODE_OPTIONS Options;
} SET_SECURE_MODE_OPTIONS;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Get Secure Mode Options command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_GET_SECURE_MODE_OPTIONS 0x1F

//
//  Constants and Structure definitions for "Get Secure Mode Options" command to follow here
//
#pragma pack(1)
typedef struct {
  SECURE_MODE_OPTIONS Options;
  UINT8               Reserved : 7;
  UINT8               SecureModeAsserted : 1;
} GET_SECURE_MODE_OPTIONS;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Get Power Supply State command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_GET_POWER_SUPPLY_STATE  0x20

//
//  Constants and Structure definitions for "Get Power Supply State" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get DIMM State command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_GET_DIMM_STATE  0x21

//
//  Constants and Structure definitions for "Get DIMM State" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set DIMM State command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_SET_DIMM_STATE  0x22

//
//  Constants and Structure definitions for "Set DIMM State" command to follow here
//
#pragma pack(1)
typedef struct {
  UINT8 GroupSelector;    // DIMM Group ID
  UINT8 DimmSlotMask;     // Mask of physical DIMM slots belonging to this group
  UINT8 FailedDimmMask;   // Mask of failed DIMMs belonging to this group
  UINT8 DisabledDimmMask; // Mask of disabled DIMMs belonging to this group
  UINT8 SpareDimmMask;    // Mask of Spare DIMMs belonging to this group
  UINT8 PresentDimmMask;  // Mask of Present DIMMs belonging to this group
} EFI_SET_DIMM_STATE_INFO;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for ReArm DIMMS command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_REARM_DIMMS 0x23

//
//  Constants and Structure definitions for "ReArm DIMMS" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Control Memory Monitoring command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_CONTROL_MEMORY_MONITORING 0x24

//
//  Constants and Structure definitions for "Control Memory Monitoring" command to follow here
//
#pragma pack(1)
typedef struct {
  UINT8 Enable : 1;   // 1 - Enable, 0 - Disable
  UINT8 Reserved : 7; // Not used...
} EFI_SET_MEMORY_MONITORING_INFO;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Acquire System Resource Semaphore command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_ACQUIRE_SEMAPHORE 0x25

//
//  Constants and Structure definitions for "Acquire System Resource Semaphore" command to follow here
//
#pragma pack(1)
typedef struct {
  UINT8 SystemResource;   // 00h - SMBus, All other values reserved
  UINT8 Instance;         // 0-CPU#1, 1-CPU#2, 2-CPU#3, 3-CPU#4, All other values reserved
  UINT8 Action : 1;       // 0-Release Semaphore, 1-Acquire Semaphore
  UINT8 Reserved : 7;
} EFI_ACQUIRE_SYSTEM_RESOURCE_SEMAPHORE;

typedef struct {
  UINT8 CompletionCode;
} EFI_ACQUIRE_SYSTEM_RESOURCE_SEMAPHORE_RESPONSE;
#pragma pack()

#define EFI_INTEL_OEM_SYSTEM_RESOURCE_SMBUS         0x00
#define EFI_INTEL_OEM_SMBUS_SEMAPHORE_RELEASE_LOCK  0
#define EFI_INTEL_OEM_SMBUS_SEMAPHORE_ACQUIRE_LOCK  1

/*----------------------------------------------------------------------------------------
    Definitions for Set Processor State command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_SET_PROCESSOR_STATE 0x28

//
//  Constants and Structure definitions for "Set Processor State" command to follow here
//
#pragma pack(1)
typedef struct _EFI_BMC_CPU_STATE {
  UINT16  IERR : 1;
  UINT16  ThermTrip : 1;
  UINT16  BistFailure : 1;
  UINT16  PostHang : 1;
  UINT16  StartFailure : 1;
  UINT16  ConfigError : 1;
  UINT16  SmbiosError : 1;
  UINT16  CpuPresent : 1;
  UINT16  CpuDisabled : 1;
  UINT16  Reserved : 7;
} EFI_BMC_CPU_STATE;

typedef struct _EFI_BMC_CPU_STATE_ACTION {
  UINT8 ResetSystem : 1;
  UINT8 Reserved : 7;
} EFI_BMC_CPU_STATE_ACTION;

typedef struct _EFI_BMC_SET_CPU_STATE_REQUEST {
  UINT8                     ProcessorId;
  EFI_BMC_CPU_STATE         NewState;
  EFI_BMC_CPU_STATE_ACTION  Action;
} EFI_BMC_SET_CPU_STATE_REQUEST;
#pragma pack()
/*----------------------------------------------------------------------------------------
    Definitions for Get Processor State command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_GET_PROCESSOR_STATE 0x29

//
//  Constants and Structure definitions for "Get Processor State" command to follow here
//
#pragma pack(1)
typedef struct _EFI_BMC_GET_CPU_STATE_REQUEST {
  UINT8 ProcessorId;
} EFI_BMC_GET_CPU_STATE_REQUEST;

typedef struct _EFI_BMC_GET_CPU_STATE_RESPONSE {
  EFI_BMC_CPU_STATE CurrentState;
} EFI_BMC_GET_CPU_STATE_RESPONSE;
#pragma pack()
/*----------------------------------------------------------------------------------------
    Definitions for Rearm processors command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_REARM_PROCESSORS  0x2A

//
//  Constants and Structure definitions for "Rearm processors" command to follow here
//
#pragma pack(1)
typedef struct _EFI_BMC_REARM_ALL_CPUS_REQUEST {
  UINT8 Command;
} EFI_BMC_REARM_ALL_CPUS_REQUEST;
#pragma pack()
/*----------------------------------------------------------------------------------------
    Definitions for Set Processor TControl command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_SET_PROCESSOR_TCONTROL  0x2C

//
//  Constants and Structure definitions for "Set Processor TControl" command to follow here
//
#pragma pack(1)
typedef struct {
  UINT8 ProcessorId;    // Processor ID
  UINT8 TControlValue;  // Processor TControl value
} EFI_BMC_SET_PROCESSOR_TCONTROL;
#pragma pack()
/*----------------------------------------------------------------------------------------
    Definitions for NPTM INFO
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_SET_NODE_MANAGER_PLATFORM_INFO 0x5A

#pragma pack(1)
typedef struct {
  UINT32 TdpHigh63to32;
  UINT32 TdpLow31to0;
  UINT32 PlatformInfo63to32;
  UINT32 PlatformInfo31to0;
  UINT16 MinPower; 
  UINT16 MaxPower;
  UINT8  TotalPStates;
  UINT8  TotalTStates;
} EFI_SET_NODE_MANAGER_PLATFORM_INFO;
#pragma pack()


/*----------------------------------------------------------------------------------------
    Definitions for Chassis identify LED command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_CHASSIS_IDENTIFY_LED  0x36

//
//  Constants and Structure definitions for "Chassis identify LED" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Resolve IP Address command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_RESOLVE_IPADDRESS 0x40

//
//  Constants and Structure definitions for "Resolve IP Address" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set System GUID command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_SET_SYSTEM_GUID 0x41

//
//  Constants and Structure definitions for "Set System GUID" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Auxillary Channel Info command
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_SET_AUX_CHANNEL_INFO  0x42

//
//  Constants and Structure definitions for "Set Auxillary Channel Info" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Internal Platform Event Message command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_INTENAL_PLATFORM_EVENT_MSG  0x48

//
//  Constants and Structure definitions for "Internal Platform Event Message" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Fault Indication command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_SET_FAULT_INDICATION  0x57

//
// Support for System Fault Indication...
// Used by the runtime Memory RAS handlers...
//
#define BIOS                                3
#define EFI_FAULT_SOURCE_ID                 BIOS

typedef enum {
  EfiSmFanFault = 0x00,
  EfiSmTemperatureFault,
  EfiSmPowerFault,
  EfiSmDriveFault,
  EfiSmSoftwareFault = 0x04,
  EfiDimmFault = 0x05,
} EFI_FAULT_INDICATION_CATEGORY;

#define EFI_MEMORY_FAULT_INDICATION_TYPE    (UINT8)EfiDimmFault

//
//  Constants and Structure definitions for "Set Fault Indication" command to follow here
//
#pragma pack(1)
typedef enum {
  EfiSmFaultIndicationLedStatusOkay           = 0x00,
  EfiSmFaultIndicationLedStatusDegraded,
  EfiSmFaultIndicationLedStatusNonCritical,
  EfiSmFaultIndicationLedStatusCritical,
  EfiSmFaultIndicationLedStatusNonRecoverable
} EFI_FAULT_INDICATION_STATE;

typedef struct _EFI_SET_FAULT_INDICATION {
  UINT8 SourceId;
  UINT8 FaultType;
  UINT8 NewState;
  UINT8  SubSystem;     //Used to select groupings of 32 DIMMs.
  UINT32 DIMMBitField;  //32bit field to indicate DIMMs BMC will light
} EFI_SET_FAULT_INDICATION;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Set Platform Information command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_SET_PLATFORM_INFORMATION  0x63

//
//  Constants and Structure definitions for "Set Platform  Information" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Platform Information command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_GET_PLATFORM_INFORMATION  0x64

//
//  Constants and Structure definitions for "Get Platform  Information" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Graceful OS Shutdown command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_GRACEFUL_OS_SHUTDOWN  0x70

//
//  Constants and Structure definitions for "Graceful OS Shutdown" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Init Agent Started command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_INIT_AGENT_STARTED  0x80

//
//  Constants and Structure definitions for "Init Agent Started" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Init Agent End command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_INIT_AGENT_END  0x81

//
//  Constants and Structure definitions for "Init Agent End" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get ACPI configuration command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_GET_ACPI_CONFIGURATION  0x82

//
//  Constants and Structure definitions for "Get ACPI configuration" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set ACPI configuration command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_SET_ACPI_CONFIGURATION  0x83

//
//  Constants and Structure definitions for "set ACPI configuration" command to follow here
//
#pragma pack(1)
typedef struct {
  UINT8 NewState;
  UINT8 Mask;
} EFI_BMC_ACPI_REQUEST;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Send RMCP Pong command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_SEND_RMCP_PONG  0x84

//
//  Constants and Structure definitions for "Send RMCP Pong" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Memory RAS Reduncancy State command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_SET_RAS_REDUNDANCY_STATE  0x89

//
//  Constants and Structure definitions for "Set Memory RAS Reduncancy State" command to follow here
//
#pragma pack(1)
typedef struct {
  UINT8 DomainSelector; // Bits[7:4]: Domain Type, Bits[3:0]: Domain Instance
  // (Spare 1, 2) (Mirror 1|2), RAID (none)
  //
  UINT8 DomainState;  // "Redundant"/"Not Redundant"/"Failed"
} EFI_SET_MEM_RAS_REDUNDANCY_INFO;
#pragma pack()
/*----------------------------------------------------------------------------------------
    Definitions for Get Memory RAS Reduncancy State command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_GET_RAS_REDUNDANCY_STATE  0x8A

//
//  Constants and Structure definitions for "Get Memory RAS Reduncancy State" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Memory RAS Configuration command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_SET_RAS_CONFIGURATION 0x8B

//
//  Constants and Structure definitions for "Set Memory RAS Configuration" command to follow here
//
#pragma pack(1)
typedef struct {
  UINT8 SparingDomainEnableMask;    // Mask of spare groups
  UINT8 MirroringDomainEnableMask;  // Mask of mirror groups - only one possible: 0 (B1MB2)
  UINT8 RaidDomainEnableMask;       // Mask of RAID groups - none supported
} EFI_SET_MEM_RAS_CONFIG_INFO;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Get Memory RAS Configuration command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_GET_RAS_CONFIGURATION 0x8C

//
//  Constants and Structure definitions for "Get Memory RAS Configuration" command to follow here
//
#pragma pack(1)
typedef struct {
  UINT8 SparingDomainEnableMask;    // Mask of which branch has spare units
  UINT8 MirroringDomainEnableMask;  // Mask of mirroring domains: only one possible (B1MB2)
  UINT8 RaidDomainEnableMask;       // Mask of RAID group - none.
} EFI_GET_MEM_RAS_CONFIG_INFO;
#pragma pack()
/*----------------------------------------------------------------------------------------
    Definitions for Get Chassis Name command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_GET_CHASSIS_NAME  0x90

//
//  Constants and Structure definitions for "Get Chassis Name" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Chassis Name command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_SET_CHASSIS_NAME  0x91

//
//  Constants and Structure definitions for "Set Chassis Name" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get NMI Source command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_GET_NMI_SOURCE  0xE6

//
//  Constants and Structure definitions for "Get NMI Source" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set NMI Source command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_SET_NMI_SOURCE  0xED

//
//  Constants and Structure definitions for "Set NMI Source" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get all SEL Entry command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_GET_ALL_SEL_ENTRY 0xEE

//
//  Constants and Structure definitions for "Get all SEL Entry" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for NMI Enable/Disable command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_NMI_ENABLE_DISABLE  0xF7

//
//  Constants and Structure definitions for "NMI Enable/Disable" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Post Progress command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_SET_POST_PROGRESS 0xF8

//
//  Constants and Structure definitions for "Set Post Progress" command to follow here
//
#pragma pack(1)
typedef struct {
  UINT8 AsfCode;
  UINT8 ProgressFlag;
} EFI_ASF_PACKET;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Get Post Progress command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_GET_POST_PROGRESS 0xF9

//
//  Constants and Structure definitions for "Get Post Progress" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Online update prepare for update command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_UPDATE_PREPARE_UPDATE 0xA0

//
//  Constants and Structure definitions for "Online update prepare for update" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Online update Open Area command
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_UPDATE_OPEN_AREA  0xA1

//
//  Constants and Structure definitions for "Online update Open Area" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Online update Write Area command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_UPDATE_WRITE_AREA 0xA2

//
//  Constants and Structure definitions for "Online update Write Area" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Online update Close Area command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_UPDATE_CLOSE_AREA 0xA3

//
//  Constants and Structure definitions for "Online update Close Area" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Online update Close Area command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_UPDATE_REGISTER_UPDATE  0xA4

//
//  Constants and Structure definitions for "Online update Register Update" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Online update Capture Rollback Image command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_UPDATE_CAPTURE_ROLLBACK_IMAGE 0xA5

//
//  Constants and Structure definitions for "Online update Capture Rollback Image" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Online update Get Status Image command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_UPDATE_CAPTURE_GET_STATUS 0xA6

//
//  Constants and Structure definitions for "Online update Get Status" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Online update Get Capabilities command 
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_UPDATE_CAPTURE_GET_CAPABILITIES 0xA7

//
//  Constants and Structure definitions for "Online update Get Capabilities" command to follow here
//

#endif
