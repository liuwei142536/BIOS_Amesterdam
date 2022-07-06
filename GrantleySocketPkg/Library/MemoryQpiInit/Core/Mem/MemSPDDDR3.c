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
 * Copyright 2006 - 2017 Intel Corporation All Rights Reserved.
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
#include "MemHost.h"


extern  UINT8 tCK[MAX_SUP_FREQ];
extern  UINT32 preciseTCK[MAX_SUP_FREQ];
extern struct lrbufRefresh RefreshStaggerLookupTable[MAX_SUP_FREQ][MAX_DENSITY][MAX_RM];
extern  UINT8 tXSOFFSET[MAX_SUP_FREQ];
extern  UINT8 primaryWidth[MAX_TECH]; // Primary SDRAM device width
extern  UINT8 rowBits[MAX_TECH];      // Number of row address bits
extern  UINT8 columnBits[MAX_TECH];   // Number of column address bits
extern  UINT8 internalBanks[MAX_TECH];// Number of internal SDRAM banks
extern  UINT8 primaryWidthDDR4[MAX_TECH];   // Primary SDRAM device width
extern  UINT8 tFAWtable[2][MAX_SUP_FREQ];
extern  UINT8 lrDimmModuleDelayAdjust[MAX_SUP_FREQ];

//
// Local Prototypes
//

void   GetDimmTimeBaseDDR3(PSYSHOST host,UINT8 mtbDividend,UINT8 mtbDivisor,UINT8 ftbDividend,UINT8 ftbDivisor,INT32 * mtb,INT32 * ftb);


