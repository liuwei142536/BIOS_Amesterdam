/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/** @file
  Code for processor configuration.

  Copyright (c) 2006 - 2014, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  ProcessorConfig.c

**/

#include "MpService.h"
#include "Cpu.h"
#include "Microcode.h"

#ifdef BTG_FLAG
#include "BootGuardLibrary.h"
#endif

//ppsong double check #include "UBOX_CFG.h"
#include "X2Apic.h"
#include "MchkInit.h"
#include "ThermalManagement.h"
#include "CState.h"
#include "Vt.h"
#include "Misc.h"

MP_SYSTEM_DATA                      mMPSystemData;
CPU_CONFIG_CONTEXT_BUFFER           mCpuConfigContextBuffer;
EFI_PHYSICAL_ADDRESS                mStartupVector;
UINT8                               mPlatformType;
ACPI_CPU_DATA                       *mAcpiCpuData;
EFI_HANDLE                          mHandle = NULL;
MTRR_SETTINGS                       *mMtrrSettings;
EFI_EVENT                           mSmmConfigurationNotificationEvent;
EFI_HANDLE                          mImageHandle;
EFI_CPU_CSR_ACCESS_PROTOCOL         *mCpuCsrAccess;
EFI_TIMER_ARCH_PROTOCOL             *mTimer;
UINTN                               mLocalApicTimerDivisor;
UINT32                              mLocalApicTimerInitialCount;
//ESS Override Start
// 4168379: Odin: PL1 cannot be changed greater than 200W and lock bit set.
UINT32                              mTurboOverride;
//ESS Override End
VOID                                *mMicrocodeBaseAddress;
VOID                                *mMicrocodePatchAddress;

//Global variables from PCD values
UINT64                              mCpuIioLlcWaysBitMask;
UINT64                              mCpuQlruCfgBitMask;
UINT8                               mCpuDebugInterface;
BOOLEAN                             mCpuHaswellFamilyFlag;


/**
  Prepares memory region for processor configuration.
  
  This function prepares memory region for processor configuration.

**/
VOID
PrepareMemoryForConfiguration (
  VOID
  )
{
  UINTN                NumberOfProcessors;
  UINTN                Index;
  MONITOR_MWAIT_DATA   *MonitorData;
  EFI_PHYSICAL_ADDRESS  StackStartAddress = 0xffffffff;
  EFI_STATUS            Status;

  //
  // Initialize Spin Locks for system
  //
  InitializeSpinLock (&mMPSystemData.APSerializeLock);
  InitializeSpinLock (&mMPSystemData.MsrLock);
  for (Index = 0; Index < PcdGet32(PcdCpuMaxLogicalProcessorNumber); Index++) {
    InitializeSpinLock (&mMPSystemData.CpuData[Index].CpuDataLock);
  }

  //
  // Claim memory for AP stack.
  //
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiReservedMemoryType,
                  EFI_SIZE_TO_PAGES (PcdGet32(PcdCpuMaxLogicalProcessorNumber) * PcdGet32 (PcdCpuApStackSize)),
                  &StackStartAddress
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ( (EFI_D_ERROR, "::::::: Failed to allocate pages for ProcessorConfig\n") );
    return;
  }  

  mExchangeInfo->StackStart = (VOID *)(UINTN)StackStartAddress; //AllocateAcpiNvsMemoryBelow4G (PcdGet32(PcdCpuMaxLogicalProcessorNumber) * PcdGet32 (PcdCpuApStackSize));
  mExchangeInfo->StackSize  = PcdGet32 (PcdCpuApStackSize);

  //
  // Initialize the Monitor Data structure in APs' stack
  //
  for (Index = 0; Index < PcdGet32(PcdCpuMaxLogicalProcessorNumber); Index++) {
    MonitorData = GetMonitorDataAddress (Index);
    MonitorData->ApLoopMode = ApInHltLoop;
  }

  //
  // Initialize data for CPU configuration context buffer
  //
  NumberOfProcessors = mCpuConfigContextBuffer.NumberOfProcessors;
  mCpuConfigContextBuffer.CollectedDataBuffer  = AllocateZeroPool (sizeof (CPU_COLLECTED_DATA) * NumberOfProcessors);
  mCpuConfigContextBuffer.FeatureLinkListEntry = AllocateZeroPool (sizeof (LIST_ENTRY) * NumberOfProcessors);

  //
  // Initialize Processor Feature List for all logical processors.
  //
  for (Index = 0; Index < NumberOfProcessors; Index++) {
    InitializeListHead (&mCpuConfigContextBuffer.FeatureLinkListEntry[Index]);
  }

  mCpuConfigContextBuffer.RegisterTable   = AllocateAcpiNvsMemoryBelow4G (
                                            (sizeof (CPU_REGISTER_TABLE) + sizeof (UINTN)) * NumberOfProcessors
                                            );
  mCpuConfigContextBuffer.PreSmmInitRegisterTable = AllocateAcpiNvsMemoryBelow4G (
                                                     (sizeof (CPU_REGISTER_TABLE) + sizeof (UINTN)) * NumberOfProcessors
                                                     );

  mCpuConfigContextBuffer.SettingSequence = (UINTN *) (((UINTN) mCpuConfigContextBuffer.RegisterTable) + (sizeof (CPU_REGISTER_TABLE) * NumberOfProcessors));
  for (Index = 0; Index < NumberOfProcessors; Index++) {
    mCpuConfigContextBuffer.SettingSequence[Index] = Index;
  }

  //
  // Set the value for PcdCpuConfigContextBuffer.
  //
  mCpuConfigLibConfigContextBuffer = &mCpuConfigContextBuffer;
  PcdSet64 (PcdCpuConfigContextBuffer, (UINT64) (UINTN) mCpuConfigLibConfigContextBuffer);

  //
  // Read the platform type from PCD
  //
  mPlatformType = PcdGet8 (PcdPlatformType);
}

