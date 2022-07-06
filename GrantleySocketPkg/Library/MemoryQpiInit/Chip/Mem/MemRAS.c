
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
 *
 ************************************************************************/

#include "SysFunc.h"

#ifdef WINAPP
#define rcPrintf  APPprint
void
APPprint (
  PSYSHOST   host,
  const char *format,
  ...
  );
#endif

#define RAS_SKIP_CHANNEL  0
#define RAS_IS_CAPABLE    1
#define RAS_NOT_CAPABLE   2
#define RAS_CH_MIX_INV    3

//
// Disable warning for unsued input parameters
//
#pragma warning(disable : 4100)

typedef struct{
    UINT8  dimm;
    UINT8  rank;
}SparedRank;

//
// Local Prototypes
//
void   SyncronizeTimings(PSYSHOST host, UINT8 socket);
void   SetMirrorMode(struct sysHost *host, UINT8 socket);
void   SetErrorThreshold (struct sysHost *host,  UINT8 socket, UINT8 ch, UINT32 imcCORRERRTHRSHLD01Reg, UINT32 ErrThreshold);
void   SetSpareMode(struct sysHost *host, UINT8 socket);
UINT8  CheckMLCapable(struct sysHost *host, UINT8 socket, UINT8 ch0, UINT8 ch1);
UINT8  CheckSparingCapable(struct sysHost *host, UINT8 socket, UINT8 ch0);
UINT8  checkSupportedRASModes(struct sysHost *host);
UINT8  imcRasCapable(PSYSHOST host);

void
SetRankSizeForSpare(
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch
  )
/*++

Routine Description:

  Find out the spare ranks per channel, and sets the rank size in rankList

Arguments:

  host    - Pointer to sysHost
  socket  - Socket number
  ch      - Channel number (0-based)
Returns:

  N/A

--*/
{
  UINT8               spareRankNum;
  UINT8               rank;
  UINT16              size;
  UINT8               dimm;
  UINT8               k;
  UINT8               j;
  SparedRank          spareLogicalRank[MAX_RANK_CH/2];
  UINT8               sparedimm;
  UINT8               sparerank;
  UINT8               chRankEnabled = 0;
  UINT16              sparesize;
  struct ddrRank      *rankNvList0;
  struct dimmNvram    *dimmNvList0;
  struct channelNvram *channelNvList;

  channelNvList = &host->nvram.mem.socket[socket].channelList[ch];

  //
  // Find how many ranks need to be spared
  //
  for (dimm = 0; (dimm < channelNvList->maxDimm && dimm < MAX_DIMM); dimm++) {
    dimmNvList0 = &channelNvList->dimmList[dimm];
    if (dimmNvList0->dimmPresent == 0) continue;

    for (rank = 0; (rank < dimmNvList0->numRanks && rank < MAX_RANK_DIMM); rank++) {
      if (CheckRank(host, socket, ch, dimm, rank, CHECK_MAPOUT) == 0) chRankEnabled++;
    }

  }

  //
  // 50% spare rank
  //
  spareRankNum = chRankEnabled / 2;

  if(host->setup.mem.spareRanks < spareRankNum) {
    spareRankNum = host->setup.mem.spareRanks;
  }

  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "SpareRankNum:%d, SetupSpareNum: %d, Ch Ranks: %d, chRankEnabled: %d\n",
                spareRankNum, host->setup.mem.spareRanks, channelNvList->numRanks, chRankEnabled));

  for(j = 0; j < MAX_RANK_CH / 2; j++){
    spareLogicalRank[j].dimm= 0xff;
    spareLogicalRank[j].rank= 0xff;
  }

  //
  //search all DIMM and ranks to find spare ranks.
  //
  for(k = 0; k < spareRankNum; k++){

    sparedimm     = 0xff;
    sparerank     = 0xff;
    sparesize = 0;

    for (dimm = 0; (dimm < channelNvList->maxDimm && dimm < MAX_DIMM); dimm++) {

      dimmNvList0 = &channelNvList->dimmList[dimm];

      if (dimmNvList0->dimmPresent == 0) continue;

      //
      // Correct rank size for RAS mode
      // Loop for each rank
      //
      for (rank = 0; (rank < dimmNvList0->numRanks && rank < MAX_RANK_DIMM); rank++) {
        rankNvList0 = &channelNvList->dimmList[dimm].rankList[rank];

        //
        // Skip ranks that are mapped out
        //
        if (CheckRank(host, socket, ch, dimm, rank, CHECK_MAPOUT)) continue;

        //
        // If this rank has been selected, go ahead.
        //
        for(j = 0; j < MAX_RANK_CH / 2; j++){
          if((spareLogicalRank[j].dimm == dimm) && (spareLogicalRank[j].rank == rank)) break;
        }

        //
        // This rank has been logged, skip it.
        //
        if(j < MAX_RANK_CH / 2) continue;

        size = rankNvList0->rankSize;

        if (size > sparesize) {
          sparesize = size;
          sparedimm = dimm;
          sparerank = rank;
        }
        // Optimize spare rank to be non-termination rank
        else if ((size == sparesize) && !IsTermRank(host, socket, ch, dimm, rank)) {
          sparesize = size;
          sparedimm = dimm;
          sparerank = rank;
        }
      } // rank loop
    } // dimm loop

     if(sparedimm != 0xff){
         spareLogicalRank[k].dimm = sparedimm;
         spareLogicalRank[k].rank = sparerank;
         MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  " spared dimm and rank:%x  %x \n",
                  sparedimm,
                  sparerank
                  ));
          }
  } // spare rank loop


  for(k = 0; k < (MAX_RANK_CH / 2); k++){
    //
    // Spare is rank on largest dimm. If more than one largest choose rank 0 on the minimum largest dimm
    //
    if(spareLogicalRank[k].dimm == 0xff){
      channelNvList->spareDimm[k]  = 0xff;
      channelNvList->spareRankSize[k] = 0;
      channelNvList->spareLogicalRank[k] = 0xff;
      channelNvList->sparePhysicalRank[k] = 0xff;
      continue;
    }
    sparedimm                 = spareLogicalRank[k].dimm;
    sparerank                 = spareLogicalRank[k].rank;
    rankNvList0               = &channelNvList->dimmList[sparedimm].rankList[sparerank];
    channelNvList->spareDimm[k]  = sparedimm;
    channelNvList->spareRank[k]  = sparerank;
    channelNvList->spareRankSize[k] = rankNvList0->rankSize;
    channelNvList->spareLogicalRank[k] = rankNvList0->logicalRank;
    channelNvList->sparePhysicalRank[k] = rankNvList0->phyRank;
    rankNvList0->rankSize     = 0;
    rankNvList0->remSize      = 0;
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Spare DIMM %d, Spare logical rank %d, Spare phy rank id %d, ch spare logical rank:%x  %x  %x  %x\n",
                  sparedimm, rankNvList0->logicalRank, rankNvList0->phyRank,
                  channelNvList->spareLogicalRank[0],
                  channelNvList->spareLogicalRank[1],
                  channelNvList->spareLogicalRank[2],
                  channelNvList->spareLogicalRank[3]
                  ));
  } // spare rank loop

      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  " ch spare logical rank:%x  %x  %x  %x\n",
                  channelNvList->spareLogicalRank[0],
                  channelNvList->spareLogicalRank[1],
                  channelNvList->spareLogicalRank[2],
                  channelNvList->spareLogicalRank[3]
                  ));
}


void LockStepModeRankSize (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    channelCopy,
  UINT8    sizeFactor,
  UINT8    RASMode
  )
/*++

Routine Description:

  This method sets rank size for Channel 0/2 and Channel 1/3

Arguments:

  host    - Pointer to sysHost
  socket  - Socket number
  ch      - Channel number (0-based)
  channelCopy - Channel number to be copied from
  RASMode - Current RASMode
Returns:

  N/A

--*/
{
  UINT8               i;
  UINT16              size;
  UINT8               dimm;
  struct ddrRank      *rankNvList0;
  struct dimmNvram    *dimmNvList0;
  struct channelNvram *channelNvList;

  channelNvList = &host->nvram.mem.socket[socket].channelList[ch];

  for (dimm = 0; dimm < channelNvList->maxDimm; dimm++) {

    dimmNvList0 = &channelNvList->dimmList[dimm];

    if (dimmNvList0->dimmPresent == 0) continue;

    //
    // Correct rank size for RAS mode
    // Loop for each rank
    //
    for (i = 0; i < dimmNvList0->numDramRanks; i++) {
      rankNvList0 = &channelNvList->dimmList[dimm].rankList[i];

      //
      // Skip ranks that are mapped out
      //
      if (CheckRank(host, socket, ch, dimm, i, CHECK_MAPOUT)) continue;

      //
      // Determine rank size
      //
      size = rankNvList0->rankSize;

      if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {

        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, i, NO_STROBE, NO_BIT,
                       "size %d\nTechIndex 0x%x, size 0x%x\n", size, dimmNvList0->techIndex, rankSizeDDR4[dimmNvList0->techIndex]));
      } else
      MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, i, NO_STROBE, NO_BIT,
                    "size %d\nTechIndex 0x%x, size 0x%x\n", size, dimmNvList0->techIndex, rankSize[dimmNvList0->techIndex]));

      //
      // Correct for RAS mode
      //
      rankNvList0->rankSize = sizeFactor * size;
      rankNvList0->remSize  = sizeFactor * size;
    } // rank loop
  } // dimm loop
}

void LockStepModeSparing (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    channelCopy
  )
/*++

Routine Description:

  This method copies the sparedimm and sparerank properties from Channel 0/2 into Channel 1/3

Arguments:

  host    - Pointer to sysHost
  socket  - Socket number
  ch      - Channel number (0-based)
  channelCopy - Channel number to be copied from
  RASMode - Current RASMode
Returns:

  N/A

--*/
{
  UINT8               sparedimm;
  UINT8               sparerank;
  UINT8               k;
  struct ddrRank      *rankNvList0;
  struct channelNvram *channelNvList,*channelNvListCopy;

  channelNvList = &host->nvram.mem.socket[socket].channelList[ch];
  channelNvListCopy = &host->nvram.mem.socket[socket].channelList[channelCopy];

  for(k = 0; k < (MAX_RANK_CH/2); k++){
    //
    // copy contents of channel0/2 to channel 1/3 respectively.
    //
    if(channelNvListCopy->spareDimm[k] == 0xff){ // Check if Dimm has been chosen as spare.
      channelNvList->spareDimm[k]  = 0xff;       // Rank is spared when spareDimm != 0xff
      channelNvList->spareRankSize[k] = 0;
      channelNvList->spareLogicalRank[k] = 0xff;
      channelNvList->sparePhysicalRank[k] = 0xff;
      continue;
    }
    sparedimm                 = channelNvListCopy->spareDimm[k];
    sparerank                 = channelNvListCopy->spareRank[k];
    rankNvList0               = &channelNvList->dimmList[sparedimm].rankList[sparerank];
    channelNvList->spareDimm[k]  = sparedimm;
    channelNvList->spareRank[k]  = sparerank;
    channelNvList->spareRankSize[k] = channelNvListCopy->spareRankSize[k];
    channelNvList->spareLogicalRank[k] = channelNvListCopy->spareLogicalRank[k];
    channelNvList->sparePhysicalRank[k] = channelNvListCopy->sparePhysicalRank[k];
    rankNvList0->rankSize     = 0;
    rankNvList0->remSize      = 0;
    MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Spare DIMM %d, Spare logical rank %d, Spare phy rank id %d ch spare logical rank:%x  %x  %x  %x\n",
                  sparedimm, rankNvList0->logicalRank, rankNvList0->phyRank,
                  channelNvList->spareLogicalRank[0],
                  channelNvList->spareLogicalRank[1],
                  channelNvList->spareLogicalRank[2],
                  channelNvList->spareLogicalRank[3]
                  ));
  } // spare rank loop

  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "ch spare logical rank:%x %x %x %x\n",
                channelNvList->spareLogicalRank[0],
                channelNvList->spareLogicalRank[1],
                channelNvList->spareLogicalRank[2],
                channelNvList->spareLogicalRank[3]
                ));

}

void
SetRankSize (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    sizeFactor,
  UINT8    RASMode
  )
