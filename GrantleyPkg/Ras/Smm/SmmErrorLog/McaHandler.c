/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

Copyright (c)2009-2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  McaErrorHandler.c

Abstract:

  Hooks for OEM error handler

------------------------------------------------------------------*/
#include "CommonErrorHandlerIncludes.h"

/**
  Prints a message to the serial port.

  @param  Format      Format string for the message to print.
  @param  ...         Variable argument list whose contents are accessed 
                      based on the format string specified by Format.

**/
VOID
EFIAPI
InternalPrintMessage1 (
  IN  CONST CHAR8  *Format,
  ...
  )
{
  CHAR8    Buffer[100];
  VA_LIST  Marker;

  //
  // Convert the message to an ASCII String
  //
  VA_START (Marker, Format);
  AsciiVSPrint (Buffer, sizeof (Buffer), Format, Marker);
  VA_END (Marker);

  //
  // Send the print string to a Serial Port 
  //
  AcquireSpinLock(&mSmmErrorMpSyncData.SerialLock);
  SerialPortWrite ((UINT8 *)Buffer, AsciiStrLen (Buffer));
  ReleaseSpinLock(&mSmmErrorMpSyncData.SerialLock);
}


UINTN
CountCpuEnabled(
VOID
)
{
  return mELogHost.cpuCountEnabled;
}

BOOLEAN
isMCIP(
VOID
)
{
  IA32_MCG_STATUS_MSR_STRUCT McgStatus;
  
  McgStatus.Data = AsmReadMsr32(IA32_MCG_STATUS);

  return (McgStatus.Bits.mcip == 1);
}

BOOLEAN
isRIPV(
VOID
)
{
  IA32_MCG_STATUS_MSR_STRUCT McgStatus;
  
  McgStatus.Data = AsmReadMsr32(IA32_MCG_STATUS);

  return (McgStatus.Bits.ripv== 1);
}


VOID
clearMCIP(
VOID
)
{
  IA32_MCG_STATUS_MSR_STRUCT McgStatus;
  McgStatus.Data = AsmReadMsr32(IA32_MCG_STATUS);
  McgStatus.Bits.mcip = 0;
  AsmWriteMsr32(IA32_MCG_STATUS,McgStatus.Data);  
}

BOOLEAN
SelectMonarch(
IN UINT32 *Semaphore,
OUT SMM_MCA_CPU_INFO *CpuInfo
)
{

  ASSERT(CpuInfo != NULL);
  
  CpuInfo->ApicId = GetApicID();
  ExtractProcessorLocation(CpuInfo->ApicId,&CpuInfo->Loc);
  CpuInfo->CpuIndex = GetCpuNumber((UINT8)CpuInfo->Loc.Package, (UINT8)CpuInfo->Loc.Core, (UINT8)CpuInfo->Loc.Thread);
  
  InterlockedCompareExchange32 (
              (UINT32*)Semaphore,
              (UINT32)-1, //-1 is expected to be the non-initialized apicid. 
              (UINT32)CpuInfo->CpuIndex 
              );

  return (*Semaphore == CpuInfo->CpuIndex);
}

