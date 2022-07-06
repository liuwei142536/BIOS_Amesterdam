/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  2010 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:

  AlertStandardFormatPei.h

@brief:

  Include file for ASF Driver
Revision History

**/
#ifndef _ALERT_STANDARD_FORMAT_PEI_H
#define _ALERT_STANDARD_FORMAT_PEI_H

#include "AmtLibPei.h"
#include "MkhiMsgs.h"
#include "AlertStandardFormatCommon.h"
#include "AmtStatusCode.h"

///
/// ASF Over HECI
///
#pragma pack(1)
typedef struct {
  UINT8 SubCommand;
  UINT8 Version;
  UINT8 EventSensorType;
  UINT8 EventType;
  UINT8 EventOffset;
  UINT8 EventSourceType;
  UINT8 EventSeverity;
  UINT8 SensorDevice;
  UINT8 SensorNumber;
  UINT8 Entity;
  UINT8 EntityInstance;
  UINT8 Data0;
  UINT8 Data1;
} EFI_ASF_MESSAGE;
#pragma pack()

typedef struct _HECI_ASF_PUSH_PROGRESS_CODE {
  UINT8           Command;
  UINT8           ByteCount;
  EFI_ASF_MESSAGE AsfMessage;
  UINT8           EventData[3];
  UINT8           Reserved[2];
} HECI_ASF_PUSH_PROGRESS_CODE;

#define HECI_ASF_PUSH_PROGRESS_CODE_LENGTH  0x12

typedef enum _HASFM_COMMAND_CODE
{
  ASF_MESSAGING_CMD             = 0x04,
  ASF_PUSH_PROGESS_CODE_SUBCMD  = 0x12,
  ASF_MENAGEMENT_CONTROL        = 0x02,
  ASF_WDT_START_SUBCMD          = 0x13,
  ASF_WDT_STOP_SUBCMD           = 0x14,
  ASF_CONFIGURATION_CMD         = 0x03,
  ASF_CLEAR_BOOT_OPTION_SUBCMD  = 0x15,
  ASF_RETURN_BOOT_OPTION_SUBCMD = 0x16,
  ASF_NO_BOOT_OPTION_SUBCMD     = 0x17
} HASFM_COMMAND_CODE;

typedef struct {
  EFI_FRAMEWORK_MESSAGE_TYPE  MessageType;
  EFI_ASF_MESSAGE             Message;
} EFI_ASF_FRAMEWORK_MESSAGE;

typedef struct {
  EFI_FRAMEWORK_MESSAGE_TYPE  MessageType;
  EFI_STATUS_CODE_VALUE       StatusCodeValue;
} EFI_ASF_DATA_HUB_MAP;

///
/// Prototypes
///
EFI_STATUS
SendAsfMessage (
  IN  EFI_PEI_SERVICES                     **PeiServices,
  IN  EFI_ASF_MESSAGE                      *AsfMessage
  )
/**

  @brief
    Send ASF Message.

  @param[in] PeiServices        General purpose services available to every PEIM.
  @param[in] AsfMessage         Pointer to ASF message

  @retval EFI_SUCCESS           Boot options copied
  @retval EFI_INVALID_PARAMETER Invalid pointer
  @retval EFI_NOT_READY         No controller
  @retval EFI_DEVICE_ERROR      The function should not be completed due to a device error.

**/
;

EFI_STATUS
SendPostPacket (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_FRAMEWORK_MESSAGE_TYPE      MessageType
  )
/**

  @brief

  Sends a POST packet across ASF

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] MessageType          POST Status Code

  @retval EFI_SUCCESS             The function completed successfully

**/
;

EFI_STATUS
EFIAPI
PeiAmtReportStatusCode (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  PEI_AMT_STATUS_CODE_PPI     * This,
  IN  EFI_STATUS_CODE_TYPE        Type,
  IN  EFI_STATUS_CODE_VALUE       Value,
  IN  UINT32                      Instance,
  IN  EFI_GUID                    * CallerId OPTIONAL,
  IN  EFI_STATUS_CODE_DATA        * Data OPTIONAL
  )
/**
  @brief

  Provides an interface that a software module can call to report an ASF PEI status code.

  @param[in] PeiServices          PeiServices pointer.
  @param[in] This                 This interface.
  @param[in] Type                 Indicates the type of status code being reported.
  @param[in] Value                Describes the current status of a hardware or software entity.
                                  This included information about the class and subclass that is
                                  used to classify the entity as well as an operation.
  @param[in] Instance             The enumeration of a hardware or software entity within
                                  the system. Valid instance numbers start with 1.
  @param[in] CallerId             This optional parameter may be used to identify the caller.
                                  This parameter allows the status code driver to apply different
                                  rules to different callers.
  @param[in] Data                 This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_DEVICE_ERROR        The function should not be completed due to a device error.

**/
;

#endif
