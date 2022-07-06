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

#ifdef RC_SIM
#include "MemSpdSimInit.h"
#endif

//
// Local Prototypes
//
void   AdvancedCentering(PSYSHOST host, UINT8 socket, GSM_GT group);
UINT32 VrefCentering(PSYSHOST host, UINT8 socket, GSM_GT group);
UINT8  VrefQuick (PSYSHOST host, UINT8 socket, GSM_GT group, INT16 *low, INT16 *high);
UINT32 EarlyVrefCentering(PSYSHOST host, UINT8 socket, GSM_GT group);
UINT32 CombineVrefResults(PSYSHOST host, UINT8 socket, GSM_GT group, VOID *value);
void CalculateBitCenters (PSYSHOST host, UINT8 socket, UINT8 ch, INT8 rankBitCenter[MAX_DIMM][MAX_RANK_DIMM][MAX_BITS], struct bitMargin *resultsBit);


#ifdef RC_SIM_FEEDBACK
/*++
  RCSIM feedback
--*/
const INT16 RCSimSenseAmp [MAX_STROBE][4] = {
    {   13,   14,   20,   15},
    {   10,   14,   10,   13},
    {   18,   20,   15,   11},
    {   12,   22,   23,   22},
    {   8,    22,   21,   18},
    {   21,   8,    9,    20},
    {   9,    17,   12,   16},
    {   12,   10,   9,    23},
    {   23,   13,   21,   8},
    {   12,   16,   19,   14},
    {   16,   10,   21,   9},
    {   14,   22,   16,   13},
    {   15,   18,   21,   19},
    {   15,   13,   12,   9},
    {   17,   23,   21,   18},
    {   9,    13,   17,   11},
    {   13,   13,   11,   14},
    {   9,    20,   20,   16},
};

const INT16 RmtSimFeedback [][14] = {
 //RxDqLeft RxDqRight  RxVLow RxVHigh  TxDqLeft TxDqRight  TxVLow TxVHigh  CmdLeft CmdRight  CmdLo, CmdHi,  Ctl-,  CTL +
  {     -18,      15,     -28,     28,     -16,       20,     -14,     15,     -48,      49,    -18,    20,      -29,      31}, //ch0
  {     -17,      15,     -32,     32,     -17,       19,     -15,     16,     -50,      49,    -19,    21,      -30,      32}, //ch1
  {     -18,      17,     -29,     30,     -15,       19,     -14,     14,     -46,      48,    -20,    22,      -31,      33}, //ch2
  {     -18,      15,     -31,     30,     -17,       19,     -14,     16,     -50,      52,    -21,    23,      -32,      34}, //ch3
  {     -18,      15,     -28,     28,     -16,       20,     -14,     15,     -48,      49,    -22,    24,      -33,      35}, //ch4
  {     -17,      15,     -32,     32,     -17,       19,     -15,     16,     -50,      49,    -23,    25,      -38,      30}  //ch5
};

const INT16 RmtSimFeedbackRxPBit [MAX_STROBE][8] = {
 //RxDqLeft[0] RxDqRight[0] RxDqLeft[1] RxDqRight[1] RxDqLeft[2] RxDqRight[2] RxDqLeft[3] RxDqRight[3]
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20}
};

const INT16 RmtSimFeedbackRxNBit [MAX_STROBE][8] = {
 //RxDqLeft[0] RxDqRight[0] RxDqLeft[1] RxDqRight[1] RxDqLeft[2] RxDqRight[2] RxDqLeft[3] RxDqRight[3]
  {     -18,      18,     -14,     16,     -20,       16,     -18,     20},
  {     -18,      18,     -14,     16,     -20,       16,     -18,     20},
  {     -18,      18,     -14,     16,     -20,       16,     -18,     20},
  {     -18,      18,     -14,     16,     -20,       16,     -18,     20},
  {     -18,      18,     -14,     16,     -20,       16,     -18,     20},
  {     -18,      18,     -14,     16,     -20,       16,     -18,     20},
  {     -18,      18,     -14,     16,     -20,       16,     -18,     20},
  {     -18,      18,     -14,     16,     -20,       16,     -18,     20},
  {     -18,      18,     -14,     16,     -20,       16,     -18,     20},
  {     -18,      18,     -14,     16,     -20,       16,     -18,     20},
  {     -18,      18,     -14,     16,     -20,       16,     -18,     20},
  {     -18,      18,     -14,     16,     -20,       16,     -18,     20},
  {     -18,      18,     -14,     16,     -20,       16,     -18,     20},
  {     -18,      18,     -14,     16,     -20,       16,     -18,     20},
  {     -18,      18,     -14,     16,     -20,       16,     -18,     20},
  {     -18,      18,     -14,     16,     -20,       16,     -18,     20},
  {     -18,      18,     -14,     16,     -20,       16,     -18,     20},
  {     -18,      18,     -14,     16,     -20,       16,     -18,     20}
};

const INT16 RmtSimFeedbackTxBit [MAX_STROBE][8] = {
 //TxDqLeft[0] TxDqRight[0] TxDqLeft[1] TxDqRight[1] TxDqLeft[2] TxDqRight[2] TxDqLeft[3] TxDqRight[3]
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20},
  {     -18,      20,     -16,     12,     -20,       24,     -16,     20}
};

const INT16 RmtSimFeedbackTxVref  [MAX_STROBE][8] = {
  {   -13,   17,   -13,   12,   -15,   10,   -18,   17},
  {   -14,   10,   -14,   12,   -17,   13,   -14,   16},
  {   -11,   12,   -18,   14,   -13,   12,   -15,   16},
  {   -18,   14,   -18,   13,   -13,   13,   -14,   17},
  {   -12,   11,   -17,   13,   -17,   15,   -12,   11},
  {   -12,   13,   -14,   11,   -14,   14,   -12,   11},
  {   -12,   12,   -16,   15,   -13,   14,   -15,   12},
  {   -11,   17,   -14,   15,   -15,   11,   -11,   17},
  {   -11,   13,   -18,   14,   -12,   15,   -16,   11},
  {   -17,   16,   -12,   11,   -12,   16,   -16,   11},
  {   -18,   15,   -14,   16,   -12,   17,   -15,   10},
  {   -13,   11,   -18,   12,   -17,   15,   -18,   13},
  {   -14,   17,   -15,   13,   -13,   13,   -18,   15},
  {   -18,   14,   -14,   17,   -12,   13,   -14,   12},
  {   -17,   11,   -14,   13,   -18,   14,   -16,   15},
  {   -17,   13,   -16,   12,   -17,   16,   -12,   14},
  {   -14,   13,   -13,   10,   -13,   13,   -17,   17},
  {   -17,   15,   -15,   10,   -14,   11,   -11,   11}
};

const INT16 RmtSimFeedbackRxVref  [MAX_STROBE][8] = {
  {   -16,   17,   -18,   15,   -21,   17,   -17,   16},
  {   -18,   17,   -21,   17,   -18,   16,   -17,   16},
  {   -17,   16,   -21,   14,   -15,   13,   -21,   14},
  {   -17,   14,   -16,   18,   -15,   18,   -15,   13},
  {   -18,   19,   -20,   16,   -14,   20,   -19,   15},
  {   -21,   19,   -17,   13,   -15,   13,   -21,   18},
  {   -20,   17,   -20,   15,   -20,   20,   -18,   18},
  {   -15,   17,   -20,   14,   -20,   20,   -17,   15},
  {   -15,   15,   -21,   16,   -20,   19,   -15,   17},
  {   -18,   20,   -20,   15,   -14,   19,   -18,   15},
  {   -17,   17,   -14,   19,   -15,   19,   -16,   19},
  {   -14,   19,   -16,   16,   -17,   13,   -15,   19},
  {   -21,   19,   -14,   19,   -16,   14,   -14,   14},
  {   -14,   14,   -21,   13,   -17,   18,   -21,   19},
  {   -20,   15,   -15,   18,   -17,   14,   -17,   18},
  {   -18,   16,   -17,   18,   -17,   13,   -20,   13},
  {   -19,   13,   -17,   13,   -17,   14,   -15,   18},
  {   -20,   16,   -17,   16,   -16,   20,   -15,   20}
};


#ifdef  RC_SIM_EYE_DATA_FEEDBACK

/**
  RCSIM feedback
**/
void
RCSIMFeedbackResults (
  PSYSHOST        host,
  UINT8           socket,
  UINT8           ch,
  UINT8           dimm,
  UINT8           rank,
  UINT8           scope,
  GSM_GT          group,
  INT16           previousMargin,
  UINT32          bwSerr[MAX_CH][3]
  )
{

  UINT8                 bit;
  INT16                 margin_p;
  INT16                 margin_n;


  UINT8                 error_count_no_noise = 0;
  UINT8                 error_count_no_noise2 = 0;


  //
  // Find feedback per bit
  ///



  if ((scope == SCOPE_BIT) | (scope == SCOPE_STROBE)){
    for (bit = 0; bit < 72; bit++) {
      switch (group){
        case TxVref:
        case TxDqDelay:
        case TxDqsDelay:
          error_count_no_noise = (UINT8)EyeData_getErrorCount(host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct[rank % 2].txEyeData[bit],0);
          if (error_count_no_noise>5) error_count_no_noise = 1; else error_count_no_noise = 0;
          break;
        case RxVref:
        case RxDqDelay:
        case RxDqsDelay:
        //Getting Error Count
       // restoreMsg (host);
       // MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, bit,
        //   "get error count"));
       // disableAllMsg (host);
          error_count_no_noise = (UINT8)EyeData_getErrorCount(host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct[rank % 2].rxNEyeData[bit],0);
          error_count_no_noise2 = (UINT8)EyeData_getErrorCount(host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct[rank % 2].rxPEyeData[bit],0);
          if ((error_count_no_noise>5) || (error_count_no_noise2>5)) error_count_no_noise = 1; else error_count_no_noise = 0;
          break;
        case RxDqsPDelay:
          error_count_no_noise = (UINT8)EyeData_getErrorCount(host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct[rank % 2].rxPEyeData[bit],0);
          if (error_count_no_noise>5) error_count_no_noise = 1; else error_count_no_noise = 0;
          break;

        case RxDqsNDelay:
          error_count_no_noise = (UINT8)EyeData_getErrorCount(host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct[rank % 2].rxNEyeData[bit],0);
          if (error_count_no_noise>5) error_count_no_noise = 1; else error_count_no_noise = 0;
          break;
        default:
          break;
      }

      switch (group) {
        case TxDqDelay:
        case TxDqsDelay:
        case RxDqDelay:
        case RxDqsDelay:
        case RxDqsPDelay:
        case RxDqsNDelay:
        case TxVref:
        case RxVref:
        default:
          margin_p =  0;
          margin_n =  0;
          break;
      } //switch


      if ((group == TxDqDelay) || (group == TxDqsDelay) || (group == RxDqDelay) || (group == RxDqsDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay) || (group == RxVref) || (group == TxVref)){
        switch (bit/32) {
          case 0:
            bwSerr[ch][0] |= error_count_no_noise<<(bit%32);
            break;
          case 1:
            bwSerr[ch][1] |= error_count_no_noise<<(bit%32);
            break;
          case 2:
            bwSerr[ch][2] |= error_count_no_noise<<(bit%32);
            break;
          default:
            break;
        }
      } else if ((previousMargin <= margin_n) | (previousMargin >= margin_p)){
        switch (bit/32) {
          case 0:
            bwSerr[ch][0] |= 1<<(bit%32);
            break;
          case 1:
            bwSerr[ch][1] |= 1<<(bit%32);
            break;
          case 2:
            bwSerr[ch][2] |= 1<<(bit%32);
            break;
          default:
            break;
        } // switch
      } // if error range
    } // bit

  //
  // Find feedback per rank
  //
  } else if(scope == SCOPE_RANK){

    for (bit = 0; bit < 72; bit++) {
        switch (group){
            case TxDqDelay:
            case TxDqsDelay:
            case TxVref:
              error_count_no_noise = (UINT8)EyeData_getErrorCount(host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct[rank % 2].txEyeData[bit],0);
              if (error_count_no_noise>5) error_count_no_noise = 1; else error_count_no_noise = 0;
              break;
            case RxDqDelay:
            case RxDqsDelay:
            case RxEq:
            case RxVref:
              error_count_no_noise = (UINT8)EyeData_getErrorCount(host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct[rank % 2].rxNEyeData[bit],0);
              error_count_no_noise2 = (UINT8)EyeData_getErrorCount(host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct[rank % 2].rxPEyeData[bit],0);
              if ((error_count_no_noise>5) || (error_count_no_noise2>5)) error_count_no_noise = 1; else error_count_no_noise = 0;
              break;
            case RxDqsPDelay:
              error_count_no_noise = (UINT8)EyeData_getErrorCount(host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct[rank % 2].rxPEyeData[bit],0);
              if (error_count_no_noise>5) error_count_no_noise = 1; else error_count_no_noise = 0;
              break;

            case RxDqsNDelay:
              error_count_no_noise = (UINT8)EyeData_getErrorCount(host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct[rank % 2].rxNEyeData[bit],0);
              if (error_count_no_noise>5) error_count_no_noise = 1; else error_count_no_noise = 0;
              break;

            default:
              break;
        }
    } // bit
    switch (group) {
      case RxDqsDelay:
      case RxDqsPDelay:
      case RxDqsNDelay:
      case RxDqDelay:
      case RxEq:
      case TxDqsDelay:
      case TxDqDelay:
      case RxVref:
      case TxVref:
        margin_p =  0;
        margin_n =  0;
        break;

      case CmdAll:
        margin_p =  RmtSimFeedback[ch][9] + rank*2;
        margin_n =  RmtSimFeedback[ch][8] + rank*2;
        break;

      case CtlAll:
        margin_p =  RmtSimFeedback[ch][13] + rank*2;
        margin_n =  RmtSimFeedback[ch][12] + rank*2;
        break;

      case CmdVref:
        margin_p =  RmtSimFeedback[ch][11] + rank*2;
        margin_n =  RmtSimFeedback[ch][10] + rank*2;
        break;

      default:
        margin_p =  0;
        margin_n =  0;
        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT, "RCSIMFeedbackResults called for an unknown group memtraining.c!\n"));
        break;
    } // switch group

    if ((group == TxDqDelay) || (group == TxDqsDelay)|| (group == RxEq) || (group == RxDqDelay) || (group == RxDqsDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay) || (group == RxVref) || (group == TxVref)){
      switch (bit/32) {
        case 0:
          bwSerr[ch][0] |= error_count_no_noise<<(bit%32);
          break;
        case 1:
          bwSerr[ch][1] |= error_count_no_noise<<(bit%32);
          break;
        case 2:
          bwSerr[ch][2] |= error_count_no_noise<<(bit%32);
          break;
        default:
          break;
      }
    } else if ((previousMargin <= margin_n) | (previousMargin >= margin_p)){
      bwSerr[ch][0] = (UINT32)-1;
      bwSerr[ch][1] = (UINT32)-1;
      bwSerr[ch][2] = (UINT32)-1;
    }
  } //scope = RANK
} // RCSIMFeedbackResults
#else
void
RCSIMFeedbackResults (
  PSYSHOST        host,
  UINT8           socket,
  UINT8           ch,
  UINT8           rank,
  UINT8           scope,
  GSM_GT          group,
  INT16           previousMargin,
  UINT32          bwSerr[MAX_CH][3]
  )
