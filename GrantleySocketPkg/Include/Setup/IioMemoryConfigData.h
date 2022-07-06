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
  IioMemoryConfigData.h

Abstract:
  Data format for Ioh Memory Config Data Structure

--*/

//typedef struct {
    UINT8                   Slit1HopValue;
    UINT8                   InterleaveType;
    UINT8                   NUMA;
    UINT8                   svMaxNodeInterleave;
//    UINT8                   MemoryHotPlug;
    UINT8                   MemoryHotPlugBase;
    UINT8                   MemoryHotPlugLen;
    UINT8                   SratMemoryHotPlug;
    UINT8                   SratCpuHotPlug;
    UINT8                   Srat;
    UINT8                   StandbyToS1;
    UINT8                   PagePolicy;
    UINT8                   TgtIdxDecode;
    UINT8                   LowGap;
    UINT8                   HighGap;
    UINT8                   Mpst;
    UINT8                   Lfo;
    UINT8                   FreqLimit;
    UINT8                   PatrolScrub;
    UINT8                   PatrolScrubDuration;
    UINT8                   DemandScrubMode;
    UINT8                   DeviceTaggingMode;
    UINT8                   SpareMode;
    UINT16                  spareErrTh;
    UINT8                   leakyBktLo;
    UINT8                   leakyBktHi;
    UINT8                   DramMaint;
    UINT8                   DramMaintMode;
    UINT8                   DramMaintTRRMode;
    UINT8                   WritePosting;
    UINT8                   S15DimmSelfRefresh;
    UINT8                   MrcDebugMessages;
    UINT8                   HighTemp;
    UINT8                   SpreadSpectrum;
    UINT8                   ThermThrt;
    UINT8                   TempGb;
    UINT8                   TempInlet;
    UINT8                   TempRise;
    UINT16                  AirFlow;
    UINT16                  Altitude;
    UINT16                  DimmPitch;
    UINT8                   Socket0Branch0En;
    UINT8                   Socket0Branch1En;
    UINT8                   Socket1Branch0En;
    UINT8                   Socket1Branch1En;
    UINT8                   Socket2Branch0En;
    UINT8                   Socket2Branch1En;
    UINT8                   Socket3Branch0En;
    UINT8                   Socket3Branch1En;
    UINT8                   Socket0Branch0XorMap;
    UINT8                   Socket0Branch1XorMap;
    UINT8                   Socket1Branch0XorMap;
    UINT8                   Socket1Branch1XorMap;
    UINT8                   Socket2Branch0XorMap;
    UINT8                   Socket2Branch1XorMap;
    UINT8                   Socket3Branch0XorMap;
    UINT8                   Socket3Branch1XorMap;

    UINT8                   DdrMemoryType;
    UINT8                   HwMemTest;
    UINT16                  MemTestLoops;
    UINT8                   DramMaintenanceTest;
    UINT8                   DMTDir;
    UINT8                   DMTInv;
    UINT32                  DMTRep;
    UINT32                  DMTIter;
    UINT8                   DMTSwizzleEn;
    UINT8                   DMRefreshEn;
    UINT8                   EccSupport;
    UINT8                   SocketInterleaveBelow4GB;
    UINT8                   ChannelInterleaving;
    UINT8                   RankInterleaving;
    UINT8                   ckeThrottling;
    UINT8                   pdwnCkMode;
    UINT8                   oppSR;
    UINT8                   DdrVoltage;
    UINT8                   MemTestOnFastBoot;
    UINT8                   AttemptFastBoot;
    UINT8                   AttemptFastBootCold;
    UINT8                   bdatEn;
    UINT8                   ScrambleEn;
    UINT8                   allowCorrectableError;
    UINT16                  ScrambleSeedLow;
    UINT16                  ScrambleSeedHigh;
    UINT8                   enableNbPerst;
    UINT16                  nbErrTh;
    UINT8                   enableSbPerst;
    UINT16                  sbErrTh;
    UINT8                   zqCalEn;
    UINT8                   refreshMode;
    UINT8                   CustomRefreshRateEn;
    UINT8                   CustomRefreshRate;
    UINT8                   lrdimCas2CasSubrank;
    UINT8                   mc3dsSupport;
    UINT8                   dddcWireKill;
    UINT8                   pushtoCasLatency;
    UINT8                   mcBgfThreshold;
    UINT8                   dllResetTestLoops;
    UINT8                   haEarlyWritePost;
    UINT8                   safeTurnAround;
    UINT8                   readVrefCenter;
    UINT8                   wrVrefCenter;
    UINT8                   cmd2DataTuning;
    UINT8                   haltOnMemErr;
    UINT8                   dramRaplMode;
    //UINT8                   tolm;
    UINT8                   thermalthrottlingsupport;
    UINT8                   DimmTempStatValue;
    UINT8                   mcODTOverride;
    UINT8                   advancedClkTraining;
    UINT8                   cmdNormalize;
    UINT8                   advancedStrobeCentering;
    UINT8                   extDimmVrefCircuit;
    UINT8                   earlyCmdTraining;
    UINT8                   earlyCtlTraining;
    UINT8                   txEqCalibration;
    UINT8                   iModeTraining;
    UINT8                   RoundTripLatency;
    UINT8                   DutyCycleTraining;
    UINT8                   XMPProfilesSup;
    UINT8                   XMPMode;
    UINT8                   tCAS;
    UINT8                   tRP;
    UINT8                   tRCD;
    UINT8                   tRAS;
    UINT8                   tWR;
    UINT16                  tRFC;
    UINT8                   tRRD;
    UINT8                   tRTP;
    UINT8                   tWTR;
    UINT8                   tFAW;
    UINT8                   tCWL;
    UINT8                   tRC;
    UINT8                   commandTiming;
    UINT16                  tREFI;
    UINT8                   DdrFreqLimit;
    UINT8                   Vdd;
    UINT8                   lrdimmRankMult;
    UINT8                   lrdimmModuleDelay;
    UINT8                   rcompType;
    UINT8                   PerBitMargins;
    UINT32                  rmtPatternLength;
    UINT8                   RxSenseAmp;
    UINT8                   ADRCh1;
    UINT8                   ADRCh2;
    UINT8                   ADRCh3;
    UINT8                   check_pm_sts;
    UINT8                   check_platform_detect;
