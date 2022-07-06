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
// *      This file contains PHY/LL portion of QPIRC.                       *
// *                                                                        *
// **************************************************************************
--*/

#include "DataTypes.h"
#include "PlatformHost.h"
#include "SysHost.h"
#include "SysFunc.h"
#include "QpiLib.h"
#include "QpiMisc.h"

QPI_STATUS
IssuePhyResetOnCpuLink(
  struct sysHost      *host,
  UINT8               Cpu,
  UINT8               Link,
  UINT8               ResetType
  );

STATIC
QPI_STATUS
ProgramLateAction (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  UINT32              Value
  );


QPI_STATUS
ProgramPhyLayer (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

STATIC
QPI_STATUS
DisableLinkForceToLowPower(
  struct sysHost      *host,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal,
  UINT8               Socket,
  UINT8               Link
  );

STATIC
QPI_STATUS
DisableForceToLowPowerAllUnusedLinks (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );


STATIC
QPI_STATUS
EnableClockGatingToAllUnusedLinks (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

STATIC
QPI_STATUS
LinkLayerResetWA (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

STATIC
QPI_STATUS
GetSocketLinkEparams(
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  UINT8               Socket,
  UINT8               Link,
  VOID   *LinkSpeedParameter,
  VOID   *LinkHalfSpeedParameter
  );


QPI_STATUS
ConfigSocketLinkPhyMisc (
  struct sysHost            *host,
  QPI_SOCKET_DATA           *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal,
  UINT8                     Socket,
  UINT8                     Link,
  BOOLEAN                   Qpi11FeatureDisable
  );

QPI_STATUS QpiPmWa (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

QPI_STATUS
ConfigSocketLinkPhyMisc_0 (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  UINT8               Socket,
  UINT8               Link,
  BOOLEAN             ProbeInstalled
  );

QPI_STATUS
ConfigSocketLinkEparams (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  UINT8               Socket,
  UINT8               Link,
  VOID                *LinkSpeedParameter,
  VOID                *LinkHalfSpeedParameter
  );

QPI_STATUS
ConfigSocketLinkEparamsHsx (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  UINT8               Socket,
  UINT8               Link,
  HSX_PER_LANE_EPARAM_LINK_INFO                *LinkSpeedParameter,
  HSX_ALL_LANES_HS_EPARAM_LINK_INFO            *LinkHalfSpeedParameter
  );



QPI_STATUS
ConfigSocketLinkForLinkLayerMisc (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal,
  UINT8               Index,
  UINT8               LinkIndex,
  QPILCP_QPIAGENT_LL_HSX_BDX_STRUCT   QpiLcp
  );

QPI_STATUS
ConfigSocketLinkForLinkLayerMisc_0 (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal,
  UINT8               Index  
  );

STATIC
QPI_STATUS
ProgramLinkLayer (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

QPI_STATUS
SelectSupportedLinkSpeedForHotAdd (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal,
  UINT8              *LinkSpeed
  );

QPI_STATUS
SelectSupportedLinkSpeed (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

UINT8
GetMaxSupportedLinkSpeed (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData
  );

QPI_STATUS
CheckLinkSpeedSupported (
   struct sysHost      *host,
   QPI_SOCKET_DATA     *SocketData,
   UINT8               RequestedLinkSpeed
  );

QPI_STATUS
NormalizeLinkOptions (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

                     


QPI_STATUS
SendMailBoxCmdSetQpiFreq (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

BOOLEAN
DisableFeatureWhenQpiProbeInstalled (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData
  );

BOOLEAN
IsQpiProbeInstalled (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData
  );

QPI_STATUS
ProgramLateActionOnCpuLink(
   struct sysHost           *host,
   UINT8                    Cpu,
   UINT8                    Link,
   UINT32                   Value
  );

QPI_STATUS
ProgramPhyLayerOnCpuLink (
  struct sysHost         *host,
  QPI_SOCKET_DATA        *SocketData,
  UINT8                  Index,
  UINT8                  LinkIndex,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );


//
// Convert target Link Speed to the corresponding pcode Link Freq ratio.
//
_inline UINT32 ConvertSpeedToFreq (
  struct sysHost            *host,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal,
  QPI_LINK_SPEED            LinkSpeed
){
  return (LinkSpeed + 7);  //Link Freq = Link Speed + 7;
};


QPI_STATUS
QpiTransitionFullSpeed (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  )
/*++

Routine Description:
   Transition the QPI links to full speed operation.

   To transition the link to full speed, for each link we need to:
//      1. Validate the phy/link layer input parameters; force safe values if needed
      2. Ensure that no partial loading of parameters by setting LA Disable
      3. Program the Link & Phy layer parameters
      4. Clear LA Disable

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Socket specific data structure
  QpiInternalGlobal - QPIRC internal variables.
   
Returns:

--*/
{
  UINT32 LinkFreqRatio;
  UINT32 OverClockPllRatio;
  UINT32 Data32;
  UINT8  LinkSpeed;

  QpiCheckPoint(0xFF, 0xFF, 0xFF, STS_FULL_SPEED_TRANSITION, 0, host);

  //
  // Transition to FULL speed?
  //
  if ((host->setup.qpi.QpiLinkSpeedMode == QPI_LINK_FULL_SPEED_MODE) && ((QpiInternalGlobal->TotCpu != 1) || (host->setup.qpi.QpiLbEn))) {
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n;  ****** Selecting QPI freq. - START ******"));   
    SelectSupportedLinkSpeed(host, SocketData, QpiInternalGlobal);
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n;  ****** Selecting QPI freq. - END   ******"));   
    
    NormalizeLinkOptions (host, SocketData, QpiInternalGlobal);
    
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n;  Setting Late Action Disable bit for each socket/link"));
    ProgramLateAction (host, SocketData, 1);

    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n;  ******       Phy init      - START ******"));   
    QpiCheckPoint(0xFF, 0xFF, 0xFF, STS_PHY_LAYER_SETTING, 0, host);
    ProgramPhyLayer (host, SocketData, QpiInternalGlobal);
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n;  ******       Phy init      - END   ******"));   

    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n;  ******       LL init       - START ******"));   
    QpiCheckPoint(0xFF, 0xFF, 0xFF, STS_LINK_LAYER_SETTING, 0, host);
    ProgramLinkLayer (host, SocketData, QpiInternalGlobal);
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n;  ******       LL init       - END   ******\n"));   

    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Clearing Late Action Disable bit for each socket/link"));
    ProgramLateAction (host, SocketData, 0);


    //
    //MailBoxCMD to set freq
    //
    SendMailBoxCmdSetQpiFreq(host, SocketData, QpiInternalGlobal);

    host->var.common.resetRequired |= POST_RESET_WARM;

  } else {
    //
    // If boot with only one socket, prepare for future hot plug as pcode will not be executed again for the existing socket.
    //
    if (QpiInternalGlobal->QpiCpuSktHotPlugEn == TRUE) {  //RAS enabled
      // For EX- flow, select the frequency for hot-add
      QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n; ****** Selecting QPI freq. for Hot Add - START ******"));
      SelectSupportedLinkSpeedForHotAdd(host, SocketData, QpiInternalGlobal, &LinkSpeed);
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;  ****** Selecting QPI freq. for Hot Add - END   ******"));  
      LinkFreqRatio = ConvertSpeedToFreq(host, QpiInternalGlobal, LinkSpeed);
      SendMailBoxCmdToPcode(host, (UINT8) QpiInternalGlobal->SbspSoc, MAILBOX_BIOS_CMD_SET_QPI_FREQ,             LinkFreqRatio);
      SendMailBoxCmdToPcode(host, (UINT8) QpiInternalGlobal->SbspSoc, MAILBOX_BIOS_CMD_SET_QPI_FREQ |(0x2 << 8), LinkFreqRatio);
    } else if (QpiInternalGlobal->TotCpu == 1) {  //RAS disabled and only 1 CPU
      // Apply this for EP- and EN- SKU only (Skipping for DE- and EX- SKUs)
      if (QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] <= SKU_EP) {
        // With only 1 CPU running (no RAS), check for Over-clocking and send the required Link Freq ratio to pcode.
        // - Init the list of pcode Link Freq ratios required based on the amount of Over-clocking (PPL Ratio) 
        // - Determine the amount of Over-clocking (PLL Ratio).
        //   Note:  Over-clocking PLL Ratio is stored in MSR-0x61E Bits[1:0] but is passed to BIOS via NonStickyScratchPad2 Bits[3:2]
        // - Determine required Link Freq ratio based on the amount of Over-clocking (PLL Ratio)
        // - Send over-clock adjusted Link Freq ratio to pcode.
        UINT8 OverClockLinkFreqRatio[] = {8, 8, 6, 4};  //Init list of Link Freq ratios required for Over-clocking. Array indexed by PLL Ratio.
      
        Data32 = QpiReadPciCfg (host, QpiInternalGlobal->SbspSoc, 0, BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG);
        OverClockPllRatio = (Data32 & (BIT3+BIT2)) >> 2;            //PLL Ratio passed to BIOS via ScratchPad2 Bits[3:2]

        LinkFreqRatio = OverClockLinkFreqRatio[OverClockPllRatio];

        SendMailBoxCmdToPcode(host, (UINT8) QpiInternalGlobal->SbspSoc, MAILBOX_BIOS_CMD_SET_QPI_FREQ,             LinkFreqRatio);
        SendMailBoxCmdToPcode(host, (UINT8) QpiInternalGlobal->SbspSoc, MAILBOX_BIOS_CMD_SET_QPI_FREQ |(0x2 << 8), LinkFreqRatio);

        if (OverClockPllRatio == 0) {
          QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  1 CPU detected but no Over-clocking (PLL Ratio=0x%02x).  Pcode Link Freq ratio set to 0x%02x",
                                                     OverClockPllRatio, LinkFreqRatio));
        } else {
          QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  1 CPU detected with Over-clocking (PLL Ratio=0x%02x).  Pcode Link Freq ratio set to 0x%02x",
                                                     OverClockPllRatio, LinkFreqRatio));
        }
      } // EX- SKU Check
    }//1-CPU and RAS disabled

    // Inform user why we're not transitioning to Full-Speed mode
    // Force all unused ports to low-power mode.
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;  %s",
       (QpiInternalGlobal->TotCpu == 1) ? ";Single Socket, no QPI Links to transition" : ";Full speed transition not requested"));
    DisableForceToLowPowerAllUnusedLinks(host, SocketData, QpiInternalGlobal);     
  }//No Full-Speed Transition
    
  return QPI_SUCCESS;
}


QPI_STATUS QpiPhyLinkL1WaHsx (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  )
/*++

Routine Description:
  Apply HSX L1 W/A (4169009): HSX Clone: <ES2 Gate>DRT: pkg_cstate fails to enter deep cstate with B0 patch 0xc

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Socket specific data structure
  QpiInternalGlobal - QPIRC internal variables.
   
Returns:

--*/
{
  VID_QPIAGENT_LL_STRUCT                            QpiAgentVid;
  QPIPHYPWRCTRL_QPIAGENT_LL_HSX_BDX_STRUCT          QpiPhyPwrCtrl;
  QPIREUT_PH_CTR_QPIAGENT_REUT_HSX_BDX_STRUCT       QpiReutPhCtr; 
  UINT32                                            L1PortMap[MAX_CPU_SOCKETS];
  UINT8                                             Socket, Link;

  // HSX W/A (4169009 - All Steppings): HSX Clone: <ES2 Gate>DRT: pkg_cstate fails to enter deep cstate with B0 patch 0xc
  // Note: MUST be applied only to ports already in L1 AND after each and EVERY warm reset.

  // Part-1 (HSX W/A 4169009) - Send L1 Port Map to pCode via mailbox.
  // Note: This mailbox command needs to be sent before RST_CPL4
  for (Socket=0; Socket < MAX_CPU_SOCKETS; Socket++) {
    L1PortMap[Socket] = 0;  //Examining next CPU so clear it's L1 port map

    if ((SocketData->Cpu[Socket].Valid == TRUE) || (host->var.qpi.RasSocId == Socket)) {
      // Build L1 Port Map for this Socket and "mail it" to pCode.
      for (Link=0; Link < MAX_QPI_PORTS; Link++) {
        QpiAgentVid.Data   = (UINT16)QpiReadPciCfg(host, Socket, Link, VID_QPIAGENT_LL_REG);
        QpiPhyPwrCtrl.Data = QpiReadPciCfg(host, Socket, Link, QPIPHYPWRCTRL_QPIAGENT_LL_REG);
        if ((QpiAgentVid.Bits.vendor_identification_number != 0xFFFF) &&  //Verify reg is valid before checking L1 mode
            (QpiPhyPwrCtrl.Bits.force_l1 == 1)) {
          QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Applying L1 W/A to Socket %u QPI Link %u.", Socket, Link));
          L1PortMap[Socket] |= (1 << Link);  //Make note that this link is in L1 mode
        }//Link in L1 Mode
      }//For each LInk

      // For HSX-EP-, send the B2P command only when L1PortMap is non-zero. For HSX-EX- and all BDX-, we send this
      // irrespective of the L1PortMap just like the HSX-EX-. This is done to keep up with the backward compatibility
      // with HSX-EP-.
      if ((host->var.common.cpuType == CPU_HSX && host->var.common.cpuSubType == CPU_HSX_EX) || (host->var.common.cpuType == CPU_BDX)) {
        SendMailBoxCmdToPcode(host, Socket, MAILBOX_BIOS_CMD_PROGRAM_QPI_PORTS, L1PortMap[Socket]);
      } else { // For HSX-EP- only,
        // Sent Socket's L1 Port Map to pCode but only if there's at least 1 port in L1 mode.
        if (L1PortMap[Socket] != 0) {
          SendMailBoxCmdToPcode(host, Socket, MAILBOX_BIOS_CMD_PROGRAM_QPI_PORTS, L1PortMap[Socket]);
        }
      }//For HSX-EX- and all BDX-
    }//Socket valid
  }//For each Socket

  // Part-2 (HSX W/A 4169009) - Swizzle the L1 enable bit of each port in L1 mode.
  // Sequence:  Turn L1 off, Reset Phy (1 of 2), Turn L1 on, Reset Phy (2 of 2)
  // Note: Applies to all "new" CPUs i.e. 1) All valid CPUs on first boot, 2) The newly added CPUs via RAS online.
  // Note: No need to re-enter "Begin Phy Init".
  for (Socket=0; Socket < MAX_CPU_SOCKETS; Socket++) {
    if (((host->var.qpi.RasInProgress == FALSE) && (SocketData->Cpu[Socket].Valid == TRUE)) ||
        ((host->var.qpi.RasInProgress == TRUE)  && (host->var.qpi.RasSocId == Socket))) {
      for (Link=0; Link < MAX_QPI_PORTS; Link++) {
        // Apply W/A only on those ports in L1 mode.
        if ((L1PortMap[Socket] & (1 << Link)) != 0) {
          // Turn L1 off
          QpiPhyPwrCtrl.Data = QpiReadPciCfg(host, Socket, Link, QPIPHYPWRCTRL_QPIAGENT_LL_REG);
          QpiPhyPwrCtrl.Bits.force_l1 = 0;
          QpiWritePciCfg(host, Socket, Link, QPIPHYPWRCTRL_QPIAGENT_LL_REG, QpiPhyPwrCtrl.Data);

          // Reset Phy Layer (1st time)
          QpiReutPhCtr.Data = QpiReadPciCfg(host, Socket, Link, QPIREUT_PH_CTR_QPIAGENT_REUT_REG);
          QpiReutPhCtr.Bits.phylayerreset = 1;
          QpiWritePciCfg(host, Socket, Link, QPIREUT_PH_CTR_QPIAGENT_REUT_REG, QpiReutPhCtr.Data);

          QpiFixedDelay(host, 100);  //Need small delay for 1st PhyLayerReset to settle down before 2nd PhyLayerReset

          // Turn L1 back on
          QpiPhyPwrCtrl.Data = QpiReadPciCfg(host, Socket, Link, QPIPHYPWRCTRL_QPIAGENT_LL_REG);
          QpiPhyPwrCtrl.Bits.force_l1 = 1;
          QpiWritePciCfg(host, Socket, Link, QPIPHYPWRCTRL_QPIAGENT_LL_REG, QpiPhyPwrCtrl.Data);

          // Reset Phy Layer (2nd time)
          QpiReutPhCtr.Data = QpiReadPciCfg(host, Socket, Link, QPIREUT_PH_CTR_QPIAGENT_REUT_REG);
          QpiReutPhCtr.Bits.phylayerreset = 1;
          QpiWritePciCfg(host, Socket, Link, QPIREUT_PH_CTR_QPIAGENT_REUT_REG, QpiReutPhCtr.Data);
        }//Link in L1 mode
      }//For each LInk
    }//Socket valid
  }//For each Socket

  return(QPI_SUCCESS);
}


QPI_STATUS QpiPhyLBC46ClearOffWA (
  struct sysHost            *host,
  QPI_SOCKET_DATA           *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  )
/*++

Routine Description:
  Apply BDX- A0 WA of LBC 46 getting cleared after warm-reset

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Socket specific data structure
  QpiInternalGlobal - QPIRC internal variables.
   
Returns:

--*/
{
  UINT8                                             Socket, LinkIndex;
  LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_STRUCT       LbcPerIouControl;
  LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_STRUCT          LbcPerIouData;
  QPIREUT_PH_CTR_QPIAGENT_REUT_HSX_BDX_STRUCT       QpiReutPhCtr;
  QPI_DFX_LCK_CTL_CFG_QPIAGENT_LL_STRUCT            QpiDfxLckCtl;
  QPIPHPRIVC10_QPIAGENT_DFX_PHY_HSX_BDX_STRUCT      QpiPhprivc10;
  QPILS_QPIAGENT_LL_HSX_BDX_STRUCT                  QpiLlLs;
  BOOLEAN                                           FlagAssert=FALSE;
  CC_MISC_CTL_1_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT  CcMiscCtl1;

  for (Socket=0; Socket < MAX_CPU_SOCKETS; Socket++) {
    if (((host->var.qpi.RasInProgress == FALSE) && (SocketData->Cpu[Socket].Valid == TRUE)) ||
        ((host->var.qpi.RasInProgress == TRUE)  && (host->var.qpi.RasSocId == Socket))) {
      for (LinkIndex=0; LinkIndex < MAX_QPI_PORTS; LinkIndex++) {
        if (SocketData->Cpu[Socket].LinkData[LinkIndex].Valid == TRUE) {
          // Setting LBC 46, datalane params - Enable the per-bundle control enable for source-degeneration Cap (st1_ctle_peak) in source-degen Preamp stage (Bit 0),
          // set rx_st1_ctle_peak_g1_qpihalf as 1 and set rx_st1_ctle_peak_g3_qpifull as 15; zero other fields.
          LbcPerIouData.Data = QpiReadPciCfg (host, Socket, LinkIndex, LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_REG);
          LbcPerIouData.Bits.lbc_data = 0x1E03;
          QpiWritePciCfg (host, Socket, LinkIndex, LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_REG, LbcPerIouData.Data);

          LbcPerIouControl.Data = QpiReadPciCfg (host, Socket, LinkIndex, LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG);
          LbcPerIouControl.Bits.lbc_ln_sel = 0xFFFFF;  // Enable this for all data bundles
          LbcPerIouControl.Bits.lbc_ld_sel = 0x2E;
          LbcPerIouControl.Bits.lbc_req = 0x2;
          LbcPerIouControl.Bits.lbc_start = 0x1;
          QpiWritePciCfg (host, Socket, LinkIndex, LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG, LbcPerIouControl.Data);

          // Spin in a tight loop unless the LBC is free
          SpinUntilLbcIsFree( host, Socket, LinkIndex);

          // Must first set this field to zero before attempting to write the QPIPHPRIVC10 registers
          QpiDfxLckCtl.Data = QpiReadPciCfg(host, Socket,  LinkIndex, QPI_DFX_LCK_CTL_CFG_QPIAGENT_LL_REG);
          QpiDfxLckCtl.Bits.privdbg_phy_l0_sync_lck  = 0;
          QpiWritePciCfg(host, Socket,  LinkIndex, QPI_DFX_LCK_CTL_CFG_QPIAGENT_LL_REG, QpiDfxLckCtl.Data);

          QpiPhprivc10.Data = QpiReadPciCfg(host, Socket,  LinkIndex, QPIPHPRIVC10_QPIAGENT_DFX_PHY_REG);
          QpiPhprivc10.Bits.ignore_fast_oc_done  = 1;
          QpiWritePciCfg(host, Socket,  LinkIndex, QPIPHPRIVC10_QPIAGENT_DFX_PHY_REG, QpiPhprivc10.Data);

          // Brickland BDX HSD 4971319 : BDX EX: Need QPI LBC 46 Work-Around Enhancements
          // Grantley BDX HSD 5002719 : BDX QPI: Need hvm_mode = 0x10 for LBC 46 WA enhancent on A & L - Step BDX Si
          if (LinkIndex != 1) {
            CcMiscCtl1.Data = QpiReadPciCfg (host, Socket, LinkIndex, CC_MISC_CTL_1_QPIAGENT_DFX_COMMON_REG);
            CcMiscCtl1.Bits.hvm_mode = 0x10;
            QpiWritePciCfg (host, Socket, LinkIndex, CC_MISC_CTL_1_QPIAGENT_DFX_COMMON_REG, CcMiscCtl1.Data);
          } // LinkIndex check
        } // if link valid
      } // for link loop
    } // if Socket valid
  } // for Socket loop

  QpiCheckPoint(0xFF, 0xFF, 0xFF, STS_PBSP_SYNC_UP, MINOR_STS_ISSUE_PHY_RST, host);

  FlagAssert = FALSE;

  for (Socket=0; Socket < MAX_CPU_SOCKETS; Socket++) {
    if (((host->var.qpi.RasInProgress == FALSE) && (SocketData->Cpu[Socket].Valid == TRUE)) ||
        ((host->var.qpi.RasInProgress == TRUE)  && (host->var.qpi.RasSocId == Socket))) {
      // Issue a reset to all active ports under this socket
      for (LinkIndex=0; LinkIndex < MAX_QPI_PORTS; LinkIndex++) {
        if (SocketData->Cpu[Socket].LinkData[LinkIndex].Valid == TRUE) {
          // Reset Phy Layer
          QpiReutPhCtr.Data = QpiReadPciCfg(host, Socket, LinkIndex, QPIREUT_PH_CTR_QPIAGENT_REUT_REG);
          QpiReutPhCtr.Bits.phylayerreset = 1;
          QpiWritePciCfg(host, Socket, LinkIndex, QPIREUT_PH_CTR_QPIAGENT_REUT_REG, QpiReutPhCtr.Data);
        } // if link valid
      } // for link loop

      // Delay and check if we have trained back by reading the link layer status
      QpiFixedDelay(host, 5000);

      for (LinkIndex=0; LinkIndex < MAX_QPI_PORTS; LinkIndex++) {
        if (SocketData->Cpu[Socket].LinkData[LinkIndex].Valid == TRUE) {
          QpiLlLs.Data = QpiReadPciCfg(host, Socket, LinkIndex, QPILS_QPIAGENT_LL_REG);
          if (QpiLlLs.Bits.link_initialization_status != LINK_SM_NORMAL_OPERATION) {
            FlagAssert = TRUE;
            QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;  Link failure after phy-reset at S%u P%u. Link Status = %u\n", Socket, LinkIndex, QpiLlLs.Bits.link_initialization_status));
          }
        } // if link valid
      } // for link loop

      // Assert with an error that one or more links didn't train post reset
      if (FlagAssert == TRUE) {
        QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;  One or more QPI links failed to train back to normal state in S%u. Skipping other sockets ...", Socket));
        QPI_ASSERT ((FlagAssert != TRUE), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
      }

    } // if Socket valid
  } // for Socket loop

  for (Socket=0; Socket < MAX_CPU_SOCKETS; Socket++) {
    if (((host->var.qpi.RasInProgress == FALSE) && (SocketData->Cpu[Socket].Valid == TRUE)) ||
        ((host->var.qpi.RasInProgress == TRUE)  && (host->var.qpi.RasSocId == Socket))) {
      for (LinkIndex=0; LinkIndex < MAX_QPI_PORTS; LinkIndex++) {
        if (SocketData->Cpu[Socket].LinkData[LinkIndex].Valid == TRUE) {
          // Must first set this field to zero before attempting to write the QPIPHPRIVC10 registers
          QpiDfxLckCtl.Data = QpiReadPciCfg(host, Socket,  LinkIndex, QPI_DFX_LCK_CTL_CFG_QPIAGENT_LL_REG);
          QpiDfxLckCtl.Bits.privdbg_phy_l0_sync_lck  = 0;
          QpiWritePciCfg(host, Socket,  LinkIndex, QPI_DFX_LCK_CTL_CFG_QPIAGENT_LL_REG, QpiDfxLckCtl.Data);

          QpiPhprivc10.Data = QpiReadPciCfg(host, Socket,  LinkIndex, QPIPHPRIVC10_QPIAGENT_DFX_PHY_REG);
          QpiPhprivc10.Bits.ignore_fast_oc_done  = 0;
          QpiWritePciCfg(host, Socket,  LinkIndex, QPIPHPRIVC10_QPIAGENT_DFX_PHY_REG, QpiPhprivc10.Data);

        } // if link valid
      } // for link loop
    } // if Socket valid
  } // for Socket loop

  return QPI_SUCCESS;
}


QPI_STATUS QpiPmWa (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  )
/*++

Routine Description:
  HSD 4677771/HSD 4969111/HSD 4986182: Disable IO/QPI clock gating
   
Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Socket specific data structure
  QpiInternalGlobal - QPIRC internal variables.
   
Returns:
  QPI_SUCCESS

--*/
{
  SAPMCTL_PCU_FUN1_HSX_BDX_STRUCT  SapmCtl; 
  UINT8 Socket;

  // This WA is NOT required for HSX-EX-/BDX-EX- as the EX- power management code is applying the fix,
  // thus return and do nothing for EX- SKU
  if ((host->var.common.cpuType == CPU_HSX && host->var.common.cpuSubType == CPU_HSX_EX) || 
      (host->var.common.cpuType == CPU_BDX && host->var.common.cpuSubType == CPU_BDX_EX)) {
    return QPI_SUCCESS;
  }
  
  for (Socket=0; Socket < MAX_CPU_SOCKETS; Socket++) {
    if (SocketData->Cpu[Socket].Valid == TRUE) {
      SapmCtl.Data = QpiReadPciCfg (host, Socket, 0, SAPMCTL_PCU_FUN1_REG);
      SapmCtl.Bits.iio_pkgc_clock_gate_disable = 1;
      SapmCtl.Bits.qpi01_pkgc_clock_gate_disable = 1;
      SapmCtl.Bits.qpi2_pkgc_clock_gate_disable = 1;
      QpiWritePciCfg (host, Socket, 0, SAPMCTL_PCU_FUN1_REG, SapmCtl.Data);
    }
  }
  
  return (QPI_SUCCESS);
}


QPI_STATUS
QpiPhyLinkAfterWarmReset (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  )
/*++

Routine Description:

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Socket specific data structure
  QpiInternalGlobal - QPIRC internal variables.
   
Returns:

--*/
{
  QPIMISCSTAT_QPIAGENT_LL_STRUCT  QpiMiscSts;
  QPILCL_QPIAGENT_LL_STRUCT       QpiLcl;
  UINT8                           Index, LinkIndex, LinkSpeed;
  BOOLEAN                         PerLinkSpeed;
  UINT16                          QpiLkDfxCtl;
  CC_MISC_CTL_1_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT  CcMiscCtl1Hsx; 

  //
  // Walk all unused ports again and force disabled/low power mode.   This is done here
  // to handle cases where topology changed due to link failure on FS transition.  No need to call for 
  // reset as higher level code handling the topology change will call for the reset.
  //
  DisableForceToLowPowerAllUnusedLinks(host, SocketData, QpiInternalGlobal);

  //
  // If EXA and hot plug enabled, then skip "clock gating"
  //
  if (QpiInternalGlobal->QpiCpuSktHotPlugEn == FALSE) {
      // IVT WA (s4677870/be4951234, s3618079/s4032821,---):  Silicon Workaround: need to disable QPI clock gating on disabled links for IVT
      // In case it becomes needed again in future, then it needs to be moved to the cold reset phase because of late action bits as well as sticky bits.
      // if (host->var.common.cpuType == CPU_IVT) 
      // EnableClockGatingToAllUnusedLinks(host, SocketData, QpiInternalGlobal);
  } 

  // FOR HSX EP or EX     
  //s4167037: CLONE from HSX: Clone from HSX Sighting:QPI EP: Rx AGC and Rx Vref train low on lanes 9&10 after BIOS boot
  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    if (SocketData->Cpu[Index].Valid == TRUE) {
      for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {        
        // Always do this regardless of link enabled
        if (LinkIndex != 1) {
           // unlock DFX register 
           QpiLkDfxCtl = (UINT16) QpiReadPciCfg (host, Index, LinkIndex, QPILKDFXCTL_QPIAGENT_DFX_COMMON_REG);
           QpiLkDfxCtl &= (~BIT7); // clear iodfxlck (checking bit without structure)
           QpiWritePciCfg (host, Index, LinkIndex, QPILKDFXCTL_QPIAGENT_DFX_COMMON_REG, (UINT32) QpiLkDfxCtl);

           CcMiscCtl1Hsx.Data = QpiReadPciCfg (host, Index, LinkIndex, CC_MISC_CTL_1_QPIAGENT_DFX_COMMON_REG);
           CcMiscCtl1Hsx.Bits.icomp_track = 1;    
           QpiWritePciCfg (host, Index, LinkIndex, CC_MISC_CTL_1_QPIAGENT_DFX_COMMON_REG, CcMiscCtl1Hsx.Data);
        }
      }
    }
  }

  //
  // If get here we are booting, so display QPI Freq
  //
  if (QpiInternalGlobal->TotCpu != 1) {
    // 
    // We are in warm reset path; Log a warning if the links are in slow mode.
    // 
    if (host->var.qpi.QpiCurrentLinkSpeedMode == QPI_LINK_SLOW_SPEED_MODE) {
      QpiLogWarning(host, 0xFF, 0xFF, 0xFF, WARN_LINK_SLOW_SPEED_MODE);
      QpiDebugPrintWarn((host, QPI_DEBUG_WARN, "\n; WARNING: System is operating in QPI Link Slow Speed Mode. "));
    } else {
      //
      // Identify the per link speed
      //
      LinkSpeed = 0xFF;
      PerLinkSpeed = FALSE;
      for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
        if (SocketData->Cpu[Index].Valid == TRUE) {
          for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {
            if (SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE){               
              // Display the link's fast-mode speed.
              QpiMiscSts.Data = QpiReadPciCfg (host, Index, (UINT8)((LinkIndex/2)*2), QPIMISCSTAT_QPIAGENT_LL_REG); // Only ports 0 & 2 have this register
              host->var.qpi.OutPerLinkSpeed[Index][LinkIndex] = (UINT8) (QpiMiscSts.Bits.qpi_rate - 2);
              QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Socket %u QPI Link %u Freq is currently %u.", Index, LinkIndex, ((QpiMiscSts.Bits.qpi_rate+5)*8)));
          
              // Keep track of the flag to indicate if two links have different speed (which implies per link speed)
              if (LinkSpeed == 0xFF) {
                LinkSpeed = host->var.qpi.OutPerLinkSpeed[Index][LinkIndex];
              } else if (LinkSpeed != host->var.qpi.OutPerLinkSpeed[Index][LinkIndex]){
                PerLinkSpeed = TRUE;
              }
            }
          }
        }        
      }

      //
      // Identify the system wide speed
      //
      host->var.qpi.OutQpiLinkSpeed = MAX_QPI_LINK_SPEED;
      if (PerLinkSpeed == TRUE) {
        host->var.qpi.OutQpiLinkSpeed = FREQ_PER_LINK;
        QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;  Per Link speed is used."));
      } else {
        //
        // QpiMiscstat is nonfunctional in A0.   
        //
        if (((host->var.common.cpuType == CPU_HSX) && (host->var.common.stepping >= B0_REV_HSX)) ||
             (host->var.common.cpuType == CPU_BDX)) {
          QPI_ASSERT ((LinkSpeed != 0xFF), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
        }
        host->var.qpi.OutQpiLinkSpeed = LinkSpeed;
        QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;  System wide QPI Freq is currently %u.", ((host->var.qpi.OutQpiLinkSpeed+7)*8)));
      }
    }

    //
    // Identify the current L0p/L1/CrcMode settings.These settings are system wide, so just read it for only one valid link.
    // If there are no valid links, it is a single socekt system; nothing to update in that case.
    //
    for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {    
      if (SocketData->Cpu[Index].Valid == TRUE) {       
        for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {
          if (SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE){               
            QpiLcl.Data = QpiReadPciCfg (host, Index, LinkIndex, QPILCL_QPIAGENT_LL_REG);
            host->var.qpi.OutQpiLinkL0pEn = (UINT8) QpiLcl.Bits.l0p_enable_mode_at_transmitter;
            host->var.qpi.OutQpiLinkL1En = (UINT8) QpiLcl.Bits.l1_enable;
            host->var.qpi.OutQpiCrcMode = (UINT8) QpiLcl.Bits.crc_mode;
            break;
          }
        }
      }
    }

  }
  return QPI_SUCCESS;
}

QPI_STATUS
SendMailBoxCmdSetQpiFreq (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  )
{

  UINT8  Index, LinkIndex;
  UINT32 LinkSpeed=0;

  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    if (SocketData->Cpu[Index].Valid == TRUE) {
      for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {
        if ((SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) || (QpiInternalGlobal->QpiCpuSktHotPlugEn == TRUE)) {
          LinkSpeed = ConvertSpeedToFreq(host, QpiInternalGlobal, host->setup.qpi.PhyLinkPerPortSetting[Index].Phy[LinkIndex].QpiLinkSpeed);
          if (LinkIndex < SI_MAX_QPI_PORTS - 1) {
            // Send common command/link speed for ports 0/1
            SendMailBoxCmdToPcode(host, Index, MAILBOX_BIOS_CMD_SET_QPI_FREQ, LinkSpeed);
            LinkIndex = 1;
          } else {
            // Send separate command/link speed for port 2
            SendMailBoxCmdToPcode(host, Index, MAILBOX_BIOS_CMD_SET_QPI_FREQ |(0x2 << 8), LinkSpeed);
          }//Link-0/1 vs. Link-2 
        }//Valid link?
      } // for LinkIndex loop
    }   // valid socket?
  } // for socket

  
  return QPI_SUCCESS;
}

