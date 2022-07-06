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

  SparingEngine.c

Abstract:
  Functions to access/setup Sparing engine
--*/
#include <Token.h> // AptioV server override

#ifndef GRANGEVILLE_FLAG
#include "SparingEngine.h"
#include "PatrolScrub.h"

// Externals used
extern  SMMELOG_HOST                  mMemrasS3Param;


//
// Sparing engine status
//
typedef enum {
  SparingEngineFree,       // Sparing engine is free
  SparingEngineInUse       // Sparing engine is in use
} SPARING_ENGINE_STATUS;

//
// Internal data structure to maintain status for
// all memory nodes
//
static
SPARING_ENGINE_STATUS mSpareEngineStatus[MC_MAX_NODE];
//
// mSocketPkgCSemaphore will be used to enable / disable PkgC6states in a socket.
// PkgC6states can be enabled / disabled only at a socket level. If a node
// requests PkgC6states disable, the semaphore for the socket will be
// incremented.  If a node requests PkgCstates enable, the semaphore will
// be decremented. PkgCstates will be enabled only when mSocketGvSemaphore is 0
//
UINT8 mSocketPkgCSemaphore[MAX_SOCKET];


/*++

Routine Description:
  Checks the internal status and takes ownership of the sparing engine

Arguments:
  NodeId - Memory controller ID

Returns:

  EFI_SUCCESS if sparing engine is free
  EFI_OUT_OF_RESOURCES if sparing engine is already in use

--*/
EFI_STATUS
AcquireSparingEngine(
    IN UINT8 NodeId
 )
{
  EFI_STATUS status = EFI_SUCCESS;

  RAS_DEBUG((4, "AcquireSparingEngine - Node[%d] Entry\n", NodeId));

  if(mSpareEngineStatus[NodeId] == SparingEngineFree) {
    //
    // Disable patrol scrub if enabled
    //
    status = DisablePatrolScrubEngine( NodeId);
    if( status == EFI_SUCCESS){
      mSpareEngineStatus[NodeId] = SparingEngineInUse;
      RAS_DEBUG((4, "AcquireSparingEngine - Node%d - Success\n", NodeId));
      // Disable Pkg C6 before Sparing
      DisablePkgC6(NodeId);

    }
  } else {
    status = EFI_OUT_OF_RESOURCES;
    RAS_DEBUG((4, "AcquireSparingEngine - Node%d - Failed\n", NodeId));
  }

  RAS_DEBUG((4, "AcquireSparingEngine - Node[%d] Exit\n", NodeId));
  return status;
}

/*++

Routine Description:
  Checks the internal status and releases ownership of the sparing engine.
  It is expected that only caller who has successfully acquired ownership
  using AcquireSparingEngine calls this function

Arguments:
  NodeId - Memory controller ID

Returns:

  EFI_SUCCESS if sparing engine is released
  EFI_UNSUPPORTED any other error

--*/
EFI_STATUS
ReleaseSparingEngine(
    IN UINT8 NodeId
 )
{
  EFI_STATUS status = EFI_SUCCESS;

  RAS_DEBUG((1, "ReleaseSparingEngine - Node[%d] Entry\n", NodeId));
  if(mSpareEngineStatus[NodeId] == SparingEngineInUse) {

    //
    // Re-enable patrol scrub if it was enabled before
    //
    if(CheckPatrolScrubEnable()){
//      status = ReEnablePatrolScrubEngine( NodeId);
    } 

    //enable PkgC6 after sparing
    EnablePkgC6(NodeId);
    mSpareEngineStatus[NodeId] = SparingEngineFree;

  } else {
    status = EFI_UNSUPPORTED;     // called before AcquireSparingEngine
    RAS_DEBUG((2, "ReleaseSparingEngine - Node%d - Failed\n", NodeId));
  }

  RAS_DEBUG((1, "ReleaseSparingEngine - Node[%d] Exit\n", NodeId));
  return status;
}



