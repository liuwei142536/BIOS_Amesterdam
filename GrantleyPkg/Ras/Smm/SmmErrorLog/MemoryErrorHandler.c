/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//

Copyright (c) 2009 - 2017 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  MemoryErrorHandler.c

Abstract:
  This file will contain all definitions related to Memory Error Handler
  
------------------------------------------------------------------*/
#include <Token.h> // AptioV server override

#include "CommonErrorHandlerIncludes.h"

extern SYSTEM_CONFIGURATION         setupData;
//AptioV Server Override Start: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions
//extern EFI_SMM_VARIABLE_PROTOCOL    *mSmmVariable;
extern EFI_RUNTIME_SERVICES         *gSmmpRs;
//AptioV Server Override End: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions

UINT8   mHaCmciOnCorrecetdThr;
UINT8   mIvtCpuEx   = 01;
//UINT8   mSavedLinkWidth[MC_MAX_NODE][MAX_VMSE_CH];
//UINT16  mPersistentErrCnt[MC_MAX_NODE][MAX_VMSE_CH];
//UINT32  mVmseErrCnt[MC_MAX_NODE][MAX_VMSE_CH];
//UINT16  mTransientErrOverFlowCnt[MC_MAX_NODE][MAX_VMSE_CH];
UINT8   mFailedDevice[MC_MAX_NODE][MAX_CH][MAX_RANK_CH];
//UINT8   mAvailableLinkWidth[MC_MAX_NODE][MAX_VMSE_CH];

//#define HalfWidthA            01//BIT0
//#define HalfWidthB            02//BIT1
//#define HalfWidthC            04//BIT2
//#define HalfWidthABC          0x7 // = HalfWidthA + HalfWidthB + HalfWidthC;

//#define VMSE_NB_HALF_WIDTH_A  0x07
//#define VMSE_NB_HALF_WIDTH_B  0x06
//#define VMSE_NB_HALF_WIDTH_C  0x05

#define MSCOD_DDR4_CAP_ERROR 0x0200

static
MEM_RAS_EVENT_HNDLR mRasEventHndlrTable[] = {
    //HandleDddc,
#ifndef GRANGEVILLE_FLAG   
    HardErrorDetection,
    HandleRankSparing,
#endif    // GRANGEVILLE_FLAG
    HandleDevTagging,
    //HandleSpareNodeMigration,
    HandleDefaultMemoryError,  // Aptio V Server Override : Changes done to handle the Memory Correctable Erros (Default Memory Handler) when RankSparing/DevTagging setup option is disabled. 
    ClearUnhandledCorrError,
    END_OF_TABLE
};

UINT8 mCSMIEnNode[MAX_SOCKET];
// AptioV Server Override Start: Support added for Mode1 Memory Error Reporting
UINT64 				McStatusMsr;
UINT64				McMiscMsr;
// AptioV Server Override End: Support added for Mode1 Memory Error Reporting

// AptioV Server Override Start : Changes done to resolve the MC7Status MSR read correctly based  on the failing CPU's
UINT64				MC7Status ;
UINTN				HAMsrIndex;
UINT64				Mc7Addr;
// AptioV Server Override End : Changes done to resolve the MC7Status MSR read correctly based  on the failing CPU's

// Aptio V Server Override Start : Changes done to find the Rank/Channel/DIMM information when Default Memory Handler Code gets control.

BOOLEAN	gGetErrInfo = FALSE;

UINT8
ConvertLogicalToPhysicalRankId(
    IN UINT8  NodeId,
    IN UINT8  DdrCh,
    IN UINT8  LogicalRank
 )
/**

Routine Description:
   Convert Logical to Physical rank ID

Arguments:
    NodeId
    DdrCh
    Logical Rank

Returns:
  Physical Rank Id

--*/
{
  UINT8   csMapping2DPC[] = {0, 1, 2, 3, 4, 5, 6, 7};
  UINT8   csMapping3DPC[] = {0, 1, 2, 3, 4, 5, 8, 9};
  UINT8   phyRank = 0xFF;

  if( mMemRas->SystemMemInfo->NodeInfo[NodeId].ChannelInfo[DdrCh].MaxDimm == 3){
    if (LogicalRank >= 0 && LogicalRank < 8) {
        phyRank = csMapping3DPC[LogicalRank];
    }    
  } 
  else {
    if (LogicalRank >= 0 && LogicalRank < 8) {
        phyRank = csMapping2DPC[LogicalRank];
    }       
  }

  RAS_DEBUG((4,"ConvertLogicalToPhysicalRankId  logical rank:%d -->physical rank:%d\n",LogicalRank,phyRank));
  return phyRank;
}

BOOLEAN
GetErrInfo(
    IN UINT8                          NodeId,
    IN UINT8                          *DdrCh,
    IN UINT8                          *Dimm,
    IN UINT8                          *Rank
)
{

  UINT8       ch;
  UINT8       rank;
  UINT8       dimmSlot = 0;
  UINT8       PhysicalRank;
  CORRERRORSTATUS_MCDDC_DP_HSX_STRUCT corrErrStsReg;

  for( ch = 0; ch < MAX_CH; ch++ ){
    if (mELogHost.imc[NodeId].channelList[ch].chEnabled == 0) continue;
    //
    // Check if there is any rank in this channel
    // that has overflow bit set
    //
    for( rank = 0; (rank < MAX_RANK_CH) ; rank++ ){
      //
      // Check error status in CPU for this rank
      //
      corrErrStsReg.Data = mMemRas->MemReadPciCfg( NodeId, ch, CORRERRORSTATUS_MCDDC_DP_REG );
      if( corrErrStsReg.Bits.err_overflow_stat & (1 << rank)){
        PhysicalRank = ConvertLogicalToPhysicalRankId(NodeId, ch, rank);
        dimmSlot = PhysicalRank / MAX_RANK_DIMM;
        *Dimm = dimmSlot;
        *Rank = PhysicalRank % MAX_RANK_DIMM;
        *DdrCh = ch;
        return TRUE;
      }
    }
  }
  return FALSE;
}

// Aptio V Server Override End : Changes done to find the Rank/Channel/DIMM information when Default Memory Handler Code gets control.

// Aptio V Server Override  Start: Changes done to handle the Memory Correctable Erros (Default Memory Handler) when RankSparing/DevTagging setup option is disabled. 
/*++

Routine Description:
	Handle Default Memory Handlers

Arguments:

  NodeID		  - Memory controller ID
  EventType 	  - In progress / new events
  *IsEventHandled - ptr to buffer (BOOLEAN) to get Event handled status
Returns:

	status - EFI_SUCCESS - no error
			 non-zero code - if error
	IsEventHandled - Event handled status - TRUE/ FALSE

--*/
static
EFI_STATUS
HandleDefaultMemoryError (
  IN  UINT8       NodeId,
	IN  EVENT_TYPE  EventType,
	IN OUT BOOLEAN  *IsEventHandled
	)
{
// Need to check 	GLOBAL_RAS_POLICY later
// If any of these setup options dddcSupport,DeviceTaggingMode,enableSparing are enabled then no need handle the errors in defaultMemoryError handler code.
	if (mErrLogSetupData->dddcSupport || mErrLogSetupData->DeviceTaggingMode || mErrLogSetupData->enableSparing )
	{
		*IsEventHandled = FALSE;
	}
// If any of these setup options dddcSupport,DeviceTaggingMode,enableSparing are not enabled then need to handle the errors in defaultMemoryError handler code.	
	else
	{
		gGetErrInfo = TRUE;
	    *IsEventHandled = TRUE;
	}
	 return EFI_SUCCESS;
}
// Aptio V Server Override End : Changes done to handle the Memory Correctable Erros (Default Memory Handler) when RankSparing/DevTagging setup option is disabled. 


DIMM_ISOLATION_SKIP_DESC mSkipDimmIsolation[] = {
  // MCACOD , MCACODMASK, MSCOD, MSCODMASK
  { IMC_PATROL_SCRUB_ERR_MCACOD, IMC_PATROL_SCRUB_ERR_MCACOD_MSK, IMC_UNC_PATROL_SCRUB_ERR_MSCOD, IMC_PATROL_SCRUB_ERR_MSCOD_MSK},
  { IMC_PATROL_SCRUB_ERR_MCACOD, IMC_PATROL_SCRUB_ERR_MCACOD_MSK, IMC_COR_PATROL_SCRUB_ERR_MSCOD, IMC_PATROL_SCRUB_ERR_MSCOD_MSK},
  { IMC_SPARING_ERR_MCACOD, IMC_SPARING_ERR_MCACOD_MSK, IMC_UNC_SPARING_ERR_MSCOD, IMC_SPARING_ERR_MSCOD_MSK},
  { IMC_SPARING_ERR_MCACOD, IMC_SPARING_ERR_MCACOD_MSK, IMC_COR_SPARING_ERR_MSCOD, IMC_SPARING_ERR_MSCOD_MSK},
  { IMC_CMD_ADD_PARITY_ERR_MCACOD, IMC_CMD_ADD_PARITY_ERR_MCACOD_MSK, IMC_CMD_ADD_PARITY_ERR_MSCOD, IMC_CMD_ADD_PARITY_ERR_MSCOD_MSK}
};

UINTN mSkipDimmIsolationEntries = sizeof(mSkipDimmIsolation)/sizeof(DIMM_ISOLATION_SKIP_DESC);

VOID
UpdateFailDeviceId(
  UINT8                          NodeId,
  UINT8                          DdrCh,
  UINT8                          Rank
 );

/*++

Routine Description:
  Handle DDDC

Arguments:

  NodeID      - Memory controller ID
  EventType     - In progress / new events
  *IsEventHandled - ptr to buffer (BOOLEAN) to get Event handled status
Returns:

  status - EFI_SUCCESS - no error
       non-zero code - if error
  IsEventHandled - Event handled status - TRUE/ FALSE

--*/
/*static
EFI_STATUS
HandleDddc(
  IN  UINT8       NodeId,
  IN  EVENT_TYPE  EventType,
  IN OUT BOOLEAN  *IsEventHandled
  )
{
  return mMemRas->CheckAndHandleDddc(  NodeId, EventType, IsEventHandled );
}*/


#ifndef GRANGEVILLE_FLAG

/*++

Routine Description:
  Handle Hard Error Detection

Arguments:

  NodeID      - Memory controller ID
  EventType     - In progress / new events
  *IsEventHandled - ptr to buffer (BOOLEAN) to get Event handled status
Returns:

  IsEventHandled - Event handled status - TRUE/ FALSE
  IsEventHandled - TRUE - if HED is enabled
  IsEventHandled - FALSE - if hed is not enabled

--*/

static
EFI_STATUS
HardErrorDetection(
  IN  UINT8     NodeId,
  IN  EVENT_TYPE  EventType,
  IN OUT BOOLEAN  *IsEventHandled
  )
{ 
  *IsEventHandled = FALSE;
  if (mIioUds->SystemStatus.cpuType == CPU_BDX) {
    RAS_DEBUG((1, "[RAS]Hard Error Detection \n"));
    return mMemRas->CheckAndHandleHardErrorDetection(  NodeId, EventType, IsEventHandled );
  }
  return *IsEventHandled;
}
/*++

Routine Description:
  Handle RankSparing Events

Arguments:

  NodeID      - Memory controller ID
  EventType     - In progress / new events
  *IsEventHandled - ptr to buffer (BOOLEAN) to get Event handled status
Returns:

  status - EFI_SUCCESS - no error
       non-zero code - if error
  IsEventHandled - Event handled status - TRUE/ FALSE

--*/

static
EFI_STATUS
HandleRankSparing(
  IN  UINT8     NodeId,
  IN  EVENT_TYPE  EventType,
  IN OUT BOOLEAN  *IsEventHandled
  )
{
  return mMemRas->CheckAndHandleRankSparing(  NodeId, EventType, IsEventHandled );
}
#endif // GRANGEVILLE_FLAG
/*++

Routine Description:
  Handle Spare Node migration

Arguments:

  NodeID      - Memory controller ID
  EventType     - In progress / new events
  *IsEventHandled - ptr to buffer (BOOLEAN) to get Event handled status
Returns:

  status - EFI_SUCCESS - no error
       non-zero code - if error
  IsEventHandled - Event handled status - TRUE/ FALSE

--*/

/*
static
EFI_STATUS
HandleSpareNodeMigration(
  IN  UINT8     NodeId,
  IN  EVENT_TYPE  EventType,
  IN OUT BOOLEAN  *IsEventHandled
  )
{
  *IsEventHandled = FALSE;

  //
  // TODO:
  // Handle Spare node migration when supporting functionality
  // is implemented
  //
  // mMemRas->CheckAndHandleSpareNodeMigration( NodeId, EventType, IsEventHandled );
  //
  return EFI_SUCCESS;
}
*/


/*++

Routine Description:
  Handle Device Tagging Events

Arguments:

  NodeID      - Memory controller ID
  EventType     - In progress / new events
  *IsEventHandled - ptr to buffer (BOOLEAN) to get Event handled status
Returns:

  status - EFI_SUCCESS - no error
       non-zero code - if error
  IsEventHandled - Event handled status - TRUE/ FALSE

--*/
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

