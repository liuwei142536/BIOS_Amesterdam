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
 *      IMC and DDR modules compliant with JEDEC specification.
 *
 ************************************************************************/

#include "SysFunc.h"

/************************************************************************
  --Table of Contents--
  Training:
    1) TrainDramRon
    2) TrainMcOdt
    3) TrainNonTgtOdt
    4) TrainRttWr
    5) TrainMcRon
    6) TrainTxEq
    7) TrainImode
    8) TrainCTLE
  API
    1) TrainDDROptParam
    2) CalcPowerTrend
    3) FindOptimalTradeoff
*************************************************************************/
//#define POWER_TRAINING_DEBUG 1

#ifdef RC_SIM_FEEDBACK
const UINT8 RCSimTxEqScale[MAX_TX_EQ] = {70, 83, 96, 100, 90, 80, 60, 40, 20, 10, 10, 0, 0, 0, 0};
const UINT8 RCSimImodeScale[MAX_IMODE] = {70, 83, 95, 100, 85, 65, 30, 0};
const UINT8 RCSimRxCTLEScale[MAX_CTLE] = {65, 73, 85, 94, 100, 85, 65, 45, 25};
const UINT8 RCSimDRAMRonScale[MAX_DRAMRON] = {85, 100};
const UINT8 RCSimMCODTScale[MAX_MCODT] = {88, 100};
const UINT8 RCSimNONTGTODTScale[MAX_NONTGTODT] = {72, 85, 94, 100, 90, 78, 59, 20, 0};
const UINT8 RCSimRTTWRScale[MAX_RTTWR] = {78, 100, 60};
const UINT8 RCSimMCRonScale[MAX_MCRON] = {80, 100, 65};
#endif

UINT32
TrainDramRon(
  PSYSHOST host
)
{
  UINT8   socket;
  UINT16  mode;
  UINT32  patternLength;
  GSM_GT  group[MAX_GROUPS];
  UINT8   numGroups;
  INT16   settingList[MAX_SETTINGS][MAX_PARAMS] = {{DDR4_RON_34},{DDR4_RON_48}};
  UINT16  powerLevel[MAX_SETTINGS] = LINEAR_ARRAY;
  UINT8   numSettings;
  GSM_GT  param[MAX_PARAMS];
  UINT8   paramType;
  UINT8   numParams;
  UINT8   marginType;

  socket = host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return SUCCESS;

  //
  // This step only applies to DDR4
  //
  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) return SUCCESS;

  //
  // Return if this step is disabled
  //
  if (~host->setup.mem.optionsExt & DRAM_RON_EN) return SUCCESS;

  // Parameters to Pass
  mode             = MODE_VIC_AGG;
  patternLength    = 64;
  group[0]         = RxVref;
  group[1]         = RxDqsDelay;
  numGroups        = 2;
  marginType       = GET_MARGINS;
  numSettings      = 2;
  param[0]         = DramDrvStr;
  numParams        = 1;

  paramType = GetChipParamType (host, traindramron);

  TrainDDROptParam(host, socket, mode, patternLength, group, numGroups, marginType, settingList, powerLevel, numSettings,
                   param, paramType, numParams);

  return SUCCESS;

} // TrainDramRon

UINT32
TrainMcOdt(
  PSYSHOST host
)
{
  UINT8   socket;
  UINT16  mode;
  UINT32  patternLength;
  GSM_GT  group[MAX_GROUPS];
  UINT8   numGroups;
  INT16   settingList[MAX_SETTINGS][MAX_PARAMS] = {{ODT_50_OHMS}, {ODT_100_OHMS}};
  UINT16  powerLevel[MAX_SETTINGS] = LINEAR_ARRAY;
  UINT8   numSettings;
  GSM_GT  param[MAX_PARAMS];
  UINT8   paramType;
  UINT8   numParams;
  UINT8   marginType;

  socket = host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return SUCCESS;

  //
  // This step only applies to DDR4
  //
  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) return SUCCESS;


  //
  // Return if this step is disabled
  //
  if (~host->setup.mem.optionsExt & RX_ODT_EN) return SUCCESS;

  //
  // Parameters to Pass
  //
  mode            = MODE_VIC_AGG;
  patternLength   = 64;
  group[0]        = RxVref;
  group[1]        = RxDqsDelay;
  numGroups       = 2;
  marginType      = GET_MARGINS;
  numSettings     = 2;
  param[0]        = RxOdt;
  numParams       = 1;

  paramType = GetChipParamType (host, trainmcodt);

  TrainDDROptParam(host,socket,mode,patternLength,group,numGroups,marginType,settingList,powerLevel,numSettings,param,paramType,numParams);

  return SUCCESS;

} // TrainMcOdt

