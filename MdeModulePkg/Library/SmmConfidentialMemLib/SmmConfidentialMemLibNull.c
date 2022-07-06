//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.

  This file contains 'Framework Code' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may not be
  modified, except as allowed by additional terms of your license agreement.

**/

#include <PiSmm.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>

#include <Library/SmmConfidentialMemLib.h>
#include <Library/MemoryAllocationLib.h>


/**
  SmmAllocateConfidentialPool - Allocates SMM memory for confidential storage

  @param Size                   Requested size in bytes
  @param Buffer                 Buffer address

  @retval EFI_SUCCESS           Requested buffer was allocated successfully.
  @retval EFI_NOT_READY         Requested buffer could not be allocated.
  @retval EFI_INVALID_PARAMETER Requested buffer could not be allocated.
  @retval EFI_OUT_OF_RESOURCES  Requested buffer could not be allocated.

**/
EFI_STATUS
EFIAPI
SmmAllocateConfidentialPool (
  IN      UINTN  Size,
  IN OUT  VOID** Buffer
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if (Size == 0 || Buffer == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Invalid parameter\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  // Call to the default memory library instead of the confidential library.  
  //  This is a NULL library, the chipset needs to implement the corrent implementation 
  *Buffer = AllocatePool(Size);

  if (*Buffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((EFI_D_ERROR, "%a() - Allocate failed\n", __FUNCTION__));
  }

  return Status;
}

/**
  SmmFreeConfidentialPool -     Frees previously allocated SMM memory

  @param Buffer                 Buffer address

  @retval EFI_SUCCESS           Buffer was freed successfully.
  @retval EFI_NOT_STARTED       Buffer could not be freed.
  @retval EFI_INVALID_PARAMETER Buffer could not be freed.
  @retval EFI_NOT_FOUND         Buffer could not be freed.

**/
EFI_STATUS
EFIAPI
SmmFreeConfidentialPool (
  IN OUT  VOID**  Buffer
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if ((Buffer == NULL) || (*Buffer == NULL)) {
    DEBUG ((EFI_D_ERROR, "%a() - Invalid parameter\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }
  
  // Call to the default memory library instead of the confidential library.  
  //  This is a NULL library, the chipset needs to implement the current implementation 
  FreePool(*Buffer);

  return Status;
}

/**
  SmmConfidentialMemConstructor

  @param ImageHandle
  @param SystemTable

  @retval EFI_SUCCESS           Buffer was freed successfully.
  @retval EFI_OUT_OF_RESOURCES  Buffer could not be allocated.

**/
EFI_STATUS
EFIAPI
SmmConfidentialMemConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return EFI_SUCCESS;
}
