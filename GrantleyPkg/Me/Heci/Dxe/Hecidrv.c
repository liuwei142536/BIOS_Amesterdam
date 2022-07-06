/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  2007 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:
  Hecidrv.c

@brief:
  HECI driver for AMT support

**/
#if defined(AMT_SUPPORT) && AMT_SUPPORT

// AptioV server override: Including Token.h here so that token.h gets included first and redefintion errors are avoided when including token.h
#include <Token.h>
// AptioV server override: Including Token.h here so that token.h gets included first and redefintion errors are avoided when including token.h
#include <PchAccess.h>
#include <PchRegs.h>
#include <Library/S3BootScriptLib.h>
#include <Library/PerformanceLib.h>
#include <PchInit/Dxe/PchInitCommon.h>

#include "CommonIncludes.h"
#include "Library/RcFviDxeLib.h"
#include "Protocol/AmtReadyToBoot.h"
#include "Protocol/MebxProtocol.h"
#include <Protocol/MeRcInfo.h>
#include <Protocol/HeciProtocol.h>
#include "Guid/MeBiosExtensionSetup.h"
#include "Guid/MePlatformReadyToBoot.h"

#include "HeciInit.h"
#include "HeciDrv.h"
#include "MeLib.h"
#include "Library/MePolicyLib.h"
#include "MeAccess.h"
#include "Library/PciLib.h"
#include "IndustryStandard/Pci.h"

#ifdef TCG_SUPPORT_FLAG
#include "Acpi1_0.h"
#include "Acpi2_0.h"
#include "Acpi3_0.h"

#include "TpmPc.h"
#endif /// TCG_SUPPORT_FLAG

#include "Hecidrv.h"

#define ONE_SECOND_TIMEOUT  1000000
#define FWU_TIMEOUT         90

///
/// Global driver data
///
extern HECI_INSTANCE          *mHeciContext;
extern DXE_ME_POLICY_PROTOCOL *mDxePlatformMePolicy;
DXE_MBP_DATA_PROTOCOL         mMbpData;
CHAR16                        gEfiMeBiosExtensionSetupName[]   = EFI_ME_BIOS_EXTENSION_SETUP_VARIABLE_NAME;

FVI_ELEMENT_AND_FUNCTION      mMeFviElementsData[] = {
  {
    DEFAULT_FVI_ELEMENT_DATA(ME),
    NULL
  },
  {
    DEFAULT_FVI_ELEMENT_DATA(MEBX),
    NULL
  },
  {
    {
      1,
      3,
      MEFW_VERSION,
      MEFW_FVI_STRING,
      MEFW_1_5M_STRING,
    },
    NULL
  }
};

FVI_DATA_HUB_CALLBACK_CONTEXT mMeFviVersionData = {
  MISC_SUBCLASS_FVI_HEADER_ENTRY(ME),
  mMeFviElementsData,
};

UINTN                         mMeFviElements = sizeof (mMeFviElementsData) / sizeof (FVI_ELEMENT_AND_FUNCTION);


#ifdef TCG_SUPPORT_FLAG
EFI_STATUS
MeasureHer (
  IN  UINT32                      *HerValue,
  IN  UINT8                       Index
  )
