/**

Copyright (c) 2012 - 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file
  AtAm.c

@brief
 This file contines routines responsible for hangling AT and provide interface for creating UI.

**/

/**
 This file contains an 'Intel Peripheral Driver' and is
 licensed for Intel CPUs and chipsets under the terms of your
 license agreement with Intel or your vendor.  This file may
 be modified by the user, subject to additional terms of the
 license agreement
**/
#include <Library/BaseLib.h>
#include <Framework/DxeCis.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include "Protocol/AtPlatformPolicy.h"
#include "MeState.h"
#include "AtAm.h"
#include "Library/MeTypeLib.h"

DXE_AT_POLICY_PROTOCOL          *mDxePlatformAtPolicy;
static AT_BIOS_RECOVERY_CONFIG  gBiosRecoveryGlobalVar;
static AT_STATE_INFO            gAtStateGlobalVar;
static UINT16                   gRecoveryAMGlobalVar;

EFI_GUID  gEfiPBAVariableGuid  = PBA_FAILED_COUNT_VARIABLE_GUID;

extern AT_INSTANCE  mAtInstance;

ATAM_INSTANCE AtAmInstance = {
  ATAM_PRIVATE_DATA_SIGNATURE,
  NULL,
  {
    (UINT8) ATAM_PROTOCOL_REVISION,
    (EFI_ATAM_GET_ISV_ID) AtAmGetIsvId,
    (EFI_ATAM_GET_RECOVERY_CONFIG) AtAmGetRecoveryConfig,
    (EFI_ATAM_GET_TIMER) AtAmGetTimer,
    (EFI_ATAM_GET_NONCE) AtAmGetNonce,
    (EFI_ATAM_VERIFY_PASSWORD) AtAmVerifyPassword,
    (EFI_ATAM_GET_ATTEMPTS) AtAmGetAttempts,
    (EFI_ATAM_SET_SUSPEND_STATE) AtAmSetSuspendState,
    (EFI_ATAM_GET_AT_STATE_INFO) AtAmGetAtStateInfo
  }
};

EFI_STATUS
AtAmEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
/**

  Driver entry point.

  @retval EFI_SUCCESS       The function completed successfully.

**/
{
  EFI_STATUS                    Status;
  UINT32                        MeStatus;
  EFI_HECI_PROTOCOL             *Heci;
  UINT32                        TimeOut;
  UINT8                         AtState;
  UINT16                        AtAmPref;
  DXE_MBP_DATA_PROTOCOL         *MbpData;

#if defined(SPS_SUPPORT) && SPS_SUPPORT
  if (!MeTypeIsAmt()) {
    return EFI_UNSUPPORTED;
  }
#endif // SPS_SUPPORT


  MeStatus  = 0;
  AtState   = 0;
  AtAmPref  = 0;

  ///
  ///Install AT protocol
  ///
  Status = AtEntryPoint(0, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "ATAM: AT Protocol isn't installed, Status = %r\n", Status));
    return Status;
  }

  Status = gBS->LocateProtocol (
                  &gDxePlatformAtPolicyGuid,
                  NULL,
                  &mDxePlatformAtPolicy
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "ATAM: No AT Platform Policy Protocol available"));
    return Status;
  }
  ASSERT_EFI_ERROR (Status);

#ifdef EFI_DEBUG
  ///
  /// Dump the AT platform policy
  ///
  DxeAtPolicyDebugDump ();
