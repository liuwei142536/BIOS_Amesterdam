/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed  
  for Intel CPUs and Chipsets under the terms of your license   
  agreement with Intel or your vendor.  This file may be        
  modified by the user, subject to additional terms of the      
  license agreement                                             
--*/
/** @file
Implementation of MP CPU driver for PEI phase.

This PEIM is to expose the MpService Ppi

  Copyright (c) 2012, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.                                       

**/

#include "CpuMpPei.h"
#include "MpCommon.h"

//
// Used to point to the count the APs executing AP Count function
//
volatile UINT32       *CpusExecutingCount;


typedef struct {
  UINTN            SpinLock;
  PEI_MP_HOB_DATA  MpHob;
} MP_WAKEUP_DATA;

//
// Global Descriptor Table (GDT)
//
GLOBAL_REMOVE_IF_UNREFERENCED IA32_GDT mGdtEntries[] = {
/* selector { Global Segment Descriptor                              } */
/* 0x00 */  {{0,      0,  0,  0,    0,  0,  0,  0,    0,  0, 0,  0,  0}},
/* 0x08 */  {{0,      0,  0,  0,    0,  0,  0,  0,    0,  0, 0,  0,  0}},
/* 0x10 */  {{0xFFFF, 0,  0,  0xB,  1,  0,  1,  0xF,  0,  0, 1,  1,  0}},
/* 0x18 */  {{0xFFFF, 0,  0,  0x3,  1,  0,  1,  0xF,  0,  0, 1,  1,  0}},
/* 0x20 */  {{0,      0,  0,  0,    0,  0,  0,  0,    0,  0, 0,  0,  0}},
/* 0x28 */  {{0xFFFF, 0,  0,  0xB,  1,  0,  1,  0xF,  0,  0, 0,  1,  0}},
/* 0x30 */  {{0xFFFF, 0,  0,  0x3,  1,  0,  1,  0xF,  0,  0, 0,  1,  0}},
/* 0x38 */  {{0xFFFF, 0,  0,  0xB,  1,  0,  1,  0xF,  0,  1, 0,  1,  0}},
/* 0x40 */  {{0,      0,  0,  0,    0,  0,  0,  0,    0,  0, 0,  0,  0}},
};

//
// IA32 Gdt register
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST IA32_DESCRIPTOR mGdt = {
  sizeof (mGdtEntries) - 1,
  (UINTN) mGdtEntries
  };

EFI_GUID  mPeiMpHobGuid = PEI_MP_HOB_GUID;

/**
  This function will be called in AP, if BSP uses WakeUpAP.

  @param ExchangeInfo  Pointer to the exchange info buffer for output.
**/
VOID
EFIAPI
ApCFunction (
  IN MP_CPU_EXCHANGE_INFO      *ExchangeInfo
  )
{
  EFI_AP_PROCEDURE          Procedure;

  if (ExchangeInfo->ApFunction != 0) {
    Procedure = (EFI_AP_PROCEDURE)(UINTN)ExchangeInfo->ApFunction;
    Procedure ((VOID *)(UINTN)ExchangeInfo->ApFunctionArgument);
  }

  InterlockedIncrement ((UINT32 *)&ExchangeInfo->FinishedCount);

  //
  // AP is finished executing C code, decrement the value.
  //
  InterlockedDecrement ((UINT32 *)&ExchangeInfo->NumApsExecuting);

  AsmCliHltLoop ();
  return ;
}

VOID
BackupWakeUpBuffer (
  IN UINT32            SavedWakeUpBuffer 
  )
{
  CopyMem ((VOID *) (UINTN) SavedWakeUpBuffer, (VOID *) (UINTN) CPU_WAKEUP_BUFFER_ADDRESS, CPU_WAKEUP_BUFFER_SIZE);
}

VOID
RestoreWakeUpBuffer (
  IN UINT32            SavedWakeUpBuffer 
  )
{
  CopyMem ((VOID *) (UINTN) CPU_WAKEUP_BUFFER_ADDRESS, (VOID *) (UINTN) SavedWakeUpBuffer, CPU_WAKEUP_BUFFER_SIZE);
}

