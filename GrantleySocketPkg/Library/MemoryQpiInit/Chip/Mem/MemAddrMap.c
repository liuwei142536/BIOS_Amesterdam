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
#define rcPrintf APPprint
void APPprint (PSYSHOST host, const char *format, ...);
#endif

//
// Local Prototypes
//
UINT32 SocketInterleave(PSYSHOST host, UINT16 TOLM);
UINT32 SocketNonInterleave(PSYSHOST host, UINT16 TOLM);
UINT32 RankInterleave(PSYSHOST host);
void   DisplayMemoryMapInfo(PSYSHOST host);
void   DisplaySystemInfo(PSYSHOST host);
void   PrintSize(PSYSHOST host, UINT16 size);
void   PopulateMemorySizeFields(PSYSHOST host);

//
// Disable warning for unused input parameters
//
#pragma warning(disable : 4100)
// -----------------------------------------------------------------------------
//
#if 0
UINT32
TADOptimize (
  PSYSHOST host,
  UINT16   TOLM,
  UINT8    Optimize,
  UINT8    *interNum,
  UINT16   *interSize,
  UINT32   TADLimit
  )
/*++

  Optimize TAD interleave
  @param host      - Pointer to sysHost
  @param TOLM      - Top of low memory
  @param Optimize  - Optimization level
  @param interNum  - Number of interleave ways
  @param interSize - Size of interleave for each "way"
  @param TADLimit  - The limit of the current TAD


**/
{
  int             i;
  int             j;
  int             si;
  UINT8           socket;
  UINT8           ch;
  UINT8           removed;
  UINT8           SADIndex;
  UINT8           TADIndex[2] = { 0, 0 };
  UINT8           count;
  UINT8           sortNum;
  UINT8           socket0Count;
  UINT8           socket1Count;
  UINT8           socket0SI;
  UINT8           socket1SI;
  UINT8           keepGoing;
  UINT8           mode;
  UINT16          previousSADLimit;
  UINT16          chMemUsed;
  UINT16          TOLMTemp;
  struct socketSort socketSortItem;
  struct socketSort socketSortList[MAX_SOCKET];
  removed           = 0;
  SADIndex          = 0;
  socket0SI           = 0;
  socket1SI           = 0;
  previousSADLimit  = 0;

  struct channelNvram (*channelNvList)[MAX_CH];
  struct ddrChannel (*channelList)[MAX_CH];
  struct imcNode (*socketList)[MAX_SOCKET];

  //
  // TAD index for 3:1
  //
  if (((interNum == 3) || (interNum == 6)) &&
      ((sortList[0].socketNum == (socket + 1)) || (sortList[1].socketNum == (socket + 1)))
      ) {
    host->imc[socket].TAD[TADIndex[socket]].Enable  = 1;
    host->imc[socket].intList[TADIndex[socket]]     = 0x03210321;
    count = MAD_RULES;
  }
  //
  // TAD index either 2:1 or 1:1, calculate dynamically
  //
  else {
    count = 0;
    host->imc[socket].intList[TADIndex[socket]] = 0;
    for (i = 0; (i < 4) && (i < interNum); i++) {
      if (sortList[i].socketNum == (socket + 1)) {
        host->imc[socket].TAD[TADIndex[socket]].Enable = 1;
        host->imc[socket].intList[TADIndex[socket]] |= (sortList[i].chNum << i * 4);
      }

      count++;
    }
  }
  //
  // Merge this TAD rule with the previous TAD rule if they are both disabled
  //
  if (host->imc[socket].TAD[TADIndex[socket] - 1].Enable == 0) {
    host->imc[socket].TAD[TADIndex[socket] - 1].Limit = host->SAD[SADIndex].Limit;
  }
  //
  // Go to next TAD rule if this one was consumed
  //
  else {
    TADIndex[socket]++;
  }
}
}
#endif // 0
// -----------------------------------------------------------------------------
// TAD (Channel) Sort
//
// Description:
//    Computes a list of channels sorted by size, largest to smallest.
//
// Returns:
//    sortNum: Number of channels with memory remaining to be allocated.
//            (sortNum may be 0 indicating all memory allocated)
//
// -----------------------------------------------------------------------------
//
UINT8
TADSort(PSYSHOST host,
               struct channelSort chSortList[],
               struct socketNvram *socketNvram,
               struct Socket *socketVar,
               UINT8 ha,
               UINT8 socket)

{
  UINT8 i;
  UINT8 si;
  UINT8 ch;
  UINT8 sortNum;
  struct channelNvram *channelNvList;
  struct ddrChannel *channelList;
  UINT8 channelListIndex;

  // Sort channels by size, largest first
  sortNum = 0;


  // Get channelLists for this ha

  channelListIndex = socketNvram->haList[ha].haNvChannelListStartIndex;

  for (ch = 0; ch < host->var.mem.numChPerHA; ch++) {

    channelNvList = &socketNvram->channelList[ch + channelListIndex];
    channelList = &socketVar->channelList[ch + channelListIndex];

    if (channelNvList->enabled == 0) continue;

    // Search for channels with remaining size
    if (channelList->remSize == 0) continue;

    // Determine sort list position
    for (si = 0; si < sortNum; si++) {
      if (chSortList[si].chSize < channelList->remSize) break;
    }

    // Shift smaller channels towards end of list
    if (si < sortNum) {
      for (i = sortNum; i > si; i--) chSortList[i] = chSortList[i - 1];
    }

    // Insert current channel
    chSortList[si].chNum = ch;
    chSortList[si].chSize = channelList->remSize;
    sortNum++;

  } // Channel Loop

  return sortNum;

}



//-----------------------------------------------------------------------------
// TAD (Channel) Interleave
//
// Description: Interleaves memory on the channel level within a socket. This
// routine figures out the mapping and fills in the structures TAD
// table.  It does not program any registers.
// assumed all SAD entries are sorted correctly at this point
//
//-----------------------------------------------------------------------------
UINT32 TADInterleave(PSYSHOST host, UINT16 TOLM, UINT8 Optimize)
{
  int i;
  int si;
  UINT8 socket;
  UINT8 ch;
  UINT8 interNum;
  UINT8 modInterNum;
  UINT8 SADInterNum;
  UINT8 ChWays;
  UINT8 SADIndex = 0;
  UINT8 TADIndex;
  UINT8 count;
  UINT8 sortNum;
  UINT8 target;
  UINT8 ha;
  UINT16 interSize;
  UINT32 remMem = 0;
  UINT32 TADLimit;
  UINT32 MaxLimit;
  UINT32 previousTADLimit = 0;
  UINT32 channelMemUsed;
  UINT32 HighAddressStart;
  UINT32 temp32;
  UINT64_STRUCT TADBase;
  UINT64_STRUCT temp64;
  int Offset = 0;
  struct memVar *mem;
  struct Socket *socketVar;
  struct socketNvram *socketNvram;
  struct memNvram *memNv;
  struct channelSort chSortList[MAX_CH];
  struct ddrChannel *channelList;
  struct haNvram *haNv;
  struct homeAgent *haMem;
  struct ddrChannel *haChannelList;
  UINT8 channelListIndex;
  UINT8 maxSADRules = MAX_SAD_RULES;
#ifdef NVMEM_FEATURE_EN
  UINT8 NVPass;
  UINT8 chTemp;
  UINT32 sysAddr;
  BOOLEAN TadNeg = 0;
#endif

  mem = &host->var.mem;
  memNv = &host->nvram.mem;

  // Compute TADs for each socket
  for (socket = 0; socket < MAX_SOCKET; socket++) {

    socketVar = &mem->socket[socket];
    socketNvram = &host->nvram.mem.socket[socket];

    // Skip socket if not enabled
    if (socketNvram->enabled == 0) {
      continue;
    }

    // Go to next socket if remsize is 0
    if (socketVar->remSize == 0) {
      continue;
    }

    for (ha = 0; ha < MAX_HA; ha++) {

      haNv = &socketNvram->haList[ha];
      if (haNv->enabled == 0) continue;

      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "TAD setup HA %d\n", ha));

      haMem = &socketVar->haList[ha];
      channelListIndex = haMem->haChannelListStartIndex;
      haChannelList = &socketVar->channelList[channelListIndex];

      // Compute TAD interleave for socket
      SADIndex = 0;

      previousTADLimit = 0;
      remMem = 0;
#ifdef NVMEM_FEATURE_EN
      NVPass = 0;
#endif

      for (TADIndex = 0; TADIndex < TAD_RULES; TADIndex++) {

        // Skip SADs that do not target current socket
        while (SADIndex < maxSADRules) {
          for (i = 0; i < mem->SAD[SADIndex].ways; i++) {
            if (mem->SADintList[SADIndex][i] == haNv->haNodeId){
              break;
            }
          }
          if ((i == mem->SAD[SADIndex].ways) && (mem->SAD[SADIndex].Enable !=0)) {
            previousTADLimit = mem->SAD[SADIndex].Limit;   // Must move TAD to skip address space covered by SAD
            SADIndex++;
          }
          else
            break;
        }

        sortNum = TADSort (host, chSortList, socketNvram, socketVar, ha, socket);

        // Determine largest N channels, where N:1 is the channel interleave
        if (sortNum) {

          // Should not reach end of SAD rules before allocating all memory, Issue warning and break TAD loop.
          // Only check if non-zero amount of memory left to be allocated as indicated by sortNum != 0.
          if (SADIndex >= maxSADRules) {
            MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "SAD rules exceeds %d\n", maxSADRules));
            OutputWarning(host, WARN_INTERLEAVE_FAILURE, WARN_SAD_RULES_EXCEEDED, socket, 0xFF, 0xFF, 0xFF);
            break;
          }

          SADInterNum = mem->SAD[SADIndex].ways;

          // TAD bound by SADlimit
          TADLimit = mem->SAD[SADIndex].Limit;

          //
          // Bound TADLimit by TOLM, if range crosses TOLM
          // TAD cannot span TOLM because need a TAD offset to avoid including memory hole in channel address space
          // Adjust for hole allocation below
          //

          HighAddressStart = host->var.mem.highMemBase;

          if ((previousTADLimit < TOLM) && (TADLimit >= TOLM)) {
            TADLimit = TOLM;
          }

          if (previousTADLimit == TOLM) {
            TADLimit = TADLimit - (host->var.mem.highMemBase - TOLM);
          }

          interNum = host->var.mem.chInter;

          // Compare N to remaining channels
          if (interNum > sortNum) {
            interNum = sortNum;
          }

#ifdef NVMEM_FEATURE_EN
          if ((host->setup.mem.interNVDIMMS == 0) && (NVPass == 1)) interNum = 1;
#endif

          //
          // Check for 3-way channel interleaving restrictions
          //
          if ((interNum == 3) && (host->nvram.mem.RASmodeEx & A7_MODE_EN)){
            //
            // If A7_MODE is enabled, 3-way CH interleave cannot be supported.
            // Per HSD 4987234, force A7 mode disabled and keep 3-way channel interleave
            host->nvram.mem.RASmodeEx &= ~A7_MODE_EN;
          }
          if ((interNum == 3) && (host->var.mem.InterleaveMode == 0)) {
            if (host->var.mem.InterleaveMode == 0){
              //Override the SAD XOR if 3-way ch interleaving found.
              //Regardless of setup option
              host->var.mem.InterleaveMode = 1;
              host->nvram.mem.socket[socket].disallowXorInterleave = 1;

            }
          }

          // Get size common to N channels
          interSize = chSortList[interNum - 1].chSize;

  //        TADOptimize (host, TOLM, Optimize, &interNum, &interSize, TADLimit);

          // Bound size by TADLimit
          if ( (SADInterNum * interNum * interSize + previousTADLimit) > TADLimit) {
            interSize = (UINT16)(TADLimit - previousTADLimit)/(interNum * SADInterNum);

            // Attempt to reduce fragmentation by ensuring remaining size is multiple of
            // (interNum * SADInternum * intersize).
            // Note TADOffset must be > 0 so should not reduce interNum when remaining mem
            // size is large (do not want to allocate memory block with a large interSize
            // and a small interNum followed by a block with a larger interNum => could produce
            // a TADOffest < 0)
            if (((UINT32)(TADLimit - previousTADLimit) < (UINT32)(interNum * SADInterNum)) && (interNum > 1)) {
              modInterNum = interNum;
              do {
                modInterNum--;
                if ((TADLimit - previousTADLimit) % (modInterNum * SADInterNum) == 0) {
                  interNum = modInterNum;
                  interSize = (UINT16)(TADLimit - previousTADLimit) / (interNum * SADInterNum);
                  if (interSize > chSortList[interNum - 1].chSize) {
                    interSize = chSortList[interNum - 1].chSize;
                  }
                  break;
                }
              } while (modInterNum > 1);

  /*            saveInterNum = interNum;
              do {
                modInterNum = (TADLimit - previousTADLimit) % (interNum * SADInterNum);
                if (modInterNum != 0) interNum--;
              } while (modInterNum != 0);

              // Recompute interSize, if interNum changes
              if (saveInterNum != interNum) {
                // Get size common to N channels
                interSize = chSortList[interNum - 1].chSize;
                if ( (SADInterNum * interNum * interSize + previousTADLimit) > TADLimit) {
                  interSize = (TADLimit - previousTADLimit)/(interNum * SADInterNum);
                }
              }
  */
            }
          }

          // TAD limit is in system address space: must multiply size by both channel and socket ways
          TADLimit = previousTADLimit + (interSize * interNum * SADInterNum);

          // Account for memory hole if previous TAD limit is TOLM (current TAD must account for memory hole)
          // Extend TAD limit by size of hole
          if (previousTADLimit == TOLM) {
            TADLimit += HighAddressStart - TOLM;
          }

          // Error check: TAD limit should not cross SADLimit
          if (TADLimit > mem->SAD[SADIndex].Limit) {
            MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "TAD > SAD Limit\n"));
            OutputWarning(host, WARN_INTERLEAVE_FAILURE, WARN_TAD_LIMIT_ERROR, socket, 0xFF, 0xFF, 0xFF);

            TADLimit = mem->SAD[SADIndex].Limit;
          }
          // Fill in TAD fields

          haMem->TAD[TADIndex].Limit = TADLimit;
          haMem->TAD[TADIndex].Enable = 1;
          haMem->TAD[TADIndex].SocketWays = mem->SAD[SADIndex].ways;
          haMem->TAD[TADIndex].ChannelWays = interNum;

          haMem->TAD[TADIndex].Mode = mem->InterleaveMode;    // Set mode

          // Initialize per channel  info: Interleave List, offset, channel index offset
          for (i = 0; i < host->var.mem.numChPerHA; i++) {
            haMem->TADintList[TADIndex][i] = 0;
            haMem->TADOffset[TADIndex][i] = 0;
            haMem->TADChnIndex[TADIndex][i] = 0;
          }

          // Fill in the first part of the Interleave List
          count = 0;
          for (i = 0; i < interNum; i++) {
            ch = chSortList[i].chNum;
            haMem->TADintList[TADIndex][i] = ch;

#ifdef NVMEM_FEATURE_EN
            if (NVPass == 0) {
              channelMemUsed = ((haChannelList[ch].memSize - haChannelList[ch].NVmemSize) - haChannelList[ch].remSize) + interSize;
              Offset = TADLimit - (haMem->TAD[TADIndex].SocketWays * interNum * channelMemUsed);
            }
            else {
              channelMemUsed = (haChannelList[ch].memSize - haChannelList[ch].NVmemSize) + (haChannelList[ch].NVmemSize - haChannelList[ch].remSize);
              TadNeg = 1;
              sysAddr = TADLimit - (haMem->TAD[TADIndex].SocketWays * interNum * interSize);
              Offset =  sysAddr - (haMem->TAD[TADIndex].SocketWays * interNum * channelMemUsed);
            }
#else
            // Compute TAD offset = system base address - (Socket_ways * Channel_ways * channel_Address)
            channelMemUsed = (haChannelList[ch].memSize - haChannelList[ch].remSize) + interSize;
            Offset = TADLimit - (haMem->TAD[TADIndex].SocketWays * interNum * channelMemUsed);
#endif
#ifdef NVMEM_FEATURE_EN
            if (Offset < 0 && !TadNeg) {
#else
            if (Offset < 0) {
#endif

              OutputWarning(host, WARN_INTERLEAVE_FAILURE, WARN_TAD_OFFSET_NEGATIVE, socket, 0xFF, 0xFF, 0xFF);
              TADIndex -= 1; // Last valid TAD rule
              break; // Should not occur;
            }

            haMem->TADOffset[TADIndex][ch] = Offset;
            //
            // if CH Ways != 3 then CHN_IDX_OFFSET = (TAD[N].BASE / TAD[N].TAD_SKT_WAY) % TAD[N].TAD_CH_WAY
            // else  CHN_IDX_OFFSET = (TAD[N].BASE) % TAD[N].TAD_CH_WAY
            //

            // Adjust TAD[N].BASE to account for MMIO hole, if necessary
            temp64.lo = (previousTADLimit == TOLM) ? HighAddressStart : previousTADLimit;
            if (haMem->TAD[TADIndex].ChannelWays == 3) {
              temp64.lo = temp64.lo + (( socket % haMem->TAD[TADIndex].SocketWays) * 0x40);
            }
            temp64.hi = 0;

            // Compute full 46-bit TADBase address from 64MB granularity Limit (shift left 26)
            TADBase = temp64;
            haMem->TAD[TADIndex].ChannelAddressLow = TADBase.lo;
            haMem->TAD[TADIndex].ChannelAddressHigh = TADBase.hi;

            // Do 64-bit division
            if (haMem->TAD[TADIndex].ChannelWays != 3) {
              temp64 = DivUINT64x32(TADBase, haMem->TAD[TADIndex].SocketWays, &temp32);
            } else {
              temp64 = DivUINT64x32(TADBase, 1, &temp32);
            }

            // 64-bit Mod -> temp32
            temp64 = DivUINT64x32(temp64, haMem->TAD[TADIndex].ChannelWays, &temp32);

            haMem->TADChnIndex[TADIndex][ch] = (UINT8)temp32;
            count++;    // Keep track of how many entries have been filled
          }

          // Handle negative Offset case by exiting TADIndex loop
#ifdef NVMEM_FEATURE_EN
          if (Offset < 0 && !TadNeg) break;
#else
          if (Offset < 0) break;
#endif

          // Save current TAD Limit
          previousTADLimit = TADLimit;

          // Check if current SAD completed
          if (haMem->TAD[TADIndex].Limit == mem->SAD[SADIndex].Limit) {
            SADIndex++;
          }

          // Subtract common size from N channels
          for (si = 0; si < interNum; si++) {
            channelList = &haChannelList[chSortList[si].chNum];
            channelList->remSize = channelList->remSize - interSize;
          }  // end  for (si
        } else { // if sortNum
#ifdef NVMEM_FEATURE_EN
          if(NVPass != 0) break;
          if(!haMem->NVmemSize) break;

          NVPass = 1;
          TADIndex--;
          haMem->remSize = haMem->NVmemSize;
          for (chTemp = 0; chTemp < host->var.mem.numChPerHA; chTemp++) {
            socketVar->channelList[chTemp + channelListIndex].remSize = socketVar->channelList[chTemp + channelListIndex].NVmemSize;
          } // chTemp loop
#else
          break;

#endif  //NVMEM_FEATURE_EN
        }
      } // for TAD Index

      // Issue warning if all TAD rules used before all memory allocated
      if (TADIndex >= TAD_RULES) {
        MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "TAD rules exeeds %d\n", TAD_RULES));
        OutputWarning(host, WARN_INTERLEAVE_FAILURE, WARN_TAD_RULES_EXCEEDED, socket, 0xFF, 0xFF, 0xFF);
      } else if (TADIndex != 0) {
        // Fill out the remaining entries
        MaxLimit = haMem->TAD[TADIndex - 1].Limit;

        for(TADIndex; TADIndex < TAD_RULES; TADIndex++) {
          haMem->TAD[TADIndex].Limit = MaxLimit;
          haMem->TAD[TADIndex].Enable = 0;
          // Fill in the rest of the Interleave List
          for (i = 0; i < MAX_CH; i++) {
            haMem->TADintList[TADIndex][i] = 0;
            haMem->TADOffset[TADIndex][i] = 0;
            haMem->TADChnIndex[TADIndex][i] = 0;
          }
        }
      }

      // Fill in the the Interleave, offset and ch idx list for unspecified channels
      // Note for Channel+Lockstep we do ch0->ch1, ch0->ch2, ch1->ch3 =>
      // all channels will have the same programming.
      for (TADIndex = 0; TADIndex < TAD_RULES; TADIndex++) {
        ChWays = haMem->TAD[TADIndex].ChannelWays;
        for (i = 0; i < ChWays; i++) {
          if (memNv->RASmode & CH_LOCKSTEP) {
            // Copy ch0 -> ch1, ch2 -> ch3
            target = haMem->TADintList[TADIndex][i];
            if (target == 0 || target == 2) {
              haMem->TADOffset[TADIndex][target+1] = haMem->TADOffset[TADIndex][target];
              haMem->TADChnIndex[TADIndex][target+1] = haMem->TADChnIndex[TADIndex][target];
            }
          } // if CH_LOCKSTEP

          if (memNv->RASmode & CH_MIRROR) {
            // Copy ch0 -> ch1, ch2 -> ch3
            target = haMem->TADintList[TADIndex][i];
            if (target == 0 || target == 2) {
              // Note: for mirror mode the interleave list includes the secondary mirror channel
              // in the target list (even though ChannelWays only counts the primary (!)
              haMem->TADintList[TADIndex][i+ChWays] = target + 1;
              haMem->TADOffset[TADIndex][target+1] = haMem->TADOffset[TADIndex][target];
              haMem->TADChnIndex[TADIndex][target+1] = haMem->TADChnIndex[TADIndex][target];
            }
          } // if CH_MIRROR

        } // for channel
      } // for TADIndex
    } // for ha
  } // for socket
  return SUCCESS;
} // TADInterleave

