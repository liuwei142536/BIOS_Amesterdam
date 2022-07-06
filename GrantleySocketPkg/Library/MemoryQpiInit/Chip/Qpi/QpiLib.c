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
// *      Copyright (c) 2004 - 2015 Intel Corp.                             *
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
#include "SysHost.h"
#include "SysFunc.h"
#include "QpiLib.h"
#include "QpiMisc.h"

#ifdef BUILDING_FOR_X64

//
// Note: WriteCpuPciCfgRas()/ReadCpuPciCfgRas(), and if any, all RAS related code must be in the RAS folder and QPI RC can use extern.
// Otherwise, all RAS #define flag will not be available for QPI RAS build even with BUILDING_FOR_X64.
// Fortunately, for now. QpiLib.c / WriteCpuPciCfgRas() is the only file and it only needs #define QPI_RAS_DEBUG as in CsiRas.h
// 
#define QPI_RAS_DEBUG
#define QPI_RAS_DEBUG_REG_DETAIL
#pragma message( " BUILDING_FOR_X64 Compiling " __FILE__ )  

#include <Protocol/QuiesceProtocol.h>

#define MAX_REG_OFFSET             0x200
#define MAX_BUF_PER_FUN            MAX_REG_OFFSET
#define MAX_BUF_PER_DEV            (8 * MAX_BUF_PER_FUN)
#define MAX_BUF_PER_CPU            (32 * MAX_BUF_PER_DEV)

// Every bit keeps track of a BYTE
#define MAX_FLAG_BUF_PER_CPU       (MAX_BUF_PER_CPU / 8)

#define CPU_SHADOW_BUF_SIZE        (MAX_CPU_SOCKETS * MAX_BUF_PER_CPU)
#define CPU_SHADOW_FLAG_SIZE       (MAX_CPU_SOCKETS * MAX_FLAG_BUF_PER_CPU)

UINT8                       *mCpuShadowFlag = NULL;  // 16KB per CPU, to keep track of every byte of the shadow buffer
UINT8                       *mCpuShadowBuf  = NULL;  // 128KB per CPU, Byte/Word/Dword accesses
QUIESCE_DATA_TABLE_ENTRY    *mNextEntry     = NULL;  // Points to first available slot in the quiesce buffer; used as semaphore as well
BOOLEAN                     mDirectCommit   = TRUE;  // Flag to control whether register writes should be committed immediately
#endif // BUILDING_FOR_X64

#ifdef BUILDING_FOR_IA32
#pragma message( " BUILDING_FOR_IA32 Compiling " __FILE__ )  
#endif

BOOLEAN
Is4SCODSupportedOnTopology (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Based on the topology, return whether the current running topology can support and is eligible
  to run 4S COD feature or not

Arguments:

  host  - Pointer to the system host (root) structure

Returns:

  TRUE if 4S COD could be supported, FALSE otherwise

--*/
{
  UINT8   Idx1, Idx2;
  UINT8   PeerSocList[MAX_CPU_SOCKETS];
  UINT8   PeerSocCnt[MAX_CPU_SOCKETS];
  BOOLEAN Is4SFC, Is4SRing;

#ifdef QPI_4S_COD_FEATURE_PROTOTYPE  
//Grangeville AptioV server override - CPP Check
//  if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType == CPU_HSX_2HA)) &&
  if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType == CPU_HSX_2HA) &&
        (host->var.qpi.SysConfig == SYS_CONFIG_4S) && (QpiInternalGlobal->Is4SRing == TRUE)) {
      return TRUE;
    } else {
      return FALSE;
    }
#endif

  // 4S-COD is a BDX- only feature. This feature is supported *ONLY* on 4S-fully connected (EX-), 4S-ring (EX-)
  // and on 4S-ring (EP-).
  if ((host->var.common.cpuType != CPU_BDX) || (host->var.common.numCpus != 4)) {
    return FALSE;
  } // cpuType, num soc check

  if ((host->var.common.cpuType == CPU_BDX) && (host->var.common.cpuSubType == CPU_BDX_EX)) {
    // Identify the peer socket list and count for each populated socket.
    for (Idx1=0; Idx1 < MAX_CPU_SOCKETS; Idx1++) {
      PeerSocList[Idx1] = 0;
      PeerSocCnt[Idx1] = 0;
      if (SocketData->Cpu[Idx1].Valid == TRUE) {
        for (Idx2=0; Idx2 < MAX_QPI_PORTS; Idx2++) {
          if (SocketData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) {
            if ((PeerSocList[Idx1] & (1 << SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId)) == 0) {
              PeerSocList[Idx1] = PeerSocList[Idx1] |(UINT8)(1 << SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId);
              PeerSocCnt[Idx1]++;
            }
          } // link valid
        } // for link loop
      } // socket valida
    } // for socket

    // Check if we are in ring or fully connected topology on EX-
    //
    Is4SFC = Is4SRing = TRUE;

    for (Idx1=0; Idx1 < 4; Idx1++) {
      // All 4 sockets should have 3 neighbors for fully connected topology
      if (PeerSocCnt[Idx1] != 3) {
          Is4SFC = FALSE;
      }

      // All 4 sockets should have 2 neighbors for ring topology
      if (PeerSocCnt[Idx1] != 2) {
        Is4SRing = FALSE;
      }
    } // socket loop

    if ((Is4SFC == TRUE) || (Is4SRing == TRUE)) {
      return TRUE;
    } else {
      return FALSE;
    }
  } else { // For EP- SKUs, we rely on SysConfig and Is4SRing to take a decision
    if ((host->var.qpi.SysConfig == SYS_CONFIG_4S) && (QpiInternalGlobal->Is4SRing == TRUE)) {
      return TRUE;
    } else {
      return FALSE;
    }
  } // SKU- type check
}
  
UINT8
GetSbspSktIdFromLocal (
  struct sysHost   *host
  )
/*++

Routine Description:

  Get CPU socket id for the system boot strap processor

Arguments:

  host  - Pointer to the system host (root) structure

Returns:

  system boot strap processor socket ID

--*/
{
   return LEGACY_CPU;
}


QPI_STATUS
GetSocketSkuType (
  struct sysHost   *host,
  UINT8            SocId, 
  UINT8            *SocketSkuType,
  UINT8            *SubSkuType
  )
/*++

Routine Description:
   Returns SKU Type.

Arguments:
   host        - add argument and description to function comment
   SocId       - Soc ID of the CPU who's enabled LLC slice info is requested, 0...3
   SocketSkuType     - SKU type in given socket 
   SubSkuType     - Sub SKU type in given socket; for EXA only

Returns:

  0 - Successful
  1 - Failure
 --*/
{
  UINT8   Ctr;
  UINT32  Data32;

  Data32 = QpiReadPciCfg (host, SocId, 0, CAPID0_PCU_FUN3_REG);
  host->var.common.nsSkuId = DefaultSkuID;
  
  if (Data32 & (1 << SKU_DE)) {
    // HSD 4987295: CLONE from broadwell_server: PO fuses creating wrong SKU type for BDX-DE CAPID0
    // Workaround to address additional bits in CAPID0[4:0] being set in addition to CAPID0[5] for DE-SKU
    QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n CAPID0[5] is set. SKU Detected as DE."));
    *SocketSkuType = SKU_DE;
    host->var.common.nsSkuId = DESkuID;
    *SubSkuType = 0;
	if (Data32 & (1 << SKU_NS)) {
      *SocketSkuType = SKU_NS;
      // identify the specified sku of NS
      Data32 = QpiReadPciCfg (host, SocId, 0, CAPID1_PCU_FUN3_REG);
      if (Data32 & (UINT32)NS_STORAGE_SEG_SEL) {
        host->var.common.nsSkuId = NSStorageSkuID;
      } else {
        host->var.common.nsSkuId = NSCommsSkuID;    
      }
	}
  } else if (Data32 & (1 << SKU_EXA)){
    *SocketSkuType = SKU_EXA;
    *SubSkuType = (UINT8)(Data32 >> 2) & 0x3; // [3:2] contains sub sku type for all EXA SKUs
  } else {
    for (Ctr = 0; Ctr < SKU_MAX; ++Ctr) {
      if (Data32 & (1 << Ctr)){
        *SocketSkuType = Ctr;
        *SubSkuType = 0;
        break;
      }
    }
    QPI_ASSERT((Ctr != SKU_MAX), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
  }

  return QPI_SUCCESS;
}


QPI_STATUS
GetCboCountAndList (
  struct sysHost   *host,
  UINT8            SocId,
  UINT8            *CboCount,
  UINT32           *CboMap 
  )
/*++

Routine Description:
   Returns the total Cbos and List of Cbos (& LLCs) enabled for a given CPU socket.

Arguments:
   host        - add argument and description to function comment
   SocId       - Soc ID of the CPU who's enabled LLC slice info is requested, 0...3
   CboCount    - Total Cbos in given socket
   CboMap      -  List of Cbos in given socket
  
Returns:

  0 - Successful
  1 - Failure

--*/
{
  UINT8   Ctr, MaxCbo;
  UINT8   Count = 0;
  UINT32  LlcMap = 0;
  CAPID5_PCU_FUN3_HSX_STRUCT    Capid5_Hsx;
  CAPID5_PCU_FUN3_BDX_STRUCT    Capid5_Bdx;

  //
  // Make sure we are not accessing more than what is supported for the given CPU
  //
  if (host->var.common.cpuType == CPU_HSX) {
    MaxCbo = MAX_HSX_CBO;
    Capid5_Hsx.Data = QpiReadPciCfg (host, SocId, 0, CAPID5_PCU_FUN3_REG);
    LlcMap = Capid5_Hsx.Bits.llc_slice_en;
  } else {
    MaxCbo = MAX_BDX_CBO;
    Capid5_Bdx.Data = QpiReadPciCfg (host, SocId, 0, CAPID5_PCU_FUN3_REG);
    LlcMap = Capid5_Bdx.Bits.llc_slice_en;
  }

  for (Ctr = 0; Ctr < MaxCbo; ++Ctr) {
    if (LlcMap & (1 << Ctr)) {
      Count++;
    }

    if ((host->var.common.cpuType == CPU_BDX) && (host->var.common.stepping == A0_REV_BDX) &&
        (host->var.common.chopType[SocId] == LCC_DE)) { // Alt. for QpiInternalGlobal->SkuType[sbspSktId] == SKU_DE
      // HSD 4987354 : CLONE from broadwell_server: Wrong PO value for fuse LLC_SLICE_DISABLE impacts PCU3 CAPID LLC slice enabled list 
      // Workaround to address llc_slice_en set incorrectly as 0xFFFB6D in CAPID5 for BDX-DE-8C part.
      if (Count >= 8) {
         *CboCount = Count;
         *CboMap   = LlcMap & FLD_MASK(Ctr + 1, 0);
         QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n BDX-DE : count of enabled cbos in llc_slice_en >= 8, CboMap=0x%08X, CboCount=%d", *CboMap, *CboCount));
         return QPI_SUCCESS;
      } // SKU check
    } // cpuType and stepping check
  } // for loop Ctr

  *CboCount = Count;
  *CboMap   = LlcMap;
  return QPI_SUCCESS;
}


QPI_STATUS
GetSboCount (
  struct sysHost   *host,
  UINT8            SocId,
  UINT8            *SboCount
  )
/*++

Routine Description:
   Returns the total Sbos enabled for a given CPU socket.

Arguments:
   host        - add argument and description to function comment
   SocId       - Soc ID of the CPU who's enabled Sbo info is requested, 0...3
   SboCount    - Total Sbos in given socket
  
Returns:

  0 - Successful
  1 - Failure

--*/
{
  UINT8   Ctr, MaxSbo;
  UINT8   Count = 0;
  UINT16  Data16;

  //
  // Make sure we are not accessing more than what is supported for the given CPU
  //
  MaxSbo = MAX_HSX_SBO;

  for (Ctr = 0; Ctr < MaxSbo; ++Ctr) {
    Data16 = (UINT16) QpiReadPciCfg (host, SocId, Ctr, VID_SBO_CFG_REG);
    if (Data16 != 0xFFFF) {
      Count++;
    }
  }

  *SboCount = Count;

  return QPI_SUCCESS;
}

QPI_STATUS
GetHACount (
  struct sysHost   *host,
  UINT8            SocId, 
  UINT8            *HaCount
  )
/*++

Routine Description:
   Returns the total HAs supported by this CPU socket.

Arguments:
   host        - add argument and description to function comment
   SocId       - Soc ID of the CPU who's enabled LLC slice info is requested, 0...3
   HaCount     - Total HomeAgent in given socket 
    
Returns:

  0 - Successful
  1 - Failure

--*/
{
  UINT8   MaxHa;
  UINT8   Count = 0;

  UINT32  Capid6;

  UINT16  Data16;  
  Data16 = 0;  

  //
  // Make sure we are not accessing more than what is supported for the given CPU
  //
  MaxHa = MAX_HSX_HA;

  Capid6 = QpiReadPciCfg (host, SocId, 0, CAPID6_PCU_FUN3_REG);
  Count++;
  if (Capid6 & BIT29) { // de_skt_secondha (checking bit without structure)
    Count++;
  }

  *HaCount = Count; 
  return QPI_SUCCESS;
}

QPI_STATUS
GetR3QPICount (
  struct sysHost   *host,
  UINT8            SocId,
  UINT8            *R3QpiCount
  )
/*++

Routine Description:
   Returns the total R3QPIs supported by this CPU socket.

Arguments:
   host        - add argument and description to function comment
   SocId       - Soc ID of the CPU who's enabled LLC slice info is requested, 0...3   
   R3QpiCount  - Total R3QPI in given socket 
Returns:

  0 - Successful
  1 - Failure

--*/
{
  UINT8   Ctr, MaxR3Qpi;
  UINT8   Count = 0;
  UINT16  Data16;
 
  //
  // Make sure we are not accessing more than what is supported for the given CPU
  //
  MaxR3Qpi = MAX_HSX_R3QPI;

  for (Ctr = 0; Ctr < MaxR3Qpi; ++Ctr) {
    Data16 = (UINT16) QpiReadPciCfg (host, SocId, Ctr, VID_R3QPI_CFG_REG);
    if (Data16 != 0xFFFF) {
      Count++;      
    }
  }

  *R3QpiCount = Count;
  return QPI_SUCCESS;
}

QPI_STATUS
GetQPIAgentCount (
  struct sysHost   *host,
  UINT8            SocId,
  UINT8            *QpiAgentCount
  )
/*++

Routine Description:
   Returns the total QPIAgents supported by this CPU socket.

Arguments:
   host        - add argument and description to function comment
   SocId       - Soc ID of the CPU who's enabled LLC slice info is requested, 0...3   
   QpiAgentCount  - Total QPIAgent in given socket 
Returns:

  0 - Successful
  1 - Failure

--*/
{
  UINT8   Ctr, MaxQpiAgent;
  UINT8   Count = 0;
  UINT16  Data16;
 
  //
  // Make sure we are not accessing more than what is supported for the given CPU
  //
  MaxQpiAgent = MAX_HSX_QPIAGENT;

  for (Ctr = 0; Ctr < MaxQpiAgent; ++Ctr) {
    Data16 = (UINT16) QpiReadPciCfg (host, SocId, Ctr, VID_QPIAGENT_LL_REG);
    if (Data16 != 0xFFFF) {
      Count++;      
    }
  }

  *QpiAgentCount = Count;
  return QPI_SUCCESS;
}


QPI_STATUS
GetLegacyPchTargetSktId (
  struct sysHost   *host,
  UINT8            SocId,
  UINT8            *LegacyPchSktId
  )
/*++

Routine Description:
   Returns the Legacy Pch Socket Id for this CPU socket.

Arguments:
   host        - add argument and description to function comment
   SocId       - Soc ID of the CPU who's enabled LLC slice info is requested, 0...3   
   LegacyPchSktId  - Legacy PCH Socket ID 
Returns:

  0 - Successful
  1 - Failure

--*/
{
  SAD_TARGET_CBOBC_IOSAD_HSX_BDX_STRUCT      SadTarget;

  SadTarget.Data = QpiReadPciCfg (host, SocId, 0, SAD_TARGET_CBOBC_IOSAD_REG);
  *LegacyPchSktId = (UINT8) SadTarget.Bits.legacy_pch_target; 
  
  return QPI_SUCCESS;
}

QPI_STATUS
ParseCpuLep (
  struct sysHost             *host,
  QPI_SOCKET_DATA            *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8                      SocId
  )
/*++

Routine Description:
   Parse the Link Exchange Parameter (LEP) for a given CPU socket. 

Arguments:
   host    - QPI input/output structure
   SocketData - add argument and description to function comment
   QpiInternalGlobal - QPIRC internal variables.
   SocId       - CPU Socket Number, 0 based

Returns:
  0 - found
  1 - not found


--*/
{
  QPI_LEP0                Lep0;
  UINT32                  Lep2;
  QPIPCSTS_QPIAGENT_LL_STRUCT         PcSts;
  QPILS_QPIAGENT_LL_HSX_BDX_STRUCT    QpiLs;

  UINT8                   Port;
  UINT8                   PeerSocId;
  UINT8                   ApSocId;

  UINT16                  Data16;
  UINT32                  Data32;

  UINT32                  Ctr;
  UINT32                  WaitTime;
  BOOLEAN                 CheckInFlag, UseQpiPcSts;

  QPI_ASSERT ((SocId < MAX_CPU_SOCKETS), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  //
  // Parse the LEP of the socket
  //
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n   CPU%u Link Exchange", SocId));

  // 
  // During RAS online operation, use QPILS_QPIAGENT_LL_REG for pre-existing sockets because pcode will not be executed and QPIPCSTS_QPIAGENT_LL_REG is updated by pcode.
  // The online socket will be using QPIPCSTS_QPIAGENT_LL_REG as required as before if its LEP gets needed. 
  //
  //
  UseQpiPcSts = ((host->var.qpi.RasInProgress != TRUE) || ((host->var.qpi.RasInProgress == TRUE) && (host->var.qpi.RasSocId == SocId))) ? (BOOLEAN)TRUE : (BOOLEAN)FALSE;
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO0, "\n   UseQpiPcSts = %u", UseQpiPcSts));

  if ((host->var.qpi.RasInProgress == TRUE) && (host->var.qpi.RasSocId == SocId)) {
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO0, "\n   ParseCpuLep() for online CPU%u", SocId));  
  }

  for (Port = 0; Port < MAX_QPI_PORTS; ++Port) {
    Data16 = (UINT16) QpiReadPciCfg (host, SocId, Port, VID_QPIAGENT_LL_REG);
    if (Data16 == 0xFFFF) {
      continue;
    }

    PcSts.Data = QpiReadPciCfg (host, SocId,Port, QPIPCSTS_QPIAGENT_LL_REG);
    QpiLs.Data = QpiReadPciCfg (host, SocId, Port, QPILS_QPIAGENT_LL_REG); 
#ifdef QPI_RAS_DEBUG
    QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO0, "\n   PcSts = 0x%08X", PcSts.Data));
    QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO0, "\n   QpiLs = 0x%08X", QpiLs.Data));
