/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

Copyright (c)2009-2017 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  SmmErrorHandler.c

Abstract:

  SMM Error Logging.

------------------------------------------------------------------*/
#include <Token.h> // AptioV server override
#include <AmiDxeLib.h> // AptioV server override

#include "CommonErrorHandlerIncludes.h"
#pragma optimize("", off)

EFI_SMM_VARIABLE_PROTOCOL       *mSmmVariable;
EFI_SMM_RUNTIME_PROTOCOL        *mSmmRT = NULL;
static EFI_RUNTIME_SERVICES     *mRT;
EFI_MEM_RAS_PROTOCOL            *mMemRas = NULL;
EFI_CPU_CSR_ACCESS_PROTOCOL     *mCpuCsrAccess = NULL;
SYSTEM_CONFIGURATION             setupData;

//AptioV Server Override Start: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions
#define EFI_SMM_RUNTIME_SERVICES_TABLE_GUID \
      { 0x395c33fe, 0x287f, 0x413e, { 0xa0, 0x55, 0x80, 0x88, 0xc0, 0xe1, 0xd4, 0x3e } }

EFI_RUNTIME_SERVICES 			*gSmmpRs = NULL;
EFI_CONFIGURATION_TABLE 		*SmmConfigurationTable = NULL;
EFI_GUID SmmRtServTableGuid 		= EFI_SMM_RUNTIME_SERVICES_TABLE_GUID; 
//AptioV Server Override end: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions

/*
#ifdef  CRB_FLAG
CARD_DATA                        CardData[100];
UINT32  						 MaxCardDataCount = 0;
#endif
*/

//
// General Global values required in the Runtime
//
BOOLEAN                       mPoisonEnabled                    = FALSE;
//BOOLEAN                       mViralEnabled                     = FALSE;
BOOLEAN                       mLogIohErrors                     = FALSE;
BOOLEAN                       mNmiOnSerr                        = FALSE;
BOOLEAN                       mNmiOnPerr                        = FALSE;
BOOLEAN                       mPciSerrDetected                  = FALSE;
BOOLEAN                       mPciPerrDetected                  = FALSE;
BOOLEAN                       mPcieFatalErrDetected             = FALSE;
BOOLEAN                       mPcieNonFatalErrDetected          = FALSE;
BOOLEAN                       mPcieCorErrDetected               = FALSE;
BOOLEAN                       mNumCorrectablePcieErrs           = 0;
BOOLEAN                       mAllowClearingOfPcieDeviceStatus  = TRUE;
BOOLEAN                       mAllowClearingOfUEStatus          = TRUE;    // CV requirement
BOOLEAN                       mPcieCorrErrorEn                  = FALSE;
BOOLEAN                       mPcieUnCorrErrorEn                = FALSE;
BOOLEAN                       mPcieFatalErrorEn                 = FALSE;

EFI_MP_SERVICES_PROTOCOL      *mpService;
//EFI_QPI_RAS_PROTOCOL          *mQpiRas;
MEMORY_CFG_DATA               mMemCfgData;
EFI_HANDLE                    mImageHandle;
BOOLEAN		       			  mVtdEnable                    	= FALSE; //AptioV Server Override: Changes done to Support IOHCore/QPI/VTD Errors

static VOID                   *mWheaEventReg = NULL;
static EFI_EVENT              mWheaEvent;
BOOLEAN                       mWheaPlatformSupportAvailable = FALSE;
EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL      *mWheaPlatformSupport = NULL;

static EFI_SMM_BASE2_PROTOCOL *mSmmBase2;
EFI_SMM_SYSTEM_TABLE2*        mSmst = NULL;
EFI_SMBIOS_PROTOCOL           *mSmbiosProtocol = NULL;
PLATFORM_SLOT_INFO_PROTOCOL  *PlatformInfo = NULL;

EFI_HANDLE                    mHandle = NULL;
EFI_SMM_ERROR_LOG_PROTOCOL     *mSmmErrorLogProtocol;

//
// ELOG Host Structure
//
SMMELOG_HOST mELogHost;
SMM_ERROR_MP_SYNC_DATA mSmmErrorMpSyncData;
EFI_SMM_CPU_SERVICE_PROTOCOL  *mSmmCpuService;




//
// Host Structure pointer
//
PSYSHOST mHostPtr; 
IIO_UDS *mIioUds          = NULL;


//
// Define module globals used to register for notification of when
// the Generic ELOG protocol has been produced.
//
UINT8 *mEmcaL1DirPtr;
UINT8 *mEmcaElogDirPtr;
BIOS_ACPI_PARAM *mAcpiParameter;



VOID
EFIAPI
WheaPlatformCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
/*++

Routine Description:

  Call back function to locate the mWheaPlatformSupport protocol

Arguments:

  Event    - Event type
  *Context - Context for the event

Returns:

  None

--*/
{
  EFI_STATUS        Status;
  mWheaPlatformSupportAvailable = FALSE;    
  mWheaPlatformSupport = NULL;    
  Status = gBS->LocateProtocol (&gEfiWheaPlatformSupportProtocolGuid, NULL, (VOID **)&mWheaPlatformSupport);
  if (Status == EFI_SUCCESS) {
    mWheaPlatformSupportAvailable = TRUE;
  }
}


VOID
UpdateSmmELogHostGlobals (VOID)
/*
  Description
    This function updates local structure mELogHost used by the error log driver    

  Parameters
    none

  Returns
    none   

*/ 
{
  UINT8                       node;
  UINT8                       socket;
  UINT8                       ch;
  //UINT8                       VmseCh = 0;

  if ((mMemRas == NULL) /*|| (mQpiRas == NULL)*/) {
    ASSERT_EFI_ERROR(EFI_INVALID_PARAMETER);
    return;
  }

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    mELogHost.changedSocketBitmap = (mELogHost.socketPresentBitMap) ^ (mIioUds->SystemStatus.socketPresentBitMap);
  }

  mELogHost.socketPresentBitMap = mIioUds->SystemStatus.socketPresentBitMap;
  mELogHost.mmCfgBase           = mIioUds->SystemStatus.mmCfgBase;
  mELogHost.CpuStepping         = mIioUds->SystemStatus.MinimumCpuStepping;
  mELogHost.CpuFamily           = mIioUds->SystemStatus.cpuType;

  for( socket=0; socket < MAX_SOCKET; socket++) {
    if ((mELogHost.socketPresentBitMap & (1 << socket)) == 0) continue;
    mELogHost.busUncore[socket] = mIioUds->PlatformData.CpuQpiInfo[socket].BusNum;
    mELogHost.busIio[socket]    = mIioUds->PlatformData.IioQpiInfo[socket].BusNum;
  } // socket

  for (node = 0; node < MC_MAX_NODE; node++) {
    mELogHost.imc[node].enabled = mMemRas->SystemMemInfo->NodeInfo[node].Enabled;   // Need to use mem ras
    if (mELogHost.imc[node].enabled == 0 ) {
      continue;
    }
  /* add this for loop to take into account. Refer to HSD from Eswar.
     for (ha = 0; ha < MAX_HA; ha++) {
       mELogHost.imc[node].haList[ha].enabled = mHostPtr->nvram.common.imc[node].haList[ha].enabled;
       mELogHost.imc[node].haList[ha].haNodeId = mHostPtr->nvram.common.imc[node].haList[ha].haNodeId;
       mELogHost.imc[node].haList[ha].haNum = mHostPtr->nvram.common.imc[node].haList[ha].haNum;
       }
*/

    mELogHost.RASmode[node]     = mMemRas->SystemMemInfo->NodeInfo[node].RasModesEnabled;
    for (ch = 0; ch < MAX_CH; ch++) {
      mELogHost.imc[node].channelList[ch].chEnabled       = mMemRas->SystemMemInfo->NodeInfo[node].ChannelInfo[ch].Enabled;
    } // ch

    /*for (VmseCh = 0; VmseCh < MAX_VMSE_CH; VmseCh++) {

      if (mQpiRas->Host) {
        mELogHost.imc[node].vmseChannelEnabled[VmseCh] = mQpiRas->Host->nvram.common.imc[node].vmseChannelList[VmseCh].enabled;
      }

    }*/
  } // node
  
// node is based on socket level. Ha list[ha] we need to add the condition (the FOR loop that is
//commented out above may be used. However, Eswar will create flow to clearly outline what is needed.
//until then, we will leave the code as-is.

  //Line below is not needed for EP. Will need for EX. (RAS)
  //mMemRas->EnableMemoryErrorsPerNode = EnableElogMemoryRuntime;
}