VOID 
MceLogError(
UINT32 McBank,
SMM_MCA_CPU_INFO *CpuInfo,
GENERIC_ERROR_STATUS *ErrSts
)
{
  UINT8 skt;
  EMCA_MC_SIGNATURE_ERR_RECORD McErrSig;
  EFI_STATUS Status;
  IA32_MCI_STATUS_MSR_STRUCT MciStsMsr;
  UINT32 sev;
  UINT32 PrevSev;
  BOOLEAN UpdateSev = FALSE;

  skt = (UINT8)CpuInfo->Loc.Package;
  
  Status = ReadMcBankSigHook(McBank,skt,&McErrSig);
  ASSERT_EFI_ERROR(Status);
  MciStsMsr.Data = McErrSig.Signature.McSts;

  if(MciStsMsr.Bits.val != 1 || MciStsMsr.Bits.uc != 1 ) { 
    return; //Not valid error  end of the flow
  }
  sev = MapErrorSeverity(&McErrSig);

  InternalPrintMessage1("t %ld : MceLogError : skt = %d, McBank = %d, sev = %d\n", CpuInfo->CpuIndex, skt, McBank, sev);

  FillErrorSection(McBank, &skt, sev,&McErrSig,ErrSts);
  ConstructErrorRecord(sev, ErrSts);
  LogErrorRecord(McBank, &McErrSig, ErrSts);

  if(sev == EFI_ACPI_5_0_ERROR_SEVERITY_FATAL) {
     mSmmErrorMpSyncData.CpuData[CpuInfo->CpuIndex].Pcc = 1; 
  }

  if(mSmmErrorMpSyncData.InEmcaHander == FALSE) {

    if(sev == EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTABLE || sev == EFI_ACPI_5_0_ERROR_SEVERITY_FATAL) {
      mSmmErrorMpSyncData.MceSignal = TRUE;
    }
  } else {
    //Update severity 
    PrevSev = mSmmErrorMpSyncData.CpuData[CpuInfo->CpuIndex].EventLog.Events[McbankSrc].Severity;
    UpdateSev = (PrevSev == EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTED || 
                (PrevSev == EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTABLE && sev == EFI_ACPI_5_0_ERROR_SEVERITY_FATAL ));
    if(UpdateSev) {
      //InternalPrintMessage1("t %ld: Update Serverity\n",CpuInfo->CpuIndex);
      InterlockedCompareExchange32( (UINT32*) &mSmmErrorMpSyncData.CpuData[CpuInfo->CpuIndex].EventLog.Events[McbankSrc].Severity,
                                       PrevSev,
                                       sev
                                      );
      mSmmErrorMpSyncData.CpuData[CpuInfo->CpuIndex].EventLog.ErrorsFound= TRUE;
      mSmmErrorMpSyncData.CpuData[CpuInfo->CpuIndex].EventLog.Events[McbankSrc].ErrorFound = TRUE;
      mSmmErrorMpSyncData.CpuData[CpuInfo->CpuIndex].EventLog.Events[McbankSrc].Source = McbankSrc;
      mSmmErrorMpSyncData.CpuData[CpuInfo->CpuIndex].EventLog.Events[McbankSrc].ApicId = McErrSig.ApicId;
      mSmmErrorMpSyncData.CpuData[CpuInfo->CpuIndex].EventLog.Events[McbankSrc].BankIdx = McBank;
    }
  }

}


VOID
McaProcessCoreErrors(
  IN SMM_MCA_CPU_INFO *CpuInfo
)
{
  UINT64 CoreSmiSrc;
  UINTN cpunumber;
  UINT64 NewErrorsBitmap;
  UINT8 mcbank;
  UINT32 msraddr;
  IA32_MCI_STATUS_MSR_STRUCT MciStsMsr;
  UINT8 errorStsBlk[GES_SIZE];

  //Only Thread 0 needs to run the core handler
  if(CpuInfo->Loc.Thread != 0) {
    return;
  }
  cpunumber = CpuInfo->CpuIndex;
  //InternalPrintMessage1("t %ld: Core handler\n",cpunumber);
  CoreSmiSrc = ReadCoreSmiSrc();
  
  if((CoreSmiSrc & mSmmErrorMpSyncData.CpuData[cpunumber].CoreProcessingBank) != 0 && mSmmErrorMpSyncData.InEmcaHander == TRUE ) {
    //Mce interrupted smm handler processing for this bank
    switch(mSmmErrorMpSyncData.CpuData[cpunumber].SmmMceSyncState) {
      case SmmMceReadingState:
        //InternalPrintMessage1("t %ld: Reading Interrupted\n",cpunumber);
        mSmmErrorMpSyncData.CpuData[cpunumber].SmmMceSyncState = SmmMceReadingInterrupted;
        FindFirstSet(mSmmErrorMpSyncData.CpuData[cpunumber].CoreProcessingBank,&mcbank);
        MceLogError(mcbank,CpuInfo,(GENERIC_ERROR_STATUS *)&errorStsBlk);
        ClearCoreMcBankBitField(1ULL << mcbank); 
        break;
      case SmmMceProcessingState:
        //InternalPrintMessage1("t %ld: Processing Interrupted\n",cpunumber);
        mSmmErrorMpSyncData.CpuData[cpunumber].SmmMceSyncState = SmmMceProcessingInterrupted;
        FindFirstSet(mSmmErrorMpSyncData.CpuData[cpunumber].CoreProcessingBank,&mcbank);
        MceLogError(mcbank,CpuInfo,(GENERIC_ERROR_STATUS *)&errorStsBlk);
        ClearCoreMcBankBitField(1ULL << mcbank); 
        break;
      case SmmMceWritingState:
        //InternalPrintMessage1("t %ld: Writing Interrupted\n",cpunumber);
        mSmmErrorMpSyncData.CpuData[cpunumber].SmmMceSyncState = SmmMceWritingInterrupted;
        break;
      case SmmMceLoggingState:
      case SmmMceClearedState:
        //Do nothing
        break;
      default:
        ASSERT(FALSE);
        break;
    }    
  }

  NewErrorsBitmap = CoreSmiSrc;
  while(NewErrorsBitmap != 0) {
    FindFirstSet(NewErrorsBitmap,&mcbank);
    NewErrorsBitmap &= ~(1ULL << mcbank);
    msraddr = GetAddrMcBankSts(mcbank);
    MciStsMsr.Data = AsmReadMsr64(msraddr);
	InternalPrintMessage1("MCE Hand Core: SMIErrSrc %x, CpuNum = %d, McBank = %d, MCi_STS = %x\n",NewErrorsBitmap,cpunumber, mcbank,MciStsMsr.Data);
     if(MciStsMsr.Bits.val == 1 && MciStsMsr.Bits.uc == 1 && MciStsMsr.Bits.en == 1) {
      if(MciStsMsr.Bits.pcc == 1) {
		 mSmmErrorMpSyncData.CpuData[CpuInfo->CpuIndex].Pcc = 1;
        //InternalPrintMessage1("t %ld: New Errors had PCC\n",cpunumber);
      }
      if(mSmmErrorMpSyncData.InEmcaHander == FALSE) {
        //InternalPrintMessage1("t %ld: Log Err: bank = %d\n",cpunumber,mcbank);
        MceLogError(mcbank,CpuInfo,(GENERIC_ERROR_STATUS *)&errorStsBlk);
        ClearCoreMcBankBitField(1ULL << mcbank);        
      }
    }
  }      
}

