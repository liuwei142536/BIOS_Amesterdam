/**
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement

@copyright
  Copyright (c)  2008 - 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

@file
  AmtPlatformPolicyPei.h

@brief
  AMT Platform Policy for AMT PEIMs

**/
#ifndef _PEI_AMT_PLATFORM_POLICY_PPI_H_
#define _PEI_AMT_PLATFORM_POLICY_PPI_H_

///
/// Include other definitions
///
///
/// Define the PPI GUID
///
#define PEI_AMT_PLATFORM_POLICY_PPI_GUID \
  { \
    0xb4a1208e, 0x4d9a, 0x4ea2, 0x9d, 0x6b, 0xe4, 0x1a, 0x61, 0xe6, 0xc5, 0xac \
  }

///
/// Extern the GUID for PPI users.
///
extern EFI_GUID gPeiAmtPlatformPolicyPpiGuid;

///
/// Revision
///
#define PEI_AMT_PLATFORM_POLICY_PPI_REVISION_1  1
///
/// Add AsfEnabled, and ManageabilityMode
///
#define PEI_AMT_PLATFORM_POLICY_PPI_REVISION_2  2
///
/// Added FWProgress
///
#define PEI_AMT_PLATFORM_POLICY_PPI_REVISION_3  3
///
/// Cleanup
///
#define PEI_AMT_PLATFORM_POLICY_PPI_REVISION_4  4
///
/// Cleanup
///
#define PEI_AMT_PLATFORM_POLICY_PPI_REVISION_5  5

///
/// PPI definition
///
typedef struct _PEI_AMT_PLATFORM_POLICY_PPI {
  UINT8   Revision;
  UINT8   iAmtEnabled : 1;
  UINT8   WatchDog : 1;
  UINT8   WatchDogOs : 1;
  UINT8   AsfEnabled : 1;
  UINT8   ManageabilityMode : 1;  ///< 0: Off, 1:On
  UINT8   Reserved1 : 1;
  UINT16  WatchDogTimerOs;
  UINT16  WatchDogTimerBios;
  UINT8   FWProgress;
} PEI_AMT_PLATFORM_POLICY_PPI;

#endif
