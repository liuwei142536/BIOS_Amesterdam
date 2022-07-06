/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  RankSparing.c

Abstract:

  This file implements Rank Sparing code

--*/

#include <Token.h> // AptioV server override

#ifndef GRANGEVILLE_FLAG

#include "RankSparing.h"
#include "SparingEngine.h"
#include "PatrolScrub.h"
#include "Platform.h"


extern SYSTEM_CONFIGURATION  mSystemConfiguration;


//
// Spare Rank Status
//

static
RANK_STATUS_TYPE  mRankStatus[MC_MAX_NODE][MAX_CH][MAX_RANK_CH];



VOID
EFIAPI
ParseRankState(
      IN UINT8 McNodeId,
      IN UINT8 Ch,
      IN UINT8 Rank
)
/*++

Routine Description:
   Parse rank status.

Arguments:
   McNodeId -- Memory controller ID
   Ch -- channel number.
   Rank -- logical ran


Returns:
   none

--*/
{
  RANK_STATUS_TYPE state;
  UINT32           rankstate;

  state.Data= mRankStatus[McNodeId][Ch][Rank].Data;
  RAS_DEBUG((4,"MC:%d   Ch:%d  Rank:%d  ----  ",McNodeId,Ch,Rank));
  if(state.Bits.rank_exist) RAS_DEBUG((4,"rank exist,   "));
    if(state.Bits.rank_fail) RAS_DEBUG((4,"fail,   "));
  
    if(state.Bits.non_spare_rank) {
       RAS_DEBUG((4,"non-spare rank state:   "));
       rankstate = state.Bits.non_spare_status;
       switch(rankstate){

       case NONSPARESTATUS_SCRUBNEXT:
            RAS_DEBUG((4,"scrub next"));
            break;
       case NONSPARESTATUS_SCRUBBING:
            RAS_DEBUG((4,"scrubbing"));
            break;
       case NONSPARESTATUS_DOSPARECOPY:
            RAS_DEBUG((4,"do spare copying"));
            break;
       case NONSPARESTATUS_SPARECOPYDONE:
            RAS_DEBUG((4,"spare copy done"));  
            break;
       case NONSPARESTATUS_TRY_TAG:
            RAS_DEBUG((4,"try do tag"));  
            break;

       default:
            RAS_DEBUG((4,"wrong state"));
            break;
        }
      }

    if(state.Bits.spare_rank) {
       RAS_DEBUG((4,"spare rank state:   "));
       rankstate = state.Bits.spare_rank_status;
       switch(rankstate){
       case SPARERANKSTATUS_AVAILABLE:
            RAS_DEBUG((4,"available"));
            break;
       case SPARERANKSTATUS_SCRUBBING:
            RAS_DEBUG((4,"scrubbing"));
            break;
       case SPARERANKSTATUS_INPROGRESS:
             RAS_DEBUG((4,"do spare copying (in progress)"));
             break;
       case SPARERANKSTATUS_NOTAVAILALBE:
             RAS_DEBUG((4,"not available"));
             break;
       default:
              RAS_DEBUG((4,"wrong state"));
              break;
       }
    }
   
    RAS_DEBUG((4,"\n "));  
}


RANK_STATUS_TYPE
EFIAPI
GetRankState(
   IN  UINT8  McNodeId,
   IN  UINT8  Ch,
   IN  UINT8  LogicalRank
)
/*++

Routine Description:
   get specific rank state

Arguments:
   McNodeId -- Memory controller ID
   Ch -- channel number.
   LogicalRank -- logical rank index.


Returns:
   rank state

--*/
{
  return mRankStatus[McNodeId][Ch][LogicalRank];
}



