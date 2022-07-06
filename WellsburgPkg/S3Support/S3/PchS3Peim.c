/** @file
  This is the PEIM that performs the S3 resume tasks.

@copyright
  Copyright (c) 2008 - 2017 Intel Corporation. All rights reserved
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
#include "PchS3Peim.h"
#include "S3SupportHob.h"
#include <Ppi/SmmAccess.h>
#include <Library/PeimEntryPoint.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>

//
// GUID Declarations
//
extern EFI_GUID gS3SupportHobGuid;
extern EFI_GUID gS3SupportSmramDataGuid;
extern EFI_GUID gS3DataHobGuid;

static EFI_PEI_NOTIFY_DESCRIPTOR mSmmAccessCallbackList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiSmmAccessPpiGuid,
  CreateS3DataHob
};

/**
  PCH S3 PEIM entry point.  This Entry Point is entered for three separate reasons.
    1. It is entered via dispatcher in standard POST in order to create a HOB for
       the DXE module to find it's EntryPoint.
    2. It is entered via dispatcher in S3 Resume in order to find the Dispatch Script
       in SMRAM and copy it to a HOB in Boot Services memory.
    3. It is entered in response to an invocation from the Boot Script Dispatch Opcode.

  In all three cases it is critical that this code is executed directly from Flash and
  not from a location in memory.

  @param[in] FileHandle           PEI File handle.
  @param[in] PeiServices          PEI Services table pointer

  @retval EFI_SUCCESS             Successfully completed
**/
EFI_STATUS
EFIAPI
InitializePchS3Peim (
  IN      EFI_PEI_FILE_HANDLE       FileHandle,
  IN      CONST EFI_PEI_SERVICES          **PeiServices
  )
{
  EFI_STATUS                    Status;
  EFI_BOOT_MODE                 BootMode;
  UINT32                        TypeSize;
  UINT32                        ParameterSize;
  UINT32                        Size;
  EFI_PCH_S3_DISPATCH_ARRAY     *DispatchArray;
  EFI_PCH_S3_DISPATCH_ITEM      *DispatchItem;
  S3_SUPPORT_HOB                *S3SupportHob;
  S3_DATA_HOB                   *GuidHob;
  PEI_SMM_ACCESS_PPI            *SmmAccessPpi;

  DEBUG ((DEBUG_INFO, "InitializePchS3Peim() Start\n"));

  ///
  /// If not in S3 boot path, only generate the Hob with entry point data
  ///
  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "PchS3Peim S3 Boot Mode not available.\n"));
    return Status;
  }

  DEBUG ((EFI_D_INFO, "PchS3Peim S3 Boot Mode Check: %x\n", BootMode));

  if (BootMode != BOOT_ON_S3_RESUME) {
    //
    // If we are entering the entry point during POST, then we need to generate a HOB with the entry point information in order to
    // pass the data to DXE for entry into the Boot Script.
    //
    DEBUG ((EFI_D_INFO, "Attempt to create the PchS3Peim S3 Support Hob\n"));

    Status = (*PeiServices)->CreateHob (
      PeiServices,
      EFI_HOB_TYPE_GUID_EXTENSION,
      sizeof (S3_SUPPORT_HOB),
      &S3SupportHob
      );
    if (!EFI_ERROR (Status)) {
      S3SupportHob->Header.Name = gS3SupportHobGuid;
      S3SupportHob->PchS3PeimEntryPoint = (UINTN) _ModuleEntryPoint;
      DEBUG ((EFI_D_INFO, "PCH S3 Hob Created - %g\n", &gS3SupportHobGuid));
    }
    return Status;
  }
  else
  {
    GuidHob = (S3_DATA_HOB *)GetFirstGuidHob (&gS3DataHobGuid);

    ///
    /// Search for the S3 Data Hob
    ///
    if (GuidHob == NULL)
    {
      DEBUG ((EFI_D_INFO, "PCH S3 Data HOB didn't exist - So collect data from SMRAM for PCH S3 Data Script\n"));

      Status = PeiServicesLocatePpi (
        &gPeiSmmAccessPpiGuid,
        0,
        NULL,
        (VOID **)&SmmAccessPpi
        );
      if (Status == EFI_SUCCESS)
      {

        ///
        /// Then this is our first call on S3 resume
        /// Find the PCH S3 Boot Script within SMRAM and create the HOB used for executing the Script
        ///
        Status = CreateS3DataHob ((EFI_PEI_SERVICES **) PeiServices, NULL, SmmAccessPpi);

        DEBUG ((EFI_D_INFO, "CreateS3DataHob, result=%r\n", Status));
      }
      else
      {
        ///
        /// Register for notify if SMM_ACCESS isn't yet available.
        /// We can't have a module dependency on SMM_ACCESS because the module
        /// must be called on both Normal Boot and S3 resume.  SMM_ACCESS
        /// PPI isn't published on Normal Boot, however.
        ///
        DEBUG ((EFI_D_INFO, "SMM Access protocol not yet available -> Register for notification later.\n"));

        //
        // Register notify to set default variable once variable service is ready.
        //
        Status = (**PeiServices).NotifyPpi (PeiServices, &mSmmAccessCallbackList);
        ASSERT_EFI_ERROR (Status);

      }
      return Status;
    }
    else
    {
      DEBUG ((EFI_D_INFO, "PchS3Peim S3 Data Hob located.  Proceed with Dispatch.\n"));

      ///
      /// Setup the DispatchArray variable to point at the Hob
      ///
      DispatchArray = (EFI_PCH_S3_DISPATCH_ARRAY *)(VOID *)&GuidHob->S3DispatchDataArray;
    }
  }

  DEBUG ((EFI_D_INFO, "Dispatch Array Located -> 0x%x\n", DispatchArray));
  DEBUG ((EFI_D_INFO, "Dispatch Item Located (Current NextDispatchItem entry) -> 0x%x\n", DispatchArray->NextDispatchItem));

  DispatchItem = (EFI_PCH_S3_DISPATCH_ITEM *)DispatchArray->NextDispatchItem;

  DEBUG ((EFI_D_INFO, "Dispatch Item Type -> 0x%x\n", DispatchItem->ItemType.Value));

  ///
  /// Calculate the size required;
  /// ** Always round up to be 8 byte aligned as the script is initially created from 64-bit code in DXE
  ///
  TypeSize = QWORD_ALIGNED_SIZE (EFI_PCH_S3_DISPATCH_ITEM_TYPE);

  switch (DispatchItem->ItemType.Value) {
  case PchS3ItemTypeSendCodecCommand:
    ParameterSize = QWORD_ALIGNED_SIZE(EFI_PCH_S3_PARAMETER_SEND_CODEC_COMMAND);
    Status        = PchS3SendCodecCommand ((EFI_PCH_S3_PARAMETER_SEND_CODEC_COMMAND *) &DispatchItem->Parameter);
    break;

  case PchS3ItemTypeInitPcieRootPortDownstream:
    ParameterSize = QWORD_ALIGNED_SIZE(EFI_PCH_S3_PARAMETER_INIT_PCIE_ROOT_PORT_DOWNSTREAM);
    Status        = PchS3InitPcieRootPortDownstream ((EFI_PCH_S3_PARAMETER_INIT_PCIE_ROOT_PORT_DOWNSTREAM *) &DispatchItem->Parameter);
    if (Status == EFI_NOT_FOUND) {
      ///
      /// EFI_NOT_FOUND is not an error here
      ///
      Status = EFI_SUCCESS;
    }
    break;

  case PchS3ItemTypePcieSetPm:
    ParameterSize = QWORD_ALIGNED_SIZE(EFI_PCH_S3_PARAMETER_PCIE_SET_PM);
    Status        = PchS3PcieSetPm ((EFI_PCH_S3_PARAMETER_PCIE_SET_PM *) &DispatchItem->Parameter);
    if (Status == EFI_NOT_FOUND) {
      ///
      /// EFI_NOT_FOUND is not an error here
      ///
      Status = EFI_SUCCESS;
    }
    break;

  case PchS3ItemTypeProgramIobp:
    ParameterSize = QWORD_ALIGNED_SIZE(EFI_PCH_S3_PARAMETER_PROG_IOBP);
    Status        = PchS3ProgramIobp ((EFI_PCH_S3_PARAMETER_PROG_IOBP *) &DispatchItem->Parameter);
    break;

  default:
    ParameterSize = 0;
    DEBUG ((EFI_D_INFO, "Parameter not found\n"));

    ASSERT (FALSE);
    break;
  }

  ///
  /// Advance the Execution Position
  ///
  Size = ParameterSize + TypeSize;
  DispatchArray->NextDispatchItem += Size;

  if ((UINTN)DispatchArray->NextDispatchItem > (UINTN)((UINT8*)DispatchArray + DispatchArray->MaximumBufferSize)) {
    ///
    /// We are beyond end, wrap for the next S3 resume path
    ///
    DispatchArray->NextDispatchItem = (UINT8*)DispatchArray + QWORD_ALIGNED_SIZE(EFI_PCH_S3_DISPATCH_ARRAY);
  }

  DEBUG ((DEBUG_INFO, "InitializePchS3Peim() End\n"));

  return Status;
}

