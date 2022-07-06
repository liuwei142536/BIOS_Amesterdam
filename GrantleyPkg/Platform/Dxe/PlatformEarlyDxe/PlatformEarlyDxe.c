/*++

Copyright (c)  1999 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PlatformEarlyDxe.c


--*/

#include "PlatformEarlyDxe.h"

#define MmPciAddress(Bus, Device, Function, Register) \
      ( \
        (UINTN) PcdGet64(PcdPciExpressBaseAddress) + (UINTN) (Bus << 20) + (UINTN) (Device << 15) + (UINTN) \
          (Function << 12) + Register \
      )
extern EFI_GUID gSignalBeforeEnterSetupGuid;

VOID
ProgramDPRregs (
  VOID
  )
/*++

Routine Description:

  This function will retrieve the DPR data from HOBs produced by MRC
  and will use it to program the DPR registers

Arguments:

  VOID

Returns:

  VOID

--*/
{
  EFI_STATUS                      Status;
  EFI_PLATFORM_TXT_DEVICE_MEMORY  *PlatformTxtMemory;
  EFI_HOB_GUID_TYPE               *GuidHob      = NULL;
  VOID                            *HobData      = NULL;
  EFI_HANDLE                      Handle        = NULL; 
  volatile UINT64                 *Ptr64;
  UINT64                          Value64;
  UINT32                          Value32;
  UINTN                           PciAddress;
  UINT8                           BusNum = 0;
  
  //
  // Get Txt Device Memory HOB if it was published
  //
  GuidHob = GetFirstGuidHob (&gEfiPlatformTxtDeviceMemoryGuid);
  
  if (GuidHob != NULL) {
    
    HobData = GET_GUID_HOB_DATA (GuidHob);
    PlatformTxtMemory = (EFI_PLATFORM_TXT_DEVICE_MEMORY*)HobData;
    
    PciAddress = MmPciAddress(BusNum, IIO_VTD_DEV, IIO_VTD_FUN, SA_DPR_REG);
    Value32 = *(UINT32*)PciAddress;

    //
    // Program DPR Registers if not programmed already
    //
    if (PlatformTxtMemory->DMAProtectionMemoryRegionAddress != 0 && ((Value32 & 0x0FFF) == 0)) {

      //
      // Program the DPR registers on all sockets.
      //
      while(1) {
      
        Value32 = ((UINT32)(PlatformTxtMemory->DMAProtectionMemoryRegionSize) >> 16) | SA_DPR_LOCK | SA_DPR_PRM;
        
        //
        // Write DPR reg of next IIO
        //
        PciAddress = MmPciAddress(BusNum, IIO_VTD_DEV, IIO_VTD_FUN, SA_DPR_REG);
        *(UINT32*)PciAddress = Value32;

        S3BootScriptSaveMemWrite (
          S3BootScriptWidthUint32,
          (UINTN)MmPciAddress (BusNum, IIO_VTD_DEV, IIO_VTD_FUN, SA_DPR_REG),
          1,
          &Value32
        );
        
        //
        // Find out Bus# of next IIO, Uncore Bus# + 1
        //
        PciAddress = MmPciAddress(BusNum, IIO_VTD_DEV, IIO_VTD_FUN, R_IIO_CPUBUSNO);
        Value32 = *(UINT32*)PciAddress;
        
        BusNum = ((UINT8)((Value32 >> 8) & 0x0ff)) + 1;
        
        //
        // If we have reached the end of the sockets, exit loop
        //
        if (BusNum == 0)
          break;
      
      }

      //
      // Read back the register programmed to display in debug output
      //
      PciAddress = MmPciAddress(0, IIO_VTD_DEV, IIO_VTD_FUN, SA_DPR_REG);
      Value32 = *(UINT32*)PciAddress;

      DEBUG ((EFI_D_ERROR, "DPR Register [%08x] = %08x! on bus 0\n", (UINTN)MmPciAddress(0, IIO_VTD_DEV, IIO_VTD_FUN, SA_DPR_REG), Value32));

      //
      // Need to double check DPR REG
      //
      if (((Value32 & 0xFF0) << 16) != PlatformTxtMemory->DMAProtectionMemoryRegionSize){
        DEBUG ((EFI_D_ERROR, "ProgramDPRregs SA_DPR_REG[0x%02x] = 0x%08x not set correctly!!! \n", SA_DPR_REG, Value32));
      }

      //
      // DPR registers
      //
      Value32 &= 0x0FFF00000;
      Ptr64   = (UINT64 *) (UINTN) (LT_PUBLIC_SPACE_BASE_ADDRESS + LT_DMA_PROTECTED_RANGE_REG_OFF);
      Value64 = RShiftU64 (PlatformTxtMemory->DMAProtectionMemoryRegionSize, 16) | SA_DPR_LOCK | SA_DPR_PRM;

      //
      // Save DPR register value to LT DPR Reg offset
      //
      *Ptr64  = Value64 | Value32;
      Value64 = *Ptr64;

      //
      // Save for S3 resume if LT DPR Reg was programmed correctly.
      //      
      if ((LShiftU64 ((Value64 & 0xFFE), 16)) == PlatformTxtMemory->DMAProtectionMemoryRegionSize) {

        S3BootScriptSaveMemWrite (
          S3BootScriptWidthUint32,
          (UINT64) (UINTN) (Ptr64),
          2,
          &Value64
          );
      }

      //
      // Clear out the DMA protected memory area for use.
      //
      ZeroMem (
        (VOID *)(UINTN)PlatformTxtMemory->DMAProtectionMemoryRegionAddress, 
        (UINTN)PlatformTxtMemory->DMAProtectionMemoryRegionSize
        );
    
    }

  } else {
      DEBUG((EFI_D_INFO, "ProgramDPRregs: Txt Device Memory HOB not found.  DPR regs not programmed.\n"));
  }
    
  //
  // Install dummy protocol to signal DPR regs are programmed.
  //
  Status = gBS->InstallProtocolInterface (
                &Handle,
                &gEfiDprRegsProgrammedGuid,
                EFI_NATIVE_INTERFACE,
                NULL
                );

  return;
}

