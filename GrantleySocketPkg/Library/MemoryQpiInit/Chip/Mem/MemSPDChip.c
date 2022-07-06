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
 * Copyright 2006 - 2019 Intel Corporation All Rights Reserved.
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
extern  UINT8 tXSOFFSET[MAX_SUP_FREQ];
extern  UINT8 primaryWidth[MAX_TECH]; // Primary SDRAM device width
extern  UINT8 rowBits[MAX_TECH];      // Number of row address bits
extern  UINT8 columnBits[MAX_TECH];   // Number of column address bits
extern  UINT8 internalBanks[MAX_TECH];// Number of internal SDRAM banks
extern  UINT8 primaryWidthDDR4[MAX_TECH];   // Primary SDRAM device width
extern  UINT8 rowBitsDDR4[MAX_TECH];        // Number of row address bits
extern  UINT8 columnBitsDDR4[MAX_TECH];     // Number of column address bits
extern  UINT8 bankGroupsDDR4[MAX_TECH];     // Number of internal SDRAM banks
extern  UINT8 tFAWtable[2][MAX_SUP_FREQ];
extern  UINT8 tFAWTableDDR4[3][MAX_SUP_FREQ];
extern  UINT8 tRTPTableDDR4[MAX_SUP_FREQ];
extern  UINT8 tWTRLTableDDR4[MAX_SUP_FREQ];
extern  UINT8 tWTRSTableDDR4[MAX_SUP_FREQ];
extern  UINT8 tWRTableDDR4[MAX_SUP_FREQ];
extern  UINT8 lrDimmModuleDelayAdjust[MAX_SUP_FREQ];

// Prototypes
UINT32 GatherSPDDataDDR4 (PSYSHOST host);
UINT16 CalcDimmCapacity (PSYSHOST host, struct dimmNvram *theDimm);
UINT8  GetTechIndex(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT16 dramType);

/**

  Detect SPD Type DIMM configuration

  @param host    - Pointer to sysHost
  @param socket  - Socket ID
  @param ch      - Channel on socket
  @param dimm    - DIMM on channel
  @param spd     - SPD structure
  @param keyByte - Key byte

  @retval SUCCESS

**/
UINT32
DetectSpdTypeDIMMConfig (
                        PSYSHOST         host,
                        UINT8            socket,
                        UINT8            ch,
                        UINT8            dimm,
                        UINT8            keyByte
                        )
{
  UINT32 Status;
  if (keyByte == SPD_TYPE_DDR4) {
    host->nvram.mem.dramType = SPD_TYPE_DDR4;
    host->nvram.mem.socket[socket].dramType = SPD_TYPE_DDR4;
    InitDDRVoltageDDR4(host, socket);
    Status = DetectDDR4DIMMConfig (host, socket, ch, dimm);
  } else if (keyByte == SPD_TYPE_DDR3) {
    host->nvram.mem.dramType = SPD_TYPE_DDR3;
    host->nvram.mem.socket[socket].dramType = SPD_TYPE_DDR3;
    InitDDRVoltageDDR3(host, socket);
    Status = DetectDDR3DIMMConfig (host, socket, ch, dimm);
  }
  return SUCCESS;
}


UINT32
GatherSPDData (
  PSYSHOST host
  )
  /*++

      Calls to gather device specific SPDData

    @param host  - Point to sysHost

    @retval SUCCESS

    --*/
{
  if (host->nvram.mem.dramType == SPD_TYPE_DDR3){
    GatherSPDDataDDR3 (host);
    return SUCCESS;
  } else {
    GatherSPDDataDDR4 (host);
    return SUCCESS;
  }
}


UINT32
GatherSPDDataDDR4(
  PSYSHOST host
  )