/**

  Collects data from SPD for DDR3

  @param host  - Point to sysHost

  @retval SUCCESS

**/
UINT32
GatherSPDDataDDR3 (
  PSYSHOST host
  )
{
  UINT8               socket;
  UINT8               ch;
  UINT8               dimm;
  UINT8               SPDReg;
  UINT8               ftbVal;
  UINT8               i;
  UINT8               tCKMTB = 0;
  UINT16              cycles;
#ifdef  LRDIMM_SUPPORT
  UINT8               sb, vb;
  UINT16              nRFC;
  UINT8               density;
#endif  //  LRDIMM_SUPPORT
  UINT8               pageSize;
  UINT32              casSup = 0;
  UINT16              tRC;
  UINT16              tRAS;
  UINT16              tRFC;
  UINT8               tFAW;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct smbDevice    spd;
#ifdef SERIAL_DBG_MSG
  UINT32  serialNum;
  struct dimmDevice (*chdimmList)[MAX_DIMM];
#endif  // SERIAL_DBG_MSG

  socket = host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return SUCCESS;

  //
  // Don't access the SPD if not necessary
  //
  if ((host->nvram.mem.DataGood != 1) || ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot))) {

    //
    // Only gather SPD info if we don't already have it
    //
    channelNvList = &host->nvram.mem.socket[socket].channelList;

    //
    //
    // Initialize common parts of the smbDevice structure for all SPD devices
    //
    spd.compId = SPD;
    spd.address.controller = PROCESSOR;
    spd.address.deviceType = DTI_EEPROM;

    // Check strap addresses on each channel moving near to far
    //
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
#ifdef SERIAL_DBG_MSG
      chdimmList  = &host->var.mem.socket[socket].channelList[ch].dimmList;
#endif  // SERIAL_DBG_MSG

      //
      // Initialize to all CAS Latencies supported
      //
      (*channelNvList)[ch].common.casSup = 0x0000FFFF;

      //
      // Initialize to ASR supported. This bit will get cleared if any DIMM on the channel does not support ASR
      //
      (*channelNvList)[ch].features |= ASR_SUPPORT;

#ifdef LRDIMM_SUPPORT
      //
      // Initialize LRDIMM module delay min/max times
      //
      (*channelNvList)[ch].common.nMDD = 0;
      (*channelNvList)[ch].common.nMDL = 0x7F;
#endif

      //
      // Calculate tCK in MTB in case we had to down clock
      //
      //tCKMTB = (UINT8)((((*dimmNvList)[dimm].mtbDiv * 1000) + host->nvram.mem.socket[socket].ddrFreqMHz - 1) /
      //    host->nvram.mem.socket[socket].ddrFreqMHz);
      //tCKMTB = tCK[host->nvram.mem.ratioIndex];

      for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {

        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        {
        UINT8 rcWriteRegDumpCurrent = host->var.common.rcWriteRegDump;

        host->var.common.rcWriteRegDump = 0;
        OutputExtendedCheckpoint((host, STS_SPD_DATA, SUB_SEND_DATA_SPD, (UINT16)((socket << 8)|(ch << 4)|(dimm))));
        host->var.common.rcWriteRegDump = rcWriteRegDumpCurrent;
        }

        tCKMTB = (UINT8)((((*dimmNvList)[dimm].mtbDiv * 1000) + host->nvram.mem.socket[socket].ddrFreqMHz - 1) /
            host->nvram.mem.socket[socket].ddrFreqMHz);
        //tCKMTB = tCK[host->nvram.mem.ratioIndex];

        spd.address.busSegment = (ch & BIT1) >> 1;
        spd.address.strapAddress = dimm + ((ch & BIT0) * 4);
        if (IsDESku(host)) {
          spd.address.strapAddress = dimm + ((ch & BIT0) * 2);
        }
        spd.mcId = GetMCID(host, socket, ch);
        //
        // Get the page size
        //
        if ((*dimmNvList)[dimm].sdramCapacity == SPD_8Gb) {
          pageSize = (UINT8)PAGESIZE2K;  //All 8Gb DDR3 are of page Size 2K
        } else {
          pageSize = ((*channelNvList)[ch].features & X16_PRESENT) ? (UINT8)PAGESIZE2K : (UINT8)PAGESIZE1K;
        }
        //
        // Use lookup table for tFAW
        //
        tFAW = tFAWtable[pageSize][host->nvram.mem.socket[socket].ddrFreq];

        if ((*channelNvList)[ch].common.nFAW < tFAW) {
          (*channelNvList)[ch].common.nFAW = tFAW;
        }

        //
        // Get tRRD
        //
        ReadSpd (host, socket, ch, dimm, SPD_MIN_TRRD, &SPDReg);
        if ((*channelNvList)[ch].common.tRRD < SPDReg) {
          (*channelNvList)[ch].common.tRRD = SPDReg;

          // Convert to number of cycles
          cycles = TimeBaseToDCLK(host, socket, ch, dimm, (UINT16)SPDReg, 0);
          if ((*channelNvList)[ch].common.nRRD < (UINT8)cycles) {
            (*channelNvList)[ch].common.nRRD = (UINT8)cycles;
          }
        }

        //
        // Get tWTR
        //
        ReadSpd (host, socket, ch, dimm, SPD_MIN_TWTR, &SPDReg);
        if ((*channelNvList)[ch].common.tWTR < SPDReg) {
          (*channelNvList)[ch].common.tWTR = SPDReg;

          // Convert to number of cycles
          cycles = TimeBaseToDCLK(host, socket, ch, dimm, (UINT16)SPDReg, 0);
          if ((*channelNvList)[ch].common.nWTR < (UINT8)cycles) {
            (*channelNvList)[ch].common.nWTR = (UINT8)cycles;
          }
        }

        ReadSpd (host, socket, ch, dimm, SPD_MIN_TRP, &SPDReg);
        ReadSpd (host, socket, ch, dimm, SPD_FTB_TRP, &ftbVal);
#ifdef  SERIAL_DBG_MSG
        (*chdimmList)[dimm].tRP = SPDReg;
#endif  // SERIAL_DBG_MSG
        (*channelNvList)[ch].dimmList[dimm].tRP = SPDReg;
        if ((*channelNvList)[ch].common.tRP < SPDReg) {
          (*channelNvList)[ch].common.tRP = SPDReg;

          // Convert to number of cycles
          cycles = TimeBaseToDCLK(host, socket, ch, dimm, (UINT16)SPDReg, ftbVal);
          if ((*channelNvList)[ch].common.nRP < (UINT8)cycles) {
            (*channelNvList)[ch].common.nRP = (UINT8)cycles;
          }
        }

        //
        // Get upper nibble of tRC and tRAS
        //
        ReadSpd (host, socket, ch, dimm, SPD_EXT_TRC_TRAS, &SPDReg);
        tRC = (UINT16) (SPDReg & 0xF0) << 4;
        tRAS = (UINT16) (SPDReg & 0x0F) << 8;

        //
        // Get lower byte of tRAS
        //

        ReadSpd (host, socket, ch, dimm, SPD_MIN_TRAS, &SPDReg);
        tRAS = tRAS | (UINT16) SPDReg;
        if ((*channelNvList)[ch].common.tRAS < tRAS) {
          (*channelNvList)[ch].common.tRAS = tRAS;

          // Convert to number of cycles
          cycles = TimeBaseToDCLK(host, socket, ch, dimm, tRAS, 0);
          if ((*channelNvList)[ch].common.nRAS < (UINT8)cycles) {
            (*channelNvList)[ch].common.nRAS = (UINT8)cycles;
          }
        }

        //
        // Get lower byte of tRC
        //
        ReadSpd (host, socket, ch, dimm, SPD_MIN_TRC, &SPDReg);
        tRC = tRC | (UINT16) SPDReg;
        ReadSpd (host, socket, ch, dimm, SPD_FTB_TRC, &ftbVal);
        if ((*channelNvList)[ch].common.tRC < tRC) {
          (*channelNvList)[ch].common.tRC = tRC;

          // Convert to number of cycles
          cycles = TimeBaseToDCLK(host, socket, ch, dimm, tRC, ftbVal);
          if ((*channelNvList)[ch].common.nRC < cycles) {
            (*channelNvList)[ch].common.nRC = (UINT8)cycles;
          }
        }

        //
        // Get tRCD
        //
        ReadSpd (host, socket, ch, dimm, SPD_MIN_TRCD, &SPDReg);
        ReadSpd (host, socket, ch, dimm, SPD_FTB_TRCD, &ftbVal);
#ifdef  SERIAL_DBG_MSG
        (*chdimmList)[dimm].tRCD = SPDReg;
#endif  // SERIAL_DBG_MSG
        (*channelNvList)[ch].dimmList[dimm].tRCD = SPDReg;
        if ((*channelNvList)[ch].common.tRCD < SPDReg) {
          (*channelNvList)[ch].common.tRCD = SPDReg;

          // Convert to number of cycles
          cycles = TimeBaseToDCLK(host, socket, ch, dimm, (UINT16)SPDReg, ftbVal);
          if ((*channelNvList)[ch].common.nRCD < cycles) {
            (*channelNvList)[ch].common.nRCD = (UINT8)cycles;
          }
        }

        //
        // Get tRTP
        //
        ReadSpd (host, socket, ch, dimm, SPD_MIN_TRTP, &SPDReg);
        if ((*channelNvList)[ch].common.tRTP < SPDReg) {
          (*channelNvList)[ch].common.tRTP = SPDReg;

          // Convert to number of cycles
          cycles = TimeBaseToDCLK(host, socket, ch, dimm, (UINT16)SPDReg, 0);
          if ((*channelNvList)[ch].common.nRTP < cycles) {
            (*channelNvList)[ch].common.nRTP = (UINT8)cycles;
          }
        }

        //
        // Get upper byte of tRFC
        //
        ReadSpd (host, socket, ch, dimm, SPD_MIN_TRFC_LSB, &SPDReg);
        tRFC = (UINT16) SPDReg;

        //
        // Get lower byte of tRFC
        //
        ReadSpd (host, socket, ch, dimm, SPD_MIN_TRFC_MSB, &SPDReg);
        tRFC = tRFC | ((UINT16) SPDReg << 8);

        if ((*channelNvList)[ch].common.tRFC < tRFC) {
          (*channelNvList)[ch].common.tRFC = tRFC;

          // Convert to number of cycles
          cycles = TimeBaseToDCLK(host, socket, ch, dimm, tRFC, 0);
          if ((*channelNvList)[ch].common.nRFC < cycles) {
            (*channelNvList)[ch].common.nRFC = cycles;
          }
        }

#ifdef LRDIMM_SUPPORT
        if ((*dimmNvList)[dimm].lrRankMult > 1) {
          // Update common nRFC cycles for MC timing
          density = (((*dimmNvList)[dimm].sdramCapacity) - 2) - ((*dimmNvList)[dimm].lrRankMult >> 1);
          // density value should be less than MAX_DENSITY to avoid overflow exception. KWID : 9453
          if (density < MAX_DENSITY) {
            nRFC = RefreshStaggerLookupTable[host->nvram.mem.socket[socket].ddrFreq][density][(*dimmNvList)[dimm].lrRankMult >> 2].nRFC;
            if ((*channelNvList)[ch].common.nRFC < nRFC) {
              (*channelNvList)[ch].common.nRFC = nRFC;
            }
          } else { //warning non supported density
#ifdef SERIAL_DBG_MSG
            (*dimmNvList)[dimm].dimmPresent = 0; //map out dimm
              MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                          "DIMM not supported!\n"));
              OutputWarning (host, WARN_DIMM_COMPAT, WARN_DIMM_COMPAT_MINOR_NOT_SUPPORTED, socket, ch, dimm, 0xFF);
#endif
          }
        }