#ifdef MEM_NVDIMM_EN
    UINT8                   eraseArmNVDIMMS;
    UINT8                   restoreNVDIMMS;
#endif
#ifdef NVMEM_FEATURE_EN
    UINT8                   interNVDIMMS;
#endif
    UINT8                   ddrCycling;
    UINT8                   perfStats;
    UINT8                   MemoryLaiProbesPresent;
    UINT8                   RxOffCalibration;
    UINT8                   idlePageRstVal;
    UINT8                   serialDebugMsgLvlTrainResults;
    UINT8                   eyeDiagram;
    UINT8                   cmdClkTraining;
    UINT8                   numVrefPoints;
    UINT8                   vrefStepSize;
    UINT8                   compOptions;
    UINT8                   zqOptions;
    UINT8                   MemPwrSave;
    UINT8                   ElectricalThrottlingMode;
    UINT16                  ChPpdsIdleTimer;
    UINT8                   pdwnIdleCntr;
    UINT8                   MultiThreaded;
    UINT8                   promoteMrcWarnings;
    UINT8                   promoteWarnings;
    UINT8                   oppReadInWmm;
    UINT16                  normOppInterval;
    UINT16                  numSparingTransactions;
    UINT8                   PSMISupport;
    UINT8                   sck0ch0;
    UINT8                   sck0ch1;
    UINT8                   sck0ch2;
    UINT8                   sck0ch3;
    UINT8                   sck1ch0;
    UINT8                   sck1ch1;
    UINT8                   sck1ch2;
    UINT8                   sck1ch3;
    UINT8                   sck2ch0;
    UINT8                   sck2ch1;
    UINT8                   sck2ch2;
    UINT8                   sck2ch3;
    UINT8                   sck3ch0;
    UINT8                   sck3ch1;
    UINT8                   sck3ch2;
    UINT8                   sck3ch3;
    UINT8                   a7Mode;
    UINT8                   dddcSupport;
    UINT8                   mdllSden;
    UINT8                   memhotSupport;
    UINT8                   ADREn;
    UINT8                   DdrVrPhaseShedding;
    UINT8                   RankMargin;
    UINT8                   rankMaskEn;
    UINT8                   enableSparing;
    UINT8                   multiSparingRanks;
    UINT8                   svCh0SpareRank;
    UINT8                   svCh1SpareRank;
    UINT8                   svCh2SpareRank;
    UINT8                   svCh3SpareRank;
    UINT8                   lockstepEnableX4;
    UINT8                   caParity;
    UINT8                   smbSpeed;
    UINT8                   perBitDeskew;
    UINT8                   EnforcePOR;
    UINT8                   pda;
    UINT8                   turnaroundOpt;
    UINT16                  IotMemBufferRsvtn;
    UINT8                   DramRaplInit;
    UINT8                   BwLimitTfOvrd;
    UINT8                   perbitmargin;
    UINT8                   DramRaplExtendedRange;
    UINT8                   logParsing;
#ifdef DE_SKU
    UINT8                   BlockScTrafficOnAdr;
#endif
//} MEMORY_CONFIG;