VOID
UpdateType16AndType17SmbiosHandles(
	)
	/*++

Routine Description:

  This function updates the Type 16 and Type 17 handles and othe required information.
  
Arguments:
  
	none

Returns:

	none

--*/
{
  UINT8	Node;
  UINT8	Ch;
  UINT8	Dimm;
  EFI_STATUS	Status;
  SMBIOS_TABLE_TYPE17                     *SmbiosType17Record;
  SMBIOS_TABLE_TYPE16                     *SmbiosType16Record;
  EFI_SMBIOS_HANDLE                       SmbiosHandle;
  EFI_SMBIOS_TYPE                         SmbiosType;
  EFI_SMBIOS_TABLE_HEADER                 *SmbiosRecord;

  if (mSmbiosProtocol == NULL) {
    ASSERT_EFI_ERROR(EFI_INVALID_PARAMETER);
    return;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  SmbiosType = EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY;
  Status = mSmbiosProtocol->GetNext (mSmbiosProtocol, &SmbiosHandle, &SmbiosType, &SmbiosRecord, NULL);

  mELogHost.ElogType16Hanlde	= 00;
  if (!(EFI_ERROR(Status))) {
    SmbiosType16Record = (SMBIOS_TABLE_TYPE16 *) SmbiosRecord;
    mELogHost.ElogType16Hanlde	= (SmbiosType16Record->Hdr.Handle);
  } 
  
  ASSERT(PlatformInfo != NULL);

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  for (Node = 0; Node < MAX_MEM_NODE; Node++) { // Loop for each CPU socket
    for (Ch = 0; Ch < MAX_CH; Ch++) { // Detect DIMMs on each channel
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

        if((PlatformInfo->SocketSupportedBitmap & (1 << NODE_TO_SKT(Node))) == 0) {
          continue;
        }

        if(PlatformInfo->NodeInfo[Node].Valid == FALSE) {
          continue;
        }

        if(PlatformInfo->NodeInfo[Node].ChannelInfo[Ch].Valid== FALSE) {
          continue;
        }

        if((PlatformInfo->NodeInfo[Node].ChannelSupportedBitmap & (1 << Ch)) == 0) {
          continue;
        }

        if((PlatformInfo->NodeInfo[Node].ChannelInfo[Ch].DimmSlotSupportedBitmap & (1 << Dimm)) == 0) {
          continue;
        }
        
        mELogHost.ElogSmBiosType17[Node][Ch][Dimm].SmBiosType17Handle = 0;
        SmbiosType = EFI_SMBIOS_TYPE_MEMORY_DEVICE;
        Status = mSmbiosProtocol->GetNext (mSmbiosProtocol, &SmbiosHandle, &SmbiosType, &SmbiosRecord, NULL);
        if (!(EFI_ERROR(Status))) {
          SmbiosType17Record = (SMBIOS_TABLE_TYPE17 *) SmbiosRecord;
          mELogHost.ElogSmBiosType17[Node][Ch][Dimm].SmBiosType17Handle	= (SmbiosType17Record->Hdr.Handle);
          mELogHost.ElogSmBiosType17[Node][Ch][Dimm].DeviceLocator	= (SmbiosType17Record->DeviceLocator);
          mELogHost.ElogSmBiosType17[Node][Ch][Dimm].BankLocator	= (SmbiosType17Record->BankLocator);
        }
      }
    }
  }
}

VOID
InitMCASyncGlobals(
  VOID
)
{
  UINT32 i;
  
  mSmmErrorMpSyncData.ApExec = 0;
  mSmmErrorMpSyncData.Counter = 0;
  mSmmErrorMpSyncData.MceInProgress = FALSE;
  mSmmErrorMpSyncData.MonarchIndex = (UINT32)-1;
 mSmmErrorMpSyncData.MceSignal = FALSE;
  InitializeSpinLock (&mSmmErrorMpSyncData.LoggingLock);
  InitializeSpinLock(&mSmmErrorMpSyncData.SerialLock);
  
  for(i = 0; i < FixedPcdGet32(PcdCpuMaxLogicalProcessorNumber); i++) {
    SetMem(&mSmmErrorMpSyncData.CpuData[i],sizeof(SMM_ERROR_CPU_DATA_BLOCK),0);
    mSmmErrorMpSyncData.CpuData[i].EventLog.ErrorsFound = FALSE;
    mSmmErrorMpSyncData.CpuData[i].EventLog.Events[McbankSrc].Severity = EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTED;
    mSmmErrorMpSyncData.CpuData[i].EventLog.Events[McbankSrc].ErrorFound = FALSE;   
    InitializeSpinLock(&mSmmErrorMpSyncData.CpuData[i].ApBusy);
  }
}



VOID
InitSmmELogHostGlobals (VOID)
  /*
  Description
    This function initilizes the local structure mELogHost in DXE phase
  
  Parameters
    none

  Returns
    none   

*/
{

  EFI_STATUS                  status;
  UINT8                       socket;
  UINT32                      core;
  UINT32                      thread;
  UINTN                       numberOfCPUs;
 
  EFI_PROCESSOR_INFORMATION   processorInfoBuffer;
  UINTN                       currProcessor;
  UINTN                       CoreBits;
  UINTN                       ThreadBits;
  

  //
  // Get the Memory Info HOB Protocol if it exists.
  //

  //
  // Update the mELogHost variables.
  //
  UpdateSmmELogHostGlobals();

  //
  // Update Type 16 and Type 17 smbios handles to be useful in runtime.
  //
  UpdateType16AndType17SmbiosHandles();

  InitMCASyncGlobals();
  
  //
  // Determine the number of processors.
  //
  numberOfCPUs = mSmst->NumberOfCpus;
  mELogHost.cpuCount = numberOfCPUs;
  GetCoreAndSMTBits(&ThreadBits,&CoreBits);
  DEBUG((EFI_D_ERROR, "SMT: Corebits=%d ThreadBits=%d\n",CoreBits,ThreadBits));

  for (currProcessor = 0; currProcessor < mSmst->NumberOfCpus; currProcessor++) {
    status = mpService->GetProcessorInfo (
                              mpService,
                              currProcessor,
                              &processorInfoBuffer
                              );
    if (status == EFI_SUCCESS &&
        (processorInfoBuffer.StatusFlag & PROCESSOR_ENABLED_BIT) != 0) {

      mSmmErrorMpSyncData.CpuData[currProcessor].Present = TRUE;
      mELogHost.cpuCountEnabled++;
      
      //
      // Save CpuNumber and APIC ID info
      //
      socket  = (UINT8)(((UINT32)processorInfoBuffer.ProcessorId) >> (CoreBits+ThreadBits));
      core    = (UINT32)((processorInfoBuffer.ProcessorId >> ThreadBits) & ~((~0) << CoreBits));
      thread  = (UINT32)(processorInfoBuffer.ProcessorId & ~((~0) << ThreadBits));
      
      if (socket < MAX_SOCKET && core < MAXCOREID && thread < MAX_THREAD) {
        mELogHost.cpuInfo[socket].Valid[core][thread] = TRUE;
        mELogHost.cpuInfo[socket].cpuNumber[core][thread] = currProcessor;
        mELogHost.cpuInfo[socket].ApicId[core][thread]    = ((UINT32)processorInfoBuffer.ProcessorId);
        mELogHost.cpuInfo[socket].ApicVer[core][thread]   = 2;
      } else {
        //If we get here, MAXCOREID needs to be updated.  
        DEBUG((EFI_D_ERROR, "ERROR: ApicId convertion in InitSmmELogHostGlobals may be wrong\n"));
        ASSERT(FALSE);
      }
    }
  }
}

VOID
InitSMMErrorLogGlobals(VOID)
{
	InitSmmELogHostGlobals(); 
	InitErrDevInfo();
	InitIOHInfo();
}