/*++

  Collects data from SPD for DDR4

  @param host  - Point to sysHost

  @retval SUCCESS

--*/
{
  UINT8                           socket;
  UINT8                           ch;
  UINT8                           dimm;
  UINT8                           mcId;
  UINT8                           SPDReg;
  UINT8                           ftbVal;
  UINT8                           i;
  UINT8                           tCKMTB = 0;
  UINT16                          cycles;
  UINT8                           pageSize = 0;
  UINT8                           ti;
  UINT32                          casSup = 0;
  UINT16                          tRC;
  UINT16                          tRAS;
  UINT16                          tRFC;
  UINT8                           tFAW;
  UINT8                           tWTRL;
  UINT8                           tWTR;
  UINT8                           tWR;
  UINT8                           tRTP;
  UINT16                          dateCode;
  struct dimmNvram                (*dimmNvList)[MAX_DIMM];
  struct channelNvram             (*channelNvList)[MAX_CH];
#ifdef SERIAL_DBG_MSG
  struct dimmDevice (*chdimmList)[MAX_DIMM];
#endif  // SERIAL_DBG_MSG
  MC_BIOS_REQ_PCU_FUN1_STRUCT     mcBiosReq;
  SMB_PERIOD_CFG_MC_MAIN_STRUCT   smbPeriod;

  socket = host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return SUCCESS;

  //
  // Get current DCLK ratio
  //
  mcBiosReq.Data = ReadCpuPciCfgEx (host, socket, 0, MC_BIOS_REQ_PCU_FUN1_REG);

  for (mcId = 0; mcId < MAX_IMC; mcId++) {
    if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

    smbPeriod.Data = MemReadPciCfgMC (host, socket, mcId, SMB_PERIOD_CFG_MC_MAIN_REG);

    //
    // Get the period to use based on the current DCLK ratio
    //
    // SMB Period in DCLKs = QCLK_Multiplier *
    // SMB_DCLK = 6 (800MHz QCLK) * 187(133Mhz BCLK) = 1122 * 2.5ns = 2805ns -> 1/2.801us = 0.3565 -> 357KHz
    // SMB_DCLK = 8 (800MHz QCLK) * 140(100Mhz BCLK) = 1120 * 2.5ns = 2800ns -> 1/2.800us = 0.3571 -> 357KHz
    if (host->setup.mem.SpdSmbSpeed == SMB_CLK_400K){
      if (host->var.mem.memBCLK == 100){
        smbPeriod.Bits.smb_clk_prd = mcBiosReq.Bits.req_data * 140; // 357Khz, 100Mhz BCLK
      } else {
        smbPeriod.Bits.smb_clk_prd = mcBiosReq.Bits.req_data * 187; // 357Khz, 133Mhz BCLK
      }
    } else if(host->setup.mem.SpdSmbSpeed == SMB_CLK_1M){
      if (host->var.mem.memBCLK == 100){
        smbPeriod.Bits.smb_clk_prd = mcBiosReq.Bits.req_data * 67; // 750Khz, 100Mhz BCLK
      } else {
        smbPeriod.Bits.smb_clk_prd = mcBiosReq.Bits.req_data * 89; // 750Khz, 133Mhz BCLK
      }
    }
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "SMB Clock Period = %d\n", smbPeriod.Bits.smb_clk_prd));
    MemWritePciCfgMC (host, socket, mcId, SMB_PERIOD_CFG_MC_MAIN_REG, smbPeriod.Data);
  } // mcId loop

  //
  // Don't access the SPD if not necessary
  //
  if ((host->nvram.mem.DataGood != 1) || ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)))
  {
    //
    // Only gather SPD info if we don't already have it
    //
    channelNvList = &host->nvram.mem.socket[socket].channelList;
    //
    // Check strap addresses on each channel moving far to near
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
      (*channelNvList)[ch].common.casSup = 0xFFFFFFFF;

      //
      // Initialize to ASR supported. This bit will get cleared if any DIMM on the channel does not support ASR; 2x Refresh and ETR are suppoted by default for DDR4 Dimms
      //
      (*channelNvList)[ch].features |= ASR_SUPPORT + DOUBLE_REFI + EXTENDED_TEMP;


      for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {

        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        //
        //Need to calculate tCKMTB for DDR4
        //
        tCKMTB = (UINT8) ((((1000000 + host->nvram.mem.socket[socket].ddrFreqMHz - 1) / host->nvram.mem.socket[socket].ddrFreqMHz) +
            (*dimmNvList)[dimm].SPDmtb - 1) / (*dimmNvList)[dimm].SPDmtb);

        //
        // Lookup geometry in DIMM technology table to determine page size
        //
        ti = GetTechIndex(host, socket, ch, dimm, (UINT16)(*dimmNvList)[dimm].keyByte);
        switch(ti){
          case 0:
          case 3:
          case 6:
          case 9:
            pageSize = 0;  // 1/2K Page Size
            break;
          case 1:
          case 4:
          case 7:
          case 10:
            pageSize = 1;  // 1K Page Size
            break;
          case 2:
          case 5:
          case 8:
          case 11:
            pageSize = 2;  // 2K Page Size
            break;
        }

        //
        // Use lookup table for tFAW
        //
        tFAW = tFAWTableDDR4[pageSize][(host->nvram.mem.socket[socket].ddrFreq)];

        if ((*channelNvList)[ch].common.nFAW < tFAW) {
          (*channelNvList)[ch].common.nFAW = tFAW;
        }

        //
        // Use lookup table for tWTR_L
        //
        tWTRL = tWTRLTableDDR4[(host->nvram.mem.socket[socket].ddrFreq)];

        if ((*channelNvList)[ch].common.nWTR_L < tWTRL) {
          (*channelNvList)[ch].common.nWTR_L = tWTRL;
        }

        //
        // Use lookup table for tWTR_S
        //
        tWTR = tWTRSTableDDR4[(host->nvram.mem.socket[socket].ddrFreq)];

        if ((*channelNvList)[ch].common.nWTR < tWTR) {
          (*channelNvList)[ch].common.nWTR = tWTR;
        }

        //
        // Use lookup table for tWR
        //
        tWR = tWRTableDDR4[(host->nvram.mem.socket[socket].ddrFreq)];
        if ((*channelNvList)[ch].common.nWR < tWR) {
          (*channelNvList)[ch].common.nWR = tWR;
        }

        //
        // Use lookup table for tRTP
        //
        tRTP = tRTPTableDDR4[(host->nvram.mem.socket[socket].ddrFreq)];
        if ((*channelNvList)[ch].common.nRTP < tRTP) {
          (*channelNvList)[ch].common.nRTP = tRTP;
        }

        //
        // Get tRRD_S
        //
        ReadSpd (host, socket, ch, dimm, SPD_MIN_TRRDS_DDR4, &SPDReg);
        ReadSpd (host, socket, ch, dimm, SPD_FTB_TRRDS_DDR4, &ftbVal);

        if ((*channelNvList)[ch].common.tRRD < SPDReg) {
          (*channelNvList)[ch].common.tRRD = SPDReg;

          // Convert to number of cycles
          cycles = TimeBaseToDCLK(host, socket, ch, dimm, (UINT16)SPDReg, ftbVal);

          if ((*channelNvList)[ch].common.nRRD < cycles) {
            (*channelNvList)[ch].common.nRRD = (UINT8)cycles;
          }
        }

        //
        // Get tRRD_L
        //
        ReadSpd (host, socket, ch, dimm, SPD_MIN_TRRDL_DDR4, &SPDReg);
        ReadSpd (host, socket, ch, dimm, SPD_FTB_TRRDL_DDR4, &ftbVal);

        if ((*channelNvList)[ch].common.tRRD_L < SPDReg) {
          (*channelNvList)[ch].common.tRRD_L = SPDReg;

          // Convert to number of cycles
          cycles = TimeBaseToDCLK(host, socket, ch, dimm, (UINT16)SPDReg, ftbVal);
          if ((*channelNvList)[ch].common.nRRD_L < cycles) {
            (*channelNvList)[ch].common.nRRD_L = (UINT8)cycles;
          }
        }

        //
        // Get tRP
        //
        ReadSpd (host, socket, ch, dimm, SPD_MIN_TRP_DDR4, &SPDReg);
        ReadSpd (host, socket, ch, dimm, SPD_FTB_TRP_DDR4, &ftbVal);
#ifdef  SERIAL_DBG_MSG
        (*chdimmList)[dimm].tRP = SPDReg;
#endif  // SERIAL_DBG_MSG
        (*channelNvList)[ch].dimmList[dimm].tRP = SPDReg;
        if ((*channelNvList)[ch].common.tRP < SPDReg) {
          (*channelNvList)[ch].common.tRP = SPDReg;

          // Convert to number of cycles
          cycles = TimeBaseToDCLK(host, socket, ch, dimm, (UINT16)SPDReg, ftbVal);

          if ((*channelNvList)[ch].common.nRP < cycles) {
            (*channelNvList)[ch].common.nRP = (UINT8)cycles;
          }
        }

        //
        // Get upper nibble of tRC and tRAS
        //
        ReadSpd (host, socket, ch, dimm, SPD_EXT_TRC_TRAS_DDR4, &SPDReg);
        tRC = (UINT16) (SPDReg & 0xF0) << 4;
        tRAS = (UINT16) (SPDReg & 0x0F) << 8;

        //
        // Get lower byte of tRAS
        //

        ReadSpd (host, socket, ch, dimm, SPD_MIN_TRAS_DDR4, &SPDReg);
        tRAS = tRAS | (UINT16) SPDReg;
        if ((*channelNvList)[ch].common.tRAS < tRAS) {
          (*channelNvList)[ch].common.tRAS = tRAS;

          // Convert to number of cycles
          cycles = TimeBaseToDCLK(host, socket, ch, dimm, tRAS, 0);
          if ((*channelNvList)[ch].common.nRAS < cycles) {
            (*channelNvList)[ch].common.nRAS = (UINT8)cycles;
          }
        }

        //
        // Get lower byte of tRC
        //
        ReadSpd (host, socket, ch, dimm, SPD_MIN_TRC_DDR4, &SPDReg);
        ReadSpd (host, socket, ch, dimm, SPD_FTB_TRC_DDR4, &ftbVal);
        tRC = tRC | (UINT16) SPDReg;
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
        ReadSpd (host, socket, ch, dimm, SPD_MIN_TRCD_DDR4, &SPDReg);
        ReadSpd (host, socket, ch, dimm, SPD_FTB_TRCD_DDR4, &ftbVal);
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
        // Get upper byte of tRFC1
        //
        ReadSpd (host, socket, ch, dimm, SPD_MIN_TRFC1_LSB_DDR4, &SPDReg);
        tRFC = (UINT16) SPDReg;

        //
        // Get lower byte of tRFC1
        //
        ReadSpd (host, socket, ch, dimm, SPD_MIN_TRFC1_MSB_DDR4, &SPDReg);
        tRFC = tRFC | ((UINT16) SPDReg << 8);

        if ((*channelNvList)[ch].common.tRFC < tRFC) {
          (*channelNvList)[ch].common.tRFC = tRFC;

          // Convert to number of cycles
          cycles = TimeBaseToDCLK(host, socket, ch, dimm, tRFC, 0);
          if ((*channelNvList)[ch].common.nRFC < cycles) {
            (*channelNvList)[ch].common.nRFC = cycles;
          }
        }
        //
        // nXS is the amount of time needed after CKE is asserted before
        // a refresh command can be sent to the DIMMs that are exiting
        // self refresh.
        //
        // nXS = ((common.tRFC  * MTB * 1000) + (10ns * 1000000)) /tCK
        //
        (*channelNvList)[ch].common.nXS = (*channelNvList)[ch].common.nRFC + tXSOFFSET[host->nvram.mem.socket[socket].ddrFreq];

        //
        // Get tAA
        //
        ReadSpd (host, socket, ch, dimm, SPD_MIN_TAA_DDR4, &SPDReg);
        ReadSpd (host, socket, ch, dimm, SPD_FTB_TAA_DDR4, &ftbVal);
#ifdef  SERIAL_DBG_MSG
        (*chdimmList)[dimm].tCL = SPDReg;
#endif  // SERIAL_DBG_MSG
        (*channelNvList)[ch].dimmList[dimm].tCL = SPDReg;
        if ((*channelNvList)[ch].common.tCL < SPDReg) {
          (*channelNvList)[ch].common.tCL = SPDReg;

          // Convert to number of cycles
          cycles = TimeBaseToDCLK(host, socket, ch, dimm, (UINT16)SPDReg, (UINT16)ftbVal);

          if ((*channelNvList)[ch].common.nCL < cycles) {
            (*channelNvList)[ch].common.nCL = (UINT8)cycles;
          }
        }

        //
        // Get byte 4 of casSup
        //
        ReadSpd (host, socket, ch, dimm, SPD_CAS_LT_SUP_4_DDR4, &SPDReg);
        casSup = (UINT32)(SPDReg << 24);

        //
        // Get byte 3 of casSup
        //
        ReadSpd (host, socket, ch, dimm, SPD_CAS_LT_SUP_3_DDR4, &SPDReg);
        casSup |= (UINT32)(SPDReg << 16);

        //
        // Get byte 2 of casSup
        //
        ReadSpd (host, socket, ch, dimm, SPD_CAS_LT_SUP_2_DDR4, &SPDReg);
        casSup |= (UINT32)(SPDReg << 8);

        //
        // Get byte 1 of casSup
        //
        ReadSpd (host, socket, ch, dimm, SPD_CAS_LT_SUP_1_DDR4, &SPDReg);
        casSup |= (UINT32)SPDReg;
        //
        // Mask off the unsupported CLs
        //
        (*channelNvList)[ch].common.casSup &= casSup;

        //
        // Get DIMM thermal sensor
        //
        ReadSpd (host, socket, ch, dimm, SPD_DIMM_TS_DDR4, &(*dimmNvList)[dimm].dimmTs);

        //
        // Get SDRAM type
        //
        ReadSpd (host, socket, ch, dimm, SPD_SDRAM_TYPE_DDR4, &(*dimmNvList)[dimm].sdramType);

        //
        // Get SDRAM optional features
        //
        ReadSpd (host, socket, ch, dimm, SPD_OPT_FEAT_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDOptionalFeature = SPDReg;

        ReadSpd (host, socket, ch, dimm, SPD_RFSH_OPT_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDThermRefsh = SPDReg;

        ReadSpd (host, socket, ch, dimm, SPD_OTH_OP_FEAT_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDOtherOptFeatures = SPDReg;

        //
        // Get module mfg id from SPD 320-321.
        //
        ReadSpd (host, socket, ch, dimm, SPD_MMID_MSB_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDMMfgId = (UINT16) (SPDReg << 8);
        ReadSpd (host, socket, ch, dimm, SPD_MMID_LSB_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDMMfgId |= (UINT16) (SPDReg &~BIT7);

        //
        // Get module module mfg location from SPD 322
        //
        ReadSpd (host, socket, ch, dimm,  SPD_MM_LOC_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDMMfLoc = SPDReg; //1 byte Dimm Location

        //
        // Get module module date code from SPD 323-324.
        //
        ReadSpd (host, socket, ch, dimm, SPD_MM_DATE_YR_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDModDate = (UINT16) (SPDReg);
        ReadSpd (host, socket, ch, dimm, SPD_MM_DATE_WK_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDModDate |= (UINT16) (SPDReg << 8);

        //
        // Get module Serial Number from SPD 325-328
        //
        for (i = 0; i < 4; i++) {
          ReadSpd (host, socket, ch, dimm, SPD_MODULE_SN_DDR4 + i, &SPDReg);
          (*dimmNvList)[dimm].SPDModSN[i] = SPDReg;
        }

        //
        // Get module part number from SPD 329-348.
        //
        for (i = 0; i < SPD_MODULE_PART_DDR4; i++) {
          ReadSpd (host, socket, ch, dimm, SPD_MODULE_PN_DDR4 + i, &SPDReg);
          (*dimmNvList)[dimm].SPDModPartDDR4[i] = SPDReg;
        }

        // Module Revision Code from SPD 349.
        ReadSpd (host, socket, ch, dimm, SPD_MODULE_RC_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDModRevCode = SPDReg;

        //
        // Get MSB/LSB of 16-bit CRC from SPD 382-383
        //
        ReadSpd (host, socket, ch, dimm, SPD_CRC_MSB_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDCrcDDR4 = (UINT16) (SPDReg << 8);
        ReadSpd (host, socket, ch, dimm, SPD_CRC_LSB_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDCrcDDR4 |= (UINT16) (SPDReg &~BIT7);


        //
        // Get DRAM mfg id from SPD 350-351.
        //
        ReadSpd (host, socket, ch, dimm, SPD_DRAM_MIDC_MSB_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDDramMfgId = (UINT16) (SPDReg << 8);
        ReadSpd (host, socket, ch, dimm, SPD_DRAM_MIDC_LSB_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDDramMfgId |= (UINT16) (SPDReg &~BIT7);

        //
        // Get DRAM revision id from SPD 352.
        //
        ReadSpd (host, socket, ch, dimm, SPD_DRAM_REV_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDDramRev = SPDReg;

        //
        // Get MSB/LSB of 16-bit CRC from SPD 382-383
        //
        ReadSpd (host, socket, ch, dimm, SPD_CRC_MSB_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDCrcDDR4 = (UINT16) (SPDReg << 8);
        ReadSpd (host, socket, ch, dimm, SPD_CRC_LSB_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDCrcDDR4 |= (UINT16) (SPDReg &~BIT7);

        //
        // Get SPD Spec revision
        //
        ReadSpd (host, socket, ch, dimm, SPD_REVISION, &SPDReg);
        (*dimmNvList)[dimm].SPDSpecRev = SPDReg;

        //
        // RDIMM specific bytes
        //
        if (host->nvram.mem.dimmTypePresent == RDIMM) {

          //
          // Register Output Drive Strength for Control
          //
          ReadSpd (host, socket, ch, dimm, SPD_REG_OD_CTL_DDR4, &SPDReg);
          (*dimmNvList)[dimm].SPDODCtl = SPDReg;

          //
          // Register Output Drive Strength for Clock
          //
          ReadSpd (host, socket, ch, dimm, SPD_REG_OD_CK_DDR4, &SPDReg);
          (*dimmNvList)[dimm].SPDODCk = SPDReg;

#ifdef  LRDIMM_SUPPORT
          //
          // LRDIMM specific bytes
          //
          if ((*channelNvList)[ch].lrDimmPresent) {

            //
            // Get DIMM attributes
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRDIMM_ATTR_DDR4, &(*dimmNvList)[dimm].dimmAttrib);
            (*dimmNvList)[dimm].lrbufGen = ((*dimmNvList)[dimm].dimmAttrib & 0xF0) >>4;

            //
            // Get LRDIMM Address Mapping from Register to DRAM
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_ADD_MAP_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDAddrMapp = SPDReg & BIT0;

            //
            // MSB of the register ID
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_VEN_MSB_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDRegVen = (UINT16) (SPDReg << 8);

            //
            // LSB of the register ID
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_VEN_LSB_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDRegVen |= (UINT16) (SPDReg &~BIT7);

            //
            // Register Revision
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_REV_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDRegRev = SPDReg;

            //
            // Data Buffer Revision
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_DB_REV_DDR4, &SPDReg);
            (*dimmNvList)[dimm].lrbufRid = SPDReg;

            //
            // DRAM VrefDQ for Package Rank 0
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_DRAM_VREFDQ_R0_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDLrbufDramVrefdqR0 = SPDReg;
            //
            // DRAM VrefDQ for Package Rank 1
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_DRAM_VREFDQ_R1_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDLrbufDramVrefdqR1 = SPDReg;
            //
            // DRAM VrefDQ for Package Rank 2
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_DRAM_VREFDQ_R2_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDLrbufDramVrefdqR2 = SPDReg;
            //
            // DRAM VrefDQ for Package Rank 3
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_DRAM_VREFDQ_R3_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDLrbufDramVrefdqR3 = SPDReg;
            //
            // LR Data Buffer VrefDQ for DRAM Interface
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_DB_VREFDQ_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDLrbufDbVrefdq = SPDReg;
            //
            // LR Data Buffer MDQ Drive Strength and RTT for data rate <= 1866
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_DB_DS_RTT_LE1866_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDLrbufDbDsRttLe1866 = SPDReg;
            //
            // LR Data Buffer MDQ Drive Strength and RTT for data rate > 1866 and <= 2400
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_DB_DS_RTT_GT1866_LE2400_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDLrbufDbDsRttGt1866Le2400 = SPDReg;
            //
            // LR Data Buffer MDQ Drive Strength and RTT for data rate > 2400 and <= 3200
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_DB_DS_RTT_GT2400_LE3200_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDLrbufDbDsRttGt2400Le3200 = SPDReg;
            //
            // LR Buffer DRAM Drive Strength (for data rates <1866, 1866 < data rate < 2400, and 2400 < data rate < 3200)
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_DRAM_DS_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDLrbufDramDs = SPDReg;
            //
            // LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate <= 1866
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_DRAM_ODT_WR_NOM_LE1866_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDLrbufDramOdtWrNomLe1866 = SPDReg;
            //
            // LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate > 1866 and <= 2400
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_DRAM_ODT_WR_NOM_GT1866_LE2400_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDLrbufDramOdtWrNomGt1866Le2400 = SPDReg;
            //
            // LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate > 2400 and <= 3200
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_DRAM_ODT_WR_NOM_GT2400_LE3200_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDLrbufDramOdtWrNomGt2400Le3200 = SPDReg;
            //
            // LR Buffer DRAM ODT (RTT_PARK) for data rate <= 1866
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_DRAM_ODT_PARK_LE1866_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDLrbufDramOdtParkLe1866 = SPDReg;
            //
            // LR Buffer DRAM ODT (RTT_PARK) for data rate > 1866 and <= 2400
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_DRAM_ODT_PARK_GT1866_LE2400_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDLrbufDramOdtParkGt1866Le2400 = SPDReg;
            //
            // LR Buffer DRAM ODT (RTT_PARK) for data rate > 2400 and <= 3200
            //
            ReadSpd (host, socket, ch, dimm, SPD_LRBUF_DRAM_ODT_PARK_GT2400_LE3200_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDLrbufDramOdtParkGt2400Le3200 = SPDReg;

          } else
#endif
          // SPD bytes for RDIMM module
          {
            //
            // Get RDIMM module attributes
            //
            ReadSpd (host, socket, ch, dimm, SPD_RDIMM_ATTR_DDR4, &(*dimmNvList)[dimm].dimmAttrib);

            //
            // Get Heat spreader info
            //
            ReadSpd (host, socket, ch, dimm, SPD_DIMM_HS_DDR4, &(*dimmNvList)[dimm].dimmHs);

            //
            // MSB of the register ID
            //
            ReadSpd (host, socket, ch, dimm, SPD_REG_VEN_MSB_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDRegVen = (UINT16) (SPDReg << 8);

            //
            // LSB of the register ID
            //
            ReadSpd (host, socket, ch, dimm, SPD_REG_VEN_LSB_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDRegVen |= (UINT16) (SPDReg &~BIT7);

            //
            // Register Revision
            //
            ReadSpd (host, socket, ch, dimm, SPD_REG_REV_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDRegRev = SPDReg;

            //
            // Check if this register is supported
            //
            if ((((*dimmNvList)[dimm].SPDRegVen == MFGID_IDT) && ((*dimmNvList)[dimm].SPDRegRev <= IDT_SPD_REGREV_B0)) ||
                (((*dimmNvList)[dimm].SPDRegVen == MFGID_INPHI) && ((*dimmNvList)[dimm].SPDRegRev <= 0)) ||
                (((*dimmNvList)[dimm].SPDRegVen == MFGID_MONTAGE) && ((*dimmNvList)[dimm].SPDRegRev <= 0)) ||
                (((*dimmNvList)[dimm].SPDRegVen == MFGID_TI) && ((*dimmNvList)[dimm].SPDRegRev <= 5))) {

              MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                            "ERROR: You are using a non-production memory module that is not supported\n"));
              DisableChannel(host, socket, ch);
              OutputWarning (host, WARN_DIMM_COMPAT, WARN_DIMM_COMPAT_MINOR_NOT_SUPPORTED, socket, ch, dimm, 0);
             } // Supported register check

            //
            // Address Mapping form Register to DRAM
            //
            ReadSpd (host, socket, ch, dimm, SPD_ADD_MAPPING_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDAddrMapp = SPDReg;
          }

        } else {
          //
          // UDIMM/SODIMM specific bytes
          //
          //
          // Address Mapping from Edge connector to DRAM
          //
          ReadSpd (host, socket, ch, dimm, SPD_ADDR_MAP_FECTD_DDR4, &SPDReg);
          (*dimmNvList)[dimm].SPDAddrMapp = SPDReg & BIT0;
        }


         //
        // Set tCase for each DIMM based on temperature range supported
        //
        if ((*channelNvList)[ch].features & EXTENDED_TEMP) {
          (*dimmNvList)[dimm].dramTcaseMax = DRAM_TCASE_MAX;
        } else {
          (*dimmNvList)[dimm].dramTcaseMax = DRAM_TCASE_DEFAULT;
        }


        //
        // Samsung SR RDIMMs older than 2013 WW01 are not supported
        //
        dateCode = (*dimmNvList)[dimm].SPDModDate >> 8;
        dateCode |= (*dimmNvList)[dimm].SPDModDate << 8;

        if (((*dimmNvList)[dimm].SPDDramMfgId == MFGID_SAMSUNG) && (host->nvram.mem.dimmTypePresent == RDIMM) &&
            ((*dimmNvList)[dimm].numRanks == 1) && (dateCode < 0x1301)) {

          for (i = 0; i < 10; i++) {
            MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                            "ERROR: Samsung SR RDIMM detected older than 2013 WW01. This DIMM is not supported!!!\n"));
          } // i loop
          DisableChannel(host, socket, ch);
          OutputWarning (host, WARN_DIMM_COMPAT, WARN_DIMM_COMPAT_MINOR_NOT_SUPPORTED, socket, ch, dimm, 0);
        }

      } // dimm loop
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
} // GatherSPDDataDDR4


#ifdef SERIAL_DBG_MSG
char *GetDimmTypeStr(PSYSHOST host, UINT8 DimmType, char *strBuf) {

  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
    switch (DimmType) {
    case SPD_UDIMM:
      StrCpyLocal(strBuf, "UDIMM");
      break;
    case SPD_RDIMM:
      StrCpyLocal(strBuf, "RDIMM");
      break;
    case SPD_LRDIMM:
      StrCpyLocal(strBuf, "LRDIMM");
      break;
    case SPD_SODIMM:
      StrCpyLocal(strBuf, "SODIMM");
      break;
    case SPD_MICRO_DIMM:
      StrCpyLocal(strBuf, "Micro-DIMM");
      break;
    case SPD_MINI_RDIMM:
      StrCpyLocal(strBuf, "Mini-RDIMM");
      break;
    case SPD_MINI_UDIMM:
      StrCpyLocal(strBuf, "Mini-UDIMM");
      break;
    case SPD_ECC_SO_UDIMM:
      StrCpyLocal(strBuf, "72b-SO-UDIMM");
      break;
    case SPD_ECC_SO_RDIMM:
      StrCpyLocal(strBuf, "72b-SO-RDIMM");
      break;
    case SPD_ECC_SO_CDIMM:
      StrCpyLocal(strBuf, "72b-SO-CDIMM");
      break;
    default:
      StrCpyLocal(strBuf, "Unknown DIMM");
      break;
    }
  } else {
    //DDR4
    switch (DimmType) {
    case SPD_UDIMM:
      StrCpyLocal(strBuf, "UDIMM");
      break;
    case SPD_RDIMM:
      StrCpyLocal(strBuf, "RDIMM");
      break;
    case SPD_LRDIMM_DDR4:
      StrCpyLocal(strBuf, "LRDIMM");
      break;
    case SPD_SODIMM:
      StrCpyLocal(strBuf, "SODIMM");
      break;
    default:
      StrCpyLocal(strBuf, "Unknown DIMM");
      break;
    }
  }
  return strBuf;
}
#endif  // SERIAL_DBG_MSG

UINT32
CheckPORCompat (
  PSYSHOST host
  )
/*++


  Verifies current population does not validate POR restrictions

  @param host  - Pointer to sysHost

  @retval N/A

--*/
{
  UINT8                       socket;
  UINT8                       ch;
  UINT8                       dimm;
  INT32                       minTCKcommon;
  UINT8                       MemPresent;
  UINT8                       chMemPresent;
  UINT8                       mapOutCh;
  UINT8                       DimmTypeStatus;
  UINT8                       DimmType;
  UINT8                       emptySlot;
  UINT8                       dimmTypeSupOrg;
  UINT8                       maxDimm;
  UINT8                       ti;
  UINT8                       i;
  BIOSSCRATCHPAD1_UBOX_MISC_STRUCT biosScratchPad1;
  struct dimmDevice           (*chdimmList)[MAX_DIMM];
  struct dimmNvram            (*dimmNvList)[MAX_DIMM];
  struct channelNvram         (*channelNvList)[MAX_CH];
  DIMMMTR_0_MCDDC_DESC_BDX_STRUCT dimmMtr;
  CAPID3_PCU_FUN3_BDX_STRUCT  capId3;
  struct smbDevice  spd;
#ifdef SERIAL_DBG_MSG
  char strBuf0[128];
  char strBuf1[128];
#endif  // SERIAL_DBG_MSG
#ifdef  LRDIMM_SUPPORT
  UINT8                       numRanks = 0;
  UINT8                       numRanks0;
  UINT8                       numRanks1;
  UINT8                       numRowBits;
  UINT8                       numDramRanks;
  UINT8                       dramDensity;
#endif  //  LRDIMM_SUPPORT
  UINT8                       ECCDimmSupport;

  MemPresent      = 0;
  DimmTypeStatus  = 0;
  dimmMtr.Data    = 0;
  DimmType        = 0;

  //
  // Initialize common parts of the smbDevice structure for all SPD devices
  //
  spd.compId = SPD;
  spd.address.controller = PROCESSOR;
  spd.address.deviceType = DTI_EEPROM;

  //
  // Disable the fast cold boot path if any socket has disabled it
  //
  if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBootFast)) {
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      if (host->nvram.mem.socket[socket].enabled == 0) continue;
      // Check if any socket sub boot mode is set to ColdBoot
      if (host->var.mem.socket[socket].socketSubBootMode == ColdBoot) {
        host->var.common.resetRequired |= POST_RESET_WARM;
        // If any socket has changed to ColdBoot, set scratchpad bit on all sockets to disable ColdBootFast
        for (i = 0; i < MAX_SOCKET; i++){
          if (host->nvram.mem.socket[i].enabled == 0) continue;
          biosScratchPad1.Data = ReadCpuPciCfgEx (host, i, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG);
          biosScratchPad1.Data |=  BIT6;
          WriteCpuPciCfgEx (host, i, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG, biosScratchPad1.Data);
        }

        break;
      }
    } // socket loop
  }

  if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)) {

    host->nvram.common.rcVersion = RC_REVISION;
    //
    // Initialize ECC support to enabled
    //
    host->nvram.mem.eccEn = 1;

    //
    // Sync the socket based dramType's to the global dramType
    //
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      if(host->nvram.mem.socket[socket].dramType) {
        host->nvram.mem.dramType = host->nvram.mem.socket[socket].dramType;
        break;
      }
    } // socket loop

    //
    // Detect DIMMs on each Socket
    //
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      //
      // Loop for each CPU socket
      //
      if (host->nvram.mem.socket[socket].enabled == 0) continue;

      capId3.Data = ReadCpuPciCfgEx(host, socket, 0, CAPID3_PCU_FUN3_REG);

      //
      // Disable ECC if it is not supported by HW
      //
      if (capId3.Bits.disable_ecc) {
        host->nvram.mem.eccEn = 0;
      }
      //
      // Disable ECC if option requires it
      //
      if (!(host->setup.mem.options & ECC_CHECK_EN)) {
        host->nvram.mem.eccEn = 0;
      }
      //
      // Disable DDR3L (1.35v) if it is not supported by HW, Only do for DDR3
      //
      if (host->nvram.mem.dramType == SPD_TYPE_DDR3){
        if (capId3.Bits.mc2gdfuse & BIT0) {
          host->nvram.mem.socket[socket].ddrVoltage = SPD_VDD_150;
        }
      }

      //
      // Check DDR3/DDR4 mode
      //
      if (!(CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX))) {
        if (((host->nvram.mem.dramType == SPD_TYPE_DDR3) && (!capId3.Bits.disable_mem_ddr4)) ||
          ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && (capId3.Bits.disable_mem_ddr4))){
          MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
            "Memory type mismatch!\n"));
          OutputError (host,ERR_DIMM_COMPAT, ERR_MISMATCH_DIMM_TYPE, socket, NO_CH, NO_DIMM, NO_RANK);
        }
      } // CheckSteppingGreaterThan

      //
      // Initialize to 0
      //
      host->nvram.mem.socket[socket].maxDimmPop = 0;

      //
      // Save the original value
      //
      dimmTypeSupOrg = host->setup.mem.dimmTypeSupport;

      //
      // Initialize to 0
      //
      minTCKcommon  = 0;

      channelNvList = &host->nvram.mem.socket[socket].channelList;

      //
      // Detect DIMMs on each channel
      //
      for (ch = 0; ch < MAX_CH; ch++) {
        //
        // Initialize to 0 for each channel
        //
        chMemPresent = 0;

        if ((*channelNvList)[ch].enabled == 0) continue;

        (*channelNvList)[ch].lrRankMultEnabled = 0;
        (*channelNvList)[ch].encodedCSMode = 0;
        (*channelNvList)[ch].cidBitMap = 0;

        //
        // Initialize the maximum number of DIMMs on this channel to 0
        //
        maxDimm = (*channelNvList)[ch].maxDimm;

        emptySlot = 0;

        //
        // Initialize flag used to map out the current channel to 0
        //
        mapOutCh    = 0;

        chdimmList  = &host->var.mem.socket[socket].channelList[ch].dimmList;

        //
        // Detect DIMMs in each slot
        //
        dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
        for (dimm = 0; dimm < host->var.mem.socket[socket].channelList[ch].numDimmSlots; dimm++) {
          spd.address.busSegment = (ch & BIT1) >> 1;
          spd.address.strapAddress = dimm + ((ch & BIT0) * 4);
          if (IsDESku(host)) {
            spd.address.strapAddress = dimm + ((ch & BIT0) * 2);
          }
          spd.mcId = GetMCID(host, socket, ch);

          if ((*dimmNvList)[dimm].dimmPresent == 0) {
            emptySlot = 1;
            continue;
          }

          //
          // Log a warning and continue if a DIMM is populated out of order
          //
          if (emptySlot) {
            MemDebugPrint((host, SDBG_MINMAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "DIMM out of order\nSMBUS Segment - %d, DIMM Address - 0x%x\n",
                          spd.address.busSegment, spd.address.strapAddress));
            OutputWarning (host, WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_MINOR_OUT_OF_ORDER, socket, ch, dimm, 0xFF);

            mapOutCh = 1;
            continue;
          }

          dimmMtr.Data    = 0;

#ifdef  LRDIMM_SUPPORT

          // Determine LRDIMM CS/ADDR mapping here
          if (((*channelNvList)[ch].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {

            // Save the number of physical ranks for later use
            switch (((*dimmNvList)[dimm].actSPDModuleOrg >> 3) & 7) {
            case 0:
               numDramRanks = 1;
               break;
            case 1:
               numDramRanks = 2;
               break;
            case 3:
               numDramRanks = 4;
               break;
            case 4:
               numDramRanks = 8;
               break;
            default:
               numDramRanks = 1;
               break;
            }
            (*dimmNvList)[dimm].numDramRanks = numDramRanks;

            // Translate number of ranks and row bits
            numRanks = ((*dimmNvList)[dimm].actSPDModuleOrg >> 3) & 7;
            numRowBits = (*dimmNvList)[dimm].numRowBits;
            dramDensity = (*dimmNvList)[dimm].sdramCapacity;

            // Support Rank Multiplication factor
            switch (host->setup.mem.forceRankMult) {

            case RANK_MULT_AUTO:

              // Auto-configure RM factor based on #DPC
              (*dimmNvList)[dimm].lrRankMult = 1;

              switch ((*channelNvList)[ch].maxDimm) {

                case 3:

                  // 3dpc x2pr => use native x2 CS (3 DR config)

                  // 3dpc x4pr => use x2 RM (3 DR config)
                  if (numRanks == SPD_NUM_RANKS_4) {
                    numRanks = SPD_NUM_RANKS_2;
                    numRowBits = numRowBits + 1;
                    (*dimmNvList)[dimm].lrRankMult = 2;
                    dramDensity = dramDensity + 1;
                  }
                  // 3dpc x8pr => use x4 RM (3 DR config)
                  else if (numRanks == SPD_NUM_RANKS_8) {
                    numRanks = SPD_NUM_RANKS_2;
                    numRowBits = numRowBits + 2;
                    (*dimmNvList)[dimm].lrRankMult = 4;
                    dramDensity = dramDensity + 2;
                  }
                  break;

                case 2:

                  // 2dpc x2pr => use native x2 CS (2 DR config)
                  // 2dpc x4pr => use native x4 CS (2 QR config)

                  // 2dpc x8pr => use x4 RM (2 DR config)
                  if (numRanks == SPD_NUM_RANKS_8) {
                    numRanks = SPD_NUM_RANKS_2;
                    numRowBits = numRowBits + 2;
                    (*dimmNvList)[dimm].lrRankMult = 4;
                    dramDensity = dramDensity + 2;
                  }
                  // mixed 2DPC cases => x4pr uses x2 RM
                  // case 1: slot 0 - 8R, slot 1 - 4R
                  // case 2: slot 0 - 4R, slot 1 - 8R
                  numRanks0 = ((*dimmNvList)[0].actSPDModuleOrg >> 3) & 7;
                  numRanks1 = ((*dimmNvList)[1].actSPDModuleOrg >> 3) & 7;
                  if (numRanks == SPD_NUM_RANKS_4) {

                    if (((dimm == 0) && (numRanks1 == SPD_NUM_RANKS_8)) ||
                        ((dimm == 1) && (numRanks0 == SPD_NUM_RANKS_8))) {

                      numRanks = SPD_NUM_RANKS_2;
                      numRowBits = numRowBits + 1;
                      (*dimmNvList)[dimm].lrRankMult = 2;
                      dramDensity = dramDensity + 1;
                    }
                  }

                  break;
                case 1:

                  // 1dpc x2pr => use native x2 CS (1 DR config)
                  // 1dpc x4pr => use native x4 CS (1 QR config)

                  // 1dpc x8pr => use x4 RM (1 DR config)
                  if (numRanks == SPD_NUM_RANKS_8) {
                    numRanks = SPD_NUM_RANKS_2;
                    numRowBits = numRowBits + 2;
                    (*dimmNvList)[dimm].lrRankMult = 4;
                    dramDensity = dramDensity + 2;
                  }
                  break;

                default:
                mapOutCh = 1;  // If we get here something went wrong
              }
              break;

            case RANK_MULT_EN:

              if (numRanks == SPD_NUM_RANKS_4) {
                // Force 2x RM factor
                numRowBits = numRowBits + 1;
                (*dimmNvList)[dimm].lrRankMult = 2;
                dramDensity = dramDensity + 1;
                numRanks = SPD_NUM_RANKS_2;
              }
              else if (numRanks == SPD_NUM_RANKS_8) {
                // Force 4x RM factor
                numRowBits = numRowBits + 2;
                (*dimmNvList)[dimm].lrRankMult = 4;
                dramDensity = dramDensity + 2;
                numRanks = SPD_NUM_RANKS_2;
              }
              break;

            default:
              mapOutCh = 1;  // If we get here something went wrong
            }

            // Update logical ranks and DRAM row bits / density
            (*dimmNvList)[dimm].SPDModuleOrg = ((*dimmNvList)[dimm].actSPDModuleOrg & ~(BIT5 + BIT4 + BIT3)) | (numRanks << 3);
            (*dimmNvList)[dimm].numRowBits = numRowBits;
            (*dimmNvList)[dimm].sdramCapacity = dramDensity;

            // Update host memory timings for Rank Multiplication mode
            if ((*dimmNvList)[dimm].lrRankMult > 1) {
               // Indicate that tCCD needs padding for physical rank r2r/w2w turnaround
               (*channelNvList)[ch].lrRankMultEnabled = 1;
            }
            // Indicate when extended addressing mode is enabled
            if ((*dimmNvList)[dimm].numRowBits > 16) {
               (*channelNvList)[ch].lrdimmExtAddrMode = 1;
            }
          }

          //
          // Save die count per package
          //
          (*dimmNvList)[dimm].dieCount = (((*dimmNvList)[dimm].SPDDeviceType & SPD_DIE_COUNT) >> 4) + 1;

          //
          // 3DS support
          //
          if (((*dimmNvList)[dimm].SPDDeviceType & SPD_SIGNAL_LOADING) == SPD_3DS_TYPE) {
            (*channelNvList)[ch].encodedCSMode = 2;
          }

          // Determine DDR4 LRDIMM CS/ADDR mapping here
          if (((*channelNvList)[ch].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR4)) {

            // Save the number of physical ranks for later use
            numRanks = (((*dimmNvList)[dimm].actSPDModuleOrg >> 3) & 3);
            //
            // translate 0-based SPD number of ranks to 1-based
            numDramRanks = numRanks + 1;
            (*dimmNvList)[dimm].numDramRanks = numDramRanks;

            // Reuse byte for Rank Multiplication factor as rank aliasing in DDR4
            (*dimmNvList)[dimm].lrRankMult = 1;
            if ((((*channelNvList)[ch].maxDimm > 2) &&  (numDramRanks > 2)) || (host->setup.mem.forceRankMult == RANK_MULT_EN)){
              //
              // for 3DPC, if any dimm has > 2 ranks, must use encoded chip select mode
              //
              (*channelNvList)[ch].encodedCSMode |= 1;
            }
            //
            // if any dimm on this channel is in encoded mode, then all dimms must be in encoded mode
            if ((*channelNvList)[ch].encodedCSMode == 1) {
              (*dimmNvList)[dimm].lrRankMult = 2;
              numRanks = SPD_NUM_RANKS_2;
            }

            // Update logical ranks and DRAM row bits / density
            (*dimmNvList)[dimm].SPDModuleOrg = ((*dimmNvList)[dimm].actSPDModuleOrg & ~(BIT4 + BIT3)) | (numRanks << 3);
          }

#endif  //LRDIMM_SUPPORT

          //
          //Comparison of DIMM's present on each slot to see if the DimmRank[slot] > DimmRank[slot-1]
          //Halt System if Non POR Config detected (Ranks not placed in descending order in DIMM slots for EP
          //or mixed rank configuration found for DE)

          if(dimm != 0) {
            if(((*dimmNvList)[dimm].SPDModuleOrg >> 3) > ((*dimmNvList)[dimm - 1].SPDModuleOrg >> 3)) {
               MemDebugPrint((host, SDBG_MINMAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                            "DIMM config Not POR Config Slot%d SPD Num Rank= %d Slot%d SPD Num Rank= %d",
                            dimm - 1, ((*dimmNvList)[dimm - 1].SPDModuleOrg >> 3),
                            dimm, ((*dimmNvList)[dimm].SPDModuleOrg >> 3)));
              OutputWarning (host, WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_MINOR_OUT_OF_ORDER, socket, ch, dimm, 0xFF);
            }

            if ((IsDESku(host)) && (((*dimmNvList)[dimm].SPDModuleOrg >> 3) != ((*dimmNvList)[dimm - 1].SPDModuleOrg >> 3))) {
              MemDebugPrint((host, SDBG_MINMAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                           "DIMM config Not POR Config Slot%d SPD Num Rank= %d Slot%d SPD Num Rank= %d",
                           dimm - 1, ((*dimmNvList)[dimm - 1].SPDModuleOrg >> 3),
                           dimm, ((*dimmNvList)[dimm].SPDModuleOrg >> 3)));
              OutputWarning (host, WARN_DIMM_POP_RUL, WARM_DIMM_POP_RUL_MINOR_MIXED_RANKS_FOUND, socket, ch, dimm, 0xFF);
            }
          }

          //
          // Lookup geometry in DIMM technology table
          //
          ti = GetTechIndex(host, socket, ch, dimm, (UINT16)(*dimmNvList)[dimm].keyByte);

          //
          // Check if a valid tech index was found
          //
          if (ti == 0xFF) {
            MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                          "DIMM not supported!\n"));
            OutputWarning (host, WARN_DIMM_COMPAT, 0, socket, ch, dimm, 0xFF);
            mapOutCh = 1;
            continue;
          }

          //
          // ti = pointer into techTable.  If ti = MAX_TECH then there was no match with supported geometries
          // Check for compatible DDR3 module parameters
          // SPD Key byte is DDR3 SDRAM Registered, Unbuffered or Small Outline
          // min cycle time for DDR3-800  (2.5 ns max)
          // Check if this SDRAM density is supported by the HW
          //
          if (((*dimmNvList)[dimm].sdramCapacity > SPD_2Gb) && capId3.Bits.disable_4gbit_ddr3) {
            MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                          "4Gb devices fused off!\n"));
            OutputWarning (host, WARN_4Gb_FUSE, 0, socket, ch, dimm, 0xFF);
            mapOutCh = 1;
            continue;
          }
          //
          // Check if this SDRAM density is supported by the HW
          //
          if ((host->nvram.mem.dramType == SPD_TYPE_DDR3) && (((*dimmNvList)[dimm].sdramCapacity == SPD_8Gb) && capId3.Bits.disable_8gbit_ddr3)) {
            MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                          "8Gb devices fused off!\n"));
            OutputWarning (host, WARN_8Gb_FUSE, 0, socket, ch, dimm, 0xFF);
            mapOutCh = 1;
            continue;
          }
          //
          // 1, 2 or 4 Ranks
          //
          if (!((((*dimmNvList)[dimm].SPDModuleOrg >> 3) == SPD_NUM_RANKS_1) ||
              (((*dimmNvList)[dimm].SPDModuleOrg >> 3) == SPD_NUM_RANKS_2) ||
              (((*dimmNvList)[dimm].SPDModuleOrg >> 3) == SPD_NUM_RANKS_4))) {
            MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                          "Number of ranks on device not supported!\n"));
            OutputWarning (host, WARN_DIMM_COMPAT, WARN_DIMM_COMPAT_MINOR_MAX_RANKS, socket, ch, dimm, 0xFF);
            mapOutCh = 1;
            continue;
          }

          //
          // Check if an RDIMM is plugged in to a UDIMM only board or connected to a cpu that does not support it
          //
          if (((*dimmNvList)[dimm].keyByte2 == SPD_RDIMM) &&
              ((host->setup.mem.dimmTypeSupport == UDIMM) || capId3.Bits.disable_rdimm)
              ) {
            MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                          "%s in UDIMM only board!\n",
                          GetDimmTypeStr(host, (*dimmNvList)[dimm].actKeyByte2, (char *)&strBuf0)));
            OutputWarning (host, WARN_RDIMM_ON_UDIMM, 0, socket, ch, dimm, 0xFF);
            mapOutCh = 1;
            continue;
          }

          //
          // Check if an UDIMM is plugged in to a RDIMM only board or connected to a cpu that does not support it
          //
          if (((*dimmNvList)[dimm].keyByte2 == SPD_UDIMM) &&
              ((host->setup.mem.dimmTypeSupport == RDIMM) || capId3.Bits.disable_udimm)
              ) {
            MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                          "UDIMM in RDIMM only board!\n"));
            OutputWarning (host, WARN_UDIMM_ON_RDIMM, 0, socket, ch, dimm, 0xFF);
            mapOutCh = 1;
            continue;
          }

          //
          // Check if an SODIMM is plugged in to a RDIMM only board or connected to a cpu that does not support it
          //
          if (((*dimmNvList)[dimm].keyByte2 == SPD_SODIMM) &&
              ((host->setup.mem.dimmTypeSupport == RDIMM) || capId3.Bits.disable_udimm)
              ) {
            MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                          "SODIMM in RDIMM only board!\n"));
            OutputWarning (host, WARN_SODIMM_ON_RDIMM, 0, socket, ch, dimm, 0xFF);
            mapOutCh = 1;
            continue;
          }

          if (host->nvram.mem.dramType == SPD_TYPE_DDR3){
            if ((*chdimmList)[dimm].minTCK > 2500000) {
              //
              // Log warning indicating we only support DDR3 800 and higher
              //
              MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                          "DIMM does not support DDR3-800 or higher!\n"));
              OutputWarning (host, WARN_DIMM_COMPAT, WARN_TOO_SLOW, socket, ch, dimm, 0xFF);
              mapOutCh = 1;
              continue;
            }
          }else{
            if ((*chdimmList)[dimm].minTCK > 1500000) {
              //
              // Log warning indicating we only support DDR4 1333 and higher
              //
              MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                          "DIMM does not support DDR4-1333 or higher!\n"));
              OutputWarning (host, WARN_DIMM_COMPAT, WARN_TOO_SLOW, socket, ch, dimm, 0xFF);
              mapOutCh = 1;
              continue;
            }
          }
          //
          // UDIMMs and SODIMMs are not supported in the third DIMM slot
          //
          if (((*dimmNvList)[dimm].keyByte2 != SPD_RDIMM) && (dimm == 2))
          {
#ifdef SERIAL_DBG_MSG
            if (checkMsgLevel(host, SDBG_MINMAX)) {
              if ((*dimmNvList)[dimm].keyByte2 == SPD_UDIMM) {
                MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                              "UDIMM in 3rd slot!\n"));
              } else {
                MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                              "SODIMM in 3rd slot!\n"));
              }
            }
