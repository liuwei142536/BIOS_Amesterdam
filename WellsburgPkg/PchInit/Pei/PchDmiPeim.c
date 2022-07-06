/** @file
  This file contains functions for PCH DMI TC/VC programing and status polling

@copyright
  Copyright (c) 2009 - 2014 Intel Corporation. All rights reserved
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
#include "Library/PchChipsetInitHobLib.h"

/**
  Programing transaction classes of the corresponding virtual channel and Enable it

  @param[in] RootComplexBar       PCH Root Complex Base Address
  @param[in] Vc                   The virtual channel number for programing
  @param[in] VcId                 The Identifier to be used for this virtual channel
  @param[in] VcMap                The transaction classes are mapped to this virtual channel.
                                  When a bit is set, this transaction class is mapped to the virtual channel

  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
PchSetDmiTcVcMapping (
  IN   UINT32           RootComplexBar,
  IN   UINT8            Vc,
  IN   UINT8            VcId,
  IN   UINT8            VcMap
  )
{
  UINTN   Address;
  UINT32  VxCtlAnd;
  UINT32  VxCtlOr;

  Address   = RootComplexBar;

  VxCtlAnd  = (UINT32) (~(B_PCH_RCRB_V1CTL_ID | V_PCH_RCRB_V1CTL_TVM_MASK));
  VxCtlOr   = (VcId << N_PCH_RCRB_V1CTL_ID) & B_PCH_RCRB_V1CTL_ID;
  VxCtlOr |= VcMap;
  VxCtlOr |= B_PCH_RCRB_V1CTL_EN;

  switch (Vc) {
  case DmiVcTypeVc0:
    Address += R_PCH_RCRB_V0CTL;
    break;

  case DmiVcTypeVc1:
    Address += R_PCH_RCRB_V1CTL;
    break;

  case DmiVcTypeVcp:
    Address += R_PCH_RCRB_CIR31;
    break;

  case DmiVcTypeVcm:
    Address += R_PCH_RCRB_CIR32;
    break;

  default:
    return EFI_INVALID_PARAMETER;
  }

  MmioAndThenOr32 (Address, VxCtlAnd, VxCtlOr);
  if ((Vc == DmiVcTypeVc1) || (Vc == DmiVcTypeVcp)) {
    //
    // Reads back for posted write to take effect
    //
    MmioRead32 (Address);
  }

  return EFI_SUCCESS;
}

/**
  Polling negotiation status of the corresponding virtual channel

  @param[in] RootComplexBar       PCH Root Complex Base Address
  @param[in] Vc                   The virtual channel number for programing

  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
PchPollDmiVcStatus (
  IN   UINT32           RootComplexBar,
  IN   UINT8            Vc
  )
{
  UINTN Address;

  Address = RootComplexBar;

  switch (Vc) {
  case DmiVcTypeVc0:
    Address += R_PCH_RCRB_V0STS;
    break;

  case DmiVcTypeVc1:
    Address += R_PCH_RCRB_V1STS;
    break;

  case DmiVcTypeVcp:
    Address += 0x2036;
    break;

  case DmiVcTypeVcm:
    Address += 0x2046;
    break;

  default:
    return EFI_INVALID_PARAMETER;
  }
  //
  // Wait for negotiation to complete
  //
  while ((MmioRead16 (Address) & B_PCH_RCRB_V1STS_NP) != 0);

  return EFI_SUCCESS;
}

/**
  The function performing TC/VC mapping program, and poll all PCH Virtual Channel
  until negotiation completion

  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval EFI_SUCCESS             The function completed successfully
  @retval Others                  All other error conditions encountered result in an ASSERT.
**/
EFI_STATUS
EFIAPI
PchDmiTcVcProgPoll (
  IN  EFI_PEI_SERVICES            **PeiServices
  )
{
  EFI_STATUS              Status;
  PCH_DMI_TC_VC_PPI       *PchDmiTcVcMapPpi;
  UINT32                  RootComplexBar;
  UINT8                   Index;
  UINT8                   VcMap[DmiVcTypeMax] = { 0 };
  ///
  /// Locate PchDmiTcVcMap Ppi
  ///
  Status = (*PeiServices)->LocatePpi (PeiServices, &gPchDmiTcVcMapPpiGuid, 0, NULL, &PchDmiTcVcMapPpi);
  ASSERT_EFI_ERROR (Status);
  RootComplexBar = PCH_RCRB_BASE;

  ///
  /// PCH BIOS Spec Rev 0.5.0, Section 7.1.5
  /// Step 3.1
  /// RCBA + Offset 50h[19] = 1b
  /// Step 3.2
  /// RCBA + Offset 50h[23:20] = 2h and RCBA + Offset 50h[17] = 1b,
  /// ensure that D29/D26:F0:88h [2] = 0b (Done at PchMiscInit() on PchInitPeim.c)
  ///
  MmioAndThenOr32 (RootComplexBar + R_PCH_RCRB_CIR0, (UINT32) (~0x00F00000), (UINT32) (0x00200000));

  if (PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVcp].Enable == PCH_DEVICE_ENABLE) {
    MmioOr32 (RootComplexBar + R_PCH_RCRB_CIR0, BIT17 | BIT19);
  }
  ///
  /// Reads back for posted write to take effect
  ///
  MmioRead32 (RootComplexBar + R_PCH_RCRB_CIR0);

  ///
  /// Step 3.3, Step 3.4, Step 3.5, Step 3,6, Set the TC/VC mappings
  ///
  for (Index = 0; Index < DmiTcTypeMax; Index++) {
    DEBUG ((DEBUG_INFO, "TC:%0x VC:%0x!\n", Index, PchDmiTcVcMapPpi->DmiTc[Index].Vc));
    VcMap[PchDmiTcVcMapPpi->DmiTc[Index].Vc] |= (BIT0 << Index);
  }

  for (Index = 0; Index < DmiVcTypeMax; Index++) {
    DEBUG ((DEBUG_INFO, "VC:%0x VCID:%0x Enable:%0x!\n",Index, PchDmiTcVcMapPpi->DmiVc[Index].VcId, PchDmiTcVcMapPpi->DmiVc[Index].Enable));
    if (PchDmiTcVcMapPpi->DmiVc[Index].Enable == PCH_DEVICE_ENABLE) {
      PchSetDmiTcVcMapping (
        RootComplexBar,
        Index,
        PchDmiTcVcMapPpi->DmiVc[Index].VcId,
        VcMap[Index]
        );
    }
  }

  ///
  /// Step 3.7
  /// Set RCBA + Offset 50h[31] = 1b
  /// Lock down the TC mapping if no further changes are required to bits [30:16]
  ///
    MmioOr32 (RootComplexBar + R_PCH_RCRB_CIR0, B_PCH_RCRB_CIR0_TCLOCKDN);
  ///
  /// Reads back for posted write to take effect
  ///
  MmioRead32 (RootComplexBar + R_PCH_RCRB_CIR0);



  ///
  /// Step 3.8
  /// After both above and System Agent DMI TC/VC mapping are programmed,
  /// poll VC negotiation pending status until is zero:
  /// 3.8.1 RCBA + Offset 201Ah[1]
  /// 3.8.2 RCBA + Offset 2026h[1]
  /// 3.8.3 RCBA + Offset 2036h[1]
  /// 3.8.4 RCBA + Offset 2046h[1]
  ///
  for (Index = 0; Index < DmiVcTypeMax; Index++) {
    if (PchDmiTcVcMapPpi->DmiVc[Index].Enable == PCH_DEVICE_ENABLE) {
      PchPollDmiVcStatus (RootComplexBar, Index);
    }
  }

  return EFI_SUCCESS;
}

