/**
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
**/
/**

Copyright (c)  2010 - 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:

  MePlatformPolicyPei.h

@brief:

  ME Platform Policy for ME PEIMs

**/
#ifndef _PEI_ME_PLATFORM_POLICY_PPI_H_
#define _PEI_ME_PLATFORM_POLICY_PPI_H_

///
/// Include other definitions
///
///
/// Define the PPI GUID
///
#define PEI_ME_PLATFORM_POLICY_PPI_GUID \
  { \
    0x7ae3ceb7, 0x2ee2, 0x48fa, 0xaa, 0x49, 0x35, 0x10, 0xbc, 0x83, 0xca, 0xbf \
  }

///
/// Extern the GUID for PPI users.
///
extern EFI_GUID gPeiMePlatformPolicyPpiGuid;

///
/// Revision
///
#define PEI_ME_PLATFORM_POLICY_PPI_REVISION_1 1
#define PEI_ME_PLATFORM_POLICY_PPI_REVISION_2 2

///
/// PPI definition
///
typedef struct _PEI_ME_PLATFORM_POLICY_PPI {
  UINT8 Revision;
  UINT8 DidEnabled  : 1;
  UINT8 DidTimeout  : 1;
  UINT8 DidInitStat : 4;
  UINT8 Reserved;
} PEI_ME_PLATFORM_POLICY_PPI;

#endif
