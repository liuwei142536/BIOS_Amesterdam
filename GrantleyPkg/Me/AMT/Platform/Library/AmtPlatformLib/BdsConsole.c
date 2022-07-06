/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2004 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  BdsConsole.c

Abstract:

  BDS Lib functions which contain all the code to connect console device

--*/

#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/GlobalVariable.h>
#include "Library/BdsLib.h"

EFI_STATUS
BdsLibUpdateConsoleVariable (
  IN  CHAR16                    *ConVarName,
  IN  EFI_DEVICE_PATH_PROTOCOL  *CustomizedConDevicePath,
  IN  EFI_DEVICE_PATH_PROTOCOL  *ExclusiveDevicePath
  )
/*++

Routine Description:

  This function update console variable based on ConVarName, it can
  add or remove one specific console device path from the variable

Arguments:

  ConVarName   - Console related variable name, ConIn, ConOut, ErrOut.

  CustomizedConDevicePath - The console device path which will be added to
                            the console variable ConVarName, this parameter
                            can not be multi-instance.

  ExclusiveDevicePath     - The console device path which will be removed
                            from the console variable ConVarName, this
                            parameter can not be multi-instance.

Returns:

  EFI_UNSUPPORTED         - Add or remove the same device path.

  EFI_SUCCESS             - Success add or remove the device path from
                            the console variable.

--*/
{
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL  *VarConsole;
  UINTN                     DevicePathSize;
  EFI_DEVICE_PATH_PROTOCOL  *Instance;
  EFI_DEVICE_PATH_PROTOCOL  *NewDevicePath;

  VarConsole      = NULL;
  DevicePathSize  = 0;
  NewDevicePath   = NULL;
  Status          = EFI_UNSUPPORTED;

  //
  // Notes: check the device path point, here should check
  // with compare memory
  //
  if (CustomizedConDevicePath == ExclusiveDevicePath) {
    return EFI_UNSUPPORTED;
  }
  //
  // Delete the ExclusiveDevicePath from current default console
  //
  VarConsole = BdsLibGetVariableAndSize (
                ConVarName,
                &gEfiGlobalVariableGuid,
                &DevicePathSize
                );

  if (ExclusiveDevicePath != NULL && VarConsole != NULL) {
    if (BdsLibMatchDevicePaths (VarConsole, ExclusiveDevicePath)) {

      Instance = GetNextDevicePathInstance (&VarConsole, &DevicePathSize);

      while (VarConsole != NULL) {
        if (CompareMem (
              Instance,
              ExclusiveDevicePath,
              DevicePathSize - sizeof (EFI_DEVICE_PATH_PROTOCOL)
              ) == 0) {
          //
          // Remove the match part
          //
          NewDevicePath = AppendDevicePathInstance (NewDevicePath, VarConsole);
          break;
        } else {
          //
          // Continue the next instance
          //
          NewDevicePath = AppendDevicePathInstance (NewDevicePath, Instance);
        }

        Instance = GetNextDevicePathInstance (&VarConsole, &DevicePathSize);
      }
      //
      // Reset the console variable with new device path
      //
      gRT->SetVariable (
            ConVarName,
            &gEfiGlobalVariableGuid,
            EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
            GetDevicePathSize (NewDevicePath),
            NewDevicePath
            );
    }
  }
  //
  // Try to append customized device path
  //
  VarConsole = BdsLibGetVariableAndSize (
                ConVarName,
                &gEfiGlobalVariableGuid,
                &DevicePathSize
                );

  if (CustomizedConDevicePath != NULL) {
    if (!BdsLibMatchDevicePaths (VarConsole, CustomizedConDevicePath)) {
      //
      // In the first check, the default console variable will be null,
      // just append current customized device path
      //
      VarConsole = AppendDevicePathInstance (VarConsole, CustomizedConDevicePath);

      //
      // Update the variable of the default console
      //
      gRT->SetVariable (
            ConVarName,
            &gEfiGlobalVariableGuid,
            EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
            GetDevicePathSize (VarConsole),
            VarConsole
            );
    }
  }

  return EFI_SUCCESS;
}