VOID
EFIAPI
SetRankState(
      IN UINT8     McNodeId,
      IN UINT8     Ch,
      IN UINT8     LogicalRank,
      IN UINT8     NewState
)
/*++

Routine Description:
   set  rank new status. New status is  should downgrade, not upgrade here.

Arguments:
   McNodeId -- Memory controller ID
   Ch -- channel number.
   LogicalRank -- logical rank index.
   SpareStatus -- spare rank new status. 

Returns:
   none

--*/
{
  UINT32  temp = NewState;

  RAS_DEBUG((4,"SetRankState \n"));
  ParseRankState(McNodeId,Ch,LogicalRank);
  

  if(mRankStatus[McNodeId][Ch][LogicalRank].Bits.rank_exist  &&
     mRankStatus[McNodeId][Ch][LogicalRank].Bits.spare_rank  &&
     (mRankStatus[McNodeId][Ch][LogicalRank].Bits.rank_fail == 0)){
        mRankStatus[McNodeId][Ch][LogicalRank].Bits.spare_rank_status = temp;
  }
  

  if(mRankStatus[McNodeId][Ch][LogicalRank].Bits.rank_exist  &&
     mRankStatus[McNodeId][Ch][LogicalRank].Bits.non_spare_rank ){
     mRankStatus[McNodeId][Ch][LogicalRank].Bits.non_spare_status = temp;
  }  

  ParseRankState(McNodeId,Ch,LogicalRank);
  
  return ;
}





BOOLEAN
EFIAPI
CheckRankState(
      IN UINT8     McNodeId,
      IN UINT8     Ch,
      IN UINT8     LogicalRank,
      IN UINT8     CheckState      
)
/*++

Routine Description:
   check if current rank is specific state of  rank.

Arguments:
   McNodeId -- Memory controller ID
   Ch -- channel number.
   LogicalRank -- logical rank.
   CheckState  -- check state

Returns:
    TRUE  -- available state of spare rank.
    FALSE -- not available state of spare rank.

--*/
{
  BOOLEAN RankStatusExist = FALSE;  
  
  if(mRankStatus[McNodeId][Ch][LogicalRank].Bits.rank_exist  &&
     mRankStatus[McNodeId][Ch][LogicalRank].Bits.spare_rank  &&
     (mRankStatus[McNodeId][Ch][LogicalRank].Bits.rank_fail == 0)){
        if(mRankStatus[McNodeId][Ch][LogicalRank].Bits.spare_rank_status  == CheckState){
           RankStatusExist = TRUE;
        }
  }

  if(mRankStatus[McNodeId][Ch][LogicalRank].Bits.rank_exist  &&
     mRankStatus[McNodeId][Ch][LogicalRank].Bits.non_spare_rank ){
       if(mRankStatus[McNodeId][Ch][LogicalRank].Bits.non_spare_status == CheckState){
          RankStatusExist = TRUE;
       }
  }

  ParseRankState(McNodeId,Ch,LogicalRank);
  RAS_DEBUG((4," check rank state : check state%d   status:%d\n",
           CheckState,
           RankStatusExist));
  
  return RankStatusExist;
}



BOOLEAN
EFIAPI
CheckRankStatePerCh(
      IN     UINT8       McNodeId,
      IN     UINT8       Ch,
      IN     UINT8       state,
      IN OUT UINT8 *     pFoundRank
)
/*++

Routine Description:
   check if current channel has specific state  rank, 

Arguments:
   McNodeId -- Memory controller ID
   Ch -- channel number.
   state -- find state status
   pLogicalRank -- pointer to found rank

Returns:
    TRUE  -- found.
    FALSE -- no found.
    Status

--*/
{
  BOOLEAN RankStatusExist = FALSE;
  UINT8   i;

  for(i=0; i <(MAX_RANK_CH); i ++){
    if(CheckRankState( McNodeId, Ch,i, state)){
      RankStatusExist = TRUE;
      if(pFoundRank != NULL){
        *pFoundRank = i;
      }
     break;
     }
  }
   
  RAS_DEBUG((4, "CheckRankStatePerCh --spare rank specific state exist per channel?   MC:%d   Channel:%d  State:%d  exist:%d (1-- true, 0-- false)  \n",
          McNodeId,
          Ch,
          state,
          RankStatusExist));
  
  return RankStatusExist;
}