VOID
McaProcessUncoreErrors(
  IN SMM_MCA_CPU_INFO *CpuInfo
)
{
  UINTN cpunumber;
  UINT64 UncoreSmiSrc;
  UINT64 NewErrorsBitmap;
  UINT8 mcbank;
  UINT32 msraddr;
  IA32_MCI_STATUS_MSR_STRUCT MciStsMsr;
  UINT8 errorStsBlk[GES_SIZE];

  //Only Thread 0 , core 0 needs to run the core handler
  if(CpuInfo->Loc.Thread != 0 || CpuInfo->Loc.Core != 0) {
    return;
  }
  cpunumber = CpuInfo->CpuIndex;
  //InternalPrintMessage1("t %ld: Uncore handler\n",cpunumber);
  UncoreSmiSrc = ReadUncoreSmiSrc();
  
  if((UncoreSmiSrc & mSmmErrorMpSyncData.CpuData[cpunumber].UncoreProcessingBank) != 0 && mSmmErrorMpSyncData.InEmcaHander == TRUE) {
    //Mce interrupted smm handler processing for this bank
    switch(mSmmErrorMpSyncData.CpuData[cpunumber].SmmMceSyncState) {
      case SmmMceReadingState:
        //InternalPrintMessage1("t %ld: Reading Interrupted\n",cpunumber);
        mSmmErrorMpSyncData.CpuData[cpunumber].SmmMceSyncState = SmmMceReadingInterrupted;
        FindFirstSet(mSmmErrorMpSyncData.CpuData[cpunumber].UncoreProcessingBank,&mcbank);
        MceLogError(mcbank,CpuInfo,(GENERIC_ERROR_STATUS *)&errorStsBlk);
        ClearUncoreMcBankBitField(1ULL << mcbank);
        break;
      case SmmMceProcessingState:
        //InternalPrintMessage1("t %ld: Processing Interrupted\n",cpunumber);
        mSmmErrorMpSyncData.CpuData[cpunumber].SmmMceSyncState = SmmMceProcessingInterrupted;
        FindFirstSet(mSmmErrorMpSyncData.CpuData[cpunumber].UncoreProcessingBank,&mcbank);
        MceLogError(mcbank,CpuInfo,(GENERIC_ERROR_STATUS *)&errorStsBlk);
        ClearUncoreMcBankBitField(1ULL << mcbank);
        break;
      case SmmMceWritingState:
        //InternalPrintMessage1("t %ld: Writing Interrupted\n",cpunumber);
        mSmmErrorMpSyncData.CpuData[cpunumber].SmmMceSyncState = SmmMceWritingInterrupted;
        break;
      case SmmMceLoggingState:
      case SmmMceClearedState:
        //Do nothing
        break;
      default:
        ASSERT(FALSE);
        break;
    }    
  }

  NewErrorsBitmap =  UncoreSmiSrc;
  while(NewErrorsBitmap != 0) {
    FindFirstSet(NewErrorsBitmap,&mcbank);
    NewErrorsBitmap &= ~(1ULL << mcbank);
    msraddr = GetAddrMcBankSts(mcbank);
    MciStsMsr.Data = AsmReadMsr64(msraddr);
	InternalPrintMessage1("MCEHand UnCore:SMIErrSrc %x,CpuNum = %d, McBank = %d, MCi_STS = %x\n",NewErrorsBitmap,cpunumber, mcbank,MciStsMsr.Data);
    if(MciStsMsr.Bits.val == 1 && MciStsMsr.Bits.uc == 1 && MciStsMsr.Bits.en == 1) {
      if(MciStsMsr.Bits.pcc == 1) {
	    mSmmErrorMpSyncData.CpuData[CpuInfo->CpuIndex].Pcc = 1;
        //InternalPrintMessage1("t %ld: New Errors had PCC\n",cpunumber);
      }
      if(mSmmErrorMpSyncData.InEmcaHander == FALSE) {
        //InternalPrintMessage1("t %ld: Log Err: bank = %d\n",cpunumber,mcbank);
        MceLogError(mcbank,CpuInfo,(GENERIC_ERROR_STATUS *)&errorStsBlk);
        ClearUncoreMcBankBitField(1ULL << mcbank);        
      }
    }
  }      
}