#endif

  Status = gBS->LocateProtocol (
                  &gEfiHeciProtocolGuid,
                  NULL,
                  &Heci
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "ATAM: failed to locate HECI driver, Status = %r\n", Status));
    return Status;
  }

  Status = Heci->GetMeStatus (&MeStatus);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Get the MBP Data.
  ///
  Status = gBS->LocateProtocol (&gMeBiosPayloadDataProtocolGuid, NULL, &MbpData);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "ATAM: No MBP Data Protocol available - Exit Early"));
    return EFI_SUCCESS;
  }

  if (mDxePlatformAtPolicy->At.AtAmBypass == 0) {
    TimeOut = 0;
    while (ME_STATUS_ME_STATE_ONLY(MeStatus) != ME_READY &&
            ME_STATUS_ME_STATE_ONLY(MeStatus) != ME_IN_RECOVERY_MODE) {
      gBS->Stall (1000000);
      TimeOut++;
      if (TimeOut > 10) {
        ASSERT_EFI_ERROR (MeStatus);
        break;
      }
      Heci->GetMeStatus (&MeStatus);
    }

    ///
    /// Check if ME is Normal State or Recovery Mode
    ///
    if(ME_STATUS_ME_STATE_ONLY(MeStatus) == ME_READY ||
               ME_STATUS_ME_STATE_ONLY(MeStatus) == ME_IN_RECOVERY_MODE) {
      ///
      /// Get the MBP Data.
      ///
      Status = gBS->LocateProtocol (&gMeBiosPayloadDataProtocolGuid, NULL, &MbpData);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "ATAM: No MBP Data Protocol available - Exit Early"));
        return EFI_SUCCESS;
      }

      if (!MbpData->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.IntelAT) {
        DEBUG ((EFI_D_INFO, "ATAM: Exit Early - MEFWCAPS_SKU_RULE_ID indicates AT does not exist"));
        return EFI_SUCCESS;
      } else {
        DEBUG ((EFI_D_INFO, "ATAM: ME FW SKU Info Variables indicates that AT exists"));
      }

      DEBUG ((EFI_D_INFO, "ATAM::GetMeStatus is ME_READY\n", Status));

      Status = GetAtStateInfo (
                &gAtStateGlobalVar
                );
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "ATAM::GetAtStateInfo failed, Status = %r\n", Status));
        return Status;
      }

      ///
      /// Update Static Variable
      ///
      gRecoveryAMGlobalVar = gAtStateGlobalVar.flags.AuthenticateModule;

      DEBUG ((EFI_D_INFO, "ATAM: MeAtRuleDate AtState = %x\n", gAtStateGlobalVar.State));
      DEBUG ((EFI_D_INFO, "ATAM: MeAtRuleDate AtAmPref = %x\n", gAtStateGlobalVar.flags.AuthenticateModule));

      ///
      /// Check for PBA_ERROR_THRESHOLDS Level .... i.e. if PBA fails for x number of times then BIOS AM will
      /// Ignore the PREFERRED_AM Selection
      ///
      AtAmPref = gAtStateGlobalVar.flags.AuthenticateModule;
      AtState  = gAtStateGlobalVar.State;

      Status = AtAmValidatePreferredAM (&AtState, &AtAmPref);
      DEBUG ((EFI_D_INFO, "ATAM: ValidatePreferredAM, AtAmPref = %d\n", gAtStateGlobalVar.flags.AuthenticateModule));
      ///
      /// Ignore the PREFERRED_AM Selection
      ///
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_INFO, "ATAM: ValidatePreferredAM failed, Status = %r\n", Status));
      }
      Status = gBS->InstallMultipleProtocolInterfaces (
                      &(AtAmInstance.Handle),
                      &gEfiAtAmProtocolGuid,
                      &(AtAmInstance.AtAmProtocol),
                      NULL
                      );
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "ATAM: ATAM Protocol NOT Instaled, Status = %r\n", Status));
      } else {
        DEBUG ((EFI_D_ERROR, "ATAM: ATAM Protocol Instaled, Status = %r\n", Status));
      }
    } else {
      DEBUG ((EFI_D_INFO, "ATAM: ME_READY Failed\n"));
      Status = EFI_DEVICE_ERROR;
    }
  } else {
    DEBUG ((EFI_D_INFO, "ATAM: AT Disabled in the BIOS\n"));

    Status = GetAtStateInfo (
              &gAtStateGlobalVar
              );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "ATAM::GetAtStateInfo failed, Status = %r\n", Status));
      return Status;
    }

    Status = gBS->InstallMultipleProtocolInterfaces (
                    &(AtAmInstance.Handle),
                    &gEfiAtAmProtocolGuid,
                    &(AtAmInstance.AtAmProtocol),
                    NULL
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "ATAM: ATAM Protocol NOT Installed, Status = %r\n", Status));
    } else {
      DEBUG ((EFI_D_ERROR, "ATAM: ATAM Protocol Installed, Status = %r\n", Status));
    }
  }

  return Status;
}

EFI_STATUS
AtAmGetIsvId (
  IN  EFI_AT_PROTOCOL *This,
  OUT UINT8           *IsvString,
  OUT UINT32          *IsvId
  )
