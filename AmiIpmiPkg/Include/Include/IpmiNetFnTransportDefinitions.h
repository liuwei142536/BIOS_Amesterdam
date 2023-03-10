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

  This file contains all NetFn Transport commands, including:
    IPM LAN Commands (Chapter 23)
    IPMI Serial/Modem Commands (Chapter 25)
    SOL Commands (Chapter 26)
    Command Forwarding Commands (Chapter 35b)

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

#ifndef _IPMINETFNTRANSPORTDEFINITIONS_H_
#define _IPMINETFNTRANSPORTDEFINITIONS_H_

//
// LUN definition
//
#ifndef BMC_LUN
    #define BMC_LUN     0x00
#endif

// AMI_IPMI_PORTING_END

#ifndef _IPMI_NET_FN_TRANSPORT_H_
#define _IPMI_NET_FN_TRANSPORT_H_

//
// Net function definition for Transport command
//
#define IPMI_NETFN_TRANSPORT  0x0C

// AMI_IPMI_PORTING_START

typedef enum {
  EfiSmReserved,
  EfiSmIpmb,
  EfiSmIcmb1_0,
  EfiSmIcmb0_9,
  EfiSm802_3_Lan,
  EfiSmRs_232,
  EfiSmOtherLan,
  EfiSmPciSmBus,
  EfiSmSmBus1_0,
  EfiSmSmBus2_0,
  EfiSmUsb1_x,
  EfiSmUsb2_x,
  EfiSmBmc
} EFI_SM_CHANNEL_MEDIA_TYPE;

typedef enum {
  EfiSmTcp,
  EfiSmUdp,
  EfiSmIcmp,
  EfiSmIpmi
} EFI_SM_PROTOCOL_TYPE;

typedef struct {
  BOOLEAN IpAddressType;
  UINT16  IpPort;
  UINT8   IpAddress[16];
} EFI_SM_IP_ADDRESS;

typedef struct {
  UINT8 SlaveAddress;
  UINT8 LunAddress;
  UINT8 NetFunction;
  UINT8 ChannelAddress;
} EFI_SM_IPMI_ADDRESS;

typedef struct {
  UINT8 SerialPortNumber;
} EFI_SM_SERIAL_ADDRESS;

typedef union {
  EFI_SM_IP_ADDRESS     IpAddress;
  EFI_SM_IPMI_ADDRESS   BmcAddress;
  EFI_SM_SERIAL_ADDRESS SerialAddress;
} EFI_SM_COM_ADDRESS_TYPE;

typedef struct {
  EFI_SM_CHANNEL_MEDIA_TYPE ChannelType;
  EFI_SM_COM_ADDRESS_TYPE   Address;
} EFI_SM_COM_ADDRESS;

// AMI_IPMI_PORTING_END

//
//  Below is Definitions for IPM LAN Commands (Chapter 23)
//

//
//  Definitions for Set Lan Configuration Parameters command
//
#define IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS 0x01

//
//  Constants and Structure definitions for "Set Lan Configuration Parameters" command to follow here
//
#pragma pack(1)  // AMI_IPMI_PORTING
//
// LAN Management Structure
//
typedef enum {
  IpmiLanReserved1,
  IpmiLanReserved2,
  IpmiLanAuthType,
  IpmiLanIpAddress,
  IpmiLanIpAddressSource,
  IpmiLanMacAddress,
  IpmiLanSubnetMask,
  IpmiLanIpv4HeaderParam,
  IpmiLanPrimaryRcmpPort,
  IpmiLanSecondaryRcmpPort,
  IpmiLanBmcGeneratedArpCtrl,
  IpmiLanArpInterval,
  IpmiLanDefaultGateway,
  IpmiLanDefaultGatewayMac,
  IpmiLanBackupGateway,
  IpmiLanBackupGatewayMac,
  IpmiLanCommunityString,
  IpmiLanReserved3,
  IpmiLanDestinationType,
  IpmiLanDestinationAddress,
// AMI_IPMI_PORTING_START
  // Other members below are maintained for Ipv4 and IPv6 configuration.
  EfiIpv4OrIpv6Support = 0x32,
  EfiIpv4OrIpv6AddressEnable,
  EfiIpv6HdrStatTrafficClass,
  EfiIpv6HdrStatHopLimit,
  EfiIpv6HdrFlowLabel,
  EfiIpv6Status,
  EfiIpv6StaticAddress,
  EfiIpv6DhcpStaticDuidLen,
  EfiIpv6DhcpStaticDuid,
  EfiIpv6DhcpAddress,
  EfiIpv6DhcpDynamicDuidLen,
  EfiIpv6DhcpDynamicDuid,
  EfiIpv6RouterConfig = 0x40,
  EfiIpv6StaticRouter1IpAddr,
  EfiIpv6DyanamicRouterIpAddr = 0x4a
// AMI_IPMI_PORTING_END
} IPMI_LAN_OPTION_TYPE;