#endif

    if ((UseQpiPcSts && (PcSts.Bits.ll_status == LINK_SM_NORMAL_OPERATION)) ||
        (!UseQpiPcSts && (QpiLs.Bits.link_initialization_status == LINK_SM_NORMAL_OPERATION))) {
      //
      // Identify the Peer Port number
      //
      Lep0.RawData = QpiReadPciCfg (host, SocId,Port, QPILP0_QPIAGENT_LL_REG);

      QPI_ASSERT ((Lep0.Fields.Port < MAX_QPI_PORTS), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);

      //
      // Identify the Peer Socket Id. The socket ID[1:0] correspond to NID[1:0].
      // NID[1:0] is reported in one of the 4 bytes of LEP2 depending on if the peer agent's socket ID.
      //
      Lep2 = QpiReadPciCfg (host, SocId,Port, QPILP2_QPIAGENT_LL_REG);
      if (Lep2 & 0x000000FF) {
        PeerSocId = 3;
      } else if (Lep2 & 0x0000FF00) {
        PeerSocId = 2;
      } else if (Lep2 & 0x00FF0000) {
        PeerSocId = 1;
      } else if (Lep2 & 0xFF000000) {
        PeerSocId = 0;
      } else {
        QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n\n CPU%u Link %u has invalid LEP2", SocId, Port));
        QPI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
      }
      PeerSocId = (UINT8)(Lep0.Fields.NodeID << 2) | PeerSocId;

      QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, " : LEP%u(%u,CPU%u)", Port, Lep0.Fields.Port, PeerSocId));

      // Invalidate this link if either the current port or its peer port were set to "Disabled" via Setup.
      if ((host->setup.qpi.PhyLinkPerPortSetting[SocId].Link[Port].QpiPortDisable == TRUE) || 
          (host->setup.qpi.PhyLinkPerPortSetting[PeerSocId].Link[Lep0.Fields.Port].QpiPortDisable == TRUE)) {

        // Before disabling the link, if it's attached to the SBSP, make sure the AP (non-SBSP) socket
        // has completed its reset and has "checked-in" with the SBSP.  Otherwise,
        // disabling the link may cause MCA when socket becomes isolated.
        if ((SocId == QpiInternalGlobal->SbspSoc) || (PeerSocId == QpiInternalGlobal->SbspSoc)) {
          ApSocId = ((SocId == QpiInternalGlobal->SbspSoc) ? PeerSocId : SocId);  //Non-SBSP Socket
          CheckInFlag = FALSE;
          OemWaitTimeForPSBP(host, &WaitTime);
          for (Ctr = 0; Ctr < WaitTime; ++Ctr) { 
            Data32 = QpiReadPciCfg (host, (UINT8)ApSocId, 0, BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG);
            if ((Data32 & 0x1) != 0) {            //check bit 0 to indicate AP has checked-in 
              CheckInFlag = TRUE;
              QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "-Disabled"));
              break;
            }
            QpiFixedDelay(host, 1000); // 1000 microsecs 
          }
          if (CheckInFlag == FALSE) {
            QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n\n AP Socket %u is discovered but hasn't come out of reset. System Halted!!!", ApSocId));
            QPI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
          }
        }//if SBSP link

        // SBSP neighbor (AP) socket has checked-in.  The link is no longer needed so mark it 'disabled' and move on to the next link.
        SocketData->Cpu[SocId].LinkData[Port].Valid = FALSE;
        continue;
      }//if link set to "disabled"

      //
      // Mark the Link as valid for the socket
      //
      SocketData->Cpu[SocId].LinkData[Port].Valid       = TRUE;
      SocketData->Cpu[SocId].LinkData[Port].PeerPort    = (UINT8) Lep0.Fields.Port;
      SocketData->Cpu[SocId].LinkData[Port].PeerSocId   = PeerSocId;
      SocketData->Cpu[SocId].LinkData[Port].PeerSocType = SOCKET_TYPE_CPU;
    }
  }

  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n"));
  return QPI_SUCCESS;
}


QPI_STATUS
UpdateHostMmiohBaseAndLimit(                            
  struct sysHost      *host,
  UINT8               socket,
  BOOLEAN             OemRequestSetMmioh,
  UINT64_STRUCT       *MmiohBase,                
  UINT16              MmiohSize,                 
  UINT16              OemMmiohBaseHi
  )
  /*++

Routine Description:
  Update host mmioh base and limit for this socket
  If OemRequestSetMmioh = FALSE, will use MmiohBase & MmiohSize
  If OemRequestSetMmioh = TRUE,  will use MmiohBaseHi & MmiohSize (which oem custerm overwrite)
  
Arguments:
  host    - Input/Output structure of the QPIRC
  Socket  - socket number
  MmiohBase - Input/Ouput 
  MmiohSize - Input
  MmiohBaseHi - Input

Returns:
  QPI_SUCCESS

--*/

{
  UINT64_STRUCT  MmiohLimit, TmpMmiohBase, TmpMmioh;

  TmpMmioh.lo = 0x40000000; // 1GB
  TmpMmioh.hi = 0;
  TmpMmioh = MultUINT64x32(TmpMmioh, MmiohSize);
  TmpMmioh = SubUINT64x32(TmpMmioh, 1);

  if(OemRequestSetMmioh == FALSE){
    TmpMmiohBase = *MmiohBase;
  } else {
    TmpMmiohBase.lo = 0;
    TmpMmiohBase.hi = OemMmiohBaseHi;
  }

  MmiohLimit = AddUINT64 (TmpMmiohBase, TmpMmioh);

  host->var.qpi.CpuInfo[socket].CpuRes.MmiohBase = TmpMmiohBase;
  host->var.qpi.CpuInfo[socket].CpuRes.MmiohLimit = MmiohLimit;

  if(OemRequestSetMmioh == FALSE){  //if not Oem request, update TmpMmiohBase as temp mmioh base for next socket
    *MmiohBase = AddUINT64x32 (MmiohLimit, 1);
  } 

  return QPI_SUCCESS;
}

QPI_STATUS
UpdateHostMmiolBaseAndLimit(                            
  struct sysHost      *host,
  UINT8               socket,
  BOOLEAN             OemRequestSetMmiol,
  UINT32              *MmiolBase,                
  UINT16              MmiolSize,                 
  UINT32              OemMmiolBase
  )
  /*++

Routine Description:
  Update host mmioh base and limit for this socket
  If OemRequestSetMmioh = FALSE, will use MmiohBase & MmiohSize
  If OemRequestSetMmioh = TRUE,  will use MmiohBaseHi & MmiohSize (which oem custerm overwrite)
  
Arguments:
  host    - Input/Output structure of the QPIRC
  Socket  - socket number
  MmiolBase - Input/Ouput 
  MmiolSize - Input
  OemMmiolBase - Input If OemRequestSetMmiol = TRUE, then this field is valid 

Returns:
  QPI_SUCCESS

--*/

{
  UINT32  MmiolLimit, TmpMmiolBase;

  if (MmiolSize != 0) {
    if(OemRequestSetMmiol == FALSE){  //if not Oem request, update TmpMmiolBase as temp mmiol base 
      TmpMmiolBase = *MmiolBase;
    } else {
      TmpMmiolBase = OemMmiolBase;
    }

    MmiolLimit = TmpMmiolBase + (MmiolSize * SAD_MMIOL_GRANTY);
    host->var.qpi.CpuInfo[socket].CpuRes.MmiolBase  = TmpMmiolBase;
    host->var.qpi.CpuInfo[socket].CpuRes.MmiolLimit = MmiolLimit - 1;
    if(OemRequestSetMmiol == FALSE){  //if not Oem request, update TmpMmiolBase as temp mmiol base for next socket
      *MmiolBase = MmiolLimit;
    }
    if (host->var.qpi.CpuInfo[socket].CpuRes.MmiolLimit > MMIOL_LIMIT) {
      // Reserve 4G-64M to 4G for chipset CSR accesses
      host->var.qpi.CpuInfo[socket].CpuRes.MmiolLimit = MMIOL_LIMIT;
    }
  } else {
    host->var.qpi.CpuInfo[socket].CpuRes.MmiolBase = 0xFFFFFFFF;
    host->var.qpi.CpuInfo[socket].CpuRes.MmiolLimit = 0;
  }

  return QPI_SUCCESS;
}


QPI_STATUS
AllocateIioResources (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )

/*++

Routine Description:
  Ration the Bus/IO/MMIO/IOAPIC resources using the ratio specified for the CPU
  sockets populated. If equl distribution is not possible, assign the remainings resources
  to the CPUs with lowest NID. Legacy CPU is always allocated space at the base of the 
  resource region. Other CPUs are assigned resources in the increasing order of Socket ID.
  Example: CPU2's resource base address will be greater than CPU1's resource base address.

Arguments:
  host    - Input/Output structure of the QPIRC
  SocketData  - CPU related info
  QpiInternalGlobal - QPIRC internal variables.

Returns:
  QPI_SUCCESS

--*/
{
  UINT8          Idx1;
  UINT8          TmpTotCpu;
  UINT8          BusSize[MAX_CPU_SOCKETS];
  UINT8          IoSize[MAX_CPU_SOCKETS]; 
  UINT8          IoApicSize[MAX_CPU_SOCKETS];
  
  //MMIOL
  UINT8          MmiolSize[MAX_CPU_SOCKETS];            //Need to initialize or Input from Oem      
  UINT32         MmiolBase[MAX_CPU_SOCKETS] = { 0 };    //Need to initialize or Input from Oem  
  UINT32         TmpMmiolBase;
  BOOLEAN        OemRequestSetMmiol;

  //MMIOH
  UINT16         MmiohSize[MAX_CPU_SOCKETS];            //Need to initialize or Input from Oem      
  UINT16         MmiohBaseHi[MAX_CPU_SOCKETS] = { 0 };  //Need to initialize or Input from Oem    
  UINT64_STRUCT  TmpMmiohBase;
  UINT32         TotMmiohSize = 0;
  BOOLEAN        OemRequestSetMmioh;

  UINT32         Data32;
  UINT32         BusBase, IoBase, IoApicBase;
  OemRequestSetMmiol = OemRequestSetMmioh = FALSE; 
 
  QpiCheckPoint(0xFF, 0xFF, 0xFF, STS_PROGRAM_FINAL_IO_SAD, MINOR_STS_ALLOCATE_CPU_RESOURCE, host);

  if( QpiInternalGlobal->QpiCpuSktHotPlugEn == TRUE){
    if (host->setup.qpi.QpiCpuSktHotPlugTopology == 0) {
      TmpTotCpu =  4;         //online/offline based on 4S topology
    } else {
      TmpTotCpu =  8;         //online/offline based on 8S topology
    }
  } else {
    TmpTotCpu =  QpiInternalGlobal->TotCpu;
  }

  // Calculate the resources for each CPU based on input given to QPIRC, it is based on OemRequestSetting == FALSE 
  CalculateResourceRatio(host, SocketData, QpiInternalGlobal, BusSize, IoSize, MmiolSize, IoApicSize);

  //
  // Check if get MmiohSize[MAX_CPU_SOCKETS] & MmiohBaseHi[MAX_CPU_SOCKETS] from Oem settings
  //
  OemRequestSetMmioh = OemQpiGetMmioh(host, MmiohSize, MmiohBaseHi);
  //
  // Check if get MmiolSize[MAX_CPU_SOCKETS] & MmiolBase[MAX_CPU_SOCKETS] from Oem settings
  //
  OemRequestSetMmiol = OemQpiGetMmiol(host, MmiolSize, MmiolBase);

  if(OemRequestSetMmioh == FALSE){
    for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
      MmiohSize[Idx1] = host->setup.common.mmiohSize;     // update mmioh size from setup setting
    }
    TmpMmiohBase.lo = 0;
    TmpMmiohBase.hi = host->setup.common.mmiohBase;
  }

  //
  // If the MMIOH size requested is 0, force it to 256G
  //
  if (MmiohSize[QpiInternalGlobal->SbspSoc] == 0) {
    for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
      MmiohSize[Idx1] = 256;
    }
    QpiLogWarning(host, 0xFF, 0xFF, 0xFF, WARN_MMIOH_SIZE_UNSUPPORTED);
    QpiDebugPrintWarn((host, QPI_DEBUG_WARN, "\n WARNING:  MMIOH size requested is 0GB per CPU. Forcing it to 256GB. "));
  }
  
  //
  // MMIOH - Assert when the total MMIOH requirement exceeds 4 TB
  // HSD 4987719 : Intel RC restricting High MMIO space to < 1TB per socket
  //
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    // Find the total MMIOH requirements for all valid sockets
    if (SocketData->Cpu[Idx1].Valid == TRUE || QpiInternalGlobal->QpiCpuSktHotPlugEn == TRUE) {
      TotMmiohSize += MmiohSize[Idx1];
    }
  }
  
  QPI_ASSERT((TotMmiohSize <= 8192), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);    // Total MMIOH exceeded 8192 GB assuming MMIOH_BASE as 0x3800
  
  //
  // MMIOL = MMCFG Base + MMCFG Size. Make sure at least 64M of MMIOL space is available for each CPU.
  //
  if(OemRequestSetMmiol == FALSE){
    Data32 = MMIOL_LIMIT - (host->setup.common.mmCfgBase + host->setup.common.mmCfgSize - 1) ;
    if((Data32 >> 26)  < TmpTotCpu) {
      QpiCheckPoint(0xFF, 0xFF, 0xFF, ERR_SAD_SETUP, MINOR_ERR_INSUFFICIENT_RESOURCE_SPACE, host);
      QpiDebugPrintFatal((host, QPI_DEBUG_ERROR, "\n Invalid MMCFG Base Supplied. Insufficient MMIOL space. "));
      QpiDebugPrintFatal((host, QPI_DEBUG_ERROR, "\n Each CPU must be allocated at least 64MB of MMIOL space"));
      QPI_ASSERT(FALSE, ERR_SAD_SETUP, MINOR_ERR_INSUFFICIENT_RESOURCE_SPACE);
    }
    TmpMmiolBase = host->setup.common.mmCfgBase + host->setup.common.mmCfgSize;
  }

  //
  // Make sure at least 32M of MMCFG space is available for each CPU.
  //
  if (host->setup.common.mmCfgSize < (UINT32)(TmpTotCpu * 32 * 1024 * 1024)) {
    QpiCheckPoint(0xFF, 0xFF, 0xFF, ERR_SAD_SETUP, MINOR_ERR_INSUFFICIENT_RESOURCE_SPACE, host);
    QpiDebugPrintFatal((host, QPI_DEBUG_ERROR, "\n Invalid MMCFG Size Supplied. "));
    QpiDebugPrintFatal((host, QPI_DEBUG_ERROR, "\n Each CPU must be allocated at least 32MB of MMCFG space"));
    QPI_ASSERT(FALSE, ERR_SAD_SETUP, MINOR_ERR_INSUFFICIENT_RESOURCE_SPACE);
  }

  //
  // Initialize the Base value of the resources
  //
  BusBase = 0;
  IoBase = 0;
  IoApicBase = IOAPIC_BASE;

  //
  // Assign the resources for CPUs. Legacy socket's resource range starts at the base of the resource.
  //
  host->var.qpi.CpuInfo[QpiInternalGlobal->SbspSoc].CpuRes.BusBase  = (UINT8)BusBase;
  host->var.qpi.CpuInfo[QpiInternalGlobal->SbspSoc].CpuRes.BusLimit = (UINT8)(BusBase + (BusSize[QpiInternalGlobal->SbspSoc] * SAD_BUS_GRANTY) - 1);
  BusBase = (UINT32)host->var.qpi.CpuInfo[QpiInternalGlobal->SbspSoc].CpuRes.BusLimit + 1;

  host->var.qpi.CpuInfo[QpiInternalGlobal->SbspSoc].CpuRes.IoBase  = (UINT16)IoBase;
  host->var.qpi.CpuInfo[QpiInternalGlobal->SbspSoc].CpuRes.IoLimit = (UINT16)(IoBase + (IoSize[QpiInternalGlobal->SbspSoc] * SAD_LEG_IO_GRANTY) - 1);
  IoBase = (UINT32)host->var.qpi.CpuInfo[QpiInternalGlobal->SbspSoc].CpuRes.IoLimit + 1;

  host->var.qpi.CpuInfo[QpiInternalGlobal->SbspSoc].CpuRes.IoApicBase  = IoApicBase;
  host->var.qpi.CpuInfo[QpiInternalGlobal->SbspSoc].CpuRes.IoApicLimit = IoApicBase + (IoApicSize[QpiInternalGlobal->SbspSoc] * SAD_IOAPIC_GRANTY) - 1;
  IoApicBase = host->var.qpi.CpuInfo[QpiInternalGlobal->SbspSoc].CpuRes.IoApicLimit + 1;

  UpdateHostMmiolBaseAndLimit(host, QpiInternalGlobal->SbspSoc, OemRequestSetMmiol, &TmpMmiolBase, MmiolSize[QpiInternalGlobal->SbspSoc], MmiolBase[QpiInternalGlobal->SbspSoc]);
  UpdateHostMmiohBaseAndLimit(host, QpiInternalGlobal->SbspSoc, OemRequestSetMmioh, &TmpMmiohBase, MmiohSize[QpiInternalGlobal->SbspSoc], MmiohBaseHi[QpiInternalGlobal->SbspSoc]);


  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (Idx1 == QpiInternalGlobal->SbspSoc) {
      continue;
    }
    //
    // if socket exists or Hot Plug enabled (reserved for onlining sockets)
    //
    if ( SocketData->Cpu[Idx1].Valid == TRUE || QpiInternalGlobal->QpiCpuSktHotPlugEn == TRUE ){
      // Update Bus Base & Limit
      host->var.qpi.CpuInfo[Idx1].CpuRes.BusBase  = (UINT8)BusBase;
      host->var.qpi.CpuInfo[Idx1].CpuRes.BusLimit = (UINT8)(BusBase + (BusSize[Idx1] * SAD_BUS_GRANTY) - 1);
      BusBase = (UINT32)host->var.qpi.CpuInfo[Idx1].CpuRes.BusLimit + 1;
      // Update Io Base & Limit
      if (IoSize[Idx1] != 0) {
        host->var.qpi.CpuInfo[Idx1].CpuRes.IoBase  = (UINT16)IoBase;
        host->var.qpi.CpuInfo[Idx1].CpuRes.IoLimit = (UINT16)(IoBase + (IoSize[Idx1] * SAD_LEG_IO_GRANTY) - 1);
        IoBase = (UINT32)host->var.qpi.CpuInfo[Idx1].CpuRes.IoLimit + 1;
      } else {
        host->var.qpi.CpuInfo[Idx1].CpuRes.IoBase = 0xFFFF;
        host->var.qpi.CpuInfo[Idx1].CpuRes.IoLimit = 0;
      }
      // Update IoApic Base & Limit
      host->var.qpi.CpuInfo[Idx1].CpuRes.IoApicBase  = IoApicBase;
      host->var.qpi.CpuInfo[Idx1].CpuRes.IoApicLimit = IoApicBase + (IoApicSize[Idx1] * SAD_IOAPIC_GRANTY) - 1;
      IoApicBase = host->var.qpi.CpuInfo[Idx1].CpuRes.IoApicLimit + 1;
      // Update MmioL Base & Limit
      UpdateHostMmiolBaseAndLimit(host, Idx1, OemRequestSetMmiol, &TmpMmiolBase, MmiolSize[Idx1], MmiolBase[Idx1]);
      // Update MmioH Base & Limit
      UpdateHostMmiohBaseAndLimit(host, Idx1, OemRequestSetMmioh, &TmpMmiohBase, MmiohSize[Idx1], MmiohBaseHi[Idx1]);
    } else { // Indicate the resources are not valid by setting Base > Limit
      host->var.qpi.CpuInfo[Idx1].CpuRes.BusBase = 0xFF;
      host->var.qpi.CpuInfo[Idx1].CpuRes.BusLimit = 0;
      host->var.qpi.CpuInfo[Idx1].CpuRes.IoBase = 0xFFFF;
      host->var.qpi.CpuInfo[Idx1].CpuRes.IoLimit = 0;
      host->var.qpi.CpuInfo[Idx1].CpuRes.IoApicBase = 0xFFFFFFFF;
      host->var.qpi.CpuInfo[Idx1].CpuRes.IoApicLimit = 0;
      host->var.qpi.CpuInfo[Idx1].CpuRes.MmiolBase = 0xFFFFFFFF;
      host->var.qpi.CpuInfo[Idx1].CpuRes.MmiolLimit = 0;
      host->var.qpi.CpuInfo[Idx1].CpuRes.MmiohBase.lo = 0xFFFFFFFF;
      host->var.qpi.CpuInfo[Idx1].CpuRes.MmiohBase.hi = 0xFFFFFFFF;
      host->var.qpi.CpuInfo[Idx1].CpuRes.MmiohLimit.lo = 0;
      host->var.qpi.CpuInfo[Idx1].CpuRes.MmiohLimit.hi = 0;

      // IO APIC region is always assigned for all CPUs including the ones that are not populated. 
      IoApicBase += (IoApicSize[Idx1] * SAD_IOAPIC_GRANTY);
    }
  }

  QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n\n;CPU Resource Allocation"));
  QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;-----------------------"));
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if ( SocketData->Cpu[Idx1].Valid == TRUE || QpiInternalGlobal->QpiCpuSktHotPlugEn == TRUE ){

      // Make sure the Base & Limits are naturally aligned
      QPI_ASSERT(((host->var.qpi.CpuInfo[Idx1].CpuRes.BusBase % SAD_BUS_GRANTY) == 0), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
      QPI_ASSERT((((host->var.qpi.CpuInfo[Idx1].CpuRes.BusLimit + 1) % SAD_BUS_GRANTY) == 0), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
      if (host->var.qpi.CpuInfo[Idx1].CpuRes.IoBase < host->var.qpi.CpuInfo[Idx1].CpuRes.IoLimit) {
        QPI_ASSERT(((host->var.qpi.CpuInfo[Idx1].CpuRes.IoBase % SAD_LEG_IO_GRANTY) == 0), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
        QPI_ASSERT((((host->var.qpi.CpuInfo[Idx1].CpuRes.IoLimit + 1) % SAD_LEG_IO_GRANTY) == 0), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
      }
      QPI_ASSERT(((host->var.qpi.CpuInfo[Idx1].CpuRes.IoApicBase % SAD_IOAPIC_GRANTY) == 0), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
      QPI_ASSERT((((host->var.qpi.CpuInfo[Idx1].CpuRes.IoApicLimit + 1) % SAD_IOAPIC_GRANTY) == 0), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
      if (host->var.qpi.CpuInfo[Idx1].CpuRes.MmiolBase < host->var.qpi.CpuInfo[Idx1].CpuRes.MmiolLimit) {
        QPI_ASSERT(((host->var.qpi.CpuInfo[Idx1].CpuRes.MmiolBase % SAD_MMIOL_GRANTY) == 0), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
        QPI_ASSERT((((host->var.qpi.CpuInfo[Idx1].CpuRes.MmiolLimit + 1) % SAD_MMIOL_GRANTY) == 0), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
      }
      QPI_ASSERT(((host->var.qpi.CpuInfo[Idx1].CpuRes.MmiohBase.lo % 0x40000000) == 0), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL); // 1GB
      QPI_ASSERT((((host->var.qpi.CpuInfo[Idx1].CpuRes.MmiohLimit.lo + 1) % 0x40000000) == 0), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

      QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;CPU%u   Bus: 0x%02X - 0x%02X  IO: 0x%04X - 0x%04X  IOAPIC: 0x%08X - 0x%08X  ",
           Idx1,
           host->var.qpi.CpuInfo[Idx1].CpuRes.BusBase,
           host->var.qpi.CpuInfo[Idx1].CpuRes.BusLimit,
           host->var.qpi.CpuInfo[Idx1].CpuRes.IoBase,
           host->var.qpi.CpuInfo[Idx1].CpuRes.IoLimit,
           host->var.qpi.CpuInfo[Idx1].CpuRes.IoApicBase,
           host->var.qpi.CpuInfo[Idx1].CpuRes.IoApicLimit
      ));
      QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "MMIOL: 0x%08X - 0x%08X  MMIOH: 0x%08X %08X - 0x%08X %08X",
           host->var.qpi.CpuInfo[Idx1].CpuRes.MmiolBase,
           host->var.qpi.CpuInfo[Idx1].CpuRes.MmiolLimit,
           host->var.qpi.CpuInfo[Idx1].CpuRes.MmiohBase.hi,
           host->var.qpi.CpuInfo[Idx1].CpuRes.MmiohBase.lo,
           host->var.qpi.CpuInfo[Idx1].CpuRes.MmiohLimit.hi,
           host->var.qpi.CpuInfo[Idx1].CpuRes.MmiohLimit.lo
           ));
    }
  }

  QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n"));

  return QPI_SUCCESS;
}

QPI_STATUS
CalculateResourceRatio (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8                  *BusSize,
  UINT8                  *IoSize,
  UINT8                  *MmiolSize,
  UINT8                  *IoApicSize
  )

/*++

Routine Description:
  This routine assigns resources for CPUs that are populated. It consults the appropriate inputs given to QPIRC and
  applies the following set of rules:
    1. Resource will be allocated only for the CPUs that are currently populated.
       (Except IOAPIC range which is always reserved for all 4 sockets; this makes ACPI & other IOAPIC related setting easier)
    2. If a CPU is populated, bus resource must be requested by the platform. Otherwise force the resource ratio to 1 for that CPU.
    3. Sum of ratios of Bus/IO/IOAPIC resources must be less than or equal to 8
    4. Sum of ratios of MMIOL resource must be less than or equal to (4G - 64M - MMIOLBase)  / 64M
    5. If the ratio is leading to leftover resource, it will be allocated for CPUs starting with lowest NID. 

Note:
  1. MMIOL/Bus space is varible in size. Also, 4G-64M to 4G region is reserved for chipset register access
     and must be mapped to the nearest MMIOL target where it is master aborted.
  2. Each CPU is allocated 256KB of IOAPIC region; there is no choice available for the platform to choose the size per CPU.
      For >4S configs, each CPU is allocated 128KB of IOAPIC region.

Arguments:
  host    - Input/Output structure of the QPIRC
  SocketData  - CPU related info
  QpiInternalGlobal - QPIRC internal variables.
  BusSize     - Calculated Bus size for CPUs 0..3 (Granularity is 32 buses)
  IoSize      - Calculated IO space size for CPUs 0..3 (Granularity is 8KB)
  MmiolSize   - Calculated MMIOL space size for CPUs 0..3 (Granularity is 64MB)
  IoApicSize  - Calculated IOAPIC space size for CPUs 0..3 (Granularity is 128KB)

Returns:
  QPI_SUCCESS

--*/
{
  BOOLEAN        WarnFlag;
  UINT8          Idx1, AvailMmiolChunk, AvailBusChunk;
  UINT8          TmpTotCpu;
  UINT8          TotBusChunk, TotIoChunk, TotMmiolChunk;
  UINT8          ResiBusChunk, ResiIoChunk, ResiMmiolChunk;
  UINT8          TmpBusRatio[MAX_CPU_SOCKETS];
  UINT8          TmpIoRatio[MAX_CPU_SOCKETS];
  UINT8          TmpMmiolRatio[MAX_CPU_SOCKETS];
  UINT32         MmiolBase;

  TmpTotCpu = QpiInternalGlobal->TotCpu;
  AvailBusChunk = (UINT8)(host->setup.common.mmCfgSize >> 25); // Convert the available MMCFG space into 32M chunck size
  MmiolBase = host->setup.common.mmCfgBase + host->setup.common.mmCfgSize; // MMIOL Base = MMCFG Base + MMCFG Size
  AvailMmiolChunk  = (UINT8)((UINT32)((MMIOL_LIMIT - MmiolBase) + 1) >> 26); // Convert the available MMIOL space into 64M chunck size

  // Sanity check the resource allocation requested by the platform
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if ( SocketData->Cpu[Idx1].Valid == TRUE || QpiInternalGlobal->QpiCpuSktHotPlugEn == TRUE ){
      WarnFlag = FALSE;

      //
      // Make sure Bus resource is requested. Otherwise log a warning and force the ratio for this CPU to 1.
      //
      if (host->setup.qpi.BusRatio[Idx1] == 0) {
        TmpBusRatio[Idx1] = 1;
        WarnFlag = TRUE;
      } else {
        TmpBusRatio[Idx1] = host->setup.qpi.BusRatio[Idx1];
      }

      // IO/MMIOL resource ratio can be zero for a populated socket except for legacy socket
      if (Idx1 == QpiInternalGlobal->SbspSoc) {
        if (host->setup.qpi.IoRatio[Idx1] == 0) {
          TmpIoRatio[Idx1] = 1;
          WarnFlag = TRUE;
        } else {
          TmpIoRatio[Idx1] = host->setup.qpi.IoRatio[Idx1];
        }
        if (host->setup.qpi.MmiolRatio[Idx1] == 0) {
          TmpMmiolRatio[Idx1] = 1;
          WarnFlag = TRUE;
        }  else {
          TmpMmiolRatio[Idx1] = host->setup.qpi.MmiolRatio[Idx1];
        }
      } else {
        TmpIoRatio[Idx1] = host->setup.qpi.IoRatio[Idx1];
        TmpMmiolRatio[Idx1] = host->setup.qpi.MmiolRatio[Idx1];
      }

      if (WarnFlag == TRUE) {
        QpiLogWarning(host, (UINT8)(1 << Idx1), SOCKET_TYPE_CPU, 0xFF, WARN_RESOURCE_NOT_REQUESTED_FOR_CPU);
        QpiDebugPrintWarn((host, QPI_DEBUG_WARN, "\n WARNING:  A resource is not requested for CPU%u. Forcing resource allocation. ", Idx1));
      }
    } else {
      //
      // The CPU is not present, so don't allocate resources.
      //
      TmpBusRatio[Idx1] = 0;
      TmpIoRatio[Idx1] = 0;
      TmpMmiolRatio[Idx1] = 0;
    }
  }

  //
  // Calculate total resource ratio requested by the platform
  //
  TotBusChunk = TotIoChunk = TotMmiolChunk = 0;
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    TotBusChunk    = TotBusChunk + TmpBusRatio[Idx1];
    TotIoChunk     = TotIoChunk + TmpIoRatio[Idx1];
    TotMmiolChunk  = TotMmiolChunk + TmpMmiolRatio[Idx1]; 
  }

  // Log a warning if the platform requested resource is more than what is available
  if ((TotBusChunk > AvailBusChunk) || (TotIoChunk > MAX_SAD_TARGETS) || (TotMmiolChunk > AvailMmiolChunk)) {
    QpiLogWarning(host, 0xFF, 0xFF, 0xFF, WARN_RESOURCE_REQUEST_NOT_MET);
    QpiDebugPrintWarn((host, QPI_DEBUG_WARN, "\n WARNING:  Requested Resource can not be allocated. Forcing to maximum possible value. "));

    //
    // Force the resource to maximum possible value and force the resource ratio to 1 for each socket
    // for which the resource is requested
    //
    if (TotBusChunk > AvailBusChunk) {
      for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
        if (TmpBusRatio[Idx1] != 0) {
          TmpBusRatio[Idx1] = 1;
        }
      }
    }

    if (TotIoChunk > MAX_SAD_TARGETS) {
      for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
        if (TmpIoRatio[Idx1] != 0) {
          TmpIoRatio[Idx1] = 1;
        }
      }
    }

    if (TotMmiolChunk > AvailMmiolChunk) {
      for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
        if (TmpMmiolRatio[Idx1] != 0) {
          TmpMmiolRatio[Idx1] = 1;
        }
      }
    }

    //
    // Recalculate total resource ratio requested by the platform
    //
    TotBusChunk = TotIoChunk = TotMmiolChunk = 0;
    for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
      TotBusChunk    = TotBusChunk + TmpBusRatio[Idx1];
      TotIoChunk     = TotIoChunk + TmpIoRatio[Idx1];
      TotMmiolChunk  = TotMmiolChunk + TmpMmiolRatio[Idx1]; 
    }
  }

  //
  // Calculate the residual resources
  //
  ResiBusChunk    = AvailBusChunk % TotBusChunk;
  ResiIoChunk     = MAX_SAD_TARGETS % TotIoChunk;
  ResiMmiolChunk  = AvailMmiolChunk % TotMmiolChunk; 

  //
  // Distribute the "whole" resources
  //
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    BusSize[Idx1]    = TmpBusRatio[Idx1] * (AvailBusChunk / TotBusChunk);
    IoSize[Idx1]     = TmpIoRatio[Idx1] * (MAX_SAD_TARGETS / TotIoChunk);
    MmiolSize[Idx1]  = TmpMmiolRatio[Idx1] * (AvailMmiolChunk / TotMmiolChunk); 
  }

  //
  // Distribute the "residual" resources
  //
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (ResiBusChunk && (TmpBusRatio[Idx1] != 0) ) {
      ++BusSize[Idx1];
      --ResiBusChunk;
    }
    if (ResiIoChunk && (TmpIoRatio[Idx1] != 0)) {
      ++IoSize[Idx1];
      --ResiIoChunk;
    }
    if (ResiMmiolChunk && (TmpMmiolRatio[Idx1] != 0)) {
      ++MmiolSize[Idx1];
      --ResiMmiolChunk;
    }