#if MAX_SOCKET > 1
UINT32
SADOptimize (
  PSYSHOST host,
  UINT16   TOLM,
  UINT8    Optimize,
  UINT8    *interNum,
  UINT16   *interSize
  )
/*++

  Optimize SAD interleave

  @param host      - Pointer to sysHost
  @param TOLM      - Top of low memory
  @param Optimize  - Optimization level
  @param interNum  - Number of interleave ways
  @param interSize - Size of interleave for each "way"

  @retval SUCCESS

**/
{
  //
  // UINT8 SADIndex = 0;
  // UINT8 TADIndex[2] = {0, 0};
  // Sort channels again using the following priorities:
  // 1. Channels with remaining size == interSize ensures maximum memory allocation
  //
  return SUCCESS;
} // SADOptimize

UINT8
SADSort(PSYSHOST host,
        struct socketSort socketSortList[])
/*++

  Computes a list of sockets sorted by size, largest to smallest.
  Used for single HA SKU or pseudo-hemisphere mode.

  @param host          - Pointer to sysHost
  @param socketSortList  - Node sort list

  @retval sortNum - Number of sockets with memory remaining to be allocated
                    (sortNum may be 0 indicating all memory allocated)

--*/
{
  UINT8 ha;
  UINT8 i;
  UINT8 si;
  UINT8 socket;
  UINT8 sortNum;
  UINT16 remSize;

  // Sort sockets by size, largest first
  sortNum = 0;
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    remSize = host->var.mem.socket[socket].remSize;

    if (host->nvram.mem.socket[socket].enabled == 0) continue;

    // Search for sockets with remaining size
    for (ha = 0; ha < MAX_HA; ha++) {

      if (host->nvram.mem.socket[socket].haList[ha].enabled == 0) continue;

      if (remSize == 0) continue;
      // Determine sort list position
      for (si = 0; si < sortNum; si++) {
        if (socketSortList[si].socketSize < remSize){
          break;
        }
      }

      // Shift smaller sockets towards end of list
      if (si < sortNum) {
        for (i = sortNum; i > si; i--) socketSortList[i] = socketSortList[i - 1];
      }

    // Insert current socket
      socketSortList[si].socketNum = socket;
      socketSortList[si].haNum = host->nvram.mem.socket[socket].haList[ha].haNum;
      socketSortList[si].haNodeId = host->nvram.mem.socket[socket].haList[ha].haNodeId;
      socketSortList[si].socketSize = remSize;
      sortNum++;
    } //ha loop
  } // socket Loop

  return sortNum;
}



UINT8
SADSort2HA (PSYSHOST host,
            struct socketSort socketSortList[])
/*++

  Computes a list of HA sockets sorted by size, largest to smallest.
  Used for interleave across multiple HAs per socket. Not used for
  pseudo-hemisphere mode.

  @param host          - Pointer to sysHost
  @param socketSortList  - Node sort list

  @retval sortNum - Number of sockets with memory remaining to be allocated
                    (sortNum may be 0 indicating all memory allocated)

--*/
{
  UINT8 i;
  UINT8 si;
  UINT8 socket;
  UINT8 sortNum;
  UINT8 ha;
  UINT16 remSize;

  // Sort sockets by size, largest first
  sortNum = 0;
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    remSize = host->var.mem.socket[socket].remSize;

    if (host->nvram.mem.socket[socket].enabled == 0) continue;

    // Search for ha sockets with remaining size

    for (ha = 0; ha < MAX_HA; ha++) {

      if (host->nvram.mem.socket[socket].haList[ha].enabled == 0) continue;

      remSize = host->var.mem.socket[socket].haList[ha].remSize;

      if (remSize == 0) continue;

      // Determine sort list position
      for (si = 0; si < sortNum; si++) {
        if (socketSortList[si].socketSize < remSize)
          break;
      }

      // Shift smaller sockets towards end of list
      if (si < sortNum) {
        for (i = sortNum; i > si; i--) socketSortList[i] = socketSortList[i - 1];
      }

      // Insert current socket
      socketSortList[si].socketNum = socket;
      socketSortList[si].haNum = ha;
      socketSortList[si].haNodeId = host->nvram.mem.socket[socket].haList[ha].haNodeId;
      socketSortList[si].socketSize = remSize;
      sortNum++;
    } // ha loop
  } // socket Loop

  return sortNum;
} // SADSort2HA



UINT32
SADInterleave (
  PSYSHOST  host,
  UINT16    TOLM,
  UINT8     Optimize
  )
/*++

  Interleaves memory across sockets. This routine figures out the mapping and
  fills in the structures for the SAD tables.  It does not program any
  registers.

  @param host      - Pointer to sysHost
  @param TOLM      - Top of low memory
  @param Optimize  - Optimization level

  @retval SUCCESS

--*/
{
  INT32             j;
  INT32             si;
  UINT8             interNum;
  UINT8             SADIndex = 0;
  UINT8             sortNum;
  UINT8             i;
  UINT8             i0 = 0;
  UINT8             socket;
  UINT8             ha;
  UINT16            interSize;
  UINT32            SADLimit = 0;
  UINT32            previousSADLimit = 0;
  UINT32            HighAddressStart;
  UINT8             maxSADRules = MAX_SAD_RULES;
  struct socketSort socketSortList[MAX_SOCKET*MAX_HA];
  struct Socket     *socketVar;
  struct memVar     *mem;
  struct homeAgent  *haMem;
#ifdef NVMEM_FEATURE_EN
  int               NVPass;
  UINT8             haTemp;
  UINT8             nodeTemp;

  NVPass = 0;
#endif // NVMEM_FEATURE_EN

  mem = &host->var.mem;

  for (SADIndex = 0; SADIndex < maxSADRules; SADIndex++) {

    i0 = 0; // Track index for socket 0

    interNum = host->var.mem.socketInter;

    // SAD interleave on socket or HA socket

    if (host->var.common.numActiveHAperSocket == 1) {
      sortNum = SADSort (host, socketSortList);
    } else {
      sortNum = SADSort2HA (host, socketSortList);
      interNum = interNum * host->var.mem.socketInter;
    }

#ifdef NVMEM_FEATURE_EN
    if ((host->setup.mem.interNVDIMMS == 0) && (NVPass == 1)) interNum = 1;
#endif

    // Determine largest N sockets, where N:1 is the socket interleave
    if (sortNum) {

      // Compare N to remaining sockets
      while (interNum > sortNum) {
        interNum >>= 1;
      }

      // Get size common to N sockets
      interSize = socketSortList[interNum - 1].socketSize;

      // SADOptimize (host, TOLM, Optimize, &interNum, &interSize);

      // Increment SAD.Limit by interNum * interSize
      SADLimit = (interNum * interSize) + previousSADLimit;

      HighAddressStart = host->var.mem.highMemBase;

      // Adjust SAD Limit for low memory hole, if necessary
      if ((previousSADLimit <= TOLM) && (SADLimit > TOLM) && (host->var.mem.memSize > TOLM)) {

        // Add low Gap to SAD Limit
        SADLimit += HighAddressStart - TOLM;
      }

      // Enforce system address remap limit - this also limits the TAD Limit

      if (SADLimit > host->var.common.tohmLimit) {
        interSize = (UINT16)(host->var.common.tohmLimit - previousSADLimit) / interNum;
        SADLimit = (interNum * interSize) + previousSADLimit;
        // Indicate memory size limited
        MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "Warning: TOHM limit exceeded\n"));
        LogWarning(host, WARN_MEM_LIMIT, 0, (UINT32)-1);
      }

      // Fill in SAD entry fields
      mem->SAD[SADIndex].Limit = SADLimit;
      mem->SAD[SADIndex].Enable = 1;                    // Enable this entry
      mem->SAD[SADIndex].Mode = mem->InterleaveMode;    // Set Interleave Mode

      mem->SAD[SADIndex].ways = interNum;
      // Fill in the first part of the Interleave List
      for (i = 0; i < interNum; i++) {
        mem->SADintList[SADIndex][i] = socketSortList[i].haNodeId;
        if ((socketSortList[i].socketNum == 0) && (socketSortList[i].haNodeId == 0)) {
          i0 = i; // Track index for socket 0
        }
      }

      // Flip socket 0 with socket i0 to put socket 0 first in interleave list
      if (i0 != 0) {
        mem->SADintList[SADIndex][i0] = mem->SADintList[SADIndex][0];
        mem->SADintList[SADIndex][0] = 0;
      }

      // Fill in the rest of the Interleave List
      for (i = interNum; i < MAX_SAD_WAYS; i = i + interNum) {
        for (j = 0; j < interNum; j++) {
          mem->SADintList[SADIndex][i+j] = mem->SADintList[SADIndex][j];
        }
      }

      // Save current SAD Limit
      previousSADLimit = SADLimit;

      // Subtract common size from N sockets for socket and ha socket
      for (si = 0; si < interNum; si++) {
        socket = socketSortList[si].socketNum;
        ha = socketSortList[si].haNum;
        socketVar = &mem->socket[socket];
        haMem = &mem->socket[socket].haList[ha];
        // Update socket remSize
        socketVar->remSize = socketVar->remSize - interSize;
        // Update ha socket remSize
        haMem->remSize = haMem->remSize - interSize;
      } // for
#ifdef NVMEM_FEATURE_EN
    } else {
      if (NVPass != 0) break; // if sortNum, exit SADIndex loop if all memory allocated
      if (!mem->NVmemSize) break;
      NVPass = 1;
      SADIndex--;
      // Switch to NVSize
      for (nodeTemp = 0; nodeTemp < MAX_SOCKET; nodeTemp++) {
        socketVar = &mem->socket[nodeTemp];
        socketVar->remSize = socketVar->NVmemSize;
        for (haTemp = 0; haTemp < MAX_HA; haTemp++) {
          haMem = &socketVar->haList[haTemp];
          haMem->remSize = haMem->NVmemSize;
        } // haTemp loop
      } // nodeTemp loop
    }
#else
    } else break; // if sortNum, exit SADIndex loop if all memory allocated
