//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c)  1999 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

Abstract:

--*/

#include <Token.h> // AptioV server override
#include <MemDefaults.h>

//
// Remove the comtents but keep the stub to pass the build process for future clean up.
// Until the default values of HII VFR forms becomes available, we will keep the QPI RC defaults as from previous programs
//
static COMMONRC_CONFIG CommonRcConfigDefault = {
    MMCFG_BASE_ADDRESS,               //UINT32   MmcfgBase;
    MMCFG_SIZE,                       //UINT32   MmcfgSize;
    MMCFG_BASE_ADDRESS+MMCFG_SIZE,    //UINT32   MmiolBase;
    MMIOL_LIMIT-MMCFG_BASE_ADDRESS-MMCFG_SIZE+ 1, //UINT32   MmiolSize;
    MMIOH_BASE,                       //UINT16   MmiohBase;
    MMIOH_SIZE,                       //UINT16   MmiohSize;
    0,                                //UINT8   IsocEn;
    0,                                //UINT8   MeSegEn;
    1,                                //UINT8   NumaEn;
};

static CSI_CONFIG CsiConfigDefault = {
    0
};

static MEMORY_CONFIG MemoryConfigDefault = {
    21,   // UINT8                   Slit1HopValue;
    1,    // UINT8                   InterleaveType;
    NUMA_DEFAULT,    //UINT8                    NUMA;
    0,    //UINT8                    svMaxNodeInterleave;
//    UINT8                   MemoryHotPlug;
    0,    // UINT8                   MemoryHotPlugBase;
    1,    // UINT8                   MemoryHotPlugLen;
    0,    // UINT8                   SratMemoryHotPlug;
    0,    // UINT8                   SratCpuHotPlug;
    0,    // UINT8                   Srat;
    0,    // UINT8                   StandbyToS1;
    CLOSED_PAGE_DEFAULT,    // UINT8                   PagePolicy;
    0,    // UINT8                   TgtIdxDecode;
    0,    // UINT8                   LowGap;
    0,    // UINT8                   HighGap;
    0,    // UINT8                   Mpst;
    0,    // UINT8                   Lfo;
    0,    // UINT8                   FreqLimit;
    1,    // UINT8                   PatrolScrub;
    PATROL_SCRUB_DURATION_DEFAULT,   // UINT8                   PatrolScrubDuration;
    1,    // UINT8                   DemandScrubMode;
    0,    // UINT8                   DeviceTaggingMode;
    0,    // UINT8                   SpareMode;
    0x7FFF,// UINT16                 spareErrTh;
    0x28, // UINT8                   leakyBktLo
    0x29, // UINT8                   leakyBktHi
    DRAM_MAINT_AUTO,  //             DramMaint;
    DRAM_MAINT_TRR,   //             DramMaintMode;
    DRAM_MAINT_MODE_A,//             DramMaintTRRMode;
    1,    // UINT8                   WritePosting;
    0,    // UINT8                   S15DimmSelfRefresh;
    1,    // UINT8                   MrcDebugMessages;
    0,    // UINT8                   HighTemp;
    0,    // UINT8                   SpreadSpectrum;
    0,    // UINT8                   ThermThrt;
    6,    // UINT8                   TempGb;
    70,   // UINT8                   TempInlet;
    0,    // UINT8                   TempRise;
    500,  // UINT16                  AirFlow;
    1500, // UINT16                  Altitude;
    400,  // UINT16                  DimmPitch;
    1,    // UINT8                   Socket0Branch0En;
    1,    // UINT8                   Socket0Branch1En;
    1,    // UINT8                   Socket1Branch0En;
    1,    // UINT8                   Socket1Branch1En;
    1,    // UINT8                   Socket2Branch0En;
    1,    // UINT8                   Socket2Branch1En;
    1,    // UINT8                   Socket3Branch0En;
    1,    // UINT8                   Socket3Branch1En;
    0,    // UINT8                   Socket0Branch0XorMap;
    0,    // UINT8                   Socket0Branch1XorMap;
    0,    // UINT8                   Socket1Branch0XorMap;
    0,    // UINT8                   Socket1Branch1XorMap;
    0,    // UINT8                   Socket2Branch0XorMap;
    0,    // UINT8                   Socket2Branch1XorMap;
    0,    // UINT8                   Socket3Branch0XorMap;
    0,    // UINT8                   Socket3Branch1XorMap;

    2,    // UINT8                   DdrMemoryType;
    MEM_TEST_DEFAULT,       // UINT8  HwMemTest;
    MEM_TEST_LOOPS_DEFAULT, //UINT16  MemTestLoops;
    DMT_AUTO,               //UINT8   DramMaintenanceTest:
    DMT_DIRECTION_UP,       //UINT8   DMTDir;
    DMT_INVERT_DIS,         //UINT8   DMTInv;
    DMT_REPETITIONS,        //UINT32  DMTRep;
    DMT_ITERATIONS,         //UINT32  DMTIter;
    DMT_SWIZZLE_AUTO,       //UINT8   DMTSwizzleEn;
    DMT_REFRESH_EN,         //UINT8   DMRefreshEn;
    ECC_DEFAULT,            // UINT8  EccSupport;
    0,                      // UINT8  SocketInterleaveBelow4GB;
    CH_INTER_DEFAULT,       // UINT8  ChannelInterleaving;
    RANK_INTER_DEFAULT,     // UINT8  RankInterleaving;
    CKE_MODE_DEFAULT,       // UINT8  ckeThrottling;
    CK_SELF_REF_AUTO,       // UINT8  pdwnCkMode;
    OPP_SR_DEFAULT,         // UINT8  oppSR;
    DDR3_VDD_DEFAULT,       // UINT8  DdrVoltage;
    MEM_TEST_FAST_BOOT_DEFAULT, // UINT8 MemTestOnFastBoot;
    FAST_BOOT_DEFAULT,      // UINT8  AttemptFastBoot;
    FAST_BOOT_COLD_DEFAULT, // UINT8  AttemptFastBootCold;
    0,    // UINT8                   bdatEn;
    SCRAMBLE_DEFAULT,    // UINT8                   ScrambleEn;
    ALLOW_CORRECTABLE_ERROR_DEFAULT,    // UINT8                   allowCorrectableErrors;
    41003,// UINT16                 ScrambleSeedLow;
    54165,// UINT16                 ScrambleSeedHigh;
    0,    // UINT8                   enableNbPerst;
    0x10, // UINT16                  nbErrTh;
    0,    // UINT8                   enableSbPerst;
    0x10, // UINT16                  sbErrTh;
    1,    // UINT8                   zqCalEn;
    0,    // UINT8                   refreshMode;
    0,    // UINT8                   CustomRefreshRateEn;
    Cust_Refresh_Rate_Default, //UINT8            CustomRefreshRate;
    1,    // UINT8                   lrdimCas2CasSubrank;
    0,    // UINT8                   mc3dsSupport;
    0,    // UINT8                   dddcWireKill;
    0x16, // UINT8                   pushtoCasLatency;
    BGF_THRESHOLD, // UINT8          mcBgfThreshold;
    DLL_RESET_TEST_LOOPS, // UINT8   dllResetTestLoops;
    1,    // UINT8                   haEarlyWritePost;
    1,    // UINT8                   safeTurnAround;
    RX_VREF_DEFAULT,    // UINT8                   readVrefCenter;
    TX_VREF_DEFAULT,    // UINT8                   wrVrefCenter;
    1,    // UINT8                   cmd2DataTuning;
    1,    // UINT8                   haltOnMemErr;
    0,    // UINT8                   dramRaplMode;
    //1,    // UINT8                   tolm;
    0,    // UINT8                   thermalthrottlingsupport;
    0,    // UINT8                   DimmTempStatValue;
    MC_ODT_DEFAULT,    // UINT8                   mcODTOverride;
    1,    // UINT8                   advancedClkTraining;
    0,    // UINT8                   cmdNormalize;
    1,    // UINT8                   advancedStrobeCentering;
    1,    // UINT8                   extDimmVrefCircuit;
    EARLY_CMD_CLK_DEFAULT,          // earlyCmdTraining;
    EARLY_CTL_CLK_DEFAULT,          // earlyCtlTraining;
    TX_EQ_AUTO,                     // txEqCalibration
    IMODE_AUTO,                     // iModeTraining
    ROUND_TRIP_LATENCY_DEFAULT,     // RoundTripLatency;
    DUTY_CYCLE_AUTO,                // DutyCycleTraining
    0,    // UINT8                   XMPProfilesSup;
    0,    // UINT8                   XMPMode;
    0,    // UINT8                   tCAS;
    0,    // UINT8                   tRP;
    0,    // UINT8                   tRCD;
    0,    // UINT8                   tRAS;
    0,    // UINT8                   tWR;
    0,    // UINT16                  tRFC;
    0,    // UINT8                   tRRD;
    0,    // UINT8                   tRTP;
    0,    // UINT8                   tWTR;
    0,    // UINT8                   tFAW;
    0,    // UINT8                   tCWL;
    0,    // UINT8                   tRC;
    0,    // UINT8                   commandTiming;
    0,    // UINT16                  tREFI;
    DDR3_FREQ_DEFAULT,  // UINT8     DdrFreqLimit;
    0,                  // UINT8     Vdd;
    RANK_MULT_AUTO,     // UINT8     lrdimmRankMult;
    MODULE_DELAY_AUTO,  // UINT8     lrdimmModuleDelay;
    RCOMP_TYPE_AUTO,    // UINT8     rcompType;
    0,    // UINT8                   PerBitMargins;
    RMT_PATTERN_LENGTH, // UINT32    rmtPatternLength;
    0,    // UINT8                   RxSenseAmp;
    0,    // UINT8                   ADRCh1;
    0,    // UINT8                   ADRCh2;
    0,    // UINT8                   ADRCh3;
    0,    // UINT8                   check_pm_sts;
    0,    // UINT8                   check_platform_detect;
#ifdef MEM_NVDIMM_EN
    1,    // UINT8                   eraseArmNVDIMMS
    1,    // UINT8                   restoreNVDIMMS
#endif
#ifdef NVMEM_FEATURE_EN
    0,   // UINT8                    interNVDIMMS
#endif
    0,    // UINT8                   ddrCycling;
    0,    // UINT8                   perfStats;
    0,    // UINT8                   MemoryLaiProbesPresent;
    0,    // UINT8                   RxOffCalibration;
    IDLE_PAGE_RST_VAL,  // UINT8     idlePageRstVal;
    0,    // UINT8                   serialDebugMsgLvlTrainResults;
    0,    // UINT8                   eyeDiagram;
    0,    // UINT8                   cmdClkTraining;
    0,    // UINT8                   numVrefPoints;
    0,    // UINT8                   vrefStepSize;
    4,    // UINT8                   compOptions;
    1,    // UINT8                   zqOptions;
    MEM_PWR_SAV_AUTO,    // UINT8                   MemPwrSave;
    0,    // UINT8                   ElectricalThrottlingMode;
    0x200,// UINT16                  ChPpdsIdleTimer;
    0x40, // UINT8                   pdwnIdleCntr;
    MULTI_THREADED_DEFAULT,// UINT8  MultiThreaded;
    1,    // UINT8                   promoteMrcWarnings;
    1,    // UINT8                   promoteWarnings;
    OPP_RD_WMM_DEFAULT,    // UINT8                   oppReadInWmm;
    1024, // UINT16                  normOppInteval;
    4,    // UINT16                  numSparingTransactions;
    0,    // UINT8                   PSMISupport;
    0xFF, // UINT8                   sck0ch0;
    0xFF, // UINT8                   sck0ch1;
    0xFF, // UINT8                   sck0ch2;
    0xFF, // UINT8                   sck0ch3;
    0xFF, // UINT8                   sck1ch0;
    0xFF, // UINT8                   sck1ch1;
    0xFF, // UINT8                   sck1ch2;
    0xFF, // UINT8                   sck1ch3;
    0xFF, // UINT8                   sck2ch0;
    0xFF, // UINT8                   sck2ch1;
    0xFF, // UINT8                   sck2ch2;
    0xFF, // UINT8                   sck2ch3;
    0xFF, // UINT8                   sck3ch0;
    0xFF, // UINT8                   sck3ch1;
    0xFF, // UINT8                   sck3ch2;
    0xFF, // UINT8                   sck3ch3;
    A7_MODE_DEFAULT, // UINT8        a7Mode;
    0,    // UINT8                   dddcSupport;
    MDLL_SDEN_DEFAULT,    // UINT8                   mdllSden;
    0,    // UINT8                   memhotSupport;
    0,    // UINT8                   ADREn;
    1,    // UINT8                   DdrVrPhaseShedding;
    RMT_DEFAULT, // UINT8                   RankMargin;
    0,    // UINT8                   rankMaskEn;
    0,    // UINT8                   enableSparing;
    4,	  // UINT8                   multiSparingRanks;
    0,    // UINT8 	             svCh0SpareRank;
    0,    // UINT8 	             svCh1SpareRank;
    0,    // UINT8 	             svCh2SpareRank;
    0,    // UINT8 	             svCh3SpareRank;
    LOCKSTEPX4_ENABLE,    // UINT8                   lockstepEnableX4;
    0,    // UINT8                   caParity;
    SMB_CLK_DEFAULT,    // UINT8                   smbSpeed;
    BIT_DESKEW_DEFAULT,            // perBitDeskew
    ENFORCE_POR_DEFAULT,            // EnforcePOR
    PDA_DEFAULT,                    // pda
    TURNAROUND_DEFAULT,             // turnaroundOpt
    IOT_BUFFER_DEFAULT,             // IotMemBufferRsvtn
    2,    // UINT8                  DramRaplInit;
    1,    // UINT8                  BwLimitTfOvrd
    PER_BIT_MARGIN_DEFAULT,         // perbitmargin
    DRAM_RAPL_EXTENDED_RANGE_DEFAULT, //DramRaplExtendedRange
    LOG_PARSING_DIS,                // UINT8  logParsing

#ifdef DE_SKU
    0,     //BlockScTrafficOnAdr
#endif
};