//
// ESS: OverrideBegin
//
EFI_STATUS
EFIAPI
PchDmiTcVcProg (
  IN  EFI_PEI_SERVICES            **PeiServices
  )
/*++

Routine Description:

  The function performing TC/VC mapping program, and poll all PCH Virtual Channel 
  until negotiation completion

Arguments:

  PeiServices             General purpose services available to every PEIM.

Returns:

  EFI_SUCCESS             The function completed successfully
  All other error conditions encountered result in an ASSERT.

--*/
{
  EFI_STATUS        Status;
  PCH_DMI_TC_VC_PPI *PchDmiTcVcMapPpi;
  UINT32            RootComplexBar;
  UINT32            PciD31F0RegBase;
  UINT8             Index;
  UINT8             VcMap[DmiVcTypeMax] = { 0 };
  UINT32            Data32And;
  UINT32            Data32Or;
  UINT16            Register;
   
  //
  // Locate PchDmiTcVcMap Ppi
  //
  Status = (*PeiServices)->LocatePpi (PeiServices, &gPchDmiTcVcMapPpiGuid, 0, NULL, &PchDmiTcVcMapPpi);
  ASSERT_EFI_ERROR (Status);

  PciD31F0RegBase         = MmPciAddress (
                               0,
                              DEFAULT_PCI_BUS_NUMBER_PCH,
                              PCI_DEVICE_NUMBER_PCH_LPC,
                              PCI_FUNCTION_NUMBER_PCH_LPC,
                              0
                              );
  
  RootComplexBar = (MmioRead32 (PciD31F0RegBase + R_PCH_LPC_RCBA) & B_PCH_LPC_RCBA_BAR);
  //
  // PCH BIOS Spec Rev 0.5.0, Section 7.1.5
  // Step 4.1
  // RCBA + Offset 50h[11:0] = 654h, Set the recommended TC encoding
  // Step 4.2
  // RCBA + Offset 50h[23:16] = 2Ah, and also ensure that D29/D26:F0:88h [2] = 0b (Done at PchMiscInit() on PchInitPeim.c)
  //
  MmioAndThenOr32 (RootComplexBar + R_PCH_RCRB_CIR0, (UINT32) (~0x00F00FFF), (UINT32) (0x00200654));

  if (PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVcp].Enable == PCH_DEVICE_ENABLE) {
    MmioOr32 (RootComplexBar + R_PCH_RCRB_CIR0, BIT17 | BIT19);
  }
  //
  // Step 4.3, Step 4.4, Set RCBA + Offset 1114h[2][0] = 1b
  //
  MmioOr8 (RootComplexBar + 0x1114, (UINT8) (BIT2 | BIT0));

  //
  // Step 4.5, Step 4.6, Step 4.7, Step 4,8, Set the TC/VC mappings
  //
  for (Index = 0; Index < DmiTcTypeMax; Index++) {
    DEBUG ((DEBUG_INFO, "TC:%0x VC:%0x!\n",Index, PchDmiTcVcMapPpi->DmiTc[Index].Vc));
    VcMap[PchDmiTcVcMapPpi->DmiTc[Index].Vc] |= (BIT0 << Index);
  }

  for (Index = 0; Index < DmiVcTypeMax; Index++) {
    DEBUG ((DEBUG_INFO, "VC:%0x VCID:%0x Enable:%0x!\n",Index, PchDmiTcVcMapPpi->DmiVc[Index].VcId, PchDmiTcVcMapPpi->DmiVc[Index].Enable));
    if (PchDmiTcVcMapPpi->DmiVc[Index].Enable == PCH_DEVICE_ENABLE) {
      PchSetDmiTcVcMapping (
        RootComplexBar,
        Index,
        PchDmiTcVcMapPpi->DmiVc[Index].VcId,
        VcMap[Index]
        );
    }
  }
  
  //
  //4167453 SB traffic on VCp does not generate critical priority on ring
  //
  if (PchDmiTcVcMapPpi->WA_4167453) {
    //1. Disable VC1 in the PCH side. 
    Register = R_PCH_RCRB_V1CTL;
    Data32And = (UINT32) (~(B_PCH_RCRB_V1CTL_ID | V_PCH_RCRB_V1CTL_TVM_MASK | BIT31));
    Data32Or=0x0;
    MmioAndThenOr32 ((UINTN) (RootComplexBar + Register), Data32And, Data32Or);
    
    //2. Set VCp ID = 01 3. Set TC=2
    Register = R_PCH_RCRB_CIR31;
    Data32And = (UINT32) (~(B_PCH_RCRB_V1CTL_ID | V_PCH_RCRB_VXCTL_TVM_MASK));
    Data32Or = 0x01 << 24;  //VCID
    Data32Or |= 0x02 << 01;  //TC = 2
    MmioAndThenOr32 ((UINTN) (RootComplexBar + Register), Data32And, Data32Or);
  }
  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
