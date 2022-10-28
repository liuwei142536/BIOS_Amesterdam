/** @file
  This is the driver that initializes the Intel PCH.

@copyright
  Copyright (c) 1999 - 2017 Intel Corporation. All rights reserved
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
#include "PchInit.h"

// AptioV Server Override - Start: Program PCH OEM Svid and Sid.
#include "ProgramOemSvidSid.h"

typedef VOID (PROGRAM_PCH_OEM_SVID_SID_FUNC) (VOID);
extern PROGRAM_PCH_OEM_SVID_SID_FUNC PROGRAM_PCH_OEM_SVID_SID_LIST EndOfProgramPchOemSvidSidList;
PROGRAM_PCH_OEM_SVID_SID_FUNC*    ProgramPchOemSvidSidList[] = { PROGRAM_PCH_OEM_SVID_SID_LIST NULL};
// AptioV Server Override - End: Program PCH OEM Svid and Sid.

//
// Global Variables
//
EFI_HANDLE  mImageHandle;
#ifdef USB_PRECONDITION_ENABLE_FLAG
extern EFI_USB_HC_PORT_PRECONDITION *mPrivatePreConditionList;
#endif  // USB_PRECONDITION_ENABLE_FLAG

//
// Local function prototypes
//
EFI_STATUS
InitializePchDevice (
  IN OUT PCH_INSTANCE_PRIVATE_DATA           *PchInstance,
  IN     DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  IN     UINT32                              RootComplexBar,
  IN     UINT16                              PmBase,
  IN     UINT16                              GpioBase
  );

EFI_STATUS
ProgramSvidSid (
  IN      DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  IN      UINT32                              RootComplexBar
  );

VOID
EFIAPI
PchExitBootServicesEvent (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

VOID
EFIAPI
PchInitBeforeBoot (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

EFI_STATUS
ChipsetInitSettingsCheck (
  IN     DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy
  );
/**
  Configures PCH IOBP and stores this configuration in S3 boot script

  @param[in] RootComplexBar       RootComplexBar value of this PCH device
  @param[in] Address              Address of the IOBP register block
  @param[in] AndMask              Mask to AND with the register
  @param[in] OrMask               Mask to OR with the register
**/
VOID
ProgramIobpWithScript (
  IN UINT32  RootComplexBar,
  IN UINT32  Address,
  IN UINT32  AndMask,
  IN UINT32  OrMask
  )
{
  EFI_STATUS Status;

  Status = ProgramIobp (RootComplexBar, Address, AndMask, OrMask);
  ASSERT_EFI_ERROR (Status);

  Status = PCH_INIT_COMMON_SCRIPT_SAVE_IOBP_S3_ITEM (
              RootComplexBar,
              Address,
              AndMask,
              OrMask
              );
  ASSERT_EFI_ERROR (Status);
}

/**
  Configures 32-bit MMIO register and stores this configuration in S3 boot script

  @param[in] RootComplexBar       RootComplexBar value of this PCH device
  @param[in] Address              Address of the IOBP register block
  @param[in] AndMask              Mask to AND with the register
  @param[in] OrMask               Mask to OR with the register
**/
VOID
MmioAndThenOr32WithScript (
  IN UINTN   Address,
  IN UINT32  AndMask,
  IN UINT32  OrMask
  )
{
  MmioAndThenOr32 (Address, AndMask, OrMask);

  PCH_INIT_COMMON_SCRIPT_MEM_READ_WRITE (
    EFI_ACPI_S3_RESUME_SCRIPT_TABLE,
    EfiBootScriptWidthUint32,
    Address,
    &OrMask,
    &AndMask
    );
}

