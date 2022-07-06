//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/*++

Copyright (c) 2012 Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  MpstNodeData.h
    
Abstract:

  GUID used for MPST Node Data entries in the HOB list.

--*/

#ifndef _MPST_NODE_DATA_GUID_H_
#define _MPST_NODE_DATA_GUID_H_

#define MPST_MAX_NODES        32

extern EFI_GUID gEfiMpstNodeDataGuid;

extern CHAR16   EfiMPSTNodeDataVariable[];

//
//  MPST Data HOB information
//

#pragma pack(1)

struct MpstNodeElement {
   UINT8    BusNumber;      // Bus number for the CSR of the CPU Owning the memory
   UINT16   BaseAddress;    // Base Address of the element in 64MB chunks
   UINT16   ElementSize;    // Size of this memory element in 64MB chunks
   UINT8    AssocChannels;  // Channels mapped to this range
   UINT8    SplitByHole;    // Channel is split by a memory hole
};

struct MpstNodeData {
   UINT8    numberEntries;  // Number of Memory Map Elements
   struct   MpstNodeElement Element[MPST_MAX_NODES]; 
};

#pragma pack()

#endif