UINT64   mTimeoutTicker = 0;
//
//  Number of counts in a roll-over cycle of the performance counter.
//
UINT64   mCycle = 0;
//
// Flag to indicate the performance counter is count-up or count-down.
//
BOOLEAN  mCountDown;

/**
  Initialize Timer for Smm AP Sync.
  
**/
VOID
InitializeSmmTimer (
  VOID
  )
{
  UINT64  TimerFrequency;
  UINT64  Start;
  UINT64  End;

  TimerFrequency = GetPerformanceCounterProperties (&Start, &End);
  mTimeoutTicker = DivU64x32 (
                     MultU64x64(TimerFrequency, MceHndApTimeOut),
                     1000 * 1000
                     ); 
  if (End < Start) {
    mCountDown = TRUE;
    mCycle = Start - End;
  } else {
    mCountDown = FALSE;
    mCycle = End - Start;
  }
}


/**
  Start Timer for Smm AP Sync.
  
**/
UINT64
EFIAPI
StartSyncTimer (
  VOID
  )
{
  return GetPerformanceCounter ();
}


/**
  Check if the Smm AP Sync timer is timeout.
  
  @param Timer  The start timer from the begin.

**/
BOOLEAN
EFIAPI
IsSyncTimerTimeout (
  IN      UINT64                    Timer
  )
{
  UINT64  CurrentTimer;
  UINT64  Delta;

  CurrentTimer = GetPerformanceCounter ();

  if (mCountDown) {
    //
    // The performance counter counts down.  Check for roll over condition.
    //
    if (CurrentTimer < Timer) {
      Delta = Timer - CurrentTimer;
    } else {
      //
      // Handle one roll-over. 
      //
      Delta = mCycle - (CurrentTimer - Timer);
    }
  } else {
    //
    // The performance counter counts up.  Check for roll over condition.
    //
    if (CurrentTimer > Timer) {
      Delta = CurrentTimer - Timer;
    } else {
      //
      // Handle one roll-over. 
      //
      Delta = mCycle - (Timer - CurrentTimer);
    }
  }
 
  return (BOOLEAN) (Delta >= mTimeoutTicker);
}


VOID
WaitForApCheckIn(
VOID
)
{
  UINT64 Timer;
  
  //
  // Sync with APs 1st timeout
  //
  for (Timer = StartSyncTimer ();
       !IsSyncTimerTimeout (Timer) &&
        mSmmErrorMpSyncData.Counter < (CountCpuEnabled() - 1);
       ) {
    CpuPause ();
  }
       
  //InternalPrintMessage1("WaitForApCheckIn - Counter: %d \n",mSmmErrorMpSyncData.Counter);
  if(mSmmErrorMpSyncData.Counter != (CountCpuEnabled() - 1)) {
    InternalPrintMessage1("WaitForApCheckIn - Timeout\n");
    InternalPrintMessage1("WaitForApCheckIn - Counter: %d \n",mSmmErrorMpSyncData.Counter);
  }
  
}