/**
  This is the standard EFI driver point that detects
  whether there is an PCH southbridge in the system
  and if so, initializes the chip.

  @param[in] ImageHandle          Handle for the image of this driver
  @param[in] SystemTable          Pointer to the EFI System Table

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
PchInitEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                        Status;
  UINT8                             BusNumber;
  UINT32                            RootComplexBar;
  DXE_PCH_PLATFORM_POLICY_PROTOCOL  *PchPlatformPolicy;
  UINTN                             NumHandles;
  EFI_HANDLE                        *HandleBuffer;
  UINT32                            Index;
  PCH_INSTANCE_PRIVATE_DATA         *PchInstance;
  UINT16                            PmBase;
  UINT16                            GpioBase;
  UINTN                             PciD31F0RegBase;

  DEBUG ((DEBUG_INFO, "PchInitEntryPoint() Start\n"));

  PchInstance       = NULL;
  PchPlatformPolicy = NULL;

  mImageHandle = ImageHandle;

  ///
  /// Retrieve all instances of PCH Platform Policy protocol
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gDxePchPlatformPolicyProtocolGuid,
                  NULL,
                  &NumHandles,
                  &HandleBuffer
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (Index = 0; Index < NumHandles; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gDxePchPlatformPolicyProtocolGuid,
                    (VOID **) &PchPlatformPolicy
                    );
    ASSERT_EFI_ERROR (Status);

    ///
    /// Allocate and install the PCH Info protocol
    ///
    BusNumber = PchPlatformPolicy->BusNumber;
    PciD31F0RegBase = MmPciAddress (
                        0,
                        PchPlatformPolicy->BusNumber,
                        PCI_DEVICE_NUMBER_PCH_LPC,
                        PCI_FUNCTION_NUMBER_PCH_LPC,
                        0
                        );
    RootComplexBar  = MmioRead32 (PciD31F0RegBase + R_PCH_LPC_RCBA) & B_PCH_LPC_RCBA_BAR;
    PmBase          = MmioRead16 (PciD31F0RegBase + R_PCH_LPC_ACPI_BASE) & B_PCH_LPC_ACPI_BASE_BAR;
    GpioBase        = MmioRead16 (PciD31F0RegBase + R_PCH_LPC_GPIO_BASE) & B_PCH_LPC_GPIO_BASE_BAR;

    ASSERT (RootComplexBar != 0);
    ASSERT (PmBase != 0);
    ASSERT (GpioBase != 0);

    DEBUG ((DEBUG_INFO, "PCH Device:\n-------------\n"));
    DEBUG ((DEBUG_INFO, "  RCBA     0x%X\n", RootComplexBar));
    DEBUG ((DEBUG_INFO, "  PmBase   0x%X\n", PmBase));
    DEBUG ((DEBUG_INFO, "  GpioBase 0x%X\n", GpioBase));
    DEBUG ((DEBUG_INFO, "-------------\n"));

    ///
    /// Dump whole DXE_PCH_PLATFORM_POLICY_PROTOCOL and serial out.
    ///
    PchDumpPlatformProtocol (PchPlatformPolicy);
    ///
    /// Initialize the PCH device
    ///
    InitializePchDevice (PchInstance, PchPlatformPolicy, RootComplexBar, PmBase, GpioBase);

    PchInstance = AllocateZeroPool (sizeof (PCH_INSTANCE_PRIVATE_DATA));
    if (PchInstance == NULL) {
      ASSERT (FALSE);
      return EFI_OUT_OF_RESOURCES;
    }

    PchInstance->PchInfo.Revision   = PCH_INFO_PROTOCOL_REVISION_2;
    PchInstance->PchInfo.BusNumber  = BusNumber;
    PchInstance->PchInfo.RCVersion  = PCH_RC_VERSION;
#ifdef USB_PRECONDITION_ENABLE_FLAG
    PchInstance->PchInfo.Preconditioned = mPrivatePreConditionList;
#endif  // USB_PRECONDITION_ENABLE_FLAG

    Status = gBS->InstallMultipleProtocolInterfaces (
                    &(HandleBuffer[Index]),
                    &gEfiPchInfoProtocolGuid,
                    &(PchInstance->PchInfo),
                    NULL
                    );
  }

  (gBS->FreePool) (HandleBuffer);

  DEBUG ((DEBUG_INFO, "PchInitEntryPoint() End\n"));

  return EFI_SUCCESS;
}

//Aptiov server override start: BLE support 
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//  Procedure:   PchBiosLockEnable
//
//  Description: This function will be called when Ready To Boot will signaled
//  will update data to work in RunTime.
//
//  Input:  IN EFI_EVENT Event - signalled event
//          IN VOID *Context - calling context
//
//  Output: VOID
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID PchBiosLockEnable (
    IN EFI_EVENT    Event,
    IN VOID         *Context
)
{
	  EFI_STATUS              Status;
	  UINT8                   Data8Or;
	  UINT8                   Data8And;
	  DXE_PCH_PLATFORM_POLICY_PROTOCOL  *PchPlatformPolicy;	  
	  UINTN                             NumHandles;
	  EFI_HANDLE                        *HandleBuffer;
	  UINT32                            Index;	  
	  DEBUG ((DEBUG_INFO, "PchBiosLockEnable() Start\n"));	  
	  ///
	  /// Retrieve all instances of PCH Platform Policy protocol
	  ///
	  Status = gBS->LocateHandleBuffer (
	                  ByProtocol,
	                  &gDxePchPlatformPolicyProtocolGuid,
	                  NULL,
	                  &NumHandles,
	                  &HandleBuffer
	                  );
	  ASSERT_EFI_ERROR (Status);
	  if (EFI_ERROR (Status)) {
	    return;
	  }
	  
	  for (Index = 0; Index < NumHandles; Index++) {
	    Status = gBS->HandleProtocol (
	                    HandleBuffer[Index],
	                    &gDxePchPlatformPolicyProtocolGuid,
	                    (VOID **) &PchPlatformPolicy
	                    );
	    ASSERT_EFI_ERROR (Status);	
	       
	    if ((PchPlatformPolicy->LockDownConfig->BiosLock == PCH_DEVICE_ENABLE)){	  
		    ///
		    /// Set SMM_BWP and BLE bit (D31:F0:RegDCh[5][1])
		    ///
		    Data8And  = 0xFF;
		    Data8Or   = (UINT8) (B_PCH_LPC_BIOS_CNTL_SMM_BWP + B_PCH_LPC_BIOS_CNTL_BLE);		    

		    MmioAndThenOr8 (
		    MmPciAddress (0,
		    PchPlatformPolicy->BusNumber,
		    PCI_DEVICE_NUMBER_PCH_LPC,
		    PCI_FUNCTION_NUMBER_PCH_LPC,
		    R_PCH_LPC_BIOS_CNTL),
		    Data8And,
		    Data8Or
		    );	
		    
		    S3BootScriptSaveMemReadWrite (
		    S3BootScriptWidthUint8,
		    MmPciAddress (0,
		    PchPlatformPolicy->BusNumber,
		    PCI_DEVICE_NUMBER_PCH_LPC,
		    PCI_FUNCTION_NUMBER_PCH_LPC,
		    R_PCH_LPC_BIOS_CNTL),
		    &Data8Or, // Data to be ORed
		    &Data8And // Data to be ANDed
		    ); 		    
	    }
	  }
	  DEBUG ((DEBUG_INFO, "PchBiosLockEnable() End\n"));		  
} //Aptiov server override end: BLE support

// AptioV Server Override - Start: Program PCH OEM Svid and Sid.
VOID
ProgramPchOemSvidSid (
  VOID )
{
  UINTN     Index;

  DEBUG ((DEBUG_INFO, "ProgramPchOemSvidSid() Start\n"));

  for ( Index=0; ProgramPchOemSvidSidList[Index]; Index++) {
      ProgramPchOemSvidSidList[Index] ();
  }

  DEBUG ((DEBUG_INFO, "ProgramPchOemSvidSid() End\n"));
}
// AptioV Server Override - End: Program PCH OEM Svid and Sid.

/**
  Initialize the PCH device according to the PCH Platform Policy protocol

  @param[in, out] PchInstance     PCH instance private data. May get updated by this function
  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] RootComplexBar       RootComplexBar value of this PCH device
  @param[in] PmBase               Power Management IO base address of this PCH device
  @param[in] GpioBase             GPIO base address of this PCH device

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
InitializePchDevice (
  IN OUT PCH_INSTANCE_PRIVATE_DATA           *PchInstance,
  IN     DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  IN     UINT32                              RootComplexBar,
  IN     UINT16                              PmBase,
  IN     UINT16                              GpioBase
  )
{
  EFI_STATUS  Status;
  BOOLEAN     AzaliaEnable;
  UINT32      FuncDisableReg;
  VOID        *Registration;
  EFI_EVENT   LegacyBootEvent;
  EFI_EVENT   ExitBootServicesEvent;
  UINT16      LpcDeviceId;
  //Aptiov server override start: BLE support
  static EFI_GUID    StartNvramSmiServicesGuid = NVRAM_START_SMI_SERVICES_GUID;  
  EFI_EVENT   EndOfDxeEvent;
  VOID        *BiosLockReg =  NULL;  
  //Aptiov server override end: BLE support
  
  DEBUG ((DEBUG_INFO, "InitializePchDevice() Start\n"));

  FuncDisableReg = MmioRead32 (RootComplexBar + R_PCH_RCRB_FUNC_DIS);

  LpcDeviceId = MmioRead16 (
                          MmPciAddress (
                          0,
                          DEFAULT_PCI_BUS_NUMBER_PCH,
                          PCI_DEVICE_NUMBER_PCH_LPC,
                          PCI_FUNCTION_NUMBER_PCH_LPC,
                          0
                          ) + R_PCH_LPC_DEVICE_ID);
  ///
  /// Take care of any ChipsetInit settings before going any further.
  ///
  Status = ChipsetInitSettingsCheck(PchPlatformPolicy);
  ASSERT_EFI_ERROR (Status);
  ///
  /// Miscellaneous power management handling
  ///
  Status = ConfigureMiscPm (PchPlatformPolicy, RootComplexBar, GpioBase);
  ASSERT_EFI_ERROR (Status);
  ///
  /// Additional power management setting
  ///
  Status = ConfigureAdditionalPm (PchPlatformPolicy, RootComplexBar);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Configures PCH DMI power management configuration
  ///
  Status = ConfigureDmiPm (PchPlatformPolicy, RootComplexBar);
  ASSERT_EFI_ERROR (Status);
  ///
  /// Deep Sx Enabling
  ///
  Status = ProgramDeepSx (PchPlatformPolicy, RootComplexBar);
  ASSERT_EFI_ERROR (Status);
  ///
  /// Perform PCH initialization sequence
  ///
  Status = ConfigureMiscItems (PchPlatformPolicy, RootComplexBar, &FuncDisableReg);
  ASSERT_EFI_ERROR (Status);

#ifdef ADSP_FLAG
    ///
    /// Configure AudioDSP
    ///
    if(IS_PCH_LPTLP_LPC_DEVICE_ID(LpcDeviceId)) {
      Status = ConfigureAudioDsp (PchPlatformPolicy, RootComplexBar, &FuncDisableReg);
      ASSERT_EFI_ERROR (Status);
    }
#endif // ADSP_FLAG

#if !defined(GROVEPORT_FLAG) && !defined(DE_SKU)
  ///
  /// Detect and initialize the type of codec present in the system
  ///
  Status = ConfigureAzalia (PchPlatformPolicy, RootComplexBar, &AzaliaEnable);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Check to disable Azalia controller
  ///

#else
  AzaliaEnable = FALSE;
#endif // !GROVEPORT_FLAG
  if (!AzaliaEnable) {
    FuncDisableReg |= B_PCH_RCRB_FUNC_DIS_AZALIA;
  }
  ///
  /// Initialize LAN
  ///
  Status = ConfigureLan (PchPlatformPolicy, RootComplexBar);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Configure USB
  ///
  Status = ConfigureUsb (PchPlatformPolicy, RootComplexBar, &FuncDisableReg);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Initialize PCIE root ports
  ///
  Status = PchInitRootPorts (PchPlatformPolicy, RootComplexBar, PmBase, &FuncDisableReg);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Thermal controller already enabled in PEI
  ///

  ///
  ///  Sata Controllers
  ///
  Status = ConfigureSata (PchPlatformPolicy, RootComplexBar, &FuncDisableReg, GpioBase);
  ASSERT_EFI_ERROR (Status);


#ifdef PCH_SERVER_BIOS_FLAG
  ///
  /// Eva Devices
  ///
  if(GetIsPchsSataPresent() == TRUE){
    Status = ConfiguresSata(PchPlatformPolicy, RootComplexBar, GpioBase);
    ASSERT_EFI_ERROR (Status);

    Status = ConfigureEva(PchPlatformPolicy);
    ASSERT_EFI_ERROR (Status);
  }

#endif // PCH_SERVER_BIOS_FLAG
  ///
  ///  Display link
  ///
  Status = ConfigureDisplay (PchPlatformPolicy, RootComplexBar);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Set the PCH Function Disable Register
  ///
  MmioWrite32 ((UINTN) (RootComplexBar + R_PCH_RCRB_FUNC_DIS), (UINT32) (FuncDisableReg));

  ///
  /// Reads back for posted write to take effect
  ///
  MmioRead32 (RootComplexBar + R_PCH_RCRB_FUNC_DIS);

  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) (RootComplexBar + R_PCH_RCRB_FUNC_DIS),
    1,
    &FuncDisableReg
    );

  ///
  /// Reads back for posted write to take effect
  ///
  S3BootScriptSaveMemReadWrite (
    S3BootScriptWidthUint32,
    (UINTN) (RootComplexBar + R_PCH_RCRB_FUNC_DIS),
    &FuncDisableReg,  // Data to be ORed
    &FuncDisableReg   // Data to be ANDed
    );
  ///
  /// Perform clock gating register settings
  /// PCH BIOS Spec Rev 0.5.0, section 19.10 Enabling Clock Gating
  ///
  ///
  /// Perform clock gating register settings
  /// PCH BIOS Spec Rev 0.5.0, section 19.10 Enabling Clock Gating
  ///
  Status = ConfigureClockGating (PchPlatformPolicy, RootComplexBar, FuncDisableReg);
  ASSERT_EFI_ERROR (Status);


  Status = ConfigureIoApic (PchPlatformPolicy, RootComplexBar);
  ASSERT_EFI_ERROR (Status);

  ProgramPchOemSvidSid (); // AptioV Server Override: Program PCH OEM Svid and Sid.

  Status = ProgramSvidSid (PchPlatformPolicy, RootComplexBar);
  ASSERT_EFI_ERROR (Status);

#ifdef SERIAL_IO_FLAG
  ///
  /// Configure Serial IO Controllers
  ///
  if(IS_PCH_LPTLP_LPC_DEVICE_ID(LpcDeviceId)) {
    Status = ConfigureSerialIo (PchPlatformPolicy, RootComplexBar);
    ASSERT_EFI_ERROR (Status);
  }
#endif // SERIAL_IO_FLAG

  ///
  /// Create an ExitPmAuth protocol call back event.
  ///
  EfiCreateProtocolNotifyEvent (
    &gExitPmAuthProtocolGuid,
    TPL_CALLBACK,
    PchInitBeforeBoot,
    NULL,
    &Registration
    );
  
//AptioV server override start:: BLE support   
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  PchBiosLockEnable,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  if (Status == EFI_SUCCESS) {
    Status = gBS->RegisterProtocolNotify (
                  &StartNvramSmiServicesGuid,
                  EndOfDxeEvent,
                  &BiosLockReg
                );  
  }
  //AptioV server override end:  BLE support

  ///
  /// Create events for PCH to do the task before ExitBootServices/LegacyBoot.
  /// It is guaranteed that only one of two events below will be signalled
  ///
  Status = gBS->CreateEvent (
                  EVT_SIGNAL_EXIT_BOOT_SERVICES,
                  TPL_CALLBACK,
                  PchExitBootServicesEvent,
                  NULL,
                  &ExitBootServicesEvent
                  );
  ASSERT_EFI_ERROR (Status);

  Status = EfiCreateEventLegacyBootEx (
            TPL_CALLBACK,
            PchExitBootServicesEvent,
            NULL,
            &LegacyBootEvent
            );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "InitializePchDevice() End\n"));

  return Status;
}

/**
  Program Pch devices Subsystem Vendor Identifier (SVID) and Subsystem Identifier (SID).

  @param[in] PchPlatformPolicy  The PCH Platform Policy protocol instance
  @param[in] RootComplexBar     RootComplexBar value of this PCH device

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ProgramSvidSid (
  IN      DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  IN      UINT32                              RootComplexBar
  )
{
  UINT8                           Index;
  UINT16                          EhciAccessCntl;
  UINT8                           BusNumber;
  UINTN                           PciEAddressBase;
  UINT8                           DeviceNumber;
  UINT8                           FunctionNumber;
  UINT8                           SvidRegOffset;
  BOOLEAN                         IsPchEhci;
  UINT16                          SubSystemVendorId;
  UINT16                          SubSystemId;

  PCH_SERIES                      PchSeries;

  DEBUG ((DEBUG_INFO, "ProgramSvidSid() Start\n"));

  PchSeries       = GetPchSeries();
  EhciAccessCntl  = 0;
  BusNumber       = PchPlatformPolicy->BusNumber;

  for (Index = 0; Index < PCH_MAX_DEVICES; Index++) {
      DeviceNumber    = PchPlatformPolicy->SvidSidConfig->SvidSidItems[Index].DeviceNumber;
      FunctionNumber  = PchPlatformPolicy->SvidSidConfig->SvidSidItems[Index].FunctionNumber;
      SvidRegOffset   = PchPlatformPolicy->SvidSidConfig->SvidSidItems[Index].SvidRegOffset;
      SubSystemVendorId = PchPlatformPolicy->SvidSidConfig->SvidSidItems[Index].DefaultSvidSid.SubSystemVendorId;
      SubSystemId = PchPlatformPolicy->SvidSidConfig->SvidSidItems[Index].DefaultSvidSid.SubSystemId;
      PciEAddressBase = MmPciAddress (
                          0,
                          BusNumber,
                          DeviceNumber,
                          FunctionNumber,
                          0
                          );
      ///
      /// Skip if the device is disabled
      ///
      if (MmioRead16 (PciEAddressBase) != V_PCH_INTEL_VENDOR_ID) {
        continue;
      }

      IsPchEhci = FALSE;
      if (PchSeries == PchH) {
        IsPchEhci = IS_PCH_H_EHCI (DeviceNumber, FunctionNumber);
      } else if (PchSeries == PchLp) {
        IsPchEhci = IS_PCH_LP_EHCI (DeviceNumber, FunctionNumber);
      }

      ///
      /// Set EHCI devices WRT_RDONLY bit (D29:F0,D26:F0:80h, bit 0) to 1, to make SVID and SID registers are writable
      ///
      if (IsPchEhci) {
        EhciAccessCntl = MmioRead16 ((UINTN) (PciEAddressBase + R_PCH_EHCI_ACCESS_CNTL));
        MmioOr16 ((UINTN) (PciEAddressBase + R_PCH_EHCI_ACCESS_CNTL), B_PCH_EHCI_ACCESS_CNTL_ENABLE);
        S3BootScriptSaveMemWrite_MMIO (
          S3BootScriptWidthUint16,
          (UINTN) (PciEAddressBase + R_PCH_EHCI_ACCESS_CNTL),
          1,
          (VOID *) (UINTN) (PciEAddressBase + R_PCH_EHCI_ACCESS_CNTL)
          );
      }

      if ((DeviceNumber == 22 && FunctionNumber == 2) || (DeviceNumber == 22 && FunctionNumber == 3)) {
        ///
        /// Sub System Identifiers register of D22:F2&F3 is 32bit access and write once
        ///
        MmioWrite32 (
          (UINTN) (PciEAddressBase + SvidRegOffset),
          (UINT32) (SubSystemVendorId |
                   (SubSystemId << 16))
          );
        S3BootScriptSaveMemWrite_MMIO (
          S3BootScriptWidthUint32,
          (UINTN) (PciEAddressBase + SvidRegOffset),
          1,
          (VOID *) (UINTN) (PciEAddressBase + SvidRegOffset)
          );
      } else {
        ///
        /// Program Pch devices Subsystem Vendor Identifier (SVID)
        ///
        MmioWrite16 (
          (UINTN) (PciEAddressBase + SvidRegOffset),
          SubSystemVendorId
          );
        S3BootScriptSaveMemWrite_MMIO (
          S3BootScriptWidthUint16,
          (UINTN) (PciEAddressBase + SvidRegOffset),
          1,
          (VOID *) (UINTN) (PciEAddressBase + SvidRegOffset)
          );

        ///
        /// Program Pch devices Subsystem Identifier (SID)
        ///
        MmioWrite16 (
          (UINTN) (PciEAddressBase + SvidRegOffset + 2),
          SubSystemId
          );
        S3BootScriptSaveMemWrite_MMIO (
          S3BootScriptWidthUint16,
          (UINTN) (PciEAddressBase + SvidRegOffset + 2),
          1,
          (VOID *) (PciEAddressBase + SvidRegOffset + 2)
          );
      }
      ///
      /// Restore the EHCI devices WRT_RDONLY bit (D29:F0,D26:F0:80h, bit 0) value
      ///
      if (IsPchEhci) {
        MmioWrite16 ((UINTN) (PciEAddressBase + R_PCH_EHCI_ACCESS_CNTL), EhciAccessCntl);
        S3BootScriptSaveMemWrite_MMIO (
          S3BootScriptWidthUint16,
          (UINTN) (PciEAddressBase + R_PCH_EHCI_ACCESS_CNTL),
          1,
          &EhciAccessCntl
          );
      }
    }

  DEBUG ((DEBUG_INFO, "ProgramSvidSid() End\n"));

  return EFI_SUCCESS;
}

/**
  Initialize R/WO Registers that described in PCH BIOS Spec

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] RootComplexBar       RootComplexBar address of this PCH device
  @param[in, out] FuncDisableReg  The value of Function disable register

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
PciERWORegInit (
  IN      DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  IN      UINT32                              RootComplexBar,
  IN OUT  UINT32                              *FuncDisableReg
  )
{
  UINTN       Index;
  UINTN       PciD31F5RegBase;
  UINT8       BusNumber;
  UINT8       RootPortFunction;
  UINTN       RPBase;
  UINT32      Data32;
  UINT16      Data16;
  UINT8       Data8;
  PCH_SERIES  PchSeries;

  DEBUG ((DEBUG_INFO, "PciERWORegInit() Start\n"));

  PchSeries       = GetPchSeries();
  BusNumber       = PchPlatformPolicy->BusNumber;
  PciD31F5RegBase = 0;
  if (PchSeries == PchH) {
    PciD31F5RegBase = MmPciAddress (0, BusNumber, 31, 5, 0);
  }

  ///
  /// PCH BIOS Spec Rev 0.5.0, Section 5.12 R/WO Registers, Table 5-4
  /// System BIOS must read the register and write the same value back to the register
  /// before passing control to the operating system.
  /// Dev:Func/Type Register Offset   Register Name                               Bits
  /// D28:F0-F7     034h              Capabilities Pointer                        7:0
  /// D28:F0-F7     040h              Capabilities List                           15:8
  /// D28:F0-F7     042h              PCI Express Capabilities                    8
  /// D28:F0~F7     044h              Device Capabilities                         2:0
  /// D28:F0-F7     04Ch              Link Capabilities                           11:10, 17:15
  /// D28:F0-F7     050h              Link Control                                3
  /// D28:F0-F7     054h              Slot Capabilities                           31:19, 16:5
  /// D28:F0-F7     064h              Device Capabilities 2                       11
  /// D28:F0-F7     080h              Message Signaled Interrupt Capability ID    15:8
  /// D28:F0-F7     090h              Port Mapping Regster                        15:8
  /// D28:F0-F7     094h              Subsystem Vendor ID                         31:0
  /// D28:F0-F7     0D8h              Miscellaneous Port Configuration            23, 2
  /// D28:F0-F7     404h              Latency Tolerance Reporting Override 2      2
  /// RCBA          21A4h             Link Capabilities                           17:15 For PCH H
  /// D31:F5        0A8h              Next Capabilities Pointer                   15:8
  /// D31:F5        0B2h              Capabilities List                           9:8
  ///
  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    if (((*FuncDisableReg) & (B_PCH_RCRB_FUNC_DIS_PCI_EX_PORT1 << Index)) == 0) {
      RootPortFunction  = GetPchPcieRpfn(RootComplexBar, (UINT8)Index);
      RPBase            = MmPciAddress (0, BusNumber, PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS, RootPortFunction, 0);
      Data32            = MmioRead32 (RPBase + R_PCH_PCIE_LCAP);
      MmioWrite32 (RPBase + R_PCH_PCIE_LCAP, Data32);
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (RPBase + R_PCH_PCIE_LCAP),
        1,
        &Data32
        );

      Data32 = MmioRead32 (RPBase + R_PCH_PCIE_SVID);
      MmioWrite32 (RPBase + R_PCH_PCIE_SVID, Data32);
      S3BootScriptSaveMemWrite (
       S3BootScriptWidthUint32,
        (UINTN) (RPBase + R_PCH_PCIE_SVID),
        1,
        &Data32
        );

      Data32 = MmioRead32 (RPBase + R_PCH_PCIE_SLCAP);
      MmioWrite32 (RPBase + R_PCH_PCIE_SLCAP, Data32);
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (RPBase + R_PCH_PCIE_SLCAP),
        1,
        &Data32
        );
      ///
      /// Added PCIe register to be lockdown
      ///
      Data8 = MmioRead8 (RPBase + R_PCH_PCIE_CAPP);
      MmioWrite8 (RPBase + R_PCH_PCIE_CAPP, Data8);
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint8,
        (UINTN) (RPBase + R_PCH_PCIE_CAPP),
        1,
        &Data8
        );

      Data16 = MmioRead16 (RPBase + R_PCH_PCIE_CLIST);
      MmioWrite16 (RPBase + R_PCH_PCIE_CLIST, Data16);
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint16,
        (UINTN) (RPBase + R_PCH_PCIE_CLIST),
        1,
        &Data16
        );

      Data16 = MmioRead16 (RPBase + R_PCH_PCIE_DCAP);
      MmioWrite16 (RPBase + R_PCH_PCIE_DCAP, Data16);
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint16,
        (UINTN) (RPBase + R_PCH_PCIE_DCAP),
        1,
        &Data16
        );

      Data16 = MmioRead16 (RPBase + R_PCH_PCIE_MID);
      MmioWrite16 (RPBase + R_PCH_PCIE_MID, Data16);
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint16,
        (UINTN) (RPBase + R_PCH_PCIE_MID),
        1,
        &Data16
        );

      Data16 = MmioRead16 (RPBase + R_PCH_PCIE_SVCAP);
      MmioWrite16 (RPBase + R_PCH_PCIE_SVCAP, Data16);
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint16,
        (UINTN) (RPBase + R_PCH_PCIE_SVCAP),
        1,
        &Data16
        );

      if (PchSeries == PchLp) {
        Data32 = MmioRead32 (RPBase + R_PCH_PCIE_L1SECH);
        Data32 |= V_PCH_PCIE_L1SECH_L1SUBST_CAP_ID;
        MmioWrite32 (RPBase + R_PCH_PCIE_L1SECH, Data32);
        S3BootScriptSaveMemWrite(
          S3BootScriptWidthUint32,
          (UINTN) (RPBase + R_PCH_PCIE_L1SECH),
          1,
          &Data32
          );

        Data32 = MmioRead32 (RPBase + R_PCH_PCIE_L1SCAP);
        MmioWrite32 (RPBase + R_PCH_PCIE_L1SCAP, Data32);
        S3BootScriptSaveMemWrite (
          S3BootScriptWidthUint32,
          (UINTN) (RPBase + R_PCH_PCIE_L1SCAP),
          1,
          &Data32
          );
      }
    }
  }

  if (PchSeries == PchH) {
    ///
    /// D31:F5:A8h[15:8]
    ///
    Data16 = MmioRead16 (PciD31F5RegBase + R_PCH_SATA_CR0);
    MmioWrite16 (PciD31F5RegBase + R_PCH_SATA_CR0, Data16);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint16,
      (UINTN) (PciD31F5RegBase + R_PCH_SATA_CR0),
      1,
      &Data16
      );
    ///
    /// D31:F5:B2h[9:8]
    ///
    Data16 = MmioRead16 (PciD31F5RegBase + R_PCH_SATA_FLR_CLV);
    MmioWrite16 (PciD31F5RegBase + R_PCH_SATA_FLR_CLV, Data16);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint16,
      (UINTN) (PciD31F5RegBase + R_PCH_SATA_FLR_CLV),
      1,
      &Data16
      );
  }

  ///
  /// D28:F0-F7:42h[8] (PCI Express Capabilities) has been done in PchInitRootPorts().
  /// D28:F0-F7:4Ch[17:15] (Link Capabilities) has been done in PchInitSingleRootPort().
  /// D28:F0-F7:404h[2] (Latency Tolerance Reporting Override 2) has been done in PchInitSingleRootPort().
  /// RCBA + 21A4h[17:15] (Link Capabilities) has been done in ConfigureDmiPm() for PCH H.
  ///

  DEBUG ((DEBUG_INFO, "PciERWORegInit() End\n"));

  return EFI_SUCCESS;
}

/**
  Set a Root Port Downstream devices ASPM and LTR S3 dispatch item, this function may assert if any error happend

  @param[in] RootPortBus          Pci Bus Number of the root port
  @param[in] RootPortDevice       Pci Device Number of the root port
  @param[in] RootPortFunc         Pci Function Number of the root port
  @param[in] RootPortAspm         Root port Aspm configuration
  @param[in] NumOfDevAspmOverride Number of Device specific ASPM policy override items
  @param[in] DevAspmOverride      Pointer to array of Device specific ASPM policy override items
  @param[in] TempBusNumberMin     Minimal temp bus number that can be assigned to the root port (as secondary
                                  bus number) and its down stream switches
  @param[in] TempBusNumberMax     Maximal temp bus number that can be assigned to the root port (as subordinate
                                  bus number) and its down stream switches
  @param[in] NumOfDevLtrOverride  Number of Device specific LTR override items
  @param[in] DevLtrOverride       Pointer to array of Device specific LTR policy override items
  @param[in] PchPwrOptPcie        Pcie Power Optimizer Configuration

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
SetPciePmS3Item (
  IN  UINT8                         RootPortBus,
  IN  UINT8                         RootPortDevice,
  IN  UINT8                         RootPortFunc,
  IN  PCH_PCI_EXPRESS_ASPM_CONTROL  RootPortAspm,
  IN  UINT8                         NumOfDevAspmOverride,
  IN  PCH_PCIE_DEVICE_ASPM_OVERRIDE *DevAspmOverride,
  IN  UINT8                         TempBusNumberMin,
  IN  UINT8                         TempBusNumberMax,
  IN  UINT8                         NumOfDevLtrOverride,
  IN  PCH_PCIE_DEVICE_LTR_OVERRIDE  *DevLtrOverride,
  IN  PCH_PCIE_PWR_OPT                      *PchPwrOptPcie,
  IN  PCH_PCIE_EXPRESS_L1SUBSTATES_CONTROL  L1SubstatesConfig,
  IN  UINT8                                 PolicyRevision,
  IN  BOOLEAN                               FirstRPToSetPm,
  IN  BOOLEAN                               L1SupportedInAllEnabledPorts,
  IN  BOOLEAN                               ClkreqSupportedInAllEnabledPorts
  )
{
  EFI_STATUS                                      Status;
#ifdef EFI_S3_RESUME
  STATIC EFI_PCH_S3_SUPPORT_PROTOCOL              *PchS3Support;
  STATIC EFI_PCH_S3_PARAMETER_PCIE_SET_PM         S3ParameterSetPm;
  STATIC EFI_PCH_S3_DISPATCH_ITEM                 S3DispatchItem = {
    PchS3ItemTypePcieSetPm,
    &S3ParameterSetPm
  };
  EFI_PHYSICAL_ADDRESS                            S3DispatchEntryPoint;

  if (!PchS3Support) {
    ///
    /// Get the PCH S3 Support Protocol
    ///
    Status = gBS->LocateProtocol (
                    &gEfiPchS3SupportProtocolGuid,
                    NULL,
                    (VOID **) &PchS3Support
                    );
///
    ASSERT_EFI_ERROR (Status);
///
///
    if (EFI_ERROR (Status)) {
      return Status;
    }

    DEBUG ((EFI_D_INFO, "Located the S3 Support Protocol - PCH Init: %x\n", (UINTN)PchS3Support));
  }

  DEBUG ((EFI_D_INFO, "Attempting to set Custom PCH Init Dispatch Item\n"));

  S3ParameterSetPm.RootPortBus          = RootPortBus;
  S3ParameterSetPm.RootPortDevice       = RootPortDevice;
  S3ParameterSetPm.RootPortFunc         = RootPortFunc;
  S3ParameterSetPm.RootPortAspm         = RootPortAspm;
  S3ParameterSetPm.NumOfDevAspmOverride = NumOfDevAspmOverride;
  S3ParameterSetPm.DevAspmOverrideAddr  = (UINT32) (UINTN) DevAspmOverride;
  S3ParameterSetPm.TempBusNumberMin     = TempBusNumberMin;
  S3ParameterSetPm.TempBusNumberMax     = TempBusNumberMax;
  S3ParameterSetPm.PchPwrOptPcie        = (UINT32) (UINTN) PchPwrOptPcie;
  S3ParameterSetPm.NumOfDevLtrOverride  = NumOfDevLtrOverride;
  S3ParameterSetPm.DevLtrOverrideAddr   = (UINT32) (UINTN) DevLtrOverride;
  S3ParameterSetPm.L1SubstatesConfig                = L1SubstatesConfig;
  S3ParameterSetPm.PolicyRevision                   = PolicyRevision;
  S3ParameterSetPm.FirstRPToSetPm                   = FirstRPToSetPm;
  S3ParameterSetPm.L1SupportedInAllEnabledPorts     = L1SupportedInAllEnabledPorts;
  S3ParameterSetPm.ClkreqSupportedInAllEnabledPorts = ClkreqSupportedInAllEnabledPorts;
  Status = PchS3Support->SetDispatchItem (
                          PchS3Support,
                          &S3DispatchItem,
                          &S3DispatchEntryPoint
                          );
  ASSERT_EFI_ERROR (Status);
  ///
  /// Save the script dispatch item in the Boot Script
  ///
  S3BootScriptSaveDispatch ((VOID*)(UINTN)S3DispatchEntryPoint);
#else
  Status = EFI_SUCCESS;
#endif
  return Status;
}

/**
  PCH initialization before ExitBootServices / LegacyBoot events
  Useful for operations which must happen later than at EndOfPost event

  @param[in] Event                A pointer to the Event that triggered the callback.
  @param[in] Context              A pointer to private data registered with the callback function.

  @retval None
**/
VOID
EFIAPI
PchExitBootServicesEvent (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                        Status;
  UINTN                             NumHandles;
  EFI_HANDLE                        *HandleBuffer;
  DXE_PCH_PLATFORM_POLICY_PROTOCOL  *PchPlatformPolicy;
  UINTN                             Index;
  UINT32                            AhciBar;
  UINTN                             PciD31F2RegBase;
  UINT16                            SataModeSelect;
  UINT16                            LpcDeviceId;
  UINT32                            PxSctlDet;
  UINT32                            PxCmdSud;
  UINT16                            OrgCmdWord;
#ifdef PCH_SERVER_BIOS_FLAG
  UINTN                             PciD17F4RegBase;
  UINT16                            sSataModeSelect;
#endif // PCH_SERVER_BIOS_FLAG

  ///
  /// Closed the event to avoid call twice
  ///
  gBS->CloseEvent (Event);

  LpcDeviceId = MmioRead16 (
                          MmPciAddress (
                          0,
                          DEFAULT_PCI_BUS_NUMBER_PCH,
                          PCI_DEVICE_NUMBER_PCH_LPC,
                          PCI_FUNCTION_NUMBER_PCH_LPC,
                          0
                          ) + R_PCH_LPC_DEVICE_ID);

  ///
  /// Retrieve all instances of PCH Platform Policy protocol
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gDxePchPlatformPolicyProtocolGuid,
                  NULL,
                  &NumHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR | DEBUG_INFO, "Failed to locate handle buffer for PCH Policy protocol.\n"));
    return;
  }

  for (Index = 0; Index < NumHandles; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gDxePchPlatformPolicyProtocolGuid,
                    (VOID **)&PchPlatformPolicy
                    );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR | DEBUG_INFO, "Failed to find PCH Policy protocol.\n"));
      return;
    }

    ConfigureXhciAtBoot (PchPlatformPolicy);

    ///
    /// eSATA port support only up to Gen2
    ///
    PciD31F2RegBase = MmPciAddress (0, PchPlatformPolicy->BusNumber, PCI_DEVICE_NUMBER_PCH_SATA, PCI_FUNCTION_NUMBER_PCH_SATA, 0);
    //
    // Make sure SATA device exists.
    //
    if (MmioRead16 (PciD31F2RegBase + R_PCH_SATA_VENDOR_ID) != 0xFFFF) {
    SataModeSelect  = MmioRead16 (PciD31F2RegBase + R_PCH_SATA_MAP) & B_PCH_SATA_MAP_SMS_MASK;
    if ((SataModeSelect == V_PCH_SATA_MAP_SMS_AHCI) ||
        (SataModeSelect == V_PCH_SATA_MAP_SMS_RAID)) {
        AhciBar = MmioRead32 (PciD31F2RegBase + R_PCH_SATA_AHCI_BAR) & B_PCH_SATA_AHCI_BAR_BA;
        //
        // Make sure the AhciBar is valid.
        //
        if ((AhciBar != 0x00000000) && (AhciBar != 0xFFFFFFFF)) {
          ///
          /// Keep original CMD word, and enable MSE
          ///
          OrgCmdWord = MmioRead16 (PciD31F2RegBase + R_PCH_SATA_COMMAND);
          if ((OrgCmdWord & B_PCH_SATA_COMMAND_MSE) == 0) {
            MmioOr16 ((PciD31F2RegBase + R_PCH_SATA_COMMAND), B_PCH_SATA_COMMAND_MSE);
          }
      for (Index = 0; Index < GetPchMaxSataPortNum (); Index++) {
        if (PchPlatformPolicy->SataConfig->PortSettings[Index].External == PCH_DEVICE_ENABLE) {
              PxSctlDet = MmioRead32(AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index))) & B_PCH_SATA_AHCI_PXSCTL_DET;
              PxCmdSud = MmioRead32(AhciBar + (R_PCH_SATA_AHCI_P0CMD  + (0x80 * Index))) & B_PCH_SATA_AHCI_PxCMD_SUD;
              ///
              /// Limit speed to Gen2
              ///
          MmioAndThenOr32 (
            (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index))),
            (UINT32)~(B_PCH_SATA_AHCI_PXSCTL_SPD),
            (UINT32) V_PCH_SATA_AHCI_PXSCTL_SPD_2
            );
          ///
          /// If port is not offline, and it's spin up, need to port reset.
          /// After port reset, clear the SERR.
          /// - Set DET=1, and then set DET=0.
          ///
          if ((PxSctlDet == V_PCH_SATA_AHCI_PXSCTL_DET_0) &&
              (PxCmdSud == B_PCH_SATA_AHCI_PxCMD_SUD))
          {
            MmioOr32 (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index)), V_PCH_SATA_AHCI_PXSCTL_DET_1);
            PchPmTimerStall (1000);
            MmioAnd32(AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index)), (UINT32) ~(B_PCH_SATA_AHCI_PXSCTL_DET));
            MmioWrite32 (AhciBar + (R_PCH_SATA_AHCI_P0SERR + (0x80 * Index)), (UINT32)~0u);
          }
          ///
          /// If port is offline, and it's not spin up, meets the power bug.
          /// Need to do the W/A to spin up the port and then spin down.
          /// Then entering back to offline and listen.
          /// - Set DET=0, SUD=1, and then set SUD=0, DET=4.
          ///
          if ((PxSctlDet == V_PCH_SATA_AHCI_PXSCTL_DET_4) &&
              (PxCmdSud == 0))
          {
            MmioAnd32(AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index)), (UINT32) ~(B_PCH_SATA_AHCI_PXSCTL_DET));
            MmioOr32 (AhciBar + (R_PCH_SATA_AHCI_P0CMD  + (0x80 * Index)), B_PCH_SATA_AHCI_PxCMD_SUD);
            PchPmTimerStall (1000);
            MmioAnd32(AhciBar + (R_PCH_SATA_AHCI_P0CMD  + (0x80 * Index)), (UINT32) ~(B_PCH_SATA_AHCI_PxCMD_SUD));
            MmioOr32 (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index)), V_PCH_SATA_AHCI_PXSCTL_DET_4);
          }
        }
      }
      ///
      /// Restore original CMD word.
      ///
      if ((OrgCmdWord & B_PCH_SATA_COMMAND_MSE) == 0) {
        MmioWrite16 ((PciD31F2RegBase + R_PCH_SATA_COMMAND), OrgCmdWord);
      }
    } // AhciBar is vaild
  } // SATA mode is AHCI or RAID
  } // if D31F2 is existed

