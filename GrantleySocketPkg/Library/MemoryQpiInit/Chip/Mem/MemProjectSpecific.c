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
 *      This file contains memory detection and initialization for
 *      IMC and DDR3 modules compliant with JEDEC specification.
 *
 ************************************************************************/

#include "SysFunc.h"
#include "Cpgc.h"

// Disable warning for unsued input parameters
//
#pragma warning(disable : 4100)

#define  MCODT_50       0
#define  MCODT_100      1

#define  MCODT_ACT      1
#define  ODT_ACT        1
#define  NO_ACT         0

#include "DdrOdtValueTable.h"
#include "DdrOdtActivationTable.h"

//#include "Ddr4OdtValueTable.h"
#include "Ddr4OdtActivationTable.h"


extern const UINT32 rxOffsetTable[MAX_STROBE];

// PRIVATE Prototypes
BOOLEAN DidChipClearAEPDimmParityErrors(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, PCHIP_CLEAR_PARITY_RESULTS_STRUCT chipClearParityResStruct);
BOOLEAN ShouldChipClearPtyAltWriteMR5(PSYSHOST host);
void DoChipSimClearParityResultsAltPath(PSYSHOST host, UINT8 socket);
void PerformChipSpecificNormalCMDTimingSetup(PSYSHOST host, UINT8 socket, UINT8 ch, PCHIP_SET_NORMAL_CMD_TIMING_STRUCT chipSetNormalCMDTimingStruct);

VOID
SetCpgcPatControl (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     mux0,
  UINT8     mux1,
  UINT8     mux2,
  UINT32    patbuf0,
  UINT32    patbuf1,
  UINT32    patbuf2
)
{
  CPGC_PATWDBCL_MUXCTL_MCDDC_DP_HSX_BDX_STRUCT  patWDBCLMuxCtl;

  patWDBCLMuxCtl.Data = MemReadPciCfgEp (host, socket, ch, CPGC_PATWDBCL_MUXCTL_MCDDC_DP_REG);
  patWDBCLMuxCtl.Bits.mux0 = mux0;
  patWDBCLMuxCtl.Bits.mux1 = mux1;
  patWDBCLMuxCtl.Bits.mux2 = mux2;
  MemWritePciCfgEp (host, socket, ch, CPGC_PATWDBCL_MUXCTL_MCDDC_DP_REG,    patWDBCLMuxCtl.Data);
  MemWritePciCfgEp (host, socket, ch, CPGC_PATWDBCL_MUX0_PBRD_MCDDC_DP_REG, patbuf0);
  MemWritePciCfgEp (host, socket, ch, CPGC_PATWDBCL_MUX1_PBRD_MCDDC_DP_REG, patbuf1);
  MemWritePciCfgEp (host, socket, ch, CPGC_PATWDBCL_MUX2_PBRD_MCDDC_DP_REG, patbuf2);
  MemWritePciCfgEp (host, socket, ch, CPGC_PATWDBCL_MUX0_PBWR_MCDDC_DP_REG, patbuf0);
  MemWritePciCfgEp (host, socket, ch, CPGC_PATWDBCL_MUX1_PBWR_MCDDC_DP_REG, patbuf1);
  MemWritePciCfgEp (host, socket, ch, CPGC_PATWDBCL_MUX2_PBWR_MCDDC_DP_REG, patbuf2);
}

UINT32
GetErrorResults (
  PSYSHOST   host,
  UINT8      socket,
  UINT8      ch
)
{
  return MemReadPciCfgEp (host, socket, ch, CPGC_ERR_BYTE_MCDDC_CTL_REG);
}

UINT8
GetWrDqDqsOffset (
  PSYSHOST   host
)
{
  UINT8 Offset;

  if (IsBDXEP(host) || (IsDESku(host) && CheckSteppingGreaterThan(host, CPU_BDX, B0_REV_BDX))) {
    Offset = WR_DQ_DQS_BASIC_OFFSET_BDX;
  } else {
    Offset = WR_DQ_DQS_BASIC_OFFSET_HSX;
  }

  return Offset;
}

VOID
SetMprTrainMode (
  PSYSHOST    host,
  UINT8       socket,
  UINT8       ch,
  UINT8       enable
)
{
  CPGC_MISCODTCTL_MCDDC_CTL_HSX_BDX_STRUCT cpgcMiscODTCtl;
  cpgcMiscODTCtl.Data = MemReadPciCfgEp (host, socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG);
  cpgcMiscODTCtl.Bits.mpr_train_ddr_on = enable;
  MemWritePciCfgEp (host, socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG, cpgcMiscODTCtl.Data);
}

UINT8
GetChipParamType (
  PSYSHOST    host,
  UINT8       Param
)
{
  UINT8 ParamType = DEFAULT_PARAM;

  switch(Param){
  case traindramron:
    ParamType        = PER_RANK;
    break;
  case trainmcodt:
    ParamType       = PER_MC;
    break;
  case trainnontgtodt:
    ParamType       = PER_CH_PER_DIMM;
    break;
  case trainrttwr:
    ParamType       = PER_RANK;
    break;
  case trainmcron:
    ParamType       = PER_MC;
    break;
  case traintxeq:
    ParamType       = PER_STROBE;
    break;
  case trainimode:
    ParamType       = PER_CH_PER_BYTE;
    break;
  case trainctle:
    ParamType       = PER_STROBE;
    break;
  default:
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "GetChipParamType called unknown parameter type\n"));
    break;
  }
  return ParamType;
}

UINT32
GetStickyScratchpad (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     scratchpad
  )
{
  return ReadCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG + (scratchpad * 4));
} // GetStickyScratchpad

void
SetStickyScratchpad (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     scratchpad,
  UINT32    data
  )
{
  WriteCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG + (scratchpad * 4), data);
} // SetStickyScratchpad

UINT32
GetNonStickyScratchpad (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     scratchpad
  )
{
  return ReadCpuPciCfgEx (host, socket, 0, BIOSNONSTICKYSCRATCHPAD0_UBOX_MISC_REG + (scratchpad * 4));
} // GetNonStickyScratchpad

void
SetNonStickyScratchpad (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     scratchpad,
  UINT32    data
  )
{
  WriteCpuPciCfgEx (host, socket, 0, BIOSNONSTICKYSCRATCHPAD0_UBOX_MISC_REG + (scratchpad * 4), data);
} // SetNonStickyScratchpad

UINT32
GetDataTrainFeedback (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     strobe
)
{
  return MemReadPciCfgEp (host, socket, ch, UpdateIoRegisterCh (host, strobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG));
}

void
SetActToActPeriod(
           PSYSHOST        host,
           UINT8           socket,
           UINT8           ch,
           UINT32          tcrapOrg[MAX_CH]
)
{
  TCRAP_MCDDC_CTL_HSX_BDX_STRUCT tcrap;

  tcrap.Data       = MemReadPciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG);

  if (tcrapOrg[ch] == 0){
    tcrapOrg[ch]   = tcrap.Data;
  }

  tcrap.Bits.t_rrd = 4;
  MemWritePciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG, tcrap.Data);
} // SetActToActPeriod

void
SetIOLatencyComp(
                  PSYSHOST  host,
                  UINT8     socket,
                  UINT8     ch,
                  UINT8     rtError,
                  UINT32    tcrapOrg[MAX_CH]
)
{
    IOLATENCY1_MCDDC_DP_BDX_STRUCT    ioLatency1;
    struct channelNvram               (*channelNvList)[MAX_CH];
    channelNvList = GetChannelNvList(host, socket);

    // If not error flag, set IO Latency Compensation for normal operation
    if (!rtError) {
      ioLatency1.Data = MemReadPciCfgEp (host, socket, ch, IOLATENCY1_MCDDC_DP_REG);
      ioLatency1.Bits.io_lat_io_comp = MRC_ROUND_TRIP_IO_COMPENSATION;
      //
      // HSD 4168054: Set IO Comp to 15 for ch 2/3 in 1HA mode
      //
      if (Is1HA(host) && (ch > 1)) {
        ioLatency1.Bits.io_lat_io_comp++;
      }

      //
      // Additional latency required in 2:2 mode
      //
      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        ioLatency1.Bits.io_lat_io_comp += 2;
      }

      MemWritePciCfgEp (host, socket, ch, IOLATENCY1_MCDDC_DP_REG, ioLatency1.Data);
      (*channelNvList)[ch].ioLatency1 = ioLatency1.Data;
    } else {
      DisableChannel(host, socket, ch);
    }

    MemWritePciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG, tcrapOrg[ch]);
} // SetIOLatencyComp

void
ReceiveEnableCleanup(
  PSYSHOST host,
  UINT8    socket
)
{
} // ReceiveEnableCleanup

void
SetSenseAmpODTDelays (
              PSYSHOST  host,
              UINT8     socket
              )
{
  UINT8                                ch;
  UINT8                                dimm;
  UINT8                                rank;
  UINT8                                strobe;
  INT16                                delay;
  UINT16                               MaxRcven;
  struct channelNvram                  (*channelNvList)[MAX_CH];
  struct dimmNvram                     (*dimmNvList)[MAX_DIMM];
  DATACONTROL1_0_MCIO_DDRIO_BDX_STRUCT dataControl1;

  if (IsDESku(host)) {
    channelNvList = GetChannelNvList(host, socket);
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "\n Starting Senseamp and ODT delay calculations \n"));
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "------------------------------------------------------------ \n"));
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      dimmNvList = GetDimmNvList(host, socket, ch);

      for (strobe = 0; strobe < MAX_STROBE; strobe++) {
        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

        // clear out min and max values
        MaxRcven=0;
        for (dimm = 0; dimm < MAX_DIMM; dimm++) {

          //for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
          for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
            if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

            GetSetDataGroup (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY, &delay);
            if ((UINT16)delay > MaxRcven) {
              MaxRcven = (UINT16)delay;
            }
          } //rank

        } //dimm

        //
        //program new values
        //

        //read cached values
        dataControl1.Data = (*channelNvList)[ch].dataControl1[0];

        //modify with new values
      
        if ((MaxRcven & (BIT8 + BIT7)) == (BIT8 + BIT7)) {
          dataControl1.Bits.odtdelay          =  0xF;
          dataControl1.Bits.senseampdelay     =  0xF;
        }
      
        if ((MaxRcven & (BIT8 + BIT7 + BIT6)) == (BIT8 + BIT7 + BIT6)) {
          dataControl1.Bits.odtdelay          =  0x0;
          dataControl1.Bits.senseampdelay     =  0x0;
        }

        //write new values
        (*channelNvList)[ch].dataControl1[0] =  dataControl1.Data;
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL1_0_MCIO_DDRIO_REG), dataControl1.Data);
      
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, strobe, NO_BIT,
             "MaxRcven=%3u,  New Senseamp/Odt delay=%u\n", MaxRcven, dataControl1.Bits.odtdelay));

      } //strobe
    } //ch
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "\n\n"));
  }
}  // SetSenseAmpODTDelays

void
SetLongPreamble(
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  BOOLEAN  set
)
{

  UINT8                             strobe;
  DATACONTROL0_0_MCIO_DDRIO_STRUCT  dataControl0;
  struct channelNvram               (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);
  if (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG))) {
    dataControl0.Data = (*channelNvList)[ch].dataControl0;
    dataControl0.Bits.longpreambleenable = 1;

    for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
      //
      // Skip if this is an ECC strobe when ECC is disabled
      //
      if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
      if (set == TRUE) {
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
      } else {
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), (*channelNvList)[ch].dataControl0);
      } // if set
    } // strobe loop
  } // skip training
} // SetLongPreamble

void
DecIoLatency(
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank,
  UINT16   centerPoint[MAX_CH][MAX_STROBE]
)
{
} // DecIoLatency

void
SetupRecEnFineParam(
                     PSYSHOST host,
                     UINT8 *loopCount,
                     UINT8 *numCl
)
{
  if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
    *loopCount = 1;
  } else {
    *loopCount = REC_EN_LOOP_COUNT;
  }
  *numCl     = 8;
} // SetupRecEnFineParam

void
SetupPiSettingsParams(
                 PSYSHOST host,
                 UINT8    socket,
                 UINT8    step,
                 UINT16  *piSetting,
                 UINT16  *piStart,
                 UINT16  *NumberOfCycle,
                 struct  TrainingResults  (**trainRes)[MAX_CH][MAX_STROBE]
)
{
  if (step == FPT_PI_WRITE_TYPE) {
    if ((host->nvram.mem.socket[socket].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {
      *piStart = 128;
    } else {
      *piStart = WR_LVL_PI_START;
    }
    *piSetting     = *piStart;
    *NumberOfCycle = *piStart + WR_LVL_PI_RANGE;
#ifdef LRDIMM_SUPPORT
  } else if (step == FPT_PI_LRDIMM_READ_TYPE) {
    *piSetting     = LRDIMM_BACKSIDE_PI_START;
    *piStart       = LRDIMM_BACKSIDE_PI_START;
    *NumberOfCycle = LRDIMM_BACKSIDE_PI_START + LRDIMM_BACKSIDE_PI_RANGE;
    *trainRes       = &host->var.mem.lrTrainRes;
  } else if (step == FPT_PI_LRDIMM_WRITE_TYPE) {
    *piSetting     = LRDIMM_BACKSIDE_PI_START;
    *piStart       = LRDIMM_BACKSIDE_PI_START;
    *NumberOfCycle = LRDIMM_BACKSIDE_PI_START + LRDIMM_BACKSIDE_PI_RANGE;
    *trainRes       = &host->var.mem.lrDwlTrainRes;
  } else if (step == FPT_PI_LRDIMM_RD_MRD_TYPE) {
    *piSetting     = LRDIMM_BACKSIDE_PI_START;
    *piStart       = LRDIMM_BACKSIDE_PI_START;
    *NumberOfCycle = LRDIMM_BACKSIDE_PI_START + LRDIMM_BACKSIDE_PI_RANGE;
    *trainRes       = &host->var.mem.lrMrdTrainRes;
  } else if (step == FPT_PI_LRDIMM_WR_MRD_TYPE) {
    *piSetting     = LRDIMM_BACKSIDE_PI_START;
    *piStart       = LRDIMM_BACKSIDE_PI_START;
    *NumberOfCycle = LRDIMM_BACKSIDE_PI_START + LRDIMM_BACKSIDE_PI_RANGE;
    *trainRes       = &host->var.mem.lrMwdTrainRes;
#endif  //LRDIMM_SUPPORT
  } else {
    *piSetting     = REC_EN_PI_START;
    *piStart       = REC_EN_PI_START;
    *NumberOfCycle = REC_EN_PI_START + REC_EN_PI_RANGE;
  }
} // SetupPiSettings

UINT8
GetTrainingResultIndex(
                       UINT8  step,
                       UINT16 piSetting,
                       UINT16 piStart
)
{
  UINT8 dWord;
  dWord = (UINT8)((piSetting - piStart) / 32);
  return dWord;
} // GetTrainingResultIndex

UINT32
ReadTrainFeedback (
                   PSYSHOST host,
                   UINT8    socket,
                   UINT8    ch,
                   UINT8    strobe
)
{
  UINT32 feedback;

  if (strobe < 9) {
    feedback = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG)) & 0x1FF;
  } else {
    feedback = (MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe - 9, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG)) >> 9) & 0x1FF;
  }
  return feedback;

} // ReadTrainFeedback

void
RecEnTrainFeedback (
                    PSYSHOST host,
                    UINT8    socket,
                    UINT8    ch,
                    UINT8    strobe,
                    UINT32   csrReg,
                    UINT16   piDelay
)
{
  UINT8  step;
  UINT8  dWord;
  UINT8  minGood;
  struct TrainingResults  (*trainRes)[MAX_CH][MAX_STROBE];

  trainRes = &host->var.mem.trainRes;
  minGood  = SetMinGoodInit(host);
  dWord    = (UINT8)((piDelay - REC_EN_PI_START) / 32);

  if ((csrReg & 0x1FF) >= minGood) {
    for (step = 0; step < REC_EN_STEP_SIZE; step++) {
      (*trainRes)[ch][strobe].results[dWord] = (*trainRes)[ch][strobe].results[dWord] | (1 << ((piDelay + step) % 32));
    } // step loop
  }
} // RecEnTrainFeedback

UINT16
StartBackOver (
                UINT8  step,
                UINT16 piStart
)
{
  return piStart;
} // StartBackOver

void
SetInitRoundTrip(
                 PSYSHOST host,
                 UINT8    socket,
                 UINT8    ch,
                 UINT8    dimm,
                 UINT8    rank
)
{
  UINT8  rtDelay;
  UINT8  logRank;
  struct ddrRank (*rankList)[MAX_RANK_DIMM];
  struct RankCh  (*rankPerCh)[MAX_RANK_CH];

  rankList  = &host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].rankList;
  rankPerCh = &host->nvram.mem.socket[socket].channelList[ch].rankPerCh;
  //
  // Get the logical rank #
  //
  logRank = GetLogicalRank(host, socket, ch, dimm, rank);

  // Default ROUNDT_LAT = HW_ROUNDT_LAT_DEFAULT_VALUE + nMode value*2 + 8* tCL
  rtDelay = MRC_ROUND_TRIP_DEFAULT_VALUE;
  //
  // More RTL is required at higher speeds
  //
  if(host->nvram.mem.socket[socket].ddrFreq >= DDR_2400) {
    rtDelay = rtDelay + 10;
  }
  (*rankPerCh)[(*rankList)[rank].rankIndex].Roundtrip = rtDelay;
  SetRoundTrip (host, socket, ch, logRank, rtDelay);

  (*rankPerCh)[(*rankList)[rank].rankIndex].IOLatency = MRC_IO_LATENCY_DEFAULT_VALUE;
  if (host->nvram.mem.socket[socket].ddrFreq > DDR_2400) {
    (*rankPerCh)[(*rankList)[rank].rankIndex].IOLatency += 2;
  }
  SetIOLatency (host, socket, ch, logRank, (*rankPerCh)[(*rankList)[rank].rankIndex].IOLatency);
} // SetInitRoundTrip

void
SetIoLatComp(
             PSYSHOST host,
             UINT8    socket,
             UINT8    ch
)
{
    struct channelNvram             (*channelNvList)[MAX_CH];
    IOLATENCY1_MCDDC_DP_BDX_STRUCT    ioLatency1;

    channelNvList = GetChannelNvList(host, socket);
    // IO Latency Compensation starting point
    ioLatency1.Data = MemReadPciCfgEp (host, socket, ch, IOLATENCY1_MCDDC_DP_REG);
    ioLatency1.Bits.io_lat_io_comp = MRC_ROUND_TRIP_IO_COMP_START;
    //
    // HSD 4168054: Set IO Comp to 15 for ch 2/3 in 1HA mode
    //
    if (Is1HA(host) && (ch > 1)) {
      ioLatency1.Bits.io_lat_io_comp++;
    }

    //
    // Additional latency required in 2:2 mode
    //
    if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
      ioLatency1.Bits.io_lat_io_comp += 2;
    }

    MemWritePciCfgEp (host, socket, ch, IOLATENCY1_MCDDC_DP_REG, ioLatency1.Data);
    (*channelNvList)[ch].ioLatency1 = ioLatency1.Data;
} // SetIoLatComp

