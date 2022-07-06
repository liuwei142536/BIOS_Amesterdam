/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2011 - 2013, Intel Corporation. All rights reserved.
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

--*/
#include <Library/UefiBootServicesTableLib.h>
#include "Protocol/AmtWrapperProtocol.h"
#include "Library/MeTypeLib.h"

EFI_STATUS
EFIAPI
AmtBootEntry (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
// AptioV server override   UINTN i;
    EFI_STATUS Status;
    AMT_WRAPPER_PROTOCOL *pAmtWrapper = NULL;

#if defined(SPS_SUPPORT) && SPS_SUPPORT
    if (!MeTypeIsAmt()) {
      return EFI_UNSUPPORTED;
    }
#endif // SPS_SUPPORT

    //
    // Initialize EFI library
    //
//TBD    EfiInitializeDriverLib (ImageHandle, SystemTable);
    //InitAmiLib(ImageHandle, SystemTable);

    //Replace the following for loop with boot code
//AptioV server override start : this code no work. 
//    for(i=0; i<0xFF; i++)
//    	SystemTable->ConOut->OutputString( SystemTable->ConOut, L"Fake to Boot to Remote CD...\r\n" );
//AptioV server override end
    Status = gBS->LocateProtocol(&gEfiAmtWrapperProtocolGuid, NULL, &pAmtWrapper);
    if (pAmtWrapper != NULL) {
      //Status = gST->ConIn->Reset( gST->ConIn, FALSE );
      Status = pAmtWrapper->BdsBootViaAsf();
   }

   return EFI_SUCCESS;
}