/*++

Routine Description:

  Sets the rank size in rankList

Arguments:

  host        - Pointer to sysHost
  socket        - Socket number
  ch          - Channel number (0-based)
  sizeFactor  - Size factor based on the RAS mode
  RASMode     - Current RAS mode

Returns:

  N/A

--*/
{

  UINT8               i;
  UINT16              size;
  UINT8               dimm;

  struct ddrRank      *rankNvList0;
  struct dimmNvram    *dimmNvList0;
  struct channelNvram *channelNvList;


  channelNvList = &host->nvram.mem.socket[socket].channelList[ch];

  for (dimm = 0; dimm < channelNvList->maxDimm; dimm++) {

    dimmNvList0 = &channelNvList->dimmList[dimm];

    if (dimmNvList0->dimmPresent == 0) continue;

#ifdef MEM_NVDIMM_EN
    if (dimmNvList0->nvDimmType) continue;
#endif
#ifdef NVMEM_FEATURE_EN
    if(host->setup.mem.socket[socket].ddrCh[ch].batterybacked) continue;
#endif
    //
    // Correct rank size for RAS mode
    // Loop for each rank
    //
    for (i = 0; i < dimmNvList0->numDramRanks; i++) {
      rankNvList0 = &channelNvList->dimmList[dimm].rankList[i];

      //
      // Skip ranks that are mapped out
      //
      if (CheckRank(host, socket, ch, dimm, i, CHECK_MAPOUT)) continue;

      //
      // Determine rank size
      //
      size = rankNvList0->rankSize;

      if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {

        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, i, NO_STROBE, NO_BIT,
                       "size %d\nTechIndex 0x%x, size 0x%x\n", size, dimmNvList0->techIndex, rankSizeDDR4[dimmNvList0->techIndex]));
      } else
      MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, i, NO_STROBE, NO_BIT,
                    "size %d\nTechIndex 0x%x, size 0x%x\n", size, dimmNvList0->techIndex, rankSize[dimmNvList0->techIndex]));

      //
      // Correct for RAS mode
      //
      rankNvList0->rankSize = sizeFactor * size;
      rankNvList0->remSize  = sizeFactor * size;
    } // rank loop
  } // dimm loop
}

void
CheckExtendedRASModes (PSYSHOST host)
{
  UINT8 setupRASmodeEx;
  UINT8 supportedRASModesEx;
  UINT8 missingRASModesEx;
  UINT8 ch;
  UINT8 socket;
  struct channelNvram (*channelNvList)[MAX_CH];

  // Get supported RAS Modes
  setupRASmodeEx = host->setup.mem.RASmodeEx;
  supportedRASModesEx = host->var.mem.RASModesEx;

  //
  // Disable Device Tagging if in Independent mode with X8 DIMMs
  //
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) continue;

    channelNvList = &host->nvram.mem.socket[socket].channelList;

    for (ch = 0; ch < MAX_CH; ch++){

      if ((*channelNvList)[ch].enabled == 0) continue;

      if (!(host->var.mem.RASModes & LOCKSTEP_EN)){

        if ((*channelNvList)[ch].features & X8_PRESENT){
          supportedRASModesEx &= ~DEVTAGGING_EN;
        }
      }
    }
  }

  host->nvram.mem.RASmodeEx = setupRASmodeEx & supportedRASModesEx;
  missingRASModesEx = setupRASmodeEx & (~supportedRASModesEx);

  // Check if patrol scrubbing supported
  if (missingRASModesEx & PTRLSCRB_EN) {
    // Indicate RAS mode disabled
    //LogWarning(host, WARN_PTRLSCRB_DISABLE, 0, (UINT32)-1);
  }
  return;
}

UINT32
RASCheckDimmRanks (
  PSYSHOST host
  )
/*++

Routine Description:

  Initialize rank structures.  This is based on the requested
  RAS mode and what RAS modes the configuration supports. This
  routine also evalues the requested RAS modes to ensure they
  are supported by the system configuration.

Arguments:

  host  - Pointer to sysHost

Returns:

  SUCCESS

--*/
{
  UINT8 socket;
  UINT8 setupRASModes;
  UINT8 nvramRASModes;
  UINT8 supportedRASModes;
  UINT8 missingRASModes;
  UINT8 RASMode;

  //
  // Get RAS Modes supported by the current DIMM configuration
  //
  supportedRASModes       = checkSupportedRASModes (host);

  //
  // Save supported modes in host
  //
  host->var.mem.RASModes  = supportedRASModes;

  //
  // this function updates the EX RAS MODE based on the setup and supported values.
  //
  CheckExtendedRASModes (host);

  //
  // Check which mode to run by comparing supported RAS capabilities and setup
  //
  setupRASModes = host->setup.mem.RASmode;

  //
  // Sparing and mirror are mutually exclusive: should not both be set in setup
  // If both set, turn off mirroring and issue warning
  //
  if ((setupRASModes & CH_MS) == CH_MS) {
    setupRASModes &= ~CH_MIRROR;
    LogWarning (host, WARN_MIRROR_DISABLE, WARN_MIRROR_DISABLE_MINOR_RAS_DISABLED, (UINT32) -1);
  }

  nvramRASModes = setupRASModes & supportedRASModes;
  missingRASModes = setupRASModes & (~supportedRASModes);

  //
  // If either MIRROR or LOCKSTEP is missing clear both (set to CH_INDEPENDENT)
  //
  if ((missingRASModes & CH_ML) != 0) {
    nvramRASModes = nvramRASModes &~CH_ML;
  }

  RASMode = nvramRASModes;

  //
  // Save operational RAS mode in host (nvram and volatile)
  //
  //host->var.mem.OpRASmode = RASMode;
  host->nvram.mem.RASmode = RASMode;

  //
  // Output warning for non-supported modes
  //
  if (missingRASModes & CH_MIRROR) {
    LogWarning (host, WARN_MIRROR_DISABLE, WARN_MIRROR_DISABLE_MINOR_RAS_DISABLED, (UINT32) -1);
  }

  if (missingRASModes & RK_SPARE) {
    LogWarning (host, WARN_SPARE_DISABLE, 0, (UINT32) -1);
  }

  if (missingRASModes & CH_LOCKSTEP) {
    LogWarning (host, WARN_LOCKSTEP_DISABLE, WARN_LOCKSTEP_DISABLE_MINOR_RAS_MODE, (UINT32) -1);
  }

// Set RANK size for selected mode, includes size accounting for the spare rank size
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) continue;

    switch (RASMode & CH_ML) {
    case CH_LOCKSTEP:
      //
      // set rank size for each lockstep channel: ch0/2: size = 2x, ch1/3: size = 0x
      //
      SetRankSize (host, socket, 0, 2, RASMode);
      SetRankSize (host, socket, 2, 2, RASMode);
      LockStepModeRankSize (host, socket, 1, 0, 0, RASMode);
      LockStepModeRankSize (host, socket, 3, 2, 0, RASMode);

      break;

    case CH_MIRROR:
      //
      // set rank size for each mirror channel: ch0/2: size = 1x, ch1/3: size = 0x
      //
      SetRankSize (host, socket, 0, 1, RASMode);
      SetRankSize (host, socket, 1, 0, RASMode);
      SetRankSize (host, socket, 2, 1, RASMode);
      SetRankSize (host, socket, 3, 0, RASMode);
      break;
    case CH_INDEPENDENT:
      //
      // set rank size for each channel: ch0/1/2/3: size = 1x
      //
      SetRankSize (host, socket, 0, 1, RASMode);
      SetRankSize (host, socket, 1, 1, RASMode);
      SetRankSize (host, socket, 2, 1, RASMode);
      SetRankSize (host, socket, 3, 1, RASMode);
      break;
    }
  } // socket loop

  return SUCCESS;
}

UINT32
CheckRASSupportAfterMemInit (
  PSYSHOST host
  )
/*++

Routine Description:

  Initialize rank structures.  This is based on the requested
  RAS mode and what RAS modes the configuration supports. This
  routine also evalues the requested RAS modes to ensure they
  are supported by the system configuration.

Arguments:

  host  - Pointer to sysHost

Returns:

  SUCCESS

--*/
{

  UINT8                     dimm;
  UINT8                     rank;
  UINT8                     ch;
  UINT8                     socket;
  UINT32                    dimmAmap;
  AMAP_MCDDC_DESC_STRUCT    imcAMAPReg;
  DIMMMTR_0_MCDDC_DESC_BDX_STRUCT dimmMtr;
  struct channelNvram       (*channelNvList)[MAX_CH];
  struct dimmNvram          (*dimmNvList)[MAX_DIMM];

  socket = host->var.mem.currentSocket;

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  //
  // Update dimmmtr.rank_disable setting on mapout results after memtest
  //
  CheckRankPopLocal(host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
    dimmAmap = 0;

    //
    // If more than 1+ rank is mapped out on a channel use AMAP dimmX_pat_rank_disable to disable scrubbing
    //
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      dimmMtr.Data = MemReadPciCfgEp (host, socket, ch, DIMMMTR_0_MCDDC_DESC_REG + (dimm * 4));

      for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {

        // Disable Patrol Scrub when ranks are mapped out and rank_disable is 0

        if (((*dimmNvList)[dimm].mapOut[rank] && ((dimmMtr.Bits.rank_disable & (1 << rank)) == 0)) ||
            (((*dimmNvList)[dimm].SPDSecondaryDeviceType & (SPD_QUARTER_SIZE_SECOND_RANK + SPD_HALF_SIZE_SECOND_RANK)) && ((rank) % 2))) {

          dimmAmap |= (1 << (dimm * 8 + rank));

          // Update AMAP register
          imcAMAPReg.Data = MemReadPciCfgEp (host, socket, ch, AMAP_MCDDC_DESC_REG);

          imcAMAPReg.Bits.dimm0_pat_rank_disable |= (UINT8)(dimmAmap & 0xff);
          imcAMAPReg.Bits.dimm1_pat_rank_disable |= (UINT8)((dimmAmap >> 8) & 0xff);
          imcAMAPReg.Bits.dimm2_pat_rank_disable |= (UINT8)((dimmAmap >> 16) & 0xff);
          MemWritePciCfgEp (host, socket, ch, AMAP_MCDDC_DESC_REG,imcAMAPReg.Data);
        }
      }// rank
    }// dimm
  } // ch

  //
  // Adjust for spare ranksize
  //
  if (host->nvram.mem.RASmode & RK_SPARE) {
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      SetRankSizeForSpare(host, socket, ch);
    }
    if (host->nvram.mem.RASmode & CH_LOCKSTEP){
      LockStepModeSparing (host, socket, 1, 0);
      LockStepModeSparing (host, socket, 3, 2);
    }
  }

  return SUCCESS;
}

void
SetLockstepMode (
  PSYSHOST host,
  UINT8    socket
  )
