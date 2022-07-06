/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

Copyright (c) 2009-2010 Intel Corporation.  All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license.  Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:
  IsPlatformSupportWhea.c

Abstract:
  IsPlatformSupportWhea Protocol driver, should be ported for platform.

---------------------------------------------------------------------------------*/

#include <Library\Debuglib.h>
#include <Library\UefiBootServicesTableLib.h>
#include <Library\UefiRuntimeServicesTableLib.h>


#include <Guid\SetupVariable.h>
#include <Protocol\IsPlatformSupportWhea.h>


BOOLEAN
IsSystemSupportWhea (
  IN EFI_IS_PLATFORM_SUPPORT_WHEA_PROTOCOL          *This
  );

STATIC
EFI_IS_PLATFORM_SUPPORT_WHEA_PROTOCOL  IsPlatformSupportWheaInst = \
{
    WheaNotSupported,
    0,
    &IsSystemSupportWhea
};


EFI_STATUS
InstallIsPlatformSupportWhea(
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
/*++

Routine Description: 

  This function will install IsPlatformSupportWhea Protocol if the platform supports Whea.
  The IsPlatformSupportWhea will be consumed by core whea module 

Arguments:  

  ImageHandle   EFI_HANDLE: A handle for the image that is initializing this driver
  SystemTable   EFI_SYSTEM_TABLE: A pointer to the EFI system table        

Returns:  

  EFI_SUCCESS:              Driver initialized successfully
  EFI_LOAD_ERROR:           Failed to Initialize or has been loaded 
  EFI_OUT_OF_RESOURCES:     Could not allocate needed resources

--*/
{
  EFI_STATUS                  Status;
  EFI_HANDLE                  Handle;
  SYSTEM_CONFIGURATION        SetupData;          
  UINTN                      VarSize;
  
  IsPlatformSupportWheaInst.WheaSupport = WheaNotSupported;

  //
  // Get the whea setup variable from setup.
  //
  //If Whea Support is not enabled, just return EFI_SUCCESS;
  //If yes, install EFI_IS_PLATFORM_SUPPORT_WHEA_PROTOCOL for WHEA core module to consume

  VarSize   = sizeof(SYSTEM_CONFIGURATION); 

  // AptioV server override: Changed variable name to Intelsetup
 Status = gRT->GetVariable(
    L"IntelSetup",
    &gEfiSetupVariableGuid,
    NULL,
    &VarSize,
    &SetupData);

if( EFI_ERROR(Status)) {
        SetupData.WheaSupport = 0;
  }

//  ASSERT_EFI_ERROR (Status);
  DEBUG((DEBUG_ERROR, "IsPlatformSupportWhea. wheaData.WheaSupport == %x \n",SetupData.WheaSupport));
  
  // if Whea is disabled, return Success
  if ( SetupData.WheaSupport == 0) { 
    return EFI_SUCCESS;
  } else if (SetupData.WheaSupport == 1) {

    // WHEA EINJ 5.0 setup
    if(SetupData.WheaEinj_05Extn)	IsPlatformSupportWheaInst.WheaAcpi5Support  = TRUE;
    else    IsPlatformSupportWheaInst.WheaAcpi5Support  = FALSE;

    IsPlatformSupportWheaInst.WheaSupport = WheaSupported;
    Handle = NULL;
    Status = gBS->InstallProtocolInterface (
                      &Handle,
                      &gEfiIsPlatformSupportWheaProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      &IsPlatformSupportWheaInst
                      );
    DEBUG((DEBUG_ERROR, "IsPlatformSupportWhea. InstallProtocolInterface.Status = %x \n",Status));
  }

  return Status;
}

BOOLEAN
IsSystemSupportWhea (
  IN EFI_IS_PLATFORM_SUPPORT_WHEA_PROTOCOL          *This
  )
{
  if(This->WheaSupport == WheaNotSupported) {
      return FALSE;
  }
  
  return TRUE;
}
