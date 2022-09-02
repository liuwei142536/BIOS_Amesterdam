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
#include "MemFunc.h"
#include "MemHost.h"

extern  UINT32 preciseTCK[MAX_SUP_FREQ];

//
// Local function prototypes
//
VOID
InitSpd (
  PSYSHOST host,
  UINT8 socket
);

//                                       |   512Mb   |    1Gb    |    2Gb    |    4Gb    |     8Gb   |4Gb2xRM|4Gb4xRM|
const   UINT8 primaryWidth[MAX_TECH]   = {  4,  8, 16,  4,  8, 16,  4,  8, 16,  4,  8, 16,  4,  8, 16,  4,  8,  4,  8};   // Primary SDRAM device width
const   UINT8 rowBits[MAX_TECH]        = { 13, 13, 12, 14, 14, 13, 15, 15, 14, 16, 16, 15, 16, 16, 16, 17, 17, 18, 18};   // Number of row address bits
const   UINT8 columnBits[MAX_TECH]     = { 11, 10, 10, 11, 10, 10, 11, 10, 10, 11, 10, 10, 12, 11, 10, 11, 10, 11, 10};   // Number of column address bits
const   UINT8 internalBanks[MAX_TECH]  = {  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8};   // Number of internal SDRAM banks
const   UINT16 rankSize[MAX_TECH]      = { 16,  8,  4, 32, 16,  8, 64, 32, 16,128, 64, 32,256,128, 64,256,128,512,256};   // Rank size in 64 MB units

//                                          |      2Gb     |      4Gb     |      8Gb     |     16Gb     |
const   UINT8 primaryWidthDDR4[MAX_TECH]  = {   4,   8,  16,   4,   8,  16,   4,   8,  16,   4,   8,  16};   // Primary SDRAM device width
const   UINT8 rowBitsDDR4[MAX_TECH]       = {  15,  14,  14,  16,  15,  15,  17,  16,  16,  18,  17,  17};   // Number of row address bits
const   UINT8 columnBitsDDR4[MAX_TECH]    = {  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10};   // Number of column address bits
const   UINT8 bankGroupsDDR4[MAX_TECH]    = {   4,   4,   2,   4,   4,   2,   4,   4,   2,   4,   4,   2};   // Number of internal SDRAM banks
const   UINT16 rankSizeDDR4[MAX_TECH]     = {  64,  32,  16, 128,  64,  32, 256, 128,  64, 512, 256, 128};   // Rank size in 64 MB units

const   UINT16 rankSizeAEP[MAX_TECH_AEP] = {128, 256,  512, 1024, 2048,4096,8192,16384}; // Rank Size in 64 MB Units

//
// Timing tables are indexed by these frequencies:
// 800  1000  1067  1200  1333  1400  1600  1800  1867  2000  2133  2200  2400  2600  2667  2800  2933  3000  3200
//
UINT8 tFAWtable[2][MAX_SUP_FREQ] = {   // tFAW lookup table
  // units of DCLK calculated from DDR3 spec tFAW in nsec / actual tCK (rounded up)
  {16, 19, 20, 23, 20, 21, 24, 25, 26, 27, 27, 28, 31, 33, 34, 34, 34, 34, 34},    // 1K Page Size
  {20, 25, 27, 30, 30, 32, 32, 32, 33, 35, 38, 39, 43, 46, 47, 47, 47, 47, 47}     // 2K Page Size
};

UINT8 tFAWTableDDR4[3][MAX_SUP_FREQ] = {  //DDR4 tFAW lookup table
  // units of DCLK calculated from DDR4 spec, tFAW in nsec / actual tCK (rounded up)
  {14, 14, 14, 14, 14, 14, 16, 16, 16, 17, 16, 17, 16, 17, 16, 17, 18, 18, 16},    // 1/2K Page Size
  {17, 17, 17, 17, 17, 18, 20, 21, 22, 23, 23, 24, 26, 28, 28, 30, 31, 32, 34},    // 1K Page Size
  {24, 24, 24, 24, 24, 25, 28, 28, 29, 30, 32, 34, 37, 40, 40, 43, 44, 45, 48}     // 2K Page Size
};