//
// IP Address Source
//
typedef enum {
  IpmiUnspecified,
  IpmiStaticAddress,  // AMI_IPMI_PORTING
  IpmiDynamicAddressBmcDhcp,
  IpmiDynamicAddressBiosDhcp,
  IpmiDynamicAddressBmcNonDhcp
} IPMI_IP_ADDRESS_SRC;

//
// Destination Type
//
typedef enum {
  IpmiPetTrapDestination,
  IpmiDirectedEventDestination,
  IpmiReserved1,
  IpmiReserved2,
  IpmiReserved3,
  IpmiReserved4,
  IpmiReserved5,
  IpmiOem1,
  IpmiOem2
} IPMI_LAN_DEST_TYPE_DEST_TYPE;

typedef struct {
  UINT8 NoAuth : 1;
  UINT8 MD2Auth : 1;
  UINT8 MD5Auth : 1;
  UINT8 Reserved1 : 1;
  UINT8 StraightPswd : 1;
  UINT8 OemType : 1;
  UINT8 Reserved2 : 2;
} IPMI_LAN_AUTH_TYPE;

typedef struct {
  UINT8 IpAddress[4];
} IPMI_LAN_IP_ADDRESS;

typedef struct {
  UINT8 AddressSrc : 4;
  UINT8 Reserved : 4;
} IPMI_LAN_IP_ADDRESS_SRC;

typedef struct {
  UINT8 MacAddress[6];
} IPMI_LAN_MAC_ADDRESS;

typedef struct {
  UINT8 IpAddress[4];
} IPMI_LAN_SUBNET_MASK;

typedef struct {
  UINT8 TimeToLive;
  UINT8 IpFlag : 3;
  UINT8 Reserved1 : 5;
  UINT8 Precedence : 3;
  UINT8 Reserved2 : 1;
  UINT8 ServiceType : 4;
} IPMI_LAN_IPV4_HDR_PARAM;

typedef struct {
  UINT8 RcmpPortMsb;
  UINT8 RcmpPortLsb;
} IPMI_LAN_RCMP_PORT;

typedef struct {
  UINT8 EnableBmcArpResponse : 1;
  UINT8 EnableBmcGratuitousArp : 1;
  UINT8 Reserved : 6;
} IPMI_LAN_BMC_GENERATED_ARP_CONTROL;

typedef struct {
  UINT8 ArpInterval;
} IPMI_LAN_ARP_INTERVAL;

typedef struct {
  UINT8 Data[18];
} IPMI_LAN_COMMUNITY_STRING;

typedef struct {
  UINT8 DestinationSelector : 4;
  UINT8 Reserved2 : 4;
  UINT8 DestinationType : 3;
  UINT8 Reserved1 : 4;
  UINT8 AlertAcknowledged : 1;
} IPMI_LAN_DEST_TYPE;

typedef struct {
  UINT8               DestinationSelector : 4;
  UINT8               Reserved1 : 4;
  UINT8               AlertingIpAddressSelector : 4;
  UINT8               AddressFormat : 4;
  UINT8               UseDefaultGateway : 1;
  UINT8               Reserved2 : 7;
  IPMI_LAN_IP_ADDRESS  AlertingIpAddress;
  IPMI_LAN_MAC_ADDRESS AlertingMacAddress;
} IPMI_LAN_DEST_ADDRESS;