#if MAX_CPU_SOCKETS > 4
  IoApicSize[Idx1] = 1;          //if > 4, // Each socket is assigned 128K of IO APIC space
#else
  IoApicSize[Idx1] = 2;          //if <= 4, // Each socket is assigned 256K of IO APIC space
#endif
  }

  //
  // Anything left, apply to Legacy socket
  //   
  BusSize[QpiInternalGlobal->SbspSoc] = BusSize[QpiInternalGlobal->SbspSoc] + ResiBusChunk;
  IoSize[QpiInternalGlobal->SbspSoc] = IoSize[QpiInternalGlobal->SbspSoc] + ResiIoChunk;
  MmiolSize[QpiInternalGlobal->SbspSoc] = MmiolSize[QpiInternalGlobal->SbspSoc] + ResiMmiolChunk;

  //
  // Verify the resource allocation
  //
  TotBusChunk = TotIoChunk = TotMmiolChunk = 0;
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    TotBusChunk    = TotBusChunk + BusSize[Idx1];
    TotIoChunk     = TotIoChunk + IoSize[Idx1];
    TotMmiolChunk  = TotMmiolChunk + MmiolSize[Idx1]; 
  }
  QPI_ASSERT ((TotBusChunk == AvailBusChunk), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((TotIoChunk == MAX_SAD_TARGETS), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((TotMmiolChunk == AvailMmiolChunk), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  return QPI_SUCCESS;
}

QPI_STATUS
FillSadTargetList (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT32                 *IoTgtLst,
  UINT32                 *ApicTgtLst,
  UINT32                 *MmioTgtLst
  )
/*++

Routine Description:
  Routine to fill SAD target list based on the resources assigned to different sockets in
  the system.

Arguments:
  host    - Input/Output structure of the QPIRC
  SocketData  - CPU route info to be programmed
  QpiInternalGlobal - QPIRC internal variables.
  IoTgtLst   - Legacy IO region target list
  ApicTgtLst   - IO APIC region target list
  MmioTgtLst   - MMIOL/H region target list

Returns:

  0 - Successful
  Other - failure

--*/
{
  UINT8          Idx1, Idx2, TgtLstIdx, TotEntry;
  UINT32         IoApicBase, IoApicLimit, Mask;
  UINT32         LegIoBase, LegIoLimit, TmpTgtLst;

  QpiCheckPoint(0xFF, 0xFF, 0xFF, STS_PROGRAM_FINAL_IO_SAD, MINOR_STS_FILL_SAD_TGTLST, host);

  // Make the Legacy CPU as the default target
  Mask = FLD_MASK(SADTGT_SIZE, 0);
  TmpTgtLst = 0;
  for (Idx1 = 0; Idx1 < MAX_SAD_TARGETS; ++Idx1) {
    TmpTgtLst = TmpTgtLst | (QpiInternalGlobal->SbspSoc << (Idx1 * SADTGT_SIZE));
  }
  *IoTgtLst = *ApicTgtLst = *MmioTgtLst = TmpTgtLst;

  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if ((SocketData->Cpu[Idx1].Valid == TRUE) && (host->var.qpi.OutIioUniphyDisable[Idx1] == 0)) {
      IoApicBase = host->var.qpi.CpuInfo[Idx1].CpuRes.IoApicBase;
      IoApicLimit = host->var.qpi.CpuInfo[Idx1].CpuRes.IoApicLimit;
      LegIoBase = host->var.qpi.CpuInfo[Idx1].CpuRes.IoBase;
      LegIoLimit = host->var.qpi.CpuInfo[Idx1].CpuRes.IoLimit;

      //
      // Adjust the limit for easy target calculation
      //
      ++IoApicLimit;
      ++LegIoLimit;

      //
      // Calculate the Target List Index & Total Target List for IOAPIC entry
      // and set the target list
      //
      TgtLstIdx = (UINT8)((IoApicBase - IOAPIC_BASE) / SAD_IOAPIC_GRANTY);
      TotEntry = (UINT8)((IoApicLimit - IoApicBase) / SAD_IOAPIC_GRANTY);
      for (Idx2 = 0; Idx2 < TotEntry; ++Idx2) {
        *ApicTgtLst = (*ApicTgtLst & (~(Mask << ((TgtLstIdx + Idx2) * SADTGT_SIZE)))) | (Idx1 << ((TgtLstIdx + Idx2) * SADTGT_SIZE));
      }

      //
      // Calculate the Target List Index & Total Target List for Legacy IO entry
      // and set the target list
      //
      if (host->var.qpi.CpuInfo[Idx1].CpuRes.IoBase < host->var.qpi.CpuInfo[Idx1].CpuRes.IoLimit) {
        TgtLstIdx = (UINT8)(LegIoBase / SAD_LEG_IO_GRANTY);
        TotEntry = (UINT8)((LegIoLimit - LegIoBase) / SAD_LEG_IO_GRANTY);
        for (Idx2 = 0; Idx2 < TotEntry; ++Idx2) {
          *IoTgtLst |= (Idx1 << ((TgtLstIdx + Idx2) * SADTGT_SIZE));
          *IoTgtLst = (*IoTgtLst & (~(Mask << ((TgtLstIdx + Idx2) * SADTGT_SIZE)))) | (Idx1 << ((TgtLstIdx + Idx2) * SADTGT_SIZE));
        }
      }

      //
      // For EP:
      //   MMIO rules 0-3 used for MMIOL region of CPUs 0-3 and rules 4-7 used for MMIOH region of CPUs 0-3.
      // 
      // For EX:
      //   MMIO rules 0-7 used for MMIOL region of CPUs 0-7 and rules 8-15 used for MMIOH region of CPUs 0-7.
      //   Since the 32-bit target list value will be same for MMIOL & MMIOH region, the caller will use the same 
      //   value for both target list registers
      //
      if (((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType == CPU_HSX_EX)) ||
          ((host->var.common.cpuType == CPU_BDX) && (host->var.common.cpuSubType == CPU_BDX_EX)) 
          ) {
        *MmioTgtLst = *MmioTgtLst & (~(Mask << (Idx1 * SADTGT_SIZE)));
        *MmioTgtLst = *MmioTgtLst | (Idx1 << (Idx1 * SADTGT_SIZE));
      } else {
        *MmioTgtLst = *MmioTgtLst & (~((Mask << (Idx1 * SADTGT_SIZE)) | (Mask << ((Idx1 + 4) * SADTGT_SIZE))));
        *MmioTgtLst = *MmioTgtLst | ((Idx1 << (Idx1 * SADTGT_SIZE)) | (Idx1 << ((Idx1 + 4) * SADTGT_SIZE)));
      }
    }
  }

  return QPI_SUCCESS;
}

QPI_STATUS
ConvertR3QPIRoutingTable (
  UINT8                   InPort,
  UINT8                   OutPort,
  UINT8                   DestSocId,
  UINT32                  *EncodedRoute
  )
/*++

Routine Description:

  Calculate the R3QPI pass-thru route encoding for given input & output port 

Arguments:

  InPort      - input QPI port for route through  
  OutPort      - Output QPI port for route through
  DestSocId   - endpoint processor socket ID

Returns:

  EncodingBit - Encoded pass thru route setting

  0 - Successful
  1 - Failure

--*/
{
  UINT32 Data32;
  UINT8  RouteLookUpTable[SI_MAX_QPI_PORTS][SI_MAX_QPI_PORTS]= { 
        {0xFF, 0x01, 0x02} ,
        {0x01, 0xFF, 0x02} ,
        {0x02, 0x03, 0xFF}
    }; 

  Data32 = RouteLookUpTable[InPort][OutPort];  // determine route port based on InPort & Outport
  *EncodedRoute = Data32 << ((2 * DestSocId) + (16 * (InPort & 0x1)));

  return QPI_SUCCESS;
}

QPI_STATUS
CheckForTopologyChange (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:
  Routine to check if the topology changed after warm reset. Any such topology change requires 
  QPIRC cold reset flow to be forced on the system since the uncore ring & RTID programming no longer correct for the
  changed topology. Note that when such topology change occurs it could be only due to some links not training; it can not 
  be due to some links that failed to train before but trained successfully this time around. The reason is a link
  that failt to train is disabled by clearing PhyInitBegin. So such links can successfully train only after Cold Reset.

Arguments:
  host   - Input/Output structure of the QPIRC
  SocketData - Info such as CPUs populated and their LEP.
  QpiInternalGlobal - QPIRC internal variables.

Returns:

--*/
{
  UINT8           Index, Port;
  UINT32          Data32, Sr02;

  // Collect the previous topology/LEP from SR02
  Data32 = QpiReadPciCfg(host, QpiInternalGlobal->SbspSoc, 0, BIOSSCRATCHPAD2_UBOX_MISC_REG);
  Sr02 = (Data32 & 0xFFFFFF00) >> 8;

  // Check if the current LEP matches previous boot LEP
  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    if (SocketData->Cpu[Index].Valid == TRUE) {
      for (Port = 0; Port < MAX_QPI_PORTS; ++Port) {
        if (SocketData->Cpu[Index].LinkData[Port].Valid != ((Sr02 >> ((Index * SI_MAX_QPI_PORTS) + Port)) & 0x1)) {
          QpiLogWarning(host, Index, SOCKET_TYPE_CPU, Port, WARN_LINK_FAILURE);
          QpiDebugPrintWarn((host, QPI_DEBUG_WARN, "\n    CPU%u Link%u failed to train. Topology changed across reset!!! ", Index, Port));
          QpiInternalGlobal->ForceColdResetFlow = TRUE; // Indicate that the Cold Reset flow needs to be redone
        }
      }
    }
  }

  return QPI_SUCCESS;
}

QPI_STATUS
SanityCheckDiscoveredTopology (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  interrogatest discovered topology for errors

Arguments:

  host  - Pointer to the system host (root) structure
  SocketData  - socket specific data structure
  QpiInternalGlobal - QPIRC internal variables.


Returns:

  0 - Successful
  1 - Failures
  
--*/
{
  BOOLEAN             ErrFlag;
  UINT8               Index;
  UINT8               Index1;
  UINT32              TempCpuList;
  TOPOLOGY_TREE_NODE  *TopologyTree;

  ErrFlag = FALSE;

  //
  // Topology Tree sanity check. Check the topology tree of each socket to see if it has all the other sockets in its tree.
  //
  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    TempCpuList = 0;
    if (SocketData->Cpu[Index].Valid == TRUE) {
      TopologyTree  = SocketData->CpuTree[Index];
      TempCpuList   = QpiInternalGlobal->CpuList;
      for (Index1 = 0; TopologyTree[Index1].Node.Valid != FALSE; ++Index1) {
        if (TopologyTree[Index1].Node.SocType == SOCKET_TYPE_CPU) {
          if (TempCpuList) {
            TempCpuList &= (~(1 << TopologyTree[Index1].Node.SocId));
          } else {
            QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n CPU%u not found in CPU%u Topology Tree!!! ", Index1, Index));
            ErrFlag = TRUE;
            break;
          }
        } else {
          QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n Invalid Socket Type found in CPU%u Topology Tree!!! ", Index));
          ErrFlag = TRUE;
        }
      }
    }

    if (TempCpuList) {
      QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n One or more CPUs not found in CPU%u Topology Tree!!! ", Index));
      ErrFlag = TRUE;
    }
  }

  if (ErrFlag == TRUE) {
    QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n Topology Discovery Failed!!!\n"));
    if (host->var.qpi.RasInProgress == TRUE) {
      return QPI_FAILURE;
    } else {
    QPI_ASSERT (FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  }
  }

  return QPI_SUCCESS;
}

QPI_STATUS
SanityCheckLep (
  struct sysHost         *host,
  QPI_CPU_SOCKET_DATA    *CpuData
  )
/*++

Routine Description:

  Sanity checks the LEP data.

Arguments:
  CpuData  - socket specific data structure

Returns:

  QPI_SUCCESS

--*/
{
  UINT8   Idx, Port;
  UINT8   PeerSocId, PeerPort;

  //
  // Do some sanity checking of the link exchange data such as link Valid,
  // Peer Socket ID and Peer Port are consistent at both the ends of a given link.
  //

  for (Idx = 0; Idx < MAX_CPU_SOCKETS; ++Idx) {
    if (CpuData[Idx].Valid == TRUE) {
      for (Port = 0; Port < MAX_QPI_PORTS; ++Port) {
        if (CpuData[Idx].LinkData[Port].Valid == TRUE) {
          PeerSocId = (UINT8) CpuData[Idx].LinkData[Port].PeerSocId;
          PeerPort  = (UINT8) CpuData[Idx].LinkData[Port].PeerPort;
          if (CpuData[PeerSocId].Valid != TRUE ||
              CpuData[PeerSocId].LinkData[PeerPort].Valid != TRUE ||
              CpuData[PeerSocId].LinkData[PeerPort].PeerSocId != Idx ||
              CpuData[PeerSocId].LinkData[PeerPort].PeerPort != Port) {
            QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n CPU%u Port%u has incosistent LEP with Peer socket. Topology Not Supported ", Idx, Port));
            QPI_ASSERT (FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
          }
        }
      }
    }
  }

  return QPI_SUCCESS;
}

QPI_STATUS
PreProcessFeatureVsTopology (
  struct sysHost             *host,
  QPI_SOCKET_DATA            *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Chooses config precedence topology vs feature

Arguments:
  None
  
Returns:

  QPI_SUCCESS

--*/
{
  UINT8    Idx1, Idx2;
  UINT8    PeerSocList[MAX_CPU_SOCKETS];
  UINT8    PeerSocCnt[MAX_CPU_SOCKETS];
  UINT8    ThreeNeighborCount = 0;
  UINT8    ThreeNeighborSocList[MAX_CPU_SOCKETS];
  UINT8    CrossLinkEndSoc1, CrossLinkEndSoc2;
  UINT8    RemotePort;
  UINT32   Data32;
  
  //
  // By default we will choose topology as precedence for defining configuration, if setup option is set then choose feature as precedence
  //
  if (host->setup.qpi.DegradePrecedence == FEATURE_PRECEDENCE) {
    if (((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType != CPU_HSX_EX)) ||
        ((host->var.common.cpuType == CPU_BDX) && (host->var.common.cpuSubType != CPU_BDX_EX)))
    {
      if (host->setup.common.isocEn || host->setup.qpi.ClusterOnDieEn) {
        if (host->var.qpi.SysConfig == SYS_CONFIG_2SEN)  {
          host->var.qpi.SysConfig = SYS_CONFIG_1S;
          DegradeTo1S (host, SocketData, QpiInternalGlobal);
        }
      }
    }
  }
  
  // 
  // COD WA
  if (host->var.qpi.OutClusterOnDieReduction) {
    if (QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_EXA) { // For EX- SKUs,
      // OutClusterOnDieReduction can be set for 2S-1Q, 2S-2Q, 3S and
      // non-4S-FC- topologies
      if (host->var.common.numCpus == 4) { // For topo with 4 sockets - ring and non-FC-
        if (QpiInternalGlobal->Is4SRing == TRUE) { // 4S Ring - no reduction required
          // Print a note and continue boot without any reduction
          QpiDebugPrintWarn ((host, QPI_DEBUG_WARN, "\n;  Ignoring the false flag of COD reduction for 4S-Ring."));
        } else {
          // Identify the peer socket list and count for each populated socket first
          for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; Idx1++) {
            PeerSocList[Idx1] = 0;
            PeerSocCnt[Idx1] = 0;
            if (SocketData->Cpu[Idx1].Valid == TRUE) {
              for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; Idx2++) {
                if (SocketData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) {
                  if ((PeerSocList[Idx1] & (1 << SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId)) == 0) {
                    PeerSocList[Idx1] = PeerSocList[Idx1] |(UINT8)(1 << SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId);
                    PeerSocCnt[Idx1]++;
                  }
                } // link valid
              } // for loop Idx2
            } // cpu valid
          } // for loop Idx1
          
          // Scan the number of neighbors and mark the sockets with 
          // number of neighbors == 3
          for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; Idx1++) {
            if (PeerSocCnt[Idx1] == 3) {
              ThreeNeighborSocList[ThreeNeighborCount] = Idx1;
              ThreeNeighborCount++;
            }
          } // Idx1 loop
          
          if (ThreeNeighborCount == 2) {
            // We have a ring and one of the cross link active;
            // We can degrade to a 4S-Ring from this topology
            CrossLinkEndSoc1 = ThreeNeighborSocList[0];
            CrossLinkEndSoc2 = ThreeNeighborSocList[1];
            
            // Scan the QPI ports of 'CrossLinkEndSoc1' socket to find the port which connects
            // the neighbor socket
            for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; Idx2++) {
              if (SocketData->Cpu[CrossLinkEndSoc1].LinkData[Idx2].Valid == TRUE) {
                if (SocketData->Cpu[CrossLinkEndSoc1].LinkData[Idx2].PeerSocId == CrossLinkEndSoc2) {
                  RemotePort = (UINT8) SocketData->Cpu[CrossLinkEndSoc1].LinkData[Idx2].PeerPort;
                  SocketData->Cpu[CrossLinkEndSoc2].LinkData[RemotePort].Valid = FALSE;
                  SocketData->Cpu[CrossLinkEndSoc1].LinkData[Idx2].Valid = FALSE;
                  QpiDebugPrintWarn ((host, QPI_DEBUG_WARN, "\n; Invalidate link between S%uP%u and S%uP%u for forcing to 4S ring topology", CrossLinkEndSoc1, Idx2, CrossLinkEndSoc2, RemotePort));
                  break;
                }
              } // valid port
            } // Idx2 loop
          } else {
            QpiDebugPrintWarn ((host, QPI_DEBUG_WARN, "\n; Unsupported or unknown topology for COD. Forcing to 1S topology"));
            host->var.qpi.SysConfig = SYS_CONFIG_1S;
            DegradeTo1S (host, SocketData, QpiInternalGlobal);
          } // PeerSocCnt checks
        } // ring check
      } else {
        QpiDebugPrintWarn ((host, QPI_DEBUG_WARN, "\n; Unsupported or unknown topology for COD. Forcing to 1S topology"));
        host->var.qpi.SysConfig = SYS_CONFIG_1S;
        DegradeTo1S (host, SocketData, QpiInternalGlobal);  
      } 
    } else { // For EP- SKUs, 
      // OutClusterOnDieReduction can be set for 2S-1Q, 3S and non-4S-ring- topologies
      if ((host->var.qpi.SysConfig == SYS_CONFIG_2SEP) ||
          (host->var.qpi.SysConfig == SYS_CONFIG_4S && QpiInternalGlobal->Is4SRing == TRUE)) {
          // Do nothing as COD is supported in here for these topologies
      } else {  
        QpiDebugPrintWarn ((host, QPI_DEBUG_WARN, "\n; Unsupported or unknown topology for COD. Forcing to 1S topology"));
        host->var.qpi.SysConfig = SYS_CONFIG_1S;
        DegradeTo1S (host, SocketData, QpiInternalGlobal);
      } // topology check
    } // SKU check
    
    // Since we have degraded to 1S, skip the vetting of 
    // AP checkins of sockets (earlier bypassed)
    if (host->var.qpi.SysConfig != SYS_CONFIG_1S) {
      for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; Idx1++) {
        if ((SocketData->Cpu[Idx1].Valid == TRUE) && 
            (QpiInternalGlobal->SkippedCheckedInCheck & (1 << Idx1))) {
          // AP checkin should have happened by now for skipped sockets
          Data32 = QpiReadPciCfg (host, Idx1, 0, BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG);
          if ((Data32 & 0x1) == 0) {
            QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n\n AP Socket %u is discovered but hasn't come out of reset. System Halted!!!", Idx1));
            QPI_ASSERT (FALSE, ERR_BOOT_MODE, MINOR_ERR_PBSP_CHKIN_FAILURE);
          }
        } // valid socket
      } // for loop Idx1
    } // 1S topo check
  } // OutClusterOnDieReduction check
  return QPI_SUCCESS;
}