UINT32
TrainNonTgtOdt(
  PSYSHOST host
)
{
  UINT8               socket;
  UINT16              mode;
  UINT32              patternLength;
  GSM_GT              group[MAX_GROUPS];
  UINT8               numGroups;
  INT16               settingList[MAX_ODT_SETTINGS][MAX_PARAMS] = {{0}};
  UINT16              powerLevel[MAX_SETTINGS]          = LINEAR_ARRAY;
  UINT8               numSettings                       = 0;
  UINT8               totalNumSettings[MAX_CH][MAX_DIMM]  = {{1}};   // SKX for per DIMM case
  GSM_GT              param[MAX_PARAMS];
  UINT8               paramType;
  UINT8               numParams;
  UINT8               marginType;
                                      //        0                  7                 3                 5                 1                 6                 2                  4
  INT16               ntLookupPrk[] = {DDR4_RTT_PARK_DIS, DDR4_RTT_PARK_34, DDR4_RTT_PARK_40, DDR4_RTT_PARK_48, DDR4_RTT_PARK_60, DDR4_RTT_PARK_80, DDR4_RTT_PARK_120, DDR4_RTT_PARK_240};
  INT16               ntLookupNom[] = {DDR4_RTT_NOM_DIS,  DDR4_RTT_NOM_34,  DDR4_RTT_NOM_40,  DDR4_RTT_NOM_48,  DDR4_RTT_NOM_60,  DDR4_RTT_NOM_80,  DDR4_RTT_NOM_120,  DDR4_RTT_NOM_240};

  UINT8               counter;
  UINT8               odtValue      = 0;
  UINT8               setting       = 0;
  UINT8               setting2      = 0;
 #if POWER_TRAINING_DEBUG
  INT16               initOdt       = 0;
 #endif
  UINT8               prkDivisor[MAX_CH][MAX_DIMM]    = {{1}};    // Initial values assume only disabled setting
  UINT8               nomDivisor[MAX_CH][MAX_DIMM]    = {{1}};
  UINT8               startingprk[MAX_CH][MAX_DIMM]   = {{0}};
  UINT8               startingnom[MAX_CH][MAX_DIMM]   = {{0}};

  UINT8               dimm;
  UINT8               rank;
  UINT8               ch;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];

  socket = host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return SUCCESS;

  //
  // This step only applies to DDR4
  //
  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) return SUCCESS;

  //
  // Return if this step is disabled
  //
  if (~host->setup.mem.optionsExt & NON_TGT_ODT_EN) return SUCCESS;

  //
  // Parameters to Pass
  //
  mode            = MODE_VIC_AGG;
  patternLength   = 64;
  group[0]        = RxVref;
  group[1]        = RxDqsDelay;
  group[2]        = TxVref;
  group[3]        = TxDqDelay;
  numGroups       = 4;
  marginType      = GET_MARGINS;
  param[0]        = ParkOdt;
  param[1]        = NomOdt;
  param[2]        = ParkOdt;
  param[3]        = NomOdt;
  numParams       = 4;

  paramType = GetChipParamType (host, trainnontgtodt);
  //
  // Dynamically populate the array for NomOdt, and ParkOdt
  //
  channelNvList = GetChannelNvList(host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    dimmNvList = GetDimmNvList(host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
      rankList = GetRankNvList(host, socket, ch, dimm);
      rank = 0;
      //
      // Only do rank 0 for each DIMM
      //
      // Skip if no rank
      //
      if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

      //
      // Find matching values for RTT_PARK and RTT_NOM for each DIMM
      //
      //
      // Boundary Checks afterwards

      for(counter = 0; counter < MAX_RTT_PARK; counter++) {
        if (ntLookupPrk[counter] == (*rankList)[rank].RttPrk >> DDR4_RTT_PRK_ODT_SHIFT ) {

          switch (counter) {
            case 0:
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "DIS"));
              break;
            case 1:
              odtValue = 34;
              break;
            case 2:
              odtValue = 40;
              break;
            case 3:
              odtValue = 48;
              break;
            case 4:
              odtValue = 60;
              break;
            case 5:
              odtValue = 80;
              break;
            case 6:
              odtValue = 120;
              break;
            case 7:
              odtValue = 240;
              break;
            default:
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "No match found for RTT_PRK...\n"));
          }
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
            "RTT_PARK is %d Ohms\n", odtValue));
          odtValue = 0;

          //
          // Conditionals to assign array index positions for first and last index cases
          // Decrement the starting counter by 1 for two cases:
          //  1) If it's at index 2 since disabled is ignored and we sweep
          //  2) If it's the last index
          //

          // General case
          if ( (counter > 1) && (counter < MAX_RTT_PARK-1) ){
            startingprk[ch][dimm] = counter - 1;
            prkDivisor[ch][dimm]  = 3;
          }
          // Last case
          if (counter == MAX_RTT_PARK-1){
            startingprk[ch][dimm] = counter - 1;
            prkDivisor[ch][dimm]  = 2;
          }
          // First case
          if (counter == 1){
            startingprk[ch][dimm] = counter;
            prkDivisor[ch][dimm] = 2;
          }
        }
      } // park loop

      for(counter = 0; counter < MAX_RTT_NOM; counter++) {
        if (ntLookupNom[counter] == (*rankList)[rank].RttNom >> DDR4_RTT_NOM_ODT_SHIFT ) {
        switch (counter) {
            case 0:
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "DIS"));
              break;
            case 1:
              odtValue = 34;
              break;
            case 2:
              odtValue = 40;
              break;
            case 3:
              odtValue = 48;
              break;
            case 4:
              odtValue = 60;
              break;
            case 5:
              odtValue = 80;
              break;
            case 6:
              odtValue = 120;
              break;
            case 7:
              odtValue = 240;
              break;
            default:
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                "No match found for RTT_NOM...\n"));
          }
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
            "RTT_NOM is %d Ohms\n", odtValue));
          odtValue = 0;

          //
          // Conditionals to assign array index positions for first and last index cases
          // Decrement the starting counter by 1 for two cases:
          //  1) If it's at index 2 since disabled is ignored and we sweep
          //  2) If it's the last index
          //

          // General case
          if ( (counter > 1) && (counter < MAX_RTT_NOM-1) ){
            startingnom[ch][dimm] = counter - 1;
            nomDivisor[ch][dimm] = 3;
          }
          // Last case
          if (counter == MAX_RTT_NOM-1){
            startingnom[ch][dimm] = counter - 1;
            nomDivisor[ch][dimm] = 2;
          }
          // First case
          if (counter == 1){
            startingnom[ch][dimm] = counter;
            nomDivisor[ch][dimm] = 2;
          }
        } // switch loop
      } // nom loop
      // Calculate the number of settings for this channel
      totalNumSettings[ch][dimm] = prkDivisor[ch][dimm] * nomDivisor[ch][dimm];
    } // dimm loop
  } // ch loop

  //
  // Generate the array entries for RTT_PARK and RTT_NOM based on the starting indices for each DIMM in the channel
  //
  numSettings = 0;
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    dimmNvList = GetDimmNvList(host, socket, ch);

    for(setting = 0; setting < totalNumSettings[ch][0]; setting++) {
      dimm = 0;
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
      // For 2 x 3 settings or 3 x 2 settings for nom and park
      if (totalNumSettings[ch][dimm]%6 == 0) {
        if (prkDivisor[ch][dimm] == 3){
          settingList[numSettings][dimm]    = ntLookupPrk[startingprk[ch][dimm] + (setting/2)%prkDivisor[ch][dimm]];
          settingList[numSettings][dimm+1]  = ntLookupNom[startingnom[ch][dimm] + setting%nomDivisor[ch][dimm]];
        }
        if (nomDivisor[ch][dimm] == 3){
          settingList[numSettings][dimm]    = ntLookupPrk[startingprk[ch][dimm] + setting%prkDivisor[ch][dimm]];
          settingList[numSettings][dimm+1]  = ntLookupNom[startingnom[ch][dimm] + (setting/2)%nomDivisor[ch][dimm]];
        }
      // General settings
      } else {
        settingList[numSettings][dimm]      = ntLookupPrk[startingprk[ch][dimm] + (setting/prkDivisor[ch][dimm])%prkDivisor[ch][dimm]];
        settingList[numSettings][dimm+1]    = ntLookupNom[startingnom[ch][dimm] + setting%nomDivisor[ch][dimm]];
      }

      for(setting2 = 0; setting2 < totalNumSettings[ch][1]; setting2++) {
        dimm = 1;
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          numSettings += 1;
          continue;
        }
        // For 2 x 3 settings or 3 x 2 settings for nom and park
        if (totalNumSettings[ch][dimm]%6 == 0) {
          if (prkDivisor[ch][dimm] == 3){
            settingList[numSettings][dimm+1]  = ntLookupPrk[startingprk[ch][dimm] + (setting2/2)%prkDivisor[ch][dimm]];
            settingList[numSettings][dimm+2]  = ntLookupNom[startingnom[ch][dimm] + setting2%nomDivisor[ch][dimm]];
          }
          if (nomDivisor[ch][dimm] == 3){
            settingList[numSettings][dimm+1]  = ntLookupPrk[startingprk[ch][dimm] + setting2%prkDivisor[ch][dimm]];
            settingList[numSettings][dimm+2]  = ntLookupNom[startingnom[ch][dimm] + (setting2/2)%nomDivisor[ch][dimm]];
          }
        // General settings
        } else {
          settingList[numSettings][dimm+1]    = ntLookupPrk[startingprk[ch][dimm] + (setting2/prkDivisor[ch][dimm])%prkDivisor[ch][dimm]];
          settingList[numSettings][dimm+2]    = ntLookupNom[startingnom[ch][dimm] + setting2%nomDivisor[ch][dimm]];
        }
        numSettings += 1;

        // Copy the current dimm 0 settings to each iteration of dimm 1
        settingList[numSettings][0]    = settingList[numSettings-1][0];
        settingList[numSettings][1]    = settingList[numSettings-1][1];
      } // setting2 loop
    } // setting loop
  } // ch loop
  // End array generation from initial RTT_NOM and RTT_PARK values

 #if POWER_TRAINING_DEBUG
  //--------------------------
  // Print the resulting array
  //--------------------------
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
    "\nPrinting initialized array of ODT values\n\n"));

  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
    "Setting\tD0 Park\tD0 Nom\tD1 Park\tD1 Nom\n"));
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
    "--------------------------------------------\n"));
  for (setting = 0; setting < numSettings; setting++) {
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
      "%d\t",setting ));
    for (counter = 0; counter < numParams; counter++) {
      initOdt = settingList[setting][counter];
      switch (initOdt) {
        case 0:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "DIS"));
          break;
        case 1:
          initOdt = 60;
          break;
        case 2:
          initOdt = 120;
          break;
        case 3:
          initOdt = 40;
          break;
        case 4:
          initOdt = 240;
          break;
        case 5:
          initOdt = 48;
          break;
        case 6:
          initOdt = 80;
          break;
        case 7:
          initOdt = 34;
          break;
        default:
          initOdt = 0;
      }
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        "%3d\t", initOdt ));
    }
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
      "\n" ));
  }
  // End output of array
 #endif // POWER_TRAINING_DEBUG

  TrainDDROptParam(host, socket, mode, patternLength, group, numGroups, marginType, settingList, powerLevel, numSettings,
                   param, paramType, numParams);

  return SUCCESS;

} // TrainNonTgtOdt

UINT32
TrainRttWr(
  PSYSHOST host
)
{
  UINT8   socket;
  UINT16  mode;
  UINT32  patternLength;
  GSM_GT  group[MAX_GROUPS];
  UINT8   numGroups;
  INT16   settingList[MAX_SETTINGS][MAX_PARAMS] = {{RTTWR_DIS},{RTTWR_120},{RTTWR_240}};
  UINT16  powerLevel[MAX_SETTINGS] = LINEAR_ARRAY;
  UINT8   numSettings;
  GSM_GT  param[MAX_PARAMS];
  UINT8   paramType;
  UINT8   numParams;
  UINT8   marginType;

  socket = host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return SUCCESS;

  //
  // This step only applies to DDR4
  //
  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) return SUCCESS;

  //
  // Return if this step is disabled
  //
  if (~host->setup.mem.optionsExt & RTT_WR_EN) return SUCCESS;

  // Parameters to Pass
  mode             = MODE_VIC_AGG;
  patternLength    = 64;
  group[0]         = TxVref;
  group[1]         = TxDqDelay;
  numGroups        = 2;
  marginType       = GET_MARGINS;
  numSettings      = 3;
  param[0]         = WrOdt;
  numParams        = 1;

  paramType = GetChipParamType (host, trainrttwr);

  TrainDDROptParam(host,socket,mode,patternLength,group,numGroups,marginType,settingList,powerLevel,numSettings,param,paramType,numParams);

  return SUCCESS;

} // TrainRttWr