/**

  @brief
  Perform measurement for HER register.

  @param[in] HerValue             The value of HECI Extend Register.
  @param[in] Index                HerValue Size.

  @retval EFI_SUCCESS             Measurement performed

**/
{
  EFI_STATUS            Status;
  EFI_TCG_PROTOCOL      *TcgProtocol;
  EFI_TCG_PCR_EVENT     TcgEvent;
  UINT32                EventNumber;
  EFI_PHYSICAL_ADDRESS  EventLogLastEntry;

  Status = gBS->LocateProtocol (
                  &gEfiTcgProtocolGuid,
                  NULL,
                  &TcgProtocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ///
  /// This below data will be stored in Tcg eventlog
  ///
  TcgEvent.Event.PostCode.PostCodeAddress = *HerValue;
  TcgEvent.Event.PostCode.PostCodeLength  = sizeof (UINT32);

  ///
  /// Fill the TcgEvent Header
  ///
  TcgEvent.Header.PCRIndex      = PCRi_CRTM_AND_POST_BIOS;
  TcgEvent.Header.EventType     = EV_S_CRTM_CONTENTS;

  TcgEvent.Header.EventDataSize = (Index * sizeof (UINT32));

  Status = TcgProtocol->HashLogExtendEvent (
                          TcgProtocol,
                          (EFI_PHYSICAL_ADDRESS) HerValue,
                          TcgEvent.Header.EventDataSize,
                          TPM_ALG_SHA,
                          (TCG_PCR_EVENT *) &TcgEvent,
                          &EventNumber,
                          &EventLogLastEntry
                          );
  return Status;
}

EFI_STATUS
MeMeasurement (
  VOID
  )
/**
  @brief
  Me Measurement.

  @param[in] None.

  @retval EFI_NOT_READY           Not ready for measurement.
  @retval EFI_SUCCESS             Measurement done

**/
{
  EFI_STATUS    Status;
  DATA32_UNION  Data32[7];
  UINT8         HerMax;
  UINT8         HerIndex;
  UINT8         Index;

  Index = 0;
  ///
  /// Measure HER
  ///
  HerMax                = R_ME_HER5;
  Data32[Index].Data32  = PciRead32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, R_ME_HERS));

  if ((Data32[Index].Data32 & B_ME_EXTEND_REG_VALID) == B_ME_EXTEND_REG_VALID) {
    if ((Data32[Index].Data8[0] & B_ME_EXTEND_REG_ALGORITHM) == V_ME_SHA_256) {
      HerMax = R_ME_HER8;
    }

    for (HerIndex = R_ME_HER1, Index = 0; HerIndex <= HerMax; HerIndex += 4, Index++) {
      Data32[Index].Data32 = PciRead32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, HerIndex));
    }

    Status = MeasureHer (&Data32->Data32, Index);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "ME Measurement feature failed, Status is %r \n", Status));
    }
  } else {
    Status = EFI_NOT_READY;
  }

  return Status;
}

VOID
MeMeasurementEvent (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *ParentImageHandle
  )
/**
  @brief
  Signal a event for last checking.  

  @param[in] Event                The event that triggered this notification function  
  @param[in] ParentImageHandle    Pointer to the notification functions context

  @retval EFI_SUCCESS             Event executed and closed.

**/
{
  DEBUG ((EFI_D_ERROR, "MeMeasurementEvent is called.\n"));

  MeMeasurement ();

  gBS->CloseEvent (Event);

  return;
}
#endif

EFI_STATUS
MeWarningMessage (
  VOID
  )
/**
  @brief
  Show warning message to user.  

  @param[in] None.

  @retval EFI_SUCCESS             Warning reported

**/
{
  HECI_FWS_REGISTER MeFirmwareStatus;

  MeFirmwareStatus.ul = HeciPciReadMefs1();

  ///
  /// Check for ME FPT Bad & FT BUP LD FLR
  ///
  if (MeFirmwareStatus.r.FptBad != 0 || MeFirmwareStatus.r.FtBupLdFlr != 0) {
    MeReportError (MSG_ME_FW_UPDATE_FAILED);
  }

  return EFI_SUCCESS;
}

VOID
DeviceStatusSave (
  VOID
  )
/**

  @brief
  Store the current value of DEVEN for S3 resume path

  @param[in] None


**/
{
  UINT32  Data;

  ///
  /// Read RCBA register for saving
  ///
  Data = Mmio16 (PCH_RCRB_BASE, R_PCH_RCRB_FD2);

  SCRIPT_MEM_WRITE (
    EFI_ACPI_S3_RESUME_SCRIPT_TABLE,
    EfiBootScriptWidthUint16,
    (UINTN) (PCH_RCRB_BASE + R_PCH_RCRB_FD2),
    1,
    &Data
    );
  Data = Mmio16 (PCH_RCRB_BASE, R_PCH_RCRB_FDSW);

  SCRIPT_MEM_WRITE (
    EFI_ACPI_S3_RESUME_SCRIPT_TABLE,
    EfiBootScriptWidthUint16,
    (UINTN) (PCH_RCRB_BASE + R_PCH_RCRB_FDSW),
    1,
    &Data
    );
}

EFI_STATUS
MeDeviceConfigure (
  VOID
  )
