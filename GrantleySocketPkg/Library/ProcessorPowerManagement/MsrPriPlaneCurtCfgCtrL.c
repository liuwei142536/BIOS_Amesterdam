//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/** @file

  Code File for CPU Power Management

  Copyright (c) 2006 - 2013, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  MsrPriPlaneCurtCftCtrl.c

**/

#include "CpuPpmIncludes.h"

#define   Current_Limit 0x1FFF

VOID
ProgMsrPriPlaneCurtCfgCtrL (
    EFI_CPU_PM_STRUCT *ppm
    )
/*++

    Routine Description:

    This routine is called to program MSR PRIMARY_PLANE_CURRENT_CONFIG_CONTROL.

    Arguments:

    PPMPolicy Pointer to PPM Policy protocol instance

    Returns:

    EFI_SUCCESS

--*/
{
  MSR_REGISTER MsrValue;
  UINT8 Psi3CodeVal;
  UINT32 data;
  EFI_CPU_CSR_ACCESS_PROTOCOL *CpuCsrAccess;

  CpuCsrAccess = ppm->CpuCsrAccess;

  //
  // Read PRIMARY_PLANE_CURRENT_CONFIG_CONTROL
  //
  MsrValue.Qword = AsmReadMsr64 (MSR_PRIMARY_PLANE_CURRENT_CONFIG_CONTROL);

  //DEBUG ( (EFI_D_ERROR, ":: Read MSR MSR_PRIMARY_PLANE_CURRENT_CONFIG_CONTROL(0%x) = %x %x \n", MSR_PRIMARY_PLANE_CURRENT_CONFIG_CONTROL, MsrValue.Dwords.High, MsrValue.Dwords.Low ) );
  if ((ppm->Setup->AdvPwrMgtCtlFlags & PCD_CPU_PRI_PLN_CURR_CFG_CTL)== 0) {
    // if here Auto is selected and BIOS will apply normal POR settings and/or WA's
    data = CpuCsrAccess->ReadCpuCsr(0, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG);

    // Check Bit6 of the sticky BIOS Strachpad reg. 0 to see if we have programmed the PRIMARY_PLANE_CURRENT_CONFIG_CONTROL
    // as part of the VA WA's. If bit6 set then just skip changing that code. Thanks. NOTE: We are only need to check the bit on the BSP and apply to all.
    if (!(data & 0x0040)) {
      Psi3CodeVal = 0x00; // value to program into PSI3_CODE, value need to be programmed into CSR & MSR

      MsrValue.Dwords.High &= ~PSI3_CODE; // clear PSI3_CODE (bits 61:59) Note: bits 29:27 in lower dword
      MsrValue.Dwords.High |= (Psi3CodeVal << PSI3_CODE_SHIFT); // Set new value, for now we are setting PSI3_CODE = 000b but this may change later
    }

  } else {
    // If here then user has selected manual configuration of SAPMCTL overriding normal BIOS settings

    MsrValue.Dwords.High &= ~(PSI3_THSHLD + PSI2_THSHLD + PSI1_THSHLD);
    MsrValue.Dwords.Low &= ~(PPCCC_LOCK);

    MsrValue.Dwords.High |= ppm->Setup->PP0CurrentCfg.Dwords.High;


    if (!(ppm->Info->CapId4 & B_OC_ENABLED)){
    MsrValue.Dwords.Low  |=	(ppm->Setup->PP0CurrentCfg.Dwords.Low & PPCCC_LOCK);
	}
	else
	{
    MsrValue.Dwords.Low  |= (ppm->Setup->PP0CurrentCfg.Dwords.Low);
	}

   

    if (ppm->Setup->AdvPwrMgtCtlFlags & PCD_CPU_CURRENT_CONFIG) {
      if ((ppm->Setup->PP0CurrentCfg.Dwords.Low & CURRENT_LIMIT) < Current_Limit) {
        MsrValue.Dwords.Low &= ~(CURRENT_LIMIT);
        MsrValue.Dwords.Low |= (ppm->Setup->PP0CurrentCfg.Dwords.Low & CURRENT_LIMIT);
      }
    }
  }

  //
  // Write PRIMARY_PLANE_CURRENT_CONFIG_CONTROL
  //
	AsmWriteMsr64 (MSR_PRIMARY_PLANE_CURRENT_CONFIG_CONTROL, MsrValue.Qword);
  //save for S3
  WriteRegisterTable(ppm->ProcessorNumber, Msr, MSR_PRIMARY_PLANE_CURRENT_CONFIG_CONTROL, 0, 64, MsrValue.Qword);

}
