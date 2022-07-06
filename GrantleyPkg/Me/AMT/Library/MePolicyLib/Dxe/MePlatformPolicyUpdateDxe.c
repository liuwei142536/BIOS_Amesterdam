/*++

This file contains a 'Sample Driver' and is licensed as such
under the terms of your license agreement with Intel or your
vendor.  This file may be modified by the user, subject to
the additional terms of the license agreement

--*/

/*++
Copyright (c)  2008 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

 MePlatformPolicyUpdateDxe.c

Abstract:


--*/

#include "Protocol/FirmwareVolume.h"
#include "FrameworkDxe.h "
#include "Library/DebugLib.h"
#include "Guid/HiiSetupEnter.h"
#include "Framework/DxeCis.h"
#include "CommonIncludes.h "
#include "Library/UefiBootServicesTableLib.h"
#include "Guid/SetupVariable.h"
#include "Library/UefiRuntimeServicesTableLib.h"
#include "MePlatformPolicyUpdateDxe.h"
#include "Protocol/MeBiosPayloadData.h"
#include "MeState.h"
#include "Library/MeTypeLib.h"


extern UINT8      mTsDimmSmbusAddress[];

BOOLEAN IsAfterEndOfPost (void);

//
// Function implementations
//
UINT8 GetMeMode(void)
{
  EFI_STATUS        Status;
  UINT32            retVal;
  EFI_HECI_PROTOCOL *Heci;

  //
  // Default ME Mode
  //
  retVal = ME_MODE_FAILED;

  Status = gBS->LocateProtocol (&gEfiHeciProtocolGuid, NULL, &Heci);
  if (!EFI_ERROR (Status)) {
    Status = Heci->GetMeMode (&retVal);
    ASSERT_EFI_ERROR(Status);
  }
  return (UINT8)retVal;
}


EFI_STATUS
EFIAPI
InstallMEPlatformPolicyLibrary (
  IN OUT  DXE_ME_POLICY_PROTOCOL  *mDxePlatformMePolicy
)
/*++

Routine Description:

  Install the ME Platform Policy Library

Arguments:

  mDxePlatformMePolicy  ME Platform Policy in DXE phase

Returns:

  EFI_SUCCESS           Initialization complete.
  EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  EFI_DEVICE_ERROR      Device error, driver exits abnormally.

--*/
{
  EFI_STATUS              Status;
  EFI_EVENT               SetupEnterEvent;
  VOID                    *pSetupRegistration;
  EFI_EVENT               Event;

  DEBUG ((EFI_D_ERROR, "MePlatformPolicyEntryPoint start [InstallMEPlatformPolicyLibrary()]\n"));
  ASSERT(mDxePlatformMePolicy != NULL);

  Status = UpdateMePolicyFromSetup (mDxePlatformMePolicy, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "UpdateMePolicyFromSetup Failed and the Status is %x\n", Status));
    return Status;
  }
  //
  // Get info for ME setup options
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  GetMeSetupInfo,
                  NULL,
                  &SetupEnterEvent
                  );
  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Get FW data if we enter UI
  //
  Status = gBS->RegisterProtocolNotify (
                  &gEfiSetupEnterGuid,
                  SetupEnterEvent,
                  &pSetupRegistration
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  SignalMeStateChng,
                  NULL,
                  &gMeStateChgGuid,
                  &Event
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  SignalIfrStateChng,
                  NULL,
                  &gMeSetupInfoGuid,
                  &Event
                  );

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  SignalMeUpdatePolicyFromSetup,
                  NULL,
                  &gMeSetupPolicyUpdate,
                  &Event
                  );

  return Status;
}

EFI_STATUS
UpdateMePolicyFromSetup (
  DXE_ME_POLICY_PROTOCOL  *MePlatformPolicyInstance,
  BOOLEAN                 updateMeFwDownGrade
  )