#ifdef PCH_SERVER_BIOS_FLAG
    PciD17F4RegBase = MmPciAddress (0, PchPlatformPolicy->BusNumber, PCI_DEVICE_NUMBER_PCH_SSATA, PCI_FUNCTION_NUMBER_PCH_SSATA, 0);
    //
    // Make sure SATA device exists.
    //
    if (MmioRead16 (PciD17F4RegBase + R_PCH_SATA_VENDOR_ID) != 0xFFFF) {
      sSataModeSelect  = MmioRead16 (PciD17F4RegBase + R_PCH_SATA_MAP) & B_PCH_SATA_MAP_SMS_MASK;
      if ((sSataModeSelect == V_PCH_SATA_MAP_SMS_AHCI) ||
          (sSataModeSelect == V_PCH_SATA_MAP_SMS_RAID)) {
        AhciBar = MmioRead32 (PciD17F4RegBase + R_PCH_SATA_AHCI_BAR) & B_PCH_SATA_AHCI_BAR_BA;
        //
        // Make sure the AhciBar is valid.
        //
        if ((AhciBar != 0x00000000) && (AhciBar != 0xFFFFFFFF)) {
          ///
          /// Keep original CMD word, and enable MSE
          ///
          OrgCmdWord = MmioRead16 (PciD17F4RegBase + R_PCH_SATA_COMMAND);
          if ((OrgCmdWord & B_PCH_SATA_COMMAND_MSE) == 0) {
            MmioOr16 ((PciD17F4RegBase + R_PCH_SATA_COMMAND), B_PCH_SATA_COMMAND_MSE);
          }
      for (Index = 0; Index < GetPchMaxsSataPortNum (); Index++) {
        if (PchPlatformPolicy->sSataConfig->PortSettings[Index].External == PCH_DEVICE_ENABLE) {
              PxSctlDet = MmioRead32(AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index))) & B_PCH_SATA_AHCI_PXSCTL_DET;
              PxCmdSud = MmioRead32(AhciBar + (R_PCH_SATA_AHCI_P0CMD  + (0x80 * Index))) & B_PCH_SATA_AHCI_PxCMD_SUD;
              ///
              /// Limit speed to Gen2
              ///
          MmioAndThenOr32 (
            (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index))),
            (UINT32)~(B_PCH_SATA_AHCI_PXSCTL_SPD),
            (UINT32) V_PCH_SATA_AHCI_PXSCTL_SPD_2
            );
              ///
              /// If port is not offline, and it's spin up, need to port reset.
              /// After port reset, clear the SERR.
              /// - Set DET=1, and then set DET=0.
              ///
              if ((PxSctlDet == V_PCH_SATA_AHCI_PXSCTL_DET_0) &&
                  (PxCmdSud == B_PCH_SATA_AHCI_PxCMD_SUD))
              {
          MmioOr32 (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index)), V_PCH_SATA_AHCI_PXSCTL_DET_1);
          PchPmTimerStall (1000);
          MmioAnd32(AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index)), (UINT32) ~(B_PCH_SATA_AHCI_PXSCTL_DET));
                MmioWrite32 (AhciBar + (R_PCH_SATA_AHCI_P0SERR + (0x80 * Index)), (UINT32)~0u);
              }
              ///
              /// If port is offline, and it's not spin up, meets the power bug.
              /// Need to do the W/A to spin up the port and then spin down.
              /// Then entering back to offline and listen.
              /// - Set DET=0, SUD=1, and then set SUD=0, DET=4.
              ///
              if ((PxSctlDet == V_PCH_SATA_AHCI_PXSCTL_DET_4) &&
                  (PxCmdSud == 0))
              {
                MmioAnd32(AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index)), (UINT32) ~(B_PCH_SATA_AHCI_PXSCTL_DET));
                MmioOr32 (AhciBar + (R_PCH_SATA_AHCI_P0CMD  + (0x80 * Index)), B_PCH_SATA_AHCI_PxCMD_SUD);
                PchPmTimerStall (1000);
                MmioAnd32(AhciBar + (R_PCH_SATA_AHCI_P0CMD  + (0x80 * Index)), (UINT32) ~(B_PCH_SATA_AHCI_PxCMD_SUD));
                MmioOr32 (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index)), V_PCH_SATA_AHCI_PXSCTL_DET_4);
              }
            }
          }
          ///
          /// Restore original CMD word.
          ///
          if ((OrgCmdWord & B_PCH_SATA_COMMAND_MSE) == 0) {
            MmioWrite16 ((PciD17F4RegBase + R_PCH_SATA_COMMAND), OrgCmdWord);
          }
        } // AhciBar is vaild
      } // SATA mode is AHCI or RAID
    } // if D17F4 is existed
