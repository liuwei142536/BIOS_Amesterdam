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

  IioWorkAround.c

Abstract:

  Houses all code related to IIO workarounds 

Revision History:

--*/

#include "IioEarlyInitialize.h"

#define IIO_BIFURCATE_x4x4x4x4  0
#define IIO_BIFURCATE_x4x4xxx8  1
#define IIO_BIFURCATE_xxx8x4x4  2
#define IIO_BIFURCATE_xxx8xxx8  3
#define IIO_BIFURCATE_xxxxxx16  4

UINT64 CdrPpmBifurcationMap[5][4] = { {0x0000000000000081, 0x0000000000204000, 0x0000008100000000, 0x0020400000000000},
                                      {0x0000000000204081, 0x0000000000000000, 0x0000008100000000, 0x0020400000000000},
                                      {0x0000000000000081, 0x0000000000204000, 0x0020408100000000, 0x0000000000000000},
                                      {0x0000000000204081, 0x0000000000000000, 0x0020408100000000, 0x0000000000000000},
                                      {0x0020408100204081, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000} };

UINT8 LbcStartLane[] = {0, 0, 4, 0, 4, 8, 12, 0, 4, 8, 12};                   // start lane per port in Load bus control

/*++
=========================================================================================
===================== Subroutines should be defined starting here    ====================
=========================================================================================
--*/

VOID
B2P_Pcu_Misc_Config_WA(
    IIO_GLOBALS         *IioGlobalData,
    UINT8               Iio,
    UINT8               featurebit,
    BOOLEAN             EnableFeature
)
{
  UINT32  MailBoxCommand, MailboxData;

  MailBoxCommand = MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG;
  MailboxData = IioBios2PcodeMailBoxWrite(IioGlobalData, Iio, MailBoxCommand, 0);
  if (MailboxData == 0) { //if read is successful
    MailboxData = IioReadCpuCsr32(IioGlobalData, Iio, 0, BIOS_MAILBOX_DATA_PCU_FUN1_REG);
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Writing Write_PCU_Misc_Config Bit  %d, Socket %d\n", featurebit, Iio);
    MailBoxCommand = MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG;
    if(EnableFeature == TRUE)
      MailboxData |= (1 << featurebit);
    else
      MailboxData &= ~(1 << featurebit);
    MailboxData = IioBios2PcodeMailBoxWrite(IioGlobalData, Iio, MailBoxCommand, MailboxData);
    if(MailboxData)
      IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_WARN, "ERROR! [IIO%d:Errcode:0x%x] Writing Write_PCU_Misc_Config Bit %d\n", Iio, MailboxData, featurebit);
  }
  else{
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_WARN, "ERROR! [IIO%d:Errcode:0x%x] - unable to enable/disable PCU_Misc_Config Bit %d\n", Iio, MailboxData, featurebit);
  }
}

/*
 * Routine name: IioGlobalPermanentWorkarounds()
 * Description: This routine contains permanent workarounds involving
 * the IIO DFX global settings
 */
VOID
IioGlobalPermanentWorkarounds (
    IIO_GLOBALS              *IioGlobalData,
    UINT8                    Iio
)
{
  UINT8   BusBase;
  UINT32  Data32;

  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];
  //
  // ivt_eco: s4419547:  [Legacy] ASC response selection is incorrectly connected
  //                      in TSWPXPTHRC2 & TSWPXPTHRC1
  //  [cloned to s4029977]
  if (IioGlobalData->IioVData.CpuType == CPU_IVT) {
  Data32 =  IioPciExpressRead32( IioGlobalData,  PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
                ONLY_REGISTER_OFFSET(TSWPXPTHRC1_IIO_DFX_GLOBAL_REG))) ;
    Data32 &= ~(BIT19 | BIT18 | BIT17 | BIT16);
  if(!IioGlobalData->SetupData.ASCSelect)
    Data32 |= BIT16;                                //setting value 1 for TSWTHR0
  IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
        ONLY_REGISTER_OFFSET(TSWPXPTHRC1_IIO_DFX_GLOBAL_REG)), Data32);
  }

  //
  // ivt_eco: s4540394: A write prefetch not dropped after a primary_pf_flush
  //  [cloned to s4029974]
  if (IioGlobalData->IioVData.CpuType == CPU_IVT) {
    Data32 =  IioPciExpressRead8 ( IioGlobalData, PCI_PCIE_ADDR(BusBase, CORE05_DEV_NUM, CORE05_FUNC_NUM, \
        ONLY_REGISTER_OFFSET(IRPSPAREREGS_IIO_VTD_REG)));
    Data32 |= BIT2;
    //
    // ivt_eco: s4676958: IVT EX : CatError with Ubox Master Lock Time Out seen
    //                    while running Rocket local p2p content
    //  [cloned to s4030904]
    Data32 |= BIT3;
    Data32 &= 0x7C;   //clear bit7.....spare_csr = 0x7C

    IioPciExpressWrite8 (IioGlobalData, PCI_PCIE_ADDR(BusBase,  CORE05_DEV_NUM, CORE05_FUNC_NUM, \
        ONLY_REGISTER_OFFSET(IRPSPAREREGS_IIO_VTD_REG)), (UINT8)(Data32 & 0xFF));
  }

  //
  // ivt_eco: s4620429:: [Legacy] A622901 does not account for link down scenario
  //                      resulting in incorrect firing of the assertion
  //  [cloned to s4030418] will not be fixed perm. WA
  if (IioGlobalData->IioVData.CpuType == CPU_IVT) {
    Data32 =  IioPciExpressRead8 ( IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
                ONLY_REGISTER_OFFSET(TSWCTL0_IIO_DFX_GLOBAL_REG)));
    Data32 |= BIT23;    //disrstptr=1
    IioPciExpressWrite8 (IioGlobalData, PCI_PCIE_ADDR(BusBase,  PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
      ONLY_REGISTER_OFFSET(TSWCTL0_IIO_DFX_GLOBAL_REG)), (UINT8)(Data32 & 0xFF));

  //
  // ivt_eco: s4620569: UniPhy PCIe: Continuous Tx DCC Calibration Adversely
  //                    Affects Link Performance
  //  [cloned to s4030987]
    Data32 =  IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
              ONLY_REGISTER_OFFSET(CC_MISC_CTL_1_IIO_DFX_GLOBAL_REG)));
    Data32 &= ~(BIT12 | BIT11 | BIT10);
    Data32 |= (BIT12 | BIT10);    //txdcc_gainsel_b = 5
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
      ONLY_REGISTER_OFFSET(CC_MISC_CTL_1_IIO_DFX_GLOBAL_REG)), Data32);
  }  //CPU_IVT
}

/*
 * Routine name: IioOtherMiecellaneousWorkarounds
 * Description: This routine may hold other miscellaneous workarounds which may
 * be temporary or require only for testing purpose
 */