/*++

Routine Description:

  Update Me platform policy while MePlatformProtocol is installed.

Arguments:

  MePlatformPolicyInstance - ME Platform Policy
  updateMeFwDownGrade      - Flag to updateMeFwDowngrade update

Returns:

  None

--*/
{
  EFI_STATUS           Status;
  SYSTEM_CONFIGURATION SetupData;
  UINTN                SetupVariableSize;
  UINT32               SetupVariableAttributes;

  DEBUG ((EFI_D_ERROR, "UpdateMePolicyFromSetup: start\n"));
  ASSERT(MePlatformPolicyInstance != NULL);

  MePlatformPolicyUpdateMeSettings (MePlatformPolicyInstance);

  SetupVariableSize = sizeof (SYSTEM_CONFIGURATION);
  //
  // Get Me configuration from Setup Data
  //
  // AptioV Server Override: Changed variable name to IntelSetup
  Status = gRT->GetVariable (
                  L"IntelSetup",
                  &gEfiSetupVariableGuid,
                  &SetupVariableAttributes,
                  &SetupVariableSize,
                  &SetupData
                  );
  if (!EFI_ERROR (Status)) {
    MePlatformPolicyInstance->MeConfig.MeFwDownGrade        = SetupData.MeFwDowngrade;
    MePlatformPolicyInstance->MeConfig.MeLocalFwUpdEnabled  = SetupData.LocalFwUpdEnabled;
    MePlatformPolicyInstance->MeConfig.MdesCapability       = SetupData.MdesCapability;
    MePlatformPolicyInstance->MeConfig.SvtForPchCap         = SetupData.SvtForPchCap;
    MePlatformPolicyInstance->MeConfig.MdesForBiosState     = SetupData.MdesForBiosState;

    if (  (SetupData.MeFwDowngrade != 0) && updateMeFwDownGrade) {
      //
      // FW Downgrade BIOS Setup Option is only valid for one boot
      //
      SetupData.MeFwDowngrade = 0;

      //
      // update FW Downgrade in Setup
      //
      Status = gRT->SetVariable (
                    L"IntelSetup",
                    &gEfiSetupVariableGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof (SYSTEM_CONFIGURATION),
                    &SetupData
                    );
    }

  }

  return Status;
}

EFI_STATUS
EFIAPI
MePlatformPolicyUpdateMeSettings (
  DXE_ME_POLICY_PROTOCOL *MePlatformPolicyInstance
  )
/*++

Routine Description:

  Functions performs HECI exchange with FW to update MePlatformPolicy settings.

Arguments:

  None

Returns:

  None.

--*/
{
  EFI_STATUS            Status;
  ME_INFO_SETUP_DATA    MeInfoSetupData;
  DXE_MBP_DATA_PROTOCOL *MbpData;
  SYSTEM_CONFIGURATION  SetupData;
  UINTN                 SetupVariableSize;
  UINT32                SetupVariableAttributes;
  EFI_EVENT             Event;

  DEBUG ((EFI_D_ERROR, "MePlatformPolicyUpdateMeSettings start\n"));
  ASSERT(MePlatformPolicyInstance != NULL);

  Status = gBS->LocateProtocol (
                &gMeBiosPayloadDataProtocolGuid,
                NULL,
                &MbpData
                );
  if (EFI_ERROR(Status))  {
    MeInfoSetupData.MeFirmwareInfo  = ME_MODE_FAILED;
    MeInfoSetupData.MeMajor         = 0;
    MeInfoSetupData.MeMinor         = 0;
    MeInfoSetupData.MeHotFix        = 0;
    MeInfoSetupData.MeBuildNo       = 0;
    MeInfoSetupData.MeFileSystem    = 0;
    MeInfoSetupData.MeImageType     = 0;
    MbpData                         = NULL;
  } else {
    MeInfoSetupData.MeMajor      = MbpData->MeBiosPayload.FwVersionName.MajorVersion;
    MeInfoSetupData.MeMinor      = MbpData->MeBiosPayload.FwVersionName.MinorVersion;
    MeInfoSetupData.MeHotFix     = MbpData->MeBiosPayload.FwVersionName.HotfixVersion;
    MeInfoSetupData.MeBuildNo    = MbpData->MeBiosPayload.FwVersionName.BuildVersion;
    MeInfoSetupData.MeFileSystem = MbpData->MeBiosPayload.MFSIntegrity;
    MeInfoSetupData.MeImageType  = (UINT8) MbpData->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType;

    MeInfoSetupData.MeFirmwareInfo = GetMeMode();
  }

  Status = gRT->SetVariable (
                  L"MeInfoSetup",
                  &gMeSetupInfoGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof(MeInfoSetupData),
                  &MeInfoSetupData
                  );
  if (EFI_ERROR(Status))  {
    DEBUG ((EFI_D_ERROR, "MePlatformPolicyUpdateMeSettings Error: Cannot find MeInfoSetup\n"));
    ASSERT_EFI_ERROR(Status);
    return Status;
  }


  //
  // Get Me configuration from Setup Data
  //
  SetupVariableSize = sizeof (SYSTEM_CONFIGURATION);
  // AptioV Server Override: Changed variable name to IntelSetup
  Status = gRT->GetVariable (
                  L"IntelSetup",
                  &gEfiSetupVariableGuid,
                  &SetupVariableAttributes,
                  &SetupVariableSize,
                  &SetupData
                  );

  if (!EFI_ERROR(Status)) {
    //
    // Set Image Type for MEBx disposition
    //
    if (MbpData) {
      SetupData.MeImageType    = (UINT8) MbpData->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType;
    } else {
      SetupData.MeImageType    = 0;
    }
    SetupData.MeFirmwareInfo = MeInfoSetupData.MeFirmwareInfo;
    Status = gRT->SetVariable (
                    L"IntelSetup", //AptioV Grangeville Override
                    &gEfiSetupVariableGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof (SYSTEM_CONFIGURATION),
                    &SetupData
                    );
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "MePlatformPolicyUpdateMeSettings Error: Cannot save Setup\n"));
      ASSERT_EFI_ERROR (Status);
    }
  }

  if ((GetMeMode() == ME_MODE_NORMAL) &&
      (!EFI_ERROR(Status))) {
    //
    // Create an Event if User changes BIOS option
    //
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    SignalFwuDataChng,
                    NULL,
                    &gSetupFwUpdChgGuid,
                    &Event
                    );
    if (!EFI_ERROR(Status)) {
      //
      // Create an Event if User changes BIOS option
      //
      Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    SignalMngStateChng,
                    NULL,
                    &gSetupMngChgGuid,
                    &Event
                    );
    }
  }

  return Status;
}


