/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2011 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  HardErrorDetection.c

Abstract:

  Implements Hard Error Detection functionality

--*/
	
#ifndef GRANGEVILLE_FLAG

#include <Protocol/MemRasProtocol.h>
#include <Protocol/SmmErrorLogProtocol.h>
#include "MemRas.h"
#include "HardErrorDetection.h"

#define PERIODIC_TIMER_CALL_COUNT 3
EFI_SMM_ERROR_LOG_PROTOCOL     *mSmmErrorLogProtocol;

#ifndef EFI_DEADLOOP 
 #define EFI_DEADLOOP()    { volatile int __iii; __iii = 1; while (__iii); }
#endif

static
MEM_RAS_EVENT_HNDLR mHEDRasEventHndlrTable[] = {
    HandleRankSparing,
    HandleDevTagging,
    HEDClearUnhandledCorrError,
    END_OF_TABLE
};

extern EFI_SMM_SYSTEM_TABLE2         *mSmst;
EFI_IIO_UDS_PROTOCOL  *mIioUds;

extern struct  sysHost        mHost;
TIME_STAMP                    mCorrErrRankTimeStamp[MC_MAX_NODE][MAX_CH][MAX_RANK_CH];
EFI_HANDLE                    mPeriodicTimerHandle=NULL;
UINT8                         mtotalCSMI=0;
UINT32                        mthreshold, mduration;

UINT8                         mErrNode, mErrDdrCh, mErrRank;
BOOLEAN                       PeriodicTimer=FALSE;
BOOLEAN                       mTransientError=FALSE;
BOOLEAN                       mNoStorm=FALSE;
//BOOLEAN                       mStorm=FALSE;
UINT32                        *mHardErrorDetectionTimestamp;
UINT32                        mInitTimeStamp;
TIME_STAMP                    mInitTime;
static UINT8                  mClearErrorCount=PERIODIC_TIMER_CALL_COUNT;

extern SYSTEM_CONFIGURATION  mSystemConfiguration;


HED_EMCA_MCBANK_DESC *gMcBankList = NULL;

HED_EMCA_MCBANK_DESC gMcBankListIvtEx[] = { 
  { MSR_MC00_CTL2},
  { MSR_MC01_CTL2},
  { MSR_MC02_CTL2},
  { MSR_MC03_CTL2},
  { MSR_MC04_CTL2},
  { MSR_MC05_CTL2},
  { MSR_MC06_CTL2},
  { MSR_MC07_CTL2},
  { MSR_MC08_CTL2},
  { MSR_MC09_CTL2},
  { MSR_MC10_CTL2},
  { MSR_MC11_CTL2},
  { MSR_MC12_CTL2},
  { MSR_MC13_CTL2},
  { MSR_MC14_CTL2},
  { MSR_MC15_CTL2},
  { MSR_MC16_CTL2},
  { MSR_MC17_CTL2},
  { MSR_MC18_CTL2},
  { MSR_MC19_CTL2},
  { MSR_MC20_CTL2},
  { MSR_MC21_CTL2},
  { MSR_MC22_CTL2},
  { MSR_MC23_CTL2},
  { MSR_MC24_CTL2},
  { MSR_MC25_CTL2},
  { MSR_MC26_CTL2},
  { MSR_MC27_CTL2},
  { MSR_MC28_CTL2},
  { MSR_MC29_CTL2},
  { MSR_MC30_CTL2},
  { MSR_MC31_CTL2}
};

HED_EMCA_MCBANK_DESC gMcBankListHswHCC[] = { 
  { MSR_MC00_CTL2},
  { MSR_MC01_CTL2},
  { MSR_MC02_CTL2},
  { MSR_MC03_CTL2},
  { MSR_MC04_CTL2},
  { MSR_MC05_CTL2},
  { MSR_MC06_CTL2},
  { MSR_MC07_CTL2},
  { MSR_MC08_CTL2},
  { MSR_MC09_CTL2},
  { MSR_MC10_CTL2},
  { MSR_MC11_CTL2},
  { MSR_MC12_CTL2},
  { MSR_MC13_CTL2},
  { MSR_MC14_CTL2},
  { MSR_MC15_CTL2},
  { MSR_MC16_CTL2},
  { MSR_MC17_CTL2},
  { MSR_MC18_CTL2},
  { MSR_MC19_CTL2},
  { MSR_MC20_CTL2},
  { MSR_MC21_CTL2}
};

HED_EMCA_MCBANK_DESC gMcBankListHswMCC[] = { 
  { MSR_MC00_CTL2},
  { MSR_MC01_CTL2},
  { MSR_MC02_CTL2},
  { MSR_MC03_CTL2},
  { MSR_MC04_CTL2},
  { MSR_MC05_CTL2},
  { MSR_MC06_CTL2},
  { MSR_MC07_CTL2},
  { MSR_MC08_CTL2},
  { MSR_MC09_CTL2},
  { MSR_MC10_CTL2},
  { MSR_MC11_CTL2},
  { MSR_MC12_CTL2},
  { MSR_MC13_CTL2},
  { MSR_MC14_CTL2},
  { MSR_MC15_CTL2},
  { MSR_MC16_CTL2},
  { MSR_MC17_CTL2},
  { MSR_MC18_CTL2},
  { MSR_MC19_CTL2},
  { MSR_MC20_CTL2},
  //{ MSR_MC21_CTL2}
};

HED_EMCA_MCBANK_DESC gMcBankListHswLCC[] = { 
  { MSR_MC00_CTL2},
  { MSR_MC01_CTL2},
  { MSR_MC02_CTL2},
  { MSR_MC03_CTL2},
  { MSR_MC04_CTL2},
  { MSR_MC05_CTL2},
  { MSR_MC06_CTL2},
  { MSR_MC07_CTL2},
  //{ MSR_MC08_CTL2},
  { MSR_MC09_CTL2},
  { MSR_MC10_CTL2},
  { MSR_MC11_CTL2},
  { MSR_MC12_CTL2},
  //{ MSR_MC13_CTL2},
  //{ MSR_MC14_CTL2},
  //{ MSR_MC15_CTL2},
  //{ MSR_MC16_CTL2},
  { MSR_MC17_CTL2},
  { MSR_MC18_CTL2},
  { MSR_MC19_CTL2},
  { MSR_MC20_CTL2},
  //{ MSR_MC21_CTL2}
};

