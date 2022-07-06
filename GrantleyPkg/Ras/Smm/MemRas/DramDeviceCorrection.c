/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2011 - 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  DramDeviceCorrection.c

Abstract:

  Implements DRAM device correction functionality

--*/

#include <Protocol/MemRasProtocol.h>
#include "MemRas.h"
#include "SparingEngine.h"

extern  SMMELOG_HOST                  mMemrasS3Param;

//
// Device tagging status
//
typedef enum {
  DevTaggingAvailable,        // Device tagging available
  DevTaggingNotAvailable      // Device tagging is not available
} DEV_TAGGING_STATUS;

//
// Device tagging ranks status
//
static
DEV_TAGGING_STATUS mDevTaggingStatus[MC_MAX_NODE][MAX_CH][MAX_RANK_CH];


/*++

Routine Description:
    Initialize all Device tagging  related data structures
    This routine should be called only once during boot

Arguments:

  None

Returns:

    Status

--*/
EFI_STATUS
EFIAPI
InitializeDevTagging(
 )
{
  UINT8 nodeId;
  UINT8 ddrCh;
  UINT8 rank;

  //
  // Initialize all sparing related data structures
  // here
  //
  for(nodeId = 0; nodeId < MC_MAX_NODE; nodeId++) {
    for(ddrCh = 0; ddrCh < MAX_CH; ddrCh++) {
      for(rank = 0; rank < MAX_RANK_CH; rank++ ) {
        //
        // Enable device tagging based on following rules
        // Lock-step | x4 | x8 | Enable Device Tagging
        // --------------------------------------------
        //    X      | Y  | -  |    Y
        //    N      | X  | Y  |    N
        //    Y      | X  | Y  |    Y
        //
        if( (mSystemMemoryMap->NodeInfo[nodeId].ExRasModesEnabled & DEVTAGGING_EN) &&
            mSystemMemoryMap->NodeInfo[nodeId].Enabled &&
             (mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ddrCh].EnabledLogicalRanks & (1 << rank)) &&
              mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ddrCh].Enabled &&
                ((!(mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ddrCh].Features & X8_PRESENT)) ||
                    ((mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ddrCh].Features & X8_PRESENT) &&
                        (mSystemMemoryMap->NodeInfo[nodeId].RasModesEnabled & CH_LOCKSTEP))) ){
          mDevTaggingStatus[nodeId][ddrCh][rank] = DevTaggingAvailable;
          DEBUG ((EFI_D_ERROR, "InitializeDevTagging - Node%d.DdrCh%d.Rank%d - Devtagging Enabled\n", nodeId, ddrCh, rank));
        } else {
          mDevTaggingStatus[nodeId][ddrCh][rank] = DevTaggingNotAvailable;
          DEBUG ((EFI_D_ERROR, "InitializeDevTagging - Node%d.DdrCh%d.Rank%d - Devtagging Disabled\n", nodeId, ddrCh, rank));
        }
      }
    }
  }

  return EFI_SUCCESS;
}