VOID
SignalFwuDataChng (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
/*++

Routine Description:

  Function that is called when we save UI Data

Arguments:

  Event         Pointer to the event that triggered this Callback Function
  Context       VOID Pointer required for Callback function

Returns:

  None


--*/
{
  EFI_STATUS              Status;
  SYSTEM_CONFIGURATION    SetupDataBuffer;
  SYSTEM_CONFIGURATION    *SetupData;
  UINTN                   SetupVariableSize;
  UINT32                  SetupVariableAttributes;
  DXE_ME_POLICY_PROTOCOL  *MePlatformPolicy;

  SetupData = &SetupDataBuffer;
  // AptioV Server Override: Changed variable name to IntelSetup
  SetupVariableSize = sizeof (SYSTEM_CONFIGURATION);
  Status = gRT->GetVariable (
                  L"IntelSetup",
                  &gEfiSetupVariableGuid,
                  &SetupVariableAttributes,
                  &SetupVariableSize,
                  &SetupDataBuffer
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "SignalFwuDataChng - Error %r getting Setup Data \n", Status));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  if (SetupData->MeImageType != INTEL_ME_1_5MB_FW) {
    return;
  }

  Status = gBS->LocateProtocol (&gDxePlatformMePolicyGuid, NULL, &MePlatformPolicy);
  if (EFI_ERROR(Status)) {
    // no gDxePlatformMePolicyGuid, no update (reset should be requested)
    return;
  }

  if (MePlatformPolicy->PreviousLocalFwUpdEnabled != SetupData->LocalFwUpdEnabled) {
    if (!IsAfterEndOfPost()) {
      Status = HeciSetLocalFwUpdate (SetupData->LocalFwUpdEnabled);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "HeciSetLocalFwUpdate Status = %r\n", Status));
        ASSERT_EFI_ERROR (Status);
        return;
      }
    }
    MePlatformPolicy->PreviousLocalFwUpdEnabled = SetupData->LocalFwUpdEnabled;
    MePlatformPolicy->AnythingChanged = TRUE;
  }
  return;
}

