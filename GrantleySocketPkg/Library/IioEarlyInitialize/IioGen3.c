
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

  IioGen3.c

Abstract:

  Iio Gen3 initialization.

--*/

#include "IioEarlyInitialize.h"
#include "IioGen3TxEq.h"

IIO_STATUS
Gen3ProgPrivTxEqCoefficients(
  IIO_GLOBALS  *IioGlobalData,
  UINT8 Socket,
  UINT8 Port,
  UINT32 Precursor,
  UINT32 Cursor,
  UINT32 Postcursor
  );

IIO_STATUS
Gen3ProgPrivRmtTxEqCoefficients(
  IIO_GLOBALS  *IioGlobalData,
  UINT8 Socket,
  UINT8 Port,
  UINT32 Precursor,
  UINT32 Cursor,
  UINT32 Postcursor
  );

IIO_STATUS
Gen3ProgTxPh3Coefficients(
  IIO_GLOBALS  *IioGlobalData,
  UINT8 Socket,
  UINT8 Port,
  UINT8 Gen3EqMode
  );

GEN3_STATUS
Gen3GetLinkStatus(
  IIO_GLOBALS  *IioGlobalData,
  UINT8 Socket,
  UINT8 Port,
  UINT32 *LtssmState,
  UINT16 *LinkSpeed,
  UINT16 *LinkWidth
  );

GEN3_STATUS
Gen3ForceGen1 (
  IIO_GLOBALS                       *IioGlobalData,
  UINT8                             Socket,
  UINT8                             Port
  );

GEN3_STATUS
Gen3GetEndpointInfo(
    IIO_GLOBALS                      *IioGlobalData,
    UINT8                            Socket,
    UINT8                            Port,
    UINT16                          *Vid,
    UINT16                          *Did,
    UINT32                          *MaxSpeed,
    UINT32                          *MaxWidth
    );

VOID
AssignPreLinkGen3ConfigVariable (
	IIO_GLOBALS       *IioGlobalData,
	UINT8             Iio,
	UINT8			  PrtIdx,
	UINT8			  PrimPrtIdx
)
{
  UINT8		TargetPortIndex = (Iio * NUMBER_PORTS_PER_SOCKET) + PrtIdx;
  UINT8		PrimaryPortIndex = (Iio * NUMBER_PORTS_PER_SOCKET) + PrimPrtIdx;

  if(TargetPortIndex == PrimaryPortIndex)
	return;

  IioGlobalData->SetupData.Gen3EqMode[TargetPortIndex] =
		  IioGlobalData->SetupData.Gen3EqMode[PrimaryPortIndex];
  IioGlobalData->SetupData.Gen3SpecMode[TargetPortIndex] =
  		  IioGlobalData->SetupData.Gen3SpecMode[PrimaryPortIndex];
  IioGlobalData->SetupData.Gen3Phase2EqMode[TargetPortIndex] =
  		  IioGlobalData->SetupData.Gen3Phase2EqMode[PrimaryPortIndex];
  IioGlobalData->SetupData.Phase2PrivTxEqOvrPreCursor[TargetPortIndex] =
  		  IioGlobalData->SetupData.Phase2PrivTxEqOvrPreCursor[PrimaryPortIndex];
  IioGlobalData->SetupData.Phase2PrivTxEqOvrCursor[TargetPortIndex] =
  		  IioGlobalData->SetupData.Phase2PrivTxEqOvrCursor[PrimaryPortIndex];
  IioGlobalData->SetupData.Phase2PrivTxEqOvrPostCursor[TargetPortIndex] =
  		  IioGlobalData->SetupData.Phase2PrivTxEqOvrPostCursor[PrimaryPortIndex];
  IioGlobalData->SetupData.Gen3Phase3EqMode[TargetPortIndex] =
  		  IioGlobalData->SetupData.Gen3Phase3EqMode[PrimaryPortIndex];
  IioGlobalData->SetupData.Phase3PrivTxEqOvrPreCursor[TargetPortIndex] =
  		  IioGlobalData->SetupData.Phase3PrivTxEqOvrPreCursor[PrimaryPortIndex];
  IioGlobalData->SetupData.Phase3PrivTxEqOvrCursor[TargetPortIndex] =
  		  IioGlobalData->SetupData.Phase3PrivTxEqOvrCursor[PrimaryPortIndex];
  IioGlobalData->SetupData.Phase3PrivTxEqOvrPostCursor[TargetPortIndex] =
  		  IioGlobalData->SetupData.Phase3PrivTxEqOvrPostCursor[PrimaryPortIndex];
  IioGlobalData->SetupData.Gen3DnTxPreset[TargetPortIndex] =
  		  IioGlobalData->SetupData.Gen3DnTxPreset[PrimaryPortIndex];
  IioGlobalData->SetupData.Gen3DnRxPreset[TargetPortIndex] =
  		  IioGlobalData->SetupData.Gen3DnRxPreset[PrimaryPortIndex];
  IioGlobalData->SetupData.Gen3UpTxPreset[TargetPortIndex] =
  		  IioGlobalData->SetupData.Gen3UpTxPreset[PrimaryPortIndex];
}

VOID
AlignGen3PreLinkConfigurationSettings (
	IIO_GLOBALS       *IioGlobalData,
	UINT8             Socket
)
/*
 * This Routine should take care of Gen3 EQ user configuration settings which are applied
 * before the Gen3 link training
 */
{
  UINT8   Iou2, Iou0, Iou1, pxp;

  Iou2 = IioGlobalData->SetupData.ConfigIOU2[Socket];
  Iou2 = (Iou2 >= IOU2_BIF_RANGE) ? (IOU2_BIF_RANGE - 1) : Iou2;

  Iou0 = IioGlobalData->SetupData.ConfigIOU0[Socket];
  Iou0 = (Iou0 >= x16_IOU_BIF_RANGE) ? (x16_IOU_BIF_RANGE - 1) : Iou0;

  Iou1 = IioGlobalData->SetupData.ConfigIOU1[Socket];
  Iou1 = (Iou1 >= x16_IOU_BIF_RANGE) ? (x16_IOU_BIF_RANGE - 1) : Iou1;

  for (pxp = 0; pxp < NUMBER_PORTS_PER_SOCKET; pxp++) {
	switch(pxp){
	case PORT_1A_INDEX:
	case PORT_1B_INDEX:
		AssignPreLinkGen3ConfigVariable(
				IioGlobalData,
				Socket,
				pxp,
				IOU2_PrimaryPortIndexMap[Iou2][pxp - PCIE_PORT_1A_SUPER_CLUSTER_PORT]
		);
		break;
	case PORT_2A_INDEX:
	case PORT_2B_INDEX:
	case PORT_2C_INDEX:
	case PORT_2D_INDEX:
		AssignPreLinkGen3ConfigVariable(
				IioGlobalData,
				Socket,
				pxp,
				IOU0_PrimaryPortIndexMap[Iou0][pxp - PCIE_PORT_2A_SUPER_CLUSTER_PORT]
		);
		break;
	case PORT_3A_INDEX:
	case PORT_3B_INDEX:
	case PORT_3C_INDEX:
	case PORT_3D_INDEX:
		AssignPreLinkGen3ConfigVariable(
				IioGlobalData,
				Socket,
				pxp,
				IOU1_PrimaryPortIndexMap[Iou1][pxp - PCIE_PORT_3A_SUPER_CLUSTER_PORT]
		);
		break;
	default:
		break;
	}
  }
}

