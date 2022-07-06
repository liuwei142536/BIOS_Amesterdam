/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/** @file

  CPU Configuration Library.

  Copyright (c) 2006 - 2010, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  SmmCpuPlatformHookLibNull.c

**/

#include <Library/SmmCpuPlatformHookLib.h>



/**
  Checks if platform produces a valid SMI. 
  
  This function checks if platform produces a valid SMI. This function is 
  called at SMM entry to detect if this is a spurious SMI. This function 
  must be implemented in an MP safe way because it is called by multiple CPU
  threads.
  
  @retval TRUE              There is a valid SMI
  @retval FALSE             There is no valid SMI

**/
BOOLEAN
EFIAPI
PlatformValidSmi (
  VOID
  )
{
  return TRUE;
}


/**
  Clears platform top level SMI status bit. 
  
  This function clears platform top level SMI status bit.
  
  @retval TRUE              The platform top level SMI status is cleared.
  @retval FALSE             The paltform top level SMI status cannot be cleared.

**/
BOOLEAN
EFIAPI
ClearTopLevelSmiStatus (
  VOID
  )
{
  return TRUE;
}


/**
  Performs platform specific way of SMM BSP election.
  
  This function performs platform specific way of SMM BSP election.
  
  @param  IsBsp             Output parameter. TRUE: the CPU this function executes
                            on is elected to be the SMM BSP. FALSE: the CPU this 
                            function executes on is to be SMM AP.

  @retval EFI_SUCCESS       The function executes successfully.
  @retval EFI_NOT_READY     The function does not determine whether this CPU should be
                            BSP or AP. This may occur if hardware init sequence to
                            enable the determination is yet to be done, or the function
                            chooses not to do BSP election and will let SMM CPU driver to
                            use its default BSP election process.
  @retval EFI_DEVICE_ERROR  The function cannot determine whether this CPU should be
                            BSP or AP due to hardware error.

**/
EFI_STATUS
EFIAPI
PlatformSmmBspElection (
  OUT BOOLEAN     *IsBsp
  )
{
  return EFI_NOT_READY;
}

/**
  Get platform page table attribute .

  This function gets page table attribute of platform.
  
  @retval EFI_SUCCESS       The platform page table attribute from the address is determined.
  @retval EFI_UNSUPPORTED   The paltform not supoort to get page table attribute from the address.

**/
EFI_STATUS
EFIAPI
GetPlatformPageTableAttribute (
  IN  UINT64                Address,
  IN OUT SMM_PAGE_SIZE_TYPE    *PageSize,
  IN OUT UINTN              *NumOfPages,
  IN OUT UINTN              *PageAttribute
  )
{
  return EFI_UNSUPPORTED;
}