/*++

Routine Description:
  Handle indepencent mode correctable error with device tagging disabled.

Arguments:

  NodeID    - Memory controller ID
  EventType     - In progress / new events
  *IsEventHandled - ptr to buffer (BOOLEAN) to get Event handled status
Returns:

  status - EFI_SUCCESS - no error
       non-zero code - if error
  IsEventHandled - Event handled status - TRUE/ FALSE

--*/
static
EFI_STATUS
ClearUnhandledCorrError(
  IN  UINT8     NodeId,
  IN  EVENT_TYPE  EventType,
  IN OUT BOOLEAN  *IsEventHandled
  )
{
      RAS_DEBUG((1, "[RAS]ClearUnhandledCorrError\n"));
      return mMemRas->ClearUnhandledCorrError(NodeId, EventType, IsEventHandled);
}
EFI_STATUS
EFIAPI
DisableCSMI(VOID) 
{
  //UINTN cpu;
  UINT8 node;
  UINT8 socket= 0;
  EFI_STATUS Status;
  CMCI_GEN2_PARAMS Params;
  UINT8 core;
  UINTN cpuNumber;
#if defined(HSX_HOST) || defined(BDX_HOST)
  SMICTRL_UBOX_MISC_HSX_BDX_STRUCT uboxSMICtrlReg;
#endif
  DEBUG ((DEBUG_ERROR, "DisableCSMI Start \n"));	 
//EFI_DEADLOOP();
  if(mErrLogSetupData->EMcaCSmiEn == 1) {

    if(isEMCAGen2Cap()) {

      for(socket = 0; socket < MAX_SOCKET; socket++) {
        //
        // Skip if socket not enabled
        //
        if ((mELogHost.socketPresentBitMap & (1 << socket)) == 0) {
          continue;
        }
        
#if defined(HSX_HOST) || defined(BDX_HOST)
        //*DISABLE* generation of CSMI
        uboxSMICtrlReg.Data = mMemRas->MemReadPciCfg(socket, 0, SMICTRL_UBOX_MISC_REG);
        uboxSMICtrlReg.Bits.smidis4 = 1;
        mMemRas->MemWritePciCfg(socket, 0, SMICTRL_UBOX_MISC_REG, uboxSMICtrlReg.Data);
#endif

        InitializeSpinLock(&Params.Busy);
        for (core = 0; core < MAXCOREID; core++) {
        if (ExcutingOnLocalNodeCore (socket, core) == TRUE) {
          DisableCSMIGen2(&Params);
        } else {
          if(mELogHost.cpuInfo[socket].Valid[core][0]) {
            cpuNumber = GetCpuNumber (socket, core, 0);
            AcquireSpinLock(&Params.Busy);
            Status = mSmst->SmmStartupThisAp (DisableCSMIGen2,
                                                    cpuNumber,
                                                    &Params);
            if(Status == EFI_SUCCESS) {
              AcquireSpinLock(&Params.Busy);  
            }
            ReleaseSpinLock(&Params.Busy);    
          }
        }
      }
    }
    DEBUG ((DEBUG_ERROR, "end of emcagen2 disabled \n"));
    } else {
      for(socket=0; socket < MAX_SOCKET; socket++) {
        //
        // Skip if socket not enabled
        //
        if ((mELogHost.socketPresentBitMap & (1 << socket)) == 0) {
          continue;
        }
#if defined(HSX_HOST) || defined(BDX_HOST)
        //grantley - s4986148: Memory error counter overflow causes both SMI and CSMI 
        //*DISABLE* generation of CSMI 
        if((mELogHost.CpuFamily== CPU_HSX) && (mELogHost.CpuStepping < C0_REV_HSX)) {
          uboxSMICtrlReg.Data = mMemRas->MemReadPciCfg(  socket, 0, SMICTRL_UBOX_MISC_REG);
          uboxSMICtrlReg.Bits.smidis4 = 1;
          mMemRas->MemWritePciCfg(socket, 0, SMICTRL_UBOX_MISC_REG, uboxSMICtrlReg.Data);
        }
#endif
        for( node = SKTMC_TO_NODE(socket, 0); node <= SKTMC_TO_NODE(socket, (MAX_HA-1)); node++) {
          if ( mELogHost.imc[node].enabled == 0 ) continue;
          DisableCSMIGen1(&node);
        }
      }
    }
  DEBUG ((DEBUG_ERROR, "end of emca csmi disabled \n"));
  }
return EFI_SUCCESS;
}
EFI_STATUS
EFIAPI
EnableCSMI(
VOID
)
{
  UINTN cpu;
  UINT8 node;
  UINT8 socket  = 0;
  EFI_STATUS Status;
  CMCI_GEN2_PARAMS Params;
  #if defined(HSX_HOST) || defined(BDX_HOST)
  SMICTRL_UBOX_MISC_HSX_BDX_STRUCT uboxSMICtrlReg;
  #endif
  
  if(mErrLogSetupData->EMcaCSmiEn == 1) {

    if(isEMCAGen2Cap()) {
#if defined(HSX_HOST) || defined(BDX_HOST) 
      for(socket=0; socket < MAX_SOCKET; socket++) {
        //
        // Skip if socket not enabled
        //
        if ((mELogHost.socketPresentBitMap & (1 << socket)) == 0) {
          continue;
        }
        //*ENABLE* generation of CSMI
        uboxSMICtrlReg.Data = mMemRas->MemReadPciCfg(socket, 0, SMICTRL_UBOX_MISC_REG);
        uboxSMICtrlReg.Bits.smidis4 = 0;
        mMemRas->MemWritePciCfg(socket, 0, SMICTRL_UBOX_MISC_REG, uboxSMICtrlReg.Data);
      }
#endif
      InitializeSpinLock(&Params.Busy);

      for(cpu=0;cpu < mSmst->NumberOfCpus; cpu++) {
        if(cpu == mSmst->CurrentlyExecutingCpu) {
          EnableCSMIGen2(&Params);
        } else {
          AcquireSpinLock(&Params.Busy);
          Status = mSmst->SmmStartupThisAp(EnableCSMIGen2,
                                           cpu,
                                           &Params);
          if(Status == EFI_SUCCESS) {
            AcquireSpinLock(&Params.Busy);
          }
          ReleaseSpinLock(&Params.Busy);
        }
      }
    } else {
      for(socket=0; socket < MAX_SOCKET; socket++) {
        //
        // Skip if socket not enabled
        //
        if ((mELogHost.socketPresentBitMap & (1 << socket)) == 0) {
          continue;
        }
#if defined(HSX_HOST) || defined(BDX_HOST)
        //grantley - s4986148: Memory error counter overflow causes both SMI and CSMI 
        //*DISABLE* generation of CSMI 
        if((mELogHost.CpuFamily == CPU_HSX) && (mELogHost.CpuStepping < C0_REV_HSX)) {
          uboxSMICtrlReg.Data = mMemRas->MemReadPciCfg(  socket, 0, SMICTRL_UBOX_MISC_REG);
          uboxSMICtrlReg.Bits.smidis4 = 1;
          mMemRas->MemWritePciCfg(socket, 0, SMICTRL_UBOX_MISC_REG, uboxSMICtrlReg.Data);
        }
#endif
        for( node = SKTMC_TO_NODE(socket, 0); node <= SKTMC_TO_NODE(socket, (MAX_HA-1)); node++) {
          if ( mELogHost.imc[node].enabled == 0 ) continue;
          EnableCSMIGen1(&node);
        }
      }
    }
  }
  return EFI_SUCCESS;
}


EFI_STATUS
EnableElogMemory (
  VOID
  )
/*++

Routine Description:

  Enables or didsables the memory and HA specifc error reporting based on the ErrorLogSetupData structure.

Arguments:

    N/A

Returns:

    EFI_SUCCESS if the call is succeed

--*/
{

  UINT8       socket  = 0;
  UINT8       ch      = 0;
  UINT8       rank    = 0;
  UINT8       node    = 0;
  RETRY_RD_ERR_LOG_MCDDC_DP_STRUCT RetryRdErrLog;

  //
  // Used for corrected error SMI throttling
  //
  mEccErrorSmiCount = 0;

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Skip if socket not enabled
    //
    if ((mELogHost.socketPresentBitMap & (1 << socket)) == 0) {
      continue;
    }

    for( node = SKTMC_TO_NODE(socket, 0); node <= SKTMC_TO_NODE(socket, (MAX_HA-1)); node++){
    if ( mELogHost.imc[node].enabled == 0 ) continue;

      //
      // Init timestamps for corrected error rate SMI throttling
      //
      for (ch = 0; ch < MAX_CH; ch++) {
        if (mELogHost.imc[node].channelList[ch].chEnabled == 0) {
          continue;
        }
        RetryRdErrLog.Data = 0;

        //Enable log of HA and Also sparing/patrol scrub errors in the RETRY_RD_ERROR_LOG CSR
        RetryRdErrLog.Bits.en = 1;
        RetryRdErrLog.Bits.en_patspr = 1;
        mMemRas->MemWritePciCfg(node,
                                ch,
                                RETRY_RD_ERR_LOG_MCDDC_DP_REG,
                                RetryRdErrLog.Data
                                );
        for (rank = 0; rank < MAX_RANK_CH; rank++) {
          mCorrectedErrorRankTimeStamp[node][ch][rank].Year = 0;
        }
      }

      if( mELogHost.imc[node].enabled == 0 ){
        continue;
      }

      //
      // Init IMC correctable errors / Enable SMI routing
      //
      InitIMCCorrectableError (node);
      //
      // Enable VMSE errors.
      //
      //EnableVmseChErrors(node);

    } // node
  } // socket

  return EFI_SUCCESS;
}



INT16 Crc16 (char *ptr, int count)
{
  INT16 crc, i;
  crc = 0;
  while (--count >= 0)
  {
    crc = crc ^ (INT16)(int)*ptr++ << 8;
    for (i = 0; i < 8; ++i)
    {
      if (crc & 0x8000)
      {
        crc = crc << 1 ^ 0x1021;
      }
      else
      {
        crc = crc << 1;
      }
    }
  }
  return (crc & 0xFFFF);
}

void
UpdatePprAddress(
    UINT8 NodeId,
    UINT8 ChannelId,
    UINT8 DimmSlot,
    UINT8 DimmRank,
    UINT8 BankGroup,
    UINT8 Bank,
    UINT32 Row,
    UINT8 ChipSelect
  )
//
// IF PPR testing is enabled, update PPR err address
//
{
  UINT8             i;
  EFI_STATUS        status;
  UINTN             varSize;
  PPR_ADDR_VARIABLE pprData;
  UINT16            oldCrc;
  UINT16            curCrc;
  UINT8             targetFailDevId;
  UINT8             tgtDdrCh;

  varSize = sizeof(PPR_ADDR_VARIABLE);
  if (setupData.pprType && setupData.pprErrInjTest) {
//AptioV Server Override Start: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions	  
/*
    status = mSmmVariable->SmmGetVariable (
                                PPR_VARIABLE_NAME,
                                &gEfiPprVariableGuid,
                                NULL,
                                &varSize,
                                &pprData
                                );*/
    status = gSmmpRs->GetVariable(
		        PPR_VARIABLE_NAME,
                        &gEfiPprVariableGuid,
                        NULL,
                        &varSize,
                        &pprData
                        );
//AptioV Server Override End: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions	  
    oldCrc = pprData.crc16;
    pprData.crc16 = 0;
    curCrc = Crc16 ((char*)&pprData, varSize);
    if (EFI_ERROR(status) || (oldCrc != curCrc)) {
      //
      // Variable is not present. Initialize the data structure to all 0
      //
      ZeroMem (&pprData, varSize);
      DEBUG ((EFI_D_ERROR | EFI_D_INFO, "[PPR] Var not found or crc mismatch! Zero-out struct - oldcrc:0x%0x, curCrc:0x%0x\n", oldCrc, curCrc));
    }

    for (i = 0; i < MAX_PPR_ADDR_ENTRIES; i++) {
      if (!(pprData.pprAddrSetup[i].pprAddrStatus & PPR_STS_ADDR_VALID)) {
        pprData.pprAddrSetup[i].pprAddrStatus |= PPR_STS_ADDR_VALID;
        pprData.pprAddrSetup[i].socket  = NODE_TO_SKT(NodeId);
        pprData.pprAddrSetup[i].mc      = NODE_TO_MC(NodeId);
        tgtDdrCh = ChannelId;
        pprData.pprAddrSetup[i].dimm    = DimmSlot;
        pprData.pprAddrSetup[i].rank    = DimmRank;
        pprData.pprAddrSetup[i].bank    = (BankGroup << 2) | Bank;
        pprData.pprAddrSetup[i].row     = Row;
        pprData.pprAddrSetup[i].subRank = ChipSelect;
        UpdateFailDeviceId (NodeId, ChannelId, (DimmSlot * MAX_RANK_DIMM) + DimmRank);

        if (mFailedDevice[NodeId][ChannelId][(DimmSlot * MAX_RANK_DIMM) + DimmRank] < 0x3F) {
          //
          // Correctable error, valid device ID
          //
          if(mMemRas->SystemMemInfo->NodeInfo[NodeId].RasModesEnabled & LOCKSTEP_EN) {
            //
            // In case of Lockstep, fail device id for secondary CH is logged in the primary CH register
            // Need to adjust the fail device ID and the CH ID accordingly
            //
            targetFailDevId = mFailedDevice[NodeId][ChannelId][(DimmSlot * MAX_RANK_DIMM) + DimmRank];
            //
            // Device to DDRCH mapping
            // x4 -> DEV0-17 DDRCH0; DEV18-35 DDRCH1;
            // x8 -> DEV0-8  DDRCH0; DEV09-17 DDRCH1;
            //
            if(mMemRas->SystemMemInfo->NodeInfo[NodeId].ChannelInfo[ChannelId].DimmInfo[DimmSlot].X4Present){
              //
              // X4 DIMM
              //
              if(targetFailDevId > 17){
                tgtDdrCh++;
                targetFailDevId -= 18;
              }
            } else {
              //
              // X8 DIMM
              //
              if(targetFailDevId > 8){
                tgtDdrCh++;
                targetFailDevId -= 9;
              }
            } // if x4
            pprData.pprAddrSetup[i].dramMask = 1 << targetFailDevId;
          } else {
            pprData.pprAddrSetup[i].dramMask = 1 << mFailedDevice[NodeId][ChannelId][(DimmSlot * MAX_RANK_DIMM) + DimmRank];
          }
        } else {
          //
          // Uncorrectable error; failing devices should be found during PPR
          //
          pprData.pprAddrSetup[i].dramMask = 0;
        }
        pprData.pprAddrSetup[i].ch = tgtDdrCh;
        DEBUG ((EFI_D_ERROR | EFI_D_INFO, "[PPR] Error logged - Entry: %d \n", i));
        DEBUG ((EFI_D_ERROR | EFI_D_INFO, "[PPR] Address-Socket[%d].MC[%d].CH[%d].D[%d].R[%d].CID[%d].Bank[%d].Row[%d].FailDevMask = 0x%0x\n",
            pprData.pprAddrSetup[i].socket,
            pprData.pprAddrSetup[i].mc,
            pprData.pprAddrSetup[i].ch,
            pprData.pprAddrSetup[i].dimm,
            pprData.pprAddrSetup[i].rank,
            pprData.pprAddrSetup[i].subRank,
            pprData.pprAddrSetup[i].bank,
            pprData.pprAddrSetup[i].row,
            pprData.pprAddrSetup[i].dramMask));

        //
        // Update CRC and update the variable
        //
        pprData.crc16 = 0;
        pprData.crc16 = Crc16 ((char*)&pprData, varSize);
//AptioV Server Override Start: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions	  
/*        
        status = mSmmVariable->SmmSetVariable (
                                PPR_VARIABLE_NAME,
                                &gEfiPprVariableGuid,
                                EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                                varSize,
                                &pprData
                                );
*/       
        status = gSmmpRs->SetVariable(
        			      PPR_VARIABLE_NAME,
                	              &gEfiPprVariableGuid,
                	              EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                	              varSize,
                	              &pprData 
                	              );
//AptioV Server Override End: EFI SMM Variable Protocol is not installed.Using AMI NVRAM Runtime Get/Variable functions	                  	              );
        
        if (EFI_ERROR(status)) {
          DEBUG ((EFI_D_ERROR | EFI_D_INFO, "PPR: Var Update for PPR new entry failed - status: %d\n",
              "SetVariable failed",
              status)
          );
        }
        break;
      }
    } // for loop
  } // if pprType and pprErrInjTest
}