VOID
ProgramGen3Equalization (
    IIO_GLOBALS       *IioGlobalData,
    UINT8 Socket
    )
{
  UINT8   i, j, k;
  UINT8   BusBase;
  UINT8   Device;
  UINT8   Function;
  UINT32  RegLocation;
  UINT8   Gen3EqMode, Gen3SpecMode;
  UINT8   IioDfxDev;
  UINT8   IioDfxFunc;
  PXPPRTGEN3EQPRIV_IIO_DFX_STRUCT             PxpPrtGen3EqPriv;
  TXEQREUTPRIV_IIO_DFX_STRUCT                 TxEqReutPriv;
  GEN3PRIVTXEQ_IIO_DFX_STRUCT                 Gen3PrivTxEq;
  GEN3PRIVRMTTXEQ_IIO_DFX_STRUCT              Gen3PrivRmtTxEq;
  LNKCON2_IIO_PCIEDMI_STRUCT                  PcieLnkCon2;
  LN0EQ_IIO_PCIE_STRUCT                       LnEq;

  i = Socket;

  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[i];

  //
  // Implementation note:
  // Not all of the per-port buffered registers are actually per-port (TxEqReutPriv), they are per-cluster.
  // In general this code avoids adding a unneccessary conditionals when setting up those registers for the various
  // modes when the assignment is a don't care for a per-port register that doesn't control a cluster (any device
  // other than j == 1 || j == 3 || j == 7).  Instead the code always checks to make sure the DFX register controls
  // a cluster before it writes the buffered register.
  //

  for (j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
    IioDfxDev = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxDevice;
    IioDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxFunction;
    Device = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
    Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;

    // Skip IOSF port
    if(OemIdentifyVendorSpecificPort(IioGlobalData, BusBase, IioDfxDev, IioDfxFunc, TRUE) == TRUE)
      continue;

    if (j == NONE_GEN3_PCIE_PORT) continue; /* Gen3 not supported on port 0 */

    if ((IioGlobalData->IioOutData.PciePortPresent[(i*NUMBER_PORTS_PER_SOCKET)+j] == 0 ) ||
        (IioGlobalData->IioOutData.PciePortConfig[(i*NUMBER_PORTS_PER_SOCKET)+j] == 0 ) )  continue;
    //
    // As per ivt_eco s4714484, if user option is to disable the uplink port,
    // than the physical lanes would also be disabled, hence skip uplink Gen3 EQ init
    //  [cloned to s4030806]
    if (IsThisUplinkPort(IioGlobalData, i, j) == TRUE &&
        IioGlobalData->SetupData.PciePortDisable[(i*NUMBER_PORTS_PER_SOCKET)+j] == 2) //uplink disable
      continue;

    Gen3EqMode = IioGlobalData->SetupData.Gen3EqMode[i*NUMBER_PORTS_PER_SOCKET + j];
    Gen3SpecMode  = IioGlobalData->SetupData.Gen3SpecMode[i*NUMBER_PORTS_PER_SOCKET + j];

    // Still don't do auto-negotiation on PBG C0, but allow it if requested.  Will change.
    if (IsThisUplinkPort(IioGlobalData, i, j) == TRUE && Gen3EqMode == GEN3_EQ_MODE_AUTO ) {
      Gen3EqMode = GEN3_EQ_MODE_FIXED;
      IioGlobalData->SetupData.Gen3EqMode[i*NUMBER_PORTS_PER_SOCKET + j] = GEN3_EQ_MODE_FIXED;
    }

    if (Gen3EqMode == GEN3_EQ_MODE_AUTO) {
      Gen3EqMode = GEN3_EQ_MODE_ADVANCED;
      IioGlobalData->SetupData.Gen3EqMode[i*NUMBER_PORTS_PER_SOCKET + j] = GEN3_EQ_MODE_ADVANCED;
      IioGlobalData->SetupData.Gen3Phase2EqMode[i*NUMBER_PORTS_PER_SOCKET + j] = GEN3_EQ_MODE_HW_ADAPTIVE;
      if ((IioGlobalData->IioVData.CpuType == CPU_IVT) || (IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX))
        IioGlobalData->SetupData.Gen3Phase3EqMode[i*NUMBER_PORTS_PER_SOCKET + j] = GEN3_EQ_MODE_MAXBOOST_HW;
    }

    if (Gen3SpecMode == GEN3_SPEC_MODE_AUTO) {
      Gen3SpecMode  = GEN3_SPEC_MODE_0P71_SEPT;
    }

    //
    // If the first port in the cluster requested a cluster-based override, enforce this on the other ports.
    //
    if (Gen3EqMode == GEN3_EQ_MODE_DISABLE_P0_P1_P2_P3 || Gen3EqMode == GEN3_EQ_MODE_ENABLE_P1_ONLY) {
      if (j == 1 || j == 3 || j == 7) {
        IioGlobalData->SetupData.Gen3EqMode[i*NUMBER_PORTS_PER_SOCKET + j + 0] = Gen3EqMode;
        if (j > 0) {
          IioGlobalData->SetupData.Gen3EqMode[i*NUMBER_PORTS_PER_SOCKET + j + 1] = Gen3EqMode;
        } 
        if (j > 1) {
          IioGlobalData->SetupData.Gen3EqMode[i*NUMBER_PORTS_PER_SOCKET + j + 2] = Gen3EqMode;
          IioGlobalData->SetupData.Gen3EqMode[i*NUMBER_PORTS_PER_SOCKET + j + 3] = Gen3EqMode;
        }
      }
    }
    RegLocation = ONLY_REGISTER_OFFSET(PXPPRTGEN3EQPRIV_IIO_DFX_REG);
    PxpPrtGen3EqPriv.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation));
    RegLocation = ONLY_REGISTER_OFFSET(TXEQREUTPRIV_IIO_DFX_REG);
    TxEqReutPriv.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation));
    if ((i == 0) && (j == 0)) {
      RegLocation = ONLY_REGISTER_OFFSET(LNKCON2_IIO_PCIEDMI_REG);    // This is DMI port
    } else {
      RegLocation = ONLY_REGISTER_OFFSET(LNKCON2_IIO_PCIE_REG);   // This is a PCIe port
    }
    PcieLnkCon2.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR( BusBase, Device, Function, RegLocation));
    switch (Gen3EqMode) {
    case GEN3_EQ_MODE_ENABLE_P0_P1_P2_P3:
      //PxpPrtGen3EqPriv.Bits.dis_mask_p2l_lnkup4eq = 1;
      break;

    case GEN3_EQ_MODE_DISABLE_P0_P1_P2_P3:
      TxEqReutPriv.Bits.dis_cmp_00ecinrcvrlck = 1;
      TxEqReutPriv.Bits.dis_cmp_nzecinrcvrlck = 1;
      TxEqReutPriv.Bits.dis_g12eqts1gen = 1;
      TxEqReutPriv.Bits.dis_g12eqts2gen = 1;
      PxpPrtGen3EqPriv.Bits.bypass_g3eq = 1;
      PxpPrtGen3EqPriv.Bits.use_g3eqprivatecsrvalues = 1;
      break;

    case GEN3_EQ_MODE_ENABLE_P0_P1_ONLY:
      PxpPrtGen3EqPriv.Bits.dis_eqph23 = 1;
      //PxpPrtGen3EqPriv.Bits.dis_mask_p2l_lnkup4eq = 1;
      break;

    case GEN3_EQ_MODE_ENABLE_P1_ONLY:
      TxEqReutPriv.Bits.dis_g12eqts2gen = 1;
      PxpPrtGen3EqPriv.Bits.dis_eqph23 = 1;
      //PxpPrtGen3EqPriv.Bits.dis_mask_p2l_lnkup4eq = 1;
      break;

    case GEN3_EQ_MODE_ADVANCED:
      //PxpPrtGen3EqPriv.Bits.dis_mask_p2l_lnkup4eq = 1;
      PxpPrtGen3EqPriv.Bits.dis_eqph23 = 0;
      PxpPrtGen3EqPriv.Bits.byp_adaptsm_but_capcoeff = 1;
      PxpPrtGen3EqPriv.Bits.bypass_g3eq = 0;
      PxpPrtGen3EqPriv.Bits.use_g3eqprivatecsrvalues = 0;

      if ((IioGlobalData->SetupData.Gen3Phase2EqMode[i * NUMBER_PORTS_PER_SOCKET + j] == GEN3_EQ_MODE_LOOKUP_TABLE ||
           IioGlobalData->SetupData.Gen3Phase3EqMode[i * NUMBER_PORTS_PER_SOCKET + j] == GEN3_EQ_MODE_LOOKUP_TABLE ||
           IioGlobalData->SetupData.Gen3Phase3EqMode[i * NUMBER_PORTS_PER_SOCKET + j] == GEN3_EQ_MODE_MAXBOOST_LOOKUP_TABLE) &&
          (CheckVarPortLinkSpeed(IioGlobalData, i, j) == GEN3_LINK_SPEED_GEN3 )
          ) {
        PcieLnkCon2.Bits.target_link_speed = 2;
      }

      if (IioGlobalData->SetupData.Gen3Phase3EqMode[i * NUMBER_PORTS_PER_SOCKET + j] == GEN3_EQ_MODE_MAXBOOST_SW) {
        PxpPrtGen3EqPriv.Bits.dis_eqph23 = 1;
      }

      // Many of the advanced modes can't use 0.7 July.
      if (Gen3SpecMode == GEN3_SPEC_MODE_0P7_JULY) {
        Gen3SpecMode = GEN3_SPEC_MODE_0P71_SEPT;
      }

      break;
    case GEN3_EQ_MODE_FIXED:
      //PxpPrtGen3EqPriv.Bits.dis_mask_p2l_lnkup4eq = 1;
      PxpPrtGen3EqPriv.Bits.use_g3eqprivatecsrvalues = 1;
      PxpPrtGen3EqPriv.Bits.use_g3eqprivcsr4adaptsm = 1;
      RegLocation = ONLY_REGISTER_OFFSET(GEN3PRIVTXEQ_IIO_DFX_REG);
      Gen3PrivTxEq.Data = IioPciExpressRead32( IioGlobalData,  PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      RegLocation = ONLY_REGISTER_OFFSET(GEN3PRIVRMTTXEQ_IIO_DFX_REG);
      Gen3PrivRmtTxEq.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      Gen3ProgPrivTxEqCoefficients(IioGlobalData, i, j,
                                    Gen3PrivTxEq.Bits.bndl0_g3ptxeq_precursor,
                                    Gen3PrivTxEq.Bits.bndl0_g3ptxeq_cursor,
                                    Gen3PrivTxEq.Bits.bndl0_g3ptxeq_postcursor);
      Gen3ProgPrivRmtTxEqCoefficients(IioGlobalData, i, j,
                                      Gen3PrivRmtTxEq.Bits.bndl0_g3prmttxeq_precursor,
                                      Gen3PrivRmtTxEq.Bits.bndl0_g3prmttxeq_cursor,
                                      Gen3PrivRmtTxEq.Bits.bndl0_g3prmttxeq_postcursor);
      break;
     //
     // 4986019 : Add Low-Loss Equalization Option for PCIe GEN3 to BIOS Setup Menu
     //
     case GEN3_EQ_MODE_ENABLE_MMM_OFF_WEST:
       // Program pxpX_tx_ph3_cursor.b1sub_post_cursor, this is a per-bundle field.
       // So this means it must be set for all ports. If, for example, we are looking at 
       // IOU2 and it is bifurcated as a x8, then pxp2_tx_ph3_cursor (port 2 (1a)) 
       // as well as pxpX_tx_ph3_cursor (port 3 (1b)). 
       Gen3ProgTxPh3Coefficients(IioGlobalData, i, j, GEN3_EQ_MODE_ENABLE_MMM_OFF_WEST);
       break;

     case GEN3_EQ_MODE_ALT_SHORT_CHANNEL:
       //
       // 4971127: BDX EX: PCIe Gen 3 Alternate Short Channel EQ
       //
       Gen3ProgTxPh3Coefficients(IioGlobalData, i, j, GEN3_EQ_MODE_ALT_SHORT_CHANNEL);
       break;


    default:
      IioCpuDeadLoop();
    }

    switch (Gen3SpecMode) {
    case GEN3_SPEC_MODE_0P7_JULY:
      PxpPrtGen3EqPriv.Bits.en_july0_7spec = 1;
      PxpPrtGen3EqPriv.Bits.dis_txdcbal = 1;
      PxpPrtGen3EqPriv.Bits.dis_rxdcbal = 0;
      PxpPrtGen3EqPriv.Bits.bypass_txeqcoeff_rule4b = 1;
      break;

    case GEN3_SPEC_MODE_0P7_SEPT:
      PxpPrtGen3EqPriv.Bits.en_july0_7spec = 0;
      PxpPrtGen3EqPriv.Bits.dis_txdcbal = 1;
      PxpPrtGen3EqPriv.Bits.dis_rxdcbal = 0;
      PxpPrtGen3EqPriv.Bits.bypass_txeqcoeff_rule4b = 0;
      break;

    case GEN3_SPEC_MODE_0P71_SEPT:
      PxpPrtGen3EqPriv.Bits.en_july0_7spec = 0;
      PxpPrtGen3EqPriv.Bits.dis_txdcbal = 0;
      PxpPrtGen3EqPriv.Bits.dis_rxdcbal = 0;
      PxpPrtGen3EqPriv.Bits.bypass_txeqcoeff_rule4b = 0;
      break;

    default:
      IioCpuDeadLoop();
    }
    //
    // as per s4030771, setting of "en_gen3dis200mstimer4eqfail" to 1, is common b/w IVT and HSX
    //
    PxpPrtGen3EqPriv.Bits.en_gen3dis200mstimer4eqfail = 1;

      if (j != 0) {
        RegLocation = ONLY_REGISTER_OFFSET(PXPPRTGEN3EQPRIV_IIO_DFX_REG);
        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation),
                      PxpPrtGen3EqPriv.Data);
      }

      if (j == 1 || j == 3 || j ==7) {
        RegLocation = ONLY_REGISTER_OFFSET(TXEQREUTPRIV_IIO_DFX_REG);
        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation),
                    TxEqReutPriv.Data);
      }

    if (j != 0) {
      for (k = 0; k < MaxGen3EqRegs[j]; k++) {
        RegLocation = ONLY_REGISTER_OFFSET(LN0EQ_IIO_PCIE_REG) + (k * 2);
        LnEq.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, Device, Function, RegLocation));
        
        if (IioGlobalData->SetupData.Gen3DnTxPreset[i*NUMBER_PORTS_PER_SOCKET + j] != GEN3_TX_PRESET_AUTO) {
         LnEq.Bits.dntxpreset = IioGlobalData->SetupData.Gen3DnTxPreset[i*NUMBER_PORTS_PER_SOCKET + j];
        } else {
         LnEq.Bits.dntxpreset = 7;   //as per s4030837, set similar to Romley
        }
        if (IioGlobalData->SetupData.Gen3DnRxPreset[i*NUMBER_PORTS_PER_SOCKET + j] != GEN3_RX_PRESET_AUTO) {
         LnEq.Bits.dnrxpreset = IioGlobalData->SetupData.Gen3DnRxPreset[i*NUMBER_PORTS_PER_SOCKET + j];
        }
        if (IioGlobalData->SetupData.Gen3UpTxPreset[i*NUMBER_PORTS_PER_SOCKET + j] != GEN3_TX_PRESET_AUTO) {
          LnEq.Bits.uptxpreset = IioGlobalData->SetupData.Gen3UpTxPreset[i*NUMBER_PORTS_PER_SOCKET + j];
        } else {
            // 4986238: upTxPreset UniPhy Recipe Parameter Not Set Correctly in C0 BIOS
            LnEq.Bits.uptxpreset = 7; 
        }
          IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR(BusBase, Device, Function, RegLocation), LnEq.Data);;
        }
      }

    if (IioGlobalData->SetupData.Gen3LinkupFlow[i*NUMBER_PORTS_PER_SOCKET + j] != GEN3_LINKUP_FLOW_DISABLED) {
      PcieLnkCon2.Bits.target_link_speed = 1;
    }
    IioStall (IioGlobalData, UNIPHY_RECIPE_DELAY);   //Intended fix for s4030382: give a delay of 180us after programming Gen3 EQ
  } // j = port

}