UINT32
TrainMcRon(
  PSYSHOST host
)
{
  UINT8   socket;
  UINT16  mode;
  UINT32  patternLength;
  GSM_GT  group[MAX_GROUPS];
  UINT8   numGroups;
  INT16   settingList[MAX_SETTINGS][MAX_PARAMS];
  UINT16  powerLevel[MAX_SETTINGS] = LINEAR_ARRAY;
  UINT8   numSettings;
  GSM_GT  param[MAX_PARAMS];
  UINT8   paramType;
  UINT8   numParams;
  UINT8   marginType;

  socket = host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return SUCCESS;

  //
  // This step only applies to DDR4
  //
  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) return SUCCESS;

  //
  // Return if this step is disabled
  //
  if (~host->setup.mem.optionsExt & MC_RON_EN) return SUCCESS;

  // Parameters to Pass
  mode          = MODE_VIC_AGG;
  patternLength = 64;
  group[0]      = TxVref;
  group[1]      = TxDqDelay;
  numGroups     = 2;
  marginType    = GET_MARGINS;
  numSettings   = 3;
  param[0]      = TxRon;
  numParams     = 1;

  paramType = GetChipParamType (host, trainmcron);

  //
  // Update the settings list
  //
  numSettings = GetMcRonSettingsList(host, socket, settingList);

  TrainDDROptParam(host,socket,mode,patternLength,group,numGroups,marginType,settingList,powerLevel,numSettings,param,paramType,numParams);

  return SUCCESS;

} // TrainMcRon

UINT32
TrainTxEq(
  PSYSHOST host
)
{
  UINT8                               socket;
  UINT16                              mode;
  UINT32                              patternLength;
  GSM_GT                              group[MAX_GROUPS];
  UINT8                               numGroups;
  INT16                               settingList[MAX_SETTINGS][MAX_PARAMS];
  UINT16                              powerLevel[MAX_SETTINGS] = LINEAR_ARRAY;
  UINT8                               numSettings;
  GSM_GT                              param[MAX_PARAMS];
  UINT8                               paramType;
  UINT8                               numParams;
  UINT8                               marginType;

  socket = host->var.mem.currentSocket;

  //if ((~host->memFlows & MF_TX_EQ_EN) || (host->nvram.mem.socket[socket].lrDimmPresent)) return SUCCESS;
  if (~host->memFlows & MF_TX_EQ_EN) return SUCCESS;

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return SUCCESS;

  //
  // This step only applies to DDR4
  //
  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) return SUCCESS;

  //
  // Return if this step is disabled
  //
  if (~host->setup.mem.optionsExt & TX_EQ_EN) return SUCCESS;

  //
  // This step only applies to DDR4
  //
  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) return SUCCESS;

  // Parameters to Pass
  mode             = MODE_VIC_AGG;
  patternLength    = 64;
  group[0]         = TxVref;
  group[1]         = TxDqDelay;
  numGroups        = 2;
  marginType       = GET_MARGINS;
  param[0]         = TxEq;
  numParams        = 1;

  paramType = GetChipParamType (host, traintxeq);

  //
  // Update the settings list
  //
  numSettings = GetTxEqSettingsList(host, socket, settingList);

  TrainDDROptParam(host, socket, mode, patternLength, group, numGroups, marginType, settingList, powerLevel, numSettings,
                   param, paramType, numParams);

  IO_Reset (host, socket);
  JedecInitSequence (host, socket, CH_BITMASK);

  return SUCCESS;

} // TrainTxEq

UINT32
TrainImode(
  PSYSHOST host
)
{
  UINT8   socket;
  UINT8   ch;
  UINT16  mode;
  UINT32  patternLength;
  GSM_GT  group[MAX_GROUPS];
  UINT8   numGroups;
  INT16   settingList[MAX_SETTINGS][MAX_PARAMS];
  UINT16  powerLevel[MAX_SETTINGS] = LINEAR_ARRAY;
  UINT8   numSettings;
  GSM_GT  param[MAX_PARAMS];
  UINT8   paramType;
  UINT8   numParams;
  UINT8   marginType;
  UINT16  ConstantValue = 0xF;
  struct channelNvram (*channelNvList)[MAX_CH];

  socket = host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return SUCCESS;

  //
  // This step only applies to DDR4
  //
  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) return SUCCESS;

  IO_Reset (host, socket);

  if ((~host->memFlows & MF_IMODE_EN) || ((host->setup.mem.optionsExt & IMODE_EN) == 0)) {// Set IMode to 0xF 
    channelNvList = GetChannelNvList(host, socket);
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      GetSetDataGroup (host, socket, ch, 0, 0, ALL_STROBES, 0, DdrLevel, TxImode, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, (INT16 *)&ConstantValue);
    }
  } else { // Train IMode
    //
    // Update the settings list
    //
    numSettings = GetImodeSettingsList(host, socket, settingList);
 
    // Parameters to Pass
    mode             = MODE_VIC_AGG;
    patternLength    = 64;
    group[0]         = TxVref;
    group[1]         = TxDqDelay;
    numGroups        = 2;
    marginType       = GET_MARGINS;
    param[0]         = TxImode;
    numParams        = 1;

    paramType = GetChipParamType (host, trainimode);

    TrainDDROptParam(host, socket, mode, patternLength, group, numGroups, marginType, settingList, powerLevel, numSettings, param, paramType, numParams);
  }

  return SUCCESS;

} // TrainImode

UINT32
TrainCTLE(
  PSYSHOST host
)
{
  UINT8   socket;
  UINT16  mode;
  UINT32  patternLength;
  GSM_GT  group[MAX_GROUPS];
  UINT8   numGroups;
  INT16   settingList[MAX_SETTINGS][MAX_PARAMS] = {
                                                    {4,0,0},
                                                    {4,0,1},
                                                    {4,0,2},
                                                    {4,1,0},
                                                    {4,1,1},
                                                    {4,1,2},
                                                    {4,2,0},
                                                    {4,2,1},
                                                    {4,2,2}
                                                  };
  UINT16  powerLevel[MAX_SETTINGS] = LINEAR_ARRAY;
  UINT8   numSettings;
  GSM_GT  param[MAX_PARAMS];
  UINT8   paramType;
  UINT8   numParams;
  UINT8   marginType;

  socket = host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return SUCCESS;

  //
  // This step only applies to DDR4
  //
  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) return SUCCESS;

  //
  // Return if this step is disabled
  //
  if (~host->memFlowsExt & MF_EXT_RX_CTLE_EN) return SUCCESS;
  if ((~host->setup.mem.optionsExt & RX_CTLE_TRN_EN)) return SUCCESS;

  // Parameters to Pass
  mode              = MODE_VIC_AGG;
  patternLength     = 64;
  group[0]          = RxVref;
  group[1]          = RxDqsDelay;
  numGroups         = 2;
  marginType        = GET_MARGINS;
  param[0]          = RxEq;
  param[1]          = RxCtleC;
  param[2]          = RxCtleR;
  numParams         = 3;

  paramType   = GetChipParamType (host, trainctle);
  numSettings = GetCtleSettingsList (host, socket, settingList);
  TrainDDROptParam(host, socket, mode, patternLength, group, numGroups, marginType, settingList, powerLevel, numSettings,
                   param, paramType, numParams);

  return SUCCESS;

} // TrainCTLE


