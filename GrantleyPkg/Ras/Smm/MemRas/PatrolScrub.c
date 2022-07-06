/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PatrolScrub.c

Abstract:

  Patrol scrub support

--*/
#include <Token.h> // AptioV server override
#include "PatrolScrub.h"
#include "RankSparing.h"
#include "SparingEngine.h"
#include "DramDeviceCorrection.h"


typedef struct {
  SCRUBCTL_MC_MAINEXT_STRUCT           ScrubCtlReg;
  SCRUBADDRESSLO_MC_MAINEXT_HSX_BDX_STRUCT  ScrubAddrLoReg;
  SCRUBADDRESSHI_MC_MAINEXT_STRUCT     ScrubAddrHiReg;
  SCRUBCTL_MC_MAINEXT_STRUCT           ScrubCtlRegSave;
  UINT8                                ReEnableFlag;
  SSRSTATETYPE                         SSRState;  
  UINT8                                ScrubSpareCh;
  UINT8                                ScrubSpareRank;
  SCRUBMASK_MC_MAINEXT_HSX_BDX_STRUCT  ScrubMaskReg;
} PATROL_SCRUB_STATUS_STRUC;

static
PATROL_SCRUB_STATUS_STRUC mPatrolScrubStatus[MC_MAX_NODE];

static
UINT8 mPatrolScrubFlag;

#define POISON_SCRUB_INTERVAL         0x3e8   // TODO: set this to max possible fastest scrub interval



static AMAP_MCDDC_DESC_STRUCT   mAMAPArray[MC_MAX_NODE][MAX_CH];
static DIMMMTR_0_MCDDC_DESC_BDX_STRUCT  mDIMMMTR0[MC_MAX_NODE][MAX_CH];
static DIMMMTR_1_MCDDC_DESC_BDX_STRUCT  mDIMMMTR1[MC_MAX_NODE][MAX_CH];
static DIMMMTR_2_MCDDC_DESC_BDX_STRUCT  mDIMMMTR2[MC_MAX_NODE][MAX_CH];

static UINT16 mWAState[MC_MAX_NODE];
extern EFI_SMM_SYSTEM_TABLE2         *mSmst;



#ifndef GRANGEVILLE_FLAG
#endif // GRANGEVILLE_FLAG

BOOLEAN
EFIAPI
CheckPatrolScrubEnable( 
)
/*++
	
Routine Description:
 Check if patrol scrub is enabled or not.

Arguments:
none

Returns:
	 
TRUE -- patrol scrub is enabled.
FALSE -- patrol scrub is disabled.

--*/
{
    if(mPatrolScrubFlag){
#ifndef GRANGEVILLE_FLAG        
#endif // GRANGEVILLE_FLAG
      return TRUE;
    }
    else{
      return FALSE;
    }
}




EFI_STATUS
EFIAPI 
ReEnablePatrolScrubEngine(
    IN UINT8 NodeId
 )