VOID
GetMeSetupInfo (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
/*++

Routine Description:
  Gets data from FW and updates initial values of BIOS setup data structures.
  Function is called only if we enter BIOS Setup, because it sends heci
  messages and takes some time to execute.

Arguments:

  Event         Pointer to the event that triggered this Callback Function
  Context       VOID Pointer required for Callback function

Returns:

  None


--*/
{

  EFI_STATUS             Status;
  UINT8                  FwUpdateState;
  SYSTEM_CONFIGURATION   SetupDataBuffer;
  SYSTEM_CONFIGURATION   *SetupData;
  UINTN                  SetupVariableSize;
  UINT32                 SetupVariableAttributes;
  UINT32                 MeMode;
  MEFWCAPS_SKU           CurrentFeatures;
  UINT8                  GetRuleData;
  DXE_ME_POLICY_PROTOCOL *MePlatformPolicy;

  FwUpdateState = 0;
  SetupData     = &SetupDataBuffer;

  MeMode = GetMeMode();

  if (MeMode == ME_MODE_FAILED) {
    return;
  }
  // AptioV Server Override: Changed variable name to IntelSetup
  SetupVariableSize = sizeof (SYSTEM_CONFIGURATION);
  Status = gRT->GetVariable (
                  L"IntelSetup",
                  &gEfiSetupVariableGuid,
                  &SetupVariableAttributes,
                  &SetupVariableSize,
                  &SetupDataBuffer
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "GetMeSetupInfo - Error %r getting Setup Data \n", Status));
    ASSERT_EFI_ERROR (Status);
    return;
  }

#if defined(AMT_SUPPORT) && AMT_SUPPORT
  SetupData->MeStateControl = 0;
#endif // AMT_SUPPORT
  SetupData->MngState       = 0;
  if (MeMode == ME_MODE_NORMAL) {
#if defined(AMT_SUPPORT) && AMT_SUPPORT
    SetupData->MeStateControl = 1;
#endif // AMT_SUPPORT
    Status = HeciGetFwFeatureStateMsg (&CurrentFeatures);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "HeciGetFwFeatureStateMsg Status = %r\n", Status));
      return; // Event should not return error status, so just exit routine
    }

    if (CurrentFeatures.Fields.Manageability) {
      SetupData->MngState = 1;
    } else {
      SetupData->MngState = 0;
    }

    SetupDataBuffer.IfrUpdateEnabled = IFR_UPDATE_DISABLE;
    Status = HeciGetIfrUpdate(&GetRuleData);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "HeciGetIfrUpdate Status = %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      return;
    }

    if (GetRuleData) {
      SetupDataBuffer.IfrUpdateEnabled = IFR_UPDATE_ENABLE;
    } else {
      SetupDataBuffer.IfrUpdateEnabled = IFR_UPDATE_DISABLE;
    }
  }
  // AptioV Server Override: Changed variable name to IntelSetup
  Status = gRT->SetVariable (
                  L"IntelSetup",
                  &gEfiSetupVariableGuid,
                  SetupVariableAttributes,
                  SetupVariableSize,
                  &SetupDataBuffer
                  );

  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "[ME]: Error: Can't read setup configuration.\n"));
    return;
  }

  Status = gBS->LocateProtocol (&gDxePlatformMePolicyGuid, NULL, &MePlatformPolicy);
  if (!EFI_ERROR(Status)) {
#if AMT_SUPPORT
    MePlatformPolicy->PreviousMeStateControl = SetupData->MeStateControl;
#endif // AMT_SUPPORT
    MePlatformPolicy->AnythingChanged = FALSE;
  }

  //
  // Skip Local Firmware update if not 1.5M sku or not in normal mode
  //
  if ((SetupData->MeImageType != INTEL_ME_1_5MB_FW) || (MeMode != ME_MODE_NORMAL)) {
    return;
  }

  Status = HeciGetLocalFwUpdate (&FwUpdateState);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "HeciGetLocalFwUpdate Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  //
  // MEBx Provides three options - Enabled/Disable/Password Protected
  // BIOS provides two options. If for some reason Password Protected
  // is selected then update FW with BIOS default/Selection
  //
  if (SetupData->LocalFwUpdEnabled != FwUpdateState) {
    if (FwUpdateState < MAX_FW_UPDATE_BIOS_SELECTIONS) {
      SetupData->LocalFwUpdEnabled = FwUpdateState;
      if (MePlatformPolicy) {
        MePlatformPolicy->PreviousLocalFwUpdEnabled = SetupData->LocalFwUpdEnabled;
      }
    
      Status = gRT->SetVariable (
                    L"IntelSetup", // AptioV Override - "Setup" -> "IntelSetup"
                    &gEfiSetupVariableGuid,
                    SetupVariableAttributes,
                    SetupVariableSize,
                    &SetupDataBuffer
                    );
    }
  }

  return;
}