/**

  @brief
  Disable ME Devices when needed

  @param[in] None

  @retval EFI_SUCCESS             Always return EFI_SUCCESS

**/
{
  UINT32  MeMode;

  ///
  /// Disable Heci2 if policy dictates
  ///
  ///
  ///
    Heci2Disable ();

  HeciGetMeMode (&MeMode);
  if (MeMode == ME_MODE_NORMAL) {
    if (mHeciContext->MeFwImageType == INTEL_ME_1_5MB_FW) {
      ///
      /// We will disable all AMT relevant devices in 1.5M SKU
      ///
      IderDisable ();
      SolDisable ();
      Usbr1Disable ();
      Usbr2Disable ();
    }
  }
  ///
  /// If ME Mode is running in ME Temp Disable state, disable Heci1, HECI2, Ider and Sol
  /// The he same should occur when SECOVR_JMPR is used
  ///
  if ((MeMode == ME_MODE_TEMP_DISABLED) ||
      (MeMode == ME_MODE_SECOVER))  {
    DisableAllMEDevices ();
  }

  return EFI_SUCCESS;
}

EFI_STATUS
MeEndOfPostEvent (
  VOID
  )
/**
  @brief
  Send ME the BIOS end of Post message.

  @param[in] None.

  @retval EFI_SUCCESS             Always return EFI_SUCCESS except for policy initialization failure.
  @exception EFI_UNSUPPORTED      Policy initialization failure.

**/
{
  EFI_STATUS        Status;
  EFI_HECI_PROTOCOL *Heci;
  UINT32            MeStatus;

  ///
  /// Init ME Policy Library, continue to send EOP message continue even if can't find Me Platform Policy
  ///
  if (mDxePlatformMePolicy == NULL) {
    MePolicyLibInit ();
  }

  Status = gBS->LocateProtocol (
                  &gEfiHeciProtocolGuid,
                  NULL,
                  &Heci
                  );
  if (!EFI_ERROR (Status)) {
    ///
    /// Check ME Status
    ///
    Status = Heci->GetMeStatus (&MeStatus);
    ASSERT_EFI_ERROR (Status);

    ///
    /// Send EOP message when ME is ready.  Do not care about if ME FW INIT is completed.
    ///
    if (ME_STATUS_ME_STATE_ONLY (MeStatus) == ME_READY) {
      if (MeEndOfPostEnabled ()) {
        ///
        /// Send ME the BIOS Extensions exit code and End Of Post message.
        /// if no success, we display an error message and halt the system.
        ///
        DEBUG ((EFI_D_ERROR, "Sending EOP...\n"));
        Status = HeciSendEndOfPostMessage ();
        if (Status == EFI_SUCCESS) {
          if (mDxePlatformMePolicy != NULL) {
            mDxePlatformMePolicy->MeConfig.EndOfPostDone = 1;
          }
        }

        if ((EFI_ERROR (Status)) && (Status != EFI_UNSUPPORTED)) {
          MeReportError (MSG_EOP_ERROR);
        }
      } else {
        DEBUG ((EFI_D_ERROR, "Sending EOP... is disabled\n"));
      }
      ///
      /// End of EOP setup option
      ///
    } else {
      DEBUG ((EFI_D_ERROR, "Sending EOP...no, ME is in wrong state\n"));
    }
  }
  ///
  /// End of EFI_ERROR of locate HECI driver
  ///
  return EFI_SUCCESS;
}

EFI_STATUS
LockConfig (
  VOID
  )
