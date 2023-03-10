//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file
  IPMI 2.0 definitions from the IPMI Specification Version 2.0, Revision 1.1.

  This file contains all NetFn Sensor/Event commands, including:
    Event Commands (Chapter 29)
    PEF and Alerting Commands (Chapter 30)
    Sensor Device Commands (Chapter 35)

  See IPMI specification, Appendix G, Command Assignments
  and Appendix H, Sub-function Assignments.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

// AMI_IPMI_PORTING_START

#ifndef _IPMINETFNSENSORDEFINITIONS_H_
#define _IPMINETFNSENSORDEFINITIONS_H_

//
// LUN definition
//
#ifndef BMC_LUN
    #define BMC_LUN     0x00
#endif

// AMI_IPMI_PORTING_END

#ifndef _IPMI_NET_FN_SENSOR_EVENT_H_
#define _IPMI_NET_FN_SENSOR_EVENT_H_

//
// Net function definition for Sensor command
//
#define IPMI_NETFN_SENSOR_EVENT 0x04

//
// All Sensor commands and their structure definitions to follow here
//

//
//  Definitions for Send Platform Event Message command
//
#define IPMI_SENSOR_PLATFORM_EVENT_MESSAGE   0x02

#pragma pack(1)  // AMI_IPMI_PORTING
typedef struct {
  UINT8   GeneratorId;
  UINT8   EvMRevision;
  UINT8   SensorType;
  UINT8   SensorNumber;
  UINT8   EventDirType;
  UINT8   OEMEvData1;
  UINT8   OEMEvData2;
  UINT8   OEMEvData3;
} IPMI_PLATFORM_EVENT_MESSAGE_DATA_REQUEST;

#pragma pack()
#endif
#endif  // AMI_IPMI_PORTING

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