STATIC
QPI_STATUS
ProgramLateAction (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  UINT32              Value
  )
{
  QPIREUT_PH_CTR_QPIAGENT_REUT_HSX_BDX_STRUCT  QpiReutPhCtr;

  UINT8 Index;
  UINT8 LinkIndex;
       
  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
         
    if (SocketData->Cpu[Index].Valid == TRUE) {
         
      for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {
         
        //
        // Program lateactionloaddisable bit
        //
        if ((SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) || (host->setup.qpi.QpiLbEn)) {
          QpiReutPhCtr.Data = QpiReadPciCfg (host, Index, LinkIndex, QPIREUT_PH_CTR_QPIAGENT_REUT_REG);
          QpiReutPhCtr.Bits.lateactloaddis = Value;
          QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Socket %u Link %u : QPIREUT_PH_CTR write 0x%x", Index, LinkIndex, QpiReutPhCtr.Data));
          QpiWritePciCfg (host, Index, LinkIndex, QPIREUT_PH_CTR_QPIAGENT_REUT_REG, QpiReutPhCtr.Data);

        }
      }
    }
  }

  return QPI_SUCCESS;
}

QPI_STATUS
ConfigSocketLinkPhyMisc_0 (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  UINT8               Socket,
  UINT8               LinkIndex,
  BOOLEAN             ProbeInstalled
  )
