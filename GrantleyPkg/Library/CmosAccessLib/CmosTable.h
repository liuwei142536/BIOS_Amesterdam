//
// This file contains a 'Sample Driver' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may be modified by the user, subject to
// the additional terms of the license agreement
//
/*++

Copyright (c) 2010 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  CmosTable.h

Abstract:

--*/

#ifndef _CMOS_TABLE_H_
#define _CMOS_TABLE_H_


#include "CmosMap.h"


#define	DEFAULT_ATTRIBUTES		 0
#define	EXCLUDE_FROM_CHECKSUM	 (1 << 0)

// Add the CMOS entry below

static struct CMOS_ENTRY {
  UINT8 CmosAddress;
  UINT8 DefaultValue;
  UINT8 Attribute;
}CMOS_TABLE[] = {
{ CPU_HT_POLICY, CPU_HT_POLICY_ENABLED, EXCLUDE_FROM_CHECKSUM },
{ TPM_POLICY, TPM_POLICY_ENABLED, DEFAULT_ATTRIBUTES },
};


#endif
