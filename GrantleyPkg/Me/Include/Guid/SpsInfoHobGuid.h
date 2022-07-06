//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c)  1999-2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  SpsInfoHobGuid.h

Abstract:

  SPS info HOB definitions.

--*/

#ifndef _SPS_INFO_HOB_GUID_H_
#define _SPS_INFO_HOB_GUID_H_

#include <SpsMisc.h>

typedef struct _PLATFORM_INFORMATION_HOB_ {
    UINT16   FeatureSet;
    UINT8    WorkFlow;
    BOOLEAN  PwrOptBoot;    // NM may request power reduction at boot
    UINT8    Cores2Disable; // NM may request disabling processor cores
    UINT64   TimeStampCounter[TIMESTAMP_TYPES];
} EFI_HOB_TYPE_SPS_INFO;

//
// Definitions for SPS Info HOB
//
#define EFI_SPS_INFO_HOB_GUID \
        {0x489d2a71, 0xba4a, 0x444c, {0x9f, 0xe2, 0xa6, 0xb7, 0xe5, 0xcd, 0x78, 0x47}}

extern EFI_GUID gEfiSpsInfoHobGuid;

#endif // _SPS_INFO_HOB_GUID_H_