UINT8 tRTPTableDDR4[MAX_SUP_FREQ] = {4, 5, 6, 6, 6, 6, 6, 7, 8, 8, 8, 9, 10, 10, 10, 11, 11, 12, 12};

// DDR4 tWTR_L & tWTR_S lookup table;
// units of DCLK calcualted from DDR4 spec, tWTR_x in ns / actual tCK (rounded up)
// units of DCLK calculated from DDR4 spec tWTR_L in nsec / actual tCK (rounded up)
UINT8 tWTRLTableDDR4[MAX_SUP_FREQ] = {4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12};
UINT8 tWTRSTableDDR4[MAX_SUP_FREQ] = {2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4};

// DDR4 tWR lookup table
// units of DCLK calcualted from DDR4 spec, tWR in ns / actual tCK (rounded up)
UINT8 tWRTableDDR4[MAX_SUP_FREQ] = {10, 10, 10, 10, 10, 11, 12, 14, 15, 15, 16, 17, 19, 20, 20, 22, 22, 23, 24};


// Table for how the Module Delay bytes are adjusted per frequency in MTP units
// The general equation for the delay adjustment value is ROUNDDOWN(((tCK - 2.5ns) * 3/4) / tMTB)
// 800:    (2.5ns   - 2.5ns) * 3/4 / 0.125 = 0     = 0
// 1066:   (1.875ns - 2.5ns) * 3/4 / 0.125 = 3.75  = 3
// 1333:   (1.5ns   - 2.5ns) * 3/4 / 0.125 = 6     = 6
// 1600:   (1.25ns  - 2.5ns) * 3/4 / 0.125 = 7.5   = 7
// 1876:   (1.066ns - 2.5ns) * 3/4 / 0.125 = 8.6   = 8
// 2133:   (0.938ns - 2.5ns) * 3/4 / 0.125 = 9.37  = 9
// 2400:   (0.833ns - 2.5ns) * 3/4 / 0.125 = 9.996 = 10
// 2667:   (0.75ns - 2.5ns) * 3/4 / 0.125 = 10.5 = 10
// 2933:   (0.682ns - 2.5ns) * 3/4 / 0.125 = 10.908 = 11
// 3200:   (0.625ns - 2.5ns) * 3/4 / 0.125 = 11.25 = 11

UINT8 lrDimmModuleDelayAdjust[MAX_SUP_FREQ] = { 0, 2, 3, 5, 6, 7, 7, 8, 8, 9, 9, 10, 10, 10, 10, 10, 11, 11, 11};


UINT32
DetectDIMMConfig (
                 PSYSHOST host
                 )