/**
  @brief
  1. Cf9Gr Lock Config
      - PCH BIOS Spec Rev 0.9 Section 18.4	Additional Power Management Programming
        Step 2
        Set "Power Management Initialization Register (PMIR) Field 1", D31:F0:ACh[31] = 1b 
        for production machine according to "RS - PCH Intel Management Engine 
        (Intel(r) ME) BIOS Writer's Guide".
  2. Function Disable SUS Well Lock
      - PCH EDS 10.1.76 request that FDSW must be set when Intel Active Management Technology
        is Enabled

  @param[in] None

  @retval Status.

**/
{
  EFI_STATUS        Status;
  EFI_HECI_PROTOCOL *Heci;
  UINT32            MeMode;
  HECI_FWS_REGISTER MeFirmwareStatus;
  UINTN             Address;
  UINT32            Data;

  Status = gBS->LocateProtocol (
                  &gEfiHeciProtocolGuid,
                  NULL,
                  &Heci
                  );
  if (!EFI_ERROR (Status)) {
    ///
    /// Check ME Status
    ///
    Status = Heci->GetMeMode (&MeMode);
    ASSERT_EFI_ERROR (Status);

    MeFirmwareStatus.ul = HeciPciRead32 (R_FWSTATE);

    ///
    /// PCH BIOS Spec Rev 0.9 Section 18.4	Additional Power Management Programming
    /// Step 2
    ///   Set "Power Management Initialization Register (PMIR) Field 1", D31:F0:ACh[31] = 1b
    ///   for production machine according to "RS - PCH Intel Management Engine
    ///  (Intel(r) ME) BIOS Writer's Guide".
    ///
    /// PCH ME BWG section 4.5.1
    /// The IntelR FPT tool /GRST option uses CF9GR bit to trigger global reset.
    /// Based on above reason, the BIOS should not lock down CF9GR bit during Manufacturing and
    /// Re-manufacturing environment.
    ///
    Data = 0;
    if ((((MeMode == ME_MODE_NORMAL) || (MeMode == ME_MODE_TEMP_DISABLED)) && !(MeFirmwareStatus.r.ManufacturingMode))) {
      ///
      /// PCH ME BWG section 4.4.1
      /// BIOS must also ensure that CF9GR is cleared and locked (via bit31 of the same register) before
      /// handing control to the OS in order to prevent the host from issuing global resets and resetting
      /// Intel Management Engine.
      ///
      Data |= B_PCH_LPC_PMIR_CF9LOCK; 
    }

    Address = PCI_LIB_ADDRESS (
                DEFAULT_PCI_BUS_NUMBER_PCH,
                PCI_DEVICE_NUMBER_PCH_LPC,
                PCI_FUNCTION_NUMBER_PCH_LPC,
                R_PCH_LPC_PMIR
                );
    PciAndThenOr32 (
      Address,
      (UINT32) (~(B_PCH_LPC_PMIR_CF9LOCK | B_PCH_LPC_PMIR_CF9GR)),
      (UINT32) Data
      );

    ///
    /// Function Disable SUS Well lockdown
    ///
    if (MeMode == ME_MODE_NORMAL) {
      if (mHeciContext->MeFwImageType == INTEL_ME_5MB_FW) {
        DEBUG ((EFI_D_ERROR, "Function Disable SUS Well lockdown!\n"));
        FunctionDisableWellLockdown ();
      }
    }
  }

  return Status;
}

VOID
CheckFwUpdInProgress (
  VOID
  )
/**
  @brief
  Halt Boot for up to 90 seconds if Bit 11 of FW Status Register (FW_UPD_IN_PROGRESS) is set

  @param[in] None

  @retval None

**/
{
  HECI_FWS_REGISTER FwStatus;
  UINT8             StallCount;
  EFI_STATUS        Status;

  StallCount  = 0;
  Status      = mHeciContext->HeciCtlr.GetMeStatus (&FwStatus.ul);
  if (!EFI_ERROR (Status)) {
    if (FwStatus.ul & ME_FW_UPDATES_IN_PROGRESS) {
      MeReportError (MSG_ME_FW_UPDATE_WAIT);
    }

    while ((FwStatus.ul & ME_FW_UPDATES_IN_PROGRESS) && (StallCount < FWU_TIMEOUT)) {
      gBS->Stall (ONE_SECOND_TIMEOUT);
      StallCount  = StallCount + 1;
      Status      = mHeciContext->HeciCtlr.GetMeStatus (&FwStatus.ul);
    }
  }

  return ;
}

VOID
MeReadyToBootEventFunction (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *ParentImageHandle
  )