EFI_STATUS
FillPlatformMemorySection
(
  UINT8 McBank,
  UINT8 *skt,
  UINT32 *Sev,
  EMCA_MC_SIGNATURE_ERR_RECORD *McErrSig,
  PLATFORM_MEMORY_ERROR_DATA_UEFI_231 *MemSection
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8 SocketId = 00;
  UINT8 core = 00;
  UINT8 MemoryControllerId = 00;
  UINT8 ChannelId = 00;
  UINT8 DimmSlot = 00;
  UINT8 DimmRank = 00;
  UINT32 Row = 00;
  UINT32 Col = 00;
  UINT8  Bank = 00;
  UINT8  BankGroup = 00;
  UINT8 ChipSelect = 00;

  IA32_MCI_STATUS_MSR_STRUCT MsrIa32MCiStatus;
  BOOLEAN DimmIsolation = FALSE;
  UINTN i;

  if(MemSection == NULL || McErrSig == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SetMem(MemSection, sizeof(PLATFORM_MEMORY_ERROR_DATA_UEFI_231), 0);
  ExtractNodeCoreFromApicId (&SocketId, &core); //TODO: Implement function to get socketid, core, using apicid from the signature.
  MemSection->Node = SocketId;
  MemSection->ValidFields |= PLATFORM_MEM_NODE_VALID; 
  MemSection->ErrorType = PLATFORM_MEM_ERROR_UNKNOWN;


  MsrIa32MCiStatus.Data = McErrSig->Signature.McSts;
  if(MsrIa32MCiStatus.Bits.addrv == 1 && MsrIa32MCiStatus.Bits.miscv == 1) {
    MemSection->PhysicalAddress = MCAddress(McErrSig->Signature.McAddr, McErrSig->Signature.McMisc); 
    MemSection->ValidFields |= PLATFORM_MEM_PHY_ADDRESS_VALID;
    DimmIsolation = TRUE;

    //Error types for which address traslation is not appropiate
    for(i=0;i < mSkipDimmIsolationEntries;i++) {
      if( ((MsrIa32MCiStatus.Bits.mca_code & mSkipDimmIsolation[i].McaCodMsk) == mSkipDimmIsolation[i].McaCod) &&
          ((MsrIa32MCiStatus.Bits.ms_code & mSkipDimmIsolation[i].MsCodMsk) == mSkipDimmIsolation[i].MsCod)
        ) {
    DimmIsolation   = FALSE;
    }
    }

    if(DimmIsolation) {
      Status = mMemRas->SystemAddressToDimmAddress (MemSection->PhysicalAddress,&SocketId,&MemoryControllerId,&ChannelId,&DimmSlot,&DimmRank,&Row,&Col,&Bank,&BankGroup, &ChipSelect);
    }
  
    if(DimmIsolation == TRUE  && Status == EFI_SUCCESS) {
  
      MemSection->Node = SKTMC_TO_NODE(SocketId, MemoryControllerId);
      MemSection->Card = ChannelId;
      MemSection->RankNumber = DimmRank;
      MemSection->ModuleRank = DimmSlot;
      MemSection->Column = Col;
      MemSection->Bank = Bank;
      MemSection->Row = Row; 
      
      //
      // Update PPR Address if enabled
      //
      UpdatePprAddress( MemSection->Node, ChannelId, DimmSlot, DimmRank, BankGroup, Bank, Row, ChipSelect);

      MemSection->ValidFields |= PLATFORM_MEM_NODE_VALID           | 
                                 PLATFORM_MEM_CARD_VALID           |
                                 PLATFORM_MEM_MODULE_VALID         |
                                 PLATFORM_MEM_ROW_VALID            |
                                 PLATFORM_MEM_COLUMN_VALID         |
                                 PLATFORM_MEM_BANK_VALID           |
                                 PLATFORM_RANK_NUMBER_VALID ;  
    }
  }
  
  Status = MCBankCsrInfoHook(McBank, skt, McErrSig, MemSection);
  
      if(mSmbiosProtocol != NULL) {
    if((MemSection->ValidFields & PLATFORM_MEM_NODE_VALID) != 0 &&
       (MemSection->ValidFields & PLATFORM_MEM_CARD_VALID) != 0 &&
       (MemSection->ValidFields & PLATFORM_MEM_MODULE_VALID) != 0
      ) {

        MemSection->ValidFields |= PLATFORM_CARD_HANDLE_VALID | PLATFORM_Module_HANDLE_VALID;
        MemSection->SmBiosCardHandle = mELogHost.ElogType16Hanlde; // Type 16 handle;
      MemSection->SmBiosModuleHandle = mELogHost.ElogSmBiosType17[MemSection->Node][MemSection->Card][MemSection->ModuleRank].SmBiosType17Handle;// Type 17 handle
      }
    }
  return EFI_SUCCESS;
}

/*
static
UINT32
GetCorrErrCntJckRegAddr(
    IN UINT8  NodeId,
    IN UINT8  DdrCh,
    IN UINT8  Rank
 )
/*++

Routine Description:
    Get Address of Correctable Error count JCK register
    for a given DDR CH, Rank

Arguments:

  NodeId    - Memory Node ID
  DdrCh     - DDR Channel ID
  Rank      - Logical Rank ID (0..7)

Returns:
  Address of correctable Error count register

--*/
/*{
  UINT8   csMapping2DPC[] = {0, 1, 2, 3, 4, 5, 6, 7};
  UINT8   csMapping3DPC[] = {0, 1, 2, 3, 4, 5, 8, 9};
  UINT32  corrErrCntJckRegAddr;

  if( mMemRas->SystemMemInfo->NodeInfo[NodeId].ChannelInfo[DdrCh].MaxDimm == 3){
    corrErrCntJckRegAddr = CORRERRCNT00_JCK_REG + csMapping3DPC[Rank] * 2;
  } else {
    corrErrCntJckRegAddr = CORRERRCNT00_JCK_REG + csMapping2DPC[Rank] * 2;
  }

  return corrErrCntJckRegAddr;
}*/

VOID
ClearIMCErrors(
 UINT8 Node
 )
{
  ClearImcRankCntrsPerNode(Node);
}

VOID
ClearImcRankCntrsPerCh (UINT8 Node,
                           UINT8 Ch)
/*++

Routine Description:
    clears corrected error status/counts for the channel
    Note: corrected error threshold init is done by MRC

Arguments:

    Node: Node to init
    Ch: channel to init
 
Returns:

    void

--*/
{
  CORRERRCNT_0_MCDDC_DP_STRUCT        corrErrCntReg;
  //CORRERRCNT00_JCK_STRUCT           corrErrCntJckReg;
  //UINT32                            corrErrCntJckRegAddr;
  UINT8                               Rank;
  CORRERRORSTATUS_MCDDC_DP_BDX_STRUCT corrErrStsReg;

  //
  // Ensure channel is enabled
  //
  if (mELogHost.imc[Node].channelList[Ch].chEnabled == 0) {
    return;
  }

  //
  // Clear bank corrected error threshold status
  //
  // Clear CORRERRORSTATUS, write 1 to clear
  //
  corrErrStsReg.Data = 0;
  for( Rank = 0; (Rank < MAX_RANK_CH) ; Rank++ ){
    corrErrStsReg.Data = mMemRas->MemReadPciCfg( Node, Ch, CORRERRORSTATUS_MCDDC_DP_REG );
    corrErrStsReg.Bits.err_overflow_stat = 1 << Rank; // clear each rank
  }
  mMemRas->MemWritePciCfg(Node, Ch, CORRERRORSTATUS_MCDDC_DP_REG, corrErrStsReg.Data );
  //
  // Clear corrected error count for each rank
  //
  for (Rank = 0; Rank < MAX_RANK_CH; Rank++) {
    corrErrCntReg.Bits.cor_err_cnt_0  = 0;
    corrErrCntReg.Bits.overflow_0     = 1;
    corrErrCntReg.Bits.cor_err_cnt_1  = 0;
    corrErrCntReg.Bits.overflow_1     = 1;
    mMemRas->MemWritePciCfg(   Node, Ch, CORRERRCNT_0_MCDDC_DP_REG + (Rank/2) * 4, corrErrCntReg.Data);
    //
    // Clear error counters and status in JCK for this rank
    //
    /*corrErrCntJckRegAddr  = GetCorrErrCntJckRegAddr( Node, Ch, Rank );
    corrErrCntJckReg.Data = (UINT16) mMemRas->ReadJckCfg(  Node, Ch, DDRGLOBAL, corrErrCntJckRegAddr );
    corrErrCntJckReg.Bits.corerrcnt = 0;
    corrErrCntJckReg.Bits.overflow  = 1;
    mMemRas->WriteJckCfg(  Node, Ch, DDRGLOBAL, corrErrCntJckRegAddr, corrErrCntJckReg.Data );*/

  }

  return;
}

VOID
ClearImcRankCntrsPerNode (UINT8 Node)
/*++

Routine Description:
    Clears the Imc per rank error status regsietrs of each channel

Arguments:

    Node: Node to init

Returns:

    none

--*/
{
  UINT8 ch;

  for (ch = 0; ch < MAX_CH; ch++) {
    if (mELogHost.imc[Node].channelList[ch].chEnabled == 0) {
      continue;
    }
    ClearImcRankCntrsPerCh (Node, ch);
  }

  return;
}

VOID
DisableCmci (
  VOID *Buffer
  )
{
/*++

Routine Description:
    Disables the CMCI signalling.
    MSR 17f - setting Bit4 disables CMCI.

Arguments:
    none

Returns:
    none

--*/
  UINT64 Data;

  Data = AsmReadMsr64(0x17F);
  Data |= BIT4;
  AsmWriteMsr64 (0x17f, Data);
  return;
}

VOID
DisableCmci_s4030032 (
  )
/*++

Routine Description:
    Disables the CMCI signalling in the system.

Arguments:
    none

Returns:
    none

--*/
{
  UINT8       thread;
  if( mErrLogSetupData->Cmci == 01 )  return;

  for (thread = 0; thread < mELogHost.cpuCount; thread++) {
    mSmst->SmmStartupThisAp (DisableCmci,
                             thread, 
                             NULL);
  }
  DisableCmci(&thread);
}

VOID
InitIMCCorrectableError (UINT8 Node)
/*++

Routine Description:
    Init each source of correctable errors: IMC spare, IMC error threshold, HA mirroring

Arguments:

    Node: Node to clear

Returns:

    void

--*/
{
  //
  // Enable SMI generation for corrected errors. Includes sparing / dev tagging, mirroring, corrected err logging
  //
  ClearImcRankCntrsPerNode(Node);
    if (mErrLogSetupData->SystemErrorEn == 1)
      EnableSMIForCorrectedMemoryErrors (Node);

  return;
}

VOID
EFIAPI
EnableSMIForCorrectedMemoryErrors (UINT8 node)
/*++

Routine Description:
    This function enables or disables SMI generation of memory and HA corrected memeory errors.

Arguments:

  i/p : node number
Returns:

  none

--*/

{
  EXRAS_CONFIG_HA_CFG_STRUCT      ExRasCfg;
  SMICTRL_UBOX_MISC_HSX_BDX_STRUCT uboxSMICtrlReg;

  RAS_DEBUG((4, "[RAS]   mErrLogSetupData->SystemErrorEn:%d, mErrLogSetupData->EMcaCSmiEn:%d, mErrLogSetupData->CorrectedMemoryErrors:%d  mSpareIntSelect:%d\n",
    mErrLogSetupData->SystemErrorEn,
    mErrLogSetupData->EMcaCSmiEn,
    mErrLogSetupData->CorrectedMemoryErrors,
    mSpareIntSelect));

  ExRasCfg.Data = mMemRas->MemReadPciCfg(  NODE_TO_SKT(node), NODE_TO_MC(node), EXRAS_CONFIG_HA_CFG_REG);

  if (mErrLogSetupData->SystemErrorEn == 1) {
    if (mErrLogSetupData->CorrectedMemoryErrors == 1) {
#if defined(HSX_HOST) || defined(BDX_HOST)      
      if ((mELogHost.CpuFamily == CPU_HSX) && (mELogHost.CpuStepping < C0_REV_HSX)) {
        //grantley - s4986148: Memory error counter overflow causes both SMI and CSMI 
        //*DISABLE* generation of CSMI 
        uboxSMICtrlReg.Data = mMemRas->MemReadPciCfg(  NODE_TO_SKT(node), 0, SMICTRL_UBOX_MISC_REG);
        uboxSMICtrlReg.Bits.smidis4 = 1;
        mMemRas->MemWritePciCfg(NODE_TO_SKT(node), 0, SMICTRL_UBOX_MISC_REG, uboxSMICtrlReg.Data);
      }
#endif

      // Enable SMI due to HA corrected errors
      // This are not in setup, this are 0 by default.
      //
      if (mErrLogSetupData->HaCorrErrSmiEn == 0) {
        ExRasCfg.Bits.cfgmcasmioncorr = 00;
      } else {
        ExRasCfg.Bits.cfgmcasmioncorr = 01;
      }

      if (mErrLogSetupData->MirrCorrected == 0) {
        ExRasCfg.Bits.cfgmcacmcionmirrorcorr = 01;
        ExRasCfg.Bits.cfgmcasmionmirrorcorr = 00;
      } else {
        ExRasCfg.Bits.cfgmcasmionmirrorcorr = 01;
        ExRasCfg.Bits.cfgmcacmcionmirrorcorr = 00;
      }

      if( mHaCmciOnCorrecetdThr) {
        ExRasCfg.Bits.cfgmcacmcioncorrcountthr = 01;
      } else {
        ExRasCfg.Bits.cfgmcacmcioncorrcountthr = 00;
      }

#ifndef GRANGEVILLE_FLAG 
      if (mErrLogSetupData->MirrorFailOver == 0) {
        ExRasCfg.Bits.cfgmcacmcionfailover = 01;
        ExRasCfg.Bits.cfgmcasmionfailover = 00;
      } else {
        ExRasCfg.Bits.cfgmcasmionfailover = 01;
        ExRasCfg.Bits.cfgmcacmcionfailover = 00;
      }

#endif // GRANGEVILLE_FLAG
      mMemRas->MemWritePciCfg(  NODE_TO_SKT(node), NODE_TO_MC(node), EXRAS_CONFIG_HA_CFG_REG, ExRasCfg.Data);

      //
      // Enable interrupt for logging corrected error threshold exceeded / sparing / dev tagging
      //
      EnableDisableINTForEccCorrectedMemErrors (node);
    }else {
      ExRasCfg.Bits.cfgmcasmioncorr = 00;
      ExRasCfg.Bits.cfgmcasmionmirrorcorr = 00;
      ExRasCfg.Bits.cfgmcasmionfailover = 00;
      mMemRas->MemWritePciCfg(  NODE_TO_SKT(node), NODE_TO_MC(node), EXRAS_CONFIG_HA_CFG_REG, ExRasCfg.Data);

    }
  }

}




UINT64
MCAddress (
  UINT64 Addr,
  UINT64 Misc)
{
  UINT64 Mask = 0;
  UINT64 McAddress = 0;
  UINT8 Lsb = 0;

  Mask = MCA_ADDR_BIT_MASK;
  McAddress = Addr & Mask;

  Lsb = Misc & MCA_MISC_LSB_MASK;
  Mask = ~((1 << Lsb) - 1);

  McAddress &= Mask;

  return McAddress;
}

BOOLEAN
McErrNodeFn()
{
  UINT8   socket, socketCnt =0;
  // UINT8   McErrNode = 0xFF;
  MCA_ERR_SRC_LOG_PCU_FUN2_HSX_BDX_STRUCT pcuMCAErrSrcLogReg;
  BOOLEAN mcErrFlag = FALSE;

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Ignore disabled nodes
    //
    if ((mELogHost.socketPresentBitMap & (1 << socket)) == 0) {
      continue;
    }
    socketCnt++;
    pcuMCAErrSrcLogReg.Data = mMemRas->MemReadPciCfg(  socket, 0, MCA_ERR_SRC_LOG_PCU_FUN2_REG);

    if ( (pcuMCAErrSrcLogReg.Bits.caterr == 1) && (pcuMCAErrSrcLogReg.Bits.ierr == 1) ) {
      ProcessorCatastrophicErrorHandler (socket);
      return FALSE;
    }

    if ( (pcuMCAErrSrcLogReg.Bits.caterr == 1) && (pcuMCAErrSrcLogReg.Bits.mcerr == 1) ) {
      //
      // Clear the error status bits
      //
      mcErrFlag = TRUE;
      continue;
    }
    // McErrNode = socket;
  }

  // Single socket case
  if(socketCnt == 1) return TRUE;
  if((socketCnt >= 1) && (mcErrFlag == TRUE)) return TRUE;
  // This indicatred every socket got MCA . Could multi Machine checks
  //if((mcErrFlag == TRUE) && (McErrNode ==0xFF)) return TRUE;
  //if((mcErrFlag == TRUE) && (McErrNode !=0xFF)) return TRUE;

  return FALSE;
}

