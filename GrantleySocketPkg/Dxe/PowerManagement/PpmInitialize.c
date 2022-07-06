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

  Module Name:  PpmInitialize.c

**/

#include "PpmInitialize.h"
#include "CpuPpmIncludes.h"
#include <Library/OemCpuInit.h>

BOOLEAN                           PreBootCallbackComplete = FALSE;
EFI_CPU_PM_STRUCT                 mPpmLib ={ 0, 0, 0, 0 };


EFI_STATUS
ProgramMiscPowerThermalMsrsOnAllThreads(
    VOID
)
{
  EFI_STATUS    Status = \
      //
      // program the product specific power/thermal MSRs to its initial expected state
      //
      mMpService->StartupAllAPs(mMpService,
          CpuPmProgramMsrs,
          TRUE,
          NULL,
          0,
          (VOID *)&mPpmLib,
          NULL
      );

  CpuPmProgramMsrs((EFI_CPU_PM_STRUCT *)&mPpmLib);

  return Status;
}

EFI_STATUS
AlignPStatesOnAllThreads (
    VOID
)
{
  EFI_STATUS    Status = EFI_SUCCESS;

  //allow APs switch to max non-turbo ratio if user choose
  Status = mMpService->StartupAllAPs(
            mMpService,
            PStateTransition,
            TRUE,
            NULL,
            0,
            (VOID *)&mPpmLib,
            NULL
            );
  //allow SBSP switch to max non-turbo ratio if user choose
  PStateTransition((EFI_CPU_PM_STRUCT *)&mPpmLib);

  return Status;
}

EFI_STATUS
SignalPcuToReadTurboLimits (
    VOID
    )
{
  EFI_STATUS    Status = \

  //set BIOS_PCU_SEMAPHORE if OC part is detected
  mMpService->StartupAllAPs(
                mMpService,
                PpmSetTurboLimitPcuSemaphore,
                TRUE,
                NULL,
                0,
                (VOID *)&mPpmLib,
                NULL
              );

  PpmSetTurboLimitPcuSemaphore((EFI_CPU_PM_STRUCT *)&mPpmLib);
  return Status;
}

EFI_STATUS
LockUpMiscPwrCfgOnAllThreads (
    VOID
)
{
  EFI_STATUS    Status = \
      mMpService->StartupAllAPs(mMpService,
          PpmSetMsrLockBit,
          TRUE,
          NULL,
          0,
          (VOID *)&mPpmLib,
          NULL
      );

  //BSP do lock
  PpmSetMsrLockBit((EFI_CPU_PM_STRUCT *)&mPpmLib);

  return Status;
}

EFI_STATUS
LockHWPMConfig (
    VOID
)
{
  EFI_STATUS    Status = EFI_SUCCESS;

  //
  //Si WA283389:Ucode incorrectly signals GP during WRMSR with lock bit set in MISC_PWR_MGM
  //With MISC_PWR_MGMT[HWPM_LOCK]=1,pcode signals GP if user tries to write the locked bits bit no 0,6,8,13 with the same values as present before the write.
  //
  if(((mPpmLib.Info->CpuType== CPU_BDX_EP_SERVER) && (mPpmLib.Info->CpuStepping >= A0_REV_BDX)) ||
      (( mPpmLib.Info->CpuType == CPU_BDX_DE_SERVER) && (mPpmLib.Info->CpuStepping >= C0_REV_BDX))){
    //BSP first to set lock bit
    CpuPpmMISCPWRMGMTLock((EFI_CPU_PM_STRUCT *)&mPpmLib);
    //set on non-SBSP only in case of multi-socket system
    if (mPpmInfo->SocketPresentBitMap & 0x0E) {
      Status = mMpService->StartupAllAPs(
                mMpService,
                CpuPpmMISCPWRMGMTLock,
                TRUE,
                NULL,
                0,
                (VOID *)&mPpmLib,
                NULL
              );
    }
  }//hwpm lock end
  //
  //Si WA # 283389:Ucode incorrectly signals GP during WRMSR with lock bit set in MISC_PWR_MGM
  //

  return Status;
}

