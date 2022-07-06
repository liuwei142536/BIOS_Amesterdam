/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2010 - 2012, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license.  Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  AmtSupportLib

Abstract:

  This file include all platform action which can be customized
  by IBV/OEM.

--*/

//Aptio V Server override #include "Library/PlatformBdsLibNul/BdsPlatform.h"
#include "Library/AmtSupportLib.h"
//Aptio V Server override start
#include "Library/PrintLib.h"
#include "AMT/Platform/Dxe/AmtWrapperDxe/AmtWrapperDxe.h"
#include "Library/GenericBdsLib.h"
#include "Library/DebugLib.h"
#include "Library/UefiBootServicesTableLib.h"
#include "Library/UefiRuntimeServicesTableLib.h"
#include <Guid\GlobalVariable.h>
#include <Library/MemoryAllocationLib.h>
//Aptio V Server override end
#include <Library/PciLib.h>
#include "MeChipset.h"


BOOLEAN   mAssetFullConfig = FALSE;
EFI_EVENT *MeHotkeyTriggered = NULL;

//
// AMT Wrapper Protocol
//


VOID       *mRegistration;

PLATFORM_IDER_DEVICE_PATH gIderDevicePath = {
  gPciRootBridge,
  {
      HARDWARE_DEVICE_PATH,
      HW_PCI_DP,
      (UINT8)(sizeof(PCI_DEVICE_PATH)),
      (UINT8)((sizeof(PCI_DEVICE_PATH)) >> 8),
      2,
      0x16
  },
  gEndEntire
};

VOID
DeleteConIn (
  VOID   **Variable,
  UINTN  *VariableSize
  )
{
  *Variable     = NULL;
  *VariableSize = 0;
}

VOID
ForceSetup (
  VOID
  );
////////////////////////////////////////////
typedef
VOID
(* BDS_PROCESS_VARIABLE) (
  VOID  **Variable,
  UINTN *VariableSize
  );

/*CHAR16 *
EFIAPI
DevicePathToStr (
  IN EFI_DEVICE_PATH_PROTOCOL     *DevPath
  );*/

VOID
UpdateEfiGlobalVariable (
  CHAR16               *VariableName,
  EFI_GUID             *AgentGuid,
  BDS_PROCESS_VARIABLE ProcessVariable
  )