VOID
ElogMcExecuteUncorrectableErrorHandlingPolicies (
  MEMORY_COMBINED_ERROR_RECORD    *MemoryErrRecord
  )
{

  return;
}

VOID
MirrorFailoverChannelHandler (
  UINT8 Node,
  UINT8 Ch,
  UINT8 Status)
{

  ROWHAMCONFIG_MCDDC_CTL_STRUCT rowHammerCfg;
  AMAP_MCDDC_DESC_STRUCT AMAPReg;

  if ((Status == 1) && (mELogHost.imc[Node].channelList[Ch].chFailed == 0)) {
    //
    // s4985935: WA needs to be limited to HSX stepping < C0
    //
    if ((mELogHost.CpuFamily == CPU_HSX) && (mELogHost.CpuStepping < C0_REV_HSX)) {
      //
      //  WA :When the link fail is detected BIOS needs to turn off the row hammer machine in the failed channel by setting rowhamconfig.enable to zero.
      //
      rowHammerCfg.Data =  mMemRas->MemReadPciCfg ( Node, Ch , ROWHAMCONFIG_MCDDC_CTL_REG);
      rowHammerCfg.Bits.enable = 0;
      mMemRas->MemWritePciCfg ( Node, Ch, ROWHAMCONFIG_MCDDC_CTL_REG, rowHammerCfg.Data);

      rowHammerCfg.Data =  mMemRas->MemReadPciCfg ( Node, (Ch ^ 01) , ROWHAMCONFIG_MCDDC_CTL_REG);
      rowHammerCfg.Bits.enable = 0;
      mMemRas->MemWritePciCfg ( Node, (Ch ^ 01) , ROWHAMCONFIG_MCDDC_CTL_REG, rowHammerCfg.Data);
    }
	
	// Prevent scrubbing of a failed channel
    mMemRas->DisablePatrolScrubEngine(Node);
    AMAPReg.Data= mMemRas->MemReadPciCfg(Node, Ch, AMAP_MCDDC_DESC_REG);       
    AMAPReg.Bits.dimm0_pat_rank_disable = 0xF;
    AMAPReg.Bits.dimm1_pat_rank_disable = 0xF;
    AMAPReg.Bits.dimm2_pat_rank_disable = 0xF;
    mMemRas->MemWritePciCfg(Node, Ch, AMAP_MCDDC_DESC_REG, AMAPReg.Data);
    mMemRas->ReEnablePatrolScrubEngine(Node); 
    //
    // New channel Ch failure
    //
    mELogHost.imc[Node].channelList[Ch].chFailed = 1;
    // Hardware Status is read only . The inside LogMemError function determines the dimm ,rank, physical addrerss etc.
    LogMemError (COR_ERROR_TYPE, MEM_MIRROR_FAILOVER, Node, Ch, 0, 0, 0);
  }

  return;
}

VOID
MirrorFailoverHandler (
  UINT8   Node
  )
{

  HACHFAILSTS_HA_CFG_STRUCT haChFailStsReg;
  //
  // Check if mirroring enabled
  //
  if ((mELogHost.RASmode[Node] & CH_MIRROR) != CH_MIRROR) {
    return;
  }
  //
  // Check mirror failover status for channels 0-3
  //
  haChFailStsReg.Data = mMemRas->MemReadPciCfg(  NODE_TO_SKT(Node), NODE_TO_MC(Node), HACHFAILSTS_HA_CFG_REG);

  MirrorFailoverChannelHandler (Node, 0, (UINT8)haChFailStsReg.Bits.failch0);
  MirrorFailoverChannelHandler (Node, 1, (UINT8)haChFailStsReg.Bits.failch1);
  MirrorFailoverChannelHandler (Node, 2, (UINT8)haChFailStsReg.Bits.failch2);
  MirrorFailoverChannelHandler (Node, 3, (UINT8)haChFailStsReg.Bits.failch3);

  return;
}

VOID
CAPErrorHandler (
  UINT8   Node
  )
{
  UINT16 Msr;
  UINT64 StatusMsrValue;
  UINT8 ChannelNumber=0;
  for(Msr=MSR_MC09_STATUS; Msr<=MSR_MC16_STATUS; Msr=Msr+4) {
  	StatusMsrValue=AsmReadMsr64(Msr);
	if(((StatusMsrValue & (0xFFFF0000) >> 16)) == MSCOD_DDR4_CAP_ERROR){
		ChannelNumber = StatusMsrValue & (0xF);
		LogMemError (COR_ERROR_TYPE, MEM_DDR_PARITY_ERROR, Node, ChannelNumber, 00, 0xFF, 00);
		} 
	}
  return;
}


VOID
CorrectedMemoryErrorHandler (
  UINT8   Socket,
  UINT32  ImcEventsProgress,
  UINT32  ImcNewEvents
  )
{
  BOOLEAN     isEventHandled = FALSE;
  UINT8       rasEventHndlrEntry;
  UINT8       ddrCh;
  UINT8       dimm;
  UINT8       rank;
  UINT8       node;
  EFI_STATUS  status;
//  BOOLEAN     SavedWheaFfmFlag; // AptioV Server Override: Changes done to Log Memory Errors in case of EMCA Enabled.
  //
  // Handle Mirroring, Sparing, and Correctable Memory error logging
  //

  //
  // TODO: need to read uncore MC Bank status for logging corrected memory error's mcacode?
  // 

  RAS_DEBUG((4,"Corrected Memory Error Hander start!  \n"));
// AptioV Server Override Start: Changes done to Log Memory Errors in case of EMCA Enabled.
//  SavedWheaFfmFlag = mWheaFfmCorErrLogging;
//  if (mErrLogSetupData->EMcaCSmiEn == 1) {
//    mWheaFfmCorErrLogging = FALSE;
//  }
// AptioV Server Override End: Changes done to Log Memory Errors in case of EMCA Enabled.
  //
  // Handle Mirror failover case
  //
  for( node = SKTMC_TO_NODE(Socket, 0); node <= SKTMC_TO_NODE(Socket, (MAX_HA-1)); node++) {
  if ( mELogHost.imc[node].enabled == 0 ) continue;

    //
    //CAP error Handler
    //
    CAPErrorHandler(node);

    MirrorFailoverHandler (node);

    //
    // Handle Sparing / DDDC / Device Tagging / Corrected error logging
    //

    //
    // Check if there are any events in progress or new events
    // and handle them. This SMI could be due that
    // completion of that event handling or new event
    //
    if( ImcEventsProgress != 0 || ImcNewEvents != 0 ){
      if( ImcEventsProgress & (1 << node) ){
        for( rasEventHndlrEntry = 0; mRasEventHndlrTable[rasEventHndlrEntry] != END_OF_TABLE; rasEventHndlrEntry++ ){
          RAS_DEBUG((4,"    Events in progress start!  \n"));

          status = mRasEventHndlrTable[rasEventHndlrEntry]( node, EVENTS_IN_PROGRESS, &isEventHandled );
          ASSERT(status == EFI_SUCCESS);
          if( isEventHandled == TRUE){
            RAS_DEBUG((4,"    Events in progress end!  \n"));
            break;      // Event handled on this node. Check other nodes
          }
        } // for(rasEventHndlrEntry)
      }

       if( ImcNewEvents & (1 << node) ){
      //  do {
          isEventHandled = FALSE;
        for( rasEventHndlrEntry = 0; mRasEventHndlrTable[rasEventHndlrEntry] != END_OF_TABLE; rasEventHndlrEntry++ ){
          RAS_DEBUG((4,"    New Events  start!  \n"));
          status = mRasEventHndlrTable[rasEventHndlrEntry]( node, NEW_EVENTS, &isEventHandled );
          ASSERT(status == EFI_SUCCESS);
          if( isEventHandled == TRUE){
            //
            // Get the ch/dimm/rank that had the error
            //
            // Aptio V Server Override Start : Changes done to find the Rank/Channel/DIMM information when Default Memory Handler Code gets control.
            RAS_DEBUG((4, " gGetErrInfo: %x \n", gGetErrInfo)); 
	    if ( gGetErrInfo ) 
	    {
	      gGetErrInfo = FALSE;
	      GetErrInfo (  node, &ddrCh, &dimm, &rank);
	      ClearUnhandledCorrError( node, NEW_EVENTS, &isEventHandled); // Aptio V Serve Override  : Clear the counters and status register when the Default Memory Handler code gets control.
            } 
	    else 
	    {
              mMemRas->GetLastErrInfo(  node, &ddrCh, &dimm, &rank);
            }
      	    // Aptio V Server Override End : Changes done to find the Rank/Channel/DIMM information when Default Memory Handler Code gets control.
            RAS_DEBUG((4, "Last Err Info Node=%X ddrch=%X dimm=%X rank=%X \n", node, ddrCh, dimm, rank));

            //
            // Log error
            //
            LogMemError (COR_ERROR_TYPE,
                MEM_ECC_ERROR,
                node,
                ddrCh,
                rank,
                (UINT8)dimm,
                (UINT8)0);
            RAS_DEBUG((4,"    New Events  end!  \n"));
              break;      // Event handled on this node.
          }
        } // for(rasEventHndlrEntry)
          //
          // If an event was handled on this node (isEventHandled == TRUE),
          // loop to check if another event is on this node.
          //
     //   } while (isEventHandled == TRUE);
      }
    } // if( ImcEventsProgress != 0 || ImcNewEvents != 0 )
  } // node loop
// AptioV Server Override Start: Changes done to Log Memory Errors in case of EMCA Enabled.
//  if (mErrLogSetupData->EMcaCSmiEn == 1) {
//    mWheaFfmCorErrLogging = SavedWheaFfmFlag;
//  }
// AptioV Server Override End: Changes done to Log Memory Errors in case of EMCA Enabled.
  RAS_DEBUG((4,"Corrected Memory Error Hander end!  \n"));
  //SparingHandler (Node);

  //
  // SMBUS handling not needed?
  // SMBUSErrorHandler(Node);
  //
  return;
}

EFI_STATUS
EnableElogMemoryRuntime(
  UINT8 Node
)
/*++

Routine Description:

  This function is called from the runtime memras code after meory online or offline or the migration changes etc.

Arguments:

    i/p : node number

Returns:

    EFI_SUCCESS if the call is succeed

--*/
{

  UpdateSmmELogHostGlobals();
  DisableCmci_s4030032();
  InitIMCCorrectableError (Node);
  //EnableVmseChErrors(Node);
  ClearUboxAndMiscErrorStatusPerSkt(NODE_TO_SKT(Node));
  EnableDisableSMIForUncorrectedErrors (NODE_TO_SKT(Node));
  //ProgramViralPoison();
  EnableDisableGlobalSMIGenerationPerSocket ( NODE_TO_SKT(Node));
  return EFI_SUCCESS;
}

VOID
ClearMemoryErrors(VOID)
{
  UINT8 node, Socket/*, VmseCh*/;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (node = SKTMC_TO_NODE(Socket, 0); node <= SKTMC_TO_NODE(Socket, (MAX_HA-1)); node++) {
      if ( mELogHost.imc[node].enabled == 0 ) continue;
        ClearIMCErrors(node);
        /*if (mIvtCpuEx){
          for (VmseCh = 0; VmseCh < MAX_VMSE_CH; VmseCh++) {
            if(mELogHost.imc[node].vmseChannelEnabled[VmseCh] == 00) continue;
            ClearVmseErrors(SKTMC_TO_NODE(Socket, 0));
          }
        }*/
    }
  }
  return;
}