VOID
IioOtherMiecellaneousWorkarounds (
    IIO_GLOBALS              *IioGlobalData,
    UINT8                    Iio
)
{
  UINT8   BusBase;
  UINT32  Data32;
  UINT32  RegLocation;
  CSIPOOLDFX0_IIO_DFX_GLOBAL_STRUCT        CsiPoolDfx0;
  CSIPOOLDFX1_IIO_DFX_GLOBAL_STRUCT        CsiPoolDfx1;
  IIOERRCTL_IIO_RAS_HSX_BDX_STRUCT         IioErrCtlRas;
  IRPEGCREDITS_IIO_VTD_STRUCT              IrpEgCredits;
  CC_MISC_CTL_2_IIO_DFX_GLOBAL_BDX_STRUCT  CcMiscCtl2;
  CC_SPARE2_IIO_DFX_GLOBAL_STRUCT          CcSpare2;

  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];

  //
  // Following are the ECOs which may get change later as stepping specific
  //

  //
  // ivt_eco: s4253362: outbound switch data parity errror is detected for dmi
  //                    when disable parity error is set to zero
  //  [cloned to s4030164] - only for A0
  if (IioGlobalData->IioVData.CpuType == CPU_IVT) {
  Data32 =  IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
      ONLY_REGISTER_OFFSET(TSWCTL_E_IIO_DFX_GLOBAL_REG))) ;
  Data32 |= BIT3;   //csr_dfx_disable_parity_fix
  IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
      ONLY_REGISTER_OFFSET(TSWCTL_E_IIO_DFX_GLOBAL_REG)), Data32);
  }

  //
  // ivt_eco s4536544:  [IRP] : chip0.iio0.iirp.iirpc0.irpsqg.unexp_f_pf_numbering firing
  //  [cloned to s4030344] - recommendation for IVT A0 is to write large threshold
  //                          value to tickle timer, default value is 0
  if (IioGlobalData->IioVData.CpuType == CPU_IVT) {
    RegLocation = ONLY_REGISTER_OFFSET(IRP_MISC_DFX4_IIO_VTD_REG);
    Data32 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, CORE05_DEV_NUM, CORE05_FUNC_NUM, RegLocation));
    Data32 |= 0xFFFF;     //tkl_timer_threshold (bit 15:0)
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, CORE05_DEV_NUM, CORE05_FUNC_NUM, RegLocation), Data32);
    RegLocation = ONLY_REGISTER_OFFSET(IRP_MISC_DFX5_IIO_VTD_REG);
    Data32 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, CORE05_DEV_NUM, CORE05_FUNC_NUM, RegLocation));
    Data32 |= 0xFFFF;     //tkl_timer_threshold (bit 15:0)
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, CORE05_DEV_NUM, CORE05_FUNC_NUM, RegLocation), Data32);
  }

      //
      // ivt_eco: s4677079: Running Rocket base causes TOR Timeouts due to Write Cache occupancy related issues in IIO
      //  [cloned to s4031255]
  if ((IioGlobalData->IioVData.CpuType == CPU_IVT) && (IioGlobalData->IioVData.CpuStepping < B3_REV_IVT)) {

      CsiPoolDfx0.Data =  IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
                    ONLY_REGISTER_OFFSET(CSIPOOLDFX0_IIO_DFX_GLOBAL_REG)));
      CsiPoolDfx0.Bits.max_cache_p = 0x14; 
      CsiPoolDfx0.Bits.max_cache_np_pf = 0x10; 
      CsiPoolDfx0.Bits.max_cache_np = 0x14; 
      CsiPoolDfx0.Bits.vc0_max_cache = 0x10; 
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase,  PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
          ONLY_REGISTER_OFFSET(CSIPOOLDFX0_IIO_DFX_GLOBAL_REG)), CsiPoolDfx0.Data);
  
      CsiPoolDfx1.Data =  IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
                    ONLY_REGISTER_OFFSET(CSIPOOLDFX1_IIO_DFX_GLOBAL_REG)));
      CsiPoolDfx1.Bits.max_cache = 0x18; 
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase,  PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
          ONLY_REGISTER_OFFSET(CSIPOOLDFX1_IIO_DFX_GLOBAL_REG)), CsiPoolDfx1.Data);
  
      Data32 =  IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IRP_MISC_DFX2));
      Data32 &= ~(BIT16+BIT17+BIT18+BIT19+BIT20+BIT21+BIT22+BIT23+BIT24+BIT25+BIT26+BIT27+BIT28);
      Data32 |= (0x7 << 16);
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase,  CORE05_DEV_NUM, CORE05_FUNC_NUM, \
        R_IRP_MISC_DFX2), Data32);
  
      Data32 =  IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, CORE05_DEV_NUM, CORE05_FUNC_NUM, \
          ONLY_REGISTER_OFFSET(IRP_MISC_DFX3_IIO_VTD_REG)));
      Data32 &= ~(BIT16+BIT17+BIT18+BIT19+BIT20+BIT21+BIT22+BIT23+BIT24+BIT25+BIT26+BIT27+BIT28);
      Data32 |= (0x7 << 16);
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase,  CORE05_DEV_NUM, CORE05_FUNC_NUM, \
          ONLY_REGISTER_OFFSET(IRP_MISC_DFX3_IIO_VTD_REG)), Data32);
  }
  //
  // ivt_eco: s4622037: The CSR_CACHE_NEW_JKT_MODE may causing hanging in Switch
  //  [cloned to s4031969] - WA only for C0
  if ((IioGlobalData->IioVData.CpuType == CPU_IVT) && (IioGlobalData->IioVData.CpuStepping == C0_REV_IVT)) {
    CsiPoolDfx0.Data =  IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
                          ONLY_REGISTER_OFFSET(CSIPOOLDFX0_IIO_DFX_GLOBAL_REG)));
    CsiPoolDfx0.Bits.max_cache_p = 0x64;
    CsiPoolDfx0.Bits.max_cache_np = 0x64;
    IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusBase,  PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
        ONLY_REGISTER_OFFSET(CSIPOOLDFX0_IIO_DFX_GLOBAL_REG)), CsiPoolDfx0.Data);
  }
  //
  // 4986612 HSX Clone: PCIE: RocketBase CATERR with MCTP
  // WA for C0 Set max_cache and vc0_max_cache to 0x40. 
  if (((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping >= C0_REV_HSX)) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
    CsiPoolDfx0.Data =  IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
                          ONLY_REGISTER_OFFSET(CSIPOOLDFX0_IIO_DFX_GLOBAL_REG)));
    CsiPoolDfx0.Bits.vc0_max_cache=0x40; 
    IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusBase,  PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
        ONLY_REGISTER_OFFSET(CSIPOOLDFX0_IIO_DFX_GLOBAL_REG)), CsiPoolDfx0.Data);
    CsiPoolDfx1.Data =  IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
                    ONLY_REGISTER_OFFSET(CSIPOOLDFX1_IIO_DFX_GLOBAL_REG)));
    CsiPoolDfx1.Bits.max_cache = 0x40; 
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase,  PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
          ONLY_REGISTER_OFFSET(CSIPOOLDFX1_IIO_DFX_GLOBAL_REG)), CsiPoolDfx1.Data);
   
  }
  //
  // bdx_sighting: 5097794:  [Isoch] Lock + PRd to MMIO TOR Timeout with cross product
  //                            of VTd enabled isoch/non-isoch traffic and CPU lock
  //  [cloned to 5002514] - permanent WA for BDX-EP only when the ISOC is enabled
  if( (IioGlobalData->IioVData.CpuFamily == CPU_FAMILY_BDX) && (IioGlobalData->IioVData.IsocEnable) ){
    CsiPoolDfx0.Data =  IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
                                                            ONLY_REGISTER_OFFSET(CSIPOOLDFX0_IIO_DFX_GLOBAL_REG)));
    CsiPoolDfx0.Bits.vc0_max_cache=0x38;
    IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusBase,  PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, \
                                        ONLY_REGISTER_OFFSET(CSIPOOLDFX0_IIO_DFX_GLOBAL_REG)), CsiPoolDfx0.Data);
  }

  if ((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping < B0_REV_HSX)) {
    //
    // HSD 4166048: CLONE from HSX: rpegr_ak_egress.ak_agent_queue.R_R2PCIE_EgressQueue_Overflow 
    //
    RegLocation = ONLY_REGISTER_OFFSET(IRPEGCREDITS_IIO_VTD_REG);
    IrpEgCredits.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, CORE05_DEV_NUM, CORE05_FUNC_NUM, RegLocation));
    IrpEgCredits.Bits.ad0_ak0_plus_one_shared_cdt = 0;
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, CORE05_DEV_NUM, CORE05_FUNC_NUM, RegLocation), IrpEgCredits.Data);
  }
    //
    // hsx_eco: 249198,249465 : inbound packets do not calculate proper header parity in certain error cases
    // [cloned to s4032188]
    //
    // HSD 4166424: [HSX_A0_PO] CLONE from HSX: Switch parity calculation zeroes TH bit in header so PCIDCAHint is not sent
    //
    RegLocation = ONLY_REGISTER_OFFSET(IIOERRCTL_IIO_RAS_REG);
    IioErrCtlRas.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, CORE05_DEV_NUM, CORE05_FUNC2_NUM, RegLocation));
    if (((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping >= B0_REV_HSX)) || (IioGlobalData->IioVData.CpuType == CPU_BDX)){
      IioErrCtlRas.Bits.c8_ib_header_parity_detect = 1;
    } else {
      IioErrCtlRas.Bits.c8_ib_header_parity_detect = 0;
    }
    //
    // 4987823: Grantley RAS code failed to enable c6/c5/c4 bits of IIOERRCTL_IIO_RAS_REG
    //
    if (IioGlobalData->IioVData.IioErrorEn) {
      IioErrCtlRas.Bits.c4_inbound_ler_disable = 0;
      IioErrCtlRas.Bits.c4_outbound_ler_disable = 0;     
      IioErrCtlRas.Bits.c6 = 1;
      IioErrCtlRas.Bits.c5 = 1;
      IioErrCtlRas.Bits.c4 = 1;
    } 

    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, CORE05_DEV_NUM, CORE05_FUNC2_NUM, RegLocation), IioErrCtlRas.Data);
  
  //
  // 4166740: BIOS not writing to PCU mailbox has pcode disable PCIE hot plug
  // 4988223: Some HSW CPU would CATERR during warm reset when PCIE hotplug and COD are enabled (Read before writing the register 
  // to preserve other bit values)
  //
  if ((IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
    if (IioGlobalData->SetupData.PcieHotPlugEnable) {
      B2P_Pcu_Misc_Config_WA( IioGlobalData, Iio, 27, TRUE);
    }
  }

  //
  // hsx_eco s256827: iio tlb forward progress mechanism is broken in presence of locks
  //  [cloned to s4032241] -  (from EP:- read tor timeout when under lock with oversubscribed tlb)
  if ((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping <= B0_REV_HSX)){
    Data32 =  IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(SWDBGCTL0_IIO_DFX_GLOBAL_REG))) ;
    Data32 &= ~BIT24;   //enable_extended_vtstf
    IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(SWDBGCTL0_IIO_DFX_GLOBAL_REG)), Data32);
  }

  if (IioGlobalData->IioVData.CpuType == CPU_BDX) {
    //
    // bdx_eco: s280816 : CLONE from broadwell_server: TX DCC Thermal code is not converging when thermal sample_votes >128 is programmed
    // [cloned to s4986722 , s4987760 , s5001776 ] - Permanent for BDX all stepping
    //
    CcMiscCtl2.Data =  IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(CC_MISC_CTL_2_IIO_DFX_GLOBAL_REG)));
    CcMiscCtl2.Bits.txdcc_gain_sel = 5;
    IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(CC_MISC_CTL_2_IIO_DFX_GLOBAL_REG)), CcMiscCtl2.Data);
    //
    // bdx_eco: s270703 : CLONE from broadwell_server: IVT Bug: Uniphy DCC FSM's accumulator overflows for  thermometer calibration's gain_sel setting of 4 and less
    // [cloned to s4986723 , s4987760 , s5001774 ] - Permanent for BDX all stepping
    //
    CcSpare2.Data = IioPciExpressRead32(IioGlobalData,PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(CC_SPARE2_IIO_DFX_GLOBAL_REG)));
    CcSpare2.Bits.cc_spare2 |= (BIT2 | BIT0);
    IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(CC_SPARE2_IIO_DFX_GLOBAL_REG)), CcSpare2.Data);
  } // CPU_BDX
}