BOOLEAN
EfiInManagementInterrupt (
  VOID
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:


Returns:

  GC_TODO: add return values

--*/
{
   
  BOOLEAN InSmm;

  if (mSmmBase2 == NULL) {
    InSmm = FALSE;
  } else {
    mSmmBase2->InSmm (mSmmBase2, &InSmm);
    mSmmBase2->GetSmstLocation (mSmmBase2, &mSmst);
  }

  return InSmm;
}

EFI_SMM_RUNTIME_PROTOCOL *
GetSmmRt (
  IN  VOID
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:


Returns:

  if succuess reurns mSmmRT->SmmRuntime
  if failure returns NULL

--*/
{
  if (mSmmRT == NULL) {
    if (EFI_ERROR (gBS->LocateProtocol (&gEfiSmmRuntimeProtocolGuid, NULL, &mSmmRT))) {
      mSmmRT = NULL;
    } else {
      if (mSmmRT->SmmRuntime.Hdr.Signature == EFI_RUNTIME_SERVICES_SIGNATURE) {
        mRT = &(mSmmRT->SmmRuntime);
      }
    }
  }

  return mSmmRT;
}


VOID
LocateWheaPlatformProtocol (
  VOID
  )
/*++

Routine Description:

  This function is used to locate the mWheaPlatformSupportProtocol protocol

Arguments:
  
  ErrorLogSetupData : 

Returns:

  none

--*/
{
  EFI_STATUS Status;

  //
  // Locate the WHEA Platform driver and initialize the module globals.
  //
	mWheaPlatformSupportAvailable = FALSE;
	mWheaPlatformSupport		  = NULL;
	 
	Status = gBS->LocateProtocol (&gEfiWheaPlatformSupportProtocolGuid, NULL, (VOID **)&mWheaPlatformSupport); 
   
	if (Status == EFI_SUCCESS && mWheaPlatformSupport != NULL) {
	  mWheaPlatformSupportAvailable = TRUE;
	} else {
	  //
	  // Find Whea Platform protocol instance
	  //
	  mWheaPlatformSupport = NULL;	  

		Status = gBS->CreateEvent (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, WheaPlatformCallback, NULL, &mWheaEvent);
		if(EFI_ERROR(Status))  return;

		Status = gBS->RegisterProtocolNotify (&gEfiWheaPlatformSupportProtocolGuid, mWheaEvent, &mWheaEventReg);
		if(EFI_ERROR(Status))  return;
	}
}

VOID
InitializePPRData(
    IN ERROR_LOG_SETUP_DATA  *ErrorLogSetupData
 )
{

  EFI_STATUS          status;


  //
  // Delete PPR Variable from storage
  //
  status = gRT->SetVariable (
      PPR_VARIABLE_NAME,
      &gEfiPprVariableGuid,
      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
      0,
      NULL
  );
}

VOID
SetupInterface (
  IN ERROR_LOG_SETUP_DATA  *ErrorLogSetupData
  )
  /*++

Routine Description:

 Access setup variables and updates in the local structures to be used in the error log driver

Arguments:
  
  ErrorLogSetupData : 

Returns:

  none

--*/
{
  EFI_STATUS            status = EFI_SUCCESS;
  UINTN                 varSize;


  varSize = sizeof(SYSTEM_CONFIGURATION);
// AptioV server override: Changed variable name to Intelsetup
  status = gRT->GetVariable( 
                  L"IntelSetup",
                  &gEfiSetupVariableGuid,
                  NULL,
                  &varSize,
                  &setupData 
                  );
	if( EFI_ERROR(status)) return;
	// Clear the ErrrorLogSetUpData structure.
	ZeroMem (ErrorLogSetupData, sizeof (ERROR_LOG_SETUP_DATA));

	ErrorLogSetupData->SystemErrorEn				= setupData.SystemErrorEn;
	// Aptio V Server Override Start   : Changes done to handle the Memory Correctable Erros (Default Memory Handler) when RankSparing/DevTagging setup option is disabled. 	
	ErrorLogSetupData->dddcSupport       = setupData.dddcSupport;
	ErrorLogSetupData->DeviceTaggingMode = setupData.DeviceTaggingMode;	
	ErrorLogSetupData->enableSparing     = setupData.enableSparing;	
	// Aptio V Server Override End   : Changes done to handle the Memory Correctable Erros (Default Memory Handler) when RankSparing/DevTagging setup option is disabled. 		
	if( ErrorLogSetupData->SystemErrorEn ) {
    ErrorLogSetupData->PoisonEn                    = setupData.PoisonEn;
	    /*ErrorLogSetupData->ViralEn                     = setupData.ViralEn;
		ErrorLogSetupData->LerEnable                   = setupData.LerEnable;
		if (ErrorLogSetupData->LerEnable)
			ErrorLogSetupData->DisableC4LoggingDueToLER= setupData.DisableC4LoggingDueToLER;
    */
    ErrorLogSetupData->EMcaLogEn                      = setupData.EMcaLogEn;
    ErrorLogSetupData->EMcaCSmiEn                  = setupData.EMcaCSmiEn;
    ErrorLogSetupData->EMcaForceOptin              = setupData.EMcaForceOptin;
	
    ErrorLogSetupData->CloakingEn                  = setupData.CloakingEn;	
		
/*    
    // Aptio V Server Override Start : Enable MceSmiMorphingEn Setup option & disable UboxUmcSmiEn Setup option by default when EMcaLogEn Setup option is enabled.  
    // 								 : Enable UboxUmcSmiEn Setup option & disable MceSmiMorphingEn Setup option by default when EMcaLogEn Setup option is disabled.
	ErrorLogSetupData->MceSmiMorphingEn              = setupData.MceSmiMorphingEn;
	ErrorLogSetupData->UboxUmcSmiEn              = setupData.UboxUmcSmiEn;*/
    
    if (setupData.EMcaLogEn)
    {
    	ErrorLogSetupData->MceSmiMorphingEn          = 1; // Enable MCESMI
    	ErrorLogSetupData->UboxUmcSmiEn              = 0; // DISABLE UBOX
    }
    else
    {
    	ErrorLogSetupData->MceSmiMorphingEn          = 0; // Disable MCESMI
    	ErrorLogSetupData->UboxUmcSmiEn              = 1; // ENABLE UBOX
    }

/*    
// Aptio V Server Override End : Enable MceSmiMorphingEn Setup option & disable UboxUmcSmiEn Setup option by default when EMcaLogEn Setup option is enabled.  
// 								 : Enable UboxUmcSmiEn Setup option & disable MceSmiMorphingEn Setup option by default when EMcaLogEn Setup option is disabled.
 */
    /*
		if( ErrorLogSetupData->EMcaLogEn )	{
			ErrorLogSetupData->IoMcaEn				= setupData.IoMcaEn;
		}*/

		//Whea Setup Options
		ErrorLogSetupData->WheaSupport				= setupData.WheaSupport;
		if( ErrorLogSetupData->WheaSupport)	{
			ErrorLogSetupData->WheaLogging			= setupData.WheaLogging;
			// WheaErrorRecordFormat Value 0 -> UEFI 2.2 ; value 1 -> uefi 2.3.1
			if( setupData.WheaErrorRecordFormat == 1)
				ErrorLogSetupData->WheaErrorRecordFormat	= GENERIC_ERROR_SECTION_REVISION_UEFI231;
			else 
				ErrorLogSetupData->WheaErrorRecordFormat	= GENERIC_ERROR_SECTION_REVISION;
		}
		else {
			ErrorLogSetupData->WheaLogging				    = 00;
		}
  	ErrorLogSetupData->IohErrorEn               = setupData.IohErrorEn;
		ErrorLogSetupData->PchErrorEn               = setupData.PchErrorEn;
    ErrorLogSetupData->ErrorPinProgrammingForIIO = setupData.ErrorPinProgrammingForIIO;

		// IIO specific errors
		if( ErrorLogSetupData->IohErrorEn ) {
#if defined(BDX_HOST) && defined(DE_SKU) 
          ErrorLogSetupData->VtdErrorEn                = setupData.VtdErrorEn;
          ErrorLogSetupData->MiErrorEn                 = setupData.MiErrorEn;
          ErrorLogSetupData->CoherentInterfaceErrorEn  = setupData.CoherentInterfaceErrorEn;
          ErrorLogSetupData->DmaErrorEn                = setupData.DmaErrorEn;
          ErrorLogSetupData->IioCoreErrorEn            = setupData.IioCoreErrorEn;
          ErrorLogSetupData->DmiErrorEn                = setupData.DmiErrorEn;
#endif
 
    //Options for Masking Errors. 
    ErrorLogSetupData->irpp0_parityError           = setupData.irpp0_parityError;
    ErrorLogSetupData->irpp0_qtOverflow            = setupData.irpp0_qtOverflow;
    ErrorLogSetupData->irpp0_unexprsp              = setupData.irpp0_unexprsp;
    ErrorLogSetupData->irpp0_csraccunaligned       = setupData.irpp0_csraccunaligned;
    ErrorLogSetupData->irpp0_uncecc                = setupData.irpp0_uncecc;
    ErrorLogSetupData->irpp0_rcvdpoison            = setupData.irpp0_rcvdpoison;
    ErrorLogSetupData->irpp0_crrecc                = setupData.irpp0_crrecc;
    ErrorLogSetupData->irpp1_parityError           = setupData.irpp1_parityError;
    ErrorLogSetupData->irpp1_qtOverflow            = setupData.irpp1_qtOverflow;
    ErrorLogSetupData->irpp1_unexprsp              = setupData.irpp1_unexprsp;
    ErrorLogSetupData->irpp1_csraccunaligned       = setupData.irpp1_csraccunaligned;
    ErrorLogSetupData->irpp1_uncecc                = setupData.irpp1_uncecc;
    ErrorLogSetupData->irpp1_rcvdpoison            = setupData.irpp1_rcvdpoison;
    ErrorLogSetupData->irpp1_crrecc                = setupData.irpp1_crrecc;
  }

		// PCI/Pci-ex errors
		ErrorLogSetupData->PcieErrorsEn				= setupData.PcieErrorsEn;
    if( ErrorLogSetupData->PcieErrorsEn ) {
			ErrorLogSetupData->PCIEFatalErrorEn			= setupData.PCIEFatalErrorEn;
			ErrorLogSetupData->PCIEUnCorrErrorEn		= setupData.PCIEUnCorrErrorEn;
			ErrorLogSetupData->PCIECorrErrorEn			= setupData.PCIECorrErrorEn;
			ErrorLogSetupData->PropagateSerr			  = setupData.PropagateSerr; 
			ErrorLogSetupData->PropagatePerr			  = setupData.PropagatePerr;
			ErrorLogSetupData->PcieCeErrThreshold        = setupData.PcieCeErrThreshold;
  }

      ErrorLogSetupData->PcieExtndErrorsEn    = setupData.PcieExtndErrorsEn;

      ErrorLogSetupData->CorrectedMemoryErrors = setupData.CorrectedMemoryErrors;
    
    if(ErrorLogSetupData->CorrectedMemoryErrors)	{

			ErrorLogSetupData->SpareInt   				  = setupData.SpareInt;
#ifndef GRANGEVILLE_FLAG
			ErrorLogSetupData->MirrorFailOver		    = setupData.MirrorFailOver;
#endif	// GRANGEVILLE_FLAG		
  }
// Aptio V Server Override Start: Unlocking the MSR 0x790 for the ErrorInjection to work from RAS tools
#if ENABLE_ERROR_INJECTOR_SUPPORT
	ErrorLogSetupData->UnlockMsr		            	= setupData.UnlockMsr;
#else
	ErrorLogSetupData->UnlockMsr		            	= 0;
#endif
// Aptio V Server Override End: Unlocking the MSR 0x790 for the ErrorInjection to work from RAS tools

 		//ErrorLogSetupData->VmseNbPersistentError		= setupData.VmseNbPersistentError;  //
		//ErrorLogSetupData->VmseSbPersistentError		= setupData.VmseSbPersistentError;
		//ErrorLogSetupData->VmseLinkFailure				  = setupData.VmseLinkFailure;
		//ErrorLogSetupData->VmseDdrParityError		    = setupData.VmseDdrParityError;

		// QPI Errors
		ErrorLogSetupData->QPIFailoverSmiEn					= setupData.QPIFailoverSmiEn;

  }

	// ErrorlogSetupdata memory variables initilization which are not included in setup.
	//ErrorLogSetupData->HaCorrErrSmiEn						= 0;
	//ErrorLogSetupData->iMcCorrErrSmiEn					= 0;

	//ErrorLogSetupData->MirrCorrected					  = 0;
	//ErrorLogSetupData->VmseErrorObserved		    = 0;
//AptioV Server Override Start: Changes done to Support IOHCore/QPI/VTD Errors
	if (setupData.VTdSupport == 1) {
		mVtdEnable = TRUE;
	} else {
		mVtdEnable = FALSE;
	}
//AptioV Server Override End: Changes done to Support IOHCore/QPI/VTD Errors
  ConfigOEMSetupData(ErrorLogSetupData);
  UpdateSetupGlobalVariables(ErrorLogSetupData);
  InitializePPRData(ErrorLogSetupData);
}

// Aptio V Server Override Start- Unlocking the MSR 0x790 for the ErrorInjection to work from RAS tools

VOID
UnLockMSR790 (
  VOID *Buffer
  )
{
	UINT64 Data;

	Data = AsmReadMsr64(MC_ERR_INJ_LCK);
	Data &= ~(BIT0);
	AsmWriteMsr64 (MC_ERR_INJ_LCK, Data);
	return;
}
VOID
UnLockInjLogic()
{
	 UINTN       thread;
	 for (thread = 0; thread < mELogHost.cpuCount; thread++) {
	    mSmst->SmmStartupThisAp (UnLockMSR790,
	                             thread, 
	                             NULL);
	 }
	 UnLockMSR790(&thread);
}
//Aptio V Server Override End - Unlocking the MSR 0x790 for the ErrorInjection to work from RAS tools

// AptioV Server Override Start: Support added for Mode1 Memory Error Reporting
VOID
EnableMode1 (
  VOID *Buffer
  )
{
	UINT64 Data;

	Data = AsmReadMsr64(MC_ERROR_CONTROL);
	Data |= (BIT1);
	AsmWriteMsr64 (MC_ERROR_CONTROL, Data);
	return;
}

VOID
EnableMode1MemErrorReporting()
{
	 UINTN       thread;
	 for (thread = 0; thread < mELogHost.cpuCount; thread++) {
	    mSmst->SmmStartupThisAp (EnableMode1,
	                             thread, 
	                             NULL);
	 }
	 EnableMode1(&thread);
}
// AptioV Server Override End: Support added for Mode1 Memory Error Reporting

VOID
ClearSystemErrors(
  IN UINT32 SkipFlag)
/*++

Routine Description:

  Clear all system errors before enabling them.

Arguments:
  SkipFlag  -- to distinguish if need to clear MC CE counter.
  

Returns:

  none

--*/
{  
  ClearCoreErrors();
  ClearUnCoreErrors(SkipFlag);
  ClearPCHErrors();
  ClearPlatformErrors();
}

VOID
ClearCoreErrors()
/*++

Routine Description:

  Clear all Core errors before enabling them.

Arguments:
  
  ErrorLogSetupData : Setup data for error logging

Returns:

  none


--*/
{ 

	UINT8 Skt;

    if (mIioUds != NULL) {	
		for(Skt=0; Skt < MAX_CPU_SOCKETS; Skt++) {
		  //Note: after mQpiRas data structure gets ported over from BKLND, need to update the line below to use mQpiRas instead.
                  if(mIioUds->PlatformData.CpuQpiInfo[Skt].Valid == TRUE) {
			ClearPCUErrors(Skt);
		  }
		}
    }
	else
		ASSERT_EFI_ERROR (0);
	return;
}


VOID
ClearUnCoreErrors(
  IN UINT32 SkipFlag)
/*++

Routine Description:

  Clear all UnCore errors before enabling them.

Arguments:
  
  ErrorLogSetupData : Setup data for error logging

Returns:

  none

--*/
{
    ClearCBOErrors();
    ClearUboxAndMiscErrorStatus();
    if((SkipFlag & SKIP_MC_CE_CNT_FLAG) == 0){
       ClearMemoryErrors();
    }
    ClearQPIErrors();	
    ClearIIOErrors();
}

VOID
UpdateSetupGlobalVariables(
	IN ERROR_LOG_SETUP_DATA  *ErrorLogSetupData
	)
/*++

Routine Description:

  This function Enables or disables the memory, IIO , QPI PCH errors.

Arguments:
  
  ErrorLogSetupData : 

Returns:

  none

--*/
{
    // Global variables related to Poison and Viral start here
	mErrLogSetupData	= ErrorLogSetupData;
	mPoisonEnabled = FALSE; //Moved this line here until we get an EX wrapper of some sort.
	if (mErrLogSetupData->PoisonEn == 1) {
	  mPoisonEnabled = TRUE;
	} /*else if (mErrLogSetupData->ViralEn == 1) {
	  mPoisonEnabled = TRUE;	  // Enable Posion also when Viral enabled
	} else {
	  mPoisonEnabled = FALSE;
	}
	if (mErrLogSetupData->ViralEn == 1) {
	  mViralEnabled = TRUE;
	} else {
	  mViralEnabled = FALSE;
	}*/
    // Global variables related to Poison and Viral End here

    mWheaFfmCorErrLogging	= FALSE;
    mWheaFfmUnCorErrLogging	= FALSE;
    if( mErrLogSetupData->WheaLogging == 1)	{
  	  mWheaFfmCorErrLogging	= TRUE;
	  mWheaFfmUnCorErrLogging	= TRUE;
    }
	
    if ( mErrLogSetupData->MceToSmiEn == 1) {
	  mErrLogSetupData->UmcSmiEnable = 00;
    } else {
	  if (mErrLogSetupData->UmcSmiEnable == 1) {
		mErrLogSetupData->UmcSmiEnable = TRUE;
	  }
    }

    mHaCmciOnCorrecetdThr		= 01; 


    if (mErrLogSetupData->SpareInt == 0) {
      mSpareIntSelect |= SPARE_INT_SMI;
    } else if (mErrLogSetupData->SpareInt == 1) {
      mSpareIntSelect |= SPARE_INT_CMCI;
    } else {
      mSpareIntSelect |= SPARE_INT_PIN;
    }

    if(ErrorLogSetupData->ErrorPinProgrammingForIIO == V_SYSMAP_NoInband)  {
      mErrorPinEnabled = TRUE;
    }
	
    /*if(ErrorLogSetupData->EMcaLogEn == TRUE && ErrorLogSetupData->IoMcaEn == TRUE) {
      mErrorIOMCAEnabled = TRUE; 
    }*/

    if (ErrorLogSetupData->PropagateSerr == 1) {
      mNmiOnSerr = TRUE;
    }
    if (ErrorLogSetupData->PropagatePerr == 1) {
      mNmiOnPerr = TRUE;  
    }
	// Global variables related to memory errors end here
}

VOID
ProgramErrorSignals(
	VOID
	)
{
	//
	// Enable Cloaking 
	//
	if (mIioUds->SystemStatus.cpuType == CPU_BDX) {
    if(mErrLogSetupData->CloakingEn == TRUE) {
		  EnableCloaking ();
    }
		}
		
	//
    // Enable memory error enbaling
    //
	EnableElogMemory();


#ifndef GRANGEVILLE_FLAG
    EnableCSMI();
#endif	// GRANGEVILLE_FLAG

	//
	// Enable QPI errors
	//
#ifndef DE_SKU
	EnableElogQPI();
#endif

    //
    // Enable IO Hub(IOHub) Error Reporting (Like MCH, SNC etc). BE SURE THIS ONE CALLED FIRST
    //
	EnableElogIoHub ();

  //
  // Enable UboxError to PCU mask.
  //
  EnableUboxError();

}

VOID
MaskDMIErrorsToPreventSMIFlood()
{
	XPUNCERRMSK_IIO_PCIE_STRUCT  XpunMask;

    XpunMask.Data =0;
    XpunMask.Bits.received_pcie_completion_with_ur_status_mask =1;
    XpunMask.Bits.received_pcie_completion_with_ca_status_mask =1;		
    PciExpressWrite32 (PCI_PCIE_ADDR(0, 0, 0, ONLY_REGISTER_OFFSET(XPUNCERRMSK_IIO_PCIE_REG)), XpunMask.Data); 
}



VOID
ProgramErrorSignalAndServerity(
	IN ERROR_LOG_SETUP_DATA  *ErrorLogSetupData
	)
/*++

Routine Description:

  This function Enables or disables the memory, IIO , QPI PCH errors.

Arguments:
  
  ErrorLogSetupData : 

Returns:

  none

--*/
{

    //
    //on Brickland/grantley we are seeing SMI flood due to received_pcie_completion_with_ur_status
    //and received_pcie_completion_with_ca_status errors.  BIOS will not boot unless
    //these errors are masked. 
    //
    MaskDMIErrorsToPreventSMIFlood();

    //
    // Enbale or disable Poison forwarding & viral 
    //
    ProgramViralPoison();

	//
    // Program Error Signals
    //
    ProgramErrorSignals();	
}

EFI_STATUS
ClearSystemErrorsCallback (
  IN  EFI_HANDLE                          DispatchHandle,
  IN  CONST EFI_SMM_SW_REGISTER_CONTEXT   *DispatchContext, OPTIONAL
  IN OUT VOID                             *CommBuffer,     OPTIONAL
  IN OUT UINTN                            *CommBufferSize  OPTIONAL
  )
{
  ClearSystemErrors(0);

  return EFI_SUCCESS;

}

EFI_STATUS
WmaaCallBack (
  IN  EFI_HANDLE                          DispatchHandle,
  IN  CONST EFI_SMM_SW_REGISTER_CONTEXT   *DispatchContext, OPTIONAL
  IN OUT VOID                             *CommBuffer,     OPTIONAL
  IN OUT UINTN                            *CommBufferSize  OPTIONAL
  )
{
  NmiSciUpdateHandler();

  return EFI_SUCCESS;
}

VOID
InitializeEDKProtocols()
{
  EFI_STATUS            Status;
  EFI_IIO_UDS_PROTOCOL *iioUdsProtocol;
  EFI_GLOBAL_NVS_AREA_PROTOCOL    *AcpiNvsProtocol = NULL;

	// Locate MemRas Protocol
  Status = gSmst->SmmLocateProtocol (&gEfiMemRasProtocolGuid, NULL, &mMemRas);
  ASSERT_EFI_ERROR(Status);

  Status = gSmst->SmmLocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess);
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmLocateProtocol(&gEfiSmmCpuServiceProtocolGuid, NULL, &mSmmCpuService);

  Status = gBS->LocateProtocol (
                  &gEfiGlobalNvsAreaProtocolGuid, 
                  NULL, 
                  &AcpiNvsProtocol
                  );
  ASSERT_EFI_ERROR (Status);
  mAcpiParameter = AcpiNvsProtocol->Area;
  ASSERT (mAcpiParameter);

  Status = gBS->LocateProtocol(&gPlatformSlotInfoProtocolGuid, NULL,&PlatformInfo);
  ASSERT_EFI_ERROR (Status);

  //
  // Locate the IIO Protocol Interface
  //
  Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid,NULL, &iioUdsProtocol);
  ASSERT_EFI_ERROR (Status);
  mIioUds = iioUdsProtocol->IioUdsPtr;
	
    // Locate QpiRas Protocol
	//Status = gSmst->SmmLocateProtocol (&gEfiQpiRasProtocolGuid, NULL, &mQpiRas);
	//ASSERT_EFI_ERROR (Status);

	// Locate SmBios Protocol
  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &mSmbiosProtocol);
	if (EFI_ERROR(Status)) mSmbiosProtocol = NULL;

  //
  // Find the MP Protocol. This is an MP platform, so MP protocol must be
  // there.
  //
  Status = gBS->LocateProtocol (
          &gEfiMpServiceProtocolGuid,
          NULL,
          &mpService
          );
  ASSERT_EFI_ERROR (Status);

	// Locate WheaPlatform Protocol
    LocateWheaPlatformProtocol ();
	Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (EFI_SMM_ERROR_LOG_PROTOCOL), &mSmmErrorLogProtocol);
	ASSERT_EFI_ERROR (Status);
	ZeroMem (mSmmErrorLogProtocol, sizeof (EFI_SMM_ERROR_LOG_PROTOCOL));
	mSmmErrorLogProtocol->PropagateSignals = PropagateSignals;
	mSmmErrorLogProtocol->DisableCSMI = DisableCSMI;
	mSmmErrorLogProtocol->EnableCSMI = EnableCSMI;
	mSmmErrorLogProtocol->EnableSMIForCorrectedMemoryErrors = EnableSMIForCorrectedMemoryErrors;

	//Install SmmErrorLogProtocol
	  Status = gSmst->SmmInstallProtocolInterface (
      &mHandle,
      &gEfiSmmErrorLogProtocolGuid,
      EFI_NATIVE_INTERFACE,
      mSmmErrorLogProtocol
  );
  ASSERT_EFI_ERROR (Status);
  
  DEBUG ((DEBUG_ERROR, "mSmmErrorLogProtocol = %lx \n", mSmmErrorLogProtocol));
 // DEBUG ((DEBUG_ERROR, "mSmmErrorLogProtocol->PropagateSignals() Address = %lx \n", mSmmErrorLogProtocol->PropagateSignals()));
	
}
   