/*++

Routine Description:
  Enable Patrol scrub engine to start from the last Address
  it had scrubbed

Arguments:
  NodeId - Memory controller ID

Returns:

  EFI_SUCCESS - always

--*/ 
{
//  if( mPatrolScrubStatus[NodeId].ReEnableFlag == 1) {
    //
    // Patrol scrub was enabled before. Re-enable it now and restore old register values
    //
    MemWritePciCfg( NodeId, PARAM_UNUSED, SCRUBADDRESSLO_MC_MAINEXT_REG, mPatrolScrubStatus[NodeId].ScrubAddrLoReg.Data);
    MemWritePciCfg( NodeId, PARAM_UNUSED, SCRUBADDRESSHI_MC_MAINEXT_REG, mPatrolScrubStatus[NodeId].ScrubAddrHiReg.Data);	

    MemWritePciCfg( NodeId, PARAM_UNUSED, SCRUBCTL_MC_MAINEXT_REG, mPatrolScrubStatus[NodeId].ScrubCtlReg.Data);

    RAS_DEBUG((4, "ReEnablePatrolScrubEngine Mc:%d  value:%x \n",NodeId, mPatrolScrubStatus[NodeId].ScrubCtlReg.Data));
//  }
  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI 
DisablePatrolScrubEngine(
    IN UINT8 NodeId
 )
/*++

Routine Description:
  Disable Patrol scrub engine and save patrol registers

Arguments:
  NodeId - Memory controller ID

Returns:

  EFI_SUCCESS - Always

--*/ 
{
  SCRUBCTL_MC_MAINEXT_STRUCT      ScrubCtlReg;

  ScrubCtlReg.Data = MemReadPciCfg( NodeId, PARAM_UNUSED, SCRUBCTL_MC_MAINEXT_REG);
  if( ScrubCtlReg.Bits.scrub_en == 1) {
    RAS_DEBUG((4, "DisablePatrolScrubEngine : node = %x \n", NodeId));
    //
    // Save registers and disable scrubbing
    //
    mPatrolScrubStatus[NodeId].ScrubCtlReg.Data = ScrubCtlReg.Data;
    ScrubCtlReg.Bits.scrub_en = 0;
    MemWritePciCfg( NodeId, PARAM_UNUSED, SCRUBCTL_MC_MAINEXT_REG, ScrubCtlReg.Data);

    mPatrolScrubStatus[NodeId].ScrubAddrLoReg.Data = MemReadPciCfg( NodeId, PARAM_UNUSED, SCRUBADDRESSLO_MC_MAINEXT_REG);
    mPatrolScrubStatus[NodeId].ScrubAddrHiReg.Data = MemReadPciCfg( NodeId, PARAM_UNUSED, SCRUBADDRESSHI_MC_MAINEXT_REG);

    //
    // Set flag to indicate patrol scrub should be re-enabled
    //
    mPatrolScrubStatus[NodeId].ReEnableFlag = 1;
  }

  return EFI_SUCCESS;
}

#ifndef GRANGEVILLE_FLAG
static
VOID
EFIAPI
UpdateAMAP(
    IN  UINT8 NodeId,
    IN  UINT8 Ch
)
/*++

Routine Description:
    Update AMAP register before scrubbing non-spare ranks or 
    after doing spare copy operation.
    AMAP register is from spare rank and failed rank which is map out current address map.

Arguments:
    NodeId  -- MC id
    Ch -- specified channel.
  Returns:
    None    

--*/
{
  UINT32                     DimmAmap;
  UINT8                      i;   
  UINT8                      physicalRank;
  UINT8                      Dimm;
  UINT8                      RankPerDimm;  
  AMAP_MCDDC_DESC_STRUCT     AMAPReg;   

  DimmAmap = 0;

  for(i = 0;  i < MAX_RANK_CH; i++){
    //
    //scan spare rank array 
    //
    if(CheckRankState(NodeId, Ch, i, SPARERANKSTATUS_AVAILABLE)){
      physicalRank = GetLogicalToPhysicalRankId(NodeId,Ch,i);
      Dimm = physicalRank /4;
      RankPerDimm  = physicalRank % 4;
      DimmAmap |= (1 << (Dimm * 8 + RankPerDimm)); 
    }

    //
    //scan spare failed rank.
    //
    if(GetRankState(NodeId,Ch,i).Bits.rank_exist  &&
       GetRankState(NodeId,Ch,i).Bits.spare_rank  &&
       GetRankState(NodeId,Ch,i).Bits.rank_fail ) {
         physicalRank = GetLogicalToPhysicalRankId(NodeId,Ch,i);
         Dimm = physicalRank /4;
         RankPerDimm  = physicalRank % 4;
         DimmAmap |= (1 << (Dimm * 8 + RankPerDimm)); 
     }

     //
     //scan non-spare failed rank which has done spare copy.
     //
     if(GetRankState(NodeId,Ch,i).Bits.rank_exist   &&
        GetRankState(NodeId,Ch,i).Bits.non_spare_rank  &&
       (GetRankState(NodeId,Ch,i).Bits.rank_fail )) {
          if(CheckRankState(NodeId, Ch, i,NONSPARESTATUS_SPARECOPYDONE)){
            physicalRank = GetLogicalToPhysicalRankId(NodeId,Ch,i);
            Dimm = physicalRank /4;
            RankPerDimm  = physicalRank % 4;
            DimmAmap |= (1 << (Dimm * 8 + RankPerDimm)); 
          }
       }

     //
     //scan disabled rank which done in MRC.
     //
     if((GetRankState(NodeId,Ch,i).Bits.rank_exist   &&
        GetRankState(NodeId,Ch,i).Bits.rank_disable  )) {

          physicalRank = GetLogicalToPhysicalRankId(NodeId,Ch,i);
          Dimm = physicalRank /4;
          RankPerDimm  = physicalRank % 4;
          DimmAmap |= (1 << (Dimm * 8 + RankPerDimm)); 

       }
  }//for

  //
  // Update AMAP to enable patrolling any rank
  //
  AMAPReg.Data= MemReadPciCfg(NodeId, Ch, AMAP_MCDDC_DESC_REG);
  RAS_DEBUG((4, "UpdateAMAP -  before AMAP value Channel:%d  reg:%lx   OrData:%lx\n", Ch,AMAPReg.Data, DimmAmap));
   
  AMAPReg.Bits.dimm0_pat_rank_disable = (UINT8)(DimmAmap & 0xff);
  AMAPReg.Bits.dimm1_pat_rank_disable = (UINT8)((DimmAmap >> 8) & 0xff);
  AMAPReg.Bits.dimm2_pat_rank_disable = (UINT8)((DimmAmap >> 16) & 0xff);
  MemWritePciCfg(NodeId, Ch, AMAP_MCDDC_DESC_REG,AMAPReg.Data);
   
  if(mSystemMemoryMap->NodeInfo[NodeId].RasModesEnabled & CH_LOCKSTEP)	{
	  MemWritePciCfg(NodeId, (Ch^01), AMAP_MCDDC_DESC_REG, AMAPReg.Data);
	  RAS_DEBUG((4, "UpdateAMAP -  Lockstep Channel:%d  AmapRegData:%x \n", Ch^1,AMAPReg.Data));
  }
  RAS_DEBUG((4, "UpdateAMAP -  current AMAP value Channel:%d  reg:%lx\n", Ch,AMAPReg.Data));
  
}


EFI_STATUS
ReEnableSSR(
    IN UINT8   NodeId,
    IN BOOLEAN CheckSSRStateFlag
)
/*++

Routine Description:
  re-enable Spare Scrub engine when scrub spare rank or do spare copying is finished.

Arguments:
  NodeId - Memory controller ID
  CheckSSRStateFlag - TRUE -- check SSR state; FALSE -- not check SSR state.
  
Returns:

  EFI_SUCCESS - Always

--*/
{
  UINT8                               NON_SPARECh;
  UINT8                               NON_SPARERank;
  MCMAIN_CHKN_BITS_MC_MAIN_STRUCT     mcMainChknReg;

  //
  //this is for there is other rank is doing scrubbing or do spare copy.
  //
  if(CheckSSRStateFlag){
    if(mPatrolScrubStatus[NodeId].SSRState != SCRUBBING_NON_SPARERANK){
      return EFI_SUCCESS;
    }
  }

  DisablePatrolScrubEngine(NodeId);
  
  //
  //update AMAP
  //
  UpdateAMAP( NodeId, mPatrolScrubStatus[NodeId].ScrubSpareCh);

  //
  //stop MCMAIN_CHKN_BITS_MC_MAIN_REG
  //
  mcMainChknReg.Data = MemReadPciCfg(NodeId, PARAM_UNUSED, MCMAIN_CHKN_BITS_MC_MAIN_REG);
  if(mcMainChknReg.Bits.defeature_12  == 1){
    mcMainChknReg.Bits.defeature_12 = 0;      // disable patrol to stop after rank is scrubbed
    mcMainChknReg.Bits.dis_ptrl_err_log = 0;  // Disable patrol from logging errors when scrubbing spare rank
    MemWritePciCfg(NodeId, PARAM_UNUSED, MCMAIN_CHKN_BITS_MC_MAIN_REG, mcMainChknReg.Data);  
  }
  

  //
  //get next non-spare rank index
  //
  GetNextAvailableNonSprRankForPatrol(NodeId,  &NON_SPARECh, &NON_SPARERank);

  //
  // Change patrol scrub address to scrub specified spare rank
  //
  mPatrolScrubStatus[NodeId].ScrubAddrHiReg.Data= MemReadPciCfg(NodeId, PARAM_UNUSED, SCRUBADDRESSHI_MC_MAINEXT_REG);
  mPatrolScrubStatus[NodeId].ScrubAddrHiReg.Bits.chnl = NON_SPARECh;
  mPatrolScrubStatus[NodeId].ScrubAddrHiReg.Bits.rank = GetLogicalToPhysicalRankId(NodeId,NON_SPARECh,NON_SPARERank);	 
  mPatrolScrubStatus[NodeId].ScrubAddrLoReg.Data = 0;	 

  mPatrolScrubStatus[NodeId].ScrubCtlReg.Data = mPatrolScrubStatus[NodeId].ScrubCtlRegSave.Data;
  mPatrolScrubStatus[NodeId].ScrubCtlReg.Bits.scrub_en = 1;
  mPatrolScrubStatus[NodeId].ScrubCtlReg.Bits.startscrub = 1;
  mPatrolScrubStatus[NodeId].ScrubCtlReg.Bits.stop_on_cmpl = 0;	   
  mPatrolScrubStatus[NodeId].ScrubCtlReg.Bits.stop_on_err = 0; 	 
  ReEnablePatrolScrubEngine(NodeId);

  //
  //update SSRState.
  //
  mPatrolScrubStatus[NodeId].SSRState = SCRUBBING_NON_SPARERANK;  
                        
  return EFI_SUCCESS;
}



static
VOID
GetNextAvailableNonSprRankForPatrol(
    IN  UINT8  NodeId,
    OUT UINT8  *DdrCh,
    OUT UINT8  *LogicalRank
 )
 /**

Routine Description:
  Get Next available non-Spare rank to scrub

Arguments:
    NodeId
    DdrCh - ptr that holds current ddr ch
    Logical Rank - ptr that holds current rank

  Note:
  If *DDRCH >= MAX_CH and *Logical rank >= MAX_RANK_CH, then this routine will
  return the first available ch/rank

Returns:
   None

--*/
{
  UINT8   ch;
  UINT8   rank;
  SPARECTL_MC_MAINEXT_STRUCT        spareCtlReg;
  SSRSTATUS_MC_MAINEXT_STRUCT       ssrStatusReg;
  BOOLEAN rankFound = FALSE;

  ssrStatusReg.Data = MemReadPciCfg( NodeId, PARAM_UNUSED, SSRSTATUS_MC_MAINEXT_REG);
  spareCtlReg.Data = MemReadPciCfg( NodeId, PARAM_UNUSED, SPARECTL_MC_MAINEXT_REG);

  for(ch = 0; (ch < MAX_CH) && (rankFound == FALSE); ch++){
    //
    // Skip channels that are not present
    //
    if( !mSystemMemoryMap->NodeInfo[NodeId].ChannelInfo[ch].Enabled ){
      continue;
    }
    
    //
    // the following data is used to re-enable the patrol scrub to start scrubbing from dst_rank(spare rank) after sparing complete.
    //
    RAS_DEBUG((4, "GetNextSprRankForPatrol -  enabled logical ranks:%x\n", mSystemMemoryMap->NodeInfo[NodeId].ChannelInfo[ch].EnabledLogicalRanks));
    for(rank = 0; rank < MAX_RANK_CH; rank++){
      //
      // Skip ranks that are not enabled
      //      
      if(!(mSystemMemoryMap->NodeInfo[NodeId].ChannelInfo[ch].EnabledLogicalRanks & (1 << rank))){
        continue;
      }

      //
      // Skip spare ranks
      //
      if(CheckRankState(NodeId,ch,rank, SPARERANKSTATUS_AVAILABLE))continue;

      //
      //Skip the non-spare rank which is map out address.
      //
      if(CheckRankState(NodeId,ch,rank, NONSPARESTATUS_SPARECOPYDONE))continue;   

  
  
      //
      //skip BIST failed rank.
      //
      if((GetRankState(NodeId,ch,rank).Bits.rank_exist   &&
        GetRankState(NodeId,ch,rank).Bits.rank_disable  ))	  continue;
  


      //
      // We found a rank, update the return data
      //
     if( (ssrStatusReg.Bits.sprinprogress == 0) && (ssrStatusReg.Bits.sprcmplt == 1) && (spareCtlReg.Bits.spare_enable == 0) && 
	      (spareCtlReg.Bits.src_rank  ==  rank ) && (spareCtlReg.Bits.chn_sel == ch ) )	{
	      
        RAS_DEBUG((4, "GetNextSprRankForPatrol -  failed_rank  ==  selected rank : %x \n", rank));

        spareCtlReg.Data   = MemReadPciCfg( NodeId, PARAM_UNUSED, SPARECTL_MC_MAINEXT_REG);
        *DdrCh             = (UINT8)spareCtlReg.Bits.chn_sel;
        *LogicalRank       = (UINT8)spareCtlReg.Bits.dst_rank;
        RAS_DEBUG((2, "GetNextSprRankForPatrol -  channel : %x,  spareCtlReg.dst_rank(logical) : %x\n", spareCtlReg.Bits.chn_sel,spareCtlReg.Bits.dst_rank));	      
	      return;
      }  else {
        *DdrCh        = ch;
        *LogicalRank  = rank;
        rankFound     = TRUE;
        RAS_DEBUG((2, "GetNextAvailableNonSprRankForPatrol -  non-spare ranks channel:%d,  rank:%d\n", ch,rank));	  
        break;
      }
    } // rank loop
  } // ch loop
}

static
VOID
EFIAPI
DoRankScrubbing(
  IN UINT8 NodeId,
  IN UINT8 Ch,
  IN UINT8 Rank,
  IN UINT8 ScrubSource  
)
/*++

Routine Description:
  do scrubbing on spare rank.

Arguments:
  NodeId  - Memory controller ID
  Ch - Failed Channel number
  Rank - Failed rank number
  ScrubSource -NON_SPARE_CE,SPARE_UC,SPARE_CE
Returns:

  None

--*/
{      
  UINT8                               NextSpareRank;
  UINT32                              SparePhysicalRank;
  UINT32                              SpareDimm;
  UINT32                              SpareRankPerDimm;  
  MCMAIN_CHKN_BITS_MC_MAIN_STRUCT     mcMainChknReg;
  AMAP_MCDDC_DESC_STRUCT              AMAPReg;
  //
  //save spare scrub info.
  //
  mPatrolScrubStatus[NodeId].SSRState = SCRUBBING_SPARERANK;
  mPatrolScrubStatus[NodeId].ScrubSpareCh = Ch;
  GetLowestPrioritySpareRank(NodeId,Ch,&NextSpareRank);
  

  mPatrolScrubStatus[NodeId].ScrubSpareRank = NextSpareRank;

  //
  //update the source CE state.
  //
  SetRankState(NodeId,Ch,NextSpareRank,SPARERANKSTATUS_SCRUBBING);
  if((ScrubSource == NON_SPARE_CE) || ((ScrubSource == NON_SPARE_CE_EVENT))){
    SetRankState(NodeId,Ch, Rank, NONSPARESTATUS_SCRUBBING);
  }


  //
  //stop scrubbing.
  //
  DisablePatrolScrubEngine(NodeId);

  //
  // if Spare rank available, then patrol spare rank
  // update ScrubSpareRankDdrCh to indicate the current DDR CH that is
  // patrolled
  //
  mcMainChknReg.Data = MemReadPciCfg(NodeId, PARAM_UNUSED, MCMAIN_CHKN_BITS_MC_MAIN_REG);
  mcMainChknReg.Bits.defeature_12 = 1;      // Enable patrol to stop after rank is scrubbed
  mcMainChknReg.Bits.dis_ptrl_err_log = 1;  // Disable patrol from MCA logging errors when scrubbing spare rank
  MemWritePciCfg(NodeId, PARAM_UNUSED, MCMAIN_CHKN_BITS_MC_MAIN_REG, mcMainChknReg.Data);

  //
  // Update AMAP to disable all other ranks except the specific rank.
  //
  AMAPReg.Data= MemReadPciCfg(NodeId, Ch, AMAP_MCDDC_DESC_REG);
  RAS_DEBUG((4, "UpdateAMAP for scrubbing -   AMAP value Channel:%d  read value %lx\n", Ch,AMAPReg.Data));
  AMAPReg.Bits.dimm0_pat_rank_disable = 0x0f;
  AMAPReg.Bits.dimm1_pat_rank_disable = 0x0f;
  AMAPReg.Bits.dimm2_pat_rank_disable = 0x0f;
  SparePhysicalRank =   GetLogicalToPhysicalRankId(NodeId,Ch,NextSpareRank);
  SpareDimm = SparePhysicalRank/4;
  SpareRankPerDimm = SparePhysicalRank % 4;

  if(SpareDimm == 0){
    AMAPReg.Bits.dimm0_pat_rank_disable  &= (~(1 << SpareRankPerDimm));  
  }
  
  if(SpareDimm == 1){
    AMAPReg.Bits.dimm1_pat_rank_disable  &= (~(1 << SpareRankPerDimm));  
  }
 
  if(SpareDimm == 2){
    AMAPReg.Bits.dimm2_pat_rank_disable  &= (~(1 << SpareRankPerDimm));  
  }      

  RAS_DEBUG((4, "UpdateAMAP for scrubbing -   AMAP value Channel:%d  Write value:%lx\n", Ch,AMAPReg.Data));
    MemWritePciCfg(NodeId, Ch, AMAP_MCDDC_DESC_REG,AMAPReg.Data);   

  // Disable Pkg C6 before Scrubbing spare rank
  DisablePkgC6(NodeId);

  //
  // Change patrol scrub address to scrub specified spare rank
  //       
  mPatrolScrubStatus[NodeId].ScrubAddrHiReg.Data=  MemReadPciCfg(NodeId, PARAM_UNUSED, SCRUBADDRESSHI_MC_MAINEXT_REG);
  mPatrolScrubStatus[NodeId].ScrubAddrHiReg.Bits.chnl = Ch;
  mPatrolScrubStatus[NodeId].ScrubAddrHiReg.Bits.rank = GetLogicalToPhysicalRankId(NodeId,Ch,NextSpareRank); 
  mPatrolScrubStatus[NodeId].ScrubAddrLoReg.Data = 0;

  mPatrolScrubStatus[NodeId].ScrubCtlRegSave.Data = MemReadPciCfg(NodeId, PARAM_UNUSED, SCRUBCTL_MC_MAINEXT_REG);
  mPatrolScrubStatus[NodeId].ScrubCtlReg.Data = mPatrolScrubStatus[NodeId].ScrubCtlRegSave.Data;
  mPatrolScrubStatus[NodeId].ScrubCtlReg.Bits.scrub_en = 1;
  mPatrolScrubStatus[NodeId].ScrubCtlReg.Bits.startscrub = 1;  
  mPatrolScrubStatus[NodeId].ScrubCtlReg.Bits.scrubinterval = POISON_SCRUB_INTERVAL;
  mPatrolScrubStatus[NodeId].ScrubCtlReg.Bits.stop_on_cmpl = 1; 
  mPatrolScrubStatus[NodeId].ScrubCtlReg.Bits.stop_on_err = 1;
  ReEnablePatrolScrubEngine(NodeId);

  
  RAS_DEBUG((1, "[RAS scrubbing end] Scrubbing spare rank  Mc:%d  Ch:%d  Rank:%d \n", NodeId, Ch,NextSpareRank));
  


}


VOID
EFIAPI
ScrubNextRank(
    IN UINT8 NodeId,
    IN UINT8 Ch,
    IN UINT8 Rank,
    IN UINT8 ScrubSource
)
/*++

Routine Description:
  Handles patrol scrub one rank by one rank for specifid node and chanel. Assume there must be an available spare rank exist on this channel.
In case ScrubSource is NON_SPARE_CE, it is CE happen on non-spare rank, it need scrub spare rank first, in case SSR is busy with sparecopying or scrub spare rank,
log an event.
In case ScrubSource is SPARE_UC, or SPARE_CE, it scrub spare rank, no need check SSR status for it must be scrubbing spare rank. 
Invoker make sure this channel has available spare rank exist.

Arguments:
  NodeId  - Memory controller ID
  Ch - Failed Channel number
  Rank - Failed rank number
  ScrubSource -NON_SPARE_CE,SPARE_UC,SPARE_CE
Returns:

  None

--*/
{
  UINT8                               FoundedCh;
  UINT8                               FoundedPendingRank;
  UINT8                               SourceRank;

  if(!CheckRankStatePerCh(NodeId,Ch, SPARERANKSTATUS_AVAILABLE, NULL)){
  
    //
    //note: for NON_SPARE_CE source, it can't go here, at invoker, it will check available spare rank.
    //     
    RAS_DEBUG((1," [RAS scrubbing] No available spare rank for Ch:%d\n",Ch));
    
    //
    //for user case: if scrubing spare rank CE or UC, but no available spare rank exist,
    //start SSR whatever and enable pkgc6 states
    // 
    EnablePkgC6(NodeId);
    ReEnableSSR(NodeId,FALSE);
 
    //
    //update the non spare rank which state is NONSPARESTATUS_SCRUBNEXT.
    //
    if(ScrubSource == NON_SPARE_CE_EVENT){
      SetRankState(NodeId, Ch,Rank, NONSPARESTATUS_TRY_TAG);
   
      RAS_DEBUG((1," [RAS scrubbing] start device tag Mc:%d  Ch:%d  Rank:%d\n",
        NodeId,Ch,Rank));   

      HandleDevTagForPendingEvent(NodeId,Ch,Rank); 
     }
 
    if((ScrubSource == SPARE_UC) ||(ScrubSource == SPARE_CE))  { 
      if(CheckRankStatePerCh(NodeId, Ch, NONSPARESTATUS_SCRUBNEXT, &SourceRank)){
        SetRankState(NodeId, Ch,SourceRank, NONSPARESTATUS_TRY_TAG);
        if(mSystemMemoryMap->NodeInfo[NodeId].RasModesEnabled & CH_LOCKSTEP)	{
          SetRankState(NodeId, Ch ^ 01,SourceRank, NONSPARESTATUS_TRY_TAG);
        }
        RAS_DEBUG((1," [RAS scrubbing] start device tag Mc:%d  Ch:%d  Rank:%d\n",
           NodeId,Ch,SourceRank));   

        HandleDevTagForPendingEvent(NodeId,Ch,SourceRank);
      }else{
        RAS_DEBUG((1," [RAS scrubbing] error! No found pending source rank\n"));
        return; 
      }
    }
    
    //
    //check if any left event here.
    //
    if(CheckRankStatePerMC(NodeId, NONSPARESTATUS_SCRUBNEXT, &FoundedCh, &FoundedPendingRank)){
      RAS_DEBUG((1, "[RAS scurb next] there is a pending CE event exist  Ch:%d   Rank:%d\n",FoundedCh,FoundedPendingRank));	   
      //   
      //check if available rank exist in this MC.   
      //
      ScrubNextRank(NodeId,FoundedCh,FoundedPendingRank,NON_SPARE_CE_EVENT); 

    }
    
    return; 
  }


  if((ScrubSource == SPARE_CE) || (ScrubSource == SPARE_UC) )  {
    if(mPatrolScrubStatus[NodeId].SSRState != SCRUBBING_SPARERANK){
       RAS_DEBUG((1,"[RAS scrubbing] SSR state:%d is wrong during scrub next rank 0\n",mPatrolScrubStatus[NodeId].SSRState ));
       return;
    }
    
    if((Ch != mPatrolScrubStatus[NodeId].ScrubSpareCh) || (Rank != mPatrolScrubStatus[NodeId].ScrubSpareRank) ){
       RAS_DEBUG((1,"[RAS scrubbing] SSR state not match CE/US passdown\n"));
       return;
    }
       RAS_DEBUG((1, "[RAS scrubbing]  spare rank CE/UC  Mc:%d  Ch:%d  Rank:%d \n", NodeId, Ch,Rank));
  }

  if(ScrubSource == NON_SPARE_CE_EVENT){  
     RAS_DEBUG((1, "[RAS scrubbing]  non-spare rank event  CE Mc:%d  Ch:%d  Rank:%d \n", NodeId, Ch,Rank));
  }
  
  if(ScrubSource == NON_SPARE_CE){
     //
     //in case multi overflow happen on one MC, and if SSR is busy with scrubbing or spare copy, 
     //just go out,  at  spare copy end, it will re-enter.
     //
     if(mPatrolScrubStatus[NodeId].SSRState != SCRUBBING_NON_SPARERANK){
        RAS_DEBUG((1, "[RAS scrubbing]  SSR is busy, go out.   Mc:%d  Ch:%d  Rank:%d  wait for SSR state:%d \n", 
           NodeId, Ch,Rank,mPatrolScrubStatus[NodeId].SSRState));
        return;
     }
  
     RAS_DEBUG((1, "[RAS scrubbing]  non-spare rank CE Mc:%d  Ch:%d  Rank:%d \n", NodeId, Ch,Rank));
  }

  DoRankScrubbing(NodeId,Ch,Rank,ScrubSource);

  return;  
}
#endif  // GRANGEVILLE_FLAG

static
EFI_STATUS
HandlePatrolCompleteEventForNode(
    IN  UINT8 NodeId
 )
 /*++

Routine Description:
  Handles patrol complete event for a node

Arguments:
  NodeId  - Memory controller ID

Returns:

  EFI_SUCCESS / Error code

--*/
{
  EFI_STATUS                          status = EFI_SUCCESS;
#ifndef GRANGEVILLE_FLAG  
  UINT8                               SourceRank;
  UINT8                               DestRank;
  UINT8                               Ch = mPatrolScrubStatus[NodeId].ScrubSpareCh;
  UINT32                              errStatus;
#endif  // GRANGEVILLE_FLAG
  SCRUBCTL_MC_MAINEXT_STRUCT          ScrubCtlReg;
  MCMAIN_CHKN_BITS_MC_MAIN_STRUCT     mcMainChknReg;

  RAS_DEBUG((1, "[RAS patrol complete]HandlePatrolCompleteEventForNode - Enter for Node[%d]\n", NodeId));

  //
  //get failed rank info
  //
#ifndef GRANGEVILLE_FLAG  
  if(!CheckRankStatePerCh(NodeId, Ch,NONSPARESTATUS_SCRUBBING,&SourceRank)){
       RAS_DEBUG((1, "[RAS patrol complete] Error! didn't find the scrubbing non spare rank:%d\n"));  	
  }
  RAS_DEBUG((1, "[RAS patrol complete] CE failed rank:%d\n", SourceRank));  
  //
  //get scrubbed rank info
  //
  CheckRankStatePerCh(NodeId, Ch,SPARERANKSTATUS_SCRUBBING,&DestRank);  
  if( mPatrolScrubStatus[NodeId].ScrubSpareRank != DestRank){
    RAS_DEBUG((1,"ERROR! spare desti rank is not equal to SSR rank\n"));
  }
  RAS_DEBUG((1, "[RAS patrol complete] patrol scrub complete rank:%d\n", DestRank)); 
  
  //
  //update rank info
  //
  SetRankState(NodeId,Ch,DestRank,SPARERANKSTATUS_INPROGRESS);
  SetRankState(NodeId,Ch, SourceRank, NONSPARESTATUS_DOSPARECOPY);
  if(mSystemMemoryMap->NodeInfo[NodeId].RasModesEnabled & CH_LOCKSTEP)	{
    SetRankState(NodeId,Ch^1,DestRank,SPARERANKSTATUS_INPROGRESS);
    SetRankState(NodeId,Ch^1, SourceRank, NONSPARESTATUS_DOSPARECOPY); 
  }
#endif // GRANGEVILLE_FLAG
  
  //
  //clear patrol scrub stop_on_cpml status first, and disable scrub.
  //update scrub info and restore other register before do spare copying.
  //
  ScrubCtlReg.Data = MemReadPciCfg(NodeId, PARAM_UNUSED, SCRUBCTL_MC_MAINEXT_REG);
  ScrubCtlReg.Bits.ptl_cmpl = 0;
  ScrubCtlReg.Bits.stop_on_cmpl  = 0;
  ScrubCtlReg.Bits.stop_on_err = 0;
  ScrubCtlReg.Bits.scrub_en  = 0;   
  MemWritePciCfg(NodeId, PARAM_UNUSED, SCRUBCTL_MC_MAINEXT_REG,ScrubCtlReg.Data);
  
  

  mcMainChknReg.Data = MemReadPciCfg(NodeId, PARAM_UNUSED, MCMAIN_CHKN_BITS_MC_MAIN_REG);
  mcMainChknReg.Bits.defeature_12 = 0;      // Disable patrol to stop after rank is scrubbed
  mcMainChknReg.Bits.dis_ptrl_err_log =0;  // enable patrol from MCA logging errors when scrubbing spare rank
  MemWritePciCfg(NodeId, PARAM_UNUSED, MCMAIN_CHKN_BITS_MC_MAIN_REG, mcMainChknReg.Data); 
  
#ifndef GRANGEVILLE_FLAG
  //
  //Enable Pkgc6 after scrubbing the spare rank
  //
   EnablePkgC6(NodeId); 
  //
  //do spare copy operation
  //
  RAS_DEBUG((4, "[RAS patrol complete] do spare copying\n"));    

  if(AcquireSparingEngine(NodeId) == EFI_SUCCESS){
     StartSparing(NodeId,SourceRank,DestRank,Ch,RankSparing);
  }
  
  //
  //update progress info
  //
  GetOrSetErrorHandlingStatus(NodeId, GetEventPendingStatus, &errStatus);
  if(!errStatus) {
     errStatus = 1;
     GetOrSetErrorHandlingStatus(NodeId, UpdateEventPendingStatus, &errStatus); // Save the error bitmap for internal use
  }
  errStatus = 1;
  GetOrSetErrorHandlingStatus(  NodeId, UpdateEventProgressStatus, &errStatus );
#endif // GRANGEVILLE_FLAG
  RAS_DEBUG((1, "[RAS patrol complete] end!\n"));      
  return status;
}



static
EFI_STATUS
HandlePatrolUCEventForNode(
    IN  UINT8 NodeId
)
/*++

Routine Description:
  Handles patrol UC event for a node

Arguments:
  NodeId  - Memory controller ID

Returns:

  EFI_SUCCESS / Error code

--*/
{
  EFI_STATUS Status =EFI_SUCCESS;
  RAS_DEBUG((1, "[RAS patrol UC]  this is an spare rank UC\n"));

#ifndef GRANGEVILLE_FLAG
  if(mPatrolScrubStatus[NodeId].SSRState == SCRUBBING_SPARERANK){  
     //
     //mark this rank as "dirty"
     //
     MarkRankDirtyFlag(NodeId,mPatrolScrubStatus[NodeId].ScrubSpareCh,mPatrolScrubStatus[NodeId].ScrubSpareRank);

     //
     //move to next rank scrub......
     //
     ScrubNextRank(NodeId,mPatrolScrubStatus[NodeId].ScrubSpareCh,mPatrolScrubStatus[NodeId].ScrubSpareRank,SPARE_UC); 
  }
#endif // GRANGEVILLE_FLAG
  return Status;	 
}   



EFI_STATUS
EFIAPI
CheckAndHandlePatrolEvent(
    IN EFI_HANDLE        DispatchHandle,
    IN CONST VOID        *DispatchContext, // optional
    IN OUT   VOID        *CommBuffer,      // optional
    IN OUT   UINTN       *CommBufferSize   // optional
)
/*++

Routine Description:
  Checks if patrol has completed on any node and handles it

Arguments:
  DispatchHandle - Dispatch handle (unused for now!)
  Dispatch Context - Context information (unused for now!)
  CommBuffer     - Buffer used for communication between caller/callback function (unused for now!)
  CommBufferSize - size of communication buffer (unused for now!)

Returns:

  EFI_SUCCESS / Error code

--*/
{
  UINT8                       node;
  EFI_STATUS                  status = EFI_SUCCESS;
  SCRUBCTL_MC_MAINEXT_STRUCT  scrubCtlReg;


  for(node = 0; node < MC_MAX_NODE; node++){
    if(!mSystemMemoryMap->NodeInfo[node].Enabled){
      continue;
    }
  
    scrubCtlReg.Data = MemReadPciCfg(node, PARAM_UNUSED, SCRUBCTL_MC_MAINEXT_REG);
    if(scrubCtlReg.Bits.ptl_cmpl){
      RAS_DEBUG((1, "[RAS patrol]Patrol non_spare memory complete event start! \n"));
      HandlePatrolCompleteEventForNode(node);
    }

    if(scrubCtlReg.Bits.ptl_stopped){
      RAS_DEBUG((1, "[RAS patrol]Patrol scrub spare rank UC event start \n")); 
      HandlePatrolUCEventForNode(node);
    }    


  } // for node

  return status;
}


EFI_STATUS
InstallPatrolSmiHandler(
    VOID
 )
 /*++

Routine Description:
  Install patrol SMI handler if required

Arguments:
  None

Returns:

  EFI_SUCCESS / Error code

--*/
{
  UINT8       rankSpareEnabled;
  EFI_STATUS  status = EFI_SUCCESS;
  EFI_HANDLE  imageHandle;
  
  //
  //each node has the same value of RasModesEnabled
  //Judge whole system RAS configuration here.
  //Judge each node if it support spare mode later.
  //
  rankSpareEnabled = mSystemMemoryMap->NodeInfo[0].RasModesEnabled & RK_SPARE;
  
  //
  //check if support patrol scrub.
  //
  mPatrolScrubFlag = mSystemMemoryMap->NodeInfo[0].ExRasModesEnabled & PTRLSCRB_EN;


  //
  if(rankSpareEnabled && CheckPatrolScrubEnable()) {
    //
    // We need to handle patrol  SMI event, install handler
    //
    imageHandle = NULL;
    status = gSmst->SmiHandlerRegister (
                        CheckAndHandlePatrolEvent,
                        NULL,
                        &imageHandle
                        );   

   RAS_DEBUG((1, "[RAS Enable] Patrol scrub SMI handler installed\n"));
  }

  return status;
}


/**

Routine Description:
   Get Physical to Logical rank ID

Arguments:
    NodeId
    DdrCh
    Physical Rank

Returns:
  Logical Rank Id

--*/
UINT8
GetPhysicalToLogicalRankId(
    IN UINT8  NodeId,
    IN UINT8  DdrCh,
    IN UINT8  PhysicalRank
 )
{
  UINT8   LogicalRank = 0xFF;

  if( mMemRas->SystemMemInfo->NodeInfo[NodeId].ChannelInfo[DdrCh].MaxDimm == 3){
    if (PhysicalRank >= 8){
        LogicalRank = PhysicalRank - 2;
    }
    else{
      LogicalRank = PhysicalRank;
    }
  } else {
    LogicalRank = PhysicalRank;
  }
  return LogicalRank;
}

/**
Routine Description:
  Get Next available channel to scrub

Arguments:
    NodeId
    DdrCh - ptr that holds current ddr ch

Returns:
    DdrCh

--*/
static
VOID 
GetNextAvailableChannel(
    IN UINT8    NodeId,
    IN UINT8    *DdrCh,
    IN BOOLEAN  ChannelRollover
)
{
  UINT8   ch;
  UINT8   counter;
  BOOLEAN NextChannelFound = FALSE;

  if (((*DdrCh) < 0) || ((*DdrCh) >= MAX_CH)){
    (*DdrCh) = 0xFF;
     return;
  }

    ch = (*DdrCh) + 1;
    for(counter = 0; (counter < MAX_CH); counter++, ch++){
      //
      // When we move to next DDR CH; if we get past the last DDRCH,
      // we will rollover if ChannelRollover = TRUE
      //
      if(ch >= MAX_CH){
        if (ChannelRollover == TRUE){
            ch=0;
        }    
        else{
            break;
        }    
      }
      //
      // Skip channels that are not populated
      //
      if( !mSystemMemoryMap->NodeInfo[NodeId].ChannelInfo[ch].Enabled ){
        continue;
      } else {
          // Check whether the channel is already masked for scrubbing
          if (mPatrolScrubStatus[NodeId].ScrubMaskReg.Bits.ch_mask & (1 << ch))
            continue;
          else {
            NextChannelFound = TRUE;            
            break;
          }
      }
    }
  if (NextChannelFound){
    *DdrCh = ch;
  } 
  else{
    *DdrCh = 0xFF;  
  }
}



/**
Routine Description:
  Get Next available Rank to scrub

Arguments:
    NodeId
    DdrCh - ptr that holds current ddr ch
    DdrRank - ptr that holds current ddr rank

Returns:
    Next DdrCh and DdrRank

    Note : The function will not rollover to Rank zero after reaching last
              rank.
--*/
static
VOID 
GetNextAvailableRank(
    IN UINT8  NodeId,
    IN UINT8  *Channel,
    IN OUT UINT8  *DdrRank,
    IN BOOLEAN ChannelRollOver
)
{
  UINT8   Rank;
  UINT8   counter;
  UINT8   RankMask = 0;
  BOOLEAN NextRankFound = FALSE;
  UINT8   LogicalRank, NextLogicalRank;
  AMAP_MCDDC_DESC_STRUCT  amapReg;

  switch (*Channel){
    case 0:
      RankMask = (UINT8)mPatrolScrubStatus[NodeId].ScrubMaskReg.Bits.ch0_rank;
      break;
    case 1:
      RankMask = (UINT8)mPatrolScrubStatus[NodeId].ScrubMaskReg.Bits.ch1_rank;
      break;
    case 2:
      RankMask = (UINT8)mPatrolScrubStatus[NodeId].ScrubMaskReg.Bits.ch2_rank;
      break;
    case 3:
      RankMask = (UINT8)mPatrolScrubStatus[NodeId].ScrubMaskReg.Bits.ch3_rank;
      break;
  }

  Rank = (*DdrRank) + 1;
  if(mPatrolScrubStatus[NodeId].ScrubMaskReg.Bits.ch_mask & (1 << (*Channel))) {
    GetNextAvailableChannel(NodeId, Channel, ChannelRollOver);
    if((*Channel) == 0xFF) {
      (*DdrRank) = 0xFF;
      return;
      } else {
        Rank = 0;
    }
  }
  LogicalRank = GetPhysicalToLogicalRankId(NodeId, (*Channel), Rank);
  NextLogicalRank = LogicalRank;
  for(counter = 0; (counter < MAX_RANK_CH); counter++, NextLogicalRank++){
    if(NextLogicalRank >= MAX_RANK_CH){
      GetNextAvailableChannel(NodeId, Channel, ChannelRollOver);
      if ((*Channel) == 0xFF){
        break;
      }
      NextLogicalRank = 0;
      counter = 0;
      switch (*Channel){
        case 0:
          RankMask = (UINT8)mPatrolScrubStatus[NodeId].ScrubMaskReg.Bits.ch0_rank;
          break;
        case 1:
          RankMask = (UINT8)mPatrolScrubStatus[NodeId].ScrubMaskReg.Bits.ch1_rank;
          break;
        case 2:
          RankMask = (UINT8)mPatrolScrubStatus[NodeId].ScrubMaskReg.Bits.ch2_rank;
          break;
        case 3:
          RankMask = (UINT8)mPatrolScrubStatus[NodeId].ScrubMaskReg.Bits.ch3_rank;
          break;
      }        
    }
    //
    // Skip ranks that are not populated
    //
    if (!(mSystemMemoryMap->NodeInfo[NodeId].ChannelInfo[(*Channel)].EnabledLogicalRanks & (1 << NextLogicalRank))){
      continue;
      } else {
          // Check whether the channel is already masked for scrubbing
        if ((NextLogicalRank == RankMask) && (mPatrolScrubStatus[NodeId].ScrubMaskReg.Bits.all_ranks & ( 1 << (*Channel)))){
          continue;
        }
        //Read the AMAP register and update the rank
        amapReg.Data = MemReadPciCfg(NodeId, *Channel, AMAP_MCDDC_DESC_REG);
        // dimm0_pat_rank_disable - Bits[18:15];	dimm1_pat_rank_disable - Bits[22:19]; dimm2_pat_rank_disable - Bits[26:23];
        // Get the logical rank index through (GetLogicalToPhysicalRankId(NodeId, *Channel, NextLogicalRank) + 15)
        if((amapReg.Data) & ( 1 << (GetLogicalToPhysicalRankId(NodeId, *Channel, NextLogicalRank) + 15)))
          continue;
          else {  
            NextRankFound = TRUE;
            break;
          }  
        }
    }
  if (NextRankFound){
     *DdrRank = GetLogicalToPhysicalRankId(NodeId, (*Channel), NextLogicalRank);
  }
  else{
    *DdrRank = 0xFF; 
    (*Channel) = 0xFF;
  }
}

/*++

Routine Description:
  Handles Asymmetric DIMM patrol event for a node based on the workaround variable
  When asymmetric DIMMs are populated, patrol scrubber should scrub ranks based on AMAP programmed.

Arguments:
  NodeId  - Memory controller ID

Returns:

  EFI_SUCCESS

--*/

static
EFI_STATUS
HandleAsymmetricDIMMPatrolEvent(
    IN  UINT8 nodeId
)
{
  UINT8   ch;
  UINT8   rank;
  UINT8   dimm;
  EFI_STATUS   Status = EFI_SUCCESS;
  UINT8   no_of_halfsize_ranks =0;
  UINT8   no_of_quartersize_ranks = 0;
  UINT8   ScrubRank;
  UINT8   ScrubChannel;
  AMAP_MCDDC_DESC_STRUCT AMAP_Value[MAX_CH];
  DIMMMTR_0_MCDDC_DESC_BDX_STRUCT DIMMMTR0_Value[MAX_CH];
  DIMMMTR_1_MCDDC_DESC_BDX_STRUCT DIMMMTR1_Value[MAX_CH];
  DIMMMTR_2_MCDDC_DESC_BDX_STRUCT DIMMMTR2_Value[MAX_CH];

  DisablePatrolScrubEngine(nodeId);
  mPatrolScrubStatus[nodeId].ScrubMaskReg.Data = MemReadPciCfg( nodeId, PARAM_UNUSED, SCRUBMASK_MC_MAINEXT_REG);
  DEBUG ((DEBUG_ERROR, "\t mWAState[%x] = %x \n", nodeId, mWAState[nodeId]));
  switch(mWAState[nodeId]) {
    case FULL_SIZE_RANK_CHECK:
      //half size rank check
      for(ch = 0; ch < MAX_CH; ch++) {
        if(mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].Enabled == 0) continue;
        for(dimm = 0; dimm < MAX_DIMM ; dimm ++){
          //
          // Skip dimms that are not present
          //
          for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
            if ((mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].DimmInfo[dimm].Present) && (mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].DimmInfo[dimm].mapOut[rank] == 1) ||
               ((mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].DimmInfo[dimm].SPDSecondaryDeviceType &  SPD_HALF_SIZE_SECOND_RANK) && ((rank) % 2))) {
              AMAP_Value[ch].Data = MemReadPciCfg(nodeId, ch, AMAP_MCDDC_DESC_REG);
              // map out all full sized ranks and map 1/2 size ranks		
              AMAP_Value[ch].Bits.dimm0_pat_rank_disable =  0x05; //0x06EE8001; //Assuming rank0 has full rank size, rank1 has half size
              AMAP_Value[ch].Bits.dimm1_pat_rank_disable =  0x05;
              AMAP_Value[ch].Bits.dimm2_pat_rank_disable =  0x05;
              MemWritePciCfg(nodeId, ch, AMAP_MCDDC_DESC_REG, AMAP_Value[ch].Data);

              DIMMMTR0_Value[ch].Data = MemReadPciCfg(nodeId, ch, DIMMMTR_0_MCDDC_DESC_REG);
              // set ra_width to map 1/2 size
              DIMMMTR0_Value[ch].Bits.ra_width = DIMMMTR0_Value[ch].Bits.ra_width - 1;
              MemWritePciCfg(nodeId, ch, DIMMMTR_0_MCDDC_DESC_REG, DIMMMTR0_Value[ch].Data);

              DIMMMTR1_Value[ch].Data = MemReadPciCfg(nodeId, ch, DIMMMTR_1_MCDDC_DESC_REG);
              // set ra_width to map 1/2 size
              DIMMMTR1_Value[ch].Bits.ra_width = DIMMMTR1_Value[ch].Bits.ra_width - 1;
              MemWritePciCfg(nodeId, ch, DIMMMTR_1_MCDDC_DESC_REG, DIMMMTR1_Value[ch].Data);

              DIMMMTR2_Value[ch].Data = MemReadPciCfg(nodeId, ch, DIMMMTR_2_MCDDC_DESC_REG);
              // set ra_width to map 1/2 size
              DIMMMTR2_Value[ch].Bits.ra_width = DIMMMTR2_Value[ch].Bits.ra_width - 1;
              MemWritePciCfg(nodeId, ch, DIMMMTR_2_MCDDC_DESC_REG, DIMMMTR2_Value[ch].Data);
              no_of_halfsize_ranks = no_of_halfsize_ranks + 1;
            }
          }		
        }
      }
     
      // If no 1/2 size ranks exist, go to the Quarter_size_rank_check
      if(no_of_halfsize_ranks != 0) {
        mWAState[nodeId] = HALF_SIZE_RANK_CHECK;
        break;
      }
    case HALF_SIZE_RANK_CHECK:
      // Quarter size rank check
      for(ch = 0; ch < MAX_CH; ch++) {
        if(mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].Enabled == 0) continue;
        for(dimm = 0; dimm < MAX_DIMM ; dimm ++){
          //
          // Skip dimms that are not present
          //
          for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
            if ((mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].DimmInfo[dimm].Present) && (mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].DimmInfo[dimm].mapOut[rank] == 1) ||
               ((mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].DimmInfo[dimm].SPDSecondaryDeviceType &  SPD_QUARTER_SIZE_SECOND_RANK) && ((rank) % 2))) {
              AMAP_Value[ch].Data = MemReadPciCfg(nodeId, ch, AMAP_MCDDC_DESC_REG);
              // map out all full sized ranks and map 1/4 size ranks
              AMAP_Value[ch].Bits.dimm0_pat_rank_disable =  0x05; //0x06EE8001; //Assuming rank0 has full rank size, rank1 has quarter size
              AMAP_Value[ch].Bits.dimm1_pat_rank_disable =  0x05;
              AMAP_Value[ch].Bits.dimm2_pat_rank_disable =  0x05; 
              MemWritePciCfg(nodeId, ch, AMAP_MCDDC_DESC_REG, AMAP_Value[ch].Data);

              DIMMMTR0_Value[ch].Data = mDIMMMTR0[nodeId][ch].Data;
              // set ra_width to map 1/4 size
              DIMMMTR0_Value[ch].Bits.ra_width = DIMMMTR0_Value[ch].Bits.ra_width - 2;
              MemWritePciCfg(nodeId, ch, DIMMMTR_0_MCDDC_DESC_REG, DIMMMTR0_Value[ch].Data);

              DIMMMTR1_Value[ch].Data = mDIMMMTR1[nodeId][ch].Data;
              // set ra_width to map 1/4 size
              DIMMMTR1_Value[ch].Bits.ra_width = DIMMMTR1_Value[ch].Bits.ra_width - 2;
              MemWritePciCfg(nodeId, ch, DIMMMTR_1_MCDDC_DESC_REG, DIMMMTR1_Value[ch].Data);

              DIMMMTR2_Value[ch].Data = mDIMMMTR2[nodeId][ch].Data;
              // set ra_width to map 1/4 size
              DIMMMTR2_Value[ch].Bits.ra_width = DIMMMTR2_Value[ch].Bits.ra_width - 2;
              MemWritePciCfg(nodeId, ch, DIMMMTR_2_MCDDC_DESC_REG, DIMMMTR2_Value[ch].Data);
              no_of_quartersize_ranks = no_of_quartersize_ranks+1;
              }
            }
          }
        }
      // If no 1/4 size ranks exist, go to the Full_size_rank_check
      if(no_of_quartersize_ranks != 0) {
        mWAState[nodeId] = QUARTER_SIZE_RANK_CHECK;      
        break;
        }
    case QUARTER_SIZE_RANK_CHECK:
      // Full size rank check
      for(ch = 0; ch < MAX_CH; ch++) {
        if(mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].Enabled == 0) continue;

        MemWritePciCfg(nodeId, ch, AMAP_MCDDC_DESC_REG, mAMAPArray[nodeId][ch].Data);
        MemWritePciCfg(nodeId, ch, DIMMMTR_0_MCDDC_DESC_REG, mDIMMMTR0[nodeId][ch].Data);
        MemWritePciCfg(nodeId, ch, DIMMMTR_1_MCDDC_DESC_REG, mDIMMMTR1[nodeId][ch].Data);
        MemWritePciCfg(nodeId, ch, DIMMMTR_2_MCDDC_DESC_REG, mDIMMMTR2[nodeId][ch].Data);
        }
      mWAState[nodeId] = FULL_SIZE_RANK_CHECK;
      break;
  }

  ScrubRank = (UINT8)mPatrolScrubStatus[nodeId].ScrubAddrHiReg.Bits.rank;
  DEBUG ((DEBUG_ERROR, "\t rank last scrubbed = %x \n", ScrubRank ));

  ScrubChannel = (UINT8)mPatrolScrubStatus[nodeId].ScrubAddrHiReg.Bits.chnl;
  DEBUG ((DEBUG_ERROR, "\t channel last scrubbed = %x \n", ScrubChannel ));

  GetNextAvailableRank(nodeId, &ScrubChannel, &ScrubRank, TRUE);
  DEBUG ((DEBUG_ERROR, "\t rank_tobe_updated = %x \n", ScrubRank ));
  DEBUG ((DEBUG_ERROR, "\t chnl to be updated = %x \n", ScrubChannel));

  //
  // Change patrol scrub address to scrub specified spare rank
  //
  mPatrolScrubStatus[nodeId].ScrubAddrHiReg.Data= MemReadPciCfg(nodeId, PARAM_UNUSED, SCRUBADDRESSHI_MC_MAINEXT_REG);
  mPatrolScrubStatus[nodeId].ScrubAddrHiReg.Bits.chnl = ScrubChannel;
  mPatrolScrubStatus[nodeId].ScrubAddrHiReg.Bits.rank = ScrubRank;
  mPatrolScrubStatus[nodeId].ScrubAddrLoReg.Data = 0;

  mPatrolScrubStatus[nodeId].ScrubCtlRegSave.Data = MemReadPciCfg(nodeId, PARAM_UNUSED, SCRUBCTL_MC_MAINEXT_REG);
  mPatrolScrubStatus[nodeId].ScrubCtlReg.Data = mPatrolScrubStatus[nodeId].ScrubCtlRegSave.Data;
  mPatrolScrubStatus[nodeId].ScrubCtlReg.Bits.ptl_cmpl= 0;
  mPatrolScrubStatus[nodeId].ScrubCtlReg.Bits.scrub_en = 1;
  mPatrolScrubStatus[nodeId].ScrubCtlReg.Bits.startscrub = 1;
  mPatrolScrubStatus[nodeId].ScrubCtlReg.Bits.stop_on_cmpl = 1;
  ReEnablePatrolScrubEngine(nodeId);

  return Status; 
}

