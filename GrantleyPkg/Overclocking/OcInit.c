/** @file
  OC Early Post initializations.

@copyright
  Copyright (c) 2011 - 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

**/
#include <Token.h> // AptioV server override
#include "OcInit.h"
#include <Ppi/ReadOnlyVariable2.h>
extern EFI_GUID gEfiOcDataGuid;
//AptioV server Override for XTU - START 
#if defined(PERF_TUNE_SUPPORT) && PERF_TUNE_SUPPORT == 1
#include "../AmiCRBPkg/XtuCrbTiming/Cpu/Pei/Library/CpuPerfTunePeiLib.h"
#endif
//AptioV server Override for XTU - END

VOID
PolicyInit(IN EFI_PEI_SERVICES         **PeiServices,
           volatile PEI_CPU_PLATFORM_POLICY_PPI *CpuPlatformPolicyPpi,
           SA_PLATFORM_POLICY_PPI      *SaPlatformPolicyPpi
           );

EFI_STATUS
EFIAPI
OcEntryPoint(
  IN        EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST  EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                  Status = EFI_SUCCESS;
#ifndef GRANGEVILLE_FLAG
  volatile PEI_CPU_PLATFORM_POLICY_PPI CpuPlatformPolicyPpi;
  SA_PLATFORM_POLICY_PPI      SaPlatformPolicyPpi;
  EFI_PLATFORM_INFO           *PlatformInfo;
  EFI_HOB_GUID_TYPE           *GuidHob;

  SYSTEM_CONFIGURATION                  SystemConfiguration;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI       *PeiVariable;
  UINTN                                 VariableSize;
  UINT8                           WdtTimeOut;
  WDT_PPI                         *gWdtPei;
  OC_CAPABILITIES_ITEM			OcCaps;
  UINT32                        CapStatus,UboxBase,ScratchData,RegEax,RegEbx,RegEcx,RegEdx;
  UINT32				CpuID= 0x00;
  UINT64                PciePll;
  UINT8					Ratio[]={100,125,166,250};
  OC_DATA_HOB			OcDataHob;
  UINT32				FwStatus3,FwDmiRatio = 0x00;
  //volatile UINT8		Indx=1;
  

  //DEBUG ((EFI_D_INFO, "(OC) PEIM Start.\n"));

  //
  // Update the PCIE base and 32/64bit PCI resource support
  //
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfo);

  //
  // Check for board support for HEDT Overclocking features
  //
  if (PlatformInfo->BoardId == TypeHedtSKU1CRB ||
      PlatformInfo->BoardId == TypeHedtEv ||
      PlatformInfo->BoardId == TypeHedtDDR3
      ) {

	//DEBUG ((EFI_D_INFO, "(OC) Board ID good for OC.\n"));
    
	PolicyInit((EFI_PEI_SERVICES**)PeiServices, &CpuPlatformPolicyPpi, &SaPlatformPolicyPpi);

	//DEBUG ((EFI_D_INFO, "(OC) Policy Init OK.\n"));

	//
	// Locate Variable PPI
	//
	Status = (**PeiServices).LocatePpi (PeiServices,
										&gEfiPeiReadOnlyVariable2PpiGuid,
										0,
										NULL,
										&PeiVariable
									   );  

	if (EFI_ERROR (Status))
	{
		  //DEBUG((EFI_D_ERROR, "LocatePpi Error for Variable PPI in OcInit.c !\n"));
	}
	else
	{
	  VariableSize = sizeof (SYSTEM_CONFIGURATION);
	  ZeroMem(&SystemConfiguration, VariableSize);

	  Status = PeiVariable->GetVariable (
							PeiVariable,
                    		L"IntelSetup", //AptioV Grangeville override
							&gEfiSetupVariableGuid,
							NULL,
							&VariableSize,
							&SystemConfiguration
							);

	  //
	  // Only execute OC functions if enabled in BIOS Setup
	  //
	  if (SystemConfiguration.OverclockingSupport) {

		///
		/// Get OC Capabilities of the domain
		///
		ZeroMem(&OcCaps,sizeof(OcCaps));
		OcCaps.DomainId = OC_LIB_DOMAIN_ID_IA_CORE;
		Status = GetOcCapabilities(&OcCaps,&CapStatus);
	    if(!OcCaps.RatioOcSupported){
		DEBUG ((EFI_D_INFO, "(OC) OCRatio not supported  = %X\n",OcCaps.RatioOcSupported));
        return Status;
		}
        // Read the current DMI/PEG Ratio
        PciePll =  AsmReadMsr64 (MSR_PCIE_PLLGEN3);
        OcDataHob.DmiPeg = Ratio[(PciePll & 0x03)];

        // Send the DMI/PEG Ratio to the QPI thru scratchpad
		FwStatus3 = PciRead32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, FWSTATUS3_OFFSET));
		FwDmiRatio = (UINT8)(FwStatus3 & FW_DMI_RATIO_MASK);

		UboxBase = (UINT32)AsmReadMsr64 (0x300);
		UboxBase = (UboxBase >> 8) & 0xFF;
		ScratchData = PciRead32 (PCI_LIB_ADDRESS (UboxBase, UBOXBUS , UBOXDEV , UBOXSCOFFSET ));
		ScratchData = ScratchData | (((UINT8)FwDmiRatio)<< 2);
		PciWrite32 (PCI_LIB_ADDRESS (UboxBase, UBOXBUS , UBOXDEV , UBOXSCOFFSET),ScratchData);
		DEBUG ((EFI_D_INFO, "(OC) QPI FwDmiRatio  = %X\n",FwDmiRatio));
		DEBUG ((EFI_D_INFO, "(OC) QPI ScratchPad2 = %X\n",ScratchData));
		
		//Get CPU Family ID 
		AsmCpuid(0x01,&RegEax,&RegEbx,&RegEcx,&RegEdx);
		CpuID = (UINT32)(RegEax >> 4);
		
		if(CpuID == CPU_FAMILY_HSX)
		{
			if(SystemConfiguration.SfrTrim == 0x01)
			{
				WriteVCUMailbox(OPEN_SEQ_HSX,PVDRATIO_HSX, 0x00);
				WriteVCUMailbox(ADJUST_PLL_HSX, 0x00, SFRTRIM_HSX);
				WriteVCUMailbox(CLOSE_SEQ_HSX, 0x00, 0x00);
			}
		}

		//build OC HOB
        // Save Instance into HOB
		//
		BuildGuidDataHob (
			&gEfiOcDataGuid,
			(VOID*) &OcDataHob,
			sizeof (OC_DATA_HOB)
			);


	    /// Disable Hooks
	    //DEBUG ((EFI_D_INFO, "(OC) About to run the PEI OC functions.\n"));
		
       if((UINT32)SystemConfiguration.PllTrim != 0)
		{
			if(CpuID == CPU_FAMILY_BDX)
			{
				if(SystemConfiguration.PllTrimPrefix != 0)
				SystemConfiguration.PllTrim = ~(SystemConfiguration.PllTrim)+ 1;
	            
				DEBUG ((EFI_D_ERROR, "(OC) PLLTrim Value:%x\n",SystemConfiguration.PllTrim));
				WriteVCUMailbox(OPEN_SEQ_BDX,PLL_SEQ_BDX, 0x00);
				WriteVCUMailbox(ADJUST_PLL_BDX, SystemConfiguration.PllTrim, PLLTRIM_INDX);
				WriteVCUMailbox(CLOSE_SEQ_BDX, 0x00, 0x00);
			}
		}
		

        if(SystemConfiguration.TjMaxoffset != 0x00)
		{

			WriteTjMax(SystemConfiguration.TjMaxoffset);
		
		}

		if(OcDataHob.DmiPeg == 166)
		{
			if(SystemConfiguration.AdjustPll == 1)
			{
				if(CpuID == CPU_FAMILY_BDX)
				{
					DEBUG ((EFI_D_ERROR, "(OC) BCLK 167 Flow BDX:%x\n",OcDataHob.DmiPeg));
					WriteVCUMailbox(OPEN_SEQ_BDX, PLL_SEQ_BDX, 0x00);
					WriteVCUMailbox(ADJUST_PLL_BDX, 0x00, 0x00);
					WriteVCUMailbox(CLOSE_SEQ_BDX, 0x00, 0x00);
				}
				if(CpuID == CPU_FAMILY_HSX)
				{
					DEBUG ((EFI_D_ERROR, "(OC) BCLK 167 Flow HSX:%x\n",OcDataHob.DmiPeg));
					WriteVCUMailbox(OPEN_SEQ_HSX, PVDRATIO_HSX, 0x00);
					WriteVCUMailbox(ADJUST_PLL_HSX, 0x00, 0x00);
					WriteVCUMailbox(CLOSE_SEQ_HSX, 0x00, 0x00);
					RingMinRatio((UINT8)SystemConfiguration.RingMinRatio);
					WriteVCUMailbox(OPEN_SEQ_HSX, PVDRATIO_HSX, 0x00);
					WriteVCUMailbox(ADJUST_PLL_HSX,PLLTRIM_HSX, 0x00);
					WriteVCUMailbox(CLOSE_SEQ_HSX, 0x00, 0x00);

				}
			}
		}

		if(OcDataHob.DmiPeg == 125)
		{
			if(SystemConfiguration.AdjustPll == 1)
			{
				if(CpuID == CPU_FAMILY_BDX)
				{
					DEBUG ((EFI_D_ERROR, "(OC) BCLK 125 Flow BDX:%x\n",OcDataHob.DmiPeg));
					WriteVCUMailbox(OPEN_SEQ_BDX, PLL_SEQ_BDX, 0x00);
					WriteVCUMailbox(ADJUST_PLL_BDX, 0x00, 0x00);
					WriteVCUMailbox(CLOSE_SEQ_BDX, 0x00, 0x00);
				}
				if(CpuID == CPU_FAMILY_HSX)
				{
					DEBUG ((EFI_D_ERROR, "(OC) BCLK 125 Flow HSX:%x\n",OcDataHob.DmiPeg));
					WriteVCUMailbox(OPEN_SEQ_HSX, PVDRATIO_HSX, 0x00);
					WriteVCUMailbox(ADJUST_PLL_HSX, 0x00, 0x00);
					WriteVCUMailbox(CLOSE_SEQ_HSX, 0x00, 0x00);
					RingMinRatio((UINT8)SystemConfiguration.RingMinRatio);
				}
			}
		}
        
		if(OcDataHob.DmiPeg == 100)
		{
			DEBUG ((EFI_D_ERROR, "(OC) BCLK 100 Flow :%x\n",OcDataHob.DmiPeg));
			if((UINT8)SystemConfiguration.RingMinRatio > 12 )
			RingMinRatio((UINT8)SystemConfiguration.RingMinRatio);
			
		}
        
		
		VccioOverride(PeiServices, &SystemConfiguration);

		//DEBUG ((EFI_D_INFO, "(OC) Vccio Override completed.\n"));

		VsmOverride(PeiServices, &SystemConfiguration);

		//DEBUG ((EFI_D_INFO, "(OC) Vsm Override completed.\n"));

		CpuVccInOverride(PeiServices, &SystemConfiguration);

		//DEBUG ((EFI_D_INFO, "(OC) CpuVccIn Override completed.\n"));

		CpuOcInit(PeiServices, &CpuPlatformPolicyPpi);

		//DEBUG ((EFI_D_INFO, "(OC) CPU Oc Init completed.\n"));

		SaOcInit (PeiServices, &SaPlatformPolicyPpi);

		//DEBUG ((EFI_D_INFO, "(OC) SA Oc Init completed.\n"));

		AVXOffset((UINT8)SystemConfiguration.AVX2Offset);
		


		//
		// Locate WDT_PPI (ICC WDT PPI)
		//
		Status = (**PeiServices).LocatePpi (PeiServices,
											&gWdtPpiGuid,
											0,
											NULL,
											&gWdtPei
										   );
		ASSERT_EFI_ERROR (Status);

		WdtTimeOut = gWdtPei->CheckStatus ();
		if (!WdtTimeOut) {
		  //DEBUG ((EFI_D_INFO, "(OC) Perform Bclk OC Flow.\n"));
		  Status = PerformBclkOcFlow(PeiServices, &SystemConfiguration);
		} else {
		  //DEBUG ((EFI_D_INFO, "(OC) Bypassing Bclk programming because of WDT expiration.\n"));
		}
	  } else {
		//DEBUG ((EFI_D_ERROR, "(OC) Overclocking Support not enabled in BIOS.\n"));
	  }
	}

  }
	//DEBUG ((EFI_D_INFO, "(OC) PEIM End.\n"));
