/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2014   Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  IioInit.c

Abstract:

  Implement IIO initialization protocol to provide IIO initialization functions.
  This interfaces will be called by PciHostBridge driver in the PCI bus enumeration.

--*/

#include <Library/UefiDriverEntryPoint.h>
#include <Protocol/IioSystem.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IioEarlyInitializeLib.h>
#include <Library/IioLateInitializeLib.h>

EFI_IIO_SYSTEM_PROTOCOL       mIioSystemProtocol;
IIO_GLOBALS *IioGlobalData;          

EFI_STATUS
EFIAPI
IioInitEntry (
  IN EFI_HANDLE                 ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable
  )
/*++

Routine Description:

  Install the IIO Initialization

Arguments:

  ImageHandle  - ImageHandle of the loaded driver
  SystemTable  - Pointer to the BIOS System Table

Returns:

  Status
    EFI_SUCCESS           - Installed successfully
    Others                - Failed to install this protocol

--*/
{
  EFI_HANDLE  IioSystemProtocolHandle = NULL;
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_CPU_CSR_ACCESS_PROTOCOL   *CpuCsrAccess;

  //
  // Allocate Memory Pool for IIO Global Data Storage so that protocol can expose it
  //
  Status = gBS->AllocatePool ( EfiReservedMemoryType, sizeof (IIO_GLOBALS), (VOID **) &IioGlobalData );
  ASSERT_EFI_ERROR (Status);
  ZeroMem (IioGlobalData, sizeof (IIO_GLOBALS));

  //
  // Initialize Input, output, variable and Pre-link data
  //
  InitializeIioGlobalData(IioGlobalData);

  //
  // Build the IIO_SYSTEM_PROTOCOL driver instance for protocol publishing  
  //
  ZeroMem (&mIioSystemProtocol, sizeof (EFI_IIO_SYSTEM_PROTOCOL));
  mIioSystemProtocol.IioGlobalData = IioGlobalData;
  mIioSystemProtocol.IioGetCpuUplinkPort = IioGetCpuUplinkPort;
  Status = gBS->LocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &CpuCsrAccess);
  ASSERT_EFI_ERROR (Status);
  IioGlobalData->IioOutData.CpuCsrAccess = CpuCsrAccess;
  //
  // Install IIO System Protocol
  //
  Status = gBS->InstallProtocolInterface (
                    &IioSystemProtocolHandle,
                    &gEfiIioSystemProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mIioSystemProtocol
                    );
  ASSERT_EFI_ERROR (Status);

  //
  // Call IIO Early Entry (Link training, DFX init)
  //
  Status = IioEarlyInitializeEntry(mIioSystemProtocol.IioGlobalData);

  //
  // Call IIO Late Entry (PCIE port, IOAPIC, NTB, IOAT, VTD init, setup call backs for before OS handoff)
  //
  Status = IioLateInitializeEntry(mIioSystemProtocol.IioGlobalData, ImageHandle);

  return Status;
}