VOID
InitializeWhea(
	void
	)
{

      }

VOID
InitializeMCA(
  void
)
{
  if(mErrLogSetupData->EMcaLogEn == 1 && EfiInManagementInterrupt ()) {
    if(InitEMCA(&mEmcaL1DirPtr,&mEmcaElogDirPtr) != EFI_SUCCESS) {
      mErrLogSetupData->EMcaLogEn = 0; //Disable Emca Logging      
      mAcpiParameter->EmcaL1DirAddr = 0;   
      DEBUG ((EFI_D_ERROR, "ERROR: While creating L1 Directory - disabling emca"));
    } else {
      mAcpiParameter->EmcaL1DirAddr = (UINT64)mEmcaL1DirPtr;
      DEBUG ((EFI_D_ERROR, "EmcaL1DirAddr = 0x%08x\n", mAcpiParameter->EmcaL1DirAddr));
    }
  }
}

VOID
InitializeUserConfiguration(
	IN ERROR_LOG_SETUP_DATA  *ErrorLogSetupData 
	)
/*
Routine Description:

  This function updates the ERROR_LOG_SETUP_DATA with setup and other platform needed information

Arguments:
  
  ErrorLogSetupData : 

Returns:

  none


*/
{
	// setup Varibale update
	SetupInterface(ErrorLogSetupData);

	// Update EDKII proitocols used.
	InitializeEDKProtocols();

	//InitializeErrorStructure();

	// Init host structure globals
    InitSMMErrorLogGlobals ();	

	InitializeMCA();

	InitializeWhea();
}

