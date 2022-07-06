/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++
Copyright (c)  2009 - 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  IccCallbacks.c

Abstract:

  Setup hooks for ICC.

Simple flow description:
  0) IccSetup variable in flash values are temporary. ICC settings are not assumed to be valid across BIOS reboots
  1) InitIccHandles() is called, it initializes handle needed for string manipulation and reads MPB information
  2) if Setup is entered, ICCOnEnterSetup() is called. It sends lots of HECI messages and initializes all ICC-related setup data
  3) if user modifies ICC-related options, appropriate Callback function is called
  4) when settings are saved in browser, RouteConfig() is called, it sends HECI msgs with new settings to FW
  5) there's a global variable gIccReset that upgrades next reset type to power cycle - this is needed for ICC changes to be applied

--*/

#if defined(ICC_SUPPORT) && ICC_SUPPORT

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include "PchAccess.h"
#include <Protocol/HiiConfigAccess.h>
#include <Guid/SetupVariable.h>
#include <Guid/GlobalVariable.h>
#include <Guid/HiiSetupEnter.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Protocol/Wdt.h>
#include <Protocol/PlatformType.h>
#include "IccSetup.h"
#include "Protocol/IccOverClocking.h"
#include "IccCallbacks.h"
#include "IccPlatform.h"
#include "MeState.h"
#include "MeLib.h"
#include <Library/MeTypeLib.h>
#include "Configuration.h"
#include "Protocol/IccDataProtocol.h"

#define STRING_BUFFER_SIZE 100
#define NUM_USAGES         14
#define NOT_INITIALIZED    0xFF
#define HZ_TO_10KHZ        10000

void UpdateSubmenuStrings (IN UINT8 SubmenuNumber, IN ICC_CLOCK_RANGES Ranges);
void ApplyClockSettings (IN ICC_CLOCK_SETTINGS RequestedClockSettings, IN ICC_CLOCK_ID ClockNumber);
void MaybeChangeLockMask (UINT8 LockMask);
void MaybeChangeProfile (UINT8 Profile);

static ICC_CLOCK_SETTINGS  mSettingsClock2;
static ICC_CLOCK_SETTINGS  mSettingsClock3;
static ICC_CLOCK_RANGES    mRangesClock2;
static ICC_CLOCK_RANGES    mRangesClock3;
static BOOLEAN             mProfileChanged = FALSE;
ICC_MBP_DATA               gIccMbpData;
static BOOLEAN             gClockChanged[MAX_UI_CLOCKS] = {0};

EFI_HII_HANDLE             gHiiHandle = NULL;
ICC_OVERCLOCKING_PROTOCOL* gIccOverClockingProtocol = NULL;
static EFI_HECI_PROTOCOL*  mHeci = NULL;
static ICC_DATA_PROTOCOL*  mIccData = NULL;

BOOLEAN                    gIccReset = FALSE;

extern                     EFI_GUID gSignalBeforeEnterSetupGuid;

BOOLEAN IsAfterEndOfPost (void);

UINT8  GetBoardId(
  void
 )
{
  UINT8                      BoardId = 0;
  EFI_PLATFORM_TYPE_PROTOCOL *PlatformInfoProtocol;
  EFI_STATUS                 Status = gBS->LocateProtocol (&gEfiPlatformTypeProtocolGuid, NULL, &PlatformInfoProtocol);

  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_INFO, "ERROR: Cannot get PlatformType information\n"));
  } else {
    BoardId = PlatformInfoProtocol->BoardId;
    DEBUG ((EFI_D_INFO, "Read BoardId is %d\n", BoardId));
  }

  return BoardId;
}

void
UpdateClockData (
  IN  ICC_CLOCK_SETTINGS ClockSettings,
  IN  UINT8 ClockNumber,
  OUT ICC_SETUP_DATA* SetupPtr
 )
/*++
--*/
{
  SetupPtr->Frequency[ClockNumber]  = (UINT16) ( (ClockSettings.Frequency + (HZ_TO_10KHZ/2)) / HZ_TO_10KHZ);
  SetupPtr->Spread[ClockNumber] = ClockSettings.SscPercent;
  SetupPtr->DmiPegRatio[ClockNumber] = ClockSettings.DmiPegRatio;
}

void
ExtractClockData (
  IN  SYSTEM_CONFIGURATION* SetupPtr,
  IN  UINT8 ClockNumber,
  OUT ICC_CLOCK_SETTINGS* ClockSettings
 )
/*++
--*/
{
  ClockSettings->Frequency   = HZ_TO_10KHZ * SetupPtr->Frequency[ClockNumber];
  ClockSettings->SscPercent  = SetupPtr->Spread[ClockNumber];
  ClockSettings->SscMode     = ICC_SSC_DOWN;
  ClockSettings->DmiPegRatio = SetupPtr->DmiPegRatio[ClockNumber];
}