HED_EMCA_MCBANK_DESC gMcBankListHswEx[] = { 
  { MSR_MC00_CTL2},
  { MSR_MC01_CTL2},
  { MSR_MC02_CTL2},
  { MSR_MC03_CTL2},
  { MSR_MC04_CTL2},
  { MSR_MC05_CTL2},
  { MSR_MC06_CTL2},
  { MSR_MC07_CTL2},
  { MSR_MC08_CTL2},
  { MSR_MC09_CTL2},
  { MSR_MC10_CTL2},
  { MSR_MC11_CTL2},
  { MSR_MC12_CTL2},
  { MSR_MC13_CTL2},
  { MSR_MC14_CTL2},
  { MSR_MC15_CTL2},
  { MSR_MC16_CTL2},
  { MSR_MC17_CTL2},
  { MSR_MC18_CTL2},
  { MSR_MC19_CTL2},
  { MSR_MC20_CTL2},
  { MSR_MC21_CTL2}
};

typedef struct {
  UINT32 *pTimeStamp;
  UINT8 start;
  UINT8 end;
  UINT8  *ptotal;
} DETECT_HARD_ERROR;

DETECT_HARD_ERROR mDetectHardError[MC_MAX_NODE][MAX_CH][MAX_RANK_CH];


/**
  Get the number of MCBANKS_PER_LP.
  
  MCBANKS_PER_LP = MCBANK_CORE + MCBANKS_UNCORE = MCG_CAP.Count
  Assumtion of implementation is that all sockets are symetric, they all have the same number of MCBanks Per LP
  
  @return MCBANKS_PER_LP


UINT64
GetNumMcBanksPerLp(
  VOID
  )

{
  UINT32 McgCapAddr = 0;
  UINT64 McBanksNum = 0;
  
  McgCapAddr = EFI_IA32_MCG_CAP;
  McBanksNum = AsmReadMsr64(McgCapAddr);
  McBanksNum = BitFieldRead64(McBanksNum,N_MSR_MCG_COUNT_START, N_MSR_MCG_COUNT_STOP);
  return McBanksNum;
}

UINT64
GetEMcaBankSupp(
VOID
)
{
  UINT64 SmmMcgCap;

#ifdef IVT_HOST
  SmmMcgCap = 0;
#else
  SmmMcgCap = AsmReadMsr64(MSR_SMM_MCA_CAP);
#endif
  
  return SmmMcgCap & (~((UINT32)0));
}

UINT32
GetAddrMcBankCtl2
(
  UINT32 McBankIdx
)
{
  ASSERT(gMcBankList != NULL);
  return gMcBankList[McBankIdx].Ctl2Msr;
}
*/

STATIC
EFI_STATUS
ReadMCGCAP(
IN VOID *Buffer
) 
{

 UINT64 *Value;
 Value = (UINT64 *)Buffer;

 if(Value != NULL) {
  *Value = AsmReadMsr64(EFI_IA32_MCG_CAP);
 }
 return EFI_SUCCESS;
}


BOOLEAN
isEMCAGen2Cap(
VOID
)
{
  UINT64 McgCap;
  
  if(ReadMCGCAP(&McgCap) != EFI_SUCCESS) {
    McgCap = 0;
  }
    
  return ((McgCap & BIT25) != 0); 
}

EFI_STATUS
InitMcBankList(
VOID
)
{ UINT8 CpuType = 0;
  UINT8 CpuSubType = 0;
  UINT8 physical_chop = 0;
  UINT32 capid4;
  DEBUG((EFI_D_ERROR, "\n InitMcBankList start \n"));
  capid4 = mCpuCsrAccess->ReadCpuCsr(0, 0, CAPID4_PCU_FUN3_REG);
  DEBUG((EFI_D_ERROR, "\n done cap id \n"));

  CpuType =  mIioUds->IioUdsPtr->SystemStatus.cpuType;
  CpuSubType= mIioUds->IioUdsPtr->SystemStatus.cpuSubType;
  DEBUG((EFI_D_ERROR, "\n done cputype \n"));

  if (CpuType == CPU_HSX)
    physical_chop = (UINT8) ((capid4 >> 6) & 0x3); // physical_chop CAPID [7:6]
  else if (CpuType == CPU_BDX)
    physical_chop = (UINT8) ((capid4 >> 6) & 0x7); // physical_chop CAPID [8:6]
  else
    return EFI_SUCCESS;
  DEBUG((EFI_D_ERROR, "\n done physical chop \n"));

  
  DEBUG((EFI_D_ERROR, "\nEmcaplatformhookslib: initMcBankList\n"));
  DEBUG((EFI_D_ERROR, "\nCpuType = %x, CpuSubType = %x\n", CpuType, CpuSubType));
  switch(CpuType) {
    case CPU_HSX:
    case CPU_BDX:
      switch(CpuSubType) {
        case CPU_HSX_EX:
        //case CPU_BDX_EX:
          DEBUG((EFI_D_ERROR, "\nEmcaplatformhookslib: CPU_HSX_EX, CPU_BDX_EX\n"));
          gMcBankList = &gMcBankListHswEx[0];
          break;          
        case CPU_HSX_1HA:
        //case CPU_BDX_1HA:
          DEBUG((EFI_D_ERROR, "\nEmcaplatformhookslib: CPU_HSX_LCC, CPU_BDX_LCC\n"));
          gMcBankList = &gMcBankListHswLCC[0];
          break;
        case CPU_HSX_2HA:
        //case CPU_BDX_2HA:
          DEBUG((EFI_D_ERROR, "\nphysical_chop = %x\n", physical_chop));
          switch(physical_chop) {
            case HSX_PHY_CHOP_MCC:
            //case BDX_PHY_CHOP_MCC:
              DEBUG((EFI_D_ERROR, "\nEmcaplatformhookslib: CPU_HSX_MCC, CPU_BDX_MCC\n"));
              gMcBankList = &gMcBankListHswMCC[0];
              break;
            case HSX_PHY_CHOP_HCC:
            //case BDX_PHY_CHOP_MCC:
              DEBUG((EFI_D_ERROR, "\nEmcaplatformhookslib: CPU_HSX_HCC, CPU_BDX_HCC\n"));
              gMcBankList = &gMcBankListHswHCC[0];
              break;
            default:
              DEBUG((EFI_D_ERROR, "\nERROR:Unable to determine between MCC and HCC, default to MCC\n"));
              gMcBankList = &gMcBankListHswMCC[0];
              break;
          }
          break;
      }
      break;
    case CPU_IVT:
      break;
  }
  if(gMcBankList == NULL) {
    DEBUG((EFI_D_ERROR, "\nERROR Emcaplatformhookslib:gMcBankList or mHaLookup are NULL\n"));
  }
  return EFI_SUCCESS;
}