/*++

Routine Description:
   Programs the values from the EPARAMS entry for specified link

Arguments:
   host  - Pointer to the system host (root) structure
   SocketData - Socket specific data structure
   Socket - SocId we are looking up
   Link - Link# on socket
   ProbeInstalled -  TRUE   Probe installed
                     FALSE  No probe installed 
Returns:

--*/
{
  CC_ICOMP_QPIAGENT_DFX_COMMON_STRUCT                   CcIcomp;
  CC_PWR_QPIAGENT_DFX_COMMON_STRUCT                     CcPwr;
  CC_MISC_CTL_1_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT      CcMiscCtl1; 
  CC_TXDCC_FWDC_QPIAGENT_DFX_COMMON_STRUCT              CcTxdccFwdc;
  CC_RX_CTL_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT          CcRxCtlDfx;

  CcIcomp.Data = QpiReadPciCfg (host, Socket, LinkIndex, CC_ICOMP_QPIAGENT_DFX_COMMON_REG);
  CcIcomp.Bits.tx_icomp_gain_sel = 1;
  QpiWritePciCfg (host, Socket, LinkIndex, CC_ICOMP_QPIAGENT_DFX_COMMON_REG, CcIcomp.Data);

  CcMiscCtl1.Data = QpiReadPciCfg (host, Socket, LinkIndex, CC_MISC_CTL_1_QPIAGENT_DFX_COMMON_REG);
  CcMiscCtl1.Bits.icomp_track = 1;    
  QpiWritePciCfg (host, Socket, LinkIndex, CC_MISC_CTL_1_QPIAGENT_DFX_COMMON_REG, CcMiscCtl1.Data);

  CcPwr.Data = QpiReadPciCfg (host, Socket, LinkIndex, CC_PWR_QPIAGENT_DFX_COMMON_REG);
  CcPwr.Bits.tx_pwr_cfg = 0x20;    //HSX HSD-4905443
  QpiWritePciCfg (host, Socket, LinkIndex, CC_PWR_QPIAGENT_DFX_COMMON_REG, CcPwr.Data);
  
  // HSD 4988292, 4970528 : UniPhy recipe for BDX-EP and BDX-EX (v 1.0)
  if (host->var.common.cpuType == CPU_BDX) {
    CcTxdccFwdc.Data = QpiReadPciCfg (host, Socket, LinkIndex, CC_TXDCC_FWDC_QPIAGENT_DFX_COMMON_REG);
    CcTxdccFwdc.Bits.txdcc_gainsel = 5;
    CcTxdccFwdc.Bits.sel_vcm_cml2cmos = 2;
    QpiWritePciCfg (host, Socket, LinkIndex, CC_TXDCC_FWDC_QPIAGENT_DFX_COMMON_REG, CcTxdccFwdc.Data);

    CcRxCtlDfx.Data = QpiReadPciCfg (host, Socket, LinkIndex, CC_RX_CTL_QPIAGENT_DFX_COMMON_REG);
    CcRxCtlDfx.Bits.rx_ctle_ctl = 2;
    QpiWritePciCfg (host, Socket, LinkIndex, CC_RX_CTL_QPIAGENT_DFX_COMMON_REG, CcRxCtlDfx.Data);
  }

  return QPI_SUCCESS;
}


QPI_STATUS
UniphySettingHsxAndBdx(
  struct sysHost            *host,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal,  
  UINT8                     Socket,
  UINT8                     LinkIndex
  )
/*++

Routine Description:
   Programs the values from the EPARAMS entry for specified link

Arguments:
   host  - Pointer to the system host (root) structure
   SocketData - Socket specific data structure
   Socket - SocId we are looking up
   Link - Link# on socket
   Qpi11FeatureDisable -  TRUE   Probe installed/need disable feature
                          FALSE  No probe installed that requires feature disable
Returns:

--*/
{ 
  UINT8        UnitId, i, Data8, *regPtr, regIndex, regMask, regValue, IndexNum;
  QPI_DFX_LCK_CTL_CFG_QPIAGENT_LL_STRUCT QpiDfxLckCtl;
  QPIPHPRIVPTV2_QPIAGENT_DFX_PHY_HSX_BDX_STRUCT QpiPhprivptv2;    // QpiPhprivptv2 is used for HSX-EP/EN only
  QPIPHPRIVC10_QPIAGENT_DFX_PHY_HSX_BDX_STRUCT  QpiPhprivc10;
  QPIPHPRIVPTV_QPIAGENT_DFX_PHY_HSX_BDX_STRUCT  QpiPhprivptv;
  UINT32 capid0;

  // IOVB Table for HSX
  // Note - The following are the 'common' values for HSX.  Custom values specific
  //        to certain steppings and/or speeds are applied below.
  UINT8  IOVB_REG_TABLE_HSX[]= {                        //mask      value 
    IOVB_CDR3_0_0_QPIAGENT_DFX_PHY_IOVB_REG,            0x3F,      (2 << 6),        //cdr_acq_length      - Bits[7:6] = 2
    IOVB_PG1_0_0_QPIAGENT_DFX_PHY_IOVB_REG,             0xC0,         6,            //pg_track            - Bits[5:0] = 6  
    IOVB_IG0_0_0_QPIAGENT_DFX_PHY_IOVB_REG,             0xC7,         0,            //ig_track            - Bits[5:3] = 0
    IOVB_IG0_0_0_QPIAGENT_DFX_PHY_IOVB_REG,             0xF8,         0,            //ig_acq              - Bits[2:0] = 0,

    IOVB_AFE_CTL0_0_0_QPIAGENT_DFX_PHY_IOVB_REG,        0x3F,      (1 << 6),        //psa_cfg             - Bits[7:6] = 1   (version 1.0)
    IOVB_AGC1_0_0_QPIAGENT_DFX_PHY_IOVB_REG,            0xCF,      (2 << 4),        //agc_acq_length      - Bits[5:4] = 2
    IOVB_AGC1_0_0_QPIAGENT_DFX_PHY_IOVB_REG,            0xFC,         2,            //agc_gain_acq        - Bits[1:0] = 3   (version 1.0)
    IOVB_AGC0_0_0_QPIAGENT_DFX_PHY_IOVB_REG,            0x7F,         0,            //rx_agc_use_err_data - Bits[7]   = 0   (version 1.0)
    IOVB_RX_VREF0_0_0_QPIAGENT_DFX_PHY_IOVB_REG,        0xF0,         7,            //rx_vref_sel         - Bits[3:0] = 7 
    IOVB_DFE2_0_0_QPIAGENT_DFX_PHY_IOVB_REG,            0xFC,         2,            //rx_dfesum_mfc_10gen - Bits[1:0] = 2
    IOVB_RX_VREF0_0_0_QPIAGENT_DFX_PHY_IOVB_REG,        0x9F,      (3 << 5),        //rx_vcm_ds_sel       - Bits[6:5] = 3 
    IOVB_RX_VREF0_0_0_QPIAGENT_DFX_PHY_IOVB_REG,        0xEF,         0,            //rx_vcm_sum_sel      - Bits[4:4] = 0 
    IOVB_SPARE0_0_0_QPIAGENT_DFX_PHY_IOVB_REG,          0x00,         32,           //spare               - Bits[7:0] - 32
    IOVB_IREF_OVR_0_0_QPIAGENT_DFX_PHY_IOVB_REG,        0xFE,         1,            //ism_iref_ovr_en     - Bits[0:0] = 1
      
    IOVB_RX_CTLE2_0_0_QPIAGENT_DFX_PHY_IOVB_REG,        0xF0,         4,            //ctle_rdeg_fullspeed - Bits[3:0] = 6   (ToDo: check with EV)
    IOVB_RX_CTLE2_0_0_QPIAGENT_DFX_PHY_IOVB_REG,        0x0F,      (3 << 4),        //ctle_rdeg_halfspeed - Bits[7:4] = 3   
    IOVB_RX_CTL0_0_0_QPIAGENT_DFX_PHY_IOVB_REG,         0X3F,      (3 << 6),        //rx_dfe_summer_ctl   - Bits[7:6] = 3     
    IOVB_TX_IREF_0_0_QPIAGENT_DFX_PHY_IOVB_REG,         0xE0,         0,            //tx_iref_ctl         - Bits[4:0] = 0   (s4166053)     
    //IOVB_RX_CTLE1_0_0_QPIAGENT_DFX_PHY_IOVB_REG,        0xE0,        11,          //rx_ctle_peak_halfspeed - Bits[4:0] = 11 (version 0.6)
  };

  // IOVB Table for BDX
  // Note - The following are the 'common' values for BDX.  Custom values specific
  //        to certain steppings and/or speeds are applied below.  
  UINT8  IOVB_REG_TABLE_BDX[]= {                        //mask      value
    IOVB_CDR3_0_0_QPIAGENT_DFX_PHY_IOVB_REG,            0x3F,      (2 << 6),        //cdr_acq_length      - Bits[7:6] = 2
    IOVB_PG1_0_0_QPIAGENT_DFX_PHY_IOVB_REG,             0xC0,         6,            //pg_track            - Bits[5:0] = 6
    IOVB_PG0_0_0_QPIAGENT_DFX_PHY_IOVB_REG,             0xC0,        16,            //pg_acq              - Bits[5:0] = 16 
    IOVB_IG0_0_0_QPIAGENT_DFX_PHY_IOVB_REG,             0xC7,      (0 << 3),        //ig_track            - Bits[5:3] = 0
    IOVB_IG0_0_0_QPIAGENT_DFX_PHY_IOVB_REG,             0xF8,         0,            //ig_acq              - Bits[2:0] = 0
    IOVB_AGC1_0_0_QPIAGENT_DFX_PHY_IOVB_REG,            0xCF,      (2 << 4),        //agc_acq_length      - Bits[5:4] = 2
    IOVB_AGC1_0_0_QPIAGENT_DFX_PHY_IOVB_REG,            0xFC,         2,            //agc_gain_acq        - Bits[1:0] = 3
    IOVB_DFE2_0_0_QPIAGENT_DFX_PHY_IOVB_REG,            0xFC,         2,            //rx_dfesum_mfc_10gen - Bits[1:0] = 2
    IOVB_RX_VREF0_0_0_QPIAGENT_DFX_PHY_IOVB_REG,        0x7F,      (0 << 7),        //rx_vcm_sum_sel_extnd  Bits[7:7] = 0
    IOVB_RX_CTL1_0_0_QPIAGENT_DFX_PHY_IOVB_REG,         0xFE,         0,            //rxsq_en               Bits[0:0] = 0
    IOVB_RX_CTLE2_0_0_QPIAGENT_DFX_PHY_IOVB_REG,        0xF0,         5,            //ctle_rdeg_fullspeed - Bits[3:0] = 5
    IOVB_RX_CTLE2_0_0_QPIAGENT_DFX_PHY_IOVB_REG,        0x0F,      (5 << 4),        //ctle_rdeg_halfspeed - Bits[7:4] = 5
    IOVB_DFE_BLEED0_0_0_QPIAGENT_DFX_PHY_IOVB_REG,      0xCF,      (1 << 4),        //gain_boost          - Bits[5:4] = 1
    IOVB_DFE_BLEED0_0_0_QPIAGENT_DFX_PHY_IOVB_REG,      0xBF,      (1 << 6),        //ratesel_en_cap      - Bits[6:6] = 1
    IOVB_DFE_BLEED1_0_0_QPIAGENT_DFX_PHY_IOVB_REG,      0xF7,      (1 << 3),        //rload_slowck        - Bits[3:3] = 1
    IOVB_SPARE0_0_0_QPIAGENT_DFX_PHY_IOVB_REG,          0xFF,        32,            //spare0              - Bits[7:0] = 32
    IOVB_TXEQ_HALF_LVL1_0_0_QPIAGENT_DFX_PHY_IOVB_REG,  0xC0,        57,            //txeq_half_lvl1      - Bits[5:0] = 57
    IOVB_TXEQ_HALF_LVL2_0_0_QPIAGENT_DFX_PHY_IOVB_REG,  0xC0,        50,            //txeq_half_lvl2      - Bits[5:0] = 50
    IOVB_TXEQ_HALF_LVL3_0_0_QPIAGENT_DFX_PHY_IOVB_REG,  0xC0,        44,            //txeq_half_lvl3      - Bits[5:0] = 44
    IOVB_RX_CTL0_0_0_QPIAGENT_DFX_PHY_IOVB_REG,         0x3F,      (3 << 6),        //rx_dfe_summer_ctl   - Bits[7:6] = 3
    IOVB_TX_IREF_0_0_QPIAGENT_DFX_PHY_IOVB_REG,         0xE0,        28,            //tx_iref_ctl         - Bits[4:0] = 28
  };

  // IOVC Declarations
  IOVC_LCPKAMP_CFG1_QPIAGENT_DFX_PHY_IOVC_STRUCT     IovcLcpkamp;

  // IOVC Table for HSX
  // Note - The following are the 'common' values for HSX.  Custom values specific
  //        to certain steppings and/or speeds are applied below.  
  UINT8  IOVC_REG_TABLE_HSX[]= {                        //mask      value    
    IOVC_SPARE0_QPIAGENT_DFX_PHY_IOVC_REG,              0x00,        32,             //spare             - Bits[7:0] - 32  (version 1.0)
    IOVC_IREF_OVR_QPIAGENT_DFX_PHY_IOVC_REG,            0xFE,         1,             //ism_iref_ovr_en   - Bits[0:0] = 1; 
    IOVC_DLL_CTL2_QPIAGENT_DFX_PHY_IOVC_REG,            0xF0,         9,             //fwdc_dllvsetctl   - Bits[3:0] = 9;     
    IOVC_TXDCC_CALSM_CTL0_QPIAGENT_DFX_PHY_IOVC_REG,    0xF8,         6,             //wsel              - Bits[2:0] = 6;   
    IOVC_DLL_CALSM_CTL0_QPIAGENT_DFX_PHY_IOVC_REG,      0xF8,         6,             //wsel              - Bits[2:0] = 6;   
    IOVC_TX_CTL2_QPIAGENT_DFX_PHY_IOVC_REG,             0xC3,         32,            //fwdc_tx_spare     - Bits[5:2] = 8;
  };

  // IOVC Table for BDX
  // Note - The following are the 'common' values for BDX.  Custom values specific
  //        to certain steppings and/or speeds are applied below.  
  UINT8  IOVC_REG_TABLE_BDX[]= {                        //mask      value
    IOVC_BIAS0_QPIAGENT_DFX_PHY_IOVC_REG,               0xE0,        30,             //fwdctx_vref_sel   - Bits[4:0] = 30
    IOVC_DLL_CTL2_QPIAGENT_DFX_PHY_IOVC_REG,            0xF0,         8,             //fwdc_dllvsetctl   - Bits[3:0] = 8
    IOVC_DLL_CTL2_QPIAGENT_DFX_PHY_IOVC_REG,            0x0F,         4,             //fwdc_dllcpctl     - Bits[7:4] = 4
    IOVC_DLL_CTL0_QPIAGENT_DFX_PHY_IOVC_REG,            0xFF,         7,             //fwdc_dllctl       - Bits[7:0] = 7
    IOVC_DLL_CALSM_CTL0_QPIAGENT_DFX_PHY_IOVC_REG,      0xF8,         6,             //wsel              - Bits[2:0] = 6
  };

  capid0 = QpiReadPciCfg (host, Socket, 0, CAPID0_PCU_FUN3_REG);

  // Program for IOVB
  //    For port 0/2, we only do swizzle for signals between QPIPH/QIO and QPIPH/VENEER. 
  //    For Veneer signals to/from QIO are not swizzle, since they are either IAR control to qio or status from QIO to IAR. 
  //    To handle the swizzle for IAR SW, QPIPH IAR controller will do the address swizzle. 
  //    For example, if SW want to write/read port 0 physical lane 0 IAR. 
  //    SW will write Unit ID 0 to perform the read write.
  //    The IAR controller will automatic change the Unit ID from 0 to Unit ID 1 
  //    so the write/read will target to veneer bundle 0, L1 instead of bundle 0, L0.

  if (host->var.common.cpuType == CPU_HSX) {
    IndexNum  = sizeof(IOVB_REG_TABLE_HSX) /3;
    regPtr = IOVB_REG_TABLE_HSX;
  } else {
    IndexNum  = sizeof(IOVB_REG_TABLE_BDX) /3;
    regPtr = IOVB_REG_TABLE_BDX; 
  } // cpuType checks

  for(i = 0; i < IndexNum; i++){
    regIndex = *regPtr++; 
    regMask = *regPtr++;
    regValue = *regPtr++;

    if( LinkIndex == 1) {
      UnitId = 0;     // for port 1
    } else {
      UnitId = 1;     // for port 0 or 2
    }
      
    //
    // 4168554: Apply uniphy 6.00 if dcu_mode == 0 because it's concurrent change
    // w/ peak adder fuse change required for uniphy 6.00
    //
    if ((host->var.common.cpuType == CPU_HSX) &&
        ((capid0 & BIT30)  == 0)) { // dcu_mode (checking bit without structure)

      if (regMask == 0xF0 && regIndex == IOVB_RX_CTLE2_0_0_QPIAGENT_DFX_PHY_IOVB_REG) {
        regValue = 5;
      }
      
      if (regMask == 0x0F && regIndex == IOVB_RX_CTLE2_0_0_QPIAGENT_DFX_PHY_IOVB_REG) {
        regValue = (5 << 4);
      }
    }

    // Apply W/As specific to link-speed
    switch (host->setup.qpi.PhyLinkPerPortSetting[Socket].Phy[LinkIndex].QpiLinkSpeed) {
      case SPEED_REC_96GT:
        // HSD-4970191: HSX Clone : HSX EX QPI slightly failing UPM at 9.6GT/s
        if (regIndex == IOVB_AFE_CTL0_0_0_QPIAGENT_DFX_PHY_IOVB_REG) {
          if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType == CPU_HSX_EX)) {
            regValue = (2 << 6);  //psa_cfg (Bits[7:6]) for 9.6GT/s only
          } // cpuType check
        } // regIndex check
        break;
      default:
        // Default value already set from table above.
        break;
    }

    Data8 = QpiReadIar (host, Socket, LinkIndex, UnitId, regIndex);     
    Data8 = ((Data8 & regMask) | regValue);
    QpiWriteIar (host, Socket, LinkIndex, QPILIB_IAR_BCAST, regIndex, Data8);
  }
  
  // Program for IOVC
  if (host->var.common.cpuType == CPU_HSX) {
    IndexNum  = sizeof(IOVC_REG_TABLE_HSX) /3;
    regPtr = IOVC_REG_TABLE_HSX;
  } else {
    IndexNum  = sizeof(IOVC_REG_TABLE_BDX) /3;
    regPtr = IOVC_REG_TABLE_BDX; 
  } // cpuType checks

  for(i = 0; i < IndexNum; i++){
    regIndex = *regPtr++; 
    regMask = *regPtr++;
    regValue = *regPtr++;  
    
    //Apply stepping-based W/A's
    if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.stepping < B0_REV_HSX)) {
      if (regIndex == IOVC_TXDCC_CALSM_CTL0_QPIAGENT_DFX_PHY_IOVC_REG) {
        regValue = 5;
      }
    }

    //(HSD-4969795):HSX Clone : QPI: fast retrain loops using reset or L1 entry/exit causes lane drop and EP Init Aborts / EX DFO
    //enables SI fix instead of pcode fix
    if (regIndex == IOVC_TX_CTL2_QPIAGENT_DFX_PHY_IOVC_REG) {
      if (((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType == CPU_HSX_EX) && (host->var.common.stepping >= D0_REV_HSX)) || 
          ((host->var.common.cpuType == CPU_BDX) && (host->var.common.cpuSubType == CPU_BDX_EX)) &&
          (QpiInternalGlobal->LaneDropPcodeFixEn == FALSE)) {
         // Do nothing - this will program the IOVC_TX_CTL2 values below using QpiReadIar()/QpiWriteIar()
       } else {
         // Skip to next iteration of loop - Don't program the values
         continue;
       } // cpuType check
    }

    // Program current IOVC reg with desired value.
    Data8 = QpiReadIar (host, Socket, LinkIndex, QPILIB_IAR_IOVC, regIndex);     
    Data8 = ((Data8 & regMask) | regValue);
    QpiWriteIar (host, Socket, LinkIndex, QPILIB_IAR_IOVC, regIndex, Data8);
  } // for loop

  //
  // Need to modify FWDC_LCAMPCAPCTL based on freq (Capacitance control for QPI LCAMP)
  //
  IovcLcpkamp.Data = QpiReadIar (host, Socket, LinkIndex, QPILIB_IAR_IOVC, IOVC_LCPKAMP_CFG1_QPIAGENT_DFX_PHY_IOVC_REG);
  //(s5003442)
  if ((host->var.common.cpuType == CPU_HSX) ||
      ((host->var.common.cpuType == CPU_BDX) && (host->var.common.cpuSubType == CPU_BDX_EX))) {
    IovcLcpkamp.Bits.fwdc_lcampen = 0;  //Disable LC Peak Amplifier (LCbypass).   HSD-4986848,HSD-4969272,HSD-4969509.
  }
  switch (host->setup.qpi.PhyLinkPerPortSetting[Socket].Phy[LinkIndex].QpiLinkSpeed) {
    case SPEED_REC_96GT:
      if (host->var.common.cpuType == CPU_HSX) {
        IovcLcpkamp.Bits.fwdc_lcampcapctl = 2;
      } else { // For CPU_BDX
        IovcLcpkamp.Bits.fwdc_lcampcapctl = 3;
      }
      if (((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType == CPU_HSX_EX)) ||
          ((host->var.common.cpuType == CPU_BDX) && (host->var.common.cpuSubType == CPU_BDX_EX))) {
        IovcLcpkamp.Bits.fwdc_lcampen = 1;  //For 9.6GT EX, always enable LC Peak Amplifier (LCbypass).   HSD-4969961.
      }
      break;

    case SPEED_REC_80GT:
      if (host->var.common.cpuType == CPU_HSX) {
        IovcLcpkamp.Bits.fwdc_lcampcapctl = 10;
      } else { // For CPU_BDX
        IovcLcpkamp.Bits.fwdc_lcampcapctl = 11;
      }
      break;

    case SPEED_REC_72GT:
      if (host->var.common.cpuType == CPU_HSX) {
        IovcLcpkamp.Bits.fwdc_lcampcapctl = 16;
      } else { // For CPU_BDX
        IovcLcpkamp.Bits.fwdc_lcampcapctl = 17;
      }
      break;

    case SPEED_REC_64GT:
    default:
      if (host->var.common.cpuType == CPU_HSX) {
        IovcLcpkamp.Bits.fwdc_lcampcapctl = 24;
      } else { // For CPU_BDX
        IovcLcpkamp.Bits.fwdc_lcampcapctl = 23;
      }
      break;

    }
  QpiWriteIar (host, Socket, LinkIndex, QPILIB_IAR_IOVC, IOVC_LCPKAMP_CFG1_QPIAGENT_DFX_PHY_IOVC_REG, IovcLcpkamp.Data);


  // HSX W/A (Permanent) (4166445):  QPI link calibration timers:  etoc = 14 & etdcc =12  (Applied for Grantley [HSX-EP/EN] only)
  if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType != CPU_HSX_EX)) {
    //must first set this field to zero before attempting to write the QPIPHPRIVC10 registers
    QpiDfxLckCtl.Data = QpiReadPciCfg(host, Socket,  LinkIndex, QPI_DFX_LCK_CTL_CFG_QPIAGENT_LL_REG);
    QpiDfxLckCtl.Bits.privdbg_phy_l0_sync_lck  = 0;
    QpiWritePciCfg(host, Socket,  LinkIndex, QPI_DFX_LCK_CTL_CFG_QPIAGENT_LL_REG, QpiDfxLckCtl.Data);

    QpiPhprivptv.Data = QpiReadPciCfg(host, Socket,  LinkIndex, QPIPHPRIVPTV_QPIAGENT_DFX_PHY_REG);
    QpiPhprivptv.Bits.etoc = 14;
    QpiWritePciCfg(host, Socket,  LinkIndex, QPIPHPRIVPTV_QPIAGENT_DFX_PHY_REG, QpiPhprivptv.Data);

    QpiPhprivptv2.Data = QpiReadPciCfg(host, Socket,  LinkIndex, QPIPHPRIVPTV2_QPIAGENT_DFX_PHY_REG);
    QpiPhprivptv2.Bits.etdcc = 12;
    QpiWritePciCfg(host, Socket,  LinkIndex, QPIPHPRIVPTV2_QPIAGENT_DFX_PHY_REG, QpiPhprivptv2.Data);

    QpiPhprivc10.Data = QpiReadPciCfg(host, Socket,  LinkIndex, QPIPHPRIVC10_QPIAGENT_DFX_PHY_REG);
    QpiPhprivc10.Bits.ignore_slow_dcc_done  = 0;
    QpiPhprivc10.Bits.ignore_fast_dcc_done  = 0;
    QpiWritePciCfg(host, Socket,  LinkIndex, QPIPHPRIVC10_QPIAGENT_DFX_PHY_REG, QpiPhprivc10.Data);
  } 

  return QPI_SUCCESS;
}            