UINT8
SetMinGoodInit (
  PSYSHOST host
)
{
  if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
    return 1;
  } else {
    return (UINT8)(1 << (REC_EN_LOOP_COUNT - 1));
  }
} // SetMinGoodInit

//
// MemDDR4Lrdimm project specific
//

UINT32
LrDimmReadTrainFeedback (
                   PSYSHOST host,
                   UINT8    socket,
                   UINT8    ch,
                   UINT8    strobe,
                   UINT8    bit
)
{
  UINT32 csrReg;
  UINT32 feedback;

  if (strobe < 9) {
    csrReg = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG)) & 0x1FF;
  } else {
    csrReg = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe - 9, DATATRAINFEEDBACKN1_0_MCIO_DDRIO_REG)) & 0x1FF;
  }

  if (bit == 0xFF) {
    feedback = ((csrReg >> ((strobe/9)*4)) & 0xF);
  } else {
    feedback = ((csrReg&0x01) << bit);
  }
  return feedback;

} // LrDimmReadTrainFeedback

UINT8
IsLrdimmPresent (
                    PSYSHOST host,
                    UINT8    socket,
                    UINT8    ch,
                    UINT8    dimm
)
{
  struct channelNvram (*channelNvList)[MAX_CH];
  channelNvList = GetChannelNvList(host, socket);
  return ((*channelNvList)[ch].lrDimmPresent);
} // IsLrdimmPresent

void
SetDisable2cycBypass (
                       PSYSHOST host,
                       UINT8    socket,
                       UINT8    data
)
{
    UINT8 ch;
    MCSCHED_CHKN_BIT_MCDDC_CTL_HSX_BDX_STRUCT chknBit;
    struct channelNvram                      (*channelNvList)[MAX_CH];

    chknBit.Data = 0;

    channelNvList = GetChannelNvList(host, socket);

    if (!(CheckSteppingEqual(host, CPU_HSX, A0_REV_HSX))) {
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      chknBit.Data = MemReadPciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG);
      chknBit.Bits.dis_2cyc_byp = data;
      MemWritePciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG, chknBit.Data);
    } //ch
  }
} // Disable2cycBypass

//
// memODT project specific
//
UINT32  rdOdtTableRegs[MAX_DIMM] = {
                                     RD_ODT_TBL0_MCDDC_CTL_REG,
                                     RD_ODT_TBL1_MCDDC_CTL_REG,
                                     RD_ODT_TBL2_MCDDC_CTL_REG
                                   };

UINT32  wrOdtTableRegs[MAX_DIMM] = {
                                     WR_ODT_TBL0_MCDDC_CTL_REG,
                                     WR_ODT_TBL1_MCDDC_CTL_REG,
                                     WR_ODT_TBL2_MCDDC_CTL_REG
                                   };

UINT16
SetOdtConfigInOdtValueIndex (
                              PSYSHOST host,
                              UINT8    socket
)
{
  ODT_VALUE_INDEX config;
  // Construct config index
  config.Data = 0;
  config.Bits.freq = GetClosestFreq(host, socket);
  if(config.Bits.freq > DDR_2133) {
    config.Bits.freq = DDR_2133;
  }
  return config.Data;
} // SetOdtConfigInOdtValueIndex

//---------------------------------------------------------------

/**

Routine Description:

  Program ODT Timing

Arguments:

  @param host        - Pointer to sysHost
  @param socket        - Socket number
  @param ch          - Channel number (0-based)

  @retval N/A

**/
void
SetOdtTiming (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch
  )
{
  RD_ODT_TBL2_MCDDC_CTL_STRUCT rdOdtTiming;
  WR_ODT_TBL2_MCDDC_CTL_STRUCT wrOdtTiming;


  //
  // Write the Read and Write ODT Activates to the Matrix registers
  //
  rdOdtTiming.Data = MemReadPciCfgEp (host, socket, ch, RD_ODT_TBL2_MCDDC_CTL_REG);
  wrOdtTiming.Data = MemReadPciCfgEp (host, socket, ch, WR_ODT_TBL2_MCDDC_CTL_REG);

  rdOdtTiming.Bits.extraleadingodt = ODT_RD_CYCLE_LEADING;

  if ((host->nvram.mem.socket[socket].channelList[ch].lrDimmPresent) &&
    (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {
      rdOdtTiming.Bits.extratrailingodt = host->nvram.mem.socket[socket].channelList[ch].common.nMDD;
  } else {
    rdOdtTiming.Bits.extratrailingodt = ODT_RD_CYCLE_TRAILING;
  }

  wrOdtTiming.Bits.extra_leading_odt = ODT_WR_CYCLE_LEADING;
  if ((host->nvram.mem.socket[socket].channelList[ch].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR4)) {
    wrOdtTiming.Bits.extra_trailing_odt = DDR4_LRDIMM_ODT_WR_CYCLE_TRAILING;
  } else {
    wrOdtTiming.Bits.extra_trailing_odt = ODT_WR_CYCLE_TRAILING;
    if (host->nvram.mem.WrCRC) {
      if (wrOdtTiming.Bits.extra_trailing_odt < 3) {
        wrOdtTiming.Bits.extra_trailing_odt += 1;
      } else {
        host->nvram.mem.WrCRC = 0;
        MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "No More room to increase trailing ODT. Disabling CRC.\n"));
        OutputWarning (host, WARN_WRCRC_DISABLE, WARN_TRAIL_ODT_LIMIT_REACHED, socket, ch, 0xFF, 0xFF);
      }
    }
  }

  //
  // Write the Read and Write ODT Activates to the Matrix registers
  //
  MemWritePciCfgEp (host, socket, ch, RD_ODT_TBL2_MCDDC_CTL_REG, rdOdtTiming.Data);
  MemWritePciCfgEp (host, socket, ch, WR_ODT_TBL2_MCDDC_CTL_REG, wrOdtTiming.Data);
} // SetOdtTiming

void
SetDdr3OdtMatrix (
                   PSYSHOST host,
                   UINT8    socket,
                   UINT8    ch,
                   UINT8    dimm,
                   UINT8    rank
)
{

  UINT8  tableVal;
#ifdef SERIAL_DBG_MSG
  UINT8 rtt_wr_lookup[] = {0, 60, 120};
  UINT8 rtt_nom_lookup[] = {0, 60, 120, 40, 20, 30};
#endif //SERIAL_DBG_MSG
  struct odtValueStruct *odtValuePtr = NULL;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  struct ddrRank (*rankList)[MAX_RANK_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = &host->nvram.mem.socket[socket].channelList;
  dimmNvList    = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
  rankList      = &host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].rankList;

  odtValuePtr = LookupOdtValue(host, socket, ch);
  if (odtValuePtr == NULL) //Klocwork warnings fix: It shouldn't happen (such rank is disabled), but to be sure...
  return;

  // Get the ODT values
  tableVal = odtValuePtr->dramOdt[dimm][rank];

#ifdef  LRDIMM_SUPPORT
  if ((*channelNvList)[ch].lrDimmPresent) {

    // LRDIMM uses SPD data for DRAM Rtt_nom and Rtt_wr fields
    if (rank < 2) {
      tableVal = ((*dimmNvList)[dimm].spdLrBuf_MR1_2_RTT >> 2) & 0x37;
    } else {
      // DRAM Rtt_nom must be disabled for ranks > 1
      tableVal = ((*dimmNvList)[dimm].spdLrBuf_MR1_2_RTT >> 2) & 0x30;
    } // end if rank
  } // end if LRDIMM
#endif  //LRDIMM_SUPPORT

  MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                "DRAM Rtt_wr = %d, Rtt_nom = %d\n", rtt_wr_lookup[tableVal >> 4], rtt_nom_lookup[tableVal & 0xF]));

  // Set encodings for DRAM Mode Registers
  (*rankList)[rank].RttWr = 0;
  if (tableVal & (RTT_WR_60 << 4))  (*rankList)[rank].RttWr |= RTT_WR_ODT_60;
  if (tableVal & (RTT_WR_120 << 4)) (*rankList)[rank].RttWr |= RTT_WR_ODT_120;

  (*rankList)[rank].RttNom = 0;
  if (tableVal & RTT_NOM_60)  (*rankList)[rank].RttNom |= RTT_NOM_ODT_60;
  if (tableVal & RTT_NOM_120) (*rankList)[rank].RttNom |= RTT_NOM_ODT_120;
  if (tableVal & RTT_NOM_20)  (*rankList)[rank].RttNom |= RTT_NOM_ODT_20;
} // SetDdr3OdtMatrix

//---------------------------------------------------------------
struct odtValueStruct *LookupOdtValue (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch
  )
{
   ODT_VALUE_INDEX config;
   UINT32 i;

   // Construct config index
   config = GetOdtValueIndex (host, socket, ch, DDR_1867);

   MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "config.Bits.freq = %d\nconfig.Bits.slot0 = %d\nconfig.Bits.slot1 = %d\nconfig.Bits.slot2 = %d\n",
                 config.Bits.freq, config.Bits.slot0, config.Bits.slot1, config.Bits.slot2));

   // Now lookup the config...
   for (i = 0; i < sizeof(odtValueTable) / sizeof(struct odtValueStruct); i++) {
      if (odtValueTable[i].config == config.Data) {
         break;
      }
   }
   // Check for error
   if (i >= sizeof(odtValueTable) / sizeof(struct odtValueStruct)) {
      OutputWarning (host, WARN_DIMM_COMPAT, WARN_CHANNEL_CONFIG_NOT_SUPPORTED, socket, ch, 0xFF, 0xFF);
      return NULL;
   }

   // Return pointer
   return &odtValueTable[i];
} // LookupOdtValue

void
GetOdtActTablePointers (
                         PSYSHOST host,
                         UINT32   *odtActTableSize,
                         struct odtActStruct **odtActTablePtr
)
{
   *odtActTableSize = sizeof(odtActTable);
   *odtActTablePtr = odtActTable;

   if (host->nvram.mem.dramType != SPD_TYPE_DDR3) {
     *odtActTablePtr = ddr4OdtActTable;
     *odtActTableSize = sizeof(ddr4OdtActTable);
   } // if DDR4
} // GetOdtActTablePointers

void
SetOdtActMatrixRegs (
                   PSYSHOST host,
                   UINT8    socket,
                   UINT8    ch,
                   UINT8    dimm,
                   UINT32   rdOdtTableData,
                   UINT32   wrOdtTableData
)
{

  //
  // Write the Read and Write ODT Activates to the Matrix registers
  //
  MemWritePciCfgEp (host, socket, ch, rdOdtTableRegs[dimm], rdOdtTableData);
  MemWritePciCfgEp (host, socket, ch, wrOdtTableRegs[dimm], wrOdtTableData);
} // OdtActMatrixRegs


//------------------------------------------------------------------------------------

/**

  Chip specific setup to set risingEdge offset in simulation. Only implemented differently in Groveport because of simulation limitation

  @param risingEdge  - Pointer to the risingEdge array
  @param ch  - processor socket channel number
  @param strobe  - data strobe

  @retval N/A

**/
void
SetChipSimRisingEdge(
  UINT16 risingEdge[MAX_CH][MAX_STROBE],
  UINT8 ch,
  UINT8 strobe
  )
{
  //Does the same as non sim path. Needed to add for Groveport compatability.
  risingEdge[ch][strobe] = risingEdge[ch][strobe] -64;
}

/**

  setup to do before TxPiSample() main body. Grantley does not have any special setup to do

  @param VOID

  @retval N/A

**/
void
TxPiSampleChipInit(
  PCHIP_DDRIO_STRUCT dataControl
  )
{
  dataControl->dataControl0.Data = 0;
  dataControl->dataControl2.Data = 0;
  dataControl->dataControl0.Bits.wltrainingmode  = 1;
  dataControl->dataControl0.Bits.readrfwr        = 1;
  dataControl->dataControl0.Bits.txpion          = 1;
  dataControl->dataControl2.Bits.forcebiason     = 1;
}

void
GetChipLogicalRank(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank,
  PCHIP_DDRIO_STRUCT dataControl
  )
{
  dataControl->dataControl0.Bits.readrfrank =  GetLogicalRank(host, socket, ch, dimm, rank);
}

/**

  Chip specific setup to set Pi training mode

  @param host  - Pointer to the system host (root) structure
  @param socket  - processor socket ID
  @param ch  - processor socket channel number
  @param strobe  - processor socket channel strobe number
  @param dataControl0data - training mode data

  @retval N/A

**/
void
SetChipTxPiSampleTrainingMode(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_DDRIO_STRUCT dataControl
  )
{
  UINT8 strobe;
  struct channelNvram     (*channelNvList)[MAX_CH];
  channelNvList = &host->nvram.mem.socket[socket].channelList;

  for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
    if ((!host->nvram.mem.eccEn) && (strobe == 8)) continue;
    //
    // Set training mode and rank to all channels
    //
    MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL2_0_MCIO_DDRIO_REG), dataControl->dataControl2.Data | (*channelNvList)[ch].dataControl2[strobe]);
    MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl->dataControl0.Data | (*channelNvList)[ch].dataControl0);
  } // strobe loop
}

/**

  Chip specific setup to initialize loop indices when performing a pidelay step

  @param host  - Pointer to the system host (root) structure
  @param socket  - processor socket ID
  @param piDelayStart  - Pointer to the loop start index
  @param piDelayEnd  - Pointer to the loop end index

  @retval N/A

**/
void
GetChipTiPiSamplepiDelayStartandEnd(
  PSYSHOST host,
  UINT8 socket,
  INT16* piDelayStart,
  UINT16* piDelayEnd
  )
{
  if ((host->nvram.mem.socket[socket].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {
    *piDelayStart = 128;
  } else {
    *piDelayStart = WR_LVL_PI_START;
  }

  *piDelayEnd = *piDelayStart + WR_LVL_PI_RANGE;
}

/**

  Chip specific setup for TxPiSample TXDQS Strobe training, part 1

  @param host  - Pointer to the system host (root) structure
  @param socket  - processor socket ID
  @param ch  - Processor socket Channel number
  @param dimm  - Processor socket channel dimm number
  @param rank - Processor socket channel dim rank number
  @param nibble - Processor socket channel dim rank nibble number
  @param piDelay - piDelay for nibble

  @retval N/A

**/
void
SetChipTxPiSampleTrainingTXDQSStrobesI(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank,
  INT16 piDelay,
  PCHIP_DDRIO_STRUCT dataControl
  )
{
  UINT8 strobe;
  struct channelNvram     (*channelNvList)[MAX_CH];
  channelNvList = &host->nvram.mem.socket[socket].channelList;

  for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
    if ((!host->nvram.mem.eccEn) && (strobe == 8)) continue;
    //
    // Set training mode and rank to all channels
    //
    MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), (*channelNvList)[ch].dataControl0);
  } // strobe loop

  GetSetDataGroup (host, socket, ch, dimm, rank, ALL_STROBES, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE, (INT16 *)&piDelay);
}

/**

  Chip specific setup for TxPiSample TXDQS Strobe training, part 2

  @param host  - Pointer to the system host (root) structure
  @param socket  - processor socket ID
  @param ch  - Processor socket Channel number
  @param dimm  - Processor socket channel dimm number
  @param dataControl0data - training mode data

  @retval N/A

**/
void
SetChipTxPiSampleTrainingTXDQSStrobesII(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  PCHIP_DDRIO_STRUCT dataControl
  )
{
  UINT8 strobe;
  struct channelNvram     (*channelNvList)[MAX_CH];
  channelNvList = &host->nvram.mem.socket[socket].channelList;

  for (strobe = 0; strobe < (MAX_STROBE / 2); strobe++) {
    if ((!host->nvram.mem.eccEn) && (strobe == 8)) continue;
    //
    // Set training mode and rank to all channels
    //
    MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl->dataControl0.Data | (*channelNvList)[ch].dataControl0);
  } // strobe loop
}

void
EnableChipRecieveEnableMode(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 strobe,
  PEXECUTE_CTL_CLK_TEST_CHIP_STRUCT executeCtlCLKTestChipStruct
  )
{
  MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), executeCtlCLKTestChipStruct->dataControl0.Data);
}

void
EnableChipRecieveEnableModeII(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 strobe,
  PEXECUTE_CTL_CLK_TEST_CHIP_STRUCT executeCtlCLKTestChipStruct
  )
{
  struct channelNvram  (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), (*channelNvList)[ch].dataControl0);
}

void
PerformChipODTMPRDimmActivationStep(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  PEXECUTE_CTL_CLK_TEST_CHIP_STRUCT executeCtlCLKTestChipStruct
  )
{
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];

  dimmNvList  = GetDimmNvList(host, socket, ch);

  executeCtlCLKTestChipStruct->cpgcMiscODTCtl.Data = MemReadPciCfgEp (host, socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG);
  if (!(*dimmNvList)[dimm].aepDimmPresent) {
    executeCtlCLKTestChipStruct->cpgcMiscODTCtl.Bits.mpr_train_ddr_on = 1;
  }
  MemWritePciCfgEp (host, socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG, executeCtlCLKTestChipStruct->cpgcMiscODTCtl.Data);
}

void
DisableChipChlRecEnOffsetTrainingMode(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  PEXECUTE_CTL_CLK_TEST_CHIP_STRUCT executeCtlCLKTestChipStruct
  )
{
  executeCtlCLKTestChipStruct->cpgcMiscODTCtl.Data = MemReadPciCfgEp (host, socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG);
  executeCtlCLKTestChipStruct->cpgcMiscODTCtl.Bits.mpr_train_ddr_on = 0;
  MemWritePciCfgEp (host, socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG, executeCtlCLKTestChipStruct->cpgcMiscODTCtl.Data);
}