#endif
            OutputError (host, ERR_DIMM_COMPAT, ERR_INVALID_POP_MINOR_UDIMM_AND_3RD_SLOT, socket, ch, dimm, 0xFF);
            break;
            //
            // Break in case the user does not want to halt
            //
          }
          //
          // Make sure RDIMMs and UDIMMs are not mixed. They are not electrically compatible
          // DimmTypeStatus is initialized to zero and will get set to the DIMM type of the DIMM
          // detected on the first pass. On subsequent passes keyByte2 must equal DimmTypeStatus or
          // else there is a mismatch and the system should halt to prevent electrical damage.
          // SODIMMs are not expected to be mixed with either RDIMM or UDIMM.
          //
          if ((DimmTypeStatus != 0) && (DimmTypeStatus != ((*dimmNvList)[dimm].actKeyByte2 & 0x0F)))
          {
            MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                              "%s is mixed with %s!\n",
                              GetDimmTypeStr(host, (*dimmNvList)[dimm].actKeyByte2, (char *)&strBuf0),
                              GetDimmTypeStr(host, DimmTypeStatus, (char *)&strBuf1)));
            FatalError (host, ERR_MIXED_MEM_TYPE, 0);
            break;
            //
            // Break incase the user does not want to halt
            //
          }
          //
          // If DDR3U Detected, set the Vddq to 0 and log a fatal error
          //
          if ((*dimmNvList)[dimm].keyByte == SPD_TYPE_DDR3){
            if (host->nvram.mem.socket[socket].ulvDimmPresent != 0)
            {
              MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                           "Unsupported DIMM Voltage!\n"));

              host->nvram.mem.socket[socket].ddrVoltage = SPD_VDD_135;
              SetVdd (host, socket);

              OutputError (host, ERR_DIMM_COMPAT, ERR_INVALID_POP_MINOR_UNSUPPORTED_VOLTAGE, socket, ch, dimm, 0xFF);
              // OutputWarning (host, WARN_DIMM_COMPAT, WARN_DIMM_COMPAT_MINOR_VOLTAGE_MIX, socket, ch, dimm, 0xFF);
            }
          }

          //
          // DDR3 and DDR4 dimms cannot be mixed.  If a DDR4 dimm was previously detected
          // on the current channel and the current dimm is not DDR4 the system should halt.
          //
          if ((DimmType != 0) && (DimmType != (*dimmNvList)[dimm].keyByte)){
            MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
            "DDR3 is mixed with DDR4!\n"));
            FatalError (host, ERR_DIMM_COMPAT, ERR_DDR3_DDR4_MIXED);
            mapOutCh = 1;
            break;
            //
            // Map out the channel and break in case the user does not want to halt
            //
          }

          //
          // Disable ECC for the entire system if one DIMM is found that does not support it
          //
          if(dimm == 0){
            ECCDimmSupport = ((*dimmNvList)[dimm].SPDMemBusWidth & BIT3);
            if (ECCDimmSupport == 0) {
              host->nvram.mem.eccEn = 0;
            }
          }else{
            ECCDimmSupport =((*dimmNvList)[dimm-1].SPDMemBusWidth & BIT3);
          }

          if (((*dimmNvList)[dimm].SPDMemBusWidth & BIT3) != ECCDimmSupport) {

            OutputWarning (host, WARN_DIMM_COMPAT,  WARN_CHANNEL_MIX_ECC_NONECC, socket, ch, dimm, 0xFF);

            if (!(host->setup.mem.options & ECC_MIX_EN)){
              host->nvram.mem.eccEn = 0;
            } else {
              host->nvram.mem.eccEn = 1;
              DisableChannel(host, socket, ch);
            }
          }