QPI_STATUS
ConfigSocketLinkPhyMisc (
  struct sysHost            *host,
  QPI_SOCKET_DATA           *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal,
  UINT8                     Socket,
  UINT8                     LinkIndex,
  BOOLEAN                   Qpi11FeatureDisable
  )
/*++

Routine Description:
   Programs the values from the EPARAMS entry for specified link

Arguments:
   host  - Pointer to the system host (root) structure
   SocketData - Socket specific data structure
   Socket - SocId we are looking up
   Link - Link# on socket
   Qpi11FeatureDisable -  TRUE   Probe installed/need disable feature
                          FALSE  No probe installed that requires feature disable
Returns:

--*/
{
  QPIREUT_PH_CPR_QPIAGENT_REUT_HSX_BDX_STRUCT  QpiReutPhCpr;
  QPIREUT_PH_CTR_QPIAGENT_REUT_HSX_BDX_STRUCT  QpiReutPhCtr;
  QPIREUT_PH_STV_QPIAGENT_REUT_HSX_BDX_STRUCT  QpiReutPhStv;
  QPIREUT_PH_WCI_QPIAGENT_REUT_STRUCT          QpiReutPhWci;
  QPIREUT_PH_PTV_QPIAGENT_REUT_STRUCT          QpiReutPhPtv;
  LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_STRUCT  LbcPerIouControl;
  LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_STRUCT     LbcPerIouData;
  UINT32  Data32; 

  // Get capabilities & control registers
  QpiReutPhCpr.Data = QpiReadPciCfg (host, Socket, LinkIndex, QPIREUT_PH_CPR_QPIAGENT_REUT_REG);
  QpiReutPhCtr.Data = QpiReadPciCfg (host, Socket, LinkIndex, QPIREUT_PH_CTR_QPIAGENT_REUT_REG);
  QpiReutPhPtv.Data = QpiReadPciCfg (host, Socket, LinkIndex, QPIREUT_PH_PTV_QPIAGENT_REUT_REG);

  // enable fast mode
  QpiReutPhCtr.Bits.link_speed = 1;

  //
  // Setup PHWCI (Link Width Control) if user chooses Auto Setup then don't program PHWCI so the default set bits remain.
  //
  Data32 = host->setup.qpi.PhyLinkPerPortSetting[Socket].Phy[LinkIndex].QpiConfigTxWci;
  QpiReutPhWci.Data = QpiReadPciCfg (host, Socket, LinkIndex, QPIREUT_PH_WCI_QPIAGENT_REUT_REG);
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Socket %u Link %u :PH_WCI 0x%x as Default", Socket, LinkIndex, QpiReutPhWci.Data));        
  if((Data32 == WIDTH_Q3Q2Q1Q0) || (Data32 == WIDTH_Q1Q0) || (Data32 == WIDTH_Q3Q2)) { 
    //
    // Only support 0x43 -> bit0-> full width, bit1-> (Q1, Q0), bit6-> (Q3, Q2)         
    //
    QpiReutPhWci.Bits.configdtxwci = (1 << Data32); 
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Socket %u Link %u :PH_WCI 0x%x is programmed per Setup", Socket, LinkIndex, QpiReutPhWci.Data));        
    QpiWritePciCfg (host, Socket, LinkIndex, QPIREUT_PH_WCI_QPIAGENT_REUT_REG, QpiReutPhWci.Data);
  } else if(Data32 != MAX_QPI_LINK_WIDTH) {
    //
    //  if not MAX_QPI_LINK_WIDTH that implies "Auto", then it is not supported by silicon.
    //
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Link Width Selection in Setup of QpiConfigTxWci = %u is not supported. Keep Default.", Data32));        
  }

 //s4032661/s4168513: HSX Clone : HSX EX: QPI CFO Observed on Large Number of Ports (Disable CFO)
 if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.stepping < C0_REV_HSX)) {        
    QpiReutPhCtr.Bits.dropbadclkenabled = 0;
 }
   
  //
  // Disableautocompliance
  //
  QpiReutPhCtr.Bits.disableautocompliance = 1;                          

  //
  // Scrambling
  //    
    // Try to enable scrambling
    if (QpiReutPhCpr.Bits.datscramblelfsr) {
      //
      // Scrambling is supported by chip
      // Enable bitlock B pattern, scrambling,
      //
      QpiReutPhCtr.Bits.enablescramblelfsr = 1;
    }

    // Try enable StaggerScrambleL0

      if (Qpi11FeatureDisable == FALSE) {
        //
        // If a debug probe is installed, do not enable these QPI 1.1 features
        //
      
        if (QpiReutPhCpr.Bits.scrmblstaggerl0) {
          //
          // Scrambling supported in L0
          // Enable bitlock B pattern, scrambling,
          //
          QpiReutPhCtr.Bits.enstgrscrmbll0 = 1;
        }
   
        //Try enable StaggerScramble During Init

          if (QpiReutPhCpr.Bits.scrmblstaggerinit) {
            //
            // Scrambling supported during init
            // Enable bitlock B pattern, scrambling,
            //
            QpiReutPhCtr.Bits.enstgrscrmblinit = 1;
          }
        }
  
    if (QpiReutPhCpr.Bits.bitlockretrainpatt) {
      //
      // enablebitlockretrainwithpatt.  Use pattern in bitlock/retraining
      // Enable bitlock B pattern, scrambling,
      //
      QpiReutPhCtr.Bits.enablebitlockretrainwithpatt = 1;

    } 
  


        
  // This is a FULL SPEED transtion
  // Set ResetModifier = 0
  QpiReutPhCtr.Bits.resetmodifier = 0; 


  //
  // Only do this on FULL speed transitions
  // AdaptiveRx
  //
  if (QpiReutPhCpr.Bits.rxadapt) {
    //
    // Supported, so enable 
    //
    QpiReutPhCtr.Bits.enadaptrxeq = 1;


  }

  //
  // Write values to chip
  //
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Socket %u Link %u : QPIREUT_PH_CTR write 0x%x", Socket, LinkIndex, QpiReutPhCtr.Data));
  QpiWritePciCfg (host, Socket, LinkIndex, QPIREUT_PH_CTR_QPIAGENT_REUT_REG, QpiReutPhCtr.Data);
  
  // IVT
  // BIOS sighting 4032681 CLONE from ivytown: QPI Bus Interconnect Error with MCA errors seen during reset testing on CV Thunder Ridge platform
  // BIOS sighting 4032409 CLONE from ivytown: 4 soskcet CPU online is not stable - causing system hung and/or correctable QPI errors
  // Ivytown sighting 4677771, oc_done retry count maxes out. Recommend increasing FwdClk timeout from default value of 0x8 -> 0x9. 
  // This is to allow for oc_done retry attempts. Above finding is related to the CFO timer programming which seems to affect offline/ online.
  //                                
  // HSX
  // QPIREUT_PH_STV.etdetectfwdclk. s4032604/s4168125
  //
  // BDX
  // Grantley Refresh BIOS HSD 4986662 (For both BDX- EP and EX)
  // Remove the WA s4032604/s4168125 WA inline to HSX- C0+ stepping
  //

  if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.stepping < C0_REV_HSX)) {
    QpiReutPhStv.Data = QpiReadPciCfg (host, Socket, LinkIndex, QPIREUT_PH_STV_QPIAGENT_REUT_REG);
    QpiReutPhStv.Bits.etdetectfwdclk  = 0x9;          
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Socket %u Link %u : QPIREUT_PH_STV write 0x%x", Socket, LinkIndex, QpiReutPhStv.Data));
    QpiWritePciCfg (host, Socket, LinkIndex, QPIREUT_PH_STV_QPIAGENT_REUT_REG, QpiReutPhStv.Data);
  }

  // HSX
  // QPIREUT_PH_PTV.etinbandrstinit. s4968826
  //
  // BDX
  // TODO : Whether this WA is required ?
  if ((host->var.common.cpuType == CPU_HSX && host->var.common.cpuSubType == CPU_HSX_EX) ||
      (host->var.common.cpuType == CPU_BDX && host->var.common.cpuSubType == CPU_BDX_EX)) {
    
    QpiReutPhStv.Data = QpiReadPciCfg (host, Socket, LinkIndex, QPIREUT_PH_STV_QPIAGENT_REUT_REG);
    QpiReutPhPtv.Data = QpiReadPciCfg (host, Socket, LinkIndex, QPIREUT_PH_PTV_QPIAGENT_REUT_REG);
    
    if (QpiReutPhPtv.Bits.etinbandrstinit < QpiReutPhStv.Bits.etdetectfwdclk) {
      QpiReutPhPtv.Bits.etinbandrstinit = QpiReutPhStv.Bits.etdetectfwdclk;
    }
    
    QpiWritePciCfg (host, Socket, LinkIndex, QPIREUT_PH_PTV_QPIAGENT_REUT_REG, QpiReutPhPtv.Data);
  }
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Socket %u Link %u : QPIREUT_PH_PTV write 0x%x", Socket, LinkIndex, QpiReutPhPtv.Data));

  // Using the UniPhy Automation as default for CRB- Builds with BDX- CPU and
  // UniphySettingHsxAndBdx() as default with CRB- Builds for HSX- CPU
  if (host->var.common.cpuType == CPU_BDX) {
    QpiEvAutoRecipe(host, SocketData, Socket, LinkIndex);
  } else {
    UniphySettingHsxAndBdx(host, QpiInternalGlobal, Socket, LinkIndex);
  }
  
  //
  // HSD s4969272(EX)/s4986356(EP): HSX Clone : [C0 PO] s4904719:b256105 patch Required for Cstep
  // s5003444   HSX EP recipe update to 7.0 --- all steppings
  if ((host->var.common.cpuType == CPU_HSX && host->var.common.cpuSubType != CPU_HSX_EX)) {

    // Spin in a tight loop until the LBC is free
    SpinUntilLbcIsFree( host, Socket, LinkIndex);
    
    // Program LBC 59 register
    LbcPerIouData.Data = QpiReadPciCfg (host, Socket, LinkIndex, LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_REG);
    // For LBC 59, apply 0x3B00 value to all HSX-EP- at all speeds 
    LbcPerIouData.Bits.lbc_data = 0x3B00;  //HSX W/A (s5003442)
    QpiWritePciCfg (host, Socket, LinkIndex, LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_REG, LbcPerIouData.Data);

    LbcPerIouControl.Data = QpiReadPciCfg (host, Socket, LinkIndex, LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG);
    LbcPerIouControl.Bits.lbc_ln_sel = 0x100000;
    LbcPerIouControl.Bits.lbc_ld_sel = 0x3B;
    LbcPerIouControl.Bits.lbc_req = 0x2;
    LbcPerIouControl.Bits.lbc_start = 0x1;
    QpiWritePciCfg (host, Socket, LinkIndex, LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG, LbcPerIouControl.Data);

    // Spin in a tight loop until the LBC is free
    SpinUntilLbcIsFree( host, Socket, LinkIndex);
  } // HSX EP check
  
  if ((host->var.common.cpuType == CPU_HSX && host->var.common.cpuSubType == CPU_HSX_EX)) {
    // Spin in a tight loop until the LBC is free
    SpinUntilLbcIsFree( host, Socket, LinkIndex);

    // Program LBC 58 register - Applicable to EX- SKU only (HSX- and BDX-)
    LbcPerIouData.Data = QpiReadPciCfg (host, Socket, LinkIndex, LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_REG);
    if (host->setup.qpi.PhyLinkPerPortSetting[Socket].Phy[LinkIndex].QpiLinkSpeed == SPEED_REC_96GT) {
      LbcPerIouData.Bits.lbc_data = 0x3E00;  //HSX W/A (s4969961)
    } else {
      LbcPerIouData.Bits.lbc_data = 0x3B00;  //HSX W/A (s4986848,s4969272)
    } // EX- SKU running at 9.6 GT/s check
    QpiWritePciCfg (host, Socket, LinkIndex, LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_REG, LbcPerIouData.Data);

    LbcPerIouControl.Data = QpiReadPciCfg (host, Socket, LinkIndex, LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG);
    LbcPerIouControl.Bits.lbc_ln_sel = 0x100000;
    LbcPerIouControl.Bits.lbc_ld_sel = 0x3A;
    LbcPerIouControl.Bits.lbc_req = 0x2;
    LbcPerIouControl.Bits.lbc_start = 0x1;
    QpiWritePciCfg (host, Socket, LinkIndex, LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG, LbcPerIouControl.Data);

    // Spin in a tight loop until the LBC is free
    SpinUntilLbcIsFree( host, Socket, LinkIndex);
  } // HSX-EX

  //
  // Grantley BDX HSD 5002055 : Clone from BDX Sighting:[QPI-EP]: Changes for BIOS LBC writes as reviewed with design team [QPI Only] and
  // Brickland BDX HSD 4970971 : Clone from BDX Sighting:[QPI-EP]: Changes for BIOS LBC writes as reviewed with design team [QPI Only]
  //
  if (host->var.common.cpuType == CPU_BDX) {
    // Setting LBC 56, clocklane params - vrefinputcm bit[13:12] to 0x2, vreflc bit[11:10] to 0x2, vrefrc bit[9:8] to 2, 
    // Vref bit[8:6] to 0x2 and LPFClk detune bit[5:3] to 0x7; zero as other fields.
    LbcPerIouData.Data = QpiReadPciCfg (host, Socket, LinkIndex, LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_REG);
    //
    // Grantley BDX 5002387 : Clone from BDX Sighting:QPI Drift buffer overrun errors with PkgC6 sensitive to LBC 56 0x2A5C fix on certain SKU's and
    // Brickland BDX HSD 4971151 : Need BDX EX BIOS with QPI CTLE St 1+ Fast OC Recal + LBC 56 implementation
    //
    // Grantley BDX 5002459 : Need BDX EP BIOS with QPI LBC 56 = 0x2A5C + LBC 59= 0x3F1C
    if (host->var.common.cpuType == CPU_BDX && host->var.common.cpuSubType == CPU_BDX_EX) {
      LbcPerIouData.Bits.lbc_data = 0x2A5C;
    } else { // For EP- SKU
      if (host->var.common.cpuType == CPU_BDX && host->var.common.stepping >= B0_REV_BDX) {
        LbcPerIouData.Bits.lbc_data = 0x2A5C;
      } else {
        LbcPerIouData.Bits.lbc_data = 0x5C;
      } // stepping check
    } // BDX-EX- check
    QpiWritePciCfg (host, Socket, LinkIndex, LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_REG, LbcPerIouData.Data);

    LbcPerIouControl.Data = QpiReadPciCfg (host, Socket, LinkIndex, LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG);
    LbcPerIouControl.Bits.lbc_ln_sel = 0x100000;  // Enable fwd clock lane
    LbcPerIouControl.Bits.lbc_ld_sel = 0x38;      // Select the fwd clock lane vref
    LbcPerIouControl.Bits.lbc_req = 0x2;
    LbcPerIouControl.Bits.lbc_start = 0x1;
    QpiWritePciCfg (host, Socket, LinkIndex, LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG, LbcPerIouControl.Data);

    // Spin in a tight loop until the LBC is free
    SpinUntilLbcIsFree( host, Socket, LinkIndex);

    // Grantley BDX 5002673 : Need BIOS with LBC 57 = 0x28
    //
    if (host->var.common.cpuType == CPU_BDX && host->var.common.cpuSubType != CPU_BDX_EX &&
        host->var.common.stepping >= B0_REV_BDX) {
      LbcPerIouData.Bits.lbc_data = 0x28;
      QpiWritePciCfg (host, Socket, LinkIndex, LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_REG, LbcPerIouData.Data);

      LbcPerIouControl.Data = QpiReadPciCfg (host, Socket, LinkIndex, LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG);
      LbcPerIouControl.Bits.lbc_ln_sel = 0x100000;  // Enable fwd clock lane
      LbcPerIouControl.Bits.lbc_ld_sel = 0x39;      // Select the fwd clock lane degen load bus
      LbcPerIouControl.Bits.lbc_req = 0x2;
      LbcPerIouControl.Bits.lbc_start = 0x1;
      QpiWritePciCfg (host, Socket, LinkIndex, LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG, LbcPerIouControl.Data);

      // Spin in a tight loop until the LBC is free
      SpinUntilLbcIsFree( host, Socket, LinkIndex);
    } // BDX-EX- and stepping check
  
    // Setting LBC 46, datalane params - Enable the per-bundle control enable for source-degeneration Cap (st1_ctle_peak) in source-degen Preamp stage (Bit 0),
    // set rx_st1_ctle_peak_g1_qpihalf as 1 and set rx_st1_ctle_peak_g3_qpifull as 15; zero other fields.
    LbcPerIouData.Data = QpiReadPciCfg (host, Socket, LinkIndex, LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_REG);
    LbcPerIouData.Bits.lbc_data = 0x1E03;
    QpiWritePciCfg (host, Socket, LinkIndex, LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_REG, LbcPerIouData.Data);

    LbcPerIouControl.Data = QpiReadPciCfg (host, Socket, LinkIndex, LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG);
    LbcPerIouControl.Bits.lbc_ln_sel = 0xFFFFF;  // Enable this for all data bundles
    LbcPerIouControl.Bits.lbc_ld_sel = 0x2E;
    LbcPerIouControl.Bits.lbc_req = 0x2;
    LbcPerIouControl.Bits.lbc_start = 0x1;
    QpiWritePciCfg (host, Socket, LinkIndex, LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG, LbcPerIouControl.Data);

    // Spin in a tight loop until the LBC is free
    SpinUntilLbcIsFree( host, Socket, LinkIndex);

    // Setting LBC 59 to 0x3F1C for BDX-EX and BDX-EP-B0+ for all speeds and 0x3E00 for BDX-EP-A0/A1 for all speeds
    // Brickland BDX 4971187 : Clone from BDX Sighting:[QPI-EX]: Seeing CRC's after issuing phyresets at all full speeds which in turn eventually leads to Drift Buffer Issue
    // Grantley BDX 5002459 : Need BDX EP BIOS with QPI LBC 56 = 0x2A5C + LBC 59= 0x3F1C
    LbcPerIouData.Data = QpiReadPciCfg (host, Socket, LinkIndex, LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_REG);
    LbcPerIouData.Bits.lbc_data = 0x3F1C;
    if (host->var.common.cpuType == CPU_BDX && host->var.common.cpuSubType == CPU_BDX_EX) {
      LbcPerIouData.Bits.lbc_data = 0x3F1C;
    } else { // For EP- SKU
      if (host->var.common.cpuType == CPU_BDX && host->var.common.stepping >= B0_REV_BDX) {
        LbcPerIouData.Bits.lbc_data = 0x3F1C;
      } else {
        LbcPerIouData.Bits.lbc_data = 0x3E00;
      } // stepping check
    } // BDX-EX- check
    QpiWritePciCfg (host, Socket, LinkIndex, LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_REG, LbcPerIouData.Data);

    LbcPerIouControl.Data = QpiReadPciCfg (host, Socket, LinkIndex, LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG);
    LbcPerIouControl.Bits.lbc_ln_sel = 0x100000;
    LbcPerIouControl.Bits.lbc_ld_sel = 0x3B;
    LbcPerIouControl.Bits.lbc_req = 0x2;
    LbcPerIouControl.Bits.lbc_start = 0x1;
    QpiWritePciCfg (host, Socket, LinkIndex, LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG, LbcPerIouControl.Data);

    // Spin in a tight loop until the LBC is free
    SpinUntilLbcIsFree( host, Socket, LinkIndex);

    // Setting LBC 58 to 0x3F10 for BDX-EX- for all speeds (EX- SKU only, not to touch for EP- SKU)
    // Brickland BDX 4971187 : Clone from BDX Sighting:[QPI-EX]: Seeing CRC's after issuing phyresets at all full speeds which in turn eventually leads to Drift Buffer Issue
    if (host->var.common.cpuType == CPU_BDX && host->var.common.cpuSubType == CPU_BDX_EX) {
      // Spin in a tight loop until the LBC is free
      SpinUntilLbcIsFree( host, Socket, LinkIndex);

      // Program LBC 58 register - Applicable to EX- SKU only (BDX-)
      LbcPerIouData.Data = QpiReadPciCfg (host, Socket, LinkIndex, LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_REG);
      LbcPerIouData.Bits.lbc_data = 0x3F10;
      QpiWritePciCfg (host, Socket, LinkIndex, LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_REG, LbcPerIouData.Data);

      LbcPerIouControl.Data = QpiReadPciCfg (host, Socket, LinkIndex, LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG);
      LbcPerIouControl.Bits.lbc_ln_sel = 0x100000;
      LbcPerIouControl.Bits.lbc_ld_sel = 0x3A;
      LbcPerIouControl.Bits.lbc_req = 0x2;
      LbcPerIouControl.Bits.lbc_start = 0x1;
      QpiWritePciCfg (host, Socket, LinkIndex, LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG, LbcPerIouControl.Data);

      // Spin in a tight loop until the LBC is free
      SpinUntilLbcIsFree( host, Socket, LinkIndex);
    }
  } // BDX- cpuType check

  return QPI_SUCCESS;
}

