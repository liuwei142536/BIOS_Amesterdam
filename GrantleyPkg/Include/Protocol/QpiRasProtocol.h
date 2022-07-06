//
// This file contains 'Framework Code' and is licensed as such 
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.                 
//
/*++

Copyright (c)  1999 - 2005 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
  
    QpiRasProtocol.h
    
Abstract:

  GUID for QPI RAS protocol

--*/

#ifndef _QPI_RAS_PROTOCOL_H_
#define _QPI_RAS_PROTOCOL_H_

//#include "Tiano.h"
//#include "QpiRcParm.h"

//
// Forward reference for pure ANSI compatability
//

#define EFI_QPI_RAS_PROTOCOL_GUID \
   { \
      0x8fdc888d, 0x2162, 0x4971, 0x81, 0xc2, 0x35, 0xd3, 0xa1, 0xaa, 0x50, 0x47 \
   }

//EFI_FORWARD_DECLARATION (EFI_QPI_RAS_PROTOCOL);

typedef enum {
  QPIRAS_EVNT_ONLINE = 0,      // Logical/Physical Online event
  QPIRAS_EVNT_OFFLINE,         // Logical/Physical Offline event
  QPIRAS_EVNT_RESET_SM         // Reset QPI RAS state machine
}QPIRAS_EVENT_TYPE;

#define  QPIRAS_SOCKET_TYPE_CPU       0
#define  QPIRAS_SOCKET_TYPE_IOH       1
#define  QPIRAS_SOCKET_INFO_NA        0xFF

typedef enum {
  QPIRAS_SUCCESS = 0,  // The event handling completed and register settings are committed
  QPIRAS_PENDING,      // Some more work related to the event is pending (for Time sliced SMI RAS support)
  QPIRAS_INT_DOMAIN_PENDING,      // Indicate CPU online is complete except for Int domain setup  (for Time sliced SMI RAS support)
  QPIRAS_FAILURE_EVT_IN_PROGRESS,  // Error: An event is already in progress
  QPIRAS_FAILURE_INVALID_EVTTYPE,  // Error: Invalid Event type
  QPIRAS_FAILURE_CANNOT_OFFLINE,   // Error: Socket can not be offlined because it is needed to reach other sockets
  QPIRAS_FAILURE_SOC_UNREACHABLE,  // Error: Hot Added Socket is unreachable
  QPIRAS_FAILURE_CHKIN,            // Error: Hot Added Socket failed to check in
  QPIRAS_FAILURE_GENERAL           // Error: QPI RAS handler general failure
}QPIRAS_STATUS;

// Protocol Interfaces

//
// To cehck if onlining/offlining a given socket can succeed. If the onlined socket is not reachable from
// (i.e none of the existing sockets are immediate neighbor of the onlined socket) 
// or if the socket to be offlined is part of a pass thru path and if it is the only path to reach other
// sockets in the system, then the socket online/offline request will be rejected. For online event,
// the caller should make sure the socket is powered on before calling this API.
//
typedef 
QPIRAS_STATUS 
//EFI_RUNTIMESERVICE 
(EFIAPI *EFI_QPI_RAS_CHECK_EVENT) (
  IN UINT8          EventType,
  IN UINT8          SocketInfo
);

//
// To be called for each O*L event that affects the QPI fabric. After successful handling of the event,
// the QPI RAS driver will Quiesce the system and commit the register settings.
// This API is time sliced; so it should be called repeatedly when the return code is "QPIRAS_PENDING"
// or "QPIRAS_INT_DOMAIN_PENDING"; return code "QPIRAS_SUCCESS" indicates successful hanlding;
// all other codes indicate failure. In all failure cases, the QPIRAS driver will reset the state machine and
// return the error code. The O*L handling can be aborted by calling this API with event type set to
// "QPIRAS_EVNT_RESET_SM".
//
typedef 
QPIRAS_STATUS 
//EFI_RUNTIMESERVICE 
(EFIAPI *EFI_QPI_RAS_HANDLER) (
  IN UINT8          EventType,
  IN UINT8          SocketInfo
);

//
// To be called to include/exclude the onlined socket in IPI target list of the SBSP
//
typedef 
QPIRAS_STATUS 
//EFI_RUNTIMESERVICE 
(EFIAPI *EFI_QPI_RAS_ADJUST_IPI_LIST) (
  IN BOOLEAN        Include
);

//
// To be called for RAS event that affects system wide memory configuration. For example,
// if there is some change in system memory configuration including mirror mode, hemisphere setting, etc.
// this API needs to be called to change the System Coherency setting accordingly. Note that the caller must
// make sure this API can acquaire the Quiesce buffer. This API will release the Quiesce buffer before
// returning. Also, the caller is responsible for committing the register settings produced by this API.
//
typedef 
QPIRAS_STATUS 
//EFI_RUNTIMESERVICE 
(EFIAPI *EFI_QPI_RAS_COH_HANDLER) (
  IN BOOLEAN        HemiMode,
  IN UINT8          SlaveB0List,
  IN UINT8          SlaveB1List
);


typedef struct _EFI_QPI_RAS_PROTOCOL {
  BOOLEAN                       HostParmValid; // Flag to indicate if the QPI system info is valid
  // BKL_PORTING
  //QPI_PARM                      *HostParm; // All the latest QPI info of the system
  EFI_QPI_RAS_CHECK_EVENT       QpiRasCheckEvent;
  EFI_QPI_RAS_HANDLER           QpiRasHandler;
  EFI_QPI_RAS_ADJUST_IPI_LIST   QpiRasAdjustIpiList;
  EFI_QPI_RAS_COH_HANDLER       QpiRasCohHandler;
} EFI_QPI_RAS_PROTOCOL;

extern EFI_GUID gEfiQpiRasProtocolGuid;

//Porting for Platform reqd
// dummy definition for structure; to be later replaced by native definition
// for Platform

typedef struct{
  UINT8 dummy;
}QPI_PARM;
//BKL_PORTING

#endif
