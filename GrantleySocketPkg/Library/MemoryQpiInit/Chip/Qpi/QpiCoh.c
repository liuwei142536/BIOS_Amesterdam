/*++
  This file contains 'Framework Code' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may not be modified, except as allowed by
  additional terms of your license agreement.
--*/
/*++
// **************************************************************************
// *                                                                        *
// *      Intel Restricted Secret                                           *
// *                                                                        *
// *      QPI Reference Code                                                *
// *                                                                        *
// *      ESS - Enterprise Silicon Software                                 *
// *                                                                        *
// *      Copyright (c) 2004 - 2016 Intel Corp.                             *
// *                                                                        *
// *      This program has been developed by Intel Corporation.             *
// *      Licensee has Intel's permission to incorporate this source code   *
// *      into their product, royalty free.  This source code may NOT be    *
// *      redistributed to anyone without Intel's written permission.       *
// *                                                                        *
// *      Intel specifically disclaims all warranties, express or           *
// *      implied, and all liability, including consequential and other     *
// *      indirect damages, for the use of this code, including liability   *
// *      for infringement of any proprietary rights, and including the     *
// *      warranties of merchantability and fitness for a particular        *
// *      purpose.  Intel does not assume any responsibility for any        *
// *      errors which may appear in this code nor any responsibility to    *
// *      update it.                                                        *
// *                                                                        *
// **************************************************************************
// **************************************************************************
// *                                                                        *
// *  PURPOSE:                                                              *
// *                                                                        *
// *                                                                        *
// **************************************************************************
--*/
#include "DataTypes.h"
#include "PlatformHost.h"
#include "QpiLib.h"
#include "QpiMisc.h"
#include "SysHost.h"
#include "SysFunc.h"
#include "QpiCoh.h"

//
// List of Cbos that requires preferential treatment in the order while allocating the left over RTIDs
//
STATIC UINT8 HsxEPAsymRtidCboList[] = {8, 10, 12, 14, 6, 4, 2, 0, 7, 9, 11, 13};
STATIC UINT8 HsxEXAsymRtidCboList[] = {8, 10, 12, 14, 4, 2, 0, 7, 9, 11, 13, 15, 6, 17, 1, 3, 5, 16};
// Grantley Refresh HSD 4988202 : Keeping the BDX- asymmetric RTID allocation inline with HSX-EX.
STATIC UINT8 BdxAsymRtidCboList[]   = {8, 10, 12, 14, 4, 2, 0, 7, 9, 11, 13, 15, 6, 17, 1, 3, 5, 16};
// Grantley Refresh HSD 5002776 : Asymmetric rtid assignment for DE 16c
STATIC UINT8 BdxDEHCCAsymRtidCboList[] = {14, 12, 1, 10, 3, 8, 5, 6, 7, 4, 9, 2, 11, 0, 13, 15};


CreditTablePara   CreditTableHsx[MAX_CONFIG_TYPE]= {  

  //For ring credit setting
  //    left_ad_vna_r3qpi0;               // Cbo vna ad ingress (Left cbo)    -> R3QPI
  //    right_ad_vna_r3qpi0;              // Cbo vna ad ingress (Right cbo)   -> R3QPI
  //    left_bl_vna_r3qpi0;               // Cbo vna bl ingress (Left cbo)    -> R3QPI
  //    right_bl_vna_r3qpi0;              // Cbo vna bl ingress (Right cbo)   -> R3QPI
  //    Qpi_To_Cbo_Ipq;                   // Qpi_To_Cbo_Ipq in R3QCBOHACR_R3QPI_CFG.numsnpcreditsl0
  //    Qpi_To_Ha_WB_Credit;              // Qpi_To_Ha_WB_Credit in R3QCBOHACR_R3QPI_CFG_HSX_STRUCT.ha0creditr0
  //For Ha credit setting
  //    AdVnaCrdt;                        // HA VNA AD Ingress  -> R3QPI
  //    BlVnaCrdt;                        // HA VNA BL Ingress  -> R3QPI
  //    Ipq                               // HA IPQ -> Cbo

                                           {2, 2, 1 ,1, 3, 4,  8,  5, 3},      // EXA_8S_HCC
                                           {2, 3, 1 ,1, 3, 4, 15, 13, 3},      // EXA_4S_HCC 
                                           {2, 3, 1 ,1, 4, 4, 11, 10, 4},      // EP_4S_HCC 

                                           {2, 3, 1 ,1, 4, 4, 15, 13, 4},      // EP-HCC_DHT_ENABLED  & EXA-2S 
                                           {3, 4, 1, 2, 4, 0,  6,  5, 4},      // EP_HCC_SHT_ISOC_DISABLED 
                                           {3, 4, 1, 2, 4, 0,  6,  5, 4},      // EP_HCC_ES_ISOC_DISABLED 
                                           {3, 4, 1, 2, 4, 0,  6,  5, 4},      // EP_HCC_SHT_ISOC_ENABLED   
                                          
                                           {2, 3, 1, 1, 4, 4, 15, 15, 4},      // EP_MCC_DHT_ENABLED
                                           {5, 6, 2, 3, 4, 0,  6,  5, 4},      // EP_MCC_SHT_ISOC_DISABLED 
                                           {5, 6, 2, 3, 4, 0,  6,  5, 4},      // EP_MCC_ES_ISOC_DISABLED 
                                           {5, 6, 2, 3, 4, 0,  6,  5, 4},      // EP_MCC_SHT_ISOC_ENABLED
                                          
                                           {2, 0, 1, 0, 7, 4, 15, 15, 5},      // EP_LCC_DHT_ENABLED 
                                           {7, 0, 4, 0, 5, 0, 12, 10, 5},      // EP_LCC_SHT_ISOC_DISABLED 
                                           {7, 0, 4, 0, 5, 0, 12, 10, 5},      // EP_LCC_SHT_ISOC_ENABLED

                                           {2, 3, 1, 1, 7, 4, 15, 15, 7},      // EN_MCC_DHT_ENABLED 
                                           {2, 0, 1, 0, 7, 4, 15, 15, 7},      // EN_LCC_DHT_ENABLED  
                                           {0, 0, 0, 0, 0, 0,  0,  0, 7},      // SYS_1S    
                                           };

CreditTablePara   CreditTableBdx[MAX_CONFIG_TYPE]= {  

  //For ring credit setting
  //    left_ad_vna_r3qpi0;               // Cbo vna ad ingress (Left cbo)    -> R3QPI
  //    right_ad_vna_r3qpi0;              // Cbo vna ad ingress (Right cbo)   -> R3QPI
  //    left_bl_vna_r3qpi0;               // Cbo vna bl ingress (Left cbo)    -> R3QPI
  //    right_bl_vna_r3qpi0;              // Cbo vna bl ingress (Right cbo)   -> R3QPI
  //    Qpi_To_Cbo_Ipq;                   // Qpi_To_Cbo_Ipq in R3QCBOHACR_R3QPI_CFG.numsnpcreditsl0
  //    Qpi_To_Ha_WB_Credit;              // Qpi_To_Ha_WB_Credit in R3QCBOHACR_R3QPI_CFG_HSX_STRUCT.ha0creditr0
  //For Ha credit setting
  //    AdVnaCrdt;                        // HA VNA AD Ingress  -> R3QPI
  //    BlVnaCrdt;                        // HA VNA BL Ingress  -> R3QPI
  //    Ipq                               // HA IPQ -> Cbo

  // Updated the table as per BDX Buffer Credit Sheet rev 4.5
                                           {2, 2, 1 ,1, 3, 4,  6,  4, 3},      // EXA_8S_HCC
                                           {2, 3, 1 ,1, 3, 4, 13, 15, 3},      // EXA_4S_HCC 
                                           {2, 3, 1 ,1, 4, 6,  7, 11, 4},      // EP_4S_HCC 

                                           {2, 3, 1 ,1, 4, 6, 13, 15, 4},      // EP-HCC_DHT_ENABLED  & EXA-2S 
                                           {3, 4, 1, 2, 4, 0,  6,  5, 4},      // EP_HCC_SHT_ISOC_DISABLED [NA, Using values from EP_HCC_ES_ISOC_DISABLED, as in HSX] 
                                           {3, 4, 1, 2, 4, 0,  6,  5, 4},      // EP_HCC_ES_ISOC_DISABLED 
                                           {3, 3, 1, 2, 4, 0,  4,  4, 4},      // EP_HCC_SHT_ISOC_ENABLED   
                                          
                                           {2, 3, 1, 1, 4, 6, 15, 15, 4},      // EP_MCC_DHT_ENABLED
                                           {4, 4, 2, 3, 4, 0,  6,  5, 4},      // EP_MCC_SHT_ISOC_DISABLED [NA, Using values from EP_MCC_ES_ISOC_DISABLED, as in HSX] 
                                           {4, 4, 2, 3, 4, 0,  6,  5, 4},      // EP_MCC_ES_ISOC_DISABLED 
                                           {4, 4, 2, 3, 4, 0,  4,  4, 4},      // EP_MCC_SHT_ISOC_ENABLED
                                          
                                           {2, 0, 1, 0, 5, 6, 15, 15, 5},      // EP_LCC_DHT_ENABLED 
                                           {7, 0, 4, 0, 5, 0, 10,  9, 5},      // EP_LCC_SHT_ISOC_DISABLED [NA, Using values from EP_LCC_SHT_ISOC_ENABLED, as in HSX]
                                           {7, 0, 4, 0, 5, 0, 10,  9, 5},      // EP_LCC_SHT_ISOC_ENABLED

                                           {2, 3, 1, 1, 7, 7, 15, 15, 7},      // EN_MCC_DHT_ENABLED 
                                           {2, 0, 1, 0, 7, 7, 15, 15, 7},      // EN_LCC_DHT_ENABLED  
                                           {0, 0, 0, 0, 0, 0,  0,  0, 7},      // SYS_1S    
                                           };

QPI_STATUS
AllocateRtids (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  )
/*++

Routine Description:

  Allocates RTIDs to sockets

Arguments:

  host  - Pointer to the system host (root) structure
  SocketData  - socket specific data structure
  QpiInternalGlobal - QPIRC Internal Global data

Returns:

  0 - Successful
  1 - Failures
  
--*/
{
  QpiInternalGlobal->UboxRtids = UBOX_RTIDS_HSX;


  if (QpiInternalGlobal->BtModeEn == FALSE) {
    if (host->var.qpi.SysConfig == SYS_CONFIG_1S) {
      // Grantley Refresh HSD 5002776 : Asymmetric rtid assignment for DE 16c
      // Distribute the extra RTIDs to certain Cbos for performance needs
      if ((host->var.common.cpuType == CPU_BDX) && \
          (QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_DE || QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_NS) &&
          (host->var.common.chopType[QpiInternalGlobal->SbspSoc] == HCC_DE)) {
        AllocateRtidSHT1SBdxHCCDE (host, QpiInternalGlobal);
      } else {
        AllocateRtidSHT1SHsx (host, QpiInternalGlobal); // Takes care of ISOC and Non-ISOC cases
      } // BDX-HCC-DE- check
    } else if (host->var.qpi.SysConfig == SYS_CONFIG_2SEN || host->var.qpi.SysConfig == SYS_CONFIG_2SEP) {
      if (host->var.qpi.OutIsocEn == TRUE) {
        AllocateRtidSHTIsoc2SHsx (host, QpiInternalGlobal);
      } else {
        AllocateRtidSHTNonIsoc2SHsx (host, QpiInternalGlobal);
      }
    } else {
      QPI_ASSERT(FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL); // Bug in priming the host structure
    }
  } else {

    if (QpiInternalGlobal->BtMode != BT_MODE_2) {
      if (host->var.qpi.OutClusterOnDieEn == TRUE) {
        AllocateRtidDHTCODHsx (host, QpiInternalGlobal);    // Takes care of DHT COD case
      } else {
        AllocateRtidDHTCommonHsx (host, QpiInternalGlobal); // Takes care of all DHT cases including COD
      }

    } else {
      if (QpiInternalGlobal->Sys4SClusterOnDieEn == TRUE) {
        // Check for cpuType before calling the corresponding allocation routines
        if (host->var.common.cpuType == CPU_BDX) {
          AllocateRtids4SCODBdx (host, QpiInternalGlobal);
        } 
#ifdef QPI_4S_COD_FEATURE_PROTOTYPE
        else if (host->var.common.cpuType == CPU_HSX) { // For cpuType as CPU_HSX
          AllocateRtids4SCODHsx (host, QpiInternalGlobal);
        }
#endif
        else {
          QPI_ASSERT(FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL); // Bug in priming the host structure
        } // cpuType checks      
      } else { // Not 4SCOD means we are running 8S
        if (host->var.common.cpuType == CPU_HSX) {
          AllocateRtid8SHsx (host, QpiInternalGlobal);
        } else { // For cpuType as CPU_BDX
          AllocateRtid8SBdx (host, QpiInternalGlobal);
        } // cpuType check
      } // COD check
    } // BtMode2 check
  }

  return QPI_SUCCESS;
}

VOID
AllocateRtidSHT1SHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Allocates RTIDs in 1S system

Arguments:

  host  - Pointer to the system host (root) structure
  QpiInternalGlobal - QPIRC Internal Global data

Returns:

  VOID
    
--*/
{
  UINT8   Idx1, Idx3;
  UINT8   TotCbo, LocalRtidPerCbo, IsocRtids, TotExtraRtids;
  UINT8   LocalBase, TempBase, ReallocBase;


  // Identify number of ISOC RTIDs
  if (host->var.qpi.OutIsocEn == TRUE) {
    IsocRtids = DEFAULT_ISOC_RTIDS;
  } else {
    IsocRtids = 0;  //Normal mode
  }

  // Calculate the total RTIDs per Cbo
  TotCbo = QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc];
  MemCheckBoundary(host, TotCbo, MAX_CBO); //Klocwork 2228: Array 'QpiInternalGlobal->RtidBase[0]' of size 52 may use index value(s) 53..253.

  LocalRtidPerCbo = (MAX_RTID_SYSTEM - (CBO_WB_RTIDS * 2) - IsocRtids) / TotCbo;

  // Saturate the RTID at 16 since with 2 pool per Cbo and 8 RTID per pool, a Cbo can not consume more than 16 RTIDs
  if (LocalRtidPerCbo > (2 * MAX_RTID_PER_POOL)) {
    LocalRtidPerCbo = 2 * MAX_RTID_PER_POOL;
  }

  TotExtraRtids = MAX_RTID_SYSTEM - (CBO_WB_RTIDS * 2) - IsocRtids  - (LocalRtidPerCbo * TotCbo);
  LocalBase = 0;
  ReallocBase = (MAX_RTID_SYSTEM / 2) + CBO_WB_RTIDS; // RTID0 in extended RTID pool is reserved.

  // Fill the Base & Size for WB in Local Pool
  QpiInternalGlobal->RtidBase[0][0] = LocalBase; // WB always assigned RTID 0
  QpiInternalGlobal->RtidSize[0][0] = CBO_WB_RTIDS;
  LocalBase = QpiInternalGlobal->RtidBase[0][0] + QpiInternalGlobal->RtidSize[0][0];

  // Fill the Base & Size for ISOC in Local Pool
  QpiInternalGlobal->RtidBase[0][2] = LocalBase;
  QpiInternalGlobal->RtidSize[0][2] = IsocRtids;
  LocalBase = QpiInternalGlobal->RtidBase[0][2] + QpiInternalGlobal->RtidSize[0][2];

  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;    WB - %02u   Ubox - %02u   Isoc - %02u   Local RTID PerCbo - %02u   Extra - %02u",
                             (CBO_WB_RTIDS * 2), 0, IsocRtids, LocalRtidPerCbo, TotExtraRtids));
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;    Local Base - %02u   Reallocation Base - %02u", LocalBase, ReallocBase));

  //
  // Fill the Base & Size for Cbo. Cbo can split its available RTID per HA into two pools. But the spliting is needed only when it has >8 RTIDs
  // or when its RTIDs straddles into xRTID pool space. 
  //
  Idx3 = TotCbo * 2 + 3;
  for (Idx1 = 3; Idx1 < Idx3; Idx1 += 2) { // Start with Cbo 0 Rtid pool

    // 
    // If the base is straddling into xRTID, split the Cbo's RTID range into two and append the 
    // second part of the RTIDs into the xRTID pool space 
    // 
    if ((LocalBase < (MAX_RTID_SYSTEM / 2)) && ((LocalBase + LocalRtidPerCbo) > (MAX_RTID_SYSTEM / 2))) {
      QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n;    Cbo %02u RTID straddles into xRTID space", ((Idx1 - 3) / 2)));
      TempBase = HandleCboRtidStraddle (host, QpiInternalGlobal, QpiInternalGlobal->RtidBase[0], QpiInternalGlobal->RtidSize[0],
                               &TotExtraRtids, LocalBase, Idx1, LocalRtidPerCbo, ReallocBase);
      QPI_ASSERT ((TempBase > LocalBase), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
      LocalBase = TempBase;
    } else {
      QpiInternalGlobal->RtidBase[0][Idx1] = LocalBase;
      QpiInternalGlobal->RtidSize[0][Idx1] = LocalRtidPerCbo;
      LocalBase = QpiInternalGlobal->RtidBase[0][Idx1] + QpiInternalGlobal->RtidSize[0][Idx1];
      if (LocalBase == (MAX_RTID_SYSTEM / 2)) {
        QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n;    Adjusting Cbo %02u base to Reallocation base", (((Idx1 - 3) / 2) + 1)));
        LocalBase = ReallocBase; 
      }
    }
  }

  // Extra RTIDs
  QpiInternalGlobal->RtidSize[0][MAX_CBO * 2 + 3]  = TotExtraRtids;

  // Calculate the HT Base based on these contraints:
  //  - Home Tracker entries must be non overlapping; since all 128 trackers are allocated to local socket set NID0 base to 0
  //    and NID4 base 64.
  //
  QpiInternalGlobal->HtBase[0] = 0x00400000;

  CheckAndSplitCboRtids (host, QpiInternalGlobal, (UINT8 *)QpiInternalGlobal->RtidBase, (UINT8 *)QpiInternalGlobal->RtidSize);
  DumpRtidAllocationSHT (host, QpiInternalGlobal);

  return;
}


VOID
AllocateRtidSHT1SBdxHCCDE (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Allocates RTIDs for a BDX-HCC-DE- SKU (1S) assymmetrically

Arguments:

  host  - Pointer to the system host (root) structure
  QpiInternalGlobal - QPIRC Internal Global data

Returns:

  VOID
    
--*/
{
  UINT8   Idx1, Idx3;
  UINT8   TotCbo, LocalRtidPerCbo, TotExtraRtids, Cbo;
  UINT8   LocalBase, TempBase, ReallocBase;

  // Calculate the total RTIDs per Cbo
  TotCbo = QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc];
  MemCheckBoundary(host, TotCbo, MAX_CBO); //Klocwork 2228: Array 'QpiInternalGlobal->RtidBase[0]' of size 52 may use index value(s) 53..253.

  LocalRtidPerCbo = (MAX_RTID_SYSTEM - (CBO_WB_RTIDS * 2)) / TotCbo;

  // Saturate the RTID at 16 since with 2 pool per Cbo and 8 RTID per pool, a Cbo can not consume more than 16 RTIDs
  if (LocalRtidPerCbo > (2 * MAX_RTID_PER_POOL)) {
    LocalRtidPerCbo = 2 * MAX_RTID_PER_POOL;
  }

  TotExtraRtids = MAX_RTID_SYSTEM - (CBO_WB_RTIDS * 2) - (LocalRtidPerCbo * TotCbo);
  LocalBase = 0;
  ReallocBase = (MAX_RTID_SYSTEM / 2) + CBO_WB_RTIDS; // RTID0 in extended RTID pool is reserved.

  // Fill the Base & Size for WB in Local Pool
  QpiInternalGlobal->RtidBase[0][0] = LocalBase; // WB always assigned RTID 0
  QpiInternalGlobal->RtidSize[0][0] = CBO_WB_RTIDS;
  LocalBase = QpiInternalGlobal->RtidBase[0][0] + QpiInternalGlobal->RtidSize[0][0];

  // Fill in the size based on RTIDs per Cbo for all Cbos
  Idx3 = TotCbo * 2 + 3;
  for (Idx1 = 3; Idx1 < Idx3; Idx1 += 2) {
    QpiInternalGlobal->RtidSize[0][Idx1] = LocalRtidPerCbo;
  }
  
  // Distribute the extra RTIDs by assigning them to Cbos based on priority list
  for (Idx1=0; Idx1 < sizeof(BdxDEHCCAsymRtidCboList) && (TotExtraRtids != 0); Idx1++) {
    Cbo = BdxDEHCCAsymRtidCboList[Idx1];
      
    if (Cbo >= TotCbo) {
      continue;
    }
    
    Idx3 = (Cbo * 2) + 3; // get the index to first pool of the Cbo
    
    // Skip allocation of extra RTID if the pool is saturated
    if (QpiInternalGlobal->RtidSize[0][Idx3] >= (2 * MAX_RTID_PER_CBO_PER_POOL)) {
      continue;
    }
    
    TotExtraRtids--;
    QpiInternalGlobal->RtidSize[0][Idx3]++;
  } // Idx1 loop
    
  
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;    WB - %02u   Ubox - %02u   Local RTID PerCbo (base) - %02u   Extra - %02u",
                             (CBO_WB_RTIDS * 2), 0, LocalRtidPerCbo, TotExtraRtids));
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;    Local Base - %02u   Reallocation Base - %02u", LocalBase, ReallocBase));

  //
  // Fill the base and final size for Cbo. Cbo can split its available RTID per HA into two pools. But the spliting is needed only when it has > 8 RTIDs
  // or when its RTIDs straddles into xRTID pool space. 
  //
  Idx3 = TotCbo * 2 + 3;
  for (Idx1 = 3; Idx1 < Idx3; Idx1 += 2) { // Start with Cbo 0 Rtid pool
    
    // Get the size for this given Cbo
    LocalRtidPerCbo = QpiInternalGlobal->RtidSize[0][Idx1];
    
    // 
    // If the base is straddling into xRTID, split the Cbo's RTID range into two and append the 
    // second part of the RTIDs into the xRTID pool space 
    // 
    if ((LocalBase < (MAX_RTID_SYSTEM / 2)) && ((LocalBase + LocalRtidPerCbo) > (MAX_RTID_SYSTEM / 2))) {
      QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n;    Cbo %02u RTID straddles into xRTID space", ((Idx1 - 3) / 2)));
      TempBase = HandleCboRtidStraddle (host, QpiInternalGlobal, QpiInternalGlobal->RtidBase[0], QpiInternalGlobal->RtidSize[0],
                               &TotExtraRtids, LocalBase, Idx1, LocalRtidPerCbo, ReallocBase);
      QPI_ASSERT ((TempBase > LocalBase), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
      LocalBase = TempBase;
    } else {
      QpiInternalGlobal->RtidBase[0][Idx1] = LocalBase;
      QpiInternalGlobal->RtidSize[0][Idx1] = LocalRtidPerCbo;
      LocalBase = QpiInternalGlobal->RtidBase[0][Idx1] + QpiInternalGlobal->RtidSize[0][Idx1];
      
      if (LocalBase == (MAX_RTID_SYSTEM / 2)) {
        QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n;    Adjusting Cbo %02u base to Reallocation base", (((Idx1 - 3) / 2) + 1)));
        LocalBase = ReallocBase; 
      }
    }
  }

  // Extra RTIDs
  QpiInternalGlobal->RtidSize[0][MAX_CBO * 2 + 3]  = TotExtraRtids;

  // Calculate the HT Base based on these contraints:
  //  - Home Tracker entries must be non overlapping; since all 128 trackers are allocated to local socket set NID0 base to 0
  //    and NID4 base 64.
  //
  QpiInternalGlobal->HtBase[0] = 0x00400000;

  CheckAndSplitCboRtids (host, QpiInternalGlobal, (UINT8 *)QpiInternalGlobal->RtidBase, (UINT8 *)QpiInternalGlobal->RtidSize);
  DumpRtidAllocationSHT (host, QpiInternalGlobal);

  return;
}


VOID
AllocateRtidSHTIsoc2SHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Allocates RTIDs for 2S ISOC Static HT config.

Arguments:

  host  - Pointer to the system host (root) structure
  QpiInternalGlobal - QPIRC Internal Global data

Returns:

 VOID  
 
