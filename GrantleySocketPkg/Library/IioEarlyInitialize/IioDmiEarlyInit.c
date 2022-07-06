/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  IioDmiEarlyInit.c

Abstract:

  Houses all code related to Initialize DFX 

Revision History:

--*/

#include "IioEarlyInitialize.h"
#ifdef  DE_SKU
#include "uart16550.h"
#endif

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

IIO_STATUS
IioGetDmiVcTcSettings(
  IIO_GLOBALS               *IioGlobalData
  )
{

  if ( IioGlobalData->IioVData.IsocEnable == 0) {
     IioGlobalData->IioVData.DmiVc[DmiTypeVc1] = 0;
     IioGlobalData->IioVData.DmiVc[DmiTypeVcp] = 0;
     if ( IioGlobalData->IioVData.MeSegEn == 0){
        IioGlobalData->IioVData.DmiVc[DmiTypeVcm] = 0;
     } else { /// Use configured value.
       IioGlobalData->IioVData.DmiVc[DmiTypeVcm] = IioGlobalData->SetupData.DmiVcm;
     }
  } else {
    /// Use what is configured.
    IioGlobalData->IioVData.DmiVc[DmiTypeVc1] = IioGlobalData->SetupData.DmiVc1;
    IioGlobalData->IioVData.DmiVc[DmiTypeVcp] = IioGlobalData->SetupData.DmiVcp;
    IioGlobalData->IioVData.DmiVc[DmiTypeVcm] = IioGlobalData->SetupData.DmiVcm;
  }
  return IIO_SUCCESS;
}


VOID
InitDmiSpecificSiWorkaorunds  (
  IIO_GLOBALS  *IioGlobalData
)
{
}

IIO_STATUS
IioSetDmiTcVcMapping (
   IIO_GLOBALS         *IioGlobalData,
   UINT64              DmiBar
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
  UINT8   VcMap[MaxDmiVcType] = {0};

  //
  // Set the TC/VC mappings
  //
  for (Index = 0; Index < MaxDmiTcType; Index++) {
    VcMap[IioGlobalData->IioVData.DmiTc[Index]] |= (BIT0 << Index);
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
  for (Index = 0; Index < MaxDmiVcType; Index++) {
    if(IioGlobalData->IioVData.DmiVc[Index]){
      //
      // Map TCs to VC, Set the VC ID, Enable VC
      //

      VcId = IioGlobalData->IioVData.DmiVcId[Index];

      Data32And = (UINT32) (~(B_IIO_RCRB_DMIVCxRCTL_VCx_ID_MASK | B_IIO_RCRB_DMIVCxRCTL_VCx_TCVCxM_MASK));
      Data32Or = VcId << 24;
      Data32Or |= VcMap[Index];
      Data32Or |= B_IIO_RCRB_DMIVCxRCTL_VCx_EN;

      switch (Index) {
      case DmiTypeVc0:
        Register = R_IIO_RCRB_DMIVC0RCTL;
        break;

      case DmiTypeVc1:
        Register = R_IIO_RCRB_DMIVC1RCTL;
        break;

      case DmiTypeVcp:
        Register = R_IIO_RCRB_DMIVCPRCTL;
        break;

      case DmiTypeVcm:
        Register = R_IIO_RCRB_DMIVCMRCTL;
        break;

      default:
        return IIO_INVALID_PARAMETER;
      }

      IioMmioAndThenOr32 (IioGlobalData, (UINT32)(DmiBar + Register), Data32And, Data32Or);
    }
  }

  //
  // 6.2.3.1 b. System BIOS must program the extended VC Count:
  //    Set the DMI Port Register DMIBAR Offset 004h[2:0]=001b
  //
  Data8And = (UINT8) (~0x07);
  if(IioGlobalData->IioVData.DmiVc[DmiTypeVc1]){
    Data8Or = 1;
  } else {
    Data8Or = 0;
  }

  IioMmioAndThenOr8 (IioGlobalData, (UINT32)(DmiBar + R_IIO_RCRB_DMIPVCCAP1), Data8And, Data8Or);

  //
  //4167453 SB traffic on VCp does not generate critical priority on ring
  //
  if(IioGlobalData->SetupData.WA_4167453 ) {
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR, "WA_4167453 IIO side!\n");
    //1. Disable VCP in the IIO side 2. 
    Register = R_IIO_RCRB_DMIVCPRCTL;
    Data32And = (UINT32) (~(B_IIO_RCRB_DMIVCxRCTL_VCx_ID_MASK | B_IIO_RCRB_DMIVCxRCTL_VCx_TCVCxM_MASK | B_IIO_RCRB_DMIVCxRCTL_VCx_EN));
    Data32Or=0x0;
     IioMmioAndThenOr32 (IioGlobalData, (UINT32)(DmiBar + Register), Data32And, Data32Or);
    
    //2. Set VC1 ID = 01 3. Set TC=2
    Register = R_IIO_RCRB_DMIVC1RCTL;
    Data32And = (UINT32) (~(B_IIO_RCRB_DMIVCxRCTL_VCx_ID_MASK | B_IIO_RCRB_DMIVCxRCTL_VCx_TCVCxM_MASK ));
    Data32Or = (0x01 << 24);  //VCID = 01
    Data32Or |= (0x02 << 01);  //TC = 2
    IioMmioAndThenOr32 (IioGlobalData, (UINT32)(DmiBar + Register), Data32And, Data32Or);
  }

  return IIO_SUCCESS;
}


