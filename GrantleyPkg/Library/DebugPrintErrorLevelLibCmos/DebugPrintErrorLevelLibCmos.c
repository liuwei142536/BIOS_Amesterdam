//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/** @file
  DebugPrintErrorLevel access library instance.

Copyright (c) 2010 - 2017, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#include <Base.h>
#include <Uefi.h>
#include <Library/CmosAccessLib.h>
#include <Platform.h>

#define SPI_FLASHTOP_4GB  0xFFFFFFFF  // Memory address of 4GB
#define SPI_FLASHBOTTOM 0xFF000000 // Memory address of 4GB - 16MB

UINT32 mDebugPrintErrorLevel  = 0xFFFFFFFF;

/**
  Returns the debug print error level mask for the current module.

  @return  Debug print error level mask for the current module.

**/
UINT32
EFIAPI
GetDebugPrintErrorLevel (
  VOID
  )
{
  EFI_STATUS Status;
  UINT32 ErrorLevel;

  if(!(((UINTN)&mDebugPrintErrorLevel <  (UINTN)SPI_FLASHTOP_4GB ) && ((UINTN)&mDebugPrintErrorLevel >= (UINTN)SPI_FLASHBOTTOM ))){
	  if (mDebugPrintErrorLevel == 0xFFFFFFFF) {
		  Status = ReadCmos32 (CMOS_DEBUG_PRINT_LEVEL_REG, &ErrorLevel);
		  mDebugPrintErrorLevel = ErrorLevel;
		  return ErrorLevel;
	  } else {
	      return mDebugPrintErrorLevel;
	  }
  } else {
      Status = ReadCmos32 (CMOS_DEBUG_PRINT_LEVEL_REG, &ErrorLevel);
      return ErrorLevel;
  }
}

/**
  Sets the global debug print error level mask fpr the entire platform.
  
  @param   ErrorLevel     Global debug print error level
  
  @retval  TRUE           The debug print error level mask was sucessfully set.
  @retval  FALSE          The debug print error level mask could not be set.

**/
BOOLEAN
EFIAPI
SetDebugPrintErrorLevel (
  IN UINT32 ErrorLevel
  )
{
  EFI_STATUS  Status;
  Status = WriteCmos32 (CMOS_DEBUG_PRINT_LEVEL_REG, &ErrorLevel);
  return (BOOLEAN)(Status == EFI_SUCCESS);
}