#endif // PCH_SERVER_BIOS_FLAG
  }

  return;
  }


/**
  Register the HECI protocol callback function for the ChipsetInit sync message.

  @param[in] *PchPlatformPolicy          A pointer to the PchPlatformPolicy.

  @retval EFI_STATUS
**/
EFI_STATUS
ChipsetInitSettingsCheck (
  IN     DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy
  )
{
  EFI_STATUS                            Status;

  DEBUG ((EFI_D_INFO, "ChipsetInitSettingsCheck() Start\n"));
  Status = PchPlatformPolicy->MphyRegisterPchInitSycnCallback(PchPlatformPolicy);
  return Status;
}

/**
  Update ASL object before Boot

  @param[in] *PchPlatformPolicy         A pointer to the PchPlatformPolicy.
  @param[in] RootComplexBar             RootComplexBar value of this PCH device

  @retval EFI_STATUS
  @retval EFI_NOT_READY                 The Acpi protocols are not ready.
**/
EFI_STATUS
PchUpdateAslObjects (
  IN DXE_PCH_PLATFORM_POLICY_PROTOCOL   *PchPlatformPolicy,
  IN UINT32                             RootComplexBar
  )
{
  EFI_STATUS                            Status;
  UINTN                                 Index;
  UINTN                                 VariableSize;
  UINT32                                PciMemBase;
  PCH_LATE_INIT_SMM_VARIABLE            SaveRestoreData;
  UINT32                                AslSignature;
  UINT32                                RpFn;
  UINT32                                Data32;

  Status = InitializePchAslUpdateLib();
  DEBUG ((DEBUG_INFO, "InitializePchAslUpdateLib Status %x\n", Status));
  ASSERT_EFI_ERROR (Status);

  if(EFI_ERROR(Status)) {
    return EFI_NOT_READY;
  }

  //
  // Update SRMB, Save & Restore Memroy Base
  //
  VariableSize = sizeof (PCH_LATE_INIT_SMM_VARIABLE);
  Status = gRT->GetVariable (
              PCH_INIT_PEI_VARIABLE_NAME,
              &gPchInitPeiVariableGuid,
              NULL,
              &VariableSize,
              &SaveRestoreData
              );
  PciMemBase = SaveRestoreData.PciMemBase;
  Status = UpdateNameAslCode(SIGNATURE_32('S','R','M','B'), &PciMemBase, sizeof(PciMemBase));

  //
  // Update RPA0, RPA1, RPA2, RPA3, RPA4, RPA5, RPA6, RPA7 for root port function swapping
  //
  RpFn = MmioRead32 (RootComplexBar + R_PCH_RCRB_RPFN);
  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    Data32       =  '0' + (UINT32)Index;
    AslSignature =  SIGNATURE_32('R','P','A',Data32);
    Data32       =  (UINT32)((RpFn >> (Index * S_PCH_RCRB_PRFN_RP_FIELD)) & B_PCH_RCRB_RPFN_RP1FN);
    Data32       |= (UINT32)(PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS << 16);
    Status       =  UpdateNameAslCode(AslSignature, &Data32, sizeof(UINT32));
    DEBUG ((DEBUG_INFO, "Update RPAx %x %x\n", Index, Data32));
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Update PCHS.
  //
  Data32 = (UINT32) GetPchSeries();
  Status = UpdateNameAslCode(SIGNATURE_32('P','C','H','S'), &Data32, sizeof(UINT32));

  return EFI_SUCCESS;
}