VOID
WaitForApCheckOut(
  VOID
)
{
  UINT64 Timer;
  UINTN  Timeouts = 0;

   //
  // Sync with APs 1st timeout
  //
#if 0
  for (Timer = StartSyncTimer ();
       !IsSyncTimerTimeout (Timer) &&
        mSmmErrorMpSyncData.ApExec < mSmmErrorMpSyncData.Counter;
       ) {
    CpuPause ();
  }
#endif

  for (Timer = StartSyncTimer ();
        mSmmErrorMpSyncData.ApExec <= mSmmErrorMpSyncData.Counter;
       ) {
    if(IsSyncTimerTimeout (Timer)) {
      Timeouts++;
      Timer = StartSyncTimer ();
    }
    CpuPause ();
  }
  //InternalPrintMessage1("WaitForApCheckOut - Timeouts: %ld \n",Timeouts);

       
  //InternalPrintMessage1("WaitForApCheckOut - ApExec: %d \n",mSmmErrorMpSyncData.ApExec);
  //if(mSmmErrorMpSyncData.ApExec != mSmmErrorMpSyncData.Counter) {
  //  InternalPrintMessage1("WaitForApCheckOut - Timeout\n");
  //}
  //InternalPrintMessage1("WaitForApCheckOut - ApExec: %d \n",mSmmErrorMpSyncData.ApExec);
  
}

UINTN
AtomicIncrement(
IN OUT volatile UINT32 *Sem
)
{
  UINT32 Value;
  
  do {
    Value = *Sem;
  } while(InterlockedCompareExchange32(
          (UINT32*)Sem,
          Value,
          Value + 1
          ) != Value);
  return Value + 1;  
}

UINT32
CheckIn(
IN OUT volatile UINT32 *Sem
)
{
  return AtomicIncrement(Sem);
}

VOID
McaSignalMce(
  IN SMM_MCA_CPU_INFO *CpuInfo
)
{

  UINT32 SaveStateType;
  UINTN LogicalCpu;
  MCEMSR_ARGS_STRUCT MceArgs;

  MceArgs.LogicalCpu = CpuInfo->CpuIndex;
  MceArgs.Sync = FALSE;
  SaveStateType = GetSaveStateType();
  if(SaveStateType == 0) {
    if(mSmmErrorMpSyncData.MonarchIndex == CpuInfo->CpuIndex) {
      for(LogicalCpu = 0; LogicalCpu <  mSmst->NumberOfCpus; LogicalCpu++) {
        SignalMceMmio(LogicalCpu);
      }
    }
  } else {
    SignalMceMsr(&MceArgs);
  }
}

VOID
McaMonarchHandler(
IN SMM_MCA_CPU_INFO *CpuInfo
)
{
  //InternalPrintMessage1("Monarch %ld \n",CpuInfo->CpuIndex);
  WaitForApCheckIn();
  mSmmErrorMpSyncData.MceSignal = FALSE;
  mSmmErrorMpSyncData.MceInProgress = TRUE;
  
  mSmmErrorMpSyncData.CpuData[CpuInfo->CpuIndex].Pcc = 0;           // Initialize pcc to 0 before core/uncore updates it 
  McaProcessCoreErrors(CpuInfo);
  McaProcessUncoreErrors(CpuInfo);

  mSmmErrorMpSyncData.CpuData[CpuInfo->CpuIndex].Ripv = isRIPV();

  mSmmErrorMpSyncData.ApExec++;
  WaitForApCheckOut();
  if (mSmmErrorMpSyncData.CpuData[CpuInfo->CpuIndex].SmmMceSyncState == SmmMceWritingInterrupted ) { 
    	   InternalPrintMessage1("t %ld: MceReset Monarch Writing interrupted \n",CpuInfo->CpuIndex);
	   // PCH team need to fix code and warm reset sequnce (Granley HSD 4987629, 
       // till then we use the ResetCold() as it is doing the warm reset sequnce
       // ResetWarm();
       ResetCold();
  
  }
  
  if (mSmmErrorMpSyncData.CpuData[CpuInfo->CpuIndex].Ripv == 0 ) { 
	   InternalPrintMessage1("t %ld: Monarch MceReset RIPV = 0 \n",CpuInfo->CpuIndex);
	   // PCH team need to fix code and warm reset sequnce (Granley HSD 4987629, 
       // till then we use the ResetCold() as it is doing the warm reset sequnce
       // ResetWarm();
       ResetCold();
  } else {        // return back 
      if(mSmmErrorMpSyncData.MceSignal) {
        InternalPrintMessage1("t %ld: Mce\n",CpuInfo->CpuIndex);
        McaSignalMce(CpuInfo); 
      }
      InternalPrintMessage1("t %ld: MceRestart\n",CpuInfo->CpuIndex);
      clearMCIP();
      mSmmErrorMpSyncData.Counter = 0;
      mSmmErrorMpSyncData.ApExec = 0;
      mSmmErrorMpSyncData.MonarchIndex = (UINT32)-1;
      mSmmErrorMpSyncData.MceInProgress = FALSE;
   } 
}

