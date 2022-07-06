//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c)  1999 - 2010 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
   IpmiNetFnIntelPlatformSpecificDefinitions.h
  
Abstract:
  The file has all the definitions required for Platform Specific commands
--*/

#ifndef _IPMINETFNPLATFORMSPECIFICDEFINITIONS_H_
#define _IPMINETFNPLATFORMSPECIFICDEFINITIONS_H_

//
// Net function definition for Platform Specific command
//
#define EFI_SM_NETFN_PLATFORM 0x32
#define EFI_SM_NETFN_GENERAL_APPLICATION  0x30

/*----------------------------------------------------------------------------------------
    Definitions for Get Expection Address command   
----------------------------------------------------------------------------------------*/
#define EFI_INTEL_OEM_GET_EXCEPTION_ADDRESS 0x01

/*----------------------------------------------------------------------------------------
    Definitions for Get RAM Info command    
----------------------------------------------------------------------------------------*/
#define EFI_PLATFORM_GET_RAM_INFO 0x00

//
//  Constants and Structure definitions for "Get RAM Info" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get LED Status command  
----------------------------------------------------------------------------------------*/
#define EFI_PLATFORM_GET_LED_STATUS 0x40

//
//  Constants and Structure definitions for "Get LED Status" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set PCI-E Link State command    
----------------------------------------------------------------------------------------*/
#define EFI_PLATFORM_SET_PCIE_LINK_STATE  0x50

//
//  Constants and Structure definitions for "Set PCI-E Link State" command to follow here
//
#pragma pack(1)
typedef struct {
  UINT8 LinkEnum;
  UINT8 Action;
  UINT8 ErrorState;
  UINT8 OEMEvData1;
  UINT8 OEMEvData2;
} EFI_SET_LINK_STATE;

typedef struct {
  UINT8 CompletionCode;
  UINT8 CurrentErrorState;
} EFI_SET_LINK_STATE_RESPONSE;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Set ADD-IN Support configuration command    
----------------------------------------------------------------------------------------*/
#define EFI_PLATFORM_SET_ADDIN_SUPPORT_CONFIGURATION  0x70

//
//  Constants and Structure definitions for "Set ADD-IN Support configuration" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get ADD-IN Support configuration command    
----------------------------------------------------------------------------------------*/
#define EFI_PLATFORM_GET_ADDIN_SUPPORT_CONFIGURATION  0x71

//
//  Constants and Structure definitions for "Get ADD-IN Support configuration" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set FAN profile command     
----------------------------------------------------------------------------------------*/
#define EFI_PLATFORM_SET_FAN_PROFILE  0x89

//
//  Constants and Structure definitions for "Set FAN Profile" command to follow here
//
#pragma pack(1)
typedef struct {
  UINT8   Profile;
  UINT8   MemoryThrottlingMode:2;  // 0 - not supported, 1 - OLTT, 2 - CLTT,  3 - reserved 
  UINT8   DimmStatus:1;            // 0 - not started or in-progress, 1 - Completed
  UINT8   Reserved:5;
  UINT8   GroupId;
  UINT32  DimmPresenceBitMap;
} EFI_SET_FAN_PROFILE;

typedef struct {
  UINT8 TempData;
} EFI_SET_FAN_PROFILE_RESPONSE;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Get FAN profile command     
----------------------------------------------------------------------------------------*/
#define EFI_PLATFORM_GET_FAN_PROFILE  0x75

//
//  Constants and Structure definitions for "Get FAN Profile" command to follow here
//
#pragma pack(1)
typedef struct {
  UINT8   FanControlProfileSupport;
  UINT8   FanControlProfileEnable;
  UINT8   MemoryThrottlingMode:2;  // 0 - not supported, 1 - OLTT, 2 - CLTT,  3 - reserved 
  UINT8   Reserved:6;
} EFI_GET_FAN_PROFILE_RESPONSE;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Get Thermal Profile Data command    
----------------------------------------------------------------------------------------*/
#define EFI_PLATFORM_GET_THERMAL_PROFILE_DATA 0x8B