BOOLEAN
EFIAPI
CheckRankStatePerMC(
      IN     UINT8     McNodeId,
      IN     UINT8     state,
      IN OUT UINT8 *   pFoundCh,
      IN OUT UINT8 *   pFoundRank
)
/*++

Routine Description:
   check if current MC has specific state rank, 

Arguments:
   McNodeId -- Memory controller ID
   Ch -- channel number.
   state -- find state status
   pLogicalRank -- pointer to found rank

Returns:
    TRUE  -- found.
    FALSE -- no found.
    Status

--*/
{
  BOOLEAN MCStatusExist = FALSE;
  UINT8   i;

  for(i=0; i <(MAX_CH); i ++){
    //if (mSystemMemoryMap->NodeInfo[McNodeId].ChannelInfo[i].Enabled == 0) {
    //  continue;
    //}
    MCStatusExist = CheckRankStatePerCh( McNodeId, i,state, pFoundRank);
    if(MCStatusExist){
      if(pFoundCh){
        *pFoundCh = i;
      }
      break;
    }//if
  }//for
   
  RAS_DEBUG((4, "CheckRankStatePerMC--rank specific state exist per MC?   MC:%d     State:%d  exist:%d  \n",
          McNodeId,
          state,
          MCStatusExist));
     
  return MCStatusExist;
}





VOID 
EFIAPI
GetLowestPrioritySpareRank(
      IN   UINT8     McNodeId,
      IN   UINT8     Ch,
      OUT  UINT8*    pLogicalRank
)
/*++

Routine Description:
   get the lowest priority spare rank.

Arguments:
   McNodeId -- Memory controller ID
   Ch -- channel number.
   LogicalRank -- pointer to logical rank.

Returns:
   EFI_STATUS

Note: invoker should call IsAvailSpareRankExist first.
--*/
{
  UINT8   i;
  UINT8   LowestRank = 0;
  UINT32  LowestSeq = 0;
  UINT32  Seq;
   
  for( i =0 ; i < MAX_RANK_CH; i ++){
    if(CheckRankState(McNodeId,Ch,i,SPARERANKSTATUS_AVAILABLE)){
      Seq = mRankStatus[McNodeId][Ch][i].Bits.spare_rank_sequence ;
      if(Seq >= LowestSeq){
        LowestRank = i;
        LowestSeq = Seq;
      }
    }
  }
  
  *pLogicalRank = LowestRank;

  RAS_DEBUG((4, "GetLowestPrioritySpareRank--  MC:%d   Ch:%d  low priority rank:%d \n",
            McNodeId,
            Ch,
            LowestRank));

  return ;
}