EFI_STATUS
ConfigureAutonomousCStateOnAllCores (
    VOID
)
{
  EFI_STATUS    Status = EFI_SUCCESS;

  if(mPpmLib.Info->CpuType == CPU_BDX_DE_SERVER || mPpmLib.Info->CpuType== CPU_BDX_EP_SERVER) {
    //
    //Enable ACC if CPU supports +setup option enabled
    //
    Status =  \
    mMpService->StartupAllAPs(mMpService,
                              HWPMAutonomousCstateModeReg,
                              TRUE,
                              NULL,
                              0,
                              (VOID *)&mPpmLib,
                              NULL
                              );

    //BSP to enable ACC
    HWPMAutonomousCstateModeReg(&mPpmLib);
  }//ACC end

  return Status;
}

EFI_STATUS
ConfigureAutonomousPStateOnAllPackages (
    VOID
)
{
  EFI_STATUS    Status = EFI_SUCCESS;

  if(mPpmLib.Info->CpuType == CPU_BDX_DE_SERVER || mPpmLib.Info->CpuType== CPU_BDX_EP_SERVER) {
    //
    //set on non-SBSP only in case of multi-socket system
    //
    if (mPpmInfo->SocketPresentBitMap & 0x0E) {
      Status = mMpService->StartupAllAPs(
                mMpService,
                ConfigureHWPMmode,
                TRUE,
                NULL,
                0,
                (VOID *)&mPpmLib,
                NULL
              );
    }
    //
    //configure HWPM on SBSP
    //
    ConfigureHWPMmode( &mPpmLib);
  }

  return Status;
}

EFI_STATUS
CpuPpmInitDone (
    VOID
    )
/*++

    Routine Description:

    Set BIOS init done to enable PPM features.

    Arguments:

    PPMPolicy Pointer to PPM Policy protocol instance

    Returns:

    EFI_SUCCESS
--*/
{
  UINT8       SocketNumber;
  EFI_STATUS  Status = EFI_SUCCESS;

  //
  // signal PCU to read the Turbo limits set by BIOS
  //
  Status = SignalPcuToReadTurboLimits();
  if ( EFI_ERROR(Status)) {
    DEBUG ((EFI_D_INFO, "WARNING!!! - @PpmInitialize:- signal PCU to read the Turbo limits settings on AP/threads [err_code:%x]!", Status));
  }

  //
  // configure the HW-managed P-states on all packages of BDX processor family
  //
  Status = ConfigureAutonomousPStateOnAllPackages();
  if ( EFI_ERROR(Status)) {
    DEBUG ((EFI_D_INFO, "WARNING!!! - @PpmInitialize:- HWPM config settings on AP/threads [err_code:%x]!", Status));
  }

  //
  // lock up miscellaneous power related configuration MSRs on all threads
  //
  Status = LockUpMiscPwrCfgOnAllThreads();
  if ( EFI_ERROR(Status)) {
    DEBUG ((EFI_D_INFO, "WARNING!!! - @PpmInitialize:- misc pwr cfg settings on AP/threads [err_code:%x]!", Status));
  }
  //
  // configure the Uncore side of processor to signal the power management settings complete
  //
  for (SocketNumber = 0; SocketNumber < MAX_SOCKET; SocketNumber++) {
    if (mPpmInfo->SocketPresentBitMap & (1 << SocketNumber)){
      mPpmLib.Info->SocketNumber = SocketNumber;
      //
      // Set CSR lock bits on all processors.
      //
      PpmSetCsrLockBit((EFI_CPU_PM_STRUCT *)&mPpmLib);

      //
      // Set BIOS Init Done on all processors.
      //
      PpmSetBiosInitDone((EFI_CPU_PM_STRUCT *)&mPpmLib);
    }
  }
  //
  // enable/disable autonomous C-state on all cores of BDX processor family
  // this code should not be moved above power management settings configuration as the same have depenedency on C-state
  //
  Status = ConfigureAutonomousCStateOnAllCores();
  if ( EFI_ERROR(Status)) {
    DEBUG ((EFI_D_INFO, "WARNING!!! - @PpmInitialize:- ACC settings on AP/threads [err_code:%x]!", Status));
  }

  return EFI_SUCCESS;
}
  
EFI_STATUS
EFIAPI
PpmInitializeEntry (
  IN EFI_HANDLE                 ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable
  )
/*++

    Routine Description:

    Call back routine used to do the Power Management related Register Programming

    Arguments:

    Event - PpmProcessorSupportProtocol Install

    Returns:

    Status
    EFI_SUCCESS
--*/