--*/
{
  UINT8   Idx1, Idx2, Idx3;
  UINT8   TotCbo, TotalRtidPerCbo, IsocRtids, TotExtraRtids;
  UINT8   LocalRtidPerCbo, RemoteRtidPerCbo;
  UINT8   LocalBase, RemoteBase, ReallocBase, TempBase;
  BOOLEAN RemoteInXRtidPool;

  // Identify number of ISOC RTIDs
  IsocRtids = DEFAULT_ISOC_RTIDS;

  // 
  // Calculate the total RTIDs per Cbo. Note that we need to allocate twice the number of WB & Ubox/IIO though
  // only one range will be used. This is due to HT tracker and ROT alignment constraints.
  // 
  TotCbo = QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc];
  MemCheckBoundary(host, TotCbo, MAX_CBO); //Klocwork 2233, 2234, 2226, 2230: Array 'QpiInternalGlobal->RtidBaseIsocS0[x]' of size 52 may use index value(s) 53..253. Also there is one similar error on line 501
  TotalRtidPerCbo = (MAX_RTID_SYSTEM - (2 * (CBO_WB_RTIDS + QpiInternalGlobal->UboxRtids)) - IsocRtids) / TotCbo;

  // 
  // Calculate the Local and Remote RTIDs per Cbo. For NUMA, remote is always assigned 2 RTIDs (minimum required)
  // and the rest of the RTIDs are assigned for local HA communication. For UMA >=12C SKUs, local is always assigned
  // 3 RTIDs (minimum required) and the rest of the RTIDs are assigned for remote HA communication. For other SKUs
  // of UMA, local HA is assigned 1 RTID less than Remote HA.
  // 
  if (host->setup.common.numaEn == TRUE) {
    RemoteRtidPerCbo = MIN_REMOTE_RTID;
    LocalRtidPerCbo = TotalRtidPerCbo - RemoteRtidPerCbo;
  } else {
    if (TotCbo >= 12) {
      LocalRtidPerCbo = MIN_LOCAL_RTID;
      RemoteRtidPerCbo = TotalRtidPerCbo - LocalRtidPerCbo;
    } else {
      if (TotalRtidPerCbo % 2) {
        LocalRtidPerCbo = (TotalRtidPerCbo - 1) / 2;
        RemoteRtidPerCbo = ((TotalRtidPerCbo - 1) / 2) + 1;
      } else {
        LocalRtidPerCbo = (TotalRtidPerCbo / 2) - 1;
        RemoteRtidPerCbo = (TotalRtidPerCbo / 2) + 1;
      }
    }
  }

  // Saturate the local/remote RTID at 16 since with 2 pool per Cbo and 8 RTID per Cbo, a Cbo can not consume more than 16 RTIDs
  if (LocalRtidPerCbo > (2 * MAX_RTID_PER_POOL)) {
    LocalRtidPerCbo = 2 * MAX_RTID_PER_POOL;
  }
  if (RemoteRtidPerCbo > (2 * MAX_RTID_PER_POOL)) {
    RemoteRtidPerCbo = 2 * MAX_RTID_PER_POOL;
  }

  TotExtraRtids = MAX_RTID_SYSTEM - (2 * (CBO_WB_RTIDS + QpiInternalGlobal->UboxRtids)) - IsocRtids  - ((LocalRtidPerCbo + RemoteRtidPerCbo) * TotCbo);

  //
  // Calculate the RTID base and size for Legacy Socket. Note that unlike other RTID schemes, for the ISOC 2S case, the RTID
  // allocation is not symmetrical; i.e for Legacy and Non-Legacy socket the Base RTID for local & remote communication is different.
  // This is due to the fact that the Non-Legacy socket is not originator of ISOC traffic (though it can receive ISOC requests from Legacy socket)
  // 


  // Fill the Base & Size for WB in Local/Remote Pool
  TempBase = 0;
  QpiInternalGlobal->RtidBaseIsocS0[0][0] = QpiInternalGlobal->RtidBaseIsocS0[1][0] = TempBase;
  QpiInternalGlobal->RtidSizeIsocS0[0][0] = QpiInternalGlobal->RtidSizeIsocS0[1][0] = CBO_WB_RTIDS;
  TempBase = QpiInternalGlobal->RtidBaseIsocS0[0][0] + QpiInternalGlobal->RtidSizeIsocS0[0][0];

  // Fill the Base & Size for Ubox & IIO in Local/Remote Pool
  QpiInternalGlobal->RtidBaseIsocS0[0][1] = QpiInternalGlobal->RtidBaseIsocS0[1][1] = TempBase;
  QpiInternalGlobal->RtidSizeIsocS0[0][1] = QpiInternalGlobal->RtidSizeIsocS0[1][1] = QpiInternalGlobal->UboxRtids;
  TempBase = QpiInternalGlobal->RtidBaseIsocS0[0][1] + QpiInternalGlobal->RtidSizeIsocS0[0][1];

  // 
  // Fill the Base & Size for ISOC in Local/Remote HA.
  // 
  QpiInternalGlobal->RtidBaseIsocS0[0][2] = QpiInternalGlobal->RtidBaseIsocS0[1][2] = TempBase;
  QpiInternalGlobal->RtidSizeIsocS0[0][2] = QpiInternalGlobal->RtidSizeIsocS0[1][2] = IsocRtids;

  //
  // If the number of RTID per Cbo in the local pool > remote pool, then the local RTIDs are placed starting from non-extended RTID pool 
  // while the remote RTIDs are placed in extended RTID pool. Otherwise remote RTIDs are placed starting from non-extended RTID pool.
  // Though ISOC RTIDs are assigned for Local and Remote HA communication, at any given time it will be used only for local or remote
  // communication but not both. So RBT entries for ISOC RTIDs are allocated only in non-EXtRTID segment and the remote base calculation
  // below must ignore ISOC RTIDs.
  // 
  LocalBase = QpiInternalGlobal->RtidBaseIsocS0[0][2] + QpiInternalGlobal->RtidSizeIsocS0[0][2];
  RemoteBase = (MAX_RTID_SYSTEM / 2) + QpiInternalGlobal->RtidBaseIsocS0[1][1] + QpiInternalGlobal->RtidSizeIsocS0[1][1];
  RemoteInXRtidPool = TRUE;
  ReallocBase = RemoteBase + TotCbo * RemoteRtidPerCbo;
  if (LocalRtidPerCbo < RemoteRtidPerCbo) {
    TempBase = LocalBase;
    LocalBase = RemoteBase;
    RemoteBase = TempBase;
    RemoteInXRtidPool = FALSE;
    ReallocBase = LocalBase + TotCbo * LocalRtidPerCbo;
  }

  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;    WB - %02u   Ubox - %02u   Isoc - %02u   Local RTID PerCbo - %02u   Remote RTID PerCbo - %02u   Extra - %02u",
                             (2 * CBO_WB_RTIDS), (2 * QpiInternalGlobal->UboxRtids), IsocRtids, LocalRtidPerCbo, RemoteRtidPerCbo, TotExtraRtids));
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;    Local Base - %02u   Remote Base - %02u   Reallocation Base - %02u",
                             LocalBase, RemoteBase, ReallocBase));


  //
  // Fill the Base & Size for Cbo. Cbo can split its available RTID per HA into two pools. But the spliting is needed only when it has >8 RTIDs
  // or when its RTIDs straddles into xRTID boundary. 
  //
  Idx3 = TotCbo * 2 + 3;
  for (Idx1 = 3; Idx1 < Idx3; Idx1 += 2) { // Start with Cbo 0 Rtid pool
    if (RemoteInXRtidPool == TRUE) {
      // 
      // If the RTID range of a Cbo is straddling into xRTID space, split the Cbo's RTID range into two and append the 
      // second part of the RTIDs into the xRTID pool space just below remote RTID space (the base is stored in ReallocaBase).
      // If a Cbo range is terminating exactly at non-xRTID boundary, then shift the next Cbo's Local RTID base to after remote RTID space.
      // Note that the boudary checking is not needed for Remote pool because in this case it is guranteed that Remote has < 64 RTIDs.
      // There is also a corner case for configs where a Cbo is assigned >= 10 RTIDs. In 10 RTID case, the Cbo that straddles into xRTID space
      // might have 1 RTIDs in non-xRTID space while remaining 9 will be in xRTID space. This is not allowed since it will require 3 pools to
      // assign the RTIDs. If we have enough Extra RTIDs available then we can push the RTIDs for the Cbo entirely into xRTID space and
      // allow the Cbo to use all the 10 RTIDs. Otherwise we can not avoid the 3 way split of the RTIDs and ignore the split that has least
      // number of RTIDs.
      // 
      if ((LocalBase < (MAX_RTID_SYSTEM / 2)) && ((LocalBase + LocalRtidPerCbo) > (MAX_RTID_SYSTEM / 2))) {
        QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n;    Cbo %02u RTID straddles into xRTID space", ((Idx1 - 3) / 2)));
        TempBase = HandleCboRtidStraddle (host, QpiInternalGlobal, QpiInternalGlobal->RtidBaseIsocS0[0], QpiInternalGlobal->RtidSizeIsocS0[0],
                               &TotExtraRtids, LocalBase, Idx1, LocalRtidPerCbo, ReallocBase);
        QPI_ASSERT ((TempBase > LocalBase), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
        LocalBase = TempBase;
      } else {
        QpiInternalGlobal->RtidBaseIsocS0[0][Idx1] = LocalBase;
        QpiInternalGlobal->RtidSizeIsocS0[0][Idx1] = LocalRtidPerCbo;
        LocalBase = LocalBase + LocalRtidPerCbo;
        if (LocalBase == (MAX_RTID_SYSTEM / 2)) {
          QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n;    Adjusting Cbo %02u base to Reallocation base", (((Idx1 - 3) / 2) + 1)));
          LocalBase = ReallocBase; // Shift the base to right below the Remote RTID space
        }
      }
      QpiInternalGlobal->RtidBaseIsocS0[1][Idx1] = RemoteBase;
      QpiInternalGlobal->RtidSizeIsocS0[1][Idx1] = RemoteRtidPerCbo;
      RemoteBase = RemoteBase + RemoteRtidPerCbo;
    } else {
      // 
      // Repeat the steps same as above except that the role of Local and Remote is swapped.
      // 
      if ((RemoteBase < (MAX_RTID_SYSTEM / 2)) && ((RemoteBase + RemoteRtidPerCbo) > (MAX_RTID_SYSTEM / 2))) {
        QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n;    Cbo %02u RTID straddles into xRTID space", ((Idx1 - 3) / 2)));
        TempBase = HandleCboRtidStraddle (host, QpiInternalGlobal, QpiInternalGlobal->RtidBaseIsocS0[1], QpiInternalGlobal->RtidSizeIsocS0[1],
                               &TotExtraRtids, RemoteBase, Idx1, RemoteRtidPerCbo, ReallocBase);
        QPI_ASSERT ((TempBase > RemoteBase), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
        RemoteBase = TempBase;
      } else {
        QpiInternalGlobal->RtidBaseIsocS0[1][Idx1] = RemoteBase;
        QpiInternalGlobal->RtidSizeIsocS0[1][Idx1] = RemoteRtidPerCbo;
        RemoteBase = RemoteBase + RemoteRtidPerCbo;
        if (RemoteBase == (MAX_RTID_SYSTEM / 2)) {
          QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n;    Adjusting Cbo %02u base to Reallocation base", (((Idx1 - 3) / 2) + 1)));
          RemoteBase = ReallocBase; // Shift the base to right below the Local RTID space
        }
      }
      QpiInternalGlobal->RtidBaseIsocS0[0][Idx1] = LocalBase;
      QpiInternalGlobal->RtidSizeIsocS0[0][Idx1] = LocalRtidPerCbo;
      LocalBase = LocalBase + LocalRtidPerCbo;
    }
  }

  // Extra RTIDs
  QpiInternalGlobal->RtidSizeIsocS0[0][MAX_CBO * 2 + 3]  = TotExtraRtids;

  //
  // Calculate the RTID base and size for Non-Legacy Socket. It is basically same seeting as that of the Legacy socket's value
  // except that the Non-XRTID and XRTID spaces are swapped. And also, there is no ISOC RTIDs assigned since non-legacy
  // never sends out any ISOC requests.
  //

  MemCopy ((UINT8 *)QpiInternalGlobal->RtidBaseIsocS1, (UINT8 *)QpiInternalGlobal->RtidBaseIsocS0, sizeof (QpiInternalGlobal->RtidBaseIsocS1));
  MemCopy ((UINT8 *)QpiInternalGlobal->RtidSizeIsocS1, (UINT8 *)QpiInternalGlobal->RtidSizeIsocS0, sizeof (QpiInternalGlobal->RtidSizeIsocS1));

  // Clear the ISOC RTIDs since Non-Legacy socket doesn't use them
  QpiInternalGlobal->RtidBaseIsocS1[0][2] = QpiInternalGlobal->RtidBaseIsocS1[1][2] = 0;
  QpiInternalGlobal->RtidSizeIsocS1[0][2] = QpiInternalGlobal->RtidSizeIsocS1[1][2] = 0;

  // Add or subtract 64 to create the mirror image of the legacy socket's RTID allocation
  for (Idx2 = 0; Idx2 < 2; Idx2++) {
    for (Idx1 = 3; Idx1 < Idx3; Idx1++) {
      if (QpiInternalGlobal->RtidBaseIsocS1[Idx2][Idx1] != 0) {
        if (QpiInternalGlobal->RtidBaseIsocS1[Idx2][Idx1] > (MAX_RTID_SYSTEM / 2)) {
          QpiInternalGlobal->RtidBaseIsocS1[Idx2][Idx1] = QpiInternalGlobal->RtidBaseIsocS1[Idx2][Idx1] - (MAX_RTID_SYSTEM / 2);
        } else {
          QpiInternalGlobal->RtidBaseIsocS1[Idx2][Idx1] = QpiInternalGlobal->RtidBaseIsocS1[Idx2][Idx1] + (MAX_RTID_SYSTEM / 2);
        }
      }
    }
  }


  // 
  // Calculate the HT Base based on these contraints:
  //  - Home Tracker entries must be non overlapping
  //  - ISOC requests are made only by S0 and it can target both S0 & S1 HAs. ISOC always uses non-xRTID.
  //     So NID0 base must be always 0 for S0 & S1 HTBASE setting.
  //  - EWB can be made by S0 & S1 and it can target both S0 & S1 HAs. EWB always uses non-xRTID 0.
  //     So NID1 base must be always 64 for S0 & S1 HTBASE setting.
  //  - The base value can be either 0 or 64; no other value possible because HSX hard codes ROT at 0 or 64
  //
  QpiInternalGlobal->HtBase[0] = 0x00404000;
  QpiInternalGlobal->HtBase[1] = 0x00404000;

  //
  // If a Cbo is assigned >8 RTIDs, then split that into two pools because a pool is allowed to have max 8 RTIDs.
  //
  CheckAndSplitCboRtids (host, QpiInternalGlobal, (UINT8 *)QpiInternalGlobal->RtidBaseIsocS0, (UINT8 *)QpiInternalGlobal->RtidSizeIsocS0);
  CheckAndSplitCboRtids (host, QpiInternalGlobal, (UINT8 *)QpiInternalGlobal->RtidBaseIsocS1, (UINT8 *)QpiInternalGlobal->RtidSizeIsocS1);
  DumpRtidAllocationSHT (host, QpiInternalGlobal);

  return;
}

VOID
AllocateRtidSHTNonIsoc2SHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Allocates RTIDs for 2S ISOC config

Arguments:

  host  - Pointer to the system host (root) structure
  QpiInternalGlobal - QPIRC Internal Global data

Returns:

 VOID  
 
--*/
{
  UINT8   Idx1, Idx3;
  UINT8   TotCbo, TotalRtidPerCbo, TotExtraRtids;
  UINT8   LocalRtidPerCbo, RemoteRtidPerCbo;
  UINT8   LocalBase, RemoteBase, ReallocBase, TempBase;
  BOOLEAN RemoteInXRtidPool;


  // 
  // Calculate the total RTIDs per Cbo. Note that we need to allocate twice the number of WB
  // since the RTID0 and RTID64 are reserved for WB..
  // 
  TotCbo = QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc];
  MemCheckBoundary(host, TotCbo, MAX_CBO); //Klocwork 2229, 2236, 2227, 2238Array 'QpiInternalGlobal->RtidBase[x]' of size 52 may use index value(s) 53..253. Also there is one similar error on line 718
  TotalRtidPerCbo = (MAX_RTID_SYSTEM - (CBO_WB_RTIDS * 2) - QpiInternalGlobal->UboxRtids) / TotCbo;

  // 
  // Calculate the Local and Remote RTIDs per Cbo. For NUMA, remote is always assigned 2 RTIDs (minimum required)
  // and the rest of the RTIDs are assigned for local HA communication. For UMA >=12C SKUs, local is always assigned
  // 3 RTIDs (minimum required) and the rest of the RTIDs are assigned for remote HA communication. For other SKUs
  // of UMA, local HA is assigned 1 RTID less than Remote HA.
  // 
  if (host->setup.common.numaEn == TRUE) {
    RemoteRtidPerCbo = MIN_REMOTE_RTID;
    LocalRtidPerCbo = TotalRtidPerCbo - RemoteRtidPerCbo;
  } else {
    if (TotCbo >= 12) {
      LocalRtidPerCbo = MIN_LOCAL_RTID;
      RemoteRtidPerCbo = TotalRtidPerCbo - LocalRtidPerCbo;
    } else {
      if (TotalRtidPerCbo % 2) {
        LocalRtidPerCbo = (TotalRtidPerCbo - 1) / 2;
        RemoteRtidPerCbo = ((TotalRtidPerCbo - 1) / 2) + 1;
      } else {
        LocalRtidPerCbo = (TotalRtidPerCbo / 2) - 1;
        RemoteRtidPerCbo = (TotalRtidPerCbo / 2) + 1;
      }
    }
  }

  // Saturate the local/remote RTID at 16 since with 2 pool per Cbo and 8 RTID per Cbo, a Cbo can not consume more than 16 RTIDs
  if (LocalRtidPerCbo > (2 * MAX_RTID_PER_POOL)) {
    LocalRtidPerCbo = 2 * MAX_RTID_PER_POOL;
  }
  if (RemoteRtidPerCbo > (2 * MAX_RTID_PER_POOL)) {
    RemoteRtidPerCbo = 2 * MAX_RTID_PER_POOL;
  }

  TotExtraRtids = MAX_RTID_SYSTEM - (2 * CBO_WB_RTIDS) - QpiInternalGlobal->UboxRtids - ((LocalRtidPerCbo + RemoteRtidPerCbo) * TotCbo);

  //
  // Calculate the RTID base and size.
  // 


  // Fill the Base & Size for WB in Local/Remote Pool
  TempBase = 0;
  QpiInternalGlobal->RtidBase[0][0] = QpiInternalGlobal->RtidBase[1][0] = TempBase;
  QpiInternalGlobal->RtidSize[0][0] = QpiInternalGlobal->RtidSize[1][0] = CBO_WB_RTIDS;
  TempBase = QpiInternalGlobal->RtidBase[0][0] + QpiInternalGlobal->RtidSize[0][0];

  // Fill the Base & Size for Ubox & IIO in Local/Remote Pool
  QpiInternalGlobal->RtidBase[0][1] = QpiInternalGlobal->RtidBase[1][1] = TempBase;
  QpiInternalGlobal->RtidSize[0][1] = QpiInternalGlobal->RtidSize[1][1] = QpiInternalGlobal->UboxRtids;
  TempBase = QpiInternalGlobal->RtidBase[0][1] + QpiInternalGlobal->RtidSize[0][1];

  //
  // If the number of RTID per Cbo in the local pool > remote pool, then the local RTIDs are placed starting from non-extended RTID pool 
  // while the remote RTIDs are placed in extended RTID pool. Otherwise remote RTIDs are placed starting from non-extended RTID pool.
  // 
  LocalBase = TempBase;
  RemoteBase = (MAX_RTID_SYSTEM / 2) + CBO_WB_RTIDS;
  RemoteInXRtidPool = TRUE;
  ReallocBase = RemoteBase + TotCbo * RemoteRtidPerCbo;
  if (LocalRtidPerCbo < RemoteRtidPerCbo) {
    TempBase = LocalBase;
    LocalBase = RemoteBase;
    RemoteBase = TempBase;
    RemoteInXRtidPool = FALSE;
    ReallocBase = LocalBase + TotCbo * LocalRtidPerCbo;
  }

  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;    WB - %02u   Ubox - %02u   Local RTID PerCbo - %02u   Remote RTID PerCbo - %02u   Extra - %02u",
                             (2 * CBO_WB_RTIDS), QpiInternalGlobal->UboxRtids, LocalRtidPerCbo, RemoteRtidPerCbo, TotExtraRtids));
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;    Local Base - %02u   Remote Base - %02u   Reallocation Base - %02u",
                             LocalBase, RemoteBase, ReallocBase));


  //
  // Fill the Base & Size for Cbo. Cbo can split its available RTID per HA into two pools. But the spliting is needed only when it has >8 RTIDs
  // or when its RTIDs straddles into xRTID boundary. 
  //
  Idx3 = TotCbo * 2 + 3;
  for (Idx1 = 3; Idx1 < Idx3; Idx1 += 2) { // Start with Cbo 0 Rtid pool
    if (RemoteInXRtidPool == TRUE) {
      if ((LocalBase < (MAX_RTID_SYSTEM / 2)) && ((LocalBase + LocalRtidPerCbo) > (MAX_RTID_SYSTEM / 2))) {
        QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n;    Cbo %02u RTID straddles into xRTID space", ((Idx1 - 3) / 2)));
        TempBase = HandleCboRtidStraddle (host, QpiInternalGlobal, QpiInternalGlobal->RtidBase[0], QpiInternalGlobal->RtidSize[0],
                               &TotExtraRtids, LocalBase, Idx1, LocalRtidPerCbo, ReallocBase);
        QPI_ASSERT ((TempBase > LocalBase), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
        LocalBase = TempBase;
      } else {
        QpiInternalGlobal->RtidBase[0][Idx1] = LocalBase;
        QpiInternalGlobal->RtidSize[0][Idx1] = LocalRtidPerCbo;
        LocalBase = LocalBase + LocalRtidPerCbo;
        if (LocalBase == (MAX_RTID_SYSTEM / 2)) {
          QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n;    Adjusting Cbo %02u base to Reallocation base", (((Idx1 - 3) / 2) + 1)));
          LocalBase = ReallocBase; // Shift the base to right below the Remote RTID space
        }
      }
      QpiInternalGlobal->RtidBase[1][Idx1] = RemoteBase;
      QpiInternalGlobal->RtidSize[1][Idx1] = RemoteRtidPerCbo;
      RemoteBase = RemoteBase + RemoteRtidPerCbo;
    } else {
      // 
      // Repeat the steps same as above except that the role of Local and Remote is swapped.
      // 
      if ((RemoteBase < (MAX_RTID_SYSTEM / 2)) && ((RemoteBase + RemoteRtidPerCbo) > (MAX_RTID_SYSTEM / 2))) {
        QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n;    Cbo %02u RTID straddles into xRTID space", ((Idx1 - 3) / 2)));
        TempBase = HandleCboRtidStraddle (host, QpiInternalGlobal, QpiInternalGlobal->RtidBase[1], QpiInternalGlobal->RtidSize[1],
                               &TotExtraRtids, RemoteBase, Idx1, RemoteRtidPerCbo, ReallocBase);
        QPI_ASSERT ((TempBase > RemoteBase), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
        RemoteBase = TempBase;
      } else {
        QpiInternalGlobal->RtidBase[1][Idx1] = RemoteBase;
        QpiInternalGlobal->RtidSize[1][Idx1] = RemoteRtidPerCbo;
        RemoteBase = RemoteBase + RemoteRtidPerCbo;
        if (RemoteBase == (MAX_RTID_SYSTEM / 2)) {
          QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n;    Adjusting Cbo %02u base to Reallocation base", (((Idx1 - 3) / 2) + 1)));
          RemoteBase = ReallocBase; // Shift the base to right below the Local RTID space
        }
      }
      QpiInternalGlobal->RtidBase[0][Idx1] = LocalBase;
      QpiInternalGlobal->RtidSize[0][Idx1] = LocalRtidPerCbo;
      LocalBase = LocalBase + LocalRtidPerCbo;
    }
  }

  // Extra RTIDs
  QpiInternalGlobal->RtidSize[0][MAX_CBO * 2 + 3]  = TotExtraRtids;

  // 
  // Calculate the HT Base based on these contraints:
  //  - Home Tracker entries must be non overlapping
  //  - EWB can be made by S0 & S1 and it can target both S0 & S1 HAs. EWB always uses non-xRTID 0.
  //     So NID1 base must be always 64 for S0 & S1 HTBASE setting.
  //  - The base value can be either 0 or 64; no other value possible because HSX hard codes ROT at 0 or 64
  //
  if (RemoteInXRtidPool == TRUE) {
    QpiInternalGlobal->HtBase[0] = 0x40404000;
    QpiInternalGlobal->HtBase[1] = 0x00004000;
  } else {
    QpiInternalGlobal->HtBase[0] = 0x00004000;
    QpiInternalGlobal->HtBase[1] = 0x40404000;
  }

  //
  // If a Cbo is assigned >8 RTIDs, then split that into two pools because a pool is allowed to have max 8 RTIDs.
  //
  CheckAndSplitCboRtids (host, QpiInternalGlobal, (UINT8 *)QpiInternalGlobal->RtidBase, (UINT8 *)QpiInternalGlobal->RtidSize);
  DumpRtidAllocationSHT (host, QpiInternalGlobal);

  return;
}

VOID
AllocateRtidDHTCommonHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Allocates RTIDs for HSX configs that are not 1S, ISOC, COD and BT Mode 2

Arguments:

  host  - Pointer to the system host (root) structure
  QpiInternalGlobal  - QPIRC Internal Global data

Returns:

  VOID
  
--*/
{
  UINT8 Cbo, Idx1, Idx2;
  UINT8 TotRtid, TempAsymRtid;
  UINT8 TotCbo, TotCboRtidFirstPool, TotCboRtidSecondPool;
  UINT8 TotPerCboRtid, TotPerCboRtidFirstPool, TotPerCboRtidSecondPool;
  UINT8 TotExtraRtid, TotFirstPoolExtraRtid, TotSecondPoolExtraRtid;
  UINT8 TotFirstPoolAsymExtraRtid, TotSecondPoolAsymExtraRtid;
  UINT8 FirstPoolBase, SecondPoolBase; 
  UINT8 *AsymRtidCboListPtr, AsymRtidCboListSize = 0;

  //
  // The Cbos can operate in double pool mode where they draw RTIDs from two different pools.
  // Depending on the BT mode, a Cbo will use one or two pools for each HA communication.
  // For example, in BT mode 1 it can use two pools for local & remote HA communication whereas
  // in BT mode 0 it will use only one pool. But since the RTID allocation is symmetrical for
  // local & remote HAs in BT Mode 0, 1 & 3, we can calculate the RTID allocation for only one
  // pool for local HA, and use the same setting for the remote HA and the second pool, if applicable.
  // RTID 0 is always assigned for WB and RTIDs 1-3 are used for Ubox. Note that in case a Cbo
  // using RTIDs from two pools, the second pool's RTID 0-3 will go unused. RTIDs 4-63 from
  // pool 0 and pool1 (if applicable) are assigned to Cbos.
  //

  TotCbo = QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc];
  MemCheckBoundary(host, TotCbo, MAX_CBO); //Klocwork 2237: Array 'QpiInternalGlobal->RtidSize[0]' of size 52 may use index value(s) 53..253. Also there is one similar error on line 868

  // Total number of RTIDs available to a socket in local & remote pools

  TotRtid = TOT_RTIDS_PER_POOL * 2;

  // Total number of RTIDs for all CBos in first pool and second pool
  TotCboRtidFirstPool = TOT_RTIDS_PER_POOL - CBO_WB_RTIDS - QpiInternalGlobal->UboxRtids;
  TotCboRtidSecondPool = TOT_RTIDS_PER_POOL - CBO_WB_RTIDS;

  // Total number of RTIDs per CBo in first and second pool
  TotPerCboRtidFirstPool = TotCboRtidFirstPool / TotCbo;
  TotPerCboRtidSecondPool = TotCboRtidSecondPool / TotCbo;
  if (TotPerCboRtidFirstPool > MAX_RTID_PER_CBO_PER_POOL) {
    TotPerCboRtidFirstPool = MAX_RTID_PER_CBO_PER_POOL; // Saturate if it exceeds maximum allowed by h/w
  }
  if (TotPerCboRtidSecondPool > MAX_RTID_PER_CBO_PER_POOL) {
    TotPerCboRtidSecondPool = MAX_RTID_PER_CBO_PER_POOL; // Saturate if it exceeds maximum allowed by h/w
  }

  // Extra RTIDs in first Pool, second pool and total
  TotFirstPoolExtraRtid = TotCboRtidFirstPool % TotCbo;
  TotSecondPoolExtraRtid = TotCboRtidSecondPool % TotCbo;
  TotExtraRtid = TotFirstPoolExtraRtid + TotSecondPoolExtraRtid;
  if ((TotPerCboRtidFirstPool == MAX_RTID_PER_CBO_PER_POOL) || (TotPerCboRtidSecondPool == MAX_RTID_PER_CBO_PER_POOL)) {
    TotFirstPoolExtraRtid = 0;
    TotSecondPoolExtraRtid = 0;
    TotExtraRtid = 0;
  }

  // Total number of RTIDs per Cbo.
  TotPerCboRtid = TotPerCboRtidFirstPool + TotPerCboRtidSecondPool;

  //
  // If Total Extra RTIDs is greater than total Cbos, then each Cbo can be given one additional RTID. 
  // Some Cbo will get it from first pool while rest will get it from the second pool.
  // If there are still some extra RTIDs left, use them for asymmetrical RTID allocation that allows
  // some Cbos to get one extra RTID that will benefit them due to their location in the ring.
  //
  TotFirstPoolAsymExtraRtid = TotFirstPoolExtraRtid;
  TotSecondPoolAsymExtraRtid = TotSecondPoolExtraRtid;
  if (TotExtraRtid >= TotCbo) {
    ++TotPerCboRtid;
    TotFirstPoolAsymExtraRtid = 0; // Asymmetric RTIDs are assigned completely using the second pool RTIDs
    TotSecondPoolAsymExtraRtid = TotSecondPoolExtraRtid - (TotCbo - TotFirstPoolExtraRtid);
  }

  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;    RTIDs Distribution :   TotPerCbo - %02u", TotPerCboRtid));
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;    First Pool         :   TotPerCbo - %02u   TotExtra - %02u   TotAsym - %02u",
                       TotPerCboRtidFirstPool, TotFirstPoolExtraRtid, TotFirstPoolAsymExtraRtid));
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;    Second Pool        :   TotPerCbo - %02u   TotExtra - %02u   TotAsym - %02u",
                       TotPerCboRtidSecondPool, TotSecondPoolExtraRtid, TotSecondPoolAsymExtraRtid));

  //
  // Fill in the size for each Cbo.
  //
  Idx2 = TotCbo * 2 + 3;
  for (Idx1 = 3; Idx1 < Idx2; Idx1 += 2) { // Start with Cbo 0 Rtid pool
    QpiInternalGlobal->RtidSize[0][Idx1] = TotPerCboRtidFirstPool;
    QpiInternalGlobal->RtidSize[0][Idx1 + 1] = TotPerCboRtidSecondPool;

    //
    // If the Cbos are assigned one extra RTID, determine whether it is in first pool or second pool and 
    // adjust the size accordingly.
    //
    if (TotPerCboRtid > (TotPerCboRtidFirstPool + TotPerCboRtidSecondPool)) {
      Cbo = (Idx1 - 3) / 2;
      if (Cbo < TotFirstPoolExtraRtid) {
        ++QpiInternalGlobal->RtidSize[0][Idx1]; // Get the one extra RTID from the first pool's extra RTID
      } else {
        ++QpiInternalGlobal->RtidSize[0][Idx1 + 1]; // Get the one extra RTID from the second pool's extra RTID
      }
    }
  }

  //
  // Asymmetrical RTID allocation.
  //

  // Based on the cpuType, choose the asymmetrical RTIDs list
  //
  if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType != CPU_HSX_EX)) { // For HSX-EP-
    AsymRtidCboListPtr = &HsxEPAsymRtidCboList[0];
    AsymRtidCboListSize = sizeof(HsxEPAsymRtidCboList);
  } else if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType == CPU_HSX_EX)) { // For HSX-EX-
    AsymRtidCboListPtr = &HsxEXAsymRtidCboList[0];
    AsymRtidCboListSize = sizeof(HsxEXAsymRtidCboList);
  } else { // For BDX- all variants
    AsymRtidCboListPtr = &BdxAsymRtidCboList[0];
    AsymRtidCboListSize = sizeof(BdxAsymRtidCboList);
  }

  // First pool
  TempAsymRtid = TotFirstPoolAsymExtraRtid;
  for (Idx2 = 0; (Idx2 < AsymRtidCboListSize) && (TempAsymRtid != 0); Idx2++) {
    Cbo = AsymRtidCboListPtr[Idx2];
    if (Cbo >= TotCbo) {
      continue;
    }

    Idx1 = Cbo * 2 + 3;
    if (QpiInternalGlobal->RtidSize[0][Idx1] < MAX_RTID_PER_CBO_PER_POOL) {
      ++QpiInternalGlobal->RtidSize[0][Idx1];
      --TempAsymRtid;
    }
  }

  // Second pool
  TempAsymRtid = TotSecondPoolAsymExtraRtid;
  //s4968619: HSX Clone : False BT TO for EX-4S - BT entry 510 or 511
  //          for 2nd pool, don't assign to index 62.63 (i.e. entry 510, 511)
  if (TempAsymRtid > 2) {
    TempAsymRtid = TempAsymRtid -2;
  } else {
    TempAsymRtid = 0;
  }
  for (; (Idx2 < AsymRtidCboListSize) && (TempAsymRtid != 0); Idx2++) {
    Cbo = AsymRtidCboListPtr[Idx2];
    if (Cbo >= TotCbo) {
      continue;
    }

    Idx1 = Cbo * 2 + 3;
    if (QpiInternalGlobal->RtidSize[0][Idx1 + 1] < MAX_RTID_PER_CBO_PER_POOL) {
      ++QpiInternalGlobal->RtidSize[0][Idx1 + 1];
      --TempAsymRtid;
    }
  }

  // Fill the Base & Size for WB in first pool
  FirstPoolBase = 0;
  QpiInternalGlobal->RtidBase[0][0] = FirstPoolBase; // WB always assigned RTID 0
  QpiInternalGlobal->RtidSize[0][0] = CBO_WB_RTIDS; // WB always assigned one RTID
  FirstPoolBase = FirstPoolBase + QpiInternalGlobal->RtidSize[0][0];

  // Fill the Base & Size for Ubox in first pool
  QpiInternalGlobal->RtidBase[0][1] = FirstPoolBase;
  QpiInternalGlobal->RtidSize[0][1] = QpiInternalGlobal->UboxRtids;
  FirstPoolBase = FirstPoolBase + QpiInternalGlobal->RtidSize[0][1];

  // Set the second pool base (RTID64 is reserved and can not be used)
  SecondPoolBase = 1;

  //
  // Fill the Base for Cbo in first and second pool based on the size calculated above
  //
  Idx2 = QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc] * 2 + 3;
  for (Idx1 = 3; Idx1 < Idx2; Idx1 += 2) { // Start with Cbo 0 Rtid pool
    QpiInternalGlobal->RtidBase[0][Idx1] = FirstPoolBase;
    FirstPoolBase = FirstPoolBase + QpiInternalGlobal->RtidSize[0][Idx1];

    QpiInternalGlobal->RtidBase[0][Idx1 + 1] = SecondPoolBase;
    SecondPoolBase = SecondPoolBase + QpiInternalGlobal->RtidSize[0][Idx1 + 1];
  }

  //
  // Local and Neighbor/Remote pools are same for this mode; so duplicate the Local pool into Neighbor/Remote pool
  //
  MemCopy ((UINT8 *)QpiInternalGlobal->RtidBase[1], (UINT8 *)QpiInternalGlobal->RtidBase[0], sizeof(QpiInternalGlobal->RtidBase[1]));
  MemCopy ((UINT8 *)QpiInternalGlobal->RtidSize[1], (UINT8 *)QpiInternalGlobal->RtidSize[0], sizeof(QpiInternalGlobal->RtidSize[1]));
  
  DumpRtidAllocation (host, QpiInternalGlobal);

  return;
}

