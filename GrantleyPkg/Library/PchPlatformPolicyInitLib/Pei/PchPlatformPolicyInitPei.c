/** @file
  This file is SampleCode for Intel PCH PEI Platform Policy initialzation.

@copyright
  Copyright (c) 2009 - 2015 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement

**/
#include "PchPlatformPolicyInitPei.h"
#include <Library\PchPlatformPolicyInitCommonLib.h>
#include <Library\PlatformHooksLib.h>

extern EFI_GUID gEfiPlatformInfoGuid;

//AptioV server override start - Using the existing PCH_USB_CONFIG in PCH_PLATFORM_POLICY_PPI for PCH_USB_POLICY_PPI instead declaring new 
#include <Include\Ppi\MemoryDiscovered.h>
#include <Library\BaseMemoryLib.h>
#include <Library\PciExpressLib.h>
EFI_STATUS
EFIAPI
UpdatedPchPlatformPolicyPpi (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR mNotifyList[] = {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiMemoryDiscoveredPpiGuid,
    UpdatedPchPlatformPolicyPpi
};


//<AMI_PHDR_START>
//---------------------------------------------------------------------------
//
// Procedure: UpdatedPchPlatformPolicyPpi
//
// Description:
//   After memory is discovered, Re Install the PCH_PLATFORM_POLICY_PPI with updated
// 	 Pointer values
//
// Input:
//	EFI_PEI_SERVICES	 **PeiServices	- Pointer to the PEI Core data Structure
//	EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor	- Pointer to the instance of
//												  the EFI_PEI_NOTIFY_DESCRIPTOR
//	VOID	 *Ppi	- Pointer to the instance of the interface
//
// Output:
//   EFI_STATUS - Return Status
//
//---------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS
EFIAPI
UpdatedPchPlatformPolicyPpi (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi )
{
	 EFI_STATUS                    Status;
	  EFI_PEI_PPI_DESCRIPTOR        *PchPlatformPolicyPpiDesc;
	  PCH_PLATFORM_POLICY_PPI       *PchPlatformPolicyPpi;
      	  PCH_GBE_CONFIG                *GbeConfig;
	  PCH_THERMAL_MANAGEMENT        *ThermalMgmt;
	  PCH_MEMORY_THROTTLING         *MemoryThrottling;
	  PCH_HPET_CONFIG               *HpetConfig;
	  PCH_SATA_CONTROL              *SataConfig;
	  PCH_SATA_TRACE_CONFIG         *SataTraceConfig;
	  PCH_PCIE_CONFIG               *PcieConfig;
	  PCH_IOAPIC_CONFIG             *IoApicConfig;
	  PCH_PLATFORM_DATA             *PlatformData;
	  PCH_USB_CONFIG                *UsbConfig;
	  PCH_SATA_CONTROL              *sSataConfig;
	  PCH_SATA_TRACE_CONFIG         *sSataTraceConfig;

	  PchPlatformPolicyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
	  ASSERT (PchPlatformPolicyPpiDesc != NULL);
	  if (PchPlatformPolicyPpiDesc == NULL) {
	    return EFI_OUT_OF_RESOURCES;
	  }

	  PchPlatformPolicyPpi = (PCH_PLATFORM_POLICY_PPI *) AllocateZeroPool (sizeof (PCH_PLATFORM_POLICY_PPI));
	  ASSERT (PchPlatformPolicyPpi != NULL);
	  if (PchPlatformPolicyPpi == NULL) {
	    return EFI_OUT_OF_RESOURCES;
	  }

	  GbeConfig = (PCH_GBE_CONFIG *) AllocateZeroPool (sizeof (PCH_GBE_CONFIG));
	  ASSERT (GbeConfig != NULL);
	  if (GbeConfig == NULL) {
	    return EFI_OUT_OF_RESOURCES;
	  }

	  ThermalMgmt = (PCH_THERMAL_MANAGEMENT *) AllocateZeroPool (sizeof (PCH_THERMAL_MANAGEMENT));
	  ASSERT (ThermalMgmt != NULL);
	  if (ThermalMgmt == NULL) {
	    return EFI_OUT_OF_RESOURCES;
	  }

	  MemoryThrottling = (PCH_MEMORY_THROTTLING *) AllocateZeroPool (sizeof (PCH_MEMORY_THROTTLING));
	  ASSERT (MemoryThrottling != NULL);
	  if (MemoryThrottling == NULL) {
	    return EFI_OUT_OF_RESOURCES;
	  }

	  HpetConfig = (PCH_HPET_CONFIG *) AllocateZeroPool (sizeof (PCH_HPET_CONFIG));
	  ASSERT (HpetConfig != NULL);
	  if (HpetConfig == NULL) {
	    return EFI_OUT_OF_RESOURCES;
	  }

	  SataConfig = (PCH_SATA_CONTROL *) AllocateZeroPool (sizeof (PCH_SATA_CONTROL));
	  ASSERT (SataConfig != NULL);
	  if (SataConfig == NULL) {
	    return EFI_OUT_OF_RESOURCES;
	  }

	  SataTraceConfig = (PCH_SATA_TRACE_CONFIG *) AllocateZeroPool (sizeof (PCH_SATA_TRACE_CONFIG));
	  ASSERT (SataTraceConfig != NULL);
	  if (SataTraceConfig == NULL) {
	    return EFI_OUT_OF_RESOURCES;
	  }

	  PcieConfig = (PCH_PCIE_CONFIG *) AllocateZeroPool (sizeof (PCH_PCIE_CONFIG));
	  ASSERT (PcieConfig != NULL);
	  if (PcieConfig == NULL) {
	    return EFI_OUT_OF_RESOURCES;
	  }

	  IoApicConfig = (PCH_IOAPIC_CONFIG *) AllocateZeroPool (sizeof (PCH_IOAPIC_CONFIG));
	  ASSERT (IoApicConfig != NULL);
	  if (IoApicConfig == NULL) {
	    return EFI_OUT_OF_RESOURCES;
	  }

	  PlatformData = (PCH_PLATFORM_DATA *) AllocateZeroPool (sizeof (PCH_PLATFORM_DATA));
	  ASSERT (PlatformData != NULL);
	  if (PlatformData == NULL) {
	    return EFI_OUT_OF_RESOURCES;
	  }

	  UsbConfig = (PCH_USB_CONFIG *) AllocateZeroPool (sizeof (PCH_USB_CONFIG));
	  ASSERT (UsbConfig != NULL);
	  if (UsbConfig == NULL) {
	    return EFI_OUT_OF_RESOURCES;
	  }
	  
	  sSataConfig = (PCH_SATA_CONTROL *) AllocateZeroPool (sizeof (PCH_SATA_CONTROL));
	  ASSERT (sSataConfig != NULL);
	  if (sSataConfig == NULL) {
	    return EFI_OUT_OF_RESOURCES;
	  }
	  
	  sSataTraceConfig = (PCH_SATA_TRACE_CONFIG *) AllocateZeroPool (sizeof (PCH_SATA_TRACE_CONFIG));
	  ASSERT (sSataTraceConfig != NULL);
	  if (sSataTraceConfig == NULL) {
	    return EFI_OUT_OF_RESOURCES;
	  }
	  
	                
	  
	  Status = (**PeiServices).LocatePpi (
	                            PeiServices,
	                            &gPchPlatformPolicyPpiGuid,
	                            0,
	                            &PchPlatformPolicyPpiDesc,
	                            (VOID **)&PchPlatformPolicyPpi
	                            );
	 
	 
	  CopyMem(GbeConfig,PchPlatformPolicyPpi->GbeConfig,sizeof(PCH_GBE_CONFIG));
	  
	  CopyMem(ThermalMgmt,PchPlatformPolicyPpi->ThermalMgmt,sizeof(PCH_THERMAL_MANAGEMENT));
	  
	  CopyMem(MemoryThrottling,PchPlatformPolicyPpi->ThermalMgmt->MemoryThrottling,sizeof(PCH_MEMORY_THROTTLING));
	 	  
	  CopyMem(HpetConfig,PchPlatformPolicyPpi->HpetConfig,sizeof(PCH_HPET_CONFIG));
	   
	  CopyMem(SataConfig,PchPlatformPolicyPpi->SataConfig,sizeof(PCH_SATA_CONTROL));
	 
	  CopyMem(SataTraceConfig,PchPlatformPolicyPpi->SataConfig->SataTraceConfig,sizeof(PCH_SATA_TRACE_CONFIG));
	
	  CopyMem(PcieConfig,PchPlatformPolicyPpi->PcieConfig,sizeof(PCH_PCIE_CONFIG));

	  CopyMem(IoApicConfig,PchPlatformPolicyPpi->IoApicConfig,sizeof(PCH_IOAPIC_CONFIG));
	
	  CopyMem(PlatformData,PchPlatformPolicyPpi->PlatformData,sizeof(PCH_PLATFORM_DATA));
	 
	  CopyMem(UsbConfig,PchPlatformPolicyPpi->UsbConfig,sizeof(PCH_USB_CONFIG));
#ifdef PCH_SERVER_BIOS_FLAG
	  CopyMem(sSataConfig,PchPlatformPolicyPpi->sSataConfig,sizeof(PCH_SATA_CONTROL));

	  CopyMem(sSataTraceConfig,PchPlatformPolicyPpi->sSataConfig->SataTraceConfig,sizeof(PCH_SATA_TRACE_CONFIG));
#endif
	   PchPlatformPolicyPpiDesc->Ppi   = PchPlatformPolicyPpi; // PchPlatformPolicyPpi
 
	                           PchPlatformPolicyPpi->GbeConfig               = GbeConfig;
		 		   PchPlatformPolicyPpi->ThermalMgmt             = ThermalMgmt;
		 		   ThermalMgmt->MemoryThrottling                                 = MemoryThrottling;
		 		   PchPlatformPolicyPpi->HpetConfig              = HpetConfig;
		 		   PchPlatformPolicyPpi->SataConfig              = SataConfig;
		 		   SataConfig->SataTraceConfig = SataTraceConfig;
		 		   PchPlatformPolicyPpi->PcieConfig              = PcieConfig;
		 		   PchPlatformPolicyPpi->IoApicConfig            = IoApicConfig;
		 		   PchPlatformPolicyPpi->PlatformData            = PlatformData;
		 		   PchPlatformPolicyPpi->UsbConfig               = UsbConfig;
#ifdef PCH_SERVER_BIOS_FLAG
		 		   PchPlatformPolicyPpi->sSataConfig             = sSataConfig; 
#endif
		 		  sSataConfig->SataTraceConfig = sSataTraceConfig;
		
	    //
	    // Re Install the   PCH_PLATFORM_POLICY_PPI with values assigned to new pointer
	    //
	    Status = (*PeiServices)->ReInstallPpi (
					PeiServices,
					PchPlatformPolicyPpiDesc,
					PchPlatformPolicyPpiDesc);
	  
    return Status;
}
// AptioV server override end - Using the existing PCH_USB_CONFIG in PCH_PLATFORM_POLICY_PPI for PCH_USB_POLICY_PPI instead declaring new


/**
  This PEIM performs PCH PEI Platform Policy initialzation.

  @param[in] FfsHeader            Pointer to Firmware File System file header.
  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
InitPchPolicyPei (  
  IN EFI_PEI_SERVICES          **PeiServices,
  IN SYSTEM_CONFIGURATION      *SystemConfiguration,
  IN UINT8                     PlatformType
  )
{
  EFI_STATUS                    Status;
  EFI_PEI_PPI_DESCRIPTOR        *PchPlatformPolicyPpiDesc;
  PCH_PLATFORM_POLICY_PPI       *PchPlatformPolicyPpi;
  PCH_GBE_CONFIG                *GbeConfig;
  PCH_THERMAL_MANAGEMENT        *ThermalMgmt;
  PCH_MEMORY_THROTTLING         *MemoryThrottling;
  THERMAL_THROTTLE_LEVELS_PEI   *ThermalThrottleLevelsPei;
  PCH_HPET_CONFIG               *HpetConfig;
  PCH_SATA_CONTROL              *SataConfig;
  PCH_SATA_TRACE_CONFIG         *SataTraceConfig;
  PCH_PCIE_CONFIG               *PcieConfig;
  PCH_IOAPIC_CONFIG             *IoApicConfig;
  PCH_PLATFORM_DATA             *PlatformData;
  PCH_USB_CONFIG                *UsbConfig;
#ifdef PCH_SERVER_BIOS_FLAG
  PCH_SATA_CONTROL              *sSataConfig;
  PCH_SATA_TRACE_CONFIG         *sSataTraceConfig;
#endif // PCH_SERVER_BIOS_FLAG
#ifdef USB_PRECONDITION_ENABLE_FLAG
  EFI_BOOT_MODE                 BootMode;
#endif  // USB_PRECONDITION_ENABLE_FLAG

  UINT8                         PortIndex;

  
  PchPlatformPolicyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  ASSERT (PchPlatformPolicyPpiDesc != NULL);
  if (PchPlatformPolicyPpiDesc == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  PchPlatformPolicyPpi = (PCH_PLATFORM_POLICY_PPI *) AllocateZeroPool (sizeof (PCH_PLATFORM_POLICY_PPI));
  ASSERT (PchPlatformPolicyPpi != NULL);
  if (PchPlatformPolicyPpi == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  GbeConfig = (PCH_GBE_CONFIG *) AllocateZeroPool (sizeof (PCH_GBE_CONFIG));
  ASSERT (GbeConfig != NULL);
  if (GbeConfig == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ThermalMgmt = (PCH_THERMAL_MANAGEMENT *) AllocateZeroPool (sizeof (PCH_THERMAL_MANAGEMENT));
  ASSERT (ThermalMgmt != NULL);
  if (ThermalMgmt == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  MemoryThrottling = (PCH_MEMORY_THROTTLING *) AllocateZeroPool (sizeof (PCH_MEMORY_THROTTLING));
  ASSERT (MemoryThrottling != NULL);
  if (MemoryThrottling == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

    ThermalThrottleLevelsPei = (THERMAL_THROTTLE_LEVELS_PEI *) AllocateZeroPool (sizeof (THERMAL_THROTTLE_LEVELS_PEI));
  ASSERT (ThermalThrottleLevelsPei != NULL);
  if (ThermalThrottleLevelsPei == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  HpetConfig = (PCH_HPET_CONFIG *) AllocateZeroPool (sizeof (PCH_HPET_CONFIG));
  ASSERT (HpetConfig != NULL);
  if (HpetConfig == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SataConfig = (PCH_SATA_CONTROL *) AllocateZeroPool (sizeof (PCH_SATA_CONTROL));
  ASSERT (SataConfig != NULL);
  if (SataConfig == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SataTraceConfig = (PCH_SATA_TRACE_CONFIG *) AllocateZeroPool (sizeof (PCH_SATA_TRACE_CONFIG));
  ASSERT (SataTraceConfig != NULL);
  if (SataTraceConfig == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  PcieConfig = (PCH_PCIE_CONFIG *) AllocateZeroPool (sizeof (PCH_PCIE_CONFIG));
  ASSERT (PcieConfig != NULL);
  if (PcieConfig == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  IoApicConfig = (PCH_IOAPIC_CONFIG *) AllocateZeroPool (sizeof (PCH_IOAPIC_CONFIG));
  ASSERT (IoApicConfig != NULL);
  if (IoApicConfig == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  PlatformData = (PCH_PLATFORM_DATA *) AllocateZeroPool (sizeof (PCH_PLATFORM_DATA));
  ASSERT (PlatformData != NULL);
  if (PlatformData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  UsbConfig = (PCH_USB_CONFIG *) AllocateZeroPool (sizeof (PCH_USB_CONFIG));
  ASSERT (UsbConfig != NULL);
  if (UsbConfig == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  
#ifdef PCH_SERVER_BIOS_FLAG
  sSataConfig = (PCH_SATA_CONTROL *) AllocateZeroPool (sizeof (PCH_SATA_CONTROL));
  ASSERT (sSataConfig != NULL);
  if (sSataConfig == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  
  sSataTraceConfig = (PCH_SATA_TRACE_CONFIG *) AllocateZeroPool (sizeof (PCH_SATA_TRACE_CONFIG));
  ASSERT (sSataTraceConfig != NULL);
  if (sSataTraceConfig == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  
#endif // PCH_SERVER_BIOS_FLAG
  
#ifndef PCH_SERVER_BIOS_FLAG
  PchPlatformPolicyPpi->Revision                = PCH_PLATFORM_POLICY_PPI_REVISION_4;
#else
  PchPlatformPolicyPpi->Revision                = PCH_PLATFORM_POLICY_PPI_REVISION_3;
#endif //PCH_SERVER_BIOS_FLAG
  PchPlatformPolicyPpi->BusNumber               = 0;
  PchPlatformPolicyPpi->Rcba                    = PCH_RCBA_ADDRESS;
  PchPlatformPolicyPpi->PmBase                  = PCH_ACPI_BASE_ADDRESS;
  PchPlatformPolicyPpi->GpioBase                = PCH_GPIO_BASE_ADDRESS;
  PchPlatformPolicyPpi->Port80Route             = 0;

  PchPlatformPolicyPpi->GbeConfig               = GbeConfig;
  PchPlatformPolicyPpi->ThermalMgmt             = ThermalMgmt;
  PchPlatformPolicyPpi->HpetConfig              = HpetConfig;
  PchPlatformPolicyPpi->SataConfig              = SataConfig;
  PchPlatformPolicyPpi->PcieConfig              = PcieConfig;
  PchPlatformPolicyPpi->IoApicConfig            = IoApicConfig;
  PchPlatformPolicyPpi->PlatformData            = PlatformData;
  PchPlatformPolicyPpi->UsbConfig               = UsbConfig;
#ifdef PCH_SERVER_BIOS_FLAG
  PchPlatformPolicyPpi->sSataConfig             = sSataConfig; 
#endif //PCH_SERVER_BIOS_FLAG

#ifndef PCH_SERVER_BIOS_FLAG
  //
  // Mphy Sata
  //
  PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataHsio_DT.Table = NULL;
  PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataHsio_DT.Size = 0;
  PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataSharedHsio_DT.Table = NULL;
  PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataSharedHsio_DT.Size = 0;
#endif //PCH_SERVER_BIOS_FLAG

  PchPlatformPolicyPpi->MphyHeciTrigger_GETHsioVER = MphyHeciTrigger_GETHsioVER;
  PchPlatformPolicyPpi->MphyHeciReceive_GETHsioVER = MphyHeciReceive_GETHsioVER;
  PchPlatformPolicyPpi->MphyHeciClose_GETHsioVER = MphyHeciClose_GETHsioVER;
///
///
  GbeConfig->EnableGbe      = 1;
///
///
  ThermalMgmt->MemoryThrottling                                 = MemoryThrottling;
  MemoryThrottling->Enable                                      = PCH_DEVICE_DISABLE;
  MemoryThrottling->TsGpioPinSetting[TsGpioC].PmsyncEnable      = PCH_DEVICE_ENABLE;
  MemoryThrottling->TsGpioPinSetting[TsGpioD].PmsyncEnable      = PCH_DEVICE_ENABLE;
  MemoryThrottling->TsGpioPinSetting[TsGpioC].C0TransmitEnable  = PCH_DEVICE_ENABLE;
  MemoryThrottling->TsGpioPinSetting[TsGpioD].C0TransmitEnable  = PCH_DEVICE_ENABLE;
  MemoryThrottling->TsGpioPinSetting[TsGpioC].PinSelection      = 1;
  MemoryThrottling->TsGpioPinSetting[TsGpioD].PinSelection      = 0;
  ThermalMgmt->ThermalThrottleLevelsPei                         = ThermalThrottleLevelsPei;
  ThermalThrottleLevelsPei->SuggestedSetting                    = PCH_DEVICE_ENABLE;
  ThermalThrottleLevelsPei->Ctt                                 = TL_CTT;             // R_PCH_TBARB_CTT

  HpetConfig->Enable = 1;
  HpetConfig->Base = PCH_HPET_BASE_ADDRESS;

  SataConfig->SataMode = PchSataModeAhci;
  SataConfig->SataTraceConfig = SataTraceConfig;
  SataTraceConfig->TestMode        = PCH_DEVICE_DISABLE;

  for( PortIndex = 0; PortIndex < 6; PortIndex++ ) {
    SataTraceConfig->PortRxEq[PortIndex].GenSpeed[0].Enable = PCH_DEVICE_DISABLE;
    SataTraceConfig->PortRxEq[PortIndex].GenSpeed[1].Enable = PCH_DEVICE_DISABLE;
    SataTraceConfig->PortRxEq[PortIndex].GenSpeed[2].Enable = PCH_DEVICE_DISABLE;
    SataTraceConfig->PortRxEq[PortIndex].GenSpeed[0].RxEq = 0x0;
    SataTraceConfig->PortRxEq[PortIndex].GenSpeed[1].RxEq = 0x0;
    SataTraceConfig->PortRxEq[PortIndex].GenSpeed[2].RxEq = 0x0;
  }

#ifdef PCH_SERVER_BIOS_FLAG  
  sSataConfig->SataMode = PchSataModeAhci;
  sSataConfig->SataTraceConfig = sSataTraceConfig;
  sSataTraceConfig->TestMode        = PCH_DEVICE_DISABLE;

  for( PortIndex = 0; PortIndex < 6; PortIndex++ ) {
    sSataTraceConfig->PortRxEq[PortIndex].GenSpeed[0].Enable = PCH_DEVICE_DISABLE;
    sSataTraceConfig->PortRxEq[PortIndex].GenSpeed[1].Enable = PCH_DEVICE_DISABLE;
    sSataTraceConfig->PortRxEq[PortIndex].GenSpeed[2].Enable = PCH_DEVICE_DISABLE;
    sSataTraceConfig->PortRxEq[PortIndex].GenSpeed[0].RxEq = 0x0;
    sSataTraceConfig->PortRxEq[PortIndex].GenSpeed[1].RxEq = 0x0;
    sSataTraceConfig->PortRxEq[PortIndex].GenSpeed[2].RxEq = 0x0;
  }  
#endif // PCH_SERVER_BIOS_FLAG

  PcieConfig->PcieSpeed[0] = PchPcieAuto;
  PcieConfig->PcieSpeed[1] = PchPcieAuto;
  PcieConfig->PcieSpeed[2] = PchPcieAuto;
  PcieConfig->PcieSpeed[3] = PchPcieAuto;
  PcieConfig->PcieSpeed[4] = PchPcieAuto;
  PcieConfig->PcieSpeed[5] = PchPcieAuto;
  PcieConfig->PcieSpeed[6] = PchPcieAuto;
  PcieConfig->PcieSpeed[7] = PchPcieAuto;

  IoApicConfig->IoApicId = 0x02;
  IoApicConfig->ApicRangeSelect = 0x00;
  IoApicConfig->IoApicEntry24_39 = PCH_DEVICE_DISABLE;
  
  PlatformData->EcPresent = 0;
  ///
  /// PlatformData->SmmBwp value directly depends on the value of CpuConfig->Pfat
  /// (found in CpuPolicyInitPei.c file)
  /// If CpuConfig->Pfat is set to 1 (enabled) then
  ///   PlatformData->SmmBwp has to be set to 1 (enabled)
  /// This is a PFAT Security requirement that needs to be addressed
  /// If CpuConfig->Pfat is set to 0 (disabled) then
  ///   PlatformData->SmmBwp value don't care, it can be set to either
  ///   1 (enabled) or 0 (disabled) based on customer implementation
  ///

      PlatformData->SmmBwp    = 1;
#ifdef SDP_FLAG
      PlatformData->SmmBwp    = 0;
#endif

  ///
  /// Temporary Memory Base Address for PCI devices to be used to initialize MMIO registers.
  /// Minimum size is 64KB bytes.
  ///
  PlatformData->TempMemBaseAddr   = PCH_TEMP_MEM_BASE_ADDRESS;

  ///
  /// Init USB related setting
  ///
  InitPchUsbConfig (UsbConfig);

  PchPlatformPolicyPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  PchPlatformPolicyPpiDesc->Guid  = &gPchPlatformPolicyPpiGuid;

  UpdatePeiPchPlatformPolicy (
     PeiServices, 
     PchPlatformPolicyPpi, 
     SystemConfiguration,
     PlatformType
     );

  PchPlatformPolicyPpiDesc->Ppi = PchPlatformPolicyPpi;
#ifdef USB_PRECONDITION_ENABLE_FLAG
  ///
  /// Update Precondition option for S4 resume. 
  /// Skip Precondition for S4 resume in case this boot may not connect BIOS USB driver.
  /// If BIOS USB driver will be connected always for S4, then disable below update.
  /// To keep consistency during boot, must enabled or disabled below function in both PEI and DXE
  /// PlatformPolicyInit driver.
  ///
  if (UsbConfig->UsbPrecondition == TRUE) {
    (*PeiServices)->GetBootMode (PeiServices, &BootMode);
    if (BootMode == BOOT_ON_S4_RESUME) {
      UsbConfig->UsbPrecondition = FALSE;
      DEBUG ((DEBUG_INFO, "BootMode is BOOT_ON_S4_RESUME, disable Precondition\n"));
    }
  }
#endif  // USB_PRECONDITION_ENABLE_FLAG

  ///
  /// Install PCH Platform Policy PPI
  ///
  Status = (**PeiServices).InstallPpi (PeiServices, PchPlatformPolicyPpiDesc);
  ASSERT_EFI_ERROR (Status);
  /// AptioV server override start - Using the exsisting PCH_USB_CONFIG in PCH_PLATFORM_POLICY_PPI for PCH_USB_POLICY_PPI instead decalring new - START///////
  Status = (*PeiServices)->NotifyPpi (PeiServices, &mNotifyList[0]);
  ASSERT_EFI_ERROR (Status);
  /// AptioV server override end - Using the exsisting PCH_USB_CONFIG in PCH_PLATFORM_POLICY_PPI for PCH_USB_POLICY_PPI instead decalring new - END///////
  return Status;
}


/**
  This function performs PCH PEI Platform Policy initialzation.

  @param[in] PeiServices             General purpose services available to every PEIM.
  @param[in] PchPlatformPolicyPpi    The PCH Platform Policy PPI instance

  @retval    EFI_SUCCESS             The PPI is installed and initialized.
  @retval    EFI ERRORS              The PPI is not successfully installed.
  @retval    EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver

**/
EFI_STATUS
EFIAPI
UpdatePeiPchPlatformPolicy (
  IN      EFI_PEI_SERVICES        **PeiServices,
  IN OUT  PCH_PLATFORM_POLICY_PPI *PchPlatformPolicyPpi,
  IN SYSTEM_CONFIGURATION         *SystemConfiguration,
  IN UINT8                        PlatformType
  )
{
  UINT32                        SpiHsfsReg;
  UINT32                        SpiFdodReg;
  UINT8                         Index;
  EFI_PLATFORM_INFO             *PlatformInfoHobData;
  EFI_HOB_GUID_TYPE             *GuidHob;
  
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT(GuidHob);
  PlatformInfoHobData = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfoHobData);

  PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[0].GenSpeed[0].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[0].GenSpeed[1].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[0].GenSpeed[2].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[1].GenSpeed[0].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[1].GenSpeed[1].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[1].GenSpeed[2].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[2].GenSpeed[0].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[2].GenSpeed[1].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[2].GenSpeed[2].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[3].GenSpeed[0].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[3].GenSpeed[1].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[3].GenSpeed[2].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[0].GenSpeed[2].RxEq   = 0x2;
  PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[1].GenSpeed[2].RxEq   = 0x2;
  PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[2].GenSpeed[2].RxEq   = 0x2;
  PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[3].GenSpeed[2].RxEq   = 0x2;
#ifdef PCH_SERVER_BIOS_FLAG
  PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->PortRxEq[0].GenSpeed[0].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->PortRxEq[0].GenSpeed[1].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->PortRxEq[0].GenSpeed[2].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->PortRxEq[1].GenSpeed[0].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->PortRxEq[1].GenSpeed[1].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->PortRxEq[1].GenSpeed[2].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->PortRxEq[2].GenSpeed[0].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->PortRxEq[2].GenSpeed[1].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->PortRxEq[2].GenSpeed[2].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->PortRxEq[3].GenSpeed[0].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->PortRxEq[3].GenSpeed[1].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->PortRxEq[3].GenSpeed[2].Enable = PCH_DEVICE_ENABLE;
  PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->PortRxEq[0].GenSpeed[2].RxEq   = 0x2;
  PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->PortRxEq[1].GenSpeed[2].RxEq   = 0x2;
  PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->PortRxEq[2].GenSpeed[2].RxEq   = 0x2;
  PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->PortRxEq[3].GenSpeed[2].RxEq   = 0x2;
#endif //PCH_SERVER_BIOS_FLAG
#ifdef TRAD_FLAG
  if (PlatformType <= TypePlatformMax /*== TypeTrad*/) {
    PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[4].GenSpeed[0].Enable = PCH_DEVICE_ENABLE;
    PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[4].GenSpeed[1].Enable = PCH_DEVICE_ENABLE;
    PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[4].GenSpeed[2].Enable = PCH_DEVICE_ENABLE;
    PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[5].GenSpeed[0].Enable = PCH_DEVICE_ENABLE;
    PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[5].GenSpeed[1].Enable = PCH_DEVICE_ENABLE;
    PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[5].GenSpeed[2].Enable = PCH_DEVICE_ENABLE;
    PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[4].GenSpeed[2].RxEq   = 0x2;
    PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[5].GenSpeed[2].RxEq   = 0x2;
  }
#endif // TRAD_FLAG


  SpiHsfsReg = PchMmRcrb32 (R_PCH_SPI_HSFS);
  if ((SpiHsfsReg & B_PCH_SPI_HSFS_FDV) == B_PCH_SPI_HSFS_FDV) {
      MmioWrite32 (PCH_RCRB_BASE + R_PCH_SPI_FDOC, V_PCH_SPI_FDOC_FDSS_FSDM);
      SpiFdodReg = PchMmRcrb32 (R_PCH_SPI_FDOD);
      if (SpiFdodReg == V_PCH_SPI_FDBAR_FLVALSIG) {
        if (MmioRead32 (PCH_RCRB_BASE + R_PCH_SPI_FREG3_GBE) == 0x00007FFF) {
          //
          // GbE Region does not exist, so set EnableGbe policy to disabled.
          //
          PchPlatformPolicyPpi->GbeConfig->EnableGbe = PCH_DEVICE_DISABLE;
        } else {
           if(PlatformInfoHobData->PchData.PchLanSupported){
             PchPlatformPolicyPpi->GbeConfig->EnableGbe = SystemConfiguration->PchLan;
           } else {
               PchPlatformPolicyPpi->GbeConfig->EnableGbe = PCH_DEVICE_DISABLE;
           }
        }
      }
  }
  PchPlatformPolicyPpi->Port80Route           = SystemConfiguration->IchPort80Route;

#ifndef PCH_SERVER_BIOS_FLAG
#ifdef GRANGEVILLE_FLAG
  //
  // Mphy Sata
  //
  switch (PlatformInfoHobData->BoardId){

    case TypeBeverlyCoveCRB:
    case TypeBeverlyCoveSODimmCRB:  
      PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataHsio_DT.Size = (sizeof (PchSataHsioLptH_DT_C0_BeverlyCove) / sizeof (IOBP_MMIO_TABLE_STRUCT));
      PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataHsio_DT.Table = PchSataHsioLptH_DT_C0_BeverlyCove;
      PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataSharedHsio_DT.Size = (sizeof (PchSataSharedHsioLptH_DT_C0_BeverlyCove) / sizeof (IOBP_MMIO_TABLE_STRUCT));
      PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataSharedHsio_DT.Table = PchSataSharedHsioLptH_DT_C0_BeverlyCove;  
      break;

    default:
      PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataHsio_DT.Size = (sizeof (PchSataHsioLptH_DT_C0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
      PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataHsio_DT.Table = PchSataHsioLptH_DT_C0;
      PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataSharedHsio_DT.Size = (sizeof (PchSataSharedHsioLptH_DT_C0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
      PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataSharedHsio_DT.Table = PchSataSharedHsioLptH_DT_C0;
      break;
  }
#endif
#endif //PCH_SERVER_BIOS_FLAG



  PchPlatformPolicyPpi->SataConfig->SataMode = SystemConfiguration->SataInterfaceMode;
#ifdef PCH_SERVER_BIOS_FLAG
  PchPlatformPolicyPpi->sSataConfig->SataMode = SystemConfiguration->sSataInterfaceMode;
#endif // PCH_SERVER_BIOS_FLAG
  	
  PchPlatformPolicyPpi->SataConfig->SataTraceConfig->TestMode = SystemConfiguration->SataTestMode;
#ifdef PCH_SERVER_BIOS_FLAG
  PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->TestMode = SystemConfiguration->sSataTestMode;
#endif // PCH_SERVER_BIOS_FLAG

  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
     PchPlatformPolicyPpi->PcieConfig->PcieSpeed[Index] = SystemConfiguration->PcieRootPortSpeed[Index];
  }

  ///
  /// PlatformData->SmmBwp value directly depends on the value of CpuConfig->Pfat
  /// (found in CpuPolicyInitPei.c file)
  /// If CpuConfig->Pfat is set to 1 (enabled) then
  /// PlatformData->SmmBwp has to be set to 1 (enabled)
  /// This is a PFAT Security requirement that needs to be addressed
  /// If CpuConfig->Pfat is set to 0 (disabled) then
  /// PlatformData->SmmBwp value don't care, it can be set to either
  /// 1 (enabled) or 0 (disabled) based on customer implementation
  /// 
  if ( SystemConfiguration->PfatEnabled == 1){
    PchPlatformPolicyPpi->PlatformData->SmmBwp = 1; 
  }

///
///
    //
    // Update Pch Usb Config
    //
  UpdatePchUsbConfig (
      PchPlatformPolicyPpi->UsbConfig,
      SystemConfiguration,
      PlatformType,
      PlatformInfoHobData
      );
	  
  if ( SystemConfiguration->PchSerm == 1 ){
     MmioOr32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_GCS, (UINT32)B_PCH_RCRB_GCS_SERM);
  }
  else{
     MmioAnd32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_GCS, (UINT32)~B_PCH_RCRB_GCS_SERM);
  }
///
///

#ifdef PCH_SERVER_BIOS_FLAG
  PchPlatformPolicyPpi->XTpmLen = SystemConfiguration->XTpmLen;
#endif //PCH_SERVER_BIOS_FLAG

///
///
  return EFI_SUCCESS;
}


/**
  This function triggers GETHsioVER command to ME.

  @param[in] This    The PCH Platform Policy PPI instance
  @param[out] Response    Response time
  
  @retval    EFI_SUCCESS             The command was issued successfuly to ME.
  @retval    EFI_TIMEOUT              Timeout occured, no ack from ME.

**/
EFI_STATUS
EFIAPI
MphyHeciTrigger_GETHsioVER (
  IN      PCH_PLATFORM_POLICY_PPI *This,
  OUT     UINT32                  *Response  
  )
{
  HECI_MEFS1 Mefs1;
  UINT32     Timeout = MAX_ME_READY_TIMEOUT / STALL_100_MICROSECONDS;

  ASSERT(This != NULL);
  ASSERT(Response != NULL);

  if(IsSoftSdvPlatform())
  {
    Timeout = 1;
    DEBUG ((EFI_D_INFO, "[MPHY] Simics detected, setting short timeout\n"));
  }
  //
  // Write Get MPHY (HSIO) Version request command to H_GS register in HECI-1
  // and wait up to 2 seconds for ME response in MEFS1.MsgAck bits. ME is
  // initializing. The timeout for ME initialization is 2 seconds. Within this
  // timeout ME should respond for Get MPHY (HSIO) Version request.
  //
  DEBUG((EFI_D_INFO, "[MPHY] Writing Get MPHY Version command in HECI-1\n"));
  HeciPciAndThenOr32(R_ME_H_GS, 0, (H2M_HSIO_MESSAGE | H2M_HSIO_CMD_GETHSIOVER));
  while (1)
  {
    Mefs1.DWord = HeciPciReadMefs1();
    if (Mefs1.Bits.ErrorCode != MEFS1_ERRCODE_NOERROR)
    {
      DEBUG((EFI_D_ERROR, "[MPHY] ERROR: Error signaled in ME status register (MEFS1: %08X)\n", Mefs1.DWord));
      return EFI_DEVICE_ERROR;
    }
    if (Mefs1.Bits.MsgAck == M2H_HSIO_MSG_ACK)
    {
      DEBUG((EFI_D_INFO, "[MPHY] Got ACK for Get MPHY Version (MEFS1: %08X)\n", Mefs1.DWord));
      break;
    }
    if (Timeout-- == 0)
    {
      DEBUG((EFI_D_ERROR, "[MPHY] ERROR: Timeout when waiting for Get MPHY Version ACK (MEFS1: %08X)\n", Mefs1.DWord));
      return EFI_TIMEOUT;
    }
    PchPmTimerStall(STALL_100_MICROSECONDS);
  }
  
  *Response = MAX_ME_READY_TIMEOUT - Timeout * STALL_100_MICROSECONDS;  
  
  return EFI_SUCCESS;
}


/**
  This function receives the response for GETHsioVER command .

  @param[in] This      The PCH Platform Policy PPI instance
  @param[out] Msg    ME response to GETHsioVER.
  
  @retval    EFI_SUCCESS             The command was issued successfuly to ME.

**/
EFI_STATUS
EFIAPI
MphyHeciReceive_GETHsioVER(
  IN      PCH_PLATFORM_POLICY_PPI *This,
  OUT     UINT32                  *Msg
  )
{
  ASSERT(This != NULL);
  ASSERT(Msg != NULL);
  
  //
  // Receive the Hsio Version reported by ME FW.
  //
  *Msg = HeciPciRead32(R_ME_HFS_5);
  return EFI_SUCCESS;
}

/**
  This function closes the GETHsioVER command.

  @param[in] This    The PCH Platform Policy PPI instance
  
  @retval    EFI_SUCCESS             The command was issued successfuly to ME.

**/

EFI_STATUS
EFIAPI
MphyHeciClose_GETHsioVER(
  IN      PCH_PLATFORM_POLICY_PPI *This 
  )
{
  ASSERT(This != NULL);

  //
  // Send final message back to ME so that it can restore the FWSTS5 value (used for other messaging)
  //
  HeciPciAndThenOr32 (R_ME_H_GS, 0, H2M_HSIO_MESSAGE | H2M_HSIO_CMD_CLOSE);
  return EFI_SUCCESS;
}