/**

  This function gets the ISV Strings stored by AT Server that BIOS will display.

  @param[in] This        The address of protocol
  @param[out] IsvString  Isv string pointer
  @param[out] IsvId      Intel(R) Anti-Theft service provider Id

  @retval EFI_SUCCESS   The function completed successfully.

**/
{
  EFI_STATUS      Status;
  UINT32          StringLength;
  UINT32          StringId;

  StringLength = 0;

  Status = mAtInstance.AtProtocol.GetIsvId (&mAtInstance.AtProtocol, IsvId);
  if (EFI_ERROR (Status)) {
    ///
    ///  Let it continue even if the error ....
    ///
    DEBUG ((EFI_D_ERROR, "ATAM: GetIsvId failed, Status = %r\n", Status));
  } else {
    DEBUG ((EFI_D_ERROR, "ATAM: GetIsvId IsvId = %r\n", *IsvId));
  }

  StringId  = AT_VENDOR_STRING_ID_RECOVERY_HELP;
  Status    = mAtInstance.AtProtocol.GetRecoveryString (&mAtInstance.AtProtocol, &StringId, IsvString, &StringLength);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "ATAM: GetRecoveryString AT_VENDOR_STRING_ID_RECOVERY_HELP failed, Status = %r\n", Status));
  ///
  /// Let it continue even if the error
  ///
  }

  return Status;
}

EFI_STATUS
AtAmGetTimer (
  IN  EFI_AT_PROTOCOL *This,
  OUT UINT32          *TimeLeft,
  OUT UINT32          *TimeInterval
  )
/**

  This function returns time left to enter password.

  @param[in] This          The address of protocol
  @param[out] TimeLeft     Time
  @param[out] TimeInterval Time interval

  @retval EFI_SUCCESS   The function completed successfully.

**/
{
  EFI_STATUS      Status;

  Status = mAtInstance.AtProtocol.GetTimerInfo (&mAtInstance.AtProtocol, TimeInterval, TimeLeft);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "ATAM: GetTimerInfo failed, Status = %r\n", Status));
  }

  return Status;
}

EFI_STATUS
AtAmGetNonce (
  IN  EFI_AT_PROTOCOL *This,
  OUT UINT8           *NonceStr
  )
/**

  This function gets 16 bytes nonce from firmware and also converts it to string according to format "XXXX-XXXX-XXXX-XXXX-XXXX-XXXX-XX".

  @param[in] This          The address of protocol
  @param[out] NonceStr     Pointer to Nonce string

  @retval EFI_SUCCESS   The function completed successfully.

**/
{
  EFI_STATUS      Status;
  UINTN           StrNonceLength;
  UINT8           Nonce[NONCE_LENGTH];
  CHAR16          *UniCodeNonceStr;

  StrNonceLength = STR_NONCE_LENGTH;

  UniCodeNonceStr = AllocateZeroPool ((STR_NONCE_LENGTH + 1) * sizeof (CHAR16));
  if (UniCodeNonceStr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = mAtInstance.AtProtocol.GetNonce (&mAtInstance.AtProtocol, Nonce);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "ATAM: GetSuspendAuthentication::GetNonce failed, Status = %r\n", Status));
//Grangeville AptioV server Override - CPP Check
    FreePool(UniCodeNonceStr);
    return Status;
  }

  Base32Encode (NonceStr, &StrNonceLength, Nonce, NONCE_LENGTH);
  Uint8ToUnicode (NonceStr, UniCodeNonceStr);

  FreePool (UniCodeNonceStr);

  return Status;
}

EFI_STATUS
AtAmGetRecoveryConfig (
  IN  EFI_AT_PROTOCOL         *This,
  OUT AT_BIOS_RECOVERY_CONFIG *RecoveryConfig
  )