VOID
DisableSMIForCorrectedMemoryErrors (VOID) 
{
  EXRAS_CONFIG_HA_CFG_STRUCT      ExRasCfg;
  UINT8                           node;
  UINT8                           socket;
  DEBUG ((DEBUG_ERROR, "DisableSMIForCorrectedMemoryErrors Start \n"));
  for(socket=0; socket < MAX_SOCKET; socket++) {
    //
    // Skip if socket not enabled
    //
    if ((mHost.var.common.socketPresentBitMap & (1 << socket)) == 0) {
      continue;
    }
    for( node = SKTMC_TO_NODE(socket, 0); node <= SKTMC_TO_NODE(socket, (MAX_HA-1)); node++){
      if ( mMemRas->SystemMemInfo->NodeInfo[node].Enabled == 0 ) continue;
      ExRasCfg.Data = mMemRas->MemReadPciCfg(  NODE_TO_SKT(node), NODE_TO_MC(node), EXRAS_CONFIG_HA_CFG_REG);
      DEBUG ((DEBUG_ERROR, "before exras reg value = %lx \n", ExRasCfg.Data));
      ExRasCfg.Bits.cfgmcasmioncorr = 00;
      ExRasCfg.Bits.cfgmcasmionmirrorcorr = 00;
      ExRasCfg.Bits.cfgmcasmionfailover = 00;
      mMemRas->MemWritePciCfg(  NODE_TO_SKT(node), NODE_TO_MC(node), EXRAS_CONFIG_HA_CFG_REG, ExRasCfg.Data);
      DEBUG ((DEBUG_ERROR, "after exras reg value = %lx \n", ExRasCfg.Data));
    }
  }
  DEBUG ((DEBUG_ERROR, "DisableSMIForCorrectedMemoryErrors End \n"));
  return;
}

static
EFI_STATUS
HandleRankSparing(
  IN  UINT8     NodeId,
  IN  EVENT_TYPE  EventType,
  IN OUT BOOLEAN  *IsEventHandled
  )
{
  RAS_DEBUG((1, "[RAS]Handle Rank sparing \n"));
  return mMemRas->CheckAndHandleRankSparing(  NodeId, EventType, IsEventHandled );
}

static
EFI_STATUS
HandleDevTagging(
  IN  UINT8     NodeId,
  IN  EVENT_TYPE  EventType,
  IN OUT BOOLEAN  *IsEventHandled
  )
{
  RAS_DEBUG((1, "[RAS]Handle DevTagging\n"));
  return mMemRas->CheckAndHandleDevTagging(  NodeId, EventType, IsEventHandled );
}

static
EFI_STATUS
HEDClearUnhandledCorrError(
  IN  UINT8     NodeId,
  IN  EVENT_TYPE  EventType,
  IN OUT BOOLEAN  *IsEventHandled
  )
{
      RAS_DEBUG((1, "[RAS]ClearUnhandledCorrError\n"));
      return mMemRas->ClearUnhandledCorrError(NodeId, EventType, IsEventHandled);
}

/*++

Routine Description:
    Initialize all Hard Error Detection  related data structures
    This routine should be called only once during boot

Arguments:

  None

Returns:

    Status

--*/
EFI_STATUS
EFIAPI
InitializeHardErrorDetection(
 )
{
  UINT8        ddrCh, rank, m, nodeId;  
  EFI_STATUS   Status;

  DEBUG ((DEBUG_ERROR, "Initialize_HED start \n"));

  mthreshold = mSystemConfiguration.HardErrorDetection_threshold;
  mduration = mSystemConfiguration.HardErrorDetection_duration;
  // DEBUG ((DEBUG_ERROR, "Threshold = %x \n", mthreshold));
  // DEBUG ((DEBUG_ERROR, "duration = %x \n", mduration));

  for(nodeId = 0; nodeId < MC_MAX_NODE; nodeId++) {
    for(ddrCh = 0;ddrCh < MAX_CH; ddrCh++) {
      for(rank = 0;rank < MAX_RANK_CH; rank++){
        Status = mSmst->SmmAllocatePool (EfiRuntimeServicesData, mthreshold + 1, &mHardErrorDetectionTimestamp);
        ASSERT_EFI_ERROR (Status);
        ZeroMem (mHardErrorDetectionTimestamp, mthreshold+1);
        HedSmmGetTime (&mInitTime);
        mInitTimeStamp = (UINT32)
        ((((((mInitTime.Year - 2000) * 360 + 
        (mInitTime.Month - 1)) * 30 + 
        (mInitTime.Day - 1)) * 24 + 
        mInitTime.Hour) * 60 + 
        mInitTime.Minute) * 60 +
        mInitTime.Second
        );
        mHardErrorDetectionTimestamp[0] = mInitTimeStamp;
        mDetectHardError[nodeId][ddrCh][rank].pTimeStamp=mHardErrorDetectionTimestamp;
        mDetectHardError[nodeId][ddrCh][rank].start=0;
        mDetectHardError[nodeId][ddrCh][rank].end=0;
        // DEBUG ((DEBUG_ERROR, "pTimeStamp[0] = %x \n", mDetectHardError[nodeId][ddrCh][rank].pTimeStamp[0]));

        Status = mSmst->SmmAllocatePool (EfiRuntimeServicesData, mthreshold+1, &mDetectHardError[nodeId][ddrCh][rank].ptotal);
        ASSERT_EFI_ERROR (Status);
        for(m = 0; m <= mthreshold; m++){
        mDetectHardError[nodeId][ddrCh][rank].ptotal[m]=0;
        // DEBUG ((DEBUG_ERROR, "threshold init total[%x] = %x \n", m, mDetectHardError[nodeId][ddrCh][rank].ptotal[m]));
        }
      }
    }
  }

  // Locate the IIO Protocol Interface
  Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid, NULL, &mIioUds);
  ASSERT_EFI_ERROR (Status);

  if(gMcBankList == NULL) {
    InitMcBankList();
  }
  DEBUG ((DEBUG_ERROR, "Initialize_HED end \n"));
  return EFI_SUCCESS;
}

