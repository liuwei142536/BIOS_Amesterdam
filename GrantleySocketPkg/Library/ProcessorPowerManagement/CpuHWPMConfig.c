//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/** @file

  Code File for CPU Power Management

  Copyright (c) 2006 - 2019, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  CpuHWPMConfig.c

**/

#include "CpuPpmIncludes.h"




/**
	  Enables HWPM Interface feature for specified processor.
	  This function enables of HWPM Interface for specified processor.
	  @param  setup provided user options.
 **/
VOID
HWPMInterfaceReg (
    EFI_CPU_PM_STRUCT   *ppm
)
{
  MSR_REGISTER                 MsrValue;
  UINT32                       CapId5 = ppm->Info->CapId5 ;

  //
  // configure the BDX's HWPM feature in native mode
  //
  if ( (CapId5 & B_HWPM_ENABLE) && (ppm->Setup->Hwpm.HWPMNative) ) {
    //
    // Read MSR_MISC_PWR_MGMT
    //
    MsrValue.Qword = AsmReadMsr64 (MSR_MISC_PWR_MGMT);

    MsrValue.Dwords.Low |= N_MSR_HWPM_NATIVE_INTERFACE_ENABLE;
    //
    //Clear OOB Native if Native mode is being configured.
    //
    MsrValue.Dwords.Low &= ~N_MSR_HWPM_OOB_INTERFACE_ENABLE;

    //
    //Write to 0x1AA if HWPM lock is not set,GP fault occurs if HWP lock is set
    //
    if(!(MsrValue.Dwords.Low & N_MSR_MISC_PWR_MGMT_LOCK)){
      AsmWriteMsr64 (MSR_MISC_PWR_MGMT, MsrValue.Qword);
      //save for S3
      WriteRegisterTable(ppm->ProcessorNumber, Msr, MSR_MISC_PWR_MGMT, 0, 64, MsrValue.Qword);
    }

  }
}

/**
	  Enables HWPM-OOB feature  for specified processor.
	  This function enables of HWPM-OOB  feature  for specified processor.
	  @param  setup provided user options.
 **/
VOID
HWPMOOBModeReg (
    EFI_CPU_PM_STRUCT   *ppm
)
{
  MSR_REGISTER                MsrValue;
  UINT32                      CapId5 = ppm->Info->CapId5;

  //
  // HWPM Interface setup for BDX family processors.
  //
  if ( (CapId5 & B_HWPM_ENABLE) && (ppm->Setup->Hwpm.HWPMOOB) ) {

    //
    // Read MSR_MISC_PWR_MGMT
    //
    MsrValue.Qword = AsmReadMsr64 (MSR_MISC_PWR_MGMT);

    MsrValue.Dwords.Low |= N_MSR_HWPM_OOB_INTERFACE_ENABLE;
    //
    //Clear HWPM Native if OOB mode is being configured.
    //
    MsrValue.Dwords.Low &= ~N_MSR_HWPM_NATIVE_INTERFACE_ENABLE;

    //
    //Write to 0x1AA if HWPM lock is not set,GP fault occurs if hwp lock is set
    //
    if(!(MsrValue.Dwords.Low & N_MSR_MISC_PWR_MGMT_LOCK)){
      AsmWriteMsr64 (MSR_MISC_PWR_MGMT, MsrValue.Qword);
      //save for S3
      WriteRegisterTable(ppm->ProcessorNumber, Msr, MSR_MISC_PWR_MGMT, 0, 64, MsrValue.Qword);
    }
  }
}

/**
	  Enables Autonomous C-state feature for specified processor.
	  This function enables of Autonomous C-state feature for processor each core
	  @param  setup provided user options.
 **/