/*++

Routine Description:

  Configure Lockstep mode

Arguments:

  host  - Pointer to sysHost
  socket  - Socket number

Returns:

  N/A

--*/
{

  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             rank;
  UINT8                             strobe;
  UINT8                             mcId;
  UINT8                             logRank;
  UINT8                             chDone;
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  HASYSDEFEATURE_HA_CFG_STRUCT      haSYSDEFEATURE;
  MCMTR_MC_MAIN_HSX_BDX_STRUCT      imcMCMTR;
  TCRWP_MCDDC_CTL_STRUCT            tcrwp;
  RXGROUP0N0RANK0_0_MCIO_DDRIO_STRUCT rxGroup0;

  DATACONTROL0_0_MCIO_DDRIO_STRUCT  dataControl0;
  CPGC_GLOBALCTRL_MC_MAINEXT_STRUCT cpgcGlobalCtrlReg;

  //
  // Configure Lockstep mode
  // Note: lockstep capability bit checked in checkSupportedRASModes() function
  //
  if ((host->nvram.mem.RASmode & CH_LOCKSTEP) == CH_LOCKSTEP) {

    //
    // Make sure timings are syncronized accross lockstep pairs
    //
    SyncronizeTimings(host, socket);

    //
    // Set burst length to 4 for lockstep
    //
    channelNvList = &host->nvram.mem.socket[socket].channelList;

    if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)) {

      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;

        dimmNvList  = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

        for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {
          //
          // Skip if no DIMM present
          //
          if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

          rankStruct  = &host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct;

          //
          // Loop for each rank
          //
          for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
            //
            // Skip if no rank
            //
            if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

            (*rankStruct)[rank].MR0 = (*rankStruct)[rank].MR0 | MR0_B4;
            WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR0, BANK0);

          } // rank loop
        } // dimm loop

        dataControl0.Data = (*channelNvList)[ch].dataControl0;
        dataControl0.Bits.burstlength4 = 1;
        (*channelNvList)[ch].dataControl0 = dataControl0.Data;

        for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);

        } // strobe loop
      } // ch loop

      //
      // Lockstep WA for HSD4985799
      //
      if (host->nvram.mem.dramType == SPD_TYPE_DDR3){
        for (ch = 0; ch < MAX_CH; ch++) {
          if ((*channelNvList)[ch].enabled == 0) continue;

          chDone = 0;
          dimmNvList  = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
          tcrwp.Data = MemReadPciCfgEp (host, socket, ch, TCRWP_MCDDC_CTL_REG);

          if ((tcrwp.Bits.t_rrdr == 0) || (tcrwp.Bits.t_rrdd == 0)){
            for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {

              if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
              if (chDone) break;

              for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {

                if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
                if (chDone) break;

                logRank = GetLogicalRank(host, socket, ch, dimm, rank);

                for (strobe = 0; strobe < MAX_STROBE; strobe++) {

                  rxGroup0.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, logRank, strobe, RXGROUP0N0RANK0_0_MCIO_DDRIO_REG));

                  if (rxGroup0.Bits.rcvendelay >= 0x180){
                    if (tcrwp.Bits.t_rrdr == 0) tcrwp.Bits.t_rrdr = 1;
                    if (tcrwp.Bits.t_rrdd == 0) tcrwp.Bits.t_rrdd = 1;
                    (*channelNvList)[ch].tcrwp = tcrwp.Data;
                    MemWritePciCfgEp (host, socket, ch, TCRWP_MCDDC_CTL_REG, tcrwp.Data);
                    chDone = 1;
                    break;
                  }
                } // strobe loop
              } // rank loop
            } // dimm loop
          }
        }// ch loop
      }
    }else if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBootFast)) {
      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;

        dimmNvList  = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

        for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {
          //
          // Skip if no DIMM present
          //
          if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

          rankStruct  = &host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct;

          //
          // Loop for each rank
          //
          for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
            //
            // Skip if no rank
            //
            if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

            (*rankStruct)[rank].MR0 = (*rankStruct)[rank].MR0 | MR0_B4;
            WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR0, BANK0);

          } // rank loop
        } // dimm loop
      } // ch loop
    }

    //
    // Program HA Lockstep Enables
    //
    for (mcId = 0; mcId < MAX_IMC; mcId++) {
      if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

      //
      // Program iMC Lockstep enables
      //
      imcMCMTR.Data       = MemReadPciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG);
      imcMCMTR.Bits.ls_en = 1;
      MemWritePciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG, imcMCMTR.Data);

      haSYSDEFEATURE.Data             = MemReadPciCfgMC (host, socket, mcId, HASYSDEFEATURE_HA_CFG_REG);
      haSYSDEFEATURE.Bits.lockstep_en = 1;
      MemWritePciCfgMC (host, socket, mcId, HASYSDEFEATURE_HA_CFG_REG, haSYSDEFEATURE.Data);

      cpgcGlobalCtrlReg.Data = MemReadPciCfgMC (host, socket, mcId, CPGC_GLOBALCTRL_MC_MAINEXT_REG);
      cpgcGlobalCtrlReg.Bits.use_bl4_col_addr = 1;
      MemWritePciCfgMC (host, socket, mcId, CPGC_GLOBALCTRL_MC_MAINEXT_REG, cpgcGlobalCtrlReg.Data);

    } // mcId loop

    MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Lockstep enabled\n"));
  }

  return;
}

void
SyncronizeTimings (
  PSYSHOST  host,
  UINT8     socket
  )
/*++

Routine Description:

  Make sure timings are syncronized accross lockstep pairs

Arguments:

  host  - Pointer to sysHost
  socket  - Socket number

Returns:

  N/A

--*/
{
  UINT8                     chA;
  UINT8                     chB;
  UINT8                     chLD;
  UINT8                     chCWL;
  UINT8                     dimm;
  UINT8                     rank;
  UINT8                     logRank;
  UINT8                     strobe;
  UINT8                     rtA;
  UINT8                     rtB;
  UINT8                     ioA;
  UINT8                     ioB;
  INT8                      tCWLAdjA;
  INT8                      tCWLAdjB;
  INT8                      tCWLAdjUpdate;
  INT16                     txLogicDelayAMax;
  INT16                     txLogicDelayAMin;
  INT16                     txLogicDelayBMax;
  INT16                     txLogicDelayBMin;
  INT16                     txLogicDelayOffset;
  UINT16                    tmpLogicDelay;
  struct dimmNvram          (*dimmNvList)[MAX_DIMM];
  struct channelNvram       (*channelNvList)[MAX_CH];
  TCOTHP_MCDDC_CTL_STRUCT   tCOTHP;
  UINT8 rcWriteRegDumpCurrent = host->var.common.rcWriteRegDump;

  host->var.common.rcWriteRegDump = 0;

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  for (chA = 0; chA < MAX_CH; chA = chA + 2) {
    if ((*channelNvList)[chA].enabled == 0) continue;

    chB = chA + 1;

    dimmNvList  = &host->nvram.mem.socket[socket].channelList[chA].dimmList;
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank(host, socket, chA, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        //
        // Get the logical rank #
        //
        logRank = GetLogicalRank(host, socket, chA, dimm, rank);

        //
        // Get round trip latency for each channel
        //
        rtA = GetRoundTrip(host, socket, chA, logRank);
        rtB = GetRoundTrip(host, socket, chB, logRank);

        MemDebugPrint((host, SDBG_MAX, socket, chA, dimm, rank, NO_STROBE, NO_BIT,
                      "round trip latency  %d\n", rtA));
        MemDebugPrint((host, SDBG_MAX, socket, chA, dimm, rank, NO_STROBE, NO_BIT,
                      "IO latency  %d\n", GetIOLatency(host, socket, chA, logRank)));
        MemDebugPrint((host, SDBG_MAX, socket, chB, dimm, rank, NO_STROBE, NO_BIT,
                      "round trip latency  %d\n", rtB));
        MemDebugPrint((host, SDBG_MAX, socket, chB, dimm, rank, NO_STROBE, NO_BIT,
                      "IO latency  %d\n", GetIOLatency(host, socket, chB, logRank)));
        //
        // If round trip A is larger than B
        //
        if (rtA > rtB) {
          //
          // Use the larger RTL
          //
          SetRoundTrip(host, socket, chB, logRank, rtA);

          //
          // Adjust IO latency so receive enable happens at the same point in time
          //
          ioB = GetIOLatency(host, socket, chB, logRank);
          SetIOLatency(host, socket, chB, logRank, ioB + (rtA - rtB));

          MemDebugPrint((host, SDBG_MAX, socket, chB, dimm, rank, NO_STROBE, NO_BIT,
                      "Changing round trip lat to %d\n", rtA));
          MemDebugPrint((host, SDBG_MAX, socket, chB, dimm, rank, NO_STROBE, NO_BIT,
                      "Changing IO lat to %d\n", ioB + (rtA - rtB)));
        //
        // If round trip B is larger than A
        //
        } else if (rtB > rtA) {
          //
          // Use the larger RTL
          //
          SetRoundTrip(host, socket, chA, logRank, rtB);

          //
          // Adjust IO latency so receive enable happens at the same point in time
          //
          ioA = GetIOLatency(host, socket, chA, logRank);
          SetIOLatency(host, socket, chA, logRank, ioA + (rtB - rtA));

          MemDebugPrint((host, SDBG_MAX, socket, chA, dimm, rank, NO_STROBE, NO_BIT,
                        "Changing round trip lat to %d\n", rtB));
          MemDebugPrint((host, SDBG_MAX, socket, chA, dimm, rank, NO_STROBE, NO_BIT,
                        "Changing IO lat to %d\n", ioA + (rtB - rtA)));
        }
      } // rank loop
    } // dimm loop

    //
    // Read the CWL Adjust for each channel
    //
    tCOTHP.Data = MemReadPciCfgEp (host, socket, chA, TCOTHP_MCDDC_CTL_REG);
    tCWLAdjA = (INT8)tCOTHP.Bits.t_cwl_adj;
    tCOTHP.Data = MemReadPciCfgEp (host, socket, chB, TCOTHP_MCDDC_CTL_REG);
    tCWLAdjB = (INT8)tCOTHP.Bits.t_cwl_adj;

    //
    // Convert each to QCLK
    //
    if (tCWLAdjA != tCWLAdjB) {
      switch (tCWLAdjA) {
        case 0:
          tCWLAdjA = 0;
          break;
        case 1:
          tCWLAdjA = 1;
          break;
        case 2:
          tCWLAdjA = 2;
          break;
        case 3:
          tCWLAdjA = 3;
          break;
        case 4:
          tCWLAdjA = -1;
          break;
      } // switch tCWLAdjA

      switch (tCWLAdjB) {
        case 0:
          tCWLAdjB = 0;
          break;
        case 1:
          tCWLAdjB = 1;
          break;
        case 2:
          tCWLAdjB = 2;
          break;
        case 3:
          tCWLAdjB = 3;
          break;
        case 4:
          tCWLAdjB = -1;
          break;
      } // switch tCWLAdjB

      //
      // Initialize min/max parameters
      //
      txLogicDelayAMax = 0;
      txLogicDelayAMin = 0x7F;
      txLogicDelayBMax = 0;
      txLogicDelayBMin = 0x7F;
      txLogicDelayOffset = 0;
      tCWLAdjUpdate = 0;
      chLD = 0;
      chCWL = 0;

      dimmNvList  = &host->nvram.mem.socket[socket].channelList[chA].dimmList;
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank(host, socket, chA, dimm, rank, DONT_CHECK_MAPOUT)) continue;

          for (strobe = 0; strobe < MAX_STROBE; strobe++) {
            GetSetDataGroup (host, socket, chA, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&tmpLogicDelay);
            tmpLogicDelay = tmpLogicDelay / 64;
            UpdateMinMax((UINT16)tmpLogicDelay, (UINT16 *)&txLogicDelayAMin, (UINT16 *)&txLogicDelayAMax);

            GetSetDataGroup (host, socket, chB, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&tmpLogicDelay);
            tmpLogicDelay = tmpLogicDelay / 64;
            UpdateMinMax((UINT16)tmpLogicDelay, (UINT16 *)&txLogicDelayBMin, (UINT16 *)&txLogicDelayBMax);
          } // strobe loop
        } // rank loop
      } // dimm loop

      MemDebugPrint((host, SDBG_MAX, socket, chA, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "txLogicDelayMin =  %d\n", txLogicDelayAMin));
      MemDebugPrint((host, SDBG_MAX, socket, chA, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "txLogicDelayMax =  %d\n", txLogicDelayAMax));
      MemDebugPrint((host, SDBG_MAX, socket, chB, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "txLogicDelayMin =  %d\n", txLogicDelayBMin));
      MemDebugPrint((host, SDBG_MAX, socket, chB, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "txLogicDelayMax =  %d\n", txLogicDelayBMax));
      if ((tCWLAdjA < tCWLAdjB) && (txLogicDelayBMax + ((tCWLAdjB - tCWLAdjA)*2) <= 7)) {
        // tCWLAdjA is smaller and txLogicDelayB has upside range
        chLD = chB;
        txLogicDelayOffset = (tCWLAdjB - tCWLAdjA);
        chCWL = chB;
        tCWLAdjUpdate = tCWLAdjA;
      } else if ((tCWLAdjA < tCWLAdjB) && (txLogicDelayAMin - ((tCWLAdjB - tCWLAdjA)*2) >= 0)) {
        // tCWLAdjA is smaller and txLogicDelayA has downside range
        chLD = chA;
        txLogicDelayOffset = (INT16)-(tCWLAdjB - tCWLAdjA);
        chCWL = chA;
        tCWLAdjUpdate = tCWLAdjB;
      } else if ((tCWLAdjB < tCWLAdjA) && (txLogicDelayAMax + ((tCWLAdjA - tCWLAdjB)*2) <= 7)) {
        // tCWLAdjB is smaller and txLogicDelayA has upside range
        chLD = chA;
        txLogicDelayOffset = tCWLAdjA - tCWLAdjB;
        chCWL = chA;
        tCWLAdjUpdate = tCWLAdjB;
      } else if ((tCWLAdjB < tCWLAdjA) && (txLogicDelayBMin - ((tCWLAdjA - tCWLAdjB)*2) >= 0)) {
        // tCWLAdjB is smaller and txLogicDelayB has downside range
        chLD = chB;
        txLogicDelayOffset = (INT16)-(tCWLAdjA - tCWLAdjB);
        chCWL = chB;
        tCWLAdjUpdate = tCWLAdjA;
      } else {
        // Too much channel to channel skew for lockstep operation
        MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "WARNING: Not enough Tx Pi range found between channels %d and %d\n", chA, chB));
      }

      //
      // Update t_cwl_adj
      //
      tCOTHP.Data = MemReadPciCfgEp (host, socket, chCWL, TCOTHP_MCDDC_CTL_REG);
      if (tCWLAdjUpdate < 0)
        tCOTHP.Bits.t_cwl_adj = 4;
      else {
        tCOTHP.Bits.t_cwl_adj = tCWLAdjUpdate;
      }
      MemWritePciCfgEp (host, socket, chCWL, TCOTHP_MCDDC_CTL_REG, tCOTHP.Data);
      (*channelNvList)[chCWL].tcothp = tCOTHP.Data;

      MemDebugPrint((host, SDBG_MAX, socket, chCWL, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Adjusting t_cwl_adj to %d\n", tCWLAdjUpdate));
      MemDebugPrint((host, SDBG_MAX, socket, chLD, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Adjusting Tx Dqs logic delay by %d\n", txLogicDelayOffset));
      dimmNvList  = &host->nvram.mem.socket[socket].channelList[chLD].dimmList;
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank(host, socket, chLD, dimm, rank, DONT_CHECK_MAPOUT)) continue;

          for (strobe = 0; strobe < MAX_STROBE; strobe++) {
            GetSetDataGroup (host, socket, chLD, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&tmpLogicDelay);
            tmpLogicDelay = (UINT16)((INT16)tmpLogicDelay + (txLogicDelayOffset * 128));
            GetSetDataGroup (host, socket, chLD, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE, (INT16 *)&tmpLogicDelay);

            GetSetDataGroup (host, socket, chLD, dimm, rank, strobe, 0, DdrLevel, TxDqDelay, GSM_READ_ONLY, (INT16 *)&tmpLogicDelay);
            if (((INT16)tmpLogicDelay + (txLogicDelayOffset * 128)) < 0) {
              MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                            "WARNING: Not enough downside Tx Pi range to adjust channels %d and %d\n", chA, chB));
            } else {
              tmpLogicDelay = (UINT16)((INT16)tmpLogicDelay + ((INT16)txLogicDelayOffset * 128));
            }
            GetSetDataGroup (host, socket, chLD, dimm, rank, strobe, 0, DdrLevel, TxDqDelay, GSM_UPDATE_CACHE, (INT16 *)&tmpLogicDelay);
          } // strobe loop
        } // rank loop
      } // dimm loop

    } // if tCLAdj's are not equal
  } // chA loop

  host->var.common.rcWriteRegDump = rcWriteRegDumpCurrent;

} // SyncronizeTimings