GEN3_STATUS
Gen3GetLinkStatus (
  IIO_GLOBALS  *IioGlobalData,
  UINT8 Socket,
  UINT8 Port,
  UINT32 *LtssmState,
  UINT16 *LinkSpeed,
  UINT16 *LinkWidth
  )
{
  LNKSTS_IIO_PCIE_STRUCT     Lnksts;
  REUTPHPIS_IIO_DFX_STRUCT       Reutphpis;
  REUTENGLTRCON_IIO_DFX_STRUCT Reutengltrcon;
  UINT8                          BusBase;
  UINT8                          IioSuperClusterPort;
  UINT8                          IioDfxDevSuperCluster;
  UINT8                          IioDfxFuncSuperCluster;

  IioSuperClusterPort = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].SuperClusterPort;
  IioDfxDevSuperCluster = IioGlobalData->PreLinkData.PcieInfo.PortInfo[IioSuperClusterPort].DfxDevice;
  IioDfxFuncSuperCluster = IioGlobalData->PreLinkData.PcieInfo.PortInfo[IioSuperClusterPort].DfxFunction;
  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Socket];
  Lnksts.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function,
                       ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIE_REG)));

  Reutengltrcon.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDevSuperCluster, IioDfxFuncSuperCluster,
                       ONLY_REGISTER_OFFSET(REUTENGLTRCON_IIO_DFX_REG)));
  Reutengltrcon.Bits.linkselect = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].DfxReutLinkSel;
  IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDevSuperCluster, IioDfxDevSuperCluster,
                       ONLY_REGISTER_OFFSET(REUTENGLTRCON_IIO_DFX_REG)), Reutengltrcon.Data);

  Reutphpis.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDevSuperCluster, IioDfxDevSuperCluster,
                       ONLY_REGISTER_OFFSET(REUTPHPIS_IIO_DFX_REG)));

  *LtssmState = Reutphpis.Bits.ltssmstate;
  *LinkSpeed = Lnksts.Bits.current_link_speed;
  *LinkWidth = Lnksts.Bits.negotiated_link_width;

  return GEN3_SUCCESS;
}

GEN3_STATUS
Gen3PollLinkStatus (
  IIO_GLOBALS   *IioGlobalData,
  UINT8 Socket,
  UINT8 Port,
  UINT32 LtssmState,
  UINT16 LinkSpeed,
  UINT16 LinkWidth,
  UINT32 Timeout,
  UINT32 *OutLtssmState,
  UINT16 *OutLinkSpeed,
  UINT16 *OutLinkWidth
  )
{
  GEN3_STATUS                    Status;
  UINT32                         PollTime;
  UINT32                         CurrentLtssmState;
  UINT16                         CurrentLinkSpeed;
  UINT16                         CurrentLinkWidth;

  CurrentLtssmState = 0;
  CurrentLinkSpeed = 0;
  CurrentLinkWidth = 0;
  PollTime = 0;
  Status = GEN3_FAILURE;

  while ((Status != GEN3_SUCCESS) && ((PollTime <= Timeout) || (Timeout == GEN3_LINK_POLL_INFINITE))) {
    Status = Gen3GetLinkStatus(IioGlobalData, Socket, Port, &CurrentLtssmState, &CurrentLinkSpeed, &CurrentLinkWidth);

    // TODO: implement tight poll.
    if (((LtssmState != 0) && (CurrentLtssmState != LtssmState)) ||
        ((LinkSpeed  != 0) && (CurrentLinkSpeed  != LinkSpeed )) ||
        ((LinkWidth  != 0) && (CurrentLinkWidth  != LinkWidth ))) {
      PollTime += GEN3_LINK_POLL_GRANULARITY;
      IioStall (IioGlobalData, GEN3_LINK_POLL_GRANULARITY);
      Status = GEN3_FAILURE;
    } else {
      Status = GEN3_SUCCESS;
    }
  }

  if ((Status != GEN3_SUCCESS) || ((PollTime > Timeout) && (Timeout != GEN3_LINK_POLL_INFINITE))) {
    Status = GEN3_FAILURE;
  } else {
    Status = GEN3_SUCCESS;
  }

  IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR, "Gen3: Gen3PollLinkStatus: %d (%s), LTSSM=%d, LinkSpeed=%d, LinkWidth=%d\n",
              Status, (Status == GEN3_SUCCESS ? "GEN3_SUCCESS" : "GEN3_FAILURE"), CurrentLtssmState, CurrentLinkSpeed, CurrentLinkWidth);
  
  *OutLtssmState = CurrentLtssmState;
  *OutLinkSpeed = CurrentLinkSpeed;
  *OutLinkWidth = CurrentLinkWidth;

  return Status;
}

IIO_STATUS
Gen3ProgPrivTxEqCoefficients(
  IIO_GLOBALS  *IioGlobalData,
  UINT8 Socket,
  UINT8 Port,
  UINT32 Precursor,
  UINT32 Cursor,
  UINT32 Postcursor
  )
{
  UINT8                                BusBase;
  UINT8                                Ctr;
  UINT8                                ClusterPort;
  UINT8                                Bifurcation;
  UINT32                               RegLocation;
  GEN3PRIVTXEQ_IIO_DFX_STRUCT          Gen3PrivTxEq;
  UINT8   IioDfxDevCtr;
  UINT8   IioDfxFuncCtr;

  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Socket];
  ClusterPort = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].SuperClusterPort;

  Bifurcation = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[ClusterPort].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[ClusterPort].Function, ONLY_REGISTER_OFFSET(PCIE_IOU_BIF_CTRL_IIO_PCIE_REG))) & 0x7;
  
  if (Bifurcation > 4){
    IIO_ASSERT(IioGlobalData, IIO_INVALID_PARAMETER, Bifurcation);
  }
  
  for (Ctr = 0; Ctr < (BifurcationMap[Bifurcation][Port - ClusterPort] / 4); Ctr++) {
    IioDfxDevCtr = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port+Ctr].DfxDevice;
    IioDfxFuncCtr = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port+Ctr].DfxFunction;
    Gen3PrivTxEq.Data = 0;
    Gen3PrivTxEq.Bits.bndl0_g3ptxeq_precursor  = Precursor;
    Gen3PrivTxEq.Bits.bndl0_g3ptxeq_cursor     = Cursor;
    Gen3PrivTxEq.Bits.bndl0_g3ptxeq_postcursor = Postcursor;
    Gen3PrivTxEq.Bits.bndl1_g3ptxeq_precursor  = Precursor;
    Gen3PrivTxEq.Bits.bndl1_g3ptxeq_cursor     = Cursor;
    Gen3PrivTxEq.Bits.bndl1_g3ptxeq_postcursor = Postcursor;
    RegLocation = ONLY_REGISTER_OFFSET(GEN3PRIVTXEQ_IIO_DFX_REG);
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDevCtr, IioDfxFuncCtr, RegLocation), Gen3PrivTxEq.Data);
  }

  return IIO_SUCCESS;
}

IIO_STATUS
Gen3ProgPrivRmtTxEqCoefficients(
  IIO_GLOBALS  *IioGlobalData,
  UINT8 Socket,
  UINT8 Port,
  UINT32 Precursor,
  UINT32 Cursor,
  UINT32 Postcursor
  )
{
  UINT8                                BusBase;
  UINT8                                Ctr;
  UINT8                                ClusterPort;
  UINT8                                Bifurcation;
  UINT32                               RegLocation;
  GEN3PRIVRMTTXEQ_IIO_DFX_STRUCT       Gen3PrivRmtTxEq;
  UINT8   IioDfxDevCtr;
  UINT8   IioDfxFuncCtr;

  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Socket];
  ClusterPort = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].SuperClusterPort;

  Bifurcation = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[ClusterPort].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[ClusterPort].Function, ONLY_REGISTER_OFFSET(PCIE_IOU_BIF_CTRL_IIO_PCIE_REG))) & 0x7;
  if (Bifurcation > 4){
    IIO_ASSERT(IioGlobalData, IIO_INVALID_PARAMETER, Bifurcation);
  }

  for (Ctr = 0; Ctr < (BifurcationMap[Bifurcation][Port - ClusterPort] / 4); Ctr++) {
    IioDfxDevCtr = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port+Ctr].DfxDevice;
    IioDfxFuncCtr = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port+Ctr].DfxFunction;
    Gen3PrivRmtTxEq.Data = 0;
    Gen3PrivRmtTxEq.Bits.bndl0_g3prmttxeq_precursor = Precursor;
    Gen3PrivRmtTxEq.Bits.bndl0_g3prmttxeq_cursor     = Cursor;
    Gen3PrivRmtTxEq.Bits.bndl0_g3prmttxeq_postcursor = Postcursor;
    Gen3PrivRmtTxEq.Bits.bndl1_g3prmttxeq_precursor  = Precursor;
    Gen3PrivRmtTxEq.Bits.bndl1_g3prmttxeq_cursor     = Cursor;
    Gen3PrivRmtTxEq.Bits.bndl1_g3prmttxeq_postcursor = Postcursor;
    RegLocation = ONLY_REGISTER_OFFSET(GEN3PRIVRMTTXEQ_IIO_DFX_REG);
    IioPciExpressWrite32 (IioGlobalData,  PCI_PCIE_ADDR(BusBase, IioDfxDevCtr, IioDfxFuncCtr, RegLocation), Gen3PrivRmtTxEq.Data);
  }

  return IIO_SUCCESS;
}

IIO_STATUS
Gen3ProgTxPh3Coefficients(
  IIO_GLOBALS  *IioGlobalData,
  UINT8 Socket,
  UINT8 Port,
  UINT8 Gen3EqMode
  )
{
  UINT8                                BusBase;
  UINT8                                Ctr;
  UINT8                                ClusterPort;
  UINT8                                Bifurcation;
  UINT32                               RegLocation;
  TX_PH3_CURSOR_IIO_DFX_STRUCT         TxPh3Cursor;
  FSLFVAL_REG_IIO_DFX_STRUCT           FslFval;
  UINT8   IioDfxDevCtr;
  UINT8   IioDfxFuncCtr;

  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Socket];
  ClusterPort = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].SuperClusterPort;

  Bifurcation = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[ClusterPort].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[ClusterPort].Function, ONLY_REGISTER_OFFSET(PCIE_IOU_BIF_CTRL_IIO_PCIE_REG))) & 0x7;
  if (Bifurcation > 4){
    IIO_ASSERT(IioGlobalData, IIO_INVALID_PARAMETER, Bifurcation);
  }

  for (Ctr = 0; Ctr < (BifurcationMap[Bifurcation][Port - ClusterPort] / 4); Ctr++) {
    IioDfxDevCtr = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port+Ctr].DfxDevice;
    IioDfxFuncCtr = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port+Ctr].DfxFunction;

    RegLocation = ONLY_REGISTER_OFFSET(TX_PH3_CURSOR_IIO_DFX_REG);
    TxPh3Cursor.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDevCtr, IioDfxFuncCtr, RegLocation));

    if (Gen3EqMode == GEN3_EQ_MODE_ENABLE_MMM_OFF_WEST) {
      TxPh3Cursor.Bits.b0sub_post_cursor = 6;
      TxPh3Cursor.Bits.b1sub_post_cursor = 6;
    } else if (Gen3EqMode == GEN3_EQ_MODE_ALT_SHORT_CHANNEL) {
      TxPh3Cursor.Bits.b0sub_post_cursor = 4;
      TxPh3Cursor.Bits.b1sub_post_cursor = 4;
      TxPh3Cursor.Bits.b0sub_pre_cursor  = 4;
      TxPh3Cursor.Bits.b1sub_pre_cursor  = 4;
    }

    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDevCtr, IioDfxFuncCtr, RegLocation), TxPh3Cursor.Data);
  }

  IioDfxDevCtr = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].DfxDevice;
  IioDfxFuncCtr = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].DfxFunction;

  RegLocation = ONLY_REGISTER_OFFSET(FSLFVAL_REG_IIO_DFX_REG);
  FslFval.Data = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDevCtr, IioDfxFuncCtr, RegLocation));
  FslFval.Bits.fsthreshold = 39;
  IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDevCtr, IioDfxFuncCtr, RegLocation), FslFval.Data);

  return IIO_SUCCESS;
}

