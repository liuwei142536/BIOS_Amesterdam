/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

  Copyright (c) 2007 - 2013 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.


  Module Name:

    LtDxe.c

  Abstract:

  This is the main DXE file for LT.  It represents an abstract outline of the
  steps required during DXE for enabling LT.  Each individual step is further
  abstracted behind a function call interface.  This is intended to minimize 
  the need to modify this file when porting LT to future platforms.  

--*/

#include "TxtDxeLib.h"
// Aptio V server Override - Start: Changes done related to TXT

#define EFI_BDS_EVENT_BEFORE_SETUP_GUID  \
  {0xdbc9fd21, 0xfad8, 0x45b0, 0x9e, 0x78, 0x27, 0x15, 0x88, 0x67, 0xcc, 0x93};

// Aptio V server Override - End: Changes done related to TXT

LT_DXE_LIB_CONTEXT          mLtDxeCtx ;
EFI_LEGACY_REGION2_PROTOCOL *mLegacyRegion = NULL;
BOOLEAN                     ScheckDone = FALSE;
BOOLEAN                     LtDxeEnabled = TRUE;

VOID WA_UnLock()
{
  UINT32              RegEax;
  UINT8               SteppingId;

  //
  // Do nothing if >= JKT-C1 stepping
  //
  AsmCpuid (EFI_CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  SteppingId = (UINT8) (RegEax & 0xF);
 
  if (SteppingId > 5) return; 
  if (mLegacyRegion) {
    //
    // Unlock Legacy region segment to let LockConfig not to block legacy boot
    //
    mLegacyRegion->UnLock (mLegacyRegion, 
                           LEGACY_BIOS_MEMORY_SEGMENT,  
                           LEGACY_BIOS_MEMORY_LENGTH,   
                           NULL
                           ); 
  }

}

EFI_STATUS
EFIAPI
ScheckCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
/*++

Routine Description:

  This function gets registered as a callback to run the SCHECK function
  from the LT BIOS ACM as a result of Boot Events.

Arguments:

  Event     - A pointer to the Event that triggered the callback.
  Context   - A pointer to private data registered with the callback function.

Returns:

  EFI_SUCCESS   - Always.

--*/
{
  UINT32                      Data32;

  DEBUG ((EFI_D_ERROR, "DXE DoScheck ...\n"));
  DoScheck(&mLtDxeCtx) ;
  DEBUG ((EFI_D_ERROR, "DoScheck Done!\n"));

  WA_UnLock();
  
  // Aptio V Server Override Start : TXT Error and status Register message printing.
    PrintLtsxErrors();
  // Aptio V Server Override end : TXT Error and status Register message printing.

    // If LT_SPAD_HIGH 0xFED300A0 bit63 = 0, do legacy boot  
  Data32 = *(volatile UINT32 *)(UINTN)LT_SPAD_HIGH;
    DEBUG ((EFI_D_ERROR, "LT_SPAD_HIGH (0xFED300A4): %r\n", Data32));
  if ( (BIT31 & (*(volatile UINT32 *)(UINTN)LT_SPAD_HIGH)) == 0 ) {
    // BIOS ACM DoScheck error  
    DEBUG ((EFI_D_ERROR, "Bios Acm DoScheck Failed. Reboot in non-ltsx mode\n"));        
    DisableLtsxAndReset();
  }
  
  DEBUG ((EFI_D_ERROR, "DoLockConfig ...\n"));
  TxtLockConfigForAll(&mLtDxeCtx) ;
  DEBUG ((EFI_D_ERROR, "DoLockConfig Done!\n")); 
  
  // Aptio V Server Override Start : TXT Error and status Register message printing.
    PrintLtsxErrors();
  // Aptio V Server Override end : TXT Error and status Register message printing.

   // If LT_SPAD_HIGH 0xFED300A0 bit63 = 0, do legacy boot  
  Data32 = *(volatile UINT32 *)(UINTN)LT_SPAD_HIGH;
    DEBUG ((EFI_D_ERROR, "LT_SPAD_HIGH (0xFED300A4): %r\n", Data32));
  if ( (BIT31 & (*(volatile UINT32 *)(UINTN)LT_SPAD_HIGH)) == 0 ) {
    // BIOS ACM DoScheck error  
    DEBUG ((EFI_D_ERROR, "Bios Acm LockConfig Failed. Reboot in non-ltsx mode\n"));        
    DisableLtsxAndReset();
  }

  return EFI_SUCCESS ;
}

EFI_STATUS
LockLTPatch (  
  IN  EFI_MP_SERVICES_PROTOCOL  *MpService
  
  )
/*++

Routine Description:

  Lock MSR for this thread.

Arguments:

  None

Returns:

  EFI_SUCCESS           - MSR Locked

--*/
{
  UINT64              LtCtrl;

  //
  // Set LT_CONTROL_LOCK
  //
  LtCtrl = 0x1;
  AsmWriteMsr64 (EFI_MSR_LT_CONTROL, LtCtrl);

  return EFI_SUCCESS;
}

EFI_STATUS
LockLtMsr (
  VOID
  )
/*++

Routine Description:

  Lock LT MSR.

Arguments:

  None

Returns:

  EFI_SUCCESS           - Multiple processors re-load microcode patch

--*/
{
  EFI_STATUS              Status;  

  LockLTPatch (mLtDxeCtx.MpService);
  ASSERT (mLtDxeCtx.MpService);
  Status = mLtDxeCtx.MpService->StartupAllAPs (
                        mLtDxeCtx.MpService,
                        LockLTPatch,
                        TRUE,
                        NULL,
                        0,
                        (VOID *)mLtDxeCtx.MpService,
                        NULL
                        );

  return EFI_SUCCESS;
}

/*
VOID 
PciCallback (
  IN  EFI_HANDLE                                   RootBridgeHandle,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS  PciAddress,
  IN  EFI_PCI_ENUMERATION_PHASE                    Phase,
  IN  EFI_PCI_CALLBACK_CONTEXT                     *Context
  )
{
  if((!ScheckDone) && (Phase == EfiPciEnumerationResourceAssigned) ){
    LockLtMsr();
    ScheckCallback (0, NULL);
    ScheckDone = TRUE;
  }
}
*/



VOID
EFIAPI
LaunchAcm (
  IN  EFI_EVENT                             Event,
  IN  VOID                                  *Context
  )
{
  EFI_STATUS                Status;
  UINTN                     SysCfgSize;
  SYSTEM_CONFIGURATION      SysCfg;

//Aptio V Server Override - Start
//Aptio V uses guidBdsBeforeSetup instead before PciCallback
#if 0
  EFI_IIO_SYSTEM_PROTOCOL   *IioSystemProtocol = NULL;
  IIO_GLOBALS               *IioGlobalData = NULL;

  //
  // Locate IioSystemProtocol so we can make sure we lock after IioInit happened
  //
  Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &IioSystemProtocol);
  if (EFI_ERROR(Status))
    return;

  IioGlobalData = IioSystemProtocol->IioGlobalData;

  //
  // Only lock after IioInitAfterResources has completed and has not beed done yet
  //
  if (IioGlobalData->IioVData.CompletedReadyToBootEventServices == TRUE && 
       (!ScheckDone) && (Phase == EfiPciEnumerationResourceAssigned)) {
#endif

  if((!ScheckDone) /*&& (Phase == EfiPciEnumerationResourceAssigned)*/ ){

//Aptio V Server Override - End

    SysCfgSize = sizeof (SysCfg);
    Status = gRT->GetVariable (
                  L"IntelSetup",	//Aptio V Server Override
                  &gEfiSetupVariableGuid,
                  NULL,
                  &SysCfgSize,
                  &SysCfg
                  );
    //
    // If Setup variable cannot be found, lock LtMsr.
    //
    if (EFI_ERROR(Status)) {
      SysCfg.LockChipset = 1;
      SysCfg.EVMode = 0;
    }
    //
    // Only lock when disabled if LockChipset is set and EVMode is disabled
    //
    if (LtDxeEnabled || (!LtDxeEnabled && SysCfg.LockChipset && !SysCfg.EVMode))
      LockLtMsr();
    
    if (LtDxeEnabled) {
      ScheckCallback (0, NULL);
    }
    ScheckDone = TRUE;
  }
}

EFI_STATUS
EFIAPI
DriverEntry (
  IN      EFI_HANDLE                ImageHandle,
  IN      EFI_SYSTEM_TABLE          *SystemTable
  )
/*++

Routine Description:

  This is the entry point to the LT DXE Driver.  This routine checks to see if
  the platform should be configured for LT and if so, configures the platform
  by reserving and initializing LT Configuration Space and LT Device Memory and
  registering a callback to run SCHECK from the LT BIOS ACM prior to boot.

  If the platform should not be configured for LT, this routine checks the
  establishment bit in the TPM and resets it if it is asserted.

Arguments:

  ImageHandle       - A handle for this module
  SystemTable       - A pointer to the EFI System Table

Returns:

  EFI_SUCCESS   - Always.

--*/
{
  EFI_STATUS                  Status;
  UINT32                      Data32;
  //AptioV server override start: Chipset regions specific reservations are made in SDL using RSVCHIPSET
  //EFI_PHYSICAL_ADDRESS        TPMConfigBase  = TPM_CONFIG_BASE;
  //EFI_PHYSICAL_ADDRESS        TXTPublicBase  = TXT_PUBLIC_BASE;
  //EFI_PHYSICAL_ADDRESS        TXTPrivateBase = TXT_PRIVATE_BASE;
  //AptioV server override end: Chipset regions specific reservations are made in SDL using RSVCHIPSET
// Aptio V Server Override - Start : Changes done related to TXT support.
//  EFI_PCI_CALLBACK_PROTOCOL   *PciCallBackProtocol;
  VOID            *BdsEventBeforeSetupRegistration;
  EFI_EVENT 	   Event = NULL;
  EFI_GUID         guidBdsBeforeSetup = EFI_BDS_EVENT_BEFORE_SETUP_GUID;
// Aptio V Server Override - End : Changes done related to TXT support.

  //
  // Initialize the LT DXE Lib functions
  //
  Status = InitializeLtDxeLib (ImageHandle, SystemTable, &mLtDxeCtx);
  if (EFI_ERROR (Status)) {
    LtDxeEnabled = FALSE;
    DEBUG ((EFI_D_ERROR, "InitializeLtDxeLib Error: %r\n", Status));
//Start patch: For Platforms without TPM //EIP# 541656 	
    DEBUG ((EFI_D_ERROR, "Perform LT Config Lock\n"));
    LockLtMsr();  
//End patch	
    return EFI_SUCCESS;
  }

  //
  // Determine TPM presense. If not present - nothing to do.
  // Nothing but presense is precondition of this module since all
  // commands executed by BIOS ACM don't depend on TPM state.
  // TPM_NV_read will be successfully executed even if TPM is disabled
  // and/or deactivated because all indices defined for BIOS ACM
  // usage don't require authorization. TPM_ResetEstablishmentBit
  // doesn't depend on TPM state at all and can
  // be executed with disabled/deactivated TPM always.
  // Case when TPM is comletely not functional is not cinsidered.
  //
#ifndef SOFTSDV_FLAG
  if (!IsTpmPresent (&mLtDxeCtx)) {
    DEBUG ((EFI_D_ERROR, "IsTpmPresent Error\n"));
#ifdef MKF_TPM_PRESENT
    LtDxeEnabled = FALSE;
    return EFI_SUCCESS;
#endif
  }
#endif

  //
  // Detect LT capable Processor 
  //
  if (!IsLtProcessor (&mLtDxeCtx)) {
    DEBUG ((EFI_D_ERROR, "LtDxe Error: Not LT processor\n"));
    LtDxeEnabled = FALSE;
    return EFI_SUCCESS ;
  }

  Status = gBS->LocateProtocol ( 
                 &gEfiLegacyRegion2ProtocolGuid, 
                 NULL, 
                 &mLegacyRegion
                 );
  ASSERT_EFI_ERROR(Status); 

  //
  // If LT is enabled, configure platform appropriately
  //
#ifdef MKF_TPM_PRESENT
  if (IsLtEnabled (&mLtDxeCtx)) {
#else     
  if (1) {
#endif     

      // If LT_SPAD_HIGH 0xFED300A0 bit63 = 0, do legacy boot  
    Data32 = *(volatile UINT32 *)(UINTN)LT_SPAD_HIGH;
      DEBUG ((EFI_D_ERROR, "LT_SPAD_HIGH (0xFED300A4): %r\n", Data32));
    if ( (BIT31 & (*(volatile UINT32 *)(UINTN)LT_SPAD_HIGH)) == 0 ) {
      // BIOS ACM error  
      DEBUG ((EFI_D_ERROR, "Bios Acm Failed. Reboot in non-ltsx mode\n"));        
      DisableLtsxAndReset();
    }
    //
    // Initialize LT Device Memory
    //
    Status = SetupLtDeviceMemory (&mLtDxeCtx) ;
    ASSERT_EFI_ERROR (Status);
  /*
  //AptioV server override start: Chipset regions specific reservations are made in SDL using RSVCHIPSET
    //
    // Add TPM memory region as Runtime memory so that it can be reported in E820 table
    //
    Status = gDS->AddMemorySpace (
                    EfiGcdMemoryTypeMemoryMappedIo,
                    TPMConfigBase,
                    TPM_CONFIG_SIZE,
                    EFI_MEMORY_RUNTIME | EFI_MEMORY_UC
                    );
    ASSERT_EFI_ERROR(Status);

    Status = gDS->AllocateMemorySpace (
                    EfiGcdAllocateAddress,
                    EfiGcdMemoryTypeMemoryMappedIo,
                    0,
                    TPM_CONFIG_SIZE,
                    &TPMConfigBase,
                    ImageHandle,
                    NULL
                    );
    ASSERT_EFI_ERROR(Status);

    Status = gDS->SetMemorySpaceAttributes (
                    TPMConfigBase,
                    TPM_CONFIG_SIZE,
                    EFI_MEMORY_RUNTIME | EFI_MEMORY_UC
                    );
    ASSERT_EFI_ERROR (Status);

    //
    // Add TXT Public memory region as Runtime memory so that it can be reported in E820 table
    //
    Status = gDS->AddMemorySpace (
                    EfiGcdMemoryTypeMemoryMappedIo,
                    TXTPublicBase,
                    TXT_PUBLIC_SIZE,
                    EFI_MEMORY_RUNTIME | EFI_MEMORY_UC
                    );
    ASSERT_EFI_ERROR(Status);

    Status = gDS->AllocateMemorySpace (
                    EfiGcdAllocateAddress,
                    EfiGcdMemoryTypeMemoryMappedIo,
                    0,
                    TXT_PUBLIC_SIZE,
                    &TXTPublicBase,
                    ImageHandle,
                    NULL
                    );
    ASSERT_EFI_ERROR(Status);

    Status = gDS->SetMemorySpaceAttributes (
                    TXTPublicBase,
                    TXT_PUBLIC_SIZE,
                    EFI_MEMORY_RUNTIME | EFI_MEMORY_UC
                    );
    ASSERT_EFI_ERROR (Status);

    //
    // Add TXT Private memory region as Runtime memory so that it can be reported in E820 table
    //

    Status = gDS->AddMemorySpace (
                    EfiGcdMemoryTypeMemoryMappedIo,
                    TXTPrivateBase,
                    TXT_PRIVATE_SIZE,
                    EFI_MEMORY_RUNTIME | EFI_MEMORY_UC
                    );
    ASSERT_EFI_ERROR(Status);

    Status = gDS->AllocateMemorySpace (
                    EfiGcdAllocateAddress,
                    EfiGcdMemoryTypeMemoryMappedIo,
                    0,
                    TXT_PRIVATE_SIZE,
                    &TXTPrivateBase,
                    ImageHandle,
                    NULL
                    );
    ASSERT_EFI_ERROR(Status);

    Status = gDS->SetMemorySpaceAttributes (
                    TXTPrivateBase,
                    TXT_PRIVATE_SIZE,
                    EFI_MEMORY_RUNTIME | EFI_MEMORY_UC
                    );
    ASSERT_EFI_ERROR (Status);
  //AptioV server override end: Chipset regions specific reservations are made in SDL using RSVCHIPSET
*/		

//Aptio V Server Override - Start 

    //
    // Register for protocol notifactions, because ReadyToBoot is too late
    //

    Status = gBS->CreateEvent (
                         	 	 EVT_NOTIFY_SIGNAL,
                         	 	 TPL_CALLBACK,
                         	 	 LaunchAcm,
                         	 	 NULL,
                         	 	 &Event
                         	  );
    ASSERT_EFI_ERROR (Status);

    //
    // Register for protocol notifications on this event
    //

    Status = gBS->RegisterProtocolNotify (
        		 	 	 	 	 	 	 	&guidBdsBeforeSetup,
        		 	 	 	 	 	 	 	Event,
        		 	 	 	 	 	 	 	&BdsEventBeforeSetupRegistration
                         	 	 	 	 );

    ASSERT_EFI_ERROR (Status);
    DEBUG ((EFI_D_ERROR, "Register for Scheck/LockConfig Callback\n"));
// Aptio V Server Override - End 
    
  } else {
    DEBUG ((EFI_D_ERROR, "Lt not enabled\n"));
    //
    // LT is not enabled, so make sure TPM Establishment
    // bit is de-asserted
    //
    if (IsLtEstablished (&mLtDxeCtx)) {
      Status = ResetTpmEstBit (&mLtDxeCtx) ;
      ASSERT_EFI_ERROR (Status) ;
    }
  }

  return EFI_SUCCESS ;
}


