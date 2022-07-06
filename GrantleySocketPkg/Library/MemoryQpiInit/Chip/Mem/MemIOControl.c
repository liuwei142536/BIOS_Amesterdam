/**
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
**/
/*************************************************************************
 *
 * Memory Reference Code
 *
 * ESS - Enterprise Silicon Software
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2006 - 2016 Intel Corporation All Rights Reserved.
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
 ************************************************************************
 *
 *  PURPOSE:
 *
 *      This file contains memory detection and initialization for
 *      IMC and DDR3 modules compliant with JEDEC specification.
 *
 ************************************************************************/

#include "SysFunc.h"

//
// Internal data types
//
#define  TX_DQ_XOVER_2TO2_SETUP_TIME    24
#define  TX_PER_BIT_SETUP_2TO2_OFFSET   35

const UINT32 ddrioOffsetTable[MAX_STROBE] = {0, 0x200, 0x400, 0x600, 0x800, 0xA00, 0xC00, 0, 0x200, 0x100, 0x300, 0x500, 0x700,
                                             0x900, 0xB00, 0xD00, 0x100, 0x300};

const UINT32 rxVrefCtrlTable[MAX_STROBE] = {RXVREFCTRLN0_0_MCIO_DDRIO_REG,
                                            RXVREFCTRLN0_0_MCIO_DDRIO_REG + 0x200,
                                            RXVREFCTRLN0_0_MCIO_DDRIO_REG + 0x400,
                                            RXVREFCTRLN0_0_MCIO_DDRIO_REG + 0x600,
                                            RXVREFCTRLN0_0_MCIO_DDRIO_REG + 0x800,
                                            RXVREFCTRLN0_0_MCIO_DDRIO_REG + 0xA00,
                                            RXVREFCTRLN0_0_MCIO_DDRIO_REG + 0xC00,
                                            RXVREFCTRLN0_7_MCIO_DDRIOEXT_REG + 0,
                                            RXVREFCTRLN0_7_MCIO_DDRIOEXT_REG + 0x200,
                                            RXVREFCTRLN1_0_MCIO_DDRIO_REG,
                                            RXVREFCTRLN1_0_MCIO_DDRIO_REG + 0x200,
                                            RXVREFCTRLN1_0_MCIO_DDRIO_REG + 0x400,
                                            RXVREFCTRLN1_0_MCIO_DDRIO_REG + 0x600,
                                            RXVREFCTRLN1_0_MCIO_DDRIO_REG + 0x800,
                                            RXVREFCTRLN1_0_MCIO_DDRIO_REG + 0xA00,
                                            RXVREFCTRLN1_0_MCIO_DDRIO_REG + 0xC00,
                                            RXVREFCTRLN1_7_MCIO_DDRIOEXT_REG + 0,
                                            RXVREFCTRLN1_7_MCIO_DDRIOEXT_REG + 0x200
                                            };
const UINT32 rxOffsetTable[MAX_STROBE] = {RXOFFSETN0RANK0_0_MCIO_DDRIO_REG,
                                          RXOFFSETN0RANK0_0_MCIO_DDRIO_REG + 0x200,
                                          RXOFFSETN0RANK0_0_MCIO_DDRIO_REG + 0x400,
                                          RXOFFSETN0RANK0_0_MCIO_DDRIO_REG + 0x600,
                                          RXOFFSETN0RANK0_0_MCIO_DDRIO_REG + 0x800,
                                          RXOFFSETN0RANK0_0_MCIO_DDRIO_REG + 0xA00,
                                          RXOFFSETN0RANK0_0_MCIO_DDRIO_REG + 0xC00,
                                          RXOFFSETN0RANK0_7_MCIO_DDRIOEXT_REG + 0,
                                          RXOFFSETN0RANK0_7_MCIO_DDRIOEXT_REG + 0x200,
                                          RXOFFSETN1RANK0_0_MCIO_DDRIO_REG,
                                          RXOFFSETN1RANK0_0_MCIO_DDRIO_REG + 0x200,
                                          RXOFFSETN1RANK0_0_MCIO_DDRIO_REG + 0x400,
                                          RXOFFSETN1RANK0_0_MCIO_DDRIO_REG + 0x600,
                                          RXOFFSETN1RANK0_0_MCIO_DDRIO_REG + 0x800,
                                          RXOFFSETN1RANK0_0_MCIO_DDRIO_REG + 0xA00,
                                          RXOFFSETN1RANK0_0_MCIO_DDRIO_REG + 0xC00,
                                          RXOFFSETN1RANK0_7_MCIO_DDRIOEXT_REG + 0,
                                          RXOFFSETN1RANK0_7_MCIO_DDRIOEXT_REG + 0x200
                                          };
const UINT32 dataTrainFeedbackTable[MAX_STROBE] = {DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG,
                                                   DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG + 0x200,
                                                   DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG + 0x400,
                                                   DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG + 0x600,
                                                   DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG + 0x800,
                                                   DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG + 0xA00,
                                                   DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG + 0xC00,
                                                   DATATRAINFEEDBACKN0_7_MCIO_DDRIOEXT_REG + 0,
                                                   DATATRAINFEEDBACKN0_7_MCIO_DDRIOEXT_REG + 0x200,
                                                   DATATRAINFEEDBACKN1_0_MCIO_DDRIO_REG,
                                                   DATATRAINFEEDBACKN1_0_MCIO_DDRIO_REG + 0x200,
                                                   DATATRAINFEEDBACKN1_0_MCIO_DDRIO_REG + 0x400,
                                                   DATATRAINFEEDBACKN1_0_MCIO_DDRIO_REG + 0x600,
                                                   DATATRAINFEEDBACKN1_0_MCIO_DDRIO_REG + 0x800,
                                                   DATATRAINFEEDBACKN1_0_MCIO_DDRIO_REG + 0xA00,
                                                   DATATRAINFEEDBACKN1_0_MCIO_DDRIO_REG + 0xC00,
                                                   DATATRAINFEEDBACKN1_7_MCIO_DDRIOEXT_REG + 0,
                                                   DATATRAINFEEDBACKN1_7_MCIO_DDRIOEXT_REG + 0x200
                                                   };
const UINT32 txXtalkTable[MAX_STROBE] = {TXXTALKN0_0_MCIO_DDRIO_REG,
                                         TXXTALKN0_0_MCIO_DDRIO_REG + 0x200,
                                         TXXTALKN0_0_MCIO_DDRIO_REG + 0x400,
                                         TXXTALKN0_0_MCIO_DDRIO_REG + 0x600,
                                         TXXTALKN0_0_MCIO_DDRIO_REG + 0x800,
                                         TXXTALKN0_0_MCIO_DDRIO_REG + 0xA00,
                                         TXXTALKN0_0_MCIO_DDRIO_REG + 0xC00,
                                         TXXTALKN0_7_MCIO_DDRIOEXT_REG + 0,
                                         TXXTALKN0_7_MCIO_DDRIOEXT_REG + 0x200,
                                         TXXTALKN1_0_MCIO_DDRIO_REG,
                                         TXXTALKN1_0_MCIO_DDRIO_REG + 0x200,
                                         TXXTALKN1_0_MCIO_DDRIO_REG + 0x400,
                                         TXXTALKN1_0_MCIO_DDRIO_REG + 0x600,
                                         TXXTALKN1_0_MCIO_DDRIO_REG + 0x800,
                                         TXXTALKN1_0_MCIO_DDRIO_REG + 0xA00,
                                         TXXTALKN1_0_MCIO_DDRIO_REG + 0xC00,
                                         TXXTALKN1_7_MCIO_DDRIOEXT_REG + 0,
                                         TXXTALKN1_7_MCIO_DDRIOEXT_REG + 0x200
                                         };
/*
const UINT32 dataTrainFeedbackTable[MAX_STROBE] = {DATAOFFSETTRAIN_MCIO_DDRIO_HSX_REG,
                                                   DATAOFFSETTRAIN_MCIO_DDRIO_HSX_REG + 0x200,
                                                   DATAOFFSETTRAIN_MCIO_DDRIO_HSX_REG + 0x400,
                                                   DATAOFFSETTRAIN_MCIO_DDRIO_HSX_REG + 0x600,
                                                   DATAOFFSETTRAIN_MCIO_DDRIO_HSX_REG + 0x800,
                                                   DATAOFFSETTRAIN_MCIO_DDRIO_HSX_REG + 0xA00,
                                                   DATAOFFSETTRAIN_MCIO_DDRIO_HSX_REG + 0xC00,
                                                   DATAOFFSETTRAIN_MCIO_DDRIOEXT_HSX_REG + 0,
                                                   DATAOFFSETTRAIN_MCIO_DDRIOEXT_HSX_REG + 0x200,
                                                   DATAOFFSETTRAIN_MCIO_DDRIO_HSX_REG,
                                                    + 0x200,
                                                    + 0x400,
                                                    + 0x600,
                                                    + 0x800,
                                                    + 0xA00,
                                                    + 0xC00,
                                                   REG + 0,
                                                   REG + 0x200
                                                   };
 */
extern UINT32 CmdCtlAddressConvertTable[2][MAX_CH];
extern UINT32 ClkAddressConvertTable[MAX_CH];

//
// Local Prototypes
//
UINT32 ProgramIOCompValues(PSYSHOST host, UINT8 socket);
MRC_STATUS GetSetTxDelayBit(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, UINT8 bit, GSM_LT level,
             GSM_GT group, UINT8 mode, INT16 *value);
MRC_STATUS GetSetTxVrefDDR4(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, UINT8 bit, GSM_LT level,
             GSM_GT group, UINT8 mode, INT16 *value);
MRC_STATUS GetSetTxVrefFnv(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, UINT8 bit, GSM_LT level,
             GSM_GT group, UINT8 mode, INT16 *value);
MRC_STATUS GetSetImode (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 strobe, UINT8 mode, INT16 *value);
MRC_STATUS GetSetCTLE (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, UINT8 bit, GSM_LT level,
             GSM_GT group, UINT8 mode, INT16 *value);
MRC_STATUS GetSetCPUODT (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, UINT8 bit, GSM_LT level,
             GSM_GT group, UINT8 mode, INT16 *value);
MRC_STATUS GetSetDIMMODT (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, UINT8 bit, GSM_LT level,
             GSM_GT group, UINT8 mode, INT16 *value);
UINT32 CacheDDRIO(PSYSHOST host, UINT8 socket);
UINT32 MemWriteDimmVref(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, GSM_LT level, UINT8 vref);
UINT32 UpdateIoRegister2 (PSYSHOST host, UINT8 strobe, UINT32 regOffset);

UINT16 GetTwoTwoLogicDelay (PSYSHOST host, TXXTALKN0_0_MCIO_DDRIO_STRUCT* txGroup2, UINT8 rank, UINT8 bit, UINT8 debug);
void   SetTwoTwoLogicDelay (PSYSHOST host, TXXTALKN0_0_MCIO_DDRIO_STRUCT* txGroup2, UINT8 rank, UINT8 bit, UINT16 Value, UINT8 debug);
UINT8  GetTwoTwoLogicDelaysAllSet (PSYSHOST host, TXXTALKN0_0_MCIO_DDRIO_STRUCT* txGroup2, UINT8 rank, UINT8 debug);

#ifdef SERIAL_DBG_MSG
char   *GetGroupStr(GSM_GT group, char *strBuf);
#endif  // SERIAL_DBG_MSG


