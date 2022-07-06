//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c)  1999 - 2006 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  MemorySetupData.h
    
Abstract:

  GUID used for Memory Setup Data entries in the HOB list.

--*/

#ifndef _PLATFORM_SLOT_INFO_PROTOCOL_H_
#define _PLATFORM_SLOT_INFO_PROTOCOL_H_

#include <UncoreCommonIncludes.h>

  // {F6A5F63E-1AA0-409f-8342-6A1B8F2ED6BC}
#define PLATFORM_SLOT_INFO_GUID \
    { \
      0xf6a5f63e, 0x1aa0, 0x409f, 0x83, 0x42, 0x6a, 0x1b, 0x8f, 0x2e, 0xd6, 0xbc \
    }


  /*
  // {F6A5F63E-1AA0-409f-8342-6A1B8F2ED6BC}
static const GUID <<name>> = 
{ 0xf6a5f63e, 0x1aa0, 0x409f, { 0x83, 0x42, 0x6a, 0x1b, 0x8f, 0x2e, 0xd6, 0xbc } };
*/

extern EFI_GUID gPlatformSlotInfoProtocolGuid;

typedef struct _PLATFORM_CHANNEL_INFO {
  BOOLEAN Valid;
  UINT8 numDimmSlotsSupp;
  UINT32 DimmSlotSupportedBitmap;
} PLATFORM_CHANNEL_INFO;

typedef struct _PLATFORM_NODE_INFO {
   BOOLEAN Valid;
   UINT8    numChSupp;
   UINT32    ChannelSupportedBitmap;   
   PLATFORM_CHANNEL_INFO ChannelInfo[MAX_CH];
} PLATFORM_NODE_INFO;

typedef struct _PLATFORM_INFO {
  UINT8 PlatformMaxSocket;
  UINT8 PlatformMaxNode;
  UINT32 SocketSupportedBitmap;
  UINT8 PlatformDimmsPerChannel;
  UINT8 NumSlots;
  PLATFORM_NODE_INFO NodeInfo[MAX_MEM_NODE];
} PLATFORM_SLOT_INFO_PROTOCOL;


#endif
