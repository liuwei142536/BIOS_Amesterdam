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

    LtDxeLib.c

  Abstract:

    This file contains an implementation of the function call interfaces
    required by the main LT DXE file.  Hopefully, future platform porting
    tasks will be mostly limited to modifying the functions in this file.

--*/


#include "TxtDxeLib.h"
#include "Common\LaunchBiosAcm.h"
#ifdef DYNAMIC_MMCFG_BASE_FLAG //AptioV server override: dynamic MMCFG base support
#include <PchAccess.h> //AptioV server override: dynamic MMCFG base support
#endif
#include <Guid/SetupVariable.h>

//AptioV Server Override Start: Changes done to load SINIT binary from FV_MAIN, to avoid separate FD AREA
#define AMI_LTSX_SINIT_GUID \
	{0x8bd816b2, 0x655f, 0x4c71, 0x90, 0x32, 0xc9, 0x0d, 0x72, 0xc0, 0x7c, 0x03}

EFI_GUID gAmiLtsxSinitGuid          = AMI_LTSX_SINIT_GUID;
//AptioV Server Override End: Changes done to load SINIT binary from FV_MAIN, to avoid separate FD AREA

VOID
SendIpi (
  IN UINT32          IcrLow,
  IN UINT32          ApicId
  );

VOID
EFIAPI
SendInitIpiAllExcludingSelf (
  VOID
  );

VOID
EFIAPI
SendInitSipiSipiAllExcludingSelf (
  IN UINT32          StartupRoutine
  );
UINT8
EFIAPI
IoRead8 (
  IN      UINTN                     Port
  );

UINT8
EFIAPI
IoWrite8 (
  IN      UINTN                     Port,
  IN      UINT8                     Value
  );

//AptioV Server Override Start: Already Defined in Token.h
//#define PCH_ACPI_BASE_ADDRESS           0x0400
//#define PM_BASE_ADDRESS PCH_ACPI_BASE_ADDRESS
//AptioV Server Override End: Already Defined in Token.h

#define R_PCH_SMI_EN                              0x30
#define B_PCH_SMI_EN_SWSMI_TMR                    BIT6

//
// LT Configuration Space Register Definitions
//
#define LT_CONFIG_SPACE_BASE_ADDRESS        0xfed20000

#define LT_PUBLIC_SPACE_BASE_ADDRESS        0xfed30000
#define LT_ESTS_OFF                         0x8
#define   LT_TXT_RESET_STS                  0x1
#define LT_NODMA_BASE_REG_OFF               0x260
#define LT_NODMA_SIZE_REG_OFF               0x268
#define LT_SINIT_BASE_REG_OFF               0x270
#define LT_SINIT_SIZE_REG_OFF               0x278
#define LT_HEAP_BASE_REG_OFF                0x300
#define LT_HEAP_SIZE_REG_OFF                0x308
#define LT_DMA_PROTECTED_RANGE_REG_OFF      0x330

#define LT_TPM_SPACE_BASE_ADDRESS           0xfed40000
#define LT_TPM_SPACE_ACCESS0                0x0

#define B_GBL_SMI_EN  1

#define CPUID_SMX_BIT                   (1 << 6)

#define FEATURE_LOCK_BIT                0x0001
#define FEATURE_VMON_IN_SMX             0x0002
#define FEATURE_VMON_OUT_SMX            0x0004
#define FEATURE_SENTER_ENABLE           0xFF00

#ifndef DELIVERY_MODE_INIT
#define DELIVERY_MODE_INIT                    0x05
#endif
#ifndef DELIVERY_MODE_SIPI
#define DELIVERY_MODE_SIPI                    0x06
#endif

#define ONE_MEGABYTE                        0x100000

#define ACMOD_SIZE                      24

#ifndef EFI_CPUID_CORE_TOPOLOGY
#define EFI_CPUID_CORE_TOPOLOGY     0x0B
#endif

#define CMOS_LTSX_OFFSET             LTSX_TXT_CMOS_ADDRESS /* Aptio Server Override use LTSX_TXT_CMOS_ADDRESS token value0x2A*/

//
// Ap init function
//
VOID
DoApInit (
  VOID
  );

VOID
ApSaveConfig (
  VOID
  );

VOID
ApRestoreConfig (
  VOID
  );

#define  IA32_MTRR_PHYSBASE0  0x200
#define  IA32_MTRR_PHYSMASK7  0x20F 

UINTN   mAcmBase;
UINT64  mApIdt[2];
UINTN   mApCr4;
UINT64  mApMtrrTab[2*(IA32_MTRR_PHYSMASK7 - IA32_MTRR_PHYSBASE0 + 1) + 1];

//
// LtDxeLibLaunchBiosAcm is an internal routine used to
// wrap the BIOS ACM initialization code for the various
// functions inside of it.
//
STATIC
EFI_STATUS
LtDxeLibLaunchBiosAcm (
  IN LT_DXE_LIB_CONTEXT      *LtDxeCtx,
  IN UINT32                  AcmFunction
  );

STATIC
EFI_STATUS
GetApVector (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );


// Aptio V Server Override Start : TXT Error and status Register message printing.
VOID PrintLtsxErrors(
)
{
  UINT32                      Data32;
	
  Data32 = *(volatile UINT32 *)(UINTN)TXT_ACM_STATUS_LO;
  DEBUG ((EFI_D_ERROR, "TXT_ACM_STATUS_LO (0xFED300A0): %X\n", Data32));
	
  Data32 = *(volatile UINT32 *)(UINTN)TXT_ACM_STATUS_HI;
  DEBUG ((EFI_D_ERROR, "TXT_ACM_STATUS_HI (0xFED300A4): %X\n", Data32));
	
  Data32 = *(volatile UINT32 *)(UINTN)TXT_ACM_ERRORCODE;
  DEBUG ((EFI_D_ERROR, "TXT_ACM_ERRORCODE (0xFED30030): %X\n", Data32));
	
  Data32 = *(volatile UINT32 *)(UINTN)TXT_BIOS_ACM_ERRORCODE;
  DEBUG ((EFI_D_ERROR, "TXT_BIOS_ACM_ERRORCODE (0xFED30328): %X\n", Data32));
}

// Aptio V Server Override End : TXT Error and status Register message printing.