VOID
SignalMngStateChng (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
/*++

Routine Description:

  Function that is called when we save UI Data

Arguments:

  Event         Pointer to the event that triggered this Callback Function
  Context       VOID Pointer required for Callback function

Returns:

 None.


--*/
{
  EFI_STATUS             Status;
  SYSTEM_CONFIGURATION   SetupDataBuffer;
  SYSTEM_CONFIGURATION   *SetupData;
  UINTN                  SetupVariableSize;
  UINT32                 SetupVariableAttributes;
  UINT32                 EnableBitmap;
  UINT32                 DisableBitmap;
  MEFWCAPS_SKU           CurrentFeatures;
  UINT8                  AmtState;
  DXE_ME_POLICY_PROTOCOL *MePlatformPolicy;

  SetupData     = &SetupDataBuffer;
  EnableBitmap  = 0;
  DisableBitmap = 0;
  AmtState      = 0;

  DEBUG ((EFI_D_ERROR, "SignalMngStateChng - called\n"));

  SetupVariableSize = sizeof (SYSTEM_CONFIGURATION);
  // AptioV Server Override: Changed variable name to IntelSetup
  Status = gRT->GetVariable (
                  L"IntelSetup",
                  &gEfiSetupVariableGuid,
                  &SetupVariableAttributes,
                  &SetupVariableSize,
                  &SetupDataBuffer
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "SignalMngStateChng - Error %r getting Setup Data \n", Status));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  Status = HeciGetFwFeatureStateMsg (&CurrentFeatures);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "HeciGetFwFeatureStateMsg Status = %r\n", Status));
    return;
  }

  if (CurrentFeatures.Fields.Manageability) {
    AmtState = 1;
  } else {
    AmtState = 0;
  }

  if (AmtState == SetupData->MngState) {
    return;
  }

  if (SetupData->MngState == 1) {
    //
    // User wants to enable AMT
    //
    EnableBitmap  = AMT_BITMASK;
    DisableBitmap = CLEAR_AMT_BIT;
  } else if (SetupData->MngState == 0) {
    //
    // User wants to disable AMT
    //
    EnableBitmap  = CLEAR_AMT_BIT;
    DisableBitmap = AMT_BITMASK;
  }

  if (!IsAfterEndOfPost()) {
    Status = HeciFwFeatureStateOverride (EnableBitmap, DisableBitmap);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "HeciFwFeatureStateOverride Status = %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      return;
    }
  }

  Status = gBS->LocateProtocol (&gDxePlatformMePolicyGuid, NULL, &MePlatformPolicy);
  if (!EFI_ERROR(Status)) {
    MePlatformPolicy->AnythingChanged = TRUE;
  }

  return;
}