VOID
McaApHandler(
  IN SMM_MCA_CPU_INFO *CpuInfo
)
{
  UINT32 CheckInCnt;
  UINT64 Timer;

  CheckInCnt = CheckIn(&mSmmErrorMpSyncData.Counter);
  
  //wait for monarch to set the MceInProgress flag
  while(mSmmErrorMpSyncData.MceInProgress == FALSE){
    CpuPause();  
  }

  while(mSmmErrorMpSyncData.ApExec != CheckInCnt) {
    CpuPause();
  }
    
  mSmmErrorMpSyncData.CpuData[CpuInfo->CpuIndex].Pcc = 0;    // Initalize PCC nefore calling the core/uncore banks
  //InternalPrintMessage1("t %ld: CheckinCnt %d\n",CpuInfo->CpuIndex,CheckInCnt);
  
  mSmmErrorMpSyncData.CpuData[CpuInfo->CpuIndex].Ripv = isRIPV();
  McaProcessCoreErrors(CpuInfo);
  McaProcessUncoreErrors(CpuInfo);

  mSmmErrorMpSyncData.ApExec++;
  
  while(mSmmErrorMpSyncData.MceInProgress == TRUE) {
    CpuPause();
  }

      if(mSmmErrorMpSyncData.MceSignal) {
        InternalPrintMessage1("t %ld:Mce\n",CpuInfo->CpuIndex);
        McaSignalMce(CpuInfo);  
      }
 if (mSmmErrorMpSyncData.CpuData[CpuInfo->CpuIndex].SmmMceSyncState == SmmMceWritingInterrupted ) { 
    	   InternalPrintMessage1("t %ld: MceReset Writing interrupted \n",CpuInfo->CpuIndex);
	   // PCH team need to fix code and warm reset sequnce (Granley HSD 4987629, 
       // till then we use the ResetCold() as it is doing the warm reset sequnce
       // ResetWarm();
       ResetCold();
    
  }
  if (mSmmErrorMpSyncData.CpuData[CpuInfo->CpuIndex].Ripv == 0 ) { 
	 InternalPrintMessage1("t %ld: MCE AP in Spin loop for Monarch Reset\n",CpuInfo->CpuIndex);
	  for (Timer = StartSyncTimer ();
		
             !IsSyncTimerTimeout (Timer) ;
          ) {
          CpuPause ();
          }
          InternalPrintMessage1("t %ld: MceReset AP RIPV =0  \n",CpuInfo->CpuIndex);
	  // PCH team need to fix code and warm reset sequnce (Granley HSD 4987629, 
          // till then we use the ResetCold() as it is doing the warm reset sequnce
          // ResetWarm();
          ResetCold();
   } else {     // Resume to SMM handler 
      clearMCIP();
  }  
}


/**
  MCA handler for SMM use.

**/
VOID
EFIAPI
SmiMcaHandler (
  IN EFI_EXCEPTION_TYPE   InterruptType,
  IN EFI_SYSTEM_CONTEXT   SystemContext
)
{
  SMM_MCA_CPU_INFO CpuInfo;
  
  if(GetApicID() == 0) {
    InternalPrintMessage1("MCAHANDLER\n");
  }

  if(isMCIP() == FALSE) {
     InternalPrintMessage1("In Platform MCE Handler MCIP = 0\n");
	 ASSERT(FALSE);
    return;
  }

  InitializeSmmTimer();

  ASSERT(isEMCAGen2Cap() == TRUE); 

  if(SelectMonarch((UINT32*)&mSmmErrorMpSyncData.MonarchIndex,&CpuInfo)) {
    McaMonarchHandler(&CpuInfo); 
  } else {
    McaApHandler(&CpuInfo);
  }
}