/*
#ifdef CRB_FLAG
EFI_STATUS ReadCardData(
	IN	EFI_HANDLE					  DispatchHandle,
	IN CONST EFI_SMM_SW_REGISTER_CONTEXT  *DispatchContext,
	IN OUT	 VOID						  *CommBuffer,	   OPTIONAL
	IN OUT	 UINTN						  *CommBufferSize  OPTIONAL
  )
{
    UINT8 RootBridgeLoop, Port, Function, Device;
    UINT16 CapabilitiesOffset;
	UINT8  SecondaryBus = 0;
	UINT8 i, j;

    for ( RootBridgeLoop =0; RootBridgeLoop < MAX_SOCKET; RootBridgeLoop++) {
   		if(mIohInfo[RootBridgeLoop].Valid != 1) continue;

	    for(Port = 0; Port < NUMBER_PORTS_PER_SOCKET; Port++) {

	      Device = mIohInfo[RootBridgeLoop].Port[Port].Device;
	      Function = mIohInfo[RootBridgeLoop].Port[Port].Function;

		  if (!PCIeIsRootPort(mIohInfo[RootBridgeLoop].BusNum, Device, Function)) {
	          continue;
	      }

		  // Get the capability offset for LER   
		  CapabilitiesOffset = PciErrLibGetLERCapabilitiesOffset(mIohInfo[RootBridgeLoop].BusNum, Device, Function);
	      if (CapabilitiesOffset == 0)
	         continue;

		  //Check if PCI bridge
		   
		  if (!(PciErrLibIsPciBridgeDevice (mIohInfo[RootBridgeLoop].BusNum, Device, Function))) 
		  	  continue;
          SecondaryBus = PciExpressRead8(PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device,Function, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));

          if (SecondaryBus == 0)
              continue;

		  for (i=0; i<MAX_PCI_DEVICES; i++){
			  for (j=0; j<MAX_PCI_FUNCTIONS; j++){
				  if (!PciErrLibIsDevicePresent (SecondaryBus, i, j)) {
					  continue;
				  }else{
				      CardData[MaxCardDataCount].RootPortBus = mIohInfo[RootBridgeLoop].BusNum;
					  CardData[MaxCardDataCount].RootPortDevice  = Device;
					  CardData[MaxCardDataCount].RootPortFunction = Function;
				      CardData[MaxCardDataCount].PcieSlotBus = SecondaryBus;
					  CardData[MaxCardDataCount].PcieSlotDevice  = i;
					  CardData[MaxCardDataCount].PcieSlotFunction = j;
					  CardData[MaxCardDataCount].PCICommand = PciExpressRead32(PCI_PCIE_ADDR(SecondaryBus, i,j, PCI_COMMAND_OFFSET));
					  CardData[MaxCardDataCount].Remap1 = PciExpressRead32(PCI_PCIE_ADDR(SecondaryBus, i,j, REMAP1_OFFSET));
					  CardData[MaxCardDataCount].Remap2 = PciExpressRead32(PCI_PCIE_ADDR(SecondaryBus, i,j, REMAP2_OFFSET));
					  CardData[MaxCardDataCount].Remap3 = PciExpressRead32(PCI_PCIE_ADDR(SecondaryBus, i,j, REMAP3_OFFSET));
					  CardData[MaxCardDataCount].Remap4Low = PciExpressRead32(PCI_PCIE_ADDR(SecondaryBus, i,j, REMAP4LOW_OFFSET));
					  CardData[MaxCardDataCount].Remap4High= PciExpressRead32(PCI_PCIE_ADDR(SecondaryBus, i,j, REMAP4HIGH_OFFSET));
					  CardData[MaxCardDataCount].PCIInterruptLine = PciExpressRead32(PCI_PCIE_ADDR(SecondaryBus, i,j, PCIINTERRUPT_LINE__OFFSET));
					  CardData[MaxCardDataCount].PCIExpLinkCtrl = PciExpressRead32(PCI_PCIE_ADDR(SecondaryBus, i,j, PCIEXP_LINKCTRL_OFFSET));
					  CardData[MaxCardDataCount].LinkDbgCtrl = PciExpressRead32(PCI_PCIE_ADDR(SecondaryBus, i,j, LINK_DBG_CTRL_OFFSET));
					  CardData[MaxCardDataCount].DFS1Register = PciExpressRead32(PCI_PCIE_ADDR(SecondaryBus, i,j, DFS1REGISTER_OFFSET));
					  CardData[MaxCardDataCount].TLCompletionTimeout = PciExpressRead32(PCI_PCIE_ADDR(SecondaryBus, i,j, TLCOMPLETION_TIMEOUT_OFFSET));					  	
					  MaxCardDataCount++;
				  } 				  
			  }
		  }	
	   }	  
	}
	return EFI_SUCCESS;
}
#endif
*/

//AptioV Server Override Start: Clear the Errors which are coming in Cold Boot.
VOID
ClearColdBootErrors()
{
  UINT8       Port;
  UINT8       RootBridgeLoop;

  ClearSystemErrors(0);

  for ( RootBridgeLoop = 0; RootBridgeLoop < MAX_SOCKET; RootBridgeLoop++) {
      if (mIohInfo[RootBridgeLoop].Valid != 1)
          continue;
      MaskIIOErrorReporting();
      //
      // Clear Global registers device status
      //
      ClearIIOErrorRegisterStatus(mIohInfo[RootBridgeLoop].SocketId);
      for(Port = 0; Port < NUMBER_PORTS_PER_SOCKET; Port++) {
          //
          // Check if the root port is enabled.
          //
          if (!PciErrLibIsDevicePresent (mIohInfo[RootBridgeLoop].BusNum, mIohInfo[RootBridgeLoop].Port[Port].Device, mIohInfo[RootBridgeLoop].Port[Port].Function)) {
              continue;
          }
          ClearIohExtPcieDeviceStatus (mIohInfo[RootBridgeLoop].BusNum, mIohInfo[RootBridgeLoop].Port[Port].Device, mIohInfo[RootBridgeLoop].Port[Port].Function);
          PciErrLibClearPcieDeviceStatus(mIohInfo[RootBridgeLoop].BusNum, mIohInfo[RootBridgeLoop].Port[Port].Device, mIohInfo[RootBridgeLoop].Port[Port].Function);
      }
  }
}
//AptioV Server Override End: Clear the Errors which are coming in Cold Boot.

