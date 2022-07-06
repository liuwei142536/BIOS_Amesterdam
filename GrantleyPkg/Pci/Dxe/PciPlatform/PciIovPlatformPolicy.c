/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  2008 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PciIovPlatformPolicy.c
    
Abstract:

  Implementation of EFI PCI IOV PLATFORM Protocol.
    
Revision History:

--*/


#include <PiDxe.h>
#include "PciPlatform.h"
#include <Guid\SetupVariable.h>

#ifdef EFI_PCI_IOV_SUPPORT

EFI_STATUS
EFIAPI
GetSystemLowestPageSize (
  IN  EFI_PCI_IOV_PLATFORM_PROTOCOL           *This,
  OUT UINT32                                  *SystemLowestPageSize
)
/*++

  Routine Description:
    The GetSystemLowestPageSize() function retrieves the system lowest page size.
    
  Arguments:
    This                 - Pointer to the EFI_PCI_IOV_PLATFORM_PROTOCOL instance.
    SystemLowestPageSize - The system lowest page size. (This system supports a
                           page size of 2^(n+12) if bit n is set.)
    
  Returns:
    EFI_SUCCESS           - The function completed successfully.
    EFI_INVALID_PARAMETER - SystemLowestPageSize is NULL.
    
--*/
{
  UINT64                                Size;
  EFI_STATUS                            Status;
  SYSTEM_CONFIGURATION                  mSystemConfiguration;
  EFI_GUID                              mSystemConfigurationGuid = SYSTEM_CONFIGURATION_GUID;
  
  Size = sizeof(SYSTEM_CONFIGURATION);
  // AptioV server override: Changed variable name to Intelsetup
  Status = gRT->GetVariable(L"IntelSetup",
                            &mSystemConfigurationGuid,
                            NULL,
                            &Size,
                            &mSystemConfiguration);

  ASSERT_EFI_ERROR (Status);
  
  if (SystemLowestPageSize != NULL) {
    //
    // Page size is 4K
    //
    //*SystemLowestPageSize = 1;
    *SystemLowestPageSize = mSystemConfiguration.SystemPageSize;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI 
GetIovPlatformPolicy (
  IN  EFI_PCI_IOV_PLATFORM_PROTOCOL           *This,
  OUT EFI_PCI_IOV_PLATFORM_POLICY             *PciIovPolicy
)
/*++

  Routine Description:
    The GetIovPlatformPolicy() function retrieves the platform policy regarding PCI IOV.
    
  Arguments:
    This         - Pointer to the EFI_PCI_IOV_PLATFORM_PROTOCOL instance.
    PciIovPolicy - The platform policy for PCI IOV configuration.
    
  Returns:
    EFI_SUCCESS           - The function completed successfully.
    EFI_INVALID_PARAMETER - PciPolicy is NULL.
    
--*/
{
  UINT64                                Size;
  EFI_STATUS                            Status;
  SYSTEM_CONFIGURATION                  mSystemConfiguration;
  EFI_GUID                              mSystemConfigurationGuid = SYSTEM_CONFIGURATION_GUID;
  UINT8                                 PolicyEnable;

  PolicyEnable = 0;
  
  Size = sizeof(SYSTEM_CONFIGURATION);
  // AptioV server override: Changed variable name to Intelsetup
  Status = gRT->GetVariable(L"IntelSetup",
                            &mSystemConfigurationGuid,
                            NULL,
                            &Size,
                            &mSystemConfiguration);

  ASSERT_EFI_ERROR (Status);

  if (mSystemConfiguration.ARIEnable == TRUE) {
    PolicyEnable = PolicyEnable | EFI_PCI_IOV_POLICY_ARI;
  }

  if (mSystemConfiguration.SRIOVEnable == TRUE) {
    PolicyEnable = PolicyEnable | EFI_PCI_IOV_POLICY_SRIOV;
  }

  if (mSystemConfiguration.MRIOVEnable == TRUE) {
    PolicyEnable = PolicyEnable | EFI_PCI_IOV_POLICY_MRIOV;
  }
  
  if (PciIovPolicy != NULL) {
    //*PciIovPolicy = EFI_PCI_IOV_POLICY_ARI | EFI_PCI_IOV_POLICY_SRIOV;
    *PciIovPolicy = PolicyEnable;
  }
  return EFI_SUCCESS;
}

#endif