typedef union {
  IPMI_LAN_AUTH_TYPE                 IpmiLanAuthType;
  IPMI_LAN_IP_ADDRESS                IpmiLanIpAddress;
  IPMI_LAN_IP_ADDRESS_SRC            IpmiLanIpAddressSrc;
  IPMI_LAN_MAC_ADDRESS               IpmiLanMacAddress;
  IPMI_LAN_SUBNET_MASK               IpmiLanSubnetMask;
  IPMI_LAN_IPV4_HDR_PARAM            IpmiLanIpv4HdrParam;
  IPMI_LAN_RCMP_PORT                 IpmiLanPrimaryRcmpPort;
  IPMI_LAN_BMC_GENERATED_ARP_CONTROL IpmiLanArpControl;
  IPMI_LAN_ARP_INTERVAL              IpmiLanArpInterval;
  IPMI_LAN_COMMUNITY_STRING          IpmiLanCommunityString;
  IPMI_LAN_DEST_TYPE                 IpmiLanDestType;
  IPMI_LAN_DEST_ADDRESS              IpmiLanDestAddress;
} IPMI_LAN_OPTIONS;

#pragma pack()  // AMI_IPMI_PORTING
//
//  Definitions for Get Lan Configuration Parameters command
//
#define IPMI_TRANSPORT_GET_LAN_CONFIG_PARAMETERS 0x02

//
//  Constants and Structure definitions for "Get Lan Configuration Parameters" command to follow here
//

// AMI_IPMI_PORTING_START

#define EFI_BMC_IPV6_DIS     0x00
#define EFI_BMC_IPV4_DIS     0x01
#define EFI_BMC_IPV_4_6_SUPP 0x02

#define EFI_DHCP_SLAAC       0x01
#define EFI_DHCP_DHCP6       0x02

typedef struct {
  UINT8 Param;
  UINT8 IpAddress[4];
} EFI_GET_LAN_IP_ADDRESS;

typedef struct {
  UINT8 Param;
  UINT8 AddressSrc : 4;
  UINT8 Reserved : 4;
} EFI_GET_LAN_IP_ADDRESS_SRC;

typedef struct {
  UINT8 Param;
  UINT8 MacAddress[6];
} EFI_GET_LAN_MAC_ADDRESS;

typedef struct {
  UINT8 Param;
  UINT8 IpAddress[4];
} EFI_GET_LAN_SUBNET_MASK;

typedef struct {
  UINT8  ChannelNumber : 4;
  UINT8  Reserved : 3;
  UINT8  GetParam : 1;
}EFI_LAN_CHANNEL_NUM;

typedef struct {
  EFI_LAN_CHANNEL_NUM LanChannel;
  UINT8                                     ParameterSelector;
  UINT8                                     SetSelector;
  UINT8                                     BlockSelector;
}EFI_IPMI_GET_LAN_CONFIG_PRAM;

typedef struct {
  UINT8    SetInProgress:2;                   /// Set-In Progress Bits
  UINT8    Reserved:6;                        /// Reserved
} EFI_IPMI_SET_IN_PROGRESS;

typedef struct {
  UINT8 ParamRev;                             /// Parameter Revision
  EFI_IPMI_SET_IN_PROGRESS Param0;            /// Set In Progress
} EFI_GET_LAN_CONFIG_PARAM0;


typedef struct {
  UINT8 Addr[16];                             /// IPv6 address value
} IPMI_IPV6_ADDRESS;

typedef struct {
  UINT8 ParamRev;                             /// Parameter Revision
  UINT8 Data1;                                /// Enable Details
} EFI_GET_LAN_CONFIG_IP_4_6_SUPPORT_ENABLE;


typedef struct {
  EFI_LAN_CHANNEL_NUM    LanChannel;         /// Channel to be updated
  UINT8                  Param;              /// Parameter selector
  UINT8                  Data1;              /// Enable Details
} EFI_SET_LAN_CONFIG_IP_4_6_SUPPORT_ENABLE;

typedef struct {
  UINT8 Dhcp6Supp:1;                         /// DHCP uses Dhcp6 method for address
  UINT8 SlaacSupp:1;                         /// DHCP uses SLAAC method for address
  UINT8 Reserved:6;                          /// Reserved data
} EFI_IPMI_IPV6_ADDR_SUPP;

typedef struct {
  UINT8                   ParamRev;           /// Parameter Revision.
  UINT8                   MaxStaticAddr;      /// Static Address supported in case of non zero value.
  UINT8                   MaxDynamicAddr;     /// Dynamic Address supported in case of non zero value.
  EFI_IPMI_IPV6_ADDR_SUPP AddressingSupp;     /// IPV6 addressing method used.
} EFI_IPMI_IPV6_STATUS;

