/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:
  AmtPolicyDebugDumpDxe.c

@brief:
  Dump whole DXE_AMT_POLICY_PROTOCOL and serial out.
**/
#include <Library/DebugLib.h>
#include "AmtPolicyLib.h"
#include "Protocol/AmtPlatformPolicy.h"

extern DXE_AMT_POLICY_PROTOCOL  *mDxePlatformAmtPolicy;

VOID
DxeAmtPolicyDebugDump (
  VOID
  )
/**

  Dump DXE Amt Platform Policy

  @param[in] None.

**/
{
  EFI_STATUS Status;

  if (mDxePlatformAmtPolicy == NULL) {
    Status = AmtPolicyLibInit ();
    if (EFI_ERROR(Status)) {
      return;
    }
  }
  DEBUG ((EFI_D_INFO, "\n------------------------ AmtPlatformPolicy Dump Begin -----------------\n"));
  DEBUG ((EFI_D_INFO, " Revision : 0x%x\n", mDxePlatformAmtPolicy->Revision));
  DEBUG ((EFI_D_INFO, "AmtConfig ---\n"));
  //
  // Byte 0, bit definition for functionality enable/disable
  //
  DEBUG ((EFI_D_INFO, " AsfEnabled : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.AsfEnabled));
  DEBUG ((EFI_D_INFO, " iAmtEnabled : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.iAmtEnabled));
  DEBUG ((EFI_D_INFO, " iAmtbxPasswordWrite : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.iAmtbxPasswordWrite));
  DEBUG ((EFI_D_INFO, " WatchDog : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.WatchDog));
  DEBUG ((EFI_D_INFO, " WatchDogOs : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.WatchDogOs));
  DEBUG ((EFI_D_INFO, " CiraRequest : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.CiraRequest));
  DEBUG ((EFI_D_INFO, " ManageabilityMode : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.ManageabilityMode));
  DEBUG ((EFI_D_INFO, " UnConfigureMe : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.UnConfigureMe));
  DEBUG ((EFI_D_INFO, " MebxDebugMsg : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.MebxDebugMsg));

  //
  // Byte 1, bit definition for functionality enable/disable
  //
  DEBUG ((EFI_D_INFO, " ForcMebxSyncUp : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.ForcMebxSyncUp));
  DEBUG ((EFI_D_INFO, " UsbrEnabled : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.UsbrEnabled));
  DEBUG ((EFI_D_INFO, " UsbLockingEnabled : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.UsbLockingEnabled));
  DEBUG ((EFI_D_INFO, " HideUnConfigureMeConfirm : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.HideUnConfigureMeConfirm));
  DEBUG ((EFI_D_INFO, " USBProvision : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.USBProvision));
  DEBUG ((EFI_D_INFO, " FWProgress : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.FWProgress));
  DEBUG ((EFI_D_INFO, " iAmtbxHotKeyPressed : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.iAmtbxHotkeyPressed));
  DEBUG ((EFI_D_INFO, " iAmtbxSelectionScreen  : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.iAmtbxSelectionScreen));

  //
  // Byte 2, bit definition for functionality enable/disable
  //
  DEBUG ((EFI_D_INFO, " AtEnabled : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.AtEnabled));

  //
  // Byte 3-4 OS WatchDog Timer
  //
  DEBUG ((EFI_D_INFO, " WatchDogTimerOs : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.WatchDogTimerOs));

  //
  // Byte 5-6 BIOS WatchDog Timer
  //
  DEBUG ((EFI_D_INFO, " WatchDogTimerBios : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.WatchDogTimerBios));

  //
  // Byte 7 CIRA Timeout, Client Initiated Remote Access Timeout
  //             OEM defined timeout for MPS connection to be established.
  //
  DEBUG ((EFI_D_INFO, " CiraTimeout : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.CiraTimeout));

  //
  // Byte 8 CPU Replacement Timeout
  //
  DEBUG ((EFI_D_INFO, " CPU Replacement Timeout : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.CpuReplacementTimeout));

  //
  // Byte 9-10 OemResolutionSettings
  //
  DEBUG ((EFI_D_INFO, " MebxNonUiTextMode : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.MebxNonUiTextMode));
  DEBUG ((EFI_D_INFO, " MebxUiTextMode : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.MebxUiTextMode));
  DEBUG ((EFI_D_INFO, " MebxGraphicsMode : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.MebxGraphicsMode));

  //
  // Byte 11-14 Pointer to a list which contain on-board devices bus/device/fun number
  //
  DEBUG ((EFI_D_INFO, " PciDeviceFilterOutTable Pointer : 0x%x\n", mDxePlatformAmtPolicy->AmtConfig.PciDeviceFilterOutTable));

  DEBUG ((EFI_D_INFO, "\n------------------------ AmtPlatformPolicy Dump End -------------------\n"));
}