void
TrainDDROptParam(
  PSYSHOST          host,
  UINT8             socket,
  UINT16            mode,
  UINT32            patternLength,
  GSM_GT            group[MAX_GROUPS],
  UINT8             numGroups,
  UINT8             marginType,
  INT16             settingList[MAX_ODT_SETTINGS][MAX_PARAMS],
  UINT16            powerLevel[MAX_SETTINGS],
  UINT8             numSettings,
  GSM_GT            param[MAX_PARAMS],
  UINT8             paramType,
  UINT8             numParams
)
/*++

Routine Description:

  This function implements a generic 1-D parameter optimization

Arguments:

  host    - Point to sysHost

Returns:

  UINT32  - if it succeeded, return -1

--*/
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  UINT8               setting;
  UINT8               paramIndex;
  UINT8               groupIndex;
  INT16               margins[MAX_CH][MAX_RANK_CH][MAX_STROBE][MAX_GROUPS][MAX_SETTINGS];
  INT16               averageMargins[MAX_CH][MAX_RANK_CH][MAX_STROBE][MAX_GROUPS][MAX_ODT_SETTINGS];
  INT16               bestSetting[MAX_CH][MAX_RANK_CH][MAX_STROBE];
  INT16               powerTrendLine[MAX_CH][MAX_RANK_CH][MAX_STROBE][MAX_SETTINGS];
  INT16               defaultValues[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_STROBE][MAX_PARAMS] = {{{{{0}}}}};
  INT16               margin;
  INT16               minusOneMargin;
  INT16               plusOneMargin;
  UINT8               slopeFactor = 0;
  INT16               curVal       = 0;
  UINT8               curParam     = 0;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  struct bitMask      filter;
  struct strobeMargin strobeMargins;

  channelNvList = GetChannelNvList(host, socket);
  //host->var.mem.serialDebugMsgLvl = SDBG_MAX;
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
    "\nPrinting initialized array of cached values...\n\n"));
  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter(host, socket, &filter);
  for (groupIndex = 0; groupIndex < numGroups; groupIndex++) {
    for (ch = 0; ch < MAX_CH; ch++) {
      for (rank = 0; rank < MAX_RANK_CH; rank++) {
        for (strobe = 0; strobe < MAX_STROBE; strobe++) {
            bestSetting[ch][rank][strobe]             = 0;
        } // strobe loop
      } // rank loop
    } // ch loop
  } // group loop

#if POWER_TRAINING_DEBUG
  //
  // Initialize the default value array with the cached values
  //
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
    "\nDefault Cached Values\n"));
#endif // POWER_TRAINING_DEBUG
  //------------------------------
  // Cycle through parameter types
  //------------------------------
  for (paramIndex = 0; paramIndex < numParams; paramIndex++) {
    if (paramType == PER_CH){
      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        GetSetDataGroup(host, socket, ch, 0, 0, 0, ALL_BITS, DdrLevel, param[paramIndex], GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &defaultValues[ch][0][0][0][paramIndex]);
        curVal = defaultValues[ch][0][0][0][paramIndex];
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "%d\n", curVal ));
      } // ch loop
    } // per ch

    else if (paramType == PER_MC){
      for (ch = 0; ch < MAX_CH; ch += MAX_MC_CH) {    // chipset specific
        if ((*channelNvList)[ch].enabled == 0) continue;
        GetSetDataGroup(host, socket, ch, 0, 0, 0, ALL_BITS, DdrLevel, param[paramIndex], GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &defaultValues[ch][0][0][0][paramIndex]);
        curVal = defaultValues[ch][0][0][0][paramIndex];
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "%d\n", curVal ));
      } // ch loop
    } // per MC

    else if (paramType == PER_RANK) {   // DRAM Ron is the only test that does per rank
      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        dimmNvList = GetDimmNvList(host, socket, ch);
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
            GetSetDataGroup(host, socket, ch, dimm, rank, 0, ALL_BITS, DdrLevel, param[paramIndex], GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &defaultValues[ch][dimm][rank][0][paramIndex]);
            curVal = defaultValues[ch][dimm][rank][0][paramIndex];
            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
              "%2d\n", curVal ));
          } // rank loop
        } // dimm loop
      } // ch loop
    } // per rank

    else if (paramType == PER_CH_PER_STROBE) {
      // Fill in margin entries for strobes for per ch per strobe
      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;

        // if (param[paramIndex] == RxOdt){
        // curSetting = MAX_STROBE;
        if (host->nvram.mem.eccEn) {
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "\nStrobe   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  \n"));
        } else {
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "\nStrobe   0   1   2   3   4   5   6   7   9  10  11  12  13  14  15  16  \n"));
        }
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "--------------------------------------------------------------------------------\n"));
        // } else {
          // curSetting = MAX_STROBE/2;
          // MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            // "\nNibble   0   1   2   3   4   5   6   7   8 \n"));
          // MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            // "--------------------------------------------\n"));
        // }
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          ""));
        for (strobe = 0; strobe < MAX_STROBE; strobe++) {
          if ((!host->nvram.mem.eccEn) && (strobe == 8))  continue;
          GetSetDataGroup(host, socket, ch, NO_DIMM, NO_RANK, strobe, ALL_BITS, DdrLevel, param[paramIndex], GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &defaultValues[ch][0][0][strobe][paramIndex] );
          curVal = defaultValues[ch][0][0][strobe][paramIndex];
          if (param[paramIndex] == TxImode){
            curVal = curVal;
          }
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "%3d ", curVal ));
        } // strobe loop
      } // ch loop
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        "\n"));
    } // per ch per byte or per ch per strobe

    else if (paramType == PER_CH_PER_DIMM) {
      curParam = param[paramIndex];
      switch (curParam){
        case ParkOdt:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "RTT PARK\n"));
          break;
        case NomOdt:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "RTT NOM\n"));
          break;
        case WrOdt:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "RTT WR\n"));
          break;
        default:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "Unknown parameter\n"));
          break;
      }

      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        dimmNvList = GetDimmNvList(host, socket, ch);

        //
        // Skip if no DIMM present
        //
        rank    = 0;
        dimm    = paramIndex/2;     // 0 0 1 1 dimm
        // curDimm = paramIndex%2;     // 0 1 0 1 prk/nom
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        rankList = GetRankNvList(host, socket, ch, dimm);

        //
        // Skip if no rank
        //
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        GetSetDataGroup(host, socket, ch, dimm, rank, 0, ALL_BITS, DdrLevel, param[paramIndex], GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &defaultValues[ch][dimm][0][0][paramIndex]);

// #if POWER_TRAINING_DEBUG
        curVal = defaultValues[ch][dimm][0][0][paramIndex];

        if (param[paramIndex] == WrOdt){
          if (0 == curVal) {
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "DIS"));
          }
        }

        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
          "%3d\n", curVal ));
// #endif
      } // ch loop
    } // per ch per dimm

    else if (paramType == PER_STROBE) {
      curParam = param[paramIndex];
      switch (curParam){
        case TxEq:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "Tx Eq\n"));
          break;
        case RxEq:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "Rx Eq\n"));
          break;
        case RxCtleC:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "CTLE C\n"));
          break;
        case RxCtleR:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "CTLE R\n"));
          break;
        case TxImode:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "Tx Imode\n"));
          break;
        default:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "Unknown parameter\n"));
          break;
      }
      if (host->nvram.mem.eccEn) {
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "\nStrobe        0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 \n"));
      } else {
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
           "\nStrobe        0  1  2  3  4  5  6  7  9 10 11 12 13 14 15 16 \n"));
      }
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        "-------------------------------------------------------------------\n"));

      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        dimmNvList = GetDimmNvList(host, socket, ch);

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
            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                          "" ));
            for (strobe = 0; strobe < MAX_STROBE; strobe++) {
              if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
              GetSetDataGroup(host, socket, ch, dimm, rank, strobe, ALL_BITS, DdrLevel, param[paramIndex], GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &defaultValues[ch][dimm][rank][strobe][paramIndex] );
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "%2d ", defaultValues[ch][dimm][rank][strobe][paramIndex] ));
            } // strobe loop
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "\n"));
          } // rank loop
        } // dimm loop
      } // ch loop
    } // per strobe
    else {
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "\nUnrecognized parameter type\n"));
      continue;
    }
  } // paramIndex
  //
  // End cycling through param indices
  //

  //--------------------------------------------------------
  // Loop through settings to find the optimal Tx Vref
  //--------------------------------------------------------
  for(setting = 0; setting < numSettings; setting++) {
    //
    // Set Parameter
    //

// #if POWER_TRAINING_DEBUG
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        "\nSetting %d\tParams ", setting ));
    for (paramIndex = 0; paramIndex < numParams; paramIndex++) {
      if ((param[paramIndex] == DramDrvStr) || (param[paramIndex] == RxOdt)){
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "[%d]  = %d Ohms\t", paramIndex, settingList[setting][paramIndex]));
      } else if (param[paramIndex] == WrOdt){
        if (settingList[setting][paramIndex] == 0){
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "[%d] 0x%x ==> Disable\t", paramIndex, settingList[setting][paramIndex]));
        } else {
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "[%d] %d Ohms\t", paramIndex, settingList[setting][paramIndex]));
        }
      } else if (param[paramIndex] == TxRon){
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "[%d] %d Ohms \t", paramIndex, settingList[setting][paramIndex]));
      } else if ((param[paramIndex] == ParkOdt) || (param[paramIndex] == NomOdt)){
        if (paramIndex == 0){
          curVal = settingList[setting][0];
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "D0:PRK "));
        }
        if (paramIndex == 1){
          curVal = settingList[setting][1];
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "\tD0:NOM "));
        }
        if (paramIndex == 2){
          curVal = settingList[setting][2];
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "\tD1:PRK "));
        }
        if (paramIndex == 3){
          curVal = settingList[setting][3];
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "\tD1:NOM "));
        }
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "%3d", curVal));
      }

      else if (param[paramIndex] == RxEq ||  param[paramIndex] == RxCtleC || param[paramIndex] == RxCtleR){
          if (param[paramIndex] == RxEq){
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "Rx Eq 0x%x ", settingList[setting][0]  ));
          } else if (param[paramIndex] == RxCtleC){
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "CTLE C 0x%x ", settingList[setting][1] ));
          } else {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "CTLE R 0x%x\n", settingList[setting][2] ));
          }
      }

      else {
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "[%d] 0x%x\n", paramIndex, settingList[setting][paramIndex]));
      }