#if !defined(DE_SKU) || defined(MEM_SV_HOOKS)
          if ((host->setup.mem.optionsExt & WR_CRC) && (host->var.common.cpuType == CPU_BDX) && (host->nvram.mem.eccEn) && (host->setup.mem.options & CA_PARITY_EN)) {
            host->nvram.mem.WrCRC = 1;
          } else {
            host->nvram.mem.WrCRC = 0;
          }
#endif
          //
          // Set DIMM Type status to the first type of DIMM we find. If a different type is detect later
          // a fatal error will be logged.  Mixed DIMM types are not supported.
          // DimmTypeStatus = SPD_X definition = keyByte2 (1,2 & 3 = RDIMM, UDIMM & SODIMM)
          // dimmTypePresent is zero based so = keyByte2 -1
          // Set DRAM type to first type found
          //
          if(DimmTypeStatus == 0){
            host->nvram.mem.dimmTypePresent = ((*dimmNvList)[dimm].keyByte2 - 1);
            DimmTypeStatus                  = (*dimmNvList)[dimm].actKeyByte2 & 0x0F;
            DimmType                        = (*dimmNvList)[dimm].keyByte;
          }

          //
          // Set local variable to indicate DIMM Type
          // Get number of ranks
          //
          (*dimmNvList)[dimm].numRanks = ((*dimmNvList)[dimm].SPDModuleOrg >> 3) & 7;
          //
          // Get Num of Ranks
          //
          ++(*dimmNvList)[dimm].numRanks;

          if ((*channelNvList)[ch].lrDimmPresent == 0) (*dimmNvList)[dimm].numDramRanks = (*dimmNvList)[dimm].numRanks;

