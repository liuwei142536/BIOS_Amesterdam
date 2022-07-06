/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c) 2007 - 2016 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  ProcessorErrorHandler.c

Abstract:

  Processor based component hooks for event logging.

--*/
#include "CommonErrorHandlerIncludes.h"


UINT32      mEccErrorSmiCount;
TIME_STAMP  mCorrectedErrorRankTimeStamp[MC_MAX_NODE][MAX_CH][MAX_RANK_CH];

static EFI_SMM_BASE2_PROTOCOL *mSmmBase;
EFI_SMM_SYSTEM_TABLE2         *mSmst;
SYSTEM_CONFIGURATION          mSystemConfiguration;
BOOLEAN                       mWheaFfmCorErrLogging = FALSE;
BOOLEAN                       mWheaFfmUnCorErrLogging = FALSE;
UINT8                         mSpareIntSelect                   = 0; 
ERROR_LOG_SETUP_DATA      *mErrLogSetupData;
SPIN_LOCK                               mApLock;
// Aptio V Server Override Start:  Changes done to support LastBoot Error Handling and Logging Uncorrectable Errors (Memory/PCIE ...)
extern BOOLEAN mLastbootMemErrFlag;

extern 
EFI_STATUS
EFIAPI 
ReadMcBankSignature(
  IN UINT32 McBankIdx,
  IN UINT8 skt,    
  OUT EMCA_MC_SIGNATURE_ERR_RECORD *McSig
);

// Aptio V Server Override End:  Changes done to support LastBoot Error Handling and Logging Uncorrectable Errors (Memory/PCIE ...)
// This flag control if any other functions started Whea Logging or not. If this is true, we don't need to call whea restart function.
BOOLEAN	mWheaLogStarted	= FALSE;
// HA Mcbank addresses
// AptioV Server Override Start: Commented the unused variables.
//UINT16	mHaMcBankAddress[MAX_HA] = { 0x41c, 0x420};
//UINT16	mIMCbankStartAddress	 = 0x424; 
//UINT16	mIMCbankEndAddress		 = 0x440;
// AptioV Server Override End: Commented the unused variables.

UINT32
GetApicID (
  VOID
  )
/*++

Routine Description:
    Returns the apic ID  of the thread executing the code

Arguments:

  none
Returns:

    status - EFI_SUCCESS - no error
             non-zero code - if error
    IsEventHandled - Event handled status - TRUE/ FALSE

--*/
{
  UINT32  ApicId;
  UINT32  RegEax;

  AsmCpuid (EFI_CPUID_SIGNATURE, &RegEax, NULL, NULL, NULL);
  if (RegEax >= 0xB) {
    //
    // Is X2Apic capable.
    // Get ApicId from cpuid leaf B.
    //
    AsmCpuidEx (EFI_CPUID_CORE_TOPOLOGY, 0, NULL, NULL, NULL, &ApicId);
  } else {
    //
    // Get ApicId from cpuid leaf 1.
    //
    AsmCpuid (EFI_CPUID_VERSION_INFO, NULL, &ApicId, NULL, NULL);
    ApicId = ((ApicId >> 24) & 0xFF);
  }

  return ApicId;
}

VOID
ExtractNodeCoreFromApicId (
  OUT UINT8 *Socket,
  OUT UINT8 *Core
  )
{
  //
  // the APIC ID is composed of the Socket ID
  // and a combination of the Core ID and Thread ID, as illustrated
  // below:
  //
  //  ----------- -------------------
  // |  Bit 7-n  |      Bit n-0      |
  //  ----------- -------------------
  // | Socket ID | Core ID/Thread ID |
  //  ----------- -------------------
  // Bit "n" is Socket specific (defined via CPUID)
  //

  UINT8 coreShift;
  UINT8 packageShift;
  UINT8 coreTemp;
  UINT8 coreMask;

  UINT32 RegEax;
  UINT32 RegEbx;
  UINT32 RegEcx;
  UINT32 RegEdx;

  RegEax = 0;
  RegEbx = 0;
  RegEcx = 0;
  RegEdx = 0;

  //
  // Execute CPUID Leaf 0Bh, with ECX=0 (thread level), then
  // EAX[4:0]  = Number of bits to right-shift APIC ID to get CoreID
  // EDX[31:0] = Extended APIC ID -- Lower 8 bits identical to the legacy APIC ID
  //

  RegEax = CPUID_TOPO_INFO;
  RegEcx = 0;

  AsmCpuidEx (RegEax, RegEcx, &RegEax, &RegEbx, &RegEcx, &RegEdx);

  coreShift = (UINT8)(RegEax & 0x1f);
  coreTemp = (UINT8)((RegEdx & 0xff ) >> coreShift);

  //
  // Execute CPUID Leaf 0Bh, with ECX=1 (Core level), then
  // EAX[4:0]  = Number of bits to right-shift APIC ID to get SocketID (i.e., Packge-leval ID)
  // EDX[31:0] = Extended APIC ID -- Lower 8 bits identical to the legacy APIC ID
  //

  RegEax = CPUID_TOPO_INFO;
  RegEcx = 1;

  AsmCpuidEx (RegEax, RegEcx, &RegEax, &RegEbx, &RegEcx, &RegEdx);

  packageShift = (UINT8)RegEax & 0x1f;
  *Socket = (UINT8)(((UINT8)(RegEdx & 0xff )) >> packageShift);

  //
  // Mask package Id to get Core Id
  //
  coreMask = (1 << (packageShift - coreShift)) - 1;
  *Core = (UINT8) (coreTemp & coreMask);
  return;
}

UINTN
GetCpuNumber (
  UINT8 Socket,
  UINT8 Core,
  UINT8 Thread)
{
  if ((Socket < MAX_SOCKET) && 
      (Core < MAXCOREID)     && 
      (Thread < MAX_THREAD) &&
      (mELogHost.cpuInfo[Socket].Valid[Core][Thread])
      ) {
    return mELogHost.cpuInfo[Socket].cpuNumber[Core][Thread];
  } else {
    return (UINTN)-1;
  }
}


BOOLEAN
ExcutingOnLocalNode (
  UINT8 Socket
  )
{
  UINT8 localSocket = 0;
  UINT8 localCore = 0;

  ExtractNodeCoreFromApicId (&localSocket, &localCore);

  return (BOOLEAN)(Socket == localSocket);
}


BOOLEAN
ExcutingOnLocalNodeCore (
  UINT8 Socket,
  UINT8 Core
  )
{
  UINT8 localSocket = 0;
  UINT8 localCore = 0;

  ExtractNodeCoreFromApicId (&localSocket, &localCore);

  if ((Socket == localSocket) &&
      (Core == localCore)) {
    return TRUE;
  } else {
    return FALSE;
  }
}


EFI_STATUS
FindNodeBSPApicIdOnApSocket (
  IN  UINT8   Socket,
  OUT UINT32  *NBspApicId
  )
{
  UINT32 ApicId;

  // 
  // Return ApciId for Socket (core 0)
  //

  ApicId = mELogHost.cpuInfo[Socket].ApicId[0][0];

  if (ApicId != 0) {
    *NBspApicId = ApicId;
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_FOUND;
  }
}

VOID
ClearPCUErrors(UINT8 Socket)
{
#if 0 // 4988855: System loops in BIOS after cpu went into unbreakable shutdown
      //  [clone from b286903] -  do not clear the  PCU MCA error source status
      //                          before RST_CPL3

  MCA_ERR_SRC_LOG_PCU_FUN2_HSX_BDX_STRUCT pcuMCAErrSrcLogReg;

  //
  // Clear PCU uncorrected error status
  //
  pcuMCAErrSrcLogReg.Data = 0;
  if (mMemRas != NULL)
    mMemRas->MemWritePciCfg(   Socket, 0, MCA_ERR_SRC_LOG_PCU_FUN2_REG, pcuMCAErrSrcLogReg.Data);
  else
    ASSERT_EFI_ERROR (0);
#endif

}

VOID
ClearPCUErrorsEp()
{
  MCA_ERR_SRC_LOG_PCU_FUN2_HSX_BDX_STRUCT pcuMCAErrSrcLogReg;
  UINT8 Socket;

  if (mMemRas != NULL){
    for(Socket=0; Socket < MAX_CPU_SOCKETS; Socket++) {
      if(mIioUds->PlatformData.CpuQpiInfo[Socket].Valid == TRUE) {
        //
        // 4988861: Clone from BDX Sighting:EMCA Mode: System loops in BIOS after cpu
        //          went into unbreakable shutdown
        //  [cloned from Si s5287734] - clear all PCU MCA error source status
        //                              only if its not MSMI IERR
        //
        pcuMCAErrSrcLogReg.Data = mMemRas->MemReadPciCfg(Socket, 0, MCA_ERR_SRC_LOG_PCU_FUN2_REG);
        if(pcuMCAErrSrcLogReg.Bits.msmi_ierr_internal || pcuMCAErrSrcLogReg.Bits.msmi_ierr)
          continue;   //do not clear errors as MSMI IERR is logged
        else
          pcuMCAErrSrcLogReg.Data = 0;  //clear all errors since no MSMI internal errors are logged
        mMemRas->MemWritePciCfg(   Socket, 0, MCA_ERR_SRC_LOG_PCU_FUN2_REG, pcuMCAErrSrcLogReg.Data);

      }
    }
  }
  else
    ASSERT_EFI_ERROR (0);
}