QPI_STATUS
ProgramPhyLayer (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  )
/*++

Routine Description:
   Programs PHY layer registers

Arguments:
   host  - Pointer to the system host (root) structure
   SocketData - Socket specific data structure

Returns:

--*/

{

  UINT8 Index;
  UINT8 LinkIndex;
  UINT8                  LinkSpeedParameter[128];
  UINT8                  LinkHalfSpeedParameter[128];
  BOOLEAN   Qpi11FeatureDisable, ProbeInstalled;

  Qpi11FeatureDisable = DisableFeatureWhenQpiProbeInstalled(host, SocketData);
  ProbeInstalled =  IsQpiProbeInstalled(host, SocketData);

  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    if (SocketData->Cpu[Index].Valid == TRUE) {
       for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {
        // Always do this regardless of link enabled
        if (LinkIndex != 1) {
          // Using the conventional way for CRB- Builds with HSX- CPU. For CRB- Builds with
          // BDX- CPU, UniPhy Recipe automation covers these register programming
          if (host->var.common.cpuType == CPU_HSX) {
            ConfigSocketLinkPhyMisc_0(host, SocketData, Index, LinkIndex, ProbeInstalled);
          }
        }
      } //For LinkIndex (1st instance - Valid and non-valid links)

      for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {
        if ((SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) || (host->setup.qpi.QpiLbEn)) {
          // Locate EPARAM table for Link
          GetSocketLinkEparams(host, SocketData, Index, LinkIndex, &LinkSpeedParameter, &LinkHalfSpeedParameter);
          ConfigSocketLinkEparams(host, SocketData, Index, LinkIndex, &LinkSpeedParameter, &LinkHalfSpeedParameter);
          ConfigSocketLinkPhyMisc(host, SocketData, QpiInternalGlobal, Index, LinkIndex, Qpi11FeatureDisable);
        } else {
          //
          // Put unused links into low power/disable state
          //
          DisableLinkForceToLowPower(host, QpiInternalGlobal, Index, LinkIndex);
        }
      } //For LinkIndex (2nd instance - Valid links only)
    } //If Socket valid
  } //For Socket

  return QPI_SUCCESS;
}



STATIC
QPI_STATUS
DisableLinkForceToLowPower(
  struct sysHost      *host,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal, 
  UINT8               Socket,
  UINT8               Link
  )