/**

  Chip specific code path to determine if DDR4 LRDIM Host side training should be enabled. Disables legacy host side training if it has been enabled

  @param host  - Pointer to the system host (root) structure
  @param socket  - processor socket ID
  @param ch  - Processor socket Channel number
  @param dimm  - Processor socket channel dimm number
  @param rank - Processor socket channel dimm rank number

  @retval BOOLEAN - true if the chip should disable DDR4 LRDIM Host side training

**/
BOOLEAN
ShouldChipEnableDDR4LRDIMMHostSideTraining(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank
  )
{
  UINT8 cwData;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct odtValueStruct *odtValuePtr;

  channelNvList = &host->nvram.mem.socket[socket].channelList;
  dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

  if (((*channelNvList)[ch].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {

    // Write to LR buffer to enable WL mode and to override Rtt_nom

#ifdef IDT_LRBUF_WA
    dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
    // check for IDT B0 iMB
    if (((*dimmNvList)[dimm].lrbufDVid == LRBUF_IDT_DVID) &&
      ((*dimmNvList)[dimm].lrbufRid == LRBUF_IDT_RID_B0)) {
        // Sighting 200001
        WriteRcLrbufExtended(host, socket, ch, dimm, 0, 1, 0x80f8);
    }
#endif // IDT_LRBUF_WA

    // F0-RC12: Enable Write leveling mode
    // Includes wait for tMOD before enabling ODT
    cwData = BIT0;
    WriteRcLrbuf(host, socket, ch, dimm, 0, cwData, 0, 12);

    // F3-RC0: Connector DQ Rtt_nom = Rtt_wr
    odtValuePtr = LookupOdtValue(host, socket, ch);
    if ((odtValuePtr != NULL) && ((odtValuePtr->dramOdt[dimm][0] >> 4) & 7) != DRAM_ODT_DIS) {
      cwData = (odtValuePtr->dramOdt[dimm][0] >> 4) & 7;
      WriteRcLrbuf(host, socket, ch, dimm, 0, cwData, 3, 0);
    }
  } else if (((*channelNvList)[ch].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR4))
  {return TRUE;}

  return FALSE;
}

/**

  Chip specific code path to support each platforms individual set of RTT_NOM values

  @param host  - Pointer to the system host (root) structure
  @param socket  - processor socket ID
  @param ch  - Processor socket Channel number
  @param dimm  - Processor socket channel dimm number
  @param rank - Processor socket channel dimm rank number

  @retval N/A

**/
void
SetChipRTTPark(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank
  )
{
  UINT16              MR5;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct rankDevice   (*rankStruct)[MAX_RANK_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];

  channelNvList = &host->nvram.mem.socket[socket].channelList;
  rankList = &(*channelNvList)[ch].dimmList[dimm].rankList;
  rankStruct  = &host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct;

  if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && ((*rankList)[rank].RttWr & DDR4_RTT_WR_ODT_MASK)) {
    //
    // Get the current settings for MR5
    //
    MR5 = (*rankStruct)[rank].MR5;
    MR5 &= ~DDR4_RTT_PRK_ODT_MASK;
    // Set Rtt Park to the same value as Rtt Wr
    switch ((*rankList)[rank].RttWr >> DDR4_RTT_WR_ODT_SHIFT) {

    case DDR4_RTT_WR_120:
      MR5 |= (DDR4_RTT_NOM_120 << DDR4_RTT_PRK_ODT_SHIFT);
      break;

    case DDR4_RTT_WR_240:
      MR5 |= (DDR4_RTT_NOM_240 << DDR4_RTT_PRK_ODT_SHIFT);
      break;

    case DDR4_RTT_WR_INF:
      MR5 |= (DDR4_RTT_NOM_INF << DDR4_RTT_PRK_ODT_SHIFT);
      break;
    } // switch
    WriteMRS (host, socket, ch, dimm, rank, MR5, BANK5);
  } // ddr4
}

/**

  Chip specific code path to Disable any legacy LRDRIM WriteLeveling and return false, or return true if should disable DDR4 write leveling.

  @param host  - Pointer to the system host (root) structure
  @param socket  - processor socket ID
  @param ch  - Processor socket Channel number
  @param dimm  - Processor socket channel dimm number
  @param rank - Processor socket channel dimm rank number

  @retval BOOLEAN - true if the chip should disable DDR4 write leveling

**/
BOOLEAN
ShouldChipDisableDDR4LRDIMMWriteLeveling(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank
  )
{
  UINT8 cwData;
  struct rankDevice   (*rankStruct)[MAX_RANK_DIMM];
  struct odtValueStruct *odtValuePtr;

  rankStruct  = &host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct;

  if (IsLrdimmPresent(host, socket, ch, dimm) && (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {

    // Write to LR buffer to disable WL mode and to restore Rtt_nom

    // F3-RC0: Connector DQ Rtt_nom = Rtt_nom
    odtValuePtr = LookupOdtValue(host, socket, ch);
    if (odtValuePtr != NULL) {
      cwData = odtValuePtr->dramOdt[dimm][0] & 7;
      WriteRcLrbuf(host, socket, ch, dimm, 0, cwData, 3, 0);
    }

    // F0-RC12: Disable Write leveling mode
    cwData = 0;
    WriteRcLrbuf(host, socket, ch, dimm, 0, cwData, 0, 12);

  } else if (IsLrdimmPresent(host, socket, ch, dimm) && (host->nvram.mem.dramType == SPD_TYPE_DDR4) &&
    ((*rankStruct)[rank].CurrentLrdimmTrainingMode != LRDIMM_DWL_TRAINING_MODE) &&
    (host->var.mem.InPbaWaMode == 0))
  {return TRUE;}

  return FALSE;
}

/**

  Chip specific code path. Platforms supporting DDR3 require seperate code path for DDR4 that to restore RTT_PARK. DDR3 does not need to do this.

  @param host  - Pointer to the system host (root) structure
  @param socket  - processor socket ID
  @param ch  - Processor socket Channel number
  @param dimm  - Processor socket channel dimm number
  @param rank - Processor socket channel dimm rank number

  @retval BOOLEAN - true if the chip should disable DDR4 write leveling

**/
void
DoChipCompatibleRTT_PARKRestore(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank
  )
{
  struct rankDevice   (*rankStruct)[MAX_RANK_DIMM];

  rankStruct  = &host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct;

  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    //
    // Restore RTT_PARK
    //
    WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR5, BANK5);
  } // ddr4
}

#ifdef RC_SIM_FEEDBACK
UINT32 RcSimWLFineResults[MAX_STROBE/2][4] = {
  {0x000000ff, 0xffffffff, 0xffffff00, 0x00000000}, //strobe0
  {0x00000fff, 0xffffffff, 0xfffff000, 0x00000000}, //strobe1
  {0x0000ffff, 0xffffffff, 0xfff00000, 0x00000000}, //strobe2
  {0x000fffff, 0xffffffff, 0xfff00000, 0x00000000}, //strobe3
  {0x0000ffff, 0xffffffff, 0xfffff000, 0x00000000}, //strobe4
  {0x00000fff, 0xffffffff, 0xfffff000, 0x00000000}, //strobe5
  {0x000000ff, 0xffffffff, 0xffffff00, 0x00000000}, //strobe6
  {0x0000000f, 0xffffffff, 0xffffffc0, 0x00000000}, //strobe7
  {0x00000000, 0xffffffff, 0xfffffff3, 0x00000000}, //strobe8
};

#endif // RC_SIM_FEEDBACK

/*++

Routine Description:

  Get's the results from the current test

Arguments:

  host      - Pointer to sysHost
  dimm      - DIMM number
  rank      - rank number of the DIMM

Returns:

  N/A

--*/
void
GetResultsWL (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank,
  UINT16    piDelay
  )
{
  UINT8                   ch;
  UINT8                   strobe;
  UINT8                   dWord;
  UINT8                   step;
  UINT32                  csrReg;
  UINT16                  piDelayStart;
  struct channelNvram     (*channelNvList)[MAX_CH];
  struct dimmNvram        (*dimmNvList)[MAX_DIMM];
  struct TrainingResults  (*trainRes)[MAX_CH][MAX_STROBE];

  trainRes      = &host->var.mem.trainRes;
  channelNvList = &host->nvram.mem.socket[socket].channelList;

  if ((host->nvram.mem.socket[socket].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {
    piDelayStart = 128;
  } else {
    piDelayStart = WR_LVL_PI_START;
  }

  dWord = (UINT8)((piDelay - piDelayStart) / 32);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

    // Continue to the next rank if this one is disabled
    if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList)[dimm].numRanks)) continue;

    for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
      csrReg = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG));
#ifdef  RC_SIM_FEEDBACK
      if (RcSimWLFineResults[strobe%9][dWord] & (BIT31 >> (piDelay % 32))) {
        csrReg = 0x3ffff;
      } else {
        csrReg = 0;
      }
#endif // RC_SIM_FEEDBACK
      if ((*dimmNvList)[dimm].x4Present == 0) {
        if (((csrReg & 0x1FF) >= 6) || (((csrReg >> 9) & 0x1FF) >= 6)) {
          for (step = 0; step < WR_LVL_STEP_SIZE; step++) {
            (*trainRes)[ch][strobe].results[dWord] = (*trainRes)[ch][strobe].results[dWord] | (1 << ((piDelay + step) % 32));
          }
        }
      } else {

        if ((csrReg & 0x1FF) >= 6) {
          for (step = 0; step < WR_LVL_STEP_SIZE; step++) {
            (*trainRes)[ch][strobe].results[dWord] = (*trainRes)[ch][strobe].results[dWord] | (1 << ((piDelay + step) % 32));
          }
        }

        if (((csrReg >> 9) & 0x1FF) >= 6) {
          for (step = 0; step < WR_LVL_STEP_SIZE; step++) {
            (*trainRes)[ch][strobe + 9].results[dWord] = (*trainRes)[ch][strobe + 9].results[dWord] | (1 << ((piDelay + step) % 32));
          }
        }
      }
    } // strobe loop
  } // ch loop
} // GetResultsWL

/**

  this function execute the write leveling Cleanup.
  Center TxDQS-CLK timing

  @param host    - Pointer to sysHost
  @param socket  - Socket number

**/
void
UpdateGlobalOffsets (
  PSYSHOST  host,
  UINT8     ch,
  INT16     TargetOffset,
  UINT32     *CRAddDelay,
  INT16     *GlobalByteOff
  )
{
  // Calculate offsets
  *GlobalByteOff = 0;
  if (TargetOffset > 3) {
    *CRAddDelay = 3;
    *GlobalByteOff = 128 * (TargetOffset - 3);
  } else if (TargetOffset < 0) {
    //
    // Use cwl_adj to back up
    //
    *CRAddDelay = BIT2;

    *GlobalByteOff = 128 * (TargetOffset + 1);
  } else {
    *CRAddDelay = TargetOffset;
  }
} // UpdateGlobalOffsets

UINT8  GetChipWriteLevelingCleanUpOffset(PSYSHOST  host)
{
  UINT8 Offset = 0;

  if (IsBDXEP(host) || (IsDESku(host) && CheckSteppingGreaterThan(host, CPU_BDX, B0_REV_BDX))) {
    Offset = 64;
  } else {
    Offset = 0;
  }

  return Offset;
}

/**

  Chip specific code path. Groveport does not support CPGC_PATWDBCL_MUX0_PBWR_MCDDC_DP_REG (missing from .h only?). Therefore differences in the sequencer setup were moved to hooks

  @param host  - Pointer to the system host (root) structure
  @param socket  - processor socket ID
  @param ch  - Processor socket Channel number
  @param dimm  - Processor socket channel dimm number
  @param rank - Processor socket channel dimm rank number

  @retval - VOID

**/
void
SetupChipPatternSequencer(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank
  )
{
  UINT32 chBitmask;
  CPGC_PATWDBCL_MUXCTL_MCDDC_DP_HSX_BDX_STRUCT  patWDBCLMuxCtl;
  struct dimmNvram        (*dimmNvList)[MAX_DIMM];

  chBitmask = GetChBitmask (host, socket, dimm, rank);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ( !((1 << ch) & chBitmask )) continue;

    dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
    if (rank >= (*dimmNvList)[dimm].numRanks) continue;
    // ECC, Bit Buffer
    patWDBCLMuxCtl.Data = 0;
    patWDBCLMuxCtl.Bits.mux0 = BTBUFFER;
    patWDBCLMuxCtl.Bits.mux1 = BTBUFFER;
    patWDBCLMuxCtl.Bits.mux2 = BTBUFFER;
    patWDBCLMuxCtl.Bits.ecc_datasrc_sel = 1;
    MemWritePciCfgEp (host, socket, ch, CPGC_PATWDBCL_MUXCTL_MCDDC_DP_REG, patWDBCLMuxCtl.Data);

    MemWritePciCfgEp (host, socket, ch, CPGC_PATWDBCL_MUX0_PBWR_MCDDC_DP_REG, 0xAAAAAA);
    MemWritePciCfgEp (host, socket, ch, CPGC_PATWDBCL_MUX1_PBWR_MCDDC_DP_REG, 0xCCCCCC);
    MemWritePciCfgEp (host, socket, ch, CPGC_PATWDBCL_MUX2_PBWR_MCDDC_DP_REG, 0xF0F0F0);
    MemWritePciCfgEp (host, socket, ch, CPGC_PATWDBCL_MUX0_PBRD_MCDDC_DP_REG, 0xAAAAAA);
    MemWritePciCfgEp (host, socket, ch, CPGC_PATWDBCL_MUX1_PBRD_MCDDC_DP_REG, 0xCCCCCC);
    MemWritePciCfgEp (host, socket, ch, CPGC_PATWDBCL_MUX2_PBRD_MCDDC_DP_REG, 0xF0F0F0);
  } // ch loop
}


/**

  Chip specific code path to initialize "GlobalByteOff"

  @param host  - Pointer to the system host (root) structure
  @param socket  - processor socket ID
  @param CRAddDelay  -
  @param GlobalByteOff  -

  @retval - VOID

**/
void
SetChipDQDQSTiming(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  INT16 offsetValue,
  UINT32* CRAddDelay,
  INT16* GlobalByteOff,
  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct
  )
{
  WLChipCleanUpStruct->tCOTHP.Data = MemReadPciCfgEp (host, socket, ch, TCOTHP_MCDDC_CTL_REG);
  *GlobalByteOff = 0;
  UpdateGlobalOffsets(host, ch, offsetValue, CRAddDelay, GlobalByteOff);
}

/**

  Chip specific code path to write Chip Wr Addr delays

  @param host  - Pointer to the system host (root) structure
  @param socket  - processor socket ID
  @param ch  - Processor socket Channel number
  @param CRAddDelay  -
  @param tCOTHP -

  @retval - VOID

**/
void
WriteChipWrADDDelays(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch, INT16
  offsetValue,
  UINT32* CRAddDelay,
  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct
  )
{
  WLChipCleanUpStruct->tCOTHP.Bits.t_cwl_adj = CRAddDelay[ch];

  MemWritePciCfgEp (host, socket, ch, TCOTHP_MCDDC_CTL_REG, WLChipCleanUpStruct->tCOTHP.Data);
}

/**

  Chip specific code path to getChipTimingData

  @param host  - Pointer to the system host (root) structure
  @param socket  - processor socket ID
  @param ch  - Processor socket Channel number

  @retval - VOID

**/
void
GetChipTimingData(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct
  )
{
  WLChipCleanUpStruct->tCOTHP.Data = MemReadPciCfgEp (host, socket, ch, TCOTHP_MCDDC_CTL_REG);
}

/**

  Chip specific code path to updateChipMCDelay

  @param host  - Pointer to the system host (root) structure
  @param socket  - processor socket ID
  @param ch  - Processor socket Channel number
  @param channelNVram  - Pointer to the Channel nvram
  @param tCOTHP -

  @retval - VOID

**/
void
UpdateChipMCDelay(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
struct channelNvram (*channelNvList)[MAX_CH],
  INT16 TargetOffset,
  UINT32* CRAddDelay,
  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct
  )
{
  WLChipCleanUpStruct->tCOTHP.Bits.t_cwl_adj = CRAddDelay[ch];
  MemWritePciCfgEp (host, socket, ch, TCOTHP_MCDDC_CTL_REG, WLChipCleanUpStruct->tCOTHP.Data);
  (*channelNvList)[ch].tcothp = WLChipCleanUpStruct->tCOTHP.Data;
}

void
AssertChipODTOveride(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT32 wrOdtTableData
  )
{
  CPGC_MISCODTCTL_MCDDC_CTL_HSX_BDX_STRUCT cpgcMiscODTCtl;

  cpgcMiscODTCtl.Data = MemReadPciCfgEp (host, socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG);
  cpgcMiscODTCtl.Bits.odt_override  = wrOdtTableData;
  cpgcMiscODTCtl.Bits.odt_on        = wrOdtTableData;
  MemWritePciCfgEp (host, socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG, cpgcMiscODTCtl.Data);
}

void
DeAssertChipODTOveride(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch
  )
{
  CPGC_MISCODTCTL_MCDDC_CTL_HSX_BDX_STRUCT cpgcMiscODTCtl;

  cpgcMiscODTCtl.Data = MemReadPciCfgEp (host, socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG);
  cpgcMiscODTCtl.Bits.odt_override  = 0;
  cpgcMiscODTCtl.Bits.odt_on        = 0;
  MemWritePciCfgEp (host, socket, ch, CPGC_MISCODTCTL_MCDDC_CTL_REG, cpgcMiscODTCtl.Data);
}

void
DoFinalChipSpecificWLCleanUp(
  PSYSHOST host,
  UINT8 socket
  )
{
#ifndef GROVEPORT_FLAG
  UINT8 ch;
  struct channelNvram  (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    MemWritePciCfgEp (host, socket, ch, CPGC_PATWDB_INV_MCDDC_DP_REG, 0);
    MemWritePciCfgEp (host, socket, ch, CPGC_PATWDB_INV0_MCDDC_DP_REG, 0);
    MemWritePciCfgEp (host, socket, ch, CPGC_PATWDB_INV1_MCDDC_DP_REG, 0);
  } // ch loop
#endif
}

//------------------------------------------------------------------------------------

#define PER_GROUP         0
#define COMPOSITE_GROUPS  1

/**

  Exectues command/clock training

  @param host      - Pointer to sysHost

  @retval N/A

**/
UINT32
ChipLateCmdClk(
  PSYSHOST  host
  )
{
  UINT8                       socket;
  //
  // Return if this socket is disabled
  //
  socket = host->var.mem.currentSocket;
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return SUCCESS;

  host->var.mem.checkMappedOutRanks = 0;

  // Do not run late command clock if socket is in 3N Timing
  if (host->nvram.mem.socket[socket].channelList[0].timingMode == TIMING_3N) return SUCCESS;

  if ((~host->memFlows & MF_CMDCLK_EN) || ((host->setup.mem.options & CMD_CLK_TRAINING_EN) == 0)) {
    return SUCCESS;
  }

  if (host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG)) {
    return SUCCESS;
  }

  //
  // Execute the first two steps if early CMD/CLK training was not executed
  //
  if (host->var.mem.earlyCmdClkExecuted == 0) {
    //
    // Aggressive CMD Pattern Margin Test
    //
    AggressiveCMDTest(host, host->var.mem.currentSocket);

    //
    // Per CMD Group Margin & Group Alignment
    //
    PerCMDGroup(host, host->var.mem.currentSocket, PER_GROUP);

  } // if DDR3

  //
  // Final Composite Margin & CK Alignment
  //
  PerCMDGroup(host, host->var.mem.currentSocket, COMPOSITE_GROUPS);

  //
  // Indicate CMD training is done
  //
  host->nvram.mem.socket[host->var.mem.currentSocket].cmdClkTrainingDone = 1;

  // Enable PPDF in next JEDEC init
  IO_Reset (host, host->var.mem.currentSocket);
  JedecInitSequence (host, host->var.mem.currentSocket, CH_BITMASK);

  //
  // Run CMD normalization if enabled and not DDR3 LRDIMM
  //
  if ((host->setup.mem.optionsExt & CMD_NORMAL_EN) &&
      !((host->nvram.mem.socket[host->var.mem.currentSocket].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR3))) {
    NormalizeCCC(host, host->var.mem.currentSocket);
  }

  return SUCCESS;
} // LateCmdClk