#endif  //LRDIMM_SUPPORT

        //
        // nXS is the amount of time needed after CKE is asserted before
        // a refresh command can be sent to the DIMMs that are exiting
        // self refresh.
        //
        // nXS = MaxVal( 5, ROUNDUP((common.tRFC + (10ns * 8)) /
        // tCK[imc[socket].freq]))
        //
        (*channelNvList)[ch].common.nXS = (*channelNvList)[ch].common.nRFC + tXSOFFSET[host->nvram.mem.socket[socket].ddrFreq];
        if ((*channelNvList)[ch].common.nXS < 5) {
          (*channelNvList)[ch].common.nXS = 5;
        }

        //
        // Get tWR
        //
        ReadSpd (host, socket, ch, dimm, SPD_MIN_TWR, &SPDReg);
        if ((*channelNvList)[ch].common.tWR < SPDReg) {
          (*channelNvList)[ch].common.tWR = SPDReg;

          // Convert to number of cycles
          cycles = TimeBaseToDCLK(host, socket, ch, dimm, (UINT16)SPDReg, 0);
          if ((*channelNvList)[ch].common.nWR < cycles) {
            (*channelNvList)[ch].common.nWR = (UINT8)cycles;
          }
        }

        //
        // Get tAA
        //
        ReadSpd (host, socket, ch, dimm, SPD_MIN_TAA, &SPDReg);
        ReadSpd (host, socket, ch, dimm, SPD_FTB_TAA, &ftbVal);
#ifdef  SERIAL_DBG_MSG
        (*chdimmList)[dimm].tCL = SPDReg;