/**
  Event notification that is fired every time a gEfiSmmConfigurationProtocol installs.

  This function configures all logical processors with three-phase architecture.

  @param  Event                 The Event that is being processed, not used.
  @param  Context               Event Context, not used.

**/
VOID
EFIAPI
SmmConfigurationEventNotify (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS    Status;
  VOID  *Registration;
  EFI_SMM_CONFIGURATION_PROTOCOL  *SmmConfiguration;

  //
  // Make sure this notification is for this handler
  //
  Status = gBS->LocateProtocol (&gEfiSmmConfigurationProtocolGuid, NULL, (VOID **)&SmmConfiguration);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Wakeup APs. Collect data of all processors. BSP polls to
  // wait for APs' completion.
  //
  DataCollectionPhase ();
  //
  // With collected data, BSP analyzes processors'configuration
  // according to user's policy.
  //
  AnalysisPhase ();

  //
  // Wakeup APs. Program registers of all processors, according to
  // the result of Analysis phase. BSP polls to wait for AP's completion.
  //
  SettingPhase ();

  //
  // Select least-feature procesosr as BSP
  //
  if (FeaturePcdGet (PcdCpuSelectLfpAsBspFlag)) {
    SelectLfpAsBsp ();
  }

  //
  // Add SMBIOS Processor Type and Cache Type tables for the CPU.
  //
  AddCpuSmbiosTables ();

  //
  // Save CPU S3 data
  //
  SaveCpuS3Data (mImageHandle);

  Status = gBS->SetTimer (
                  mMPSystemData.CheckAPsEvent,
                  TimerPeriodic,
                  10000 * MICROSECOND
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Setup notification on Legacy BIOS Protocol to reallocate AP wakeup
  //
  EfiCreateProtocolNotifyEvent (
    &gEfiLegacyBiosProtocolGuid,
    TPL_CALLBACK,
    ReAllocateMemoryForAP,
    NULL,
    &Registration
    );
}

/**
  Early MP Initialization.
  
  This function does early MP initialization, including MTRR sync and first time microcode load.

  @param  ProcessorNumber    Handle number of specified logical processor.

**/
VOID
EarlyMpInit (
  IN UINTN  ProcessorNumber
  )
{
  //
  // first time microcode load.
  // Microcode patch must be applied before collecting capability
  // of processor features.
  //
  if (FeaturePcdGet (PcdCpuMicrocodeUpdateFlag)) {
    MicrocodeDetect (ProcessorNumber, TRUE);
  }


  MtrrSetAllMtrrs (mMtrrSettings);

  CollectBasicProcessorData (ProcessorNumber);

  //
  // If matching microcode patch has been found for this processor, then it
  // has already been applied at earlier phase. So here simply check if
  // microcode has been applied and collect the version.
  //
  if (FeaturePcdGet (PcdCpuMicrocodeUpdateFlag)) {
    CollectMicrocodeRevision (ProcessorNumber);
  }
}

/**
  Early MP package-scope Initialization.
  
  This function does early MP package-scope initialization, typically programming of packege-scope MSRs
  It is called after AssignPackageBsp() has been executed.
    
**/
VOID
EarlyMpInitPkgScope ()
{

    UINT32  i;
	UINT32  PbspApicId;

	DEBUG ( (EFI_D_ERROR, " \n:: BIOS in EarlyMpInitPkgScope()\n") );
   
	for (i=0; i< mCpuConfigContextBuffer.NumberOfProcessors; i++) {

		//
		//	Skip SBSP for now
		//
		if(i == mCpuConfigContextBuffer.BspNumber)   continue; 

        //
        //   Dispatch Pkg BSPs one by one for earlyMpInitPbsp()
        //
        if (mCpuConfigContextBuffer.CollectedDataBuffer[i].PackageBsp) {
              PbspApicId = mCpuConfigContextBuffer.CollectedDataBuffer[i].CpuMiscData.ApicID;

              DispatchAPAndWait (
              FALSE,
              PbspApicId,
              EarlyMpInitPbsp
              );
        }
	}

    //
    //  Now SBSP
    //
    EarlyMpInitPbsp(mCpuConfigContextBuffer.BspNumber);
}


/**
  Configures all logical processors with three-phase architecture.
  
  This function configures all logical processors with three-phase architecture.

**/
EFI_STATUS
ProcessorConfiguration (
  VOID
  )
{
  EFI_STATUS    Status;

#ifdef BTG_FLAG //&& (BTG_FLAG == 1)
  DEBUG ( (EFI_D_ERROR, "BTG Outside of StopPbeTimer ()\n") );
  ///
  /// Disable PBET before send IPI to APs
  ///
  DEBUG ( (EFI_D_ERROR, "BTG Inside of StopPbeTimer ()\n") );
  StopPbeTimer ();
#endif

  //
  // Wakeup APs for the first time, BSP stalls for arbitrary
  // time for APs' completion. BSP then collects the number
  // and BIST information of APs.
  //
  WakeupAPAndCollectBist ();
  //
  // Sort APIC ID of all processors in asending order. Processor number
  // is assigned in this order to ease MP debug. SMBIOS logic also depends on it.
  //
  SortApicId ();

  //
  // Prepare data in memory for processor configuration
  //
  PrepareMemoryForConfiguration ();


  //
  // Early MP initialization
  //
  mMtrrSettings = (MTRR_SETTINGS *)(UINTN)PcdGet64 (PcdCpuMtrrTableAddress);
  if (mMtrrSettings == NULL) {
    //
    // MtrrSettings is not set. Set it here. 
    // Allocate memory for fixed MTRRs, variable MTRRs and MTRR_DEF_TYPE
    //
    mMtrrSettings = AllocateAcpiNvsMemoryBelow4G (sizeof (MTRR_SETTINGS));
    PcdSet64 (PcdCpuMtrrTableAddress, (UINT64) (UINTN) mMtrrSettings);
  }
  MtrrGetAllMtrrs (mMtrrSettings);
 
  mTurboOverride = PcdGet32(PcdCpuTurboOverride);
  //DEBUG ((EFI_D_ERROR, ":: mTurboOverride = %X\n", mTurboOverride));


  DispatchAPAndWait (
    TRUE,
    0,
    EarlyMpInit
    );

  EarlyMpInit (mCpuConfigContextBuffer.BspNumber);

  DEBUG_CODE (
    //
    // Verify that all processors have same APIC ID topology. New APIC IDs
    // were constructed based on this assumption.
    //
    UINTN Index;
    UINT8 PackageIdBitOffset;

    PackageIdBitOffset = mCpuConfigContextBuffer.CollectedDataBuffer[0].PackageIdBitOffset;
    for (Index = 1; Index < mCpuConfigContextBuffer.NumberOfProcessors; Index++) {
      if (PackageIdBitOffset != mCpuConfigContextBuffer.CollectedDataBuffer[Index].PackageIdBitOffset) {
        ASSERT (FALSE);
      }
    }
  );

  //
  // Check if there is legacy APIC ID conflict among all processors.
  //
  Status = CheckApicId ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Assign Package BSP for package scope programming later.
  //
  AssignPackageBsp ();

  //
  // Perform processor package-scope early configuration
  //
  EarlyMpInitPkgScope();
  //
  // Produce pre-SMM-init register table.
  //
  ProducePreSmmInitRegisterTable ();

  //
  // Early MP initialization step 2
  //
  DispatchAPAndWait (
    TRUE,
    0,
    SetPreSmmInitProcessorRegister
    );

  SetPreSmmInitProcessorRegister (mCpuConfigContextBuffer.BspNumber);

  //
  // Re-program Local APIC for virtual wire mode if socket ID for the BSP has been changed.
  //
  ReprogramVirtualWireMode ();

  //
  // If socket IDs were reassigned, update APIC ID related info collected before.
  //
  UpdateApicId ();

  MeasureMicrocode ();

  //
  // Locate Timer Arch Protocol
  //
  Status = gBS->LocateProtocol (&gEfiTimerArchProtocolGuid, NULL, (VOID **) &mTimer);
  ASSERT_EFI_ERROR (Status);

 //ESS Override start

  WA_Lock ();

  //
  // Install MP Services Protocol
  //
  InstallMpServicesProtocol ();

  return EFI_SUCCESS;
}

/**
  Installs MP Services Protocol and register related timer event.
  
  This function installs MP Services Protocol and register related timer event.

**/
VOID
InstallMpServicesProtocol (
  VOID
  )
{
  EFI_STATUS    Status;

  //
  // Create timer event to check AP state for non-blocking execution.
  //
  Status = gBS->CreateEvent (
                  EVT_TIMER | EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  CheckAPsStatus,
                  NULL,
                  &mMPSystemData.CheckAPsEvent
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Now install the MP services protocol.
  //
  Status = gBS->InstallProtocolInterface (
                  &mHandle,
                  &gEfiMpServiceProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mMpService
                  );
  ASSERT_EFI_ERROR (Status);
}

/**
  Callback function for idle events.
 
  @param  Event                 Event whose notification function is being invoked.
  @param  Context               The pointer to the notification function's context,
                                which is implementation-dependent.

**/
VOID
EFIAPI
IdleLoopEventCallback (
  IN EFI_EVENT                Event,
  IN VOID                     *Context
  )
{
  CpuSleep ();
}

/**
  Entrypoint of CPU MP DXE module.
  
  This function is the entrypoint of CPU MP DXE module.
  It initializes Multi-processor configuration and installs MP Services Protocol.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS   The entrypoint always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
MultiProcessorInitialize (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS  Status;
  VOID        *Registration;
  EFI_EVENT   IdleLoopEvent;
  EFI_EVENT   ExitBootServiceEvent;
  EFI_EVENT   LegacyToBootEvent;

  mImageHandle = ImageHandle;
	

	//
	// Copy Microcode to reserved memory
	//
    mMicrocodeBaseAddress = AllocateReservedPool(PcdGet32 (PcdFlashNvStorageMicrocodeSize));
    CopyMem(mMicrocodeBaseAddress, (VOID*)(UINTN)PcdGet32 (PcdFlashNvStorageMicrocodeBase), (UINTN)PcdGet32 (PcdFlashNvStorageMicrocodeSize));
	mMicrocodePatchAddress = (VOID *)((UINTN)mMicrocodeBaseAddress + (UINTN)(PcdGet64 (PcdCpuMicrocodePatchAddress) - PcdGet32 (PcdFlashNvStorageMicrocodeBase)));

	//
	// Store the PCD values needed in AP functions to global variables
	//
	mCpuIioLlcWaysBitMask  = PcdGet64(PcdCpuIioLlcWaysBitMask);
	mCpuQlruCfgBitMask     = PcdGet64(PcdCpuQlruCfgBitMask);
	mCpuDebugInterface     = PcdGet8(PcdCpuDebugInterface);
	mCpuHaswellFamilyFlag  = FeaturePcdGet (PcdCpuHaswellFamilyFlag);

  BdeHookInitialize();

  //
  // Configure processors with three-phase architecture
  //
  Status = ProcessorConfiguration ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Install notification callback on SMM Configuration Protocol
  //
  mSmmConfigurationNotificationEvent = EfiCreateProtocolNotifyEvent (
                                         &gEfiSmmConfigurationProtocolGuid,
                                         TPL_CALLBACK,
                                         SmmConfigurationEventNotify,
                                         NULL,
                                         &Registration
                                         );
  //
  // Check if there is AP existed
  //
  if (mCpuConfigContextBuffer.NumberOfProcessors > 1) {
    //
    // Create EXIT_BOOT_SERIVES Event to set AP to suitable status
    //
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_NOTIFY,
                    ChangeApLoopModeCallBack,
                    NULL,
                    &gEfiEventExitBootServicesGuid,
                    &ExitBootServiceEvent
                    );
    ASSERT_EFI_ERROR (Status);
    //
    // Create an event to be signalled when Legacy Boot occurs to set AP to suitable status
    //
    Status = EfiCreateEventLegacyBootEx(
               TPL_NOTIFY,
               ChangeApLoopModeCallBack,
               NULL,
               &LegacyToBootEvent
               );
    ASSERT_EFI_ERROR (Status);
  }
  //
  // Setup a callback for idle events
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  IdleLoopEventCallback,
                  NULL,
                  &gIdleLoopEventGuid,
                  &IdleLoopEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
  Wakes up APs for the first time to count their number and collect BIST data.

  This function wakes up APs for the first time to count their number and collect BIST data.

**/
VOID
WakeupAPAndCollectBist (
  VOID
  )
{
  //
  // Save BSP's Local APIC Timer setting
  //
  GetApicTimerState (&mLocalApicTimerDivisor, NULL, NULL);
  mLocalApicTimerInitialCount = GetApicTimerInitCount ();

  //
  // Prepare code and data for APs' startup vector
  //
  PrepareAPStartupVector ();

  mCpuConfigContextBuffer.NumberOfProcessors = 1;
  mCpuConfigContextBuffer.BspNumber = 0;
  //
  // Item 0 of BistBuffer is for BSP.
  //
  mExchangeInfo->BistBuffer[0].ApicId = GetInitialApicId ();
  
  SendInitSipiSipiIpis (
    TRUE,
    0,
    NULL
    );

  //
  // Wait for task to complete and then exit.
  //
  MicroSecondDelay (PcdGet32 (PcdCpuApInitTimeOutInMicroSeconds));
  mExchangeInfo->InitFlag = 0;
}


/**
  Prepare ACPI NVS memory below 4G memory for use of S3 resume.
  
  This function allocates ACPI NVS memory below 4G memory for use of S3 resume,
  and saves data into the memory region.

  @param  Context   The Context save the info.

**/
VOID
SaveCpuS3Data (
  VOID    *Context
  )
{
  MP_CPU_SAVED_DATA       *MpCpuSavedData;

  //
  // Allocate ACPI NVS memory below 4G memory for use of S3 resume.
  //
  MpCpuSavedData = AllocateAcpiNvsMemoryBelow4G (sizeof (MP_CPU_SAVED_DATA));

  //
  // Set the value for CPU data
  //
  mAcpiCpuData                 = &(MpCpuSavedData->AcpiCpuData);
  mAcpiCpuData->GdtrProfile    = (EFI_PHYSICAL_ADDRESS) (UINTN) &(MpCpuSavedData->GdtrProfile);
  mAcpiCpuData->IdtrProfile    = (EFI_PHYSICAL_ADDRESS) (UINTN) &(MpCpuSavedData->IdtrProfile);
  mAcpiCpuData->StackAddress   = (EFI_PHYSICAL_ADDRESS) (UINTN) mExchangeInfo->StackStart;
  mAcpiCpuData->StackSize      = PcdGet32 (PcdCpuApStackSize);
  mAcpiCpuData->MtrrTable      = (EFI_PHYSICAL_ADDRESS) (UINTN) PcdGet64 (PcdCpuMtrrTableAddress);
  mAcpiCpuData->RegisterTable  = (EFI_PHYSICAL_ADDRESS) (UINTN) mCpuConfigContextBuffer.RegisterTable;

  mAcpiCpuData->PreSmmInitRegisterTable   = (EFI_PHYSICAL_ADDRESS) (UINTN) mCpuConfigContextBuffer.PreSmmInitRegisterTable;
  mAcpiCpuData->ApMachineCheckHandlerBase = mApMachineCheckHandlerBase;
  mAcpiCpuData->ApMachineCheckHandlerSize = mApMachineCheckHandlerSize;

  //
  // Check user's policy for HT enable.
  //
  mAcpiCpuData->APState        = FALSE;
  if ((PcdGet32 (PcdCpuProcessorFeatureUserConfiguration) & PCD_CPU_HT_BIT) != 0) {
    mAcpiCpuData->APState = TRUE;
  }

  //
  // Copy GDTR and IDTR profiles
  //
  CopyMem ((VOID *) (UINTN) mAcpiCpuData->GdtrProfile, (VOID *) (UINTN) &mExchangeInfo->GdtrProfile, sizeof (IA32_DESCRIPTOR));
  CopyMem ((VOID *) (UINTN) mAcpiCpuData->IdtrProfile, (VOID *) (UINTN) &mExchangeInfo->IdtrProfile, sizeof (IA32_DESCRIPTOR));

  mAcpiCpuData->NumberOfCpus  = (UINT32) mCpuConfigContextBuffer.NumberOfProcessors;
  
  //
  // Set the base address of CPU S3 data to PcdCpuS3DataAddress
  //
  PcdSet64 (PcdCpuS3DataAddress, (UINT64)(UINTN)mAcpiCpuData); 
}