/**
  PCH initialization before Boot Sript Table is closed

  @param[in] Event                A pointer to the Event that triggered the callback.
  @param[in] Context              A pointer to private data registered with the callback function.

  @retval None
**/
VOID
EFIAPI
PchInitBeforeBoot (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                        Status;
  UINTN                             NumHandles;
  EFI_HANDLE                        *HandleBuffer;
  UINTN                             Index;
  DXE_PCH_PLATFORM_POLICY_PROTOCOL  *PchPlatformPolicy;
  UINT32                            RootComplexBar;
  UINT32                            FuncDisableReg;
  UINTN                             PciD25F0RegBase;
  UINTN                             PciD31F2RegBase;
#ifdef PCH_SERVER_BIOS_FLAG
  UINTN                             PciD17F4RegBase;
#endif // PCH_SERVER_BIOS_FLAG
  UINT32                            GbEMemBar;
  UINT32                            GbEFlashLock;
  UINTN                             PciD31F0RegBase;
  UINT16                            PmBase;
  UINT16                            GpioBase;
  UINT32                            Data32And;
  UINT32                            Data32Or;
  UINT16                            Data16And;
  UINT16                            Data16Or;
  UINT8                             Data8;
  VOID                              *ProtocolPointer;
  UINTN                             AzaliaBase;
  const UINT8                       StrEnabled[sizeof (PCH_CRID_ENABLED)]    = PCH_CRID_ENABLED;
  const UINT8                       StrDisabled[sizeof (PCH_CRID_DISABLED)]  = PCH_CRID_DISABLED;
  EFI_HANDLE                        Handle;
  EFI_PCH_SET_TMCSRCCLK_PROTOCOL    *PchSetTmcSrcClk;
  PCH_SERIES                        PchSeries;
  BOOLEAN                           L1SubstatesSupportedPerPort;
  BOOLEAN                           L1SubstatesSupported;
  PCI_DATA_STRUCTURE                        *PcirBlockPtr;
  EFI_PCI_IO_PROTOCOL                       *PciIo;
  PCI_EXPANSION_ROM_HEADER                  *RomImage;
  BOOLEAN                                   FoundLegacyRaid;
  EFI_COMPONENT_NAME2_PROTOCOL              *ComponentName2;
  CHAR16                                    *RstDriverName1;//[] = L"Intel RST";
  CHAR16                                    *RstDriverName2;//[] = L"Intel(R) RST";
  EFI_DRIVER_SUPPORTED_EFI_VERSION_PROTOCOL *DriverEfiVersion;
  CHAR16                                    *DriverName;
  UINT16                                    AspmVal;
  BOOLEAN                                   ClkreqPerPortSupported;
  BOOLEAN                                   ClkreqSupportedInAllEnabledPorts;
  BOOLEAN                                   L1SupportedInAllEnabledPorts;
  BOOLEAN                                   FirstRPToSetPm;
#ifdef EFI_S3_RESUME
  STATIC EFI_PCH_S3_SUPPORT_PROTOCOL        *PchS3Support;
#endif
  L1SubstatesSupportedPerPort      = FALSE;
  L1SubstatesSupported             = FALSE;
  Handle                           = NULL;
  PchSetTmcSrcClk                  = NULL;
  PchSeries                        = GetPchSeries();
  AspmVal                          = 0;
  ClkreqPerPortSupported           = FALSE;
  ClkreqSupportedInAllEnabledPorts = TRUE;
  L1SupportedInAllEnabledPorts     = TRUE;
  FirstRPToSetPm                   = TRUE;
  RstDriverName1 = L"Intel RSTe";
  RstDriverName2 = L"Intel(R) RSTe";
  ///
  /// Check whether this is real ExitPmAuth notification, or just a SignalEvent
  ///
  Status = gBS->LocateProtocol (&gExitPmAuthProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_ERROR (Status)) {
    return;
  }

  DEBUG ((DEBUG_INFO, "PchInitBeforeBoot() Start\n"));

  ///
  /// Closed the event to avoid call twice when launch shell
  ///
  gBS->CloseEvent (Event);

  ///
  /// Retrieve all instances of PCH Platform Policy protocol
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gDxePchPlatformPolicyProtocolGuid,
                  NULL,
                  &NumHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR | DEBUG_INFO, "Failed to locate handle buffer for PCH Policy protocol.\n"));
    return;
  }
  ///
  /// Find the matching PCH Policy protocol
  ///
  for (Index = 0; Index < NumHandles; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gDxePchPlatformPolicyProtocolGuid,
                    (VOID **) &PchPlatformPolicy
                    );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR | DEBUG_INFO, "Failed to find PCH Policy protocol.\n"));
      return;
    }

    InitFviDataHubCbContext (
      PchPlatformPolicy->MiscConfig->FviSmbiosType,
      (UINT8) mPchFviElements,
      &mPchFviVersionData
      );

    RootComplexBar  = PCH_RCRB_BASE;

    FuncDisableReg  = MmioRead32 (RootComplexBar + R_PCH_RCRB_FUNC_DIS);
    PciD31F0RegBase = MmPciAddress (0, PchPlatformPolicy->BusNumber, 31, 0, 0);
    GpioBase        = MmioRead16 (PciD31F0RegBase + R_PCH_LPC_GPIO_BASE) & B_PCH_LPC_GPIO_BASE_BAR;
    PmBase          = MmioRead16 (PciD31F0RegBase + R_PCH_LPC_ACPI_BASE) & B_PCH_LPC_ACPI_BASE_BAR;

    Status          = PciERWORegInit (PchPlatformPolicy, RootComplexBar, &FuncDisableReg);
    ASSERT_EFI_ERROR (Status);

