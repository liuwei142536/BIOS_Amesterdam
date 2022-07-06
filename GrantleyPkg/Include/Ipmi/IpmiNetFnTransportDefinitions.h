//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/** @file
  All the definitions required for Transport commands Header File.

  Copyright (c) 1999 - 2014, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.
**/

#ifndef _IPMINETFNTRANSPORTDEFINITIONS_H_
#define _IPMINETFNTRANSPORTDEFINITIONS_H_

//
// Net function definition for Transport command
//
#define EFI_SM_NETFN_TRANSPORT  0x0C
/*
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
*/

/*----------------------------------------------------------------------------------------
    Definitions for Set Lan Configuration Parameters command    
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS 0x01

//
//  Constants and Structure definitions for "Set Lan Configuration Parameters" command to follow here
//
#pragma pack(1)
//
// LAN Management Structure
//
typedef enum {
  EfiIpmiLanReserved1,
  EfiIpmiLanReserved2,
  EfiIpmiLanAuthType,
  EfiIpmiLanIpAddress,
  EfiIpmiLanIpAddressSource,
  EfiIpmiLanMacAddress,
  EfiIpmiLanSubnetMask,
  EfiIpmiLanIpv4HeaderParam,
  EfiIpmiLanPrimaryRcmpPort,
  EfiIpmiLanSecondaryRcmpPort,
  EfiIpmiLanBmcGeneratedArpCtrl,
  EfiIpmiLanArpInterval,
  EfiIpmiLanDefaultGateway,
  EfiIpmiLanDefaultGatewayMac,
  EfiIpmiLanBackupGateway,
  EfiIpmiLanBackupGatewayMac,
  EfiIpmiLanCommunityString,
  EfiIpmiLanReserved3,
  EfiIpmiLanDestinationType,
  EfiIpmiLanDestinationAddress
} EFI_LAN_OPTION_TYPE;

typedef enum {
  Unspecified,
  StaticAddrsss,
  DynamicAddressBmcDhcp,
  DynamicAddressBiosDhcp,
  DynamicAddressBmcNonDhcp
} IPMI_IP_ADDRESS_SRC;

typedef enum {
  PetTrapDestination,
  DirectedEventDestination,
  Reserved1,
  Reserved2,
  Reserved3,
  Reserved4,
  Reserved5,
  Oem1,
  Oem2
} SM_LAN_DEST_TYPE;

typedef struct {
  UINT8 NoAuth : 1;
  UINT8 MD2Auth : 1;
  UINT8 MD5Auth : 1;
  UINT8 Reserved1 : 1;
  UINT8 StraightPswd : 1;
  UINT8 OemType : 1;
  UINT8 Reserved2 : 2;
} EFI_LAN_AUTH_TYPE;

typedef struct {
  UINT8 IpAddress[4];
} EFI_LAN_IP_ADDRESS;

typedef struct {
  UINT8 AddressSrc : 4;
  UINT8 Reserved : 4;
} EFI_LAN_IP_ADDRESS_SRC;

typedef struct {
  UINT8 MacAddress[6];
} EFI_LAN_MAC_ADDRESS;

typedef struct {
  UINT8 IpAddress[4];
} EFI_LAN_SUBNET_MASK;

typedef struct {
  UINT8 TimeToLive;
  UINT8 IpFlag : 3;
  UINT8 Reserved1 : 5;
  UINT8 Precedence : 3;
  UINT8 Reserved2 : 1;
  UINT8 ServiceType : 4;
} EFI_LAN_IPV4_HDR_PARAM;

typedef struct {
  UINT8 RcmpPortMsb;
  UINT8 RcmpPortLsb;
} EFI_LAN_RCMP_PORT;

typedef struct {
  UINT8 EnableBmcArpResponse : 1;
  UINT8 EnableBmcGratuitousArp : 1;
  UINT8 Reserved : 6;
} EFI_LAN_BMC_GENERATED_ARP_CONTROL;

typedef struct {
  UINT8 ArpInterval;
} EFI_LAN_ARP_INTERVAL;

typedef struct {
  UINT8 Data[18];
} EFI_LAN_COMMUNITY_STRING;

typedef struct {
  UINT8 DestinationSelector : 4;
  UINT8 Reserved2 : 4;
  UINT8 DestinationType : 3;
  UINT8 Reserved1 : 4;
  UINT8 AlertAcknowledged : 1;
} EFI_LAN_DEST_TYPE;

typedef struct {
  UINT8               DestinationSelector : 4;
  UINT8               Reserved1 : 4;
  UINT8               AlertingIpAddressSelector : 4;
  UINT8               AddressFormat : 4;
  UINT8               UseDefaultGateway : 1;
  UINT8               Reserved2 : 7;
  EFI_LAN_IP_ADDRESS  AlertingIpAddress;
  EFI_LAN_MAC_ADDRESS AlertingMacAddress;
} EFI_LAN_DEST_ADDRESS;

typedef union {
  EFI_LAN_AUTH_TYPE                 EfiLanAuthType;
  EFI_LAN_IP_ADDRESS                EfiLanIpAddress;
  EFI_LAN_IP_ADDRESS_SRC            EfiLanIpAddressSrc;
  EFI_LAN_MAC_ADDRESS               EfiLanMacAddress;
  EFI_LAN_SUBNET_MASK               EfiLanSubnetMask;
  EFI_LAN_IPV4_HDR_PARAM            EfiLanIpv4HdrParam;
  EFI_LAN_RCMP_PORT                 EfiLanPrimaryRcmpPort;
  EFI_LAN_BMC_GENERATED_ARP_CONTROL EfiLanArpControl;
  EFI_LAN_ARP_INTERVAL              EfiLanArpInterval;
  EFI_LAN_COMMUNITY_STRING          EfiLanCommunityString;
  EFI_LAN_DEST_TYPE                 EfiLanDestType;
  EFI_LAN_DEST_ADDRESS              EfiLanDestAddress;
} EFI_LAN_OPTIONS;

#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Get Lan Configuration Parameters command    
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_GET_LAN_CONFIG_PARAMETERS 0x02

//
//  Constants and Structure definitions for "Get Lan Configuration Parameters" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Suspend BMC ARPs command    
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_SUSPEND_BMC_ARPS  0x03

//
//  Constants and Structure definitions for "Suspend BMC ARPs" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get IP-UDP-RMCP Statistics command  
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_GET_PACKET_STATISTICS 0x04

//
//  Constants and Structure definitions for "Get IP-UDP-RMCP Statistics" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Serial/Modem Configuration command  
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_SET_SERIAL_CONFIGURATION  0x10

//
//  Constants and Structure definitions for "Set Serial/Modem Configuration" command to follow here
//
#pragma pack(1)
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
} EMP_AUTH_TYPE;

typedef struct {
  UINT8 EnableBasicMode : 1;
  UINT8 EnablePPPMode : 1;
  UINT8 EnableTerminalMode : 1;
  UINT8 Reserved1 : 2;
  UINT8 SnoopOsPPPNegotiation : 1;
  UINT8 Reserved2 : 1;
  UINT8 DirectConnect : 1;
} EMP_CONNECTION_TYPE;

typedef struct {
  UINT8 InactivityTimeout : 4;
  UINT8 Reserved : 4;
} EMP_INACTIVITY_TIMEOUT;

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
} EMP_CHANNEL_CALLBACK_CONTROL;

typedef struct {
  UINT8 CloseSessionOnDCDLoss : 1;
  UINT8 EnableSessionInactivityTimeout : 1;
  UINT8 Reserved : 6;
} EMP_SESSION_TERMINATION;

typedef struct {
  UINT8 Reserved1 : 5;
  UINT8 EnableDtrHangup : 1;
  UINT8 FlowControl : 2;
  UINT8 BitRate : 4;
  UINT8 Reserved2 : 4;
  UINT8 SaveSetting : 1;
  UINT8 SetComPort : 1;
  UINT8 Reserved3 : 6;
} EMP_MESSAGING_COM_SETTING;

typedef struct {
  UINT8 RingDurationInterval : 6;
  UINT8 Reserved1 : 2;
  UINT8 RingDeadTime : 4;
  UINT8 Reserved : 4;
} EMP_MODEM_RING_TIME;

typedef struct {
  UINT8 Reserved;
  UINT8 InitString[48];
} EMP_MODEM_INIT_STRING;

typedef struct {
  UINT8 EscapeSequence[5];
} EMP_MODEM_ESC_SEQUENCE;

typedef struct {
  UINT8 HangupSequence[8];
} EMP_MODEM_HANGUP_SEQUENCE;

typedef struct {
  UINT8 ModelDialCommend[8];
} MODEM_DIALUP_COMMAND;

typedef struct {
  UINT8 PageBlackoutInterval;
} PAGE_BLACKOUT_INTERVAL;

typedef struct {
  UINT8 CommunityString[18];
} EMP_COMMUNITY_STRING;

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
  union {
    struct {
      UINT8 Reserved5 : 4;
      UINT8 DialStringSelector : 4;
    } DIAL_PAGE_DESTINATION;
    struct {
      UINT8 TapAccountSelector : 4;
      UINT8 Reserved : 4;
    } TAP_PAGE_DESTINATION;
    struct {
      UINT8 PPPAccountSetSelector;
      UINT8 DialStringSelector;
    } PPP_ALERT_DESTINATION;
  } DEST_TYPE_SPECIFIC;
} EMP_DESTINATION_INFO;

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
} EMP_DESTINATION_COM_SETTING;

typedef struct {
  UINT8 DialStringSelector : 4;
  UINT8 Reserved1 : 4;
  UINT8 Reserved2;
  UINT8 DialString[48];
} DESTINATION_DIAL_STRING;

typedef struct {
  UINT8 IpAddressSelector : 4;
  UINT8 Reserved1 : 4;
  union {
    UINT32  IpAddressLong;
    UINT8   IpAddress[4];
  } PPP_IP_ADDRESS;
} DESTINATION_IP_ADDRESS;

typedef struct {
  UINT8 TapSelector;
  UINT8 TapServiceSelector : 4;
  UINT8 TapDialStringSelector : 4;
} DESTINATION_TAP_ACCOUNT;

typedef struct {
  UINT8 TapSelector;
  UINT8 PagerIdString[16];
} TAP_PAGER_ID_STRING;

typedef union {
  UINT8                         OptionData;
  EMP_AUTH_TYPE                 EmpAuthType;
  EMP_CONNECTION_TYPE           EmpConnectionType;
  EMP_INACTIVITY_TIMEOUT        EmpInactivityTimeout;
  EMP_CHANNEL_CALLBACK_CONTROL  EmpCallbackControl;
  EMP_SESSION_TERMINATION       EmpSessionTermination;
  EMP_MESSAGING_COM_SETTING     EmpMessagingComSetting;
  EMP_MODEM_RING_TIME           EmpModemRingTime;
  EMP_MODEM_INIT_STRING         EmpModemInitString;
  EMP_MODEM_ESC_SEQUENCE        EmpModemEscSequence;
  EMP_MODEM_HANGUP_SEQUENCE     EmpModemHangupSequence;
  MODEM_DIALUP_COMMAND          EmpModemDialupCommand;
  PAGE_BLACKOUT_INTERVAL        EmpPageBlackoutInterval;
  EMP_COMMUNITY_STRING          EmpCommunityString;
  EMP_DESTINATION_INFO          EmpDestinationInfo;
  EMP_DESTINATION_COM_SETTING   EmpDestinationComSetting;
  UINT8                         CallRetryBusySignalInterval;
  DESTINATION_DIAL_STRING       DestinationDialString;
  DESTINATION_IP_ADDRESS        DestinationIpAddress;
  DESTINATION_TAP_ACCOUNT       DestinationTapAccount;
  TAP_PAGER_ID_STRING           TapPagerIdString;
} EFI_EMP_OPTIONS;
#pragma pack()

/*----------------------------------------------------------------------------------------
    Definitions for Get Serial/Modem Configuration command  
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_GET_SERIAL_CONFIGURATION  0x11

//
//  Constants and Structure definitions for "Get Serial/Modem Configuration" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Serial/Modem Mux command    
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_SET_SERIAL_MUX  0x12

//
//  Constants and Structure definitions for "Set Serial/Modem Mux" command to follow here
//
typedef struct {
  UINT8 ChannelNo : 4;
  UINT8 Reserved1 : 4;
  UINT8 MuxSetting : 4;
  UINT8 Reserved2 : 4;
} EFI_SET_MUX_COMMAND;

typedef struct {
  UINT8 MuxSetToBmc : 1;
  UINT8 CommandStatus : 1;
  UINT8 MessagingSessionActive : 1;
  UINT8 AlertInProgress : 1;
  UINT8 Reserved2 : 2;
  UINT8 MuxToBmcAllowed : 1;
  UINT8 MuxToSystemBlocked : 1;
} EFI_MUX_CONFIG;

/*----------------------------------------------------------------------------------------
    Definitions for Get TAP Response Code command   
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_GET_TAP_RESPONSE_CODE 0x13

//
//  Constants and Structure definitions for "Get TAP Response Code" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set PPP UDP Proxy Transmit Data command     
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_SET_PPP_UDP_PROXY_TXDATA  0x14

//
//  Constants and Structure definitions for "Set PPP UDP Proxy Transmit Data" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get PPP UDP Proxy Transmit Data command     
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_GET_PPP_UDP_PROXY_TXDATA  0x15

//
//  Constants and Structure definitions for "Get PPP UDP Proxy Transmit Data" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Send PPP UDP Proxy Packet command   
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_SEND_PPP_UDP_PROXY_PACKET 0x16

//
//  Constants and Structure definitions for "Send PPP UDP Proxy Packet" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get PPP UDP Proxy Receive Data command  
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_GET_PPP_UDP_PROXY_RX  0x17

//
//  Constants and Structure definitions for "Get PPP UDP Proxy Receive Data" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Serial/Modem connection active command  
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_SERIAL_CONNECTION_ACTIVE  0x18

//
//  Constants and Structure definitions for "Serial/Modem connection active" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Callback command    
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_CALLBACK  0x19

//
//  Constants and Structure definitions for "Callback" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set user Callback Options command   
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_SET_USER_CALLBACK_OPTIONS 0x1A

//
//  Constants and Structure definitions for "Set user Callback Options" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get user Callback Options command   
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_GET_USER_CALLBACK_OPTIONS 0x1B

//
//  Constants and Structure definitions for "Get user Callback Options" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for SOL activating command  
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_SOL_ACTIVATING  0x20

//
//  Constants and Structure definitions for "SOL activating" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set SOL Configuration Parameters command    
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_SET_SOL_CONFIG_PARAM  0x21

//
//  Constants and Structure definitions for "Set SOL Configuration Parameters" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get SOL Configuration Parameters command    
----------------------------------------------------------------------------------------*/
#define EFI_TRANSPORT_GET_SOL_CONFIG_PARAM  0x22

//
//  Constants and Structure definitions for "Get SOL Configuration Parameters" command to follow here
//

#endif
