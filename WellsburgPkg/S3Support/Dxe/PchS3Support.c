/** @file
  This is the driver that implements the PCH S3 Support protocol

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

#include "PchS3Support.h"
#include "S3SupportHob.h"
#include <Protocol/Spi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include "Token.h" //AptioV server override: S3 support on AztecCity
///
/// Global Variables
///
EFI_HANDLE                      mImageHandle;
EFI_PCH_S3_SUPPORT_PROTOCOL     mPchS3SupportProtocol;
EFI_PCH_S3_SUPPORT_SMM_PROTOCOL mPchS3SupportSmmProtocol;
UINT32                          mPchS3ImageEntryPoint;
EFI_PCH_S3_DISPATCH_ARRAY       *mPchS3CustomDispatchScript;

//
// GUID Declarations
//
extern EFI_GUID gS3SupportHobGuid;
extern EFI_GUID gS3SupportSmramDataGuid;

//
// Functions
//

/**
  PCH S3 support driver entry point

  @param[in] ImageHandle          Handle for the image of this driver
  @param[in] SystemTable          Pointer to the EFI System Table

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
PchS3SupportEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "PchS3SupportEntryPoint() Start\n"));
  mImageHandle = ImageHandle;
#ifdef EFI_S3_RESUME //AptioV server override: S3 support on AztecCity
  ///
  /// Initialize the Boot Services memory for the Dispatch Script Array
  ///
  Status = InitializePchS3CustomScriptMemory ();
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((EFI_D_INFO, "Dispatch Script Array Space initialized.\n"));

  ///
  /// Retrieve the PCH S3 Support PEIM entry point and load it into the Module variable
  ///
  Status = LoadPchS3ImageEntryPoint (&mPchS3ImageEntryPoint);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((EFI_D_INFO, "PCH S3 Image Entry Point intialized.\n"));
#endif //AptioV server override: S3 support on AztecCity

  ///
  /// Initialize and Install the PCH S3 Support and PCH S3 SMM Support protocol
  ///
  mPchS3SupportSmmProtocol.DispatchArray = mPchS3CustomDispatchScript;
  mPchS3SupportSmmProtocol.ProtocolSize = 1;  // Allocate one page
  mPchS3SupportProtocol.SetDispatchItem = PchS3SetDispatchItem;
  mPchS3SupportProtocol.ReadyToLock = S3SupportReadyToLock;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mImageHandle,
                  &gEfiPchS3SupportProtocolGuid,
                  &mPchS3SupportProtocol,
                  &gEfiPchS3SupportSmmProtocolGuid,
                  &mPchS3SupportSmmProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "PchS3SupportEntryPoint() End\n"));

  return Status;
}

/**
  Set an item to be dispatched at S3 resume time. This will initially create a Script
  entry in Boot Services memory.  At the same time, the entry point of the PCH S3 support
  image is returned to be used in subsequent boot script save calls.

  @param[in] This                       Pointer to the protocol instance.
  @param[in] DispatchItem               The item to be dispatched.
  @param[out] S3DispatchEntryPoint      The entry point of the PCH S3 support image.

  @retval EFI_STATUS                    Successfully completed.
  @retval EFI_OUT_OF_RESOURCES          Out of resources.
**/
EFI_STATUS
EFIAPI
PchS3SetDispatchItem (
  IN     EFI_PCH_S3_SUPPORT_PROTOCOL   *This,
  IN     EFI_PCH_S3_DISPATCH_ITEM      *InputDispatchItem,
  OUT    EFI_PHYSICAL_ADDRESS          *S3DispatchEntryPoint
  )
{
  EFI_STATUS  Status;
  UINT32      TypeSize;
  UINT32      ParameterSize;
  UINT32      Size;
  UINT8       *CurrentPos;

  DEBUG ((DEBUG_INFO, "PchS3SetDispatchItem() Start\n"));

  Status = EFI_SUCCESS;
  ///
  /// Calculate the size required;
  /// ** Always round up to be 8 byte aligned as the script is initially created from 64-bit code in DXE
  ///
  switch (InputDispatchItem->ItemType.Value) {
  case PchS3ItemTypeSendCodecCommand:
    ParameterSize = QWORD_ALIGNED_SIZE (EFI_PCH_S3_PARAMETER_SEND_CODEC_COMMAND);
    break;

  case PchS3ItemTypeInitPcieRootPortDownstream:
    ParameterSize = QWORD_ALIGNED_SIZE (EFI_PCH_S3_PARAMETER_INIT_PCIE_ROOT_PORT_DOWNSTREAM);
    break;

  case PchS3ItemTypePcieSetPm:
    ParameterSize = QWORD_ALIGNED_SIZE (EFI_PCH_S3_PARAMETER_PCIE_SET_PM);
    break;

  case PchS3ItemTypeProgramIobp:
    ParameterSize = QWORD_ALIGNED_SIZE(EFI_PCH_S3_PARAMETER_PROG_IOBP);
    break;

  default:
    DEBUG ((EFI_D_INFO, "Unrecognized Custom Dispatch Type\n"));
    return EFI_UNSUPPORTED; //EDKII implementarion // KR: check callers for EFI_INVALID_PARAMETER
  }
  ///
  /// Round up TypeSize to be 8 byte aligned
  ///
  TypeSize = QWORD_ALIGNED_SIZE (EFI_PCH_S3_DISPATCH_ITEM_TYPE);

  ///
  /// Total size is TypeSize + ParameterSize
  ///
  Size = TypeSize + ParameterSize;

  if (mPchS3CustomDispatchScript->BufferSpaceRemaining < Size) {
    DEBUG ((EFI_D_INFO, "Space remaining in Dispatch Script buffer is too small\n"));
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  if (mPchS3CustomDispatchScript->NextDispatchItem == NULL) {
    DEBUG ((EFI_D_INFO, "S3 Support Protocol has been unregistered.  Error.\n"));
    ASSERT (FALSE);
    return EFI_ACCESS_DENIED;
  }

  ///
  /// Store the dispatch type and dispatch parameter
  ///
  CurrentPos  = mPchS3CustomDispatchScript->NextDispatchItem;
  *(EFI_PCH_S3_DISPATCH_ITEM_TYPE *)CurrentPos = InputDispatchItem->ItemType.Value;
  CurrentPos += TypeSize;
  CopyMem (CurrentPos, InputDispatchItem->Parameter, ParameterSize);

  ///
  /// Move the pointer to the NextDispatchItem ahead to free space in our buffer
  /// and decrement the space remaining data
  ///
  mPchS3CustomDispatchScript->NextDispatchItem += Size;
  mPchS3CustomDispatchScript->BufferSpaceRemaining -= Size;

  ///
  /// Return the S3 Image's entry point
  ///
  *S3DispatchEntryPoint = mPchS3ImageEntryPoint;

  DEBUG ((DEBUG_INFO, "PchS3SetDispatchItem() End\n"));

  return Status;
}

/**
  Perform the EFI_PCH_S3_SUPPORT_SMM_PROTOCOL IO Trap to invoke DispatchArray data copy and
  IO Trap Unregister.

  @param[in] This                       Pointer to the protocol instance.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
EFIAPI
S3SupportReadyToLock(
  IN    EFI_PCH_S3_SUPPORT_PROTOCOL   *This
  )
{
#ifdef EFI_S3_RESUME //AptioV server override: S3 support on AztecCity
  EFI_STATUS                        Status;


  DEBUG ((EFI_D_INFO, "S3SupportExitPmAuthCallback() Start\n"));

  Status = EFI_SUCCESS;

  DEBUG ((EFI_D_INFO, "Invoke the S3 Support IO Trap: 0x%x\n", mPchS3SupportSmmProtocol.PchS3SupportIoTrap));

  ///
  /// Invoke the SMM IO Trap Handler for invoking the data copy to SMRAM and unregistration of the IO Trap
  ///
  IoWrite32 (mPchS3SupportSmmProtocol.PchS3SupportIoTrap, 0);

  if (mImageHandle != NULL)
  {
    DEBUG ((EFI_D_INFO, "Uninstall the S3 Support Protocol\n", mPchS3SupportSmmProtocol.PchS3SupportIoTrap));

    Status = gBS->UninstallMultipleProtocolInterfaces (
                  mImageHandle,
                  &gEfiPchS3SupportProtocolGuid,
                  &mPchS3SupportProtocol,
                  NULL
                  );
    ASSERT_EFI_ERROR (Status);
  }
  DEBUG ((EFI_D_INFO, "S3SupportExitPmAuthCallback() End\n"));

  return Status;
}

/**
  Initialize the Pch S3 Custom Script memory area.  This will later be transferred to SMRAM.

  @param[in] VOID

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_OUT_OF_RESOURCES          Not enough space was available to allocate for the BS memory required.
**/
EFI_STATUS
InitializePchS3CustomScriptMemory (
  VOID
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  Address;

  ///
  /// Allocate Boot Services memory for the initial copy of the PCH S3 Custom Dispatch Script
  ///
  Status = (gBS->AllocatePool)(
    EfiBootServicesData,
    EFI_PAGE_SIZE,
    &(VOID *)Address);
  if (!EFI_ERROR (Status)) {

    mPchS3CustomDispatchScript = (EFI_PCH_S3_DISPATCH_ARRAY *)(UINTN)Address;

    ///
    /// Initialize the DispatchScriptArray
    /// Ensure to account for the HOB space that will be needed for moving the data from SMRAM to normal
    /// memory during S3 resume in the MaximumBufferSize parameter.
    ///
    mPchS3CustomDispatchScript->PchS3CustomScriptGuid = gS3SupportSmramDataGuid;
    mPchS3CustomDispatchScript->MaximumBufferSize = EFI_PAGE_SIZE - QWORD_ALIGNED_SIZE(EFI_HOB_GUID_TYPE);
    mPchS3CustomDispatchScript->BufferSpaceRemaining = mPchS3CustomDispatchScript->MaximumBufferSize - QWORD_ALIGNED_SIZE(EFI_PCH_S3_DISPATCH_ARRAY);
    mPchS3CustomDispatchScript->NextDispatchItem = (UINT8*)mPchS3CustomDispatchScript + QWORD_ALIGNED_SIZE(EFI_PCH_S3_DISPATCH_ARRAY);
  }

  return Status;
}

/**
  Load the entry point address of the PCHS3Peim from the HOB that it generated during the PEI phase of POST

  @param[out] ImageEntryPoint     The ImageEntryPoint after success loading

  @retval EFI_STATUS
**/
EFI_STATUS
LoadPchS3ImageEntryPoint (
  OUT   UINT32          *ImageEntryPoint
  )
{
  EFI_STATUS                      Status;
  S3_SUPPORT_HOB                  *S3SupportHob;
  EFI_SPI_DATA_PROTOCOL           *SpiDataInterface;

  DEBUG ((EFI_D_INFO, "LoadPchS3ImageEntryPoint() Start\n"));

  Status = EFI_SUCCESS;
  S3SupportHob = NULL;
  *ImageEntryPoint = 0;

  //
  // Search for the S3SupportHob
  //
  S3SupportHob = GetFirstGuidHob(&gS3SupportHobGuid);
  if (S3SupportHob == NULL) {
    DEBUG ((EFI_D_INFO, "S3SupportHob not found.\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Find the SPI protocol and save the pointer.
  //
  Status = gBS->LocateProtocol (&gEfiSpiDataProtocolGuid, NULL, &SpiDataInterface);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "ERROR - Spi LocateProtocol failed!\n"));
    return Status;
  }

  ///
  /// If the PCH S3 PEIM is not located in flash, fail
  ///
  if (S3SupportHob->PchS3PeimEntryPoint < SpiDataInterface->BiosStartMemoryAddress ||
      S3SupportHob->PchS3PeimEntryPoint > SpiDataInterface->BiosStartMemoryAddress + SpiDataInterface->BiosSize)
  {
    DEBUG ((EFI_D_INFO, "PchS3Image is NOT located in Flash. Current Entry Point: %x\n", S3SupportHob->PchS3PeimEntryPoint));

    ASSERT(FALSE);
    return EFI_SECURITY_VIOLATION;
  }

  // Load the HOB data from PEI execution which contains the entry point of the PCHS3Peim from Flash
  *ImageEntryPoint = S3SupportHob->PchS3PeimEntryPoint;

  DEBUG ((EFI_D_INFO, "PchS3Image is Located in Flash at Entry Point: %x\n", S3SupportHob->PchS3PeimEntryPoint));
  DEBUG ((EFI_D_INFO, "LoadPchS3ImageEntryPoint() End\n"));

  return Status;
#else //AptioV server override: S3 support on AztecCity
  return EFI_SUCCESS;
#endif  //AptioV server override: S3 support on AztecCity
}