PchDmiTcVcEnablePoll (
  IN  EFI_PEI_SERVICES            **PeiServices
  )
/*++

Routine Description:

  The function performing TC/VC mapping program, and poll all PCH Virtual Channel 
  until negotiation completion

Arguments:

  PeiServices             General purpose services available to every PEIM.

Returns:

  EFI_SUCCESS             The function completed successfully
  All other error conditions encountered result in an ASSERT.

--*/
{
  EFI_STATUS        Status;
  PCH_DMI_TC_VC_PPI *PchDmiTcVcMapPpi;
  UINT32            RootComplexBar;
  UINT32            PciD31F0RegBase;
  UINT8             Index;

  //
  // Locate PchDmiTcVcMap Ppi
  //
  Status = (*PeiServices)->LocatePpi (PeiServices, &gPchDmiTcVcMapPpiGuid, 0, NULL, &PchDmiTcVcMapPpi);
  ASSERT_EFI_ERROR (Status);

  PciD31F0RegBase         = MmPciAddress (
                               0,
                              DEFAULT_PCI_BUS_NUMBER_PCH,
                              PCI_DEVICE_NUMBER_PCH_LPC,
                              PCI_FUNCTION_NUMBER_PCH_LPC,
                              0
                              );
  
  RootComplexBar = (MmioRead32 (PciD31F0RegBase + R_PCH_LPC_RCBA) & B_PCH_LPC_RCBA_BAR);

  //
  // Step 4.9
  // Set RCBA + Offset 50h[31] = 1b
  // Lock down the TC mapping if no further changes are required to bits [30:16]
  //

  //
  // Step 4.9
  // Set RCBA + Offset 50h[31] = 1b
  // Lock down the TC mapping if no further changes are required to bits [30:16]
  //
  MmioOr32 (RootComplexBar + R_PCH_RCRB_CIR0, B_PCH_RCRB_CIR0_TCLOCKDN);
  ///
  /// Reads back for posted write to take effect
  ///
  MmioRead32 (RootComplexBar + R_PCH_RCRB_CIR0);
  //
  // Step 4.10
  // After both above and Sandy Bridge DMI TC/VC mapping are programmed, 
  // poll VC negotiation pending status until is zero:
  // 4.10.1 RCBA + Offset 201Ah[1]
  // 4.10.2 RCBA + Offset 2026h[1]
  // 4.10.3 RCBA + Offset 2036h[1]
  // 4.10.4 RCBA + Offset 2046h[1]
  //
  for (Index = 0; Index < DmiVcTypeMax; Index++) {
    if (PchDmiTcVcMapPpi->DmiVc[Index].Enable == PCH_DEVICE_ENABLE) {
      PchPollDmiVcStatus (RootComplexBar, Index);
    }
  }

  return EFI_SUCCESS;
}
//
// ESS: OverrideEnd
//

/**
  The function set the Target Link Speed in PCH to DMI GEN 2.

  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval None
**/
VOID
EFIAPI
PchDmiGen2Prog (
  IN  CONST EFI_PEI_SERVICES            **PeiServices
  )
{
#ifdef TRAD_FLAG
  UINT32  RootComplexBar;

  if (GetPchSeries() == PchH) {
    DEBUG ((DEBUG_INFO, "PchDmiGen2Prog() Start\n"));
    RootComplexBar = PCH_RCRB_BASE;

    ///
    /// PCH BIOS Spec Rev 0.5.0, Section 7.1.5
    /// Step 2
    /// Configure DMI Link Speed as early as possible
    /// Step 2.1
    /// Please refer to the System Agent BIOS Writer's Guide on Supported Link Speed
    /// field in Link Capabilities register in CPU complex. (Done in SA code)
    /// Step 2.2
    /// If the Supported Link Speed in CPU complex is 0010b (Done in SA code)
    /// and RCBA + Offset 21A4h[3:0] = 0010b
    ///
    if ((MmioRead32 (RootComplexBar + R_PCH_RCRB_LCAP) & B_PCH_RCRB_LCAP_MLS) == 0x02) {
      ///
      /// Step 2.2.1
      /// Set RCBA + Offset 21B0h[3:0] = 0010b
      ///
      MmioAndThenOr8 (RootComplexBar + 0x21B0, (UINT8)~(BIT3 | BIT2 | BIT1 | BIT0), (UINT8) BIT1);
      ///
      /// Step 2.2.2
      /// Please refer to the System Agent BIOS Writer's Guide to perform DMI Link Retrain after
      /// configures new DMI Link Speed. (Done in SA code)
      ///
    }
    DEBUG ((DEBUG_INFO, "PchDmiGen2Prog() End\n"));
  }
#endif // TRAD_FLAG
}