#endif  // SERIAL_DBG_MSG
        (*channelNvList)[ch].dimmList[dimm].tCL = SPDReg;
        if ((*channelNvList)[ch].common.tCL < SPDReg) {
          (*channelNvList)[ch].common.tCL = SPDReg;

          // Convert to number of cycles
          cycles = TimeBaseToDCLK(host, socket, ch, dimm, (UINT16)SPDReg, ftbVal);
          if ((*channelNvList)[ch].common.nCL < cycles) {
            (*channelNvList)[ch].common.nCL = (UINT8)cycles;
          }
        }

        //
        // Get MSB of casSup
        //
        ReadSpd (host, socket, ch, dimm, SPD_CAS_LT_SUP_MSB, &SPDReg);
        casSup = (UINT32) (SPDReg << 8);

        //
        // Get LSB of casSup
        //
        ReadSpd (host, socket, ch, dimm, SPD_CAS_LT_SUP_LSB, &SPDReg);
        casSup |= (UINT32) SPDReg;

        //
        // Mask off the unsupported CLs
        //
        (*channelNvList)[ch].common.casSup &= casSup;

        //
        // Get DIMM thermal sensor
        //
        ReadSpd (host, socket, ch, dimm, SPD_DIMM_TS, &(*dimmNvList)[dimm].dimmTs);

        //
        // Get SDRAM type
        //
        ReadSpd (host, socket, ch, dimm, SPD_SDRAM_TYPE, &(*dimmNvList)[dimm].sdramType);

        //
        // Get SDRAM optional features
        //
        ReadSpd (host, socket, ch, dimm, SPD_OPT_FEAT, &SPDReg);
        (*dimmNvList)[dimm].SPDOptionalFeature = SPDReg;

        //
        // Get module mfg id from SPD 117-118.
        //
        ReadSpd (host, socket, ch, dimm, SPD_MMID_MSB, &SPDReg);
        (*dimmNvList)[dimm].SPDMMfgId = (UINT16) (SPDReg << 8);
        ReadSpd (host, socket, ch, dimm, SPD_MMID_LSB, &SPDReg);
        (*dimmNvList)[dimm].SPDMMfgId |= (UINT16) (SPDReg &~BIT7);

        //
        // Get module manufacturing location
        //
        ReadSpd (host, socket, ch, dimm, SPD_MM_LOC, &SPDReg);
        (*dimmNvList)[dimm].SPDMMfLoc = SPDReg; //1 byte Dimm Location

        //
        // Get module module date code from SPD 120-121.
        //
        ReadSpd (host, socket, ch, dimm, SPD_MM_DATE, &SPDReg);
        (*dimmNvList)[dimm].SPDModDate = (UINT16) (SPDReg);
        ReadSpd (host, socket, ch, dimm, SPD_MM_DATE + 1, &SPDReg);
        (*dimmNvList)[dimm].SPDModDate |= (UINT16) (SPDReg << 8);

        //
        // Get module Serial Number from SPD 122-125
        //
        for (i = 0; i < 4; i++) {
          ReadSpd (host, socket, ch, dimm, SPD_MODULE_SN + i, &SPDReg);
          (*dimmNvList)[dimm].SPDModSN[i] = SPDReg;
        }

        //
        // Get module part number from SPD 128-145.
        //
        for (i = 0; i < SPD_MODULE_PART; i++) {
          ReadSpd (host, socket, ch, dimm, SPD_MODULE_PN + i, &SPDReg);
          (*dimmNvList)[dimm].SPDModPart[i] = SPDReg;
        }

        //
        // Get Intel DIMM serial number from SPD 250-252.
        //
        for (i = 0; i < 3; i++) {
          ReadSpd (host, socket, ch, dimm, SPD_INTEL_ID + i, &SPDReg);
          (*dimmNvList)[dimm].SPDIntelSN[i] = SPDReg;
        }

        //
        // Get DRAM mfg id from SPD 148-149.
        //
        ReadSpd (host, socket, ch, dimm, SPD_DRAM_MIDC_MSB, &SPDReg);
        (*dimmNvList)[dimm].SPDDramMfgId = (UINT16) (SPDReg << 8);
        ReadSpd (host, socket, ch, dimm, SPD_DRAM_MIDC_LSB, &SPDReg);
        (*dimmNvList)[dimm].SPDDramMfgId |= (UINT16) (SPDReg &~BIT7);


        ReadSpd (host, socket, ch, dimm, SPD_RFSH_OPT, &SPDReg);
        (*dimmNvList)[dimm].SPDThermRefsh = SPDReg;

        //
        // Set the flag to enable 2X refresh and extended temperature range if the extended operating temperature is
        // supported in 2X refresh, and if the user supports high temperatures and allows 2X refresh
        //
        if (((SPDReg & (ETR + ETRR)) == ETR) &&
            (host->setup.mem.options & TEMPHIGH_EN) &&
            (host->setup.mem.options & ALLOW2XREF_EN)
            ) {
          (*channelNvList)[ch].features |= DOUBLE_REFI + EXTENDED_TEMP;
        }
        //
        // Set the flag to enable extended temperature range if the extended operating temperature is
        // supported in 1X refresh, and if the user supports high temperatures
        //
        else if (((SPDReg & (ETR + ETRR)) == (ETR + ETRR)) && (host->setup.mem.options & TEMPHIGH_EN)) {
          (*channelNvList)[ch].features |= EXTENDED_TEMP;
        }

        //
        // Check if ASR is supported by this DIMM
        // W/a is to set ASR to the memory controller as long as ETR is supported
        //            if (!((*dimmNvList)[dimm].SPDThermRefsh & ASR)) (*channelNvList)[ch].features &= ~ASR_SUPPORT;

        //
        // RDIMM specific bytes
        //
        if (host->nvram.mem.dimmTypePresent == RDIMM) {

#ifdef  LRDIMM_SUPPORT
          //
          // LRDIMM specific bytes
          //
          if ((*channelNvList)[ch].lrDimmPresent) {

            //
            // Get LRDIMM module attributes
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRDIMM_ATTR, &(*dimmNvList)[dimm].dimmAttrib);
            (*dimmNvList)[dimm].SPDAddrMapp = (*dimmNvList)[dimm].dimmAttrib & BIT0;

            //
            // MSB of the register ID
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_VEN_MSB, &SPDReg);
            (*dimmNvList)[dimm].SPDRegVen = (UINT16) (SPDReg << 8);

            //
            // LSB of the register ID
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_VEN_LSB, &SPDReg);
            (*dimmNvList)[dimm].SPDRegVen |= (UINT16) (SPDReg &~BIT7);

            //
            // Register Revision
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_REV, &SPDReg);
            (*dimmNvList)[dimm].SPDRegRev = SPDReg;

            //
            // LR Buffer Function 0, Control Word 2 & 3
            //
            ReadSpd (host, socket, ch, dimm, SPD_LR_F0_RC2_3, &SPDReg);
            (*dimmNvList)[dimm].spdLrBuf_F0_RC2_3 = SPDReg;

            //
            // LR Buffer Function 0, Control Word 4 & 5
            //
            ReadSpd (host, socket, ch, dimm, SPD_LR_F0_RC4_5, &SPDReg);
            (*dimmNvList)[dimm].spdLrBuf_F0_RC4_5 = SPDReg;

            //
            // LR Buffer Function 1, Control Word 8 & 11
            //
            ReadSpd (host, socket, ch, dimm, SPD_LR_F1_RC8_11, &SPDReg);
            (*dimmNvList)[dimm].spdLrBuf_F1_RC8_11 = SPDReg;

            //
            // LR Buffer Function 1, Control Word 12 & 13
            //
            ReadSpd (host, socket, ch, dimm, SPD_LR_F1_RC12_13, &SPDReg);
            (*dimmNvList)[dimm].spdLrBuf_F1_RC12_13 = SPDReg;

            //
            // LR Buffer Function 1, Control Word 14 & 15
            //
            ReadSpd (host, socket, ch, dimm, SPD_LR_F1_RC14_15, &SPDReg);
            (*dimmNvList)[dimm].spdLrBuf_F1_RC14_15 = SPDReg;

            // Index into set of bytes based on actual DDR frequency
            // set of 6 bytes
            if (host->nvram.mem.socket[socket].ddrFreq <= DDR_1067) {
                sb = 0;
            } else if (host->nvram.mem.socket[socket].ddrFreq <= DDR_1600) {
                sb = 6;
            } else {
                sb = 12;
            }

            //
            // LR Buffer Function 3, Control Word 8 & 9
            //
            ReadSpd (host, socket, ch, dimm, SPD_LR_SB0_MDQ_DS_ODT + sb, &SPDReg);
            (*dimmNvList)[dimm].spdLrBuf_MDQ_DS_ODT = SPDReg;

            //
            // LR Buffer Function 3 & 4, Control Word 10 & 11
            //
            ReadSpd (host, socket, ch, dimm, SPD_LR_SB0_DR01_QODT_ACT + sb, &SPDReg);
            (*dimmNvList)[dimm].spdLrBuf_DRAM_QODT_ACT[0] = SPDReg;

            //
            // LR Buffer Function 5 & 6, Control Word 10 & 11
            //
            ReadSpd (host, socket, ch, dimm, SPD_LR_SB0_DR23_QODT_ACT + sb, &SPDReg);
            (*dimmNvList)[dimm].spdLrBuf_DRAM_QODT_ACT[1] = SPDReg;

            //
            // LR Buffer Function 7 & 8, Control Word 10 & 11
            //
            ReadSpd (host, socket, ch, dimm, SPD_LR_SB0_DR45_QODT_ACT + sb, &SPDReg);
            (*dimmNvList)[dimm].spdLrBuf_DRAM_QODT_ACT[2] = SPDReg;

            //
            // LR Buffer Function 9 & 10, Control Word 10 & 11
            //
            ReadSpd (host, socket, ch, dimm, SPD_LR_SB0_DR67_QODT_ACT + sb, &SPDReg);
            (*dimmNvList)[dimm].spdLrBuf_DRAM_QODT_ACT[3] = SPDReg;

            //
            // LR Buffer SMBus offsets 0xC0 - 0xC7
            //
            ReadSpd (host, socket, ch, dimm, SPD_LR_SB0_MR1_2_RTT + sb, &SPDReg);
            (*dimmNvList)[dimm].spdLrBuf_MR1_2_RTT = SPDReg;

            // Index into next set of bytes based on DDR Voltage
            // set of 2 bytes
            vb = host->nvram.mem.socket[socket].ddrVoltage >> 1;
            if (vb > 2) vb = 2;
            vb = vb * 2;

            //
            // LR DIMM minimum DQ Read propagation delay
            //
            ReadSpd (host, socket, ch, dimm, SPD_LR_150_MIN_MOD_DELAY + vb, &SPDReg);

            // mask off reserved bit 7
            SPDReg = SPDReg & 0x7F;
            (*dimmNvList)[dimm].spdLrBuf_150_MIN_MOD_DELAY = SPDReg;

            // Update channel common Module Delay Low
            if ((*channelNvList)[ch].common.nMDL > SPDReg) {
              (*channelNvList)[ch].common.nMDL = SPDReg;
            }

            //
            // LR DIMM maximum DQ Read propagation delay
            //
            ReadSpd (host, socket, ch, dimm, SPD_LR_150_MAX_MOD_DELAY + vb, &SPDReg);

            // mask off reserved bit 7
            SPDReg = SPDReg & 0x7F;
            (*dimmNvList)[dimm].spdLrBuf_150_MAX_MOD_DELAY = SPDReg;

            // Update channel common Module Delay Diff with MTB's for now
            if ((*channelNvList)[ch].common.nMDD < SPDReg) {
              (*channelNvList)[ch].common.nMDD = SPDReg;
            }

            //
            // LR DIMM Personality Bytes
            //
            for (i = 0; i < SPD_LR_PERS_BYTES_TOTAL; i++) {
              ReadSpd (host, socket, ch, dimm, SPD_LR_PERS_BYTE_0 + i, &SPDReg);
              (*dimmNvList)[dimm].lrbufPersBytes[i] = SPDReg;
            }

          } else
