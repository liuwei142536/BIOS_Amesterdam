/**
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
**/
/**

Copyright (c)  1999 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:
  MeDxePolicyInit.c

@brief:
  This file is SampleCode for Intel ME DXE Platform Policy initialzation.

**/
#include "MeDxePolicyInit.h"
#include "Library/MeTypeLib.h"

DXE_ME_POLICY_PROTOCOL  mMeDxePlatformPolicy = { 0 };

///
/// TS DIMM thermal polling Smbus Address.
/// This is platform specific.
///
UINT8                   mTsDimmSmbusAddress[] = { 0x30, 0x34 };

///
/// Function implementations
///
EFI_STATUS
EFIAPI
MeDxePolicyInitEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
/**

  @brief
  Initilize Intel ME DXE Platform Policy

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval EFI_SUCCESS             Initialization complete.
  @exception EFI_UNSUPPORTED      The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.

**/
{
  EFI_STATUS  Status;
  PLATFORM_DEBUG_CAP  DebugCapability;
  UINT8               Result;

  DEBUG ((EFI_D_ERROR | EFI_D_INFO, "[ME] ME Policy (Dxe) Entry Point\n"));

  ///
  /// ME DXE Policy Init
  ///
  mMeDxePlatformPolicy.Revision = DXE_PLATFORM_ME_POLICY_PROTOCOL_REVISION_1;

  ///
  /// Initialzie the Me Configuration
  ///
  mMeDxePlatformPolicy.MeConfig.EndOfPostEnabled        = 1;
  mMeDxePlatformPolicy.MeConfig.MeFwDownGrade           = 0;
  mMeDxePlatformPolicy.MeConfig.MeLocalFwUpdEnabled     = 0;
  mMeDxePlatformPolicy.MeConfig.MbpSecurity             = 1;
  mMeDxePlatformPolicy.MeConfig.MdesCapability          = 0;
  mMeDxePlatformPolicy.MeConfig.SvtForPchCap            = 0;
  mMeDxePlatformPolicy.MeConfig.MdesForBiosState        = 0;
  ///
  /// Please don't change the default value of EndOfPostDone and
  /// the value will be update to 1 when EOP has been sent to ME FW successfully
  ///
  mMeDxePlatformPolicy.MeConfig.EndOfPostDone = 0;

  ///
  /// Thermal reporting policy is based on strap settings
  ///
  MmioAndThenOr32 (
    PCH_RCRB_BASE + R_PCH_SPI_FDOC,
    (UINT32) (~(B_PCH_SPI_FDOC_FDSS_MASK | B_PCH_SPI_FDOC_FDSI_MASK)),
    (UINT32) (V_PCH_SPI_FDOC_FDSS_PCHS | R_PCH_SPI_STRP15)
    );

  mMeDxePlatformPolicy.MeReportError = ShowMeReportError;

  InstallMEPlatformPolicyLibrary (&mMeDxePlatformPolicy);
  ///
  /// BIOS utilizes the PLATFORM DEBUG CAPABILITY MEI message to turn on MDES capability or
  /// Intel SVT for PCH capability or both when system is in a post-manufactured state. The debug
  /// capabilities will be automatically disabled if the message is not receieved before
  /// receiving EOP in post-manufacturing. If BIOS wants to enabled/disabled the interface,
  /// BIOS should send the message during every boot.
  ///
  DebugCapability.Data = 0;   // Clear reserved bit fields.
  DebugCapability.Fields.Mdes = mMeDxePlatformPolicy.MeConfig.MdesCapability;
  DebugCapability.Fields.SvtForPch = mMeDxePlatformPolicy.MeConfig.SvtForPchCap;

  if (DebugCapability.Data != 0) {
    Status = HeciPlatformDebugCapabilityMsg (DebugCapability, &Result);
    if (EFI_ERROR (Status) || Result != 0) {
      DEBUG ((EFI_D_ERROR, "MDES Failed and the Status is %r, the Result is %x\n", Status, Result));
    }
  }

  ///
  /// Misc. Config
  ///
  /// FviSmbiosType is the SMBIOS OEM type (0x80 to 0xFF) defined in SMBIOS Type 14 - Group
  /// Associations structure - item type. FVI structure uses it as SMBIOS OEM type to provide
  /// version information. The default value is type 221.
  ///
  mMeDxePlatformPolicy.MeMiscConfig.FviSmbiosType = 0xDD;

  ///
  /// Install protocol to to allow access to this Policy.
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gDxePlatformMePolicyGuid,
                  &mMeDxePlatformPolicy,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

VOID
ShowMeReportError (
  IN ME_ERROR_MSG_ID            MsgId
  )
/**

@brief
  Show Me Error message.

  @param[in] MsgId                Me error message ID.


**/
{
  gST->ConOut->ClearScreen (gST->ConOut);

  switch (MsgId) {
  case MSG_EOP_ERROR:
    gST->ConOut->OutputString (gST->ConOut, L"Error sending End Of Post message to ME!\n");
    //
    // Additional delay for EOP message error
    //
    gBS->Stall (4 * HECI_MSG_DELAY);
    break;

  case MSG_ME_FW_UPDATE_FAILED:
    gST->ConOut->OutputString (gST->ConOut, L"ME FW Update Failed, please try again!\n");
    break;

  case MSG_ASF_BOOT_DISK_MISSING:
    gST->ConOut->OutputString (gST->ConOut, L"Boot disk missing, please insert boot disk and press ENTER\r\n");
    break;

  case MSG_KVM_TIMES_UP:
    gST->ConOut->OutputString (gST->ConOut, L"Error!! Times up and the KVM session was cancelled!!");
    break;

  case MSG_KVM_REJECTED:
    gST->ConOut->OutputString (gST->ConOut, L"Error!! The request has rejected and the KVM session was cancelled!!");
    break;

  case MSG_HMRFPO_LOCK_FAILURE:
    gST->ConOut->OutputString (gST->ConOut, L"(A7) Me FW Downgrade - Request MeSpiLock Failed\n");
    break;

  case MSG_HMRFPO_UNLOCK_FAILURE:
    gST->ConOut->OutputString (gST->ConOut, L"(A7) Me FW Downgrade - Request MeSpiEnable Failed\n");
    break;

  case MSG_ME_FW_UPDATE_WAIT:
    gST->ConOut->OutputString (
                  gST->ConOut,
                  L"Intel(R) Firmware Update is in progress. It may take up to 90 seconds. Please wait.\n"
                  );
    break;

  case MSG_ILLEGAL_CPU_PLUGGED_IN:
    gST->ConOut->OutputString (
                  gST->ConOut,
                  L"\n\n\rAn unsupported CPU/PCH configuration has been identified.\n"
                  );
    gST->ConOut->OutputString (
                  gST->ConOut,
                  L"\rPlease refer to the Haswell Platform Validation Matrix\n\rfor supported CPU/PCH combinations."
                  );
    break;

  case MSG_KVM_WAIT:
    gST->ConOut->OutputString (gST->ConOut, L"Waiting Up to 8 Minutes For KVM FW.....");
    break;

  default:
    DEBUG ((EFI_D_ERROR, "This Message Id hasn't been defined yet, MsgId = %x\n", MsgId));
    break;
  }

  gBS->Stall (HECI_MSG_DELAY);

}