/*++
  RCSIM feedback

  @param host            - Pointer to sysHost
  @param socket          - Socket number
  @param ch              - Channel number
  @param rank            - Dimm Rank
  @param scope           - Scope
  @param group           - Group
  @param previousMargin  - Previous Margin
  @param bwSerr          - BitWise error status

  @retval N/A

--*/
{

  UINT8                 bit;
  INT16                 margin_p;
  INT16                 margin_n;

  //
  // Find feedback per bit
  ///
  if ((scope == SCOPE_BIT) | (scope == SCOPE_STROBE)){
    for (bit = 0; bit < 72; bit++) {
      switch (group) {
        case TxDqDelay:
        case TxDqsDelay:
          margin_n = RmtSimFeedbackTxBit[bit % MAX_STROBE][(bit % 4) * 2];
          margin_p = RmtSimFeedbackTxBit[bit % MAX_STROBE][((bit % 4) * 2) + 1];
          break;

        case RxDqDelay:
        case RxDqsDelay:
        case RxDqsPDelay:
        case CmdAll:
        case CtlAll:
          margin_n = RmtSimFeedbackRxPBit[bit % MAX_STROBE][(bit % 4) * 2];
          margin_p = RmtSimFeedbackRxPBit[bit % MAX_STROBE][((bit % 4) * 2) + 1];
          break;

        case RxDqsNDelay:
          margin_n = RmtSimFeedbackRxNBit[bit % MAX_STROBE][(bit % 4) * 2];
          margin_p = RmtSimFeedbackRxNBit[bit % MAX_STROBE][((bit % 4) * 2) + 1];
          break;

        case TxVref:
          margin_n = RmtSimFeedbackTxVref[bit % MAX_STROBE][(bit % 4) * 2] - rank;
          margin_p = RmtSimFeedbackTxVref[bit % MAX_STROBE][((bit % 4) * 2) + 1] - rank; // rank to rank variation
          break;

        case RxVref:
          margin_n = RmtSimFeedbackRxVref[bit % MAX_STROBE][(bit % 4) * 2];
          margin_p = RmtSimFeedbackRxVref[bit % MAX_STROBE][((bit % 4) * 2) + 1];
          break;

        default:
          margin_p =  0;
          margin_n =  0;
          break;
      } //switch

      //MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, rank, NO_STROBE, NO_BIT, "**Debug %d to %d, pi %d\n",margin_n, margin_p, previousMargin));

      //
      // feedback for power training steps
      //
      margin_p = (margin_p * host->var.mem.eyeSizeScaling)/100;
      margin_n = (margin_n * host->var.mem.eyeSizeScaling)/100;

      // update bwSerr
      if ((previousMargin <= margin_n) | (previousMargin >= margin_p)){
        switch (bit/32) {
          case 0:
            bwSerr[ch][0] |= 1<<(bit%32);
            break;
          case 1:
            bwSerr[ch][1] |= 1<<(bit%32);
            break;
          case 2:
            bwSerr[ch][2] |= 1<<(bit%32);
            break;
          default:
            break;
        } // switch
      } // if error range
    } // bit

  //
  // Find feedback per rank
  //
  } else if(scope == SCOPE_RANK){
    switch (group) {
      case RxDqsDelay:
      case RxDqsPDelay:
      case RxDqsNDelay:
      case RxDqDelay:
      case RxEq:
        margin_p =  RmtSimFeedback[ch][1] + rank*2; // rank*2 just adds some variation from rank to rank
        margin_n =  RmtSimFeedback[ch][0] + rank*2;
        break;

      case TxDqsDelay:
      case TxDqDelay:
        margin_p =  RmtSimFeedback[ch][5] + rank*2;
        margin_n =  RmtSimFeedback[ch][4] + rank*2;
        break;

      case RxVref:
        margin_p =  RmtSimFeedback[ch][3] + rank*2;
        margin_n =  RmtSimFeedback[ch][2] + rank*2;
        break;

      case TxVref:
        margin_p =  RmtSimFeedback[ch][7] + rank;
        margin_n =  RmtSimFeedback[ch][6] + rank;
        break;

      case CmdAll:
        margin_p =  RmtSimFeedback[ch][9] + rank*2;
        margin_n =  RmtSimFeedback[ch][8] + rank*2;
        break;

      case CtlAll:
        margin_p =  RmtSimFeedback[ch][13] + rank*2;
        margin_n =  RmtSimFeedback[ch][12] + rank*2;
        break;

      case CmdVref:
        margin_p =  RmtSimFeedback[ch][11] + rank*2;
        margin_n =  RmtSimFeedback[ch][10] + rank*2;
        break;

      default:
        margin_p =  0;
        margin_n =  0;
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, rank, NO_STROBE, NO_BIT, "RCSIMFeedbackResults called for an unknown group memtraining.c!\n"));
        break;
    } // switch group
    //MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, rank, NO_STROBE, NO_BIT, "**Debug %d to %d, pi %d\n",margin_n, margin_p, previousMargin));
    if ((previousMargin <= margin_n) | (previousMargin >= margin_p)){
      bwSerr[ch][0] = (UINT32)-1;
      bwSerr[ch][1] = (UINT32)-1;
      bwSerr[ch][2] = (UINT32)-1;
    }
  } //scope = RANK
} // RCSIMFeedbackResults

#endif //   RC_SIM_EYE_DATA_FEEDBACK

 /*const INT16 RmtSimFeedback [][10] = {
 //RxDqLeft RxDqRight  RxVLow RxVHigh  TxDqLeft TxDqRight  TxVLow TxVHigh  CmdLeft CmdRight
  {     -18,      15,     -28,     28,     -16,       20,     -14,     15,     -48,      49},
  {     -17,      15,     -32,     32,     -17,       19,     -15,     16,     -50,      49},
  {     -18,      17,     -29,     30,     -15,       19,     -14,     14,     -46,      48},
  {     -18,      15,     -31,     30,     -17,       19,     -14,     16,     -50,      52},
  {     -18,      15,     -28,     28,     -16,       20,     -14,     15,     -48,      49},
  {     -17,      15,     -32,     32,     -17,       19,     -15,     16,     -50,      49}
};*/

void
RmtFeedbackResults (
  PSYSHOST        host,
  UINT8           socket,
  UINT8           dimm,
  UINT8           rank,
  UINT8           sign,
  UINT8           bitMask,
  UINT8           chStatus,
  UINT8           scope,
  GSM_GT          group,
  UINT16          mode,
  void            *marginData
  )
/*++
--*/
{
  UINT8                 ch;
  UINT8                 strobe;
  UINT8                 maxDimm;
  UINT8                 maxRank;
  UINT8                 setAllStrobe;
  UINT8                 strobeByte = 0;
  UINT8                 i;
  UINT8                 bit;
  UINT8                 tmpBitMask;
  INT16                 margin_p;
  INT16                 margin_n;
  struct  ddrRank       (*rankList)[MAX_RANK_DIMM];
  struct  baseMargin    *socketM;
  struct  chMargin      *channelM;
  struct  rankMargin    *rankM;
  struct  strobeMargin  *strobeM;
  struct  bitMargin     *bitM;

  struct  channelNvram  (*channelNvList)[MAX_CH];

  if((host->setup.mem.options & MARGIN_RANKS_EN) == 0) return;

  channelNvList = GetChannelNvList(host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    if (ch > (sizeof(RmtSimFeedback) / sizeof(RmtSimFeedback[0]) - 1)) {
      MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                      "RmtFeedbackResults does not support %d channels!\n", ch+1));
      RC_ASSERT(FALSE, ERR_RC_INTERNAL, 0);
    }

    // No need to check this channel if it is done
    if ((chStatus & (1 << ch)) == 0) continue;

    if (dimm == SET_ALL) {
      dimm = 0;
      maxDimm = MAX_DIMM;
    } else {
      maxDimm = dimm + 1;
    }
    if (rank == SET_ALL) {
      rank = 0;
      maxRank = MAX_RANK_DIMM;
    } else {
      maxRank = rank + 1;
    }

    for (strobe = 0; strobe < MAX_STROBE/2; strobe++) {
      if ((!host->nvram.mem.eccEn) && (strobe == 8)) continue;

      if (strobe == SET_ALL) {
        strobe = 0;
        setAllStrobe = SET_ALL;
      } else {
        strobeByte = strobe;
        if (strobe > 8) strobe = strobe - 9;
        setAllStrobe = strobe;
      }

      switch (group) {
        case TxDqDelay:
        case TxDqsDelay:
            margin_p =  RmtSimFeedback[ch][5];
            margin_n =  RmtSimFeedback[ch][4];
          break;

        case TxEq:
        case TxDqBitDelay:
            margin_p =  0;
            margin_n =  0;
          break;

        case RxDqsBitDelay:
            margin_p =  0;
            margin_n =  0;
          break;

        case RecEnDelay:
        case RxDqsPDelay:
        case RxDqsNDelay:
        case RxDqDelay:
        case RxEq:
            margin_p =  0;
            margin_n =  0;
          break;

        case RxDqsDelay:
            margin_p =  RmtSimFeedback[ch][1];
            margin_n =  RmtSimFeedback[ch][0];
          break;

        case RxVref:
            margin_p =  RmtSimFeedback[ch][3];
            margin_n =  RmtSimFeedback[ch][2];
          break;

        case TxVref:
            margin_p =  RmtSimFeedback[ch][7];
            margin_n =  RmtSimFeedback[ch][6];
          break;
        case CmdAll:
            margin_p =  RmtSimFeedback[ch][9];
            margin_n =  RmtSimFeedback[ch][8];
          break;

        default:
          margin_p =  0;
          margin_n =  0;
          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                      "RmtFeedbackResults called for an unknown group!\n"));
      } // switch group

      if (scope == SCOPE_SOCKET){
        socketM = marginData;

        // Save new margin at the failing point
        if (sign) (*socketM).n = margin_n;
        else (*socketM).p = margin_p;
      } else if (scope == SCOPE_CH){
        channelM = marginData;

        // Save new margin at the failing point
        if (sign) (*channelM).channel[ch].n = margin_n;
        else (*channelM).channel[ch].p = margin_p;
      } else if (scope == SCOPE_DIMM){
      } else if (scope == SCOPE_RANK){
        rankM = marginData;
        rankList = GetRankNvList(host, socket, ch, dimm);

        // Save new margin at the failing point
        if (sign) (*rankM).rank[ch][(*rankList)[rank].rankIndex].n = margin_n;
        else (*rankM).rank[ch][(*rankList)[rank].rankIndex].p = margin_p;
      } else if (scope == SCOPE_STROBE){
        strobeM = marginData;
        rankList = GetRankNvList(host, socket, ch, dimm);

        if (sign) {
          (*strobeM).strobe[ch][(*rankList)[rank].rankIndex][strobeByte].n = margin_n;
        } else {
          (*strobeM).strobe[ch][(*rankList)[rank].rankIndex][strobeByte].p = margin_p;
        }
      } else if (scope == SCOPE_BIT){
        bitM = marginData;
        rankList = GetRankNvList(host, socket, ch, dimm);

        i = 0;
        tmpBitMask = bitMask;
        while (tmpBitMask != 1) {
          tmpBitMask >>= 1;
          i++;
        }

        bit = (strobe * 8) + i;

        if (sign) (*bitM).bits[ch][(*rankList)[rank].rankIndex][bit].n = margin_n;
        else      (*bitM).bits[ch][(*rankList)[rank].rankIndex][bit].p = margin_p;
      } // if scope
    } // for strobe
  } // for ch
} // RmtFeedbackResults

#endif // RC_SIM_FEEDBACK

/**

  Evaluate if the Rank List work is complete

  @param host       - Pointer to sysHost
  @param socket     - Socket number
  @param ch         - Current channel
  @param dimm       - Dimm on current channel
  @param rank       - Rank of current Dimm
  @param scope      - Margin granularity
  @param ranksDone  - Array of bits showing if ranks are done. If bitX = 1, the rank is done.

  @retval N/A

**/
void
EvalResSetRanksDone (
                    PSYSHOST  host,
                    UINT8     socket,
                    UINT8     ch,
                    UINT8     dimm,
                    UINT8     rank,
                    UINT8     scope,
                    UINT32    *ranksDone
                    )
{
  UINT8           imc;
  struct  ddrRank (*rankList)[MAX_RANK_DIMM];


  imc      = GetMCID(host, socket, ch);
  rankList = GetRankNvList(host, socket, ch, dimm);

  if ((scope == SCOPE_SOCKET) || (scope == SCOPE_CH)) {
    ranksDone[imc] = 0xFFFFFFFF;
  } else {
    ranksDone[imc] |= (1 << ((*rankList)[rank].rankIndex + (GetMCCh(imc, ch) * 8)));
  }
} // EvalResSetRanksDone


void
AdvancedCentering (
                  PSYSHOST  host,
                  UINT8     socket,
                  GSM_GT    group
                  )
/*++

  Re-centers rx or tx dq timing using a more robust test

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param group   - Parameter to center
                   RxDqsDelay    1: Center Rx DqDqs
                   RxDqsPDelay   2: Center Rx DqDqsp
                   RxDqsNDelay   3: Center Rx DqDqsn
                   TxDqDelay     12: Center Tx DqDqs

  @retval N/A

--*/
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  UINT8               maxStrobe;
  UINT8               maxVref;
  UINT8               numPoints = 0;
  UINT8               stepSize = 0;
  INT16               offset;
  INT16               low = 0;
  INT16               high = 0;
  INT32               numerator;
  INT32               denominator;
  INT16               initalOffset;
  INT16               worstEyeWidth = 0;
  INT16               timingCenter = 0;
  INT16               offset1;
  INT16               offset2;
  INT16               temp;
  UINT8               Multiplier = 0;
  INT16               eyeWidth;
  GSM_GT              vrefGroup = 0;
  INT16               maxOffsetFromTimingCenter = 0;
  UINT16              vrefRatio = 0;
  UINT16              vrefRatioSpec = 0;
  UINT8               mode = 0;
  UINT8               vIndex;
  UINT8               vIndexCenter;
  UINT32              patternLength = 1;
  INT16               eyeHeight;
#ifdef SERIAL_DBG_MSG
  INT16               margin;
  INT16               delay[MAX_STROBE];
  INT16               oldDelay[MAX_STROBE];
#endif  //SERIAL_DBG_MSG
  struct bitMask      filter;
  struct strobeMargin results[MAX_VREF_POINTS];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return;

  channelNvList = GetChannelNvList(host, socket);

  // Get pattern length
  if ((group == RxDqsDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay)) {
    mode = MODE_VIC_AGG;
    vrefGroup = RxVref;
    patternLength = TRAIN_RD_DQS_PL;
    numPoints = RX_MULTI_VREF_POINTS;
    stepSize = RX_MULTI_VREF_STEP;

  } else if (group == TxDqDelay) {
    mode = MODE_VIC_AGG;
    vrefGroup = TxVref;
    patternLength = TRAIN_WR_DQS_PL;
    numPoints = TX_MULTI_VREF_POINTS;
    stepSize = TX_MULTI_VREF_STEP;
  }

  vIndexCenter = numPoints / 2;

  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter(host, socket, &filter);

  // Setup bitMask
  for (ch = 0; ch < MAX_CH; ch++) {
    for (rank = 0; rank < MAX_RANK_CH; rank++) {
      for (strobe = 0; strobe < MAX_STROBE; strobe++) {
#ifdef SERIAL_DBG_MSG
        oldDelay[strobe] = 0;
        delay[strobe] = 0;
#endif
        for (vIndex = 0; vIndex < numPoints; vIndex++) {
          results[vIndex].strobe[ch][rank][strobe].n = 0;
          results[vIndex].strobe[ch][rank][strobe].p = 0;
        }
      } // strobe loop
    } // rank loop
  } // ch loop

  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "Get eye width\n"));
  VrefQuick(host, socket, group, &low, &high);
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "Low: %3d High: %3d\n", low, high));
  initalOffset = ((low + high))/2; // Doesn't matter if we round
  worstEyeWidth = -low + high;

  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "Offset = %d\n", initalOffset));
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "Eye width = %d\n", worstEyeWidth));

  // Center timing for all strobes and ranks if required to get a better vref value for risk analysis
  for (ch = 0; ch < MAX_CH; ch++) {
    //
    // Skip if channel is disabled
    //
    if ((*channelNvList)[ch].enabled == 0) continue;
    dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
      rankList = GetRankNvList(host, socket, ch, dimm);
      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        maxStrobe = GetMaxStrobe(host, socket, ch, dimm, group, DdrLevel);

        // Get Vref Margins at timing center
        for (strobe = 0; strobe < maxStrobe; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
          // Apply new center
          GetSetDataGroup (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, group, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &initalOffset);
        } // strobe loop
      } // rank loop
    } // dimm loop
  } // ch loop
  //
  // Get eye height (Vref margins)
  //
  //GetMargins (host, socket, DdrLevel, vrefGroup, mode, SCOPE_SOCKET, &filter, (void *)&vrefResults, patternLength, 0, 0, 0,
  //            WDB_BURST_LENGTH);
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "Get eye height\n"));
  VrefQuick(host, socket, vrefGroup, &low, &high);
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "Low: %3d High: %3d\n", low, high));
  eyeHeight = (-low) + high;

  if (vrefGroup == RxVref) {
    eyeHeight = (5 * eyeHeight) / 8;
  }

  if (eyeHeight < 8) {
    eyeHeight = 8;
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "New Eye height = %d\n", eyeHeight));
  }
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "Eye height = %d\n", eyeHeight));

  // Now find the ratio between the worst-case timing and voltage margins across all rank and figure out it we are
  // timing or voltage margin limited.
  vrefRatio = (worstEyeWidth * 1000 / eyeHeight); // 1000x and RxVref adjusted

  // All ratios are 1000x and RxVref adjusted
  if (host->nvram.mem.socket[socket].ddrFreq >= DDR_2400) {
    if ((group == RxDqsDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay)) {
      vrefRatioSpec = 1467; // * 1000
    } else if (group == TxDqDelay) {
      vrefRatioSpec = 1300; // * 1000
    }
  } else if (host->nvram.mem.socket[socket].ddrFreq >= DDR_2133) {
    if ((group == RxDqsDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay)) {
      vrefRatioSpec = 1333; // * 1000
    } else if (group == TxDqDelay) {
      vrefRatioSpec = 1200; // * 1000
    }
  } else if (host->nvram.mem.socket[socket].ddrFreq >= DDR_1867) {
    if ((group == RxDqsDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay)) {
      vrefRatioSpec = 1333; // * 1000
    } else if (group == TxDqDelay) {
      vrefRatioSpec = 1200; // * 1000
    }
  } else if (host->nvram.mem.socket[socket].ddrFreq >= DDR_1600) {
    if ((group == RxDqsDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay)) {
      vrefRatioSpec = 1200; // * 1000
    } else if (group == TxDqDelay) {
      vrefRatioSpec = 1100; // * 1000
    }
  } else { // Anything lower than 1600
    if ((group == RxDqsDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay)) {
      vrefRatioSpec = 1200; // * 1000
    } else if (group == TxDqDelay) {
      vrefRatioSpec = 1100; // * 1000
    }
  }

  // Find the minimum timing offset from 1D timing center (given a worst-case triangular eye) that must be set to end up
  // timing margin limited. Make sure if we are doing a 2D centering, we do not run over this value.
  numerator = (INT32)(1000 * worstEyeWidth * worstEyeWidth - vrefRatioSpec * eyeHeight * worstEyeWidth); // *1000 to account for VrefMultiplier * 1000. Numerator is 1000x what it really is
  if (numerator < 0) {
    numerator = 0;
  }
  denominator = (INT32)(worstEyeWidth * 2 * 1000 + 2 * vrefRatioSpec * eyeHeight); // 1000 to account for VrefMultiplier * 1000. Denominator is 1000x what it really is
  if (denominator < 0) {
    denominator = -denominator;
  }
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "numerator: %9d\n", numerator));
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "denominator: %9d\n", denominator));

  maxOffsetFromTimingCenter = (INT16)(numerator / denominator); // No need to round, since we want minimum
  if (maxOffsetFromTimingCenter < 0) {
    maxOffsetFromTimingCenter = -maxOffsetFromTimingCenter;
  }
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "vrefRatio: %5d, vrefRatioSpec: %5d\n", vrefRatio, vrefRatioSpec));
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "Max offset from timing center: %3d\n", maxOffsetFromTimingCenter));
  //
  // 1D timing centering
  //
  if(vrefRatio <= vrefRatioSpec) {
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "Timing Limited\n"));
  } else {
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "Voltage Limited\n"));
  }

  vIndexCenter = numPoints / 2;

  //
  // Set eyeHeight
  //
  //eyeHeight = vrefResults.p - vrefResults.n;

  //
  // Calculate Multi-Vref step size
  //
  stepSize = (UINT8)(eyeHeight / 8);
  maxVref = (numPoints / 2) * stepSize;

  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "GetMultiVref: patternLength = %d, stepSize = %d, numPoints = %d\n",
                patternLength, stepSize, numPoints));

  // Get margins per strobe group
  GetMultiVref (host, socket, DdrLevel, group, mode, SCOPE_STROBE, &filter, (void *)results, patternLength, 0, 0, stepSize, numPoints);