//HSD 3613071 ClearXPUncErrStsBit6
VOID 
ClearXPUncErrStsBit6(
    IIO_GLOBALS              *IioGlobalData,
    UINT8                Iio
)
{
    UINT8   BusBase;
    UINT8   Device;
    UINT8   Function;
    UINT8   j;
    UINT32  Data32;

    BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];

    //P0 DMI or PCIE0
    Data32 = 0;
    //this bit is R/W1CS = write 1 to clear 
    Data32 |= RECEIVED_PCIE_COMPELTION_WITH_UR_STATUS;
    for(j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
      Device = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
      Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, Device, Function, R_XPUNCERRSTS), Data32);
    }
    return;
}


//HSD 3613306: set dfr int remap
//Note: This is permanent WA for Processors
VOID
SetDfrIntRemap  (
    IIO_GLOBALS                 *IioGlobalData,
    UINT8                       BusBase
    )
{
  UINT32  Data32;
  UINT8   Device  = PCIE_PORT_GLOBAL_DFX_DEV;
  UINT8   Func    = PCIE_PORT_GLOBAL_DFX_FUNC;

  //
  // HSD 3613306 SetDfr Int Remap needs to be set, permanent WA
  //    IVT version below
  //
  if (IioGlobalData->IioVData.CpuType == CPU_IVT) {
    Data32 =  IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, Device, Func, ONLY_REGISTER_OFFSET(DBG_MUX1_IIO_DFX_GLOBAL_REG))) ;
    if( !(Data32 & BIT26) )//this bit is RW-Once ,so check 1st to see if hasn't been written already
    {
      Data32 |= BIT26;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, Device, Func, ONLY_REGISTER_OFFSET(DBG_MUX1_IIO_DFX_GLOBAL_REG)), Data32);
    }
  }
  //
  // HSD 3613306 SetDfr Int Remap needs to be set, permanent WA
  //    HSX version below
  // HSD 4166412: DBG_MUX1_IIO_DFX_GLOBAL_REG Does Not Exist in HSX
  //              The dfr_en_intremap bit was moved from offset 450h to offset 3c4h.
  //              The bit position was moved from bit 26 to bit 27.
  //
  if ((IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
    Data32 =  IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, Device, Func, ONLY_REGISTER_OFFSET(SWDBGCTL0_IIO_DFX_GLOBAL_REG))) ;
    Data32 |= BIT27;
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, Device, Func, ONLY_REGISTER_OFFSET(SWDBGCTL0_IIO_DFX_GLOBAL_REG)), Data32);
  }
}


/*
 * Routine name: IioMiscellaneousCommonWorkarounds()
 * Description: Routine contains common cross processor workarounds
 */