/*++

Routine Description:
  Programs the hardware registers for DDDC or Rank Sparing and
  starts spare copy operation

Arguments:
  NodeId    - Memory controller ID
  SrcRank   - Source rank
  DstRank   - Destination rank or target rank
  Ch        - Channel in which Src and Destination ranks reside
  SparingMode - Type of sparing - DDDC / Rank

Returns:

  EFI_SUCCESS if spare copy is started
  EFI_DEVICE_ERROR if there is an error in spare copy operation
  EFI_UNSUPPORTED all other errors

--*/
EFI_STATUS
StartSparing(
    IN UINT8 NodeId,
    IN UINT8 SrcRank,
    IN UINT8 DstRank,
    IN UINT8 Ch,
    IN SPARING_MODE SparingMode
 )
{
    EFI_STATUS                        status = EFI_SUCCESS;
    SPARECTL_MC_MAINEXT_STRUCT        spareCtlReg;
    SCRUBADDRESSHI_MC_MAINEXT_STRUCT  ScrubAddrHiReg;
	    SPAREADDRESSLO_MC_MAINEXT_HSX_BDX_STRUCT spareAddrLoReg;
    AMAP_MCDDC_DESC_STRUCT     AMAPReg;  
    UINT32                              SparePhysicalRank;
    UINT32                              SpareDimm;
    UINT32                              SpareRankPerDimm;
    UINT32                              FailedPhysicalRank;
    UINT32                              FailedDimm;
    UINT32                              FailedRankPerDimm;
    //
    // make sure scrub ctrl register is not set to scrub on failed rank after sparing.
    //
    RAS_DEBUG((4, "StartSparing -   NodeId :%x  Ch: %x\n", NodeId,Ch));
    ScrubAddrHiReg.Data=  MemReadPciCfg(NodeId, PARAM_UNUSED, SCRUBADDRESSHI_MC_MAINEXT_REG);
    if( (ScrubAddrHiReg.Bits.chnl == Ch) && (ScrubAddrHiReg.Bits.rank == GetLogicalToPhysicalRankId(NodeId, Ch, SrcRank)) )  {
      RAS_DEBUG((4, "StartSparing - spare ctrl Src Logical rank[%x] same as Scrubbing channel phy. rankbefore sparing. Chaning the scrub rank to spare ctrl Dstrank[%x] \n", SrcRank, DstRank ));
      //RAS_DEBUG((4, "StartSparing - spare ctrl Src Logical rank[%x] same as Scrubbing channel[%x] phy. rank[%x] before sparing. Chaning the scrub rank to spare ctrl Dstrank[%x] \n", SrcRank, Ch, ScrubAddrHiReg.Bits.rank, DstRank ));
      ScrubAddrHiReg.Bits.rank  = GetLogicalToPhysicalRankId(NodeId, Ch, DstRank);
      MemWritePciCfg( NodeId, PARAM_UNUSED, SCRUBADDRESSLO_MC_MAINEXT_REG, 00);
      MemWritePciCfg( NodeId, PARAM_UNUSED, SCRUBADDRESSHI_MC_MAINEXT_REG, ScrubAddrHiReg.Data);
    } else {
      RAS_DEBUG((4, "StartSparing - Spare ctrl Src Logical rank[%x] Scrubbing channel phy. rank[%x] \n", SrcRank, ScrubAddrHiReg.Bits.rank ));
    }
    //
    // update AMP register to exclude spare rank.
    //
    AMAPReg.Data= MemReadPciCfg(NodeId, Ch, AMAP_MCDDC_DESC_REG);
    SparePhysicalRank = GetLogicalToPhysicalRankId(NodeId,Ch,DstRank);
    SpareDimm = SparePhysicalRank /4;
    SpareRankPerDimm  = SparePhysicalRank % 4;
    FailedPhysicalRank = GetLogicalToPhysicalRankId(NodeId,Ch,SrcRank);
    FailedDimm = FailedPhysicalRank /4;
    FailedRankPerDimm  = FailedPhysicalRank % 4;
  	RAS_DEBUG((4, "StartSparing -  Spare Rank :%x  SpareDimm:%x \n", DstRank, SpareDimm));
  	RAS_DEBUG((4, "StartSparing -  SpareRankPerDimm :%x  AmapReadValue:%x \n", SpareRankPerDimm, AMAPReg.Data));
    if(SpareDimm == 0){
        AMAPReg.Bits.dimm0_pat_rank_disable  &= (~(1 << SpareRankPerDimm));  
    }
    if(SpareDimm == 1){
      AMAPReg.Bits.dimm1_pat_rank_disable  &= (~(1 << SpareRankPerDimm));  
    }
    if(SpareDimm == 2){
      AMAPReg.Bits.dimm2_pat_rank_disable  &= (~(1 << SpareRankPerDimm));  
    } 
    if(FailedDimm == 0){
      AMAPReg.Bits.dimm0_pat_rank_disable  |= (1 << FailedRankPerDimm);  
    }
    if(FailedDimm == 1){
      AMAPReg.Bits.dimm1_pat_rank_disable  |= (1 << FailedRankPerDimm);  
    }
    if(FailedDimm == 2){
      AMAPReg.Bits.dimm2_pat_rank_disable  |= (1 << FailedRankPerDimm);  
    } 	
    RAS_DEBUG((4, "StartSparing -  ch : %x, Amap write Value:%x \n", Ch,AMAPReg.Data));
    MemWritePciCfg(NodeId, Ch, AMAP_MCDDC_DESC_REG,AMAPReg.Data);
    if(mSystemMemoryMap->NodeInfo[NodeId].RasModesEnabled & CH_LOCKSTEP)	{
	    MemWritePciCfg(NodeId, (Ch^01), AMAP_MCDDC_DESC_REG, AMAPReg.Data);
	    RAS_DEBUG((4, "StartSparing -  Lockstep Channel:%d  Amap write Value:%x \n", Ch^1, AMAPReg.Data));
    }

    //
    // Setup sparing engine
    //
    spareCtlReg.Data          = MemReadPciCfg( NodeId, PARAM_UNUSED, SPARECTL_MC_MAINEXT_REG);
    spareCtlReg.Bits.chn_sel  = Ch;
    spareCtlReg.Bits.src_rank = SrcRank;
    spareCtlReg.Bits.dst_rank = DstRank;
    MemWritePciCfg( NodeId, PARAM_UNUSED, SPARECTL_MC_MAINEXT_REG, spareCtlReg.Data);

    //
    // Clear Spare Address Lo register
    //
    spareAddrLoReg.Data = MemReadPciCfg( NodeId, PARAM_UNUSED, SPAREADDRESSLO_MC_MAINEXT_REG);
    spareAddrLoReg.Bits.rankadd = 0;
    MemWritePciCfg( NodeId, PARAM_UNUSED, SPAREADDRESSLO_MC_MAINEXT_REG, spareAddrLoReg.Data);
	
    switch(SparingMode){
    case RankSparing:
      //
      // Start Rank Sparing
      //
      spareCtlReg.Bits.spare_enable = 1;
      MemWritePciCfg( NodeId, PARAM_UNUSED, SPARECTL_MC_MAINEXT_REG, spareCtlReg.Data);
      //
      // Record spare in use
      //
      mMemrasS3Param.imc[NodeId].channelList[Ch].spareInUse       = 1;
      mMemrasS3Param.imc[NodeId].channelList[Ch].oldLogicalRank   = SrcRank;
      //mMemrasS3Param.imc[NodeId].channelList[ch].oldLogicalRank[mMemrasS3Param.imc[NodeId].channelList[ch].MultiSpareRankCnt] = SrcRank;
      //(mMemrasS3Param.imc[NodeId].channelList[ch].MultiSpareRankCnt)++;

      break;

    default:
      status = EFI_UNSUPPORTED;   // Invalid sparing mode
      break;
    }
  
  return status;
}