#endif // NVMEM_FEATURE_EN
  } // for SAD Index

  // Issue warning if all SAD rules used before all memory allocated
  if (SADIndex >= maxSADRules) {
    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Warning: SAD rules exceeds %d\n", maxSADRules));
    LogWarning(host, WARN_INTERLEAVE_FAILURE, WARN_SAD_RULES_EXCEEDED, (UINT32)-1);
  } else {
    // Fill in the remaining SAD rules
    for (SADIndex; SADIndex < maxSADRules; SADIndex++) {
      if (SADIndex != 0) {
        mem->SAD[SADIndex].Enable = 0;
        mem->SAD[SADIndex].Limit = mem->SAD[SADIndex - 1].Limit;
        for (j = 0; j < MAX_SAD_WAYS; j++) {
          mem->SADintList[SADIndex][j] = 0;
        }
      }
    }
  }

  // Restore socket remSize for socekt / ha
  for (i = 0; i < MAX_SOCKET; i++) {
    socketVar = &mem->socket[i];
    socketVar->remSize = socketVar->memSize;
    for (ha = 0; ha < MAX_HA; ha++) {
      haMem = &socketVar->haList[ha];
      haMem->remSize = haMem->memSize;
    }
  }

  return SUCCESS;
} // SADInterleave

UINT32 SocketInterleave(PSYSHOST host, UINT16 TOLM)
/*++

  Interleaves memory on the channel level across sockets. This
  routine figures out the mapping and fills in the structures for the SAD, TAD,
  and SAG tables.  It does not program any registers.

  @param host      - Pointer to sysHost
  @param TOLM      - Top of low memory

  @retval SUCCESS

--*/
{
  UINT32 status;
  UINT8 Optimize = 2;  // Enable 1 way optimization
  //
  // Default to XOR mode 0 for UMA.
  //
  host->var.mem.InterleaveMode = 0;


  OutputExtendedCheckpoint((host, STS_DDR_MEMMAP, SUB_SAD_INTERLEAVE, 0));
  SADInterleave(host, TOLM, Optimize);

  OutputExtendedCheckpoint((host, STS_DDR_MEMMAP, SUB_TAD_INTERLEAVE, 0));
  status = TADInterleave(host, TOLM, Optimize);

  return status;

}
#endif // MAX_SOCKET > 1




UINT32 SADSplitBelow4GB1HA (
  PSYSHOST host,
  UINT16 TOLM,
  UINT8* SADIndexArg,
  UINT32 *previousSADLimitArg)

/*++

  Creates SADs for memory below 4GB for single HA (or pseudo-hemisphere) case.
  It does not program any registers.

  @param host                - Pointer to sysHost
  @param TOLM                - Top of low memory
  @param SADIndexArg         - Index into the current SAD
  @param previousSADLimitArg - SAD limit for the previous SAD entry

  @retval SUCCESS

--*/
{
  UINT8 socket;
  UINT8 i;
  UINT8 haIndex;
  UINT8 ha;
  UINT8 SADIndex = *SADIndexArg;
  UINT8 socketCount;
  UINT32 limit;
  UINT32 previousSADLimit = *previousSADLimitArg;
  struct memVar *mem;
  struct homeAgent *haMem;
  struct SADTable *SADEntry;
  UINT8 (*SADList)[MAX_SAD_WAYS];
  UINT8 maxSADRules = MAX_SAD_RULES;

  mem = &host->var.mem;

  // Count number of sockets populated with memory
  socketCount = 0;
  for (socket = 0; socket < MAX_SOCKET; socket++) {

    if (host->nvram.mem.socket[socket].enabled == 0)
      continue;

    if (host->var.mem.socket[socket].memSize != 0) {
      socketCount++;
    }
  }

  // Evenly distribute low memory across sockets
  
  if (socketCount) {	// (This is check added to resolve KWID : 5546)
	  limit = TOLM / socketCount;
  }
  else {
	  limit = TOLM;
  }

  // Build SADs for each socket. SADlimit = min (socket memSize, limit) + previousSADLimit

  previousSADLimit = 0;

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    // No SAD needed for sockets with 0 memory
    if (host->nvram.mem.socket[socket].enabled == 0)
      continue;
    if (mem->socket[socket].memSize == 0)
      continue;

    // find active HA
    ha       = 0;
    for (haIndex = 0; haIndex < MAX_HA; haIndex++) {
      haMem = &mem->socket[socket].haList[haIndex];
      if (haMem->memSize != 0) {
        ha = haIndex;
        break;
      }//if Ha enable
    } // ha

    SADEntry = &mem->SAD[SADIndex];

    if (mem->socket[socket].memSize > limit) {
      SADEntry->Limit = previousSADLimit + limit;
    }
    else {
      SADEntry->Limit = previousSADLimit + mem->socket[socket].memSize;
    }

    // Decrement socket remSize by amount of memory allocated
    mem->socket[socket].remSize = (UINT16)(mem->socket[socket].remSize - (SADEntry->Limit - previousSADLimit));

    previousSADLimit = SADEntry->Limit;

    SADEntry->Enable = 1;
    SADEntry->ways = 1;
    SADEntry->Mode = mem->InterleaveMode;

    SADList = &mem->SADintList[SADIndex];

    for (i = 0; i < MAX_SAD_WAYS; i++) {
      (*SADList)[i] = host->nvram.mem.socket[socket].haList[ha].haNodeId;
    }
    SADIndex++;

    // Issue warning if all SAD rules used before all sockets allocated
    if (SADIndex >= maxSADRules) {
      MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "SAD rules exceeds %d\n", maxSADRules));
      OutputWarning(host, WARN_INTERLEAVE_FAILURE, WARN_SAD_RULES_EXCEEDED, socket, 0xFF, 0xFF, 0xFF);
      break;
    }
  } // for socket

  *previousSADLimitArg = previousSADLimit;
  *SADIndexArg = SADIndex;

  return SUCCESS;

} // SADSplitBelow4GB1HA



UINT32
SADNuma1HA (
  PSYSHOST host,
  UINT16 TOLM,
  UINT8 Optimize)
/*++

  Interleaves memory on channel level but not across HA or sockets.
  This routine figures out the mapping and fills in the structures for the SAD and TAD
  tables.  It does not program any registers.

  @param host                - Pointer to sysHost
  @param TOLM                - Top of low memory
  @param SADIndexArg         - Index into the current SAD
  @param previousSADLimitArg - SAD limit for the previous SAD entry

  @retval SUCCESS

--*/
{
  UINT8 socket;
  UINT8 j;
  UINT8 haIndex;
  UINT8 ha;
  UINT8 SADIndex;
  UINT32 MaxLimit = 0;
  UINT32 previousSADLimit = 0;
  struct socketNvram *socketNv;
  struct Socket *socketVar;
  struct memVar *mem;
  struct homeAgent *haMem;
  UINT32 HighAddressStart;
  UINT8 maxSADRules = MAX_SAD_RULES;
  UINT8 counterAddRange = 0;
  UINT8 iotLoop;
  UINT8 iotLoopCounter;
  UINT32 iotLimit;

  // Initialize SAD rule number
  SADIndex = 0;
  mem = &host->var.mem;
  mem->socketInter = 1;
  ha       = 0;

#if (MAX_SOCKET > 1 && MAX_SOCKET <= 4)
  if (((host->var.mem.memSize) > TOLM) &&
       (host->setup.mem.options & SPLIT_BELOW_4GB_EN)) { // More than 4GB split?

    SADSplitBelow4GB1HA (host, TOLM, &SADIndex, &previousSADLimit);
  } // if SPLIT 4GB
#endif //MAX_SOCKET > 1

  //Disables IOT for socket with IOT Mem > memSize
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    socketNv = &host->nvram.mem.socket[socket];
    socketVar = &mem->socket[socket];

    if ((socketNv->enabled == 0) || (socketVar->memSize == 0)) continue; //Reqd. as IOT will always be > memSize when a socket is not enabled
    host->var.mem.iotDisabled[socket] = 0;
    if (host->setup.mem.iotMemBufferRsvtn > host->var.mem.socket[socket].memSize) {
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Warning: IOT disabled! Size > available memory S%d\n", socket));
      host->var.mem.iotDisabled[socket] = 1;
    }
  }

  //Enabling/Disabling IOT
  if (host->setup.mem.iotMemBufferRsvtn != 0) { //Enabled, loop 2 times around socket loop
    iotLoop = 2;
  } else {
    iotLoop = 1; //Disabled, loop 1 time around socket loop
  }

//If IOT is enabled, allocate it memory only on the 2nd iteration of iotLoopCounter
  for (iotLoopCounter = 0; iotLoopCounter < iotLoop; iotLoopCounter ++) {
    // Create an entry for remaining memory on each socket
    for (socket = 0; socket < MAX_SOCKET; socket++) {

      //If IOT > memSize of a socket, then skip IOT for that socket only during the 2nd iteration
      if ((host->var.mem.iotDisabled[socket] == 1) && (iotLoopCounter == 1)) continue;

      // find active HA
      ha       = 0;
      for (haIndex = 0; haIndex < MAX_HA; haIndex++) {
        haMem = &mem->socket[socket].haList[haIndex];
        if (haMem->memSize != 0) {
          ha = haIndex;
          break;
        }//if Ha enable
      } // ha

      socketNv = &host->nvram.mem.socket[socket];
      socketVar = &mem->socket[socket];

      //
      // Skip if socket not enabled or no memory for socket
      //
      if (socketNv->enabled == 0) continue;
      if (socketVar->memSize == 0) continue;

      if ((iotLoop == 2) && (iotLoopCounter == 0) && (host->var.mem.iotDisabled[socket] != 1)) {      //IOT enabled, on first loop reserve memory on each socket
        socketVar->remSize = socketVar->remSize - host->setup.mem.iotMemBufferRsvtn; // remHAsize - IOT Value
      }

      //
      //After socket loop has run once, allocate the remaining Socket memory to the IOT buffer
      //Now, create a new SAD rule for the IOT buffer per socket as we enter the socket loop for the second time
      //
      if (iotLoopCounter == 0) {                              // All non-IOT SAD rules, created in first loop iteration
        mem->SAD[SADIndex].IotEnabled = 0;
      } else {                                                // Second time when we enter the socket loop all IOT rules created
        mem->SAD[SADIndex].IotEnabled = 1;
        socketVar->remSize = host->setup.mem.iotMemBufferRsvtn;    //  Add previously reserved IOT size to the remSize for the socket
      }
#ifdef NVMEM_FEATURE_EN
      // NVDIMM Since we aren't interleaving sockets, set remSize to MemSize
      socketVar->remSize += socketVar->NVmemSize;
#endif

      // Allocate remaining socket memory with next SAD
      MaxLimit = previousSADLimit + socketVar->remSize;

      // Adjust SAD Limit for memory hole, if necessary
      if ((previousSADLimit <= TOLM) && (MaxLimit > TOLM) && (host->var.mem.memSize > TOLM)) {

        HighAddressStart = host->var.mem.highMemBase;

        MaxLimit += HighAddressStart - TOLM;            // Add Gap to SAD Limit
      }

      // Enforce system address remap limit - this also limits the TAD Limit

      if (MaxLimit > host->var.common.tohmLimit) {
        MaxLimit = host->var.common.tohmLimit;
        // Indicate memory size limited
        MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "Warning: TOHM exceeded\n"));
        LogWarning(host, WARN_MEM_LIMIT, 0, (UINT32)-1);
      }

      mem->SAD[SADIndex].Limit = MaxLimit;
      previousSADLimit = mem->SAD[SADIndex].Limit;
      mem->SAD[SADIndex].Enable = 1;                    // Enable this entry
      mem->SAD[SADIndex].Mode = mem->InterleaveMode;    // Set Interleave Mode
      mem->SAD[SADIndex].ways = 1;                      // 1-way interleave
      socketVar->remSize = 0;                                 // No more memory
      // Create Interleave List
      for (j = 0; j < MAX_SAD_WAYS; j++) {
        mem->SADintList[SADIndex][j] = host->nvram.mem.socket[socket].haList[ha].haNodeId;
      }

      // Point to next SAD entry. Issue warning if use all SAD rules before memory
      // completely allocated.
      SADIndex++;

      if (SADIndex >= maxSADRules) {
        MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "SAD rules exceeds %d\n", maxSADRules));
        OutputWarning(host, WARN_INTERLEAVE_FAILURE, WARN_SAD_RULES_EXCEEDED, socket, 0xFF, 0xFF, 0xFF);
        break;
      }

      socketVar->remSize = socketVar->memSize;       // reset socket remSize
    } // for socket
  } // for iotLoopCounter

  // Fill out the remaining entries
  if (SADIndex > 0) MaxLimit = mem->SAD[SADIndex - 1].Limit;

  for(SADIndex; SADIndex < maxSADRules; SADIndex++) {
     mem->SAD[SADIndex].Limit = MaxLimit;
     mem->SAD[SADIndex].Enable = 0;
     mem->SAD[SADIndex].IotEnabled = 0;
     for (j = 0; j < MAX_SAD_WAYS; j++) {
       mem->SADintList[SADIndex][j] = 0;
     }
  }

  //Enter this loop for saving the base addresses of all IOT SAD rules
  for(SADIndex = 0; SADIndex < maxSADRules; SADIndex++) {
    if (mem->SAD[SADIndex].IotEnabled == 0) continue;
    else {
      if (SADIndex == 0) {
        iotLimit = 0;
      } else {
        iotLimit = (mem->SAD[SADIndex - 1].Limit);  //Limit of previous SAD is the base of the current SAD
      }
	  
      if ( counterAddRange >= SAD_RULES_ADDR_RANGE ){
    	  break; // breaking the loop here,if counterAddRange goes beyond the array size. KWID :7503
      }
	  
      host->var.mem.baseIOTPhysicalAddress[counterAddRange].lo = (iotLimit << 26);   //6:0 Bits of Limit saved in lo
      host->var.mem.baseIOTPhysicalAddress[counterAddRange].hi = (iotLimit >> 6);   //20:7 Bits of Limit saved in hi

      counterAddRange++; // counterAddRange < 4
    }
  }

  return SUCCESS;

} // SADNuma1HA



UINT32 SADSplitBelow4GB2HA (
  PSYSHOST host,
  UINT16 TOLM,
  UINT8* SADIndexArg,
  UINT32 *previousSADLimitArg)