/*++

Routine Description:
  Disables the link forces it to low power

Arguments:
  host  - Pointer to the system host (root) structure
  Socket - SocId we are looking up
  Link - Link# on socket (0 or 1)

Returns:

--*/
{
  QPIREUT_PH_CTR_QPIAGENT_REUT_HSX_BDX_STRUCT  QpiReutPhCtr;
  UINT32 QpiPhyPwrCtrl;

  //Unused link, so disable
  //CLR PhyINitBegin  8:9/0/FCh:0
  QpiReutPhCtr.Data = QpiReadPciCfg (host, Socket,Link,  QPIREUT_PH_CTR_QPIAGENT_REUT_REG);
  QpiReutPhCtr.Bits.phyinitbegin = 0;
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Clr PhyInitBegin on Socket %u Link %u : QPIREUT_PH_CTR write 0x%x", Socket, Link, QpiReutPhCtr.Data));
  QpiWritePciCfg (host, Socket,Link,  QPIREUT_PH_CTR_QPIAGENT_REUT_REG, QpiReutPhCtr.Data);

  //
  // If IVT EXA and hot plug enabled, then mask FW_AGENT through pcode mailbox command and skip "force low power - L1 state" if a0 stepping
  //
  if( QpiInternalGlobal->QpiCpuSktHotPlugEn == TRUE ) {
    SendMailBoxCmdToPcode(host, Socket, MAILBOX_BIOS_CMD_MASK_FW_AGENT |(Link << 8), 0);       
  }

    //
    //Force unused link to L1
    //
    QpiPhyPwrCtrl = QpiReadPciCfg (host, Socket,Link, QPIPHYPWRCTRL_QPIAGENT_LL_REG);
    QpiPhyPwrCtrl |= 1;
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n  Set force_l1 on Socket %u Link %u : QPIPHYPWRCTRL write 0x%x", Socket, Link, QpiPhyPwrCtrl));
    QpiWritePciCfg (host, Socket,Link, QPIPHYPWRCTRL_QPIAGENT_LL_REG, QpiPhyPwrCtrl);
    

 
  return QPI_SUCCESS;

}



STATIC
QPI_STATUS
DisableForceToLowPowerAllUnusedLinks(
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  )
/*++

Routine Description:
  Disables the link forces it to low power

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Socket specific data structure
  QpiInternalGlobal - QPIRC internal variables.

Returns:

--*/
{
  
  UINT8 Index;
  UINT8 LinkIndex;

  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Force unused links to disabled/low power state."));      
  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    
    if (SocketData->Cpu[Index].Valid == TRUE) {
       
      for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {
        
        if ((SocketData->Cpu[Index].LinkData[LinkIndex].Valid == FALSE)  && !(host->setup.qpi.QpiLbEn)) {
          DisableLinkForceToLowPower(host, QpiInternalGlobal, Index, LinkIndex);
        }
      }
    }
  }

  return QPI_SUCCESS;

}

STATIC
QPI_STATUS
EnableClockGatingToAllUnusedLinks(
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  )
/*++

Routine Description:
  Enables clock gating to the unused links 

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Socket specific data structure
  QpiInternalGlobal - QPIRC internal variables.

Returns:

--*/
{
  
  UINT8 Index;
  UINT8 LinkIndex;
  UINT32  Data32;

  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Enable clock gating to unused links."));      
  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    
    if (SocketData->Cpu[Index].Valid == TRUE) {
       
      for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {
        
        if ((SocketData->Cpu[Index].LinkData[LinkIndex].Valid == FALSE)  && !(host->setup.qpi.QpiLbEn)) {

          Data32 = QpiReadPciCfg (host, Index, LinkIndex, QPIMISCCLKCTRL_QPIAGENT_LL_REG);
          Data32 |= 0x23;
          QpiWritePciCfg (host, Index, LinkIndex, QPIMISCCLKCTRL_QPIAGENT_LL_REG, Data32);
        }
      }
    }
  }

  return QPI_SUCCESS;

}

STATIC
QPI_STATUS
LinkLayerResetWA(
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  )
/*++

Routine Description:
  Reset link layer for CRC 16 W/A

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Socket specific data structure
  QpiInternalGlobal - QPIRC internal variables.

Returns:

--*/
{
  return QPI_SUCCESS;
}


STATIC
QPI_STATUS
GetSocketLinkEparams(
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  UINT8               Socket,
  UINT8               Link,
  VOID                *LinkSpeedParameter,
  VOID                *LinkHalfSpeedParameter
  )
/*++

Routine Description:
   Locates the EPARAMS based on socket/link

Arguments:
   host  - Pointer to the system host (root) structure
   SocketData - Socket specific data structure
   Socket - SocId we are looking up
   Link - Link# on socket
   *LinkSpeedParameter - return ptr
   *LinkHalfSpeedParameter - return ptr

Returns:
   ptr to EPARAM table
   QPI_SUCCESS if entry found
   QPI_FAILURE if no entry found

--*/
{
  QPI_STATUS                  FullSpeedStatus, HalfSpeedStatus;
  UINT8                       LinkSpeed, ProbeType;
  
  LinkSpeed = (UINT8) host->setup.qpi.PhyLinkPerPortSetting[Socket].Phy[Link].QpiLinkSpeed;
  ProbeType = (UINT8) host->setup.qpi.PhyLinkPerPortSetting[Socket].Phy[Link].QpiProbeType;

  QpiDebugPrintInfo1((host, QPI_DEBUG_ERROR, "\n\n  ***** QPI Electrical Parameters for CPU%u  Link %u LinkSpeed %u **** \n", Socket, Link, LinkSpeed)); 

  //
  //OEM call to get Eparamtable.   OEM call handles returning the correct EPARAM based on
  //Socket, Link, Freq, Probetype, Platform
  //
  FullSpeedStatus = OemQpiGetEParams(host, Socket, Link, LinkSpeed, ProbeType, LinkSpeedParameter, FULL_SPEED);
  HalfSpeedStatus = OemQpiGetEParams(host, Socket, Link, LinkSpeed, ProbeType, LinkHalfSpeedParameter, HALF_SPEED);

  if ((FullSpeedStatus != QPI_SUCCESS) || (HalfSpeedStatus != QPI_SUCCESS)) {
      
    if (ProbeType != PROBE_TYPE_NO_PROBE) {  
      //
      // Didn't find an entry for this PROBE TYPE, so look for NO PROBE entry
      //
      QpiCheckPoint((UINT8)(1 << Socket), SOCKET_TYPE_CPU, (UINT8)(1 << Link), ERR_FULL_SPEED_TRANSITION, MINOR_ERR_QPI_ELEC_PARAM_NOT_FOUND, host);
      QpiDebugPrintInfo1((host, QPI_DEBUG_ERROR, "\n\n;***** QPI Electrical Parameters for CPU%u  Link %u with ProbeType %u is not found**** \n", Socket, Link, host->setup.qpi.PhyLinkPerPortSetting[Socket].Phy[Link].QpiProbeType));
    
      QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n;Searching for matching entry with PROBE_TYPE == NO_PROBE."));      
      
      FullSpeedStatus = OemQpiGetEParams(host, Socket, Link, LinkSpeed, PROBE_TYPE_NO_PROBE, LinkSpeedParameter, FULL_SPEED);
      HalfSpeedStatus = OemQpiGetEParams(host, Socket, Link, LinkSpeed, PROBE_TYPE_NO_PROBE, LinkHalfSpeedParameter, HALF_SPEED);
    }

    if ((FullSpeedStatus != QPI_SUCCESS) || (HalfSpeedStatus != QPI_SUCCESS)) {

      //
      // No entry found at all!!
      //
      QpiCheckPoint((UINT8)(1 << Socket), SOCKET_TYPE_CPU, (UINT8)(1 << Link), ERR_FULL_SPEED_TRANSITION, MINOR_ERR_QPI_ELEC_PARAM_NOT_FOUND, host);
      QpiDebugPrintFatal((host, QPI_DEBUG_ERROR, "\n\n;***** QPI Electrical Parameters for CPU%u  Link %u is not found**** \n", Socket, Link));      
      QPI_ASSERT(FALSE, ERR_FULL_SPEED_TRANSITION, MINOR_ERR_QPI_ELEC_PARAM_NOT_FOUND);  
      
    }
      
  }
   
  return QPI_SUCCESS;
}


QPI_STATUS
ConfigSocketLinkEparams (
  struct sysHost         *host,
  QPI_SOCKET_DATA        *SocketData,
  UINT8                  Socket,
  UINT8                  Link,
  VOID                   *LinkSpeedParameter,
  VOID                   *LinkHalfSpeedParameter
  )
/*++

Routine Description:
   Programs the values from the EPARAMS entry for specified link

Arguments:
   host  - Pointer to the system host (root) structure
   SocketData - Socket specific data structure
   Socket - SocId we are looking up
   Link - Link# on socket
   *LinkSpeedParameter - ptr to eparam data
   *LinkHalfSpeedParameter - ptr to half speed eparam data
   
Returns:

--*/
{
  UINT32    ptv=0, prt=0;
  HSX_PER_LANE_EPARAM_LINK_INFO *LinkSpeedParameterHsx;
  HSX_ALL_LANES_HS_EPARAM_LINK_INFO *LinkHalfSpeedParameterHsx;

  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Program EPARAM values for Socket %u Link %u.", Socket, Link)); 

  LinkSpeedParameterHsx = (HSX_PER_LANE_EPARAM_LINK_INFO *) LinkSpeedParameter;
  LinkHalfSpeedParameterHsx = (HSX_ALL_LANES_HS_EPARAM_LINK_INFO *) LinkHalfSpeedParameter;
  ptv = LinkSpeedParameterHsx->PTV;
  prt = LinkSpeedParameterHsx->PRT;
  ConfigSocketLinkEparamsHsx(host, SocketData, Socket, Link, LinkSpeedParameterHsx, LinkHalfSpeedParameterHsx);

  QpiWritePciCfg (host, Socket, Link, QPIREUT_PH_PTV_QPIAGENT_REUT_REG, ptv);

  if(host->var.qpi.OutQpiLinkL0rEn == 0) {
    prt = 0;     // disabled  
  } 

  QpiWritePciCfg (host, Socket, Link, QPIREUT_PH_PRT_QPIAGENT_REUT_REG, prt);

  return QPI_SUCCESS;
}


QPI_STATUS
ConfigSocketLinkEparamsHsx (
  struct sysHost         *host,
  QPI_SOCKET_DATA        *SocketData,
  UINT8                  Socket,
  UINT8                  Link,
  HSX_PER_LANE_EPARAM_LINK_INFO                 *LinkSpeedParameter,
  HSX_ALL_LANES_HS_EPARAM_LINK_INFO             *LinkHalfSpeedParameter
  )
/*++

Routine Description:
   Programs the values from the EPARAMS entry for specified link

Arguments:
   host  - Pointer to the system host (root) structure
   SocketData - Socket specific data structure
   Socket - SocId we are looking up
   Link - Link# on socket
   *LinkSpeedParameter - ptr to eparam data
   *LinkHalfSpeedParameter - ptr to half speed eparam data
   
Returns:

--*/
{
  UINT8     Data8;             
  UINT8     Level, Lane;
  UINT8     TxeqLvlReg[4]   = {IOVB_TXEQ_LVL0_0_0_QPIAGENT_DFX_PHY_IOVB_REG, 
                               IOVB_TXEQ_LVL1_0_0_QPIAGENT_DFX_PHY_IOVB_REG,
                               IOVB_TXEQ_LVL2_0_0_QPIAGENT_DFX_PHY_IOVB_REG, 
                               IOVB_TXEQ_LVL3_0_0_QPIAGENT_DFX_PHY_IOVB_REG};

  UINT8     TxeqHsLvlReg[4] = {IOVB_TXEQ_HALF_LVL0_0_0_QPIAGENT_DFX_PHY_IOVB_REG,
                               IOVB_TXEQ_HALF_LVL1_0_0_QPIAGENT_DFX_PHY_IOVB_REG,
                               IOVB_TXEQ_HALF_LVL2_0_0_QPIAGENT_DFX_PHY_IOVB_REG,
                               IOVB_TXEQ_HALF_LVL3_0_0_QPIAGENT_DFX_PHY_IOVB_REG};

  if (LinkSpeedParameter->AllLanesUseSameTxeq == ALL_LANES_TXEQ_ENABLED) {      // all lanes use same txeq value, use broadcast as unit ID
    //Program TXEQ & Half speed TXEQ
    for( Level = 0; Level < 4; Level++){
        Data8 = (UINT8) ((LinkSpeedParameter->TXEQL[0] >> (Level *8)) & 0x3f);
        QpiWriteIar (host, Socket, Link, QPILIB_IAR_BCAST, TxeqLvlReg[Level], Data8);
        Data8 = ((LinkHalfSpeedParameter->HS_AllLanesTXEQ >> (Level *8)) & 0x3f);
        QpiWriteIar (host, Socket, Link, QPILIB_IAR_BCAST, TxeqHsLvlReg[Level], Data8);
    } 

    //Program CTLE
    Data8 = ((UINT8) LinkSpeedParameter->CTLEPEAK[0]) << 1;
    Data8 = Data8 | 0x01;           //rx_ctle_offset_en = 1; 
    QpiWriteIar (host, Socket, Link, QPILIB_IAR_BCAST, IOVB_RX_CTLE0_0_0_QPIAGENT_DFX_PHY_IOVB_REG, Data8);

  } else {                                                // each lanes has different txeq value, use lane # as unit ID                            
   //Program TXEQ & Half speed TXEQ
   for(Lane= 0; Lane < MAX_QPI_LANES; Lane++) { 
     for( Level = 0; Level < 4; Level++){ 
        Data8 = (UINT8) ((LinkSpeedParameter->TXEQL[Lane] >> (Level *8)) & 0x3f);
        QpiWriteIar (host, Socket, Link, Lane, TxeqLvlReg[Level], Data8);
        Data8 = ((LinkHalfSpeedParameter->HS_AllLanesTXEQ >> (Level *8)) & 0x3f);
        QpiWriteIar (host, Socket, Link, Lane, TxeqHsLvlReg[Level], Data8);
      } 
    } 

    //Program CTLE
    for(Lane= 0; Lane < MAX_QPI_LANES; Lane++) { 
      Data8 = ((UINT8) (LinkSpeedParameter->CTLEPEAK[Lane/4] >> ((Lane%4) * 8)) & 0xFF) << 1;
      Data8 = Data8 | 0x01;           //rx_ctle_offset_en = 1; 
      QpiWriteIar (host, Socket, Link, Lane, IOVB_RX_CTLE0_0_0_QPIAGENT_DFX_PHY_IOVB_REG, Data8);
    } 
  }

  return QPI_SUCCESS;
}