VOID
TopologyInfoDump (
  struct sysHost      *host,
  QPI_CPU_SOCKET_DATA     *Cpu,
  TOPOLOGY_TREE_NODE     (*CpuTree)[MAX_TREE_NODES],
  BOOLEAN                    SiPort
  )
/*++

Routine Description:

  Dump topology info to output device

Arguments:

  host  - Pointer to the system host (root) structure
  Cpu   - socket specific data structure

Returns:

  VOID

--*/
{
  UINT8               Index, Index1, Index2;
  UINT8               Port;
  UINT8               RootVn;
  UINT8               TempStr[MAX_CPU_SOCKETS * 2];
  TOPOLOGY_TREE_NODE  *TopologyTree;
  TOPOLOGY_TREE_NODE  Parent, Child;

  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    if (Cpu[Index].Valid == TRUE) {
      TopologyTree = CpuTree[Index];
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;CPU%u Topology Tree", Index));
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;-------------------"));
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;Index  Socket  ParentSocket  ParentPort  ParentIndex  Hop"));
       for (Index1 = 0; TopologyTree[Index1].Node.Valid != FALSE; ++Index1) {
        if (Index1 == 0) {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; %02u     CPU%u      %s           %s          %s        %1u",
                          Index1, TopologyTree[Index1].Node.SocId, " -- ", "--", "--", TopologyTree[Index1].Node.Hop));
        } else {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; %02u     CPU%u      CPU%u           %02u          %02u        %1u",
                          Index1, TopologyTree[Index1].Node.SocId, TopologyTree[TopologyTree[Index1].Node.ParentIndex].Node.SocId, TopologyTree[Index1].Node.ParentPort,
                          TopologyTree[Index1].Node.ParentIndex, TopologyTree[Index1].Node.Hop));
        }
      }
    }
  }

  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    if (Cpu[Index].Valid == TRUE) {
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;"));
      TopologyTree = CpuTree[Index];
      RootVn = Cpu[TopologyTree[0].Node.SocId].Vn;
      for (Index1 = 1; TopologyTree[Index1].Node.Valid != FALSE; ++Index1) {
        MemSetLocal ((VOID *) TempStr, '$', sizeof (TempStr));
        Index2 = 0;
        Child = TopologyTree[Index1];
        while (Child.Node.Hop != 0) {
          Parent = TopologyTree[Child.Node.ParentIndex];
          Port = GetChildLocalPortFromLEP (host, Cpu, &Parent, &Child);
          TempStr[Index2++] = (UINT8)((Child.Node.SocId  << 4) | Child.Node.ParentPort);
          if (SiPort == TRUE) {
            Port = TranslateToSiPort (host, Port);
            TempStr[Index2++] = (UINT8)(Parent.Node.SocId << 4) | Port;
          } else {
            TempStr[Index2++] = (UINT8)(Parent.Node.SocId << 4) | Port;
          }
          Child = Parent;
        }
        
        if (Index2 > (MAX_CPU_SOCKETS * 2)) {
          QPI_ASSERT (FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
        } else {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n"));
        for (Index2 = Index2 - 1; Index2 != 0; --Index2) {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\"S%u P%u VN%u %s\" -> ", (TempStr[Index2] >> 4), (TempStr[Index2] & 0xF),
                                        RootVn, (Index2 % 2) ? "TX" : "RX"));
        }
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\"S%u P%u VN%u RX\";", (TempStr[Index2] >> 4), (TempStr[Index2] & 0xF), RootVn));
        } // checking Index2 value
      }
    }
  }

  return;
}

VOID
RouteTableDump (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Dump topology info to output device

Arguments:

  host  - Pointer to the system host (root) structure
  SocketData  - socket specific data structure
  QpiInternalGlobal - QPIRC internal variables.

Returns:

  VOID

--*/
{
  UINT8               Index, Index1;
  UINT32              Data32;
  UINT8               PortFieldWidth;
  UINT8               PortFieldMask;
  
  PortFieldWidth = 4; // Two bit field to encode link 0, 1 & 2
  PortFieldMask  = 0xF;
  
  //
  // Dump the Route Table
  //
  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    if (SocketData->Cpu[Index].Valid == TRUE) {
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n\n;CPU %1u Routing Table", Index));
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;-------------------"));
      if (SocketData->Route[Index].CboOddQpiRt == SocketData->Route[Index].CboEvenQpiRt) {
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;DestSocket  Port"));
        for (Index1 = 0; Index1 < MAX_CPU_SOCKETS; ++Index1) {
          if (SocketData->Cpu[Index1].Valid == TRUE && Index1 != Index) { // Exclude Self
            Data32 = PortFieldWidth * Index1;                   //HSX/BDX fields indexed by socket Id
            QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;   CPU%1u       %1u", Index1,((SocketData->Route[Index].CboOddQpiRt >> Data32) & PortFieldMask)));
          }
        }
      } else {  //Interleaved (non-uniform) routing configured.  Display both "even" and "odd" routes.
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;Cbo    DestSocket    Port"));
        for (Index1 = 0; Index1 < MAX_CPU_SOCKETS; ++Index1) {
          if (SocketData->Cpu[Index1].Valid == TRUE && Index1 != Index) { // Exclude Self
            Data32 = PortFieldWidth * Index1;                   //HSX/BDX fields indexed by socket Id
            QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;Even     CPU%1u         %1u", Index1, ((SocketData->Route[Index].CboEvenQpiRt >> Data32) & PortFieldMask)));
            QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;Odd      CPU%1u         %1u", Index1, ((SocketData->Route[Index].CboOddQpiRt  >> Data32) & PortFieldMask)));
          }//Dest socket valid
        }//For each dest socket
      }//Uniform vs. interleaved routes
    }//Current socket valid
  }//For each socket

  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n"));

  return ;
}