/**

  Detect DIMM population

  @param host  - Point to sysHost

  @retval SUCCESS

**/
{
  UINT8               socket;
  UINT8               ch;
  UINT8               dimm;
  UINT8               SPDReg;
  UINT8               i;
  UINT32              dimmFound;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct smbDevice    spd;
  UINT16              SPDMMfgId = 0;
  UINT16              SPDModDate = 0;

  socket = host->var.mem.currentSocket;

  //
  // Initialize the smbus base address
  //
  InitSmb (host, socket);
  //
  // Initialize SPD page select
  //
  InitSpd (host, socket);

  channelNvList = GetChannelNvList(host, socket);
  UpdateAccessMode(host, socket);

  // Check if DIMMs changed on ColdFastBoot
  if (host->var.mem.subBootMode == ColdBootFast || host->var.mem.socket[socket].socketSubBootMode == ColdBootFast){
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Check DIMM's for ColdFastBoot\n"));

    WaitForMailboxReady (host, socket);

    for (ch = 0; ch < MAX_CH; ch++) {
      dimmNvList = GetDimmNvList(host, socket, ch);
      if (host->var.mem.socket[socket].socketSubBootMode == ColdBoot) break;
      for (dimm = 0; dimm < host->var.mem.socket[socket].channelList[ch].numDimmSlots; dimm++){
        dimmFound = ReadSpd (host, socket, ch, dimm, SPD_KEY_BYTE, &SPDReg);
        if (((*dimmNvList)[dimm].dimmPresent == 0 && dimmFound == SUCCESS) ||
          ((*dimmNvList)[dimm].dimmPresent == 1 && dimmFound != SUCCESS)){
          //host->var.mem.subBootMode = ColdBoot;
          host->var.mem.socket[socket].socketSubBootMode = ColdBoot;

          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                  "DIMM mismatch found\n"));
          break;
        } else if (dimmFound == SUCCESS){
          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                "DIMM found check SN\n"));
          // Check Mfg ID
          if (SPDReg == SPD_TYPE_DDR3) {
            ReadSpd (host, socket, ch, dimm, SPD_MMID_MSB, &SPDReg);
            SPDMMfgId = (UINT16) (SPDReg << 8);
            ReadSpd (host, socket, ch, dimm, SPD_MMID_LSB, &SPDReg);
            SPDMMfgId |= (UINT16) (SPDReg &~BIT7);
          } else {
            ReadSpd (host, socket, ch, dimm, SPD_MMID_MSB_DDR4, &SPDReg);
            SPDMMfgId = (UINT16) (SPDReg << 8);
            ReadSpd (host, socket, ch, dimm, SPD_MMID_LSB_DDR4, &SPDReg);
            SPDMMfgId |= (UINT16) (SPDReg &~BIT7);
          }
          if ((*dimmNvList)[dimm].SPDMMfgId != SPDMMfgId) {
            host->var.mem.socket[socket].socketSubBootMode = ColdBoot;
            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                  "DIMM Mfg ID mismatch found\n"));
            break;
          }
          // Check mfg location
          if (SPDReg == SPD_TYPE_DDR3) {
            ReadSpd (host, socket, ch, dimm, SPD_MM_LOC, &SPDReg);
          } else {
            ReadSpd (host, socket, ch, dimm, SPD_MM_LOC_DDR4, &SPDReg);
          }
          if ((*dimmNvList)[dimm].SPDMMfLoc != SPDReg) {
            host->var.mem.socket[socket].socketSubBootMode = ColdBoot;
            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                  "DIMM Mfg Location mismatch found\n"));
            break;
          }
          // Check MSB module date
          if (SPDReg == SPD_TYPE_DDR3) {
            ReadSpd (host, socket, ch, dimm, SPD_MM_DATE, &SPDReg);
            SPDModDate = (UINT16) (SPDReg);
            ReadSpd (host, socket, ch, dimm, SPD_MM_DATE + 1, &SPDReg);
            SPDModDate |= (UINT16) (SPDReg << 8);
          } else {
            ReadSpd (host, socket, ch, dimm, SPD_MM_DATE_YR_DDR4, &SPDReg);
            SPDModDate = (UINT16) (SPDReg);
            ReadSpd (host, socket, ch, dimm, SPD_MM_DATE_WK_DDR4, &SPDReg);
            SPDModDate |= (UINT16) (SPDReg << 8);
          }
          if ((*dimmNvList)[dimm].SPDModDate != SPDModDate) {
            host->var.mem.socket[socket].socketSubBootMode = ColdBoot;
            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                  "DIMM Module date mismatch found\n"));
            break;
          }
          // Check module Serial Number Bytes 320 - 328
          for (i = 0; i < 4; i++) {
            
            GetModuleSerialNumber(host, socket, ch, dimm, i, &SPDReg);
            
            if ((*dimmNvList)[dimm].SPDModSN[i] != SPDReg){
              //host->var.mem.subBootMode = ColdBoot;
              host->var.mem.socket[socket].socketSubBootMode = ColdBoot;
              MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                    "DIMM SN mismatch found\n"));
              break;
            }
          } // i loop
          if (host->var.mem.socket[socket].socketSubBootMode == ColdBoot) break;

