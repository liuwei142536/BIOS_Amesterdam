/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2010 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
  
    CpuInitPeim.c
   
Abstract:

    Functions for CPU initilization
    
--*/

#include "PlatformEarlyInit.h"


/**
  Program ACC bit to MSR for this thread.

  @param None

  @retval EFI_SUCCESS           - MSR Locked

**/
EFI_STATUS
AccProgramPatch (
  IN  PEI_MP_SERVICES_PPI  *MpService

  )
{
	UINT64 Data = 0;
	Data = AsmReadMsr64(0xe2);
	Data |= BIT16;
	AsmWriteMsr64 (0xe2, Data);
  return EFI_SUCCESS;
}

/**

  Executes the ACC enabling on S3 Resume.


  @param PeiServices   - Pointer to PEI Services Table.
  @param NotifyDesc    - Pointer to the descriptor for the Notification
                         event that caused this function to execute.
  @param Ppi           - Pointer the the Ppi data associated with this function.


  @retval EFI_SUCCESS in all cases.

**/

EFI_STATUS
EFIAPI
AccProgramCallback (
  IN      EFI_PEI_SERVICES          **PeiServices,
  IN      EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
  IN      VOID                      *Ppi
  )
{
  EFI_STATUS  Status;
  PEI_MP_SERVICES_PPI  *PeiMpServices;


  Status = (*PeiServices)->LocatePpi(
                      PeiServices,
                      &gPeiMpServicePpiGuid,
                      0,
                      NULL,
                      &PeiMpServices  
                      );
 
  
  AccProgramPatch(NULL);
  Status = PeiMpServices->StartupAllAPs (
            PeiServices,
            PeiMpServices,
            AccProgramPatch,
            TRUE,
            0,
            (VOID*)NULL
            );
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "Error: Not able to Program ACC.  Status: %r\n", Status));
  }
  return EFI_SUCCESS ;
}

STATIC
EFI_PEI_NOTIFY_DESCRIPTOR        mAccProgramNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiPostScriptTablePpiGuid,
  AccProgramCallback
};


EFI_STATUS
PlatformCpuInit (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN SYSTEM_CONFIGURATION       *SystemConfiguration
  )
{
  EFI_BOOT_MODE BootMode;
  EFI_STATUS Status = 0 ;
 
  PeiServicesGetBootMode (&BootMode);
  
  DEBUG((EFI_D_ERROR, "Error: Program ACC callback.  Status:\n"));
  //register call back for S3 bootscript
  if(SystemConfiguration->ProcessorAutonumousCstateEnable){
  Status = (*PeiServices)->NotifyPpi (PeiServices, &mAccProgramNotifyList);
  ASSERT_EFI_ERROR (Status);
  }
 
  return EFI_SUCCESS;
}