// #endif
      //
      // Set parameters for each setting
      //
      if (paramType == PER_CH){
        for (ch = 0; ch < MAX_CH; ch++) {
          if ((*channelNvList)[ch].enabled == 0) continue;
          GetSetDataGroup(host, socket, ch, 0, 0, 0, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &settingList[setting][paramIndex]);
        }
      } else if (paramType == PER_MC){
        for (ch = 0; ch < MAX_CH; ch+=3) {
          if ((*channelNvList)[ch].enabled == 0) continue;
          GetSetDataGroup(host, socket, ch, 0, 0, 0, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &settingList[setting][paramIndex]);
        }
      } else if (paramType == PER_RANK) {
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
              GetSetDataGroup(host, socket, ch, dimm, rank, 0, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &settingList[setting][paramIndex]);
            } // rank loop
          } // dimm loop
        } // ch loop
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "\n"));
      } else if (paramType == PER_STROBE) {
        for (ch = 0; ch < MAX_CH; ch++) {
          if ((*channelNvList)[ch].enabled == 0) continue;
          dimmNvList = GetDimmNvList(host, socket, ch);

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
              for (strobe = 0; strobe < MAX_STROBE; strobe++) {
                if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
                GetSetDataGroup(host, socket, ch, dimm, rank, strobe, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &settingList[setting][paramIndex]);
              } // strobe loop
            } // rank loop
          } // dimm loop
        } // ch loop
      } // per strobe

      else if (paramType == PER_CH_PER_BYTE) {
        // Fill in margin entries for strobes for per ch per byte and per rank cases
        for (ch = 0; ch < MAX_CH; ch++) {
          if ((*channelNvList)[ch].enabled == 0) continue;
          for (strobe = 0; strobe < MAX_STROBE/2; strobe++) {
            if ((!host->nvram.mem.eccEn) && (strobe == 8))  continue;
            GetSetDataGroup(host, socket, ch, NO_DIMM, NO_RANK, strobe, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &settingList[setting][paramIndex]);
          } // strobe loop
        } // ch loop
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "\n"));
      } // per_ch_per_byte

      else if (paramType == PER_CH_PER_STROBE) {
        // Fill in margin entries for strobes for per ch per strobe
        for (ch = 0; ch < MAX_CH; ch++) {
          if ((*channelNvList)[ch].enabled == 0) continue;
          for (strobe = 0; strobe < MAX_STROBE; strobe++) {
            if ((!host->nvram.mem.eccEn) && (strobe == 8))  continue;
            GetSetDataGroup(host, socket, ch, NO_DIMM, NO_RANK, strobe, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &settingList[setting][paramIndex]);
          } // strobe loop
        } // ch loop
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "\n"));
      } // per_ch_per_strobe
      else if (paramType == PER_CH_PER_DIMM) {
        for (ch = 0; ch < MAX_CH; ch++) {
          if ((*channelNvList)[ch].enabled == 0) continue;
          dimmNvList = GetDimmNvList(host, socket, ch);
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
              GetSetDataGroup(host, socket, ch, dimm, rank, 0, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &settingList[setting][paramIndex]);
            } // rank loop
          } // dimm loop
        } // ch loop
      } // per_ch_per_dimm
#ifdef RC_SIM_FEEDBACK
      //
      // Update the setting value for rcsimfeedback results
      //
      if (param[paramIndex] == DramDrvStr) {
        host->var.mem.eyeSizeScaling = RCSimDRAMRonScale[setting];
      } else if (param[paramIndex] == RxOdt) {
        host->var.mem.eyeSizeScaling = RCSimMCODTScale[setting];
      } else if ( (param[paramIndex] == ParkOdt) || (param[paramIndex] == NomOdt) ) {
        host->var.mem.eyeSizeScaling = RCSimNONTGTODTScale[setting];
      } else if (param[paramIndex] == WrOdt) {
        host->var.mem.eyeSizeScaling = RCSimRTTWRScale[setting];
      } else if (param[paramIndex] == TxRon) {
        host->var.mem.eyeSizeScaling = RCSimMCRonScale[setting];
      } else if (param[paramIndex] == TxEq) {
        host->var.mem.eyeSizeScaling = RCSimTxEqScale[setting];
      } else if (param[paramIndex] == TxImode) {
        host->var.mem.eyeSizeScaling = RCSimImodeScale[setting];
      } else if ( (param[paramIndex] == RxEq) || (param[paramIndex] == RxCtleR) || (param[paramIndex] == RxCtleC) ) {
        host->var.mem.eyeSizeScaling = RCSimRxCTLEScale[setting];
      }
