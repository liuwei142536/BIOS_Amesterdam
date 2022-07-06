/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c) 2009 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  IioDmiPeim.c

Abstract:

  Dmi PEI Initialization 

--*/

#include "Token.h" // Aptio V server override
#include "IioDmiPeim.h"

static EFI_PEI_NOTIFY_DESCRIPTOR mNotifyList[] = {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiMemoryDiscoveredPpiGuid,
    DmiTcVcSetup
};

#ifdef DE_SKU
static EFI_PEI_NOTIFY_DESCRIPTOR mNotifyList1[] = {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPlatformInfoGuid,
    IioSouthComplexPeiInit
};
#endif // DE_SKU

//
// 4986112:ESCALATE from hexaii_hsx_mock:C0 BIOS issues for isoc/ME
//
static ISOC_VC_TABLE_STRUCT IsocVc_Table_HsxC0[] = {
// Isoc  MeSegEn DmiVc1 DmiVcp DMiVcm Vc1_pri_en Vcp_pri_en Isoc_enabled
  { 0,    0,       0,      0,     0,    0,           0 ,      0}, // 0
  { 0,    0,       0,      0,     1,    0,           0 ,      0}, // 1 
  { 0,    0,       0,      1,     0,    0,           0 ,      0}, // 2 
  { 0,    0,       0,      1,     1,    0,           0 ,      0}, // 3 
  { 0,    0,       1,      0,     0,    0,           0 ,      0}, // 4 
  { 0,    0,       1,      0,     1,    0,           0 ,      0}, // 5 
  { 0,    0,       1,      1,     0,    0,           0 ,      0}, // 6 
  { 0,    0,       1,      1,     1,    0,           0 ,      0}, // 7 
  { 0,    1,       0,      0,     0,    0,           0 ,      0}, // 8 
  { 0,    1,       0,      0,     1,    1,           0 ,      0}, // 9 
  { 0,    1,       0,      1,     0,    0,           0 ,      0}, // 10
  { 0,    1,       0,      1,     1,    1,           0 ,      0}, // 11
  { 0,    1,       1,      0,     0,    0,           0 ,      0}, // 12
  { 0,    1,       1,      0,     1,    1,           0 ,      0}, // 13
  { 0,    1,       1,      1,     0,    0,           0 ,      0}, // 14
  { 0,    1,       1,      1,     1,    1,           0 ,      0}, // 15
  { 1,    0,       0,      0,     0,    0,           0 ,      1}, // 16
  { 1,    0,       0,      0,     1,    1,           0 ,      1}, // 17
  { 1,    0,       0,      1,     0,    0,           1 ,      1}, // 18
  { 1,    0,       0,      1,     1,    1,           1 ,      1}, // 19
  { 1,    0,       1,      0,     0,    1,           0 ,      1}, // 20
  { 1,    0,       1,      0,     1,    1,           0 ,      1}, // 21
  { 1,    0,       1,      1,     0,    1,           1 ,      1}, // 22
  { 1,    0,       1,      1,     1,    1,           1 ,      1}, // 23
  { 1,    1,       0,      0,     0,    0,           0 ,      1}, // 24
  { 1,    1,       0,      0,     1,    1,           0 ,      1}, // 25
  { 1,    1,       0,      1,     0,    0,           1 ,      1}, // 26
  { 1,    1,       0,      1,     1,    1,           1 ,      1}, // 27
  { 1,    1,       1,      0,     0,    1,           0 ,      1}, // 28
  { 1,    1,       1,      0,     1,    1,           0 ,      1}, // 29
  { 1,    1,       1,      1,     0,    1,           1 ,      1}, // 30
  { 1,    1,       1,      1,     1,    1,           1 ,      1}  // 31
}; 


VOID
GetIioUdsHob (
    IN CONST EFI_PEI_SERVICES         **PeiServices,
    IN IIO_UDS                  **UdsHobPtr

  )
{
  EFI_GUID                      UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
  EFI_HOB_GUID_TYPE            *GuidHob;

  //
  // Get BistHob
  //
  ASSERT(UdsHobPtr);

  *UdsHobPtr = NULL;
  
  GuidHob       = GetFirstGuidHob (&UniversalDataGuid);
  if (GuidHob){
    *UdsHobPtr  = GET_GUID_HOB_DATA (GuidHob);
    ASSERT (*UdsHobPtr);
  } 
}

EFI_STATUS
GetPlatformDmiPolicy(
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN OUT   VC_SETUP_INPUT       *VcInput,
  IN IIO_UDS                     *IioUds
  )
{
  EFI_PLATFORM_INFO                     *PlatformInfo;
  EFI_HOB_GUID_TYPE                     *GuidHob;

  //
  // Update the PCIE base and 32/64bit PCI resource support
  //
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);  

  if (PlatformInfo == NULL) {
    DEBUG((EFI_D_ERROR, "First Boot after flash BIOS!\n"));
    DEBUG((EFI_D_ERROR, "Use the default DMI parameters..."));
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
    DEBUG((EFI_D_ERROR, "enable all DMI VCx\n")); // to enable UMA on first boot
    /// 4986450 ESCALATE from hexaii_hsx_mock:C0:  Virtual channels not disabled when Isoc disabled via secondary means
    /// QPI could have changed IsocEn and MeSegEn bits when discovering 
    /// QPI Topology. Use the correct QPI value. 
    VcInput->IsocEn = IioUds->SystemStatus.IsocEnable;
    VcInput->MeSegEn = IioUds->SystemStatus.MeSegEn; 
    if (VcInput->IsocEn == 0) {
      VcInput->DmiVc1 = 0;
      VcInput->DmiVcp = 0;
      if (VcInput->MeSegEn == 0)
        VcInput->DmiVcm = 0;
      else 
        VcInput->DmiVcm = 1;
    } else {
      VcInput->DmiVc1 = 1;
      VcInput->DmiVcp = 1;
      VcInput->DmiVcm = 1;
    }
#else
    DEBUG((EFI_D_ERROR, "disable all DMI VCx\n"));
    VcInput->IsocEn = 0;
    VcInput->MeSegEn = 0;
    VcInput->DmiVc1 = 0;
    VcInput->DmiVcp = 0;
    VcInput->DmiVcm = 0;
#endif // ME_SUPPORT_FLAG
  } else {
    /// 4986450 ESCALATE from hexaii_hsx_mock:C0:  Virtual channels not disabled when Isoc disabled via secondary means
    /// QPI could have changed IsocEn and MeSegEn bits when discovering 
    /// QPI Topology. Use the correct QPI value. 
    VcInput->IsocEn    = IioUds->SystemStatus.IsocEnable; 
    VcInput->MeSegEn   = IioUds->SystemStatus.MeSegEn; 
    if (VcInput->IsocEn == 0) {
      VcInput->DmiVc1 = 0;
      VcInput->DmiVcp = 0;
      if (VcInput->MeSegEn == 0)
        VcInput->DmiVcm = 0;
      else /// Use configured value. 
        VcInput->DmiVcm = PlatformInfo->DmiVcm;
    } else {
      /// Use what is configured.
      VcInput->DmiVc1 = PlatformInfo->DmiVc1;
      VcInput->DmiVcp = PlatformInfo->DmiVcp;
      VcInput->DmiVcm = PlatformInfo->DmiVcm;
    }
  }
  return EFI_SUCCESS;
}

EFI_STATUS
IioSetDmiTcVcMapping (
  IN    PCH_DMI_TC_VC_PPI   *PchDmiTcVcMapPpi,
  IN    UINT64              DmiBar
  )