VOID
HWPMAutonomousCstateModeReg (
    EFI_CPU_PM_STRUCT   *ppm
)
{
  MSR_REGISTER                MsrValue;
  UINT32                      CapId5 = ppm->Info->CapId5;

  //
  // based on processor's capability, enable / disable autonomous C-state for the core
  //
  if(CapId5 & B_ACC_ENABLE){
    //
    // Read MSR_MISC_PWR_MGMT
    //
    MsrValue.Qword = AsmReadMsr64 (MSR_PMG_CST_CONFIG_CONTROL);

    if (ppm->Setup->Hwpm.AutoCState){
      MsrValue.Dwords.Low |=  N_MSR_AUTONOUMOUS_CSTATE_INTERFACE_ENABLE;
    } else {
      MsrValue.Dwords.Low &=  ~N_MSR_AUTONOUMOUS_CSTATE_INTERFACE_ENABLE;
    }

    //
    // HWPM Interface setup for BDX family processors.
    //
    AsmWriteMsr64 (MSR_PMG_CST_CONFIG_CONTROL, MsrValue.Qword);
    //save for S3
    //WriteRegisterTable(ppm->ProcessorNumber, Msr, MSR_PMG_CST_CONFIG_CONTROL, 0, 64, MsrValue.Qword);

  } //capid check
}

//
//MSR_MISC_PWR_MGMT lock-start
//
VOID
CpuPpmMISCPWRMGMTLock (
    EFI_CPU_PM_STRUCT   *ppm
)
/*++
			Routine Description:
			This routine is called to lock MSR_MISC_PWR_MGMT register

			Arguments:
			PPMPolicy Pointer to PPM Policy protocol instance

			Returns:
			VOID

	--*/
{

  MSR_REGISTER                MsrValue;
  UINT32                      CapId5 = ppm->Info->CapId5;

  //
  // Read MSR_MISC_PWR_MGMT
  //
  MsrValue.Qword = AsmReadMsr64 (MSR_MISC_PWR_MGMT);

  //
  // lock the Processor's HWPM configured state
  //
  if(CapId5 & B_HWPM_ENABLE){
    // HWPM lock bit is unset before setting bit,otherwise BIOS will see GP fault.
    if( !(MsrValue.Dwords.Low & N_MSR_MISC_PWR_MGMT_LOCK)){
      MsrValue.Dwords.Low |= N_MSR_MISC_PWR_MGMT_LOCK;

      //
      //Program  LOCK BIT to lock MSR_MISC_PWR_MGMT register
      //

      AsmWriteMsr64 (MSR_MISC_PWR_MGMT, MsrValue.Qword);
      //save for S3,causing S3 issue,WA for now
      // WriteRegisterTable(ppm->ProcessorNumber, Msr, MSR_MISC_PWR_MGMT, 0, 64, MsrValue.Qword);
    }
  }

}
//
//HWPM_END
//

VOLATILE BOOLEAN MutexConfigureHWPMmode = FALSE;

VOID
ConfigureHWPMmode (
    EFI_CPU_PM_STRUCT   *PPM
)
/*++
      Routine Description:
      This routine configures the BDX processor's HWPM mode in native or OOB mode

      Arguments:
      PPMPolicy Pointer to PPM Policy protocol instance

      Returns:
      none

  --*/
{
  VOLATILE BOOLEAN  *Semaphore = &MutexConfigureHWPMmode;

  while( *Semaphore == TRUE) {  //spin to wait on mutex
    CpuPause();
  }
  *Semaphore = TRUE;            //acquire mutex

  if(PPM->Info->CpuType == CPU_BDX_DE_SERVER || PPM->Info->CpuType== CPU_BDX_EP_SERVER) {
    //
    //Configure the HWPM mode based on user / platform's option
    //
    if ( PPM->Setup->Hwpm.HWPMNative ) {
      HWPMInterfaceReg( PPM);
    } else if ( PPM->Setup->Hwpm.HWPMOOB ) {
      HWPMOOBModeReg( PPM);
    }
    //
    // lock the processor's HWPM mode
    //
    if( PPM->Info->CpuStepping >= C0_REV_BDX ) {
      CpuPpmMISCPWRMGMTLock( PPM);
    }
  }

  *Semaphore = FALSE;         //release mutex for other thread
}

