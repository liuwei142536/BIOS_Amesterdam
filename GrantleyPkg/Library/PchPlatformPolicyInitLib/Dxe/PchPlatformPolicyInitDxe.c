/** @file
  This file is SampleCode for Intel PCH DXE Platform Policy initialzation.

@copyright
  Copyright (c) 2009 - 2015 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement

**/

#include "PchPlatformPolicyInitDxe.h"
#include "Library/HeciMsgLib.h"
#include "Library/PchChipsetInitHobLib.h"
#if defined(AMT_SUPPORT) && AMT_SUPPORT
#include "Library/MeTypeLib.h"
#include "Guid/MeBiosExtensionSetup.h"
CHAR16                        gEfiMeBiosExtensionSetupName[]   = EFI_ME_BIOS_EXTENSION_SETUP_VARIABLE_NAME;
#endif  //AMT_SUPPORT
extern EFI_GUID gEfiPlatformInfoGuid;

//
// EFI_EVENT
//
EFI_EVENT mHeciEvent;

DXE_PCH_PLATFORM_POLICY_PROTOCOL  mPchPolicyData        = { 0 };
PCH_DEVICE_ENABLING               mPchDeviceEnabling    = { 0 };
PCH_USB_CONFIG                    mPchUsbConfig         = { 0 };
PCH_PCI_EXPRESS_CONFIG            mPchPciExpressConfig  = { 0 };
PCH_SATA_CONFIG                   mPchSataConfig        = { 0 };
#ifdef PCH_SERVER_BIOS_FLAG       
PCH_SATA_CONFIG                   mPchsSataConfig       = { 0 }; 
#endif // PCH_SERVER_BIOS_FLAG
PCH_AZALIA_CONFIG                 mPchAzaliaConfig      = { 0 };
PCH_SMBUS_CONFIG                  mPchSmbusConfig       = { 0 };
PCH_MISC_PM_CONFIG                mPchMiscPmConfig      = { 0 };
PCH_IO_APIC_CONFIG                mPchIoApicConfig      = { 0 };
PCH_SVID_SID_CONFIG               mPchSvidSidConfig        = { 0 };
PCH_DEFAULT_SVID_SID              mPchDefaultSvidSid    = { 0 };
PCH_LOCK_DOWN_CONFIG              mPchLockDownConfig    = { 0 };
PCH_THERMAL_CONFIG                mPchThermalConfig     = { 0 };
PCH_LPC_HPET_CONFIG               mPchHpetConfig        = { 0 };
PCH_LPC_SIRQ_CONFIG               mSerialIrqConfig      = { 0 };
PCH_DMI_CONFIG                    mPchDmiConfig         = { 0 };
PCH_PWR_OPT_CONFIG                mPchPwrOptConfig      = { 0 };
#ifdef PCH_SERVER_BIOS_FLAG       
PCH_EVA_CONFIG                    mPchEvaConfig         = { 0 };
PCH_RAID_DEVICE                   mPchRaidDevice        = { 0 };
#endif // PCH_SERVER_BIOS_FLAG
PCH_MISC_CONFIG                   mPchMiscConfig        = { 0 };
PCH_AUDIO_DSP_CONFIG              mAudioDspConfig       = { 0 };
PCH_SERIAL_IO_CONFIG              mSerialIoConfig      = { 0 };

///
/// Function implementations
///