#ifdef SPARING_SCRATCHPAD_SUPPORT
VOID
UpdateScratchPadReg(
   IN UINT8 McNodeId,
   IN UINT8 Ch
)
/*++

Routine Description:
    Update scratch pad register after spare rank is mark dirty.
    
Suggestion on implementation of biosscratchpad register:
 Proposed setup of the registerHSX:
 BIOSNonStickyScratchpad4 (MC0)/BIOSNonStickyScratchpad6 (MC1)
 bits 0-7     one-hot encoding of ch 0 spare ranks, each bit position represents a logical rank id
 bits 8-15   one-hot encoding of ch 1 spare ranks, each bit position represents a logical rank id
 bits 16-23 one-hot encoding of ch 2 spare ranks, each bit position represents a logical rank id
 bits 24-31 one-hot encoding of ch 3 spare ranks, each bit position represents a logical rank id


 BIOSSCRATCHPAD5:
 bit 16 ch0 sparing enabled
 bit 17 ch1 sparing enabled
 bit 18 ch2 sparing enabled
 bit 19 ch3 sparing enabled
 bit 20 ch4 sparing enabled
 bit 21 ch5 sparing enabled
 bit 22 ch6 sparing enabled
 bit 23 ch7 sparing enabled

Arguments:
    NodeId  -- MC id
    Ch -- when value is 0xff, do whole MC; other do specified channel.
 Returns:
    None    

--*/
{
  UINT8                                     i;
  UINT32                                    ScratchPadValue;
  UINT8                                     ShiftLeftBit;
  BIOSNONSTICKYSCRATCHPAD4_UBOX_MISC_STRUCT NonScratchPadReg;


  ScratchPadValue = 0;
  for(i = 0;  i < MAX_RANK_CH; i++){
    if(CheckRankState(McNodeId,  Ch,i, SPARERANKSTATUS_AVAILABLE)){
       ScratchPadValue |= (1 << i);
    }
  }


  if(McNodeId % 2){
  
    //
    //second MC
    //
    NonScratchPadReg.Data= MemReadPciCfg(NODE_TO_SKT(McNodeId), 0,  BIOSNONSTICKYSCRATCHPAD6_UBOX_MISC_REG);
    RAS_DEBUG((4, "UpdateScratchPadReg -  before scratch pad 6 reg:%x\n", NonScratchPadReg.Data));  
  }else{
    //
    //first MC
    //
    NonScratchPadReg.Data= MemReadPciCfg(NODE_TO_SKT(McNodeId), 0, BIOSNONSTICKYSCRATCHPAD4_UBOX_MISC_REG);
    RAS_DEBUG((4, "UpdateScratchPadReg -  before scratch pad 4 reg:%x\n", NonScratchPadReg.Data));  
  }

  NonScratchPadReg.Data &= ~((UINT32)0xff << (Ch * 8));
  NonScratchPadReg.Data |=  (ScratchPadValue << (Ch * 8));

  if(mSystemMemoryMap->NodeInfo[McNodeId].RasModesEnabled & CH_LOCKSTEP)	{
    NonScratchPadReg.Data &= ~((UINT32)0xff << ((Ch ^ 01) * 8));
    NonScratchPadReg.Data |=  (ScratchPadValue << ((Ch^01) * 8));
  }
  if(McNodeId % 2){
  
    //
    //second MC
    //
    MemWritePciCfg(NODE_TO_SKT(McNodeId), 0, BIOSNONSTICKYSCRATCHPAD6_UBOX_MISC_REG,NonScratchPadReg.Data);
    RAS_DEBUG((4, "UpdateScratchPadReg -  current scratch pad 6 reg:%x\n", NonScratchPadReg.Data));  
   }else{
   
     //
     //first MC
     //
     MemWritePciCfg(NODE_TO_SKT(McNodeId), 0, BIOSNONSTICKYSCRATCHPAD4_UBOX_MISC_REG,NonScratchPadReg.Data);
     RAS_DEBUG((4, "UpdateScratchPadReg -  current scratch pad 4 reg:%x\n", NonScratchPadReg.Data));  
   }

  if(ScratchPadValue == 0){
  
    //
    //clear channel enable status on scratch pad 5
    //
    if(McNodeId % 2){
        ShiftLeftBit = 20;
    }else{
        ShiftLeftBit = 16;
    }
   
    NonScratchPadReg.Data= MemReadPciCfg(NODE_TO_SKT(McNodeId), 0, BIOSSCRATCHPAD5_UBOX_MISC_REG);
   
    RAS_DEBUG((4, "UpdateScratchPadReg -   before updating, scratch pad 5 reg:%x\n", NonScratchPadReg.Data));
   
    NonScratchPadReg.Data &= (~((UINT32)1 << (ShiftLeftBit + Ch)));
   
    if(mSystemMemoryMap->NodeInfo[McNodeId].RasModesEnabled & CH_LOCKSTEP)	{   
      NonScratchPadReg.Data &= (~((UINT32)1 << (ShiftLeftBit + (Ch^01))));
    }
    MemWritePciCfg(NODE_TO_SKT(McNodeId), 0, BIOSSCRATCHPAD5_UBOX_MISC_REG, NonScratchPadReg.Data);
   
    RAS_DEBUG((4, "UpdateScratchPadReg -   current scratch pad 5 reg:%x\n", NonScratchPadReg.Data));
   }
    return;
}
#endif


VOID
EFIAPI
MarkRankDirtyFlag(
   IN   UINT8 McNodeId,
   IN   UINT8 Ch,
   IN   UINT8 LogicalRank
)
/*++

Routine Description:
   Mark the rank as dirty flag.
   1. set mSpareRankStatus as RankSpareNotAvailable if it is a spare rank
   2. add it to mFailedRank

Arguments:
   McNodeId -- Memory controller ID
   Ch -- channel number.
   LogicalRank -- logical rank.

Returns:
    TRUE  -- available spare rank.
    FALSE -- not an spare rank.
    Status

--*/
{
    mRankStatus[McNodeId][Ch][LogicalRank].Bits.rank_fail = 1;

    RAS_DEBUG((4, "MarkRankDirtyFlag -  MC:%d  channel:%d,  rank:%d is dirty\n",McNodeId, Ch,LogicalRank));
    return;
}