UINT32
MemWaDisableUCPatrolPoisoning(
    PSYSHOST host,
    UINT8    socket
  )
  /*
  Routine Description:`
   A CV mirroring silicon sighting (s4677957) exposed a Home Agent bug when
   performing Windows stress testing. CV is seeing corrected errors when they
   enable channel mirroring (without error injection). The workaround suggested
   by DE is to prevent the patrol scrub engine from poisoning a line when
   mirroring is enabled by setting the following bit - mcmain_chkn_bits[dis_ptrl_psn]=1
   and jktpoisonhandling =1. Please note that this WA is to be applied only under mirroring.

Arguments:

    i/p : Socket ID and host data structure

Returns:

    EFI_SUCCESS if the call is succeed


  */
{
  UINT8                                 mcId;
  MCMAIN_CHKN_BITS_MC_MAIN_STRUCT       mcMainChknBits;
  HASYSDEFEATURE2_HA_CFG_HSX_BDX_STRUCT haSYSDEFEATURE2;

  for (mcId = 0; mcId < MAX_IMC; mcId++) {
    if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

    mcMainChknBits.Data = MemReadPciCfgMC (host, socket, mcId, MCMAIN_CHKN_BITS_MC_MAIN_REG);
    mcMainChknBits.Bits.dis_ptrl_psn = 1;
    MemWritePciCfgMC (host, socket, mcId, MCMAIN_CHKN_BITS_MC_MAIN_REG, mcMainChknBits.Data);

    haSYSDEFEATURE2.Data = MemReadPciCfgMC (host, socket, mcId, HASYSDEFEATURE2_HA_CFG_REG);
    haSYSDEFEATURE2.Bits.jktpoisonhandling = 1;
    MemWritePciCfgMC (host, socket, mcId, HASYSDEFEATURE2_HA_CFG_REG, haSYSDEFEATURE2.Data);
  }

  return SUCCESS;
}

UINT32
MemWaDisableUCPatrol(
    PSYSHOST host,
    UINT8    socket
  )
  /*
  Routine Description:
    Sighting : CLONE from ivytown: HA Rd Error does not report when false poison pattern detected
  BIOS WA: For non-mirrored, poison enabled systems:
  set defeature MC0_DP_CHKN_BIT:ENA_ERR_WHEN_POISON and MC1_DP_CHKN_BIT:ENA_ERR_WHEN_POISON

Arguments:

    i/p : Socket ID and host structure
Returns:

    EFI_SUCCESS if the call is succeed


  */
{
  UINT8                                 ch;
  UINT8                                 tempCh;
  UINT32                                Offset;
  struct channelNvram                   (*channelNvList)[MAX_CH];
  MC0_DP_CHKN_BIT_MCDDC_DP_STRUCT       dpCkhnBit;

  channelNvList = GetChannelNvList(host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    tempCh = ch;
    Offset = MC0_DP_CHKN_BIT_MCDDC_DP_REG;
    if (ch & BIT0) {
      Offset = Offset + 4;
      tempCh = (ch & BIT1);
    }
    dpCkhnBit.Data = MemReadPciCfgEp (host, socket, tempCh, Offset);
    dpCkhnBit.Bits.ign_ptrl_uc = 1;
    MemWritePciCfgEp (host, socket, tempCh, Offset, dpCkhnBit.Data);
  } // ch loop

  return SUCCESS;
}

void
SetMirrorMode (
  PSYSHOST host,
  UINT8    socket
  )
/*++

Routine Description:

  Configure Mirror mode

Arguments:

  host  - Pointer to sysHost
  socket  - Socket number

Returns:

  N/A

--*/
{
  UINT8                         ch;
  UINT8                         mcId;
  RASENABLES_MC_MAINEXT_STRUCT  imcRASEnables;
  HASYSDEFEATURE_HA_CFG_STRUCT  haSYSDEFEATURE;
  HAFRCCHFAIL_HA_CFG_STRUCT     haFRCCHFAIL;
  HASYSDEFEATURE2_HA_CFG_HSX_BDX_STRUCT     haSYSDEFEATURE2;

  //
  // Configure Mirror mode
  // Note: mirror capability bit checked in checkSupportedRASModes() function
  //
  if ((host->nvram.mem.RASmode & CH_MIRROR) == CH_MIRROR) {
    // WA for 4168803: ISOC and Mirror/Lockstep cannot be enabled together
    if (host->var.qpi.OutIsocEn){
      host->nvram.mem.RASmode &= ~CH_ML;
      MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "ISOC enabled, mirror mode disabled\n"));
      return;
    }

    //
    // Make sure timings are syncronized accross lockstep pairs
    //
    SyncronizeTimings(host, socket);

    //
    // Program HA Mirror Enables
    // Mirror mod 0x1 is for ch 0/1 ch 2/3 mirroring
    //
    for (mcId = 0; mcId < MAX_IMC; mcId++) {
      if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

      //
      // Program iMC RAS Enables to enable Mirror mode
      //
      imcRASEnables.Data          = MemReadPciCfgMC (host, socket, mcId, RASENABLES_MC_MAINEXT_REG);
      imcRASEnables.Bits.mirroren = 1;
      MemWritePciCfgMC (host, socket, mcId, RASENABLES_MC_MAINEXT_REG, imcRASEnables.Data);

      haSYSDEFEATURE.Data           = MemReadPciCfgMC (host, socket, mcId, HASYSDEFEATURE_HA_CFG_REG);
      // Mirroring ch 0 & 1, and 2 & 3
      haSYSDEFEATURE.Bits.mirrormod = 0x1;
      MemWritePciCfgMC (host, socket, mcId, HASYSDEFEATURE_HA_CFG_REG, haSYSDEFEATURE.Data);

      //
      // MirrorScrub enable
      //
      haSYSDEFEATURE2.Data = MemReadPciCfgMC (host, socket, mcId, HASYSDEFEATURE2_HA_CFG_REG);
      haSYSDEFEATURE2.Bits.mirrorsrb = 1;
      MemWritePciCfgMC (host, socket, mcId, HASYSDEFEATURE2_HA_CFG_REG, haSYSDEFEATURE2.Data);

    } // mcId loop

    //
    // On S3 RESUME, set HA channel failover status from NVRAM
    // Note: S3 / Mirror is not POR and may not be fully validataed
    //
    if (host->var.common.bootMode == S3Resume || host->var.mem.subBootMode == WarmBootFast || host->var.mem.subBootMode == ColdBootFast) {
      for (ch = 0; ch < MAX_CH; ch++) {
        if (host->nvram.mem.socket[socket].channelList[ch].enabled == 0) continue;

        if (host->nvram.mem.socket[socket].channelList[ch].chFailed == 1) {
          mcId = GetMCID(host, socket, ch);
          haFRCCHFAIL.Data = MemReadPciCfgMC (host, socket, mcId, HAFRCCHFAIL_HA_CFG_REG);
          haFRCCHFAIL.Data |= (1 << (ch - (mcId * MAX_IMC)));
          MemWritePciCfgMC (host, socket, mcId, HAFRCCHFAIL_HA_CFG_REG, haFRCCHFAIL.Data);
        }
      }
    }
    MemWaDisableUCPatrol(host, socket);
    MemWaDisableUCPatrolPoisoning(host, socket);
    MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Mirror mode enabled\n"));
  }else {
    //
    // MirrorScrub disable
    //
    for (mcId = 0; mcId < MAX_IMC; mcId++) {
      if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

      haSYSDEFEATURE2.Data = MemReadPciCfgMC (host, socket, mcId, HASYSDEFEATURE2_HA_CFG_REG);
      haSYSDEFEATURE2.Bits.mirrorsrb = 0;
      MemWritePciCfgMC (host, socket, mcId, HASYSDEFEATURE2_HA_CFG_REG, haSYSDEFEATURE2.Data);
    }
  }

  return;
}

void
SetErrorThreshold (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT32   imcCORRERRTHRSHLD01Reg,
  UINT32   ErrThreshold
  )
/*++

Routine Description:

  Configure Spare error threshold

Arguments:

  host                    - Pointer to sysHost
  socket                    - Socket number
  ch                      - Channel number (0-based)
  imcCORRERRTHRSHLD01Reg  -
  ErrThreshold            -

Returns:

  N/A

--*/
{
  UINT8                               rank;
  CORRERRTHRSHLD_0_MCDDC_DP_STRUCT  imcCORRERRTHRSHLD01;

  if (host->nvram.mem.socket[socket].channelList[ch].enabled == 1) {
    //
    // Set Error Threshold
    //
    for (rank = 0; rank < MAX_RANK_CH; rank += 2) {
      imcCORRERRTHRSHLD01.Data              = MemReadPciCfgEp (host, socket, ch, imcCORRERRTHRSHLD01Reg + (rank * 2));
      //
      // first rank
      //
      imcCORRERRTHRSHLD01.Bits.cor_err_th_0 = ErrThreshold;
      //
      // second rank
      //
      imcCORRERRTHRSHLD01.Bits.cor_err_th_1 = ErrThreshold;
      MemWritePciCfgEp (host, socket, ch, imcCORRERRTHRSHLD01Reg + (rank * 2), imcCORRERRTHRSHLD01.Data);
    }
  }

  return;
}