//HSX SPECIFIC 15/07/2014

UINT32
ChipAggressiveCMDTest(
  PSYSHOST  host,
  UINT8     socket
  )
/*++

Routine Description:

  Aggressive CMD test to get a rough centering of CMD

Arguments:

  host - Pointer to sysHost

Returns:

  N/A

--*/
{
  UINT8                               ch;
  UINT8                               dimm;
  UINT8                               rank;
  UINT8                               rankEnabled;
  INT16                               le[MAX_CH];
  INT16                               re[MAX_CH];
  UINT32                              status = SUCCESS;
  struct channelNvram                 (*channelNvList)[MAX_CH];
  struct dimmNvram                    (*dimmNvList)[MAX_DIMM];
  struct ddrRank                      (*rankList)[MAX_RANK_DIMM];
  struct RankCh                       (*rankPerCh)[MAX_RANK_CH];
  GSM_CGEDGE                          groupEdgeData[MAX_CH][MAX_RANK_CH][MAX_CMD];
  TCDBP_MCDDC_CTL_HSX_BDX_STRUCT      tcdbp;

  // Skip socket if not enabled
  if (host->nvram.mem.socket[socket].enabled == 0) return status;

  channelNvList = GetChannelNvList(host, socket);

/*
  //
  // Code to test the SetCombinedCmdGroup function
  //
  ch = 0;
        //
        // Load the eye data
        //
        for (signal = 0; signal < NUM_SIGNALS_T0_SWEEP; signal++) {
          signalEdgeData[0][signal].signal = signalsToSweep[sig];
          signalEdgeData[0][signal].le = le0[signal];
          signalEdgeData[0][signal].re = re0[signal];
          signalEdgeData[4][signal].signal = signalsToSweep[signal];
          signalEdgeData[4][signal].le = le1[signal];
          signalEdgeData[4][signal].re = re1[signal];
        } // signal loop
  //
  // Call the combine function
  //
  status = (UINT32)SetCombinedCmdGroup(host, socket, ch, DdrLevel, GSM_COMBINE_EDGES, CMD_SIGNAL_DELAY, NUM_SIGNALS_T0_SWEEP, signalEdgeData);
 */


#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MEM_TRAIN)) {
    getPrintFControl (host);
  }
#endif // SERIAL_DBG_MSG
  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "START_CMD_CLK_SWEEP\n"));

  //
  // Enable normal CMD timing
  //
  SetNormalCMDTiming(host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    dimmNvList = GetDimmNvList(host, socket, ch);
    rankPerCh = &host->nvram.mem.socket[socket].channelList[ch].rankPerCh;

    //
    // Turn on command/address output enable
    //
    tcdbp.Data= MemReadPciCfgEp (host, socket, ch, TCDBP_MCDDC_CTL_REG);
    tcdbp.Bits.cmd_oe_on = 1;
    MemWritePciCfgEp (host, socket, ch, TCDBP_MCDDC_CTL_REG, tcdbp.Data);

  } // ch loop

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MEM_TRAIN)) {
    releasePrintFControl (host);
  }
#endif // SERIAL_DBG_MSG

  IO_Reset (host, socket);

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {

      rankEnabled = 0;

      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
        dimmNvList = GetDimmNvList(host, socket, ch);
        if (rank >= (*dimmNvList)[dimm].numRanks) continue;

        rankEnabled = 1;

        break;
      } // ch loop

      //
      // Continue to the next rank if this one is not populated
      //
      if (rankEnabled == 0) continue;

      //
      // Get CMD margins for current rank and signal
      //
     if ((host->nvram.mem.dimmTypePresent == RDIMM) && (rank > 0)) {
        for (ch = 0; ch < MAX_CH; ch++) {
          if ((*channelNvList)[ch].enabled == 0) continue;
          rankList = GetRankNvList(host, socket, ch, dimm);
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
          dimmNvList = GetDimmNvList(host, socket, ch);
          if (rank >= (*dimmNvList)[dimm].numRanks) continue;
          le[ch] = groupEdgeData[ch][(*rankList)[0].rankIndex][0].le;
          re[ch] = groupEdgeData[ch][(*rankList)[0].rankIndex][0].re;
        } //ch loop
      } else {
        GetCmdMarginsSweepGroup(host, socket, dimm, rank, CmdAll, le, re);
      }

      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        rankList = GetRankNvList(host, socket, ch, dimm);
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
        dimmNvList = GetDimmNvList(host, socket, ch);
        if (rank >= (*dimmNvList)[dimm].numRanks) continue;

        //
        // Convert results from absolute pi values to offset values and save the results
        //
        groupEdgeData[ch][(*rankList)[rank].rankIndex][0].group = CmdGrp0;
        groupEdgeData[ch][(*rankList)[rank].rankIndex][0].le = le[ch];
        groupEdgeData[ch][(*rankList)[rank].rankIndex][0].re = re[ch];
        groupEdgeData[ch][(*rankList)[rank].rankIndex][1].group = CmdGrp1;
        groupEdgeData[ch][(*rankList)[rank].rankIndex][1].le = le[ch];
        groupEdgeData[ch][(*rankList)[rank].rankIndex][1].re = re[ch];
        groupEdgeData[ch][(*rankList)[rank].rankIndex][2].group = CmdGrp2;
        groupEdgeData[ch][(*rankList)[rank].rankIndex][2].le = le[ch];
        groupEdgeData[ch][(*rankList)[rank].rankIndex][2].re = re[ch];

      } // ch loop
    } // rank loop
  } // dimm loop

  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "\n"));

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    //
    // Evaluate results and write them
    //
    status = (UINT32)SetCombinedCmdGroup(host, socket, ch, DdrLevel, GSM_COMBINE_EDGES, CMD_GRP_DELAY, MAX_CMD,
                                         groupEdgeData[ch]);

    //
    // Turn off command/address output enable
    //
    tcdbp.Data = MemReadPciCfgEp (host, socket, ch, TCDBP_MCDDC_CTL_REG);
    tcdbp.Bits.cmd_oe_on = 0;
    MemWritePciCfgEp (host, socket, ch, TCDBP_MCDDC_CTL_REG, tcdbp.Data);

  } // ch loop

  // Execute JEDEC reset/init sequence
  disableAllMsg (host);
  IO_Reset (host, socket);
  JedecInitSequence (host, socket, CH_BITMASK);
  restoreMsg (host);

  return SUCCESS;
} // AggressiveCMDTest

void
ChipEnableCheckParity(
  PSYSHOST host,
  UINT16* mode
  )
{
  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    *mode |= MODE_CHECK_PARITY;
  }
}

void
ChipSimEnableNmlCMDTrng(
  PSYSHOST host,
  UINT8 socket
  )
{
  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    SetNormalCMDTiming(host, socket);
  }
}

void ChipEarlyCmdCKSvHook(
  PSYSHOST host,
  UINT8 socket,
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct
  )
{
  //Boiler plate
}

BOOLEAN
ShouldChipSkipElyCMDTrAsPrtyNotSprtd(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm
  )
{
  UINT8                               parityWorkaround;
  struct channelNvram                 (*channelNvList)[MAX_CH];

  parityWorkaround = 0;
  channelNvList = GetChannelNvList(host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      parityWorkaround = CheckParityWorkaround(host, socket, ch, dimm);

      if(parityWorkaround) break;
    } // dimm loop

    if(parityWorkaround) break;
  } // ch loop

  if ((~host->memFlows & MF_E_CMDCLK_EN) || ((host->setup.mem.optionsExt & EARLY_CMD_CLK_TRAINING_EN) == 0) ||
      (host->nvram.mem.dramType != SPD_TYPE_DDR4) || parityWorkaround || (~host->setup.mem.options & CA_PARITY_EN)) {

    return TRUE;
  }
  return FALSE;
}

void
TrnOnChipCMDADDOpEn(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct
  )
{
    chipErlyCmdCKStruct->tcdbp.Data= MemReadPciCfgEp (host, socket, ch, TCDBP_MCDDC_CTL_REG);
    chipErlyCmdCKStruct->tcdbp.Bits.cmd_oe_on = 1;
    MemWritePciCfgEp (host, socket, ch, TCDBP_MCDDC_CTL_REG, chipErlyCmdCKStruct->tcdbp.Data);
}

void
EnableChipFNV(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct
  )
{
}

void
DisableChipFNV(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct
  )
{
}

void
TrnOffChipCMDADDOpEn(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct
  )
{
  chipErlyCmdCKStruct->tcdbp.Data = MemReadPciCfgEp (host, socket, ch, TCDBP_MCDDC_CTL_REG);
  chipErlyCmdCKStruct->tcdbp.Bits.cmd_oe_on = 0;
  MemWritePciCfgEp (host, socket, ch, TCDBP_MCDDC_CTL_REG, chipErlyCmdCKStruct->tcdbp.Data);
}

void
GetCmdMarginsSweepInit(
  PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct
  )
{
  UINT8 ch;

  chipGetCmdMgnSwpStruct->chEyemask = 0;
  chipGetCmdMgnSwpStruct->chDonemask = 0;

  for (ch = 0; ch < MAX_CH; ch++) {
    chipGetCmdMgnSwpStruct->eyeSize[ch] = 0;
  }
}

void
UpdateCmdMarginMasks(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  INT16 piIndex,
  INT16 offset,
  UINT8 errorResult[MAX_CH][PAR_TEST_RANGE],
  PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct
  )
{
  UINT8 step;

  if (errorResult[ch][piIndex]) {
    chipGetCmdMgnSwpStruct->eyeSize[ch] = 0;
  } else {
    // errorResult[ch][piIndex] = 0;
    chipGetCmdMgnSwpStruct->eyeSize[ch] += offset;
  }

  if (chipGetCmdMgnSwpStruct->eyeSize[ch] > 64) {
     chipGetCmdMgnSwpStruct->chEyemask &= ~(1 << ch);
  }
  if (((chipGetCmdMgnSwpStruct->chEyemask && (1 << ch)) == 0) && (errorResult[ch][piIndex] == 1)) {
     chipGetCmdMgnSwpStruct->chDonemask &= ~(1 << ch);
  }

  //
  // Fill in results for skipped Pi delays
  //
  for (step = 0; step < offset; step++) {
    if ((piIndex + step) >= PAR_TEST_RANGE) continue;

    errorResult[ch][piIndex + step] = errorResult[ch][piIndex];
  }
} // UpdateCmdMarginMasks

UINT8
CheckChDoneMask(
  PSYSHOST host,
  PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct
  )
{
  if(chipGetCmdMgnSwpStruct->chDonemask == 0) {
    return 0;
  } else {
    return 1;
  }
} // CheckChDoneMask

BOOLEAN
ShouldChipClearCMDMGNParityError(
  PSYSHOST host,
  UINT32 status
  )
{
  if ((status) || ((host->nvram.mem.dimmTypePresent == UDIMM) || (host->nvram.mem.dimmTypePresent == SODIMM))) {
    return TRUE;
  }
  return FALSE;
}

void
ChipClearDimmErrRslts(
  UINT8 ch,
  UINT8 errorResult[MAX_CH][CHIP_CMD_CLK_TEST_RANGE],
  UINT32* chBitmask,
  PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct
  )
{
  INT16 piIndex;

  for (piIndex = 0; piIndex < CHIP_CMD_CLK_TEST_RANGE; piIndex++) {
    errorResult[ch][piIndex] = 1;
  } //piIndex loop

  // Indicate rank present and enabled on this ch
  *chBitmask |= 1 << ch;
  chipGetCmdMgnSwpStruct->chEyemask |= 1 << ch;
  chipGetCmdMgnSwpStruct->chDonemask |= 1 << ch;
}

void
GetChipBwSerrErrors(
  UINT32 errMask,
  UINT8 ch,
  UINT8 errorResult[MAX_CH][CHIP_CMD_CLK_TEST_RANGE],
  INT16 piIndex,
  INT16 offset,
  PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct
  )
{
  if (errMask){
    chipGetCmdMgnSwpStruct->eyeSize[ch] = 0;
  } else {
    errorResult[ch][piIndex] = 0;
    chipGetCmdMgnSwpStruct->eyeSize[ch] += offset;
  }

  if (chipGetCmdMgnSwpStruct->eyeSize[ch] > 64) {
    chipGetCmdMgnSwpStruct->chEyemask &= ~(1 << ch);
  }
  if (((chipGetCmdMgnSwpStruct->chEyemask && (1 << ch)) == 0) && (errorResult[ch][piIndex] == 1)) {
    chipGetCmdMgnSwpStruct->chDonemask &= ~(1 << ch);
  }
}

/**
  Collects the results of the previous parity based test

  @param host:         Pointer to sysHost
  @param socket:         Processor socket to check

  @retval status:       Non-zero if any failure was detected
**/
UINT32
ChipCollectParityResults (
  PSYSHOST  host,
  UINT8     socket,
  UINT32    chToCheck,
  UINT8     dimm,
  UINT32    bwSerr[MAX_CH][3]
  )
{
  UINT8                       ch;
  UINT32                      status = 0;
  struct channelNvram         (*channelNvList)[MAX_CH];
  ALERTSIGNAL_MCDDC_DP_STRUCT alertSignal;

  channelNvList = GetChannelNvList(host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    // No need to check this channel if it's bit isn't set
    if ((chToCheck & (1 << ch))) {

      alertSignal.Data = (UINT8)MemReadPciCfgEp (host, socket, ch, ALERTSIGNAL_MCDDC_DP_REG);
      status |= alertSignal.Bits.seen;

      if (alertSignal.Bits.seen) {
        bwSerr[ch][0] = 0xFFFFFFFF;
        bwSerr[ch][1] = 0xFFFFFFFF;
        if (host->nvram.mem.eccEn) {
          bwSerr[ch][2] = 0xFF;
        } else {
          bwSerr[ch][2] = 0;
        }
      }
    }
  } // ch loop

  return  status;
} // ChipCollectParityResults

BOOLEAN
DidChipClearAEPDimmParityErrors(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  PCHIP_CLEAR_PARITY_RESULTS_STRUCT chipClearParityResStruct
  )
{
  return FALSE; //HSX does not support NVMDIMM or does not need to clear parity errors
}

/**
  Clears the results of the previous parity based test

  @param host:         Pointer to sysHost
  @param socket:         Processor socket to check

  @retval status:       Non-zero if any failure was detected
**/
void
ChipClearParityResults (
  PSYSHOST  host,
  UINT8     socket,
  UINT32     chToCheck,
  UINT8     dimm
  )
{
  UINT8                       ch;
  UINT32                      tCrapOrg[MAX_CH];
  struct channelNvram         (*channelNvList)[MAX_CH];
  ALERTSIGNAL_MCDDC_DP_STRUCT alertSignal;
  CHIP_CLEAR_PARITY_RESULTS_STRUCT chipClearParityResStruct;

  channelNvList = GetChannelNvList(host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    tCrapOrg[ch] = 0;

      //
      // Set 3N timing and save original setting
      //
      chipClearParityResStruct.tCrap.Data = tCrapOrg[ch] = MemReadPciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG);
      chipClearParityResStruct.tCrap.Bits.cmd_stretch  = (*channelNvList)[ch].trainTimingMode;
      MemWritePciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG, chipClearParityResStruct.tCrap.Data);
  } // ch loop

  IO_Reset (host, socket);
  JedecInitSequence (host, socket, CH_BITMASK);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

      //
      // Clear parity error status in the memory controller
      //
      if(!DidChipClearAEPDimmParityErrors(host, socket, ch, dimm, &chipClearParityResStruct)){
        alertSignal.Data = (UINT8)MemReadPciCfgEp (host, socket, ch, ALERTSIGNAL_MCDDC_DP_REG);
        alertSignal.Bits.seen = 1;
        MemWritePciCfgEp (host, socket, ch, ALERTSIGNAL_MCDDC_DP_REG, (UINT32)alertSignal.Data);
      }
      //
      // Restore original CMD timing
      //
      MemWritePciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG, tCrapOrg[ch]);
  //  }
  } // ch loop
} // ClearParityResults

BOOLEAN
ShouldChipClearPtyAltWriteMR5(
  PSYSHOST host
  )
{
  if ((host->nvram.mem.dimmTypePresent == UDIMM) || (host->nvram.mem.dimmTypePresent == SODIMM))
    return TRUE;
  else
    return FALSE;
}

void
DoChipSimClearParityResultsAltPath(
  PSYSHOST host,
  UINT8 socket
  )
{
}

BOOLEAN
DoesChipDetectCSorLRRnkDimm(
  UINT8 ch,
  struct channelNvram (*channelNvList)[MAX_CH]
  )
{
  if (((*channelNvList)[ch].encodedCSMode) || ((*channelNvList)[ch].lrRankMultEnabled))
    return TRUE;
  else
    return FALSE;
}

void ChipEnableParity(PSYSHOST host, UINT8 socket, UINT8 ch, PCHIP_PARITY_CHECKING_STRUCT chipEnableParityCheckingStruct)
{
  chipEnableParityCheckingStruct->ddrCRCtlControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG);
  if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
    chipEnableParityCheckingStruct->ddrCRCtlControls.Bits.paritydisable = 1;
  } else {
    chipEnableParityCheckingStruct->ddrCRCtlControls.Bits.paritydisable = 0;
    // Need internalvref enabled
    chipEnableParityCheckingStruct->ddrCRCmdControls1.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXT_REG);
    chipEnableParityCheckingStruct->ddrCRCmdControls1.Bits.internalvrefen = 1;
    MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXT_REG, chipEnableParityCheckingStruct->ddrCRCmdControls1.Data);
  }
  MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG, chipEnableParityCheckingStruct->ddrCRCtlControls.Data);
}

void
ChipSet3NTimingandSaveOrigSetting(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_PARITY_CHECKING_STRUCT chipEnableParityCheckingStruct
  )
{
  struct channelNvram         (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);
  chipEnableParityCheckingStruct->tCrapOrg = MemReadPciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG);
  chipEnableParityCheckingStruct->tCrap.Data = chipEnableParityCheckingStruct->tCrapOrg;
  chipEnableParityCheckingStruct->tCrap.Bits.cmd_stretch  = (*channelNvList)[ch].trainTimingMode;
  MemWritePciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG, chipEnableParityCheckingStruct->tCrap.Data);
}

void
ChipRestoreOrigCMDtiming(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_PARITY_CHECKING_STRUCT chipEnableParityCheckingStruct
  )
{
  MemWritePciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG, chipEnableParityCheckingStruct->tCrapOrg);
}

BOOLEAN
DoesChipSupportParityChecking(
  PSYSHOST host
  )
{
  //
  // Parity is not supported on DDR3 UDIMMs
  //
  if ((host->nvram.mem.dramType == SPD_TYPE_DDR3) && ((host->nvram.mem.dimmTypePresent == UDIMM) ||(host->nvram.mem.dimmTypePresent == SODIMM))) return FALSE;

  return TRUE;  //Parity checking is supported on all other supported DIMM types
}