IIO_STATUS
IioPollDmiVcStatus (
    IIO_GLOBALS        *IioGlobalData,
    UINT64              DmiBar
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
  for (Index = 0; Index < MaxDmiVcType; Index ++) {
   if(IioGlobalData->IioVData.DmiVc[Index]){
      switch (Index) {
      case DmiTypeVc0:
        Register = R_IIO_RCRB_DMIVC0RSTS;
        break;

      case DmiTypeVc1:
        Register = R_IIO_RCRB_DMIVC1RSTS;
        break;

      case DmiTypeVcp:
        Register = R_IIO_RCRB_DMIVCPRSTS;
        break;

      case DmiTypeVcm:
        Register = R_IIO_RCRB_DMIVCMRSTS;
        break;

      default:
        return IIO_INVALID_PARAMETER;
      }
      if (CheckSoftSimulationPlatform() == FALSE) {
        //
        // Wait for negotiation to complete
        //

        while ((IioMmioRead16(IioGlobalData, (UINT32)(DmiBar + Register)) & B_IIO_RCRB_DMIVCxRSTS_VCx_NP) != 0);
      }
    }
  }

  return IIO_SUCCESS;
}

#ifdef DE_SKU
VOID
GetPlatformSouthComplexPolicy(
  IIO_GLOBALS *IioGlobalData,
  SC_SETUP_INPUT       *ScInput
)
{
    IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO, "Use the assumed default SC parameters...\n");
    ScInput->Disable_SC_Cb3Dma = 0;
    ScInput->Disable_SC_GbE = 0;
    ScInput->SC_GbE_PF0 = 2;
    ScInput->SC_GbE_PF1 = 2;
    ScInput->DisableSCUart = 0;
    ScInput->SC_Gbe_Status = 1;  // Initialize as GBE is UP if Platform Info Hob not located
    ScInput->Disable_SC_CPM = 0;
    ScInput->Disable_SC_GbE1 = 0;
}

UINT8
GetUncoreBusNo (
  IIO_GLOBALS *IioGlobalData,
  UINT8 IioBusNo
)
{
  CPUBUSNO_IIO_VTD_STRUCT          CpuBusNo;

  CpuBusNo.Data = IioPciExpressRead32 ( IioGlobalData, PCI_PCIE_ADDR (IioBusNo,
                                                      IIO_VTD_DEV,
                                                      IIO_VTD_FUN,
                                                      ONLY_REGISTER_OFFSET(CPUBUSNO_IIO_VTD_REG)));
  return (UINT8)CpuBusNo.Bits.bus1;
}

UINT32 
Bios2PcodeMailboxCommand(
  IIO_GLOBALS *IioGlobalData,
  UINT8  UncoreBusNo,
  UINT32 MailboxCmd,
  UINT32 MailboxData
  )
{
  UINT32 MailboxStatus;
  //
  // Poll Mailbox BUSY_RUN flag until clear (ready)
  //
  while ( IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR (UncoreBusNo, PCU1_DEV_NUM, PCU1_FUNC_NUM, BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG)) & BIT31) {}
  //
  // Write the mailbox command
  //
  IioPciExpressWrite32 ( IioGlobalData, PCI_PCIE_ADDR (UncoreBusNo, PCU1_DEV_NUM, PCU1_FUNC_NUM, BIOS_MAILBOX_DATA_PCU_FUN1_REG), MailboxData);
  IioPciExpressWrite32 ( IioGlobalData, PCI_PCIE_ADDR (UncoreBusNo, PCU1_DEV_NUM, PCU1_FUNC_NUM, BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG), MailboxCmd | BIT31);
  //
  // Poll Mailbox BUSY_RUN flag until clear (ready)
  //
  while ( IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR (UncoreBusNo, PCU1_DEV_NUM, PCU1_FUNC_NUM, BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG)) & BIT31) {}
  //
  // Read Mailbox Status
  //
  MailboxStatus = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR (UncoreBusNo, PCU1_DEV_NUM, PCU1_FUNC_NUM, BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG));

  return MailboxStatus;
}