/**

  Initialize the DDRIO interface

  @param host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
InitDdrioInterface (
  PSYSHOST  host
  )
{
  UINT8                                             socket;
  UINT8                                             ch;
  UINT8                                             MaxCh;
  UINT8                                             dimm;
  UINT8                                             rank;
  UINT8                                             MaxDpc;
  UINT8                                             strobe;
  UINT8                                             txVref = 0;
  INT16                                             rxVref;
  INT16                                             rxDqDelay;
  UINT32                                            tempReg = 0;
  UINT8                                             NumDimmsPoped;
  struct channelNvram                               (*channelNvList)[MAX_CH];
  struct dimmNvram                                  (*dimmNvList)[MAX_DIMM];
  struct ddrRank                                    (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                                 (*rankStruct)[MAX_RANK_DIMM];
  struct ddr4OdtValueStruct                         *ddr4OdtValuePtr = NULL;
  DATACONTROL0_0_MCIO_DDRIO_STRUCT                  dataControl0;
  DATACONTROL1_0_MCIO_DDRIO_BDX_STRUCT              dataControl1;
  DATACONTROL2_0_MCIO_DDRIO_BDX_STRUCT              dataControl2;
  DATACONTROL4_0_MCIO_DDRIO_BDX_STRUCT              dataControl4;
  DDRCRCLKCONTROLS_MCIO_DDRIOEXT_HSX_STRUCT         ddrCRClkControls;
  DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_HSX_STRUCT    ddrCRCmdControls;
  DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_HSX_STRUCT     ddrCRCtlControls;
  DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXTMCC_HSX_STRUCT ddrCrDimmVrefControl1Hsx;
  DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXTMCC_BDX_STRUCT ddrCrDimmVrefControl1Bdx;
  DDRCRCLKCOMP_CMP_MCIO_DDRIOEXTMCC_STRUCT          ddrCrClkCompCmp;
  COMPDATA0_0_MCIO_DDRIO_HSX_STRUCT                 compData0Hsx;
  COMPDATA0_0_MCIO_DDRIO_BDX_STRUCT                 compData0Bdx;
  COMPDATA1_0_MCIO_DDRIO_HSX_STRUCT                 compData1Hsx;
  COMPDATA1_0_MCIO_DDRIO_BDX_STRUCT                 compData1Bdx;
  DDRCRCOMPOVR_MCIO_DDRIOEXTMCC_BDX_STRUCT          ddrCrCompOvr;
  DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXT_BDX_STRUCT    ddrCRCtlControls1;
  DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_STRUCT             ddrCRCompCtl0;
  DDRCRCMDCONTROLS1_CMDN_MCIO_DDRIOEXT_BDX_STRUCT   ddrCRCmdControls1;
  DDRCRCOMPVSSHICONTROL_MCIO_DDRIOEXTMCC_STRUCT     ddrCrCompVssHiControl;
  DATACONTROL3_0_MCIO_DDRIO_STRUCT                  dataControl3;
  RXOFFSETN0RANK0_0_MCIO_DDRIO_STRUCT               rxOffset;
  VSSHIORVREFCONTROL_0_MCIO_DDRIO_STRUCT            vssControl;
  DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_BDX_STRUCT    ddrCRCtlRanksUsed;
  DDRCRCOMPCTL1_MCIO_DDRIOEXTMCC_STRUCT             ddrCRCompCtl1;
  DDRCRCOMPCTL2_MCIO_DDRIOEXTMCC_STRUCT             ddrCRCompCtl2;
  DDRCRCOMPCTL3_MCIO_DDRIOEXTMCC_STRUCT             ddrCRCompCtl3;
  DDRCRCLKCOMP_CLK_MCIO_DDRIOEXT_STRUCT             ddrCRClkComp;
  DDRCRCMDCOMP_CMDN_MCIO_DDRIOEXT_STRUCT            ddrCRCmdComp;
  DDRCRCMDCOMP_MCIO_DDRIOEXTMCC_STRUCT              ddrCRCmdCompmc;
  DDRCRCTLCOMP_MCIO_DDRIOEXTMCC_STRUCT              ddrCRCtlCompmc;
  DDRCRCTLCOMP_CKE_MCIO_DDRIOEXT_STRUCT             ddrCRCtlComp;
  DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_BDX_STRUCT        ddrCRClkRanksUsed;
  DDRCRCOMPVSSHI_MCIO_DDRIOEXTMCC_STRUCT            ddrCRCompVssHi;
  DDRCRDIMMVREFCONTROL2_MCIO_DDRIOEXTMCC_STRUCT     ddrCrDimmVrefControl2;
  DDRCRSPDCFG1_MCIO_DDRIOEXTMCC_STRUCT              ddrCRSPDCfg1;
  DDRCRSPDCFG2_MCIO_DDRIOEXTMCC_STRUCT              ddrCRSPDCfg2;
  DDRCRCOMPVSSHI1_MCIO_DDRIOEXTMCC_STRUCT           ddrCRCompVssHibdx;
  DDRCRCOMPVSSHICONTROL1_MCIO_DDRIOEXTMCC_STRUCT    VssHiorVrefcontrol1;
  DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_BDX_STRUCT        save_ddrCRClkRanksUsed[MAX_CH];
  MCMTR_MC_MAIN_HSX_BDX_STRUCT                     mcMtr;
  UINT8                                            delta;
  UINT16                                           VssHi; // Target VssHi Voltage
  UINT16                                           Qclkps;
  UINT16                                           VccIomV;
  UINT16                                           Vdd;
  UINT8                                            userVdd;
  UINT16                                           VssHiSwingTarget;
  UINT32                                           Itarget;
  UINT32                                           IcompUp;
  UINT32                                           IcompDn;
  UINT16                                           rcmdref_vsshi_up_res;
  UINT16                                           rcmdref_vsshi_dn_res;
  UINT8                                            legs;
  UINT16                                           mcodt;
  UINT8                                            skipDdrioInit = 1;

  delta              = 15;   // VssHi change voltage during panic: 15mV
  VccIomV            = 1000; // Assume 1.0 volts
  VssHiSwingTarget   = 950;  // VssHi target voltage in mV
  mcodt              = 0;

  socket = host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return SUCCESS;

  channelNvList = &host->nvram.mem.socket[socket].channelList;
  
  //
  // Return if all the channels are disabled on this socket
  //
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    skipDdrioInit = 0;
    break;  //exiting after the first enabled channel
  }
  if(skipDdrioInit == 1) return SUCCESS;

  rxDqDelay = 1;

  //
  // Disable non-existent DDR CH 2 and 3 in BDX-DE and BDX-EP 2HA so RCOMP engine doesn't
  // generate RCOMP to these CH
  //
  if (IsDESku(host)) {
    mcMtr.Data = MemReadPciCfgMC (host, socket, 0, MCMTR_MC_MAIN_REG);
    mcMtr.Bits.chn_disable = 0xc;
    MemWritePciCfgMC (host, socket, 0, MCMTR_MC_MAIN_REG, mcMtr.Data);
  } else if (IsBDXEP(host) && Is2HA(host)) {
    mcMtr.Data = MemReadPciCfgMC (host, socket, 0, MCMTR_MC_MAIN_REG);
    mcMtr.Bits.chn_disable = 0xc;
    MemWritePciCfgMC (host, socket, 0, MCMTR_MC_MAIN_REG, mcMtr.Data);
    mcMtr.Data = MemReadPciCfgMC (host, socket, 1, MCMTR_MC_MAIN_REG);
    mcMtr.Bits.chn_disable = 0xc;
    MemWritePciCfgMC (host, socket, 1, MCMTR_MC_MAIN_REG, mcMtr.Data);
  }

  //
  // Misc DDRIO programming
  //

  rxOffset.Data = 0;
  rxOffset.Bits.saoffset0 = 15;
  rxOffset.Bits.saoffset1 = 15;
  rxOffset.Bits.saoffset2 = 15;
  rxOffset.Bits.saoffset3 = 15;

  vssControl.Data = 0;

  if(host->var.common.cpuType == CPU_HSX) {
    switch (host->nvram.mem.socket[socket].ddrVoltage) {
      case SPD_VDD_150:
        vssControl.Data = 54; // codetarget
        break;
      case SPD_VDD_135:
        vssControl.Data = 44; // codetarget
        break;
      case SPD_VDD_120:
        vssControl.Data = 30; // codetarget
        break;
    }
    vssControl.Data |= 0 << 6; // HiBWDivider
    vssControl.Data |= 0 << 8; // LoBWDivider
    vssControl.Data |= 0 << 10; // SampleDivider
    vssControl.Data |= 0 << 13; // OpenLoop
    vssControl.Data |= 0 << 14; // BWError
    vssControl.Data |= 1 << 16; // PanicEn
    vssControl.Data |= 1 << 17; // CloseLoop
    if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
      vssControl.Data |= 3 << 18; // PanicVoltage
    } else {
      vssControl.Data |= 5 << 18; // PanicVoltage
    }
    vssControl.Data |= 1 << 22; // GainBoost
    vssControl.Data |= 0 << 23; // SelCode
  }

  dataControl1.Data = 0;
  dataControl2.Data = 0;
  dataControl3.Data = 0;
  dataControl4.Data = 0;
  ddrCRClkControls.Data = 0;
  ddrCRClkComp.Data = 0;
  ddrCRCmdComp.Data = 0;
  ddrCRCtlComp.Data = 0;
  ddrCRCompVssHi.Data = 0;
  ddrCRCtlControls.Data = 0;
  ddrCrCompOvr.Data = 0;
  ddrCRCmdCompmc.Data = 0;
  ddrCRCtlCompmc.Data = 0;

  ddrCrCompOvr.Bits.dqtco   = 1;

  if (host->var.common.cpuType == CPU_HSX || (IsDESku(host) && CheckSteppingLessThan(host, CPU_BDX, C0_REV_BDX))) {
    ddrCrCompOvr.Bits.cmdsr   = 1;
    ddrCrCompOvr.Bits.ctlsr   = 1;
  }
  if (host->nvram.mem.dimmTypePresent == UDIMM || host->nvram.mem.dimmTypePresent == SODIMM) {
    ddrCrCompOvr.Bits.clkdrvu = 1;
    ddrCrCompOvr.Bits.clkdrvd = 1;
  }
  if (CheckSteppingLessThan(host, CPU_HSX, C0_REV_HSX)) {
    ddrCrCompOvr.Bits.cmdtco  = 1;
    ddrCrCompOvr.Bits.ctltco  = 1;
    ddrCrCompOvr.Bits.clktco  = 1;
  }

  if (host->var.common.cpuType == CPU_BDX){
    ddrCrCompOvr.Bits.cmdtco  = 1;
    ddrCrCompOvr.Bits.ctltco  = 1;
    ddrCrCompOvr.Bits.clktco  = 1;
  }

  dataControl1.Bits.odtdelay          = (UINT32)-2;
  dataControl1.Bits.senseampdelay     = (UINT32)-2;
  dataControl1.Bits.odtduration       = 7;  // 18 QCLK
  dataControl1.Bits.senseampduration  = 7;  // 18 QCLK
  dataControl1.Bits.rxbiasctl         = 4;  // 1.33Idll

  // Initialize
  compData0Hsx.Data = 0;
  compData1Hsx.Data = 0;
  compData0Bdx.Data = 0;
  compData1Bdx.Data = 0;

  if (host->var.common.cpuType == CPU_HSX) {
    compData0Hsx.Bits.vtcomp       = 5;
    compData0Hsx.Bits.rcompdrvup   = 0x13;
    compData0Hsx.Bits.rcompdrvdown = 0x13;
    compData0Hsx.Bits.slewratecomp = 9;

    compData1Hsx.Bits.panicdrvdn       = 1;
    compData1Hsx.Bits.panicdrvup       = 1;
    compData1Hsx.Bits.rcompodtup       = 0x10;
    compData1Hsx.Bits.rcompodtdown     = 0x10;
    compData1Hsx.Bits.levelshiftercomp = 1;
  } else if (host->var.common.cpuType == CPU_BDX) {
    compData0Bdx.Bits.vtcomp           = 5;
    compData0Bdx.Bits.rcompdrvup       = 0x13;
    compData0Bdx.Bits.rcompdrvdown     = 0x13;
    compData0Bdx.Bits.slewratecomp     = 9;
    compData0Bdx.Bits.levelshiftercomp = 1;

    compData1Bdx.Bits.panicdrvdn       = 1;
    compData1Bdx.Bits.panicdrvup       = 1;
    compData1Bdx.Bits.rcompodtup       = 0x10;
    compData1Bdx.Bits.rcompodtdown     = 0x10;
  }

  dataControl0.Data = 0;
  dataControl0.Bits.forceodton = 0;

  if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {//proc is A0
    if (host->nvram.mem.dramType == SPD_TYPE_DDR3)  dataControl0.Bits.forceodton = 1;
  }

  if (host->nvram.mem.socket[socket].ddrVoltage == SPD_VDD_150) {
    dataControl0.Bits.datavccddqhi  = 1;
  } else {
    dataControl0.Bits.datavccddqhi  = 0;
  }

  if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
    // This is a workaround. BIOS HSD 4166577
    if (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG))) {
      dataControl0.Bits.longpreambleenable = 1;
    }
  }
  dataControl0.Bits.rfon            = 0;

  if (host->var.common.cpuType == CPU_HSX) {
    if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
      dataControl0.Bits.rxpion = 1;
    } else {
      dataControl0.Bits.rxpion = 0;
    }
  } else {// BDX
    if (IsDESku(host)) { //DE
      dataControl0.Bits.rxpion = 1;
      if (CheckSteppingLessThan(host, CPU_BDX, C0_REV_BDX)) {
        dataControl0.Bits.txpion = 1;
      }
    } else { // EP
        dataControl0.Bits.rxpion = 1;
    }
  }

  dataControl0.Bits.txlong            = 1;

  // This is a workaround. We may set this to 0 in later steppings.
  if (CheckSteppingLessThan(host, CPU_HSX, C0_REV_HSX)) {
    dataControl0.Bits.internalclockson  = 1;
  } else {
    dataControl0.Bits.internalclockson  = 0;
  }

  if (host->var.common.cpuType == CPU_BDX) {
    dataControl0.Bits.internalclockson  = 1;
  }

  dataControl0.Bits.dcdetectmode      = 0;

  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
      dataControl2.Bits.cttermination = 1;
  }
  //dataControl2.Bits.imodebiasen   = 1;
  dataControl2.Bits.rxdqssaoffset = 0x10;
  dataControl2.Bits.imodeenable   = 1;

  // This is a workaround. BIOS HSD 4166093
  if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
    dataControl2.Bits.forcebiason     = 1;
  }

  //
  // Enable 12 legs
  //
  dataControl3.Bits.drvstaticlegcfg       = 3;
  dataControl3.Bits.odtstaticlegcfg       = 3;
  // HSD 4986302
  dataControl3.Bits.longodtr2w = 1;

  // Get   MCODT Value
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    dataControl3.Bits.odtsegmentenable    = GetMcOdtValue(host, socket, ch, &mcodt);
    break;  //exiting after the first enabled channel
  }
  // Count the number of legs that is enabled.
  for(legs = 0; legs < 3; legs++) {
    if(!(dataControl3.Bits.odtsegmentenable & (1 << legs))) break;
  }

  // if the ODT settings for this config failed, freeze!!!!!
  if (!legs) {
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "No ODT segments enabled. Halting MRC \n"));
    FatalError (host, 0, 0);
  }

  dataControl3.Bits.datasegmentenable     = 7;
  dataControl3.Bits.imodebiashighcm       = 0;
  dataControl3.Bits.imodebiasvrefen       = 0;
  dataControl3.Bits.imodebiasdfxddr3legup = 0;
  dataControl3.Bits.imodebiasdfxddr4legup = 0;
  dataControl3.Bits.imodebiasdfxlegdn     = 0;
  dataControl3.Bits.ddrcrctleresen        = 0;
  dataControl3.Bits.pcasbiasclosedloopen  = 1;
  dataControl3.Bits.rxbiassel             = 0;
  dataControl3.Bits.rxbiasfoldedlegtrim   = 0;
  dataControl3.Bits.rxbiasgcncomp         = 2;
  // HSD4166483: Remove for B0
  if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
    dataControl3.Bits.imodebiasrxbiastrim   = 4;
  } else {
    dataControl3.Bits.imodebiasrxbiastrim   = 1;
  }

  if (host->var.common.cpuType == CPU_BDX) {
    dataControl4.Bits.bdx_sys = 1;
  }

  dataControl4.Bits.xoveropmode = 1;
  if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
    if ((host->var.common.emulation) && (host->var.common.cpuType == CPU_BDX)) {
      dataControl4.Bits.dqsvmselogicdelay = 0;
      dataControl4.Bits.dqvmselogicdelay  = 0;
    } else {
      dataControl4.Bits.dqsvmselogicdelay  = 3;
      if (IsBDXEP(host) || (IsDESku(host) && CheckSteppingGreaterThan(host, CPU_BDX, B0_REV_BDX))) {
        dataControl4.Bits.dqvmselogicdelay   = 0;
      } else {
        dataControl4.Bits.dqvmselogicdelay   = 0xFF;
      }
    }
  }

  if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
    ddrCRClkControls.Bits.iolbctl = 2;
  } else {
    ddrCRClkControls.Bits.iolbctl = 0;
  }
  ddrCRClkControls.Bits.statlegen   = 3;
  ddrCRClkControls.Bits.xoveropmode = 1;
  ddrCRClkControls.Bits.rxvref      = 0x0F;
  if (CheckSteppingLessThan(host, CPU_HSX, C0_REV_HSX)) {
    ddrCRClkControls.Bits.intclkon  = 1;
  } else {
    ddrCRClkControls.Bits.intclkon  = 0;
  }

  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
    ddrCRClkControls.Bits.cttermination = 1;
  }

  if ((host->nvram.mem.dimmTypePresent != UDIMM) && (host->nvram.mem.dimmTypePresent != SODIMM)) {
    ddrCRClkControls.Bits.is_rdimm = 1;
  }

  ddrCRClkComp.Bits.scomp         = 9;
  ddrCRClkComp.Bits.rcompdrvup    = 19;
  ddrCRClkComp.Bits.rcompdrvdown  = 0x13;
  ddrCRClkComp.Bits.lscomp        = 0x1;

  ddrCRCmdComp.Bits.rcompdrvup    = 0x0C;
  ddrCRCmdComp.Bits.rcompdrvdown  = 0x0C;

  ddrCRCtlComp.Bits.rcompdrvup    = 0x13;
  ddrCRCtlComp.Bits.rcompdrvdown  = 0x13;
  ddrCRCtlComp.Bits.lscomp        = 0x1;

  // if BDX DE, set the bit as a WA for A0 only. HSD 4986750
  if (host->var.common.cpuType == CPU_HSX) {
    ddrCRCmdComp.Bits.scomp = 0;
    ddrCRCtlComp.Bits.scomp = 0;
  }

  if (IsDESku(host) && CheckSteppingLessThan(host, CPU_BDX, C0_REV_BDX)) {
     ddrCRCmdCompmc.Bits.scomp = 0;
     ddrCRCtlCompmc.Bits.scomp = 0;
  }

  // Time 1: lvmode=0, lvmodevalide = 0
  ddrCRCompCtl0.Data = MemReadPciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG);
  switch (host->nvram.mem.socket[socket].ddrVoltage) {
    case SPD_VDD_150:
      ddrCRCompCtl0.Bits.lvmode = 0;
      break;
    case SPD_VDD_135:
      ddrCRCompCtl0.Bits.lvmode = 1;
      break;
    case SPD_VDD_125:
      ddrCRCompCtl0.Bits.lvmode = 2;
      break;
    case SPD_VDD_120:
      ddrCRCompCtl0.Bits.lvmode = 3;
      break;
  }

  if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
    if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
      ddrCRCompCtl0.Bits.dqdrvpupvref = 44;
    } else {
      ddrCRCompCtl0.Bits.dqdrvpupvref = 5;
    }
  } else {
    if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
      if (host->var.common.cpuType == CPU_HSX) {
        ddrCRCompCtl0.Bits.dqdrvpupvref = 36;
      }  else {
        ddrCRCompCtl0.Bits.dqdrvpupvref = 32;
      }
    } else { // DDR4
      if (host->var.common.cpuType == CPU_HSX) {
        ddrCRCompCtl0.Bits.dqdrvpupvref = 44;
      }  else { // BDX
        ddrCRCompCtl0.Bits.dqdrvpupvref = 43;
      }
    }
  }
  ddrCRCompCtl0.Bits.dqdrvpdnvref = 32;
  ddrCRCompCtl0.Bits.dqodtpupvref = 0x26;

  // Time 2: lvmode = target value, lvmodevalid = 0
  ddrCRCompCtl0.Bits.lvmodevalid  = 0;
  MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);
  MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);

  // Time 3: lvmode = target value, lvmodevalid = 1
  ddrCRCompCtl0.Bits.lvmodevalid  = 1;
  MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);
  MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);

  // Time 4: lvmode = target value, lvmodevalid =0
  ddrCRCompCtl0.Bits.lvmodevalid  = 0;
  MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);
  MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);

  // Time 5: lvmode = target value, lvmodevalid = 1
  ddrCRCompCtl0.Bits.lvmodevalid  = 1;
  MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);
  MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);

  ddrCRCompCtl1.Data = MemReadPciCfgEp (host, socket, 0, DDRCRCOMPCTL1_MCIO_DDRIOEXTMCC_REG);
  ddrCRCompCtl1.Bits.dqscomppc      = 1;
  ddrCRCompCtl1.Bits.cmdscomppc     = 1;
  ddrCRCompCtl1.Bits.ctlscomppc     = 1;


  switch (GetClosestFreq(host, socket)){
    case(DDR_800):
      ddrCRCompCtl1.Bits.dqscompcells   = 15;
      ddrCRCompCtl1.Bits.cmdscompcells  = 5;
      ddrCRCompCtl1.Bits.ctlscompcells  = 15;
      break;
    case(DDR_1067):
      ddrCRCompCtl1.Bits.dqscompcells   = 12;
      ddrCRCompCtl1.Bits.cmdscompcells  = 6;
      ddrCRCompCtl1.Bits.ctlscompcells  = 12;
      break;
    case(DDR_1333):
      ddrCRCompCtl1.Bits.dqscompcells   = 10;
      ddrCRCompCtl1.Bits.cmdscompcells  = 6;
      ddrCRCompCtl1.Bits.ctlscompcells  = 10;
      break;
    case(DDR_1600):
      ddrCRCompCtl1.Bits.dqscompcells   = 8;
      ddrCRCompCtl1.Bits.cmdscompcells  = 5;
      ddrCRCompCtl1.Bits.ctlscompcells  = 8;
      break;
    case(DDR_1867):
      ddrCRCompCtl1.Bits.dqscompcells   = 7;
      ddrCRCompCtl1.Bits.cmdscompcells  = 4;
      ddrCRCompCtl1.Bits.ctlscompcells  = 7;
      break;
    case(DDR_2133):
      ddrCRCompCtl1.Bits.dqscompcells   = 6;
      ddrCRCompCtl1.Bits.cmdscompcells  = 4;
      ddrCRCompCtl1.Bits.ctlscompcells  = 6;
      break;
    case(DDR_2400):
      ddrCRCompCtl1.Bits.dqscompcells   = 5;
      ddrCRCompCtl1.Bits.cmdscompcells  = 3;
      ddrCRCompCtl1.Bits.ctlscompcells  = 5;
      break;
    case(DDR_2667):
      ddrCRCompCtl1.Bits.dqscompcells   = 5;
      ddrCRCompCtl1.Bits.cmdscompcells  = 3;
      ddrCRCompCtl1.Bits.ctlscompcells  = 5;
      break;
    case(DDR_2800):
    default:
      ddrCRCompCtl1.Bits.dqscompcells   = 4;
      ddrCRCompCtl1.Bits.cmdscompcells  = 3;
      ddrCRCompCtl1.Bits.ctlscompcells  = 4;
      break;
  }

  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
    ddrCRCompCtl1.Bits.tcovref        = 32;
  } else {
    ddrCRCompCtl1.Bits.tcovref        = 64;
  }
  MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL1_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl1.Data);
  MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL1_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl1.Data);

  ddrCRCompCtl2.Data = MemReadPciCfgEp (host, socket, 0, DDRCRCOMPCTL2_MCIO_DDRIOEXTMCC_REG);
  ddrCRCompCtl2.Bits.odtstatlegen   = 3;
  ddrCRCompCtl2.Bits.txdqstatlegen  = 3;
  ddrCRCompCtl2.Bits.cmdstatlegen   = 3;
  ddrCRCompCtl2.Bits.dqodtpdnvref   = 32;
  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
    if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
      ddrCRCompCtl2.Bits.clkdrvpupvref  = 44;
    } else {
      ddrCRCompCtl2.Bits.clkdrvpupvref  = 32;
    }
    ddrCRCompCtl2.Bits.ddr3nren       = 1;
  } else {
    if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
      ddrCRCompCtl2.Bits.clkdrvpupvref  = 5;
    } else {
      if (host->var.common.cpuType == CPU_HSX) {
        ddrCRCompCtl2.Bits.clkdrvpupvref  = 44;
      } else { // BDX
        ddrCRCompCtl2.Bits.clkdrvpupvref  = 41;
      }
    }
    ddrCRCompCtl2.Bits.ddr3nren       = 0;
  }
  ddrCRCompCtl2.Bits.clkdrvpdnvref  = 32;
  MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL2_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl2.Data);
  MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL2_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl2.Data);

  ddrCRCompCtl3.Data = MemReadPciCfgEp (host, socket, 0, DDRCRCOMPCTL3_MCIO_DDRIOEXTMCC_REG);
  ddrCRCompCtl3.Bits.swcapcmpclksel = 2;
  ddrCRCompCtl3.Bits.ctldrvpdnvref  = 32;
  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
    if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
      ddrCRCompCtl3.Bits.ctldrvpupvref  = 44;
    } else {
      ddrCRCompCtl3.Bits.ctldrvpupvref  = 32;
    }
  } else {
    if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
      ddrCRCompCtl3.Bits.ctldrvpupvref  = 5;
    } else {
      if (host->var.common.cpuType == CPU_HSX) {
        ddrCRCompCtl3.Bits.ctldrvpupvref  = 44;
      } else {
        ddrCRCompCtl3.Bits.ctldrvpupvref  = 41;
      }
    }
  }
  ddrCRCompCtl3.Bits.cmddrvpdnvref  = 32;

  if (IsDESku(host)){
    ddrCRCompCtl3.Bits.cmddrvpupvref  = 27; // Init for 1DPC
    for (ch = 0; ch < MAX_CH/2; ch++) {
      if ((*channelNvList)[ch].maxDimm > 1) {//Override for 2DPC and above
        ddrCRCompCtl3.Bits.cmddrvpupvref  = 44;
      }
    }
  } else {
    if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
      if ((host->nvram.mem.dimmTypePresent != RDIMM) || (host->nvram.mem.socket[socket].lrDimmPresent)) {
        ddrCRCompCtl3.Bits.cmddrvpupvref  = 49;  //RON=18ohms
      } else {
        ddrCRCompCtl3.Bits.cmddrvpupvref  = 31;  //RON=30ohms
      }
    } else {
      if ((host->nvram.mem.dimmTypePresent == UDIMM)||(host->nvram.mem.dimmTypePresent == SODIMM)) {
        ddrCRCompCtl3.Bits.cmddrvpupvref  = 49;  //RON=18ohms
      } else {
        ddrCRCompCtl3.Bits.cmddrvpupvref  = 31;  //RON=30ohms
      }
    }
  }
  MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL3_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl3.Data);
  MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL3_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl3.Data);

  ddrCRCompVssHi.Bits.panicdrvdnvref = 32;
  ddrCRCompVssHi.Bits.panicdrvupvref = 32;
  ddrCRCompVssHi.Bits.ddrdmvrfs3localpwrgoodoverride = 1;
  MemWritePciCfgEp (host, socket, 0, DDRCRCOMPVSSHI_MCIO_DDRIOEXTMCC_REG, ddrCRCompVssHi.Data);
  MemWritePciCfgEp (host, socket, 2, DDRCRCOMPVSSHI_MCIO_DDRIOEXTMCC_REG, ddrCRCompVssHi.Data);

  if (host->var.common.cpuType == CPU_BDX) {
    Qclkps = (UINT16) (1000000/(host->nvram.mem.socket[socket].ddrFreqMHz)); // QCLK period in pico seconds.

    Vdd = 0;
    if (host->nvram.mem.socket[socket].ddrVoltage == SPD_VDD_120) {
      Vdd = 1200; //1200mv
    } else if (host->nvram.mem.socket[socket].ddrVoltage == SPD_VDD_135) {
      Vdd = 1350; //1350mv
    } else if (host->nvram.mem.socket[socket].ddrVoltage == SPD_VDD_150) {
      Vdd = 1500; //1500mv
    } else {
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Vdd is unknowm. Halting in DDRIO Init. Vsshi programming.\n"));
      FatalError (host, 0, 0);
    }

    //
    // Get current Vdd setting
    //
    userVdd = host->setup.mem.inputMemTime.vdd;
    if (userVdd != 0) {
      if (host->nvram.mem.dramType == SPD_TYPE_DDR3){
        if((userVdd < 100) || (userVdd > 185)) {
          userVdd = 0;
        }
      } else {
        if((userVdd < 90) || (userVdd > 185)) {
          userVdd = 0;
        }
      }
    }

    if (userVdd) {
      Vdd = userVdd * 10;
    }

    VssHi = ((UINT16) Vdd - VssHiSwingTarget);   // VssHiSwingTarget = 950 mV, VddmV=1500 mV
    //
    // Target Current.  pF * mV / pS -> mA
    //
    Itarget = CDIEVSSHI_BDW * delta / Qclkps;

    rcmdref_vsshi_up_res = (legs * mcodt);
    rcmdref_vsshi_dn_res = (legs * mcodt);

    //
    // Compensated to this current
    //
    IcompUp = VssHi / rcmdref_vsshi_dn_res;
    IcompDn = (VccIomV - VssHi) / rcmdref_vsshi_up_res;
    if (Vdd > 1350) IcompUp /= 2;

    if (host->nvram.mem.dramType == SPD_TYPE_DDR4){ //DDR4
      ddrCRCompVssHi.Bits.vtoffset = 0x15;
      ddrCRCompVssHi.Bits.vtslopea = 5;
      ddrCRCompVssHi.Bits.vtslopeb = 7;
    } else if ((host->nvram.mem.dramType == SPD_TYPE_DDR3) && (host->nvram.mem.socket[socket].ddrVoltage == SPD_VDD_135)) { //(VMSE,DDR3L)
      ddrCRCompVssHi.Bits.vtoffset = 0x1C;
      ddrCRCompVssHi.Bits.vtslopea = 4;
      ddrCRCompVssHi.Bits.vtslopeb = 3;
    } else if (host->nvram.mem.dramType == SPD_TYPE_DDR3) { //(DDR3)
      ddrCRCompVssHi.Bits.vtoffset = 0x12;
      ddrCRCompVssHi.Bits.vtslopea = 6;
      ddrCRCompVssHi.Bits.vtslopeb = 0;
    }
    //
    // Calculate multipler to get target current
    //
    ddrCRCompVssHibdx.Data = 0;
    ddrCRCompVssHibdx.Bits.paniccompupmult = MIN ((Itarget / IcompUp), PANICCOMPUPMULT);
    ddrCRCompVssHibdx.Bits.paniccompdnmult = MIN ((Itarget / IcompDn), PANICCOMPDNMULT);

    ddrCrCompVssHiControl.Data = MemReadPciCfgEp (host, socket, 0, DDRCRCOMPVSSHICONTROL_MCIO_DDRIOEXTMCC_REG);
    VssHiorVrefcontrol1.Data = MemReadPciCfgEp (host, socket, 0, DDRCRCOMPVSSHICONTROL1_MCIO_DDRIOEXTMCC_REG);
    if (host->nvram.mem.dramType == SPD_TYPE_DDR4) { //(DDR4,DDR3U)
      ddrCrCompVssHiControl.Bits.vsshiorvrefctl = 0x5F861E;
      VssHiorVrefcontrol1.Bits.vsshiorvrefctl = 0x01;
    } else if ((host->nvram.mem.dramType == SPD_TYPE_DDR3)&&(host->nvram.mem.socket[socket].ddrVoltage == SPD_VDD_135)) { //(VMSE,DDR3L)
      ddrCrCompVssHiControl.Bits.vsshiorvrefctl = 0x5F8632;
      VssHiorVrefcontrol1.Bits.vsshiorvrefctl = 0x1D;
    } else if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {//(DDR3)
      ddrCrCompVssHiControl.Bits.vsshiorvrefctl = 0x5F8636;
      VssHiorVrefcontrol1.Bits.vsshiorvrefctl = 0x19;
    }

    ddrCRCompVssHi.Bits.panicdrvupvref = (0x3F & ddrCrCompVssHiControl.Bits.vsshiorvrefctl); // Mirrot the last 6 bits [5:0] of the register
    ddrCRCompVssHi.Bits.panicdrvdnvref = (0x3F & ddrCrCompVssHiControl.Bits.vsshiorvrefctl); // Mirrot the last 6 bits [5:0] of the register

    vssControl.Data = ddrCrCompVssHiControl.Data;

    MemWritePciCfgEp (host, socket, 0, DDRCRCOMPVSSHI1_MCIO_DDRIOEXTMCC_REG, ddrCRCompVssHibdx.Data);
    MemWritePciCfgEp (host, socket, 2, DDRCRCOMPVSSHI1_MCIO_DDRIOEXTMCC_REG, ddrCRCompVssHibdx.Data);

    MemWritePciCfgEp (host, socket, 0, DDRCRCOMPVSSHICONTROL_MCIO_DDRIOEXTMCC_REG, ddrCrCompVssHiControl.Data);
    MemWritePciCfgEp (host, socket, 2, DDRCRCOMPVSSHICONTROL_MCIO_DDRIOEXTMCC_REG, ddrCrCompVssHiControl.Data);

    MemWritePciCfgEp (host, socket, 0, DDRCRCOMPVSSHICONTROL1_MCIO_DDRIOEXTMCC_REG, VssHiorVrefcontrol1.Data);
    MemWritePciCfgEp (host, socket, 2, DDRCRCOMPVSSHICONTROL1_MCIO_DDRIOEXTMCC_REG, VssHiorVrefcontrol1.Data);

    MemWritePciCfgEp (host, socket, 0, DDRCRCOMPVSSHI_MCIO_DDRIOEXTMCC_REG, ddrCRCompVssHi.Data);
    MemWritePciCfgEp (host, socket, 2, DDRCRCOMPVSSHI_MCIO_DDRIOEXTMCC_REG, ddrCRCompVssHi.Data);

    for (ch = 0; ch < MAX_CH; ch++) {
      for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, VSSHIORVREFCONTROL1_0_MCIO_DDRIO_REG), VssHiorVrefcontrol1.Data);
      } // strobe loop
    } // ch loop

  }

  if (host->var.common.cpuType == CPU_HSX) {
    ddrCrDimmVrefControl1Hsx.Data = 0;
    ddrCrDimmVrefControl1Hsx.Bits.cavrefctl      = 64;
    ddrCrDimmVrefControl1Hsx.Bits.ch0vrefctl     = 64;
    ddrCrDimmVrefControl1Hsx.Bits.ch1vrefctl     = 64;
    ddrCrDimmVrefControl1Hsx.Bits.endimmvrefca   = 1;
    if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
      ddrCrDimmVrefControl1Hsx.Bits.endimmvrefch0  = 1;
      ddrCrDimmVrefControl1Hsx.Bits.endimmvrefch1  = 1;
      ddrCrDimmVrefControl1Hsx.Bits.cttermination  = 1;
    }
    ddrCrDimmVrefControl1Hsx.Bits.hiztimerctrl   = 2;
    tempReg = ddrCrDimmVrefControl1Hsx.Data;
  } else if (host->var.common.cpuType == CPU_BDX) {
    ddrCrDimmVrefControl1Bdx.Data = 0;
    if (IsDESku(host) && CheckSteppingEqual(host, CPU_BDX, B0_REV_BDX)) {
      ddrCrDimmVrefControl1Bdx.Bits.cavrefctl = 54;
    } else {
      ddrCrDimmVrefControl1Bdx.Bits.cavrefctl = 64;
    }
    ddrCrDimmVrefControl1Bdx.Bits.ch0vrefctl     = 64;
    ddrCrDimmVrefControl1Bdx.Bits.ch1vrefctl     = 64;
    ddrCrDimmVrefControl1Bdx.Bits.endimmvrefca   = 1;
    if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
      ddrCrDimmVrefControl1Bdx.Bits.endimmvrefch0  = 1;
      ddrCrDimmVrefControl1Bdx.Bits.endimmvrefch1  = 1;
      ddrCrCompOvr.Bits.cttermination            = 1;
    }
    ddrCrDimmVrefControl1Bdx.Bits.hiztimerctrl   = 2;
    tempReg = ddrCrDimmVrefControl1Bdx.Data;
  }
  (*channelNvList)[0].dimmVrefControl1 = tempReg;
  (*channelNvList)[1].dimmVrefControl1 = tempReg;
  (*channelNvList)[2].dimmVrefControl1 = tempReg;
  (*channelNvList)[3].dimmVrefControl1 = tempReg;
  MemWritePciCfgEp (host, socket, 0, DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXTMCC_REG, tempReg);
  MemWritePciCfgEp (host, socket, 2, DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXTMCC_REG, tempReg);

  //
  // BDX Si sighting: 5097497 - apply rcompdrvup/down overrides to DDR3 UDIMM as well
  //
  if (host->nvram.mem.dimmTypePresent != RDIMM) {
    ddrCrClkCompCmp.Data = 0;
    ddrCrClkCompCmp.Bits.rcompdrvup = 60;
    ddrCrClkCompCmp.Bits.rcompdrvdown = 63;
    MemWritePciCfgEp (host, socket, 0, DDRCRCLKCOMP_CMP_MCIO_DDRIOEXTMCC_REG, ddrCrClkCompCmp.Data);
    MemWritePciCfgEp (host, socket, 2, DDRCRCLKCOMP_CMP_MCIO_DDRIOEXTMCC_REG, ddrCrClkCompCmp.Data);
  }

  ddrCrDimmVrefControl2.Data = MemReadPciCfgEp (host, socket, 0, DDRCRDIMMVREFCONTROL2_MCIO_DDRIOEXTMCC_REG);
  if (host->var.common.cpuType == CPU_HSX) {
    ddrCrDimmVrefControl2.Data |= 2 << 8;
    if(((GetClosestFreq(host, socket) >= DDR_1600) && ((host->nvram.mem.dramType == SPD_TYPE_DDR3) ||
       ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && ((host->nvram.mem.dimmTypePresent == UDIMM) || host->nvram.mem.dimmTypePresent == SODIMM))))) {
      //
      // Enable the divide by 2 clock in the Vref circuit
      //
      ddrCrDimmVrefControl2.Data |= 1 << 4;
    }
  } else { // BDX
    //
    // Clear bits 23:12, 10:0
    ddrCrDimmVrefControl2.Bits.dimmvrefctl &= ~0xFFF7FF;
    //    11:11 - reserved
    //    10:10 - 1'b1
    //    9:8 - 2'b01
    //    7:7 - 1'b0
    //    3:2 - 2'b01
    //    1:0 - 2'b10
    ddrCrDimmVrefControl2.Bits.dimmvrefctl |= 1 << 10;
    ddrCrDimmVrefControl2.Bits.dimmvrefctl |= 1 << 8;
    ddrCrDimmVrefControl2.Bits.dimmvrefctl |= 0 << 7;
    ddrCrDimmVrefControl2.Bits.dimmvrefctl |= 1 << 2;
    ddrCrDimmVrefControl2.Bits.dimmvrefctl |= 2 << 0;
    if (host->nvram.mem.dramType == SPD_TYPE_DDR4) { //(DDR4)
      //      20:18 - 3'b101
      //      17:15 - 3'b100
      //      14:12 - 3'b110
      //      6:4 - 2'b011
      ddrCrDimmVrefControl2.Bits.dimmvrefctl |= 5 << 18;
      ddrCrDimmVrefControl2.Bits.dimmvrefctl |= 4 << 15;
      ddrCrDimmVrefControl2.Bits.dimmvrefctl |= 6 << 12;
      ddrCrDimmVrefControl2.Bits.dimmvrefctl |= 3 << 4;
    } else if ((host->nvram.mem.dramType == SPD_TYPE_DDR3)&&(host->nvram.mem.socket[socket].ddrVoltage == SPD_VDD_135)) { //(DDR3L)
      //      20:18 - 3'b101
      //      17:15 - 3'b101
      //      14:12 - 3'b101
      //      6:4 - 2'b010
      ddrCrDimmVrefControl2.Bits.dimmvrefctl |= 5 << 18;
      ddrCrDimmVrefControl2.Bits.dimmvrefctl |= 5 << 15;
      ddrCrDimmVrefControl2.Bits.dimmvrefctl |= 5 << 12;
      ddrCrDimmVrefControl2.Bits.dimmvrefctl |= 2 << 4;
    } else if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {//(DDR3)
      //      20:18 - 3'b110
      //      17:15 - 3'b110
      //      14:12 - 3'b000
      //      6:4 - 2'b010
      ddrCrDimmVrefControl2.Bits.dimmvrefctl |= 6 << 18;
      ddrCrDimmVrefControl2.Bits.dimmvrefctl |= 6 << 15;
      ddrCrDimmVrefControl2.Bits.dimmvrefctl |= 0 << 12;
      ddrCrDimmVrefControl2.Bits.dimmvrefctl |= 2 << 4;
    }
  }
  MemWritePciCfgEp (host, socket, 0, DDRCRDIMMVREFCONTROL2_MCIO_DDRIOEXTMCC_REG, ddrCrDimmVrefControl2.Data);
  MemWritePciCfgEp (host, socket, 2, DDRCRDIMMVREFCONTROL2_MCIO_DDRIOEXTMCC_REG, ddrCrDimmVrefControl2.Data);

  if (host->var.common.cpuType == CPU_HSX) {
    ddrCrCompVssHiControl.Data = vssControl.Data;
  }
  if (((host->var.common.bootMode == S3Resume) || (host->var.mem.subBootMode == WarmBootFast)) && (host->var.mem.subBootMode != AdrResume)) {
    ddrCrCompVssHiControl.Data |= BIT13;
    ddrCrCompVssHiControl.Data &= ~BIT17;
  }
  MemWritePciCfgEp (host, socket, 0, DDRCRCOMPVSSHICONTROL_MCIO_DDRIOEXTMCC_REG, ddrCrCompVssHiControl.Data);
  MemWritePciCfgEp (host, socket, 2, DDRCRCOMPVSSHICONTROL_MCIO_DDRIOEXTMCC_REG, ddrCrCompVssHiControl.Data);

  MemWritePciCfgEp (host, socket, 0, DDRCRCOMPOVR_MCIO_DDRIOEXTMCC_REG, ddrCrCompOvr.Data);
  MemWritePciCfgEp (host, socket, 2, DDRCRCOMPOVR_MCIO_DDRIOEXTMCC_REG, ddrCrCompOvr.Data);

  ddrCRSPDCfg1.Data = MemReadPciCfgEp (host, socket, 0, DDRCRSPDCFG1_MCIO_DDRIOEXTMCC_REG);
  ddrCRSPDCfg1.Bits.crspdcfg1 = 0x83F8;
  if (!(CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX))) {
    ddrCRSPDCfg1.Bits.odtsegovrd = 1;
    ddrCRSPDCfg1.Bits.odtupsegen = 1;
    if ((host->nvram.mem.dramType == SPD_TYPE_DDR3)||(host->var.common.cpuType == CPU_BDX)) {
      ddrCRSPDCfg1.Bits.odtdnsegen = 1;
    } else {
      ddrCRSPDCfg1.Bits.odtdnsegen = 2;
    }
  }
  MemWritePciCfgEp (host, socket, 0, DDRCRSPDCFG1_MCIO_DDRIOEXTMCC_REG, ddrCRSPDCfg1.Data);
  MemWritePciCfgEp (host, socket, 2, DDRCRSPDCFG1_MCIO_DDRIOEXTMCC_REG, ddrCRSPDCfg1.Data);

  ddrCRSPDCfg2.Data = MemReadPciCfgEp (host, socket, 0, DDRCRSPDCFG2_MCIO_DDRIOEXTMCC_REG);
  if (!(CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX))) {
    ddrCRSPDCfg2.Bits.ctlsegovrd = 1;
    ddrCRSPDCfg2.Bits.clksegovrd = 1;
    ddrCRSPDCfg2.Bits.dqsegovrd = 1;
    if (host->var.common.cpuType == CPU_HSX) {
      if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
        ddrCRSPDCfg2.Bits.ctldnsegen = 2;
        ddrCRSPDCfg2.Bits.ctlupsegen = 2;
        ddrCRSPDCfg2.Bits.clkdnsegen = 2;
        ddrCRSPDCfg2.Bits.clkupsegen = 2;
        ddrCRSPDCfg2.Bits.dqdnsegen = 2;
        ddrCRSPDCfg2.Bits.dqupsegen = 2;
      } else {
        ddrCRSPDCfg2.Bits.ctldnsegen = 3;
        ddrCRSPDCfg2.Bits.ctlupsegen = 3;
        ddrCRSPDCfg2.Bits.clkdnsegen = 3;
        ddrCRSPDCfg2.Bits.clkupsegen = 3;
        ddrCRSPDCfg2.Bits.dqdnsegen = 3;
        ddrCRSPDCfg2.Bits.dqupsegen = 3;
      }
    } else {//BDX
      ddrCRSPDCfg2.Bits.ctldnsegen = 1;
      ddrCRSPDCfg2.Bits.clkdnsegen = 1;
      ddrCRSPDCfg2.Bits.dqdnsegen  = 1;
      if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
        ddrCRSPDCfg2.Bits.ctlupsegen = 2;
        ddrCRSPDCfg2.Bits.clkupsegen = 2;
        ddrCRSPDCfg2.Bits.dqupsegen  = 2;
      } else {
        ddrCRSPDCfg2.Bits.ctlupsegen = 3;
        ddrCRSPDCfg2.Bits.clkupsegen = 3;
        ddrCRSPDCfg2.Bits.dqupsegen  = 3;
      }
    }
  }

  if (host->var.common.cpuType == CPU_HSX){
    ddrCRSPDCfg2.Bits.crspdcfg2 = 0x44F0;
  }else{//BDX
  ddrCRSPDCfg2.Bits.crspdcfg2 = 0x56B0;
  }
  MemWritePciCfgEp (host, socket, 0, DDRCRSPDCFG2_MCIO_DDRIOEXTMCC_REG, ddrCRSPDCfg2.Data);
  if (Is1HA(host) || (host->var.common.cpuType == CPU_BDX)) {
    if (host->var.common.cpuType == CPU_HSX){
      ddrCRSPDCfg2.Bits.crspdcfg2 = 0x4F0;
    }else{//BDX
      ddrCRSPDCfg2.Bits.crspdcfg2 = 0x16B0;
    }
  }

  MemWritePciCfgEp (host, socket, 2, DDRCRSPDCFG2_MCIO_DDRIOEXTMCC_REG, ddrCRSPDCfg2.Data);

  for (ch = 0; ch < MAX_CH; ch++) {
    for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, VSSHIORVREFCONTROL_0_MCIO_DDRIO_REG), vssControl.Data);
    } // strobe loop
  } // ch loop

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)){

      for (strobe = 0; strobe < MAX_STROBE; strobe++) {
        MemWritePciCfgEp (host, socket, ch, rxVrefCtrlTable[strobe], 0);
      } // strobe loop

      //
      // Set all Rx Vref per bit
      //
      if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {

        // Lookup nominal RxVref setting
        ddr4OdtValuePtr = LookupDdr4OdtValue(host, socket, ch);
        if(ddr4OdtValuePtr == NULL) {
          RC_ASSERT(FALSE, ERR_INVALID_POP, 0);
        }
        // Selects the Vref voltage value coming out of internal Vref generator.
        // The absolute Vref values are calculated as below:
        rxVref = ((ddr4OdtValuePtr->mcVref - 50) * 127) / 50;

        // Assume DRAM Vref range 1 (60% - 92.5%)
        txVref = ((((ddr4OdtValuePtr->dramVref - 60) * 100) / 65) & 0x3F);

      } else {
        rxVref = 32;
      }
      GetSetDataGroup (host, socket, ch, 0, 0, ALL_STROBES, ALL_BITS, DdrLevel, RxVref, GSM_READ_CSR | GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &rxVref);
    }

    ddrCRCmdControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG);
    ddrCRCmdControls1.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS1_CMDN_MCIO_DDRIOEXT_REG);

    if (host->var.common.cpuType == CPU_HSX) {
      ddrCRCmdControls.Bits.ddrphasextalkenableqnnnh = 0;
    }
    ddrCRCmdControls.Bits.gqclkdivenable  = 0;
    ddrCRCmdControls.Bits.xoveropmode     = 1;
    ddrCRCmdControls.Bits.txon            = 0;
    if (CheckSteppingLessThan(host, CPU_HSX, C0_REV_HSX)) {
      ddrCRCmdControls.Bits.intclkon      = 1;
    } else {
      ddrCRCmdControls.Bits.intclkon      = 0;
    }
    ddrCRCmdControls.Bits.iolbctl         = 0;
    ddrCRCmdControls.Bits.odtmode         = 0;
    ddrCRCmdControls.Bits.earlyweakdrive  = 0;
    ddrCRCmdControls.Bits.lvlshft_holden  = 0;
    ddrCRCmdControls.Bits.rxvref          = 0x0F;
    ddrCRCmdControls.Bits.paritydisable   = 1;
    ddrCRCmdControls.Bits.xovercal        = 0;
    ddrCRCmdControls.Bits.ddr4modeenable = 0;

    //
    // Enable ODT static legs
    //
    ddrCRCmdControls1.Bits.odtstatdflt  = 3;
    ddrCRCmdControls1.Bits.dqstatdflt   = 3;
    ddrCRCmdControls1.Bits.ddrcrimodeen = 0;
    if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
      ddrCRCmdControls1.Bits.cttermination = 1;
    }

    if ((host->var.common.bootMode == S3Resume && host->var.mem.subBootMode !=AdrResume) || (host->var.mem.subBootMode == WarmBootFast)){ 
      ddrCRCmdControls.Bits.drvpupdis = 0;
      MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdControls.Data);
      MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControls.Data);
      ddrCRCmdControls.Bits.drvpupdis = 1;
      MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdControls.Data);
      MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControls.Data);
      ddrCRCmdControls.Bits.drvpupdis = 0;
    }

    MemWritePciCfgEp (host, socket, ch, DDRCRCMDCOMP_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdComp.Data);
    MemWritePciCfgEp (host, socket, ch, DDRCRCMDCOMP_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdComp.Data);
    MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControls.Data);
    MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdControls.Data);
    MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS1_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdControls1.Data);
    MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControls1.Data);

    ddrCRCtlControls.Bits.xoveropmode = 1;
    ddrCRCtlControls.Bits.paritydisable = 1;
    ddrCRCtlControls.Bits.intclkon      = 1;
    ddrCRCtlControls.Bits.rxvref      = 0x0F;
    if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
      ddrCRCtlControls.Bits.ddr3nren    = 1;
    }
    MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlControls.Data);
    MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlControls.Data);
    MemWritePciCfgEp (host, socket, ch, DDRCRCTLCOMP_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlComp.Data);
    MemWritePciCfgEp (host, socket, ch, DDRCRCTLCOMP_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlComp.Data);
    if (IsDESku(host) && CheckSteppingLessThan(host, CPU_BDX, C0_REV_BDX)) {
      MemWritePciCfgEp (host, socket, ch, DDRCRCMDCOMP_MCIO_DDRIOEXTMCC_REG, ddrCRCmdCompmc.Data);
      MemWritePciCfgEp (host, socket, ch, DDRCRCTLCOMP_MCIO_DDRIOEXTMCC_REG, ddrCRCtlCompmc.Data);
    }
    FixedDelay(host, 1);

    ddrCRCtlRanksUsed.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_REG);
    ddrCRCtlControls1.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXT_REG);
    if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
      ddrCRCtlControls1.Bits.cttermination = 1;
    }
    ddrCRCtlControls1.Bits.odtstatdflt  = 3;
    ddrCRCtlControls1.Bits.dqstatdflt   = 3;

    if ((host->var.common.bootMode == S3Resume && host->var.mem.subBootMode !=AdrResume) || (host->var.mem.subBootMode == WarmBootFast)){ 
      ddrCRCtlRanksUsed.Bits.drvpupdis = 0;
      MemWritePciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlRanksUsed.Data);
      MemWritePciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlRanksUsed.Data);
      ddrCRCtlControls1.Bits.ddrcrimodesrcntl = 0;
      MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlControls1.Data);
      MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlControls1.Data);
      ddrCRCtlRanksUsed.Bits.drvpupdis = 1;
      MemWritePciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlRanksUsed.Data);
      MemWritePciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlRanksUsed.Data);
      ddrCRCtlControls1.Bits.ddrcrimodesrcntl = 1;
      MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlControls1.Data);
      MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlControls1.Data);
    }
    ddrCRCtlRanksUsed.Bits.drvpupdis = 0;
    ddrCRCtlControls1.Bits.ddrcrimodesrcntl = 0;
    MemWritePciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlRanksUsed.Data);
    MemWritePciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlRanksUsed.Data);
    MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlControls1.Data);
    MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlControls1.Data);

    if ((host->var.common.bootMode == S3Resume && host->var.mem.subBootMode !=AdrResume) || (host->var.mem.subBootMode == WarmBootFast)){ 
      ddrCRClkRanksUsed.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG);
      ddrCRClkRanksUsed.Bits.drvpupdis = 0;
      ddrCRClkRanksUsed.Bits.drvsegen = 7;
      MemWritePciCfgEp (host, socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, ddrCRClkRanksUsed.Data);
      ddrCRClkRanksUsed.Bits.drvpupdis = 1;
      MemWritePciCfgEp (host, socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, ddrCRClkRanksUsed.Data);
      ddrCRClkRanksUsed.Bits.drvpupdis = 0;
      MemWritePciCfgEp (host, socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, ddrCRClkRanksUsed.Data);
    }


    if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)){

      (*channelNvList)[ch].dataControl0 = dataControl0.Data;
      (*channelNvList)[ch].dataControl1[0] = dataControl1.Data;
      (*channelNvList)[ch].ddrCRClkControls = ddrCRClkControls.Data;
    } else if ((host->var.common.bootMode == S3Resume) || (host->var.mem.subBootMode == WarmBootFast) || (host->var.mem.subBootMode == ColdBootFast)){
      dataControl0.Data = (*channelNvList)[ch].dataControl0;
      dataControl1.Data = (*channelNvList)[ch].dataControl1[0];
      ddrCRClkControls.Data = (*channelNvList)[ch].ddrCRClkControls;
    }


    if (host->var.mem.subBootMode == ColdBootFast && host->setup.mem.options & RMT_COLD_FAST_BOOT){
      dataControl0.Bits.rfon = 0;
    }


    MemWritePciCfgEp (host, socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG, ddrCRClkControls.Data);
    MemWritePciCfgEp (host, socket, ch, DDRCRCLKCOMP_CLK_MCIO_DDRIOEXT_REG, ddrCRClkComp.Data);

    if ((host->var.common.bootMode == S3Resume && host->var.mem.subBootMode !=AdrResume) || (host->var.mem.subBootMode == WarmBootFast)){ 
      dataControl4.Bits.drvpupdis = 0;
      for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL4_0_MCIO_DDRIO_REG), dataControl4.Data);
      } // strobe loop
      dataControl4.Bits.drvpupdis = 1;
      for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL4_0_MCIO_DDRIO_REG), dataControl4.Data);
      } // strobe loop
      dataControl4.Bits.drvpupdis = 0;
    }
    for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
      if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)){
        (*channelNvList)[ch].dataControl2[strobe] = dataControl2.Data;
        (*channelNvList)[ch].dataControl4[strobe] = dataControl4.Data;
        (*channelNvList)[ch].dataControl3[strobe] = dataControl3.Data;
      } else if ((host->var.common.bootMode == S3Resume) || (host->var.mem.subBootMode == WarmBootFast) || (host->var.mem.subBootMode == ColdBootFast)){
        dataControl2.Data = (*channelNvList)[ch].dataControl2[strobe];
        dataControl4.Data = (*channelNvList)[ch].dataControl4[strobe];
        dataControl3.Data = (*channelNvList)[ch].dataControl3[strobe];
      }
      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL1_0_MCIO_DDRIO_REG), dataControl1.Data);
      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL2_0_MCIO_DDRIO_REG), dataControl2.Data);
      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL3_0_MCIO_DDRIO_REG), dataControl3.Data);
      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL4_0_MCIO_DDRIO_REG), dataControl4.Data);
      if (host->var.common.cpuType == CPU_HSX) {
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, COMPDATA0_0_MCIO_DDRIO_REG), compData0Hsx.Data);
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, COMPDATA1_0_MCIO_DDRIO_REG), compData1Hsx.Data);
      } else if (host->var.common.cpuType == CPU_BDX) {
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, COMPDATA0_0_MCIO_DDRIO_REG), compData0Bdx.Data);
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, COMPDATA1_0_MCIO_DDRIO_REG), compData1Bdx.Data);
      }
    } // strobe loop

    for (strobe = 0; strobe < MAX_STROBE; strobe++) {
      MemWritePciCfgEp (host, socket, ch, txXtalkTable[strobe], 0);
    } // strobe loop

    //
    // Construct CKE mask
    //
    dimmNvList  = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      rankList = &(*dimmNvList)[dimm].rankList;
      rankStruct  = &host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct;

      for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        if (((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)) || (host->var.mem.xoverModeVar == XOVER_MODE_2TO2)) {

          if (host->var.common.bootMode == NormalBoot && host->var.mem.subBootMode == ColdBoot){

            if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
              //
              // Save Tx Vref
              //
              (*channelNvList)[ch].txVrefSafe[(*rankList)[rank].rankIndex] = txVref + DDR4_VREF_RANGE1_OFFSET;
              for (strobe = 0; strobe < MAX_STROBE; strobe++) {
#ifdef  LRDIMM_SUPPORT
                if (host->nvram.mem.socket[socket].channelList[ch].lrDimmPresent) {
                  if ((*dimmNvList)[dimm].SPDSpecRev > 7) {
                    // For SPD rev > 7, get the TxVref value from SPD
                    switch (rank) {
                      case 0:
                        (*rankList)[rank].lrbufTxVref[strobe] = (*dimmNvList)[dimm].SPDLrbufDramVrefdqR0;
                        break;

                      case 1:
                        (*rankList)[rank].lrbufTxVref[strobe] = (*dimmNvList)[dimm].SPDLrbufDramVrefdqR1;
                        break;

                      case 2:
                        (*rankList)[rank].lrbufTxVref[strobe] = (*dimmNvList)[dimm].SPDLrbufDramVrefdqR2;
                        break;

                      case 3:
                        (*rankList)[rank].lrbufTxVref[strobe] = (*dimmNvList)[dimm].SPDLrbufDramVrefdqR3;
                        break;
                    }
                    (*rankStruct)[rank].lrbufTxVrefCache[strobe] = (*rankList)[rank].lrbufTxVref[strobe];
                    // Set MR6 value with training mode enabled
                    (*rankStruct)[rank].MR6[strobe] = (UINT8)((*rankList)[rank].lrbufTxVref[strobe] | BIT7);
                    //also save backside rxVref here
                    (*rankList)[rank].lrbufRxVref[strobe] = (*dimmNvList)[dimm].SPDLrbufDbVrefdq;
                    (*rankStruct)[rank].lrbufRxVrefCache[strobe] = (*rankList)[rank].lrbufRxVref[strobe];
                  } else {
                    if ((*dimmNvList)[dimm].numDramRanks == 4) {
                      //Backside DRAM txVref, hard code MR6 value for 78.2% = 0x1C with training mode enabled
                      (*rankStruct)[rank].MR6[strobe] = (0x1C | BIT7);
                      (*rankList)[rank].lrbufTxVref[strobe] = 0x1C;
                      (*rankStruct)[rank].lrbufTxVrefCache[strobe] = 0x1C;
                      //also save backside rxVref here
                      //quad rank buffer backside vref: 82.1% = 0x22 (range 1)
                      (*rankList)[rank].lrbufRxVref[strobe] = 0x22;
                      (*rankStruct)[rank].lrbufRxVrefCache[strobe] = 0x22;
                    } else {
                      //Backside DRAM txVref, hard code MR6 value for 74% = 0x16 with training mode enabled
                      (*rankStruct)[rank].MR6[strobe] = (0x16 | BIT7);
                      (*rankList)[rank].lrbufTxVref[strobe] = 0x16;
                      (*rankStruct)[rank].lrbufTxVrefCache[strobe] = 0x16;
                      //also save backside rxVref here
                      //dual rank buffer backside vref: 76.9% = 0x1A (range 1)
                      (*rankList)[rank].lrbufRxVref[strobe] = 0x1A;
                      (*rankStruct)[rank].lrbufRxVrefCache[strobe] = 0x1A;
                    }
                  }
                } else
#endif
                {
                  (*rankStruct)[rank].MR6[strobe] = (txVref | BIT7);
                }
                (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][strobe] = (*channelNvList)[ch].txVrefSafe[(*rankList)[rank].rankIndex];
                (*channelNvList)[ch].txVrefCache[(*rankList)[rank].rankIndex][strobe] = (*channelNvList)[ch].txVrefSafe[(*rankList)[rank].rankIndex];
              } // strobe loop
            }
          }

          if (IsDESku(host)) {
            rxDqDelay = 2;
          } else {
            rxDqDelay = 1;
          }

          for (strobe = 0; strobe < MAX_STROBE; strobe++) {

            MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, (*rankList)[rank].logicalRank, strobe, RXGROUP0N0RANK0_0_MCIO_DDRIO_REG), 0x0);
            MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, (*rankList)[rank].logicalRank, strobe, RXGROUP1N0RANK0_0_MCIO_DDRIO_REG), 0);
            MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, (*rankList)[rank].logicalRank, strobe, TXGROUP0N0RANK0_0_MCIO_DDRIO_REG), 0x0);
            MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, (*rankList)[rank].logicalRank, strobe, TXGROUP1N0RANK0_0_MCIO_DDRIO_REG), 0x00208208);

            if (host->var.common.bootMode == NormalBoot && host->var.mem.subBootMode == ColdBoot){

              (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][strobe] |= rxOffset.Data;
              MemWritePciCfgEp (host, socket, ch, rxOffsetTable[strobe] + ((*rankList)[rank].logicalRank * 4),
                                (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][strobe]);

              if ((strobe == 9) && ((*dimmNvList)[dimm].x4Present == 0)) {
                if (IsDESku(host)) {
                  rxDqDelay = 3;
                } else {
                  rxDqDelay = 2;
                }
              }
              GetSetDataGroup (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, RxDqDelay, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &rxDqDelay);
            } //if bootMode
          } // strobe loop
        }

        if ((host->var.common.bootMode == S3Resume) || (host->var.mem.subBootMode == WarmBootFast) || (host->var.mem.subBootMode == ColdBootFast)) {
          if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
           //Retrieve TxVref, RxVref and MR6 values from NVRAM for ColdBootFast/WarmBootFast/S3Resume
            for (strobe = 0; strobe < MAX_STROBE; strobe++) {
#ifdef  LRDIMM_SUPPORT
              if (host->nvram.mem.socket[socket].channelList[ch].lrDimmPresent) {
                (*rankStruct)[rank].lrbufTxVrefCache[strobe] = (*rankList)[rank].lrbufTxVref[strobe];
                // Set MR6 value with training mode enabled
                (*rankStruct)[rank].MR6[strobe] = (UINT8)((*rankList)[rank].lrbufTxVref[strobe] | BIT7);
                (*rankStruct)[rank].lrbufRxVrefCache[strobe] = (*rankList)[rank].lrbufRxVref[strobe];
              } else
#endif
              {
                (*rankStruct)[rank].MR6[strobe] = ((*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][strobe] | BIT7);
              }
            }//strobe
          } //SPD_TYPE_DDR4
        }// if bootMode
      } // rank loop
    } // dimm loop
  } // ch loop

  if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)){

    //
    // Initialize DDRIO cached values
    //
    CacheDDRIO(host, socket);
  }

  //
  // Enable clocks
  //

  if (host->var.common.cpuType == CPU_BDX) {
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      ddrCRClkRanksUsed.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG);
      //
      // Make sure DDR Clocks are enabled
      //
      ddrCRClkRanksUsed.Bits.drvpupdis = 0;
      MemWritePciCfgEp (host, socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG,  ddrCRClkRanksUsed.Data);
    } //ch loop
  } // if BDX

  if ((host->var.common.bootMode == S3Resume) || (host->var.mem.subBootMode == WarmBootFast)) {

    CrossoverCalib(host);

    if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && (host->nvram.mem.dimmTypePresent == RDIMM)) {
      //if (!(IsBDXEP(host) && CheckSteppingGreaterThan( host, CPU_BDX, A0_REV_BDX))) {  // WA reversed for BDX ML B0 s5003486--remove this line

        //s4987926/s5001711 WA starts
        for (ch = 0; ch < MAX_CH; ch++) {
          if ((*channelNvList)[ch].enabled == 0) continue;

          // Save original value
          ddrCRClkRanksUsed.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG);
          save_ddrCRClkRanksUsed[ch].Data = ddrCRClkRanksUsed.Data;

          // Set drvpupdis bit to pull CK low
          ddrCRClkRanksUsed.Bits.drvpupdis = 1;

          // Apply setting to each channel
          MemWritePciCfgEp (host, socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, ddrCRClkRanksUsed.Data);
        } //ch loop
        //s4987926/s5001711 WA ends
      // } s5003486--remove this line
    }  //if DDR4 RDIMM

    // Original DDRIO init code
    MemWritePciCfgEp (host, socket, 0, DDRCRCOMPVSSHICONTROL_MCIO_DDRIOEXTMCC_REG, vssControl.Data);
    MemWritePciCfgEp (host, socket, 2, DDRCRCOMPVSSHICONTROL_MCIO_DDRIOEXTMCC_REG, vssControl.Data);

    if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && (host->nvram.mem.dimmTypePresent == RDIMM)) {
      //  if (!(IsBDXEP(host) && CheckSteppingGreaterThan( host, CPU_BDX, A0_REV_BDX))) {  // WA reversed for BDX ML B0 s5003486--remove this line

        // s4987926/s5001711 WA re-starts
        // Delay for Register to recognize CK stop mode
        FixedDelay(host, 10);

        for (ch = 0; ch < MAX_CH; ch++) {
          if ((*channelNvList)[ch].enabled == 0) continue;

          // Restore drvpupdis setting on each channel
          MemWritePciCfgEp (host, socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, save_ddrCRClkRanksUsed[ch].Data);
        } //ch loop
        //s4987926/s5001711 WA ends
      //} s5003486--remove this line
    } //if DDR4 RDIMM

    //
    // Give clocks clocks Vss Hi time to lock
    //
    FixedDelay(host, 5);

  }//if S3Resume || WarmBootFast

  //
  // Start a COMP cycle
  //
  DoComp (host, socket);

  //
  // This workaround applies to B0 (HSD 4167608)
  //
  if (CheckSteppingEqual(host, CPU_HSX, B0_REV_HSX)) {

    ddrCrCompOvr.Bits.dqdrvu  = 1;
    ddrCrCompOvr.Bits.dqdrvd  = 1;
    ddrCrCompOvr.Bits.dqodtu  = 1;
    ddrCrCompOvr.Bits.dqodtd  = 1;
    ddrCrCompOvr.Bits.ctldrvu = 1;
    ddrCrCompOvr.Bits.ctldrvd = 1;
    ddrCrCompOvr.Bits.clkdrvu = 1;
    ddrCrCompOvr.Bits.clkdrvd = 1;
    ddrCrCompOvr.Bits.dqtco   = 1;
    ddrCrCompOvr.Bits.cmdtco  = 1;
    ddrCrCompOvr.Bits.ctltco  = 1;
    ddrCrCompOvr.Bits.clktco  = 1;
  }
  if (host->var.common.cpuType == CPU_HSX){
    ddrCrCompOvr.Bits.cmddrvu = 1;
    ddrCrCompOvr.Bits.cmddrvd = 1;
  }
  MemWritePciCfgEp (host, socket, 0, DDRCRCOMPOVR_MCIO_DDRIOEXTMCC_REG, ddrCrCompOvr.Data);
  MemWritePciCfgEp (host, socket, 2, DDRCRCOMPOVR_MCIO_DDRIOEXTMCC_REG, ddrCrCompOvr.Data);

  //
  // Hard coded COMP settings
  //
  host->var.common.rcWriteRegDump = 0;
  ProgramIOCompValues(host, socket);
  host->var.common.rcWriteRegDump = 1;

  if (!((host->var.common.bootMode == S3Resume) || (host->var.mem.subBootMode == WarmBootFast))) {
    CrossoverCalib(host);
  }

  if (((host->var.common.bootMode == NormalBoot) && ((host->var.mem.subBootMode == ColdBoot) || (host->var.mem.subBootMode == ColdBootFast))) || (host->var.mem.subBootMode == AdrResume)){

    if (host->var.mem.subBootMode == ColdBoot){
      //
      // Initialize CMD/CTL/CLK pi delays
      //
      SetStartingCCC (host, socket);
    }

    //
    // Perform the JEDEC initialization sequence
    //
    JedecInitSequence(host, socket, CH_BITMASK);


    if (host->var.mem.subBootMode == ColdBoot){
      if (host->var.common.cpuType == CPU_HSX) {
        SenseAmpOffset(host);
      } else {
        SenseAmpOffsetBdx(host);
      }
    }

  }

  //
  // Disable Clocks that are not used
  //
  MaxCh = (IsDESku(host))? 2:4;
  for (ch = 0; ch < MaxCh; ch++) {
    ddrCRClkRanksUsed.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG);
  
    if ((IsMccChop(host) && CheckSteppingLessThan(host, CPU_BDX, B0_REV_BDX)) ||
        (CheckSteppingEqual(host, CPU_BDX, B0_REV_BDX) && IsBDXEP(host))  ||
        (IsDEHccSku(host)) ) {
      ddrCRClkRanksUsed.Bits.disablepc6fix = 0;
    }
  
    if ((*channelNvList)[ch].enabled) {
      MaxDpc = host->var.mem.socket[socket].channelList[ch].numDimmSlots;
      dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
      NumDimmsPoped = 0;
  
      for (dimm = 0; dimm < MaxDpc; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
        NumDimmsPoped = NumDimmsPoped + 1;
      }
      MemDebugPrint((host, SDBG_MINMAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "Number of DIMMS in channel: %d\n", NumDimmsPoped));
  
      if (NumDimmsPoped == 0) {
        ddrCRClkRanksUsed.Bits.ranken = 0;
      } else if (NumDimmsPoped == 1) {
        if (host->nvram.mem.dimmTypePresent == RDIMM) {
          ddrCRClkRanksUsed.Bits.ranken = 0x1;
        } else { // UDIMM
          if (1 == (*dimmNvList)[0].numRanks) { //Single rank UDIMM in Slot 0
            ddrCRClkRanksUsed.Bits.ranken = 0x1;
          } else { // Dual ranked UDIMM
            ddrCRClkRanksUsed.Bits.ranken = 0x5;
          }
        }
      } else if (NumDimmsPoped == 2) {
        if (host->nvram.mem.dimmTypePresent == RDIMM) {
          ddrCRClkRanksUsed.Bits.ranken = 0x3;
        } else { // UDIMM
          if ((1 == (*dimmNvList)[0].numRanks) && (1 == (*dimmNvList)[1].numRanks))  {//Single rank UDIMM in both slots
            ddrCRClkRanksUsed.Bits.ranken = 0x3;
          } else if ((2 == (*dimmNvList)[0].numRanks) && (2 == (*dimmNvList)[1].numRanks)) {//Dual ranked UDIMM in both slots
            ddrCRClkRanksUsed.Bits.ranken = 0xF;
          } else if ((1 == (*dimmNvList)[0].numRanks) && (2 == (*dimmNvList)[1].numRanks)) {//Single rank UDIMM in slot 0 and Dual rank UDIMM in Slot 1
            ddrCRClkRanksUsed.Bits.ranken = 0xB;
          } else { // Dual rank UDIMM in slot 0 and Single rank UDIMM in Slot 1
            ddrCRClkRanksUsed.Bits.ranken = 0x7;
          }
        }
      } else if (NumDimmsPoped == 3) {//3DPC
        ddrCRClkRanksUsed.Bits.ranken = 0x7;
      } else {// Error Handler
        MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "Halting at clock disable for empty slots.\n"));
        FatalError(host, 0, 0);// We are here because of a Bug.
      }
    } else {
      ddrCRClkRanksUsed.Bits.ranken = 0;
    }
    (*channelNvList)[ch].ddrCRClkRanksUsed = ddrCRClkRanksUsed.Data;
    MemWritePciCfgEp (host, socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, ddrCRClkRanksUsed.Data);
  }//ch loop

  return SUCCESS;
} // InitDdrioInterface


/**

  Programs IO delays

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - Bit-field of different modes
  @param value   - Pointer to delay value or offset based on mode

  @retval MRC_STATUS

**/
MRC_STATUS
GetSetDataGroup (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  GSM_LT    level,
  GSM_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  MRC_STATUS  status = MRC_STATUS_SUCCESS;

  switch (group) {
    case TxDqDelay:
    case TxDqsDelay:
    case TxDqsDelayCycle:
    case TxEq:
      status = GetSetTxDelay (host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;

    case TxDqBitDelay:
      status = GetSetTxDelayBit (host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;

    case RxDqsBitDelay:
    case RxDqsPBitDelay:
    case RxDqsNBitDelay:
      status = GetSetRxDelayBit (host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;

    case RecEnDelay:
    case RecEnDelayCycle:
    case RxDqsPDelay:
    case RxDqsNDelay:
    case RxDqsDelay:
    case RxDqDelay:
    case RxEq:
      status = GetSetRxDelay (host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;

    case RxVref:
      status = GetSetRxVref (host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;

    case TxVref:
      if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
        status = GetSetTxVrefDDR4 (host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      } else {
        status = GetSetTxVref (host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      }
      break;
    case TxImode:
      status = GetSetImode (host,socket,ch,strobe,mode,value);
      break;
    case RxCtleC:
    case RxCtleR:
      status = GetSetCTLE (host,socket,ch,dimm,rank,strobe,bit,level,group,mode,value);
      break;
    case RxOdt:
    case TxRon:
      status = GetSetCPUODT (host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;
    case DramDrvStr:
    case ParkOdt:
    case NomOdt:
    case WrOdt:
      status = GetSetDIMMODT (host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
      break;
    default:
      MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                    "GetSetDataGroup called unknown group!\n"));
      RC_ASSERT(FALSE, ERR_RC_INTERNAL, 0);
      break;

  }

  return status;
} // GetSetDataGroup

/**

  Get TxDelay

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - Bit-field of different modes
  @param value   - Pointer to delay value or offset based on mode

  @retval IO delay

**/
MRC_STATUS
GetSetTxDelay (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  GSM_LT    level,
  GSM_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8                                     bitlane;
  UINT8                                     logRank;
  UINT8                                     maxStrobe;
  UINT16                                    curVal = 65535;
  UINT16                                    curVal2 = 65535;
  UINT16                                    piDelay;
  UINT16                                    perBitOffset;
  INT16                                     tmp;
  UINT8                                     counter;
  UINT16                                    min;
  UINT16                                    maxLimit = 0;
  UINT16                                    minLimit = 0;
  UINT16                                    usDelay = 0;
  UINT32                                    TxPerBitSetup2to2Offset;
#ifdef SERIAL_DBG_MSG
  char                                      strBuf0[128];
#endif  // SERIAL_DBG_MSG
  struct channelNvram                       (*channelNvList)[MAX_CH];
  TXGROUP0N0RANK0_0_MCIO_DDRIO_BDX_STRUCT   txGroup0;
  TXGROUP1N0RANK0_0_MCIO_DDRIO_STRUCT       txGroup1;
  TXXTALKN0_0_MCIO_DDRIO_STRUCT             txXtalk;
  TXXTALKN0_0_MCIO_DDRIO_STRUCT             txXtalkCache;
  DATAOFFSETTRAIN_0_MCIO_DDRIO_STRUCT       dataOffsetTrain;
  DATACONTROL0_0_MCIO_DDRIO_STRUCT          dataControl0;
#ifdef LRDIMM_SUPPORT
  UINT8                                     updateNeeded;
  UINT8                                     *controlWordDataPtr;
  UINT8                                     controlWordAddr;
  struct dimmNvram                          (*dimmNvList)[MAX_DIMM];
  struct rankDevice                         (*rankStruct)[MAX_RANK_DIMM];
  UINT16                                     UIOffsets[4];
#endif //LRDIMM_SUPPORT
  channelNvList = GetChannelNvList(host, socket);

#ifdef LRDIMM_SUPPORT
  rankStruct  = &host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct;
  dimmNvList = GetDimmNvList(host, socket, ch);
#endif //LRDIMM_SUPPORT

  txGroup0.Data = 0;
  txGroup1.Data = 0;
  txXtalk.Data = 0;
  dataOffsetTrain.Data = 0;
  dataControl0.Data = 0;
  TxPerBitSetup2to2Offset = TX_PER_BIT_SETUP_2TO2_OFFSET;

  //
  // Get the logical rank #
  //
  logRank = GetLogicalRank(host, socket, ch, dimm, rank);

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits(host, level, group, &minLimit, &maxLimit, &usDelay);

  if (strobe == ALL_STROBES) {
    strobe      = 0;
    maxStrobe   = MAX_STROBE;
  } else {
    maxStrobe = strobe + 1;
  }

#ifdef LRDIMM_SUPPORT
  updateNeeded = 0;
#endif //LRDIMM_SUPPORT
  if (host->setup.mem.perBitDeSkew) {
    for (strobe; strobe < maxStrobe; strobe++) {
      if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

      bitlane = 0;
      while (bitlane < 4) {
        if ((bitlane > 0 && host->var.mem.xoverModeVar != XOVER_MODE_2TO2) || (bitlane > 0 && host->var.mem.xoverModeVar == XOVER_MODE_2TO2 && (group != TxDqDelay || (group == TxDqDelay && level == LrbufLevel)))) {
          break;
        }
        if(curVal < curVal2) {
          curVal2 = curVal;
        }

#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {

        if(mode & GSM_READ_CSR) {
          //
          // Read from "actual setting" cache
          //
          switch (group) {
            case TxDqDelay:
              if ((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] < 16) {
                curVal = (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] + 15;
              } else {
                curVal = (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] ^ 31;
              }
              break;
            case TxDqsDelay:
              curVal = (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCAxBx[strobe];
              break;
            case TxDqsDelayCycle:
              curVal = (((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[strobe % 9] >> (4 * (strobe / 9))) & 0xF);
              break;
            default:
              MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                            "GetSetTxDelay called unsupported group!\n"));
              RC_ASSERT(FALSE, ERR_RC_INTERNAL, 0);
              break;
          } //switch group
        } else {
          //
          // Read from setting cache
          //
          switch (group) {
            case TxDqDelay:
              if ((*rankStruct)[rank].cachedLrBuf_FxBC8x9x[strobe] < 16) {
                curVal = (*rankStruct)[rank].cachedLrBuf_FxBC8x9x[strobe] + 15;
              } else {
                 curVal = (*rankStruct)[rank].cachedLrBuf_FxBC8x9x[strobe] ^ 31;
              }
              break;
            case TxDqsDelay:
              curVal = (*rankStruct)[rank].cachedLrBuf_FxBCAxBx[strobe];
              break;
            case TxDqsDelayCycle:
              curVal = (((*rankStruct)[rank].cachedLrBuf_FxBCDxFx[strobe % 9] >> (4 * (strobe / 9))) & 0xF);
              break;
            default:
              MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                            "GetSetTxDelay called unsupported group!\n"));
              RC_ASSERT(FALSE, ERR_RC_INTERNAL, 0);
              break;
          } //switch group
        } //mode
      } else
#endif //LRDIMM_SUPPORT
        {
          if (bitlane == 0) {
            if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
              dataOffsetTrain.Data = (*channelNvList)[ch].dataOffsetTrain[strobe];
             }

             if(mode & GSM_READ_CSR) {
             //
             // Read from PCI config space
             //
             txGroup0.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, TXGROUP0N0RANK0_0_MCIO_DDRIO_REG));

             if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
               txGroup1.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, TXGROUP1N0RANK0_0_MCIO_DDRIO_REG));
               txXtalk.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister2 (host, strobe, TXXTALKN0_0_MCIO_DDRIO_REG));
               tmp = strobe % 9;
               dataControl0.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, (UINT8)tmp, DATACONTROL0_0_MCIO_DDRIO_REG));
             }
           } else {
           //
           // Read from cache
           //
           txGroup0.Data = (*channelNvList)[ch].txGroup0[logRank][strobe];

           if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
             txGroup1.Data = (*channelNvList)[ch].txGroup1[logRank][strobe];
             txXtalk.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister2 (host, strobe, TXXTALKN0_0_MCIO_DDRIO_REG)); // Due to the different register architecture of TxXtalk, we must always read it from the register and then replace the rank field we are using with data from the cache.
             txXtalkCache.Data = (*channelNvList)[ch].txXtalk[strobe];
             switch (logRank) {
               case 0:
                 txXtalk.Bits.twototwologicdelayrank0 = txXtalkCache.Bits.twototwologicdelayrank0;
                 break;
               case 1:
                 txXtalk.Bits.twototwologicdelayrank1 = txXtalkCache.Bits.twototwologicdelayrank1;
                 break;
               case 2:
                 txXtalk.Bits.twototwologicdelayrank2 = txXtalkCache.Bits.twototwologicdelayrank2;
                 break;
               case 3:
                 txXtalk.Bits.twototwologicdelayrank3 = txXtalkCache.Bits.twototwologicdelayrank3;
                 break;
               case 4:
                 txXtalk.Bits.twototwologicdelayrank4 = txXtalkCache.Bits.twototwologicdelayrank4;
                 break;
               case 5:
                 txXtalk.Bits.twototwologicdelayrank5 = txXtalkCache.Bits.twototwologicdelayrank5;
                 break;
               case 6:
                 txXtalk.Bits.twototwologicdelayrank6 = txXtalkCache.Bits.twototwologicdelayrank6;
                 break;
               case 7:
                 txXtalk.Bits.twototwologicdelayrank7 = txXtalkCache.Bits.twototwologicdelayrank7;
                 break;
             }
             dataControl0.Data = (*channelNvList)[ch].dataControl0;
           }
         }
       }
       switch (group) {
         case TxDqDelay:

           if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
              dataOffsetTrain.Data = (*channelNvList)[ch].dataOffsetTrain[strobe];
              switch(bitlane) {
                case 0:
                  curVal = GetTwoTwoLogicDelay(host, &txXtalk, logRank, bitlane, 0) * 64 + ((UINT16)txGroup0.Bits.txdqdelay & (BIT8 | BIT7 | BIT6)) | (((UINT16)txGroup1.Bits.txdqpicode0 + 128 - (dataOffsetTrain.Bits.txdqoffset + (UINT16)TxPerBitSetup2to2Offset)) % 64);
                  break;
                case 1:
                  curVal = GetTwoTwoLogicDelay(host, &txXtalk, logRank, bitlane, 0) * 64 + ((UINT16)txGroup0.Bits.txdqdelay & (BIT8 | BIT7 | BIT6)) | (((UINT16)txGroup1.Bits.txdqpicode1 + 128 - (dataOffsetTrain.Bits.txdqoffset + (UINT16)TxPerBitSetup2to2Offset)) % 64);
                  break;
                case 2:
                  curVal = GetTwoTwoLogicDelay(host, &txXtalk, logRank, bitlane, 0) * 64 + ((UINT16)txGroup0.Bits.txdqdelay & (BIT8 | BIT7 | BIT6)) | (((UINT16)txGroup1.Bits.txdqpicode2 + 128 - (dataOffsetTrain.Bits.txdqoffset + (UINT16)TxPerBitSetup2to2Offset)) % 64);
                  break;
                case 3:
                  curVal = GetTwoTwoLogicDelay(host, &txXtalk, logRank, bitlane, 0) * 64 + ((UINT16)txGroup0.Bits.txdqdelay & (BIT8 | BIT7 | BIT6)) | (((UINT16)txGroup1.Bits.txdqpicode3 + 128 - (dataOffsetTrain.Bits.txdqoffset + (UINT16)TxPerBitSetup2to2Offset)) % 64);
                  break;
              }
            } else {
              curVal = (UINT16)txGroup0.Bits.txdqdelay;
            }
            break;

          case TxDqsDelay:
            curVal = (UINT16)txGroup0.Bits.txdqsdelay;
            break;

          case TxEq:
            curVal = (UINT16)txGroup0.Bits.txeq;
            break;
        } // switch
      }

      //
      // Check if read only
      //
      if (mode & GSM_READ_ONLY) {
        // The overall TxDqDelay value returned in 2:2 mode should be the max (per-bit value added to the nibble value minus the 2:2 logic delay value)
        if (group == TxDqDelay && host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
          if(curVal < curVal2) {
            *value = curVal;
          }
        } else {
          *value = curVal;
        }
      } else {
        //
        // Write
        //
        // Adjust the current CMD delay value by offset
        if (mode & GSM_WRITE_OFFSET) {
          tmp = curVal + *value;
        } else {
          tmp = *value;
        }

        if (tmp >= minLimit) {
          piDelay = tmp;
        } else {
          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                        "%s Out of range!! tmp = 0x%x, minLimit = 0x%x\n", GetGroupStr(group, (char *)&strBuf0), tmp, minLimit));
          piDelay = minLimit;
        }
        // Ensure we do not exceed maximums
        if (piDelay > maxLimit) {
          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                        "%s Out of range!! piDelay = 0x%x, maxLimit = 0x%x\n", GetGroupStr(group, (char *)&strBuf0), piDelay, maxLimit));
          piDelay = maxLimit;
        }