UINT8
NumCBosPerRBT2SCOD (
  UINT8 NumCBos,
  UINT8 RBT
  )
/*++

Routine Description:


Arguments:

  NumCBos  - Number of Cbos, 
  RBT      - Which RBT

Returns:

   Number Cbos per RBT
  
--*/
{
    UINT8   NumCBosPerRBT2SCOD;
    if (RBT == 0) {
        NumCBosPerRBT2SCOD = ((NumCBos + 2) / 2) - (((NumCBos + 3) % 4) / 3);
    } else {
        NumCBosPerRBT2SCOD = (NumCBos / 2) - (((NumCBos + 1) % 4) / 3);
    }
    return NumCBosPerRBT2SCOD;
}


UINT8 
RTIDsPerCBo2SCOD(
  UINT8 NumCBos,
  UINT8 xRTID,
  UINT8 RBT,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:


Arguments:

  NumCBos  - Number of Cbos, 
  xRTID    - Is entry xrtid  
  RBT      - Which RBT
  QpiInternalGlobal  - QPIRC Internal Global data

Returns:

   Rtids per Cbo
  
--*/
{
    
    UINT8   xRTID1Idx, RsvdRTIDs, AvailableRTIDs, RTIDsPerCBo2SCOD, NumCBosPerRBT;
    xRTID1Idx = 64;
    
    if (xRTID == 0) {
        RsvdRTIDs = CBO_WB_RTIDS + QpiInternalGlobal->UboxRtids;
    } else {
        RsvdRTIDs = CBO_WB_RTIDS;
    }

    
    if (xRTID == 0) {
        AvailableRTIDs = xRTID1Idx - RsvdRTIDs;
    } else {
        AvailableRTIDs = MAX_RBT_ENTRIES - xRTID1Idx - RsvdRTIDs;
    }

    NumCBosPerRBT = NumCBosPerRBT2SCOD(NumCBos, RBT);

    RTIDsPerCBo2SCOD = (AvailableRTIDs / NumCBosPerRBT);
    return RTIDsPerCBo2SCOD;
    
}


UINT8
COD2SCBo(
  UINT8 RBTEntry, 
  UINT8 NumCBos, 
  UINT8 xRTID,
  UINT8 RBT,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Allocates RTIDs for HSX BT Mode 2

Arguments:

  RBTEntry - Entry in the RBT, 
  NumCBos  - Number of Cbos, 
  xRTID    - Is entry xrtid
  RBT      - Which RBT
  QpiInternalGlobal  - QPIRC Internal Global data

Returns:

   Cboid this RTID/Rbtentry belongs too
  
--*/
{
    UINT8   xRTID1Idx, MainStart, RsvdRTIDsx0, TotalRsvdRTIDs, RTIDsPerCBox0, RTIDsPerCBox1, RTIDsPerCBo;
    UINT8   UnshiftedCBo, LastCBo, COD2SCBo, ReallocBorder, Index, Remainder, NumCBosThisRBT, ReallocRTID;
    BOOLEAN Capped;
    
    xRTID1Idx = 64;
    
    if (xRTID == 1) {
      RBTEntry = RBTEntry - xRTID1Idx;
    }
    
    if (xRTID == 0) {
      MainStart = CBO_WB_RTIDS + QpiInternalGlobal->UboxRtids;
    } else {
      MainStart = CBO_WB_RTIDS;
    }
    
    RsvdRTIDsx0 = CBO_WB_RTIDS + QpiInternalGlobal->UboxRtids;
    TotalRsvdRTIDs = 2 * CBO_WB_RTIDS + QpiInternalGlobal->UboxRtids;
    
    // CBos are split among CAs by CBo_ID[0]
    // In order to ensure RTIDs are unique for CA,
    // we have to make sure the two CBos with CBo_ID[5:1] aren't assigned
    // The same RTIDs in the two different RBTs
    // That's why we assign the same number of RTIDs regardless of RBT
    // By tying the last parameter here to 0
    RTIDsPerCBox0 = RTIDsPerCBo2SCOD(NumCBos, 0, 0, QpiInternalGlobal);
    RTIDsPerCBox1 = RTIDsPerCBo2SCOD(NumCBos, 1, 0, QpiInternalGlobal);
    Capped = FALSE;
    if (RTIDsPerCBox0 >= MAX_RTID_PER_CBO_PER_POOL) {
        RTIDsPerCBox0 = MAX_RTID_PER_CBO_PER_POOL;
        Capped = TRUE;
    }
    if (RTIDsPerCBox1 >= MAX_RTID_PER_CBO_PER_POOL) {
        RTIDsPerCBox1 = MAX_RTID_PER_CBO_PER_POOL;
        Capped = TRUE;
    }
    if (xRTID == 0) {
        RTIDsPerCBo = RTIDsPerCBox0;
    } else {
        RTIDsPerCBo = RTIDsPerCBox1;
    }
    
    NumCBosThisRBT=NumCBosPerRBT2SCOD(NumCBos, 0);
    
    // Whether we have enough extras across the 2 segments
    // to give an extra to every CBo
    Remainder = MAX_RBT_ENTRIES - TotalRsvdRTIDs - NumCBosThisRBT * (RTIDsPerCBox0 + RTIDsPerCBox1);
    ReallocBorder = 0;
    if ((NumCBosThisRBT <= Remainder) && (Capped == FALSE)) {
        ReallocBorder = (xRTID1Idx - RsvdRTIDsx0) % NumCBosThisRBT;
    }
    
    if (xRTID == 0) {
        ReallocRTID = MainStart + ReallocBorder * (RTIDsPerCBo + 1);
        if (RBTEntry < ReallocRTID) {
            Index = ((RBTEntry - MainStart) / (RTIDsPerCBo + 1));
        } else {
            Index = ((RBTEntry - ReallocRTID) / RTIDsPerCBo) + ReallocBorder;
        }
    } else {
        ReallocRTID = MainStart + ReallocBorder * RTIDsPerCBo;
        if ((RBTEntry < ReallocRTID) || Capped || (ReallocBorder == 0)) {
            Index = ((RBTEntry - MainStart) / RTIDsPerCBo);
        } else {
            Index = ((RBTEntry - ReallocRTID) / (RTIDsPerCBo + 1)) + ReallocBorder;
        }
    }
    
    if (RBT == 0) {
        COD2SCBo = 2 * Index - (Index % 2);
    } else {
    
        // CBos are split among CAs by CBo_ID[0]
        // In order to ensure RTIDs are unique for CA,
        // we have to make sure the two CBos with CBo_ID[5:1] aren't assigned
        // The same RTIDs in the two different RBTs
        // We do this by shifting the assignment "up" the RBT for RBT1
        // And wrapping the first CBo to the bottom of the assignment
        UnshiftedCBo = 2 * (Index + 1) - (Index % 2);
        LastCBo = 2 * ((NumCBosThisRBT - 1) + 1) - ((NumCBosThisRBT - 1) % 2);
        if (UnshiftedCBo == LastCBo) {
            COD2SCBo = 2 * (0 + 1) - (0 % 2);
        } else {
            COD2SCBo = 2 * (Index + 2) - ((Index + 1) % 2);
        }
        
    }

    if ((COD2SCBo >= NumCBos) || ((xRTID == 1) && (RBTEntry == 0))) {
        COD2SCBo = (UINT8) QPI_FAILURE;
    }
    return COD2SCBo;
}


VOID
AllocateRtidDHTCODHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Allocates RTIDs for HSX configs COD

Arguments:

  host  - Pointer to the system host (root) structure
  QpiInternalGlobal  - QPIRC Internal Global data

Returns:

  VOID
  
--*/
{
  UINT8 Idx1, Idx2, Idx3, CurrentCbo;
  UINT8 TotCbo;
  UINT8 TotPerCboRtidFirstPool, TotPerCboRtidSecondPool;
  UINT8 FirstPoolBase, SecondPoolBase; 

  //
  // The Cbos can operate in double pool mode where they draw RTIDs from two different pools.
  // Depending on the BT mode, a Cbo will use one or two pools for each HA communication.
  // In BT mode 3 it can use two pools for local cluster HA communication
  // and 1 pool for remote socket/cluster HA .  Due to this, 1S and 2S RTID maps are the same.
  // RTID 0 is always assigned for WB and RTIDs 1-3 are used for Ubox, and RTID4 is reserved for MCTP. Note that in case a Cbo
  // using RTIDs from two pools, the second pool's RTID 0-4 will go unused. RTIDs 5-63 from
  // pool 0 and pool1 (if applicable) are assigned to Cbos.
  //

  //
  // Treat the clusters individually.   In clustered mode, even Cbos are part of Cluster0 and odd Cbos are part of Cluster1
  // Since Cbo traffic is link interleaved based on Cboid[1], need to offset the Rtids assigned such that Cboid[x0] do not share common Rtids
  //
  TotCbo = (QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc]);

  // Total number of RTIDs available to a socket in local & remote pools

  TotPerCboRtidFirstPool = RTIDsPerCBo2SCOD((QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc]), 0, 0, QpiInternalGlobal);
  TotPerCboRtidSecondPool = RTIDsPerCBo2SCOD((QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc]), 1, 0, QpiInternalGlobal);


  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;    First Pool         :   TotPerCbo - %02u   ",
                       TotPerCboRtidFirstPool));
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;    Second Pool        :   TotPerCbo - %02u   ",
                       TotPerCboRtidSecondPool));

  //
  // First pool stores the normal RTIDS, Second pool holds the xRtids
  // First pool is local and neighbor pool
  // Second pool is local only
  //

  //
  // Fill the Base & Size for WB in first pool
  FirstPoolBase = 0;
  QpiInternalGlobal->RtidBase[0][0] = FirstPoolBase; // WB always assigned RTID 0
  QpiInternalGlobal->RtidSize[0][0] = CBO_WB_RTIDS; // WB always assigned one RTID
  FirstPoolBase = FirstPoolBase + QpiInternalGlobal->RtidSize[0][0];

  // Fill the Base & Size for Ubox in first pool
  QpiInternalGlobal->RtidBase[0][1] = FirstPoolBase;
  QpiInternalGlobal->RtidSize[0][1] = QpiInternalGlobal->UboxRtids;
  FirstPoolBase = FirstPoolBase + QpiInternalGlobal->RtidSize[0][1];

  // Set the second pool base (RTID64 is reserved and can not be used)
  SecondPoolBase = 64 + CBO_WB_RTIDS;

  //
  // Fill the Base for Cbo in first and second pool based on the size calculated above
  //
  //First 64 for RBT0/1
  for (Idx3=0; Idx3 <2; Idx3++) {
    
    for (Idx1=FirstPoolBase; Idx1 < MAX_RBT_ENTRIES/2; Idx1++) {

      CurrentCbo = COD2SCBo(Idx1, TotCbo, 0, Idx3, QpiInternalGlobal);
      if (CurrentCbo == (UINT8) QPI_FAILURE) {
        continue;
      }
      QpiInternalGlobal->RtidBase[0][(CurrentCbo*2)+3] = Idx1;
      QpiInternalGlobal->RtidSize[0][(CurrentCbo*2)+3] = 1;
      for (Idx2=Idx1+1; Idx2 < MAX_RBT_ENTRIES/2; Idx2++) {
        if (CurrentCbo == COD2SCBo(Idx2, TotCbo, 0, Idx3, QpiInternalGlobal)) {
          QpiInternalGlobal->RtidSize[0][(CurrentCbo*2)+3] = QpiInternalGlobal->RtidSize[0][(CurrentCbo*2)+3] + 1;
        } else {
          break;
        }
      }
    Idx1 = Idx2-1;
    }
  }

  //Xrtids for RBT0/1
  for (Idx3=0; Idx3 <2; Idx3++) {
    for (Idx1=SecondPoolBase; Idx1 < MAX_RBT_ENTRIES; Idx1++) {
      
      CurrentCbo = COD2SCBo(Idx1, TotCbo, 1, Idx3, QpiInternalGlobal);
      if (CurrentCbo == (UINT8) QPI_FAILURE) {
        continue;
      }
      QpiInternalGlobal->RtidBase[0][(CurrentCbo*2)+4] = Idx1-64;
      QpiInternalGlobal->RtidSize[0][(CurrentCbo*2)+4] = 1;
      for (Idx2=Idx1+1; Idx2 < MAX_RBT_ENTRIES; Idx2++) {
        if (CurrentCbo == COD2SCBo(Idx2, TotCbo, 1, Idx3, QpiInternalGlobal)) {
          QpiInternalGlobal->RtidSize[0][(CurrentCbo*2)+4] = QpiInternalGlobal->RtidSize[0][(CurrentCbo*2)+4] + 1;
        } else {  
          Idx1 = Idx2-1;
          break;
        }
      }
    }
  }
  
  //
  // Local and Neighbor/Remote pools are same for this mode; so duplicate the Local pool into Neighbor/Remote pool
  //
  MemCopy ((UINT8 *)QpiInternalGlobal->RtidBase[1], (UINT8 *)QpiInternalGlobal->RtidBase[0], sizeof(QpiInternalGlobal->RtidBase[1]));
  MemCopy ((UINT8 *)QpiInternalGlobal->RtidSize[1], (UINT8 *)QpiInternalGlobal->RtidSize[0], sizeof(QpiInternalGlobal->RtidSize[1]));
  DumpRtidAllocation (host, QpiInternalGlobal);

  return;
}

VOID
AllocateRtid8SHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Allocates RTIDs for HSX BT Mode 2

Arguments:

  host  - Pointer to the system host (root) structure
  QpiInternalGlobal  - QPIRC Internal Global data

Returns:

  VOID
  