EFI_STATUS
InitializeLtDxeLib(
  IN      EFI_HANDLE              ImageHandle,
  IN      EFI_SYSTEM_TABLE        *SystemTable,
  IN OUT  LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
/*++

Routine Description:

  This routine initializes and collects all Protocols and data required
  by the routines in this file.

Arguments:

  ImageHandle   - A pointer to the Image Handle for this file.
  SystemTable   - A pointer to the EFI System Table
  LtDxeCtx      - A pointer to a caller allocated data structure that contains
                  all of the Protocols and data required by the routines
                  in this file.

Returns:

    EFI_SUCCESS     - Always.

--*/
{
  EFI_STATUS                Status;
  UINTN                     CpuCount;
  UINTN                     CpusEnabled;
  UINTN                     CpuIndex;
  EFI_PROCESSOR_INFORMATION ProcInfo;
  EFI_HOB_GUID_TYPE         *GuidHob;
  VOID                      *DataInHob;
//APTIOV_SERVER_OVERRIDE_START: System lock shouldn't be configurable on release BIOSes
#if 0
  UINTN                     SysCfgSize;
  SYSTEM_CONFIGURATION      SysCfg;
#endif
//APTIOV_SERVER_OVERRIDE_END: System lock shouldn't be configurable on release BIOSes

  //Aptio V Server Override - Move disabling periodic timer before ACM launch and restore original value afterwards.
  //Aptio V Server Override - UINT8 Buffer8 = IoRead8(PM_BASE_ADDRESS+R_PCH_SMI_EN) & ~(B_PCH_SMI_EN_SWSMI_TMR);
  //Aptio V Server Override - IoWrite8(PM_BASE_ADDRESS+R_PCH_SMI_EN, Buffer8);  // disable Software SMI Timer

  LtDxeCtx->ImageHandle = ImageHandle;
  LtDxeCtx->SystemTable = SystemTable;

//APTIOV_SERVER_OVERRIDE_START: System lock shouldn't be configurable on release BIOSes
#if 0
  SysCfgSize = sizeof (SysCfg);
  Status = gRT->GetVariable (
                  L"IntelSetup",	//Aptio V Server Override
                  &gEfiSetupVariableGuid,
                  NULL, // Attributes
                  &SysCfgSize,
                  &SysCfg
                  );
#endif
//APTIOV_SERVER_OVERRIDE_END: System lock shouldn't be configurable on release BIOSes
  //
  // Find the CpuIo protocol
  //
  Status = gBS->LocateProtocol (
                  &gEfiCpuIoProtocolGuid,
                  NULL,
                  &(LtDxeCtx->CpuIo)
                  );
  ASSERT_EFI_ERROR (Status) ;

  //
  // Find the MpService Protocol to use with LT Config lock
  //
  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  &(LtDxeCtx->MpService)
                  );
  ASSERT_EFI_ERROR (Status);  
                  
  if (EFI_ERROR(Status)) {
     return Status;
  }
//APTIOV_SERVER_OVERRIDE_START: System lock shouldn't be configurable on release BIOSes
#if 0
 DEBUG((EFI_D_ERROR, "SysCfg.ProcessorLtsxEnable: %02x\n", SysCfg.ProcessorLtsxEnable));
  if (SysCfg.ProcessorLtsxEnable == 0) {
#endif
  if (!PcdGetBool(PcdProcessorLtsxEnable)) {
//APTIOV_SERVER_OVERRIDE_END: System lock shouldn't be configurable on release BIOSes
    Status = EFI_UNSUPPORTED;
    return Status;
  }

  //
  // Find the PlatformTxtDeviceMemory hob
  //
  GuidHob = GetFirstGuidHob (&gEfiPlatformTxtDeviceMemoryGuid);
  if  (GuidHob == NULL){
    DEBUG ((EFI_D_ERROR, "gEfiPlatformTxtDeviceMemoryGuid not found! TxtDxe return error!\n"));
    return EFI_NOT_FOUND;
  }
  DataInHob = GET_GUID_HOB_DATA (GuidHob);
  LtDxeCtx->PlatformTxtDeviceMemory = (EFI_PLATFORM_TXT_DEVICE_MEMORY*)DataInHob;

  //
  // Find the PlatformTxtPolicyData hob
  //
  GuidHob = GetFirstGuidHob (&gEfiPlatformTxtPolicyDataGuid);
  if  (GuidHob == NULL){
    DEBUG ((EFI_D_ERROR, "gEfiPlatformTxtPolicyDataGuid not found! TxtDxe return error!\n"));
    return EFI_NOT_FOUND;
  }
  DataInHob = GET_GUID_HOB_DATA (GuidHob);
  LtDxeCtx->PlatformTxtPolicyData = (EFI_PLATFORM_TXT_POLICY_DATA*)DataInHob;
  
  DEBUG ((EFI_D_ERROR, "PlatformTxtDeviceMemory:\n"));
  DEBUG ((EFI_D_ERROR, "  NoDMATableAddress                - 0x%08x\n", LtDxeCtx->PlatformTxtDeviceMemory->NoDMATableAddress));
  DEBUG ((EFI_D_ERROR, "  NoDMATableSize                   - 0x%08x\n", LtDxeCtx->PlatformTxtDeviceMemory->NoDMATableSize));
  DEBUG ((EFI_D_ERROR, "  SINITMemoryAddress               - 0x%08x\n", LtDxeCtx->PlatformTxtDeviceMemory->SINITMemoryAddress));
  DEBUG ((EFI_D_ERROR, "  SINITMemorySize                  - 0x%08x\n", LtDxeCtx->PlatformTxtDeviceMemory->SINITMemorySize));
  DEBUG ((EFI_D_ERROR, "  TXTHeapMemoryAddress             - 0x%08x\n", LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemoryAddress));
  DEBUG ((EFI_D_ERROR, "  TXTHeapMemorySize                - 0x%08x\n", LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemorySize));
  DEBUG ((EFI_D_ERROR, "  DMAProtectionMemoryRegionAddress - 0x%08x\n", LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionAddress));
  DEBUG ((EFI_D_ERROR, "  DMAProtectionMemoryRegionSize    - 0x%08x\n", LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionSize));
  DEBUG ((EFI_D_ERROR, "PlatformTxtPolicy:\n"));
  DEBUG ((EFI_D_ERROR, "  BiosAcmAddress             - 0x%08x\n", LtDxeCtx->PlatformTxtPolicyData->BiosAcmAddress));
  DEBUG ((EFI_D_ERROR, "  StartupAcmAddressInFit     - 0x%08x\n", LtDxeCtx->PlatformTxtPolicyData->StartupAcmAddressInFit));
  DEBUG ((EFI_D_ERROR, "  BiosOsDataRegionRevision   - 0x%08x\n", LtDxeCtx->PlatformTxtPolicyData->BiosOsDataRegionRevision));
  DEBUG ((EFI_D_ERROR, "  LcpPolicyDataBase          - 0x%08x\n", LtDxeCtx->PlatformTxtPolicyData->LcpPolicyDataBase));
  DEBUG ((EFI_D_ERROR, "  LcpPolicyDataSize          - 0x%08x\n", LtDxeCtx->PlatformTxtPolicyData->LcpPolicyDataSize));
  DEBUG ((EFI_D_ERROR, "  TxtScratchAddress          - 0x%08x\n", LtDxeCtx->PlatformTxtPolicyData->TxtScratchAddress));
  DEBUG ((EFI_D_ERROR, "  BiosAcmPolicy              - 0x%08x\n", LtDxeCtx->PlatformTxtPolicyData->BiosAcmPolicy));

  if (LtDxeCtx->PlatformTxtPolicyData->BiosAcmAddress == 0){
    Status = EFI_UNSUPPORTED;
    return Status;
  }
  //
  // Get CPU Information
  //
  Status = LtDxeCtx->MpService->GetNumberOfProcessors (
                           LtDxeCtx->MpService,
                           &CpuCount,
                           &CpusEnabled
                           );
  ASSERT_EFI_ERROR (Status);
  LtDxeCtx->ApCount = (UINT8)(CpuCount - 1);
  DEBUG ((EFI_D_ERROR, "ApCount - 0x%08x\n", LtDxeCtx->ApCount));

  if (LtDxeCtx->ApCount > 0) {
    Status = GetApVector (LtDxeCtx);
    ASSERT_EFI_ERROR (Status);
    DEBUG ((EFI_D_ERROR, "ApVector - 0x%08x\n", LtDxeCtx->ApVector));

    //
    // Get All processor APIC ID info
    //
    for (CpuIndex = 0; CpuIndex < CpuCount; CpuIndex ++){
      Status = LtDxeCtx->MpService->GetProcessorInfo (
                                        LtDxeCtx->MpService,
                                        CpuIndex,
                                        &ProcInfo);
      ASSERT_EFI_ERROR (Status);
      LtDxeCtx->ApicId[CpuIndex] = (UINT8)ProcInfo.ProcessorId;
      DEBUG ((EFI_D_ERROR, "TXT-LIB APIC[%d] = 0x%08x\n", CpuIndex, LtDxeCtx->ApicId[CpuIndex]));
    }
  }

  return EFI_SUCCESS ;
}

BOOLEAN
IsLtProcessor (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
/*++

Routine Description:

  Determines whether or not the current processor is LT Capable.

Arguments:

  LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

Returns:

  TRUE          - If the current processor supports LT
  FALSE         - If the current processor does not support LT

--*/
{
  EFI_CPUID_REGISTER      CpuidRegs;

  AsmCpuid (EFI_CPUID_VERSION_INFO, &CpuidRegs.RegEax, &CpuidRegs.RegEbx, &CpuidRegs.RegEcx, &CpuidRegs.RegEdx);
  return (CpuidRegs.RegEcx & CPUID_SMX_BIT) ? TRUE : FALSE;
}

BOOLEAN
IsLtEnabled (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
/*++

Routine Description:

  Determines whether or not the platform requires initialization for LT use.

Arguments:

  LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

Returns:

  TRUE          - If the the platoform should be configured for LT.
  FALSE         - If LT is not to be used.

--*/
{
  UINT64  Ia32FeatureControl;

  Ia32FeatureControl = AsmReadMsr64(EFI_MSR_IA32_FEATURE_CONTROL);
  if (((Ia32FeatureControl & FEATURE_SENTER_ENABLE) != FEATURE_SENTER_ENABLE) ||
      ((Ia32FeatureControl & FEATURE_VMON_IN_SMX)   != FEATURE_VMON_IN_SMX) ) {
    return FALSE;
  } else {
    return TRUE;
  }
}

BOOLEAN
IsLtEstablished (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
/*++

Routine Description:

  Determines whether or not the platform has executed an LT launch by
  examining the TPM Establishment bit.

Arguments:

  LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

Returns:

  TRUE          - If the TPM establishment bit is asserted.
  FALSE         - If the TPM establishment bit is unasserted.

--*/
{
#ifdef MKF_TPM_PRESENT
  EFI_STATUS              Status;
  UINT8                   Access;

  //
  // Read TPM status register
  //
  Status = LtDxeCtx->CpuIo->Mem.Read(
                                  LtDxeCtx->CpuIo,
                                  EfiCpuIoWidthUint8,
                                  (UINT64)(LT_TPM_SPACE_BASE_ADDRESS + LT_TPM_SPACE_ACCESS0),
                                  1,
                                  &Access
                                  );
  ASSERT_EFI_ERROR (Status);

  //
  // The bit we're interested in uses negative logic:
  // If bit 0 == 1 then return False
  // Else return True
  //
  return (Access & 0x1) ? FALSE : TRUE;
#else
  return TRUE;
#endif        
}

BOOLEAN
IsTpmPresent (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
/*++

Routine Description:

  Determines presence of TPM in system

Arguments:

  LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

Returns:

  TRUE          - If the TPM is present.
  FALSE         - If the TPM is not present.

--*/
{
/*              Replaced next block with line below. might need to check setup option rather than actaul TPM presence. 
                TPM existance and/or functionality is not essential for DPR programming (which is part of LT code), 
                nor TPM functionality is needed for LT functionality (accroding to the calling function)

  EFI_STATUS              Status;
  UINT8                   Access;

  //
  // Read TPM status register
  //
  Status = LtDxeCtx->CpuIo->Mem.Read (
                                  LtDxeCtx->CpuIo,
                                  EfiCpuIoWidthUint8,
                                  (UINT64)(LT_TPM_SPACE_BASE_ADDRESS + LT_TPM_SPACE_ACCESS0),
                                  1,
                                  &Access
                                  );
  ASSERT_EFI_ERROR (Status);

  return (Access == 0xff) ? FALSE : TRUE;
*/
  return TRUE;
}

BOOLEAN
IsLtResetStatus (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
/*++

Routine Description:

  Determines whether TXT Reset Status is set

Arguments:

  LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

Returns:

  TRUE          - If TXT Reset Status is set.
  FALSE         - If TXT Reset Status is not set.

--*/
{
  UINT8       Ests;
  EFI_STATUS  Status;

  //
  // Read LT.ESTS register
  //
  Status = LtDxeCtx->CpuIo->Mem.Read (
                              LtDxeCtx->CpuIo,
                              EfiCpuIoWidthUint8,
                              (UINT64)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_ESTS_OFF),
                              1,
                              &Ests
                              );
  ASSERT_EFI_ERROR (Status);

  return (Ests & LT_TXT_RESET_STS) ? TRUE : FALSE;
}

EFI_STATUS
GetFvImage (
  IN      EFI_GUID                  *NameGuid,
  IN      EFI_SECTION_TYPE          SectionType,
  IN OUT  VOID                      **Buffer,
  IN OUT  UINTN                     *Size,
  IN      EFI_HANDLE                ImageHandle
  )
/*++

Routine Description:

  Get image from FV

Arguments:

  NameGuid      - image file guid
  SectionType   - image file section type
  Buffer        - buffer to hold the image
  Size          - image file size
  ImageHandle   - FV handle

Returns:

  EFI_SUCCESS  the image is returned.
  EFI_ERROR    the image is not found.

--*/
{
  EFI_STATUS                    Status;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *Fv;
  UINT32                        AuthenticationStatus;
  EFI_LOADED_IMAGE_PROTOCOL     *LoadedImageInfo;

  *Buffer = NULL;
  *Size = 0;
    
  Status = gBS->HandleProtocol (
                  ImageHandle,
                  &gEfiLoadedImageProtocolGuid,
                  &LoadedImageInfo
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Find desired image in Fv where SmmBase is 
  //
  Status = gBS->HandleProtocol (
                  LoadedImageInfo->DeviceHandle,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  &Fv
                  );
  ASSERT_EFI_ERROR (Status);

  Status = Fv->ReadSection (
                 Fv,
                 NameGuid,
                 SectionType,
                 0,
                 Buffer,
                 Size,
                 &AuthenticationStatus
                 );
  return Status;
}

//AptioV Server Override Start: Changes done to load SINIT binary from FV_MAIN, to avoid separate FD AREA
//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   GetRawImage
//
// Description: Loads binary from RAW section of main firwmare volume
//
//
// Input:       IN     EFI_GUID   *NameGuid,
//              IN OUT VOID   **Buffer,
//              IN OUT UINTN  *Size
//
// Output:      EFI STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS GetRawImage(
    IN EFI_GUID  *NameGuid,
    IN OUT VOID  **Buffer,
    IN OUT UINTN *Size  )
{
    EFI_STATUS                   Status;
    EFI_HANDLE                   *HandleBuffer = 0;
    UINTN                        HandleCount   = 0;
    UINTN                        i;
    EFI_FIRMWARE_VOLUME2_PROTOCOL *Fv;
    UINT32                       AuthenticationStatus;
    Status = gBS->LocateHandleBuffer(
        ByProtocol,
        &gEfiFirmwareVolume2ProtocolGuid,
        NULL,
        &HandleCount,
        &HandleBuffer
        );
    if ( EFI_ERROR( Status ) || HandleCount == 0 ) {
        return EFI_NOT_FOUND;
    }

    //
    // Find desired image in all Fvs
    //
    for ( i = 0; i < HandleCount; i++ ) {
        Status = gBS->HandleProtocol(
            HandleBuffer[i],
            &gEfiFirmwareVolume2ProtocolGuid,
            &Fv
            );

        if ( EFI_ERROR( Status )) {
            gBS->FreePool( HandleBuffer );
            return EFI_LOAD_ERROR;
        } // if EFI_ERROR

        //
        // Try a raw file
        //
        *Buffer = NULL;
        *Size   = 0;
        Status  = Fv->ReadSection(
            Fv,
            NameGuid,
            EFI_SECTION_RAW,
            0,
            Buffer,
            Size,
            &AuthenticationStatus
            );

        if ( !EFI_ERROR( Status )) {
            break;
        } // if EFI_ERROR
    } // for i
    gBS->FreePool( HandleBuffer );

    if ( i >= HandleCount ) {
        return EFI_NOT_FOUND;
    }

    return EFI_SUCCESS;
}
//AptioV Server Override End: Changes done to load SINIT binary from FV_MAIN, to avoid separate FD AREA

EFI_STATUS
SetupLtDeviceMemory (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
/*++

Routine Description:

  Allocates 1 MB of 1MB-aligned memory for use as LT Device Memory.  Records
  the location of LT Device Memory in LT Chipset registers and then adds
  programming instructions for these registers into BootScript.

Arguments:

  LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

Returns:

  EFI_SUCCESS   - Always.

--*/
{
  EFI_STATUS                          Status;
  volatile UINT64                     *Ptr64;
  UINT64                              Value64;
  UINT32                              Value32;
  EFI_PHYSICAL_ADDRESS                Addr;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL     *PciRootBridge;
  UINT64                              PciAddress;
  EFI_IIO_SYSTEM_PROTOCOL             *IioSystemProtocol;
  IIO_GLOBALS                         *IioGlobalData;
  UINT8                               Iio;
  EFI_PLATFORM_INFO                   *PlatformInfo;
  EFI_HOB_GUID_TYPE                   *GuidHob;

#if 1 //SINIT_IN_FLASH
  UINT8   * PtrSinitAddress;
  UINTN   SinitSize;
#endif
  //
  // Check whether we need to allocate memory
  //
  if ((LtDxeCtx->PlatformTxtDeviceMemory->SINITMemoryAddress == 0) ||
      (LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemoryAddress == 0)) {
    //
    // We do not need support the case that SINIT/TXT heap unallocated, because they must be in DPR region.
    //
    return EFI_UNSUPPORTED;
  }
  
  Status = gBS->LocateProtocol (
                &gEfiPciRootBridgeIoProtocolGuid,
                NULL,
                &PciRootBridge
                );
  ASSERT_EFI_ERROR (Status);
  
  //
  // Find the PlatformInfo hob
  //
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfo);

  //
  // Program LT Device Memory Chipset Registers and record them in
  // BootScript so they will be saved and restored on S3
  //

  //
  // NoDMA Registers
  // We do not need NoDMA support, which is deprecated by VTd
  //

  //
  // SINIT Registers
  //
  Ptr64       = (UINT64 *)(UINTN)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_SINIT_BASE_REG_OFF);
  Value64     = (UINT64)LtDxeCtx->PlatformTxtDeviceMemory->SINITMemoryAddress;
  *Ptr64      = Value64;
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32, 
    (UINT64)(UINTN)(Ptr64), 
    2, 
    &Value64
    );

  Ptr64       = (UINT64 *)(UINTN)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_SINIT_SIZE_REG_OFF);
  Value64     = (UINT64)LtDxeCtx->PlatformTxtDeviceMemory->SINITMemorySize;
  *Ptr64      = Value64;
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32, 
    (UINT64)(UINTN)(Ptr64), 
    2, 
    &Value64
    );

  ZeroMem (
    (VOID *)(UINTN)LtDxeCtx->PlatformTxtDeviceMemory->SINITMemoryAddress, 
    (UINTN)LtDxeCtx->PlatformTxtDeviceMemory->SINITMemorySize
    );

  //
  // TXTHEAP Registers
  //
  Ptr64       = (UINT64 *)(UINTN)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_HEAP_BASE_REG_OFF);
  Value64     = (UINT64)LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemoryAddress;
  *Ptr64      = Value64;
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32, 
    (UINT64)(UINTN)(Ptr64), 
    2, 
    &Value64
    );

  Ptr64       = (UINT64 *)(UINTN)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_HEAP_SIZE_REG_OFF);
  Value64     = (UINT64)LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemorySize;
  *Ptr64      = Value64;
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32, 
    (UINT64)(UINTN)(Ptr64), 
    2, 
    &Value64
    );

  ZeroMem (
    (VOID *)(UINTN)LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemoryAddress, 
    (UINTN)LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemorySize
    );   
    
  Status = PciRootBridge->Pci.Read (
                           PciRootBridge,
                           EfiPciWidthUint32,
                           EFI_PCI_ADDRESS_EXT (0, 5, 0, SA_DPR_REG),
                           1,
                           &Value32
                           );
  //
  // DPR Registers - skip if already programmed
  //  
  if (LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionAddress != 0 && ((Value32 & 0x0FFF) == 0)) {

    Status = gBS->LocateProtocol (
                    &gEfiIioSystemProtocolGuid,
                    NULL,
                    &IioSystemProtocol
                    );
    ASSERT_EFI_ERROR (Status);

    IioGlobalData = IioSystemProtocol->IioGlobalData;

    Value32 = ((UINT32)(LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionSize) >> 16) | SA_DPR_LOCK | SA_DPR_PRM;

    for (Iio = 0; Iio < MaxIIO; Iio++) {
      if (!IioGlobalData->IioVData.SocketPresent [Iio]) {
        continue;
      }
      
    //
	  // BUGBUG: the PciAddress for DPR Register is changed from generation to gen, so need a PCD here.
  	//
      PciAddress = EFI_PCI_ADDRESS_EXT (
                    IioGlobalData->IioVData.SocketBaseBusNumber [Iio],
                    5,
                    0,
                    SA_DPR_REG
                                      );

    Status = PciRootBridge->Pci.Write (
                                       PciRootBridge,
                                       EfiPciWidthUint32,
                                       PciAddress,
                                       1,
                                       &Value32
                                      );
      ASSERT_EFI_ERROR (Status);

    Value32 = 0;
    Status = PciRootBridge->Pci.Read (
                                       PciRootBridge,
                                       EfiPciWidthUint32,
                                       PciAddress,
                                       1,
                                       &Value32
                                       );
      DEBUG ((EFI_D_ERROR, "DPR Register [%08x] = %08x! on bus %x\n", PciAddress, Value32, IioGlobalData->IioVData.SocketBaseBusNumber [Iio]));

    //
    // Assert error if programmed value is different from requested. This
    // means error is requested size.
    //
    //BUGBUG: need to double check DPR REG//ASSERT (((Value32 & 0xFF0) << 16) == LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionSize);
    if (((Value32 & 0xFF0) << 16) == LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionSize){

        S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
		//AptioV server override start: dynamic MMCFG base support
		#ifdef DYNAMIC_MMCFG_BASE_FLAG
          (UINTN)MmPciAddress (0, IioGlobalData->IioVData.SocketBaseBusNumber [Iio], 5, 0, SA_DPR_REG),	//Aptio V Server Override
		#endif
		#ifndef DYNAMIC_MMCFG_BASE_FLAG  
		 (UINTN)MmPciAddress (IioGlobalData->IioVData.SocketBaseBusNumber [Iio], 5, 0, SA_DPR_REG),	//Aptio V Server Override
		#endif
		//AptioV server override end: dynamic MMCFG base support
        1,
        &Value32
        );

    } else {
      DEBUG ((EFI_D_ERROR, "TXT-LIB SA_DPR_REG[0x%02x] = 0x%08x not set correctly!!! \n", SA_DPR_REG, Value32));
    }
    }

    //
    // DPR registers
    //
    Ptr64   = (UINT64 *) (UINTN) (LT_PUBLIC_SPACE_BASE_ADDRESS + LT_DMA_PROTECTED_RANGE_REG_OFF);
    Value64 = RShiftU64 (LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionSize, 16) | SA_DPR_LOCK ;
    //BUGBUG: we add top of the DPR, currently no one to set it?
    Value64 |= ((LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionAddress + LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionSize) & 0xFFF00000);

    *Ptr64 = Value64;
    //
    // Assert error if programmed value is different from requested. This
    // means error is requested size.
    //
    Value64 = *Ptr64;
    ASSERT ((LShiftU64 ((Value64 & 0xFFE), 16)) == LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionSize);

    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINT64) (UINTN) (Ptr64),
      2,
      &Value64
      );

    ZeroMem (
      (VOID *)(UINTN)LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionAddress, 
      (UINTN)LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionSize
      );
  
  }

  //
  // Fill BiosOsDataRegion
  //
  Ptr64     = (UINT64 *)(UINTN)(LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemoryAddress + 8);
  
  if (LtDxeCtx->PlatformTxtPolicyData->BiosOsDataRegionRevision >= 2) {
    *(UINT64 *)(UINTN)LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemoryAddress = sizeof(EFI_TXT_BIOS_OS_DATA_REGION_V3)+ 8 ;
    
    ((EFI_TXT_BIOS_OS_DATA_REGION_V3 *)Ptr64)->Revision = LtDxeCtx->PlatformTxtPolicyData->BiosOsDataRegionRevision;

    //
    // Allocate Physical Memory for LCP
    //
    ((EFI_TXT_BIOS_OS_DATA_REGION_V2 *)Ptr64)->LcpPolicyDataBase = 0;
    ((EFI_TXT_BIOS_OS_DATA_REGION_V2 *)Ptr64)->LcpPolicyDataSize = 0;

    if ((LtDxeCtx->PlatformTxtPolicyData->LcpPolicyDataBase != 0) &&
        (LtDxeCtx->PlatformTxtPolicyData->LcpPolicyDataSize != 0)) {
     //LCP PD must be copied into a DMA protected region, here it will be copied to DPR region
      Addr = LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionAddress + (UINT32)LShiftU64 (PlatformInfo->PfatMemSize, 20);
      ZeroMem (
        (VOID *)(UINTN)Addr, 
        (UINTN)LtDxeCtx->PlatformTxtPolicyData->LcpPolicyDataSize
        );
      CopyMem (
        (VOID *)(UINTN)Addr,
        (VOID *)(UINTN)LtDxeCtx->PlatformTxtPolicyData->LcpPolicyDataBase,
        (UINTN)LtDxeCtx->PlatformTxtPolicyData->LcpPolicyDataSize
        );
      ((EFI_TXT_BIOS_OS_DATA_REGION_V3 *)Ptr64)->LcpPolicyDataBase = Addr;
      ((EFI_TXT_BIOS_OS_DATA_REGION_V3 *)Ptr64)->LcpPolicyDataSize = LtDxeCtx->PlatformTxtPolicyData->LcpPolicyDataSize;
    }
    
    ((EFI_TXT_BIOS_OS_DATA_REGION_V3 *)Ptr64)->NumberOfLogicalProcessors = (UINT32)LtDxeCtx->ApCount + 1;

    if (LtDxeCtx->PlatformTxtPolicyData->BiosOsDataRegionRevision == 3){
      ((EFI_TXT_BIOS_OS_DATA_REGION_V3 *)Ptr64)->FlagVariable = LtDxeCtx->PlatformTxtPolicyData->FlagVariable;
    }

  } 
  
  //
  // Set it to zero because SINIT_ACM is not loaded here.
  //
  ((EFI_TXT_BIOS_OS_DATA_REGION_V1 *)Ptr64)->BiosSinitSize = 0;

  //Below debug should = 0x2c, when version = 3
  DEBUG ((EFI_D_ERROR, "  Lt - BiosDataSize = 0x%08x\n", *(UINT64 *)(UINTN)LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemoryAddress));
