/** @file
  Configures PCH Sata Controller

@copyright
  Copyright (c) 2008 - 2013 Intel Corporation. All rights reserved
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
#include "PchInit.h"

EFI_IIO_UDS_PROTOCOL  		*mIohUds; // AptioV server override: For SATA BAR allocation based on MMIO/IO ranges  

EFI_STATUS
ConfigureSataAhci (
  DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  UINT32                              RootComplexBar,
  UINT16                              GpioBase
  );

EFI_STATUS
ConfigureSataSpeedIde (
  DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  UINTN                               PciDevFuncRegBase,
  UINTN                               MaxPorts
  );
/**
  Set a "Poll Status" S3 dispatch item

   @param[in] MmioAddress     - Address
   @param[in] Width           - Operation Width
   @param[in] Mask            - Mask
   @param[in] Value           - Value to wait for

   @retval    EFI_STATUS

**/
EFI_STATUS
SetPollStatusS3Item (
  IN   UINT64                    MmioAddress,
  IN   EFI_BOOT_SCRIPT_WIDTH     Width,
  IN   UINT64                    Mask,
  IN   UINT64                    Value,
  IN   UINT32                    TimeOut
  )
{
    //
  // Just call the built in S3 poll support.
    //
  S3BootScriptSaveMemPoll (
    Width,
    MmioAddress,
    &Mask,
    &Value,
    EFI_PCH_S3_STALL_INTERVAL,
    TimeOut/EFI_PCH_S3_STALL_INTERVAL
                    );
  return EFI_SUCCESS;

  }



/**
  Set a "PmTimer Stall" S3 dispatch item

  @param[i] DelayTime         The request number of microseconds
  @retval   EFI_SUCCESS       The function completed successfully

**/
EFI_STATUS
PmTimerStallS3Item (
  IN   UINT32                    DelayTime
  )
{
  S3BootScriptSaveStall(DelayTime);
  return EFI_SUCCESS;
    }


EFI_STATUS
DisableSataController (
  IN      DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  IN OUT  UINT32                              *FuncDisableReg
  );