/*VOID
ClearVmseErrors (UINT8 node)
/*++

Routine Description:
    this function clears VMSE errors other than rd ecc error counters.

Arguments:

  i/p : node number
Returns:

  none

--*/
/*{

  VMSE_MCA_CTL_MCDDC_DP_STRUCT                  VmseSmiCtrl;
  UINT8                                         VmseCh = 0;
  VMSE_ERROR_MCDDC_DP_STRUCT                    vmseError;
  VMSE_RETRY_TIMER_MCDDC_DP_STRUCT              vmseRetTimer;
  VMSE_ERROR_COUNTS_0_MCDDC_DP_STRUCT           vmseErrCounts0;
  VMSE_ERROR_COUNTS_1_MCDDC_DP_STRUCT           vmseErrCounts1;
  VMSE_RETRY_SB_ERR_COUNT_MCDDC_DP_STRUCT       vmseRetSbErrCnt;
  VMSE_RETRY_NB_ERR_COUNT_MCDDC_DP_STRUCT       vmseRetNbErrCnt;
    
  if(mIvtCpuEx) {


    for (VmseCh = 0; VmseCh < MAX_VMSE_CH; VmseCh++) {
      if(mELogHost.imc[node].vmseChannelEnabled[VmseCh] == 00) continue;

      vmseErrCounts0.Data = mMemRas->MemReadPciCfg(  node, (VmseCh * MAX_JCK_CH), VMSE_ERROR_COUNTS_0_MCDDC_DP_REG);
      vmseErrCounts0.Bits.cmd_crc_count = 0;
      vmseErrCounts0.Bits.wr_data_ecc_count = 0;
      mMemRas->MemWritePciCfg(  node, (VmseCh * MAX_JCK_CH), VMSE_ERROR_COUNTS_0_MCDDC_DP_REG, vmseErrCounts0.Data);

      vmseErrCounts1.Data = mMemRas->MemReadPciCfg(  node, (VmseCh * MAX_JCK_CH), VMSE_ERROR_COUNTS_1_MCDDC_DP_REG);
      vmseErrCounts1.Bits.forced_err_count = 0;
      vmseErrCounts1.Bits.register_parity_a = 0;
      vmseErrCounts1.Bits.register_parity_b = 0;
      mMemRas->MemWritePciCfg(  node, (VmseCh * MAX_JCK_CH), VMSE_ERROR_COUNTS_1_MCDDC_DP_REG, vmseErrCounts1.Data);

      // Hardware resets the SB and NB error transient and persistent counters as well as 
      // the link fail counters when this bit changes from a 0 to 1.
      vmseRetTimer.Data = mMemRas->MemReadPciCfg (node, (VmseCh * MAX_JCK_CH), VMSE_RETRY_TIMER_MCDDC_DP_REG);
      vmseRetTimer.Bits.reset_verr_err_cnt = 1;
      mMemRas->MemWritePciCfg (node, (VmseCh * MAX_JCK_CH), VMSE_RETRY_TIMER_MCDDC_DP_REG, vmseRetTimer.Data);

      ClearJcErrors(node, VmseCh);
      ClearJcLogREccError(node, VmseCh);

      vmseRetSbErrCnt.Data = mMemRas->MemReadPciCfg (node, (VmseCh * MAX_JCK_CH), VMSE_RETRY_SB_ERR_COUNT_MCDDC_DP_REG);
      vmseRetSbErrCnt.Bits.transient_err_cnt = 0;
      vmseRetSbErrCnt.Bits.persistent_err_cnt = 0;
      mMemRas->MemWritePciCfg (node, (VmseCh * MAX_JCK_CH), VMSE_RETRY_SB_ERR_COUNT_MCDDC_DP_REG, vmseRetSbErrCnt.Data);

        vmseRetNbErrCnt.Data = mMemRas->MemReadPciCfg  (node, (VmseCh * MAX_JCK_CH), VMSE_RETRY_NB_ERR_COUNT_MCDDC_DP_REG);
      vmseRetNbErrCnt.Bits.transient_err_cnt = 0;
      vmseRetNbErrCnt.Bits.persistent_err_cnt = 0;
      mMemRas->MemWritePciCfg  (node, (VmseCh * MAX_JCK_CH), VMSE_RETRY_NB_ERR_COUNT_MCDDC_DP_REG, vmseRetNbErrCnt.Data);

    /*
      //
      // Program VMSE Retry error limit thresholds
      //
      vmseRetErrLimit.Data = mMemRas->MemWritePciCfg (node, (VmseCh * MAX_JCK_CH), VMSE_RETRY_ERR_LIMITS_MCDDC_DP_REG);
      if(host->setup.mem.vmseOptions & VMSE_NB_PERST_EN){
        vmseRetErrLimit.Bits.nb_err_enable = 1;
        } else {
        vmseRetErrLimit.Bits.nb_err_enable = 0;
        }
        if(host->setup.mem.vmseOptions & VMSE_SB_PERST_EN){
        vmseRetErrLimit.Bits.sb_err_enable = 1;
        } else {
        vmseRetErrLimit.Bits.sb_err_enable = 0;
        }
        vmseRetErrLimit.Bits.nb_err_limit = host->setup.mem.nbErrTh;
        vmseRetErrLimit.Bits.sb_err_limit = host->setup.mem.sbErrTh;;
        mMemRas->MemWritePciCfg (node, (VmseCh * MAX_JCK_CH), VMSE_RETRY_ERR_LIMITS_MCDDC_DP_REG, vmseRetErrLimit.Data);
    */
      // Clear the Error log enable bits in the VMSE_MCA_CTL CSR
      /*VmseSmiCtrl.Data  = mMemRas->MemReadPciCfg(  node, (VmseCh * MAX_JCK_CH), VMSE_MCA_CTL_MCDDC_DP_REG);

      VmseSmiCtrl.Bits.err0_log = 01; // CLears the prev error log.
      VmseSmiCtrl.Bits.err1_log = 01; // CLears the prev error log.
      VmseSmiCtrl.Bits.err2_log = 01; // CLears the prev error log.
      VmseSmiCtrl.Bits.err3_log = 01; // CLears the prev error log.
      VmseSmiCtrl.Bits.err4_log = 01; // CLears the prev error log.

      mMemRas->MemWritePciCfg(  node, (VmseCh * MAX_JCK_CH), VMSE_MCA_CTL_MCDDC_DP_REG, VmseSmiCtrl.Data);

      //Clear ignore_vmse_err pin to unmask VMSE_ERR# in the iMc. - default is cleared.
      vmseError.Data  = mMemRas->MemReadPciCfg(  node, (VmseCh * MAX_JCK_CH), VMSE_ERROR_MCDDC_DP_REG);
      //VmseError.Bits.ignore_vmse_err pin  = 0;
      vmseError.Bits.current_vmse_err = 1;
      vmseError.Bits.observed_vmse_err_assert = 1;
      mMemRas->MemWritePciCfg(  node, (VmseCh * MAX_JCK_CH), VMSE_ERROR_MCDDC_DP_REG, vmseError.Data );
    }
  }
  return;
}*/

/*VOID
InitializeNbErrFlowVariables (
  UINT8 node)
/*
  Routine Description:
    This function initializes the variables used CheckAndUpdateLinkWidth.
    
  Arguments:
    i/p : node number

  Returns:
    none

--*/
/*{

  UINT8     VmseCh = 0;

  for (VmseCh = 0; VmseCh < MAX_VMSE_CH; VmseCh++) {

    //mTotalJckEccCorRankErrCnt[node][VmseCh] = 0;
    mVmseErrCnt[node][VmseCh]               = 0;
    mPersistentErrCnt[node][VmseCh]         = 0;
    mTransientErrOverFlowCnt[node][VmseCh]  = 1;
    mAvailableLinkWidth[node][VmseCh]       = 0;
  }
}*/
/*
VOID
EnableVmseChErrors (UINT8 node)
/*++

Routine Description:
    this function enables the smi genearation of VMSE/JCK errors reported through VMSE_ERR siganl.

Arguments:

  i/p : node number
Returns:

  none

--*/
/*{

  VMSE_MCA_CTL_MCDDC_DP_STRUCT  VmseSmiCtrl;
  UINT8                         VmseCh = 0;
  EMASK_JCK_STRUCT              emask;
  VMSE_LINK_WIDTH_MCDDC_DP_STRUCT vmseLinkWidthReg;

  if(mIvtCpuEx) {

  ClearVmseErrors(node);

  for (VmseCh = 0; VmseCh < MAX_VMSE_CH; VmseCh++) {
    if(mELogHost.imc[node].vmseChannelEnabled[VmseCh] == 00) continue;

    VmseSmiCtrl.Data  = mMemRas->MemReadPciCfg(  node, (VmseCh * MAX_JCK_CH), VMSE_MCA_CTL_MCDDC_DP_REG);

    if(mErrLogSetupData->VmseLinkFailure) {
      VmseSmiCtrl.Bits.err0_en  = 01; // EnableCSR logging
      VmseSmiCtrl.Bits.err0_log = 00; // CLears the prev error log.
        VmseSmiCtrl.Bits.err0_smi = 01;
    }
    else {
      VmseSmiCtrl.Bits.err0_en  = 01; // EnableCSR logging
      VmseSmiCtrl.Bits.err0_log = 00; // CLears the prev error log.
        VmseSmiCtrl.Bits.err0_cmci  = 01;
    }
/*
    if(mErrLogSetupData->VmseErrorObservedSmi)  {
      VmseSmiCtrl.Bits.err1_en  = 01; // EnableCSR logging
      VmseSmiCtrl.Bits.err1_log = 00; // CLears the prev error log.
      if(mErrLogSetupData->VmseErrorObservedSmi != DEFAULT)
        VmseSmiCtrl.Bits.err1_smi = 01;
    }
    else if(mErrLogSetupData->VmseErrorObservedCmci)  {
      VmseSmiCtrl.Bits.err1_en  = 01; // EnableCSR logging
      VmseSmiCtrl.Bits.err1_log = 00; // CLears the prev error log.
      if(mErrLogSetupData->VmseErrorObservedCmci != DEFAULT)
        VmseSmiCtrl.Bits.err1_cmci  = 01;
    } else{
      VmseSmiCtrl.Bits.err1_en  = 00; // DisableCSR logging
      VmseSmiCtrl.Bits.err1_smi = 00;
      VmseSmiCtrl.Bits.err1_cmci  = 00;
    }

    // Enbale DDR parity error
    if(mErrLogSetupData->VmseDdrParityError)  {
      VmseSmiCtrl.Bits.err2_en  = 01; // EnableCSR logging
      VmseSmiCtrl.Bits.err2_log = 00; // CLears the prev error log.
        VmseSmiCtrl.Bits.err2_smi = 01;
    }
    else  {
      VmseSmiCtrl.Bits.err2_en  = 01; // EnableCSR logging
      VmseSmiCtrl.Bits.err2_log = 00; // CLears the prev error log.
        VmseSmiCtrl.Bits.err2_cmci  = 01;
    }

    if(mErrLogSetupData->VmseNbPersistentError) {
      VmseSmiCtrl.Bits.err3_en  = 01; // EnableCSR logging
      VmseSmiCtrl.Bits.err3_log = 00; // CLears the prev error log.
        VmseSmiCtrl.Bits.err3_smi = 01;
    }
    else{
      VmseSmiCtrl.Bits.err3_en  = 01; // EnableCSR logging
      VmseSmiCtrl.Bits.err3_log = 00; // CLears the prev error log.
        VmseSmiCtrl.Bits.err3_cmci  = 01;
    }

    if(mErrLogSetupData->VmseSbPersistentError) {
      VmseSmiCtrl.Bits.err4_en  = 01; // EnableCSR logging
      VmseSmiCtrl.Bits.err4_log = 00; // CLears the prev error log.
        VmseSmiCtrl.Bits.err4_smi = 01;
    }
    else  {
      VmseSmiCtrl.Bits.err4_en  = 01; // EnableCSR logging
      VmseSmiCtrl.Bits.err4_log = 00; // CLears the prev error log.
        VmseSmiCtrl.Bits.err4_cmci  = 01;
    }

    mMemRas->MemWritePciCfg(  node, (VmseCh * MAX_JCK_CH), VMSE_MCA_CTL_MCDDC_DP_REG, VmseSmiCtrl.Data);

    // nb_err_enable, sb_err_enable, nb_err_limit, sb_err_limit, fail_count, fail_threshold are programmed in the MRC. To be added if needed.

    // Enable errors on Jordan Creek using EMASK (Offset 0x20C). Write BIT[8:0] to 0.
    emask.Data  = mMemRas->ReadJckCfg(node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, EMASK_JCK_REG);
    // Enable other registers if needed. Not changing the others.
    emask.Bits.vmseweccerr = 0;
    emask.Bits.ddr0parerr = 0;
    emask.Bits.ddr1parerr = 0;
    //emask.Bits.catrip = 0;
    //emask.Bits.injerr = 0;
    emask.Bits.readeccerr = 0;
    //emask.Bits.unimpcfgerr = 0;
    emask.Bits.vmsecrc0err = 0;
    emask.Bits.vmsecrc1err = 0;
    //emask.Bits.softwareerror = 0;
    //emask.Bits.ddr0rxfifooverrunerr = 0;
    //emask.Bits.ddr1rxfifooverrunerr = 0;
    //emask.Bits.ddr0txfifooverrunerr = 0;
    //emask.Bits.ddr1txfifooverrunerr = 0;
    //emask.Bits.ddr0rxfifounderrunerr = 0;
    //emask.Bits.ddr1rxfifounderrunerr = 0;
    //emask.Bits.ddr0txfifounderrunerr = 0;
    //emask.Bits.ddr1txfifounderrunerr = 0;
    mMemRas->WriteJckCfg ( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, EMASK_JCK_REG, emask.Data);

    vmseLinkWidthReg.Data = mMemRas->MemReadPciCfg( node, VmseCh * MAX_JCK_CH, VMSE_LINK_WIDTH_MCDDC_DP_REG );
    mSavedLinkWidth[node][VmseCh] = vmseLinkWidthReg.Bits.current_width & 03;
  }
  }

  return;
}*/

VOID
UpdateFailDeviceId(
  UINT8                          NodeId,
  UINT8                          DdrCh,
  UINT8                          Rank
 )
{

  //UINT32                            dddcCntlRegBaseAddr;
  UINT32                            devTagCntlRegBaseAddr;
  //DDDC_CNTL_0_MCDDC_DP_STRUCT       dddcCntlReg;
  DEVTAG_CNTL_0_MCDDC_DP_STRUCT devTagCntlReg;
  devTagCntlRegBaseAddr = DEVTAG_CNTL_0_MCDDC_DP_REG;
  //dddcCntlRegBaseAddr   = DDDC_CNTL_0_MCDDC_DP_REG;
  devTagCntlReg.Data  = (UINT8) mMemRas->MemReadPciCfg( NodeId, DdrCh, devTagCntlRegBaseAddr + Rank );
  mFailedDevice[NodeId][DdrCh][Rank] = devTagCntlReg.Bits.faildevice;
  /*if(mMemRas->SystemMemInfo->NodeInfo[NodeId].RasModesEnabled & LOCKSTEP_EN){
    dddcCntlReg.Data    = (UINT8) mMemRas->MemReadPciCfg( NodeId, DdrCh, dddcCntlRegBaseAddr + Rank );
    if((mMemRas->SystemMemInfo->NodeInfo[NodeId].RasModesEnabled & DDDC_EN) && dddcCntlReg.Bits.en == 0){
      mFailedDevice[NodeId][DdrCh][Rank] = dddcCntlReg.Bits.faildevice;
    }
  } // if LOCKSTEP_EN */
}