VOID
EFIAPI
DoSpareCopy(
      IN UINT8           NodeId,
      IN UINT8           Ch,
      IN UINT8           Rank,
      IN SPARE_COPY_TYPE type

)
/*++

Routine Description:
   do spare copy operation for patrol scrub is disabled case.


Arguments:
   NodeId -- Memory controller ID
   Ch -- channel number.
   Rank -- logical rank.
   type -- identify error source type.

Returns:
   none

--*/
{
  UINT32    errStatus;
  UINT8     NextSpareRank;

  if(AcquireSparingEngine(NodeId) == EFI_SUCCESS){

    if(type == EVENT_SPARE_COPY){
       RAS_DEBUG((1, "[RAS event] CE event on ch:%d  rank:%d\n",Ch,Rank)); 
       if(!CheckRankStatePerCh(NodeId,Ch,SPARERANKSTATUS_AVAILABLE, NULL)){
          RAS_DEBUG((1, "[RAS event] no available rank on channel:%d\n",Ch)); 
          //
          //Todo: tag
          //
          return;
       }
    }else{
        RAS_DEBUG((1, "[RAS new] do spare copying, CE on ch:%d  rank:%d\n",Ch,Rank)); 
    }
  
    GetLowestPrioritySpareRank(NodeId,Ch,&NextSpareRank);


    //
    //do spare copy operation
    //   
    StartSparing(NodeId,Rank,NextSpareRank,Ch,RankSparing);
 
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

    SetRankState(NodeId,Ch,NextSpareRank,SPARERANKSTATUS_INPROGRESS);
    SetRankState(NodeId,Ch, Rank, NONSPARESTATUS_DOSPARECOPY);
    if(mSystemMemoryMap->NodeInfo[NodeId].RasModesEnabled & CH_LOCKSTEP)	{
      SetRankState(NodeId,Ch^01,NextSpareRank,SPARERANKSTATUS_INPROGRESS);
      SetRankState(NodeId,Ch^01, Rank, NONSPARESTATUS_DOSPARECOPY);
    }

  }else{
    RAS_DEBUG((1, "[RAS NEW] spare engine is busy!\n"));  
  }

}