--*/
{
  UINT8 Cbo;
  UINT8 Idx1, Idx2;
  UINT8 LocalRtidTotFirstPool = 0, LocalRtidTotSecondPool = 0, RemoteRtidTot = 0;
  UINT8 LocalRtidCboFirstPool = 0, LocalRtidCboSecondPool = 0, RemoteRtidCbo = 0;
  UINT8 LocalRtidPerCboFirstPool = 0, LocalRtidPerCboSecondPool = 0, RemoteRtidPerCbo = 0;
  UINT8 LocalBaseFirstPool = 0, LocalBaseSecondPool = 0, RemoteBase = 0;

  UINT8 LocalExtraRtids;
  UINT8 LocalExtraRtidsPerCbo[MAX_CBO] = {0};
  UINT8 RemoteExtraRtids;
  UINT8 RemoteExtraRtidsPerCbo[MAX_CBO] = {0};

  //
  // For 8S, the Cbos operate in a mode where they use 2 pools for local HA communication
  // and single pool for all remote HA communication.
  // RTID 0 is always assigned for WB and RTIDs 1-3 are used for Ubox.
  //

  LocalRtidTotFirstPool = TOT_RTIDS_PER_LOCAL_POOL_HSX;
  LocalRtidTotSecondPool = TOT_RTIDS_PER_SECOND_POOL_HSX;

  if (QpiInternalGlobal->BtMode2Alt == 1) {
      LocalRtidTotFirstPool = TOT_RTIDS_PER_LOCAL_POOL_ALT_HSX;
  }

  RemoteRtidTot = TOT_RTIDS_PER_REMOTE_POOL_HSX;

  // Total number of RTIDs for all CBos
  if (QpiInternalGlobal->BtMode2Alt == 0) {
    LocalRtidCboFirstPool = LocalRtidTotFirstPool - CBO_WB_RTIDS - QpiInternalGlobal->UboxRtids;
  } else {
    LocalRtidCboFirstPool = LocalRtidTotFirstPool - CBO_WB_RTIDS;  //Local UBOX RTIDs not needed for BtMode2 Alt-1 
  }

  LocalRtidCboSecondPool = LocalRtidTotSecondPool - CBO_WB_RTIDS;
  if (QpiInternalGlobal->BtMode2Alt == 1) {
    LocalRtidCboSecondPool = LocalRtidCboFirstPool;
  }
  RemoteRtidCbo = RemoteRtidTot - CBO_WB_RTIDS - QpiInternalGlobal->UboxRtids;

  // Total number of RTIDs - per CBo per pool
  LocalRtidPerCboFirstPool = LocalRtidCboFirstPool / QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc];
  if (LocalRtidPerCboFirstPool > MAX_RTID_PER_CBO_PER_POOL) {
    LocalRtidPerCboFirstPool = MAX_RTID_PER_CBO_PER_POOL; // Saturate if it exceeds maximum allowed by h/w
  }
  LocalRtidPerCboSecondPool = LocalRtidCboSecondPool / QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc];
  if (LocalRtidPerCboSecondPool > MAX_RTID_PER_CBO_PER_POOL) {
    LocalRtidPerCboSecondPool = MAX_RTID_PER_CBO_PER_POOL; // Saturate if it exceeds maximum allowed by h/w
  }
  RemoteRtidPerCbo = RemoteRtidCbo / QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc];
  if (RemoteRtidPerCbo > MAX_RTID_PER_CBO_PER_POOL) {
    RemoteRtidPerCbo = MAX_RTID_PER_CBO_PER_POOL; // Saturate if it exceeds maximum allowed by h/w
  }

  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;    RTIDs Distribution Local First Pool:     Tot - %02u   WB - %02u   Ubox - %02u   TotCbo - %02u   PerCbo - %02u",
                           LocalRtidTotFirstPool, CBO_WB_RTIDS, QpiInternalGlobal->UboxRtids, LocalRtidCboFirstPool, LocalRtidPerCboFirstPool));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;    RTIDs Distribution Local Second Pool:    Tot - %02u                         TotCbo - %02u   PerCbo - %02u",
                           LocalRtidTotSecondPool, LocalRtidCboSecondPool, LocalRtidPerCboSecondPool));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;    RTIDs Distribution Remote Pool:          Tot - %02u   WB - %02u   Ubox - %02u   TotCbo - %02u   PerCbo - %02u",
                           RemoteRtidTot, CBO_WB_RTIDS, QpiInternalGlobal->UboxRtids, RemoteRtidCbo, RemoteRtidPerCbo));

  //
  // For BtMode2 Alt-1 mode, try to use any 'extra' RTIDs by distributing them to various CBos.
  // For all other BT modes, no 'extra' RTIDs are used.
  //
  Idx1 = sizeof(LocalExtraRtidsPerCbo) / sizeof(LocalExtraRtidsPerCbo[0]);
  for (Cbo=0; Cbo < Idx1; Cbo++) {
    LocalExtraRtidsPerCbo[Cbo]  = 0;  //Default value: No extra local  RTIDs for this Cbo
    RemoteExtraRtidsPerCbo[Cbo] = 0;  //Default value: No extra remote RTIDs for this Cbo
  }
  
  // Allocate 'extra' BtMode2 Alt-1 RTIDs, if any
  if (QpiInternalGlobal->BtMode2Alt == 1) {
    // Calc total # of extra local RTIDs
    if (LocalRtidPerCboFirstPool >= MAX_RTID_PER_CBO_PER_POOL) {  //Extra RTIDs not usable if RTIDs per Cbo is already maxed out
      LocalExtraRtids = 0;         //Each Cbo already allocated max # of RTIDs allowed by HW
    } else {
      LocalExtraRtids = LocalRtidCboFirstPool % QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc];
      if (LocalExtraRtids >= 2) {  //Ensure we have at least 2 'extra' RTIDs to distribute 
        LocalExtraRtids -= 2;      //BT entries 510-511 are unsable so RTIDs 62-63 are unusable (in both 1st and 2nd pools)
      } else {
        LocalExtraRtids = 0;       //Fewer than 2 'extra' RTIDs are not usable
      }
    }
    QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;    BtMode2 Alt-1:  Total of %d extra RTID(s) per local pool.   Allocated to Cbo(s): ", LocalExtraRtids));

    // Distribute extra local RTIDs to each Cbo based on the Cbo prioritization list.
    for (Idx1=0; (LocalExtraRtids != 0) && (Idx1 < MAX_HSX_CBO); Idx1++) {
      Cbo = HsxEXAsymRtidCboList[Idx1];  //Lookup next highest priority Cbo
      if (Cbo < QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc]) {  //Target Cbo actually exists
        LocalExtraRtidsPerCbo[Cbo]++;
        LocalExtraRtids--;
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "  %2d", Cbo));
      }
    }//For each Cbo

    // Calc total # of extra remote RTIDs
    if (RemoteRtidPerCbo >= MAX_RTID_PER_CBO_PER_POOL) {  //Extra RTIDs not usable if RTIDs per Cbo is already maxed out
      RemoteExtraRtids = 0;         //Each Cbo already allocated max # of RTIDs allowed by HW
    } else {
      RemoteExtraRtids = RemoteRtidCbo % QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc];
    }
    QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;    BtMode2 Alt-1:  Total of %d extra RTID(s) for remote pool.  Allocated to Cbo(s): ", RemoteExtraRtids));

    // Distribute extra remote RTIDs to each Cbo based on the Cbo prioritization list.
    for (Idx1=0; (RemoteExtraRtids != 0) && (Idx1 < MAX_HSX_CBO); Idx1++) {
      Cbo = HsxEXAsymRtidCboList[Idx1];  //Lookup next highest priority Cbo
      if (Cbo < QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc]) {  //Target Cbo actually exists
        RemoteExtraRtidsPerCbo[Cbo]++;
        RemoteExtraRtids--;
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "  %2d", Cbo));
      }
    }//For each Cbo
  }//BtMode2 Alt-1

  // Initialize the Pool base values
  LocalBaseFirstPool = RemoteBase = 0;
  LocalBaseSecondPool = 1; // RTID0 in second pool is unused

  //
  // Fill the Base & Size for WB in Local First Pool & Remote Pool.
  //
  QpiInternalGlobal->RtidBase[0][0] = LocalBaseFirstPool; // WB always assigned RTID 0
  QpiInternalGlobal->RtidSize[0][0] = CBO_WB_RTIDS; // WB always assigned one RTID
  LocalBaseFirstPool = LocalBaseFirstPool + QpiInternalGlobal->RtidSize[0][0];
  QpiInternalGlobal->RtidBase[1][0] = RemoteBase; // WB always assigned RTID 0
  QpiInternalGlobal->RtidSize[1][0] = CBO_WB_RTIDS; // WB always assigned one RTID
  RemoteBase = RemoteBase + QpiInternalGlobal->RtidSize[1][0];

  //
  // Fill the Base & Size for Ubox in Local First Pool & Remote Pool.
  // Ubox RTIDs are not assigned from Local Second Pool.
  // Note: For BtMode2 Alt-1, no local RTIDs needed for UBOX.
  if (QpiInternalGlobal->BtMode2Alt != 1) {
    QpiInternalGlobal->RtidBase[0][1] = LocalBaseFirstPool; // Ubox always assigned RTID Base 1
    QpiInternalGlobal->RtidSize[0][1] = QpiInternalGlobal->UboxRtids; // Ubox always assigned 3 RTIDs
    LocalBaseFirstPool = LocalBaseFirstPool + QpiInternalGlobal->RtidSize[0][1];
  }
  QpiInternalGlobal->RtidBase[1][1] = RemoteBase; // Ubox always assigned RTID Base 1
  QpiInternalGlobal->RtidSize[1][1] = QpiInternalGlobal->UboxRtids; // Ubox always assigned 3 RTIDs
  RemoteBase = RemoteBase + QpiInternalGlobal->RtidSize[1][1];

  //
  // For BTMode2 Alt-1 mode, Cbo allocations for 1st and 2nd pools must match.
  // So, set the starting Cbo base of the 2nd pool to match the 1st pool.
  //
  if (QpiInternalGlobal->BtMode2Alt == 1) {
    LocalBaseSecondPool = LocalBaseFirstPool;
  }

  //
  // Fill the Base & Size for Cbo. Only the Local Pool credits are split into two pools and assigned
  // to a Cbo. In Remote Pool credits are assigned using single pool only.
  //
  Idx2 = QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc] * 2 + 3;
  for (Cbo=0,Idx1=3; Idx1 < Idx2; Cbo++, Idx1 += 2) { // Start with Cbo 0 Rtid pool
    // Local Pool, First Pool
    QpiInternalGlobal->RtidBase[0][Idx1] = LocalBaseFirstPool;
    QpiInternalGlobal->RtidSize[0][Idx1] = LocalRtidPerCboFirstPool + LocalExtraRtidsPerCbo[Cbo];
    LocalBaseFirstPool = LocalBaseFirstPool + QpiInternalGlobal->RtidSize[0][Idx1];

    // Local Pool, Second Pool
    QpiInternalGlobal->RtidBase[0][Idx1 + 1] = LocalBaseSecondPool;
    QpiInternalGlobal->RtidSize[0][Idx1 + 1] = LocalRtidPerCboSecondPool + LocalExtraRtidsPerCbo[Cbo];
    LocalBaseSecondPool = LocalBaseSecondPool + QpiInternalGlobal->RtidSize[0][Idx1 + 1];

    // Remote Pool First Pool only; second pool is n/a.
    QpiInternalGlobal->RtidBase[1][Idx1] = RemoteBase;
    QpiInternalGlobal->RtidSize[1][Idx1] = RemoteRtidPerCbo + RemoteExtraRtidsPerCbo[Cbo];
    RemoteBase = RemoteBase + QpiInternalGlobal->RtidSize[1][Idx1];
  }

  DumpRtidAllocation (host, QpiInternalGlobal);

  return;
}


STATIC VOID
AllocateRtidFor8SOr4SCODBdx (
  struct sysHost             *host,
  UINT8                       NumCbos,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
{
  UINT8 Idx1, Idx2;
  // Notes :- We need to partition the following RTIDs amongst the Cbos,
  // [1] Local HA xRTID=0 and RTID range [0,53]
  // [2] Local HA xRTID=1 and RTID range [0,63]
  // [3] Local HA xRTID=0 and RTID range [54,63]
  // [4] Remote HA xRTID=0 and RTID range [0,53] Same as [1]
  //
  // LocalRtidTotFirstPool correspond to [1] and this overlaps with RemoteRtidTot which is [4].
  // LocalRtidTotSecondPool correspond to [2] and LocalRtidTotSpecialPool correspond to [3].
  //
  // As [2] and [3] are solely for local HA, they can be clubbed together during allocation. Any
  // extra RTIDs from [1] can be added to [2] and [3] during the allocation also.
  
  UINT8 LocalRtidTotFirstPool, LocalRtidTotSecondPool, LocalRtidTotSpecialPool, RemoteRtidTot;
  UINT8 LocalRtidCboFirstPool, LocalRtidCboSecondPool, LocalRtidCboSpecialPool, LocalRtidCbo, RemoteRtidCbo;
  UINT8 LocalRtidPerCboFirstPool = 0, LocalRtidPerCbo = 0, RemoteRtidPerCbo = 0;
  // In BtMode-2, we have 3 pools per Cbo - Pool[0-F] which are used for local as well as remote HA.
  // Pool [G,H] - local HAs only and maps to [2], Pool [I,J] - local HAs only and maps to [1] and [3].
  // LocalBaseFirstPool correspond to [1] and RemoteBase correspond to [4]. Both are same though.
  // LocalBaseSecondPool correspond to combined [2] and [3] with extra RTIDs from [1], if available. 
  UINT8 LocalBaseFirstPool = 0, LocalBaseSecondPool = 0, RemoteBase = 0;
  UINT8 RemoteRTIDLimit, LocalBaseSecondPoolLimit, ExtraRtidsFirstPool;
  
  // Assign the maximum RTIDs which are available each of the poolsets for BDX BtMode-2
  RemoteRtidTot = LocalRtidTotFirstPool = TOT_RTIDS_PER_LOCAL_FIRST_POOL_BDX;
  LocalRtidTotSecondPool = TOT_RTIDS_PER_LOCAL_SECOND_POOL_BDX;
  LocalRtidTotSpecialPool = TOT_RTIDS_PER_LOCAL_SPECIAL_POOL_BDX;
  
  // Find the net RTIDs which can be allocated to the Cbos
  RemoteRtidCbo = LocalRtidCboFirstPool = (LocalRtidTotFirstPool - CBO_WB_RTIDS - QpiInternalGlobal->UboxRtids);
  LocalRtidCboSecondPool =  LocalRtidTotSecondPool - CBO_WB_RTIDS;
  LocalRtidCboSpecialPool = LocalRtidTotSpecialPool;
  
  // Start by allocating the RTIDs for remote sockets i.e. RemoteRtidTot and LocalRtidTotFirstPool
  // Find the 'Remote RTID Limit' which is the base for second pool. Unused RTIDs in [1] can be used
  // along with [2] and [3].
  RemoteRtidPerCbo = LocalRtidPerCboFirstPool = RemoteRtidCbo / NumCbos;  
    
  if (RemoteRtidPerCbo > MAX_RTID_PER_CBO_PER_POOL)
    RemoteRtidPerCbo = LocalRtidPerCboFirstPool = MAX_RTID_PER_CBO_PER_POOL;    // Saturate at Max RTID per pool (3 bit restrict the max to 8)
  
  // Calculate the 'Remote RTID Limit', which acts as the base RTID for [2] and [3]
  RemoteRTIDLimit = CBO_WB_RTIDS + QpiInternalGlobal->UboxRtids + (RemoteRtidPerCbo * NumCbos);
    
  // Add in the extra RTIDs from [1], if available
  ExtraRtidsFirstPool = TOT_RTIDS_PER_LOCAL_FIRST_POOL_BDX - RemoteRTIDLimit;
  
  // Treating the LocalRtidTotSpecialPool and LocalRtidTotSecondPool as one lot during the allocation
  // with extra RTIDs any from [1] pool
  LocalRtidCbo = LocalRtidCboSecondPool + LocalRtidCboSpecialPool + ExtraRtidsFirstPool;
  
  // Brickland BDX 4970186 : CLONE from broadwell_server.bugeco: PUSH from haswell_server: Clone from HSX Sighting:False BT TO for EX-4S - BT entry 510 or 511
  // Brickland BDX 4971307 : BT TO on enabling 4S EX COD with 22 Cbo parts
  // Remove 2 RTIDs so that BTIDs 510, 511 are not used in the system
  LocalRtidCbo -= 2;
  
  // Find the RTIDs per Cbo for local socket - This covers [2] + [3] + extras from [1] as mentioned in Notes
  //
  LocalRtidPerCbo = LocalRtidCbo / NumCbos;
  
  if (LocalRtidPerCbo > MAX_RTID_PER_CBO_PER_POOL) {
    LocalRtidPerCbo = MAX_RTID_PER_CBO_PER_POOL;    // Saturate at Max RTID per pool (3 bit restrict the max to 8)
  }
     
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;    RTIDs Distribution Local First Pool:     Tot - %02u   WB - %02u   Ubox - %02u   TotCbo - %02u   PerCbo - %02u  Extra given - %02u",
                           LocalRtidTotFirstPool, CBO_WB_RTIDS, QpiInternalGlobal->UboxRtids, LocalRtidCboFirstPool, LocalRtidPerCboFirstPool, ExtraRtidsFirstPool));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;    RTIDs Distribution Local Second Pool :   Tot - %02u                         TotCbo - %02u   PerCbo - %02u",
                           LocalRtidTotSecondPool + LocalRtidTotSpecialPool + ExtraRtidsFirstPool, LocalRtidCbo, LocalRtidPerCbo));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;    RTIDs Distribution Remote Pool:          Tot - %02u   WB - %02u   Ubox - %02u   TotCbo - %02u   PerCbo - %02u  Extra unused - %02u",
                           RemoteRtidTot, CBO_WB_RTIDS, QpiInternalGlobal->UboxRtids, RemoteRtidCbo, RemoteRtidPerCbo, ExtraRtidsFirstPool));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;    Note: RTIDs Distribution Local Second Pool includes extra RTIDs from local first pool, xRTID=1 pool and xRTID=0 [54-63] pool"));

  // Initialize the Pool base values
  LocalBaseFirstPool = RemoteBase = 0;
  LocalBaseSecondPool = RemoteRTIDLimit;
  
  //
  // Using RtidBase[0][x] for local pools(both xRTID=0 and xRTID=1), RtidBase[1][x] for remote xRTID=0 pool and
  // RtidBase[2][x] for local xRTID=0 special pool.
  // Fill the Base & Size for WB in Local First Pool & Remote Pool.
  //
  QpiInternalGlobal->RtidBase[0][0] = LocalBaseFirstPool; // WB always assigned RTID 0
  QpiInternalGlobal->RtidSize[0][0] = CBO_WB_RTIDS; // WB always assigned one RTID
  LocalBaseFirstPool += QpiInternalGlobal->RtidSize[0][0];
  QpiInternalGlobal->RtidBase[1][0] = RemoteBase; // WB always assigned RTID 0
  QpiInternalGlobal->RtidSize[1][0] = CBO_WB_RTIDS; // WB always assigned one RTID
  RemoteBase += QpiInternalGlobal->RtidSize[1][0];

  //
  // Fill the Base & Size for Ubox in Local First Pool & Remote Pool.
  // Ubox RTIDs are not assigned from Local Second Pool.
  //
  QpiInternalGlobal->RtidBase[0][1] = LocalBaseFirstPool; // Ubox always assigned RTID Base 1
  QpiInternalGlobal->RtidSize[0][1] = QpiInternalGlobal->UboxRtids; // Ubox always assigned 3 RTIDs
  LocalBaseFirstPool += QpiInternalGlobal->RtidSize[0][1];
  QpiInternalGlobal->RtidBase[1][1] = RemoteBase; // Ubox always assigned RTID Base 1
  QpiInternalGlobal->RtidSize[1][1] = QpiInternalGlobal->UboxRtids; // Ubox always assigned 3 RTIDs
  RemoteBase += QpiInternalGlobal->RtidSize[1][1];

  //
  // Fill the Base & Size for Cbo. Only the Local Pool credits are split into two pools and assigned
  // to a Cbo. In Remote Pool credits are assigned using single pool only.
  //
  Idx2 = NumCbos * 2 + 3;
  for (Idx1 = 3; Idx1 < Idx2; Idx1 += 2) { // Start with Cbo 0 Rtid pool
    // Local Pool, First Pool (xRTID=0 pool)
    QpiInternalGlobal->RtidBase[0][Idx1] = LocalBaseFirstPool;
    QpiInternalGlobal->RtidSize[0][Idx1] = LocalRtidPerCboFirstPool;
    LocalBaseFirstPool += QpiInternalGlobal->RtidSize[0][Idx1];
    
    // Remote Pool First Pool only; second pool is n/a.
    QpiInternalGlobal->RtidBase[1][Idx1] = RemoteBase;
    QpiInternalGlobal->RtidSize[1][Idx1] = RemoteRtidPerCbo;
    RemoteBase += QpiInternalGlobal->RtidSize[1][Idx1];
    
    // Local Pool, Second Pool's last RTID
    LocalBaseSecondPoolLimit = LocalBaseSecondPool + LocalRtidPerCbo;
    
    // Check whether we straddle the 63-64 boundary line. We handle for when we are in xRTID=0 or xRTID=1 or
    // we straddle across both.
    if ((LocalBaseSecondPool < (MAX_RBT_ENTRIES / 2))&& (LocalBaseSecondPoolLimit >= (MAX_RBT_ENTRIES / 2))) {
      // Since we straddle the boundary, split it across the 2 pools
        QpiInternalGlobal->RtidBase[2][Idx1] = LocalBaseSecondPool;
        QpiInternalGlobal->RtidSize[2][Idx1] = ((MAX_RBT_ENTRIES / 2) - 1) - LocalBaseSecondPool + 1;  // 63 - LocalBaseSecondPool + 1
        QpiInternalGlobal->RtidBase[0][Idx1+1] = RTID_BASE_OFFSET((MAX_RBT_ENTRIES / 2) + 1);          // Always points at RTID 65 for xRTID=1 to begin as 64 is reserved
        QpiInternalGlobal->RtidSize[0][Idx1+1] = LocalRtidPerCbo - QpiInternalGlobal->RtidSize[2][Idx1];
        LocalBaseSecondPool = ((MAX_RBT_ENTRIES / 2) + 1) + QpiInternalGlobal->RtidSize[0][Idx1+1];    // Points to (65 + Size of second pool which straddled across)
    } else {
      if (LocalBaseSecondPool > (MAX_RBT_ENTRIES / 2)) {
        // This is still in xRTID=1 pool [2], use Idx1 + 1, adjust the base offset by 64
        QpiInternalGlobal->RtidBase[0][Idx1+1] = RTID_BASE_OFFSET(LocalBaseSecondPool);
        QpiInternalGlobal->RtidSize[0][Idx1+1] = LocalRtidPerCbo;
      } else {
        // This is still in xRTID=0 special pool [3], use Idx1 only
        QpiInternalGlobal->RtidBase[2][Idx1] = LocalBaseSecondPool;
        QpiInternalGlobal->RtidSize[2][Idx1] = LocalRtidPerCbo;   
      }
      LocalBaseSecondPool += LocalRtidPerCbo;
    } // check for straddling
  } // loops
    
  return;
}


VOID
AllocateRtid8SBdx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Allocates RTIDs for BDX BT Mode 2 for 8S topologies

Arguments:

  host  - Pointer to the system host (root) structure
  QpiInternalGlobal  - QPIRC Internal Global data

Returns:

  VOID
  
--*/
{
  UINT8 NumCbos = QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc];
  AllocateRtidFor8SOr4SCODBdx(host, NumCbos, QpiInternalGlobal);
  DumpRtidAllocationBdxBtMode2(host, QpiInternalGlobal);
  
  return;
}

#ifdef QPI_4S_COD_FEATURE_PROTOTYPE
VOID  
AllocateRtids4SCODHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Allocates RTIDs for HSX BT Mode 2 for 4S-COD Feature

Arguments:

  host  - Pointer to the system host (root) structure
  QpiInternalGlobal  - QPIRC Internal Global data

Returns:

  VOID
  
--*/
{
  // 4S COD allocation for HSX can be indeed be handled by 8S HSX case
  // So making a call to 8S RTID allocation routine
  AllocateRtid8SHsx ( host, QpiInternalGlobal);
  return;
}
#endif // QPI_4S_COD_FEATURE_PROTOTYPE
 
  
VOID  
AllocateRtids4SCODBdx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Allocates RTIDs for BDX BT Mode 2 for 4S-COD Feature

Arguments:

  host  - Pointer to the system host (root) structure
  QpiInternalGlobal  - QPIRC Internal Global data

Returns:

  VOID
  
--*/
{
  UINT8 NumCbos;

  NumCbos = QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc];
  MemCheckBoundary(host, NumCbos, MAX_CBO); //Klocwork 4840: Array 'QpiInternalGlobal->RtidBase[1]' of size 52 may use index value(s) 2..256
  
  if ((NumCbos % 2) == 1) {
    QPI_ASSERT (FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);  // PCODE should have notified us in CAPIDs or bug in priming
  }
  
  // In BDX 4S-COD, the Cbos are divided into even and odd clusters and the RBT logic allows the even cluser
  // Cbo and the odd cluster Cbo (which have same logical CboId[5:1]) to share the same set of RTIDs. The RBT
  // uses the DNID[3] to derive the net ring stop. Thus, we divide the RTIDs among Cbos of one cluster and 
  // use the same for other cluster since they share the RTIDs.
  NumCbos = NumCbos / 2;
    
  AllocateRtidFor8SOr4SCODBdx(host, NumCbos, QpiInternalGlobal);
  DumpRtidAllocationBdxBtMode2(host, QpiInternalGlobal);
  return;
}

UINT8
HandleCboRtidStraddle (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8                      *RtidBase,
  UINT8                      *RtidSize,
  UINT8                      *ExtraRtids,
  UINT8                      CboRtidBase,
  UINT8                      CboIndex,
  UINT8                      RtidPerCbo,
  UINT8                      ReallocBase
  )