VOID
IioPeiInitIOSF(
  IIO_GLOBALS *IioGlobalData,
  UINT8 IioBusNo,
  BOOLEAN  DisableSCCbDma,
  BOOLEAN DisableSCGbE
)
{
  UINT16 IOSFBifCtrl;
  //
  // Enable / disable the SC CBDMA and GbE ports in the IIO IOSF bridge
  //
  IOSFBifCtrl = IioPciExpressRead16 ( IioGlobalData, PCI_PCIE_ADDR (IioBusNo,
                                   PCIE_PORT_2_DEV,
                                   PCIE_PORT_2A_FUNC,
                                   R_PCIE_PORT_PCIEIOUxBIFCTRL));
  if(DisableSCCbDma == FALSE)
    IOSFBifCtrl |= B_IOSF_PORT_CBDMA_ENABLE;  //enable the CBDMA port of IOSF 0,2,0
  if(DisableSCGbE == FALSE)
    IOSFBifCtrl |= B_IOSF_PORT_GBE_ENABLE;     //enable the GbE port of IOSF 0,2,2
    IioPciExpressWrite16 ( IioGlobalData, PCI_PCIE_ADDR (IioBusNo,
                        PCIE_PORT_2_DEV,
                        PCIE_PORT_2A_FUNC,
                        R_PCIE_PORT_PCIEIOUxBIFCTRL),
                        IOSFBifCtrl);
    IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO,"Enable/disable the SC CBDMA and GbE ports in the IIO IOSF bridge...");
    IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO,"(%d,%d,%d,0x%x) = 0x%x\n",IioBusNo,
                       PCIE_PORT_2_DEV,PCIE_PORT_2A_FUNC,R_PCIE_PORT_PCIEIOUxBIFCTRL,IOSFBifCtrl);
}

VOID
IioPeiHide  (
  IIO_GLOBALS *IioGlobalData,
  UINT8  UncoreBusNo,
  UINT8  DeviceNo,
  UINT8  FuncNo
)
{
  DEVHIDE0_UBOX_MISC_STRUCT	UboxDevHide;
  DEVHIDE0_PCU_FUN3_STRUCT	PCU3DevHide;

  UboxDevHide.Data = IioPciExpressRead32 ( IioGlobalData, PCI_PCIE_ADDR (
                                        UncoreBusNo,
                                        UBOX_MISC_DEV_NUM,
                                        UBOX_MISC_FUNC_NUM,
                                        ONLY_REGISTER_OFFSET(DEVHIDE0_UBOX_MISC_REG) + (FuncNo * 4)));
  UboxDevHide.Bits.disfn |= (1 << DeviceNo);
  IioPciExpressWrite32 ( IioGlobalData, PCI_PCIE_ADDR (UncoreBusNo,
                      UBOX_MISC_DEV_NUM,
                      UBOX_MISC_FUNC_NUM,
                      ONLY_REGISTER_OFFSET(DEVHIDE0_UBOX_MISC_REG) + (FuncNo * 4)),
                      UboxDevHide.Data);

  PCU3DevHide.Data = IioPciExpressRead32 ( IioGlobalData, PCI_PCIE_ADDR (
                                        UncoreBusNo,
                                        PCU22_DEV_NUM,
                                        PCU22_FUNC_NUM,
                                        ONLY_REGISTER_OFFSET(DEVHIDE0_PCU_FUN3_REG) + (FuncNo * 4)));
  PCU3DevHide.Bits.dis_fn |= (1 << DeviceNo);
  IioPciExpressWrite32 ( IioGlobalData, PCI_PCIE_ADDR (UncoreBusNo,
                      PCU22_DEV_NUM,
                      PCU22_FUNC_NUM,
                      ONLY_REGISTER_OFFSET(DEVHIDE0_PCU_FUN3_REG) + (FuncNo * 4)),
                      PCU3DevHide.Data);
}