VOID
ClearUboxAndMiscErrorStatusPerSkt (UINT8 Socket)
/*++

Routine Description:
    This function clears the UBOX error status and misc error data

Arguments:

  i/p : scoket number
Returns:

  none

--*/
{

  UBOXERRSTS_UBOX_CFG_STRUCT  uboxErrStsReg;
  UBOXERRMISC_UBOX_CFG_HSX_BDX_STRUCT     uboxErrMisc;
  UBOXERRMISC2_UBOX_CFG_HSX_BDX_STRUCT    uboxErrMisc2;
  IERRLOGGINGREG_UBOX_CFG_BDX_STRUCT                    IerrLoggingReg; // AptioV Server Override: Clear IerrLoggingReg and MCerrLoggingReg
  MCERRLOGGINGREG_UBOX_CFG_BDX_STRUCT               McerrLoggingReg; // AptioV Server Override: Clear IerrLoggingReg and MCerrLoggingReg
  //
  // Note: MCA bank status initially cleared by InitializeMckRegs()
  //       MCA bank status during runtime cleared by OS
  //

  //
  // Clear UBOX uncorrected error status
  //
  uboxErrStsReg.Data = 0; 
  uboxErrStsReg.Bits.poisonrsvd = 0;
  uboxErrStsReg.Bits.smisrcumc = 0;
  uboxErrStsReg.Bits.smisrcimc = 0;
  uboxErrStsReg.Bits.unsupportedopcode = 0;
  uboxErrStsReg.Bits.smi_delivery_valid  = 1;
  mMemRas->MemWritePciCfg(   Socket, 0, UBOXERRSTS_UBOX_CFG_REG, uboxErrStsReg.Data);
  
  uboxErrMisc.Data = 0;
  mMemRas->MemWritePciCfg(   Socket, 0, UBOXERRMISC_UBOX_CFG_REG, uboxErrMisc.Data);
  
  uboxErrMisc2.Data = 0; 
  mMemRas->MemWritePciCfg(   Socket, 0, UBOXERRMISC2_UBOX_CFG_REG, uboxErrMisc2.Data);
// AptioV Server Override Start: Clear IerrLoggingReg and MCerrLoggingReg
  IerrLoggingReg.Data = 0;
  mMemRas->MemWritePciCfg(   Socket, 0, IERRLOGGINGREG_UBOX_CFG_REG, IerrLoggingReg.Data);
  
  McerrLoggingReg.Data = 0;
  mMemRas->MemWritePciCfg(   Socket, 0, MCERRLOGGINGREG_UBOX_CFG_REG, McerrLoggingReg.Data);
// AptioV Server Override End: Clear IerrLoggingReg and MCerrLoggingReg

  //
  // Clear PCU uncorrected error status
  //
  ClearPCUErrors(Socket);

  return;

}

VOID
ClearCBOErrors(VOID)
{
  return;
}


EFI_STATUS
ClearUboxAndMiscErrorStatus (VOID)
/*++

Routine Description:
    This function calls the functuion "ClearUboxAndMiscErrorStatusPerSkt" 
  to clear the UBOX error status and misc error data

Arguments:

  i/p : scoket number
Returns:

  none

--*/
{
  UINT8 Skt;

  for(Skt=0; Skt < MAX_CPU_SOCKETS; Skt++) {
    if(mIioUds->PlatformData.CpuQpiInfo[Skt].Valid == TRUE) {
      ClearUboxAndMiscErrorStatusPerSkt(Skt);
    }
  }
    
  return EFI_SUCCESS;
}

VOID
EnableDisableSMIForUncorrectedErrors (UINT8 Socket)
/*++

Routine Description:
    This function enables the SMI generation of uncorrected errors.

Arguments:

  i/p : socket number
Returns:`

  none

--*/
{
  EFI_STATUS Status;
  EMCAMSMI_ARGS_STRUCT MsmiArgs;

  SMICTRL_UBOX_MISC_HSX_BDX_STRUCT uboxSMICtrlReg;
  VIRAL_CONTROL_PCU_FUN4_HSX_BDX_STRUCT           ViralControlPcuFun4;
  EMCA_EN_CORE_IERR_TO_MSMI_UBOX_CFG_STRUCT EmcaEnIERRMSMIUboxCfg;  
  UINT8 core;
  UINTN cpuNumber;

  //uboxSMICtrlReg.Data = mMemRas->MemReadPciCfg(   Socket, 0, SMICTRL_UBOX_MISC_REG );
  //uboxSMICtrlReg.Bits.umcsmien = 0;
  //mMemRas->MemWritePciCfg(   Socket, 0, SMICTRL_UBOX_MISC_REG, uboxSMICtrlReg.Data);

  if (mErrLogSetupData->SystemErrorEn == 1 && (mErrLogSetupData->WheaLogging || mErrLogSetupData->EMcaLogEn))  {
    if(mErrLogSetupData->MceSmiMorphingEn) {
    if(isEMCAGen2Cap()) {

    uboxSMICtrlReg.Data = mMemRas->MemReadPciCfg(   Socket, 0, SMICTRL_UBOX_MISC_REG );
    uboxSMICtrlReg.Bits.umcsmien = 0;
    mMemRas->MemWritePciCfg(   Socket, 0, SMICTRL_UBOX_MISC_REG, uboxSMICtrlReg.Data);

      EmcaEnIERRMSMIUboxCfg.Data = mMemRas->MemReadPciCfg(Socket, 0, EMCA_EN_CORE_IERR_TO_MSMI_UBOX_CFG_REG);
      EmcaEnIERRMSMIUboxCfg.Bits.encoreierrtomsmi = 1;
      mMemRas->MemWritePciCfg(Socket, 0, EMCA_EN_CORE_IERR_TO_MSMI_UBOX_CFG_REG, EmcaEnIERRMSMIUboxCfg.Data);

      ViralControlPcuFun4.Data = mMemRas->MemReadPciCfg(Socket, 0, VIRAL_CONTROL_PCU_FUN4_REG);
      ViralControlPcuFun4.Bits.emca_mode = 1;
      mMemRas->MemWritePciCfg(Socket, 0, VIRAL_CONTROL_PCU_FUN4_REG, ViralControlPcuFun4.Data);
      
      InitializeSpinLock(&MsmiArgs.ApBusy);

      for (core = 0; core < MAXCOREID; core++) {

        if (ExcutingOnLocalNodeCore (Socket, core) == TRUE) {
          EmcaMSMICoreBanks(&MsmiArgs);
        } else {

          if(mELogHost.cpuInfo[Socket].Valid[core][0]) {
            cpuNumber = GetCpuNumber (Socket, core, 0);
            AcquireSpinLock(&MsmiArgs.ApBusy);
            Status = mSmst->SmmStartupThisAp (EmcaMSMICoreBanks,
                                              cpuNumber,
                                              &MsmiArgs);
            if(Status == EFI_SUCCESS) {
              AcquireSpinLock(&MsmiArgs.ApBusy);  
            }
            ReleaseSpinLock(&MsmiArgs.ApBusy);    
          }
        }
      }
          
      if (ExcutingOnLocalNode (Socket) == TRUE) {
        EmcaMSMIPkgBanks(&MsmiArgs);
      } else {
        if(mELogHost.cpuInfo[Socket].Valid[0][0]) {
          cpuNumber = GetCpuNumber (Socket, 0, 0);
          AcquireSpinLock(&MsmiArgs.ApBusy);
          Status = mSmst->SmmStartupThisAp (EmcaMSMIPkgBanks,
                                            cpuNumber,
                                            &MsmiArgs);
          if(Status == EFI_SUCCESS) {
            AcquireSpinLock(&MsmiArgs.ApBusy);  
          }
          ReleaseSpinLock(&MsmiArgs.ApBusy);            
        }
      }
  }
      }
    // Below code enables UBOX signalling for all uncorrected errors, in non emca (legacy)
    //.Only minimal validation has been done on this, for Grantley
  else if(mErrLogSetupData->UboxUmcSmiEn) {
    uboxSMICtrlReg.Data = mMemRas->MemReadPciCfg(   Socket, 0, SMICTRL_UBOX_MISC_REG );
    uboxSMICtrlReg.Bits.umcsmien = 1;
  uboxSMICtrlReg.Bits.threshold = UBOX_SMI_THRESHOLD_DEFAULT;
    mMemRas->MemWritePciCfg(   Socket, 0, SMICTRL_UBOX_MISC_REG, uboxSMICtrlReg.Data);
      }
  else {
    uboxSMICtrlReg.Data = mMemRas->MemReadPciCfg(   Socket, 0, SMICTRL_UBOX_MISC_REG );
    uboxSMICtrlReg.Bits.umcsmien = 0;
    mMemRas->MemWritePciCfg(   Socket, 0, SMICTRL_UBOX_MISC_REG, uboxSMICtrlReg.Data);
      }
  }

   else {

  }
  return;
}