EFI_STATUS
EFIAPI
IccCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION Action,
  IN EFI_QUESTION_ID KeyValue,
  IN UINT8 Type,
  IN EFI_IFR_TYPE_VALUE *Value,
  OUT EFI_BROWSER_ACTION_REQUEST *ActionRequest OPTIONAL
)
{

  // TODO: get defaults from PreUob record instead hardcoding
  if (!MeTypeIsAmt()) {
    return EFI_SUCCESS;
  }

  if (IsAfterEndOfPost()) {
    return EFI_UNSUPPORTED;
  }

  DEBUG ((EFI_D_ERROR, "(ICC) callback: key %d(%x), action %x\n", KeyValue, KeyValue, Action));

  switch (KeyValue) {

    case KEY_ICC_RATIO2:
      if (Action >= EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        gClockChanged[CLOCK2] = TRUE;
        Value->u8 = 0; // set ratio to 5:5
        return EFI_SUCCESS;
      } else if (Action == EFI_BROWSER_ACTION_CHANGING) {
        gClockChanged[CLOCK2] = TRUE;
        return EFI_SUCCESS;
      } else {
        return EFI_UNSUPPORTED;
      }

    case KEY_ICC_FREQ2:
      if (Action >= EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        gClockChanged[CLOCK2] = TRUE;
        Value->u16 = 10000;
        return EFI_SUCCESS;
      } else if (Action == EFI_BROWSER_ACTION_CHANGING) {
        gClockChanged[CLOCK2] = TRUE;
        if (Value->u16 < (UINT16)(mRangesClock2.FrequencyMin / HZ_TO_10KHZ)) {
          Value->u16 = (UINT16)(mRangesClock2.FrequencyMin / HZ_TO_10KHZ);
        } else if (Value->u16 > (UINT16)(mRangesClock2.FrequencyMax / HZ_TO_10KHZ)) {
          Value->u16 = (UINT16)(mRangesClock2.FrequencyMax / HZ_TO_10KHZ);
        }
        return EFI_SUCCESS;
      } else {
        return EFI_UNSUPPORTED;
      }

    case KEY_ICC_FREQ3:
      if (Action >= EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        gClockChanged[CLOCK3] = TRUE;
        Value->u16 = 10000;
        return EFI_SUCCESS;
      } else if (Action == EFI_BROWSER_ACTION_CHANGING) {
        gClockChanged[CLOCK3] = TRUE;
        if (Value->u16 < (UINT16)(mRangesClock3.FrequencyMin / HZ_TO_10KHZ)) {
          Value->u16 = (UINT16)(mRangesClock3.FrequencyMin / HZ_TO_10KHZ);
        } else if (Value->u16 > (UINT16)(mRangesClock3.FrequencyMax / HZ_TO_10KHZ)) {
          Value->u16 = (UINT16)(mRangesClock3.FrequencyMax / HZ_TO_10KHZ);
        }
        return EFI_SUCCESS;
      } else {
      return EFI_UNSUPPORTED;
      }

    case KEY_ICC_SPREAD2:
      if (Action >= EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        gClockChanged[CLOCK2] = TRUE;
        Value->u8 = 50;
        return EFI_SUCCESS;
      } else if (Action == EFI_BROWSER_ACTION_CHANGING) {
        gClockChanged[CLOCK2] = TRUE;
        if (Value->u8 > mRangesClock2.SscPercentMax) {
          Value->u8 = mRangesClock2.SscPercentMax;
        }
        return EFI_SUCCESS;
      } else {
      return EFI_UNSUPPORTED;
      }

    case KEY_ICC_SPREAD3:
      if (Action >= EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        gClockChanged[CLOCK3] = TRUE;
        Value->u8 = 50;
        return EFI_SUCCESS;
      } else if (Action == EFI_BROWSER_ACTION_CHANGING) {
        gClockChanged[CLOCK3] = TRUE;
        if (Value->u8 > mRangesClock3.SscPercentMax) {
          Value->u8 = mRangesClock3.SscPercentMax;
        }
        return EFI_SUCCESS;
      } else {
      return EFI_UNSUPPORTED;
      }

    default:
      return EFI_UNSUPPORTED;
  }
}