GEN3OVERRIDE_STRUCT Gen3OverrideLinkParamTable[] = {
  // VID, DID, FS/LF valid, FS, LF, Phase2 TxEq valid, {pre, cursor, post}, Phase3 TxEq valid, {pre, cursor, post}
  {0x8086, 0x1D74, 1, 30, 10, 1, {10, 41, 11}, 1, {5, 20, 5}}, // Intel Patsburg
  {0x8086, 0x5375, 1, 30, 10, 1, {11, 41, 11}, 1, {5, 20, 5}}, // Intel Laguna (optimized Ph2 IVT specific, as per 4030555)
  {0x14E4, 0x168E, 1, 31,  3, 1, {10, 41, 11}, 1, {7, 17, 7}}, // Broadcom
};

char *Gen3EqModeStr[] = {
    "Hardware Adaptive",
    "Laguna Optimized",
    "Manual",
    "Lookup Table",
    "Mid@Max Boost Detect",
    "Mid@Max Boost Lookup Table",
    "Test",
    "Emulated PSA",
    "Emulated PSA Detect",
    "NTB"
};

#define TEMP_BUSBASE_OFFSET            1


GEN3_STATUS
Gen3LinkupFlow (
    IIO_GLOBALS                       *IioGlobalData,
    UINT8                             Socket,
    UINT8                             Port,
    UINT8                             TargetLinkSpeed 
    )
{
  UINT8                                BusBase;
  UINT8                                Device;
  UINT8                                Function;
  UINT32                               RegLocation;
  LNKCON2_IIO_PCIE_STRUCT              Lnkcon2;

  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Socket];
  Device = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device;
  Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function;

  if ((TargetLinkSpeed == GEN3_LINK_SPEED_GEN1) || ((TargetLinkSpeed > GEN3_LINK_SPEED_GEN1) &&
      ((IioGlobalData->SetupData.Gen3LinkupFlow[Socket*NUMBER_PORTS_PER_SOCKET + Port] == GEN3_LINKUP_FLOW_G1_G3) ||
       (IioGlobalData->SetupData.Gen3LinkupFlow[Socket*NUMBER_PORTS_PER_SOCKET + Port] == GEN3_LINKUP_FLOW_G1_G2_G3)))) {
    RegLocation = ONLY_REGISTER_OFFSET(LNKCON2_IIO_PCIE_REG);
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Gen3: Training to Gen1... (SKT=%d, PORT=%s(%d)).\n", Socket, IIO_PORT_LABEL(Port), Port );
    Lnkcon2.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, Device, Function, RegLocation));
    Lnkcon2.Bits.target_link_speed = 1;
    IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR(BusBase, Device, Function, RegLocation), Lnkcon2.Data);
    Gen3WaitTillLinkRetrains(IioGlobalData, BusBase, Device, Function);
  }

  if ((TargetLinkSpeed == GEN3_LINK_SPEED_GEN2) || ((TargetLinkSpeed > GEN3_LINK_SPEED_GEN2) && 
      (IioGlobalData->SetupData.Gen3LinkupFlow[Socket*NUMBER_PORTS_PER_SOCKET + Port] == GEN3_LINKUP_FLOW_G1_G2_G3))) {
    RegLocation = ONLY_REGISTER_OFFSET(LNKCON2_IIO_PCIE_REG);
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Gen3: Training to Gen2... (SKT=%d, PORT=%s(%d)).\n", Socket, IIO_PORT_LABEL(Port), Port);
    Lnkcon2.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, Device, Function, RegLocation));
    Lnkcon2.Bits.target_link_speed = 2;
    IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR(BusBase, Device, Function, RegLocation), Lnkcon2.Data);
    Gen3WaitTillLinkRetrains(IioGlobalData, BusBase, Device, Function);
  }

  if ((TargetLinkSpeed == GEN3_LINK_SPEED_GEN3) || ((TargetLinkSpeed > GEN3_LINK_SPEED_GEN3) &&
      ((IioGlobalData->SetupData.Gen3LinkupFlow[Socket*NUMBER_PORTS_PER_SOCKET + Port] == GEN3_LINKUP_FLOW_G1_G3) ||
       (IioGlobalData->SetupData.Gen3LinkupFlow[Socket*NUMBER_PORTS_PER_SOCKET + Port] == GEN3_LINKUP_FLOW_G1_G2_G3)))) {
    RegLocation = ONLY_REGISTER_OFFSET(LNKCON2_IIO_PCIE_REG);
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Gen3: Training to Gen3... (SKT=%d, PORT=%s(%d)).\n", Socket, IIO_PORT_LABEL(Port), Port);
    Lnkcon2.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, Device, Function, RegLocation));
    Lnkcon2.Bits.target_link_speed = 3;
    IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR(BusBase, Device, Function, RegLocation), Lnkcon2.Data);
    Gen3WaitTillLinkRetrains(IioGlobalData, BusBase, Device, Function);
  }

  return GEN3_SUCCESS;
}
 
GEN3_STATUS
Gen3GetEndpointInfo(
    IIO_GLOBALS                      *IioGlobalData,
    UINT8                            Socket,
    UINT8                            Port,
    UINT16                           *Vid, 
    UINT16                           *Did,
    UINT32                           *MaxSpeed,
    UINT32                           *MaxWidth
    )
{
  UINT8                                BusBase;
  UINT32                               RegLocation;
  UINT32                               RegLocation2;
  LNKCAP_IIO_PCIE_STRUCT               LnkCap;
  LNKCAP2_IIO_PCIE_STRUCT              LnkCap2;
  UINT16                               CapId;
  PBUS_IIO_PCIE_STRUCT                 Pbus;
  SECBUS_IIO_PCIE_STRUCT               Secbus;
  SUBBUS_IIO_PCIE_STRUCT               Subbus;
  GEN3_STATUS                          status;
  UINT32                                FailSafeCapListParse = 0;

  IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Gen3: Gen3GetEndpointInfo(SKT=%d, PORT=%s(%d)).\n", Socket, IIO_PORT_LABEL(Port), Port );

  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Socket];

  Pbus.Bits.pbn = BusBase;
  Secbus.Bits.sbn = BusBase + TEMP_BUSBASE_OFFSET;
  Subbus.Bits.subordinate_bus_number = BusBase + TEMP_BUSBASE_OFFSET;
  IioPciExpressWrite8 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, ONLY_REGISTER_OFFSET(PBUS_IIO_PCIE_REG)), Pbus.Bits.pbn);

  IioPciExpressWrite8 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, ONLY_REGISTER_OFFSET(SECBUS_IIO_PCIE_REG)), Secbus.Bits.sbn);

  IioPciExpressWrite8 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, ONLY_REGISTER_OFFSET(SUBBUS_IIO_PCIE_REG)), Subbus.Bits.subordinate_bus_number);

  if(IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, 0)) != 0xFFFF) {
    LnkCap.Data = 0;
    // Compare endpoint max and negotiated link widths.
    CapId = 0;
    RegLocation =  ONLY_REGISTER_OFFSET(CAPPTR_IIO_PCIEDMI_REG);
    RegLocation =  IioPciExpressRead8 ( IioGlobalData, PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation));
    while (RegLocation != 0) {
      if( (RegLocation & 1) == 0 )    //thus making sure that WORD read is not on odd byte boundary
        CapId =  IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation));
      else
        CapId =  (UINT16) ((IioPciExpressRead8 ( IioGlobalData, PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation+1)) << 8) | \
            IioPciExpressRead8 ( IioGlobalData, PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation)) );
      if ((CapId & 0xFF) == GEN3_CAPID_PCIE) {
        break;
      } else {
        RegLocation = (CapId & 0xFF00) >> 8;
      }
      //track how many capid nodes it is parsing if >255 than pci space corrupted
      FailSafeCapListParse++;
      if(RegLocation == 0xFF || FailSafeCapListParse >= 0xFF){
        IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR, "  Invalid capability list pointer!!!\n");
        break;
      }
    }
  }
  else  {
    CapId = 0;
    RegLocation = 0;
  }

  if ((CapId & 0xFF) != GEN3_CAPID_PCIE) {
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR, "Gen3: Can't get CAPID.\n");
    status = GEN3_FAILURE;
//    goto errexit;
  }
  else  {
    RegLocation2 = RegLocation + ONLY_REGISTER_OFFSET(LNKCAP2_IIO_PCIE_REG) - ONLY_REGISTER_OFFSET(PXPCAPID_IIO_PCIE_REG);
    if( (RegLocation2 & 3) == 0)   //making sure it is on DWORD boundary
      LnkCap2.Data =  IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation2));
    else{
      LnkCap2.Data =  IioPciExpressRead8 ( IioGlobalData, PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation2));
      LnkCap2.Data |= IioPciExpressRead8 ( IioGlobalData, PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation2+1)) << 8;
      LnkCap2.Data |= IioPciExpressRead8 ( IioGlobalData, PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation2+2)) << 16;
      LnkCap2.Data |= IioPciExpressRead8 ( IioGlobalData, PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation2+3)) << 24;
    }

    RegLocation = RegLocation + ONLY_REGISTER_OFFSET(LNKCAP_IIO_PCIE_REG) - ONLY_REGISTER_OFFSET(PXPCAPID_IIO_PCIE_REG);
    if( (RegLocation & 3) == 0)   //making sure it is on DWORD boundary
      LnkCap.Data =  IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation));
    else{
      LnkCap.Data =  IioPciExpressRead8 ( IioGlobalData, PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation));
      LnkCap.Data |= IioPciExpressRead8 ( IioGlobalData, PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation+1)) << 8;
      LnkCap.Data |= IioPciExpressRead8 ( IioGlobalData, PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation+2)) << 16;
      LnkCap.Data |= IioPciExpressRead8 ( IioGlobalData, PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation+3)) << 24;
    }

    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Gen3: ");
    if(Vid != NULL) {
      *Vid = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, ONLY_REGISTER_OFFSET(VID_IIO_PCIE_REG)));
      IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, " VID=%04X", *Vid);
    }
    if(Did != NULL) {
      *Did = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, ONLY_REGISTER_OFFSET(DID_IIO_PCIE_REG)));
      IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, " DID=%04X", *Did);
    }
    if(MaxSpeed != NULL)  {
      // Check if the  max lnkspd is coherent with the vector. 
      if (( (1 << (LnkCap.Bits.maxlnkspd-1)) & (LnkCap2.Bits.lnkspdvec)) != 0)
      {
        *MaxSpeed = LnkCap.Bits.maxlnkspd;
      } else {
        *MaxSpeed = 1;
      }
      IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, " MaxLinkSpeed=%d", *MaxSpeed);
    }
    if(MaxWidth != NULL)  {
      *MaxWidth = LnkCap.Bits.maximum_link_width;
      IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, " MaxLinkWidth=%d\n", *MaxWidth);
    }
    status = GEN3_SUCCESS;
  }

  //clean up before exit
  Pbus.Bits.pbn = 0;
  Secbus.Bits.sbn = 0;
  Subbus.Bits.subordinate_bus_number = 0;
  IioPciExpressWrite8 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, ONLY_REGISTER_OFFSET(PBUS_IIO_PCIE_REG)), Pbus.Bits.pbn);

  IioPciExpressWrite8 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, ONLY_REGISTER_OFFSET(SECBUS_IIO_PCIE_REG)), Secbus.Bits.sbn);

  IioPciExpressWrite8 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, ONLY_REGISTER_OFFSET(SUBBUS_IIO_PCIE_REG)), Subbus.Bits.subordinate_bus_number);
  return  status;