VOID
ProgramGenProtRangeRegs(
  VOID
  )
/*++

Routine Description:

  This function will program the legacy range and RCBA range into the
  General Protected Range registers.

Arguments:

  VOID

Returns:

  VOID

--*/
{

  UINT32              Data32;
  UINT32              Value32;
  UINTN               PciAddress;
  UINT8               BusNum = 0;
  
  while(1) {
    //
    //  Legacy address ranges need to be protected from inbound memory reads to prevent CTOs.
    // (3614978, 3876023, 3247123, 3876024, 3876008)
	// Perform DWORD Access to PCI configuration registers 
    Data32 = 0x0A0000;   // Base address of PAM region [31:16], [15:0] assumed zero
    PciAddress = MmPciAddress(BusNum, IIO_VTD_DEV, IIO_VTD_FUN, R_IIO_GENPROTRANGE2Base0);
    *(UINT32*)PciAddress = Data32;
    PciAddress = MmPciAddress(BusNum, IIO_VTD_DEV, IIO_VTD_FUN, R_IIO_GENPROTRANGE2Base1);
    *(UINT32*)PciAddress = 0x00;
    Data32 = 0x0F0000;   // Limit address of PAM region [31:16], [15:0] assumed one
    PciAddress = MmPciAddress(BusNum, IIO_VTD_DEV, IIO_VTD_FUN, R_IIO_GENPROTRANGE2Limit0);
    *(UINT32*)PciAddress = Data32;
	PciAddress = MmPciAddress(BusNum, IIO_VTD_DEV, IIO_VTD_FUN, R_IIO_GENPROTRANGE2Limit1);
	*(UINT32*)PciAddress = 0x00;

    //
    // HSD 4167093: TXT BIOS Must Program GenProtect Range to Overlap RCBA
    // Cover the RCBA address space
    //
    Data32 = PCH_RCBA_ADDRESS & 0xFFFF0000;   // Base address of RCBA region overlaps at 0xFED1C000
    PciAddress = MmPciAddress(BusNum, IIO_VTD_DEV, IIO_VTD_FUN, R_IIO_GENPROTRANGE1Base0);
    *(UINT32*)PciAddress = Data32;
	PciAddress = MmPciAddress(BusNum, IIO_VTD_DEV, IIO_VTD_FUN, R_IIO_GENPROTRANGE1Base1);
	*(UINT32*)PciAddress = 0x00;

    Data32 = (PCH_RCBA_ADDRESS + 0x4000) & 0xFFFF0000;   // End of RCBA Base address 0xFED20000
    PciAddress = MmPciAddress(BusNum, IIO_VTD_DEV, IIO_VTD_FUN, R_IIO_GENPROTRANGE1Limit0);
    *(UINT32*)PciAddress = Data32;
	PciAddress = MmPciAddress(BusNum, IIO_VTD_DEV, IIO_VTD_FUN, R_IIO_GENPROTRANGE1Limit1);
	*(UINT32*)PciAddress = 0x00;
    
    //
    // Find out Bus# of next IIO, Uncore Bus# + 1
    //
    PciAddress = MmPciAddress(BusNum, IIO_VTD_DEV, IIO_VTD_FUN, R_IIO_CPUBUSNO);
    Value32 = *(UINT32*)PciAddress;
    
    BusNum = ((UINT8)((Value32 >> 8) & 0x0ff)) + 1;
    
    //
    // If we have reached the end of the sockets, exit loop
    //
    if (BusNum == 0)
      break;
  }

}

