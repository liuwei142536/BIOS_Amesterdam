/** @file
  The PCH Init PEIM implements the PCH PEI Init PPI.

@copyright
  Copyright (c) 2004 - 2014 Intel Corporation. All rights reserved
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
#include "PchInitPeim.h"
#ifdef DYNAMIC_MMCFG_BASE_FLAG 
#include "Iio\IioRegs.h" //AptioV server overrride : Dynamic mmcfg base address change
#endif
#ifdef SDP_FLAG
#include <Library/TimerLib.h>
#endif

//
// Global variables
//
static PCH_DMI_TC_VC_PPI          mPchDmiTcVcMap = {
  {
    DmiVcTypeVc0,
    DmiVcTypeVc1,
    DmiVcTypeVcp,
    DmiVcTypeVc0,
    DmiVcTypeVc0,
    DmiVcTypeVc0,
    DmiVcTypeVc0,
    DmiVcTypeVcm
  },
  {
    {PCH_DEVICE_ENABLE, (UINT8) 0},
    {PCH_DEVICE_ENABLE, (UINT8) 1},
    {PCH_DEVICE_ENABLE, (UINT8) 2},
    {PCH_DEVICE_ENABLE, (UINT8) 7}
  }
};

static PCH_INIT_PPI               mPchInitPpi = {
  PchUsbInit,
  PchDmiTcVcProgPoll,
//
// ESS: OverrideBegin
//
  PchDmiTcVcProg,
  PchDmiTcVcEnablePoll,
//
// ESS: OverrideEnd
//
  PchDmiGen2Prog,
  PchCpuStrapSet
};

static EFI_PEI_PPI_DESCRIPTOR     mPpiListVariable = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPchInitPpiGuid,
  &mPchInitPpi
};

static EFI_PEI_PPI_DESCRIPTOR     mPpiPchPeiInitDone = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPchPeiInitDonePpiGuid,
  NULL
};

static EFI_PEI_NOTIFY_DESCRIPTOR  mNotifyList = {
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gPchPlatformPolicyPpiGuid,
  PchInitialize
};

//
// Functions
//

/**
  Internal function performing SATA init needed in PEI phase

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] PchPlatformPolicyPpi The PCH Platform Policy PPI instance

  @retval EFI_SUCCESS             No platform reset action is taken. System can continue boot flow.
  @retval Others                  Won't return if platform reset action is taken
**/
EFI_STATUS
EFIAPI
PchSataInit (
  IN  CONST EFI_PEI_SERVICES            **PeiServices,
  IN  PCH_PLATFORM_POLICY_PPI           *PchPlatformPolicyPpi
  )
{

  UINT32      Data32And;
  UINT32      Data32Or;
  UINTN       PciD31F0RegBase;
  UINT16      LpcDeviceId;
  UINTN       PciD31F2RegBase;
  UINTN       PciD28F0RegBase;
  BOOLEAN     SkipSataInit;
  UINT16      i;
  UINT16      GSpeed;
  UINT16      PortId;
  UINT8       RxEq;
  UINT32      OrMask;
  UINT16      size;
  UINT32      RootComplexBar;
  UINT8       DeviceLaneOwner;
  EFI_STATUS  Status;
  PCH_SERIES  PchSeries;
  UINT32      PchSataTraceId;
#ifdef TRAD_FLAG
  IOBP_MMIO_TABLE_STRUCT *PchSataHsio;
  IOBP_MMIO_TABLE_STRUCT *PchSataHsio_DT;
  IOBP_SATA_RXEQ_TABLE *PchSataRxEqHsio;
#endif // TRAD_FLAG
  IOBP_MMIO_TABLE_STRUCT *PchSataSharedHsio;
  IOBP_MMIO_TABLE_STRUCT *PchSataSharedHsio_DT;
  IOBP_SATA_RXEQ_TABLE *PchSataRxEqSharedHsio;
#ifdef SDP_FLAG
  UINTN       CPUUnCoreBusNo;
  UINTN       BIOSScratchPad0Address;
  UINT8       BIOSScratchPad0;
#endif

  DEBUG ((DEBUG_INFO, "PchSataInit() - Start\n"));

  PchSeries       = GetPchSeries();
  RootComplexBar  = PCH_RCRB_BASE;
  PciD31F0RegBase = MmPciAddress (
                      0,
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_LPC,
                      PCI_FUNCTION_NUMBER_PCH_LPC,
                      0
                      );
  LpcDeviceId = MmioRead16 (PciD31F0RegBase + R_PCH_LPC_DEVICE_ID);
  PciD31F2RegBase = MmPciAddress (
                      0,
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_SATA,
                      PCI_FUNCTION_NUMBER_PCH_SATA,
                      0
                      );
  PciD28F0RegBase = MmPciAddress (
                      0,
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS,
                      PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1,
                      0
                      );
  SkipSataInit = FALSE;

  ///
  /// Skip SATA init if any of SATA port0 ~ port3 is enabled
  ///
  if ((MmioRead8 (PciD31F2RegBase + R_PCH_SATA_PCS) & (UINT8) (B_PCH_SATA_PCS_PORT3_EN |
                                                               B_PCH_SATA_PCS_PORT2_EN |
                                                               B_PCH_SATA_PCS_PORT1_EN |
                                                               B_PCH_SATA_PCS_PORT0_EN)) != 0) {
    SkipSataInit = TRUE;
  }
  if (PchSeries == PchH) {
    ///
    /// Skip SATA init if SATA port4 or port5 is enabled
    ///
    if ((MmioRead8 (PciD31F2RegBase + R_PCH_SATA_PCS) & (UINT8) (B_PCH_SATA_PCS_PORT5_EN |
                                                                 B_PCH_SATA_PCS_PORT4_EN)) != 0) {
      SkipSataInit = TRUE;
    }
  }
  if (SkipSataInit == TRUE) {
    if (PchSeries == PchH) {
      ///
      /// Any SATA port should not be enabled unless CPU only reset.
      /// The value of 0xEA000AAC[5:4] is 10b after issuing CPU only reset.
      /// Note:
      /// The default value of 0xEA000AAC[5:4] is 00b.
      /// The following "if" condition will need to update while the
      /// BIOS recommended setting of 0xEA000AAC[5:4] is changed.
      /// Asset if any SATA port is enabled before SATA  Hsio initialization is done
      ///
      Status = ReadIobp (RootComplexBar, 0xEA000AAC, &Data32And);
      if ((Data32And & (UINT32) (BIT4 | BIT5)) != 0x20) {
        DEBUG ((DEBUG_ERROR, "Please do not enable any SATA port before SATA  Hsio initialization is done.\n"));
//
// ESS: OverrideBegin
//
        //ASSERT (0);  
//
// ESS: OverrideEnd
//
      }
    }
  } else {
    ///
    /// Assume SATA mode will be AHCI, SATA Port 0 - Port 5 are all for D31:F2
    ///
    if (PchSeries == PchH) {
      MmioAndThenOr8 (
        PciD31F2RegBase + R_PCH_SATA_MAP,
        (UINT8) (~B_PCH_SATA_MAP_SMS_MASK),
        (UINT8) (V_PCH_SATA_MAP_SMS_AHCI | B_PCH_SATA_PORT_TO_CONTROLLER_CFG)
        );
    } else if (PchSeries == PchLp) {
      MmioAndThenOr8 (
        PciD31F2RegBase + R_PCH_SATA_MAP,
        (UINT8) (~B_PCH_SATA_MAP_SMS_MASK),
        (UINT8) (V_PCH_SATA_MAP_SMS_AHCI)
        );
    }
    ///
    /// PCH BIOS Spec Rev 0.5.0 section 14.1.7 Additional Programming Requirements during
    /// SATA Initialization
    /// Step 2
    /// System BIOS must set D31:F2:Reg 94h[8:0] = 183h as part of the chipset initialization
    /// prior to SATA configuration. These bits should be restored while resuming from a S3
    /// sleep state.
    ///
    Data32And = (UINT32)~(BIT8 | BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
    Data32Or  = 0x183;
    MmioAndThenOr32 (
      (UINTN) (PciD31F2RegBase + R_PCH_SATA_SCLKCG),
      Data32And,
      Data32Or
      );
    ///
    /// Step 3
    /// D31:F2:Reg 92h[15] = 1b
    /// Set OOB Retry Mode bit of Port Control and Status (PCS) register
    /// These bits should be restored while resuming from a S3 sleep state
    ///
    MmioOr16 ((UINTN) (PciD31F2RegBase + R_PCH_SATA_PCS), (UINT16) (B_PCH_SATA_PCS_OOB_RETRY));
    ///
    /// Step 4
    /// System BIOS must program SATA Hsio table as stated in Table 7-7 to 7-8 BEFORE the SATA
    /// ports are enabled.
    ///
    /// PCH BIOS Spec Rev 0.5.6, Section 7.1.5
    /// Step 8
    /// Bios is required to program IOBP setting according to the table 7-7 to 7-8
    /// using 7.1.4 IOSF SBI with OPCODE "PHY Configuration Register".
    /// Table 7-7 SATA dedicated lane setting
    ///
	DeviceLaneOwner = MmioRead8 (PciD28F0RegBase + 0x410);
#ifdef TRAD_FLAG        
    switch (PchStepping()) {
      case LptHB0:
#ifdef PCH_SERVER_BIOS_FLAG
        DEBUG ((DEBUG_INFO, " PCH Hsio Ver : 0x%xp1\n", PCH_WBG_HSIO_VER_A0));
        size = (sizeof (PchSataHsioWbg_A0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
        PchSataHsio = PchSataHsioWbg_A0;
#else
        size = (sizeof (PchSataHsioLptH_B0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
        PchSataHsio = PchSataHsioLptH_B0;
#endif //PCH_SERVER_BIOS_FLAG
        break;
      case LptHC0:
      case LptHC1:
      case LptHC2:
#ifdef PCH_SERVER_BIOS_FLAG
        DEBUG ((DEBUG_INFO, " PCH Hsio Ver : 0x%xp1\n", PCH_WBG_HSIO_VER_B0));
        size = (sizeof (PchSataHsioWbg_B0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
        PchSataHsio = PchSataHsioWbg_B0;
#else
        size = (sizeof (PchSataHsioLptH_C0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
        PchSataHsio = PchSataHsioLptH_C0;
#endif //PCH_SERVER_BIOS_FLAG
        break;
      default:
        PchSataHsio = NULL;
        size = 0;
        DEBUG ((DEBUG_ERROR, "Unsupported PCH Stepping for PchSataHsio\n"));
    }

    for (i = 0; i < size; i++) {
      Status = ProgramIobp (
                RootComplexBar,
                PchSataHsio[i].Address,
                PchSataHsio[i].AndMask,
                PchSataHsio[i].OrMask
                );
      ASSERT_EFI_ERROR (Status);
    }
#endif // TRAD_FLAG    
    ///
    /// Table 7-8 SATA Shared lane setting
    ///
    switch (PchStepping()) {
#ifdef ULT_FLAG
      case LptLpB0:
      case LptLpB1:
      case LptLpB2:
        size = (sizeof (PchSataSharedHsioLptLp_Bx) / sizeof (IOBP_MMIO_TABLE_STRUCT));
        PchSataSharedHsio = PchSataSharedHsioLptLp_Bx;
        break;
#endif // ULT_FLAG
#ifdef TRAD_FLAG
      case LptHB0:
#ifdef PCH_SERVER_BIOS_FLAG
        size = (sizeof (PchSataSharedHsioWbg_A0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
        PchSataSharedHsio = PchSataSharedHsioWbg_A0;
#else	  
        size = (sizeof (PchSataSharedHsioLptH_B0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
        PchSataSharedHsio = PchSataSharedHsioLptH_B0;
#endif //PCH_SERVER_BIOS_FLAG
        break;
      case LptHC0:
      case LptHC1:
      case LptHC2:
#ifdef PCH_SERVER_BIOS_FLAG
        size = (sizeof (PchSataSharedHsioWbg_B0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
        PchSataSharedHsio = PchSataSharedHsioWbg_B0;
#else
        size = (sizeof (PchSataSharedHsioLptH_C0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
        PchSataSharedHsio = PchSataSharedHsioLptH_C0;
#endif //PCH_SERVER_BIOS_FLAG
        break;
#endif // TRAD_FLAG
      default:
        size = 0;
        PchSataSharedHsio = NULL;
        DEBUG ((DEBUG_ERROR, "Unsupported PCH Stepping for PchSataSharedHsio\n"));
    }
    for (i = 0; i < size; i++) {
      if (PchSeries == PchLp) {
        if ((((PchSataSharedHsio[i].Address & 0xFE00) == 0x2000) && ((DeviceLaneOwner & BIT4) == BIT4)) ||
            (((PchSataSharedHsio[i].Address & 0xFE00) == 0x2200) && ((DeviceLaneOwner & BIT5) == BIT5)) ||
            (((PchSataSharedHsio[i].Address & 0xFE00) == 0x2400) && ((DeviceLaneOwner & BIT6) == BIT6)) ||
            (((PchSataSharedHsio[i].Address & 0xFE00) == 0x2600) && ((DeviceLaneOwner & BIT7) == BIT7))) {
          continue;
        }
      } else if (PchSeries == PchH) {
        if ((((PchSataSharedHsio[i].Address & 0xFE00) == 0x2000) && ((DeviceLaneOwner & BIT4) == BIT4)) ||
            (((PchSataSharedHsio[i].Address & 0xFE00) == 0x2200) && ((DeviceLaneOwner & BIT5) == BIT5))) {
          continue;
        }
      }
      Status = ProgramIobp (
                RootComplexBar,
                PchSataSharedHsio[i].Address,
                PchSataSharedHsio[i].AndMask,
                PchSataSharedHsio[i].OrMask
                );
      ASSERT_EFI_ERROR (Status);
    }

      ///
      /// For Desktop:
      /// BIOS is required to program IOBP setting according to Table 7-13 and
      /// Table 7-14 using settings in Section 7.1.4 with OPCODE "PHY Configuration Register".
      /// Table 7-13 SATA Dedicated Lane Setting
      ///
#ifdef TRAD_FLAG
      switch (PchStepping()) {
        case LptHB0:
#ifdef PCH_SERVER_BIOS_FLAG
          size = (sizeof (PchSataHsioWbg_DT_A0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
          PchSataHsio_DT = PchSataHsioWbg_DT_A0;
#else		
          size = (sizeof (PchSataHsioLptH_DT_B0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
          PchSataHsio_DT = PchSataHsioLptH_DT_B0;
#endif //PCH_SERVER_BIOS_FLAG
          break;
        case LptHC0:
        case LptHC1:
        case LptHC2:
#ifdef PCH_SERVER_BIOS_FLAG
          size = (sizeof (PchSataHsioWbg_DT_B0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
          PchSataHsio_DT = PchSataHsioWbg_DT_B0;
#else
          if(PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataHsio_DT.Size != 0 &&
            PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataHsio_DT.Table != NULL)
          {
            size = PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataHsio_DT.Size;
            PchSataHsio_DT = PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataHsio_DT.Table;
          } else {
            size = 0;
            PchSataHsio_DT = NULL;
            DEBUG ((EFI_D_ERROR, "Cannot apply MPHY values for PchSataHsio_DT\n"));
            ASSERT(FALSE);
          }
#endif //PCH_SERVER_BIOS_FLAG
          break;
        default:
          size = 0;
          PchSataHsio_DT = NULL;
          DEBUG ((EFI_D_ERROR, "Unsupported PCH Stepping for PchSataHsio_DT\n"));
      }
      for (i = 0; i < size; i++) {
        Status = ProgramIobp (
                  RootComplexBar,
                  PchSataHsio_DT[i].Address,
                  PchSataHsio_DT[i].AndMask,
                  PchSataHsio_DT[i].OrMask
                  );
        ASSERT_EFI_ERROR (Status);
      }
#endif // TRAD_FLAG
      ///
      /// Table 7-14 SATA Shared Lane Setting
      ///
      switch (PchStepping()) {
#ifdef ULT_FLAG
        case LptLpB0:
        case LptLpB1:
        case LptLpB2:
          size = (sizeof (PchSataSharedHsioLptLp_DT_Bx) / sizeof (IOBP_MMIO_TABLE_STRUCT));
          PchSataSharedHsio_DT = PchSataSharedHsioLptLp_DT_Bx;
          break;
#endif // ULT_FLAG
#ifdef TRAD_FLAG
        case LptHB0:
#ifdef PCH_SERVER_BIOS_FLAG
          size = (sizeof (PchSataSharedHsioWbg_DT_A0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
          PchSataSharedHsio_DT = PchSataSharedHsioWbg_DT_A0;
#else
          size = (sizeof (PchSataSharedHsioLptH_DT_B0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
          PchSataSharedHsio_DT = PchSataSharedHsioLptH_DT_B0;
#endif //PCH_SERVER_BIOS_FLAG
          break;
        case LptHC0:
        case LptHC1:
        case LptHC2:
#ifdef PCH_SERVER_BIOS_FLAG
          size = (sizeof (PchSataSharedHsioWbg_DT_B0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
          PchSataSharedHsio_DT = PchSataSharedHsioWbg_DT_B0;
#else
          if(PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataSharedHsio_DT.Size != 0 &&
            PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataSharedHsio_DT.Table != NULL)
          {
            size = PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataSharedHsio_DT.Size;
            PchSataSharedHsio_DT = PchPlatformPolicyPpi->BoardDependentMphyTables.PchSataSharedHsio_DT.Table;
          } else {        
            size = 0;
            PchSataSharedHsio_DT = NULL;
            DEBUG ((EFI_D_ERROR, "Cannot apply MPHY values for PchSataSharedHsio_DT\n"));            
            ASSERT(FALSE);
          }
#endif //PCH_SERVER_BIOS_FLAG
          break;
#endif // TRAD_FLAG
        default:
          size = 0;
          PchSataSharedHsio_DT = NULL;
          DEBUG ((EFI_D_ERROR, "Unsupported PCH Stepping for PchSataSharedHsio_DT\n"));
      }
      for (i = 0; i < size; i++) {
        if (PchSeries == PchLp) {
          if ((((PchSataSharedHsio_DT[i].Address & 0xFE00) == 0x2000) && ((DeviceLaneOwner & BIT4) == BIT4)) ||
              (((PchSataSharedHsio_DT[i].Address & 0xFE00) == 0x2200) && ((DeviceLaneOwner & BIT5) == BIT5)) ||
              (((PchSataSharedHsio_DT[i].Address & 0xFE00) == 0x2400) && ((DeviceLaneOwner & BIT6) == BIT6)) ||
              (((PchSataSharedHsio_DT[i].Address & 0xFE00) == 0x2600) && ((DeviceLaneOwner & BIT7) == BIT7))) {
            continue;
          }
        } else if (PchSeries == PchH) {
          if ((((PchSataSharedHsio_DT[i].Address & 0xFE00) == 0x2000) && ((DeviceLaneOwner & BIT4) == BIT4)) ||
              (((PchSataSharedHsio_DT[i].Address & 0xFE00) == 0x2200) && ((DeviceLaneOwner & BIT5) == BIT5))) {
            continue;
          }
        }
        Status = ProgramIobp (
                  RootComplexBar,
                  PchSataSharedHsio_DT[i].Address,
                  PchSataSharedHsio_DT[i].AndMask,
                  PchSataSharedHsio_DT[i].OrMask
                  );
        ASSERT_EFI_ERROR (Status);
      }

    ///
    /// Table 7-15 SATA RxEq Dedicated Lane Setting
    ///
    PchSataTraceId = 0;
#ifdef TRAD_FLAG
    switch (PchStepping()) {
      case LptHC0:
      case LptHC1:
      case LptHC2:
#ifdef PCH_SERVER_BIOS_FLAG
        size = (sizeof (PchSataRxEqHsioWbg_B0) / sizeof (IOBP_SATA_RXEQ_TABLE));
        PchSataRxEqHsio = PchSataRxEqHsioWbg_B0;
#else         
//        size = (sizeof (PchSataRxEqHsioLptH_C0) / sizeof (IOBP_SATA_RXEQ_TABLE));
//        PchSataRxEqHsio = PchSataRxEqHsioLptH_C0;
      PchSataRxEqHsio = NULL;
      size = 0;
#endif //PCH_SERVER_BIOS_FLAG
        break;
      default:
        PchSataRxEqHsio = NULL;
        size = 0;
        DEBUG ((EFI_D_ERROR, "Unsupported PCH Stepping for PchSataRxEqHsio\n"));
    }

    for(PortId = 0; PortId < GetPchMaxSataPortNum (); PortId++){
      for(GSpeed = 0; GSpeed < 3; GSpeed++){
        if(PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[PortId].GenSpeed[GSpeed].Enable == PCH_DEVICE_ENABLE) {
          PchSataTraceId = PCH_SATA_RXEQ_ID(PortId, GSpeed);
          for (i = 0; i < size; i++) {
            if(PchSataRxEqHsio[i].TraceId == PchSataTraceId) {
              RxEq = PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[PortId].GenSpeed[GSpeed].RxEq;
              OrMask = (((UINT32) (((RxEq) << 24 ) + ((RxEq) << 16 ) + ((RxEq) << 8 ) + RxEq)) & ((UINT32)~(PchSataRxEqHsio[i].AndMask)));
              Status = ProgramIobp (
                         RootComplexBar,
                         PchSataRxEqHsio[i].Address,
                         PchSataRxEqHsio[i].AndMask,
                         OrMask
                         );
              ASSERT_EFI_ERROR (Status);
            }
          }
        }
      }
    }
#endif // TRAD_FLAG

    ///
    /// Table 7-16 SATA RxEq Shared Lane Setting
    ///
    switch (PchStepping()) {
#ifdef ULT_FLAG
      case LptLpB0:
      case LptLpB1:
      case LptLpB2:
        size = (sizeof (PchSataRxEqSharedHsioLptLp_Bx) / sizeof (IOBP_SATA_RXEQ_TABLE));
        PchSataRxEqSharedHsio = PchSataRxEqSharedHsioLptLp_Bx;
        break;
#endif // ULT_FLAG
#ifdef TRAD_FLAG
      case LptHC0:
      case LptHC1:
      case LptHC2:
#ifdef PCH_SERVER_BIOS_FLAG
        size = (sizeof (PchSataRxEqSharedHsioWbg_B0) / sizeof (IOBP_SATA_RXEQ_TABLE));
        PchSataRxEqSharedHsio = PchSataRxEqSharedHsioWbg_B0;       
#else
//        size = (sizeof (PchSataRxEqSharedHsioLptH_C0) / sizeof (IOBP_SATA_RXEQ_TABLE));
//        PchSataRxEqSharedHsio = PchSataRxEqSharedHsioLptH_C0;
        size = 0; 
        PchSataRxEqSharedHsio = NULL;
#endif //PCH_SERVER_BIOS_FLAG        

        break;
#endif // TRAD_FLAG
      default:
        size = 0;
        PchSataRxEqSharedHsio = NULL;
        DEBUG ((EFI_D_ERROR, "Unsupported PCH Stepping for PchSataRxEqSharedHsio\n"));
    }

    for(PortId = 0; PortId < GetPchMaxSataPortNum (); PortId++){
      for(GSpeed = 0; GSpeed < 3; GSpeed++){
        if(PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[PortId].GenSpeed[GSpeed].Enable == PCH_DEVICE_ENABLE) {
          PchSataTraceId = PCH_SATA_RXEQ_ID(PortId, GSpeed);
          for (i = 0; i < size; i++) {
            if(PchSataRxEqSharedHsio[i].TraceId == PchSataTraceId) {
              if (PchSeries == PchLp) {
                if ((((PchSataRxEqSharedHsio[i].Address & 0xFE00) == 0x2000) && ((DeviceLaneOwner & BIT4) == BIT4)) ||
                  (((PchSataRxEqSharedHsio[i].Address & 0xFE00) == 0x2200) && ((DeviceLaneOwner & BIT5) == BIT5)) ||
                  (((PchSataRxEqSharedHsio[i].Address & 0xFE00) == 0x2400) && ((DeviceLaneOwner & BIT6) == BIT6)) ||
                  (((PchSataRxEqSharedHsio[i].Address & 0xFE00) == 0x2600) && ((DeviceLaneOwner & BIT7) == BIT7)))
                {
                    continue;
                }
              } else if (PchSeries == PchH) {
                if ((((PchSataRxEqSharedHsio[i].Address & 0xFE00) == 0x2000) && ((DeviceLaneOwner & BIT4) == BIT4)) ||
                  (((PchSataRxEqSharedHsio[i].Address & 0xFE00) == 0x2200) && ((DeviceLaneOwner & BIT5) == BIT5)))
                {
                    continue;
                }
              }
              RxEq = PchPlatformPolicyPpi->SataConfig->SataTraceConfig->PortRxEq[PortId].GenSpeed[GSpeed].RxEq;
              OrMask = (((UINT32) (((RxEq) << 24 ) + ((RxEq) << 16 ) + ((RxEq) << 8 ) + RxEq)) & ((UINT32)~(PchSataRxEqSharedHsio[i].AndMask)));
              Status = ProgramIobp (
                         RootComplexBar,
                         PchSataRxEqSharedHsio[i].Address,
                         PchSataRxEqSharedHsio[i].AndMask,
                         OrMask
                         );
              ASSERT_EFI_ERROR (Status);
            }
          }
        }
      }
    }

    ///
    /// PCH BIOS Spec Rev 0.5.0 section 14.1.7 Additional Programming Requirements during
    /// SATA Initialization
    /// Step 5
    /// Program D31:F2:98h[22] to 1b for desktop and mobile platform only.
    ///
    if (IS_PCH_LPT_LPC_DEVICE_ID_DESKTOP (LpcDeviceId) ||
        IS_PCH_LPT_LPC_DEVICE_ID_MOBILE (LpcDeviceId) 
#ifdef PCH_SERVER_BIOS_FLAG
       || IS_PCH_WBG_LPC_DEVICE_ID (LpcDeviceId)
#endif //PCH_SERVER_BIOS_FLAG
){
      MmioOr32 (
        (UINTN) (PciD31F2RegBase + 0x98),
        (UINT32) (BIT22)
        );
    }
    ///
    /// Step 6
    /// Program D31:F2:98h[19] = 1b
    ///
    MmioOr32 (
      (UINTN) (PciD31F2RegBase + 0x98),
      (UINT32) (BIT19)
      );
    ///
    /// Step 7
    /// Program D31:F2:98h[12:7] = 04h
    ///
    Data32And = (UINT32) (~(BIT7 | BIT8 | BIT10 | BIT11 | BIT12));
    Data32Or  = (UINT32) (BIT9);
    MmioAndThenOr32 (
      (UINTN) (PciD31F2RegBase + 0x98),
      Data32And,
      Data32Or
      );
    ///
    /// Step 8
    /// Program D31:F2:98h[20] to 1b
    ///
    MmioOr32 ((UINTN) (PciD31F2RegBase + 0x98), (UINT32) (BIT20));
    ///
    /// Step 9
    /// Program D31:F2:98h[6:5] to 01b
    ///
    MmioAndThenOr32 (
      (UINTN) (PciD31F2RegBase + 0x98),
      (UINT32) (~(BIT6 | BIT5)),
      BIT5
      );
    ///
    /// Step 10
    /// Program D31:F2:98h [18] to 1b
    ///
    Data32Or  = (UINT32) (BIT18);
    MmioOr32 (
      (UINTN) (PciD31F2RegBase + 0x98),
      Data32Or
      );
    ///
    /// Step 11
    /// Program D31:F2:98h[29] to 1b
    /// Done in PchInitBeforeBoot()
    ///
    /// Step 12
    /// Program D31:F2:9Ch[5] to 1b (Note: this must be programmed together with D31:F2:9Ch[7:6]
    /// in word write)
    /// Done in ConfigureSata ()
    ///
    /// Step 13
    /// When SATA in IDE mode
    /// a. Program D31:F2:34h [7:0] to 70h
    /// b. Program D31:F2:70h [15:8] to 0h
    /// Done in PchMiscInit ()
    ///
    /// Step 14
    /// Program D31:F2:9Ch[31] to 1b at the End of Post
    /// Done in PchInitBeforeBoot()
    ///
    /// Enable the SATA port0 ~ port3.
    ///
    if (PchSeries == PchH) {
#ifdef SDP_FLAG
      CPUUnCoreBusNo = (UINT16)AsmReadMsr64(MSR_CPU_BUS_NUMBER);
      CPUUnCoreBusNo >>= 8;

      BIOSScratchPad0Address = MmPciAddress (
        0,
        CPUUnCoreBusNo,
        UBOX_SCRATCH_DEVICE,
        UBOX_SCRATCH_FUNC7,
        UBOX_BIOS_SCRATCHPAD0
        );
      BIOSScratchPad0 = MmioRead8(BIOSScratchPad0Address);

      MmioOr8 (
        PciD31F2RegBase + R_PCH_SATA_PCS,
        (UINT8) (B_PCH_SATA_PCS_PORT1_EN | B_PCH_SATA_PCS_PORT0_EN)
      );

      if (!(BIOSScratchPad0 & BIT1)){          // Check for COLD BOOT
        DEBUG ((DEBUG_INFO, "PchsSataInit() - HDD Spin-up 1 Second Delay\n"));		  
        MicroSecondDelay (STALL_ONE_SECOND);
      }	

      MmioOr8 (
        PciD31F2RegBase + R_PCH_SATA_PCS,
        (UINT8) (B_PCH_SATA_PCS_PORT3_EN | B_PCH_SATA_PCS_PORT2_EN)
      );
#else	  
      MmioOr8 (
        PciD31F2RegBase + R_PCH_SATA_PCS,
        (UINT8) (B_PCH_SATA_PCS_PORT3_EN | B_PCH_SATA_PCS_PORT2_EN | B_PCH_SATA_PCS_PORT1_EN | B_PCH_SATA_PCS_PORT0_EN)
        );
#endif
    }
    if (PchSeries == PchLp) {
      ///
      /// If D28:F0:410h[7] = 1b, System BIOS should not enable the SATA port0
      /// If D28:F0:410h[6] = 1b, System BIOS should not enable the SATA port1
      /// If D28:F0:410h[5] = 1b, System BIOS should not enable the SATA port2
      /// If D28:F0:410h[4] = 1b, System BIOS should not enable the SATA port3
      ///
      if ((MmioRead8 (PciD28F0RegBase + 0x410) & BIT7) == 0) {
        MmioOr8 (
          PciD31F2RegBase + R_PCH_SATA_PCS,
          (UINT8) B_PCH_SATA_PCS_PORT0_EN
          );
      }
      if ((MmioRead8 (PciD28F0RegBase + 0x410) & BIT6) == 0) {
        MmioOr8 (
          PciD31F2RegBase + R_PCH_SATA_PCS,
          (UINT8) B_PCH_SATA_PCS_PORT1_EN
          );
      }
      if ((MmioRead8 (PciD28F0RegBase + 0x410) & BIT5) == 0) {
        MmioOr8 (
          PciD31F2RegBase + R_PCH_SATA_PCS,
          (UINT8) B_PCH_SATA_PCS_PORT2_EN
          );
      }
      if ((MmioRead8 (PciD28F0RegBase + 0x410) & BIT4) == 0) {
        MmioOr8 (
          PciD31F2RegBase + R_PCH_SATA_PCS,
          (UINT8) B_PCH_SATA_PCS_PORT3_EN
          );
      }
    }
    if (PchSeries == PchH) {
      ///
      /// Enable the SATA port4 and port5.
      /// Step 1.a
      /// If D28:F0:410h[4] = 1b, System BIOS should not enable the SATA port4
      /// Step 1.b
      /// If D28:F0:410h[5] = 1b, System BIOS should not enable the SATA port5
      ///
      if ((MmioRead8 (PciD28F0RegBase + 0x410) & BIT4) == 0) {
        MmioOr8 (
          PciD31F2RegBase + R_PCH_SATA_PCS,
          (UINT8) B_PCH_SATA_PCS_PORT4_EN
          );
      }
      if ((MmioRead8 (PciD28F0RegBase + 0x410) & BIT5) == 0) {
        MmioOr8 (
          PciD31F2RegBase + R_PCH_SATA_PCS,
          (UINT8) B_PCH_SATA_PCS_PORT5_EN
          );
      }
    }
  }

  DEBUG ((DEBUG_INFO, "PchSataInit() - End\n"));
  return EFI_SUCCESS;
}

/**
  The function is used while doing CPU Only Reset, where PCH may be required
  to initialize strap data before soft reset.

  @param[in] PeiServices          General purpose services available to every PEIM
  @param[in] Operation            Get/Set Cpu Strap Set Data
  @param[in, out] CpuStrapSet     Cpu Strap Set Data

  @retval EFI_SUCCESS             The function completed successfully.
  @exception EFI_UNSUPPORTED      The function is not supported.
**/
EFI_STATUS
EFIAPI
PchCpuStrapSet (
  IN      CONST EFI_PEI_SERVICES            **PeiServices,
  IN      CPU_STRAP_OPERATION         Operation,
  IN OUT  UINT16                      *CpuStrapSet
  )
{
  UINT32  RootComplexBar;

  DEBUG ((DEBUG_INFO, "PchCpuStrapSet() - Start\n"));

  RootComplexBar = PCH_RCRB_BASE;

  switch (Operation) {
  case GetCpuStrapSetData:
    ///
    /// Get CPU Strap Settings select. 0 = from descriptor, 1 = from PCH
    ///
    if ((MmioRead8 ((UINTN) (RootComplexBar + R_PCH_SPI_SRDC)) & B_PCH_SPI_SRDC_SRDS) == 0) {
      ///
      /// Read Strap from Flash Descriptor
      ///
      *CpuStrapSet = 0;
      return EFI_SUCCESS;
    } else {
      ///
      /// Read Strap from PCH Soft Strap.
      ///
      *CpuStrapSet = MmioRead16 ((UINTN) (RootComplexBar + R_PCH_SPI_SRD));
    }
    break;

  case SetCpuStrapSetData:
    ///
    /// PCH BIOS Spec Rev 0.5.0, Section 4.3 Soft Reset Control
    /// 2. If there are CPU configuration changes, program the strap setting into the
    ///    Soft Reset Data register located at SPIBAR Offset F8h [15:0] (RCBA + Offset 38F8h [15:0])
    ///    and follow the steps outlined in the "CPU Only Reset BIOS Flow" section of the Processor
    ///    BIOS Writer's Guide and skip steps 3 and 4.
    ///    a. Program Soft Reset Data Register SPIBAR + F8h [13:0] (RCBA + 38F8h [13:0])
    ///       (details in Processor BIOS Writer's Guide)
    ///    b. Set RCBA + Offset 38F4h[0] = 1b
    ///    c. Set RCBA + Offset 38F0h[0] = 1b
    ///    d. Skip steps 3 and 4.
    ///
    MmioWrite16 ((UINTN) (RootComplexBar + R_PCH_SPI_SRD), *CpuStrapSet);
    MmioOr8 ((UINTN) (RootComplexBar + R_PCH_SPI_SRDC), B_PCH_SPI_SRDC_SRDS);
    MmioOr8 ((UINTN) (RootComplexBar + R_PCH_SPI_SRDL), B_PCH_SPI_SRDL_SSL);
    break;

  case LockCpuStrapSetData:
    MmioOr8 ((UINTN) (RootComplexBar + R_PCH_SPI_SRDL), B_PCH_SPI_SRDL_SSL);
    break;

  default:
    break;
  }

  DEBUG ((DEBUG_INFO, "PchCpuStrapSet() - End\n"));

  return EFI_SUCCESS;
}

/**
  This function may trigger platform reset depending on the current GbE status,
  the intended GbE enabling, and current ME status. (When ME is enabled, this function
  may trigger a Global reset.)
  This function may not return if it triggers an platform reset and the BIOS boot flow
  restarts.
  If this function returns EFI_SUCCESS it indicates there is no need for platform
  reset in this boot, and boot flow continues.
  If this function returns EFI_DEVICE_ERROR, something error happens.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] PchPlatformPolicyPpi The PCH Platform Policy PPI instance

  @retval EFI_SUCCESS             No platform reset action is taken. System can continue boot flow.
  @retval Others                  Won't return if platform reset action is taken
**/
EFI_STATUS
EFIAPI
PchGbeMandatedReset (
  IN CONST EFI_PEI_SERVICES             **PeiServices,
  IN  PCH_PLATFORM_POLICY_PPI     *PchPlatformPolicyPpi
  )
{
  UINT8           RegData8;
  UINTN           PciD25F0RegBase;
  UINT32          GbEMemBar;
  UINT32          TempGbEMemBar;
  UINT16          CmdReg;
  BOOLEAN         ResetRequired;
  BOOLEAN         GbeRegion;
  PCH_RESET_PPI   *PchResetPpi;
  EFI_STATUS      Status;
  PCH_RESET_TYPE  PchResetType;

  PciD25F0RegBase = 0;
  GbEMemBar       = 0;
  ResetRequired   = FALSE;

  ///
  /// Read the BUC register
  ///
  RegData8 = MmioRead8 (PchPlatformPolicyPpi->Rcba + R_PCH_RCRB_BUC);

  GbeRegion = PchIsGbeRegionValid (PchPlatformPolicyPpi->Rcba);

  ///
  /// If no change of status, just return success
  ///
  if (((RegData8 & B_PCH_RCRB_BUC_LAN_DIS) &&
      !PchPlatformPolicyPpi->GbeConfig->EnableGbe) ||
      (!(RegData8 & B_PCH_RCRB_BUC_LAN_DIS) &&
      PchPlatformPolicyPpi->GbeConfig->EnableGbe)) {
    return EFI_SUCCESS;
  }
  ///
  /// Set the BUC register
  ///
  if (PchPlatformPolicyPpi->GbeConfig->EnableGbe) {
    ///
    /// Change internal Gbe from disabled to enabled
    ///
    if (GbeRegion == TRUE) {
      ResetRequired = TRUE;
      ///
      /// PCH BIOS Spec Rev 0.5.0, Section 10.2.1 Enable the Internal GbE Controller
      /// Step 1
      /// Set RCBA + 3414h[5] = 0b
      ///
      MmioAnd8 (PchPlatformPolicyPpi->Rcba + R_PCH_RCRB_BUC, (UINT8) (~B_PCH_RCRB_BUC_LAN_DIS));
    }
  } else {
    ///
    /// Change internal Gbe from enabled to disabled
    ///
    if (GbeRegion == TRUE) {
      ///
      /// PCH BIOS Spec Rev 0.5.0, Section 10.2.2 Disable the Internal GbE Controller
      /// Step 1a
      /// If Intel ME enable then detect if it supports GBe. Read FWSM_S[15] bit in MBARA + offset 5B54h register.
      ///
      PciD25F0RegBase = MmPciAddress (
                          0,
                          PchPlatformPolicyPpi->BusNumber,
                          PCI_DEVICE_NUMBER_PCH_LAN,
                          PCI_FUNCTION_NUMBER_PCH_LAN,
                          0
                          );
      ///
      /// Store current value of PCH_LAN_MEM_BASE_A
      ///
      TempGbEMemBar = MmioRead32 (PciD25F0RegBase + R_PCH_LAN_MEM_BASE_A);
      ///
      /// As PCI enumeration has not been done, set PCH_LAN_MBARB per the platform policy
      ///
      MmioWrite32 (PciD25F0RegBase + R_PCH_LAN_MEM_BASE_A, PchPlatformPolicyPpi->PlatformData->TempMemBaseAddr);
      ///
      /// Store the setting of R_PCH_LAN_CMD
      ///
      CmdReg = MmioRead16 (PciD25F0RegBase + R_PCH_LAN_CMD);
      ///
      /// Enable memory space decoding in command register
      ///
      MmioOr16 (PciD25F0RegBase + R_PCH_LAN_CMD, (UINT16) B_PCH_LAN_CMD_MSE);
      ///
      /// Check if GbE device exists
      ///
      GbEMemBar = MmioRead32 (PciD25F0RegBase + R_PCH_LAN_MEM_BASE_A) & B_PCH_LAN_MBARA_BA;

      if (GbEMemBar != 0xFFFFFFFF) {
        if ((MmioRead16 (GbEMemBar + 0x5B54)) & BIT15) {
          ResetRequired = TRUE;
        }
      }
      ///
      /// Restore the setting of R_PCH_LAN_CMD
      ///
      MmioWrite16 (PciD25F0RegBase + R_PCH_LAN_CMD, CmdReg);
      ///
      /// Restore the value of PCH_LAN_MEM_BASE_A
      ///
      MmioWrite32 (PciD25F0RegBase + R_PCH_LAN_MEM_BASE_A, TempGbEMemBar);
    }
    ///
    /// Step 1
    /// Set RCBA + 3414h[5] = 1b
    ///
    MmioOr8 (PchPlatformPolicyPpi->Rcba + R_PCH_RCRB_BUC, (UINT8) B_PCH_RCRB_BUC_LAN_DIS);
  }
  ///
  /// PCH BIOS Spec Rev 0.5.0, Section 10.2.1 & 10.2.2
  /// Step 2
  /// Read back for posted write to take effect
  ///
  MmioRead8 (PchPlatformPolicyPpi->Rcba + R_PCH_RCRB_BUC);

  if (!ResetRequired) {
    return EFI_SUCCESS;
  }

  Status = (*PeiServices)->LocatePpi (
                            PeiServices,
                            &gPchResetPpiGuid,
                            0,
                            NULL,
                            (VOID **)&PchResetPpi
                            );

  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_ERROR, "PeiPchGbeMandatedReset: resetting the board via CF9...\n"));
  if ((MmioRead32 (PchPlatformPolicyPpi->Rcba + R_PCH_RCRB_FD2) & B_PCH_RCRB_FD2_MEI1D) == 0) {
    if (PchPlatformPolicyPpi->PlatformData->EcPresent) {
      PchResetType = GlobalResetWithEc;
    } else {
      PchResetType = GlobalReset;
    }
  } else {
    PchResetType = PowerCycleReset;
  }

  PchResetPpi->Reset (PchResetPpi, PchResetType);
  ///
  /// Shouldn't reach here
  ///
  return EFI_SUCCESS;
}



/**
  Internal function performing miscellaneous init needed in early PEI phase

  @param[in] PchPlatformPolicyPpi The PCH Platform Policy PPI instance

  @retval EFI_SUCCESS             Succeeds.
  @retval EFI_DEVICE_ERROR        Device error, aborts abnormally.
**/
EFI_STATUS
PchMiscInit (
  IN  PCH_PLATFORM_POLICY_PPI           *PchPlatformPolicyPpi
  )
{
  UINT8                 Index;
  UINTN                 PciD31F2RegBase;
  UINTN                 PciD31F5RegBase;
  UINTN                 PciD28F0RegBase;
  UINTN                 PciD31F0RegBase;
  UINT16                LpcDeviceId;
  PCH_HPET_CONFIG       *HpetConfig;
  UINT16                Data16;
  UINT32                Data32;
  UINT32                Data32And;
  UINT32                Data32Or;
  UINTN                 RPBase;
#ifdef PCH_SERVER_BIOS_FLAG
  UINTN                 PciD17F4RegBase;
#endif //PCH_SERVER_BIOS_FLAG

  BOOLEAN       LinkTrainFlag;
  UINT8         PchMaxPciePortNum;
  UINT16        UnCoreBusNo;
  UINT32        ScratchPad1Address;
  UINT32        BiosScratchPad1;

  const USB_CONTROLLER  EhciControllersMap[PchEhciControllerMax] = {
    {
      PCI_DEVICE_NUMBER_PCH_USB,
      PCI_FUNCTION_NUMBER_PCH_EHCI
    },
    {
      PCI_DEVICE_NUMBER_PCH_USB_EXT,
      PCI_FUNCTION_NUMBER_PCH_EHCI2
    }
  };
  PCH_SERIES      PchSeries;
  LinkTrainFlag = FALSE;
  PchSeries       = GetPchSeries();
  PciD31F0RegBase = MmPciAddress (
                      0,
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_LPC,
                      PCI_FUNCTION_NUMBER_PCH_LPC,
                      0
                      );
  PciD31F2RegBase = MmPciAddress (
                      0,
                      PchPlatformPolicyPpi->BusNumber,
                      PCI_DEVICE_NUMBER_PCH_SATA,
                      PCI_FUNCTION_NUMBER_PCH_SATA,
                      0
                      );
#ifdef PCH_SERVER_BIOS_FLAG
  PciD17F4RegBase = MmPciAddress (0, 
                                  PchPlatformPolicyPpi->BusNumber, 
                                  PCI_DEVICE_NUMBER_EVA,
                                  PCI_FUNCTION_NUMBER_EVA_SSATA,
                                  0);
#endif //PCH_SERVER_BIOS_FLAG
  PciD31F5RegBase = 0;
  if (PchSeries == PchH) {
    PciD31F5RegBase = MmPciAddress (
                        0,
                        DEFAULT_PCI_BUS_NUMBER_PCH,
                        PCI_DEVICE_NUMBER_PCH_SATA,
                        PCI_FUNCTION_NUMBER_PCH_SATA2,
                        0
                        );
  }
  PciD28F0RegBase = MmPciAddress (
                      0,
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS,
                      PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1,
                      0
                      );
  HpetConfig = PchPlatformPolicyPpi->HpetConfig;

  UnCoreBusNo = (UINT16)AsmReadMsr64(MSR_CPU_BUS_NUMBER);
  UnCoreBusNo >>= 8;
  ScratchPad1Address = MmPciAddress (
                      0,
                      UnCoreBusNo,
                      UBOX_SCRATCH_DEVICE,
                      UBOX_SCRATCH_FUNC7,
                      UBOX_BIOS_SCRATCHPAD1
                      );
  BiosScratchPad1 = MmioRead32(ScratchPad1Address);
  ///
  /// Set B0:D31:F0 + ACh[20] = 0 at early boot
  ///
  MmioAnd32 (PciD31F0RegBase + R_PCH_LPC_PMIR, (UINT32)~(B_PCH_LPC_PMIR_CF9GR));
  PchMaxPciePortNum = GetPchMaxPciePortNum ();

  for (Index = 0; Index < PchMaxPciePortNum; Index++) {
    RPBase = MmPciAddress (
              0,
              PchPlatformPolicyPpi->BusNumber,
              PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS,
              Index,
              0
              );
    ///
    /// PCH BIOS Spec Rev 0.5.0, Section 8.14 Additional PCI Express* Programming Steps
    /// NOTE: Detection of Non-Complaint PCI Express Devices in Gen2 Ports
    /// Some non-graphics PCI Express devices do not follow PCI Express Specification and currently report
    /// the incorrect Gen capability or link width. This may cause the improper detection of the card
    /// by the Intel Gen2 PCI Express port.
    /// The following settings may improve the ability of an Intel Gen2 PCI Express port to detect
    /// these non-compliant PCI Express devices.
    /// If BIOS cannot detect or train the device: Set B0:D28:F0~F7 + 70h [3:0]= 1h
    /// Wait 100 ms for link to train up
    /// Please note the above setting is "as-is" as Intel cannot verify all non-compliant devices.
    /// You need to ensure that the workaround works with devices you are planning to use.
    ///
    switch (PchPlatformPolicyPpi->PcieConfig->PcieSpeed[Index]) {
      case PchPcieGen1:
        Data16 = BIT0;
        break;
      case PchPcieGen2:
      case PchPcieAuto:
      default:
        Data16 = BIT1;
        break;
    }
	//
	// Check if Target Link Speed(TLS) matches Platform policy, if not program the TLS as 
	// required by platform policy & set 'LinkTrainFlag'= TRUE to provide necessary delay
	//
    if ((MmioRead16 (RPBase + R_PCH_PCIE_LCTL2) & (UINT16) (B_PCH_PCIE_LCTL2_TLS)) != Data16) {
      MmioAndThenOr16 (RPBase + R_PCH_PCIE_LCTL2, (UINT16)~(B_PCH_PCIE_LCTL2_TLS), Data16);
      if(BiosScratchPad1 & BIT4) {
        LinkTrainFlag = TRUE;
	  }
    }
  }

  if (LinkTrainFlag) {              // Condition to avoid unnecessary delay
    PchPmTimerStall (100 * 1000);	// Common 100ms delay to link train all PCIe root ports.
  }

  for (Index = 0; Index < PchMaxPciePortNum; Index++) {
    RPBase = MmPciAddress (
              0,
              PchPlatformPolicyPpi->BusNumber,
              PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS,
              Index,
              0
              );

    ///
    /// PCH BIOS Spec Rev 0.5.0 Section 8.2
    /// Else if the port is hot-plug enable, do not disable the port. If BIOS wants to disable the port,
    /// BIOS should not enable the hot plug capability or must disable the hot plug capability of the port.
    /// Set B0:D28:Fn + 338h [26] = 0b at early POST.
    ///
    MmioAnd32 ((RPBase + 0x338), (UINT32) ~BIT26);

    ///
    /// PCH BIOS Spec Rev 0.5.0, Section 8.14 Additional PCI Express* Programming Steps
    /// Step 1
    /// Before MRC execution, system BIOS must program the following register.
    /// B0:D28:F0 + F4h[6:5] = 0b
    /// B0:D28:F0 + F4h[7] = 1b
    ///
    if (Index == 0) {
      MmioAndThenOr8 ((RPBase + 0xF4), (UINT8) ~(BIT5 | BIT6), BIT7);
    }
  }

  for (Index = 0; Index < GetPchEhciMaxControllerNum (); Index++) {
    ///
    /// PCH BIOS Spec Rev 0.5.0 Section 7.1.5 Additional PCH DMI Programming Steps
    /// Step 3.2
    /// RCBA + Offset 50h[23:20] = 2h and RCBA + Offset 50h[17] = 1b (Done at
    /// PchDmiTcVcProgPoll() on PchDmiPeim.c)
    /// and also ensure that D29/D26:F0:88h [2] = 0b
    ///
    Data32 = MmioRead32 (
              (UINTN) MmPciAddress (0,
              PchPlatformPolicyPpi->BusNumber,
              EhciControllersMap[Index].Device,
              EhciControllersMap[Index].Function,
              0x88)
              );
    Data32 &= (UINT32) (~BIT2);
    MmioWrite32 (
      MmPciAddress (0,
      PchPlatformPolicyPpi->BusNumber,
      EhciControllersMap[Index].Device,
      EhciControllersMap[Index].Function,
      0x88),
      Data32
      );
  }
  ///
  /// Initial and enable HPET High Precision Timer memory address for basic usage
  ///
  if (HpetConfig->Enable == PCH_DEVICE_ENABLE) {
    MmioAndThenOr32 (
      (UINTN) (PchPlatformPolicyPpi->Rcba + R_PCH_RCRB_HPTC),
      (UINT32)~B_PCH_RCRB_HPTC_AS,
      (UINT32) (((HpetConfig->Base >> N_PCH_HPET_ADDR_ASEL) & B_PCH_RCRB_HPTC_AS) | B_PCH_RCRB_HPTC_AE)
      );
    ///
    /// Read back for posted write to take effect
    ///
    MmioRead32 (PchPlatformPolicyPpi->Rcba + R_PCH_RCRB_HPTC);
    ///
    /// Set HPET Timer enable to start counter spinning
    ///
    MmioOr32 (HpetConfig->Base + 0x10, 0x1);
  }

  if (PchPlatformPolicyPpi->Port80Route == PchReservedPageToLpc) {
    MmioAnd32 (PchPlatformPolicyPpi->Rcba + R_PCH_RCRB_GCS, (UINT32) (~B_PCH_RCRB_GCS_RPR));
  } else {
    MmioOr32 (PchPlatformPolicyPpi->Rcba + R_PCH_RCRB_GCS, (UINT32) B_PCH_RCRB_GCS_RPR);
  }
  ///
  /// Read back for posted write to take effect
  ///
  MmioRead32 (PchPlatformPolicyPpi->Rcba + R_PCH_RCRB_GCS);

#ifdef TRAD_FLAG
  if (PchSeries == PchH) {
    if (PchPlatformPolicyPpi->SataConfig->SataMode == PchSataModeIde) {
      ///
      /// PCH BIOS Spec Rev 0.5.0 section 14.1.7 Additional Programming Requirements during
      /// Step 13
      /// When SATA in IDE mode
      /// a. Program D31:F2:34h [7:0] to 70h
      ///
      Data32And = (UINT32) ~(0xFF);
      Data32Or  = (UINT32) (0x70);
      MmioAndThenOr32 (
        (UINTN) (PciD31F2RegBase + 0x34),
        Data32And,
        Data32Or
        );
      ///
      /// b. Program D31:F2:70h [15:8] to 0h
      ///
      Data32And = (UINT32) ~(0xFF00);
      MmioAnd32 (
        (UINTN) (PciD31F2RegBase + 0x70),
        Data32And
        );
      ///
      /// IDE mode, SATA Port 0 - Port 3 are for D31:F2, Port4 and Port 5 are for D31:F5
      ///
      MmioAnd8 (
        PciD31F2RegBase + R_PCH_SATA_MAP,
        (UINT8)~(B_PCH_SATA_MAP_SMS_MASK | B_PCH_SATA_PORT_TO_CONTROLLER_CFG)
        );
      ///
      /// PCH BIOS Spec Rev 0.5.0 section 14.1.7 Additional Programming Requirements during
      /// SATA Initialization
      /// Step 1
      /// If D28:F0:410h[5:4] = 11b, System BIOS must disable D31:F5 by setting SAD2 bit,
      /// RCBA + 3418[25]
      ///
      if ((MmioRead8 (PciD28F0RegBase + 0x410) & (UINT8) (BIT5 | BIT4)) == (UINT8) (BIT5 | BIT4)) {
        MmioOr32 (PchPlatformPolicyPpi->Rcba + R_PCH_RCRB_FUNC_DIS, (UINT32) B_PCH_RCRB_FUNC_DIS_SATA2);
        ///
        /// Reads back for posted write to take effect
        ///
        MmioRead32 (PchPlatformPolicyPpi->Rcba + R_PCH_RCRB_FUNC_DIS);
      } else {
        ///
        /// Enable the SATA port4 and port5.
        /// Step 1.a
        /// If D28:F0:410h[4] = 1b, System BIOS should not enable the SATA port4
        /// Step 1.b
        /// If D28:F0:410h[5] = 1b, System BIOS should not enable the SATA port5
        ///
        if ((MmioRead8 (PciD28F0RegBase + 0x410) & BIT4) == 0) {
          MmioOr8 (
            PciD31F5RegBase + R_PCH_SATA_PCS,
            (UINT8) B_PCH_SATA2_PCS_PORT4_EN
            );
        }
        if ((MmioRead8 (PciD28F0RegBase + 0x410) & BIT5) == 0) {
          MmioOr8 (
            PciD31F5RegBase + R_PCH_SATA_PCS,
            (UINT8) B_PCH_SATA2_PCS_PORT5_EN
            );
        }
      }
    } else {
      MmioOr32 (PchPlatformPolicyPpi->Rcba + R_PCH_RCRB_FUNC_DIS, (UINT32) B_PCH_RCRB_FUNC_DIS_SATA2);
      ///
      /// Reads back for posted write to take effect
      ///
      MmioRead32 (PchPlatformPolicyPpi->Rcba + R_PCH_RCRB_FUNC_DIS);
    }

#ifdef PCH_SERVER_BIOS_FLAG
	if (PchPlatformPolicyPpi->sSataConfig->SataMode == PchSataModeIde) {
      ///
      /// PCH BIOS Spec Rev 0.5.0 section 14.1.7 Additional Programming Requirements during
      /// Step 13
      /// When sSATA in IDE mode
      /// a. Program D17:F4:34h [7:0] to 70h
      ///
      Data32And = (UINT32) ~(0xFF);
      Data32Or  = (UINT32) (0x70);
      MmioAndThenOr32 (
        (UINTN) (PciD17F4RegBase + 0x34),
        Data32And,
        Data32Or
        );
      ///
      /// b. Program D17:F4:70h [15:8] to 0h
      ///
      Data32And = (UINT32) ~(0xFF00);
      MmioAnd32 (
        (UINTN) (PciD17F4RegBase + 0x70),
        Data32And
        );
      ///
      /// IDE mode, SATA Port 0 - Port 3 are for D17:F4
      ///
      MmioAnd8 (
        PciD17F4RegBase + R_PCH_SATA_MAP,
        (UINT8)~(B_PCH_SATA_MAP_SMS_MASK | B_PCH_SATA_PORT_TO_CONTROLLER_CFG)
        );
    } 
#endif //PCH_SERVER_BIOS_FLAG
  }
#endif //TRAD_FLAG
#ifdef ULT_FLAG
  if (PchSeries == PchLp) {
    if (PchPlatformPolicyPpi->SataConfig->SataMode == PchSataModeLoopbackTest) {
      ///
      /// Set D31:F2:90h[7:6] to 00b
      ///
      MmioAnd8 (
        PciD31F2RegBase + R_PCH_SATA_MAP,
        (UINT8)~(B_PCH_SATA_MAP_SMS_MASK)
        );
      ///
      /// Set D31:F2 + SIR Index 00h[15] = 1b
      ///
      MmioWrite8 (PciD31F2RegBase + R_PCH_SATA_SIRI, 0x00);
      Data32And = 0xFFFF7FFF;
      Data32Or  = 0x00008000;
      MmioAndThenOr32 (
        (UINTN) (PciD31F2RegBase + R_PCH_SATA_STRD),
        Data32And,
        Data32Or
        );
    }
  }
#endif // ULT_FLAG
  if (PchPlatformPolicyPpi->SataConfig->SataMode == PchSataModeRaid) {
    LpcDeviceId = MmioRead16 (PciD31F0RegBase + R_PCH_LPC_DEVICE_ID);
    if (IS_PCH_LPT_RAID_AVAILABLE (LpcDeviceId)
#ifdef PCH_SERVER_BIOS_FLAG
        || IS_PCH_WBG_RAID_AVAILABLE (LpcDeviceId)
#endif //PCH_SERVER_BIOS_FLAG
       ) {
      MmioAndThenOr8 (
        PciD31F2RegBase + R_PCH_SATA_MAP,
        (UINT8) (~B_PCH_SATA_MAP_SMS_MASK),
        (UINT8) (V_PCH_SATA_MAP_SMS_RAID)
        );
    } else {
      DEBUG ((DEBUG_INFO, "PCH Device ID : 0x%x\n", LpcDeviceId));
      DEBUG ((DEBUG_ERROR, "This SKU doesn't support RAID feature. Set to AHCI mode.\n"));
    }
  }

  return EFI_SUCCESS;
}

/**
  Perform Thermal Management Support initialization

  @param[in] PchPlatformPolicyPpi The PCH Platform Policy PPI instance

  @retval EFI_SUCCESS             Succeeds.
  @retval EFI_DEVICE_ERROR        Device error, aborts abnormally.
**/
EFI_STATUS
PchThermalInit (
  IN  PCH_PLATFORM_POLICY_PPI     *PchPlatformPolicyPpi
  )
{
  UINTN                         PciD31F6RegBase;
  UINTN                         PciD0F0RegBase;
  UINT32                        ThermalBaseB;
  PCH_MEMORY_THROTTLING         *MemoryThrottling;
  UINT32                        Data32And;
  UINT32                        Data32Or;
  UINT32                        Softstrap15;

  PciD31F6RegBase = MmPciAddress (
                      0,
                      PchPlatformPolicyPpi->BusNumber,
                      PCI_DEVICE_NUMBER_PCH_THERMAL,
                      PCI_FUNCTION_NUMBER_PCH_THERMAL,
                      0
                      );
  PciD0F0RegBase          = MmPciAddress (0, 0, 0, 0, 0);

  MemoryThrottling        = PchPlatformPolicyPpi->ThermalMgmt->MemoryThrottling;
  ThermalBaseB            = PchPlatformPolicyPpi->PlatformData->TempMemBaseAddr;

  ///
  /// D31:F6:Reg 44h[31:0], with a 64-bit BAR for BIOS.
  /// Enable the BAR by setting the SPTYPEN bit, D31:F6:Reg 40h[0].
  ///
  MmioWrite32 (PciD31F6RegBase + R_PCH_THERMAL_TBARB, ThermalBaseB);
  MmioWrite32 (PciD31F6RegBase + R_PCH_THERMAL_TBARBH, 0);
  MmioOr32 (PciD31F6RegBase + R_PCH_THERMAL_TBARB, (UINT32) B_PCH_THERMAL_SPTYPEN);

  ///
  /// PCH BIOS Spec Rev 0.5.0, 17.2 Thermal Subsystem Device Initialization
  /// The System BIOS must perform the following steps to initialize the PCH thermal subsystem device, D31:F6.
  /// Step 1
  /// Enable Thermal Subsystem device by making sure FD.TTD is cleared.
  /// The default value of FD.TTD is cleared.
  ///
  /// Step 2
  /// Optionally program Device 31 Interrupt Pin/Route registers
  /// Left this to platform code
  ///
  /// Step 3
  /// Go through general PCI enumeration and assign standard PCI resource, including TBARB, TBARBH, etc.
  /// Left this to platform code
  ///
  /// Step 4
  /// Initialize relevant Thermal subsystems for the desired features.
  ///

  ///
  /// PCH BIOS Spec Rev 0.5.0, Section 17.3.1 Initializing Lynx Point Thermal Sensors
  /// Step 1
  /// Set various trip points based on the particular usage model.  Note that Cat Trip must always be programmed.
  /// - CTT must be programmed for Cat Trip, CTT must never be changed while the TS enable is set.
  ///   This rule prevents a spurious trip from occurring and causing a system shutdown.
  ///   TSC must then be written to 0x81 to enable the power down and lock the register.
  ///   TSC programming is done in PchPm.c ThermalLockDown()
  /// - TAHV and TAHL may be programmed if the BIOS or driver wish to force a SW notification for PCH temperature
  ///    - If TAHL/TAHV programmed much later in the flow when a driver is loaded, this means that the TS had been
  ///      enabled long before this, the thermal sensor must be disabled when TAHL/TAHV are programmed, and then
  ///      re-enabled.
  ///    - TSPIEN or TSGPEN may be programmed to cause either an interrupt or SMI/SCI.
  ///    - It is recommended that TAHV, TALV, TSPIEN and TSGPEN be left at their default value, unless there is a
  ///      specific usage that requires these to be programmed.
  ///
  MmioWrite16 (ThermalBaseB + R_PCH_TBARB_CTT,
    ((PchPlatformPolicyPpi->ThermalMgmt->ThermalThrottleLevelsPei->Ctt + 50) * 2));

  ///
  /// Step 2
  /// Clear trip status from TSS/TAS. BIOS should write 0xFF to clear any bit that was inadvertently set while programming
  /// the TS. This write of 0xFF should be done before continuing to the next steps.
  ///
  MmioWrite8 (ThermalBaseB + R_PCH_TBARB_TSS, 0xFF);
  MmioWrite8 (ThermalBaseB + R_PCH_TBARB_TAS, 0xFF);

  ///
  /// Step 3
  /// Enable the desired thermal trip alert methods, i.e. GPE (TSGPEN), SMI (TSMIC) or Interrupt (TSPIEN).
  /// Only one of the methods should be enabled and the method will be depending on the platform implementation.
  /// - TSGPEN: BIOS should leave this as default 00h, unless it is required to enable GPE.
  /// - TSMIC: BIOS should leave TSMIC[7:0] as default 00h, unless the SMI handler is loaded
  ///   and it's safe to enable SMI for these events.
  /// - TSPIEN: BIOS should leave this as default 0x00, so that a driver can enable later
  ///
  MmioWrite8 (ThermalBaseB + R_PCH_TBARB_TSGPEN, 0x00);
  MmioWrite8 (ThermalBaseB + R_PCH_TBARB_TSPIEN, 0x00);

  ///
  /// If PCHSTRP15[14] is 1, PMC will set up SML1 for temp reporting to an EC
  ///
  MmioAndThenOr32 (
    PCH_RCRB_BASE + R_PCH_SPI_FDOC,
    (UINT32) (~(B_PCH_SPI_FDOC_FDSS_MASK | B_PCH_SPI_FDOC_FDSI_MASK)),
    (UINT32) (V_PCH_SPI_FDOC_FDSS_PCHS | R_PCH_SPI_STRP15)
    );

  Softstrap15 = MmioRead32 (PCH_RCRB_BASE + R_PCH_SPI_FDOD);

#if !defined(SPS_SUPPORT) || !SPS_SUPPORT
  if ((Softstrap15 & R_PCH_SPI_STRP15_SML1_THRMSEL) != 0)
#endif //SPS_SUPPORT
  {
    ///
    /// Step 4
    /// If thermal reporting to an EC over SMBus is supported, then write 0x01 to TSREL, else leave at default.
    ///
    MmioWrite8 (ThermalBaseB + R_PCH_TBARB_TSREL, 0x01);
  }

  ///
  /// Step 5
  /// If the PCH_Hot pin reporting is supported, then write the temperature value and set the enable in PHL.
  /// Done in PchPm.c ThermalLockDown()
  ///
  /// Step 6
  /// If thermal throttling is supported, then set the desired values in TL.
  /// Done in PchPm.c ThermalLockDown()
  ///
  /// Step 7
  /// Enable thermal sensor by programming TSEL register to 0x01.
  /// Done in PchPm.c ThermalLockDown()
  ///
  /// Step 8
  /// Lock down the thermal reporting to prevent outside agents from changing the values
  /// Done in PchPm.c ThermalLockDown()
  ///

  ///
  /// Clear BAR and disable access
  ///
  MmioAnd32 ((UINTN) (PciD31F6RegBase + R_PCH_THERMAL_TBARB), (UINT32)~B_PCH_THERMAL_SPTYPEN);
  MmioWrite32 ((UINTN) (PciD31F6RegBase + R_PCH_THERMAL_TBARB), 0);

  ///
  /// PCH BIOS Spec Rev 0.5.0, Section 17.5.1 Memory Bandwidth Throttling
  /// If the platform supports an external memory thermal sensor (TS-on-DIMM or TS-on-Board),
  /// system BIOS needs to program the registers bellow.
  /// Here are the settings used in the Intel CRB:
  /// 1. Program RCBA + 33D4h [31:28] = 1100b, for GPIO_D and GPIO_C to PM_SYNC Enable
  /// 2. Program RCBA + 33D4h [15:12] = 1100b, for GPIO_D and GPIO_C C0 Transmit Enable.
  /// 3. Program RCBA + 33C8h [11:8] = 0100b to select GPIO 4 to GPIO_C (EXTTS#0) and
  ///     GPIO 5 to GPIO_D (EXTTS#1)
  ///     GPIOBASE + 00h [5:4] = 11b (Done in platform code)
  ///
  if (MemoryThrottling->Enable == PCH_DEVICE_ENABLE) {
    Data32And = 0x0FFF0FFF;
    Data32Or  = 0;
    if (MemoryThrottling->TsGpioPinSetting[TsGpioC].PmsyncEnable == PCH_DEVICE_ENABLE) {
      Data32Or |= BIT30;
    }

    if (MemoryThrottling->TsGpioPinSetting[TsGpioD].PmsyncEnable == PCH_DEVICE_ENABLE) {
      Data32Or |= BIT31;
    }

    if (MemoryThrottling->TsGpioPinSetting[TsGpioC].C0TransmitEnable == PCH_DEVICE_ENABLE) {
      Data32Or |= BIT14;
    }

    if (MemoryThrottling->TsGpioPinSetting[TsGpioD].C0TransmitEnable == PCH_DEVICE_ENABLE) {
      Data32Or |= BIT15;
    }

    MmioAndThenOr32 (
      PchPlatformPolicyPpi->Rcba + R_PCH_RCRB_CIR22,
      Data32And,
      Data32Or
      );

    Data32And = 0xFFFFF0FF;
    Data32Or  = 0;
    if (MemoryThrottling->TsGpioPinSetting[TsGpioC].PinSelection == 1) {
      Data32Or |= B_PCH_RCRB_PMSYNC_GPIO_C_SEL;
    }

    if (MemoryThrottling->TsGpioPinSetting[TsGpioD].PinSelection == 1) {
      Data32Or |= B_PCH_RCRB_PMSYNC_GPIO_D_SEL;
    }

    MmioAndThenOr32 (
      PchPlatformPolicyPpi->Rcba + R_PCH_RCRB_PMSYNC,
      Data32And,
      Data32Or
      );
  }

  return EFI_SUCCESS;
}

/**
  Initialize IOAPIC according to IoApicConfig policy of the PCH
  Platform Policy PPI

  @param[in] PchPlatformPolicyPpi The PCH Platform Policy PPI instance

  @retval EFI_SUCCESS             Succeeds.
  @retval EFI_DEVICE_ERROR        Device error, aborts abnormally.
**/
EFI_STATUS
PchIoApicInit (
  IN  PCH_PLATFORM_POLICY_PPI     *PchPlatformPolicyPpi
  )
{
  UINT32            RootComplexBar;
  PCH_IOAPIC_CONFIG *IoApicConfig;
  UINT32            IoApicAddress;
  UINT32            IoApicId;

  RootComplexBar  = PchPlatformPolicyPpi->Rcba;
  IoApicConfig    = PchPlatformPolicyPpi->IoApicConfig;

  if (IoApicConfig->ApicRangeSelect != MmioRead8 (RootComplexBar + R_PCH_RCRB_OIC)) {
    ///
    /// Program APIC Range Select bits that define address bits 19:12 for the IOxAPIC range.
    /// This value must not be changed unless the IOxAPIC Enable bit is cleared.
    ///
    MmioAnd16 ((UINTN) (RootComplexBar + R_PCH_RCRB_OIC), (UINT16)~(B_PCH_RCRB_OIC_AEN));
    ///
    /// Program APIC Range Select bits at RCBA + 31FEh[7:0]
    ///
    MmioAndThenOr16 (
      RootComplexBar + R_PCH_RCRB_OIC,
      (UINT16)~(V_PCH_RCRB_OIC_ASEL),
      (UINT16) IoApicConfig->ApicRangeSelect
      );
  }
  ///
  /// PCH BIOS Spec Rev 0.5.0, Section 6.6.2.1
  /// 1. Enable the IOAPIC by setting the APIC Enable bit, RCBA + offset 31FFh, Bit[0] if the
  ///    system needs to use the IOxAPIC. The APIC Enable bits needs read back after the bit is written.
  ///
  MmioOr16 ((UINTN) (RootComplexBar + R_PCH_RCRB_OIC), (UINT16) B_PCH_RCRB_OIC_AEN);
  ///
  /// Reads back for posted write to take effect
  ///
  MmioRead16 (RootComplexBar + R_PCH_RCRB_OIC);

  ///
  /// Get current IO APIC ID
  ///
  IoApicAddress = (UINT32) (MmioRead8 (RootComplexBar + R_PCH_RCRB_OIC) << N_PCH_IO_APIC_ASEL);
  MmioWrite8 ((UINTN) (R_PCH_IO_APIC_INDEX | IoApicAddress), 0);
  IoApicId = MmioRead32 ((UINTN) (R_PCH_IO_APIC_DATA | IoApicAddress)) >> 24;
  ///
  /// IO APIC ID is at APIC Identification Register [27:24]
  ///
  if ((IoApicConfig->IoApicId != IoApicId) && (IoApicConfig->IoApicId < 0x10)) {
    ///
    /// Program APIC ID
    ///
    MmioWrite8 ((UINTN) (R_PCH_IO_APIC_INDEX | IoApicAddress), 0);
    MmioWrite32 ((UINTN) (R_PCH_IO_APIC_DATA | IoApicAddress), (UINT32) (IoApicConfig->IoApicId << 24));
  }

  if (GetPchSeries() == PchLp) {
    if (IoApicConfig->IoApicEntry24_39 == PCH_DEVICE_DISABLE) {
      ///
      /// Program IOAPIC Entry 24-39 Disable bit at RCBA + 31FEh[11]
      ///
      MmioOr16 (RootComplexBar + R_PCH_RCRB_OIC, (UINT16) B_PCH_RCRB_OIC_OA24_39_D);
    }
  }

  return EFI_SUCCESS;
}

/**
  This function performs basic initialization for PCH in PEI phase.
  If any of the base address arguments is zero, this function will disable the corresponding
  decoding, otherwise this function will enable the decoding.
  This function locks down the PMBase.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The memory discovered PPI.  Not used.

  @retval EFI_SUCCESS             Succeeds.
  @retval EFI_DEVICE_ERROR        Device error, aborts abnormally.
**/
EFI_STATUS
EFIAPI
PchInitialize (
  IN CONST EFI_PEI_SERVICES             **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN VOID                         *Ppi
  )
{
  EFI_STATUS              Status;
//  UINT8                   Data8Or; //AptioV server override
//  UINT8                   Data8And; //AptioV server override
  PCH_PLATFORM_POLICY_PPI *PchPlatformPolicyPpi;
#ifdef USB_PRECONDITION_ENABLE_FLAG
  EFI_BOOT_MODE           BootMode;
#endif  // USB_PRECONDITION_ENABLE_FLAG
  UINTN                   AcpiBarAddress;
  UINTN                   GpioBarAddress;
  
  //AptioV server override start: Avoid redundant entry post memory discovery when gPchPlatformPolicyPpiGuid is re-installed.
  Status = (**PeiServices).LocatePpi (
                            PeiServices,
                            &gPchPeiInitDonePpiGuid,
                            0,
                            NULL,
                            NULL
                            );  

  if (!EFI_ERROR(Status)){
	return EFI_SUCCESS;  
  }
  //AptioV server override end: Avoid redundant entry post memory discovery when gPchPlatformPolicyPpiGuid is re-installed.
  
  DEBUG ((DEBUG_INFO, "PchInitialize() - Start\n"));

  ///
  /// Get platform policy settings through the PchPlatformPolicy PPI
  ///
  Status = (**PeiServices).LocatePpi (
                            PeiServices,
                            &gPchPlatformPolicyPpiGuid,
                            0,
                            NULL,
                            (VOID **)&PchPlatformPolicyPpi
                            );
  ASSERT_EFI_ERROR (Status);
  ///
  /// Set Rcba
  ///
  ASSERT ((PchPlatformPolicyPpi->Rcba & (UINT32) (~B_PCH_LPC_RCBA_BAR)) == 0);
  MmioAndThenOr32 (
    MmPciAddress (0,
    PchPlatformPolicyPpi->BusNumber,
    PCI_DEVICE_NUMBER_PCH_LPC,
    PCI_FUNCTION_NUMBER_PCH_LPC,
    R_PCH_LPC_RCBA),
    (UINT32) (~B_PCH_LPC_RCBA_BAR),
    PchPlatformPolicyPpi->Rcba | B_PCH_LPC_RCBA_EN
    );

  ///
  /// Set PM Base
  ///
  AcpiBarAddress =  MmPciAddress (0,
    PchPlatformPolicyPpi->BusNumber,
    PCI_DEVICE_NUMBER_PCH_LPC,
    PCI_FUNCTION_NUMBER_PCH_LPC,
    R_PCH_LPC_ACPI_BASE
    );
  MmioWrite32 (AcpiBarAddress, PchPlatformPolicyPpi->PmBase);
  ASSERT ((MmioRead32 (AcpiBarAddress) & B_PCH_LPC_ACPI_BASE_BAR) == PchPlatformPolicyPpi->PmBase);
  if (PchPlatformPolicyPpi->PmBase != 0) {
    ///
    /// Enable PM Base
    ///
    MmioOr8 (
      MmPciAddress (0,
      PchPlatformPolicyPpi->BusNumber,
      PCI_DEVICE_NUMBER_PCH_LPC,
      PCI_FUNCTION_NUMBER_PCH_LPC,
      R_PCH_LPC_ACPI_CNT),
      (UINT8) B_PCH_LPC_ACPI_CNT_ACPI_EN
      );
  } else {
    ///
    /// Disable PM Base
    ///
    MmioAnd8 (
      MmPciAddress (0,
      PchPlatformPolicyPpi->BusNumber,
      PCI_DEVICE_NUMBER_PCH_LPC,
      PCI_FUNCTION_NUMBER_PCH_LPC,
      R_PCH_LPC_ACPI_CNT),
      (UINT8) (~B_PCH_LPC_ACPI_CNT_ACPI_EN)
      );
  }
  ///
  /// Lock down the PM Base
  ///
  MmioOr8 (
    MmPciAddress (0,
    PchPlatformPolicyPpi->BusNumber,
    PCI_DEVICE_NUMBER_PCH_LPC,
    PCI_FUNCTION_NUMBER_PCH_LPC,
    R_PCH_LPC_GEN_PMCON_LOCK),
    (UINT8) (B_PCH_LPC_GEN_PMCON_LOCK_ABASE_LK)
    );

  ///
  /// Set GPIO Base
  ///
  GpioBarAddress = MmPciAddress (0,
    PchPlatformPolicyPpi->BusNumber,
    PCI_DEVICE_NUMBER_PCH_LPC,
    PCI_FUNCTION_NUMBER_PCH_LPC,
    R_PCH_LPC_GPIO_BASE
    );
  MmioWrite32 (GpioBarAddress, PchPlatformPolicyPpi->GpioBase);
  ASSERT ((MmioRead32 (GpioBarAddress) & B_PCH_LPC_GPIO_BASE_BAR) == PchPlatformPolicyPpi->GpioBase);
  if (PchPlatformPolicyPpi->GpioBase != 0) {
    ///
    /// Enable GPIO Base
    ///
    MmioOr8 (
      MmPciAddress (0,
      PchPlatformPolicyPpi->BusNumber,
      PCI_DEVICE_NUMBER_PCH_LPC,
      PCI_FUNCTION_NUMBER_PCH_LPC,
      R_PCH_LPC_GPIO_CNT),
      (UINT8) B_PCH_LPC_GPIO_CNT_GPIO_EN
      );
  } else {
    ///
    /// Disable GPIO Base
    ///
    MmioAnd8 (
      MmPciAddress (0,
      PchPlatformPolicyPpi->BusNumber,
      PCI_DEVICE_NUMBER_PCH_LPC,
      PCI_FUNCTION_NUMBER_PCH_LPC,
      R_PCH_LPC_GPIO_CNT),
      (UINT8) (~B_PCH_LPC_GPIO_CNT_GPIO_EN)
      );
  }

  /* AptioV server override
    if (PchPlatformPolicyPpi->PlatformData->SmmBwp == 0) {
      ///
      /// Clear SMM_BWP bit (D31:F0:RegDCh[5])
      ///
      Data8And  = (UINT8) ~B_PCH_LPC_BIOS_CNTL_SMM_BWP;
      Data8Or   = 0x00;
    } else {
      ///
      /// Set SMM_BWP and BLE bit (D31:F0:RegDCh[5][1])
      ///
      Data8And  = 0xFF;
      Data8Or   = (UINT8) (B_PCH_LPC_BIOS_CNTL_SMM_BWP + B_PCH_LPC_BIOS_CNTL_BLE);
    }

    MmioAndThenOr8 (
      MmPciAddress (0,
      PchPlatformPolicyPpi->BusNumber,
      PCI_DEVICE_NUMBER_PCH_LPC,
      PCI_FUNCTION_NUMBER_PCH_LPC,
      R_PCH_LPC_BIOS_CNTL),
      Data8And,
      Data8Or
      );
  */ //AptioV server override

  ///
  /// Perform SATA init needed in PEI phase
  /// Note: Rcba needs to be programmed before here.
  ///
  Status = PchSataInit (PeiServices, PchPlatformPolicyPpi);
  ASSERT_EFI_ERROR (Status);

#ifdef PCH_SERVER_BIOS_FLAG
  ///
  /// Perform EVA sSata SATA init needed in PEI phase
  /// Note: Rcba needs to be programmed before here.
  ///
  if(GetIsPchsSataPresent() == TRUE){
     PchsSataInit (PeiServices, PchPlatformPolicyPpi); 
   }
#endif  //PCH_SERVER_BIOS_FLAG 

  Status = PchDmiMiscProg (PeiServices, PchPlatformPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  Status = PchGbeMandatedReset (PeiServices, PchPlatformPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  Status = PchMiscInit (PchPlatformPolicyPpi);

#ifdef PCH_SERVER_BIOS_FLAG
  if (GetIsPchsSataPresent() == TRUE){
      Status = PchEvaMiscInit (PeiServices, PchPlatformPolicyPpi); 
  }
#endif //PCH_SERVER_BIOS_FLAG
  ASSERT_EFI_ERROR (Status);
  Status = PchThermalInit (PchPlatformPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  Status = PchIoApicInit (PchPlatformPolicyPpi);
  ASSERT_EFI_ERROR (Status);

#ifdef USB_PRECONDITION_ENABLE_FLAG
  if (PchPlatformPolicyPpi->Revision > PCH_PLATFORM_POLICY_PPI_REVISION_1) {
    ///
    /// If it is in S3 boot path or recovery mode, do nothing
    ///
    Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
    if (!EFI_ERROR (Status)) {
      if ((BootMode != BOOT_ON_S3_RESUME) && (BootMode != BOOT_IN_RECOVERY_MODE)) {
        if (PchPlatformPolicyPpi->UsbConfig->UsbPrecondition) {

          ///
          /// Initialize PCH USB when USB_PRECONDITION feature is enabled by USB_CONFIG policy
          /// Initialize PCH EHCI and XHCI by the same MMIO resource one by one
          ///
          Status = PchStartUsbInit (
                    PchPlatformPolicyPpi->UsbConfig,
                    PchPlatformPolicyPpi->PlatformData->TempMemBaseAddr,
                    PchPlatformPolicyPpi->PlatformData->TempMemBaseAddr,
                    PchPlatformPolicyPpi->Revision
                    );
          ASSERT_EFI_ERROR (Status);
        }
      }
    }
  }
#endif  // USB_PRECONDITION_ENABLE_FLAG
  DEBUG ((DEBUG_INFO, "PchInitialize() - End\n"));

  ///
  /// Install the PCH PEI Init Done PPI
  ///
  Status = (**PeiServices).InstallPpi (PeiServices, &mPpiPchPeiInitDone);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Internal function performing miscellaneous init needed in very early PEI phase

  @param[in] RootComplexBar       RootComplexBar address of this PCH device

  @retval None
**/
VOID
PchMiscEarlyInit (
  IN  UINT32                      RootComplexBar
  )
{
  UINTN                   PciD31F0RegBase;
  UINT8                   Nmi;
  UINT8                   Data8;

  DEBUG ((DEBUG_INFO, "PchMiscEarlyInit() - Start\n"));

  ///
  /// PCH BIOS Spec Rev 0.5.0, Section 19.3 Power Failure Considerations
  /// RTC_PWR_STS bit, GEN_PMCON_3 (D31:F0:A4h[2])
  /// When the RTC_PWR_STS bit is set, it indicates that the RTCRST# signal went low.
  /// Software should clear this bit. For example, changing the RTC battery sets this bit.
  /// System BIOS should reset CMOS to default values if the RTC_PWR_STS bit is set.
  /// The System BIOS should execute the sequence below if the RTC_PWR_STS bit is set
  /// before memory initialization. This will ensure that the RTC state machine has been
  /// initialized.
  /// 1. If the RTC_PWR_STS bit is set which indicates a new coin-cell battery insertion or a
  ///    battery failure, steps 2 through 5 should be executed.
  /// 2. Set RTC Register 0Ah[6:4] to 110b or 111b
  /// 3. Set RTC Register 0Bh[7].
  /// 4. Set RTC Register 0Ah[6:4] to 010b
  /// 5. Clear RTC Register 0Bh[7].
  ///
  PciD31F0RegBase = MmPciAddress (
                      0,
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_LPC,
                      PCI_FUNCTION_NUMBER_PCH_LPC,
                      0
                      );

  if ((MmioRead16 (PciD31F0RegBase + R_PCH_LPC_GEN_PMCON_3) &
      (UINT16) B_PCH_LPC_GEN_PMCON_RTC_PWR_STS) != 0) {
    ///
    /// Enable Alternate Access Mode
    /// Note: The RTC Index field (including the NMI mask at bit7) is write-only
    /// for normal operation and can only be read in Alt Access Mode.
    ///
    PchAlternateAccessMode (RootComplexBar, TRUE);
    ///
    /// Read NMI Enable bit
    ///
    Nmi = IoRead8 (R_PCH_NMI_EN) & (UINT8) B_PCH_NMI_EN_NMI_EN;
    ///
    /// Disable Alternate Access Mode
    ///
    PchAlternateAccessMode (RootComplexBar, FALSE);
    ///
    /// 2. Set RTC Register 0Ah[6:4] to 110b or 111b
    ///
    IoWrite8 (R_PCH_RTC_INDEX, (UINT8) (R_PCH_RTC_REGA | Nmi));
    Data8 = IoRead8 (R_PCH_RTC_TARGET) & (UINT8) ~(BIT6 | BIT5 | BIT4);
    Data8 |= (UINT8) (BIT6 | BIT5);
    IoWrite8 (R_PCH_RTC_TARGET, Data8);
    ///
    /// 3. Set RTC Register 0Bh[7].
    ///
    IoWrite8 (R_PCH_RTC_INDEX, (UINT8) (R_PCH_RTC_REGB | Nmi));
    IoOr8 (R_PCH_RTC_TARGET, (UINT8) B_PCH_RTC_REGB_SET);
    ///
    /// 4. Set RTC Register 0Ah[6:4] to 010b
    ///
    IoWrite8 (R_PCH_RTC_INDEX, (UINT8) (R_PCH_RTC_REGA | Nmi));
    Data8 = IoRead8 (R_PCH_RTC_TARGET) & (UINT8) ~(BIT6 | BIT5 | BIT4);
    Data8 |= (UINT8) (BIT5);
    IoWrite8 (R_PCH_RTC_TARGET, Data8);
    ///
    /// 5. Clear RTC Register 0Bh[7].
    ///
    IoWrite8 (R_PCH_RTC_INDEX, (UINT8) (R_PCH_RTC_REGB | Nmi));
    IoAnd8 (R_PCH_RTC_TARGET, (UINT8) ~B_PCH_RTC_REGB_SET);
  }
  ///
  /// PCH BIOS Spec Rev 0.5.0, Section 19.1 Handling Status Registers
  /// System BIOS must set 1b to clear the following registers during power-on
  /// and resuming from Sx sleep state.
  /// - RCBA + Offset 3310h[0] = 1b
  ///   Done in ConfigureMiscItems ()
  /// - RCBA + Offset 3310h[4] = 1b, needs to be done as early as possible during PEI
  /// - RCBA + Offset 3310h[5] = 1b
  ///   Done in ConfigureMiscItems ()
  ///
  MmioWrite32 (
    (UINTN) (RootComplexBar + R_PCH_RCRB_PRSTS),
    (UINT32) (B_PCH_RCRB_PRSTS_FIELD_1)
    );

  DEBUG ((DEBUG_INFO, "PchMiscEarlyInit() - End\n"));

  return;
}

///
/// Entry point
///

/**
  Installs the PCH PEI Init PPI
  Performing Pch early init after PchPlatfromPolicy PPI produced

  @param[in] FfsHeader            Not used.
  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval EFI_SUCCESS             The function completes successfully
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create database
**/
EFI_STATUS
EFIAPI
InstallPchInitPpi (
  IN EFI_FFS_FILE_HEADER       *FfsHeader,
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
{
  EFI_STATUS              Status;
  UINT32                  RootComplexBar;
  EFI_PEI_PPI_DESCRIPTOR  *PchDmiTcVcMapPpiDesc;
  PCH_DMI_TC_VC_PPI       *PchDmiTcVcMapPpi;
#ifdef DYNAMIC_MMCFG_BASE_FLAG 
  UINT32                SecpciExbase;   //AptioV server overrride: Dynamic mmcfg base address change
  
  
//AptioV server overrride start: Dynamic mmcfg base address change
  IoWrite32 (0xCF8, 0x80000000 | (CORE05_BUS_NUM << 16) + (CORE05_DEV_NUM << 11) + (CORE05_FUNC_NUM << 8) + R_IIO_MMCFG_B0);
  SecpciExbase = IoRead32 (0xCFC) & 0xFFFFFFF0; 
  PcdSet64 (PcdPciExpressBaseAddress, (UINT64) SecpciExbase);
#endif  
//AptioV server overrride end: Dynamic mmcfg base address change
DEBUG ((DEBUG_INFO, "InstallPchInitPpi() - Start\n"));
  ///
  /// Check if Rcba has been set
  ///
  RootComplexBar = PCH_RCRB_BASE;
  DEBUG ((DEBUG_INFO, "Rcba needs to be programmed before here\n"));
  ASSERT ((RootComplexBar & (UINT32) (~B_PCH_LPC_RCBA_BAR)) == 0);
  ///
  /// Perform miscellaneous init needed in very early PEI phase
  ///
  PchMiscEarlyInit (RootComplexBar);
  
  ///
  /// Install the DMI TC VC PPI
  /// Allocate descriptor and PPI structures.  Since these are dynamically updated
  ///
  PchDmiTcVcMapPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (PchDmiTcVcMapPpiDesc == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate memory for PchDmiTcVcMapPpiDesc! \n"));
    return EFI_OUT_OF_RESOURCES;
  }

  PchDmiTcVcMapPpi = (PCH_DMI_TC_VC_PPI *) AllocateZeroPool (sizeof (PCH_DMI_TC_VC_PPI));
  if (PchDmiTcVcMapPpi == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate memory for PchDmiTcVcMapPpi! \n"));
// Grangeville AptioV server override - Cpp Check
    FreePool(PchDmiTcVcMapPpiDesc);  
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (PchDmiTcVcMapPpi, &mPchDmiTcVcMap, sizeof (PCH_DMI_TC_VC_PPI));

  PchDmiTcVcMapPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  PchDmiTcVcMapPpiDesc->Guid  = &gPchDmiTcVcMapPpiGuid;
  PchDmiTcVcMapPpiDesc->Ppi   = PchDmiTcVcMapPpi;
  Status                      = (**PeiServices).InstallPpi (PeiServices, PchDmiTcVcMapPpiDesc);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Install the PCH PEI Init PPI
  ///
  Status = (**PeiServices).InstallPpi (PeiServices, &mPpiListVariable);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Performing Pch early init after PchPlatfromPolicy PPI produced
  ///
  Status = (**PeiServices).NotifyPpi (PeiServices, &mNotifyList);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "InstallPchInitPpi() - End\n"));

  return Status;
}