VOID
IioMiscellaneousCommonWorkarounds (
    IIO_GLOBALS                 *IioGlobalData,
    UINT8                   Iio
)
{
    UINT32  Data32;
    UINT8   BusBase;
    UINT8   BusUncore;
    UINT16  Data16;
    FWDPROGRESS1_IIO_DFX_GLOBAL_STRUCT      FwdProgress1;
    FWDPROGRESS0_IIO_DFX_GLOBAL_STRUCT      FwdProgress0;
    UINT32                                  MiscDfx0;
    UINT32                                  RegLocation;
    TSWCTL1_IIO_DFX_GLOBAL_STRUCT           TswCtl1;	
    TSWCTL0_IIO_DFX_GLOBAL_HSX_BDX_STRUCT   TswCtl0; 
    ERRPINDAT_IIO_RAS_STRUCT                iioErrPinDatReg;

    BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];
		BusUncore = IioGlobalData->IioVData.SocketUncoreBusNumber[Iio];
    //
    // 4986248: CLONE from HSX: Malicious SW could negatively impact servers availability
    // due to incorrect default value of IIO_CR_ERRPINDAT_0_5_2_CFG
    //
    if ((IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
      RegLocation = ONLY_REGISTER_OFFSET(ERRPINDAT_IIO_RAS_REG);
      iioErrPinDatReg.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, CORE05_DEV_NUM, CORE05_FUNC2_NUM, RegLocation));
      iioErrPinDatReg.Bits.pin0 = 1;
      iioErrPinDatReg.Bits.pin1 = 1;
      iioErrPinDatReg.Bits.pin2 = 1;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, CORE05_DEV_NUM, CORE05_FUNC2_NUM, RegLocation), iioErrPinDatReg.Data);
      IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR,"Update iioErrPinDatReg = %x\n", iioErrPinDatReg.Data);
    }
 
  	// The following is common for IVT, HSX
    // s3612991: Workaround for IIO is supposed to handle unimplemented functions.
    //            Chicken bit must be set for all steppings.
    Data16 = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, R_DFX_HVM));

    //
    // 4168945: PCIE:  L0 20-lane reduced part hides Device 1 function 0
    // 4986974: HEDT (Lituya Bay) PCIe Bifurcation Set Incorrectly
    // 4986968: HEDT SKU w/PCIe 28-lanes, algorithm to allow IOU2 be detected as x4 does not work for 40-lanes
    //
    // D1:F0 was being hidden on HSW SKUs with reduced PCIe lanes (used only on HEDT),
    //  this W/A takes care of it. But, make sure we implement the W/A *only* on
    //  those parts, otherwise port bifurcation can get mangled.
    //
    // Read CAPID2 fuses in B1:D30:F3 to see if this IIO has reduced PCIe lanes
    // If PCIE_DISXPDEV != 0, lanes are reduced
    //
    Data32 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusUncore, PCIE_PORT_CPU_CAPIDS_DEV, PCIE_PORT_CPU_CAPIDS_FUNC, R_CPU_CAPID2));
    Data32 = (Data32 >> 3) & 0xFFF;
    if (Data32) {
      Data16 &= ~BIT3; //cfg_invalid_dev_mmio_chk_dis = 0x00 to avoid hide D1F0 for HEDT SKU
    } else {
      Data16 |= BIT3; //cfg_invalid_dev_mmio_chk_dis = 0x01;
    }

    IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR( BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, R_DFX_HVM), Data16);

    //
    // 3613791: CLONE from jaketown: IIO hang in lock and vtd invalidtion test
    //  Also covers 3614968
    // 3613242 CLONE from jaketown: System hang running supercollider MultWrite test
    // (This settings is also good for IVT A0)
    if (IioGlobalData->IioVData.CpuType == CPU_IVT) {
      MiscDfx0 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IRP_MISC_DFX0));
      MiscDfx0 &= ~(BIT23+BIT22);
      MiscDfx0 |= (0x1 << 22);    // aging_timer_rollover (512 us)
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IRP_MISC_DFX0), MiscDfx0);

      FwdProgress1.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(FWDPROGRESS1_IIO_DFX_GLOBAL_REG)));
      FwdProgress1.Bits.disbypass = 0x00;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(FWDPROGRESS1_IIO_DFX_GLOBAL_REG)), FwdProgress1.Data);
    }
    //
    // 4032858: HSX BIOS is setting bit 31 (disable_pf_ack_bypass_path) of irp_misc_dfx0 CSR
    // 4168363: HSX BIOS is setting bit 31 (disable_pf_ack_bypass_path) of irp_misc_dfx0 CSR
    // 4032862: HSX Clone : SC: Completion starved at head of switch queue, many MWr to loopback address with ACS bits cleared
    // 4168495: HSX Clone: SC: Completion starved at head of switch queue, many MWr to loopback address with ACS bits cleared
    //
    if ((IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
      MiscDfx0 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IRP_MISC_DFX0));
      MiscDfx0 &= ~BIT31;
      // Clean bits 3:0:  p_pf_snp_rsp_holdoff_cnt
      MiscDfx0 &= ~(BIT3| BIT2| BIT1| BIT0);
      MiscDfx0 |= IioGlobalData->SetupData.SnoopResponseHoldOff; // Set bits 3:0:  p_pf_snp_rsp_holdoff_cnt based on setup option
      IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR( BusBase, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IRP_MISC_DFX0), MiscDfx0);

      FwdProgress0.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(FWDPROGRESS0_IIO_DFX_GLOBAL_REG)));
      FwdProgress0.Bits.inbarbbackoff = 0xf;
      //
      // 4168851: HSX Clone: SC: PCIe Test card (Laguna) Completion TimeOut with HA_OSB_EDR_OPTIMIZATION on P2P
      // 
      FwdProgress0.Bits.arb_timeout_val = 0xff;
      IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(FWDPROGRESS0_IIO_DFX_GLOBAL_REG)), FwdProgress0.Data);
    }

    //
    //  s3613216, 3246475: Workaround for CBDMA quiesce flow is broken (M2M and ROL)
    //  s4167150: CBDMA M2M+ROL traffic causes 3 strike (Ubox Lock FSM hang#3)
    //
    Data32 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(TSWCTL1_IIO_DFX_GLOBAL_REG)));
    Data32 |= BIT1;     //mode_disable_cb_wrhdr_squish = 1
    //
    // ivt_eco s4947426: CLONE from ivytown: Copy from HSX : CBDMA M2M+ROL traffic causes 3 strike (IIO Hang)
    //  [cloned to s4032101]
    Data32 |= BIT0;     //jkt_mode_disable_rfofreeze = 1

    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(TSWCTL1_IIO_DFX_GLOBAL_REG)), Data32);

    //
    // 3614137: CLONE from jaketown: PCIE: Request to change TxRxDetect from P leg only to alternating P/N Legs
    //  (4031655: also required for CRB build)
    //
    Data32 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, R_CC_TX_CTL));
    Data32 |= 2;      //tx_rxdet_ctl = 2;
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, R_CC_TX_CTL), Data32);


    // HSD 3613306 SetDfr Int Remap needs to be set, permanent WA
    SetDfrIntRemap(IioGlobalData,BusBase);


    //
    // hsx_eco: 257369 : HSX Clone: SC: Rand_Stress13b - IRP hang with Rd/Wr conflicts from multiple devices 
    // cloned to 4167723 - undo the change for BIOS HSD 4166930 
    // 
    if ((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping <= B0_REV_HSX)){
      RegLocation = ONLY_REGISTER_OFFSET(TSWCTL1_IIO_DFX_GLOBAL_REG);
      TswCtl1.Data = IioPciExpressRead32(IioGlobalData,PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, RegLocation));
      TswCtl1.Bits.np_pf_disable_global_iou1 = 1;
      TswCtl1.Bits.np_pf_disable_global_non_vc0 = 1;
      TswCtl1.Bits.np_pf_disable_global_misc = 1;
      TswCtl1.Bits.np_pf_disable_global_dmi_vc0 = 1;
      TswCtl1.Bits.np_pf_disable_global_iou2 = 1;
      TswCtl1.Bits.np_pf_disable_global_cb = 1;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, RegLocation), TswCtl1.Data);	  
 
      RegLocation = ONLY_REGISTER_OFFSET(TSWCTL0_IIO_DFX_GLOBAL_REG);
      TswCtl0.Data = IioPciExpressRead32(IioGlobalData,PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, RegLocation));
      TswCtl0.Bits.np_pf_disable_global_iou0 = 1;    
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, RegLocation), TswCtl0.Data);  
    } else {  // > HSX C0 , BDX
      RegLocation = ONLY_REGISTER_OFFSET(TSWCTL1_IIO_DFX_GLOBAL_REG);
      TswCtl1.Data = IioPciExpressRead32(IioGlobalData,PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, RegLocation));
      TswCtl1.Bits.np_pf_disable_global_iou1 = 0; 
      TswCtl1.Bits.np_pf_disable_global_non_vc0 = 0;
      TswCtl1.Bits.np_pf_disable_global_misc = 0;
      TswCtl1.Bits.np_pf_disable_global_dmi_vc0 = 0;
      TswCtl1.Bits.np_pf_disable_global_iou2 = 0;   
      TswCtl1.Bits.np_pf_disable_global_cb = 0;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, RegLocation), TswCtl1.Data);

      RegLocation = ONLY_REGISTER_OFFSET(TSWCTL0_IIO_DFX_GLOBAL_REG);
      TswCtl0.Data = IioPciExpressRead32(IioGlobalData,PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, RegLocation));
      TswCtl0.Bits.np_pf_disable_global_iou0 = 0;      
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, RegLocation), TswCtl0.Data);      
    }

    // 
    //4167204 :HSX Clone: Rckt: read tor timeout when under lock with oversubscribed tlb
    //
    if ((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping <= B0_REV_HSX)){
      Data32 =  IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(SWDBGCTL0_IIO_DFX_GLOBAL_REG))) ;
      Data32 &= ~BIT24;		//enable_extended_vtstf
      IioPciExpressWrite32 (IioGlobalData,  PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(SWDBGCTL0_IIO_DFX_GLOBAL_REG)), Data32);
    }

}

