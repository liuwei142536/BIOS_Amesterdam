/** @file

  Header file for PCH common Initialization Driver.

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
#ifndef _PCH_INIT_COMMON_DRIVER_H_
#define _PCH_INIT_COMMON_DRIVER_H_
//AptioV server override start: dynamic MMCFG base change support.
#ifdef DYNAMIC_MMCFG_BASE_FLAG 
#include "PchAccess.h" //AptioV server override: Support for Dynamic MMCFG. Do not USe PCIe PCD in SMM since PCD SMM is unavailable.
#endif
//AptioV server override end: dynamic MMCFG base change support.
#include <Library/PchPciExpressHelpersLib.h>
#include <Protocol/PchPlatformPolicy.h>
#ifdef USB_PRECONDITION_ENABLE_FLAG
#include "PchUsbPrecondition.h"
#endif  // USB_PRECONDITION_ENABLE_FLAG

#define PCH_INIT_COMMON_SCRIPT_IO_WRITE(TableName, Width, Address, Count, Buffer) \
          S3BootScriptSaveIoWrite(Width, Address, Count, Buffer)

#define PCH_INIT_COMMON_SCRIPT_IO_READ_WRITE(TableName, Width, Address, Data, DataMask) \
          S3BootScriptSaveIoReadWrite(Width, Address, Data, DataMask)

#define PCH_INIT_COMMON_SCRIPT_MEM_WRITE(TableName, Width, Address, Count, Buffer) \
          S3BootScriptSaveMemWrite(Width, Address, Count, Buffer)

#define PCH_INIT_COMMON_SCRIPT_MEM_READ_WRITE(TableName, Width, Address, Data, DataMask) \
          S3BootScriptSaveMemReadWrite(Width, Address, Data, DataMask)

#define PCH_INIT_COMMON_SCRIPT_PCI_CFG_WRITE(TableName, Width, Address, Count, Buffer) \
          S3BootScriptSavePciCfgWrite(Width, Address, Count, Buffer)

#define PCH_INIT_COMMON_SCRIPT_PCI_CFG_READ_WRITE(TableName, Width, Address, Data, DataMask) \
          S3BootScriptSavePciCfgReadWrite(Width, Address, Data, DataMask)

#define PCH_INIT_COMMON_SCRIPT_STALL(TableName, Duration) SCRIPT_STALL (TableName, Duration)

#define PCH_INIT_COMMON_SCRIPT_SAVE_IOBP_S3_ITEM(RootComplexBar, Address, AndMask, OrMask) \
          SetProgramIobpS3Item(RootComplexBar, Address, AndMask, OrMask)

#ifdef USB_PRECONDITION_ENABLE_FLAG
///
/// Execute function when running in PEI
/// It is always FALSE for DXE phase check
///
#define USB_RUN_IN_PEI FALSE

///
/// Execute function when running in DXE
///
#define USB_RUN_IN_DXE TRUE

///
/// USB precondition policy check
///
#define USB_PRECONDITION_POLICY_SUPPORT(UsbPolicy) \
          ((UsbPolicy)->UsbPrecondition)

#endif  // USB_PRECONDITION_ENABLE_FLAG

/**
  Set an PCH IOBP programming S3 dispatch item, this function may assert if any error happend

  @param[in] RootComplexBar       RootComplexBar value of this PCH device
  @param[in] Address              Address of the IOBP register block
  @param[in] AndMask              Mask to AND with the register
  @param[in] OrMask               Mask to OR with the register

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    Out of resources
  @retval EFI_INVALID_PARAMETER   Invalid parameter
  @retval EFI_NOT_FOUND           Protocol interface not found
**/
EFI_STATUS
SetProgramIobpS3Item (
  IN UINT32               RootComplexBar,
  IN UINT32               Address,
  IN UINT32               AndMask,
  IN UINT32               OrMask
  );

#endif
