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
  IpmiDefinitions.h
  
Abstract:
  To make the definitions of all BMC command readable, the definitions are devided into 
  different files depending upon their netfunctions.  The filename and their net functions are 
  as follows.
 
  IpmiNetFnChassisDefinitions.h                ---  All command definitions for CHASSIS Net Functions
  IpmiNetFnBridgeDefinitions.h                 ---  All command definitions for BRIDGE Net Functions
  IpmiNetFnSensorDefinitions.h                 ---  All command definitions for SENSOR Net Functions
  IpmiNetFnAppDefinitions.h                    ---  All command definitions for APPLICATION Net Functions
  IpmiNetFnFirmwareDefinitions.h               ---  All command definitions for FIRMWARE Net Functions
  IpmiNetFnStorageDefinitions.h                ---  All command definitions for STORAGE Net Functions
  IpmiNetFnTransportDefinitions.h              ---  All command definitions for TRANSPORT Net Functions
  IpmiNetFnGroupExtDefinitions.h               ---  All command definitions for GROUP EXTENSION Net Functions
  IpmiNetFnIntelGeneralDefinitions.h           ---  All command definitions for INTEL GENERIC Net Functions
  IpmiNetFnIntelPlatformSpecificDefinitions.h  ---  All command definitions for INTEL PLATFORM SPECIFIC Net Functions
  IpmiNetFnIntelTPTDefinitions.h               ---  All command definitions for TCPIP PASS THROUGH Net Functions
     
--*/

#ifndef _IPMIDEFINITIONS_H_
#define _IPMIDEFINITIONS_H_

#include "IpmiNetFnChassisDefinitions.h"
#include "IpmiNetFnBridgeDefinitions.h"
#include "IpmiNetFnSensorDefinitions.h"
#include "IpmiNetFnAppDefinitions.h"
#include "IpmiNetFnFirmwareDefinitions.h"
#include "IpmiNetFnStorageDefinitions.h"
#include "IpmiNetFnTransportDefinitions.h"
#include "IpmiNetFnGroupExtDefinitions.h"
#include "IpmiNetFnIntelGeneralDefinitions.h"
#include "IpmiNetFnIntelPlatformSpecificDefinitions.h"
#include "IpmiNetFnIntelTPTDefinitions.h"
/*
//
//  IPMI HOB
//
typedef struct {
  UINT16  IoBasePort;
} IPMI_HOB_DATA;
*/
//
// Prototype definitions for IPMI Library in Dxe Phase
//
EFI_STATUS
EfiInitializeIpmiBase (
  VOID
  )
/*++

Routine Description:

  Initialize the global varible with the pointer of IpmiTransport Protocol

Arguments:

  None

Returns:

  EFI_SUCCESS - Always return success

--*/
;

EFI_STATUS
EfiSendCommandToBMC (
  IN UINT8     NetFunction,
  IN UINT8     Command,
  IN UINT8     *CommandData,
  IN UINT8     CommandDataSize,
  OUT UINT8    *ResponseData,
  OUT UINT8    *ResponseDataSize
  )
/*++

Routine Description:

  Routine to send commands to BMC

Arguments:

  NetFunction       - Net function of the command
  Command           - IPMI Command
  CommandData       - Command Data
  CommandDataSize   - Size of CommandData
  ResponseData      - Response Data
  ResponseDataSize  - Response Data Size

Returns:

  EFI_NOT_AVAILABLE_YET - IpmiTransport Protocol is not installed yet

--*/
;

#endif