VOID
IioEarlyWABeforeBif (
    IIO_GLOBALS             *IioGlobalData,
    UINT8                   Iio
  )
{
  UINT8                                 BusBase;
  UINT8                                 BusUncore;
  UINT8                                 j;
  UINT32                                DbgClst1;
  UINT32                                Data32;
  UINT32                                DbgClst0;
  UINT16                                Pxpsqcnt;          // HSD 4166441 - Changed from 32 bits to 16 bits to match CSR size
  UINT32                                ClsPhyCtrl6;

  UINT32  RegLocation;

  UNIPHYPMCTL_IIO_DFX_STRUCT      UniPhyPmCtl;
  PXPPMCTRL_IIO_DFX_STRUCT        PxpPmCtrl;
  CLSPHYCTL6_IIO_DFX_STRUCT       sClsPhyCtrl6;
  LER_CTRLSTS_IIO_PCIEDMI_STRUCT  LerCtrlSts;
  CLSPHYCTL8_IIO_DFX_STRUCT           sClsPhyCtrl8;
  XPUNCERRSTS_IIO_PCIEDMI_STRUCT      XpUncErrSts;
  CLSPHYCTL1_IIO_DFX_HSX_BDX_STRUCT   ClsPhyCtrl1;
  TSWCTL1_IIO_DFX_GLOBAL_STRUCT         TswCtl1;
  LTSSMDBG0_IIO_DFX_STRUCT        LtssmDbg0;
  L0S_RX_IIO_DFX_STRUCT           L0sRx;
  UINT8   IioDfxDev;
  UINT8   IioDfxFunc;
  UINT16  Data16;
  EXPBERR1_IIO_DFX_STRUCT        Expberr1;
  EXPBERR3_IIO_DFX_STRUCT        Expberr3;

  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];
  BusUncore = IioGlobalData->IioVData.SocketUncoreBusNumber[Iio];
  //
  // apply IIO global permanent workaorunds
  //
  IioGlobalPermanentWorkarounds(IioGlobalData, Iio);
  //
  // Other miscellaneous ECOs
  //
  IioOtherMiecellaneousWorkarounds(IioGlobalData, Iio);

  //
  //Apply miscellaneous workarounds which are common among cross-processor generations
  //
  IioMiscellaneousCommonWorkarounds(IioGlobalData, Iio);
  
  //
  // Enable error logging to log when a link is down on DMI
  //
  IioDfxDev = IioGlobalData->PreLinkData.PcieInfo.PortInfo[0].DfxDevice;
  IioDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[0].DfxFunction;
  if ((IioGlobalData->IioVData.CpuType == CPU_HSX || IioGlobalData->IioVData.CpuType == CPU_BDX) && (OemIdentifyDmiDfxPort(IioGlobalData, BusBase, IioDfxDev, IioDfxFunc) == TRUE)) {
    //
    // s4986427: HSX Clone: CLONE ivytown: [PCIE_DMI]: SLD bit in uncerrsts not logged on DMI even though link is down
    //
    RegLocation = ONLY_REGISTER_OFFSET(EXPBERR1_IIO_DFX_REG);
    Expberr1.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
    Expberr1.Data &= ~(0x3FFF << 12);       //ltslog_stoplog[25:12] = 0x0
    Expberr1.Data |= (0x0100 << 12);        //ltslog_stoplog[25:12] = 0x100
    Expberr1.Data |= (BIT8);                //ltslog_cur_stoplog_bypass[8] = 1
    Expberr1.Data &= ~(BIT9);               //ltslog_start32log[9] = 0
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation), Expberr1.Data);

    RegLocation = ONLY_REGISTER_OFFSET(EXPBERR3_IIO_DFX_REG);
    Expberr3.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
    Expberr3.Data &= ~(0x3FFF << 14);       //ltslog_stoplog_mask[27:14] = 0x0
    Expberr3.Data |= (0x3FF << 14);         //ltslog_stoplog_mask[27:14] = 0x3FF
    Expberr3.Data &= ~(UINT32)0x3FFF;       //ltslog_cur_stoplog[13:0] = 0x0
    Expberr3.Data |= 0x100;                 //ltslog_cur_stoplog[13:0] = 0x100    
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation), Expberr3.Data);

    RegLocation = ONLY_REGISTER_OFFSET(EXPBERR1_IIO_DFX_REG);
    Expberr1.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
    Expberr1.Data |= (BIT9);                //ltslog_start32log[9] = 1
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation), Expberr1.Data);
  }

  //
  //Enable Non-Posted Prefetch for CBDMA traffic 
  //
  if ((IioGlobalData->IioVData.CpuType == CPU_IVT) || (IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
    //
    // s4031444: Enable Non-Posted Prefetch for CBDMA traffic in EX
    //              Reset Bit 2
    //
    RegLocation = ONLY_REGISTER_OFFSET(TSWCTL1_IIO_DFX_GLOBAL_REG);
    TswCtl1.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, RegLocation));
    TswCtl1.Bits.np_pf_disable_global_cb = 0;                                       
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, RegLocation), TswCtl1.Data);
  }

  //
  // Following are the Workarounds/ECOs which gets applied for each PCIe port
  //
  for(j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
    IioDfxDev = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxDevice;
    IioDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxFunction;

    // Skip IOSF port
    if(OemIdentifyVendorSpecificPort(IioGlobalData, BusBase, IioDfxDev, IioDfxFunc, TRUE) == TRUE)
      continue;

    if (!IioVData_ConfigurePciePort(IioGlobalData, Iio, j)) continue;

     if ((j == 0) || (j == 1) || (j == 3) || (j == 7)) { 
      //
      //ivt_eco: s4540515: PCIe Link equalization for Gen3 is mixed with PCIe
      //                   automatic BW change notification.
      //  [cloned to s4031085] - disable logging PCIE Link Equalization Request
      //                         from being logged in XPCOERRSTS, on all Gen3 ports
      if((IioGlobalData->IioVData.CpuType == CPU_IVT && IioGlobalData->IioVData.CpuStepping >= B0_REV_IVT) ||
          (IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)){
        if(j != NONE_GEN3_PCIE_PORT){
          Data32 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc,
                    ONLY_REGISTER_OFFSET(DFXCHICKENBIT1_IIO_DFX_REG)));
          Data32 |= BIT16;    //en_txn_eq_bw_only to 1
          IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc,
              ONLY_REGISTER_OFFSET(DFXCHICKENBIT1_IIO_DFX_REG)), Data32);
        }
      }
      //
      // HSD 4166557: HSX Clone: PCIE - Gen2/3 degraded width won't recover full width due to OC Done not being complete
      //
      if ((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping < B0_REV_HSX)) {
        if(IioGlobalData->SetupData.RxClockWA) {
          if (!((Iio == 0) && (j == 0))) {
            L0sRx.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, ONLY_REGISTER_OFFSET(L0S_RX_IIO_DFX_REG)));
            L0sRx.Bits.override_enable = 1;  
            IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, ONLY_REGISTER_OFFSET(L0S_RX_IIO_DFX_REG)), L0sRx.Data);

          }
        }
      }
      if(IioGlobalData->IioVData.CpuType == CPU_BDX) {
    	//
    	// bdx_eco 281127, bdx_sighting 4970718: Bits for enabling DLW SLD Fix
    	// 		are not set correctly in UP Recipe/BIOS
    	// [cloned to 5001603] - bits for enabling the DLW SLD fix applicable to BDX-DE V0 and all ML stepping
    	if( ((IioGlobalData->IioVData.CpuFamily == CPU_FAMILY_BDX_DE) && (IioGlobalData->IioVData.CpuStepping >= C0_REV_BDX)) ||
    		(IioGlobalData->IioVData.CpuFamily == CPU_FAMILY_BDX) ) {
    		RegLocation = ONLY_REGISTER_OFFSET(DEBUGCLUSTER1_IIO_DFX_REG);
    		DbgClst1 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
    		DbgClst1 |= BIT29 + BIT27;  //set spare_signals[0] = 1, spare_signals[2] = 1
    		IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation), DbgClst1);
    	}
      }
      if (IioGlobalData->IioVData.CpuType == CPU_IVT) {
        //
        // ivt_eco s4676646 (s4620466): PCIE:  Mellanox Gen3 device sending InitFC at
        //        Gen1 causes infinite Recovery->Config Loop or Recovery->Detect Loop
        //  [cloned to s4030408] - permanent WA
    	RegLocation = ONLY_REGISTER_OFFSET(DEBUGCLUSTER1_IIO_DFX_REG);
        DbgClst1 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
        DbgClst1 &= 0x07FFFFFF;     //clear [31: 27]
        DbgClst1 |= BIT27;  //set spare_signals[0] = 1

        //
        // ivt_eco: s4676741: PCIE Gen3: PML1 testing with Mellanox results in
        //                    SLDs and DLW results in link width degrade
        //  [cloned to s4030529] - permanent WA
        DbgClst1 &= 0xF807FFFF;     //clear [26: 19]
        // as per IVT s4677488:[PCIE]: SLD and Received UR status with PML1 test at Gen2 and Gen3
        // [cloned to s4031942] - value of l1exiteito is revised to 0x28
        DbgClst1 |= (0x28 << 19);   //DbgClst1.Bits.l1exiteito = 0x28;

        Data32 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc,
          ONLY_REGISTER_OFFSET(DFXCHICKENBIT0_IIO_DFX_REG)));
        Data32 &= ~(BIT28 + BIT27);
        Data32 |= BIT27;        //cfg_lanewait_timer_setting = 1b
        IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc,
          ONLY_REGISTER_OFFSET(DFXCHICKENBIT0_IIO_DFX_REG)), Data32);
  
        //
        // As per s4030772,"recexiteito" needs to be set to 0x10
        //
        DbgClst1 &= 0xFFFFF00F;     //clear [11: 4]
        DbgClst1 |= (0x10 << 4);    //DbgClst1.Bits.recexiteito = 0x10;
        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation), DbgClst1);
      }
      //
      // ivt_eco: s4714484:  PCIe Clock Gating not  Working on A1 DP System
      //  [cloned to s4030806] - disable port's physical lanes, if knob is set to disable
      //
      //
      // HSD 4166675: HSX: Former IVT Uplink Port Code is Present in BIOS
      //
      if (IsThisUplinkPort(IioGlobalData, Iio, j) == TRUE && IioGlobalData->SetupData.PciePortDisable[(Iio * NUMBER_PORTS_PER_SOCKET) + j] == 2) { //uplink disable

        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxClusterFunction,
            ONLY_REGISTER_OFFSET(REUTPHTDC_IIO_DFX_REG)), 0x0000F000);    //disable last 4 lanes of the cluster port
      }
    } // end if ((j == 0) || (j == 1) || (j == 3) || (j == 7))
    
    //  Note: The following ECO is also requested for IVT s4676884 [as per 4030576]
    // 3613972: CLONE from jaketown: CLONE from jaketown: PCIE: GEN3/EQ. Log registers showing LTSSM hangs in RECOVERY.WAIT_FOR_GEN_TRANS state
    //
    if (IioGlobalData->IioVData.CpuType == CPU_IVT) {
        RegLocation = ONLY_REGISTER_OFFSET(CLSPHYCTL8_IIO_DFX_REG);
      sClsPhyCtrl8.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation));
        sClsPhyCtrl8.Bits.rxsqexitw4alllanes = 0;
        sClsPhyCtrl8.Bits.spare0 |= 1;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), sClsPhyCtrl8.Data);
    }

    //
    //4168289:PCIE: HSX reported receiver (framing) errors in DLW tests.
    //
    if (((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping >= B0_REV_HSX)) || (IioGlobalData->IioVData.CpuType == CPU_BDX))  {
      if ((j == 0) || (j == 1) || (j == 3) || (j == 7)) { 
        RegLocation = ONLY_REGISTER_OFFSET(CLSPHYCTL8_IIO_DFX_REG);
        sClsPhyCtrl8.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation));
        sClsPhyCtrl8.Bits.en_ltidlestate2rxl0s = 0;
        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), sClsPhyCtrl8.Data);
      } 
    }

    //
    // 4168945: PCIE:  L0 20-lane reduced part hides Device 1 function 0
    // 4986974: HEDT (Lituya Bay) PCIe Bifurcation Set Incorrectly
    // 4986968: HEDT SKU w/PCIe 28-lanes, algorithm to allow IOU2 be detected as x4 does not work for 40-lanes
    //
    // D1:F0 was being hidden on HSW SKUs with reduced PCIe lanes (used only on HEDT),
    //  this W/A takes care of it. But, make sure we implement the W/A *only* on
    //  those parts, otherwise port bifurcation can get mangled.
    //
    // Read CAPID2 fuses in B1:D30:F3 to see if this IIO has reduced PCIe lanes
    // If PCIE_DISXPDEV != 0, lanes are reduced
    //
    if (j == 1) {
      Data32 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusUncore, PCIE_PORT_CPU_CAPIDS_DEV, PCIE_PORT_CPU_CAPIDS_FUNC, R_CPU_CAPID2));
      Data32 = (Data32 >> 3) & 0xFFF;
      if (Data32) {
        RegLocation = ONLY_REGISTER_OFFSET(CLSPHYCTL8_IIO_DFX_REG);
        sClsPhyCtrl8.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation));
        sClsPhyCtrl8.Bits.gate_on_all_queues = 0;
        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), sClsPhyCtrl8.Data);
      }
    }

    if( j==0 || j==1 || j==3 || j==7) {              
      // 3614123: CLONE from jaketown: Spurious receiver error logged when BFM aggresively goes into L0s 
      DbgClst0 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, R_DBGCLST0));
      DbgClst0 &= 0xFF87FFFF; 
      DbgClst0 |= BIT22 + BIT21;    //extend_eiosseen_cnt_ing3 = 3;
      DbgClst0 |= BIT20 + BIT19;    //extend_eiosseen_cnt_ing12 = 3;
      // 3614824: Data errors with Gen2 x16 nVidia cards (Once infered electrical idle in rec.cfg state it times out to detect in some cases Legaccy bug)
      DbgClst0 &= ~BIT30; //dis_infereifix4reccfgstate = 0;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, R_DBGCLST0), DbgClst0);

      // 3615404: PCIE: Endpoint Receiver Errors in SV testing
      // 3615505: PCIE: Staggering interval needs to be set to 4 
      Data32 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, R_UNIPHYPMCTL));
      Data32 &= 0xFE0FFFFF;  //clear [24:20] .Bits.dis_tx10sentrystagger = 0;
      if ((IioGlobalData->IioVData.CpuType == CPU_IVT) || (IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX))
        Data32 |= (BIT21+BIT20);      //for IVT pxp*_uniphypmctl.txlanestagint = 3
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, R_UNIPHYPMCTL), Data32);

      if ((IioGlobalData->IioVData.CpuType == CPU_IVT) || (IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
        Data32 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, ONLY_REGISTER_OFFSET(DFXCHICKENBIT0_IIO_DFX_REG)));
        Data32 |= BIT30;        //en_l0s_tx_txval_cntr = 1b
   
        //4167573: HSX Clone: PCIe - Rx termination not disabled during link_disable state
        if (!((Iio == 0) && (j == 0)) && ((IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)))
          Data32 &= ~BIT12;        //dis_send_ts1_pol_state = 0
        
        IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, ONLY_REGISTER_OFFSET(DFXCHICKENBIT0_IIO_DFX_REG)), Data32);
      }

      // 3615143: PCIE: GEN3: issue reported by Broadcom appears to lock up data link layer
      ClsPhyCtrl6 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, R_CLSPHYCTL6));
      ClsPhyCtrl6 &= 0xFFFC0000;  //clear bit17:0
      ClsPhyCtrl6 |= 0x1FFFF;  //dskindexmsk = 0x1FFFF;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, R_CLSPHYCTL6), ClsPhyCtrl6);


      // The following is approved for IVT as well
      // Update timer for data rate dependent calibration for ports that support Gen3 speeds
      // Note: As per the BSU 0.8.15: 3875429, 3613529, 3614291, 3246634: Data-Sampler VOC Values are
      // not Accurate
      //
      if ((IioGlobalData->IioVData.CpuType == CPU_IVT) || (IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)){
        if((j == 1) || (j == 3) || (j == 7)) {  //note: For IVT, remove j === 1
          //
          // for IOUx
          //
        ClsPhyCtrl1.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, R_CLSPHYCTL1));
          ClsPhyCtrl1.Bits.speedchangetime_constin4us = 0x3F;
        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, R_CLSPHYCTL1), ClsPhyCtrl1.Data);

        }
      }
    }   //end if( j==0 || j==1 || j==3 || j==7)

    //
    // 3615781: PCIE: Test BIOS to remove disable-rxreset-on-detect work-around (enable rxreset on detect)
    //
    if ((j == 0) || (j == 1) || (j == 3) || (j == 7)) { 
      RegLocation = ONLY_REGISTER_OFFSET(UNIPHYPMCTL_IIO_DFX_REG);
      UniPhyPmCtl.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      UniPhyPmCtl.Bits.dis_rxrst4inferredei = 0;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation), UniPhyPmCtl.Data);


      RegLocation = ONLY_REGISTER_OFFSET(CLSPHYCTL6_IIO_DFX_REG);
      sClsPhyCtrl6.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      sClsPhyCtrl6.Data &= ~BIT21;  //dis_uniphyrxrst4detectrecombine = 0;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation), sClsPhyCtrl6.Data);

    } // end if ((j == 0) || (j == 1) || (j == 3) || (j == 7))
    //
    // 3614100: CLONE from jaketown: PCIe setting change for nVidia and other SBR mis-behavors
    //
    if (IioGlobalData->IioVData.CpuType == CPU_IVT) {
      Pxpsqcnt = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, R_PXPSQCNT));
      Pxpsqcnt &= 0xE000;  //clear bit12:0
      Pxpsqcnt |= 4;       //sqcnt = 4;
      IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, R_PXPSQCNT), Pxpsqcnt);

    }

    // 3613833 PXT CCB: Remove PCIe L0s from the POR
    RegLocation = ONLY_REGISTER_OFFSET(PXPPMCTRL_IIO_DFX_REG);
    PxpPmCtrl.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
    PxpPmCtrl.Bits.use_cr_l0s_idle_timer = 0;
    PxpPmCtrl.Bits.l0s_idle_timer = 0;
    IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation), PxpPmCtrl.Data);


    //3614964, 3876204: PCIe: LER enable bit is sticky
    RegLocation = ONLY_REGISTER_OFFSET(LER_CTRLSTS_IIO_PCIE_REG);
    LerCtrlSts.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device, \
                                                     IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function, RegLocation));
    LerCtrlSts.Bits.ler_ss_status = 0x01;
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device, \
                                     IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function, RegLocation), LerCtrlSts.Data);

    RegLocation = ONLY_REGISTER_OFFSET(XPUNCERRSTS_IIO_PCIE_REG);
    XpUncErrSts.Data = 0;
    XpUncErrSts.Bits.outbound_poisoned_data = 0x01;
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device, \
                                     IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function, RegLocation), XpUncErrSts.Data);
    //
    // HSD 4030421: IVT PCIE Link Layer not returning posted data credits for MSI transactions
    //  Resolution: Remove 2nd part of W/A for s4030322
    //  Removed setting of XPDFXSPAREREG.disable_msi_gt_1dw to 1

    //
    // ivt_eco: b3247660: CLONE from jaketown: Bad Replay Packets during
    //                        PCIe NAK Test in Gen3 X16 config with 256B MPS
    //  [cloned to s4030491] - common for IVT EP,EX,
    //
    //  4167964: PCIe HSX UniPhy Recipe Updated to v5.00 (PCIe changes only)
    //  4985815: UniPhy Recipe Not Being Set On Port 0 Of Non-Legacy Sockets
    //    
    //  Skip DMI but do it for Port 0 in Non-Legacy Sockets.
    if(((j == 0) && (OemIdentifyDmiDfxPort(IioGlobalData, BusBase, IioDfxDev, IioDfxFunc) == FALSE)) ||
        (j == 1) || (j == 3) || (j == 7)) {
      RegLocation = ONLY_REGISTER_OFFSET(PXPRETRYCTRL3_IIO_DFX_REG);
      Data32 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      Data32 |= (BIT1 << 6);
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation), Data32);
    }

    //
    // 4032294 W/A for HSX B0, 
    // iio_cr_xpdfxsparereg_0_(6_0|6_3|7_0)_cfg::disable_msi_gt_1dw = 1 
    //
    if(IioGlobalData->IioVData.CpuType == CPU_HSX && IioGlobalData->IioVData.CpuStepping == B0_REV_HSX){
      RegLocation = ONLY_REGISTER_OFFSET(XPDFXSPAREREG_IIO_DFX_REG);
      Data32 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      Data32 |= BIT18;    //set disable_msi_gt_1dw to 1
      IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation), Data32);
    }

    //
    // 4031204: Legacy Pcie Txn layer bug with enabling csr_disable_ok2startnxttlp performance feature
    //
    // 4985746:  CLONE from HSX: OVL ERROR: OB TL sent data, but insufficient data
    if (((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping >= C0_REV_HSX)) ||
           (IioGlobalData->IioVData.CpuType == CPU_BDX) || (j == 0 || j == 3 || j == 7)) {
      RegLocation = ONLY_REGISTER_OFFSET(SPARE_IIO_DFX_REG);
      Data32 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      Data32 &= ~(0xF << 8);
      Data32 |= (BIT1 << 8);    //cgtt_sparebits[1] = 1, rest of the bits in the vector should be 0
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation), Data32);
    }

    //
    // HSD 4166558 (HSX): HSX Clone: PCIE: Pcode is setting the ltssmdbg0.forcereverse bit in IOU0 (Dev6 / Fun3)
    //
    if ((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping < B0_REV_HSX)) {
      if (j == 3){
        RegLocation = ONLY_REGISTER_OFFSET(LTSSMDBG0_IIO_DFX_REG);
        LtssmDbg0.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
        LtssmDbg0.Bits.forcereverse = 0;
        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), LtssmDbg0.Data);
      }
    }

    //
    // ivt_eco: 4677272: PCIE: In Gen1 X1 L1 test IVT sometimes drops EIOS
    //  [cloned to 4031304] - permanent WA for IVT
    RegLocation = ONLY_REGISTER_OFFSET(PRTPHYCTL_IIO_DFX_REG);
    Data32 = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
    Data32 &= ~(BIT25 + BIT24);
    Data32 |= BIT25;              //kalign_mode = 2 for "Dynamic up to L0"
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation), Data32);

    //
    // as per the s4031981, the WA for PM L2/3 failures as per the IVT s4677468
    // has to be removed..
    //
  } //end of for port

  //
  // 4985836: PCIe NTB: NTB settings set in DEVCAP and CAPPTR even in PCIe mode
  //  
  if (IioGlobalData->SetupData.NtbPpd[Iio] == NTB_PORT_DEF_TRANSPARENT ||
      (IioGlobalData->SetupData.NtbPpd[Iio] == NTB_PORT_DEF_NTB_AUTO && IioGlobalData->BdxSetupData.NtbPpdAutoConfig[Iio] == NTB_PORT_DEF_TRANSPARENT)) {
     Data16 = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, NTB_DEV, NTB_FUNC, R_PCIE_PORT_CAPPTR));
     Data16 = 0x40; // set 0x40 as CAPPTR for D3F0 if RP configuration is selected. Otherwise keep 0x60 for NTB configuration
     IioPciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(BusBase, NTB_DEV, NTB_FUNC, R_PCIE_PORT_CAPPTR), Data16);
  }
  //
  // bdx_eco: 287300-EP, 287525-DE:  CLONE from broadwell_server: CLONE: NTB bug fix is using bit22 in
  //                                 BIOS_MBX_PCU_MISC_CONFIG which is also used for pcode Rcomp Disable
  // [cloned to 5003238-DE, 5003239-EP] - For BDX V2/B0, when Port 3A is configured in NTB mode, initiate the
  //                                      B2P mailbox command PCU_MISC_CONFIG by setting bit 17, to enable
  //                                      the NTB workaround through PCODE.
  if( ((IioGlobalData->IioVData.CpuFamily == CPU_FAMILY_BDX_DE) && (IioGlobalData->IioVData.CpuStepping >= D0_REV_BDX)) ||
       (IioGlobalData->IioVData.CpuFamily == CPU_FAMILY_BDX) && (IioGlobalData->IioVData.CpuStepping >= B0_REV_BDX) ){
    if (IioGlobalData->SetupData.NtbPpd[Iio] == NTB_PORT_DEF_NTB_NTB ||
        IioGlobalData->SetupData.NtbPpd[Iio] == NTB_PORT_DEF_NTB_RP ||
        (IioGlobalData->SetupData.NtbPpd[Iio] == NTB_PORT_DEF_NTB_AUTO &&
            IioGlobalData->BdxSetupData.NtbPpdAutoConfig[Iio] != NTB_PORT_DEF_TRANSPARENT)) {
      B2P_Pcu_Misc_Config_WA( IioGlobalData, Iio, 17, TRUE);
    }
  }


  return;
}