#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) {
    getPrintFControl (host);

    if (group == RxDqsDelay) {
      rcPrintf ((host, "START_DATA_RX_DQS_ADV\n"));
    } else if (group == RxDqsPDelay) {
      rcPrintf ((host, "START_DATA_RX_DQSP_ADV\n"));
    } else if (group == RxDqsNDelay) {
      rcPrintf ((host, "START_DATA_RX_DQSN_ADV\n"));
    } else if (group == TxDqDelay) {
      rcPrintf ((host, "START_DATA_TX_DQ_ADV\n"));
    }
  }
#endif  // SERIAL_DBG_MSG

  for (ch = 0; ch < MAX_CH; ch++) {
    //
    // Skip if channel is disabled
    //
    if ((*channelNvList)[ch].enabled == 0) continue;

    dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      rankList = GetRankNvList(host, socket, ch, dimm);

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        maxStrobe = GetMaxStrobe(host, socket, ch, dimm, group, DdrLevel);

        for (strobe = 0; strobe < maxStrobe; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

          for (vIndex = 0; vIndex < numPoints; vIndex++) {
            if (results[vIndex].strobe[ch][(*rankList)[rank].rankIndex][strobe].n >
                results[vIndex].strobe[ch][(*rankList)[rank].rankIndex][strobe].p) {

              results[vIndex].strobe[ch][(*rankList)[rank].rankIndex][strobe].n = 0;
              results[vIndex].strobe[ch][(*rankList)[rank].rankIndex][strobe].p = 0;
            }
          } // vIndex loop

          //
          // Get the total eye width
          //
          eyeWidth = results[vIndexCenter].strobe[ch][(*rankList)[rank].rankIndex][strobe].p -
                     results[vIndexCenter].strobe[ch][(*rankList)[rank].rankIndex][strobe].n;
          timingCenter = (results[vIndexCenter].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                          results[vIndexCenter].strobe[ch][(*rankList)[rank].rankIndex][strobe].n)/2;
          // Round to counter pattern asymmetry issues
          temp = (results[vIndexCenter].strobe[ch][(*rankList)[rank].rankIndex][strobe].p + results[vIndexCenter].strobe[ch][(*rankList)[rank].rankIndex][strobe].n);
          if (temp < 0) {
            temp = -temp;
          }
          if ((temp % 2) == 1) {
            if (results[vIndexCenter].strobe[ch][(*rankList)[rank].rankIndex][strobe].p + results[vIndexCenter].strobe[ch][(*rankList)[rank].rankIndex][strobe].n < 0) {
              timingCenter -= 1;
            } else {
              timingCenter += 0;
            }
          }
          Multiplier = 1; //BDX Multiplier

          //
          // If RX centering and speed > 1866, then do 1D centering calculation
          //
          if (numPoints == 1) {
            offset = (results[vIndexCenter].strobe[ch][(*rankList)[rank].rankIndex][strobe].p + results[vIndexCenter].strobe[ch][(*rankList)[rank].rankIndex][strobe].n) / 2;
          } else {
            //MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
            //              "EW = R0 + L0 = %d\n", eyeWidth));
            offset = eyeHeight * Multiplier * ((2 * results[vIndexCenter].strobe[ch][(*rankList)[rank].rankIndex][strobe].p) +
                                  (2 * results[vIndexCenter].strobe[ch][(*rankList)[rank].rankIndex][strobe].n));
            //MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
            //              "Center = EH * (2*R0 + 2*L0) = %d\n", offset));
            offset += ((((10 * eyeWidth) / 3) + ((10 * 2 * eyeHeight) / 3)) / 10) *
                      (results[vIndexCenter + 1].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                       results[vIndexCenter - 1].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                       results[vIndexCenter + 1].strobe[ch][(*rankList)[rank].rankIndex][strobe].n +
                       results[vIndexCenter - 1].strobe[ch][(*rankList)[rank].rankIndex][strobe].n);
            //MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
            //              "Center += (1/3EW + 2/3EH) * (R+1 + R-1 + L+1 + L-1) = %d\n", offset));
            offset += ((((10 * 2 * eyeWidth) / 3) + ((10 * eyeHeight) / 3)) / 10)*
                      (results[vIndexCenter + 2].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                       results[vIndexCenter - 2].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                       results[vIndexCenter + 2].strobe[ch][(*rankList)[rank].rankIndex][strobe].n +
                       results[vIndexCenter - 2].strobe[ch][(*rankList)[rank].rankIndex][strobe].n);
            //MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
            //              "Center += (2/3EW + 1/3EH) * (R+2 + R-2 + L+2 + L-2) = %d\n", offset));
            offset += eyeWidth *
                      (results[vIndexCenter + 3].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                       results[vIndexCenter - 3].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                       results[vIndexCenter + 3].strobe[ch][(*rankList)[rank].rankIndex][strobe].n +
                       results[vIndexCenter - 3].strobe[ch][(*rankList)[rank].rankIndex][strobe].n);
            //MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
            //              "Center += EW * (R+3 + R-3 + L+3 + L-3) = %d\n", offset));
            offset1 = 100 * ((offset / ((4 * eyeWidth) + (4 * eyeHeight))) / 2);
            offset2 = ((100 * offset) / ((4* eyeWidth) + (4 * eyeHeight))) / 2;
            if ((offset2 - offset1) >= 50) {
              offset = (offset1 / 100) + 1;
            } else {
              offset = offset1 / 100;
            }
            //MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
            //              "Center = Center / (4*EW + 4*EH) / 2 = %d\n", offset));
          }
          if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
            temp = offset - timingCenter;
            if (temp < 0) {
              temp = -temp;
            }
            if(temp > maxOffsetFromTimingCenter) {
              if(offset < 0) {
                MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT, "Truncated: %2d -> %2d\n", offset, timingCenter - maxOffsetFromTimingCenter));
                offset = timingCenter - maxOffsetFromTimingCenter;
              } else {
                MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT, "Truncated: %2d -> %2d\n", offset, timingCenter + maxOffsetFromTimingCenter));
                offset = timingCenter + maxOffsetFromTimingCenter;
              }
            }
          }

#ifdef SERIAL_DBG_MSG
          if (checkMsgLevel(host, SDBG_MAX)) {
            GetSetDataGroup (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, group, GSM_READ_CSR | GSM_READ_ONLY, &oldDelay[strobe]);
          }
#endif
          // Apply new center
          GetSetDataGroup (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, group, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &offset);
#ifdef SERIAL_DBG_MSG
          if (checkMsgLevel(host, SDBG_MAX)) {
            GetSetDataGroup (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, group, GSM_READ_ONLY, &delay[strobe]);
          }
#endif
        } // strobe loop
#ifdef SERIAL_DBG_MSG

        if (checkMsgLevel(host, SDBG_MAX)) {
          rcPrintf ((host, "Results for "));
          DisplayDev (host, socket, ch, dimm, rank, NO_STROBE, NO_BIT);
          rcPrintf ((host, "\n"));
          rcPrintf ((host, "        "));
          for (strobe = 0; strobe < MAX_STROBE; strobe++) {
            if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
            rcPrintf ((host, " %3d", strobe));
          } // strobe loop
          rcPrintf ((host, "\n"));
          PrintLine(host, 79, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

          for (vIndex = numPoints; vIndex > 0; vIndex--) {
            margin = -maxVref + ((vIndex - 1) * stepSize);
            rcPrintf ((host, "Vref offset: %d\n", margin));
            // Left Edge
            rcPrintf ((host, "Left:   "));
            for (strobe = 0; strobe < MAX_STROBE; strobe++) {
              if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
              rcPrintf ((host, " %3d", results[(vIndex - 1)].strobe[ch][(*rankList)[rank].rankIndex][strobe].n));
            } // strobe loop

            // Right Edge
            rcPrintf ((host, "\nRight:  "));
            for (strobe = 0; strobe < MAX_STROBE; strobe++) {
              if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
              rcPrintf ((host, " %3d", results[(vIndex - 1)].strobe[ch][(*rankList)[rank].rankIndex][strobe].p));
            } // strobe loop
            rcPrintf ((host, "\n"));
          } // vIndex loop

          PrintLine(host, 79, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

          // Previous delay
          rcPrintf ((host, "Prev Pi:"));
          for (strobe = 0; strobe < MAX_STROBE; strobe++) {
            if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
            rcPrintf ((host, " %3d", oldDelay[strobe]));
          } // strobe loop

          // Print new delay
          rcPrintf ((host, "\nNew Pi: "));
          for (strobe = 0; strobe < MAX_STROBE; strobe++) {
            if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
            rcPrintf ((host, " %3d", delay[strobe]));
          } // strobe loop

          // Print difference
          rcPrintf ((host, "\nDiff:   "));
          for (strobe = 0; strobe < MAX_STROBE; strobe++) {
            if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
            rcPrintf ((host, " %3d", delay[strobe] - oldDelay[strobe]));
          } // strobe loop

          rcPrintf ((host, "\n\n"));
        }
#endif  // SERIAL_DBG_MSG
      } // rank loop
    } // dimm loop
  } // ch loop
#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) {
    if (group == RxDqsDelay) {
      rcPrintf ((host, "STOP_DATA_RX_DQS_ADV\n"));
    } else if (group == RxDqsPDelay) {
      rcPrintf ((host, "STOP_DATA_RX_DQSP_ADV\n"));
    } else if (group == RxDqsNDelay) {
      rcPrintf ((host, "STOP_DATA_RX_DQSN_ADV\n"));
    } else if (group == TxDqDelay) {
      rcPrintf ((host, "STOP_DATA_TX_DQ_ADV\n"));
    }

    releasePrintFControl (host);
  }
#endif  // SERIAL_DBG_MSG

} // AdvancedCentering

UINT32
RdAdvancedCentering (
  PSYSHOST  host
  )
/*++

  Re-centers read dq timing using a more robust test

  @param host - Pointer to sysHost

  @retval N/A

--*/
{
  if (~host->memFlows & MF_RD_ADV_EN) return SUCCESS;

  OutputExtendedCheckpoint((host, STS_CHANNEL_TRAINING, TRAIN_RD_DQS, host->var.mem.currentSocket));
  if (host->setup.mem.optionsExt & DUTY_CYCLE_EN) {
    AdvancedCentering(host, host->var.mem.currentSocket, RxDqsNDelay);
    AdvancedCentering(host, host->var.mem.currentSocket, RxDqsPDelay);
  } else {
    AdvancedCentering(host, host->var.mem.currentSocket, RxDqsDelay);
  }

  return SUCCESS;
} // RdAdvancedCentering

UINT32
WrAdvancedCentering (
  PSYSHOST  host
  )
/*++

  Re-centers tx dq timing using a more robust test

  @param host - Pointer to sysHost

  @retval SUCCESS

--*/
{
  if (~host->memFlows & MF_WR_ADV_EN) return SUCCESS;

  OutputExtendedCheckpoint((host, STS_CHANNEL_TRAINING, TRAIN_WR_DQS, host->var.mem.currentSocket));
  AdvancedCentering(host, host->var.mem.currentSocket, TxDqDelay);

  return SUCCESS;
} // WrAdvancedCentering

UINT8
VrefQuick (
  PSYSHOST  host,
  UINT8     socket,
  GSM_GT    group,
  INT16     *low,
  INT16     *high
  )
/*++

  Quick centering of Vref

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param group   - Parameter to center
                   RxDqsDelay   0: Center Rx DqDqs
                   TxDqDelay    1: Center Tx DqDqs
  @param low     - Pointer to worst low
  @param high    - Pointer to worst high

  @retval UINT8   - Vref eye height

--*/
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT16              mode;
  UINT32              patternLength;
  INT16               compLow = 0;
  INT16               compHigh = 0;
  INT16               worstLow = 0;
  INT16               worstHigh = 0;
  //INT16               offset[MAX_RANK_CH];
  struct bitMask      filter;
  struct rankMargin   resultsRank;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8               rankPerChannel;

  channelNvList = GetChannelNvList(host, socket);

  patternLength = 64;
  mode = MODE_VIC_AGG;

  // Setup bitMask
  for (ch = 0; ch < MAX_CH; ch++) {
    for (rankPerChannel = 0; rankPerChannel < MAX_RANK_CH; rankPerChannel++ ) {
      resultsRank.rank[ch][rankPerChannel].n = 0;
      resultsRank.rank[ch][rankPerChannel].p = 0;
      //offset[rankPerChannel] = 0;
    }
  } // ch loop

  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter(host, socket, &filter);

  GetMargins (host, socket, DdrLevel, group, mode, SCOPE_RANK, &filter, (void *)&resultsRank, patternLength, 0, 0, 0,
              WDB_BURST_LENGTH);

  //if (group == TxVref) {
  //  compLow = -0xFF;
  //  compHigh = 0xFF;
 // }

  worstLow = -0xFF;
  worstHigh = 0xFF;

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    //if (group == RxVref) {
      compLow = -0xFF;
      compHigh = 0xFF;
    //}

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankList = GetRankNvList(host, socket, ch, dimm);
      for (rank = 0; rank < (*channelNvList)[ch].dimmList[dimm].numRanks; rank++) {
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        if (resultsRank.rank[ch][(*rankList)[rank].rankIndex].n > resultsRank.rank[ch][(*rankList)[rank].rankIndex].p) {
          resultsRank.rank[ch][(*rankList)[rank].rankIndex].n = 0;
          resultsRank.rank[ch][(*rankList)[rank].rankIndex].p = 0;
        }
        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                      "High = %d - Low = %d\n",
                      resultsRank.rank[ch][(*rankList)[rank].rankIndex].p, resultsRank.rank[ch][(*rankList)[rank].rankIndex].n));

        if (compLow < resultsRank.rank[ch][(*rankList)[rank].rankIndex].n) {
          compLow = resultsRank.rank[ch][(*rankList)[rank].rankIndex].n;
        }
        if (compHigh > resultsRank.rank[ch][(*rankList)[rank].rankIndex].p) {
          compHigh = resultsRank.rank[ch][(*rankList)[rank].rankIndex].p;
        }
        //offset[(*rankList)[rank].rankIndex] = (resultsRank.rank[ch][(*rankList)[rank].rankIndex].n +
        //      resultsRank.rank[ch][(*rankList)[rank].rankIndex].p) / 2;
      } // rank loop
    } // dimm loop

    if (worstLow < compLow) {
      worstLow = compLow;
    }
    if (worstHigh > compHigh) {
      worstHigh = compHigh;
    }
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Composite High = %d - Composite Low = %d\n", compHigh, compLow));
    //}
  } // ch loop

  if ((group == TxVref) && (host->nvram.mem.dramType != SPD_TYPE_DDR3)) {
    UpdateSafeTxVref(host, socket, DdrLevel);
  }
  //
  // Reset the IO
  //
  IO_Reset (host, socket);

  *low = worstLow;
  *high = worstHigh;
  if (worstLow > worstHigh) {
    return 0;
  } else {
    return (UINT8)(worstHigh - worstLow);
  }
} // VrefQuick