//  return GEN3_SUCCESS;

//errexit:
//  return GEN3_FAILURE;
}

GEN3_STATUS
Gen3RetrainLink(
    IIO_GLOBALS                       *IioGlobalData,
    UINT8                             Socket,
    UINT8                             Port
    )
{
  UINT8                                BusBase;
  UINT32                               RegLocation;
  BCTRL_IIO_PCIE_STRUCT            Bctrl;
  LNKCON_IIO_PCIE_STRUCT               Lnkcon;

  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Socket];
  if (IioGlobalData->SetupData.Gen3RetrainType[Socket*NUMBER_PORTS_PER_SOCKET + Port] == GEN3_RETRAIN_TYPE_SBR) {
    RegLocation = ONLY_REGISTER_OFFSET(BCTRL_IIO_PCIE_REG);
    Bctrl.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation));
    Bctrl.Bits.sbr = 1;
    IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation), Bctrl.Data);
    IioStall (IioGlobalData, GEN3_STALL_SBR);
    
    Bctrl.Bits.sbr = 0;
    IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation), Bctrl.Data);

    IioStall (IioGlobalData, IIO_LINK_TRAIN_TIME_MARGIN_G3EQ);
  } else if (IioGlobalData->SetupData.Gen3RetrainType[Socket*NUMBER_PORTS_PER_SOCKET + Port] == GEN3_RETRAIN_TYPE_LINK_DISABLE) {
    RegLocation = ONLY_REGISTER_OFFSET(LNKCON_IIO_PCIE_REG);
    Lnkcon.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation));
    Lnkcon.Bits.link_disable = 1;
    IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation), Lnkcon.Data);
    Lnkcon.Bits.link_disable = 0;
    IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation), Lnkcon.Data);

  } else if (IioGlobalData->SetupData.Gen3RetrainType[Socket*NUMBER_PORTS_PER_SOCKET + Port] == GEN3_RETRAIN_TYPE_PHY_RESET) {
    Gen3PhyReset (IioGlobalData, Socket, Port, GEN3_RESET_PHYRESET_TOGGLE);
  } else if (IioGlobalData->SetupData.Gen3RetrainType[Socket*NUMBER_PORTS_PER_SOCKET + Port] == GEN3_RETRAIN_TYPE_SKIP) {
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Socket:[%d] Port:[%d] GEN3 retrain reset skipped...\n", Socket,Port);
  } else {
    Gen3PhyReset (IioGlobalData, Socket, Port, GEN3_RESET_PHYRESET_TOGGLE);
  }

    return GEN3_SUCCESS;
}