#ifdef LRDIMM_SUPPORT
        if (level == LrbufLevel) {
          curVal = piDelay;

          switch (group) {
            case TxDqDelay:
              if (curVal < 16) {
                if ((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] != (UINT8) (curVal ^ 31)) {
                  updateNeeded = 1;
                  (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] = (UINT8) (curVal ^ 31);
                }
              } else {
                if ((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] != (UINT8) (curVal - 15)) {
                  updateNeeded = 1;
                  (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] = (UINT8) (curVal - 15);
                }
              }
              break;
            case TxDqsDelay:
              if ((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCAxBx[strobe] != (UINT8) curVal) {
                updateNeeded = 1;
                (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] = (UINT8) curVal;
              }
              break;
            case TxDqsDelayCycle:
              if ((((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[strobe % 9]>>(4 * (strobe / 9))) & 0xF) != (UINT8) curVal) {
                updateNeeded = 1;
                (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[strobe % 9] &= 0xF0 >> (4 * (strobe / 9));
                (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[strobe % 9] |= (UINT8) curVal<<(4 * (strobe / 9));
              }
              break;
            default:
              break;
          }

        } else
#endif  //LRDIMM
        {
          switch (group) {
            case TxDqDelay:
              dataOffsetTrain.Data = (*channelNvList)[ch].dataOffsetTrain[strobe];
              if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
                switch(bitlane) {
                  case 0:
                    txGroup1.Bits.txdqpicode0 = (piDelay + dataOffsetTrain.Bits.txdqoffset + (UINT16)TxPerBitSetup2to2Offset) % 64;
                    break;
                  case 1:
                    txGroup1.Bits.txdqpicode1 = (piDelay + dataOffsetTrain.Bits.txdqoffset + (UINT16)TxPerBitSetup2to2Offset) % 64;
                     break;
                  case 2:
                    txGroup1.Bits.txdqpicode2 = (piDelay + dataOffsetTrain.Bits.txdqoffset + (UINT16)TxPerBitSetup2to2Offset) % 64;
                    break;
                  case 3:
                    txGroup1.Bits.txdqpicode3 = (piDelay + dataOffsetTrain.Bits.txdqoffset + (UINT16)TxPerBitSetup2to2Offset) % 64;
                    break;
                }

                UIOffsets[bitlane] = piDelay & (BIT8|BIT7|BIT6); // Will end up being a multiple of 64

                if(bitlane == 3) {
                  // Find the minimum of the values, and set the nibble offset to that. Any extra per bit will have to be set to the 2:2 offset bits
                  min = 448;
                  for(counter = 0; counter < 4; counter++) {
                    if (UIOffsets[counter] < min) {
                      min = UIOffsets[counter];
                    }
                  }
                  for(counter = 0; counter < 4; counter++) {
                    SetTwoTwoLogicDelay(host, &txXtalk, logRank, counter, ((UIOffsets[counter] - min) >> 6), 0);
                    }

                    txGroup0.Bits.txdqdelay = txGroup0.Bits.txdqdelay & (BIT5|BIT4|BIT3|BIT2|BIT1|BIT0); // Clear the upper bits of the register, the lower ones will be 0 anyways
                    txGroup0.Bits.txdqdelay = txGroup0.Bits.txdqdelay | min;

                  }

                  if ((piDelay % 64) > 31) {
                    switch(bitlane) {
                      case 0:
                        txGroup0.Bits.txdqxsel0 = 1;
                        break;
                      case 1:
                        txGroup0.Bits.txdqxsel1 = 1;
                        break;
                      case 2:
                        txGroup0.Bits.txdqxsel2 = 1;
                        break;
                      case 3:
                        txGroup0.Bits.txdqxsel3 = 1;
                        break;
                    }
                  } else {
                    switch(bitlane) {
                      case 0:
                        txGroup0.Bits.txdqxsel0 = 0;
                        break;
                      case 1:
                        txGroup0.Bits.txdqxsel1 = 0;
                        break;
                      case 2:
                        txGroup0.Bits.txdqxsel2 = 0;
                        break;
                      case 3:
                        txGroup0.Bits.txdqxsel3 = 0;
                        break;
                    }
                  }
                } else {
                  txGroup0.Bits.txdqdelay = piDelay;
                  dataOffsetTrain.Data = (*channelNvList)[ch].dataOffsetTrain[strobe];
                  perBitOffset = (((dataOffsetTrain.Bits.txdqoffset + piDelay) % 64) + TX_PER_BIT_SETTLE_TIME) % 64;
                  txGroup1.Bits.txdqpicode0 = (perBitOffset + (*channelNvList)[ch].txPerBitDeskew[logRank][strobe][0]) % 64;
                  txGroup1.Bits.txdqpicode1 = (perBitOffset + (*channelNvList)[ch].txPerBitDeskew[logRank][strobe][1]) % 64;
                  txGroup1.Bits.txdqpicode2 = (perBitOffset + (*channelNvList)[ch].txPerBitDeskew[logRank][strobe][2]) % 64;
                  txGroup1.Bits.txdqpicode3 = (perBitOffset + (*channelNvList)[ch].txPerBitDeskew[logRank][strobe][3]) % 64;
                }
                break;

              case TxDqsDelay:
                txGroup0.Bits.txdqsdelay = piDelay;
                if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
                  if ((piDelay % 64) > 31) {
                    txGroup0.Bits.txdqsxsel = 1;
                  } else {
                    txGroup0.Bits.txdqsxsel = 0;
                  }
                }
                break;

              case TxEq:
                txGroup0.Bits.txeq = piDelay;
                break;
            } // switch

            if (bitlane == 0 || bitlane == 3) {
              if (!(bitlane == 0 && host->var.mem.xoverModeVar == XOVER_MODE_2TO2 && group == TxDqDelay && level != LrbufLevel)) {
                //
                // Write it back if the current data does not equal the cache value or if in force write mode
                //
                if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
                  if ((group == TxDqDelay) && ((txXtalk.Data != (*channelNvList)[ch].txXtalk[strobe]) || (mode & GSM_FORCE_WRITE))) {
                    MemWritePciCfgEp (host, socket, ch, UpdateIoRegister2 (host, strobe, TXXTALKN0_0_MCIO_DDRIO_REG), txXtalk.Data);
                    dataControl0.Bits.readrfwr = 1;
                    dataControl0.Bits.readrfrank = logRank;
                    FixedDelay(host, usDelay);
                  }
                  if ((group == TxDqDelay)&&((txGroup1.Data != (*channelNvList)[ch].txGroup1[logRank][strobe]) || (mode & GSM_FORCE_WRITE))) {
                    MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, TXGROUP1N0RANK0_0_MCIO_DDRIO_REG), txGroup1.Data);
                    FixedDelay(host, usDelay);
                  }
                  if ((txGroup0.Data != (*channelNvList)[ch].txGroup0[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
                    MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, TXGROUP0N0RANK0_0_MCIO_DDRIO_REG), txGroup0.Data);
                    FixedDelay(host, usDelay);
                  }
                  if ((dataControl0.Data != (*channelNvList)[ch].dataControl0) || (mode & GSM_FORCE_WRITE)) {
                    MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
                    dataControl0.Bits.readrfwr = 0; // Make sure the cache value is correct
                    FixedDelay(host, usDelay);
                  }
                } else {
                  if ((txGroup0.Data != (*channelNvList)[ch].txGroup0[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
                    MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, TXGROUP0N0RANK0_0_MCIO_DDRIO_REG), txGroup0.Data);
                    if (group == TxDqDelay) {
                      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, TXGROUP1N0RANK0_0_MCIO_DDRIO_REG), txGroup1.Data);
                    }

                    //
                    // Wait for the new value to settle
                    //
                    FixedDelay(host, usDelay);
                  }
                }//if
              } //2-2
            } //else
          } //bitlane
        } // if read only

        if (bitlane == 0 || bitlane == 3) {
          if(!(bitlane == 0 && host->var.mem.xoverModeVar == XOVER_MODE_2TO2 && group == TxDqDelay && level != LrbufLevel)) {
            //
            // Save to cache
            //
            if (mode & GSM_UPDATE_CACHE) {
              if (level == DdrLevel) {
                (*channelNvList)[ch].txGroup0[logRank][strobe] = txGroup0.Data;
                if (group == TxDqDelay) {
                  (*channelNvList)[ch].txGroup1[logRank][strobe] = txGroup1.Data;
                  if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
                    (*channelNvList)[ch].txXtalk[strobe] = txXtalk.Data;
                    (*channelNvList)[ch].dataControl0 = dataControl0.Data;
                  }
                }
              } //level
            }
          }
        }
        bitlane++;
      } // bit loop
    } // strobe loop
  } else { // Grantley implementation

  for (strobe; strobe < maxStrobe; strobe++) {
    if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

#ifdef LRDIMM_SUPPORT
    if (level == LrbufLevel) {

      if(mode & GSM_READ_CSR) {
        //
        // Read from "actual setting" cache
        //
        switch (group) {
          case TxDqDelay:
            if ((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] < 16) {
              curVal = (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] + 15;
            } else {
              curVal = (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] ^ 31;
            }
            break;
          case TxDqsDelay:
            curVal = (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCAxBx[strobe];
            break;
          case TxDqsDelayCycle:
            curVal = (((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[strobe % 9] >> (4 * (strobe / 9))) & 0xF);
            break;
          default:
            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                          "GetSetTxDelay called unsupported group!\n"));
            RC_ASSERT(FALSE, ERR_RC_INTERNAL, 0);
            break;
          } //switch group
      } else {
        //
        // Read from setting cache
        //
        switch (group) {
          case TxDqDelay:
            if ((*rankStruct)[rank].cachedLrBuf_FxBC8x9x[strobe] < 16) {
              curVal = (*rankStruct)[rank].cachedLrBuf_FxBC8x9x[strobe] + 15;
            } else {
               curVal = (*rankStruct)[rank].cachedLrBuf_FxBC8x9x[strobe] ^ 31;
            }
            break;
          case TxDqsDelay:
            curVal = (*rankStruct)[rank].cachedLrBuf_FxBCAxBx[strobe];
            break;
          case TxDqsDelayCycle:
            curVal = (((*rankStruct)[rank].cachedLrBuf_FxBCDxFx[strobe % 9] >> (4 * (strobe / 9))) & 0xF);
            break;
          default:
            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                          "GetSetTxDelay called unsupported group!\n"));
            RC_ASSERT(FALSE, ERR_RC_INTERNAL, 0);
            break;
        } //switch group
      }
    } else
