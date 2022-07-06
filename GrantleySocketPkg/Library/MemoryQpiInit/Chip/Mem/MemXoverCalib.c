 /*++
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
--*/
/*************************************************************************
 *
 * Memory Reference Code
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
 ************************************************************************
 *
 *  PURPOSE:
 *
 *      This file contains memory detection and initialization for
 *      IMC and DDR3 modules compliant with JEDEC specification.
 *
 ************************************************************************/

#include "SysFunc.h"
#include "Cpgc.h"

extern const UINT32 dataTrainFeedbackTable[MAX_STROBE];

#define   X_OVER_STEP_SIZE          1
#define   X_OVER_PI_START           0
#define   X_OVER_MAX_SAMPLE         64
#define   X_OVER_OFFSET_HSX         58
#define   X_OVER_OFFSET_BDX         58

#define   X_OVER_ODD_FUB_OFFSET     32

#define   X_OVER_OFFSET_2TO2_RCVEN_HSX  34
#define   X_OVER_OFFSET_2TO2_TXDQS_HSX  40
#define   X_OVER_OFFSET_2TO2_TXDQ_HSX   0
#define   X_OVER_OFFSET_2TO2_CMD_HSX    112
#define   X_OVER_OFFSET_2TO2_CTL_HSX    112
#define   X_OVER_OFFSET_2TO2_CLK_HSX    112

#define   X_OVER_OFFSET_2TO2_RCVEN_DDR2133  33
#define   X_OVER_OFFSET_2TO2_RCVEN          35
#define   X_OVER_OFFSET_2TO2_TXDQS_DDR2133  40
#define   X_OVER_OFFSET_2TO2_TXDQS          42
#define   X_OVER_OFFSET_2TO2_TXDQ   0
#define   X_OVER_OFFSET_2TO2_CMD    112
#define   X_OVER_OFFSET_2TO2_CTL    112
#define   X_OVER_OFFSET_2TO2_CLK    112

UINT32
CrossoverCalib (
  PSYSHOST  host
  )