/*++

  Early Vref centering

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param group   - Parameter to center
                   RxVref    0: Center Rx Vref
                   TxVref    1: Center Tx Vref

  @retval SUCCESS

--*/
UINT32
EarlyVrefCentering (
  PSYSHOST  host,
  UINT8     socket,
  GSM_GT    group
  )
{
  UINT8               ch;
  UINT16              eyesize=0;
  UINT8               dimm;
  UINT8               rank;
  UINT8               rank2;
  UINT8               dimm2 = 0;
  UINT8               strobe;
  UINT8               done = 0;
  UINT8               chDoneMask = 0;
  UINT8               dimmLoopBreak = 0;
  UINT8               dimmDone[MAX_CH][MAX_DIMM];
  UINT8               rankDone[MAX_CH][MAX_DIMM][MAX_RANK_DIMM];
  UINT8               counter[MAX_CH][MAX_DIMM][MAX_RANK_DIMM];
  INT16               offset[MAX_CH][MAX_DIMM][MAX_RANK_DIMM];
  INT16               inital[MAX_CH][MAX_DIMM][MAX_RANK_DIMM];
  INT16               setting[MAX_CH][MAX_DIMM][MAX_RANK_DIMM];
  INT16               tOffset;
  INT16               counterSign = 1;
  INT16               lowMargin;
  INT16               highMargin;
  UINT32              status;
  GSM_GT              tGroup = 0;
  struct bitMask      filter;
  struct rankMargin   rankResults;
  struct strobeMargin results;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  channelNvList = GetChannelNvList(host, socket);

  //
  //This algorithm is only for DDR4 DIMMs.  If DDR3 DIMMs present, just exit.
  //
  if ((host->nvram.mem.dramType == SPD_TYPE_DDR3) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return SUCCESS;

#ifdef SERIAL_DBG_MSG
  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
               "\nPrevious Settings "));

  if (group == RxVref) {
    DisplayResults(host, socket, RxVref);
    DisplayResults(host, socket, RxDqsDelay);
  } else if (group == TxVref) {
    DisplayResults(host, socket, TxVref);
    DisplayResults(host, socket, TxDqDelay);
  } else {
    // Error
  }
#endif //SERIAL_DBG_MSG

  if (group == RxVref) {
    tGroup = RxDqsDelay;
  } else if (group == TxVref) {
    tGroup = TxDqDelay;
  } else {
  // Error
  }

  ClearFilter(host, socket, &filter);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    dimmNvList = GetDimmNvList(host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      dimmDone[ch][dimm] = 0;
      rankList = GetRankNvList(host, socket, ch, dimm);
      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
        counter[ch][dimm][rank] = 0;
        offset[ch][dimm][rank] = 0;
        setting[ch][dimm][rank] = 0;
        GetSetDataGroup (host, socket, ch, dimm, rank, ALL_STROBES, ALL_BITS, DdrLevel, group, GSM_READ_ONLY, &inital[ch][dimm][rank]);
        rankDone[ch][dimm][rank] = 0;
      } // rank loop
    } // dimm loop
  } // ch loop

  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "\n"));


  //
  // Get per rank eye width
  // W/A for unusually low or high margins s4986776. Try multiple vref offsets until a timing eye is found.
  //
  while (!done) {
    GetMargins (host, socket, DdrLevel, tGroup, MODE_VIC_AGG, SCOPE_STROBE, &filter, &results, 64, 0, 0, 0,
                WDB_BURST_LENGTH);

    done = 1;
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      if (chDoneMask & (1 << ch)) continue;
      chDoneMask |= (1 << ch);

      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Cycling through ranks to check eye width for 2DPC margining W/A!!\n"));
      dimmNvList = GetDimmNvList(host, socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if (dimmDone[ch][dimm]) continue;
        if (group == TxVref) {
          dimmDone[ch][dimm] = 1;
        }
        rankList = GetRankNvList(host, socket, ch, dimm);
        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

          //
          // Skip if this rank is done
          //
          if (rankDone[ch][dimm][rank]) continue;

          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                        "Checking for eye widths!! Current VREF offset is %d\n", offset[ch][dimm][rank]));
          status = SUCCESS;
          for (strobe = 0; strobe < MAX_STROBE; strobe++) {
            if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
            eyesize = (results.strobe[ch][(*rankList)[rank].rankIndex][strobe].p - results.strobe[ch][(*rankList)[rank].rankIndex][strobe].n);

            if (eyesize < MIN_EARLYVREF_EYE) {
              status = FAILURE;
              break;
            }
          } //strobe loop

          if (status == FAILURE) {
            chDoneMask &= ~(1 << ch);
            dimmDone[ch][dimm] = 0;
            if (counter[ch][dimm][rank] & BIT0) {
              counterSign = -1;
            } else {
              counterSign = 1;
            }
            offset[ch][dimm][rank] = offset[ch][dimm][rank] + counterSign * 5 * (counter[ch][dimm][rank] + 1);

            setting[ch][dimm][rank] = inital[ch][dimm][rank] + offset[ch][dimm][rank];
            GetSetDataGroup (host, socket, ch, dimm, rank, ALL_STROBES, ALL_BITS, DdrLevel, group, GSM_UPDATE_CACHE, &setting[ch][dimm][rank]);

            counter[ch][dimm][rank]++;
            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                          "Eye width %2d detected!! Setting VREF offset to %d using counter %d...\n",
                          eyesize, offset[ch][dimm][rank],counter[ch][dimm][rank]));
            done = 0;
            if (counter[ch][dimm][rank] > MAX_VREF_SETTINGS) {
              if(group == TxVref) {
                MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                            "Unable to find the TxVref for the eye!\n"));
              } else if (group == RxVref) {
                MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                            "Unable to find the RxVref for the eye!\n"));
              }
              //
              // Map out channel
              //
              DisableChannel(host, socket, ch);
              //
              // Add this error to the warning log for both correctable and uncorrectable errors.
              //
              OutputWarning (host, WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_VREF_TRAINING, socket, ch, dimm, rank);
            }
            // If this is Tx (and not LRDIMM), then continue to the next rank. If Rx, then we need to set the vref offset
            // and break to the channel loop. Otherwise, we will get multiple per-channel RxVref offsets
            // if multiple ranks per channel are failing RxVref.
            if (group == RxVref) {
              // Offsets and counters are shared with the entire channel for Rx
              for (dimm2 = 0; dimm2 < MAX_DIMM; dimm2++) {
                rankList = GetRankNvList(host, socket, ch, dimm2);
                for (rank2 = 0; rank2 < (*dimmNvList)[dimm2].numRanks; rank2++) {

                  // Do not test if this rank is disabled
                  if (CheckRank(host, socket, ch, dimm2, rank2, DONT_CHECK_MAPOUT)) continue;

                  offset[ch][dimm2][rank2] = offset[ch][dimm][rank];
                  counter[ch][dimm2][rank2] = counter[ch][dimm][rank];
                } // rank2 loop
              } // dimm2 loop
              dimmLoopBreak = 1;
              break; // breaks out of rank loop
            } else if (group == TxVref && IsLrdimmPresent(host, socket, ch, dimm)) {
              // If Tx LRDIMM, then we need to set the vref offset
              // and break to the dimm loop.
              // Offsets and counters are shared with the entire DIMM
              rankList = GetRankNvList(host, socket, ch, dimm);
              for (rank2 = 0; rank2 < (*dimmNvList)[dimm].numRanks; rank2++) {
                if (CheckRank(host, socket, ch, dimm, rank2, DONT_CHECK_MAPOUT)) continue;

                // Do not test if this rank is mapped out
                if(rank2 == rank) continue; // Don't re-update what was just done

                offset[ch][dimm][rank2] = offset[ch][dimm][rank];
                counter[ch][dimm][rank2] = counter[ch][dimm][rank];
                setting[ch][dimm][rank2] = inital[ch][dimm][rank2] + offset[ch][dimm][rank2];
                GetSetDataGroup (host, socket, ch, dimm, rank2, ALL_STROBES, ALL_BITS, DdrLevel, group, GSM_UPDATE_CACHE, &setting[ch][dimm][rank2]); // Set the cache values for all ranks in the LRDIMM
              } // rank2 loop
              break;
            }
          } else {

            //
            // Tx Vref is centering per rank
            //
            if (group == TxVref) {
              if (IsLrdimmPresent(host, socket, ch, dimm)) {
                rankDone[ch][dimm][rank] = 0;
              } else {
                rankDone[ch][dimm][rank] = 1;
              }
            }

          }
        } // rank loop
        if (dimmLoopBreak == 1)  { // Allow Rx to break the DIMM/rank loops
          dimmLoopBreak = 0;
          break;
        }
      } // dimm loop

    } // ch loop
  } // while loop

  //
  // Recenter timing based on the new Vref offsets
  //
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    dimmNvList = GetDimmNvList(host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankList = GetRankNvList(host, socket, ch, dimm);
      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        for (strobe = 0; strobe < MAX_STROBE; strobe++) {
          if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
          tOffset = (results.strobe[ch][(*rankList)[rank].rankIndex][strobe].p + results.strobe[ch][(*rankList)[rank].rankIndex][strobe].n) / 2;
          GetSetDataGroup (host, socket, ch, dimm, rank, strobe, ALL_BITS, DdrLevel, tGroup, GSM_UPDATE_CACHE + GSM_WRITE_OFFSET,
                           &tOffset);
        } //strobe loop
      } // rank loop
    } // dimbecm loop
  } // ch loop

  //
  // Recenter Vref at the current timing
  //
  GetMargins (host, socket, DdrLevel, group, MODE_VIC_AGG, SCOPE_RANK, &filter, &rankResults, 64, 0, 0, 0,
              WDB_BURST_LENGTH);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    lowMargin = -0x7F;
    highMargin = 0x7F;

    dimmNvList = GetDimmNvList(host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankList = GetRankNvList(host, socket, ch, dimm);
      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        if (group == TxVref) {
          tOffset = (rankResults.rank[ch][(*rankList)[rank].rankIndex].p + rankResults.rank[ch][(*rankList)[rank].rankIndex].n) / 2;
          GetSetDataGroup (host, socket, ch, dimm, rank, ALL_STROBES, ALL_BITS, DdrLevel, group, GSM_UPDATE_CACHE + GSM_WRITE_OFFSET,
                           &tOffset);
        } else { // calculate composite eye for RX VREF
          if (lowMargin < rankResults.rank[ch][(*rankList)[rank].rankIndex].n) {
            lowMargin = rankResults.rank[ch][(*rankList)[rank].rankIndex].n;
          }

          if (highMargin > rankResults.rank[ch][(*rankList)[rank].rankIndex].p) {
            highMargin = rankResults.rank[ch][(*rankList)[rank].rankIndex].p;
          }
        }
      } // rank loop
    } // dimm loop
    if (group == RxVref) {
      tOffset = (highMargin + lowMargin) / 2;
      GetSetDataGroup (host, socket, ch, 0, 0, ALL_STROBES, ALL_BITS, DdrLevel, group, GSM_UPDATE_CACHE + GSM_WRITE_OFFSET,
                       &tOffset);
    }
  } // ch loop

  if ((group == TxVref) && (host->nvram.mem.dramType != SPD_TYPE_DDR3)) {
    UpdateSafeTxVref(host, socket, DdrLevel);
  }
#ifdef SERIAL_DBG_MSG
  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                         "\nNew Settings"));

  if (group == RxVref) {
    DisplayResults(host, socket, RxVref);
    DisplayResults(host, socket, RxDqsDelay);
  } else if (group == TxVref) {
    DisplayResults(host, socket, TxVref);
    DisplayResults(host, socket, TxDqDelay);
  } else {
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                         "\nERROR - Invalid group\n "));
  }
#endif //SERIAL_DBG_MSG
  return SUCCESS;
} // EarlyVrefCentering

UINT32
RdEarlyVrefCentering (
  PSYSHOST  host
  )
/*++

  Early Vref centering

  @param host    - Pointer to sysHost

  @retval SUCCESS

--*/
{
  if (~host->memFlows & MF_E_RD_VREF_EN  || ((host->setup.mem.optionsExt & RD_VREF_EN) == 0)) return SUCCESS;
  return EarlyVrefCentering(host, host->var.mem.currentSocket, RxVref);
} // RcEarlyVrefCentering

UINT32
WrEarlyVrefCentering (
  PSYSHOST  host
  )
/*++

  Early Vref centering

  @param host    - Pointer to sysHost

  @retval SUCCESS

--*/
{
  if (~host->memFlows & MF_E_WR_VREF_EN || ((host->setup.mem.optionsExt & WR_VREF_EN) == 0)) return SUCCESS;
  return EarlyVrefCentering(host, host->var.mem.currentSocket, TxVref);
} // WrEarlyVrefCentering

UINT32
CmdVrefCentering (
  PSYSHOST  host
  )
/*++

  Re-centers using a more robust test

  @param host    - Pointer to sysHost

  @retval SUCCESS

--*/
{
  if ((~host->memFlows & MF_CMD_VREF_EN) || ((host->setup.mem.optionsExt & CMD_VREF_EN) == 0)) return SUCCESS;
  //if ((~host->memFlows & MF_CMD_VREF_EN)) return SUCCESS;

  if (!(host->nvram.mem.dramType == SPD_TYPE_DDR3))  SetNormalCMDTiming(host, host->var.mem.currentSocket);

  OutputExtendedCheckpoint((host, STS_CHANNEL_TRAINING, CMD_VREF_CENTERING, host->var.mem.currentSocket));
  return CmdVrefQuick(host, host->var.mem.currentSocket, CmdVref);

} // CMDVrefCentering

UINT32
RdVrefCentering (
                PSYSHOST  host
                )
/*++

  Re-centers using a more robust test

  @param host    - Pointer to sysHost

  @retval N/A

--*/
{
  if (~host->memFlows & MF_RD_VREF_EN) return SUCCESS;
  if ((host->setup.mem.optionsExt & RD_VREF_EN) == 0) return SUCCESS;

  OutputExtendedCheckpoint((host, STS_CHANNEL_TRAINING, TRAIN_RD_VREF, host->var.mem.currentSocket));
  return VrefCentering(host, host->var.mem.currentSocket, RxVref);
} // RdVrefCentering

UINT32
WrVrefCentering (
                PSYSHOST  host
                )
/*++

  Re-centers using a more robust test

  @param host    - Pointer to sysHost

  @retval SUCCESS

--*/
{
  if (~host->memFlows & MF_WR_VREF_EN) return SUCCESS;
  if ((host->setup.mem.optionsExt & WR_VREF_EN) == 0) return SUCCESS;

  OutputExtendedCheckpoint((host, STS_CHANNEL_TRAINING, TRAIN_WR_VREF, host->var.mem.currentSocket));
  return VrefCentering(host, host->var.mem.currentSocket, TxVref);
} // WrVrefCentering

UINT32
WrLRDIMMVrefCentering (
  PSYSHOST  host
  )
/*++

  Re-centers using a more robust test

  @param host    - Pointer to sysHost

  @retval SUCCESS

--*/
{

  if ((~host->memFlows & MF_L_WR_VREF_EN) || ((host->setup.mem.optionsExt & LRDIMM_BACKSIDE_VREF_EN) == 0) ||
      ((host->setup.mem.optionsExt & LRDIMM_WR_VREF_EN) == 0)) return SUCCESS;
  return  LRDIMMVrefCentering(host, host->var.mem.currentSocket, TxVref, LrbufLevel, 1);

} // WrVrefCentering

UINT32
RdLRDIMMVrefCentering (
  PSYSHOST  host
  )
