/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed  
  for Intel CPUs and Chipsets under the terms of your license   
  agreement with Intel or your vendor.  This file may be        
  modified by the user, subject to additional terms of the      
  license agreement                                             
--*/
/** @file
Implementation of CPU driver for PEI phase.

  Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.                                       

Module Name: Bist.c

**/

#include "CpuPei.h"
#include "Bist.h"

EFI_SEC_PLATFORM_INFORMATION_PPI mSecPlatformInformationPpi = {
  SecPlatformInformation
};

EFI_PEI_PPI_DESCRIPTOR mPeiSecPlatformInformationPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiSecPlatformInformationPpiGuid,
  &mSecPlatformInformationPpi
};

/**

  Implementation of the PlatformInformation service
  
  Implementation of the PlatformInformation service in EFI_SEC_PLATFORM_INFORMATION_PPI.
  This function will parse SecPlatform Information from BIST Hob.

  @param  PeiServices                Pointer to the PEI Services Table.
  @param  StructureSize              Pointer to the variable describing size of the input buffer.
  @param  PlatformInformationRecord  Pointer to the EFI_SEC_PLATFORM_INFORMATION_RECORD.
  
  @retval EFI_SUCCESS                The data was successfully returned.
  @retval EFI_BUFFER_TOO_SMALL       The buffer was too small.
  
**/
EFI_STATUS
EFIAPI
SecPlatformInformation (
  IN CONST EFI_PEI_SERVICES                  **PeiServices,
  IN OUT UINT64                              *StructureSize,
     OUT EFI_SEC_PLATFORM_INFORMATION_RECORD *PlatformInformationRecord
  )
{
  EFI_HOB_GUID_TYPE       *GuidHob;
  VOID                    *DataInHob;
  UINTN                   DataSize;

  GuidHob = GetFirstGuidHob (&gEfiHtBistHobGuid);
  if (GuidHob == NULL) {
    *StructureSize = 0;
    return EFI_SUCCESS;
  }

  DataInHob = GET_GUID_HOB_DATA (GuidHob);
  DataSize  = GET_GUID_HOB_DATA_SIZE (GuidHob);

  //
  // return the information from BistHob
  //
  if ((*StructureSize) < (UINT64) DataSize) {
    *StructureSize = (UINT64) DataSize;
    return EFI_BUFFER_TOO_SMALL;
  }

  *StructureSize = (UINT64) DataSize;
   
  CopyMem (PlatformInformationRecord, DataInHob, DataSize);

  return EFI_SUCCESS;
}

/**
  A callback function to build CPU(only BSP) BIST. 

  This function is a callback function to build bsp's BIST by calling SecPlatformInformationPpi

  @param  PeiServices      Pointer to PEI Services Table      
  @param  NotifyDescriptor Address if the notification descriptor data structure 
  @param  Ppi              Address of the PPI that was installed     
  @retval EFI_SUCCESS      Retrieve of the BIST data successfully 
  @retval EFI_SUCCESS      No sec platform information ppi export   
  @retval EFI_SUCCESS      The boot mode is S3 path   
**/
EFI_STATUS
EFIAPI
BuildBistHob (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                        Status;
  EFI_BOOT_MODE                     BootMode;
  EFI_SEC_PLATFORM_INFORMATION_PPI  *SecPlatformInformationPpi;
  EFI_PEI_PPI_DESCRIPTOR            *SecInformationDescriptor;
  UINT64                            InformationSize;
  EFI_SEC_PLATFORM_INFORMATION_RECORD   *SecPlatformInformation;

  Status = (**PeiServices).GetBootMode (PeiServices, &BootMode);
  if (!EFI_ERROR (Status) && (BootMode == BOOT_ON_S3_RESUME)) {
    return EFI_SUCCESS;
  }

  Status = (**PeiServices).LocatePpi (
                            PeiServices,
                            &gEfiSecPlatformInformationPpiGuid, // GUID
                            0,                                  // INSTANCE
                            //#ifndef R864_BUILD
                            &SecInformationDescriptor,          // EFI_PEI_PPI_DESCRIPTOR
                            //#else
                            //NULL,
                            //#endif
                            &SecPlatformInformationPpi          // PPI
                            );

  if (Status == EFI_NOT_FOUND) {
    return EFI_SUCCESS;
  }

  if (EFI_ERROR (Status)) {
    return Status;
  }

  InformationSize         = 0;
  SecPlatformInformation  = NULL;
  Status = SecPlatformInformationPpi->PlatformInformation (
                                        PeiServices,
                                        &InformationSize,
                                        SecPlatformInformation
                                        );
  if (Status == EFI_BUFFER_TOO_SMALL) {
    Status = (*PeiServices)->AllocatePool (
                              PeiServices,
                              (UINTN) InformationSize,
                              &SecPlatformInformation
                              );

    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = SecPlatformInformationPpi->PlatformInformation (
                                          PeiServices,
                                          &InformationSize,
                                          SecPlatformInformation
                                          );
  }

  if (EFI_ERROR (Status)) {
    return Status;
  }

    BuildGuidDataHob (
            &gEfiHtBistHobGuid,
            SecPlatformInformation,
            (UINTN) InformationSize
            );

  //
  // The old SecPlatformInformation Ppi is on CAR.
  // After memory discovered, we should never get it from CAR, or the data will be crashed.
  // So, we reinstall SecPlatformInformation PPI here.
  //
  Status = (**PeiServices).ReInstallPpi (
                             PeiServices,
                             SecInformationDescriptor,
                             &mPeiSecPlatformInformationPpi
                             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return Status;
}