{
  EFI_STATUS                Status;
  EFI_EVENT                 Event;
  VOID                      *Registration;

  Status = EFI_SUCCESS;

  InitializeCpuInfoStruct();
  InitializeCpuPPMLib ((EFI_CPU_PM_STRUCT *)&mPpmLib);

  CpuPmProgramCsrs();

  Status = ProgramMiscPowerThermalMsrsOnAllThreads();
  if ( EFI_ERROR(Status)) {
    DEBUG ((EFI_D_INFO, "WARNING!!! - @PpmInitialize:- initial power/thermal plane settings on AP/threads [err_code:%x]!", Status));
  }

  OemCpuOtherChipThermalInit(&mPpmLib);

  //
  // align all the thread's P-state as per its package's min/max ratio
  //
  Status = AlignPStatesOnAllThreads();
  if ( EFI_ERROR(Status)) {
    DEBUG ((EFI_D_INFO, "WARNING!!! - @PpmInitialize:- P-state alignment settings on AP/threads [err_code:%x]!", Status));
  }

  Event = EfiCreateProtocolNotifyEvent (
            &gExitPmAuthProtocolGuid,
            TPL_CALLBACK,
            PpmOnReadyToBoot,
            NULL,
            &Registration
           );
  ASSERT ( Event );

  return Status;
}

VOID
EFIAPI
PpmOnReadyToBoot (
  IN      EFI_EVENT                 Event,
  IN      VOID                      *Context
  )
{
  EFI_STATUS Status;
  VOID       *ProtocolPointer;

  if( PreBootCallbackComplete == TRUE) {
    DEBUG ((EFI_D_INFO, "WARNING!!! - @PpmInitialize: pre-boot callback re-entry...aborted!"));
    return;
  }

  Status = gBS->LocateProtocol (&gExitPmAuthProtocolGuid, NULL, &ProtocolPointer);
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = CpuPpmInitDone();

  PreBootCallbackComplete = TRUE;   //protect against code re-entry...
  return;
}
//End of OnReadyToBoot


#ifdef DE_SKU
VOID
ProgramCsrPchThermControl (
		EFI_CPU_PM_STRUCT *ppm
		)
/*++

		Routine Description:

		This routine is called to program CSR_PCH_TEMP Config. It will be called multiple times by passing any of the SocketNumber to be programmed.

		Arguments:

		PPMPolicy Pointer to PPM Policy protocol instance

		Returns:

		EFI_SUCCESS

--*/
{
  PCH_TEMP_PCU_FUN0_STRUCT pchTempPcuReg;
  UINTN RegAddr = 0; 
  UINT8  Size = 4;
  UINT8  SocketNumber;
  EFI_CPU_CSR_ACCESS_PROTOCOL *CpuCsrAccess;
  UINT64 MsrValue;

  SocketNumber = ppm->Info->SocketNumber;
  CpuCsrAccess = ppm->CpuCsrAccess;
  
  // Read (CSR)
  pchTempPcuReg.Data = CpuCsrAccess->ReadCpuCsr(SocketNumber, 0, PCH_TEMP_PCU_FUN0_REG);

  //Program CSR, PCH_TEMP_PCU_FUN0_REG
  pchTempPcuReg.Bits.use_pch_hot = ppm->Setup->UsePchHot;
  pchTempPcuReg.Bits.use_pch_temp_from_me = ppm->Setup->UsePchTempFromME;
  pchTempPcuReg.Bits.allow_pch_throttle = ppm->Setup->CpuToPchThrottle;

  //PCH Tcontrol Offset is calculated by reading Temp Control Offset from MSR_TEMPERATURE_TARGET (0x1A2) and converting it to 9 bit value.
  MsrValue = AsmReadMsr64 (MSR_TEMPERATURE_TARGET);
  MsrValue &= 	B_MSR_TEMPERATURE_TARGET_FAN_TEMP_TARGET_OFFSET;
  MsrValue = (UINT8) RShiftU64 (MsrValue, N_MSR_TEMPERATURE_TARGET_FAN_TEMP_TARGET_OFFSET);
  pchTempPcuReg.Bits.t_control_offset = (UINT32)((MsrValue + 50) * 2);
  
  CpuCsrAccess->WriteCpuCsr(SocketNumber, 0, PCH_TEMP_PCU_FUN0_REG, pchTempPcuReg.Data);
  RegAddr = CpuCsrAccess->GetCpuCsrAddress(SocketNumber, 0, PCH_TEMP_PCU_FUN0_REG, &Size);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &pchTempPcuReg.Data ); 

  return;
}
#endif  //#ifdef DE_SKU

