/*++
This file contains an 'Intel Peripheral Driver' and is
licensed for Intel CPUs and chipsets under the terms of your
license agreement with Intel or your vendor. This file may
be modified by the user, subject to additional terms of the
license agreement
--*/
/** @file

Code for Enhanced Intel Speedstep Technology

Copyright (c) 2006 - 2013, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name: PCPS.c

**/

#include "CpuPpmIncludes.h"


VOID
ProgramB2PPcuMiscConfig (
    EFI_CPU_PM_STRUCT *ppm
  )
/*++

		Routine Description:

		This routine is called to program PCU_MISC_CONFIG. It will be call multiple time passing any the SocketNumber to be programmed.

		Arguments:

		PPMPolicy Pointer to PPM Policy protocol instance

		Returns:

		EFI_SUCCESS

--*/
{
  UINT32 PM_MBX_CMD = 0;
  UINT32 PM_MBX_DATA = 0;
  UINT32 data32 = 0;
  UINT32 Tempdata = 0;
  UINT8   SocketNumber;
  EFI_CPU_CSR_ACCESS_PROTOCOL *CpuCsrAccess;

  SocketNumber = ppm->Info->SocketNumber;
  CpuCsrAccess = ppm->CpuCsrAccess;
  data32 = CpuCsrAccess->ReadCpuCsr(SocketNumber, 0, BIOS_MAILBOX_DATA_PCU_FUN1_REG);

  SocketNumber = ppm->Info->SocketNumber;

  if ( ((ppm->Info->CapId4 & B_PCPS_DISABLE) == 0) && ((ppm->Setup->PcpsCtrl & PCD_CPU_PCPS_SPD) != 0) ) {

    PM_MBX_DATA = 0; 
    PM_MBX_CMD = (UINT32) MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG;
    Tempdata = ppm->CpuCsrAccess->Bios2PcodeMailBoxWrite(SocketNumber, PM_MBX_CMD, PM_MBX_DATA);
    if ((Tempdata & 0x000000ff) != PPM_MAILBOX_BIOS_ERROR_CODE_INVALID_COMMAND) {
      data32 = CpuCsrAccess->ReadCpuCsr(SocketNumber, 0, BIOS_MAILBOX_DATA_PCU_FUN1_REG);
      data32 |= PCU_MISC_CONFIG_SPD_ENABLE;

      PM_MBX_CMD = (UINT32) MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG;
      PM_MBX_DATA |= data32; 

      Tempdata = ppm->CpuCsrAccess->Bios2PcodeMailBoxWrite(SocketNumber, PM_MBX_CMD, PM_MBX_DATA);
//      DEBUG ( (EFI_D_ERROR, ":: Write MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG, Tempdata = %x\n", data32) );
      if ((Tempdata & 0x000000ff) != PPM_MAILBOX_BIOS_ERROR_CODE_INVALID_COMMAND) {
        DEBUG ( (EFI_D_ERROR, "\n Set SPD in PCU_MISC_CONFIG, data32 = %x, SETUP PcpsCtrl = %d\n", data32, ppm->Setup->PcpsCtrl) );
      }
    }

    PM_MBX_DATA = 0; 
    PM_MBX_CMD = (UINT32) MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG;
    Tempdata = ppm->CpuCsrAccess->Bios2PcodeMailBoxWrite(SocketNumber, PM_MBX_CMD, PM_MBX_DATA);
    if ((Tempdata & 0x000000ff) != PPM_MAILBOX_BIOS_ERROR_CODE_INVALID_COMMAND) {
      data32 = CpuCsrAccess->ReadCpuCsr(SocketNumber, 0, BIOS_MAILBOX_DATA_PCU_FUN1_REG);
      DEBUG ( (EFI_D_ERROR, "\n Read MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG back = %x\n", data32) );
    }
  }

}



VOID
ProgramMsrMiscPwrMgmt (
   EFI_CPU_PM_STRUCT *ppm
  )
/*++

Routine Description:

  This routine is called to program MSR_MISC_PWR_MGMT.

Arguments:

  PPMPolicy      Pointer to PPM Policy protocol instance

Returns:

  EFI_SUCCESS

--*/

{
  MSR_REGISTER  MsrData;
    
  //
  // Program MSR_MISC_PWR_MGMT
  //
  MsrData.Qword = AsmReadMsr64 (MSR_MISC_PWR_MGMT);
  MsrData.Dwords.Low &= (MsrData.Dwords.Low & ~(B_SINGLE_PCTL_EN)) ;
 
  if (ppm->Setup->PcpsCtrl & PCD_CPU_PCPS_SINGLEPCTL) { 
    MsrData.Dwords.Low |= B_SINGLE_PCTL_EN;
  }
  //
  //Write to 0x1AA if HWPM lock is not set,GP fault occurs if hwp lock is set 
  //
   if(!(MsrData.Dwords.Low & N_MSR_MISC_PWR_MGMT_LOCK)){
    AsmWriteMsr64 (MSR_MISC_PWR_MGMT, MsrData.Qword);
   }
  return;
}