/**
  The function program DMI miscellaneous registers.

  @param[in] PchPlatformPolicyPpi The PCH Platform Policy PPI instance

  @retval EFI_SUCCESS             The DMI required settings programmed correctly
**/
EFI_STATUS
EFIAPI
PchDmiMiscProg (
  IN  CONST EFI_PEI_SERVICES             **PeiServices,
  IN  PCH_PLATFORM_POLICY_PPI      *PchPlatformPolicyPpi
  )
{
  UINT32                   RootComplexBar;
  EFI_STATUS               Status;
  UINT16                   LpcDeviceId;
  UINTN                    PciD31F0RegBase;
  UINTN                    PciD28F0RegBase;
  UINTN                    PciD20F0RegBase;
  UINT32                   Data32And;
  UINT32                   Data32Or;
  UINT16                   i;
  UINT16                   size;
  UINT8                    DeviceLaneOwner;
  UINT32                   StrpFuseCfg1;
  UINT8                    GbePort;
#ifdef ULT_FLAG
  UINTN                    RPBase;
  UINT8                    PortIndex;
#endif // ULT_FLAG
  PCH_SERIES               PchSeries;
  UINT8                    PchSteppingValue;
  UINT32                   Msg;
  UINT32                   Response;
  UINT32                   PchChipsetInitTableId;
  UINT32                   PchChipsetInitTableLength;
  UINT8                    *PchChipsetInitTable;
  CHIPSET_INIT_INFO_HOB    *ChipsetInitHob;
  EFI_BOOT_MODE            BootMode;
#ifdef TRAD_FLAG
  IOBP_MMIO_TABLE_STRUCT   *PchDmiHsio;
#ifndef PCH_SERVER_BIOS_FLAG
  IOBP_MMIO_ADDRESS        *PchUsb3HsioLptRstSeq;
#endif //PCH_SERVER_BIOS_FLAG
#endif // TRAD_FLAG
  IOBP_MMIO_TABLE_STRUCT   *PchUsb3Hsio;
  IOBP_MMIO_TABLE_STRUCT   *PchUsb3SharedHsio;
  IOBP_MMIO_TABLE_STRUCT   *PchGbeSharedHsio;  
  PchSeries               = GetPchSeries();
  Status                  = EFI_SUCCESS;
  RootComplexBar          = PchPlatformPolicyPpi->Rcba;
  PchChipsetInitTable   = NULL;
  PchChipsetInitTableLength     = 0;
  Msg                     = 0;
  PciD31F0RegBase         = MmPciAddress (
                               0,
                              DEFAULT_PCI_BUS_NUMBER_PCH,
                              PCI_DEVICE_NUMBER_PCH_LPC,
                              PCI_FUNCTION_NUMBER_PCH_LPC,
                              0
                              );
  PciD28F0RegBase         = MmPciAddress (
                              0,
                              DEFAULT_PCI_BUS_NUMBER_PCH,
                              PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS,
                              PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1,
                              0
                              );
  PciD20F0RegBase         = MmPciAddress (
                              0,
                              DEFAULT_PCI_BUS_NUMBER_PCH,
                              PCI_DEVICE_NUMBER_PCH_XHCI,
                              PCI_FUNCTION_NUMBER_PCH_XHCI,
                              0
                              );
  LpcDeviceId             = MmioRead16 (PciD31F0RegBase + R_PCH_LPC_DEVICE_ID);
  PchSteppingValue        = PchStepping();
#ifdef PCH_SERVER_BIOS_FLAG
  if(( PchSteppingValue >= LptHC0 ) && ( PchPlatformPolicyPpi->XTpmLen == PCH_DEVICE_ENABLE )){
    MmioOr32((RootComplexBar + 0x232C),(UINT32) BIT3);
  }
#endif //PCH_SERVER_BIOS_FLAG

  //
  // This code was used by LPT to pass surviability 
  // values to ME. Now it is also used by Wellsburg.
  //

  //
  // Get PchSeries and assign the appropriate ChipsetInit table
  //
  switch (PchSteppingValue) {
#ifdef ULT_FLAG
    case LptLpB0:
    case LptLpB1:
    case LptLpB2:
      PchChipsetInitTable = PchChipsetInitTableLptLp_Bx;
      PchChipsetInitTableLength   = sizeof(PchChipsetInitTableLptLp_Bx);
      break;
#endif // ULT_FLAG
#ifdef TRAD_FLAG
    case LptHB0:
#ifdef PCH_SERVER_BIOS_FLAG
      PchChipsetInitTable = PchChipsetInitTableWbg_A0;
      PchChipsetInitTableLength   = sizeof(PchChipsetInitTableWbg_A0);
#else      
      PchChipsetInitTable = PchChipsetInitTableLptH_B0;
      PchChipsetInitTableLength   = sizeof(PchChipsetInitTableLptH_B0);
#endif //PCH_SERVER_BIOS_FLAG
      break;
    case LptHC0:
    case LptHC1:
    case LptHC2:
#ifdef PCH_SERVER_BIOS_FLAG
      PchChipsetInitTable = PchChipsetInitTableWbg_B0;
      PchChipsetInitTableLength   = sizeof(PchChipsetInitTableWbg_B0);
#else
      PchChipsetInitTable = PchChipsetInitTableLptH_C0;
      PchChipsetInitTableLength   = sizeof(PchChipsetInitTableLptH_C0);
#endif //PCH_SERVER_BIOS_FLAG
      break;
#endif // TRAD_FLAG
    default:
      DEBUG ((EFI_D_ERROR, "Unsupported PCH Stepping %d for PchChipsetInitTable\n",PchSteppingValue));
  }

  //
  // Get the BootMode variable
  //
  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
  
  if (EFI_ERROR(Status)) {
    BootMode = 0;
  }

  //
  // Do not perform ChipsetInit check on S3 RESUME
  //
  if(BootMode != BOOT_ON_S3_RESUME) {
    //
    // Create Hob to send ChipsetInit table status to DXE phase.
    //
    DEBUG((EFI_D_INFO, "[MPHY] Creating HOB to adjust Hsio settings from DXE, if required.\n"));
    Status = (**PeiServices).CreateHob (
                                       PeiServices,
                                       EFI_HOB_TYPE_GUID_EXTENSION,
                                       sizeof (CHIPSET_INIT_INFO_HOB),
                                       &ChipsetInitHob
                                       );
    ASSERT_EFI_ERROR (Status);

    //
    // Initialize ChipsetInitHob
    //
    ChipsetInitHob->Header.Name       = gChipsetInitInfoHobGuid;
    ChipsetInitHob->ChipsetInitTableLen      = PchChipsetInitTableLength;
    ChipsetInitHob->ChipsetInitTableUpdReq   = 0;

      DEBUG((EFI_D_INFO, "[MPHY] SystemConfiguration.MeMphyDebugEnableSurvivabilityTable:%d\n", PchPlatformPolicyPpi->MeMphyDebugEnableSurvivabilityTable));
      DEBUG((EFI_D_INFO, "[MPHY] SystemConfiguration.MeMphyDebugCorruptEndpoints:%d\n", PchPlatformPolicyPpi->MeMphyDebugCorruptEndpoints));

    if(!PchPlatformPolicyPpi->MeMphyDebugEnableSurvivabilityTable){    
      //
      // Suppress passing the expected ChipsetInit table to the DXE code, and further on to ME
      //
      DEBUG((EFI_D_INFO, "[MPHY] Suppress passing the expected ChipsetInit table to the DXE code, and further on to ME\n"));
      Status = EFI_UNSUPPORTED;
  } else {
    //
    // Set the Host To ME flag requestint the Hsio ChipsetInit Table Version applied by ME FW
    //
    Status = PchPlatformPolicyPpi->MphyHeciTrigger_GETHsioVER(PchPlatformPolicyPpi, &Response);    
    }
    //
    // If successfully got the ACK from ME, then the Hsio Version info should be in the FWSTATUS register
    // Otherwise, just continue Hsio programming assuming the ChipsetInit settings programmed through other means.
    //
    if (Status == EFI_SUCCESS) {
      //
      // Receive the Hsio Version reported by ME FW.
      //
      Response = 0;
      PchPlatformPolicyPpi->MphyHeciReceive_GETHsioVER(PchPlatformPolicyPpi,&Msg);
      DEBUG((EFI_D_INFO, "[MPHY] ME Reported Hsio Version:%d CRC=0x%04X Response=%d us\n", (Msg>>16), (Msg&0xFFFF), Response));

      //
      // Send final message back to ME so that it can restore the FWSTS5 value (used for other messaging)
      //
      PchPlatformPolicyPpi->MphyHeciClose_GETHsioVER(PchPlatformPolicyPpi);

      //
      // Get ChipsetInit table indentifier from the one found in the code
      //
      if(PchChipsetInitTable != NULL) {
        PchChipsetInitTableId = *((UINT32*)PchChipsetInitTable);
        DEBUG((EFI_D_INFO, "[MPHY] BIOS Hsio Version:%d CRC=0x%04X Length=%d bytes.\n", (PchChipsetInitTableId>>16),(PchChipsetInitTableId&0xFFFF), PchChipsetInitTableLength));

        //
        // If expected table id is not found, then skip the rest of the Hsio programming until it can be updated from DXE
        //
        if (Msg != PchChipsetInitTableId) {
          //
          // Pass the expected ChipsetInit table to the DXE code that will apply the settings to ME and reset.
          //
          ChipsetInitHob->ChipsetInitTableUpdReq = 1;
          //
          // Copy the ChipsetInit settings from local table into the HOB
          //
          if (sizeof(ChipsetInitHob->ChipsetInitTable) >= PchChipsetInitTableLength) {
            CopyMem (ChipsetInitHob->ChipsetInitTable, PchChipsetInitTable, PchChipsetInitTableLength);
            //
            // Mphy corruption debug option
            //
            {           
              PTR_ICC_MPHY_BIOS_TABLE MphySurvivabilityHobPtr;
              UINT32 numEntries;
              UINT32 i;
              UINT8 oryginalEndpoint;
              
              if(PchPlatformPolicyPpi->MeMphyDebugCorruptEndpoints){ 
                DEBUG((EFI_D_INFO, "[MPHY] MeMphyDebugCorruptEndpoints\n"));

                //
                // Get pointer to survivability table and corrupt endpoints entries
                //
                MphySurvivabilityHobPtr = (PTR_ICC_MPHY_BIOS_TABLE) ChipsetInitHob->ChipsetInitTable;
                numEntries = MphySurvivabilityHobPtr->NumEntries;             

                DEBUG((EFI_D_INFO, "[MPHY] MphySurvivabilityHobPtr->NumEntries:%d\nValues:\n", numEntries));

                for (i = 0; i < numEntries; i++){
                  oryginalEndpoint = MphySurvivabilityHobPtr->Entries[i].Endpoint;
                  MphySurvivabilityHobPtr->Entries[i].Endpoint = INVALID_MPHY_ENDPOINT;
                  DEBUG((EFI_D_INFO, " Endpoint[%02d] oryginal:0x%x current:0x%x \n", i, oryginalEndpoint, MphySurvivabilityHobPtr->Entries[i].Endpoint));
                }
                DEBUG((EFI_D_INFO, "[MPHY] End of table.\n", numEntries));
              }
            }
          } else {
            ASSERT(FALSE); // Table should always fit into HOB structure.
          }

          //
          //  Skip the Hsio programming, DMI setting in ChipsetInit table should be good enough to get through DMI init.
          //
          return Status;
        }
      } else {
        ASSERT(FALSE);
      }
    }
  }
  ///
  /// PCH BIOS Spec Rev 0.5.0, Section 7.1.5
  /// Step 1.1
  /// RCBA + Offset 2088h = 00109000h
  ///
  MmioWrite32 (
    (RootComplexBar + R_PCH_RCRB_CIR1),
    0x00109000
    );
  ///
  /// Step 1.2
  /// RCBA + offset 20ACh[30] = 1b
  ///
  MmioOr32 (RootComplexBar + R_PCH_RCRB_REC, BIT30);
  if (PchSeries == PchH) {
    ///
    /// Step 1.3
    /// Set RCBA + Offset 2340h[7:0] = 1Bh
    ///
    MmioWrite8 (RootComplexBar + 0x2340, 0x1B);
    ///
    /// Step 1.4
    /// Set RCBA + Offset 2340h[23:16] = 3Ah
    ///
    Data32And = (UINT32) 0xFF00FFFF;
    Data32Or  = (UINT32) (0x3A << 16);

    MmioAndThenOr32 (
      RootComplexBar + 0x2340,
      Data32And,
      Data32Or
      );
    ///
    /// Step 1.5
    /// Program RCBA + Offset 2324[31:0] = 00854C74h
    ///
    MmioWrite32 (RootComplexBar + 0x2324, 0x00854C74);
  }

  ///
  /// Program Hsio Setting
  ///
  DeviceLaneOwner = MmioRead8 (PciD28F0RegBase + 0x410);
  StrpFuseCfg1    = MmioRead32 (PciD28F0RegBase + R_PCH_PCIE_STRPFUSECFG);
#ifdef TRAD_FLAG
  ///
  /// PCH BIOS Spec Rev 0.5.6, Section 7.1.5
  /// Step 6
  /// Bios is required to program IOBP setting according to the following table:
  /// Table 7-10 DMI Lane Setting
  ///
  if (PchSeries == PchH) {
    switch (PchSteppingValue) {
      case LptHB0:
#ifdef PCH_SERVER_BIOS_FLAG
      size = (sizeof (PchDmiHsioWbg_A0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
      PchDmiHsio = PchDmiHsioWbg_A0;
#else
        size = (sizeof (PchDmiHsioLptH_B0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
        PchDmiHsio = PchDmiHsioLptH_B0;
#endif //PCH_SERVER_BIOS_FLAG
        break;
      default:
        PchDmiHsio = NULL;
        size = 0;
        DEBUG ((DEBUG_ERROR, "Unsupported PCH Stepping for PchDmiHsio\n"));
    }
    for (i = 0; i < size; i++) {
      Status = ProgramIobp (
                RootComplexBar,
                PchDmiHsio[i].Address,
                PchDmiHsio[i].AndMask,
                PchDmiHsio[i].OrMask
                );
      ASSERT_EFI_ERROR (Status);
    }
  }
#endif // TRAD_FLAG

  ///
  /// PCH BIOS Spec Rev 0.5.6, Section 7.1.5
  /// Table 7-3 USB3 dedicated lane Setting
  ///
  switch (PchSteppingValue) {
#ifdef ULT_FLAG
    case LptLpB0:
    case LptLpB1:
    case LptLpB2:
     size = (sizeof (PchUsb3HsioLptLp_Bx) / sizeof (IOBP_MMIO_TABLE_STRUCT));
      PchUsb3Hsio = PchUsb3HsioLptLp_Bx;
      break;
#endif // ULT_FLAG
#ifdef TRAD_FLAG
    case LptHB0:
#ifdef PCH_SERVER_BIOS_FLAG
      size = (sizeof (PchUsb3HsioWbg_A0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
      PchUsb3Hsio = PchUsb3HsioWbg_A0;
#else
      size = (sizeof (PchUsb3HsioLptH_B0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
      PchUsb3Hsio = PchUsb3HsioLptH_B0;
#endif //PCH_SERVER_BIOS_FLAG
      break;
    case LptHC0:
    case LptHC1:
    case LptHC2:
#ifdef PCH_SERVER_BIOS_FLAG
      size = (sizeof (PchUsb3HsioWbg_B0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
      PchUsb3Hsio = PchUsb3HsioWbg_B0;
#else
      size = (sizeof (PchUsb3HsioLptH_C0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
      PchUsb3Hsio = PchUsb3HsioLptH_C0;
#endif //PCH_SERVER_BIOS_FLAG
      break;
#endif // TRAD_FLAG
    default:
      PchUsb3Hsio = NULL;
      size = 0;
      DEBUG ((EFI_D_ERROR, "Unsupported PCH Stepping for PchUsb3Hsio\n"));
  }
  for (i = 0; i < size; i++) {
    Status = ProgramIobp (
              RootComplexBar,
              PchUsb3Hsio[i].Address,
              PchUsb3Hsio[i].AndMask,
              PchUsb3Hsio[i].OrMask
              );
    ASSERT_EFI_ERROR (Status);
  }


  ///
  /// PCH BIOS Spec Rev 0.5.6, Section 7.1.5
  /// Table 7-5 USB3 Shared laneSetting
  ///
  switch (PchSteppingValue) {
#ifdef ULT_FLAG
    case LptLpB0:
    case LptLpB1:
    case LptLpB2:
      size = (sizeof (PchUsb3SharedHsioLptLp_Bx) / sizeof (IOBP_MMIO_TABLE_STRUCT));
      PchUsb3SharedHsio = PchUsb3SharedHsioLptLp_Bx;
      break;
#endif // ULT_FLAG
#ifdef TRAD_FLAG
    case LptHB0:
#ifdef PCH_SERVER_BIOS_FLAG
      size = (sizeof (PchUsb3SharedHsioWbg_A0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
      PchUsb3SharedHsio = PchUsb3SharedHsioWbg_A0;
#else
      size = (sizeof (PchUsb3SharedHsioLptH_B0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
      PchUsb3SharedHsio = PchUsb3SharedHsioLptH_B0;
#endif //PCH_SERVER_BIOS_FLAG
      break;
    case LptHC0:
    case LptHC1:
    case LptHC2:
#ifdef PCH_SERVER_BIOS_FLAG
      size = (sizeof (PchUsb3SharedHsioWbg_B0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
      PchUsb3SharedHsio = PchUsb3SharedHsioWbg_B0;
#else
      size = (sizeof (PchUsb3SharedHsioLptH_C0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
      PchUsb3SharedHsio = PchUsb3SharedHsioLptH_C0;
#endif //PCH_SERVER_BIOS_FLAG
      break;
#endif // TRAD_FLAG
    default:
      PchUsb3SharedHsio = NULL;
      size = 0;
      DEBUG ((DEBUG_ERROR, "Unsupported PCH Stepping for PchUsb3SharedHsio\n"));
  }
  for (i = 0; i < size; i++) {
    if (PchSeries == PchLp) {
      if ((((PchUsb3SharedHsio[i].Address & 0xFE00) == 0x2400) && ((DeviceLaneOwner & (BIT1 | BIT0)) != BIT1)) ||
          (((PchUsb3SharedHsio[i].Address & 0xFE00) == 0x2600) && ((DeviceLaneOwner & (BIT3 | BIT2)) != BIT3))) {
        continue;
      }
    } else if (PchSeries == PchH) {
      if ((((PchUsb3SharedHsio[i].Address & 0xFE00) == 0x2C00) && ((DeviceLaneOwner & (BIT3 | BIT2)) != BIT3)) ||
          (((PchUsb3SharedHsio[i].Address & 0xFE00) == 0x2E00) && ((DeviceLaneOwner & (BIT1 | BIT0)) != BIT1))) {
        continue;
      }
    }
    Status = ProgramIobp (
              RootComplexBar,
              PchUsb3SharedHsio[i].Address,
              PchUsb3SharedHsio[i].AndMask,
              PchUsb3SharedHsio[i].OrMask
              );
    ASSERT_EFI_ERROR (Status);
  }

#ifndef PCH_SERVER_BIOS_FLAG
#ifdef TRAD_FLAG
  switch (PchSteppingValue) {
    ///
    /// If LptHCx addresses 0xE9002E04 and 0xE9002C04    
    ///
    case LptHC0:
    case LptHC1:
    case LptHC2:

      // Settings moved to survivability options

      PchUsb3HsioLptRstSeq = NULL;
      size = 0;

      break;
    default:
      PchUsb3HsioLptRstSeq = NULL;
      size = 0;
      DEBUG ((DEBUG_ERROR, "Unsupported PCH Stepping for PchUsb3HsioLptRstSeq\n"));
  }
  for (i = 0; i < size; i++) {
    if (PchSeries == PchH) {
      if ((((PchUsb3HsioLptRstSeq[i].Address & 0xFE00) == 0x2C00) && ((DeviceLaneOwner & (BIT3 | BIT2)) != BIT3)) ||
              (((PchUsb3HsioLptRstSeq[i].Address & 0xFE00) == 0x2E00) && ((DeviceLaneOwner & (BIT1 | BIT0)) != BIT1))) {
            continue;
      }
    }

    ///
    /// Set reg_softreset_enable to '1'
    ///
    DEBUG((DEBUG_INFO, "ProgramIobp PchUsb3HsioShared  :%04X \n", PchUsb3HsioLptRstSeq[i].Address));
    Status = ProgramIobp (
              RootComplexBar,
              PchUsb3HsioLptRstSeq[i].Address,
              (UINT32)~(0x00800000),
              0x00800000
              );
    ASSERT_EFI_ERROR (Status);
    ///
    /// Set soft_reset_n to '0'
    ///
    Status = ProgramIobp (
              RootComplexBar,
              PchUsb3HsioLptRstSeq[i].Address,
              (UINT32)~(0x00000020),
              0x00000000
              );
    ASSERT_EFI_ERROR (Status);
    ///
    /// Set soft_reset_n to '1'
    ///
    Status = ProgramIobp (
              RootComplexBar,
              PchUsb3HsioLptRstSeq[i].Address,
              (UINT32)~(0x00000020),
              0x00000020
              );
    ASSERT_EFI_ERROR (Status);
    ///
    /// Wait 500us for calibration to complete
    ///
    PchPmTimerStall(500);
    ///
    /// Set reg_softreset_enable to '0'
    ///
    Status = ProgramIobp (
              RootComplexBar,
              PchUsb3HsioLptRstSeq[i].Address,
              (UINT32)~(0x00800000),
              0x00000000
              );
    ASSERT_EFI_ERROR (Status);
  }
#endif //TRAD_FLAG
#endif //PCH_SERVER_BIOS_FLAG

  ///
  /// Table 7-9 Gbe Lane Setting
  /// Bios should check the PCIE port that is assigned to Gbe and program the following address accordingly
  ///
  switch (PchSteppingValue) {
#ifdef ULT_FLAG
    case LptLpB0:
    case LptLpB1:
    case LptLpB2:
      PchGbeSharedHsio = PchGbeSharedHsioLptLp_Bx;
      size = (sizeof (PchGbeSharedHsioLptLp_Bx) / sizeof (IOBP_MMIO_TABLE_STRUCT));
      break;
#endif // ULT_FLAG
#ifdef TRAD_FLAG
    case LptHB0:
#ifdef PCH_SERVER_BIOS_FLAG
      PchGbeSharedHsio = PchGbeSharedHsioWbg_A0;
      size = (sizeof (PchGbeSharedHsioWbg_A0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
#else
      PchGbeSharedHsio = PchGbeSharedHsioLptH_B0;
      size = (sizeof (PchGbeSharedHsioLptH_B0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
#endif //PCH_SERVER_BIOS_FLAG
      break;
    case LptHC0:
    case LptHC1:
    case LptHC2:
#ifdef PCH_SERVER_BIOS_FLAG
      PchGbeSharedHsio = PchGbeSharedHsioWbg_B0;
      size = (sizeof (PchGbeSharedHsioWbg_B0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
#else
      PchGbeSharedHsio = PchGbeSharedHsioLptH_C0;
      size = (sizeof (PchGbeSharedHsioLptH_C0) / sizeof (IOBP_MMIO_TABLE_STRUCT));
#endif //PCH_SERVER_BIOS_FLAG
      break;
#endif // TRAD_FLAG
    default:
      PchGbeSharedHsio = NULL;
      size = 0;
      DEBUG ((EFI_D_ERROR, "Unsupported PCH Stepping for PchGbeSharedHsio\n"));
  }


  if (PchGbeSharedHsio != NULL) {
    if ((StrpFuseCfg1 & B_PCH_PCIE_STRPFUSECFG_GBE_PCIE_PEN) != 0) {
      GbePort = (UINT8) ((StrpFuseCfg1 & B_PCH_PCIE_STRPFUSECFG_GBE_PCIEPORTSEL) >> N_PCH_PCIE_STRPFUSECFG_GBE_PCIEPORTSEL);
    } else {
      GbePort = 0xFF;
    }

  if ((GbePort != 0xFF)  && (PchGbeSharedHsio != NULL)) {
#ifdef ULT_FLAG
      if (PchSeries == PchLp) {
        if (GbePort <= 0x5) {
          Status = ProgramIobp (
                    RootComplexBar,
                    PchGbeSharedHsio[GbePort].Address,
                    PchGbeSharedHsio[GbePort].AndMask,
                    PchGbeSharedHsio[GbePort].OrMask
                    );
          ASSERT_EFI_ERROR (Status);
        }
      }
#endif // ULT_FLAG
#ifdef TRAD_FLAG
      if (PchSeries == PchH) {
        if (GbePort == 0x0) {
          if ((DeviceLaneOwner & (BIT1 | BIT0)) == BIT0) {
            Status = ProgramIobp (
                      RootComplexBar,
                      PchGbeSharedHsio[GbePort].Address,
                      PchGbeSharedHsio[GbePort].AndMask,
                      PchGbeSharedHsio[GbePort].OrMask
                      );
            ASSERT_EFI_ERROR (Status);
          }
        } else if (GbePort == 0x1) {
          if ((DeviceLaneOwner & (BIT3 | BIT2)) == BIT2) {
            Status = ProgramIobp (
                      RootComplexBar,
                      PchGbeSharedHsio[GbePort].Address,
                      PchGbeSharedHsio[GbePort].AndMask,
                      PchGbeSharedHsio[GbePort].OrMask
                      );
            ASSERT_EFI_ERROR (Status);
          }
        } else {
          Status = ProgramIobp (
                    RootComplexBar,
                    PchGbeSharedHsio[GbePort].Address,
                    PchGbeSharedHsio[GbePort].AndMask,
                    PchGbeSharedHsio[GbePort].OrMask
                    );
          ASSERT_EFI_ERROR (Status);
        }
      }
#endif // TRAD_FLAG
    }
  }
  ///
  /// Step 7
  /// For LP, clear B0:D28:F0~F7:110h[13, 12, 8:6, 0] = 1b, 1b, 111b, 1b
  /// For LP, clear B0:D28:F0~F7:104h[20, 18:14, 12, 4] = 1b, 11111b, 1b, 1b
  ///
#ifdef ULT_FLAG
  if (GetPchSeries() == PchLp) {
    for (PortIndex = 0; PortIndex < GetPchMaxPciePortNum (); PortIndex++) {
      RPBase  = MmPciAddress (
                  0,
                  DEFAULT_PCI_BUS_NUMBER_PCH,
                  PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS,
                  PortIndex,
                  0
                  );
      MmioOr32 (RPBase + R_PCH_PCIE_CES, (UINT32)(B_PCH_PCIE_CES_ANFES | B_PCH_PCIE_CES_RTT |
                                                   B_PCH_PCIE_CES_RNR | B_PCH_PCIE_CES_BD   |
                                                   B_PCH_PCIE_CES_BT | B_PCH_PCIE_CES_RE));
      MmioOr32 (RPBase + R_PCH_PCIE_UES, (UINT32)(B_PCH_PCIE_UES_URE | B_PCH_PCIE_UES_MT |
                                                   B_PCH_PCIE_UES_RO | B_PCH_PCIE_UES_UC |
                                                   B_PCH_PCIE_UES_CA | B_PCH_PCIE_UES_CT |
                                                   B_PCH_PCIE_UES_PT | B_PCH_PCIE_UES_DLPE));
    }
  }
#endif //ULT_FLAG

  ///
  /// Step 8
  /// Bios is required to program IOBP setting according to the table 7-7 to 7-8
  /// using 7.1.4 IOSF SBI with OPCODE "PHY Configuration Register".
  /// Done in PchSataInit().
  ///
  /// PCH BIOS Spec Rev 0.5.1, Section 7.1.5
  /// Step 9
  /// IOBP Programming:
  /// For Mobile:
  /// BIOS is required to program IOBP setting according to Table 7-11 and
  /// Table 7-12 using settings in Section 7.1.4 with OPCODE "PHY Configuration Register".
  /// For Desktop:
  /// BIOS is required to program IOBP setting according to Table 7-13 and
  /// Table 7-14 using settings in Section 7.1.4 with OPCODE "PHY Configuration Register".
  /// Done in PchSataInit().
  ///
  /// Step 10, 11
  /// Set D20:F0:B0h[7] to 0b
  /// Set D20:F0:B0h[16] to 1b
  ///
  Data32And = (UINT32) ~(BIT7);
  Data32Or  = (UINT32) (BIT16);

  MmioAndThenOr32 (
    PciD20F0RegBase + 0xB0,
    Data32And,
    Data32Or
    );
  if (GetPchSeries() == PchLp) {
    ///
    /// Step 12
    /// Sideband Minimum Duration. T_SB_MIN = 16ns
    /// RCBA + Offset 260Ch[15:0]=0010h
    ///
    MmioWrite16 (
      (RootComplexBar + 0x260C),
      0x0010
      );
    ///
    /// Step x
    /// Program Iobp 0xEC000106 to 3100h
    ///
    Status = ProgramIobp (
              RootComplexBar,
              0xEC000106,
              (UINT32)~(0x00003100),
              0x00003100
              );
    ASSERT_EFI_ERROR (Status);
  }
  return Status;
}