/*VOID
CheckAndUpdateLinkWidth (
  UINT8 node)
  /*++

  Routine Description:
    This function performs the link width change if the numebr of VMSE errors crosses the error limit

  Arguments:
    i/p : node number

  Returns:
    none

--*/
/*{
  VMSE_RETRY_NB_ERR_COUNT_MCDDC_DP_STRUCT vmseRetNbErrCnt;
  VMSE_RETRY_ERR_LIMITS_MCDDC_DP_STRUCT   vmseRetErrLimit;
  VMSE_LINK_WIDTH_MCDDC_DP_STRUCT         vmseLinkWidthReg;
  CORRERRCNT00_JCK_STRUCT                 corrErrCntJckReg;
  UINT32                                  corrErrCntJckRegAddr;
  UINT8                                   VmseCh = 0;
  UINT8                                   Ch = 0;
  UINT8                                   Rank = 0;
  UINT8                                   NumRanks = 0;
  UINT8                                   FailDeviceId;
  struct                                  channelNvram (*channelNvList)[MAX_CH];
  struct                                  dimmNvram (*dimmNvList)[MAX_DIMM];
  UINT32                                  JckErrCnt[MAX_VMSE_CH];
  UINT8                                   DqMap = 00;
  UINT8                                   VdqEqiMap = 0;
  UINT8                                   Data8 = 0;
  UINT8                                   Dimm = 0;
  UINT8                                   Multiplier = 1;
  UINT8                                   DimmRankCnt = 0;
  UINT8                                   LinkWidthToChange;

  channelNvList = &mQpiRas->Host->nvram.common.imc[node].channelList;
  for (VmseCh = 0; VmseCh < MAX_VMSE_CH; VmseCh++) {
    if(mELogHost.imc[node].vmseChannelEnabled[VmseCh] == 00) continue;
    // Check if Half width A or B or C is available
    if( (mAvailableLinkWidth[node][VmseCh] & HalfWidthABC ) == HalfWidthABC)  continue;

    JckErrCnt[VmseCh]   = 00;
    vmseRetNbErrCnt.Data = mMemRas->MemReadPciCfg ( node, (VmseCh * MAX_JCK_CH), VMSE_RETRY_NB_ERR_COUNT_MCDDC_DP_REG);
    // Check Persistent error count changed
    if(mPersistentErrCnt[node][VmseCh] == vmseRetNbErrCnt.Bits.persistent_err_cnt)  return;
      mPersistentErrCnt[node][VmseCh] = vmseRetNbErrCnt.Bits.persistent_err_cnt;
    // Update the fail device ID
    FailDeviceId  = (UINT8)-1;
    for( Ch = 0; Ch < MAX_JCK_CH; Ch++) {
      if(mQpiRas->Host->nvram.common.imc[node].channelList[(VmseCh * MAX_JCK_CH) + Ch].enabled == 0) continue;
      dimmNvList = &mQpiRas->Host->nvram.common.imc[node].channelList[Ch].dimmList;
      NumRanks  = mQpiRas->Host->nvram.common.imc[node].channelList[Ch].numRanks;
      for (Rank = 0; Rank < NumRanks; Rank++) {
        corrErrCntJckRegAddr  = GetCorrErrCntJckRegAddr( node, (VmseCh * MAX_JCK_CH)+ Ch, Rank );
        corrErrCntJckReg.Data = (UINT16) mMemRas->ReadJckCfg(  node, (VmseCh * MAX_JCK_CH)+ Ch, DDRGLOBAL, corrErrCntJckRegAddr );
        // Update the total Jck ecc error count per vmse channel
        JckErrCnt[VmseCh] += corrErrCntJckReg.Bits.corerrcnt;
        // Check If  Jck Error count is zero .
        if(corrErrCntJckReg.Bits.corerrcnt == 0) {
          mFailedDevice[node][(VmseCh * MAX_JCK_CH)+ Ch][Rank] = (UINT8)-1;
          continue;
        }
        UpdateFailDeviceId(node, (VmseCh * MAX_JCK_CH)+ Ch, Rank);
      }
    }
    // 
    //mTotalJckEccCorRankErrCnt[node][VmseCh] += JckErrCnt[VmseCh];
    vmseRetErrLimit.Data = mMemRas->MemReadPciCfg (node, (VmseCh * MAX_JCK_CH), VMSE_RETRY_ERR_LIMITS_MCDDC_DP_REG);
    // calculate th vmse error count.
    //mVmseErrCnt[node][VmseCh] = (vmseRetErrLimit.Bits.nb_err_limit * mTransientErrOverFlowCnt[node][VmseCh] ) - mTotalJckEccCorRankErrCnt[node][VmseCh];
    mVmseErrCnt[node][VmseCh] = (vmseRetErrLimit.Bits.nb_err_limit * mTransientErrOverFlowCnt[node][VmseCh] ) - JckErrCnt[VmseCh];
    // check if VMSE err count is greter than error limit
    if( mVmseErrCnt[node][VmseCh] >= vmseRetErrLimit.Bits.nb_err_limit )  {
      for( Ch = 0; Ch < MAX_JCK_CH; Ch++) {
        if(mQpiRas->Host->nvram.common.imc[node].channelList[(VmseCh * MAX_JCK_CH) + Ch].enabled == 0) continue;

        dimmNvList = &mQpiRas->Host->nvram.common.imc[node].channelList[Ch].dimmList;
        NumRanks  = mQpiRas->Host->nvram.common.imc[node].channelList[Ch].numRanks;
        Dimm = 0;
        DimmRankCnt = 1;
        // Check each ranks fails device ID
        for (Rank = 0; Rank < NumRanks; Rank++) {
          // Rank0 fail device ID
          FailDeviceId  = mFailedDevice[node][(VmseCh * MAX_JCK_CH) + Ch][0];
          if(FailDeviceId == -1) break;

          Multiplier  = 1;
          if ((*channelNvList)[Ch].features != X4_AND_X8_PRESENT) {
            if(FailDeviceId != mFailedDevice[node][(VmseCh * MAX_JCK_CH) + Ch][Rank]) {
              FailDeviceId  = (UINT8)-1;
              break;
            }
          } else {  // Mixed dimms case
            if ((DimmRankCnt == (*dimmNvList)[Dimm].numRanks) || ((*dimmNvList)[Dimm].dimmPresent == 00) ){
              DimmRankCnt = 1;
              Dimm++;
            }else {
              DimmRankCnt++;
            }
          if( (*dimmNvList)[Dimm].x4Present == 1 )  {
            Multiplier = 2; // Equivelent of X8 map
          }
        }
        if(FailDeviceId != (mFailedDevice[node][(VmseCh * MAX_JCK_CH) + Ch][Rank] * Multiplier) ) {
          FailDeviceId  = (UINT8)-1;
          break;
          }
        }  //  for (Rank = 0; Rank < NumRanks; Rank++) {

        if(FailDeviceId == (-1))  continue;
        // faild device ID  to Cache line map
        if ((*channelNvList)[Ch].features & X4_PRESENT) {
          DqMap   = FailDeviceId * 4;
        } else if ((*channelNvList)[Ch].features & X8_PRESENT) {
          DqMap   = FailDeviceId * 8;
        } else if ((*channelNvList)[Ch].features & X16_PRESENT) {
          DqMap   = FailDeviceId * 16;
        } else if ((*channelNvList)[Ch].features & X4_AND_X8_PRESENT) {
          if( (*dimmNvList)[0].x4Present == 1 )
            DqMap = FailDeviceId * 4;
          else
            DqMap = FailDeviceId * 8;
        }
        // get the available link width
        VdqEqiMap = DqMap/24;
        Data8     = (VdqEqiMap / 8 ) ;
        vmseLinkWidthReg.Data = mMemRas->MemReadPciCfg( node, (VmseCh * MAX_JCK_CH), VMSE_LINK_WIDTH_MCDDC_DP_REG );
        LinkWidthToChange   = (UINT8)-1;
        if(Data8 == 02) {   // Link WIdth A not possible
          if((mAvailableLinkWidth[node][VmseCh] & HalfWidthB) == 00)  {
            mAvailableLinkWidth[node][VmseCh] |= HalfWidthB;
            LinkWidthToChange = VMSE_NB_HALF_WIDTH_B;
          }
          if((mAvailableLinkWidth[node][VmseCh] & HalfWidthC) == 00)  {
            mAvailableLinkWidth[node][VmseCh] |= HalfWidthC;
           LinkWidthToChange  = VMSE_NB_HALF_WIDTH_C;
          }
        } 
        if(Data8 == 00) {   // Link WIdth B not possible
          if((mAvailableLinkWidth[node][VmseCh] & HalfWidthA) == 00)  {
           mAvailableLinkWidth[node][VmseCh] |= HalfWidthA;
           LinkWidthToChange  = VMSE_NB_HALF_WIDTH_A;
          }
          if((mAvailableLinkWidth[node][VmseCh] & HalfWidthC) == 00)  {
           mAvailableLinkWidth[node][VmseCh] |= HalfWidthC;
           LinkWidthToChange  = VMSE_NB_HALF_WIDTH_C;
          }
        } 
        if(Data8 == 01) {   // Link WIdth C not possible
          if((mAvailableLinkWidth[node][VmseCh] & HalfWidthA) == 00)  {
           mAvailableLinkWidth[node][VmseCh] |= HalfWidthA;
            LinkWidthToChange = VMSE_NB_HALF_WIDTH_A;
          }
          if((mAvailableLinkWidth[node][VmseCh] & HalfWidthB) == 00)  {
            mAvailableLinkWidth[node][VmseCh] |= HalfWidthB;
             LinkWidthToChange  = VMSE_NB_HALF_WIDTH_B;
          }
        } 
      
        if(LinkWidthToChange != (UINT8)-1 ) {
          vmseLinkWidthReg.Bits.override_link_width = LinkWidthToChange;
          vmseLinkWidthReg.Bits.override_once       = 1;
          mMemRas->MemWritePciCfg( node, (VmseCh * MAX_JCK_CH), VMSE_LINK_WIDTH_MCDDC_DP_REG, vmseLinkWidthReg.Data );
          mVmseErrCnt[node][VmseCh]         = 00;
          // SendVmseErrCmd
          mMemRas->SendVmseErrCmd(node, VmseCh);
          break;  // for( Ch = 0; Ch < MAX_JCK_CH; Ch++)
        }

        // Reset the variables.
        mVmseErrCnt[node][VmseCh]         = 00;
        //mTotalJckEccCorRankErrCnt[node][VmseCh]   = 00;   // ???
        // mTransientErrOverFlowCnt[node][VmseCh]     = 01;   // ???
      } // end of for( Ch = 0; Ch < MAX_JCK_CH; Ch++)
    } else {
      mTransientErrOverFlowCnt[node][VmseCh]++;
    }  // if( mVmseErrCnt[node][VmseCh] >= vmseRetErrLimit.Bits.nb_err_limit )  {
  }
}*/