/*++

  Re-centers using a more robust test

  @param host    - Pointer to sysHost

  @retval SUCCESS

--*/
{

  if ((~host->memFlows & MF_L_WR_VREF_EN) ||
      ((host->setup.mem.optionsExt & LRDIMM_BACKSIDE_VREF_EN) == 0) ||
      ((host->setup.mem.optionsExt & LRDIMM_RD_VREF_EN) == 0)) return SUCCESS;

  return  LRDIMMVrefCentering(host, host->var.mem.currentSocket, RxVref, LrbufLevel, 1);

} // RdVrefCentering

UINT32
TxLRDIMMDqCentering (
  PSYSHOST  host
  )
/*++

  Re-centers using a more robust test

  @param host    - Pointer to sysHost

  @retval SUCCESS

--*/
{

  if ((~host->memFlows & MF_L_WR_VREF_EN) ||
      ((host->setup.mem.optionsExt & LRDIMM_BACKSIDE_VREF_EN) == 0) ||
      ((host->setup.mem.optionsExt & LRDIMM_TX_DQ_CENTERING) == 0)) return SUCCESS;

  return  LRDIMMVrefCentering(host, host->var.mem.currentSocket, TxDqDelay, LrbufLevel, 1);

} // TxDqCentering

UINT32
RxLRDIMMDqCentering (
  PSYSHOST  host
  )
/*++

  Re-centers using a more robust test

  @param host    - Pointer to sysHost

  @retval SUCCESS

--*/
{

  if ((~host->memFlows & MF_L_WR_VREF_EN) ||
      ((host->setup.mem.optionsExt & LRDIMM_BACKSIDE_VREF_EN) == 0) ||
      ((host->setup.mem.optionsExt & LRDIMM_RX_DQ_CENTERING) == 0)) return SUCCESS;

  return  LRDIMMVrefCentering(host, host->var.mem.currentSocket, RxDqsDelay, LrbufLevel, 1);

} // RxDqCentering

UINT32
LRDIMMVrefCentering (
  PSYSHOST  host,
  UINT8     socket,
  GSM_GT    group,
  GSM_LT    level,
  UINT8     perRankCentering
  )
/*++

  VRef Centering for LRDIMM (DDR4)

  @param host                - Pointer to sysHost
  @param socket              - Processor socket to check
  @param group               - Parameter to be margined
  @param level               - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param perRankCentering    - 0 = false, 1 = true (if false, PDA training is done)

  @retval SUCCESS

--*/
{
  UINT8               ch;
  UINT8               MaxSubRankCh = 0;
  UINT8               dimm;
  UINT8               rank;
  UINT8               subRank;
  UINT8               maxSubRank[MAX_CH];
  UINT8               strobe;
  UINT8               maxStrobe;
  INT16               vlo;
  INT16               vhi;
  UINT16              mode;
  UINT32              patternLength = 1;
  INT16               offset=0;
  struct bitMask      filter;
  struct rankMargin   marginRank[MAX_SUBRANK];
  struct strobeMargin marginStrobe[MAX_SUBRANK];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];

  //
  // Return under any of these conditions.
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) ||
     (host->nvram.mem.socket[socket].maxDimmPop == 0) ||
      (host->nvram.mem.dramType == SPD_TYPE_DDR3) ||
      (host->nvram.mem.socket[host->var.mem.currentSocket].lrDimmPresent == 0)) return SUCCESS;

  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "Starting LRDIMM backside Training. \n"));

  channelNvList = GetChannelNvList(host, socket);

  // Clearout margin data structure
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((host->nvram.mem.socket[socket].channelList[ch].encodedCSMode == 1)) {
      maxSubRank[ch] = MAX_SUBRANK;
    } else {
      maxSubRank[ch] = 1;
    }
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankList = GetRankNvList(host, socket, ch, dimm);
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        for (subRank = 0; subRank < maxSubRank[ch]; subRank++) {
          for (strobe = 0; strobe < MAX_STROBE; strobe++) {
            marginStrobe[subRank].strobe[ch][(*rankList)[rank].rankIndex][strobe].n = 0;
            marginStrobe[subRank].strobe[ch][(*rankList)[rank].rankIndex][strobe].p = 0;
          } // strobe
          marginRank[subRank].rank[ch][rank].n = 0;
          marginRank[subRank].rank[ch][rank].p = 0;
        } // subRank
      } // rank loop
    } // dimm loop
  } // ch loop

  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter(host, socket, &filter);

  // Get pattern length
  mode = MODE_VIC_AGG;
  if (group == RxVref) {
      patternLength = TRAIN_RD_DQS_PL;
      maxStrobe = MAX_STROBE/2;   // Buffers are x8 level
  } else {
      patternLength = TRAIN_WR_DQS_PL;
      maxStrobe = MAX_STROBE;     // ALL LRDIMM are x4 DRAMS
  }

  for (ch = 1; ch < MAX_CH; ch++) {
    if(maxSubRank[ch] > maxSubRank[MaxSubRankCh]) {
      MaxSubRankCh = ch;
    }
  }

  for (subRank = 0; subRank < maxSubRank[MaxSubRankCh]; subRank++) {
    host->var.mem.currentSubRank = subRank;

    if ((perRankCentering == 1) || ((group == TxVref) && ((host->setup.mem.optionsExt & PDA_EN) == 0))) { // Per rank
      GetMargins (host, socket, level, group, mode, SCOPE_RANK, &filter, &marginRank[subRank], patternLength, 0, 0, 0, WDB_BURST_LENGTH);
    } else { // Per strobe
      GetMargins (host, socket, level, group, mode, SCOPE_STROBE, &filter, &marginStrobe[subRank], patternLength, 0, 0, 0, WDB_BURST_LENGTH);
    }
  } // subRank loop
  host->var.mem.currentSubRank = 0;

  if (group == RxVref) {
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "START_BACKSIDE_RX_VREF_CENTER\n"));
  } else if (group == TxVref) {
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "START_BACKSIDE_TX_VREF_CENTER\n"));
  } else if (group == TxDqDelay) {
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "START_BACKSIDE_TX_DQ_CENTER\n"));
  } else if (group == RxDqsDelay) {
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "START_BACKSIDE_RX_DQ_CENTER\n"));
  }

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    dimmNvList = GetDimmNvList(host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if (((*dimmNvList)[dimm].dimmPresent == 0) || ((*dimmNvList)[dimm].aepDimmPresent)) continue;
      if (IsLrdimmPresent(host, socket, ch, dimm) == 0) continue;
      rankList = GetRankNvList(host, socket, ch, dimm);

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
        for (subRank = 0; subRank < maxSubRank[ch]; subRank++) {
          if ((perRankCentering == 1) || ((group == TxVref) && ((host->setup.mem.optionsExt & PDA_EN) == 0))) { // Per rank
            if (marginRank[subRank].rank[ch][(*rankList)[rank].rankIndex].n > marginRank[subRank].rank[ch][(*rankList)[rank].rankIndex].p) {
              marginRank[subRank].rank[ch][(*rankList)[rank].rankIndex].p = 0;
              marginRank[subRank].rank[ch][(*rankList)[rank].rankIndex].n = 0;
            }
          } else { // Per strobe
            for (strobe = 0; strobe < maxStrobe; strobe++) {
              if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

              if (marginStrobe[subRank].strobe[ch][(*rankList)[rank].rankIndex][strobe].n >
                  marginStrobe[subRank].strobe[ch][(*rankList)[rank].rankIndex][strobe].p) {

                marginStrobe[subRank].strobe[ch][(*rankList)[rank].rankIndex][strobe].p = 0;
                marginStrobe[subRank].strobe[ch][(*rankList)[rank].rankIndex][strobe].n = 0;
              }
              MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, (rank * maxSubRank[ch]) + subRank, strobe, NO_BIT, " %3d : %3d\n", marginStrobe[subRank].strobe[ch][(*rankList)[rank].rankIndex][strobe].n, marginStrobe[subRank].strobe[ch][(*rankList)[rank].rankIndex][strobe].p));
            } // strobe loop
          } // if per strobe
        } // subRank
      } // rank loop
    } // dimm loop
  } // ch loop

  if (group == RxVref) {
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "STOP_BACKSIDE_RX_VREF_CENTER\n"));
  } else if (group == TxVref) {
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "STOP_BACKSIDE_TX_VREF_CENTER\n"));
  } else if (group == TxDqDelay) {
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "STOP_BACKSIDE_TX_DQ_CENTER\n"));
  } else if (group == RxDqsDelay) {
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "STOP_BACKSIDE_RX_DQ_CENTER\n"));
  }

  //
  //  program final values
  //
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    dimmNvList = GetDimmNvList(host, socket, ch);

    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "                 vLo   vHi   offset\n"));

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      // reset the composite eye for per dimm results
      vlo = -0xff;
      vhi = 0xff;

      if (((*dimmNvList)[dimm].dimmPresent == 0) || ((*dimmNvList)[dimm].aepDimmPresent)) continue;
      if (IsLrdimmPresent(host, socket, ch, dimm) == 0) continue;
      rankList = GetRankNvList(host, socket, ch, dimm);

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        for (subRank = 0; subRank < maxSubRank[ch]; subRank++) {
          // reset the composite eye for per rank results
          if ((perRankCentering == 1) || ((group == TxVref) && ((host->setup.mem.optionsExt & PDA_EN) == 0))) { // Per rank
            vhi = marginRank[subRank].rank[ch][(*rankList)[rank].rankIndex].p;
            vlo = marginRank[subRank].rank[ch][(*rankList)[rank].rankIndex].n;
            offset = (vhi+vlo) / 2;

            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, (rank * maxSubRank[ch]) + subRank, NO_STROBE, NO_BIT, " %3d     %3d      %3d\n", vlo, vhi, offset));
            GetSetDataGroup (host, socket, ch, dimm, (rank * maxSubRank[ch]) + subRank, ALL_STROBES, ALL_BITS, level, group,
                             GSM_UPDATE_CACHE | GSM_WRITE_OFFSET | GSM_FORCE_WRITE, &offset);
          } else { // Per strobe
            for (strobe = 0; strobe < maxStrobe; strobe++) {
              if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
              vhi = marginStrobe[subRank].strobe[ch][(*rankList)[rank].rankIndex][strobe].p;
              vlo = marginStrobe[subRank].strobe[ch][(*rankList)[rank].rankIndex][strobe].n;
              offset = (vhi+vlo) / 2;

              MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, (rank * maxSubRank[ch]) + subRank, NO_STROBE, NO_BIT, " %3d     %3d      %3d\n", vlo, vhi, offset));
              GetSetDataGroup (host, socket, ch, dimm, (rank * maxSubRank[ch]) + subRank, strobe, ALL_BITS, level, group,
                               GSM_UPDATE_CACHE | GSM_WRITE_OFFSET | GSM_FORCE_WRITE, &offset);
            } // strobe loop
          } // if per rank / per strobe
        } // subRank loop
      } // rank loop
    } // dimm loop
  } // ch loop

  IO_Reset (host, socket);

  if ((group == TxVref) && (host->setup.mem.optionsExt & PDA_EN)) {
    host->var.mem.txVrefBacksideDone = 1;
    UpdateSafeTxVref(host, socket, level);
  }

  return SUCCESS;
} // LRDIMMVrefCentering

UINT32
VrefCentering (
              PSYSHOST  host,
              UINT8     socket,
              GSM_GT    group
              )
/*++

  Re-centers using a more robust test

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param group   - Parameter to center
                   RxDqsDelay   0: Center Rx DqDqs
                   TxDqDelay    1: Center Tx DqDqs

  @retval SUCCESS

--*/
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
#ifdef SERIAL_DBG_MSG
  UINT8               maxStrobe;
#endif // SERIAL_DBG_MSG
  INT16               tOffset[MAX_CH][MAX_RANK_CH][3];
  GSM_GT              tGroup = 0;
  UINT16              mode;
  UINT8               timeIndex;
  UINT32              patternLength = 1;
  struct bitMask      filter;
  struct strobeMargin vrefMargin[3];
  struct rankMargin   eyeWidth;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return SUCCESS;

  channelNvList = GetChannelNvList(host, socket);

  //
  // RxVref Tuning
  //
  RxVrefTuning (host, socket, group);

  if (group == RxVref) {
    tGroup = RxDqsDelay;
  } else if (group == TxVref) {
    tGroup = TxDqDelay;
  } else {
    // Error
  }

  // Setup bitMask
  for (ch = 0; ch < MAX_CH; ch++) {
    for (rank = 0; rank < MAX_RANK_CH; rank++) {
      eyeWidth.rank[ch][rank].n = 0;
      eyeWidth.rank[ch][rank].p = 0;
      tOffset[ch][rank][0] = 0;
      tOffset[ch][rank][1] = 0;
      tOffset[ch][rank][2] = 0;

      for (strobe = 0; strobe < MAX_STROBE; strobe++) {
        for (timeIndex = 0; timeIndex < 3; timeIndex++) {
          vrefMargin[timeIndex].strobe[ch][rank][strobe].n = 0;
          vrefMargin[timeIndex].strobe[ch][rank][strobe].p = 0;
        }
      } // strobe loop
    } // rank loop
  } // ch loop

  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter(host, socket, &filter);

  //
  // Get per rank eye width
  //
  GetMargins (host, socket, DdrLevel, tGroup, MODE_VIC_AGG, SCOPE_RANK, &filter, &eyeWidth, 64, 0, 0, 0,
              WDB_BURST_LENGTH);

  //
  // Determine the timing offsets per rank
  //
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "Timing offsets to test\n"));
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "tOffset      0     1    2\n"));
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      rankList = GetRankNvList(host, socket, ch, dimm);

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        if (eyeWidth.rank[ch][(*rankList)[rank].rankIndex].n > eyeWidth.rank[ch][(*rankList)[rank].rankIndex].p) {
          eyeWidth.rank[ch][(*rankList)[rank].rankIndex].n = 0;
          eyeWidth.rank[ch][(*rankList)[rank].rankIndex].p = 0;
        }

        // T = min(12, EW/2)/3
        if (12 <= ((eyeWidth.rank[ch][(*rankList)[rank].rankIndex].p - eyeWidth.rank[ch][(*rankList)[rank].rankIndex].n) / 2)) {
          tOffset[ch][(*rankList)[rank].rankIndex][0] = -12/3;
          tOffset[ch][(*rankList)[rank].rankIndex][1] = 12/3;
        } else {
          tOffset[ch][(*rankList)[rank].rankIndex][0] = -((eyeWidth.rank[ch][(*rankList)[rank].rankIndex].p - eyeWidth.rank[ch][(*rankList)[rank].rankIndex].n) / 6);
          tOffset[ch][(*rankList)[rank].rankIndex][1] = (eyeWidth.rank[ch][(*rankList)[rank].rankIndex].p - eyeWidth.rank[ch][(*rankList)[rank].rankIndex].n) / 6;
        }
        tOffset[ch][(*rankList)[rank].rankIndex][2] = 0;
        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                      "     %3d   %3d   %3d\n", tOffset[ch][(*rankList)[rank].rankIndex][0], tOffset[ch][(*rankList)[rank].rankIndex][1], tOffset[ch][(*rankList)[rank].rankIndex][2]));
      } // rank loop
    } // dimm loop
  } // ch loop

  //
  // Get per strobe Vref margins at 3 timing settings
  //
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "Get vref margins at 3 timing points\n"));
  // Get pattern length
  mode = MODE_VIC_AGG;
  patternLength = TRAIN_RD_DQS_PL;

  // Set timing offset per rank
  for (timeIndex = 0; timeIndex < 3; timeIndex++) {
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        rankList = GetRankNvList(host, socket, ch, dimm);

        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

          GetSetDataGroup (host, socket, ch, dimm, rank, ALL_STROBES, 0, DdrLevel, tGroup, GSM_WRITE_OFFSET | GSM_FORCE_WRITE,
                           &tOffset[ch][(*rankList)[rank].rankIndex][timeIndex]);

        } // rank loop
      } // dimm loop
    } // ch loop

    //
    // Get Vref margin at this timing offset
    //
    GetMargins (host, socket, DdrLevel, group, mode, SCOPE_STROBE, &filter, &vrefMargin[timeIndex], patternLength, 0, 0, 0,
                WDB_BURST_LENGTH);
  } // timeIndex loop

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) {
    getPrintFControl (host);

    if (group == RxVref) {
      rcPrintf ((host, "START_RX_VREF_CENTER\n"));
    } else {
      rcPrintf ((host, "START_TX_VREF_CENTER\n"));
    }

    for (ch = 0; ch < MAX_CH; ch++) {
      //
      // Skip if channel is disabled
      //
      if ((*channelNvList)[ch].enabled == 0) continue;
      dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        //
        // Skip if no DIMM present
        //
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
        maxStrobe = GetMaxStrobe(host, socket, ch, dimm, group, DdrLevel);

        rankList = GetRankNvList(host, socket, ch, dimm);

        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          //
          // Skip if no rank or if it is mapped out
          //

          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
          MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "vref Margins      -          0          +\n"));

          for (strobe = 0; strobe < maxStrobe; strobe++) {
            //
            // Skip if this is an ECC strobe when ECC is disabled
            //

            if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;


            for (timeIndex = 0; timeIndex < 3; timeIndex++) {
              if (vrefMargin[timeIndex].strobe[ch][(*rankList)[rank].rankIndex][strobe].n >
                  vrefMargin[timeIndex].strobe[ch][(*rankList)[rank].rankIndex][strobe].p) {

                vrefMargin[timeIndex].strobe[ch][(*rankList)[rank].rankIndex][strobe].p = 0;
                vrefMargin[timeIndex].strobe[ch][(*rankList)[rank].rankIndex][strobe].n = 0;
              }
            } // timeIndex loop
            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                           " %3d : %3d,  %3d : %3d,  %3d : %3d\n", vrefMargin[0].strobe[ch][(*rankList)[rank].rankIndex][strobe].n,
                           vrefMargin[0].strobe[ch][(*rankList)[rank].rankIndex][strobe].p,
                           vrefMargin[2].strobe[ch][(*rankList)[rank].rankIndex][strobe].n,
                           vrefMargin[2].strobe[ch][(*rankList)[rank].rankIndex][strobe].p,
                           vrefMargin[1].strobe[ch][(*rankList)[rank].rankIndex][strobe].n,
                           vrefMargin[1].strobe[ch][(*rankList)[rank].rankIndex][strobe].p));
          } // strobe loop
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "\n"));
        } // rank loop
      } // dimm loop
    } // ch loop

    if (group == RxVref) {
      rcPrintf ((host, "STOP_RX_VREF_CENTER\n"));
    } else {
      rcPrintf ((host, "STOP_TX_VREF_CENTER\n"));
    }

    releasePrintFControl (host);
  }