BOOLEAN
Override_SC_LANPorts(
 IIO_GLOBALS *IioGlobalData,
 UINT8 UncoreBusNo,
 UINT8 EnableLanPort0,
 UINT8 EnableLanPort1
)
{
  UINT32 PmcLanPortSel = 0;
  BOOLEAN OverrideStrap = FALSE;
  UINT32  MailboxStatus;

  //
  // Read PMC_LAN_PORT_SEL
  //
  MailboxStatus = Bios2PcodeMailboxCommand( IioGlobalData, UncoreBusNo, MAILBOX_BIOS_CMD_READ_PMC_LAN_PORT_SEL, 0);

  if (MailboxStatus == 0) {
      PmcLanPortSel = IioPciExpressRead32 ( IioGlobalData, PCI_PCIE_ADDR (UncoreBusNo, PCU1_DEV_NUM, PCU1_FUNC_NUM, BIOS_MAILBOX_DATA_PCU_FUN1_REG));
  }

  if(EnableLanPort0 != SC_LAN_PF_AUTO){
    if((PmcLanPortSel & B_ENABLE_LAN_PORT0) !=  (UINT32)((EnableLanPort0 & 1) << 0) )
        OverrideStrap = TRUE;
  }
  if(EnableLanPort1 != SC_LAN_PF_AUTO){
    if((PmcLanPortSel & B_ENABLE_LAN_PORT1) !=  (UINT32)((EnableLanPort1 & 1) << 1) )
        OverrideStrap = TRUE;
   }

  if(OverrideStrap == TRUE){
    IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO, "Override the LAN port select strap!\n");

    if(EnableLanPort0 != SC_LAN_PF_AUTO){
       PmcLanPortSel &= ~B_ENABLE_LAN_PORT0 ;
       PmcLanPortSel |= ((EnableLanPort0 & 1) << 0);
       IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO,"  ENABLE/DISABLE LAN_PORT_SEL 0\n");
     }

    if(EnableLanPort1 != SC_LAN_PF_AUTO){
      PmcLanPortSel &= ~B_ENABLE_LAN_PORT1;
      PmcLanPortSel |= ((EnableLanPort1 & 1) << 1);
      IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO,"  ENABLE/DISABLE LAN_PORT_SEL 1\n");
    }
   //
   // Write PMC_LAN_PORT_SEL
   //
   MailboxStatus = Bios2PcodeMailboxCommand(IioGlobalData,UncoreBusNo, MAILBOX_BIOS_CMD_WRITE_PMC_LAN_PORT_SEL, PmcLanPortSel);
  }
  return OverrideStrap;
}

VOID
ProcessSC_Gbe_PF( 
  IIO_GLOBALS *IioGlobalData
)
{
  SC_SETUP_INPUT    ScSetupData;
  UINT8             UncoreBusNo;     

  //
  // get the uncore bus no
  //
  UncoreBusNo = GetUncoreBusNo(IioGlobalData, IIO_BUS_NUM);

  GetPlatformSouthComplexPolicy(IioGlobalData, &ScSetupData);
  //
  // Override the LAN_PORT_SEL straps if applicable (4168143)
  //
  if(ScSetupData.SC_Gbe_Status == 1 ){
    if(ScSetupData.SC_GbE_PF0 != SC_LAN_PF_AUTO || ScSetupData.SC_GbE_PF1 != SC_LAN_PF_AUTO){	//overwrite the LAN_DISABLE straps
      IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO,"overwrite the LAN_PORT_SEL straps!\n");
      if(Override_SC_LANPorts( IioGlobalData, UncoreBusNo, ScSetupData.SC_GbE_PF0, ScSetupData.SC_GbE_PF1) == TRUE) {
        IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO,"  Issuing warm reset to overwrite the LAN_PORT_SEL straps...\n");
        IioIoWrite8 (IioGlobalData, 0xcf9, 0x6);
      }
    }
  }
}

#ifdef  DE_SKU


UINT16 gComBase  = 0x3f8;
UINTN  gBps      = 115200;
UINT8  gData     = 8;
UINT8  gStop     = 1;
UINT8  gParity   = 0;
UINT8  gBreakSet = 0;
#define BDX_DE_V1_DIVISOR 4
#define BDX_DE_DIVISOR    1


VOID SerialPortInitialize (
           IIO_GLOBALS *IioGlobalData,
           UINT8              CPUType,
           UINT8             Stepping
           )
/*++

Routine Description:

  Initialize Serial Port

    The Baud Rate Divisor registers are programmed and the LCR
    is used to configure the communications format. Hard coded
    UART config comes from globals in DebugSerialPlatform lib.

Arguments:

  None

Returns:

  None

--*/
{
  UINTN Divisor;
  UINT8 OutputData;
  UINT8 Data;

  //
  // Some init is done by the platform status code initialization.
  //
  //
  // Map 5..8 to 0..3
  //
  Data = (UINT8) (gData - (UINT8) 5);

  //
  // Calculate divisor for baud generator
  //
//  Divisor = 115200 / gBps;

  if((CPUType == CPU_BDX) && (Stepping  == C0_REV_HSX))
  	Divisor= BDX_DE_V1_DIVISOR;// 153600/38400;
  else
  	Divisor=BDX_DE_DIVISOR;

  //
  // Set communications format
  //
  OutputData = (UINT8) (DIVISOR_LATCH_ACCESS | ((gBreakSet << 6) | ((gParity << 3) | ((gStop << 2) | Data))));
  IioIoWrite8 (IioGlobalData, gComBase + LINE_CONTROL, OutputData);

  //
  // Configure baud rate
  //
  IioIoWrite8 (IioGlobalData, gComBase + DIVISOR_LATCH_HIGH, (UINT8) (Divisor >> 8));
  IioIoWrite8 (IioGlobalData, gComBase + DIVISOR_LATCH_LOW, (UINT8) (Divisor & 0xff));

  //
  // Switch back to bank 0
  //
  OutputData = (UINT8) ( (DIVISOR_LATCH_ACCESS & ~DIVISOR_LATCH_ACCESS) | ((gBreakSet << 6) | ((gParity << 3) | ((gStop << 2) | Data))));
  IioIoWrite8 (IioGlobalData, gComBase + LINE_CONTROL, OutputData);

}

