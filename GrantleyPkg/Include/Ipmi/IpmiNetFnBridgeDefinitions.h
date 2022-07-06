//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/** @file
  All the definitions required for Bridge commands Header File.

  Copyright (c) 1999 - 2014, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.
**/

#ifndef _IPMINETFNBRIDGEDEFINITIONS_H_
#define _IPMINETFNBRIDGEDEFINITIONS_H_

//
// Net function definition for Bridge command
//
#define EFI_SM_NETFN_BRIDGE 0x02

/*----------------------------------------------------------------------------------------
    Definitions for Get Bridge State command    
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_GET_STATE  0x00

//
//  Constants and Structure definitions for "Get Bridge State" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Bridge State command    
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_SET_STATE  0x01

//
//  Constants and Structure definitions for "Set Bridge State" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get ICMB Address command    
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_GET_ICMB_ADDRESS 0x02

//
//  Constants and Structure definitions for "Get ICMB Address" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set ICMB Address command    
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_SET_ICMB_ADDRESS 0x03

//
//  Constants and Structure definitions for "Set ICMB Address" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Bridge Proxy Address command    
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_SET_PROXY_ADDRESS  0x04

//
//  Constants and Structure definitions for "Set Bridge Proxy Address" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Bridge Statistics command   
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_GET_BRIDGE_STATISTICS  0x05

//
//  Constants and Structure definitions for "Get Bridge Statistics" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get ICMB Capabilities command   
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_GET_ICMB_CAPABILITIES  0x06

//
//  Constants and Structure definitions for "Get ICMB Capabilities" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Clear Bridge Statistics command     
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_CLEAR_STATISTICS 0x08

//
//  Constants and Structure definitions for "Clear Bridge Statistics" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Bridge Proxy Address command    
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_GET_PROXY_ADDRESS  0x09

//
//  Constants and Structure definitions for "Get Bridge Proxy Address" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get ICMB Connector Info command     
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_GET_ICMB_CONNECTOR_INFO  0x0A

//
//  Constants and Structure definitions for "Get ICMB Connector Info " command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get ICMB Connection ID command  
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_GET_ICMB_CONNECTION_ID 0x0B

//
//  Constants and Structure definitions for "Get ICMB Connection ID" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get ICMB Connection ID command  
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_SEND_ICMB_CONNECTION_ID  0x0C

//
//  Constants and Structure definitions for "Send ICMB Connection ID" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Prepare for Discoveery command  
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_PREPARE_FOR_DISCOVERY  0x10

//
//  Constants and Structure definitions for "Prepare for Discoveery" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Addresses command   
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_GET_ADDRESSES  0x11

//
//  Constants and Structure definitions for "Get Addresses" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Discovered command  
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_SET_DISCOVERED 0x12

//
//  Constants and Structure definitions for "Set Discovered" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Chassis Device ID command   
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_GET_CHASSIS_DEVICEID 0x13

//
//  Constants and Structure definitions for "Get Chassis Device ID" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Chassis Device ID command   
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_SET_CHASSIS_DEVICEID 0x14

//
//  Constants and Structure definitions for "Set Chassis Device ID" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Bridge Request command  
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_REQUEST  0x20

//
//  Constants and Structure definitions for "Bridge Request" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Bridge Message command  
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_MESSAGE  0x21

//
//  Constants and Structure definitions for "Bridge Message" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get Event Count command     
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_GET_EVENT_COUNT  0x30

//
//  Constants and Structure definitions for "Get Event Count" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Event Destination command   
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_SET_EVENT_DESTINATION  0x31

//
//  Constants and Structure definitions for "Set Event Destination" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Event Reception State command   
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_SET_EVENT_RECEPTION_STATE  0x32

//
//  Constants and Structure definitions for "Set Event Reception State" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set Event Reception State command   
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_SET_EVENT_RECEPTION_STATE  0x32

//
//  Constants and Structure definitions for "Set Event Reception State" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Send ICMB Event Message command     
----------------------------------------------------------------------------------------*/
#define EFI_BRIDGE_SEND_ICMB_EVENT_MESSAGE  0x33

//
//  Constants and Structure definitions for "Send ICMB Event Message" command to follow here
//

#endif