#ifdef PCH_SERVER_BIOS_FLAG
    // EVA (RHS) lock down
    if (GetIsPchsSataPresent() == TRUE){
       Status = EvaLockDown(PchPlatformPolicy);
       ASSERT_EFI_ERROR(Status);
    }
#endif // PCH_SERVER_BIOS_FLAG
    ///
    /// Locking Thermal Reporting Settings prior to end of POST
    ///
#ifdef DE_SKU
    ///
    /// PowerManagement WA
    ///
    if(PchPlatformPolicy->EnableThermalLockDown){
#endif //DE_SKU
      Status = ThermalLockDown (PchPlatformPolicy, GpioBase);
#ifdef DE_SKU
    ///
    /// PowerManagement WA
    ///
    }
#endif //DE_SKU
    ASSERT_EFI_ERROR (Status);

    if ((FuncDisableReg & B_PCH_RCRB_FUNC_DIS_AZALIA) == 0) {
      AzaliaBase = MmPciAddress (
                    0,
                    PchPlatformPolicy->BusNumber,
                    PCI_DEVICE_NUMBER_PCH_AZALIA,
                    PCI_FUNCTION_NUMBER_PCH_AZALIA,
                    0
                    );
      ///
      /// PCH BIOS Spec Rev 0.5.0 Section 9.5
      /// Additional High Definition Audio Programming Steps
      /// Step 5
      /// Set D27:F0:40h[1] = 1b after all settings done including 19.10.5
      ///
      MmioOr8 (AzaliaBase + R_PCH_HDA_HDCTL, B_PCH_HDA_HDCTL_BCLD);
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint8,
        (UINTN) (AzaliaBase + R_PCH_HDA_HDCTL),
        1,
        (VOID *) (UINTN) (AzaliaBase + R_PCH_HDA_HDCTL)
        );
    }
    ///
    /// PCH BIOS Spec Rev 0.5.0 Section 19.4
    /// Step 29
    /// Set RCBA + Offset 3A6Ch[31:0] = 0x00000001, after step #26 to #28 are done
    ///
    MmioWrite32 (
      (UINTN) (RootComplexBar + R_PCH_RCRB_CIR26),
      (UINT32) (0x00000001)
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (RootComplexBar + R_PCH_RCRB_CIR26),
      1,
      (VOID *) (UINTN) (RootComplexBar + R_PCH_RCRB_CIR26)
      );

    if (PchSeries == PchH) {
      ///
      /// PCH BIOS Spec Rev 0.5.5, Section 19.4 Additional Power Management Programming
      /// Step 30
      /// Set RCBA + Offset 2344h[31:24] = 0xFF
      /// Set RCBA + Offset 2344h[7:0] = 0x0C
      ///
      Data32And = (UINT32)~(0xFF00000F);
      Data32Or  = (UINT32) (0xFF00000C);
      MmioAndThenOr32 (
        (UINTN) (RootComplexBar + 0x2344),
        Data32And,
        Data32Or
        );
      S3BootScriptSaveMemReadWrite (
        S3BootScriptWidthUint32,
        (UINTN) (RootComplexBar + 0x2344),
        &Data32Or,  // Data to be ORed
        &Data32And  // Data to be ANDed
        );
    } else if (PchSeries == PchLp) {
      ///
      /// PCH BIOS Spec Rev 0.3.0 Section 31.7.2
      /// Step 5
      /// Set RCBA + Offset 2618h [25] = 1b.
      ///
      MmioOr32 (
        (UINTN) (RootComplexBar + 0x2618),
        (UINT32) (BIT25)
      );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (RootComplexBar + 0x2618),
        1,
        (VOID *) (UINTN) (RootComplexBar + 0x2618)
       );
    }
    ///
    /// SPI Flash Programming Guide Section 5.5.1 Flash Configuration Lockdown
    /// It is strongly recommended that BIOS sets the Host and GbE Flash Configuration Lock-Down (FLOCKDN)
    /// bits (located at SPIBAR + 04h and MBAR + 04h respectively) to 1 on production platforms
    ///
    if (PchPlatformPolicy->LockDownConfig->FlashLockDown == 1) { //AptioV server override: Adding control for SPI Lock    
    if (PchSeries == PchH) {
      MmioOr16 ((UINTN) (RootComplexBar + R_PCH_SPI_HSFS), (UINT16) (B_PCH_SPI_HSFS_FLOCKDN + B_PCH_SPI_PRR3PRR4_LOCKDN));
    } else if (PchSeries == PchLp) {
      MmioOr16 ((UINTN) (RootComplexBar + R_PCH_SPI_HSFS), (UINT16) (B_PCH_SPI_HSFS_FLOCKDN));
    }
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint16,
      (UINTN) (RootComplexBar + R_PCH_SPI_HSFS),
      1,
      (VOID *) (UINTN) (RootComplexBar + R_PCH_SPI_HSFS)
      );
    }  //AptioV server override: Adding control for SPI Lock
    ///
    /// Set the GbE Flash Configuration Lock-Down (FLOCKDN) bit (MBAR + 04h[15]) to 1
    ///
    if (PchPlatformPolicy->DeviceEnabling->Lan == PCH_DEVICE_ENABLE) {
      PciD25F0RegBase = MmPciAddress (0, PchPlatformPolicy->BusNumber, 25, 0, 0);
      ///
      /// Enable memory space decoding in command register
      ///
      Data16And = 0xFFFF;
      Data16Or  = (UINT16) B_PCH_LAN_CMD_MSE;
      MmioOr16 (PciD25F0RegBase + R_PCH_LAN_CMD, Data16Or);
      S3BootScriptSaveMemReadWrite (
          S3BootScriptWidthUint16,
        (UINTN) (PciD25F0RegBase + R_PCH_LAN_CMD),
        &Data16Or,  // Data to be ORed
        &Data16And  // Data to be ANDed
        );
      GbEMemBar = MmioRead32 (PciD25F0RegBase + R_PCH_LAN_MBARB) & B_PCH_LAN_MBARB_BA;
      ///
      /// Assert if the memory data of GbEMemBar is invalid.
      ///
      if (MmioRead32 (GbEMemBar) == 0xFFFFFFFF) {
        ASSERT (FALSE);
      } else {
        S3BootScriptSaveMemWrite (
          S3BootScriptWidthUint32,
          (UINTN) (PciD25F0RegBase + R_PCH_LAN_MBARB),
          1,
          (VOID *) (UINTN) (PciD25F0RegBase + R_PCH_LAN_MBARB)
          );

      if (PchPlatformPolicy->LockDownConfig->GbeFlashLockDown == 1) {
 DEBUG ((DEBUG_INFO, " PCH_LOCK_DOWN_CONFIG GbeFlashLockDown= %x\n", PchPlatformPolicy->LockDownConfig->GbeFlashLockDown));
 DEBUG ((DEBUG_INFO, " GbEMemBar + 0x04= %x\n", GbEMemBar + 0x04));
        MmioOr16 (GbEMemBar + 0x04, BIT15);
      }

	  GbEFlashLock = MmioRead32 (GbEMemBar + 0x04);
		  DEBUG ((DEBUG_INFO, "GbEFlashLock= %x\n", GbEFlashLock));

      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint16,
          (UINTN) (GbEMemBar + 0x04),
          1,
          (VOID *) (UINTN) (GbEMemBar + 0x04)
          );
      }
      ///
      /// PCH BIOS Spec Rev 0.5.0 Section 10.5 Additional GbE Controller Configurations for WOL Support
      /// System BIOS requires to program the registers listed below for internal GbE.
      /// Step 1, Set MBARA + Offset 2Ch [31] = 1b
      /// Step 2, If WOL is enabled set MBARA + Offset 2Ch [30] = 1b
      /// else if disabled set MBARA + Offset 2Ch [30] = 0b
      ///
      /// Additional Steppings:
      /// Set MBARA + Offset 10h [31] = 1b
      ///
      GbEMemBar = MmioRead32 (PciD25F0RegBase + R_PCH_LAN_MEM_BASE_A) & B_PCH_LAN_MBARA_BA;
      ///
      /// Assert if the memory data of GbEMemBar is invalid.
      ///
      if (MmioRead32 (GbEMemBar) == 0xFFFFFFFF) {
//
// ESS: OverrideBegin
//
//#ifndef SV_HOOKS_CPU
//        ASSERT (FALSE);
//#endif
//
// ESS: OverrideEnd
//
      } else {
        S3BootScriptSaveMemWrite (
          S3BootScriptWidthUint32,
          (UINTN) (PciD25F0RegBase + R_PCH_LAN_MEM_BASE_A),
          1,
          (VOID *) (UINTN) (PciD25F0RegBase + R_PCH_LAN_MEM_BASE_A)
          );
        MmioOr32 (GbEMemBar + 0x2c, (BIT31));
        if (PchPlatformPolicy->MiscPmConfig->WakeConfig.WolEnableOverride) {
          MmioOr32 (GbEMemBar + 0x2c, (BIT30));
        } else {
          MmioAnd32 (GbEMemBar + 0x2c, (UINT32) (~BIT30));
        }
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
          (UINTN) (GbEMemBar + 0x2c),
          1,
          (VOID *) (UINTN) (GbEMemBar + 0x2c)
          );

        MmioOr32(GbEMemBar + 0x10, BIT31);

      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
          (UINTN) (GbEMemBar + 0x10),
          1,
          (VOID *) (UINTN) (GbEMemBar + 0x10)
          );
      }
    }
    ///
    /// PCH BIOS Spec Rev 0.5.0 section 14.1.7 Additional Programming Requirements during
    /// SATA Initialization
    /// Step 11
    /// Program D31:F2:98h [29] to 1b
    ///
    PciD31F2RegBase = MmPciAddress (0, PchPlatformPolicy->BusNumber, 31, 2, 0);
    MmioOr32 ((UINTN) (PciD31F2RegBase + 0x98), BIT29);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciD31F2RegBase + 0x98),
      1,
      (VOID *) (UINTN) (PciD31F2RegBase + 0x98)
      );
    ///
    /// PCH BIOS Spec Rev 0.5.0 section 14.1.7 Additional Programming Requirements during
    /// SATA Initialization
    /// Step 14
    /// Program D31:F2:9Ch [31] to 1b
    ///
    MmioOr32 ((UINTN) (PciD31F2RegBase + R_PCH_SATA_SCLKGC), B_PCH_SATA_SCLKGC_REGLOCK);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_SCLKGC),
      1,
      (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_SCLKGC)
      );