#if 1 //SINIT_IN_FLASH
    //
    // copy SINIT-ACM to LT.SINIT.MEMORY.BASE(LT offset 0x270)
//AptioV Server Override Start: Changes done to load SINIT binary from FV_MAIN, to avoid separate FD AREA
      Status = GetRawImage( &gAmiLtsxSinitGuid, &PtrSinitAddress, &SinitSize );

      if ( EFI_ERROR( Status )) {
	  DEBUG((EFI_D_ERROR, "Failed to load SINIT ACM BINARY: Status=%r\n", Status));
          return Status;
      }
//    PtrSinitAddress = (UINT8 *)(UINTN)(LtDxeCtx->PlatformTxtPolicyData->StartupAcmAddress + 0x10000); // Get SINIT-ACM addresss 
//    SinitSize =  (UINTN) *(UINT32 *)(PtrSinitAddress+0x18);   // get SINIT-ACM size from ACM header
//    SinitSize *= 4;                                 // Convert the size from 16 byte countes into byte countes
//AptioV Server Override End: Changes done to load SINIT binary from FV_MAIN, to avoid separate FD AREA
    DEBUG ((EFI_D_ERROR, " SINIT_IN_FLASH: copy from base:0x%08x, Size:0x%x\n", PtrSinitAddress,SinitSize));
      CopyMem (
        (VOID *)(UINTN)LtDxeCtx->PlatformTxtDeviceMemory->SINITMemoryAddress, 
        (VOID *)PtrSinitAddress,
        (UINTN)SinitSize
        );
    // update the SINIT size in BiosOsData region (in the TXT HEAP)
    ((EFI_TXT_BIOS_OS_DATA_REGION_V3 *)Ptr64)->BiosSinitSize = (UINT32) SinitSize;