/**

  This function gets recovery config.

  @param[in]  This            The address of protocol
  @param[out] RecoveryConfig  Pointer to structure

  @retval EFI_SUCCESS   The function completed successfully.

**/
{
  EFI_STATUS      Status;
  UINT32          StringId;
  UINT32          StringLength;

  StringLength = 0;

  StringId  = AT_CUSTOM_RECOVERY_ID_CONFIGURATIONS;
  Status    = mAtInstance.AtProtocol.GetRecoveryString (&mAtInstance.AtProtocol, &StringId, (UINT8 *) RecoveryConfig, &StringLength);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "ATAM: GetRecoveryString failed, Status = %r\n", Status));
    ///
    /// Let it continue even if the error
    ///
  } else {
    DEBUG ((EFI_D_ERROR, "ATAM: GetRecoveryString StringLength = %d\n", StringLength));
    DEBUG ((EFI_D_ERROR, "ATAM: GetRecoveryString *IsvString = %p\n", RecoveryConfig));
    DEBUG ((EFI_D_ERROR, "ATAM: GetRecoveryString StringId = %d\n", StringId));
  }

  return Status;
}

EFI_STATUS
AtAmVerifyPassword (
  IN  EFI_AT_PROTOCOL *This,
  IN  UINT8           *PasswordEntered,
  IN  UINT32          PassType,
  OUT UINT8           *IsAuthenticated
  )
/**

  This routine receives the data (passphrase or SRTK) from UI and verifies it if the password (either passphrase or SRTK) is acceptable.

  @param[in]  This            The address of protocol
  @param[in]  PasswordEntered Pointer to string
  @param[in]  PassType        Password type
  @param[out] IsAuthenticated Pointer to result of verification

  @retval EFI_SUCCESS   The function completed successfully.

**/
{
  EFI_STATUS      Status;
  UINT8           *Hash;
  UINT8           *PasswordHex;
  UINT8           AtState;
  UINT16          AtAmPref;

  Status = EFI_SUCCESS;

  PasswordHex = AllocateZeroPool ((ATAM_SETUP_PASSWORD_LENGTH + 1) * sizeof (UINT8));
  Hash        = AllocateZeroPool ((ATAM_SETUP_PASSWORD_LENGTH + 1) * sizeof (UINT8));
  if (Hash == NULL || PasswordHex == NULL) {
//Grangeville AptioV server override start - CPP Check
    if (Hash != NULL)
      FreePool(Hash);
    if( PasswordHex != NULL)
      FreePool(PasswordHex);
//Grangeville AptioV server override end - CPP Check
    return EFI_OUT_OF_RESOURCES;
  }

  if (PassType == AT_CREDENTIAL_TYPE_USER_PASSPHRASE) {

    Status = mAtInstance.AtProtocol.ComputeHash (&mAtInstance.AtProtocol, PasswordEntered, Hash);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "ATAM: ComputeHash failed, Status = %r\n", Status));
    }

    Status = mAtInstance.AtProtocol.AuthenticateCredential (&mAtInstance.AtProtocol, Hash, &PassType, IsAuthenticated);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "ATAM: AuthenticateCredential, Status = %r\n", Status));
    }

  } else if (PassType == AT_CREDENTIAL_TYPE_SRTK) {
    ///
    /// User selected Base32 based Server based recovery, for BASE32 the length is always 32
    ///
    if (AsciiStrLen(PasswordEntered) == 32) {
      DEBUG ((EFI_D_ERROR, "ATAM: Password entered is SRTK BASE32 PASSWORD\n"));
      ///
      /// Decoded value is 20 byte hex
      ///
      Base32Decode (PasswordEntered, PasswordHex);
    } else if (AsciiStrLen(PasswordEntered) > 40) {
    ///
    /// User selected Basee10 based Server based recovery, for BASE10 the length is always more than 40
    ///
      DEBUG ((EFI_D_ERROR, "ATAM: Password entered is SRTK BASE10 PASSWORD\n"));
      ///
      /// Decoded value is 20 byte hex
      ///
      DecimalToHexString (PasswordEntered, PasswordHex, MAX_HEX_BYTES - 1);
    }

    Status = mAtInstance.AtProtocol.AuthenticateCredential (&mAtInstance.AtProtocol, PasswordHex, &PassType, IsAuthenticated);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "ATAM: AuthenticateCredential, Status = %r\n", Status));
    } else {
      ///
      /// Calling ValidatePreferredAM to reset the PbafailedCount
      ///
      if (gAtStateGlobalVar.flags.AuthenticateModule == AT_AM_SELECTION_PBAM) {
        AtState  = AT_STATE_ACTIVE;
        AtAmPref = AT_AM_SELECTION_PBAM;
        Status    = AtAmValidatePreferredAM (&AtState, &AtAmPref);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "ATAM: AtAmValidatePreferredAM failed, Status = %r\n", Status));
        }
      }
    }

  } else {
    DEBUG ((EFI_D_ERROR, "ATAM: Unknown Pass type \n"));
  }

  ZeroMem (Hash, (ATAM_SETUP_PASSWORD_LENGTH + 1) * sizeof (UINT8));
  ZeroMem (PasswordHex, (ATAM_SETUP_PASSWORD_LENGTH + 1) * sizeof (UINT8));
  FreePool (Hash);
  FreePool (PasswordHex);

  return Status;
}