void
S3UpdateRIR (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    oldSparePhysRank,
  UINT8    newSparePhysRank
  )
/*++

Routine Description:

  Update RIR with new physical rank id for spare

Arguments:

  host  - Pointer to sysHost
  socket  - Socket number
  ch    - Channel
  oldSparePhysRank - original physical rank id
  newSparePhysRank - new physical rank id of spare

Returns:

  N/A

--*/
{
   UINT8 rir;
   UINT8 rirway;
   struct ddrChannel *channelList;

   //
   // Search RIR interleave list and replace old physical rank id with new physical rank id
   //

   channelList = &host->var.mem.socket[socket].channelList[ch];

   //
   // For each RIR in the channel replace old rank target with new spare target in the interleave list
   // Note: the RIR offsets do not need to be updated
   //
   for (rir = 0; rir < MAX_RIR; rir++) {
     for (rirway = 0; rirway < MAX_RIR_WAYS; rirway++) {
       if (channelList->rirIntList[rir][rirway] == oldSparePhysRank) {
          channelList->rirIntList[rir][rirway] = newSparePhysRank;
       }
     }
   }

   WriteRIRForChannel (host, socket, ch);

   return;
}


void
S3SpareResume (
  PSYSHOST host,
  UINT8    socket
  )
/*++

Routine Description:

  Configure Sparing on S3 resume
  Note: S3 / Sparing is not POR and may not be fully validated

Arguments:

  host  - Pointer to sysHost
  socket  - Socket number

Returns:

  N/A

--*/
{
  return;
}


void
SetSpareMode (
  PSYSHOST host,
  UINT8    socket
  )
/*++

Routine Description:

  Configure Spare mode

Arguments:

  host  - Pointer to sysHost
  socket  - Socket number

Returns:

  N/A

--*/
{
  UINT8                                     ch,rankIndex, dimm, rank;
  UINT32                                    DimmAmap;
  UINT8                                     physicalRank,Dimm,RankPerDimm;
  struct channelNvram                       *channelNvList;
  struct dimmNvram                          (*dimmNvList)[MAX_DIMM];
  struct ddrRank                            (*rankList)[MAX_RANK_DIMM];
  SMISPARECTL_MC_MAINEXT_STRUCT             imcSMISPARECTL;
  SPAREINTERVAL_MC_MAINEXT_STRUCT           imcSPAREINTERVAL;
  SPARECTL_MC_MAINEXT_STRUCT                imcSpareCtlReg;
  SPARING_MCDDC_CTL_STRUCT                  imcSparingReg;
  AMAP_MCDDC_DESC_STRUCT                    imcAMAPReg;
  DIMMMTR_0_MCDDC_DESC_BDX_STRUCT           dimmMtr;
#ifdef SPARING_SCRATCHPAD_SUPPORT
  UINT32                                    ScratchPadValue;
  UINT8                                     logicalRank;
  UINT8                                     ChOnMc;
  UINT32                                    imcScratchPadRegOff;
  UINT8                                     LeftShiftBit;
  BOOLEAN                                   SingleHA;
  BOOLEAN                                   ChOnSecondHA;
  BIOSNONSTICKYSCRATCHPAD4_UBOX_MISC_STRUCT imcScratchPadReg;
  BIOSSCRATCHPAD5_UBOX_MISC_STRUCT          imcScratchPad5Reg;
#endif

  //
  // Configure Spare mode
  // Note: spare capability bit checked in checkSupportedRASModes() function
  //
  if ((host->nvram.mem.RASmode & RK_SPARE) == RK_SPARE) {
    //
    // Program iMC for sparing
    //
    imcSMISPARECTL.Data                 = MemReadPciCfgMain (host, socket, SMISPARECTL_MC_MAINEXT_REG);
    imcSMISPARECTL.Bits.intrpt_sel_cmci = 0;
    imcSMISPARECTL.Bits.intrpt_sel_smi  = 0;
    imcSMISPARECTL.Bits.intrpt_sel_pin  = 0;
    MemWritePciCfgMain (host, socket, SMISPARECTL_MC_MAINEXT_REG, imcSMISPARECTL.Data);

  /*
  Sighting 3612831
  For each socket, each iMC channel, program the following register
        s.uncore0.imc_c<num>_sparing.sparecrdts = 0x10

  For each socket, program the following register
        s.uncore0.imc_sparectl.hafifowm = 0x2
        s.uncore0.imc_sparectl.diswpqwm = 0x1
        s.uncore0.imc_spareinterval.numspare = 0x4
  */
    for (ch = 0; ch < MAX_CH; ch++) {
      imcSparingReg.Data = MemReadPciCfgEp (host, socket, ch, SPARING_MCDDC_CTL_REG);
      imcSparingReg.Bits.sparecrdts = 0x0F;
      MemWritePciCfgEp (host, socket, ch, SPARING_MCDDC_CTL_REG, imcSparingReg.Data);
      imcSparingReg.Data = MemReadPciCfgEp (host, socket, ch, SPARING_MCDDC_CTL_REG);
    } // ch loop

    imcSpareCtlReg.Data = MemReadPciCfgMain (host, socket, SPARECTL_MC_MAINEXT_REG);
    imcSpareCtlReg.Bits.hafifowm = 0x02;
    imcSpareCtlReg.Bits.diswpqwm = 0x01;
    MemWritePciCfgMain (host, socket, SPARECTL_MC_MAINEXT_REG, imcSpareCtlReg.Data);

    //
    // Set spare interval - to recommended value (from imc design)
    //
    imcSPAREINTERVAL.Data           = MemReadPciCfgMain (host, socket, SPAREINTERVAL_MC_MAINEXT_REG);
    imcSPAREINTERVAL.Bits.normopdur = host->setup.mem.normOppIntvl;
    // imcSPAREINTERVAL.Bits.numspare  = 20;
    // Please refer the above comments for sighting 3612831
    imcSPAREINTERVAL.Bits.numspare  = host->setup.mem.numSparTrans;
    MemWritePciCfgMain (host, socket, SPAREINTERVAL_MC_MAINEXT_REG, imcSPAREINTERVAL.Data);

    if (host->var.common.bootMode == S3Resume) {
      S3SpareResume (host, socket);
    }

    MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Sparing configured\n"));

    //
    //update AMAP register
    //
    if((host->nvram.mem.RASmodeEx & PTRLSCRB_EN) ){
      for(ch = 0; ch < MAX_CH; ch++){
        DimmAmap = 0;
        channelNvList = &host->nvram.mem.socket[socket].channelList[ch];
        dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
        if(channelNvList->enabled == 0) continue;

        for(rankIndex = 0; rankIndex < MAX_RANK_CH/2; rankIndex ++){
          physicalRank = channelNvList->sparePhysicalRank[rankIndex];
          if(physicalRank == 0xff) continue;
          Dimm = physicalRank /4;
          RankPerDimm  = physicalRank % 4;
          DimmAmap |= (1 << (Dimm * 8 + RankPerDimm));
        } // rankIndex loop

        //
        // Update AMAP register with mapped out ranks
        //
        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

          dimmMtr.Data = MemReadPciCfgEp (host, socket, ch, DIMMMTR_0_MCDDC_DESC_REG + (dimm * 4));

          for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
            rankList = &host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].rankList;

            // Disable Patrol Scrub when ranks are mapped out
            if ((*dimmNvList)[dimm].mapOut[rank]) {
              DimmAmap |= (1 << (dimm * 8 + rank));
            }
          }
        }
        MemDebugPrint ((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
              "DimmAmap: 0x%x\n", DimmAmap));

        imcAMAPReg.Data = MemReadPciCfgEp (host, socket, ch, AMAP_MCDDC_DESC_REG);
        imcAMAPReg.Bits.dimm0_pat_rank_disable = (UINT8)(DimmAmap & 0xff);
        imcAMAPReg.Bits.dimm1_pat_rank_disable = (UINT8)((DimmAmap >> 8) & 0xff);
        imcAMAPReg.Bits.dimm2_pat_rank_disable = (UINT8)((DimmAmap >> 16) & 0xff);
        MemWritePciCfgEp (host, socket, ch, AMAP_MCDDC_DESC_REG,imcAMAPReg.Data);
      }
    }

#ifdef SPARING_SCRATCHPAD_SUPPORT
    //
    //update scratch pad register
    //
    SingleHA = (Is1HA(host)) ? TRUE : FALSE;
    imcScratchPad5Reg.Data = ReadCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD5_UBOX_MISC_REG);

    for(ch = 0; ch < MAX_CH; ch++){
      channelNvList = &host->nvram.mem.socket[socket].channelList[ch];
      if(channelNvList->enabled == 0) continue;

      ScratchPadValue = 0;
      for(rankIndex = 0; rankIndex < MAX_RANK_CH/2; rankIndex ++){
        logicalRank = channelNvList->spareLogicalRank[rankIndex];

        if(logicalRank == 0xff) continue;

        ScratchPadValue |= (1 << logicalRank);
      } // rankIndex loop

      if(SingleHA){
        ChOnMc = ch;
        ChOnSecondHA = FALSE;
        imcScratchPadRegOff = BIOSNONSTICKYSCRATCHPAD4_UBOX_MISC_REG;
      } else {
        if(ch < 2){
          ChOnMc = ch;
          ChOnSecondHA = FALSE;
          imcScratchPadRegOff = BIOSNONSTICKYSCRATCHPAD4_UBOX_MISC_REG;
        } else {
          ChOnMc = ch - 2;
          ChOnSecondHA = TRUE;
          imcScratchPadRegOff = BIOSNONSTICKYSCRATCHPAD6_UBOX_MISC_REG;
        }
      }

      imcScratchPadReg.Data = ReadCpuPciCfgEx (host, socket, 0, imcScratchPadRegOff);
      imcScratchPadReg.Data &= (~(0xff << (ChOnMc * 8)));
      imcScratchPadReg.Data |= (ScratchPadValue << (ChOnMc * 8));
      WriteCpuPciCfgEx(host, socket, 0, imcScratchPadRegOff,imcScratchPadReg.Data);

      LeftShiftBit = 16 + ChOnMc;

      //
      // Channel on second MC is on bit20~23.
      //
      if((SingleHA == FALSE) && (ChOnSecondHA == TRUE)){
        LeftShiftBit = 16 + 4 + ChOnMc;
      }

      if(ScratchPadValue) imcScratchPad5Reg.Data |= (1 << LeftShiftBit);
      else imcScratchPad5Reg.Data &= (~(1 << LeftShiftBit));
    } // ch loop

    WriteCpuPciCfgEx(host, socket, 0, BIOSSCRATCHPAD5_UBOX_MISC_REG,imcScratchPad5Reg.Data);

    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Scratchpad5, 4, 6: %x %x %x\n",
                   imcScratchPad5Reg.Data,
                   ReadCpuPciCfgEx (host, socket, 0, BIOSNONSTICKYSCRATCHPAD4_UBOX_MISC_REG),
                   ReadCpuPciCfgEx (host, socket, 0, BIOSNONSTICKYSCRATCHPAD6_UBOX_MISC_REG)
                   ));
#endif
  }
} // SetSpareMode

void
SetDeviceTagging (
  PSYSHOST host,
  UINT8    socket
  )
