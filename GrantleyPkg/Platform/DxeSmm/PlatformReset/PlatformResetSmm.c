/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  2011 - 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:

  PlatformReset.c

@brief:

  Provide the ResetSystem AP.

**/
#include "PlatformResetSmm.h"

EFI_RUNTIME_SERVICES  *gSmmRs;

VOID
EFIAPI
PlatformSmmResetSystem (
  IN EFI_RESET_TYPE   ResetType,
  IN EFI_STATUS       ResetStatus,
  IN UINTN            DataSize,
  IN CHAR16           *ResetData OPTIONAL
  )
/**

  @brief
  Reset the system.

  @param[in] ResetType            Warm or cold
  @param[in] ResetStatus          Possible cause of reset
  @param[in] DataSize             Size of ResetData in bytes
  @param[in] ResetData            Optional Unicode string

  @retval Does not return if the reset takes place.

**/
{
	  PCH_RESET_TYPE                      PchResetType;
	  //PCH_EXTENDED_RESET_TYPES            Type;
	  
	  DEBUG ((EFI_D_ERROR, "Entering PlatformSmmResetSystem.\n"));	  
	  
	  //
	  // Initialize the Reset Types structure
	  //
	  //ZeroMem(&Type, sizeof(PCH_EXTENDED_RESET_TYPES));

	  PchResetType = ResetType;

	   //
	   // Override PChReset to be HardReset to avoid issues with CPU only reser poliy
	   //
	   if ((PchResetType == EfiResetWarm) || (PchResetType == EfiResetCold)){
	      PchResetType = EfiResetCold;
	   }
/* Commenting below block of code as EFI_RESET_TYPE does not support GlobalReset and GlobalResetWithEc and also this ME command is not supported after END of post message.
	#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
	#if defined(AMT_SUPPORT) && AMT_SUPPORT
	    {
	      if ((PchResetType == GlobalReset) || (PchResetType == GlobalResetWithEc)) {
	        ///
	        /// Let ME do global reset if Me Fw is available
	        ///
	        Status = HeciSendCbmResetRequest (CBM_RR_REQ_ORIGIN_BIOS_POST, CBM_HRR_GLOBAL_RESET);
	        // When reset via ME is not supported use PchReset Instead
	        if (!EFI_ERROR (Status) && (Status != EFI_UNSUPPORTED)) {
	          ///
	          /// ME Global Reset should fail after EOP is sent.
	          /// Go to use PCH Reset
	          ///
	          gBS->Stall (1000000);
	        }
	      }
	    }
	#endif // AMT_SUPPORT
	#endif // ME_SUPPORT_FLAG
*/ 
        PchResetSmm  (PchResetType);

}

EFI_STATUS
InitializePlatformResetSmm (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
/**

  @brief
  Entry point of Platform Reset driver.

  @param[in] ImageHandle          Standard entry point parameter
  @param[in] SystemTable          Standard entry point parameter

  @retval EFI_SUCCESS             Reset RT protocol installed
  @retval All other error conditions encountered result in an ASSERT

**/
{	
  UINTN       i = 0;

  //
  // Check for SmmRsTableGuid in configuration table
  //
  for (i = 0; i < gSmst->NumberOfTableEntries; ++i) {
    if (CompareGuid (&gSmst->SmmConfigurationTable[i].VendorGuid, &SmmRsTableGuid)) {
    //
    // Get the SMM version of the Runtime Services Table SMM Configuration Table
    //
    gSmmRs = gSmst->SmmConfigurationTable[i].VendorTable;	      
    }	    
  }

  if ( gSmmRs == NULL ) {
    DEBUG ((EFI_D_LOAD, "SMM Runtime Services table not found\n"));
    return EFI_NOT_FOUND;
  }	

  //
  // Hook the reset system service in SMM runtime services table
  //
  gSmmRs->ResetSystem = PlatformSmmResetSystem;


  return EFI_SUCCESS;
}