/*++

Routine Description:

  Generic function to update the console variable.
  Please refer to FastBootSupport.c for how to use it.

Arguments:

  VariableName    - The name of the variable to be updated
  AgentGuid       - The Agent GUID
  ProcessVariable - The function pointer to update the variable
                    NULL means to restore to the original value

--*/
{
  EFI_STATUS  Status;
  CHAR16      BackupVariableName[20];
  CHAR16      FlagVariableName[20];
  VOID        *Variable;
  VOID        *BackupVariable;
  VOID        *NewVariable;
  UINTN       VariableSize;
  UINTN       BackupVariableSize;
  UINTN       NewVariableSize;
  BOOLEAN     Flag;
  BOOLEAN     *FlagVariable;
  UINTN       FlagSize;
  CHAR16      *Str;

  ASSERT (StrLen (VariableName) <= 13);
  UnicodeSPrint (BackupVariableName, sizeof (BackupVariableName), L"%sBackup", VariableName);
  UnicodeSPrint (FlagVariableName, sizeof (FlagVariableName), L"%sModify", VariableName);
//Aptio V Server override start
  Variable       = BdsLibGetVariableAndSize (VariableName, &gEfiGlobalVariableGuid, &VariableSize);
  BackupVariable = BdsLibGetVariableAndSize (BackupVariableName, AgentGuid, &BackupVariableSize);
  FlagVariable   = BdsLibGetVariableAndSize (FlagVariableName, AgentGuid, &FlagSize);
//Aptio V Server override end
  if (ProcessVariable != NULL) {
    if (FlagVariable == NULL) {
      //
      // Last boot is normal boot
      // Set flag
      // BackupVariable <- Variable
      // Variable       <- ProcessVariable (Variable)
      //
      Flag   = TRUE;
      Status = gRT->SetVariable (
                      FlagVariableName,
                      AgentGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                      sizeof (Flag),
                      &Flag
                      );
      ASSERT_EFI_ERROR (Status);

      Status = gRT->SetVariable (
                      BackupVariableName,
                      AgentGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                      VariableSize,
                      Variable
                      );
      ASSERT ((Status == EFI_SUCCESS) || (Status == EFI_NOT_FOUND));


      NewVariable     = Variable;
      NewVariableSize = VariableSize;
      ProcessVariable (&NewVariable, &NewVariableSize);
      DEBUG ((EFI_D_ERROR, "============================%s============================\n", VariableName));
      Str = DevicePathToStr ((EFI_DEVICE_PATH_PROTOCOL *) Variable);    DEBUG ((EFI_D_ERROR, "O:%s\n", Str)); gBS->FreePool (Str);
      Str = DevicePathToStr ((EFI_DEVICE_PATH_PROTOCOL *) NewVariable); DEBUG ((EFI_D_ERROR, "N:%s\n", Str)); gBS->FreePool (Str);

      Status = gRT->SetVariable (
                      VariableName,
                      &gEfiGlobalVariableGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                      NewVariableSize,
                      NewVariable
                      );
      ASSERT ((Status == EFI_SUCCESS) || (Status == EFI_NOT_FOUND));

      if (NewVariable != NULL) {
        FreePool (NewVariable);
      }
    } else { // LastBootIsModifiedPtr != NULL
      //
      // Last Boot is modified boot
      //
    }
  } else {
    if (FlagVariable != NULL) {
      //
      // Last boot is modified boot
      // Clear LastBootIsModified flag
      // Variable       <- BackupVariable
      // BackupVariable <- NULL
      //
      Status = gRT->SetVariable (
                      FlagVariableName,
                      AgentGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                      0,
                      NULL
                      );
      ASSERT_EFI_ERROR (Status);

      Status = gRT->SetVariable (
                      VariableName,
                      &gEfiGlobalVariableGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                      BackupVariableSize,
                      BackupVariable
                      );
      ASSERT ((Status == EFI_SUCCESS) || (Status == EFI_NOT_FOUND));

      Status = gRT->SetVariable (
                      BackupVariableName,
                      AgentGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                      0,
                      NULL
                      );
      ASSERT ((Status == EFI_SUCCESS) || (Status == EFI_NOT_FOUND));
    } else { // LastBootIsModifiedPtr == NULL
      //
      // Last boot is normal boot
      //
    }
  }

  if (Variable != NULL) {
    FreePool (Variable);
  }

  if (BackupVariable != NULL) {
    FreePool (BackupVariable);
  }

  if (FlagVariable != NULL) {
    FreePool (FlagVariable);
  }
}
////////////////////////////////////////////

