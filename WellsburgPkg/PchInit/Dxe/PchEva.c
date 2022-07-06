/** @file
  Configures PCH EVA Controller Unit.

@copyright
  Copyright (c) 2011 - 2013 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
#include <Token.h> // AptioV server override

#ifdef PCH_SERVER_BIOS_FLAG
#include "PchInit.h"
#include "PchRegs.h"
#include "Library/PchPlatformLib.h"
#include <IndustryStandard/Pci.h>
#include <Protocol/PciIo.h>
#include <Library/UefiLib.h>

EFI_IIO_UDS_PROTOCOL  		*mIohUds; // AptioV server override: For sSATA BAR allocation based on MMIO/IO ranges  
  
EFI_STATUS
ConfiguresSataAhci (
  DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  UINT32                              RootComplexBar,
  UINT16                              GpioBase
  );
  
EFI_STATUS
ConfiguresSataSpeedIde (
  DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  UINTN                               PciDevFuncRegBase,
  UINTN                               MaxPorts
  );
  

/**
   This function is called by a ready to boot event.to lock all Eva devices
   such as MSROM, MSUINT and SMBUS
  
  @param[in]   PchPlatformPolicy. PolicyProtocol
   
  @retval      EFI_SUCCESS. 

**/
EFI_STATUS
EvaLockDown (
     DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy
  )
{
  UINTN   evaMemBase;
  UINT8   func;
  UINT32  dnDevFuncHide;
  UINT32  PchEvaPlkCtl[PCH_WBG_MSUINT_FUNCS]= {
                R_PCH_WBG_MROM_PLKCTL,
                R_PCH_WBG_SMBUS0_PLKCTL,
                R_PCH_WBG_SMBUS1_PLKCTL,
                R_PCH_WBG_SMBUS2_PLKCTL                
                };
  UINT32  PchEvaMsunitFuncsNo[PCH_WBG_MSUINT_FUNCS]= {
                PCI_FUNCTION_NUMBER_EVA_MROM,
                PCI_FUNCTION_NUMBER_EVA_SMBUS0,
                PCI_FUNCTION_NUMBER_EVA_SMBUS1,
                PCI_FUNCTION_NUMBER_EVA_SMBUS2
                };  
  PCH_EVA_CONFIG    *EvaConfig;
  
  EvaConfig = PchPlatformPolicy->EvaConfig;

  /// Don't lock anything if setup value ask for it. 
  if (EvaConfig->LockDown == 0){
    return EFI_SUCCESS;
  }

  evaMemBase = MmPciAddress (
                    0,
                    PchPlatformPolicy->BusNumber,
                    PCI_DEVICE_NUMBER_EVA,
                    PCI_FUNCTION_NUMBER_EVA_MROM,
                    0
                    );
  
  dnDevFuncHide =MmioRead32(evaMemBase + R_PCH_WBG_MSUINT_MSDEVFUNCHIDE);
   
  // Lock down registers on all functions on EVA Controller
  for (func=0; func < PCH_WBG_MSUINT_FUNCS; func++)
  {
     evaMemBase = MmPciAddress (
                       0,
                       PchPlatformPolicy->BusNumber,
                       PCI_DEVICE_NUMBER_EVA,
                       PchEvaMsunitFuncsNo[func],
                       0
                       );
     
     // Ignore hidden functions. 
     if ((dnDevFuncHide & (1<<func)) == 0 ) {
         MmioWrite16(evaMemBase + PchEvaPlkCtl[func],BIT0);
       }
  }
  
  evaMemBase = MmPciAddress (
                    0,
                    PchPlatformPolicy->BusNumber,
                    PCI_DEVICE_NUMBER_EVA,
                    PCI_FUNCTION_NUMBER_EVA_SSATA,
                    0
                    );

  // Lock registers on SSATA Port:
  MmioWrite32(evaMemBase + R_PCH_SATA_SCLKGC, BIT31);
  
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) (evaMemBase + R_PCH_SATA_SCLKGC),
    1,
    (VOID *) (UINTN) (evaMemBase + R_PCH_SATA_SCLKGC)
    );
  
  return EFI_SUCCESS;
}