/*++

Routine Description:
    Check for Device Tagging events and handle them

Arguments:

  NodeId    - Memory controller ID
  *IsEventHandled - ptr to buffer to hold event handled status
Returns:

    status - EFI_SUCCESS if no failures
             non-zero error code for failures

    IsEventHandled - TRUE -  event was handled
                     FALSE - event not handled

--*/
static
EFI_STATUS
HandleNewDevTaggingEventsIfAny(
    IN UINT8 NodeId,
    OUT BOOLEAN *IsEventHandled
 )
{
  UINT8                             ddrCh;
  UINT8                             ddrChPartner;
  DEVTAG_CNTL_0_MCDDC_DP_STRUCT     devTagCntlRegPartner;
  UINT8                             rank = 0;
  UINT32                            errStatus;
  EFI_STATUS                        status = EFI_SUCCESS;
  UINT32                            devTagCntlRegBaseAddr;
  DEVTAG_CNTL_0_MCDDC_DP_STRUCT     devTagCntlReg;


  *IsEventHandled       = FALSE;
  devTagCntlRegBaseAddr = DEVTAG_CNTL_0_MCDDC_DP_REG;

  for( ddrCh = 0; ddrCh < MAX_CH; ddrCh++ ){
    if (mSystemMemoryMap->NodeInfo[NodeId].ChannelInfo[ddrCh].Enabled == 0) {
      continue;
    }
    //
    // Check if there is any rank in this channel
    // that has overflow bit set
    //
    for( rank = 0; (rank < MAX_RANK_CH) ; rank++ ){
      //
      // If this ddrCh /rank doesn't have Dev tagging support, skip to next one
      //
      if( mDevTaggingStatus[NodeId][ddrCh][rank] != DevTaggingAvailable ){
        continue;
      }

      if( IsErrorExcdForThisRank( NodeId, ddrCh, rank) ){
        break;
      }
    } // for(rank ...)

    if( rank != MAX_RANK_CH ){
      //
      // We found a rank with error, handle it
      //
      devTagCntlReg.Data = (UINT8) MemReadPciCfg( NodeId, ddrCh, devTagCntlRegBaseAddr + rank );

      if (mSystemMemoryMap->NodeInfo[NodeId].RasModesEnabled & CH_LOCKSTEP) {
        //
        // In Lockstep Mode, ddrCh 0 is "partnered" with ddrCh 1
        //               and ddrCh 2 is "partnered" with ddrCh 3.
        // Copy faildevice from the channel partner and set the en flag.
        //
        // At this point, devTagCntlReg.Bits.faildevice holds the failed device ID.
        //
        RAS_DEBUG((4, "HandleNewDevTaggingEventsIfAny - Device tagging enabled on Primary Ch :%x, nodeId : %x,\n", ddrCh, NodeId));  
        if ((ddrCh & 1) == 0) {
          ddrChPartner = ddrCh + 1;
          if (mDevTaggingStatus[NodeId][ddrChPartner][rank] == DevTaggingAvailable) {
            devTagCntlRegPartner.Data = (UINT8) MemReadPciCfg (NodeId, ddrChPartner, devTagCntlRegBaseAddr + rank);
              devTagCntlRegPartner.Bits.faildevice = devTagCntlReg.Bits.faildevice;
              devTagCntlRegPartner.Bits.en = 1;
              RAS_DEBUG((4, "HandleNewDevTaggingEventsIfAny - Device tagging enabled on Partner Ch :%x, nodeId : %x,\n", ddrChPartner, NodeId));
              MemWritePciCfg (NodeId, ddrChPartner, devTagCntlRegBaseAddr + rank, devTagCntlRegPartner.Data);
              mMemrasS3Param.imc[NodeId].channelList[ddrChPartner].devTag[rank] = devTagCntlRegPartner.Data;
              mDevTaggingStatus[NodeId][ddrChPartner][rank] = DevTaggingNotAvailable;
          }
        }
      }

      devTagCntlReg.Bits.en = 1;
      MemWritePciCfg( NodeId, ddrCh, devTagCntlRegBaseAddr + rank, devTagCntlReg.Data );
      mMemrasS3Param.imc[NodeId].channelList[ddrCh].devTag[rank]  = devTagCntlReg.Data;
      mDevTaggingStatus[NodeId][ddrCh][rank] = DevTaggingNotAvailable;

      //
      // Clear error count registers in CPU and JCK
      //
      ClearErrorsForThisRank( NodeId, ddrCh, rank );

      //
      // Update internal status & event handled status
      //
      errStatus = 0;
      GetOrSetErrorHandlingStatus( NodeId, UpdateEventPendingStatus, &errStatus );
      *IsEventHandled = TRUE;
      break; // exit from DDRCH loop
    }
  } // for(ddrCh ...)
  return status;
}