/*++

  Creates SADs for memory below 4GB for multiple HA case.
  It does not program any registers.

  @param host                - Pointer to sysHost
  @param TOLM                - Top of low memory
  @param SADIndexArg         - Index into the current SAD
  @param previousSADLimitArg - SAD limit for the previous SAD entry

  @retval SUCCESS

--*/
{
  UINT8 socket;
  UINT8 i;
  UINT8 SADIndex = *SADIndexArg;
  UINT8 socketCount;
  UINT32 limit;
  UINT8 haNum;
  UINT8 haWays;
  UINT32 haLimitMin;
  UINT32 haLimitMax;
  UINT32 previousSADLimit = *previousSADLimitArg;
  struct memVar *mem;
  struct SADTable *SADEntry;
  UINT8 (*SADList)[MAX_SAD_WAYS];
  //UINT8 maxSADRules = MAX_SAD_RULES;

  mem = &host->var.mem;

  // Count number of sockets populated with memory
  socketCount = 0;
  for (socket = 0; socket < MAX_SOCKET; socket++) {

    if (host->nvram.mem.socket[socket].enabled == 0)
      continue;

    if (host->var.mem.socket[socket].memSize != 0) {
      socketCount++;
    }
  }

  // Evenly distribute low memory across sockets

  limit = TOLM / socketCount;

  // Build SADs for each socket. SADlimit = min (socket memSize, limit) + previousSADLimit

  previousSADLimit = 0;

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    // No SAD needed for sockets with 0 memory
    if (host->nvram.mem.socket[socket].enabled == 0)
      continue;
    if (mem->socket[socket].memSize == 0)
      continue;

    SADEntry = &mem->SAD[SADIndex];

    // SAD entry either interleaved across HAs or for one HA

    // Compute min size
    haLimitMin = (mem->socket[socket].haList[0].memSize <= mem->socket[socket].haList[1].memSize) ? mem->socket[socket].haList[0].memSize :
                                                                                          mem->socket[socket].haList[1].memSize;
    // Compute max size
    haLimitMax = (mem->socket[socket].haList[0].memSize > mem->socket[socket].haList[1].memSize) ? mem->socket[socket].haList[0].memSize :
                                                                                         mem->socket[socket].haList[1].memSize;
    haNum = 0;

    if (haLimitMin * 2 > limit) {
      SADEntry->Limit = previousSADLimit + (limit / 2) * 2;
      haWays = 2;
    } else if (haLimitMin * 2 > haLimitMax) {
      SADEntry->Limit = previousSADLimit + 2*haLimitMin;
      haWays = 2;
    } else {
      SADEntry->Limit = previousSADLimit + haLimitMax;
      haWays = 1;
      if (mem->socket[socket].haList[0].memSize >= mem->socket[socket].haList[1].memSize) {
        haNum = 0;
      } else {
        haNum = 1;
      }
    }

    SADEntry->Enable = 1;
    SADEntry->ways = haWays;
    SADEntry->Mode = mem->InterleaveMode;

    SADList = &mem->SADintList[SADIndex];

    mem->socket[socket].remSize = (UINT16)(mem->socket[socket].remSize - (SADEntry->Limit - previousSADLimit));

    if (haWays == 1) {
      // Decrement HA socket remSize by amount of memory allocated
      mem->socket[socket].haList[haNum].remSize = (UINT16)(mem->socket[socket].haList[haNum].remSize - (SADEntry->Limit - previousSADLimit));
      // Fill in SAD interleave list for 1-way interleave
      for (i = 0; i < MAX_SAD_WAYS; i++) {
        (*SADList)[i] = host->nvram.mem.socket[socket].haList[haNum].haNodeId;
      }
    } else {
      // Decrement 2 HA socket remSize by amount of memory allocated
      mem->socket[socket].haList[0].remSize = (UINT16)(mem->socket[socket].haList[haNum].remSize - (SADEntry->Limit - previousSADLimit) / 2);
      mem->socket[socket].haList[1].remSize = (UINT16)(mem->socket[socket].haList[haNum].remSize - (SADEntry->Limit - previousSADLimit) / 2);
      // Fill in SAD interleave list for 2-way interleave
      for (i = 0; i < MAX_SAD_WAYS; i=i+2) {
        (*SADList)[i] = host->nvram.mem.socket[socket].haList[0].haNodeId;
        (*SADList)[i+1] = host->nvram.mem.socket[socket].haList[1].haNodeId;
      }
    }

    // Adjust SADLimit to top of hole, if needed
    if ((SADEntry->Limit == TOLM) && (host->var.mem.memSize > TOLM)) {
      SADEntry->Limit = host->var.mem.highMemBase;
    }

    previousSADLimit = SADEntry->Limit;

    SADIndex++;

    // Issue warning if all SAD rules used before all sockets allocated
    if (SADIndex >= SAD_RULES) {
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "SAD rules exceeds %d\n", SAD_RULES));
      OutputWarning(host, WARN_INTERLEAVE_FAILURE, WARN_SAD_RULES_EXCEEDED, socket, 0xFF, 0xFF, 0xFF);
     break;
    }
  } // for socket

  *previousSADLimitArg = previousSADLimit;
  *SADIndexArg = SADIndex;

  return SUCCESS;

} // SADSplitBelow4GB2HA

UINT32
SADNuma2HA (
  PSYSHOST  host,
  UINT16    TOLM,
  UINT8     Optimize
  )
/*++

  Interleaves memory on the HA and channel level but not across sockets.
  This routine figures out the mapping and fills in the structures for the SAD and TAD
  tables.  It does not program any registers.

  @param host                - Pointer to sysHost
  @param TOLM                - Top of low memory
  @param SADIndexArg         - Index into the current SAD
  @param previousSADLimitArg - SAD limit for the previous SAD entry

  @retval SUCCESS

--*/
{
  UINT8               socket;
  UINT8               haIndex;
  UINT8               j;
  UINT8               SADIndex;
  UINT8               haNum;
  UINT8               maxSADRules = MAX_SAD_RULES;
  UINT16              interSize;
  UINT32              MaxLimit;
  UINT32              previousSADLimit = 0;
  UINT32              HighAddressStart;
  UINT32              haLimit;
  UINT32              iotLimit;
  UINT8               counterAddRange = 0;
  UINT8               iotLoop;
  UINT8               iotLoopCounter;
  struct socketNvram  *socketNv;
  struct Socket       *socketVar;
  struct memVar       *mem;
  struct homeAgent    *haMem;
#ifdef NVMEM_FEATURE_EN
  int                 passnum;
#endif // NVMEM_FEATURE_EN

  // Initialize SAD rule number
  SADIndex = 0;
  mem = &host->var.mem;
  mem->socketInter = 1;

#if (MAX_SOCKET > 1 && MAX_SOCKET <= 4)
  if (((host->var.mem.memSize) > TOLM) &&
      (host->setup.mem.options & SPLIT_BELOW_4GB_EN)) { // More than 4GB split?

    SADSplitBelow4GB2HA (host, TOLM, &SADIndex, &previousSADLimit);

  } // if SPLIT 4GB
#endif //MAX_SOCKET > 1

  //Disables IOT for socket with IOT Mem > memSize
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    socketNv = &host->nvram.mem.socket[socket];
    socketVar = &mem->socket[socket];

    if ((socketNv->enabled == 0) || (socketVar->memSize == 0)) continue;  //Reqd. as IOT will always be > memSize when a socket is not enabled
    host->var.mem.iotDisabled[socket] = 0;
    if (host->setup.mem.iotMemBufferRsvtn > host->var.mem.socket[socket].memSize) {
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "Warning: IOT disabled! Size > available memory S%d\n", socket));
      host->var.mem.iotDisabled[socket] = 1;
    }
  }

  //Enabling/Disabling IOT
  if (host->setup.mem.iotMemBufferRsvtn != 0) { //Enabled, loop 2 times around socket loop
    iotLoop = 2;
  } else {
    iotLoop = 1; //Disabled, loop 1 time around socket loop
  }

//If IOT is enabled, allocate it memory only on the 2nd iteration of iotLoopCounter
  for (iotLoopCounter = 0; iotLoopCounter < iotLoop; iotLoopCounter ++) {
    // Create an entry for remaining memory on each socket
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      socketNv = &host->nvram.mem.socket[socket];
      socketVar = &mem->socket[socket];

      //If IOT > memSize of a socket, then skip IOT for that socket only during the 2nd iteration
      if ((host->var.mem.iotDisabled[socket] == 1) && (iotLoopCounter == 1)) continue;

      //
      // Skip if socket not enabled or no memory for socket
      //
      if (socketNv->enabled == 0) continue;
      if (socketVar->memSize == 0) continue;

      if ((iotLoop == 2) && (iotLoopCounter == 0) && (host->var.mem.iotDisabled[socket] != 1)) {      //IOT enabled, on first loop reserve memory on each socket
        socketVar->remSize = socketVar->remSize - host->setup.mem.iotMemBufferRsvtn; // remHAsize - IOT Value
        socketVar->haList[0].remSize = socketVar->haList[0].remSize - host->setup.mem.iotMemBufferRsvtn;// on first loop reserve memory on one HA
      }

      //
      //After socket loop has run once, allocate the remaining Socket memory to the IOT buffer
      //Now, create a new SAD rule for the IOT buffer per socket as we enter the socket loop for the second time
      //

      if (iotLoopCounter == 0) {                              // All non-IOT SAD rules, created in first loop iteration
        mem->SAD[SADIndex].IotEnabled = 0;
      } else {                                                // Second time when we enter the socket loop
        mem->SAD[SADIndex].IotEnabled = 1;
        socketVar->remSize = host->setup.mem.iotMemBufferRsvtn;    //  Add previously reserved IOT size to the remSize for the socket
        socketVar->haList[0].remSize = host->setup.mem.iotMemBufferRsvtn;   //Now, ha[0], will have only IOT size of memory
      }
#ifdef NVMEM_FEATURE_EN
      socketVar->remSize += socketVar->NVmemSize;

      for (passnum = 0; passnum < 2; passnum++) {
        if (passnum == 1) {
          socketVar->haList[0].remSize = socketVar->haList[0].NVmemSize;
          socketVar->haList[1].remSize = socketVar->haList[1].NVmemSize;
        }
#endif // NVMEM_FEATURE_EN

#ifdef NVMEM_FEATURE_EN
      if (host->var.mem.doHAInterleave && !((host->setup.mem.interNVDIMMS == 0) && passnum == 1)) {
#else
      if (host->var.mem.doHAInterleave) {
#endif
        //
        // Interleave across HA's
        //

        // Allocate remaining socket memory with next SAD
#ifdef NVMEM_FEATURE_EN
        if ((socketVar->haList[0].remSize == 0) && (socketVar->haList[1].remSize == 0)) continue;
        MaxLimit = previousSADLimit + (socketVar->haList[0].remSize + socketVar->haList[1].remSize);
#else
        MaxLimit = previousSADLimit + socketVar->remSize;
#endif

        // Allocate all remaining memory in socket across HAs
        // Requires two SADs if memory unequally distributed across HAs

        // Allocate first SAD, 2-way interleave
        if (iotLoopCounter == 0) {
          if (socketVar->haList[0].remSize == socketVar->haList[1].remSize) {
            haNum = 0;
            haLimit = MaxLimit;
            interSize = socketVar->haList[0].remSize * 2;
          } else {
            if (socketVar->haList[0].remSize > socketVar->haList[1].remSize) {
              haNum = 0;
              interSize = (2 * socketVar->haList[1].remSize);
            } else {
              haNum = 1;
              interSize = (2 * socketVar->haList[0].remSize);
            }//if HA remSize
            if( interSize != 0 ){
              haLimit = previousSADLimit + interSize;
            } else {
              haLimit = MaxLimit;
            }
          }//if HA1 == HA2

          // Adjust SAD Limit for memory hole, if necessary
          if ((previousSADLimit <= TOLM) && (haLimit > TOLM) && (host->var.mem.memSize > TOLM)) {
            HighAddressStart = host->var.mem.highMemBase;

            haLimit  += HighAddressStart - TOLM;            // Add Gap to SAD Limit
            MaxLimit += HighAddressStart - TOLM;            // Add Gap to SAD Limit
          }

          // Enforce system address remap limit - this also limits the TAD Limit

          if (haLimit > host->var.common.tohmLimit) {
            if( interSize != 0 ){
              interSize = interSize - (UINT16) (haLimit - host->var.common.tohmLimit);  // Adjust interleave size accordingly
            }
            haLimit = host->var.common.tohmLimit;
            // Indicate memory size limited
            MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "Warning: TOHM exceeded\n"));
            LogWarning(host, WARN_MEM_LIMIT, 0, (UINT32)-1);
          }// if TOHM

          // Allocate first SAD for interleave across 2 HAs
          mem->SAD[SADIndex].Limit = haLimit;
          mem->SAD[SADIndex].Enable = 1;                    // Enable this entry
          mem->SAD[SADIndex].Mode = mem->InterleaveMode;
          if( interSize != 0 ){
            mem->SAD[SADIndex].ways = 2;                      // 2-way interleave
            // Create Interleave List for 2 HAs
            for (j = 0; j < MAX_SAD_WAYS; j = j + 2) {
              mem->SADintList[SADIndex][j] = host->nvram.mem.socket[socket].haList[0].haNodeId;
              mem->SADintList[SADIndex][j+1] = host->nvram.mem.socket[socket].haList[1].haNodeId;
            }

            // Update remSize for socket and each HA socket
            socketVar->remSize = socketVar->remSize - interSize;
            socketVar->haList[0].remSize = socketVar->haList[0].remSize - (interSize /2);
            socketVar->haList[1].remSize = socketVar->haList[1].remSize - (interSize /2);
          } else {
            mem->SAD[SADIndex].ways = 1;                      // 1-way interleave

            // Create Interleave List
            for (j = 0; j < MAX_SAD_WAYS; j++) {
              if(haNum == 0){
                mem->SADintList[SADIndex][j] = host->nvram.mem.socket[socket].haList[haNum].haNodeId;
                socketVar->haList[0].remSize = 0;
              } else {
                mem->SADintList[SADIndex][j] = host->nvram.mem.socket[socket].haList[haNum].haNodeId;
                socketVar->haList[1].remSize = 0;
              }//if haNum
            }// for j
          }//if interleave size

          // Update remSize for socket and each HA socket
          //imc->remSize = 0;
          //imc->haList[haNum].remSize = 0;
          //previousSADLimit = mem->SAD[SADIndex].Limit;

          // Point to next SAD entry. Issue warning if use all SAD rules before memory
          // completely allocated.
          SADIndex++;
        } //if iotLoopCounter == 0

        // unmapped memory after interleaving
        if((socketVar->haList[0].remSize != 0) || (socketVar->haList[1].remSize != 0)){

          // Allocate next SAD entry for 1 HAs
          mem->SAD[SADIndex].Limit = MaxLimit;
          mem->SAD[SADIndex].Enable = 1;                    // Enable this entry
          mem->SAD[SADIndex].Mode = mem->InterleaveMode;

          mem->SAD[SADIndex].ways = 1;                      // 1-way interleave
          // Create Interleave List
          for (j = 0; j < MAX_SAD_WAYS; j++) {
            if(socketVar->haList[0].remSize != 0){
              mem->SADintList[SADIndex][j] = host->nvram.mem.socket[socket].haList[0].haNodeId;
            } else {
              mem->SADintList[SADIndex][j] = host->nvram.mem.socket[socket].haList[1].haNodeId;
            }//if HA remSize
          }//for Sad ways
          socketVar->haList[0].remSize = 0;
          socketVar->haList[1].remSize = 0;
          SADIndex++;
        }//if memory leftover

        if (SADIndex > 0) previousSADLimit = mem->SAD[SADIndex-1].Limit;
        if (SADIndex >= maxSADRules) {
          MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "Warning: SAD rules exceeds %d\n", maxSADRules));
          LogWarning(host, WARN_INTERLEAVE_FAILURE, WARN_SAD_RULES_EXCEEDED, socket);
          break;
        }
      } else {
        //
        // Do not interleave across HA's
        //

        for (haIndex = 0; haIndex < MAX_HA; haIndex++) {
          haMem = &mem->socket[socket].haList[haIndex];
          if ((haMem->memSize == 0) && (host->var.qpi.OutClusterOnDieEn == 1)) {
            MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "Warning: COD Enabled but memory not behind each HA \n"));
            OutputWarning(host, WARN_COD_HA_NOT_ACTIVE, 0, socket, 0xFF, 0xFF, 0xFF);
          }
#ifdef NVMEM_FEATURE_EN
          if (haMem->remSize == 0) continue;
#else
          if (haMem->memSize == 0) continue;