/*++

Routine Description:

  Map SA DMI TCs to VC
  
Arguments:

  PchDmiTcVcMapPpi     - Instance of PCH_DMI_TC_VC_PPI
  DmiBar               - DMIBAR address

Returns:

  EFI_SUCCESS

--*/
{
  UINT32  Data32And;
  UINT32  Data32Or;
  UINT8   Data8And;
  UINT8   Data8Or;
  UINT8   Index;
  UINT16  Register;
  UINT8   VcId;
  UINT8   VcMap[DmiVcTypeMax] = {0};

  //
  // Set the TC/VC mappings
  //
  for (Index = 0; Index < DmiTcTypeMax; Index++) {
    VcMap[PchDmiTcVcMapPpi->DmiTc[Index].Vc] |= (BIT0 << Index);
  }
  //
  // 6.2.3.1  Virtual Channel 0 Configuration
  // a. System BIOS must ensure that only TC0 is mapped to VC0:
  //
  // Set DMIBAR Offset 014h [7:0] = `00010001b'
  // Set DMIBAR Offest 20h  [7:0] = `00100010b'
  // Set DMIBAR Offeset 2Ch [7:0] = `01000100b'
  //
  // 6.2.3.2 Virtual Channel 1 Configuration
  // Step1. Assign Virtual Channel ID 1 to VC1:
  //    Set the DMIVC1RCTL DMI Port Register DMIBAR Offset 020h[26:24]=001b
  //
  // Step2. Set TC1 and TC5 (Traffic Class) to VC1:
  //    Program the DMIVC1RCTL DMI Port Register DMIBAR Offset 020h[7:1] = '0010001b' .
  //
  // Step3.	Enable VC1 by programming the DMIVC1RCTL DMI Port Register DMIBAR Offset 020h[31] = '1b'.
  //
  //
  // 6.2.3.3 Private Virtual Channel Configuration
  // Step1. Assign Virtual Channel ID 2 to VCp:
  //    Programming the DMIVCPRCTL DMI Port Register DMIBAR Offset 02Ch[26:24] = '010b'.
  //
  // Step2. Set TC2 and TC6 (Traffic Class) to VC2:
  //    Program the DMIVCPRCTL DMI Port Register DMIBAR Offset 02Ch[7:1] = '0100010b' .
  //
  // Step3.	Enable VCp by programming the DMIVCPRCTL DMI Port Register DMIBAR Offset 02Ch[31] = '1b'.
  //
  // 6.2.3.4 Virtual Channel for ME (VCm) Configuration
  // This is configured by ConfigMemMe
  //
  // Step1. Assign Virtual Channel ID 7 to VCm:
  //    Programming the DMIVCMRCTL DMI Port Register DMIBAR Offset 038h[26:24] = '111b'.
  //
  // Step2. Enable VCm:
  //    Programming the DMIVMPRCTL DMI Port Register DMIBAR Offset 038h[31] = '1b'.
  //
  // Step3.	Enable VCm by programming the DMIVCMRCTL DMI Port Register DMIBAR Offset 038h[31] = '1b'.
  //
  for (Index = 0; Index < DmiVcTypeMax; Index++) {
    if (PchDmiTcVcMapPpi->DmiVc[Index].Enable == PCH_DEVICE_ENABLE) {
      //
      // Map TCs to VC, Set the VC ID, Enable VC
      //
      VcId = PchDmiTcVcMapPpi->DmiVc[Index].VcId,

      Data32And = (UINT32) (~(B_IIO_RCRB_DMIVCxRCTL_VCx_ID_MASK | B_IIO_RCRB_DMIVCxRCTL_VCx_TCVCxM_MASK));
      Data32Or = VcId << 24;
      Data32Or |= VcMap[Index];
      Data32Or |= B_IIO_RCRB_DMIVCxRCTL_VCx_EN;

      switch (Index) {
      case DmiVcTypeVc0:
        Register = R_IIO_RCRB_DMIVC0RCTL;
        break;

      case DmiVcTypeVc1:
        Register = R_IIO_RCRB_DMIVC1RCTL;
        break;

      case DmiVcTypeVcp:
        Register = R_IIO_RCRB_DMIVCPRCTL;
        break;

      case DmiVcTypeVcm:
        Register = R_IIO_RCRB_DMIVCMRCTL;
        break;

      default:
        return EFI_INVALID_PARAMETER;
      }

      MmioAndThenOr32 ((UINTN) (DmiBar + Register), Data32And, Data32Or);
    }
  }

  //
  // 6.2.3.1 b. System BIOS must program the extended VC Count:
  //    Set the DMI Port Register DMIBAR Offset 004h[2:0]=001b
  //
  Data8And = (UINT8) (~0x07);
  if (PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVc1].Enable == TRUE) {
    Data8Or = 1;
  } else {
    Data8Or = 0;
  }

  MmioAndThenOr8 ((UINTN) (DmiBar + R_IIO_RCRB_DMIPVCCAP1), Data8And, Data8Or);

  //
  //4167453 SB traffic on VCp does not generate critical priority on ring
  //
  if(PchDmiTcVcMapPpi->WA_4167453 ) {
    DEBUG((EFI_D_ERROR, "WA_4167453 IIO side!\n"));
    //1. Disable VCP in the IIO side 2. 
    Register = R_IIO_RCRB_DMIVCPRCTL;
    Data32And = (UINT32) (~(B_IIO_RCRB_DMIVCxRCTL_VCx_ID_MASK | B_IIO_RCRB_DMIVCxRCTL_VCx_TCVCxM_MASK | B_IIO_RCRB_DMIVCxRCTL_VCx_EN));
    Data32Or=0x0;
    MmioAndThenOr32 ((UINTN) (DmiBar + Register), Data32And, Data32Or);
    
    //2. Set VC1 ID = 01 3. Set TC=2
    Register = R_IIO_RCRB_DMIVC1RCTL;
    Data32And = (UINT32) (~(B_IIO_RCRB_DMIVCxRCTL_VCx_ID_MASK | B_IIO_RCRB_DMIVCxRCTL_VCx_TCVCxM_MASK ));
    Data32Or = (0x01 << 24);  //VCID = 01
    Data32Or |= (0x02 << 01);  //TC = 2
    MmioAndThenOr32 ((UINTN) (DmiBar + Register), Data32And, Data32Or);
  }

  return EFI_SUCCESS;
}


EFI_STATUS
IioPollDmiVcStatus (
  IN    PCH_DMI_TC_VC_PPI   *PchDmiTcVcMapPpi,
  IN    UINT64              DmiBar
  )
