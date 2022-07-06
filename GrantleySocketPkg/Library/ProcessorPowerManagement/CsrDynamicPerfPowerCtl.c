//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/** @file

  Code File for CPU Power Management

  Copyright (c) 2006 - 2012, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  CsrDynamicPerfPowerCtl.c

**/

#include "CpuPpmIncludes.h"

VOID
ProgramCsrDynamicPerfPowerCtl (
  EFI_CPU_PM_STRUCT   *ppm
  )
/*++

Routine Description:

  This routine is called to program DYNAMIC_PERF_POWER_CTL.  It will be call multiple time passing any the SocketNumber to be programmed.

Arguments:

  PPMPolicy      Pointer to PPM Policy protocol instance

Returns:

  EFI_SUCCESS

--*/
{
  UINT32   data;
  //
  //4987134 start
  //
   
  UINTN  RegAddr = 0;
  
  //
  //4987134 end
  //
  UINT8    Size = 4;
  UINT8     SocketNumber;
  EFI_CPU_CSR_ACCESS_PROTOCOL *CpuCsrAccess;


  SocketNumber = ppm->Info->SocketNumber;
  CpuCsrAccess = ppm->CpuCsrAccess;

  // Read (CSR)
  data = CpuCsrAccess->ReadCpuCsr(SocketNumber, 0, DYNAMIC_PERF_POWER_CTL_PCU_FUN2_REG);
  data &= ~(0x3C000000 + EEP_L_OVERRIDE_ENABLE \
			+ I_TURBO_OVERRIDE_ENABLE \
			+ CST_DEMOTION_OVERRIDE_ENABLE \
			+ TURBO_DEMOTION_OVERRIDE_ENABLE \
			+ UNOCRE_PERF_PLIMIT_OVERRIDE_ENABLE \
			+ EET_OVERRIDE_ENABLE \
			+ IO_BW_PLIMIT_OVERRIDE_ENABLE \
			+ IOM_APM_OVERRIDE_ENABLE \
			+ QPI_APM_OVERRIDE_ENABLE
			);

  data |= ppm->Setup->DynamicPerPowerCtl;
  CpuCsrAccess->WriteCpuCsr(SocketNumber, 0, DYNAMIC_PERF_POWER_CTL_PCU_FUN2_REG, data);
  RegAddr = CpuCsrAccess->GetCpuCsrAddress(SocketNumber, 0, DYNAMIC_PERF_POWER_CTL_PCU_FUN2_REG, &Size);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &data );

  // TBD-TODO: Code above handles the programming via setup options but still need to investigate if we need an auto option

  return;
}