//
// Defaults for BDX_MEMORY_CONFIG structure
// if the setup variable is not present. Not
// currently used.
//

static BDX_MEMORY_CONFIG BdxMemoryConfigDefault = {
  CMD_VREF_AUTO,                // UINT8  cmdVrefCenter;
  RX_CTLE_AUTO,                 // trainrxctle
  LRDIMM_BACKSIDE_VREF_AUTO,    // lrdimmbacksideVref

#ifdef SSA_FLAG
  BIOS_SSA_DEFAULT,             // enableBiosSsaLoader
#endif

  0,                            // UINT8  C2FMode
  0,                            // MemDownConfig
  1,                           //ADRDataSaveMode = ADR_BBU;


#if !defined(DE_SKU) || defined(SV_HOOKS)
  WR_CRC_AUTO,
#endif
  CECC_WA_CHMASK_DEFAULT,       // UINT8 ceccWaChMask
  RMT_COLD_FAST_BOOT_DEFAULT,     // RmtOnColdFastBoot

};


static MISC_CONFIG MiscConfigDefault = {
    1,    // UINT8                   ValidationIohMiscSsWa;
    0,    // UINT8                   Poison;
    0,    // UINT8                   Viral;
    //Do not change order of IoApic BYTES or insert items between them...
    1,    // UINT8                   DevPresIoApicIio0;
    1,    // UINT8                   DevPresIoApicIio1;
    1,    // UINT8                   DevPresIoApicIio2;
    1,    // UINT8                   DevPresIoApicIio3;
    1,    // UINT8                   Ck410bConfigSpreadSpectrumEnable;
    0     // UINT8                   EVMode;
};