QPI_STATUS
ConfigSocketLinkForLinkLayerMisc (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal,
  UINT8               Index,
  UINT8               LinkIndex,
  QPILCP_QPIAGENT_LL_HSX_BDX_STRUCT   QpiLcp
  )
{
  QPILCL_QPIAGENT_LL_STRUCT                QpiLcl;
  QPILCRDC_QPIAGENT_LL_STRUCT              QpiLcrdc;

  QPIREUT_PM_R1_QPIAGENT_REUT_STRUCT       QpiReutPmR1;
  QPIREUT_PM_R2_QPIAGENT_REUT_STRUCT       QpiReutPmR2;  

  QPI_LINK_DATA             *pPeerInfo; 


  QPIREUT_PM_R3_QPIAGENT_REUT_HSX_BDX_STRUCT      QpiReutPmR3;
  QPIERRDIS_QPIAGENT_LL_HSX_BDX_STRUCT            QpiErrDis;
  QPIDBGERRSTDIS0_QPIAGENT_DFX_LINK_HSX_BDX_STRUCT    QpiDbgErrStDis0; 

  //
  // For the purpose to build successfully, initialize 
  //
  
  // get remote socket id, and pbox port
  pPeerInfo = &(SocketData->Cpu[Index].LinkData[LinkIndex]); 
  
  QpiLcl.Data = QpiReadPciCfg (host, Index, LinkIndex, QPILCL_QPIAGENT_LL_REG);
  QpiLcrdc.Data = QpiReadPciCfg (host, Index, LinkIndex, QPILCRDC_QPIAGENT_LL_REG);
  
  //
  // Setup QpiLcl QPI Link Capability Register
  //

  if (QpiLcp.Bits.crc_mode_supported ) {
    QpiLcl.Bits.crc_mode = host->var.qpi.OutQpiCrcMode;
  } else {
    host->var.qpi.OutQpiCrcMode = 0;
    QpiLcl.Bits.crc_mode = host->var.qpi.OutQpiCrcMode;
  }

  //
  //Program VNA/VN0/Vn1 from table
  //
  //
  //Program LLCredits LinkCreditControl
  //
  if( QpiInternalGlobal->Vn1En == TRUE ){
      QpiLcrdc.Bits.vn1_credit_init = 1;
  } else {
      QpiLcrdc.Bits.vna_credits = QPI_LL_VNA_NORMAL_HSX;
  }
  
  if (host->setup.qpi.PhyLinkPerPortSetting[Index].Link[LinkIndex].QpiLinkCreditReduce == 1) {
      QpiLcrdc.Bits.vna_credits = QPI_LL_VNA_MIN;
  }
  
  //
  //Program L0p and params
  //
  if (QpiLcp.Bits.l0p_support) {
    QpiLcl.Bits.l0p_enable_mode_at_transmitter = host->var.qpi.OutQpiLinkL0pEn;
  } else {
    host->var.qpi.OutQpiLinkL0pEn = 0;
  }

    //
    // Program blocking values based on speed
    //
    QpiReutPmR1.Data = QpiReadPciCfg (host, Index, LinkIndex, QPIREUT_PM_R1_QPIAGENT_REUT_REG);
    QpiReutPmR2.Data = QpiReadPciCfg (host, Index, LinkIndex, QPIREUT_PM_R2_QPIAGENT_REUT_REG);
    QpiReutPmR3.Data = QpiReadPciCfg (host, Index, LinkIndex, QPIREUT_PM_R3_QPIAGENT_REUT_REG);

    switch (host->setup.qpi.PhyLinkPerPortSetting[Index].Phy[LinkIndex].QpiLinkSpeed) {
    case SPEED_REC_96GT:
    case SPEED_REC_80GT:
    case SPEED_REC_72GT:
    case SPEED_REC_64GT:
    default:            
      //these are actually phy
      QpiReutPmR1.Bits.tl0pnullstop2done = 0x0A;  //HSX QpiReutPmR1
      QpiReutPmR1.Bits.tl0ptran2nullstop = 0x02;
      QpiReutPmR1.Bits.tl0pnull2tran     = 0x0;
      QpiReutPmR1.Bits.tl0penter2null    = 0x40;

      QpiReutPmR2.Bits.tl0punblock2done  = 0x0A;  //HSX QpiReutPmR2
      QpiReutPmR2.Bits.tl0ptran2unblock  = 0x02;
      QpiReutPmR2.Bits.tl0penter2tran    = 0x40;
        
      QpiReutPmR3.Bits.txl0pexitrtrnalarmsel = 0x5;
      QpiReutPmR3.Bits.rxl0pexitrtrnalarmsel = 0x1;

      // HSD 4969593 : QPI:  Need registers changes to support L0p when CRC mode is 16b
      // (Si workaround haswell_server_4905871)
      if (QpiLcl.Bits.crc_mode == CRC_MODE_ROLLING_16BIT) {
        // Override the QPIREUT_PM_R1.tl0pnull2tran and QPIREUT_PM_R2.tl0penter2tran values
        // when both L0p and 16-bit CRC are enabled.
        QpiReutPmR1.Bits.tl0pnull2tran = 0x02;
        QpiReutPmR2.Bits.tl0penter2tran = 0x42;
      }
      break;
    }//switch

    QpiWritePciCfg (host, Index, LinkIndex, QPIREUT_PM_R1_QPIAGENT_REUT_REG, QpiReutPmR1.Data);
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Socket %u Link %u : QPIREUT_PM_R1 write 0x%x", Index, LinkIndex, QpiReutPmR1.Data));
    
    QpiWritePciCfg (host, Index, LinkIndex, QPIREUT_PM_R2_QPIAGENT_REUT_REG, QpiReutPmR2.Data);
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Socket %u Link %u : QPIREUT_PM_R2 write 0x%x", Index, LinkIndex, QpiReutPmR2.Data));

    QpiWritePciCfg (host, Index, LinkIndex, QPIREUT_PM_R3_QPIAGENT_REUT_REG, QpiReutPmR3.Data);
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Socket %u Link %u : QPIREUT_PM_R3 write 0x%x", Index, LinkIndex, QpiReutPmR3.Data));

  //
  //Program L1 and params
  //
  if ((QpiLcp.Bits.l1_slave_support) && (QpiLcp.Bits.l1_master_support)) {

    if (host->var.qpi.OutQpiLinkL1En) {
      //
      //Program Master/Slave.  Only sockets with smaller ID are Master.
      //
      if( Index < (UINT8)pPeerInfo->PeerSocId)   
      {  
        QpiLcl.Bits.l1_master_slave = 1;
      }
      else
      {
        QpiLcl.Bits.l1_master_slave = 0;
      }
      QpiLcl.Bits.l1_enable = host->var.qpi.OutQpiLinkL1En;
    }
  } else {
     
    host->var.qpi.OutQpiLinkL1En = 0;
  }

  //
  // Update LCL and LCRDC registers
  //
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Socket %u Link %u : QPILCL write 0x%x", Index, LinkIndex, QpiLcl.Data));
  QpiWritePciCfg (host, Index, LinkIndex, QPILCL_QPIAGENT_LL_REG, QpiLcl.Data);

  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Socket %u Link %u : QPILCRDC write 0x%x", Index, LinkIndex, QpiLcrdc.Data));
  QpiWritePciCfg (host, Index, LinkIndex, QPILCRDC_QPIAGENT_LL_REG, QpiLcrdc.Data);



  if (host->var.qpi.OutQpiLinkL1En) {
    if (host->var.common.cpuType == CPU_HSX || host->var.common.cpuType == CPU_BDX) {
      // HSX W/A (Permanent) (4166852): HSX Clone: QPI EP : Enabling L1 is flagging CorErr 0x22 phy detected inband reset (cor_mask[2]=1)
      QpiErrDis.Data = QpiReadPciCfg(host, Index, LinkIndex, QPIERRDIS_QPIAGENT_LL_REG);
      QpiErrDis.Bits.cor_mask |= (1 << 2);  //cor_mask[2]=1 (Disable MCA Error Code 22)
      QpiWritePciCfg (host, Index, LinkIndex, QPIERRDIS_QPIAGENT_LL_REG, QpiErrDis.Data);
    }
  }

  // HSX EP/EX W/A (4167997): HSX Clone: Unexpected PM Ack/Nak (0x13)
  if (host->var.common.cpuType == CPU_HSX && host->var.common.stepping < C0_REV_HSX) {
    QpiDbgErrStDis0.Data = QpiReadPciCfg (host, Index, LinkIndex, QPIDBGERRSTDIS0_QPIAGENT_DFX_LINK_REG);
    QpiDbgErrStDis0.Bits.disable_err |= BIT17;      //Disable reporting of this error.
    QpiWritePciCfg (host, Index, LinkIndex, QPIDBGERRSTDIS0_QPIAGENT_DFX_LINK_REG, QpiDbgErrStDis0.Data);
  }

  return QPI_SUCCESS;
}

QPI_STATUS
ConfigSocketLinkForLinkLayerMisc_0 (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal,
  UINT8               Index  
  )
{
  return QPI_SUCCESS;
}

STATIC
QPI_STATUS
ProgramLinkLayer (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  )
/*++

Routine Description:
   Program Link layer registers

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Socket specific data structure

Returns:

--*/

{
   
  UINT8 Index, LinkIndex;
  QPILCP_QPIAGENT_LL_HSX_BDX_STRUCT        QpiLcp;
  
  //
  // Gather common capabilites of all valid sockets
  //
  QpiLcp.Data = 0xffffffff;

  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    if (SocketData->Cpu[Index].Valid == TRUE) {
      for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {        
        if ((SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) || (host->setup.qpi.QpiLbEn)) {
          
          QpiLcp.Data &= QpiReadPciCfg (host, Index, LinkIndex, QPILCP_QPIAGENT_LL_REG);
          
        }
      }
    }
  }
          

  //
  // Based on common capabilites, program LCL etc
  //
  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    if (SocketData->Cpu[Index].Valid == TRUE) {
      for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {
        if (SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) {
           ConfigSocketLinkForLinkLayerMisc(host, SocketData, QpiInternalGlobal,Index, LinkIndex, QpiLcp);
        }       
      }
      ConfigSocketLinkForLinkLayerMisc_0(host, SocketData, QpiInternalGlobal, Index);
     
    }//if (SocketData->Cpu[Index].Valid == TRUE)
  }//for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index)
               
  return QPI_SUCCESS;
}

QPI_STATUS
SelectSupportedLinkSpeedForHotAdd (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal,
  UINT8              *LinkSpeed
  )
/*++
Routine Description:
   Routine to determine and select the maximum QPI link speed supported by CPUs. It is determined by
   Min(Max link speed of all populated CPU sockets).  A valid User Selected freq will be returned 
   if enabled. To be used only for SBSP in case hotplug is enabled, and we're booting as 1S.

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Socket specific data structure
  QpiInternalGlobal - QPIRC internal variables
  LinkSpeed - Pointer to the link speed value.

Returns:
   Speed to transition to via *LinkSpeed

--*/
{
  UINT8  Index, LinkIndex, LocalLinkSpeed;
  struct sysSetup *setup;

  setup = (struct sysSetup *) &host->setup;
  LocalLinkSpeed = SPEED_REC_64GT;

  if (host->var.qpi.OutQpiLinkSpeed == MAX_QPI_LINK_SPEED) {
    //
    // Max speed is requested; so set the speed to maximum speed supported by all populated sockets.
    //
    LocalLinkSpeed = GetMaxSupportedLinkSpeed (host, SocketData);
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n  Selected QPI Link Speed: %u", (LocalLinkSpeed + 7) * 8));

  } else if (host->var.qpi.OutQpiLinkSpeed < MAX_QPI_LINK_SPEED) {
    //
    // See if requested freq is supported
    //
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n  Request: QPI Speed of %u", (((UINT8) host->var.qpi.OutQpiLinkSpeed+7)*8)));
    if (CheckLinkSpeedSupported(host, SocketData, (UINT8) host->var.qpi.OutQpiLinkSpeed) == QPI_SUCCESS) {
      LocalLinkSpeed = host->var.qpi.OutQpiLinkSpeed;
      QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n  Requested speed is valid"));
    } else {
      //
      // Requested speed unsupported, default to 6.4GT
      //
      QpiLogWarning(host, 0xFF, 0xFF, 0xFF, WARN_UNSUPPORTED_LINK_SPEED);
      QpiDebugPrintWarn((host, QPI_DEBUG_WARN, "\n WARNING:  Requested Link Speed %u is not supported. Defaulting to 6.4GT \n",
                         (((UINT8) host->var.qpi.OutQpiLinkSpeed+7)*8)));
      LocalLinkSpeed = SPEED_REC_64GT;
    }
  }

  if (host->var.qpi.RasInProgress == TRUE) {
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n  QPI RAS Online QPI Freq is %u GT.", ((LocalLinkSpeed+7)*8)));
    host->var.qpi.OutQpiLinkSpeed = LocalLinkSpeed;  
    for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
      if (SocketData->Cpu[Index].Valid == TRUE) {
        for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {
          setup->qpi.PhyLinkPerPortSetting[Index].Phy[LinkIndex].QpiLinkSpeed = LocalLinkSpeed;
        }  
      }
    }
  }
  
  //
  // 4970550: HSX Clone : [HSX D0 PO] QPI EX : Remove L0p @ 9.6G QPI - csiLinkTrainTest Hard resets is causing CRCs on SS2A, 8B crc config
  // 4971062: BDX-EX QPI: Disable L0p at 9.6GT/s for all EX 
  //
  if ((LocalLinkSpeed == SPEED_REC_96GT) && 
      (((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType == CPU_HSX_EX)) ||
      ((host->var.common.cpuType == CPU_BDX) && (host->var.common.cpuSubType == CPU_BDX_EX)))) {
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n  SelectSupportedLinkSpeedForHotAdd: Disabling L0p b/c of 9.6GT/s"));
    host->var.qpi.OutQpiLinkL0pEn = 0;
  } // if HSX_EX or BDX_EX and at 9.6GT/s speed

  *LinkSpeed = LocalLinkSpeed;

  return QPI_SUCCESS;
}

QPI_STATUS
SelectSupportedLinkSpeed (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  )
/*++
Routine Description:
   Routine to determine and select the maximum QPI link speed supported by CPUs. It is determined by
   Min(Max link speed of all populated CPU sockets).  A valid User Selected freq will be returned 
   if enabled. If PER_LINK_SPEED is selected, then the links speeds will be normalized for both ends
   of the link. Note that the normalization will be done only for the sockets & links that are valid.
   So that the per link option of the unpopulated/unconnected links will not influence the normalization
   outcome. For O*L operation, this routine should be called after the socket/link valid information is
   collected so that the per link options can be normalized properly for the new topology scenario.

   If adaptation is scheduled to run, the speed selected will be the one decided by adaptation code.
   Also, the PER_LINK_SPEED request will not be fullfilled since the adaptation can support only
   system wide common speed, not per link speed adaptation.

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Socket specific data structure
  QpiInternalGlobal - QPIRC internal variables.

Returns:
   Speed to transition too via host->var.qpi.OutQpiLinkSpeed

--*/
{
  UINT8                    LinkSpeed, MaxSpeed;
  UINT8                    Speed0, Speed1, Speed2;
  UINT8                    Index, LinkIndex;
  BOOLEAN                  LinkDependency;
  QPI_LINK_DATA            *pPeerInfo;
  struct sysSetup          *setup;

  setup = (struct sysSetup *) &host->setup;

  // Get the Max QPI link speed supported by all populated sockets
  MaxSpeed = GetMaxSupportedLinkSpeed (host, SocketData);

  LinkSpeed = SPEED_REC_64GT;

  if (QpiInternalGlobal->QpiRunAdaptation == TRUE) {
    //
    // QPI adaptation is scheduled to run. Use the speed selected by the adaptation engine.
    //
    LinkSpeed = QpiInternalGlobal->CurrentSpeedAdapted;
  } else if (host->var.qpi.OutQpiLinkSpeed == MAX_QPI_LINK_SPEED || host->var.qpi.OutQpiLinkSpeed == MAX_LIMITED_QPI_LINK_SPEED) {
    //
    // Max speed is requested; so set the speed to maximum speed supported by all populated sockets.
    //
    LinkSpeed = MaxSpeed;
  } else if (host->var.qpi.OutQpiLinkSpeed < MAX_QPI_LINK_SPEED) {
    //
    // See if requested freq is supported
    //
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Request: QPI Speed of %u", (((UINT8) host->var.qpi.OutQpiLinkSpeed+7)*8)));
    if (CheckLinkSpeedSupported(host, SocketData, (UINT8) host->var.qpi.OutQpiLinkSpeed) == QPI_SUCCESS) {
      LinkSpeed = host->var.qpi.OutQpiLinkSpeed;
      QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Requested speed is valid"));
    } else {
      //
      // Requested speed unsupported, default to 6.4GT
      //
      QpiLogWarning(host, 0xFF, 0xFF, 0xFF, WARN_UNSUPPORTED_LINK_SPEED);
      QpiDebugPrintWarn((host, QPI_DEBUG_WARN, "\n; WARNING:  Requested Link Speed %u is not supported. Defaulting to 6.4GT \n",
                         (((UINT8) host->var.qpi.OutQpiLinkSpeed+7)*8)));
      LinkSpeed = SPEED_REC_64GT;
    }
  } else if (host->var.qpi.OutQpiLinkSpeed == FREQ_PER_LINK) {
    // 
    // Normalize per link speed option.  Link 0 & 1 share same PLL while Link 2 has separate PLL. As a result link 0 & 1 must
    // operate at same speed while link 2 can be at different speed than link 0/1. Also, both sides of a given link must operate
    // at same speed. So if  link 2 of each populated socket is connected to link 0 or 1 at the peer side, then all links in the 
    // system will be forced to operate same speed. So to take advantage of the per link Si feature, the platform should have
    // link 2 connected to link 2 at the peer side.
    //

    // 
    // First if any of the links request for MAX_QPI_LINK_SPEED, or speed > MaxSpeed then set the speed for those links
    // to max allowed speed. Note that max allowed speed is system wide, not per link or per socket. Because Si
    // doesn't support per link max allowed speed, and we don't allow mixing of SKUs.
    // Note: The assumption is MaxSpeed and all speeds below it are assumed to be supported.
    // 
    for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
      if (SocketData->Cpu[Index].Valid == TRUE) {
        for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {       
          if (SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) {
            if (setup->qpi.PhyLinkPerPortSetting[Index].Phy[LinkIndex].QpiLinkSpeed > MaxSpeed) {
              setup->qpi.PhyLinkPerPortSetting[Index].Phy[LinkIndex].QpiLinkSpeed = MaxSpeed;
            }
          }
        }
      }
    }

    // 
    // Normalize speeds by:
    //       1. Making sure link 0 & 1 have same link speed, otherwise force the speed to minmum of the two.
    //       2. Making sure the peers have same link speed; otherwise select the minimum of the two.
    // Note that step 2 can cuase "chain reaction". For example, if there is a S0 link1- S1 link2 connection,
    // step will 2 can cause S0 link1's speed to change. That requires S0 link0 speed to be changed as well.
    // So we have to repeat step 1. That could cause further dependencies. So we need  to repeat step 1 & 2
    // until step they don't change any link's speed. Since during each occurance of mismatched speed the
    // minimum speed is selected, the "chain reaction" is guranteed to stop and the loop will break.
    // 
    do {
      LinkDependency = FALSE;

      // Make sure link 0 & 1 have same link speed; otherwise force the speed to minmum of the two.
      for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
        if (SocketData->Cpu[Index].Valid == TRUE) {
          if (SocketData->Cpu[Index].LinkData[0].Valid == TRUE && SocketData->Cpu[Index].LinkData[1].Valid == TRUE) {
            Speed0 = (UINT8)setup->qpi.PhyLinkPerPortSetting[Index].Phy[0].QpiLinkSpeed;
            Speed1 = (UINT8)setup->qpi.PhyLinkPerPortSetting[Index].Phy[1].QpiLinkSpeed;
            if (Speed0 != Speed1) {
              (Speed0 < Speed1) ? (Speed2 = Speed0) : (Speed2 = Speed1);
              setup->qpi.PhyLinkPerPortSetting[Index].Phy[0].QpiLinkSpeed = Speed2;
              setup->qpi.PhyLinkPerPortSetting[Index].Phy[1].QpiLinkSpeed = Speed2;
              LinkDependency = TRUE;
            }
          }
        }
      }

      // Make sure the peers have same link speed; otherwise select the minimum of the two.
      for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
        if (SocketData->Cpu[Index].Valid == TRUE) {
          for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {       
            if (SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) {
              pPeerInfo = &(SocketData->Cpu[Index].LinkData[LinkIndex]);
              Speed0 = (UINT8)setup->qpi.PhyLinkPerPortSetting[Index].Phy[LinkIndex].QpiLinkSpeed;
              Speed1 = (UINT8)setup->qpi.PhyLinkPerPortSetting[pPeerInfo->PeerSocId].Phy[pPeerInfo->PeerPort].QpiLinkSpeed;
              if (Speed0 != Speed1) {
                (Speed0 < Speed1) ? (Speed2 = Speed0) : (Speed2 = Speed1);
                setup->qpi.PhyLinkPerPortSetting[Index].Phy[LinkIndex].QpiLinkSpeed = Speed2;
                setup->qpi.PhyLinkPerPortSetting[pPeerInfo->PeerSocId].Phy[pPeerInfo->PeerPort].QpiLinkSpeed = Speed2;
                LinkDependency = TRUE;
              }
            }
          }
        }
      }
    }while (LinkDependency == TRUE);

    // Print the normalized per link speed.
    for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
      if (SocketData->Cpu[Index].Valid == TRUE) {
         for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {       
           if (SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) {
             QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Socket %u Link %u QPI Freq is %u GT.", Index, LinkIndex, ((setup->qpi.PhyLinkPerPortSetting[Index].Phy[LinkIndex].QpiLinkSpeed +7)*8)));
           }
         }
      }
    }
  }

  //
  // If the requested speed is not per link, copy the selected speed in to the output structure and replicate it into the per link setup structure.
  //
  if (QpiInternalGlobal->QpiRunAdaptation == TRUE || host->var.qpi.OutQpiLinkSpeed != FREQ_PER_LINK) {
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  Selected QPI Freq is %u GT.", ((LinkSpeed+7)*8)));
    host->var.qpi.OutQpiLinkSpeed = LinkSpeed;  
    for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
      if (SocketData->Cpu[Index].Valid == TRUE) {
        for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {
          setup->qpi.PhyLinkPerPortSetting[Index].Phy[LinkIndex].QpiLinkSpeed = LinkSpeed;
        }  
      }
    }
  }

  //
  // Update the host output per link speed entry with the selected speed
  //
  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    if (SocketData->Cpu[Index].Valid == TRUE) {
      for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {
        host->var.qpi.OutPerLinkSpeed[Index][LinkIndex] = (UINT8)setup->qpi.PhyLinkPerPortSetting[Index].Phy[LinkIndex].QpiLinkSpeed;

        //
        // 4970550: HSX Clone : [HSX D0 PO] QPI EX : Remove L0p @ 9.6G QPI - csiLinkTrainTest Hard resets is causing CRCs on SS2A, 8B crc config
        // 4971062: BDX-EX QPI: Disable L0p at 9.6GT/s for all EX 
        //
        if ((host->var.qpi.OutPerLinkSpeed[Index][LinkIndex] == SPEED_REC_96GT) &&
            (((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType == CPU_HSX_EX)) ||
            ((host->var.common.cpuType == CPU_BDX) && (host->var.common.cpuSubType == CPU_BDX_EX)))) {
          QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  SelectSupportedLinkSpeed: Disabling L0p b/c of 9.6GT/s on Socket%d Port%d", Index, LinkIndex));
          host->var.qpi.OutQpiLinkL0pEn = 0;
        } // if HSX_EX or BDX_EX and at 9.6GT/s speed

      }  
    }
  }

  return QPI_SUCCESS;
}