#ifdef QR_DIMM_SUPPORT
          //
          // Check for quad rank DIMM
          //
          if ((*dimmNvList)[dimm].numRanks == 4) {
            //
            // Update number of QR DIMMs
            //
            (*channelNvList)[ch].numQuadRanks++;

            host->nvram.mem.socket[socket].qrPresent = 1;
          }
#endif // QR_DIMM_SUPPORT

          // Check if number of ranks on this channel has been exceeded
          //
          if (((*channelNvList)[ch].numRanks + (*dimmNvList)[dimm].numRanks) > MAX_RANK_CH)
          {
            MemDebugPrint((host, SDBG_MINMAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "Max ranks per ch exceeded\n"));
            OutputWarning (host, WARN_DIMM_COMPAT, WARN_RANK_NUM, socket, ch, 0xFF, 0xFF);

            //
            // Indicate this channel needs to be mapped out after detecting all populated DIMMs
            //
            mapOutCh = 1;
            continue;
          } else {
            //
            // Update ranks on this channel
            //
            (*channelNvList)[ch].numRanks = (*channelNvList)[ch].numRanks + (*dimmNvList)[dimm].numRanks;

            //
            // Check if this is a x4 DIMM
            //
            if (((*dimmNvList)[dimm].SPDModuleOrg & 7) == 0) {
              if (((*channelNvList)[ch].features & X8_PRESENT)) {
                //
                // Set flag to enable workaround
                //
                (*channelNvList)[ch].features |= X4_AND_X8_PRESENT;
              }
              (*dimmNvList)[dimm].x4Present = 1;
              (*channelNvList)[ch].features |= X4_PRESENT;
            }
            //
            // Check if this 16Gb SDRAM density is supported by the HW
            //
            if (((*dimmNvList)[dimm].sdramCapacity == SPD_16Gb) && ((*dimmNvList)[dimm].x4Present == 1)) {
              MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                            "x4 16Gb devices are not supported!\n"));
			  FatalError (host, ERR_DIMM_COMPAT, ERR_DIMM_COMPAT_16Gb_X4_NOT_SUPPORTED);
              mapOutCh = 1;
              continue;
            }
            //
            // Check if this is a x8 DIMM
            //
            if (((*dimmNvList)[dimm].SPDModuleOrg & 7) == 1) {
              if (((*channelNvList)[ch].features & X4_PRESENT)) {
                //
                // Set flag to enable workaround
                //
                (*channelNvList)[ch].features |= X4_AND_X8_PRESENT;
              }
              (*channelNvList)[ch].features |= X8_PRESENT;
            }

            // Check if this is a x16 DIMM
            if (((*dimmNvList)[dimm].SPDModuleOrg & 7) == 2) {
               (*channelNvList)[ch].features |= X16_PRESENT;
            }

            //
            // Save DIMM technology
            //
            (*dimmNvList)[dimm].techIndex = ti;

            //
            // Update the fastest common tCK
            //
            if (minTCKcommon < (*chdimmList)[dimm].minTCK) {
              minTCKcommon = (*chdimmList)[dimm].minTCK;
            }

            //
            // Indicate there is memory present on this channel
            //
            chMemPresent = 1;

            //
            // Indicate this DIMM is DDR4
            //
            if ((*dimmNvList)[dimm].keyByte == SPD_TYPE_DDR4) {
              dimmMtr.Bits.ddr4_mode = 1;
            }

            //
            // Check that DDR4 DIMM supports 1.2V.  If 1.2v not supported
            // map out channel so voltage level does not get set.
            //
            if ((*dimmNvList)[dimm].keyByte == SPD_TYPE_DDR4) {
              if (host->nvram.mem.socket[socket].channelList[ch].v120NotSupported) {
                MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                    "DDR4 DIMM does not support 1.2V!\n"));
                OutputWarning (host, WARN_DIMM_COMPAT, WARN_DIMM_VOLTAGE_NOT_SUPPORTED, socket, ch, dimm, 0xFF);
                mapOutCh = 1;
              }
            }

            //
            // Indicate this DIMM is populated
            //
            dimmMtr.Bits.dimm_pop = 1;

            //
            // Set rank count
            //
            dimmMtr.Bits.rank_cnt = ((*dimmNvList)[dimm].SPDModuleOrg >> 3) & 7;

            //
            // Get Num of Ranks
            //
            if (dimmMtr.Bits.rank_cnt == 3) {
              dimmMtr.Bits.rank_cnt--;
            }

            if (((*channelNvList)[ch].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {
              //
              // Check if we are in normal or extended address mode
              //
              if ((*dimmNvList)[dimm].numRowBits > 16) {
                //
                // enable hdrl for extended address mode
                //
                dimmMtr.Bits.hdrl = 1;
                dimmMtr.Bits.hdrl_parity = 1;
              } else {
                dimmMtr.Bits.hdrl = 0;
                dimmMtr.Bits.hdrl_parity = 0;
              }
            }

            if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
              //
              // 3DS and non-3DS lrdimms cannot be mixed.  If a one type was previously detected
              // on the current channel and the current dimm is not the same the system should halt.
              //
              if (dimm > 0) {
                if (((((*dimmNvList)[dimm].SPDDeviceType & SPD_SIGNAL_LOADING) == SPD_3DS_TYPE ) && (((*dimmNvList)[dimm-1].SPDDeviceType & SPD_SIGNAL_LOADING) != SPD_3DS_TYPE)) ||
                        ((((*dimmNvList)[dimm-1].SPDDeviceType & SPD_SIGNAL_LOADING) == SPD_3DS_TYPE ) && (((*dimmNvList)[dimm].SPDDeviceType & SPD_SIGNAL_LOADING) != SPD_3DS_TYPE))) {
                  MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                    "3DS is mixed with non-3DS LRDIMM!\n"));
                  FatalError (host, ERR_DIMM_COMPAT, ERR_MISMATCH_DIMM_TYPE);
                  mapOutCh = 1;
                  break;
                  //
                  // Map out the channel and break in case the user does not want to halt
                  //
                }
              }
              //
              // Setup encoded mode number of chip selects per rank, check if we are in 3DS, direct, or encoded mode
              //

              if (((*dimmNvList)[dimm].SPDDeviceType & SPD_SIGNAL_LOADING) == SPD_3DS_TYPE) {
                switch ((*dimmNvList)[dimm].dieCount) {
                  case 2:
                    dimmMtr.Bits.ddr4_3dsnumranks_cs = 1;
                    (*channelNvList)[ch].cidBitMap |= BIT0;
                    break;

                  case 4:
                    dimmMtr.Bits.ddr4_3dsnumranks_cs = 2;
                    (*channelNvList)[ch].cidBitMap |= (BIT0 + BIT1);
                    break;

                  case 8:
                    dimmMtr.Bits.ddr4_3dsnumranks_cs = 3;
                    (*channelNvList)[ch].cidBitMap |= (BIT0 + BIT1 + BIT2);
                    break;

                  default:
                    dimmMtr.Bits.ddr4_3dsnumranks_cs = 0;
                    break;
                }
              } else if ((*dimmNvList)[dimm].lrRankMult > 1) {
                //
                // Setup encoded mode number of chip selects per rank
                //
                dimmMtr.Bits.ddr4_3dsnumranks_cs = 1;
              } else {
                dimmMtr.Bits.ddr4_3dsnumranks_cs = 0;
              }
            }

            //
            // Set DRAM density (not used by the hardware)
            //
            if (((*dimmNvList)[dimm].sdramCapacity - 2) > 3) {
              dimmMtr.Bits.ddr3_dnsty =  3;
            } else {
              dimmMtr.Bits.ddr3_dnsty =  (*dimmNvList)[dimm].sdramCapacity - 2;
            }

            //
            // Set DIMM width (not used by the hardware)
            //
            if (host->var.common.cpuType == CPU_HSX) {
              dimmMtr.Data |= ((*dimmNvList)[dimm].SPDModuleOrg & 3) << 7;
            } else {
              dimmMtr.Data |= ((*dimmNvList)[dimm].SPDModuleOrg & 3) << 8;
            }

            //
            // Set Row Address width
            //
            dimmMtr.Bits.ra_width = (*dimmNvList)[dimm].numRowBits - 12;

            //
            // Set Column Address width
            //
            dimmMtr.Bits.ca_width = (*dimmNvList)[dimm].numColBits - 10;

            //
            // Save for later
            //
            (*dimmNvList)[dimm].dimmMemTech = dimmMtr.Data;

          }
        } // dimm loop

        //
        // Map out this channel if an invalid DIMM or configuration was detected
        //
        if (mapOutCh) {
          //
          // Disable this channel
          //
          (*channelNvList)[ch].enabled = 0;

          //
          // Indicate no memory is available on this channel
          //
          chMemPresent = 0;

          //
          // Mapping out DIMM 0 maps out all other DIMMs on this channel
          // MapOutDIMM(host, socket, ch, 0, ch);
          //
          maxDimm = 0;
        }

        //
        // The 3rd DIMM slot can not be populated when QR DIMMs are installed
        //
        if (((*channelNvList)[ch].numQuadRanks != 0) && (maxDimm > 2))
        {
          MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                        "3rd slot populated with QR DIMM present!\n"));
          OutputError (host, ERR_DIMM_COMPAT, ERR_INVALID_POP_MINOR_QR_AND_3RD_SLOT, socket, ch, dimm, 0xFF);
        }

        //
        // Save the max number of DIMMs on this channel to enforce population rules
        //
        (*channelNvList)[ch].maxDimm = maxDimm;

        //
        // Disable this channel if there is no memory installed
        //
        if (maxDimm == 0) {
          (*channelNvList)[ch].enabled = 0;

          MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "Channel disabled in MemSPD: mcId = %d, mcCh = %d\n", GetMCID(host, socket, ch), GetMCCh(GetMCID(host, socket, ch), ch)));
        }
        //
        // Keep track of the largest number of DIMMs in a channel on this socket
        //
        if (host->nvram.mem.socket[socket].maxDimmPop < maxDimm) {
          host->nvram.mem.socket[socket].maxDimmPop = maxDimm;
        }
        //
        // Or into the global MemPresent status
        //
        MemPresent |= chMemPresent;

      } // ch loop

      if (host->nvram.mem.socket[socket].minTCK < minTCKcommon) {
        host->nvram.mem.socket[socket].minTCK = minTCKcommon;
      }

      if (!(host->nvram.mem.socket[socket].ddrVoltage & SPD_VDD_135) && host->nvram.mem.socket[socket].lvDimmPresent) {
        MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "DDR3L DIMMs operating at 1.50V!\n"));
      }
    } // socket loop

    //
    // Check for no memory error
    //
    if (MemPresent == 0)
    {
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "No memory found!\n"));
      FatalError (host, ERR_NO_MEMORY, ERR_NO_MEMORY_MINOR_NO_MEMORY);
    }

    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "%s population\n", GetDimmTypeStr(host, DimmTypeStatus, (char *)&strBuf0)));
  } // if (host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)

  //
  // Configure imcEnabled
  //
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (host->nvram.mem.socket[socket].enabled == 0) continue;

    channelNvList = GetChannelNvList(host, socket);

    if (Is2HA(host)) {
      if (((*channelNvList)[0].enabled) || ((*channelNvList)[1].enabled)) {
        host->var.mem.socket[socket].imcEnabled[0] = 1;
      } else {
        host->var.mem.socket[socket].imcEnabled[0] = 0;
      }
      if (((*channelNvList)[2].enabled) || ((*channelNvList)[3].enabled)) {
        host->var.mem.socket[socket].imcEnabled[1] = 1;
      } else {
        host->var.mem.socket[socket].imcEnabled[1] = 0;
      }
    } else {
      if (((*channelNvList)[0].enabled) || ((*channelNvList)[1].enabled) || ((*channelNvList)[2].enabled) || ((*channelNvList)[3].enabled)) {
        host->var.mem.socket[socket].imcEnabled[0] = 1;
      } else {
        host->var.mem.socket[socket].imcEnabled[0] = 0;
      }
      host->var.mem.socket[socket].imcEnabled[1] = 0;
    }
  } // socket loop

 CoreCheckPORCompat(host);

  return SUCCESS;
}

