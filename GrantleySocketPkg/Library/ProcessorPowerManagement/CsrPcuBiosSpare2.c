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

  Module Name:  CsrPcuBiosSpare2.c

**/

#include "CpuPpmIncludes.h"
#include "PCU_FUN3.h"


VOID
ProgramCsrPcuBiosSpare2 (
  EFI_CPU_PM_STRUCT   *ppm
  )
/*++
  Routine Description:

  This routine is called to program PCU_BIOS_SPARE2. It will be call multiple time passing any the SocketNumber to be programmed.

  Arguments:

  PPMPolicy Pointer to PPM Policy protocol instance

  Returns:

  EFI_SUCCESS

--*/
{

  UINT32    data;
  //
  //4987134 start
  //  
  
  UINTN    RegAddr = 0;
  //
  //4987134 end
  //
  UINT8     Size = 4;
  UINT8     SocketNumber;
  EFI_CPU_CSR_ACCESS_PROTOCOL *CpuCsrAccess;

  SocketNumber = ppm->Info->SocketNumber;
  CpuCsrAccess = ppm->CpuCsrAccess;

  // Read (CSR)
  //PCU_BIOS_SPARE2_PCU_FUN3_REG 
  data = CpuCsrAccess->ReadCpuCsr(SocketNumber, 0, PCU_BIOS_SPARE2_PCU_FUN3_REG);

  data &= ~(PCU_BIOS_SPARE2_MASK);
  if (ppm->Info->dimmTypePresent[SocketNumber] == UDIMM_DIMM) {
	data |= 0 << TYPE_OF_DIMM_SHIFT;
  } else if (ppm->Info->dimmTypePresent[SocketNumber] == RDIMM_DIMM){
	data |= 1 << TYPE_OF_DIMM_SHIFT;
  } else if (ppm->Info->dimmTypePresent[SocketNumber] == LRDIMM_DIMM){
    data |= 2 << TYPE_OF_DIMM_SHIFT;
  }

  if (ppm->Info->ddrVoltage[SocketNumber] == VDD_150) {
    data |= 2 << DIMM_VDD_SHIFT;
  } else if (ppm->Info->ddrVoltage[SocketNumber] == VDD_135){
    data |= 1 << DIMM_VDD_SHIFT;
  } else if (ppm->Info->ddrVoltage[SocketNumber] == VDD_125){
    data |= 0 << DIMM_VDD_SHIFT;
  }

  // Program CSR_PCU_BIOS_SPARE2
  CpuCsrAccess->WriteCpuCsr(SocketNumber, 0, PCU_BIOS_SPARE2_PCU_FUN3_REG, data);  
  RegAddr = CpuCsrAccess->GetCpuCsrAddress(SocketNumber, 0, PCU_BIOS_SPARE2_PCU_FUN3_REG, &Size);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &data );

  return;
}