GEN3_STATUS
Gen3Override(
    IIO_GLOBALS                       *IioGlobalData,
    UINT8                             Socket,
    UINT8                             Port,
    UINT8                             Phase2EqMode,
    UINT8                             Phase3EqMode
    )
{
    PARTNER_TXEQ_FSSTAR_IIO_DFX_STRUCT   PartnerTxeqFsstar;
    PXPPRTGEN3EQPRIV_IIO_DFX_STRUCT      PxpPrtGen3EqPriv;
    TXEQMISCSTATUS_IIO_DFX_STRUCT        TxeqMiscStatus;
    UINT8                                BusBase;
    UINT32                               Fs, Lf, TmpFs, TmpLf, cm1, c0, cp1;
    UINT16                               Bifurcation;
    UINT32                               RegLocation;
    UINT32                               Ctr;
    UINT16                               Vid;
    UINT16                               Did;
    UINT32                               MaxSpeed;
    GEN3OVERRIDE_STRUCT                  *OverrideParamPtr = NULL;
    UINT32                               ClusterPortIndex;
    UINT16                               CurrentLinkWidth;
    UINT8                                FsLfValid = FALSE;
    LNKCON2_IIO_PCIE_STRUCT              Lnkcon2;
    BCTRL_IIO_PCIE_STRUCT                Bctrl;
    LNKSTS_IIO_PCIE_STRUCT               Lnksts;
    TX_PH3_CURSOR_IIO_DFX_STRUCT         TxPh3Cursor;
    DFXCHICKENBIT1_IIO_DFX_STRUCT        DfxChicken1;
    UINT8   IioDfxDev;
    UINT8   IioDfxFunc;
    UINT8   IioDfxDevCtr;
    UINT8   IioDfxFuncCtr;
    UINT8                          IioSuperClusterPort;
    UINT8   TxEqWaFound; 
    UINT32  Capid4; 
    BOOLEAN TxEqWaConfig; 
    BOOLEAN HsxC1Stepping; 

    IioSuperClusterPort = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].SuperClusterPort;
    IioDfxDev = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].DfxDevice;
    IioDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].DfxFunction;

    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Gen3: Gen3Override(SKT=%d, PORT=%s(%d), Phase2=%d, Phase3=%d)\n",
               Socket, IIO_PORT_LABEL(Port), Port, Phase2EqMode, Phase3EqMode);      //fix for s4030599

    Fs = 0;
    Lf = 0;
    cm1 = 0;
    c0 = 0;
    cp1 = 0;
    Vid = 0;
    Did = 0;
    MaxSpeed = 0;
    ClusterPortIndex = 0;
    CurrentLinkWidth = 0;
    TxEqWaFound = 0;
    //
    // 4987791: Gen3Override uses wrong bus number  to get cpu stepping
    //
    BusBase = IioGlobalData->IioVData.SocketUncoreBusNumber[Socket];
   
    // Read CPU CAPID4 to try to determine if the CPU is HSX C1. 
    // On HSX C1, CpuStepping is reporting C0_REV_HSX. We need to 
    // check that BIT 29, Bit 7 and BIT 6 on CAPID4 == 110b
    Capid4 = IioPciExpressRead32( IioGlobalData,  PCI_PCIE_ADDR(BusBase, PCIE_PORT_CPU_CAPIDS_DEV, PCIE_PORT_CPU_CAPIDS_FUNC, R_CPU_CAPID4));
    HsxC1Stepping = ((Capid4 & BIT29) != 0 );
    TxEqWaConfig =  ((IioGlobalData->SetupData.TxEq_LookupTable_WA == 1          ) && 
                    (IioGlobalData->IioVData.CpuType              == CPU_HSX    ) && 
                    (IioGlobalData->IioVData.CpuStepping          == C0_REV_HSX ) && 
                    (!HsxC1Stepping));
    
    BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Socket];
    
    Bifurcation = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[IioSuperClusterPort].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[IioSuperClusterPort].Function, ONLY_REGISTER_OFFSET(PCIE_IOU_BIF_CTRL_IIO_PCIE_REG))) & 0x7;
  if (Bifurcation > 4){
    IIO_ASSERT(IioGlobalData, IIO_INVALID_PARAMETER, Bifurcation);
  }

    ClusterPortIndex = Port - IioSuperClusterPort;
    if(ClusterPortIndex >= 0 && ClusterPortIndex < 4) {     //fix for s4031021
      CurrentLinkWidth = (UINT16)BifurcationMap[Bifurcation][ClusterPortIndex];
      if(CurrentLinkWidth > 16 || CurrentLinkWidth == 0)
        return GEN3_FAILURE;
    }
    else
      return GEN3_FAILURE;

    if ((Phase2EqMode    == GEN3_EQ_MODE_LOOKUP_TABLE)
        || (Phase3EqMode == GEN3_EQ_MODE_LOOKUP_TABLE)
        || (Phase3EqMode == GEN3_EQ_MODE_MAXBOOST_LOOKUP_TABLE) 
        // 
        // HSX s4905948 - Use the lookup table for all the updated remote devices. 
        //
        || TxEqWaConfig ) {
      Gen3LinkupFlow(IioGlobalData,  Socket, Port, GEN3_LINK_SPEED_GEN2);
      Gen3GetEndpointInfo(IioGlobalData, Socket, Port, &Vid, &Did, &MaxSpeed, NULL);

      for(Ctr = 0; Ctr < sizeof(Gen3OverrideTxEqParamTable)/sizeof(GEN3OVERRIDE_STRUCT); Ctr++) {
        if (Gen3OverrideTxEqParamTable[Ctr].Vid== Vid && Gen3OverrideTxEqParamTable[Ctr].Did == Did &&
           ((Gen3OverrideTxEqParamTable[Ctr].Platform == 0xFF) || (Gen3OverrideTxEqParamTable[Ctr].Platform == IioGlobalData->IioVData.PlatformType)) &&
           ((Gen3OverrideTxEqParamTable[Ctr].Socket == 0xFF)     || ((Gen3OverrideTxEqParamTable[Ctr].Socket & (1 << Socket)) != 0)) &&
           ((Gen3OverrideTxEqParamTable[Ctr].Port == 0xFFFF)     || ((Gen3OverrideTxEqParamTable[Ctr].Port & (1 << Port)) != 0)) ) {
          OverrideParamPtr = (GEN3OVERRIDE_STRUCT *)&Gen3OverrideTxEqParamTable[Ctr];
          IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO,  "Gen3: Found overrides for VID=%04X, DID=%04X. Using Socket=%x;Port=%x;Platform=%x;",
              Vid, Did,OverrideParamPtr->PortParams.Socket, OverrideParamPtr->PortParams.Port, OverrideParamPtr->PortParams.Platform);
          IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO,  "UPTX=(%d,%d,%d);", OverrideParamPtr->PortParams.UpstreamTxEq.Precursor,
              OverrideParamPtr->PortParams.UpstreamTxEq.Cursor, OverrideParamPtr->PortParams.UpstreamTxEq.Postcursor);
          IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO,  "DNTX=(%d,%d,%d).\n", OverrideParamPtr->PortParams.DownstreamTxEq.Precursor,
              OverrideParamPtr->PortParams.DownstreamTxEq.Cursor, OverrideParamPtr->PortParams.DownstreamTxEq.Postcursor);
          break;
        }
      }

      if (OverrideParamPtr == NULL) {
        for(Ctr = 0; Ctr < sizeof(Gen3OverrideLinkParamTable)/sizeof(GEN3OVERRIDE_STRUCT); Ctr++) {
          if (Gen3OverrideLinkParamTable[Ctr].LinkParams.Vid == Vid && Gen3OverrideLinkParamTable[Ctr].LinkParams.Did == Did) {
          OverrideParamPtr = &Gen3OverrideLinkParamTable[Ctr];
          IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO,  "Gen3: Found overrides for VID=%04X, DID=%04X. Using FS=%d;LF=%d;UPTX=(%d,%d,%d);DNTX=(%d,%d,%d).\n", Vid, Did,
                        OverrideParamPtr->LinkParams.Fs, OverrideParamPtr->LinkParams.Lf,
                        OverrideParamPtr->LinkParams.UpstreamTxEq.Precursor, OverrideParamPtr->LinkParams.UpstreamTxEq.Cursor, OverrideParamPtr->LinkParams.UpstreamTxEq.Postcursor,
                        OverrideParamPtr->LinkParams.DownstreamTxEq.Precursor, OverrideParamPtr->LinkParams.DownstreamTxEq.Cursor, OverrideParamPtr->LinkParams.DownstreamTxEq.Postcursor);
          break;
        }
      }
      }

      if (!OverrideParamPtr) {
        IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR, "Gen3: Card (VID=%04X, DID=%04X) not found in lookup table. Proceeding\n", Vid, Did);
        goto errexit;
      }
      //
      // HSX s4905948 - Use the lookup table for all the updated remote devices. 
      //
      if ((Phase2EqMode != GEN3_EQ_MODE_LOOKUP_TABLE) && 
          (OverrideParamPtr->LinkParams.UpstreamTxEqValid) &&
           TxEqWaConfig ) {
        // If the table shows an entry for the current value, then we will use it. 
        TxEqWaFound = 1; 
      }

      if ((Phase2EqMode == GEN3_EQ_MODE_LOOKUP_TABLE && !OverrideParamPtr->LinkParams.UpstreamTxEqValid)
          || (Phase3EqMode == GEN3_EQ_MODE_LOOKUP_TABLE && !OverrideParamPtr->LinkParams.DownstreamTxEqValid)
          || (Phase3EqMode == GEN3_EQ_MODE_MAXBOOST_LOOKUP_TABLE && !OverrideParamPtr->LinkParams.FsLfValid)) {
        IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR, "Gen3: FATAL_ERROR: Lookup table info invalid for requested mode.\n");
        goto errexit;
      }
      RegLocation = ONLY_REGISTER_OFFSET(BCTRL_IIO_PCIE_REG);
      Bctrl.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation));
      Bctrl.Bits.sbr = 1;
      IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation), Bctrl.Data);
      IioStall (IioGlobalData, GEN3_STALL_SBR);
    } 

    if ((Phase2EqMode == GEN3_EQ_MODE_LOOKUP_TABLE)
        // 
        // HSX s4905948 - Use the lookup table for all the updated remote devices. 
        //
        || (TxEqWaFound == 1)) {
      RegLocation = ONLY_REGISTER_OFFSET(PXPPRTGEN3EQPRIV_IIO_DFX_REG);
      PxpPrtGen3EqPriv.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      PxpPrtGen3EqPriv.Bits.use_g3eqprivatecsrvalues = 1;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), PxpPrtGen3EqPriv.Data);

      Gen3ProgPrivTxEqCoefficients(IioGlobalData,Socket,Port,
                                    OverrideParamPtr->LinkParams.UpstreamTxEq.Precursor,
                                    OverrideParamPtr->LinkParams.UpstreamTxEq.Cursor,
                                    OverrideParamPtr->LinkParams.UpstreamTxEq.Postcursor);
    }

    // Setup for Gen3.
    RegLocation = ONLY_REGISTER_OFFSET(PXPPRTGEN3EQPRIV_IIO_DFX_REG);
    PxpPrtGen3EqPriv.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation));
    PxpPrtGen3EqPriv.Bits.en_july0_7spec = 0;
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation), PxpPrtGen3EqPriv.Data);

    if (Phase3EqMode == GEN3_EQ_MODE_MAXBOOST_SW) {
      RegLocation = ONLY_REGISTER_OFFSET(PXPPRTGEN3EQPRIV_IIO_DFX_REG);
      PxpPrtGen3EqPriv.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      PxpPrtGen3EqPriv.Bits.dis_eqph23 = 1;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation), PxpPrtGen3EqPriv.Data);

      IioGlobalData->SetupData.Gen3LinkupFlow[Socket*NUMBER_PORTS_PER_SOCKET + Port] = GEN3_LINKUP_FLOW_G1_G2_G3;
      Gen3LinkupFlow(IioGlobalData, Socket, Port, GEN3_LINK_SPEED_GEN3);

      Lnksts.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function,
                               ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIE_REG)));

      if ((Lnksts.Bits.current_link_speed != GEN3_LINK_SPEED_GEN3) || (Lnksts.Bits.data_link_layer_link_active != 1)) {
        IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR,  "Gen3: DL_UP: %d, LinkSpeed=%d.\n", Lnksts.Bits.data_link_layer_link_active, Lnksts.Bits.current_link_speed);
        FsLfValid = FALSE;
      } else {
        for (Ctr = 0; Ctr < CurrentLinkWidth; Ctr++) {
          IioDfxDevCtr = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port+(Ctr/4)].DfxDevice;
          IioDfxFuncCtr = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port+(Ctr/4)].DfxFunction;
          // 3614973: PCIe Phase 3 override FS/LF robustness enhancement
          // 3615473: Need to implement PCIe Phase 3 override FS/LF handling for reversal case
          PartnerTxeqFsstar.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDevCtr, IioDfxFuncCtr, ONLY_REGISTER_OFFSET(PARTNER_TXEQ_FSSTAR_IIO_DFX_REG)));
          TxeqMiscStatus.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDevCtr, IioDfxFuncCtr, ONLY_REGISTER_OFFSET(TXEQMISCSTATUS_IIO_DFX_REG)));
          TmpFs = ((PartnerTxeqFsstar.Bits.patnerx4_txeq_fsstart >> (6 * (Ctr % 4))) & 0x3F);
          TmpLf = ((TxeqMiscStatus.Data >> (6 * (Ctr % 4))) & 0x3F);
          if (TmpFs == 0 || TmpLf == 0) continue;
          if (Fs == 0 || Lf == 0) {
            Fs = TmpFs;
            Lf = TmpLf;
          }

          if (TmpFs != Fs || TmpLf != Lf) {
            IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR, "Gen3: FS/LF MISMATCH. FS/LF=%d/%d, Mismatch (lane=%d) FS/LF=%d/%d\n", Fs, Lf, Ctr, TmpFs, TmpLf);
            IioWarmReset(IioGlobalData);
          }
        }
        if ((Fs != 0) && (Lf != 0)) {
          FsLfValid = TRUE;
          IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Gen3: Detected FS=%d, LF=%d\n", Fs, Lf);

          RegLocation = ONLY_REGISTER_OFFSET(PXPPRTGEN3EQPRIV_IIO_DFX_REG);
          PxpPrtGen3EqPriv.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation));
          PxpPrtGen3EqPriv.Bits.dis_eqph23 = 0;
          IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation), PxpPrtGen3EqPriv.Data);
        } else {
          FsLfValid = FALSE;
        }
      }
    }

    if (Phase3EqMode == GEN3_EQ_MODE_LOOKUP_TABLE) {
      RegLocation = ONLY_REGISTER_OFFSET(PXPPRTGEN3EQPRIV_IIO_DFX_REG);
      PxpPrtGen3EqPriv.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      PxpPrtGen3EqPriv.Bits.use_g3eqprivcsr4adaptsm = 1;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), PxpPrtGen3EqPriv.Data);

      Gen3ProgPrivRmtTxEqCoefficients(IioGlobalData, Socket, Port,
                                  OverrideParamPtr->LinkParams.DownstreamTxEq.Precursor,
                                  OverrideParamPtr->LinkParams.DownstreamTxEq.Cursor,
                                  OverrideParamPtr->LinkParams.DownstreamTxEq.Postcursor);
    } else if (Phase3EqMode == GEN3_EQ_MODE_MAXBOOST_LOOKUP_TABLE) {
      RegLocation = ONLY_REGISTER_OFFSET(PXPPRTGEN3EQPRIV_IIO_DFX_REG);
      PxpPrtGen3EqPriv.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      PxpPrtGen3EqPriv.Bits.use_g3eqprivcsr4adaptsm = 1;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), PxpPrtGen3EqPriv.Data);
      cm1 = (OverrideParamPtr->LinkParams.Fs - OverrideParamPtr->LinkParams.Lf) / 4;
      c0 = (OverrideParamPtr->LinkParams.Fs + OverrideParamPtr->LinkParams.Lf) / 2;
      cp1 = cm1;
      // handle roundoff error.
      if ((cm1 + c0 + cp1) < Fs) {
        cm1 += 1;
      }
      if ((cm1 + c0 + cp1) == (Fs - 1)) {
        c0++;
      }
      if ((c0 - cm1 - cp1) < Lf) {
        c0++;
        cm1--;
      }

      Gen3ProgPrivRmtTxEqCoefficients(IioGlobalData,Socket,Port, cm1, c0, cp1);
    } else if (Phase3EqMode == GEN3_EQ_MODE_MAXBOOST_SW && FsLfValid == TRUE) {
      RegLocation = ONLY_REGISTER_OFFSET(PXPPRTGEN3EQPRIV_IIO_DFX_REG);
      PxpPrtGen3EqPriv.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      PxpPrtGen3EqPriv.Bits.use_g3eqprivcsr4adaptsm = 1;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), PxpPrtGen3EqPriv.Data);
      if ((IioGlobalData->IioVData.CpuType == CPU_IVT) || (IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
        RegLocation = ONLY_REGISTER_OFFSET(TX_PH3_CURSOR_IIO_DFX_REG);
        TxPh3Cursor.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
        TxPh3Cursor.Bits.bypass_endcard_coeff = 0;
        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), TxPh3Cursor.Data);

        if ((Port == 0) || (Port == 1) || (Port == 3) || (Port == 7)) { 
          RegLocation = ONLY_REGISTER_OFFSET(DFXCHICKENBIT1_IIO_DFX_REG);
          DfxChicken1.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
          DfxChicken1.Bits.dis_mid_max_cal = 1;
          DfxChicken1.Bits.en_byp_ph3_with_neq0 = 0;
          IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), DfxChicken1.Data);
        } // end if ((Port == 0) || (Port == 1) || (Port == 3) || (Port == 7)) { 
      }
      cm1 = (Fs - Lf) / 4;
      c0 = (Fs + Lf) / 2;
      cp1 = cm1;

      // handle roundoff error.
      if ((cm1 + c0 + cp1) < Fs) {
        cm1 += 1;
      }
      if ((cm1 + c0 + cp1) == (Fs - 1)) {
        c0++;
      }
      if ((c0 - cm1 - cp1) < Lf) {
        c0++;
        cm1--;
      }

      IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Gen3: Using Phase3 Override (%d,%d,%d)\n", cm1, c0, cp1);
      Gen3ProgPrivRmtTxEqCoefficients(IioGlobalData,Socket,Port, cm1, c0, cp1);
      Gen3RetrainLink(IioGlobalData, Socket, Port);
      return GEN3_SUCCESS;
    }

    if ((Phase2EqMode == GEN3_EQ_MODE_LOOKUP_TABLE) ||
        (Phase3EqMode == GEN3_EQ_MODE_LOOKUP_TABLE) ||
        (Phase3EqMode == GEN3_EQ_MODE_MAXBOOST_LOOKUP_TABLE) /*||
        (Phase3EqMode == GEN3_EQ_MODE_EMULATED_PSA_DETECT)*/
        // 
        // HSX s4905948 - Use the lookup table for all the updated remote devices. 
        //
        || (TxEqWaFound == 1) )  {
      RegLocation = ONLY_REGISTER_OFFSET(LNKCON2_IIO_PCIE_REG);
      Lnkcon2.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation));
      Lnkcon2.Bits.target_link_speed = GEN3_LINK_SPEED_GEN3;
      IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation), Lnkcon2.Data);

      RegLocation = ONLY_REGISTER_OFFSET(BCTRL_IIO_PCIE_REG);
      Bctrl.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation));
      Bctrl.Bits.sbr = 0;
      IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation), Bctrl.Data);
      IioStall (IioGlobalData, IIO_LINK_TRAIN_TIME_MARGIN_G3EQ);
    } else {
      // Temporarily restore this retrain to unblock boot with certain PCIE devices.
      Gen3RetrainLink(IioGlobalData, Socket, Port);
      //
      // HSD 4166644: ESCALATE from hexaii_hsx_mock:PCIE: GEN2 De-emphasis is not being set correctly in PO BIOS
      //
      // Gen3LinkupFlow(IioGlobalData, Socket, Port, GEN3_LINK_SPEED_GEN3);
      //
    }
    //
    // As per the review with PCIe design, the IIO's PCIe Gen3 capable ports by default
    // will train to Gen3 mode and hence their is no point in forcing to additional
    // link retrains.
    //