/*++

Routine Description:
  Routine to handle the placement of RTIDs for Cbo that straddles the xRTID boundary. Since such straddling requires
  splitting the RTID into 2 pools, this routine handles the splitting. In some low core count SKUs it is possible that
  they will have enoguh RTIDs that will result in 3 pools needed for consuming all the RTIDs. However since the Si allows
  only 2 pools per Cbo per HA, we need to sacrifice one of the pools. We need to sacrifice the one with least number of 
  RTIDs to reduce perf penality. Since in general the low core count SKUs have more RTIDs the perf loss can be made almost
  nill by properly choosing the pool to be discarded.

Arguments:
  RtidBase - Base to be upated
  RtidSize - Sizee to be upated
  ExtraRtids - How many extra RTIDs are available to reduce the loss by pushing the RTIDs entirely/partially into the xRTID space
  CboRtidBase - Starting RTID base for this Cbo
  CboIndex - Index into RtidBase/RtidSize array
  RtidPerCbo - RTIDs per Cbo to be allocated
  ReallocBase - Reallocation base to be used to place the RTIDs in xRTID space

Returns:
  Adjusted RTID base to be used for next Cbo.

 
--*/
{
  UINT8   Split1Size, Split2Size, Split3Size;


  // 
  // Split the RTID into 3 groups; 3rd group size could be 0.
  // 
  Split1Size = (MAX_RTID_SYSTEM / 2) - CboRtidBase;
  if (Split1Size > MAX_RTID_PER_POOL) {
    Split2Size = Split1Size - MAX_RTID_PER_POOL;
    Split1Size = MAX_RTID_PER_POOL;
    Split3Size = RtidPerCbo - Split1Size - Split2Size;
    QPI_ASSERT ((Split2Size < MAX_RTID_PER_POOL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
    QPI_ASSERT ((Split3Size < MAX_RTID_PER_POOL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  } else {
    Split2Size = RtidPerCbo - Split1Size;
    if (Split2Size > MAX_RTID_PER_POOL) {
      Split2Size = MAX_RTID_PER_POOL;
      Split3Size = RtidPerCbo - Split1Size - Split2Size;
      QPI_ASSERT ((Split3Size < MAX_RTID_PER_POOL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
    } else {
      Split3Size = 0;
    }
  }


  if (Split3Size == 0) {
    //
    // If the 3rd group size is 0, then place the first group in non-xRTID space and the second group in xRTID space
    //
    QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;      RTIDs split into two pools of size %u and %u with no loss of RTIDs", Split1Size, Split2Size));
    RtidBase[CboIndex] = CboRtidBase;
    RtidSize[CboIndex] = Split1Size;
    CboRtidBase = ReallocBase; // Shift the base to right below the Remote RTID space
    RtidBase[CboIndex + 1] = CboRtidBase;
    RtidSize[CboIndex + 1] = Split2Size;
    CboRtidBase = CboRtidBase + Split2Size;
  } else {
    //
    // The 3rd group size is non-zero. Check if there are enough extra RTIDs available so that group 1 or 2 RTIDs can be placed 
    // in xRTID space. If not we need to sacrifice one of the groups. Choose the one that has least number of RTIDs.
    //
    QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;      RTIDs split into three pools of size %u, %u and %u", Split1Size, Split2Size, Split3Size));
    if ((Split1Size == MAX_RTID_PER_POOL) && (Split2Size <= *ExtraRtids)) {
      // 
      // This case implies most of the RTIDs are in non-xRTID space and there are enough extra RTIDs available in xRTID space
      // to move the Second group to xRTID space. Do the move by merging it with the Third group which must be in xRTID space already.
      // 
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;        Sufficient extra RTIDs are available to move the Second Pool into xRTID space. No loss of RTIDs for the CBo."));
      QPI_ASSERT (((Split2Size + Split3Size) <= MAX_RTID_PER_POOL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
      RtidBase[CboIndex] = CboRtidBase;
      RtidSize[CboIndex] = Split1Size;
      CboRtidBase = ReallocBase; // Shift the base to right below the Remote RTID space
      RtidBase[CboIndex + 1] = CboRtidBase;
      RtidSize[CboIndex + 1] = Split2Size + Split3Size;
      CboRtidBase = CboRtidBase + Split2Size + Split3Size;
      *ExtraRtids = *ExtraRtids - Split2Size;
    } else if ((Split2Size == MAX_RTID_PER_POOL) && (Split1Size <= *ExtraRtids)) {
      // 
      // This case implies most of the RTIDs are in xRTID space and there are enough extra RTIDs available in xRTID space
      // to move the First group to xRTID space. Do the move by merging it with the Second & Third group which must be in xRTID space already.
      // We don't need to split the RTIDs into 2 pools at this point since for this case the RTIDs are entirely in xRTID pool space 
      // and splitting of the RTIDs into 2 pools will be handled later.
      // 
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;        Sufficient extra RTIDs are available to move all the RTIDs into xRTID space. No loss of RTIDs for the CBo."));
      RtidBase[CboIndex] = ReallocBase; // Shift the base to right below the Remote RTID space
      RtidSize[CboIndex] = RtidPerCbo;
      CboRtidBase = ReallocBase + RtidPerCbo;
      *ExtraRtids = *ExtraRtids - Split1Size;
    } else if (*ExtraRtids == 0) {
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;        No Extra RTIDs are available avoid the Cbo RTID loss."));
      if (Split1Size == MAX_RTID_PER_POOL) {
        // 
        // This case implies most of the RTIDs are in non-xRTID space. Check which one among split 2 & split 3 are small and sacrifice it.
        // 
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;        Most RTIDs of the Cbo are in non-xRTID space"));
        if (Split2Size >= Split3Size) {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;        Cbo loses %u RTIDs from the Third pool", Split3Size));
          QPI_ASSERT ((((MAX_RTID_SYSTEM / 2) - CboRtidBase) == (Split1Size + Split2Size)), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
          RtidBase[CboIndex] = CboRtidBase;
          RtidSize[CboIndex] = Split1Size + Split2Size; // Splitting RTIDs that doesn't cross xRTID boundary will be handled later
          CboRtidBase = ReallocBase; // Shift the base to right below the Remote RTID space; sacrificed RTIDs go to the end of the xRTID space.
        } else {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;        Cbo loses %u RTIDs from the Second pool", Split2Size));
          RtidBase[CboIndex] = CboRtidBase;
          RtidSize[CboIndex] = Split1Size;
          CboRtidBase = ReallocBase; // Shift the base to right below the Remote RTID space; sacrificed RTIDs go to the end of the NonxRTID space.
          RtidBase[CboIndex + 1] = CboRtidBase;
          RtidSize[CboIndex + 1] = Split3Size;
          CboRtidBase = CboRtidBase + Split3Size;
        }
      } else {
        // 
        // This case implies most of the RTIDs are in xRTID space. Check which one among split 1 & split 3 are small and sacrifice it.
        // 
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;        Most RTIDs of the Cbo are in xRTID space"));
        QPI_ASSERT ((Split2Size == MAX_RTID_PER_POOL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
        if (Split1Size >= Split3Size) {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;        Cbo loses %u RTIDs from the Third pool", Split3Size));
          QPI_ASSERT ((((MAX_RTID_SYSTEM / 2) - CboRtidBase) == Split1Size), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
          RtidBase[CboIndex] = CboRtidBase;
          RtidSize[CboIndex] = Split1Size;
          CboRtidBase = ReallocBase; // Shift the base to right below the Remote RTID space; sacrificed RTIDs go to the end of the xRTID space.
          RtidBase[CboIndex + 1] = CboRtidBase;
          RtidSize[CboIndex + 1] = Split2Size;
          CboRtidBase = CboRtidBase + Split2Size;
        } else {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;        Cbo loses %u RTIDs from the First pool", Split1Size));
          CboRtidBase = ReallocBase; // Shift the base to right below the Remote RTID space; sacrificed RTIDs go to the end of the nonxRTID space.
          RtidBase[CboIndex] = CboRtidBase;
          RtidSize[CboIndex] = Split2Size + Split3Size; // Splitting RTIDs that doesn't cross xRTID boundary will be handled later
          CboRtidBase = CboRtidBase + Split2Size + Split3Size;
        }
      }
    } else {
      // 
      // Some extra RTIDs are available. Use that to minimize the number of RTIDs sacrificed.
      // 
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;        Some Extra RTIDs are available to reduce the Cbo RTID loss."));
      if (Split1Size == MAX_RTID_PER_POOL) {
        // 
        // This case implies most of the RTIDs are in non-xRTID space. Check which one among split 2 & split 3 are small and sacrifice it.
        // Since the extra RTIDs are in xRTID pool, only split 3 can be combined with extra RTIDs.
        // 
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;        Most RTIDs of the Cbo are in non-xRTID space"));
        if (Split2Size >= (Split3Size + *ExtraRtids)) {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;        Extra RTIDs are not big enough to avoid any loss. Cbo loses %u RTIDs from the Third pool", Split3Size));
          QPI_ASSERT ((((MAX_RTID_SYSTEM / 2) - CboRtidBase) == (Split1Size + Split2Size)), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
          RtidBase[CboIndex] = CboRtidBase;
          RtidSize[CboIndex] = Split1Size + Split2Size; // Splitting RTIDs that doesn't cross xRTID boundary will be handled later
          CboRtidBase = ReallocBase; // Shift the base to right below the Remote RTID space; sacrificed RTIDs go to the end of the xRTID space.
        } else {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;        Extra RTIDs reduces some loss. Third pool gains %u RTIDs and Second pool is lost", *ExtraRtids));
          RtidBase[CboIndex] = CboRtidBase;
          RtidSize[CboIndex] = Split1Size;
          CboRtidBase = ReallocBase; // Shift the base to right below the Remote RTID space; sacrificed RTIDs go to the end of the NonxRTID space.
          RtidBase[CboIndex + 1] = CboRtidBase;
          RtidSize[CboIndex + 1] = Split3Size + *ExtraRtids;
          CboRtidBase = CboRtidBase + RtidSize[CboIndex + 1];
          *ExtraRtids = 0;
        }
      } else {
        // 
        // This case implies most of the RTIDs are in xRTID space. Check which one among split 1 & split 3 are small and sacrifice it.
        // 
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;        Most RTIDs of the Cbo are in xRTID space"));
        QPI_ASSERT ((Split2Size == MAX_RTID_PER_POOL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
        if (Split1Size >= (Split3Size + *ExtraRtids)) {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;        Extra RTIDs are not big enough to avoid any loss. Cbo loses %u RTIDs from the Third pool", Split3Size));
          QPI_ASSERT ((((MAX_RTID_SYSTEM / 2) - CboRtidBase) == Split1Size), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
          RtidBase[CboIndex] = CboRtidBase;
          RtidSize[CboIndex] = Split1Size;
          CboRtidBase = ReallocBase; // Shift the base to right below the Remote RTID space; sacrificed RTIDs go to the end of the xRTID space.
          RtidBase[CboIndex + 1] = CboRtidBase;
          RtidSize[CboIndex + 1] = Split2Size;
          CboRtidBase = CboRtidBase + Split2Size;
        } else {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;        Extra RTIDs reduces the loss. Third pool gains %u RTIDs and First pool is lost", *ExtraRtids));
          CboRtidBase = ReallocBase; // Shift the base to right below the Remote RTID space; sacrificed RTIDs go to the end of the nonxRTID space.
          RtidBase[CboIndex] = CboRtidBase;
          RtidSize[CboIndex] = Split2Size + Split3Size + *ExtraRtids; // Splitting RTIDs that doesn't cross xRTID boundary will be handled later
          CboRtidBase = CboRtidBase + RtidSize[CboIndex];
          *ExtraRtids = 0;
        }
      }
    }
  }

  return CboRtidBase;
}

VOID
CheckAndSplitCboRtids (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8                      *RtidBase,
  UINT8                      *RtidSize
  )
/*++

Routine Description:

  Check if a given Cbo has >8 RTIDs in any of its pool. If it does, split the RTIDs and assign into 2 pools.

Arguments:

  host  - Pointer to the system host (root) structure
  QpiInternalGlobal - QPIRC Internal Global data
  RtidBase - RTID Base of the local & remote socket to be split into 2 pools
  RtidSizee - RTID Sizee of the local & remote socket to be split into 2 pools

Returns:

 VOID  
 
--*/
{
  UINT8   Idx1, Idx2, Idx3;
  UINT8   TempSize;
  UINT8   (*TempRtidBase)[MAX_CBO * 2 + 4];
  UINT8   (*TempRtidSize)[MAX_CBO * 2 + 4];

  // 
  // If Cbo has >8 RTIDs, split it into 2 and assign the RTIDs to the second pool. After the splitting, if a Cbo still has >8 RTIDs in a pool
  // clip it to 8.
  // 

  Idx2 = QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc] * 2 + 3;
  MemCheckBoundary(host, Idx2, (MAX_CBO * 2 + 4)); //Klocwork 1861 1862 1863: Array '&TempRtidSize[Idx3]' of size 52 may use index value(s) 53..253. Also there are 6 similar errors on line(s) 1693, 1694, 1695, 1698, 1699, 1700
  TempRtidBase = (UINT8 (*)[MAX_CBO * 2 + 4]) RtidBase;
  TempRtidSize = (UINT8 (*)[MAX_CBO * 2 + 4]) RtidSize;

  for (Idx3 = 0; Idx3 < 2; ++Idx3) { // Local & Remote pools
    for (Idx1 = 3; Idx1 < Idx2; Idx1 += 2) { // Start with Cbo 0 Rtid pool
      //
      // Some Cbos might have used the second pool already due to its RTID straddling into xRTID space. For such cases,
      // we just have to clip the RTIDs per pool to 8; no splitting is needed.
      //
      if ((TempRtidSize[Idx3][Idx1] > 8) && (TempRtidSize[Idx3][Idx1 + 1] == 0)) {
        TempSize = TempRtidSize[Idx3][Idx1];
        TempRtidSize[Idx3][Idx1] = (TempSize / 2) + (TempSize % 2);
        TempRtidBase[Idx3][Idx1 + 1] = TempRtidBase[Idx3][Idx1] + TempRtidSize[Idx3][Idx1];
        TempRtidSize[Idx3][Idx1 + 1] = TempSize / 2;
      }
      if (TempRtidSize[Idx3][Idx1] > 8) { // First Pool
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; Clipping Cbo%02u First pool RTID from %u to 8", (Idx1 - 3)/2, TempRtidSize[Idx3][Idx1]));
        TempRtidSize[Idx3][Idx1] = 8;
      }
      if (TempRtidSize[Idx3][Idx1 + 1] > 8) { // Second Pool
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; Clipping Cbo%02u Second pool RTID from %u to 8", (Idx1 - 3)/2, TempRtidSize[Idx3][Idx1 + 1]));
        TempRtidSize[Idx3][Idx1 + 1] = 8;
      }
    }
  }

  return;
}


QPI_STATUS
ProgramRtids (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Programs allocated RTIDs

Arguments:

  host  - Pointer to the system host (root) structure
  SocketData  - socket specific data structure
  QpiInternalGlobal - QPIRC Internal Global data

Returns:

  0 - Successful
  1 - Failures
  
--*/
{
  UINT8                                     Soc, Ctr;
  UINT32                                    Data32; 
  R3QCTRL_R3QPI_CFG_HSX_STRUCT              R3QCtrlHsx;
  R3QCTRL_R3QPI_CFG_BDX_STRUCT              R3QCtrlBdx;

  QpiCheckPoint(0xFF, 0xFF, 0xFF, STS_PROTOCOL_LAYER_SETTING, MINOR_STS_PROGRAM_RTID, host);

  //
  // Program the RTID base/size and other related registers in the Cbo. Note that in HSX the ROT base is set by
  // h/w to 0 for non-XRTID and 64 for xRTID. So there is no ROT table to be programmed. 
  //
  if (QpiInternalGlobal->BtModeEn == FALSE) {
    if (host->var.qpi.SysConfig == SYS_CONFIG_1S) {
      ProgramRtidSHT1SHsx (host, QpiInternalGlobal);
    } else {
      ProgramRtidSHT2SHsx (host, QpiInternalGlobal); // Handles Isoc and NonIsoc cases
    }
  } else if (host->var.qpi.OutClusterOnDieEn == TRUE) {
    if (QpiInternalGlobal->Sys4SClusterOnDieEn == FALSE) {
      ProgramRtidCod (host, QpiInternalGlobal); // Handles BT3 mode case
    } else {
      if (host->var.common.cpuType == CPU_BDX) {
        ProgramRtid4SCODBdx (host, QpiInternalGlobal);
      }
#ifdef QPI_4S_COD_FEATURE_PROTOTYPE
      else { // when cpuType is CPU_HSX
        ProgramRtid4SCODHsx( host, QpiInternalGlobal);
      }
#endif
    }
  } else {
    ProgramRtidCommon (host, QpiInternalGlobal); // Handles at BT mode cases except BT3
  }

  // Program the ExtRTID Mode enable bit
  if (QpiInternalGlobal->ExtRtidMode == TRUE) {
    for (Soc = 0; Soc < MAX_CPU_SOCKETS; ++Soc) {
      if ((QpiInternalGlobal->CpuList & (1 << Soc))) {
        // R2PCIe ExtRTID mode enable
        Data32 = QpiReadPciCfg (host, Soc, 0, R2PGNCTRL_R2PCIE_CFG_REG);
        Data32 |= 0x1;
        QpiWritePciCfg (host, Soc, 0, R2PGNCTRL_R2PCIE_CFG_REG, Data32);

        // R3QPI ExtRTID mode enable
        for (Ctr = 0; Ctr < QpiInternalGlobal->TotR3Qpi[Soc]; ++Ctr) {
          Data32 = QpiReadPciCfg (host, Soc, Ctr, R3QCTRL_R3QPI_CFG_REG);
          if (host->var.common.cpuType == CPU_HSX) {
            R3QCtrlHsx.Data = Data32;
            R3QCtrlHsx.Bits.extendedrtidmode = 0x1;
            Data32 = R3QCtrlHsx.Data;
          } else {
            R3QCtrlBdx.Data = Data32;
            R3QCtrlBdx.Bits.extendedrtidmode = 0x1;
            Data32 = R3QCtrlBdx.Data;
          }
          QpiWritePciCfg (host, Soc, Ctr, R3QCTRL_R3QPI_CFG_REG, Data32);
        }
      }
    }
  }


  //
  // RTID Base/Size are programmed into shadow registers that are Sticky. H/W will sample those registers
  // only during WarmReset). So we need to request Warm reset.
  //
  host->var.common.resetRequired |= POST_RESET_WARM;

  return QPI_SUCCESS;
}

VOID
ProgramRtidSHT1SHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Programs allocated RTIDs in 1S config

Arguments:

  host  - Pointer to the system host (root) structure
  QpiInternalGlobal - QPIRC Internal Global data

Returns:

  VOID
  
--*/
{
  BOOLEAN                                   ExtRtidMode = FALSE;
  UINT8                                     IsocBase;
  UINT8                                     Cbo;
  UINT8                                     Pool, Ha;
  UINT8                                     Idx;
  UINT8                                     Base;
  UINT8                                     Size;
  UINT32                                    Data32;
  UINT32                                    PoolBaseReg[SI_MAX_HA * 2] = {
        RTID_CONFIG_POOL01_BASE_SHADOW_CBO_CFG_REG,
        RTID_CONFIG_POOL45_BASE_SHADOW_CBO_CFG_REG,
        RTID_CONFIG_POOL89_BASE_SHADOW_CBO_CFG_REG,
        RTID_CONFIG_POOLCD_BASE_SHADOW_CBO_CFG_REG
  };

  UINT32                                    PoolSizeReg[SI_MAX_HA * 2] = {
        RTID_CONFIG_POOL01_SIZE_CBO_CFG_REG,
        RTID_CONFIG_POOL45_SIZE_CBO_CFG_REG,
        RTID_CONFIG_POOL89_SIZE_CBO_CFG_REG,
        RTID_CONFIG_POOLCD_SIZE_CBO_CFG_REG
 };
  RTID_POOL_CONFIG_SHADOW_CBO_CFG_HSX_STRUCT PoolCfg;

  // Index 2 is for ISOC RTID; initialized to 0 during RTID allocation if ISOC is not enabled
  IsocBase = QpiInternalGlobal->RtidBase[0][2];
  

  //
  // In 1S, the CPU being populated is always CPU0. So program pool 0, 8 for HA0 and 4, 12 for HA1 (for 2 pool mode operation).
  //
  for (Cbo = 0; Cbo < QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc]; ++Cbo) {
    for (Pool = 0; Pool < 2; ++Pool) { // Two pool mode
      Idx = Cbo * 2 + Pool + 3; // Cbo RTIDs start from index 3
      Base = QpiInternalGlobal->RtidBase[0][Idx]; // Local Pool Base
      Size = QpiInternalGlobal->RtidSize[0][Idx]; // Local Pool Size


      // 
      // Program Base
      // 
      for (Ha = 0; Ha < QpiInternalGlobal->TotHa[QpiInternalGlobal->SbspSoc]; ++Ha) {
        Idx = Pool * 2 + Ha;
        if (Idx < SI_MAX_HA * 2) {
          Data32 = QpiReadPciCfg (host, QpiInternalGlobal->SbspSoc, Cbo, PoolBaseReg[Idx]);
          Data32 &= 0xFDC0FDC0; // Clear Pool Base & ExtRTID fileds
          if (Base < MAX_RTID_PER_SOC) {
            Data32 |= Base;
          } else {
            ExtRtidMode = TRUE;
            Data32 |= ((1 << 9) | (Base - MAX_RTID_PER_SOC)); // Set ExtendedMode bit
          }
          QpiWritePciCfg (host, QpiInternalGlobal->SbspSoc, Cbo, PoolBaseReg[Idx], Data32);

          // Program Size
          Data32 = QpiReadPciCfg (host, QpiInternalGlobal->SbspSoc, Cbo, PoolSizeReg[Idx]);
          Data32 = (Data32 & 0xFC3FFC3F) | (Size << 6); // Clear Pool size field
          QpiWritePciCfg (host, QpiInternalGlobal->SbspSoc, Cbo, PoolSizeReg[Idx], Data32);
        } else {
          QPI_ASSERT (FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
        }
      }
    }

    //
    // Program Pool Config Register
    //
    PoolCfg.Data = QpiReadPciCfg (host, QpiInternalGlobal->SbspSoc, Cbo, RTID_POOL_CONFIG_SHADOW_CBO_CFG_REG);
    PoolCfg.Bits.rtidpoolsel = DOUBLE_POOLIVT_MODE;
    PoolCfg.Bits.isocha0rtidbase = IsocBase;
    PoolCfg.Bits.isocha1rtidbase = IsocBase;
    PoolCfg.Bits.extendedrtiden = TRUE;
    PoolCfg.Bits.isocrtidpoolsel = 0x1;
    QpiWritePciCfg (host, QpiInternalGlobal->SbspSoc, Cbo, RTID_POOL_CONFIG_SHADOW_CBO_CFG_REG, PoolCfg.Data);
  }

  // Update the scratch register in SBSP with ExtRTID mode info
  Data32  = QpiReadPciCfg(host, 0, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG);
  Data32  = (Data32 & 0xFFFFFF7F) | (ExtRtidMode << 7);
  QpiWritePciCfg(host, 0, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG, Data32);
  QpiInternalGlobal->ExtRtidMode = ExtRtidMode;

  return;
}


VOID
ProgramRtidSHT2SHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Programs allocated RTIDs in 2S config

Arguments:

  host  - Pointer to the system host (root) structure
  QpiInternalGlobal - QPIRC Internal Global data

Returns:

  VOID
  
--*/
{
  BOOLEAN                                   ExtRtidMode = FALSE;
  UINT8                                     IsocBase;
  UINT8                                     Soc;
  UINT8                                     RemoteSoc;
  UINT8                                     Cbo;
  UINT8                                     Pool;
  UINT8                                     Idx, Ha;
  UINT8                                     Base[2];
  UINT8                                     Size[2];
  UINT32                                    Data32;
  UINT32                                    PoolBaseReg[SI_MAX_HA * 2] = {
        RTID_CONFIG_POOL01_BASE_SHADOW_CBO_CFG_REG,
        RTID_CONFIG_POOL45_BASE_SHADOW_CBO_CFG_REG,
        RTID_CONFIG_POOL89_BASE_SHADOW_CBO_CFG_REG,
        RTID_CONFIG_POOLCD_BASE_SHADOW_CBO_CFG_REG
  };

  UINT32                                    PoolSizeReg[SI_MAX_HA * 2] = {
        RTID_CONFIG_POOL01_SIZE_CBO_CFG_REG,
        RTID_CONFIG_POOL45_SIZE_CBO_CFG_REG,
        RTID_CONFIG_POOL89_SIZE_CBO_CFG_REG,
        RTID_CONFIG_POOLCD_SIZE_CBO_CFG_REG
 };
  RTID_POOL_CONFIG_SHADOW_CBO_CFG_HSX_STRUCT PoolCfg;
  UINT8                                     sbspSktId;
  UINT8                                     (*TempRtidBase)[MAX_CBO * 2 + 4];
  UINT8                                     (*TempRtidSize)[MAX_CBO * 2 + 4];

  sbspSktId = QpiInternalGlobal->SbspSoc;

  IsocBase = QpiInternalGlobal->RtidBaseIsocS0[0][2]; // Index 2 is for ISOC RTID; initialized to 0 during RTID allocation if ISOC is not enabled
  
  //
  // In 2S, the CPU populated is always CPU0 & CPU1. Program pool 0, 8 for S0 HA0; 4, 12 for S0 HA1;
  // 1, 9 for S1 HA0 and 5, 13 for S1 HA1 (for 2 pool mode operation).
  //
  for (Soc = 0; Soc < QpiInternalGlobal->TotCpu; ++Soc) {

    (Soc == 0) ? (RemoteSoc = 1) : (RemoteSoc = 0);

    if (host->var.qpi.OutIsocEn == TRUE) {
      if (Soc == 0) {
        TempRtidBase = QpiInternalGlobal->RtidBaseIsocS0;
        TempRtidSize = QpiInternalGlobal->RtidSizeIsocS0;
      } else {
        TempRtidBase = QpiInternalGlobal->RtidBaseIsocS1;
        TempRtidSize = QpiInternalGlobal->RtidSizeIsocS1;
      }
    } else {
      TempRtidBase = QpiInternalGlobal->RtidBase;
      TempRtidSize = QpiInternalGlobal->RtidSize;
    }

    for (Cbo = 0; Cbo < QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc]; ++Cbo) {
      for (Pool = 0; Pool < 2; ++Pool) { // Two pool mode
        Idx = Cbo * 2 + Pool + 3; // Cbo RTIDs start from index 3
        Base[Soc] = TempRtidBase[0][Idx]; // Local Base
        Size[Soc] = TempRtidSize[0][Idx]; // Local Size
        Base[RemoteSoc] = TempRtidBase[1][Idx]; // Remote Base
        Size[RemoteSoc] = TempRtidSize[1][Idx]; // Remote Size

        //
        // Program Base & Size
        //
        for (Ha = 0; Ha < QpiInternalGlobal->TotHa[QpiInternalGlobal->SbspSoc]; ++Ha) {
          Idx = Pool * 2 + Ha;
          if (Idx < SI_MAX_HA * 2) {
            Data32 = QpiReadPciCfg (host, Soc, Cbo, PoolBaseReg[Idx]);
            Data32 &= 0xFDC0FDC0; // Clear Pool Base & ExtRTID fileds
            if (Base[0] < MAX_RTID_PER_SOC) {
              Data32 |= Base[0];
            } else {
              ExtRtidMode = TRUE;
              Data32 |= ((1 << 9) | (Base[0] - MAX_RTID_PER_SOC)); // Set ExtendedMode bit
            }
            if (Base[1] < MAX_RTID_PER_SOC) {
              Data32 |= ((Base[1] << 16));
            } else {
              ExtRtidMode = TRUE;
              Data32 |= ((1 << 25) | ((Base[1] - MAX_RTID_PER_SOC) << 16)); // Set ExtendedMode bit
            }
            QpiWritePciCfg (host, Soc, Cbo, PoolBaseReg[Idx], Data32);

            Data32 = QpiReadPciCfg (host, Soc, Cbo, PoolSizeReg[Idx]);
            Data32 = (Data32 & 0xFC3FFC3F) | (Size[1] << 22) | (Size[0] << 6); // Clear Pool size field
            QpiWritePciCfg (host, Soc, Cbo, PoolSizeReg[Idx], Data32);
          } else {
            QPI_ASSERT (FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
          }
        }
      }

      //
      // Program Pool Config Register
      //
      PoolCfg.Data = QpiReadPciCfg (host, Soc, Cbo, RTID_POOL_CONFIG_SHADOW_CBO_CFG_REG);
      PoolCfg.Bits.rtidpoolsel    = DOUBLE_POOLIVT_MODE;
      PoolCfg.Bits.isocha0rtidbase = IsocBase;
      PoolCfg.Bits.isocha1rtidbase = IsocBase;
      PoolCfg.Bits.extendedrtiden = TRUE;
      if (host->var.qpi.OutIsocEn == TRUE) {
        if (host->var.qpi.SysConfig == SYS_CONFIG_2SEN) {
          PoolCfg.Bits.isocrtidpoolsel = 0x2;
        } else {
          PoolCfg.Bits.isocrtidpoolsel = 0x1;
        }
      }
      QpiWritePciCfg (host, Soc, Cbo, RTID_POOL_CONFIG_SHADOW_CBO_CFG_REG, PoolCfg.Data);
    }
  }


  // Update the scratch register in SBSP with ExtRTID mode info
  Data32 = QpiReadPciCfg(host, 0, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG);
  Data32  = (Data32 & 0xFFFFFF7F) | (ExtRtidMode << 7);
  QpiWritePciCfg(host, 0, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG, Data32);
  QpiInternalGlobal->ExtRtidMode = ExtRtidMode;

  return;
}


VOID
ProgramRtidCommon (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Programs allocated RTIDs

Arguments:

  host  - Pointer to the system host (root) structure
  QpiInternalGlobal  - QPIRC global variables

Returns:

  VOID
  
--*/
{
  BOOLEAN      LocalPool;
  UINT8        Soc, Cbo, Pool;
  UINT8        Idx1, Idx2, Idx3=0, RegIdx;
  UINT8        Base[2][MAX_POOL_SETS] = {{0, 0, 0}};
  UINT8        Size[2][MAX_POOL_SETS] = {{0, 0, 0}};
  UINT8        TempBase[2] = {0}, TempSize[2] = {0}, SecondPool[2] = {0};
  UINT8        LclHaPoolNum[SI_MAX_HA][MAX_POOL_SETS] = {{0, 0, 0}};
  UINT8        NumPoolSets, NumPools;
  UINT32       Data32;
  UINT32       PoolBaseReg[MAX_CBO_POOLS / 2] = {
                      RTID_CONFIG_POOL01_BASE_SHADOW_CBO_CFG_REG, RTID_CONFIG_POOL23_BASE_SHADOW_CBO_CFG_REG,
                      RTID_CONFIG_POOL45_BASE_SHADOW_CBO_CFG_REG, RTID_CONFIG_POOL67_BASE_SHADOW_CBO_CFG_REG,
                      RTID_CONFIG_POOL89_BASE_SHADOW_CBO_CFG_REG, RTID_CONFIG_POOLAB_BASE_SHADOW_CBO_CFG_REG,
                      RTID_CONFIG_POOLCD_BASE_SHADOW_CBO_CFG_REG, RTID_CONFIG_POOLEF_BASE_SHADOW_CBO_CFG_REG,
                      RTID_CONFIG_POOLGH_BASE_SHADOW_CBO_CFG_REG, 
                      RTID_CONFIG_POOLGH_BASE_SHADOW_CBO_CFG_REG    // Base for pools I,J are placed inside GH_BASE_SHADOW
                  };
  UINT32        PoolSizeReg[MAX_CBO_POOLS / 2] = {
                      RTID_CONFIG_POOL01_SIZE_CBO_CFG_REG, RTID_CONFIG_POOL23_SIZE_CBO_CFG_REG,
                      RTID_CONFIG_POOL45_SIZE_CBO_CFG_REG, RTID_CONFIG_POOL67_SIZE_CBO_CFG_REG,
                      RTID_CONFIG_POOL89_SIZE_CBO_CFG_REG, RTID_CONFIG_POOLAB_SIZE_CBO_CFG_REG,
                      RTID_CONFIG_POOLCD_SIZE_CBO_CFG_REG, RTID_CONFIG_POOLEF_SIZE_CBO_CFG_REG,
                      RTID_CONFIG_POOLGH_SIZE_CBO_CFG_REG, 
                      RTID_CONFIG_POOLEF_BASE_SHADOW_CBO_CFG_REG    // Size for pools I,J are placed inside EF_BASE_SHADOW
                  };

  // RTID_POOL_CONFIG_SHADOW_CBO_CFG_BDX_STRUCT is a super-set of the _HSX_STRUCT. Using this struct for
  // HSX and BDX accesses, though bit 26 in this struct is a BDX- only bit
  RTID_POOL_CONFIG_SHADOW_CBO_CFG_BDX_STRUCT     PoolCfg;
  
  // Set the NumPoolSets and NumPools based on cpuType
  if (host->var.common.cpuType == CPU_BDX) {
    NumPoolSets = MAX_POOL_SETS_BDX;
    NumPools    = MAX_POOLS_BDX;
  } else { // CPU_HSX
    NumPoolSets = MAX_POOL_SETS_HSX;
    NumPools    = MAX_POOLS_HSX;
  }


  // All the double pool modes use ExtRTID mode
  if (QpiInternalGlobal->CboPoolMode != SINGLE_POOL01_MODE) {
    QpiInternalGlobal->ExtRtidMode = TRUE;
  }

  for (Soc = 0; Soc < MAX_CPU_SOCKETS; ++Soc) {
    if ((host->var.qpi.RasInProgress == FALSE && (QpiInternalGlobal->CpuList & (1 << Soc))) ||
        (host->var.qpi.RasInProgress == TRUE && host->var.qpi.RasSocId == Soc)) {
      //
      // Identify which two of the 18 pools will be used by Cbos of this socket for its local HA communication.
      // In all the Cbo pool modes (relevant to EX Platform that is), first pool is selected based on NID[3:0]
      // of the local HA NID. Note that even though DOUBLE_POOLIVT_MODE uses only NID[2:0], since
      // NID[3] is always for this config, we can use NID[3:0] to select the first pool. The second pool
      // selection varies as described below.
      //
      MemSetLocal((UINT8*)LclHaPoolNum, 0xFF, sizeof(LclHaPoolNum));

      LclHaPoolNum[0][0] = (UINT8)SOCID_TO_NID (Soc, 0); // HA0 NID
      LclHaPoolNum[1][0] = (UINT8)SOCID_TO_NID (Soc, 1); // HA1 NID
      if (QpiInternalGlobal->CboPoolMode == SINGLE_POOL01_MODE) {
        // Second poolset xRTID=1 which is poolset 1 and the special 
        // local second xRTID=0 poolset which is poolset 2 must be
        // initialized to 0xFF by now.
      } else if (QpiInternalGlobal->CboPoolMode == DOUBLE_POOLIVT_MODE) {
        // Second pool is selected by adding 8 to the first pool number.
        LclHaPoolNum[0][1] = LclHaPoolNum[0][0] + 8;
        LclHaPoolNum[1][1] = LclHaPoolNum[1][0] + 8;
      } else { // DOUBLE_POOL8S_MODE
        // Second pool number is 16 for HA0 and 17 for HA1.
        LclHaPoolNum[0][1] = 16;
        LclHaPoolNum[1][1] = 17;

        if (host->var.common.cpuType == CPU_BDX) {
          LclHaPoolNum[0][2] = 18;
          LclHaPoolNum[1][2] = 19;
        } // cpuType check for BDX
      }

      //
      // Program the Pool base & size registers for each Cbo
      //
      for (Cbo = 0; Cbo < QpiInternalGlobal->TotCbo[Soc]; ++Cbo) {

        //
        // Identify the RTID base & size allocated for this Cbo for Local First Pool, Local Second Pool, 
        // Remote First Pool, Remote Second Pool, 
        //
        Idx1 = Cbo * 2 + 3; // Cbo RTIDs start from index 3
        for (Idx2 = 0; Idx2 < 2; ++Idx2) { // Local & Remote HA pools
          for (Idx3 = 0; Idx3 < NumPoolSets; ++Idx3) { // First & Second Pools
            if ((host->var.common.cpuType == CPU_BDX) && (Idx3 == (MAX_POOL_SETS_BDX - 1))) { // Idx3 can reach MAX_POOLSETS_BDX only for BDX- flow
              Base[Idx2][Idx3] = QpiInternalGlobal->RtidBase[Idx3][Idx1 + Idx2];
              Size[Idx2][Idx3] = QpiInternalGlobal->RtidSize[Idx3][Idx1 + Idx2];
            } else {
              Base[Idx2][Idx3] = QpiInternalGlobal->RtidBase[Idx2][Idx1 + Idx3];
              Size[Idx2][Idx3] = QpiInternalGlobal->RtidSize[Idx2][Idx1 + Idx3];
            }
          } // for loop - Idx3
        } // for loop - Idx2


        //
        // Program the pool base & size values for all the pools
        //
        for (Pool = 0; Pool < NumPools; ++Pool) {
          //
          // Identify the pool base and size values used by this pool. Also identify if this pool
          // is used as First pool or Second pool.
          //
          SecondPool[Pool % 2] = 0;
          if ((QpiInternalGlobal->CboPoolMode != DOUBLE_POOL8S_MODE) && (Pool >= 16 && Pool <=  19)) {
            //
            // In this pool mode, pool 16 - 19 are not used
            //
            TempBase[Pool % 2] = 0;
            TempSize[Pool % 2] = 0;
          } else {
            //
            // Check if this pool is used for local HA communication. Also indentify
            // if this pool is First pool or Second Pool.
            //
            LocalPool = FALSE;
            for (Idx2 = 0; Idx2 < 2; ++Idx2) { // HA 0 & 1
              for (Idx3 = 0; Idx3 < NumPoolSets; ++Idx3) { // First & Second Pools
                if (LclHaPoolNum[Idx2][Idx3] == Pool) {
                  LocalPool = TRUE;
                  break;
                } 
              } 
              if (LocalPool == TRUE) {
                break;
              }
            } 

            // When the search for pool in LclHaPoolNum breaks or completes, 
            // [1] When LocalPool = TRUE, Idx2 contains the HA0/1 which match and Idx3 contains the pool set number of match
            // [2] When LocalPool = FALSE, it denotes a remote HA and thus Idx2 and Idx3 are invalid
            if (LocalPool == TRUE) {
              TempBase[Pool % 2] = Base[0][Idx3]; // Local pool base (Idx3 = 1st vs 2nd pool)
              TempSize[Pool % 2] = Size[0][Idx3]; // Local pool size (Idx3 = 1st vs 2nd pool)
              if (Idx3 == 1) { // Is it Second pool? Second pool corresponds to xRTID = 1
                SecondPool[Pool % 2] = 1;
              } 
            } else {
              //
              // This pool must be used for Remote HA. Indentify if this pool is First pool or Second Pool.
              //
              if (QpiInternalGlobal->CboPoolMode == SINGLE_POOL01_MODE) {
                Idx3 = 0; // This mode uses only First pool
              } else if (QpiInternalGlobal->CboPoolMode == DOUBLE_POOLIVT_MODE) {
                Idx3 = (Pool < 8)  ? 0 : 1; // 0-7 are used as First Pool and 8-15 are used as Second pool
              } else {
                Idx3 = (Pool < 16) ? 0 : (Pool <= 17) ? 1 : 2; // 0-15 are used as First Pool and 16-17 are used as Second pool
              }
              TempBase[Pool % 2] = Base[1][Idx3]; // Remote pool base
              TempSize[Pool % 2] = Size[1][Idx3]; // Remote pool size
              if (Idx3 == 1) { // Is it Second pool?
                SecondPool[Pool % 2] = 1;
              }
            }//Remote pool
          }//Not DOUBLE_POOL8S_MODE Pools 16 & 17 


          //
          // Odd & Even pools share the same register. So program the pools by combining them to reduce number of
          // register reads/writes.
          //
          if (Pool % 2) {
            //
            // Program RTID Base & ExtendedMode settings
            //
            RegIdx = Pool / 2;
            Data32 = QpiReadPciCfg (host, Soc, Cbo, PoolBaseReg[RegIdx]);
            if (Pool < 18) {
              if (host->var.common.cpuType == CPU_BDX) {
                // We can have the sizes in GH registers as zero. In such a case, we should override Ext RTID bit as 0 as the size is 0
                if (TempSize[0] == 0) {
                  SecondPool[0] = 0;
                }

                if (TempSize[1] == 0) {
                  SecondPool[1] = 0;
                }
              } // cpuType check

              Data32 = (Data32 & 0xFDC0FDC0) | (SecondPool[1] << 25) | (TempBase[1] << 16) | (SecondPool[0] << 9) | TempBase[0];
            } else {
              // Set the RTID bases for Pool I and J(BDX only). Base for pools I,J are placed inside GH_BASE_SHADOW
              Data32 = (Data32 & 0x03FF03FF) | (TempBase[1] << 26) | (TempBase[0] << 10);
            }
            QpiWritePciCfg (host, Soc, Cbo, PoolBaseReg[RegIdx], Data32);

            //
            // Program Size
            //
            Data32 = QpiReadPciCfg (host, Soc, Cbo, PoolSizeReg[RegIdx]);
            if (Pool < 18) {
              Data32 = (Data32 & 0xFC3FFC3F) | (TempSize[1] << 22) | (TempSize[0] << 6);
            } else {
              // Set the RTID sizes for Pool I and J(BDX only). Size for pools I,J are placed inside EF_BASE_SHADOW
              Data32 = (Data32 & 0X0FFF87FF) | (TempSize[1] << 28) | (TempSize[0] << 11);
            }
            QpiWritePciCfg (host, Soc, Cbo, PoolSizeReg[RegIdx], Data32);            
          }
        } // for loop - pool

        //
        // Program Pool Config Register
        //

        PoolCfg.Data = QpiReadPciCfg (host, Soc, Cbo, RTID_POOL_CONFIG_SHADOW_CBO_CFG_REG);
        PoolCfg.Bits.rtidpoolsel    = QpiInternalGlobal->CboPoolMode;                   // update rtidpoolsel - Bits[2:0], RWS, default =3'b00 
        PoolCfg.Bits.extendedrtiden = QpiInternalGlobal->ExtRtidMode;                   // update extendedrtiden - Bits[24:24], RWS, default =1'b0 

        if ((host->var.common.cpuType == CPU_BDX) && (QpiInternalGlobal->CboPoolMode == DOUBLE_POOL8S_MODE)) {
            PoolCfg.Bits.enbdxbt2rtidpool = 1;
        } // cpuType check for BDX

        QpiWritePciCfg (host, Soc, Cbo, RTID_POOL_CONFIG_SHADOW_CBO_CFG_REG, PoolCfg.Data);
      } // for loop - cbo
    }
  } // for loop - socket

  // Update the scratch register in SBSP with ExtRTID mode info
  Data32  = QpiReadPciCfg (host, QpiInternalGlobal->SbspSoc, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG);
  Data32  = (Data32 & 0xFFFFFF7F) | (QpiInternalGlobal->ExtRtidMode <<7);
  QpiWritePciCfg (host, QpiInternalGlobal->SbspSoc, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG, Data32);

  return;
}

VOID
ProgramRtidCod (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Programs allocated RTIDs

Arguments:

  host  - Pointer to the system host (root) structure
  QpiInternalGlobal  - QPIRC global variables

Returns:

  VOID
  
--*/
{
  BOOLEAN      LocalPool;
  UINT8        Soc, Cbo, Pool, Cluster;
  UINT8        Idx1, Idx2, Idx3=0, RegIdx;
  UINT8        Base[2][MAX_POOL_SETS];
  UINT8        Size[2][MAX_POOL_SETS];
  UINT8        TempBase[2], TempSize[2], SecondPool[2];
  UINT8        LclHaPoolNum[MAX_POOL_SETS];
  UINT32       Data32;
  UINT8        NumPoolSets, NumPools;
  UINT32       PoolBaseReg[MAX_CBO_POOLS / 2] = {
                      RTID_CONFIG_POOL01_BASE_SHADOW_CBO_CFG_REG, RTID_CONFIG_POOL23_BASE_SHADOW_CBO_CFG_REG,
                      RTID_CONFIG_POOL45_BASE_SHADOW_CBO_CFG_REG, RTID_CONFIG_POOL67_BASE_SHADOW_CBO_CFG_REG,
                      RTID_CONFIG_POOL89_BASE_SHADOW_CBO_CFG_REG, RTID_CONFIG_POOLAB_BASE_SHADOW_CBO_CFG_REG,
                      RTID_CONFIG_POOLCD_BASE_SHADOW_CBO_CFG_REG, RTID_CONFIG_POOLEF_BASE_SHADOW_CBO_CFG_REG,
                      RTID_CONFIG_POOLGH_BASE_SHADOW_CBO_CFG_REG 
                  };
  UINT32        PoolSizeReg[MAX_CBO_POOLS / 2] = {
                      RTID_CONFIG_POOL01_SIZE_CBO_CFG_REG, RTID_CONFIG_POOL23_SIZE_CBO_CFG_REG,
                      RTID_CONFIG_POOL45_SIZE_CBO_CFG_REG, RTID_CONFIG_POOL67_SIZE_CBO_CFG_REG,
                      RTID_CONFIG_POOL89_SIZE_CBO_CFG_REG, RTID_CONFIG_POOLAB_SIZE_CBO_CFG_REG,
                      RTID_CONFIG_POOLCD_SIZE_CBO_CFG_REG, RTID_CONFIG_POOLEF_SIZE_CBO_CFG_REG,
                      RTID_CONFIG_POOLGH_SIZE_CBO_CFG_REG
                  };

  RTID_POOL_CONFIG_SHADOW_CBO_CFG_HSX_STRUCT PoolCfg;

  // Both HSX and BDX uses BtMode 3 to handle 1S and 2S COD. In these cases, the IJ pools are
  // not relevant. Thus, restricting the pool programming loop to HSX-.
  NumPoolSets = MAX_POOL_SETS_HSX;
  NumPools = MAX_POOLS_HSX;

  // All the double pool modes use ExtRTID mode
  QpiInternalGlobal->ExtRtidMode = TRUE;

  for (Soc = 0; Soc < MAX_CPU_SOCKETS; ++Soc) {
    if ((host->var.qpi.RasInProgress == FALSE && (QpiInternalGlobal->CpuList & (1 << Soc))) ||
        (host->var.qpi.RasInProgress == TRUE && host->var.qpi.RasSocId == Soc)) {
      for (Cluster = 0; Cluster < 2; ++Cluster) {
        //
        // Identify which two of the 18 pools will be used by Cbos of this socket for its local HA communication.
        // In all the Cbo pool modes (relevant to EX Platform that is), first pool is selected based on NID[3:0]
        // of the local HA NID. Note that even though DOUBLE_POOLIVT_MODE uses only NID[2:0], since
        // NID[3] is always for this config, we can use NID[3:0] to select the first pool. The second pool
        // selection varies as described below.
        //
        LclHaPoolNum[0] = (UINT8)SOCID_TO_NID (Soc, Cluster); // HA NID of the current cluster
        // Second pool is selected by adding 8 to the first pool number.
        LclHaPoolNum[1] = LclHaPoolNum[0] + 8;
        
        //
        // Program the Pool base & size registers for each Cbo
        //
        for (Cbo = Cluster; Cbo < QpiInternalGlobal->TotCbo[Soc]; Cbo+=2) {
          //
          // Identify the RTID base & size allocated for this Cbo for Local First Pool, Local Second Pool, 
          // Remote First Pool, Remote Second Pool, 
          //
          Idx1 = Cbo * 2 + 3; // Cbo RTIDs start from index 3
          for (Idx2 = 0; Idx2 < 2; ++Idx2) { // Local & Remote HA pools
            for (Idx3 = 0; Idx3 < NumPoolSets; ++Idx3) { // First & Second Pools
              if (Idx2 == 1 && Idx3 == 1) {
                //
                // No second pool for remote in BTmode3.   
                //
                Base[Idx2][Idx3] = 0;
                Size[Idx2][Idx3] = 0;
              } else {
                Base[Idx2][Idx3] = QpiInternalGlobal->RtidBase[Idx2][Idx1 + Idx3];
                Size[Idx2][Idx3] = QpiInternalGlobal->RtidSize[Idx2][Idx1 + Idx3];
              }
            }//For each pool type (1st,2nd)
          }//For each access type (Local,Remote)

          //
          // Program the pool base & size values for all the pools
          //
          for (Pool = 0; Pool < NumPools; ++Pool) {
            //
            // Identify the pool base and size values used by this pool. Also identify if this pool
            // is used as First pool or Second pool.
            //
            SecondPool[Pool % 2] = 0;
            if (Pool > 15) {
              //
              // In this pool mode, pool 16 & 17 are not used
              //
              TempBase[Pool % 2] = 0;
              TempSize[Pool % 2] = 0;
            } else {
              //
              // Check if this pool is used for local HA communication. Also indentify
              // if this pool is First pool or Second Pool.
              //
              LocalPool = FALSE;
              for (Idx3 = 0; Idx3 < NumPoolSets; ++Idx3) { // First & Second Pools
                if (LclHaPoolNum[Idx3] == Pool) {
                  LocalPool = TRUE;
                  break;
                }
              }//For each Pool (1st,2nd)

              if (LocalPool == TRUE) {
                TempBase[Pool % 2] = Base[0][Idx3]; // Local pool base
                TempSize[Pool % 2] = Size[0][Idx3]; // Local pool size
                if (Idx3 == 1) { // Is it Second pool?
                  SecondPool[Pool % 2] = 1;
                } 
              } else {
                //
                // This pool must be used for Remote HA. Indentify if this pool is First pool or Second Pool.
                //
                Idx3 = (Pool < 8) ? 0 : 1; // 0-7 are used as First Pool and 8-15 are used as Second pool
                TempBase[Pool % 2] = Base[1][Idx3]; // Remote pool base
                TempSize[Pool % 2] = Size[1][Idx3]; // Remote pool size
              }//Local vs. Remote HA

            }//Not Pool 16 or 17


            //
            // Odd & Even pools share the same register. So program the pools by combining them to reduce number of
            // register reads/writes.
            //
            if (Pool % 2) {
              //
              // Program RTID Base & ExtendedMode settings
              //
              RegIdx = Pool / 2;
              Data32 = QpiReadPciCfg (host, Soc, Cbo, PoolBaseReg[RegIdx]);
              Data32 = (Data32 & 0xFDC0FDC0) | (SecondPool[1] << 25) | (TempBase[1] << 16) | (SecondPool[0] << 9) | TempBase[0];
              QpiWritePciCfg (host, Soc, Cbo, PoolBaseReg[RegIdx], Data32);

              //
              // Program Size
              //
              Data32 = QpiReadPciCfg (host, Soc, Cbo, PoolSizeReg[RegIdx]);
              Data32 = (Data32 & 0xFC3FFC3F) | (TempSize[1] << 22) | (TempSize[0] << 6);
              QpiWritePciCfg (host, Soc, Cbo, PoolSizeReg[RegIdx], Data32);
            }//If both halves of the CSR filled in (i.e. Odd # Pool) 
          }//For each Cbo Pool

          //
          // Program Pool Config Register
          //
          PoolCfg.Data = QpiReadPciCfg (host, Soc, Cbo, RTID_POOL_CONFIG_SHADOW_CBO_CFG_REG);
          PoolCfg.Bits.rtidpoolsel    = QpiInternalGlobal->CboPoolMode;                   // update rtidpoolsel - Bits[2:0], RWS, default =3'b00 
          PoolCfg.Bits.extendedrtiden = QpiInternalGlobal->ExtRtidMode;                   // update extendedrtiden - Bits[24:24], RWS, default =1'b0 
            QpiWritePciCfg (host, Soc, Cbo, RTID_POOL_CONFIG_SHADOW_CBO_CFG_REG, PoolCfg.Data);
          }//For each CBo in Cluster 
        }//For each Cluster
      }//If Socket is valid
    }//For each Socket

  // Update the scratch register in SBSP with ExtRTID mode info
  Data32  = QpiReadPciCfg (host, QpiInternalGlobal->SbspSoc, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG);
  Data32  = (Data32 & 0xFFFFFF7F) | (QpiInternalGlobal->ExtRtidMode <<7);
  QpiWritePciCfg (host, QpiInternalGlobal->SbspSoc, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG, Data32);

  return;
}


#ifdef QPI_4S_COD_FEATURE_PROTOTYPE
VOID
ProgramRtid4SCODHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Programs allocated RTIDs for 4S COD configuration in HSX

Arguments:

  host  - Pointer to the system host (root) structure
  QpiInternalGlobal  - QPIRC global variables

Returns:

  VOID
  
--*/
{
  BOOLEAN      LocalPool;
  UINT8        Soc, Cbo, Pool, Cluster;
  UINT8        Idx1, Idx2, Idx3=0, RegIdx;
  UINT8        Base[2][MAX_POOL_SETS_HSX];
  UINT8        Size[2][MAX_POOL_SETS_HSX];
  UINT8        TempBase[2] = {0}, TempSize[2] = {0}, SecondPool[2] = {0};
  UINT8        LclHaPoolNum[MAX_POOL_SETS_HSX];
  UINT32       Data32;
  UINT8        NumPoolSets, NumPools;
  UINT32       PoolBaseReg[MAX_POOLS_HSX / 2] = {
                      RTID_CONFIG_POOL01_BASE_SHADOW_CBO_CFG_REG, RTID_CONFIG_POOL23_BASE_SHADOW_CBO_CFG_REG,
                      RTID_CONFIG_POOL45_BASE_SHADOW_CBO_CFG_REG, RTID_CONFIG_POOL67_BASE_SHADOW_CBO_CFG_REG,
                      RTID_CONFIG_POOL89_BASE_SHADOW_CBO_CFG_REG, RTID_CONFIG_POOLAB_BASE_SHADOW_CBO_CFG_REG,
                      RTID_CONFIG_POOLCD_BASE_SHADOW_CBO_CFG_REG, RTID_CONFIG_POOLEF_BASE_SHADOW_CBO_CFG_REG,
                      RTID_CONFIG_POOLGH_BASE_SHADOW_CBO_CFG_REG 
                  };
  UINT32        PoolSizeReg[MAX_POOLS_HSX / 2] = {
                      RTID_CONFIG_POOL01_SIZE_CBO_CFG_REG, RTID_CONFIG_POOL23_SIZE_CBO_CFG_REG,
                      RTID_CONFIG_POOL45_SIZE_CBO_CFG_REG, RTID_CONFIG_POOL67_SIZE_CBO_CFG_REG,
                      RTID_CONFIG_POOL89_SIZE_CBO_CFG_REG, RTID_CONFIG_POOLAB_SIZE_CBO_CFG_REG,
                      RTID_CONFIG_POOLCD_SIZE_CBO_CFG_REG, RTID_CONFIG_POOLEF_SIZE_CBO_CFG_REG,
                      RTID_CONFIG_POOLGH_SIZE_CBO_CFG_REG
                  };

  RTID_POOL_CONFIG_SHADOW_CBO_CFG_HSX_STRUCT PoolCfg;

  // This function is called only for HSX and thus set the NumPools and
  // NumPoolSets for HSX-. Note that, in HSX- IJ pools are not available.
  NumPoolSets = MAX_POOL_SETS_HSX;
  NumPools    = MAX_POOLS_HSX;
  
  // All the double pool modes use ExtRTID mode
  QpiInternalGlobal->ExtRtidMode = TRUE;

  for (Soc = 0; Soc < MAX_CPU_SOCKETS; Soc++) {
    if ((host->var.qpi.RasInProgress == FALSE && (QpiInternalGlobal->CpuList & (1 << Soc))) ||
        (host->var.qpi.RasInProgress == TRUE && host->var.qpi.RasSocId == Soc)) {
      for (Cluster = 0; Cluster < 2; Cluster++) {
        // Assign the local HA pool numbers in LclHaPoolNum
        LclHaPoolNum[0] = (UINT8)SOCID_TO_NID(Soc, Cluster);  // Pool corresponding to HA NID of the current cluster in xRTID=0 pool
        LclHaPoolNum[1] = (Cluster==0)?16:17;                 // Pool corresponding to xRTID=1 pool for HA0(in cluster0)/HA1(in cluster1)

        for (Cbo=Cluster; Cbo < QpiInternalGlobal->TotCbo[Soc]; Cbo+=2) {
          //
          // Identify the RTID base & size allocated for this Cbo for local first xRTID=0 pool, local xRTID=1 pool, 
          // and remote first xRTID=0 pool
          //
          Idx1 = Cbo * 2 + 3; // Cbo RTIDs start from Index 3
          for (Idx2 = 0; Idx2 < 2; ++Idx2) { // Local & Remote HA pools
            for (Idx3 = 0; Idx3 < NumPoolSets; ++Idx3) { // local first xRTID=0 pool and xRTID=1 pool
              //
              // Set the remote xRTID=1 pool as 0 as they
              // are not applicable for remote HAs
              if (Idx2 == 1 && Idx3 == 1) {
                Base[Idx2][Idx3] = 0;
                Size[Idx2][Idx3] = 0;
              } else {
                Base[Idx2][Idx3] = QpiInternalGlobal->RtidBase[Idx2][Idx1 + Idx3];
                Size[Idx2][Idx3] = QpiInternalGlobal->RtidSize[Idx2][Idx1 + Idx3];
              } // Idx2 == 1 || Idx3 == 1 check
            } //for each pool type (1st,2nd)
          } //for each access type (Local,Remote)

          //
          // Program the pool base & size values for all the pools
          //
          for (Pool = 0; Pool < NumPools; ++Pool) {
            //
            // Identify the pool base and size values used by this pool. Also identify if this pool
            // is used as First pool or Second pool.
            //
            SecondPool[Pool % 2] = 0;

            //
            // Check if this pool is used for local HA communication. Also indentify
            // if this pool is First pool or Second Pool.
            //
            LocalPool = FALSE;

            for (Idx3 = 0; Idx3 < NumPoolSets; ++Idx3) { // local first xRTID=0 pool and xRTID=1 pool
              if (LclHaPoolNum[Idx3] == Pool) {
                LocalPool = TRUE;
                break;
              }
            } //for each pool - first or second

            // When LocalPool is set as TRUE and break happens, Idx3 points to the xRTID=0 pool (or) 
            // xRTID=1 pool
            if (LocalPool == TRUE) {
              TempBase[Pool % 2] = Base[0][Idx3]; // Local pool base
              TempSize[Pool % 2] = Size[0][Idx3]; // Local pool size

              if (Idx3 == 1) { // Is it the xRTID=1 pool? Then, set ExtRtid bit
                SecondPool[Pool % 2] = 1;
              } 
            } else {
              // This pool must be used for Remote HA communications. Verify if this pool is in local xRTID=0 pool.
              // In BtMode 3 and BtMode 2, there is no xRTID=1 pool for remote sockets or clusters
              if (Pool < 16) {
                TempBase[Pool % 2] = Base[1][0]; // remote pool base
                TempSize[Pool % 2] = Size[1][0]; // remote pool size
              } else {
                // This is for pools - G, H, one of which would not be applicable for remote HA communication.
                // For example, when writing GH pool register for S0-C0, G corresponding to local HA could be valid but
                // H corresponding to remote cluster's HA is not valid.
                SecondPool[Pool % 2] = 0;
                TempBase[Pool % 2] = 0; // remote pool base
                TempSize[Pool % 2] = 0; // remote pool size
              }
            }// Local vs. Remote HA check

            // Odd & Even pools share the same register. So program the pools by combining them to reduce number of
            // register reads/writes.
            //
            if (Pool % 2) {
              //
              // Program RTID Base & ExtendedMode settings
              //
              RegIdx = Pool / 2;
              Data32 = QpiReadPciCfg (host, Soc, Cbo, PoolBaseReg[RegIdx]);
              Data32 = (Data32 & 0xFDC0FDC0) | (SecondPool[1] << 25) | (TempBase[1] << 16) | (SecondPool[0] << 9) | TempBase[0];
              QpiWritePciCfg (host, Soc, Cbo, PoolBaseReg[RegIdx], Data32);

              //
              // Program Size
              //
              Data32 = QpiReadPciCfg (host, Soc, Cbo, PoolSizeReg[RegIdx]);
              Data32 = (Data32 & 0xFC3FFC3F) | (TempSize[1] << 22) | (TempSize[0] << 6);
              QpiWritePciCfg (host, Soc, Cbo, PoolSizeReg[RegIdx], Data32);
            }//If both halves of the CSR filled in (i.e. Odd # Pool) 
          }//For each Cbo Pool

          //
          // Program Pool Config Register
          //
          PoolCfg.Data = QpiReadPciCfg (host, Soc, Cbo, RTID_POOL_CONFIG_SHADOW_CBO_CFG_REG);
          PoolCfg.Bits.rtidpoolsel    = QpiInternalGlobal->CboPoolMode;                   // update rtidpoolsel - Bits[2:0], RWS, default =3'b00 
          PoolCfg.Bits.extendedrtiden = QpiInternalGlobal->ExtRtidMode;                   // update extendedrtiden - Bits[24:24], RWS, default =1'b0 
            QpiWritePciCfg (host, Soc, Cbo, RTID_POOL_CONFIG_SHADOW_CBO_CFG_REG, PoolCfg.Data);
          }//For each CBo in Cluster 
        }//For each Cluster
      }//If Socket is valid
    }//For each Socket

  // Update the scratch register in SBSP with ExtRTID mode info
  Data32  = QpiReadPciCfg (host, QpiInternalGlobal->SbspSoc, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG);
  Data32  = (Data32 & 0xFFFFFF7F) | (QpiInternalGlobal->ExtRtidMode <<7);
  QpiWritePciCfg (host, QpiInternalGlobal->SbspSoc, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG, Data32);

  return;
}
#endif // QPI_4S_COD_FEATURE_PROTOTYPE

VOID
ProgramRtid4SCODBdx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Programs allocated RTIDs for 4S COD configuration in BDX

Arguments:

  host  - Pointer to the system host (root) structure
  QpiInternalGlobal  - QPIRC global variables

Returns:

  VOID
  
--*/
{
  BOOLEAN      LocalPool;
  UINT8        Soc, Cbo, Pool, Cluster;
  UINT8        Idx1, Idx2, Idx3=0, RegIdx;
  UINT8        Base[2][MAX_POOL_SETS_BDX];
  UINT8        Size[2][MAX_POOL_SETS_BDX];
  UINT8        TempBase[2] = {0}, TempSize[2] = {0}, SecondPool[2] = {0};
  UINT8        LclHaPoolNum[MAX_POOL_SETS_BDX];
  UINT32       Data32;
  UINT8        NumPoolSets, NumPools;
  UINT32       PoolBaseReg[MAX_POOLS_BDX / 2] = {
                      RTID_CONFIG_POOL01_BASE_SHADOW_CBO_CFG_REG, RTID_CONFIG_POOL23_BASE_SHADOW_CBO_CFG_REG,
                      RTID_CONFIG_POOL45_BASE_SHADOW_CBO_CFG_REG, RTID_CONFIG_POOL67_BASE_SHADOW_CBO_CFG_REG,
                      RTID_CONFIG_POOL89_BASE_SHADOW_CBO_CFG_REG, RTID_CONFIG_POOLAB_BASE_SHADOW_CBO_CFG_REG,
                      RTID_CONFIG_POOLCD_BASE_SHADOW_CBO_CFG_REG, RTID_CONFIG_POOLEF_BASE_SHADOW_CBO_CFG_REG,
                      RTID_CONFIG_POOLGH_BASE_SHADOW_CBO_CFG_REG, 
                      RTID_CONFIG_POOLGH_BASE_SHADOW_CBO_CFG_REG    // Base for pools I,J are placed inside GH_BASE_SHADOW
                  };
  UINT32        PoolSizeReg[MAX_POOLS_BDX / 2] = {
                      RTID_CONFIG_POOL01_SIZE_CBO_CFG_REG, RTID_CONFIG_POOL23_SIZE_CBO_CFG_REG,
                      RTID_CONFIG_POOL45_SIZE_CBO_CFG_REG, RTID_CONFIG_POOL67_SIZE_CBO_CFG_REG,
                      RTID_CONFIG_POOL89_SIZE_CBO_CFG_REG, RTID_CONFIG_POOLAB_SIZE_CBO_CFG_REG,
                      RTID_CONFIG_POOLCD_SIZE_CBO_CFG_REG, RTID_CONFIG_POOLEF_SIZE_CBO_CFG_REG,
                      RTID_CONFIG_POOLGH_SIZE_CBO_CFG_REG, 
                      RTID_CONFIG_POOLEF_BASE_SHADOW_CBO_CFG_REG    // Size for pools I,J are placed inside EF_BASE_SHADOW
                  };

  RTID_POOL_CONFIG_SHADOW_CBO_CFG_BDX_STRUCT PoolCfg;

  // This function is called only for BDX and thus set the NumPools and
  // NumPoolSets for BDX-.
  NumPoolSets = MAX_POOL_SETS_BDX;
  NumPools    = MAX_POOLS_BDX;

  // All the double pool modes use ExtRTID mode
  QpiInternalGlobal->ExtRtidMode = TRUE;

  for (Soc = 0; Soc < MAX_CPU_SOCKETS; Soc++) {
    if ((host->var.qpi.RasInProgress == FALSE && (QpiInternalGlobal->CpuList & (1 << Soc))) ||
        (host->var.qpi.RasInProgress == TRUE && host->var.qpi.RasSocId == Soc)) {
      for (Cluster = 0; Cluster < 2; Cluster++) {
        // Assign the local HA pool numbers in LclHaPoolNum
        LclHaPoolNum[0] = (UINT8)SOCID_TO_NID(Soc, Cluster);  // Pool corresponding to HA NID of the current cluster in xRTID=0 pool
        LclHaPoolNum[1] = (Cluster==0)?16:17;                 // Pool corresponding to xRTID=1 pool for HA0(in cluster0)/HA1(in cluster1)
        LclHaPoolNum[2] = (Cluster==0)?18:19;                 // Pool corresponding to the second pool in xRTID=0 for HA0(in cluster0)/HA1(in cluster1)

        for (Cbo=Cluster; Cbo < QpiInternalGlobal->TotCbo[Soc]; Cbo+=2) {
          //
          // Identify the RTID base & size allocated for this Cbo for local first xRTID=0 pool, local xRTID=1 pool, 
          // local second xRTID=0 pool and remote first xRTID=0 pool
          //
          // In BDX 4S-COD, Cbos reuse the RTIDs across the clusters. For example, a 24 Cbo HCC part would have the 12 Cbos on left cluster
          // and 12 Cbos on right cluster share the RTIDs. Adjust the Cbo such that 0-23 uses 0-11 in reality.
          Idx1 = ((Cbo / 2) * 2) + 3; // Cbo RTIDs start from Index 3
          
          for (Idx2 = 0; Idx2 < 2; ++Idx2) { // Local & Remote HA pools
            for (Idx3 = 0; Idx3 < NumPoolSets; ++Idx3) { // local first xRTID=0 pool, xRTID=1 pool and local second xRTID=0 pool
              //
              // Set the remote xRTID=1 pool and remote second xRTID=0 pool as 0 as they
              // are not applicable for remote HAs
              if ((Idx2 == 1) && (Idx3 >= 1)) {
                Base[Idx2][Idx3] = 0;
                Size[Idx2][Idx3] = 0;
              } else {
                if ((host->var.common.cpuType == CPU_BDX) && (Idx3 == (MAX_POOL_SETS_BDX - 1))) { // Idx3 can reach MAX_POOLSETS_BDX only for BDX- flow
                  Base[Idx2][Idx3] = QpiInternalGlobal->RtidBase[Idx3][Idx1 + Idx2];
                  Size[Idx2][Idx3] = QpiInternalGlobal->RtidSize[Idx3][Idx1 + Idx2];
                } else {
                  Base[Idx2][Idx3] = QpiInternalGlobal->RtidBase[Idx2][Idx1 + Idx3];
                  Size[Idx2][Idx3] = QpiInternalGlobal->RtidSize[Idx2][Idx1 + Idx3];
                 } // Idx3 check
              } // Idx2 == 1 || Idx3 >= 1 check
            } //for each pool type (1st,2nd)
          } //for each access type (Local,Remote)

          //
          // Program the pool base & size values for all the pools
          //
          for (Pool = 0; Pool < NumPools; ++Pool) {
            //
            // Identify the pool base and size values used by this pool. Also identify if this pool
            // is used as First pool or Second pool.
            //
            SecondPool[Pool % 2] = 0;

            //
            // Check if this pool is used for local HA communication. Also indentify
            // if this pool is First pool or Second Pool.
            //
            LocalPool = FALSE;

            for (Idx3 = 0; Idx3 < NumPoolSets; ++Idx3) { // local first xRTID=0 pool, xRTID=1 pool and local second xRTID=0 pool
              if (LclHaPoolNum[Idx3] == Pool) {
                LocalPool = TRUE;
                break;
              }
            } //for each pool - first or second

            // When LocalPool is set as TRUE and break happens, Idx3 points to the xRTID=0 pool (or) 
            // xRTID=1 pool (or) local second xRTID=0 pool
            if (LocalPool == TRUE) {
              TempBase[Pool % 2] = Base[0][Idx3]; // Local pool base
              TempSize[Pool % 2] = Size[0][Idx3]; // Local pool size

              if (Idx3 == 1) { // Is it the xRTID=1 pool? Then, set ExtRtid bit
                SecondPool[Pool % 2] = 1;
              } 
            } else {
              // This pool must be used for Remote HA communications. Verify if this pool is in local xRTID=0 pool.
              // In BtMode 3 and BtMode 2, there is no xRTID=1 pool for remote sockets or clusters
              if (Pool < 16) {
                TempBase[Pool % 2] = Base[1][0]; // remote pool base
                TempSize[Pool % 2] = Size[1][0]; // remote pool size
              } else {
                // This is for pools - G, H, I or J, one of which would not be applicable for remote HA communication.
                // For example, when writing GH pool register for S0-C0, G corresponding to local HA could be valid but
                // H corresponding to remote cluster's HA is not valid.
                SecondPool[Pool % 2] = 0;
                TempBase[Pool % 2] = 0; // remote pool base
                TempSize[Pool % 2] = 0; // remote pool size
              }
            }// Local vs. Remote HA check

            // Odd & Even pools share the same register. So program the pools by combining them to reduce number of
            // register reads/writes.
            //
            if (Pool % 2) {
              //
              // Program RTID Base & ExtendedMode settings
              //
              RegIdx = Pool / 2;
              Data32 = QpiReadPciCfg (host, Soc, Cbo, PoolBaseReg[RegIdx]);
              if (Pool < 18) {
                if (host->var.common.cpuType == CPU_BDX) {
                  // We can have the sizes in GH registers as zero. In such a case, we should override Ext RTID bit as 0 as the size is 0
                  if (TempSize[0] == 0) {
                    SecondPool[0] = 0;
                  }

                  if (TempSize[1] == 0) {
                    SecondPool[1] = 0;
                  }
                } // cpuType check

                Data32 = (Data32 & 0xFDC0FDC0) | (SecondPool[1] << 25) | (TempBase[1] << 16) | (SecondPool[0] << 9) | TempBase[0];
              } else {
                // Set the RTID bases for Pool I and J(BDX only). Base for pools I,J are placed inside GH_BASE_SHADOW
                Data32 = (Data32 & 0x03FF03FF) | (TempBase[1] << 26) | (TempBase[0] << 10);
              } // (Pool < 18) check
              QpiWritePciCfg (host, Soc, Cbo, PoolBaseReg[RegIdx], Data32);

              //
              // Program Size
              //
              Data32 = QpiReadPciCfg (host, Soc, Cbo, PoolSizeReg[RegIdx]);
              if (Pool < 18) {
                Data32 = (Data32 & 0xFC3FFC3F) | (TempSize[1] << 22) | (TempSize[0] << 6);
              } else {
                // Set the RTID sizes for Pool I and J(BDX only). Size for pools I,J are placed inside EF_BASE_SHADOW
                Data32 = (Data32 & 0X0FFF87FF) | (TempSize[1] << 28) | (TempSize[0] << 11);
              } // (Pool < 18) check
              QpiWritePciCfg (host, Soc, Cbo, PoolSizeReg[RegIdx], Data32);
            }//If both halves of the CSR filled in (i.e. Odd # Pool) 
          }//For each Cbo Pool

          //
          // Program Pool Config Register
          //
          PoolCfg.Data = QpiReadPciCfg (host, Soc, Cbo, RTID_POOL_CONFIG_SHADOW_CBO_CFG_REG);
          PoolCfg.Bits.rtidpoolsel    = QpiInternalGlobal->CboPoolMode;                   // update rtidpoolsel - Bits[2:0], RWS, default =3'b00 
          PoolCfg.Bits.extendedrtiden = QpiInternalGlobal->ExtRtidMode;                   // update extendedrtiden - Bits[24:24], RWS, default =1'b0 

          // Set the BDX specific Bt-Mode 2 bit to also select the local second xRTID=0 pool
          if (QpiInternalGlobal->CboPoolMode == DOUBLE_POOL8S_MODE) {
            PoolCfg.Bits.enbdxbt2rtidpool = 1;
          } 
 
            QpiWritePciCfg (host, Soc, Cbo, RTID_POOL_CONFIG_SHADOW_CBO_CFG_REG, PoolCfg.Data);
          }//For each CBo in Cluster 
        }//For each Cluster
      }//If Socket is valid
    }//For each Socket

  // Update the scratch register in SBSP with ExtRTID mode info
  Data32  = QpiReadPciCfg (host, QpiInternalGlobal->SbspSoc, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG);
  Data32  = (Data32 & 0xFFFFFF7F) | (QpiInternalGlobal->ExtRtidMode <<7);
  QpiWritePciCfg (host, QpiInternalGlobal->SbspSoc, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG, Data32);

  return;
}

QPI_STATUS
ProgramRingCredits (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Add function description

Arguments:

  host        - add argument description
  SocketData  - add argument description
  QpiInternalGlobal - QPIRC Internal Global data

Returns:

  add return values

--*/
{
  if (host->var.qpi.SysConfig == SYS_CONFIG_1S) {
    ProgramRingCredits1S (host, SocketData, QpiInternalGlobal);
  } else {
    ProgramRingCreditsCommonHsxAndBdx (host, SocketData, QpiInternalGlobal);
  } // 1S Check
  return QPI_SUCCESS;
}



QPI_STATUS
ProgramRingCredits1S (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Programt the ring credits that require reset for it to take effect.

Arguments:

  host        - add argument description
  SocketData  - add argument description
  QpiInternalGlobal - QPIRC Internal Global data

Returns:

  add return values

--*/
{
  UINT8                 Sbsp, Cbo, BlVnaR2Pcie, TotalCboCredit, NumHtCredits = 2;
  UINT32                Data32, MaskForR3QCBOHACR;

  Sbsp = QpiInternalGlobal->SbspSoc;

  MaskForR3QCBOHACR = 0x00FF0000;
  if (host->var.common.cpuType == CPU_HSX) {
    TotalCboCredit = TOT_CBO_CREDIT_HSX;    // Set numsnpcreditsl0 and numsnpcreditsl1, ha0creditr0, ha1creditr0, ha0creditr1, ha1creditr1 to 0
  } else {
    TotalCboCredit = TOT_CBO_CREDIT_BDX;    // Set numsnpcreditsl0 and numsnpcreditsl1, ha0creditr0, ha1creditr0, ha0creditr1, ha1creditr1 to 0
  }

  // Cbo
  BlVnaR2Pcie = (TotalCboCredit / QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc]);

  // Grantley BDX HSD : 4988155 : [BDX-DE] : Saturate the BL_VNA_R2PCIe credits to max. value
  // in case of over-flow in low core counts (broadwell_server 5096982)
  if (BlVnaR2Pcie > 7) { // register field is 3-bits
    BlVnaR2Pcie = 7;
  }

  // Brickland BDX HSD 4971313 : Clone from BDX Sighting:BDX EX : BT TO seen on 24C EX sku parts with memicals traffic passes on 23C EX
  // Updated to BDX- Buffer Credit Sheet v.4.6
  if (host->var.common.cpuType == CPU_BDX) {
    NumHtCredits = (UINT8) (TOT_CBO_CREDIT_BDX / QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc]);

    if (NumHtCredits > 3) {
      NumHtCredits = 3;
    }
  } // cpuType check

  for (Cbo = 0; Cbo < QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc]; ++Cbo) {
    Data32 = QpiReadPciCfg (host, Sbsp, Cbo, VNA_CREDIT_CONFIG_CBO_CFG_REG);
    Data32 = Data32 | (1 << 31); // Set disisocvnres field
    Data32 = (Data32 & 0xFFFF8FFF) | (BlVnaR2Pcie << 12); // Set BL_VNA_R2PCIE field

    // Brickland BDX HSD 4971313 : Clone from BDX Sighting:BDX EX : BT TO seen on 24C EX sku parts with memicals traffic passes on 23C EX
    // Updated to BDX- Buffer Credit Sheet v.4.6
    if (host->var.common.cpuType == CPU_BDX) {
      Data32 = (Data32 & 0xFE1FFFFF) | ( NumHtCredits << 21) | (NumHtCredits << 23);
    } // cpuType check
    QpiWritePciCfg (host, Sbsp, Cbo, VNA_CREDIT_CONFIG_CBO_CFG_REG, Data32);
  }

  
  return QPI_SUCCESS;
}
 
BOOLEAN
IdentifyLeftCbo (
  struct sysHost      *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8               Cbo
  )
/*
  If (PHYSICAL_CHOP==HCC)
                If(LLC_SLICE_ENABLE==0x3ffff)                                                // case 1
                     Left Ring CBos: {0, 2, 4, 6, 8, 10, 12, 14}
                     Right Ring CBos: {16, 1, 3, 5, 7, 9, 11, 13, 15, 17}
                Else if(NUMBER_ENABLED_CBOS==16 && LLC_SLICE_ENABLE!=0x33fff)                // case 2: 16 CBos enabled and CBos 14 and 15 are enabled (physical ID)
                     Left Ring CBos: {0, 2, 4, 6, 8, 10, 12}
                     Right Ring CBos: {14, 1, 3, 5, 7, 9, 11, 13, 15}
                Else                                                                         // case 3
                     Left Ring CBos: {All even IDs}
                     Right Ring CBos: {All odd IDs}
  Else
                Allocate all CBos equal VNA credits (use the left ring allocation for this since that is the lesser amount).

  Case 1 is the obvious 18-core case, just have to account for physical-to-logical mapping without any disables to worry about. 
  Case 2 is the fuse-down case where one CBo is disabled from each ring - 7 CBos on left ring,
         9 on the right ring (see attached slide for supported fuse down combos). 
  Case 3 is the case where each ring has equal number of enabled CBos.

  return : TURE  -> left Cbo
           FALSE -> right Cbo
*/                
{

  BOOLEAN  LeftCbo = FALSE;
  UINT8  sbspSktId;
  CAPID4_PCU_FUN3_BDX_STRUCT      Capid4_Bdx;
  CAPID5_PCU_FUN3_BDX_STRUCT      Capid5_Bdx;

  CAPID4_PCU_FUN3_HSX_STRUCT      Capid4_Hsx;
  CAPID5_PCU_FUN3_HSX_STRUCT      Capid5_Hsx;

  UINT32                          Capid4_Bits_physical_chop;
  UINT32                          Capid5_Bits_llc_slice_en;

  sbspSktId = QpiInternalGlobal->SbspSoc;
 
  if (host->var.common.cpuType == CPU_HSX) {
    Capid4_Hsx.Data = QpiReadPciCfg (host, sbspSktId, 0, CAPID4_PCU_FUN3_REG);
    Capid5_Hsx.Data = QpiReadPciCfg (host, sbspSktId, 0, CAPID5_PCU_FUN3_REG);
    Capid4_Bits_physical_chop = Capid4_Hsx.Bits.physical_chop;
    Capid5_Bits_llc_slice_en  = Capid5_Hsx.Bits.llc_slice_en;
  } else {
    Capid4_Bdx.Data = QpiReadPciCfg (host, sbspSktId, 0, CAPID4_PCU_FUN3_REG);
    Capid5_Bdx.Data = QpiReadPciCfg (host, sbspSktId, 0, CAPID5_PCU_FUN3_REG);
    Capid4_Bits_physical_chop = Capid4_Bdx.Bits.physical_chop;
    Capid5_Bits_llc_slice_en  = Capid5_Bdx.Bits.llc_slice_en;
  }

  if (Capid4_Bits_physical_chop == HCC) {      
    if((Cbo % 2) == 0){
      LeftCbo = TRUE;             //assume even cbo is left cbo
    } else {
      LeftCbo = FALSE;            //       odd cbo is right cbo 
    }

    // For BDX, the Cbos in case of HCC are arranged as 6 per each column and 12 per each cluster. 
    // There is no imbalances as in HSX HCC. Thus, all even Cbos are in left cluster while odd Cbos are in right cluster.
    //
    if (host->var.common.cpuType == CPU_BDX) {
      return LeftCbo;
    } // cpuType check

    if (Capid5_Bits_llc_slice_en == 0x3FFFF) {                                                          //case 1
      if( Cbo == 16){
        LeftCbo = FALSE;        //cbo #16 is right cbo         
      } 
    } else if ((QpiInternalGlobal->TotCbo[sbspSktId] == 16) && (Capid5_Bits_llc_slice_en != 0x33fff)) { //case 2
      if( Cbo == 14){
        LeftCbo = FALSE;        //cbo #14 is right cbo      
      } 
    }
  } else {             //For MCC and LCC, use leftcbo value
    LeftCbo = TRUE;    
  }

  return LeftCbo;
}

QPI_STATUS
ProgramRingCreditsCommonHsxAndBdx (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Program the ring credits that require reset for it to take effect.

Arguments:

  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data

Returns:

  add return values

--*/
{
  UINT8                 Soc, R3Qpi, Port, Cbo, ad_vna_r3qpi0,bl_vna_r3qpi0, ConfigType, CboR2PCIeCredits, NumHtCredits = 2;
  UINT32                Data32, TotalCboCredit = 0;
  VNA_CREDIT_CONFIG_CBO_CFG_STRUCT  VnaCrdtCfg = {0};   
  R3QPCIR0CR_R3QPI_CFG_STRUCT  R3QpiPciRingCrdtCtrl = {0};
  QPIA2RCRCTRL_QPIAGENT_LL_STRUCT  QpiAgtCrdtCtrl = {0};
  R3QRTE0CR_R3QPI_CFG_STRUCT        R3QpiRtEgressCrdtCtrl = {0};
  R3QCBOHACR_R3QPI_CFG_HSX_BDX_STRUCT  R3qCboHaCr;
  CreditTablePara   *CreditTableParam = NULL;
  BOOLEAN           LeftCbo;
      

  // Identify the config type
  ConfigType = QpiInternalGlobal->SysConfigType;
  if (host->var.common.cpuType == CPU_HSX) {
    CreditTableParam = &CreditTableHsx[ConfigType];
  } else {
    CreditTableParam = &CreditTableBdx[ConfigType];
  }

  if (CreditTableParam == NULL) {
    QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);;
  } 
  // 
  // Allocate credits for all the ring agents communication.
  // 

  // Credits for CBo->QPI transactions
  if (host->var.common.cpuType == CPU_HSX) {
    TotalCboCredit = TOT_CBO_CREDIT_HSX;    // Set numsnpcreditsl0 and numsnpcreditsl1, ha0creditr0, ha1creditr0, ha0creditr1, ha1creditr1 to 0
  } else {
    TotalCboCredit = TOT_CBO_CREDIT_BDX;    // Set numsnpcreditsl0 and numsnpcreditsl1, ha0creditr0, ha1creditr0, ha0creditr1, ha1creditr1 to 0
  } 

  CboR2PCIeCredits = (UINT8) (TotalCboCredit / QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc]);

  // Grantley BDX HSD : 4988155 : [BDX-DE] : Saturate the BL_VNA_R2PCIe credits to max. value
  // in case of over-flow in low core counts (broadwell_server 5096982)
  if (CboR2PCIeCredits > 7) { // register field is 3-bits
    CboR2PCIeCredits = 7;
  }

  VnaCrdtCfg.Bits.bl_vna_r2pcie = CboR2PCIeCredits;
  
  //
  // Program the values into each socket.
  //
  for (Soc = 0; Soc < MAX_CPU_SOCKETS; ++Soc) {
    if (SocketData->Cpu[Soc].Valid == TRUE) {

      // Cbo
      for (Cbo = 0; Cbo < QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc]; ++Cbo) {
        Data32 = QpiReadPciCfg (host, Soc, Cbo, VNA_CREDIT_CONFIG_CBO_CFG_REG);

        // When ISOCH disabled, ISOCH credit reservation in Cbo should be disabled by setting disisocvnres.
        // and when enabled all Cbos in Non-Legacy socket should set disisocvnres field.
        //
        Data32 = Data32 | (1 << 31); // Set disisocvnres field to disable reserving credits for isoch
        if (host->var.qpi.OutIsocEn == TRUE) {
          Data32 = Data32 & 0x7FFFFFFF; // Clear disisocvnres field to allow reserving credits for isoch
        }

        Data32 = Data32 & 0xFE000000; // Clear AD/BL VNA R3QPI 0/1, r2pcie, numhtcred_ha0, numhtcred_ha1  fields

        LeftCbo = IdentifyLeftCbo(host, QpiInternalGlobal, Cbo);

        if( LeftCbo == TRUE) {                                        //left cbo
          ad_vna_r3qpi0 = CreditTableParam->left_ad_vna_r3qpi0;       //ad vna left  cbo table
          bl_vna_r3qpi0 = CreditTableParam->left_bl_vna_r3qpi0;       //bl vna left  cbo table
        } else {                                                      //right cbo
          ad_vna_r3qpi0 = CreditTableParam->right_ad_vna_r3qpi0;      //vna ad right cbo table
          bl_vna_r3qpi0 = CreditTableParam->right_bl_vna_r3qpi0;      //bl vna right cbo table
          // HSX B0 W/A: 4986057: EP early snoop workaround missing VNA credits reduction for B0 BIOS
          if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.stepping < C0_REV_HSX)) {
            switch (ConfigType) {
              case EP_HCC_ES_ISOC_DISABLED:
              case EP_HCC_SHT_ISOC_ENABLED:
                ad_vna_r3qpi0 =  3;  //HSX A0/B0 High core count (HCC) VNA AD credits for right-side Cbos when running Early Snoop
                break;
              case EP_MCC_ES_ISOC_DISABLED:
              case EP_MCC_SHT_ISOC_ENABLED:
                ad_vna_r3qpi0 = 5;  //HSX A0/B0 Medium core count (MCC) VNA AD credits for right-side Cbos when running Early Snoop
                break;
            }//switch
          }//if HSX A0/B0
        }//if left or right Cbo 

        // Credits for CBo->QPI transactions; the setting is same for odd/even Cbos and port 0/1 fields
        VnaCrdtCfg.Bits.ad_vna_r3qpi1 = VnaCrdtCfg.Bits.ad_vna_r3qpi0 = ad_vna_r3qpi0;
        VnaCrdtCfg.Bits.bl_vna_r3qpi1 = VnaCrdtCfg.Bits.bl_vna_r3qpi0 = bl_vna_r3qpi0;

        // for qpi port 2
        if((host->var.qpi.SysConfig >= SYS_CONFIG_4S) && (QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_EXA)) {
          if( LeftCbo == TRUE) {                                      //left cbo
            ad_vna_r3qpi0 = CreditTableParam->right_ad_vna_r3qpi0;    //ad vna right  cbo table
            bl_vna_r3qpi0 = CreditTableParam->right_bl_vna_r3qpi0;    //bl vna right  cbo table
          } else {                                                    //right cbo
            ad_vna_r3qpi0 = CreditTableParam->left_ad_vna_r3qpi0;     //vna ad left cbo table
            bl_vna_r3qpi0 = CreditTableParam->left_bl_vna_r3qpi0;     //bl vna left cbo table
          } 

          VnaCrdtCfg.Bits.ad_vna_r3qpi2 =  ad_vna_r3qpi0;
          VnaCrdtCfg.Bits.bl_vna_r3qpi2 =  bl_vna_r3qpi0;
        }

        // Brickland BDX HSD 4971313 : Clone from BDX Sighting:BDX EX : BT TO seen on 24C EX sku parts with memicals traffic passes on 23C EX
        // Updated to BDX- Buffer Credit Sheet v.4.6
        if (host->var.common.cpuType == CPU_HSX) {
          if (QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc] <= 12) {
            VnaCrdtCfg.Bits.numhtcred_ha0 = VnaCrdtCfg.Bits.numhtcred_ha1 = 3;
          } else {
            VnaCrdtCfg.Bits.numhtcred_ha0 = VnaCrdtCfg.Bits.numhtcred_ha1 = 2;
          }
        } else { // For CPU_BDX
          NumHtCredits = (UINT8) (TOT_CBO_CREDIT_BDX / QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc]);

          if (NumHtCredits > 3) {
            NumHtCredits = 3;
          }

          VnaCrdtCfg.Bits.numhtcred_ha0 = VnaCrdtCfg.Bits.numhtcred_ha1 = NumHtCredits;
        } // cpuType check

        Data32 = Data32 | VnaCrdtCfg.Data; 

        QpiWritePciCfg (host, Soc, Cbo, VNA_CREDIT_CONFIG_CBO_CFG_REG, Data32);
      }

 
      // R3QPI
      R3QpiPciRingCrdtCtrl.Bits.pcincscredit = 2;
      R3QpiPciRingCrdtCtrl.Bits.pcincbcredit = 2;

      // R3QRTE0CR or R3QRTE1CR - used for route-through traffic
      R3QpiRtEgressCrdtCtrl.Bits.advnaqpi = 4;        
      if (QpiInternalGlobal->Is4SRing == TRUE) {
         R3QpiRtEgressCrdtCtrl.Bits.advnaqpi = 8;    
      }
      R3QpiRtEgressCrdtCtrl.Bits.vn0snpqpi = 1;          
      R3QpiRtEgressCrdtCtrl.Bits.vn0homqpi = 1;          
      R3QpiRtEgressCrdtCtrl.Bits.vn0ndrqpi = 1;
      
      if (host->var.common.cpuType == CPU_BDX) {
        R3QpiRtEgressCrdtCtrl.Bits.blvnaqpi  = 6;
      } else { // For CPU_HSX
        R3QpiRtEgressCrdtCtrl.Bits.blvnaqpi  = 2;
      }
      R3QpiRtEgressCrdtCtrl.Bits.vn0drsqpi = 1;          
      R3QpiRtEgressCrdtCtrl.Bits.vn0ncsqpi = 1;          
      R3QpiRtEgressCrdtCtrl.Bits.vn0ncbqpi = 1; 


      for (R3Qpi = 0; R3Qpi < QpiInternalGlobal->TotR3Qpi[Soc]; ++R3Qpi) {

        Data32 = QpiReadPciCfg(host, Soc, R3Qpi, R3QPCIR0CR_R3QPI_CFG_REG);
        Data32 = (Data32 & 0xFFFFF88F) | R3QpiPciRingCrdtCtrl.Data;
        QpiWritePciCfg (host, Soc, R3Qpi, R3QPCIR0CR_R3QPI_CFG_REG, Data32);

        //
        // R3QPCIR NCS/NCB credits
        //
        Data32 = QpiReadPciCfg(host, Soc, R3Qpi, R3QPCIR1CR_R3QPI_CFG_REG);
        Data32 = (Data32 & 0xFFFFF88F) | R3QpiPciRingCrdtCtrl.Data;
        QpiWritePciCfg (host, Soc, R3Qpi, R3QPCIR1CR_R3QPI_CFG_REG, Data32);

        // 
        R3qCboHaCr.Data = QpiReadPciCfg (host, Soc, R3Qpi, R3QCBOHACR_R3QPI_CFG_REG);
        R3qCboHaCr.Data = Data32 & 0x00FF0000;            // Clear Set numsnpcreditsl0 and numsnpcreditsl1, ha0creditr0, ha1creditr0, ha0creditr1, ha1creditr1 to 0
     
        if (QpiInternalGlobal->BtModeEn) {
           R3qCboHaCr.Bits.hacrediten = 1;                // Set hacrediten to 1 
        }
        R3qCboHaCr.Bits.numsnpcreditsl0 = R3qCboHaCr.Bits.numsnpcreditsl1 = CreditTableParam->Qpi_To_Cbo_Ipq;
        R3qCboHaCr.Bits.ha0creditr0 = R3qCboHaCr.Bits.ha0creditr1 = R3qCboHaCr.Bits.ha1creditr0 = R3qCboHaCr.Bits.ha1creditr1 = CreditTableParam->Qpi_To_Ha_WB_Credit;
          
      
        QpiWritePciCfg (host, Soc, R3Qpi, R3QCBOHACR_R3QPI_CFG_REG, R3qCboHaCr.Data);

        //
        // R3QRTECR - shared (AD/BL VNA) and VN0 credits. VN1 uses the same value as VN0.
        //
        
        Data32 = QpiReadPciCfg (host, Soc, R3Qpi, R3QRTE0CR_R3QPI_CFG_REG);
        Data32 = (Data32 & 0xFC00FC00) | R3QpiRtEgressCrdtCtrl.Data;
        QpiWritePciCfg (host, Soc, R3Qpi, R3QRTE0CR_R3QPI_CFG_REG, Data32);

        Data32 = QpiReadPciCfg (host, Soc, R3Qpi, R3QRTE1CR_R3QPI_CFG_REG);
        Data32 = (Data32 & 0xFC00FC00) | R3QpiRtEgressCrdtCtrl.Data;
        QpiWritePciCfg (host, Soc, R3Qpi, R3QRTE1CR_R3QPI_CFG_REG, Data32);
        
        if (host->setup.common.PSMIEnabledSupport) {
          Data32 = QpiReadPciCfg (host, Soc, R3Qpi, R3QE0CR_R3QPI_CFG_REG);
          Data32 = (Data32 & 0xFFF8000F) | (1 << 14) | (1 << 9) | (1 << 4);
          QpiWritePciCfg (host, Soc, R3Qpi, R3QE0CR_R3QPI_CFG_REG, Data32);
        }

        // For HSX C0+, clear r3qctrl.flowq_vn1enable if not using VN1 or early snoop.
        // This step is needed because r3qctrl.flowq_vn1enable defaults to 1 on cold reset.
        // For HSX A0/B0, 'flowq_vn1enable' is not defined so we don't need to worry about it.
        if ((host->var.common.cpuType == CPU_HSX && host->var.common.stepping >= C0_REV_HSX) ||
            (host->var.common.cpuType == CPU_BDX)) {
          if ((QpiInternalGlobal->Vn1En == FALSE) && (QpiInternalGlobal->SysSnoopMode != EARLY_SNOOP_MODE)) {
            Data32 = QpiReadPciCfg (host, Soc, R3Qpi, R3QCTRL_R3QPI_CFG_REG);
            Data32 = Data32 & (~(1 << 7));     //clear flowq_vn1enable        
            QpiWritePciCfg (host, Soc, R3Qpi, R3QCTRL_R3QPI_CFG_REG, Data32);
          } 
        }

      } //R3QPI

      // QPI Agent
      //
      // QPIA2RCRCTRL (port x, vn0) credits, 
      //
      // HSX W/A (Permanent) (4166978):  HSX Clone: CBO TOR Timeout : Memory Stress with Early Snoop Enabled
      // Use reduced credits (ndr_ad, snp, home) when VN1 (flowq_vn1enable) is enabled either
      // directly via Vn1En or indirectly via the Early Snoop W/A.
      // This affects isoc too since ES is used in isoc config.
      // 
      // If early snoop is enabled, 
      //    Program Qpia2rcrctrl to not exceed 32 flowq credits per VN (0 or 1) if necessary.
      if( (QpiInternalGlobal->Vn1En == TRUE) || (QpiInternalGlobal->SysSnoopMode == EARLY_SNOOP_MODE)){
        QpiAgtCrdtCtrl.Bits.ndr_ad = 4;
        QpiAgtCrdtCtrl.Bits.home = 14;
        QpiAgtCrdtCtrl.Bits.snp = 14;
      } else { // vn0 only
        QpiAgtCrdtCtrl.Bits.ndr_ad = 8;
        QpiAgtCrdtCtrl.Bits.home = 28;
        QpiAgtCrdtCtrl.Bits.snp = 28; 
      }
      QpiAgtCrdtCtrl.Bits.bl_shr = 6;
      QpiAgtCrdtCtrl.Bits.drs = 1;

      //s4166453. HSX A0 B0 WA - Do not program the home field of the QPIA2RCRCTRL register to a value greater than 0xe (the default).
      if (host->var.common.cpuType == CPU_HSX && host->var.common.stepping < C0_REV_HSX) {
         QpiAgtCrdtCtrl.Bits.home = 14;         
      }
           
      for (Port = 0; Port < MAX_QPI_PORTS; ++Port) {
        if (SocketData->Cpu[Soc].LinkData[Port].Valid == TRUE) {
          Data32 = QpiReadPciCfg (host, Soc, Port, QPIA2RCRCTRL_QPIAGENT_LL_REG);
          Data32 = (Data32 & 0xFF0E0000) | QpiAgtCrdtCtrl.Data;
          if (host->setup.common.PSMIEnabledSupport) {
            Data32 = (Data32 & 0x1E3FF) | 1 << 28 | 1 << 24 | 1 << 20 | 1 << 17 | 1 << 10;
          }          
          QpiWritePciCfg (host, Soc, Port, QPIA2RCRCTRL_QPIAGENT_LL_REG, Data32);
        }
      }
    }
  }

  return QPI_SUCCESS;
}
   

VOID
DumpRtidAllocationSHT (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Dump RTID allocation to output stream

Arguments:

  host  - Pointer to the system host (root) structure
  QpiInternalGlobal - QPIRC Internal Global data

Returns:

  VOID

--*/
{
  UINT8      Idx1, Idx2, Soc;
  BOOLEAN    Skip;
  UINT8      (*TempRtidBase)[MAX_CBO * 2 + 4];
  UINT8      (*TempRtidSize)[MAX_CBO * 2 + 4];

  Idx2 = QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc] * 2 + 3;

  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n; RTID Allocation Table"));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; ---------------------"));

  for (Soc = 0; Soc < 2; ++Soc) {
    if (host->var.qpi.SysConfig != SYS_CONFIG_1S && host->var.qpi.OutIsocEn == TRUE) {
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;  S%u:", Soc));
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;          Local          Neighbor"));
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;          -----          --------"));
      if (Soc == 0) {
        TempRtidBase = QpiInternalGlobal->RtidBaseIsocS0;
        TempRtidSize = QpiInternalGlobal->RtidSizeIsocS0;
      } else {
        TempRtidBase = QpiInternalGlobal->RtidBaseIsocS1;
        TempRtidSize = QpiInternalGlobal->RtidSizeIsocS1;
      }
    } else {
      if (Soc != 0) {
        continue;
      }
      TempRtidBase = QpiInternalGlobal->RtidBase;
      TempRtidSize = QpiInternalGlobal->RtidSize;
      if (host->var.qpi.SysConfig == SYS_CONFIG_1S) {
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;          Local"));
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;          -----"));
      } else {
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;          Local          Neighbor"));
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;          -----          --------"));
      }
    }


    for (Idx1 = 0; Idx1 < (MAX_CBO * 2 + 4); ++Idx1) {
      Skip = FALSE;

      switch (Idx1) {
      case 0:
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  WB   "));
        break;

      case 1:
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  UBOX "));
        break;

      case 2:
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  ISOC "));
        break;

      case MAX_CBO * 2 + 3:
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  EXTRA"));
        break;

      default:
        if ((TempRtidSize[0][Idx1] == 0) && (TempRtidSize[1][Idx1] == 0)) {
          Skip = TRUE;
        } else {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  CBO%02u", (Idx1 - 3) / 2));
        }
        break;
      }

      if (Skip == TRUE) {
        continue;
      }

      if (host->var.qpi.SysConfig == SYS_CONFIG_1S) {
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "   %3u  %3u ", TempRtidBase[0][Idx1], TempRtidSize[0][Idx1]));
      } else {
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "   %3u  %3u         %3u  %3u ", TempRtidBase[0][Idx1], TempRtidSize[0][Idx1],
                  TempRtidBase[1][Idx1], TempRtidSize[1][Idx1]));
      }
    }
  }

  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n"));
  return;
}


VOID
DumpRtidAllocation (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Dump RTID allocation to output stream
  QpiInternalGlobal - QPIRC Internal Global data

Arguments:

  host  - Pointer to the system host (root) structure

Returns:

  VOID

--*/
{
  UINT8    Idx1, Pool;
  BOOLEAN  Skip;

  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n; RTID Allocation Table"));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; ---------------------"));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;          Local          Neighbor"));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;          -----          --------"));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;         Base  Size    Base  Size"));

  for (Pool = 0; Pool < 2; ++Pool) { // First & Second Pool

    QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;  %s Pool",(Pool == 0) ? "First" : "Second"));
    QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  ----------"));

    for (Idx1 = 0; Idx1 < MAX_CBO * 2 + 4; ++Idx1) {
      Skip = FALSE;

      switch (Idx1) {
      case 0:
        if (Pool == 0) {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  WB   "));
        } else {
          Skip = TRUE;
        }
        break;

      case 1:
        if (Pool == 0) {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  UBOX "));
        } else {
          Skip = TRUE;
        }
        break;

      case 2: // Isoch
        if (Pool == 0) {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  MESEG"));
        } else {
          Skip = TRUE;
        }
        break;

      case MAX_CBO * 2 + 3: // Extra RTIDs
          Skip = TRUE;
        break;

      default:
        if (((Pool == 0) && (Idx1 % 2)) || ((Pool == 1) && (!(Idx1 % 2)))) {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  CBO%02u", (Idx1 - 3) / 2));
        } else {
          Skip = TRUE;
        }
        break;
      }

      if (Skip == TRUE) {
        continue;
      }

      //
      // In  BT modes except mode 1, the remote target contains only one pool. In BT Mode1 both local & remote targets have 2 pools
      //
      if (QpiInternalGlobal->BtMode == BT_MODE_1 || Pool == 0) {
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "   %02u  %02u         %02u  %02u ", QpiInternalGlobal->RtidBase[0][Idx1], QpiInternalGlobal->RtidSize[0][Idx1],
                QpiInternalGlobal->RtidBase[1][Idx1], QpiInternalGlobal->RtidSize[1][Idx1]));
      } else {
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "   %02u  %02u ", QpiInternalGlobal->RtidBase[0][Idx1], QpiInternalGlobal->RtidSize[0][Idx1]));
      }
    }
  }

  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n"));
  return ;
}