/**
  Initilize Intel PCH DXE Platform Policy

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval EFI_SUCCESS             Initialization complete.
  @exception EFI_UNSUPPORTED      The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
InitDxePchPlatformPolicy (
  IN SYSTEM_CONFIGURATION         *SetupVariables,
  IN EFI_PLATFORM_TYPE_PROTOCOL   *mPlatformType
  )
{
  EFI_HANDLE  Handle;
  EFI_STATUS  Status;
  UINT8       PortIndex;
  UINTN       Index;
  PCH_SERIES  PchSeries;
  PchSeries = GetPchSeries();
  //
  // General intialization
  //
  mPchPolicyData.Revision             = DXE_PCH_PLATFORM_POLICY_PROTOCOL_REVISION_4;
  mPchPolicyData.BusNumber            = 0;

  mPchPolicyData.DeviceEnabling       = &mPchDeviceEnabling;
  mPchPolicyData.UsbConfig            = &mPchUsbConfig;
  mPchPolicyData.PciExpressConfig     = &mPchPciExpressConfig;
  mPchPolicyData.SataConfig           = &mPchSataConfig;
  mPchPolicyData.AzaliaConfig         = &mPchAzaliaConfig;
  mPchPolicyData.SmbusConfig          = &mPchSmbusConfig;
  mPchPolicyData.MiscPmConfig         = &mPchMiscPmConfig;
  mPchPolicyData.IoApicConfig         = &mPchIoApicConfig;
  mPchPolicyData.SvidSidConfig        = &mPchSvidSidConfig;
  mPchPolicyData.DefaultSvidSid       = &mPchDefaultSvidSid;
  mPchPolicyData.LockDownConfig       = &mPchLockDownConfig;
  mPchPolicyData.ThermalConfig        = &mPchThermalConfig;
  mPchPolicyData.HpetConfig           = &mPchHpetConfig;
  mPchPolicyData.SerialIrqConfig      = &mSerialIrqConfig;
  mPchPolicyData.DmiConfig            = &mPchDmiConfig;
  mPchPolicyData.PwrOptConfig         = &mPchPwrOptConfig;
  mPchPolicyData.MiscConfig           = &mPchMiscConfig;
  mPchPolicyData.AudioDspConfig       = &mAudioDspConfig;
  mPchPolicyData.SerialIoConfig       = &mSerialIoConfig;
#ifdef PCH_SERVER_BIOS_FLAG
  mPchPolicyData.sSataConfig          = &mPchsSataConfig; 
  mPchPolicyData.EvaConfig            = &mPchEvaConfig; 
  mPchPolicyData.RaidDevice           = &mPchRaidDevice;
#endif //PCH_SERVER_BIOS_FLAG
  mPchPolicyData.MphyRegisterPchInitSycnCallback = (PCH_MPHY_REGISTER_PCH_INIT_SYNC_CALLBACK)RegisterPchInitSyncCallback;

#ifdef DE_SKU
  /// Power Management WA
  mPchPolicyData.EnableThermalLockDown = FALSE;
#endif //DE_SKU

  ///
  /// PCH BIOS Spec Rev 0.5.0 Section 3.6 Flash Security Recommendations,
  /// Intel strongly recommends that BIOS sets the BIOS Interface Lock Down bit. Enabling this bit
  /// will mitigate malicious software attempts to replace the system BIOS option ROM with its own code.
  /// We always enable this as a platform policy.
  ///
  mPchLockDownConfig.BiosInterface          = PCH_DEVICE_ENABLE;
  mPchLockDownConfig.GlobalSmi              = PCH_DEVICE_ENABLE;
  mPchLockDownConfig.GpioLockDown           = PCH_DEVICE_DISABLE;
  mPchLockDownConfig.RtcLock                = PCH_DEVICE_ENABLE;
  
  mPchLockDownConfig.FlashLockDown    = 0;   //AptioV server override: Adding control for SPI Lock
  mPchLockDownConfig.GbeFlashLockDown    = 0;
  ///
  /// While BiosLock is enabled, BIOS can only be modified from SMM after ExitPmAuth.
  ///
  mPchLockDownConfig.BiosLock                 = PCH_DEVICE_ENABLE;
  ///
  /// If PchBiosLockIoTrapAddress is 0, BIOS will allocate available IO address with
  /// 256 byte range from GCD and pass it to PchBiosLockIoTrapAddress.
  ///
  mPchLockDownConfig.PchBiosLockIoTrapAddress = 0;
  ///
  /// Initialize policy to default values when variable isn't found.
  ///
  mPchDeviceEnabling.Lan                = PCH_DEVICE_ENABLE;
  mPchDeviceEnabling.Azalia             = 2;
  mPchDeviceEnabling.Sata               = PCH_DEVICE_ENABLE;
  mPchDeviceEnabling.Smbus              = PCH_DEVICE_ENABLE;
  mPchDeviceEnabling.PciClockRun        = PCH_DEVICE_ENABLE;
  mPchDeviceEnabling.Display            = 1;
  mPchDeviceEnabling.Crid               = PCH_DEVICE_DISABLE;
  mPchDeviceEnabling.SerialIoDma        = PCH_DEVICE_ENABLE;
  mPchDeviceEnabling.SerialIoI2c0       = PCH_DEVICE_ENABLE;
  mPchDeviceEnabling.SerialIoI2c1       = PCH_DEVICE_ENABLE;
  mPchDeviceEnabling.SerialIoSpi0       = PCH_DEVICE_ENABLE;
  mPchDeviceEnabling.SerialIoSpi1       = PCH_DEVICE_ENABLE;
  mPchDeviceEnabling.SerialIoUart0      = PCH_DEVICE_ENABLE;
  mPchDeviceEnabling.SerialIoUart1      = PCH_DEVICE_ENABLE;
  mPchDeviceEnabling.SerialIoSdio       = PCH_DEVICE_ENABLE;
  mPchDeviceEnabling.AudioDsp           = PCH_DEVICE_DISABLE;

  ///
  /// Init USB related setting
  ///
  InitPchUsbConfig (&mPchUsbConfig);

  ///
  /// PCI Express related settings from setup variable
  ///
  mPchPciExpressConfig.RootPortClockGating    = PCH_DEVICE_ENABLE;
  mPchPciExpressConfig.TempRootPortBusNumMin  = 2;
  mPchPciExpressConfig.TempRootPortBusNumMax  = 4;

  for (PortIndex = 0; PortIndex < GetPchMaxPciePortNum (); PortIndex++) {
    mPchPciExpressConfig.RootPort[PortIndex].Enable             = PCH_DEVICE_ENABLE;
    mPchPciExpressConfig.RootPort[PortIndex].FunctionNumber     = PortIndex;
    mPchPciExpressConfig.RootPort[PortIndex].PhysicalSlotNumber = PortIndex;
    mPchPciExpressConfig.RootPort[PortIndex].Aspm               = 4;
    mPchPciExpressConfig.RootPort[PortIndex].SlotImplemented    = 1;
    mPchPciExpressConfig.RootPort[PortIndex].L1Substates        = 3;

  }
  mPchPciExpressConfig.RootPort[7].HotPlug  = 1;
  mPchPciExpressConfig.NumOfDevAspmOverride = sizeof (mDevAspmOverride) / sizeof (PCH_PCIE_DEVICE_ASPM_OVERRIDE);
  mPchPciExpressConfig.DevAspmOverride      = mDevAspmOverride;
  mPchPciExpressConfig.EnableSubDecode      = 0;
  mPchPciExpressConfig.PchPcieSbdePort      = 0;
  mPchPciExpressConfig.RootPortFunctionSwapping = 1;
  mPchPciExpressConfig.VTdEnable            = 0;
 mPchPciExpressConfig.PchPcieGlobalAspm    = 0;

  for (PortIndex = 0; PortIndex < GetPchMaxSataPortNum (); PortIndex++) {
    mPchSataConfig.PortSettings[PortIndex].Enable           = PCH_DEVICE_ENABLE;
    mPchSataConfig.PortSettings[PortIndex].HotPlug          = PCH_DEVICE_DISABLE;
    mPchSataConfig.PortSettings[PortIndex].InterlockSw      = PCH_DEVICE_DISABLE;
    mPchSataConfig.PortSettings[PortIndex].External         = PCH_DEVICE_DISABLE;
    mPchSataConfig.PortSettings[PortIndex].SpinUp           = PCH_DEVICE_DISABLE;
    mPchSataConfig.PortSettings[PortIndex].SolidStateDrive  = PCH_DEVICE_DISABLE;
    mPchSataConfig.PortSettings[PortIndex].DevSlp           = PCH_DEVICE_DISABLE;
    mPchSataConfig.PortSettings[PortIndex].EnableDitoConfig = PCH_DEVICE_DISABLE;
    mPchSataConfig.PortSettings[PortIndex].DmVal            = 15;
    mPchSataConfig.PortSettings[PortIndex].DitoVal          = 625;
  }

  mPchSataConfig.RaidAlternateId  = PCH_DEVICE_DISABLE;
  mPchSataConfig.Raid0            = PCH_DEVICE_ENABLE;
  mPchSataConfig.Raid1            = PCH_DEVICE_ENABLE;
  mPchSataConfig.Raid10           = PCH_DEVICE_ENABLE;
  mPchSataConfig.Raid5            = PCH_DEVICE_ENABLE;
  mPchSataConfig.Irrt             = PCH_DEVICE_ENABLE;
  mPchSataConfig.OromUiBanner     = PCH_DEVICE_ENABLE;
  mPchSataConfig.HddUnlock        = PCH_DEVICE_ENABLE;
  mPchSataConfig.LedLocate        = PCH_DEVICE_ENABLE;
  mPchSataConfig.IrrtOnly         = PCH_DEVICE_ENABLE;
  mPchSataConfig.SmartStorage     = PCH_DEVICE_ENABLE;
  mPchSataConfig.OromUiDelay      = PchSataOromDelay2sec;
  mPchSataConfig.TestMode         = PCH_DEVICE_DISABLE;
  mPchSataConfig.SalpSupport      = PCH_DEVICE_ENABLE;
  mPchSataConfig.LegacyMode       = PCH_DEVICE_DISABLE;
  mPchSataConfig.SpeedSupport     = PchSataSpeedSupportDefault;

#ifdef PCH_SERVER_BIOS_FLAG
  for (PortIndex = 0; PortIndex < GetPchMaxsSataPortNum (); PortIndex++) {
    mPchsSataConfig.PortSettings[PortIndex].Enable           = PCH_DEVICE_ENABLE;
    mPchsSataConfig.PortSettings[PortIndex].HotPlug          = PCH_DEVICE_DISABLE;
    mPchsSataConfig.PortSettings[PortIndex].InterlockSw      = PCH_DEVICE_DISABLE;
    mPchsSataConfig.PortSettings[PortIndex].External         = PCH_DEVICE_DISABLE;
    mPchsSataConfig.PortSettings[PortIndex].SpinUp           = PCH_DEVICE_DISABLE;
    mPchsSataConfig.PortSettings[PortIndex].SolidStateDrive  = PCH_DEVICE_DISABLE;
    mPchsSataConfig.PortSettings[PortIndex].DevSlp           = PCH_DEVICE_DISABLE;
    mPchsSataConfig.PortSettings[PortIndex].EnableDitoConfig = PCH_DEVICE_DISABLE;
    mPchsSataConfig.PortSettings[PortIndex].DmVal            = 15;
    mPchsSataConfig.PortSettings[PortIndex].DitoVal          = 625;

  }

  mPchsSataConfig.RaidAlternateId  = PCH_DEVICE_DISABLE;
  mPchsSataConfig.Raid0            = PCH_DEVICE_ENABLE;
  mPchsSataConfig.Raid1            = PCH_DEVICE_ENABLE;
  mPchsSataConfig.Raid10           = PCH_DEVICE_ENABLE;
  mPchsSataConfig.Raid5            = PCH_DEVICE_ENABLE;
  mPchsSataConfig.Irrt             = PCH_DEVICE_ENABLE;
  mPchsSataConfig.OromUiBanner     = PCH_DEVICE_ENABLE;
  mPchsSataConfig.HddUnlock        = PCH_DEVICE_ENABLE;
  mPchsSataConfig.LedLocate        = PCH_DEVICE_ENABLE;
  mPchsSataConfig.IrrtOnly         = PCH_DEVICE_ENABLE;
  mPchsSataConfig.SmartStorage     = PCH_DEVICE_ENABLE;
  mPchsSataConfig.OromUiDelay      = PchSataOromDelay2sec;
  mPchsSataConfig.TestMode         = PCH_DEVICE_DISABLE;
  mPchsSataConfig.SalpSupport      = PCH_DEVICE_ENABLE;
  mPchsSataConfig.LegacyMode       = PCH_DEVICE_DISABLE;
  mPchsSataConfig.SpeedSupport     = PchSataSpeedSupportGen3;
  
  mPchEvaConfig.LockDown                                      = PCH_DEVICE_ENABLE;  // Set locks if required.

  //
  // Set Rapid Storage Technology Enterprise (RSTe) as default raid controller device id
  //
  mPchRaidDevice.DeviceId = PchRaidRSTe;
  
#endif // PCH_SERVER_BIOS_FLAG
  ///
  /// AzaliaConfig
  ///
  mPchAzaliaConfig.Pme            = PCH_DEVICE_DISABLE;
  mPchAzaliaConfig.ResetWaitTimer = 300;
  mPchAzaliaConfig.DS             = 1;
  mPchAzaliaConfig.DA             = 0;

  ///
  /// Reserved SMBus Address
  ///
  mPchSmbusConfig.NumRsvdSmbusAddresses = 4;
  mPchSmbusConfig.RsvdSmbusAddressTable = mSmbusRsvdAddresses;

  ///
  /// MiscPm Configuration
  ///
  mPchMiscPmConfig.PchDeepSxPol                         = PchDeepSxPolDisable;
  mPchMiscPmConfig.WakeConfig.PmeB0S5Dis                = PCH_DEVICE_DISABLE;
  mPchMiscPmConfig.WakeConfig.WolEnableOverride         = PCH_DEVICE_DISABLE;
  mPchMiscPmConfig.WakeConfig.Gp27WakeFromDeepSx        = PCH_DEVICE_ENABLE;
  mPchMiscPmConfig.PowerResetStatusClear.MeWakeSts      = PCH_DEVICE_ENABLE;
  mPchMiscPmConfig.PowerResetStatusClear.MeHrstColdSts  = PCH_DEVICE_ENABLE;
  mPchMiscPmConfig.PowerResetStatusClear.MeHrstWarmSts  = PCH_DEVICE_ENABLE;

  mPchMiscPmConfig.PchSlpS3MinAssert                    = PchSlpS350ms;
  mPchMiscPmConfig.PchSlpS4MinAssert                    = PchSlpS44s;
  mPchMiscPmConfig.PchSlpSusMinAssert                   = PchSlpSus4s;
  mPchMiscPmConfig.PchSlpAMinAssert                     = PchSlpA2s;
  mPchMiscPmConfig.PchPwrCycDur                         = 4;  // 4-5 seconds (PCH default setting)
  mPchMiscPmConfig.SlpStrchSusUp                        = PCH_DEVICE_DISABLE;
  mPchMiscPmConfig.SlpLanLowDc                          = PCH_DEVICE_DISABLE;

  ///
  /// Io Apic configuration
  ///
  mPchIoApicConfig.BdfValid                             = 1;
  mPchIoApicConfig.BusNumber                            = 0xF0;
  mPchIoApicConfig.DeviceNumber                         = 0x1F;
  mPchIoApicConfig.FunctionNumber                       = 0x00;
  //
  // Keep this setting disable for Grantley 
  //
  mPchIoApicConfig.IoApicEntry24_39                     = PCH_DEVICE_DISABLE;

  for (Index =0; Index < PCH_MAX_DEVICES; Index++){
      mPchSvidSidConfig.SvidSidItems[Index].DeviceNumber    = SvidSidInitTable[Index].DeviceNumber; 
      mPchSvidSidConfig.SvidSidItems[Index].FunctionNumber  = SvidSidInitTable[Index].FunctionNumber;
      mPchSvidSidConfig.SvidSidItems[Index].SvidRegOffset   = SvidSidInitTable[Index].SvidRegOffset;
      mPchSvidSidConfig.SvidSidItems[Index].DefaultSvidSid.SubSystemVendorId = SvidSidInitTable[Index].DefaultSvidSid.SubSystemVendorId;
      mPchSvidSidConfig.SvidSidItems[Index].DefaultSvidSid.SubSystemId       = SvidSidInitTable[Index].DefaultSvidSid.SubSystemId;
  }
  ///
  /// Default Svid Sdid configuration
  ///
  mPchDefaultSvidSid.SubSystemVendorId                  = V_PCH_INTEL_VENDOR_ID;
  mPchDefaultSvidSid.SubSystemId                        = V_PCH_DEFAULT_SID;

  ///
  /// Thermal configuration - Initialize policy to default values when variable isn't found.
  ///
  mPchThermalConfig.ThermalAlertEnable.TselLock                   = PCH_DEVICE_ENABLE;
  mPchThermalConfig.ThermalAlertEnable.TscLock                    = PCH_DEVICE_ENABLE;
  mPchThermalConfig.ThermalAlertEnable.TsmicLock                  = PCH_DEVICE_ENABLE;
  mPchThermalConfig.ThermalAlertEnable.PhlcLock                   = PCH_DEVICE_ENABLE;
  mPchThermalConfig.ThermalThrottling.TTLevels.SuggestedSetting   = PCH_DEVICE_ENABLE;
  mPchThermalConfig.ThermalThrottling.TTLevels.TTLock             = PCH_DEVICE_ENABLE;  // B_PCH_TBARB_TL_LOCK
  mPchThermalConfig.ThermalThrottling.TTLevels.TTState13Enable    = PCH_DEVICE_DISABLE; // B_PCH_TBARB_TL_TT13EN
  mPchThermalConfig.ThermalThrottling.TTLevels.TTEnable           = PCH_DEVICE_ENABLE;  // B_PCH_TBARB_TL_TTEN
  mPchThermalConfig.ThermalThrottling.TTLevels.T2Level            = TL_T3-1;            //        T3 > T2L
  mPchThermalConfig.ThermalThrottling.TTLevels.T1Level            = TL_T2-1;            // T2L >= T2 > T1L
  mPchThermalConfig.ThermalThrottling.TTLevels.T0Level            = TL_T1-1;            // T1L >= T1 > T0L
  mPchThermalConfig.ThermalThrottling.TTLevels.Phl                = TL_PHL;
  mPchThermalConfig.ThermalThrottling.TTLevels.Ctt                = TL_CTT;             // R_PCH_TBARB_CTT


  mPchThermalConfig.ThermalThrottling.TTLevels.PchCrossThrottling = PCH_DEVICE_ENABLE;
  mPchThermalConfig.ThermalThrottling.DmiHaAWC.SuggestedSetting   = PCH_DEVICE_ENABLE;
  mPchThermalConfig.ThermalThrottling.SataTT.SuggestedSetting     = PCH_DEVICE_ENABLE;
#ifdef PCH_SERVER_BIOS_FLAG
  mPchThermalConfig.ThermalThrottling.sSataTT.SuggestedSetting     = PCH_DEVICE_ENABLE;
#endif // PCH_SERVER_BIOS_FLAG

  //
  // Default disable for PCH thermal enable
  //
  mPchThermalConfig.ThermalDeviceEnable                           = PCH_DEVICE_DISABLE;
#ifdef DE_SKU
  mPchThermalConfig.EnablePmSyncFlow                              = PCH_DEVICE_DISABLE;
#endif //DE_SKU

#ifndef GRANGEVILLE_FLAG
  //
  // If AUTO = PCH thermal enable
  //  and AZTECSTHI = board
  //
  if ( (2 == SetupVariables->ThermalDeviceEnable) &&
  ( (TypeAztecCitySTHI == mPlatformType->BoardId) || (TypeHedtEv == mPlatformType->BoardId) ) ) {
    mPchThermalConfig.ThermalDeviceEnable = PCH_DEVICE_ENABLE;
  }
#endif  

  mPchThermalConfig.PchHotLevel = TL_PHL;

  ///
  /// HEPT Configuration
  ///
  mPchHpetConfig.BdfValid       = 1;
  for (Index = 0; Index < PCH_HPET_BDF_MAX; Index++) {
    mPchHpetConfig.Hpet[Index].BusNumber       = 0xF0;
    mPchHpetConfig.Hpet[Index].DeviceNumber    = 0x0F;
    mPchHpetConfig.Hpet[Index].FunctionNumber  = 0x00;
  }
  ///
  /// Initialize Serial IRQ Config
  ///
  mSerialIrqConfig.SirqEnable       = TRUE;
  mSerialIrqConfig.StartFramePulse  = PchSfpw4Clk;
  mSerialIrqConfig.SirqMode         = PchContinuousMode;

  ///
  /// DMI related settings
  ///
  mPchDmiConfig.DmiAspm         = PCH_DEVICE_ENABLE;
  mPchDmiConfig.DmiExtSync      = PCH_DEVICE_DISABLE;
  mPchDmiConfig.DmiIot          = PCH_DEVICE_DISABLE;

  ///
  /// Power Optimizer related settings
  ///
  mPchPwrOptConfig.PchPwrOptDmi      = PCH_DEVICE_ENABLE;
  mPchPwrOptConfig.PchPwrOptGbe      = PCH_DEVICE_ENABLE;
  mPchPwrOptConfig.PchPwrOptXhci     = PCH_DEVICE_DISABLE;
  mPchPwrOptConfig.PchPwrOptEhci     = PCH_DEVICE_DISABLE;
  mPchPwrOptConfig.PchPwrOptSata     = PCH_DEVICE_DISABLE;
#ifdef PCH_SERVER_BIOS_FLAG
  mPchPwrOptConfig.PchPwrOptsSata     = PCH_DEVICE_ENABLE;
#endif //PCH_SERVER_BIOS_FLAG
  mPchPwrOptConfig.MemCloseStateEn   = PCH_DEVICE_ENABLE;
  mPchPwrOptConfig.InternalObffEn    = PCH_DEVICE_ENABLE;
  mPchPwrOptConfig.ExternalObffEn    = PCH_DEVICE_DISABLE;  // De-feature OBFF from LPT-H/LPT-LP.
  mPchPwrOptConfig.NumOfDevLtrOverride      = 0;
  mPchPwrOptConfig.DevLtrOverride           = NULL;
  for (PortIndex = 0; PortIndex < GetPchMaxPciePortNum (); PortIndex++) {
    mPchPwrOptConfig.PchPwrOptPcie[PortIndex].LtrEnable             = PCH_DEVICE_ENABLE;
    //
    // De-feature OBFF from LPT-H/LPT-LP.
    // Doesn't enable Obff policy anymore.
    //
    mPchPwrOptConfig.PchPwrOptPcie[PortIndex].ObffEnable            = PCH_DEVICE_DISABLE;
  }
  mPchPwrOptConfig.LegacyDmaDisable  = PCH_DEVICE_DISABLE;

  ///
  /// Misc. Config
  ///
  /// FviSmbiosType is the SMBIOS OEM type (0x80 to 0xFF) defined in SMBIOS Type 14 - Group
  /// Associations structure - item type. FVI structure uses it as SMBIOS OEM type to provide
  /// version information. The default value is type 221.
  ///
  mPchMiscConfig.FviSmbiosType  = 0xDD;

  ///
  /// Audio Dsp Configuration
  ///
  mAudioDspConfig.AudioDspD3PowerGating     = PCH_DEVICE_ENABLE;
  mAudioDspConfig.AudioDspAcpiMode          = 1; //1: ACPI mode, 0: PCI mode
  mAudioDspConfig.AudioDspAcpiInterruptMode = 1; //1: ACPI mode, 0: PCI mode
  mAudioDspConfig.AudioDspBluetoothSupport  = PCH_DEVICE_DISABLE; // Bluetooth SCO disabled

  ///
  /// Serial IO Configuration
  ///
  mSerialIoConfig.SerialIoMode              = PchSerialIoIsAcpi;
  switch(PchStepping()) {
    default:
       mSerialIoConfig.SerialIoInterruptMode = PchSerialIoIsAcpi;
       break;
  }
  mSerialIoConfig.I2c0VoltageSelect = PchSerialIoIs18V;
  mSerialIoConfig.I2c1VoltageSelect = PchSerialIoIs33V;


  ///
  /// Update policy by platform setting
  ///
  UpdateDxePchPlatformPolicy (
     SetupVariables, 
     mPlatformType, 
     &mPchPolicyData
     );


#ifdef USB_PRECONDITION_ENABLE_FLAG
  ///
  /// Update Precondition option for S4 resume.
  /// Skip Precondition for S4 resume in case this boot may not connect BIOS USB driver.
  /// If BIOS USB driver will be connected always for S4, then disable below update.
  /// To keep consistency during boot, must enabled or disabled below function in both PEI and DXE
  /// PlatformPolicyInit driver.
  ///
  if (mPchUsbConfig.UsbPrecondition == TRUE) {
    if (GetBootModeHob () == BOOT_ON_S4_RESUME) {
      mPchUsbConfig.UsbPrecondition = FALSE;
      DEBUG ((EFI_D_INFO, "BootMode is BOOT_ON_S4_RESUME, disable Precondition\n"));
    }
  }
#endif  // USB_PRECONDITION_ENABLE_FLAG
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gDxePchPlatformPolicyProtocolGuid,
                  &mPchPolicyData,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;

}


/**
  This function installs Dxe Pch Platform Policy Protocol

  @param[in] DxePlatformPchPolicy            The PCH Platform Policy protocol instance

 
  @retval EFI_SUCCESS                     Initialization complete.
  @retval EFI_UNSUPPORTED                 The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES            Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR                Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
UpdateDxePchPlatformPolicy (  
  IN SYSTEM_CONFIGURATION                       *SetupVariables,
  IN EFI_PLATFORM_TYPE_PROTOCOL                 *mPlatformType,
  IN OUT DXE_PCH_PLATFORM_POLICY_PROTOCOL       *DxePlatformPchPolicy  
  )
{
  UINT8                             Index;  
  PCH_SERIES                        PchSeries; 
  EFI_BOOT_MODE                     mBootMode;
  EFI_PLATFORM_INFO                 *PlatformInfoHobData;
  EFI_HOB_GUID_TYPE                 *GuidHob;
#if defined(AMT_SUPPORT) && AMT_SUPPORT
  EFI_STATUS                        Status;
  UINTN                             VariableSize;
  ME_BIOS_EXTENSION_SETUP           MeBiosExtensionSetupData;
#endif //AMT_SUPPORT

  UINT8  PCHPortToSlotMap [] = {PCH_PtoSMap};  // Aptio V Server Override: Program PCH physical slot number based on SDL porting  

  //
  // Get PlatformInfoHobData
  //
  GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT(GuidHob);
  PlatformInfoHobData = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfoHobData);
  
  
  //DXE_PCH_PLATFORM_POLICY_PROTOCOL  *DxePlatformPchPolicy;
  //
  // Get current Boot Mode
  //
  mBootMode = GetBootModeHob ();

  PchSeries = GetPchSeries();   
  DxePlatformPchPolicy->Revision = DXE_PCH_PLATFORM_POLICY_PROTOCOL_REVISION_1;
  DxePlatformPchPolicy->BusNumber = 0;

  //
  // Print current Cpu Type and Stepping
  //
  DEBUG ((EFI_D_INFO, "Cpu Type= 0x%x, Cpu Stepping= 0x%x\n",
           PlatformInfoHobData->CpuType,
           PlatformInfoHobData->CpuStepping
         ));

  //
  // DeviceEnables
  //
  if (PchIsGbeRegionValid (PCH_RCRB_BASE) == FALSE) {
    //
    // GbE Region does not exist, so set Lan policy to disabled.
    //
    DxePlatformPchPolicy->DeviceEnabling->Lan = PCH_DEVICE_DISABLE;
  } else {
      if(PlatformInfoHobData->PchData.PchLanSupported){
        DxePlatformPchPolicy->DeviceEnabling->Lan = SetupVariables->PchLan;
      } else {
        DxePlatformPchPolicy->DeviceEnabling->Lan = PCH_DEVICE_DISABLE;
      }
  }

  DxePlatformPchPolicy->DeviceEnabling->Azalia        = SetupVariables->PchAzalia;
  DxePlatformPchPolicy->DeviceEnabling->Sata          = SetupVariables->PchSata;
#ifdef PCH_SERVER_BIOS_FLAG
  DxePlatformPchPolicy->DeviceEnabling->sSata         = SetupVariables->PchsSata;
#endif //PCH_SERVER_BIOS_FLAG
  DxePlatformPchPolicy->DeviceEnabling->Smbus         = PCH_DEVICE_ENABLE; //  always enable.
  DxePlatformPchPolicy->DeviceEnabling->PciClockRun   = SetupVariables->PchPciClockRun;
  DxePlatformPchPolicy->DeviceEnabling->Display       = SetupVariables->PchDisplay;
  DxePlatformPchPolicy->DeviceEnabling->Crid          = SetupVariables->PchCrid;

#ifdef DE_SKU
  //
  // PowerManagement WA
  //
  DxePlatformPchPolicy->EnableThermalLockDown        = SetupVariables->PchEnableThermalLockDown;
#endif //DE_SKU

  //
  // LockDown
  //
  DxePlatformPchPolicy->LockDownConfig->GpioLockDown  = SetupVariables->PchGpioLockDown;
  DxePlatformPchPolicy->LockDownConfig->RtcLock       = SetupVariables->PchRtcLock;
  DxePlatformPchPolicy->LockDownConfig->BiosLock      = SetupVariables->PchBiosLock;
  DxePlatformPchPolicy->LockDownConfig->FlashLockDown  = SetupVariables->PchFlashLockDown;  //AptioV server override: Adding control for SPI Lock
  DxePlatformPchPolicy->LockDownConfig->GbeFlashLockDown  = SetupVariables->PchGbeFlashLockDown;

  //
  // Audio DSP
  //
  DxePlatformPchPolicy->DeviceEnabling->AudioDsp                  = SetupVariables->PchAudioDsp;
  DxePlatformPchPolicy->AudioDspConfig->AudioDspD3PowerGating     = SetupVariables->PchAudioDspD3PowerGating;
  DxePlatformPchPolicy->AudioDspConfig->AudioDspBluetoothSupport  = SetupVariables->PchAudioDspBluetooth;
  DxePlatformPchPolicy->AudioDspConfig->AudioDspAcpiMode          = SetupVariables->PchAudioDspAcpiMode;
  DxePlatformPchPolicy->AudioDspConfig->AudioDspAcpiInterruptMode = SetupVariables->PchAudioDspAcpiInterruptMode;

  //
  // Serial IO Config
  //
  // NOTE: Keep it disable since doesn;t apply to Wellsburg 
  //
  DxePlatformPchPolicy->DeviceEnabling->SerialIoDma        = PCH_DEVICE_DISABLE;
  DxePlatformPchPolicy->DeviceEnabling->SerialIoI2c0       = PCH_DEVICE_DISABLE;
  DxePlatformPchPolicy->DeviceEnabling->SerialIoI2c1       = PCH_DEVICE_DISABLE;
  DxePlatformPchPolicy->DeviceEnabling->SerialIoSpi0       = PCH_DEVICE_DISABLE;
  DxePlatformPchPolicy->DeviceEnabling->SerialIoSpi1       = PCH_DEVICE_DISABLE;
  DxePlatformPchPolicy->DeviceEnabling->SerialIoUart0      = PCH_DEVICE_DISABLE;
  DxePlatformPchPolicy->DeviceEnabling->SerialIoUart1      = PCH_DEVICE_DISABLE;
  DxePlatformPchPolicy->DeviceEnabling->SerialIoSdio       = PCH_DEVICE_DISABLE;
  DxePlatformPchPolicy->SerialIoConfig->SerialIoMode       = PCH_DEVICE_DISABLE;
  DxePlatformPchPolicy->SerialIoConfig->I2c0VoltageSelect  = PCH_DEVICE_DISABLE;
  DxePlatformPchPolicy->SerialIoConfig->I2c1VoltageSelect  = PCH_DEVICE_DISABLE;

  //
  // GpioInt Configuration
  //
  DxePlatformPchPolicy->SerialIoConfig->GpioIrqRoute          = PCH_DEVICE_DISABLE;
  DxePlatformPchPolicy->SerialIoConfig->DriverModeTouchPanel  = PCH_DEVICE_DISABLE;
  DxePlatformPchPolicy->SerialIoConfig->DriverModeTouchPad    = PCH_DEVICE_DISABLE;
  DxePlatformPchPolicy->SerialIoConfig->DriverModeSensorHub   = PCH_DEVICE_DISABLE;

  //
  // Usb devices
  //
  DxePlatformPchPolicy->UsbConfig->Ehci1Usbr = PCH_DEVICE_DISABLE;
  DxePlatformPchPolicy->UsbConfig->Ehci2Usbr = PCH_DEVICE_DISABLE;
    
#if defined(AMT_SUPPORT) && AMT_SUPPORT
#if SPS_SUPPORT
  if (MeTypeIsAmt())
#endif // SPS_SUPPORT
  {
    VariableSize = sizeof (ME_BIOS_EXTENSION_SETUP);
    Status = gST->RuntimeServices->GetVariable (
                                gEfiMeBiosExtensionSetupName,
                                &gEfiMeBiosExtensionSetupGuid,
                                NULL,
                                &VariableSize,
                                &MeBiosExtensionSetupData
                                );
    if (!EFI_ERROR (Status) && MeBiosExtensionSetupData.InterfaceVersion != 0) {
      if (MeBiosExtensionSetupData.KvmEnable & KVM_ENABLE) {
        DxePlatformPchPolicy->UsbConfig->Ehci1Usbr = PCH_DEVICE_ENABLE;
        DxePlatformPchPolicy->UsbConfig->Ehci2Usbr = PCH_DEVICE_ENABLE;
      }
    }
  }
#endif // AMT_SUPPORT


  UpdatePchUsbConfig(
     DxePlatformPchPolicy->UsbConfig,
     SetupVariables, 
     mPlatformType->BoardId,
     PlatformInfoHobData
    );

  //
  // Initiate DMI Configuration
  //
  DxePlatformPchPolicy->DmiConfig->DmiAspm              = SetupVariables->PcieDmiAspm;
  DxePlatformPchPolicy->DmiConfig->DmiExtSync           = SetupVariables->PcieDmiExtSync;
  DxePlatformPchPolicy->DmiConfig->DmiIot               = PCH_DEVICE_DISABLE; //Must be a new setup option on IIO config for this one 
    DxePlatformPchPolicy->DmiConfig->DmiStopAndScreamEnable = SetupVariables->PcieDmiStopAndScreamEnable;

  //
  // PCI express config
  //
  DxePlatformPchPolicy->PciExpressConfig->RootPortClockGating   = SetupVariables->PcieClockGating;  
  DxePlatformPchPolicy->PciExpressConfig->TempRootPortBusNumMin = 2;
  DxePlatformPchPolicy->PciExpressConfig->TempRootPortBusNumMax = 2;
  // Enable ASPM L1 for all PCH Pcie Root Ports 
  DxePlatformPchPolicy->PciExpressConfig->PchPcieGlobalAspm    = SetupVariables->PchPcieGlobalAspm;

  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].Enable                         = SetupVariables->PcieRootPortEn[Index];
    // Aptio V Server Override Start: Program PCH physical slot number based on SDL porting.     
    //DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].SlotImplemented                = SetupVariables->PcieRootPortEn[Index];
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].SlotImplemented                = PCHPortToSlotMap[Index];
    // Aptio V Server Override End: Program PCH physical slot number based on SDL porting. 
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].FunctionNumber                 = (UINT8) Index;
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].PhysicalSlotNumber             = (UINT8) Index;
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].Aspm                           = SetupVariables->PcieRootPortAspm[Index];
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].L1Substates                    = SetupVariables->PcieRootPortL1SubStates[Index];
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].PmSci                          = SetupVariables->PcieRootPortPMCE[Index];
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].HotPlug                        = SetupVariables->PcieRootPortHPE[Index];

    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].AdvancedErrorReporting         = PCH_DEVICE_DISABLE;
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].UnsupportedRequestReport       = SetupVariables->PcieRootPortURE[Index];
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].FatalErrorReport               = SetupVariables->PcieRootPortFEE[Index];
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].NoFatalErrorReport             = SetupVariables->PcieRootPortNFE[Index];
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].CorrectableErrorReport         = SetupVariables->PcieRootPortCEE[Index];
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].PmeInterrupt                   = 0;
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].SystemErrorOnFatalError        = SetupVariables->PcieRootPortSFE[Index];
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].SystemErrorOnNonFatalError     = SetupVariables->PcieRootPortSNE[Index];
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].SystemErrorOnCorrectableError  = SetupVariables->PcieRootPortSCE[Index];
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].TransmitterHalfSwing           = SetupVariables->PcieRootPortTHS[Index];
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].CompletionTimeout              = PchPcieCompletionTO_Default;
    DxePlatformPchPolicy->PciExpressConfig->RootPort[Index].MsiEnable                      = SetupVariables->PcieRootPortMSIE[Index];
  }

  if (SetupVariables->PcieSBDE) {
    DxePlatformPchPolicy->PciExpressConfig->EnableSubDecode = SetupVariables->PcieRootPortSBDE;
    DxePlatformPchPolicy->PciExpressConfig->PchPcieSbdePort = SetupVariables->PcieSBDEPort;
  } else {
    DxePlatformPchPolicy->PciExpressConfig->EnableSubDecode = PCH_DEVICE_DISABLE;
  }
  DxePlatformPchPolicy->PciExpressConfig->RootPortFunctionSwapping          = SetupVariables->PcieRootPortFunctionSwapping;

  //
  // For Wellsburg (or LPT-H on uServer) get VTdSupport to decide if BIT25 of MSC(D8h) needs to be clear or not
  //
  DxePlatformPchPolicy->PciExpressConfig->VTdEnable       = SetupVariables->VTdSupport;
  //
  // SATA Config
  //
  for (Index = 0; Index < GetPchMaxSataPortNum (); Index++) {
    if (SetupVariables->SataInterfaceMode == SATA_MODE_IDE) {
    DxePlatformPchPolicy->SataConfig->PortSettings[Index].Enable      = PCH_DEVICE_ENABLE;
    } else {
      DxePlatformPchPolicy->SataConfig->PortSettings[Index].Enable = SetupVariables->SataPort[Index];
    }

    DxePlatformPchPolicy->SataConfig->PortSettings[Index].HotPlug     = SetupVariables->SataHotPlug[Index];

    DxePlatformPchPolicy->SataConfig->PortSettings[Index].SpinUp      = SetupVariables->SataSpinUp[Index];

    DxePlatformPchPolicy->SataConfig->PortSettings[Index].External    = SetupVariables->SataExternal[Index];
    DxePlatformPchPolicy->SataConfig->PortSettings[Index].DevSlp      = SetupVariables->PxDevSlp[Index];
    DxePlatformPchPolicy->SataConfig->PortSettings[Index].EnableDitoConfig = SetupVariables->EnableDitoConfig;

    DxePlatformPchPolicy->SataConfig->PortSettings[Index].DmVal            = SetupVariables->DmVal;
    DxePlatformPchPolicy->SataConfig->PortSettings[Index].DitoVal          = SetupVariables->DitoVal;
    DxePlatformPchPolicy->SataConfig->PortSettings[Index].SolidStateDrive  = SetupVariables->SataType[Index];
  }

  DxePlatformPchPolicy->SataConfig->RaidAlternateId = SetupVariables->SataAlternateId;
  DxePlatformPchPolicy->SataConfig->Raid0           = SetupVariables->SataRaidR0;
  DxePlatformPchPolicy->SataConfig->Raid1           = SetupVariables->SataRaidR1;
  DxePlatformPchPolicy->SataConfig->Raid10          = SetupVariables->SataRaidR10;
  DxePlatformPchPolicy->SataConfig->Raid5           = SetupVariables->SataRaidR5;
  DxePlatformPchPolicy->SataConfig->Irrt            = SetupVariables->SataRaidIrrt;
  DxePlatformPchPolicy->SataConfig->OromUiBanner    = SetupVariables->SataRaidOub;
  DxePlatformPchPolicy->SataConfig->HddUnlock       = SetupVariables->SataHddlk;
  DxePlatformPchPolicy->SataConfig->LedLocate       = SetupVariables->SataLedl;
  DxePlatformPchPolicy->SataConfig->IrrtOnly        = SetupVariables->SataRaidIooe;
  DxePlatformPchPolicy->SataConfig->SmartStorage    = SetupVariables->SataRaidSrt;
  DxePlatformPchPolicy->SataConfig->OromUiDelay     = SetupVariables->SataRaidOromDelay;
  DxePlatformPchPolicy->SataConfig->LpmSupport      = SetupVariables->SataLpm;
  DxePlatformPchPolicy->SataConfig->SalpSupport     = SetupVariables->SataSalp;
  DxePlatformPchPolicy->SataConfig->TestMode        = SetupVariables->SataTestMode;
  DxePlatformPchPolicy->SataConfig->LegacyMode      = PCH_DEVICE_DISABLE;
  DxePlatformPchPolicy->SataConfig->SpeedSupport    = PchSataSpeedSupportGen3;
  DxePlatformPchPolicy->SataConfig->SataEfiRaid     = SetupVariables->SataRaidLoadEfiDriver;

#ifdef PCH_SERVER_BIOS_FLAG
  //
  // sSATA Config
  //
  for (Index = 0; Index < GetPchMaxsSataPortNum (); Index++) {

    if ((SetupVariables->sSataInterfaceMode) == SATA_MODE_IDE) {
      DxePlatformPchPolicy->sSataConfig->PortSettings[Index].Enable    = PCH_DEVICE_ENABLE;
    } else {
      DxePlatformPchPolicy->sSataConfig->PortSettings[Index].Enable = SetupVariables->sSataPort[Index];
    }  
    
    DxePlatformPchPolicy->sSataConfig->PortSettings[Index].HotPlug     = SetupVariables->sSataHotPlug[Index];
    DxePlatformPchPolicy->sSataConfig->PortSettings[Index].SpinUp      = SetupVariables->sSataSpinUp[Index];	    
    DxePlatformPchPolicy->sSataConfig->PortSettings[Index].External    = SetupVariables->sSataExternal[Index];
    DxePlatformPchPolicy->sSataConfig->PortSettings[Index].DevSlp      = SetupVariables->sPxDevSlp[Index];

    DxePlatformPchPolicy->sSataConfig->PortSettings[Index].DevSlp      = SetupVariables->sPxDevSlp[Index];
    DxePlatformPchPolicy->sSataConfig->PortSettings[Index].EnableDitoConfig = SetupVariables->sEnableDitoConfig;

    DxePlatformPchPolicy->sSataConfig->PortSettings[Index].DmVal            = SetupVariables->sDmVal;
    DxePlatformPchPolicy->sSataConfig->PortSettings[Index].DitoVal          = SetupVariables->sDitoVal;
    DxePlatformPchPolicy->sSataConfig->PortSettings[Index].SolidStateDrive = SetupVariables->sSataType[Index];
  }

  DxePlatformPchPolicy->sSataConfig->RaidAlternateId = SetupVariables->sSataAlternateId;
  if ((SetupVariables->PchSata == TRUE) &&
      (SetupVariables->PchsSata == TRUE) &&
      (SetupVariables->SataInterfaceMode == SATA_MODE_RAID) &&
      (SetupVariables->sSataInterfaceMode == SATA_MODE_RAID) &&
      (SetupVariables->SataAlternateId == TRUE) &&
      (SetupVariables->sSataAlternateId == TRUE) &&
      (SetupVariables->SataRaidLoadEfiDriver == SetupVariables->sSataRaidLoadEfiDriver)) {
    DxePlatformPchPolicy->sSataConfig->HddUnlock       = SetupVariables->SataHddlk;
    DxePlatformPchPolicy->sSataConfig->LedLocate       = SetupVariables->SataLedl;
    DxePlatformPchPolicy->sSataConfig->Raid0           = SetupVariables->SataRaidR0;
    DxePlatformPchPolicy->sSataConfig->Raid1           = SetupVariables->SataRaidR1;
    DxePlatformPchPolicy->sSataConfig->Raid10          = SetupVariables->SataRaidR10;
    DxePlatformPchPolicy->sSataConfig->Raid5           = SetupVariables->SataRaidR5;
    DxePlatformPchPolicy->sSataConfig->Irrt            = SetupVariables->SataRaidIrrt;
    DxePlatformPchPolicy->sSataConfig->OromUiBanner    = SetupVariables->SataRaidOub;
    DxePlatformPchPolicy->sSataConfig->IrrtOnly        = SetupVariables->SataRaidIooe;
    DxePlatformPchPolicy->sSataConfig->SmartStorage    = SetupVariables->SataRaidSrt;
    DxePlatformPchPolicy->sSataConfig->OromUiDelay     = SetupVariables->SataRaidOromDelay;
      
  } else {
    DxePlatformPchPolicy->sSataConfig->HddUnlock       = SetupVariables->sSataHddlk;
    DxePlatformPchPolicy->sSataConfig->LedLocate       = SetupVariables->sSataLedl;
    DxePlatformPchPolicy->sSataConfig->Raid0           = SetupVariables->sSataRaidR0;
    DxePlatformPchPolicy->sSataConfig->Raid1           = SetupVariables->sSataRaidR1;
    DxePlatformPchPolicy->sSataConfig->Raid10          = SetupVariables->sSataRaidR10;
    DxePlatformPchPolicy->sSataConfig->Raid5           = SetupVariables->sSataRaidR5;
    DxePlatformPchPolicy->sSataConfig->Irrt            = SetupVariables->sSataRaidIrrt;
    DxePlatformPchPolicy->sSataConfig->OromUiBanner    = SetupVariables->sSataRaidOub;
    DxePlatformPchPolicy->sSataConfig->IrrtOnly        = SetupVariables->sSataRaidIooe;
    DxePlatformPchPolicy->sSataConfig->SmartStorage    = SetupVariables->sSataRaidSrt;
    DxePlatformPchPolicy->sSataConfig->OromUiDelay     = SetupVariables->sSataRaidOromDelay;
  }
  DxePlatformPchPolicy->sSataConfig->LpmSupport      = SetupVariables->sSataLpm;
  DxePlatformPchPolicy->sSataConfig->SalpSupport     = SetupVariables->sSataSalp;
  DxePlatformPchPolicy->sSataConfig->TestMode        = SetupVariables->sSataTestMode;
  DxePlatformPchPolicy->sSataConfig->LegacyMode      = PCH_DEVICE_DISABLE;    
  DxePlatformPchPolicy->sSataConfig->SpeedSupport    = PchSataSpeedSupportGen3;
  DxePlatformPchPolicy->sSataConfig->SataEfiRaid     = SetupVariables->sSataRaidLoadEfiDriver;

//  AptioV override because we can't figure out how it builds
//  if ( mPlatformType->BoardId == TypeHedtSKU1CRB ||
//       mPlatformType->BoardId == TypeHedtEv ||
//       mPlatformType->BoardId == TypeHedtDDR3 ) {
	   
	//
    // Set raid controller device id to Rapid Storage Technology (RST) for HEDT
    //
//    DxePlatformPchPolicy->RaidDevice->DeviceId = PchRaidRST;
//  } else {
	//
    // Set raid controller device id to Rapid Storage Technology Enterprice (RSTe)
    //
    DxePlatformPchPolicy->RaidDevice->DeviceId = PchRaidRSTe;
//  }
#endif //PCH_SERVER_BIOS_FLAG
  //
  // AzaliaConfig
  //
  DxePlatformPchPolicy->AzaliaConfig->Pme = SetupVariables->AzaliaPme;
  DxePlatformPchPolicy->AzaliaConfig->DS  = SetupVariables->AzaliaDs;
  DxePlatformPchPolicy->AzaliaConfig->DA  = PCH_DEVICE_DISABLE; 
    
  //
  // If BootMode is Manufacturing Mode, disable Azalia and NVSRAM
  //    
  if (mBootMode == BOOT_WITH_MFG_MODE_SETTINGS) {
    DxePlatformPchPolicy->DeviceEnabling->Azalia = PCH_DEVICE_DISABLE;
  }

  if (DxePlatformPchPolicy->DeviceEnabling->Azalia != PCH_DEVICE_DISABLE) {
    SetupAzalia(DxePlatformPchPolicy);
  } 
  
  //
  // Reserved SMBus Address
  //  
  DxePlatformPchPolicy->SmbusConfig->NumRsvdSmbusAddresses = PLATFORM_NUM_SMBUS_RSVD_ADDRESSES;
  DxePlatformPchPolicy->SmbusConfig->RsvdSmbusAddressTable = mSmbusRsvdAddresses;
     
  //
  // Misc PM Config
  //
  DxePlatformPchPolicy->MiscPmConfig->PchDeepSxPol                         = SetupVariables->DeepSxMode;
  DxePlatformPchPolicy->MiscPmConfig->WakeConfig.WolEnableOverride         = SetupVariables->PchWakeOnLan;
  DxePlatformPchPolicy->MiscPmConfig->WakeConfig.Gp27WakeFromDeepSx        = SetupVariables->Gp27WakeFromDeepSx;
  DxePlatformPchPolicy->MiscPmConfig->SlpLanLowDc                          = SetupVariables->PchSlpLanLowDc;
  DxePlatformPchPolicy->MiscPmConfig->PowerResetStatusClear.MeWakeSts      = PCH_DEVICE_ENABLE;
  DxePlatformPchPolicy->MiscPmConfig->PowerResetStatusClear.MeHrstColdSts  = PCH_DEVICE_ENABLE;
  DxePlatformPchPolicy->MiscPmConfig->PowerResetStatusClear.MeHrstWarmSts  = PCH_DEVICE_ENABLE;



  //
  // IOAPIC Config
  //
  DxePlatformPchPolicy->IoApicConfig->BdfValid           = 1;
  DxePlatformPchPolicy->IoApicConfig->BusNumber          = PCI_BUS_NUMBER_PCH_IOAPIC;
  DxePlatformPchPolicy->IoApicConfig->DeviceNumber       = PCI_DEVICE_NUMBER_PCH_IOAPIC;
  DxePlatformPchPolicy->IoApicConfig->FunctionNumber     = PCI_FUNCTION_NUMBER_PCH_IOAPIC;
  DxePlatformPchPolicy->IoApicConfig->IoApicEntry24_39   = PCH_DEVICE_ENABLE;
  //
  // Hpet device ID config
  // The default values are B0:D31:F0 which is where the HPET is
  // But change the default to B240:D15:F0
  // This is an unused BDF that matches with the HPET entry in DMAR
  //
  DxePlatformPchPolicy->HpetConfig->BdfValid  = TRUE;
  for (Index = 0; Index < PCH_HPET_BDF_MAX; Index++) {
    DxePlatformPchPolicy->HpetConfig->Hpet[Index].BusNumber = 240;
    DxePlatformPchPolicy->HpetConfig->Hpet[Index].DeviceNumber = 15;
    DxePlatformPchPolicy->HpetConfig->Hpet[Index].FunctionNumber = 0;
  }
  
  //
  // Power Optimizer configuration
  //
  DxePlatformPchPolicy->PwrOptConfig->LegacyDmaDisable =  PCH_DEVICE_DISABLE;
  //
  // DefaultSvidSid Config
  //
  DxePlatformPchPolicy->DefaultSvidSid->SubSystemVendorId = V_PCH_INTEL_VENDOR_ID;
  DxePlatformPchPolicy->DefaultSvidSid->SubSystemId       = V_PCH_DEFAULT_SID;

  //
  // LockDown Config
  //
  DxePlatformPchPolicy->LockDownConfig->GlobalSmi           = PCH_DEVICE_ENABLE;//PCH_DEVICE_ENABLE;

    
  //
  // Thermal Config
  //
  DxePlatformPchPolicy->ThermalConfig->ThermalAlertEnable.TselLock  = PCH_DEVICE_ENABLE;
  DxePlatformPchPolicy->ThermalConfig->ThermalAlertEnable.TscLock   = PCH_DEVICE_ENABLE;
  DxePlatformPchPolicy->ThermalConfig->ThermalAlertEnable.TsmicLock = PCH_DEVICE_ENABLE;
  DxePlatformPchPolicy->ThermalConfig->ThermalAlertEnable.PhlcLock  = PCH_DEVICE_ENABLE;
  //
  // Check SMLink Thermal Reporting Select (PCH Strap 15 [14]) to know if using ME based thermal reporting (false)
  //
  if ((PchMmRcrb16 (R_PCH_SPI_HSFS) & B_PCH_SPI_HSFS_FDV) == B_PCH_SPI_HSFS_FDV) {
    PchMmRcrb32And (R_PCH_SPI_FDOC, (UINT32) (~(B_PCH_SPI_FDOC_FDSS_MASK | B_PCH_SPI_FDOC_FDSI_MASK)));
    PchMmRcrb32Or  (R_PCH_SPI_FDOC, (UINT32) (V_PCH_SPI_FDOC_FDSS_PCHS | R_PCH_SPI_STRP15));
    if ((PchMmRcrb32 (R_PCH_SPI_FDOD) & (UINT32) R_PCH_SPI_STRP15_SML1_THRMSEL) == 0) {
      //
      // While Thermal Reporting is enabled and DPTF is disabled, set "TscLock", "TspcLock" and "TselLock" to "Enabled".
      //
      DxePlatformPchPolicy->ThermalConfig->ThermalAlertEnable.TscLock = PCH_DEVICE_ENABLE;
    }
  }
  DxePlatformPchPolicy->ThermalConfig->ThermalThrottling.TTLevels.PchCrossThrottling = SetupVariables->PchCrossThrottling;
  DxePlatformPchPolicy->ThermalConfig->ThermalThrottling.DmiHaAWC.SuggestedSetting  = PCH_DEVICE_ENABLE;
  DxePlatformPchPolicy->ThermalConfig->ThermalThrottling.SataTT.SuggestedSetting    = PCH_DEVICE_ENABLE;
#ifdef PCH_SERVER_BIOS_FLAG
  DxePlatformPchPolicy->ThermalConfig->ThermalThrottling.sSataTT.SuggestedSetting    = PCH_DEVICE_ENABLE;
#endif // PCH_SERVER_BIOS_FLAG

  // PCH thermal device D31:F6 enable.
  //
  // If AUTO = PCH thermal enable
  //
  if (2 == SetupVariables->ThermalDeviceEnable) {

#ifndef GRANGEVILLE_FLAG
    // AUTO for AZTECSTHI = Enable
    //
    if ( (TypeHedtEv == mPlatformType->BoardId) || (TypeAztecCitySTHI == mPlatformType->BoardId) ) {
      DxePlatformPchPolicy->ThermalConfig->ThermalDeviceEnable = PCH_DEVICE_ENABLE;
    } else { 

      // Other boards = Disable
      //
      DxePlatformPchPolicy->ThermalConfig->ThermalDeviceEnable = PCH_DEVICE_DISABLE;
    }
#else
    DxePlatformPchPolicy->ThermalConfig->ThermalDeviceEnable = PCH_DEVICE_DISABLE;
#endif

  // Not AUTO, just follow setup choice
  //
  } else {
  DxePlatformPchPolicy->ThermalConfig->ThermalDeviceEnable = SetupVariables->ThermalDeviceEnable;
  }

#ifdef DE_SKU
  //
  // Initialize PMSYNC Flow knob in BDX-DE MCP
  //
  DxePlatformPchPolicy->ThermalConfig->EnablePmSyncFlow = SetupVariables->CpuToPchThrottle;

  // 
  // For DE_SKU enable PMSYNC Flow only if CpuToPchThrottle is enabled 
  //
  if (SetupVariables->CpuToPchThrottle){
   DxePlatformPchPolicy->ThermalConfig->ThermalThrottling.TTLevels.TTState13Enable = PCH_DEVICE_ENABLE;
  }

#endif //DE_SKU

  //
  // Initialize Serial IRQ Config
  //
  DxePlatformPchPolicy->SerialIrqConfig->SirqEnable       = TRUE;
  DxePlatformPchPolicy->SerialIrqConfig->StartFramePulse  = PchSfpw4Clk;
  DxePlatformPchPolicy->SerialIrqConfig->SirqMode         = PchContinuousMode;

  //
  // Set Serial IRQ Mode Select according to setup value.
  //
  if (SetupVariables->PchSirqMode == 0) {
    DxePlatformPchPolicy->SerialIrqConfig->SirqMode = PchQuietMode;
  }
  


  return EFI_SUCCESS;

}

EFI_STATUS
SetupAzalia (
   IN     DXE_PCH_PLATFORM_POLICY_PROTOCOL   *DxePlatformPchPolicy
   )
 {
   if (!DxePlatformPchPolicy){
    return EFI_INVALID_PARAMETER;
   }
   
   mAzaliaVerbTable[0].VerbTableData   = mAzaliaVerbTableData1;
   mAzaliaVerbTable[1].VerbTableData   = mAzaliaVerbTableData2;
   mAzaliaVerbTable[2].VerbTableData   = mAzaliaVerbTableData3;
   mAzaliaVerbTable[3].VerbTableData   = mAzaliaVerbTableData4;
   mAzaliaVerbTable[4].VerbTableData   = mAzaliaVerbTableData5;
   mAzaliaVerbTable[5].VerbTableData   = mAzaliaVerbTableData6;
   mAzaliaVerbTable[6].VerbTableData   = mAzaliaVerbTableData7;
   mAzaliaVerbTable[7].VerbTableData   = mAzaliaVerbTableData8;
   mAzaliaVerbTable[8].VerbTableData   = mAzaliaVerbTableData9;
   mAzaliaVerbTable[9].VerbTableData   = mAzaliaVerbTableData10;
   mAzaliaVerbTable[10].VerbTableData  = mAzaliaVerbTableData11;
   
   DxePlatformPchPolicy->AzaliaConfig->AzaliaVerbTableNum = sizeof (mAzaliaVerbTable) / sizeof (PCH_AZALIA_VERB_TABLE);
   DxePlatformPchPolicy->AzaliaConfig->AzaliaVerbTable    = mAzaliaVerbTable;
   DxePlatformPchPolicy->AzaliaConfig->ResetWaitTimer     = 300;

   return EFI_SUCCESS;
 }


/**
  PCH checkes the HECI protocol and sends ChipsetInitSyncMsg

  @param[in] Event                      Event objext
  @param[in] *Context                   VOID Pointer

  @retval None
**/
VOID
EFIAPI
ChipsetInitSyncCallback (
  IN  EFI_EVENT                         Event,
  IN  VOID                              *Context
  )
{
  EFI_STATUS                            Status;
  EFI_HECI_PROTOCOL                     *Heci;
  CHIPSET_INIT_INFO_HOB                 *ChipsetInitHob;

  ChipsetInitHob = NULL;
  Status           = EFI_SUCCESS;
  DEBUG ((EFI_D_INFO, "ChipsetInitSyncCallback() Start\n"));

  //
  // Get the HECI protocol to make sure HECI is ready.
  //
  Status = gBS->LocateProtocol (&gEfiHeciProtocolGuid, NULL, (VOID **) &Heci);
  if (EFI_ERROR (Status)) {
    return;
  }

  if (mHeciEvent) {
    gBS->CloseEvent (mHeciEvent);
  }

  //
  // Search for the ChipsetInit Info PEIM GUID HOB.
  //
  ChipsetInitHob = GetFirstGuidHob (&gChipsetInitInfoHobGuid);
  if (ChipsetInitHob == NULL) {
    DEBUG ((EFI_D_INFO, "ChipsetInitHob not found.\n"));
    return;
  }

  //
  // If ChipsetInitTableUpdReq == 0, settings are already in sync and no furhter work needed
  //
  if (ChipsetInitHob->ChipsetInitTableUpdReq == 1) {
    //
    // If we do not have the ChipsetInit table that ME FW expects us to have,
    // we must send the current ChipsetInit table to ME FW via HECI message.
    //
    Status = HeciChipsetInitSyncMsg(ChipsetInitHob->ChipsetInitTable, ChipsetInitHob->ChipsetInitTableLen);
  }

  DEBUG ((EFI_D_INFO, "ChipsetInitSyncCallback() End\n"));

  return;
}

EFI_STATUS
EFIAPI
RegisterPchInitSyncCallback (
  IN     DXE_PCH_PLATFORM_POLICY_PROTOCOL   *This
  )
{
  VOID                                  *HeciRegistration;
  EFI_STATUS                            Status;   
  
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  ChipsetInitSyncCallback,
                  NULL,
                  &mHeciEvent
                  );
  ASSERT_EFI_ERROR(Status);

  Status = gBS->RegisterProtocolNotify (
                  &gEfiHeciProtocolGuid,
                  mHeciEvent,
                  &HeciRegistration
                  );
  ASSERT_EFI_ERROR(Status);

  ChipsetInitSyncCallback (NULL, NULL);

  return EFI_SUCCESS;
}