EFI_STATUS
EnDisUncorrectableErrRep (
  IN ERROR_LOG_SETUP_DATA  *ErrorLogSetupData
  )
/*++

Routine Description:`

    This function enables the SMI generation of uncorrected errors.

Arguments:

    i/p: ErrorLogSetupData
Returns:

    EFI_SUCCESS if the call is succeed

--*/
{
  UINT8 Skt;
    //
  // 1. Enable MCA banks on each cpu
  // Note: All MCA bank MCERRs are enabled at start up in InitializeMckRegs() when MCERR option is enabled.
  //
  
  //
  // Enable SMI for uncorrected errors, if logging needed or UmcSmiEnable
  //
  for(Skt=0; Skt < MAX_CPU_SOCKETS; Skt++) {
    if(mIioUds->PlatformData.CpuQpiInfo[Skt].Valid == TRUE) {
      EnableDisableSMIForUncorrectedErrors(Skt);
    }
  }
  
  return EFI_SUCCESS;
}

VOID
EnableDisableGlobalSmiGeneration(
  )
/*++

Routine Description:

  Enables or disables the generation of SMI

Arguments:

    N/A

Returns:

    EFI_SUCCESS if the call is succeed

--*/
{

  UINT8 Socket;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

  //
    // Ignore disabled nodes
  //
    if ((mELogHost.socketPresentBitMap & (1 << Socket)) == 0) {
      continue;
  }

    EnableDisableGlobalSMIGenerationPerSocket ( Socket);
  }
}

VOID
EnableDisableViralPoisonOnThread (
  VOID *Buffer
  )
/*++

Routine Description:

    Enable poison and viral in the core.

Arguments:


Returns:

--*/
{

  UINT64 Data;
  BOOLEAN poisonSupported;

  // Read MCG_CAP msr 0x179 bit 24 for Poison forwarding enable support
  poisonSupported = ((AsmReadMsr64 (MSR_IA32_MCG_CAP)) & 0x1000000)? 1:0;
  if(poisonSupported == FALSE) { 
    mPoisonEnabled = FALSE;
    return;
  }

  /*if ( (mViralEnabled == 1) && (mPoisonEnabled == 1)) {
    Data = AsmReadMsr64(MSR_MCG_CONTAIN);
      Data |= BIT1 + BIT0;
      AsmWriteMsr64 (MSR_MCG_CONTAIN, Data);
  } else*/
  if ( mPoisonEnabled == TRUE) {
  // Write poison enable bit in MCG_CONTAIN MSR
  // Note: write will GP fault if poison not supported by core
    Data = AsmReadMsr64(MSR_MCG_CONTAIN);
      Data |=  BIT0;                        // just enable poison
      AsmWriteMsr64 (MSR_MCG_CONTAIN, Data);
  } else {
    // Disable Poison & viral bit
    Data = AsmReadMsr64(MSR_MCG_CONTAIN);
    Data &= ~(03); // Clear bit0 & bit1 - poison & viral  bits
    AsmWriteMsr64 (MSR_MCG_CONTAIN, Data);
  }
  return;

}


VOID
ProgramViralPoison(
  VOID
  )
/*++

Routine Description:

  this function enbales  or disables poison forwarding and viral.

Arguments:

    N/A

Returns:

    EFI_SUCCESS if the call is succeed

--*/
{
  UINTN       thread;
  UINT8   RootBridgeLoop;

  // mSmmBase->GetSmstLocation (mSmmBase, &mSmst);
  for (thread = 0; thread < mELogHost.cpuCount; thread++) {
    mSmst->SmmStartupThisAp (EnableDisableViralPoisonOnThread,
                             thread, 
                             NULL);
  }
    // Enable PF on SBSP also.
  EnableDisableViralPoisonOnThread (NULL);
  //TODO: Commenting out the Viral Enable code
  /*
  if(mViralEnabled == TRUE) {  
  EnableQPIViral();
  EnableUBoxViral();
  }*/
  //
  // Enable poison control in IOH
  //
  for ( RootBridgeLoop =0; RootBridgeLoop < mNumIoh; RootBridgeLoop++) {
    if (mIohInfo[RootBridgeLoop].Valid != 1)
      continue;
  if (mPoisonEnabled == TRUE) {
    EnablePoisonIoh (RootBridgeLoop);
   //Viral can only be enabled if poison is enabled
  /*if(mViralEnabled == TRUE) {
    IIOEnableViral(RootBridgeLoop);
      }*/
    }
  }
}

VOID
EnableDisableGlobalSMIGenerationPerSocket (UINT8 Socket)
/*++

Routine Description:
    This function enables/disable the generation of smi

Arguments:

  i/p : Socket number
Returns:

  none

--*/
{
  SMICTRL_UBOX_MISC_HSX_BDX_STRUCT uboxSMICtrlReg;
  uboxSMICtrlReg.Data = mMemRas->MemReadPciCfg(  Socket, 0, SMICTRL_UBOX_MISC_REG);
  if(mErrLogSetupData->SystemErrorEn != 0) {
    uboxSMICtrlReg.Bits.smidis = 0;
  } else {
    uboxSMICtrlReg.Bits.smidis = 1;
  }
  mMemRas->MemWritePciCfg(   Socket, 0, SMICTRL_UBOX_MISC_REG, uboxSMICtrlReg.Data);

  return;
}


VOID
ClearQPIErrors(VOID)
{
  return;
}