#endif //RC_SIM_FEEDBACK
    } // param

    //----------------------------------------
    // Get per strobe margins for each setting
    //----------------------------------------
    for (groupIndex = 0; groupIndex < numGroups; groupIndex++ ) {
      // Initialize to 0 for all strobe values
      for (ch = 0; ch < MAX_CH; ch++) {
        for (rank = 0; rank < MAX_RANK_CH; rank++) {
          for (strobe = 0; strobe < MAX_STROBE; strobe++) {
            strobeMargins.strobe[ch][rank][strobe].n = 0;
            strobeMargins.strobe[ch][rank][strobe].p = 0;
          } // strobe loop
        } // rank loop
      } // ch loop

      // Get margins
      switch (marginType) {
        case GET_MARGINS:
          GetMargins(host, socket, DdrLevel, group[groupIndex], mode, SCOPE_STROBE, &filter, &strobeMargins, patternLength, 0, 0, 0,
            WDB_BURST_LENGTH);
          break;
        case TER_MARGINS:
          //GetTERMargins(host, socket, DdrLevel, group, mode, SCOPE_STROBE, &filter, &eyeMargin, patternLength, 0, 0, 0,
          //  WDB_BURST_LENGTH);
          break;
        case BER_MARGINS:
          //GetBERMargins(host, socket, DdrLevel, group, mode, SCOPE_STROBE, &filter, &eyeMargin, patternLength, 0, 0, 0,
          //  WDB_BURST_LENGTH);
          break;
        default:
          break;
      }



      // Fill in margin entries for strobes for each case and determine the composite eye --> place it in 0 index
      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        dimmNvList = GetDimmNvList(host, socket, ch);
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

            for (strobe = 0; strobe < MAX_STROBE; strobe++) {
              if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17)))  continue;
              // Per ch per strobe
              if ((paramType == PER_CH_PER_STROBE) || (paramType == PER_CH_PER_BYTE)){
                if (strobe < 9 ) {
                  if (strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].p < strobeMargins.strobe[ch][0][strobe].p){
                    strobeMargins.strobe[ch][0][strobe].p = strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].p;
                  }
                  if (strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].n > strobeMargins.strobe[ch][0][strobe].n){
                    strobeMargins.strobe[ch][0][strobe].n = strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].n;
                  }
                } else if (((*dimmNvList)[dimm].x4Present == 1) && (strobe > 8)  && (group[groupIndex] != TxVref)){
                  if (strobeMargins.strobe[ch][0][strobe].p < strobeMargins.strobe[ch][0][strobe - 9].p){
                    strobeMargins.strobe[ch][0][strobe - 9].p = strobeMargins.strobe[ch][0][strobe].p;
                  }
                  if (strobeMargins.strobe[ch][0][strobe].n > strobeMargins.strobe[ch][0][strobe - 9].n){
                    strobeMargins.strobe[ch][0][strobe - 9].n = strobeMargins.strobe[ch][0][strobe].n;
                  }
                }
              } // per ch per byte

              // Per rank
              if ( (paramType == PER_RANK) ){
                if (strobe < 9) {
                  if (strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].p <= strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][0].p){
                    strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][0].p = strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].p;
                  }
                  if (strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].n >= strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][0].n){
                    strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][0].n = strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].n;
                  }
                } else if (((*dimmNvList)[dimm].x4Present == 1) && (strobe > 8)  && (group[groupIndex] != TxVref)){
                  if (strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].p < strobeMargins.strobe[ch][0][strobe - 9].p){
                    strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][0].p = strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].p;
                  }
                  if (strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].n > strobeMargins.strobe[ch][0][strobe - 9].n){
                    strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][0].n = strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].n;
                  }
                }
              } // per rank

              // Per ch and per mc
              if ( (paramType == PER_CH) || (paramType == PER_MC ) ){
                if (strobe < 9) {
                  if (strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].p < strobeMargins.strobe[ch][0][0].p){
                    strobeMargins.strobe[ch][0][0].p = strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].p;
                  }
                  if (strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].n > strobeMargins.strobe[ch][0][0].n){
                    strobeMargins.strobe[ch][0][0].n = strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].n;
                  }
                } else if (((*dimmNvList)[dimm].x4Present == 1) && (strobe > 8)  && (group[groupIndex] != TxVref)){
                  if (strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].p < strobeMargins.strobe[ch][0][0].p){
                    strobeMargins.strobe[ch][0][0].p = strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].p;
                  }
                  if (strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].n > strobeMargins.strobe[ch][0][0].n){
                    strobeMargins.strobe[ch][0][0].n = strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].n;
                  }
                }
              } // per ch and per ch per dimm

            } // strobe loop
          } // rank loop
        } // dimm loop
      } // ch loop

      // Print results
      switch(group[groupIndex]){
        case TxDqDelay:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "\nTx Eye Widths\n"));
          break;
        case TxVref:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "\nTx Eye Heights\n"));
          break;
        case RxDqsDelay:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "\nRx Eye Widths\n"));
          break;
        case RxVref:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "\nRx Eye Heights\n"));
          break;
        default:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "\nInvalid group\n"));
          break;
      }
     if (host->nvram.mem.eccEn) {
       MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "Strobe         0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17 \n"));
     } else {
       MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "Strobe         0   1   2   3   4   5   6   7   9  10  11  12  13  14  15  16 \n"));
     }
     MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "------------------------------------------------------------------------------------\n"));

     for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        dimmNvList = GetDimmNvList(host, socket, ch);
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

            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                    ""));
            for (strobe = 0; strobe < MAX_STROBE; strobe++) {
              if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17)))  continue;
              if (strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].n > strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].p) {
                strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].p = 0;
                strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].n = 0;
              }
              if (((*dimmNvList)[dimm].x4Present == 0) && (strobe > 8)  ){
                margins[ch][(*rankList)[rank].rankIndex][strobe][groupIndex][setting] =
                  strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe - 9].p - strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe - 9].n;
              } else {
                margins[ch][(*rankList)[rank].rankIndex][strobe][groupIndex][setting] =
                  strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].p - strobeMargins.strobe[ch][(*rankList)[rank].rankIndex][strobe].n;
              }
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "%3d ", margins[ch][(*rankList)[rank].rankIndex][strobe][groupIndex][setting]));
            } // strobe loop
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "\n"));
          } // rank loop
        } // dimm loop
      } // ch loop
    } // group loop
  } // setting loop

  //
  // Average Margins
  //
  for(setting = 0; setting < numSettings; setting++) {
    for (paramIndex = 0; paramIndex < numParams; paramIndex++) {
      for (groupIndex = 0; groupIndex < numGroups; groupIndex++ ) {
        for (ch = 0; ch < MAX_CH; ch++) {
          if ((*channelNvList)[ch].enabled == 0) continue;
          dimmNvList = GetDimmNvList(host, socket, ch);
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

              for (strobe = 0; strobe < MAX_STROBE; strobe++) {
                if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
                //
                // Margin for the current setting = (margin[setting - 1] + (2 * margin[setting]) + margin[setting + 1] + 2) / 4
                // The "+ 2" is to make the rounding correct
                //
                if(setting > 0) {
                  minusOneMargin = margins[ch][(*rankList)[rank].rankIndex][strobe][groupIndex][setting - 1];
                } else {
                  minusOneMargin = margins[ch][(*rankList)[rank].rankIndex][strobe][groupIndex][setting];
                }
                margin = margins[ch][(*rankList)[rank].rankIndex][strobe][groupIndex][setting];
                if(setting < (numSettings - 1)) {
                  plusOneMargin = margins[ch][(*rankList)[rank].rankIndex][strobe][groupIndex][setting + 1];
                } else {
                  plusOneMargin = margins[ch][(*rankList)[rank].rankIndex][strobe][groupIndex][setting];
                }

                curParam = param[paramIndex];

                switch (curParam){
                  // Tx Eq, Imode apply filtering
                  case TxEq:
                  case TxImode:
                    averageMargins[ch][(*rankList)[rank].rankIndex][strobe][groupIndex][setting] = ((minusOneMargin + (2 * margin) + plusOneMargin) / 4) * 10;
                    break;
                  // Nontgtodt calculation
                  case NomOdt:
                  case ParkOdt:
                    averageMargins[ch][(*rankList)[rank].rankIndex][strobe][groupIndex][setting] = margin;
                    break;
                  // All other training steps
                  default:
                    averageMargins[ch][(*rankList)[rank].rankIndex][strobe][groupIndex][setting] = margin * 10;
                    break;
                } // switch
              } // strobe loop
            } // rank loop
          } // dimm loop
        } // ch loop
      } // group loop
    } // param loop
  } // setting loop

// #if POWER_TRAINING_DEBUG
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
    "\nPower TrendLine Calculation\n"));
// #endif

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

            rankList = GetRankNvList(host, socket, ch, dimm);

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks ; rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        for (strobe = 0; strobe < MAX_STROBE; strobe++) {
          if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

// #if POWER_TRAINING_DEBUG
          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
            ""));
// #endif
          CalcPowerTrend(host, socket, &averageMargins[ch][(*rankList)[rank].rankIndex][strobe][0], powerLevel, slopeFactor,
                         powerTrendLine[ch][(*rankList)[rank].rankIndex][strobe], numSettings, numGroups);

// #if POWER_TRAINING_DEBUG
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "\nFindOptimalTradeoff for Strobe %d:\n", strobe));
// #endif
          //
          // Conditional for 5 tick minimum check to reset to default values
          //
          bestSetting[ch][(*rankList)[rank].rankIndex][strobe] = FindOptimalTradeOff(host,socket,&averageMargins[ch][(*rankList)[rank].rankIndex][strobe][0],powerTrendLine[ch][(*rankList)[rank].rankIndex][strobe],numSettings,numGroups);