/**
  Clears the results of the previous parity based test

  @param host:         Pointer to sysHost
  @param socket:       Processor socket to check
  @param rank:         Rank to clear
  @retval status:       Non-zero if any failure was detected
**/
void
ChipClearParityResultsAlt (
  PSYSHOST  host,
  UINT8     socket,
  UINT32     chToCheck,
  UINT8     rank,
  UINT8     dimm
  )
{
  UINT8                       ch;
  UINT32                      tCrapOrg[MAX_CH];
  struct channelNvram         (*channelNvList)[MAX_CH];
  struct dimmNvram            (*dimmNvList)[MAX_DIMM];
  struct rankDevice           (*rankStruct)[MAX_RANK_DIMM];
  ALERTSIGNAL_MCDDC_DP_STRUCT alertSignal;
  CHIP_CLEAR_PARITY_RESULTS_STRUCT chipClearParityResStruct;

  channelNvList = GetChannelNvList(host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    // No need to check this channel if it's bit isn't set
    if ((chToCheck & (1 << ch))) {
      tCrapOrg[ch] = 0;
      //
      // Set 3N timing and save original setting
      //
      chipClearParityResStruct.tCrap.Data = tCrapOrg[ch] = MemReadPciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG);
      chipClearParityResStruct.tCrap.Bits.cmd_stretch  = (*channelNvList)[ch].trainTimingMode;
      MemWritePciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG, chipClearParityResStruct.tCrap.Data);

      //
      // Skip if no DIMM present
      //
      dimmNvList = GetDimmNvList(host, socket, ch);
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      rankStruct  = &host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct;

      //
      // Skip if no rank
      //
      if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

      if (ShouldChipClearPtyAltWriteMR5(host)) {
        WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR5, BANK5);
      } else {
        WriteRC (host, socket, ch, dimm, rank, 6, RDIMM_RC06);
        WriteRC (host, socket, ch, dimm, rank, BIT0, RDIMM_RC0E);
      }

      FixedDelay(host, 1);
    }
  } // ch loop

  IO_Reset (host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    if (chToCheck & (1 << ch)) {

    //
    // Clear parity error status in the memory controller
    //
    if(!DidChipClearAEPDimmParityErrors(host, socket, ch, dimm, &chipClearParityResStruct)){
      alertSignal.Data = (UINT8)MemReadPciCfgEp (host, socket, ch, ALERTSIGNAL_MCDDC_DP_REG);
      alertSignal.Bits.seen = 1;
      MemWritePciCfgEp (host, socket, ch, ALERTSIGNAL_MCDDC_DP_REG, (UINT32)alertSignal.Data);
    }
    //
    // Restore original CMD timing
    //
      MemWritePciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG, tCrapOrg[ch]);
    }
  } // ch loop
} // ClearParityResultsAlt

void
ChipSpecificParityCheckDisable(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_PARITY_CHECKING_STRUCT chipDisableParityCheckingStruct
  )
{
  chipDisableParityCheckingStruct->ddrCRCtlControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG);
//DBG VI
  if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
    chipDisableParityCheckingStruct->ddrCRCtlControls.Bits.paritydisable = 0;
  } else {
    chipDisableParityCheckingStruct->ddrCRCtlControls.Bits.paritydisable = 1;
  }
//END DBG VI
  MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG, chipDisableParityCheckingStruct->ddrCRCtlControls.Data);
}

void
ChipSpecificErrorEnable(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_PARITY_CHECKING_STRUCT chipDisableParityCheckingStruct
  )
{
  VMSE_ERROR_MCDDC_DP_HSX_BDX_STRUCT  vmseError;
  if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX) == 0) {

    chipDisableParityCheckingStruct->ddr4cactl.Data = MemReadPciCfgEp(host, socket, ch & BIT1, DDR4_CA_CTL_MCDDC_DP_REG);

    // Enable error flow for steppings > A0
    if (ch & BIT0){
      chipDisableParityCheckingStruct->ddr4cactl.Bits.erf_en0 = 0;
    }  else {
      chipDisableParityCheckingStruct->ddr4cactl.Bits.erf_en1 = 0;
    }

    MemWritePciCfgEp(host, socket, ch & BIT1, DDR4_CA_CTL_MCDDC_DP_REG, chipDisableParityCheckingStruct->ddr4cactl.Data);
  }
  if (host->var.common.cpuType == CPU_BDX){
    //
    // Clear vmse error (error bits are w1c)
    //
    vmseError.Data = MemReadPciCfgEp (host, socket, ch & 2, VMSE_ERROR_MCDDC_DP_REG);
    MemWritePciCfgEp (host, socket, ch & 2, VMSE_ERROR_MCDDC_DP_REG, vmseError.Data);
  }
}

void
PerformChipSpecificNormalCMDTimingSetup(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_SET_NORMAL_CMD_TIMING_STRUCT chipSetNormalCMDTimingStruct
  )
{
  //Boiler plate
}

BOOLEAN
ShouldChipSetNormalCMDTiming(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_SET_NORMAL_CMD_TIMING_STRUCT chipSetNormalCMDTimingStruct
  )
{
  struct channelNvram         (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  chipSetNormalCMDTimingStruct->tCrap.Data = MemReadPciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG);

  if (chipSetNormalCMDTimingStruct->tCrap.Bits.cmd_stretch != (*channelNvList)[ch].timingMode) {
    chipSetNormalCMDTimingStruct->tCrap.Bits.cmd_stretch  = (*channelNvList)[ch].timingMode;
    MemWritePciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG, chipSetNormalCMDTimingStruct->tCrap.Data);

    PerformChipSpecificNormalCMDTimingSetup(host, socket, ch, chipSetNormalCMDTimingStruct);
    return TRUE;
  }

  return FALSE;
}


#ifdef RC_SIM_FEEDBACK
INT16 CtlClkChRk [][2] = {{ 56, -73}, { 53, -76}, {56, -72}, { 56, -73}, { 56, -73}, { 53, -76}};

VOID
ChipCtlClkFeedBack (
  PSYSHOST  host,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     sign,
  INT16     *ctlMargins
)
{
}
#endif //RC_SIM_FEEDBACK

BOOLEAN
ShouldChipPerformEarlyCtlClk(
  PSYSHOST host
  )
{
  if ((~host->memFlows & MF_E_CTLCLK_EN) || ((host->setup.mem.optionsExt & EARLY_CTL_CLK_TRAINING_EN) == 0)) {
    return FALSE;
  }
  return TRUE;
}

void
PerformChipLRDIMMEarlyCtlClk(
  PSYSHOST host,
  UINT8 socket
  )
{
  UINT8 ch;
  UINT8 dimm;
#ifdef LRDIMM_SUPPORT
  UINT8               controlWordData;
#endif
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];

  channelNvList = GetChannelNvList(host, socket);

#ifdef LRDIMM_SUPPORT

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    if (((*channelNvList)[ch].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR4)) {
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
        //
        // Skip if no DIMM present
        //
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
        //F0BC1x = 00010010b
        // enable MPR override mode in the buffer
        //
        (*dimmNvList)[dimm].lrBuf_BC1x = (*dimmNvList)[dimm].lrBuf_BC1x | LRDIMM_MPR_OVERRIDE_MODE;
        controlWordData = (*dimmNvList)[dimm].lrBuf_BC1x;
        WriteLrbuf(host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC1x);
      } //dimm
    } //lrdimm
  } //ch
#endif
}

void
DisableChipLRDIMMMPROverrideMode(
  PSYSHOST host,
  UINT8 socket
  )
{
  UINT8 ch;
  UINT8 dimm;
#ifdef LRDIMM_SUPPORT
  UINT8               controlWordData;
#endif

  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];

  channelNvList = GetChannelNvList(host, socket);

#ifdef LRDIMM_SUPPORT

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    if (((*channelNvList)[ch].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR4)) {
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
        //
        // Skip if no DIMM present
        //
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
        //
        // disable MPR override mode in the buffer
        //
        (*dimmNvList)[dimm].lrBuf_BC1x = (*dimmNvList)[dimm].lrBuf_BC1x & ~LRDIMM_MPR_OVERRIDE_MODE;
        controlWordData = (*dimmNvList)[dimm].lrBuf_BC1x;
        WriteLrbuf(host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC1x);
      } //dimm
    } //lrdimm
  } //ch
#endif

}

void
EnableChipLRDIMMPreambleTrMode(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm
  )
{
#ifdef LRDIMM_SUPPORT
  UINT8                                 controlWordData;
#endif
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];

  channelNvList = GetChannelNvList(host, socket);
  dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

#ifdef  LRDIMM_SUPPORT
  if ((*channelNvList)[ch].lrDimmPresent) {
    //F0BC1x = 00010000b
    // enable read preamble training mode in the buffer
    (*dimmNvList)[dimm].lrBuf_BC1x = (*dimmNvList)[dimm].lrBuf_BC1x | LRDIMM_RD_PREAMBLE_TRAINING_MODE;
    controlWordData = (*dimmNvList)[dimm].lrBuf_BC1x;
    WriteLrbuf(host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC1x);
  }
#endif
}

void
DisableChipLRDIMMPreambleTrMode(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm
  )
{
#ifdef LRDIMM_SUPPORT
  UINT8  controlWordData;
#endif
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];

  channelNvList = GetChannelNvList(host, socket);
  dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
#ifdef  LRDIMM_SUPPORT
  if (((*channelNvList)[ch].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR4)) {
    //F0BC1x = 00010000b
    // disable read preamble training mode in the buffer
    (*dimmNvList)[dimm].lrBuf_BC1x = (*dimmNvList)[dimm].lrBuf_BC1x & ~LRDIMM_RD_PREAMBLE_TRAINING_MODE;
    controlWordData = (*dimmNvList)[dimm].lrBuf_BC1x;
    WriteLrbuf(host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC1x);
  }
#endif
}

void
StepChipCtlCKIOLatency(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank,
  UINT8 logRank
  )
{
  UINT8                                 ioLatency;
  //
  // Add 1 to IO latency
  //
  ioLatency = GetIOLatency (host, socket, ch, logRank) + 1;
  SetIOLatency (host, socket, ch, logRank, ioLatency);
}

void
RestoreChipCtlCKIOLatency(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank,
  UINT8 logRank
  )
{
  UINT8                                 ioLatency;
  //
  // Restore IO latency
  //
  ioLatency = GetIOLatency (host, socket, ch, logRank) - 1;
  SetIOLatency (host, socket, ch, logRank, ioLatency);
}



UINT8
CheckParityWorkaroundChipSpecific(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT16 dateCode
  )
{
  UINT8 parityWorkaround = 0;
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];

  dimmNvList = GetDimmNvList(host, socket, ch);
  //
  // Parity might not work properly on DDR4 x8 SR RDIMMs older than WW1 2013
  //
  if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && (host->nvram.mem.dimmTypePresent == RDIMM) &&
      ((*dimmNvList)[dimm].numRanks == 1) && ((*dimmNvList)[dimm].x4Present == 0) && (dateCode < 0x1301)) {
    parityWorkaround = 1;
  }

  //
  // Parity does not work properly on Hynix DDR4 UDIMMs older than WW15 2013
  //
  if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && (host->nvram.mem.dimmTypePresent == UDIMM) && (dateCode < 0x1315) &&
      ((*dimmNvList)[dimm].SPDDramMfgId == MFGID_HYNIX)) {
    parityWorkaround = 1;
  }

  return parityWorkaround;
}

void
SetChipGetCtlClkResLowerNibbleStrobeFeedback(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 strobe,
  UINT32* csrReg,
  UINT32* strobeFeedback
  )
{
  //Let compiler optimize if possible.
  UINT32 _strobeFeedback = *strobeFeedback;
  //
  // Common accross HSX, SKX and KNL
  //
  *csrReg = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG));
  //
  // Lower nibble
  //
  if ((*csrReg & 0x1FF) > 0) {
    _strobeFeedback |= 1 << strobe;
    *strobeFeedback = _strobeFeedback;
  }
}

void
SetChipGetCtlClkResUpperNibbleStrobeFeedback(
UINT8 strobe,
UINT32 csrReg,
  UINT32* strobeFeedback
  )
{
  //Let compiler optimize if possible.
  UINT32 _strobeFeedback = *strobeFeedback;
  //
  // Upper nibble
  //
  if (((csrReg >> 9) & 0x1FF) > 0) {
    _strobeFeedback |= 1 << (strobe + 9);
    *strobeFeedback = _strobeFeedback;
  }
}

/**

  Handles any register interaction on GetMargins entry/exit.

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number
  @param entry   - TRUE when entering GetMargins, FALSE when exiting.

**/
UINT32
GetMarginsHook (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  BOOLEAN   entry
)
{
  TCDBP_MCDDC_CTL_HSX_BDX_STRUCT  tcdbp;
  tcdbp.Data = MemReadPciCfgEp (host, socket, ch, TCDBP_MCDDC_CTL_REG);
  tcdbp.Bits.cmd_oe_on = entry;
  MemWritePciCfgEp (host, socket, ch, TCDBP_MCDDC_CTL_REG, tcdbp.Data);
  return SUCCESS;
}

//
//Mem Training project specific
//
void
SetChipSenseAmpOffset(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 rank,
  UINT8 nibble,
  UINT32 OffsetData
  )
{
  //
  //Common accross HSX, SKX and KNL
  //
  MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, rank, nibble, RXOFFSETN0RANK0_0_MCIO_DDRIO_REG), OffsetData);
}

//
//InitMem.c
//
void
SetRankTerminationMask(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch
  )
{
  UINT8   dimm;
  UINT8   i;
  struct ddrRank (*rankList)[MAX_RANK_DIMM];
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];
  MC_TERM_RNK_MSK_MCDDC_CTL_STRUCT rankMask;

  channelNvList = GetChannelNvList(host, socket);
  dimmNvList = GetDimmNvList(host, socket, ch);

  //
  // Look at each DIMM
  //
  rankMask.Data = 0;
  for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

    rankList = GetRankNvList(host, socket, ch, dimm);
    for (i = 0; i < (*dimmNvList)[dimm].numRanks; i++) {
      if ((*rankList)[i].enabled == 0) continue;

      if ((i < 2) && IsTermRank(host, socket, ch, dimm, i)) {
        rankMask.Data |= 1 << ((dimm * MAX_RANK_DIMM) + i);
      }
    } // rank loop

    //
    // Make sure at least one rank is set per DIMM
    //
    if ((rankMask.Data & (0xF << (dimm * MAX_RANK_DIMM))) == 0) {
      rankMask.Data |= 1 << (dimm * MAX_RANK_DIMM);
    }
  } // dimm loop

  //
  // Set a termination rank for each DIMM
  //
  if (rankMask.Data == 0) rankMask.Data = 1;

  //
  // Channel PPDS idle counter
  //
  rankMask.Bits.ch_ppds_idle_timer  = PPDS_IDLE_TIMER;

  //
  // Update the CSR
  //
  MemWritePciCfgEp (host, socket, ch, MC_TERM_RNK_MSK_MCDDC_CTL_REG, rankMask.Data);

  //MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
  //              "TermRnkMsk = 0x%x\n", rankMask.Data));
  //
}

/**

  Checks if DQ swizzling is supported

  @param host    - Pointer to sysHost

**/
UINT8
CheckDqSwizzleSupport (
  PSYSHOST  host,
  UINT8     socket
)
{
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0) || (~host->memFlows & MF_DQ_SWIZ_EN) ||
      (CheckSteppingLessThan(host, CPU_BDX, C0_REV_BDX) && IsDESku(host)) || (host->nvram.mem.eccEn == 0) || (host->var.common.cpuType == CPU_HSX) ||
      (host->nvram.mem.WrCRC == 0) || (host->nvram.mem.dramType != SPD_TYPE_DDR4) || (host->nvram.mem.RASmode & CH_LOCKSTEP)) {
    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "Skipping Swizzle Discovery.\n"));
    return 0;
  }
  return 1;
}

