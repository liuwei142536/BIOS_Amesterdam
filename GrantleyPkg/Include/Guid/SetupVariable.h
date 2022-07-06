//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c)  1999 - 2021 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

    SetupVariable.h

Abstract:

    Driver configuration include file

Revision History

--*/

#ifndef __SETUP_VARIABLE_H__
#define __SETUP_VARIABLE_H__
#include <Token.h>  //AptioV server override
#include <UncoreCommonIncludes.h>
// ---------------------------------------------------------------------------
//
// Driver Configuration
//
// ---------------------------------------------------------------------------
//
#define EFI_EPG_GENERIC_VARIABLE_GUID \
  { \
    0x8302cc54, 0xbb1a, 0x4564, 0x92, 0xdc, 0xae, 0x1a, 0xbb, 0x15, 0x5f, 0x35 \
  }

//
// {EC87D643-EBA4-4bb5-A1E5-3F3E36B20DA9}
//
#define SYSTEM_CONFIGURATION_GUID \
  { \
    0xec87d643, 0xeba4, 0x4bb5, 0xa1, 0xe5, 0x3f, 0x3e, 0x36, 0xb2, 0xd, 0xa9 \
  }

#define SYSTEM_CONFIGURATION_CONTROL_GUID \
  { \
    0x8d247131, 0x385e, 0x491f, 0xba, 0x68, 0x8d, 0xe9, 0x55, 0x30, 0xb3, 0xa6 \
  }

#define SYSTEM_CONFIGURATION_ICHPCIE_GUID \
  { \
    0x10e023a7, 0x4ce5, 0x4a6a, 0xa1, 0xbb, 0xbd, 0xd0, 0x8d, 0x16, 0x37, 0x57 \
  }

#define SYSTEM_DEFAULT_CONFIGURATION_GUID \
  { \
    0x99a96812, 0x4730, 0x4290, 0x8b, 0xfe, 0x7b, 0x4e, 0x51, 0x4f, 0xf9, 0x3b \
  }

  #define SETUP_VARIABLE_ATTRIBUTES	   (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS)



#define PCH_SATA_MAX_PORTS    6

/// sSATA max ports for Wellsburg
#define PCH_SSATA_MAX_PORTS    6

#define PCH_PCIE_MAX_ROOT_PORTS         8

#define MAX_PCH_PCI_EXPRESS_ROOT_PORTS  8
// #define PASSWORD_MAX_SIZE              16
#define EFI_VARIABLE_BOOTSERVICE_ACCESS 0x00000002
//
// Resource Ratio units used by Uncore Init PEIM.
// bugbug: these should be defined and exposed by the Uncore Init PEIM.
//
// Assumption: these values must be 2^N; Otherwise the algorithm in OemMemoryQpiInit.c
// needs to be adjusted: the "if (Alignment > QPI_SOCKET_BUS_RATIO_UNIT) {" should be
// removed when computing adjusted "Length".
//
#define QPI_SOCKET_BUS_RATIO_UNIT              0x20
#define QPI_SOCKET_IO_RATIO_UNIT               0x2000
#define QPI_SOCKET_MMIOL_RATIO_UNIT            0x4000000

//
// Maximum alignment bit allowed for Socket PCI Resources.
// bugbug: these should be defined and exposed by the Uncore Init PEIM
//
#define QPI_SOCKET_MAX_BUS_ALIGNMENT           0x8
#define QPI_SOCKET_MAX_IO_ALIGNMENT            0x10
#define QPI_SOCKET_MAX_MMIOL_ALIGNMENT         0x20

//
// Defines used for variables to be range checked before consumption.
//
#define MAX_CAS_LATENCY                        32
#define MAX_TRP_LATENCY                        32
#define MAX_TRCD_LATENCY                       32
#define MAX_TRRD_LATENCY                       255
#define MAX_TWTR_LATENCY                       255
#define MAX_TRAS_LATENCY                       63
#define MAX_TRTP_LATENCY                       255
#define MAX_TWR_LATENCY                        50
#define MAX_TFAW_LATENCY                       63
#define MAX_TCWL_LATENCY                       31
#define MAX_TRC_LATENCY                        255
#define MAX_REFRESH_RATE                       32767
#define MAX_TRFC_LATENCY                       1023
#define MAX_MC_BGF_THRESHOLD                   15


#pragma pack(1)