//AptioV Server Override Start: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions
UpdateSmmRsPointer(
    )
{    
    UINTN            NumberOfTableEntries;
    UINT8		     Index;  

    if (SmmConfigurationTable == NULL) {
        SmmConfigurationTable = gSmst->SmmConfigurationTable;
        NumberOfTableEntries = gSmst->NumberOfTableEntries;
        for (Index = 0; Index < NumberOfTableEntries; Index++) {
            if (CompareGuid (&SmmConfigurationTable[Index].VendorGuid, &SmmRtServTableGuid)) {
         	  gSmmpRs = SmmConfigurationTable[Index].VendorTable;
            }
        }
    }
    if (gSmmpRs == NULL) {
         ASSERT_EFI_ERROR (EFI_NOT_FOUND);
    }
}
//AptioV Server Override End: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions    
//
// Driver entry point
//

ERROR_LOG_SETUP_DATA       errorLogSetupData;

EFI_STATUS
InitializeSmmErrorLog (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS                        Status;
  EFI_SMM_SW_REGISTER_CONTEXT       SwContext;
  EFI_SMM_SW_DISPATCH2_PROTOCOL     *SwDispatch  = 0;
  EFI_HANDLE                        SwHandle;  
  VOID                              *Registration;
// AptioV Server Override Start: Added to skip LastBoot Error Handler for cold boot.
  struct SystemMemoryMapHob             *systemMemoryMap;
  EFI_HOB_GUID_TYPE                     *GuidHob;
// AptioV Server Override End: Added to skip LastBoot Error Handler for cold boot.
  Status = gBS->LocateProtocol (
                  &gEfiSmmBase2ProtocolGuid,
                  NULL,
                  (VOID **) &mSmmBase2
                  );

  if (EfiInManagementInterrupt ()) {

    // Updates the ERROR_LOG_SETUP_DATA with setup and other platform RELATED information
    InitializeUserConfiguration(&errorLogSetupData);

    //
    // Install SW SMI handler to clear system errors at end of the POST
    //
    Status = mSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, &SwDispatch);
    ASSERT_EFI_ERROR (Status);
   
    SwContext.SwSmiInputValue = CLEAR_ERROR_SWSMI;
    Status                    = SwDispatch->Register (SwDispatch, ClearSystemErrorsCallback, &SwContext, &SwHandle);
    ASSERT_EFI_ERROR (Status);

#ifndef  GRANGEVILLE_FLAG
    // Register the WMAA SWSMI 0x97
    SwContext.SwSmiInputValue = EFI_WMAA_UR_SUPPORT_SWSMI;
    Status                    = SwDispatch->Register (SwDispatch, WmaaCallBack, &SwContext, &SwHandle);
    ASSERT_EFI_ERROR (Status);
    DEBUG ((DEBUG_INFO, "EFI_WMAA_UR_SUPPORT_SWSMI register successfully!\n"));
#endif

    //When SystemErrorEn is Auto, we return from here, so adding callback to publish HEST Entry
    //[BDX-WA for Ecrc issue]  HSD-500193
    if (((errorLogSetupData.SystemErrorEn == 2) || (errorLogSetupData.SystemErrorEn == 0)) && (errorLogSetupData.WheaSupport == 1)) {
      Status = gSmst->SmmRegisterProtocolNotify (
                            &gEfiSmmReadyToLockProtocolGuid,
                            WheaHESTCallBack,
                            &Registration
                            );
    }
//AptioV Server Override Start: Clear the Errors which are coming in Cold Boot.
	  GuidHob = GetFirstGuidHob (&gEfiMemoryMapGuid);
	  if ( GuidHob != NULL) {
	        systemMemoryMap = (struct SystemMemoryMapHob *) GET_GUID_HOB_DATA (GuidHob);
	  }
	  else {
	        systemMemoryMap = NULL;
	        Status = EFI_DEVICE_ERROR;
	        ASSERT_EFI_ERROR (Status);
	  }

	  if ((systemMemoryMap != NULL)  && (systemMemoryMap->IsColdBoot)) {
	  	ClearColdBootErrors ();
	  }
//AptioV Server Override End: Clear the Errors which are coming in Cold Boot.

      UpdateSmmRsPointer(); //AptioV Server Override: EFI SMM Variable Protocol is not installed. Using AMI NVRAM Runtime Get/Variable functions    

    //  if the request is to skip the ErrorHandler driver, then skip this driver
/* Aptio V Server Override : Decided to comment	  Auto Option so  that ClearSystemErrors() will be called always.
    if (errorLogSetupData.SystemErrorEn == 2) {
        return EFI_SUCCESS;
    }
*/
//AptioV Server Override Start: Changes done to Support IOHCore/QPI/VTD Errors
	//
	//  Handle the errors occured in the last boot, some times systems will
	//  get reset on fatal errors before the error handling code gets control.
	//  Note: MSR are getting cleared in Cpu Pei module, need to change the CPU module
	//  if errors are logged to MSRs
	//
#if HandleLastbootError_Support    
    if ( ( systemMemoryMap != NULL )  && ( systemMemoryMap->IsColdBoot != TRUE ) ) {
	    LastBootErrorHandler(mSmst);
    }
#endif
    // Unmask PCIE errors in cold boot
    if ( (systemMemoryMap != NULL) && ((systemMemoryMap->IsColdBoot) && (mErrLogSetupData->IohErrorEn == 1 || mErrLogSetupData->SystemErrorEn == 1)))  // AptioV server override - Cpp check
    {
	    UnMaskIIOErrorReporting();
    }
//AptioV Server Override End: Changes done to Support IOHCore/QPI/VTD Errors

    //
    // Collects the previous boot error record and log to whea bert dirver if whea support enbaled.
    // TBD - Based on QPI MRC provided errors, Wheat plaftform driver not integrated in ERDKII
    //
    ClearSystemErrors(0);

	//  If LogErrors is 00 -> disble error enabling, 01 -> Error Enabling; 02 -> skip error log driver ERROR enbaling or disabling code;
	//
	// Enables or Disables Errors
    //
	ProgramErrorSignalAndServerity(&errorLogSetupData);
    //
    // Init Uncorrected processor errors / Enable SMI routing
    //
    EnDisUncorrectableErrRep(&errorLogSetupData);
    //
	// Enable or disable Global SMI generation
    //
    EnableDisableGlobalSmiGeneration();
    //
    // Register SMI handler to handle Memory or Processor errors
    //
    RegisterErrorSMIHandler();
    //
    // Enable SMI Logic such that an SMI is generated upon any of the anticipated errors.
    //
    EnableAsyncSmiLogic (&errorLogSetupData);
	
    
    // 
    // Aptio V Server Override Start- Unlocking the MSR 0x790 for the ErrorInjection to work from RAS tools
    // 
    if (errorLogSetupData.UnlockMsr) {
	UnLockInjLogic();
    } 
// AptioV Server Override Start: Support added for Mode1 Memory Error Reporting    
#if MODE1_MEM_ERROR_REPORTING
    EnableMode1MemErrorReporting();
#endif
// AptioV Server Override End: Support added for Mode1 Memory Error Reporting
    //
    // Aptio V Server Override End- Unlocking the MSR 0x790 for the ErrorInjection to work from RAS tools
    //
    // clear any status that occured b/c of enabling GPI
    //
    ClearGpiSmiStatus();
	
  }
  return Status;
}

EFI_STATUS
SignalMceMmio (
  UINTN LogicalCpu
)
{
  //
  //write 0x1 at the offset pointed by Events control field
  //
  /* Events control field is a 2-byte field in at offset 0xff04 of SMRAM 
    if on chip save state is not enabled */
  UINTN  SMRAM_SMBASE;
  UINTN   EventsCtrl;
  UINT16  *EventsCtrlPtr;
  
  if(LogicalCpu >= mSmst->NumberOfCpus) {
    return EFI_INVALID_PARAMETER;
  }
  SMRAM_SMBASE = ((UINTN *)mSmst->CpuSaveState)[LogicalCpu] - SIZE_64KB + ((UINTN *)mSmst->CpuSaveStateSize)[LogicalCpu];
  EventsCtrl = (UINTN)SMRAM_SMBASE + EVENTS_CTRL_OFFSET;
  EventsCtrlPtr = (UINT16 *) EventsCtrl; 
  *EventsCtrlPtr = 0x0001;  

  return EFI_SUCCESS;
}

VOID
SignalMceMsr (
  MCEMSR_ARGS_STRUCT *MceArgs
)
{
  /* Events control is 2-byte field in MSR 0xC1F (b[47:32])
      if on-chip state save is enabled*/
  UINT64  EventsCtrlMsr;
  UINT64  EventsCtrlMsrVal;
  UINTN   CpuIndex;

  if(MceArgs == NULL) {
    return;
  }
  
  EventsCtrlMsr = AsmReadMsr64(EFI_MSR_HASWELL_EVENT_CTL_HLT_IO);
  EventsCtrlMsrVal = EventsCtrlMsr | 0x100000000;
  AsmWriteMsr64(EFI_MSR_HASWELL_EVENT_CTL_HLT_IO,EventsCtrlMsrVal);
  if(MceArgs->Sync == TRUE) {
    //MpSyncReleaseAp
  	CpuIndex = MceArgs->LogicalCpu;
  	ReleaseSpinLock(&mSmmErrorMpSyncData.CpuData[CpuIndex].ApBusy);
  }
}