#endif

          //
          // Determine the maximum memory limit accounting for the current HA
          //
          MaxLimit = previousSADLimit + socketVar->haList[haIndex].remSize;
          haLimit = MaxLimit;
          interSize = (UINT16)MaxLimit;

          //
          // Adjust SAD Limit for memory hole, if necessary
          //
          if ((previousSADLimit <= TOLM) && (haLimit > TOLM) && (host->var.mem.memSize > TOLM)) {
            HighAddressStart = host->var.mem.highMemBase;

            haLimit  += HighAddressStart - TOLM;            // Add Gap to SAD Limit
            MaxLimit += HighAddressStart - TOLM;            // Add Gap to SAD Limit
          }

          //
          // Enforce system address remap limit - this also limits the TAD Limit
          //
          if (haLimit > host->var.common.tohmLimit) {
            if (interSize != 0) {
              interSize = interSize - (UINT16) (haLimit - host->var.common.tohmLimit);  // Adjust interleave size accordingly
            }
            haLimit = host->var.common.tohmLimit;

            //
            // Indicate memory size limited
            //
            MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "Warning: TOHM exceeded\n"));
            LogWarning(host, WARN_MEM_LIMIT, 0, (UINT32)-1);
          }// if TOHM

          //
          // Allocate SAD
          //
          mem->SAD[SADIndex].Limit = haLimit;
          mem->SAD[SADIndex].Enable = 1;
          mem->SAD[SADIndex].Mode = mem->InterleaveMode;

          mem->SAD[SADIndex].ways = 1;
          if (interSize != 0) {
            //
            // Create Interleave List
            //
            for (j = 0; j < MAX_SAD_WAYS; j++) {
              mem->SADintList[SADIndex][j] = host->nvram.mem.socket[socket].haList[haIndex].haNodeId;
            } // j loop

            //
            // Update remSize for socket and HA
            socketVar->remSize = socketVar->remSize - interSize;
            socketVar->haList[haIndex].remSize = socketVar->haList[haIndex].remSize - interSize;
          } else {

            // Create Interleave List
            for (j = 0; j < MAX_SAD_WAYS; j++) {
              mem->SADintList[SADIndex][j] = host->nvram.mem.socket[socket].haList[haIndex].haNodeId;
              socketVar->haList[haIndex].remSize = 0;
            }// for j
          }//if interleave size

          //
          // For 2HA, IOT is allocated only on HA[0], not for HA[1]; to prevent a SAD entry being created for haIndex = 1
          // We exit the socket loop on the 2nd iteration of iotLoopCounter
          //
          if ((iotLoopCounter == 1) && (haIndex == 1)) {
            continue;
          } else if (iotLoopCounter == 1) { //IOT has been allocated to HA[0], point to next SAD entry.and exit loop
            previousSADLimit = mem->SAD[SADIndex].Limit;
            SADIndex++;
            continue;
          }

          previousSADLimit = mem->SAD[SADIndex].Limit;
          // Point to next SAD entry. Issue warning if use all SAD rules before memory
          // completely allocated.
          SADIndex++;
        } // haIndex

        if (SADIndex >= maxSADRules) {
          MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "Warning: SAD rules exceeds %d\n", maxSADRules));
          LogWarning(host, WARN_INTERLEAVE_FAILURE, WARN_SAD_RULES_EXCEEDED, socket);
          break;
        }

      } // if HA interleave

#ifdef NVMEM_FEATURE_EN
      }
      // reset remSize
      socketVar->remSize = socketVar->memSize;
      socketVar->haList[0].remSize = (socketVar->haList[0].memSize - socketVar->haList[0].NVmemSize);
      socketVar->haList[1].remSize = (socketVar->haList[1].memSize - socketVar->haList[1].NVmemSize);
#else
      socketVar->remSize = socketVar->memSize;
      socketVar->haList[0].remSize = socketVar->haList[0].memSize;
      socketVar->haList[1].remSize = socketVar->haList[1].memSize;
#endif

    } // for socket
  } // iot_lopp

  // Fill out the remaining entries
  if (SADIndex == 0) {
    MaxLimit = 0;
  } else {
    MaxLimit = mem->SAD[SADIndex - 1].Limit;
  }

  while(SADIndex < maxSADRules) {
     mem->SAD[SADIndex].Limit = MaxLimit;
     mem->SAD[SADIndex].Enable = 0;
     mem->SAD[SADIndex].IotEnabled = 0;
     for (j = 0; j < MAX_SAD_WAYS; j++) {
       mem->SADintList[SADIndex][j] = 0;
     }
     SADIndex ++;
  }

  //Enter this loop for saving the base addresses of all IOT rules
  for(SADIndex = 0; SADIndex < maxSADRules; SADIndex++) {
    if (mem->SAD[SADIndex].IotEnabled == 0) continue;
    else {           // Saving the address ranges of all IOT SAD rules
      if (SADIndex == 0) {
        iotLimit = 0;
      } else {
        iotLimit = (mem->SAD[SADIndex - 1].Limit);
      }

      host->var.mem.baseIOTPhysicalAddress[counterAddRange].lo = (iotLimit << 26);   //6:0 Bits of Limit saved in lo
      host->var.mem.baseIOTPhysicalAddress[counterAddRange].hi = (iotLimit >> 6);   //20:7 Bits of Limit saved in hi

      counterAddRange++; // counterAddRange < 4
    }
  }

  return SUCCESS;

} // SADNuma2HA

UINT32 SADNuma (
  PSYSHOST  host,
  UINT16    TOLM,
  UINT8     Optimize
  )
/*++

  Interleaves memory on the HA and channel level but not across sockets for NUMA configuration.
  This routine figures out the mapping and fills in the structures for the SAD and TAD
  tables.  It does not program any registers.

  @param host                - Pointer to sysHost
  @param TOLM                - Top of low memory
  @param Optimize            - Optimization level

  @retval SUCCESS

--*/
{

  UINT8   Socket = 0;
  UINT8   maxSADRules = MAX_SAD_RULES;
  UINT8   SADIndex;
  UINT8   nodeID;
  struct  memVar *mem;
  mem = &host->var.mem;

  if (host->var.common.numActiveHAperSocket == 2) {
    SADNuma2HA (host, TOLM, Optimize);
  } else {
        if (host->var.qpi.OutClusterOnDieEn == 1) {
          MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "Warning: COD Enabled but memory not behind each HA \n"));
          OutputWarning(host, WARN_COD_HA_NOT_ACTIVE, 0, Socket, 0xFF, 0xFF, 0xFF);
        }
    SADNuma1HA (host, TOLM, Optimize);
  }
  //
  // Call BIOS/VCU Mailbox API; sending to SetIotMemoryBuffer() the Socket ID on which each IOT rule exists
  // The addresses of the IOT rules need to be reported to a VCU via a newly defined BIOS/VCU mailbox interface
  //
  for(SADIndex = 0; SADIndex < maxSADRules; SADIndex++) {
    if (mem->SAD[SADIndex].IotEnabled == 0) continue;

    nodeID = mem->SADintList[SADIndex][0];
    Socket = (nodeID & 3);
  }

  return SUCCESS;

} // SAD NonInterleave


UINT32
SocketNonInterleave (
  PSYSHOST  host,
  UINT16    TOLM
  )
/*++

  Description: Interleaves memory on the channel level but not across sockets.
  This routine figures out the mapping and fills in the structures for the SAD, TAD,
  tables.  It does not program any registers.

  @param host      - Pointer to sysHost
  @param TOLM      - Top of low memory
  @param Optimize  - Optimization level

  @retval status

--*/
{
  UINT32  status;
  UINT8   Optimize;

  Optimize = 2;
  //
  // Default to mode 0 for NUMA.  Index into the interleave list for a given SAD or TAD rule is determined by
  // XORing system address bits [18,17,16] with bits [8,7,6]
  //
  host->var.mem.InterleaveMode  = 0;


  // Create SADs for NUMA mode
  OutputExtendedCheckpoint((host, STS_DDR_MEMMAP, SUB_SAD_NONINTER, 0));
  SADNuma (host, TOLM, Optimize);

  // Program TAD registers for each socket
  OutputExtendedCheckpoint((host, STS_DDR_MEMMAP, SUB_TAD_NONINTER, 0));
  status = TADInterleave (host, TOLM, Optimize);

  return status;
}

UINT8
RankSort(PSYSHOST host,
         struct rankSort rkSortList[],
         UINT8 socket,
         UINT8 ch)
/*++

  Computes a list of ranks sorted by size, largest to smallest.

  @param host        - Pointer to sysHost
  @param rkSortList  - Rank sort list
  @param socket      - Socket Id
  @param ch          - Channel number (0-based)

  @retval sortNum - Number of ranks with memory remaining to be allocated
            @retval (sortNum may be 0 indicating all memory allocated)

--*/
{
  UINT8 dimm;
  UINT8 rank;
  UINT8 si;
  UINT8 i;
  UINT8 sortNum;
  struct socketNvram *socketNvram;
  struct ddrRank (*rankList)[MAX_RANK_DIMM];
  struct dimmNvram (*dimmNvList)[MAX_DIMM];

  socketNvram = &host->nvram.mem.socket[socket];
  dimmNvList = &socketNvram->channelList[ch].dimmList;

  // Sort ranks by size, largest first
  sortNum = 0;

  for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      // Skip if this DIMM is not present
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      rankList = &socketNvram->channelList[ch].dimmList[dimm].rankList;

      if (CheckRank(host, socket, ch, dimm, rank, CHECK_MAPOUT)) continue;

      // Search for ranks with remaining size
      if ((*rankList)[rank].remSize == 0) continue;

      // Determine sort list position
      for (si = 0; si < sortNum; si++) {
         if (rkSortList[si].rankSize < (*rankList)[rank].remSize) break;
      }
      // Shift smaller ranks towards end of list
      if (si < sortNum) {
         for (i = sortNum; i > si; i--) rkSortList[i] = rkSortList[i - 1];
      }
      // Insert current rank
      rkSortList[si].socketNum = socket;
      rkSortList[si].chNum = ch;
      rkSortList[si].dimmNum = dimm;
      rkSortList[si].rankNum = rank;
      rkSortList[si].rankID = (*rankList)[rank].phyRank;
      rkSortList[si].rankSize = (*rankList)[rank].remSize;
      sortNum++;
    } // DIMM loop
  } // Rank loop

  return sortNum;
}

UINT8  
RankSortTSV3DPC (
        PSYSHOST host,
        struct rankSort rkSortList[],
        UINT8 socket,
        UINT8 ch
    )
/*++

Routine Description:

  Computes a list of ranks sorted by size, largest to smallest.

Arguments:

  host        - Pointer to sysHost
  rkSortList  - Rank sort list
  socket      - Socket Id
  ch          - Channel number (0-based)

Returns:

  sortNum - Number of ranks with memory remaining to be allocated
            (sortNum may be 0 indicating all memory allocated)

--*/
{
  UINT8 dimm;
  UINT8 rank;
  UINT8 si;
  UINT8 i;
  UINT8 sortNum = 0;  // Sort ranks by size, largest first
  struct socketNvram *socketNvram;
  struct ddrRank (*rankList)[MAX_RANK_DIMM];
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  socketNvram = &host->nvram.mem.socket[socket];
  dimmNvList = &socketNvram->channelList[ch].dimmList;

  for (dimm = 0; dimm < MAX_DIMM; dimm++) { //Change Sort priority for Rank first
    if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
    rankList = &socketNvram->channelList[ch].dimmList[dimm].rankList;
    for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
      if (CheckRank(host, socket, ch, dimm, rank, CHECK_MAPOUT)) continue;
      // Search for ranks with remaining size
      if ((*rankList)[rank].remSize == 0) continue;
      // Determine sort list position
      for (si = 0; si < sortNum; si++) {
        if (rkSortList[si].rankSize < (*rankList)[rank].remSize) break;
      }
      // Shift smaller ranks towards end of list
      if (si < sortNum) {
        for (i = sortNum; i > si; i--) rkSortList[i] = rkSortList[i - 1];
      }
      // Insert current rank
      rkSortList[si].socketNum = socket;
      rkSortList[si].chNum = ch;
      rkSortList[si].dimmNum = dimm;
      rkSortList[si].rankNum = rank;
      rkSortList[si].rankID = (*rankList)[rank].phyRank;
      rkSortList[si].rankSize = (*rankList)[rank].rankSize /2;  //Set SortList rank size for halt of actual rank size
      sortNum++;
    } // DIMM loop
  } // Rank loop

  return sortNum;
}

UINT32 RankInterleave(PSYSHOST host)
/*++

  Interleaves memory on the rank level. This routine figures out
  the mapping and fills in the structures for the RIT_Way and RIR_Limit tables.
  It does not program any registers.

  @param host  - Pointer to sysHost

  @retval SUCCESS

--*/
{
  UINT8 socket;
  UINT8 ch;
  UINT8 rank;
  UINT8 si;
  UINT8 i;
  UINT8 j;
  UINT8 interNum;
  UINT8 sortNum;
#ifdef NVMEM_FEATURE_EN
  int NVPass;
  struct ddrRank (*rankListTemp)[MAX_RANK_DIMM];
  UINT8 rankTemp;
  UINT8 dimmTemp;
#endif  //NVMEM_FEATURE_EN
  UINT8 dimmNum;
  UINT16 interSize;
  UINT16 rirLimit;
  UINT8 rirIndex;
  UINT16 rankMemUsed;
  UINT16 previousRIRLimit;
  struct socketNvram (*socketNvram)[MAX_SOCKET];
  struct ddrRank (*rankList)[MAX_RANK_DIMM];
  struct rankSort rkSortList[MAX_RANK_CH];
  struct rankSort rkSortItem;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  struct ddrChannel *channelList;
  struct ddrChannel *channelList1;
  struct channelNvram (*channelNvList)[MAX_CH];

  socketNvram = &host->nvram.mem.socket;
  // Loop for each socket
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if ((*socketNvram)[socket].enabled == 0) continue;
	channelNvList = &host->nvram.mem.socket[socket].channelList;

    // Loop for each channel
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*socketNvram)[socket].channelList[ch].enabled == 0) continue;

      channelList = &host->var.mem.socket[socket].channelList[ch];

      if (channelList->memSize == 0) continue;

      dimmNvList = &(*socketNvram)[socket].channelList[ch].dimmList;

      // Get desired interleave width
      interNum = host->var.mem.rankInter;

      // Start the rirLimit off at 0 for each channel
      rirLimit = 0;
      previousRIRLimit = 0;
#ifdef NVMEM_FEATURE_EN
      NVPass = 0;
#endif  //NVMEM_FEATURE_EN

      // 5 Rank Level Interleaves can be defined
      for (rirIndex = 0; rirIndex < MAX_RIR; rirIndex++) {

        // Check for exit condition: all channel memory allocated in RIR
        if (rirLimit == channelList->memSize) {
            break;
        }

        // Sort ranks by size, largest first
        if((host->nvram.mem.dramType == SPD_TYPE_DDR4) && ((*channelNvList)[ch].encodedCSMode == 2) && ((*channelNvList)[ch].maxDimm == 3) && (!(*channelNvList)[ch].lrDimmPresent)) { 
          sortNum = RankSortTSV3DPC (host, rkSortList, socket, ch);  //Set rank sort first for TSV 3DPC case
        } else sortNum = RankSort (host, rkSortList, socket, ch);
        // Determine largest N ranks, where N:1 is the rank interleave
        if (sortNum) {

          // Compare N to remaining ranks
          while (interNum > sortNum) interNum >>= 1;

          // Get size common to N ranks
          interSize = rkSortList[interNum - 1].rankSize;

          // Update the size common to N ranks
          if (interSize > rkSortList[interNum - 1].rankSize)
             interSize = rkSortList[interNum - 1].rankSize;
             //Skip eliminate adjacent for TSV 3DPC case
             if (!((host->nvram.mem.dramType == SPD_TYPE_DDR4) && ((*channelNvList)[ch].encodedCSMode == 2) && ((*channelNvList)[ch].maxDimm == 3) && (!(*channelNvList)[ch].lrDimmPresent))) {
               // Optimize sortList to eliminate adjacent
               if (interNum > 2) {
	           for (i = 0; i < interNum - 1; i++) {
	             // Check if these ranks are adjacent
	             if (rkSortList[i].dimmNum == rkSortList[i+1].dimmNum) {
                   // Point to a entry to trade with
	               j = i + 2;

	               // Break if we can't find an entry to trade with
	               if (j >= interNum) break;
	              // Find an entry that is different
                  while (rkSortList[i].dimmNum == rkSortList[j].dimmNum) {
	                j++;
	                // Break if we can't find an entry to trade with
	                if (j >= interNum) break;
                  }

	              // Swap entries if this entry is valid
	              if (j >= interNum) break;

	              // Exchange sortList entries 1 and 2
	              rkSortItem = rkSortList[i];
	              rkSortList[i] = rkSortList[j];
	              rkSortList[j] = rkSortItem;
	            }
	          }
	        }
          }
          // Increment RIR.Limit by interNum * interSize
          rirLimit += (interNum * interSize);

          // Save RIR Limit / ways
          channelList->rirLimit[rirIndex] = rirLimit;
          channelList->rirWay[rirIndex] = interNum;
          channelList->rirValid[rirIndex] = 1;

          // Initialize RIR Interleave
          for (i = 0; i < interNum; i++) {
            rank = rkSortList[i].rankNum;

            // Fill in rank ID
            channelList->rirIntList[rirIndex][i] = rkSortList[i].rankID;

            // Compute RIR offset = (channel_address_base / RIRways) - Rank_Address_base
            dimmNum = rkSortList[i].dimmNum;

            rankList = &(*socketNvram)[socket].channelList[ch].dimmList[dimmNum].rankList;
#ifdef NVMEM_FEATURE_EN
            if (NVPass == 0) {
              rankMemUsed = (*rankList)[rank].rankSize - ((*rankList)[rank].remSize + (*rankList)[rank].NVrankSize);
            } else
#endif // NVMEM_FEATURE_EN
            rankMemUsed = (*rankList)[rank].rankSize - (*rankList)[rank].remSize;
            channelList->rirOffset[rirIndex][i] = (previousRIRLimit / interNum) - rankMemUsed;
          }

          // Fill in the rest of the rir registers for this interleave
          for(i; i < MAX_RIR_WAYS; i++) {
            channelList->rirIntList[rirIndex][i] = 0;
            channelList->rirOffset[rirIndex][i] = 0;
          }

          // Subtract common size from N ranks
          for (si = 0; si < interNum; si++) {
            (*socketNvram)[socket].channelList[ch].dimmList[rkSortList[si].dimmNum].rankList[rkSortList[si].rankNum].remSize = (*socketNvram)[socket].channelList[ch].dimmList[rkSortList[si].dimmNum].rankList[rkSortList[si].rankNum].remSize - interSize;
          }

          // Save current rirLimit to use while creating the next entry
          previousRIRLimit = rirLimit;

#ifdef NVMEM_FEATURE_EN
        } else {
          if(NVPass != 0) break;
          if(!channelList->NVmemSize) break;
          NVPass = 1;
          rirIndex--;
          channelList->remSize = channelList->NVmemSize;
#ifdef NVMEM_FEATURE_EN
          if (host->setup.mem.interNVDIMMS == 0) interNum = 1;
#endif

          for (dimmTemp = 0; dimmTemp < MAX_DIMM; dimmTemp++) {
            if ((*dimmNvList)[dimmTemp].dimmPresent == 0) continue;

            rankListTemp = &host->nvram.mem.socket[socket].channelList[ch].dimmList[dimmTemp].rankList;

            for (rankTemp = 0; rankTemp < MAX_RANK_DIMM; rankTemp++) {
              if ((*rankListTemp)[rankTemp].enabled == 0) continue;
              (*rankListTemp)[rankTemp].remSize = (*rankListTemp)[rankTemp].NVrankSize;
            } // rankTemp loop
          } // dimmTemp loop
        }
#else
        } else break;// if sortNum