VOID
WA_4986406(
  IIO_GLOBALS                                             *IioGlobalData,
  UINT8                                                   Iio
)
{
  PCI_ROOT_BRIDGE_PCI_ADDRESS         PciAddress;
  LNKSTS_IIO_PCIE_STRUCT              Lnksts;
  LBC_PER_IOU_CONTROL_IIO_DFX_STRUCT  LbcCtrl;
  LBC_PER_IOU_DATA_IIO_DFX_STRUCT     LbcData;
  BCTRL_IIO_PCIE_STRUCT               Bctrl;
  UINT32  RegLocation;
  UINT32  data32;
  UINT16  lane, oc_done_status;
  UINT8   ClusterDfxDev , ClusterDfxFunc;
  UINT8   CtleOp_OffScale, CtelOp_Range , CurrentBif;
  UINT8   j, LaneCount, StartLane, count;
  BOOLEAN sbr;

  if (!IioGlobalData->IioVData.SocketPresent[Iio])
    return;

  for (j = 0 ; j < NUMBER_PORTS_PER_SOCKET ; j++) {

    // Check ports based on bifurcation width only
    CurrentBif = IioGlobalData->IioOutData.CurrentPXPMap[(Iio*VARIABLE_GROUP_ELEMENTS)+j];
    if (!CurrentBif) 
      continue;

    sbr = FALSE;       // set it to false initialy for every port
    PciAddress.Bus      = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];
    PciAddress.Device   = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
    PciAddress.Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;
    if( IioVData_ConfigurePciePort(IioGlobalData, Iio, j) == FALSE)  //check whether the PCIe port can be configured
        continue;
    //
    // WA not applicable for DMI & IOSF
    //
    if ( OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE)
      continue;
    
    if(OemIdentifyVendorSpecificPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function, FALSE) == TRUE)
      continue;

    Lnksts.Data = IioPciExpressRead16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, LNKSTS_IIO_PCIE_REG));
    //
    // step 1. If Link in L0 then only apply WA
    //
    if(!(Lnksts.Bits.data_link_layer_link_active)) {
      IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO,"Skt[%d], D[%d]:F[%d]", Iio, PciAddress.Device, PciAddress.Function);
      IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, " : Link Down , WA not required! \n");
      continue;
    }    
    //
    // step 2. If link is not in Gen3 , then apply WA
    //
    if (Lnksts.Bits.current_link_speed == GEN3_LINK_SPEED_GEN3) {
      IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO,"Skt[%d], D[%d]:F[%d]", Iio, PciAddress.Device, PciAddress.Function);
      IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, " : Link up in Gen3 , WA not required\n");
      continue;
    }
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO,"Skt[%d], D[%d]:F[%d]", Iio, PciAddress.Device, PciAddress.Function);
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, " : Link up in Gen1/Gen2 , Process Lanes for WA \n");
    //
    // step 3.Obtain the lane count based on the current port bifurcation    
    //
    StartLane       = LbcStartLane[j];
    LaneCount       = CurrentBif;   // Process lanes equivalent to port bifurcation i.e. x2 = 2 lanes, x4 = 4 lanes , x8 = 8 lanes 

    ClusterDfxDev  = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxClusterDevice;
    ClusterDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxClusterFunction;

    for (lane = StartLane; lane < StartLane + LaneCount ; lane++ ) {
      data32 = IioPciExpressRead32(IioGlobalData,PCI_PCIE_ADDR(PciAddress.Bus, ClusterDfxDev, ClusterDfxFunc, OC_DONE_IIO_DFX_REG));
      //
      // step 4. Check oc_done for each lane , skip lane if oc_done = 1
      //
      oc_done_status = data32 & lane;
      if(oc_done_status) 
        continue;
      //
      // step 5. Use Load Bus command to check "err_ctle" BIT8 for each lane , skip lane if err_ctle == 0
      //
      LbcCtrl.Data = 0;
      LbcCtrl.Bits.lbc_start = 0x1;
      LbcCtrl.Bits.lbc_req = LBC_READ_COMMAND;   // read
      LbcCtrl.Bits.lbc_ld_sel = LOAD_BUS_ADDRESS_0; // command
      LbcCtrl.Bits.cfg_cya_delay_rd_capture = 1;
      LbcCtrl.Data |= (1 << (lane + 11));
      IioPciExpressWrite32 (IioGlobalData,PCI_PCIE_ADDR( PciAddress.Bus, ClusterDfxDev, ClusterDfxFunc, LBC_PER_IOU_CONTROL_IIO_DFX_REG), LbcCtrl.Data);

      count = 0; 
      do {
        count++;
        IioStall (IioGlobalData, LBC_READ_DELAY);    // 50 microsecs. minimun 1 us
        LbcData.Data = IioPciExpressRead32 (IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, ClusterDfxDev, ClusterDfxFunc, LBC_PER_IOU_DATA_IIO_DFX_REG));
      } while ((LbcData.Bits.lbc_data_valid == 0) && (count < 10)); // check the valid bit[0]
      
      if( count == 10) { 
       IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO,"Skt[%d], D[%d]:F[%d]:Lane[%d]", Iio, PciAddress.Device, PciAddress.Function,lane);
       IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO," : Count Timeout OR LBC command fail \n");
       continue;
      }

      IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO,"LBC Read value on lane:%d is 0x%x \n", lane , LbcData.Data);
     
      if(!(LbcData.Bits.lbc_data & V_LANE_ERR_CTLE))
        continue; 
      //
      // step 6. If Offset is saturated i.e. either 0x0 or 0xf , then only apply WA
      //     
      CtleOp_OffScale = (LbcData.Bits.lbc_data & B_CTLEOP_OFFSCAL) >> 2;
      CtelOp_Range    = LbcData.Bits.lbc_data & B_CTLEOP_RANGE;

      if(!(CtleOp_OffScale == 0 || CtleOp_OffScale == 0xF))
        continue;

      if((CtleOp_OffScale == 0 &&  CtelOp_Range== V_CTLEOP_RANGE_R0) || (CtleOp_OffScale == 0xF &&  CtelOp_Range== V_CTLEOP_RANGE_R2))
        continue; // No WA required
      else if (CtleOp_OffScale == 0)
        CtelOp_Range = V_CTLEOP_RANGE_R0;
      else if (CtleOp_OffScale == 0xF)
        CtelOp_Range = V_CTLEOP_RANGE_R2;
  
      //
      // step 7 Restore the range through Load Bus command
      //
      LbcData.Data = 0;
      LbcData.Bits.lbc_data |= CtelOp_Range;
      IioPciExpressWrite32 (IioGlobalData,PCI_PCIE_ADDR( PciAddress.Bus, ClusterDfxDev, ClusterDfxFunc, LBC_PER_IOU_DATA_IIO_DFX_REG), LbcData.Data);

      LbcCtrl.Data = 0;
      LbcCtrl.Bits.lbc_start = 0x1;
      LbcCtrl.Bits.lbc_req = LBC_WRITE_COMMAND;   // write
      LbcCtrl.Bits.lbc_ld_sel = LOAD_BUS_ADDRESS_1; // command
      LbcCtrl.Data |= (1 << (lane + 11));
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( PciAddress.Bus, ClusterDfxDev, ClusterDfxFunc, LBC_PER_IOU_CONTROL_IIO_DFX_REG), LbcCtrl.Data);

      sbr = TRUE; // set it to TRUE if we come here           
    } // for(lane)

    //
    // step 8. Issue sbr if flag is set
    if ( sbr ) {
      IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO,"Skt[%d], D[%d]:F[%d]", Iio, PciAddress.Device, PciAddress.Function);
      IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, " : Issuing SBR to apply WA  \n");

      RegLocation = ONLY_REGISTER_OFFSET(BCTRL_IIO_PCIE_REG);
      Bctrl.Data = IioPciExpressRead16(IioGlobalData,PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, RegLocation));
      Bctrl.Bits.sbr = 1;
      IioPciExpressWrite16 (IioGlobalData,PCI_PCIE_ADDR( PciAddress.Bus, PciAddress.Device, PciAddress.Function, RegLocation), Bctrl.Data);
      IioStall (IioGlobalData, GEN3_STALL_SBR);
      
      Bctrl.Bits.sbr = 0;
      IioPciExpressWrite16 (IioGlobalData,PCI_PCIE_ADDR( PciAddress.Bus, PciAddress.Device, PciAddress.Function, RegLocation), Bctrl.Data);  				
      IioStall (IioGlobalData, IIO_LINK_TRAIN_TIME_MARGIN_G3EQ);
    } // if (sbr)
  } // for(ports) 
}