/*++

Routine Description:

  Poll SA DMI negotiation completion
  
Arguments:

  PchDmiTcVcMapPpi     - Instance of PCH_DMI_TC_VC_PPI
  DmiBar               - DMIBAR address

Returns:

  EFI_SUCCESS

--*/
{
  UINT8   Index;
  UINT16  Register;

  //
  // 6.2.3.2 - Step 4, Poll until VC1 has been negotiated
  //    Read the DMIVC1RSTS DMI Port Register Offset 026h until [1]==0
  //
  // 6.2.3.3 - Step4. Poll the VCp Negotiation Pending bit until it reads 0:
  //    Read the DMIVCPRSTS DMI Port Register Offset 032h until [1]==0
  //
  // 6.2.3.4 - Step4. Poll the VCm Negotiation Pending bit until it reads 0:
  //    Read the DMIVCMRSTS DMI Port Register Offset 03Eh until [1]==0
  //
  for (Index = 0; Index < DmiVcTypeMax; Index ++) {
    if (PchDmiTcVcMapPpi->DmiVc[Index].Enable == PCH_DEVICE_ENABLE) {
      switch (Index) {
      case DmiVcTypeVc0:
        Register = R_IIO_RCRB_DMIVC0RSTS;
        break;

      case DmiVcTypeVc1:
        Register = R_IIO_RCRB_DMIVC1RSTS;
        break;

      case DmiVcTypeVcp:
        Register = R_IIO_RCRB_DMIVCPRSTS;
        break;

      case DmiVcTypeVcm:
        Register = R_IIO_RCRB_DMIVCMRSTS;
        break;

      default:
        return EFI_INVALID_PARAMETER;
      }
      if (IsSoftSdvPlatform() == FALSE) {
        //
        // Wait for negotiation to complete
        //
        while ((MmioRead16 ((UINTN)(DmiBar + Register)) & B_IIO_RCRB_DMIVCxRSTS_VCx_NP) != 0);
      }
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
DmiTcVcSetup (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
{
  EFI_STATUS        Status;
  UINT64            DmiBar;
  PCH_INIT_PPI      *PchInitPpi;
  PCH_DMI_TC_VC_PPI *PchDmiTcVcMapPpi;
  UINT8             i;
  VC_SETUP_INPUT    VcInput;

  IIO_UDS           *IioUds = NULL;

  //
  // Locate PchInitPpi and PchDmiTcVcMapPpi
  //
  Status = (*PeiServices)->LocatePpi (PeiServices, &gPchInitPpiGuid, 0, NULL, &PchInitPpi);
  ASSERT_EFI_ERROR (Status);

  Status = (*PeiServices)->LocatePpi (PeiServices, &gPchDmiTcVcMapPpiGuid, 0, NULL, &PchDmiTcVcMapPpi);
  ASSERT_EFI_ERROR (Status);

  GetIioUdsHob(PeiServices, &IioUds);
  if( !IioUds ){    //fix for s4031021
    DEBUG((EFI_D_INFO, "ERROR!!! IIO UDS Hob not found....unable to configure DMI!\n"));
    return EFI_NOT_FOUND;
  }

  //
  //  Apply any DMI specific Si workarounds first
  //
  InitDmiSpecificSiWorkaorunds(PeiServices, IioUds);
  //
  // For DMI 2.0
  // First Program PCH DMI to gen2
  //
  PchInitPpi->DmiGen2Prog (PeiServices);
  //
  // Then program SA DMI to gen2, retrain will be in the DXE driver
  //
  //  PciWrite16 ( PCI_LIB_ADDRESS (DMI_BUS_NUM, DMI_DEV_NUM, DMI_FUNC_NUM, R_DMI_PORT_PXPLNKCTRL2), V_DMI_PORT_PXPLNKCTRL2_LS_GEN2);

  //For 4167036, the following line would cause PchInitPpi->DmiTcVcProg not called, which setup VCM registers in WBG
  //if (!(IioUds->SystemStatus.IsocEnable || IioUds->SystemStatus.MeSegEn))  return EFI_SUCCESS;
  // 4986115: ESCALATE from hexaii_hsx_mock:17 D 51 not programming Vcm Channel in MESEG mode.
  // Program DmiVcx if either Isoc or Meseg are enable
  /*if ((IioUds->SystemStatus.IsocEnable == 0) && (IioUds->SystemStatus.MeSegEn == 0)){       
      return EFI_SUCCESS;
  } */ 

#ifdef DE_SKU
  //
  // s4988153 : CLONE from broadwell_server: Clone from BDX Sighting:U0 PO: PFE: BIOS enabling GBE physical 
  // functions making system hangs - access to pmc_cfg_cr_svid
  //
  ProcessSC_Gbe_PF(PeiServices, IioUds);
#endif

  if (IioUds->PlatformData.IIO_resource[0].PciResourceMem32Limit <= IioUds->PlatformData.IIO_resource[0].PciResourceMem32Base) {
    return EFI_OUT_OF_RESOURCES;
  }
  DmiBar = (UINT64)IioUds->PlatformData.IIO_resource[0].PciResourceMem32Limit & (~0xFFF) ; 
  
  //
  // Update DmiTcVcMapping based on Policy
  //
  if(EFI_ERROR(GetPlatformDmiPolicy(PeiServices, &VcInput, IioUds))){
    
    // HSD 4166750: HSX: ESCALATE from hexaii_hsx_mock:Isoc: VC1/VCp BIOS knobs are not working
    //
    VcInput.DmiVc1 = IioUds->SetupInfo.Config.DmiVc1;
    VcInput.DmiVcp = IioUds->SetupInfo.Config.DmiVcp;
    VcInput.DmiVcm = IioUds->SetupInfo.Config.DmiVcm;
  }

  PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVc1].Enable = VcInput.DmiVc1;
  PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVcp].Enable = VcInput.DmiVcp;
  PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVcm].Enable = VcInput.DmiVcm;
  PchDmiTcVcMapPpi->WA_4167453                 = IioUds->SetupInfo.Config.WA_4167453;		


  for (i = 0; i < DmiTcTypeMax; i++) {
    if (((PchDmiTcVcMapPpi->DmiTc[i].Vc == DmiVcTypeVc1) && (PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVc1].Enable == FALSE)) ||
        ((PchDmiTcVcMapPpi->DmiTc[i].Vc == DmiVcTypeVcp) && (PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVcp].Enable == FALSE)) ||
        ((PchDmiTcVcMapPpi->DmiTc[i].Vc == DmiVcTypeVcm) && (PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVcm].Enable == FALSE)))
    {
      PchDmiTcVcMapPpi->DmiTc[i].Vc = DmiVcTypeVc0;
    }
  }


  //
  // IIO Isoc Enabling 
  //
  IioIsocConfiguration(PeiServices, IioUds, PchDmiTcVcMapPpi);

  DEBUG((EFI_D_INFO, "DEBUG:::: DmiVc1 = %x ; DmiVcp = %x ; DmiVcm = %x\n", VcInput.DmiVc1, VcInput.DmiVcp, VcInput.DmiVcm));
  if (!(VcInput.DmiVc1 + VcInput.DmiVcp + VcInput.DmiVcm)) 
    return EFI_SUCCESS;

  //
  // Temporarily enable DmiRcBar at a valid address
  //
  PciExpressWrite32 ( PCI_LIB_ADDRESS (DMI_BUS_NUM, DMI_DEV_NUM, DMI_FUNC_NUM, R_PCIE_PORT_DMIRCBAR), (UINT32)DmiBar+ BIT0);
  //
  // Call PchDmiTcVcProg
  //
  DEBUG((EFI_D_INFO, "DEBUG::::  Calling DmiTcVcProg\n"));
  Status = PchInitPpi->DmiTcVcProg (PeiServices);
  ASSERT_EFI_ERROR (Status);
  
  //
  // Program NB TC/VC mapping
  //
  DEBUG((EFI_D_INFO, "DEBUG::::  SetDmiTcVcMapping\n"));
  IioSetDmiTcVcMapping (PchDmiTcVcMapPpi, DmiBar);
  //
  // Poll NB negotiation completion
  //
  IioPollDmiVcStatus (PchDmiTcVcMapPpi, DmiBar);

  //
  // Call PchDmiTcVcProgPoll
  //
  Status = PchInitPpi->DmiTcVcEnablePoll (PeiServices);
  ASSERT_EFI_ERROR (Status);


  //
  // Turn off the DmiRcBar so it can be appropriately allocated later in BIOS
  //
  PciExpressWrite32 ( PCI_LIB_ADDRESS (DMI_BUS_NUM, DMI_DEV_NUM, DMI_FUNC_NUM, R_PCIE_PORT_DMIRCBAR), 0);
  DEBUG((EFI_D_INFO, "DEBUG::::  Done\n"));


  return EFI_SUCCESS;

}

VOID
DmiLinkTrain (
  IN  UINT64 DmiBar
  )
/*++

Routine Description:

  DMI link training
  
Arguments:

  DmiBar - DMIBAR address

Returns:

  None

--*/
{
  //
  // Retrain link
  //
  MmioOr8 ((UINTN) (DmiBar + R_IIO_RCRB_DMILCTRL), BIT5);

  //
  // Wait for link training complete
  //
  while ((MmioRead16 ((UINTN) (DmiBar + R_IIO_RCRB_DMILSTS)) & BIT11) != 0);
}