UINT8
GetTechIndex (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    dramType
  )
/*++

  Returns the tech index for the current dimm

  @param host     - Pointer to sysHost
  @param socket   - Socket ID
  @param ch       - Channel on socket
  @param dimm     - DIMM on channel
  @param dramType - DRAM Type

  @retval Tech Index

--*/
{
  UINT8               ti;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];

  dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

  if (dramType == SPD_TYPE_DDR3) {
    //
    // DDR3
    //
    for (ti = 0; ti < MAX_TECH; ti++) {

      if (((primaryWidth[ti] >> 3) == ((*dimmNvList)[dimm].SPDModuleOrg & 7)) &&
          (rowBits[ti] == (*dimmNvList)[dimm].numRowBits) &&
          (columnBits[ti] == (*dimmNvList)[dimm].numColBits) &&
          (internalBanks[ti] == (*dimmNvList)[dimm].numBanks)
          ) {
        break;
      }
    } // ti loop

    if (ti >= MAX_TECH) {
      ti = 0xFF;
    }
  } else {
    //
    // DDR4
    //
    for (ti = 0; ti < MAX_TECH_DDR4; ti++) {

      if (((primaryWidthDDR4[ti] >> 3) == ((*dimmNvList)[dimm].SPDModuleOrg & 7)) &&
          (rowBitsDDR4[ti] == (*dimmNvList)[dimm].numRowBits) &&
          (columnBitsDDR4[ti] == (*dimmNvList)[dimm].numColBits) &&
          (bankGroupsDDR4[ti] == (*dimmNvList)[dimm].numBankGroups)
          ) {
        break;
      }
    } // ti loop

    if (ti >= MAX_TECH_DDR4) {
      ti = 0xFF;
    }
  }

  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "primaryWidthDDR4: %d, rowBitsDDR4: %d, columnBitsDDR4: %d, bankGroupsDDR4: %d \n",((*dimmNvList)[dimm].SPDModuleOrg & 7),
                (*dimmNvList)[dimm].numRowBits, (*dimmNvList)[dimm].numColBits, (*dimmNvList)[dimm].numBankGroups));

  return ti;
} // GetTechIndex

