/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/** @file
  ICH7 Smm Library Services that implements both S/W SMI generation and detection. 

  Copyright (c) 2007 - 2009 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

**/


#include <Base.h>
#include <PchRegs.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>

#define BOOT_SERVICE_SOFTWARE_SMI_DATA          0
#define RUNTIME_SOFTWARE_SMI_DATA               1

/**
  Triggers a run time or boot time SMI.  

  This function triggers a software SMM interrupt and set the APMC status with an 8-bit Data.

  @param  Data                 The value to set the APMC status.

**/
VOID
InternalTriggerSmi (
  IN UINT8                     Data
  )
{
  UINT16                       PmBase;

  //
  // Get PMBase
  //
  PmBase = PcdGet16 (PcdPchAcpiIoPortBaseAddress);

  //
  // Set the EOS Bit & enable APMC SMI & enable SMI globally
  //
  IoOr32 (
    PmBase + R_PCH_SMI_EN,
    B_PCH_SMI_EN_APMC | B_PCH_SMI_EN_EOS | B_PCH_SMI_EN_GBL_SMI
    );

  //
  // Set APMC_STS
  //
  IoWrite8 (R_PCH_APM_STS, Data);

  //
  // Generate the APMC SMI
  //
  IoWrite8 (R_PCH_APM_CNT, PcdGet8 (PcdSmmActivationData));
}


/**
  Triggers an SMI at boot time.  

  This function triggers a software SMM interrupt at boot time.

**/
VOID
EFIAPI
TriggerBootServiceSoftwareSmi (
  VOID
  )
{
  InternalTriggerSmi (BOOT_SERVICE_SOFTWARE_SMI_DATA);
}


/**
  Triggers an SMI at run time.  

  This function triggers a software SMM interrupt at run time.

**/
VOID
EFIAPI
TriggerRuntimeSoftwareSmi (
  VOID
  )
{
  InternalTriggerSmi (RUNTIME_SOFTWARE_SMI_DATA);
}


/**
  Gets the software SMI data.  

  This function tests if a software SMM interrupt happens. If a software SMI happens, 
  it retrieves the SMM data and returns it as a non-negative value; otherwise a negative
  value is returned. 

  @return Data                 The data retrieved from SMM data port in case of a software SMI;
                               otherwise a negative value.

**/
INTN
InternalGetSwSmiData (
  VOID
  )
{
  UINT8                        SmiStatus;
  UINT8                        Data; 

  SmiStatus = IoRead8 (PcdGet16 (PcdPchAcpiIoPortBaseAddress) + R_PCH_SMI_STS);
  if (((SmiStatus & B_PCH_SMI_STS_APM) != 0) &&
       (IoRead8 (R_PCH_APM_CNT) == PcdGet8 (PcdSmmActivationData))) {
    Data = IoRead8 (R_PCH_APM_STS);
    return (INTN)(UINTN)Data; 
  } 
  
  return -1;
}  


/**
  Test if a boot time software SMI happened.  

  This function tests if a software SMM interrupt happened. If a software SMM interrupt happened and
  it was triggered at boot time, it returns TRUE. Otherwise, it returns FALSE.

  @retval TRUE   A software SMI triggered at boot time happened.
  @retval FLASE  No software SMI happened or the software SMI was triggered at run time.

**/
BOOLEAN
EFIAPI
IsBootServiceSoftwareSmi (
  VOID
  )
{
  return (BOOLEAN) (InternalGetSwSmiData () == BOOT_SERVICE_SOFTWARE_SMI_DATA);
}


/**
  Test if a run time software SMI happened.  

  This function tests if a software SMM interrupt happened. If a software SMM interrupt happened and
  it was triggered at run time, it returns TRUE. Otherwise, it returns FALSE.

  @retval TRUE   A software SMI triggered at run time happened.
  @retval FLASE  No software SMI happened or the software SMI was triggered at boot time.

**/
BOOLEAN
EFIAPI
IsRuntimeSoftwareSmi (
  VOID
  )
{
  return (BOOLEAN) (InternalGetSwSmiData () == RUNTIME_SOFTWARE_SMI_DATA);
}



/**

  Clear APM SMI Status Bit; Set the EOS bit. 
  
**/
VOID
EFIAPI
ClearSmi (
  VOID
  )
{

  UINT16                       PmBase;

  //
  // Get PMBase
  //
  PmBase = PcdGet16 (PcdPchAcpiIoPortBaseAddress);

  //
  // Clear the APM SMI Status Bit
  //
  IoWrite16 (PmBase + R_PCH_SMI_STS, B_PCH_SMI_STS_APM);

  //
  // Set the EOS Bit
  //
  IoOr32 (PmBase + R_PCH_SMI_EN, B_PCH_SMI_EN_EOS);
}