typedef struct {
  UINT8 Type:4;            /// Address type
  UINT8 Reserved:3;        /// Reserved data
  UINT8 EnableStat:1;      /// Enable or disable static support
} EFI_IPMI_IPV6_SOURCE;

typedef struct {
  UINT8                        SetSelector;            /// Set selector.
  EFI_IPMI_IPV6_SOURCE         AddressSource;          /// Address source
  IPMI_IPV6_ADDRESS            Ipv6Address;            /// Static IP address
  UINT8                        AddressPrefixLen;       /// IP address prefix length
  UINT8                        AddressStatus;          /// IP address status
} EFI_IPMI_IPV6_ADDRESS;

typedef struct {
  UINT8                   ParamRev;            /// Parameter Revision
  EFI_IPMI_IPV6_ADDRESS    Ip6AddressParam;    /// Ipv6 Address
} EFI_IPMI_GET_IPV6_ADDRESS;

typedef struct {
  EFI_LAN_CHANNEL_NUM    LanChannel;         /// Channel to be updated
  UINT8                  Param;              /// Parameter selector
  EFI_IPMI_IPV6_ADDRESS  Ip6AddressParam;    /// Parameter value
} EFI_IPMI_SET_IPV6_ADDRESS;

typedef struct {
    EFI_LAN_CHANNEL_NUM            LanChannel;         /// Channel to be updated
    UINT8                          Param;              /// Parameter selector
    UINT8                          Data;               /// Ipv6 Address
} EFI_IPMI_SET_IPV6_ROUTER_CONFIG;

typedef struct {
  IPMI_IPV6_ADDRESS            Ipv6Address;            /// Static IP address
} EFI_IPMI_IPV6_ROUTER1_IP_ADDRESS;

typedef struct {
  UINT8                               ParamRev;           /// Parameter Revision
  EFI_IPMI_IPV6_ROUTER1_IP_ADDRESS    Ip6AddressParam;    /// Ipv6 Address
} EFI_IPMI_GET_IPV6_STATIC_ROUTER1_IP_ADDRESS;

typedef struct {
  EFI_LAN_CHANNEL_NUM               LanChannel;         /// Channel to be updated
  UINT8                             Param;              /// Parameter selector
  EFI_IPMI_IPV6_ROUTER1_IP_ADDRESS  Ip6AddressParam;    /// Parameter value
} EFI_IPMI_SET_IPV6_ROUTER1_IP_ADDRESS;

typedef struct {
  UINT8                        SetSelector;            /// Set selector.
  IPMI_IPV6_ADDRESS            Ipv6Address;            /// Static IP address
} EFI_IPMI_IPV6_DYNAMIC_ROUTER1_IP_ADDRESS;

typedef struct {
  UINT8                                    ParamRev;            /// Parameter Revision
  EFI_IPMI_IPV6_DYNAMIC_ROUTER1_IP_ADDRESS  Ip6AddressParam;    /// Ipv6 Address
} EFI_IPMI_GET_IPV6_DYNAMIC_ROUTER1_IP_ADDRESS;

// AMI_IPMI_PORTING_END

//
//  Definitions for Suspend BMC ARPs command
//
#define IPMI_TRANSPORT_SUSPEND_BMC_ARPS  0x03

//
//  Constants and Structure definitions for "Suspend BMC ARPs" command to follow here
//

//
//  Definitions for Get IP-UDP-RMCP Statistics command
//
#define IPMI_TRANSPORT_GET_PACKET_STATISTICS 0x04

//
//  Constants and Structure definitions for "Get IP-UDP-RMCP Statistics" command to follow here
//

//
//  Below is Definitions for IPMI Serial/Modem Commands (Chapter 25)
//

//
//  Definitions for Set Serial/Modem Configuration command
//
#define IPMI_TRANSPORT_SET_SERIAL_CONFIGURATION  0x10