EFI_STATUS
EFIAPI
InitializeRankSparing(
 )
 /*++

Routine Description:
    Initialize all rank sparing related data structures
    This routine should be called only once during boot

Arguments:

  None
Returns:

    Status

--*/ 
{
  UINT8     nodeId;
  UINT8     ch;
  UINT8     rankIndex;
  UINT8     rank;

  AMAP_MCDDC_DESC_STRUCT            AMAPReg;
  UINT8                             i;
  UINT8                             dimmSlot;
  UINT32                            rankDisabledMap =0;



  //
  // It don't init non-spare rank info here, init it at error handler in case error happen.
  //
  for(nodeId = 0; nodeId < MC_MAX_NODE; nodeId++) {
    for(ch = 0; ch < MAX_CH; ch++) {
       for(rankIndex = 0; rankIndex < (MAX_RANK_CH/2);rankIndex++){ 
        if( mSystemMemoryMap->NodeInfo[nodeId].Enabled &&
            mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].Enabled &&
            (mSystemMemoryMap->NodeInfo[nodeId].RasModesEnabled & RK_SPARE) &&
            (mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].SpareLogicalRank[rankIndex] != 0xFF)){
               
                 rank = mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].SpareLogicalRank[rankIndex];
                 mRankStatus[nodeId][ch][rank].Data = 0;
                 mRankStatus[nodeId][ch][rank].Bits.rank_exist = 1;
                 mRankStatus[nodeId][ch][rank].Bits.spare_rank = 1;
                 mRankStatus[nodeId][ch][rank].Bits.spare_rank_sequence = rankIndex;
                 RAS_DEBUG((1, "[RAS]       spare rank from HOB   MC:%d   channel:%d   spare rank index:%d   logical rank:%d\n",
                   nodeId,
                   ch,
                   rankIndex,
                   rank));
                   
             } 
       }//rank loop
    } // ch loop
  } // node loop


  //
  //In case the whole channel disabled, use scrub_mask to disabled for patrol scrub;
  //other case use AMAP to control.
  //
  for(nodeId = 0; nodeId < MC_MAX_NODE; nodeId++) { 

    if((mSystemMemoryMap->NodeInfo[nodeId].Enabled) == 0){
      continue;
    }

    //
    //exit in case not spare mode
    //
    if((mSystemMemoryMap->NodeInfo[nodeId].RasModesEnabled & RK_SPARE) == 0){
      break;
    }

    //
    //scan for each channel
    //
    for(ch = 0; ch < MAX_CH; ch++) {
      AMAPReg.Data= MemReadPciCfg(nodeId, ch, AMAP_MCDDC_DESC_REG);

      //
      //no disabled rank detected
      //
      if((AMAPReg.Bits.dimm0_pat_rank_disable == 0) &&
        (AMAPReg.Bits.dimm1_pat_rank_disable == 0) &&
        (AMAPReg.Bits.dimm2_pat_rank_disable == 0)){
          continue;
        }
  
       for(dimmSlot = 0; dimmSlot < MAX_DIMM; dimmSlot ++){
         if(dimmSlot == 0){
           rankDisabledMap = AMAPReg.Bits.dimm0_pat_rank_disable;
         }

         if(dimmSlot == 1){
           rankDisabledMap = AMAPReg.Bits.dimm1_pat_rank_disable;
         }

         if(dimmSlot == 2){
           rankDisabledMap = AMAPReg.Bits.dimm2_pat_rank_disable;
         }

         if(rankDisabledMap == 0){
           continue;
         }

         for(i = 0; i< 4; i ++){
           if(rankDisabledMap & (1 << i)){
              rank = dimmSlot * 4 + i;
              if(dimmSlot == 2){
                rank -= 2;
              }
             //
             // In the AMAP register, some bits is for spare rank, and other is for memory test failed rank,
             // we need only to init the memory error rank.
             //
             if ((mRankStatus[nodeId][ch][rank].Bits.rank_exist == 1) && (mRankStatus[nodeId][ch][rank].Bits.spare_rank == 1)) {
               continue;
             }
              mRankStatus[nodeId][ch][rank].Data = 0;
              mRankStatus[nodeId][ch][rank].Bits.rank_exist = 1;
              mRankStatus[nodeId][ch][rank].Bits.rank_disable = 1;
              RAS_DEBUG((1, "[RAS] get MRC disaled rank   MC:%d   channel:%d   logical rank:%d\n",
                   nodeId,
                   ch,
                   rank));
           }
         }
       }
    }
  }
  return EFI_SUCCESS;
}