VOID
DumpRtidAllocationBdxBtMode2 (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Dump RTID allocation for BDX BtMode2 to output stream
  QpiInternalGlobal - QPIRC Internal Global data

Arguments:

  host  - Pointer to the system host (root) structure

Returns:

  VOID

--*/
{
  UINT8    Idx1, Pool, Cbo, AdjCbo;

  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n; RTID Allocation Table"));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; ---------------------"));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;          Local          Neighbor"));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;          -----          ---------"));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;        Base  Size     Base  Size"));
  
  // Print the WB, UBOX and MESEG based allocations which apply only to First Pool
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;  First Pool (xRTID=0)"));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  --------------------"));
  
  for (Idx1 = 0; Idx1 < 3; ++Idx1) { // Run the loop for WB, UBOX and MESEG
  
    switch (Idx1) {
      case 0:
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  WB   "));
        break;

      case 1:
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  UBOX "));
        break;

      case 2: // Isoc or Meseg
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  MESEG"));
        break;
     } // switch case
     
     QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "   %02u  %02u         %02u  %02u ", 
                          QpiInternalGlobal->RtidBase[0][Idx1], QpiInternalGlobal->RtidSize[0][Idx1],
                          QpiInternalGlobal->RtidBase[1][Idx1], QpiInternalGlobal->RtidSize[1][Idx1]));
  } // for loop
  
  // Print the CBo related dumps now
  //
  for (Pool = 0; Pool < MAX_POOL_SETS_BDX; Pool++) { // First, Second and Special Pool
  
    // Print the banner for second and special pool here. The banner for first pool is 
    // printed above along with with WB, UBOX and MESEG
    if (Pool == 1) { // second pool
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;  Second Pool (xRTID=1)"));
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  ---------------------"));
    } else if (Pool == 2) { // special pool
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;  Special Pool (xRTID=0)"));
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  ----------------------"));
    }
  
    for (Cbo = 0; Cbo < MAX_CBO; Cbo++) {
    
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  CBO%02u", Cbo));  
      
      // When 4S COD is enabled, Cbos share the RTIDs across the clusters. For example, Cbo 0 in left ring
      // and Cbo 1 in right ring would have the same RTID map
      if (QpiInternalGlobal->Sys4SClusterOnDieEn == TRUE) {
        AdjCbo = Cbo / 2;
      } else {
        AdjCbo = Cbo;
      }
      
      // Map the index to array from which base and size need to be pulled out for printing
      Idx1 = (AdjCbo * 2) + 3;
      if (Pool == 1) { // Add 1 to access xRTID=1 pool
        Idx1 += 1;
      }
      
      switch (Pool) {
        case 0: // First Pool(xRTID=0) - local and remote
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "   %02u  %02u         %02u  %02u ", 
                               QpiInternalGlobal->RtidBase[0][Idx1], QpiInternalGlobal->RtidSize[0][Idx1],
                               QpiInternalGlobal->RtidBase[1][Idx1], QpiInternalGlobal->RtidSize[1][Idx1]));
          break;

        case 1: // Second Pool(xRTID=1)
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "   %02u  %02u ", QpiInternalGlobal->RtidBase[0][Idx1], QpiInternalGlobal->RtidSize[0][Idx1]));
          break;

        case 2: // Special Pool(xRTID=0)
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "   %02u  %02u ", QpiInternalGlobal->RtidBase[2][Idx1], QpiInternalGlobal->RtidSize[2][Idx1]));
          break;

        default:
          // Do nothing
          break;
      } // switch pool
    
    } // Cbo loop
  } // for loop - Pool
  
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n"));
  return ;
}