/**
  This function will be called by BSP to wakeup AP.

  @param ApicId             APIC ID for the processor to be waked.
                            0xFFFFFFFF means to wake up all APs.
  @param Procedure          The function to be invoked by AP.
  @param ProcedureArgument  The argument to be passed into AP function.
  @param FinishedCount      The pointer of pointer for finished count
**/
VOID
WakeUpAP (
  IN UINT32                    ApicId,
  IN EFI_AP_PROCEDURE          Procedure,
  IN EFI_PHYSICAL_ADDRESS      Buffer,
  IN VOID                      *ProcedureArgument      OPTIONAL,
  OUT volatile UINT32          **FinishedCount         OPTIONAL
  )
{
  UINT32                           WakeUpBuffer;
  UINT32                           StackAddress;
  volatile MP_CPU_EXCHANGE_INFO    *ExchangeInfo;

  WakeUpBuffer = CPU_WAKEUP_BUFFER_ADDRESS;
  StackAddress = (UINT32) Buffer + CPU_WAKEUP_BUFFER_SIZE;
  CopyMem ((VOID *) (UINTN) WakeUpBuffer, AsmGetAddressMap (), CPU_WAKEUP_BUFFER_SIZE - 0x200);

  ExchangeInfo                        = (MP_CPU_EXCHANGE_INFO *) (UINTN) (WakeUpBuffer + (CPU_WAKEUP_BUFFER_SIZE - 0x200));

  ExchangeInfo->Lock                  = 0;
  ExchangeInfo->StackStart            = (UINT32) StackAddress;
  ExchangeInfo->StackSize             = STACK_SIZE_PER_PROC;
  ExchangeInfo->ApFunction            = (UINT32)(UINTN)Procedure;
  ExchangeInfo->ApFunctionArgument    = (UINT32)(UINTN)ProcedureArgument;
  ExchangeInfo->BufferStart           = (UINT32) WakeUpBuffer;
  ExchangeInfo->PmodeOffset           = (UINT32) (AsmGetPmodeOffset ());
  ExchangeInfo->SemaphoreCheck        = (VOID (*) (UINT32 *)) (AsmGetSemaphoreCheckOffset () + (UINT32) WakeUpBuffer);
  ExchangeInfo->FinishedCount         = (UINT32) 0;
  ExchangeInfo->SerializeLock         = (UINT32) 0;
  ExchangeInfo->StartState            = (UINT32) 0;
  ExchangeInfo->CFunction             = (UINT32)(UINTN)ApCFunction;
  ExchangeInfo->NumApsExecuting       = (UINT32) 0;
  
  //
  // Point the local global pointer to the memory used in the wakeup buffer
  //
  CpusExecutingCount = &ExchangeInfo->NumApsExecuting;

  CopyMem ((VOID *)&ExchangeInfo->GdtrProfile, &mGdt, sizeof(mGdt));
  AsmReadIdtr ((IA32_DESCRIPTOR *)&ExchangeInfo->IdtrProfile);

  if (ApicId != 0xFFFFFFFF) {
    SendInitSipiSipi (ApicId, WakeUpBuffer);
  } else {
    SendInitSipiSipiAllExcludingSelf (WakeUpBuffer);
  }

  if (FinishedCount != NULL) {
    *FinishedCount = &ExchangeInfo->FinishedCount;
  }

  return ;
}

/**
  This function will be called by AP function to collect CPU information.

  @param[in,out] Buffer  The pointer to private data buffer.
                         Here it is MP_WAKEUP_DATA.
**/
VOID
EFIAPI
CountCpuNumberApFunc (
  IN OUT VOID  *Buffer
  )
{
  MP_WAKEUP_DATA    *MpWakeupData;

  MpWakeupData = Buffer;

  AcquireSpinLock (&MpWakeupData->SpinLock);

  MpWakeupData->MpHob.CpuData[MpWakeupData->MpHob.CpuCount].ApicId = GetApicId ();

  InterlockedIncrement (&MpWakeupData->MpHob.CpuCount);

  ReleaseSpinLock (&MpWakeupData->SpinLock);
}

/**
  This function will get CPU count from system,
  and build Hob to save the data.

  @param MaxCpuCount  The MaxCpuCount could be supported by system
**/
VOID
CountCpuNumber (
  IN UINTN  MaxCpuCount
  )
{
  volatile MP_WAKEUP_DATA    MpWakeupData;
  UINTN                      Index;
  EFI_PHYSICAL_ADDRESS       Buffer;
  EFI_STATUS                 Status;

  ASSERT (MaxCpuCount <= FixedPcdGet32(PcdCpuMaxLogicalProcessorNumber));

  //
  // Fill BSP info
  //
  ZeroMem ((VOID *)&MpWakeupData, sizeof(MpWakeupData));
  InitializeSpinLock (&MpWakeupData.SpinLock);
  MpWakeupData.MpHob.CpuCount = 1;
  MpWakeupData.MpHob.CpuData[0].ApicId = GetApicId ();
  *(UINT32 *)&MpWakeupData.MpHob.CpuData[0].Health = 0;

  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES (STACK_SIZE_PER_PROC * MaxCpuCount + CPU_WAKEUP_BUFFER_SIZE),
             &Buffer
             );
  ASSERT_EFI_ERROR (Status);
  MpWakeupData.MpHob.Buffer = Buffer;

  BackupWakeUpBuffer ((UINT32)Buffer);
  //
  // Wake up AP
  //
  WakeUpAP (0xFFFFFFFF, CountCpuNumberApFunc, Buffer, (VOID *)&MpWakeupData, NULL);

  //
  // Give time for first AP to get to AP Count function
  //
  MicroSecondDelay (CPU_CHECK_AP_INTERVAL);
  
  //
  // Wait until all APs have executed before continuing
  //
  while (*CpusExecutingCount != 0);

  RestoreWakeUpBuffer ((UINT32)Buffer);

  //
  // DumpInfo
  //
  DEBUG ((EFI_D_INFO, "CpuMpPeim: CPU Count - 0x%08x\n", MpWakeupData.MpHob.CpuCount));
  for (Index = 0; Index < MpWakeupData.MpHob.CpuCount; Index++) {
    //DEBUG ((EFI_D_INFO, "CpuMpPeim: APICID - 0x%08x\n", MpWakeupData.MpHob.CpuData[Index].ApicId));
  }

  //
  // Record data
  //
  BuildGuidDataHob (
    &mPeiMpHobGuid,
    (VOID *)&MpWakeupData.MpHob,
    sizeof(MpWakeupData.MpHob)
    );
}