/*
return value: 0 MMIO
                  1 MSR
*/
UINT32 
GetSaveStateType(
VOID
)
{
  UINTN skt;
  SMM_FEATURE_CONTROL_UBOX_DFX_STRUCT SmmCsrVal;
  UINT32 retval;
  retval = 0;

  for(skt=0;skt < MAX_SOCKET; skt++) {
    if ((mELogHost.socketPresentBitMap & (1 << skt)) == 0) {
      continue;
    }
    SmmCsrVal.Data = mCpuCsrAccess->ReadCpuCsr(skt,0,SMM_FEATURE_CONTROL_UBOX_DFX_REG); 
    if (SmmCsrVal.Bits.smm_cpu_save_en == 0) {
      retval |= 0;   
    } else {
      retval |= 1;
    }
  }

  return retval;
}

EFI_STATUS
SignalMCEtoOS (
  UINT32 ApicId
)
{  
  UINTN              NumberOfCpus;
  UINTN              LogicalCpu;
  MCEMSR_ARGS_STRUCT MceArgs;
  UINT32             SaveStateType;
  EFI_STATUS         Status;
  
  NumberOfCpus = mSmst->NumberOfCpus;
  SaveStateType = GetSaveStateType();
  
  for(LogicalCpu=0; LogicalCpu < NumberOfCpus; LogicalCpu++) {
    if (SaveStateType == 0) {
      //MMIO
      SignalMceMmio(LogicalCpu);
    } else {
      //MSR
      MceArgs.LogicalCpu = LogicalCpu;
      MceArgs.Sync = TRUE;
      if(mSmst->CurrentlyExecutingCpu != LogicalCpu) {
        /*SmmBlockingStartupThisAp(SignalMceMsr,
                                 LogicalCpu,
                                 &MceArgs
                                 );   */

				AcquireSpinLock(&mSmmErrorMpSyncData.CpuData[LogicalCpu].ApBusy);
				Status = mSmst->SmmStartupThisAp (SignalMceMsr,
																					LogicalCpu,
																					&MceArgs);

				if(Status == EFI_SUCCESS) {
					AcquireSpinLock(&mSmmErrorMpSyncData.CpuData[LogicalCpu].ApBusy);
				}

				ReleaseSpinLock(&mSmmErrorMpSyncData.CpuData[LogicalCpu].ApBusy);

      } else {
        SignalMceMsr(&MceArgs);               
      }
      }
  }
  return EFI_SUCCESS;
}

VOID
SignalCMCItoOS (
  UINT32 BankIndex, 
  UINT32 ApicId
)
{
  UINT16              CMCIScope;
  UINTN               CoreBits;
  UINTN               ThreadBits;
  UINT8               socket;
  UINT8               core;
  UINT8               thread;
  UINTN              cpuNumber;
  EFI_STATUS          status;
  CMCI_ARGS_STRUCT    CMCIVar;
  UINT32              num_thread;
  UINT32              num_core;
  EFI_CPU_PHYSICAL_LOCATION Location;
  
  GetCoreAndSMTBits(&ThreadBits,&CoreBits);
  socket  = (UINT8)(ApicId >> (CoreBits+ThreadBits));
  core    = (UINT8)((ApicId >> ThreadBits) & ~((~0) << CoreBits));
  thread  = (UINT8)(ApicId & ~((~0) << ThreadBits));
  
    InitializeSpinLock(&CMCIVar.ApBusy);
    
    CMCIScope = GetMcBankScope(BankIndex);
    switch(CMCIScope) {
    
      case MC_SCOPE_CORE:      
        /* Code to calculate the logical processor and assign it to cpuNumber*/
        num_thread = 1 << ThreadBits;
        for(thread=0; thread<num_thread;thread++){
            cpuNumber = GetCpuNumber(socket, core, thread);
            if (cpuNumber!= 0xffffffff){
              Location.Core = core;
              Location.Package = socket;
              Location.Thread = thread;
              CMCIVar.ApicId = GetApicIdByLoc(&Location);
              CMCIVar.BankIndex = BankIndex;

              if(cpuNumber == mSmst->CurrentlyExecutingCpu)
              {             
                  SignalCMCI (&CMCIVar);
                  /*no return here because it will be a for loop sometime in future*/        
              }
              /*not current CPU use smmstartup to call fn*/
              else {
                AcquireSpinLock(&CMCIVar.ApBusy);
                status = mSmst->SmmStartupThisAp (SignalCMCI,
                                          cpuNumber,
                                          &CMCIVar); 
                if(status == EFI_SUCCESS){
                    AcquireSpinLock(&CMCIVar.ApBusy);
                }
                ReleaseSpinLock(&CMCIVar.ApBusy);
              }
            }
            else {
              /*wrong CPU number we should not be here*/
              continue;
            }
        }
      break;

      case MC_SCOPE_PKG:          
          /* Code to calculate the logical processor and assign it to cpuNumber*/
          num_thread = 1 << ThreadBits;
          num_core = 1 << CoreBits;
          for(core=0; core< num_core;core++)
          {
            for(thread=0; thread<num_thread;thread++){
              cpuNumber = GetCpuNumber (socket, core, thread);
              if (cpuNumber!= 0xffffffff){
                Location.Core = core;
                Location.Package = socket;
                Location.Thread = thread;
                CMCIVar.ApicId = GetApicIdByLoc(&Location);
                CMCIVar.BankIndex = BankIndex;
                InitializeSpinLock(&CMCIVar.ApBusy);
                if(cpuNumber == mSmst->CurrentlyExecutingCpu)
                {             
                  SignalCMCI (&CMCIVar);
                  /*no return here because it will be a for loop sometime in future*/        
                }
              /*not current CPU use smmstartup to call fn*/
              else
              {
                AcquireSpinLock(&CMCIVar.ApBusy);
                status = mSmst->SmmStartupThisAp (SignalCMCI,
                                          cpuNumber,
                                          &CMCIVar); 
                if(status == EFI_SUCCESS){
                  AcquireSpinLock(&CMCIVar.ApBusy);
                }
                ReleaseSpinLock(&CMCIVar.ApBusy);                            
              }   
            }
            /*wrong CPU number we should not be here*/
            else
            {
                      continue;
            }
          }
        }
        break;
        default:
            return; 
    } 
    return;
}


VOID SignalCMCI(
  CMCI_ARGS_STRUCT *CMCIVarPtr
)
{
    UINT32                      BankIndex;
    UINT32                      ApicId;
    LOCAL_APIC_LVT_CMCI         CMCI_LVT;
    UINT32                      CMCIVec;
    UINT32                      msr_idx;
    IA32_MCI_CTL2_MSR_STRUCT    ctl2;
    
    BankIndex = CMCIVarPtr->BankIndex;
    ApicId = CMCIVarPtr->ApicId;

    /*Find if CMCI enabled*/
    msr_idx = GetAddrMcBankCtl2(BankIndex);
    ctl2.Data = AsmReadMsr64(msr_idx);
    if (ctl2.Bits.CMCI_EN)
    {
    CMCI_LVT.Uint32 = ReadLocalApicRegrep(CMCI_LVT_OFFSET);
    if(!CMCI_LVT.Bits.Mask){
        CMCIVec = CMCI_LVT.Bits.Vector;
        SendFixedIpi (ApicId,CMCIVec);
        }
    }
    ReleaseSpinLock(&CMCIVarPtr->ApBusy);
    return ;
}

VOID
ProcessUncorrectedError
(
VOID
) 
{
  UINT8 skt;
  
  for( skt = 0; skt < MAX_SOCKET; skt++) {
    //
    // Ignore disabled nodes
    //
    if ((mELogHost.socketPresentBitMap & (1 << skt)) == 0) {
      continue;
    }
    ProcessSocketUncorrectedError(skt); 
  }

  ProcessIIOUncorrectedError();
  ProcessPCHUncorrectedError();
  ProcessPlatformUncorrectedError();  
}


VOID
EmcaSmiSrc(
  VOID  *ErrorFound
)
{ 
  BOOLEAN *RetVal;
  UINT32 ApicId;
  EFI_CPU_PHYSICAL_LOCATION Loc;
  UINT8 skt;
  UINTN cpunumber;
  UINT64 UnSmiSrc = 0;

  RetVal = (BOOLEAN*)ErrorFound;
  *RetVal = FALSE;

  ApicId = GetApicID();
  ExtractProcessorLocation(ApicId,&Loc);
  skt = (UINT8)Loc.Package;
  cpunumber = GetCpuNumber(skt, (UINT8)Loc.Core, (UINT8)Loc.Thread); 
  UnSmiSrc =  ReadUncoreSmiSrc();
  if ((Loc.Core == 0 ) && (Loc.Thread == 0)) 
	  mELogHost.UncoreSmiErrSrc[skt] = UnSmiSrc;
  
  if(ReadCoreSmiSrc() != 0 || UnSmiSrc != 0) {
    *RetVal = TRUE;
  }
  
  ReleaseSpinLock(&mSmmErrorMpSyncData.CpuData[cpunumber].ApBusy);  
}