static
EFI_STATUS
CheckAndHandleRankSparingDone(
    IN  UINT8    NodeId,
    OUT BOOLEAN *IsEventHandled
)
/*++

Routine Description:
    Check and handle ranks sparing done event

Arguments:

  NodeId    - Memory controller ID
  *IsEventHandled - ptr to a buffer to hold event handled status

Returns:

    status - EFI_SUCCESS if no failures
             non-zero error code for failures

    IsEventHandled - TRUE -  event was handled
                     FALSE - event not handled

--*/
{
  EFI_STATUS  status = EFI_SUCCESS;
  UINT8       ch;
  BOOLEAN     rankSpareInProgress;
  UINT32      errStatus;
  UINT8       spareProgressRank = 0;
  UINT8       FoundedCh;
  UINT8       FoundedPendingRank;
  UINT8       SourceCh;
  UINT8       SourceRank;

  //
  // Check if there is any rank sparing event already in progress
  // At a time only one rank sparing operation can be in progress
  // for a given node.
  //
  rankSpareInProgress = CheckRankStatePerMC(NodeId, SPARERANKSTATUS_INPROGRESS, &ch, &spareProgressRank);

  if( rankSpareInProgress ){
    //
    // Check if copy operation is complete
    //
    status = IsCopyComplete(  NodeId, RankSparing );
    switch( status ){
    case EFI_SUCCESS:
     RAS_DEBUG((1, "[RAS pending event] spare copy complete! \n"));

     //
     //update status
     //
     SetRankState(NodeId,ch,spareProgressRank,SPARERANKSTATUS_NOTAVAILALBE); 
     if(mSystemMemoryMap->NodeInfo[NodeId].RasModesEnabled & CH_LOCKSTEP)	{
      SetRankState(NodeId,ch ^ 01,spareProgressRank,SPARERANKSTATUS_NOTAVAILALBE);
     } 
     if(CheckRankStatePerMC(NodeId,  NONSPARESTATUS_DOSPARECOPY, &SourceCh, &SourceRank)){
         if(SourceCh != ch){
         RAS_DEBUG((1,"error! the spare copy ch:%d is not equal to found ch:%d",ch,SourceCh));
         }
         SetRankState(NodeId, SourceCh, SourceRank, NONSPARESTATUS_SPARECOPYDONE);
         if(mSystemMemoryMap->NodeInfo[NodeId].RasModesEnabled & CH_LOCKSTEP)	{
          SetRankState(NodeId,SourceCh ^ 01,SourceRank,NONSPARESTATUS_SPARECOPYDONE);
         }

     }

#ifdef SPARING_SCRATCHPAD_SUPPORT
    //
    //update scratch pad register here
    //
    UpdateScratchPadReg(NodeId,SourceCh);
#endif
      //
      // Clear error count registers in CPU and JCK
      //
      ClearErrorsForThisRank(  NodeId, ch, spareProgressRank);


      ReleaseSparingEngine(NodeId);
      //
      //restore patrol scrub.
      //
      if(CheckPatrolScrubEnable()){
        ReEnableSSR(NodeId,FALSE);
      }


      errStatus = 0;
      GetOrSetErrorHandlingStatus(  NodeId, UpdateEventProgressStatus, &errStatus );
      GetOrSetErrorHandlingStatus(  NodeId, UpdateEventPendingStatus, &errStatus ); 

     //
     //check if any pending event on this MC. In case any pending event exist and available spare rank exist,
     //do spare copying again.
     //
     if(CheckRankStatePerMC(NodeId, NONSPARESTATUS_SCRUBNEXT, &FoundedCh, &FoundedPendingRank)){
       RAS_DEBUG((1, "[RAS pending event] there is a pending CE event exist  Ch:%d   Rank:%d\n",FoundedCh,FoundedPendingRank));
       //
       //check if available rank exist
       //
       if(CheckPatrolScrubEnable()){
         ScrubNextRank(NodeId,FoundedCh,FoundedPendingRank,NON_SPARE_CE_EVENT); 
       }else{
         DoSpareCopy(NodeId,FoundedCh,FoundedPendingRank,EVENT_SPARE_COPY);
       }
     }

      *IsEventHandled = TRUE;
      status = EFI_SUCCESS;
      break;

    case EFI_NOT_READY:   // copy operation in progress
      RAS_DEBUG((1, "[RAS pending event] spare copy not complete! Copy in progress for Node[%d].CH[%d]\n", NodeId, ch));

      status = EFI_SUCCESS;
      *IsEventHandled = FALSE;
    default:              // Error in operation
      *IsEventHandled = FALSE;
      break;
    }
  } else {
    //
    // No Ranks in this node are in Rank Sparing mode
    //
    *IsEventHandled = FALSE;
  }

  return status;
}