// #if POWER_TRAINING_DEBUG
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "\n"));
// #endif
        } // strobe loop
      } // rank loop
    } // dimm loop
  } // ch loop

  //
  // Set optimal setting
  //
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
    "START_OPTIMAL_TRAINING_RESULTS\n"));

  for (paramIndex = 0; paramIndex < numParams; paramIndex++) {
    if (paramType == PER_CH){
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        "Per channel\n"));
      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        if (bestSetting[ch][0][0] == POWERTRAINING_DEFAULT_SETTING){
          GetSetDataGroup(host, socket, ch, 0, 0, 0, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE, &defaultValues[ch][0][0][0][paramIndex]);
          curVal = defaultValues[ch][0][0][0][paramIndex];
        } else {
          GetSetDataGroup(host, socket, ch, 0, 0, 0, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE, &settingList[bestSetting[ch][0][0]][paramIndex]);
          curVal = settingList[bestSetting[ch][0][0]][paramIndex];
        }
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "%2d\n", curVal));
      } // ch loop
    } // per ch

    else if (paramType == PER_MC){
      for (ch = 0; ch < MAX_CH; ch+=3) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        if (bestSetting[ch][0][0] == POWERTRAINING_DEFAULT_SETTING){
          GetSetDataGroup(host, socket, ch, 0, 0, 0, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE, &defaultValues[ch][0][0][0][paramIndex]);
          curVal = defaultValues[ch][0][0][0][paramIndex];
        } else {
          GetSetDataGroup(host, socket, ch, 0, 0, 0, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE, &settingList[bestSetting[ch][0][0]][paramIndex]);
          curVal = settingList[bestSetting[ch][0][0]][paramIndex];
        }
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "%2d\n", curVal ));

      } // ch loop
    } // per MC

    else if (paramType == PER_RANK) {
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        "Per rank\n"));

      // MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        // "\nRank    0  1  2  3 \n"));
      // MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        // "------------------\n"));

      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        dimmNvList = GetDimmNvList(host, socket, ch);
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
            if (bestSetting[ch][(*rankList)[rank].rankIndex][0] == POWERTRAINING_DEFAULT_SETTING){
              GetSetDataGroup(host, socket, ch, 0, 0, 0, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE, &defaultValues[ch][dimm][rank][0][paramIndex]);
              curVal = defaultValues[ch][dimm][rank][0][paramIndex];
            } else {
              GetSetDataGroup(host, socket, ch, 0, 0, 0, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE, &settingList[bestSetting[ch][(*rankList)[rank].rankIndex][0]][paramIndex]);
              curVal = settingList[bestSetting[ch][(*rankList)[rank].rankIndex][0]][paramIndex];
            }
            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
              "%2d\n", curVal ));
          } // rank loop
        } // dimm loop
      } // ch loop
    } // per rank

    else if (paramType == PER_STROBE) {
      curParam = param[paramIndex];
      switch (curParam){
        case TxEq:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "Tx Eq\n"));
          break;
        case RxEq:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "Rx Eq\n"));
          break;
        case RxCtleC:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "CTLE C\n"));
          break;
        case RxCtleR:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "CTLE R\n"));
          break;
        case TxImode:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "Tx Imode\n"));
          break;
        default:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "Unknown parameter\n"));
          break;
      }

      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        "Per Strobe\n"));
      if (host->nvram.mem.eccEn) {
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "\nStrobe        0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 \n"));
      } else {
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "\nStrobe        0  1  2  3  4  5  6  7  9 10 11 12 13 14 15 16 \n"));
      }
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        "-------------------------------------------------------------------\n"));

      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        dimmNvList = GetDimmNvList(host, socket, ch);
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
            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                    ""));
            for (strobe = 0; strobe < MAX_STROBE; strobe++) {
              if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
              if (bestSetting[ch][(*rankList)[rank].rankIndex][strobe] == POWERTRAINING_DEFAULT_SETTING){
                GetSetDataGroup(host, socket, ch, dimm, rank, strobe, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE, &defaultValues[ch][dimm][rank][strobe][paramIndex]);
                curVal = defaultValues[ch][dimm][rank][strobe][paramIndex];
              } else {
                GetSetDataGroup (host, socket, ch, dimm, rank, strobe, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE, &settingList[bestSetting[ch][(*rankList)[rank].rankIndex][strobe]][paramIndex]);
                curVal = settingList[bestSetting[ch][(*rankList)[rank].rankIndex][strobe]][paramIndex];
              }
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
               "%2d ", curVal));
            } // strobe loop
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "\n"));
          } // rank loop
        } // dimm loop
      } // ch loop
    } // per strobe

    else if ((paramType == PER_CH_PER_STROBE) || (paramType == PER_CH_PER_BYTE)) {
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        "Per channel per strobe\n"));

      // if (param[paramIndex] == RxOdt){
      if (host->nvram.mem.eccEn) {
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "Ch/Strobe      0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17 \n"));
      } else {
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "Ch/Strobe      0   1   2   3   4   5   6   7   9  10  11  12  13  14  15  16 \n"));
      }
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        "---------------------------------------------------------------------------------------\n"));

      // Fill in margin entries for strobes for per ch per strobe
      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        dimmNvList = GetDimmNvList(host, socket, ch);
        for (dimm = 0; dimm < 1; dimm++) {    // dimm = 0
          //
          // Skip if no DIMM present
          //
          if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
          rankList = GetRankNvList(host, socket, ch, dimm);

          for (rank = 0; rank < 1; rank++) {    // rank = 0
            //
            // Skip if no rank
            //
            if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                    ""));
            for (strobe = 0; strobe < MAX_STROBE; strobe++) {
              if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17)))  continue;
                // Per ch per byte
              if (bestSetting[ch][(*rankList)[rank].rankIndex][strobe] == POWERTRAINING_DEFAULT_SETTING){
                GetSetDataGroup(host, socket, ch, dimm, rank, strobe, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE, &defaultValues[ch][dimm][rank][strobe][paramIndex]);
                curVal = defaultValues[ch][0][0][strobe][paramIndex];
              } else {
                GetSetDataGroup (host, socket, ch, dimm, rank, strobe, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE, &settingList[bestSetting[ch][(*rankList)[rank].rankIndex][strobe]][paramIndex]);
                curVal = settingList[bestSetting[ch][(*rankList)[rank].rankIndex][strobe]][paramIndex];
              }

              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "%3d ", curVal));
            } // strobe loop
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "\n"));
          } // rank loop
        } // dimm loop
      } // ch loop
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        "\n"));
    } // per ch per byte
    else if (paramType == PER_CH_PER_DIMM) {
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        "Per channel per dimm\n"));
      curParam = param[paramIndex];
      switch (curParam){
         case ParkOdt:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "RTT PARK\n"));
          break;
        case NomOdt:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "RTT NOM\n"));
          break;
        case WrOdt:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "RTT WR\n"));
          break;
        default:
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "Unknown parameter\n"));
          break;
      }

      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        dimmNvList = GetDimmNvList(host, socket, ch);

        //
        // Skip if no DIMM present
        //
        dimm    = paramIndex/2;     // 0 0 1 1 dimm
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
        rankList = GetRankNvList(host, socket, ch, dimm);
        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          //
          // Skip if no rank
          //
            if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
          if (bestSetting[ch][0][0] == POWERTRAINING_DEFAULT_SETTING){
            GetSetDataGroup(host, socket, ch, 0, 0, 0, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE, &defaultValues[ch][dimm][rank][0][paramIndex]);
            curVal = defaultValues[ch][dimm][0][0][paramIndex];
          } else {
              GetSetDataGroup(host, socket, ch, 0, 0, 0, ALL_BITS, DdrLevel, param[paramIndex], GSM_UPDATE_CACHE, &settingList[bestSetting[ch][(*rankList)[rank].rankIndex][0]][paramIndex]);
            curVal = settingList[bestSetting[ch][0][0]][paramIndex];
          }
           // MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
             // "%3d\n", curVal));
        } // rank loop
        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
          "%3d\n", curVal));
      } // ch loop
    } // per ch per dimm
  } // optimal setting loop

  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
    "END_OPTIMAL_TRAINING_RESULTS\n"));
#ifdef RC_SIM_FEEDBACK
  //
  // Set scaling back to init value
  //
  host->var.mem.eyeSizeScaling = 100;
#endif //RC_SIM_FEEDBACK
} // TrainDDROptParam

void
CalcPowerTrend(
  PSYSHOST host,
  UINT8     socket,
  INT16     averageMargins[MAX_GROUPS][MAX_ODT_SETTINGS],
  UINT16    powerLevel[MAX_SETTINGS],
  UINT8     slopeFactor,
  INT16     powerTrendLine[MAX_SETTINGS],
  UINT8     numSettings,
  UINT8     numGroups
)
{
  UINT8     setting;
  INT16     maxMargin[MAX_GROUPS] = {0};
  INT16     maxMarginAllGroups = 0;
  UINT8     group;
  UINT16    minPower;
  UINT16    maxPower;
  INT16     AveOfMax;
  INT16     slope;
  UINT16    UPMLimit = 200;
  INT16     PWRLimit = 480;

  minPower = 0xFFFF;
  maxPower = 0;

  //
  // MaxPower = MAX(Power[params]); MinPower = MIN(Power[params])
  //
  for (setting = 0; setting < numSettings; setting++) {
    if (powerLevel[setting] < minPower) {
      minPower = powerLevel[setting];
    }
    if (powerLevel[setting] > maxPower) {
      maxPower = powerLevel[setting];
    }
    for (group = 0; group < numGroups; group++) {
      if ((averageMargins[group][setting]) > maxMargin[group]) {
        maxMargin[group] = averageMargins[group][setting];
      }
    }
  }

// #if POWER_TRAINING_DEBUG
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
    "minPower = %d : maxPower = %d\n",minPower, maxPower));