/*++

Routine Description:
    handle rank tag for pending event.

Arguments:

  NodeId    - Memory controller ID
  Ch  - Channel
  Rank - rank

Returns:

    None

--*/
VOID
EFIAPI
HandleDevTagForPendingEvent(
  IN  UINT8 NodeId,
  IN  UINT8 Ch,
  IN  UINT8 Rank
){
  UINT32                            devTagCntlRegBaseAddr;
  DEVTAG_CNTL_0_MCDDC_DP_STRUCT     devTagCntlReg;
  DEVTAG_CNTL_0_MCDDC_DP_STRUCT     devTagCntlRegPartner;
  UINT8                             ChPartner;

  devTagCntlRegBaseAddr = DEVTAG_CNTL_0_MCDDC_DP_REG;

  RAS_DEBUG((4, "HandleDevTagForPendingEvent - \n")); 

  if(mSystemMemoryMap->NodeInfo[NodeId].ExRasModesEnabled & DEVTAGGING_EN){
    if( mDevTaggingStatus[NodeId][Ch][Rank] == DevTaggingAvailable ){

      devTagCntlReg.Data = (UINT8) MemReadPciCfg( NodeId, Ch, devTagCntlRegBaseAddr + Rank );

        RAS_DEBUG((4, "HandleDevTagForPendingEvent - Device tagging enabled on Primary Ch :%x, nodeId : %x,\n", Ch, NodeId));
        if (mSystemMemoryMap->NodeInfo[NodeId].RasModesEnabled & CH_LOCKSTEP) {
          if ((Ch & 1) == 0) {
            ChPartner = Ch ^ 1;
            if (mDevTaggingStatus[NodeId][ChPartner][Rank] == DevTaggingAvailable) {
              devTagCntlRegPartner.Data = (UINT8) MemReadPciCfg(NodeId, ChPartner, devTagCntlRegBaseAddr + Rank);
                devTagCntlRegPartner.Bits.faildevice = devTagCntlReg.Bits.faildevice;
                devTagCntlRegPartner.Bits.en = 1;
                RAS_DEBUG((4, "HandleDevTagForPendingEvent - Device tagging enabled on Partner Ch :%x, nodeId : %x,\n", ChPartner, NodeId));
                MemWritePciCfg (NodeId, ChPartner, devTagCntlRegBaseAddr + Rank, devTagCntlRegPartner.Data);
                mDevTaggingStatus[NodeId][ChPartner][Rank] = DevTaggingNotAvailable;
            }
          }
        }

        devTagCntlReg.Bits.en = 1;
        MemWritePciCfg( NodeId, Ch, devTagCntlRegBaseAddr + Rank, devTagCntlReg.Data );
        mDevTaggingStatus[NodeId][Ch][Rank] = DevTaggingNotAvailable;
    }
  }
}

/*++

Routine Description:
    Check for Device Tagging events and handle them

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
CheckAndHandleDevTagging(
    IN UINT8 NodeId,
    IN EVENT_TYPE EventType,
    OUT BOOLEAN *IsEventHandled
 )
{
  EFI_STATUS  status;

  if(mSystemMemoryMap->NodeInfo[NodeId].ExRasModesEnabled & DEVTAGGING_EN){

    switch( EventType ){
    case EVENTS_IN_PROGRESS:
      //
      // DEVICE tagging events cannot be 'IN PROGRESS'
      // indicate that event as not handled always
      //
      status          = EFI_SUCCESS;
      *IsEventHandled = FALSE;
      break;

    case NEW_EVENTS:
      status = HandleNewDevTaggingEventsIfAny( NodeId, IsEventHandled );
      break;

    default:
      status = EFI_UNSUPPORTED;     // Invalid event type
      break;
    }
  } else {
    //
    // If device tagging is not enabled, do nothing
    //
    *IsEventHandled = FALSE;
    status          = EFI_SUCCESS;
  }
  return status;
}


