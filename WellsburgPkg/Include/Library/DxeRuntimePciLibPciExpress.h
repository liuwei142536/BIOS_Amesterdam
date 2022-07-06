/** @file
  Header file for the Dxe Runtime PCI Express library.

@copyright
  Copyright (c) 1999 - 2012 Intel Corporation. All rights reserved
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
#ifndef _DXE_RUNTIME_PCI_LIB_PCIEXPRESS_H_
#define _DXE_RUNTIME_PCI_LIB_PCIEXPRESS_H_

#include <Library/DebugLib.h>
#include <Library/IoLib.h> 
#include <Uefi.h> 
#include <Uefi/UefiSpec.h> 
#include <Library/BaseMemoryLib.h> 
#include <Library/UefiRuntimeLib.h> 
#include <Library/UefiRuntimeServicesTableLib.h> 
#include <Library/UefiBootServicesTableLib.h> 

/**
  Constructor for Pci library. Register VirtualAddressNotifyEvent() notify function
  It will ASSERT() if that operation fails

  @param[in] None

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
EFIAPI
PciLibConstructor (
  VOID
  );

/**
  Register memory space
  If StartAddress > 0x0FFFFFFF, then ASSERT().
  If SmPciLibAddressMapIndex) > PCI_LIB_ADDRESS_MAP_MAX_ITEM, then ASSERT().

  @param[in] Address              Starting address of the memory space
  @param[in] Length               Length of the memory space

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
EFIAPI
PciLibRegisterMemory (
  IN UINTN   Address,
  IN UINTN   Length
  );

#endif