/**
  Configures PCH Sata Controller

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] RootComplexBar       RootComplexBar value of this PCH device
  @param[in] GpioBase             GPIO base address of this PCH device

  @retval EFI_SUCCESS             The function completed successfully

**/
EFI_STATUS
ConfiguresSata (
  IN      DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  IN      UINT32                              RootComplexBar,
  IN      UINT16                              GpioBase
  )
{
  EFI_STATUS      Status;
  UINT32          Index;
  UINT16          WordReg;
  UINT16          sSataGcReg;
  UINTN           PciD17F4RegBase;
  UINTN           PciD17F0RegBase; 
  UINTN           PciD31F0RegBase;
  UINT16          LpcDeviceId;
  PCH_SATA_CONFIG *sSataConfig;
  UINT16          sSataPortsEnabled;
  UINT16          sSataModeSelect;
  UINTN           PciDevFuncRegBase;
  UINTN           MaxPorts;
  UINT32          Data32And;
  UINT32          Data32Or;
  UINTN           DummyHandle; 

  UINT16          AnyPortsEnabled;

  DEBUG ((DEBUG_INFO, "ConfiguresSata() Start\n"));

  sSataConfig      = PchPlatformPolicy->sSataConfig;
  PciD17F4RegBase = MmPciAddress (0, 
                                  PchPlatformPolicy->BusNumber, 
                                  PCI_DEVICE_NUMBER_EVA,
                                  PCI_FUNCTION_NUMBER_EVA_SSATA,
                                  0);
  PciD17F0RegBase = MmPciAddress (0, 
                                  PchPlatformPolicy->BusNumber, 
                                  PCI_DEVICE_NUMBER_EVA, 
                                  PCI_FUNCTION_NUMBER_EVA_MROM, 
                                  0); 

  sSataModeSelect  = MmioRead16 (PciD17F4RegBase + R_PCH_SATA_MAP) & B_PCH_SATA_MAP_SMS_MASK;
  PciD31F0RegBase = MmPciAddress (0, PchPlatformPolicy->BusNumber, 31, 0, 0);
  LpcDeviceId     = MmioRead16 (PciD31F0RegBase + R_PCH_LPC_DEVICE_ID);
  Status          = EFI_SUCCESS;
  
  /// If running with LPT do not initialize sSATA
  if (!IS_PCH_WBG_LPC_DEVICE_ID(LpcDeviceId))
    return Status; 

  //
  // If for all sSATA ports:
  // - Hot plug is disabled
  // - A device is not attached
  // - Test mode is not enabled
  // - Not configured as eSATA port
  // Then disable the sSATA controller *for CRB only*
  //

  AnyPortsEnabled = 0;

  //
  // Get the port enable bits
  //
  WordReg = MmioRead16 (PciD17F4RegBase + R_PCH_SATA_PCS);

  //
  // Look at each port for a reason *not* to disable the whole controller
  //
  for (Index = 0; Index < GetPchMaxsSataPortNum (); Index++) {
    if (
        (sSataConfig->PortSettings[Index].HotPlug == PCH_DEVICE_ENABLE) ||
        (WordReg & (BIT0 << (8 + Index))) ||
        (sSataConfig->TestMode == PCH_DEVICE_ENABLE) ||
        (sSataConfig->PortSettings[Index].External == PCH_DEVICE_ENABLE)
       ) {
      AnyPortsEnabled |= (1 << Index);
    }
  }

  ///
  /// Check to disable SATA controller
  ///
  if ( (PchPlatformPolicy->DeviceEnabling->sSata == PCH_DEVICE_DISABLE)
      || (!AnyPortsEnabled)
      ) {
    ///
    /// PCH BIOS Spec Rev 0.5.0, Section 14.2 SATA Controller Disabling
    /// Step 1
    /// Set D17:F4:92h [5:0] to 000000b
    ///
    MmioAnd16 (PciD17F4RegBase + R_PCH_SATA_PCS,
              (UINT16) ~(B_PCH_SATA_PCS_PORT0_EN | B_PCH_SATA_PCS_PORT1_EN | B_PCH_SATA_PCS_PORT2_EN |
                         B_PCH_SATA_PCS_PORT3_EN | B_PCH_SATA_PCS_PORT4_EN | B_PCH_SATA_PCS_PORT5_EN));
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint16,
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_PCS),
      1,
      (VOID *) (UINTN) (PciD17F4RegBase + R_PCH_SATA_PCS)
      );

    ///
    /// Step 3
    /// Set Sata Port Clock Disable bits D17:F4:94h[29:24] to 3Fh
    ///
    MmioOr32 ((UINTN) (PciD17F4RegBase + R_PCH_SATA_SCLKCG),
              (UINT32) (B_PCH_SATA_SCLKCG_PORT0_PCD | B_PCH_SATA_SCLKCG_PORT1_PCD | B_PCH_SATA_SCLKCG_PORT2_PCD |
                        B_PCH_SATA_SCLKCG_PORT3_PCD ));
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_SCLKCG),
      1,
      (VOID *) (UINTN) (PciD17F4RegBase + R_PCH_SATA_SCLKCG)
      );
    ///
    /// Step 4
    /// Disabling SATA Device by programming BDF 0 17 0 Offset D4 bits 4 and 5
    ///
    MmioOr32(PciD17F0RegBase + R_PCH_WBG_MSUINT_MSDEVFUNCHIDE, 
            (UINT32)(B_PCH_EVA_MSUNIT_MSDEVFUNCHIDE_SSATA2 | 
             B_PCH_EVA_MSUNIT_MSDEVFUNCHIDE_SSATA ));
             
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciD17F0RegBase + R_PCH_WBG_MSUINT_MSDEVFUNCHIDE),
      1,
      (VOID *) (UINTN) (PciD17F0RegBase + R_PCH_WBG_MSUINT_MSDEVFUNCHIDE)
      );

  if (!AnyPortsEnabled) {
    DEBUG ((EFI_D_ERROR,"ConfiguresSata(): sSATA controller disabled - no ports enabled, no hotplug, no eSATA\n", Status));
  } else {
    DEBUG ((EFI_D_ERROR,"ConfiguresSata(): sSATA controller disabled by user input\n", Status));
  }

    return EFI_SUCCESS;
  }
  ///
  /// PCH BIOS Spec Rev 0.5.0 section 14.1.7 Additional Programming Requirements during
  /// SATA Initialization
  /// Step 12
  /// Program D17:F4:9Ch[5] to 1b (Note: this must be programmed together with D17:F4:9Ch[7:6]
  /// in word write)
  ///
  sSataGcReg = MmioRead16 (PciD17F4RegBase + R_PCH_SATA_SCLKGC);
  sSataGcReg |= BIT5;

  switch (sSataModeSelect) {

  case V_PCH_SATA_MAP_SMS_IDE:
    ///
    /// Set Native IDE decoding
    ///
    /// PCH BIOS Spec Rev 0.5.0 Section 14.1.3 Set the Programming Interface
    /// Using native IDE is only possible when the SATA controller is operating in IDE mode.
    /// The programming interface is selected by setting the programming interface register
    /// (PI = Reg: 09h) appropriately.  There are two native mode enabling bits in the
    /// PI register to control the primary and secondary channels of SATA1.
    ///
    /// Only D17:F4 needs to be set. D17:F5 is readonly
    ///
    if (sSataConfig->LegacyMode == PCH_DEVICE_ENABLE) {
      MmioAnd8 (
        PciD17F4RegBase + R_PCH_SATA_PI_REGISTER,
        (UINT8)~(B_PCH_SATA_PI_REGISTER_PNE | B_PCH_SATA_PI_REGISTER_SNE)
        );
    } else {
      MmioOr8 (
        PciD17F4RegBase + R_PCH_SATA_PI_REGISTER,
        (UINT8) B_PCH_SATA_PI_REGISTER_PNE | B_PCH_SATA_PI_REGISTER_SNE
        );
    }

    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint8,
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_PI_REGISTER),
      1,
      (VOID *) (UINTN) (PciD17F4RegBase + R_PCH_SATA_PI_REGISTER)
      );

    break;


  case V_PCH_SATA_MAP_SMS_RAID:
    if (IS_PCH_LPT_LPC_DEVICE_ID_SERVER (LpcDeviceId) 
     || IS_PCH_WBG_LPC_DEVICE_ID (LpcDeviceId)
    ) {
#ifdef PCH_SERVER_BIOS_FLAG
 if ( PchPlatformPolicy->RaidDevice->DeviceId == PchRaidRST )
      {
        sSataGcReg &= ~B_PCH_SATA_SCLKGC_AIE;
        sSataGcReg &= ~B_PCH_SATA_SCLKGC_AIES;
      } else {
        sSataGcReg &= ~B_PCH_SATA_SCLKGC_AIE;
        sSataGcReg |= B_PCH_SATA_SCLKGC_AIES;
      }
#else
      sSataGcReg &= ~B_PCH_SATA_SCLKGC_AIE;
      sSataGcReg |= B_PCH_SATA_SCLKGC_AIES;
#endif
    }    
    ///
    /// When RAID alternate ID is enabled, the Device ID will change to 30XX from 282X in RAID mode
    ///
    if (sSataConfig->RaidAlternateId == PCH_DEVICE_ENABLE) {
      sSataGcReg &= ~B_PCH_SATA_SCLKGC_AIES;
      sSataGcReg |= B_PCH_SATA_SCLKGC_AIE;
    }

    if (sSataConfig->SataEfiRaid == 1){
        Status = gBS->LocateProtocol(&gPchsSataEfiLoadProtocolGuid, NULL, (VOID **)&DummyHandle ); 
        if (EFI_ERROR(Status)) {
           Status = gBS->InstallProtocolInterface (
                         &mImageHandle, 
                         &gPchsSataEfiLoadProtocolGuid,
                         EFI_NATIVE_INTERFACE,
                         NULL
                        );
           if (EFI_ERROR(Status)){
              DEBUG ((EFI_D_ERROR,"sSata RSTe EFI OpRom NOT installed = %x!!!\n", Status));
           } else {
              DEBUG ((EFI_D_ERROR,"sSata RSTe EFI OpRom install successfully = %x!\n",Status));
           }
         }
       }
    break;
  }
  ///
  /// Unconditional write is necessary to lock the register
  ///
  MmioWrite16 (PciD17F4RegBase + R_PCH_SATA_SCLKGC, sSataGcReg);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint16,
    (UINTN) (PciD17F4RegBase + R_PCH_SATA_SCLKGC),
    1,
    &sSataGcReg
    );

  ///
  /// Set legacy IDE decoding
  /// These bits also effect with AHCI mode when PCH is using legacy mechanisms.
  ///
  MmioOr16 ((UINTN) (PciD17F4RegBase + R_PCH_SATA_TIMP), (UINT16) (B_PCH_SATA_TIM_IDE));
  WordReg = MmioRead16 (PciD17F4RegBase + R_PCH_SATA_TIMP);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint16,
    (UINTN) (PciD17F4RegBase + R_PCH_SATA_TIMP),
    1,
    &WordReg
    );

  MmioOr16 ((UINTN) (PciD17F4RegBase + R_PCH_SATA_TIMS), (UINT16) (B_PCH_SATA_TIM_IDE));
  WordReg = MmioRead16 (PciD17F4RegBase + R_PCH_SATA_TIMS);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint16,
    (UINTN) (PciD17F4RegBase + R_PCH_SATA_TIMS),
    1,
    &WordReg
    );
  ///
  /// PCH BIOS Spec Rev 0.5.0, section 19.10
  /// Step 4.2
  /// D17:F4 PCS: Enable the port in any of below condition:
  /// Hot plug is enabled
  /// A device is attached
  /// Test mode is enabled
  /// Configured as eSATA port
  ///
  sSataPortsEnabled  = 0;
  WordReg           = MmioRead16 (PciD17F4RegBase + R_PCH_SATA_PCS);
  for (Index = 0; Index < GetPchMaxsSataPortNum (); Index++) {
    if ((sSataConfig->PortSettings[Index].HotPlug == PCH_DEVICE_ENABLE) ||
        (WordReg & (BIT0 << (8 + Index))) ||
        (sSataConfig->TestMode == PCH_DEVICE_ENABLE) ||
        (sSataConfig->PortSettings[Index].External == PCH_DEVICE_ENABLE)) {
      sSataPortsEnabled |= (sSataConfig->PortSettings[Index].Enable << Index);
    }
  }