#endif
          // SPD bytes for RDIMM module
          {
            //
            // Get RDIMM module attributes
            //
            ReadSpd (host, socket, ch, dimm, SPD_RDIMM_ATTR, &(*dimmNvList)[dimm].dimmAttrib);

            //
            // Get Heat spreader info
            //
            ReadSpd (host, socket, ch, dimm, SPD_DIMM_HS, &(*dimmNvList)[dimm].dimmHs);

            //
            // MSB of the register ID
            //
            ReadSpd (host, socket, ch, dimm, SPD_REG_VEN_MSB, &SPDReg);
            (*dimmNvList)[dimm].SPDRegVen = (UINT16) (SPDReg << 8);

            //
            // LSB of the register ID
            //
            ReadSpd (host, socket, ch, dimm, SPD_REG_VEN_LSB, &SPDReg);
            (*dimmNvList)[dimm].SPDRegVen |= (UINT16) (SPDReg &~BIT7);

            //
            // Register Revision
            //
            ReadSpd (host, socket, ch, dimm, SPD_REG_REV, &SPDReg);
            (*dimmNvList)[dimm].SPDRegRev = SPDReg;

            //
            // Get control words
            //
            for (i = 0; i < 2; i++) {
              ReadSpd (host, socket, ch, dimm, SPD_CNTL_1 + i, &SPDReg);
              (*dimmNvList)[dimm].SPDControlWords[i] = SPDReg;
            }
          }
        } else {
        //
        // UDIMM/SODIMM specific bytes
        //
          //
          // Address Mapping from Edge connector to DRAM
          //
          ReadSpd (host, socket, ch, dimm, SPD_ADDR_MAP_FECTD, &SPDReg);
          (*dimmNvList)[dimm].SPDAddrMapp = SPDReg;
        }
        //
        // Set tCase for each DIMM based on temperature range supported
        //
        if ((*channelNvList)[ch].features & EXTENDED_TEMP) {
          (*dimmNvList)[dimm].dramTcaseMax = DRAM_TCASE_MAX;
        } else {
          (*dimmNvList)[dimm].dramTcaseMax = DRAM_TCASE_DEFAULT;
        }