/*
VOID
EnableQPIViral(
   VOID
  )
{
  UINT8 Skt;
  UINT8 LinkIndex;
  UINT8 Ctr; 
  UINT8 num_r3qpi;
#ifdef IVT_HOST
  QPIVIRAL_QPIAGENT_LL_STRUCT   qpiviralReg;
  QPIERRDIS_QPIAGENT_LL_IVT_STRUCT  qpierrdisReg;
  R2_GL_ERR_CFG_R2PCIE_CFG_STRUCT   r2glerrcfgReg;
  R3_GL_ERR_CFG_R3QPI_CFG_STRUCT  r3glerrcfgReg; 
#else
  QPIVIRAL_QPIAGENT_DFX_LINK_STRUCT qpiviralReg;
  R2GLERRCFG_R2PCIE_CFG_STRUCT      r2glerrcfgReg;
  R3GLERRCFG_R3QPI_CFG_STRUCT       r3glerrcfgReg;
  QPIERRDIS_QPIAGENT_LL_HSX_BDX_STRUCT  qpierrdisReg;
#endif
  if (mIioUds->SystemStatus.cpuType == (CPU_IVT || CPU_HSX || CPU_BDX)) { //If IVT or HSX or BDX
      for (Skt = 0; Skt < MAX_CPU_SOCKETS; Skt++) { //walk through each socket populated
        if(mIioUds->PlatformData.CpuQpiInfo[Skt].Valid == TRUE) { //proceed if socket is online
          for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; LinkIndex++) { //walk through each QPI link
            if(mIioUds->PlatformData.CpuQpiInfo[Skt].PeerInfo[LinkIndex].Valid == TRUE) { //proceed if link LEP is enabled            
          if ( mErrLogSetupData->ViralEn == TRUE) {  //proceed if Viral enabled
            //Clear failover status
                qpiviralReg.Data = 0;
#ifdef IVT_HOST
              qpiviralReg.Data |=  (mMemRas->MemReadPciCfg(Skt, LinkIndex, QPIVIRAL_QPIAGENT_LL_REG) & 0x7);
              qpiviralReg.Bits.qpi_fatal_viral_alert_enable = 1;
              qpiviralReg.Bits.qpi_signal_global_fatal = 1;
              qpiviralReg.Bits.qpi_global_viral_mask =1;
              mMemRas->MemWritePciCfg(Skt, LinkIndex, QPIVIRAL_QPIAGENT_LL_REG, qpiviralReg.Data);
#else
                qpiviralReg.Data |=  mMemRas->MemReadPciCfg(Skt, LinkIndex, QPIVIRAL_QPIAGENT_DFX_LINK_REG);
                qpiviralReg.Bits.qpi_failover_status = 1;
                mMemRas->MemWritePciCfg(Skt, LinkIndex, QPIVIRAL_QPIAGENT_DFX_LINK_REG, qpiviralReg.Data);
#endif 
                qpierrdisReg.Data = mMemRas->MemReadPciCfg(Skt, LinkIndex, QPIERRDIS_QPIAGENT_LL_REG);           
          if ( mErrLogSetupData->QPIFailoverSmiEn == TRUE) {
                 //Set smi enable bit
                  qpierrdisReg.Bits.smi_en = 1;
                } else {
                //Clear smi enable bit
                qpierrdisReg.Bits.smi_en = 0;           
                  } //End of if Failover
                mMemRas->MemWritePciCfg(Skt, LinkIndex, QPIERRDIS_QPIAGENT_LL_REG, qpierrdisReg.Data);
              } //End if Viral enabled
            }  //End if Link LEP enabled
          } //End for each QPI link


    //
        // Enable Viral in QPI R2_GL_ERR_CFG  - there is one R2_GL_ERR_CFG per socket
    //
        if ( mErrLogSetupData->ViralEn == TRUE) { //If Viral is enabled
      r2glerrcfgReg.Data = 0;
          num_r3qpi = 2;                // 2 r3qpi per socket in IVT and HSW
      Ctr= 0;
#ifdef IVT_HOST
          r2glerrcfgReg.Data |=  mMemRas->MemReadPciCfg(Skt, 0, R2_GL_ERR_CFG_R2PCIE_CFG_REG);
      // unmask bit [0-14] in this register for enabling Viral 
        r2glerrcfgReg.Data &= 0xFFFF8000;
      mMemRas->MemWritePciCfg(Skt, 0, R2_GL_ERR_CFG_R2PCIE_CFG_REG,r2glerrcfgReg.Data);
        // Enable Viral in r3qpi 
      for (Ctr = 0; Ctr < num_r3qpi; Ctr++) {
      r3glerrcfgReg.Data = 0;
      r3glerrcfgReg.Data |= mMemRas->MemReadPciCfg(Skt, Ctr, R3_GL_ERR_CFG_R3QPI_CFG_REG);
      //unmask bit [0-17] in this register
      r3glerrcfgReg.Data &= 0xFFFC0000;
      mMemRas->MemWritePciCfg(Skt, Ctr, R3_GL_ERR_CFG_R3QPI_CFG_REG,r2glerrcfgReg.Data);
        }
#else
          r2glerrcfgReg.Data |=  mMemRas->MemReadPciCfg(Skt, 0, R2GLERRCFG_R2PCIE_CFG_REG);
          // unmask bits [3:7], [9:10], [13:20] in this register for enabling Viral 
          r2glerrcfgReg.Data &= 0xFFE01907;
          mMemRas->MemWritePciCfg(Skt, 0, R2GLERRCFG_R2PCIE_CFG_REG,r2glerrcfgReg.Data);
          // Enable Viral in r3qpi 
          for (Ctr = 0; Ctr < num_r3qpi; Ctr++) {
            r3glerrcfgReg.Data = 0;
            r3glerrcfgReg.Data |= mMemRas->MemReadPciCfg(Skt, Ctr, R3GLERRCFG_R3QPI_CFG_REG);
            //unmask bits [3:10], [13:25] in this register
            r3glerrcfgReg.Data &= 0xFC001807;
            mMemRas->MemWritePciCfg(Skt, Ctr, R3GLERRCFG_R3QPI_CFG_REG,r2glerrcfgReg.Data);
          }
#endif 
        }  //End of Viral Enable
    }  //End If Socket Online
  }  //End For Socket Populated
  }  //End if IVT || HSX
}

VOID
EnableUBoxViral(
  VOID
  )
{
#ifdef IVT_HOST
  UBOX_GL_ERR_CFG_UBOX_CFG_IVT_STRUCT   uboxglerrcfgReg;
#else
  UBOX_GL_ERR_CFG_UBOX_CFG_HSX_BDX_STRUCT   uboxglerrcfgReg;
#endif

  UINT8 Skt;

  //SMI failover is new in IVT
  if (mIioUds->SystemStatus.cpuType == (CPU_IVT || CPU_HSX || CPU_BDX)) {
  for (Skt = 0; Skt < MAX_CPU_SOCKETS; Skt++) {
    if(mIioUds->PlatformData.CpuQpiInfo[Skt].Valid == TRUE) {
    // Enable Viral in ubox
    uboxglerrcfgReg.Data=0;
    uboxglerrcfgReg.Data |= ( mMemRas->MemReadPciCfg(Skt, 0, UBOX_GL_ERR_CFG_UBOX_CFG_REG) | 0x30);
    //unmask bit [0-21] 
    uboxglerrcfgReg.Data &= 0xFFF80030;
    mMemRas->MemWritePciCfg(Skt, 0, UBOX_GL_ERR_CFG_UBOX_CFG_REG, uboxglerrcfgReg.Data );
      }
    }
  }
}
*/

VOID
EnableUboxError(
  VOID
  ) 
/*++

Routine Description: Function to enbale Ubox erros.
      -  Sighting 4031463: CLONE from ivytown: need to enable MCA's in R3CSI/R2PCIE

Arguments:

   i/p : none

Returns:

    none

--*/
{
  UINT8    Skt;
  UBOXERRCTL_UBOX_CFG_STRUCT    uboxErrCtrl;
  SMICTRL_UBOX_MISC_HSX_BDX_STRUCT uboxSMICtrlReg;


  for (Skt = 0; Skt < MAX_CPU_SOCKETS; Skt++) {
    if(mIioUds->PlatformData.CpuQpiInfo[Skt].Valid == TRUE) {
      uboxErrCtrl.Data = mMemRas->MemReadPciCfg( Skt, 0, UBOXERRCTL_UBOX_CFG_REG);
      // Sighting 4031463: CLONE from ivytown: need to enable MCA's in R3CSI/R2PCIE
      // Deatils : uboxerrctl.uboxerrortopcumask (bit 24) = 0 This bit will enable UBox propopagation of errors to the PCU.
      if( mErrLogSetupData->SystemErrorEn == 0)  {
        uboxErrCtrl.Bits.uboxerrortopcumask  = 1;
        mMemRas->MemWritePciCfg( Skt, 0, UBOXERRCTL_UBOX_CFG_REG, uboxErrCtrl.Data);
        continue;

      } else {
        uboxErrCtrl.Bits.uboxerrortopcumask  = 0;
        mMemRas->MemWritePciCfg( Skt, 0, UBOXERRCTL_UBOX_CFG_REG, uboxErrCtrl.Data);
        // Enable the Generation of SMI from message channel
        uboxSMICtrlReg.Data = mMemRas->MemReadPciCfg(  Skt, 0, SMICTRL_UBOX_MISC_REG);
        uboxSMICtrlReg.Bits.smidis3 = 0;
        mMemRas->MemWritePciCfg(   Skt, 0, SMICTRL_UBOX_MISC_REG, uboxSMICtrlReg.Data);
      }
    }
  }
}

EFI_STATUS
EnableElogQPI (
  VOID
  )
/*++

Routine Description:`


Arguments:

   i/p : ErrorLogSetupData

Returns:

    EFI_SUCCESS if the call is succeed

--*/
{
  R2PINGERRMSK0_R2PCIE_CFG_HSX_BDX_STRUCT   r2pIngErrReg;
  R2EGRERRMSK_R2PCIE_CFG_HSX_BDX_STRUCT     r2EgrMskReg;
  R2EGRERRMSK2_R2PCIE_CFG_HSX_BDX_STRUCT    r2EgrMsk2Reg;
  R3EGRERRMSK0_R3QPI_CFG_STRUCT         r3EgrMsk0;
  R3EGRERRMSK1_R3QPI_CFG_STRUCT         r3EgrMsk1;
  UINT8 Skt;

  //SMI failover is new in IVT
  if ((mIioUds->SystemStatus.cpuType == CPU_IVT) || 
      (mIioUds->SystemStatus.cpuType == CPU_HSX) || 
      (mIioUds->SystemStatus.cpuType == CPU_BDX)) {
    for (Skt = 0; Skt < MAX_CPU_SOCKETS; Skt++) {
      if(mIioUds->PlatformData.CpuQpiInfo[Skt].Valid == TRUE) {
         r2pIngErrReg.Data = mMemRas->MemReadPciCfg(Skt, 0, R2PINGERRMSK0_R2PCIE_CFG_REG);
         if (mErrLogSetupData->SystemErrorEn == 1){
            //s4988141.Issue: unmasking ParErrE2E1Msk & ParErrE2E0Msk triggers CATERR
            r2pIngErrReg.Data &= 0x3000;
            r2EgrMskReg.Data = 0;
            r2EgrMsk2Reg.Data = 0;
            r3EgrMsk0.Data = 0;
            r3EgrMsk1.Data = 0;
         }
         else{
            r2pIngErrReg.Data = 0xffff;
            r2EgrMskReg.Data = 0x3FFFFFFF;
            r2EgrMsk2Reg.Data = 0x3c3fffff;
            r3EgrMsk0.Data = 0xFF6FFFFF;
            r3EgrMsk1.Data = 0xFF6FFFFE;
         }
        mMemRas->MemWritePciCfg(Skt, 0, R2PINGERRMSK0_R2PCIE_CFG_REG, r2pIngErrReg.Data);   
        mMemRas->MemWritePciCfg(Skt, 0, R2EGRERRMSK_R2PCIE_CFG_REG, r2EgrMskReg.Data);
        mMemRas->MemWritePciCfg(Skt, 0, R2EGRERRMSK2_R2PCIE_CFG_REG, r2EgrMsk2Reg.Data);
        mMemRas->MemWritePciCfg(Skt, 0, R3EGRERRMSK0_R3QPI_CFG_REG, r3EgrMsk0.Data);
        mMemRas->MemWritePciCfg(Skt, 0, R3EGRERRMSK1_R3QPI_CFG_REG, r3EgrMsk1.Data);
    if ((mIioUds->SystemStatus.MinimumCpuStepping == A0_REV) && (mIioUds->SystemStatus.cpuType == (CPU_IVT))) {
          // 
          // HSD BRICKLAND BIOS 4030016: 
          // the logic is reversed as per the definition. It will gate if the bit mask value is 0 and allow if it is 1.
          // This bug would be only for A0 stepping, expected to get fixed in B0 stepping. 
          // Enable by default always - TODO: Check if setup option is needed. 
          //
          r2pIngErrReg.Bits.blbgfcrderrmsk = 1;
          r2pIngErrReg.Bits.ubxcboncbcrderrmsk = 1;
          r2pIngErrReg.Bits.ubxcboncscrderrmsk = 1;
          r2pIngErrReg.Bits.ubxqpincbcrderrmsk = 1;
          r2pIngErrReg.Bits.ubxqpincscrderrmsk = 1;
          r2pIngErrReg.Bits.iioidicrderrmsk = 1;
          r2pIngErrReg.Bits.iioncbcrderrmsk = 1;
          r2pIngErrReg.Bits.iioncscrderrmsk = 1;
        }
        mMemRas->MemWritePciCfg(Skt, 0, R2PINGERRMSK0_R2PCIE_CFG_REG, r2pIngErrReg.Data);     
	  }
    } 
  }
  return EFI_SUCCESS;
}