VOID
AmtUpdateConsoleVariable (
  VOID
  )
{
  EFI_STATUS               Status;
  AMT_WRAPPER_PROTOCOL     *AmtWrapper;
  EFI_DEVICE_PATH_PROTOCOL *SolDevicePath;
  EFI_GUID                 AmtConsoleVariableGuid = {
    0xd9aaf1e5, 0xcd14, 0x4312, 0x9c, 0xa4, 0x85, 0xc3, 0xa, 0xde, 0x43, 0xe8
  };

  Status = gBS->LocateProtocol (&gEfiAmtWrapperProtocolGuid, NULL, &AmtWrapper);
  if (!EFI_ERROR (Status))	{
    AmtWrapper->AmtWrapperInit (gImageHandle, gST);

    if (AmtWrapper->ActiveManagementConsoleLocked ()) {
      UpdateEfiGlobalVariable (L"ConIn", &AmtConsoleVariableGuid, DeleteConIn);
//	  BdsLibUpdateConsoleVariable
//	  UpdateSystemTableConsole
//	  ...EfiGlobalVariable
//	  EfiLibGetVariable
    } else {
      UpdateEfiGlobalVariable (L"ConIn", &AmtConsoleVariableGuid, NULL);
    }

    //
    // Add/Del AMT SOL device path to/from the ConIn, ConOut and StdErr variable when SOL is enabled/disabled.
    //
    if (AmtWrapper->AmtWrapperGet (GET_SOL_DP, (VOID **) &SolDevicePath)) {
      if (AmtWrapper->ActiveManagementEnableSol ()) {
        AmtWrapper->AmtWrapperSet (SET_OS_SOL, NULL);
     //Aptio V Server override start: Modify it for AMI code base 
        BdsLibUpdateConsoleVariable (L"ConIn", SolDevicePath, NULL);
        BdsLibUpdateConsoleVariable (L"Conout", SolDevicePath, NULL);
        BdsLibUpdateConsoleVariable (L"ErrOut", SolDevicePath, NULL);
      } else {
	BdsLibUpdateConsoleVariable (L"ConIn", NULL, SolDevicePath);
	BdsLibUpdateConsoleVariable (L"Conout", NULL, SolDevicePath);
	BdsLibUpdateConsoleVariable (L"ErrOut", NULL, SolDevicePath);
	//Aptio V Server override end
// //Aptio V Server override: comment it for building PASS TBD         EfiBootManagerUpdateConsoleVariable (ErrOut, NULL, SolDevicePath);
      }
    }
  }
}
/* AptioV server override : Don't use intel code.
VOID
AmtBdsBoot(
  VOID
  )
{
  EFI_STATUS               Status;
  AMT_WRAPPER_PROTOCOL     *AmtWrapper;
  //EFI_HANDLE               IderHandle;
  UINTN                    EventIndex;

  Status = gBS->LocateProtocol (&gEfiAmtWrapperProtocolGuid, NULL, &AmtWrapper);
  if (!EFI_ERROR (Status) && AmtWrapper->ActiveManagementPauseBoot ()) {
    //
    // Wait until user presses any key when PauseBoot is requested
    //
    gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &EventIndex);
    return;
  }

  if (!EFI_ERROR (Status) && AmtWrapper->ActiveManagementEnterSetup ()) {
    //
    // Enter to setup when SetupBoot is requested
    //
    ForceSetup();
    return;
  }


  if (!EFI_ERROR (Status) && AmtWrapper->ActiveManagementBootOptionExist())	{
    //
    // The following two things can be done in AmtWrapperDxe driver as well.
    // But doing it in BDS is simpler because every needed function is defined in BDS.
    //
    // 1. necessary to connect IDER device in case IDER boot
    //
//Aptio V Server override: Comment it for building PASS    
    Status = BdsLibConnectDevicePath ((EFI_DEVICE_PATH_PROTOCOL *) &gIderDevicePath);
//    if (!EFI_ERROR (Status)) {
      //
      // Recursively connect is needed because ScsiDisk produces BlkIo
      //
      //gBS->ConnectController (IderHandle, NULL, NULL, TRUE);
//    }
    //
    // 2. necessary to refresh the legacy boot option before doing the AMT legacy boot.
    //
//Aptio V Server override: Comment it for building PASS   EfiBootManagerRefreshAllBootOption ();

    AmtWrapper->BdsBootViaAsf();
  }
}
AptioV server override */
/* AptioV server override 
#define ME_ALERT_AT_HANDLER_GUID {0xb441df87, 0x8d94, 0x4811, {0x85, 0xf7, 0xf, 0x9a, 0x7b, 0xf8, 0x9d, 0x2a}}

EFI_GUID   mAlertAtHandlerGuid = ME_ALERT_AT_HANDLER_GUID;

VOID
EFIAPI
EmptyFuntion (
  IN EFI_EVENT                Event,
  IN VOID                     *Context
  )
{
  return;
}
AptioV server override */

/**
  AMT action when console is ready.
  1. Signal AlertAtHandler event.
  2. Dynamically switch the output mode to 80x25 when SOL is enabled.
**/
/*AptioV server override 
VOID
AmtConsoleReady (
  VOID
  )
{
  EFI_STATUS              Status;
  EFI_EVENT               Event;
  INTN                    Index;
  AMT_WRAPPER_PROTOCOL    *AmtWrapper;
  UINTN                   Columns;
  UINTN                   Rows;
  INTN                    FailSafeMode;

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  EmptyFuntion,
                  NULL,
                  &mAlertAtHandlerGuid,
                  &Event
                  );

  if(!EFI_ERROR(Status)) {
    gBS->SignalEvent (Event);
    gBS->CloseEvent (Event);
  }

  //
  // Set mode to 80 x 25
  //
  if (gST->ConOut == NULL) {
    return;
  }

  Status = gBS->LocateProtocol (&gEfiAmtWrapperProtocolGuid, NULL, &AmtWrapper);
  if (!EFI_ERROR (Status))	{
    AmtWrapper->AmtWrapperInit (gImageHandle, gST);

    //
    // Add AMT SOL device path to the ConIn, ConOut and StdErr variable when SOL is enabled.
    //
    FailSafeMode = gST->ConOut->Mode->MaxMode;
    if (AmtWrapper->ActiveManagementEnableSol ()) {
      for (Index = 0; Index < gST->ConOut->Mode->MaxMode; Index++) {
        Status = gST->ConOut->QueryMode (gST->ConOut, Index, &Columns, &Rows);
        if (!EFI_ERROR (Status) && (Columns == 80)) {
          if (Rows == 24) {
            break;
          }
          FailSafeMode = Index;
        }
      }

      if (Index == gST->ConOut->Mode->MaxMode) {
        //
        // When no 80x24 mode, use the 80x* mode.
        //
        Index = FailSafeMode;
      }

      if ((Index != gST->ConOut->Mode->MaxMode) && (Index != gST->ConOut->Mode->Mode)) {
        gST->ConOut->SetMode (gST->ConOut, (UINTN) Index);
      }
    }
  }
}
AptioV server override */