/**
  @brief
  Signal a event for Me ready to boot.  

  @param[in] Event                The event that triggered this notification function  
  @param[in] ParentImageHandle    Pointer to the notification functions context

  @retval EFI_SUCCESS             Always return EDI_SUCCESS

**/
{
  EFI_STATUS                  Status;
  EFI_EVENT                   MePlatformReadyToBootEvent;
  EFI_HANDLE                  *Handles;
  UINTN                       Index;
  UINTN                       Count;
  AMT_READY_TO_BOOT_PROTOCOL  *AmtReadyToBoot;
  UINT32                      MeMode;
  UINT32                      MeStatus;
#ifdef TCG_SUPPORT_FLAG
  EFI_EVENT                   LegacyBootEvent;
  EFI_EVENT                   ExitBootServicesEvent;
#endif // TCG_SUPPORT_FLAG
  static BOOLEAN              AlreadyCalled = FALSE;

  DEBUG ((EFI_D_ERROR, "MeReadyToBootEventFunction is called.\n"));

  if (AlreadyCalled) {
    DEBUG ((EFI_D_ERROR, "MeReadyToBootEventFunction is already called. Exit\n"));
    gBS->CloseEvent (Event);
    return;
  }
  AlreadyCalled = TRUE;

#ifdef TCG_SUPPORT_FLAG
  Status = MeMeasurement ();
  if (Status == EFI_NOT_READY) {
    ///
    /// Create a Legacy Boot event.
    ///
    Status = EfiCreateEventLegacyBootEx (
              TPL_CALLBACK,
              MeMeasurementEvent,
              NULL,
              &LegacyBootEvent
              );
    ASSERT_EFI_ERROR (Status);

    ///
    /// Create a ExitBootService event.
    ///
    Status = gBS->CreateEvent (
                    EVENT_SIGNAL_EXIT_BOOT_SERVICES,
                    TPL_CALLBACK,
                    MeMeasurementEvent,
                    NULL,
                    &ExitBootServicesEvent
                    );
    ASSERT_EFI_ERROR (Status);
  }

#endif ///TCG_SUPPORT_FLAG
  /// We will trigger all events in order
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gAmtReadyToBootProtocolGuid,
                  NULL,
                  &Count,
                  &Handles
                  );
  if (!EFI_ERROR (Status)) {
    for (Index = 0; Index < Count; Index++) {
      Status = gBS->HandleProtocol (Handles[Index], &gAmtReadyToBootProtocolGuid, &AmtReadyToBoot);
      ASSERT_EFI_ERROR (Status);
      AmtReadyToBoot->Signal ();
    }
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  MeEmptyEventFunction,
                  NULL,
                  &gMePlatformReadyToBootGuid,
                  &MePlatformReadyToBootEvent
                  );
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    gBS->SignalEvent (MePlatformReadyToBootEvent);
    gBS->CloseEvent (MePlatformReadyToBootEvent);
  }

  HeciGetMeMode (&MeMode);
  HeciGetMeStatus (&MeStatus);
  if ((MeMode == ME_MODE_NORMAL) &&
      ((ME_STATUS_ME_STATE_ONLY (MeStatus) == ME_IN_RECOVERY_MODE) || (ME_STATUS_ME_STATE_ONLY (MeStatus) == ME_READY))
      ) {

    CheckFwUpdInProgress ();

    Status = MeWarningMessage ();
    ASSERT_EFI_ERROR (Status);

    Status = MeEndOfPostEvent ();
    ASSERT_EFI_ERROR (Status);

  }

  ///
  /// PMIR Configuration & FDSW Lockdown
  ///
  Status = LockConfig ();
  ASSERT_EFI_ERROR (Status);

  ///
  /// Hide Me relevant Devices
  ///
  Status = MeDeviceConfigure ();
  ASSERT_EFI_ERROR (Status);

  gBS->CloseEvent (Event);

  return;
}

VOID
MeScriptSaveEventFunction (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *ParentImageHandle
  )
