/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
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

    OemIioInit.c

Abstract:

    Oem Hooks file for IioInit Module

--*/

#include <Library/OemIioInit.h>
#include "OemIioInitCommon.h"

EFI_STATUS
GetSystemVariable(
    SYSTEM_CONFIGURATION  *SetupData
)
{
  UINTN         VarSize;

  //
  // Obtain the system configuration data
  //
  VarSize = sizeof (SYSTEM_CONFIGURATION);
  return gRT->GetVariable (
              L"IntelSetup", //AptioV Grangeville Override
              &gEfiSetupVariableGuid,
              NULL,
              &VarSize,
              SetupData
          );
}


EFI_STATUS
SetSystemVariable(
    SYSTEM_CONFIGURATION  *SetupData,
    UINT32                 VarSize
)
{
  return gRT->SetVariable(
            L"IntelSetup", // AptioV Grangeville Override
            &gEfiSetupVariableGuid,
		    SETUP_VARIABLE_ATTRIBUTES,
            VarSize,
            SetupData
        );
}

EFI_STATUS
LocateSmBus (
  VOID                    **SmBus
  )
{
  return gBS->LocateProtocol (&gEfiSmbusHcProtocolGuid, NULL, SmBus);
}

EFI_STATUS
EFIAPI
OemAllocate32BitMmio(
    IIO_GLOBALS            *IioGlobalData,
    UINT8                  Socket,
    EFI_HANDLE             ImageHandle,
    UINTN                  Alignment,
    UINT64                 Length,
    EFI_PHYSICAL_ADDRESS   *BaseAddress
  )
{
  EFI_STATUS    Status;
  
  *BaseAddress = IioGlobalData->IioVData.PciResourceMem32Limit[Socket] + 1;
  
  Status = gDS->AllocateMemorySpace(EfiGcdAllocateMaxAddressSearchTopDown,
               EfiGcdMemoryTypeMemoryMappedIo,
               Alignment,
               Length,
               BaseAddress,
               ImageHandle, 
               NULL);

  ASSERT( *BaseAddress >= IioGlobalData->IioVData.PciResourceMem32Base[Socket]
         && *BaseAddress <= IioGlobalData->IioVData.PciResourceMem32Limit[Socket]);
		 
  if ((*BaseAddress == 0) || ( (*BaseAddress & 0xffffffff00000000) != 0) )
  {
  	Status = EFI_UNSUPPORTED;
  }
  
  return Status; 
}

EFI_STATUS
EFIAPI
OemFreeMemorySpace (
   EFI_PHYSICAL_ADDRESS  BaseAddress,
   UINT64                Length
  )
{
  EFI_STATUS    Status;

  Status = gDS->FreeMemorySpace(BaseAddress, Length);

  return Status;  
}

EFI_STATUS
EFIAPI
OemAllocateSystemMemory(
    IIO_GLOBALS            *IioGlobalData,
    EFI_HANDLE             ImageHandle,
    UINTN                  Alignment,
    UINT64                 Length,
    EFI_PHYSICAL_ADDRESS   BelowRegion,
    VOID   **BaseAddress
)
{
  UINTN                 Pages;
  EFI_PHYSICAL_ADDRESS  Address;
  EFI_STATUS            Status;

  Pages = EFI_SIZE_TO_PAGES (Length);
  Address = BelowRegion;

  Status  = (gBS->AllocatePages) (
      AllocateAnyPages,
      EfiBootServicesData,
      Pages,
      &Address
  );

  *BaseAddress = (VOID *) (UINTN) Address;

  return Status;
}

EFI_STATUS
EFIAPI
OemFreeSystemMemorySpace (
    EFI_PHYSICAL_ADDRESS  BaseAddress,
    UINT64                Length
)
{
  EFI_STATUS            Status;
  UINTN                 Pages = EFI_SIZE_TO_PAGES (Length);

  Status = gBS->FreePages(BaseAddress, Pages);

  return Status;
}