/*AptioV server override 
BOOLEAN
AmtEnterSetup (
  VOID
  )
{
  AMT_WRAPPER_PROTOCOL          *AmtWrapper;
  EFI_STATUS                    Status;

  Status = gBS->LocateProtocol (&gEfiAmtWrapperProtocolGuid, NULL, &AmtWrapper);
  if (!EFI_ERROR (Status))	{
    AmtWrapper->AmtWrapperInit (gImageHandle, gST);
    return AmtWrapper->ActiveManagementEnterSetup ();
  }
  return FALSE;
}

EFI_STATUS
AmtGetSetupPrompt (
  IN OUT  UINTN     *PromptIndex,
  OUT     CHAR16    **String
  )
{
  AMT_WRAPPER_PROTOCOL          *AmtWrapper;
  EFI_STATUS                    Status;

  Status = gBS->LocateProtocol (&gEfiAmtWrapperProtocolGuid, NULL, &AmtWrapper);
  if (!EFI_ERROR (Status))	{
    //Status = AmtWrapper->GetSetupPrompt (PromptIndex, String);
  }
  return Status;
}

EFI_STATUS
AmtGetSupportedHotkeys (
  OUT  UINTN             *KeyCnt,
  OUT  EFI_KEY_DATA      **KeyData
  )
{
  AMT_WRAPPER_PROTOCOL          *AmtWrapper;
  EFI_STATUS                    Status;

  *KeyCnt = 0;
  *KeyData = NULL;
  Status = gBS->LocateProtocol (&gEfiAmtWrapperProtocolGuid, NULL, &AmtWrapper);
  if (!EFI_ERROR (Status))	{
    //Status = AmtWrapper->GetSupportedHotkeys (KeyCnt, KeyData);
  }
  return Status;
}
AptioV server override */

/**
  Notification function for keystrokes.

  @param[in] KeyData    The key that was pressed.

  @retval EFI_SUCCESS   The operation was successful.
**/
/*AptioV server override 
EFI_STATUS
EFIAPI
MeHotkeyCallback(
  IN EFI_KEY_DATA *KeyData
  )
{
  AMT_WRAPPER_PROTOCOL          *AmtWrapper;
  EFI_STATUS                    Status;

  Status = gBS->LocateProtocol (&gEfiAmtWrapperProtocolGuid, NULL, &AmtWrapper);
  if (!EFI_ERROR (Status)) {
    //Status = AmtWrapper->DetermineSetupHotKey (KeyData);
    if (!EFI_ERROR (Status) && MeHotkeyTriggered) {
      Status = gBS->SignalEvent (*MeHotkeyTriggered);
      DEBUG ((EFI_D_INFO, "[MeHotkeyCallback]SignalEvent(MeHotkeyTriggered): %r \n", Status));
    }
  }
  return Status;
}
AptioV server override */