/*++

  Peform crossover calibration

  @param host  - Pointer to sysHost

  @retval Status

--*/
{
  UINT8                                           socket;
  UINT8                                           ch;
  UINT8                                           strobe;
  UINT8                                           sweepPi;
  UINT8                                           xoverLoops;
  UINT8                                           breakOut;
  UINT16                                          piDelay;
  UINT32                                          status;
  UINT32                                          feedBack;
  UINT8                                           sampleCounter;
  UINT32                                          trainingDoneRecEn[MAX_CH];
  UINT32                                          trainingDoneTxDq[MAX_CH];
  UINT32                                          trainingDoneTxDqs[MAX_CH];
  UINT32                                          trainingDoneCmdN[MAX_CH][2];
  UINT32                                          trainingDoneCmdS[MAX_CH][2];
  UINT32                                          trainingDoneCtl[MAX_CH];
  UINT32                                          trainingDoneCke[MAX_CH];
  UINT32                                          trainingDoneClk[MAX_CH];
  UINT8                                           previousStatusRecEn[MAX_CH][MAX_STROBE];
  UINT8                                           previousStatusTxDq[MAX_CH][MAX_STROBE];
  UINT8                                           previousStatusTxDqs[MAX_CH][MAX_STROBE];
  UINT8                                           previousStatusCmdN[MAX_CH][2];
  UINT8                                           previousStatusCmdS[MAX_CH][2];
  UINT8                                           previousStatusCtl[MAX_CH];
  UINT8                                           previousStatusCke[MAX_CH];
  UINT8                                           previousStatusClk[MAX_CH];
  UINT16                                          offset[MAX_CH][MAX_STROBE];
  UINT16                                          offsetCmdN[MAX_CH][2];
  UINT16                                          offsetCmdS[MAX_CH][2];
  UINT16                                          offsetCtl[MAX_CH];
  UINT16                                          offsetCke[MAX_CH];
  UINT16                                          offsetClk[MAX_CH];
  UINT8                                           cmdLoop;
  UINT8                                           cmdLoops;
  UINT8                                           cmdnCounter[2];
  UINT8                                           cmdsCounter[2];
  UINT8                                           ckeCounter;
  UINT8                                           ctlCounter;
  UINT8                                           clkCounter;
  UINT8                                           cmdnResult[2];
  UINT8                                           cmdsResult[2];
  UINT8                                           ckeResult;
  UINT8                                           ctlResult;
  UINT8                                           clkResult;
  UINT8                                           recEnCntr[MAX_STROBE];
  UINT8                                           txDqsCntr[MAX_STROBE];
  UINT8                                           txDqCntr[MAX_STROBE];
  UINT8                                           recEnResult[MAX_STROBE];
  UINT8                                           txDqsResult[MAX_STROBE];
  UINT8                                           txDqResult[MAX_STROBE];
  UINT32                                          TxDqs2to2Offset;
  UINT32                                          TxDq2to2Offset;
  UINT32                                          RcvEn2to2Offset;
  UINT32                                          CmdOffset;
  UINT32                                          CtlOffset;
  UINT32                                          ClkOffset;
  UINT32                                          TxPerBitSetup2to2Offset;
  UINT32                                          xover1to1Offset;
  struct channelNvram                             (*channelNvList)[MAX_CH];
  DATACONTROL0_0_MCIO_DDRIO_STRUCT                  dataControl0;
  DATAOFFSETTRAIN_0_MCIO_DDRIO_STRUCT               dataOffsetTrain;
  DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_STRUCT        ddrCRCmdTraining;
  DDRCRCLKTRAINING_MCIO_DDRIOEXT_STRUCT             ddrCRClkTraining;
  DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_BDX_STRUCT        ddrCRClkRanksUsed;
  DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_BDX_STRUCT    ddrCRCtlControlsCkeRanksUsed;
  DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_BDX_STRUCT    ddrCRCtlControlsRanksUsed;
  DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_HSX_STRUCT    ddrCRCmdControlsCmdS;
  DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_HSX_STRUCT    ddrCRCmdControlsCmdN;
  DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_HSX_STRUCT     ddrCRCtlControls;
  DDRCRCTLTRAINING_CKE_MCIO_DDRIOEXT_HSX_STRUCT     ddrCRCkeTraining;
  DDRCRCTLTRAINING_CTL_MCIO_DDRIOEXT_HSX_STRUCT     ddrCRCtlTraining;
  DDRCRCLKCONTROLS_MCIO_DDRIOEXT_HSX_STRUCT         ddrCRClkControls;
  DDRCRCMDCONTROLS2_CMDN_MCIO_DDRIOEXT_HSX_STRUCT   ddrCRCmdControls2CmdN;
  DDRCRCMDCONTROLS2_CMDS_MCIO_DDRIOEXT_HSX_STRUCT   ddrCRCmdControls2CmdS;
  DATACONTROL4_0_MCIO_DDRIO_HSX_STRUCT              dataControl4[MAX_STROBE/2];

  if ((~host->memFlows & MF_X_OVER_EN) || ((host->setup.mem.optionsExt & XOVER_EN) == 0)) return SUCCESS;

  status = SUCCESS;

#ifdef SERIAL_DBG_MSG
  getPrintFControl(host);
#endif // SERIAL_DBG_MSG

  socket = host->var.mem.currentSocket;

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  dataControl0.Data = 0;

  dataOffsetTrain.Data = 0;

  if (host->var.common.cpuType == CPU_HSX) {
    TxDqs2to2Offset = X_OVER_OFFSET_2TO2_TXDQS_HSX;
    RcvEn2to2Offset = X_OVER_OFFSET_2TO2_RCVEN_HSX;
    TxDq2to2Offset = X_OVER_OFFSET_2TO2_TXDQ_HSX;
    TxPerBitSetup2to2Offset = 36;
    CmdOffset = X_OVER_OFFSET_2TO2_CMD_HSX;
    CtlOffset = X_OVER_OFFSET_2TO2_CTL_HSX;
    ClkOffset = X_OVER_OFFSET_2TO2_CLK_HSX;
  } else {
    if (host->nvram.mem.socket[socket].ddrFreq >= DDR_2400) {
      TxDqs2to2Offset = X_OVER_OFFSET_2TO2_TXDQS;
      RcvEn2to2Offset = X_OVER_OFFSET_2TO2_RCVEN;
    } else {
      TxDqs2to2Offset = X_OVER_OFFSET_2TO2_TXDQS_DDR2133;
      RcvEn2to2Offset = X_OVER_OFFSET_2TO2_RCVEN_DDR2133;
    }
    TxDq2to2Offset = X_OVER_OFFSET_2TO2_TXDQ;
    TxPerBitSetup2to2Offset = 36;
    CmdOffset = X_OVER_OFFSET_2TO2_CMD;
    CtlOffset = X_OVER_OFFSET_2TO2_CTL;
    ClkOffset = X_OVER_OFFSET_2TO2_CLK;
  }

  if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)){

    dataControl0.Bits.xovercal = 1;

    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "Enable xovercal\n"));

    if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
      MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Enabling xover 2:2 mode\n"));
    } else {
      MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Enabling xover 1:1 mode\n"));
    }

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      ddrCRCmdControls2CmdN.Data        = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS2_CMDN_MCIO_DDRIOEXT_REG);
      ddrCRCmdControls2CmdS.Data        = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS2_CMDS_MCIO_DDRIOEXT_REG);
      ddrCRCtlControlsCkeRanksUsed.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_REG);
      ddrCRCtlControlsRanksUsed.Data    = MemReadPciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_REG);
      ddrCRClkRanksUsed.Data            = MemReadPciCfgEp (host, socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG);

      ddrCRCmdControlsCmdN.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG);
      ddrCRCmdControlsCmdN.Bits.xovercal = 1;
      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        ddrCRCmdControlsCmdN.Bits.pirefclkdivenable = 1;
        ddrCRCmdControlsCmdN.Bits.gqclkdivenable = 1;
      } else {
        ddrCRCmdControlsCmdN.Bits.pirefclkdivenable = 0;
        ddrCRCmdControlsCmdN.Bits.gqclkdivenable = 0;
      }
      MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdControlsCmdN.Data);

      ddrCRCmdControlsCmdS.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG);
      ddrCRCmdControlsCmdS.Bits.xovercal = 1;
      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        ddrCRCmdControlsCmdS.Bits.pirefclkdivenable = 1;
        ddrCRCmdControlsCmdS.Bits.gqclkdivenable = 1;
      } else {
        ddrCRCmdControlsCmdS.Bits.pirefclkdivenable = 0;
        ddrCRCmdControlsCmdS.Bits.gqclkdivenable = 0;
      }
      MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControlsCmdS.Data);

      ddrCRCtlControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG);
      ddrCRCtlControls.Bits.xovercal = 1;
      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        ddrCRCtlControls.Bits.pirefclkdivenable = 1;
        ddrCRCtlControls.Bits.gqclkdivenable = 1;
      } else {
        ddrCRCtlControls.Bits.pirefclkdivenable = 0;
        ddrCRCtlControls.Bits.gqclkdivenable = 0;
      }
      MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlControls.Data);

      ddrCRCtlControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_REG);
      ddrCRCtlControls.Bits.xovercal = 1;
      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        ddrCRCtlControls.Bits.pirefclkdivenable = 1;
        ddrCRCtlControls.Bits.gqclkdivenable = 1;
      } else {
        ddrCRCtlControls.Bits.pirefclkdivenable = 0;
        ddrCRCtlControls.Bits.gqclkdivenable = 0;
      }
      MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlControls.Data);

      ddrCRClkControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG);
      ddrCRClkControls.Bits.xovercal = 1;
      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        ddrCRClkControls.Bits.pirefclkdivenable = 1;
        ddrCRClkControls.Bits.gqclkdivenable = 1;
      } else {
        ddrCRClkControls.Bits.pirefclkdivenable = 0;
        ddrCRClkControls.Bits.gqclkdivenable = 0;
      }
      MemWritePciCfgEp (host, socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG, ddrCRClkControls.Data);

      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
          dataControl4[strobe].Data = (*channelNvList)[ch].dataControl4[strobe];
          dataControl4[strobe].Bits.pirefclkdivenable = 1;
          dataControl4[strobe].Bits.gqclkdivenable = 1;
          (*channelNvList)[ch].dataControl4[strobe] = dataControl4[strobe].Data;
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL4_0_MCIO_DDRIO_REG), dataControl4[strobe].Data);
        } // strobe loop
      } // if 2:2

      trainingDoneRecEn[ch] = 0;
      trainingDoneTxDq[ch] = 0;
      trainingDoneTxDqs[ch] = 0;
      trainingDoneCmdN[ch][0] = 0;
      trainingDoneCmdS[ch][0] = 0;
      trainingDoneCmdN[ch][1] = 0;
      trainingDoneCmdS[ch][1] = 0;
      trainingDoneCtl[ch] = 0;
      trainingDoneCke[ch] = 0;
      trainingDoneClk[ch] = 0;

      offsetCmdN[ch][0] = 0;
      offsetCmdS[ch][0] = 0;
      offsetCmdN[ch][1] = 0;
      offsetCmdS[ch][1] = 0;
      offsetCtl[ch] = 0;
      offsetCke[ch] = 0;
      offsetClk[ch] = 0;

      previousStatusCmdN[ch][0] = 0;
      previousStatusCmdS[ch][0] = 0;
      previousStatusCmdN[ch][1] = 0;
      previousStatusCmdS[ch][1] = 0;
      previousStatusClk[ch] = 0;
      previousStatusCtl[ch] = 0;
      previousStatusCke[ch] = 0;

      for (strobe = 0; strobe < MAX_STROBE; strobe++) {
        previousStatusRecEn[ch][strobe] = 0;
        previousStatusTxDq[ch][strobe] = 0;
        previousStatusTxDqs[ch][strobe] = 0;
        offset[ch][strobe] = 0;
      } // strobe loop

      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2){
        xoverLoops = 2;
      } else {
        xoverLoops = 1;
      }

      if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
        cmdLoops = 1;
      } else {
        cmdLoops = 2;
      }

      ddrCRCmdTraining.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG);
      ddrCRCtlTraining.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLTRAINING_CTL_MCIO_DDRIOEXT_REG);
      ddrCRCkeTraining.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLTRAINING_CKE_MCIO_DDRIOEXT_REG);
      ddrCRClkTraining.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG);

      for (sweepPi = 0; sweepPi < xoverLoops; sweepPi++) {
        //OutputCheckpoint (host, STS_CHANNEL_TRAINING, 0x58, 0);

        if (sweepPi == 1) {
          //
          // If in xover2:2 mode and in second loop (64-127 pi ticks)
          //

          //
          // If Clk xover training was not done in the first pi sweep loop
          //
          MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "Edge not found in first 0-63. Setting invert pi clk for second sweep\n"));
          MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "InvertPiClk  CLK  CMDn0  CMDn1  CMDs0  CMDs1  CKE  CTL  0  1  2  3  4  5  6  7  8\n"));
          if (!trainingDoneClk[ch]) {
            ddrCRClkRanksUsed.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG);
            ddrCRClkRanksUsed.Bits.ddrcrinvertpiclk = 0xF;
            MemWritePciCfgEp (host, socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, ddrCRClkRanksUsed.Data);
            offsetClk[ch] = 0;
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "                    %2d", 1));
          } else {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "                    %2d", 0));
          }

          ddrCRCmdControls2CmdN.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS2_CMDN_MCIO_DDRIOEXT_REG);
          if (!trainingDoneCmdN[ch][0]) {
            ddrCRCmdControls2CmdN.Bits.ddrcrinvertpiclk = ddrCRCmdControls2CmdN.Bits.ddrcrinvertpiclk | BIT0 | BIT1 | BIT6 | BIT7 | BIT8 | BIT11;
            offsetCmdN[ch][0] = 0;
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "    %2d", 1));
          } else {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "    %2d", 0));
          }

          if (!trainingDoneCmdN[ch][1]) {
            ddrCRCmdControls2CmdN.Bits.ddrcrinvertpiclk = ddrCRCmdControls2CmdN.Bits.ddrcrinvertpiclk | BIT2 | BIT3 | BIT4 | BIT5 | BIT9 | BIT10;
            offsetCmdN[ch][1] = 0;
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "     %2d", 1));
          } else {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "     %2d", 0));
          }

          MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS2_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdControls2CmdN.Data);

          ddrCRCmdControls2CmdS.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS2_CMDS_MCIO_DDRIOEXT_REG);

          if (!trainingDoneCmdS[ch][0]) {
            ddrCRCmdControls2CmdS.Bits.ddrcrinvertpiclk = ddrCRCmdControls2CmdS.Bits.ddrcrinvertpiclk | BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT8;
            offsetCmdS[ch][0] = 0;
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "     %2d", 1));
          } else {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "     %2d", 0));
          }

          if (!trainingDoneCmdS[ch][1]) {
            ddrCRCmdControls2CmdS.Bits.ddrcrinvertpiclk = ddrCRCmdControls2CmdS.Bits.ddrcrinvertpiclk | BIT5 | BIT6 | BIT7 | BIT9 | BIT10 | BIT11;
            offsetCmdS[ch][1] = 0;
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "     %2d", 1));
          } else {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "     %2d", 0));
          }

          MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS2_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControls2CmdS.Data);

          if (!trainingDoneCke[ch]) {
            ddrCRCtlControlsCkeRanksUsed.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_REG);
            ddrCRCtlControlsCkeRanksUsed.Bits.ddrcrinvertpiclk = 0xFFF;
            MemWritePciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlControlsCkeRanksUsed.Data);
            offsetCke[ch] = 0;
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "    %2d", 1));
          } else {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "    %2d", 0));
          }

          if (!trainingDoneCtl[ch]) {
            ddrCRCtlControlsRanksUsed.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_REG);
            ddrCRCtlControlsRanksUsed.Bits.ddrcrinvertpiclk = 0xFFF;
            MemWritePciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlControlsRanksUsed.Data);
            offsetCtl[ch] = 0;
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "   %2d  ", 1));
          } else {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "   %2d  ", 0));
          }
          for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
            if (((trainingDoneRecEn[ch] >> strobe) & 1) == 0) {
              dataControl4[strobe].Data = (*channelNvList)[ch].dataControl4[strobe];
              dataControl4[strobe].Bits.ddrcrinvertpiclk = 3;
              (*channelNvList)[ch].dataControl4[strobe] = dataControl4[strobe].Data;
              MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL4_0_MCIO_DDRIO_REG), dataControl4[strobe].Data);
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "%2d ", 1));
            } else {
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "%2d ", 0));
            }
          }
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "\n"));

        } // If sweepPi == 1

        if (cmdLoops == 1) {
          MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "piDelay CMDn  CMDs  CTL  CKE  CLK   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17\n"));
        } else {
          MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "piDelay CMDn0 CMDn1 CMDs0 CMDs1 CTL  CKE  CLK   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17\n"));
        }

        for (piDelay = X_OVER_PI_START; piDelay < X_OVER_MAX_SAMPLE; piDelay += X_OVER_STEP_SIZE) {

          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "         %2d     ", piDelay));

          cmdnCounter[0] = 0;
          cmdsCounter[0] = 0;
          cmdnCounter[1] = 0;
          cmdsCounter[1] = 0;
          ckeCounter = 0;
          ctlCounter = 0;
          clkCounter = 0;

          for (strobe = 0; strobe < MAX_STROBE; strobe++){
            recEnCntr[strobe]  = 0;
            txDqsCntr[strobe] = 0;
            txDqCntr[strobe] = 0;
          }

          //
          //If CMDN[0] channels 0 and 2, or if CMDN[1] channel 1 and 3, apply init_value = 32
          //
          if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
            if ((ch % 2) == 0) {
              ddrCRCmdTraining.Bits.trainingoffset = (piDelay + X_OVER_ODD_FUB_OFFSET) % 64;
              ddrCRCmdTraining.Bits.trainingoffset2 = piDelay;
            } else if ((ch % 2) == 1) {
              ddrCRCmdTraining.Bits.trainingoffset2 = (piDelay + X_OVER_ODD_FUB_OFFSET) % 64;
              ddrCRCmdTraining.Bits.trainingoffset = piDelay;
            }
          } else {
            ddrCRCmdTraining.Bits.trainingoffset = piDelay;
            if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX) == 0) {
              ddrCRCmdTraining.Bits.trainingoffset2 = piDelay;
            }
          }
          MemWritePciCfgEp (host, socket, ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdTraining.Data);

          //
          //If CMDS[0] or CMDS[1] channels 1 and 3, apply init_value = 32
          //
          if ((host->var.mem.xoverModeVar == XOVER_MODE_2TO2) && ((ch % 2) == 1)) {
            ddrCRCmdTraining.Bits.trainingoffset = (piDelay + X_OVER_ODD_FUB_OFFSET) % 64;
            ddrCRCmdTraining.Bits.trainingoffset2 = (piDelay + X_OVER_ODD_FUB_OFFSET) % 64;
          } else {
            ddrCRCmdTraining.Bits.trainingoffset = piDelay;
            if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX) == 0) {
              ddrCRCmdTraining.Bits.trainingoffset2 = piDelay;
            }
          }
          MemWritePciCfgEp (host, socket, ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdTraining.Data);

          //
          //If CTL channels 1 and 3, apply init_value = 32
          //
          if ((host->var.mem.xoverModeVar == XOVER_MODE_2TO2) && ((ch % 2) == 1)) {
            ddrCRCtlTraining.Bits.trainingoffset = (piDelay + X_OVER_ODD_FUB_OFFSET) % 64;
          } else {
            ddrCRCtlTraining.Bits.trainingoffset = piDelay;
          }
          // Workaround HSD4166180: This field returns wrong data when read so it neds to be hardcoded (Remove for B0)
          if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX) == 0) {
            ddrCRCtlTraining.Bits.ctldrvsegen    = 0;
          }
          MemWritePciCfgEp (host, socket, ch, DDRCRCTLTRAINING_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlTraining.Data);

          //
          //If CKE channels 0 and 2, apply init_value = 32
          //
          if ((host->var.mem.xoverModeVar == XOVER_MODE_2TO2) && ((ch % 2) == 0)) {
            ddrCRCkeTraining.Bits.trainingoffset = (piDelay + X_OVER_ODD_FUB_OFFSET) % 64;
          } else {
            ddrCRCkeTraining.Bits.trainingoffset = piDelay;
          }
          MemWritePciCfgEp (host, socket, ch, DDRCRCTLTRAINING_CKE_MCIO_DDRIOEXT_REG, ddrCRCkeTraining.Data);

          //
          //If CLK channels 1 and 3, apply init_value = 32
          //
          if ((host->var.mem.xoverModeVar == XOVER_MODE_2TO2) && ((ch % 2) == 1)) {
            ddrCRClkTraining.Bits.trainingoffset = (piDelay + X_OVER_ODD_FUB_OFFSET) % 64;
          } else {
            ddrCRClkTraining.Bits.trainingoffset = piDelay;
          }
          MemWritePciCfgEp (host, socket, ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG, ddrCRClkTraining.Data);

          for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
            if ((host->var.mem.xoverModeVar == XOVER_MODE_2TO2) && (((ch % 2 == 0) && ((strobe & 1) == 0)) || ((ch % 2 == 1) && ((strobe & 1) == 1)))) {
              dataOffsetTrain.Bits.rcvenoffset = (piDelay + X_OVER_ODD_FUB_OFFSET) % 64;
              dataOffsetTrain.Bits.txdqoffset = (piDelay + X_OVER_ODD_FUB_OFFSET) % 64;
              dataOffsetTrain.Bits.txdqsoffset = (piDelay + X_OVER_ODD_FUB_OFFSET) % 64;
            } else {
              dataOffsetTrain.Bits.rcvenoffset = piDelay;
              dataOffsetTrain.Bits.txdqoffset = piDelay;
              dataOffsetTrain.Bits.txdqsoffset = piDelay;
            }
            MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATAOFFSETTRAIN_0_MCIO_DDRIO_REG), dataOffsetTrain.Data);

            //
            // Reload the delay
            //
            dataControl0.Bits.readrfrd = 1;
            dataControl0.Bits.readrfwr = 0;
            MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data | (*channelNvList)[ch].dataControl0);
            dataControl0.Bits.readrfrd = 0;
            dataControl0.Bits.readrfwr = 1;
            MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data | (*channelNvList)[ch].dataControl0);
          } // strobe loop

          // HSD4165676: Multiple sample during crossover calibration
          for (sampleCounter = 0; sampleCounter < XOVER_CALIB_SAMPLE_COUNT; sampleCounter++){

            //
            // Starts the test
            //
            IO_Reset(host, socket);

            FixedDelay(host, 1);

            ddrCRCmdTraining.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG);
            for (cmdLoop = 0; cmdLoop < cmdLoops; cmdLoop++) {
              if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
                if (ddrCRCmdTraining.Bits.xoverphasedet){
                  cmdnCounter[0]++;
                }
              } else {
                if (cmdLoop == 0) {
                  if (ddrCRCmdTraining.Bits.xoverphasedet & BIT0){
                    cmdnCounter[cmdLoop]++;
                  }
                } else if (cmdLoop == 1) {
                  if (ddrCRCmdTraining.Bits.xoverphasedet & BIT5){
                    cmdnCounter[cmdLoop]++;
                  }
                }
              }
            } // cmdLoop

            ddrCRCmdTraining.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG);
            for (cmdLoop = 0; cmdLoop < cmdLoops; cmdLoop++) {
              if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
                if (ddrCRCmdTraining.Bits.xoverphasedet){
                  cmdsCounter[0]++;
                }
              } else {
                if (cmdLoop == 0) {
                  if (ddrCRCmdTraining.Bits.xoverphasedet & BIT0){
                    cmdsCounter[cmdLoop]++;
                  }
                } else if (cmdLoop == 1) {
                  if (ddrCRCmdTraining.Bits.xoverphasedet & BIT5){
                    cmdsCounter[cmdLoop]++;
                  }
                }
              }
            } // cmdLoop

            ddrCRCtlTraining.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLTRAINING_CTL_MCIO_DDRIOEXT_REG);
            if (ddrCRCtlTraining.Bits.xoverphasedet){
              ctlCounter++;
            }

            ddrCRCkeTraining.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLTRAINING_CKE_MCIO_DDRIOEXT_REG);
            if (ddrCRCkeTraining.Bits.xoverphasedet){
              ckeCounter++;
            }

            ddrCRClkTraining.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG);
            if (ddrCRClkTraining.Bits.pclkxoverphasedet){
              clkCounter++;
            }

            for (strobe = 0; strobe < MAX_STROBE; strobe++) {
              feedBack = MemReadPciCfgEp (host, socket, ch, dataTrainFeedbackTable[strobe]);

              status = (feedBack >> 8) & 3;
              if (status){
                recEnCntr[strobe]++;
              }

              status = (feedBack >> 4) & 3;
              if (status){
                txDqsCntr[strobe]++;
              }

              status = (feedBack >> 2) & 3;
              if (status){
                txDqCntr[strobe]++;
              }
            } // strobe loop
          } // sampleCounter

          breakOut = 1;

          for (cmdLoop = 0; cmdLoop < cmdLoops; cmdLoop++) {
            if ((cmdnCounter[cmdLoop] * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)){
              cmdnResult[cmdLoop] = 1;
            } else {
              cmdnResult[cmdLoop] = 0;
            }
            if ((cmdsCounter[cmdLoop] * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)){
              cmdsResult[cmdLoop] = 1;
            } else {
              cmdsResult[cmdLoop]= 0;
            }
          } // cmdLoop

          if ((ckeCounter * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)){
            ckeResult = 1;
          } else {
            ckeResult = 0;
          }

          if ((ctlCounter * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)){
            ctlResult = 1;
          } else {
            ctlResult = 0;
          }

          if ((clkCounter * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)){
            clkResult = 1;
          } else {
            clkResult = 0;
          }

          for (strobe = 0; strobe < MAX_STROBE; strobe++){

            if ((recEnCntr[strobe] * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)){
              recEnResult[strobe] = 1;
            } else {
              recEnResult[strobe] = 0;
            }

            if ((txDqsCntr[strobe] * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)){
              txDqsResult[strobe] = 1;
            } else {
              txDqsResult[strobe] = 0;
            }

            if ((txDqCntr[strobe] * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)){
              txDqResult[strobe] = 1;
            } else {
              txDqResult[strobe] = 0;
            }
          } // strobe loop

          for (cmdLoop = 0; cmdLoop < cmdLoops; cmdLoop++) {
            if (trainingDoneCmdN[ch][cmdLoop] == 0) {

              if (cmdnResult[cmdLoop]) {
                MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                              "1     "));
              } else {
                MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                              "0     "));
              }

              if ((previousStatusCmdN[ch][cmdLoop] != 0) & (cmdnResult[cmdLoop] == 0)) {
                if (piDelay > offsetCmdN[ch][cmdLoop]) {
                  offsetCmdN[ch][cmdLoop] = piDelay;
                }
                trainingDoneCmdN[ch][cmdLoop] = 1;
                //MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                //              "CMD-N xovercal offset = 0x%x\n", piDelay));
              } else {
                breakOut = 0;
                previousStatusCmdN[ch][cmdLoop] = cmdnResult[cmdLoop];
              }
            } else {
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "X     "));
            }

            if (trainingDoneCmdS[ch][cmdLoop] == 0) {

              if (cmdsResult[cmdLoop]) {
                MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                              "1     "));
              } else {
                MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                              "0     "));
              }

              if ((previousStatusCmdS[ch][cmdLoop] != 0) & (cmdsResult[cmdLoop] == 0)) {
                if (piDelay > offsetCmdS[ch][cmdLoop]) {
                  offsetCmdS[ch][cmdLoop] = piDelay;
                }
                trainingDoneCmdS[ch][cmdLoop] = 1;
                //MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                //              "CMD-S xovercal offset = 0x%x\n", piDelay));
              } else {
                breakOut = 0;
                previousStatusCmdS[ch][cmdLoop] = cmdsResult[cmdLoop];
              }
            } else {
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                            "X     "));
            }
          } // cmdLoop

          if (trainingDoneCtl[ch] == 0) {

            if (ctlResult) {
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                            "1    "));
            } else {
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                            "0    "));
            }

            if ((previousStatusCtl[ch] != 0) & (ctlResult == 0)) {
              if (piDelay > offsetCtl[ch]) {
                offsetCtl[ch] = piDelay;
              }
              trainingDoneCtl[ch] = 1;
              //MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              //              "CTL xovercal offset = 0x%x\n", piDelay));
            } else {
              breakOut = 0;
              previousStatusCtl[ch] = ctlResult;
            }
          } else {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "X    "));
          }

          if (trainingDoneCke[ch] == 0) {

            if (ckeResult) {
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                            "1    "));
            } else {
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                            "0    "));
            }

            if ((previousStatusCke[ch] != 0) & (ckeResult == 0)) {
              if (piDelay > offsetCke[ch]) {
                offsetCke[ch] = piDelay;
              }
              trainingDoneCke[ch] = 1;
              //MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              //              "CKE xovercal offset = 0x%x\n", piDelay));
            } else {
              breakOut = 0;
              previousStatusCke[ch] = ckeResult;
            }
          } else {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "X    "));
          }

          if (trainingDoneClk[ch] == 0) {

            if (clkResult) {
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                            "1    "));
            } else {
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                            "0    "));
            }

            if ((previousStatusClk[ch] != 0) & (clkResult == 0)) {
              if (piDelay > offsetClk[ch]) {
                offsetClk[ch] = piDelay;
              }
              trainingDoneClk[ch] = 1;
              //MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              //              "CLK xovercal offset = 0x%x\n", piDelay));
            } else {
              breakOut = 0;
              previousStatusClk[ch] = clkResult;
            }
          } else {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "X    "));
          }

          for (strobe = 0; strobe < MAX_STROBE; strobe++) {

            if((trainingDoneRecEn[ch] & (1 << strobe)) == 0){
              if (recEnResult[strobe]) {
                MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                              "1  "));
              } else {
                MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                              "0  "));
              }

              // Check if previous was one and current is 0
              if ((previousStatusRecEn[ch][strobe] & 3) && (recEnResult[strobe] == 0)) {
                if (piDelay > offset[ch][strobe]) {
                  offset[ch][strobe] = piDelay;
                }
                trainingDoneRecEn[ch] |= 1 << strobe;
                //MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, strobe, NO_BIT,
                //              "Rec En xovercal offset = 0x%x\n", piDelay));
              } else {
                breakOut = 0;
                previousStatusRecEn[ch][strobe] = recEnResult[strobe];
              }
            } else {
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                            "X  "));
            }

            if((trainingDoneTxDqs[ch] & (1 << strobe)) == 0){
              if ((previousStatusTxDqs[ch][strobe] & 3) && (txDqsResult[strobe]== 0)) {
                if (piDelay > offset[ch][strobe]) {
                  offset[ch][strobe] = piDelay;
                }
                trainingDoneTxDqs[ch] |= 1 << strobe;
                //MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, strobe, NO_BIT,
                //              "Tx Dqs xovercal offset = 0x%x\n", piDelay));
              } else {
                breakOut = 0;
                previousStatusTxDqs[ch][strobe] = txDqsResult[strobe];
              }
            }

            if((trainingDoneTxDq[ch] & (1 << strobe)) == 0){
              if ((previousStatusTxDq[ch][strobe] & 3) && (txDqResult[strobe] == 0)) {
                if (piDelay > offset[ch][strobe]) {
                  offset[ch][strobe] = piDelay;
                }
                trainingDoneTxDq[ch] |= 1 << strobe;
                //MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, strobe, NO_BIT,
                //              "Tx Dq xovercal offset = 0x%x\n", piDelay));
              } else {
                breakOut = 0;
                previousStatusTxDq[ch][strobe] = txDqResult[strobe];
              }
            }

          } // strobe loop

          //
          // Break out of the piDelay loop if we have found all the 1 to 0 transitions
          //
          if(breakOut) {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "\nbreakOut set!\n"));
            break;
          }
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "\n"));
        } // piDelay loop
      } // sweepPi loop

      //
      // Program offsets
      //
      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "CMDn0  CMDn1  CMDs0  CMDs1  CTL  CKE  CLK   0  1  2  3  4  5  6  7  8\n"));
        //Printing CMDN offset
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "         %2d", offsetCmdN[ch][0]));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "     %2d", offsetCmdN[ch][1]));
        //Printing CMDS offset
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "     %2d", offsetCmdS[ch][0]));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                         "     %2d", offsetCmdS[ch][1]));
        //Printing CTL offset
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "    %2d", offsetCtl[ch]));
        //Printing CKE offset
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "   %2d", offsetCke[ch]));
        //Printing CLK offset
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "   %2d  ", offsetClk[ch]));
        //Printing Data offset
        for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          " %2d", MaxVal(offset[ch][strobe], offset[ch][strobe + 9])));
        }
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "\n"));


        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "Adding +%d to CMD, Adding +%d to CTL and reevaluating Invert Pi Clk. Adding +%d to CLK and reevaluating Invert Pi Clk. \nAdding +%d to TxDq and reevaluating InvertPiClk. \nAdding +%d to RcvEn. Adding +%d to TxDqs. \nAdding +%d to odd fubs. \nAdding +%d to TxPerBitDeskew.\n", (UINT32)CmdOffset, (UINT32)CtlOffset, (UINT32)ClkOffset, (UINT16)TxDq2to2Offset, (UINT16)RcvEn2to2Offset, (UINT16)TxDqs2to2Offset, X_OVER_ODD_FUB_OFFSET, (UINT16)TxPerBitSetup2to2Offset));

        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "InvertPiClk  CMDn0  CMDn1  CMDs0  CMDs1  CKE  CTL  CLK   0  1  2  3  4  5  6  7  8\n"));

        if (((offsetCmdN[ch][0] + (UINT16)CmdOffset)/64)%2) {
          ddrCRCmdControls2CmdN.Bits.ddrcrinvertpiclk ^= (BIT0 | BIT1 | BIT6 | BIT7 | BIT8 | BIT11);
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "                      y", 1));
        } else {
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "                      n", 0));
        }
        if (((offsetCmdN[ch][1] + (UINT16)CmdOffset)/64)%2) {
          ddrCRCmdControls2CmdN.Bits.ddrcrinvertpiclk ^= (BIT2 | BIT3 | BIT4 | BIT5 | BIT9 | BIT10);
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "      y", 1));
        } else {
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "      n", 0));
        }
        if (((offsetCmdS[ch][0] + (UINT16)CmdOffset)/64)%2) {
          ddrCRCmdControls2CmdS.Bits.ddrcrinvertpiclk ^= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT8);
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "      y", 1));
        } else {
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "      n", 0));
        }
        if (((offsetCmdS[ch][1] + (UINT16)CmdOffset)/64)%2) {
          ddrCRCmdControls2CmdS.Bits.ddrcrinvertpiclk ^= (BIT5 | BIT6 | BIT7 | BIT9 | BIT10 | BIT11);
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "      y", 1));
        } else {
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "      n", 0));
        }
        if (((offsetCtl[ch] + (UINT16)CtlOffset)/64)%2) {
          ddrCRCtlControlsRanksUsed.Bits.ddrcrinvertpiclk ^= 0xFFF;
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "     y", 1));
        } else {
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "     n", 0));
        }
        if (((offsetCke[ch] + (UINT16)CtlOffset)/64)%2) {
          ddrCRCtlControlsCkeRanksUsed.Bits.ddrcrinvertpiclk ^= 0xFFF;
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "    y", 1));
        } else {
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "    n", 0));
        }
        if (((offsetClk[ch] + (UINT16)ClkOffset)/64)%2) {
          ddrCRClkRanksUsed.Bits.ddrcrinvertpiclk ^= 0xF;
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "    y", 1));
        } else {
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "    n", 0));
        }

        for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
          if (((MaxVal(offset[ch][strobe], offset[ch][strobe + 9]) + (UINT16)TxDq2to2Offset)/64)%2) {
            dataControl4[strobe].Bits.ddrcrinvertpiclk ^= 3;
            (*channelNvList)[ch].dataControl4[strobe] = dataControl4[strobe].Data;
            MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL4_0_MCIO_DDRIO_REG), dataControl4[strobe].Data);
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "  y", 1));
          } else {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "  n", 0));

          }
        }

        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "\n"));

        MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS2_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdControls2CmdN.Data);
        MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS2_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControls2CmdS.Data);
        MemWritePciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlControlsCkeRanksUsed.Data);
        MemWritePciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlControlsRanksUsed.Data);
        MemWritePciCfgEp (host, socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, ddrCRClkRanksUsed.Data);

        (*channelNvList)[ch].ddrCRCmdControls2CmdN = ddrCRCmdControls2CmdN.Data;
        (*channelNvList)[ch].ddrCRCmdControls2CmdS = ddrCRCmdControls2CmdS.Data;
        (*channelNvList)[ch].ddrCRCtlControlsCkeRanksUsed = ddrCRCtlControlsCkeRanksUsed.Data;
        (*channelNvList)[ch].ddrCRCtlControlsRanksUsed = ddrCRCtlControlsRanksUsed.Data;
        (*channelNvList)[ch].ddrCRClkRanksUsed = ddrCRClkRanksUsed.Data;

        offsetCmdN[ch][0] = (offsetCmdN[ch][0] + (UINT16)CmdOffset) % 64;
        offsetCmdS[ch][0] = (offsetCmdS[ch][0] + (UINT16)CmdOffset) % 64;
        offsetCmdN[ch][1] = (offsetCmdN[ch][1] + (UINT16)CmdOffset) % 64;
        offsetCmdS[ch][1] = (offsetCmdS[ch][1] + (UINT16)CmdOffset) % 64;
        offsetCtl[ch] = (offsetCtl[ch] + (UINT16)CtlOffset) % 64;
        offsetCke[ch] = (offsetCke[ch] + (UINT16)CtlOffset) % 64;
        offsetClk[ch] = (offsetClk[ch] + (UINT16)ClkOffset) % 64;

        //If CMDN[0] channels 0 and 2, apply init_value = 32
        //If CKE  channels 0 and 2, apply init_value = 32
        if ((ch % 2) == 0) {
          offsetCmdN[ch][0] = (offsetCmdN[ch][0] + X_OVER_ODD_FUB_OFFSET) % 64;
          offsetCke[ch] = (offsetCke[ch] + X_OVER_ODD_FUB_OFFSET) % 64;
        }
        //If CMDN[1] channels 1 and 3, apply init_value = 32
        //If CMDS channels 1 and 3, apply init_value = 32
        //If CTL channels 1 and 3, apply init_value = 32
        if ((ch % 2) == 1){
          offsetCmdS[ch][0]  = (offsetCmdS[ch][0] + X_OVER_ODD_FUB_OFFSET) % 64;
          offsetCmdN[ch][1]  = (offsetCmdN[ch][1] + X_OVER_ODD_FUB_OFFSET) % 64;
          offsetCmdS[ch][1]  = (offsetCmdS[ch][1] + X_OVER_ODD_FUB_OFFSET) % 64;
          offsetCtl[ch]      = (offsetCtl[ch] + X_OVER_ODD_FUB_OFFSET) % 64;
          offsetClk[ch]      = (offsetClk[ch] + X_OVER_ODD_FUB_OFFSET) % 64;
        }
      } else {
        if (host->var.common.cpuType == CPU_HSX) {
          xover1to1Offset = X_OVER_OFFSET_HSX;
        } else {
          xover1to1Offset = X_OVER_OFFSET_BDX;
        }
        offsetCmdN[ch][0] = (offsetCmdN[ch][0] + xover1to1Offset) % 64;
        offsetCmdS[ch][0] = (offsetCmdS[ch][0] + xover1to1Offset) % 64;
        offsetCmdN[ch][1] = (offsetCmdN[ch][1] + xover1to1Offset) % 64;
        offsetCmdS[ch][1] = (offsetCmdS[ch][1] + xover1to1Offset) % 64;
        offsetCtl[ch] = (offsetCtl[ch] + xover1to1Offset) % 64;
        offsetCke[ch] = (offsetCke[ch] + xover1to1Offset) % 64;
        offsetClk[ch] = (offsetClk[ch] + xover1to1Offset) % 64;
      }

      if (cmdLoops == 1) {
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "  CMDn  CMDs  CTL  CKE  CLK   0  1  2  3  4  5  6  7  8\n"));
      } else {
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "  CMDn0  CMDn1  CMDs0  CMDs1  CTL  CKE  CLK   0  1  2  3  4  5  6  7  8\n"));
      }

      //Printing CMDN offset
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "          %2d", offsetCmdN[ch][0]));
      if (cmdLoops == 2) {
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "      %2d", offsetCmdN[ch][1]));
      }
      //Getting and setting register for CMDN
      ddrCRCmdTraining.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG);
      ddrCRCmdTraining.Bits.trainingoffset = offsetCmdN[ch][0];
      if (cmdLoops == 2) {
        ddrCRCmdTraining.Bits.trainingoffset2 = offsetCmdN[ch][1];
      }
      MemWritePciCfgEp (host, socket, ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdTraining.Data);
      (*channelNvList)[ch].ddrCRCmdTrainingCmdN = ddrCRCmdTraining.Data;


      //Printing CMDS offset
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "    %2d", offsetCmdS[ch][0]));
      if (cmdLoops == 2) {
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "     %2d", offsetCmdS[ch][1]));
      }
      //Getting and setting register for CMDS
      ddrCRCmdTraining.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG);
      ddrCRCmdTraining.Bits.trainingoffset = offsetCmdS[ch][0];
      if (cmdLoops == 2) {
        ddrCRCmdTraining.Bits.trainingoffset2 = offsetCmdS[ch][1];
      }
      MemWritePciCfgEp (host, socket, ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdTraining.Data);
      (*channelNvList)[ch].ddrCRCmdTrainingCmdS = ddrCRCmdTraining.Data;

      //Printing CTL offset
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "    %2d", offsetCtl[ch]));
      //Getting and setting register for CTL
      ddrCRCtlTraining.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLTRAINING_CTL_MCIO_DDRIOEXT_REG);
      ddrCRCtlTraining.Bits.trainingoffset = offsetCtl[ch];
      // Workaround HSD4166180: This field returns wrong data when read so it neds to be hardcoded (Remove for B0)
      if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX) == 0) {
        ddrCRCtlTraining.Bits.ctldrvsegen    = 0;
      }
      MemWritePciCfgEp (host, socket, ch, DDRCRCTLTRAINING_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlTraining.Data);
      (*channelNvList)[ch].ddrCRCtlTraining = ddrCRCtlTraining.Data;


      //Printing CKE offset
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "   %2d", offsetCke[ch]));
      //Getting and setting register for CKE
      ddrCRCkeTraining.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLTRAINING_CKE_MCIO_DDRIOEXT_REG);
      ddrCRCkeTraining.Bits.trainingoffset = offsetCke[ch];
      MemWritePciCfgEp (host, socket, ch, DDRCRCTLTRAINING_CKE_MCIO_DDRIOEXT_REG, ddrCRCkeTraining.Data);
      (*channelNvList)[ch].ddrCRCkeTraining = ddrCRCkeTraining.Data;

      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "    %2d ", offsetClk[ch]));
      ddrCRClkTraining.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG);
      ddrCRClkTraining.Bits.trainingoffset = offsetClk[ch];
      MemWritePciCfgEp (host, socket, ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG, ddrCRClkTraining.Data);
      (*channelNvList)[ch].ddrCRClkTraining = ddrCRClkTraining.Data;

      for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
        dataOffsetTrain.Data = 0;
        if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
          dataOffsetTrain.Bits.txdqoffset  = (MaxVal(offset[ch][strobe], offset[ch][strobe + 9]) + (UINT16)TxDq2to2Offset) % 64;
          if (((ch % 2 == 0) && ((strobe & 1) == 0)) || ((ch % 2 == 1) && ((strobe & 1) == 1))) {
            dataOffsetTrain.Bits.txdqoffset  = ((UINT8)dataOffsetTrain.Bits.txdqoffset + X_OVER_ODD_FUB_OFFSET) % 64;
          }
          dataOffsetTrain.Bits.rcvenoffset = (dataOffsetTrain.Bits.txdqoffset + (UINT16)RcvEn2to2Offset) % 64;
          dataOffsetTrain.Bits.txdqsoffset = (dataOffsetTrain.Bits.txdqoffset + (UINT16)TxDqs2to2Offset) % 64;
        } else {
          if (host->var.common.cpuType == CPU_HSX) {
            xover1to1Offset = X_OVER_OFFSET_HSX;
          } else {
            xover1to1Offset = X_OVER_OFFSET_BDX;
          }
          dataOffsetTrain.Bits.rcvenoffset = (MaxVal(offset[ch][strobe], offset[ch][strobe + 9]) + xover1to1Offset) % 64;
          dataOffsetTrain.Bits.txdqoffset = dataOffsetTrain.Bits.rcvenoffset;
          dataOffsetTrain.Bits.txdqsoffset = dataOffsetTrain.Bits.rcvenoffset;
        }
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      " %2d", dataOffsetTrain.Bits.txdqoffset));
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATAOFFSETTRAIN_0_MCIO_DDRIO_REG), dataOffsetTrain.Data);
        (*channelNvList)[ch].dataOffsetTrain[strobe] = dataOffsetTrain.Data;
        (*channelNvList)[ch].dataOffsetTrain[strobe + 9] = dataOffsetTrain.Data;

        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), (*channelNvList)[ch].dataControl0);

        //
        // Reload the delay
        //
        dataControl0.Bits.readrfrd = 1;
        dataControl0.Bits.readrfwr = 0;
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data | (*channelNvList)[ch].dataControl0);
        dataControl0.Bits.readrfrd = 0;
        dataControl0.Bits.readrfwr = 1;
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data | (*channelNvList)[ch].dataControl0);
      } // strobe loop

      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "\n"));

      IO_Reset(host, socket);

      //
      // Disable xover training mode
      //
      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        ddrCRCmdControlsCmdN.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG);
        ddrCRCmdControlsCmdN.Bits.txon = 0;
        MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdControlsCmdN.Data);

        ddrCRCmdControlsCmdS.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG);
        ddrCRCmdControlsCmdS.Bits.txon = 0;
        MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControlsCmdS.Data);

        ddrCRCtlControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG);
        ddrCRCtlControls.Bits.txon = 1;
        MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlControls.Data);
        MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlControls.Data);

        ddrCRClkControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG);
        ddrCRClkControls.Bits.xovercal = 0;
        (*channelNvList)[ch].ddrCRClkControls = ddrCRClkControls.Data;
        MemWritePciCfgEp (host, socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRClkControls);

      } else {
        ddrCRClkControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG);
        ddrCRClkControls.Bits.xovercal = 0;
        MemWritePciCfgEp (host, socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG, ddrCRClkControls.Data);

        ddrCRCmdControlsCmdN.Bits.xovercal = 0;
        MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdControlsCmdN.Data);
        ddrCRCmdControlsCmdS.Bits.xovercal = 0;
        MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControlsCmdS.Data);

        ddrCRCtlControls.Bits.xovercal = 0;
        ddrCRCtlControls.Bits.txon = 1;
        MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlControls.Data);
        MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlControls.Data);

        MemWritePciCfgEp (host, socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRClkControls);
      }
    } // ch loop
  } else if ((host->var.common.bootMode == S3Resume) || (host->var.mem.subBootMode == WarmBootFast) || (host->var.mem.subBootMode == ColdBootFast)){

    if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {

      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;

        dataControl0.Bits.xovercal = 1;

        ddrCRCmdControlsCmdN.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG);
        ddrCRCmdControlsCmdN.Bits.xovercal = 1;
        ddrCRCmdControlsCmdN.Bits.pirefclkdivenable = 1;
        ddrCRCmdControlsCmdN.Bits.gqclkdivenable = 1;
        MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdControlsCmdN.Data);

        ddrCRCmdControlsCmdS.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG);
        ddrCRCmdControlsCmdS.Bits.xovercal = 1;
        ddrCRCmdControlsCmdS.Bits.pirefclkdivenable = 1;
        ddrCRCmdControlsCmdS.Bits.gqclkdivenable = 1;
        MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControlsCmdS.Data);

        ddrCRCtlControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG);
        ddrCRCtlControls.Bits.xovercal = 1;
        ddrCRCtlControls.Bits.pirefclkdivenable = 1;
        ddrCRCtlControls.Bits.gqclkdivenable = 1;
        ddrCRCtlControls.Bits.txon = 1;
        MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlControls.Data);

        ddrCRCtlControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_REG);
        ddrCRCtlControls.Bits.xovercal = 1;
        ddrCRCtlControls.Bits.pirefclkdivenable = 1;
        ddrCRCtlControls.Bits.gqclkdivenable = 1;
        ddrCRCtlControls.Bits.txon = 1;
        MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlControls.Data);

        ddrCRClkControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG);
        ddrCRClkControls.Bits.xovercal = 1;
        ddrCRClkControls.Bits.pirefclkdivenable = 1;
        ddrCRClkControls.Bits.gqclkdivenable = 1;
        MemWritePciCfgEp (host, socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG, ddrCRClkControls.Data);

        for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
          dataControl4[strobe].Data = (*channelNvList)[ch].dataControl4[strobe];
          dataControl4[strobe].Bits.pirefclkdivenable = 1;
          dataControl4[strobe].Bits.gqclkdivenable = 1;
          (*channelNvList)[ch].dataControl4[strobe] = dataControl4[strobe].Data;
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL4_0_MCIO_DDRIO_REG), dataControl4[strobe].Data);
        } // strobe loop

        MemWritePciCfgEp (host, socket, ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdTrainingCmdN);
        MemWritePciCfgEp (host, socket, ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdTrainingCmdS);
        MemWritePciCfgEp (host, socket, ch, DDRCRCTLTRAINING_CTL_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCtlTraining);
        MemWritePciCfgEp (host, socket, ch, DDRCRCTLTRAINING_CKE_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCkeTraining);
        MemWritePciCfgEp (host, socket, ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRClkTraining);
        MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS2_CMDN_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdControls2CmdN);
        MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS2_CMDS_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdControls2CmdS);
        MemWritePciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCtlControlsCkeRanksUsed);
        MemWritePciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCtlControlsRanksUsed);
        MemWritePciCfgEp (host, socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRClkRanksUsed);
        for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATAOFFSETTRAIN_0_MCIO_DDRIO_REG), (*channelNvList)[ch].dataOffsetTrain[strobe]);
        } // strobe loop
      }

      IO_Reset(host, socket);

      //
      // Disable xover training mode
      //
      dataControl0.Bits.xovercal = 0;

      for (ch = 0; ch < MAX_CH; ch++) {

        if ((*channelNvList)[ch].enabled == 0) continue;

        ddrCRCmdControlsCmdN.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG);
        ddrCRCmdControlsCmdN.Bits.txon = 0;
        MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdControlsCmdN.Data);

        ddrCRCmdControlsCmdS.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG);
        ddrCRCmdControlsCmdS.Bits.txon = 0;
        MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControlsCmdS.Data);

        ddrCRCtlControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG);
        ddrCRCtlControls.Bits.txon = 1;
        MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlControls.Data);
        MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlControls.Data);

        ddrCRClkControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG);
        ddrCRClkControls.Bits.xovercal = 0;
        (*channelNvList)[ch].ddrCRClkControls = ddrCRClkControls.Data;
        MemWritePciCfgEp (host, socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRClkControls);
        for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data | (*channelNvList)[ch].dataControl0);
        }

        for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
          //MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATAOFFSETTRAIN_0_MCIO_DDRIO_REG), (*channelNvList)[ch].dataOffsetTrain[strobe]);

          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), (*channelNvList)[ch].dataControl0);

          //
          // Reload the delay
          //
          dataControl0.Bits.readrfrd = 1;
          dataControl0.Bits.readrfwr = 0;
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data | (*channelNvList)[ch].dataControl0);
          dataControl0.Bits.readrfrd = 0;
          dataControl0.Bits.readrfwr = 1;
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data | (*channelNvList)[ch].dataControl0);
        } // strobe loop
      }

      IO_Reset(host, socket);

      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        RestoreTimings(host, socket, ch);
      }

    } else {
      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;

        MemWritePciCfgEp (host, socket, ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdTrainingCmdN);
        MemWritePciCfgEp (host, socket, ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdTrainingCmdS);
        MemWritePciCfgEp (host, socket, ch, DDRCRCTLTRAINING_CTL_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCtlTraining);
        MemWritePciCfgEp (host, socket, ch, DDRCRCTLTRAINING_CKE_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCkeTraining);
        MemWritePciCfgEp (host, socket, ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRClkTraining);

        ddrCRCtlControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG);
        ddrCRCtlControls.Bits.txon = 1;
        MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlControls.Data);
        MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlControls.Data);

        for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATAOFFSETTRAIN_0_MCIO_DDRIO_REG), (*channelNvList)[ch].dataOffsetTrain[strobe]);
        } // strobe loop
      } // ch loop
    } // if 1TO1 mode
  }

#ifdef SERIAL_DBG_MSG
  releasePrintFControl(host);
#endif // SERIAL_DBG_MSG

  return SUCCESS;
} // CrossoverCalib