/*++

Routine Description: Initialize IIO_GLOBALS values used in IioEarlyInitializeLib
                     based on sysHost 

Arguments: 
          PeiServices           Pointer to PEI services
          IioUds                Pointer to Iuds 
          PchDmiTcVcMapPpi      Pointer to DmiTcVcMapping configuration
 
Returns:
  None
--*/
VOID
IioIsocConfiguration(
    IN  CONST EFI_PEI_SERVICES    **PeiServices,
    IN  IIO_UDS                   *IioUds,
    IN  PCH_DMI_TC_VC_PPI         *PchDmiTcVcMapPpi
  )
{
  UINT8                                      Iio;
  UINT8                                      j;
  UINT8                                      BusNumber;
  IRPEGCREDITS_IIO_VTD_STRUCT                IrpEgCredits;
  VTDPRIVC0_IIO_DFX_VTD_STRUCT               VTdPrivC0;
  SWDBGCTL0_IIO_DFX_GLOBAL_HSX_BDX_STRUCT    SwDbgCtl0;
  CSIPISOCRES_IIO_DFX_GLOBAL_HSX_BDX_STRUCT  CsiPiSocres;
  CIPCTRL_IIO_VTD_HSX_BDX_STRUCT             Cipctrl;
  ISOC_VC_TABLE_STRUCT                       *IsocVc_Tbl;
  CSIPCTRL_0_IIO_DFX_GLOBAL_STRUCT           CsiPCtrl0;
  UINT32                                     IrpMiscDfx1;

  for (Iio = 0; Iio < MaxIIO; Iio++) {
     if (!(IioUds->SystemStatus.socketPresentBitMap & (1 << Iio)))
         continue;

      BusNumber = IioUds->PlatformData.IIO_resource[Iio].BusBase;
      //
      // The following is common for Processors
      // 3614823, 3876167: isoc_enabled bit in CSIPISOCRES needs to be set
      //                   when isoc is enabled
      //  [Non-Si, permanent programming for all steppings]
      //
      // 4167166: [BSX_B0_PO] CLONE from HSX: OVL Error: cache allocation exceeds max cache!
      //
      CsiPiSocres.Data = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(CSIPISOCRES_IIO_DFX_GLOBAL_REG)));  
      IrpEgCredits.Data = PciExpressRead32(PCI_PCIE_ADDR(BusNumber, CORE05_DEV_NUM, CORE05_FUNC_NUM, ONLY_REGISTER_OFFSET(IRPEGCREDITS_IIO_VTD_REG)));

      IrpMiscDfx1 = PciExpressRead32(PCI_PCIE_ADDR(BusNumber, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IRP_MISC_DFX1));

      if(IioUds->SystemStatus.cpuType == CPU_IVT){
          IrpMiscDfx1 &= ~(BIT25 | BIT24);   //pf_flush_timer = 0
      }
        
      if ((IioUds->SystemStatus.IsocEnable) || (IioUds->SystemStatus.MeSegEn)){
        //
        //4167774 : HSX Clone: workaround for all known isoc issues
        //4186011: PROMOTE from haswell_server: Isoc Flexcon Failure when IsocEn is Enable
        //
        if ((IioUds->SystemStatus.cpuType == CPU_IVT) || 
            ((IioUds->SystemStatus.cpuType == CPU_HSX) && (IioUds->SystemStatus.MinimumCpuStepping == B0_REV_HSX))){
          CsiPiSocres.Bits.isoc_enabled = 0x01;
          // 4168320 merge_vc0_cnt should be set when the following conditions are on: 
          if ((((PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVc1].Enable) || (PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVcp].Enable) || (PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVcm].Enable)) &&  
               (IioUds->SystemStatus.IsocEnable)) ||
               (PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVcm].Enable &&  IioUds->SystemStatus.MeSegEn ))
                 CsiPiSocres.Bits.merge_vc0_cnt = 0x01;
         }
        if ((IioUds->SystemStatus.cpuType == CPU_HSX) && (IioUds->SystemStatus.MinimumCpuStepping <= B0_REV_HSX)){
          IrpEgCredits.Bits.ad_shared_cdt_threshold = 0x1;
        }
      } else {
          if ((IioUds->SystemStatus.cpuType == CPU_HSX) && (IioUds->SystemStatus.MinimumCpuStepping <= B0_REV_HSX)){
            IrpEgCredits.Bits.ad_shared_cdt_threshold = 0xb;
          }
      }

      if (((IioUds->SystemStatus.cpuType == CPU_HSX) && (IioUds->SystemStatus.MinimumCpuStepping >= C0_REV_HSX)) ||
          (IioUds->SystemStatus.cpuType == CPU_BDX))  {
        if (Iio == 0) {
           //
           // 4983629 Set merge_vc0_cnt unconditionally for C0 stepping. 
           //     
           CsiPiSocres.Bits.merge_vc0_cnt = 0x01;
           //
           // 4986112: ESCALATE from hexaii_hsx_mock:C0 BIOS issues for isoc/ME
           //
           Cipctrl.Data = PciExpressRead32(PCI_PCIE_ADDR(BusNumber, CORE05_DEV_NUM, CORE05_FUNC_NUM, ONLY_REGISTER_OFFSET(CIPCTRL_IIO_VTD_REG)));

           IsocVc_Tbl = IsocVc_Table_HsxC0;
           for (j=0; j < (sizeof(IsocVc_Table_HsxC0)/sizeof(ISOC_VC_TABLE_STRUCT)); j++) {
             if(IioUds->SystemStatus.IsocEnable ==  IsocVc_Tbl[j].Isoc){
               if(IioUds->SystemStatus.MeSegEn == IsocVc_Tbl[j].Meseg){
                 if(PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVc1].Enable == IsocVc_Tbl[j].Vc1){
                   if(PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVcm].Enable == IsocVc_Tbl[j].Vcm){
                     if(PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVcp].Enable == IsocVc_Tbl[j].Vcp){ 
                       Cipctrl.Bits.vc1_pri_en = IsocVc_Tbl[j].Vc1_pri_en;
                       Cipctrl.Bits.vcp_pri_en = IsocVc_Tbl[j].Vcp_pri_en;
                       CsiPiSocres.Bits.isoc_enabled = IsocVc_Tbl[j].Isoc_Enable;                        
                       break;
                     }
                   } 
                 }
               }
             }
           }
           PciExpressWrite32(PCI_LIB_ADDRESS(BusNumber, CORE05_DEV_NUM, CORE05_FUNC_NUM, ONLY_REGISTER_OFFSET(CIPCTRL_IIO_VTD_REG)), Cipctrl.Data);
         } else {
           //
           // 4986476: ESCALATE from hexaii_hsx_mock:C0: BIOS needs to set isoc registers differently on sockets 1..N
           //
           CsiPiSocres.Bits.merge_vc0_cnt = 0x00;
           CsiPiSocres.Bits.isoc_enabled = 0;
         }
         
         //
         // 4986166: ESCALATE from hexaii_hsx_mock:Updated C0 register settings for isoc
         //
         CsiPCtrl0.Data = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(CSIPCTRL_0_IIO_DFX_GLOBAL_REG)));
         CsiPCtrl0.Bits.vcp_priority = 3; 
         PciExpressWrite32(PCI_LIB_ADDRESS(BusNumber, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(CSIPCTRL_0_IIO_DFX_GLOBAL_REG)),CsiPCtrl0.Data);
         //
         // 4986166: ESCALATE from hexaii_hsx_mock:Updated C0 register settings for isoc
         //
         IrpMiscDfx1 &= ~BIT22; //irp_misc_dfx1.use_isoc_ovf_q  = 0   
         //
         // 4986258: ESCALATE from hexaii_hsx_mock:C0 BIOS/Isoc issues with 18d15
         //
         IrpEgCredits.Bits.ad_shared_cdt_threshold = 0xb;
      }

      PciExpressWrite32 (PCI_LIB_ADDRESS(BusNumber, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(CSIPISOCRES_IIO_DFX_GLOBAL_REG)), CsiPiSocres.Data);
      PciExpressWrite32 (PCI_LIB_ADDRESS(BusNumber, CORE05_DEV_NUM, CORE05_FUNC_NUM, ONLY_REGISTER_OFFSET(IRPEGCREDITS_IIO_VTD_REG)), IrpEgCredits.Data);
      PciExpressWrite32(PCI_LIB_ADDRESS(BusNumber, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IRP_MISC_DFX1), IrpMiscDfx1); 

      //
      //4167774 : HSX Clone: workaround for all known isoc issues for HSX A0 and B0 only 
      //
      if ((IioUds->SystemStatus.cpuType == CPU_HSX)) { 
        if (IioUds->SystemStatus.MinimumCpuStepping < B0_REV_HSX) {
          VTdPrivC0.Data =  PciExpressRead32(PCI_PCIE_ADDR(BusNumber, IIO_DFX_VTD_DEV, IIO_DFX_VTD_FUNC, ONLY_REGISTER_OFFSET(VTDPRIVC0_IIO_DFX_VTD_REG))) ;
          if (IioUds->SetupInfo.Config.VTdSupport)
            VTdPrivC0.Bits.dvt = 0;              
          else
            VTdPrivC0.Bits.dvt = 0x01;
          PciExpressWrite32 (PCI_LIB_ADDRESS(BusNumber, IIO_DFX_VTD_DEV, IIO_DFX_VTD_FUNC, ONLY_REGISTER_OFFSET(VTDPRIVC0_IIO_DFX_VTD_REG)), VTdPrivC0.Data);
          
          SwDbgCtl0.Data =  PciExpressRead32(PCI_PCIE_ADDR(BusNumber, 6, 7, ONLY_REGISTER_OFFSET(SWDBGCTL0_IIO_DFX_GLOBAL_REG))) ;
          if ( (PchDmiTcVcMapPpi->WA_4167453 || PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVc1].Enable ) && ( IioUds->SetupInfo.Config.VTdSupport) )
            SwDbgCtl0.Bits.enable_irp_set = 0;
          else 
            SwDbgCtl0.Bits.enable_irp_set = 1;
          PciExpressWrite32 (PCI_LIB_ADDRESS(BusNumber, 6, 7, ONLY_REGISTER_OFFSET(SWDBGCTL0_IIO_DFX_GLOBAL_REG)), SwDbgCtl0.Data); 
        }
      }
  }
}
EFI_STATUS
EFIAPI
IioDmiInitPeiEntryPoint(
  IN EFI_FFS_FILE_HEADER       *FfsHeader,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
/*++

Routine Description:

  Initialize DMI Tc/Vc mapping through SA-PCH.
  
Arguments:

  PeiServices          - General purpose services available to every PEIM.

Returns:

  EFI_SUCCESS

--*/
{

    EFI_STATUS                  Status = EFI_SUCCESS;

    DEBUG((EFI_D_INFO, "DEBUG::::  IioDmiInitPeiEntryPoint()\n"));
    //
    // register callback on "gEfiPeiMemoryDiscoveredPpiGuid" PPI to setup DMI
    //
    Status = (**PeiServices).NotifyPpi (PeiServices, &mNotifyList[0]);
    ASSERT_EFI_ERROR (Status);

    //
    // Setup very early IIO initialization if any
    //
    IioMiscPeiInit(PeiServices);

    return Status;

}

VOID
InitDmiSpecificSiWorkaorunds  (
    IN  CONST EFI_PEI_SERVICES    **PeiServices,
    IN  IIO_UDS                   *IioUds
)
{
}

VOID
IioMiscPeiInit  (
	IN  CONST EFI_PEI_SERVICES    **PeiServices
)
{
#ifdef DE_SKU

  EFI_STATUS   Status = EFI_SUCCESS;
  //
  // register callback on "gEfiPlatformInfoGuid" PPI to get PlatformInfo Hob
  //
  Status = (**PeiServices).NotifyPpi (PeiServices, &mNotifyList1[0]);
  ASSERT_EFI_ERROR (Status);  
  //
  // Enable SC UART
  //
  IioSouthComplexUartInit(TRUE);
#endif // DE_SKU
}

#ifdef DE_SKU
VOID
GetPlatformSouthComplexPolicy(
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN OUT   SC_SETUP_INPUT       *ScInput
)
{
  EFI_PLATFORM_INFO                     *PlatformInfo;
  EFI_HOB_GUID_TYPE                     *GuidHob;

  //
  // Update the Setup variable configuration values through the PlatformInfo HOB
  //
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);

  if (PlatformInfo == NULL) {
    DEBUG((EFI_D_ERROR, "PlatformInfo HOB not found!\n"));
    DEBUG((EFI_D_ERROR, "Use the assumed default SC parameters...\n"));

    ScInput->Disable_SC_Cb3Dma = 0;
    ScInput->Disable_SC_GbE = 0;
    ScInput->SC_GbE_PF0 = 2;
    ScInput->SC_GbE_PF1 = 2;
    ScInput->DisableSCUart = 0;
    ScInput->SC_Gbe_Status = 1;  // Initialize as GBE is UP if Platform Info Hob not located
    ScInput->SC_Gbe_Status1 = 1;  // Initialize as GBE1 is UP if Platform Info Hob not located
    ScInput->Disable_SC_CPM = 0;
    ScInput->Disable_SC_GbE1 = 0;
    ScInput->SC_GbE1_PF0 = 2;
    ScInput->SC_GbE1_PF1 = 2;
    ScInput->DE_NS_SocketSkuType = DESkuID;
  } else {
    ScInput->Disable_SC_Cb3Dma = PlatformInfo->Disable_SC_Cb3Dma;
    ScInput->Disable_SC_GbE = PlatformInfo->Disable_SC_GbE;
    ScInput->SC_GbE_PF0 = PlatformInfo->SC_GbE_PF0;
    ScInput->SC_GbE_PF1 = PlatformInfo->SC_GbE_PF1;
    ScInput->DisableSCUart = PlatformInfo->DisableSCUart;
    ScInput->SC_Gbe_Status = PlatformInfo->SC_Gbe_Status;
    ScInput->SC_Gbe_Status1 = PlatformInfo->SC_Gbe_Status1;
    ScInput->Disable_SC_CPM = PlatformInfo->Disable_SC_CPM;
    ScInput->Disable_SC_GbE1 = PlatformInfo->Disable_SC_GbE1;
    ScInput->SC_GbE1_PF0 = PlatformInfo->SC_GbE1_PF0;
    ScInput->SC_GbE1_PF1 = PlatformInfo->SC_GbE1_PF1;
    ScInput->DE_NS_SocketSkuType = PlatformInfo->DE_NS_SocketSkuType;
  } 
}