/*++

Routine Description:
    Checks whether Corrected Error Storm occured or not in a particular Node, Channel and Rank.
    This is done using Ring management algorithm for each node, channel and Rank
    On detecting Storm, Disable CSMI and SMI and register a periodic timer to check for transient / hard error. 
    If Not Storm, Just return and do not tirgger any other RAS event

Arguments:
 NIL
 
Returns:
Returns True - CMCI to be suppressed
Returns False - CMCI not to be suppressed

--*/
VOID
CheckForCorrectedErrorStorm(
 UINT8 Node, 
 UINT8 Chn, 
 UINT8 Rank, 
 UINT32 Stamp)
{
  UINT32 CalcDiffStartEnd, i;
  EFI_STATUS Status;
  //DEBUG ((DEBUG_ERROR, "CheckForCorrectedErrorStorm Start \n"));
  //DEBUG ((DEBUG_ERROR, "Stamp = %x \n", Stamp));
  //DEBUG ((DEBUG_ERROR, "NodeId = %x \n", Node));
  //DEBUG ((DEBUG_ERROR, "ddrCh = %x \n", Chn));
  //DEBUG ((DEBUG_ERROR, "rank = %x \n", Rank));
  //DEBUG ((DEBUG_ERROR, "threshold = %x \n", mthreshold));
  //DEBUG ((DEBUG_ERROR, "duration = %x \n", mduration));

  if(mSmmErrorLogProtocol==NULL){
    //DEBUG ((DEBUG_ERROR, "mSmmErrorLogProtocol is NULL \n"));
    //EFI_DEADLOOP();
    Status = mSmst->SmmLocateProtocol (&gEfiSmmErrorLogProtocolGuid, NULL, &mSmmErrorLogProtocol); 
    ASSERT_EFI_ERROR (Status);
    //DEBUG ((DEBUG_ERROR, "status = %x \n", Status));
  }
  mErrNode = Node;
  mErrDdrCh = Chn;
  mErrRank = Rank;

  mDetectHardError[Node][Chn][Rank].end = mDetectHardError[Node][Chn][Rank].end + 1;
  if(mDetectHardError[Node][Chn][Rank].end > mthreshold){
    mDetectHardError[Node][Chn][Rank].end = mDetectHardError[Node][Chn][Rank].end % mthreshold;
    }
  mDetectHardError[Node][Chn][Rank].pTimeStamp[mDetectHardError[Node][Chn][Rank].end] = Stamp;
  mDetectHardError[Node][Chn][Rank].ptotal[mDetectHardError[Node][Chn][Rank].end] = 1;
  //DEBUG ((DEBUG_ERROR, "mDetectHardError[Node][Chn][Rank].start = %x \n", mDetectHardError[Node][Chn][Rank].start));
  //DEBUG ((DEBUG_ERROR, "mDetectHardError[Node][Chn][Rank].end = %x \n", mDetectHardError[Node][Chn][Rank].end));
  //DEBUG ((DEBUG_ERROR, "total[0] = %x \n", mDetectHardError[Node][Chn][Rank].ptotal[0]));
  //DEBUG ((DEBUG_ERROR, "total[1] = %x \n", mDetectHardError[Node][Chn][Rank].ptotal[1]));
  mtotalCSMI = 0;
  if(mDetectHardError[Node][Chn][Rank].start < mDetectHardError[Node][Chn][Rank].end) {
    for (i=mDetectHardError[Node][Chn][Rank].start; i<=mDetectHardError[Node][Chn][Rank].end; i++){
      mtotalCSMI = mtotalCSMI + mDetectHardError[Node][Chn][Rank].ptotal[i];
    }
  }
  if (mDetectHardError[Node][Chn][Rank].start > mDetectHardError[Node][Chn][Rank].end) {
    for (i=mDetectHardError[Node][Chn][Rank].start; i<=mthreshold; i++){
      mtotalCSMI = mtotalCSMI + mDetectHardError[Node][Chn][Rank].ptotal[i];
    }
    for (i=1; i<=mDetectHardError[Node][Chn][Rank].end; i++) {
      mtotalCSMI = mtotalCSMI + mDetectHardError[Node][Chn][Rank].ptotal[i];
    }
  }
  DEBUG ((DEBUG_ERROR, "1 totalCSMI = %d \n",mtotalCSMI));

  CalcDiffStartEnd = mDetectHardError[Node][Chn][Rank].pTimeStamp[mDetectHardError[Node][Chn][Rank].end] - mDetectHardError[Node][Chn][Rank].pTimeStamp[mDetectHardError[Node][Chn][Rank].start];
  if(CalcDiffStartEnd <= mduration){
    if (mtotalCSMI >= mthreshold) {
      mDetectHardError[Node][Chn][Rank].start = 0;
      mDetectHardError[Node][Chn][Rank].end = 0;
      ZeroMem (mHardErrorDetectionTimestamp, mthreshold+1);
      HedSmmGetTime (&mInitTime);
      mInitTimeStamp = (UINT32)
      ((((((mInitTime.Year - 2000) * 360 + 
      (mInitTime.Month - 1)) * 30 + 
      (mInitTime.Day - 1)) * 24 + 
       mInitTime.Hour) * 60 + 
      mInitTime.Minute) * 60 +
      mInitTime.Second
      );
      mHardErrorDetectionTimestamp[0] = mInitTimeStamp;
      mDetectHardError[Node][Chn][Rank].pTimeStamp=mHardErrorDetectionTimestamp;
      PeriodicTimer = TRUE;
      DEBUG ((DEBUG_ERROR, "1 Disable CSMI, start periodic timer \n"));
      //mStorm = TRUE;
      DisableSMIForCorrectedMemoryErrors();
      if(isEMCAGen2Cap() && (mSystemConfiguration.EMcaCSmiEn==1)){
        Status = mSmmErrorLogProtocol->DisableCSMI();
        ASSERT_EFI_ERROR (Status);
        //SuppressCMCI();
      }
      HEDRegisterPeriodicTimer();
      return;
    } else {
      mNoStorm = TRUE;
     // if(isEMCAGen2Cap() && (mSystemConfiguration.EMcaCSmiEn==1)) {
        //suppress CMCI for emca enabled cases
        SuppressCMCI(); // suppress for all cases
     // }
      return;
    }
  } else {
    while(CalcDiffStartEnd > mduration) {
      mtotalCSMI = 0;
      mDetectHardError[Node][Chn][Rank].start = mDetectHardError[Node][Chn][Rank].start + 1;		
      if(mDetectHardError[Node][Chn][Rank].start > mthreshold){
        mDetectHardError[Node][Chn][Rank].start = mDetectHardError[Node][Chn][Rank].start % mthreshold;
        }
      if(mDetectHardError[Node][Chn][Rank].start < mDetectHardError[Node][Chn][Rank].end) {
        for (i=mDetectHardError[Node][Chn][Rank].start; i<=mDetectHardError[Node][Chn][Rank].end; i++){
          mtotalCSMI = mtotalCSMI + mDetectHardError[Node][Chn][Rank].ptotal[i];
        }
      }
      if (mDetectHardError[Node][Chn][Rank].start > mDetectHardError[Node][Chn][Rank].end) {
        for (i=mDetectHardError[Node][Chn][Rank].start; i<=mthreshold; i++){
          mtotalCSMI = mtotalCSMI + mDetectHardError[Node][Chn][Rank].ptotal[i];
        }
        for (i=1; i<=mDetectHardError[Node][Chn][Rank].end; i++) {
          mtotalCSMI = mtotalCSMI + mDetectHardError[Node][Chn][Rank].ptotal[i];
        }
      }
      DEBUG ((DEBUG_ERROR, "2 totalCSMI = %d \n",mtotalCSMI));
      // mDetectHardError[Node][Chn][Rank].end = mDetectHardError[Node][Chn][Rank].end - mDetectHardError[Node][Chn][Rank].start + 1;
      CalcDiffStartEnd = mDetectHardError[Node][Chn][Rank].pTimeStamp[mDetectHardError[Node][Chn][Rank].end] - mDetectHardError[Node][Chn][Rank].pTimeStamp[mDetectHardError[Node][Chn][Rank].start];
      if(CalcDiffStartEnd <= mduration){
        if (mtotalCSMI >= mthreshold) {
          mDetectHardError[Node][Chn][Rank].start = 0;
          mDetectHardError[Node][Chn][Rank].end = 0;
          ZeroMem (mHardErrorDetectionTimestamp, mthreshold+1);
          HedSmmGetTime (&mInitTime);
          mInitTimeStamp = (UINT32)
          ((((((mInitTime.Year - 2000) * 360 + 
          (mInitTime.Month - 1)) * 30 + 
          (mInitTime.Day - 1)) * 24 + 
          mInitTime.Hour) * 60 + 
          mInitTime.Minute) * 60 +
          mInitTime.Second
          );
          mHardErrorDetectionTimestamp[0] = mInitTimeStamp;
          mDetectHardError[Node][Chn][Rank].pTimeStamp=mHardErrorDetectionTimestamp;
          PeriodicTimer = TRUE;
          DEBUG ((DEBUG_ERROR, "2 Disable CSMI, start periodic timer \n"));
          //mStorm = TRUE;
          DisableSMIForCorrectedMemoryErrors();
          if(isEMCAGen2Cap() && (mSystemConfiguration.EMcaCSmiEn==1)){
            Status = mSmmErrorLogProtocol->DisableCSMI();
            ASSERT_EFI_ERROR (Status);
            //SuppressCMCI();
          }
          HEDRegisterPeriodicTimer();
          return;
        } else {
          //suppress CMCI for emca enabled cases
          mNoStorm = TRUE;
        //  if(isEMCAGen2Cap() && (mSystemConfiguration.EMcaCSmiEn==1)) {
            SuppressCMCI(); // Suppress for all cases
        //  }
          return;
        }
      }
    }
  } 
  //DEBUG ((DEBUG_ERROR, "CheckForCorrectedErrorStorm Start \n"));
  return;
}