QPI_STATUS
InitializeMCTP (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )

/*++

Routine Description:
  Programs SADs for MCTP traffic

Arguments:
  host    - Input/Output structure of the QPIRC
  SocketData  - CPU related info
  QpiInternalGlobal - QPIRC internal variables.

Returns:
  QPI_SUCCESS

--*/
{
  UINT8          Index, Index1;
  MCTPSAD_N0_IIO_RAS_STRUCT MctpSad0;
  MCTPSAD_N1_IIO_RAS_STRUCT MctpSad1;
  UINT32         Data32;
  UINT8          sbspSktId;
  MMCFG_RULE_N0_CBOBC_IOSAD_HSX_BDX_STRUCT  MmcfgRule;
  UINT32         MmcfgTarget;
  UINT8          Nid = 0, PreviousNid = 0;
  
  sbspSktId = GetSbspSktIdFromLocal(host);
 
  if (host->setup.qpi.IBPECIEn) {
    MmcfgRule.Data = QpiReadPciCfg (host, 0, 0, MMCFG_RULE_N0_CBOBC_IOSAD_REG);
    MctpSad0.Data = 0;
    MctpSad1.Data = 0;
    
    MctpSad0.Bits.num_bus = MmcfgRule.Bits.length;
    MctpSad0.Bits.enable = 1;

    MmcfgTarget = QpiReadPciCfg (host, 0, 0, MMCFG_TARGET_LIST_CBOBC_IOSAD_REG);
    
    //
    // Fill in NIDs for target index fields, by converting MmcfgTarget to MCTP_SAD
    // Fill in broadcast list.   Enable 1 index per socket for broadcast msgs.
    //
    // Index1 == 2 exp MmcfgRule.Bits.length
    Index1 = 1 << MmcfgRule.Bits.length;
    
    for (Index = 0; Index < SI_MAX_CPU_SOCKETS; ++Index) {
    
      Nid = (MmcfgTarget >> ((Index / Index1) * 3) & 0x7);
      if (SocketData->Cpu[Nid].Valid == TRUE) {
        if (Index < 5) {
          MctpSad0.Data = MctpSad0.Data | (Nid << ((6 * Index) + 3));
        } else {
          MctpSad1.Data = MctpSad1.Data | (Nid << ((6 * (Index - 5)) + 1));
        }   
        //
        // Update the brd_lst
        //
        if (Nid != PreviousNid) {
          MctpSad1.Data = MctpSad1.Data | (1 << (Index + 24));
          PreviousNid = Nid;
        }
      }
    }
    
    //
    // Point at RC NID (Root complex NID/MCTP Master)
    //
    MctpSad1.Bits.rc_nid = sbspSktId;
    Data32 = MctpSad1.Data;

    //
    // Program MCTP_SADs in each socket
    //       
    for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
      if (SocketData->Cpu[Index].Valid == TRUE) {
        QpiWritePciCfg (host, Index, 0, MCTPSAD_N0_IIO_RAS_REG, MctpSad0.Data);
        MctpSad1.Data = Data32;
        if (Index == sbspSktId) {
          //
          // Indicate this is the local root complex
          //
          MctpSad1.Bits.rc_local = 1;
        } else {
          //
          // Only valid on SBSP
          //
          MctpSad1.Bits.brd_list = 0;
        }
        
        QpiWritePciCfg (host, Index, 0, MCTPSAD_N1_IIO_RAS_REG, MctpSad1.Data);
      }  
    }
  }
  return QPI_SUCCESS;
}