/**
This routine is used to search SMRAM and get SmramCpuData point.

@param[in] PeiServices  - PEI services global pointer
@param[in] SmmAccessPpi - SmmAccess PPI instance

@retval SmramCpuData - The pointer of CPU information in SMRAM.
**/
EFI_STATUS
CreateS3DataHob (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Interface
  )
{
  EFI_SMRAM_DESCRIPTOR          *SmramRanges;
  UINTN                         SmramRangeCount;
  UINTN                         Size;
  EFI_STATUS                    Status;
  UINT32                        Address;
  EFI_PCH_S3_DISPATCH_ARRAY     *S3DispatchArray;
  PEI_SMM_ACCESS_PPI            *SmmAccessPpi;
  BOOLEAN                       Found;
  UINTN                         Index;
  S3_DATA_HOB                   *GuidHob;

  DEBUG ((EFI_D_INFO, "CreateS3DataHob() Start\n"));

  SmmAccessPpi = (PEI_SMM_ACCESS_PPI *)Interface;

  Found = FALSE;
  S3DispatchArray = NULL;

  ///
  /// Open all SMM regions
  ///
  Index = 0;
  do {
    Status = SmmAccessPpi->Open (PeiServices, SmmAccessPpi, Index);
    Index++;
  } while (!EFI_ERROR (Status));


  ///
  /// Get all SMRAM range
  ///
  Size = 0;
  Status = SmmAccessPpi->GetCapabilities (PeiServices, SmmAccessPpi, &Size, NULL);
  ASSERT (Status == EFI_BUFFER_TOO_SMALL);

  Status = PeiServicesAllocatePool (
    Size,
    (VOID **)&SmramRanges
    );
  ASSERT_EFI_ERROR (Status);

  Status = SmmAccessPpi->GetCapabilities (PeiServices, SmmAccessPpi, &Size, SmramRanges);
  ASSERT_EFI_ERROR (Status);

  Size /= sizeof (*SmramRanges);
  SmramRangeCount = Size;

  ///
  ///  Assume TSEG is the last range of SMRAM in SmramRanges
  ///
  SmramRanges += SmramRangeCount - 1;

  ///
  /// Search SMRAM on page alignment for the SMM PCH S3 SMRAM Data signature
  ///
  for (Address = (UINT32)(SmramRanges->CpuStart + SmramRanges->PhysicalSize - EFI_PAGE_SIZE);
    Address >= (UINT32)SmramRanges->CpuStart;
    Address -= EFI_PAGE_SIZE
    ) {
    S3DispatchArray = (EFI_PCH_S3_DISPATCH_ARRAY *)(UINTN)Address;
    if (S3DispatchArray->PchS3CustomScriptGuid.Data1 == gS3SupportSmramDataGuid.Data1) {
    if (CompareGuid (&S3DispatchArray->PchS3CustomScriptGuid, &gS3SupportSmramDataGuid)) {
      ///
      /// Find it
      ///
      Found = TRUE;
      break;
      }
    }
  }

  ///
  /// Close all SMM regions
  ///
  Index = 0;
  do {
    Status = SmmAccessPpi->Close (PeiServices, SmmAccessPpi, Index);
    Index++;
  } while (!EFI_ERROR (Status));


  if (!Found)
  {
    DEBUG ((EFI_D_INFO, "S3 Dispatch Data Not Found!\n"));
    return EFI_NOT_FOUND;
  }

  ///
  /// Generate Hob from SMRAM Data
  ///
  Status = (*PeiServices)->CreateHob (
    PeiServices,
    EFI_HOB_TYPE_GUID_EXTENSION,
    EFI_PAGE_SIZE,
    &GuidHob
    );

  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "PchS3Peim S3 Data Hob created:  Address - 0x%x\n", (UINTN)GuidHob));

    GuidHob->Header.Name = gS3DataHobGuid;
    CopyMem (&GuidHob->S3DispatchDataArray, S3DispatchArray, S3DispatchArray->MaximumBufferSize);

    ///
    /// Reset the NextDispatchItem to the beginning of the buffer for playback.
    ///
    S3DispatchArray = (EFI_PCH_S3_DISPATCH_ARRAY *)(UINTN)&GuidHob->S3DispatchDataArray;
    S3DispatchArray->NextDispatchItem = (UINT8 *)S3DispatchArray + QWORD_ALIGNED_SIZE(EFI_PCH_S3_DISPATCH_ARRAY);

  }

  DEBUG ((EFI_D_INFO, "CreateS3DataHob() End\n"));

  return EFI_SUCCESS;
}