/*++

Routine Description:
  If the source of the SMI is a patrol scrub complete, handle the patrol event based on workaround variable
  
Arguments:
  DispatchHandle - Dispatch handle (unused for now!)
  Dispatch Context - Context information (unused for now!)
  CommBuffer     - Buffer used for communication between caller/callback function (unused for now!)
  CommBufferSize - size of communication buffer (unused for now!)

Returns:
  None

--*/
EFI_STATUS
AsymmetricDIMMWAHandler(
    IN EFI_HANDLE        DispatchHandle,
    IN CONST VOID        *DispatchContext, // optional
    IN OUT   VOID        *CommBuffer,      // optional
    IN OUT   UINTN       *CommBufferSize   // optional
)
{
  SCRUBCTL_MC_MAINEXT_STRUCT	scrubCtlReg;
  UINT8 nodeId;
  EFI_STATUS                  status = EFI_SUCCESS;
  
  for(nodeId = 0; nodeId < MC_MAX_NODE; nodeId++) {
    if(mSystemMemoryMap->NodeInfo[nodeId].Enabled == 0) continue;
    scrubCtlReg.Data = MemReadPciCfg(nodeId, PARAM_UNUSED, SCRUBCTL_MC_MAINEXT_REG);
    DEBUG ((DEBUG_ERROR, "\t scrubCtlReg.Data for node %x  = %lx \n", nodeId, scrubCtlReg.Data));
    if(scrubCtlReg.Bits.ptl_cmpl){
      HandleAsymmetricDIMMPatrolEvent(nodeId);
    }
  }
  return status;
}