/*++

Routine Description:

  Configure Device tagging

Arguments:

  host  - Pointer to sysHost
  socket  - Socket number

Returns:

  N/A

--*/
{
  INT8   ch;
  UINT8   dimm;
  UINT8   rank;
  UINT8   numRanks;
  UINT8   logicalRank;
  UINT8   devTagInfo;
  DEVTAG_CNTL_0_MCDDC_DP_STRUCT devTagReg;
  DIMMMTR_0_MCDDC_DESC_BDX_STRUCT dimmMtr;
  UINT32  orgDevWidth;

  //
  // Configure Device Tagging, if on S3 RESUME path
  //
  if (((host->nvram.mem.RASmodeEx & DEVTAGGING_EN) == DEVTAGGING_EN) &&
      (host->var.common.bootMode == S3Resume)){
    //
    // Program iMC for tagging
    //
    for (ch = 0; ch < MAX_CH; ch++) {
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].dimmPresent == 0) continue;

        numRanks = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].numRanks;

        for (rank = 0; rank < numRanks; rank++) {

          if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].mapOut[rank] == 1) continue;
          devTagInfo = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].rankList[rank].devTagInfo;

          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
          if (devTagInfo != 0) {
            //
            // Update dev tagging for logical rank
            // Note: if dev tagging is enabled h/w will update addr map info for RIR
            //
            logicalRank = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].rankList[rank].logicalRank;
            MemWritePciCfgEp (host, socket, ch, DEVTAG_CNTL_0_MCDDC_DP_REG + logicalRank, devTagInfo);
          } // if
        } // rank
      } // dimm
    } // ch
  } // if S3

  //
  // WA: BIOS sighting# 4988168 / Si sighting# 2492180
  //
  if ((host->var.common.cpuType == CPU_BDX) && host->nvram.mem.eccEn) {
    for (ch = (MAX_CH-1); ch >= 0 ; ch--) {
      if (host->nvram.mem.socket[socket].channelList[ch].enabled == 0 || host->nvram.mem.socket[socket].channelList[ch].maxDimm == 0) continue;

      devTagInfo = 0;
      //
      // Skip if User doesn't want to apply WA on this CH
      //
      if (!(host->setup.mem.ceccWaChMask & (1 << ch))) continue;

      //
      // Check if device tagging is enabled on any dimm/rank in the ch
      //
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].dimmPresent == 0) continue;

        numRanks = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].numRanks;

        for (rank = 0; rank < numRanks; rank++) {

          if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].mapOut[rank] == 1) continue;
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
          logicalRank = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].rankList[rank].logicalRank;
          devTagReg.Data = (UINT8) MemReadPciCfgEp (host, socket, ch, DEVTAG_CNTL_0_MCDDC_DP_REG + logicalRank);
          devTagInfo |= devTagReg.Bits.en;
        } // rank
      } // dimm

      //
      // If device tagging is already enabled on any rank in CH, WA need not be applied; skip to next ch
      // Else, apply WA
      //
      if (devTagInfo != 0) {
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                              "SDDC already enabled on CH; Skip CECC WA\n"));
        continue;
      }

      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "Apply CECC WA\n"));

      if ((host->nvram.mem.RASmode & CH_LOCKSTEP) && ((ch & BIT0)== 0)) continue; // Skip Odd Channels in Lockstep mode.(following code)
      //
      // If DIMM 0 is not a x4, force x4 temporarily
      //
      dimmMtr.Data = MemReadPciCfgEp (host, socket, ch, DIMMMTR_0_MCDDC_DESC_REG);
      orgDevWidth = dimmMtr.Bits.ddr3_width;
      if (!host->nvram.mem.socket[socket].channelList[ch].dimmList[0].x4Present) {
        MemDebugPrint((host, SDBG_MAX, socket, ch, 0, NO_RANK, NO_STROBE, NO_BIT,
                              "CECC WA - DIMM 0 is not x4, forcing x4\n"));
        dimmMtr.Bits.ddr3_width = 0;
        MemWritePciCfgEp (host, socket, ch, DIMMMTR_0_MCDDC_DESC_REG, dimmMtr.Data);
      }
      if (host->nvram.mem.RASmode & CH_LOCKSTEP) {
        dimmMtr.Data = MemReadPciCfgEp (host, socket, ch - 1, DIMMMTR_0_MCDDC_DESC_REG);
        orgDevWidth = dimmMtr.Bits.ddr3_width;
        if (!host->nvram.mem.socket[socket].channelList[ch - 1].dimmList[0].x4Present) {
          MemDebugPrint((host, SDBG_MAX, socket, ch - 1, 0, NO_RANK, NO_STROBE, NO_BIT,
                        "CECC WA - DIMM 0 is not x4, forcing x4\n"));
          dimmMtr.Bits.ddr3_width = 0;
          MemWritePciCfgEp (host, socket, ch - 1, DIMMMTR_0_MCDDC_DESC_REG, dimmMtr.Data);
        }
      }

      devTagReg.Data = (UINT8)MemReadPciCfgEp (host, socket, ch, DEVTAG_CNTL_0_MCDDC_DP_REG);
      devTagReg.Bits.faildevice = 0;
      devTagReg.Bits.en = 1;
      MemWritePciCfgEp (host, socket, ch, DEVTAG_CNTL_0_MCDDC_DP_REG, devTagReg.Data);
      if (host->nvram.mem.RASmode & CH_LOCKSTEP) {
        devTagReg.Data = (UINT8)MemReadPciCfgEp (host, socket, ch - 1, DEVTAG_CNTL_0_MCDDC_DP_REG);
        devTagReg.Bits.faildevice = 0;
        devTagReg.Bits.en = 1;
        MemWritePciCfgEp (host, socket, ch - 1, DEVTAG_CNTL_0_MCDDC_DP_REG, devTagReg.Data);
      }

      devTagReg.Bits.faildevice = 63;
      devTagReg.Bits.en = 0;
      MemWritePciCfgEp (host, socket, ch, DEVTAG_CNTL_0_MCDDC_DP_REG, devTagReg.Data);
      if (host->nvram.mem.RASmode & CH_LOCKSTEP) {
        devTagReg.Bits.faildevice = 63;
        devTagReg.Bits.en = 0;
        MemWritePciCfgEp (host, socket, ch - 1, DEVTAG_CNTL_0_MCDDC_DP_REG, devTagReg.Data);
      }
      //
      // If DIMM 0 is not a x4, restore the original device width
      //
      if (!host->nvram.mem.socket[socket].channelList[ch].dimmList[0].x4Present) {
        MemDebugPrint((host, SDBG_MAX, socket, ch, 0, NO_RANK, NO_STROBE, NO_BIT,
            "CECC WA - DIMM 0 is not x4, restoring orig width\n"));
        dimmMtr.Bits.ddr3_width = orgDevWidth;
        MemWritePciCfgEp (host, socket, ch, DIMMMTR_0_MCDDC_DESC_REG, dimmMtr.Data);
      }
      if (host->nvram.mem.RASmode & CH_LOCKSTEP) {
        if (!host->nvram.mem.socket[socket].channelList[ch - 1].dimmList[0].x4Present) {
          MemDebugPrint((host, SDBG_MAX, socket, ch - 1, 0, NO_RANK, NO_STROBE, NO_BIT,
                         "CECC WA - DIMM 0 is not x4, restoring orig width\n"));
          dimmMtr.Bits.ddr3_width = orgDevWidth;
          MemWritePciCfgEp (host, socket, ch - 1, DIMMMTR_0_MCDDC_DESC_REG, dimmMtr.Data);
        }
      }
    } //ch
  }
  return;
}


VOID
SetErrorThresholdNode (
  PSYSHOST host,
  UINT8    socket
  )
/*++

Routine Description:

  Program corrected error thresholds for all channels in socket

Arguments:

  host  - Pointer to sysHost
  socket  - Socket number

Returns:

  N/A

--*/
{
  UINT8                 ch;
  UINT32                ErrThreshold;

    //
    // Set Error Threshold
    //
    ErrThreshold = host->setup.mem.spareErrTh;

    if (ErrThreshold != 0) {
      //
      // Channel 0 - 3
      //
    for (ch = 0; ch < MAX_CH; ch++) {
      SetErrorThreshold (host, socket, ch, CORRERRTHRSHLD_0_MCDDC_DP_REG, ErrThreshold);
    }
  }
  return;
}


VOID
SetHACreditCount (
  PSYSHOST host,
  UINT8    socket,
  UINT8    mcRdCredits,
  UINT8    mcWrCredits
  )
/*++

Routine Description:

  Configure HA credit count
  Note: part 1 of HA credit count config is done in qpirc

Arguments:

  host      - Pointer to sysHost
  socket    - Socket number
  mcCredits - MC credit count

Returns:

  N/A

--*/
{
  UINT8 mcId;

  for (mcId = 0; mcId < MAX_IMC; mcId++) {
    if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

    // Allow all credits to drain
    MemWritePciCfgMC (host, socket, mcId, HACRDTCNT_HA_CFG_REG, 0x00030000);
    // Clear SharCrdtEn
    MemWritePciCfgMC (host, socket, mcId, HACRDTCNT_HA_CFG_REG, 0x00010000);
    // Set read credits
    MemWritePciCfgMC (host, socket, mcId, HACRDTCNT_HA_CFG_REG, 0x00052000 + mcRdCredits);
    MemWritePciCfgMC (host, socket, mcId, HACRDTCNT_HA_CFG_REG, 0x00052100 + mcRdCredits);
    MemWritePciCfgMC (host, socket, mcId, HACRDTCNT_HA_CFG_REG, 0x00052200 + mcRdCredits);
    MemWritePciCfgMC (host, socket, mcId, HACRDTCNT_HA_CFG_REG, 0x00052300 + mcRdCredits);
    // Set write credits
    MemWritePciCfgMC (host, socket, mcId, HACRDTCNT_HA_CFG_REG, 0x00052400 + mcWrCredits);
    MemWritePciCfgMC (host, socket, mcId, HACRDTCNT_HA_CFG_REG, 0x00052500 + mcWrCredits);
    MemWritePciCfgMC (host, socket, mcId, HACRDTCNT_HA_CFG_REG, 0x00052600 + mcWrCredits);
    MemWritePciCfgMC (host, socket, mcId, HACRDTCNT_HA_CFG_REG, 0x00052700 + mcWrCredits);
    // Enable shared creit pools and shared credit prefetching
    MemWritePciCfgMC (host, socket, mcId, HACRDTCNT_HA_CFG_REG, 0x00020000);
  } // mcId

  return;
}

VOID
SetLeakyBucketConfig (
  PSYSHOST host,
  UINT8    socket
  )
/*++

Routine Description:

  Configure Leaky Bucket

Arguments:

  host  - Pointer to sysHost
  socket  - Socket number

Returns:

  N/A

--*/
{
  LEAKY_BUCKET_CFG_MC_MAINEXT_STRUCT  imcLEAKY_BUCKET_CFG;
  struct channelNvram                 (*channelNvList)[MAX_CH];
  UINT8                               ch;
  UINT8                               mcId;

  //
  // Configure Leaky Bucket
  // Leaky bucket config must always be set to non-zero by MRC
  //

  if (host->var.common.bootMode != S3Resume) {
  //
  // Program leaky bucket rate
  // Set to a rate of about 1 leaky bucket pulse per 10 days at a dclk rate of 1GHz
  //
  imcLEAKY_BUCKET_CFG.Data                  = MemReadPciCfgMain (host, socket, LEAKY_BUCKET_CFG_MC_MAINEXT_REG);
  imcLEAKY_BUCKET_CFG.Bits.leaky_bkt_cfg_hi = host->setup.mem.leakyBktHi;
  imcLEAKY_BUCKET_CFG.Bits.leaky_bkt_cfg_lo = host->setup.mem.leakyBktLo;
  MemWritePciCfgMain (host, socket, LEAKY_BUCKET_CFG_MC_MAINEXT_REG, imcLEAKY_BUCKET_CFG.Data);
  } else {
    //
    // S3Resume.
    //
    for (mcId = 0; mcId < MAX_IMC; mcId++) {
      if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

      //
      // Initialize leaky bucket rate to be very slow to allow setting resume values.
      //
      imcLEAKY_BUCKET_CFG.Data                  = MemReadPciCfgMC (host, socket, mcId, LEAKY_BUCKET_CFG_MC_MAINEXT_REG);
      imcLEAKY_BUCKET_CFG.Bits.leaky_bkt_cfg_hi = 0x29; // Bit 52 of the full 53b counter.
      imcLEAKY_BUCKET_CFG.Bits.leaky_bkt_cfg_lo = 0x28; // Bit 51 of the full 53b counter.
      MemWritePciCfgMC (host, socket, mcId, LEAKY_BUCKET_CFG_MC_MAINEXT_REG, imcLEAKY_BUCKET_CFG.Data);
    }

    channelNvList = GetChannelNvList(host, socket);
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      //
      // Restore Leaky Bucket 2nd counter value.
      //
      MemWritePciCfgEp (host, socket, ch, LEAKY_BKT_2ND_CNTR_REG_MCDDC_DP_REG, host->nvram.mem.socket[socket].channelList[ch].leakyBucket2ndCounter);
    }

    //
    // Restore leaky bucket counter and rate.
    //
    for (mcId = 0; mcId < MAX_IMC; mcId++) {
      if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

      MemWritePciCfgMC (host, socket, mcId, LEAKY_BUCKET_CNTR_HI_MC_MAINEXT_REG, host->nvram.mem.socket[socket].imc[mcId].leakyBucketCounterHi);
      MemWritePciCfgMC (host, socket, mcId, LEAKY_BUCKET_CNTR_LO_MC_MAINEXT_REG, host->nvram.mem.socket[socket].imc[mcId].leakyBucketCounterLo);
      MemWritePciCfgMC (host, socket, mcId, LEAKY_BUCKET_CFG_MC_MAINEXT_REG,     host->nvram.mem.socket[socket].imc[mcId].leakyBucketCfg);
    }
  }

  return;
}