#endif

VOID
IioSouthComplexUartInit(
  IIO_GLOBALS *IioGlobalData,
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
  UncoreBusNo = GetUncoreBusNo( IioGlobalData, IIO_BUS_NUM);

  // Steps to enable/disable SC Uart
  // 1. Check for SC Bypass Mode , if set return i.e. use LPC uart else continue enabling the SC uart
  // 2. Disable/Enable UART Range in IIO first
  // 3. Enable/Disable UART range in UBOX
  
  //
  // 1. Check the SC bypass mode 
  //
  SCResetSts.Data = IioPciExpressRead32 ( IioGlobalData, PCI_PCIE_ADDR (UncoreBusNo,
                                                        PCU1_DEV_NUM,
                                                        PCU1_FUNC_NUM,
                                                        ONLY_REGISTER_OFFSET(SC_RESET_STS_PCU_FUN1_REG)));

  if(SCResetSts.Bits.sc_bypass)
    return; // continue with LPC UART if in SC bypass mode else enable the SC Uart
  
  // 
  // 2. Disable/Enable UART Range in IIO first
  //
  IrpMiscDfx1.Data = IioPciExpressRead32 ( IioGlobalData, PCI_PCIE_ADDR (IIO_BUS_NUM,
                                                         IIO_VTD_DEV,
                                                         IIO_VTD_FUN,
                                                         ONLY_REGISTER_OFFSET(IRP_MISC_DFX1_IIO_VTD_REG)));
  
  IrpMiscDfx1.Bits.disable_io_range_0x3f8_0x3ff = EnableSCUart;
  IrpMiscDfx1.Bits.disable_io_range_0x2f8_0x2ff = EnableSCUart;
 
  IioPciExpressWrite32 ( IioGlobalData, PCI_PCIE_ADDR (IIO_BUS_NUM,
                                       IIO_VTD_DEV,
                                       IIO_VTD_FUN,
                                       ONLY_REGISTER_OFFSET(IRP_MISC_DFX1_IIO_VTD_REG)), IrpMiscDfx1.Data);

  // 
  // 3. Enable/Disable UART range in UBOX
  //
  UboxUartEnable.Data = IioPciExpressRead32 ( IioGlobalData, PCI_PCIE_ADDR (UncoreBusNo,
                                                            UBOX_MISC_DEV_NUM,
                                                            UBOX_MISC_FUNC_NUM,
                                                            ONLY_REGISTER_OFFSET(UART_ENABLE_UBOX_MISC_REG)));
  UboxUartEnable.Bits.enableuartport0 = EnableSCUart;
  UboxUartEnable.Bits.enableuartport1 = EnableSCUart;

  IioPciExpressWrite32 ( IioGlobalData, PCI_PCIE_ADDR (UncoreBusNo,
                                       UBOX_MISC_DEV_NUM,
                                       UBOX_MISC_FUNC_NUM,
                                       ONLY_REGISTER_OFFSET(UART_ENABLE_UBOX_MISC_REG)),UboxUartEnable.Data);
}