/**
  Configures PCH Sata Controller

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in] RootComplexBar       RootComplexBar value of this PCH device
  @param[in, out] FuncDisableReg  Function Disable Register
  @param[in] GpioBase             GPIO base address of this PCH device

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ConfigureSata (
  IN      DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  IN      UINT32                              RootComplexBar,
  IN OUT  UINT32                              *FuncDisableReg,
  IN      UINT16                              GpioBase
  )
{
  EFI_STATUS      Status;
  UINT32          Index;
  UINT16          WordReg;
  UINT16          SataGcReg;
  UINTN           PciD31F2RegBase;
  PCH_SATA_CONFIG *SataConfig;
  UINT16          SataPortsEnabled;
  UINT16          SataModeSelect;
  UINTN           PciD31F5RegBase;
  UINTN           PciDevFuncRegBase;
  UINTN           MaxPorts;
  UINT32          Data32And;
  UINT32          Data32Or;
  PCH_SERIES      PchSeries;
  UINTN           PciD31F0RegBase;
  UINT16          LpcDeviceId;
  UINTN           DummyHandle;
  UINT32          Data32;
  UINT8           Data8;

  DEBUG ((DEBUG_INFO, "ConfigureSata() Start\n"));

  Data32          = 0x0;
  Data8           = 0x0;

  PchSeries       = GetPchSeries();
  SataConfig      = PchPlatformPolicy->SataConfig;
  PciD31F2RegBase = MmPciAddress (0, PchPlatformPolicy->BusNumber, 31, 2, 0);
  PciD31F5RegBase = 0;
  if (PchSeries == PchH) {
    PciD31F5RegBase = MmPciAddress (0, PchPlatformPolicy->BusNumber, 31, 5, 0);
  }
  SataModeSelect  = MmioRead16 (PciD31F2RegBase + R_PCH_SATA_MAP) & B_PCH_SATA_MAP_SMS_MASK;

  PciD31F0RegBase = MmPciAddress (0, PchPlatformPolicy->BusNumber, 31, 0, 0);
  LpcDeviceId     = MmioRead16 (PciD31F0RegBase + R_PCH_LPC_DEVICE_ID);

  Status          = EFI_SUCCESS;

  ///
  /// Check to disable SATA controller
  ///
  if (PchPlatformPolicy->DeviceEnabling->Sata == PCH_DEVICE_DISABLE) {
    Status = DisableSataController (PchPlatformPolicy, FuncDisableReg);
    return Status;
  }

  ///
  /// PCH BIOS Spec Rev 0.5.0 section 14.1.7 Additional Programming Requirements during
  /// SATA Initialization
  /// Step 12
  /// Program D31:F2:9Ch[5] to 1b (Note: this must be programmed together with D31:F2:9Ch[7:6]
  /// in word write)
  ///
  SataGcReg = MmioRead16 (PciD31F2RegBase + R_PCH_SATA_SCLKGC);
  SataGcReg |= BIT5;

  switch (SataModeSelect) {
#ifdef TRAD_FLAG
    case V_PCH_SATA_MAP_SMS_IDE:
      if (PchSeries == PchH) {
        ///
        /// Set Native IDE decoding
        ///
        /// PCH BIOS Spec Rev 0.5.0 Section 14.1.3 Set the Programming Interface
        /// Using native IDE is only possible when the SATA controller is operating in IDE mode.
        /// The programming interface is selected by setting the programming interface register
        /// (PI = Reg: 09h) appropriately.  There are two native mode enabling bits in the
        /// PI register to control the primary and secondary channels of SATA1.
        ///
        /// Only D31:F2 needs to be set. D31:F5 is readonly
        ///
        if (SataConfig->LegacyMode == PCH_DEVICE_ENABLE) {
          MmioAnd8 (
            PciD31F2RegBase + R_PCH_SATA_PI_REGISTER,
            (UINT8)~(B_PCH_SATA_PI_REGISTER_PNE | B_PCH_SATA_PI_REGISTER_SNE)
            );
        } else {
          MmioOr8 (
            PciD31F2RegBase + R_PCH_SATA_PI_REGISTER,
            (UINT8) B_PCH_SATA_PI_REGISTER_PNE | B_PCH_SATA_PI_REGISTER_SNE
            );
        }
        S3BootScriptSaveMemWrite (
          S3BootScriptWidthUint8,
          (UINTN) (PciD31F2RegBase + R_PCH_SATA_PI_REGISTER),
          1,
          (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_PI_REGISTER)
          );
        MmioOr16 ((UINTN) (PciD31F5RegBase + R_PCH_SATA_TIMP), (UINT16) (B_PCH_SATA_TIM_IDE));
        WordReg = MmioRead16 (PciD31F5RegBase + R_PCH_SATA_TIMP);
         S3BootScriptSaveMemWrite (
          S3BootScriptWidthUint16,
          (UINTN) (PciD31F5RegBase + R_PCH_SATA_TIMP),
          1,
          &WordReg
          );
        MmioOr16 ((UINTN) (PciD31F5RegBase + R_PCH_SATA_TIMS), (UINT16) (B_PCH_SATA_TIM_IDE));
        WordReg = MmioRead16 (PciD31F5RegBase + R_PCH_SATA_TIMS);
        S3BootScriptSaveMemWrite (
          S3BootScriptWidthUint16,
          (UINTN) (PciD31F5RegBase + R_PCH_SATA_TIMS),
          1,
          &WordReg
          );
      }
    break;
#endif // TRAD_FLAG

    case V_PCH_SATA_MAP_SMS_RAID:
      if (IS_PCH_LPT_LPC_DEVICE_ID_SERVER (LpcDeviceId) 
#ifdef PCH_SERVER_BIOS_FLAG       
       || IS_PCH_WBG_LPC_DEVICE_ID (LpcDeviceId)
#endif //SERVER_BIOS_FLAG       
      ) {

#ifdef PCH_SERVER_BIOS_FLAG
 if ( PchPlatformPolicy->RaidDevice->DeviceId == PchRaidRST )
        {
          SataGcReg &= ~B_PCH_SATA_SCLKGC_AIE;
          SataGcReg &= ~B_PCH_SATA_SCLKGC_AIES;
        } else {
          SataGcReg &= ~B_PCH_SATA_SCLKGC_AIE;
          SataGcReg |= B_PCH_SATA_SCLKGC_AIES;
        }
#else

        SataGcReg &= ~B_PCH_SATA_SCLKGC_AIE;
        SataGcReg |= B_PCH_SATA_SCLKGC_AIES;
#endif
      }      
      ///
      /// When RAID alternate ID is enabled, the Device ID will change to 30XX from 282X in RAID mode
      ///
      if (SataConfig->RaidAlternateId == PCH_DEVICE_ENABLE) {
        SataGcReg &= ~B_PCH_SATA_SCLKGC_AIES;
        SataGcReg |= B_PCH_SATA_SCLKGC_AIE;
      }

      if (SataConfig->SataEfiRaid == 1){
        Status = gBS->LocateProtocol(&gPchSataEfiLoadProtocolGuid, NULL, (VOID **)&DummyHandle ); 
        if (EFI_ERROR(Status)) {
           Status = gBS->InstallProtocolInterface (
                         &mImageHandle, 
                         &gPchSataEfiLoadProtocolGuid,
                         EFI_NATIVE_INTERFACE,
                         NULL
                        );
           if (EFI_ERROR(Status)){
              DEBUG ((EFI_D_ERROR,"Sata RSTe EFI OpRom NOT installed = %x!!!\n", Status));
           } else {
              DEBUG ((EFI_D_ERROR,"Sata RSTe EFI OpRom install successfully = %x!\n",Status));
           }
         }
      }


      break;
  }
  ///
  /// Unconditional write is necessary to lock the register
  ///
  MmioWrite16 (PciD31F2RegBase + R_PCH_SATA_SCLKGC, SataGcReg);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint16,
    (UINTN) (PciD31F2RegBase + R_PCH_SATA_SCLKGC),
    1,
    &SataGcReg
    );

  ///
  /// Set legacy IDE decoding
  /// These bits also effect with AHCI mode when PCH is using legacy mechanisms.
  ///
  MmioOr16 ((UINTN) (PciD31F2RegBase + R_PCH_SATA_TIMP), (UINT16) (B_PCH_SATA_TIM_IDE));
  WordReg = MmioRead16 (PciD31F2RegBase + R_PCH_SATA_TIMP);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint16,
    (UINTN) (PciD31F2RegBase + R_PCH_SATA_TIMP),
    1,
    &WordReg
    );

  MmioOr16 ((UINTN) (PciD31F2RegBase + R_PCH_SATA_TIMS), (UINT16) (B_PCH_SATA_TIM_IDE));
  WordReg = MmioRead16 (PciD31F2RegBase + R_PCH_SATA_TIMS);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint16,
    (UINTN) (PciD31F2RegBase + R_PCH_SATA_TIMS),
    1,
    &WordReg
    );
  ///
  /// PCH BIOS Spec Rev 0.5.0, section 19.10
  /// Step 4.2
  /// D31:F2 PCS: Enable the port in any of below condition:
  /// Hot plug is enabled
  /// A device is attached
  /// Test mode is enabled
  /// Configured as eSATA port
  ///
  SataPortsEnabled  = 0;
  WordReg           = MmioRead16 (PciD31F2RegBase + R_PCH_SATA_PCS);
  for (Index = 0; Index < GetPchMaxSataPortNum (); Index++) {
    if ((SataConfig->PortSettings[Index].HotPlug == PCH_DEVICE_ENABLE) ||
        (WordReg & (BIT0 << (8 + Index))) ||
        (SataConfig->TestMode == PCH_DEVICE_ENABLE) ||
        (SataConfig->PortSettings[Index].External == PCH_DEVICE_ENABLE)) {
      SataPortsEnabled |= (SataConfig->PortSettings[Index].Enable << Index);
    }
  }
//
// SATASGPIO_START
//       
#ifdef SDP_FLAG
  SataPortsEnabled |= 0x3F;
#endif
//
// SATASGPIO_END
//       
  if (PchSeries == PchH) {
    ///
    /// D31:F5 PCS: Enable the port in any of below condition:
    /// Hot plug is enabled
    /// A device is attached
    /// Test mode is enabled
    ///
    /// Note: In IDE mode, SATA Port 4/5 enable status needs to be checked before setting D31:F2 MAP[13:8].
    ///       It's because:
    ///       (1) SataPortsEnabled [5:4] will be set while it is configured as eSATA port that is not
    ///           supported in IDE mode.
    ///       (2) D31:F2 MAP[12] setting needs to sync up with D31:F5 MAP[8] setting.
    ///           D31:F2 MAP[13] setting needs to sync up with D31:F5 MAP[9] setting.
    ///
    if (SataModeSelect == V_PCH_SATA_MAP_SMS_IDE) {
      ///
      /// Clear SataPortsEnabled [5:4] before checking SATA Port 4/5 enable status
      ///
      SataPortsEnabled &= ~(BIT5 | BIT4);
      WordReg           = MmioRead16 (PciD31F5RegBase + R_PCH_SATA_PCS);
      for (Index = 4; Index < GetPchMaxSataPortNum (); Index++) {
        ///
        /// D31:F5 PCS[9:8] = Port 4/5 Present bit
        ///
        if ((SataConfig->PortSettings[Index].HotPlug == PCH_DEVICE_ENABLE) ||
            (WordReg & (BIT0 << (4 + Index))) ||
            (SataConfig->TestMode == PCH_DEVICE_ENABLE)) {
          ///
          /// SataPortsEnabled [5:4] = Sata Port 4/5 enable status
          ///
          SataPortsEnabled |= (SataConfig->PortSettings[Index].Enable << Index);
        }
      }
    }
  }

  ///
  /// Set MAP register for PCH H
  /// Set D31:F2 MAP[13:8] to 1b if SATA Port 0/1/2/3/4/5 is disabled
  /// SataPortsEnabled [5:0] = Sata Port 0/1/2/3/4/5 enable status
  /// MAP.SPD (D31:F2:Reg90h[13:8]) is Write Once
  ///
  /// Set MAP register for PCH LP
  /// Set D31:F2 MAP[11:8] to 1b if SATA Port 0/1/2/3 is disabled
  /// SataPortsEnabled [3:0] = Sata Port 0/1/2/3 enable status
  /// MAP.SPD (D31:F2:Reg90h[11:8]) is Write Once
  ///
  switch (PchSeries) {
    case PchLp:
      MmioOr16 ((UINTN) (PciD31F2RegBase + R_PCH_SATA_MAP), ((~SataPortsEnabled << 8) & (UINT16) B_PCH_LP_SATA_MAP_SPD));
      break;

    case PchH:
      MmioOr16 ((UINTN) (PciD31F2RegBase + R_PCH_SATA_MAP), ((~SataPortsEnabled << 8) & (UINT16) B_PCH_H_SATA_MAP_SPD));
      break;

    default:
      break;
  }
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint16,
    (UINTN) (PciD31F2RegBase + R_PCH_SATA_MAP),
    1,
    (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_MAP)
    );

    //
    // D31:F2 PCS[5:0] = Port 0~5 Enabled bit
    // as per SataPortsEnabled value.
    //
   MmioOr16 ((UINTN) (PciD31F2RegBase + R_PCH_SATA_PCS), (UINT16) (SataPortsEnabled));

   S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint16,
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_PCS),
      1,
      (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_PCS)
      );

#ifdef TRAD_FLAG
  if (PchSeries == PchH) {
    ///
    /// Set D31:F5 MAP[9:8] and D31:F5 PCS[1:0]
    ///
    if (SataModeSelect == V_PCH_SATA_MAP_SMS_IDE) {
      ///
      /// Set D31:F5 MAP[9:8] to 1b if SATA Port 4/5 is disabled
      /// SataPortsEnabled [5:4] = Sata Port 4/5 enable status
      /// MAP.SPD (D31:F5:Reg90h[9:8]) is Write Once
      ///
      MmioOr16 ((UINTN) (PciD31F5RegBase + R_PCH_SATA_MAP), ((~SataPortsEnabled << 4) & (UINT16) B_PCH_SATA2_MAP_SPD));
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint16,
        (UINTN) (PciD31F5RegBase + R_PCH_SATA_MAP),
        1,
        (VOID *) (UINTN) (PciD31F5RegBase + R_PCH_SATA_MAP)
        );
      ///
      /// D31:F5 PCS[1:0] = Port 4/5 Enabled bit
      ///
      MmioAndThenOr16 (
        (UINTN) (PciD31F5RegBase + R_PCH_SATA_PCS),
        (UINT16) (~(B_PCH_SATA2_PCS_PORT5_EN | B_PCH_SATA2_PCS_PORT4_EN)),
        (UINT16) (SataPortsEnabled >> 4)
        );
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint16,
        (UINTN) (PciD31F5RegBase + R_PCH_SATA_PCS),
        1,
        (VOID *) (UINTN) (PciD31F5RegBase + R_PCH_SATA_PCS)
        );
    }
  }
#endif // TRAD_FLAG
  ///
  /// PCH BIOS Spec Rev 0.5.0, section 19.10
  /// Step 4.2
  /// After configuring Port and Control Status (PCS) Register Port x Enabled (PxE) bits accordingly,
  /// wait 1.4 micro second
  ///
  PchPmTimerStall (0x02);
  PmTimerStallS3Item (0x02);

    ///
    /// Moved from PchPm.c - ConfigureAdditionalPm()
    ///
    /// Step 56
    /// Ensure this is done after 3A00-3A3C and before 3A6C
    /// if PchLp, Set RCBA + Offset 3A84h[31:0] = 0x00001005
    /// if PchH,  Set RCBA + Offset 3A84h[31:0] = 0x01041001
    /// if PCS.P0E and PCS.P1E = 0b, Set RCBA + Offset 3A84h[20,18] = 1b, 1b
    /// if PCS.P2E and PCS.P3E = 0b, Set RCBA + Offset 3A84h[24,26] = 1b, 1b
    ///
    if (PchSeries == PchLp) {
      Data32 = 0x00001005;
    } else {
      Data32 = 0x01041001;
    }
    Data8 = MmioRead8 (PciD31F2RegBase + R_PCH_SATA_PCS);
    if((Data8 & (UINT8) (B_PCH_SATA_PCS_PORT0_EN | B_PCH_SATA_PCS_PORT1_EN)) == 0) {
      Data32 |= (BIT20 | BIT18);
      }
    if((Data8 & (UINT8) (B_PCH_SATA_PCS_PORT2_EN | B_PCH_SATA_PCS_PORT3_EN)) == 0) {
      Data32 |= (BIT24 | BIT26);
    }
    MmioWrite32 (
      (UINTN) (RootComplexBar + R_PCH_RCRB_CIR28),
      (UINT32) (Data32)
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (RootComplexBar + R_PCH_RCRB_CIR28),
      1,
      (VOID *) (UINTN) (RootComplexBar + R_PCH_RCRB_CIR28)
      );


  ///
  /// After programming the PCS.PxE, check if it is zero.
  /// If no port enabled, terminate the SATA configuration and disable the SATA controller.
  ///
  WordReg  = MmioRead16 ((UINTN) (PciD31F2RegBase + R_PCH_SATA_PCS));
  WordReg &= (UINT16) ((1 << GetPchMaxSataPortNum ()) - 1);
#ifdef TRAD_FLAG
  if (WordReg == 0) {
    if ((PchSeries == PchH) && (SataModeSelect == V_PCH_SATA_MAP_SMS_IDE)) {
      WordReg  = MmioRead16 ((UINTN) (PciD31F5RegBase + R_PCH_SATA_PCS));
      WordReg &= (UINT16) (B_PCH_SATA2_PCS_PORT5_EN |
                           B_PCH_SATA2_PCS_PORT4_EN);
    }
  }
#endif // TRAD_FLAG
  if (WordReg == 0) {
    Status = DisableSataController (PchPlatformPolicy, FuncDisableReg);
    return Status;
  }

  if (PchSeries == PchLp) {
    ///
    /// If Listen Mode support is not required, program D31:F2:98h[24] to 1b.
    ///
    if ((SataConfig->PortSettings[0].HotPlug == PCH_DEVICE_DISABLE) &&
        (SataConfig->PortSettings[1].HotPlug == PCH_DEVICE_DISABLE) &&
        (SataConfig->PortSettings[2].HotPlug == PCH_DEVICE_DISABLE) &&
        (SataConfig->PortSettings[3].HotPlug == PCH_DEVICE_DISABLE)) {
      MmioOr32 ((UINTN) (PciD31F2RegBase + 0x98), (UINT32) (BIT24));
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (PciD31F2RegBase + 0x98),
        1,
        (VOID *) (UINTN) (PciD31F2RegBase + 0x98)
        );
    }
  }

  // AptioV server override start: Allocating SATA BAR based on IIO UDS CPU0 MMIO/IO ranges
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
  if (PchSeries == PchLp) {
    if (SataModeSelect != V_PCH_SATA_MAP_SMS_LOOBACK_TESTMODE) {
      Status = ConfigureSataAhci (PchPlatformPolicy, RootComplexBar, GpioBase);
    }
  } else if (PchSeries == PchH) {
    if (SataModeSelect != V_PCH_SATA_MAP_SMS_IDE) {
      Status = ConfigureSataAhci (PchPlatformPolicy, RootComplexBar, GpioBase);
    } else {
      ///
      /// If it is IDE mode
      ///
      if(PchPlatformPolicy->SataConfig->SpeedSupport != PchSataSpeedSupportDefault){
        PciDevFuncRegBase = 0;
        MaxPorts          = 0;
        for (Index = 0; Index < GetPchMaxSataControllerNum (); Index++) {
          switch (Index) {
            case 0:
              PciDevFuncRegBase = PciD31F2RegBase;
              MaxPorts          = PCH_IDE_1_MAX_PORTS;
              break;

            case 1:
              PciDevFuncRegBase = PciD31F5RegBase;
              MaxPorts          = PCH_IDE_2_MAX_PORTS;
              break;
          }
          Status = ConfigureSataSpeedIde (PchPlatformPolicy, PciDevFuncRegBase, MaxPorts);
        }
      }
    }
  }
  ///
  /// PCH BIOS Spec Rev 0.5.0, section 19.10
  /// Step 4.1
  /// Set bits D31:F2:Reg 94h[29:24] to 3Fh as part of the chipset initialization and before disabling the
  /// SATA function if the SATA interface is not supported. BIOS can also set PCD bits for the un-routed ports
  /// on the platform to disable clocks for the unused ports
  /// Set the PCD [port x] = 1 if the corresponding PCS.PxE = 0 (either have a device attached or have
  /// hot plug enabled)
  ///
  for (Index = 0; Index < GetPchMaxSataPortNum (); Index++) {
    if ((SataPortsEnabled & (B_PCH_SATA_PCS_PORT0_EN << Index)) == 0) {
      MmioOr32 ((UINTN) (PciD31F2RegBase + R_PCH_SATA_SCLKCG), (UINT32) (B_PCH_SATA_SCLKCG_PORT0_PCD << Index));
    }
  }
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) (PciD31F2RegBase + R_PCH_SATA_SCLKCG),
    1,
    (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_SCLKCG)
    );
  ///
  /// PCH BIOS Spec Rev 0.5.6, 14.1.6 Power Optimizer Configuration
  /// System BIOS must execute the following steps as part of System BIOS initialization
  /// of the PCH SATA controller on both cold boot (G3/S5) and S3/S4 resume path. Please
  /// refer to the PCH EDS, section 14.1.35.1 for the SATA initialization settings and
  /// the actual register indexes/values to be programmed.
  ///
  if (PchPlatformPolicy->PwrOptConfig->PchPwrOptSata == PCH_DEVICE_ENABLE) {
    ///
    /// Step 1
    /// Set D31:F2 + SIR Index 64h[31:0] = 883C9001h
    ///
    MmioWrite8 (PciD31F2RegBase + R_PCH_SATA_SIRI, 0x64);
    if (PchSeries == PchH) {
      MmioWrite32 (PciD31F2RegBase + R_PCH_SATA_STRD, 0x883C9001);
    }
    if (PchSeries == PchLp) {
      MmioWrite32 (PciD31F2RegBase + R_PCH_SATA_STRD, 0x883C9003);
    }
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint8,
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_SIRI),
      1,
      (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_SIRI)
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_STRD),
      1,
      (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_STRD)
      );
    ///
    /// Step 2
    /// Set D31:F2 + SIR Index 68h[15:0] = 880Ah
    ///
    MmioWrite8 (PciD31F2RegBase + R_PCH_SATA_SIRI, 0x68);
    Data32And = 0xFFFF0000;
#ifdef PCH_SERVER_BIOS_FLAG
    Data32Or  = 0x0000882D;
#else
    Data32Or  = 0x0000880A;
#endif
    MmioAndThenOr32 (
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_STRD),
      Data32And,
      Data32Or
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint8,
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_SIRI),
      1,
      (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_SIRI)
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_STRD),
      1,
      (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_STRD)
      );
    ///
    /// Step 3
    /// Set D31:F2 + SIR Index 60h[3] = 1b
    ///
    MmioWrite8 (PciD31F2RegBase + R_PCH_SATA_SIRI, 0x60);
    Data32And = 0xFFFFFFF7;
    Data32Or  = 0x00000008;
    MmioAndThenOr32 (
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_STRD),
      Data32And,
      Data32Or
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint8,
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_SIRI),
      1,
      (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_SIRI)
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_STRD),
      1,
      (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_STRD)
      );
    ///
    /// Step 4
    /// Set D31:F2 + SIR Index 60h[0] = 1b
    ///
    MmioWrite8 (PciD31F2RegBase + R_PCH_SATA_SIRI, 0x60);
    Data32And = 0xFFFFFFFE;
    Data32Or  = 0x00000001;
    MmioAndThenOr32 (
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_STRD),
      Data32And,
      Data32Or
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint8,
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_SIRI),
      1,
      (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_SIRI)
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_STRD),
      1,
      (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_STRD)
      );
    ///
    /// Step 5
    /// Set D31:F2 + SIR Index 60h[1] = 1b
    ///
    MmioWrite8 (PciD31F2RegBase + R_PCH_SATA_SIRI, 0x60);
    Data32And = 0xFFFFFFFD;
    Data32Or  = 0x00000002;
    MmioAndThenOr32 (
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_STRD),
      Data32And,
      Data32Or
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint8,
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_SIRI),
      1,
      (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_SIRI)
      );
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_STRD),
      1,
      (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_STRD)
      );
  }

#ifdef TRAD_FLAG
  ///
  /// For dual controller IDE mode, disable the individual controller if no port enabled.
  /// For SATA2, the SATA configuration should be done through SATA1.
  /// Therefore, disabling the SATA1/SATA2 after finishing SATA configuration.
  ///
  if (PchSeries == PchH) {
    if (SataModeSelect == V_PCH_SATA_MAP_SMS_IDE) {
      WordReg  = MmioRead16 (PciD31F5RegBase + R_PCH_SATA_PCS);
      WordReg &= (UINT16) (B_PCH_SATA2_PCS_PORT5_EN |
                           B_PCH_SATA2_PCS_PORT4_EN);
      if (WordReg == 0) {
        *FuncDisableReg |= B_PCH_RCRB_FUNC_DIS_SATA2;
      }
      WordReg  = MmioRead16 (PciD31F2RegBase + R_PCH_SATA_PCS);
      WordReg &= (UINT16) (B_PCH_SATA_PCS_PORT3_EN |
                           B_PCH_SATA_PCS_PORT2_EN |
                           B_PCH_SATA_PCS_PORT1_EN |
                           B_PCH_SATA_PCS_PORT0_EN);
      if (WordReg == 0) {
        *FuncDisableReg |= B_PCH_RCRB_FUNC_DIS_SATA1;
      }
    }
  }
#endif // TRAD_FLAG

  DEBUG ((DEBUG_INFO, "ConfigureSata() End\n"));

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
ConfigureSataSpeedIde (
  DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  UINTN                               PciDevFuncRegBase,
  UINTN                               MaxPorts
  )
{
  EFI_STATUS            Status;
#ifdef TRAD_FLAG
  PCH_SATA_CONFIG       *SataConfig;
  EFI_PHYSICAL_ADDRESS  IoBaseAddress;
  UINT16                SidpBa;
  UINT16                SidpBaSaveRestore;
  UINT16                DevCmdSaveRestore;
  UINT8                 Data8;
  UINT16                Data16;
  UINTN                 PortIndex;

  Data16      = 0;
  Data8       = 0;

  SataConfig  = PchPlatformPolicy->SataConfig;

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
DEBUG ((DEBUG_ERROR, "SATA IDE base address %lx\n", IoBaseAddress));
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

    switch (SataConfig->SpeedSupport) {
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
#else
  Status = EFI_SUCCESS;
#endif // TRAD_FLAG

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
ConfigureSataAhci (
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
  UINT32                PxCMDRegister;
  UINT16                SataPortsEnabled;
  UINT8                 Index;
  UINTN                 PciD31F2RegBase;
  UINT16                WordReg;
  UINT8                 ByteReg;
  PCH_SATA_CONFIG       *SataConfig;
  UINT16                SataModeSelect;
  UINT32                BaSaveRestore;
  UINT16                DevCmdSaveRestore;
  PCH_SERIES            PchSeries;
  UINT8                 SataResetGpio[LPTH_AHCI_MAX_PORTS] = {
                          PCH_GPIO_SATA_PORT0_RESET,
                          PCH_GPIO_SATA_PORT1_RESET,
                          PCH_GPIO_SATA_PORT2_RESET,
                          PCH_GPIO_SATA_PORT3_RESET,
                          PCH_GPIO_SATA_PORT4_RESET,
                          PCH_GPIO_SATA_PORT5_RESET,
                        };

  UINT16                 SataResetLpGpio[LPTLP_AHCI_MAX_PORTS] = {
                          PCH_LP_GPIO_SATA_PORT0_RESET,
                          PCH_LP_GPIO_SATA_PORT1_RESET,
                          PCH_LP_GPIO_SATA_PORT2_RESET,
                          PCH_LP_GPIO_SATA_PORT3_RESET,
                        };

  PchSeries       = GetPchSeries();
  SataConfig      = PchPlatformPolicy->SataConfig;
  PciD31F2RegBase = MmPciAddress (0, PchPlatformPolicy->BusNumber, 31, 2, 0);
  SataModeSelect  = MmioRead16 (PciD31F2RegBase + R_PCH_SATA_MAP) & B_PCH_SATA_MAP_SMS_MASK;
  DwordReg        = 0;

  // AptioV server override start: For AHCI BAR allocation based on MMIOL ranges
/* Commenting out code that does not follow allocation based upon CPU0 MMIOL ranges
  //
  // Allocate the AHCI BAR
  //
  MemBaseAddress = 0x0ffffffff;
  Status = gDS->AllocateMemorySpace (
                  EfiGcdAllocateMaxAddressSearchBottomUp,
                  EfiGcdMemoryTypeMemoryMappedIo,
                  N_PCH_SATA_AHCI_BAR_ALIGNMENT,  // 2^11: 2K Alignment
                  V_PCH_SATA_AHCI_BAR_LENGTH,     // 2K Length
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
    DEBUG ((DEBUG_ERROR, "AHCI base address %lx\n", MemBaseAddress));
    // AMI Implementation end
    // AptioV server override end: For AHCI BAR allocation based on MMIOL ranges
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ///
  /// Set the AHCI BAR
  ///
  AhciBar = (UINT32) MemBaseAddress;
  BaSaveRestore = MmioRead32(PciD31F2RegBase + R_PCH_SATA_AHCI_BAR);
  MmioWrite32 (PciD31F2RegBase + R_PCH_SATA_AHCI_BAR, AhciBar);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) (PciD31F2RegBase + R_PCH_SATA_AHCI_BAR),
    1,
    &AhciBar
    );

  ///
  /// Enable command register memory space decoding
  ///
  DevCmdSaveRestore = MmioRead16 (PciD31F2RegBase + R_PCH_SATA_COMMAND);
  MmioOr16 ((UINTN) (PciD31F2RegBase + R_PCH_SATA_COMMAND), (UINT16) B_PCH_SATA_COMMAND_MSE);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint16,
    (UINTN) (PciD31F2RegBase + R_PCH_SATA_COMMAND),
    1,
    (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_COMMAND)
    );

  ///
  /// Assert if the memory data of AhciBar is invalid.
  ///
  ASSERT (MmioRead32 (AhciBar) != 0xFFFFFFFF);

  ///
  /// Get Port Settings
  ///
  SataPortsEnabled = MmioRead16 ((UINTN) (PciD31F2RegBase + R_PCH_SATA_PCS));
  if (PchSeries == PchH) {
    SataPortsEnabled &= (UINT16) (B_PCH_SATA_PCS_PORT5_EN |
                                  B_PCH_SATA_PCS_PORT4_EN |
                                  B_PCH_SATA_PCS_PORT3_EN |
                                  B_PCH_SATA_PCS_PORT2_EN |
                                  B_PCH_SATA_PCS_PORT1_EN |
                                  B_PCH_SATA_PCS_PORT0_EN );
  } else if (PchSeries == PchLp) {
    SataPortsEnabled &= (UINT16) (B_PCH_SATA_PCS_PORT3_EN |
                                  B_PCH_SATA_PCS_PORT2_EN |
                                  B_PCH_SATA_PCS_PORT1_EN |
                                  B_PCH_SATA_PCS_PORT0_EN );
  }

  ///
  /// Read the default value of the Host Capabilites Register
  /// NOTE: many of the bits in this register are R/WO (Read/Write Once)
  ///
  CapRegister = MmioRead32 (AhciBar + R_PCH_SATA_AHCI_CAP);
  CapRegister &= (UINT32)~(B_PCH_SATA_AHCI_CAP_SIS | B_PCH_SATA_AHCI_CAP_SSS | B_PCH_SATA_AHCI_CAP_SALP |
                           B_PCH_SATA_AHCI_CAP_PMS | B_PCH_SATA_AHCI_CAP_SSC | B_PCH_SATA_AHCI_CAP_PSC |
                           B_PCH_SATA_AHCI_CAP_SXS);
  if (PchSeries == PchLp) {
    CapRegister &= (UINT32)~(B_PCH_SATA_AHCI_CAP_SAM);
  }

  for (Index = 0; Index < GetPchMaxSataPortNum (); Index++) {
    ///
    /// Check PCS.PxE to know if the SATA Port x is disabled.
    ///
    if ((SataPortsEnabled & (B_PCH_SATA_PCS_PORT0_EN << Index)) == 0) {
      continue;
    }
    if (PchSeries == PchLp) {
      ///
      /// Program AhciBar + 0h[18] = 1b
      ///
      CapRegister |=  B_PCH_SATA_AHCI_CAP_SAM;
    }

    if (SataConfig->PortSettings[Index].InterlockSw == PCH_DEVICE_ENABLE) {
      ///
      /// Mechanical Presence Switch is Enabled for at least one of the ports
      ///
      CapRegister |= B_PCH_SATA_AHCI_CAP_SIS;
    }

    if ((SataConfig->PortSettings[Index].SpinUp == PCH_DEVICE_ENABLE) ||
        (SataConfig->PortSettings[Index].External == PCH_DEVICE_ENABLE)) {
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

    if (SataConfig->PortSettings[Index].External == PCH_DEVICE_ENABLE) {
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
  Data32And = (UINT32) (~B_PCH_H_SATA_PORT_MASK);
  if (PchSeries == PchLp) {
    Data32And = (UINT32) (~B_PCH_LP_SATA_PORT_MASK);
  }
  Data32Or  = (UINT32) (SataPortsEnabled);
  MmioAndThenOr32 (
    (UINTN) (AhciBar + R_PCH_SATA_AHCI_PI),
    Data32And,
    Data32Or
    );
  S3BootScriptSaveMemReadWrite (
    S3BootScriptWidthUint32,
    (UINTN) (AhciBar + R_PCH_SATA_AHCI_PI),
    &Data32Or,  // Data to be ORed
    &Data32And  // Data to be ANDed
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
  if (SataConfig->LpmSupport == PCH_DEVICE_ENABLE)
  {
    //Enable LPM
    CapRegister |= (B_PCH_SATA_AHCI_CAP_SSC | B_PCH_SATA_AHCI_CAP_PSC);

    if (SataConfig->SalpSupport == PCH_DEVICE_ENABLE)
    {
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

  switch (SataConfig->SpeedSupport) {
  case PchSataSpeedSupportGen1:
    CapRegister |= (V_PCH_SATA_AHCI_CAP_ISS_1_5_G << N_PCH_SATA_AHCI_CAP_ISS);
    break;

  case PchSataSpeedSupportGen2:
    CapRegister |= (V_PCH_SATA_AHCI_CAP_ISS_3_0_G << N_PCH_SATA_AHCI_CAP_ISS);
    break;

  case PchSataSpeedSupportGen3:
  case PchSataSpeedSupportDefault:
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

  if (PchSeries == PchLp) {
    ///
    /// Set ABAR + 24h[5] to 1b
    /// Set ABAR + 24h[4:2] to 111b
    ///
    Data32Or = B_PCH_SATA_AHCI_CAP2_DESO;
    Data32Or |= B_PCH_SATA_AHCI_CAP2_SDS | B_PCH_SATA_AHCI_CAP2_SADM | B_PCH_SATA_AHCI_CAP2_APST;
    MmioOr32 (AhciBar + R_PCH_SATA_AHCI_CAP2, Data32Or);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (AhciBar + R_PCH_SATA_AHCI_CAP2),
      1,
      (VOID *) (UINTN) (AhciBar + R_PCH_SATA_AHCI_CAP2)
      );
  }

  ///
  /// Port[Max:0] Command Register update
  /// NOTE: this register must be updated after Port Implemented and Capabilities register,
  /// Many of the bits in this register are R/WO (Read/Write Once)
  ///
  for (Index = 0; Index < GetPchMaxSataPortNum (); Index++) {
    ///
    /// Check PCS.PxE to know if the SATA Port x is disabled.
    ///
    if ((SataPortsEnabled & (B_PCH_SATA_PCS_PORT0_EN << Index)) == 0) {
      continue;
    }
    ///
    /// Initial to zero first
    ///
    PxCMDRegister = 0;

    if (SataConfig->PortSettings[Index].HotPlug == PCH_DEVICE_ENABLE) {
      if (SataConfig->PortSettings[Index].External == PCH_DEVICE_DISABLE) {
        ///
        /// Hot Plug of this port is enabled
        ///
        PxCMDRegister |= B_PCH_SATA_AHCI_PxCMD_HPCP;
        if (SataConfig->PortSettings[Index].InterlockSw == PCH_DEVICE_ENABLE) {
          ///
          /// Mechanical Switch of this port is Attached
          ///
          PxCMDRegister |= B_PCH_SATA_AHCI_PxCMD_MPSP;
          ///
          /// Check the GPIO Pin is set as used for native function not a normal GPIO
          ///
          if (PchSeries == PchH) {
            DwordReg = IoRead32 (
                        (UINTN)
                        (GpioBase + R_PCH_GPIO_USE_SEL +
                       (SataResetGpio[Index] / 32 * (R_PCH_GPIO_USE_SEL2 - R_PCH_GPIO_USE_SEL))));
            DwordReg = (DwordReg & (1 << SataResetGpio[Index] % 32));
          }

          if (PchSeries == PchLp) {
            DwordReg = IoRead32 (
                        (UINTN)
                        (GpioBase + SataResetLpGpio[Index]));
            DwordReg = (DwordReg & B_PCH_GPIO_OWN0_GPIO_USE_SEL);
          }

          if(DwordReg != 0) {
            if (PchSeries == PchH) {
              DEBUG ((DEBUG_ERROR,
                      "BIOS must set the SATA%0xGP / GPIO%0d to native function if Inter Lock Switch is enabled!\n",
                      Index,
                      SataResetGpio[Index]));
              ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
            }

            if (PchSeries == PchLp) {
              DEBUG ((DEBUG_ERROR,
                      "BIOS must set the SATA%0xGP / GPIO%0d to native function if Inter Lock Switch is enabled!\n",
                      Index,
                      SataResetLpGpio[Index]));
              ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
            }
          }
        }
      }
    } else {
      ///
      /// When "Mechanical Switch Attached to Port" (PxCMD[19]) is set, it is expected that HPCP (PxCMD[18]) is also set.
      ///
      if (SataConfig->PortSettings[Index].InterlockSw == PCH_DEVICE_ENABLE) {
        DEBUG ((DEBUG_ERROR, "Hot-Plug function of Port%d should be enabled while the Inter Lock Switch of it is enabled!\n",
        Index));
      }
    }

    if (SataConfig->PortSettings[Index].External == PCH_DEVICE_ENABLE) {
      PxCMDRegister |= B_PCH_SATA_AHCI_PxCMD_ESP;
    }

    if (SataConfig->PortSettings[Index].SpinUp == PCH_DEVICE_ENABLE) {
      PxCMDRegister |= B_PCH_SATA_AHCI_PxCMD_SUD;
    }

    if (PchSeries == PchLp) {
      PxCMDRegister |= (B_PCH_SATA_AHCI_PxCMD_ALPE | B_PCH_SATA_AHCI_PxCMD_APSTE);
    }

    ///
    /// eSATA port support only up to Gen2
    ///
    if (SataConfig->PortSettings[Index].External == PCH_DEVICE_ENABLE) {
      Data32And = (UINT32)~(B_PCH_SATA_AHCI_PXSCTL_SPD);
      Data32Or = (V_PCH_SATA_AHCI_PXSCTL_SPD_2);
      if (PchSeries == PchLp) {
        Data32And = (UINT32)~(B_PCH_SATA_AHCI_PXSCTL_SPD | B_PCH_SATA_AHCI_PXSCTL_IPM);
        Data32Or = (V_PCH_SATA_AHCI_PXSCTL_SPD_2);
      }
      MmioAndThenOr32 (
        (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0SCTL + (0x80 * Index))),
        Data32And,
        Data32Or
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
    if ((PchSeries == PchLp)) {
      ///
      /// Set ABAR + 144h[1], ABAR + 1C4h[1], ABAR + 244h[1], ABAR + 2C4[1] to 0b as default
      /// Board rework is required to enable DevSlp.
      /// POR settings are ABAR + 144h[1], ABAR + 1C4h[1], ABAR + 244h[1] = 1b, ABAR + 2C4[1] to 0b
      ///
      if (SataConfig->PortSettings[Index].DevSlp == PCH_DEVICE_ENABLE) {
        Data32And = (UINT32)~(B_PCH_SATA_AHCI_PxDEVSLP_DITO_MASK | B_PCH_SATA_AHCI_PxDEVSLP_DM_MASK);
        Data32Or = (B_PCH_SATA_AHCI_PxDEVSLP_DSP | V_PCH_SATA_AHCI_PxDEVSLP_DM_16 | V_PCH_SATA_AHCI_PxDEVSLP_DITO_625);
        if (SataConfig->PortSettings[Index].EnableDitoConfig == PCH_DEVICE_ENABLE) {
          Data32Or &= Data32And;
          Data32Or |= ((SataConfig->PortSettings[Index].DitoVal << 15) | (SataConfig->PortSettings[Index].DmVal << 25));
        }
        MmioAndThenOr32 (
          (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0DEVSLP + (0x80 * Index))),
          Data32And,
          Data32Or
          );
      } else {
        MmioAnd32 (
          (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0DEVSLP + (0x80 * Index))),
          (UINT32) ~(B_PCH_SATA_AHCI_PxDEVSLP_DSP)
          );
      }
      DwordReg = MmioRead32 (AhciBar + (R_PCH_SATA_AHCI_P0DEVSLP + (0x80 * Index)));
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (AhciBar + (R_PCH_SATA_AHCI_P0DEVSLP + (0x80 * Index))),
        1,
        &DwordReg
        );
    }

    ///
    /// eSATA port support only up to Gen2.
    /// Save and restore Port Speed limitation
    ///
    if (SataConfig->PortSettings[Index].External == PCH_DEVICE_ENABLE) {
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
  if ((PchSeries == PchLp)) {
    ///
    /// DevSlp on Port 0 and Port 3 are mutual exclusive. Assert if otherwise.
    ///
    ASSERT (!((SataConfig->PortSettings[0].DevSlp) && (SataConfig->PortSettings[3].DevSlp)));
    if ((SataConfig->PortSettings[0].DevSlp == PCH_DEVICE_DISABLE) &&
        (SataConfig->PortSettings[3].DevSlp == PCH_DEVICE_ENABLE)) {
      MmioOr32 (
        (UINTN) (PciD31F2RegBase + R_PCH_SATA_SCLKCG),
        (UINT32) B_PCH_SATA_SCLKCG_POP3_DEVSLP
        );
      DwordReg = MmioRead32 (PciD31F2RegBase + R_PCH_SATA_SCLKCG);
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint32,
        (UINTN) (PciD31F2RegBase + R_PCH_SATA_SCLKCG),
        1,
        &DwordReg
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

  if (SataConfig->HddUnlock == PCH_DEVICE_ENABLE) {
    ///
    /// If set to "1", indicates that the HDD password unlock in the OS is enabled
    /// while SATA is configured as RAID mode.
    ///
    WordReg |= B_PCH_SATA_AHCI_RSTF_HDDLK;
  }

  if (SataConfig->LedLocate == PCH_DEVICE_ENABLE) {
    ///
    /// If set to "1", indicates that the LED/SGPIO hardware is attached and ping to
    /// locate feature is enabled on the OS while SATA is configured as RAID mode.
    ///
    WordReg |= B_PCH_SATA_AHCI_RSTF_LEDL;
  }

  if (SataModeSelect == V_PCH_SATA_MAP_SMS_RAID) {
    if (SataConfig->Raid0 == PCH_DEVICE_ENABLE) {
      ///
      /// If set to "1", then RAID0 is enabled in Flexible RAID Option ROM.
      ///
      WordReg |= B_PCH_SATA_AHCI_RSTF_R0E;
    }

    if (SataConfig->Raid1 == PCH_DEVICE_ENABLE) {
      ///
      /// If set to "1", then RAID1 is enabled in FD-OROM.
      ///
      WordReg |= B_PCH_SATA_AHCI_RSTF_R1E;
    }

    if (SataConfig->Raid10 == PCH_DEVICE_ENABLE) {
      ///
      /// If set to "1", then RAID10 is enabled in FD-OROM.
      ///
      WordReg |= B_PCH_SATA_AHCI_RSTF_R10E;
    }

    if (SataConfig->Raid5 == PCH_DEVICE_ENABLE) {
      ///
      /// If set to "1", then RAID5 is enabled in FD-OROM.
      ///
      WordReg |= B_PCH_SATA_AHCI_RSTF_R5E;
    }

    if (SataConfig->Irrt == PCH_DEVICE_ENABLE) {
      ///
      /// If set to "1", then Intel Rapid Recovery Technology is enabled.
      ///
      WordReg |= B_PCH_SATA_AHCI_RSTF_RSTE;
    }

    if (SataConfig->OromUiBanner == PCH_DEVICE_ENABLE) {
      ///
      /// If set to "1" then the OROM UI is shown.  Otherwise, no OROM banner or information
      /// will be displayed if all disks and RAID volumes are Normal.
      ///
      WordReg |= B_PCH_SATA_AHCI_RSTF_IRSTOROM;
    }

    if (SataConfig->IrrtOnly == PCH_DEVICE_ENABLE) {
      ///
      /// If set to "1", then only IRRT volumes can span internal and eSATA drives. If cleared
      /// to "0", then any RAID volume can span internal and eSATA drives.
      ///
      WordReg |= B_PCH_SATA_AHCI_RSTF_IROES;
    }
    ///
    /// Enable the RST Smart Storage caching bit to support Smart Storage caching.
    ///
    if (SataConfig->SmartStorage == PCH_DEVICE_ENABLE) {
      WordReg |= B_PCH_SATA_AHCI_RSTF_SEREQ;
    }
    ///
    /// Program the delay of the OROM UI Splash Screen in a normal status.
    ///
    WordReg |= (UINT16) (SataConfig->OromUiDelay << N_PCH_SATA_AHCI_RSTF_OUD);
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
  MmioWrite16 (PciD31F2RegBase + R_PCH_SATA_COMMAND, DevCmdSaveRestore);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint16,
    (UINTN) (PciD31F2RegBase + R_PCH_SATA_COMMAND),
    1,
    &DevCmdSaveRestore
    );

  ///
  /// Set Ahci Bar to zero
  ///
  MmioWrite32 (PciD31F2RegBase + R_PCH_SATA_AHCI_BAR, BaSaveRestore);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) (PciD31F2RegBase + R_PCH_SATA_AHCI_BAR),
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
  Disable Sata Controller

  @param[in] PchPlatformPolicy    The PCH Platform Policy protocol instance
  @param[in, out] FuncDisableReg  Function Disable Register

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
DisableSataController (
  IN      DXE_PCH_PLATFORM_POLICY_PROTOCOL    *PchPlatformPolicy,
  IN OUT  UINT32                              *FuncDisableReg
  )
{
  UINTN                                 PciD31F2RegBase;
  UINTN                                 PciD31F5RegBase;
  UINT16                                SataModeSelect;
  PCH_SERIES                            PchSeries;

  PciD31F2RegBase = MmPciAddress (0, PchPlatformPolicy->BusNumber, 31, 2, 0);
  PciD31F5RegBase = 0;
  SataModeSelect  = MmioRead16 (PciD31F2RegBase + R_PCH_SATA_MAP) & B_PCH_SATA_MAP_SMS_MASK;
  PchSeries       = GetPchSeries();

#ifdef ULT_FLAG
  if (PchSeries == PchLp) {
    ///
    /// PCH BIOS Spec Rev 0.5.0, Section 14.2 SATA Controller Disabling
    /// Step 1
    /// Set D31:F2:92h [3:0] to 0000b
    ///
    MmioAnd16 (PciD31F2RegBase + R_PCH_SATA_PCS,
              (UINT16) ~(B_PCH_SATA_PCS_PORT0_EN | B_PCH_SATA_PCS_PORT1_EN | B_PCH_SATA_PCS_PORT2_EN |
                         B_PCH_SATA_PCS_PORT3_EN));
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint16,
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_PCS),
      1,
      (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_PCS)
      );
    ///
    /// Step 3
    /// Set Sata Port Clock Disable bits D31:F2:94h[27:24] to Fh
    ///
    MmioOr32 ((UINTN) (PciD31F2RegBase + R_PCH_SATA_SCLKCG),
              (UINT32) (B_PCH_SATA_SCLKCG_PORT0_PCD | B_PCH_SATA_SCLKCG_PORT1_PCD | B_PCH_SATA_SCLKCG_PORT2_PCD |
                        B_PCH_SATA_SCLKCG_PORT3_PCD));
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_SCLKCG),
      1,
      (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_SCLKCG)
      );
    ///
    /// Step 4
    /// Disabling SATA Device by programming RCBA + 3418h [2][25]
    ///
    *FuncDisableReg |= B_PCH_RCRB_FUNC_DIS_SATA1;
  }
#endif // ULT_FLAG
#ifdef TRAD_FLAG
  if (PchSeries == PchH) {
    PciD31F5RegBase = MmPciAddress (0, PchPlatformPolicy->BusNumber, 31, 5, 0);
    ///
    /// PCH BIOS Spec Rev 0.5.0, Section 14.2 SATA Controller Disabling
    /// Step 1
    /// Set D31:F2:92h [5:0] to 000000b
    ///
    MmioAnd16 (PciD31F2RegBase + R_PCH_SATA_PCS,
              (UINT16) ~(B_PCH_SATA_PCS_PORT0_EN | B_PCH_SATA_PCS_PORT1_EN | B_PCH_SATA_PCS_PORT2_EN |
                         B_PCH_SATA_PCS_PORT3_EN | B_PCH_SATA_PCS_PORT4_EN | B_PCH_SATA_PCS_PORT5_EN));
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint16,
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_PCS),
      1,
      (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_PCS)
      );
    if (SataModeSelect == V_PCH_SATA_MAP_SMS_IDE) {
      ///
      /// Step 2
      /// Set D31:F5:92h [1:0] to 00b if SATA is in IDE mode
      ///
      MmioAnd16 (PciD31F5RegBase + R_PCH_SATA_PCS,
                (UINT16) ~(B_PCH_SATA_PCS_PORT0_EN | B_PCH_SATA_PCS_PORT1_EN));
      S3BootScriptSaveMemWrite (
        S3BootScriptWidthUint16,
        (UINTN) (PciD31F5RegBase + R_PCH_SATA_PCS),
        1,
        (VOID *) (UINTN) (PciD31F5RegBase + R_PCH_SATA_PCS)
        );
    }
    ///
    /// Step 3
    /// Set Sata Port Clock Disable bits D31:F2:94h[29:24] to 3Fh
    ///
    MmioOr32 ((UINTN) (PciD31F2RegBase + R_PCH_SATA_SCLKCG),
              (UINT32) (B_PCH_SATA_SCLKCG_PORT0_PCD | B_PCH_SATA_SCLKCG_PORT1_PCD | B_PCH_SATA_SCLKCG_PORT2_PCD |
                        B_PCH_SATA_SCLKCG_PORT3_PCD | B_PCH_SATA_SCLKCG_PORT4_PCD | B_PCH_SATA_SCLKCG_PORT5_PCD));
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_SCLKCG),
      1,
      (VOID *) (UINTN) (PciD31F2RegBase + R_PCH_SATA_SCLKCG)
      );
    ///
    /// Step 4
    /// Disabling SATA Device by programming RCBA + 3418h [2][25]
    ///
    *FuncDisableReg |= B_PCH_RCRB_FUNC_DIS_SATA1;
    *FuncDisableReg |= B_PCH_RCRB_FUNC_DIS_SATA2;
  }
#endif // TRAD_FLAG

  return EFI_SUCCESS;
}