//           if ((*dimmNvList)[dimm].aepDimmPresent) {
//
//             // SKX TODO: Call NVMCTLR mailbox to get dimm info
//             // Force slow cold boot path if FW revision changed
//           };

        } else {

          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                        "No Dimm\n"));
        }
      } // dimm loop
    } // ch loop
  }

  //
  // Initialize common parts of the smbDevice structure for all SPD devices
  //
  spd.compId = SPD;
  spd.address.controller = PROCESSOR;
  spd.address.deviceType = DTI_EEPROM;

  if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)) {

#ifdef SERIAL_DBG_MSG
    if (checkMsgLevel(host, SDBG_MAX)) {
      getPrintFControl(host);
    }
#endif // SERIAL_DBG_MSG
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "Socket | Channel | DIMM |  Bus Segment | SMBUS Address\n"));
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "-------|---------|------|--------------|--------------\n"));
#ifdef SERIAL_DBG_MSG
    if (checkMsgLevel(host, SDBG_MAX)) {
      releasePrintFControl(host);
    }
#endif // SERIAL_DBG_MSG
    //
    // Detect DIMMs on each channel
    //
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

        //
        // Detect DIMMs in each slot
        //
      dimmNvList = GetDimmNvList(host, socket, ch);

      for (dimm = 0; dimm < host->var.mem.socket[socket].channelList[ch].numDimmSlots; dimm++) {

        OutputExtendedCheckpoint((host, STS_DIMM_DETECT, SUB_INIT_SMB, (UINT16)((socket << 8)|(ch << 4)|(dimm))));
        //
        // Init to DIMM not present
        //
        (*dimmNvList)[dimm].dimmPresent = 0;

        GetSmbAddress(host, socket, ch, dimm, &spd);

        //
        // Read SPD device (type 0x0B) to detect DIMM presence
        //
#ifdef SERIAL_DBG_MSG
        if (checkMsgLevel(host, SDBG_MAX)) {
          getPrintFControl(host);
        }
#endif
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "   %d   |    %2d   |   %d  |      %d       |       %d      - ", socket, ch, dimm,
                       spd.address.busSegment, spd.address.strapAddress));
        if ((ReadSpd (host, socket, ch, dimm, SPD_KEY_BYTE, &SPDReg) == SUCCESS) && (ch == 0)) {

          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "Present\n"));
#ifdef SERIAL_DBG_MSG
          if (checkMsgLevel(host, SDBG_MAX)) {
            releasePrintFControl(host);
          }
#endif
          (*dimmNvList)[dimm].keyByte = SPDReg;
          //
          // We know a DIMM is present if we arrive here
          //
          (*dimmNvList)[dimm].dimmPresent = 1;

          //
          // Increment the number of DIMMs on this channel
          //
          (*channelNvList)[ch].maxDimm++;
          //
          // Get TimeBases
          //

          DetectSpdTypeDIMMConfig(host, socket, ch, dimm, SPDReg);

#ifdef MEM_NVDIMM_EN
          (*dimmNvList)[dimm].nvDimmType = CoreDetectNVDIMM(host, socket, ch, dimm, spd);
#endif
        } // DIMM present end
#ifdef SERIAL_DBG_MSG
        else {
          if (checkMsgLevel(host, SDBG_MAX)) {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "Not Present\n"));
            releasePrintFControl(host);
          }
        }
#endif
      } // dimm loop
    } // ch loop
    // Wait for NVMCTLR to indicate that Mailbox interface is ready
    // SKX TODO: handle errors
    WaitForMailboxReady (host, socket);

    PostDimmDetectChipHook (host, socket);

  } // if cold boot

  return SUCCESS;
}


/**

  Init SPD page select and cached values

  @param host       - Pointer to sysHost
  @param socket     - Socket ID

  @retval none

**/
VOID
InitSpd (
  PSYSHOST         host,
  UINT8            socket
  )
{
  UINT8             seg;

  // Call chip hook to establish DDR4 or DDR3 SPD state
  if (GetDdr4SpdPageEn (host, socket)) {
    host->nvram.mem.socket[socket].ddr4SpdPageEn = 1;

    //
    // Bypass page SPA cache for first read. This is necessary after SPD device power cycle.
    //
    for (seg = 0; seg < MAX_IMC; seg++) {
      host->nvram.mem.socket[socket].imc[seg].spdPageAddr = 2;
    }
  }
} // InitSpd