/*++

Routine Description:
  Save AMAP and DIMMMTR register values in SMRAM
  Use the SCRUBCTL register, stop_on_cmpl bit to enable SMI on the completion of a patrol scrub.  
* Initialize a per MC workaround variable in SMRAM to indicate the workaround is in the state where the full size ranks are being scrubbed

Arguments:
  NodeId - Memory controller ID
  Ch - Channel 
  dimm - DIMM

Returns:
  None

--*/

VOID
Execute_AsymmetricDIMM_WA(
  IN UINT8 nodeId,
  IN UINT8 ch,
  IN UINT8 dimm
)
{
  SCRUBCTL_MC_MAINEXT_STRUCT	scrubCtlReg;

  mAMAPArray[nodeId][ch].Data = MemReadPciCfg(nodeId, ch, AMAP_MCDDC_DESC_REG);
  mDIMMMTR0[nodeId][ch].Data = MemReadPciCfg(nodeId, ch, DIMMMTR_0_MCDDC_DESC_REG);
  mDIMMMTR1[nodeId][ch].Data = MemReadPciCfg(nodeId, ch, DIMMMTR_1_MCDDC_DESC_REG);
  mDIMMMTR2[nodeId][ch].Data = MemReadPciCfg(nodeId, ch, DIMMMTR_2_MCDDC_DESC_REG);

  scrubCtlReg.Data = MemReadPciCfg(nodeId, PARAM_UNUSED, SCRUBCTL_MC_MAINEXT_REG);
  scrubCtlReg.Bits.stop_on_cmpl = 1;
  MemWritePciCfg( nodeId, PARAM_UNUSED, SCRUBCTL_MC_MAINEXT_REG, scrubCtlReg.Data);
  mWAState[nodeId] = FULL_SIZE_RANK_CHECK;  // full size rank init
return;
}