void
UpdateSubmenuStrings (
  IN UINT8 SubMenuNumber,
  IN ICC_CLOCK_RANGES Ranges
 )
{
  CHAR16  StringBuffer[STRING_BUFFER_SIZE];
  CHAR16* TempBuffer;
  UINT8   i;
  UINT8   NeedSlash;
  UINT16  StrClockTitle;
  UINT16  StrFreqRange;
  UINT16  StrMaxSpread;
  UINT16  StrSpreadAdjust;
  CHAR16* ClockUsageName[NUM_USAGES];

  if (gHiiHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "(ICC) gHiiHandle is NULL #1\n"));
    return;
  }

  if (!MeTypeIsAmt()) {
    return;
  }

  //
  // This function is shard by both clocks. We need to map the string tokens
  // before we use them later in the function. This is intended to avoid unneeded
  // if-else statements. Any new clock support will need to add a new case.
  //
  switch (SubMenuNumber)
  {
    case CLOCK2:
      StrClockTitle   =  STR_ICC_CLOCK2_TITLE;
      StrFreqRange    =  STR_ICC_CLOCK2_FREQ_RANGE_TEXT;
      StrMaxSpread    =  STR_ICC_CLOCK2_MAX_SPREAD_TEXT;
      StrSpreadAdjust =  STR_ICC_CLOCK2_SPREAD_ADJUSTMENTS_TEXT;
    break;

    case CLOCK3:
      StrClockTitle   =  STR_ICC_CLOCK3_TITLE;
      StrFreqRange    =  STR_ICC_CLOCK3_FREQ_RANGE_TEXT;
      StrMaxSpread    =  STR_ICC_CLOCK3_MAX_SPREAD_TEXT;
      StrSpreadAdjust =  STR_ICC_CLOCK3_SPREAD_ADJUSTMENTS_TEXT;
    break;

    default:
      DEBUG ((EFI_D_ERROR, "(ICC) Unsupported submenu detected\n"));
      return;
    break;
  }

  ClockUsageName[0] = HiiGetString(gHiiHandle, STRING_TOKEN(STR_ICC_CLOCK_USE_BCLK), NULL);
  ClockUsageName[1] = HiiGetString(gHiiHandle, STRING_TOKEN(STR_ICC_CLOCK_USE_DMI), NULL);
  ClockUsageName[2] = HiiGetString(gHiiHandle, STRING_TOKEN(STR_ICC_CLOCK_USE_PEG), NULL);
  ClockUsageName[3] = HiiGetString(gHiiHandle, STRING_TOKEN(STR_ICC_CLOCK_USE_PCIE), NULL);
  ClockUsageName[4] = HiiGetString(gHiiHandle, STRING_TOKEN(STR_ICC_CLOCK_USE_PCI33), NULL);
  ClockUsageName[5] = HiiGetString(gHiiHandle, STRING_TOKEN(STR_ICC_CLOCK_USE_RESERVED), NULL);
  ClockUsageName[6] = HiiGetString(gHiiHandle, STRING_TOKEN(STR_ICC_CLOCK_USE_SATA), NULL);
  ClockUsageName[7] = HiiGetString(gHiiHandle, STRING_TOKEN(STR_ICC_CLOCK_USE_USB3), NULL);
  ClockUsageName[8] = HiiGetString(gHiiHandle, STRING_TOKEN(STR_ICC_CLOCK_USE_GFX), NULL);
  ClockUsageName[9] = HiiGetString(gHiiHandle, STRING_TOKEN(STR_ICC_CLOCK_USE_GFX_NS), NULL);
  ClockUsageName[10] = HiiGetString(gHiiHandle, STRING_TOKEN(STR_ICC_CLOCK_USE_27_GFX), NULL);
  ClockUsageName[11] = HiiGetString(gHiiHandle, STRING_TOKEN(STR_ICC_CLOCK_USE_VGA), NULL);
  ClockUsageName[12] = HiiGetString(gHiiHandle, STRING_TOKEN(STR_ICC_CLOCK_USE_LEGACY_USB), NULL);
  ClockUsageName[13] = HiiGetString(gHiiHandle, STRING_TOKEN(STR_ICC_CLOCK_USE_14_MHZ), NULL);

  //
  // ClockUsage mask is defined in the LPT ICC FAS. Bit 5 of ClockUsage is reserved,
  // so let's pretend it's zero for purpose of displaying clock names
  //
  Ranges.UsageMask &= ~(BIT5);
  StringBuffer[0] = 0;
  NeedSlash = 0;

  //
  // Clear the title buffer before we process the usages
  //
  HiiSetString(gHiiHandle, STRING_TOKEN(StrClockTitle), StringBuffer, NULL);

  //
  // Search through the usage mask list and concatenate valid usages to string
  //
  for (i=0; i<NUM_USAGES; i++) {
    if (Ranges.UsageMask & (1<<i)) {
      if (NeedSlash == 1) {
        StrCat(StringBuffer, L"/");
      }
      StrCat(StringBuffer, ClockUsageName[i]);
      NeedSlash = 1;
    }
  }

  for (i=0; i<NUM_USAGES; i++) {
    FreePool(ClockUsageName[i]);
  }


  //
  // Add "Clock Settings" to the end of the string to complete formatting
  //
  TempBuffer = HiiGetString(gHiiHandle, STRING_TOKEN(STR_ICC_CLOCK_SETTINGS), NULL);
  StrCat(StringBuffer, TempBuffer);
  FreePool(TempBuffer);

  //
  // Copy formatted strings into clock settings menu
  //
  HiiSetString(gHiiHandle, STRING_TOKEN(StrClockTitle), StringBuffer, NULL);

  //
  // Frequency Range
  //
  UnicodeSPrint(StringBuffer, STRING_BUFFER_SIZE, L"%d.%02d - %d.%02d MHz",
                Ranges.FrequencyMin/1000000,
                (Ranges.FrequencyMin/10000)%100,
                Ranges.FrequencyMax/1000000,
                (Ranges.FrequencyMax/10000)%100
               );
  HiiSetString(gHiiHandle, STRING_TOKEN(StrFreqRange), StringBuffer, NULL);

  //
  // Maximum Spread %
  //
  UnicodeSPrint(StringBuffer, STRING_BUFFER_SIZE, L"%d.%02d%%",
                Ranges.SscPercentMax/100,
                Ranges.SscPercentMax%100
               );
  HiiSetString(gHiiHandle, STRING_TOKEN(StrMaxSpread), StringBuffer, NULL);

  //
  // Spread Mode Adjustments
  //
  if(Ranges.SscChangeAllowed) {
    TempBuffer = HiiGetString(gHiiHandle, STRING_TOKEN(STR_ICC_SSC_CHANGE_ALLOWED), NULL);
  } else if(Ranges.SscHaltAllowed) {
    TempBuffer = HiiGetString(gHiiHandle, STRING_TOKEN(STR_ICC_SSC_HALT_ALLOWED), NULL);
  } else {
    TempBuffer = HiiGetString(gHiiHandle, STRING_TOKEN(STR_ICC_SSC_NOTHING_ALLOWED), NULL);
  }
  HiiSetString(gHiiHandle, STRING_TOKEN(StrSpreadAdjust), TempBuffer, NULL);
  FreePool(TempBuffer);
}

