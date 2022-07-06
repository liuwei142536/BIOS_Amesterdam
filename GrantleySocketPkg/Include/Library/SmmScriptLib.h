//
// This file contains a 'Sample Driver' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may be modified by the user, subject to  
// the additional terms of the license agreement               
//
/*++

Copyright (c) 2009 - 2010 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

    SmmScriptSave.h
    
Abstract:

  This is an implementation of the BootScript at run time.  

--*/

#ifndef _SMM_SCRIPT_SAVE_H_
#define _SMM_SCRIPT_SAVE_H_

#include <PiDxe.h>
#include <Tbd\EfiBootScript.h>

typedef EFI_PHYSICAL_ADDRESS     EFI_SMM_SCRIPT_TABLE;

EFI_STATUS
EFIAPI
InitializeSmmScriptLib (
  IN  EFI_SYSTEM_TABLE     *SystemTable,
  IN  UINTN                SmmScriptTablePages,
  OUT EFI_PHYSICAL_ADDRESS *SmmScriptTableBase
  )
/*++

Routine Description:

  Intialize Boot Script table.
  
Arguments:

  SystemTable         -  Pointer to the EFI sytem table
  SmmScriptTablePages -  The expected ScriptTable page number
  SmmScriptTableBase  -  The returned ScriptTable base address

Returns:  

  EFI_OUT_OF_RESOURCES   -  No resource to do the initialization.
  EFI_SUCCESS            -  Function has completed successfully.
  
--*/
;

EFI_STATUS 
EFIAPI
SmmBootScriptCreateTable (
  IN OUT   EFI_SMM_SCRIPT_TABLE    *ScriptTable,
  IN       UINTN                   Type
  )
/*++

Routine Description:

  Create Boot Script table.
  
Arguments:

  ScriptTable  -  Pointer to the boot script table to create.
  Type         -  The type of table to creat.


Returns:  

  EFI_INVALID_PARAMETER  -  Invalid parameter detected.
  EFI_SUCCESS            -  Function has completed successfully.
  
--*/
;

EFI_STATUS 
EFIAPI
SmmBootScriptWrite (
  IN OUT   EFI_SMM_SCRIPT_TABLE    *ScriptTable,
  IN       UINTN                   Type,
  IN       UINT16                  OpCode,
  ...
  )
/*++

Routine Description:

  Write to boot script table. Format is defined in AcpiScritSave Protocol.
  Currently it supports MemWrite, IoWrite, PciCfgWrite.

Arguments:

  ScriptTable  -  Pointer to the script table to write to.
  Type         -  Not used.
  OpCode       -  Table write's Opcode.
  ...          -  Parameter of the OpCode.

Returns:

  EFI_INVALID_PARAMETER  -  Invalid parameter passed in detected.
  EFI_SUCCESS            -  Function completed successfully.
  Other                  -  Error occurred during execution.
  
--*/
;

EFI_STATUS
EFIAPI
SmmBootScriptCloseTable (
  IN EFI_SMM_SCRIPT_TABLE        ScriptTableBase,
  IN EFI_SMM_SCRIPT_TABLE        ScriptTablePtr,
  IN UINTN                       Type
  )
/*++

Routine Description:

  Close Boot Script table.
  
Arguments:

  ScriptTableBase  -  Pointer to the boot script table to create.
  ScriptTablePtr   -  Pointer to the script table to write to.
  Type             -  The type of table to creat.


Returns:  

  EFI_SUCCESS            -  Function has completed successfully.
  
--*/
;

#endif