#endif // NVMEM_FEATURE_EN

      } // rirIndex loop

      // Issue warning if all RIR rules used before all memory allocated
      if (rirIndex > MAX_RIR) {
        MemDebugPrint((host, SDBG_MINMAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "RIR rules exceeds %d\n", MAX_RIR));
        OutputWarning(host, WARN_INTERLEAVE_FAILURE, WARN_RIR_RULES_EXCEEDED, socket, ch, 0xFF, 0xFF);
      }

      // Issue warning if all all the memory did not get mapped
      if (rirLimit < channelList->memSize) {
        MemDebugPrint((host, SDBG_MINMAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "RIR rules exceeds %d\n", MAX_RIR));

        channelList->memSize = rirLimit;

        OutputWarning(host, WARN_INTERLEAVE_FAILURE, WARN_RIR_RULES_EXCEEDED, socket, ch, 0xFF, 0xFF);
      }

      // Initialize unused RIR rules for the current channel

      // rirIndex set by the for loop above
      for (rirIndex; rirIndex < MAX_RIR; rirIndex++) {
        channelList->rirLimit[rirIndex] = rirLimit;
        channelList->rirValid[rirIndex] = 0;
        channelList->rirWay[rirIndex] = 0;

        for(i = 0; i < MAX_RIR_WAYS; i++) {
          channelList->rirIntList[rirIndex][i] = 0;
          channelList->rirOffset[rirIndex][i] = 0;
        }
      }

      // Fill in the the RIR for unspecified channels

      // If CH_LOCKSTEP or CH_MIRROR, copy rules from ch0 -> ch1 and ch2 -> ch3
      if (((host->nvram.mem.RASmode & CH_LOCKSTEP) || (host->nvram.mem.RASmode & CH_MIRROR)) &&
          ((ch == 0) || (ch == 2))) {
        channelList1 = &host->var.mem.socket[socket].channelList[ch+1];

        for (rirIndex = 0; rirIndex < MAX_RIR; rirIndex++) {
          channelList1->rirLimit[rirIndex] = channelList->rirLimit[rirIndex];
          channelList1->rirValid[rirIndex] = channelList->rirValid[rirIndex];
          channelList1->rirWay[rirIndex] = channelList->rirWay[rirIndex];

          for(i = 0; i < MAX_RIR_WAYS; i++) {
            channelList1->rirIntList[rirIndex][i] = channelList->rirIntList[rirIndex][i];
            channelList1->rirOffset[rirIndex][i] = channelList->rirOffset[rirIndex][i];
          }
        }
      } // if CH_LOCKSTEP or CH_MIRROR

    } // Ch loop
  } // socket loop

  return SUCCESS;
}


void PopulateMemorySizeFields(PSYSHOST host)
/*++

  This routine initializes the memory size fields in the structures
  for DIMMs, Channels, and Nodes.  The sizes are calculated from the sizes in
  the rank structures.

  @param host  - Pointer to sysHost

  @retval SUCCESS

--*/
{
  UINT8 socket;
  UINT8 ch;
  UINT8 i;
  UINT8 dimm;
  UINT8 ha;
  UINT8 haCh;
  UINT8 perSocketHa;
  struct memNvram *nvramMem;

  struct memVar *mem;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct ddrChannel (*channelList)[MAX_CH];
  struct ddrRank (*rankList)[MAX_RANK_DIMM];
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  struct dimmDevice (*chdimmList)[MAX_DIMM];
  struct homeAgent *haMem;
  struct haNvram *haNv;

  nvramMem = &host->nvram.mem;

  mem = &host->var.mem;

  mem->memSize = 0;

  mem->chInter = host->setup.mem.chInter;
  mem->rankInter = host->setup.mem.rankInter;
  mem->socketInter = host->setup.mem.socketInter;

  //
  // Only NUMA (1-way interleaving) is supported with 4 sockets populated
  //
  if (host->var.common.numCpus == 4) {
    mem->socketInter = 1;
  }

  host->var.common.numActiveHAperSocket = 0;

  // For each socket
  for (socket = 0; socket < MAX_SOCKET; socket++) { // Loop for each CPU socket
    if (nvramMem->socket[socket].enabled == 0) continue;

    perSocketHa   = 0;
    channelNvList = &nvramMem->socket[socket].channelList;

    mem->socket[socket].memSize = 0;
    channelList = &mem->socket[socket].channelList;

    // For each channel
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      chdimmList = &mem->socket[socket].channelList[ch].dimmList;

      (*channelList)[ch].memSize = 0;

      // For each DIMM
      for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {
        dimmNvList = &nvramMem->socket[socket].channelList[ch].dimmList;
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        rankList = &nvramMem->socket[socket].channelList[ch].dimmList[dimm].rankList;

        // DIMM size equals the sum of rank sizes in the DIMM
        // Note: Ranksize may be zero if it is the spare rank

        (*chdimmList)[dimm].memSize = 0;

        for (i = 0;  i < (*dimmNvList)[dimm].numDramRanks; i++) {
          // Check if this rank has been mapped out
          if ((*dimmNvList)[dimm].mapOut[i]) {
            // Check if a RAS mode is enabled
            if (nvramMem->RASmode & (CH_LOCKSTEP + CH_MIRROR)) {
              if (ch == 0) {
                // Make sure the cooresponding DIMM on channel 1 is mapped out as well
                nvramMem->socket[socket].channelList[1].dimmList[dimm].mapOut[i] = 1;
              } else if (ch == 2) {
                // Make sure the cooresponding DIMM on channel 3 is mapped out as well
                nvramMem->socket[socket].channelList[3].dimmList[dimm].mapOut[i] = 1;
              }
            }
            // Continue to the next rank
            continue;
          }

          if((*rankList)[i].enabled == 0){

              continue; // Continue to the next rank
          }

          (*chdimmList)[dimm].memSize = (*chdimmList)[dimm].memSize + (*rankList)[i].rankSize;
#ifdef NVMEM_FEATURE_EN
          (*chdimmList)[dimm].NVmemSize = (*chdimmList)[dimm].NVmemSize + (*rankList)[i].NVrankSize;
#endif // NVMEM_FEATURE_EN

        }

        // Channel memory sizes equals the sum of the all DIMM sizes on this channel
        (*channelList)[ch].memSize = (*channelList)[ch].memSize + (*chdimmList)[dimm].memSize;
#ifdef NVMEM_FEATURE_EN
        (*channelList)[ch].NVmemSize = (*channelList)[ch].NVmemSize + (*chdimmList)[dimm].NVmemSize;
#endif  //NVMEM_FEATURE_EN
      } // DIMM


      // Remaining size equals memory size
#ifdef NVMEM_FEATURE_EN
      (*channelList)[ch].remSize = ((*channelList)[ch].memSize - (*channelList)[ch].NVmemSize);
#else
      (*channelList)[ch].remSize = (*channelList)[ch].memSize;
#endif // NVMEM_FEATURE_EN

      // The memory size for this socket equals the sum of all channel sizes on this socket
      mem->socket[socket].memSize = mem->socket[socket].memSize + (*channelList)[ch].memSize;
#ifdef NVMEM_FEATURE_EN
      mem->socket[socket].NVmemSize = mem->socket[socket].NVmemSize + (*channelList)[ch].NVmemSize;
#endif // NVMEM_FEATURE_EN
    } // Ch

    // Initialize HA structures
    for (ha = 0; ha < MAX_HA; ha++) {
      haNv = &nvramMem->socket[socket].haList[ha];
      haMem = &mem->socket[socket].haList[ha];

      // Initialize HA nvram structure
      haNv->enabled = 0;

      if ((ha == 0) || (Is2HA(host))){
        haNv->haNodeId = ((socket & 0x4) << 1) + (socket & 0x3) + (4 * ha);
        haNv->haNum = ha;
        haNv->haNvChannelListStartIndex = ha * host->var.mem.numChPerHA;

        // Initialize HA mem structure
        haMem->haNum = ha;
        haMem->memSize = 0;
        haMem->remSize = 0;
        haMem->haChannelListStartIndex = ha * host->var.mem.numChPerHA;

        // Initialize HA memSize / remSize
        for (haCh = 0; haCh < host->var.mem.numChPerHA; haCh++) {
          haMem->memSize = haMem->memSize + host->var.mem.socket[socket].channelList[haCh + haMem->haChannelListStartIndex].memSize;
#ifdef NVMEM_FEATURE_EN
          haMem->NVmemSize = haMem->NVmemSize + host->var.mem.socket[socket].channelList[haCh + haMem->haChannelListStartIndex].NVmemSize;
#endif // NVMEM_FEATURE_EN
        }// for channel per HA

#ifdef NVMEM_FEATURE_EN
        haMem->remSize = (haMem->memSize - haMem->NVmemSize);
#else
        haMem->remSize = haMem->memSize;
#endif //NVMEM_FEATURE_EN

        // Enable HA if memsize not zero
        if (haMem->memSize != 0) {
          haNv->enabled = 1;
          perSocketHa++;
        }// if enable HA
      }//if HSX
    }// for HA


    // Save remaining size to be used during memory interleaving
#ifdef NVMEM_FEATURE_EN
    mem->socket[socket].remSize = (mem->socket[socket].memSize - mem->socket[socket].NVmemSize);
#else
    mem->socket[socket].remSize = mem->socket[socket].memSize;
#endif // NVMEM_FEATURE_EN

    // The total system memory size equals the summ of all socket sizes
    mem->memSize = mem->memSize + mem->socket[socket].memSize;
#ifdef NVMEM_FEATURE_EN
    mem->NVmemSize = mem->NVmemSize + mem->socket[socket].NVmemSize;
#endif // NVMEM_FEATURE_EN

    // update the number of HA per socket on the system
    if ( host->var.common.numActiveHAperSocket < perSocketHa){
      host->var.common.numActiveHAperSocket = perSocketHa;
    }
  } // Node

  if (host->var.common.numActiveHAperSocket > MAX_HA) {
      OutputError (host, ERR_MRC_COMPATIBILITY, 0, 0xFF, 0xFF, 0xFF, 0xFF);
  }

  //
  // Halt if there is no memory
  //
  if (mem->memSize == 0) {
    MemDebugPrint((host, 0xFFFF, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "No Memory to map!\n"));
    OutputError (host, ERR_NO_MEMORY, ERR_NO_MEMORY_MINOR_ALL_CH_DISABLED, 0xFF, 0xFF, 0xFF, 0xFF);
  }

} // PopulateMemorySizeFields

UINT32
SADTADEncodeLimit (
  UINT32 Limit
  )
/*++

  Description: Encode Limit field for all DRAM rules
  Limit is encoded such that valid address range <= Limit

  @param Limit - Limit to encode

  @retval Encoded limit

--*/
{
  return (Limit - 1);
}

void
WriteSADCSRs (
  PSYSHOST      host,
  UINT8         socket
  )
/*++

  Description: Fill in SAD CRSs for memory map from config info in host
  structure.

  @param host  - Pointer to sysHost
  @param socket  - Socket Id

  @retval N/A

--*/
{
  UINT8                                 i;
  struct memVar                         *mem;
  struct SADTable                       *SADEntry;
  DRAM_RULE_0_CBOBC_MEMSAD_STRUCT       cboDramRule;
  INTERLEAVE_LIST_0_CBOBC_MEMSAD_STRUCT cboInterleaveList;
  UINT8 maxSADRules = MAX_SAD_RULES;

  mem = &host->var.mem;
  //
  // Write DRAM rules are consecutive offsets
  //
  for (i = 0; i < maxSADRules; i++) {
    SADEntry = &mem->SAD[i];

    //
    // SAD DRAM rule
    //
    cboDramRule.Data                  = 0;
    cboDramRule.Bits.rule_enable      = SADEntry->Enable;
    cboDramRule.Bits.interleave_mode  = mem->InterleaveMode;
    cboDramRule.Bits.attr             = SAD_ATTR_DRAM;
    cboDramRule.Bits.limit            = SADTADEncodeLimit (SADEntry->Limit);

    //
    // SAD Interleave List
    //
    cboInterleaveList.Data       = 0;
    if(host->nvram.mem.RASmodeEx & A7_MODE_EN){
      cboDramRule.Bits.a7mode         = 1;
      cboInterleaveList.Bits.package0 = mem->SADintList[i][0];
      cboInterleaveList.Bits.package1 = mem->SADintList[i][4];
      cboInterleaveList.Bits.package2 = mem->SADintList[i][1];
      cboInterleaveList.Bits.package3 = mem->SADintList[i][5];
      cboInterleaveList.Bits.package4 = mem->SADintList[i][2];
      cboInterleaveList.Bits.package5 = mem->SADintList[i][6];
      cboInterleaveList.Bits.package6 = mem->SADintList[i][3];
      cboInterleaveList.Bits.package7 = mem->SADintList[i][7];
    } else {
      cboDramRule.Bits.a7mode         = 0;
      cboInterleaveList.Bits.package0 = mem->SADintList[i][0];
      cboInterleaveList.Bits.package1 = mem->SADintList[i][1];
      cboInterleaveList.Bits.package2 = mem->SADintList[i][2];
      cboInterleaveList.Bits.package3 = mem->SADintList[i][3];
      cboInterleaveList.Bits.package4 = mem->SADintList[i][4];
      cboInterleaveList.Bits.package5 = mem->SADintList[i][5];
      cboInterleaveList.Bits.package6 = mem->SADintList[i][6];
      cboInterleaveList.Bits.package7 = mem->SADintList[i][7];
    }

    //
    // Write CSRs
    //
    WriteCpuPciCfgEx (host, socket, 0, DRAM_RULE_0_CBOBC_MEMSAD_REG + (i * 8), cboDramRule.Data);
    WriteCpuPciCfgEx (host, socket, 0, INTERLEAVE_LIST_0_CBOBC_MEMSAD_REG + (i * 8), cboInterleaveList.Data);
  } // for

  return ;
}

