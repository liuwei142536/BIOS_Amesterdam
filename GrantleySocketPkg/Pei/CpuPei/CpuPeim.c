/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed  
  for Intel CPUs and Chipsets under the terms of your license   
  agreement with Intel or your vendor.  This file may be        
  modified by the user, subject to additional terms of the      
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2006 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

    CpuPeim.c
    
Abstract:

    EFI 2.0 PEIM to initialize the cache and program for unlock processor

Revision History

--*/

#include "CpuPei.h"
#include "Bist.h"


extern PEI_CACHE_PPI                mCachePpi;

EFI_PEI_PPI_DESCRIPTOR                  mPpiList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gPeiCachePpiGuid,
    &mCachePpi
  }
};


static EFI_PEI_NOTIFY_DESCRIPTOR        mNotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiMasterBootModePpiGuid,
    BuildBistHob
  }
};

VOID
InitXmm (
  VOID
  );

/**
  The Entry point of the CPU PEIM

  This function is the Entry point of the CPU PEIM which will install the CachePpi and 
  BuildBISTHob notifier. And also the function will deal with the relocation to memory when 
  permanent memory is ready
 
  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services. 
                          
  @retval EFI_SUCCESS   CachePpi and BIST hob build notification is installed
                        successfully.

**/
EFI_STATUS
EFIAPI
CpuPeimInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS  Status;

  //
  // Install PPIs
  //
  Status = PeiServicesInstallPpi(&mPpiList[0]);
  ASSERT_EFI_ERROR (Status);

  //
  // Register for PPI Notifications
  //
  Status = PeiServicesNotifyPpi (&mNotifyList[0]);
  ASSERT_EFI_ERROR (Status);

  //
  // Try to Init XMM support
  //
  InitXmm ();

  return Status;
}