#ifdef SERIAL_DBG_MSG
        if (checkMsgLevel(host, SDBG_MAX)) {
          getPrintFControl (host);

          //
          // Module part number
          //
          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                        "DIMM Module Part Number: "));
          for (i = 0; i < 17; i++) {
            if ((*dimmNvList)[dimm].SPDModPart[i] >= 32 && (*dimmNvList)[dimm].SPDModPart[i] < 127) {
              rcPrintf ((host, "%c", (*dimmNvList)[dimm].SPDModPart[i]));
            } else {
              rcPrintf ((host, "(%d)", (*dimmNvList)[dimm].SPDModPart[i]));
            }
          }

          // New line
          rcPrintf ((host, "\n"));

          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                        "Intel SN: "));
          //
          // Intel DIMM Serial Number
          //
          serialNum = 0;

          for (i = 0; i < 3; i++) {
            serialNum |= (UINT32)(*dimmNvList)[dimm].SPDIntelSN[i] << ((2 - i) * 8);
          }

          rcPrintf ((host, "%8d\n", serialNum));

          releasePrintFControl (host);
        }
#endif

      } // dimm loop
#ifdef  LRDIMM_SUPPORT
      if ((*channelNvList)[ch].lrDimmPresent) {
        // Now that we have Min/Max LRDIMM Module delay in MTB units,
        // conver them into DCLK's

        if ((*channelNvList)[ch].common.nMDL > lrDimmModuleDelayAdjust[host->nvram.mem.socket[socket].ddrFreq]) {
          (*channelNvList)[ch].common.nMDL = (*channelNvList)[ch].common.nMDL - lrDimmModuleDelayAdjust[host->nvram.mem.socket[socket].ddrFreq];
        } else {
          (*channelNvList)[ch].common.nMDL = 0;
        }

        if ((*channelNvList)[ch].common.nMDD > lrDimmModuleDelayAdjust[host->nvram.mem.socket[socket].ddrFreq]) {
          (*channelNvList)[ch].common.nMDD = (*channelNvList)[ch].common.nMDD - lrDimmModuleDelayAdjust[host->nvram.mem.socket[socket].ddrFreq];
        } else {
          (*channelNvList)[ch].common.nMDD = 0;
        }
        // At this point nMDD is the largest tMDHigh found in MTB units, so convert to additive delay
        // Round up to ensure that ODT stay's on long enough
        (*channelNvList)[ch].common.nMDD = (UINT8)TimeBaseToDCLK(host, socket, ch, 0, (*channelNvList)[ch].common.nMDD-(*channelNvList)[ch].common.nMDL, 0);
        // Round down to ensure that ODT comes on early enough
        (*channelNvList)[ch].common.nMDL = (UINT8) (*channelNvList)[ch].common.nMDL / tCKMTB;

        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "LRDIMM Module Delay per this channel: MDL %d DCLK - MDD %d DCLK\n",
                       (*channelNvList)[ch].common.nMDL,
                       (*channelNvList)[ch].common.nMDD));
      }
