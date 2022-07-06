/** @file
  The PCH Init PEIM implements the EVA PEI Init PPI.

@copyright
  Copyright (c) 2004 - 2012 Intel Corporation. All rights reserved
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

#include "PchInitPeim.h"
#include "PchHSIO.h"
#ifdef SDP_FLAG
#include <Library/TimerLib.h>
#endif

/**
   Internal function perdorming EVA's xSATA init needed in PEI phase. 
   
   @param [in] PeiServices           General purpose services available to every PEIM. 
   @param [in] PchPlatformPolicyPpi  Platform policy PPI pointer. 
   
   @retval None
  
**/
VOID
PchsSataInit (
  IN  CONST EFI_PEI_SERVICES            **PeiServices,
  IN  PCH_PLATFORM_POLICY_PPI           *PchPlatformPolicyPpi  
  )
{
   
  UINT32      Data32And;
  UINT32      Data32Or;
  
  UINTN       PciD17F4RegBase;
  BOOLEAN     SkipSataInit;
  UINT16      i;
  UINT16      size;
  UINT32      RootComplexBar;
  EFI_STATUS  Status;
  PCH_SERIES  PchSeries;
  UINT32      PchSataTraceId;  
  UINT16      GSpeed;
  UINT16      PortId;
  UINT8       RxEq;
  UINT32      OrMask;
  IOBP_MMIO_TABLE_STRUCT *PchsSataHSIO;
  IOBP_MMIO_TABLE_STRUCT *PchsSataHSIO_DT;
  IOBP_MMIO_TABLE_STRUCT *PchsSataSharedHSIO;
  IOBP_MMIO_TABLE_STRUCT *PchsSataSharedHSIO_DT;
  IOBP_SATA_RXEQ_TABLE  *PchSataRxEqHsio;  
#ifdef SDP_FLAG
  UINTN       CPUUnCoreBusNo;
  UINTN       BIOSScratchPad0Address;
  UINT8       BIOSScratchPad0;
#endif

  DEBUG ((DEBUG_INFO, "PchsSataInit() - Start\n"));

  PchSeries       = GetPchSeries();
  RootComplexBar  = PCH_RCRB_BASE;
  PciD17F4RegBase = MmPciAddress (
                      0,
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_EVA,
                      PCI_FUNCTION_NUMBER_EVA_SSATA,
                      0
                      );
  SkipSataInit = FALSE;
  

  if ((MmioRead8(PciD17F4RegBase + R_PCH_SATA_PCS) & (UINT8) (B_PCH_SATA_PCS_PORT3_EN |
                                                              B_PCH_SATA_PCS_PORT2_EN |
                                                              B_PCH_SATA_PCS_PORT1_EN |
                                                              B_PCH_SATA_PCS_PORT0_EN)) != 0) {
    SkipSataInit = TRUE;
  }
  ///
  /// Skip SATA init if SATA port4 or port5 is enabled
  ///
  if ((MmioRead8 (PciD17F4RegBase + R_PCH_SATA_PCS) & (UINT8) (B_PCH_SATA_PCS_PORT5_EN |
                                                                 B_PCH_SATA_PCS_PORT4_EN)) != 0) {
      SkipSataInit = TRUE;
    }
  
  if (SkipSataInit == TRUE) {

    ///
    /// Any SATA port should not be enabled unless CPU only reset.
    /// The value of 0xEA000AAC[5:4] is 10b after issuing CPU only reset.
    /// Note:
    /// The default value of 0xEA000AAC[5:4] is 00b.
    /// The following "if" condition will need to update while the
    /// BIOS recommended setting of 0xEA000AAC[5:4] is changed.
    /// Asset if any SATA port is enabled before SATA  HSIO initialization is done
    ///
      Status = ReadIobp (RootComplexBar, 0xEA000AAC, &Data32And);
    if ((Data32And & (UINT32) (BIT4 | BIT5)) != 0x20) {
      DEBUG ((DEBUG_ERROR, "Please do not enable any sSATA port before sSATA  HSIO initialization is done.\n"));
      ASSERT (0);
    }
  } else {
    ///
    /// Assume SATA mode will be AHCI, SATA Port 0 - Port 5 are all for D31:F2
    ///
    MmioAndThenOr8 (
      PciD17F4RegBase + R_PCH_SATA_MAP,
      (UINT8) (~B_PCH_SATA_MAP_SMS_MASK),
      (UINT8) (V_PCH_SATA_MAP_SMS_AHCI | B_PCH_SATA_PORT_TO_CONTROLLER_CFG)
      );
    /// Sata 
    ///
    /// PCH BIOS Spec Rev 0.5.0 section 14.1.7 Additional Programming Requirements during
    /// SATA Initialization
    /// Step 2
    /// System BIOS must set D17:F4:Reg 94h[8:0] = 183h as part of the chipset initialization
    /// prior to SATA configuration. These bits should be restored while resuming from a S3
    /// sleep state.
    ///
    Data32And = (UINT32)~(BIT8 | BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
    Data32Or  = 0x183;
    MmioAndThenOr32 (
      (UINTN) (PciD17F4RegBase + R_PCH_SATA_SCLKCG),
      Data32And,
      Data32Or
      );
    ///
    /// Step 3
    /// D17:F4:Reg 92h[15] = 1b
    /// Set OOB Retry Mode bit of Port Control and Status (PCS) register
    /// These bits should be restored while resuming from a S3 sleep state
    ///
    MmioOr16 ((UINTN) (PciD17F4RegBase + R_PCH_SATA_PCS), (UINT16) (B_PCH_SATA_PCS_OOB_RETRY));
    ///
    /// Step 4
    /// System BIOS must program SATA HSIO table as stated in Table 7-7 to 7-8 BEFORE the SATA
    /// ports are enabled.
    ///
    /// PCH BIOS Spec Rev 0.5.6, Section 7.1.5
    /// Step 8
    /// Bios is required to program IOBP setting according to the table 7-7 to 7-8
    /// using 7.1.4 IOSF SBI with OPCODE "PHY Configuration Register".
    /// Table 7-7 SATA dedicated lane setting
    ///
    
    switch (PchStepping()) {
      case LptHB0:
        size = (sizeof (PchsSataHsioWbg_A0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
        PchsSataHSIO = PchsSataHsioWbg_A0;
        break;
      case LptHC0:
      case LptHC1:         
        size = (sizeof (PchSsataHsioWbg_B0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
        PchsSataHSIO = PchSsataHsioWbg_B0;
        break;
      default:
        PchsSataHSIO = NULL;
        size = 0;
        DEBUG ((DEBUG_ERROR, "Unsupported PCH Stepping\n"));
    }

    for (i = 0; i < size; i++) {
      Status = ProgramIobp (
                RootComplexBar,
                PchsSataHSIO[i].Address,
                PchsSataHSIO[i].AndMask,
                PchsSataHSIO[i].OrMask
                );
      ASSERT_EFI_ERROR (Status);
    }
    ///
    /// Table 7-8 SATA Shared lane setting
    ///
      switch (PchStepping()) {
      case LptHB0:
        size = (sizeof (PchsSataSharedHsioWbg_A0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
        PchsSataSharedHSIO = PchsSataSharedHsioWbg_A0;
        break;
       case LptHC0:
       case LptHC1:         
        //
        // No defintion is need it for WBG B0 since the values are cover by PchSataHsioWbg_B0 
        // For B1 - we need to check
        // 
        size = 0;
        PchsSataSharedHSIO = NULL;
        break;
      default:
        size = 0;
        PchsSataSharedHSIO = NULL;
        DEBUG ((DEBUG_ERROR, "Unsupported PCH Stepping\n"));
      }
      if(PchsSataSharedHSIO != NULL) {
      for (i = 0; i < size; i++) {
      Status = ProgramIobp (
                RootComplexBar,
                PchsSataSharedHSIO[i].Address,
                PchsSataSharedHSIO[i].AndMask,
                PchsSataSharedHSIO[i].OrMask
                );
      ASSERT_EFI_ERROR (Status);
        }
      }
    ///
    /// For Desktop:
    /// BIOS is required to program IOBP setting according to Table 7-13 and
    /// Table 7-14 using settings in Section 7.1.4 with OPCODE "PHY Configuration Register".
    /// Table 7-13 SATA Dedicated Lane Setting
    ///
    switch (PchStepping()) {
     case LptHB0:
       size = (sizeof (PchsSataHsioWbg_DT_A0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
       PchsSataHSIO_DT = PchsSataHsioWbg_DT_A0;
       break;
     case LptHC0:
     case LptHC1:         
       size = (sizeof (PchSsataHsioWbg_DT_B0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
       PchsSataHSIO_DT = PchSsataHsioWbg_DT_B0;
       break;
     default:
       size = 0;
       PchsSataHSIO_DT = NULL;
       DEBUG ((DEBUG_ERROR, "Unsupported PCH Stepping\n"));
    }

    for (i = 0; i < size; i++) {
     Status = ProgramIobp (
               RootComplexBar,
               PchsSataHSIO_DT[i].Address,
               PchsSataHSIO_DT[i].AndMask,
               PchsSataHSIO_DT[i].OrMask
               );
     ASSERT_EFI_ERROR (Status);
    }
   
    switch (PchStepping()) {
     case LptHB0:
       size = (sizeof (PchsSataSharedHsioWbg_DT_A0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
       PchsSataSharedHSIO_DT = PchsSataSharedHsioWbg_DT_A0;
       break;	  
     case LptHC0:
     case LptHC1:         
       //
       // No defintion is need it for WBG B0 since the values are cover by PchSataHsioWbg_B0 
       // For B1 we need to check
       // 
       size = 0;
       PchsSataSharedHSIO_DT = NULL;
       break;
      default:
       size = 0;
       PchsSataSharedHSIO_DT = NULL;
       DEBUG ((DEBUG_ERROR, "Unsupported PCH Stepping\n"));
    }
//Grangeville AptioV server override start - CPP Check    
    if(PchsSataSharedHSIO_DT!=NULL) {
     for (i = 0; i < size; i++) {
      Status = ProgramIobp (
               RootComplexBar,
               PchsSataSharedHSIO_DT[i].Address,
               PchsSataSharedHSIO_DT[i].AndMask,
               PchsSataSharedHSIO_DT[i].OrMask
               );
      ASSERT_EFI_ERROR (Status);
     }
    }
//Grangeville AptioV server override end - CPP Check
    ///
    /// New in MPHY v41 (modified) sSATA RxEq Dedicated Lane Setting
    /// In response to EV sighting s4986477
    ///
    PchSataTraceId = 0;
    switch (PchStepping()) {
      case LptHC0:
      case LptHC1:
      case LptHC2:
        size = (sizeof (PchsSataRxEqHsioWbg_B0) / sizeof (IOBP_SATA_RXEQ_TABLE));
        PchSataRxEqHsio = PchsSataRxEqHsioWbg_B0;
        break;
      default:
        PchSataRxEqHsio = NULL;
        size = 0;
        DEBUG ((EFI_D_ERROR, "Unsupported PCH Stepping\n"));
    }

    for(PortId = 0; PortId < GetPchMaxsSataPortNum (); PortId++){
      for(GSpeed = 0; GSpeed < 3; GSpeed++){
        if(PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->PortRxEq[PortId].GenSpeed[GSpeed].Enable == PCH_DEVICE_ENABLE) {
          PchSataTraceId = PCH_SATA_RXEQ_ID(PortId, GSpeed);
          for (i = 0; i < size; i++) {
            if(PchSataRxEqHsio[i].TraceId == PchSataTraceId) {
              RxEq = PchPlatformPolicyPpi->sSataConfig->SataTraceConfig->PortRxEq[PortId].GenSpeed[GSpeed].RxEq;
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

   
    ///
    /// Step 6
    /// Program D17:F4:98h[19] = 1b
    ///
    MmioOr32 (
      (UINTN) (PciD17F4RegBase + 0x98),
      (UINT32) (BIT19)
      );
    ///
    /// Step 7
    /// Program D17:F4:98h[12:7] = 04h
    ///
    Data32And = (UINT32) (~(BIT7 | BIT8 | BIT10 | BIT11 | BIT12));
    Data32Or  = (UINT32) (BIT9);
    MmioAndThenOr32 (
      (UINTN) (PciD17F4RegBase + 0x98),
      Data32And,
      Data32Or
      );
    ///
    /// Step 8
    /// Program D17:F4:98h[20] to 1b
    ///
    MmioOr32 ((UINTN) (PciD17F4RegBase + 0x98), (UINT32) (BIT20));
    ///
    /// Step 9
    /// Program D17:F4:98h[6:5] to 01b
    ///
    MmioAndThenOr32 (
      (UINTN) (PciD17F4RegBase + 0x98),
      (UINT32) (~(BIT6 | BIT5)),
      BIT5
      );
    ///
    /// Step 10
    /// Program D17:F4:98h [18] to 1b
    ///
    Data32Or  = (UINT32) (BIT18);
    MmioOr32 (
      (UINTN) (PciD17F4RegBase + 0x98),
      Data32Or
      );
    ///
    /// Step 11
    /// Program D17:F4:98h[29] to 1b
    /// Done in PchInitBeforeBoot()
    ///
    /// Step 12
    /// Program D17:F4:9Ch[5] to 1b (Note: this must be programmed together with D17:F4:9Ch[7:6]
    /// in word write)
    /// Done in ConfigureSata ()
    ///
    /// Step 13
    /// When SATA in IDE mode
    /// a. Program D17:F4:34h [7:0] to 70h
    /// b. Program D17:F4:70h [15:8] to 0h
    /// Done in PchMiscInit ()
    ///
    /// Step 14
    /// Program D17:F4:9Ch[31] to 1b at the End of Post
    /// Done in PchInitBeforeBoot()
    ///
    /// Enable the SATA port0 ~ port5.
    ///
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
    if (!(BIOSScratchPad0 & BIT1)){          // Check for COLD BOOT
      DEBUG ((DEBUG_INFO, "PchsSataInit() - HDD Spin-up 1 Second Delay\n"));
      MicroSecondDelay (STALL_ONE_SECOND);
    }	
    MmioOr8 (
      PciD17F4RegBase + R_PCH_SATA_PCS,
      (UINT8) (B_PCH_SATA_PCS_PORT1_EN | B_PCH_SATA_PCS_PORT0_EN)
    );
    if (!(BIOSScratchPad0 & BIT1)){          // Check for COLD BOOT
      DEBUG ((DEBUG_INFO, "PchsSataInit() - HDD Spin-up 1 Second Delay\n"));
      MicroSecondDelay (STALL_ONE_SECOND);
    }	
    MmioOr8 (
      PciD17F4RegBase + R_PCH_SATA_PCS,
      (UINT8) (B_PCH_SATA_PCS_PORT3_EN | B_PCH_SATA_PCS_PORT2_EN)
    );
#else
    MmioOr8 (
      PciD17F4RegBase + R_PCH_SATA_PCS,
      (UINT8) (B_PCH_SATA_PCS_PORT5_EN | B_PCH_SATA_PCS_PORT4_EN |B_PCH_SATA_PCS_PORT3_EN | B_PCH_SATA_PCS_PORT2_EN | B_PCH_SATA_PCS_PORT1_EN | B_PCH_SATA_PCS_PORT0_EN)
     );
#endif
  }

  DEBUG ((DEBUG_INFO, "PchsSataInit() - End\n"));
}

/**
  Internal function performing miscellaneous init needed in early PEI phase

  @param[in] PchPlatformPolicyPpi The PCH Platform Policy PPI instance

  @retval EFI_SUCCESS             Succeeds.
  @retval EFI_DEVICE_ERROR        Device error, aborts abnormally.
**/
EFI_STATUS
PchEvaMiscInit (
  IN  CONST EFI_PEI_SERVICES                    **PeiServices,
  IN  PCH_PLATFORM_POLICY_PPI           *PchPlatformPolicyPpi
  )
{
  UINT32      Data32And;
  UINT32      Data32Or;
  UINTN       PciD17F4RegBase;
  UINTN       PciD17F0RegBase; 

  /// sSATA eva initializations, mimic the ones that are defined on PchMiscInit in PchInitPeim.c
   PciD17F4RegBase = MmPciAddress (
                      0,
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_EVA,
                      PCI_FUNCTION_NUMBER_EVA_SSATA,
                      0
                      );
  PciD17F0RegBase = MmPciAddress (
                      0,
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_EVA,
                      PCI_FUNCTION_NUMBER_EVA_MROM,
                      0
                      );
  if (PchPlatformPolicyPpi->sSataConfig->SataMode == PchSataModeIde) {
    ///
    /// PCH BIOS Spec Rev 0.5.0 section 14.1.7 Additional Programming Requirements during
    /// Step 13
    /// When SATA in IDE mode
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
    /// IDE mode, SATA Port 0 - Port 3 are for D17:F4, Port4 and Port 5 are for D17:F5 (Not implemented for Wellsburg)
    ///
    MmioAnd8 (
      PciD17F4RegBase + R_PCH_SATA_MAP,
      (UINT8)~(B_PCH_SATA_MAP_SMS_MASK | B_PCH_SATA_PORT_TO_CONTROLLER_CFG)
      );
    
    
  } else {
    /// Hide the secondary port, all ports to be read through AHCI device. 
    MmioOr32 (PciD17F0RegBase + R_PCH_WBG_MSUINT_MSDEVFUNCHIDE, (UINT32) B_PCH_EVA_MSUNIT_MSDEVFUNCHIDE_SSATA2);
    
    if (PchPlatformPolicyPpi->sSataConfig->SataMode == PchSataModeRaid) {
        MmioAndThenOr8 (
          PciD17F4RegBase + R_PCH_SATA_MAP,
          (UINT8) (~B_PCH_SATA_MAP_SMS_MASK),
          (UINT8) (V_PCH_SATA_MAP_SMS_RAID)
          );
      
    }
  }
  return (EFI_SUCCESS); 
}


#endif //PCH_SERVER_BIOS_FLAG