/**
  @brief
  Signal a event to save Me relevant registers and this event must be run before ExitPmAuth.

  @param[in] Event             - The event that triggered this notification function  
  @param[in] ParentImageHandle - Pointer to the notification functions context

  @retval EFI_SUCCESS             Always return EDI_SUCCESS

**/
{
  EFI_STATUS              Status;
  VOID                    *ProtocolPointer;
  EFI_HECI_PROTOCOL       *Heci;
  UINT32                  MeMode;
  HECI_FWS_REGISTER       MeFirmwareStatus;
  UINTN                   Address;
  UINT32                  Data;
  UINT32                  MebxSetupVariableAttributes;
  UINTN                   MebxSetupVariableDataSize;
  ME_BIOS_EXTENSION_SETUP MeBiosExtensionSetup;
  const UINT8             Str5MBFw[sizeof (MEFW_5M_STRING)]     = MEFW_5M_STRING;
  EFI_MEBX_PROTOCOL       *MebxProtocol;

  //    INITIALIZE_SCRIPT (ParentImageHandle, gST);  
  //    BootScriptSaveInitialize


  ///
  /// Check whether this is real ExitPmAuth notification, or just a SignalEvent
  ///
  Status = gBS->LocateProtocol (&gExitPmAuthProtocolGuid, NULL, &ProtocolPointer);
  if (EFI_ERROR (Status)) {
    return;
  }
  ///
  /// PMIR Configuration Save
  ///
  Status = gBS->LocateProtocol (
                  &gEfiHeciProtocolGuid,
                  NULL,
                  &Heci
                  );
  if (EFI_ERROR (Status)) {
    return;
  }
  ///
  /// Check ME Status
  ///
  Status = Heci->GetMeMode (&MeMode);
  ASSERT_EFI_ERROR (Status);

  MeFirmwareStatus.ul = HeciPciRead32 (R_FWSTATE);

  ///
  /// Init ME Policy Library
  ///
  Status = MePolicyLibInit ();
  if (EFI_ERROR (Status)) {
    return;
  }

#ifdef EFI_DEBUG
  ///
  /// Dump the ME platform policy
  ///
  DxeMePolicyDebugDump ();
#endif

  ///
  /// Report ME components version information to FVI
  ///
  InitFviDataHubCbContext (
    mDxePlatformMePolicy->MeMiscConfig.FviSmbiosType,
    (UINT8) mMeFviElements,
    &mMeFviVersionData
    );

  mMeFviElementsData[EnumMeFw].Element.Version.MajorVersion = (UINT8) mMbpData.MeBiosPayload.FwVersionName.MajorVersion;
  mMeFviElementsData[EnumMeFw].Element.Version.MinorVersion = (UINT8) mMbpData.MeBiosPayload.FwVersionName.MinorVersion;
  mMeFviElementsData[EnumMeFw].Element.Version.Revision = (UINT8) mMbpData.MeBiosPayload.FwVersionName.HotfixVersion;
  mMeFviElementsData[EnumMeFw].Element.Version.BuildNum = (UINT16) mMbpData.MeBiosPayload.FwVersionName.BuildVersion;
  if (mMbpData.MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType == INTEL_ME_5MB_FW) {
    CopyMem (mMeFviElementsData[EnumMeFw].Element.VerString, Str5MBFw, sizeof (MEFW_5M_STRING));
  }
  Status = gBS->LocateProtocol (&gEfiMebxProtocolGuid, NULL, &MebxProtocol);
  if (!EFI_ERROR (Status)) {
    mMeFviElementsData[EnumMebx].Element.Version.MajorVersion = (UINT8) MebxProtocol->MebxVersion.Major;
    mMeFviElementsData[EnumMebx].Element.Version.MinorVersion = (UINT8) MebxProtocol->MebxVersion.Minor;
    mMeFviElementsData[EnumMebx].Element.Version.Revision = (UINT8) MebxProtocol->MebxVersion.Hotfix;
    mMeFviElementsData[EnumMebx].Element.Version.BuildNum = (UINT16) MebxProtocol->MebxVersion.Build;
  }

  CreateRcFviDatahub (&mMeFviVersionData);

  ///
  /// PCH BIOS Spec Rev 0.9 Section 18.4	Additional Power Management Programming
  /// Step 2
  ///   Set "Power Management Initialization Register (PMIR) Field 1", D31:F0:ACh[31] = 1b
  ///   for production machine according to "RS - PCH Intel Management Engine
  ///  (Intel(r) ME) BIOS Writer's Guide".
  ///
  /// PCH ME BWG section 4.5.1
  /// The IntelR FPT tool /GRST option uses CF9GR bit to trigger global reset.
  /// Based on above reason, the BIOS should not lock down CF9GR bit during Manufacturing and
  /// Re-manufacturing environment.
  ///
  Address = PCI_LIB_ADDRESS (
              DEFAULT_PCI_BUS_NUMBER_PCH,
              PCI_DEVICE_NUMBER_PCH_LPC,
              PCI_FUNCTION_NUMBER_PCH_LPC,
              R_PCH_LPC_PMIR
              );
  Data = PciRead32 (Address);
  Data &= (UINT32) (~(B_PCH_LPC_PMIR_CF9LOCK | B_PCH_LPC_PMIR_CF9GR));

  if ((((MeMode == ME_MODE_NORMAL) || (MeMode == ME_MODE_TEMP_DISABLED)) && !(MeFirmwareStatus.r.ManufacturingMode))) {
    ///
    /// PCH ME BWG section 4.4.1
    /// BIOS must also ensure that CF9GR is cleared and locked (via bit31 of the same register) before
    /// handing control to the OS in order to prevent the host from issuing global resets and reseting
    /// Intel Management Engine.
    ///
    Data |= (UINT32) (B_PCH_LPC_PMIR_CF9LOCK);
  }
#ifdef RAPID_START_FLAG
  ///
  /// PMIR is a resume well register and has no script save for it.
  /// System may go through S3 resume path from G3 if RapidStart is enabled,
  /// that means all resume well registers will be reset to defaults.
  /// Save boot script for PMIR register if RapidStart is enabled.
  ///
  SCRIPT_MEM_WRITE (
    EFI_ACPI_S3_RESUME_SCRIPT_TABLE,
    EfiBootScriptWidthUint32,
    (UINTN) MmPciAddress (0x0,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            PCI_DEVICE_NUMBER_PCH_LPC,
            PCI_FUNCTION_NUMBER_PCH_LPC,
            R_PCH_LPC_PMIR),
    1,
    &Data
    );
#endif
  ///
  ///
  /// Read RCBA register for saving
  ///
  Data = Mmio16 (PCH_RCRB_BASE, R_PCH_RCRB_FD2);
  ///
  /// Disable Heci2 if policy dictates
  ///
  ///
  ///
    Data |= (BIT0 << HECI2);

  ///
  /// If ME Mode is running in ME Temp Disable state, disable Heci1, HECI2, Ider and Sol
  ///
  if ((MeMode == ME_MODE_TEMP_DISABLED) || (MeMode == ME_MODE_SECOVER)) {
    Data |= ((BIT0 << HECI1) + (BIT0 << HECI2) + (BIT0 << IDER) + (BIT0 << SOL));
  }

  if (MeMode == ME_MODE_NORMAL) {
    if (mHeciContext->MeFwImageType == INTEL_ME_1_5MB_FW) {
      ///
      /// We will disable HECI2, Ider and Sol in 1.5M SKU
      ///
      Data |= ((BIT0 << HECI2) + (BIT0 << IDER) + (BIT0 << SOL));
    } else if (mHeciContext->MeFwImageType == INTEL_ME_5MB_FW) {
      MebxSetupVariableAttributes = EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE;
      MebxSetupVariableDataSize = sizeof (ME_BIOS_EXTENSION_SETUP);

      Status = gST->RuntimeServices->GetVariable (
                                      gEfiMeBiosExtensionSetupName,
                                      &gEfiMeBiosExtensionSetupGuid,
                                      &MebxSetupVariableAttributes,
                                      &MebxSetupVariableDataSize,
                                      &MeBiosExtensionSetup
                                      );
      if (!EFI_ERROR (Status)) {
        if ((MeBiosExtensionSetup.AmtSolIder & SOL_ENABLE) == 0) {
          Data |= (BIT0 << SOL);
        }

        if ((MeBiosExtensionSetup.AmtSolIder & IDER_ENABLE) == 0) {
          Data |= (BIT0 << IDER);
        }
      }
    }
  }

  SCRIPT_MEM_WRITE (
    EFI_ACPI_S3_RESUME_SCRIPT_TABLE,
    EfiBootScriptWidthUint16,
    (UINTN) (PCH_RCRB_BASE + R_PCH_RCRB_FD2),
    1,
    &Data
    );
  ///
  /// Function Disable SUS Well lockdown
  ///
  Data = Mmio16 (PCH_RCRB_BASE, R_PCH_RCRB_FDSW);

  if (MeMode == ME_MODE_NORMAL) {
    if (mHeciContext->MeFwImageType == INTEL_ME_5MB_FW) {
      Data |= B_PCH_RCRB_FDSW_FDSWL;
    }
  }

  SCRIPT_MEM_WRITE (
    EFI_ACPI_S3_RESUME_SCRIPT_TABLE,
    EfiBootScriptWidthUint16,
    (UINTN) (PCH_RCRB_BASE + R_PCH_RCRB_FDSW),
    1,
    &Data
    );

  gBS->CloseEvent (Event);
  return;
}