#endif //LRDIMM_SUPPORT
    {
      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        dataOffsetTrain.Data = (*channelNvList)[ch].dataOffsetTrain[strobe];
      }

      if(mode & GSM_READ_CSR) {
        //
        // Read from PCI config space
        //
        txGroup0.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, TXGROUP0N0RANK0_0_MCIO_DDRIO_REG));

        if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
          txGroup1.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, TXGROUP1N0RANK0_0_MCIO_DDRIO_REG));
        }
      } else {
        //
        // Read from cache
        //
        txGroup0.Data = (*channelNvList)[ch].txGroup0[logRank][strobe];

        if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
          txGroup1.Data = (*channelNvList)[ch].txGroup1[logRank][strobe];
        }
      }

      switch (group) {
        case TxDqDelay:

          if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
            dataOffsetTrain.Data = (*channelNvList)[ch].dataOffsetTrain[strobe];
            curVal = ((UINT16)txGroup0.Bits.txdqdelay & (BIT8 | BIT7 | BIT6)) |
                     (((UINT16)txGroup1.Bits.txdqpicode0 + 128 - (dataOffsetTrain.Bits.txdqoffset + (UINT16)TxPerBitSetup2to2Offset)) % 64);
          } else {
            curVal = (UINT16)txGroup0.Bits.txdqdelay;
          }
          break;

        case TxDqsDelay:
            curVal = (UINT16)txGroup0.Bits.txdqsdelay;
          break;

        case TxEq:
            curVal = (UINT16)txGroup0.Bits.txeq;
          break;
      } // switch
    }

    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      *value = curVal;
    } else {
      //
      // Write
      //
      // Adjust the current CMD delay value by offset
      if (mode & GSM_WRITE_OFFSET) {
        tmp = curVal + *value;
      } else {
        tmp = *value;
      }

      if (tmp >= minLimit) {
        piDelay = tmp;
      } else {
        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                      "%s Out of range!! tmp = 0x%x, minLimit = 0x%x\n", GetGroupStr(group, (char *)&strBuf0), tmp, minLimit));
        piDelay = minLimit;
      }
      // Ensure we do not exceed maximums
      if (piDelay > maxLimit) {
        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                      "%s Out of range!! piDelay = 0x%x, maxLimit = 0x%x\n", GetGroupStr(group, (char *)&strBuf0), piDelay, maxLimit));
        piDelay = maxLimit;
      }

