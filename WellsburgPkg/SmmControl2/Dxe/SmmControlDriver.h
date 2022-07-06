/** @file
  Header file for SMM Control Driver.

@copyright
  Copyright (c) 1999 - 2012 Intel Corporation. All rights reserved
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
#ifndef _SMM_CONTROL_DRIVER_H_
#define _SMM_CONTROL_DRIVER_H_

#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include <IndustryStandard/Pci.h>
//
// Driver private data
//
#include <Protocol/SmmControl2.h>
//AptioV server override start: dynamic MMCFG base change support.
#ifdef DYNAMIC_MMCFG_BASE_FLAG 
#include <PchSmmAccess.h> //AptioV server override: Support for Dynamic MMCFG. Do not USe PCIe PCD in SMM since PCD SMM is unavailable.
#else
#include <PchAccess.h>
#endif
//AptioV server override end: dynamic MMCFG base change support.
#include <Library/PchPlatformLib.h>

#define EFI_INTERNAL_POINTER  0x00000004

#define SMM_CONTROL_PRIVATE_DATA_SIGNATURE  SIGNATURE_32 ('i', '4', 's', 'c')

typedef struct {
  UINTN                     Signature;
  EFI_HANDLE                Handle;
  EFI_SMM_CONTROL2_PROTOCOL  SmmControl;
} SMM_CONTROL_PRIVATE_DATA;

#define SMM_CONTROL_PRIVATE_DATA_FROM_THIS(a) CR (a, SMM_CONTROL_PRIVATE_DATA, SmmControl, SMM_CONTROL_DEV_SIGNATURE)

//
// Prototypes
//

/**
  This is the constructor for the SMM Control protocol

  @param[in] ImageHandle          Handle for the image of this driver
  @param[in] SystemTable          Pointer to the EFI System Table

  @retval EFI_STATUS              Results of the installation of the SMM Control Protocol
**/
EFI_STATUS
EFIAPI
SmmControlDriverEntryInit (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

/**
  Trigger the software SMI

  @param[in] Data                 The value to be set on the software SMI data port

  @retval EFI_SUCCESS             Function completes successfully
**/
EFI_STATUS
EFIAPI
SmmTrigger (
  UINT8   Command, //AptioV Server override: Implementing as per PI 1.2 spec
  UINT8   Data
  );

/**
  Clear the SMI status

  @param[in] None

  @retval EFI_SUCCESS             The function completes successfully
  @retval EFI_DEVICE_ERROR        Something error occurred
**/
EFI_STATUS
EFIAPI
SmmClear (
  VOID
  );

/**
  This routine generates an SMI

  @param[in] This                       The EFI SMM Control protocol instance
  @param[in, out] ArgumentBuffer        The buffer of argument
  @param[in, out] ArgumentBufferSize    The size of the argument buffer
  @param[in] Periodic                   Periodic or not
  @param[in] ActivationInterval         Interval of periodic SMI

  @retval EFI Status                    Describing the result of the operation
  @retval EFI_INVALID_PARAMETER         Some parameter value passed is not supported
**/
EFI_STATUS
EFIAPI
Activate (
  IN CONST EFI_SMM_CONTROL2_PROTOCOL   * This,
  IN OUT  UINT8                        *ArgumentBuffer OPTIONAL,
  IN OUT  UINT8                        *ArgumentBufferSize OPTIONAL,
  IN      BOOLEAN                    Periodic OPTIONAL,
  IN      UINTN                      ActivationInterval OPTIONAL
  );

/**
  This routine clears an SMI

  @param[in] This                 The EFI SMM Control protocol instance
  @param[in] Periodic             Periodic or not

  @retval EFI Status              Describing the result of the operation
  @retval EFI_INVALID_PARAMETER   Some parameter value passed is not supported
**/
EFI_STATUS
EFIAPI
Deactivate (
  IN CONST EFI_SMM_CONTROL2_PROTOCOL    *This,
  IN  BOOLEAN                     Periodic OPTIONAL
  );


/**
  Disable all pending SMIs

  @param[in] None

  @retval None
**/
VOID
EFIAPI
DisablePendingSmis (
  VOID
  );

#endif
