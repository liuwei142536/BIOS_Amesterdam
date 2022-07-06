/**
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
**/
/**

Copyright (c)  1999 - 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:

  AmtPolicyInitPei.c

@brief:

  This file is SampleCode for Intel AMT PEI Platform Policy initialzation.

**/
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include "AmtPlatformPolicyUpdatePeiLib.h"
#include "Library/MeTypeLib.h"

///
/// Function implementations
///
EFI_STATUS
AmtPolicyInitPeiEntryPoint (
  IN EFI_FFS_FILE_HEADER       *FfsHeader,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
/**

  @brief
  Initilize Intel AMT PEI Platform Policy

  @param[in] FfsHeader            Pointer to Firmware File System file header.
  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval EFI_OUT_OF_RESOURCES    Unable to allocate necessary data structures.

**/
{
  EFI_STATUS                  Status;
  PEI_AMT_PLATFORM_POLICY_PPI *AmtPlatformPolicyPpi;
  EFI_PEI_PPI_DESCRIPTOR      *AmtPlatformPolicyPpiDesc;

#if defined(SPS_SUPPORT) && SPS_SUPPORT
  if (!MeTypeIsAmt()) {
    return EFI_UNSUPPORTED;
  }
#endif //SPS_SUPPORT

  ///
  /// Allocate descriptor and PPI structures
  ///
  AmtPlatformPolicyPpi = (PEI_AMT_PLATFORM_POLICY_PPI *) AllocateZeroPool (sizeof (PEI_AMT_PLATFORM_POLICY_PPI));
  ASSERT (AmtPlatformPolicyPpi != NULL);
  if (AmtPlatformPolicyPpi == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  AmtPlatformPolicyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  ASSERT (AmtPlatformPolicyPpiDesc != NULL);
  if (AmtPlatformPolicyPpiDesc == NULL) {
    FreePool (AmtPlatformPolicyPpi); // AptioV Grangeville Override: Fix for CPP Check Error EIP#277300
    return EFI_OUT_OF_RESOURCES;
  }
  ///
  /// Initialize the PPI
  ///
  AmtPlatformPolicyPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  AmtPlatformPolicyPpiDesc->Guid  = &gPeiAmtPlatformPolicyPpiGuid;

  ///
  /// Update the REVISION number
  ///
  AmtPlatformPolicyPpi->Revision = PEI_AMT_PLATFORM_POLICY_PPI_REVISION_5;

  ///
  /// Initialize the Platform Configuration
  ///
  AmtPlatformPolicyPpi->iAmtEnabled       = 0;
  AmtPlatformPolicyPpi->WatchDog          = 0;
  AmtPlatformPolicyPpi->WatchDogOs        = 0;
  AmtPlatformPolicyPpi->WatchDogTimerBios = 0;
  AmtPlatformPolicyPpi->WatchDogTimerOs   = 0;
  AmtPlatformPolicyPpi->AsfEnabled        = 1;
  AmtPlatformPolicyPpi->FWProgress        = 1;
  AmtPlatformPolicyPpi->ManageabilityMode = 0;

  UpdatePeiAmtPlatformPolicy ((EFI_PEI_SERVICES **)PeiServices, AmtPlatformPolicyPpi);

  AmtPlatformPolicyPpiDesc->Ppi = AmtPlatformPolicyPpi;
  ///
  /// Install the AMT PEI Platform Policy PPI
  ///
  Status = (**PeiServices).InstallPpi (PeiServices, AmtPlatformPolicyPpiDesc);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((EFI_D_INFO, "AMT PEI Platform Policy PPI Installed\n"));

  return Status;
}
