//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/** @file

  Code File for CPU Power Management

  Copyright (c) 2006 - 2016, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  MsrVrMiscConfig.c

**/

#include "CpuPpmIncludes.h"

VOID
ProgramVrMiscConfigMsr (
  EFI_CPU_PM_STRUCT   *ppm
  )
/*++
Routine Description:
  This routine is called to program VR_MISC_CONFIG MSR.
Arguments:
  PPMPolicy      Pointer to PPM Policy protocol instance
Returns:
  NONE
--*/
{
#ifdef DE_SKU
#endif  //#ifdef DE_SKU
}


VOID
ProgramVrMiscConfig2Msr (
  EFI_CPU_PM_STRUCT   *ppm
  )
/*++
Routine Description:
  This routine is called to program VR_MISC_CONFIG2 MSR.
Arguments:
  PPMPolicy      Pointer to PPM Policy protocol instance
Returns:
  NONE
--*/
{
#ifdef DE_SKU
#endif  //#ifdef DE_SKU
}
