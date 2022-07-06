//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
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

  ServerMgmtSetupVariable.h
    
Abstract:

  Server Management Setup Variable definitions.

--------------------------------------------------------------------*/

#ifndef _SERVER_MGMT_SETUP_VARIABLE_H
#define _SERVER_MGMT_SETUP_VARIABLE_H

#define ERROR_LOGGING_SETUP_KEY_ID 	0xF0

//#if IPMI_SUPPORT
#pragma pack(1)
typedef struct {
  UINT8         PropagatePerr;
  UINT8         PropagateSerr;
  UINT8         SvrMngmntResumeAcPowerLoss;//DO NOT MODIFY
  UINT8         SvrMngmntSelLogClear;//DO NOT MODIFY
  UINT8         SvrMngmntFrb2Enable;//DO NOT MODIFY
  UINT8         OSBootWDTimer;//DO NOT MODIFY
  UINT16        OSBootWDTimerTimeout;//DO NOT MODIFY
  UINT8         OSBootWDTimerPolicy;//DO NOT MODIFY
  UINT8         SvrMngmntAcpiIpmi; // DO NOT MODIFY
  UINT8         SerialOverLanEnable;      //DO NOT MODIFY
  UINT8         SvrMngmntConsRedirSerialEnable;//DO NOT MODIFY
  UINT8         SvrMngmntConsRedirSerial1ConnectionType;//DO NOT MODIFY
  UINT8         SvrMngmntConsRedirSerial1FlowCtrl;//DO NOT MODIFY
  UINT8         SvrMngmntConsRedirSerial1BitRate;//DO NOT MODIFY
  UINT8         SvrMngmntConsRedirSerial2ConnectionType;//DO NOT MODIFY
  UINT8         SvrMngmntConsRedirSerial2FlowCtrl;//DO NOT MODIFY
  UINT8         SvrMngmntConsRedirSerial2BitRate;//DO NOT MODIFY

  /*
  #define SERVER_MGMT_CONFIGURATION_DATA_DEFINITION
  #include <ServerMgmtSetupDefinitions.h>
  #undef SERVER_MGMT_CONFIGURATION_DATA_DEFINITION
  */
} SERVER_MGMT_CONFIGURATION_DATA;
#pragma pack()
//#endif 

#define SERVER_MGMT_CLASS_ID  		20
#define SERVER_MGMT_SUBCLASS_ID		0x00


#define  TIME_3MIN    180	//Time in seconds
#define  TIME_4MIN    240
#define  TIME_5MIN    300
#define  TIME_6MIN    360
#define  TIME_10MIN   600
#define  TIME_15MIN   900
#define  TIME_20MIN  1200

#define VAR_EQ_ADMIN_NAME               0x0041  // A
#define VAR_EQ_ADMIN_DECIMAL_NAME       L"65"
#define VAR_EQ_USER_NAME                0x0055  // U
#define VAR_EQ_USER_DECIMAL_NAME        L"85"
#define VAR_EQ_CONFIG_MODE_NAME         0x0043  // C
#define VAR_EQ_CONFIG_MODE_DECIMAL_NAME L"67"
#define VAR_EQ_HT_MODE_NAME             0x0048  // H
#define VAR_EQ_HT_MODE_DECIMAL_NAME     L"72"
#define VAR_EQ_RAID_MODE_NAME           0x0052  // R
#define VAR_EQ_RAID_MODE_DECIMAL_NAME   L"82"

//
// Error Logging Specific Setup Variable Default Values 
//
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_SmbiosLogging		0x01
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_SmbiosEventLogFull	0x00
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_SmbiosErase		0x00
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_LogEfiStatusCodes	0x01
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_METW			0x60
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_MECI			0x01
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_ConvertEfiToLegacy	0x00
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_SelLogging		0x00
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_SelErase		0x00
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_SelFull  		0x00
#define ERROR_LOGGING_SETUP_DATA_DEFAULT_LogSelStatusCodes	0x01

#ifndef VFRCOMPILE
extern EFI_GUID gEfiServerMgmtSetupVariableGuid;
#endif
// AptioV Server Override: Changed variable name to IntelSetup
#define PLATFORM_SETUP_NVDATA_NVNAME  L"IntelSetup"

#endif // #ifndef _EL_SETUP_VARIABLE_H