#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        curVal = piDelay;

        switch (group) {
          case TxDqDelay:
            if (curVal < 16) {
              if ((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] != (UINT8) (curVal ^ 31)) {
                updateNeeded = 1;
                (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] = (UINT8) (curVal ^ 31);
              }
            } else {
              if ((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] != (UINT8) (curVal - 15)) {
                updateNeeded = 1;
                (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] = (UINT8) (curVal - 15);
              }
            }
            break;
          case TxDqsDelay:
            if ((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCAxBx[strobe] != (UINT8) curVal) {
              updateNeeded = 1;
              (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] = (UINT8) curVal;
            }
            break;
          case TxDqsDelayCycle:
            if ((((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[strobe % 9]>>(4 * (strobe / 9))) & 0xF) != (UINT8) curVal) {
              updateNeeded = 1;
              (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[strobe % 9] &= 0xF0 >> (4 * (strobe / 9));
              (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[strobe % 9] |= (UINT8) curVal<<(4 * (strobe / 9));
            }
            break;
          default:
            break;
        }

      } else
#endif  //LRDIMM
      {
        switch (group) {
          case TxDqDelay:
            dataOffsetTrain.Data = (*channelNvList)[ch].dataOffsetTrain[strobe];
            if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {

              txGroup1.Bits.txdqpicode0 = (piDelay + dataOffsetTrain.Bits.txdqoffset + (UINT16)TxPerBitSetup2to2Offset) % 64;
              txGroup1.Bits.txdqpicode1 = txGroup1.Bits.txdqpicode0;
              txGroup1.Bits.txdqpicode2 = txGroup1.Bits.txdqpicode0;
              txGroup1.Bits.txdqpicode3 = txGroup1.Bits.txdqpicode0;
              txGroup0.Bits.txdqdelay   = txGroup0.Bits.txdqdelay & (BIT5|BIT4|BIT3|BIT2|BIT1|BIT0);
              txGroup0.Bits.txdqdelay   = txGroup0.Bits.txdqdelay | (piDelay & (BIT8|BIT7|BIT6));
              if ((piDelay % 64) > 31) {
                txGroup0.Bits.txdqxsel0 = 1;
              } else {
                txGroup0.Bits.txdqxsel0 = 0;
              }
                txGroup0.Bits.txdqxsel1 = txGroup0.Bits.txdqxsel0;
                txGroup0.Bits.txdqxsel2 = txGroup0.Bits.txdqxsel0;
                txGroup0.Bits.txdqxsel3 = txGroup0.Bits.txdqxsel0;
            } else {
              txGroup0.Bits.txdqdelay = piDelay;
              dataOffsetTrain.Data = (*channelNvList)[ch].dataOffsetTrain[strobe];
              perBitOffset = (((dataOffsetTrain.Bits.txdqoffset + piDelay) % 64) + TX_PER_BIT_SETTLE_TIME) % 64;
              txGroup1.Bits.txdqpicode0 = (perBitOffset + (*channelNvList)[ch].txPerBitDeskew[logRank][strobe][0]) % 64;
              txGroup1.Bits.txdqpicode1 = (perBitOffset + (*channelNvList)[ch].txPerBitDeskew[logRank][strobe][1]) % 64;
              txGroup1.Bits.txdqpicode2 = (perBitOffset + (*channelNvList)[ch].txPerBitDeskew[logRank][strobe][2]) % 64;
              txGroup1.Bits.txdqpicode3 = (perBitOffset + (*channelNvList)[ch].txPerBitDeskew[logRank][strobe][3]) % 64;

            }
            break;

          case TxDqsDelay:
            txGroup0.Bits.txdqsdelay = piDelay;
            if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
              if ((piDelay % 64) > 31) {
                txGroup0.Bits.txdqsxsel = 1;
              } else {
                txGroup0.Bits.txdqsxsel = 0;
              }
            }
            break;

          case TxEq:
            txGroup0.Bits.txeq = piDelay;
            break;
        } // switch

        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {

          if ((group == TxDqDelay)&&((txGroup0.Data != (*channelNvList)[ch].txGroup0[logRank][strobe]) || (txGroup1.Data != (*channelNvList)[ch].txGroup1[logRank][strobe]) || (mode & GSM_FORCE_WRITE))) {
            MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, TXGROUP0N0RANK0_0_MCIO_DDRIO_REG), txGroup0.Data);
            MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, TXGROUP1N0RANK0_0_MCIO_DDRIO_REG), txGroup1.Data);
            FixedDelay(host, usDelay);
          } else {
            if ((txGroup0.Data != (*channelNvList)[ch].txGroup0[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
              MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, TXGROUP0N0RANK0_0_MCIO_DDRIO_REG), txGroup0.Data);
              FixedDelay(host, usDelay);
            }
          }
        } else {
          if ((txGroup0.Data != (*channelNvList)[ch].txGroup0[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
            MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, TXGROUP0N0RANK0_0_MCIO_DDRIO_REG), txGroup0.Data);
            if (group == TxDqDelay) {
              MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, TXGROUP1N0RANK0_0_MCIO_DDRIO_REG), txGroup1.Data);
            }

            //
            // Wait for the new value to settle
            //
            FixedDelay(host, usDelay);
          }
        }
      }
    } // if read only

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
      if (level == DdrLevel) {
        (*channelNvList)[ch].txGroup0[logRank][strobe] = txGroup0.Data;
        if (group == TxDqDelay) {
          (*channelNvList)[ch].txGroup1[logRank][strobe] = txGroup1.Data;
        }
      } //level
    }
  } // strobe loop
  } //Grantley Implementation ends

#ifdef LRDIMM_SUPPORT
  if (mode & GSM_UPDATE_CACHE) {
    if (level == LrbufLevel) {
      switch (group) {
        case TxDqDelay:
          for (strobe = 0; strobe < MAX_STROBE; strobe++) {
            (*rankStruct)[rank].cachedLrBuf_FxBC8x9x[strobe] =  (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe];
          }
          break;
        case TxDqsDelay:
          for (strobe = 0; strobe < MAX_STROBE; strobe++) {
            (*rankStruct)[rank].cachedLrBuf_FxBCAxBx[strobe] =  (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCAxBx[strobe];
          }
          break;
        case TxDqsDelayCycle:
          for (strobe = 0; strobe < MAX_STROBE; strobe++) {
            (*rankStruct)[rank].cachedLrBuf_FxBCDxFx[strobe] =  (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[strobe];
          }
          break;
        default:
          break;
      } // switch
    } //level
  }

  if ((level == LrbufLevel) && !(mode & GSM_READ_ONLY) && ((updateNeeded) || (mode & GSM_FORCE_WRITE))) {
    switch (group) {
      case TxDqDelay:
        controlWordDataPtr = &((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[0]);
        WriteLrbufPBA(host, socket, ch, dimm, rank, controlWordDataPtr, rank, LRDIMM_BC8x, ALL_DATABUFFERS);
        controlWordDataPtr = &((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[9]);
        WriteLrbufPBA(host, socket, ch, dimm, rank, controlWordDataPtr, rank, LRDIMM_BC9x, ALL_DATABUFFERS);
        break;
      case TxDqsDelay:
        controlWordDataPtr = &((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCAxBx[0]);
        WriteLrbufPBA(host, socket, ch, dimm, rank, controlWordDataPtr, rank, LRDIMM_BCAx, ALL_DATABUFFERS);
        controlWordDataPtr = &((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCAxBx[9]);
        WriteLrbufPBA(host, socket, ch, dimm, rank, controlWordDataPtr, rank, LRDIMM_BCBx, ALL_DATABUFFERS);
        break;
      case TxDqsDelayCycle:
        controlWordDataPtr = &((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[0]);
        controlWordAddr = LRDIMM_BCDx + ((rank >> 1) * 0x20);
        WriteLrbufPBA(host, socket, ch, dimm, 0, controlWordDataPtr, (rank & BIT0) , controlWordAddr, ALL_DATABUFFERS);
        break;
      default:
      break;
    } // switch
  } //update
#endif  //LRDIMM

  return MRC_STATUS_SUCCESS;
} // GetSetTxDelay

/**

  Get Set TxDelayBit

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - Bit-field of different modes
  @param value   - Pointer to delay value or offset based on mode

  @retval IO delay

**/
MRC_STATUS
GetSetTxDelayBit (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  GSM_LT    level,
  GSM_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8                               logRank;
  UINT8                               maxStrobe;
  UINT8                               maxBit;
  INT16                                     curVal = 0;
  INT16                                     piDelay;
  UINT16                                    finalDelay;
  UINT32                                    xsel = 0;
  INT16                               tmp;
  UINT16                              maxLimit = 0;
  UINT16                              minLimit = 0;
  UINT16                              usDelay = 0;
  UINT16                                    TxPerBitSetup2to2Offset;
#ifdef SERIAL_DBG_MSG
  char                                strBuf0[128];
#endif  // SERIAL_DBG_MSG
  struct channelNvram                 (*channelNvList)[MAX_CH];
  TXGROUP1N0RANK0_0_MCIO_DDRIO_STRUCT txGroup1;
  TXGROUP0N0RANK0_0_MCIO_DDRIO_BDX_STRUCT   txGroup0;
  TXXTALKN0_0_MCIO_DDRIO_STRUCT             txXtalk;
  TXXTALKN0_0_MCIO_DDRIO_STRUCT             txXtalkCache;
  DATACONTROL0_0_MCIO_DDRIO_STRUCT          dataControl0;
  DATAOFFSETTRAIN_0_MCIO_DDRIO_STRUCT       dataOffsetTrain;
  channelNvList = GetChannelNvList(host, socket);
  txGroup0.Data = 0;
  txGroup1.Data = 0;
  txXtalk.Data = 0;
  dataControl0.Data = 0;
  TxPerBitSetup2to2Offset = TX_PER_BIT_SETUP_2TO2_OFFSET;


  //
  // Get the logical rank #
  //
  logRank = GetLogicalRank(host, socket, ch, dimm, rank);

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits(host, level, group, &minLimit, &maxLimit, &usDelay);

  if (strobe == ALL_STROBES) {
    strobe    = 0;
    maxStrobe = MAX_STROBE;
  } else {
    maxStrobe = strobe + 1;
  }

  if (bit == ALL_BITS) {
    bit     = 0;
    maxBit  = 4;
  } else {
    maxBit  = bit + 1;
  }

  for (strobe; strobe < maxStrobe; strobe++) {
    if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
      dataOffsetTrain.Data = (*channelNvList)[ch].dataOffsetTrain[strobe];

    if(mode & GSM_READ_CSR) {
      //
      // Read from PCI config space
      //
      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        txGroup0.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, TXGROUP0N0RANK0_0_MCIO_DDRIO_REG));
        txXtalk.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister2 (host, strobe, TXXTALKN0_0_MCIO_DDRIO_REG));
        tmp = strobe % 9;
        dataControl0.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, (UINT8)tmp, DATACONTROL0_0_MCIO_DDRIO_REG));
      }
      txGroup1.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, TXGROUP1N0RANK0_0_MCIO_DDRIO_REG));

    } else {
      //
      // Read from cache
      //
      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        txGroup0.Data = (*channelNvList)[ch].txGroup0[logRank][strobe];
        txXtalk.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister2 (host, strobe, TXXTALKN0_0_MCIO_DDRIO_REG)); // Due to the different register architecture of TxXtalk, we must always read it from the register and then replace the rank field we are using with data from the cache.
        txXtalkCache.Data = (*channelNvList)[ch].txXtalk[strobe];
        switch (logRank) {
          case 0:
            txXtalk.Bits.twototwologicdelayrank0 = txXtalkCache.Bits.twototwologicdelayrank0;
            break;
          case 1:
            txXtalk.Bits.twototwologicdelayrank1 = txXtalkCache.Bits.twototwologicdelayrank1;
            break;
          case 2:
            txXtalk.Bits.twototwologicdelayrank2 = txXtalkCache.Bits.twototwologicdelayrank2;
            break;
          case 3:
            txXtalk.Bits.twototwologicdelayrank3 = txXtalkCache.Bits.twototwologicdelayrank3;
            break;
          case 4:
            txXtalk.Bits.twototwologicdelayrank4 = txXtalkCache.Bits.twototwologicdelayrank4;
            break;
          case 5:
            txXtalk.Bits.twototwologicdelayrank5 = txXtalkCache.Bits.twototwologicdelayrank5;
            break;
          case 6:
            txXtalk.Bits.twototwologicdelayrank6 = txXtalkCache.Bits.twototwologicdelayrank6;
            break;
          case 7:
            txXtalk.Bits.twototwologicdelayrank7 = txXtalkCache.Bits.twototwologicdelayrank7;
            break;
        }
        dataControl0.Data = (*channelNvList)[ch].dataControl0;
      }
      txGroup1.Data = (*channelNvList)[ch].txGroup1[logRank][strobe];
    }

    for (bit; bit < maxBit; bit++) {
      switch (bit) {
        case 0:
          curVal = (UINT16)txGroup1.Bits.txdqpicode0;
          xsel = txGroup0.Bits.txdqxsel0;
          break;

        case 1:
          curVal = (UINT16)txGroup1.Bits.txdqpicode1;
          xsel = txGroup0.Bits.txdqxsel1;
          break;

        case 2:
          curVal = (UINT16)txGroup1.Bits.txdqpicode2;
          xsel = txGroup0.Bits.txdqxsel2;
          break;

        case 3:
          curVal = (UINT16)txGroup1.Bits.txdqpicode3;
          xsel = txGroup0.Bits.txdqxsel3;
          break;
      } // switch

      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        curVal = (curVal + 128 - (UINT16)dataOffsetTrain.Bits.txdqoffset - TxPerBitSetup2to2Offset) % 64;
        curVal += GetTwoTwoLogicDelay(host, &txXtalk, logRank, bit, 1) * 64; // Account for logic delay
      }

      //
      // Check if read only
      //
      if (mode & GSM_READ_ONLY) {
        //
        // Save the value read into the output parameter
        //
        *value = curVal;
      } else {
        //
        // Write
        //
        // Adjust the current CMD delay value by offset
        if (mode & GSM_WRITE_OFFSET) {
          //
          // Add the offset to the current value
          //
          tmp = curVal + *value;
          (*channelNvList)[ch].txPerBitDeskew[logRank][strobe][bit] = (UINT8)*value;

        } else {
          //
          // Write absolute value
          //
          tmp = *value;
        }

        //
        // Make sure we do not exeed the limits
        //
        if (tmp >= minLimit) {
          piDelay = tmp;
        } else {
          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, bit,
                        "%s Out of range!! tmp = 0x%x, minLimit = 0x%x\n", GetGroupStr(group, (char *)&strBuf0), tmp, minLimit));
          piDelay = minLimit;
        }
        // Ensure we do not exceed maximums
        if (piDelay > maxLimit) {
          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, bit,
                        "%s Out of range!! piDelay = 0x%x, maxLimit = 0x%x\n", GetGroupStr(group, (char *)&strBuf0), piDelay, maxLimit));
          piDelay = maxLimit;
        }
        if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
          finalDelay = (piDelay + (UINT16)dataOffsetTrain.Bits.txdqoffset + (UINT16)TxPerBitSetup2to2Offset) % 64;
          SetTwoTwoLogicDelay(host, &txXtalk, logRank, bit, piDelay / 64, 1);
          if(GetTwoTwoLogicDelaysAllSet(host, &txXtalk, logRank, 1)) { // Check to see if all per-bit TTLogicDelay values are 1
            if((txGroup0.Bits.txdqdelay >> 0x6) < 7 ) { // We can't go beyond a per-nibble delay of 7
               // Add 1 to the nibble logic delay and set the TTlogicDelay for other bits back to 0.
               txGroup0.Bits.txdqdelay += (1 << 0x6);
               SetTwoTwoLogicDelay(host, &txXtalk, logRank, 0, 0x0, 1);
               SetTwoTwoLogicDelay(host, &txXtalk, logRank, 1, 0x0, 1);
               SetTwoTwoLogicDelay(host, &txXtalk, logRank, 2, 0x0, 1);
               SetTwoTwoLogicDelay(host, &txXtalk, logRank, 3, 0x0, 1);
            }
          }

          if ((piDelay % 64) > 31) {
            xsel = 1;
          } else {
            xsel = 0;
          }
        } else {
          finalDelay = piDelay;
        }

        switch (bit) {
          case 0:
            txGroup1.Bits.txdqpicode0 = finalDelay;
            txGroup0.Bits.txdqxsel0 = xsel;

            break;

          case 1:
            txGroup1.Bits.txdqpicode1 = finalDelay;
            txGroup0.Bits.txdqxsel1 = xsel;
            break;

          case 2:
            txGroup1.Bits.txdqpicode2 = finalDelay;
            txGroup0.Bits.txdqxsel2 = xsel;
            break;

          case 3:
            txGroup1.Bits.txdqpicode3 = finalDelay;
            txGroup0.Bits.txdqxsel3 = xsel;
            break;
        } // switch
        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        if ((txGroup1.Data != (*channelNvList)[ch].txGroup1[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, TXGROUP1N0RANK0_0_MCIO_DDRIO_REG), txGroup1.Data);
          //
          // Wait for the new value to settle
          //
          FixedDelay(host, usDelay);
        }
      } // if read only
    } // bit loop

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if((mode & GSM_READ_ONLY) == 0) {
      if (((txGroup1.Data != (*channelNvList)[ch].txGroup1[logRank][strobe]) || (mode & GSM_FORCE_WRITE))) {
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, TXGROUP1N0RANK0_0_MCIO_DDRIO_REG), txGroup1.Data);
        FixedDelay(host, usDelay);
      }
      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        if ((txGroup0.Data != (*channelNvList)[ch].txGroup0[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, TXGROUP0N0RANK0_0_MCIO_DDRIO_REG), txGroup0.Data);
          FixedDelay(host, usDelay);
        }
        if ((txXtalk.Data != (*channelNvList)[ch].txXtalk[strobe]) || (mode & GSM_FORCE_WRITE)) {
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister2 (host, strobe, TXXTALKN0_0_MCIO_DDRIO_REG), txXtalk.Data);
          dataControl0.Bits.readrfwr = 1;
          dataControl0.Bits.readrfrank = logRank;
          FixedDelay(host, usDelay);
        }
        if ((dataControl0.Data != (*channelNvList)[ch].dataControl0) || (mode & GSM_FORCE_WRITE)) {
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
          dataControl0.Bits.readrfwr = 0; // Make sure the cache value is correct
          FixedDelay(host, usDelay);
        }
      }
    }
    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
      (*channelNvList)[ch].txGroup1[logRank][strobe] = txGroup1.Data;
      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        (*channelNvList)[ch].txGroup0[logRank][strobe] = txGroup0.Data;
        (*channelNvList)[ch].txXtalk[strobe] = txXtalk.Data;
        (*channelNvList)[ch].dataControl0 = dataControl0.Data;
      }
    }
  } // strobe loop

  return MRC_STATUS_SUCCESS;
} // GetSetTxDelayBit

/**

    Get 2TO2 Logic Delays All Set

    @param host    - Pointer to sysHost
    @param socket  - Socket number
    @param rank    - Current rank
    @param debug   - 0

    @retval 1
    @retval 0

**/
UINT8
GetTwoTwoLogicDelaysAllSet(
  PSYSHOST                            host,
  TXXTALKN0_0_MCIO_DDRIO_STRUCT*      txXtalk,
  UINT8                               rank,
  UINT8                               debug
  )
{
    UINT32 bits = 0x0;

  switch(rank) {
    case 0:
      bits = txXtalk->Bits.twototwologicdelayrank0;
      break;
    case 1:
      bits = txXtalk->Bits.twototwologicdelayrank1;
      break;
    case 2:
      bits = txXtalk->Bits.twototwologicdelayrank2;
      break;
    case 3:
      bits = txXtalk->Bits.twototwologicdelayrank3;
      break;
    case 4:
      bits = txXtalk->Bits.twototwologicdelayrank4;
      break;
    case 5:
      bits = txXtalk->Bits.twototwologicdelayrank5;
      break;
    case 6:
      bits = txXtalk->Bits.twototwologicdelayrank6;
      break;
    case 7:
      bits = txXtalk->Bits.twototwologicdelayrank7;
      break;
  }

  if (bits == 0xF) {
    return 1;
  } else {
    return 0;
  }
}

/**

    Get 2TO2 logic Delay

    @param host    - Pointer to sysHost
    @param socket  - Socket number
    @param rank    - Current rank
    @param bit     - Bit to check
    @param debug   - 0

    @retval 2to2 Logic Delay Rank
    @retval 0

**/
UINT16
GetTwoTwoLogicDelay(
  PSYSHOST                            host,
  TXXTALKN0_0_MCIO_DDRIO_STRUCT*      txXtalk,
  UINT8                               rank,
  UINT8                               bit,
  UINT8                               debug
  )
{
    UINT8 bitmask = 0x0;

  switch(bit) {
    case 0:
      bitmask = 0x1;
      break;
    case 1:
      bitmask = 0x2;
      break;
    case 2:
      bitmask = 0x4;
      break;
    case 3:
      bitmask = 0x8;
      break;
  }
  switch (rank) {
    case 0:
      return (txXtalk->Bits.twototwologicdelayrank0 & bitmask) >> bit;
    case 1:
      return (txXtalk->Bits.twototwologicdelayrank1 & bitmask) >> bit;
    case 2:
      return (txXtalk->Bits.twototwologicdelayrank2 & bitmask) >> bit;
    case 3:
      return (txXtalk->Bits.twototwologicdelayrank3 & bitmask) >> bit;
    case 4:
      return (txXtalk->Bits.twototwologicdelayrank4 & bitmask) >> bit;
    case 5:
      return (txXtalk->Bits.twototwologicdelayrank5 & bitmask) >> bit;
    case 6:
      return (txXtalk->Bits.twototwologicdelayrank6 & bitmask) >> bit;
    case 7:
      return (txXtalk->Bits.twototwologicdelayrank7 & bitmask) >> bit;
  }

  return 0;
}

/**

    Set 2TO2 Logic Delay

    @param host    - Pointer to sysHost
    @param socket  - Socket number
    @param rank    - Current rank
    @param bit     - Bit to set
    @param value   - Value to set
    @param debug   - 0

    @retval N/A

**/
void
SetTwoTwoLogicDelay(
  PSYSHOST                            host,
  TXXTALKN0_0_MCIO_DDRIO_STRUCT*      txXtalk,
  UINT8                               rank,
  UINT8                               bit,
  UINT16                              value,
  UINT8                               debug
  )
{
  UINT8 bitmask = 0xF;

  switch (bit) {
    case 0:
      bitmask = 0xE; // 1110
      break;
    case 1:
      bitmask = 0xD; // 1101
      break;
    case 2:
      bitmask = 0xB; // 1011
      break;
    case 3:
      bitmask = 0x7; // 0111
      break;
  }

  switch (rank) {
    case 0:
      txXtalk->Bits.twototwologicdelayrank0 = (txXtalk->Bits.twototwologicdelayrank0 & bitmask) | (value << bit);
      break;
    case 1:
      txXtalk->Bits.twototwologicdelayrank1 = (txXtalk->Bits.twototwologicdelayrank1 & bitmask) | (value << bit);
      break;
    case 2:
      txXtalk->Bits.twototwologicdelayrank2 = (txXtalk->Bits.twototwologicdelayrank2 & bitmask) | (value << bit);
      break;
    case 3:
      txXtalk->Bits.twototwologicdelayrank3 = (txXtalk->Bits.twototwologicdelayrank3 & bitmask) | (value << bit);
      break;
    case 4:
      txXtalk->Bits.twototwologicdelayrank4 = (txXtalk->Bits.twototwologicdelayrank4 & bitmask) | (value << bit);
      break;
    case 5:
      txXtalk->Bits.twototwologicdelayrank5 = (txXtalk->Bits.twototwologicdelayrank5 & bitmask) | (value << bit);
      break;
    case 6:
      txXtalk->Bits.twototwologicdelayrank6 = (txXtalk->Bits.twototwologicdelayrank6 & bitmask) | (value << bit);
      break;
    case 7:
      txXtalk->Bits.twototwologicdelayrank7 = (txXtalk->Bits.twototwologicdelayrank7 & bitmask) | (value << bit);
      break;
  }
}

/**

    Update Register IO 2

    @param host      - Pointer to SysHost
    @param strobe    - Strobe number (0-based)
    @param regOffset - Register offset

    @retval csrReg.Data

**/
UINT32
UpdateIoRegister2 (
  PSYSHOST    host,
  UINT8       strobe,
  UINT32      regOffset
  )
{
  CSR_OFFSET  csrReg;

  csrReg.Data = regOffset;

  // Bytes 7 and 8 are in the DDRIO_EXT function
  if ((strobe == 7) || (strobe == 8) || (strobe == 16) || (strobe == 17)) {
    csrReg.Bits.funcblk += 1;
  }

  //
  // Add in offset for data group
  //
  csrReg.Bits.offset += ddrioOffsetTable[strobe];

  return csrReg.Data;
} // UpdateIoRegister

