//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/** @file
  All the definitions required for Sensor commands Header File.

  Copyright (c) 1999 - 2014, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.
**/

#ifndef _IPMINETFNSENSORDEFINITIONS_H_
#define _IPMINETFNSENSORDEFINITIONS_H_

//
// Net function definition for Sensor command
//
#define EFI_SM_NETFN_SENSOR 0x04

//
// All Sensor commands and their structure definitions to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Send Platform Event Message command     
----------------------------------------------------------------------------------------*/
#define EFI_SENSOR_PLATFORM_EVENT_MESSAGE   0x02

#pragma pack(1)
typedef struct {
  UINT8   GeneratorId;
  UINT8   EvMRevision;
  UINT8   SensorType;
  UINT8   SensorNumber;
  UINT8   EventDirType;
  UINT8   OEMEvData1;
  UINT8   OEMEvData2;
  UINT8   OEMEvData3;
} EFI_PLATFORM_EVENT_MESSAGE_DATA;
#pragma pack()
#endif
