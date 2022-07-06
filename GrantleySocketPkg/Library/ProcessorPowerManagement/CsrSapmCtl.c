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

  Module Name:  CsrSapmCtl.c

**/

#include "CpuPpmIncludes.h"

VOID
ProgramCsrSapmCtl (
  EFI_CPU_PM_STRUCT   *ppm
    )
/*++

    Routine Description:

    This routine is called to program SAPMCTL. It will be call multiple time passing any the SocketNumber to be programmed.

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
  UINT8    SocketNumber;
  EFI_CPU_CSR_ACCESS_PROTOCOL *CpuCsrAccess;


  SocketNumber = ppm->Info->SocketNumber;
  CpuCsrAccess = ppm->CpuCsrAccess;

   // Read (CSR)
  data = CpuCsrAccess->ReadCpuCsr(SocketNumber, 0, SAPMCTL_PCU_FUN1_REG);

  if (ppm->Setup->AdvPwrMgtCtlFlags & PCD_CPU_SAPM_CTL_VAL_CTL) {
    data &= ~(QPI2_PKGC_CLOCK_GATE_DISABLE \
      + QPI01_PKGC_CLOCK_GATE_DISABLE \
      + IIO_PKGC_CLOCK_GATE_DISABLE \
      + MDLL_ON_DE \
      + MPLL_ON_DE \
      + NSWAKE_SREXIT \
      + FORCE_PPLL_OFF \
      + PPLL_OFF_ENA \
      + MPLL_OFF_ENA
      );

  // If here then user has selected manual configuration of SAPMCTL overriding normal BIOS settings
     data |= ppm->Setup->SapmCtl;
  }
  else {
  // if here Auto is selected and BIOS will apply normal POR settings and/or WA's
  }
  
    // Override "mpll_off_ena" value in CRB build, since there is no "mpll_off_ena" setup control in CRB
    data &= ~MPLL_OFF_ENA;

  //B-0 needs to force these bits to "1" to disable power gating
    if((ppm->Info->CpuType == CPU_HASWELLSERVER) && (ppm->Info->CpuStepping < C0_REV_HSX) ) {
      data |= (IIO_PKGC_CLOCK_GATE_DISABLE+ QPI01_PKGC_CLOCK_GATE_DISABLE + QPI2_PKGC_CLOCK_GATE_DISABLE + \
               MDLL_ON_DE + MPLL_ON_DE + MPLL_OFF_ENA);
  }


    //4168850: CLONE from HSX: Naitive mode DDR in 2:2 mode clock resync doesn't work for both Pkgc6 and mc parity error. - Update working for < HSX C stepping only.
    if((ppm->Info->CpuType == CPU_HASWELLSERVER) && (ppm->Info->CpuStepping < C0_REV_HSX) ) {
        // In the "Auto" mode, BIOS is to Disable (set sapmctl.mpll_off_en = 0) if 2:2 mode is detected. 
        // Otherwise, if 1:1 mode is detected, BIOS is to leave Enabled (set sapmctl.mpll_off_en = 1).
          if ((ppm->Info->DDRXOVERMODE == XOVER_MODE_2TO2) && (ppm->Setup->AdvPwrMgtCtlFlags & MPLL_OFF_ENA_AUTO)) {
        data &= ~MPLL_OFF_ENA;
          }
    }
  //BIOS HSD# 5001808 
	
        // In the "Auto" mode , set sapmctl.mpll_off_en = 1 for BDX ML and BDx DE  =0
	
        if ((ppm->Setup->AdvPwrMgtCtlFlags & MPLL_OFF_ENA_AUTO)) {

			
				if((ppm->Info->CpuType == CPU_BDX_DE_SERVER) && (ppm->Info->CpuStepping >= C0_REV_BDX)){
					data |= MPLL_OFF_ENA;
				}else if((ppm->Info->CpuType == CPU_BDX_EP_SERVER)){

					data |= MPLL_OFF_ENA;
				}

		
          }

  CpuCsrAccess->WriteCpuCsr(SocketNumber, 0, SAPMCTL_PCU_FUN1_REG, data);
  RegAddr = CpuCsrAccess->GetCpuCsrAddress(SocketNumber, 0, SAPMCTL_PCU_FUN1_REG, &Size);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &data );
}