VOID
IioSouthComplexPeiInit( 
  IIO_GLOBALS *IioGlobalData
)
{
  SC_RESET_STS_PCU_FUN1_STRUCT     SCResetSts;
  UINT8    UncoreBusNo;
  BOOLEAN  DisableSCGbE = FALSE;
  BOOLEAN  DisableSCCbDma = FALSE;
  BOOLEAN  HideIOSF0 = FALSE;
  BOOLEAN  HideIOSF1 = FALSE;
  SC_SETUP_INPUT     ScSetupData;

  IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO, "DEBUG::::  IioSouthComplexPeiInit()\n");
  //
  // Get the Platform configuration info
  //
  GetPlatformSouthComplexPolicy( IioGlobalData, &ScSetupData);
  //
  // get the uncore bus no
  //
  UncoreBusNo = GetUncoreBusNo( IioGlobalData, IIO_BUS_NUM);
  //
  // Disable SC UART based on platformInfo Hob 
  //
  if (ScSetupData.DisableSCUart){
    IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO, "DEBUG:::: Disable SC UART \n");
    IioSouthComplexUartInit(IioGlobalData, FALSE);
  }
  //
  // Check the SC HW reset status
  //
  SCResetSts.Data = IioPciExpressRead32 ( IioGlobalData, PCI_PCIE_ADDR (UncoreBusNo,
                                                        PCU1_DEV_NUM,
                                                        PCU1_FUNC_NUM,
                                                        ONLY_REGISTER_OFFSET(SC_RESET_STS_PCU_FUN1_REG)));
  if(SCResetSts.Bits.sc_bypass){	//SC bypass defeature mode set (4985895)
    IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO,"SC bypass defeature mode set!!!\n");
    DisableSCGbE = DisableSCCbDma = TRUE;
    HideIOSF0 = HideIOSF1 = TRUE;
    //
    // 4987999 : [BDX-DE-U0-PO] Dev 30 F7 is not hidden in BIOS when SC is in bypass mode
    //
    IioPeiHide( IioGlobalData, UncoreBusNo, PCU7_DEV_NUM, PCU7_FUNC_NUM);
  }
  else if(SCResetSts.Bits.gbe0_init_failed){		//SC GbE init failed (4167489)
    IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO, "SC GbE init failed!!!\n");
    DisableSCGbE = TRUE;
    HideIOSF1 = TRUE;
  }
  else if(SCResetSts.Bits.sc_init_failed){		//SC init failed (4167489)
    IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO, "SC init failed!!!\n");
    DisableSCGbE = DisableSCCbDma = TRUE;
    HideIOSF0 = HideIOSF1 = TRUE;
  }
  if(DisableSCCbDma == FALSE){
    if(ScSetupData.Disable_SC_Cb3Dma){		//user forcing complete SC CB DMA disabled (4168144)
      IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO,"user forcing complete SC CB DMA disabled!!!\n");
      DisableSCCbDma = TRUE;
    }
  }
  if(DisableSCGbE == FALSE){
    if(ScSetupData.Disable_SC_GbE){		//user forcing complete SC GbE device disabled (4168145)
      IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO,"user forcing complete SC GbE device disabled!!!\n");
      DisableSCGbE = TRUE;
    }
  }

  //
  // Init the IIO IOSF bridge (BDF 0,2,0/2) connected to the SC
  //
  IioPeiInitIOSF( IioGlobalData, IIO_BUS_NUM, DisableSCCbDma, DisableSCGbE);

  //
  // Hide the SC CBDMA/GbE IOSF bridge
  //
  if(HideIOSF0 == TRUE){	//hide the IOSF 0,2,0 (SC CBDMA port)
    IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO,"Hiding the IOSF 0,2,0 (SC CBDMA port)!\n");
    IioPeiHide( IioGlobalData, UncoreBusNo, PCIE_PORT_2_DEV, PCIE_PORT_2A_FUNC);
    IioPeiHide( IioGlobalData, UncoreBusNo, PCIE_PORT_2_DEV, PCIE_PORT_2B_FUNC);
  }
  if(HideIOSF1 == TRUE){	//hide the IOSF 0,2,2 (SC GbE port)
    IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO,"Hiding the IOSF 0,2,2 (SC GbE port)!\n");
    IioPeiHide( IioGlobalData, UncoreBusNo, PCIE_PORT_2_DEV, PCIE_PORT_2C_FUNC);
    IioPeiHide( IioGlobalData, UncoreBusNo, PCIE_PORT_2_DEV, PCIE_PORT_2D_FUNC);
  }
  
  //
  // Update GBE status in Platform Info Hob 
  //
  if(DisableSCGbE == FALSE && HideIOSF1 == FALSE) {
    ScSetupData.SC_Gbe_Status = 1;   // GBE up
  } else {
    ScSetupData.SC_Gbe_Status = 0;   // GBE down
  }
  
}

#endif // DE_SKU