/*++
Routine Description:
  Setup callback executed when user selects a ICC Profile from the BIOS UI.
  Changes visibility of other options
Arguments:
  interface to ITEM_CALLBACK_EX
Returns:
  always SUCCESS
--*/
EFI_STATUS
EFIAPI
IccProfileCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION Action,
  IN EFI_QUESTION_ID KeyValue,
  IN UINT8 Type,
  IN EFI_IFR_TYPE_VALUE *Value,
  OUT EFI_BROWSER_ACTION_REQUEST *ActionRequest OPTIONAL
 )
{
  UINTN                 Size = sizeof(SYSTEM_CONFIGURATION);
  SYSTEM_CONFIGURATION Setup;

  if (Action != EFI_BROWSER_ACTION_CHANGING && Action < EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
    return EFI_UNSUPPORTED;
  }

  if (!MeTypeIsAmt()) {
    return EFI_SUCCESS;
  }

  if (IsAfterEndOfPost()) {
      return EFI_UNSUPPORTED;
  }

  if (Action >= EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
    if (gIccMbpData.ProfileSelectionAllowed) {
      //
      // Perform change only if profile is selectable by BIOS
      //
      Value->u8 = 0;
    }
  }

  //
  // sanity check: can't choose profile that doesn't exist
  //
  if (Value->u8 > gIccMbpData.NumProfiles - 1) {
    Value->u8 = gIccMbpData.NumProfiles - 1;
  }

  //
  // when profile is changed, most other icc options can't be changed before reboot. hide those options.
  //
  if (mProfileChanged || (Value->u8 != gIccMbpData.Profile) ) {
    HiiGetBrowserData (&gEfiSetupVariableGuid, L"IntelSetup", Size, (UINT8*)&Setup); //AptioV server override
    Setup.AllowAdvancedOptions = DONT_DISPLAY;
    HiiSetBrowserData(&gEfiSetupVariableGuid, L"IntelSetup", sizeof(SYSTEM_CONFIGURATION), (UINT8 *)&Setup, NULL); //AptioV server override
  } else if (!mProfileChanged && (Value->u8 == gIccMbpData.Profile) ) { 
    HiiGetBrowserData (&gEfiSetupVariableGuid, L"IntelSetup", Size, (UINT8*)&Setup); //AptioV server override
    Setup.AllowAdvancedOptions = DISPLAY;
    HiiSetBrowserData(&gEfiSetupVariableGuid, L"IntelSetup", sizeof(SYSTEM_CONFIGURATION), (UINT8 *)&Setup, NULL); //AptioV server override
  }

  return EFI_SUCCESS;
}

void
ApplyClockSettings (
  IN ICC_CLOCK_SETTINGS RequestedClockSettings,
  IN ICC_CLOCK_ID       ClockNumber
 )
/*++
Routine Description:
  Executed by setup calback function
  Based on data entered by user, sends clock change requests to ICC OverClocking
  Writing to susram or flash requires that old susram and flash contents be invalidated
  In case of any problem, messagebox is displayed so user can know what corrective action is required
Arguments:
  initial clock divider value
Returns:
  validated clock divider value
--*/
{
  ICC_LIB_STATUS       IccStatus;
  EFI_STATUS           Status;
  WDT_PROTOCOL*        WdtProtocol;
  UINTN                VariableSize;
  UINT32			   Attributes;
  SYSTEM_CONFIGURATION SetupVariables;

  if (!MeTypeIsAmt()) {
    return;
  }

  gIccOverClockingProtocol->SetBootClockSettings(ClockNumber, RequestedClockSettings, &IccStatus);
  DEBUG ((EFI_D_INFO, "(ICC) ApplyClockSettings, IccStatus = %d\n", IccStatus));

  VariableSize = sizeof (SYSTEM_CONFIGURATION);
  ZeroMem (&SetupVariables, VariableSize);
  Status = gRT->GetVariable (
                  L"IntelSetup", //AptioV server override
                  &gEfiSetupVariableGuid,
                  &Attributes,
                  &VariableSize,
                  &SetupVariables
                  );
  ASSERT_EFI_ERROR (Status);

  switch (IccStatus) {
    case ICC_LIB_STATUS_SSC_TOO_LOW:
    case ICC_LIB_STATUS_FREQ_MUST_HAVE_ZERO_SSC:
    case ICC_LIB_STATUS_FREQ_AND_SSC_NOT_MATCH:
      RequestedClockSettings.SscPercent = 0;
      gIccOverClockingProtocol->SetBootClockSettings(ClockNumber, RequestedClockSettings, &IccStatus);
      DEBUG ((EFI_D_INFO, "(ICC) ApplyClockSettings - 2nd attempt with spread=0, IccStatus = %d\n", IccStatus));
      if (IccStatus == ICC_LIB_STATUS_SUCCESS) {
        // TODO: somehow force browser to display that spread has changed to 0
      }
      break;
    case ICC_LIB_STATUS_SSC_CHANGE_NOT_ALLOWED:
      RequestedClockSettings.SscPercent = 50;
      gIccOverClockingProtocol->SetBootClockSettings(ClockNumber, RequestedClockSettings, &IccStatus);
      DEBUG ((EFI_D_INFO, "(ICC) ApplyClockSettings - 2nd attempt with spread=50, IccStatus = %d\n", IccStatus));
      if (IccStatus == ICC_LIB_STATUS_SUCCESS) {
        // TODO: somehow force browser to display that spread has changed to 50
      }
      break;
    default:
      break;
  }
  if (IccStatus == ICC_LIB_STATUS_SUCCESS) {
    // Update the BclkFrequency setup option to sync with MRC and XTU
    if (ClockNumber == CLOCK2) {
      SetupVariables.BClkFrequency = RequestedClockSettings.Frequency/10000;
      Status = gRT->SetVariable (
                    L"IntelSetup", //AptioV server override
                    &gEfiSetupVariableGuid,
                    Attributes,
                    VariableSize,
                    &SetupVariables
                    );
      ASSERT_EFI_ERROR (Status);
      DEBUG ((EFI_D_INFO, "(ICC) BClkFrequency = %d\n", SetupVariables.BClkFrequency));
//      DEBUG ((EFI_D_INFO, "(ICC) BClkOverride  = %d\n", SetupVariables.BClkOverride));
    }

    Status = gBS->LocateProtocol(&gWdtProtocolGuid, NULL, &WdtProtocol);
    ASSERT_EFI_ERROR(Status);

    if (Status == EFI_SUCCESS) {
      WdtProtocol->AllowKnownReset();
    }
  }
}