//
//  Constants and Structure definitions for "Set Serial/Modem Configuration" command to follow here
//
#pragma pack(1)  // AMI_IPMI_PORTING
//
// EMP OPTION DATA
//
typedef struct {
  UINT8 NoAuthentication : 1;
  UINT8 MD2Authentication : 1;
  UINT8 MD5Authentication : 1;
  UINT8 Reserved1 : 1;
  UINT8 StraightPassword : 1;
  UINT8 OemProprietary : 1;
  UINT8 Reservd2 : 2;
} IPMI_EMP_AUTH_TYPE;

typedef struct {
  UINT8 EnableBasicMode : 1;
  UINT8 EnablePPPMode : 1;
  UINT8 EnableTerminalMode : 1;
  UINT8 Reserved1 : 2;
  UINT8 SnoopOsPPPNegotiation : 1;
  UINT8 Reserved2 : 1;
  UINT8 DirectConnect : 1;
} IPMI_EMP_CONNECTION_TYPE;

typedef struct {
  UINT8 InactivityTimeout : 4;
  UINT8 Reserved : 4;
} IPMI_EMP_INACTIVITY_TIMEOUT;

typedef struct {
  UINT8 IpmiCallback : 1;
  UINT8 CBCPCallback : 1;
  UINT8 Reserved1 : 6;
  UINT8 CbcpEnableNoCallback : 1;
  UINT8 CbcpEnablePreSpecifiedNumber : 1;
  UINT8 CbcpEnableUserSpecifiedNumber : 1;
  UINT8 CbcpEnableCallbackFromList : 1;
  UINT8 Reserved : 4;
  UINT8 CallbackDestination1;
  UINT8 CallbackDestination2;
  UINT8 CallbackDestination3;
} IPMI_EMP_CHANNEL_CALLBACK_CONTROL;

typedef struct {
  UINT8 CloseSessionOnDCDLoss : 1;
  UINT8 EnableSessionInactivityTimeout : 1;
  UINT8 Reserved : 6;
} IPMI_EMP_SESSION_TERMINATION;

typedef struct {
  UINT8 Reserved1 : 5;
  UINT8 EnableDtrHangup : 1;
  UINT8 FlowControl : 2;
  UINT8 BitRate : 4;
  UINT8 Reserved2 : 4;
  UINT8 SaveSetting : 1;
  UINT8 SetComPort : 1;
  UINT8 Reserved3 : 6;
} IPMI_EMP_MESSAGING_COM_SETTING;

typedef struct {
  UINT8 RingDurationInterval : 6;
  UINT8 Reserved1 : 2;
  UINT8 RingDeadTime : 4;
  UINT8 Reserved : 4;
} IPMI_EMP_MODEM_RING_TIME;

typedef struct {
  UINT8 Reserved;
  UINT8 InitString[48];
} IPMI_EMP_MODEM_INIT_STRING;

typedef struct {
  UINT8 EscapeSequence[5];
} IPMI_EMP_MODEM_ESC_SEQUENCE;

typedef struct {
  UINT8 HangupSequence[8];
} IPMI_EMP_MODEM_HANGUP_SEQUENCE;

typedef struct {
  UINT8 ModelDialCommend[8];
} IPMI_MODEM_DIALUP_COMMAND;

typedef struct {
  UINT8 PageBlackoutInterval;
} IPMI_PAGE_BLACKOUT_INTERVAL;

typedef struct {
  UINT8 CommunityString[18];
} IPMI_EMP_COMMUNITY_STRING;

typedef struct {
  UINT8 Reserved5 : 4;
  UINT8 DialStringSelector : 4;
} IPMI_DIAL_PAGE_DESTINATION;

typedef struct {
  UINT8 TapAccountSelector : 4;
  UINT8 Reserved : 4;
} IPMI_TAP_PAGE_DESTINATION;

typedef struct {
  UINT8 PPPAccountSetSelector;
  UINT8 DialStringSelector;
} IPMI_PPP_ALERT_DESTINATION;

typedef union {
  IPMI_DIAL_PAGE_DESTINATION    DialPageDestination;
  IPMI_TAP_PAGE_DESTINATION     TapPageDestination;
  IPMI_PPP_ALERT_DESTINATION    PppAlertDestination;
} IPMI_DEST_TYPE_SPECIFIC;