/*VOID
VmseErrorHandler (
  UINT8   Socket
  )
/*++

Routine Description:
    Handler function that logs and clears the error reported by vmse_err signal

Arguments:

  i/p : Socket number
Returns:

  none

--*/
/*{

  UINT8             node;
  UINT8             VmseCh = 0;
  VMSE_MCA_CTL_MCDDC_DP_STRUCT  VmseSmiCtrl;
  VMSE_LINK_WIDTH_MCDDC_DP_STRUCT vmseLinkWidthReg;
#ifdef IVT_HOST
  SCRUBMASK_MC_MAINEXT_IVT_STRUCT ScrubMask;
#else
  SCRUBMASK_MC_MAINEXT_HSX_BDX_STRUCT ScrubMask;
#endif
  VMSE_LINK_FAIL_MCDDC_DP_STRUCT vmseLinkFail;
  LOGVCRC00_JCK_STRUCT      logvcrc;
  LERR_JCK_STRUCT       lerrjck;
  NERR_JCK_STRUCT       nerrjck;
  FERR_JCK_STRUCT       ferrjck;
  BOOLEAN           DDRParityError;
  HACHFAILSTS_HA_CFG_STRUCT   haChFailStsReg;
  BOOLEAN           LinkFailErrType;
  BOOLEAN           WireKill = 0;

  for( node = SKTMC_TO_NODE(Socket, 0); node <= SKTMC_TO_NODE(Socket, (MAX_HA-1)); node++) {
    if ( mELogHost.imc[node].enabled == 0 ) continue;

  for (VmseCh = 0; VmseCh < MAX_VMSE_CH; VmseCh++) {
  if(mELogHost.imc[node].vmseChannelEnabled[VmseCh] == 00) continue;

    VmseSmiCtrl.Data  = mMemRas->MemReadPciCfg(  node, (VmseCh * MAX_JCK_CH), VMSE_MCA_CTL_MCDDC_DP_REG);
    // Clear VMSE Unimplemented Configuration Address Error Log
    ferrjck.Data = mMemRas->ReadJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, FERR_JCK_REG);
    nerrjck.Data = mMemRas->ReadJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, NERR_JCK_REG);
    lerrjck.Data = mMemRas->ReadJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LERR_JCK_REG);

    haChFailStsReg.Data = mMemRas->MemReadPciCfg(  NODE_TO_SKT(node), NODE_TO_MC(node), HACHFAILSTS_HA_CFG_REG);

    if ( (mMemRas->SystemMemInfo->NodeInfo[node].RasModesEnabled & CH_MIRROR) && 
       ( (haChFailStsReg.Bits.failch0 ^ haChFailStsReg.Bits.failch2 ) || (haChFailStsReg.Bits.failch1 ^ haChFailStsReg.Bits.failch3 ) ) ) {
      LinkFailErrType = TRUE;
    } else {
      LinkFailErrType = FALSE;
    }

    // mErrLogSetupData->VmseLinkFailureSmi
    if((VmseSmiCtrl.Bits.err0_smi == 01) && (VmseSmiCtrl.Bits.err0_log))  {
      vmseLinkFail.Data = mMemRas->MemReadPciCfg (node, (VmseCh * MAX_JCK_CH), VMSE_LINK_FAIL_MCDDC_DP_REG);
      if( vmseLinkFail.Bits.fail_count == vmseLinkFail.Bits.fail_threshold) {
        // Link Failure. - If mirroring is on , We can report as correctable error. TBD
        if (LinkFailErrType == FALSE) {
          LogMemError (FATAL_ERROR_TYPE, MEM_LINK_ERROR, node, VmseCh * MAX_JCK_CH , 00, 00, 00);
          LogMemError (FATAL_ERROR_TYPE, MEM_LINK_ERROR, node, (VmseCh * MAX_JCK_CH) +1 , 00, 00, 00);
        } else {
          // Mirror failover handler logs this error.
          ScrubMask.Data = mMemRas->MemReadPciCfg( node, PARAM_UNUSED, SCRUBMASK_MC_MAINEXT_REG);

          if(VmseCh == 0) {
            ScrubMask.Bits.link_fail = 1;
            ScrubMask.Bits.all_ranks = 3;
            ScrubMask.Bits.ch_mask   = 3;
          } else if(VmseCh == 1){
            ScrubMask.Bits.link_fail = 02;
            ScrubMask.Bits.all_ranks = 0xc;
            ScrubMask.Bits.ch_mask   = 0xc;
          }
            mMemRas->MemWritePciCfg( node, PARAM_UNUSED, SCRUBMASK_MC_MAINEXT_REG, ScrubMask.Data);
          //LogMemError (COR_ERROR_TYPE, MEM_LINK_ERROR, node, VmseCh, 00, 00, 00);
        }
        vmseLinkFail.Bits.fail_count  = 00;
        mMemRas->MemWritePciCfg (node, (VmseCh * MAX_JCK_CH), VMSE_LINK_FAIL_MCDDC_DP_REG, vmseLinkFail.Data);
      }
    }

    // mErrLogSetupData->VmseErrorObservedSmi;
    if((VmseSmiCtrl.Bits.err1_smi == 01) && (VmseSmiCtrl.Bits.err1_log))  {
      // MXB internal errors or software errors etc... . Uncomment the code if needed.
      // LogMemError (COR_ERROR_TYPE, MEM_ERROR, node, VmseCh, 00, 00, 00);
    }

    // mErrLogSetupData->VmseDdrParityErrorSmi
    DDRParityError    = FALSE;
    if((VmseSmiCtrl.Bits.err2_smi == 01) && (VmseSmiCtrl.Bits.err2_log))  {
      if( (ferrjck.Bits.ddr0parerr) || (ferrjck.Bits.ddr1parerr) ||
        (nerrjck.Bits.ddr0parerr) || (nerrjck.Bits.ddr1parerr) ||
        (lerrjck.Bits.ddr0parerr) || (lerrjck.Bits.ddr1parerr)) {
        // Log the DDR parity error
        //if (LinkFailErrType == FALSE) {
          if(ferrjck.Bits.ddr0parerr || nerrjck.Bits.ddr0parerr || lerrjck.Bits.ddr0parerr)
            LogMemError (FATAL_ERROR_TYPE, MEM_DDR_PARITY_ERROR, node, (VmseCh * MAX_JCK_CH), 00, 00, 00);
          if(ferrjck.Bits.ddr1parerr || nerrjck.Bits.ddr1parerr || lerrjck.Bits.ddr1parerr)
            LogMemError (FATAL_ERROR_TYPE, MEM_DDR_PARITY_ERROR, node, (VmseCh * MAX_JCK_CH) + 1, 00, 00, 00);
        //} else {
          // Mirror failover handler logs this error.
        //}
        DDRParityError    = TRUE;
      }
    }

    // mErrLogSetupData->VmseNbPersistentErrorSmi
    if((VmseSmiCtrl.Bits.err3_smi == 01) && (VmseSmiCtrl.Bits.err3_log))  {

      WireKill  = 0;  // If wirelikk is not done. NO need to give control to the link width change function
      if( mELogHost.CpuStepping >= B0_REV && (WireKill == 1))   {
        // This function modifies the link width based on the number of VMSE errors.
        CheckAndUpdateLinkWidth(node);
      }

      // 0000 Full Width,0011 Half Width A - SB,0010 Half Width B - SB,0001 Half Width C - SB,0111 Half Width A - NB,0110 Half Width B - NB,0101 Half Width C - NB
      vmseLinkWidthReg.Data = mMemRas->MemReadPciCfg( node, VmseCh * MAX_JCK_CH, VMSE_LINK_WIDTH_MCDDC_DP_REG );

      if( ((vmseLinkWidthReg.Bits.current_width & 03) != mSavedLinkWidth[node][VmseCh]) ) {
        mSavedLinkWidth[node][VmseCh] = vmseLinkWidthReg.Bits.current_width & 03;
        // Link width change.
        LogMemError (COR_ERROR_TYPE, MEM_LANE_FAILOVER, node, VmseCh, 00, 00, 00);
      }


    } // if((VmseSmiCtrl.Bits.err3_smi == 01) && (VmseSmiCtrl.Bits.err3_log))

    // mErrLogSetupData->VmseSbPersistentErrorSmi
    if((VmseSmiCtrl.Bits.err4_smi == 01) && (VmseSmiCtrl.Bits.err4_log))  {

      // 0000 Full Width,0011 Half Width A - SB,0010 Half Width B - SB,0001 Half Width C - SB,0111 Half Width A - NB,0110 Half Width B - NB,0101 Half Width C - NB
      vmseLinkWidthReg.Data = mMemRas->MemReadPciCfg( node, VmseCh * MAX_JCK_CH, VMSE_LINK_WIDTH_MCDDC_DP_REG );
      // Both NB and SB has to go link width change .
      if( ((vmseLinkWidthReg.Bits.current_width & 03) != mSavedLinkWidth[node][VmseCh]) ) {
        mSavedLinkWidth[node][VmseCh] = vmseLinkWidthReg.Bits.current_width & 03;
        // Link width change.
        LogMemError (COR_ERROR_TYPE, MEM_LANE_FAILOVER, node, (VmseCh * MAX_JCK_CH), 00, 00, 00);
        LogMemError (COR_ERROR_TYPE, MEM_LANE_FAILOVER, node, (VmseCh * MAX_JCK_CH) + 1, 00, 00, 00);
      }

      if( (ferrjck.Bits.vmsecrc0err || ferrjck.Bits.vmsecrc1err ) ||
        (nerrjck.Bits.vmsecrc0err || nerrjck.Bits.vmsecrc1err ) ||
        (lerrjck.Bits.vmsecrc0err || lerrjck.Bits.vmsecrc1err ) ) {
        // Addr/cmd CRC error
        logvcrc.Data = mMemRas->ReadJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGVCRC00_JCK_REG);
        if(ferrjck.Bits.vmsecrc0err || nerrjck.Bits.vmsecrc0err || nerrjck.Bits.vmsecrc0err)
          LogMemError (FATAL_ERROR_TYPE, MEM_CRC_ERROR, node, (VmseCh * MAX_JCK_CH), 00, 00, 00);
        if(ferrjck.Bits.vmsecrc1err || nerrjck.Bits.vmsecrc1err || nerrjck.Bits.vmsecrc1err)
          LogMemError (FATAL_ERROR_TYPE, MEM_CRC_ERROR, node, (VmseCh * MAX_JCK_CH) + 1, 00, 00, 00);
      }
    }  // if((VmseSmiCtrl.Bits.err4_smi == 01) && (VmseSmiCtrl.Bits.err4_log))

  } // for (VmseCh = 0; VmseCh < MAX_VMSE_CH; VmseCh++) {
  ClearVmseErrors(node);
  } // for( node = SKTMC_TO_NODE(Socket, 0); node <= SKTMC_TO_NODE(Socket, (MAX_HA-1)); node++)

}*/
// AptioV Server Override Start: Support added for Mode1 Memory Error Reporting
VOID
GetMemErrStatus (
	VOID *Buffer
  )
{
	UINTN	Index;
	
	for (Index = MSR_MC09_STATUS; Index <= MSR_MC16_STATUS; Index += 4) {
		McStatusMsr = AsmReadMsr64(Index);
		if (McStatusMsr & BIT63) {
			McMiscMsr = AsmReadMsr64(Index + 2);
			break;
		}
	}
}
// AptioV Server Override End: Support added for Mode1 Memory Error Reporting

// AptioV Server Override Start : Changes done to resolve the MC7Status MSR read correctly based  on the failing CPU's 
VOID
GetMsrMc7OrMc8Status (
  VOID *Buffer
  )
{
	 MC7Status = AsmReadMsr64(HAMsrIndex);
}
VOID
GetMc7Addr (
  VOID *Buffer
  )
{
	   	Mc7Addr = AsmReadMsr64(HAMsrIndex + 1);
}

// AptioV Server Override end : Changes done to resolve the MC7Status MSR read correctly based  on the failing CPU's 

VOID
LogMemError (
  UINT8 type,
  UINT8 SubType,
  UINT8 node,
  UINT8 channel,
  UINT8 rank,
  UINT8 dimm,
  UINT8 mcacode
  )
/*++

Routine Description:

    Creates the memory error record based on the i/p passed.

Arguments:


Returns:

--*/
{
  MEMORY_COMBINED_ERROR_RECORD  MemoryErrRecord;  //
  MEMORY_DEV_INFO               MemInfo;  // Whea Error record uses MEMORY_DEV_INFO structure
  UINT8                         SocketId = 00;
  UINT8                         MemoryControllerId = 00;
  UINT8                         ChannelId = 00;
  UINT8                         DimmSlot = 00;
  UINT8                         DimmRank = 00;
  UINT32                        Row = 00;
  UINT32                        Col = 00;
  UINT8                         Bank = 00;
  UINT8                         BankGroup = 00;
  UINT8                         ChipSelect = 00;
  //MC5_STATUS_SHADOW_N0_HA_PMON_IVT_STRUCT Mc5Shadow0Status;
// AptioV Server Override Start: Support added for Mode1 Memory Error Reporting
//  MC5_STATUS_SHADOW_N1_HA_PMON_HSX_BDX_STRUCT Mc5ShadowStatus;
//  MC5_ADDR_SHADOW0_HA_PMON_STRUCT  Mc5Shadow0Addr;
//  MC5_ADDR_SHADOW1_HA_PMON_STRUCT  Mc5Shadow1Addr;
  UINTN                 Thread;
  UINT32                OtherInfo = 00;
  UINT64				MC7Status = 00;
  UINT64				Mc7Addr = 00;
  UINTN					Index;
  UINT32                core;
  UINT32                 thread;
  BOOLEAN                ActiveCoreThread = FALSE;
// AptioV Server Override End: Support added for Mode1 Memory Error Reporting
//  EFI_STATUS                       Status; // AptioV Server Override: Changes done to use AMI RuntimeError (RtErrorLogBMC/Whea) Listener code.
  BOOLEAN                          GetPhyAddr;

  //
  // Clear the MemInfo structure so that unused elements
  // have a predictable value in the error record.
  //
  ZeroMem (&MemInfo, sizeof (MEMORY_DEV_INFO));

  //
  // Skip if Correctable and Uncorrectable error logging is disabled
  //
  if ( (mErrLogSetupData->CorrectedMemoryErrors == 00) && (mErrLogSetupData->UncorrectedErrorsEn == 00))  return;
// AptioV Server Override Start: Changes done to use AMI RuntimeError (RtErrorLogBMC/Whea) Listener code.
  MemInfo.Source = MemSource;
  MemInfo.Type = type;
// AptioV Server Override End: Changes done to use AMI RuntimeError (RtErrorLogBMC/Whea) Listener code.
  MemInfo.ValidBits = 0;
  MemInfo.ValidBits = PLATFORM_MEM_NODE_VALID | PLATFORM_MEM_CARD_VALID | PLATFORM_MEM_ERROR_TYPE_VALID;
  MemInfo.Node      = node;
  MemInfo.Branch    = 00;
  MemInfo.PhyAddr   = 00;
  MemInfo.Channel   = channel;
  MemInfo.Dimm      = dimm;
  MemInfo.Rank      = rank;
  MemInfo.ErrorType = PLATFORM_MEM_ERROR_UNKNOWN;
  MemInfo.UefiErrorRecordRevision = mErrLogSetupData->WheaErrorRecordFormat;
// AptioV Server Override Start: Support added for Mode1 Memory Error Reporting
  SocketId = NODE_TO_SKT(node);
      for (core = 0; core < MAXCOREID && (ActiveCoreThread == FALSE); core++) {   
        for (thread = 0; thread < MAX_THREAD; thread++) {     
            if (mELogHost.cpuInfo[SocketId].Valid[core][thread])
            {
                ActiveCoreThread = TRUE;
                break;
            }
        }
      }
      if (ActiveCoreThread == TRUE) {
          if(mELogHost.cpuInfo[SocketId].Valid[core-1][thread]) {
      Thread = GetCpuNumber (SocketId, (core-1), thread);
      if(Thread == mSmst->CurrentlyExecutingCpu) {
        GetMemErrStatus (NULL);
      }
      else {
              mSmst->SmmStartupThisAp (GetMemErrStatus,
                                Thread, 
                                NULL);
          }
      }
  }

  if (type == COR_ERROR_TYPE) {
	  if (McMiscMsr & BIT62) {
		  OtherInfo = (McStatusMsr & (BIT32 | BIT33 | BIT34 | BIT35 | BIT36)) >> 32;
		  MemInfo.BitPosition = (McMiscMsr & 0x3FFFC000) >> 14;
		  MemInfo.ValidBits |= PLATFORM_MEM_BIT_POS_VALID | PLATFORM_MEM_DEVICE_VALID;
		  if (mELogHost.RASmode[0] & CH_LOCKSTEP) {
			  if (OtherInfo & BIT4)
				  MemInfo.Device = BIT4;
			  else
				  MemInfo.Device = OtherInfo & (BIT0 | BIT1 | BIT2);
		  }
		  else {
			  MemInfo.Device = OtherInfo;
		  }
	  }
  }
// AptioV Server Override End: Support added for Mode1 Memory Error Reporting
  if (type == COR_ERROR_TYPE) {
    if (SubType == MEM_ECC_ERROR) {
      MemInfo.ErrorType = PLATFORM_MEM_ERROR_SINGLEBIT_ECC;
    } else if (SubType == MEM_MIRROR_FAILOVER) {
      MemInfo.ErrorType = PLATFORM_MEM_ERROR_MIRROR_FAILED;
    } else if (SubType == MEM_DDR_PARITY_ERROR) {
      MemInfo.ErrorType = PLATFORM_MEM_ERROR_PARITY;
	  }
  }
  //
  // Use MEMORY_COMBINED_ERROR_RECORD for parameter passing...
  //
  MemoryErrRecord.PrimaryBlock.Valid = TRUE;
  MemoryErrRecord.PrimaryBlock.Source = MemSource;
  MemoryErrRecord.PrimaryBlock.Socket = NODE_TO_SKT(node);
  MemoryErrRecord.PrimaryBlock.Type = type;
  MemoryErrRecord.PrimaryBlock.SubType = MemInfo.ErrorType;
  MemoryErrRecord.PrimaryBlock.Channel = MemInfo.Channel;
  MemoryErrRecord.PrimaryBlock.Rank = rank;
  MemoryErrRecord.PrimaryBlock.Dimm = dimm;
  MemoryErrRecord.PrimaryBlock.MachineCheckCode = mcacode;

  ElogMcExecuteCorrectableErrorHandlingPolicies (&MemoryErrRecord);

  if( (type == COR_ERROR_TYPE) && (SubType == MEM_ECC_ERROR))  {
    MemInfo.ValidBits |= PLATFORM_MEM_MODULE_VALID;
    if( mErrLogSetupData->WheaErrorRecordFormat == GENERIC_ERROR_SECTION_REVISION_UEFI231) {
      MemInfo.ValidBits |= PLATFORM_RANK_NUMBER_VALID;
      if(mSmbiosProtocol != NULL) {
        MemInfo.ValidBits |= PLATFORM_CARD_HANDLE_VALID | PLATFORM_Module_HANDLE_VALID;
        MemInfo.SmBiosCardHandle = mELogHost.ElogType16Hanlde; // Type 16 handle;
        MemInfo.SmBiosModuleHandle = mELogHost.ElogSmBiosType17[node][channel][dimm].SmBiosType17Handle; // Type 17 handle
      }
    }
  }

  

// AptioV Server Override Start: Support added for Mode1 Memory Error Reporting
//  if ((type == COR_ERROR_TYPE) && (SubType == MEM_MIRROR_FAILOVER)) {
  if (type == COR_ERROR_TYPE) {
    GetPhyAddr = FALSE;
#ifdef HSX_HOST
    GetPhyAddr = TRUE;
#endif
    if (GetPhyAddr) {
//      Mc5ShadowStatus.Data = mMemRas->MemReadPciCfg(  NODE_TO_SKT(node), NODE_TO_MC(node),MC5_STATUS_SHADOW_N1_HA_PMON_REG);
		for (Index = MSR_MC07_STATUS; Index <= MSR_MC08_STATUS; Index += 4) {
			MC7Status = AsmReadMsr64(Index);
			if ((MC7Status & BIT63) && (MC7Status & BIT58)) {
				Mc7Addr = AsmReadMsr64(Index + 1);
				break;
			}
		}
		MemInfo.PhyAddr = Mc7Addr & (0x3fffffffffc0);;
/*      if( Mc5ShadowStatus.Bits.valid && Mc5ShadowStatus.Bits.addrv) {
        Mc5Shadow0Addr.Data = mMemRas->MemReadPciCfg(  NODE_TO_SKT(node), NODE_TO_MC(node), MC5_ADDR_SHADOW0_HA_PMON_REG);
        Mc5Shadow1Addr.Data = mMemRas->MemReadPciCfg(  NODE_TO_SKT(node), NODE_TO_MC(node), MC5_ADDR_SHADOW1_HA_PMON_REG);
        MemInfo.PhyAddr = ((UINT64)Mc5Shadow1Addr.Data << 32) | (Mc5Shadow0Addr.Data );
        MemInfo.PhyAddr &= (0x3fffffffffc0); // 46bit address space, bits 45:6.
        mMemRas->MemWritePciCfg(  NODE_TO_SKT(node), NODE_TO_MC(node), MC5_ADDR_SHADOW0_HA_PMON_REG, 00);
        mMemRas->MemWritePciCfg(  NODE_TO_SKT(node), NODE_TO_MC(node), MC5_ADDR_SHADOW1_HA_PMON_REG, 00);
      }*/
      if (MemInfo.PhyAddr != 00) {
        MemInfo.PhyAddrMask = 0x3fffffffffc0;
        mMemRas->SystemAddressToDimmAddress (MemInfo.PhyAddr,&SocketId,&MemoryControllerId,&ChannelId,&DimmSlot,&DimmRank,&Row,&Col,&Bank,&BankGroup,&ChipSelect);
        MemoryErrRecord.PrimaryBlock.Dimm = DimmSlot;
        MemInfo.Node      = SKTMC_TO_NODE(SocketId, MemoryControllerId);
        MemInfo.Channel   = ChannelId;
        MemInfo.Rank      = DimmRank;
        MemInfo.Branch    = 00;
        MemInfo.Dimm      = DimmSlot;
        MemInfo.Column    = Col;
        MemInfo.Bank      = Bank;
        MemInfo.Row       = Row;
        MemInfo.ValidBits |= PLATFORM_MEM_PHY_ADDRESS_VALID | PLATFORM_MEM_CARD_VALID | PLATFORM_MEM_NODE_VALID  | PLATFORM_MEM_MODULE_VALID
          | PLATFORM_MEM_ROW_VALID | PLATFORM_MEM_COLUMN_VALID | PLATFORM_MEM_PHY_ADDRESS_MASK_VALID;
        if( mErrLogSetupData->WheaErrorRecordFormat == GENERIC_ERROR_SECTION_REVISION_UEFI231) {
          MemInfo.ValidBits |= PLATFORM_RANK_NUMBER_VALID;
          if(mSmbiosProtocol != NULL) {
            MemInfo.ValidBits |= PLATFORM_CARD_HANDLE_VALID | PLATFORM_Module_HANDLE_VALID;
            MemInfo.SmBiosCardHandle = mELogHost.ElogType16Hanlde; // Type 16 handle;
            MemInfo.SmBiosModuleHandle = mELogHost.ElogSmBiosType17[node][channel][dimm].SmBiosType17Handle; // Type 17 handle
          }
        }
      }
    }
  }
// AptioV Server Override End: Support added for Mode1 Memory Error Reporting
// AptioV Server Override Start : Changes done to use AMI RuntimeError (RtErrorLogBMC/Whea) Listener code.
  CommonErrorHandling((VOID *)&MemInfo);
/*  if (mWheaPlatformSupportAvailable == TRUE) {
    //
    // Start Whea error record log
    //
    if(mWheaLogStarted == FALSE) {
      Status = mWheaPlatformSupport->WheaPlatformRestartAllElog (mWheaPlatformSupport);
      if (Status == EFI_SUCCESS) {
        mWheaLogStarted = TRUE;
      }
    }

    if ((mWheaFfmCorErrLogging == TRUE) && (type == COR_ERROR_TYPE)) {
      // prepare the error status block in HEST table.
      mWheaPlatformSupport->WheaPlatformElogMemory (
        mWheaPlatformSupport,
        GEN_ERR_SEV_PLATFORM_MEMORY_CORRECTED,
        &MemInfo
        );
      MemInfo.ValidBits = 0;
      // EndWheaElog generates SCI for this Correctable error.
      mWheaPlatformSupport->WheaPlatformEndElog (mWheaPlatformSupport );
    }

    if((mWheaFfmUnCorErrLogging == TRUE) && (type == FATAL_ERROR_TYPE)) {
      // prepare the error status block in HEST table.
      mWheaPlatformSupport->WheaPlatformElogMemory (
        mWheaPlatformSupport,
        GEN_ERR_SEV_PLATFORM_MEMORY_FATAL,
        &MemInfo
        );
      MemInfo.ValidBits = 0;
      // EndWheaElog generates NMI for uncorrectable errors
      mWheaPlatformSupport->WheaPlatformEndElog ( mWheaPlatformSupport );
    }
  }*/
// AptioV Server Override End : Changes done to use AMI RuntimeError (RtErrorLogBMC/Whea) Listener code.
}