typedef struct {

  //
  // Passwords
  // Note: UserPassword and AdminPassword need to remain on a WORD boundary, so list them first so other
  // items don't affect their alignment.  String libraries that reference the password require 16-bit
  // alignment.  Compiler uses at least WORD alignment.
  //
  UINT16  UserPassword[PASSWORD_MAX_SIZE];
  UINT16  AdminPassword[PASSWORD_MAX_SIZE];
  UINT8   Access;

  //
  // System ports
  //
  UINT8   Serial;
  UINT8   Parallel;
  UINT8   Audio;
  UINT8   Lan;

  //
  // Keyboard
  //
  UINT8   Numlock;
  UINT8   Ps2PortSwap;

  //
  // TPM
  //
  UINT8         TpmEnable;
  UINT8         TpmState;
  UINT8         MorState;

  //
  // Breakpoints
  //
  UINT8   ValidationBreakpointType;

  //
  // Power State
  //
  UINT8   PowerState;
  UINT8   AcpiSleepState;

  //
  // Wake On Lan
  //
  UINT8   WakeOnLanS5;

  //
  // Boot from Network
  //
  UINT8   BootNetwork;

  //
  // Video
  //
  UINT8   VideoSelect;
  // AptioV Server Override: Added for Vga Priority Porting
  UINT8   TargetVga;
  UINT8   EfiWindowsInt10Workaround;
  //UINT8   UefiOptimizedBoot; AptioV server override: Remove unwanted option

  //
  // TPM support
  //
  UINT8   TpmSupport;
  UINT8   TpmCommand;

  //
  // PCI 64-Bit Resource Allocation support
  //
  UINT8   Pci64BitResourceAllocation;

  //
  // PCI 64-Bit Resource Allocation support
  //
  UINT8   PcieMaxReadRequestSize;
  UINT8   PCIe_LTR;
  UINT8   PcieExtendedTagField;

  //
  // Fan PWM Offset
  //
  UINT8    FanPwmOffset;

  //
  // PCI Minimum Secondary Bus Number
  //
  UINT8   PciMinSecondaryBus;
  UINT8   PciRootPortBusPadding;

  UINT8   PCIe_AtomicOpReq;
  UINT8   SataRaidHotPlug;

  UINT8   IdeMode; // IDE Mode             0 Legacy 1 Native
  //
  // Stagger Spinnup Support
  //
  UINT8   StaggerSpinnup;

  //
  // Processor (CPU)
  //
  UINT8   CpuidMaxValue;
  UINT8   ExecuteDisableBit;

  UINT8   ProcessorEistEnable;              // EIST or GV3 setup option
  UINT8   PStateDomain;                     // P State Domain
  UINT8   ProcessorEistPsdFunc;             // EIST/PSD Function select option
  UINT8   ProcessorSPD;                     // PCPS - SPD select option
  UINT8   Pl2SafetyNetEnable;               // PL2_SAFETY_NET_ENABLE
//  Aptio V Server Override - Use ProcessorEistPsdFunc to determine hardware/software coordination.
  UINT8   ProcessorSinglePCTLEn;            // PCPS - SINGLE_PCTL select option
  UINT8   BootPState;                       // Boot Performance Mode
  UINT8   ProcessorCcxEnable;               // Enabling CPU C states of processor
  UINT8   C1AutoDemotion;                   // C1 Auto Demotion
  UINT8   C3AutoDemotion;                   // C3 Auto Demotion
  UINT8   PackageCState;                    // Package C-State Limit
  UINT8   C3Enable;                         // Enable/Disable NHM C3(ACPI C2) report to OS
  UINT8   C6Enable;                         // Enable/Disable NHM C6(ACPI C3) report to OS
  UINT8   C7Enable;                         // Enable/Disable NHM C7(ACPI C3) report to OS
  UINT8   ProcessorC1eEnable;               // Enabling C1E state of processor
  UINT8   ProcessorHyperThreadingDisable;   // Hyper Threading [ALL]
  UINT8   ProcessorLtsxEnable;              // Enabling TXT
  UINT8   ProcessorVmxEnable;               // Enabling VMX
  UINT8   ProcessorSmxEnable;               // Enabling SMX
  UINT8   ProcessorMsrLockControl;          // MSR Lock Bit Control
  UINT8   DebugInterface;                   // IA32_DEBUG_INTERFACE_MSR
  UINT8   ThreeStrikeTimer;                 // Disable 3strike timer
  UINT8   ProcessorBistEnable;              // Enabling BIST
  UINT8   ProcessorxTPRDisable;             // Disabling XTPR
  UINT8   ProcessorPeciEnable;              // Enabling PECI
  UINT8   ProcessorXEEnable;                // Enabling XE
  UINT8   FastStringEnable;                 // Fast String
  UINT8   MonitorMwaitEnable;               // Monitor/Mwait
  UINT8   MachineCheckEnable;               // Machine Check
  UINT8   iTurboModeValue;
  UINT8   DcaEnable;                        // DCA setup option
  UINT8   DcaPrefetchDelayValue;            // DCA Prefetch Delay Value
  UINT8   MlcStreamerPrefetcherEnable;      // Hardware Prefetch
  UINT8   MlcSpatialPrefetcherEnable;       // Adjacent Cache Line Prefetch
  UINT8   DCUStreamerPrefetcherEnable;      // DCU Streamer Prefetcher
  UINT8   DCUIPPrefetcherEnable;            // DCU IP Prefetcher
  UINT8   DCUModeSelection;                 // DCU Mode Selection
  UINT8   ProcessorXapic;                   // Enable Processor XAPIC
  UINT8   BspSelection;                     // Select BSP
  UINT8   IedSize;                          // IED size
  UINT8   IedTraceSize;                     // IED trace size
  UINT8   TsegSize;                         // TSEG size
  UINT8   AllowMixedPowerOnCpuRatio;        // Allow Mixed PowerOn CpuRatio
  UINT8   UFSDisable;                       // Allow Mailbox Command to PCU_MISC_CONFIG Bit[28]
  UINT8   CheckCpuBist;                     // check and disable BIST faile core or ignore
  UINT32  CoreDisableMask[MAX_CPU_SOCKET];  // one for each CPU socket
  UINT8   CoresToEnable[MAX_CPU_SOCKET];    // one for each CPU socket AptioV server override
  UINT8   ProcessorFlexibleRatio;           // Non-Turbo Mode Processor Core Ratio Multiplier
  UINT8   ProcessorFlexibleRatioOverrideEnable;         // Non-Turbo Mode Processor Core Ratio Multiplier Enable
  UINT8   ProcessorTDCLimitOverrideEnable;  // Turbo-XE Mode Processor TDC Limit Override Enable
  UINT8   ProcessorTDCLimit;                // Turbo-XE Mode Processor TDC Limit
  UINT8   ProcessorTDPLimitOverrideEnable;  // Turbo-XE Mode Processor TDP Limit Override Enable
  UINT8   ProcessorTDPLimit;                // Turbo-XE Mode Processor TDP Limit
  UINT8   UpBootSelection;                  // UP boot mode selection
  UINT8   MTRRDefTypeUncachable;            // For changing UC to WB
  UINT8   TStateEnable;                     // T states enable?
  UINT8   OnDieThermalThrottling;           // Throtte ratio
  UINT8   OSCx;                             // ACPI C States
  UINT8   ForcePhysicalModeEnable;          // Force physical destionation mode
  UINT8   ProcessorVirtualWireMode;
  UINT8   DetectedCPU;

  UINT8   AesEnable;
  UINT8   PpinControl;                      // PPIN Control MSR
  UINT8   LockChipset;                      // Lock Chipset
  //
  // Processor Control
  //
  UINT8 TurboMode;
  UINT8 Extreme;
  UINT8 Eist;
  UINT8 XapicMode;

  // IOT/OCLA configs
  UINT32  IotEnCboBitmap[MAX_CPU_SOCKET];
  UINT8   OclaMaxTorEntry[MAX_CPU_SOCKET];
  UINT8   OclaMaxWay[MAX_CPU_SOCKET];
#ifndef OCLA_TOR_ENTRY_MAX
    #define OCLA_TOR_ENTRY_MIN         0
    #define OCLA_TOR_ENTRY_MAX      0x10
    #define OCLA_TOR_ENTRY_DEFAULT     1
    #define OCLA_WAY_MIN               0
    #define OCLA_WAY_MAX              20
    #define OCLA_WAY_DEFAULT           1
#endif


UINT32 IioLlcWaysMask ;				                  // MSR CBO_SLICE0_CR_IIO_LLC_WAYS bitmask. - Only Bits[0:19] are used
UINT32 QlruCfgMask_Lo ;                               // MSR VIRTUAL_MSR_CR_QLRU_CONFIG bitmask - Lower 32-bit
UINT32 QlruCfgMask_Hi ;                               // MSR VIRTUAL_MSR_CR_QLRU_CONFIG bitmask - Higher 32-bit

  //
  // Security
  //
  UINT8   ChassisIntrusion;
  UINT8   IdeSecurityMenuOnNextBoot;

  //
  // PCH_DEVICE_ENABLES
  //
  UINT8   BoardCapability;
  UINT8   DeepSxMode;
  UINT8   Gp27WakeFromDeepSx;
  UINT8   PchLan;
  UINT8   GbeRegionInvalid;
  UINT8   PchLanSupported;
  UINT8   PchWakeOnLan;
  UINT8   PchSlpLanLowDc;
  UINT8   PchPxeRom;
  UINT8   PchEfiNetworkSupport;
  UINT8   PchSmbus;
  UINT8   PchPciClockRun;
  UINT8   PchDisplay;
  UINT8   PchCrid;
  UINT8   PchGpioLockDown;
  UINT8   PchRtcLock;
  UINT8   PchBiosLock;
  UINT8   PchSerm;
  UINT8   PchFlashLockDown;//AptioV server override: Adding control for SPI Lock
  UINT8   PchGbeFlashLockDown;
  UINT8   Hpet;
  UINT8   HpetBootTime;
  UINT8   PchPort80Route;
  UINT8   EnableClockSpreadSpec;
  UINT8   IchPort80Route;
  UINT8   PchSirqMode;

  //
  // Usb Config
  //
  UINT8   UsbXhciSupport;
  UINT8   RouteUsb2PinsToWhichHc;
  UINT8   RouteUsb2Pin0;
  UINT8   RouteUsb2Pin1;
  UINT8   RouteUsb2Pin2;
  UINT8   RouteUsb2Pin3;
  UINT8   RouteUsb2Pin4;
  UINT8   RouteUsb2Pin5;
  UINT8   RouteUsb2Pin6;
  UINT8   RouteUsb2Pin7;
  UINT8   RouteUsb2Pin8;
  UINT8   RouteUsb2Pin9;
  UINT8   RouteUsb2Pin10;
  UINT8   RouteUsb2Pin11;
  UINT8   RouteUsb2Pin12;
  UINT8   RouteUsb2Pin13;
  UINT8   Usb3PinsTermination;
  UINT8   EnableUsb3Pin1;
  UINT8   EnableUsb3Pin2;
  UINT8   EnableUsb3Pin3;
  UINT8   EnableUsb3Pin4;
  UINT8   EnableUsb3Pin5;
  UINT8   EnableUsb3Pin6;
  UINT8   PchUsb30Mode;
  UINT8   UsbS755WA;
  UINT8   XhciDisMSICapability;
#ifdef ULT_FLAG
  UINT8   PchUsb20[1];
#else
  UINT8   PchUsb20[2];
#endif // ULT_FLAG
  UINT8   PchUsbPerPortCtl;
  UINT8   PchUsbPort[14];
  UINT8   PchUsb30Port[6];
  UINT8   UsbPrecondition;
  UINT8   XhciIdleL1;
  UINT8   Btcg;

  //
  // Sata CONFIG
  //
  UINT8   PchSata;
  //
  // Sata Interface Mode
  // 0 - IDE  1 - RAID  2 - AHCI
  //
  UINT8   SataInterfaceMode;
  UINT8   SataPort[PCH_SATA_MAX_PORTS];
  UINT8   SataHotPlug[PCH_SATA_MAX_PORTS];
  UINT8   SataMechanicalSw[PCH_SATA_MAX_PORTS];
  UINT8   SataSpinUp[PCH_SATA_MAX_PORTS];
  UINT8   SataExternal[PCH_SATA_MAX_PORTS];
  UINT8   PxDevSlp[PCH_SATA_MAX_PORTS];
  UINT8   SataType[PCH_SATA_MAX_PORTS];
  UINT8   SataRaidR0;
  UINT8   SataRaidR1;
  UINT8   SataRaidR10;
  UINT8   SataRaidR5;
  UINT8   SataRaidIrrt;
  UINT8   SataRaidOub;
  UINT8   SataHddlk;
  UINT8   SataLedl;
  UINT8   SataRaidIooe;
  UINT8   SataRaidSrt;
  UINT8   SataRaidLoadEfiDriver;
  UINT8   SataRaidOromDelay;
  UINT8   SataAlternateId;
  UINT8   SataSalp;
  UINT8   SataTestMode;
  UINT8   EnableDitoConfig;
  UINT8   DmVal;
  UINT8   DitoVal;


  //
  // sSata CONFIG
  //
  UINT8   PchsSata;
  //
  // Sata Interface Mode
  // 0 - IDE  1 - RAID  2 - AHCI
  //
  UINT8   sSataInterfaceMode;
  UINT8   sSataPort[PCH_SSATA_MAX_PORTS];
  UINT8   sSataHotPlug[PCH_SSATA_MAX_PORTS];
  UINT8   sSataMechanicalSw[3];
  UINT8   sSataSpinUp[PCH_SSATA_MAX_PORTS];
  UINT8   sSataExternal[PCH_SSATA_MAX_PORTS];
  UINT8   sPxDevSlp[PCH_SSATA_MAX_PORTS];
  UINT8   sSataType[PCH_SSATA_MAX_PORTS];
  UINT8   sSataRaidR0;
  UINT8   sSataRaidR1;
  UINT8   sSataRaidR10;
  UINT8   sSataRaidR5;
  UINT8   sSataRaidIrrt;
  UINT8   sSataRaidOub;
  UINT8   sSataHddlk;
  UINT8   sSataLedl;
  UINT8   sSataRaidIooe;
  UINT8   sSataRaidSrt;
  UINT8   sSataRaidLoadEfiDriver;
  UINT8   sSataRaidOromDelay;
  UINT8   sSataAlternateId;
  UINT8   sSataSalp;
  UINT8   sSataTestMode;
  UINT8   sEnableDitoConfig;
  UINT8   sDmVal;
  UINT8   sDitoVal;


  UINT8   HddAcousticPowerManagement;
  UINT8   HddAcousticMode;


  UINT8   WakeOnLanSupport;

  //
  // Enable/disable for PCIe LOM by using GPO44/45
  // NOT PCH LAN
  //
  UINT8   LomDisableByGpio;

  //PCH THERMAL SENSOR
  UINT8   ThermalDeviceEnable;
  UINT8   AlertEnableLock;
  UINT8   PchAlert;
  UINT8   DimmAlert;
  UINT8   PchCrossThrottling;

  //
  // RTC WAKE
  //
  UINT8   WakeOnRTCS5;
  UINT8   RTCWakeupTimeHour;
  UINT8   RTCWakeupTimeMinute;
  UINT8   RTCWakeupTimeSecond;

  //
  // PCI_EXPRESS_CONFIG, 8 ROOT PORTS
  //
  UINT8   PcieClockGating;
  UINT8   PcieDmiAspm;
  UINT8   PcieSBDE;
  UINT8   GbePciePortNum;
  UINT8   PciePortConfig1;
  UINT8   PciePortConfig2;
  UINT8   PcieDmiExtSync;
  UINT8   PcieDmiStopAndScreamEnable;
  UINT8   XTpmLen;
  UINT8   PcieRootPortSBDE;
  UINT8   PcieSBDEPort;
  UINT8   PcieUsbGlitchWa;
  UINT8   PcieRootPortFunctionSwapping;
  UINT8   PcieRootPortEn[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8   PcieRootPortAspm[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8   PcieRootPortURE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8   PcieRootPortFEE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8   PcieRootPortNFE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8   PcieRootPortCEE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8   PcieRootPortCTD[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8   PcieRootPortPME[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8   PcieRootPortMSIE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8   PcieRootPortSFE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8   PcieRootPortSNE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8   PcieRootPortSCE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8   PcieRootPortPMCE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8   PcieRootPortHPE[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8   PcieRootPortSpeed[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8   PcieRootPortTHS[PCH_PCIE_MAX_ROOT_PORTS];

  //
  // PCI Bridge Resources
  //
  UINT8   PcieExtraBusRsvd[PCH_PCIE_MAX_ROOT_PORTS];
  //Aptiov server override - support SB hot plug 
  UINT16  PcieMemRsvd[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8   PcieMemRsvdalig[PCH_PCIE_MAX_ROOT_PORTS];
  UINT16  PciePFMemRsvd[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8   PciePFMemRsvdalig[PCH_PCIE_MAX_ROOT_PORTS];
 //Aptiov server override - support SB hot plug 
  UINT8   PcieIoRsvd[PCH_PCIE_MAX_ROOT_PORTS];
  UINT8   PcieRootPortL1SubStates[PCH_PCIE_MAX_ROOT_PORTS];

  UINT8   FixupPlatformSpecificSoftstraps;

  UINT8   PchPcieGlobalAspm; // Override ASPM PCH PCIE port configuration.


   //
   // Reset Testing
   //

  UINT8   PchAzalia;
  UINT8   AzaliaDs;
  UINT8   AzaliaPme;

  //
  // Audio DSP Configuration
  //
  UINT8   PchAudioDsp;
  UINT8   PchAudioDspD3PowerGating;
  UINT8   PchAudioDspCodecSelect;
  UINT8   PchAudioDspAcpiMode;
  UINT8   PchAudioDspSafeMode;
  UINT8   PchAudioDspBluetooth;
  UINT8   PchAudioDspAcpiInterruptMode;

  UINT8   PchI2C0VoltageSelect;
  UINT8   PchI2C1VoltageSelect;

  //
  // ASF
  //
  UINT8   Asf;
  //
  // WatchDog
  //
  UINT8   WatchDog;
  UINT16  WatchDogTimerOs;
  UINT16  WatchDogTimerBios;
  //
  // Intel AMT
  //
  UINT8   Amt;
  UINT8   AmtbxPrompt;
  UINT8   AmtSpiLock;
  UINT8   AmtbxPasswordWrite;
  UINT8   HeciTimeout;
  UINT16  AmtWaitTimer;

  //
  // EventLog
  //
  UINT8   SystemErrorEn;
  UINT8   PCIeDownStreamPECIWrite;
  //Viral, Vmse and IoMca are not supported in EP. Will need to wrap in an EX flag
  //UINT8   ViralEn;
  UINT8   PoisonEn;
  UINT8   PchErrorEn;
  UINT8   IohErrorEn;
  UINT8   EMcaLogEn;
  UINT8   EMcaCSmiEn;
  UINT8   EMcaForceOptin;
  UINT8   MceSmiMorphingEn;
  UINT8   UboxUmcSmiEn;
  //Viral, Vmse and IoMca are not supported in EP. Will need to wrap in an EX flag
  //UINT8   IoMcaEn;
  UINT8   WheaSupport;
  UINT8   WheaEinj_05Extn;
  UINT8   WheaErrorRecordFormat;
  UINT8   WheaLogging;
  UINT8   PciExpressErrInj;
  UINT8   PcieErrInjActionTable;

  UINT8   McBankErrorInjectionSupport;
  UINT8   McBankWarmBootClearError;
  UINT16  SmiMcaDelay;
  UINT8   QPIFailoverSmiEn;
  UINT8	  CorrectedMemoryErrors;
  UINT8   SpareInt;
  UINT8   Cmci;
  UINT8   MirrorFailOver;
  UINT8	  UnlockMsr; // Aptio V Server Override - Unlocking the MSR 0x790 for the ErrorInjection to work from RAS tools
   //Viral, Vmse and IoMca are not supported in EP. Will need to wrap in an EX flag
  //UINT8	  VmseLinkFailure;
  //UINT8	  VmseDdrParityError;
  //UINT8	  VmseNbPersistentError;
  //UINT8	  VmseSbPersistentError;

  UINT8   ErrorPinProgrammingForIIO;		// Error pin Programming for IIO
  UINT8   irpp0_parityError;
  UINT8   irpp0_qtOverflow;
  UINT8   irpp0_unexprsp;
  UINT8   irpp0_csraccunaligned;
  UINT8   irpp0_uncecc;
  UINT8   irpp0_rcvdpoison;
  UINT8   irpp0_crrecc;
  UINT8   irpp1_parityError;
  UINT8   irpp1_qtOverflow;
  UINT8   irpp1_unexprsp;
  UINT8   irpp1_csraccunaligned;
  UINT8   irpp1_uncecc;
  UINT8   irpp1_rcvdpoison;
  UINT8   irpp1_crrecc;

  //UINT8   LerEnable;
  //UINT8   DisableC4LoggingDueToLER;
  UINT8   PCIEFatalErrorEn;
  UINT8   PCIEUnCorrErrorEn;
  UINT8   PCIECorrErrorEn;
  UINT8   PcieErrorsEn;
  UINT8   PropagateSerr;
  UINT8   PropagatePerr;
  UINT8   PcieCeErrThreshold;

//  UINT8	  McaToSmiEn;
//  UINT8   UmcSmiEnable;
//  UINT8	  UncorrectedErrorsEn;

  //
  // Boot Options
  //
  UINT8   SerialTerminal;
  UINT8   DebugMessages;
  UINT8   serialDebugMsgLvl;
  UINT8   PromoteRcWarnings;
  UINT8   PromoteRcMRCWarnings;
  UINT8   serialDebugTrace;

  UINT8   ForceSetup;
  UINT8   PfatEnabled;
  UINT8   PfatPlatformSupported;
  //UINT8   Use1GPageTable;     Aptio V Server Override: Remove Question use only PCD.
  //
  // UINT8   QuietBoot;
  //
  UINT8   FastBoot;

  //
  // Reserve Memory that is hidden from the OS.
  //
  UINT8   ReserveMem;
  UINT64  ReserveStartAddr;

  //
  // Reserve TAGEC Memory
  //
  UINT8  TagecMem;

#include <Setup/IioSetupInfoData.h>

  //
  // Dummy Data
  //
  UINT8   Dummy;

  //Usb Configdata
  UINT8   UsbMassDevNum;
  UINT8   IsInteractionAllowed;
  UINT8   UsbLegacySupport;
  UINT8   UsbEhciHandoff;
  UINT8   UsbHotplugFddSupport;
  UINT8   UsbHotplugCdromSupport;
  UINT8   UsbEmul6064;
  UINT8   UsbMassResetDelay;
  UINT8   UsbEmu1;
  UINT8   UsbEmu2;
  UINT8   UsbEmu3;
  UINT8   UsbEmu4;
  UINT8   UsbEmu5;
  UINT8   UsbEmu6;
  UINT8   UsbEmu7;
  UINT8   UsbEmu8;
  UINT8   UsbEmu9;
  UINT8   UsbEmu10;
  UINT8   UsbEmu11;
  UINT8   UsbEmu12;
  UINT8   UsbEmu13;
  UINT8   UsbEmu14;
  UINT8   UsbEmu15;
  UINT8   UsbEmu16;

  // Console Redirection
  /*UINT8   ConsoleRedirection; AptioV server override: Controls commented out to use AMI Terminal setup
  UINT8   FlowControl;
  UINT64  BaudRate;
  UINT8   TerminalType;
  UINT8   LegacyOsRedirection;
  UINT8   TerminalResolution;
  UINT8   DataBits;
  UINT8   Parity;
  UINT8   StopBits; */
  
#ifdef EFI_PCI_IOV_SUPPORT
  UINT8   SystemPageSize;
  UINT8   ARIEnable;
  UINT8   ARIForwarding;
  UINT8   SRIOVEnable;
  UINT8   MRIOVEnable;
#endif
  //
  // For VT-d ACS W/A
  //
  UINT8   VtdAcsWa;

  UINT8   IIO0Present;
  UINT8   IIO1Present;
  UINT8   IIO2Present;
  UINT8   IIO3Present;

  //
  // RAS
  //
  //
  // ME Setup entries - start here
  //
  UINT8   MeTimeout;
  UINT8   SpsWaitingTimer;    // TODO:
  UINT32  SpsAltitude;
  UINT16  SpsMctpBusOwner;

  UINT8   MeDidEnabled;                // Whether DRAM Init Done should be sent to ME
  UINT8   MeDidTimeout;                // DRAM Init Done acknowledge timeout
  UINT8   MeDidInitStat;               // Override DRAM status sent to ME

  UINT8   MeHmrfpoLockEnabled;
  UINT8   MeHmrfpoEnableEnabled;
  UINT8   MeEndOfPostEnabled;
  UINT8   MeGrLockEnabled;
  UINT8   MeHeci1Enabled;
  UINT8   MeHeci2Enabled;
  UINT8   MeIderEnabled;
  UINT8   MeKtEnabled;
  UINT8   MeSiliconViewTechEnabled;

  UINT8   MeMphyDebugCorruptEndpoints; //mPhy debug option - corrupted mPhy survivability
  UINT8   MeMphyDebugEnableSurvivabilityTable; //mPhy debug option - enable/disable mPhy survivability tables

  UINT8   NmPwrOptBootOverride;
  UINT8   NmPwrOptBoot;
  UINT8   NmCores2DisableOverride;
  UINT8   NmCores2Disable;
  UINT8   NmPowerMsmtOverride;
  UINT8   NmPowerMsmtSupport;
  UINT8   NmHwChangeOverride;
  UINT8   NmHwChangeStatus;

  UINT8   DcmiConnection;
  UINT8   IpmiFrb2Enable;
  UINT8   IpmiFrb2Policy;
  UINT16  IpmiFrb2Timeout;
  UINT8   DcmiBootDeviceForceOverride;
  UINT8   DcmiPowerRestorationPolicy;
  UINT8   IpmiClearSelLog;

  UINT8   IccClockEnablesOverride;   // Enable overriding default ICC Clock Enable

#if defined(ICC_SUPPORT) && ICC_SUPPORT
  UINT8   IccWdtEnabled;
  UINT8   IccSetClockEnables;
  UINT8   IccLockRegisters;
  UINT8   IccProfile;
  UINT8   ShowProfile;
  UINT8   NumProfiles;
  UINT8   IccPllShutdownEnabled;
  UINT32  BClkFrequency;
#define MAX_UI_CLOCKS 3
  UINT16 Frequency[MAX_UI_CLOCKS];
  UINT8  Spread[MAX_UI_CLOCKS];
  UINT8  DmiPegRatio[MAX_UI_CLOCKS];
  UINT8  ShowClock[MAX_UI_CLOCKS];
  UINT8  ShowFrequency[MAX_UI_CLOCKS];
  UINT8  ShowSpread[MAX_UI_CLOCKS];
  UINT8  ShowRatio[MAX_UI_CLOCKS];
  UINT8  LockMask;
  UINT8  ShowCustomLock;
  UINT8  AllowAdvancedOptions;
  UINT8  PlatformType;
#endif // ICC_SUPPORT

#if SPS_SUPPORT
  UINT8   IccClockEnables_FLEX0;     // [0]
  UINT8   IccClockEnables_FLEX1;     // [1]
  UINT8   IccClockEnables_FLEX2;     // [2]
  UINT8   IccClockEnables_FLEX3;     // [3]
  UINT8   IccClockEnables_PCICLK0;   // [7]
  UINT8   IccClockEnables_PCICLK1;   // [8]
  UINT8   IccClockEnables_PCICLK2;   // [9]
  UINT8   IccClockEnables_PCICLK3;   // [10]
  UINT8   IccClockEnables_PCICLK4;   // [11]
  UINT8   IccClockEnables_SRC0;      // [16]
  UINT8   IccClockEnables_SRC1;      // [17]
  UINT8   IccClockEnables_SRC2;      // [18]
  UINT8   IccClockEnables_SRC3;      // [19]
  UINT8   IccClockEnables_SRC4;      // [20]
  UINT8   IccClockEnables_SRC5;      // [21]
  UINT8   IccClockEnables_SRC6;      // [22]
  UINT8   IccClockEnables_SRC7;      // [23]
  UINT8   IccClockEnables_ITPXDP;    // [24]
  UINT8   IccClockEnables_PEG_A;     // [26]
  UINT8   IccClockEnables_PEG_B;     // [27]
  UINT8   IccClockEnables_DMI;       // [28]
  UINT8   IccClockEnables_DMI2;      // [29]
  UINT8   IccClockEnables_PEG_C;     // [30]
  UINT8   IccClockEnables_Modulator4Enable; // [31]

  UINT8   IccSscOverride;            // Enable overriding default ICC Spead Spectrum configuration
  UINT8   IccSscMode[8];
#endif // SPS_SUPPORT

#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  UINT8         MeType;
  // ME Setup Data (from MeSetup.sd)
  UINT8         MeFirmwareInfo;
  UINT8         MeImageType;
  UINT8         MeFwDowngrade;
  UINT8         LocalFwUpdEnabled;
  UINT8         MngState;
  UINT8         IfrUpdateEnabled;
  UINT8         MdesForBiosState;
//  UINT8         NFCEnable;
  UINT8         MdesCapability;
  UINT8         SvtForPchCap;
  UINT8         FWProgress;
#if defined(AMT_SUPPORT) && AMT_SUPPORT
  UINT8         AmtCiraRequest;
  UINT8         AmtCiraTimeout;
  UINT8         UnConfigureMe;
  UINT8         HideUnConfigureMeConfirm;
  UINT8         MebxDebugMsg;
  UINT8         USBProvision;
  UINT8         AmtbxSelectionScreen;
  UINT8         AmtbxHotKeyPressed;
  UINT8         MebxNonUiTextMode;
  UINT8         MebxUiTextMode;
  UINT8         MebxGraphicsMode;
#if defined(AT_SUPPORT) && AT_SUPPORT

  //
  // MeStateControl has had the flags controlling it changed 
  // to only ME_SUPPORT_FLAG and AMT_SUPPORT, AT_SUPPORT is no longer
  // used to control MeStateControl. Therefore adding Reserved 
  // to preserve offsets in the setup variable and relocating 
  // MeStateControl to the end of this structure. Given that 
  // AT_SUPPORT is not used in server, this should not present a problem.
  // 

  UINT8         Reserved;
  UINT8         AtStateValue;
  UINT8         AtAmBypass;
  UINT8         AtRecoveryAttempt;
  UINT8         AtEnterSuspendState;
#endif // AT_SUPPORT

#endif //AMT_SUPPORT
#endif // ME_SUPPORT_FLAG
  //
  // ME Setup entries - end here <><><><><>
  //

  //DRAM RAPL
//  UINT8   DramRaplInit;
//  UINT8   BwLimitTfOvrd;
  UINT8   HybridMode;
  UINT8   ForcedStressTest;
  UINT8   Numa;
  UINT8   PrevBootChannelInterleaving;
  UINT8   PrevBootNuma;
  UINT8   PrevBootSpareMode;
  UINT8   RASMode;
  UINT8   PrevBootRASMode;
  UINT8   LOT26UnusedVrPowerDownEnable;
  //UINT8   DramRaplNodeChEnabled[32];//=[8*4] - [MAX_MEM_NODE][MAX_CH];
  //UINT8   DramRaplNodeChDimmPresent[96];//=[8*4*3] - [MAX_MEM_NODE][MAX_CH][MAX_DIMM];
  //UINT8   DramRaplNodeChDimmSpdData[1632];//=[8*4*3*17] - [MAX_MEM_NODE][MAX_CH][MAX_DIMM][17];
  //UINT16  DramRaplPwrInfoMaxVal[4];//4 - MAX_CPU_SOCKET
  //UINT16  DramRaplPwrInfoTdpVal[4];//4 - MAX_CPU_SOCKET
  //UINT16  DramRaplPwrInfoMinVal[4];//4 - MAX_CPU_SOCKET

//
// Network setup entries - start here <><><><><>
//
  UINT8  EfiNetworkSupport;
//
// Network setup entries - end here <><><><><>
//


  //Power Management Setup options

  UINT8   PkgCstEntryValCtl;
  UINT8   SapmctlValCtl;

  // PERF_P_LIMIT_CONTROL (CSR 1:30:2:0xe4) < HSX C stepping
  UINT8   PerfPLmtThshld;
  UINT8   PerfPLimitClip;
  UINT8   DisPerfPInput;
  UINT8   ResolutionMode;
  UINT8   PerfPLimitEn;
  // PERF_P_LIMIT_CONTROL (CSR 1:30:2:0xe4) >= HSX C stepping
  UINT8   PerfPlimitDifferential;
  UINT8   PerfPLimitClipC;


  // SAPMCTL (CSR 1:30:1:0xB0)
  UINT8   SapmctlLock;
  UINT8   SetDecayDisb;
  UINT8   QpiL0sPllSenEn;
  UINT8   QpiL0PllSenEn;
  UINT8   IioPL0sPllSenEn;
  UINT8   IioPL0PllSenEn;
  UINT8   MdllOnDe;
  UINT8   MpllOnEn;
  UINT8   SacgMpll;
  UINT8   NswakeSrexit;
  UINT8   SacgSrexit;
  UINT8   MdllOffSen;
  UINT8   MpllOffSen;
  UINT8   SscgSen;
  UINT8   QpllOffEna;
  UINT8   PpllOffEna;
  UINT8   MpllOffEna;
  UINT8   SacgEna;
  // SAPMCTL EXT FOR IVT

  UINT8 Qpi2PkgcClkGateDis; // BIT18
  UINT8 Qpi01PkgcClkGateDis; // BIT17
  UINT8 IioPkgcClkGateDis; // BIT16
  UINT8 ForcePpllOff; //BIT4
  UINT8 Qpi2L0PllSenEn;
  UINT8 Qpi2L0sPllSenEn;

  UINT8   CpuPmTuning;

  // PKG_CST_ENTRY_CRITERIA_MASK (CSR 1:30:2:0x7c)
  UINT8   DRAMinSR[MAX_CPU_SOCKET];
  UINT8   Qpi0In[MAX_CPU_SOCKET];
  UINT8   Qpi1In[MAX_CPU_SOCKET];
  UINT8   Qpi2In[MAX_CPU_SOCKET];
  UINT8   Pcie0In[MAX_CPU_SOCKET];
  UINT8   Pcie1In[MAX_CPU_SOCKET];
  UINT8   Pcie2In[MAX_CPU_SOCKET];
  UINT8   Pcie3In[MAX_CPU_SOCKET];
  UINT8   Pcie4In[MAX_CPU_SOCKET];
  UINT8   Pcie5In[MAX_CPU_SOCKET];
  UINT8   Pcie6In[MAX_CPU_SOCKET];
  UINT8   Pcie7In[MAX_CPU_SOCKET];
  UINT8   Pcie8In[MAX_CPU_SOCKET];
  UINT8   Pcie9In[MAX_CPU_SOCKET];
  UINT8   Pcie10In[MAX_CPU_SOCKET];

  // MSR_POWER_CTL  0x1FC
  UINT8   EntryCriteria;          //Control Entry_Criteria
  UINT8   C2C3TT;
  UINT8   PkgCLatNeg;
  UINT8   LTRSwInput;
  UINT8   PwrPerfTuning;
  UINT8   PwrPerfSwitch;
  UINT8   EEPControl;
  UINT8   SAPMControl;
  UINT8   PholdSrDisable;
  UINT8   PholdCstPreventionInit;
  UINT8   FASTBrkIntEn;
  UINT8   FASTBrkSnpEn;
  UINT8   EnableProcHot;
  UINT8   ProchotResponse;
  UINT8   EETurboDisable;

  // PRIMARY_PLANE_CURRENT_CONFIG_CONTROL  0x601
  UINT8   PpcccLock;
  UINT8   PriPlnCurCfgValCtl;
  UINT8   Psi3Code;
  UINT8   Psi3Thshld;
  UINT8   Psi2Code;
  UINT8   Psi2Thshld;
  UINT8   Psi1Code;
  UINT8   Psi1Thshld;
  UINT16  CurrentLimit;

  // MSR_TURBO_POWER_LIMIT  0x610
  // CSR_TURBO_POWER_LIMIT  1:30:0:0xe8
  UINT8   TurboModeAvailable;
  UINT16  PowerLimit1Power;
  UINT8   PowerLimit1En;
  UINT16  PowerLimit1Time;
  UINT8   PkgClmpLim1;
  UINT8   PowerLimit2En;
  UINT16  PowerLimit2Power;
  UINT8   PkgClmpLim2;
  UINT8   TurboPowerLimitLock;

  UINT8   FastRaplDutyCycle;

  // DYNAMIC_PERF_POWER_CTL (CSR 1:30:2:0x64)
  UINT8   EepLOverride;
  UINT8   EepLOverrideEn;
  UINT8   ITurboOvrdEn;
  UINT8   CstDemotOvrdEN;
  UINT8   TrboDemotOvrdEn;
  UINT8   UncrPerfPlmtOvrdEn;
  UINT8   EetOverrideEn;
  UINT8   IoBwPlmtOvrdEn;
  UINT8   ImcApmOvrdEn;
  UINT8   IomApmOvrdEn;
  UINT8   QpiApmOvrdEn;

  // CSR_PCIE_ILTR_OVRD  (CSR 1:30:1:78)
  // SW_LTR_OVRD (MSR 0xa02) -- not used
  UINT8   SoftwareLtrOvrdValCtl;
  UINT8   SnpLatVld;
  UINT8   SnpLatOvrd;
  UINT8   SnpLatMult;
  UINT8   SnpLatVal;
  UINT8   NonSnpLatVld;
  UINT8   NonSnpLatOvrd;
  UINT8   NonSnpLatMult;
  UINT16  NonSnpLatVal;

  //
  UINT8 EnableThermalMonitor;
  UINT8 EnergyEfficientPState;
  UINT8 CStateUnDemotion;

	//OverClocking
  UINT8   TurboP01;
  UINT8   EnableXe;
  UINT8   OverclockingLock;
  UINT16  XeTdcLimit;
  UINT16  XeTdpLimit;    // OverClocking

  //
  // This was XeRatioLimit[MAX_CPU_CORES_PER_SOCKET],
  // however MAX_CPU_CORES_PER_SOCKET has changed from 18
  // to 24 for BDX. Therefore have moved XeRatioLimit to 
  // the end of this struct, the below member is to 
  // preserve offsets.
  //

  UINT16  ObsoleteXeRatioLimit[18];
  UINT8   XeVoltage;

  UINT32 SocketDevFuncHide[128];  //AptioV server override: UINT32 used
// ENERGY_PERF_BIAS_CONFIG (MSR 0xA01)

  UINT16  EngAvgTimeWdw1;
  UINT8   P0TtlTimeLow1;
  UINT8   P0TtlTimeHigh1;
  UINT8   WorkLdConfig;
  UINT8   AltEngPerfBIAS;
  UINT8   CurrentConfig;

//MRC Setup Option
  UINT8   DDR3DimmsPresent;
  UINT8   cpuType;                // CPU Type

  //Microcode load


// Config TDP
  UINT8   ConfigTDP;
  UINT8   ConfigTDPLevel;




//
// SERIALPORT BAUD RATE: Begin
//
  UINT32        SerialBaudRate;
//
// SERIALPORT BAUD RATE: END
//


//
// SMM Save State Mode
//
  UINT8 SmmSaveState;
//
// Targeted Smi Support
//
  UINT8 TargetedSmi;

//
// Config 4S SKU variable:Begin
//
  UINT8        FourSocketsSKUFound;
//
// Config 4S SKU variable: END
//

  //UINT8         BootAllOptions; AptioV server override: Remove unwanted option
  //UINT8         SetShellFirst;  AptioV server override: Remove unwanted option

  //
  // Overclocking related setup variables
  //
  UINT32 AboveCStepping;
  UINT8  PlatformOCSupport;
  UINT8  FilterPll;
  UINT8  OverclockingSupport;
  UINT8  IsOcDefaultsInitalized;

  //
  // CPU releated
  //
  UINT8  FlexOverrideEnable;
  UINT8  MaxEfficiencyRatio;
  UINT8  MaxNonTurboRatio;
  UINT8  FlexRatioOverride;
  UINT8  EnableGvDefault;
  UINT8  TurboModeDefault;
  UINT8  PowerLimit1TimeDefault;
  UINT16 PowerLimit1Default;
  UINT16 PowerLimit2Default;
  UINT16 XeTdcLimitDefault;
  UINT16 XeTdpLimitDefault;
  UINT16 XeTdcLimitHangBootValue;
  UINT16 XeTdpLimitHangBootValue;
  UINT8  XeRatioLimit1HangBootValue;
  UINT8  XeRatioLimit2HangBootValue;
  UINT8  XeRatioLimit3HangBootValue;
  UINT8  XeRatioLimit4HangBootValue;
  UINT16 VrCurrentLimitDefault;

  UINT8  BusSpeedEnable;
  UINT8  ProcessorBusSpeedOverride;
  UINT8  ProcessorBusSpeedOverrideDefault;

  UINT8  FlexVidOverride;
  UINT8  FlexVidOverrideDefault;

  UINT8  IMONScaling;
  UINT8  PciePll;

  UINT8  CoreMaxOcRatio;
  UINT8  CoreVoltageMode;
  UINT16 CoreVoltageOverride;
  UINT16 CoreVoltageOffset;
  UINT8  CoreVoltageOffsetPrefix;
  UINT16 CoreExtraTurboVoltage;

  //
  // Memmory related
  UINT8  XmpProfileEnable;
  UINT8  MemoryVoltageOverride;
  UINT8  MemoryVoltage;
  UINT8  MemoryVoltageDefault;
  //
  // Memory timing override (these options are used for Custom/User Profile)
  //
  UINT8   DdrRefClkDefault;
  UINT8   DdrRefClk;
  UINT8   DdrRatioDefault;
  UINT8   DdrRatio;
  UINT8   tCL;
  UINT8   tRPab;
  UINT8   NModeSupport;
  //
  // Default values to restore memory timings to in case of Platform instability
  //
  UINT8  tCLDefault;
  UINT8  tCWLDefault;
  UINT16 tFAWDefault;
  UINT16 tRASDefault;
  UINT16 tRCDefault;
  UINT8  tRCDDefault;
  UINT16 tREFIDefault;
  UINT16 tRFCDefault;
  UINT8  tRPDefault;
  UINT8  tRPabDefault;
  UINT8  tRRDDefault;
  UINT8  tRTPDefault;
  UINT8  tWRDefault;
  UINT8  tWTRDefault;
  UINT8  NModeDefault;
  //
  // Saving MCFrequency to be used in PEI phase during Overclocking
  //
  UINT16  PrevBootMcFrequency;

  //
  // ICC Related
  //
  UINT8  BClkOverride;
  UINT8  BclkAdjustable;

  UINT8  WDTSupportforNextOSBoot;
  UINT16 TimeforNextOSBoot;
  UINT8  PlatformUnstable;
  UINT8  Ck505Present;

  //
  // GT related
  //
  UINT8   GtOcSupport;
  UINT8   GtOcFrequency;
  UINT16  GtExtraTurboVoltage;
  UINT8   GtVoltageMode;
  UINT16  GtVoltageOverride;
  UINT16  GtVoltageOffset;
  UINT8   GtVoltageOffsetPrefix;

  //
  // CLR Related
  //
  UINT8   ClrMaxOcRatio;
  UINT8   ClrVoltageMode;
  UINT16  ClrVoltageOverride;
  UINT16  ClrVoltageOffset;
  UINT8   ClrVoltageOffsetPrefix;
  UINT16  ClrExtraTurboVoltage;

  //
  // Uncore Related
  //
  UINT16   UncoreVoltageOffset;
  UINT8    UncoreVoltageOffsetPrefix;
  UINT16   IoaVoltageOffset;
  UINT8    IoaVoltageOffsetPrefix;
  UINT16   IodVoltageOffset;
  UINT8    IodVoltageOffsetPrefix;

  //
  //  SVID and FIVR Related
  //
  UINT8   SvidEnable;
  UINT16  SvidVoltageOverride;
  UINT8   FivrFaultsEnable;
  UINT8   FivrEfficiencyEnable;

  //
  // FIVR Default setup options
  //
  UINT8   CoreMaxOcRatioDefault;
  UINT8   CoreVoltageModeDefault;
  UINT16  CoreVoltageOverrideDefault;
  UINT16  CoreVoltageOffsetDefault;
  UINT8   CoreVoltageOffsetPrefixDefault;
  UINT16  CoreExtraTurboVoltageDefault;
  UINT8   GtOcSupportDefault;
  UINT8   GtOcFrequencyDefault;
  UINT16  GtExtraTurboVoltageDefault;
  UINT16  GtOcVoltageDefault;
  UINT8   GtVoltageModeDefault;
  UINT16  GtVoltageOverrideDefault;
  UINT16  GtVoltageOffsetDefault;
  UINT8   GtVoltageOffsetPrefixDefault;
  UINT8   ClrMaxOcRatioDefault;
  UINT8   ClrVoltageModeDefault;
  UINT16  ClrVoltageOverrideDefault;
  UINT16  ClrVoltageOffsetDefault;
  UINT8   ClrVoltageOffsetPrefixDefault;
  UINT16  ClrExtraTurboVoltageDefault;
  UINT16  UncoreVoltageOffsetDefault;
  UINT8   UncoreVoltageOffsetPrefixDefault;
  UINT16  IoaVoltageOffsetDefault;
  UINT8   IoaVoltageOffsetPrefixDefault;
  UINT16  IodVoltageOffsetDefault;
  UINT8   IodVoltageOffsetPrefixDefault;
  UINT8   SvidEnableDefault;
  UINT16  SvidVoltageOverrideDefault;
  UINT8   FivrFaultsEnableDefault;
  UINT8   FivrEfficiencyEnableDefault;

  UINT16  C01MemoryVoltage;
  UINT16  C23MemoryVoltage;

  UINT16  CpuVccInVoltage;

  UINT8   VccIoVoltage;

  //
  // Additions for Grantley Refresh
  // begin here.
  //

  UINT16  XeRatioLimit[MAX_CPU_CORES_PER_SOCKET];

  UINT8   ProcessorStepping;
  UINT8   X2ApicOptOut;                     // Enable X2APIC_OPT_OUT Flag

#ifdef DE_SKU
  UINT8  UsePchHot;
  UINT8  UsePchTempFromME;
  UINT8  CpuToPchThrottle;
#endif

#ifdef BTG_FLAG
#if TESTMENU_FLAG || ME_TESTMENU_FLAG
  UINT8   SkipStopPbet;                     // Skip StopPbet  
#endif
#endif

  UINT8   SataLpm;
  UINT8   sSataLpm;
  UINT8   TpmOnSpi; //PCH Softstrap 1 BIT25 TPM On SPI

#if defined(BDX_HOST) && defined(DE_SKU) 
  UINT8   VtdErrorEn;
  UINT8   DmiErrorEn;
  UINT8   MiErrorEn;
  UINT8   IioCoreErrorEn;
  UINT8   DmaErrorEn;
  UINT8   CoherentInterfaceErrorEn;
#endif

#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
#if defined(AMT_SUPPORT) && AMT_SUPPORT

  UINT8         MeStateControl;

#endif //AMT_SUPPORT
#endif // ME_SUPPORT_FLAG

#ifdef GRANGEVILLE_FLAG
  UINT8  Res_VR_Misc[19];
#endif   //#ifdef GRANGEVILLE_FLAG

#ifdef GRANGEVILLE_FLAG

  UINT8 IpmiSupportedPlatform;

#endif

  UINT32 AboveHSXCStepping;

  //
  // From IioConfigData.h //!!!!
  //

  UINT8 Ecrc[MAX_TOTAL_PORTS];
  UINT8 PciePortLinkMaxWidth[MAX_TOTAL_PORTS]; // on Setup
  UINT8 Iou0Np_Pf[MAX_CPU_SOCKET];                      //On setup WA  4988093
  UINT8 Iou1Np_Pf[MAX_CPU_SOCKET];                      //On setup 
  UINT8 Iou2Np_Pf[MAX_CPU_SOCKET];                      //On setup

  UINT8 NtbPpdAutoConfig[MAX_CPU_SOCKET];
  UINT8 NtbXlinkCtlAutoConfig[MAX_CPU_SOCKET];

#ifdef DE_SKU
  UINT8 Disable_SC_Cb3Dma;                   // Disable the complete SC CB3 DMA device
  UINT8 Disable_SC_GbE;                      // Disable the complete SC GbE device
  UINT8 SC_GbE_PF0;                          // Force Enable / Disable SC GbE physical function 0
  UINT8 SC_GbE_PF1;                          // Force Enable / Disable SC GbE physical function 1
#endif // DE_SKU

  UINT8 PCIe_Enable_SSD_SUPPORT;
  UINT8 VPP_INVERT[MAX_CPU_SOCKET];
  UINT8 IioLinkTrainPhase;

  //
  // From IioMemoryConfigData.h //!!!!
  //
  
  UINT8                   cmdVrefCenter;
  UINT8                   trainrxctle;
  UINT8                   lrdimmbacksideVref;

#ifdef SSA_FLAG
    UINT8                   EnableBiosSsaLoader;
#endif

  UINT8                   C2FMode;
  UINT8                   MemDownConfig;
  UINT8                   ADRDataSaveMode;

#if !defined(DE_SKU) || defined(SV_HOOKS)
  UINT8                   WrCRC;
#endif
  UINT8                   ceccWaChMask;

  UINT8                   EnCECCWA;
  UINT8                   CapErrFlow;
  UINT8                   WrCrcErrFlow;
  //
  //HWPM starts
  //
  UINT8 ProcessorHWPMEnable;
  UINT8 ProcessorAutonumousCstateEnable;
  //
  //HWPM ends
  //
#ifdef BDX_HOST 
  UINT8   CloakingEn;
  UINT8   HandleHardErrorDetection;
  UINT32  HardErrorDetection_threshold;
  UINT32  HardErrorDetection_duration;
#endif
#ifdef DE_SKU
  //
  // PowerManagement WA
  //
  UINT8  PchEnableThermalLockDown;
#endif //DE_SKU
#ifdef DE_SKU
#endif // DE_SKU

#ifdef GRANGEVILLE_FLAG
  UINT16 Res1_VR_Override;
#else
  UINT16 Res2_VR_Override;
#endif   //#ifdef GRANGEVILLE_FLAG

  UINT8  AcsControl;
  UINT8  UncoreFreq;
  UINT8  PcieExtndErrorsEn;
  UINT8  lrdimmWrVref;
  UINT8  lrdimmRdVref;
  UINT8  lrdimmRxDqCentering;
  UINT8  lrdimmTxDqCentering;
  UINT8  EnforceThreeMonthTimeout;
  UINT8  EnableBacksideRMT;
  UINT32 rmtPatternLengthExt;
#ifndef DE_SKU
  UINT8  Apply_s328_cbdma_WA;
#endif
  UINT8  UncoreCLROverrideEnable;
  UINT8  EnableAntiFlashWearout;
  UINT8  AntiFlashWearoutSupported;

  UINT8   WatchDogOs;
  
  
  //
  // Overclocking related setup variables
  //
  UINT16 ChangePll;
  UINT8  AdjustPll;
  UINT32 PllTrim;  
  UINT8  PllTrimPrefix;
  UINT8  TjMaxoffset;

  //
  // CLR Related
  //
  UINT8   RingMinRatio;

  UINT8   RmtOnColdFastBoot;
  
  //
  // USB related
  //
  UINT8   UsbNonBoot;

  //
  // PPR related
  //
  UINT8                   pprType;
  UINT8                   pprErrInjTest;

  UINT8   CoreBiosDoneEnabled;

  // WARNING : DO NOT EXTEND OR MODIFY THIS DATA STRUCTURE HERE
  //
  // New knobs (CRB- and SDP- related) added as part of Refresh program needs to go to "IioSetupInfoDataBdx.h" file
  // This is done to preserve capsule BIOS updates. 
#include <Setup/IioSetupInfoDataBdx.h>

} SYSTEM_CONFIGURATION;
#pragma pack()

#define EFI_HDD_PRESENT       0x01
#define EFI_HDD_NOT_PRESENT   0x00
#define EFI_CD_PRESENT        0x02
#define EFI_CD_NOT_PRESENT    0x00

#define EFI_HDD_WARNING_ON    0x01
#define EFI_CD_WARNING_ON     0x02
#define EFI_SMART_WARNING_ON  0x04
#define EFI_HDD_WARNING_OFF   0x00
#define EFI_CD_WARNING_OFF    0x00
#define EFI_SMART_WARNING_OFF 0x00


extern EFI_GUID  gMainPkgListGuid;
extern EFI_GUID  gAdvancedPkgListGuid;
extern EFI_GUID  gTpmPkgListGuid;
extern EFI_GUID  gSecurityPkgListGuid;
extern EFI_GUID  gBootOptionsPkgListGuid;


#ifndef VFRCOMPILE

extern EFI_GUID gEfiSetupVariableGuid;
extern EFI_GUID gEfiSetupVariableDefaultGuid;
extern EFI_GUID gEfiGlobalVariableControlGuid;

typedef struct {
  UINT8 ProcessController;
} SYSTEM_CONFIGURATION_CONTROL;

#endif

#define SYSTEM_PASSWORD_ADMIN   0
#define SYSTEM_PASSWORD_USER    1

#endif // #ifndef _SETUP_VARIABLE