VOID EnableCloakingPerSocket(
	UINT8 *buffer)
/*++
EnableCloakingPerSocket:

Routine Description:
    Enables Cloaking per socket, by disabling the CMCI in MSR 17F and by enabling bits 0&1 in MSR 52 so that 
    read to MC status registers will return 0 for UCNA & corrected Errors

Arguments:
	none
	
Returns:
	none
   
--*/
{
	UINT64  RegValue_McaErrorControl;
	UINT64  RegValue_ErrorControl;	
	if (mIioUds->SystemStatus.cpuType == CPU_BDX) {
	DEBUG ((DEBUG_ERROR, " EnableCloakingPerSocket start \n " ));
    RegValue_McaErrorControl = AsmReadMsr64(MSR_MCA_ERROR_CONTROL);
	//BIT0 - When set to 1, a rdmsr to any MCi_STATUS register will return 0,
	//while a corrected error is logged in the register unless the processor is in SMM mode
	//BIT1 - When set to 1, a rdmsr to any MCi_STATUS register will return 0 
	//while a UCNA error is logged in the register unless the processor is in SMM mode
    RegValue_McaErrorControl |= (BIT0 | BIT1); 
	AsmWriteMsr64(MSR_MCA_ERROR_CONTROL, RegValue_McaErrorControl);
	
	RegValue_ErrorControl = AsmReadMsr64(MSR_ERROR_CONTROL);
	//BIT4 - When set to 1, disables corrected machine check interrupt entirely, cleared upon each reset
	RegValue_ErrorControl |= BIT4;
	AsmWriteMsr64(MSR_ERROR_CONTROL, RegValue_ErrorControl);
	DEBUG ((DEBUG_ERROR, " EnableCloakingPerSocket end \n" ));	

	ReleaseSpinLock(&mApLock);
		}
	return;
}



VOID EnableCloaking 
(
VOID
)
/*++
EnableCloaking:

Routine Description:
	Enables Cloaking Bits for all the sockets using SmmStartupThisAp() 

Arguments:
	none
	
Returns:
	none
   
--*/
{
	UINT8	socket;
	//UINT32	cpuNumber;
	UINT8 thread;	
	BOOLEAN 									  FoundThread = FALSE;
	EFI_PROCESSOR_INFORMATION					  ProcInfo; 
	EFI_STATUS                                    Status;

	UINT64  RegValue_McaErrorControl;
	UINT64  RegValue_ErrorControl;
	if (mIioUds->SystemStatus.cpuType == CPU_BDX) {
	DEBUG ((DEBUG_ERROR, " EnableCloaking start \n" ));
	for (socket = 0; socket < MAX_SOCKET; socket++) {
		//
    	// Ignore disabled nodes
        //
        if ((mELogHost.socketPresentBitMap & (1 << socket)) == 0) {
			continue;
        	}

		if (ExcutingOnLocalNode (socket) == TRUE) {
			//EnableCloakingPerSocket(NULL);
			RegValue_McaErrorControl = AsmReadMsr64(MSR_MCA_ERROR_CONTROL);
	        RegValue_McaErrorControl |= (BIT0 | BIT1); 
        	AsmWriteMsr64(MSR_MCA_ERROR_CONTROL, RegValue_McaErrorControl);
	
	        RegValue_ErrorControl = AsmReadMsr64(MSR_ERROR_CONTROL);
	        RegValue_ErrorControl |= BIT4;
	        AsmWriteMsr64(MSR_ERROR_CONTROL, RegValue_ErrorControl);
			
			} else {
			InitializeSpinLock(&mApLock);
			FoundThread = FALSE;
			for(thread = 0; thread < mSmst->NumberOfCpus; thread++) {
				if(mSmmCpuService->GetProcessorInfo(mSmmCpuService,thread, &ProcInfo) == EFI_SUCCESS) {
					if((ProcInfo.Location.Package == socket) && (ProcInfo.StatusFlag & PROCESSOR_ENABLED_BIT) != 0) {
						FoundThread = TRUE;
						break;
					}
				  }   
				}
			    if (FoundThread){
				AcquireSpinLock(&mApLock);
				Status = mSmst->SmmStartupThisAp(EnableCloakingPerSocket, thread, &socket);
				if(Status == EFI_SUCCESS) {
				  while ( (AcquireSpinLockOrFail(&mApLock)) != TRUE  ) {
					CpuPause();
				  }
				}
				ReleaseSpinLock(&mApLock);
			    	}


			
			/*if(mELogHost.cpuInfo[socket].Valid[0][0]) {
				cpuNumber = GetCpuNumber (socket, 0, 0);
				mSmst->SmmStartupThisAp (EnableCloakingPerSocket,
										 cpuNumber,
										 NULL);
				}*/
			  }
		}
	DEBUG ((DEBUG_ERROR, " EnableCloaking end \n" ));
		}
	return;
}

VOID
ProcessorCatastrophicErrorHandler (
  UINT8 node
  )
{
  //
  // IERR => should not get here
  //
  CpuDeadLoop();
  return;
}


VOID
UncorrectedCoreErrorHandler (
  UINT8 *Socket)
{
  UINT8 mcBank;

  //
  // Process errors from each core MC bank (TODO: check which mc banks are in core)
  //
  for (mcBank = 0; mcBank < MCA_CORE_MCBANKS; mcBank++) {
    //UncorrectedMCBankErrorHandler (mcBank);
  }

  return;
}

/*Copying this function from BaseXApicX2ApicLib.c */
UINT32
EFIAPI
ReadLocalApicRegrep (
  IN UINTN  MmioOffset
  )
{
  UINT32 MsrIndex;

  ASSERT ((MmioOffset & 0xf) == 0);

  if (GetApicMode () == LOCAL_APIC_MODE_XAPIC) {
    return MmioRead32 (PcdGet32 (PcdCpuLocalApicBaseAddress) + MmioOffset);
  } else {
    //
    // DFR is not supported in x2APIC mode.
    //
    ASSERT (MmioOffset != XAPIC_ICR_DFR_OFFSET);
    //
    // Note that in x2APIC mode, ICR is a 64-bit MSR that needs special treatment. It
    // is not supported in this function for simplicity.
    //
    ASSERT (MmioOffset != XAPIC_ICR_HIGH_OFFSET);

    MsrIndex = (UINT32)(MmioOffset >> 4) + X2APIC_MSR_BASE_ADDRESS;
    return AsmReadMsr32 (MsrIndex);
  }
}




/*BOOLEAN
SocketInViralState () 
{
  BOOLEAN ViralState = FALSE;
#ifdef IVT_HOST
    QPIVIRAL_QPIAGENT_LL_STRUCT qpiviralReg;
#else
    QPIVIRAL_QPIAGENT_DFX_LINK_STRUCT qpiviralReg;
#endif
  VIRAL_IIO_RAS_STRUCT      viraliiorasReg;
  UINT8 Socket = 0, LinkIndex =0;


  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Ignore disabled nodes
    //
    if ((mELogHost.socketPresentBitMap & (1 << Socket)) == 0) {
      continue;
    }
    // Read the QPI Viral state in all the ports for the socket 
    for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; LinkIndex++) {
      qpiviralReg.Data = 0;
      qpiviralReg.Data = mMemRas->MemReadPciCfg(Socket, LinkIndex, QPIERRDIS_QPIAGENT_LL_REG);   
      if (qpiviralReg.Bits.qpi_viral_state ==1) {
          ViralState = TRUE;
          break;
      }
    }
    // read the IIO to check for Viral State
    viraliiorasReg.Data =0;
    viraliiorasReg.Data |=  mMemRas->MemReadPciCfg(Socket, 0, VIRAL_IIO_RAS_REG);
    if (viraliiorasReg.Bits.iio_viral_state ==1 ) {
      ViralState = TRUE;
      break;
    }
  }
  return ViralState;
}*/