/**

  Get RxDelay

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetRxDelay (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  GSM_LT    level,
  GSM_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8                               logRank;
  UINT8                               maxStrobe;
  UINT16                              curVal = 0;
  UINT16                              curVal2 = 0;
  UINT16                              piDelay;
  UINT16                              piDelay2 = 0;
  INT16                               tmp;
  INT16                               tmp2 = 0;
  UINT16                              maxLimit = 0;
  UINT16                              minLimit = 0;
  UINT16                              usDelay = 0;
#ifdef SERIAL_DBG_MSG
  char                                strBuf0[128];
#endif  // SERIAL_DBG_MSG
  struct channelNvram                 (*channelNvList)[MAX_CH];
  RXGROUP0N0RANK0_0_MCIO_DDRIO_STRUCT rxGroup0;
#ifdef LRDIMM_SUPPORT
  UINT8                               tmpStrobe;
  UINT8                               *controlWordDataPtr;
  UINT8                               controlWordAddr;
  struct ddrRank                      (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                   (*rankStruct)[MAX_RANK_DIMM];
  UINT8                               updateNeeded = 0;
#endif  //LRDIMM

  channelNvList = GetChannelNvList(host, socket);
#ifdef LRDIMM_SUPPORT
  rankStruct  = &host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct;
  rankList = &host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].rankList;
#endif  //LRDIMM

  rxGroup0.Data = 0;

  //
  // Get the logical rank #
  //
  logRank = GetLogicalRank(host, socket, ch, dimm, rank);

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits(host, level, group, &minLimit, &maxLimit, &usDelay);

  if (strobe == ALL_STROBES) {
    strobe    = 0;
    maxStrobe = MAX_STROBE;
  } else {
    maxStrobe = strobe + 1;
  }

  for (strobe; strobe < maxStrobe; strobe++) {
    if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
#ifdef LRDIMM_SUPPORT
    if (level == LrbufLevel) {

      if(mode & GSM_READ_CSR) {
        //
        // Read from actual setting cache
        //
        switch (group) {
          case RxDqsDelay:
            if ((*rankList)[rank].lrBuf_FxBC4x5x[strobe] < 16) {
              curVal = (*rankList)[rank].lrBuf_FxBC4x5x[strobe] + 15;
            } else {
              curVal = (*rankList)[rank].lrBuf_FxBC4x5x[strobe] ^ 31;
            }
            break;
          case RecEnDelay:
              curVal = (*rankList)[rank].lrBuf_FxBC2x3x[strobe];
            break;
          case RecEnDelayCycle:
              curVal = (((*rankList)[rank].lrBuf_FxBCCxEx[strobe % 9] >> (4 * (strobe / 9))) & 0xF);
            break;
          default:
            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                           "GetSetRxDelay called unsupported group!\n"));
            RC_ASSERT(FALSE, ERR_RC_INTERNAL, 0);
            break;
        } //switch group
      } else {
        //
        // Read from setting cache
        //
        switch (group) {
          case RxDqsDelay:
            if ((*rankStruct)[rank].cachedLrBuf_FxBC4x5x[strobe] < 16) {
              curVal = (*rankStruct)[rank].cachedLrBuf_FxBC4x5x[strobe] + 15;
            } else {
              curVal = (*rankStruct)[rank].cachedLrBuf_FxBC4x5x[strobe] ^ 31;
            }
            break;
          case RecEnDelay:
            curVal = (*rankStruct)[rank].cachedLrBuf_FxBC2x3x[strobe];
            break;
          case RecEnDelayCycle:
            curVal = (((*rankStruct)[rank].cachedLrBuf_FxBCCxEx[strobe % 9]>>(4 * (strobe / 9))) & 0xF);
            break;
          default:
            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                           "GetSetRxDelay called unsupported group!\n"));
            RC_ASSERT(FALSE, ERR_RC_INTERNAL, 0);
            break;
        } //switch group
      }
    } else
#endif //LRDIMM
    {
      if(mode & GSM_READ_CSR) {
        //
        // Read from PCI config space
        //
        rxGroup0.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, RXGROUP0N0RANK0_0_MCIO_DDRIO_REG));

      } else {
        //
        // Read from cache
        //
        rxGroup0.Data = (*channelNvList)[ch].rxGroup0[logRank][strobe];
      }

      switch (group) {
        case RecEnDelay:
          curVal = (UINT16)rxGroup0.Bits.rcvendelay;
          break;

        case RxDqsPDelay:
          curVal = (UINT16)rxGroup0.Bits.rxdqspdelay;
          break;

        case RxDqsNDelay:
          curVal = (UINT16)rxGroup0.Bits.rxdqsndelay;
          break;

        case RxDqsDelay:
          curVal = (UINT16)rxGroup0.Bits.rxdqspdelay;
          curVal2 = (UINT16)rxGroup0.Bits.rxdqsndelay;
          break;
        case RxDqDelay:
          curVal = (UINT16)rxGroup0.Bits.rxdqdelay;
          break;

        case RxEq:
          curVal = (UINT16)rxGroup0.Bits.rxeq;
          break;
      } // switch
    }

    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *value = curVal;
    } else {
      //
      // Write
      //
      // Adjust the current CMD delay value by offset
      if (mode & GSM_WRITE_OFFSET) {
        //
        // Add the offset to the current value
        //
        tmp = curVal + *value;
        if(group == RxDqsDelay) {
          tmp2 = curVal2 + *value;
        }

      } else {
        //
        // Write absolute value
        //
        tmp = *value;
        if(group == RxDqsDelay) {
          tmp2 = *value;
        }
      }

      //
      // Make sure we do not exeed the limits
      //
      if (tmp >= minLimit) {
        piDelay = tmp;
      } else {
        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                      "%s Out of range!! tmp = 0x%x, minLimit = 0x%x\n", GetGroupStr(group, (char *)&strBuf0), tmp, minLimit));
        piDelay = minLimit;
      }
      // Ensure we do not exceed maximums
      if (piDelay > maxLimit) {
        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                      "%s Out of range!! piDelay = 0x%x, maxLimit = 0x%x\n", GetGroupStr(group, (char *)&strBuf0), piDelay, maxLimit));
        piDelay = maxLimit;
      }

      if ((group == RxDqsDelay) && (level == DdrLevel)) {
        if (tmp2 >= minLimit) {
          piDelay2 = tmp2;
        } else {
          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                        "%s Out of range!! tmp = 0x%x, minLimit = 0x%x\n", GetGroupStr(group, (char *)&strBuf0), tmp2, minLimit));
          piDelay2 = minLimit;
        }
        // Ensure we do not exceed maximums
        if (piDelay2 > maxLimit) {
          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                        "%s Out of range!! piDelay = 0x%x, maxLimit = 0x%x\n", GetGroupStr(group, (char *)&strBuf0), piDelay2, maxLimit));
          piDelay2 = maxLimit;
        }
      }

      if (level == DdrLevel) {
        switch (group) {
          case RecEnDelay:
            rxGroup0.Bits.rcvendelay = piDelay;
            if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
              if ((piDelay % 64) > 31) {
                rxGroup0.Bits.rcvenxsel = 1;
              } else {
                rxGroup0.Bits.rcvenxsel = 0;
              }
            }
            break;

          case RxDqsPDelay:
            rxGroup0.Bits.rxdqspdelay = piDelay;
            break;

          case RxDqsNDelay:
            rxGroup0.Bits.rxdqsndelay = piDelay;
            break;

          case RxDqsDelay:
            rxGroup0.Bits.rxdqspdelay = piDelay;
            rxGroup0.Bits.rxdqsndelay = piDelay2;
            break;

          case RxDqDelay:
            rxGroup0.Bits.rxdqdelay = piDelay;
            break;

          case RxEq:
            rxGroup0.Bits.rxeq = piDelay;
        } // switch
      }

#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        //
        // first translate encoded value to register value
        //
        //curVal = piDelay >> 1;
        curVal = piDelay;

        switch (group) {
          case RxDqsDelay:
            if (curVal < 16) {
              if ((*rankList)[rank].lrBuf_FxBC4x5x[strobe] != (UINT8) (curVal ^ 31)) {
                updateNeeded = 1;
                (*rankList)[rank].lrBuf_FxBC4x5x[strobe] = (UINT8) (curVal ^ 31);
              }
            } else {
              if ((*rankList)[rank].lrBuf_FxBC4x5x[strobe] != (UINT8) (curVal - 15)) {
                updateNeeded = 1;
                (*rankList)[rank].lrBuf_FxBC4x5x[strobe] = (UINT8) (curVal - 15);
              }
            }
            break;
          case RecEnDelay:
            if ((*rankList)[rank].lrBuf_FxBC2x3x[strobe] != (UINT8) curVal) {
              updateNeeded = 1;
              (*rankList)[rank].lrBuf_FxBC2x3x[strobe] = (UINT8) curVal;
            }
            break;
          case RecEnDelayCycle:
            if ((((*rankList)[rank].lrBuf_FxBCCxEx[strobe % 9]>>(4 * (strobe / 9))) & 0xF) != (UINT8) curVal) {
              updateNeeded = 1;
              (*rankList)[rank].lrBuf_FxBCCxEx[strobe % 9] &= 0xF0 >> (4 * (strobe / 9));
              (*rankList)[rank].lrBuf_FxBCCxEx[strobe % 9] |= (UINT8) curVal<<(4 * (strobe / 9));
            }
            break;
          default:
            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                           "GetSetRxDelay called unsupported group!\n"));
            RC_ASSERT(FALSE, ERR_RC_INTERNAL, 0);
            break;
        } //switch group
      } else
#endif  //LRDIMM
      {
        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        if ((rxGroup0.Data != (*channelNvList)[ch].rxGroup0[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, RXGROUP0N0RANK0_0_MCIO_DDRIO_REG), rxGroup0.Data);

          //
          // Wait for the new value to settle
          //
          FixedDelay(host, usDelay);
        }
      }
    } // if read only

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
      if (level == DdrLevel) {
        (*channelNvList)[ch].rxGroup0[logRank][strobe] = rxGroup0.Data;
      }
    }

  } // strobe loop

#ifdef LRDIMM_SUPPORT
  if ((level == LrbufLevel) && !(mode & GSM_READ_ONLY) && ((updateNeeded) || (mode & GSM_FORCE_WRITE))) {
    switch (group) {
      case RxDqsDelay:
        controlWordDataPtr = &((*rankList)[rank].lrBuf_FxBC4x5x[0]);
        WriteLrbufPBA(host, socket, ch, dimm, rank, controlWordDataPtr, rank, LRDIMM_BC4x, ALL_DATABUFFERS);
        controlWordDataPtr = &((*rankList)[rank].lrBuf_FxBC4x5x[9]);
        WriteLrbufPBA(host, socket, ch, dimm, rank, controlWordDataPtr, rank, LRDIMM_BC5x, ALL_DATABUFFERS);
        break;
      case RecEnDelay:
        controlWordDataPtr = &((*rankList)[rank].lrBuf_FxBC2x3x[0]);
        WriteLrbufPBA(host, socket, ch, dimm, rank, controlWordDataPtr, rank, LRDIMM_BC2x, ALL_DATABUFFERS);
        controlWordDataPtr = &((*rankList)[rank].lrBuf_FxBC2x3x[9]);
        WriteLrbufPBA(host, socket, ch, dimm, rank, controlWordDataPtr, rank, LRDIMM_BC3x, ALL_DATABUFFERS);
        break;
      case RecEnDelayCycle:
        controlWordDataPtr = &((*rankList)[rank].lrBuf_FxBCCxEx[0]);
        controlWordAddr = LRDIMM_BCCx + ((rank >> 1) * 0x20);
        WriteLrbufPBA(host, socket, ch, dimm, 0, controlWordDataPtr, (rank & BIT0) , controlWordAddr, ALL_DATABUFFERS);
        break;
      default:
        break;
    } // end switch
    //
    // Wait for the new value to settle
    //
    FixedDelay(host, usDelay);
  }

  if (mode & GSM_UPDATE_CACHE) {
    if (level == LrbufLevel) {
      switch (group) {
        case RxDqsDelay:
          for (tmpStrobe = 0; tmpStrobe < MAX_STROBE; tmpStrobe++) {
            (*rankStruct)[rank].cachedLrBuf_FxBC4x5x[tmpStrobe] =  (*rankList)[rank].lrBuf_FxBC4x5x[tmpStrobe];
          }
          break;
        case RecEnDelay:
          for (tmpStrobe = 0; tmpStrobe < MAX_STROBE; tmpStrobe++) {
            (*rankStruct)[rank].cachedLrBuf_FxBC2x3x[tmpStrobe] =  (*rankList)[rank].lrBuf_FxBC2x3x[tmpStrobe];
          }
          break;
        case RecEnDelayCycle:
          for (tmpStrobe = 0; tmpStrobe < MAX_STROBE/2; tmpStrobe++) {
            (*rankStruct)[rank].cachedLrBuf_FxBCCxEx[tmpStrobe] =  (*rankList)[rank].lrBuf_FxBCCxEx[tmpStrobe];
          }
        break;
      }
    }
  }
#endif //LRDIMM

  return MRC_STATUS_SUCCESS;
} // GetSetRxDelay

/**

  Get RxDelay

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetRxDelayBit (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  GSM_LT    level,
  GSM_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8                               logRank;
  UINT8                               maxStrobe;
  UINT8                               maxBit;
  UINT16                              curVal = 0;
  UINT16                              piDelay;
  INT16                               tmp;
  UINT16                              maxLimit = 0;
  UINT16                              minLimit = 0;
  UINT16                              usDelay = 0;
#ifdef SERIAL_DBG_MSG
  char                                strBuf0[128];
#endif  // SERIAL_DBG_MSG
  struct channelNvram                 (*channelNvList)[MAX_CH];
  RXGROUP1N0RANK0_0_MCIO_DDRIO_STRUCT rxGroup1;

  channelNvList = GetChannelNvList(host, socket);

  rxGroup1.Data = 0;

  //
  // Get the logical rank #
  //
  logRank = GetLogicalRank(host, socket, ch, dimm, rank);

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits(host, level, group, &minLimit, &maxLimit, &usDelay);

  if (strobe == ALL_STROBES) {
    strobe    = 0;
    maxStrobe = MAX_STROBE;
  } else {
    maxStrobe = strobe + 1;
  }

  if (bit == ALL_BITS) {
    bit     = 0;
    maxBit  = 4;
  } else {
    maxBit  = bit + 1;
  }

  for (strobe; strobe < maxStrobe; strobe++) {
    if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

    if(mode & GSM_READ_CSR) {
      //
      // Read from PCI config space
      //
      rxGroup1.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, RXGROUP1N0RANK0_0_MCIO_DDRIO_REG));

    } else {
      //
      // Read from cache
      //
      rxGroup1.Data = (*channelNvList)[ch].rxGroup1[logRank][strobe];
    }

    for (bit; bit < maxBit; bit++) {
      switch (bit) {
        case 0:
          if (group == RxDqsPBitDelay){
          curVal = (UINT16)rxGroup1.Bits.rxdqspdelay0;
          }
          else{
            curVal = (UINT16)rxGroup1.Bits.rxdqsndelay0;
          }
          break;

        case 1:
          if (group == RxDqsPBitDelay){
          curVal = (UINT16)rxGroup1.Bits.rxdqspdelay1;
          }
          else{
            curVal = (UINT16)rxGroup1.Bits.rxdqsndelay1;
          }
          break;

        case 2:
          if (group == RxDqsPBitDelay){
          curVal = (UINT16)rxGroup1.Bits.rxdqspdelay2;
          }
          else{
            curVal = (UINT16)rxGroup1.Bits.rxdqsndelay2;
          }
          break;

        case 3:
          if (group == RxDqsPBitDelay){
          curVal = (UINT16)rxGroup1.Bits.rxdqspdelay3;
          }
          else{
            curVal = (UINT16)rxGroup1.Bits.rxdqsndelay3;
          }
          break;

      } // switch

      //
      // Check if read only
      //
      if (mode & GSM_READ_ONLY) {
        //
        // Save the value read into the output parameter
        //
        *value = curVal;
      } else {
        //
        // Write
        //
        // Adjust the current CMD delay value by offset
        if (mode & GSM_WRITE_OFFSET) {
          //
          // Add the offset to the current value
          //
          tmp = curVal + *value;

        } else {
          //
          // Write absolute value
          //
          tmp = *value;
        }

        //
        // Make sure we do not exeed the limits
        //
        if (tmp >= minLimit) {
          piDelay = tmp;
        } else {
          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, bit,
                        "%s Out of range!! tmp = 0x%x, minLimit = 0x%x\n", GetGroupStr(group, (char *)&strBuf0), tmp, minLimit));
          piDelay = minLimit;
        }
        // Ensure we do not exceed maximums
        if (piDelay > maxLimit) {
          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, bit,
                        "%s Out of range!! piDelay = 0x%x, maxLimit = 0x%x\n", GetGroupStr(group, (char *)&strBuf0), piDelay, maxLimit));
          piDelay = maxLimit;
        }

        switch (bit) {
          case 0:
            if (group == RxDqsPBitDelay){
              rxGroup1.Bits.rxdqspdelay0 = piDelay;
            }
            else{
              rxGroup1.Bits.rxdqsndelay0 = piDelay;
            }
            break;

          case 1:
            if (group == RxDqsPBitDelay){
              rxGroup1.Bits.rxdqspdelay1 = piDelay;
            }
            else{
              rxGroup1.Bits.rxdqsndelay1 = piDelay;
            }
            break;

          case 2:
            if (group == RxDqsPBitDelay){
              rxGroup1.Bits.rxdqspdelay2 = piDelay;
            }
            else{
              rxGroup1.Bits.rxdqsndelay2 = piDelay;
            }
            break;

          case 3:
            if (group == RxDqsPBitDelay){
              rxGroup1.Bits.rxdqspdelay3 = piDelay;
            }
            else{
              rxGroup1.Bits.rxdqsndelay3 = piDelay;
            }
            break;
        } // switch

        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        if ((rxGroup1.Data != (*channelNvList)[ch].rxGroup1[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, RXGROUP1N0RANK0_0_MCIO_DDRIO_REG), rxGroup1.Data);

          //
          // Wait for the new value to settle
          //
          FixedDelay(host, usDelay);
        }
      } // if read only

      //
      // Save to cache
      //
      if (mode & GSM_UPDATE_CACHE) {
        (*channelNvList)[ch].rxGroup1[logRank][strobe] = rxGroup1.Data;
      }

    } // bit loop
  } // strobe loop

  return MRC_STATUS_SUCCESS;
} // GetSetRxDelayBit

/**

  Get RxDelay

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetRxVref (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  GSM_LT    level,
  GSM_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8                             maxStrobe;
  UINT16                            curVal = 0;
  UINT16                            piDelay;
  INT16                             tmp;
  UINT16                            maxLimit = 127;
  UINT16                            minLimit = 0;
  UINT16                            usDelay = 1;
  struct channelNvram               (*channelNvList)[MAX_CH];
  RXVREFCTRLN0_0_MCIO_DDRIO_STRUCT  rxVrefCtrl;
#ifdef LRDIMM_SUPPORT
  UINT8                             *controlWordDataPtr;
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  UINT8                             updateNeeded = 0;
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
#endif //LRDIMM

  channelNvList = GetChannelNvList(host, socket);
#ifdef LRDIMM_SUPPORT
  rankStruct  = &host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct;
  rankList = GetRankNvList(host, socket, ch, dimm);
#endif //LRDIMM

  rxVrefCtrl.Data = 0;

  //
  // Get the logical rank #
  //

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits(host, level, group, &minLimit, &maxLimit, &usDelay);

  if (strobe == ALL_STROBES) {
    strobe    = 0;
    maxStrobe = MAX_STROBE;
  } else {
    maxStrobe = strobe + 1;
  }

  for (strobe; strobe < maxStrobe; strobe++) {
    if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

    if(mode & GSM_READ_CSR) {
#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        curVal = (*rankList)[rank].lrbufRxVref[strobe];
      } else
#endif //LRDIMM
      {
        //
        // Read from PCI config space
        //
        rxVrefCtrl.Data = MemReadPciCfgEp (host, socket, ch, rxVrefCtrlTable[strobe]);
      }
    } else {
      //
      // Read from cache
      //
#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        curVal = (*rankStruct)[rank].lrbufRxVrefCache[strobe];
      } else
#endif //LRDIMM
      {
        rxVrefCtrl.Data = (*channelNvList)[ch].rxVrefCtrl[strobe];
      }
    }

    if (level == DdrLevel) {
      if (bit == ALL_BITS) {
        curVal = (UINT16)rxVrefCtrl.Bits.vrefperbit0;
      } else {
        switch (bit) {
          case 0:
            curVal = (UINT16)rxVrefCtrl.Bits.vrefperbit0;
            break;

          case 1:
            curVal = (UINT16)rxVrefCtrl.Bits.vrefperbit1;
            break;

          case 2:
            curVal = (UINT16)rxVrefCtrl.Bits.vrefperbit2;
            break;

          case 3:
            curVal = (UINT16)rxVrefCtrl.Bits.vrefperbit3;
            break;
        } // switch (bit)
      }
    } // DdrLevel

    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *value = curVal;
    } else {
      //
      // Write
      //
      // Adjust the current CMD delay value by offset
      if (mode & GSM_WRITE_OFFSET) {
        //
        // Add the offset to the current value
        //
        tmp = curVal + *value;

        //
        // Make sure we do not exeed the limits
        //
        if (tmp > minLimit) {
          piDelay = tmp;
        } else {
          piDelay = minLimit;
        }
        // Ensure we do not exceed maximums
        if (piDelay > maxLimit) {
          piDelay = maxLimit;
        }
      } else {
        //
        // Write absolute value
        //
        piDelay = *value;
      }

      if (bit == ALL_BITS) {
        rxVrefCtrl.Bits.vrefperbit0 = piDelay;
        rxVrefCtrl.Bits.vrefperbit1 = piDelay;
        rxVrefCtrl.Bits.vrefperbit2 = piDelay;
        rxVrefCtrl.Bits.vrefperbit3 = piDelay;
      } else {
        switch (bit) {
          case 0:
            rxVrefCtrl.Bits.vrefperbit0 = piDelay;
            break;

          case 1:
            rxVrefCtrl.Bits.vrefperbit1 = piDelay;
            break;

          case 2:
            rxVrefCtrl.Bits.vrefperbit2 = piDelay;
            break;

          case 3:
            rxVrefCtrl.Bits.vrefperbit3 = piDelay;
            break;
        } // switch (bit)
      }

#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        if ((piDelay != (*rankList)[rank].lrbufRxVref[strobe]) || (mode & GSM_FORCE_WRITE)) {
          //(*channelNvList)[ch].lrbufRxVref[rank][strobe] = (UINT8) piDelay;
          (*rankList)[rank].lrbufRxVref[strobe] = (UINT8) piDelay;
          updateNeeded = 1;
        }
      } else
#endif //LRDIMM
      {
        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        if ((rxVrefCtrl.Data != (*channelNvList)[ch].rxVrefCtrl[strobe]) || (mode & GSM_FORCE_WRITE)) {
          MemWritePciCfgEp (host, socket, ch, rxVrefCtrlTable[strobe], rxVrefCtrl.Data);

          //
          // Wait for the new value to settle
          //
          FixedDelay(host, usDelay);
        }
      }
    } // if read only

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        (*rankStruct)[rank].lrbufRxVrefCache[strobe] = (*rankList)[rank].lrbufRxVref[strobe];
      } else
#endif //LRDIMM
      {
        (*channelNvList)[ch].rxVrefCtrl[strobe] = rxVrefCtrl.Data;
      }
    }
  } // strobe loop

#ifdef LRDIMM_SUPPORT
  if ((level == LrbufLevel) && !(mode & GSM_READ_ONLY) && ((updateNeeded) || (mode & GSM_FORCE_WRITE))) {
    //
    //Note: because Rx vref is shared between upper and lower nibbles on the backside,
    // we have to either pick 1 or do some average.  For now, just picking the lower nibble
    //
    controlWordDataPtr = &((*rankList)[rank].lrbufRxVref[0]);
    WriteLrbufPBA(host, socket, ch, dimm, rank, controlWordDataPtr, LRDIMM_F5, LRDIMM_BC6x, ALL_DATABUFFERS);

    //
    // Wait for the new value to settle
    //
    FixedDelay(host, usDelay);
  }
#endif //LRDIMM

  return MRC_STATUS_SUCCESS;
} // GetSetRxVref

/**

  Get Tx Vref

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetTxVref (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  GSM_LT    level,
  GSM_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT16                                            curVal = 0;
  UINT16                                            piDelay;
  INT16                                             tmp;
  UINT16                                            maxLimit = 127;
  UINT16                                            minLimit = 0;
  UINT16                                            usDelay = 1;
  struct channelNvram                               (*channelNvList)[MAX_CH];
  DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXTMCC_HSX_STRUCT ddrCrDimmVrefControl1Hsx;
  DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXTMCC_BDX_STRUCT ddrCrDimmVrefControl1Bdx;

  channelNvList = GetChannelNvList(host, socket);


  channelNvList = GetChannelNvList(host, socket);

  ddrCrDimmVrefControl1Hsx.Data = 0;

  //
  // Get minimum and maximum value
  GetDataGroupLimits(host, level, group, &minLimit, &maxLimit, &usDelay);

  if(mode & GSM_READ_CSR) {
    //
    // Read from PCI config space
    //
    ddrCrDimmVrefControl1Hsx.Data = MemReadPciCfgEp (host, socket, ch & BIT1, DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXTMCC_REG);

  } else {
    //
    // Read from cache
    //
    ddrCrDimmVrefControl1Hsx.Data = (*channelNvList)[ch & BIT1].dimmVrefControl1;
  }

  if (host->var.common.cpuType == CPU_BDX) {
    ddrCrDimmVrefControl1Bdx.Data = ddrCrDimmVrefControl1Hsx.Data;

    if ((ch == 0) || (ch == 2)) {
      curVal = (UINT16)(ddrCrDimmVrefControl1Bdx.Bits.ch0vrefctl >> 1);
    } else {
      curVal = (UINT16)(ddrCrDimmVrefControl1Bdx.Bits.ch1vrefctl >> 1);
    }
  } else {
    if ((ch == 0) || (ch == 2)) {
      curVal = (UINT16)(ddrCrDimmVrefControl1Hsx.Bits.ch0vrefctl >> 1);
    } else {
      curVal = (UINT16)(ddrCrDimmVrefControl1Hsx.Bits.ch1vrefctl >> 1);
    }
  }

  //
  // Check if read only
  //
  if (mode & GSM_READ_ONLY) {
    //
    // Save the value read into the output parameter
    //
    *value = curVal;
  } else {
    //
    // Write
    //
    // Adjust the current CMD delay value by offset
    if (mode & GSM_WRITE_OFFSET) {
      //
      // Add the offset to the current value
      //
      tmp = curVal + *value;

      //
      // Make sure we do not exeed the limits
      //
      if (tmp > minLimit) {
        piDelay = tmp;
      } else {
        piDelay = minLimit;
      }
      // Ensure we do not exceed maximums
      if (piDelay > maxLimit) {
        piDelay = maxLimit;
      }
    } else {
      //
      // Write absolute value
      //
      piDelay = *value;
    }

    if (host->var.common.cpuType == CPU_BDX) {
      if ((ch == 0) || (ch == 2)) {
        ddrCrDimmVrefControl1Bdx.Bits.ch0vrefctl = piDelay << 1;
      } else {
        ddrCrDimmVrefControl1Bdx.Bits.ch1vrefctl = piDelay << 1;
      }
      ddrCrDimmVrefControl1Hsx.Data = ddrCrDimmVrefControl1Bdx.Data;
    } else {
      if ((ch == 0) || (ch == 2)) {
        ddrCrDimmVrefControl1Hsx.Bits.ch0vrefctl = piDelay << 1;
      } else {
        ddrCrDimmVrefControl1Hsx.Bits.ch1vrefctl = piDelay << 1;
      }
    }

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if ((ddrCrDimmVrefControl1Hsx.Data != (*channelNvList)[ch & BIT1].dimmVrefControl1) || (mode & GSM_FORCE_WRITE)) {
      MemWritePciCfgEp (host, socket, ch & BIT1, DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXTMCC_REG, ddrCrDimmVrefControl1Hsx.Data);

      //
      // Wait for the new value to settle
      //
      FixedDelay(host, usDelay);
    }
  } // if read only

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    (*channelNvList)[ch & BIT1].dimmVrefControl1 = ddrCrDimmVrefControl1Hsx.Data;
  }

  return MRC_STATUS_SUCCESS;
} // GetSetTxVref

/**

  Get Tx Vref

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetTxVrefDDR4 (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  GSM_LT    level,
  GSM_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8               maxStrobe;
  UINT16              curVal = 0;
  INT16               newVref;
  INT16               tmp;
  UINT16              maxLimit = 0;
  UINT16              minLimit = 0;
  UINT16              usDelay = 0;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
#ifdef LRDIMM_SUPPORT
  struct rankDevice   (*rankStruct)[MAX_RANK_DIMM];
#endif //LRDIMM

  channelNvList = GetChannelNvList(host, socket);
  rankList = GetRankNvList(host, socket, ch, dimm);

#ifdef LRDIMM_SUPPORT
  rankStruct  = &host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct;
#endif //LRDIMM



  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits(host, level, group, &minLimit, &maxLimit, &usDelay);

  if ((host->setup.mem.optionsExt & PDA_EN) && (host->var.mem.txVrefDone) && (level == DdrLevel)) {

    if (strobe == ALL_STROBES) {
      strobe    = 0;
      maxStrobe = GetMaxStrobe(host, socket, ch, dimm, group, level);
    } else {
      maxStrobe = strobe + 1;
    }

    for (strobe; strobe < maxStrobe; strobe++) {
      if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

      if(mode & GSM_READ_CSR) {
#ifdef LRDIMM_SUPPORT
        if (level == LrbufLevel) {
          curVal = (UINT8)(*rankList)[rank].lrbufTxVref[strobe];
        } else
#endif  //LRDIMM
        {
          curVal = (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][strobe];
        }
      } else {
#ifdef LRDIMM_SUPPORT
        if (level == LrbufLevel) {
          curVal = (UINT8)(*rankStruct)[rank].lrbufTxVrefCache[strobe];
        } else
#endif  //LRDIMM
        {
          curVal = (*channelNvList)[ch].txVrefCache[(*rankList)[rank].rankIndex][strobe];
        }
      } // if cached

      //
      // Check if read only
      //
      if (mode & GSM_READ_ONLY) {
        //
        // Save the value read into the output parameter
        //
        *value = curVal;
      } else {
        //
        // Write
        //
        if (mode & GSM_WRITE_OFFSET) {
          //
          // Add the offset to the current value
          //
          tmp = curVal + *value;

        } else {
          //
          // Write absolute value
          //
          tmp = *value;
        }

        //
        // Make sure we do not exeed the limits
        //
        if (tmp > minLimit) {
          newVref = tmp;
        } else {
          newVref = minLimit;
        }
        // Ensure we do not exceed maximums
        if (newVref > maxLimit) {
          newVref = maxLimit;
        }

#ifdef LRDIMM_SUPPORT
        if (level == LrbufLevel) {
          //
          // Write it back if the current data does not equal the current value or if in force write mode
          //
          if ((newVref != (UINT8)(*rankList)[rank].lrbufTxVref[strobe]) || (mode & GSM_FORCE_WRITE)) {

            (*rankList)[rank].lrbufTxVref[strobe] = (UINT8)newVref;

            //
            // Write the new vref
            //
            MemWriteDimmVref(host, socket, ch, dimm, rank, strobe, level, (UINT8)newVref);

          }
        } else
#endif  //LRDIMM
        {
          //
          // Write it back if the current data does not equal the current value or if in force write mode
          //
          if ((newVref != (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][strobe]) || (mode & GSM_FORCE_WRITE)) {

            (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][strobe] = (UINT8)newVref;

            //
            // Write the new vref
            //
            MemWriteDimmVref(host, socket, ch, dimm, rank, strobe, level, (UINT8)newVref);

          }
        }
      } // if read only

      //
      // Save to cache
      //
      if (mode & GSM_UPDATE_CACHE) {
#ifdef LRDIMM_SUPPORT
        if (level == LrbufLevel) {
          (*rankStruct)[rank].lrbufTxVrefCache[strobe] = (*rankList)[rank].lrbufTxVref[strobe];
        } else
#endif //LRDIMM
        {
          (*channelNvList)[ch].txVrefCache[(*rankList)[rank].rankIndex][strobe] = (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][strobe];
        }
      }

      // Wait for Wr Vref circuit to settle
      FixedDelay(host, usDelay);
    } // strobe loop

    //
    // Make sure Vref settles
    //
    if (host->var.common.cpuType == CPU_BDX) {
      FixedDelay (host, 1);
    } else {
      FixedDelay (host, 100);
    }
  } else {
    if(mode & GSM_READ_CSR) {
#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        curVal = (UINT8)(*rankList)[rank].lrbufTxVref[0];
      } else
#endif //LRDIMM
      {
        curVal = (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][0];
      }
    } else {
#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        curVal = (UINT8)(*rankStruct)[rank].lrbufTxVrefCache[0];
      } else
#endif //LRDIMM
      {
        curVal = (*channelNvList)[ch].txVrefCache[(*rankList)[rank].rankIndex][0];
      }
    } // if cached

    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *value = curVal;
    } else {
      //
      // Write
      //
      if (mode & GSM_WRITE_OFFSET) {
        //
        // Add the offset to the current value
        //
        tmp = curVal + *value;

      } else {
        //
        // Write absolute value
        //
        tmp = *value;
      }

      //
      // Make sure we do not exeed the limits
      //
      if (tmp > minLimit) {
        newVref = tmp;
      } else {
        newVref = minLimit;
      }
      // Ensure we do not exceed maximums
      if (newVref > maxLimit) {
        newVref = maxLimit;
      }

#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        //
        // Write it back if the current data does not equal the current value or if in force write mode
        //
        if ((newVref != (UINT8)(*rankList)[rank].lrbufTxVref[0]) || (mode & GSM_FORCE_WRITE)) {

          for (strobe = 0; strobe < MAX_STROBE; strobe++) {
            (*rankList)[rank].lrbufTxVref[strobe] = (UINT8)newVref;
          } // strobe loop
          //
          // Write the new vref
          //
          MemWriteDimmVref(host, socket, ch, dimm, rank, 0, level, (UINT8)newVref); //If PDA_EN not true (ie broadcast mode, same value written to all strobes)
        }
      } else
#endif //LRDIMM
      {
        //
        // Write it back if the current data does not equal the current value or if in force write mode
        //
        if ((newVref != (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][0]) || (mode & GSM_FORCE_WRITE)) {

          for (strobe = 0; strobe < MAX_STROBE; strobe++) {
            (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][strobe] = (UINT8)newVref;
          } // strobe loop

          //
          // Write the new vref
          //
          MemWriteDimmVref(host, socket, ch, dimm, rank, 0, level, (UINT8)newVref); //If PDA_EN not true (ie broadcast mode, same value written to all strobes)

        }
      }
    } // if read only

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
      for (strobe = 0; strobe < MAX_STROBE; strobe++) {
#ifdef LRDIMM_SUPPORT
        if (level == LrbufLevel) {
          (*rankStruct)[rank].lrbufTxVrefCache[strobe] = (*rankList)[rank].lrbufTxVref[strobe];
        } else
#endif //LRDIMM
        {
          (*channelNvList)[ch].txVrefCache[(*rankList)[rank].rankIndex][strobe] = (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][strobe];
        }
      } // strobe loop
    }

    // Wait for Wr Vref circuit to settle
    FixedDelay(host, usDelay);
  }

  return MRC_STATUS_SUCCESS;
} // GetSetTxVrefDDR4

/**

  Get Imode

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetImode (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     strobe,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8                             maxStrobe;
  UINT16                            curVal = 0;
  UINT16                            piDelay;
  INT16                             tmp;
  UINT16                            maxLimit = 15;
  UINT16                            minLimit = 0;
  struct channelNvram               (*channelNvList)[MAX_CH];
  DATACONTROL2_0_MCIO_DDRIO_BDX_STRUCT  dataControl2;

  channelNvList = GetChannelNvList(host, socket);

  if ((strobe > 8) && (strobe != ALL_STROBES)) return SUCCESS;

  //
  // Get minimum and maximum value
  //
  //GetDataGroupLimits(host, level, group, &minLimit, &maxLimit, &usDelay);

  if (strobe == ALL_STROBES) {
    strobe    = 0;
    maxStrobe = MAX_STROBE / 2;
  } else {
    maxStrobe = strobe + 1;
  }

  for (strobe; strobe < maxStrobe; strobe++) {
    if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

    if(mode & GSM_READ_CSR) {

        //
        // Read from PCI config space
        //
        dataControl2.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL2_0_MCIO_DDRIO_REG));

    } else {
      //
      // Read from cache
      //
      dataControl2.Data = (*channelNvList)[ch].dataControl2[strobe];

    } // DdrLevel
    curVal = (UINT16)dataControl2.Bits.imodeeqcode;
    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *value = curVal;
    } else {
      //
      // Write
      //
      if ((dataControl2.Bits.imodebiasen == 0) && (*value != 0)) {
        //
        // Enable Imode registers per byte 
        //
        dataControl2.Bits.imodebiasen = 1;
        dataControl2.Bits.imodecfg = 1;
        dataControl2.Bits.imodeenable = 1;
      }

      if ((dataControl2.Bits.imodebiasen == 1) && (*value == 0)) {
        dataControl2.Bits.imodebiasen = 0;
        dataControl2.Bits.imodeenable = 0;
      }

      // Adjust the current CMD delay value by offset
      if (mode & GSM_WRITE_OFFSET) {
        //
        // Add the offset to the current value
        //
        tmp = curVal + *value;

        //
        // Make sure we do not exeed the limits
        //
        if (tmp > minLimit) {
          piDelay = tmp;
        } else {
          piDelay = minLimit;
        }
        // Ensure we do not exceed maximums
        if (piDelay > maxLimit) {
          piDelay = maxLimit;
        }
      } else {
        //
        // Write absolute value
        //
        piDelay = *value;
      }

      dataControl2.Bits.imodeeqcode = piDelay;

      {
        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        if ((dataControl2.Data != (*channelNvList)[ch].dataControl2[strobe]) || (mode & GSM_FORCE_WRITE)) {
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL2_0_MCIO_DDRIO_REG), dataControl2.Data);
        }
      }
    } // if read only

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
      {
        (*channelNvList)[ch].dataControl2[strobe] = dataControl2.Data;
      }
    }
  } // strobe loop

  return MRC_STATUS_SUCCESS;
} // GetSetImode

/**

  Get CTLE

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetCTLE (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  GSM_LT    level,
  GSM_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8                             maxStrobe;
  UINT16                            curVal = 0;
  UINT16                            piDelay;
  INT16                             tmp;
  UINT16                            maxLimit = 3;
  UINT16                            minLimit = 0;
  struct channelNvram               (*channelNvList)[MAX_CH];
  DATACONTROL3_0_MCIO_DDRIO_STRUCT  dataControl3;

  channelNvList = GetChannelNvList(host, socket);

  if (strobe > 8) return SUCCESS;

  //
  // Get minimum and maximum value
  //
  //GetDataGroupLimits(host, level, group, &minLimit, &maxLimit, &usDelay);

  if (strobe == ALL_STROBES) {
    strobe    = 0;
    maxStrobe = MAX_STROBE / 2;
  } else {
    maxStrobe = strobe + 1;
  }

  for (strobe; strobe < maxStrobe; strobe++) {
    if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

    if(mode & GSM_READ_CSR) {

        //
        // Read from PCI config space
        //
        dataControl3.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL3_0_MCIO_DDRIO_REG));

    } else {
      //
      // Read from cache
      //
      dataControl3.Data = (*channelNvList)[ch].dataControl3[strobe];

    } // DdrLevel

    switch (group) {
      case RxCtleC:
        curVal = (UINT16)dataControl3.Bits.ddrcrctlecapen;
        break;
      case RxCtleR:
        curVal = (UINT16)dataControl3.Bits.ddrcrctleresen;
        break;
    }

    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *value = curVal;
    } else {
      //
      // Write
      //
      // Adjust the current CMD delay value by offset
      if (mode & GSM_WRITE_OFFSET) {
        //
        // Add the offset to the current value
        //
        tmp = curVal + *value;

        //
        // Make sure we do not exeed the limits
        //
        if (tmp > minLimit) {
          piDelay = tmp;
        } else {
          piDelay = minLimit;
        }
        // Ensure we do not exceed maximums
        if (piDelay > maxLimit) {
          piDelay = maxLimit;
        }
      } else {
        //
        // Write absolute value
        //
        piDelay = *value;
      }

      switch (group) {
        case RxCtleC:
          dataControl3.Bits.ddrcrctlecapen = piDelay;
          break;
        case RxCtleR:
          dataControl3.Bits.ddrcrctleresen = piDelay;
          break;
      }

      {
        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        if ((dataControl3.Data != (*channelNvList)[ch].dataControl3[strobe]) || (mode & GSM_FORCE_WRITE)) {
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL3_0_MCIO_DDRIO_REG), dataControl3.Data);
        }
      }
    } // if read only

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
      {
        (*channelNvList)[ch].dataControl3[strobe] = dataControl3.Data;
      }
    }
  } // strobe loop

  return MRC_STATUS_SUCCESS;
} // GetSetCTLE

UINT16
CodeDeCodeRxOdt (
  UINT16 value, 
  UINT8  code
)
{
  UINT16 convertODT = 0;

  if (CODE_ODT == code) {
    // get Register value and convert it to ODT value
    switch (value){
      case 38:
        convertODT = 50;
        break;
      case 16:
        convertODT = 100;
        break;
      default:
        convertODT = 0;
        break;
    } // end switch value
  } else {
    // get ODT value and convert it to Register value
    switch (value){
      case 50:
        convertODT = 38;
        break;
      case 100:
        convertODT = 16;
        break;
      default:
        convertODT = 0;
        break;
    } // end switch value
  } // if code
  
  return convertODT;
} // CodeDeCodeRxOdt

UINT16
CodeDeCodeTxRon (
  UINT16 value, 
  UINT8  code
)
{

  UINT16 convertODT = 0;

  if (CODE_ODT == code) {
    // get Register value and convert it to ODT value
    switch (value){
      case 49:
        convertODT = TX_RON_14;
        break;
      case 44:
        convertODT = TX_RON_18;
        break;
      case 38:
        convertODT = TX_RON_24;
        break;
      default:
        convertODT = value;
        break;
    } // end switch value
  } else {
    // get ODT value and convert it to Register value
    switch (value){
      case TX_RON_14:
        convertODT = 49;
        break;
      case TX_RON_18:
        convertODT = 44;
        break;
      case TX_RON_24:
        convertODT = 38;
        break;
      default:
        convertODT = value;
        break;
    } // end switch value
  } // if code
  
  return convertODT;
} // CodeDeCodeTxRon

/**

  Get ODT

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetCPUODT (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  GSM_LT    level,
  GSM_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT16                                curVal = 0;
  UINT16                                tempVal = 0;
  UINT16                                piDelay;
  INT16                                 tmp;
  UINT16                                maxLimit = 3;
  UINT16                                minLimit = 0;
  UINT8                                 imc;
  struct channelNvram                   (*channelNvList)[MAX_CH];
  struct imcNvram                       (*imcNvList)[MAX_IMC];
  DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_STRUCT ddrCRCompCtl0;

  channelNvList = GetChannelNvList(host, socket);
  imcNvList     = GetImcNvList(host, socket);
  imc = GetMCID(host, socket, ch);

  if (ch & 1) return SUCCESS;

  //
  // Get minimum and maximum value
  //
  //GetDataGroupLimits(host, level, group, &minLimit, &maxLimit, &usDelay);

  if(mode & GSM_READ_CSR) {

    //
    // Read from PCI config space
    //
    ddrCRCompCtl0.Data = MemReadPciCfgMC (host, socket, imc, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG);

  } else {
    // Read from cache
    //
    ddrCRCompCtl0.Data = (*imcNvList)[imc].ddrCRCompCtl0;
  } // DdrLevel

  switch (group) {
    case RxOdt:
      tempVal = (UINT16)ddrCRCompCtl0.Bits.dqodtpupvref;
      curVal  = ConvertOdtValue (group, tempVal, CODE_ODT, host->nvram.mem.dimmTypePresent);
      break;
    case TxRon:
      tempVal = (UINT16)ddrCRCompCtl0.Bits.dqdrvpupvref;
      curVal  = ConvertOdtValue (group, tempVal, CODE_ODT, host->nvram.mem.dimmTypePresent);
      break;
  }

  //
  // Check if read only
  //
  if (mode & GSM_READ_ONLY) {
    //
    // Save the value read into the output parameter
    //
    *value = curVal;
  } else {
    //
    // Write
    //
    // Adjust the current CMD delay value by offset
    if (mode & GSM_WRITE_OFFSET) {
      //
      // Add the offset to the current value
      //
      tmp = tempVal + *value;

      //
      // Make sure we do not exeed the limits
      //
      if (tmp > minLimit) {
        piDelay = tmp;
      } else {
        piDelay = minLimit;
      }
      // Ensure we do not exceed maximums
      if (piDelay > maxLimit) {
        piDelay = maxLimit;
      }
    } else {
      //
      // Write absolute value
      //
      tempVal = *value;
      piDelay = ConvertOdtValue (group, tempVal, DECODE_ODT, host->nvram.mem.dimmTypePresent);  
    }

    switch (group) {
      case RxOdt:
        ddrCRCompCtl0.Bits.dqodtpupvref = piDelay;
        break;
      case TxRon:
        ddrCRCompCtl0.Bits.dqdrvpupvref = piDelay;
        break;
    }

    {
      //
      // Write it back if the current data does not equal the cache value or if in force write mode
      //
      // no cache
      if ((ddrCRCompCtl0.Data != (*imcNvList)[imc].ddrCRCompCtl0) || (mode & GSM_FORCE_WRITE)) {
        MemWritePciCfgMC (host, socket, imc, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);
        DoComp(host,socket);
      }
    }
  } // if read only

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    {
      // no cache
      (*imcNvList)[imc].ddrCRCompCtl0 = ddrCRCompCtl0.Data;
    }
  }

  return MRC_STATUS_SUCCESS;
} // GetSetCPUODT

/**

  Get ODT

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetDIMMODT (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  GSM_LT    level,
  GSM_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT16                            curVal = 0;
  UINT16                            tempVal = 0;
  UINT16                            piDelay = 0;
  INT16                             tmp;
  UINT16                            maxLimit = 15;
  UINT16                            minLimit = 0;
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  struct rankDevice   (*rankStruct)[MAX_RANK_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];

  rankList = GetRankNvList(host, socket, ch, dimm);
  rankStruct = GetRankStruct(host, socket, ch, dimm);
#ifdef  LRDIMM_SUPPORT
  channelNvList = GetChannelNvList(host, socket);
#endif

  //
  // Get minimum and maximum value
  //
  //GetDataGroupLimits(host, level, group, &minLimit, &maxLimit, &usDelay);

  //
  // Read from cache
  //
#ifdef  LRDIMM_SUPPORT
  if ((*channelNvList)[ch].lrDimmPresent) {
    switch (group) {
    case DramDrvStr:
      curVal = (*channelNvList)[ch].dimmList[dimm].lrBuf_BC03;
      break;
    case WrOdt:
      curVal = (*channelNvList)[ch].dimmList[dimm].lrBuf_BC01;
      break;
    case ParkOdt:
      curVal = (*channelNvList)[ch].dimmList[dimm].lrBuf_BC02;
      break;
    case NomOdt:
      curVal = (*channelNvList)[ch].dimmList[dimm].lrBuf_BC00;
      break;
    }
  } else
#endif
  {
    switch (group) {
    case DramDrvStr:
      tempVal = (*rankStruct)[rank].MR1 & BIT1;
      curVal  = ConvertOdtValue (group, tempVal, CODE_ODT, host->nvram.mem.dimmTypePresent);
      break;
    case WrOdt:
      tempVal = (*rankList)[rank].RttWr >> DDR4_RTT_WR_ODT_SHIFT;
      curVal  = ConvertOdtValue (group, tempVal, CODE_ODT, host->nvram.mem.dimmTypePresent);
      break;
    case ParkOdt:
      tempVal = (*rankList)[rank].RttPrk >> DDR4_RTT_PRK_ODT_SHIFT;
      curVal  = ConvertOdtValue (group, tempVal, CODE_ODT, host->nvram.mem.dimmTypePresent);
      break;
    case NomOdt:
      tempVal = (*rankList)[rank].RttNom >> DDR4_RTT_NOM_ODT_SHIFT;
      curVal  = ConvertOdtValue (group, tempVal, CODE_ODT, host->nvram.mem.dimmTypePresent);
      break;
    }
  }

  //
  // Check if read only
  //
  if (mode & GSM_READ_ONLY) {
    //
    // Save the value read into the output parameter
    //
    *value = curVal;
  } else {
    //
    // Write
    //
    // Adjust the current CMD delay value by offset
    if (mode & GSM_WRITE_OFFSET) {
      //
      // Add the offset to the current value
      //
      tmp = tempVal + *value;

      //
      // Make sure we do not exceed the limits
      //
      if (tmp > minLimit) {
        piDelay = tmp;
      } else {
        piDelay = minLimit;
      }
      // Ensure we do not exceed maximums
      if (piDelay > maxLimit) {
        piDelay = maxLimit;
      }
    } else {
      //
      // Write absolute value
      //
      tempVal = *value;
      piDelay = ConvertOdtValue (group, tempVal, DECODE_ODT, host->nvram.mem.dimmTypePresent);
    }

    {
      //
      // Write it back if the current data does not equal the cache value or if in force write mode
      //
#ifdef  LRDIMM_SUPPORT
      if ((*channelNvList)[ch].lrDimmPresent) {
        switch (group) {
        case DramDrvStr:
          (*channelNvList)[ch].dimmList[dimm].lrBuf_BC03 = (UINT8) piDelay;
          WriteLrbuf(host, socket, ch, dimm, 0, (UINT8) piDelay, LRDIMM_F0, LRDIMM_BC03);
          break;
        case WrOdt:
          (*channelNvList)[ch].dimmList[dimm].lrBuf_BC01 = (UINT8) piDelay;
          WriteLrbuf(host, socket, ch, dimm, 0, (UINT8) piDelay, LRDIMM_F0, LRDIMM_BC01);
          break;
        case ParkOdt:
          (*channelNvList)[ch].dimmList[dimm].lrBuf_BC02 = (UINT8) piDelay;
          WriteLrbuf(host, socket, ch, dimm, 0, (UINT8) piDelay, LRDIMM_F0, LRDIMM_BC02);
          break;
        case NomOdt:
          (*channelNvList)[ch].dimmList[dimm].lrBuf_BC00 = (UINT8) piDelay;
          WriteLrbuf(host, socket, ch, dimm, 0, (UINT8) piDelay, LRDIMM_F0, LRDIMM_BC00);
          break;
        }
      } else
#endif
      {
          switch (group) {
          case DramDrvStr:
            (*rankStruct)[rank].MR1 &= ~BIT1;
            (*rankStruct)[rank].MR1 |= piDelay;
            WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR1, BANK1);
            break;
          case WrOdt:
            (*rankList)[rank].RttWr = piDelay << DDR4_RTT_WR_ODT_SHIFT;
            (*rankStruct)[rank].MR2 &= ~(0x11 << DDR4_RTT_WR_ODT_SHIFT); // clear RTTWR from MR2
            (*rankStruct)[rank].MR2 |= (*rankList)[rank].RttWr;
            WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR2, BANK2);
            break;
          case ParkOdt:
            (*rankList)[rank].RttPrk = piDelay << DDR4_RTT_PRK_ODT_SHIFT;
            (*rankStruct)[rank].MR5 &= ~(0x111 << DDR4_RTT_PRK_ODT_SHIFT);
            (*rankStruct)[rank].MR5 |= (*rankList)[rank].RttPrk;
            WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR5, BANK5);
            break;
          case NomOdt:
            (*rankList)[rank].RttNom = piDelay << DDR4_RTT_NOM_ODT_SHIFT ;
            (*rankStruct)[rank].MR1 &= ~(0x111 << DDR4_RTT_NOM_ODT_SHIFT); // clear RTTNOM from MR1
            (*rankStruct)[rank].MR1 |= (*rankList)[rank].RttNom;
            WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR1, BANK1);
            break;
          }
      }
    }
  } // if read only

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
#ifdef  LRDIMM_SUPPORT
    if ((*channelNvList)[ch].lrDimmPresent) {
    } else
#endif
    {
      switch (group) {
      case DramDrvStr:
        (*rankStruct)[rank].MR1 &= ~BIT1;
        (*rankStruct)[rank].MR1 |= piDelay;
        break;
      case WrOdt:
        break;
      case ParkOdt:
        break;
      case NomOdt:
        break;
      }
    }
  }


  return MRC_STATUS_SUCCESS;
} // GetSetDIMMODT


/**

  MemWriteDimmVref: Procedure to write the DIMM Vref setting to the given target.

  @param host - Pointer to sysHost struct
  @param socket - processor number
  @param vref - vref value to write

  @retval SUCCESS if the Vref circuit is present and can be written
  @retval FAILURE if the Vref circuit is not present or cannot be written

**/
UINT32
MemWriteDimmVref (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  GSM_LT    level,
  UINT8     vref
  )
{
  UINT8                 vrefRange;
  UINT8                 dram;
  UINT8                 tempStrobe;
  struct channelNvram   (*channelNvList)[MAX_CH];
  struct rankDevice     (*rankStruct)[MAX_RANK_DIMM];
  struct dimmNvram      (*dimmNvList)[MAX_DIMM];
#ifdef LRDIMM_SUPPORT
  struct ddrRank        (*rankList)[MAX_RANK_DIMM];
  UINT8                 HostSideBufferTxVref[MAX_STROBE / 2];
  UINT8                 BufferIndex;
#endif

  channelNvList = GetChannelNvList(host, socket);
  rankStruct    = GetRankStruct(host, socket, ch, dimm);

  dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
#ifdef LRDIMM_SUPPORT
  if (level == LrbufLevel) {
    // for LR buffer side, only support RANGE1
    vrefRange = vref;
    // Enforce upper limit per JEDEC spec
    if (vrefRange > 50) {
      vrefRange = 50;
    }
  } else
#endif //LRDIMM
  {
    vrefRange = GetVrefRange(host,vref);
  }

#ifdef LRDIMM_SUPPORT
  if (host->nvram.mem.socket[socket].channelList[ch].lrDimmPresent && (host->nvram.mem.dramType == SPD_TYPE_DDR4)) {

    if (level == DdrLevel) {

      rankList = GetRankNvList(host, socket, ch, dimm);
      //
      // Get per strobe range settings into an array
      //
      // (take vref range select bit 6 and put it in the LRDIMM buffer register bit 1)
      //NOTE: the backside DRAM range is written to range 1...ok for now, but may need to change later
      for (BufferIndex = 0; BufferIndex < (MAX_STROBE / 2); BufferIndex++) {
        HostSideBufferTxVref[BufferIndex] = ((((GetVrefRange(host, (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][BufferIndex])) & BIT6) >> 5) | BIT0);
      }
      HostSideBufferTxVref[strobe % 9] =  ((vrefRange & BIT6) >> 5)| BIT0;
      if ((host->setup.mem.optionsExt & PDA_EN) && (host->var.mem.txVrefDone)) {
        //NOTE: the backside DRAM range is written to range 1...ok for now, but may need to change later
        WriteLrbufPBA(host, socket, ch, dimm, rank, HostSideBufferTxVref, LRDIMM_F6, LRDIMM_BC4x, (strobe % 9));
      } else {
        WriteLrbuf(host, socket, ch, dimm, 0, HostSideBufferTxVref[0], LRDIMM_F6, LRDIMM_BC4x);
      }
      // Get per strobe txVref settings into an array
      //
      for (BufferIndex = 0; BufferIndex < (MAX_STROBE / 2); BufferIndex++) {
        HostSideBufferTxVref[BufferIndex] = ((GetVrefRange(host, (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][BufferIndex])) & 0x3F);
      }
      HostSideBufferTxVref[strobe % 9] = vrefRange & 0x3F;
      //
      // program LRDIMM Host Interface VREF Control Word
      //
      if ((host->setup.mem.optionsExt & PDA_EN) && (host->var.mem.txVrefDone)) {
        WriteLrbufPBA(host, socket, ch, dimm, rank, HostSideBufferTxVref, LRDIMM_F5, LRDIMM_BC5x, (strobe % 9));
      } else {
        WriteLrbuf(host, socket, ch, dimm, 0, HostSideBufferTxVref[0], LRDIMM_F5, LRDIMM_BC5x);
      }
    } else {
      //LEVEL = LrbufLevel

     // Update MR6 value for each strobe with new vref setting mode value
     // For LRDIMMs, broadcast mode where the same MR6 value is used for all strobes
      for (tempStrobe = 0; tempStrobe < MAX_STROBE; tempStrobe++) {
        (*rankStruct)[rank].MR6[tempStrobe] &= ~0x7F;
        (*rankStruct)[rank].MR6[tempStrobe] |= (vrefRange & 0x7F);
      }

      MRSCmdStacking(host, socket, ch, dimm, LOAD_MRS);
      // Write the new DRAM Vref value and enable VrefDQ training mode
      WriteMRS(host, socket, ch, dimm, rank, (*rankStruct)[rank].MR6[strobe] | BIT7, BANK6);
      WriteMRS(host, socket, ch, dimm, rank, (*rankStruct)[rank].MR6[strobe] | BIT7, BANK6);
      WriteMRS(host, socket, ch, dimm, rank, (*rankStruct)[rank].MR6[strobe], BANK6);
      MRSCmdStacking(host, socket, ch, dimm, EXECUTE_MRS);
    }
    WriteBC(host, socket, ch, dimm, 0, 0, LRDIMM_F0, LRDIMM_BC7x);
    (*channelNvList)[ch].dimmList[dimm].rcLrFunc = 0;

  } else
#endif
  {
    // Update MR6 value with new vref setting mode value
    (*rankStruct)[rank].MR6[strobe] &= ~0x7F;
    (*rankStruct)[rank].MR6[strobe] |= (vrefRange & 0x7F);

    // Write the new DRAM Vref value and enable VrefDQ training mode
    if ((host->setup.mem.optionsExt & PDA_EN) && host->var.mem.txVrefDone) {
      if ((*dimmNvList)[dimm].x4Present){
        if (strobe >= 9) {
          dram = ((strobe - 9) * 2) + 1;
        } else {
          dram = strobe * 2;
        }
      } else {
        dram = strobe % 9;
      }
      WriteMRSPDA(host, socket, ch, dimm, rank, dram, (*rankStruct)[rank].MR6[strobe] | BIT7, BANK6);
      WriteMRSPDA(host, socket, ch, dimm, rank, dram, (*rankStruct)[rank].MR6[strobe] | BIT7, BANK6);
      WriteMRSPDA(host, socket, ch, dimm, rank, dram, (*rankStruct)[rank].MR6[strobe], BANK6);
    } else {
      WriteMRS(host, socket, ch, dimm, rank, (*rankStruct)[rank].MR6[strobe] | BIT7, BANK6);
      WriteMRS(host, socket, ch, dimm, rank, (*rankStruct)[rank].MR6[strobe] | BIT7, BANK6);
      WriteMRS(host, socket, ch, dimm, rank, (*rankStruct)[rank].MR6[strobe], BANK6);
    }
  }
  return SUCCESS;
} // MemWriteDimmVref

