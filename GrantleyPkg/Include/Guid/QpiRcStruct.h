//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/*++

Copyright (c)  1999 - 2008, Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  QpiRcParm.h
    
Abstract:

  GUID used for QPIRC input/output parameter structure in the HOB list.

--*/

#ifndef _QPIRC_PARM_GUID_H_
#define _QPIRC_PARM_GUID_H_

#define EFI_RAS_HOST_GUID \
  { \
    0x8149fbb8, 0xa2cf, 0x4234, 0xb5, 0x6, 0xb7, 0x62, 0x55, 0xf7, 0xa3, 0x6d \
  }

// The structure is defined in QpiRcParm.h file	

extern EFI_GUID gEfiRasHostGuid;

#endif // _QPIRC_PARM_GUID_H_