VOID
IioEarlyWAAfterBif(
    IIO_GLOBALS                                             *IioGlobalData,
    UINT8                                                   Iio
)
{
  //
  //bdx_eco:270596:CLONE from broadwell_server: Refile 269861:  CTLE/INCMGEN FSM gives no offset compensation in the saturation case
  //[cloned to s4986406 , s4987760 , s5001775] - Permanent to BDX all stepping
  //
  if (IioGlobalData->IioVData.CpuType == CPU_BDX ) {
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Program WA 4986406 \n");
    WA_4986406(IioGlobalData , Iio);
  } // CPU_BDX
}

/*++
=========================================================================================
============ Subroutine Definition ends here, Main control routine starts here ==========
=========================================================================================
--*/
VOID
IioEarlyWorkAround (
    IIO_GLOBALS                                     *IioGlobalData,
    UINT8                                           Iio,
    IIO_INIT_ENUMERATION                            Phase
    )
{

  switch(Phase) {
    case IioBeforeBifurcation:
      ClearXPUncErrStsBit6(IioGlobalData, Iio);
      IioEarlyWABeforeBif(IioGlobalData, Iio);
      break;

    case IioAfterBifurcation:
      IioEarlyWAAfterBif(IioGlobalData, Iio);
      break;

    default:
      break;
  }

  UpdateVDataAfterApplyWorkAround(IioGlobalData, Iio);

  return; 
}
