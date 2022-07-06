/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c) 1999 - 2013 Intel Corporation.  All rights reserved.

This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  MiscOemType0x82Function.c
  
Abstract: 

  The function that processes the Smbios data type 0x82 before they
  are submitted to Data Hub

--*/

#include "MeSmbios.h"
#include <PiDxe.h>
#include "SetupVariable.h"
#include "Guid/MeBiosExtensionSetup.h"
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>

CHAR16   gEfiMeBiosExtensionSetupName[] = EFI_ME_BIOS_EXTENSION_SETUP_VARIABLE_NAME;

MISC_SUBCLASS_TABLE_FUNCTION(MiscOemType0x82)
/*++
Description:

  This function makes boot time changes to the contents of the
  MiscOemType0x82Data.

Parameters:

  RecordType
    Type of record to be processed from the Data Table.
    mMiscSubclassDataTable[].RecordType

  RecordLen
    pointer to the size of static RecordData from the Data Table.
    mMiscSubclassDataTable[].RecordLen

  RecordData
    Pointer to copy of RecordData from the Data Table.  Changes made
    to this copy will be written to the Data Hub but will not alter
    the contents of the static Data Table.

  LogRecordData
    Set *LogRecordData to TRUE to log RecordData to Data Hub.
    Set *LogRecordData to FALSE when there is no more data to log.

Returns:

  EFI_SUCCESS
    All parameters were valid and *RecordData and *LogRecordData have
    been set.

  EFI_UNSUPPORTED
    Unexpected RecordType value.

  EFI_INVALID_PARAMETER
    One of the following parameter conditions was true:
      RecordLen was NULL.
      RecordData was NULL.
      LogRecordData was NULL.
--*/
{
  static BOOLEAN                            Done = FALSE;
  EFI_STATUS                                Status;
  UINT32                                    Attributes;
  UINTN                                     DataSize;
  ME_CAP                                    MeCapabilities;
  UINT32                                    MeEnabled;
  ME_BIOS_EXTENSION_SETUP                   MeBiosExtensionSetup;
  EFI_HECI_PROTOCOL                         *Heci;

  //
  // First check for invalid parameters.
  //
  if ((RecordLen == NULL) || (RecordData == NULL) || (LogRecordData == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Then check for unsupported RecordType.
  //
  if (RecordType != EFI_MISC_OEM_TYPE_0x82_RECORD_NUMBER) {
    return EFI_UNSUPPORTED;
  }

  //
  // Is this the first time through this function?
  //
  if (!Done) {
    //
    // Yes, this is the first time.  Inspect/Change the contents of the
    // RecordData structure.
    //
    //
    // Locate HECI protocol to use it for ME info, HECI messaging
    // If HECI not found register callback for it to update data
    //
    Status = gBS->LocateProtocol (
                  &gEfiHeciProtocolGuid,
                  NULL,
                  &Heci
                  );
    if (!(EFI_ERROR(Status))) {
      ZeroMem (&MeCapabilities, sizeof (ME_CAP));
      MeEnabled = *((volatile UINT32 *)(UINTN)((PciMeRegBase) + FW_STATUS_REGISTER));
      //
      // Make sure Me is in normal mode & hasn't any error
      //
      if ((MeEnabled & 0xFF000) == 0) {
        Status = HeciGetMeFwInfo(&MeCapabilities);
      }

      if (MeCapabilities.IntelAmtFw ==1) {
        ((EFI_MISC_OEM_TYPE_0x82*)RecordData)->AmtSupported = 1; 
      }
      else {
        ((EFI_MISC_OEM_TYPE_0x82*)RecordData)->AmtSupported = 0; 
      }

      Attributes  = EFI_VARIABLE_NON_VOLATILE;
      DataSize = sizeof (ME_BIOS_EXTENSION_SETUP);
      Status = gST->RuntimeServices->GetVariable (  
        gEfiMeBiosExtensionSetupName,
        &gEfiMeBiosExtensionSetupGuid,
        &Attributes,
        &DataSize,
        &MeBiosExtensionSetup
      );

      if (!EFI_ERROR(Status)) {
        if (MeCapabilities.IntelAmtFw == 1) {
          if (MeBiosExtensionSetup.PlatformMngSel & MNT_ON) {
            ((EFI_MISC_OEM_TYPE_0x82*)RecordData)->AmtEnabled = 1;
          } else {
            ((EFI_MISC_OEM_TYPE_0x82*)RecordData)->AmtEnabled = 0;
          }
        
        if (MeBiosExtensionSetup.AmtSolIder & IDER_ENABLE) {
          ((EFI_MISC_OEM_TYPE_0x82*)RecordData)->IderEnabled = 1;
        } else {
          ((EFI_MISC_OEM_TYPE_0x82*)RecordData)->IderEnabled = 0;
        }
  
        if (MeBiosExtensionSetup.AmtSolIder & SOL_ENABLE) {
          ((EFI_MISC_OEM_TYPE_0x82*)RecordData)->SolEnabled = 1;
        } else {
          ((EFI_MISC_OEM_TYPE_0x82*)RecordData)->SolEnabled = 0;
        }
        ((EFI_MISC_OEM_TYPE_0x82*)RecordData)->NetworkEnabled = 1;  // If AMT enabled, LAN is always enabled

        } 
      
        if (MeCapabilities.IntelKVM) {
          if (MeBiosExtensionSetup.KvmEnable & KVM_ENABLE) {
            ((EFI_MISC_OEM_TYPE_0x82*)RecordData)->KvmEnabled = 1;
          } else {
            ((EFI_MISC_OEM_TYPE_0x82*)RecordData)->KvmEnabled = 0;
          }
        } 
      }
    }

    Done = TRUE;
    *LogRecordData = TRUE;
  } else {
    //
    // No, this is the second time.  Reset the state of the Done flag
    // to FALSE and tell the data logger that there is no more data
    // to be logged for this record type.  If any memory allocations
    // were made by earlier passes, they must be released now.
    //
    Done = FALSE;
    *LogRecordData = FALSE;
  }

  return EFI_SUCCESS;
}

/* eof - MiscOemType0x82Function.c */
