/** @file
  PCI Library using PC Express access.

@copyright
  Copyright (c) 2005 - 2012 Intel Corporation. All rights reserved
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
#include "Library/DxeRuntimePciLibPciExpress.h"

#include <Library/UefiRuntimeLib.h> 

#ifndef EFI_INTERNAL_POINTER 
#define EFI_INTERNAL_POINTER 0x04
#endif 


/**
  Assert the validity of a PCI address. A valid PCI address should contain 1's
  only in the low 28 bits.

  @param  A The address to validate.
**/
#define ASSERT_INVALID_PCI_ADDRESS(A) ASSERT (((A) &~0xfffffff) == 0)

/**
  PCI Express base address
**/
STATIC UINTN  mPciExpressBaseAddress;

/**
  Registered memory scope.
**/
typedef struct _REGISTERED_ADDRESS_MAP {
  UINTN PciAddress;
  UINTN Length;
  UINTN RuntimeAddress;
} REGISTERED_ADDRESS_MAP;

#define PCI_LIB_ADDRESS_MAP_MAX_ITEM  64

STATIC REGISTERED_ADDRESS_MAP mPciLibAddressMap[PCI_LIB_ADDRESS_MAP_MAX_ITEM];

STATIC UINTN                  mPciLibAddressMapIndex = 0;

/**
  Virtual address notify event
**/
STATIC EFI_EVENT              mVirtualNofityEvent;


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
  IN  UINTN   Address,
  IN  UINTN   Length
  )
{
  UINTN Index;

  ASSERT_INVALID_PCI_ADDRESS (Address);
  ASSERT (mPciLibAddressMapIndex < PCI_LIB_ADDRESS_MAP_MAX_ITEM);

  //
  //  If already registered
  //
  for (Index = 0; Index < mPciLibAddressMapIndex; Index++) {
    if (mPciLibAddressMap[Index].PciAddress == Address) {
      return EFI_SUCCESS;
    }
  }

  mPciLibAddressMap[mPciLibAddressMapIndex].PciAddress      = Address;
  mPciLibAddressMap[mPciLibAddressMapIndex].Length          = Length;
  mPciLibAddressMap[mPciLibAddressMapIndex].RuntimeAddress  = mPciExpressBaseAddress + Address;
  mPciLibAddressMapIndex++;

  return EFI_SUCCESS;
}

/**
  Virtual address nofity.
  The event handler changes PCIE base address to an virtual address.
  Starting address of registered memory scope is converted as well.

  @param[in] Event                The event that be siganlled when virtual address changed
  @param[in] Context              The pointer of the ESAL procedure instance

  @retval None
**/
VOID
EFIAPI
VirtualAddressNotifyEvent (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  UINTN Index;

  for (Index = 0; Index < PCI_LIB_ADDRESS_MAP_MAX_ITEM; Index++) {
    if (mPciLibAddressMap[Index].PciAddress != 0) {
      gRT->ConvertPointer (EFI_INTERNAL_POINTER, (VOID *) &(mPciLibAddressMap[Index].RuntimeAddress));
    }
  }

  gRT->ConvertPointer (EFI_INTERNAL_POINTER, (VOID *) &mPciExpressBaseAddress);
}

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
  )
{
  EFI_STATUS  Status;

  mPciExpressBaseAddress = (UINTN) PcdGet64 (PcdPciExpressBaseAddress);

  Status = gBS->CreateEvent (
                  EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE,
                  TPL_NOTIFY,
                  VirtualAddressNotifyEvent,
                  NULL,
                  &mVirtualNofityEvent
                  );
  ASSERT_EFI_ERROR (Status);

  ZeroMem (mPciLibAddressMap, sizeof (mPciLibAddressMap));

  return EFI_SUCCESS;
}