UINT8
GetUncoreBusNo (
  IN UINT8	IioBusNo
)
{
  CPUBUSNO_IIO_VTD_STRUCT          CpuBusNo;

  CpuBusNo.Data = PciExpressRead32 ( PCI_LIB_ADDRESS (IioBusNo,
                                                      IIO_VTD_DEV,
                                                      IIO_VTD_FUN,
                                                      ONLY_REGISTER_OFFSET(CPUBUSNO_IIO_VTD_REG)));
  return (UINT8)CpuBusNo.Bits.bus1;
}

VOID
IioPeiInitIOSF(
  IN  UINT8	IioBusNo,
  IN  BOOLEAN	DisableSCCbDma,
  IN  BOOLEAN	DisableSCGbE,
  IN  BOOLEAN   DisableSCCPM,
  IN  BOOLEAN   DisableSCGbE1
)
{
  UINT16 IOSFBifCtrl;
  CAPID0_PCU_FUN3_BDX_STRUCT PcuF3CAP0;
  CAPID1_PCU_FUN3_HSX_BDX_STRUCT PcuF3CAP1; 
  UINT8  UncoreBusNo;

  //
  // Enable / disable the SC CBDMA and GbE ports in the IIO IOSF bridge
  //
  IOSFBifCtrl = PciExpressRead16 ( PCI_LIB_ADDRESS (IioBusNo,
                                   PCIE_PORT_2_DEV,
                                   PCIE_PORT_2A_FUNC,
                                   R_PCIE_PORT_PCIEIOUxBIFCTRL));
    
  //
  // get the uncore bus no
  //
  UncoreBusNo = GetUncoreBusNo(IIO_BUS_NUM);  

  PcuF3CAP0.Data = PciExpressRead32 ( PCI_LIB_ADDRESS (UncoreBusNo,
                                                  PCU22_DEV_NUM,
                                                  PCU22_FUNC_NUM,
                                                  ONLY_REGISTER_OFFSET(CAPID0_PCU_FUN3_REG)));

  // BDX-NS
  if (PcuF3CAP0.Bits.de_ns1s) {
    PcuF3CAP1.Data =  PciExpressRead32 ( PCI_LIB_ADDRESS (UncoreBusNo,
                                                  PCU22_DEV_NUM,
                                                  PCU22_FUNC_NUM,
                                                  ONLY_REGISTER_OFFSET(CAPID1_PCU_FUN3_REG)));
    // Storage Sku of NS
	if (PcuF3CAP1.Bits.storage_seg_sel) {
      if (PcuF3CAP1.Bits.cbdma_disabled == 0 && DisableSCCbDma == FALSE) {
        IOSFBifCtrl |= B_IOSF_PORT_CBDMA_ENABLE;
	  }
	  if (PcuF3CAP1.Bits.gbe0_disabled == 0 && DisableSCGbE == FALSE) {
        IOSFBifCtrl |= B_IOSF_PORT_GBE_ENABLE;
	  }
	  if (PcuF3CAP1.Bits.cpm_disabled == 0 && DisableSCCPM == FALSE) {
        IOSFBifCtrl |= B_IOSF_PORT_GBE1_CPM_ENABLE;
	  }
      DEBUG ((EFI_D_INFO,"NS PO MSG: Enable/disable the SC CBDMA, GbE & CPM ports in the IIO IOSF bridge...\n"));
	// Comms Sku
	} else {
		if (PcuF3CAP1.Bits.cpm_disabled == 0 && DisableSCCPM == FALSE) {
		  IOSFBifCtrl |= B_IOSF_PORT_CPM_ENABLE;
		}
		if (PcuF3CAP1.Bits.gbe0_disabled == 0 && DisableSCGbE == FALSE) {
		  IOSFBifCtrl |= B_IOSF_PORT_GBE_ENABLE;
		}
		if (PcuF3CAP1.Bits.gbe1_disabled == 0  && DisableSCGbE1 == FALSE) {
		  IOSFBifCtrl |= B_IOSF_PORT_GBE1_CPM_ENABLE;
		}
        DEBUG ((EFI_D_INFO,"NS PO MSG: Enable/disable the SC CPM, GbE & GbE1 ports in the IIO IOSF bridge...\n"));
	}
  } else { 
  
    if(DisableSCCbDma == FALSE)
      IOSFBifCtrl |= B_IOSF_PORT_CBDMA_ENABLE;  //enable the CBDMA port of IOSF 0,2,0
    if(DisableSCGbE == FALSE)
      IOSFBifCtrl |= B_IOSF_PORT_GBE_ENABLE;     //enable the GbE port of IOSF 0,2,2
      
    DEBUG ((EFI_D_INFO,"NS PO MSG: Enable/disable the SC CBDMA and GbE ports in the IIO IOSF bridge...\n"));
  }

  PciExpressWrite16 ( PCI_LIB_ADDRESS (IioBusNo,
                        PCIE_PORT_2_DEV,
                        PCIE_PORT_2A_FUNC,
                        R_PCIE_PORT_PCIEIOUxBIFCTRL),
                        IOSFBifCtrl);
  
  DEBUG ((EFI_D_INFO,"NS PO MSG: (%d,%d,%d,0x%x) = 0x%x\n",IioBusNo,
                       PCIE_PORT_2_DEV,PCIE_PORT_2A_FUNC,R_PCIE_PORT_PCIEIOUxBIFCTRL,IOSFBifCtrl));
}