//
// SATASGPIO_START
//
#ifdef SDP_FLAG
  sSataPortsEnabled |= 0x0F;
#endif
//
// SATASGPIO_END
//       

  ///
  /// Set MAP register
  /// Set D17:F4 MAP[13:8] to 1b if SATA Port 0/1/2/3 is disabled
  /// SataPortsEnabled [3:0] = Sata Port 0/1/2/3 enable status
  /// MAP.SPD (D17:F4:Reg90h[13:8]) is Write Once
  ///

  MmioOr16 ((UINTN) (PciD17F4RegBase + R_PCH_SATA_MAP), ((~sSataPortsEnabled << 8) & (UINT16) B_PCH_WBG_SATA_MAP_SPD));
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint16,
    (UINTN) (PciD17F4RegBase + R_PCH_SATA_MAP),
    1,
    (VOID *) (UINTN) (PciD17F4RegBase + R_PCH_SATA_MAP)
    );

  ///
  /// D17:F4 PCS[3:0] = Port 0~3 Enabled bit, Disable port 4~5
  ///
  MmioAndThenOr16 (
    (UINTN) (PciD17F4RegBase + R_PCH_SATA_PCS),
    (UINT16)(~( B_PCH_SATA_PCS_PORT5_EN |
                B_PCH_SATA_PCS_PORT4_EN |
                B_PCH_SATA_PCS_PORT3_EN |
                B_PCH_SATA_PCS_PORT2_EN |
                B_PCH_SATA_PCS_PORT1_EN |
                B_PCH_SATA_PCS_PORT0_EN )),
    (UINT16) (sSataPortsEnabled)
    );

  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint16,
    (UINTN) (PciD17F4RegBase + R_PCH_SATA_PCS),
    1,
    (VOID *) (UINTN) (PciD17F4RegBase + R_PCH_SATA_PCS)
    );
  

  // AptioV server override start: Allocating sSATA BAR based on IIO UDS CPU0 MMIO/IO ranges
  Status = gBS->LocateProtocol(
							&gEfiIioUdsProtocolGuid,
							NULL,
							&mIohUds);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)){
		return Status;
  }
  // AptioV server override end  

  ///
  /// Configure AHCI
  ///

  if (sSataModeSelect != V_PCH_SATA_MAP_SMS_IDE) {

    Status = ConfiguresSataAhci (PchPlatformPolicy, RootComplexBar, GpioBase);

  } else {
        PciDevFuncRegBase = PciD17F4RegBase;
        MaxPorts          = PCH_IDE_1_MAX_PORTS;
      Status = ConfiguresSataSpeedIde (PchPlatformPolicy, PciDevFuncRegBase, MaxPorts);
    }

  ///
  /// PCH BIOS Spec Rev 0.5.0, section 19.10
  /// Step 4.2
  /// After configuring Port and Control Status (PCS) Register Port x Enabled (PxE) bits accordingly,
  /// wait 1.4 micro second
  ///
  PchPmTimerStall (0x02);
  PmTimerStallS3Item (0x02);
  ///
  /// PCH BIOS Spec Rev 0.5.0, section 19.10
  /// Step 4.1
  /// Set bits D17:F4:Reg 94h[29:24] to 3Fh as part of the chipset initialization and before disabling the
  /// SATA function if the SATA interface is not supported. BIOS can also set PCD bits for the un-routed ports
  /// on the platform to disable clocks for the unused ports
  /// Set the PCD [port x] = 1 if the corresponding PCS.PxE = 0 (either have a device attached or have
  /// hot plug enabled)
  ///
  for (Index = 0; Index < GetPchMaxsSataPortNum (); Index++) {
    if ((sSataPortsEnabled & (B_PCH_SATA_PCS_PORT0_EN << Index)) == 0) {
      MmioOr32 ((UINTN) (PciD17F4RegBase + R_PCH_SATA_SCLKCG), (UINT32) (B_PCH_SATA_SCLKCG_PORT0_PCD << Index));
    }
  }
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) (PciD17F4RegBase + R_PCH_SATA_SCLKCG),
    1,
    (VOID *) (UINTN) (PciD17F4RegBase + R_PCH_SATA_SCLKCG)
    );
  ///
  /// PCH BIOS Spec Rev 0.5.0, 14.1.6 Power Optimizer Configuration
  /// System BIOS must execute the following steps as part of System BIOS initialization
  /// of the PCH SATA controller on both cold boot (G3/S5) and S3/S4 resume path. Please
  /// refer to the PCH EDS, section 14.1.35.1 for the SATA initialization settings and
  /// the actual register indexes/values to be programmed.
  ///
  if (PchPlatformPolicy->PwrOptConfig->PchPwrOptsSata == PCH_DEVICE_ENABLE) {
    ///
    /// Step 1
    /// Set D31:F2 + SIR Index 64h[31:0] = 883C9001h
    ///
    MmioWrite8 (PciD17F4RegBase + R_PCH_SATA_SIRI, 0x64);
    MmioWrite32 (PciD17F4RegBase + R_PCH_SATA_STRD, 0x883C9001);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint8,
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_SIRI),
      1,
      (VOID *) (UINTN) (PciD17F4RegBase + R_PCH_SATA_SIRI)
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_STRD),
      1,
      (VOID *) (UINTN) (PciD17F4RegBase + R_PCH_SATA_STRD)
      );
    ///
    /// Step 2
    /// Set D17:F4 + SIR Index 68h[15:0] = 880Ah
    ///
    MmioWrite8 (PciD17F4RegBase + R_PCH_SATA_SIRI, 0x68);
    Data32And = 0xFFFF0000;