errexit:
    if ((TxEqWaFound == 0) && (TxEqWaConfig)) { 
      // The card was not on the table and the WA is not needed.
      // the card is currently at Gen2.  We  need to reset the link. 
      Gen3LinkupFlow(IioGlobalData, Socket, Port, GEN3_LINK_SPEED_GEN3);
    }
    return GEN3_SUCCESS;
}

GEN3_STATUS
Gen3PhyReset (
  IIO_GLOBALS                       *IioGlobalData,
  UINT8                             Socket,
  UINT8                             Port,
  UINT8                             ResetType 
  )
{
  UINT8 BusBase;
  UINT32 RegLocation;
  REUTENGLTRON_IIO_DFX_STRUCT      Reutengltron;
  REUTENGLTRCON_IIO_DFX_STRUCT Reutengltrcon;
  UINT8 DFXDevice, DFXFunction;
  UINT8                          IioSuperClusterPort;

  IioSuperClusterPort = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].SuperClusterPort;
  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Socket];
  DFXDevice = IioGlobalData->PreLinkData.PcieInfo.PortInfo[IioSuperClusterPort].DfxDevice;
  DFXFunction = IioGlobalData->PreLinkData.PcieInfo.PortInfo[IioSuperClusterPort].DfxFunction;

  Reutengltrcon.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, DFXDevice, DFXFunction,
                       ONLY_REGISTER_OFFSET(REUTENGLTRCON_IIO_DFX_REG)));
  Reutengltrcon.Bits.linkselect = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].DfxReutLinkSel;
  IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, DFXDevice, DFXFunction,
                       ONLY_REGISTER_OFFSET(REUTENGLTRCON_IIO_DFX_REG)), Reutengltrcon.Data);
  RegLocation = ONLY_REGISTER_OFFSET(REUTENGLTRON_IIO_DFX_REG);
  Reutengltron.Data = IioPciExpressRead32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, DFXDevice, DFXFunction, RegLocation));

  if (ResetType == GEN3_RESET_PHYRESET_ENTER || ResetType == GEN3_RESET_PHYRESET_TOGGLE) {
    Reutengltron.Bits.phyreset = 1;
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, DFXDevice, DFXFunction, RegLocation), Reutengltron.Data);
  }
  if(ResetType == GEN3_RESET_PHYRESET_TOGGLE)
    IioStall (IioGlobalData, GEN3_STALL_SBR);

  if (ResetType == GEN3_RESET_PHYRESET_EXIT || ResetType == GEN3_RESET_PHYRESET_TOGGLE) {
    Reutengltron.Bits.phyreset = 0;
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, DFXDevice, DFXFunction, RegLocation), Reutengltron.Data);
  }
  IioStall (IioGlobalData, GEN3_STALL_SBR);   //no harm in giving additonal delay

  return GEN3_SUCCESS;
}

GEN3_STATUS
Gen3ForceLinkDisable (
  IIO_GLOBALS                       *IioGlobalData,
  UINT8                             Socket,
  UINT8                             Port
  )
{
  UINT8 BusBase;
  UINT32 RegLocation;
  LNKCON_IIO_PCIE_STRUCT              Lnkcon;

  // Disable the link.
  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Socket];

  RegLocation = ONLY_REGISTER_OFFSET(LNKCON_IIO_PCIE_REG);
  Lnkcon.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation));
  Lnkcon.Bits.link_disable = 1;
  IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation), Lnkcon.Data);

  Gen3PhyReset (IioGlobalData, Socket, Port, GEN3_RESET_PHYRESET_TOGGLE);

  return GEN3_SUCCESS;
}

GEN3_STATUS
Gen3ForceGen1 (
  IIO_GLOBALS                       *IioGlobalData,
  UINT8                             Socket,
  UINT8                             Port
  )
{
  LNKCON_IIO_PCIE_STRUCT              Lnkcon;
  LNKCON2_IIO_PCIE_STRUCT             Lnkcon2;
  UINT8 BusBase;
  UINT32 RegLocation;

  // Disable the link.
  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Socket];

  Gen3ForceLinkDisable (IioGlobalData,Socket,Port);

  RegLocation = ONLY_REGISTER_OFFSET(LNKCON2_IIO_PCIE_REG);
  Lnkcon2.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation));
  Lnkcon2.Bits.target_link_speed = GEN3_LINK_SPEED_GEN1;
  IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation), Lnkcon2.Data);

  RegLocation = ONLY_REGISTER_OFFSET(LNKCON_IIO_PCIE_REG);
  Lnkcon.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation));
  Lnkcon.Bits.link_disable = 0;
  IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation), Lnkcon.Data);

  return GEN3_SUCCESS;
}



VOID
CheckGen3Override (
    IIO_GLOBALS        *IioGlobalData,
    UINT8 Socket
)
{
  IIO_STATUS          Status;
  UINT8               BusBase;
  UINT8               i, j;
  UINT8               IioDfxDev;
  UINT8               IioDfxFunc;

  i = Socket;

  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[i];

  for (j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {

    IioDfxDev = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxDevice;
    IioDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxFunction;

    if ((IioGlobalData->IioOutData.PciePortPresent[(i*NUMBER_PORTS_PER_SOCKET)+j] == 0 ) ||
        (IioGlobalData->IioOutData.PciePortConfig[(i*NUMBER_PORTS_PER_SOCKET)+j] == 0 ) ||
        (j == NONE_GEN3_PCIE_PORT))  continue;

    //if port is set to disable than skip
    if(CheckVarPortEnable(IioGlobalData, i, j) == FALSE)
      continue;

    // Skip IOSF port
    if(OemIdentifyVendorSpecificPort(IioGlobalData, BusBase, IioDfxDev, IioDfxFunc, TRUE) == TRUE)
      continue;

    Status = Gen3Override(IioGlobalData, i, j,
                              IioGlobalData->SetupData.Gen3Phase2EqMode[(i*NUMBER_PORTS_PER_SOCKET) + j],
                              IioGlobalData->SetupData.Gen3Phase3EqMode[(i*NUMBER_PORTS_PER_SOCKET) + j]);
    IioStall (IioGlobalData, UNIPHY_RECIPE_DELAY);   //Intended fix for s4030382: give a delay of 180us after Gen3 override
    

  }
  
  return;
}


GEN3_STATUS
Gen3PrelinkOverride(
    IIO_GLOBALS                       *IioGlobalData,
    UINT8                             Socket,
    UINT8                             Port,
    UINT8                             Phase2EqMode,
    UINT8                             Phase3EqMode
    )
{
    GEN3_STATUS                          Status;
    PXPPRTGEN3EQPRIV_IIO_DFX_STRUCT    PxpPrtGen3EqPriv;
    TX_PH3_CURSOR_IIO_DFX_STRUCT         TxPh3Cursor;
    DFXCHICKENBIT1_IIO_DFX_STRUCT        DfxChickenBit1;
    UINT8                                BusBase;
    UINT32                               RegLocation;
    UINT8                                IioDfxDev;
    UINT8                                IioDfxFunc;

    Status = GEN3_SUCCESS;
    BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Socket];
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO,"Gen3: Gen3PrelinkOverride(SKT=%d, PORT=%s(%d), Phase2=%d, Phase3=%d)\n",
        Socket, IIO_PORT_LABEL(Port), Port, Phase2EqMode, Phase3EqMode);   //fix for s4030599
    IioDfxDev = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].DfxDevice;
    IioDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].DfxFunction;

    if (Phase2EqMode == GEN3_EQ_MODE_MANUAL) {
      RegLocation = ONLY_REGISTER_OFFSET(PXPPRTGEN3EQPRIV_IIO_DFX_REG);
      PxpPrtGen3EqPriv.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      PxpPrtGen3EqPriv.Bits.use_g3eqprivatecsrvalues = 1;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), PxpPrtGen3EqPriv.Data);


      Gen3ProgPrivTxEqCoefficients(IioGlobalData,Socket,Port,
                                   IioGlobalData->SetupData.Phase2PrivTxEqOvrPreCursor[Socket * NUMBER_PORTS_PER_SOCKET + Port],
                                   IioGlobalData->SetupData.Phase2PrivTxEqOvrCursor[Socket * NUMBER_PORTS_PER_SOCKET + Port],
                                   IioGlobalData->SetupData.Phase2PrivTxEqOvrPostCursor[Socket * NUMBER_PORTS_PER_SOCKET + Port]);
    } else if (Phase2EqMode == GEN3_EQ_MODE_LAGUNA){
      RegLocation = ONLY_REGISTER_OFFSET(PXPPRTGEN3EQPRIV_IIO_DFX_REG);
      PxpPrtGen3EqPriv.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      PxpPrtGen3EqPriv.Bits.use_g3eqprivatecsrvalues = 1;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), PxpPrtGen3EqPriv.Data);

	    //
	    // As per the BIOS Enhancement request (4030555): Laguna optimized BIOS options for IVT {11, 41, 11}
	    //
      Gen3ProgPrivTxEqCoefficients(IioGlobalData,Socket,Port,
                                   11,									//precursor
                                   41,									//cursor
                                   11);									//postcursor
    } else if (Phase2EqMode == GEN3_EQ_MODE_NTB) {
      //
      // s3613973: CLONE from jaketown: CLONE from jaketown: PCIE: issues enabling Adaptive EQ in NTB mode (When send IFC is disabled in Root and End point(NTB))
      // s3615728: CLONE from jaketown: PCIE: GEN3/EQ. link unexpectedly goes through EQRECOVERY.PH0 during HotReset test in NTB mode
      //
      RegLocation = ONLY_REGISTER_OFFSET(PXPPRTGEN3EQPRIV_IIO_DFX_REG);
      PxpPrtGen3EqPriv.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      PxpPrtGen3EqPriv.Bits.dis_mask_p2l_lnkup4eq = 1;        // Common for IVT and HSX (HSD 3613973)
      //The following WA is not required for IVT
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), PxpPrtGen3EqPriv.Data);
    }

    if (Phase3EqMode == GEN3_EQ_MODE_MANUAL) {
      RegLocation = ONLY_REGISTER_OFFSET(PXPPRTGEN3EQPRIV_IIO_DFX_REG);
      PxpPrtGen3EqPriv.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      PxpPrtGen3EqPriv.Bits.use_g3eqprivcsr4adaptsm = 1;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), PxpPrtGen3EqPriv.Data);
      //
      // 5003001 : Need pxpd0xfx_tx_ph3_cursor.bypass_endcard_coeff = 0 when PCIe Gen 3 Phase 3 Mode is set to Manual in BIOS
      //
      if (IioGlobalData->IioVData.CpuType == CPU_BDX) {
        RegLocation = ONLY_REGISTER_OFFSET(TX_PH3_CURSOR_IIO_DFX_REG);
        TxPh3Cursor.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
        TxPh3Cursor.Bits.bypass_endcard_coeff = 0;
        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), TxPh3Cursor.Data);
      }
      Gen3ProgPrivRmtTxEqCoefficients(IioGlobalData,Socket,Port,
                                   IioGlobalData->SetupData.Phase3PrivTxEqOvrPreCursor[Socket * NUMBER_PORTS_PER_SOCKET + Port],
                                   IioGlobalData->SetupData.Phase3PrivTxEqOvrCursor[Socket * NUMBER_PORTS_PER_SOCKET + Port],
                                   IioGlobalData->SetupData.Phase3PrivTxEqOvrPostCursor[Socket * NUMBER_PORTS_PER_SOCKET + Port]);
    }
    //
    //  4030371: Support for new IVT EQ Phase3  override
    //
    if ((Phase3EqMode == GEN3_EQ_MODE_MAXBOOST_HW) && ((IioGlobalData->IioVData.CpuType == CPU_IVT) || (IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX))) {
      RegLocation = ONLY_REGISTER_OFFSET(TX_PH3_CURSOR_IIO_DFX_REG);
      TxPh3Cursor.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      TxPh3Cursor.Bits.bypass_endcard_coeff = 1;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), TxPh3Cursor.Data);

      RegLocation = ONLY_REGISTER_OFFSET(PXPPRTGEN3EQPRIV_IIO_DFX_REG);
      PxpPrtGen3EqPriv.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      PxpPrtGen3EqPriv.Bits.use_g3eqprivcsr4adaptsm = 1;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), PxpPrtGen3EqPriv.Data);

	  if ((Port == 0) || (Port == 1) || (Port == 3) || (Port == 7)) { 
        RegLocation = ONLY_REGISTER_OFFSET(DFXCHICKENBIT1_IIO_DFX_REG);
        DfxChickenBit1.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
        DfxChickenBit1.Bits.dis_mid_max_cal = 0;
        DfxChickenBit1.Bits.en_byp_ph3_with_neq0 = 0;
        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), DfxChickenBit1.Data);
	  }
    }
    else if ((Phase3EqMode == GEN3_EQ_MODE_MAXBOOST_HWADAPT) && ((IioGlobalData->IioVData.CpuType == CPU_IVT) || (IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX))) {
      RegLocation = ONLY_REGISTER_OFFSET(TX_PH3_CURSOR_IIO_DFX_REG);
      TxPh3Cursor.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      TxPh3Cursor.Bits.bypass_endcard_coeff = 1;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), TxPh3Cursor.Data);

      RegLocation = ONLY_REGISTER_OFFSET(PXPPRTGEN3EQPRIV_IIO_DFX_REG);
      PxpPrtGen3EqPriv.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      PxpPrtGen3EqPriv.Bits.use_g3eqprivcsr4adaptsm = 0;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), PxpPrtGen3EqPriv.Data);

 	  if ((Port == 0) || (Port == 1) || (Port == 3) || (Port == 7)) { 
        RegLocation = ONLY_REGISTER_OFFSET(DFXCHICKENBIT1_IIO_DFX_REG);
        DfxChickenBit1.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
        DfxChickenBit1.Bits.dis_mid_max_cal = 0;
        DfxChickenBit1.Bits.en_byp_ph3_with_neq0 = 0;
        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), DfxChickenBit1.Data);
      }
    }
    else if ((Phase3EqMode == GEN3_EQ_MODE_MAXBOOST_BYPASS) && ((IioGlobalData->IioVData.CpuType == CPU_IVT) || (IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX))) {
      RegLocation = ONLY_REGISTER_OFFSET(TX_PH3_CURSOR_IIO_DFX_REG);
      TxPh3Cursor.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
      TxPh3Cursor.Bits.bypass_endcard_coeff = 0;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), TxPh3Cursor.Data);

      RegLocation = ONLY_REGISTER_OFFSET(PXPPRTGEN3EQPRIV_IIO_DFX_REG);
      PxpPrtGen3EqPriv.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc,RegLocation));
      PxpPrtGen3EqPriv.Bits.use_g3eqprivcsr4adaptsm = 1;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), PxpPrtGen3EqPriv.Data);

	  if ((Port == 0) || (Port == 1) || (Port == 3) || (Port == 7)) { 
        RegLocation = ONLY_REGISTER_OFFSET(DFXCHICKENBIT1_IIO_DFX_REG);
        DfxChickenBit1.Data = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
        DfxChickenBit1.Bits.dis_mid_max_cal = 1;
        DfxChickenBit1.Bits.en_byp_ph3_with_neq0 = 1;
        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), DfxChickenBit1.Data);
      }
    }
    return Status;
}