VOLATILE BOOLEAN MutexCpuPmProgramMsrs = FALSE;

VOID CpuPmProgramMsrs (
  EFI_CPU_PM_STRUCT *ppm
  )
{
  UINTN    ProcessorNumber = 0;

  VOLATILE BOOLEAN  *Semaphore = &MutexCpuPmProgramMsrs;

  while( *Semaphore == TRUE) {  //spin to wait on mutex
    CpuPause();
  }
  *Semaphore = TRUE;            //acquire mutex

  mMpService->WhoAmI(mMpService, &ProcessorNumber);
  ppm->ProcessorNumber = ProcessorNumber;
  ProgramMsrPowerCtl (ppm);
  ProgMsrPriPlaneCurtCfgCtrL (ppm);
  ProgramMsrTurboPowerLimit (ppm);
  ProgramMsrTurboRatioLimit(ppm);
  ProgramEnergyPerfBiasConfigMsr(ppm);
   //
   //BIOS HSD 4988172:We don't need these knobs as these registers are Write only.
   //Recommendation: work arounds those knobs until bug Silicon HSD 281641 is fixed
   //

#ifdef DE_SKU
#endif    //#ifdef DE_SKU
   
  
//HWPM-start
  //
  //  [GNV-NS] HWPM mode configuration is moved to pre-boot callback to align with ACC and HWPM lock
  //  there should be no-dependency on HWPM mode setting this early...
  //
//HWPM-end

  if(mPpmLib.Info->CpuType == CPU_HASWELLSERVER || mPpmLib.Info->CpuType == CPU_BDX_DE_SERVER || mPpmLib.Info->CpuType== CPU_BDX_EP_SERVER ) { 
    ProgramMsrTemperatureTarget(ppm); 
    ProgramMsrMiscPwrMgmt(ppm); 

  }

  *Semaphore = FALSE;            //release mutex
  return;

}

VOID 
CpuPmProgramCsrs( VOID )
{

  UINT8  SocketNumber;

  for (SocketNumber = 0; SocketNumber < MAX_SOCKET; SocketNumber++) {
    if (mPpmInfo->SocketPresentBitMap & (1 << SocketNumber)){
      mPpmLib.Info->SocketNumber = SocketNumber;

      ProgramCSRTurboPowerLimit((EFI_CPU_PM_STRUCT *)&mPpmLib);
      ProgramCsrPkgCstEntryCriteriaMask((EFI_CPU_PM_STRUCT *)&mPpmLib);
      //ProgramCsrPcuBiosSpare2((EFI_CPU_PM_STRUCT *)&mPpmLib);  //take care in MPM (MemThrot.c)
      ProgramCsrPerfPlimitControl((EFI_CPU_PM_STRUCT *)&mPpmLib);
      ProgramCsrDynamicPerfPowerCtl((EFI_CPU_PM_STRUCT *)&mPpmLib);
      //ProgramCsrIoBandwidthPLimitControl((EFI_CPU_PM_STRUCT *)&mPpmLib);
      ProgramCsrSapmCtl((EFI_CPU_PM_STRUCT *)&mPpmLib);
      ProgramCsrSwLtrOvrd((EFI_CPU_PM_STRUCT *)&mPpmLib);

      if (mPpmLib.Info->CpuType == CPU_HASWELLSERVER || mPpmLib.Info->CpuType == CPU_BDX_DE_SERVER || mPpmLib.Info->CpuType == CPU_BDX_EP_SERVER) {   
        ProgramB2PFastRaplDutyCycle((EFI_CPU_PM_STRUCT *)&mPpmLib);
      }

#ifdef DE_SKU
      // Program PCH_TEMP_PCU_FUN0_REG if it is BDX-DE sku
      if(mPpmLib.Info->CpuType == CPU_BDX_DE_SERVER) {
        ProgramCsrPchThermControl((EFI_CPU_PM_STRUCT *)&mPpmLib);
      }
#endif  //#ifdef DE_SKU

      ProgramB2PPcuMiscConfig((EFI_CPU_PM_STRUCT *)&mPpmLib);


    }
  }
}