VOID
IioPeiHide  (
  IN UINT8  UncoreBusNo,
  IN UINT8  DeviceNo,
  IN UINT8  FuncNo
)
{
  DEVHIDE0_UBOX_MISC_STRUCT	UboxDevHide;
  DEVHIDE0_PCU_FUN3_STRUCT	PCU3DevHide;

  UboxDevHide.Data = PciExpressRead32 ( PCI_LIB_ADDRESS (
                                        UncoreBusNo,
                                        UBOX_MISC_DEV_NUM,
                                        UBOX_MISC_FUNC_NUM,
                                        ONLY_REGISTER_OFFSET(DEVHIDE0_UBOX_MISC_REG) + (FuncNo * 4)));
  UboxDevHide.Bits.disfn |= (1 << DeviceNo);
  PciExpressWrite32 ( PCI_LIB_ADDRESS (UncoreBusNo,
                      UBOX_MISC_DEV_NUM,
                      UBOX_MISC_FUNC_NUM,
                      ONLY_REGISTER_OFFSET(DEVHIDE0_UBOX_MISC_REG) + (FuncNo * 4)),
                      UboxDevHide.Data);

  PCU3DevHide.Data = PciExpressRead32 ( PCI_LIB_ADDRESS (
                                        UncoreBusNo,
                                        PCU22_DEV_NUM,
                                        PCU22_FUNC_NUM,
                                        ONLY_REGISTER_OFFSET(DEVHIDE0_PCU_FUN3_REG) + (FuncNo * 4)));
  PCU3DevHide.Bits.dis_fn |= (1 << DeviceNo);
  PciExpressWrite32 ( PCI_LIB_ADDRESS (UncoreBusNo,
                      PCU22_DEV_NUM,
                      PCU22_FUNC_NUM,
                      ONLY_REGISTER_OFFSET(DEVHIDE0_PCU_FUN3_REG) + (FuncNo * 4)),
                      PCU3DevHide.Data);
}

BOOLEAN
Override_SC_LANPorts(
 IN UINT8    UncoreBusNo,
 IN UINT32   RegOffset,
 IN UINT8    EnableLanPort0,
 IN UINT8    EnableLanPort1
) {
  BOOLEAN OverrideStrap = FALSE;
  UINT32 PmcLanPortSel = 0;
  
  PmcLanPortSel = PciExpressRead32 (PCI_LIB_ADDRESS (UncoreBusNo, PCU7_DEV_NUM, PCU7_FUNC_NUM, ONLY_REGISTER_OFFSET(RegOffset)));
  
  if(EnableLanPort0 != SC_LAN_PF_AUTO){
    if((PmcLanPortSel & B_ENABLE_LAN_PORT0) !=  (UINT32)((EnableLanPort0 & 1) << 0) )
      OverrideStrap = TRUE;
  }
  if(EnableLanPort1 != SC_LAN_PF_AUTO){
    if((PmcLanPortSel & B_ENABLE_LAN_PORT1) !=  (UINT32)((EnableLanPort1 & 1) << 1) )
      OverrideStrap = TRUE;
   }

  if(OverrideStrap == TRUE){
    DEBUG ((EFI_D_INFO,"Override the LAN port select strap!\n"));

    if(EnableLanPort0 != SC_LAN_PF_AUTO){
       PmcLanPortSel &= ~B_ENABLE_LAN_PORT0 ;
       PmcLanPortSel |= ((EnableLanPort0 & 1) << 0);
       DEBUG ((EFI_D_INFO,"  ENABLE/DISABLE LAN_PORT_SEL 0\n"));
     }

    if(EnableLanPort1 != SC_LAN_PF_AUTO){
      PmcLanPortSel &= ~B_ENABLE_LAN_PORT1;
      PmcLanPortSel |= ((EnableLanPort1 & 1) << 1);
      DEBUG ((EFI_D_INFO,"  ENABLE/DISABLE LAN_PORT_SEL 1\n"));
    }
    
    PciExpressWrite32 (PCI_LIB_ADDRESS (UncoreBusNo, PCU7_DEV_NUM, PCU7_FUNC_NUM, ONLY_REGISTER_OFFSET(RegOffset)), PmcLanPortSel);
  }
  
  return OverrideStrap;
}


VOID
IioPeiUpdatePlatformInfoHob(
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN OUT   SC_SETUP_INPUT       *ScInput
)
{
  EFI_PLATFORM_INFO                     *PlatformInfo;
  EFI_HOB_GUID_TYPE                     *GuidHob;

  //
  // Update the Setup variable configuration values through the PlatformInfo HOB
  //
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);

  if (PlatformInfo == NULL) {
    DEBUG((EFI_D_ERROR, "PlatformInfo HOB not found!\n"));
  } else {
    PlatformInfo->SC_Gbe_Status =  ScInput->SC_Gbe_Status;
    PlatformInfo->SC_Gbe_Status1 =  ScInput->SC_Gbe_Status1;
  }
}