IIO_STATUS
IioDmiTcVcSetup (
   IIO_GLOBALS                 *IioGlobalData
  )
{
  UINT64            DmiBar;

  //
  //  Apply any DMI specific Si workarounds first
  //
  InitDmiSpecificSiWorkaorunds(IioGlobalData);
  //
  // For DMI 2.0
  // First Program PCH DMI to gen2
  //
  OemDmiGen2Prog (IioGlobalData);

#ifdef DE_SKU
  //
  // s4988153 : CLONE from broadwell_server: Clone from BDX Sighting:U0 PO: PFE: BIOS enabling GBE physical 
  // functions making system hangs - access to pmc_cfg_cr_svid
  //
  ProcessSC_Gbe_PF(IioGlobalData);
#endif

  if (IioGlobalData->IioVData.PciResourceMem32Limit[0] <= IioGlobalData->IioVData.PciResourceMem32Base[0]) {
    return IIO_OUT_OF_RESOURCES;
  }
  
  DmiBar = (UINT64) IioGlobalData->IioVData.PciResourceMem32Limit[0] & (~0xFFF);
  
  
  //
  // Update DmiTcVcMapping based on Policy
  //
  IioGetDmiVcTcSettings(IioGlobalData);
  OemPchDmiTcVcMapInit(IioGlobalData);
  IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "DEBUG:::: DmiTypeVc1 = %x ; DmiTypeVcp = %x ; DmiTypeVcm = %x\n", IioGlobalData->IioVData.DmiVc[DmiTypeVc1], IioGlobalData->IioVData.DmiVc[DmiTypeVcp], IioGlobalData->IioVData.DmiVc[DmiTypeVcm]);
  if (!(IioGlobalData->IioVData.DmiVc[DmiTypeVc1] + IioGlobalData->IioVData.DmiVc[DmiTypeVcp] + IioGlobalData->IioVData.DmiVc[DmiTypeVcm])) 
    return IIO_SUCCESS;

  //
  // Temporarily enable DmiRcBar at a valid address
  //
  IioPciExpressWrite32 ( IioGlobalData, PCI_PCIE_ADDR(DMI_BUS_NUM, DMI_DEV_NUM, DMI_FUNC_NUM, R_PCIE_PORT_DMIRCBAR), (UINT32)DmiBar+ BIT0);

  //
  // Call PchDmiTcVcProg
  //
  IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "DEBUG::::  Calling DmiTcVcProg\n");
  OemDmiTcVcProg(IioGlobalData);
  
  //
  // Program NB TC/VC mapping
  //
  IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "DEBUG::::  SetDmiTcVcMapping\n");
  IioSetDmiTcVcMapping (IioGlobalData, DmiBar);
  //
  // Poll NB negotiation completion
  //
  IioPollDmiVcStatus (IioGlobalData, DmiBar);

  //
  // Call PchDmiTcVcProgPoll
  //
  OemDmiTcVcEnablePoll(IioGlobalData);
  //
  // Turn off the DmiRcBar so it can be appropriately allocated later in BIOS
  //
  IioPciExpressWrite32 ( IioGlobalData, PCI_PCIE_ADDR(DMI_BUS_NUM, DMI_DEV_NUM, DMI_FUNC_NUM, R_PCIE_PORT_DMIRCBAR), 0);
  IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "DEBUG::::  Done\n");

  return IIO_SUCCESS;

}

VOID
DmiLinkTrain (
   IIO_GLOBALS *IioGlobalData,
   UINT64 DmiBar
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
  IioMmioOr8 (IioGlobalData, (UINT32)(DmiBar + R_IIO_RCRB_DMILCTRL), BIT5);

  //
  // Wait for link training complete
  //
  while ((IioMmioRead16 (IioGlobalData, (UINT32)(DmiBar + R_IIO_RCRB_DMILSTS)) & BIT11) != 0);
}