#ifdef PCH_SERVER_BIOS_FLAG
    Data32Or  = 0x0000882D;
#else
    Data32Or  = 0x0000880A;
#endif
    MmioAndThenOr32 (
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_STRD),
      Data32And,
      Data32Or
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint8,
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_SIRI),
      1,
      (VOID *) (UINTN) (PciD17F4RegBase + R_PCH_SATA_SIRI)
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_STRD),
      1,
      (VOID *) (UINTN) (PciD17F4RegBase + R_PCH_SATA_STRD)
      );
	}
    ///
    /// Step 3
    /// Set D17:F4 + SIR Index 60h[3] = 1b
    ///
    MmioWrite8 (PciD17F4RegBase + R_PCH_SATA_SIRI, 0x60);
    Data32And = 0xFFFFFFF7;
    Data32Or  = 0x00000008;
    MmioAndThenOr32 (
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_STRD),
      Data32And,
      Data32Or
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint8,
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_SIRI),
      1,
      (VOID *) (UINTN) (PciD17F4RegBase + R_PCH_SATA_SIRI)
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_STRD),
      1,
      (VOID *) (UINTN) (PciD17F4RegBase + R_PCH_SATA_STRD)
      );
    ///
    /// Step 4
    /// Set D17:F4 + SIR Index 60h[0] = 1b
    /// 
    MmioWrite8 (PciD17F4RegBase + R_PCH_SATA_SIRI, 0x60);
    Data32And = 0xFFFFFFFE;
    Data32Or  = 0x00000001;
    MmioAndThenOr32 (
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_STRD),
      Data32And,
      Data32Or
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint8,
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_SIRI),
      1,
      (VOID *) (UINTN) (PciD17F4RegBase + R_PCH_SATA_SIRI)
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_STRD),
      1,
      (VOID *) (UINTN) (PciD17F4RegBase + R_PCH_SATA_STRD)
      );
    ///
    /// Step 5
    /// Set D17:F4 + SIR Index 60h[1] = 1b
    ///
    
    MmioWrite8 (PciD17F4RegBase + R_PCH_SATA_SIRI, 0x60);
    Data32And = 0xFFFFFFFD;
    Data32Or  = 0x00000002;
    MmioAndThenOr32 (
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_STRD),
      Data32And,
      Data32Or
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint8,
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_SIRI),
      1,
      (VOID *) (UINTN) (PciD17F4RegBase + R_PCH_SATA_SIRI)
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_STRD),
      1,
      (VOID *) (UINTN) (PciD17F4RegBase + R_PCH_SATA_STRD)
      );

  DEBUG ((DEBUG_INFO, "ConfiguresSata() End\n"));

  return Status;
}