typedef struct {
  UINT8 DestinationSelector : 4;
  UINT8 Reserved1 : 4;
  UINT8 DestinationType : 4;
  UINT8 Reserved2 : 3;
  UINT8 AlertAckRequired : 1;
  UINT8 AlertAckTimeoutSeconds;
  UINT8 NumRetriesCall : 3;
  UINT8 Reserved3 : 1;
  UINT8 NumRetryAlert : 3;
  UINT8 Reserved4 : 1;
  IPMI_DEST_TYPE_SPECIFIC DestinationTypeSpecific;
} IPMI_EMP_DESTINATION_INFO;

typedef struct {
  UINT8 DestinationSelector : 4;
  UINT8 Reserved1 : 4;
  UINT8 Parity : 3;
  UINT8 CharacterSize : 1;
  UINT8 StopBit : 1;
  UINT8 DtrHangup : 1;
  UINT8 FlowControl : 2;
  UINT8 BitRate : 4;
  UINT8 Reserved2 : 4;
  UINT8 SaveSetting : 1;
  UINT8 SetComPort : 1;
  UINT8 Reserved3 : 6;
} IPMI_EMP_DESTINATION_COM_SETTING;

typedef struct {
  UINT8 DialStringSelector : 4;
  UINT8 Reserved1 : 4;
  UINT8 Reserved2;
  UINT8 DialString[48];
} IPMI_DESTINATION_DIAL_STRING;

typedef union {
  UINT32  IpAddressLong;
  UINT8   IpAddress[4];
} IPMI_PPP_IP_ADDRESS;

typedef struct {
  UINT8 IpAddressSelector : 4;
  UINT8 Reserved1 : 4;
  IPMI_PPP_IP_ADDRESS PppIpAddress;
} IPMI_DESTINATION_IP_ADDRESS;

typedef struct {
  UINT8 TapSelector;
  UINT8 TapServiceSelector : 4;
  UINT8 TapDialStringSelector : 4;
} IPMI_DESTINATION_TAP_ACCOUNT;

typedef struct {
  UINT8 TapSelector;
  UINT8 PagerIdString[16];
} IPMI_TAP_PAGER_ID_STRING;

typedef union {
  UINT8                              OptionData;
  IPMI_EMP_AUTH_TYPE                 EmpAuthType;
  IPMI_EMP_CONNECTION_TYPE           EmpConnectionType;
  IPMI_EMP_INACTIVITY_TIMEOUT        EmpInactivityTimeout;
  IPMI_EMP_CHANNEL_CALLBACK_CONTROL  EmpCallbackControl;
  IPMI_EMP_SESSION_TERMINATION       EmpSessionTermination;
  IPMI_EMP_MESSAGING_COM_SETTING     EmpMessagingComSetting;
  IPMI_EMP_MODEM_RING_TIME           EmpModemRingTime;
  IPMI_EMP_MODEM_INIT_STRING         EmpModemInitString;
  IPMI_EMP_MODEM_ESC_SEQUENCE        EmpModemEscSequence;
  IPMI_EMP_MODEM_HANGUP_SEQUENCE     EmpModemHangupSequence;
  IPMI_MODEM_DIALUP_COMMAND          EmpModemDialupCommand;
  IPMI_PAGE_BLACKOUT_INTERVAL        EmpPageBlackoutInterval;
  IPMI_EMP_COMMUNITY_STRING          EmpCommunityString;
  IPMI_EMP_DESTINATION_INFO          EmpDestinationInfo;
  IPMI_EMP_DESTINATION_COM_SETTING   EmpDestinationComSetting;
  UINT8                              CallRetryBusySignalInterval;
  IPMI_DESTINATION_DIAL_STRING       DestinationDialString;
  IPMI_DESTINATION_IP_ADDRESS        DestinationIpAddress;
  IPMI_DESTINATION_TAP_ACCOUNT       DestinationTapAccount;
  IPMI_TAP_PAGER_ID_STRING           TapPagerIdString;
} IPMI_EMP_OPTIONS;
#pragma pack()  // AMI_IPMI_PORTING

//
//  Definitions for Get Serial/Modem Configuration command
//
#define IPMI_TRANSPORT_GET_SERIAL_CONFIGURATION  0x11

//
//  Constants and Structure definitions for "Get Serial/Modem Configuration" command to follow here
//

//
//  Definitions for Set Serial/Modem Mux command
//
#define IPMI_TRANSPORT_SET_SERIAL_MUX  0x12