/**

    GetDdr4SpdPageEn - Detect DDR4 vs. DDR3 SPD devices on SMBus

    @param host       - Pointer to sysHost
    @param socket     - Socket ID

    @retval ddr4SpdPageEn = 1 for DDR4 SPD, 0 for DDR3 SPD

**/
UINT8
GetDdr4SpdPageEn (
  PSYSHOST         host,
  UINT8            socket
  )
{
  UINT8             ch;
  UINT8             dimm;
  UINT8             seg;
  UINT8             ddr4SpdPageEn = 0;
  UINT8             SPDReg;
  UINT8             spd256 = 0;
  UINT8             spd512 = 0;
  UINT32            status;
  UINT8             temp = 0;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct smbDevice     spd;
  BIOSSCRATCHPAD1_UBOX_MISC_STRUCT biosScratchPad1;

  channelNvList = GetChannelNvList(host, socket);

  //
  // Do not allow DDR4 page selection when a 256 byte DDR3 SPD device is present
  // because the DDR4 SPA command is the same as DDR3 PSWP command (non-volatile lock)
  //

  //
  // Assume DIMM config cannot change and nvram struct is valid during S3/ADR resume or fast warm reset flows
  //
  if ((host->var.common.bootMode == S3Resume || host->var.mem.subBootMode == WarmBootFast)) {

    ddr4SpdPageEn = host->nvram.mem.socket[socket].ddr4SpdPageEn;

  // Else if warm reset path, then assume sticky scratchpad bit is valid and nvram struct is not valid
  } else if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == WarmBoot)) {

    biosScratchPad1.Data = ReadCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG);
    if (biosScratchPad1.Data & BIT5) {
      ddr4SpdPageEn = 1;
    } else {
      ddr4SpdPageEn = 0;
    }

  // Else assume that sticky bit not valid and DIMM config could have changed
  } else {

    // Lets find if DDR4 DIMMs are present in the system in previous boot.
    biosScratchPad1.Data = ReadCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG);
    if (biosScratchPad1.Data & BIT5) {
      ddr4SpdPageEn = 1;
    } else {
      ddr4SpdPageEn = 0;
    }

    // Assume SPD device defaults to page 0 and detect DIMM config
    host->nvram.mem.socket[socket].ddr4SpdPageEn = 0;
    for (seg = 0; seg < MAX_IMC; seg++) {
      host->nvram.mem.socket[socket].imc[seg].spdPageAddr = 0;
    }
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      dimmNvList = GetDimmNvList(host, socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        // Set SPD devices to page 0 as default for DDR4.
        // DDR3 always set to page0 since it has only one page.
        if (host->setup.mem.optionsExt & DDR4_PLATFORM) {
          dimmNvList = GetDimmNvList(host, socket, ch);
          for (dimm = 0; dimm < MAX_DIMM; dimm++) {
            if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
            spd.compId = SPD;
            spd.address.controller = PROCESSOR;
            spd.address.deviceType = DTI_EEPROM;
            GetSmbAddress(host, socket, ch, dimm, &spd);
            // Update current page
            host->nvram.mem.socket[socket].imc[spd.address.busSegment].spdPageAddr = 0;
            // Set page
            spd.address.deviceType = DTI_WP_EEPROM;
            spd.address.strapAddress = SPD_EE_PAGE_SELECT_0;
            // Write to SPA
            WriteSmb (host, socket, spd, 0, &temp);
          }
        }//DDR4
        status = ReadSpd (host, socket, ch, dimm, SPD_SIZE, &SPDReg);
        if (status == SUCCESS) {
          if ((SPDReg & (BIT6 | BIT5 | BIT4)) == BIT4) {
            spd256 = 1;
          } else if ((SPDReg & (BIT6 | BIT5 | BIT4)) == BIT5) {
            ddr4SpdPageEn = 1;
            spd512 = 1;
          }
        }
      } //dimm loop
    } //ch loop

    // Check for mixing error
    if (spd256 && spd512) {

      MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        "Mixed 256 and 512 byte SPD devices!\n"));

      OutputError (host, ERR_DIMM_COMPAT, ERR_MIXED_SPD_TYPE, socket, 0xFF, 0xFF, 0xFF);
    } //mixed
  } // else

  // Set sticky scratchpad bit = 1 for DDR4, 0 for DDR3
  if (ddr4SpdPageEn) {
    biosScratchPad1.Data = ReadCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG);
    biosScratchPad1.Data |= BIT5;
    WriteCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG, biosScratchPad1.Data);
  }

  return ddr4SpdPageEn;
}


