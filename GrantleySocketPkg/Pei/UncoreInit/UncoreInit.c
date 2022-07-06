/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed  
  for Intel CPUs and Chipsets under the terms of your license   
  agreement with Intel or your vendor.  This file may be        
  modified by the user, subject to additional terms of the      
  license agreement                                             
--*/
/*++

Copyright (c) 2005-2010 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  UncoreInitPeim.c

--*/
#include "Uefi.h"
#include <Library/MemoryQpiInitLib.h>
#include <Library/OemMemoryQpiInit.h>

EFI_STATUS
EFIAPI
PeimMemoryQpiInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
/*++

Routine Description:

  Driver entry point                

Arguments:

  FfsHeader -  Not used.
  PeiServices - General purpose services available to every PEIM.

Returns:

  EFI_SUCCESS:  Memory initialization completed successfully.
  All other error conditions encountered result in an ASSERT.

--*/
{
  SYSHOST host;

  //
  // Initialize data structures
  //
  MemSetLocal ((VOID *) &host, 0x00, sizeof (SYSHOST));

  host.var.common.oemVariable = (UINT32) PeiServices;

  ProcMemInit (&host);
#ifdef SERIAL_DBG_MSG
  rcPrintf ((&host, " PeimMemoryQpiInit END \n"));
#endif
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
UncoreInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
/*++

Routine Description:

  Driver entry point                

Arguments:

  FfsHeader -  Not used.
  PeiServices - General purpose services available to every PEIM.

Returns:

  EFI_SUCCESS:  Memory initialization completed successfully.
  All other error conditions encountered result in an ASSERT.

--*/
{
  return PeimMemoryQpiInit(FileHandle, PeiServices);
}