#ifdef PCH_SERVER_BIOS_FLAG
    ///
    /// PCH BIOS Spec Rev 0.5.0 section 14.1.7 Additional Programming Requirements during
    /// sSATA Initialization
    /// Step 11
    /// Program D17:F4:98h [29] to 1b
    ///
    PciD17F4RegBase = MmPciAddress (0, PchPlatformPolicy->BusNumber, 17, 4, 0);
    MmioOr32 ((UINTN) (PciD17F4RegBase + 0x98), BIT29);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciD17F4RegBase + 0x98),
      1,
      (VOID *) (UINTN) (PciD17F4RegBase + 0x98)
      );
    ///
    /// PCH BIOS Spec Rev 0.5.0 section 14.1.7 Additional Programming Requirements during
    /// sSATA Initialization
    /// Step 14
    /// Program D17:F4:9Ch [31] to 1b
    ///
    MmioOr32 ((UINTN) (PciD17F4RegBase + R_PCH_SATA_SCLKGC), B_PCH_SATA_SCLKGC_REGLOCK);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_SCLKGC),
      1,
      (VOID *) (UINTN) (PciD17F4RegBase + R_PCH_SATA_SCLKGC)
      );
#endif // PCH_SERVER_BIOS_FLAG

    //
// ESS: OverrideBegin
    //
    // It only applies for PCH client and not for server so force not execute this code
    // to avoid memory leak issue after Readytoboot event
    //

    //
    // ESS: Override End
    //
    ///
    /// SPI Flash Programming Guide Section 5.5.2 Vendor Component Lock
    /// It is strongly recommended that BIOS sets the Vendor Component Lock (VCL) bits. VCL applies
    /// the lock to both VSCC0 and VSCC1 even if VSCC0 is not used. Without the VCL bits set, it is
    /// possible to make Host/GbE VSCC register(s) changes in that can cause undesired host and
    /// integrated GbE Serial Flash functionality.
    ///
    MmioOr32 ((UINTN) (RootComplexBar + R_PCH_SPI_VSCC0), B_PCH_SPI_VSCC0_VCL);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (RootComplexBar + R_PCH_SPI_VSCC0),
      1,
      (VOID *) (UINTN) (RootComplexBar + R_PCH_SPI_VSCC0)
      );

    ///
    /// PCH BIOS Spec Rev 0.5.0, Section 19.4 Additional Power Management Programming
    /// Step 3
    /// Set GEN_PMCON_LOCK register, D31:F0:A6h = 06h, after stretch and ACPI base programming completed.
    ///
    MmioOr8 (
      (UINTN) (PciD31F0RegBase + R_PCH_LPC_GEN_PMCON_LOCK),
      (UINT8) (B_PCH_LPC_GEN_PMCON_LOCK_S4_STRET_LD | B_PCH_LPC_GEN_PMCON_LOCK_ABASE_LK)
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint8,
      (UINTN) (PciD31F0RegBase + R_PCH_LPC_GEN_PMCON_LOCK),
      1,
      (VOID *) (UINTN) (PciD31F0RegBase + R_PCH_LPC_GEN_PMCON_LOCK)
      );
    ///
    /// PCH BIOS Spec Rev 0.5.0, Section 3.6 Flash Security Recommendation
    /// Step 1
    /// Intel strongly recommends that BIOS enables the BIOS Lock Enable (BLE) feature of the PCH.
    /// Left to platform code to register an callback function to handle IchnBiosWp SMI
    ///
    /// Step 2
    /// Intel strongly recommends that BIOS enables SMI_LOCK (B0:D31:F0:Offset A0h [4]=1)
    /// which prevent writes to the Global SMI Enable bit (GLB_SMI_EN PMBASE + 30h Bit
    /// [0]). Enabling this bit will mitigate malicious software attempts to gain system management
    /// mode privileges.
    ///
    if (PchPlatformPolicy->LockDownConfig->GlobalSmi == PCH_DEVICE_ENABLE) {
      ///
      /// Save Global SMI Enable bit setting before BIOS enables SMI_LOCK during S3 resume
      ///

      Data32Or  = IoRead32 ((UINTN) (PmBase + R_PCH_SMI_EN));
        Data32And = 0xFFFFFFFF;
        S3BootScriptSaveIoWrite (
          S3BootScriptWidthUint32,
          (UINTN) (PmBase + R_PCH_SMI_EN),
          1,
          &Data32Or  // Data to be write
          );

      MmioOr8 ((UINTN) (PciD31F0RegBase + R_PCH_LPC_GEN_PMCON_1), B_PCH_LPC_GEN_PMCON_SMI_LOCK);
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint8,
        (UINTN) (PciD31F0RegBase + R_PCH_LPC_GEN_PMCON_1),
        1,
        (VOID *) (UINTN) (PciD31F0RegBase + R_PCH_LPC_GEN_PMCON_1)
        );
    }
    ///
    /// Step 3
    /// Intel strongly recommends that BIOS sets the BIOS Interface Lock Down bit
    /// (RCBA+3410[0]=1 General Control and Status - BILD). Setting this bit will prevent writes
    /// to the Backup Control Register Top Swap bit (BUC.TS RCBA + 3414 [0]) and the General
    /// Control and Status Registers Boot BIOS Straps (RCBA + 3410h [11:10]). Enabling this bit
    /// will mitigate malicious software attempts to replace the system BIOS option ROM with its own code.
    ///
    if (PchPlatformPolicy->LockDownConfig->BiosInterface == PCH_DEVICE_ENABLE) {
      MmioOr32 ((UINTN) (RootComplexBar + R_PCH_RCRB_GCS), (UINT32) B_PCH_RCRB_GCS_BILD);
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (RootComplexBar + R_PCH_RCRB_GCS),
        1,
        (VOID *) (UINTN) (RootComplexBar + R_PCH_RCRB_GCS)
        );
      ///
      /// Reads back for posted write to take effect
      ///
      Data32Or = MmioRead32 ((UINTN) (RootComplexBar + R_PCH_RCRB_GCS));
      S3BootScriptSaveMemReadWrite (
        S3BootScriptWidthUint32,
        (UINTN) (RootComplexBar + R_PCH_RCRB_GCS),
        &Data32Or,  /// BitMask
        &Data32Or  /// BitValue
        );
    }
    ///
    /// PCH EDS Rev 1.0, Section 14.1.35.1.2
    /// SATA Indexed Register 1Ch Bit18, 1 = This bit allows entrance to the PCH SATA test modes when set.
    /// This bit should only be set when following the PCH MSQT for system board testing.
    /// It is recommended to set this bit manually.
    ///

    ///
    /// PCH BIOS Spec Rev 0.5.0, Section 5.15 IntelR Stable Image Platform Program (SIPP)
    /// For platforms supporting Intel(R) SIPP, System BIOS will need to enable the CRID feature by:
    /// Write the value 1Dh to to the RevID field in B0:D31:F0 Offset 08h
    ///

    ///
    /// Update CRID FVI record
    ///
    mPchFviElementsData[CRID_ORIGINAL].Element.Version.BuildNum = (UINT16) MmioRead8 ((UINTN) (PciD31F0RegBase + R_PCH_LPC_RID));
    if (PchPlatformPolicy->DeviceEnabling->Crid == PCH_DEVICE_ENABLE) {
      Data8 = 0x1D;
      MmioWrite8 ((UINTN) (PciD31F0RegBase + R_PCH_LPC_RID), Data8);
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint8,
        (UINTN) (PciD31F0RegBase + R_PCH_LPC_RID),
        1,
        &Data8
        );
      CopyMem (mPchFviElementsData[CRID_STATUS].Element.VerString, StrEnabled, sizeof (StrEnabled));
    } else {
      CopyMem (mPchFviElementsData[CRID_STATUS].Element.VerString, StrDisabled, sizeof (StrDisabled));
    }

    mPchFviElementsData[CRID_NEW].Element.Version.BuildNum = (UINT16) MmioRead8 ((UINTN) (PciD31F0RegBase + R_PCH_LPC_RID));

    ///
    /// PCH BIOS Spec Rev 0.5.0, Section 16.4 GPIO Registers Lockdown
    /// If GPIO configurations are not used after boot, it is recommended that the GLE Lockdown Enable
    /// and the GPIO_UNLOCK_SMI_EN bits are set by BIOS prior to end of POST.
    ///
    if (PchPlatformPolicy->LockDownConfig->GpioLockDown == PCH_DEVICE_ENABLE) {
      ///
      /// Set GPIO Lockdown Enable bit
      ///
      MmioOr8 ((UINTN) (PciD31F0RegBase + R_PCH_LPC_GPIO_CNT), (UINT8) B_PCH_LPC_GPIO_LOCKDOWN_EN);
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint8,
        (UINTN) (PciD31F0RegBase + R_PCH_LPC_GPIO_CNT),
        1,
        (VOID *) (UINTN) (PciD31F0RegBase + R_PCH_LPC_GPIO_CNT)
        );
      ///
      /// Please locate SMM ICHn SMI Dispatch Extended Protocol and register the callback function to
      /// IchnExGpioUnlock to set GPIO_UNLOCK_SMI_EN bit in the platform code.
      ///
    }
    ///
    /// PCH BIOS Spec Rev 0.5.0, Section 5.13 BIOS guide on using RTC RAM
    /// For Data integrity protection, set RTC Memory locks (Upper 128 Byte Lock and
    /// Lower 128 Byte Lock) at RCBA + 3400h[4] and RCBA + 3400h[3]. Note once locked
    /// bytes 0x38 - 0x3F in each of the Upper and Lower Byte blocks, respectively,
    /// cannot be unlocked until next reset.
    ///
    if (PchPlatformPolicy->LockDownConfig->RtcLock == PCH_DEVICE_ENABLE) {
      MmioOr32 (
        (UINTN) (RootComplexBar + R_PCH_RCRB_RTC_CONF),
        (UINT32) (B_PCH_RCRB_RTC_CONF_UCMOS_LOCK | B_PCH_RCRB_RTC_CONF_LCMOS_LOCK)
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (RootComplexBar + R_PCH_RCRB_RTC_CONF),
        1,
        (VOID *) (UINTN) (RootComplexBar + R_PCH_RCRB_RTC_CONF)
        );

      //
      //	Set Strap Message Lock
      //	All of the SSML, SSMC and SSMD is locked
      //
      MmioOr32 (
        (UINTN) (RootComplexBar + R_PCH_SPI_SRDL),
        (UINT32) (B_PCH_SPI_SRDL_SSL)
        );
    }
    ///
    ///
    /// PCH BIOS Spec Rev 0.5.0, Section 3.6 Flash Security Recommendation
    /// Step 1
    /// BIOS needs to enable the BIOS Lock Enable (BLE) feature of the PCH by setting
    /// B0:D31:F0:DCh[1] = 1b. When this bit is set, attempts to write the BIOS Write
    /// Enable (BIOSWE) bit in PCH will cause a SMI which will allow the BIOS to verify
    /// that the write is from a valid source. Remember that BIOS needs to set B0:D31:F0
    /// Offset DC [0] = 0b to enable BIOS region protection before exiting the SMI handler.
    /// Also, TCO_EN bit needs to be set (SMI_EN Register, PMBASE + 30h[13] = 1b) to keep
    /// BLE feature enabled after booting to the OS.
    ///
    /// Generate PCH IO TRAP SMI to register IchnBiosWp callback function in
    /// PchBiosLockIoTrapCallback() to handle TCO BIOSWR SMI
    ///
    if ((PchPlatformPolicy->LockDownConfig->BiosLock == PCH_DEVICE_ENABLE)) {
      DEBUG (
        (DEBUG_ERROR,
        "PchBiosLockIoTrapAddress = 0x%x\n",
        PchPlatformPolicy->LockDownConfig->PchBiosLockIoTrapAddress)
        );

      if (PchPlatformPolicy->LockDownConfig->PchBiosLockIoTrapAddress != 0) {
        ///
        /// Write PCH_BWP_SIGNATURE to IoTrap Address
        ///
        IoWrite32 (PchPlatformPolicy->LockDownConfig->PchBiosLockIoTrapAddress, PCH_BWP_SIGNATURE);
        S3BootScriptSaveMemWrite (
          S3BootScriptWidthUint8,
          (UINTN) (PciD31F0RegBase + R_PCH_LPC_BIOS_CNTL),
          1,
          (VOID *) (UINTN) (PciD31F0RegBase + R_PCH_LPC_BIOS_CNTL)
          );
      }
    }
    ///
    /// Lock Down TCO
    ///
    Data16And = 0xFFFF;
    Data16Or = B_PCH_TCO_CNT_LOCK;
    IoOr16(PmBase + PCH_TCO_BASE + R_PCH_TCO1_CNT, Data16Or);
    S3BootScriptSaveIoReadWrite (
      S3BootScriptWidthUint16,
      (UINTN) (PmBase + PCH_TCO_BASE + R_PCH_TCO1_CNT),
      &Data16Or,  // Data to be ORed
      &Data16And  // Data to be ANDed
      );
    ///
    /// PCH BIOS Spec Rev 0.5.0, Section 5.15.1 Additional Chipset Initialization
    /// Step 1
    /// Set SPIBAR + F0h [0] to 1b
    ///
    MmioOr8 ((UINTN) (RootComplexBar + R_PCH_SPI_SRDL), B_PCH_SPI_SRDL_SSL);
    ///
    /// Check to disable Smbus controller
    ///
    if (PchPlatformPolicy->DeviceEnabling->Smbus == PCH_DEVICE_DISABLE) {
      MmioOr32 ((UINTN) (RootComplexBar + R_PCH_RCRB_FUNC_DIS), (UINT32) B_PCH_RCRB_FUNC_DIS_SMBUS);
      S3BootScriptSaveMemWrite_MMIO (
        S3BootScriptWidthUint32,
        (UINTN) (RootComplexBar + R_PCH_RCRB_FUNC_DIS),
        1,
        (VOID *) (UINTN) (RootComplexBar + R_PCH_RCRB_FUNC_DIS)
        );
      ///
      /// Reads back for posted write to take effect
      ///
      Data32Or = MmioRead32 ((UINTN) (RootComplexBar + R_PCH_RCRB_FUNC_DIS));
      S3BootScriptSaveMemReadWrite (
        S3BootScriptWidthUint32,
        (UINTN) (RootComplexBar + R_PCH_RCRB_FUNC_DIS),
        &Data32Or,  /// BitMask
        &Data32Or  /// BitValue
        );
    }

    UsbInitBeforeBoot (PchPlatformPolicy);
    FoundLegacyRaid = FALSE;
    ///
    /// Get all PCI IO protocols
    ///
    Status = gBS->LocateHandleBuffer (
                    ByProtocol,
                    &gEfiPciIoProtocolGuid,
                    NULL,
                    &NumHandles,
                    &HandleBuffer
                    );
    if (!EFI_ERROR (Status)) {
      ///
      /// Find the RAID BIOS by checking each PCI IO handle for RST OPROM
      ///
      for (Index = 0; Index < NumHandles; Index++) {
        Status = gBS->HandleProtocol (
                        HandleBuffer[Index],
                        &gEfiPciIoProtocolGuid,
                        (VOID **) &PciIo
                        );
        if (EFI_ERROR (Status) || (PciIo->RomImage == NULL)) {
          ///
          /// If this PCI device doesn't have a ROM image, skip to the next device.
          ///
          continue;
        }

        RomImage = PciIo->RomImage;

        ///
        /// Get pointer to PCIR structure
        ///
        PcirBlockPtr = (PCI_DATA_STRUCTURE *) ((UINTN) RomImage + RomImage->PcirOffset);

        ///
        /// Check if we have an RAID BIOS OPROM.
        ///
        if ((RomImage->Signature == 0xAA55) &&
           (PcirBlockPtr->ClassCode[0] == 0x00) &&
           (PcirBlockPtr->ClassCode[1] == 0x04) &&
           (PcirBlockPtr->ClassCode[2] == 0x01)
          ) {
          mPchFviElementsData[RAID_VER].Element.Version.MajorVersion = (UINT8) ((PcirBlockPtr->CodeRevision & 0xFF00) >> 8);
          mPchFviElementsData[RAID_VER].Element.Version.MinorVersion = (UINT8)  (PcirBlockPtr->CodeRevision & 0x00FF);
          mPchFviElementsData[RAID_VER].Element.Version.Revision     = 0;
          mPchFviElementsData[RAID_VER].Element.Version.BuildNum     = 0;
          FoundLegacyRaid = TRUE;
        }
      }
    }
    ///
    /// Search EFI RST OPROM
    ///
    if (FoundLegacyRaid == FALSE) {
      Status = gBS->LocateHandleBuffer (
                      ByProtocol,
                      &gEfiDriverSupportedEfiVersionProtocolGuid,
                      NULL,
                      &NumHandles,
                      &HandleBuffer
                      );
      if (!EFI_ERROR (Status)) {
        for (Index = 0; Index < NumHandles; Index++) {
          Status = gBS->HandleProtocol (
                          HandleBuffer[Index],
                          &gEfiComponentName2ProtocolGuid,
                          (VOID **) &ComponentName2
                          );
          if (EFI_ERROR(Status)) {
            continue;
          }

          Status = ComponentName2->GetDriverName (ComponentName2, "en-US", &DriverName);
          if (EFI_ERROR(Status)) {
            continue;
          }

          if ((StrnCmp (DriverName, RstDriverName1, StrLen (RstDriverName1) == 0)) ||
              (StrnCmp (DriverName, RstDriverName2, StrLen (RstDriverName2) == 0))) {
            Status = gBS->HandleProtocol(
                            HandleBuffer[Index],
                            &gEfiDriverSupportedEfiVersionProtocolGuid,
                            (VOID**)&DriverEfiVersion);
            mPchFviElementsData[RAID_VER].Element.Version.MajorVersion = (UINT8) ((DriverEfiVersion->FirmwareVersion & 0x00FF0000) >> 16);
            mPchFviElementsData[RAID_VER].Element.Version.MinorVersion = (UINT8)  (DriverEfiVersion->FirmwareVersion & 0x000000FF);
            mPchFviElementsData[RAID_VER].Element.Version.Revision     = 0;
            mPchFviElementsData[RAID_VER].Element.Version.BuildNum     = 0;
          }
        }
      }
    }

      //
      // Step 4.9
      // Set RCBA + Offset 50h[31] = 1b
      // Lock down the TC mapping if no further changes are required to bits [30:16]
      //
      MmioOr32 (RootComplexBar + R_PCH_RCRB_CIR0, B_PCH_RCRB_CIR0_TCLOCKDN);
      ///
      /// Reads back for posted write to take effect
      ///
      MmioRead32 (RootComplexBar + R_PCH_RCRB_CIR0);

    if (PchSeries == PchLp) {
#ifdef SERIAL_IO_FLAG
        ConfigureSerialIoAtBoot(PchPlatformPolicy, RootComplexBar);
#endif // SERIAL_IO_FLAG
#ifdef ADSP_FLAG
      ConfigureAudioDspAtBoot (PchPlatformPolicy, RootComplexBar);
#endif // ADSP_FLAG
    }
    //
    // Create RC FVI data hubs
    //
    CreateRcFviDatahub (&mPchFviVersionData);
  }

#ifdef EFI_S3_RESUME
  if (!PchS3Support) {
    DEBUG ((EFI_D_INFO, "Locating the S3 Support Protocol - PCH Init before Boot\n"));

    ///
    /// Get the PCH S3 Support Protocol
    ///
    Status = gBS->LocateProtocol (
                    &gEfiPchS3SupportProtocolGuid,
                    NULL,
                    (VOID **) &PchS3Support
                    );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return;
    }

    Status = PchS3Support->ReadyToLock(PchS3Support);
    if (EFI_ERROR (Status)) {
      return;
    }
  }
#endif

  DEBUG ((DEBUG_INFO, "PchInitBeforeBoot() End\n"));

  return;
}