UINT8
haTADEncodeChWays (
  PSYSHOST host,
  UINT8 Ways
  )
/*++

  Encode TAD channel "ways" for TAD rule CSR in ha (home agent)

  @param Ways  - Ways to encode

  @retval Encoded ways

--*/
{

  // For ha the ch ways is 2x if mirror mode
  if (host->nvram.mem.RASmode & CH_MIRROR) {
    Ways = 2 * Ways;
  }

  return (Ways - 1);
}

UINT8
imcTADEncodeChWays (
  UINT8 Ways
  )
/*++

  Encode TAD channel "ways" for TAD rule CSR in imc (memory controller)

  @param Ways  - Ways to encode

  @retval Encoded ways

--*/
{
  return (Ways - 1);
}

UINT8
TADEncodeSkWays(
  UINT8 Ways
  )
/*++

  Encode TAD socket "ways" for TAD rule CSR.

  @param Ways  - Ways to encode

  @retval Encoded ways

--*/
{
  switch (Ways) {
    case 1:
      Ways = 0;
      break;
    case 2:
      Ways = 1;
      break;
    case 4:
      Ways = 2;
      break;
    case 8:
      Ways = 3;
      break;
    default:
      Ways = 0;
  }
  return Ways;
}

void
WriteTADOffsetForChannel(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ha,
  UINT8 ch,
  UINT8 rule,
  UINT32 imcTADChnlIlvOffset0Reg,
  UINT32 *TADOffsetList,
  UINT8 *TADChnIndexList
  )
/*++

  Write the TAD Interleave Offset register

  @param host                    - Pointer to sysHost
  @param ha                      - HA/iMC number (0-based)
  @param ch                      - Channel number (0-based)
  @param rule                    - Rule number
  @param imcTADChnlIlvOffset0Reg - TAD Channel Interleave Offset Register
  @param TADOffsetList           - TAD Offset list
  @param TADChnIndexList         - TAD channel Index list


  @retval N/A

--*/
{

  TADCHNILVOFFSET_0_MCDDC_DESC_HSX_BDX_STRUCT imcTADChnlIlvOffset0;

  struct homeAgent* haMem;

  haMem = &host->var.mem.socket[socket].haList[ha];

  // Only write CSRs if channel is enabled
  if (host->nvram.mem.socket[socket].channelList[ch + (ha * 2)].enabled != 0) {
    imcTADChnlIlvOffset0.Data = 0;
    imcTADChnlIlvOffset0.Bits.tad_offset = TADOffsetList[ch];
    imcTADChnlIlvOffset0.Bits.chn_idx_offset = TADChnIndexList[ch];

    MemWritePciCfgEp (host, socket, ch + (ha * 2), imcTADChnlIlvOffset0Reg + (rule*4), imcTADChnlIlvOffset0.Data);
  }
}


void
WriteTADCSRs (
  PSYSHOST  host,
  UINT8     socket
  )
/*++

  Write the TAD CSRs

  @param host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

--*/
{
  UINT8 i;
  UINT8 ha, haCh;
  struct TADTable *TADEntry;
  struct Socket *socketVar;
  UINT8 *TADInterleaveList;
  UINT32 *TADOffsetList;
  UINT8 *TADChnIndexList;
  struct homeAgent* haMem;

  TAD_0_HA_CFG_STRUCT haTAD0;
  TADWAYNESS_0_MC_MAIN_STRUCT imcTADWayness0;

  socketVar = &host->var.mem.socket[socket];

  // Write TADs for each HA
  for (ha = 0; ha < MAX_HA; ha++) {
    if (host->nvram.mem.socket[socket].haList[ha].enabled == 0) continue;

    haMem = &socketVar->haList[ha];

    for (i = 0; i < TAD_RULES; i++) {

      TADEntry = &haMem->TAD[i];
      TADInterleaveList = haMem->TADintList[i];
      TADOffsetList = haMem->TADOffset[i];
      TADChnIndexList = haMem->TADChnIndex[i];

      // HA TAD CSRs
      haTAD0.Data = 0;
      if (TADEntry->Enable) {
        haTAD0.Bits.channelways = haTADEncodeChWays (host, TADEntry->ChannelWays);
        haTAD0.Bits.socketways = TADEncodeSkWays (TADEntry->SocketWays);
        haTAD0.Bits.channel_0_id = TADInterleaveList[0];
        haTAD0.Bits.channel_1_id = TADInterleaveList[1];
        haTAD0.Bits.channel_2_id = TADInterleaveList[2];
        haTAD0.Bits.channel_3_id = TADInterleaveList[3];
      }
      haTAD0.Bits.limit = SADTADEncodeLimit (TADEntry->Limit);

      MemWritePciCfgMC (host, socket, ha, TAD_0_HA_CFG_REG + (i*4), haTAD0.Data);

      // iMC TAD CSRs

      // iMC TAD Rules
      imcTADWayness0.Data = 0;
      if (TADEntry->Enable) {
        imcTADWayness0.Bits.tad_ch_way = imcTADEncodeChWays (TADEntry->ChannelWays);
        imcTADWayness0.Bits.tad_skt_way = TADEncodeSkWays (TADEntry->SocketWays);
        imcTADWayness0.Bits.tad_ch_tgt0 = TADInterleaveList[0];
        imcTADWayness0.Bits.tad_ch_tgt1 = TADInterleaveList[1];
        imcTADWayness0.Bits.tad_ch_tgt2 = TADInterleaveList[2];
        imcTADWayness0.Bits.tad_ch_tgt3 = TADInterleaveList[3];
      }
      imcTADWayness0.Bits.tad_limit = SADTADEncodeLimit (TADEntry->Limit);

      MemWritePciCfgMC (host, socket, ha, TADWAYNESS_0_MC_MAIN_REG + (i * 4), imcTADWayness0.Data);

      for (haCh = 0; haCh < host->var.mem.numChPerHA; haCh++) {
        // iMC TAD Offsets (Channel 0 - 3)
        WriteTADOffsetForChannel (host, socket, ha, haCh, i, TADCHNILVOFFSET_0_MCDDC_DESC_REG, TADOffsetList, TADChnIndexList);
      }
    } // TAD loop
  } // HA loop
  return;
}

UINT32
RIREncodeLimit (UINT32 Limit)
/*++

  Encode Limit field for RIR DRAM rules

  @param Limit - Limit to encode

  @retval Encoded limit

--*/
{
  // Adjust for 64BM -> 512MB granularity and for upper range limit being <= Limit
  return ((Limit - 1) >> 3);
}

UINT8
RIREncodeRIRWays(UINT8 Ways)
/*++

  Encode RIR "ways" for RIR rule CSR.

  @param Ways  - Ways to encode

  @retval Encoded ways

--*/
{
  switch (Ways) {
    case 1:
      Ways = 0;
      break;
    case 2:
      Ways = 1;
      break;
    case 4:
      Ways = 2;
      break;
    case 8:
      Ways = 3;
      break;
    default:
      Ways = 0;
  }

  return Ways;

}

void
WriteRIRForChannel(PSYSHOST host,
                   UINT8 socket,
                   UINT8 ch)
/*++

  Write the RIR CSRs

  @param host                    - Pointer to sysHost
  @param socket                  - Socket number
  @param ch                      - Channel number (0-based)

  @retval N/A

--*/
{
  UINT8 rir, i;
  RIRWAYNESSLIMIT_0_MCDDC_DESC_BDX_STRUCT imcRIRWaynessLimit0;
  RIRILV0OFFSET_0_MCDDC_DESC_BDX_STRUCT   imcRIRIlv0Offset0Bdx;
  RIRILV0OFFSET_0_MCDDC_DESC_HSX_STRUCT   imcRIRIlv0Offset0Hsx;
  struct ddrChannel *channel;
  struct channelNvram *channelNvList;

  channelNvList = &host->nvram.mem.socket[socket].channelList[ch];

  // Only write CSRs if channel is enabled
  if (channelNvList->enabled != 0) {

    channel = &host->var.mem.socket[socket].channelList[ch];

    for (rir = 0; rir < MAX_RIR; rir++) {

      imcRIRWaynessLimit0.Data = 0;
      imcRIRWaynessLimit0.Bits.rir_val = channel->rirValid[rir];
      imcRIRWaynessLimit0.Bits.rir_limit = RIREncodeLimit (channel->rirLimit[rir]);
      imcRIRWaynessLimit0.Bits.rir_way = RIREncodeRIRWays(channel->rirWay[rir]);

      MemWritePciCfgEp (host, socket, ch, RIRWAYNESSLIMIT_0_MCDDC_DESC_REG + (rir * 4), imcRIRWaynessLimit0.Data);

      for (i = 0; i < MAX_RIR_WAYS; i++) {
        if (host->var.common.cpuType == CPU_HSX) {
          imcRIRIlv0Offset0Hsx.Data = 0;
          imcRIRIlv0Offset0Hsx.Bits.rir_rnk_tgt0 = channel->rirIntList[rir][i];
          imcRIRIlv0Offset0Hsx.Bits.rir_offset0 = channel->rirOffset[rir][i];

          MemWritePciCfgEp (host, socket, ch, RIRILV0OFFSET_0_MCDDC_DESC_REG + (rir * 8 * 4) + (i * 4), imcRIRIlv0Offset0Hsx.Data);
        } else if (host->var.common.cpuType == CPU_BDX) {
          imcRIRIlv0Offset0Bdx.Data = 0;
          imcRIRIlv0Offset0Bdx.Bits.rir_rnk_tgt0 = channel->rirIntList[rir][i];
          imcRIRIlv0Offset0Bdx.Bits.rir_offset0 = channel->rirOffset[rir][i];

          MemWritePciCfgEp (host, socket, ch, RIRILV0OFFSET_0_MCDDC_DESC_REG + (rir * 8 * 4) + (i * 4), imcRIRIlv0Offset0Bdx.Data);
        }
      } /* rank way loop */
    } /* rir loop */
  } /* if channel enabled */

  return;

}

//-----------------------------------------------------------------------------
// WriteRIRCSRs
//
// Description: Fill in RIR CRSs for memory map from config info in host
// structure.
//
//-----------------------------------------------------------------------------

void
WriteRIRCSRs (PSYSHOST host, UINT8 socket)
/*++

  Write the RIR CSRs

  @param host    - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

--*/
{
  UINT8 ch;

  // RIRs for channels 0 - 3
  for (ch = 0; ch < MAX_CH; ch++) {
    WriteRIRForChannel (host, socket, ch);
  }

  return;
}

void
WriteSpareRankCSRs (PSYSHOST host, UINT8 socket)
/*++

  Write logical rank ids of spare ranks to non-sticky scratchpad register.
  HSD 3612093.

  @param host    - Pointer to sysHost
  @param socket  - socket to process

  @retval N/A

--*/
{
/*
  UINT8 ch;
  UINT32 spareranks = 0;
  struct channelNvram *channelNvList;

  //
  // Check if sparing enabled
  //
  if (host->nvram.mem.RASmode & RK_SPARE) {
    // bits 0:2   logical rank id of ch 0 spare or failed rank
    // bit  3     ch0 enabled
    // bits 4:6   logical rank id of ch 1 spare or failed rank
    // bit  7     ch1 enabled
    // bits 8:10  logical rank id of ch 2 spare or failed rank
    // bit  11    ch 2 enabled
    // bits 12:14 logical rank id of ch 3 spare or failed rank
    // bit  15    ch 3 enabled
    // bit  16    ch 0 sparing occurred
    // bit  17    ch 1 sparing occurred
    // bit  18    ch 2 sparing occurred
    // bit  19    ch 3 sparing occurred

    for (ch = 0; ch < MAX_CH; ch++) {
     channelNvList = &host->nvram.mem.socket[socket].channelList[ch];

      //
      // Only include enabled channels
      //
      if (channelNvList->enabled != 0) {
        //
        // Bit 3 of nibble is channel enabled bit, bits 2:0 of nibble are logical id of spare rank
        //
        spareranks |= (0x8 | (channelNvList->spareLogicalRank & 0x7)) << ch*4;
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "SpareLogRank:0x%x ", channelNvList->spareLogicalRank));
      }
    }
  } else {
    spareranks = 0;
  }

  //
  // Write CSR
  //
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "BIOSNONSTICKYSCRATCHPAD4 :0x%x\n", spareranks));
  return;
  */
}


void
WriteTolmTohmCSRs (PSYSHOST host)
/*++

  Fill in TOLM/TOHM CRSs for memory map from config info in host
  structure. For all sockets.

  @param host  - Pointer to sysHost

  @retval N/A

--*/
{
  UINT8 socket;
  UINT8 socketsEn = 0;
  UINT32 SADLimit;
  UINT32 TOLMLimit;
  UINT32 tolmTmp;
  struct memVar *mem;
  struct memNvram *nvramMem;
  TOLM_IIO_VTD_STRUCT tolm_5_0;
  TOHM_0_IIO_VTD_STRUCT  tohm_0_5_0;
  TOHM_1_IIO_VTD_STRUCT  tohm_1_5_0;
  UINT8 maxSADRules = MAX_SAD_RULES;

  nvramMem = &host->nvram.mem;
  mem = &host->var.mem;

  tolm_5_0.Data = 0;
  tohm_0_5_0.Data = 0;
  tohm_1_5_0.Data = 0;

  // Program the TOLM/TOHM based on the SAD rule limits
  // Note: IIO tolm/tohm (device 0/5)  -- addr range <= limit
  //       CBO tolm/tohm (device 12/7) -- addr range <  limit

  // Compute SADLimit and TOLM such that  "addr range" <= {SAD,TOLM}Limit (64MB granularity)
  SADLimit = mem->SAD[maxSADRules - 1].Limit - 1;
  TOLMLimit = ((FOUR_GB_MEM - host->setup.common.lowGap) & ~0x3) - 1;

  if (TOLMLimit < SADLimit) {
    tolm_5_0.Bits.addr = TOLMLimit;
  } else {
    if ((SADLimit + 1) & 0x3) {
      tolmTmp = (0x4 - ((SADLimit + 1) & 0x3)) + SADLimit;
    } else {
      tolmTmp = SADLimit;
    }
    tolm_5_0.Bits.addr = tolmTmp;
  }

  host->var.mem.lowMemBase = 0;
  host->var.mem.lowMemSize = tolm_5_0.Bits.addr + 1;

  // highMemBase initialized in InitMemoryMap

  // Calculating the number of sockets on which IOT is enabled
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0) || (host->var.mem.iotDisabled[socket] == 1)) continue;
    socketsEn +=1;
  }

  if (SADLimit < host->var.mem.highMemBase) {
    // No memory above 4GB
    host->var.mem.highMemSize = 0;
  } else {
      host->var.mem.highMemSize = (SADLimit - host->var.mem.highMemBase - ((host->setup.mem.iotMemBufferRsvtn)*socketsEn) + 1); // i.e. For 1 socket IOTMemory*1
  }

  if (SADLimit >= FOUR_GB_MEM) {
     tohm_0_5_0.Bits.addr = SADLimit;
     tohm_1_5_0.Bits.addr = SADLimit >> 6;
  } else {
     tohm_0_5_0.Bits.addr = 0x3F;
     tohm_1_5_0.Bits.addr = 0x3F >> 6;
  }

  //
  // Save TOLM into host structure for later use
  //
  host->var.common.tolmLimit = (UINT16)(tolm_5_0.Bits.addr + 1);

  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "highMemBase: 0x%x\nhighMemSize: 0x%x\nTOLM: 0x%x\nTOHM: 0x%x\n", host->var.mem.highMemBase
                , host->var.mem.highMemSize, tolm_5_0.Bits.addr, (((UINT32)tohm_1_5_0.Bits.addr << 6) | tohm_0_5_0.Bits.addr)));

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (nvramMem->socket[socket].enabled == 0) continue;

    WriteCpuPciCfgEx (host, socket, 0, TOLM_IIO_VTD_REG, tolm_5_0.Data);

    WriteCpuPciCfgEx (host, socket, 0, TOHM_0_IIO_VTD_REG, tohm_0_5_0.Data);
    WriteCpuPciCfgEx (host, socket, 0, TOHM_1_IIO_VTD_REG, tohm_1_5_0.Data);
  } // socket loop

  return;
}



