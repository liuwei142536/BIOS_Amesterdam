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

  MePolicyInitPei.c

@brief:

  This file is SampleCode for Intel ME PEI Platform Policy initialzation.

**/

#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include "Protocol/MePlatformPolicyPei.h"
#include "Library/MePlatformPolicyUpdatePeiLib.h"
#include "Library/MeTypeLib.h"

///
/// Function implementations
///
EFI_STATUS
MePolicyInitPeiEntryPoint (
  IN EFI_FFS_FILE_HEADER       *FfsHeader,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
/**

  @brief
  Initilize Intel ME PEI Platform Policy

  @param[in] FfsHeader            Pointer to Firmware File System file header.
  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval EFI_OUT_OF_RESOURCES    Unable to allocate necessary data structures.

**/
{
  EFI_STATUS                  Status;
  PEI_ME_PLATFORM_POLICY_PPI  *MePlatformPolicyPpi;
  EFI_PEI_PPI_DESCRIPTOR      *MePlatformPolicyPpiDesc;

  ///
  /// Allocate descriptor and PPI structures
  ///
  MePlatformPolicyPpi = (PEI_ME_PLATFORM_POLICY_PPI *) AllocateZeroPool (sizeof (PEI_ME_PLATFORM_POLICY_PPI));
  ASSERT (MePlatformPolicyPpi != NULL);
  if (MePlatformPolicyPpi == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  MePlatformPolicyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  ASSERT (MePlatformPolicyPpiDesc != NULL);
  if (MePlatformPolicyPpiDesc == NULL) {
    FreePool (MePlatformPolicyPpi); // AptioV Grangeville Override: Fix for CPP Check Error EIP#277300
    return EFI_OUT_OF_RESOURCES;
  }
  ///
  /// Initialize the PPI
  ///
  MePlatformPolicyPpiDesc->Flags  = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  MePlatformPolicyPpiDesc->Guid   = &gPeiMePlatformPolicyPpiGuid;

  ///
  /// Update the REVISION number
  ///
  MePlatformPolicyPpi->Revision = PEI_ME_PLATFORM_POLICY_PPI_REVISION_2;

  ///
  /// Initialize the Platform Configuration
  ///
  UpdatePeiMePlatformPolicy (PeiServices, MePlatformPolicyPpi);

  MePlatformPolicyPpiDesc->Ppi = MePlatformPolicyPpi;

  ///
  /// TBD put initialize codes in here if needs
  ///
  ///
  /// Install the ME PEI Platform Policy PPI
  ///
  Status = (**PeiServices).InstallPpi (PeiServices, MePlatformPolicyPpiDesc);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((EFI_D_INFO, "ME PEI Platform Policy PPI Installed\n"));

  return Status;
}
