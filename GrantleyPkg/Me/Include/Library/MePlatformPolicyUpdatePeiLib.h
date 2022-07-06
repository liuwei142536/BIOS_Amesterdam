/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/*++

Copyright (c) 2011 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  MePlatformPolicyUpdatePeiLib.h

Abstract:

  Header file for PEI MePlatformPolicyUpdate Library.

--*/
#ifndef _ME_PLATFORM_POLICY_UPDATE_PEI_LIB_H_
#define _ME_PLATFORM_POLICY_UPDATE_PEI_LIB_H_

EFI_STATUS
UpdatePeiMePlatformPolicy (
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN OUT  PEI_ME_PLATFORM_POLICY_PPI  *PeiMePlatformPolicyPpi
  )
/*++

Routine Description:

  Update the Me Platform Policy Library

Arguments:

  PeiServices             General purpose services available to every PEIM.
  PeiMePlatformPolicyPpi  Me Platform Policy Ppi

Returns:

  EFI_SUCCESS           Initialization complete.
  EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  EFI_DEVICE_ERROR      Device error, driver exits abnormally.

--*/
;
#endif