BOOLEAN
PendingProcessorErrorExists (
  UINT32 *UmcSockets,
  UINT32 *ImcSockets,
  UINT32 *ImcNodesEventProgress,
  UINT32 *ImcNodesNewEvents
  )
{
  UINT8   localSocket  = 0;
  UINT8   localCore = 0;
  UINT8   socket = 0;
  SMICTRL_UBOX_MISC_HSX_BDX_STRUCT uboxSMICtrlReg;
  UBOXERRSTS_UBOX_CFG_STRUCT uboxErrStsReg;

  //
  // Check if smi enabled (same on every socket).
  //

  ExtractNodeCoreFromApicId (&localSocket, &localCore);

  uboxSMICtrlReg.Data = mMemRas->MemReadPciCfg(  localSocket, 0, SMICTRL_UBOX_MISC_REG);

  

  for (socket = 0; socket < MAX_SOCKET; socket++) {

    //
    // Ignore disabled nodes
    //
    if ((mELogHost.socketPresentBitMap & (1 << socket)) == 0) {
      continue;
    }

    //
    // Check for pending Uncorrected processor errors
    //
    uboxErrStsReg.Data = mMemRas->MemReadPciCfg(  socket, 0, UBOXERRSTS_UBOX_CFG_REG);

    if (uboxErrStsReg.Bits.smisrcumc == 1) {
      // Track which sockets have umc pending
      *UmcSockets |= (1 << socket);
      RAS_DEBUG((4, " misrcumc == 1  UmcSockets = %X\n",*UmcSockets));  
    }
    if (uboxErrStsReg.Bits.smisrcimc == 1) {
      // Track which sockets have imc pending
      *ImcSockets |= (1 << socket);
      //RAS_DEBUG((4, "smisrcimc == 1 ImcSockets:%x  extraRas ha0  :%x  extraRas ha1: %x  emcaltctl:%x \n",
      //           *ImcSockets,
      //           mMemRas->MemReadPciCfg( socket, 0, EXRAS_CONFIG_HA_CFG_REG),
      //           mMemRas->MemReadPciCfg( socket, 1, EXRAS_CONFIG_HA_CFG_REG),
      //           mMemRas->MemReadPciCfg(  socket, PARAM_UNUSED, EMCALTCTL_MC_MAINEXT_REG)));
      RAS_DEBUG((4, "smisrcimc == 1 ImcSockets:%x  extraRas ha0: %x  extraRas ha1: %x\n",
                 *ImcSockets,
                 mMemRas->MemReadPciCfg(socket, 0, EXRAS_CONFIG_HA_CFG_REG),
                 mMemRas->MemReadPciCfg(socket, 1, EXRAS_CONFIG_HA_CFG_REG)));
    }
  }

  mMemRas->GetBitMapOfNodeWithErrors(  ImcNodesNewEvents );
  RAS_DEBUG((4, "ImcNodesNewEvents = %X\n",*ImcNodesNewEvents));
  mMemRas->GetBitMapOfNodeWithEventInProgress(  ImcNodesEventProgress );
  RAS_DEBUG((4, "ImcNodesEventProgress = %X\n",*ImcNodesEventProgress));

  if((*UmcSockets != 0) || (*ImcNodesNewEvents != 0) ||(*ImcNodesEventProgress != 0) || (*ImcSockets != 0)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

UINT32
MapErrorSeverity
(
 EMCA_MC_SIGNATURE_ERR_RECORD *McErrSig
)
{
  IA32_MCI_STATUS_MSR_STRUCT McSts;
  UINT32 sev = EFI_ACPI_5_0_ERROR_SEVERITY_NONE;

  McSts.Data = McErrSig->Signature.McSts;

  if((McSts.Bits.uc == 1)&& (McSts.Bits.en == 1)) {
    
    if(McSts.Bits.pcc != 1) {
      
      if(McSts.Bits.s == 1) {
        sev = EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTABLE;//recoverable
        //if((McSts.Bits.ar == 1) {
        //  sev = EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTABLE; //SRAR - Recoverable
        //} else {
        //  sev = EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTABLE; //SRAO - Recoverable
        //} 
        
      } else {
        sev = EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTED; //UCNA
      }
    } else {
      sev = EFI_ACPI_5_0_ERROR_SEVERITY_FATAL;
    }
  } else {
    sev = EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTED; //Corrected Error
  }

  return sev;
}


VOID
UncorrectedMCBankErrorHandler ( 
  UINT8 McBank,
  UINT8 *skt,
  SMM_ERROR_CPU_DATA_BLOCK *CpuData,
  GENERIC_ERROR_STATUS *ErrSts
  )
{
  EFI_STATUS Status;  
  IA32_MCI_STATUS_MSR_STRUCT MciStsMsr;
  EMCA_MC_SIGNATURE_ERR_RECORD McErrSig;
  UINT32 sev;
  UINT32 PrevSev;
  BOOLEAN Recovered = FALSE;
  BOOLEAN UpdateSev = FALSE;
  UINT64 PrevState = SmmMceClearedState;
  UINT64 State = SmmMceClearedState;

  //Init structures

  McErrSig.Signature.McSts = 0;
  McErrSig.Signature.McAddr = 0;
  McErrSig.Signature.McMisc = 0;
  McErrSig.ApicId = 0;
  sev = EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTED;
  PrevSev = EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTED;

  //
  // Fill Error Signature
  // (McSts, McAddr, McMisc, apicid)
  //
  PrevState = InterlockedCompareExchange64((UINT64*) &CpuData->SmmMceSyncState, SmmMceClearedState, SmmMceReadingState);
  State = (PrevState == SmmMceClearedState) ? SmmMceReadingState : PrevState; //Reading State
  do {
    switch(State) {
      case SmmMceReadingState:
        Status = ReadMcBankSigHook(McBank,*skt,&McErrSig);
        ASSERT_EFI_ERROR(Status);
        PrevState = InterlockedCompareExchange64((UINT64*) &CpuData->SmmMceSyncState,SmmMceReadingState, SmmMceProcessingState);
        State = (PrevState == SmmMceReadingState) ? SmmMceProcessingState : PrevState;
        break;
      case SmmMceProcessingState:
        MciStsMsr.Data = McErrSig.Signature.McSts;
        if(MciStsMsr.Bits.val != 1) {
          //Removed MciStsMsr.Bits.uc != 1  || MciStsMsr.Bits.en != 1
          // This is to process correctable errors also. Further review might be needed. 
          PrevState = InterlockedCompareExchange64((UINT64*) &CpuData->SmmMceSyncState,SmmMceProcessingState, SmmMceClearedState);
          State = (PrevState == SmmMceProcessingState) ? SmmMceClearedState : PrevState;
          // Aptio V Server Override Start: Changes done to support LastBoot Error Handling and Logging Uncorrectable Errors (Memory/PCIE ...)   
        #if HandleLastbootError_Support  
            if ((McBank == MCA_BANK_HA) || (McBank == (MCA_BANK_HA + 1) ))
            {
                Status = ReadMcBankSignature(McBank,*skt,&McErrSig);
                MciStsMsr.Data = McErrSig.Signature.McSts;
                
            }
            if (MciStsMsr.Bits.val != 1)
            {
        		break; //Not valid error or not uncorrected error, end of the flow     
            }
        #else   
            break; //Not valid error or not uncorrected error, end of the flow
        #endif    
         // Aptio V Server Override End: Changes done to support LastBoot Error Handling and Logging Uncorrectable Errors (Memory/PCIE ...)     
        } //else
        sev = MapErrorSeverity(&McErrSig);
        if(sev == EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTABLE) {
          Recovered = AttemptRecovery(McBank, *skt,&McErrSig);
          if(Recovered == TRUE) {
            sev = EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTED;
            PrevState = InterlockedCompareExchange64((UINT64*) &CpuData->SmmMceSyncState,SmmMceProcessingState, SmmMceWritingState);
            State = (PrevState == SmmMceProcessingState) ? SmmMceWritingState : PrevState;
            break;
          }
        }
        //Recovery not posible, log the error. 
        PrevState = InterlockedCompareExchange64((UINT64*) &CpuData->SmmMceSyncState,SmmMceProcessingState, SmmMceLoggingState);
        State = (PrevState == SmmMceProcessingState) ? SmmMceLoggingState : PrevState;
        break;
      case SmmMceWritingState:
        WriteMcBank(McBank,&McErrSig);
        PrevState = InterlockedCompareExchange64((UINT64*) &CpuData->SmmMceSyncState,SmmMceWritingState, SmmMceLoggingState);
        State = (PrevState == SmmMceWritingState) ? SmmMceLoggingState : PrevState;
        break;
      case SmmMceLoggingState:
        FillErrorSection(McBank, skt, sev,&McErrSig,ErrSts);
        ConstructErrorRecord(sev, ErrSts);
        LogErrorRecord(McBank, &McErrSig, ErrSts); 

        //Update severity 
        PrevSev = CpuData->EventLog.Events[McbankSrc].Severity;
        UpdateSev = (PrevSev == EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTED || 
                    (PrevSev == EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTABLE && sev == EFI_ACPI_5_0_ERROR_SEVERITY_FATAL ));
        if (!mLastbootMemErrFlag) { // AptioV Server Override: To resolve Cache error with C0 CPUs.
        if(UpdateSev) {
          InterlockedCompareExchange32( (UINT32*) &CpuData->EventLog.Events[McbankSrc].Severity,
                                       PrevSev,
                                       sev
                                      );
        	  CpuData->EventLog.ErrorsFound= TRUE;
        	  CpuData->EventLog.Events[McbankSrc].ErrorFound = TRUE;
        	  CpuData->EventLog.Events[McbankSrc].Source = McbankSrc;
        	  CpuData->EventLog.Events[McbankSrc].ApicId = McErrSig.ApicId;
        	  CpuData->EventLog.Events[McbankSrc].BankIdx = McBank;
		  }
        } // AptioV Server Override: To resolve Cache error with C0 CPUs.
        PrevState = InterlockedCompareExchange64((UINT64*) &CpuData->SmmMceSyncState,SmmMceLoggingState, SmmMceClearedState);
        // Aptio V Server override :  Modified the below line if not two times Logging part is called.
        //State = (PrevState == SmmMceProcessingState) ? SmmMceLoggingState : PrevState;  
        State = (PrevState == SmmMceLoggingState) ? SmmMceClearedState : PrevState;
        HandleCoreAndUncoreErrors(McBank, *skt, &McErrSig); //Aptiov Server Override
        // Aptio V Server Override End: Changes done to support LastBoot Error Handling and Logging Uncorrectable Errors (Memory/PCIE ...)       
        break;
      case SmmMceReadingInterrupted:
      case SmmMceProcessingInterrupted:
        //Recovery is not possible
        //MCE handler already logged the error and updated the EventLog
        //CpuData->EventLog.ErrorsFound= TRUE;
        //CpuData->EventLog.Events[McbankSrc].ErrorFound = TRUE;
        //CpuData->EventLog.Events[McbankSrc].Source = McbankSrc;
        //CpuData->EventLog.Events[McbankSrc].ApicId = McErrSig.ApicId;
        //CpuData->EventLog.Events[McbankSrc].BankIdx = McBank;
        //CpuData->EventLog.Events[McbankSrc].Severity = EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTABLE;
        PrevState = InterlockedCompareExchange64((UINT64*) &CpuData->SmmMceSyncState,State, SmmMceClearedState);
        State = (PrevState == State) ? SmmMceClearedState : PrevState;
        break;
      case SmmMceClearedState:
        break;
      case SmmMceWritingInterrupted:
        ASSERT(FALSE); //MCE handler never returns when Writing is interrupted. 
        break;
      default:
        ASSERT(FALSE);
        break;
    }
  } while(State != SmmMceClearedState); 
 
  return;
}

VOID
ExtractProcessorLocation(
  IN UINT32 ApicId,
  OUT EFI_CPU_PHYSICAL_LOCATION *Location
  ) 
{
  UINTN ThreadBits;
  UINTN CoreBits;
  
  GetCoreAndSMTBits(&ThreadBits,&CoreBits);
  Location->Package = ApicId >> (ThreadBits+CoreBits);
  Location->Core = (UINT32)((ApicId >> ThreadBits) & ~((~0) << CoreBits));
  Location->Thread = (UINT32)(ApicId & ~((~0) << ThreadBits));  
}

UINT64
ReadCoreMcBankBitField(
VOID
)
{
  UINT64 CoreMcBankBitField = 0;
  UINT8 mcbank = 0;
  UINT16 mcscope = 0;
  UINT64 maxbank = 0;
  
  if(isEMCAGen2Cap() && (mErrLogSetupData->MceSmiMorphingEn == 1 || mErrLogSetupData->EMcaCSmiEn == 1) ) {
    CoreMcBankBitField = ReadCoreSmiSrc();          
  } else {
    maxbank = GetNumMcBanksPerLp();
    for (mcbank = 0, CoreMcBankBitField = 0; mcbank < maxbank; mcbank++) {
      mcscope = GetMcBankScope(mcbank);
      if (mcscope == MC_SCOPE_CORE) {
        CoreMcBankBitField |= (1ULL << mcbank);
      }
    }      
  }
  return CoreMcBankBitField;
}

VOID
ClearCoreMcBankBitField(
  UINT64 McBankBitField
)
{
  if(isEMCAGen2Cap() && (mErrLogSetupData->MceSmiMorphingEn == 1 || mErrLogSetupData->EMcaCSmiEn == 1) ) {
    ClearCoreSmiSrc(&McBankBitField);
  }
}

UINT64
ReadUncoreMcBankBitField(
VOID
)
{
  UINT64 McBankBitField = 0;
  UINT8 mcbank = 0;
  UINT16 mcscope = 0;
  UINT64 maxbank = 0;
  
//  if(isEMCAGen2Cap()) {
// Aptio V Server Override : Grantley support EMCAGEN2. When LastBootErrorHandler support is "on",
//   we should not depend on the MSR 0x17E (ReadUncoreSmiSrc). We should read all the BANKS to find out the error.
    if(isEMCAGen2Cap() && (mLastbootMemErrFlag == FALSE) && (mErrLogSetupData->EMcaLogEn != 0)) {  
    McBankBitField = ReadUncoreSmiSrc();          
  } else {
    maxbank = GetNumMcBanksPerLp();
    for (mcbank = 0, McBankBitField = 0; mcbank < maxbank; mcbank++) {
      mcscope = GetMcBankScope(mcbank);
      if (mcscope == MC_SCOPE_PKG) {
        McBankBitField |= (1ULL << mcbank);
      }
    }      
  }
  return McBankBitField;
}

VOID
ClearUncoreMcBankBitField(
  UINT64 McBankBitField
)
{
  if(isEMCAGen2Cap() && (mErrLogSetupData->MceSmiMorphingEn == 1 || mErrLogSetupData->EMcaCSmiEn == 1) ) {
    ClearUncoreSmiSrc(&McBankBitField);
  }
}



EFI_STATUS
FindFirstSet(
UINT64 McBankBitField,
UINT8 *Idx
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT64 i;
  
  if(McBankBitField != 0 || Idx != NULL) {
    Status = EFI_SUCCESS;
    i = 0;
    
    while((McBankBitField & 0x1) == 0) {
       McBankBitField >>= 1;
       i++;
    }
    
    *Idx = i;    
  } else {
   Status = EFI_INVALID_PARAMETER;
  }  
  return Status;
}

VOID
ProcessCoreUncorrectedError
(
  UINT8 *buffer
)
{
  UINT8 mcbank;
  UINT8 errorStsBlk[GES_SIZE];
  UINT64 McBankBitField;
  UINT32 ApicId;
  EFI_CPU_PHYSICAL_LOCATION Loc;
  UINTN cpunumber;
  BOOLEAN EmcaGen2Cap = FALSE;
  UINT8 skt;
  
  ApicId = GetApicID();
  ExtractProcessorLocation(ApicId,&Loc);
  skt = (UINT8)Loc.Package;
  cpunumber = GetCpuNumber(skt, (UINT8)Loc.Core, (UINT8)Loc.Thread);
  mSmmErrorMpSyncData.CpuData[cpunumber].SmmMceSyncState = SmmMceClearedState;
  mSmmErrorMpSyncData.CpuData[cpunumber].CoreProcessingBank = 0;

  McBankBitField = ReadCoreMcBankBitField();
  if( isEMCAGen2Cap() && (mErrLogSetupData->MceSmiMorphingEn == 1 || mErrLogSetupData->EMcaCSmiEn == 1))
     EmcaGen2Cap = TRUE;
  //EmcaGen2Cap = isEMCAGen2Cap();

  while(McBankBitField != 0) {
    FindFirstSet(McBankBitField,&mcbank);
    mSmmErrorMpSyncData.CpuData[cpunumber].CoreProcessingBank = (1ULL << mcbank);
    McBankBitField &= ~mSmmErrorMpSyncData.CpuData[cpunumber].CoreProcessingBank;
    ClearCoreMcBankBitField(mSmmErrorMpSyncData.CpuData[cpunumber].CoreProcessingBank);
    UncorrectedMCBankErrorHandler(mcbank,&skt, &mSmmErrorMpSyncData.CpuData[cpunumber], (GENERIC_ERROR_STATUS *)&errorStsBlk);
    mSmmErrorMpSyncData.CpuData[cpunumber].CoreProcessingBank = 0;
    
    if(EmcaGen2Cap) {
      McBankBitField = ReadCoreMcBankBitField();
    }   
  }
  ReleaseSpinLock(&mSmmErrorMpSyncData.CpuData[cpunumber].ApBusy);
}


VOID
ProcessUncoreUncorrectedError
(
  UINT8 *buffer
)
{
  UINT8 mcbank;
  UINT8 errorStsBlk[GES_SIZE];
  UINT64 McBankBitField;
  UINT32 ApicId;
  EFI_CPU_PHYSICAL_LOCATION Loc;
  UINTN cpunumber;
  BOOLEAN EmcaGen2Cap = FALSE;
  UINT8 skt;

  ApicId = GetApicID();
  ExtractProcessorLocation(ApicId,&Loc);
  skt = (UINT8)Loc.Package;
  cpunumber = GetCpuNumber(skt, (UINT8)Loc.Core, (UINT8)Loc.Thread);
  mSmmErrorMpSyncData.CpuData[cpunumber].SmmMceSyncState = SmmMceClearedState;
  mSmmErrorMpSyncData.CpuData[cpunumber].UncoreProcessingBank = 0;

  McBankBitField = ReadUncoreMcBankBitField();
  if( isEMCAGen2Cap() && (mErrLogSetupData->MceSmiMorphingEn == 1 || mErrLogSetupData->EMcaCSmiEn == 1))
     EmcaGen2Cap = TRUE;

  while(McBankBitField != 0) {
    FindFirstSet(McBankBitField,&mcbank);
    mSmmErrorMpSyncData.CpuData[cpunumber].UncoreProcessingBank =  (1ULL << mcbank);
    McBankBitField &= ~mSmmErrorMpSyncData.CpuData[cpunumber].UncoreProcessingBank;
    ClearUncoreMcBankBitField(mSmmErrorMpSyncData.CpuData[cpunumber].UncoreProcessingBank);
    UncorrectedMCBankErrorHandler(mcbank,&skt,&mSmmErrorMpSyncData.CpuData[cpunumber], (GENERIC_ERROR_STATUS *)&errorStsBlk);
    mSmmErrorMpSyncData.CpuData[cpunumber].UncoreProcessingBank = 0;
// AptioV Server Override Start: Grantley support EMCAGEN2,On current Boot and EMCA option enabled we should depend on the MSR 0x17E (ReadUncoreSmiSrc).
    if (mLastbootMemErrFlag == FALSE && mErrLogSetupData->EMcaLogEn == 1) 
    {
      if(EmcaGen2Cap) {
        McBankBitField = ReadUncoreMcBankBitField();
      }
    }
// AptioV Server Override End: Grantley support EMCAGEN2,On current Boot and EMCA option we should depend on the MSR 0x17E (ReadUncoreSmiSrc).
  }   
  ReleaseSpinLock(&mSmmErrorMpSyncData.CpuData[cpunumber].ApBusy);
}

EFI_STATUS
ProcessSocketUncorrectedError
(
IN UINT8 skt
)
{

  UINT8 core;
  UINTN cpuNumber;
  EFI_STATUS Status = EFI_SUCCESS;

  for (core = 0; core < MAXCOREID; core++) {
    if (ExcutingOnLocalNodeCore (skt, core) == TRUE) {
      ProcessCoreUncorrectedError(NULL);
    } else {
      if(mELogHost.cpuInfo[skt].Valid[core][0]) {
        cpuNumber = GetCpuNumber (skt, core, 0);
        AcquireSpinLock(&mSmmErrorMpSyncData.CpuData[cpuNumber].ApBusy);
        Status = mSmst->SmmStartupThisAp (ProcessCoreUncorrectedError,
                                 cpuNumber,
                                 NULL);
        if(Status != EFI_SUCCESS) {
          ReleaseSpinLock(&mSmmErrorMpSyncData.CpuData[cpuNumber].ApBusy);
        }
      }
    }
  }

  for (core = 0; core < MAXCOREID; core++) {
    if (ExcutingOnLocalNodeCore (skt, core) != TRUE) {
      if(mELogHost.cpuInfo[skt].Valid[core][0]) {
        cpuNumber = GetCpuNumber (skt, core, 0);
        AcquireSpinLock(&mSmmErrorMpSyncData.CpuData[cpuNumber].ApBusy);
        ReleaseSpinLock(&mSmmErrorMpSyncData.CpuData[cpuNumber].ApBusy);
      }
    }
  }

  //
  // Uncore uncorrected error
  //
  if (ExcutingOnLocalNode (skt) == TRUE) {
    ProcessUncoreUncorrectedError(NULL);
  } else {
    if(mELogHost.cpuInfo[skt].Valid[0][0]) {
      cpuNumber = GetCpuNumber (skt, 0, 0);
      AcquireSpinLock(&mSmmErrorMpSyncData.CpuData[cpuNumber].ApBusy);
      Status = mSmst->SmmStartupThisAp (ProcessUncoreUncorrectedError,
                               cpuNumber,
                               NULL);
      if(Status == EFI_SUCCESS) {
        AcquireSpinLock(&mSmmErrorMpSyncData.CpuData[cpuNumber].ApBusy);
        ReleaseSpinLock(&mSmmErrorMpSyncData.CpuData[cpuNumber].ApBusy);
      } else {
        ReleaseSpinLock(&mSmmErrorMpSyncData.CpuData[cpuNumber].ApBusy);        
      }      
    }
  }  

  return EFI_SUCCESS;
}


EFI_STATUS
ElogProcessorErrorHandler (
  VOID
)
/*++

Routine Description:

Arguments:

    None 

Returns:

    EFI_SUCCESS if the call is successful...

--*/
{
  UINT8   socket                 = 0;
  UINT32  umcSockets             = 0;
  UINT32  imcSockets             = 0;
  UINT32  imcNodesEventProgress  = 0;
  UINT32  imcNodesNewEvents      = 0;
//  BOOLEAN McErrFlag              = FALSE;
  UBOXERRSTS_UBOX_CFG_STRUCT uboxErrStsReg;
//  Aptio V Server Override Start: MemoryErrors are handled in below CorrectedMemoryErrorHandler().*/  
//  EFI_STATUS  Status = 0 ; 
  
  uboxErrStsReg.Data = 0;
/*  Aptio V Server Override Start: MemoryErrors are handled in below CorrectedMemoryErrorHandler().*/  
//  if (isEMCAGen2Cap())
//    Status = MemCorrErrorHanlder();
/*  Aptio V Server Override End: MemoryErrors are handled in below CorrectedMemoryErrorHandler().*/

  //
  // This handler is installed directly with SMM Base. The last thing we want is to get lost in
  // a protracted search for errors, thereby consuming excess time in SMM mode. Therefore, we need
  // to first quickly determine if we have a pending CPU error at all, and then kick-start the remainder
  // of core error handling part only when there is indeed an error...
  //
  // Aptio V Server Override Start: Changes done to support LastBoot Error Handling and Logging Uncorrectable Errors (Memory/PCIE ...)       
  if(mLastbootMemErrFlag == FALSE)
  {  
  	if (!PendingProcessorErrorExists (&umcSockets, &imcSockets, &imcNodesEventProgress, &imcNodesNewEvents)) {
    	RAS_DEBUG((2, "[RAS]PendingProcessorErrorExists - no error, exit!\n"));
    	return EFI_SUCCESS;
  	}
  }
// Aptio V Server Override End: Changes done to support LastBoot Error Handling and Logging Uncorrectable Errors (Memory/PCIE ...)       

    RAS_DEBUG((2, "[RAS]    PendingProcessorErrorExists - umcSockets:%x  imcSockets:%x  imcNodesEventProgress:%x  imcNodesNewEvents:%x \n",
    umcSockets,
    imcSockets,
    imcNodesEventProgress,
    imcNodesNewEvents));

  // Check if any socket got McError;
 // McErrFlag = McErrNodeFn();  For Haswell and IVYTown not needed

  for (socket = 0; socket < MAX_SOCKET; socket++) {

    //
    // Ignore disabled nodes
    //
    if ((mELogHost.socketPresentBitMap & (1 << socket)) == 0) {
      continue;
    }

    /*if ( (mErrLogSetupData->CorrectedMemoryErrors == 1) || (mErrLogSetupData->UncorrectedErrorsEn == 1 )) {
      VmseErrorHandler(socket);
    }*/
  // Aptio V Server Override Start: Changes done to support LastBoot Error Handling and Logging Uncorrectable Errors (Memory/PCIE ...)       
    //
    // Handle uncorrected error case
    //
    if ((/*McErrFlag == TRUE ||*/ mLastbootMemErrFlag == TRUE) /*&& (umcSockets & (1 << socket))*/) {
    	ProcessSocketUncorrectedError(socket);
    }
  // Aptio V Server Override End: Changes done to support LastBoot Error Handling and Logging Uncorrectable Errors (Memory/PCIE ...)       
 // AptioV Server  Override start: Fix for the EIP#205273

    if (mLastbootMemErrFlag == FALSE)
    {
    if ( (mErrLogSetupData->CorrectedMemoryErrors == 1) || ( mSpareIntSelect & SPARE_INT_SMI )) {
      CorrectedMemoryErrorHandler (socket, imcNodesEventProgress, imcNodesNewEvents);
    }
      else {
        DEBUG ((-1, "ElogProcessorErrorHandler : Call to CorrectedMemoryErrorHandler is disabled\n"));
      }
    }
  // AptioV Server  Override end: Fix for the EIP#205273
  }
  return EFI_SUCCESS;
}