VOID
SignalMeUpdatePolicyFromSetup (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
/*++

Routine Description:

  Function that is called when we save UI Data

Arguments:

  Event         Pointer to the event that triggered this Callback Function
  Context       VOID Pointer required for Callback function

Returns:

  None


--*/
{
  DXE_ME_POLICY_PROTOCOL  *MePlatformPolicy;
  EFI_STATUS              Status;

  Status = gBS->LocateProtocol (&gDxePlatformMePolicyGuid, NULL, &MePlatformPolicy);
  if (!EFI_ERROR(Status)) {
    UpdateMePolicyFromSetup (MePlatformPolicy, FALSE);
  }
}

VOID
SignalMeStateChng (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
/*++

Routine Description:

  Function that is called when we save UI Data

Arguments:

  Event         Pointer to the event that triggered this Callback Function
  Context       VOID Pointer required for Callback function

Returns:

  None


--*/
{
  EFI_STATUS              Status;
  SYSTEM_CONFIGURATION    SetupDataBuffer;
  SYSTEM_CONFIGURATION    *SetupData;
  UINTN                   SetupVariableSize;
  UINT32                  SetupVariableAttributes;
#if defined(AMT_SUPPORT) && AMT_SUPPORT
  DXE_ME_POLICY_PROTOCOL  *MePlatformPolicy;
#endif // AMT_SUPPORT

  SetupData = &SetupDataBuffer;

  DEBUG ((EFI_D_ERROR, "SignalMeStateChng called\n"));

  SetupVariableSize = sizeof (SYSTEM_CONFIGURATION);
  // AptioV Server Override: Changed variable name to IntelSetup
  Status = gRT->GetVariable (
                  L"IntelSetup",
                  &gEfiSetupVariableGuid,
                  &SetupVariableAttributes,
                  &SetupVariableSize,
                  &SetupDataBuffer
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "SignalMeStateChng - Error %r getting Setup Data \n", Status));
    ASSERT_EFI_ERROR (Status);
    return;
  }
#if defined(AMT_SUPPORT) && AMT_SUPPORT
  Status = gBS->LocateProtocol (&gDxePlatformMePolicyGuid, NULL, &MePlatformPolicy);
  if (EFI_ERROR(Status)) {
    // no gDxePlatformMePolicyGuid, no update (reset should be requested)
    return;
  }
  if (MePlatformPolicy->PreviousMeStateControl != SetupData->MeStateControl) {
    if (!IsAfterEndOfPost()) {
      //
      // ME State Control
      //
      if (SetupData->MeStateControl == 0) {
        Status = HeciSetMeDisableMsg ();
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "HeciSetMeDisableMsg Status = %r\n", Status));
          return;
        }
      }

      if (SetupData->MeStateControl == 1) {
        Status = HeciSetMeEnableMsg ();
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_INFO, "HeciSetMeEnableMsg Status = %r\n", Status));
        }
      }
    }
    MePlatformPolicy->PreviousMeStateControl = SetupData->MeStateControl;
    MePlatformPolicy->AnythingChanged = TRUE;
  }
#endif // AMT_SUPPORT

  return;
}


VOID
SignalIfrStateChng(
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
/*++

Routine Description:

  Function that is called when we save UI Data.

Arguments:

  Event         Pointer to the event that triggered this Callback Function
  Context       VOID Pointer required for Callback function

Returns:

  None.

--*/
{
  EFI_STATUS              Status;
  SYSTEM_CONFIGURATION    NewSetupData;
  UINTN                   Size;
//Grangeville AptioV server override - CPP Check
  UINT8                   GetRuleData = 0;
  UINT32                  MeMode;
  DXE_ME_POLICY_PROTOCOL  *MePlatformPolicy;

  Status = gBS->LocateProtocol (&gDxePlatformMePolicyGuid, NULL, &MePlatformPolicy);
  if (EFI_ERROR(Status)) {
    // no gDxePlatformMePolicyGuid, no update (reset should be requested)
    return;
  }

  DEBUG ((EFI_D_ERROR, "UpdateIfrState called\n"));

  MeMode = GetMeMode();
  if (MeMode != ME_MODE_NORMAL) {
    return;
  }

  Size = sizeof (SYSTEM_CONFIGURATION);
  // AptioV Server Override: Changed variable name to IntelSetup
  Status = gRT->GetVariable (
    L"IntelSetup",
    &gEfiSetupVariableGuid,
    NULL,
    &Size,
    &NewSetupData
    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  Status = HeciGetIfrUpdate(OUT &GetRuleData);
  if (EFI_ERROR (Status)) {
    return;
  }

  if (NewSetupData.IfrUpdateEnabled == IFR_UPDATE_ENABLE && GetRuleData != IFR_UPDATE_ENABLE) {
    Status = HeciSetIfrUpdate(IFR_UPDATE_ENABLE);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return;
    }
  }

  if (NewSetupData.IfrUpdateEnabled == IFR_UPDATE_DISABLE&& GetRuleData != IFR_UPDATE_DISABLE) {
    Status = HeciSetIfrUpdate(IFR_UPDATE_DISABLE);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return;
    }
  }

}

