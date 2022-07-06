/*++

Copyright (c) 1996 - 2009, Intel Corporation.

This source code and any documentation accompanying it ("Material") is furnished
under license and may only be used or copied in accordance with the terms of that
license.  No license, express or implied, by estoppel or otherwise, to any
intellectual property rights is granted to you by disclosure or delivery of these
Materials.  The Materials are subject to change without notice and should not be
construed as a commitment by Intel Corporation to market, license, sell or support
any product or technology.  Unless otherwise provided for in the license under which
this Material is provided, the Material is provided AS IS, with no warranties of
any kind, express or implied, including without limitation the implied warranties
of fitness, merchantability, or non-infringement.  Except as expressly permitted by
the license for the Material, neither Intel Corporation nor its suppliers assumes
any responsibility for any errors or inaccuracies that may appear herein.  Except
as expressly permitted by the license for the Material, no part of the Material
may be reproduced, stored in a retrieval system, transmitted in any form, or
distributed by any means without the express written consent of Intel Corporation.


Module Name:

  DcmiInit.h

Abstract:
   This file Contains Calllbacks Events for Different Sps Requirements: 
   
   EndOfPost
   DriectFirmwareUpgrade ( To be changed ) 
   
--*/
#ifndef _DCMIINIT_H 
#define _DCMIINIT_H

#define  ClientRegistered(RegisteredClient, bit )  ( ((RegisteredClient & (0x1 << bit))? TRUE : FALSE) ) 

// As defined in DCMI-HI Spec
// Revision 0.51
// Section 3.20

#define EFI_DCMI_HI_GUID \
  { \
    0x7519B383, 0x48fc, 0x43e5, 0xa5, 0xeb, 0x59, 0x59, 0xcb, 0x58, 0x10, 0x00 \
  }

#define  HOST_ENUMERATION_RESERVED_BYTES 3

#define HBM_COMMAND_TYPE_HOST_ENUMERATION_REQUEST 0x4


typedef struct {
	HECI_MSG_HEADER HeciHeader; 
	HBM_COMMAND     Command;
	UINT8           Reserved[HOST_ENUMERATION_RESERVED_BYTES]; // Must be 0
} HBM_HOST_ENUMERATION_REQUEST;


#define HBM_COMMAND_TYPE_HOST_ENUMERATION_RESPONSE 0x84
#define MAX_ADDRESS_IN_BYTES 32

typedef struct {
     HECI_MSG_HEADER HeciHeader; 
	 HBM_COMMAND     Command;
	 UINT8           Reserved[HOST_ENUMERATION_RESERVED_BYTES]; // Must be Null
	 UINT8           ValidAddress[MAX_ADDRESS_IN_BYTES];	   // Valid addresses for ME registered clients ( 256 bit array )
} HBM_HOST_ENUMERATION_RESPONSE;

#define HBM_HOST_CLIENT_PROPERTIES_RESERVED_BYTES 2

typedef union {
	HBM_HOST_ENUMERATION_REQUEST	Request;
	HBM_HOST_ENUMERATION_RESPONSE	Response;
} HBM_HOST_ENUMERATION_BUFFER;

// Client Properties Request

#define HBM_COMMAND_TYPE_HOST_CLIENT_PROPERTIES_REQUEST 0x5


typedef struct {
	HECI_MSG_HEADER HeciHeader; 
	HBM_COMMAND     Command;
	UINT8		Address;
	UINT8		Reserved[HBM_HOST_CLIENT_PROPERTIES_RESERVED_BYTES];
} HBM_HOST_CLIENT_PROPERTIES_REQUEST;

typedef struct {	
	EFI_GUID	ProtocolName;
	UINT8		ProtocolVersion;
	UINT8		MaxNumberOfConnection;
	UINT8		FixedAdddress;
	UINT8		SingleReceivedBuffer;
	UINT32		MaxMessageLength;		
} HBM_HOST_CLIENT_PROPERTIES;

#define HBM_HOST_CLIENT_PROPERTIES_RESPONSE_RESERVED_BYTES 1

typedef struct {
	HECI_MSG_HEADER HeciHeader; 
	HBM_COMMAND		Command;
	UINT8			Address;
	UINT8			Status;
	UINT8			Reserved[HBM_HOST_CLIENT_PROPERTIES_RESPONSE_RESERVED_BYTES];
	HBM_HOST_CLIENT_PROPERTIES	ClientProperties;
} HBM_HOST_CLIENT_PROPERTIES_RESPONSE;

typedef union  {
	HBM_HOST_CLIENT_PROPERTIES_REQUEST Request;
	HBM_HOST_CLIENT_PROPERTIES_RESPONSE Response;
}HBM_HOST_CLIENT_PROPERTIES_BUFFER;

// Client Connect Request


#define HBM_COMMAND_TYPE_HOST_CLIENT_CONNECT_REQUEST 0x6



typedef struct { 
	HECI_MSG_HEADER HeciHeader;
	HBM_COMMAND		Command;
	UINT8			MeAddress;
	UINT8			HostAddress;
	UINT8			Reserverd;
} HBM_HOST_CLIENT_CONNECT_REQUEST;


typedef struct { 
	HECI_MSG_HEADER HeciHeader;
	HBM_COMMAND		Command;
	UINT8			MeAddress;
	UINT8			HostAddress;
	UINT8			Status;
} HBM_HOST_CLIENT_CONNECT_RESPONSE;


#define HBM_COMMAND_TYPE_CLIENT_CONNECT_RESPONSE 0x86
#define HBM_COMMAND_TYPE_CLIENT_CONNECT_STATUS_SUCCESS 0x0
#define HBM_COMMAND_TYPE_CLIENT_CONNECT_STATUS_NOT_FOUND 0x1
#define HBM_COMMAND_TYPE_CLIENT_CONNECT_STATUS_ALREADY_CONNECTED 0x2
#define HBM_COMMAND_TYPE_CLIENT_CONNECT_STATUS_OUT_OF_RESOURCES 0x3
#define HBM_COMMAND_TYPE_CLIENT_CONNECT_STATUS_INVALID_PARAMETER 0x4

typedef union { 
	HBM_HOST_CLIENT_CONNECT_REQUEST Request;
	HBM_HOST_CLIENT_CONNECT_RESPONSE Response;
} HBM_HOST_CLIENT_CONNECT_BUFFER;

#define DM_ME_STATIC_ADDRESS	0x12
#define DM_HOST_ADDRESS		0x00
#define DM_HOST_ADDRESS_FC	0x02

#endif