#define AZALIA_MAX_LOOP_TIME  10
#define AZALIA_WAIT_PERIOD    100

/**
  Polling the Status bit

  @param[in] StatusReg            The regsiter address to read the status
  @param[in] PollingBitMap        The bit mapping for polling
  @param[in] PollingData          The Data for polling

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             Polling the bit map time out
**/
static
EFI_STATUS
CodecStatusPolling (
  IN      UINT32          StatusReg,
  IN      UINT16          PollingBitMap,
  IN      UINT16          PollingData
  )
{
  UINT32  LoopTime;

  for (LoopTime = 0; LoopTime < AZALIA_MAX_LOOP_TIME; LoopTime++) {
    if ((MmioRead16 (StatusReg) & PollingBitMap) == PollingData) {
      break;
    } else {
      PchPmTimerStall (AZALIA_WAIT_PERIOD);
    }
  }

  if (LoopTime >= AZALIA_MAX_LOOP_TIME) {
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}

/**
  Send Codec command on S3 resume

  @param[in] Parameter            Parameters passed in from DXE

  @retval EFI_DEVICE_ERROR        Device status error, operation failed
  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
PchS3SendCodecCommand (
  EFI_PCH_S3_PARAMETER_SEND_CODEC_COMMAND      *Parameter
  )
{
  UINT32      HdaBar;
  UINT32      *CodecCommandData;
  EFI_STATUS  Status;

  HdaBar            = Parameter->HdaBar;
  CodecCommandData  = &Parameter->CodecCmdData;

  DEBUG ((DEBUG_INFO, "Going to SendCodecCommand: %08x! \n", *CodecCommandData));
  Status = CodecStatusPolling (HdaBar + R_HDA_IRS, (UINT16) B_HDA_IRS_ICB, (UINT16) 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ICB bit is not zero before SendCodecCommand! \n"));
    return EFI_DEVICE_ERROR;
  }

  MmioWrite32 (HdaBar + R_HDA_IC, *CodecCommandData);
  MmioOr16 ((UINTN) (HdaBar + R_HDA_IRS), (UINT16) ((B_HDA_IRS_IRV | B_HDA_IRS_ICB)));

  Status = CodecStatusPolling (HdaBar + R_HDA_IRS, (UINT16) B_HDA_IRS_ICB, (UINT16) 0);
  if (EFI_ERROR (Status)) {
    MmioAnd16 ((UINTN) (HdaBar + R_HDA_IRS), (UINT16)~(B_HDA_IRS_ICB));
    DEBUG ((DEBUG_ERROR, "SendCodecCommand: SendCodecCommand:%08x fail! \n"));
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Perform Init Root Port Downstream devices on S3 resume

  @param[in] Parameter            Parameters passed in from DXE

  @retval EFI_STATUS
**/
EFI_STATUS
PchS3InitPcieRootPortDownstream (
  EFI_PCH_S3_PARAMETER_INIT_PCIE_ROOT_PORT_DOWNSTREAM     *Parameter
  )
{
  EFI_STATUS  Status;

  Status = PchPcieInitRootPortDownstreamDevices (
            Parameter->RootPortBus,
            Parameter->RootPortDevice,
            Parameter->RootPortFunc,
            Parameter->TempBusNumberMin,
            Parameter->TempBusNumberMax,
            NULL
            );
  ///
  /// Not checking the error status here - downstream device not present does not
  /// mean an error of this root port. Our return status of EFI_SUCCESS means this
  /// port is enabled and outer function depends on this return status to do
  /// subsequent initializations.
  ///
  return Status;
}

/**
  Perform Root Port Downstream devices PCIE ASPM and LTR override on S3 resume

  @param[in] Parameter            Parameters passed in from DXE

  @retval EFI_STATUS
**/
EFI_STATUS
PchS3PcieSetPm (
  EFI_PCH_S3_PARAMETER_PCIE_SET_PM    *Parameter
  )
{
  EFI_STATUS                    Status;
  PCH_PCIE_DEVICE_ASPM_OVERRIDE *DevAspmOverride;
  PCH_PCIE_DEVICE_LTR_OVERRIDE  *DevLtrOverride;
  PCH_PCIE_PWR_OPT              *S3PchPwrOptPcie;
  BOOLEAN                       L1SubstatesSupported;

  DevAspmOverride = (PCH_PCIE_DEVICE_ASPM_OVERRIDE *) (UINTN) Parameter->DevAspmOverrideAddr;
  DevLtrOverride  = (PCH_PCIE_DEVICE_LTR_OVERRIDE *) (UINTN) Parameter->DevLtrOverrideAddr;
  S3PchPwrOptPcie = (PCH_PCIE_PWR_OPT *) (UINTN) Parameter->PchPwrOptPcie;
  Status = PcieSetPm (
            Parameter->RootPortBus,
            Parameter->RootPortDevice,
            Parameter->RootPortFunc,
            Parameter->RootPortAspm,
            Parameter->NumOfDevAspmOverride,
            DevAspmOverride,
            Parameter->TempBusNumberMin,
            Parameter->TempBusNumberMax,
            Parameter->NumOfDevLtrOverride,
            DevLtrOverride,
            S3PchPwrOptPcie,
            &L1SubstatesSupported,
            Parameter->L1SubstatesConfig,
            Parameter->PolicyRevision,
            Parameter->FirstRPToSetPm,
            Parameter->L1SupportedInAllEnabledPorts,
            Parameter->ClkreqSupportedInAllEnabledPorts
            );
  ///
  /// Not checking the error status here - downstream device not present does not
  /// mean an error of this root port. Our return status of EFI_SUCCESS means this
  /// port is enabled and outer function depends on this return status to do
  /// subsequent initializations.
  ///
  return Status;
}

/**
  Perform PCH IOBP programming on S3 resume

  @param[in] Parameter            Parameters passed in from DXE

  @retval EFI_STATUS
**/
EFI_STATUS
PchS3ProgramIobp (
  EFI_PCH_S3_PARAMETER_PROG_IOBP      *Parameter
  )
{
  EFI_STATUS  Status;

  Status = ProgramIobp (
            Parameter->RootComplexBar,
            Parameter->Address,
            Parameter->AndMask,
            Parameter->OrMask
            );

  return Status;
}
