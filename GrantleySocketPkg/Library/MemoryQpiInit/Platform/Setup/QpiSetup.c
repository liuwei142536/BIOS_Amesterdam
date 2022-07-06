/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
--*/
/*************************************************************************
 *
 * QPI Reference Code
 *
 * ESS - Enterprise Silicon Software
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2006 - 2015 Intel Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents
 * related to the source code ("Material") are owned by Intel
 * Corporation or its suppliers or licensors. Title to the Material
 * remains with Intel Corporation or its suppliers and licensors.
 * The Material contains trade secrets and proprietary and confidential
 * information of Intel or its suppliers and licensors. The Material
 * is protected by worldwide copyright and trade secret laws and treaty
 * provisions.  No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other
 * intellectual property right is granted to or conferred upon you
 * by disclosure or delivery of the Materials, either expressly,
 * by implication, inducement, estoppel or otherwise. Any license
 * under such intellectual property rights must be express and
 * approved by Intel in writing.
 *
 ************************************************************************/

#include "DataTypes.h"
#include "PlatformHost.h"
#include "SysHost.h"
#include "SysFunc.h"
#include "QpiMisc.h"





void
GetQpiSetupOptions (
  struct sysSetup *setup
  )
/*++

  QPI specific host structure setup options

  @param setup - pointer to setup area of host structure

  @retval VOID

--*/
{
    UINT8     Ctr1, Ctr2;

    //
    // Fill-in the system wide options
    //

    for (Ctr1 = 0; Ctr1 < MAX_CPU_SOCKETS; ++Ctr1) { 
      //
      // Assign resources equally among all IIOs
      //
      setup->qpi.BusRatio[Ctr1] = 1;
      setup->qpi.IoRatio[Ctr1] = 1;
      setup->qpi.MmiolRatio[Ctr1] = 1;
      
      //
      // This option is valid only for non-legacy socket.  
      // Set to 1 for sockets w/ no active PCIe ports to reduce power
      //
      setup->qpi.IioUniphyDisable[Ctr1] = 0;
    }

    setup->qpi.LegacyVgaSoc = 0;      // Legacy VGA range target
    setup->qpi.MmioP2pDis = FALSE;    // Enable MMIOL P2P traffic across Sockets
    setup->qpi.IsocAzaliaVc1En = 0;   // Use VC1 for Azalia traffic
    setup->qpi.DebugPrintLevel = 0xF; // Enable all levels of debug messages
    setup->qpi.ClusterOnDieEn = 0;    // Disable COD
    setup->qpi.IBPECIEn = 0;          // Disable inbound PECI
    setup->qpi.E2EParityEn = 0;       // Disable E2EParityEn

    // Set the snoop modes based on Server SKU- POR- mode
    setup->qpi.EarlySnoopEn = 1;      // Enable EarlySnoop (EP) for HSX- and BDX-
#if defined(BDX_HOST)
    setup->qpi.HomeDirWOSBEn = 1;     // Enable Home Dir with OSB for BDX-
#else
    setup->qpi.HomeDirWOSBEn = 0;     // Disable Home Dir with OSB for HSX-
#endif

    setup->qpi.DegradePrecedence = TOPOLOGY_PRECEDENCE;
    
    //
    // Phy/LL system wide settings
    //
    setup->qpi.QpiLinkSpeedMode = QPI_LINK_FULL_SPEED_MODE;
    setup->qpi.QpiLinkSpeed = MAX_QPI_LINK_SPEED;
    setup->qpi.QpiLinkL0pEn = 1;
    setup->qpi.QpiLinkL1En = 1;
    setup->qpi.QpiLinkL0rEn = 2;
    setup->qpi.QpiLbEn = 0;
    setup->qpi.QpiLinkCreditReduce = 2;
    setup->qpi.QpiConfigTxWci = MAX_QPI_LINK_WIDTH;   // Implies "Auto"
    setup->qpi.QpiCrcMode = CRC_MODE_8BIT;
    setup->qpi.QpiAdVNACreditArbThreshold = 1;
    setup->qpi.QpiCpuSktHotPlugEn = 0;
    setup->qpi.QpiCpuSktHotPlugTopology = 0;    //4S Topology
    setup->qpi.PPINrOptIn = 0; 
    setup->qpi.QpiSkuMismatchCheck = 1;

    //
    // Phy/LL per link settings
    // 
    for (Ctr1 = 0; Ctr1 < MAX_CPU_SOCKETS; ++Ctr1) { 
      for (Ctr2 = 0; Ctr2 < MAX_QPI_PORTS; ++Ctr2){         
        setup->qpi.PhyLinkPerPortSetting[Ctr1].Link[Ctr2].QpiPortDisable = FALSE; //Assume all ports are enabled 

        if (setup->qpi.QpiLinkCreditReduce < 2) { // system wide?
          setup->qpi.PhyLinkPerPortSetting[Ctr1].Link[Ctr2].QpiLinkCreditReduce = setup->qpi.QpiLinkCreditReduce;
        } else {
          setup->qpi.PhyLinkPerPortSetting[Ctr1].Link[Ctr2].QpiLinkCreditReduce = 0; // Maximum allowed credits
        }

        if (setup->qpi.QpiLinkSpeed < FREQ_PER_LINK) {
          setup->qpi.PhyLinkPerPortSetting[Ctr1].Phy[Ctr2].QpiLinkSpeed = setup->qpi.QpiLinkSpeed;
        } else {
          setup->qpi.PhyLinkPerPortSetting[Ctr1].Phy[Ctr2].QpiLinkSpeed = SPEED_REC_64GT;
        }

        setup->qpi.PhyLinkPerPortSetting[Ctr1].Phy[Ctr2].QpiProbeType = PROBE_TYPE_NO_PROBE;

        if (setup->qpi.QpiConfigTxWci < QPI_LINK_WIDTH_PER_LINK) { // system wide?
          setup->qpi.PhyLinkPerPortSetting[Ctr1].Phy[Ctr2].QpiConfigTxWci = setup->qpi.QpiConfigTxWci;
        } else {
          setup->qpi.PhyLinkPerPortSetting[Ctr1].Phy[Ctr2].QpiConfigTxWci = MAX_QPI_LINK_WIDTH;  // Implies "Auto"
        }
      }
    }



  return;
}