#endif
    } // ch loop
  } // if ((host->nvram.mem.DataGood != 1) || (host->var.common.bootMode == NormalBoot)) end
  else {
    //
    // In a fast boot case, restore the SMBCMD registers.
    //
    MemWritePciCfgMC (host, socket, 0, SMBCMD_0_MC_MAIN_REG, host->nvram.mem.socket[socket].smbCmd0);

    if (Is2HA(host)){
      MemWritePciCfgMC (host, socket, 1, SMBCMD_0_MC_MAIN_REG, host->nvram.mem.socket[socket].smbCmd1);
    } else {
      MemWritePciCfgMC (host, socket, 0, SMBCMD_1_MC_MAIN_REG, host->nvram.mem.socket[socket].smbCmd1);
    }
  }

  return SUCCESS;
}


/**

  Detect DDR3 configuration

  @param host   - Point to sysHost
  @param socket - CPU socket ID
  @param ch     - channell to be accessed
  @param dimm   - DIMM to be accessed

  @retval SUCCESS

**/
UINT32
DetectDDR3DIMMConfig (
                     PSYSHOST         host,
                     UINT8            socket,
                     UINT8            ch,
                     UINT8            dimm
                     )
{
  UINT8  SPDReg;
  INT32  mediumTimebase;
  INT32  fineTimebase;
  struct dimmDevice   (*chdimmList)[MAX_DIMM];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];

  chdimmList    = GetChDimmList(host, socket, ch);
  dimmNvList    = GetDimmNvList(host, socket, ch);
  channelNvList = GetChannelNvList(host, socket);

  //
  // Read SPD bytes containing DDR module info
  //
  ReadSpd (host, socket, ch, dimm, SPD_KEY_BYTE2, &SPDReg);
  (*dimmNvList)[dimm].keyByte2 = SPDReg & 0x0F;
  (*dimmNvList)[dimm].actKeyByte2 = SPDReg;

  if (SPDReg == SPD_ECC_SO_UDIMM) {
    // Take UDIMM path to minimize changes
    (*dimmNvList)[dimm].keyByte2 = SPD_UDIMM;
  }
  if (SPDReg == SPD_ECC_SO_RDIMM) {
    // Take RDIMM path to minimize changes
    (*dimmNvList)[dimm].keyByte2 = SPD_RDIMM;
  }
#ifdef  LRDIMM_SUPPORT
  if (SPDReg == SPD_LRDIMM) {
    (*channelNvList)[ch].lrDimmPresent = 1;
    host->nvram.mem.socket[socket].lrDimmPresent = 1;

    // Take RDIMM path to minimize changes ;-)
    (*dimmNvList)[dimm].keyByte2 = SPD_RDIMM;

    // Need to read dimmAttrib to check for 1/2 populated LRDIMM later
    ReadSpd (host, socket, ch, dimm, SPD_LRDIMM_ATTR, &(*dimmNvList)[dimm].dimmAttrib);
  }