#endif  // SERIAL_DBG_MSG

  if ((group == TxVref) && (host->setup.mem.optionsExt & PDA_EN) && (CheckDimmType(host) == SUCCESS)) {
    host->var.mem.txVrefDone = 1;
  }
  //
  // Evaluate and program the results
  //
  CombineVrefResults(host, socket, group, vrefMargin);

  if ((group == TxVref) && (host->nvram.mem.dramType != SPD_TYPE_DDR3) && (host->setup.mem.optionsExt & PDA_EN)) {
    UpdateSafeTxVref(host, socket, DdrLevel);
  }

  return SUCCESS;
} // VrefCentering

UINT32
RxPerBitDeskew(
              PSYSHOST  host
              )
/*++

  Receive Per Bit Deskew

  @param host   - Pointer to sysHost

  @retval SUCCESS

--*/
{
  if (~host->memFlows & MF_RX_DESKEW_EN) return SUCCESS;

  OutputExtendedCheckpoint((host, STS_CHANNEL_TRAINING, PER_BIT_DESKEW_RX, host->var.mem.currentSocket));
  if (DESKEW_DELAY == RxDqsBitDelay) {
    PerBitDeskew(host, host->var.mem.currentSocket, RxDqsBitDelay);
  } else {
    PerBitDeskew(host, host->var.mem.currentSocket, RxDqsPBitDelay);
    PerBitDeskew(host, host->var.mem.currentSocket, RxDqsNBitDelay);
  }

  return SUCCESS;

} // RxPerBitDeskew

UINT32
TxPerBitDeskew(
              PSYSHOST  host
              )
/*++

    Tx Per Bit Deskew

    @param host  - Pointer to sysHost

    @retval SUCCESS

--*/
{
  if (~host->memFlows & MF_TX_DESKEW_EN) return SUCCESS;

  OutputExtendedCheckpoint((host, STS_CHANNEL_TRAINING, PER_BIT_DESKEW_TX, host->var.mem.currentSocket));
  PerBitDeskew(host, host->var.mem.currentSocket, TxDqBitDelay);

  return SUCCESS;
} // TxPerBitDeskew


/**

Arguments:

  host    - Pointer to sysHost

**/
void
PerBitDeskew(
            PSYSHOST  host,
            UINT8     socket,
            GSM_GT    group
            )
{
  UINT8               ch, dimm, rank, nibble, encodedNibble;
  GSM_GT              gmGroup;
  UINT8               bit, bitIdx;
  INT16               bitSkew;
  INT8                bitCenter[8]; 
  INT16               bitNibbleMin;
  UINT32              patternLength;
  INT8                rankBitCenter[MAX_DIMM][MAX_RANK_DIMM][MAX_BITS];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  struct bitMargin    resultsBit;
  struct bitMask      filter;

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return;
  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) return;

  channelNvList = GetChannelNvList(host, socket);
  patternLength = 64;

  if(host->setup.mem.perBitDeSkew == BIT_DESKEW_DISABLE){
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "PER_BIT_DESKEW - Disabled\n"));
    return;
  }
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "START_PER_BIT_DESKEW\n"));


  // Fill Filter Structure and initialize margin results structure
  for (ch = 0; ch < MAX_CH; ch++) {
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankList = GetRankNvList(host, socket, ch, dimm);
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        for (bit = 0; bit < MAX_BITS; bit++) {
          resultsBit.bits[ch][(*rankList)[rank].rankIndex][bit].n = 0;
          resultsBit.bits[ch][(*rankList)[rank].rankIndex][bit].p = 0;
        }
      } // Rank loop
    } // Dimm loop
  } // Ch loop

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) {
    getPrintFControl(host);
    PrintLine(host, 78, DOUBLE_LINE, CRLF_FLAG, CRLF_FLAG);
  }
#endif

  // Setup for either Rx and Tx
  if (group == RxDqsPBitDelay) {
    gmGroup     = RxDqsPDelay;
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Per Bit Deskew Rx P\n"));
  } else if (group == RxDqsNBitDelay) {
    gmGroup     = RxDqsNDelay;
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Per Bit Deskew Rx N\n"));
  } else if (group == RxDqsBitDelay) {
    gmGroup     = RxDqsDelay;
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Per Bit Deskew Rx\n"));
  } else {
    gmGroup     = TxDqDelay;
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "Per Bit Deskew Tx\n"));
  }

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) {
    PrintLine(host, 78, DOUBLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  }
#endif

  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter(host, socket, &filter);

  // Get per bit margins
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "PatternLength: %d\n", patternLength));
  GetMargins (host, socket, DdrLevel, gmGroup, MODE_VIC_AGG, SCOPE_BIT, &filter, (void *)&resultsBit, patternLength,
              0, 0, 0, WDB_BURST_LENGTH);

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) {
    DisplayPerBitMargins (host, socket, &resultsBit, gmGroup);
  }
#endif

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    dimmNvList = GetDimmNvList(host, socket, ch);

    MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
      "Calculating Bit Centers\n"));

    CalculateBitCenters (host, socket, ch, rankBitCenter, &resultsBit);

    dimm = 0;
    rank = 0;
    dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      rankList = GetRankNvList(host, socket, ch, dimm);
      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        // Get the Logical rank #
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "dimm = %d, rank = %d\n", dimm, rank));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "         Per Bit Margin Center       Per Bit Skew\n"));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "N# = Nibble, BCx = Bit Center, BSx = Bit Skew , MSL = Most Skewed Lane\n"));
        if ((group == RxDqsPBitDelay) || (group == RxDqsNBitDelay)) {
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "N# BC0-BC1-BC2-BC3   BS0-BS1-BS2-BS3  MSL\n"));
        } else {
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "N# BC0-BC1-BC2-BC3   BS0-BS1-BS2-BS3\n"));
        }

#ifdef SERIAL_DBG_MSG
        if (checkMsgLevel(host, SDBG_MAX)) {
          PrintLine(host, 78, DOUBLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
        }
#endif
        // Loop for each nibble/strobe.
        for (nibble = 0; nibble < MAX_STROBE; nibble++) {
          //
          // Skip if this is an ECC nibble when ECC is disabled
          // Note: unlike usual in training code, per-bit skew register are mapped linerly, so ECC bits are on the end
          //
          if ((!host->nvram.mem.eccEn) && (nibble > 15)) continue;
          if (nibble%2){
            encodedNibble = (nibble + 17)/2;
          } else {
            encodedNibble = nibble/2;
          }

          // Faulty part tracking: need to add Array checking here and if we find a faulty part.
          // need to add minimum margin (advanced strobe centering) and/or just no margin at all
          // If more than one failing nibble then that is a hard failure. So need to keep track of FPT and if more than
          // One nibble is failing, we have a real failure.
          // A sincle bit margin failure within a nibble can indicate a FPT, so flag it, if we can't rehabilitate the nibble via the deskew algo.

          bitNibbleMin = 127;   // Set to largest possible
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "%2d ", nibble));

          // Loop for all the bits in this nibble
          for (bit = 0; bit < BITS_PER_NIBBLE; bit++) {
            // Get the absolute bit index (0-72)
            bitIdx = (nibble * 4) + bit;

            // Find the center for this bit
            bitCenter[bit] = rankBitCenter[dimm][rank][bitIdx];

            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                           "%3d ", bitCenter[bit]));

            // Check if the center of this bit is smaller than the smallest found so far
            if (bitCenter[bit] < bitNibbleMin) {
              bitNibbleMin = bitCenter[bit];
            }
          } // for (bit)

          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                         "  "));

          // This bit loop is only for programing the skew register(s)
          for (bit = 0; bit < BITS_PER_NIBBLE; bit++) {
            // Get the Skew
            bitSkew = bitCenter[bit] - bitNibbleMin;
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "%3d ", bitSkew));
            // call SetGDCR to update the appropriate register field
            GetSetDataGroup (host, socket, ch, dimm, rank, encodedNibble, bit, DdrLevel, group, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &bitSkew);
          } // for bit
          GetSetDataGroup (host, socket, ch, dimm, rank, encodedNibble, 0, DdrLevel, gmGroup, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &bitNibbleMin);
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "\n"));

        } // nibble loop
      } // rank loop
    } // dimm loop
  } // ch loop

#ifdef SERIAL_DBG_MSG

  if (checkMsgLevel(host, SDBG_MAX)) {
    DisplayResults(host, socket, group);
  }

  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "STOP_PER_BIT_DESKEW\n"));

  if (checkMsgLevel(host, SDBG_MAX)) {
    releasePrintFControl(host);
  }
#endif

} // PerBitDeskew


void CalculateBitCenters (
  PSYSHOST          host,
  UINT8             socket,
  UINT8             ch,
  INT8              rankBitCenter[MAX_DIMM][MAX_RANK_DIMM][MAX_BITS],
  struct bitMargin  *resultsBit
  )
{
  UINT8   dimm, rank, bit;
  struct  ddrRank (*rankList)[MAX_RANK_DIMM];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];

  dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

  // Clear the rankBitCenter Array first
  for (bit = 0; bit < MAX_BITS; bit++) {
        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        rankBitCenter[dimm][rank][bit] = 0;
          }
    }
  }

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
    rankList = GetRankNvList(host, socket, ch, dimm);
    for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
      if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

      for (bit = 0; bit < MAX_BITS; bit++) {
        // Skip bits 64-71 if ECC not enabled
        if ((!host->nvram.mem.eccEn) && (bit > 63)) continue;
        rankBitCenter[dimm][rank][bit] = (((INT8) (resultsBit->bits[ch][(*rankList)[rank].rankIndex][bit].n +
                              resultsBit->bits[ch][(*rankList)[rank].rankIndex][bit].p))) / 2;
      } // for bit
    } // rank
  } // dimm
} // CalculateBitCenters


void
InitStartingOffset (
                   PSYSHOST  host,
                   UINT8     socket
                   )
/*++
  Initializes the starting margins

  @param host        - Pointer to sysHost
  @param socket      - Processor socket to check

  @retval N/A

--*/
{

  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];

  channelNvList = GetChannelNvList(host, socket);

  host->var.mem.socket[socket].rdVrefLo  = RD_WR_CMD_INIT_MARG;
  host->var.mem.socket[socket].rdVrefHi  = RD_WR_CMD_INIT_MARG;
  host->var.mem.socket[socket].wrVrefLo  = RD_WR_CMD_INIT_MARG;
  host->var.mem.socket[socket].wrVrefHi  = RD_WR_CMD_INIT_MARG;
  host->var.mem.socket[socket].cmdVrefLo = RD_WR_CMD_INIT_MARG;
  host->var.mem.socket[socket].cmdVrefHi = RD_WR_CMD_INIT_MARG;

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      rankList = GetRankNvList(host, socket, ch, dimm);

      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        (*rankList)[rank].rxDQLeftSt  = RX_TX_DQ_INIT_MARG;
        (*rankList)[rank].rxDQRightSt = RX_TX_DQ_INIT_MARG;
        (*rankList)[rank].txDQLeftSt  = RX_TX_DQ_INIT_MARG;
        (*rankList)[rank].txDQRightSt = RX_TX_DQ_INIT_MARG;
      } // rank loop
    } // dimm loop
  } // ch loop
} // InitStartingOffset

UINT32
CombineVrefResults (
                   PSYSHOST  host,
                   UINT8     socket,
                   GSM_GT    group,
                   VOID      *value
                   )