VOID
SetBiosInfoFlagWpe (
  VOID
  )
/*++

Routine Description:

  Sets the WPE bit of the BIOS Info Flags MSR to enable Anti-Flash wearout protection
  within BIOS Guard

Arguments:

  None
  
Returns: 

  None

--*/
{
  AsmWriteMsr64 (MSR_BIOS_INFO_FLAGS, B_MSR_BIOS_INFO_FLAGS_WPE);

  return ;
}

VOID
EFIAPI
EnableAntiFlashWearout (
      EFI_EVENT                 Event,
      VOID                      *Context
  )
/*++

Routine Description:

  Function to set the WPE bit of the BIOS Info Flags MSR to enable Anti-Flash wearout 
  protection within BIOS Guard before booting to the OS

Arguments:

  IN EFI_EVENT         Event
  IN VOID              *Context
  
Returns: 

  None

--*/
{
  EFI_STATUS                    Status;
  EFI_MP_SERVICES_PROTOCOL      *MpServices = NULL;
  UINTN                         VariableSize;
  SYSTEM_CONFIGURATION          SetupData;
  
  VariableSize = sizeof(SYSTEM_CONFIGURATION);
  Status = gRT->GetVariable (
                L"Setup",
                &gEfiSetupVariableGuid,
                NULL,
                &VariableSize,
                &SetupData
                );
  
  //
  // First check if Anti-flash wearout feature is supported by platform and Setup variable is enabled
  //
  if (SetupData.AntiFlashWearoutSupported == TRUE && SetupData.EnableAntiFlashWearout) {

    Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  (VOID **) &MpServices
                  );
    ASSERT_EFI_ERROR(Status);
    
    //
    // Set WPE on BSP, then all other APs
    //
    SetBiosInfoFlagWpe();
    
    MpServices->StartupAllAPs (
                    MpServices,
                    (EFI_AP_PROCEDURE) SetBiosInfoFlagWpe,
                    FALSE,
                    NULL,
                    0,
                    NULL,
                    NULL
                    );        
  }

}

EFI_STATUS
EFIAPI
PlatformEarlyDxeEntry ( 
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  ) 
/*++

Routine Description:

  This is the main entry point of the Platform Early DXE module.

Arguments:

  ImageHandle  -  Handle for the image of this driver.
  SystemTable  -  Pointer to the EFI System Table.

Returns:

  EFI_SUCCESS  -  Module launched successfully.

--*/
{
  EFI_STATUS            Status = EFI_SUCCESS;
  UINT32                BspCpuidSignature;
  UINT32                RegEax, RegEbx, RegEcx, RegEdx;
  EFI_EVENT             AntiFlashEvent;
  VOID                  *AntiFlashRegistration;

  //
  // Program DPR registers with the range from Memory Init
  //
  ProgramDPRregs();
  
  //
  // Program the GenProtRange registers for PFAT
  //
  ProgramGenProtRangeRegs();

  //
  // Get BSP CPU ID
  // Shift out the stepping
  //
  AsmCpuid (0x01, &RegEax, &RegEbx, &RegEcx, &RegEdx);
  BspCpuidSignature = (RegEax >> 4) & 0x0000FFFF;
  if ( (BspCpuidSignature == CPU_FAMILY_BDX)) {

    ///
    /// Create a call back event to set WPE bit in BIOS_INFO_FLAGS MSR to enable Anti Flash wearout feature.
    ///
    Status = gBS->CreateEvent (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    EnableAntiFlashWearout,
                    NULL,
                    &AntiFlashEvent
                    );
    if (!EFI_ERROR(Status)) {
      //
      // Register protocol notifications on this event.
      //
      Status = gBS->RegisterProtocolNotify (
                    &gSignalBeforeEnterSetupGuid,
                    AntiFlashEvent,
                    &AntiFlashRegistration
                    );
    }
  }
  return Status;
  
}