void DebugDumpConfig(
  SYSTEM_CONFIGURATION *Setup
 )
{
  DEBUG ((EFI_D_INFO, "Frequency %d %d\n", Setup->Frequency[1], Setup->Frequency[2]));
  DEBUG ((EFI_D_INFO, "Spread %d %d\n", Setup->Spread[1], Setup->Spread[2]));
  DEBUG ((EFI_D_INFO, "DmiPegRatio %d %d\n", Setup->DmiPegRatio[1], Setup->DmiPegRatio[2]));
  DEBUG ((EFI_D_INFO, "ShowFrequency %d %d\n", Setup->ShowFrequency[1], Setup->ShowFrequency[2]));
  DEBUG ((EFI_D_INFO, "ShowSpread %d %d\n", Setup->ShowSpread[1], Setup->ShowSpread[2]));
  DEBUG ((EFI_D_INFO, "ShowRatio %d %d\n", Setup->ShowRatio[1], Setup->ShowRatio[2]));
  DEBUG ((EFI_D_INFO, "ShowClock %d %d\n", Setup->ShowClock[1], Setup->ShowClock[2]));
  DEBUG ((EFI_D_INFO, "ShowProfile %d\n", Setup->ShowProfile));
  DEBUG ((EFI_D_INFO, "Profile %d\n", Setup->IccProfile));
  DEBUG ((EFI_D_INFO, "LockMask %d\n", Setup->LockMask));
  DEBUG ((EFI_D_INFO, "ShowCustomLock %d\n", Setup->ShowCustomLock));
  DEBUG ((EFI_D_INFO, "AllowAdvancedOptions %d\n", Setup->AllowAdvancedOptions));
  DEBUG ((EFI_D_INFO, "PlatformType %d\n", Setup->PlatformType));
}