VOID
DisableSMIForCorrectedMemErrorThreshold (UINT8 Node)
/*++

Routine Description:
    Disables the SMI generation of memory ECC errors 

Arguments:

  i/p : node number
Returns:

  none

--*/

{
  SMISPARECTL_MC_MAINEXT_STRUCT imcSMISpareCtlReg;

  //
  // Disable SMI for sparing or corrected memory error threshold
  //

  imcSMISpareCtlReg.Data = mMemRas->MemReadPciCfg(  Node, PARAM_UNUSED, SMISPARECTL_MC_MAINEXT_REG);
  imcSMISpareCtlReg.Bits.intrpt_sel_smi = 0;

  mMemRas->MemWritePciCfg(  Node, PARAM_UNUSED, SMISPARECTL_MC_MAINEXT_REG, imcSMISpareCtlReg.Data);

  return;
}


VOID
CorrectedErrorSMIThrottling (
  MEMORY_COMBINED_ERROR_RECORD  *MemoryErrRecord
  )
{
//  TIME_STAMP timeStamp;
  UINT8 node = 0;
  UINT8 failedNode;
  UINT8 failedChannel;
  UINT8 failedRank;


  failedNode = MemoryErrRecord->PrimaryBlock.Socket;
  failedChannel = MemoryErrRecord->PrimaryBlock.Channel;
  failedRank = MemoryErrRecord->PrimaryBlock.Rank;


  //
  // Compute rank error rate and disable corrected SMIs if rate > threshold
  //

  //
  // Init timestamp, if first time
  //
  if (mCorrectedErrorRankTimeStamp[failedNode][failedChannel][failedRank].Year == 0) {
    EfiSmmGetTime (&mCorrectedErrorRankTimeStamp[failedNode][failedChannel][failedRank]);
    return;
  }

  if (EfiSmmGetNoofHoursPast (&mCorrectedErrorRankTimeStamp[failedNode][failedChannel][failedRank])
                             <= PLATFORM_PER_RANK_THRESHOLD_DURATION_HOURS) {
    mEccErrorSmiCount++;

    //
    // Disable SMI when number of rank failures per TIME_DURATION hours exceeds threshold
    //
    if (mEccErrorSmiCount > PLATFORM_ERROR_COUNT_THRESHOLD) {//AptioV Server Override: Remove the extra increment for mEccErrorSmiCount.
      for (node = 0; node < MC_MAX_NODE; node++) {
    if ( mELogHost.imc[node].enabled == 0 ) continue;
         DisableSMIForCorrectedMemErrorThreshold (node);
      }
    }
  } else {
    //
    // Reset begining of time period for rank
    //
    EfiSmmGetTime (&mCorrectedErrorRankTimeStamp[failedNode][failedChannel][failedRank]);
  }

  return;
}



VOID
ElogMcExecuteCorrectableErrorHandlingPolicies (
  MEMORY_COMBINED_ERROR_RECORD  *MemoryErrRecord
  )
{

  //
  // 1. Check for SMI throttling for corrected errors
  //

  CorrectedErrorSMIThrottling (MemoryErrRecord);  
  

  return;
}

VOID
EnableDisableINTForEccCorrectedMemErrors (UINT8 Node)
/*++

Routine Description:
    This function enables or disables interrupt generation of memory ecc errors.

Arguments:

  i/p : node number
Returns:

  none

--*/

{
  SMISPARECTL_MC_MAINEXT_STRUCT imcSMISpareCtlReg;

  //
  // Enable INT for sparing, device tagging or corrected memory error threshold
  //

  imcSMISpareCtlReg.Data = mMemRas->MemReadPciCfg(  Node, PARAM_UNUSED, SMISPARECTL_MC_MAINEXT_REG);

  if (mSpareIntSelect & SPARE_INT_SMI) {
      imcSMISpareCtlReg.Bits.intrpt_sel_smi = 1;
  } else {
    imcSMISpareCtlReg.Bits.intrpt_sel_smi = 0;
  }
  if (mSpareIntSelect & SPARE_INT_PIN) {
      imcSMISpareCtlReg.Bits.intrpt_sel_pin = 1;
  } else{
      imcSMISpareCtlReg.Bits.intrpt_sel_pin = 0;
  }
  if (mSpareIntSelect & SPARE_INT_CMCI) {
      imcSMISpareCtlReg.Bits.intrpt_sel_cmci = 1;
  }else {
    imcSMISpareCtlReg.Bits.intrpt_sel_cmci = 0;
  }

  mMemRas->MemWritePciCfg(   Node, PARAM_UNUSED, SMISPARECTL_MC_MAINEXT_REG, imcSMISpareCtlReg.Data);

  return;
}


 /*VOID
ClearJcErrors (UINT8 node, UINT8  VmseCh)
/*++

Routine Description:
    this function clears the JCK detected VMSE channel errors and ddr parity errors.

Arguments:

  i/p : node number, VmseCh number
Returns:

  none

--*/

 /*{

  LOGVCRC00_JCK_STRUCT  logvcrc;
  LOGWECC0_JCK_STRUCT   logwecc0;
  LOGWECC2_JCK_STRUCT   logwecc2;
  LOGCFGRW_JCK_STRUCT   logcfgrw;
  LERR_JCK_STRUCT       lerrjck;
  NERR_JCK_STRUCT       nerrjck;
  FERR_JCK_STRUCT       ferrjck;

  // Clear VMSE CRC Error Log
  logvcrc.Data = mMemRas->ReadJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGVCRC00_JCK_REG);
  logvcrc.Bits.vcmd = 00;
  mMemRas->WriteJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGVCRC00_JCK_REG, logvcrc.Data);

  logvcrc.Data = mMemRas->ReadJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGVCRC01_JCK_REG);
  logvcrc.Bits.vcmd = 00;
  mMemRas->WriteJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGVCRC01_JCK_REG, logvcrc.Data);

  logvcrc.Data = mMemRas->ReadJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGVCRC10_JCK_REG);
  logvcrc.Bits.vcmd = 00;
  mMemRas->WriteJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGVCRC10_JCK_REG, logvcrc.Data);

  logvcrc.Data = mMemRas->ReadJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGVCRC11_JCK_REG);
  logvcrc.Bits.vcmd = 00;
  mMemRas->WriteJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGVCRC11_JCK_REG, logvcrc.Data);

  // Clear VMSE Write ECC Error Log
  logwecc0.Data = mMemRas->ReadJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGWECC0_JCK_REG);
  logwecc0.Bits.crc0  = 00;
  logwecc0.Bits.crc1  = 00;
  logwecc0.Bits.pty0  = 00;
  logwecc0.Bits.pty1  = 00;
  mMemRas->WriteJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGWECC0_JCK_REG, logwecc0.Data);

  logwecc0.Data = mMemRas->ReadJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGWECC1_JCK_REG);
  logwecc0.Bits.crc0  = 00;
  logwecc0.Bits.crc1  = 00;
  logwecc0.Bits.pty0  = 00;
  logwecc0.Bits.pty1  = 00;
  mMemRas->WriteJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGWECC1_JCK_REG, logwecc0.Data);

  logwecc2.Data = mMemRas->ReadJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGWECC2_JCK_REG);
  logwecc2.Bits.wbuff   = 00;
  logwecc2.Bits.cdwrd0  = 00;
  logwecc2.Bits.cdwrd1  = 00;
  logwecc2.Bits.eccmode = 00;
  mMemRas->WriteJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGWECC2_JCK_REG, logwecc2.Data);

  // Clear VMSE Unimplemented Configuration Address Error Log
  logcfgrw.Data = mMemRas->ReadJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGCFGRW_JCK_REG);
  logcfgrw.Bits.cfgwrite  = 00;
  logcfgrw.Bits.cfgaddr = 00;
  logcfgrw.Bits.be    = 00;
  mMemRas->WriteJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGCFGRW_JCK_REG, logcfgrw.Data);

  // Clear VMSE Unimplemented Configuration Address Error Log
  ferrjck.Data = mMemRas->ReadJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, FERR_JCK_REG);
  ferrjck.Data  = 00;
  mMemRas->WriteJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, FERR_JCK_REG, ferrjck.Data);

  nerrjck.Data = mMemRas->ReadJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, NERR_JCK_REG);
  nerrjck.Data  = 00;
  mMemRas->WriteJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, NERR_JCK_REG, nerrjck.Data);

  lerrjck.Data = mMemRas->ReadJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LERR_JCK_REG);
  lerrjck.Data  = 00;
  mMemRas->WriteJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LERR_JCK_REG, lerrjck.Data);

 }
 
VOID
ClearJcLogREccError (UINT8 node, UINT8  VmseCh)
/*++

Routine Description:
    this function clears the ECC Rd error  - Error counters clear not included..

Arguments:

  i/p : node number, VmseCh number
Returns:

  none

--*/
 /*{

  LOGRECC_JCK_STRUCT  logrecc;

  logrecc.Data = mMemRas->ReadJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGRECC_JCK_REG);
  logrecc.Bits.eccmode  = 00;
  logrecc.Bits.chn    = 00;
  logrecc.Bits.rank   = 00;
  logrecc.Bits.coladdr  = 00;
  logrecc.Bits.bank   = 00;
  mMemRas->WriteJckCfg( node, (VmseCh * MAX_JCK_CH), DDRGLOBAL, LOGRECC_JCK_REG, logrecc.Data);

  }*/

