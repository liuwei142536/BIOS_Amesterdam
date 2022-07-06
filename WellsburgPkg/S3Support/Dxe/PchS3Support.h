/** @file
  Header file for PCH S3 Support driver

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
#ifndef _PCH_S3_SUPPORT_DRIVER_H_
#define _PCH_S3_SUPPORT_DRIVER_H_

#include <Protocol/FirmwareVolume2.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/BaseMemoryLib.h> 
#include <Library/PeCoffLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
//
// Driver Produced Protocol Prototypes
//
#include <Protocol/PchS3Support.h>

#include "PchAccess.h"
#include "Library/PchPlatformLib.h"

extern EFI_GUID gPchS3ImageGuid;
extern EFI_GUID gEfiSpiProtocolGuid;
///
/// Function prototypes
///
/**
  Set an item to be dispatched at S3 resume time. At the same time, the entry point
  of the PCH S3 support image is returned to be used in subsequent boot script save
  call

  @param[in] This                       Pointer to the protocol instance.
  @param[in] InputDispatchItem          The item to be dispatched.
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
  );

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
  );

/**
  Initialize the Pch S3 Custom Script memory area.  This will later be transferred to SMRAM.

  @param[in] VOID

  @retval None
**/
EFI_STATUS
InitializePchS3CustomScriptMemory (
  VOID
  );

/**
  Load the PCH S3 Image into Efi Reserved Memory below 4G.

  @param[out] ImageEntryPoint     The ImageEntryPoint after success loading

  @retval EFI_STATUS
**/
EFI_STATUS
LoadPchS3ImageEntryPoint (
  OUT   UINT32          *ImageEntryPoint
  );

#endif