#endif
  return Status;
}

EFI_STATUS
SaOcInit (
  IN EFI_PEI_SERVICES  CONST     **PeiServices,
  IN SA_PLATFORM_POLICY_PPI      *SaPlatformPolicyPpi
  )
/**
  Initializes Overclocking settings in the processor.

  @param[in] PeiServices - General purpose services available to every PEIM.
  @param[in] OverclockingtConfig      Pointer to Policy protocol instance

  @retval EFI_SUCCESS
**/
{
  EFI_STATUS    Status;
  OC_CAPABILITIES_ITEM OcCaps;
  VOLTAGE_FREQUENCY_ITEM CurrentVfItem;
  VOLTAGE_FREQUENCY_ITEM RequestedVfItem;
  UINT32      LibStatus;
  UINT8       DomainId;
  BOOLEAN     VfUpdateNeeded;
//  WDT_PPI     *gWdtPei;

  LibStatus = 0;
  VfUpdateNeeded = FALSE;

  if (SaPlatformPolicyPpi->OcConfig.OcSupport == 0){
    ///
    /// Overclocking is disabled
    ///
    DEBUG ((EFI_D_ERROR, "(OC) Overclocking is disabled. Bypassing SA overclocking flow.\n"));
    return EFI_SUCCESS;
  }

  Status = EFI_SUCCESS;
  ZeroMem(&CurrentVfItem,sizeof(CurrentVfItem));
  ZeroMem(&RequestedVfItem,sizeof(RequestedVfItem));

//   //
//   // Locate WDT_PPI (ICC WDT PPI)
//   //
//   Status = PeiServicesLocatePpi (
//              &gWdtPpiGuid,
//              0,
//              NULL,
//              (VOID **) &gWdtPei
//              );
//   ASSERT_EFI_ERROR (Status);

  ///
  /// We will loop on the CPU domains to manage the voltage/frequency settings
  ///
  for (DomainId = OC_LIB_DOMAIN_ID_GT; DomainId <= OC_LIB_DOMAIN_ID_IOD; DomainId++) {
    ///
    /// Only GT, Uncore, IOA, and IOD are valid for System Agent
    ///
    if ((DomainId == OC_LIB_DOMAIN_ID_GT) ||(DomainId == OC_LIB_DOMAIN_ID_UNCORE) ||
        (DomainId == OC_LIB_DOMAIN_ID_IOA) || (DomainId == OC_LIB_DOMAIN_ID_IOD)){
      ///
      /// Get OC Capabilities of the domain
      ///
      ZeroMem(&OcCaps,sizeof(OcCaps));
      OcCaps.DomainId = DomainId;
      Status = GetOcCapabilities(&OcCaps,&LibStatus);

      if (LibStatus == OC_LIB_COMPLETION_CODE_SUCCESS){
        ///
        /// If any OC is supported on this domain, then proceed
        ///
        if (OcCaps.RatioOcSupported || OcCaps.VoltageOverridesSupported || OcCaps.VoltageOffsetSupported){
          ///
          /// Need to populate the user requested settings from the platform policy
          /// to determine if OC changes are desired.
          ///
          ZeroMem(&CurrentVfItem,sizeof(CurrentVfItem));
          CurrentVfItem.DomainId = DomainId;

          ///
          /// Get a copy of the current domain VfSettings from the Mailbox Library
          ///
          Status = GetVoltageFrequencyItem(&CurrentVfItem,&LibStatus);
          if ((Status != EFI_SUCCESS) || (LibStatus != OC_LIB_COMPLETION_CODE_SUCCESS)){
            continue;
          }

          ///
          /// Populate the user requested VfSettings struct
          ///
          ZeroMem(&RequestedVfItem,sizeof(RequestedVfItem));
          RequestedVfItem.DomainId = DomainId;
          if (DomainId == OC_LIB_DOMAIN_ID_GT){
            RequestedVfItem.VfSettings.MaxOcRatio = (UINT8) SaPlatformPolicyPpi->OcConfig.GtMaxOcTurboRatio;

            ///
            /// VoltageTarget has 2 uses and we need to update the target based
            /// on the voltagemode requested
            ///
            RequestedVfItem.VfSettings.VoltageTargetMode = SaPlatformPolicyPpi->OcConfig.GtVoltageMode;
            if (RequestedVfItem.VfSettings.VoltageTargetMode == OC_LIB_OFFSET_ADAPTIVE){
              RequestedVfItem.VfSettings.VoltageTarget = SaPlatformPolicyPpi->OcConfig.GtExtraTurboVoltage;
            }
            else {
              RequestedVfItem.VfSettings.VoltageTarget = SaPlatformPolicyPpi->OcConfig.GtVoltageOverride;
            }
            RequestedVfItem.VfSettings.VoltageOffset = SaPlatformPolicyPpi->OcConfig.GtVoltageOffset;

            VfUpdateNeeded = (BOOLEAN)CompareMem((VOID*)&RequestedVfItem,(VOID*)&CurrentVfItem,sizeof(VOLTAGE_FREQUENCY_ITEM));
          }
          else if ((DomainId == OC_LIB_DOMAIN_ID_UNCORE) || (DomainId == OC_LIB_DOMAIN_ID_IOA) || (DomainId == OC_LIB_DOMAIN_ID_IOD)){
            ///
            /// Uncore,IOA, and IOD domains only supports voltage offset, other settings are ignored
            ///
            switch (DomainId) {
              case OC_LIB_DOMAIN_ID_UNCORE:
                RequestedVfItem.VfSettings.VoltageOffset = SaPlatformPolicyPpi->OcConfig.SaVoltageOffset;
				if(SaPlatformPolicyPpi->OcConfig.UncoreVoltageOffsetPrefix == NEGATIVEOFFSET )
				{
					RequestedVfItem.VfSettings.VoltageOffset = ~(RequestedVfItem.VfSettings.VoltageOffset) + 1;
				}
              break;

              case OC_LIB_DOMAIN_ID_IOA:
                RequestedVfItem.VfSettings.VoltageOffset = SaPlatformPolicyPpi->OcConfig.IoaVoltageOffset;
              break;

              case OC_LIB_DOMAIN_ID_IOD:
                RequestedVfItem.VfSettings.VoltageOffset = SaPlatformPolicyPpi->OcConfig.IodVoltageOffset;
              break;
            }

            if (RequestedVfItem.VfSettings.VoltageOffset != CurrentVfItem.VfSettings.VoltageOffset)
              VfUpdateNeeded = TRUE;
          }

          if (VfUpdateNeeded){
            VfUpdateNeeded = FALSE;

            ///
            /// Arm watchdog timer for OC changes
            ///
//            Status = gWdtPei->ReloadAndStart (WDT_TIMEOUT_BETWEEN_PEI_DXE);

            ///
            /// Need to update the requested voltage/frequency values
            ///
            Status = SetVoltageFrequencyItem(RequestedVfItem,&LibStatus);
            if ((Status != EFI_SUCCESS) || (LibStatus != OC_LIB_COMPLETION_CODE_SUCCESS)){
              //DEBUG ((EFI_D_ERROR, "(OC) Set Voltage Frequency failed. EFI Status = %X, Library Status = %X\n", Status, LibStatus));
            }
          }
        }
        else {
          //DEBUG ((EFI_D_INFO, "(OC) No OC support for this Domain = %X\n", DomainId));
        }
      }
      else {
        //DEBUG ((EFI_D_ERROR, "(OC) GetOcCapabilities message failed. Library Status = %X, Domain = %X\n", LibStatus, DomainId));
      }
    }
  }

  return Status;
}