//if errors (ubox, imc, and iio) return true 
BOOLEAN
CheckForSystemError(
VOID
)
{  
  UINT8   localSocket  = 0;
  UINT8   localCore = 0;
  UINT8   socket = 0;
  UBOXERRSTS_UBOX_CFG_STRUCT uboxErrStsReg;
  SMICTRL_UBOX_MISC_HSX_BDX_STRUCT uboxSMICtrlReg;
  UINTN thread;
  EFI_STATUS Status;
  BOOLEAN ErrorFound;
  
  if(CheckForIIOErrors()) {
    return TRUE;
  }
  //
  // Check if smi enabled (same on every socket).
  //

  ExtractNodeCoreFromApicId (&localSocket, &localCore);

  uboxSMICtrlReg.Data = mMemRas->MemReadPciCfg(  localSocket, 0, SMICTRL_UBOX_MISC_REG);

  if ((uboxSMICtrlReg.Bits.smidis == 1) &&
      (uboxSMICtrlReg.Bits.umcsmien == 0)) {
    return FALSE;
  }

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

    if (uboxErrStsReg.Bits.smisrcumc == 1) return TRUE;
    if (uboxErrStsReg.Bits.smisrcimc == 1) return TRUE;
  }
  if((mErrLogSetupData->EMcaCSmiEn == 1 || mErrLogSetupData->EMcaLogEn == 1) && isEMCAGen2Cap()) {
    ErrorFound = FALSE;
    for(thread=0; thread < mSmst->NumberOfCpus; thread++) {

      if(thread == mSmst->CurrentlyExecutingCpu) {
        EmcaSmiSrc(&ErrorFound);
      } else {
        AcquireSpinLock(&mSmmErrorMpSyncData.CpuData[thread].ApBusy);
        Status = mSmst->SmmStartupThisAp (EmcaSmiSrc,
                                         thread,
                                         &ErrorFound);
        if(Status == EFI_SUCCESS) {
          AcquireSpinLock(&mSmmErrorMpSyncData.CpuData[thread].ApBusy);      
        }
        ReleaseSpinLock(&mSmmErrorMpSyncData.CpuData[thread].ApBusy);               
      }
      if(ErrorFound == TRUE) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

EFI_STATUS
ProcessFatalAndViralError (
  VOID
)
{
  UBOXERRSTS_UBOX_CFG_STRUCT uboxErrStsReg;
  MCA_ERR_SRC_LOG_PCU_FUN2_HSX_BDX_STRUCT     pcuMCAErrSrcLogReg;
  UINT8 Socket;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Skip sockets that are not present
    //
    if ((mELogHost.socketPresentBitMap & (BIT0 << Socket)) == 0) continue;

    uboxErrStsReg.Data = mMemRas->MemReadPciCfg( Socket, 0, UBOXERRSTS_UBOX_CFG_REG);
    if ((uboxErrStsReg.Bits.poisonrsvd == 1) ||
        (uboxErrStsReg.Bits.unsupportedopcode == 1)) {
      ASSERT(1);
    }

	  // check for fatal error condition in MCA_ERR_SRC 
	  if( mIioUds->SystemStatus.cpuType != CPU_IVT) {
	    pcuMCAErrSrcLogReg.Data = mMemRas->MemReadPciCfg(Socket, 0, MCA_ERR_SRC_LOG_PCU_FUN2_REG);
	    if (pcuMCAErrSrcLogReg.Bits.ierr_internal != 0){
		  DEBUG((DEBUG_ERROR,"ProcessFatalAndViralError: MCA_ERR_SRC: IERR Observed in %x\n", Socket));
		  ASSERT_EFI_ERROR(EFI_DEVICE_ERROR);
	    }
	  }
	  
	  // check for IERR in emca gen 2
	  if( mIioUds->SystemStatus.cpuType != CPU_IVT) {
	    pcuMCAErrSrcLogReg.Data = mMemRas->MemReadPciCfg(Socket, 0, MCA_ERR_SRC_LOG_PCU_FUN2_REG);
	    if (pcuMCAErrSrcLogReg.Bits.msmi_ierr_internal != 0) {
		  DEBUG((DEBUG_ERROR,"ProcessFatalAndViralError: MCA_ERR_SRC: IERR Observed in %x\n", Socket));
		  ASSERT_EFI_ERROR(EFI_DEVICE_ERROR);
	    }
	  }
  }
 return EFI_SUCCESS; 
}

EFI_STATUS 
EFIAPI
PropagateSignals(
VOID
)
{
  UINT32 cpunumber;
  UINT32 NumberOfCpus;
  BOOLEAN FatalErrorFound = FALSE;
  SMM_ERROR_CPU_DATA_BLOCK *CpuDataPtr;

  
  NumberOfCpus = mSmst->NumberOfCpus;
  for(cpunumber=0;cpunumber<NumberOfCpus;cpunumber++) {
    CpuDataPtr = &mSmmErrorMpSyncData.CpuData[cpunumber];
    if(CpuDataPtr->EventLog.ErrorsFound) {
      if((CpuDataPtr->EventLog.Events[McbankSrc].Severity == EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTABLE) || 
         (CpuDataPtr->EventLog.Events[McbankSrc].Severity == EFI_ACPI_5_0_ERROR_SEVERITY_FATAL)) {
        DEBUG ((DEBUG_INFO, "Calling the MCE Trigger from handler\n"));
        SignalMCEtoOS(CpuDataPtr->EventLog.Events[McbankSrc].ApicId);
        FatalErrorFound = TRUE;
      } else if(FatalErrorFound == FALSE) {
        DEBUG ((DEBUG_INFO, "Calling the CMCI Trigger from handler\n"));
        SignalCMCItoOS(CpuDataPtr->EventLog.Events[McbankSrc].BankIdx,CpuDataPtr->EventLog.Events[McbankSrc].ApicId);
      }
     CpuDataPtr->EventLog.Events[McbankSrc].Severity = EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTED;
     CpuDataPtr->EventLog.Events[McbankSrc].ErrorFound = FALSE;
    }
    CpuDataPtr->EventLog.ErrorsFound = FALSE;
  }

  return EFI_SUCCESS;
  
}

EFI_STATUS
ErrorHandlerMain 
(
  IN  EFI_HANDLE    DispatchHandle,
  IN CONST VOID     *Context         OPTIONAL,
  IN OUT VOID       *CommunicationBuffer,
  IN OUT UINTN      *SourceSize
) 
{
  mPcieCorErrDetected      = FALSE;

  if(!CheckForSystemError()) {
    return EFI_SUCCESS;
  }
  mSmmErrorMpSyncData.InEmcaHander = TRUE;
  DEBUG ((DEBUG_INFO, "SMM Error Handler Entry\n"));

  // TODO: Function names to be properly defined.
  mWheaLogStarted	= FALSE;
  ProcessFatalAndViralError();
  ProcessUncorrectedError();
  mSmmErrorMpSyncData.InEmcaHander = FALSE;
  ElogProcessorErrorHandler();//TODO: take out this, vmse logic should be under uncorrected and corrected respectively. 
                              //Pending error logic should be part of check system errors. 
  //ProcessCorrectedError()
  ClearSystemErrors(SKIP_MC_CE_CNT_FLAG);
  //
  // 4988861: Clone from BDX Sighting:EMCA Mode: System loops in BIOS after cpu
  //          went into unbreakable shutdown
  //  [cloned from Si s5287734]
  //
  ClearPCUErrorsEp();
 
  ErrorSeverityAction(); //AptioV Server Override: Changes done to use AMI RuntimeError (RtErrorLogBMC/Whea) Listener code.
  // Aptio V Server Override Start: PropagateSignals() needs to be called only for the EMCA Feature.
#ifndef GRANGEVILLE_FLAG                             
  if (mIioUds->SystemStatus.cpuType == CPU_BDX) {
    if(!mMemRas->SuppressCMCI()){
	  if( mErrLogSetupData->EMcaLogEn == 1)
	  {
      PropagateSignals();
	  }
	  }
  } else {
  if( mErrLogSetupData->EMcaLogEn == 1)
  {
    PropagateSignals();
	}
  }
// Aptio V Server Override End: PropagateSignals() needs to be called only for the EMCA Feature. (BSOD not happening after UCE with EMCALogEn Disabled
#endif
  return EFI_SUCCESS;
}

EFI_STATUS
RegisterErrorSMIHandler (
  VOID
  )
/*++

Routine Description:
    This function registers a SMI callback function for processor error checking and processing

Arguments:
    None

Returns:
    Nothing

--*/
{
  EFI_STATUS            Status;
  /*
#ifdef  CRB_FLAG  
  EFI_HANDLE                        SwHandle;
  EFI_SMM_SW_REGISTER_CONTEXT       SwContext;
  EFI_SMM_SW_DISPATCH2_PROTOCOL      *SwDispatch  = 0;
#endif
*/  
  //
  // TODO: Check correct error options
  // for example, system error option? 
  //

  Status  = EFI_SUCCESS;
  Status  = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, &mSmmBase2);
  ASSERT_EFI_ERROR (Status);

  if(mErrLogSetupData->SystemErrorEn == 1) {      

     Status = gSmst->SmiHandlerRegister (
                      ErrorHandlerMain,
                      NULL,
                      &mImageHandle
                      );
    ASSERT_EFI_ERROR (Status);

    if(isEMCAGen2Cap() == TRUE && mErrLogSetupData->EMcaLogEn == 1) {
      mSmmCpuService->RegisterExceptionHandler(mSmmCpuService, EXCEPT_IA32_MACHINE_CHECK, SmiMcaHandler);
    }

    
/* LER
#ifdef  CRB_FLAG
	Status = gSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, &SwDispatch);
    ASSERT_EFI_ERROR (Status);

	
    SwContext.SwSmiInputValue = EFI_LER_ENABLE_SWSMI;
    Status                    = SwDispatch->Register (SwDispatch, ReadCardData, &SwContext, &SwHandle);
    ASSERT_EFI_ERROR (Status);
#endif	
*/
/* AptioV Server : Changes done to resolve the assert issue when SystemError Setup option is enabled.Below Protocol is not installed anywhere.
    //
    // Locate SmmVariableProtocol.
    //
    Status = gSmst->SmmLocateProtocol (&gEfiSmmVariableProtocolGuid, NULL, (VOID**)&mSmmVariable);
    ASSERT_EFI_ERROR (Status);
*/

  }
  return Status;
}