VOID
LepDump (
  struct sysHost         *host,
  QPI_CPU_SOCKET_DATA    *CpuData
  )
/*++

Routine Description:

  Dumps Link Exchange parameters to output device

Arguments:
  CpuData  - socket specific data structure

Returns:

  VOID

--*/
{
  UINT8         Index;

  UINT8         Index1;
  QPI_LINK_DATA *LinkData;

  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n\n;Link Exchange Parameter"));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;-----------------------"));

  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    if (CpuData[Index].Valid == TRUE) {
      LinkData = CpuData[Index].LinkData;
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;CPU%u ", Index));
      for (Index1 = 0; Index1 < MAX_QPI_PORTS; ++Index1) {
        if (LinkData[Index1].Valid == TRUE) {
          QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, ": LEP%u(%u:CPU%u) ", Index1, LinkData[Index1].PeerPort, LinkData[Index1].PeerSocId));
        }
      }
    }
  }

  return ;
}

UINT8
TranslateToGenPort (
  struct sysHost             *host,
  UINT8                   SiPort
  )
/*++

Routine Description:

Arguments:

Returns:

   0 - Success

--*/
{

  // Currently for all supported CPUs, Si port numbering and generic port numbering is same
  return SiPort;
}

UINT8
TranslateToSiPort (
  struct sysHost             *host,
  UINT8                   GenPort
  )
/*++

Routine Description:

Arguments:
   
Returns:

   0 - Success

--*/
{

  // Currently for all supported CPUs, Si port numbering and generic port numbering is same
  return GenPort;
}

BOOLEAN
CheckCpuConnectedToLegCpu (
  struct sysHost             *host,
  QPI_SOCKET_DATA            *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8                      SocId
  )
/*++

Routine Description:
   Returns TRUE if the CPU is connected to Legacy CPU.

Arguments:
   host - add argument and description to function comment
   SocketData - add argument and description to function comment
   QpiInternalGlobal - QPIRC internal variables.
   SocId   - CPU Socket Number, 0 based

Returns:
   TRUE  - CPU is connected to Legacy CPU
   FALSE - CPU is not connected to Legacy CPU
   
--*/
{
  UINT8 Index;

  //
  // Check the peer of each link to see if the given CPU is directly connected to Legacy CPU
  //
  for (Index = 0; Index < MAX_QPI_PORTS; ++Index) {
    if (SocketData->Cpu[SocId].LinkData[Index].Valid == TRUE &&
        SocketData->Cpu[SocId].LinkData[Index].PeerSocType == SOCKET_TYPE_CPU &&
        SocketData->Cpu[SocId].LinkData[Index].PeerSocId == QpiInternalGlobal->SbspSoc) {
      return TRUE;
    }
  }

  return FALSE;
}

BOOLEAN
DuplicateLinkExists (
  struct sysHost          *host,
  QPI_CPU_SOCKET_DATA     *CpuData,
  TOPOLOGY_TREE_NODE      *Parent,
  TOPOLOGY_TREE_NODE      *Child
  )
/*++

Routine Description:
   Checks if dual-link exists between parent & child from their LEP.

Arguments:
  CpuData      - Contains the LEP of Parent & Child nodes
  Parent             - Socket at one end of the link
  Child                - Socket at the other end of the link

Returns:
  TRUE   - Dual-link exists
  FALSE  - Dual-link doesn't exist

--*/
{
  UINT8         QpiLink;
  QPI_LINK_DATA TempLep;
  UINT8         Count;

  QPI_ASSERT ((CpuData != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Parent != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Child != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  Count = 0;

  if (Parent->Node.SocType == SOCKET_TYPE_CPU) {
    for (QpiLink = 0; QpiLink < MAX_QPI_PORTS; ++QpiLink) {
      TempLep = CpuData[Parent->Node.SocId].LinkData[QpiLink];
      if (TempLep.Valid == TRUE && TempLep.PeerSocId == Child->Node.SocId && TempLep.PeerSocType == Child->Node.SocType) {
        Count++;
      }
    }
  }

  if (Count > 1) {
    return TRUE;
  }

  return FALSE;
}

QPI_STATUS
RemoveDualLink (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData
  )
/*++

Routine Description:

  This routine checks if the given non-2S topology has more than link connecting any 2 given sockets. If
  found, it retains only one and marks the other link(s) as invalid.This routine should not be called if the
  given system is 2S.

Arguments:
  
  host   - Input/Output structure of the QPIRC
  SocketData - Socket specific data structure

Returns:
  
  0 - Successful

--*/
{
  UINT8   Idx1, Idx2, Idx3;
  UINT32  PeerSocId;
  UINT32  PeerPort;

  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (SocketData->Cpu[Idx1].Valid == TRUE) {
      for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; ++Idx2) {
        if (SocketData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) {
          for (Idx3 = 0; Idx3 < MAX_QPI_PORTS; ++Idx3) {
            if ((Idx3 != Idx2) && (SocketData->Cpu[Idx1].LinkData[Idx3].Valid == TRUE)) {
              //
              // Check if this is a dual link and mark it as invalid
              //
              if ((SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId == SocketData->Cpu[Idx1].LinkData[Idx3].PeerSocId) && 
                  (SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocType == SocketData->Cpu[Idx1].LinkData[Idx3].PeerSocType)) {
                PeerSocId    = SocketData->Cpu[Idx1].LinkData[Idx3].PeerSocId; 
                PeerPort     = SocketData->Cpu[Idx1].LinkData[Idx3].PeerPort;
                QpiDebugPrintWarn ((host, QPI_DEBUG_WARN, "\n;  CPU%u has more than one link (%u, %u) connecting to CPU%u (%u, %u).   Disabling link CPU%uP%u <-> CPU%uP%u. ",
                                Idx1, Idx2, Idx3,                                                    //Soc # and local dual-link ports
                                PeerSocId, SocketData->Cpu[Idx1].LinkData[Idx2].PeerPort, PeerPort,  //Peer Soc # and Peer dual-link ports
                                Idx1, Idx3, PeerSocId, PeerPort));                                   //Link to be disabled:  Soc # and port (both ends)
                SocketData->Cpu[Idx1].LinkData[Idx3].Valid = 0;
                SocketData->Cpu[PeerSocId].LinkData[PeerPort].Valid = 0;
              }
            }
          }
        }
      }
    }
  }

  return QPI_SUCCESS;
}


QPI_STATUS
GetDualLinkFromLEP (
  struct sysHost          *host,
  QPI_CPU_SOCKET_DATA     *CpuData,
  TOPOLOGY_TREE_NODE      *Node1,
  TOPOLOGY_TREE_NODE      *Node2,
  UINT8                   *Port1,
  UINT8                   *Port2
  )