/**
  Program the Max speed suported in each ports while in IDE mode.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance.
  @param[in] PciDevFuncRegBase    Pci B/D/F/Reg Base for the Sata controler.
  @param[in] MaxPorts             Max Sata ports supported in the Controller.  

  @retval EFI_SUCESS              The function exited sucessfully
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources are available
**/
EFI_STATUS
ConfiguresSataSpeedIde (
  DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  UINTN                               PciDevFuncRegBase,
  UINTN                               MaxPorts
  )
{
  EFI_STATUS            Status;
  PCH_SATA_CONFIG       *sSataConfig;
  EFI_PHYSICAL_ADDRESS  IoBaseAddress;
  UINT16                SidpBa;
  UINT16                SidpBaSaveRestore;
  UINT16                DevCmdSaveRestore;
  UINT8                 Data8;
  UINT16                Data16;
  UINTN                 PortIndex;

  Data16      = 0;
  Data8       = 0;

  sSataConfig  = PchPlatformPolicy->sSataConfig;
  // AptioV server override start: Allocating IDE BAR based on IIO UDS CPU0 IO ranges
  /*Status = gDS->AllocateIoSpace (
                  EfiGcdAllocateAnySearchBottomUp,
                  EfiGcdIoTypeIo,
                  N_PCH_SATA_SIDP_BAR_ALIGNMENT,
                  V_PCH_SATA_SIDP_BAR_LENGTH,
                  &IoBaseAddress,
                  mImageHandle,
                  NULL
                  );*/
  
  IoBaseAddress = mIohUds->IioUdsPtr->PlatformData.IIO_resource[0].PciResourceIoLimit;
  
  Status = gDS->AllocateIoSpace (
		  EfiGcdAllocateMaxAddressSearchTopDown,
                  EfiGcdIoTypeIo,
                  N_PCH_SATA_SIDP_BAR_ALIGNMENT,
                  V_PCH_SATA_SIDP_BAR_LENGTH,
                  &IoBaseAddress,
                  mImageHandle,
                  NULL
                  );
  DEBUG ((DEBUG_ERROR, "sSATA IDE base address %lx\n", IoBaseAddress));  
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  // AptioV server override end: Allocating IDE BAR based on IIO UDS CPU0 IO ranges  
  ///
  /// Set the SIDP BAR
  ///
  SidpBa = (UINT16) IoBaseAddress;
  SidpBaSaveRestore = MmioRead16 (PciDevFuncRegBase + R_PCH_SATA_SIDP_BAR);
  MmioWrite16 (PciDevFuncRegBase + R_PCH_SATA_SIDP_BAR, SidpBa);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint16,
    (UINTN) (PciDevFuncRegBase + R_PCH_SATA_SIDP_BAR),
    1,
    &SidpBa
    );
  ///
  /// Enable command register I/O space decoding
  ///
  DevCmdSaveRestore = MmioRead16 (PciDevFuncRegBase + R_PCH_SATA_COMMAND);
  MmioOr16 ((UINTN) (PciDevFuncRegBase + R_PCH_SATA_COMMAND), (UINT16) B_PCH_SATA_COMMAND_IOSE);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint16,
    (UINTN) (PciDevFuncRegBase + R_PCH_SATA_COMMAND),
    1,
    (VOID *) (UINTN) (PciDevFuncRegBase + R_PCH_SATA_COMMAND)
    );
  ///
  /// Configure for the max speed support 1.5Gb/s, 3.0Gb/s and 6.0Gb/s.
  ///
  for (PortIndex = 0; PortIndex < MaxPorts; PortIndex++) {
    switch (PortIndex) {
    case 0:
      Data16 = V_PCH_SATA_AHCI_SINDX_RIDX_SCTL | V_PCH_SATA_AHCI_SINDX_PIDX_PORT0;
      break;

    case 1:
      Data16 = V_PCH_SATA_AHCI_SINDX_RIDX_SCTL | V_PCH_SATA_AHCI_SINDX_PIDX_PORT1;
      break;

    case 2:
      Data16 = V_PCH_SATA_AHCI_SINDX_RIDX_SCTL | V_PCH_SATA_AHCI_SINDX_PIDX_PORT2;
      break;

    case 3:
      Data16 = V_PCH_SATA_AHCI_SINDX_RIDX_SCTL | V_PCH_SATA_AHCI_SINDX_PIDX_PORT3;
      break;
    }

    IoWrite16 ((UINTN) (SidpBa + R_PCH_SATA_SIDPBA_SINDX), Data16);
    S3BootScriptSaveIoWrite (
      S3BootScriptWidthUint16,
      (UINTN) (SidpBa + R_PCH_SATA_SIDPBA_SINDX),
      1,
      &Data16
      );

    switch (sSataConfig->SpeedSupport) {
    case PchSataSpeedSupportGen1:
      Data8 = V_PCH_SATA_SIDPBA_SDATA_GEN1;
      break;

    case PchSataSpeedSupportGen2:
      Data8 = V_PCH_SATA_SIDPBA_SDATA_GEN2;
      break;

    case PchSataSpeedSupportGen3:
      Data8 = V_PCH_SATA_SIDPBA_SDATA_GEN3;
      break;
    }

    IoWrite8 ((UINTN) (SidpBa + R_PCH_SATA_SIDPBA_SDATA), Data8);
    S3BootScriptSaveIoWrite (
      S3BootScriptWidthUint8,
      (UINTN) (SidpBa + R_PCH_SATA_SIDPBA_SDATA),
      1,
      &Data8
      );
    Data16 = MmioRead16 ((UINTN) (PciDevFuncRegBase + R_PCH_SATA_PCS)) >> 8;
    if(((Data16 >> PortIndex) & BIT0)){
      Data8 = (IoRead8 (SidpBa + R_PCH_SATA_SIDPBA_SDATA) & ~B_PCH_SATA_SIDPBA_SCTL_DET) + V_PCH_SATA_SIDPBA_SCTL_DET_COMRST;
      IoWrite8 ((UINTN) (SidpBa + R_PCH_SATA_SIDPBA_SDATA), Data8);
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint8,
        (UINTN) (SidpBa + R_PCH_SATA_SIDPBA_SDATA),
        1,
        &Data8
      );
      PchPmTimerStall (1000);
      PmTimerStallS3Item (1000);
      Data8 = (IoRead8 (SidpBa + R_PCH_SATA_SIDPBA_SDATA) & ~B_PCH_SATA_SIDPBA_SCTL_DET) + V_PCH_SATA_SIDPBA_SCTL_DET_NOINT;
      IoWrite8 ((UINTN) (SidpBa + R_PCH_SATA_SIDPBA_SDATA), Data8);
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint8,
        (UINTN) (SidpBa + R_PCH_SATA_SIDPBA_SDATA),
        1,
        &Data8
      );
    }
  }
  ///
  /// Restore command register I/O space decoding
  ///
  MmioWrite16 (PciDevFuncRegBase + R_PCH_SATA_COMMAND, DevCmdSaveRestore);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint16,
    (UINTN) (PciDevFuncRegBase + R_PCH_SATA_COMMAND),
    1,
    &DevCmdSaveRestore
    );
  ///
  /// Restore the SIDP BAR
  ///
  MmioWrite16 (PciDevFuncRegBase + R_PCH_SATA_SIDP_BAR, SidpBaSaveRestore);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint16,
    (UINTN) (PciDevFuncRegBase + R_PCH_SATA_SIDP_BAR),
    1,
    &SidpBaSaveRestore
    );
  ///
  /// Free allocated resources
  ///
  gDS->FreeIoSpace (IoBaseAddress, (UINT64) V_PCH_SATA_SIDP_BAR_LENGTH);

  return Status;
}