VOID
EFIAPI
ICCOnEnterSetup (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

void
InitIccHandles (
  EFI_HII_HANDLE HiiHandle
 )
{
  EFI_STATUS Status;
  UINTN      Size;
  EFI_EVENT  DummyEvent;
  VOID       *DummyRegistration;

  if (!MeTypeIsAmt()) {
    return;
  }

  if(gHiiHandle == NULL) {
    DEBUG ((EFI_D_INFO, "(ICC) IccCallbacks: Advanced Hii Handle is set\n"));
    gHiiHandle = HiiHandle;
    //
    // Get data retrieved from MBP
    //
    if (mIccData == NULL) {
      Status = gBS->LocateProtocol (&gIccDataProtocolGuid, NULL, &mIccData);
      if (!EFI_ERROR(Status)) {
        Status = mIccData->GetMpbData(&gIccMbpData);
      }
      if (EFI_ERROR(Status)) {
        DEBUG ((EFI_D_ERROR, "(ICC) Data was not received via IccDataProtocol, check variables.\n"));
        mIccData = NULL;
        Size = sizeof(gIccMbpData);
        Status = gRT->GetVariable (ICC_MBP_DATA_NAME,
                                   &gIccDataGuid,
                                   NULL,
                                   &Size,
                                   &gIccMbpData
                                  );
      }
      if (!EFI_ERROR(Status)) {
        Status = gBS->CreateEvent (EVT_NOTIFY_SIGNAL,
                                   TPL_CALLBACK,
                                   ICCOnEnterSetup,
                                   NULL,
                                   &DummyEvent
                                  );
        ASSERT_EFI_ERROR (Status);
        if (!EFI_ERROR(Status)) {
          Status = gBS->RegisterProtocolNotify (&gEfiSetupEnterGuid,
                                                DummyEvent,
                                                &DummyRegistration
                                               );
        }
        DEBUG ((EFI_D_ERROR, "(ICC) Install BeforeEnterSetup handler (Status = %r).\n", Status));
      }
    }
  }
}

void
InitIccLibrary (
  ICC_LIB_VERSION* Version
 )
{
  ICC_LIB_STATUS  IccStatus;
  EFI_GUID        IccOverClockingProtocolGuid = ICC_OVERCLOCKING_PROTOCOL_GUID;
  EFI_STATUS      Status;
  UINT8           NumberOfClocks;
  static BOOLEAN  StringInit = FALSE;

  if (IsAfterEndOfPost() || StringInit) {
    return;
  }

  if (!MeTypeIsAmt()) {
    return;
  }

  Status = gBS->LocateProtocol(&IccOverClockingProtocolGuid, NULL, &gIccOverClockingProtocol);
  if (EFI_ERROR(Status) ) {
    return;
  }
  gIccOverClockingProtocol->GetInfo(Version, &NumberOfClocks, &IccStatus);
  if (IccStatus != ICC_LIB_STATUS_SUCCESS) {
    DEBUG ((EFI_D_INFO, "(ICC) IccOverClocking failed to start. IccStatus=0x%x, version = %d.%d.%d.%d\n", IccStatus, Version->Major, Version->Minor, Version->Hotfix, Version->Build));
    return;
  }

  gIccOverClockingProtocol->GetCurrentClockSettings(CLOCK2, &mSettingsClock2, &IccStatus);
  if (IccStatus != ICC_LIB_STATUS_SUCCESS && IccStatus != ICC_LIB_STATUS_NO_USAGE_DEFINED_FOR_THE_CLOCK) {
    return;
  }
  gIccOverClockingProtocol->GetClockRanges(CLOCK2, &mRangesClock2, &IccStatus);
  if (IccStatus != ICC_LIB_STATUS_SUCCESS && IccStatus != ICC_LIB_STATUS_NO_USAGE_DEFINED_FOR_THE_CLOCK) {
    return;
  }
  if (NumberOfClocks >= CLOCK3+1) {
    gIccOverClockingProtocol->GetCurrentClockSettings(CLOCK3, &mSettingsClock3, &IccStatus);
    if (IccStatus != ICC_LIB_STATUS_SUCCESS && IccStatus != ICC_LIB_STATUS_NO_USAGE_DEFINED_FOR_THE_CLOCK) {
      return;
    }
    gIccOverClockingProtocol->GetClockRanges(CLOCK3, &mRangesClock3, &IccStatus);
    if (IccStatus != ICC_LIB_STATUS_SUCCESS && IccStatus != ICC_LIB_STATUS_NO_USAGE_DEFINED_FOR_THE_CLOCK) {
      return;
    }
  }

  StringInit = TRUE;
}

void
UpdateVisibility(
  ICC_SETUP_DATA* SetupPtr
 )
{
  UINT32              MeMode;

  mHeci->GetMeMode (&MeMode);

  if (IsAfterEndOfPost() || (MeMode != ME_MODE_NORMAL)) {
    SetupPtr->AllowAdvancedOptions = DONT_DISPLAY;
  } else {
    SetupPtr->AllowAdvancedOptions = DISPLAY;
  }
  if (IsAfterEndOfPost() || (gIccMbpData.ProfileSelectionAllowed == DONT_DISPLAY) || (MeMode != ME_MODE_NORMAL)) {
    SetupPtr->ShowProfile = DONT_DISPLAY;
  } else {
    SetupPtr->ShowProfile = DISPLAY;
  }
  if (IsAfterEndOfPost() || mRangesClock2.UsageMask == 0) {
    SetupPtr->ShowClock[CLOCK2]= DONT_DISPLAY;
  } else {
    SetupPtr->ShowClock[CLOCK2] = DISPLAY;
  }
  if (IsAfterEndOfPost() || mRangesClock3.UsageMask == 0) {
    SetupPtr->ShowClock[CLOCK3] = DONT_DISPLAY;
  } else {
    SetupPtr->ShowClock[CLOCK3] = DISPLAY;
  }
  if (IsAfterEndOfPost() || mRangesClock2.UsageMask == 0 || mRangesClock2.FrequencyMax == mRangesClock2.FrequencyMin) {
    SetupPtr->ShowFrequency[CLOCK2]= DONT_DISPLAY;
    SetupPtr->ShowRatio[CLOCK2]= DONT_DISPLAY;
  } else {
    SetupPtr->ShowFrequency[CLOCK2] = DISPLAY;
    SetupPtr->ShowRatio[CLOCK2] = DISPLAY;
  }
  if (GetBoardId() == 1 /*TBD: e.g. TypeTrad */) {
    SetupPtr->ShowRatio[CLOCK2] = DONT_DISPLAY;
  }

  if (IsAfterEndOfPost() || mRangesClock3.UsageMask == 0 || mRangesClock3.FrequencyMax == mRangesClock3.FrequencyMin) {
    SetupPtr->ShowFrequency[CLOCK3] = DONT_DISPLAY;
  } else {
    SetupPtr->ShowFrequency[CLOCK3] = DISPLAY;
  }
  SetupPtr->ShowRatio[CLOCK3] = DONT_DISPLAY;

  if (IsAfterEndOfPost() || mRangesClock2.UsageMask == 0 || mRangesClock2.SscChangeAllowed == 0) {
    SetupPtr->ShowSpread[CLOCK2] = DONT_DISPLAY;
  } else {
    SetupPtr->ShowSpread[CLOCK2] = DISPLAY;
  }
  if (IsAfterEndOfPost() || mRangesClock3.UsageMask == 0 || mRangesClock3.SscChangeAllowed == 0) {
    SetupPtr->ShowSpread[CLOCK3] = DONT_DISPLAY;
  } else {
    SetupPtr->ShowSpread[CLOCK3] = DISPLAY;
  }

}

/* Function UpdateSetup() updates System Setup variable
   when ICC settings changes

   Returns:
     0 - update Failed (ERROR)
     1 - update executed (SUCCESS)
     2 - update not necessary (SUCCESS)
         ICC data are the same as stored in system setup
 */
int
UpdateSetup(ICC_SETUP_DATA  *IccSetup)
{
  UINTN                BufferSize = sizeof (SYSTEM_CONFIGURATION);
  UINT32			   Attributes;
  EFI_STATUS           Status;
  SYSTEM_CONFIGURATION SetupData;
  int                  i;
  int                  changed = 0;
  int                  retVal = 0;

  ZeroMem (&SetupData, BufferSize);
  Status = gRT->GetVariable( L"IntelSetup", //AptioV server override
                             &gEfiSetupVariableGuid,
                             &Attributes,
                             &BufferSize,
                             &SetupData
                           );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "(ICC) UpdateSetup()- ERROR: cannot get Setup data\n"));
    return retVal;
  }
  if (SetupData.IccProfile != IccSetup->Profile) {
    changed++;
    SetupData.IccProfile = IccSetup->Profile;
  }
  if (SetupData.ShowProfile != IccSetup->ShowProfile) {
    changed++;
    SetupData.ShowProfile = IccSetup->ShowProfile;
  }
  for (i = 0; i < MAX_UI_CLOCKS; i++) {
    if (SetupData.Frequency[i] != IccSetup->Frequency[i]) {
      changed++;
      SetupData.Frequency[i] = IccSetup->Frequency[i];
    }
    if (SetupData.Spread[i] != IccSetup->Spread[i]) {
      changed++;
      SetupData.Spread[i] = IccSetup->Spread[i];
    }
    if (SetupData.DmiPegRatio[i] != IccSetup->DmiPegRatio[i]) {
      changed++;
      SetupData.DmiPegRatio[i] = IccSetup->DmiPegRatio[i];
    }
    if (SetupData.ShowClock[i] != IccSetup->ShowClock[i]) {
      changed++;
      SetupData.ShowClock[i] = IccSetup->ShowClock[i];
    }
    if (SetupData.ShowFrequency[i] != IccSetup->ShowFrequency[i]) {
      changed++;
      SetupData.ShowFrequency[i] = IccSetup->ShowFrequency[i];
    }
    if (SetupData.ShowSpread[i] != IccSetup->ShowSpread[i]) {
      changed++;
      SetupData.ShowSpread[i] = IccSetup->ShowSpread[i];
    }
    if (SetupData.ShowRatio[i] != IccSetup->ShowRatio[i]) {
      changed++;
      SetupData.ShowRatio[i] = IccSetup->ShowRatio[i];
    }
  }
  if (SetupData.LockMask != IccSetup->LockMask) {
    changed++;
    SetupData.LockMask = IccSetup->LockMask;
  }
  if (SetupData.ShowCustomLock != IccSetup->ShowCustomLock) {
    changed++;
    SetupData.ShowCustomLock = IccSetup->ShowCustomLock;
  }
  if (SetupData.AllowAdvancedOptions != IccSetup->AllowAdvancedOptions) {
    changed++;
    SetupData.AllowAdvancedOptions = IccSetup->AllowAdvancedOptions;
  }
  if (SetupData.PlatformType != IccSetup->PlatformType) {
    changed++;
    SetupData.PlatformType = IccSetup->PlatformType;
  }

  if (changed) {
    Status = gRT->SetVariable( L"IntelSetup", //AptioV server override
                               &gEfiSetupVariableGuid,
                               Attributes,
                               sizeof (SYSTEM_CONFIGURATION),
                               &SetupData
                             );
    DEBUG ((EFI_D_ERROR, "(ICC) UpdateSetup(): Update setup data status = %r\n", Status));
    retVal = 1;
  } else {
    DEBUG ((EFI_D_ERROR, "(ICC) UpdateSetup(): No setup changes\n"));
    retVal = 2;
  }

  DebugDumpConfig(&SetupData);

  return retVal;
}