VOID
PerformIderWorkaround (
  IN EFI_HANDLE Handle
  )
/**

  @brief
  ME BWG 1.0 5.3.1.1: IDER Workaround, perform this only when IDER device is present.

  @param[in] None


**/
{
  EFI_STATUS  Status;
  UINT64      BaseAddress;
  UINT64      BaseAddress2;
  UINT64      Index;
  BOOLEAN     WorkaroundFlag;

  WorkaroundFlag = TRUE;

  Status = gDS->AllocateIoSpace (
                  EfiGcdAllocateAnySearchBottomUp,
                  EfiGcdIoTypeIo,
                  4,
                  0x10,
                  &BaseAddress,
                  Handle,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Program BAR4
  ///
  PciWrite32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, IDER_FUNCTION_NUMBER, 0x20), (UINT32) BaseAddress);

  ///
  /// Enable IDER IOE
  ///
  PciOr8 (
    PCI_LIB_ADDRESS (ME_BUS,
    ME_DEVICE_NUMBER,
    IDER_FUNCTION_NUMBER,
    PCI_COMMAND_OFFSET),
    EFI_PCI_COMMAND_IO_SPACE
    );

  ///
  /// Perform the workaround if offset 3 bit 0 is not set
  ///
  if ((IoRead8 ((UINTN) BaseAddress + 3) & 0x01) == 00) {
    Status = gDS->AllocateIoSpace (
                    EfiGcdAllocateAnySearchBottomUp,
                    EfiGcdIoTypeIo,
                    4,
                    0x10,
                    &BaseAddress2,
                    Handle,
                    NULL
                    );
    ASSERT_EFI_ERROR (Status);
    PciWrite32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, IDER_FUNCTION_NUMBER, 0x10), (UINT32) BaseAddress2);
    ///
    /// check all ports to make sure all are 0x7f before running the workaround
    ///
    for (Index = 0; Index <= 7; Index++) {
      if (IoRead8 ((UINTN) BaseAddress2 + (UINTN) Index) != 0x7f) {
        WorkaroundFlag = FALSE;
        break;
      }
    }
    ///
    /// Disable IDER IOE and clear BAR0 and BAR4
    ///
    PciAnd8 (
      PCI_LIB_ADDRESS (ME_BUS,
      ME_DEVICE_NUMBER,
      IDER_FUNCTION_NUMBER,
      PCI_COMMAND_OFFSET),
      (UINT8)~EFI_PCI_COMMAND_IO_SPACE
      );
    PciWrite32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, IDER_FUNCTION_NUMBER, 0x10), 0);
    PciWrite32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, IDER_FUNCTION_NUMBER, 0x20), 0);

    if (WorkaroundFlag) {
      IderDisable ();
    }

    gDS->FreeIoSpace (BaseAddress2, (UINT64) 0x10);
  } else {
    PciAnd8 (
      PCI_LIB_ADDRESS (ME_BUS,
      ME_DEVICE_NUMBER,
      IDER_FUNCTION_NUMBER,
      PCI_COMMAND_OFFSET),
      (UINT8)~EFI_PCI_COMMAND_IO_SPACE
      );
    PciWrite32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, IDER_FUNCTION_NUMBER, 0x20), 0);
  }

  gDS->FreeIoSpace (BaseAddress, (UINT64) 0x10);
  return ;
}