/**

  Checks DQ swizzling and programs registers to account for it

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number
  @param entry   - TRUE when entering GetMargins, FALSE when exiting.

**/
void
CheckAndProgramDqSwizzle (
  PSYSHOST  host,
  UINT8     socket
)
{
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             rank;
  UINT8                             bit;
  UINT8                             i;
  UINT16                            NibbleSwap[MAX_CH] = {0};
//  UINT8                             dqSwzinfo[MAX_RANK_DIMM][MAX_DIMM][MAX_CH][36];
  UINT32                            regDqSwz[5] = {0};
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  SWIZZLE_PATTERN_MCDDC_CTL_STRUCT  SwizzlePatternMcddcCtl;
  SWIZZLE_PATTERN1_MCDDC_CTL_STRUCT SwizzlePattern1McddcCtl;
  SWIZZLE_PATTERN2_MCDDC_CTL_STRUCT SwizzlePattern2McddcCtl;
  SWIZZLE_PATTERN3_MCDDC_CTL_STRUCT SwizzlePattern3McddcCtl;
  SWIZZLE_PATTERN4_MCDDC_CTL_STRUCT SwizzlePattern4McddcCtl;
  SWIZZLE_PATTERN5_MCDDC_CTL_STRUCT SwizzlePattern5McddcCtl;

  //
  // Check if all the dimms have the same swizzling pattern. Else letz exit.
  //
  rank = 0;// The base rank is always Rank0. Hence we need to check Rank0 only
  channelNvList = GetChannelNvList(host, socket);
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    for (dimm = 1; dimm < MAX_DIMM; dimm++) {
      dimmNvList = GetDimmNvList(host, socket, ch);
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      if (MemCompare((*dimmNvList)[dimm].dqSwz, (*dimmNvList)[dimm - 1].dqSwz, sizeof((*dimmNvList)[dimm].dqSwz))) {
        //
        // Write CRC can not be supported because the swizzling across DIMMs on this channel do not match
        //
        host->nvram.mem.WrCRC = 0;
        MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT, "Disabling WrCRC, due to swizzle pattern mismatch.\n"));
        OutputWarning (host, WARN_SWIZZLE_DISCOVERY_TRAINING, WARN_SWIZZLE_PATTERN_MISMATCH, socket, ch, 0xFF, 0xFF);
        return;
      }
    }//dimm
  }// ch

  //
  // Create the appropriate swizzle bitmap
  //
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    dimmNvList = GetDimmNvList(host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
    for (bit = 0; bit < 36; bit += 4) {
      if ((*dimmNvList)[dimm].dqSwz[bit] & 0x4) {
        NibbleSwap[ch] |= 1 << (bit / 4);
      }
    } // bit loop
        for (i = 0; i < MAX_BITS; i++) {
          regDqSwz[i/16] |= (((*dimmNvList)[dimm].dqSwz[i/2] >> ((i % 2)*4)) & 3) << ((i % 16) * 2);
        }
    } // dimm loop
  } // ch loop

  // Map the register.
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    SwizzlePatternMcddcCtl.Data = 0;
    SwizzlePattern1McddcCtl.Data = 0;
    SwizzlePattern2McddcCtl.Data = 0;
    SwizzlePattern3McddcCtl.Data = 0;
    SwizzlePattern4McddcCtl.Data = 0;
    SwizzlePattern5McddcCtl.Data = 0;
    dimmNvList = GetDimmNvList(host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
      for (bit = 0; bit < 36; bit += 4) {
        if ((*dimmNvList)[dimm].dqSwz[bit] & 0x4) {
          NibbleSwap[ch] |= 1 << (bit / 4);
        }
      } // bit loop
    } // dimm loop
    for (i = 0; i < 5; i++) {
      regDqSwz[i] = 0;
    } // i loop

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
      for (bit = 0; bit < MAX_BITS; bit++) {
        regDqSwz[bit / 16] |= (((*dimmNvList)[dimm].dqSwz[bit / 2] >> ((bit % 2)*4)) & 3) << ((bit % 16) * 2);
      } // bit loop
    } // dimm loop
    SwizzlePatternMcddcCtl.Data = NibbleSwap[ch];
    SwizzlePatternMcddcCtl.Bits.pkg_rank_even_odd = 0x2A; // Hardcode the rank map
    SwizzlePatternMcddcCtl.Bits.intra_nibble_bit_swizzle0 = (UINT8)(regDqSwz[0] & 0xFF);

    SwizzlePattern1McddcCtl.Bits.intra_nibble_bit_swizzle1 = (UINT8)((regDqSwz[0] >> 8) & 0xFF);
    SwizzlePattern1McddcCtl.Bits.intra_nibble_bit_swizzle2 = (UINT8)((regDqSwz[0] >> 16) & 0xFF);
    SwizzlePattern1McddcCtl.Bits.intra_nibble_bit_swizzle3 = (UINT8)((regDqSwz[0] >> 24) & 0xFF);
    SwizzlePattern1McddcCtl.Bits.intra_nibble_bit_swizzle4 = (UINT8)(regDqSwz[1] & 0xFF);

    SwizzlePattern2McddcCtl.Bits.intra_nibble_bit_swizzle5 = (UINT8)((regDqSwz[1] >> 8) & 0xFF);
    SwizzlePattern2McddcCtl.Bits.intra_nibble_bit_swizzle6 = (UINT8)((regDqSwz[1] >> 16) & 0xFF);
    SwizzlePattern2McddcCtl.Bits.intra_nibble_bit_swizzle7 = (UINT8)((regDqSwz[1] >> 24) & 0xFF);
    SwizzlePattern2McddcCtl.Bits.intra_nibble_bit_swizzle8 = (UINT8)(regDqSwz[2] & 0xFF);

    SwizzlePattern3McddcCtl.Bits.intra_nibble_bit_swizzle9 = (UINT8)((regDqSwz[2] >> 8) & 0xFF);
    SwizzlePattern3McddcCtl.Bits.intra_nibble_bit_swizzle10 = (UINT8)((regDqSwz[2] >> 16) & 0xFF);
    SwizzlePattern3McddcCtl.Bits.intra_nibble_bit_swizzle11 = (UINT8)((regDqSwz[2] >> 24) & 0xFF);
    SwizzlePattern3McddcCtl.Bits.intra_nibble_bit_swizzle12 = (UINT8)(regDqSwz[3] & 0xFF);

    SwizzlePattern4McddcCtl.Bits.intra_nibble_bit_swizzle13 = (UINT8)((regDqSwz[3] >> 8) & 0xFF);
    SwizzlePattern4McddcCtl.Bits.intra_nibble_bit_swizzle14 = (UINT8)((regDqSwz[3] >> 16) & 0xFF);
    SwizzlePattern4McddcCtl.Bits.intra_nibble_bit_swizzle15 = (UINT8)((regDqSwz[3] >> 24) & 0xFF);
    SwizzlePattern4McddcCtl.Bits.intra_nibble_bit_swizzle16 = (UINT8)(regDqSwz[4] & 0xFF);

    SwizzlePattern5McddcCtl.Bits.intra_nibble_bit_swizzle17 = (UINT8)((regDqSwz[4] >> 8) & 0xFF);

    //MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "SwizzlePatternMcddcCtl: %x\n",  SwizzlePatternMcddcCtl.Data));
    //MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "SwizzlePattern1McddcCtl: %x\n", SwizzlePattern1McddcCtl.Data));
    //MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "SwizzlePattern2McddcCtl: %x\n", SwizzlePattern2McddcCtl.Data));
    //MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "SwizzlePattern3McddcCtl: %x\n", SwizzlePattern3McddcCtl.Data));
    //MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "SwizzlePattern4McddcCtl: %x\n", SwizzlePattern4McddcCtl.Data));
    //MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "SwizzlePattern5McddcCtl: %x\n", SwizzlePattern5McddcCtl.Data));


    MemWritePciCfgEp (host, socket, ch, SWIZZLE_PATTERN_MCDDC_CTL_REG,  SwizzlePatternMcddcCtl.Data);
    MemWritePciCfgEp (host, socket, ch, SWIZZLE_PATTERN1_MCDDC_CTL_REG, SwizzlePattern1McddcCtl.Data);
    MemWritePciCfgEp (host, socket, ch, SWIZZLE_PATTERN2_MCDDC_CTL_REG, SwizzlePattern2McddcCtl.Data);
    MemWritePciCfgEp (host, socket, ch, SWIZZLE_PATTERN3_MCDDC_CTL_REG, SwizzlePattern3McddcCtl.Data);
    MemWritePciCfgEp (host, socket, ch, SWIZZLE_PATTERN4_MCDDC_CTL_REG, SwizzlePattern4McddcCtl.Data);
    MemWritePciCfgEp (host, socket, ch, SWIZZLE_PATTERN5_MCDDC_CTL_REG, SwizzlePattern5McddcCtl.Data);

    SwizzlePatternMcddcCtl.Data = MemReadPciCfgEp (host, socket, ch, SWIZZLE_PATTERN_MCDDC_CTL_REG);
    SwizzlePatternMcddcCtl.Bits.serial_transmit_en = 1;
    MemWritePciCfgEp (host, socket, ch, SWIZZLE_PATTERN_MCDDC_CTL_REG, SwizzlePatternMcddcCtl.Data);

    // Add wait state of 500 cycles.
    FixedDelay(host, 1500);
  } // ch loop

} // CheckAndProgramDqSwizzle

void
WriteRCFnv(
           PSYSHOST host,
           UINT8    socket,
           UINT8    ch,
           UINT8    dimm,
           UINT8    controlWordData,
           UINT8    controlWordAddr
)
{
}

//
// HSX specific functions can maybe move to somewere else (functions not use by KNL/HSX)
//


/**

  Routine Description: Enables/Disables Training Mode for NVMCTLR

  @param

  @retval SUCCESS
**/
UINT32
SetAepTrainingMode (
                 PSYSHOST host,
                 UINT8    socket,
                 UINT8    mode
                 )
{
  return SUCCESS;
}

/**

  Routine Description: Enables/Disables Training Mode for NVMCTLR

  @param

  @retval SUCCESS
**/
UINT32
SetAepTrainingModeSMB (
                 PSYSHOST host,
                 UINT8    socket,
                 UINT8    mode
                 )
{
  return SUCCESS;
}

/**

  This resets the NVMCTLR DDRTIO FIFO Pointers

  @param host    - Pointer to sysHost
  @param socket    - Socket number

  @retval N/A

**/
void
IODdrtReset (
             PSYSHOST host,
             UINT8    socket,
             UINT8    dimm
             )
{

} // IODdrtReset

/**

  Check if CTL loopback mode should be used

  @param host    - Pointer to sysHost

  @retval N/A

**/
BOOLEAN
CheckCTLLoopback (
             PSYSHOST host
             )
{
  return FALSE;
} // CheckCTLLoopback

/**

  This gets the parity error status during CTL loopback training

  @param host    - Pointer to sysHost
  @param socket    - Socket number

  @retval N/A

**/
UINT8
ChipGetParityErrorStatus (
             PSYSHOST host,
             UINT8    socket,
             UINT8    ch,
             UINT8    dimm
             )
{
  UINT8                       err = 0;
  ALERTSIGNAL_MCDDC_DP_STRUCT alertSignal;

  //
  // Clear parity error status in the memory controller
  //
  alertSignal.Data = (UINT8)MemReadPciCfgEp (host, socket, ch, ALERTSIGNAL_MCDDC_DP_REG);
  alertSignal.Bits.seen = 1;
  MemWritePciCfgEp (host, socket, ch, ALERTSIGNAL_MCDDC_DP_REG, (UINT32)alertSignal.Data);

#ifdef YAM_ENV
    C_delay_d_clk(50);
#endif
  //
  // Read Alert Feedback Sticky Bit
  //
  alertSignal.Data = (UINT8)MemReadPciCfgEp (host, socket, ch, ALERTSIGNAL_MCDDC_DP_REG);
  err = alertSignal.Bits.seen;

  return err;
} // ChipGetParityErrorStatus

/**

  This sets the CTL loopback mode in the buffer

  @param host    - Pointer to sysHost
  @param socket    - Socket number

  @retval N/A

**/
void
ChipSetCTLLoopback (
             PSYSHOST host,
             UINT8    socket,
             UINT8    ch,
             UINT8    dimm
             )
{
  UINT32              lrbufData;
  struct dimmNvram            (*dimmNvList)[MAX_DIMM];

  dimmNvList = GetDimmNvList(host, socket, ch);

  /*
  RC0C: Training Control Word
    000 = Normal operating mode
    001 = Clock-to-CA training mode1
    010 = DCS0_n loopback mode1
    011 = DCS1_n loopback mode1
    100 = DCKE0 loopback mode1
    101 = DCKE1 loopback mode1
    110 = DODT0 loopback mode1
    111 = DODT1 loopback mode1
  */
  lrbufData = ((*dimmNvList)[dimm].rcCache[RDIMM_RC0D] << 4) | (*dimmNvList)[dimm].rcCache[RDIMM_RC0C];
  WriteLrbufSmb (host, socket, ch, dimm, 1, 0x08, &lrbufData);
} // ChipSetCTLLoopback

#ifdef SERIAL_DBG_MSG
void
DisplayCCCResults(
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
  UINT8               index;
  UINT8               side;
  UINT8               ioGroup;
  INT16               delay;
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\nSTART_DATA_CMD\n"));

  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "       "));
  for (index = 0; index < 6; index++) {
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "  %2dn ", index));
  } // index loop
  for (index = 0; index < 6; index++) {
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "  %2ds ", index));
  } // index loop

  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\n"));

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    MemDebugPrint((host, SDBG_MEM_TRAIN, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  ""));
    for (side = 0; side < 2; side++) {
      for (ioGroup = 0; ioGroup < MAX_CMD_GROUPALL / 2; ioGroup++) {

        GetSetCmdDelay (host, socket, ch, ioGroup, side, GSM_READ_ONLY, &delay);
        MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "  %3d ", delay));
      } // ioGroup loop
    } // side loop

    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "\n"));
  } // ch loop

  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\nSTART_DATA_CLK\n"));

  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "       "));
  for (index = 0; index < 4; index++) {
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "  %3d ", index));
  } // index loop

  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\n"));

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    MemDebugPrint((host, SDBG_MEM_TRAIN, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  ""));
    for (ioGroup = 0; ioGroup < 4; ioGroup++) {

      GetSetClkDelay (host, socket, ch, ioGroup, GSM_READ_ONLY, &delay);
      MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "  %3d ", delay));
    } // ioGroup loop

    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "\n"));
  } // ch loop

  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\nSTART_DATA_CTL\n"));

  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "       "));
  for (index = 0; index < 10; index++) {
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "  %2dn ", index));
  } // index loop
  for (index = 0; index <= 10; index++) {
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "  %2ds ", index));
  } // index loop

  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\n"));

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    MemDebugPrint((host, SDBG_MEM_TRAIN, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  ""));

    //side 0
    for (ioGroup = 0; ioGroup < MAX_CTL_GROUPALL / 2; ioGroup++) {

      GetSetCtlDelay (host, socket, ch, ioGroup, 0, GSM_READ_ONLY, &delay);
      MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "  %3d ", delay));
    } // ioGroup loop

    //side 1
    for (ioGroup = 0; ioGroup < (MAX_CTL_GROUPALL / 2) + 1; ioGroup++) {

      GetSetCtlDelay (host, socket, ch, ioGroup, 1, GSM_READ_ONLY, &delay);
      MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "  %3d ", delay));
    } // ioGroup loop

    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "\n"));
  } // ch loop
} // DisplayCCCResults


/**
  Displays the Sense Amp register settings

  @param host:         Pointer to sysHost
  @param socket:       Processor socket

  @retval N/A
**/
void
DisplaySenseAmpTrainingResults(
  PSYSHOST      host,
  UINT8         socket
)
{
  UINT8                                     ch;
  UINT8                                     dimm;
  UINT8                                     rank;
  UINT8                                     nibble;
  struct channelNvram                       (*channelNvList)[MAX_CH];
  struct dimmNvram                          (*dimmNvList)[MAX_DIMM];
  struct ddrRank                            (*rankList)[MAX_RANK_DIMM];
  RXOFFSETN0RANK0_0_MCIO_DDRIO_STRUCT       rxOffset;

  channelNvList = GetChannelNvList(host, socket);

  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "\nSTART_SENSE_AMP_TRAINING_OFFSETS\n"));

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    dimmNvList = GetDimmNvList(host, socket, ch);

    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "\nBitSAmp for Channel %d\n            bit:\t  0  1  2  3\n", ch));
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      rankList = GetRankNvList(host, socket, ch, dimm);
      //
      // This value available only for rank 0
      //
      rank = 0;
      if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

      for (nibble = 0; nibble < MAX_STROBE; nibble++) {
        if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;

        rxOffset.Data = MemReadPciCfgEp (host, socket, ch, rxOffsetTable[nibble] + ((*rankList)[rank].logicalRank * 4));

        MemDebugPrint((host, SDBG_MEM_TRAIN, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "Nibble %d:\t", nibble));

        MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      " %2d", rxOffset.Bits.saoffset0));
        MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      " %2d", rxOffset.Bits.saoffset1));
        MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      " %2d", rxOffset.Bits.saoffset2));
        MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      " %2d", rxOffset.Bits.saoffset3));

        MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "\n"));
      } // nibble loop

      break;
    }
  }
  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "\n"));
} //DisplaySenseAmpTrainingResults

/**
  Displays the Swizzling register settings

  @param host:         Pointer to sysHost
  @param socket:       Processor socket

  @retval N/A
**/
void
DisplaySwizzleTrainResults(
  PSYSHOST      host,
  UINT8         socket
)
{
  UINT8                      ch;
  UINT8                      pattern;
  struct channelNvram        (*channelNvList)[MAX_CH];

  SWIZZLE_PATTERN_MCDDC_CTL_STRUCT  SwizzlePatternMcddcCtl;
  SWIZZLE_PATTERN1_MCDDC_CTL_STRUCT SwizzlePattern1McddcCtl;
  SWIZZLE_PATTERN2_MCDDC_CTL_STRUCT SwizzlePattern2McddcCtl;
  SWIZZLE_PATTERN3_MCDDC_CTL_STRUCT SwizzlePattern3McddcCtl;
  SWIZZLE_PATTERN4_MCDDC_CTL_STRUCT SwizzlePattern4McddcCtl;
  SWIZZLE_PATTERN5_MCDDC_CTL_STRUCT SwizzlePattern5McddcCtl;


  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "\nSTART_SWIZZLE_TRAINING_RESULTS\n"));
  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "Pattern"));
  for(pattern = 0 ; pattern < 6 ; pattern++) {
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "  %3d      ", pattern));
  }
  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "\n"));

  channelNvList = GetChannelNvList(host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    MemDebugPrint((host, SDBG_MEM_TRAIN, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, ""));

    SwizzlePatternMcddcCtl.Data = MemReadPciCfgEp (host, socket, ch, SWIZZLE_PATTERN_MCDDC_CTL_REG);
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "  %3x      ",  SwizzlePatternMcddcCtl.Data));

    SwizzlePattern1McddcCtl.Data = MemReadPciCfgEp (host, socket, ch, SWIZZLE_PATTERN1_MCDDC_CTL_REG);
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "  %3x      ",  SwizzlePattern1McddcCtl.Data));

    SwizzlePattern2McddcCtl.Data = MemReadPciCfgEp (host, socket, ch, SWIZZLE_PATTERN2_MCDDC_CTL_REG);
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "  %3x      ",  SwizzlePattern2McddcCtl.Data));

    SwizzlePattern3McddcCtl.Data = MemReadPciCfgEp (host, socket, ch, SWIZZLE_PATTERN3_MCDDC_CTL_REG);
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "  %3x      ",  SwizzlePattern3McddcCtl.Data));

    SwizzlePattern4McddcCtl.Data = MemReadPciCfgEp (host, socket, ch, SWIZZLE_PATTERN4_MCDDC_CTL_REG);
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "  %3x      ",  SwizzlePattern4McddcCtl.Data));

    SwizzlePattern5McddcCtl.Data = MemReadPciCfgEp (host, socket, ch, SWIZZLE_PATTERN5_MCDDC_CTL_REG);
    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "  %3x      ",  SwizzlePattern5McddcCtl.Data));

    MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "\n"));
  }
  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "\n"));

} //DisplaySwizzleTrainResults