VOID
EFIAPI
ICCOnEnterSetup (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{

  ICC_SETUP_DATA  IccSetup = {0};
  EFI_STATUS      Status;
  UINTN           Size;
  ICC_LIB_VERSION Version = {0};
  CHAR16          StringBuffer[STRING_BUFFER_SIZE];

  DEBUG ((EFI_D_ERROR, "(ICC) ICCOnEnterSetup() called.\n"));
  if (gHiiHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "(ICC) gHiiHandle is NULL #2\n"));
    return;
  }

  if (!MeTypeIsAmt()) {
    return;
  }

  if (!mHeci) {
    Status = gBS->LocateProtocol (&gEfiHeciProtocolGuid, NULL, &mHeci);
    if (EFI_ERROR(Status)) {
      IccSetup.AllowAdvancedOptions = DONT_DISPLAY;
      IccSetup.ShowProfile = DONT_DISPLAY;
      DEBUG ((EFI_D_ERROR, "ERROR: HECI protocol not found.\n"));
      UpdateSetup(&IccSetup);
      return;
    }
  }

  Size = sizeof(IccSetup);
  Status = gRT->GetVariable (ICC_SETUP_DATA_C_NAME,
                            &gIccDataGuid,
                            NULL,
                            &Size,
                            &IccSetup);
  if (!EFI_ERROR(Status)) {
    //
    // if the variable exists, it means init was completed. Check for EoP only and hide everything if it's after EoP.
    //
    if (IsAfterEndOfPost()) {
      IccSetup.AllowAdvancedOptions = DONT_DISPLAY;
      IccSetup.ShowProfile = DONT_DISPLAY;
      gRT->SetVariable (ICC_SETUP_DATA_C_NAME,
                        &gIccDataGuid,
                        EFI_VARIABLE_BOOTSERVICE_ACCESS,
                        sizeof(IccSetup),
                        &IccSetup);
      UpdateSetup(&IccSetup);
    }
    DEBUG ((EFI_D_ERROR, "(ICC): ICC Setup variable found. Exit\n"));
    return;
  }
  IccSetup.PlatformType = GetBoardId();
  InitIccLibrary(&Version);

  UnicodeSPrint(StringBuffer, STRING_BUFFER_SIZE, L"%d.%d.%d.%d",
                Version.Major,
                Version.Minor,
                Version.Hotfix,
                Version.Build
               );
  HiiSetString(gHiiHandle, STRING_TOKEN(STR_ICC_LIB_VERSION_NR), StringBuffer, NULL);

  UpdateSubmenuStrings(CLOCK2, mRangesClock2);
  UpdateSubmenuStrings(CLOCK3, mRangesClock3);

  IccSetup.Profile        = gIccMbpData.Profile;
  IccSetup.LockMask       = gIccMbpData.LockMask;
  IccSetup.ShowCustomLock = (gIccMbpData.LockMask == OPTION_USE_OEM) ? DISPLAY : DONT_DISPLAY;

  UpdateClockData (mSettingsClock2, CLOCK2, &IccSetup);
  UpdateClockData (mSettingsClock3, CLOCK3, &IccSetup);
  UpdateVisibility(&IccSetup);

  Status = gRT->SetVariable (ICC_SETUP_DATA_C_NAME,
                             &gIccDataGuid,
                             EFI_VARIABLE_BOOTSERVICE_ACCESS,
                             sizeof(IccSetup),
                             &IccSetup);
  DEBUG ((EFI_D_ERROR, "(ICC) ICC Data store status = %r\n", Status));

  UpdateSetup(&IccSetup);
}

void
IccRouteConfig()
{
  SYSTEM_CONFIGURATION SetupVariables;
  UINTN                Size = sizeof(SYSTEM_CONFIGURATION);
  UINT8                ClockNumber;
  ICC_CLOCK_SETTINGS   ClockSettings;

  DEBUG ((EFI_D_ERROR, "(ICC) IccRouteConfig() called.\n"));
  if ((IsAfterEndOfPost()) || (mHeci == NULL) || (gHiiHandle == NULL)) {
    return;
  }

  if (!MeTypeIsAmt()) {
    return;
  }

  gRT->GetVariable (L"IntelSetup", //AptioV server override
                    &gEfiSetupVariableGuid,
                    NULL,
                    &Size,
                    &SetupVariables);

  MaybeChangeProfile(SetupVariables.IccProfile);
  MaybeChangeLockMask(SetupVariables.LockMask);

  if (!mProfileChanged && SetupVariables.AllowAdvancedOptions  && gIccOverClockingProtocol != NULL) {
    for (ClockNumber = CLOCK2; ClockNumber <= CLOCK3; ClockNumber++) {
      if (SetupVariables.ShowClock[ClockNumber] && gClockChanged[ClockNumber]) {
        gIccReset = TRUE;
        gClockChanged[ClockNumber] = FALSE;
        ExtractClockData(&SetupVariables, ClockNumber, &ClockSettings);
        ApplyClockSettings(ClockSettings, ClockNumber);
      }
    }
  }
}