/**

  Read SPD byte from Serial EEPROM

  @param host       - Pointer to sysHost
  @param socket     - Socket ID
  @param ch         - Channel on socket
  @param dimm       - DIMM on channel
  @param byteOffset - Byte Offset to read
  @param data       - Pointer to data

  @retval status

**/
UINT32
ReadSpd (
  PSYSHOST         host,
  UINT8            socket,
  UINT8            ch,
  UINT8            dimm,
  UINT16           byteOffset,
  UINT8            *data
  )
{
  struct smbDevice spd;
  struct smbDevice spa;
  INT16 spdOffset = 0;
  UINT8 temp = 0;
  UINT32 status;

  spd.compId = SPD;
  spd.address.controller = PROCESSOR;
  spd.address.deviceType = DTI_EEPROM;
  GetSmbAddress(host, socket, ch, dimm, &spd);

  if (host->nvram.mem.socket[socket].ddr4SpdPageEn) {
    if (byteOffset > 255) {

      // If current page does not match desired page
      if (host->nvram.mem.socket[socket].imc[spd.address.busSegment].spdPageAddr != 1) {

        // Update current page
        host->nvram.mem.socket[socket].imc[spd.address.busSegment].spdPageAddr = 1;

        // Set page
        spa = spd;
        spa.address.deviceType = DTI_WP_EEPROM;
        spa.address.strapAddress = SPD_EE_PAGE_SELECT_1;

        // Write to SPA
        WriteSmb (host, socket, spa, 0, &temp);
      }

    } else {

      // If current page does not match desired page
      if (host->nvram.mem.socket[socket].imc[spd.address.busSegment].spdPageAddr != 0) {

        // Update current page
        host->nvram.mem.socket[socket].imc[spd.address.busSegment].spdPageAddr = 0;

        // Set page
        spa = spd;
        spa.address.deviceType = DTI_WP_EEPROM;
        spa.address.strapAddress = SPD_EE_PAGE_SELECT_0;

        // Write to SPA
        WriteSmb (host, socket, spa, 0, &temp);
      }
    }
  }

  // Read from SPD
  status = ReadSmb (host, socket, spd, (byteOffset + spdOffset), data);

  return status;

} //ReadSpd


UINT16
TimeBaseToDCLK (
               PSYSHOST  host,
               UINT8     socket,
               UINT8     ch,
               UINT8     dimm,
               UINT16    mtbTime,
               UINT16    ftbTime
               )
/**

  Converts MTB/FTB to DCLK

  @param mtbTime   - Time in MTB (Medium Time Base)
  @param ftbTime   - Time in FTB (Fine Time Base)

  @retval Time in DCLKs

**/
{
  UINT16              mtb;
  UINT16              ftb;
  UINT32              tempVal;

  mtb = 1250;
  ftb = 10;

  if (ftbTime & BIT7) {
    tempVal = (UINT32)((mtbTime * mtb) + ((ftbTime - 256) * ftb));
  } else {
    tempVal = (UINT32)((mtbTime * mtb) + (ftbTime * ftb));
  }

  //
  // Round up to next cycle
  //
  tempVal = (tempVal + preciseTCK[host->nvram.mem.socket[socket].ddrFreq] - 10) /
             preciseTCK[host->nvram.mem.socket[socket].ddrFreq];

  return(UINT16) tempVal;
} // TimeBaseToDCLK

UINT8
MemReadSmb (
  PSYSHOST         host,
  UINT8            socket,
  struct smbDevice dev,
  UINT16            byteOffset,
  UINT8            *data
  )
{
  return (PlatformReadSmb (host, socket, dev, byteOffset, data));
}

UINT8
MemWriteSmb(
    PSYSHOST         host,
    struct smbDevice dev,
    UINT8            byteOffset,
    UINT8            *data
    )
{
  return (PlatformWriteSmb (host, dev, byteOffset, data));
}