/*++

Routine Description:
    Suppress CMCI when respective conditions are met

Arguments:
 NIL
 
Returns:
Returns True - CMCI to be suppressed
Returns False - CMCI not to be suppressed

--*/
BOOLEAN
EFIAPI
SuppressCMCI(VOID)
{
BOOLEAN DoSuppressCMCI=FALSE;
 
   if(mTransientError || mNoStorm) {
    DoSuppressCMCI = TRUE;
    }
return DoSuppressCMCI;

}


//
// RTC read functions were copied here since we need to get the time
// in both DXE and runtime code.  The PcRtc driver is not currently a
// dual mode driver, this is more efficient since making PcRtc dual mode
// would unnecessarily bloat the SMM code space.
//

UINT8
RtcRead (
  IN  UINT8 Address
  )
/*++

Routine Description:

  Read data register and return contents.

Arguments:

  Address - Register address to read    

Returns:

  Value of data register contents

--*/
{
  IoWrite8 (PCAT_RTC_ADDRESS_REGISTER, (UINT8) (Address | (UINT8) (IoRead8 (PCAT_RTC_ADDRESS_REGISTER) & 0x80)));
  return IoRead8 (PCAT_RTC_DATA_REGISTER);
}

EFI_STATUS
HedSmmGetTime (
  IN OUT TIME_STAMP *Time
  )
  /*++

Routine Description:
    This function provides the time stamp information.

Arguments:
    time :  

Returns:
    time : 

--*/
{
  EFI_STATUS  status;
  UINT8       data8;

  status        = EFI_SUCCESS;

  data8         = RtcRead (RTC_ADDRESS_YEAR);
  Time->Year    = BCD2DEC (data8);
  data8         = RtcRead (RTC_ADDRESS_MONTH);
  Time->Month   = BCD2DEC (data8);
  data8         = RtcRead (RTC_ADDRESS_DAY_OF_THE_MONTH);
  Time->Day     = BCD2DEC (data8);
  data8         = RtcRead (RTC_ADDRESS_HOURS);
  Time->Hour    = BCD2DEC (data8);
  data8         = RtcRead (RTC_ADDRESS_MINUTES);
  Time->Minute  = BCD2DEC (data8);
  data8         = RtcRead (RTC_ADDRESS_SECONDS);
  Time->Second  = BCD2DEC (data8);

  return status;
}