#endif

  return EFI_SUCCESS ;
}

EFI_STATUS
DoScheck (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
/*++

Routine Description:

  Invokes LtDxeLibLaunchBiosAcm to execute the SCHECK function.

Arguments:

  LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

Returns:

  EFI_SUCCESS   - Always.

--*/
{
  EFI_STATUS              Status;

  if (LtDxeCtx->PlatformTxtPolicyData->StartupAcmAddress!=0) {

    Status = LtDxeLibLaunchBiosAcm (LtDxeCtx, LT_LAUNCH_SCHECK);
  } else {
    DEBUG ((EFI_D_ERROR, "Did not call ACM. The ACM module is not enabled ...\n"));
    Status = EFI_SUCCESS ;
  }

  return Status;
}

EFI_STATUS
DoLockConfig  (
  IN LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
{
  if (LtDxeCtx->PlatformTxtPolicyData->StartupAcmAddress!=0) {
    DEBUG ((EFI_D_ERROR, "Inside the Lock routine of Func6 \n"));
    LtDxeLibLaunchBiosAcm (LtDxeCtx, LT_LOCK_CONFIG);
  } else {
    DEBUG ((EFI_D_ERROR, "Did not call ACM. The ACM module is not enabled ...\n"));
  }
  return EFI_SUCCESS ;
}

VOID
LockConfig (
  IN      VOID      *LtDxeCtx
  )
/*++

Routine Description:

  Invokes LtDxeLibLaunchBiosAcm to execute the lock config function.

Arguments:

  LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

Returns:

  EFI_SUCCESS   - Always.

--*/
{ 
  LtDxeLibLaunchBiosAcm ((LT_DXE_LIB_CONTEXT *)LtDxeCtx, LT_LOCK_CONFIG);
}

VOID
DoLockConfigForAll (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
{
  UINTN       CpuNumber;
  UINTN       Index;
  EFI_STATUS  Status;

  UINT8       Socket_Index[4] = {0xFF,0xFF,0xFF,0xFF};         
  UINT8       SocketID_Found[4] = {0xFF,0xFF,0xFF,0xFF};       
  UINT8       Temp_SocketID,SocketID_BSP,Index1;               
  UINT32      mNumOfBitShift;

  //
  // LTSX lock config (BIOS ACM call)
  //

  //
  // now call LockConfig
  //
  LtDxeCtx->MpService->WhoAmI (LtDxeCtx->MpService, &CpuNumber);     // CpuNumber = Original BSP 
  DEBUG ((EFI_D_ERROR, "  DoLockConfig for BSP - %d ", CpuNumber)); 
  LockConfig((VOID *) LtDxeCtx);
  DEBUG ((EFI_D_ERROR, "Done! \n"));     

  AsmCpuidEx(EFI_CPUID_CORE_TOPOLOGY, 1, &mNumOfBitShift, NULL, NULL, NULL);
  mNumOfBitShift &= 0x1F;
    DEBUG(( EFI_D_ERROR, "mNumofBitShift! %d\n", mNumOfBitShift)); 
  SocketID_BSP = (UINT8)(LtDxeCtx->ApicId[CpuNumber])>>mNumOfBitShift;
    DEBUG(( EFI_D_ERROR, "SocketID_BSP %d\n", SocketID_BSP)); 
  for (Index = 0; Index < LtDxeCtx->ApCount + 1; Index++) {
     Temp_SocketID = (UINT8)(LtDxeCtx->ApicId[Index])>>mNumOfBitShift;  //Get socket ID
     if (Temp_SocketID == SocketID_BSP) {
       continue; 
     }
     for (Index1 = 0; Index1 < 4; Index1++) {
        if (SocketID_Found[Index1] == Temp_SocketID) {
           break;
        }
        if (SocketID_Found[Index1] == 0xFF) {
           SocketID_Found[Index1] = Temp_SocketID;
           Socket_Index[Index1] = (UINT8)Index;
           break;
        }
     }
  }

  DEBUG ((EFI_D_ERROR, "  Socket_Index[0]=%d ", Socket_Index[0])); 
  DEBUG ((EFI_D_ERROR, "  Socket_Index[1]=%d ", Socket_Index[1])); 
  DEBUG ((EFI_D_ERROR, "  Socket_Index[2]=%d ", Socket_Index[2])); 
  DEBUG ((EFI_D_ERROR, "  Socket_Index[3]=%d \n", Socket_Index[3])); 

  for (Index = 0; Index < 4; Index++) {
    if (Socket_Index[Index] == 0xFF) { 
      break;
    }

    DEBUG ((EFI_D_ERROR, "  Switch BSP to Index=%d, Socket_Index=%d \n", (UINTN)Index,(UINTN)Socket_Index[Index])); 

    // Index = The new BSP number
    //
    // call an ASM function to exchange rolls with the currently AP (but future BSP)
    //
    Status = LtDxeCtx->MpService->SwitchBSP (
              LtDxeCtx->MpService,
              (UINTN)Socket_Index[Index],               // The new BSP number
              TRUE
              );
    if (EFI_ERROR(Status)) {
      goto Done;
    }
 
    //
    // now call LockConfig
    //
    DEBUG ((EFI_D_ERROR, "  DoLockConfig for AP - %d ", (UINTN)Socket_Index[Index])); 
    LockConfig((VOID *) LtDxeCtx);
    DEBUG ((EFI_D_ERROR, "Done! \n"));     
    
  }

  if (Socket_Index[0] != 0xFF)  {    //BSP is switched, switch back to original BSP
    // call the ASM function to exchange rolls with the original BSP    
    Status = LtDxeCtx->MpService->SwitchBSP (
               LtDxeCtx->MpService,
               CpuNumber,                // Original BSP
               TRUE
               );

    if (EFI_ERROR(Status)) {
      goto Done;
    }
  }

Done:

  return;
}

EFI_STATUS
TxtLockConfigForAll (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
/*++

Routine Description:

  Invokes LtDxeLibLaunchBiosAcm to execute the lock config function.

Arguments:

  LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

Returns:

  EFI_SUCCESS   - Always.

--*/
{
  EFI_STATUS                  Status        = EFI_SUCCESS;  
  volatile UINT64 *                    Ptr64         = NULL;
  UINT64                      Value64       = 0;

 
  //
  // Below code is from BecktonMpService.c
  //
  DoLockConfigForAll (LtDxeCtx);

  //
  // TXTHEAP Registers reset after lockconfig
  //
  DEBUG ((EFI_D_ERROR, "  Set BASE/SIZEs after LockConfig... \n")); 
  Ptr64       = (UINT64 *)(UINTN)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_HEAP_BASE_REG_OFF);
  Value64     = (UINT64)LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemoryAddress;
  DEBUG ((EFI_D_ERROR, "  Allocate TXTHeapMemoryAddress = 0x%08x... \n", Value64));
  *Ptr64      = Value64;
  DEBUG ((EFI_D_ERROR, "  After setting, HEAP.BASE[0x%08x] = 0x%08x \n",(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_HEAP_BASE_REG_OFF), *Ptr64)); 

  Ptr64       = (UINT64 *)(UINTN)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_HEAP_SIZE_REG_OFF);
  Value64     = (UINT64)LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemorySize;
  DEBUG ((EFI_D_ERROR, "  Allocate TXTHeapMemorySize = 0x%08x... \n", Value64));
  *Ptr64      = Value64;
  DEBUG ((EFI_D_ERROR, "  After setting, HEAP.SIZE[0x%08x] = 0x%08x \n", (LT_PUBLIC_SPACE_BASE_ADDRESS + LT_HEAP_SIZE_REG_OFF), *Ptr64)); 

  Ptr64       = (UINT64 *)(UINTN)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_SINIT_BASE_REG_OFF);
  Value64     = (UINT64)LtDxeCtx->PlatformTxtDeviceMemory->SINITMemoryAddress;
  DEBUG ((EFI_D_ERROR, "  Allocate SINITMemoryAddress = 0x%08x... \n", Value64));
  *Ptr64      = Value64;
  DEBUG ((EFI_D_ERROR, "  After setting, SINIT.BASE[0x%08x] = 0x%08x \n", (LT_PUBLIC_SPACE_BASE_ADDRESS + LT_SINIT_BASE_REG_OFF), *Ptr64)); 
  
  Ptr64       = (UINT64 *)(UINTN)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_SINIT_SIZE_REG_OFF);
  Value64     = (UINT64)LtDxeCtx->PlatformTxtDeviceMemory->SINITMemorySize;
  DEBUG ((EFI_D_ERROR, "  Allocate SINITMemorySize = 0x%08x... \n", Value64));
  *Ptr64      = Value64;
  DEBUG ((EFI_D_ERROR, "  After setting, SINIT.SIZE[0x%08x] = 0x%08x \n", (LT_PUBLIC_SPACE_BASE_ADDRESS + LT_SINIT_SIZE_REG_OFF), *Ptr64)); 

  return Status;

}