/**

  Converts the Vref setting to be written into MR6

  @param host - Pointer to sysHost struct
  @param vref - vref value to write

**/
UINT8
GetVrefRange (
  PSYSHOST  host,
  UINT8     vref
  )
{
  UINT8 vrefRange;

  //
  // Select the Vref range
  //
  if (vref < DDR4_VREF_RANGE1_OFFSET) {

    // Use range 2 and enforce lower limit
    if (vref > DDR4_VREF_RANGE2_OFFSET) {
      vrefRange = (vref - DDR4_VREF_RANGE2_OFFSET) | BIT6;
    } else {
      vrefRange = BIT6;
    }

  } else {

    // Use range 1
    vrefRange = vref - DDR4_VREF_RANGE1_OFFSET;

    // Enforce upper limit per JEDEC spec
    if (vrefRange > 50) {
      vrefRange = 50;
    }
  }

  return vrefRange;
} // GetVrefRange

/**

  Converts the Vref setting to be written into MR6

  @param host    - Pointer to sysHost struct
  @param socket  - processor number

**/
void
UpdateSafeTxVref (
  PSYSHOST  host,
  UINT8     socket,
  GSM_LT    level
                 )
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  UINT8               maxStrobe;
  UINT8               hiVref;
  UINT8               loVref;
  UINT8               safelevel;
  INT16               vref;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];

  channelNvList = GetChannelNvList(host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
      // skip NVMDIMM backside
      if (((*dimmNvList)[dimm].aepDimmPresent) && (level == LrbufLevel)) continue;
      // skip if lrdimm level but no LRDIMM found
      if ((level == LrbufLevel) && (IsLrdimmPresent(host, socket, ch, dimm) == 0)) continue;
      maxStrobe = GetMaxStrobe(host, socket, ch, dimm, TxVref, level);

      rankList = GetRankNvList(host, socket, ch, dimm);

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        loVref = 0xFF;
        hiVref = 0;

        for (strobe = 0; strobe < maxStrobe; strobe++) {
          if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

          GetSetDataGroup (host, socket, ch, dimm, rank, strobe, ALL_BITS, level, TxVref, GSM_READ_ONLY, &vref);

          if (loVref > vref) {
            loVref = (UINT8)vref;
          }
          if (hiVref < vref) {
            hiVref = (UINT8)vref;
          }
        } // strobe loop

        safelevel = (UINT8)((loVref + hiVref) / 2);

        if (level ==  DdrLevel) {
          (*channelNvList)[ch].txVrefSafe[(*rankList)[rank].rankIndex] = safelevel;
        } else  if (level ==  LrbufLevel){
          (*channelNvList)[ch].txVrefSafeLrbufLevel[(*rankList)[rank].rankIndex] = safelevel;
        }
        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
              "txVrefSafe = 0x%x\n", safelevel));
      } // rank loop
    } // dimm loop
  } // ch loop
} // UpdateSafeTxVref