UINT8
GetMaxSupportedLinkSpeed (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData
  )
/*++
Routine Description:
   Routine to determine and select the maximum QPI link speed supported by all populated CPUs. It is determined by
   Min(Max link speed of all populated CPU sockets). 

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Socket specific data structure

Returns:
   Max Link speed supported by all populated sockets

--*/
{
  UINT8                    SupportedLinkSpeeds = 0;
  UINT8                    LinkSpeed = SPEED_REC_64GT;
  UINT8                    Index; 
  CAPID2_PCU_FUN3_STRUCT   Capid2;

  //
  // Select the highest common supported freq
  //
  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    if (SocketData->Cpu[Index].Valid == TRUE) {
      //
      // Get common supported freqs by ORing all DISABLED freqs together.   Zero bits indicate supported freqs
      //
      Capid2.Data = QpiReadPciCfg (host, Index, 0, CAPID2_PCU_FUN3_REG);
      SupportedLinkSpeeds |= Capid2.Bits.qpi_allowed_cfclk_ratio_dis;
    }
  }

  if (host->var.qpi.OutQpiLinkSpeed == MAX_LIMITED_QPI_LINK_SPEED) {
    // Max Limited QPI Link Speed Option will limit the maximum speed to at most 8.0GT/s until higher speeds are validated
    SupportedLinkSpeeds |= 0x18;
  }

  for (Index = 5; Index > 0; --Index) {
    if (((SupportedLinkSpeeds >> Index) & 1) == 0) {
      switch (Index) {
        case 1:  LinkSpeed = SPEED_REC_72GT;
                 break;
        case 2:  LinkSpeed = SPEED_REC_80GT;
                 break;
        case 4:  LinkSpeed = SPEED_REC_96GT;
                 break;
        default: continue;                         
      }        
      break;
    }   //if
  }  // for

  return LinkSpeed;
}

QPI_STATUS
CheckLinkSpeedSupported (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  UINT8               RequestedLinkSpeed
  )
/*++

Routine Description:
   Verifies the selected QPI freq is valid for the platform
   If valid, return == QPI_SUCCESS
   

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Socket specific data structure
  RequestedLinkSpeed - reuqested speed for QPI links

Returns:
   QPI_SUCCESS if valid speed
   QPI_FAILURE if invalid speed

--*/
{
  UINT8                    SupportedLinkSpeeds = 0;
  UINT8                    Index;
  CAPID2_PCU_FUN3_STRUCT   Capid2;

  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {

    if (SocketData->Cpu[Index].Valid == TRUE) {
      
      //
      // Get common supported freqs by ORing all DISABLED freqs together.   Zero bits indicate unsupported freqs
      //
      Capid2.Data = QpiReadPciCfg (host, Index, 0, CAPID2_PCU_FUN3_REG);
      QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n;  qpi_allowed_cfclk_ratio_dis of Processor %u == 0x%x", Index, Capid2.Bits.qpi_allowed_cfclk_ratio_dis));
      SupportedLinkSpeeds |= Capid2.Bits.qpi_allowed_cfclk_ratio_dis;

    }
  }

   //
   // See if requested freq is supported
   //
  switch (RequestedLinkSpeed) {
    case SPEED_REC_72GT: Index = 1;
                           break;
    case SPEED_REC_80GT: Index = 2;
                           break;
    case SPEED_REC_96GT: Index = 4;
                           break;
    default: Index = 0xFF;

  }

  if ((RequestedLinkSpeed == SPEED_REC_64GT) || ((Index != 0xFF) && (((SupportedLinkSpeeds >> Index) & 1) == 0))) {
    //
    // Selected freq is valid
    //
    return QPI_SUCCESS;
  } else {
    //
    // Selected freq is invalid
    //
    return QPI_FAILURE;
  }

}


QPI_STATUS
NormalizeLinkOptions (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  )
/*++
Routine Description:
   Routine to normalize the phy/link layer options so that the values to be programmed at both ends of 
   link is agreeable.

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Socket specific data structure
  QpiInternalGlobal - QPIRC internal variables.

Returns:


--*/
{
  BOOLEAN                  Override;
  UINT8                    Index, LinkIndex;
  QPI_LINK_DATA            *pPeerInfo;
  struct sysSetup          *setup;

  setup = (struct sysSetup *) &host->setup;

  // 
  // Normalize per link options. If the peers don't match their options, they will be normalized as follows:
  //   QpiLinkCreditReduce  - Force to normal credit allocation.
  //   QpiProbeType  - Force to no-probe mode.
  //   QpiConfigTxWci  - Force to full width mode.
  // 

  Override = FALSE;
  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    if (SocketData->Cpu[Index].Valid == TRUE) {
      for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {       
        if (SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) {
          pPeerInfo = &(SocketData->Cpu[Index].LinkData[LinkIndex]);
          if (setup->qpi.PhyLinkPerPortSetting[Index].Link[LinkIndex].QpiPortDisable !=
              setup->qpi.PhyLinkPerPortSetting[pPeerInfo->PeerSocId].Link[pPeerInfo->PeerPort].QpiPortDisable) {
            setup->qpi.PhyLinkPerPortSetting[Index].Link[LinkIndex].QpiPortDisable = TRUE;
            setup->qpi.PhyLinkPerPortSetting[pPeerInfo->PeerSocId].Link[pPeerInfo->PeerPort].QpiPortDisable = TRUE;
            Override = TRUE;
          }
          if (setup->qpi.PhyLinkPerPortSetting[Index].Link[LinkIndex].QpiLinkCreditReduce != setup->qpi.PhyLinkPerPortSetting[pPeerInfo->PeerSocId].Link[pPeerInfo->PeerPort].QpiLinkCreditReduce) {
            setup->qpi.PhyLinkPerPortSetting[Index].Link[LinkIndex].QpiLinkCreditReduce = 0;
            setup->qpi.PhyLinkPerPortSetting[pPeerInfo->PeerSocId].Link[pPeerInfo->PeerPort].QpiLinkCreditReduce = 0;
            Override = TRUE;
          }
          if (setup->qpi.PhyLinkPerPortSetting[Index].Phy[LinkIndex].QpiProbeType != setup->qpi.PhyLinkPerPortSetting[pPeerInfo->PeerSocId].Phy[pPeerInfo->PeerPort].QpiProbeType) {
            setup->qpi.PhyLinkPerPortSetting[Index].Phy[LinkIndex].QpiProbeType = PROBE_TYPE_NO_PROBE;
            setup->qpi.PhyLinkPerPortSetting[pPeerInfo->PeerSocId].Phy[pPeerInfo->PeerPort].QpiProbeType = PROBE_TYPE_NO_PROBE;
            Override = TRUE;
          }
        }
      }
    }
  }

  if (Override == TRUE) {
    QpiLogWarning(host, 0xFF, 0xFF, 0xFF, WARN_PER_LINK_OPTION_MISMATCH);
    QpiDebugPrintWarn((host, QPI_DEBUG_WARN, "\n; WARNING:   One or more per Link option mismatch detected. Forcing to common setting.  \n"));
  }


  return QPI_SUCCESS;
}

BOOLEAN
DisableFeatureWhenQpiProbeInstalled (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData
  )
/*++

Routine Description:
   Returns TRUE if probe installed.  1.1 features must be disabled when probing

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Socket specific data structure

Returns:
   TRUE   Probe installed/need disable feature
   FALSE  No probe installed that requires feature disable

--*/
{

  UINT8 Index;
  UINT8 LinkIndex;

  //
  // Check for probe installed anywhere in system so can globally disable 1.1 features
  //
  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {

    if (SocketData->Cpu[Index].Valid == TRUE) {
    
      for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {
        
        if (SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) {
        
          //
          // Check for each probe type individually as may only need to do this for PROBE_TYPE_COHASSET
          //                  
          if (host->setup.qpi.PhyLinkPerPortSetting[Index].Phy[LinkIndex].QpiProbeType == PROBE_TYPE_COHASSET) {  
            return TRUE;
          }  
        }
      }
    }
  }

  return FALSE;
  
}

BOOLEAN
IsQpiProbeInstalled (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData
  )
/*++

Routine Description:
   Returns TRUE if probe installed.

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Socket specific data structure

Returns:
   TRUE   Probe installed/need disable feature
   FALSE  No probe installed that requires feature disable

--*/
{

  UINT8 Index;
  UINT8 LinkIndex;

  //
  // Check for probe installed anywhere in system 
  //
  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {

    if (SocketData->Cpu[Index].Valid == TRUE) {
    
      for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {
        
        if (SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) {
        
          if (host->setup.qpi.PhyLinkPerPortSetting[Index].Phy[LinkIndex].QpiProbeType != PROBE_TYPE_NO_PROBE) {  
            return TRUE;
          }  
        }
      }
    }
  }

  return FALSE;
  
}

QPI_STATUS
IssuePhyResetOnCpuLink(
  struct sysHost      *host,
  UINT8               Cpu,
  UINT8               Link,
  UINT8               ResetType
  )
{
  QPIREUT_PH_CTR_QPIAGENT_REUT_HSX_BDX_STRUCT  QpiReutPhCtr;
  QPIREUT_PH_PIS_QPIAGENT_REUT_HSX_BDX_STRUCT  QpiReutPhPis;
  UINT8                         WaitTryCount;
  UINT8                         OrigRst;
  QPI_STATUS                    Status;

  // Issue the phy layer reset with the reset modifier set to the given type
  QpiReutPhCtr.Data = QpiReadPciCfg (host, Cpu,  Link,  QPIREUT_PH_CTR_QPIAGENT_REUT_REG);
  QpiReutPhCtr.Bits.phylayerreset = 1;
  OrigRst = (UINT8)QpiReutPhCtr.Bits.resetmodifier;
  if(ResetType != 0xFF){                         //if is 0xFF, don't program it and leave alone.
    QpiReutPhCtr.Bits.resetmodifier = ResetType;     
  }    
  QpiWritePciCfg (host, Cpu,  Link,  QPIREUT_PH_CTR_QPIAGENT_REUT_REG, QpiReutPhCtr.Data);

  // Wait for the link to train
  for (WaitTryCount = 0; WaitTryCount <= 5; WaitTryCount++) {
    QpiReutPhPis.Data = QpiReadPciCfg (host, Cpu,  Link,  QPIREUT_PH_PIS_QPIAGENT_REUT_REG);
    if ((QpiReutPhPis.Bits.txstatetracker == CSIPHYPORT_STATE_L0) && (QpiReutPhPis.Bits.rxstatetracker == CSIPHYPORT_STATE_L0)) {
      break;
    }    
    QpiFixedDelay(host, 100); 
  }

  // Check if the link trained or not
  if (WaitTryCount > 5) {
    Status = QPI_FAILURE;
    QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  Link faiked to train after PhyReset."));
  } else {
    Status = QPI_SUCCESS;
  }

  // Restore the reset modified state
  QpiReutPhCtr.Data = QpiReadPciCfg (host, Cpu,  Link,  QPIREUT_PH_CTR_QPIAGENT_REUT_REG);
  QpiReutPhCtr.Bits.resetmodifier = OrigRst;
  QpiWritePciCfg (host, Cpu,  Link,  QPIREUT_PH_CTR_QPIAGENT_REUT_REG, QpiReutPhCtr.Data);     
  
  return Status;
}

QPI_STATUS
ProgramLateActionOnCpuLink(
   struct sysHost           *host,
   UINT8                    Cpu,
   UINT8                    Link,
   UINT32               Value
)
{
  QPIREUT_PH_CTR_QPIAGENT_REUT_HSX_BDX_STRUCT  QpiReutPhCtr;

  QpiReutPhCtr.Data = QpiReadPciCfg (host, Cpu,  Link,  QPIREUT_PH_CTR_QPIAGENT_REUT_REG);
  QpiReutPhCtr.Bits.lateactloaddis = Value;
  QpiWritePciCfg (host, Cpu,  Link,  QPIREUT_PH_CTR_QPIAGENT_REUT_REG, QpiReutPhCtr.Data);  
  
  return QPI_SUCCESS;
}

QPI_STATUS
ProgramPhyLayerOnCpuLink (
  struct sysHost         *host,
  QPI_SOCKET_DATA        *SocketData,
  UINT8                  Index,
  UINT8                  LinkIndex,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  )
  /*++

Routine Description:
   Programs PHY layer registers

Arguments:
   host  - Pointer to the system host (root) structure
   SocketData - Socket specific data structure
   Index - socket Index
   LinkIndex - qpi port number

Returns:

--*/
{
  // QPIRAS_TODO: get rid of magic number "128", also in boot-time function too
  UINT8   LinkSpeedParameter[128];
  UINT8   LinkHalfSpeedParameter[128];
  BOOLEAN Qpi11FeatureDisable;

  //Assume if support hot-adding, no probles in the system. Otherwise, it will need to re-program/affect other links which is onlining now 
  //becasue some control register are shared by both ports( port 0 and port1).
  Qpi11FeatureDisable = FALSE;  
  
  // Locate EPARAM table for Link
  GetSocketLinkEparams(host, SocketData, Index, LinkIndex, &LinkSpeedParameter, &LinkHalfSpeedParameter);
  ConfigSocketLinkEparams(host, SocketData, Index, LinkIndex, &LinkSpeedParameter, &LinkHalfSpeedParameter);


  ConfigSocketLinkPhyMisc(host, SocketData, QpiInternalGlobal, Index, LinkIndex, Qpi11FeatureDisable);
 
  return QPI_SUCCESS;
}