/*++

Routine Description:
  Install Asymmetric DIMM SMI handler if required. 

Arguments:
  None

Returns:

  EFI_SUCCESS

--*/

EFI_STATUS
InstallAsymmetricDIMMPatrolScrubHandler(
VOID
)
{
  UINT8     nodeId;
  UINT8     ch;
  UINT8     dimm;
  UINT8     rank;
  EFI_HANDLE  imageHandle;
  EFI_STATUS   Status=EFI_SUCCESS;
  BOOLEAN   AsymmetricDIMM_WA = FALSE;

  for(nodeId = 0; nodeId < MC_MAX_NODE; nodeId++) {
    // 
    // Skip nodes that are not present
    //
    if(mSystemMemoryMap->NodeInfo[nodeId].Enabled == 0) continue;
    for(ch = 0; ch < MAX_CH; ch++) {
      //
      // Skip channels that are not present
      //
      if(mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].Enabled == 0) continue;
      for(dimm = 0; dimm < MAX_DIMM ; dimm ++){
        //
        // Skip dimms that are not present
        //
        for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
          if ((mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].DimmInfo[dimm].Present) && (mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].DimmInfo[dimm].mapOut[rank] == 1) ||
             ((mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].DimmInfo[dimm].SPDSecondaryDeviceType & (SPD_QUARTER_SIZE_SECOND_RANK | SPD_HALF_SIZE_SECOND_RANK)) && ((rank) % 2))) { // To_Do
            Execute_AsymmetricDIMM_WA(nodeId, ch, dimm); 
            DEBUG ((DEBUG_ERROR, "\t mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].DimmInfo[dimm].mapOut[%x] = %lx \n", rank, mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].DimmInfo[dimm].mapOut[rank]));
            DEBUG ((DEBUG_ERROR, "\t mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].DimmInfo[dimm].SPDSecondaryDeviceType = %lx \n", mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].DimmInfo[dimm].SPDSecondaryDeviceType));
            AsymmetricDIMM_WA = TRUE;
            } else {
            continue;
          }
        }
      }
    }
  }	
  imageHandle = NULL;
  if(AsymmetricDIMM_WA) {
    Status = gSmst->SmiHandlerRegister (
                      AsymmetricDIMMWAHandler,
                      NULL,
                      &imageHandle
                      );
    DEBUG ((DEBUG_ERROR, "\t Installing AsymmetricDIMMWAHandler done \n"));
    ASSERT_EFI_ERROR (Status); 
  }
  return Status;
}
    