static
EFI_STATUS
CheckAndHandleNewRankSparingEvent(
    IN  UINT8     NodeId,
    OUT BOOLEAN * IsEventHandled
 )
 /*++

Routine Description:
    Check for new rank sparing events and handle

Arguments:

  NodeId    - Memory controller ID
  *IsEventHandled - ptr to a buffer to hold event handled status

Returns:

    status - EFI_SUCCESS if no failures
             non-zero error code for failures

    IsEventHandled - TRUE -  event was handled
                     FALSE - event not handled

--*/
{
  EFI_STATUS  status = EFI_SUCCESS;
  UINT8       ch;
  UINT8       rank;
  UINT8       ChannelStep;

  ChannelStep = 1;
  if(mSystemMemoryMap->NodeInfo[NodeId].RasModesEnabled & CH_LOCKSTEP)  
    ChannelStep = 2;

  *IsEventHandled = FALSE;

  for( ch = 0; ch < MAX_CH; ch+= ChannelStep ){
    if (mSystemMemoryMap->NodeInfo[NodeId].ChannelInfo[ch].Enabled == 0) {
      continue;
    }

    //
    // TODO:
    // If we are in lock-step mode and this CH is not the
    // primary DDR CH, skip this
    //

    //
    // If this ch doesn't have spare rank, skip to next ch
    //
    if(!CheckRankStatePerCh(NodeId,ch,SPARERANKSTATUS_AVAILABLE, NULL)) continue;

    //
    // Check if there is any rank in this channel
    // that has overflow bit set
    //
    for( rank = 0; (rank < MAX_RANK_CH) ; rank++ ){
      if( IsErrorExcdForThisRank(  NodeId, ch, rank) ){
        break;
      }
    }

    if( rank != MAX_RANK_CH ){
        RAS_DEBUG((1, "[RAS CE new]  found a rank error on MC:%d   channel:%d,  rank:%d\n", NodeId,ch,rank));



       //
       //this error happen on spared rank, this is from patrol scrub on spare ranks.
       //
       if(CheckRankState(NodeId,ch,rank,SPARERANKSTATUS_SCRUBBING)){
           RAS_DEBUG((1, "[RAS CE new]  this is an spare rank CE\n"));
           
       //
       //mark this rank as "dirty"
       //
       MarkRankDirtyFlag(NodeId,ch,rank);

       //
       //move to next rank scrub......
       //
       ScrubNextRank(NodeId,ch,rank,SPARE_CE);
      }else{
       RAS_DEBUG((1, "[RAS CE new]  this is an non-spare rank CE\n"));

       //
       //init non-spare rank.
       //
       if(CheckRankState(NodeId, ch, rank, NONSPARESTATUS_SCRUBNEXT) ||
           CheckRankState(NodeId, ch, rank, NONSPARESTATUS_SCRUBBING) ||
           CheckRankState(NodeId, ch, rank, NONSPARESTATUS_DOSPARECOPY) ){
           //
           //during overflow rank is actually do scrubbing,  spare copy , or wait, another overflow  come in again.
           //it is to prevent re-entry.
           //
          *IsEventHandled = TRUE;
           return status;       
        }
       
       mRankStatus[NodeId][ch][rank].Data = 0;
       mRankStatus[NodeId][ch][rank].Bits.rank_exist = 1;
       mRankStatus[NodeId][ch][rank].Bits.non_spare_rank = 1;
       mRankStatus[NodeId][ch][rank].Bits.non_spare_status =  NONSPARESTATUS_SCRUBNEXT; 

       //
       //mark this rank as "dirty"
       //
       MarkRankDirtyFlag(NodeId,ch,rank);

       //
       //check patrol scrub if enabled.
       //
       if(CheckPatrolScrubEnable()){
          //
          //move to next rank scrub......
          //
          ScrubNextRank(NodeId,ch,rank,NON_SPARE_CE);
       }else{
          DoSpareCopy(NodeId,ch,rank,NEW_SPARE_COPY);
       }
  
     }

     *IsEventHandled = TRUE;

      return status;
    }

  } // for(ch)

  return status;
}


EFI_STATUS
EFIAPI
CheckAndHandleRankSparing(
    IN  UINT8      NodeId,
    IN  EVENT_TYPE EventType,
    OUT BOOLEAN *  IsEventHandled
)
/*++

Routine Description:
    Check if Rank Sparing is possible for this node and
    do Rank Sparing

Arguments:

  NodeId    - Memory controller ID
  EventType - NEW_EVENTS or EVENTS_IN_PROGRESS
  *IsEventHandled - ptr to buffer to hold event handled status

Returns:

    status - EFI_SUCCESS if no failures
             non-zero error code for failures

    IsEventHandled - TRUE -  event was handled
                     FALSE - event not handled

--*/
{
  EFI_STATUS  status = EFI_SUCCESS;

  switch( EventType ){
  case EVENTS_IN_PROGRESS:
    status = CheckAndHandleRankSparingDone(  NodeId, IsEventHandled );
    break;

  case NEW_EVENTS:
    status = CheckAndHandleNewRankSparingEvent(  NodeId, IsEventHandled );
    break;

  default:
    status = EFI_UNSUPPORTED;     // Invalid event type
    break;
  }

  return status;
}

#endif // GRANGEVILLE_FLAG