EFI_STATUS
ClearTpmAuxIndex (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
/*++

Routine Description:

  Invokes LtDxeLibLaunchBiosAcm to clear the TPM's aux index.

Arguments:

  LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

Returns:

  EFI_SUCCESS   - Always.

--*/
{
  EFI_STATUS              Status;

  //
  // We cannot do getsec if we haven't enabled this instruction in processor
  //
  if ((LtDxeCtx->PlatformTxtPolicyData->StartupAcmAddress!=0) && IsLtEnabled (LtDxeCtx)) {

    Status = LtDxeLibLaunchBiosAcm (LtDxeCtx, LT_CLEAR_TPM_AUX_INDEX);
  } else {
    DEBUG ((EFI_D_ERROR, "Did not call ACM. The ACM module is not enabled ...\n"));
    Status = EFI_SUCCESS ;
  }

  return Status;
}

EFI_STATUS	
ResetTpmEstBit (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
/*++

Routine Description:

  Invokes LtDxeLibLaunchBiosAcm to reset the TPM's establishment bit.

Arguments:

  LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

Returns:

  EFI_SUCCESS   - Always.

--*/
{
  EFI_STATUS              Status;

  //
  // We cannot do getsec if we haven't enabled this instruction in processor
  //
  if ((LtDxeCtx->PlatformTxtPolicyData->StartupAcmAddress!=0)&& IsLtEnabled (LtDxeCtx)) {

    Status = LtDxeLibLaunchBiosAcm (LtDxeCtx, LT_RESET_EST_BIT);
  } else {
    DEBUG ((EFI_D_ERROR, "Did not call ACM. The ACM module is not enabled ...\n"));
    Status = EFI_SUCCESS ;
  }

  return Status;
}

STATIC
EFI_COMPATIBILITY16_TABLE *
GetCompatibility16Table (
  VOID
  )
/*++

Routine Description:

  Search legacy region for compatibility16 table.

Arguments:

  None

Returns:

  EFI_COMPATIBILITY16_TABLE   - EFI_COMPATIBILITY16_TABLE is found
  NULL                        - EFI_COMPATIBILITY16_TABLE is not found

--*/
{
  UINT8                             *Ptr;
  EFI_COMPATIBILITY16_TABLE         *Table;

  //
  // Scan legacy region
  //
  for (Ptr = (UINT8 *)(UINTN)0xE0000; Ptr < (UINT8 *)(UINTN)(0x100000); Ptr += 0x10) {
    if (*(UINT32 *) Ptr == SIGNATURE_32 ('I', 'F', 'E', '$')) {
      Table   = (EFI_COMPATIBILITY16_TABLE *) Ptr;
      return Table;
    }
  }

  return NULL;
}

#define EFI_COMPATIBILITY16_TABLE_SIGNATURE SIGNATURE_32 ('I', 'F', 'E', '$')

UINT8 *
FindCompatibility16Table (
  VOID
  )
/*++
Routine Description:
  Find the EFI_COMPATIBILITY16_TABLE in E/F segment.

Returns:
  The found EFI_COMPATIBILITY16_TABLE or NULL if not found.
--*/
{
  UINT8  *Table;

  for (Table = (UINT8 *) (UINTN) 0xE0000; 
      Table < (UINT8 *) (UINTN) 0x100000;
      Table = (UINT8 *) Table + 0x10
      ) {
    if (*(UINT32 *) Table == EFI_COMPATIBILITY16_TABLE_SIGNATURE) {
      return Table;
    }
  }

  return NULL;
}



STATIC
EFI_STATUS
GetApVector (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
/*++

Routine Description:

  Find a memory in E/F segment for AP reset vector.

Arguments:

  LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

Returns:

  EFI_SUCCESS   - The ApVector in E/F segment.
  EFI_NOT_FOUND - No enough space for ApVector in E/F segment.

--*/
{
  EFI_LEGACY_BIOS_PROTOCOL   *LegacyBios;
  EFI_LEGACY_REGION2_PROTOCOL *LegacyRegionProtocol;
  EFI_IA32_REGISTER_SET      Regs;
  EFI_PHYSICAL_ADDRESS       LegacyRegionAddress;
  EFI_STATUS                 Status;
  UINT32                     WakeUpBufferAddr;
  UINT32                     BackupBuffer;
  EFI_COMPATIBILITY16_TABLE  *Table;

  WakeUpBufferAddr = 0;
  LtDxeCtx->ApVector = 0;

//Aptio V Server Override Start
  Status = gBS->LocateProtocol (&gEfiLegacyRegion2ProtocolGuid, NULL, &LegacyRegionProtocol);
  ASSERT_EFI_ERROR (Status);
//Aptio V Server Override End

  Status = gBS->LocateProtocol (&gEfiLegacyBiosProtocolGuid, NULL, &LegacyBios);
  if (!EFI_ERROR (Status)) {
    Table = (EFI_COMPATIBILITY16_TABLE *) FindCompatibility16Table ();
    ASSERT (Table != NULL);    
    //
    // Allocate 0x1004 bytes from below 1M memory to make sure I can
    // get a 4k aligned spaces of 0x1000 bytes, since Alignment argument does not work.
    //
    Regs.X.AX = (UINT16)Legacy16GetTableAddress;
    Regs.X.CX = (UINT16)(sizeof(BackupBuffer) + EFI_PAGE_SIZE);
    Regs.X.BX = (UINT16)0x2;//Location 0xE0000 or 0xF0000; BIT0: F seg, BIT1: E seg
    Regs.X.DX = (UINT16)0;//Alignment 2^0 = 1 byte

//Aptio V Server Override - Unlock
    LegacyRegionProtocol->UnLock (LegacyRegionProtocol, 0xC0000, 0x40000, NULL);
    LegacyBios->FarCall86 (
                  LegacyBios,
                  Table->Compatibility16CallSegment, 
                  Table->Compatibility16CallOffset, 
                  &Regs, 
                  NULL, 
                  0
                  );

//Aptio V Server Override - Lock
    LegacyRegionProtocol->Lock (LegacyRegionProtocol, 0xC0000, 0x40000, NULL);
    ASSERT (Regs.X.AX == 0);
    if (Regs.X.AX == 0) {
      WakeUpBufferAddr = (UINT32)((Regs.X.DS << 4) + Regs.X.BX + 0x4);
      WakeUpBufferAddr = (WakeUpBufferAddr + 0x0fff) & 0xfffff000;
    } else {
      return EFI_NOT_FOUND;
    }

  } else {
    //
    // No LegacyBios Protocol means NO-CSM
    //

    //
    // Allocate 0x1004 bytes from below 1M memory to make sure I can
    // get a 4k aligned spaces of 0x1000 bytes, since Alignment argument does not work.
    //
    LegacyRegionAddress = 0xFFFFF;
    Status = gBS->AllocatePages (
                    AllocateMaxAddress,
                    EfiReservedMemoryType,
                    EFI_SIZE_TO_PAGES(sizeof(BackupBuffer) + EFI_PAGE_SIZE),
                    &LegacyRegionAddress
                    );
    ASSERT_EFI_ERROR (Status);

    WakeUpBufferAddr = (UINT32) LegacyRegionAddress;
    WakeUpBufferAddr = (WakeUpBufferAddr + 0x0fff) & 0xfffff000;
  }

//Aptio V Server Override - locate protocol moved above.
// Status = gBS->LocateProtocol (&gEfiLegacyRegion2ProtocolGuid, NULL, &LegacyRegionProtocol);
//  ASSERT_EFI_ERROR (Status);

  //
  // Fill in machine code for cli, hlt, and jmp $-2
  //
  DEBUG ((EFI_D_ERROR, "  WakeUpBufferAddr                - 0x%08x\n", (UINTN)WakeUpBufferAddr));
  LegacyRegionProtocol->UnLock (LegacyRegionProtocol, 0xC0000, 0x40000, NULL);
  *(UINT32 *)(UINTN)WakeUpBufferAddr = 0xFCEBF4FA;
  LegacyRegionProtocol->Lock (LegacyRegionProtocol, 0xC0000, 0x40000, NULL);

  LtDxeCtx->ApVector = (UINT8)(WakeUpBufferAddr >> 12);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
LtDxeLibLaunchBiosAcm (
  IN LT_DXE_LIB_CONTEXT      *LtDxeCtx,
  IN UINT32                  AcmFunction
  )
/*++

Routine Description:

  Sets up the system and then launches the LT BIOS ACM to run the function
  requested by AcmFunction. 

Arguments:

  AcmFunction   - Constant that represents the function from the BIOS ACM
                  that should be executed.

Returns:

  EFI_SUCCESS   - Always.

--*/
{
  EFI_STATUS                  Status ;
  EFI_TPL                     OldTpl;
  UINTN                       MyCpuNumber ;

  //Aptio V Server Override - Start Move disabling periodic timer before ACM launch and restore original value afterwards.
  UINT8 PchSmiEnVal = IoRead8(PM_BASE_ADDRESS + R_PCH_SMI_EN);
  IoWrite8(PM_BASE_ADDRESS + R_PCH_SMI_EN, PchSmiEnVal & ~(B_PCH_SMI_EN_SWSMI_TMR));  // disable Software SMI Timer
  //Aptio V Server Override - End
  
  MyCpuNumber = 0;
  if (LtDxeCtx->ApCount > 0) {
    Status = LtDxeCtx->MpService->WhoAmI (
                                    LtDxeCtx->MpService, 
                                    &MyCpuNumber
                                    );
    ASSERT_EFI_ERROR (Status);

    //
    // Save AP configuration. Run on one AP since all must be programmed
    // identically
    SendInitIpiAllExcludingSelf();
  }

  //
  // Launch the BIOS ACM to run the requested function
  //
#if 1
  DEBUG ((EFI_D_ERROR, "  BiosAcmAddress                - 0x%08x\n", (UINTN)LtDxeCtx->PlatformTxtPolicyData->BiosAcmAddress));
#endif

  OldTpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
  LaunchBiosAcm ((UINTN)LtDxeCtx->PlatformTxtPolicyData->BiosAcmAddress, (UINTN)AcmFunction);
  gBS->RestoreTPL (OldTpl);
  SendInitSipiSipiAllExcludingSelf (LtDxeCtx->ApVector << 12);

  //Aptio V Server Override - Start Move disabling periodic timer before ACM launch and restore original value afterwards.
  IoWrite8(PM_BASE_ADDRESS + R_PCH_SMI_EN, PchSmiEnVal);  //Restore original value
  
  return EFI_SUCCESS ;
}

EFI_STATUS
DisableLtsxAndReset (
  VOID
  )
/*++
Routine Description:

  Once LT BIOS ACM error, disable Ltsx and re-boot the system.

--*/
{
  EFI_STATUS                            Status;
  UINT8                                 Data8;
  UINTN                                 SysCfgSize;
  UINT32                                SysCfgAttr; // AptioV Server Override: Remove NVRAM RT Attribute
  SYSTEM_CONFIGURATION                  SysCfg;

  //
  // Read the current system configuration variable store.
  //

      IoWrite8(0x70, CMOS_LTSX_OFFSET); 
      Data8 = IoRead8(0x71);    
      Data8 &= ~(BIT4 | BIT5);
      DEBUG((EFI_D_ERROR, "CMOS_LTSX_OFFSET(%02x)= 0x%02x: for LT/TPM policy\n", CMOS_LTSX_OFFSET, Data8));
	// write back CMOS Bit4 and Bit5
      IoWrite8(0x70, CMOS_LTSX_OFFSET); 
      IoWrite8(0x71, Data8);  

	  IoWrite8(0x70, CMOS_LTSX_OFFSET); 
      Data8 = IoRead8(0x71);
	  DEBUG((EFI_D_ERROR, "REVIEW: CMOS_LTSX_OFFSET(%02x)= 0x%02x: for LT/TPM policy\n", CMOS_LTSX_OFFSET, Data8));

      SysCfgSize = sizeof (SysCfg);
      Status = gRT->GetVariable (
                  L"IntelSetup", //AptioV server override
                  &gEfiSetupVariableGuid,
                  &SysCfgAttr, // Attributes // AptioV Server Override: Remove NVRAM RT Attribute
                  &SysCfgSize,
                  &SysCfg
                  );
      if (EFI_ERROR(Status)) {
        return Status;
      }
	  DEBUG((EFI_D_ERROR, "SysCfg.ProcessorLtsxEnable: %02x\n", SysCfg.ProcessorLtsxEnable));
	  DEBUG((EFI_D_ERROR, "SysCfg.ProcessorSmxEnable: %02x\n", SysCfg.ProcessorSmxEnable));
	  DEBUG((EFI_D_ERROR, "SysCfg.ProcessorVmxEnable: %02x\n", SysCfg.ProcessorVmxEnable));
          DEBUG((EFI_D_ERROR, "SysCfg.LockChipset: %02x\n", SysCfg.ProcessorVmxEnable));
      SysCfg.ProcessorLtsxEnable = 0;
      SysCfg.ProcessorSmxEnable = 0;
      SysCfg.ProcessorVmxEnable = 0;
      SysCfg.LockChipset = 0;
	  DEBUG((EFI_D_ERROR, "SysCfg.ProcessorLtsxEnable: %02x\n", SysCfg.ProcessorLtsxEnable));
	  DEBUG((EFI_D_ERROR, "SysCfg.ProcessorSmxEnable: %02x\n", SysCfg.ProcessorSmxEnable));
	  DEBUG((EFI_D_ERROR, "SysCfg.ProcessorVmxEnable: %02x\n", SysCfg.ProcessorVmxEnable));
          DEBUG((EFI_D_ERROR, "SysCfg.LockChipset: %02x\n", SysCfg.ProcessorVmxEnable));
      SysCfgSize = sizeof (SysCfg);
      Status = gRT->SetVariable (
                  L"IntelSetup", //AptioV server override
                  &gEfiSetupVariableGuid,
                  SysCfgAttr, // AptioV Server Override: Remove NVRAM RT Attribute
                  SysCfgSize,
                  &SysCfg
                  );
      if (EFI_ERROR(Status)) {
        return Status;
      }  
	  
	  //
      // Do this powergood reset only if we're not in HSX PO
      // where a bootscript is needed to start the CPU
      //
      DEBUG((EFI_D_ERROR, "Acm Fails : do powergood reset!\n"));
      gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
      // do powergood reset
      return Status;
}