EFI_STATUS
AtAmGetAttempts (
  IN  EFI_AT_PROTOCOL *This,
  OUT UINT32          *Attempts
  )
/**

  This routine returns number of attempts.

  @param[in]  This      The address of protocol
  @param[out] Attempts  Number of attempts

  @retval EFI_SUCCESS   The function completed successfully.

**/
{
  *Attempts = mDxePlatformAtPolicy->At.AtRecoveryAttempt;

  return EFI_SUCCESS;
}

EFI_STATUS
AtAmSetSuspendState (
  IN EFI_AT_PROTOCOL  *This,
  IN UINT32           TransitionState,
  IN UINT8            *Token
  )
/**

  This routine receives the SRTK from UI and verifies it if the password is acceptable. This requests FW to enter or exit Suspend mode based on user input.

  @param[in] This             The address of protocol
  @param[in] TransitionState  1- enter suspend state, 0 - exit suspend state
  @param[in] Token            Pointer to token

  @retval EFI_SUCCESS   The function completed successfully.

**/
{
  EFI_STATUS  Status;
  UINT8       TokenHex[ATAM_SETUP_PASSWORD_LENGTH];

  DEBUG ((EFI_D_ERROR, "ATAM: SetSuspendState: Transition state = %x\n", TransitionState));

  if(AsciiStrLen(Token) == 32) {
    DEBUG ((EFI_D_ERROR, "ATAM: Suspend Password entered is in Base32\n"));
    Base32Decode(Token, TokenHex);
  } else if(AsciiStrLen(Token) > 40) {
    DEBUG ((EFI_D_ERROR, "ATAM: Password entered is in Base10\n"));
    DecimalToHexString(Token,TokenHex, MAX_HEX_BYTES - 1);
  } else {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((EFI_D_ERROR, "ATAM: SrtkPass = %x\n", TokenHex));

  Status = mAtInstance.AtProtocol.SetSuspendState (&mAtInstance.AtProtocol, TransitionState, TokenHex);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "ATAM: SetSuspendState failed, Status = %r\n", Status));
  } else {
    DEBUG ((EFI_D_ERROR, "ATAM:  Successfully set suspended state\n"));
  }

  return Status;
}

EFI_STATUS
AtAmGetAtStateInfo (
  IN  EFI_AT_PROTOCOL *This,
  OUT AT_STATE_INFO   *AtStateInfo
  )
/**

  This routine gets AT state.

  @param[in]  This         The address of protocol
  @param[out] AtStateInfo  State of AT

  @retval EFI_SUCCESS   The function completed successfully.

**/
{

  AtStateInfo->State                       = gAtStateGlobalVar.State;
  AtStateInfo->LastTheftTrigger            = gAtStateGlobalVar.LastTheftTrigger;
  AtStateInfo->flags.LockState             = gAtStateGlobalVar.flags.LockState;
  AtStateInfo->flags.AuthenticateModule    = gAtStateGlobalVar.flags.AuthenticateModule;
  AtStateInfo->flags.S3Authentication      = gAtStateGlobalVar.flags.S3Authentication;
  AtStateInfo->flags.FlashVariableSecurity = gAtStateGlobalVar.flags.FlashVariableSecurity;
  AtStateInfo->flags.FlashWearOut          = gAtStateGlobalVar.flags.FlashWearOut;

  return EFI_SUCCESS;
}

EFI_STATUS
AtAmValidatePreferredAM (
  IN UINT8  *AtState,
  IN UINT16 *AtAmPref
  )