void
WriteMemoryMapCSRs(PSYSHOST host)
/*++

  Fill in SAD/TAD/RIR CRSs for memory map from config info in host structure

  @param host  - Pointer to sysHost

  @retval N/A

--*/
{
  UINT8 socket;
  UINT8 ha;
  struct memNvram *nvramMem;
  struct memVar *mem;
  MCMTR_MC_MAIN_HSX_BDX_STRUCT                mcMtr;
  HASYSDEFEATURE2_HA_CFG_HSX_BDX_STRUCT       haSYSDEFEATURE2;

  nvramMem = &host->nvram.mem;
  mem = &host->var.mem;

  // Write the SAD data from structures to CSRs
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (nvramMem->socket[socket].enabled == 0) continue;
    OutputExtendedCheckpoint((host, STS_DDR_MEMMAP, SUB_WRITE_SAD, socket));
    WriteSADCSRs (host, socket);
    OutputExtendedCheckpoint((host, STS_DDR_MEMMAP, SUB_WRITE_TAD, socket));
    WriteTADCSRs (host, socket);
    OutputExtendedCheckpoint((host, STS_DDR_MEMMAP, SUB_WRITE_RIR, socket));
    WriteRIRCSRs (host, socket);
    OutputExtendedCheckpoint((host, STS_DDR_MEMMAP, SUB_WRITE_SPARE_RANK, socket));
    WriteSpareRankCSRs (host, socket);
    for (ha = 0; ha < MAX_HA; ha++) {
      if(host->var.mem.socket[socket].imcEnabled[ha] == 0) continue;
      //
      // Setup HASYSDEFEATURE2.mcchanshiftupenable an MCMTR.pass76 if A7_MODE is enabled
      //
      if(host->nvram.mem.RASmodeEx & A7_MODE_EN){
        haSYSDEFEATURE2.Data = MemReadPciCfgMC(host, socket, ha, HASYSDEFEATURE2_HA_CFG_REG);
        haSYSDEFEATURE2.Bits.mcchanshiftupenable = 1;
        MemWritePciCfgMC(host, socket, ha, HASYSDEFEATURE2_HA_CFG_REG, haSYSDEFEATURE2.Data);

        mcMtr.Data = MemReadPciCfgMC(host, socket, ha, MCMTR_MC_MAIN_REG);
        mcMtr.Bits.pass76 = 1;
        MemWritePciCfgMC(host, socket, ha, MCMTR_MC_MAIN_REG, mcMtr.Data);
      }
    }
  }

  WriteTolmTohmCSRs (host);

  return;
}

//
//-----------------------------------------------------------------------------
// Initialize memory map
//
// Description: Compute contents for memory map CSRs to allocate physical address space to sockets,
// channels, and dimm ranks. Writes necessary memory map CSRs. Displays memory map
// configuration for debug.
//
//-----------------------------------------------------------------------------

UINT32 InitMemoryMap(PSYSHOST host)
/*++

  Compute contents for memory map CSRs to allocate physical address space to sockets,
  channels, and dimm ranks. Writes necessary memory map CSRs. Displays memory map
  configuration for debug.

  @param host  - Pointer to sysHost

  @retval SUCCESS

--*/
{
#if MAX_SOCKET > 1
  UINT8   socket;
  UINT8   numSockets;
#endif
  UINT16  TOLM;

  //
  // Make sure there is still memory present after running MemTest
  //
  if (!CheckMemPresentStatus (host)){
      MemDebugPrint((host, 0xFFFF, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "All channels disabled due to Memory Test failures!\n"));
      OutputError (host, ERR_NO_MEMORY, ERR_NO_MEMORY_MINOR_ALL_CH_DISABLED, 0, 0xFF, 0xFF, 0xFF);
  }

  //
  // Top of Low Memory equals 4GB - size of the low Gap
  // TOLM in 64MB units
  //
  TOLM = (UINT16)(FOUR_GB_MEM - host->setup.common.lowGap);

  // Has to be on a 256MB boundary
  TOLM = TOLM & ~0x3;

  //
  // maxAddrMem and highGap are in 256MB units. tohmLimit is 64MB granularity.
  //
  host->var.common.tohmLimit = (host->setup.common.maxAddrMem - host->setup.common.highGap) * 4;


  host->var.mem.highMemBase = FOUR_GB_MEM;

  // Populate memory size fields
  PopulateMemorySizeFields(host);

  // Interleave channels if NUMA is not supported and more than one socket has memory
#if MAX_SOCKET > 1
  numSockets = 0;
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (host->nvram.mem.socket[socket].enabled == 0)
      continue;

    if (host->var.mem.socket[socket].memSize != 0) {
      numSockets += 1;
    }
  }

  if ((!(host->setup.mem.options & NUMA_AWARE) || (host->var.mem.socketInter > 1)) && (numSockets > 1)) {
    SocketInterleave(host, TOLM);
  } else
#endif

  SocketNonInterleave(host, TOLM);

  // Do rank interleaving
  OutputExtendedCheckpoint((host, STS_DDR_MEMMAP, SUB_RANK_INTER, 0));
  RankInterleave(host);

#ifdef   SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) {
    getPrintFControl (host);
    DisplayMemoryMapInfo(host);
    releasePrintFControl (host);
  }
#endif

  WriteMemoryMapCSRs(host);

  return SUCCESS;
}

#ifdef   SERIAL_DBG_MSG
void DisplayRIRMapInfo(PSYSHOST host, UINT8 socket, UINT8 ch)
/*++

  Display RIR Info

  @param host  - Pointer to sysHost
  @param socket  - Socket number
  @param ch    - Channel number (0-based)

  @retval N/A

--*/
{
  UINT8 rir, i;
  struct ddrChannel *channel;

  channel = &host->var.mem.socket[socket].channelList[ch];

  // Display Interleave map
  // Display RIR table

  rcPrintf ((host, "\n----------- RIR Info ----------------\n"));
  rcPrintf ((host, "\n RIR Table (Socket %d, Channel %d)\n", socket, ch));
  rcPrintf ((host, "----------------------------------------------\n"));

  for (rir = 0; rir < MAX_RIR; rir++) {
    if (channel->rirValid[rir] == 0) continue;

    rcPrintf ((host, "\n Rule \tEnable \tLimit(Ch Space) \tWays"));

    rcPrintf ((host, "\n %d\t%d\t0x%x", rir, channel->rirValid[rir], channel->rirLimit[rir]));
    if (channel->rirValid[rir] == 1) {
      rcPrintf ((host, "\t\t\t%d", channel->rirWay[rir]));
      // Print rank interleave targets and offsets
      rcPrintf ((host, "\n\n\tRank Interleave List\n\tWay \tTarget \tOffset"));
      for (i = 0; i < MAX_RIR_WAYS; i++) {
        rcPrintf ((host, "\n\t%d\t%d\t%d", i, channel->rirIntList[rir][i], channel->rirOffset[rir][i]));
      } // rank way loop
    }
    rcPrintf ((host, "\n"));
  } // rir loop
  return;
}

void DisplayChannelInfo(PSYSHOST host, UINT8 socket)
/*++

  Display Channel Info

  @param host    - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

--*/
{

  UINT8 ch;
  struct Socket *socketVar;
  struct ddrChannel *channel;

  socketVar = &host->var.mem.socket[socket];

  rcPrintf ((host, "\n\n----------- Channel Info ----------------"));

  for (ch = 0; ch < MAX_CH; ch++) {
    rcPrintf ((host, "\n\n----------- Channel %d\n", ch));

    if (host->nvram.mem.socket[socket].channelList[ch].enabled == 1) {
      rcPrintf ((host, " Channel Enabled\n"));
    }
    else {
      rcPrintf ((host, " Channel not enabled\n"));
      continue;
    }

    channel = &socketVar->channelList[ch];

    rcPrintf ((host, " Channel mem size (64MB) = 0x%x\n", channel->memSize));
#ifdef NVMEM_FEATURE_EN
    rcPrintf ((host, " Channel NV mem size (64MB) = 0x%x\n", channel->NVmemSize));
#endif // NVMEM_FEATURE_EN

    DisplayRIRMapInfo (host, socket, ch);
  } // ch
}

void DisplayTADMapInfo(PSYSHOST host, UINT8 socket)
/*++

  Display TAD Info

  @param host    - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

--*/
{
  UINT8 i,j;
  UINT8 ha;
  struct Socket *socketVar;
  struct TADTable *TADEntry;
  struct homeAgent *haMem;

  socketVar = &host->var.mem.socket[socket];

  // Display Interleave map

  // Display TAD table

  rcPrintf ((host, "\n----------- TAD Info ----------------\n"));
  rcPrintf ((host, "\n TAD Table (Socket %d)\n", socket));
  rcPrintf ((host, " Rule \tEnable \tLimit \tMode \tCh Ways\n"));
  rcPrintf ((host, "---------------------------------------"));

  for (ha = 0; ha < MAX_HA; ha++) {
    if (host->nvram.mem.socket[socket].haList[ha].enabled == 0) continue;

    rcPrintf ((host, "\n Home Agent %d\n", ha));

    haMem = &socketVar->haList[ha];

    for (i = 0; i < TAD_RULES; i++) {
      TADEntry = &haMem->TAD[i];
      if (TADEntry->Enable == 0) continue;

      rcPrintf ((host, "\n %d\t%d\t0x%x", i, TADEntry->Enable, TADEntry->Limit));
      if (TADEntry->Enable == 1) {
        rcPrintf ((host, "\t%d\t%d\n", TADEntry->Mode, TADEntry->ChannelWays));
        // Print TAD interleave targets and offsets
        rcPrintf ((host, "\tTAD Interleave List\n\tWay \tTarget \tOffset \tChIndex"));
        for (j = 0; j < MAX_TAD_WAYS; j++) {
          rcPrintf ((host, "\n\t%d\t%d\t0x%x\t%d", j, haMem->TADintList[i][j], haMem->TADOffset[i][j], haMem->TADChnIndex[i][j]));
        } // TAD way loop
          rcPrintf ((host, "\n"));
      }
    } // TAD loop
  } // HA loop
  return;
}

void DisplayNodeInfo(PSYSHOST host)
/*++

  Display Node Info

  @param host  - Pointer to sysHost

  @retval N/A

--*/
{
  UINT8 socket;
  struct memVar *mem;
  struct Socket *socketVar;

  mem = &host->var.mem;

  rcPrintf ((host, "\n\n----------- Socket Info ----------------"));

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    rcPrintf ((host, "\n\n----------- Socket %d\n", socket));
    socketVar = &mem->socket[socket];

    if (host->nvram.mem.socket[socket].enabled == 1) {
      rcPrintf ((host, " Socket Enabled\n"));
    }
    else {
      rcPrintf ((host, " Socket not enabled\n"));
      continue;
    }

    rcPrintf ((host, " Socket max DIMM pop count = %d\n", host->nvram.mem.socket[socket].maxDimmPop));

    if (host->nvram.mem.socket[socket].maxDimmPop == 0)
      continue;

    rcPrintf ((host, " Socket mem size (64MB) = 0x%x\n", socketVar->memSize));
#ifdef NVMEM_FEATURE_EN
    rcPrintf ((host, " Node NV mem size (64MB) = 0x%x\n", socketVar->NVmemSize));
#endif // NVMEM_FEATURE_EN

    // rcPrintf ((host, "Channel Interleave ways = %d\n", socketVar->));

    DisplayTADMapInfo (host, socket);

    DisplayChannelInfo (host, socket);
  }
}

void DisplaySADMapInfo(PSYSHOST host)
/*++

  Display SAD Info

  @param host  - Pointer to sysHost

  @retval N/A

--*/
{
  UINT8 i, j;
  struct memVar *mem;
  struct SADTable *SADEntry;
  UINT8 maxSADRules = MAX_SAD_RULES;

  // Display Interleave map

  mem = &host->var.mem;

  // Display SAD table
  rcPrintf ((host, "\n SAD Table\n"));
  rcPrintf ((host, " Rule Enable \tLimit \tMode \tWays \tInterleave List(right to left)\n"));
  rcPrintf ((host, "-------------------------------------------------------------------"));

  for (i = 0; i < maxSADRules; i++) {
    SADEntry = &mem->SAD[i];
    if (SADEntry->Enable == 0) continue;

    rcPrintf ((host, "\n %d\t%d\t0x%x", i, SADEntry->Enable, SADEntry->Limit));
    if (SADEntry->Enable == 1) {
      rcPrintf ((host, "\t%d\t%d\t", SADEntry->Mode, SADEntry->ways));
      for (j = 0; j < MAX_SAD_WAYS; j++) {
        rcPrintf ((host, "%d", mem->SADintList[i][(MAX_SAD_WAYS - 1) - j]));
      }
    }
  } // for
  return;
}

void DisplaySystemInfo(PSYSHOST host)
/*++

  Display Syste, Info

  @param host  - Pointer to sysHost

  @retval N/A

--*/
{
  struct memVar *mem;
  struct memNvram *nvMem;

  // System info
  rcPrintf ((host, "\n\t\t----------- Memory Map Info ----------------\n"));

  mem = &host->var.mem;
  nvMem = &host->nvram.mem;

  rcPrintf ((host, " Socket XOR Config = "));

  switch (mem->InterleaveMode) {
    case 0:
      rcPrintf ((host, "Non-XOR mode\n"));
      break;
    case 1:
      rcPrintf ((host, "XOR mode\n"));
      break;
  }

  rcPrintf ((host, " Socket RAS Config = "));

  if (nvMem->RASmode & CH_MIRROR)   rcPrintf ((host, "Channel Mirror  "));
  if (nvMem->RASmode & CH_LOCKSTEP) rcPrintf ((host, "Channel Lockstep"));
  if (!(nvMem->RASmode & CH_ML))    rcPrintf ((host, "Channel Independent"));
  if (nvMem->RASmode & RK_SPARE)    rcPrintf ((host, "Rank Sparing"));
  rcPrintf ((host, "\n"));

  if (host->setup.mem.options & NUMA_AWARE)
    rcPrintf ((host, " NUMA Config\n"));
  else
    rcPrintf ((host, " Non-NUMA Config"));

  rcPrintf ((host, " Socket Interleave Ways: %d\n", mem->socketInter));

  rcPrintf ((host, " System Mem Size (64MB granularity): 0x%x\n", mem->memSize));
#ifdef NVMEM_FEATURE_EN
  rcPrintf ((host, " System NV Mem Size (64MB granularity): %d\n", mem->NVmemSize));
#endif // NVMEM_FEATURE_EN

  DisplaySADMapInfo (host);

  return;
}

void DisplayMemoryMapInfo(PSYSHOST host)
/*++

  Display Memory Map Info

  @param host  - Pointer to sysHost

  @retval N/A

--*/
{

  // Display System info and SAD table
  DisplaySystemInfo (host);

  // Display Node info (TAD and RIR tables)
  DisplayNodeInfo (host);

  rcPrintf ((host, "\n"));
}

void PrintSize(PSYSHOST host, UINT16 size)
/*++

  Print the provided size in MB or GB as appropriate

  @param host  - Pointer to sysHost
  @param size  - Memory size in 64MB units

  @retval N/A

--*/
{
   UINT16 tmp = 0;

   tmp = (size & 0xF) * 64;
   size = size & 0xFFF0;
   if (size) {
      rcPrintf ((host, "%3d",size >> 4));
      if (tmp) {
         tmp = (1000*tmp)/1024;
         while(!(tmp%10)) tmp = tmp/10;
         rcPrintf ((host, ".%iGB",tmp));
      }
      else rcPrintf ((host, "GB "));
   } else if (tmp) {
      rcPrintf ((host, "%3iMB ",tmp));
   }

   if (!(size || tmp)) rcPrintf ((host, "   0  "));
}
#endif // SERIAL_DBG_MSG