void
DisplayCompRegResults(
  PSYSHOST  host,
  UINT8     socket
)
/*++
  Displays the comp register contents

  @param host:         Pointer to sysHost
  @param socket:       Processor socket

  @retval N/A

--*/
{
  UINT8                                             ch;
  UINT8                                             strobe;
  struct channelNvram                               (*channelNvList)[MAX_CH];
  COMPDATA0_0_MCIO_DDRIO_HSX_STRUCT                 compData0Hsx;
  COMPDATA0_0_MCIO_DDRIO_BDX_STRUCT                 compData0Bdx;
  COMPDATA1_0_MCIO_DDRIO_HSX_STRUCT                 compData1Hsx;
  COMPDATA1_0_MCIO_DDRIO_BDX_STRUCT                 compData1Bdx;
  DDRCRCTLCOMP_CKE_MCIO_DDRIOEXT_STRUCT             ddrCRCtlCompCke;
  DDRCRCTLCOMP_CTL_MCIO_DDRIOEXT_STRUCT             ddrCRCtlCompCtl;
  DDRCRCMDCOMP_CMDN_MCIO_DDRIOEXT_STRUCT            ddrCRCmdCompCmdn;
  DDRCRCMDCOMP_CMDS_MCIO_DDRIOEXT_STRUCT            ddrCRCmdCompCmds;
  DDRCRCLKCOMP_CLK_MCIO_DDRIOEXT_STRUCT             ddrCRClkComp;

  channelNvList = &host->nvram.mem.socket[socket].channelList;
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\n\nSTART_COMP_REG_DUMP\n"));


  //DataComp dump
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\nCh  COMP"));
  for (strobe = 0; strobe < MAX_STROBE/2; strobe++) {
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "  %3u ", strobe));
  } // strobe loop

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "\n%u  DrvUp",  ch));
    for (strobe = 0; strobe < MAX_STROBE/2; strobe++) {
      if (host->var.common.cpuType == CPU_HSX) {
        compData0Hsx.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, COMPDATA0_0_MCIO_DDRIO_REG));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "  %3u ", compData0Hsx.Bits.rcompdrvup));
      } else if (host->var.common.cpuType == CPU_BDX) {
        compData0Bdx.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, COMPDATA0_0_MCIO_DDRIO_REG));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "  %3u ", compData0Bdx.Bits.rcompdrvup));
      }
    } // strobe loop

    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "\n%u  DrvDn",  ch));
    for (strobe = 0; strobe < MAX_STROBE/2; strobe++) {
      if (host->var.common.cpuType == CPU_HSX) {
        compData0Hsx.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, COMPDATA0_0_MCIO_DDRIO_REG));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "%  3u ", compData0Hsx.Bits.rcompdrvdown));
      } else if (host->var.common.cpuType == CPU_BDX) {
        compData0Bdx.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, COMPDATA0_0_MCIO_DDRIO_REG));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "  %3u ", compData0Bdx.Bits.rcompdrvdown));
      }
    } // strobe loop
  } //ch loop

  //ODT dump
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\n\nCh  COMP"));
  for (strobe = 0; strobe < MAX_STROBE/2; strobe++) {
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "  %3u ", strobe));
  } // strobe loop

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "\n%u  ODTUp",  ch));
    for (strobe = 0; strobe < MAX_STROBE/2; strobe++) {
      if (host->var.common.cpuType == CPU_HSX) {
        compData1Hsx.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, COMPDATA1_0_MCIO_DDRIO_REG));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "  %3u ", compData1Hsx.Bits.rcompodtup));
      } else if (host->var.common.cpuType == CPU_BDX) {
        compData1Bdx.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, COMPDATA1_0_MCIO_DDRIO_REG));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "  %3u ", compData1Bdx.Bits.rcompodtup));
      }
    } // strobe loop

    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "\n%u  ODTDn",  ch));
    for (strobe = 0; strobe < MAX_STROBE/2; strobe++) {
      if (host->var.common.cpuType == CPU_HSX) {
        compData1Hsx.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, COMPDATA1_0_MCIO_DDRIO_REG));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "  %3u ", compData1Hsx.Bits.rcompodtdown));
      } else if (host->var.common.cpuType == CPU_BDX) {
        compData1Bdx.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, COMPDATA1_0_MCIO_DDRIO_REG));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "  %3u ", compData1Bdx.Bits.rcompodtdown));
      }
    } // strobe loop
  }  //ch loop

  //Scomp
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\n\nCh  COMP"));
  for (strobe = 0; strobe < MAX_STROBE/2; strobe++) {
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "  %3u ", strobe));
  } // strobe loop


  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "\n%u  Scomp",  ch));
    for (strobe = 0; strobe < MAX_STROBE/2; strobe++) {
      if (host->var.common.cpuType == CPU_HSX) {
        compData0Hsx.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, COMPDATA0_0_MCIO_DDRIO_REG));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "  %3u ", compData0Hsx.Bits.slewratecomp));
      } else if (host->var.common.cpuType == CPU_BDX) {
        compData0Bdx.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, COMPDATA0_0_MCIO_DDRIO_REG));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "  %3u ", compData0Bdx.Bits.slewratecomp));
      }
    } // strobe loop
  } // ch loop


  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\n\nCh\tCOMP\t CLK\t CMD_n\t CMD_s\t CTL_cke\t CTL_ctl\n"));
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    ddrCRClkComp.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCLKCOMP_CLK_MCIO_DDRIOEXT_REG);
    ddrCRCmdCompCmdn.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCOMP_CMDN_MCIO_DDRIOEXT_REG);
    ddrCRCmdCompCmds.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCOMP_CMDS_MCIO_DDRIOEXT_REG);
    ddrCRCtlCompCke.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLCOMP_CKE_MCIO_DDRIOEXT_REG);
    ddrCRCtlCompCtl.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLCOMP_CTL_MCIO_DDRIOEXT_REG);

    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "%u  DrvUp\t %3u     %3u     %3u       %3u       %3u\n",  ch,
                  ddrCRClkComp.Bits.rcompdrvup, ddrCRCmdCompCmdn.Bits.rcompdrvup, ddrCRCmdCompCmds.Bits.rcompdrvup,
                  ddrCRCtlCompCke.Bits.rcompdrvup, ddrCRCtlCompCtl.Bits.rcompdrvup));

    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "%u  DrvDn\t %3u     %3u     %3u       %3u       %3u\n",  ch,
                  ddrCRClkComp.Bits.rcompdrvdown, ddrCRCmdCompCmdn.Bits.rcompdrvdown, ddrCRCmdCompCmds.Bits.rcompdrvdown,
                  ddrCRCtlCompCke.Bits.rcompdrvdown, ddrCRCtlCompCtl.Bits.rcompdrvdown));
  } // ch loop

  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\n\nCh\tCOMP\t CLK\t CMD_n\t CMD_s\t CTL_cke\t CTL_ctl\n"));
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    ddrCRClkComp.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCLKCOMP_CLK_MCIO_DDRIOEXT_REG);
    ddrCRCmdCompCmdn.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCOMP_CMDN_MCIO_DDRIOEXT_REG);
    ddrCRCmdCompCmds.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCOMP_CMDS_MCIO_DDRIOEXT_REG);
    ddrCRCtlCompCke.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLCOMP_CKE_MCIO_DDRIOEXT_REG);
    ddrCRCtlCompCtl.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLCOMP_CTL_MCIO_DDRIOEXT_REG);

    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "%u  Scomp\t %3u     %3u     %3u       %3u       %3u\n",  ch,
                  ddrCRClkComp.Bits.scomp, ddrCRCmdCompCmdn.Bits.scomp, ddrCRCmdCompCmds.Bits.scomp,
                  ddrCRCtlCompCke.Bits.scomp, ddrCRCtlCompCtl.Bits.scomp));
  } // ch loop

  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "\n\n"));
} // DisplayCompRegResults

/**
  Displays training results specific to chip/project

  @param host:         Pointer to sysHost
  @param socket:       Processor socket

  @retval N/A
**/
void
DisplayResultsChip(
    PSYSHOST  host,
    UINT8     socket
)
{
  DisplayCMDVrefResults(host, socket);
  DisplaySenseAmpTrainingResults(host, socket);

  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "\nSTART_POWER_TRAINING_DUMP\n"));
  DisplayPowerTraingResults(host, socket, TxImode);
  DisplayPowerTraingResults(host, socket, RxEq);
  DisplayPowerTraingResults(host, socket, DramDrvStr);
  DisplayPowerTraingResults(host, socket, TxRon);
  DisplayPowerTraingResults(host, socket, WrOdt);
  DisplayPowerTraingResults(host, socket, RxOdt);
  DisplayPowerTraingResults(host, socket, TxEq);
  MemDebugPrint((host, SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "\n"));
  if (host->var.common.cpuType == CPU_BDX) {
    DisplaySwizzleTrainResults(host, socket);
  }
  DisplayCompRegResults(host, socket);
  MemDebugPrint((host, SDBG_MEM_TRAIN, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "STOP_TRAINING_REGISTER_DUMP\n"));
} // DisplayResultsChip
#endif // SERIAL_DBG_MSG
void
CheckTwrMargins(
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     tWR,
  UINT8     *DisCh
)
{
  // Check if value exceeds the bits on HSX
  if ((tWR > 0x1F) && (host->var.common.cpuType == CPU_HSX)) {
    // Disable channel
    *DisCh = 1;
    DisableChannel(host, socket, ch);
    MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "twr value exceeds the available field width. Disabling Channel.\n"));
    OutputWarning (host, WARN_CH_DISABLED, WARN_TWR_LIMIT_REACHED, socket, ch, 0xFF, 0xFF);
    if (host->nvram.mem.RASmode == CH_LOCKSTEP) {
      switch (ch) {
        case 0:
        case 2:
          DisableChannel(host, socket, ch + 1);
          MemDebugPrint((host, SDBG_MAX, socket, ch + 1, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "twr value exceeds the available field width on lockstep ch. Disabling Channel.\n"));
          OutputWarning (host, WARN_CH_DISABLED, WARN_TWR_LIMIT_ON_LOCKSTEP_CH, socket, ch + 1, 0xFF, 0xFF);
          break;
        case 1:
        case 3:
          DisableChannel(host, socket, ch - 1);
          MemDebugPrint((host, SDBG_MAX, socket, ch - 1, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "twr value exceeds the available field width on lockstep ch. Disabling Channel.\n"));
          OutputWarning (host, WARN_CH_DISABLED, WARN_TWR_LIMIT_ON_LOCKSTEP_CH, socket, ch - 1, 0xFF, 0xFF);
          break;
      }
    }
  }
} // CheckTwrMargins

void
NormalizeLockstepTiming(
  PSYSHOST  host,
  UINT8     socket
)
{
  UINT8                          ch;
  TCRAP_MCDDC_CTL_HSX_BDX_STRUCT tcrap;
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  for (ch = 0; ch < MAX_CH; ch ++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    if ((host->nvram.mem.RASmode == CH_LOCKSTEP) && ((ch == 0) || (ch == 2))) {
      FastestCommon   (&((*channelNvList)[ch].common.nWR), &((*channelNvList)[ch + 1].common.nWR), 0);
    }
    tcrap.Data = MemReadPciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG);
    tcrap.Bits.t_wr = (*channelNvList)[ch].common.nWR & (0x1F);
    if (host->var.common.cpuType == CPU_BDX) {
      tcrap.Bits.t_wr_upper = (((*channelNvList)[ch].common.nWR & 0x20) >> 5);
    }
    MemWritePciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG, tcrap.Data);
  }
} // NormalizeLockstepTiming

UINT8
GetVocVrefFactor (
  PSYSHOST  host
)
{
  if (IsDESku(host) && CheckSteppingLessThan(host, CPU_BDX, C0_REV_BDX)) {
    return 5; // Conversion factor of 2.5 to 1
  } else {
    return 2; // conversion factor of 1 to 1
  }
} // GetVocVrefFactor

UINT32
SetMaxDelta (
  PSYSHOST  host,
  UINT32    maxAllowedDelta
)
{
  if (IsDESku(host) && CheckSteppingLessThan(host, CPU_BDX, C0_REV_BDX)) {
    maxAllowedDelta = (maxAllowedDelta * 5) / 2;
  }
  return maxAllowedDelta;
} // SetMaxDelta

/**

  This gets the Tx Eq settings list

  @param host    - Pointer to sysHost
  @param socket    - Socket number

  @retval numTxEqSettings - number of Tx EQ settings

**/
UINT8
GetTxEqSettingsList (
             PSYSHOST host,
             UINT8    socket,
             INT16    settingList[MAX_ODT_SETTINGS][MAX_PARAMS]
             )
{
  UINT8 loop;
  INT16 txEqSweep[MAX_TX_EQ] = {0,9,10,11,14,13,25,26,27,30,1,2,3,6,5};
  UINT8 numTxEqSettings = MAX_TX_EQ;

  if (host->var.common.cpuType == CPU_HSX) {
    numTxEqSettings = MAX_TX_EQ; // Number of points characterised.
    for (loop = 0; loop < numTxEqSettings; loop++) {
      settingList[loop][0] = txEqSweep[loop];
    }
  } else { //BDX
    if (CheckSteppingLessThan( host, CPU_BDX, C0_REV_BDX) && IsDESku(host)) {
      for (loop = 0; loop < MAX_TX_EQ; loop++) {
        settingList[loop][0] = loop;
      }
    } else if (IsBDXEP(host) || (IsDESku(host) && CheckSteppingGreaterThan(host, CPU_BDX, B0_REV_BDX))) {
      numTxEqSettings = MAX_TX_EQ_BDX;
      for (loop = 0; loop < 32; loop += 2) {
        settingList[loop/2][0] = loop;
      }
    }
  }
  return numTxEqSettings;

} // GetTxEqSettingsList

/**

  This gets  CTLE settings list

  @param host    - Pointer to sysHost
  @param socket  - Socket number

  @retval - number of CTLE settings

**/
UINT8
GetCtleSettingsList (
             PSYSHOST host,
             UINT8    socket,
             INT16    settingList[MAX_ODT_SETTINGS][MAX_PARAMS]
             )
{
  INT16 ctleSettingList[MAX_SETTINGS][MAX_PARAMS] = {  {0,0,0},
                                                       {2,1,3},
                                                       {2,2,3},
                                                       {2,3,3},
                                                       {3,1,3},
                                                       {3,2,3},
                                                       {3,3,3},
                                                       {4,1,3},
                                                       {4,2,3},
                                                       {4,3,3}
                                                     };
  MemCopy ((UINT8 *)settingList, (UINT8 *)ctleSettingList, sizeof(ctleSettingList));

  return MAX_CTLE;

} // GetCtleSettingsList

/**

  This gets MC_RON settings list

  @param host    - Pointer to sysHost
  @param socket  - Socket number

  @retval - number of MC_RON settings

**/
UINT8
GetMcRonSettingsList (
             PSYSHOST host,
             UINT8    socket,
             INT16    settingList[MAX_ODT_SETTINGS][MAX_PARAMS]
             )
{
  //
  // Override default settingList with chip specific values if required
  //
  INT16 mcRonSettingList[MAX_SETTINGS][MAX_PARAMS] = {{TX_RON_24},{TX_RON_18},{TX_RON_14}};
  MemCopy ((UINT8 *)settingList, (UINT8 *)mcRonSettingList, sizeof(mcRonSettingList));
  return MAX_MCRON;
} // GetMcRonSettingsList

#if !defined(DE_SKU) || defined(MEM_SV_HOOKS)
/*++

  Enables Write CRC

  @param host    - Pointer to sysHost
  @param socket    - Socket number

  @retval N/A

--*/
void
EnableWrCRC (
  PSYSHOST host,
  UINT8    socket
)
{

  UINT8                                  ch;
  UINT8                                  strobe;
  UINT8                                  rank;
  UINT8                                  dimm;
  IOLATENCY1_MCDDC_DP_BDX_STRUCT         ioLatency_bdx;
  VMSE_ERROR_MCDDC_DP_HSX_BDX_STRUCT     vmseerrordata;
  DATACONTROL4_0_MCIO_DDRIO_BDX_STRUCT   dataControl4;
  struct channelNvram                    (*channelNvList)[MAX_CH];
  struct dimmNvram                       (*dimmNvList)[MAX_DIMM];
  struct ddrRank                         (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                      (*rankStruct)[MAX_RANK_DIMM];
  dataControl4.Data = 0;

  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "Enabling WrCRC.\n"));

  //
  // Account for DQ swizzling if necessary
  //
  CheckAndProgramDqSwizzle(host, socket);

  channelNvList = GetChannelNvList(host, socket);
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    if (!(host->var.common.bootMode == S3Resume || host->var.mem.subBootMode == WarmBootFast)) {
      dimmNvList = GetDimmNvList(host, socket, ch);
      for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {
        if ( (*dimmNvList)[dimm].dimmPresent == 0 ) continue;
        rankList = GetRankNvList(host, socket, ch, dimm);
        rankStruct  = &host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct;
        for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
          if ((*rankList)[rank].enabled == 0) continue;
          (*rankStruct)[rank].MR2 |= BIT12;// WrCRC Bit
          WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR2, BANK2);
        }
      }
    }//!S3 resume or !WarmBootFast
    vmseerrordata.Data = MemReadPciCfgEp (host, socket, (ch & 0x02), VMSE_ERROR_MCDDC_DP_REG);
    // Hardcoded the value to 0x31. The basis of tabulation is tcwl + 10 BL + const
    vmseerrordata.Bits.vmse_err_latency = 0x31;
    MemWritePciCfgEp (host, socket, (ch & 0x02), VMSE_ERROR_MCDDC_DP_REG, vmseerrordata.Data);

    ioLatency_bdx.Data = MemReadPciCfgEp (host, socket, ch, IOLATENCY1_MCDDC_DP_REG);
    ioLatency_bdx.Bits.cr_wrcrc_enable = 1;
    MemWritePciCfgEp (host, socket, ch, IOLATENCY1_MCDDC_DP_REG, ioLatency_bdx.Data);

    for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
      dataControl4.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL4_0_MCIO_DDRIO_REG));
      dataControl4.Bits.writecrcenable = 1;
      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL4_0_MCIO_DDRIO_REG), dataControl4.Data);
      (*channelNvList)[ch].dataControl4[strobe] = dataControl4.Data;
    } // strobe loop
  } // ch loop

  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "Enabled WrCrc. \n"));

  return ;
} // EnableWrCRC
#endif

/**

  This programs nodata_trn_en

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param value   - Value to program nodata_trn_en

  @retval N/A

**/
void
ProgramNoDataTrnEn (
             PSYSHOST host,
             UINT8    socket,
             UINT8    value
             )
{
} // ProgramNoDataTrnEn

/**

  This programs CaVrefSelect

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @retval N/A

**/
UINT8
CaVrefSelect (
               PSYSHOST host
)
{
  UINT8 controlWordData;

  if (host->var.common.cpuType == CPU_BDX) {
    controlWordData = 8;
  } else {
    controlWordData = 0;
  }

  return controlWordData;
} //CaVrefSelect

UINT8
SkipReceiveEnable (
  PSYSHOST host
  )
/*++

  RCVEN is normally run twice - once in Early CTL/CLK and again after Early CMD/CLK.
  Return success if running RCVEN the second time.

  @param host            - Pointer to sysHost

  @retval   0 - RCVEN has not been run
            1 - RCVEN has been run once already

--*/
{
  return FAILURE;
} //SkipReceiveEnable

VOID
ReceiveEnableInitChip (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch
  )
/*++

  Initialize channel for Recieve Enable training.
  This initialization is needed each time training is performed on a new rank.

  @param host            - Pointer to sysHost
  @param socket          - Socket number

  @retval  N/A

--*/
{
  UINT8                             dimm;
  UINT8                             rank;
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];

  dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

    for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
      if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

      //
      // Set initial Roundtrip Latency values
      //
      SetInitRoundTrip(host, socket, ch, dimm, rank);
    } // rank loop
  } // dimm loop
  SetIoLatComp( host, socket, ch);
} // ReceiveEnableInitChip

VOID
RankRoundTrip (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank
  )
/*++

  Initialize channel for Recieve Enable training.
  This initialization is needed each time training is performed on a new rank.

  @param host            - Pointer to sysHost
  @param socket          - Socket number
  @param ch              - Channel number (0-based)
  @param dimm            - DIMM number
  @param rank            - Rank number

  @retval  N/A

--*/
{
} // RankRoundTrip

void
EarlyCmdSignalTest(
  PSYSHOST  host,
  UINT8     socket,
  UINT32    chBitmask
  )