EFI_STATUS
CpuOcInit (
  EFI_PEI_SERVICES CONST      **PeiServices,
  volatile PEI_CPU_PLATFORM_POLICY_PPI *CpuPlatformPolicyPpi
  )
{
  EFI_STATUS             Status = EFI_SUCCESS;
  OC_CAPABILITIES_ITEM   OcCaps;
  VOLTAGE_FREQUENCY_ITEM CurrentVfItem;
  VOLTAGE_FREQUENCY_ITEM RequestedVfItem;
  GLOBAL_CONFIG_ITEM     CurrentFivrItem;
  GLOBAL_CONFIG_ITEM     RequestedFivrItem;
  SVID_CONFIG_ITEM       CurrentSvidItem;
  SVID_CONFIG_ITEM       RequestedSvidItem;
  UINT32                 LibStatus;
  UINT8                  DomainId;
  UINT8                  ResetRequired;
 
//  WDT_PPI                *gWdtPei;

  LibStatus = 0; //DEBUG
  ResetRequired = FALSE;
  if (CpuPlatformPolicyPpi->OverclockingConfig.OcSupport == 0) {
    ///
    /// Overclocking is disabled
    ///
    DEBUG ((EFI_D_ERROR, "(OC) Overclocking is disabled. Bypassing CPU core overclocking flow.\n"));
    return EFI_SUCCESS;
  }

  Status = EFI_SUCCESS;
  ZeroMem(&CurrentFivrItem,sizeof(CurrentFivrItem));
  ZeroMem(&RequestedFivrItem,sizeof(RequestedFivrItem));
  ZeroMem(&CurrentSvidItem,sizeof(CurrentSvidItem));
  ZeroMem(&RequestedSvidItem,sizeof(RequestedSvidItem));

//   //
//   // Locate WDT_PPI (ICC WDT PPI)
//   //
//   Status = PeiServicesLocatePpi (
//                   &gWdtPpiGuid,
//                   0,
//                   NULL,
//                   (VOID **) &gWdtPei
//                   );
//   ASSERT_EFI_ERROR (Status);

  ///
  /// We will loop on the CPU domains to manage the voltage/frequency settings
  ///
  for (DomainId = OC_LIB_DOMAIN_ID_IA_CORE; DomainId <= OC_LIB_DOMAIN_ID_VCCU; DomainId++) {
    ///
    /// Only IA_CORE and CLR are valid for CPU Core
    ///
    if ((DomainId == OC_LIB_DOMAIN_ID_IA_CORE) || (DomainId == OC_LIB_DOMAIN_ID_CLR)||(DomainId == OC_LIB_DOMAIN_ID_VCCU)) {

      ///
      /// Get OC Capabilities of the domain
      ///
      ZeroMem(&OcCaps,sizeof(OcCaps));
      OcCaps.DomainId = DomainId;
      Status = GetOcCapabilities(&OcCaps,&LibStatus);
      
	  // If Domain is VCCU then make this lets assume GetOcCap is success
	  if(DomainId == OC_LIB_DOMAIN_ID_VCCU)
	{
	  LibStatus = OC_LIB_COMPLETION_CODE_SUCCESS;
	  OcCaps.VoltageOffsetSupported = 1;
	}




      if (LibStatus == OC_LIB_COMPLETION_CODE_SUCCESS) {
        ///
        /// If any OC is supported on this domain, then proceed
        ///
        if (OcCaps.RatioOcSupported || OcCaps.VoltageOverridesSupported || OcCaps.VoltageOffsetSupported) {
          ///
          /// Need to populate the user requested settings from the platform policy
          /// to determine if OC changes are desired.
          ///
          ZeroMem(&CurrentVfItem,sizeof(CurrentVfItem));
          CurrentVfItem.DomainId = DomainId;

          ///
          /// Get a copy of the current domain VfSettings from the Mailbox Library
          ///
          Status = GetVoltageFrequencyItem(&CurrentVfItem,&LibStatus);
          if ((Status != EFI_SUCCESS) || (LibStatus != OC_LIB_COMPLETION_CODE_SUCCESS)) {
            continue;
          }

          ///
          /// Populate the user requested VfSettings struct
          ///
          ZeroMem(&RequestedVfItem,sizeof(RequestedVfItem));
          RequestedVfItem.DomainId = DomainId;
          if (DomainId == OC_LIB_DOMAIN_ID_IA_CORE) {
            RequestedVfItem.VfSettings.MaxOcRatio = (UINT8) CpuPlatformPolicyPpi->OverclockingConfig.CoreMaxOcTurboRatio;
          } else if (DomainId == OC_LIB_DOMAIN_ID_CLR) {
            RequestedVfItem.VfSettings.MaxOcRatio = (UINT8) CpuPlatformPolicyPpi->OverclockingConfig.ClrMaxOcTurboRatio;
          } else if (DomainId == OC_LIB_DOMAIN_ID_VCCU) {
            RequestedVfItem.VfSettings.MaxOcRatio = (UINT8) CpuPlatformPolicyPpi->OverclockingConfig.VCCUMaxOcRatio;
          }

          ///
          /// VoltageTarget has 2 uses and we need to update the target based
          /// on the voltagemode requested
          ///
          if (DomainId == OC_LIB_DOMAIN_ID_IA_CORE) {
            RequestedVfItem.VfSettings.VoltageTargetMode = CpuPlatformPolicyPpi->OverclockingConfig.CoreVoltageMode;
            if (RequestedVfItem.VfSettings.VoltageTargetMode == OC_LIB_OFFSET_ADAPTIVE) {
              RequestedVfItem.VfSettings.VoltageTarget = CpuPlatformPolicyPpi->OverclockingConfig.CoreExtraTurboVoltage;
            } else {
              RequestedVfItem.VfSettings.VoltageTarget = CpuPlatformPolicyPpi->OverclockingConfig.CoreVoltageOverride;
            }
            RequestedVfItem.VfSettings.VoltageOffset = CpuPlatformPolicyPpi->OverclockingConfig.CoreVoltageOffset;
			if(CpuPlatformPolicyPpi->OverclockingConfig.CoreVoltageOffsetPrefix == NEGATIVEOFFSET)
			{
				// 2's complement the Value of the offset
			    RequestedVfItem.VfSettings.VoltageOffset = ~(RequestedVfItem.VfSettings.VoltageOffset ) + 1 ;

			}
          } else if (DomainId == OC_LIB_DOMAIN_ID_CLR) {
            RequestedVfItem.VfSettings.VoltageTargetMode = CpuPlatformPolicyPpi->OverclockingConfig.ClrVoltageMode;
            if (RequestedVfItem.VfSettings.VoltageTargetMode == OC_LIB_OFFSET_ADAPTIVE) {
              RequestedVfItem.VfSettings.VoltageTarget = CpuPlatformPolicyPpi->OverclockingConfig.ClrExtraTurboVoltage;
            } else {
              RequestedVfItem.VfSettings.VoltageTarget = CpuPlatformPolicyPpi->OverclockingConfig.ClrVoltageOverride;
            }
			RequestedVfItem.VfSettings.VoltageOffset = CpuPlatformPolicyPpi->OverclockingConfig.ClrVoltageOffset;
			if(CpuPlatformPolicyPpi->OverclockingConfig.ClrVoltageOffsetPrefix == NEGATIVEOFFSET)
			{
				// 2's complement the Value of the offset
			    RequestedVfItem.VfSettings.VoltageOffset = ~(RequestedVfItem.VfSettings.VoltageOffset ) + 1 ;

			}
		    }else if (DomainId == OC_LIB_DOMAIN_ID_VCCU) {
            RequestedVfItem.VfSettings.VoltageTargetMode = CpuPlatformPolicyPpi->OverclockingConfig.VCCUVoltageMode;
            if (RequestedVfItem.VfSettings.VoltageTargetMode == OC_LIB_OFFSET_ADAPTIVE) {
              RequestedVfItem.VfSettings.VoltageTarget = CpuPlatformPolicyPpi->OverclockingConfig.VCCUExtraTurboVoltage;
            } else {
              RequestedVfItem.VfSettings.VoltageTarget = CpuPlatformPolicyPpi->OverclockingConfig.VCCUVoltageOverride;
            }
			RequestedVfItem.VfSettings.VoltageOffset = CpuPlatformPolicyPpi->OverclockingConfig.VCCUVoltageOffset;
			if(CpuPlatformPolicyPpi->OverclockingConfig.VCCUVoltageOffsetPrefix == NEGATIVEOFFSET)
			{
				// 2's complement the Value of the offset
			    RequestedVfItem.VfSettings.VoltageOffset = ~(RequestedVfItem.VfSettings.VoltageOffset ) + 1 ;

			}
		  }
                

          ///
          /// Compare current settings with user requested settings to see if changes are needed
          ///
          if (CompareMem((VOID *)&RequestedVfItem,(VOID *)&CurrentVfItem,sizeof(VOLTAGE_FREQUENCY_ITEM))) {
            ///
            /// Arm watchdog timer for OC changes
            ///
//            Status = gWdtPei->ReloadAndStart (WDT_TIMEOUT_BETWEEN_PEI_DXE);

            ///
            /// Need to update the requested voltage/frequency values
            ///
            //DEBUG ((EFI_D_INFO, "(OC) Set Voltage Frequency for Domain = %X\n", DomainId));
            //DEBUG ((EFI_D_INFO, "(OC) RequestedVfItem.VfSettings.MaxOcRatio     = %X\n", RequestedVfItem.VfSettings.MaxOcRatio));
            //DEBUG ((EFI_D_INFO, "(OC) RequestedVfItem.VfSettings.TargetMode     = %X\n", RequestedVfItem.VfSettings.VoltageTargetMode));
            //DEBUG ((EFI_D_INFO, "(OC) RequestedVfItem.VfSettings.VoltageTarget  = %X\n", RequestedVfItem.VfSettings.VoltageTarget));
            //DEBUG ((EFI_D_INFO, "(OC) RequestedVfItem.VfSettings.VoltageOffset  = %X\n", RequestedVfItem.VfSettings.VoltageOffset));
            //DEBUG ((EFI_D_INFO, "(OC) CurrentVfItem.VfSettings.MaxOcRatio       = %X\n", CurrentVfItem.VfSettings.MaxOcRatio));
            //DEBUG ((EFI_D_INFO, "(OC) CurrentVfItem.VfSettings.TargetMode       = %X\n", CurrentVfItem.VfSettings.VoltageTargetMode));
            //DEBUG ((EFI_D_INFO, "(OC) CurrentVfItem.VfSettings.VoltageTarget    = %X\n", CurrentVfItem.VfSettings.VoltageTarget));
            //DEBUG ((EFI_D_INFO, "(OC) CurrentVfItem.VfSettings.VoltageOffset    = %X\n", CurrentVfItem.VfSettings.VoltageOffset));
            Status = SetVoltageFrequencyItem(RequestedVfItem,&LibStatus);
            if ((Status != EFI_SUCCESS) || (LibStatus != OC_LIB_COMPLETION_CODE_SUCCESS)) {
              //DEBUG ((EFI_D_ERROR, "(OC) Set Voltage Frequency failed. EFI Status = %X, Library Status = %X\n", Status, LibStatus));
            }
          }
        }
      } else {
        //DEBUG ((EFI_D_ERROR, "(OC) GetOcCapabilities message failed. Library Status = %X, Domain = %X\n", LibStatus, DomainId));
      }
    }
  }

  ///
  /// Detect changes to global FIVR settings
  ///
  Status = GetFivrConfig(&CurrentFivrItem,&LibStatus);

  if (LibStatus == OC_LIB_COMPLETION_CODE_SUCCESS) {
    ///
    /// Populate the requested FIVR settings from platform policy. The platform policy defines
    /// these bits as 0-Disabled, 1-Enabled. The Mailbox uses the reverse encoding. Need to convert
    /// the platform policy data to match the mailbox input.
    ///
    RequestedFivrItem.DisableFivrFaults = (~CpuPlatformPolicyPpi->OverclockingConfig.FivrFaultsEnable) & BIT0_MASK;
    RequestedFivrItem.DisableFivrEfficiency = (~CpuPlatformPolicyPpi->OverclockingConfig.FivrEfficiencyEnable) & BIT0_MASK;

    ///
    /// Compare current FIVR settings with requested FIVR settings to see if changes are needed
    ///
    if (CompareMem((VOID *)&RequestedFivrItem,(VOID *)&CurrentFivrItem,sizeof(GLOBAL_CONFIG_ITEM))) {
      ///
      /// Arm watchdog timer for OC changes
      ///
//      Status = gWdtPei->ReloadAndStart (8);

      ///
      /// Need to update the requested FIVR values
      ///
      //DEBUG ((EFI_D_INFO, "(OC) Set FIVR Config for Domain = %X\n", DomainId));
      Status = SetFivrConfig(RequestedFivrItem, &LibStatus);
      if ((Status != EFI_SUCCESS) || (LibStatus != OC_LIB_COMPLETION_CODE_SUCCESS)) {
        //DEBUG ((EFI_D_ERROR, "(OC) Set FIVR Config failed. EFI Status = %X, Library Status = %X\n", Status, LibStatus));
      }
      else {
        ///
        /// If Re-enabling Fivr Faults, system needs to perform a cold reset for hardware to take effect
        ///
        if ((CurrentFivrItem.DisableFivrFaults == 1) && (RequestedFivrItem.DisableFivrFaults == 0)) {
          //DEBUG ((EFI_D_ERROR, "(OC) FIVR Faults enable detected. Cold Reset required.\n"));
          ResetRequired = TRUE;
        }
      }
    }
  }
  else {
    //DEBUG ((EFI_D_ERROR, "(OC) Get FIVR Config message failed. Library Status = %X\n", LibStatus));
  }

  ///
  /// Detect changes to SVID settings
  ///
  Status = GetSvidConfig(&CurrentSvidItem,&LibStatus);

  if (LibStatus == OC_LIB_COMPLETION_CODE_SUCCESS) {
    ///
    /// Populate the requested SVID settings from platform policy. SvidDisable uses a
    /// reverse encoding from the platform policy defintion and will need to be converted.
    ///
    RequestedSvidItem.VoltageTarget = CpuPlatformPolicyPpi->OverclockingConfig.SvidVoltageOverride;
    RequestedSvidItem.SvidDisable = ~(CpuPlatformPolicyPpi->OverclockingConfig.SvidEnable) & BIT0_MASK;

    ///
    /// Compare current SVID settings with requested SVID settings to see if changes are needed
    ///
    if (CompareMem((VOID *)&RequestedSvidItem,(VOID *)&CurrentSvidItem,sizeof(SVID_CONFIG_ITEM))) {
      ///
      /// Arm watchdog timer for OC changes
      ///
//      Status = gWdtPei->ReloadAndStart (8);

      ///
      /// Need to update the requested SVID values
      ///
      //DEBUG ((EFI_D_INFO, "(OC) Set SVID Config for Domain = %X\n", DomainId));
      Status = SetSvidConfig(RequestedSvidItem, &LibStatus);
	  if ((Status != EFI_SUCCESS) || (LibStatus != OC_LIB_COMPLETION_CODE_SUCCESS)) {
        //DEBUG ((EFI_D_ERROR, "(OC) Set SVID Config failed. EFI Status = %X, Library Status = %X\n", Status, LibStatus));
      }
      else {
        ///
        /// If Re-enabling SVID, system needs to perform a cold reset for hardware to take effect
        ///
        if ((CurrentSvidItem.SvidDisable == 1) && (RequestedSvidItem.SvidDisable == 0)) {
          //DEBUG ((EFI_D_ERROR, "(OC) SVID Enable detected. Cold Reset required.\n"));
          ResetRequired = TRUE;
        }
      }
    }
  } else {
    //DEBUG ((EFI_D_ERROR, "(OC) GetFivrConfig message failed. Library Status = %X\n", LibStatus));
  }

  ///
  /// Command was successful and SVID config has changed. CPU must perform a reset
  /// for SVID settings to take effect.
  ///
  if (ResetRequired) {
    //DEBUG ((EFI_D_ERROR, "(OC) Perform Cold Reset\n"));
    PerformWarmORColdReset (PeiServices, COLDRESET);
  }

  return Status;
}