/**
  Program AHCI PI register for Enabled ports
  Handle hotplug, and interlock switch setting,
  and config related GPIOs.

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] RootComplexBar       RootComplexBar value of this PCH device
  @param[in] GpioBase             GPIO base address of this PCH device

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfiguresSataAhci (
  DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  UINT32                              RootComplexBar,
  UINT16                              GpioBase
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  MemBaseAddress;
  UINT32                AhciBar;
  UINT32                CapRegister;
  UINT32                Data32And;
  UINT32                Data32Or;
  UINT32                DwordReg;
  UINT8                 ByteReg; 
  UINT32                PxCMDRegister;
  UINT16                sSataPortsEnabled;
  UINT8                 Index;
  UINTN                 PciD17F4RegBase;
  UINT16                WordReg;
  PCH_SATA_CONFIG       *sSataConfig;
  UINT16                sSataModeSelect;
  UINT32                BaSaveRestore;
  UINT16                DevCmdSaveRestore;
  
  sSataConfig      = PchPlatformPolicy->sSataConfig;
  PciD17F4RegBase = MmPciAddress (0, 
                                  PchPlatformPolicy->BusNumber, 
                                  PCI_DEVICE_NUMBER_EVA,
                                  PCI_FUNCTION_NUMBER_EVA_SSATA,
                                  0);
  sSataModeSelect  = MmioRead16 (PciD17F4RegBase + R_PCH_SATA_MAP) & B_PCH_SATA_MAP_SMS_MASK;
  DwordReg         = 0; 

  // AptioV server override start: For AHCI  BAR allocation based on MMIOL ranges    
   /* Commenting out code that does not follow allocation based upon CPU0 MMIOL ranges  
  ///
  /// Allocate the AHCI BAR
  ///
  MemBaseAddress = 0x0ffffffff;
  Status = gDS->AllocateMemorySpace (
                  EfiGcdAllocateMaxAddressSearchBottomUp,
                  EfiGcdMemoryTypeMemoryMappedIo,
                  N_PCH_SATA_AHCI_BAR_ALIGNMENT,  /// 2^11: 2K Alignment
                  V_PCH_SATA_AHCI_BAR_LENGTH,     /// 2K Length
                  &MemBaseAddress,
                  mImageHandle,
                  NULL
                  );*/
  
  MemBaseAddress = mIohUds->IioUdsPtr->PlatformData.IIO_resource[0].PciResourceMem32Limit;

  Status = gDS->AllocateMemorySpace(
		  EfiGcdAllocateMaxAddressSearchTopDown,
                  EfiGcdMemoryTypeMemoryMappedIo,
                  N_PCH_SATA_AHCI_BAR_ALIGNMENT,  // 2^11: 2K Alignment
                  V_PCH_SATA_AHCI_BAR_LENGTH,     // 2K Length
                  &MemBaseAddress,
                  mImageHandle,
                  NULL
                  );      
  // AptioV server override end: For AHCI BAR allocation based on MMIOL ranges    
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  BaSaveRestore = MmioRead32(PciD17F4RegBase + R_PCH_SATA_AHCI_BAR);
  ///
  /// Set the AHCI BAR
  ///
  AhciBar = (UINT32) MemBaseAddress;
  MmioWrite32 (PciD17F4RegBase + R_PCH_SATA_AHCI_BAR, AhciBar);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) (PciD17F4RegBase + R_PCH_SATA_AHCI_BAR),
    1,
    &AhciBar
    );

  DevCmdSaveRestore = MmioRead16 (PciD17F4RegBase + R_PCH_SATA_COMMAND);
  ///
  /// Enable command register memory space decoding
  ///
  MmioOr16 ((UINTN) (PciD17F4RegBase + R_PCH_SATA_COMMAND), (UINT16) B_PCH_SATA_COMMAND_MSE);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint16,
    (UINTN) (PciD17F4RegBase + R_PCH_SATA_COMMAND),
    1,
    (VOID *) (UINTN) (PciD17F4RegBase + R_PCH_SATA_COMMAND)
    );

  ///
  /// Assert if the memory data of AhciBar is invalid.
  ///
  ASSERT (MmioRead32 (AhciBar) != 0xFFFFFFFF);

  ///
  /// Get Port Settings
  ///
  sSataPortsEnabled = MmioRead16 ((UINTN) (PciD17F4RegBase + R_PCH_SATA_PCS));

  sSataPortsEnabled &= (UINT16) ( B_PCH_SATA_PCS_PORT5_EN |
                                  B_PCH_SATA_PCS_PORT4_EN |
                                  B_PCH_SATA_PCS_PORT3_EN |
                                B_PCH_SATA_PCS_PORT2_EN |
                                B_PCH_SATA_PCS_PORT1_EN |
                                B_PCH_SATA_PCS_PORT0_EN );


  ///
  /// Read the default value of the Host Capabilites Register
  /// NOTE: many of the bits in this register are R/WO (Read/Write Once)
  ///
  CapRegister = MmioRead32 (AhciBar + R_PCH_SATA_AHCI_CAP);
  CapRegister &= (UINT32)~(B_PCH_SATA_AHCI_CAP_SIS | B_PCH_SATA_AHCI_CAP_SSS | B_PCH_SATA_AHCI_CAP_SALP | 
                           B_PCH_SATA_AHCI_CAP_PMS | B_PCH_SATA_AHCI_CAP_SSC | B_PCH_SATA_AHCI_CAP_PSC |
                           B_PCH_SATA_AHCI_CAP_SXS);

  for (Index = 0; Index < GetPchMaxsSataPortNum (); Index++) {
    ///
    /// Check PCS.PxE to know if the SATA Port x is disabled.
    ///
    if ((sSataPortsEnabled & (B_PCH_SATA_PCS_PORT0_EN << Index)) == 0) {
      continue;
    }

    if (sSataConfig->PortSettings[Index].InterlockSw == PCH_DEVICE_ENABLE) {
      ///
      /// Mechanical Presence Switch is Enabled for at least one of the ports
      ///
      CapRegister |= B_PCH_SATA_AHCI_CAP_SIS;
    }

    if ((sSataConfig->PortSettings[Index].SpinUp == PCH_DEVICE_ENABLE) ||
        (sSataConfig->PortSettings[Index].External == PCH_DEVICE_ENABLE)) {
      ///
      /// PCH BIOS Spec Rev 0.5.0, Section 14.1.4 Initialize Registers in AHCI Memory-Mapped Space
      /// Step 1.4
      /// Set SSS (ABAR + 00h[27]) to enable SATA controller supports staggered
      /// spin-up on its ports, for use in balancing power spikes
      /// SATA Port Spin up is supported at least one of the ports
      /// If this is an extra eSATA port. It needs to be hotpluggable but it's usually empty anyway
      /// so LPM is not available but Listen Mode is available, so it will have good power management.
      ///
      CapRegister |= B_PCH_SATA_AHCI_CAP_SSS;
    }

    if (sSataConfig->PortSettings[Index].External == PCH_DEVICE_ENABLE) {
      ///
      /// External SATA is supported at least one of the ports
      ///
      CapRegister |= B_PCH_SATA_AHCI_CAP_SXS;
    }
  }
  ///
  /// Enable Enabled SATA ports,
  /// If BIOS accesses any of the port specific AHCI address range before setting PI bit,
  /// BIOS is required to read the PI register before the initial write to the PI register.
  /// NOTE: many of the bits in this register are R/WO (Read/Write Once)
  ///
  Data32And = (UINT32) (~B_PCH_LP_SATA_PORT_MASK);
  Data32Or  = (UINT32) (sSataPortsEnabled);
  MmioAndThenOr32 (
    (UINTN) (AhciBar + R_PCH_SATA_AHCI_PI),
    Data32And,
    Data32Or
    );
  S3BootScriptSaveMemReadWrite (
    S3BootScriptWidthUint32,
    (UINTN) (AhciBar + R_PCH_SATA_AHCI_PI),
    &Data32Or,  /// Data to be ORed
    &Data32And  /// Data to be ANDed
    );
  ///
  /// After BIOS issues initial write to this register, BIOS is requested to issue two
  /// reads to this register.
  ///
  MmioRead32 (AhciBar + R_PCH_SATA_AHCI_PI);
  Data32And = 0xFFFFFFFF;
  Data32Or = 0;
  S3BootScriptSaveMemReadWrite (
    S3BootScriptWidthUint32,
    (UINTN) (AhciBar + R_PCH_SATA_AHCI_PI),
    &Data32Or,  // Data to be ORed
    &Data32Or  // Data to be ANDed
    );
  Data32And = 0xFFFFFFFF;
  Data32Or = 0;
  S3BootScriptSaveMemReadWrite (
    S3BootScriptWidthUint32,
    (UINTN) (AhciBar + R_PCH_SATA_AHCI_PI),
    &Data32Or, // Data to be ORed
    &Data32Or // Data to be ANDed
    );

  ///
  /// PCH BIOS Spec Rev 0.5.0, Section 14.1.4 Initialize Registers in AHCI Memory-Mapped Space
  /// Step 1
  /// For Lynx Point
  /// Set PSC (ABAR + 00h[13]). This bit informs the Windows software driver that the AHCI
  /// Controller supports the partial low-power state.
  /// Set SSC (ABAR + 00h[14]). This bit informs the Windows software driver that the AHCI
  /// Controller supports the slumber low-power state.
  /// Set SALP (ABAR + 00h[26]) to enable Aggressive Link Power Management (LPM) support.
  ///
  if (sSataConfig->LpmSupport == PCH_DEVICE_ENABLE)
  {
    //Enable LPM
  CapRegister |= (B_PCH_SATA_AHCI_CAP_SSC | B_PCH_SATA_AHCI_CAP_PSC);

  if (sSataConfig->SalpSupport == PCH_DEVICE_ENABLE) {
      //Enable SALP
      CapRegister |= B_PCH_SATA_AHCI_CAP_SALP;
    }
    else
    {
      //Disable SALP
      CapRegister &= ~B_PCH_SATA_AHCI_CAP_SALP;
    }
  }
  else
  {
    //Disable LPM
    CapRegister &= ~(B_PCH_SATA_AHCI_CAP_SSC | B_PCH_SATA_AHCI_CAP_PSC);
    //Disable SALP
    CapRegister &= ~B_PCH_SATA_AHCI_CAP_SALP;
  }

  ///
  /// Support Command List Override & PIO Multiple DRQ Block
  ///
  CapRegister |= (B_PCH_SATA_AHCI_CAP_SCLO | B_PCH_SATA_AHCI_CAP_PMD);

  ///
  /// Configure for the max speed support 1.5Gb/s, 3.0Gb/s and 6.0Gb/s.
  ///
  CapRegister &= ~B_PCH_SATA_AHCI_CAP_ISS_MASK;

  switch (sSataConfig->SpeedSupport) {
  case PchSataSpeedSupportGen1:
    CapRegister |= (V_PCH_SATA_AHCI_CAP_ISS_1_5_G << N_PCH_SATA_AHCI_CAP_ISS);
    break;

  case PchSataSpeedSupportGen2:
    CapRegister |= (V_PCH_SATA_AHCI_CAP_ISS_3_0_G << N_PCH_SATA_AHCI_CAP_ISS);
    break;

  case PchSataSpeedSupportGen3:
    CapRegister |= (V_PCH_SATA_AHCI_CAP_ISS_6_0_G << N_PCH_SATA_AHCI_CAP_ISS);
    break;
  }

  ///
  /// Update the Host Capabilites Register and save for S3 resume
  /// NOTE: Many of the bits in this register are R/WO (Read/Write Once)
  ///
  MmioWrite32 (AhciBar + R_PCH_SATA_AHCI_CAP, CapRegister);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) (AhciBar + R_PCH_SATA_AHCI_CAP),
    1,
    &CapRegister
    );

  ///
  /// Port[Max:0] Command Register update
  /// NOTE: this register must be updated after Port Implemented and Capabilities register,
  /// Many of the bits in this register are R/WO (Read/Write Once)
  ///
  for (Index = 0; Index < GetPchMaxsSataPortNum (); Index++) {
    ///
    /// Check PCS.PxE to know if the SATA Port x is disabled.
    ///
    if ((sSataPortsEnabled & (B_PCH_SATA_PCS_PORT0_EN << Index)) == 0) {
      continue;
    }
    ///
    /// Initial to zero first
    ///
    PxCMDRegister = 0;

    if (sSataConfig->PortSettings[Index].HotPlug == PCH_DEVICE_ENABLE) {
      if (sSataConfig->PortSettings[Index].External == PCH_DEVICE_DISABLE) {
        ///
        /// Hot Plug of this port is enabled
        ///
        PxCMDRegister |= B_PCH_SATA_AHCI_PxCMD_HPCP;
        if (sSataConfig->PortSettings[Index].InterlockSw == PCH_DEVICE_ENABLE) {
          ///
          /// Mechanical Switch of this port is Attached
          ///
          PxCMDRegister |= B_PCH_SATA_AHCI_PxCMD_MPSP;
        }
      }
    } else {
      ///
      /// When "Mechanical Switch Attached to Port" (PxCMD[19]) is set, it is expected that HPCP (PxCMD[18]) is also set.
      ///
      if (sSataConfig->PortSettings[Index].InterlockSw == PCH_DEVICE_ENABLE) {
        DEBUG ((DEBUG_ERROR, "Hot-Plug function of Port%d should be enabled while the Inter Lock Switch of it is enabled!\n",
        Index));
      }
    }

    if (sSataConfig->PortSettings[Index].External == PCH_DEVICE_ENABLE) {
      PxCMDRegister |= B_PCH_SATA_AHCI_PxCMD_ESP;
    }

    if (sSataConfig->PortSettings[Index].SpinUp == PCH_DEVICE_ENABLE) {
      PxCMDRegister |= B_PCH_SATA_AHCI_PxCMD_SUD;
    }

    ///
    /// eSATA port support only up to Gen2
    ///
    if (sSataConfig->PortSettings[Index].External == PCH_DEVICE_ENABLE) {
      MmioAndThenOr32 (
        (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index))),
        (UINT32)~(B_PCH_SATA_AHCI_PXSCTL_SPD),
        (UINT32) V_PCH_SATA_AHCI_PXSCTL_SPD_2
        );
      DwordReg = MmioRead32 (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index)));
      S3BootScriptSaveMemWrite(      
        S3BootScriptWidthUint32,
        (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index))),
        1,
        &DwordReg
        );
    }

    MmioAndThenOr32 (
      (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0CMD + (0x80 * Index))),
      (UINT32)~(B_PCH_SATA_AHCI_PxCMD_MASK),
      (UINT32) PxCMDRegister
      );
    DwordReg = MmioRead32 (AhciBar + (R_PCH_SATA_AHCI_P0CMD + (0x80 * Index)));
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0CMD + (0x80 * Index))),
      1,
      &DwordReg
      );
    ///
    /// eSATA port support only up to Gen2.
    /// Save and restore Port Speed limitation
    ///
    if (sSataConfig->PortSettings[Index].External == PCH_DEVICE_ENABLE) {
      ByteReg = MmioRead8 (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index)));
      ByteReg &= (UINT8) ~(B_PCH_SATA_AHCI_PXSCTL_SPD);
      ByteReg |= (UINT8) V_PCH_SATA_AHCI_PXSCTL_SPD_2;
      S3BootScriptSaveMemWrite (
         S3BootScriptWidthUint8,
        (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index))),
        1,
        &ByteReg
        );
    }

  }
  ///
  /// PCH BIOS Spec Rev 0.5.0, Section 14.1.5.2 Enable Flexible RAID OROM Features
  /// Lynx Point with RAID capable sku is able to customize the RAID features through setting the
  /// Intel RST Feature Capabilities (RSTF) register before loading the RAID Option ROM. The RAID
  /// OROM will enable the desired features based on the setting in that register, please refer to
  /// PCH EDS for more details.
  /// For example, if the platform desired features are RAID0, RAID1, RAID5, RAID10 and
  /// RST Smart Storage caching. System BIOS should set RSTF (ABAR + C8h [15:0]) to 022Fh before
  /// loading RAID OROM.
  ///
  WordReg = 0;

  ///
  /// If Software Feature Mask Supported (ABAR + A0h bit6) is set to "1", the platform is enabled for storage
  /// feature selection as described in VS MMIO space at offset ABAR + C8h.
  ///
  
  if (sSataConfig->HddUnlock == PCH_DEVICE_ENABLE) {
    ///
    /// If set to "1", indicates that the HDD password unlock in the OS is enabled
    /// while SATA is configured as RAID mode.
    ///
    WordReg |= B_PCH_SATA_AHCI_RSTF_HDDLK;
  }

  if (sSataConfig->LedLocate == PCH_DEVICE_ENABLE) {
    ///
    /// If set to "1", indicates that the LED/SGPIO hardware is attached and ping to
    /// locate feature is enabled on the OS while SATA is configured as RAID mode.
    ///
    WordReg |= B_PCH_SATA_AHCI_RSTF_LEDL;
  }

  if (sSataModeSelect == V_PCH_SATA_MAP_SMS_RAID) {
    if (sSataConfig->Raid0 == PCH_DEVICE_ENABLE) {
      ///
      /// If set to "1", then RAID0 is enabled in Flexible RAID Option ROM.
      ///
      WordReg |= B_PCH_SATA_AHCI_RSTF_R0E;
    }

    if (sSataConfig->Raid1 == PCH_DEVICE_ENABLE) {
      ///
      /// If set to "1", then RAID1 is enabled in FD-OROM.
      ///
      WordReg |= B_PCH_SATA_AHCI_RSTF_R1E;
    }

    if (sSataConfig->Raid10 == PCH_DEVICE_ENABLE) {
      ///
      /// If set to "1", then RAID10 is enabled in FD-OROM.
      ///
      WordReg |= B_PCH_SATA_AHCI_RSTF_R10E;
    }

    if (sSataConfig->Raid5 == PCH_DEVICE_ENABLE) {
      ///
      /// If set to "1", then RAID5 is enabled in FD-OROM.
      ///
      WordReg |= B_PCH_SATA_AHCI_RSTF_R5E;
    }

    if (sSataConfig->Irrt == PCH_DEVICE_ENABLE) {
      ///
      /// If set to "1", then Intel Rapid Recovery Technology is enabled.
      ///
      WordReg |= B_PCH_SATA_AHCI_RSTF_RSTE;
    }

    if (sSataConfig->OromUiBanner == PCH_DEVICE_ENABLE) {
      ///
      /// If set to "1" then the OROM UI is shown.  Otherwise, no OROM banner or information
      /// will be displayed if all disks and RAID volumes are Normal.
      ///
      WordReg |= B_PCH_SATA_AHCI_RSTF_IRSTOROM;
    }

    if (sSataConfig->IrrtOnly == PCH_DEVICE_ENABLE) {
      ///
      /// If set to "1", then only IRRT volumes can span internal and eSATA drives. If cleared
      /// to "0", then any RAID volume can span internal and eSATA drives.
      ///
      WordReg |= B_PCH_SATA_AHCI_RSTF_IROES;
    }
    ///
    /// Enable the RST Smart Storage caching bit to support Smart Storage caching.
    ///
    if (sSataConfig->SmartStorage == PCH_DEVICE_ENABLE) {
      WordReg |= B_PCH_SATA_AHCI_RSTF_SEREQ;
    }
    ///
    /// Program the delay of the OROM UI Splash Screen in a normal status.
    ///
    WordReg |= (UINT16) (sSataConfig->OromUiDelay << N_PCH_SATA_AHCI_RSTF_OUD);
  }
  ///
  /// RSTF(RST Feature Capabilities) is a Write-Once register.
  ///
  MmioWrite16 ((UINTN) (AhciBar + R_PCH_SATA_AHCI_RSTF), WordReg);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint16,
    (UINTN) (AhciBar + R_PCH_SATA_AHCI_RSTF),
    1,
    &WordReg
    );

  ///
  /// Restore command register I/O space decoding
  ///
  MmioWrite16 (PciD17F4RegBase + R_PCH_SATA_COMMAND, DevCmdSaveRestore);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint16,
    (UINTN) (PciD17F4RegBase + R_PCH_SATA_COMMAND),
    1,
    &DevCmdSaveRestore
    );

  ///
  /// Restore Ahci Bar
  ///
  MmioWrite32 (PciD17F4RegBase + R_PCH_SATA_AHCI_BAR, BaSaveRestore);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) (PciD17F4RegBase + R_PCH_SATA_AHCI_BAR),
    1,
    &BaSaveRestore
    );

  ///
  /// Free the GCD pool
  ///
  gDS->FreeMemorySpace (
        MemBaseAddress,
        V_PCH_SATA_AHCI_BAR_LENGTH
        );

  return EFI_SUCCESS;
}

/**
   Configures EVA devices during PCH initialization phase. 

  @param[in]  PchPlatformPolicy, to get PchBus. 
  
  @retval EFI_SUCCESS
**/

EFI_STATUS
ConfigureEva (
   DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy
)
{   

   DEBUG(((DEBUG_INFO),"Entering PchEva initialization ...\n"));
   
   DEBUG(((DEBUG_INFO),"Complete PchEva initialization!!!\n"));   
   return EFI_SUCCESS;
}

#endif //PCH_SERVER_BIOS_FLAG