#if 0
UINT16
GetChannelDimmTiming (
  PSYSHOST  host,
  UINT32    tCKmin,
  UINT32    minPossible,
  UINT32    maxPossible,
  UINT32    timingMTB,
  INT32     timingFTB
) {
  /*++
    Calculate the minimum timing value for the given DIMMs.

    @param tCKmin      - The actual SDRAM cycle time (tCK)
    @param minPossible - The minimum value possible for the timing value
    @param maxPossible - The maximum value possible for the timing value
    @param TimingMTB   - The timing's MTB value
    @param TimingFTB   - The timing's FTB value

    @retval TRUE if the timing value has been calculated, otherwise FALSE and the returned value is set to zero.
  --*/

  INT32   mediumTimebase;
  INT32   fineTimebase;
  UINT32  actual;
  UINT32  desired;
  UINT32  calculated;

  actual = 0;

  if (TRUE == GetDimmTimeBase (host, pDimm->MemoryProfile, &medium_timebase, &fine_timebase)) {
    desired = (mediumTimebase * timingMTB) + (fineTimebase * timingFTB);
    calculated = ((desired + (tCKmin - 1)) / tCKmin);
    if (*actual < calculated) {
      actual = calculated;
    }
  }

  return actual;
}
#endif // if 0


void
UpdateAccessMode(PSYSHOST host, UINT8 socket)
{
  //
  // Stubbed out as this is not supported for this chipset (NVMCTLR feature)
  //
}

void
WaitForMailboxReady (PSYSHOST host, UINT8 socket)
{
  //
  // Stubbed out as this is not supported for this chipset (NVMCTLR feature)
  //
}

/**

    Get the Serial Number of the requested module

    @param host       - Pointer to sysHost
    @param socket     - Socket ID
    @param ch         - Channel on socket
    @param dimm       - DIMM on channel
    @param i          - The number of the Module serial byte to check
    @param SPDReg     - Pointer to the SPD Register

    @retval N/A

**/
void
GetModuleSerialNumber(
                     PSYSHOST  host,
                     UINT8     socket,
                     UINT8     ch,
                     UINT8     dimm,
                     UINT8     i,
                     UINT8     *SPDReg
                     )
{
  if (*SPDReg == SPD_TYPE_DDR3){
    ReadSpd (host, socket, ch, dimm, SPD_MODULE_SN + i, SPDReg);
  } else {
    ReadSpd (host, socket, ch, dimm, SPD_MODULE_SN_DDR4 + i, SPDReg);
  }
}

struct dimmDevice (*GetChDimmList (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch
  ))[MAX_DIMM]
{
  return (&host->var.mem.socket[socket].channelList[ch].dimmList);
}

void
MemRankGeometry (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     cBits[MAX_RANK_CH][MAX_CH],
  UINT8     rBits[MAX_RANK_CH][MAX_CH],
  UINT8     bBits[MAX_RANK_CH][MAX_CH],
  UINT8     rankEnabled[MAX_RANK_CH][MAX_CH],
  UINT8     dimmRank[MAX_RANK_CH][MAX_CH],
  UINT8     rankChEnabled[MAX_RANK_CH],
  UINT8     *maxEnabledRank,
  UINT8     logical2Physical[MAX_RANK_CH][MAX_CH]
  )
/*++

  Lookup geometry information for all logical ranks

  @param host                - Pointer to sysHost
  @param socket              - Socket ID
  @param cBits               - number of column bits
  @param rBits               - number of row bits
  @param bBits               - number of bank bits
  @param rankEnabled         - logical rank enabled
  @param dimmRank            - dimm containing logicalRank
  @param rankChEnabled       - at least one logical rank on channels enabled
                               (OR of rankEnabled for all channels)
  @param maxEnabledRank      - max logical rank enabled on node

  @retval N/A

--*/
{
  UINT8               dimm;
  UINT8               rank;
  UINT8               logicalRank;
  UINT8               ch;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];

  *maxEnabledRank = 0;

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  for (ch = 0; ch < MAX_CH; ch++) {
    rankChEnabled[ch] = 0;

    dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

    for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
#ifdef MEM_NVDIMM_EN
      if (host->var.mem.subBootMode == NvDimmResume && ((*dimmNvList)[dimm].nvDimmType) && ((*dimmNvList)[dimm].nvDimmStatus & STATUS_RESTORE_SUCCESSFUL)) {
        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                       "NVDIMM Excluded from MemInit/MemTest DIMM map.\n"));
         continue;
      }
#endif  //MEM_NVDIMM_EN

      rankList = &(*dimmNvList)[dimm].rankList;

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {

        //
        // Update logical rank geometry information
        //
        if (CheckRank(host, socket, ch, dimm, rank, CHECK_MAPOUT)) continue;
        logicalRank = (*rankList)[rank].logicalRank;
        if (logicalRank > *maxEnabledRank) {
          *maxEnabledRank = logicalRank;
        }
        if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
          cBits[logicalRank][ch] = columnBitsDDR4[(*dimmNvList)[dimm].techIndex];
          rBits[logicalRank][ch] = rowBitsDDR4[(*dimmNvList)[dimm].techIndex];
          if (((*dimmNvList)[dimm].SPDSecondaryDeviceType & SPD_HALF_SIZE_SECOND_RANK) && ((rank) % 2)) {
            rBits[logicalRank][ch] = rBits[logicalRank][ch] - 1;
          }
          if (((*dimmNvList)[dimm].SPDSecondaryDeviceType & SPD_QUARTER_SIZE_SECOND_RANK) && ((rank) % 2)) {
            rBits[logicalRank][ch] = rBits[logicalRank][ch] - 2;
          }
          bBits[logicalRank][ch] = 4;
        } else {
          cBits[logicalRank][ch] = columnBits[(*dimmNvList)[dimm].techIndex];
          rBits[logicalRank][ch] = rowBits[(*dimmNvList)[dimm].techIndex];
          bBits[logicalRank][ch] = 3;
          //DDR3 only has 16 row bits, so mask off higher bits, those would have to be handled as separate logical ranks
          if (rBits[logicalRank][ch] > 16) rBits[logicalRank][ch] = 16;
        }
        dimmRank[logicalRank][ch] = dimm;
        logical2Physical[logicalRank][ch] = rank;
        rankEnabled[logicalRank][ch] = 1;
        rankChEnabled[ch] = 1;
      } // for rank
    } // for dimm
  } // for ch

  return;
} // MemRankGeometry

/**

  A hook into chip-level code that is called during DIMM detect
  phase in MRC before warm reset. This function is called once per
  socket, after the DIMM detect is complete.

  @param host  - Pointer to the system host (root) structure
  @param socket - socket ID

**/
VOID
PostDimmDetectChipHook (
  PSYSHOST host,
  UINT8    socket
  )
{

}