/*++

  Evaluates Vref results and programs optimal settings

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param group   - Parameter to center
                   RxVref 0: Center Rx Vref
                   TxVref 1: Center Tx Vref
  @param value   - Pointer to the vrefMargin

  @retval SUCCESS

--*/
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  UINT8               maxStrobe;
  INT16               vrefHi = 0x5F;
  INT16               vrefLo = -0x5F;
  INT16               tmpEdge;
  INT16               offset;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  struct strobeMargin (*vrefMargin)[3];

  vrefMargin = value;

  channelNvList = GetChannelNvList(host, socket);

  if (group == RxVref) {

    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "            vrefLo  vrefHi    offset\n"));
    for (strobe = 0; strobe < MAX_STROBE; strobe++) {
      //
      // Skip if this is an ECC strobe when ECC is disabled
      //
      if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

      for (ch = 0; ch < MAX_CH; ch++) {
        //
        // Skip if channel is disabled
        //
        if ((*channelNvList)[ch].enabled == 0) continue;

        vrefHi = 0x5F;
        vrefLo = -0x5F;

        dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          //
          // Skip if no DIMM present
          //
          if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

          rankList = GetRankNvList(host, socket, ch, dimm);

          for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
            //
            // Skip if no rank
            //
            if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

            //
            // Find the composite eye for this strobe across ranks
            //
            tmpEdge = ((*vrefMargin)[0].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                       (*vrefMargin)[1].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                       (*vrefMargin)[2].strobe[ch][(*rankList)[rank].rankIndex][strobe].p) / 3;
            if (vrefHi > tmpEdge) {
              vrefHi = tmpEdge;
            }

            tmpEdge = ((*vrefMargin)[0].strobe[ch][(*rankList)[rank].rankIndex][strobe].n +
                       (*vrefMargin)[1].strobe[ch][(*rankList)[rank].rankIndex][strobe].n +
                       (*vrefMargin)[2].strobe[ch][(*rankList)[rank].rankIndex][strobe].n) / 3;
            if (vrefLo < tmpEdge) {
              vrefLo = tmpEdge;
            }

          } // rank loop
        } // dimm loop

        offset = (vrefHi + vrefLo) / 2;
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, strobe, NO_BIT,
                       "  %2d      %2d       %2d\n", vrefLo, vrefHi, offset));

        GetSetDataGroup (host, socket, ch, 0, 0, strobe, ALL_BITS, DdrLevel, group, GSM_UPDATE_CACHE + GSM_WRITE_OFFSET,
                         &offset);
      } // ch loop
    } // strobe loop
  } else if ((group == TxVref) && (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {

    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "            vrefLo  vrefHi    offset\n"));
    for (ch = 0; ch < MAX_CH; ch++) {
      //
      // Skip if channel is disabled
      //
      if ((*channelNvList)[ch].enabled == 0) continue;

      dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

      vrefHi = 0x5F;
      vrefLo = -0x5F;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        //
        // Skip if no DIMM present
        //
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        maxStrobe = GetMaxStrobe(host, socket, ch, dimm, group, DdrLevel);

        rankList = GetRankNvList(host, socket, ch, dimm);

        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          //
          // Skip if no rank
          //
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

          for (strobe = 0; strobe < maxStrobe; strobe++) {
            //
            // Skip if this is an ECC strobe when ECC is disabled
            //
            if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;


            //
            // Find the composite eye for this strobe across ranks
            //
            tmpEdge = ((*vrefMargin)[0].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                       (*vrefMargin)[1].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                       (*vrefMargin)[2].strobe[ch][(*rankList)[rank].rankIndex][strobe].p) / 3;
            if (vrefHi > tmpEdge) {
              vrefHi = tmpEdge;
            }

            tmpEdge = ((*vrefMargin)[0].strobe[ch][(*rankList)[rank].rankIndex][strobe].n +
                       (*vrefMargin)[1].strobe[ch][(*rankList)[rank].rankIndex][strobe].n +
                       (*vrefMargin)[2].strobe[ch][(*rankList)[rank].rankIndex][strobe].n) / 3;
            if (vrefLo < tmpEdge) {
              vrefLo = tmpEdge;
            }

          } // strobe loop
        } // rank loop
      } // dimm loop

      offset = (vrefHi + vrefLo) / 2;
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "  %2d      %2d       %2d\n", vrefLo, vrefHi, offset));

      //
      // Set Tx Vref per channel
      //
      GetSetDataGroup (host, socket, ch, 0, 0, 0, ALL_BITS, DdrLevel, group, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET | GSM_FORCE_WRITE,
                       &offset);

    } // ch loop

 } else if ((group == TxVref) && (host->nvram.mem.dramType != SPD_TYPE_DDR3)) {

    //
    // Do not center Tx Vref if PDA is disabled
    //
    if (host->setup.mem.optionsExt & PDA_EN) {

      for (ch = 0; ch < MAX_CH; ch++) {
        //
        // Skip if channel is disabled
        //
        if ((*channelNvList)[ch].enabled == 0) continue;

        dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          //
          // Skip if no DIMM present
          //
          if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

          maxStrobe = GetMaxStrobe(host, socket, ch, dimm, group, DdrLevel);

          rankList = GetRankNvList(host, socket, ch, dimm);

          for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
            //
            // Skip if no rank
            //
            if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                           "            vrefLo  vrefHi    offset\n"));
            for (strobe = 0; strobe < maxStrobe; strobe++) {
              //
              // Skip if this is an ECC strobe when ECC is disabled
              //
              if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

              //
              // Find the composite eye for this strobe across ranks
              //
              vrefHi = ((*vrefMargin)[0].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                        (*vrefMargin)[1].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                        (*vrefMargin)[2].strobe[ch][(*rankList)[rank].rankIndex][strobe].p) / 3;

              vrefLo = ((*vrefMargin)[0].strobe[ch][(*rankList)[rank].rankIndex][strobe].n +
                        (*vrefMargin)[1].strobe[ch][(*rankList)[rank].rankIndex][strobe].n +
                        (*vrefMargin)[2].strobe[ch][(*rankList)[rank].rankIndex][strobe].n) / 3;

              offset = (vrefHi + vrefLo) / 2;

              MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                             "  %2d      %2d       %2d\n", vrefLo, vrefHi, offset));
              GetSetDataGroup (host, socket, ch, dimm, rank, strobe, ALL_BITS, DdrLevel, group,
                               GSM_UPDATE_CACHE | GSM_WRITE_OFFSET | GSM_FORCE_WRITE, &offset);
            } // strobe loop
          } // rank loop
        } // dimm loop
      } // ch loop

    } else {

      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "            vrefLo  vrefHi    offset\n"));
      for (ch = 0; ch < MAX_CH; ch++) {
        //
        // Skip if channel is disabled
        //
        if ((*channelNvList)[ch].enabled == 0) continue;

        dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          //
          // Skip if no DIMM present
          //
          if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

          maxStrobe = GetMaxStrobe(host, socket, ch, dimm, group, DdrLevel);

          rankList = GetRankNvList(host, socket, ch, dimm);

          for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
            //
            // Skip if no rank
            //
            if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

            vrefHi = 0x5F;
            vrefLo = -0x5F;

            for (strobe = 0; strobe < maxStrobe; strobe++) {
              //
              // Skip if this is an ECC strobe when ECC is disabled
              //
              if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;


              //
              // Find the composite eye for this strobe across ranks
              //
              tmpEdge = ((*vrefMargin)[0].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                         (*vrefMargin)[1].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                         (*vrefMargin)[2].strobe[ch][(*rankList)[rank].rankIndex][strobe].p) / 3;
              if (vrefHi > tmpEdge) {
                vrefHi = tmpEdge;
              }

              tmpEdge = ((*vrefMargin)[0].strobe[ch][(*rankList)[rank].rankIndex][strobe].n +
                         (*vrefMargin)[1].strobe[ch][(*rankList)[rank].rankIndex][strobe].n +
                         (*vrefMargin)[2].strobe[ch][(*rankList)[rank].rankIndex][strobe].n) / 3;
              if (vrefLo < tmpEdge) {
                vrefLo = tmpEdge;
              }

            } // strobe loop

            offset = (vrefHi + vrefLo) / 2;
            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                           "  %2d      %2d       %2d\n", vrefLo, vrefHi, offset));

            //
            // Set Tx Vref per channel
            //
            GetSetDataGroup (host, socket, ch, dimm, rank, ALL_STROBES, ALL_BITS, DdrLevel, group, GSM_UPDATE_CACHE + GSM_WRITE_OFFSET,
                             &offset);

          } // rank loop
        } // dimm loop
      } // ch loop
    } // PDA check
  } else {
    // Error
  }

  IO_Reset (host, socket);

  return SUCCESS;
} // CombineVrefResults

void
GetMultiVref (
             PSYSHOST        host,
             UINT8           socket,
             GSM_LT          level,
             GSM_GT          group,
             UINT16          mode,
             UINT8           scope,
             struct bitMask  *filter,
             void            *results,
             UINT32          patternLength,
             UINT8           chMask,
             UINT8           rankMask,
             UINT8           stepSize,
             UINT8           numPoints
             )
/*++
  Calculates margins at multiple Vref points

  @param host         - Pointer to sysHost
  @param socket       - Processor socket to check
  @param level        - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param group        - Parameter to be margined
                         RxDqsDelay    : Margin Rx DqDqs
                         TxDqDelay     : Margin Tx DqDqs
                         RxVref         : Margin Rx Vref
                         TxVref         : Margin Tx Vref
                         RecEnDelay    : Margin Receive Enable
                         WrLvlDelay    : Margin Write Level
                         PcodeMailboxStatus = 0;
                         CmdGrp0        : Margin CMD group 0
                         CmdGrp1        : Margin CMD group 1
                         CmdGrp2        : Margin CMD group 2
                         CmdAll         : Margin all CMD groups
  @param mode         - Test mode to use
                         MODE_XTALK            BIT0: Enable crosstalk (placeholder)
                         MODE_VIC_AGG          BIT1: Enable victim/aggressor
                         MODE_START_ZERO       BIT2: Start at margin 0
                         MODE_POS_ONLY         BIT3: Margin only the positive side
                         MODE_NEG_ONLY         BIT4: Margin only the negative side
                         MODE_DATA_MPR         BIT5: Enable MPR Data pattern
                         MODE_DATA_FIXED       BIT6: Enable Fixed Data pattern
                         MODE_DATA_LFSR        BIT7: Enable Data LFSR
                         MODE_ADDR_LFSR        BIT8: Enable Address LFSR
                         MODE_ADDR_CMD0_SETUP  BIT9
                         MODE_CHECK_PARITY     BIT10: Enable parity checking
                         MODE_DESELECT         BIT11: Enable deselect patterns
                         MODE_VA_DESELECT      BIT12: Enable Victim - Aggressor deselect patterns
  @param scope        - Margin granularity
                         SCOPE_SOCKET      0: Margin per processor socket
                         SCOPE_CH        1: Margin per channel
                         SCOPE_DIMM      2: Margin per DIMM
                         SCOPE_RANK      3: Margin per rank
                         SCOPE_STROBE    4: Margin per strobe group
                         SCOPE_BIT       5: Margin per bit
  @param filter        - Mask of bits to exclude from testing
  @param results       - Pointer to the structure to store the margin results. The structure type varies based on scope
  @param patternLength - RankMarginTest Pattern length
  @param chMask        - Bit Mask of channels to not be used
  @param rankMask      - Bit Mask of ranks to not be used
  @param stepSize      - Step size
  @param numPoints     - Number of points to test

  @retval *results:  Pointer to the structure to store the margin results. The structure type varies based on scope.
--*/
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  GSM_GT              vGroup = 0;
  UINT8               maxVref;
  UINT8               errChMask = 0;
  INT32               vIndex;
  INT16               margin;
  INT16               restoreOffset[MAX_CH];
  UINT32              status = 0;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  void                *tmpResults = 0;

  channelNvList = GetChannelNvList(host, socket);

  disableAllMsg (host);

  if ((group == RxDqsDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay)) {
    vGroup = RxVref;
  } else if (group == TxDqDelay) {
    vGroup = TxVref;
  } else if (CheckCMDGroup(group)) {
    vGroup = CmdVref;
  } else {
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "group %d not supported in GetMultiVref\n", group));
  }

  maxVref = (numPoints / 2) * stepSize;

  //
  // Loop for all Vref's
  //
  vIndex = 0;

  for (margin = -maxVref; margin < maxVref + 1; margin = margin + (INT8)stepSize) {

    // For debug
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "margin = %d, vIndex = %d, maxVref = %d\n", margin, vIndex, maxVref));

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      // Check if this channel is masked off
      if (chMask & (1 << ch)) continue;

      if (CheckCMDGroup(group) || CheckCTLGroup(group)) {
        GetSetCmdVrefCore (host, socket, ch, 0, level, GSM_READ_ONLY, &restoreOffset[ch]);
        status = SetOffset(host, socket, ch, 0, 0, margin, level, vGroup, 0);
        if (status != SUCCESS) {
          errChMask |= (1 << ch);
        }
      } else if ((host->nvram.mem.dramType == SPD_TYPE_DDR3) && (vGroup == TxVref)) {
        GetSetDataGroup (host, socket, ch, 0, 0, ALL_STROBES, ALL_BITS, level, vGroup, GSM_READ_ONLY, &restoreOffset[ch]);
        status = SetOffset(host, socket, ch, 0, 0, margin, level, vGroup, 0);
        if (status != SUCCESS) {
          errChMask |= (1 << ch);
        }
      } else {
        dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

        for (dimm = 0; dimm < MAX_DIMM; dimm++) {

          for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
            if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

            if (rankMask & (1 << GetLogicalRank(host, socket, ch, dimm, rank))) continue;

            // Skip setting TXVREF multiple times on LRDIMMs
            if ((IsLrdimmPresent(host, socket, ch, dimm) && (vGroup == TxVref) && (level == DdrLevel) && (rank > 0))) continue;

            // Set the new margin to test
            if ((vGroup != RxVref) || ((dimm == 0) && (rank == 0))) {
              status = SetOffset(host, socket, ch, dimm, rank, margin, level, vGroup, 0);
            }
            if (status != SUCCESS) {
              errChMask |= (1 << ch);
            }
          } // rank loop
        } // dimm loop
      }
    } // ch loop

    if (scope == SCOPE_RANK) {
      tmpResults = (UINT8 *)results + (sizeof(struct rankMargin) * vIndex);
    } else if (scope == SCOPE_STROBE) {
      tmpResults = (UINT8 *)results + (sizeof(struct strobeMargin) * vIndex);
    } else if (scope == SCOPE_BIT) {
      tmpResults = (UINT8 *)results + (sizeof(struct bitMargin) * vIndex);
    }

    //MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
    //              "Calling GetMargins\n"));
    //
    // Get the margins at the current Vref setting
    //
    if (tmpResults == NULL) continue;
    if (margin == 0) {
       restoreMsg (host);
       MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "margin = %d, vIndex = %d, Running 7 times pattern length\n", margin, vIndex));
       disableAllMsg (host);
       GetMargins (host, socket, level, group, mode, scope, filter, tmpResults, patternLength * 7, 1, chMask | errChMask, rankMask,
                WDB_BURST_LENGTH);
    } else {
       GetMargins (host, socket, level, group, mode, scope, filter, tmpResults, patternLength, 1, chMask | errChMask, rankMask,
                WDB_BURST_LENGTH);
    }
    disableAllMsg (host);

    if (CheckCMDGroup(group) || CheckCTLGroup(group) || ((host->nvram.mem.dramType == SPD_TYPE_DDR3) && (vGroup == TxVref))) {
      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        // Check if this channel is masked off
        if (chMask & (1 << ch)) continue;

        //
        // Restore the new margin to test
        //
        RestoreOffset(host, socket, ch, 0, 0, restoreOffset[ch], level, vGroup);
      } // ch loop
    }

    //
    // Reset the DIMMs incase Tx Vref got set to a failing voltage
    //
    if ((host->nvram.mem.dramType != SPD_TYPE_DDR3) && (vGroup == TxVref)) {
      // Execute JEDEC reset/init sequence
        IO_Reset (host, socket);
        JedecInitSequence (host, socket, CH_BITMASK);
    }

    //
    // Increment the index into the results array
    //
    vIndex++;

  } // margin loop

  if (!(CheckCMDGroup(group) || CheckCTLGroup(group) || ((host->nvram.mem.dramType == SPD_TYPE_DDR3) && (vGroup == TxVref)))) {
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      // Check if this channel is masked off
      if (chMask & (1 << ch)) continue;

      dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
          if (rankMask & (1 << GetLogicalRank(host, socket, ch, dimm, rank))) continue;

          // Skip setting TXVREF multiple times on LRDIMMs
          if ((IsLrdimmPresent(host, socket, ch, dimm) && (vGroup == TxVref) && (level == DdrLevel) && (rank > 0))) continue;

          //
          // Restore Vref
          //
          if((vGroup != RxVref) || ((dimm == 0) && (rank == 0))) {
            RestoreOffset(host, socket, ch, dimm, rank, 0, level, vGroup);
          }
        } // rank loop
      } // dimm loop
    } // ch loop
  }

  restoreMsg (host);

} // GetMultiVref

UINT32
CollectTestResults (
                   PSYSHOST  host,
                   UINT8     socket,
                   UINT32    chToCheck,
                   UINT32    bwSerr[MAX_CH][3]
                   )
/*++
  Collects the results of the previous test

  @param host         - Pointer to sysHost
  @param socket       - Processor socket to check
  @param chToCheck    - Channel to check
  @param bwSerr       - Bit wise error status

  @retval status      - Non-zero if any failure was detected

--*/
{
  UINT8                 ch;
  UINT32                status = SUCCESS;
  CPGC_ERROR_STATUS     cpgcErrorStatus[MAX_CH];
  struct  channelNvram  (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  // Collect test results
  CpgcAdvTrainingErrorStatus (host, socket, chToCheck, cpgcErrorStatus);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    // No need to check this channel if it's bit isn't set
    if ((chToCheck & (1 << ch))) {

      bwSerr[ch][0] = cpgcErrorStatus[ch].cpgcErrDat0S | cpgcErrorStatus[ch].cpgcErrDat2S;
      bwSerr[ch][1] = cpgcErrorStatus[ch].cpgcErrDat1S | cpgcErrorStatus[ch].cpgcErrDat3S;
      if (host->nvram.mem.eccEn) {
        bwSerr[ch][2] = (cpgcErrorStatus[ch].cpgcErrEccS & 0xFF) | ((cpgcErrorStatus[ch].cpgcErrEccS >> 8) & 0xFF);
      } else {
        bwSerr[ch][2] = 0;
      }

      status |= bwSerr[ch][0];
      status |= bwSerr[ch][1];
      status |= bwSerr[ch][2];
    }
  } // ch loop

  return  status;
} // CollectTestResults

static UINT32 ImcUncErrorMsr[ ] = {
  0x421, 0x425, 0x429, 0x42d, 0
};

void
ClearCmdAddrParityError (
                        PSYSHOST  host,
                        UINT8     socket,
                        UINT8     ch
                        )
/*++

  Collects the results of the previous test

  @param host         - Pointer to sysHost
  @param socket       - Processor socket to check
  @param ch           - Channel

  @retval N/A

--*/
{
  UINT64_STRUCT msrReg;

  // Clear the error status
  msrReg.hi = 0;
  msrReg.lo = 0;
  WriteMsrPipe(host, socket, ImcUncErrorMsr[ch], msrReg);

} // ClearCmdAddrParityError

UINT32
CheckCmdAddrParityError (
                        PSYSHOST  host,
                        UINT8     socket,
                        UINT8     ch
                        )
/*++

  Collects the results of the previous test

  @param host         - Pointer to sysHost
  @param socket       - Processor socket to check
  @param ch           - Channel

  @retval status:       Non-zero if any failure was detected

--*/
{
  UINT32   status = 0;
  UINT64_STRUCT msrReg;

  msrReg = ReadMsrPipe(host, socket, ImcUncErrorMsr[ch]);

  if (msrReg.hi & (BIT31 + BIT29)) {
    //MemDebugPrint((host, SDBG_MEM_TRAIN, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
    //              "MSR 0x%x = 0x%8x:%8x!!!\n", msrReg.hi, msrReg.lo));
    if (msrReg.lo & BIT16) {
      status = 1;
    }
  }
  return  status;
} // CheckCmdAddrParityError


UINT32
CheckAddrParity (
                PSYSHOST  host,
                UINT8     socket,
                UINT8     chToCheck,
                UINT32    bwSerr[MAX_CH][3]
                )