VOID
ProcessSC_Gbe_PF( 
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN IIO_UDS                   *IioUds  
)
{
  SC_SETUP_INPUT    ScSetupData;
  UINT8             UncoreBusNo;
  BOOLEAN           ResetRequired = FALSE;     

  //
  // get the uncore bus no
  //
  UncoreBusNo = GetUncoreBusNo(IIO_BUS_NUM);

  GetPlatformSouthComplexPolicy(PeiServices, &ScSetupData);
  //
  // Override the LAN_PORT_SEL straps if applicable (4168143)
  //
  if (ScSetupData.DE_NS_SocketSkuType < NSCommsSkuID) {  
    if(ScSetupData.SC_Gbe_Status == 1){
      if(ScSetupData.SC_GbE_PF0 != SC_LAN_PF_AUTO || ScSetupData.SC_GbE_PF1 != SC_LAN_PF_AUTO){	//overwrite the LAN_DISABLE straps
        DEBUG ((EFI_D_INFO,"overwrite the LAN_PORT_SEL straps for GbE0 on DE / NS Storage SKU!\n"));
        if(Override_SC_LANPorts(UncoreBusNo, LAN_PORT_SEL_PCU_FUN7_REG, ScSetupData.SC_GbE_PF0, ScSetupData.SC_GbE_PF1) == TRUE) {
          DEBUG ((EFI_D_INFO,"  Issuing warm reset to overwrite the LAN_PORT_SEL straps...\n"));
          ResetRequired = TRUE;
        }
      }   
    }
  } 
  if (ScSetupData.DE_NS_SocketSkuType == NSCommsSkuID) {
    if (ScSetupData.SC_Gbe_Status == 1) {
      if(ScSetupData.SC_GbE_PF0 != SC_LAN_PF_AUTO || ScSetupData.SC_GbE_PF1 != SC_LAN_PF_AUTO) {	//overwrite the LAN_DISABLE straps
        DEBUG ((EFI_D_INFO,"NS PO MSG: overwrite the LAN_PORT_SEL straps for GbE0 on NS COMMS SKU!\n"));
        if(Override_SC_LANPorts(UncoreBusNo, LAN_PORT_SEL_PCU_FUN7_REG, ScSetupData.SC_GbE_PF0, ScSetupData.SC_GbE_PF1) == TRUE) {
          DEBUG ((EFI_D_INFO,"  Issuing warm reset to overwrite the LAN_PORT_SEL straps...\n"));
          ResetRequired = TRUE;
        }
      }        
    }
    if (ScSetupData.SC_Gbe_Status1 == 1) {
      if(ScSetupData.SC_GbE1_PF0 != SC_LAN_PF_AUTO || ScSetupData.SC_GbE1_PF1 != SC_LAN_PF_AUTO) {	//overwrite the LAN_DISABLE straps
        DEBUG ((EFI_D_INFO,"NS PO MSG: overwrite the LAN_PORT_SEL straps for GbE1 on NS COMMS SKU!\n"));
        if(Override_SC_LANPorts(UncoreBusNo, LAN1_PORT_SEL_PCU_FUN7_REG, ScSetupData.SC_GbE1_PF0, ScSetupData.SC_GbE1_PF1) == TRUE) {
          DEBUG ((EFI_D_INFO,"  Issuing warm reset to overwrite the LAN_PORT_SEL straps...\n"));
          ResetRequired = TRUE;
        }
      }        
    }
  }
  
  if (ResetRequired) {
    IoWrite8 (0xcf9, 0x6);  
  }
}

EFI_STATUS
EFIAPI
IioSouthComplexPeiInit( 
  IN  CONST EFI_PEI_SERVICES      **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
)
{
  SC_RESET_STS_PCU_FUN1_STRUCT     SCResetSts;
  UINT8    UncoreBusNo;
  BOOLEAN  DisableSCGbE = FALSE;
  BOOLEAN  DisableSCCbDma = FALSE;
  BOOLEAN  DisableSCCPM = FALSE;
  BOOLEAN  DisableSCGbE1 = FALSE;
  BOOLEAN  HideIOSF0 = FALSE;
  BOOLEAN  HideIOSF1 = FALSE;
  BOOLEAN  HideIOSF2 = FALSE;
  SC_SETUP_INPUT    ScSetupData;

  DEBUG((EFI_D_INFO, "DEBUG::::  IioSouthComplexPeiInit()\n"));
  //
  // Get the Platform configuration info
  //
  GetPlatformSouthComplexPolicy(PeiServices, &ScSetupData);
  
  //
  // get the uncore bus no
  //
  UncoreBusNo = GetUncoreBusNo(IIO_BUS_NUM);
  //
  // Disable SC UART based on platformInfo Hob 
  //
  if (ScSetupData.DisableSCUart){
  	DEBUG((EFI_D_INFO, "DEBUG:::: Disable SC UART \n"));
    IioSouthComplexUartInit(FALSE);
  }
  //
  // Check the SC HW reset status
  //
  SCResetSts.Data = PciExpressRead32 ( PCI_LIB_ADDRESS (UncoreBusNo,
                                                        PCU1_DEV_NUM,
                                                        PCU1_FUNC_NUM,
                                                        ONLY_REGISTER_OFFSET(SC_RESET_STS_PCU_FUN1_REG)));
  DEBUG ((EFI_D_INFO,"NS PO MSG: SC_RESET_STS: %x!!!\n", SCResetSts.Data));
  if(SCResetSts.Bits.sc_bypass){	//SC bypass defeature mode set (4985895)
    if(ScSetupData.DE_NS_SocketSkuType == DESkuID) {
      DEBUG ((EFI_D_INFO,"SC bypass defeature mode set for DE!!!\n"));
      DisableSCGbE = DisableSCCbDma = TRUE;
      HideIOSF0 = HideIOSF1 = TRUE;
    } else if (ScSetupData.DE_NS_SocketSkuType == NSStorageSkuID) {
      DEBUG ((EFI_D_INFO,"NS PO MSG: SC bypass defeature mode set for NS Storage SKU!!!\n"));
      DisableSCGbE = DisableSCCbDma = DisableSCCPM = TRUE;
      HideIOSF0 = HideIOSF1 = HideIOSF2 = TRUE; 
    } else if (ScSetupData.DE_NS_SocketSkuType == NSCommsSkuID) {
      DEBUG ((EFI_D_INFO,"NS PO MSG: SC bypass defeature mode set for NS COMMS SKU!!!\n"));
      DisableSCGbE = DisableSCGbE1 = DisableSCCPM = TRUE;
      HideIOSF0 = HideIOSF1 = HideIOSF2 = TRUE; 
    }
    //
    // 4987999 : [BDX-DE-U0-PO] Dev 30 F7 is not hidden in BIOS when SC is in bypass mode
    //
    IioPeiHide(UncoreBusNo, PCU7_DEV_NUM, PCU7_FUNC_NUM);
  }
  else if(SCResetSts.Bits.gbe0_init_failed){		//SC GbE0 init failed (4167489)
    DEBUG ((EFI_D_INFO,"NS PO MSG: SC GbE0 init failed!!!\n"));
    DisableSCGbE = TRUE;
    HideIOSF1 = TRUE;
  }
  else if (SCResetSts.Bits.gbe1_init_failed) {
    DEBUG ((EFI_D_INFO,"NS PO MSG: SC GbE1 init failed!!!\n"));    //SC GbE1 init failed
    DisableSCGbE1 = TRUE;
    HideIOSF2 = TRUE; 
  }
  else if(SCResetSts.Bits.sc_init_failed){		//SC init failed (4167489)
    if(ScSetupData.DE_NS_SocketSkuType == DESkuID) {
      DEBUG ((EFI_D_INFO,"SC init failed on DE!!!\n"));
      DisableSCGbE = DisableSCCbDma = TRUE;
      HideIOSF0 = HideIOSF1 = TRUE;
    } else if (ScSetupData.DE_NS_SocketSkuType == NSStorageSkuID) {
      DEBUG ((EFI_D_INFO,"NS PO MSG: SC init failed on NS Storage SKU!!!\n"));
      DisableSCGbE = DisableSCCbDma = DisableSCCPM = TRUE;
      HideIOSF0 = HideIOSF1 = HideIOSF2 = TRUE; 
    } else if (ScSetupData.DE_NS_SocketSkuType == NSCommsSkuID) {
      DEBUG ((EFI_D_INFO,"NS PO MSG: SC init failed on NS COMMS SKU!!!\n"));
      DisableSCGbE = DisableSCGbE1 = DisableSCCPM = TRUE;
      HideIOSF0 = HideIOSF1 = HideIOSF2 = TRUE; 
    }
  }
  if(DisableSCCbDma == FALSE){
    if(ScSetupData.Disable_SC_Cb3Dma){		//user forcing complete SC CB DMA disabled (4168144)
      DEBUG ((EFI_D_INFO,"user forcing complete SC CB DMA disabled!!!\n"));
      DisableSCCbDma = TRUE;
    }
  }
  if(DisableSCGbE == FALSE){
    if(ScSetupData.Disable_SC_GbE){		//user forcing complete SC GbE device disabled (4168145)
      DEBUG ((EFI_D_INFO,"user forcing complete SC GbE device disabled!!!\n"));
      DisableSCGbE = TRUE;
    }
  }
  if(DisableSCCPM == FALSE){
    if(ScSetupData.Disable_SC_CPM){		//user forcing complete SC CPM device disabled
      DEBUG ((EFI_D_INFO,"NS PO MSG: user forcing complete SC CPM device disabled!!!\n"));
      DisableSCCPM = TRUE;
    }      
  }
  if(DisableSCGbE1 == FALSE){
    if(ScSetupData.Disable_SC_GbE1){		//user forcing complete SC GbE1 device disabled
      DEBUG ((EFI_D_INFO,"NS PO MSG: user forcing complete SC GbE1 device disabled!!!\n"));
      DisableSCGbE1 = TRUE;
    }      
  }

  //
  // Init the IIO IOSF bridge (BDF 0,2,0/2) on DE / (BDF 0,2,0/2/3) on DE NS connected to the SC 
  //
  IioPeiInitIOSF(IIO_BUS_NUM, DisableSCCbDma, DisableSCGbE, DisableSCCPM, DisableSCGbE1);

  //
  // Hide the SC CBDMA/GbE IOSF bridge
  //
  if(HideIOSF0 == TRUE){	//hide the IOSF 0,2,0 (SC CBDMA port)
    DEBUG ((EFI_D_INFO,"Hiding the IOSF 0,2,0 (SC CBDMA port)!\n"));
    IioPeiHide(UncoreBusNo, PCIE_PORT_2_DEV, PCIE_PORT_2A_FUNC);
    IioPeiHide(UncoreBusNo, PCIE_PORT_2_DEV, PCIE_PORT_2B_FUNC);
  }
  if (ScSetupData.DE_NS_SocketSkuType == DESkuID) {
    if(HideIOSF1 == TRUE){	//hide the IOSF 0,2,2 (SC GbE port)
      DEBUG ((EFI_D_INFO,"Hiding the IOSF 0,2,2 (SC GbE port) for DE SKU!\n"));
      IioPeiHide(UncoreBusNo, PCIE_PORT_2_DEV, PCIE_PORT_2C_FUNC);
      IioPeiHide(UncoreBusNo, PCIE_PORT_2_DEV, PCIE_PORT_2D_FUNC);
    }
  } else if (ScSetupData.DE_NS_SocketSkuType == NSStorageSkuID) {
    if(HideIOSF1 == TRUE){	//hide the IOSF 0,2,2 (SC GbE port)
      DEBUG ((EFI_D_INFO,"NS PO MSG: Hiding the IOSF 0,2,2 (SC GbE port) for NS Storage SKU!\n"));
      IioPeiHide(UncoreBusNo, PCIE_PORT_2_DEV, PCIE_PORT_2C_FUNC);
    } 
  } else if (ScSetupData.DE_NS_SocketSkuType == NSCommsSkuID) {
    if(HideIOSF1 == TRUE){	//hide the IOSF 0,2,2 (SC GbE port)
      DEBUG ((EFI_D_INFO,"NS PO MSG: Hiding the IOSF 0,2,2 (SC GbE0 port) for NS COMMS SKU!\n"));
      IioPeiHide(UncoreBusNo, PCIE_PORT_2_DEV, PCIE_PORT_2C_FUNC);
    }   
    if(HideIOSF2 == TRUE){
      DEBUG ((EFI_D_INFO,"NS PO MSG: Hiding the IOSF 0,2,3 (SC GbE1 port) for NS COMMS SKU!!\n"));
      IioPeiHide(UncoreBusNo, PCIE_PORT_2_DEV, PCIE_PORT_2D_FUNC);
    }  
  } 
  
  //
  // Update GBE status in Platform Info Hob 
  //
  if(DisableSCGbE == FALSE && HideIOSF1 == FALSE) {
    ScSetupData.SC_Gbe_Status = 1;   // GBE up
  } else {
    ScSetupData.SC_Gbe_Status = 0;   // GBE down
  }
  
  if(DisableSCGbE1 == FALSE && HideIOSF2 == FALSE) {
    ScSetupData.SC_Gbe_Status1 = 1;   // GBE1 up
  } else {
    ScSetupData.SC_Gbe_Status1 = 0;   // GBE1 down
  }

  IioPeiUpdatePlatformInfoHob(PeiServices, &ScSetupData);
  
  return EFI_SUCCESS;
}