VOID
CheckGen3PreLinkOverride (
    IIO_GLOBALS        *IioGlobalData,
    UINT8 Socket
)
{
  IIO_STATUS          Status;
  UINT8               BusBase;
  UINT8               i, j;
  UINT8               IioDfxDev;
  UINT8               IioDfxFunc;

  i = Socket;

  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[i];

  for (j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {

    IioDfxDev = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxDevice;
    IioDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxFunction;

    if ((IioGlobalData->IioOutData.PciePortPresent[(i*NUMBER_PORTS_PER_SOCKET)+j] == 0 ) ||
        (IioGlobalData->IioOutData.PciePortConfig[(i*NUMBER_PORTS_PER_SOCKET)+j] == 0 )  ||
        (j == NONE_GEN3_PCIE_PORT))  continue;

    //if port is set to disable than skip
    if(CheckVarPortEnable(IioGlobalData, i, j) == FALSE)
      continue;

    // Skip IOSF port
    if(OemIdentifyVendorSpecificPort(IioGlobalData, BusBase, IioDfxDev, IioDfxFunc, TRUE) == TRUE)
      continue;

    Status = Gen3PrelinkOverride(IioGlobalData, i, j,
                              IioGlobalData->SetupData.Gen3Phase2EqMode[(i*NUMBER_PORTS_PER_SOCKET) + j],
                              IioGlobalData->SetupData.Gen3Phase3EqMode[(i*NUMBER_PORTS_PER_SOCKET) + j]);
    IioStall (IioGlobalData, UNIPHY_RECIPE_DELAY);   //Intended fix for s4030382: give a delay of 180us after Pre-link Gen3 override

  }

  return;
}

VOID
MiscWA_PreLinkData(
    IIO_GLOBALS                       *IioGlobalData,
    UINT8                             Socket,
    UINT8                             Port
)
{
  //
  //DE confirms HW fix, not required for IVT, but following the below....
  //  s3247316:CLONE from jaketown: PCIE: Disabled Uplink causes Port 3
  //                      not to train after warm reset
  // (rml s3614874: Lose Slot 5 PCIe during warm reset (on Meridian) if the Uplink is disabled
  // Ensure phyreset is not asserted as this is a sticky register.
  Gen3PhyReset(IioGlobalData, Socket, Port, GEN3_RESET_PHYRESET_EXIT);

}

/*
 * Generic PCIe helper routines are defined below
 */

VOID
GetLinkStatus(
    IIO_GLOBALS                   *IioGlobalData,
    PCI_ROOT_BRIDGE_PCI_ADDRESS   PciAddress,
    LNKSTS_IIO_PCIE_STRUCT        *Lnksts
    )
{
  UINT8 bus = PciAddress.Bus;
  UINT8 device = PciAddress.Device;
  UINT8 func = PciAddress.Function;

  Lnksts->Data = (UINT16)IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(bus, device, func,
                    ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIE_REG)));
}

BOOLEAN
IsPortDisabled(
    IIO_GLOBALS                    *IioGlobalData,
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
    )
{
  LNKCON_IIO_PCIE_STRUCT   LnkCon;
  UINT32  RegLocation = ONLY_REGISTER_OFFSET(LNKCON_IIO_PCIE_REG);
  UINT8   bus = PciAddress.Bus;
  UINT8   device = PciAddress.Device;
  UINT8   func = PciAddress.Function;

  LnkCon.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(bus, device, func, RegLocation));
  if(LnkCon.Bits.link_disable)
    return TRUE;
  else
    return FALSE;
}

VOID
EnableDisablePCIePort(
    IIO_GLOBALS                                     *IioGlobalData,
    PCI_ROOT_BRIDGE_PCI_ADDRESS     PciAddress,
    BOOLEAN                                         PortEn
    )
{
  LNKCON_IIO_PCIE_STRUCT   LnkCon;
  UINT32  RegLocation = ONLY_REGISTER_OFFSET(LNKCON_IIO_PCIE_REG);
  UINT8   bus = PciAddress.Bus;
  UINT8   device = PciAddress.Device;
  UINT8   func = PciAddress.Function;

  LnkCon.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(bus, device, func, RegLocation));
  if(PortEn == TRUE)
    LnkCon.Bits.link_disable = 0;   //enable link
  else
    LnkCon.Bits.link_disable = 1;   //disable link
  IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR(bus, device, func, RegLocation), LnkCon.Data);
  IioStall (IioGlobalData, IIO_LINK_TRAIN_TIME_MARGIN_G3EQ);
}

VOID
IssuePortRetrainLink(
    IIO_GLOBALS                                    *IioGlobalData,
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
    )
{
  LNKCON_IIO_PCIE_STRUCT   LnkCon;
  UINT32  RegLocation = ONLY_REGISTER_OFFSET(LNKCON_IIO_PCIE_REG);
  UINT8   bus = PciAddress.Bus;
  UINT8   device = PciAddress.Device;
  UINT8   func = PciAddress.Function;

  LnkCon.Data = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(bus, device, func, RegLocation));
  LnkCon.Bits.retrain_link = 1;
  IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR(bus, device, func, RegLocation), LnkCon.Data);
  IioStall (IioGlobalData, IIO_LINK_TRAIN_TIME_MARGIN_G3EQ);
}

BOOLEAN
ForcePortRetrain(
    IIO_GLOBALS                                    *IioGlobalData,
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
)
{
  LNKSTS_IIO_PCIE_STRUCT   LnkSts;
  UINT32                     ExitLoopCnt = 1000;

  IssuePortRetrainLink(IioGlobalData, PciAddress);
  GetLinkStatus(IioGlobalData, PciAddress, &LnkSts);

  while(LnkSts.Bits.link_training && ExitLoopCnt){
    IioStall (IioGlobalData, GEN3_LINK_POLL_TIMEOUT);                //wait for 10ms
    GetLinkStatus(IioGlobalData, PciAddress, &LnkSts);
    ExitLoopCnt--;
  }
  if(!ExitLoopCnt && LnkSts.Bits.link_training){    //if the port is still training after 1s, disable the port
    EnableDisablePCIePort(IioGlobalData, PciAddress, FALSE);       //DISABLE THE PORT
    return FALSE;
  }
  return TRUE;
}

VOID
Gen3WaitTillLinkRetrains(
    IIO_GLOBALS *IioGlobalData, 
    UINT8 BusBase,
    UINT8 Device,
    UINT8 Function
)
{
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress;
  PciAddress.Bus = BusBase;
  PciAddress.Device = Device;
  PciAddress.Function = Function;

  if(ForcePortRetrain(IioGlobalData, PciAddress) == TRUE)
    return;
  else{   //not able to retrain and port is disabled, hence enable again
    EnableDisablePCIePort(IioGlobalData, PciAddress, TRUE);
  }
}
