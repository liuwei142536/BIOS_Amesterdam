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

  Module Name:  CsrSwLtrOvrd.c

**/

#include "CpuPpmIncludes.h"

VOID
ProgramCsrSwLtrOvrd (
  EFI_CPU_PM_STRUCT   *ppm
		)
/*++

		Routine Description:

		This routine is called to program Softwre LTR OVerride. It will be call multiple time passing any the SocketNumber to be programmed.

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

  // Read C (CSR)
  data = CpuCsrAccess->ReadCpuCsr(SocketNumber, 0, PCIE_ILTR_OVRD_PCU_FUN1_REG);

  //
  // Read SW_LTR_OVRD
  //

  if((ppm->Setup->AdvPwrMgtCtlFlags & PCD_CPU_SW_LTR_OVRD_CTL ) == 0) {
	  // if here Auto is selected and BIOS will apply normal POR settings and/or WA's
  }else{
	// If here then user has selected manual configuration of SAPMCTL overriding normal BIOS settings
	
	data &= ~SNOOP_LATENCY_VLD_MASK ;
	data &= ~FORCE_SNOOP_OVRD_MASK ;
	data &= ~SNOOP_LATENCY_MUL_MASK ;
	data &= ~SNOOP_LATENCY_Value_MASK ;
	data &= ~NON_SNOOP_LATENCY_VLD_MASK ;
	data &= ~FORCE_NON_SNOOP_OVRD_MASK ;
	data &= ~NON_SNOOP_LATENCY_MUL_MASK ;
	data &= ~NON_SNOOP_LATENCY_Value_MASK ;
	data |= ppm->Setup->PcieIltrOvrd;
  }

  //
  // Write SW_LTR_OVRD
  //
  
  CpuCsrAccess->WriteCpuCsr(SocketNumber, 0, PCIE_ILTR_OVRD_PCU_FUN1_REG, data);   
  RegAddr = CpuCsrAccess->GetCpuCsrAddress(SocketNumber, 0, PCIE_ILTR_OVRD_PCU_FUN1_REG, &Size);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &data );
  DEBUG ( (EFI_D_ERROR, ":: Wrote Socket = %X, CSR_PCIE_ILTR_OVRD=%X \n\n", SocketNumber, data) );
  
  return;
}