void
MaybeChangeProfile (
  UINT8 Profile
 )
{
  static UINT8 LastSavedProfile;

  if (!mProfileChanged) {
    LastSavedProfile = gIccMbpData.Profile;
  }
  if (Profile != LastSavedProfile) {
    DEBUG ((EFI_D_INFO, "Changing profile: old %d, new %d\n", LastSavedProfile, Profile));
    HeciSetIccProfile (Profile);
    LastSavedProfile = Profile;
    mProfileChanged = TRUE;
    gIccReset = TRUE;
  }
}

// Compiler (.NET 2008) automatically inserts memcpy/memset fn calls
// in certain cases due to optimization.
// Disable optimization to solve compilation error
#pragma optimize( "g", off )
void
MaybeChangeLockMask (
  UINT8 LockMask
 )
{
  static UINT8        LastSavedLockMask = NOT_INITIALIZED;
  ICC_LOCK_REGS_INFO  LockMaskData = {0};

  if (LastSavedLockMask == NOT_INITIALIZED) {
    LastSavedLockMask = gIccMbpData.LockMask;
  }
  if (LockMask != LastSavedLockMask) {
    UINT8 BoardId = GetBoardId();

    DEBUG ((EFI_D_INFO, "Changing lockmask: old %d, new %d\n", LastSavedLockMask, LockMask));
    switch(LockMask)
    {
      case OPTION_LOCK_STATIC:
      //
      // For static register, the All Unlocked bit (AU) bit is cleared, Bundle Count = 3,
      // and the mask data is set to static reigster mask.
      //
      if (BoardId == 1) {  // TBD
        LockMaskData.RegBundles.BundlesCnt = STATIC_REGISTERS_BUNDLE_TRAD;
        LockMaskData.RegMask[0] = STATIC_REGISTERS_MASK0;
        LockMaskData.RegMask[1] = STATIC_REGISTERS_MASK1;
        LockMaskData.RegMask[2] = STATIC_REGISTERS_MASK2;
      } else if (BoardId == 1) { //TBD
        LockMaskData.RegBundles.BundlesCnt = STATIC_REGISTERS_BUNDLE_ULT;
        LockMaskData.RegMask[0] = STATIC_REGISTERS_MASK0;
      } else {
//        ASSERT(FALSE);
      }
        break;

      case OPTION_LOCK_ALL:
        //
        // For all locked, the All Unlocked bit (AU) bit is cleared, Bundle Count = 0,
        // and all the mask data is set to 0.
        //
        LockMaskData.RegBundles.AU = 0;
        LockMaskData.RegBundles.BundlesCnt = 0;
        break;

      case OPTION_UNLOCK_ALL:
        //
        // For all locked, the All Unlocked bit (AU) bit is set, Bundle Count = 0,
        // and all the mask data is set to 0.
        //
        LockMaskData.RegBundles.AU = 1;
        LockMaskData.RegBundles.BundlesCnt = 0;
        break;

      case OPTION_USE_OEM:
      default:
        return;

    }

// TBD    HeciLockIccRegisters (ICC_LOCK_ACCESS_MODE_SET, ICC_RESPONSE_MODE_SKIP, &LockMaskData);

    // TODO: Force setup to redraw screen and show Lock question without Custom

/* commended out because it doesn't work well with current browser implementation. browser refuses to update
   its state immediately after F4 (save changes) was pressed and this function executed.

    if (LockMask != OPTION_USE_OEM) {
      // make it impossible to select CUSTOM lock mask again
      // update both browser's internal variable and efi variable
      Size = sizeof(IccSetup);
      HiiGetBrowserData(&gIccDataGuid, ICC_SETUP_DATA_C_NAME, Size, (UINT8*)&IccSetup);
      IccSetup.ShowCustomLock = DONT_DISPLAY;
      HiiSetBrowserData(&gIccDataGuid, ICC_SETUP_DATA_C_NAME, sizeof(IccSetup), (UINT8*)&IccSetup, NULL);

      Size = sizeof(IccSetup);
      gRT->GetVariable (ICC_SETUP_DATA_C_NAME,
                        &gIccDataGuid,
                        NULL,
                        Size,
                        &IccSetup);
      IccSetup.ShowCustomLock = DONT_DISPLAY;
      gRT->SetVariable (ICC_SETUP_DATA_C_NAME,
                        &gIccDataGuid,
                        EFI_VARIABLE_BOOTSERVICE_ACCESS,
                        sizeof(IccSetup),
                        &IccSetup);
    }  */

  }

  LastSavedLockMask = LockMask;
}
#pragma optimize( "g", on )

#endif // ICC_SUPPORT

#if defined(AMT_SUPPORT) && AMT_SUPPORT
BOOLEAN
IsAfterEndOfPost (
  void
  )
{
  DXE_ME_POLICY_PROTOCOL  *DxePlatformMePolicy;
  EFI_STATUS              Status;

  Status = gBS->LocateProtocol (
                  &gDxePlatformMePolicyGuid,
                  NULL,
                  &DxePlatformMePolicy
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return DxePlatformMePolicy->MeConfig.EndOfPostDone;
}
#endif // AMT_SUPPORT