VOID
PolicyInit(IN EFI_PEI_SERVICES         **PeiServices,
           volatile PEI_CPU_PLATFORM_POLICY_PPI *CpuPlatformPolicyPpi,
           SA_PLATFORM_POLICY_PPI      *SaPlatformPolicyPpi
           )
{
  EFI_STATUS                            Status;
  SYSTEM_CONFIGURATION                  SysConfig;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI       *PeiVariable;
  UINTN                                 VariableSize;
 

  VariableSize = sizeof (SYSTEM_CONFIGURATION);
  //
  // Locate Variable PPI
  //
  Status = (**PeiServices).LocatePpi (
                  PeiServices,
                  &gEfiPeiReadOnlyVariable2PpiGuid,
                  0,
                  NULL,
                  &PeiVariable
                  );
  if (EFI_ERROR (Status)) {
    //
    // Variable service was not read so assume default values for all settings
    //
    CpuPlatformPolicyPpi->OverclockingConfig.OcSupport = 0;
    SaPlatformPolicyPpi->OcConfig.OcSupport = 0;
    return;
  }
  Status = PeiVariable->GetVariable (
                          PeiVariable,
                          L"IntelSetup", //AptioV server override
                          &gEfiSetupVariableGuid,
                          NULL,
                          &VariableSize,
                          &SysConfig
                          );

  if (EFI_ERROR (Status)) {
    //
    // Setup Variable was not read so assume default values for all settings
    //
    CpuPlatformPolicyPpi->OverclockingConfig.OcSupport = 0;
    SaPlatformPolicyPpi->OcConfig.OcSupport = 0;
  }else{
    //
    // Apply setup values to the policy structures
    //
   
    CpuPlatformPolicyPpi->OverclockingConfig.OcSupport				= (UINT8)SysConfig.OverclockingSupport;
    CpuPlatformPolicyPpi->OverclockingConfig.CoreMaxOcTurboRatio   = (UINT16)SysConfig.CoreMaxOcRatio;
    CpuPlatformPolicyPpi->OverclockingConfig.ClrMaxOcTurboRatio    = (UINT16)SysConfig.ClrMaxOcRatio;
    CpuPlatformPolicyPpi->OverclockingConfig.CoreVoltageMode       = SysConfig.CoreVoltageMode;
    CpuPlatformPolicyPpi->OverclockingConfig.CoreExtraTurboVoltage = SysConfig.CoreExtraTurboVoltage;
    CpuPlatformPolicyPpi->OverclockingConfig.CoreVoltageOverride   = SysConfig.CoreVoltageOverride;
    CpuPlatformPolicyPpi->OverclockingConfig.CoreVoltageOffset     = SysConfig.CoreVoltageOffset;
    CpuPlatformPolicyPpi->OverclockingConfig.ClrVoltageMode        = SysConfig.ClrVoltageMode;
    CpuPlatformPolicyPpi->OverclockingConfig.ClrExtraTurboVoltage  = SysConfig.ClrExtraTurboVoltage;
    CpuPlatformPolicyPpi->OverclockingConfig.ClrVoltageOverride    = SysConfig.ClrVoltageOverride;
    CpuPlatformPolicyPpi->OverclockingConfig.ClrVoltageOffset      = SysConfig.ClrVoltageOffset;
    CpuPlatformPolicyPpi->OverclockingConfig.FivrFaultsEnable      = SysConfig.FivrFaultsEnable;
    CpuPlatformPolicyPpi->OverclockingConfig.FivrEfficiencyEnable  = SysConfig.FivrEfficiencyEnable;
    CpuPlatformPolicyPpi->OverclockingConfig.SvidVoltageOverride   = SysConfig.SvidVoltageOverride;
    CpuPlatformPolicyPpi->OverclockingConfig.SvidEnable            = SysConfig.SvidEnable;
	CpuPlatformPolicyPpi->OverclockingConfig.CoreVoltageOffsetPrefix = SysConfig.CoreVoltageOffsetPrefix;
    CpuPlatformPolicyPpi->OverclockingConfig.ClrVoltageOffsetPrefix  = SysConfig.ClrVoltageOffsetPrefix;
	CpuPlatformPolicyPpi->OverclockingConfig.VCCUMaxOcRatio       = SysConfig.VCCUMaxOcRatio;
	CpuPlatformPolicyPpi->OverclockingConfig.VCCUVoltageMode      = SysConfig.VCCUVoltageMode;
	CpuPlatformPolicyPpi->OverclockingConfig.VCCUVoltageOverride  = SysConfig.VCCUVoltageOverride;
	CpuPlatformPolicyPpi->OverclockingConfig.VCCUVoltageOffset    = SysConfig.VCCUVoltageOffset;
	CpuPlatformPolicyPpi->OverclockingConfig.VCCUVoltageOffsetPrefix = SysConfig.VCCUVoltageOffsetPrefix;
	CpuPlatformPolicyPpi->OverclockingConfig.VCCUExtraTurboVoltage = SysConfig.VCCUExtraTurboVoltage;


	

    SaPlatformPolicyPpi->OcConfig.OcSupport = SysConfig.OverclockingSupport;
    SaPlatformPolicyPpi->OcConfig.SaVoltageOffset = SysConfig.UncoreVoltageOffset;
    SaPlatformPolicyPpi->OcConfig.IoaVoltageOffset = SysConfig.IoaVoltageOffset;
    SaPlatformPolicyPpi->OcConfig.IodVoltageOffset = SysConfig.IodVoltageOffset;
    SaPlatformPolicyPpi->OcConfig.UncoreVoltageOffsetPrefix = SysConfig.UncoreVoltageOffsetPrefix;

	//DEBUG ((EFI_D_INFO, "(OC) TEST OcSupport_PPI= %X\n", CpuPlatformPolicyPpi->OverclockingConfig.OcSupport));
    //DEBUG ((EFI_D_INFO, "(OC) TEST OcSupport_SYS= %X\n", SysConfig.OverclockingSupport));
    //DEBUG ((EFI_D_INFO, "(OC) TEST CoreMaxOcTurboRatio_PPI= %X\n", CpuPlatformPolicyPpi->OverclockingConfig.CoreMaxOcTurboRatio));
    //DEBUG ((EFI_D_INFO, "(OC) TEST CoreMaxOcTurboRatio_SYS= %X\n", SysConfig.CoreMaxOcRatio));
	//DEBUG ((EFI_D_INFO, "(OC) TEST ClrMaxOcTurboRatio_PPI= %X\n", CpuPlatformPolicyPpi->OverclockingConfig.ClrMaxOcTurboRatio));
    //DEBUG ((EFI_D_INFO, "(OC) TEST ClrMaxOcTurboRatio_SYS= %X\n", SysConfig.ClrMaxOcRatio));
  }
}