/*++

Routine Description:
    when hard error detected, handle the error through RAS events in the RasEventHndlrTable

Arguments:
 NIL
 
Returns:
NIL

--*/

VOID
TriggerOtherRasEvents(VOID)
{
  UINT8	    node, socket;
  BOOLEAN 	isEventHandled = FALSE;
  UINT8		rasEventHndlrEntry;
  EFI_STATUS	status;

  for (socket = 0; socket < MAX_SOCKET; socket++) {

    //
    // Ignore disabled nodes
    //
    if ((mHost.var.common.socketPresentBitMap & (1 << socket)) == 0) {
      continue;
    }

    for( node = SKTMC_TO_NODE(socket, 0); node <= SKTMC_TO_NODE(socket, (MAX_HA-1)); node++) {
      if ( mMemRas->SystemMemInfo->NodeInfo[node].Enabled == 0 ) continue;

      isEventHandled = FALSE;
      for( rasEventHndlrEntry = 0; mHEDRasEventHndlrTable[rasEventHndlrEntry] != END_OF_TABLE; rasEventHndlrEntry++ ){
        RAS_DEBUG((4,"	New Events	start!	\n"));
        DEBUG ((DEBUG_ERROR, "New Events	start! \n"));
        status = mHEDRasEventHndlrTable[rasEventHndlrEntry]( node, NEW_EVENTS, &isEventHandled );
        ASSERT(status == EFI_SUCCESS);
        if( isEventHandled == TRUE){
          break;	  // Event handled on this node.
        }
     }
  
  //
  // If an event was handled on this node (isEventHandled == TRUE),
  // loop to check if another event is on this node.
  //
  }
  }
  return;
}

/*++

Routine Description:
    Periodic timer Handler to handle transient / hard error
    If Transient error, re enable SMI & CSMI, unregister the periodic timer, suppress CMCI  and return success
    If Hard Error, unregister the periodic timer, Trigger other RAS events, re enable SMI & CSMI, Trigger CMCI and return 

Arguments:
 NIL
 
Returns:
Returns EFI_SUCCESS on successful execution of the handler

--*/