/**
  This service retrieves the number of logical processor in the platform
  and the number of those logical processors that are enabled on this boot.
  This service may only be called from the BSP.

  This function is used to retrieve the following information:
    - The number of logical processors that are present in the system.
    - The number of enabled logical processors in the system at the instant 
      this call is made.

  Because MP Service Ppi provides services to enable and disable processors 
  dynamically, the number of enabled logical processors may vary during the 
  course of a boot session.
  
  If this service is called from an AP, then EFI_DEVICE_ERROR is returned. 
  If NumberOfProcessors or NumberOfEnabledProcessors is NULL, then 
  EFI_INVALID_PARAMETER is returned. Otherwise, the total number of processors 
  is returned in NumberOfProcessors, the number of currently enabled processor 
  is returned in NumberOfEnabledProcessors, and EFI_SUCCESS is returned.

  @param[in]  PeiServices                 General purpose services available to every PEIM.
  @param[in]  This                        A pointer to the PEI_MP_SERVICES_PPI
                                          instance.
  @param[out] NumberOfProcessors          Pointer to the total number of logical
                                          processors in the system, including the BSP
                                          and disabled APs.
  @param[out] NumberOfEnabledProcessors   Pointer to the number of enabled logical
                                          processors that exist in system, including
                                          the BSP.

  @retval EFI_SUCCESS             The number of logical processors and enabled 
                                  logical processors was retrieved.
  @retval EFI_DEVICE_ERROR        The calling processor is an AP.
  @retval EFI_INVALID_PARAMETER   NumberOfProcessors is NULL.
  @retval EFI_INVALID_PARAMETER   NumberOfEnabledProcessors is NULL.

**/
EFI_STATUS
EFIAPI
PeiGetNumberOfProcessors (
  IN  CONST EFI_PEI_SERVICES    **PeiServices,
  IN  PEI_MP_SERVICES_PPI       *This,
  OUT UINTN                     *NumberOfProcessors,
  OUT UINTN                     *NumberOfEnabledProcessors
  )
{
  EFI_HOB_GUID_TYPE       *GuidHob;
  PEI_MP_HOB_DATA         *DataInHob;
  UINTN                   ProcessorNumber;
  UINTN                   EnabledProcessorNumber;
  UINTN                   Index;

  if ((NumberOfProcessors == NULL) || (NumberOfEnabledProcessors == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  GuidHob = GetFirstGuidHob (&mPeiMpHobGuid);
  if (GuidHob == NULL) {
    return EFI_UNSUPPORTED;
  }
  DataInHob       = GET_GUID_HOB_DATA (GuidHob);
  ProcessorNumber = DataInHob->CpuCount;

  EnabledProcessorNumber = 0;
  for (Index = 0; Index < ProcessorNumber; Index++) {
    if (DataInHob->CpuData[Index].Health.Uint32 == 0) {
      EnabledProcessorNumber ++;
    }
  }

  *NumberOfProcessors = ProcessorNumber;
  *NumberOfEnabledProcessors = EnabledProcessorNumber;

  return EFI_SUCCESS;
}

/**
  Gets detailed MP-related information on the requested processor at the
  instant this call is made. This service may only be called from the BSP.

  This service retrieves detailed MP-related information about any processor 
  on the platform. Note the following:
    - The processor information may change during the course of a boot session.
    - The information presented here is entirely MP related.
  
  Information regarding the number of caches and their sizes, frequency of operation,
  slot numbers is all considered platform-related information and is not provided 
  by this service.

  @param[in]  PeiServices           General purpose services available to every PEIM.
  @param[in]  This                  A pointer to the PEI_MP_SERVICES_PPI
                                    instance.
  @param[in]  ProcessorNumber       The handle number of processor.
  @param[out] ProcessorInfoBuffer   A pointer to the buffer where information for
                                    the requested processor is deposited.

  @retval EFI_SUCCESS             Processor information was returned.
  @retval EFI_DEVICE_ERROR        The calling processor is an AP.
  @retval EFI_INVALID_PARAMETER   ProcessorInfoBuffer is NULL.
  @retval EFI_NOT_FOUND           The processor with the handle specified by
                                  ProcessorNumber does not exist in the platform.

**/
EFI_STATUS
EFIAPI
PeiGetProcessorInfo (
  IN  CONST EFI_PEI_SERVICES     **PeiServices,
  IN  PEI_MP_SERVICES_PPI        *This,
  IN  UINTN                      ProcessorNumber,
  OUT EFI_PROCESSOR_INFORMATION  *ProcessorInfoBuffer
  )
{
  EFI_HOB_GUID_TYPE       *GuidHob;
  PEI_MP_HOB_DATA         *DataInHob;

  if (ProcessorInfoBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  GuidHob = GetFirstGuidHob (&mPeiMpHobGuid);
  if (GuidHob == NULL) {
    return EFI_UNSUPPORTED;
  }
  DataInHob       = GET_GUID_HOB_DATA (GuidHob);
  if (ProcessorNumber >= DataInHob->CpuCount) {
    return EFI_NOT_FOUND;
  }

  //
  // TBD: Need send StartAp to collect info
  //
  ProcessorInfoBuffer->ProcessorId = (UINT64)DataInHob->CpuData[ProcessorNumber].ApicId;
  ProcessorInfoBuffer->StatusFlag  = 0;
  if (DataInHob->CpuData[ProcessorNumber].ApicId == GetApicId()) {
    ProcessorInfoBuffer->StatusFlag |= PROCESSOR_AS_BSP_BIT;
  }
  if (DataInHob->CpuData[ProcessorNumber].Health.Uint32 == 0) {
    ProcessorInfoBuffer->StatusFlag |= PROCESSOR_ENABLED_BIT;
    ProcessorInfoBuffer->StatusFlag |= PROCESSOR_HEALTH_STATUS_BIT;
  }
  //
  // BUGBUG: Need more accurate algo to decide it. Below code is for Demo only.
  //
  ProcessorInfoBuffer->Location.Package = 0;
  ProcessorInfoBuffer->Location.Core    = DataInHob->CpuData[ProcessorNumber].ApicId >> 1;
  ProcessorInfoBuffer->Location.Thread  = DataInHob->CpuData[ProcessorNumber].ApicId & 0x1;

  return EFI_SUCCESS;
}

/**
  This service executes a caller provided function on all enabled APs. APs can 
  run either simultaneously or one at a time in sequence. This service supports 
  both blocking requests only. This service may only 
  be called from the BSP.

  This function is used to dispatch all the enabled APs to the function specified 
  by Procedure.  If any enabled AP is busy, then EFI_NOT_READY is returned 
  immediately and Procedure is not started on any AP.

  If SingleThread is TRUE, all the enabled APs execute the function specified by 
  Procedure one by one, in ascending order of processor handle number. Otherwise, 
  all the enabled APs execute the function specified by Procedure simultaneously.

  If the timeout specified by TimeoutInMicroseconds expires before all APs return 
  from Procedure, then Procedure on the failed APs is terminated. All enabled APs 
  are always available for further calls to PEI_MP_SERVICES_PPI.StartupAllAPs()
  and PEI_MP_SERVICES_PPI.StartupThisAP(). If FailedCpuList is not NULL, its 
  content points to the list of processor handle numbers in which Procedure was 
  terminated.

  Note: It is the responsibility of the consumer of the PEI_MP_SERVICES_PPI.StartupAllAPs() 
  to make sure that the nature of the code that is executed on the BSP and the 
  dispatched APs is well controlled. The MP Services Ppi does not guarantee 
  that the Procedure function is MP-safe. Hence, the tasks that can be run in 
  parallel are limited to certain independent tasks and well-controlled exclusive 
  code. PEI services and Ppis may not be called by APs unless otherwise 
  specified.

  In blocking execution mode, BSP waits until all APs finish or 
  TimeoutInMicroSeconds expires.

  @param[in]  PeiServices             General purpose services available to every PEIM.
  @param[in]  This                    A pointer to the PEI_MP_SERVICES_PPI
                                      instance.
  @param[in]  Procedure               A pointer to the function to be run on 
                                      enabled APs of the system. See type
                                      EFI_AP_PROCEDURE.
  @param[in]  SingleThread            If TRUE, then all the enabled APs execute 
                                      the function specified by Procedure one by 
                                      one, in ascending order of processor handle 
                                      number.  If FALSE, then all the enabled APs 
                                      execute the function specified by Procedure
                                      simultaneously.
  @param[in]  TimeoutInMicrosecsond   Indicates the time limit in microseconds for 
                                      APs to return from Procedure, for 
                                      blocking mode only. Zero means 
                                      infinity.  If the timeout expires before 
                                      all APs return from Procedure, then Procedure
                                      on the failed APs is terminated. All enabled 
                                      APs are available for next function assigned 
                                      by PEI_MP_SERVICES_PPI.StartupAllAPs() 
                                      or PEI_MP_SERVICES_PPI.StartupThisAP().
                                      If the timeout expires in blocking mode, 
                                      BSP returns EFI_TIMEOUT.
  @param[in]  ProcedureArgument       The parameter passed into Procedure for 
                                      all APs.

  @retval EFI_SUCCESS             In blocking mode, all APs have finished before 
                                  the timeout expired.
  @retval EFI_DEVICE_ERROR        Caller processor is AP.
  @retval EFI_NOT_STARTED         No enabled APs exist in the system.
  @retval EFI_NOT_READY           Any enabled APs are busy.
  @retval EFI_TIMEOUT             In blocking mode, the timeout expired before 
                                  all enabled APs have finished.
  @retval EFI_INVALID_PARAMETER   Procedure is NULL.

**/
EFI_STATUS
EFIAPI
PeiStartupAllAPs (
  IN  CONST EFI_PEI_SERVICES    **PeiServices,
  IN  PEI_MP_SERVICES_PPI       *This,
  IN  EFI_AP_PROCEDURE          Procedure,
  IN  BOOLEAN                   SingleThread,
  IN  UINTN                     TimeoutInMicroSeconds,
  IN  VOID                      *ProcedureArgument      OPTIONAL
  )
{
  EFI_HOB_GUID_TYPE       *GuidHob;
  PEI_MP_HOB_DATA         *DataInHob;
  UINTN                   ProcessorNumber;
  UINTN                   Index;
  UINTN                   BspIndex;
  volatile UINT32         *FinishedCount;
  EFI_STATUS              Status;
  UINTN                   WaitCountIndex;
  UINTN                   WaitCountNumber;

  GuidHob = GetFirstGuidHob (&mPeiMpHobGuid);
  if (GuidHob == NULL) {
    return EFI_UNSUPPORTED;
  }
  DataInHob       = GET_GUID_HOB_DATA (GuidHob);
  ProcessorNumber = DataInHob->CpuCount;
  if (ProcessorNumber <= 1) {
    return EFI_UNSUPPORTED;
  }

  WaitCountNumber = TimeoutInMicroSeconds/CPU_CHECK_AP_INTERVAL + 1;
  WaitCountIndex = 0;
  BackupWakeUpBuffer ((UINT32)DataInHob->Buffer);
  if (!SingleThread) {
    WakeUpAP (0xFFFFFFFF, Procedure, DataInHob->Buffer, ProcedureArgument, &FinishedCount);
    //
    // Wait to finish
    //
    if (TimeoutInMicroSeconds == 0) {
      while (*FinishedCount < ProcessorNumber - 1) {
        ;
      }
      Status = EFI_SUCCESS;
    } else {
      Status = EFI_TIMEOUT;
      for (WaitCountIndex = 0; WaitCountIndex < WaitCountNumber; WaitCountIndex++) {
        MicroSecondDelay (CPU_CHECK_AP_INTERVAL);
        if (*FinishedCount >= ProcessorNumber - 1) {
          Status = EFI_SUCCESS;
          break;
        }
      }
    }
  } else {
    Status = PeiWhoAmI (
               PeiServices,
               This,
               &BspIndex
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    Status = EFI_SUCCESS;
    for (Index = 0; Index < ProcessorNumber; Index++) {
      if (Index == BspIndex) {
        continue;
      }
      WakeUpAP (DataInHob->CpuData[Index].ApicId, Procedure, DataInHob->Buffer, ProcedureArgument, &FinishedCount);
      //
      // Wait to finish
      //
      if (TimeoutInMicroSeconds == 0) {
        while (*FinishedCount < 1) {
          ;
        }
      } else {
        for (WaitCountIndex = 0; WaitCountIndex < WaitCountNumber; WaitCountIndex++) {
          MicroSecondDelay (CPU_CHECK_AP_INTERVAL);
          if (*FinishedCount >= 1) {
            break;
          }
        }
        if (WaitCountIndex == WaitCountNumber) {
          Status = EFI_TIMEOUT;
        }
      }
    }
  }
  RestoreWakeUpBuffer ((UINT32)DataInHob->Buffer);

  return Status;
}

/**
  This service lets the caller get one enabled AP to execute a caller-provided 
  function. The caller can request the BSP to wait for the completion 
  of the AP. This service may only be called from the BSP.

  This function is used to dispatch one enabled AP to the function specified by 
  Procedure passing in the argument specified by ProcedureArgument.
  The execution is in blocking mode. The BSP waits until the AP finishes or 
  TimeoutInMicroSecondss expires.
  
  If the timeout specified by TimeoutInMicroseconds expires before the AP returns 
  from Procedure, then execution of Procedure by the AP is terminated. The AP is 
  available for subsequent calls to PEI_MP_SERVICES_PPI.StartupAllAPs() and 
  PEI_MP_SERVICES_PPI.StartupThisAP().

  @param[in]  PeiServices             General purpose services available to every PEIM.
  @param[in]  This                    A pointer to the PEI_MP_SERVICES_PPI
                                      instance.
  @param[in]  Procedure               A pointer to the function to be run on 
                                      enabled APs of the system. See type
                                      EFI_AP_PROCEDURE.
  @param[in]  ProcessorNumber         The handle number of the AP. The range is 
                                      from 0 to the total number of logical
                                      processors minus 1. The total number of 
                                      logical processors can be retrieved by
                                      PEI_MP_SERVICES_PPI.GetNumberOfProcessors().
  @param[in]  TimeoutInMicrosecsond   Indicates the time limit in microseconds for 
                                      APs to return from Procedure, for 
                                      blocking mode only. Zero means 
                                      infinity.  If the timeout expires before 
                                      all APs return from Procedure, then Procedure
                                      on the failed APs is terminated. All enabled 
                                      APs are available for next function assigned 
                                      by PEI_MP_SERVICES_PPI.StartupAllAPs() 
                                      or PEI_MP_SERVICES_PPI.StartupThisAP().
                                      If the timeout expires in blocking mode, 
                                      BSP returns EFI_TIMEOUT.
  @param[in]  ProcedureArgument       The parameter passed into Procedure for 
                                      all APs.

  @retval EFI_SUCCESS             In blocking mode, specified AP finished before 
                                  the timeout expires.
  @retval EFI_DEVICE_ERROR        The calling processor is an AP.
  @retval EFI_TIMEOUT             In blocking mode, the timeout expired before 
                                  the specified AP has finished.
  @retval EFI_NOT_FOUND           The processor with the handle specified by 
                                  ProcessorNumber does not exist.
  @retval EFI_INVALID_PARAMETER   ProcessorNumber specifies the BSP or disabled AP.
  @retval EFI_INVALID_PARAMETER   Procedure is NULL.

**/
EFI_STATUS
EFIAPI
PeiStartupThisAP (
  IN  CONST EFI_PEI_SERVICES    **PeiServices,
  IN  PEI_MP_SERVICES_PPI       *This,
  IN  EFI_AP_PROCEDURE          Procedure,
  IN  UINTN                     ProcessorNumber,
  IN  UINTN                     TimeoutInMicroseconds,
  IN  VOID                      *ProcedureArgument      OPTIONAL
  )
{
  EFI_HOB_GUID_TYPE       *GuidHob;
  PEI_MP_HOB_DATA         *DataInHob;
  UINTN                   BspIndex;
  volatile UINT32         *FinishedCount;
  EFI_STATUS              Status;
  UINTN                   WaitCountIndex;
  UINTN                   WaitCountNumber;

  GuidHob = GetFirstGuidHob (&mPeiMpHobGuid);
  if (GuidHob == NULL) {
    return EFI_UNSUPPORTED;
  }
  DataInHob            = GET_GUID_HOB_DATA (GuidHob);
  if (ProcessorNumber >= DataInHob->CpuCount) {
    return EFI_NOT_FOUND;
  }

  Status = PeiWhoAmI (
             PeiServices,
             This,
             &BspIndex
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  if (ProcessorNumber == BspIndex) {
    return EFI_UNSUPPORTED;
  }

  BackupWakeUpBuffer ((UINT32)DataInHob->Buffer);

  WaitCountNumber = TimeoutInMicroseconds/CPU_CHECK_AP_INTERVAL + 1;
  WaitCountIndex = 0;
  WakeUpAP (DataInHob->CpuData[ProcessorNumber].ApicId, Procedure, DataInHob->Buffer, ProcedureArgument, &FinishedCount);

  //
  // Wait to finish
  //
  if (TimeoutInMicroseconds == 0) {
    while (*FinishedCount < 1) {
      ;
    }
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_TIMEOUT;
    for (WaitCountIndex = 0; WaitCountIndex < WaitCountNumber; WaitCountIndex++) {
      MicroSecondDelay (CPU_CHECK_AP_INTERVAL);
      if (*FinishedCount >= 1) {
        Status = EFI_SUCCESS;
        break;
      }
    }
  }
  RestoreWakeUpBuffer ((UINT32)DataInHob->Buffer);
  return Status;
}

/**
  This service switches the requested AP to be the BSP from that point onward. 
  This service changes the BSP for all purposes.   This call can only be performed 
  by the current BSP.

  This service switches the requested AP to be the BSP from that point onward. 
  This service changes the BSP for all purposes. The new BSP can take over the 
  execution of the old BSP and continue seamlessly from where the old one left 
  off.

  If the BSP cannot be switched prior to the return from this service, then 
  EFI_UNSUPPORTED must be returned.

  @param[in] PeiServices       General purpose services available to every PEIM.
  @param[in] This              A pointer to the PEI_MP_SERVICES_PPI instance.
  @param[in] ProcessorNumber   The handle number of AP that is to become the new 
                               BSP. The range is from 0 to the total number of 
                               logical processors minus 1. The total number of 
                               logical processors can be retrieved by
                               PEI_MP_SERVICES_PPI.GetNumberOfProcessors().
  @param[in] EnableOldBSP      If TRUE, then the old BSP will be listed as an 
                               enabled AP. Otherwise, it will be disabled.

  @retval EFI_SUCCESS             BSP successfully switched.
  @retval EFI_UNSUPPORTED         Switching the BSP cannot be completed prior to 
                                  this service returning.
  @retval EFI_UNSUPPORTED         Switching the BSP is not supported.
  @retval EFI_SUCCESS             The calling processor is an AP.
  @retval EFI_NOT_FOUND           The processor with the handle specified by
                                  ProcessorNumber does not exist.
  @retval EFI_INVALID_PARAMETER   ProcessorNumber specifies the current BSP or 
                                  a disabled AP.
  @retval EFI_NOT_READY           The specified AP is busy.

**/
EFI_STATUS
EFIAPI
PeiSwitchBSP (
  IN  CONST EFI_PEI_SERVICES   **PeiServices,
  IN  PEI_MP_SERVICES_PPI      *This,
  IN  UINTN                    ProcessorNumber,
  IN  BOOLEAN                  EnableOldBSP
  )
{
  //
  // Do not support it.
  //
  return EFI_UNSUPPORTED;
}

/**
  This service lets the caller enable or disable an AP from this point onward.  
  This service may only be called from the BSP.

  This service allows the caller enable or disable an AP from this point onward. 
  The caller can optionally specify the health status of the AP by Health. If 
  an AP is being disabled, then the state of the disabled AP is implementation 
  dependent. If an AP is enabled, then the implementation must guarantee that a 
  complete initialization sequence is performed on the AP, so the AP is in a state 
  that is compatible with an MP operating system.

  If the enable or disable AP operation cannot be completed prior to the return 
  from this service, then EFI_UNSUPPORTED must be returned.

  @param[in] PeiServices       General purpose services available to every PEIM.
  @param[in] This              A pointer to the PEI_MP_SERVICES_PPI instance.
  @param[in] ProcessorNumber   The handle number of AP that is to become the new 
                               BSP. The range is from 0 to the total number of 
                               logical processors minus 1. The total number of 
                               logical processors can be retrieved by
                               PEI_MP_SERVICES_PPI.GetNumberOfProcessors().
  @param[in] EnableAP          Specifies the new state for the processor for 
                               enabled, FALSE for disabled.
  @param[in] HealthFlag        If not NULL, a pointer to a value that specifies 
                               the new health status of the AP. This flag 
                               corresponds to StatusFlag defined in 
                               PEI_MP_SERVICES_PPI.GetProcessorInfo(). Only 
                               the PROCESSOR_HEALTH_STATUS_BIT is used. All other 
                               bits are ignored.  If it is NULL, this parameter 
                               is ignored.

  @retval EFI_SUCCESS             The specified AP was enabled or disabled successfully.
  @retval EFI_UNSUPPORTED         Enabling or disabling an AP cannot be completed 
                                  prior to this service returning.
  @retval EFI_UNSUPPORTED         Enabling or disabling an AP is not supported.
  @retval EFI_DEVICE_ERROR        The calling processor is an AP.
  @retval EFI_NOT_FOUND           Processor with the handle specified by ProcessorNumber
                                  does not exist.
  @retval EFI_INVALID_PARAMETER   ProcessorNumber specifies the BSP.

**/
EFI_STATUS
EFIAPI
PeiEnableDisableAP (
  IN  CONST EFI_PEI_SERVICES    **PeiServices,
  IN  PEI_MP_SERVICES_PPI       *This,
  IN  UINTN                     ProcessorNumber,
  IN  BOOLEAN                   EnableAP,
  IN  UINT32                    *HealthFlag OPTIONAL
  )
{
  //
  // Do not support it.
  //
  return EFI_UNSUPPORTED;
}

/**
  This return the handle number for the calling processor.  This service may be 
  called from the BSP and APs.

  This service returns the processor handle number for the calling processor. 
  The returned value is in the range from 0 to the total number of logical 
  processors minus 1. The total number of logical processors can be retrieved 
  with PEI_MP_SERVICES_PPI.GetNumberOfProcessors(). This service may be 
  called from the BSP and APs. If ProcessorNumber is NULL, then EFI_INVALID_PARAMETER 
  is returned. Otherwise, the current processors handle number is returned in 
  ProcessorNumber, and EFI_SUCCESS is returned.

  @param[in] PeiServices       General purpose services available to every PEIM.
  @param[in] This              A pointer to the PEI_MP_SERVICES_PPI instance.
  @param[in] ProcessorNumber   The handle number of AP that is to become the new 
                               BSP. The range is from 0 to the total number of 
                               logical processors minus 1. The total number of 
                               logical processors can be retrieved by
                               PEI_MP_SERVICES_PPI.GetNumberOfProcessors().

  @retval EFI_SUCCESS             The current processor handle number was returned 
                                  in ProcessorNumber.
  @retval EFI_INVALID_PARAMETER   ProcessorNumber is NULL.

**/
EFI_STATUS
EFIAPI
PeiWhoAmI (
  IN  CONST EFI_PEI_SERVICES   **PeiServices,
  IN  PEI_MP_SERVICES_PPI      *This,
  OUT UINTN                    *ProcessorNumber
  )
{
  EFI_HOB_GUID_TYPE       *GuidHob;
  PEI_MP_HOB_DATA         *DataInHob;
  UINTN                   TotalProcessorNumber;
  UINTN                   Index;

  if (ProcessorNumber == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  GuidHob = GetFirstGuidHob (&mPeiMpHobGuid);
  if (GuidHob == NULL) {
    return EFI_UNSUPPORTED;
  }
  DataInHob            = GET_GUID_HOB_DATA (GuidHob);
  TotalProcessorNumber = DataInHob->CpuCount;

  for (Index = 0; Index < TotalProcessorNumber; Index++) {
    if (DataInHob->CpuData[Index].ApicId == GetApicId ()) {
      *ProcessorNumber = Index;
      return EFI_SUCCESS;
    }
  }

  ASSERT(FALSE);

  return EFI_NOT_FOUND;
}

PEI_MP_SERVICES_PPI                mMpServicesPpi = {
  PeiGetNumberOfProcessors,
  PeiGetProcessorInfo,
  PeiStartupAllAPs,
  PeiStartupThisAP,
  PeiSwitchBSP,
  PeiEnableDisableAP,
  PeiWhoAmI,
};

//
// Ppis to be installed
//
EFI_PEI_PPI_DESCRIPTOR           mPpiList[] = { 
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gPeiMpServicePpiGuid,
    &mMpServicesPpi
  }
};

VOID
EFIAPI
UnitTestFuncNull (
  IN OUT VOID  *Buffer
  )
{
}

VOID
EFIAPI
UnitTestFuncGetApicId (
  IN OUT VOID  *Buffer
  )
{
  UINT32   *ApicId;

  ApicId = (UINT32 *)Buffer;
  *ApicId = GetApicId ();
}

typedef struct {
  UINTN            SpinLock;
  UINT32           ApCount;
  UINT32           ApicId[FixedPcdGet32(PcdCpuMaxLogicalProcessorNumber)];
} APIC_ID_LIST;

VOID
EFIAPI
UnitTestFuncGetApicIdList (
  IN OUT VOID  *Buffer
  )
{
  APIC_ID_LIST   *ApicIdList;

  ApicIdList = (APIC_ID_LIST *)Buffer;
  
  AcquireSpinLock (&ApicIdList->SpinLock);

  ApicIdList->ApicId[ApicIdList->ApCount] = GetApicId ();
  InterlockedIncrement (&ApicIdList->ApCount);

  ReleaseSpinLock (&ApicIdList->SpinLock);
}

VOID
UnitTest (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                Status;
  UINTN                     NumberOfProcessors;
  UINTN                     NumberOfEnabledProcessors;
  UINTN                     Index;
  EFI_PROCESSOR_INFORMATION ProcessorInfoBuffer;
  UINTN                     BspIndex;
  UINT32                    ApicId;
  APIC_ID_LIST              ApicIdList;

  Status = PeiGetNumberOfProcessors (
             PeiServices,
             &mMpServicesPpi,
             &NumberOfProcessors,
             &NumberOfEnabledProcessors
             );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((EFI_D_ERROR, "PeiGetNumberOfProcessors - NumberOfProcessors - %x\n", NumberOfProcessors));
  DEBUG ((EFI_D_ERROR, "PeiGetNumberOfProcessors - NumberOfEnabledProcessors - %x\n", NumberOfEnabledProcessors));

  for (Index = 0; Index < NumberOfProcessors; Index++) {
    Status = PeiGetProcessorInfo (
               PeiServices,
               &mMpServicesPpi,
               Index,
               &ProcessorInfoBuffer
               );
    ASSERT_EFI_ERROR (Status);
    DEBUG ((EFI_D_ERROR, "PeiGetProcessorInfo - Index - %x\n", Index));
    DEBUG ((EFI_D_ERROR, "PeiGetProcessorInfo - ProcessorId      - %016lx\n", ProcessorInfoBuffer.ProcessorId));
    DEBUG ((EFI_D_ERROR, "PeiGetProcessorInfo - StatusFlag       - %08x\n", ProcessorInfoBuffer.StatusFlag));
    DEBUG ((EFI_D_ERROR, "PeiGetProcessorInfo - Location.Package - %08x\n", ProcessorInfoBuffer.Location.Package));
    DEBUG ((EFI_D_ERROR, "PeiGetProcessorInfo - Location.Core    - %08x\n", ProcessorInfoBuffer.Location.Core));
    DEBUG ((EFI_D_ERROR, "PeiGetProcessorInfo - Location.Thread  - %08x\n", ProcessorInfoBuffer.Location.Thread));
  }

  ZeroMem (&ApicIdList, sizeof(ApicIdList));
  InitializeSpinLock (&ApicIdList.SpinLock);
  Status = PeiStartupAllAPs (
             PeiServices,
             &mMpServicesPpi,
             UnitTestFuncGetApicIdList,
             FALSE,
             0,
             &ApicIdList
             );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((EFI_D_ERROR, "PeiStartupAllAPs - SingleThread - FALSE, ApCount - %08x\n", ApicIdList.ApCount));
  for (Index = 0; Index < ApicIdList.ApCount; Index++) {
    DEBUG ((EFI_D_ERROR, "PeiStartupAllAPs - ApicId - %08x\n", ApicIdList.ApicId[Index]));
  }

  ZeroMem (&ApicIdList, sizeof(ApicIdList));
  InitializeSpinLock (&ApicIdList.SpinLock);
  Status = PeiStartupAllAPs (
             PeiServices,
             &mMpServicesPpi,
             UnitTestFuncGetApicIdList,
             TRUE,
             0,
             &ApicIdList
             );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((EFI_D_ERROR, "PeiStartupAllAPs - SingleThread - TRUE, ApCount - %08x\n", ApicIdList.ApCount));
  for (Index = 0; Index < ApicIdList.ApCount; Index++) {
    DEBUG ((EFI_D_ERROR, "PeiStartupAllAPs - ApicId - %08x\n", ApicIdList.ApicId[Index]));
  }

  Status = PeiWhoAmI (
             PeiServices,
             &mMpServicesPpi,
             &BspIndex
             );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((EFI_D_ERROR, "PeiWhoAmI - BspIndex - %x\n", BspIndex));

  for (Index = 0; Index < NumberOfProcessors; Index++) {
    if (Index == BspIndex) {
      continue;
    }
    Status = PeiStartupThisAP (
               PeiServices,
               &mMpServicesPpi,
               UnitTestFuncGetApicId,
               Index,
               0,
               &ApicId
               );
    ASSERT_EFI_ERROR (Status);
    DEBUG ((EFI_D_ERROR, "PeiStartupThisAP - Index - %x\n", Index));
    DEBUG ((EFI_D_ERROR, "PeiStartupThisAP - ApicId - %08x\n", ApicId));
  }
}

/**
  The Entry point of the MP CPU PEIM

  This function is the Entry point of the MP CPU PEIM which will install the MpServicePpi
 
  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services. 
                          
  @retval EFI_SUCCESS   MpServicePpi is installed successfully.

**/
EFI_STATUS
EFIAPI
CpuMpPeimInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
	EFI_STATUS  Status;

  //
  // Collect info
  //
  CountCpuNumber (FixedPcdGet32(PcdCpuMaxLogicalProcessorNumber));

  //
  // Install PPIs
  //
  Status = PeiServicesInstallPpi(&mPpiList[0]);
  ASSERT_EFI_ERROR (Status);

  //
  // Unit Test
  //
  //UnitTest (PeiServices);
  return Status;
}
