//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
/** @file
  The shared head file for RedFish IScsi Protocol.

Copyright (c) 2004 - 2016, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _REDFISH_ISCSI_CONFIG_H_
#define _REDFISH_ISCSI_CONFIG_H_

#define REDFISH_ISCSI_NAME_MAX_SIZE       224
#define REDFISH_ISCSI_MAX_MAC_STRING_LEN            96
#define REDFISH_ISCSI_CHAP_SECRET_STORAGE  17
#define REDFISH_ISCSI_CHAP_NAME_STORAGE    127
#define REDFISH_IP_MODE_IP4               0
#define REDFISH_IP_MODE_IP6               1
#define REDFISH_IP_MODE_AUTOCONFIG        2

#define REDFISH_ISCSI_AUTH_TYPE_NONE      0
#define REDFISH_ISCSI_AUTH_TYPE_CHAP      1
// ISCSI CHAP TYPES
#define REDFISH_ISCSI_CHAP_UNI            0
#define REDFISH_ISCSI_CHAP_MUTUAL         1

typedef struct _EFI_REDFISH_ISCSI_CONFIG_PROTOCOL EFI_REDFISH_ISCSI_CONFIG_PROTOCOL;
typedef struct _REDFISH_ISCSI_ATTEMPT_CONFIG_DATA REDFISH_ISCSI_ATTEMPT_CONFIG_DATA;  
typedef struct _REDFISH_ISCSI_CHAP_AUTH_CONFIG_NVDATA {
  UINT8 CHAPType;
  CHAR8 CHAPName[REDFISH_ISCSI_CHAP_NAME_STORAGE];
  CHAR8 CHAPSecret[REDFISH_ISCSI_CHAP_SECRET_STORAGE];
  CHAR8 ReverseCHAPName[REDFISH_ISCSI_CHAP_NAME_STORAGE];
  CHAR8 ReverseCHAPSecret[REDFISH_ISCSI_CHAP_SECRET_STORAGE];
} REDFISH_ISCSI_CHAP_AUTH_CONFIG_NVDATA;

struct _REDFISH_ISCSI_ATTEMPT_CONFIG_DATA {
  CHAR8                            IScsiInitiatorName[REDFISH_ISCSI_NAME_MAX_SIZE];  
  CHAR8                            TargetName[REDFISH_ISCSI_NAME_MAX_SIZE];
  
  CHAR8                            MacString[REDFISH_ISCSI_MAX_MAC_STRING_LEN];
  UINT8                            IpMode;
  
  EFI_IP_ADDRESS                   LocalIp;
  EFI_IP_ADDRESS                   Gateway;
  EFI_IPv4_ADDRESS                 SubnetMask;
  BOOLEAN                          InitiatorInfoFromDhcp;  
  BOOLEAN                          TargetInfoFromDhcp;

  EFI_IP_ADDRESS                   TargetIp;
  UINT16                           TargetPort;
  UINT8                            BootLun[8];
  
  BOOLEAN                          PrimaryVLANEnable;
  UINT16                           PrimaryVLANId;
  
  EFI_IP_ADDRESS                   PrimaryDns;
  
  CHAR8                            SecondaryTargetName[REDFISH_ISCSI_NAME_MAX_SIZE];
  
  EFI_IP_ADDRESS                   SecondaryTargetIp;
  UINT16                           SecondaryTargetPort;
  UINT8                            SecondaryBootLun[8];
    
  BOOLEAN                          SecondaryVLANEnable;
  UINT16                           SecondaryVLANId;
  
  EFI_IP_ADDRESS                   SecondaryDns;
  
  BOOLEAN                          IPMaskDNSViaDHCP;
  BOOLEAN                          RouterAdvertisementEnabled;
  
  UINT8                            AuthenticationType;
  union {
      REDFISH_ISCSI_CHAP_AUTH_CONFIG_NVDATA  CHAP;
  } AuthConfigData;
  UINT8                            AttemptConfigIndex;
};

/**
  Set the IScsi configuration retrieved from redfish server to NVRAM 



  @param[in]   This               Pointer to the EFI_IP4_CONFIG2_PROTOCOL instance.       
  @param[in]   DataSize           Size of the buffer pointed to by Data in bytes.
  @param[in]   Data               The data buffer to set. The type ofthe data buffer is associated 
                                  with the DataType. 

  @retval EFI_SUCCESS             The specified configuration data for the EFI IPv4 network stack is set 
                                  successfully.
  @retval EFI_INVALID_PARAMETER   One or more of the following are TRUE:
                                  This is NULL.
                                  Data is NULL.
                                  One or more fields in Data do not match the requirement of the data type 
                                  indicated by DataType.
  @retval EFI_WRITE_PROTECTED     The specified configuration data is read-only or the specified configuration 
                                  data can not be set under the current policy.
  @retval EFI_ACCESS_DENIED       Another set operation on the specified configuration data is already in process.
  @retval EFI_NOT_READY           An asynchronous process is invoked to set the specified configuration data and 
                                  the process is not finished yet.
  @retval EFI_BAD_BUFFER_SIZE     The DataSize does not match the size of the type indicated by DataType.                                 
  @retval EFI_UNSUPPORTED         This DataType is not supported.
  @retval EFI_OUT_OF_RESOURCES    Required system resources could not be allocated.
  @retval EFI_DEVICE_ERROR        An unexpected system error or network error occurred.
**/
typedef 
EFI_STATUS
(EFIAPI *EFI_REDFISH_ISCSI_CONFIG_SET_DATA) (
  IN EFI_REDFISH_ISCSI_CONFIG_PROTOCOL   *This,
  IN REDFISH_ISCSI_ATTEMPT_CONFIG_DATA    *IScsiAttemptData,
  IN UINTN                                DataSize
  );

/**
  Get the IScsi configuration data retrieved from NVRAM 

  @param[in]   AttemptIndex   Index of the attempt on the specified MAC
  @param[in]   CHAR8          MAC address for which data should be retrieved
  @param[out]  REDFISH_ISCSI_ATTEMPT_CONFIG_DATA               The attempt data associated with the given MAC and AttemptIndex

  @retval EFI_SUCCESS             The specified configuration data is retrieved successfully.
  @retval EFI_NOT_FOUND           The specified configuration data is not set.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_REDFISH_ISCSI_CONFIG_GET_DATA)(
        IN UINT8                                AttemptIndex,
        IN CHAR8                                *MacAddr,
        OUT REDFISH_ISCSI_ATTEMPT_CONFIG_DATA   *IScsiAttemptData
);

///
/// The EFI_IP4_CONFIG2_PROTOCOL is designed to be the central repository for the common 
/// configurations and the administrator configurable settings for the EFI IPv4 network stack.
/// An EFI IPv4 Configuration II Protocol instance will be installed on each communication device that 
/// the EFI IPv4 network stack runs on.
///
struct _EFI_REDFISH_ISCSI_CONFIG_PROTOCOL {
    //
    // Version of Protocol
    //
    UINTN                                       Version;
    
    //
    // Set the IScsi configuration retrieved from redfish server to NVRAM 
    //
    EFI_REDFISH_ISCSI_CONFIG_SET_DATA           SetData;
    
    EFI_REDFISH_ISCSI_CONFIG_GET_DATA           GetData;
};

#endif
