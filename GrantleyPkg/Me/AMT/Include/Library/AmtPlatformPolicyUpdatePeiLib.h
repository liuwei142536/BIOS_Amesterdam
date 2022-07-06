/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/*++

Copyright (c) 2011 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  AmtPlatformPolicyUpdatePeiLib.h

Abstract:

  Header file for PEI AmtPlatformPolicyUpdate Library.

--*/
#ifndef _AMT_PLATFORM_POLICY_UPDATE_PEI_LIB_H_
#define _AMT_PLATFORM_POLICY_UPDATE_PEI_LIB_H_

#include <Ppi\AmtPlatformPolicyPei.h>

EFI_STATUS
UpdatePeiAmtPlatformPolicy (
  IN      EFI_PEI_SERVICES            **PeiServices,
  IN OUT  PEI_AMT_PLATFORM_POLICY_PPI *PeiAmtPlatformPolicyPpi
  )
/*++

Routine Description:

  Update the Active Management Policy Ppi Library

Arguments:

  PeiServices             General purpose services available to every PEIM.
  PeiAmtPlatformPolicyPpi PEI Amt PlatformPolicy

Returns:

  EFI_SUCCESS           Initialization complete.
  EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  EFI_DEVICE_ERROR      Device error, driver exits abnormally.

--*/
;
#endif