EFI_STATUS
InstallMbpDataProtocol (
  VOID
  )
/**

  @brief
  Install MbpData protocol.

  @param[in] None


**/
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT32      MeMode;

  ZeroMem (&mMbpData, sizeof (DXE_MBP_DATA_PROTOCOL));

  HeciGetMeMode (&MeMode);

  ///
  ///
  mMbpData.Revision = DXE_MBP_DATA_PROTOCOL_REVISION_2;

  PERF_START (NULL, EVENT_REC_TOK" 0x8010", NULL, AsmReadTsc());
  Status = PrepareMeBiosPayload (&mMbpData.MeBiosPayload, &mHeciContext->MpbClearEvent);
  PERF_END (NULL, EVENT_REC_TOK" 0x8011", NULL, AsmReadTsc());

  if (!EFI_ERROR (Status)) {

#ifdef EFI_DEBUG
    ///
    /// Dump the Mbp data
    ///     
    DxeMbpDebugDump (&mMbpData);
#endif

    ///
    /// Install the MBP protocol
    ///
    mMbpData.Handle = NULL;
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &mMbpData.Handle,
                    &gMeBiosPayloadDataProtocolGuid,
                    &mMbpData,
                    NULL
                    );
  }
  if (EFI_ERROR (Status)) {
     DEBUG ((EFI_D_ERROR, "MBP data protocol install failed, Status is %r \n", Status));
  }

  return Status;
}

#endif // AMT_SUPPORT