/*++

Routine Description:

  Start and stop Early CMD/CLK tests

Arguments:

  host        - Pointer to sysHost
  socket      - Socket number
  chBitmask   - Channels to execute test on

Returns:

  N/A

--*/
{

 ExecuteCmdSignalTest (host, socket, chBitmask);

} // EarlyCmdSignalTest

UINT32
EarlyCmdSignalStop(
  PSYSHOST  host,
  UINT8     socket,
  UINT32    chBitmask,
  UINT8     dimm,
  UINT32    bwSerr[MAX_CH][3]
  )
/*++

Routine Description:

  Stop Early CMD/CLK tests

Arguments:

  host        - Pointer to sysHost
  socket      - Socket number
  chBitmask   - Channels to execute test on

Returns:

  N/A

--*/
{
  return 0;
} // EarlyCmdSignalStop
/**

  Checks if Rx Vref tuning is required

  @param host    - Pointer to sysHost

  @retval BOOLEAN - true if the chip should enable RxVref tuning

 **/
BOOLEAN
ShouldChipEnableRxVrefTuning (
    PSYSHOST host
)
{
  return (RX_VREF_TUNING == 1) && ((host->var.common.cpuType == CPU_HSX) || (IsDESku(host) && CheckSteppingLessThan(host, CPU_BDX, C0_REV_BDX)));
}


VOID
RxVrefTuning (
              PSYSHOST  host,
              UINT8     socket,
              GSM_GT    group
)
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               nibble;
  INT16               vocVrefConversionFactor;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  RXOFFSETN0RANK0_0_MCIO_DDRIO_STRUCT       rxOffset;
  INT16               vref;
  INT16               buffer;
  INT16               sampOffset;
  INT16               delta;
  UINT32              maxAllowedDelta;

  channelNvList = GetChannelNvList(host, socket);

  vocVrefConversionFactor = GetVocVrefFactor ( host);

  if (ShouldChipEnableRxVrefTuning(host)) {
    if ((group == RxVref) && (host->nvram.mem.dimmTypePresent == RDIMM) && (CheckDimmType(host) == SUCCESS)) {
      MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "RX Vref/SA offset Tuning\n"));
      buffer = 5;
      delta = 0;
      vref = 0;
      maxAllowedDelta = 63;
      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

        //For all nibbles
        for (nibble = 0; nibble < MAX_STROBE; nibble++) {
          if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;

          //If RxVref is less than 0x40 + buffer_value
          GetSetDataGroup (host, socket, ch, 0, 0, nibble, ALL_BITS, DdrLevel, RxVref, GSM_READ_ONLY, &vref);
          MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, nibble, NO_BIT,
                      "Vref:%d < (0x40 + buffer:%d)\n", vref, buffer));
          if (vref < (0x40 + buffer)){
            //Max_allowed_delta = min (sense amp offset for all rank)
            maxAllowedDelta = 63;
            for (dimm = 0; dimm < MAX_DIMM; dimm++) {
              if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
              rankList = GetRankNvList(host, socket, ch, dimm);
              //Adjust sense amp offset (for all rank settings) as follows:
              for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
                if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

                rxOffset.Data = (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][nibble];
                if (rxOffset.Bits.saoffset0 < maxAllowedDelta) maxAllowedDelta = rxOffset.Bits.saoffset0;
                if (rxOffset.Bits.saoffset1 < maxAllowedDelta) maxAllowedDelta = rxOffset.Bits.saoffset1;
                if (rxOffset.Bits.saoffset2 < maxAllowedDelta) maxAllowedDelta = rxOffset.Bits.saoffset2;
                if (rxOffset.Bits.saoffset3 < maxAllowedDelta) maxAllowedDelta = rxOffset.Bits.saoffset3;
              } // rank
            } // dimm

            maxAllowedDelta =SetMaxDelta (host, maxAllowedDelta);

            //Delta = Min[(0x40 + buffer_value) – RxVref ; Max_allowed_delta]
            delta = (0x40 + buffer) - vref;
            if ((INT16)maxAllowedDelta < delta) delta = (INT16)maxAllowedDelta;

            //If RxVvref is less than (0x40 - maxAllowedDelta - buffer)
            //then Delta = 0 *** Note that we are SUBTRACTING buffer_value
            if (vref < (INT16)(0x40 - maxAllowedDelta - buffer)) delta = 0;
            MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, nibble, NO_BIT,
                      "delta:%d, maxAllowedDelay:%d\n", delta, maxAllowedDelta));

            //Adjust sense amp offset (for all rank settings) as follows:
            for (dimm = 0; dimm < MAX_DIMM; dimm++) {
              if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
              rankList = GetRankNvList(host, socket, ch, dimm);
              for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
                if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

                //New SA offset = Old SA offset – Delta
                rxOffset.Data = (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][nibble];

                sampOffset = (INT16)rxOffset.Bits.saoffset0 - (delta * 2) / vocVrefConversionFactor;
                MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, nibble, NO_BIT,
                          "New SA Offset 0:%d = Old SA offset 0:%d – delta:%d\n", sampOffset, rxOffset.Bits.saoffset0, (delta * 2) / vocVrefConversionFactor));
                rxOffset.Bits.saoffset0 = sampOffset;

                sampOffset = (INT16)rxOffset.Bits.saoffset1 - (delta * 2) / vocVrefConversionFactor;
                MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, nibble, NO_BIT,
                          "New SA Offset 1:%d = Old SA offset 1:%d – delta:%d\n", sampOffset, rxOffset.Bits.saoffset1, (delta * 2) / vocVrefConversionFactor));
                rxOffset.Bits.saoffset1 = sampOffset;

                sampOffset = (INT16)rxOffset.Bits.saoffset2 - (delta * 2) / vocVrefConversionFactor;
                MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, nibble, NO_BIT,
                          "New SA Offset 2:%d = Old SA offset 2:%d – delta:%d\n", sampOffset, rxOffset.Bits.saoffset2, (delta * 2) / vocVrefConversionFactor));
                rxOffset.Bits.saoffset2 = sampOffset;

                sampOffset = (INT16)rxOffset.Bits.saoffset3 - (delta * 2) / vocVrefConversionFactor;
                MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, nibble, NO_BIT,
                          "New SA Offset 3:%d = Old SA offset 3:%d – delta:%d\n", sampOffset, rxOffset.Bits.saoffset3, (delta * 2) / vocVrefConversionFactor));
                rxOffset.Bits.saoffset3 = sampOffset;

                //
                //Chip specific write to sense amp offset control
                //
                SetChipSenseAmpOffset(host, socket, ch, rank, nibble, rxOffset.Data);
                (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][nibble] = rxOffset.Data;
              } // rank loop
            } // dimm loop
            //Adjust RxVref as follows:
            //New RxVref = Old RxVref + Delta
            MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, nibble, NO_BIT,
                          "New RxVref:%d = Old RxVref3:%d + delta:%d\n", (vref + delta), vref, delta));
            vref = vref + delta;
            GetSetDataGroup (host, socket, ch, 0, 0, nibble, ALL_BITS, DdrLevel, RxVref, GSM_UPDATE_CACHE, &vref);
          } // vref loop
        } // nibble loop
      } // ch loop
      MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "RX Vref/SA offset Tuning End\n"));
    } // RxVref
  } // RX_VREF_TUNING
} //RxVrefTuning


/**

  This initializes the imode setting list

  @param host    - Pointer to sysHost
  @param socket    - Socket number
  @param settingLIst - pointer to list of settings


  @retval numImodeSettings - number of Imode settings

**/
UINT8
GetImodeSettingsList (
  PSYSHOST host,
  UINT8    socket,
  INT16    settingList[MAX_ODT_SETTINGS][MAX_PARAMS]
)
{
  UINT8 numSettings = 0;
  UINT8 loop;
  INT16 SweepRange[9] = {(0),(1),(3),(4),(5),(7),(12),(13),(15)};//Characterised sweep values for BDX EP B0 and above & DE C0 and above.
  INT16 defaultImodeSettingList[5] = {{0},{1},{3},{7},{15}};

  if ((CheckSteppingGreaterThan(host, CPU_BDX, B0_REV_BDX) && IsDESku(host)) ||
      (CheckSteppingGreaterThan(host, CPU_BDX, A0_REV_BDX) && IsBDXEP(host))) {
    numSettings = 9; // Number of points characterised.
    for (loop = 0; loop < numSettings; loop++) {
      settingList[loop][0] = SweepRange[loop];
    }
  } else {
    numSettings = 5; // Number of points characterised.
    for (loop = 0; loop < numSettings; loop++) {
      settingList[loop][0] = defaultImodeSettingList[loop];
    }
  }

  return numSettings;
} // GetImodeSettingsList


/**

  This functions is for cleaning up any chipset bugs related to ReadDqDqsPerBit training

  @param host     - Pointer to sysHost
  @param socket   - Socket number

  @retval - None

**/
void
ReadDqDqsCleanup(
  PSYSHOST host,
  UINT8 socket
)
{
 // placeholde function
}

/**

  Get DIMM slot specific latency to be added to round trip

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Ch number
  @param dimm    - Dimm number

  @retval None

**/
UINT8
GetChipAdjForRTL(
    PSYSHOST host,
    UINT8    socket,
    UINT8    ch,
    UINT8    dimm
)
{
  UINT8 rtlAdj;

  if(dimm == 2) {
    rtlAdj = 2;
  } else {
    rtlAdj = 0;
  }

  return rtlAdj;
}

/**

  Get page table timer status

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Ch number

  @retval TRUE if enabled / FALSE if disabled

**/
BOOLEAN
IsChipPageTableTimerEnabled(
    PSYSHOST host,
    UINT8    socket,
    UINT8    ch
)
{
  MCSCHED_CHKN_BIT_MCDDC_CTL_HSX_BDX_STRUCT chknBit;

  chknBit.Data = MemReadPciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG);

  return (chknBit.Bits.dis_pgt_tmr == 0);
}

/**

  Enable/Disable page table timer

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Ch number
  @param state   - PGT_TIMER_DISABLE / PGT_TIMER_ENABLE

  @retval none

**/
void
SetChipPageTableTimer(
    PSYSHOST host,
    UINT8    socket,
    UINT8    ch,
    UINT8    state
)
{
  MCSCHED_CHKN_BIT_MCDDC_CTL_HSX_BDX_STRUCT chknBit;

  chknBit.Data = MemReadPciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG);
  if (state == PGT_TIMER_DISABLE) {
    chknBit.Bits.dis_pgt_tmr = 1;
  } else {
    chknBit.Bits.dis_pgt_tmr = 0;
  }
  MemWritePciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG, chknBit.Data);
}

void
RPQDrain(
         PSYSHOST host,
         UINT8    socket,
         UINT8    ch,
         UINT8    dimm,
         UINT8    rank
)
{
}

void
ToggleInternalClocksOnFnv(
                       PSYSHOST host,
                       UINT8    socket,
                       UINT32   chBitmask,
                       UINT8    dimm,
                       UINT8    value
)
{
}

void
ToggleTxOnFnv(
              PSYSHOST host,
              UINT8    socket,
              UINT32   chBitmask,
              UINT8    dimm,
              UINT8    value
)
{
}

void
GetDdrtTiming (
            PSYSHOST host,
            UINT8    socket,
            UINT8    ch,
            UINT8    dimm,
            UINT8    *CASTemp,
            UINT8    *CWLTemp
)
{
}

void
GetDdrtTimingBufferDelay (
            PSYSHOST host,
            UINT8    socket,
            UINT8    ch,
            UINT8    dimm,
            UINT8    *tCLAdd,
            UINT8    *tCWLAdd
)
{
}

void
DisableEridReturn(
            PSYSHOST host,
            UINT8    socket
)
{
}

void
FifoTrainReset (
                PSYSHOST host,
                UINT8    socket
)
{
}

UINT8
CheckSktId (
            PSYSHOST host,
            UINT8    socket
)
{
  return 0;
}

UINT8
CheckMemRas (
             PSYSHOST host
)
{
  if (host->var.common.socketId == host->nvram.common.sbspSocketId) return 1;
  else return 0;
}

void
LoadCapId (
            PSYSHOST host,
            UINT8    socket
)
{
  //
  // load local CAPID0 thru CAPID6 CSRs to host->var.common.procCom
  //
  host->var.common.procCom[socket].capid0 = ReadCpuPciCfgEx (host, socket, 0, CAPID0_PCU_FUN3_REG);
  host->var.common.procCom[socket].capid1 = ReadCpuPciCfgEx (host, socket, 0, CAPID1_PCU_FUN3_REG);
  host->var.common.procCom[socket].capid2 = ReadCpuPciCfgEx (host, socket, 0, CAPID2_PCU_FUN3_REG);
  host->var.common.procCom[socket].capid3 = ReadCpuPciCfgEx (host, socket, 0, CAPID3_PCU_FUN3_REG);
  host->var.common.procCom[socket].capid4 = ReadCpuPciCfgEx (host, socket, 0, CAPID4_PCU_FUN3_REG);
  host->var.common.procCom[socket].capid5 = ReadCpuPciCfgEx (host, socket, 0, CAPID5_PCU_FUN3_REG);
  host->var.common.procCom[socket].capid6 = ReadCpuPciCfgEx (host, socket, 0, CAPID6_PCU_FUN3_REG);
} // LoadCapId

#ifdef MEM_NVDIMM_EN
void
ChipDeassertCKE(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 ckeMask) {
    CPGC_MISCCKECTL_MCDDC_CTL_HSX_BDX_STRUCT miscCKECtl;
    // Make sure CKE is NOT asserted
    miscCKECtl.Data = MemReadPciCfgEp(host, socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
    miscCKECtl.Bits.cke_override = ckeMask;
    miscCKECtl.Bits.cke_on = 0;
    MemWritePciCfgEp(host, socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, miscCKECtl.Data);
}
#endif


/**

  Apply settings before AdvancedMemTest starts

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Ch number

  @retval UINT32 scrambleConfigOrg

**/
UINT32
PreAdvancedMemTest (
  PSYSHOST    host,
  UINT8       socket,
  UINT8       ch
)
{
  UINT32              scrambleConfigOrg;
  CPGC_INORDER_MCDDC_CTL_STRUCT             cpgcInOrder;
  MCSCHED_CHKN_BIT_MCDDC_CTL_HSX_BDX_STRUCT mcschedChx;
  MCSCRAMBLECONFIG_MCDDC_DP_STRUCT          scrambleConfig;

  // Disable scrambler
  scrambleConfig.Data = scrambleConfigOrg = MemReadPciCfgEp (host, socket, ch, MCSCRAMBLECONFIG_MCDDC_DP_REG);
  scrambleConfig.Bits.rx_enable = 0;
  scrambleConfig.Bits.tx_enable = 0;
  scrambleConfig.Bits.ch_enable = 0;
  MemWritePciCfgEp (host, socket, ch, MCSCRAMBLECONFIG_MCDDC_DP_REG, scrambleConfig.Data);

  cpgcInOrder.Data = MemReadPciCfgEp (host, socket, ch, CPGC_INORDER_MCDDC_CTL_REG);
  cpgcInOrder.Bits.wpq_inorder_en = 1;
  MemWritePciCfgEp (host, socket, ch, CPGC_INORDER_MCDDC_CTL_REG, cpgcInOrder.Data);

  mcschedChx.Data = MemReadPciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG);
  mcschedChx.Bits.dis_pgt_tmr = 1;
  MemWritePciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG, mcschedChx.Data);

  return scrambleConfigOrg;
}

/**

  Apply settings after AdvancedMemTest ends

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Ch number
  @param scrambleConfigOrg    Original scrambler config

  @retval None

**/
VOID
PostAdvancedMemTest (
  PSYSHOST    host,
  UINT8       socket,
  UINT8       ch,
  UINT32      scrambleConfigOrg
  )
{

  CPGC_INORDER_MCDDC_CTL_STRUCT             cpgcInOrder;
  MCSCHED_CHKN_BIT_MCDDC_CTL_HSX_BDX_STRUCT mcschedChx;
  MCSCRAMBLECONFIG_MCDDC_DP_STRUCT          scrambleConfig;

  // Restore scrambler
  scrambleConfig.Data = scrambleConfigOrg;
  MemWritePciCfgEp (host, socket, ch, MCSCRAMBLECONFIG_MCDDC_DP_REG, scrambleConfig.Data);

  cpgcInOrder.Data = MemReadPciCfgEp (host, socket, ch, CPGC_INORDER_MCDDC_CTL_REG);
  cpgcInOrder.Bits.wpq_inorder_en = 0;
  MemWritePciCfgEp (host, socket, ch, CPGC_INORDER_MCDDC_CTL_REG, cpgcInOrder.Data);

  mcschedChx.Data = MemReadPciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG);
  mcschedChx.Bits.dis_pgt_tmr = 0;
  MemWritePciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG, mcschedChx.Data);

}

UINT8
isCmdVrefTrainingDone (PSYSHOST host,
                       UINT8    socket
)
{
  return 0;
}

void
ToggleBcom(PSYSHOST host,
           UINT8    socket,
           UINT8    value
)
{
}

void
ProgramDAFnvMprPattern (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    pattern
)
{
}

void
ProgramDFnvMprPattern (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    pattern
  )
{
}

void
SetRankDAMprFnv (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    mprMode
  )
{
}

void
SetRankDMprFnv (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    mprMode
  )
{
}

void
EnforceWWDR(
            PSYSHOST host,
            UINT8    ch,
            UINT8    *tWWDR
)
{
}

void
EnforceWWDD(
            PSYSHOST host,
            UINT8    ch,
            UINT8    *tWWDD
)
{
}

/**

  Routine Description: Clears Prefetch Cache miss for NVMCTLR

  @param host    - Pointer to sysHost
  @param socket  - Socket number

  @retval SUCCESS
**/
UINT32
ClrPcheMiss (
                 PSYSHOST host,
                 UINT8    socket
                 )
{
  return SUCCESS;
}

void
ClearEridLfsrErr (
                   PSYSHOST host,
                   UINT8    socket,
                   UINT8    ch
)
{

} // ClearEridLfsrErr

void
AddChipRxDqsOffset (
                        PSYSHOST host,
                        UINT8 socket,
                        UINT8     ch,
                        UINT8     dimm,
                        UINT8     rank,
                        UINT8     strobe,
                        UINT16 *offset
                        )
{
  //HSD 5003120: Apply "-8" offset to RxDQs after basic training for all DDR3 configs
  if ((host->var.common.cpuType == CPU_BDX) && (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {
    if (*offset > 8) {
      *offset = *offset - 8;
      MemDebugPrint((host, SDBG_MEM_TRAIN, socket, ch, dimm, rank, strobe, NO_BIT, " Pi lowered by 8 ticks.\n"));
    } else {
      MemDebugPrint((host, SDBG_MEM_TRAIN, socket, ch, dimm, rank, strobe, NO_BIT, " Pi lower than 8 ticks. Skipping for this strobe.\n"));
    }
  }
} //AddChipRxDqsOffset