/*++

Routine Description:
  Checks the hardware registers for spare copy completion and returns
  status

Arguments:
  NodeId    - Memory controller ID
  SparingMode - Type of sparing - DDDC / Rank

Returns:

  EFI_SUCCESS if spare copy operation completed without errors
  EFI_DEVICE_ERROR if there is an error in spare copy operation
  EFI_NOT_READY if copy operation has not completed
  EFI_UNSUPPORTED all other errors

--*/
EFI_STATUS
IsCopyComplete(
    IN UINT8 NodeId,
    IN SPARING_MODE SparingMode
 )
{
  EFI_STATUS                        status = EFI_SUCCESS;
  SPARECTL_MC_MAINEXT_STRUCT        spareCtlReg;
  SSRSTATUS_MC_MAINEXT_STRUCT       ssrStatusReg;
  UINT8                             srcRank = 0;
  UINT8                             ddrCh;



    spareCtlReg.Data = MemReadPciCfg( NodeId, PARAM_UNUSED, SPARECTL_MC_MAINEXT_REG);
    srcRank = (UINT8) spareCtlReg.Bits.src_rank;
    ddrCh   = (UINT8) spareCtlReg.Bits.chn_sel;

    switch(SparingMode){
    case RankSparing:
      ssrStatusReg.Data = MemReadPciCfg( NodeId, PARAM_UNUSED, SSRSTATUS_MC_MAINEXT_REG);
      if( ssrStatusReg.Bits.sprinprogress == 0 && ssrStatusReg.Bits.sprcmplt == 1 &&
          spareCtlReg.Bits.spare_enable == 0) {
        status = EFI_SUCCESS;
      } else {
        //
        // Check for errors
        //
        if( spareCtlReg.Bits.spare_enable == 0){
          status = EFI_DEVICE_ERROR;  // Spare copy operation is done, but there are errors
        } else {
          status = EFI_NOT_READY;     // spare copy operation still in progress
        }
      }
      break;

    default:
      status = EFI_UNSUPPORTED;     // invalid sparing mode
      break;
    }
  

  return status;
}
/*++

Routine Description:
  Disables pkg C states.

Arguments:
 Node ID

Returns:

  None

--*/
VOID
DisablePkgC6(
     UINT8 NodeId)
{  
  UINT8                           socket = NODE_TO_SKT(NodeId);
  EFI_STATUS                      Status;

  //
  // If PkgC state is already disabled in the socket, do nothing
  //
  if(!mSocketPkgCSemaphore[socket]){
    // RAS start disables the Pkg C states 
        Status = mCpuCsrAccess->Bios2PcodeMailBoxWrite(socket, MAILBOX_BIOS_CMD_RAS_START, 0);    
        ASSERT_EFI_ERROR(Status);
  }

  //
  // update PkgC semaphore
  //
  mSocketPkgCSemaphore[socket]++;
}

/*++

Routine Description:
  Enables pkg C states.

Arguments:
 Node ID

Returns:

  None

--*/

VOID
EnablePkgC6(
  UINT8 NodeId)
{
  EFI_STATUS                      Status;
  UINT8                           socket = NODE_TO_SKT(NodeId);

  //
  // Reset PkgC states for this NODE
  //
  if(mSocketPkgCSemaphore){
    mSocketPkgCSemaphore[socket]--;
  }

  //
  // Do not enable PkgC states even if 1 MC behind socket has disabled status
  //
  if(!mSocketPkgCSemaphore[socket]){
    // RAS Exit Re-enables the Pkg C states 
    Status = mCpuCsrAccess->Bios2PcodeMailBoxWrite(socket, MAILBOX_BIOS_CMD_EXIT_RAS, 0);
    ASSERT_EFI_ERROR(Status);
  }
}
#endif // GRANGEVILLE_FLAG