/**
  Callback function for SimpleTextInEx protocol install events

  @param Event           the event that is signaled.
  @param Context         not used here.

**/
/*AptioV server override 
VOID
EFIAPI
PlatformBdsTxtInExCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                         Status;
  UINTN                              BufferSize;
  EFI_HANDLE                         Handle;
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL  *TxtInEx;
  UINTN                              Count;
  UINTN                              Index;
  EFI_KEY_DATA                       *KeyData;
  EFI_HANDLE                         NotifyHandle;

  Status = AmtGetSupportedHotkeys (&Count, &KeyData);
  if (EFI_ERROR (Status)) {
    return;
  }

  while (TRUE) {
    BufferSize = sizeof (EFI_HANDLE);
    Status = gBS->LocateHandle (
                    ByRegisterNotify,
                    NULL,
                    mRegistration,
                    &BufferSize,
                    &Handle
                    );
    if (EFI_ERROR (Status)) {
      //
      // If no more notification events exist
      //
      return ;
    }

    Status = gBS->HandleProtocol (
                    Handle,
                    &gEfiSimpleTextInputExProtocolGuid,
                    (VOID **) &TxtInEx
                    );
    ASSERT_EFI_ERROR (Status);

    for (Index = 0; Index < Count; Index++) {
      Status = TxtInEx->RegisterKeyNotify (TxtInEx, &KeyData[Index], MeHotkeyCallback, &NotifyHandle);
      DEBUG ((EFI_D_INFO, "[Bds]RegisterKeyNotify: %04x/%04x %08x/%02x %r\n",
                          KeyData[Index].Key.ScanCode,
                          KeyData[Index].Key.UnicodeChar,
                          KeyData[Index].KeyState.KeyShiftState,
                          KeyData[Index].KeyState.KeyToggleState,
                          Status
            ));
      ASSERT_EFI_ERROR (Status);
    }
  }
  if (KeyData != NULL) {
    FreePool (KeyData);
  }
}
AptioV server override */

/**
  Register ME Hotkey.
**/
/* AptioV server override 
VOID
RegisterMeHotKey (
  EFI_EVENT *HotkeyTriggered
  )
{
  DEBUG((EFI_D_ERROR, "Entering RegisterMeHotKey\n"));
  EfiCreateProtocolNotifyEvent (
    &gEfiSimpleTextInputExProtocolGuid,
    TPL_CALLBACK,
    PlatformBdsTxtInExCallback,
    NULL,
    &mRegistration
    );

    MeHotkeyTriggered = HotkeyTriggered;
}
AptioV server override */

/**
  Raise Exception if ME FW requests to push media table

  @param[in][out]   None

  @return None.
**/
/* AptioV server override 
VOID
AmtRaiseBootException (
  VOID
  )
{
  EFI_STATUS            Status;
  DXE_MBP_DATA_PROTOCOL *MbpData;
  UINT8                 Data8;
  BOOLEAN               WarmReset;

  mAssetFullConfig = FALSE;
  //
  // Get Mbp Protocol
  //
  Status = gBS->LocateProtocol (
            &gMeBiosPayloadDataProtocolGuid,
            NULL,
            &MbpData
            );
  if (!EFI_ERROR(Status)){

    Data8 = PciRead8 (PCI_LIB_ADDRESS (ME_BUS, PCI_DEVICE_NUMBER_PCH_LPC, PCI_FUNCTION_NUMBER_PCH_LPC, R_PCH_LPC_GEN_PMCON_2));
    WarmReset = (BOOLEAN) !!(Data8 & B_PCH_LPC_GEN_PMCON_MEM_SR);
    //
    // During FastBoot, when the BIOS detects an Indication from the ME Firmware,the BIOS shall
    // enumerate all media devices and send all asset tables to the ME Firmware.
    //
    if ((MbpData->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType != FW_IMAGE_TYPE_1_5MB)
      && (MbpData->MeBiosPayload.HwaRequest.Available == TRUE) && (MbpData->MeBiosPayload.HwaRequest.Data.Fields.MediaTablePush == TRUE)
      && !WarmReset) {
      DEBUG((EFI_D_INFO,"Boot exception raised for ME MediaTablePush, do full media discovery.\n"));
      mAssetFullConfig = TRUE;
    }
  }
}
AptioV server override */

/**
  Check whether do full configuration enumeration for media asset table

  @param[in][out]   BootMode        A pointer to BootMode to be updated.

  @return None.
**/
/* AptioV server override 
VOID
AmtUpdateBootMode (
  IN OUT EFI_BOOT_MODE *BootMode
  )
{
  if (mAssetFullConfig) {
    *BootMode = BOOT_WITH_FULL_CONFIGURATION;
  }
}
AptioV server override Don't use intel code.*/

