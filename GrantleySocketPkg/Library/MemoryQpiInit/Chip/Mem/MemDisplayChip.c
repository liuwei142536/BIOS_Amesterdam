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
 *      This file contains chip specific display routines.
 *
 ************************************************************************/

#include "SysFunc.h"
#include "MemFunc.h"

#ifdef SERIAL_DBG_MSG
void
DisplayTcothpData(
    PSYSHOST  host, 
    UINT8     socket, 
    UINT8     ch,   
    GSM_GT    group
    )
/*++

  Displays the tCOTHP Data

  @param host:         Pointer to sysHost
  @param socket:       Processor socket
  @param ch:           Memory Channel
  @param group:        GSM_GT group
   
  @retval N/A

--*/
{
  TCOTHP_MCDDC_CTL_STRUCT tCOTHP;
  if(group == TxDqsDelay) {
    tCOTHP.Data = MemReadPciCfgEp (host, socket, ch, TCOTHP_MCDDC_CTL_REG);
    MemDebugPrint((host, SDBG_MEM_TRAIN, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "t_cwl_adj = %d\n", tCOTHP.Bits.t_cwl_adj));
  }
}

void
DisplayXoverResults(
  PSYSHOST  host,
  UINT8     socket
)
/*++

  Displays the training results

  @param host:         Pointer to sysHost
  @param socket:       Processor socket
   
  @retval N/A

--*/
{
  UINT8               ch;
  UINT8               strobe;
  struct channelNvram (*channelNvList)[MAX_CH];
  DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_STRUCT        ddrCRCmdTraining;
  DDRCRCTLTRAINING_CKE_MCIO_DDRIOEXT_HSX_STRUCT     ddrCRCkeTraining;
  DDRCRCTLTRAINING_CTL_MCIO_DDRIOEXT_HSX_STRUCT     ddrCRCtlTraining;
  DDRCRCLKTRAINING_MCIO_DDRIOEXT_STRUCT             ddrCRClkTraining;
  DATAOFFSETTRAIN_0_MCIO_DDRIO_STRUCT               dataOffsetTrain;

  channelNvList = GetChannelNvList(host, socket);

  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\nSTART_DATA_XOVER\n"));

  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "      "));
  if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "  CMDn  CMDs   CTL   CKE   CLK "));
  } else {
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "  CMDn0 CMDn1 CMDs0 CMDs1  CTL   CKE   CLK "));
  }

  for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "  %3d ", strobe));
  } // strobe loop

  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\n"));

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    MemDebugPrint((host, SDBG_MEM_TRAIN, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  ""));
    ddrCRCmdTraining.Data = (*channelNvList)[ch].ddrCRCmdTrainingCmdN;
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "  %3d ", ddrCRCmdTraining.Bits.trainingoffset));
    if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX) == 0) {
      MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "  %3d ", ddrCRCmdTraining.Bits.trainingoffset2));
    }

    ddrCRCmdTraining.Data = (*channelNvList)[ch].ddrCRCmdTrainingCmdS;
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "  %3d ", ddrCRCmdTraining.Bits.trainingoffset));
    if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX) == 0) {
      MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "  %3d ", ddrCRCmdTraining.Bits.trainingoffset2));
    }

    ddrCRCtlTraining.Bits.trainingoffset = (*channelNvList)[ch].ddrCRCtlTraining;
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "  %3d ", ddrCRCtlTraining.Bits.trainingoffset));

    ddrCRCkeTraining.Bits.trainingoffset = (*channelNvList)[ch].ddrCRCkeTraining;
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "  %3d ", ddrCRCkeTraining.Bits.trainingoffset));

    ddrCRClkTraining.Bits.trainingoffset = (*channelNvList)[ch].ddrCRClkTraining;
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "  %3d ", ddrCRClkTraining.Bits.trainingoffset));

    for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
      dataOffsetTrain.Data = (*channelNvList)[ch].dataOffsetTrain[strobe];
      MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "  %3d ", dataOffsetTrain.Bits.txdqoffset));
    } // strobe loop

    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "\n"));
  } // ch loop
} // DisplayXoverResults

#endif // SERIAL_DBG_MSG