/**

  Programs IO delays

  @param host    - Pointer to sysHost
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program

  @retval N/A

**/
MRC_STATUS
GetDataGroupLimits (
  PSYSHOST  host,
  GSM_LT    level,
  GSM_GT    group,
  UINT16    *minLimit,
  UINT16    *maxLimit,
  UINT16    *usDelay
  )
{
  MRC_STATUS  status = MRC_STATUS_SUCCESS;

  if (level == DdrLevel) {
    switch (group) {
      case RecEnDelay:
      case RxDqsDelay:
      case RxDqsPDelay:
      case RxDqsNDelay:
      case RxEq:
      case WrLvlDelay:
      case TxDqsDelay:
      case TxDqDelay:
      case TxEq:
        *minLimit = 0;
        *maxLimit = 511;
        *usDelay  = 0;
        break;
      case RxDqsPBitDelay:
      case RxDqsNBitDelay:
      case TxDqBitDelay:
        *minLimit = 0;
        *maxLimit = 127;
        *usDelay  = 0;
        break;
      //case RxDqDelay:
      case RxVref:
        //*minLimit = -MAX_RD_VREF_OFFSET;
        *minLimit = 0;
        *maxLimit = 127;
        *usDelay  = 1;
        break;
      case TxVref:
        //*minLimit = -MAX_WR_VREF_OFFSET;
        if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
          *maxLimit = DDR4_VREF_RANGE1_OFFSET + 50;
          *minLimit = DDR4_VREF_RANGE2_OFFSET;
        } else {
          *minLimit = 0;
          *maxLimit = 127 >> 1;
        }
        *usDelay  = 1;
        break;
     case RxDqDelay:
        *minLimit = 0;
        *maxLimit = 7;
        *usDelay  = 0;
        break;
     default:
        status = MRC_STATUS_GROUP_NOT_SUPPORTED;
        break;
    } // switch group
#ifdef LRDIMM_SUPPORT
  } else if (level == LrbufLevel) {
    switch (group) {
      case TxDqDelay:
      case RxDqsDelay:
      case RxDqsPDelay:
      case RxDqsNDelay:
        *minLimit = 0;
        *maxLimit = 30;
        *usDelay  = 0;
        break;
      case RxVref:
        //NOTE...Assume range 1 only, 0 to 50
        *maxLimit = 50;
        *minLimit = 0;
        *usDelay  = 1;
        break;

      case TxVref:
        if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
          *maxLimit = 50;
          *minLimit = 0;
        } else {
          *minLimit = 0;
          if (host->setup.mem.optionsExt & LRDIMM_BACKSIDE_VREF_EN) {
            *maxLimit = MAX_WR_VREF_OFFSET_BACKSIDE;
          } else {
          *maxLimit = MAX_WR_VREF_OFFSET;
        }
        }
        *usDelay  = 1;
        break;
     default:
        status = MRC_STATUS_GROUP_NOT_SUPPORTED;
        break;
    } // switch group
#endif
  } else {
    status = MRC_STATUS_LEVEL_NOT_SUPPORTED;
  }

  return status;
} // GetDataGroupLimits

/**

  Cache all DDRIO registers

  @param host    - Pointer to sysHost
  @param socket  - Socket number

**/
UINT32
CacheDDRIO (
  PSYSHOST  host,
  UINT8     socket
  )
{
  UINT8               imc;
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  UINT16              minVal = 0xFF;
  UINT16              maxVal = 0x0;
  UINT16              zeroValue = 0;
  INT16               tempVal = 0;
  struct imcNvram     (*imcNvList)[MAX_IMC];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];

  channelNvList = GetChannelNvList(host, socket);
  imcNvList     = GetImcNvList(host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    if ((host->setup.mem.socket[socket].ddrCh[ch].batterybacked) && (host->var.mem.subBootMode == AdrResume)) continue;

    GetSetCmdGroupDelay (host, socket, ch, CmdAll, GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &tempVal, &minVal, &maxVal);
    GetSetDataGroup (host, socket, ch, 0, 0, ALL_STROBES, ALL_BITS, DdrLevel, RxOdt, GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &tempVal);
    GetSetDataGroup (host, socket, ch, 0, 0, ALL_STROBES, ALL_BITS, DdrLevel, TxRon, GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &tempVal);

    (*channelNvList)[ch].roundtrip0 = MemReadPciCfgEp (host, socket, ch, ROUNDTRIP0_MCDDC_DP_REG);
    (*channelNvList)[ch].roundtrip1 = MemReadPciCfgEp (host, socket, ch, ROUNDTRIP1_MCDDC_DP_REG);

    //
    // Loop for each dimm and each rank
    //
    dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      rankList = &host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].rankList;
      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        GetSetCtlGroupDelay (host, socket, ch, (*rankList)[rank].ctlIndex + CtlGrp0, GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &tempVal, &minVal, &maxVal);
        GetSetClkDelay (host, socket, ch, (*rankList)[rank].ckIndex, GSM_READ_CSR | GSM_READ_ONLY, &tempVal);

        for (strobe = 0; strobe < MAX_STROBE/2; strobe++) {
          if ((!host->nvram.mem.eccEn) && (strobe == 8)) continue;

          GetSetTxDelay (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, TxDqDelay, GSM_READ_CSR | GSM_UPDATE_CACHE |
                         GSM_READ_ONLY, (INT16 *)&zeroValue);
          GetSetTxDelay (host, socket, ch, dimm, rank, strobe + 9, 0, DdrLevel, TxDqDelay, GSM_READ_CSR | GSM_UPDATE_CACHE |
                         GSM_READ_ONLY, (INT16 *)&zeroValue);
          GetSetRxDelay (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, RxDqDelay, GSM_READ_CSR | GSM_UPDATE_CACHE |
                         GSM_READ_ONLY, (INT16 *)&zeroValue);
          GetSetRxDelay (host, socket, ch, dimm, rank, strobe + 9, 0, DdrLevel, RxDqDelay, GSM_READ_CSR | GSM_UPDATE_CACHE |
                         GSM_READ_ONLY, (INT16 *)&zeroValue);
          (*channelNvList)[ch].dataOffsetTrain[strobe] = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATAOFFSETTRAIN_0_MCIO_DDRIO_REG));
          (*channelNvList)[ch].dataOffsetTrain[strobe + 9] = (*channelNvList)[ch].dataOffsetTrain[strobe];
        } // strobe loop
      } // rank loop
    } // dimm loop
  } // ch loop

  for (imc = 0; imc < MAX_IMC; imc++) {
    if ((*imcNvList)[imc].enabled == 0) continue;
    (*imcNvList)[imc].ddrCRCompCtl0 = MemReadPciCfgMC (host, socket, imc, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG);
  }

  return SUCCESS;
} // CacheDDRIO

/**

  Offsets a field in TxRxBotRank

  @param piDelay     - Current Pi delay
  @param offset      - Amount to offset
  @param maxPi       - Maximum pi delay
  @param maxLogic    - Maximum logic delay

**/
UINT32
AddOffset (
  UINT16  *piDelay,
  INT16   offset,
  UINT16  maxPi,
  UINT8   maxLogic
  )
{
  INT16   curDelay;
  INT16   newDelay;
  INT16   maxDelay;
  UINT32  status = SUCCESS;

  curDelay = *piDelay;
  maxDelay = (INT16)maxLogic * 64 + (INT16)maxPi;

  if (curDelay + offset < 0) {
    newDelay = 0;
    status = FAILURE;
  } else if (curDelay + offset > maxDelay) {
    newDelay = maxDelay;
    status = FAILURE;
  } else {
    newDelay = curDelay + offset;
  }
  *piDelay = newDelay % maxPi;

  return  status;
} // AddOffset


/**

  This resets the MC IO module

  @param host    - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
void
IO_Reset (
  PSYSHOST host,
  UINT8    socket
  )
{
  MC_INIT_STATE_G_MC_MAIN_HSX_BDX_STRUCT mcInitStateG;

  mcInitStateG.Data = MemReadPciCfgMain (host, socket, MC_INIT_STATE_G_MC_MAIN_REG);

  //
  // Reset DDR IO
  //
  mcInitStateG.Bits.reset_io = 1;
  MemWritePciCfgMain (host, socket, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);

  //
  // Wait 20 QCLK with reset high.
  //
  FixedQClkDelay (host, 20);

  //
  // Clear DDR IO Reset
  //
  mcInitStateG.Bits.reset_io = 0;
  MemWritePciCfgMain (host, socket, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);

  //
  // Wait again 20 QCLK for stable IO.
  //
  FixedQClkDelay (host, 20);
} // IO_Reset

/**

  This function updates the function block number

  @param strobe            - Strobe number (0-based)
  @param RegisterGroupBase - Address of the the register group

  @retval IO Address

**/
UINT32
UpdateIoRegister (
  PSYSHOST    host,
  UINT8       rank,
  UINT8       strobe,
  UINT32      regOffset
  )
{
  CSR_OFFSET  csrReg;

  csrReg.Data = regOffset;

  // Bytes 7 and 8 are in the DDRIO_EXT function
  if ((strobe == 7) || (strobe == 8) || (strobe == 16) || (strobe == 17)) {
    csrReg.Bits.funcblk = csrReg.Bits.funcblk + 1;
  }

  //
  // Add in offset for data group
  //
  csrReg.Bits.offset = csrReg.Bits.offset + ddrioOffsetTable[strobe];

  //
  // Add in offset for rank
  //
  csrReg.Bits.offset = csrReg.Bits.offset + (rank * 4);

  return csrReg.Data;
} // UpdateIoRegister

/**

  This function updates the function block number

  @param strobe            - Strobe number (0-based)
  @param RegisterGroupBase - Address of the the register group

  @retval IO Address

**/
UINT32
UpdateIoRegisterCh (
                   PSYSHOST    host,
                   UINT8       strobe,
                   UINT32      regOffset
                   )
{
  return regOffset;
} // UpdateIoRegisterCh

/**

  Initializes the data control 0 register

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)

  @retval N/A

**/
void
InitDataControl0 (
  PSYSHOST host,
  UINT8    socket
)
{
  UINT8                                     ch;
  UINT8                                     strobe;
  struct channelNvram                       (*channelNvList)[MAX_CH];
  DATACONTROL0_0_MCIO_DDRIO_STRUCT      dataControl0;

  channelNvList = GetChannelNvList(host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    if (host->nvram.mem.socket[socket].ddrVoltage == SPD_VDD_150) {
      dataControl0.Bits.datavccddqhi  = 1;
    } else {
      dataControl0.Bits.datavccddqhi  = 0;
    }

    (*channelNvList)[ch].dataControl0 = dataControl0.Data;

    for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
      //
      // Skip if this is an ECC strobe when ECC is disabled
      //
      if ((!host->nvram.mem.eccEn) && (strobe == 8)) continue;

      //
      // Set training mode and rank to all channels
      //
      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
    } // strobe loop
  } // ch loop
} // InitDataControl0

/**

  Set the training mode

  @param host  - Pointer to sysHost
  @param socket  - Socket number
  @param dimm  - DIMM number (0-based)
  @param rank  - Rank number (0-based)
  @param mode  - Training mode

  @retval N/A

**/
void
SetTrainingMode (
  PSYSHOST host,
  UINT8    socket,
  UINT8    dimm,
  UINT8    rank,
  UINT8    mode
  )
{
  UINT8                                 ch;
  UINT8                                 strobe;
  struct channelNvram                   (*channelNvList)[MAX_CH];
  DATACONTROL0_0_MCIO_DDRIO_STRUCT      dataControl0;

  DATACONTROL2_0_MCIO_DDRIO_BDX_STRUCT  dataControl2;
  channelNvList = GetChannelNvList(host, socket);

  if ((host->var.common.cpuType == CPU_HSX) || (host->var.common.cpuType == CPU_BDX)) {
    dataControl0.Data = 0;
    dataControl2.Data = 0;

    switch (mode) {
      case RECEIVE_ENABLE_BASIC:
        dataControl0.Bits.rltrainingmode  = 1;
        dataControl0.Bits.forceodton      = 1;
        dataControl2.Bits.forcerxon       = 1;
        break;

      case RX_DQ_DQS_BASIC:
        dataControl0.Bits.rxtrainingmode  = 1;
        if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
          dataControl0.Bits.forceodton      = 1;
          dataControl2.Bits.forcerxon       = 1;
        }
        break;

      case WRITE_LEVELING_BASIC:
        dataControl0.Bits.wltrainingmode  = 1;
        dataControl0.Bits.txpion          = 1;
        dataControl2.Bits.forcebiason     = 1;
        break;

      case SENSE_AMP:
        dataControl0.Bits.forceodton      = 1;
        dataControl0.Bits.senseamptrainingmode  = 1;
        dataControl2.Bits.forcebiason     = 1;
        dataControl2.Bits.forcerxon       = 1;
        break;

      case DISABLE_SENSE_AMP:
        dataControl0.Bits.forceodton      = 1;
        dataControl2.Bits.forcebiason     = 1;
        dataControl2.Bits.forcerxon       = 1;
        break;

    } // switch mode

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

      for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if ((!host->nvram.mem.eccEn) && (strobe == 8)) continue;

        //
        // Set training mode and rank to all channels
        //
        // If enabling, program datacontrol2 first, if disabling program 0 first.
        if ((dataControl0.Bits.rltrainingmode == 1) ||
            (dataControl0.Bits.rxtrainingmode == 1) ||
            (dataControl0.Bits.wltrainingmode == 1) ||
            (dataControl0.Bits.senseamptrainingmode == 1)) {
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL2_0_MCIO_DDRIO_REG), dataControl2.Data | (*channelNvList)[ch].dataControl2[strobe]);
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data | (*channelNvList)[ch].dataControl0);
        } else {
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data | (*channelNvList)[ch].dataControl0);
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL2_0_MCIO_DDRIO_REG), dataControl2.Data | (*channelNvList)[ch].dataControl2[strobe]);
        }
      } // strobe loop
    } // ch loop
  } else {

  }
} // SetTrainingMode

/**

  Set Rd Vref

  @param host    - Pointer to sysHost
  @param socket    - Socket number
  @param ch      - Channel number (0-based)
  @param strobe  - Strobe number (0-based)
  @param vref    - Vref setting to set

  @retval N/A

**/
void
WriteRdVref (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    strobe,
  UINT8    vref
  )
{
} // WriteRdVref

/**

  Offset Vref

  @param vref    - Current vRef setting
  @param sign    - Direction to offset
  @param offset  - Amount to offset the current Vref

  @retval New vref

**/
UINT32
OffsetVref (
  UINT32 vref,
  INT16 offset
  )
{
  INT16 tmpVref;

  // Convert to a signed integer
  if (vref & BIT5) {
    tmpVref = (INT16)(vref & 0x1F) ;
  } else {
    tmpVref = 0 - (INT16)(vref);
  }

  tmpVref = tmpVref + offset;

  // Convert back
  if (tmpVref < 0) {
    vref = (UINT8)((0 - tmpVref));
  } else {
    vref = tmpVref | BIT5;
  }

  return vref;
} // OffsetVref


/**

  Initialize IO Comp settings and issue a comp cycle

  @param host    - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
UINT32
ProgramIOCompValues (
  PSYSHOST  host,
  UINT8     socket
  )
{
  return SUCCESS;
} // ProgramIOCompValues

/**

  Issue a comp cycle

  @param host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
void
DoComp (
  PSYSHOST host,
  UINT8    socket
  )
{
  UINT8                               mcId;
  RCOMP_TIMER_MC_MAIN_HSX_BDX_STRUCT  rcompTimer;


  if (host->var.common.emulation != VP_FLAG) {
    for (mcId = 0; mcId < MAX_IMC; mcId++) {
      if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

      //
      // Read RCOMP_TIMER
      //
      rcompTimer.Data = MemReadPciCfgMC(host, socket, mcId,RCOMP_TIMER_MC_MAIN_REG);
      //
      // Make sure rcomp bit is 0 to begin with
      //
      if(IsBDXEP(host)) {
        rcompTimer.Bits.count = 0xFFF;
      } else {
        rcompTimer.Bits.count = 0xED0;
      }
      rcompTimer.Bits.rcomp = 0;
      //
      // Write value back
      //
      MemWritePciCfgMC(host, socket, mcId, RCOMP_TIMER_MC_MAIN_REG, rcompTimer.Data);

      //
      // Set rcomp
      //
      rcompTimer.Bits.rcomp = 1;
      //
      // Write value back
      //
      MemWritePciCfgMC(host, socket, mcId, RCOMP_TIMER_MC_MAIN_REG, rcompTimer.Data);

      //
      // Wait for the COMP cycle to complete
      //
      while ((rcompTimer.Bits.rcomp_in_progress == 1) || (rcompTimer.Bits.first_rcomp_done == 0)){
        rcompTimer.Data = MemReadPciCfgMC (host, socket, mcId, RCOMP_TIMER_MC_MAIN_REG);
#ifdef  RC_SIM_FEEDBACK
        break;
#endif
      }

      //
      //Clear rcomp bit
      //
      rcompTimer.Bits.rcomp = 0;
      MemWritePciCfgMC(host, socket, mcId, RCOMP_TIMER_MC_MAIN_REG, rcompTimer.Data);
    } // mcId loop
  }

} // DoComp

/**

  Issue a ZQ cycle

  @param host            - sysHost data structure
  @param socket          - Processor socket
  @param ch              - Channel to issue the ZQ to
  @param zqType          - Type of ZQ Calibration: Long or Short

  @retval N/A

**/
void
DoZQ (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     zqType
  )
{
  CpgcIssueZQ(host, socket, ch, zqType);
} // DoZQ

/**

  Set the IO latency register value

  @param host            - Pointer to sysHost
  @param socket            - Socket numer
  @param ch              - Channel number (0-based)
  @param rank            - Rank number (0-based)
  @param IOLatencyValue  - New IO Latency value

  @retval N/A

**/
void
SetIOLatency (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    rank,
  UINT32   IOLatencyValue
  )
{
  IOLATENCY0_MCDDC_DP_STRUCT  ioLatency;
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  ioLatency.Data = MemReadPciCfgEp (host, socket, ch, IOLATENCY0_MCDDC_DP_REG);

  ioLatency.Data &= ~(0xF << (rank * 4));
  ioLatency.Data |= IOLatencyValue << (rank * 4);

  MemWritePciCfgEp (host, socket, ch, IOLATENCY0_MCDDC_DP_REG, ioLatency.Data);
  (*channelNvList)[ch].ioLatency0 = ioLatency.Data;
} // SetIOLatency

/**

  Gets the IO latency register value

  @param host  - Pointer to sysHost
  @param socket  - Socket numer
  @param ch    - Channel number (0-based)
  @param rank  - Rank number (0-based)

  @retval IO Latency

**/
UINT8
GetIOLatency (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    rank
  )
{
  IOLATENCY0_MCDDC_DP_STRUCT    ioLatency;
  UINT8                         Value;

  ioLatency.Data  = MemReadPciCfgEp (host, socket, ch, IOLATENCY0_MCDDC_DP_REG);
  Value           = (UINT8) ((ioLatency.Data >> (rank * 4)) & 0xF);

  return Value;
} // GetIOLatency

/**

  Set the round trip register value

  @param host            - Pointer to sysHost
  @param socket          - Socket number
  @param ch              - Channel number (0-based)
  @param rank            - Rank number (0-based)
  @param RoundTripValue  - New Round Trip Latency value

  @retval VOID

**/
UINT8
SetRoundTrip (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    rank,
  UINT8    RoundTripValue
  )
{
  UINT8                       rk;
  ROUNDTRIP0_MCDDC_DP_STRUCT  roundTrip;
  struct channelNvram         (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  if (rank < 4) {
    if (host->var.common.bootMode == S3Resume ||
      ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode != ColdBoot)))
    {
      roundTrip.Data = (*channelNvList)[ch].roundtrip0;
    }
    else
    {
      roundTrip.Data  = MemReadPciCfgEp (host, socket, ch, ROUNDTRIP0_MCDDC_DP_REG);
      rk              = rank;
      roundTrip.Data &= ~(0xFF << (rk * 8));
      roundTrip.Data |= RoundTripValue << (rk * 8);
    }
  } else {
    if (host->var.common.bootMode == S3Resume ||
      ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode != ColdBoot)))
    {
      roundTrip.Data = (*channelNvList)[ch].roundtrip1;
    }
    else
    {
      roundTrip.Data  = MemReadPciCfgEp (host, socket, ch, ROUNDTRIP1_MCDDC_DP_REG);
      rk              = rank - 4;
      roundTrip.Data &= ~(0xFF << (rk * 8));
      roundTrip.Data |= RoundTripValue << (rk * 8);
    }
  }

  if (rank < 4) {
    MemWritePciCfgEp (host, socket, ch, ROUNDTRIP0_MCDDC_DP_REG, roundTrip.Data);
    (*channelNvList)[ch].roundtrip0 = roundTrip.Data;
  } else {
    MemWritePciCfgEp (host, socket, ch, ROUNDTRIP1_MCDDC_DP_REG, roundTrip.Data);
    (*channelNvList)[ch].roundtrip1 = roundTrip.Data;
  }
  return RoundTripValue;
} // SetRoundTrip

/**

  Get the round trip register value

  @param host  - Pointer to sysHost
  @param socket  - Socket numer
  @param ch    - Channel number (0-based)
  @param rank  - Rank number (0-based)

  @retval N/A

**/
UINT8
GetRoundTrip (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    rank
  )
{
  UINT8                       rk;
  UINT8                       Value;
  ROUNDTRIP0_MCDDC_DP_STRUCT  roundTrip;

  if (rank < 4) {
    roundTrip.Data  = MemReadPciCfgEp (host, socket, ch, ROUNDTRIP0_MCDDC_DP_REG);
    rk              = rank;
  } else {
    roundTrip.Data  = MemReadPciCfgEp (host, socket, ch, ROUNDTRIP1_MCDDC_DP_REG);
    rk              = rank - 4;
  }

  Value = (UINT8) ((roundTrip.Data >> (rk * 8)) & 0xFF);

  return Value;
} // GetRoundTrip

/**

  Clears Bit-wise error status registers per channel

  @param host  - Pointer to sysHost
  @param socket  - Socket number
  @param ch    - Channel number (0-based)

  @retval N/A

**/
void
ClearBWErrorStatus (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch
  )
{
} // ClearBWErrorStatus


#ifdef SERIAL_DBG_MSG
char *GetGroupStr(GSM_GT group, char *strBuf) {

  switch (group) {
    case RecEnDelay:
      StrCpyLocal(strBuf, "RecEnDelay");
      break;
    case RxDqsDelay:
      StrCpyLocal(strBuf, "RxDqsDelay");
      break;
    case RxDqsPDelay:
      StrCpyLocal(strBuf, "RxDqsPDelay");
      break;
    case RxDqsNDelay:
      StrCpyLocal(strBuf, "RxDqsNDelay");
      break;
    case RxDqsBitDelay:
      StrCpyLocal(strBuf, "RxDqsBitDelay");
      break;
    case RxDqsPBitDelay:
      StrCpyLocal(strBuf, "RxDqsPBitDelay");
      break;
    case RxDqsNBitDelay:
      StrCpyLocal(strBuf, "RxDqsNBitDelay");
      break;
    case TxDqBitDelay:
      StrCpyLocal(strBuf, "TxDqBitDelay");
      break;
    case RxDqDelay:
      StrCpyLocal(strBuf, "RxDqDelay");
      break;
    case WrLvlDelay:
      StrCpyLocal(strBuf, "WrLvlDelay");
      break;
    case TxDqsDelay:
      StrCpyLocal(strBuf, "TxDqsDelay");
      break;
    case TxDqDelay:
      StrCpyLocal(strBuf, "TxDqDelay");
      break;
    case RxVref:
      StrCpyLocal(strBuf, "RxVref");
      break;
    case TxVref:
      StrCpyLocal(strBuf, "TxVref");
      break;
    case TxEq:
      StrCpyLocal(strBuf, "TxEq");
      break;
    case RxEq:
      StrCpyLocal(strBuf, "RxEq");
      break;
    case TxImode:
      StrCpyLocal(strBuf, "TxImode");
      break;
    case RxCtleC:
      StrCpyLocal(strBuf, "RxCtleC");
      break;
    case RxCtleR:
      StrCpyLocal(strBuf, "RxCtleR");
      break;
    case RxOdt:
      StrCpyLocal(strBuf, "RxOdt");
      break;
    case TxRon:
      StrCpyLocal(strBuf, "TxRon");
      break;
    case DramDrvStr:
      StrCpyLocal(strBuf, "DramDrvStr");
      break;
    case ParkOdt:
      StrCpyLocal(strBuf, "ParkOdt");
      break;
    case NomOdt:
      StrCpyLocal(strBuf, "NomOdt");
      break;
    case WrOdt:
      StrCpyLocal(strBuf, "WrOdt");
      break;
    default:
      StrCpyLocal(strBuf, "Unknown group");
      break;
  }
  return strBuf;
} // GetGroupStr
#endif  // SERIAL_DBG_MSG