#endif

  //
  // Get MTB Divisor
  //
  ReadSpd (host, socket, ch, dimm, SPD_MTB_DIVISOR, &SPDReg);
  (*dimmNvList)[dimm].mtbDiv = SPDReg;

  //
  // Get FTB
  //
  ReadSpd (host, socket, ch, dimm, SPD_FTB, &SPDReg);
  (*dimmNvList)[dimm].SPDftb = SPDReg;

  //
  // Get Min TCK
  //
  ReadSpd (host, socket, ch, dimm, SPD_MIN_TCK, &SPDReg);
  (*dimmNvList)[dimm].minTCK = SPDReg;

  //
  // Get FTB Min TCK
  //
  ReadSpd (host, socket, ch, dimm, SPD_FTB_TCK, &SPDReg);
  (*dimmNvList)[dimm].ftbTCK = SPDReg;

  GetDimmTimeBaseDDR3 (host, 1, (*dimmNvList)[dimm].mtbDiv, ((*dimmNvList)[dimm].SPDftb >> 4) & 0xF,
                       (*dimmNvList)[dimm].SPDftb & 0xF, &mediumTimebase, &fineTimebase);

  if ((*dimmNvList)[dimm].ftbTCK & 0x80) {
    (*dimmNvList)[dimm].minTCK = (mediumTimebase * (INT32) (*dimmNvList)[dimm].minTCK) +
                                 (fineTimebase * ((INT32) (*dimmNvList)[dimm].ftbTCK - 256));
  } else {
    (*dimmNvList)[dimm].minTCK = (mediumTimebase * (INT32) (*dimmNvList)[dimm].minTCK) +
                                 (fineTimebase * (INT32) (*dimmNvList)[dimm].ftbTCK);
  }
  (*chdimmList)[dimm].minTCK = (*dimmNvList)[dimm].minTCK;

  //
  // Get number of banks
  //
  ReadSpd (host, socket, ch, dimm, SPD_SDRAM_BANKS, &SPDReg);
  //
  // SDRAM Capacity
  //
  (*dimmNvList)[dimm].sdramCapacity = SPDReg & 0xF;

  //
  // Get the number of banks
  //
  switch ((SPDReg >> 4) & 3) {
    case 0:
      (*dimmNvList)[dimm].numBanks = 8;
      break;
    case 1:
      (*dimmNvList)[dimm].numBanks = 16;
      break;
  }

  //
  // Bank groups is always 0 for DDR3
  //
  (*dimmNvList)[dimm].numBankGroups = 0;

  (*dimmNvList)[dimm].SPDSDRAMBanks = SPDReg;
  (*dimmNvList)[dimm].actSPDSDRAMBanks = SPDReg;

  //
  // Get Row and Column address
  //
  ReadSpd (host, socket, ch, dimm, SPD_SDRAM_ADDR, &SPDReg);

  //
  // Get number of row bits
  //
  (*dimmNvList)[dimm].numRowBits = ((SPDReg >> 3) & 7) + 12;

  //
  // Get number of column bits
  //
  (*dimmNvList)[dimm].numColBits = (SPDReg & 7) + 9;

  (*dimmNvList)[dimm].actSPDSdramAddr = SPDReg;

  //
  // Get Num of Ranks and Device Width
  //
  ReadSpd (host, socket, ch, dimm, SPD_MODULE_ORG, &SPDReg);
  (*dimmNvList)[dimm].SPDModuleOrg = SPDReg;
  (*dimmNvList)[dimm].actSPDModuleOrg = SPDReg;

  //
  // Get SDRAM Device Type
  //
  ReadSpd (host, socket, ch, dimm, SPD_SDRAM_TYPE, &SPDReg);
  (*dimmNvList)[dimm].SPDDeviceType = SPDReg;

  //
  // Get Raw Card Number
  //
  ReadSpd (host, socket, ch, dimm, SPD_REF_RAW_CARD, &SPDReg);
  (*dimmNvList)[dimm].SPDRawCard = SPDReg;

  //
  // Get Num of Ranks and Device Width
  //
  ReadSpd (host, socket, ch, dimm, SPD_MEM_BUS_WID, &SPDReg);
  (*dimmNvList)[dimm].SPDMemBusWidth = SPDReg;

  //
  // Get Vdd DIMM Support Info
  //
  ReadSpd (host, socket, ch, dimm, SPD_VDD_SUPPORT, &SPDReg);
  if (SPDReg & SPD_VDD_135) {
    host->nvram.mem.socket[socket].lvDimmPresent = 1;
  }
  if (SPDReg & SPD_VDD_125) {
    host->nvram.mem.socket[socket].ulvDimmPresent = 1;
  }
  if ((SPDReg & (SPD_VDD_135 | SPD_VDD_125)) == 0) {
    host->nvram.mem.socket[socket].channelList[ch].v150OnlyDimmPresent = 1;
  }

  //
  // Any dimm not supporting 1.35v will force voltage to 1.5v
  //
  host->nvram.mem.socket[socket].ddrVoltage &= SPDReg;

  return SUCCESS;
}

/**

  Calculate the medium and fine timebases, using integer math.

  @param mtb - Location to store the calculated medium timebase, in femtoseconds.
               Example: dividend = 1, divisor = 8. ((1 * 1,000,000) / 8) = 125,000
  @param ftb - Location to store the calculated fine timebase, in femtoseconds.
               Example: dividend = 1, divisor = 1. ((1 * 1,000) / 1) = 1,000

  @retval N/A

**/
void
GetDimmTimeBaseDDR3 (
  PSYSHOST  host,
  UINT8     mtbDividend,
  UINT8     mtbDivisor,
  UINT8     ftbDividend,
  UINT8     ftbDivisor,
  INT32     * mtb,
  INT32     * ftb
  )
{

  *ftb = ftbDivisor;
  if (*ftb > 0) {
    *ftb = (ftbDividend * FREQUENCY_FTB_OFFSET) / *ftb;
  } else {
    *ftb = 0;
  }

  if (mtbDivisor > 0) {
    *mtb = (mtbDividend * FREQUENCY_MTB_OFFSET) / mtbDivisor;
  } else {
    *mtb = 0;
  }
} // GetDimmTimeBaseDDR3

/**

  Initialize the ddrVoltage based on setup.....DDR3 Only

  @param host   - Point to sysHost
  @param socket - CPU socket ID

  @retval N/A

**/
void
InitDDRVoltageDDR3(
              PSYSHOST  host, 
              UINT8     socket
              )
{
    //
    // Initialize the ddrVoltage based on setup.....DDR3 Only
    //
      if (host->setup.mem.socket[socket].ddrVddLimit == 1) {
        host->nvram.mem.socket[socket].ddrVoltage = SPD_VDD_150;
      } else {
        host->nvram.mem.socket[socket].ddrVoltage = SPD_VDD_135;
      }
}