/*++

  Collects the results of the previous test

  @param host         - Pointer to sysHost
  @param socket       - Processor socket to check
  @param chToCheck    - Channel to check
  @param bwSerr       - Bit wise error status

  @retval status:       Non-zero if any failure was detected

--*/
{
  UINT8   ch;
  UINT32  status = 0;
  struct  channelNvram  (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    // No need to check this channel if it's bit isn't set
    if ((chToCheck & (1 << ch))) {

      if (CheckCmdAddrParityError(host, socket, ch)) {

        status = 1;
        bwSerr[ch][0] = 0xFFFFFFFF;
        bwSerr[ch][1] = 0xFFFFFFFF;
        bwSerr[ch][2] = 0xFF;
        //MemDebugPrint((host, SDBG_MEM_TRAIN, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        //              "Address Parity Error!!!\n"));

        ClearCmdAddrParityError(host, socket, ch);
      }
    }
  } // ch loop

  return  status;
} // CheckAddrParity

UINT8
CountBitsEqOne (
               UINT32 Input
               )
/*++

  This function calculates the number of bits set to 1 in a 32-bit value

  @param Input           - the value to work on

  @retval the number of bits set to 1 in Input

--*/
{
  UINT8 NumOnes;

  NumOnes = 0;
  while (Input > 0) {
    NumOnes++;
    Input &= (Input - 1);
  }

  return NumOnes;
} // CountBitsEqOne

UINT8
GetMaxStrobe(
            PSYSHOST  host,
            UINT8     socket,
            UINT8     ch,
            UINT8     dimm,
            GSM_GT    group,
            GSM_LT    level
            )
/*++
  Get the maximum number of data groups based on DIMM and group type

  @param host  - Pointer to sysHost
  @param socket  - Socket
  @param ch    - Channel
  @param dimm  - DIMM
  @param group - Group type
  @param level - Level type

  @retval Max strobe

--*/
{
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];

  dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

#ifdef LRDIMM_SUPPORT
  if ((IsLrdimmPresent(host, socket, ch, dimm)) && (group == TxVref) && (level == DdrLevel)) {
    return MAX_STROBE / 2;
  }
  if ((IsLrdimmPresent(host, socket, ch, dimm)) && (group == TxVref) && (level == LrbufLevel)) {
    return MAX_STROBE;
  }
  if ((IsLrdimmPresent(host, socket, ch, dimm)) && (group == RxVref) && (level == LrbufLevel)) {
    return MAX_STROBE / 2;
  }
#endif
  //
  // Setup variables based on x4 or x8 DIMM
  //
  if ((*dimmNvList)[dimm].x4Present ||
      (group == RxDqsPDelay) || (group == RxDqsNDelay) || (group == RxDqDqsDelay) ||
      (group == RxDqDelay) || (group == RxDqsDelay) || (group == RecEnDelay) ||
      (group == TxDqDelay) || (group == RxVref) || (group == RxEq) ||
      (group == TxEq) || ((group == TxVref) && (host->nvram.mem.dramType == SPD_TYPE_DDR3))) {
    return MAX_STROBE;
  } else {
    return MAX_STROBE / 2;
  }
} // GetMaxStrobe

UINT32
XOR (
  UINT32 a,
  UINT32 b
  )
{
  if (a==b)
    return 0;
  else
    return 1;
} // XOR

/*++

  Calculate the next LFSR based on the inputs

  @param lfsr         - Current lfsr
  @param lfsrPoly     - Polynomial taps
  @param lfsrOrder    - order

  @retval returns updated lfsr

--*/
UINT32
NextLFSROut(
           UINT32 lfsr,
           UINT32 lfsrPoly,
           UINT32 lfsrOrder
           )
{
  UINT32  order;
  UINT32  NorOut;
  UINT32  NorCount;
  UINT32  SelectBit = 0x01;
  UINT32  feedback;
  UINT32  next_lfsr;
  UINT32  temp_lfsr;

  NorOut = 0;
  NorCount = 0;
  temp_lfsr = lfsr;

  for (order = 0; order < lfsrOrder - 1; order++) {
    if ((temp_lfsr & SelectBit) == 0) {
      NorCount++;
    }

    temp_lfsr = temp_lfsr >> 1;
  } // order loop

  if (NorCount == (lfsrOrder - 1)) {
    NorOut = 1;
  }

  feedback = XOR(NorOut, (SelectBit & (lfsr >> (lfsrOrder - 1))));
  next_lfsr = 0x0;

  for (order = 1; order < lfsrOrder; order++) {
    next_lfsr = next_lfsr + XOR(((lfsr >> (lfsrOrder - order - 1)) & SelectBit),
                                (((lfsrPoly >> (lfsrOrder - order)) & SelectBit) * feedback));
    next_lfsr = next_lfsr << 1;
  } // order loop

  next_lfsr = next_lfsr + feedback;

  return next_lfsr;
} // NextLFSROut

/*++
  Evaluates (based on ODT table) if there are any detected issues
  with strobes and if so whether the set of issues form correctable
  or non-correctable error

  @param host    - Pointer to sysHost
  @param socket  - Socket
  @param ch      - Channel

  @retval returns status -> FPT_NO_ERROR, FPT_CORRECTABLE_ERROR, FPT_NO_CORRECTABLE_ERROR

--*/
UINT8
EvaluateFaultyParts(
                   PSYSHOST host,
                   UINT8     socket,
                   UINT8     ch
                   )
{
  UINT8             found_cnt = 0;
  UINT8             strobe;
  UINT8             aux_strobe;
  UINT8             rank;
  UINT8             dimm;
  UINT8             previous_faulty_strobe[MAX_STROBE];
  UINT8             status;
  UINT8             non_correctable_get_out = 0;
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];
  struct ddrRank    (*rankList)[MAX_RANK_DIMM];

  status = FPT_NO_ERROR;
  if (host->var.common.emulation & SIMICS_FLAG) {
    return status;
  }
  /*
for (dimm1 = 0; dimm1 < MAX_STROBE; dimm1++)  {
    previous_faulty_dimm[dimm] = 0xFF;
  }
for (rank1 = 0; rank1 < MAX_STROBE; rank1++)  {
    previous_faulty_rank[rank] = 0xFF;
  }
   */
  for (strobe = 0; strobe < MAX_STROBE; strobe++) {
    previous_faulty_strobe[strobe] = 0xFF;
  }

  dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if (non_correctable_get_out) {
      break;
    }

    if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

    rankList = GetRankNvList(host, socket, ch, dimm);
    for (rank = 0; rank< MAX_RANK_DIMM; rank++) {
      if (non_correctable_get_out) {
        break;
      }

      // Continue to the next rank if this one is disabled
      if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

      // let's first check all the strobes
      for (strobe = 0; strobe < MAX_STROBE; strobe++) {
        if (non_correctable_get_out) {
          break;
        }
        // is there strobe with marked failing part
        if ((*rankList)[rank].faultyParts[strobe] > 0) {
          // there is failing part and if there is no ECC it is non-correctable error right away
          if (host->nvram.mem.eccEn) {
            if (found_cnt > 0) {
              // this is already second strobe failing - can still be ok only if we are lock step mode and nibbles are next to each other
              if (CH_LOCKSTEP & host->var.mem.OpRASmode) {
                MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                               "EFP LOCKSTEP \n"));
                //do some more analysis later - fail for now
                status = FPT_NO_CORRECTABLE_ERROR;
                non_correctable_get_out = 1;
                break;
              } else {
                // this is already second strobe failing and since we are NOT in lock step mode on x8
                // we need to check for the ranks vs strobe failures
                // 2 different strobes on the same rank - non correctable
                // 2 same strobes on different ranks - correctable
                // 2 different strobes on two different ranks - possibly correctable evaluated based on ODT matrix
                //    if both strobe-rank are terminating - non correctable
                //    if only one strobe-rank is terminating - correctable
                MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                               "EFP 2 strobes failing \n"));


                for (aux_strobe = 0; aux_strobe < found_cnt; aux_strobe++) {
                  if (previous_faulty_strobe[aux_strobe] != 0xFF) {
                    if (strobe == previous_faulty_strobe[aux_strobe]) {
                      status = FPT_CORRECTABLE_ERROR;
                      if ( 0 == (host->setup.mem.optionsExt & ALLOW_CORRECTABLE_ERROR)) {// Disable the channel on SBE during the training.
                        status = FPT_NO_CORRECTABLE_ERROR;
                        non_correctable_get_out = 1;
                        break;
                      } // if ALLOW_CORRECTABLE_ERROR
                    } else {
                      status = FPT_NO_CORRECTABLE_ERROR;
                      non_correctable_get_out = 1;
                      break;
                    } //  if previous_faulty_strobe
                  } //previous_faulty_strobe[aux_strobe]
                } // aux_strobe loop
              } // if CH_LOCKSTEP
            } else {
              // this is the first strobe failing and since there is ECC it is correctable
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                             "EFP ODT based eval expected\n"));

              status = FPT_CORRECTABLE_ERROR;
              if ( 0 == (host->setup.mem.optionsExt & ALLOW_CORRECTABLE_ERROR)) {// Disable the channel on SBE during the training.
                status = FPT_NO_CORRECTABLE_ERROR;
                non_correctable_get_out = 1;
                break;
              } // if ALLOW_CORRECTABLE_ERROR
            } // found_cnt
          } else {
            // no ECC it is non-correctable error right away
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                           "EFP NO ECC\n"));
            status = FPT_NO_CORRECTABLE_ERROR;
            non_correctable_get_out = 1;
            break;
          }
          //previous_faulty_dimm[found_cnt] = dimm;
          //previous_faulty_rank[found_cnt] = rank;
          previous_faulty_strobe[found_cnt] = strobe;
          found_cnt++;
          if (MAX_STROBE == found_cnt) {
            status = FPT_NO_CORRECTABLE_ERROR;
            non_correctable_get_out = 1;
            break;
          }
        } // if faulty part
      } // strobe loop
    } // rank loop
  } // dimm loop

  // only print if non zero
  if (status != FPT_NO_ERROR) {
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "EFP status: %d\n", status));
  }
  return(status);
} // EvaluateFaultyParts

#if 0
/*++

  Centers Tx Vref then Tx timing

  @param host    - Pointer to sysHost

  @retval N/A

--*/
void
CenterTx (
         PSYSHOST  host,
         UINT8     socket,
         UINT8     flag
         )
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  INT16               offset;
  UINT32              patternLength;
  UINT16              mode;
  struct bitMask      filter;
  struct rankMargin   vrefMargin;
  struct strobeMargin results;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];

  channelNvList = GetChannelNvList(host, socket);
  mode = MODE_VIC_AGG;
  patternLength = 64;

  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter(host, socket, &filter);

  if (flag & FIRST_TIMING) {
    //
    // Center Tx Timing
    //
    for (ch = 0; ch < MAX_CH; ch++) {
      for (rank = 0; rank < MAX_RANK_CH; rank++) {
        for (strobe = 0; strobe < MAX_STROBE; strobe++) {
          results.strobe[ch][rank][strobe].n = 0;
          results.strobe[ch][rank][strobe].p = 0;
        } // strobe loop
      } // rank loop
    } // ch loop

    GetMargins (host, socket, DdrLevel, TxDqDelay, mode, SCOPE_STROBE, &filter, &results, patternLength, 0, 0, 0,
                WDB_BURST_LENGTH);

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        //
        // Skip if no DIMM present
        //
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        rankList = GetRankNvList(host, socket, ch, dimm);

        for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
          //
          // Skip if no rank
          //
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

          for (strobe = 0; strobe < MAX_STROBE; strobe++) {
            offset = (results.strobe[ch][(*rankList)[rank].rankIndex][strobe].p + results.strobe[ch][(*rankList)[rank].rankIndex][strobe].n) / 2;
            GetSetDataGroup (host, socket, ch, dimm, rank, strobe, ALL_BITS, DdrLevel, TxDqDelay, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &offset);
          } // strobe loop
        } // rank loop
      } // dimm loop
    } // ch loop
  }

  if (flag & SECOND_VREF) {
    for (ch = 0; ch < MAX_CH; ch++) {
      for (rank = 0; rank < MAX_RANK_CH; rank++) {
        vrefMargin.rank[ch][rank].n = 0;
        vrefMargin.rank[ch][rank].p = 0;
      } // rank loop
    } // ch loop

    //
    // Center Tx Vref
    //
    GetMargins (host, socket, DdrLevel, TxVref, mode, SCOPE_RANK, &filter, &vrefMargin, patternLength, 0, 0, 0,
                WDB_BURST_LENGTH);

    disableAllMsg(host);

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        //
        // Skip if no DIMM present
        //
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        rankList = GetRankNvList(host, socket, ch, dimm);

        for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
          //
          // Skip if no rank
          //
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

          offset = (vrefMargin.rank[ch][(*rankList)[rank].rankIndex].p + vrefMargin.rank[ch][(*rankList)[rank].rankIndex].n) / 2;
          GetSetDataGroup (host, socket, ch, dimm, rank, ALL_STROBES, ALL_BITS, DdrLevel, TxVref, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &offset);
        } // rank loop
      } // dimm loop
    } // ch loop
  }

  if (flag & SECOND_TIMING) {
    //
    // Center Tx Timing
    //
    for (ch = 0; ch < MAX_CH; ch++) {
      for (rank = 0; rank < MAX_RANK_CH; rank++) {
        for (strobe = 0; strobe < MAX_STROBE; strobe++) {
          results.strobe[ch][rank][strobe].n = 0;
          results.strobe[ch][rank][strobe].p = 0;
        } // strobe loop
      } // rank loop
    } // ch loop

    GetMargins (host, socket, DdrLevel, TxDqDelay, mode, SCOPE_STROBE, &filter, &results, patternLength, 0, 0, 0,
                WDB_BURST_LENGTH);

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        //
        // Skip if no DIMM present
        //
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        rankList = GetRankNvList(host, socket, ch, dimm);

        for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
          //
          // Skip if no rank
          //
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

          for (strobe = 0; strobe < MAX_STROBE; strobe++) {
            offset = (results.strobe[ch][(*rankList)[rank].rankIndex][strobe].p + results.strobe[ch][(*rankList)[rank].rankIndex][strobe].n) / 2;
            GetSetDataGroup (host, socket, ch, dimm, rank, strobe, ALL_BITS, DdrLevel, TxDqDelay, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &offset);
          } // strobe loop
        } // rank loop
      } // dimm loop
    } // ch loop
  }

  restoreMsg(host);
} // CenterTx
#endif // if 0


UINT8
CoreSetVdd (
  PSYSHOST host,
  UINT8    socket
  )
/*++

  CoreSetVdd - Call an OEM hook to change the socket Vddq

  @param host  - Pointer to sysHost
  @param socket  - processor number

  @retval SUCCESS if the Vddq change was made
  @retval FAILURE if the Vddq change was not made

--*/
{
  return PlatformSetVdd (host,socket);
}

void
CoreReleaseADRClamps (
  PSYSHOST host,
  UINT8 socket
  )
/*++

  Core to OEM platform hook call to release any platform clamps affecting CKE
  and/or DDR Reset

  @param host      - pointer to sysHost structure

--*/
{
  PlatformReleaseADRClamps (host, socket);
}

void
CoreCheckPORCompat (
  PSYSHOST host
  )
/*++

  CoreCheckPORCompat - Core to platform call of an OEM hook to restrict supported configurations if required.

  @param host  - Pointer to sysHost

  @retval N/A

--*/
{
  PlatformCheckPORCompat (host);
}

void
CoreLookupFreqTable (
  PSYSHOST host,
  struct DimmPOREntryStruct **freqTablePtr,
  UINT16 *freqTableLength
  )
/*++

  CoreLookupFreqTable - core call to OEM hook for overriding the memory POR frequency table

  @param freqTablePtr    - Pointer to Intel POR memory frequency table
  @param freqTableLength - Pointer to number of DimmPOREntryStruct entries in POR Frequency Table

  @retval N/A

--*/
{
  OemLookupFreqTable (host, freqTablePtr, freqTableLength);
}

/*++

  Calls platform specific method to program input parameters for the MRC

  @param setup - Pointer to the setup host structure

  @retval N/A

--*/
void
GetMemSetupOptionsCore (
  PSYSHOST host
  )
{
  //
  //Call platform specific method to get Setup Options
  //
  GetMemSetupOptions(host);
#ifdef RC_SIM
  // Override default setup parameters from INI file
  InitSetupOptions(host);
#endif
}

void
CoreGetSetupOptions(
  PSYSHOST host
  )
/*++

  Core call to platform Processor specific input setup

  @param host - pointer to host structure

  @retval N/A

--*/
{
  GetSetupOptionsEx(host);
}

void
CoreGetCpuSetupOptions (
  struct sysSetup *setup
  )
{
  GetCpuSetupOptions(setup);
}
