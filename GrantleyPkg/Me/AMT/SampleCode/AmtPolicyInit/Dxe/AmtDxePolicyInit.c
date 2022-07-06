/**
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
**/
/**

Copyright (c)  1999 - 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:
  AmtDxePolicyInit.c

@brief:
  This file is SampleCode for Intel AMT DXE Platform Policy initialzation.

**/
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include "Guid/MeBiosExtensionSetup.h"
#include "Protocol/MeBiosPayloadData.h"
#include "AmtDxePolicyInit.h"
#include "Library/MeTypeLib.h"

DXE_AMT_POLICY_PROTOCOL mAmtDxePlatformPolicy   = { 0 };
UINT64 mPciDeviceFilterOutTable[]               = { EFI_MAX_ADDRESS };
CHAR16 gEfiMeBiosExtensionSetupName[]           = EFI_ME_BIOS_EXTENSION_SETUP_VARIABLE_NAME;

///
/// Function implementations
///
EFI_STATUS
EFIAPI
AmtDxePolicyInitEntryPoint (
  IN  EFI_HANDLE                  ImageHandle,
  IN  EFI_SYSTEM_TABLE            *SystemTable
  )
/**

  @brief
  Initilize Intel AMT DXE Platform Policy

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval EFI_SUCCESS             Initialization complete.
  @exception EFI_UNSUPPORTED      The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.

**/
{
  EFI_STATUS              Status;
  UINTN                   VariableSize;
  ME_BIOS_EXTENSION_SETUP MeBiosExtensionSetupData;
  DXE_MBP_DATA_PROTOCOL   *MbpData;

#if defined(SPS_SUPPORT) && SPS_SUPPORT
  if (!MeTypeIsAmt()) {
    return EFI_UNSUPPORTED;
  }
#endif // SPS_SUPPORT

  Status = gBS->LocateProtocol (
                  &gMeBiosPayloadDataProtocolGuid,
                  NULL,
                  &MbpData
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (MbpData->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType != INTEL_ME_5MB_FW) {
    return EFI_UNSUPPORTED;
  }

  VariableSize = 0;
  ///
  /// AMT DXE Policy Init
  ///
  mAmtDxePlatformPolicy.Revision      = DXE_PLATFORM_AMT_POLICY_PROTOCOL_REVISION_1;

  ///
  /// Initialzie the Me Configuration
  ///
  mAmtDxePlatformPolicy.AmtConfig.AsfEnabled                = 1;
  mAmtDxePlatformPolicy.AmtConfig.iAmtEnabled               = 1;
  mAmtDxePlatformPolicy.AmtConfig.WatchDog                  = 0;
  mAmtDxePlatformPolicy.AmtConfig.WatchDogOs                = 0;
  mAmtDxePlatformPolicy.AmtConfig.WatchDogTimerOs           = 0;
  mAmtDxePlatformPolicy.AmtConfig.WatchDogTimerBios         = 0;
  mAmtDxePlatformPolicy.AmtConfig.CiraRequest               = 0;
  mAmtDxePlatformPolicy.AmtConfig.CiraTimeout               = 0;
  mAmtDxePlatformPolicy.AmtConfig.UnConfigureMe             = 0;
  mAmtDxePlatformPolicy.AmtConfig.HideUnConfigureMeConfirm  = 0;
  mAmtDxePlatformPolicy.AmtConfig.MebxDebugMsg              = 0;
  mAmtDxePlatformPolicy.AmtConfig.USBProvision              = 0;
  mAmtDxePlatformPolicy.AmtConfig.FWProgress                = 1;
  mAmtDxePlatformPolicy.AmtConfig.iAmtbxSelectionScreen     = 0;
  mAmtDxePlatformPolicy.AmtConfig.iAmtbxHotkeyPressed       = 0;

  ///
  /// Oem Resolution Settings
  ///
  mAmtDxePlatformPolicy.AmtConfig.MebxNonUiTextMode = 0;
  ///
  /// MEBX_TEXT_AUTO
  ///
  mAmtDxePlatformPolicy.AmtConfig.MebxUiTextMode = 0;
  ///
  /// MEBX_TEXT_AUTO
  ///
  mAmtDxePlatformPolicy.AmtConfig.MebxGraphicsMode = 0;
  ///
  /// MEBX_GRAPHICS_AUTO
  ///
  mAmtDxePlatformPolicy.AmtConfig.PciDeviceFilterOutTable = (UINT32) (UINTN) mPciDeviceFilterOutTable;

  ///
  /// Please don't change the default value of ForcMebxSyncUp and
  /// This let customer to force MEBX execution if they need and
  ///
  mAmtDxePlatformPolicy.AmtConfig.ForcMebxSyncUp = 0;

  ///
  /// Get BIOS Sync-up data from MEBx to AMT platform policy
  ///
  VariableSize = sizeof (MeBiosExtensionSetupData);
  Status = gRT->GetVariable (
                  gEfiMeBiosExtensionSetupName,
                  &gEfiMeBiosExtensionSetupGuid,
                  NULL,
                  &VariableSize,
                  &MeBiosExtensionSetupData
                  );
  if (EFI_ERROR (Status)) {
    mAmtDxePlatformPolicy.AmtConfig.ManageabilityMode = 0;
  } else {
    mAmtDxePlatformPolicy.AmtConfig.ManageabilityMode = MeBiosExtensionSetupData.PlatformMngSel;

  }

  UpdateDxeAmtPlatformPolicy (&mAmtDxePlatformPolicy);

  ///
  /// Install protocol to to allow access to this Policy.
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gDxePlatformAmtPolicyGuid,
                  &mAmtDxePlatformPolicy,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