VOID
IioSouthComplexUartInit(
  BOOLEAN EnableSCUart
)
/*++

Routine Description:

  Enables/Disables SC UART 
  
Arguments:

  EnableSCUart  - TRUE for Enabling , FALSE for Disabling .
  
--*/
{
  IRP_MISC_DFX1_IIO_VTD_BDX_STRUCT          IrpMiscDfx1;
  UART_ENABLE_UBOX_MISC_STRUCT              UboxUartEnable;
  SC_RESET_STS_PCU_FUN1_STRUCT              SCResetSts;
  UINT8                                     UncoreBusNo;

  //
  // get the uncore bus no
  //
  UncoreBusNo = GetUncoreBusNo(IIO_BUS_NUM);

  // Steps to enable/disable SC Uart
  // 1. Check for SC Bypass Mode , if set return i.e. use LPC uart else continue enabling the SC uart
  // 2. Disable/Enable UART Range in IIO first
  // 3. Enable/Disable UART range in UBOX
  
  //
  // 1. Check the SC bypass mode 
  //
  SCResetSts.Data = PciExpressRead32 ( PCI_LIB_ADDRESS (UncoreBusNo,
                                                        PCU1_DEV_NUM,
                                                        PCU1_FUNC_NUM,
                                                        ONLY_REGISTER_OFFSET(SC_RESET_STS_PCU_FUN1_REG)));

  if(SCResetSts.Bits.sc_bypass)
    return; // continue with LPC UART if in SC bypass mode else enable the SC Uart
  
  // 
  // 2. Disable/Enable UART Range in IIO first
  //
  IrpMiscDfx1.Data = PciExpressRead32 ( PCI_LIB_ADDRESS (IIO_BUS_NUM,
                                                         IIO_VTD_DEV,
                                                         IIO_VTD_FUN,
                                                         ONLY_REGISTER_OFFSET(IRP_MISC_DFX1_IIO_VTD_REG)));
  
  IrpMiscDfx1.Bits.disable_io_range_0x3f8_0x3ff = EnableSCUart;
  IrpMiscDfx1.Bits.disable_io_range_0x2f8_0x2ff = EnableSCUart;
 
  PciExpressWrite32 ( PCI_LIB_ADDRESS (IIO_BUS_NUM,
                                       IIO_VTD_DEV,
                                       IIO_VTD_FUN,
                                       ONLY_REGISTER_OFFSET(IRP_MISC_DFX1_IIO_VTD_REG)), IrpMiscDfx1.Data);

  // 
  // 3. Enable/Disable UART range in UBOX
  //
  UboxUartEnable.Data = PciExpressRead32 ( PCI_LIB_ADDRESS (UncoreBusNo,
                                                            UBOX_MISC_DEV_NUM,
                                                            UBOX_MISC_FUNC_NUM,
                                                            ONLY_REGISTER_OFFSET(UART_ENABLE_UBOX_MISC_REG)));
  UboxUartEnable.Bits.enableuartport0 = EnableSCUart;
  UboxUartEnable.Bits.enableuartport1 = EnableSCUart;

  PciExpressWrite32 ( PCI_LIB_ADDRESS (UncoreBusNo,
                                       UBOX_MISC_DEV_NUM,
                                       UBOX_MISC_FUNC_NUM,
                                       ONLY_REGISTER_OFFSET(UART_ENABLE_UBOX_MISC_REG)),UboxUartEnable.Data);
}
#endif // DE_SKU

UINT32 
Bios2PcodeMailboxCommand(
  UINT8  UncoreBusNo,
  UINT32 MailboxCmd,
  UINT32 MailboxData
  )
{
  UINT32 MailboxStatus;
  //
  // Poll Mailbox BUSY_RUN flag until clear (ready)
  //
  while ( PciExpressRead32(PCI_LIB_ADDRESS (UncoreBusNo, PCU1_DEV_NUM, PCU1_FUNC_NUM, BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG)) & BIT31) {}
  //
  // Write the mailbox command
  //
  PciExpressWrite32 ( PCI_LIB_ADDRESS (UncoreBusNo, PCU1_DEV_NUM, PCU1_FUNC_NUM, BIOS_MAILBOX_DATA_PCU_FUN1_REG), MailboxData);
  PciExpressWrite32 ( PCI_LIB_ADDRESS (UncoreBusNo, PCU1_DEV_NUM, PCU1_FUNC_NUM, BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG), MailboxCmd | BIT31);
  //
  // Poll Mailbox BUSY_RUN flag until clear (ready)
  //
  while ( PciExpressRead32(PCI_LIB_ADDRESS (UncoreBusNo, PCU1_DEV_NUM, PCU1_FUNC_NUM, BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG)) & BIT31) {}
  //
  // Read Mailbox Status
  //
  MailboxStatus = PciExpressRead32(PCI_LIB_ADDRESS (UncoreBusNo, PCU1_DEV_NUM, PCU1_FUNC_NUM, BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG));

  return MailboxStatus;
}