EFI_STATUS
RasPeriodicTimerSmiHandler(
  VOID
  )
{
  UINT8 NodeId=0, DdrCh=0, Rank=0;
  CORRERRORSTATUS_MCDDC_DP_BDX_STRUCT corrErrStsReg;
  UBOXERRSTS_UBOX_CFG_STRUCT      uboxErrorStatusReg;
  /* To verify Hard error
  CORRERRCNT_0_MCDDC_DP_STRUCT  corrErrCntReg;
  UINT32                        corrErrCntRegBaseAddr = CORRERRCNT_0_MCDDC_DP_REG;
  CORRERRTHRSHLD_0_MCDDC_DP_STRUCT corrErrThReg;*/

  EFI_STATUS  Status;

  if(mSmmErrorLogProtocol==NULL){
    //DEBUG ((DEBUG_ERROR, "mSmmErrorLogProtocol is NULL \n"));
    //EFI_DEADLOOP();
    Status = mSmst->SmmLocateProtocol (&gEfiSmmErrorLogProtocolGuid, NULL, &mSmmErrorLogProtocol); 
    ASSERT_EFI_ERROR (Status);
    //DEBUG ((DEBUG_ERROR, "status = %x \n", Status));
  }

  //DEBUG ((DEBUG_ERROR, "RasPeriodicTimerSmiHandler start \n"));
  //DEBUG ((DEBUG_ERROR, "mClearErrorCount = %x \n", mClearErrorCount));
  NodeId = mErrNode;
  DdrCh = mErrDdrCh;
  Rank = mErrRank;
  //DEBUG ((DEBUG_ERROR, "NodeId = %x \n", NodeId));
  //DEBUG ((DEBUG_ERROR, "DdrCh = %x \n", DdrCh));
  //DEBUG ((DEBUG_ERROR, "Rank = %x \n", Rank));
/*	/////////////////////////////////////////////////////////
	// To verify Hard error
    corrErrCntReg.Data = MemReadPciCfg(  NodeId, DdrCh, corrErrCntRegBaseAddr + (Rank/2) * 4 );
    corrErrThReg.Data = MemReadPciCfg (NodeId, DdrCh, CORRERRTHRSHLD_0_MCDDC_DP_REG + (Rank/2) * 4);
    DEBUG ((DEBUG_ERROR, "corrErrCntReg = %x \n", corrErrCntReg.Data));
    DEBUG ((DEBUG_ERROR, "corrErrThReg = %x \n", corrErrThReg.Data));

    corrErrCntReg.Bits.cor_err_cnt_0 = corrErrThReg.Bits.cor_err_th_0;
    MemWritePciCfg(  NodeId, DdrCh, corrErrCntRegBaseAddr + (Rank/2) * 4, corrErrCntReg.Data );

    corrErrCntReg.Data = MemReadPciCfg(  NodeId, DdrCh, corrErrCntRegBaseAddr + (Rank/2) * 4 );
    DEBUG ((DEBUG_ERROR, "Read back corrErrCntReg = %x \n", corrErrCntReg.Data));
	////////////////////////////////////////////////////////*/
   if(mMemRas->MemReadPciCfg(NodeId, DdrCh, CORRERRORSTATUS_MCDDC_DP_REG ) & (1 <<Rank)) {   	
	mClearErrorCount = mClearErrorCount - 1;
	if(mClearErrorCount ==  0) {
		//hard error
		//DEBUG ((DEBUG_ERROR, "NodeId = %x \n", NodeId));
		//DEBUG ((DEBUG_ERROR, "DdrCh = %x \n", DdrCh));
		//DEBUG ((DEBUG_ERROR, "Rank = %x \n", Rank));
		//hard error
		DEBUG ((DEBUG_ERROR, "Hard Error \n"));
		//unregister periodic SMI
		Status = mSmmPeriodicTimerDispatch->UnRegister(mSmmPeriodicTimerDispatch, mPeriodicTimerHandle);
		mPeriodicTimerHandle  = NULL;
		ASSERT_EFI_ERROR (Status);
		DEBUG ((DEBUG_ERROR, "Unregistering Done \n"));
		TriggerOtherRasEvents();
		DEBUG ((DEBUG_ERROR, "handled RAS events Done \n"));
		// re enable SMI and CSMI
		mSmmErrorLogProtocol->EnableSMIForCorrectedMemoryErrors(NodeId);
		if(isEMCAGen2Cap() && (mSystemConfiguration.EMcaCSmiEn==1)) {
		  Status = mSmmErrorLogProtocol->EnableCSMI();
		  ASSERT_EFI_ERROR (Status);
		  Status = mSmmErrorLogProtocol->PropagateSignals();
		  //DEBUG ((DEBUG_ERROR, "PropagateSignals Done \n"));
		  mClearErrorCount = PERIODIC_TIMER_CALL_COUNT;
	  //trigger CMCI
			}
	    return EFI_SUCCESS;
		} else {
	//
    // Clear error status in CPU for this rank
    //
    ClearErrorsForThisRank (NodeId, DdrCh, Rank);

    //
    // Clear CORRERRORSTATUS, write 1 to clear
    //
    corrErrStsReg.Data = 0;
    corrErrStsReg.Bits.err_overflow_stat = 1 << Rank; // clear only for the rank that is handled
    MemWritePciCfg(  NodeId, DdrCh, CORRERRORSTATUS_MCDDC_DP_REG, corrErrStsReg.Data );

    if(MemReadPciCfg( NodeId, DdrCh, CORRERRORSTATUS_MCDDC_DP_REG ) & (1 <<Rank)){
      corrErrStsReg.Data = 0;
      corrErrStsReg.Bits.err_overflow_stat = 1 << Rank; // clear only for the rank that is handled
      MemWritePciCfg(  NodeId, DdrCh, CORRERRORSTATUS_MCDDC_DP_REG, corrErrStsReg.Data );
    }
    //
    // Clear SMI status
    //
    uboxErrorStatusReg.Data = MemReadPciCfg( NodeToSkt(NodeId), 0, UBOXERRSTS_UBOX_CFG_REG );
    uboxErrorStatusReg.Bits.smi_delivery_valid  = 0;
    uboxErrorStatusReg.Bits.smisrcimc           = 0;
    MemWritePciCfg( NodeToSkt(NodeId), 0, UBOXERRSTS_UBOX_CFG_REG, uboxErrorStatusReg.Data );

	    return EFI_SUCCESS; 
		}
  	
   	} else{
   		mClearErrorCount = mClearErrorCount - 1;
  if(mClearErrorCount==0) {
      // transient error
      mTransientError = TRUE;
      DEBUG ((DEBUG_ERROR, "Transient Error \n"));
      //unregister periodic SMI
      Status = mSmmPeriodicTimerDispatch->UnRegister(mSmmPeriodicTimerDispatch, mPeriodicTimerHandle);
      mPeriodicTimerHandle  = NULL;
      ASSERT_EFI_ERROR (Status);
      DEBUG ((DEBUG_ERROR, "Unregistering Done \n"));
      // re enable SMI and CSMI
      mSmmErrorLogProtocol->EnableSMIForCorrectedMemoryErrors(NodeId);
      if(isEMCAGen2Cap() && (mSystemConfiguration.EMcaCSmiEn==1)) {
        Status = mSmmErrorLogProtocol->EnableCSMI();
        SuppressCMCI();   //suppress CMCI for emca enabled cases
      }
      DEBUG ((DEBUG_ERROR, "Re enabling SMIs done \n"));
      mClearErrorCount = PERIODIC_TIMER_CALL_COUNT;
      return EFI_SUCCESS;	
    } else {  
		//
	    // Clear error status in CPU for this rank
	    //
	    ClearErrorsForThisRank (NodeId, DdrCh, Rank);

	    //
	    // Clear CORRERRORSTATUS, write 1 to clear
	    //
	    corrErrStsReg.Data = 0;
	    corrErrStsReg.Bits.err_overflow_stat = 1 << Rank; // clear only for the rank that is handled
	    MemWritePciCfg(  NodeId, DdrCh, CORRERRORSTATUS_MCDDC_DP_REG, corrErrStsReg.Data );

	    if(MemReadPciCfg( NodeId, DdrCh, CORRERRORSTATUS_MCDDC_DP_REG ) & (1 <<Rank)){
	      corrErrStsReg.Data = 0;
	      corrErrStsReg.Bits.err_overflow_stat = 1 << Rank; // clear only for the rank that is handled
	      MemWritePciCfg(  NodeId, DdrCh, CORRERRORSTATUS_MCDDC_DP_REG, corrErrStsReg.Data );
	    }
	    //
	    // Clear SMI status
	    //
	    uboxErrorStatusReg.Data = MemReadPciCfg( NODE_TO_SKT(NodeId), 0, UBOXERRSTS_UBOX_CFG_REG );
	    uboxErrorStatusReg.Bits.smi_delivery_valid  = 0;
	    uboxErrorStatusReg.Bits.smisrcimc           = 0;
	    MemWritePciCfg( NODE_TO_SKT(NodeId), 0, UBOXERRSTS_UBOX_CFG_REG, uboxErrorStatusReg.Data );
		
	    return EFI_SUCCESS; 
		}
   		}
}

/*++

Routine Description:
    Call back for the periodic timer to call the periodic timer handler 

Arguments:
 NIL
 
Returns:
Returns EFI_SUCCESS on successful execution of the handler

--*/

EFI_STATUS
RasPeriodicChildCallback (
  IN  EFI_HANDLE                  PeriodicTimerHandle,
  IN  CONST VOID     *mPeriodicTimer,
  IN  OUT VOID      *CommBuffer  OPTIONAL,
  IN  OUT UINTN     *CommBufferSize OPTIONAL
  )
{

  // Call the RAS periodic timer handler.
  RasPeriodicTimerSmiHandler();

  return EFI_SUCCESS;
}

/*++

Routine Description:
    Register a periodic timer when Corrected Error Storm is detected

Arguments:
 NIL
 
Returns:
Register the periodic timer as per the period and interval
Asserts when registering the periodic timer fails

--*/

