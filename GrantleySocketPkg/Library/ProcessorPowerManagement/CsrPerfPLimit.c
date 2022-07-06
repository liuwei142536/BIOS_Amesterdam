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

  Module Name:  CsrPerfPLimit.c

**/

#include "CpuPpmIncludes.h"

VOID
ProgramCsrPerfPlimitControl (
		EFI_CPU_PM_STRUCT   *ppm
		)
/*++

		Routine Description:

		This routine is called to program PERF_P_LIMIT_CONTROL. It will be call multiple time passing any the SocketNumber to be programmed.

		Arguments:

		PPMPolicy Pointer to PPM Policy protocol instance

		Returns:

		EFI_SUCCESS

--*/
{
  UINT32 data;
//
//4987134 start
// 
   
  UINTN RegAddr = 0;  
//
//4987134 end
//
  UINT8  Size = 4;
  UINT8   SocketNumber;
  EFI_CPU_CSR_ACCESS_PROTOCOL *CpuCsrAccess;

  SocketNumber = ppm->Info->SocketNumber;
  CpuCsrAccess = ppm->CpuCsrAccess;

  // Read CSR
  data = CpuCsrAccess->ReadCpuCsr(SocketNumber, 0, PERF_P_LIMIT_CONTROL_PCU_FUN2_REG);
  
  if((ppm->Info->CpuType == CPU_HASWELLSERVER) && (ppm->Info->CpuStepping < C0_REV_HSX) ) {
    data &= ~(PERF_P_LIMIT_THRESHOLD \
            + PERF_P_LIMIT_CLIP \
            + DISABLE_PERF_P_INPUT \
            + RESOLUTION_MODE \
            + REPERF_P_LIMIT_EN 
            );
  } else {
    data &= ~(PERF_PLIMIT_THRESHOLD \
            + PERF_PLIMIT_CLIP \
            + PERF_PLIMIT_DIFFERENTIAL \
            + REPERF_PLIMIT_EN 
            );
  }

  data |= ppm->Setup->PerPLimitCtl;

  // TBD-TODO: Code above handles the programming via setup options but still need to investigate if we need an auto option
  CpuCsrAccess->WriteCpuCsr(SocketNumber, 0, PERF_P_LIMIT_CONTROL_PCU_FUN2_REG, data);
  RegAddr = CpuCsrAccess->GetCpuCsrAddress(SocketNumber, 0, PERF_P_LIMIT_CONTROL_PCU_FUN2_REG, &Size);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &data );
}