/*++

Routine Description:
   Returns the Qpi port numbers of the dual-link of Node1 socket.

Arguments:
  CpuData      - Contains the LEP of Node1 & Node2 nodes
  Node1             - Socket at one end of the link
  Node2             - Socket at the other end of the link
  Port1              - Port1 of the dual-link
  Port2              - Port2 of the dual-link
  
Returns:
  Port1   - Port number of one link of the dual-link
  Port2   - Port number of other link of the dual-link

--*/
{
  UINT8         QpiLink;
  QPI_LINK_DATA TempLep;

  QPI_ASSERT ((CpuData != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Node1 != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Node2 != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Port1 != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Port2 != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  *Port1  = 0xFF;
  *Port2  = 0xFF;

  //
  // Only CPU sockets can have a dual-link
  //
  if (Node1->Node.SocType == SOCKET_TYPE_CPU) {
    for (QpiLink = 0; QpiLink < MAX_QPI_PORTS; ++QpiLink) {
      TempLep = CpuData[Node1->Node.SocId].LinkData[QpiLink];
      if (TempLep.Valid == TRUE && TempLep.PeerSocId == Node2->Node.SocId && TempLep.PeerSocType == Node2->Node.SocType) {
        if (*Port1 == 0xFF) {
          *Port1 = QpiLink;
        } else if (*Port2 == 0xFF) {
          *Port2 = QpiLink;
        } else {
          //
          // parent has more than 2 links to child
          //
          QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n CPU %u has more than two links to CPU %u. Topology not supported ", Node1->Node.SocId, Node2->Node.SocId));
          QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
        }
      }
    }
  }

  return QPI_SUCCESS;
}

UINT8
GetChildLocalPortFromLEP (
  struct sysHost             *host,
  QPI_CPU_SOCKET_DATA    *CpuData,
  TOPOLOGY_TREE_NODE      *Parent,
  TOPOLOGY_TREE_NODE      *Child
  )
/*++

Routine Description:
   Returns port number of the Parent socket that connects it to Child socket.

Arguments:
  CpuData      - Contains the LEP of Parent & Child nodes
  Parent             - Socket at one end of the link
  Child               - Socket at the other end of the link
  
Returns:
  Port of the Parent socket that connects it to the child socket
  0xFF - Child is not an immediate neighbor of Parent

--*/
{
  UINT8         QpiLink;
  QPI_LINK_DATA TempLep;

  QPI_ASSERT ((CpuData != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Parent != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Child != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  if (Parent->Node.SocType == SOCKET_TYPE_CPU) {
    for (QpiLink = 0; QpiLink < MAX_QPI_PORTS; ++QpiLink) {
      TempLep = CpuData[Parent->Node.SocId].LinkData[QpiLink];
      if (TempLep.Valid != TRUE || TempLep.DualLink == TRUE) { // Skip the Invalid/Duplicate Links
        continue;
      }

      if (TempLep.PeerSocId == Child->Node.SocId && TempLep.PeerSocType == Child->Node.SocType) {
        return QpiLink;
      }
    }
  }

  return 0xFF;
}

UINT8
GetChildPeerPortFromLEP (
  struct sysHost             *host,
  QPI_CPU_SOCKET_DATA    *CpuData,
  TOPOLOGY_TREE_NODE      *Parent,
  TOPOLOGY_TREE_NODE      *Child
  )
/*++

Routine Description:
   Returns port number of the Child socket that connects it to the Parent socket.

Arguments:
  CpuData      - Contains the LEP of Parent & Child nodes
  Parent             - Socket at one end of the link
  Child               - Socket at the other end of the link
  
Returns:
  Port of the Child socket that connects it to the Parent socket
  0xFF - Child is not an immediate neighbor of Parent

--*/
{
  UINT8         QpiLink;
  QPI_LINK_DATA TempLep;

  QPI_ASSERT ((CpuData != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Parent != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Child != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  if (Parent->Node.SocType == SOCKET_TYPE_CPU) {
    for (QpiLink = 0; QpiLink < MAX_QPI_PORTS; ++QpiLink) {
      TempLep = CpuData[Parent->Node.SocId].LinkData[QpiLink];
      if (TempLep.Valid != TRUE || TempLep.DualLink == TRUE) { // Skip the Invalid/Duplicate Links
        continue;
      }

      if (TempLep.PeerSocId == Child->Node.SocId && TempLep.PeerSocType == Child->Node.SocType) {
        return (UINT8) TempLep.PeerPort;
      }
    }
  }

  return 0xFF;
}


QPI_STATUS
GetParentSocket (
  struct sysHost             *host,
  TOPOLOGY_TREE_NODE      *TopologyTree,
  TOPOLOGY_TREE_NODE      *Child,
  TOPOLOGY_TREE_NODE      *Parent
  )
/*++

Routine Description:
   Returns immediate Parent of Child socket in Topology Tree.

Arguments:
  TopologyTree   - Topology Tree
  Child               - Child Socket
  Parent             - Parent Socket
  
Returns:
  0 - Successful, parent found
  1 - Failure

--*/
{

  QPI_ASSERT ((TopologyTree != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Parent != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Child != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  Parent->RawNode = TopologyTree[Child->Node.ParentIndex].RawNode;
  if (Parent->Node.Valid != TRUE) {
    QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n\n Parent Node not found."));
    QPI_ASSERT (FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  }

  return QPI_SUCCESS;

}

UINT8
GetParentIndex (
  struct sysHost             *host,
  QPI_CPU_SOCKET_DATA    *CpuData,
  TOPOLOGY_TREE_NODE   *Tree,
  TOPOLOGY_TREE_NODE   *Child
  )
/*++

Routine Description:

  finds the child's parent in tree

Arguments:
  CpuData - socket specific data structure
  Tree  - system QPI fabric node tree
  Child  - current node tree

Returns:

  0 - Successful
  1 - Failure

--*/
{
  UINT8         Index;
  QPI_LINK_DATA Lep;

  QPI_ASSERT ((Tree != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Child != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Child->Node.Valid == TRUE), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  if (Child->Node.SocType == SOCKET_TYPE_CPU) {
    Lep = CpuData[Child->Node.SocId].LinkData[Child->Node.ParentPort];

    for (Index = 0; Index < MAX_TREE_NODES; ++Index) {
      if (Tree[Index].Node.Valid == TRUE &&
          Tree[Index].Node.SocId == Lep.PeerSocId &&
          Tree[Index].Node.SocType == Lep.PeerSocType
          ) {
        return Index;
      }
    }
  } else {
    QPI_ASSERT (FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  }

  return 0xFF;
}

UINT8
GetChildHopLength (
  struct sysHost             *host,
  TOPOLOGY_TREE_NODE   *Tree,
  UINT8                SocId,
  UINT8                SocType
  )
/*++

Routine Description:

  counts hops to child from current socket

Arguments:
  Tree  - system QPI fabric node tree
  SocId   - processor socket id
  SocType - device type in socket

Returns:

  0 - Successful
  1 - Failure

--*/
{
  UINT8 Index;

  QPI_ASSERT ((Tree != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  for (Index = 0; Index < MAX_TREE_NODES; ++Index) {
    if (Tree[Index].Node.Valid == TRUE && Tree[Index].Node.SocId == SocId && Tree[Index].Node.SocType == SocType) {
      return (UINT8) Tree[Index].Node.Hop;
    }
  }

  return 0xFF;
}

BOOLEAN
NodeFoundInTopologyTree (
  struct sysHost             *host,
  TOPOLOGY_TREE_NODE    *Tree,
  TOPOLOGY_TREE_NODE    *SearchNode
  )
/*++

Routine Description:

  Find a QPI node within overall QPI fabric

Arguments:
  Tree        - QPI fabric tree structure
  LeafNode    - QPI fabric tree structure

Returns:

  0 - Successful
  1 - Failure

--*/
{
  UINT8 Index;

  QPI_ASSERT ((Tree != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((SearchNode != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  for (Index = 0; Index < MAX_TREE_NODES; ++Index) {
    //
    // If there are no more valid nodes, then the node in question is not in the tree
    //
    if (Tree[Index].Node.Valid == FALSE) {
      return FALSE;
    }
    //
    // If socket ID and Type matches then we found the node in the tree
    //
    if (Tree[Index].Node.SocId == SearchNode->Node.SocId &&
        Tree[Index].Node.SocType == SearchNode->Node.SocType &&
        Tree[Index].Node.Hop == SearchNode->Node.Hop &&
        Tree[Index].Node.ParentPort == SearchNode->Node.ParentPort 
        ) {
      return TRUE;
    }
  }

  return FALSE;
}

BOOLEAN
NodeFoundInMinPathTree (
  struct sysHost          *host,
  TOPOLOGY_TREE_NODE      *Tree,
  TOPOLOGY_TREE_NODE      *SearchNode
  )
/*++

Routine Description:
   Searches the Topology tree to see if the node under search is already found in the given SBSP min path tree.

Arguments:
  host  - Pointer to the system host (root) structure
  Tree           - Topology Tree
  SearchNode     -  Node to search in the topology tree

Returns:
  TRUE   - Node already exists in tree
  FALSE  - Node doesn't exist in tree

--*/
{
  UINT8 Index;

  QPI_ASSERT ((Tree != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((SearchNode != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  for (Index = 0; Index < MAX_TREE_NODES; ++Index) {
    //
    // If there are no more valid nodes, then the node in question is not in the tree
    //
    if (Tree[Index].Node.Valid == FALSE) {
      return FALSE;
    }
    //
    // If socket ID & Type matches we found the node
    //
    if (Tree[Index].Node.SocId == SearchNode->Node.SocId && Tree[Index].Node.SocType == SearchNode->Node.SocType) {
      return TRUE;
    }
  }

  return FALSE;
}

QPI_STATUS
AddNodeToTopologyTree (
  struct sysHost             *host,
  TOPOLOGY_TREE_NODE      *Tree,
  TOPOLOGY_TREE_NODE      *Node
  )
/*++

Routine Description:

  Adds the current node to the tree

Arguments:
  Tree  - system QPI fabric node tree
  Node  - current node tree

Returns:

  0 - Successful
  1 - Failure

--*/
{
  UINT8 Index;

  QPI_ASSERT ((Tree != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Node != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  for (Index = 0; Index < MAX_TREE_NODES; ++Index) {
    //
    // Place the tree in first available empty slot in the array
    //
    if (Tree[Index].Node.Valid == FALSE) {
      Tree[Index] = *Node;
      return QPI_SUCCESS;
    }
  }

  return (QPI_STATUS) QPI_FAILURE;
}

QPI_STATUS
AddNodeToRingTree (
  struct sysHost          *host,
  TOPOLOGY_TREE_NODE      *Tree,
  TOPOLOGY_TREE_NODE      *Node
  )
/*++

Routine Description:

  Adds the current node to the tree

Arguments:
  Tree  - system QPI fabric node tree
  Node  - current node tree

Returns:

  0 - Successful
  1 - Failure

--*/
{
  UINT8 Index;

  QPI_ASSERT ((Tree != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Node != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  for (Index = 0; Index < MAX_RING_TREE_NODES; ++Index) {
    //
    // Place the tree in first available empty slot in the array
    //
    if (Tree[Index].Node.Valid == FALSE) {
      Tree[Index] = *Node;
      return QPI_SUCCESS;
    }
  }

  return (QPI_STATUS) QPI_FAILURE;
}


UINT8
CountImmediateChildren (
  struct sysHost             *host,
  TOPOLOGY_TREE_NODE      *Tree,
  TOPOLOGY_TREE_NODE      *Parent
  )
/*++

Routine Description:

  Counts hops in path

Arguments:
  Tree  - system QPI fabric node tree
  Node  - current node tree

Returns:

  0 - Successful
  1 - Failure

--*/
{
  UINT8 Idx;
  UINT8 ParentIdx;
  UINT8 Children;

  QPI_ASSERT ((Tree != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Parent != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  ParentIdx = 0xFF;
  Children  = 0;

  for (Idx = 0; Idx < MAX_TREE_NODES; ++Idx) {
    if (Tree[Idx].Node.Valid == FALSE) {
      return Children;
    }

    if (Tree[Idx].Node.SocId == Parent->Node.SocId &&
        Tree[Idx].Node.SocType == Parent->Node.SocType &&
        Tree[Idx].Node.Hop == Parent->Node.Hop &&
        Tree[Idx].Node.ParentPort == Parent->Node.ParentPort
        ) {
      ParentIdx = Idx;
      break;
    }
  }

  if (ParentIdx != 0xFF) {
    for (Idx = 0; Idx < MAX_TREE_NODES; ++Idx) {
      if (Tree[Idx].Node.Valid == FALSE) {
        return Children;
      }

      if (Tree[Idx].Node.ParentIndex == ParentIdx) {
        ++Children;
      }
    }
  }

  return Children;
}

BOOLEAN
ShorterPathExists (
  struct sysHost             *host,
  TOPOLOGY_TREE_NODE      *Tree,
  TOPOLOGY_TREE_NODE      *SearchNode
  )
/*++

Routine Description:
   Searches the Topology tree to see if the node under search is already found in the 
   tree at lower Hop levels. In effect, it checks if there is a shorter path exists
   from the root node to this node.

Arguments:
  Tree               - Topology Tree
  SearchNode     -  Node to search in the topology tree

Returns:
  TRUE   - Shorter path exists
  FALSE  - Shorter path doesn't exist

--*/
{
  UINT8 Index;

  QPI_ASSERT ((Tree != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((SearchNode != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  for (Index = 0; Index < MAX_TREE_NODES; ++Index) {
    //
    // If there are no more valid nodes, then the node in question is not in the tree
    //
    if (Tree[Index].Node.Valid == FALSE) {
      return FALSE;
    }
    //
    // If socket ID & Type matches and the hop level of the matching socket is less than the hop level of the node under search,
    // we found a shorter path
    //
    if (Tree[Index].Node.SocId == SearchNode->Node.SocId &&
        Tree[Index].Node.SocType == SearchNode->Node.SocType &&
        Tree[Index].Node.Hop < SearchNode->Node.Hop
        ) {
      return TRUE;
    }

  }

  return FALSE;
}

BOOLEAN
CompareRing (
  struct sysHost       *host,
  UINT8                *Ring1,
  UINT8                *Ring2
  )
/*++

Routine Description:

Arguments:

Returns:

  TRUE - Rings are same
  FALSE - Rings are different

--*/
{
  UINT8 Idx1, Idx2;
  UINT8 Match = 0;

  QPI_ASSERT ((Ring1 != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT ((Ring2 != NULL), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  for (Idx1 = 0; Idx1 < 4; ++Idx1) {
    for (Idx2 = 0; Idx2 < 4; ++Idx2) {
      if (Ring1[Idx1] == Ring2[Idx2]) {
        Match = Match | (1 << Idx1);
        break;
      }
    }
  }

  if (Match == 0xF) {
    return TRUE;
  } else {
    return FALSE;
  }
}


VOID
QpiCheckPoint (
  UINT8                      SocId,
  UINT8                      SocType,
  UINT8                      QpiLink,
  UINT8                      MajorCode,
  UINT8                      MinorCode,
  struct sysHost             *host
  )
/*++

Routine Description:
  Routine to display Error/Status code; it also updates a scratch register with additional
  info such as socket, link related to the Error/Status.

Arguments:
  SocId         - Bit mask, where bit x corresponds to CPU (CPU:0...3). Set to 0xFF if the Error/Status is System Wide.
  SocType       - 0: CPU socket. Set to 0xFF if the Error/Status is System Wide.
  QpiLink       - Bit mask, where bit x corresponds to CPU QPI port 0/1. Set to 0xFF if the Error/Status is system wide, 0 if Link Not Applicable.
  MajorCode     - Major Status or Error code
  MinorCode     - Minor Status or Error code
  host          - add argument and description to function comment
  
Returns:
  None.

--*/

{
  UINT16  data;

  //
  // Update the scratch register.
  //
  // Register Usage:
  // [31:24] - Error/Status Major Code
  // [23:16] - Error/Status Minor Code
  // [15:13] - Reserved for future use
  // [12:5] - Bitmask of CPU Sockets affected, 0xF - System Wide Error/Status
  // [4:3] - Socket Type. 0: CPU Socket, 0x3 System Wide Error/Status
  // [2:0] - Bit mask of QPI Links affected, 0 - Link Not Applicable, 0x3 - System Wide Error/Status

  data = ((SocId & 0xFF) << 5) | ((SocType & 0x3) << 3) | ((QpiLink & 0x7) << 0);
  OutputCheckpoint (host, MajorCode, MinorCode, data);

  return ;
}


VOID
QpiAssert (
  struct sysHost             *host,
  UINT8                      majorCode,
  UINT8                      minorCode
  )
/*++

Routine Description:
  Routine to call platformfatal and assert
  

Arguments:
  MajorCode     - Major Status or Error code
  MinorCode     - Minor Status or Error code
  host          - add argument and description to function comment
  
Returns:
  Does not return

--*/

{
  UINT8           haltOnError;
  
  FatalErrorHook (host, majorCode, minorCode, &haltOnError);
  //
  //haltOnError is used by OEM to indicate if wants to halt or not.  This is useful in MRC, but not Qpi.  Qpi will ignore and always halt
  //
  haltOnError = 1;
  while (*(volatile UINT8 *) &haltOnError);
  
  return;
}

VOID
QpiLogWarning (
  struct sysHost             *host,
  UINT8                      SocId,
  UINT8                      SocType,
  UINT8                      QpiLink,
  UINT8                      WarnCode
  )
/*++

Routine Description:
  Routine to Log Warning in QPIRC output structure.

Arguments:
  host       - To be filled with the warning log
  SocId         - Bit mask, where bit x corresponds to CPU (CPU:0...3). Set to 0xFF if the Warning is system wide.
  SocType       - 0: CPU socket. Set to 0xFF if the Warning is system wide.
  QpiLink       - Bit mask, where bit x corresponds to CPU QPI port 0/1. Set to 0xFF if the Warning is system wide, 0 if Link Not Applicable.
  WarnCode      - Warning Code

Returns:
  None.

--*/
{
  UINT32  Ctr;

  //
  // Log the Warning information in the QPI output structure
  // [31:20] - Reserved, set to 0
  // [20:13] - Bitmask of CPU Sockets affected, 0xF - System wide Warning
  // [12:11] - Socket Type. 0 - CPU Socket, 0x3 - System Wide Warning
  // [10:08] - Bit mask of QPI Links affected. 0x3 - System Wide Warning, 0 - Link Not Applicable
  // [07:00] - Warning Code
  //
  // Identify unused log entry; last entry is used to indicate that the log is overflowing
  //
  for (Ctr = 0; Ctr < MAX_WARNING_LOGS - 1; ++Ctr) {
    if (host->var.qpi.WarningLog[Ctr] == 0) {
      break;
    }
  }

  if (Ctr == (MAX_WARNING_LOGS - 1)) {
    QpiDebugPrintWarn ((host, QPI_DEBUG_WARN, "\n\n **** Warning Log Overflowed ****\n"));
    host->var.qpi.WarningLog[Ctr] = (UINT32) WARNING_LOG_OVERFLOW;
    return ;
  }

  host->var.qpi.WarningLog[Ctr] = ((SocId & 0xFF) << 13) | ((SocType & 0x3) << 11) | ((QpiLink & 0x7) << 8) | WarnCode;

  return ;
}

UINT32
QpiReadPciCfg(
  struct sysHost        *host,
  UINT8                 SocId,
  UINT8                 BoxInst,
  UINT32                RegOffset
)
/*++

Routine Description:

    QPI specific read PCI Configuration

Arguments:
  Host      - Pointer to the system host (root) structure
  SocId     - CPU Socket Node number (Socket ID)
  BoxInst   - Box Instance, 0 based
  RegOffset - Register offset; values come from the auto generated header file

Returns:

  register value read

--*/
{
#ifndef BUILDING_FOR_X64
  return ReadCpuPciCfgEx (host, SocId, BoxInst, RegOffset);  
#else
        return ReadCpuPciCfgRas(host, SocId, BoxInst, RegOffset);  
#endif
}

VOID
QpiWritePciCfg(
  struct sysHost        *host,
  UINT8                 SocId,
  UINT8                 BoxInst,
  UINT32                RegOffset,
  UINT32                data
  )
/*++

Routine Description:

    QPI specific write PCI Configuration

Arguments:

  Host      - Pointer to the system host (root) structure
  SocId     - CPU Socket Node number (Socket ID)
  BoxInst   - Box Instance, 0 based
  RegOffset - Register offset; values come from the auto generated header file
  data      - Data to write


Returns:

  register value read

--*/
{
#ifndef BUILDING_FOR_X64
        WriteCpuPciCfgEx (host, SocId, BoxInst, RegOffset, data);  
#else
        WriteCpuPciCfgRas(host, SocId, BoxInst, RegOffset, data);  
#endif

}

UINT8
QpiReadIar(
  struct sysHost        *host,
  UINT8                 SocId,
  UINT8                 BoxInst,
  UINT8                 UnitId,
  UINT8                 RegOffset
  )
/*++

Routine Description:

    QPI specific read PCI Configuration

Arguments:

  Host      - Pointer to the system host (root) structure
  SocId     - CPU Socket Node number (Socket ID)
  BoxInst   - Box Instance, 0 based
  UnitId    - Unit ID (lane 0-19=0-19, fwdc=20, reserved=21-62, PQD=62, Broadcast Data Lane=63)
  RegOffset - Register offset; values come from the auto generated header file
  data      - Data to write


Returns:

  register value read

--*/
{
  PQ_CSR_AFEIAR_QPIAGENT_DFX_PHY_STRUCT AfeIar;
  UINT8   Data8;

  AfeIar.Data = 0;
  AfeIar.Bits.register_id = RegOffset & 0x3F;
  AfeIar.Bits.unit_id = UnitId & 0x3F;
  AfeIar.Bits.write_en = 0;
  AfeIar.Bits.hw_override_en = 1;
  QpiWritePciCfg (host, SocId, BoxInst, PQ_CSR_AFEIAR_QPIAGENT_DFX_PHY_REG, AfeIar.Data); 
  AfeIar.Data = QpiReadPciCfg (host, SocId, BoxInst, PQ_CSR_AFEIAR_QPIAGENT_DFX_PHY_REG);
  Data8 = (UINT8) AfeIar.Bits.read_data;

  return Data8;
}

VOID
QpiWriteIar(
  struct sysHost        *host,
  UINT8                 SocId,
  UINT8                 BoxInst,
  UINT8                 UnitId,
  UINT8                 RegOffset,
  UINT8                 data
  )
/*++

Routine Description:

    QPI specific write PCI Configuration

Arguments:

  Host      - Pointer to the system host (root) structure
  SocId     - CPU Socket Node number (Socket ID)
  BoxInst   - Box Instance, 0 based
  UnitId    - Unit ID (lane 0-19=0-19, fwdc=20, reserved=21-62, PQD=62, Broadcast Data Lane=63)
  RegOffset - Register offset; values come from the auto generated header file
  data      - Data to write


Returns:

  register value read

--*/
{
  PQ_CSR_AFEIAR_QPIAGENT_DFX_PHY_STRUCT AfeIar;

  AfeIar.Data = 0;
  AfeIar.Bits.write_en = 1;
  if (!(host->var.common.emulation & (SOFT_SDV_FLAG | SIMICS_FLAG | VP_FLAG))) {
    while(AfeIar.Bits.write_en != 0) {
      AfeIar.Data = QpiReadPciCfg (host, SocId, BoxInst, PQ_CSR_AFEIAR_QPIAGENT_DFX_PHY_REG);
    }
  }

  AfeIar.Data = 0;
  AfeIar.Bits.register_id = RegOffset & 0x3F;
  AfeIar.Bits.unit_id = UnitId & 0x3F;
  AfeIar.Bits.wr_data = (UINT8) data;
  AfeIar.Bits.write_en = 1;
  AfeIar.Bits.hw_override_en = 1;
  QpiWritePciCfg (host, SocId, BoxInst, PQ_CSR_AFEIAR_QPIAGENT_DFX_PHY_REG, AfeIar.Data); 
}

VOID
QpiFixedDelay (
  struct sysHost      *host,
  UINT32          MicroSecs
  )
/*++

Routine Description:
   Delay routine

Arguments:
   MicroSecs     - Number of micro seconds to delay

Returns:

--*/

{

  FixedDelay (host, MicroSecs);

  return;
}

VOID
SpinUntilLbcIsFree (
  struct sysHost            *host,
  UINT8                     Socket,
  UINT8                     LinkIndex
  )
/*++

Routine Description:
   Spins in a tight loop forever or until the LBC completes processing the previous command and is ready
   to accept a new command

Arguments:
   host  - Pointer to the system host (root) structure
   Socket - SocId we are looking up
   Link - Link# on socket

Returns: None
++*/
{
  UINT16 WaitCount = 0;
  LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_STRUCT  LbcPerIouControl;

  // Do an early return to allow the simulators to run w/o infinite loops
  if (host->var.common.emulation & (SOFT_SDV_FLAG | SIMICS_FLAG | VP_FLAG)) {
    return;
  }
  
  // Ensure that the previously issued commands were executed before LBC_PER_IOU_DATA is over-written
  // for the new command
  LbcPerIouControl.Data = QpiReadPciCfg (host, Socket, LinkIndex, LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG);

  while (LbcPerIouControl.Bits.lbc_start != 0x0) {
    QpiFixedDelay(host, 1000); // 1000 uSec delay
    LbcPerIouControl.Data = QpiReadPciCfg (host, Socket, LinkIndex, LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG);
    // Assert if the LBC fails to respond even after 1s
    if (WaitCount++ >= 1000) {
        QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n\nLBC time-out Socket=%d, Link=%d, ControlData=0x%08X", Socket, LinkIndex, LbcPerIouControl.Data));
        QPI_ASSERT(FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_TIMEOUT);
    } // WaitCount check
  } // spin loop
}

VOID
GetNeighborSocs (
  struct sysHost             *host,
  QPI_SOCKET_DATA            *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8                      Soc,
  UINT8                      *NeighborSoc1,
  UINT8                      *NeighborSoc2,
  UINT8                      *RemoteSoc
  )
/*++

Routine Description:
   Returns neighbor and/or remote socket of the given socket.

Arguments:
   host  - Pointer to the system host (root) structure
   SocketData - pointer socket specific data
   QpiInternalGlobal - QPIRC internal variables.
   Soc - current processor socket ID
   NeighborSoc1 - QPI port 1 neighbor socket ID
   NeighborSoc2 - QPI port 2 neighbor socket ID
   RemoteSoc - Remote socket ID

Returns:

  VOID                               
                               
--*/
{
  UINT8 Ctr;
  UINT32 TempList;

  *NeighborSoc1 = *NeighborSoc2 = *RemoteSoc = 0xFF;

  if (host->var.qpi.SysConfig == SYS_CONFIG_1S) {
    return;
  } else if (host->var.qpi.SysConfig == SYS_CONFIG_2SEN || host->var.qpi.SysConfig == SYS_CONFIG_2SEP) {
    (Soc == QpiInternalGlobal->SbspSoc) ? (*NeighborSoc1 = 1) : (*NeighborSoc1 = QpiInternalGlobal->SbspSoc);
    return;
  }

  //
  // Is it a 4S system with only 2 CPUs populated?
  //
  if (QpiInternalGlobal->TotCpu == 2) {
    if (SocketData->Cpu[Soc].LinkData[0].Valid == TRUE) {
      *NeighborSoc1 = (UINT8) SocketData->Cpu[Soc].LinkData[0].PeerSocId;
    } else {
      *NeighborSoc1 = (UINT8) SocketData->Cpu[Soc].LinkData[1].PeerSocId;
    }
    //
    // Even though the other 2 sockets are missing, identify them for RTID programming simplicity.
    //
    for (Ctr = 0; Ctr < MAX_CPU_SOCKETS; ++Ctr) {
      if ((QpiInternalGlobal->CpuList & (1 << Ctr)) == 0) {
        *NeighborSoc2 = Ctr;
        break;
      }
    }

    for (++Ctr; Ctr < MAX_CPU_SOCKETS; ++Ctr) {
      if ((QpiInternalGlobal->CpuList & (1 << Ctr)) == 0) {
        *RemoteSoc = Ctr;
        break;
      }
    }

    return;
  } else {
    //
    // Must be a 3S or  4S system
    //
    if (SocketData->Cpu[Soc].LinkData[0].Valid == TRUE && SocketData->Cpu[Soc].LinkData[1].Valid == TRUE) {
      *NeighborSoc1 = (UINT8) SocketData->Cpu[Soc].LinkData[0].PeerSocId;
      *NeighborSoc2 = (UINT8) SocketData->Cpu[Soc].LinkData[1].PeerSocId;
      TempList      = (QpiInternalGlobal->CpuList) & ((1 << Soc) | (1 << *NeighborSoc1) | (1 << *NeighborSoc2));
      for (Ctr = 0; Ctr < QpiInternalGlobal->TotCpu; ++Ctr) {
        if ((TempList & (1 << Ctr)) == 0) {
          //
          // Socket that is missing in the list must be the Remote Socket
          //
          *RemoteSoc = Ctr;
          break;
        }
      }
    } else {
      if (SocketData->Cpu[Soc].LinkData[0].Valid == TRUE) {
        //
        // This must be serially connected topology with the given socket at the begining or at the end
        //
        *NeighborSoc1 = (UINT8) SocketData->Cpu[Soc].LinkData[0].PeerSocId;
      } else {
        *NeighborSoc1 = (UINT8) SocketData->Cpu[Soc].LinkData[1].PeerSocId;
      }

      if (SocketData->Cpu[*NeighborSoc1].LinkData[0].PeerSocId == Soc) {
        //
        // NeighborSoc1 must have both the links valid
        //
        *NeighborSoc2 = (UINT8) SocketData->Cpu[*NeighborSoc1].LinkData[1].PeerSocId;
      } else {
        *NeighborSoc2 = (UINT8) SocketData->Cpu[*NeighborSoc1].LinkData[0].PeerSocId;
      }

      if (QpiInternalGlobal->TotCpu == 3) {
        TempList = (QpiInternalGlobal->CpuList) & ((1 << Soc) | (1 << *NeighborSoc1) | (1 << *NeighborSoc2));
        for (Ctr = 0; Ctr < QpiInternalGlobal->TotCpu; ++Ctr) {
          if ((TempList & (1 << Ctr)) == 0) {
            //
            // Socket that is missing in the list must be the Remote Socket
            //
            *RemoteSoc = Ctr;
            break;
          }
        }
      } else {
        if (SocketData->Cpu[*NeighborSoc2].LinkData[0].PeerSocId == *NeighborSoc1) {
          //
          // NeighborSoc2 must have both the links valid
          //
          *RemoteSoc = (UINT8) SocketData->Cpu[*NeighborSoc2].LinkData[1].PeerSocId;
        } else {
          *RemoteSoc = (UINT8) SocketData->Cpu[*NeighborSoc2].LinkData[0].PeerSocId;
        }
      }
    }
  }
  return;
}

#ifdef BUILDING_FOR_X64

UINT32
ReadCpuPciCfgRas (
  PSYSHOST host,
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset
  )
{
  UINT8       *RegRealAddr, Flag, Size;
  UINT32      RegAddress, ShadowBufOffset, RegRealDev, RegRealFun, RegRealOff, FlagOffset, Data32;
  CSR_OFFSET  RegOffset;

  // Register Read support is not needed for these registers
  if ((Offset == QPIRBTSO_QPIAGENT_LL_REG) || (Offset == QPIRBTSD_QPIAGENT_LL_REG)) {
    return 0;
  }
  // Get the size of the register being accessed   
  RegOffset.Data = Offset;  
  Size = (UINT8)RegOffset.Bits.size;

  // Get the real address of this register
  RegRealAddr = GetCpuPciCfgAddress (host, SocId, BoxInst, Offset, &Size);

  RegAddress = (UINT32)((UINTN)RegRealAddr);
  RegRealDev = (RegAddress >> 15) & 0x1F;
  RegRealFun = (RegAddress >> 12) & 0x7;
  RegRealOff = RegAddress & 0xFFF;
  
  if (mDirectCommit == TRUE || RegRealOff >= MAX_REG_OFFSET) {
    // These registers can not be in the shadow buffer, so read it from actual register
    if (Size == sizeof (UINT32)) {
      Data32 = *((volatile UINT32 *)RegRealAddr);
    } else if (Size == sizeof (UINT16)) {
      Data32 = *((volatile UINT16 *)RegRealAddr);
    } else {
      Data32 = *((volatile UINT8 *)RegRealAddr);
    }
  } else {
    // If the shawdow buffer has the latest copy, return it; otherwise read it from the register
    ShadowBufOffset = (SocId * MAX_BUF_PER_CPU) + (RegRealDev * MAX_BUF_PER_DEV) + (RegRealFun * MAX_BUF_PER_FUN) + RegRealOff;
    Flag = *(mCpuShadowFlag + (ShadowBufOffset / 8));
    FlagOffset = ShadowBufOffset % 8;
    if (Size == sizeof (UINT32)) {
      Flag = (Flag >> FlagOffset) & 0xF;
      if (Flag != 0) {
        Data32 = *((UINT32 *)(mCpuShadowBuf + ShadowBufOffset));
      } else {
        Data32 = *((volatile UINT32 *)RegRealAddr);
      }
    } else if (Size == sizeof (UINT16)) {
      Flag = (Flag >> FlagOffset) & 0x3;
      if (Flag != 0) {
        Data32 = *((UINT16 *)(mCpuShadowBuf + ShadowBufOffset));
      } else {
        Data32 = *((volatile UINT16 *)RegRealAddr);
      }
    } else {
      Flag = (Flag >> FlagOffset) & 0x1;
      if (Flag != 0) {
        Data32 = *((UINT8 *)(mCpuShadowBuf + ShadowBufOffset));
      } else {
        Data32 = *((volatile UINT8 *)RegRealAddr);
      }
    }
  }

  return Data32;

}

void
WriteCpuPciCfgRas (
  PSYSHOST host,
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset,
  UINT32   Data
  )
{
  UINT8       *RegRealAddr, Flag, Size;
  UINT32      RegAddress, ShadowBufOffset, RegRealDev, RegRealFun, RegRealOff, FlagOffset;
  CSR_OFFSET  RegOffset;

  // Get the size of the register being accessed   
  RegOffset.Data = Offset;  
  Size = (UINT8)RegOffset.Bits.size;

  // Get the real address of this register
  RegRealAddr = GetCpuPciCfgAddress (host, SocId, BoxInst, Offset, &Size);

  RegAddress = (UINT32)((UINTN)RegRealAddr);
  RegRealDev = (RegAddress >> 15) & 0x1F;
  RegRealFun = (RegAddress >> 12) & 0x7;
  RegRealOff = RegAddress & 0xFFF;
  
  //
  // Some registers can be programmed with out quiescing; the caller is responsible for turning this flag on/off
  // If the register is not tracked in shadow buffer, then those registers also directly commited to the real register.
  //
#ifdef QPI_RAS_DEBUG
  if (RegRealOff >= MAX_REG_OFFSET) {
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1 , "\n QPI_ASSERT(mDirectCommit == TRUE) RegAddress = = 0x%08X > 0x200 \n",RegAddress));
  }
#endif

  if (mDirectCommit == TRUE || RegRealOff >= MAX_REG_OFFSET) {   // krishna.. remove mdirectCommit
    if (Size == sizeof (UINT32)) {
      *((volatile UINT32 *)RegRealAddr) = Data;
    } else if (Size == sizeof (UINT16)) {
      *((volatile UINT16 *)RegRealAddr) = (UINT16) Data;
    } else {
      *((volatile UINT8 *)RegRealAddr) = (UINT8) Data;
    }
#ifdef QPI_RAS_DEBUG
#ifdef QPI_RAS_DEBUG_REG_DETAIL
    QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n    Direct WRITE    Addr: 0x%08X    Data:0x%08X    Size:%u",(UINT32)RegRealAddr, Data, Size));
#endif
#endif
  } else {

#ifdef QPI_RAS_DEBUG
#ifdef QPI_RAS_DEBUG_REG_DETAIL
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1 , "\n QPI milestone mNextEntry = %x",mNextEntry ));
#endif
#endif
    //
    // Update the Shawdow Buffer and respective flag indicating that the shawdow buffer has the latest copy of the register
    //
    ShadowBufOffset = (SocId * MAX_BUF_PER_CPU) + (RegRealDev * MAX_BUF_PER_DEV) + (RegRealFun * MAX_BUF_PER_FUN) + RegRealOff;
    Flag = *(mCpuShadowFlag + (ShadowBufOffset / 8));
    FlagOffset = ShadowBufOffset % 8;
    if (Size == sizeof (UINT32)) {
      *((UINT32 *)(mCpuShadowBuf + ShadowBufOffset)) = Data;
      Flag = Flag | (0xF << FlagOffset);
      *(mCpuShadowFlag + (ShadowBufOffset / 8)) = Flag;
      mNextEntry->AccessWidth = 4;
      mNextEntry->DataMask.OrMask = Data;
    } else if (Size == sizeof (UINT16)) {
      *((UINT16 *)(mCpuShadowBuf + ShadowBufOffset)) = (UINT16)Data;
      Flag = Flag | (0x3 << FlagOffset);
      *(mCpuShadowFlag + (ShadowBufOffset / 8)) = Flag;
      mNextEntry->AccessWidth = 2;
      mNextEntry->DataMask.OrMask = (UINT16)Data;
    } else {
      *((UINT8 *)(mCpuShadowBuf + ShadowBufOffset)) = (UINT8)Data;
      Flag = Flag | (0x1 << FlagOffset);
      *(mCpuShadowFlag + (ShadowBufOffset / 8)) = Flag;
      mNextEntry->AccessWidth = 1;
      mNextEntry->DataMask.OrMask = (UINT8)Data;
    }

    // Update the Quiesce Buffer
    mNextEntry->Operation = EnumQuiesceWrite;
    mNextEntry->CsrAddress = (UINT64) RegRealAddr;
    mNextEntry->AndMask = 0;
    mNextEntry++;
#ifdef QPI_RAS_DEBUG
    CsrWriteCheckPoint();
#endif
  }

  return;
}
#endif // BUILDING_FOR_X64

QPI_STATUS
SendMailBoxCmdToPcode (
  struct sysHost      *host,
  UINT8               Cpu,
  UINT32              Command,
  UINT32              Data
  )
{


  //
  //MailBoxCMD to set pcode comand
  //
  // Wait until pCode Mailbox is available (i.e. BUSY_RUN flag is clear)
  //
  
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Wait for mailbox ready"));
  while (QpiReadPciCfg (host, Cpu, 0, BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG) & BIT31) {
  }
  
  //
  // Write data to the BIOS to Pcode Mailbox data register
  //
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Send Data portion of command.  Socket = %u Data sent == 0x%x", Cpu, Data));
  QpiWritePciCfg (host, Cpu, 0, BIOS_MAILBOX_DATA_PCU_FUN1_REG, Data);

  //
  // Write pcode mailbox command code
  //
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Send Pcode mailbox command. Socket = %u Command sent == 0x%x", Cpu, Command));
  QpiWritePciCfg (host, Cpu, 0, BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG, Command | BIT31);

  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Wait for mailbox ready"));
#ifdef BUILDING_FOR_X64
  if (mDirectCommit == TRUE) {
#endif
    while (QpiReadPciCfg (host, Cpu, 0, BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG) & BIT31) {
    }
#ifdef BUILDING_FOR_X64
  }
#endif

  return QPI_SUCCESS;

}