/**

  Validate preffered AM.

  @param[in] AtState   State of AT
  @param[in] AtAmPref  Preffered AT authentication

  @retval EFI_SUCCESS  The function completed successfully.

**/
{
  EFI_STATUS              Status;
  UINTN                   VarSize;
  UINT8                   PbaFailedCount;
  UINT32                  Attributes;
  AT_BIOS_RECOVERY_CONFIG RecoveryConfig;

  VarSize = sizeof (UINT8);

  if (*AtAmPref == AT_AM_SELECTION_ATAM) {
    return EFI_SUCCESS;
  }

  if (*AtState == AT_STATE_ACTIVE) {

    Attributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS;
    Status = gRT->GetVariable (
                    L"PBA_FAILED_COUNT",
                    &gEfiPBAVariableGuid,
                    &Attributes,
                    &VarSize,
                    &PbaFailedCount
                    );
    if (EFI_ERROR (Status) && Status != EFI_NOT_FOUND) {
      DEBUG ((EFI_D_ERROR, "ATAM: ValidatePreferredAM failed, Status = %r\n", Status));
      return Status;
    }

    if (Status == EFI_NOT_FOUND || PbaFailedCount > 0) {

      DEBUG ((EFI_D_INFO, "ATAM: ValidatePreferredAM In State Active PbaFailedCount not yet defined\n"));
      ///
      /// This will be the case 1st time after enrollment when PbaFailedCount i.e PREFERRED_AM is not defined
      /// Define this variable here
      ///
      PbaFailedCount = 0;
      Status = gRT->SetVariable (
                      L"PBA_FAILED_COUNT",
                      &gEfiPBAVariableGuid,
                      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                      sizeof (UINT8),
                      &PbaFailedCount
                      );
    }
  }
  ///
  /// Now get NVRAM Varible that store the PBA_Preferred_Failed count .. i.e. STOLEN_STATE which
  /// which is incremented every boot
  ///
  if (*AtState == AT_STATE_STOLEN && *AtAmPref == AT_AM_SELECTION_PBAM) {

    AtAmGetRecoveryConfig (NULL, &RecoveryConfig);

    VarSize     = sizeof (UINT8);
    Attributes  = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS;
    Status = gRT->GetVariable (
                    L"PBA_FAILED_COUNT",
                    &gEfiPBAVariableGuid,
                    &Attributes,
                    &VarSize,
                    &PbaFailedCount
                    );
    if (EFI_ERROR (Status) && Status != EFI_NOT_FOUND) {
      DEBUG ((EFI_D_ERROR, "ATAM: ValidatePreferredAM failed, Status = %r\n", Status));
      return Status;
    }

    if (Status == EFI_NOT_FOUND) {
      DEBUG ((EFI_D_ERROR, "ATAM: ValidatePreferredAM In State Stolen PbaFailedCount not yet defined\n"));

      ///
      /// This will be the case 1st time after enrollment when PbaFailedCount i.e PREFERRED_AM is not defined and
      /// AT somehow got into stolen mode
      /// Define this variable here
      ///
      PbaFailedCount = 1;
      Status = gRT->SetVariable (
                      L"PBA_FAILED_COUNT",
                      &gEfiPBAVariableGuid,
                      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                      sizeof (UINT8),
                      &PbaFailedCount
                      );
    } else if (PbaFailedCount > 0 && PbaFailedCount == (UINT8) RecoveryConfig.PbaOverRideThreshold) {
      ///
      /// Set the PREFERRED_AM to BIOS AM here
      ///
      *AtAmPref = AT_AM_SELECTION_ATAM;
      DEBUG ((EFI_D_ERROR, "ATAM: ValidatePreferredAM PbaFailedCount  = %d\n", PbaFailedCount));
    } else {
      ///
      /// Increment the PbaFailedCount count here
      ///
      PbaFailedCount = PbaFailedCount + 1;
      Status = gRT->SetVariable (
                      L"PBA_FAILED_COUNT",
                      &gEfiPBAVariableGuid,
                      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                      sizeof (UINT8),
                      &PbaFailedCount
                      );

      DEBUG((EFI_D_INFO, "ATAM: ValidatePreferredAM PbaFailedCount incremented here:%d\n", PbaFailedCount));
      DEBUG((EFI_D_INFO, "ATAM: ValidatePreferredAM gBiosRecoveryGlobalVar.PbaOverRideThreshold:%d\n",
            RecoveryConfig.PbaOverRideThreshold));
    }
  }

  return EFI_SUCCESS;
}