// #endif

  if (maxPower == 0) {
    maxPower = 1;
  }
  //
  // AveOfMax = AVEmargins(MAX(Margin[params]))
  //
  AveOfMax = 0;
  for (group = 0; group < numGroups; group++) {
    AveOfMax = AveOfMax + maxMargin[group];
  }
  AveOfMax = AveOfMax / numGroups;  // possible truncation error?

// #if POWER_TRAINING_DEBUG
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
    "AveOfMax = %d : ",AveOfMax));
// #endif

  //
  // slope = 65% * MINmargins (MAX((MAX(Margin[params] - UPMLimit),0)/PWRLimit - UPMLimit)
  //
  slope = slopeFactor;
  for (group = 0; group < numGroups; group++) {
    if (maxMargin[group] > maxMarginAllGroups) {
      maxMarginAllGroups = maxMargin[group];
    }
  }
  if ((maxMarginAllGroups - UPMLimit) >= 0) {

// #if POWER_TRAINING_DEBUG
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
      "maxMarginAllGroups = %d : ",maxMarginAllGroups));
// #endif

    slope = slope * (((maxMarginAllGroups - UPMLimit)*100)/(PWRLimit - UPMLimit));
  }

// #if POWER_TRAINING_DEBUG
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
    "slope = %d\n",slope));
// #endif

  //
  // NormalizedPower[param] = AveOfMax * (1 - Slope * (Power[param] - MinPower)/MaxPower))
  //
  for (setting = 0; setting < numSettings; setting++) {
    powerTrendLine[setting] = AveOfMax * (10000 - slope * (((powerLevel[setting] - minPower) * 100) / maxPower)/100)/10000;
  }
} // CalcPowerTrend

INT16
FindOptimalTradeOff(
  PSYSHOST host,
  UINT8    socket,
  INT16    averageMargins[MAX_GROUPS][MAX_ODT_SETTINGS],
  INT16    powerTrendLine[MAX_SETTINGS],
  UINT8    numSettings,
  UINT8    numGroups
)
//
// Required inputs: Margin data, Power Trendline
//
{
  UINT32  maxMargin[MAX_GROUPS];
  UINT32  maxPowerTrendLine;
  UINT32  values[MAX_GROUPS + 1][MAX_ODT_SETTINGS];
  UINT32  minValue[MAX_ODT_SETTINGS];
  UINT32  maxValue;
  UINT32  smallestValue;
  UINT8   group;
  UINT8   group2;
  UINT8   setting;
  UINT8   x;
  UINT32  total[MAX_ODT_SETTINGS];

  for (setting = 0; setting < numSettings; setting++) {
    minValue[setting] = 0xFFFF;
  }

  //
  // Get Maximum margins per group and maximum trendline
  //
  for (group = 0; group < MAX_GROUPS; group++) {
    maxMargin[group] = 0;
    if (group >= numGroups) {
      maxMargin[group] = 1;
    } else {
      for (setting = 0; setting < numSettings; setting++) {
        if (maxMargin[group] < (UINT32)averageMargins[group][setting]) {
          maxMargin[group] = (UINT32)averageMargins[group][setting];
        }
        if (minValue[setting] > ((UINT32)averageMargins[group][setting])) {
          minValue[setting] = (UINT32)averageMargins[group][setting];
        }
      } // setting
    }
  } // group

  maxPowerTrendLine = 0;
  for (setting = 0; setting < numSettings; setting++) {
    if (maxPowerTrendLine < (UINT32)powerTrendLine[setting]) {
      maxPowerTrendLine = (UINT32)powerTrendLine[setting];
    }
    if (minValue[setting] > (UINT32)powerTrendLine[setting]) {
      minValue[setting] = (UINT32)powerTrendLine[setting];
    }
  } // setting

  //
  // Generate value array based on margin * max of other margins * powertrendline per setting
  //
  for (setting = 0; setting < numSettings; setting++) {
    for (group = 0; group < MAX_GROUPS; group++) {
      if (group >= numGroups) {
        values[group][setting] = 0;
      } else {
        values[group][setting] = averageMargins[group][setting];
        for (group2 = 0; group2 < numGroups ; group2++) {
          if (group != group2) {
            values[group][setting] = values[group][setting] * maxMargin[group2];
          }
        }
        values[group][setting] = values[group][setting] * maxPowerTrendLine;
      }
    } // group
    //
    // Calculate value for powerTrendline
    //
    values[MAX_GROUPS][setting] = powerTrendLine[setting];
    for (group = 0; group < numGroups ; group++) {
      values[MAX_GROUPS][setting] = values[MAX_GROUPS][setting] * maxMargin[group];
    } // group
  } // setting

  maxValue = 0;
  for (setting = 0; setting < numSettings; setting++) {
    total[setting] = 1;
    for (x = 0; x < numGroups + 1; x++) {
      total[setting] = total[setting] * minValue[setting];
    }
    for (group = 0; group < MAX_GROUPS + 1; group++) {
      total[setting] = total[setting] + values[group][setting];
    }
    if (total[setting] > maxValue) {
      maxValue = total[setting];
    }
  } // setting

#if POWER_TRAINING_DEBUG
  if (numGroups > 3 ){
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
      "Param\tV0\tV1\tV2\tV3\tT Line\tV0*Max1*TL\tV1*Max0*TL\tV2*Max3\t\tV3*Max2\t\tTL*Max0*Max1\tMin^n V\t\tTotal\t\tTotal/Max\n"));
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
      "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"));
    for (setting = 0; setting < numSettings; setting++) {
      if (maxValue == 0){     // Divide by 0 error
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "maxValue is 0...Divide by 0!!\n"));
          continue;
      }
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        "%u\t%u\t%u\t%u\t%u\t%u\t%9u\t%9u\t%9u\t%9u\t%9u\t%9u\t%9u\t%u\n",
        setting,
        averageMargins[0][setting],
        averageMargins[1][setting],
        averageMargins[2][setting],
        averageMargins[3][setting],
        powerTrendLine[setting],
        values[0][setting],
        values[1][setting],
        values[2][setting],
        values[3][setting],
        values[MAX_GROUPS][setting],
        total[setting] - values[0][setting] - values[1][setting] - values[2][setting] - values[3][setting] - values[4][setting],
        total[setting],
        total[setting] / (maxValue / 100)
      ));
    }
  } else {
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
      "Param\tGV0\tGV1\tT Line\tGV0*Max1*TL\tGV1*Max0*TL\tTL*Max0*Max1\tMin^3 V\t\tTotal\t\tTotal/Max\n"));
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
      "--------------------------------------------------------------------------------------------------------------------------\n"));
    for (setting = 0; setting < numSettings; setting++) {
      if (maxValue == 0){     // Divide by 0 error check
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "maxValue is 0...Divide by 0!!\n"));
          continue;
      }
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        "%u\t%u\t%u\t%u\t%9u\t%9u\t%9u\t%9u\t%9u\t%u\n",
        setting,
        averageMargins[0][setting],
        averageMargins[1][setting],
        powerTrendLine[setting],
        values[0][setting],
        values[1][setting],
        values[MAX_GROUPS][setting],
        total[setting] - values[0][setting] - values[1][setting] - values[4][setting],
        total[setting],
        total[setting] / (maxValue / 100)
      ));
    }
  }
#endif

  //
  // If it's the biggest, it's the best and GTFO; if the smallest value is >= PERCENTAGE_TOLERANCE(95) at the end of the check, use the default value and exit
  //
  smallestValue = 100;
  for (setting = 0; setting < numSettings; setting++) {
    //
    // Check to see if the smallest is >= PERCENTAGE_TOLERANCE(95)
    //
    if ((total[setting]/ (maxValue/100)) < smallestValue) {
      smallestValue = (total[setting]/ (maxValue/100));
    }
    if (smallestValue >= PERCENTAGE_TOLERANCE && setting == (numSettings-1)) {
      //
      // The default values for all functions were stored at the beginning of TrainDDROptParam
      // Empirically demonstrated that there is little impact in margins from 95 to 100
      //
      return POWERTRAINING_DEFAULT_SETTING;
    } else continue;
  } // setting

  //
  // Return the optimal index
  //
  for (setting = 0; setting < numSettings; setting++) {
    if (total[setting] / maxValue) {
      return setting;
    }
  } // setting
  return POWERTRAINING_DEFAULT_SETTING;
}