VOID
HEDRegisterPeriodicTimer(
  VOID
  )
{
  EFI_SMM_PERIODIC_TIMER_REGISTER_CONTEXT   PeriodicTimer;
  EFI_STATUS  Status;
  //DEBUG ((DEBUG_ERROR, "HEDRegisterPeriodicTimer start \n"));
  //mFirstTimerEntry  = 01;
  //
  // Setup Periodic SMI.
  //      A child wishes schedule a period SMI to fire on a period of 3 seconds, there
  //      are several ways to approach the problem:
  //      1. The child may accept a 4 second periodic rate, in which case it registers with
  //           Period = 40000
  //           SmiTickInterval = 20000
  //         The resulting SMI will occur every 2 seconds with the child called back on
  //         every 2nd SMI.
  //         NOTE: the same result would occur if the child set SmiTickInterval = 0.
  // Do a every second SMI
  //
  if(!mPeriodicTimerHandle) {
  	//DEBUG ((DEBUG_ERROR, "HEDRegisterPeriodicTimer inside PeriodicTimerHandle \n"));
    PeriodicTimer.Period = 12800000;
    PeriodicTimer.SmiTickInterval = 640000;
    Status = mSmmPeriodicTimerDispatch->Register(mSmmPeriodicTimerDispatch, RasPeriodicChildCallback, &PeriodicTimer, &mPeriodicTimerHandle); 
    ASSERT_EFI_ERROR (Status);
    //DEBUG ((DEBUG_ERROR, "HEDRegisterPeriodicTimer after registering \n"));
  }
}

/*++

Routine Description:
    Check for new Hard Error events and handle them

Arguments:

  NodeId    - Memory controller ID
  IsEventHandled -  ptr to a buffer to hold event handled status

Returns:

    status - EFI_SUCCESS 
 
    IsEventHandled - TRUE -  event was handled
                     FALSE - event not handled

--*/
static
EFI_STATUS
HandleNewHardErrorDetectionEventsIfAny(
       IN UINT8 NodeId,
       OUT BOOLEAN *IsEventHandled
)
{
  UINT8                             ddrCh;
  UINT8                             rank = 0;
  EFI_STATUS                        status = EFI_SUCCESS;
  UINT32  TimeStamp;
  //DEBUG ((DEBUG_ERROR, "HandleNewHardErrorDetectionEventsIfAny start \n"));
  *IsEventHandled       = FALSE;

  for( ddrCh = 0; ddrCh < MAX_CH; ddrCh++ ){
    if (mSystemMemoryMap->NodeInfo[NodeId].ChannelInfo[ddrCh].Enabled == 0) {
      continue;
    }
    //
    // Check if there is any rank in this channel
    // that has overflow bit set
    //
    for( rank = 0; (rank < MAX_RANK_CH) ; rank++ ){
      if( IsErrorExcdForThisRank( NodeId, ddrCh, rank) ){
        break;
       } 
    }
    if( rank != MAX_RANK_CH ){
      //
      // Generate a time stamp of the seconds from 2000/1/1, assume 30day/month.
      //    
      HedSmmGetTime (&mCorrErrRankTimeStamp[NodeId][ddrCh][rank]);
      TimeStamp = (UINT32)
      (
      (((((mCorrErrRankTimeStamp[NodeId][ddrCh][rank].Year - 2000) * 360 + 
      (mCorrErrRankTimeStamp[NodeId][ddrCh][rank].Month - 1)) * 30 + 
      (mCorrErrRankTimeStamp[NodeId][ddrCh][rank].Day - 1)) * 24 + 
      mCorrErrRankTimeStamp[NodeId][ddrCh][rank].Hour) * 60 + 
      mCorrErrRankTimeStamp[NodeId][ddrCh][rank].Minute) * 60 +
      mCorrErrRankTimeStamp[NodeId][ddrCh][rank].Second
      );
      //DEBUG ((DEBUG_ERROR, "Stamp = %x \n", TimeStamp));
      //DEBUG ((DEBUG_ERROR, "NodeId = %x \n", NodeId));
      //DEBUG ((DEBUG_ERROR, "ddrCh = %x \n", ddrCh));
      //DEBUG ((DEBUG_ERROR, "rank = %x \n", rank));
      CheckForCorrectedErrorStorm(NodeId, ddrCh, rank, TimeStamp);
      *IsEventHandled = TRUE;
      break;
    }
  }
  //DEBUG ((DEBUG_ERROR, "HandleNewHardErrorDetectionEventsIfAny End \n"));

return status;
}



/*++

Routine Description:
    Check for Hard Error events and handle them

Arguments:

  NodeId    - Memory controller ID
  EventType - NEW_EVENTS or EVENTS_IN_PROGRESS

Returns:

    status - EFI_SUCCESS if no failures
             non-zero error code for failures

    IsEventHandled - TRUE -  event was handled
                     FALSE - event not handled

--*/
EFI_STATUS
EFIAPI
CheckAndHandleHardErrorDetection(
    IN UINT8 NodeId,
    IN EVENT_TYPE EventType,
    OUT BOOLEAN *IsEventHandled
 )
{
  EFI_STATUS status=EFI_SUCCESS;
  //DEBUG ((DEBUG_ERROR, "HardErrorDetection start \n"));
  //DEBUG ((DEBUG_ERROR, "ExRasModesEnabled = %x \n", mSystemMemoryMap->NodeInfo[NodeId].ExRasModesEnabled));
  
//  if(mSystemMemoryMap->NodeInfo[NodeId].ExRasModesEnabled & HARDERRORDETECTION_EN){
  if(mSystemConfiguration.HandleHardErrorDetection) {
    switch( EventType ){
    case EVENTS_IN_PROGRESS:
      //
      // events 'IN PROGRESS'
      // indicate that event as not handled always
      //
      status          = EFI_SUCCESS;
      *IsEventHandled = FALSE;
      break;

    case NEW_EVENTS:
      status = HandleNewHardErrorDetectionEventsIfAny( NodeId, IsEventHandled );
      break;

    default:
      status = EFI_UNSUPPORTED;     // Invalid event type
      break;
    } 

 } else {
    //
    // If Hard Error Detection is not enabled, do nothing
    //
    *IsEventHandled = FALSE;
    status          = EFI_SUCCESS;
  }
  //DEBUG ((DEBUG_ERROR, "HardErrorDetection End \n"));
  return status;
}

#endif // GRANGEVILLE_FLAG