//
//  Constants and Structure definitions for "Set Serial/Modem Mux" command to follow here
//
typedef struct {
  UINT8 ChannelNo : 4;
  UINT8 Reserved1 : 4;
  UINT8 MuxSetting : 4;
  UINT8 Reserved2 : 4;
} IPMI_SET_SERIAL_MODEM_MUX_COMMAND_REQUEST;

typedef struct {
  UINT8 MuxSetToBmc : 1;
  UINT8 CommandStatus : 1;
  UINT8 MessagingSessionActive : 1;
  UINT8 AlertInProgress : 1;
  UINT8 Reserved2 : 2;
  UINT8 MuxToBmcAllowed : 1;
  UINT8 MuxToSystemBlocked : 1;
} IPMI_SET_SERIAL_MODEM_MUX_COMMAND_RESPONSE;

// AMI_IPMI_PORTING_START

typedef enum {
  EfiEmpMuxReserved1,
  ReqMuxToSystem,
  ReqMuxToBmc,
  ForceMuxToSystem,
  ForceMuxToBmc,
  BlockMuxSwitchToSystem,
  AllowMuxSwitchToSystem,
  BlockMuxSwitchToBmc,
  AllowMuxSwitchToBmc
} EFI_MUX_SETTING;

// AMI_IPMI_PORTING_END


//
//  Definitions for Get TAP Response Code command
//
#define IPMI_TRANSPORT_GET_TAP_RESPONSE_CODE 0x13

//
//  Constants and Structure definitions for "Get TAP Response Code" command to follow here
//

//
//  Definitions for Set PPP UDP Proxy Transmit Data command
//
#define IPMI_TRANSPORT_SET_PPP_UDP_PROXY_TXDATA  0x14

//
//  Constants and Structure definitions for "Set PPP UDP Proxy Transmit Data" command to follow here
//

//
//  Definitions for Get PPP UDP Proxy Transmit Data command
//
#define IPMI_TRANSPORT_GET_PPP_UDP_PROXY_TXDATA  0x15

//
//  Constants and Structure definitions for "Get PPP UDP Proxy Transmit Data" command to follow here
//

//
//  Definitions for Send PPP UDP Proxy Packet command
//
#define IPMI_TRANSPORT_SEND_PPP_UDP_PROXY_PACKET 0x16

//
//  Constants and Structure definitions for "Send PPP UDP Proxy Packet" command to follow here
//

//
//  Definitions for Get PPP UDP Proxy Receive Data command
//
#define IPMI_TRANSPORT_GET_PPP_UDP_PROXY_RX  0x17

//
//  Constants and Structure definitions for "Get PPP UDP Proxy Receive Data" command to follow here
//

//
//  Definitions for Serial/Modem connection active command
//
#define IPMI_TRANSPORT_SERIAL_CONNECTION_ACTIVE  0x18

//
//  Constants and Structure definitions for "Serial/Modem connection active" command to follow here
//

//
//  Definitions for Callback command
//
#define IPMI_TRANSPORT_CALLBACK  0x19

//
//  Constants and Structure definitions for "Callback" command to follow here
//

//
//  Definitions for Set user Callback Options command
//
#define IPMI_TRANSPORT_SET_USER_CALLBACK_OPTIONS 0x1A

//
//  Constants and Structure definitions for "Set user Callback Options" command to follow here
//

//
//  Definitions for Get user Callback Options command
//
#define IPMI_TRANSPORT_GET_USER_CALLBACK_OPTIONS 0x1B

//
//  Constants and Structure definitions for "Get user Callback Options" command to follow here
//

//
//  Below is Definitions for SOL Commands (Chapter 26)
//

//
//  Definitions for SOL activating command
//
#define IPMI_TRANSPORT_SOL_ACTIVATING  0x20

//
//  Constants and Structure definitions for "SOL activating" command to follow here
//

//
//  Definitions for Set SOL Configuration Parameters command
//
#define IPMI_TRANSPORT_SET_SOL_CONFIG_PARAM  0x21

//
//  Constants and Structure definitions for "Set SOL Configuration Parameters" command to follow here
//

//
//  Definitions for Get SOL Configuration Parameters command
//
#define IPMI_TRANSPORT_GET_SOL_CONFIG_PARAM  0x22

//
//  Constants and Structure definitions for "Get SOL Configuration Parameters" command to follow here
//
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