VOID 
IioIsocConfiguration(
  IIO_GLOBALS                         *IioGlobalData,
  UINT8                               Iio
)
{
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

  BusNumber = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];
  //
  // The following is common for Processors
  // 3614823, 3876167: isoc_enabled bit in CSIPISOCRES needs to be set
  //                   when isoc is enabled
  //  [Non-Si, permanent programming for all steppings]
  //
  // 4167166: [BSX_B0_PO] CLONE from HSX: OVL Error: cache allocation exceeds max cache!
  //
  CsiPiSocres.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusNumber, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(CSIPISOCRES_IIO_DFX_GLOBAL_REG)));
  IrpEgCredits.Data = IioPciExpressRead32( IioGlobalData,  PCI_PCIE_ADDR(BusNumber, CORE05_DEV_NUM, CORE05_FUNC_NUM, ONLY_REGISTER_OFFSET(IRPEGCREDITS_IIO_VTD_REG)));

  IrpMiscDfx1 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusNumber, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IRP_MISC_DFX1));

  if(IioGlobalData->IioVData.CpuType == CPU_IVT){
      IrpMiscDfx1 &= ~(BIT25 | BIT24);   //pf_flush_timer = 0
  }
        
  if ((IioGlobalData->IioVData.IsocEnable) || (IioGlobalData->IioVData.MeSegEn)){
     //
     //4167774 : HSX Clone: workaround for all known isoc issues
     //4186011: PROMOTE from haswell_server: Isoc Flexcon Failure when IsocEn is Enable
     //
     if ((IioGlobalData->IioVData.CpuType == CPU_IVT) ||
         ((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping == B0_REV_HSX))){
          CsiPiSocres.Bits.isoc_enabled = 0x01;
          // 4168320 merge_vc0_cnt should be set when the following conditions are on: 
          //   * If IsocEnabled and any of the VC is on
          //   * If MeSegEnabled and VCm is enabled
          if ((((IioGlobalData->IioVData.DmiVc[DmiTypeVc1]) || (IioGlobalData->IioVData.DmiVc[DmiTypeVcp]) || (IioGlobalData->IioVData.DmiVc[DmiTypeVcm])) &&  
               (IioGlobalData->IioVData.IsocEnable)) ||
               (IioGlobalData->IioVData.DmiVc[DmiTypeVcm] &&  IioGlobalData->IioVData.MeSegEn))
                 CsiPiSocres.Bits.merge_vc0_cnt = 0x01;
          }
         if ((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping  <= B0_REV_HSX)){
           IrpEgCredits.Bits.ad_shared_cdt_threshold = 0x1;
         }
     } else {
          if ((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping  <= B0_REV_HSX)){
            IrpEgCredits.Bits.ad_shared_cdt_threshold = 0xb;
          }
     }

     if (((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping  >= C0_REV_HSX)) ||
          (IioGlobalData->IioVData.CpuType == CPU_BDX))  {
        if (Iio == 0) {
           //
           // 4983629 Set merge_vc0_cnt unconditionally for C0 stepping. 
           //     
           CsiPiSocres.Bits.merge_vc0_cnt = 0x01;
           //
           // 4986112: ESCALATE from hexaii_hsx_mock:C0 BIOS issues for isoc/ME
           //
           Cipctrl.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusNumber, CORE05_DEV_NUM, CORE05_FUNC_NUM, ONLY_REGISTER_OFFSET(CIPCTRL_IIO_VTD_REG)));

           IsocVc_Tbl = IsocVc_Table_HsxC0;
           for (j=0; j < (sizeof(IsocVc_Table_HsxC0)/sizeof(ISOC_VC_TABLE_STRUCT)); j++) {
             if(IioGlobalData->IioVData.IsocEnable ==  IsocVc_Tbl[j].Isoc){
               if(IioGlobalData->IioVData.MeSegEn == IsocVc_Tbl[j].Meseg){
                 if(IioGlobalData->IioVData.DmiVc[DmiTypeVc1] == IsocVc_Tbl[j].Vc1){
                   if(IioGlobalData->IioVData.DmiVc[DmiTypeVcm] == IsocVc_Tbl[j].Vcm){
                     if(IioGlobalData->IioVData.DmiVc[DmiTypeVcp] == IsocVc_Tbl[j].Vcp){ 
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
           IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, CORE05_DEV_NUM, CORE05_FUNC_NUM, ONLY_REGISTER_OFFSET(CIPCTRL_IIO_VTD_REG)), Cipctrl.Data);
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
         CsiPCtrl0.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusNumber, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(CSIPCTRL_0_IIO_DFX_GLOBAL_REG)));
         CsiPCtrl0.Bits.vcp_priority = 3; 
         IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(CSIPCTRL_0_IIO_DFX_GLOBAL_REG)),CsiPCtrl0.Data);
         IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO,"CsiPCtrl0 = %x\n",CsiPCtrl0.Data);
         //
         // 4986166: ESCALATE from hexaii_hsx_mock:Updated C0 register settings for isoc
         //
         IrpMiscDfx1 &= ~BIT22; //irp_misc_dfx1.use_isoc_ovf_q  = 0   
         //
         // 4986258: ESCALATE from hexaii_hsx_mock:C0 BIOS/Isoc issues with 18d15
         //
         IrpEgCredits.Bits.ad_shared_cdt_threshold = 0xb;
     }

     IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(CSIPISOCRES_IIO_DFX_GLOBAL_REG)), CsiPiSocres.Data);
     IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, CORE05_DEV_NUM, CORE05_FUNC_NUM, ONLY_REGISTER_OFFSET(IRPEGCREDITS_IIO_VTD_REG)), IrpEgCredits.Data);
     IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IRP_MISC_DFX1), IrpMiscDfx1);

     //
     //4167774 : HSX Clone: workaround for all known isoc issues for HSX A0 and B0 only
     //
     if ((IioGlobalData->IioVData.CpuType == CPU_HSX)) { 
        if (IioGlobalData->IioVData.CpuStepping < B0_REV_HSX) {
          VTdPrivC0.Data =  IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusNumber, IIO_DFX_VTD_DEV, IIO_DFX_VTD_FUNC, ONLY_REGISTER_OFFSET(VTDPRIVC0_IIO_DFX_VTD_REG)));
          if (IioGlobalData->SetupData.VTdSupport)
            VTdPrivC0.Bits.dvt = 0;              
          else
            VTdPrivC0.Bits.dvt = 0x01;
          IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, IIO_DFX_VTD_DEV, IIO_DFX_VTD_FUNC, ONLY_REGISTER_OFFSET(VTDPRIVC0_IIO_DFX_VTD_REG)), VTdPrivC0.Data);
          
          SwDbgCtl0.Data =  IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusNumber, 6, 7, ONLY_REGISTER_OFFSET(SWDBGCTL0_IIO_DFX_GLOBAL_REG)));
          if ( (IioGlobalData->SetupData.WA_4167453 || IioGlobalData->IioVData.DmiVc[DmiTypeVc1] ) && ( IioGlobalData->SetupData.VTdSupport) )
            SwDbgCtl0.Bits.enable_irp_set = 0;
          else 
            SwDbgCtl0.Bits.enable_irp_set = 1;
          IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, 6, 7, ONLY_REGISTER_OFFSET(SWDBGCTL0_IIO_DFX_GLOBAL_REG)), SwDbgCtl0.Data); 
        }
     }
}