const UINT8 timingTable[MAX_SUP_FREQ] = {20, 17, 15, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 2, 2, 2, 2};

UINT32
SetRASConfig (
  PSYSHOST host
  )
/*++

Routine Description:

  Enable RAS modes
  NOTE: Default settings are unchanged if feature not explicitly enabled by BIOS

Arguments:

  host - Pointer to sysHost

Returns:

  SUCCESS

--*/
{
  UINT8                                 socket;
  UINT8                                 ch;
  UINT8                                 mcRdCredits;
  UINT8                                 mcWrCredits;
  UINT8                                 mcId;
  UINT8                                 StartIndex;
  UINT16                                memSize;
  UINT32                                scrubInterval;
  UINT32                                scrubFactor;
  UINT8                                 tempCh;
  UINT32                                Offset;
  struct channelNvram                   (*channelNvList)[MAX_CH];
  struct ddrChannel                     (*channelList)[MAX_CH];
  SCRUBCTL_MC_MAINEXT_STRUCT            imcSCRUBCTL;
  MC0_DP_CHKN_BIT_MCDDC_DP_STRUCT       dpCkhnBit;
  HASYSDEFEATURE2_HA_CFG_HSX_BDX_STRUCT haSYSDEFEATURE2;
  MCMAIN_CHKN_BITS_MC_MAIN_STRUCT       mcMainChknBits;
  SCRUBADDRESSHI_MC_MAINEXT_STRUCT      scrubAddrHi;
  UINT8                                 i;

  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "Set RAS Config\n"));

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) {
      continue;
    }

    channelNvList = GetChannelNvList(host, socket);

    //
    // Default values
    //
    mcRdCredits = 48;
    mcWrCredits = 40;


    OutputExtendedCheckpoint((host, STS_RAS_CONFIG, SUB_MIRROR_MODE, socket));
    SetMirrorMode (host, socket);
    OutputExtendedCheckpoint((host, STS_RAS_CONFIG, SUB_SPARE_MODE, socket));
    SetSpareMode (host, socket);
    OutputExtendedCheckpoint((host, STS_RAS_CONFIG, SUB_DEVICE_TAG, socket));
    SetDeviceTagging (host, socket);
    OutputExtendedCheckpoint((host, STS_RAS_CONFIG, SUB_ERR_THRESH, socket));
    SetErrorThresholdNode (host, socket);

    if ((host->nvram.mem.RASmode & CH_ML) == CH_INDEPENDENT) {
      MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Independent ch mode enabled\n"));
    }
    //
    // Enable patrol scrubs
    // Note: Patrol scrub capability bit checked in checkSupportedRASModes() function
    //

    if ((host->nvram.mem.RASmodeEx & PTRLSCRB_EN) && (host->var.mem.socket[socket].memSize != 0)) {

      //
      // Save one write credit for patrol scrub
      //
      mcWrCredits -= 1;

      for (mcId = 0; mcId < MAX_IMC; mcId++) {
        if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

        if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX) == 0) {
          mcMainChknBits.Data = MemReadPciCfgMC (host, socket, mcId, MCMAIN_CHKN_BITS_MC_MAIN_REG);
          mcMainChknBits.Bits.frc_spr_end = 1;
          MemWritePciCfgMC (host, socket, mcId, MCMAIN_CHKN_BITS_MC_MAIN_REG, mcMainChknBits.Data);
        }

        imcSCRUBCTL.Data = MemReadPciCfgMC (host, socket, mcId, SCRUBCTL_MC_MAINEXT_REG);

        if (host->var.common.bootMode == S3Resume) {
          scrubAddrHi.Data = host->nvram.mem.socket[socket].imc[mcId].scrubAddrHi;
          //
          // Make sure the channel is enabled
          //
          StartIndex = mcId * 2;
          while ((*channelNvList)[StartIndex + scrubAddrHi.Bits.chnl].enabled == 0) {
            scrubAddrHi.Bits.chnl = scrubAddrHi.Bits.chnl + 1;
            host->nvram.mem.socket[socket].imc[mcId].scrubAddrHi = scrubAddrHi.Data;
          }
          MemWritePciCfgMC (host, socket, mcId, SCRUBADDRESSLO_MC_MAINEXT_REG, host->nvram.mem.socket[socket].imc[mcId].scrubAddrLo);
          MemWritePciCfgMC (host, socket, mcId, SCRUBADDRESSHI_MC_MAINEXT_REG, host->nvram.mem.socket[socket].imc[mcId].scrubAddrHi);

          MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "mcId = %d - Scrub address = 0x%x:%08x \n", mcId, host->nvram.mem.socket[socket].imc[mcId].scrubAddrHi, host->nvram.mem.socket[socket].imc[mcId].scrubAddrLo));
        }

        //
        // Equation to get a scrub to every line in 24 hours is..
        // (86400/(MEMORY CAPACITY/64))/CYCLE TIME of DCLK
        // Seconds it takes to scrub all of memory (86400 seconds in a day)
        //
        if ((host->setup.mem.patrolScrubDuration != 0) && (host->setup.mem.patrolScrubDuration <= PATROL_SCRUB_DURATION_MAX)) {
          scrubFactor = 3600 * host->setup.mem.patrolScrubDuration;
        } else {
          scrubFactor = 86400;
        }

        channelList = &host->var.mem.socket[socket].channelList;
        memSize = 0;
        for(ch = 0; ch < MAX_CH; ch++) {
          if ((*channelNvList)[ch].enabled == 0) continue;

          if ((*channelList)[ch].mcId == mcId) {
            memSize += (*channelList)[ch].memSize;
          }
        } // ch loop

        if (memSize) {
          if ((host->var.common.cpuType == CPU_BDX) && (memSize >= 32)) {
            //
            // Scrub interval Calculation:
            // Scrubinterval = (((ScrubPeriod_seconds*32)/1000)*(DclkFreq_MHz/10))/(TotalMem_GBytes/2);
            // Scrubinterval = (Scrubinterval*1000/1024);
            // Scrubinterval = (Scrubinterval*1000/1024);
            // Scrubinterval = (Scrubinterval*1000/1024);
            // memSize is in 64MB units
            //
            scrubInterval = (((scrubFactor * 32) / 1000) *(host->nvram.mem.socket[socket].ddrFreqMHz / 10))/(memSize/32);
            scrubInterval = (scrubInterval * 1000)/1024;
            scrubInterval = (scrubInterval * 1000)/1024;
            scrubInterval = (scrubInterval * 1000)/1024;
          } else {
            scrubInterval = ((scrubFactor / ((memSize) * 10)) * 95367) / ((timingTable[host->nvram.mem.socket[socket].ddrFreq] * 100) / 8);
          }

          if (scrubInterval < PATROL_SCRUB_INTERVAL_MIN) {
            scrubInterval = PATROL_SCRUB_INTERVAL_MIN;
          }

        } else {
          scrubInterval = 0;
        }

        imcSCRUBCTL.Bits.scrubinterval  = scrubInterval;

        if (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG))) {
          imcSCRUBCTL.Bits.scrub_en       = 1;
        } else {
          imcSCRUBCTL.Bits.scrub_en       = 0;
        }
        if (host->var.common.bootMode == S3Resume) {
          //
          // Only set startscrub if resuming from S3
          //
          imcSCRUBCTL.Bits.startscrub   = 1;
        }

        MemWritePciCfgMC (host, socket, mcId, SCRUBCTL_MC_MAINEXT_REG, imcSCRUBCTL.Data);

        MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "Patrol scrub enabled and started\n"));
      } // mcId loop
    } // if patrol scrubbing enabled

    //
    // Enable/Disable Demand Scrubs
    // Note: Hardware default is demand scrub disabled
    //
    if (host->nvram.mem.RASmodeEx & DMNDSCRB_EN) {
      MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Demand scrub enabled\n"));
    } else {
      MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Demand scrub disabled\n"));
    }

    for (mcId = 0; mcId < MAX_IMC; mcId++) {
      if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

      haSYSDEFEATURE2.Data = MemReadPciCfgMC (host, socket, mcId, HASYSDEFEATURE2_HA_CFG_REG);
      if (host->nvram.mem.RASmodeEx & DMNDSCRB_EN) {
        haSYSDEFEATURE2.Bits.demandsrb = 1;
      } else {
        haSYSDEFEATURE2.Bits.demandsrb = 0;
      }
      MemWritePciCfgMC (host, socket, mcId, HASYSDEFEATURE2_HA_CFG_REG, haSYSDEFEATURE2.Data);

      //
      // Clear HAFRCCHFAIL_HA_CFG_REG
      //
      MemWritePciCfgMC (host, socket, mcId, HAFRCCHFAIL_HA_CFG_REG, 0);
    } // mcId

    //
    // Program HA credit count (part 1 done in qpirc / part 2 in mrc)
    //
    SetHACreditCount (host, socket, mcRdCredits, mcWrCredits);

    //
    // Set leaky bucket config to non-zero, per architect request
    //
    SetLeakyBucketConfig (host, socket);

    //
    // Convert uncorrectable patrol scrub errors to correctable if mirroring is enabled
    //
    if (((host->nvram.mem.RASmode & CH_MIRROR) == CH_MIRROR) &&
        (host->nvram.mem.RASmodeEx & PTRLSCRB_EN)) {
      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        tempCh = ch;
        Offset = MC0_DP_CHKN_BIT_MCDDC_DP_REG;
        if (ch & BIT0) {
          Offset = Offset + 4;
          tempCh = (ch & BIT1);
        }
        dpCkhnBit.Data = MemReadPciCfgEp (host, socket, tempCh, Offset);
        dpCkhnBit.Bits.ign_ptrl_uc = 1;
        MemWritePciCfgEp (host, socket, tempCh, Offset, dpCkhnBit.Data);
      } // ch loop
    } // if mirroring and patrol scrubbing enabled

    if (host->var.common.bootMode == S3Resume) {
      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        //
        // Per rank error counts and thresholds
        //
        for (i = 0; i < MAX_RANK_CH/2; i++) {
          //
          // Restore the error counters.
          //
          MemWritePciCfgEp (host, socket, ch, CORRERRCNT_0_MCDDC_DP_REG + i*4, host->nvram.mem.socket[socket].channelList[ch].rankErrCountInfo[i]);
          //
          // Restore the error thresholds.
          //
          MemWritePciCfgEp (host, socket, ch, CORRERRTHRSHLD_0_MCDDC_DP_REG + i*4, host->nvram.mem.socket[socket].channelList[ch].rankErrThresholdInfo[i]);
        } // i
      } // ch loop

      //
      // Restore SMI settings.
      //
      WriteCpuPciCfgEx (host, socket, 0, SMICTRL_UBOX_MISC_REG, host->nvram.mem.socket[socket].smiCtrlUboxMisc);
      for (mcId = 0; mcId < MAX_IMC; mcId++) {
        if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

        MemWritePciCfgMC (host, socket, mcId, EMCALTCTL_MC_MAINEXT_REG, host->nvram.mem.socket[socket].imc[mcId].EmcaLtCtlMcMainExt);
        MemWritePciCfgMC (host, socket, mcId, EXRAS_CONFIG_HA_CFG_REG, host->nvram.mem.socket[socket].imc[mcId].ExRasConfigHaCfg);
        MemWritePciCfgMC (host, socket, mcId, SMISPARECTL_MC_MAINEXT_REG, host->nvram.mem.socket[socket].imc[mcId].SmiSpareCtlMcMainExt);
      } // mcId
    }
  } // socket loop

  if (host->nvram.mem.eccEn != 0) {
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "ECC is enabled\n"));
  }

  return SUCCESS;
}

