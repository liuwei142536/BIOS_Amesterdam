/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c)  1999 - 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PlatformEarlyPolicy.c

Abstract:

  Do SMBUS Policy PPI Installation at early stage.

--*/

#include <Library/PeiServicesLib.h>
#include <Platform.h>
#include <Library/DebugLib.h>
#include <Ppi/SmbusPolicy.h>

static UINT8                        mSmbusRsvdAddresses[PLATFORM_NUM_SMBUS_RSVD_ADDRESSES] = {
  SMBUS_ADDR_CH_A_1,
  SMBUS_ADDR_CH_A_2,
  SMBUS_ADDR_CH_B_1,
  SMBUS_ADDR_CH_B_2,
};

static PEI_SMBUS_POLICY_PPI         mSmbusPolicyPpi = {
  PCH_SMBUS_BASE_ADDRESS,
  PCH_SMBUS_BUS_DEV_FUNC,
  PLATFORM_NUM_SMBUS_RSVD_ADDRESSES,
  mSmbusRsvdAddresses
};

static EFI_PEI_PPI_DESCRIPTOR       mPpiList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gPeiSmbusPolicyPpiGuid,
    &mSmbusPolicyPpi
  }
};

static EFI_PEI_PPI_DESCRIPTOR       mPlatformPolicyPpi = {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gPeiSmbusPolicyPpiGuid,
    &mSmbusPolicyPpi
  };


EFI_STATUS
EFIAPI
PlatformEarlyPolicyEntry (
  IN EFI_PEI_FILE_HANDLE       *FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
/*++

Routine Description:

  SMBUS Policy PPI Installation at early stage.

Arguments:

  FileHandle        Pointer to the PEIM FFS file header.
  PeiServices       General purpose services available to every PEIM.

Returns:

  EFI_SUCCESS       Operation completed successfully.
  Otherwise         SMBUS Policy PPI Installation failed.
--*/
{
  EFI_STATUS                            Status;

  //
  // Initialize platform PPIs
  //
  Status = (**PeiServices).InstallPpi (PeiServices, &mPlatformPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  
  return Status;
}