#pragma pack(1)

typedef struct {
  UINT8   MemoryThrottlingMode:2;  // 0 - not supported, 1 - OLTT, 2 - CLTT,  3 - reserved 
  UINT8   Reserved:6;
  UINT8   Profile;
} EFI_GET_THERMAL_PROFILE_DATA;

typedef struct {
  UINT8   SdrHeader[5]; // This is the 5-byte SDR header, which is not used but needs to be accounted for...
  UINT8   OemId1;
  UINT8   OemId2;
  UINT8   OemId3;
  UINT8   RecordSubtype;
  UINT8   MemoryThrottlingMode:2;  // 0 - not supported, 1 - OLTT, 2 - CLTT,  3 - reserved 
  UINT8   FlagsReserved:6;
  UINT8   ProfileSupportBitmap;
  UINT8   TempInlet;
  UINT8   TempRise;
  UINT16  AirFlow;
  UINT16  DimmPitch;

  //
  // Byte 12:
  // Bits [0:1] = Throttling Mode
  //  00b = Disabled
  //  01b = VTS Only (OLTT)
  //  10b = Software-based Mode
  //  11b = EXTTS CLTT
  //
  UINT8   ThrottleMode:2;
  UINT8   Reserved1:6;
  
  //
  // Byte 13:
  //  Bit [0] = Lock
  //  Bits[1:2] = Hysteresis
  //    00b = Disable Hysteresis
  //    01b = 1.5 'C
  //    10b = 3.0 'C
  //    11b = 6.0 'C
  //
  //  Bit[3] = Event Control
  //    0b = Event generated on Thigh, Tlow as well as Tcrit
  //    1b = Event generated only on Tcrit
  //
  UINT8   ThermRegisterLock:1;
  UINT8   Hysteresis:2;
  UINT8   ControlEventMode:1;
  UINT8   Reserved2:4;

  //
  // The following parameters are legacy and no longer applicable to Thurley.
  // They should be ignored...
  //

  UINT16  ThrtMidRatio;         
  UINT8   TempMidGb;
  UINT8   TempLoGb;
  UINT8   GlobalActRatio;
  UINT8   GlobalThrtEn:1;
  UINT8   ThermalThrtEn:1;
  UINT8   HighTempEn:1;
  UINT8   Rsrvd:5;
  UINT32  Reserved;
} EFI_GET_THERMAL_PROFILE_DATA_RESPONSE;

#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Set Fan Speed Offset command    
----------------------------------------------------------------------------------------*/
#define EFI_PLATFORM_SET_FAN_SPEED_OFFSET 0x8C

/*----------------------------------------------------------------------------------------
    Definitions for Get Fan Speed Offset command    
----------------------------------------------------------------------------------------*/
#define EFI_PLATFORM_GET_FAN_SPEED_OFFSET_DATA 0x8D

/*----------------------------------------------------------------------------------------
    Definitions for Set Dimm Offset command     
----------------------------------------------------------------------------------------*/
#define EFI_PLATFORM_SET_DIMM_OFFSET  0x8E

//
//  Constants and Structure definitions for "Set Dimm Offset" command to follow here
//
#pragma pack(1)
typedef struct {
  UINT8   Profile;                  // 0 - Static CLTT, 1 - Dynamic CLTT
  UINT8   DimmNumber;
  UINT8   DimmOffset1;              // Only DimmOffset1 will be used if Static CLTT, if DCLTT this is offset for 1m/s
  UINT8   DimmOffset2;              // if DCLTT this is the offset for 2m/s
  UINT8   DimmOffset3;              // if DCLTT this is the offset for 5m/s
} EFI_SET_DIMM_OFFSET;

typedef struct {
  UINT8 TempData;
} EFI_SET_DIMM_OFFSET_RESPONSE;
#pragma pack()

#endif