UINT8
CheckMLCapable (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch0,
  UINT8    ch1
  )
/*++

Routine Description:

  Check if a pair of channels are capable for mirroring/lockstep by checking if
  occupied DIMMs match on specified channels

Arguments:

  host  - Pointer to sysHost
  socket  - Socket number
  ch0   - Channel number for first channel to compare
  ch1   - Channel number for first channel to compare

Returns:

  RAS_IS_CAPABLE or RAS_NOT_CAPABLE

--*/
{
  UINT8                 dimm;
  UINT8                 rank;
  UINT8                 capable;
  struct socketNvram *socketNv;
  struct ddrRank (*rankList0)[MAX_RANK_DIMM];
  struct ddrRank (*rankList1)[MAX_RANK_DIMM];
  struct dimmNvram (*dimmNvList0)[MAX_DIMM];
  struct dimmNvram (*dimmNvList1)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];

  capable = RAS_IS_CAPABLE;

  socketNv  = &host->nvram.mem.socket[socket];

  channelNvList = &socketNv->channelList;

  //
  // If both channel ch0 and ch1 not enabled then skip channel pair for determining system RAS capability
  //
  if ((*channelNvList)[ch0].enabled == 0 && (*channelNvList)[ch1].enabled == 0) {
    capable = RAS_SKIP_CHANNEL;
  } else if ((*channelNvList)[ch0].enabled == 0 || (*channelNvList)[ch1].enabled == 0) {
    capable = RAS_NOT_CAPABLE;
  } else if (host->var.qpi.OutIsocEn) {
    // WA for 4168803: ISOC and Mirror/Lockstep cannot be enabled together
    capable = RAS_NOT_CAPABLE;
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "ISOC enabled, lockstep and mirroring not supported\n"));
  } else {
    //
    // Compare each pair of DIMMs to ensure not present or match
    //
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      rankList0   = &socketNv->channelList[ch0].dimmList[dimm].rankList;
      rankList1   = &socketNv->channelList[ch1].dimmList[dimm].rankList;
      dimmNvList0 = &socketNv->channelList[ch0].dimmList;
      dimmNvList1 = &socketNv->channelList[ch1].dimmList;

      //
      // Ensure DIMMs in both channels both present or both not present
      //
      if (((*dimmNvList0)[dimm].dimmPresent ^ (*dimmNvList1)[dimm].dimmPresent) != 0) {
        capable = RAS_NOT_CAPABLE;
        break;
      }

      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        if ((CheckRank(host, socket, ch0, dimm, rank, DONT_CHECK_MAPOUT)) || (CheckRank(host, socket, ch1, dimm, rank, DONT_CHECK_MAPOUT)))continue;

        //
        // Ensure ranks in both channels both mapped out or not mapped out
        //
        if (((*dimmNvList0)[dimm].mapOut[rank] ^ (*dimmNvList1)[dimm].mapOut[rank]) != 0) {
          capable = RAS_NOT_CAPABLE;
          break;
        }
      } // rank loop

      if (capable == RAS_NOT_CAPABLE) break;

      //
      // Compare number of ranks and tech index across channels
      //
      if (((*dimmNvList0)[dimm].techIndex == (*dimmNvList1)[dimm].techIndex) &&
          ((*dimmNvList0)[dimm].numRanks == (*dimmNvList1)[dimm].numRanks)
          ) {

        capable = RAS_IS_CAPABLE;

      } else {

        capable = RAS_CH_MIX_INV;
        break;
      } // if
    } // dimm loop
  }

  return capable;
}

UINT8
CheckSparingCapable (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch0
  )
/*++

Routine Description:

  Check if channel supports sparing by checking if occupied channel has a spare rank

Arguments:

  host    - Pointer to sysHost
  socket  - Socket number
  ch0     - Channel number to check sparing support

Returns:

  RAS_IS_CAPABLE or RAS_NOT_CAPABLE

--*/
{

  UINT8                 dimm;
  UINT8                 rank;
  UINT8                 capable;
  UINT8                 numranks;
  UINT8                 numdimms;
  struct socketNvram    *socketNv;
  struct dimmNvram (*dimmNvList0)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];

  socketNv  = &host->nvram.mem.socket[socket];
  channelNvList = &socketNv->channelList;
  capable = RAS_IS_CAPABLE;

  //
  // If channel not enabled, ignore channel for determining system RAS capability
  //
  if ((*channelNvList)[ch0].enabled == 0) {
    capable = RAS_SKIP_CHANNEL;
  } else {

    dimmNvList0 = &socketNv->channelList[ch0].dimmList;
    numranks    = 0;
    numdimms    = 0;

    //
    // Count number of ranks in channel
    //
    for (dimm = 0; dimm < (*channelNvList)[ch0].maxDimm; dimm++) {

      if (((*dimmNvList0)[dimm].dimmPresent) == 0) continue;

      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        if (CheckRank(host, socket, ch0, dimm, rank, CHECK_MAPOUT)) continue;

        numranks++;

      } // rank loop

      if(numranks) numdimms++;
    } // dimm loop
    //
    // DIMM loop
    // Note HSD 3611726 requires sparing not enabled if
    // number of dimms = 1 and number of ranks <= 2
    //
    if ((numdimms == 1) && (numranks < 2)) {
      capable = RAS_NOT_CAPABLE;
    }

  }

  return capable;
}

UINT8
checkSupportedRASModes (
  PSYSHOST host
  )
/*++

Routine Description:

  Determine if RAS features are supportabled given current memory configuration
  and user input

Arguments:

  host  - Pointer to sysHost

Returns:

  RAS_IS_CAPABLE or RAS_NOT_CAPABLE

--*/
{
  UINT8                     socket;
  UINT8                     dimm;
  UINT8                     ch;
  UINT8                     mode;
  UINT8                     notOnlyX4Dimms;
  CAPID1_PCU_FUN3_HSX_BDX_STRUCT  pcuCAPID1;
  CAPID3_PCU_FUN3_BDX_STRUCT      pcuCAPID3;

  struct channelNvram       (*channelNvList)[MAX_CH];
  struct dimmNvram          (*dimmNvList)[MAX_DIMM];

  mode = CH_INDEPENDENT;
  host->var.mem.RASModesEx = 0;
  //
  // Get info on imc RAS capability.
  //
  host->var.mem.RASCapable = imcRasCapable(host);

  if (host->var.mem.RASCapable) {
    //
    // Assume max capablity and reduce if necessary
    //
    mode = CH_MIRROR | CH_LOCKSTEP;

    if ( (host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot) ) {
      host->var.mem.RASModesEx = PTRLSCRB_EN | DMNDSCRB_EN | DEVTAGGING_EN | A7_MODE_EN;
    }else{
      host->var.mem.RASModesEx = host->nvram.mem.RASmodeEx;
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                            "RASModesEx = %x \n", host->nvram.mem.RASmodeEx));
    }

    //
    // Check for correctly configured DIMMs on each socket
    //
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) {
        continue;
      }
      notOnlyX4Dimms = 0;
      //
      // Lockstep capability bit from PCU
      //
      pcuCAPID3.Data = host->var.common.procCom[socket].capid3;

      //
      // Check if channel dimm config supports lockstep
      //
      if ((pcuCAPID3.Bits.disable_lockstep == 1) ||
          (CheckMLCapable (host, socket, 0, 1) == RAS_NOT_CAPABLE) || (CheckMLCapable (host, socket, 0, 1) == RAS_CH_MIX_INV) ||
          (CheckMLCapable (host, socket, 2, 3) == RAS_NOT_CAPABLE) || (CheckMLCapable (host, socket, 2, 3) == RAS_CH_MIX_INV)
          ) {
        mode &= ~CH_LOCKSTEP;
        if (((CheckMLCapable (host, socket, 0, 1) == RAS_CH_MIX_INV) ||
            (CheckMLCapable (host, socket, 2, 3) == RAS_CH_MIX_INV)) &&
            (host->setup.mem.RASmode & CH_LOCKSTEP)
           ) {
          LogWarning (host, WARN_LOCKSTEP_DISABLE, WARN_LOCKSTEP_DISABLE_MINOR_MISMATCHED, (UINT32) -1);
        }
      } else {
        channelNvList = &host->nvram.mem.socket[socket].channelList;
        for (ch = 0; ch < MAX_CH; ch++) {
          if ((*channelNvList)[ch].enabled == 0) continue;

          dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

          for (dimm = 0; dimm < MAX_DIMM; dimm++) {
            if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

            if (!(*dimmNvList)[dimm].x4Present) {
              notOnlyX4Dimms++;
            } //if !x4Present
          } //for dimm
        } //for ch
      } // if disable_lockstep
      if ((host->setup.mem.lockstepEnableX4 == LOCKSTEPX4_DISABLE) && (notOnlyX4Dimms == 0)) {
        mode &= ~CH_LOCKSTEP;
        MemDebugPrint ((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "Lockstep disabled, x4 DIMMs detected\n"));
      }
      //
      // Mirror capability bit from PCU
      //
      pcuCAPID1.Data = host->var.common.procCom[socket].capid1;

      //
      // Check if channel dimm config supports mirror
      //
      if ((pcuCAPID1.Bits.dis_mem_mirror == 1) ||
          (CheckMLCapable (host, socket, 0, 1) == RAS_NOT_CAPABLE) || (CheckMLCapable (host, socket, 0, 1) == RAS_CH_MIX_INV) ||
          (CheckMLCapable (host, socket, 2, 3) == RAS_NOT_CAPABLE) || (CheckMLCapable (host, socket, 2, 3) == RAS_CH_MIX_INV)
          ) {
        mode &= ~CH_MIRROR;
        if (((CheckMLCapable (host, socket, 0, 1) == RAS_CH_MIX_INV) ||
             (CheckMLCapable (host, socket, 2, 3) == RAS_CH_MIX_INV)) &&
            (host->setup.mem.RASmode & CH_MIRROR)
           ) {
          LogWarning (host, WARN_MIRROR_DISABLE, WARN_MIRROR_DISABLE_MINOR_MISMATCH, (UINT32) -1);
        }
      }
      //
      // Check for sparing support: each populated channel must have 2 spare ranks available
      // Spare capability bit from PCU
      //
      pcuCAPID3.Data = host->var.common.procCom[socket].capid3;

      //
      // enable sparing if any channel can support.
      //
      if ((pcuCAPID3.Bits.disable_sparing == 0) &&
          ((CheckSparingCapable (host, socket, 0) == RAS_IS_CAPABLE) ||
          (CheckSparingCapable (host, socket, 1) == RAS_IS_CAPABLE) ||
          (CheckSparingCapable (host, socket, 2) == RAS_IS_CAPABLE) ||
          (CheckSparingCapable (host, socket, 3) == RAS_IS_CAPABLE))
          ) {
        mode |= RK_SPARE;
      }

      //
      // Disable patrol scrub if any socket does not support patrol scrub
      // Check patrol scrub capability bit from PCU
      //
      if (pcuCAPID3.Bits.disable_patrol_scrub == 1) {
        host->var.mem.RASModesEx &= ~PTRLSCRB_EN;
      }

      //
      // Disable patrol scrub & Disable demand scrub if ECC is not supported
      //
      if (host->nvram.mem.eccEn == 0){
        host->var.mem.RASModesEx &= ~PTRLSCRB_EN;
        host->var.mem.RASModesEx &= ~DMNDSCRB_EN;
      }
    } // socket loop
  } // RASCapable

  return mode;
}

UINT8
imcRasCapable (
  PSYSHOST host
  )
/*++

Routine Description:

  Check if the imc is RAS capable

Arguments:

  host  - Pointer to sysHost

Returns:

  SUCCESS

--*/
{
  //  return (host->nvram.mem.eccEn == 1);
  return 1;
}

