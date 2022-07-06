/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

  Copyright (c) 2012 - 2021, Intel Corporation. All rights reserved.
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  MpstSmm.c

Abstract:

  MPST Support.

--*/
#pragma warning (disable:4005)    // disable multi-define warning for some of the macros
#include "MpstSmm.h"
#include "MemRas.h"
#include "CpuBaseLib.h"

//
// Macros used locally
//
#define ENABLE  1
#define DISABLE 0
#define BCD2DEC(x)  ((x & 0x0F) + ((x >> 4) * 10))

EFI_SMM_BASE2_PROTOCOL            *mSmmBase;
EFI_SMM_SYSTEM_TABLE2             *mSmst;
EFI_MPST_SUPPORT_PROTOCOL         *mMpstSupport;
UINT8                              mTimeElapsed;
UINT8 mPowerState [EFI_ACPI_MEMORY_POWER_NODES_COUNT]  = {0};
extern UINT8 OppSelfRefresh;
UINT32  mDramEnergyStatusRegOffset[MC_MAX_NODE][MAX_CH] = {
    {
        // MC 0
        DRAM_ENERGY_STATUS_CH0_N0_PCU_FUN2_REG,
        DRAM_ENERGY_STATUS_CH1_N0_PCU_FUN2_REG,
        DRAM_ENERGY_STATUS_CH2_N0_PCU_FUN2_REG,
        DRAM_ENERGY_STATUS_CH3_N0_PCU_FUN2_REG
    },
    {
        // MC1
        DRAM_ENERGY_STATUS_CH4_N0_PCU_FUN4_REG,
        DRAM_ENERGY_STATUS_CH5_N0_PCU_FUN4_REG,
        DRAM_ENERGY_STATUS_CH6_N0_PCU_FUN4_REG,
        DRAM_ENERGY_STATUS_CH7_N0_PCU_FUN4_REG,
    }
};

UINT64  mPrevEnergyConsumed[EFI_ACPI_MEMORY_POWER_NODES_COUNT];
UINT64  mPrevPcctEnergyValue[EFI_ACPI_MEMORY_POWER_NODES_COUNT];
UINT64  mOverflowCnt[EFI_ACPI_MEMORY_POWER_NODES_COUNT];
UINT64  mAddOffset[EFI_ACPI_MEMORY_POWER_NODES_COUNT];
UINT64  mSubOffset[EFI_ACPI_MEMORY_POWER_NODES_COUNT];

EFI_STATUS
GenerateSci (
    VOID
)
/*++

Routine Description:
    This function generates an SCI interrupt for Ras OS Consumption

Arguments:
    None

Returns:
    Nothing

--*/
{
  UINT8  GpeCtrl = 0;
  UINT16 PmBase = 0;
  UINT32 PmBaseReg =  (GPIO_BASE_REG & 0xffffff00) | R_ICH_LPC_ACPI_BASE;

  // PM base address
  PmBase = (UINT16)mMemRas->PciReadCfg(0, PmBaseReg);
  PmBase = (UINT16) (PmBase & 0xFF80);
  // Generate SCI event
  mSmst->SmmIo.Io.Read (&gSmst->SmmIo, SMM_IO_UINT8, (PmBase + R_ACPI_GPE_CNTL), 1, &GpeCtrl);
  GpeCtrl |= B_ACPI_GPE_CNTL_SWGPE_CTRL;
  mSmst->SmmIo.Io.Write (&gSmst->SmmIo, SMM_IO_UINT8, (PmBase + R_ACPI_GPE_CNTL), 1, &GpeCtrl);

  return EFI_SUCCESS;
}

EFI_STATUS
EnterPowerState(
  UINT8 node,
  UINT8 ch,
  UINT8 DimmMap,
  UINT8 PowerStateToEnter,
  UINT8 PrevPowerState
)
/*
  This function enters into the state specified from the Active state
  Input
  node - Node Id
  ch   - Ch number
  DimmMap - bit map of Dimms present - bit0=1 dimm0 present, bit1=1 dimm1 present etc..
  PowerStateToEnter - power state to enter CKE low or self refresh or standby
  PrevPowerState - previous state is active
  Output
  Status -  EFI_SUCCESS
            EFI_INVALID_PARAMETER
*/
{
  EFI_STATUS Status = EFI_INVALID_PARAMETER;

  if(PowerStateToEnter == PrevPowerState) {
    return Status;
  }

  switch (PowerStateToEnter) {
    case MPST_POWER_STATE_SR:
      Status = EFI_SUCCESS;
      break;
    default:
      break;
  }
  return Status;
}

EFI_STATUS
ExitPowerState(
  UINT8 node,
  UINT8 ch,
  UINT8 DimmMap,
  UINT8 PowerStateToEnter,
  UINT8 PrevPowerState
)
/*
  This function enters into the state specified from the Active state
  Input
  node - Node Id
  ch   - Ch number
  DimmMap - bit map of Dimms present - bit0=1 dimm0 present, bit1=1 dimm1 present etc..
  PowerStateToEnter - power state to enter CKE low or self refresh or standby
  PrevPowerState - previous state is active
  Output
  Status -  EFI_SUCCESS
            EFI_INVALID_PARAMETER
*/
{
  EFI_STATUS Status = EFI_INVALID_PARAMETER;

  if(PowerStateToEnter == PrevPowerState) {
    return Status;
  }

  switch (PowerStateToEnter) {
    case MPST_POWER_STATE_ACTIVE:
      Status = EFI_SUCCESS;
      break;
    default:
      return Status;
  }

  return Status;
}

EFI_STATUS
MpstSmmPwrStateFunc(
  VOID
)
/*++

Routine Description:
    Function to handle all MPST commands

Arguments:
  None

Returns:
  None
--*/
{
  EFI_STATUS status = EFI_SUCCESS;
  UINT16  memoryPowerNodeId;
  UINT8   node, ch, tad, dimmMap;
  UINT8   tempMc, tempCh, tempSkt, tempDimmSlot, tempDimmRank, tempBank, tempBankGroup, tempChipSelect;
  UINT32  tempRow, tempCol;
  EFI_ACPI_MPST_MAILBOX *mailbox;
  SCRUBADDRESSHI_MC_MAINEXT_STRUCT  scrubHiReg;
  DRAM_ENERGY_STATUS_CH0_PCU_FUN2_STRUCT chEnergy;
  EFI_ACPI_MEMORY_POWER_NODE  *memoryPowerNodePtr;
  UINT64  phyAddr;
  UINT64  energyConsumed;
  UINT16  memoryPowerNodeNum;
  UINT64  equ4gbValue = 00;
  UINT64  currentPcctValue = 00;
  UINT64  offset = 00;
  UINT64  exactPcctValue = 00;
  EFI_ACPI_MEMORY_POWER_STATE_TABLE *mpstTablePtr;

  mailbox = mMpstSupport->MpstMailbox;
  memoryPowerNodeId    = mailbox->MemoryPowerNodeId;
  chEnergy.Data = 00;

  //
  // check if the requested power node id is out of range.
  //
  if(memoryPowerNodeId > EFI_ACPI_MEMORY_POWER_NODES_COUNT) {
    mailbox->Response.Status = MPST_STATUS_INVALID_DATA;
    mailbox->Response.BusyStatus = MPST_RESPONSE_COMPLETED;
    return EFI_SUCCESS;
  }

  node = mMpstSupport->MemPwrNodeInfo[memoryPowerNodeId].Node;
  tad  = mMpstSupport->MemPwrNodeInfo[memoryPowerNodeId].Tad;
  ch   = mMpstSupport->MemPwrNodeInfo[memoryPowerNodeId].Ch;   // no socket and Ch interleave case;
  dimmMap = mMpstSupport->MemPwrNodeInfo[memoryPowerNodeId].DimmMap;
  memoryPowerNodeNum = mMpstSupport->MemPwrNodeInfo[memoryPowerNodeId].MemoryPowerNode;
  mpstTablePtr = (EFI_ACPI_MEMORY_POWER_STATE_TABLE*)mMpstSupport->MpstTable;
  memoryPowerNodePtr = &mpstTablePtr->MemoryPowerNode[memoryPowerNodeNum];


  switch(mailbox->Command){
  case MPST_COMMAND_GET:
    //
    // Get Current power state
    //
    mailbox->PowerState = mMpstSupport->MemoryPowerNodeStatus[memoryPowerNodeId];
    mailbox->Response.Status = MPST_STATUS_SUCCESS;
    mailbox->Response.BusyStatus = MPST_RESPONSE_COMPLETED;
    status = EFI_SUCCESS;
    break;

  case MPST_COMMAND_GET_AVG_PWR_CONSUMED:
    if(( mailbox->PowerState >= EFI_ACPI_MEMORY_POWER_STATES_COUNT) ||
        ((memoryPowerNodePtr->PowerManagedFlag & FLAG_ENABLED) != FLAG_ENABLED)) {
      mMpstSupport->NodeEnergyConsumed.AvgPwr  = (UINT64)-1;
    } else if(mailbox->PowerState == MPST_POWER_STATE_SR) {
      mMpstSupport->NodeEnergyConsumed.AvgPwr =  mTimeElapsed * mMpstSupport->ChPwrConsumed[node][ch];
    } else {
      mMpstSupport->NodeEnergyConsumed.AvgPwr  = mTimeElapsed * mpstTablePtr->PowerStateInfo[memoryPowerNodeId][mailbox->PowerState].AveragePowerConsumed;
    }
    if(mMpstSupport->NodeEnergyConsumed.AvgPwr == 00) {
      mMpstSupport->NodeEnergyConsumed.AvgPwr  = (UINT64)-1;
    }
    status = EFI_SUCCESS;
    break;

  case MPST_COMMAND_GET_MEM_ENERGY_CONSUMED:
    phyAddr = memoryPowerNodePtr->AddressBaseLow + ((UINT64)(memoryPowerNodePtr->AddressBaseHigh) << 0x20);
    if(mMemRas != NULL)  {
      status =  mMemRas->SystemAddressToDimmAddress(phyAddr, &tempSkt, &tempMc, &tempCh, &tempDimmSlot, &tempDimmRank, &tempRow, &tempCol, &tempBank, &tempBankGroup, &tempChipSelect);
      if (EFI_ERROR(status)) {
        mailbox->Response.Status  = MPST_STATUS_ABORTED;
        break; // break from switch statement
      }
      scrubHiReg.Data = mCpuCsrAccess->ReadMemCpuCsr( node, PARAM_UNUSED, SCRUBADDRESSHI_MC_MAINEXT_REG);
      if( tempCh == scrubHiReg.Bits.chnl) {
        mailbox->Response.Status  = MPST_STATUS_PSINPROOG_SUCCESS;
      }
    }

    if(ch < MAX_CH) {
      mMpstSupport->NodeEnergyConsumed.NodeChNum    = node | (ch << 04);
      mMpstSupport->NodeEnergyConsumed.PwrNodeId    = memoryPowerNodeId;
      chEnergy.Data = mCpuCsrAccess->ReadCpuCsr(NodeToSkt(node), 0, mDramEnergyStatusRegOffset[NODE_TO_MC(node)][ch]);
      equ4gbValue = (0x100000000 * mTimeElapsed * 25)/1634;
      energyConsumed  = ((UINT64)chEnergy.Data  * mTimeElapsed * 25 ) / 1634;  // value in mj
      if( mPrevEnergyConsumed[memoryPowerNodeId] > energyConsumed) {  // Overflow case
        mOverflowCnt[memoryPowerNodeId] += 1 ;
      }

      currentPcctValue = (mOverflowCnt[memoryPowerNodeId] * equ4gbValue) + energyConsumed + mAddOffset[memoryPowerNodeId] - mSubOffset[memoryPowerNodeId];
      if (mPrevPcctEnergyValue[memoryPowerNodeId] > currentPcctValue) {
        if( energyConsumed >= mPrevEnergyConsumed[memoryPowerNodeId]) {
          offset = energyConsumed - mPrevEnergyConsumed[memoryPowerNodeId];
        } else {
          offset = ((UINT32)(-1) - mPrevEnergyConsumed[memoryPowerNodeId]) + energyConsumed;
        }

        exactPcctValue = mPrevPcctEnergyValue[memoryPowerNodeId] + offset;

        if(energyConsumed > exactPcctValue) {
          mAddOffset[memoryPowerNodeId] = 00;
          mSubOffset[memoryPowerNodeId] = energyConsumed - exactPcctValue;
        } else {
          mAddOffset[memoryPowerNodeId] = exactPcctValue - energyConsumed;
          mSubOffset[memoryPowerNodeId] = 00;
        }

        currentPcctValue = exactPcctValue;
        mOverflowCnt[memoryPowerNodeId] = 00;

      }

      mPrevPcctEnergyValue[memoryPowerNodeId]       = currentPcctValue;
      mMpstSupport->NodeEnergyConsumed.EnergyValue  = currentPcctValue;
      mPrevEnergyConsumed[memoryPowerNodeId]        = energyConsumed;

    } else {
      mailbox->Response.Status = MPST_STATUS_INVALID;
      mMpstSupport->NodeEnergyConsumed.EnergyValue = (UINT64)-1;
    }
    status = EFI_SUCCESS;
    break;

  case MPST_COMMAND_SET:
    if (mailbox->PowerState == MPST_POWER_STATE_ACTIVE) {
      //
      // Setting ACTIVE state technically means exiting from SR/Standby/Offline
      //
      switch (mMpstSupport->MemoryPowerNodeStatus[memoryPowerNodeId]) {
      case MPST_POWER_STATE_ACTIVE:
        mailbox->Response.Status = EFI_SUCCESS;
        mailbox->Response.BusyStatus = MPST_RESPONSE_COMPLETED;
        break;

      case MPST_POWER_STATE_SR:
        mailbox->Response.BusyStatus = MPST_RESPONSE_VALID; // Set busy status
        status = ExitPowerState(node, ch, dimmMap,mailbox->PowerState, mMpstSupport->MemoryPowerNodeStatus[memoryPowerNodeId]);
        mailbox->Response.Status = (UINT8)EFI_SUCCESS;
        if(status != MPST_STATUS_BUSY) {
          mailbox->Response.BusyStatus = MPST_RESPONSE_COMPLETED;
        }
        mMpstSupport->MemoryPowerNodeStatus[memoryPowerNodeId]= mailbox->PowerState;  //EK-TBD
        break;

      } // switch (mMpstSupport->MemoryPowerNodeStatus[Node])
    } else if (mailbox->PowerState == MPST_POWER_STATE_SR) {

      //
      // It is Set SR or Standy Command
      //
      if (mMpstSupport->MemoryPowerNodeStatus[memoryPowerNodeId] == MPST_POWER_STATE_ACTIVE) {
        mailbox->Response.BusyStatus = MPST_RESPONSE_VALID;
        status = EnterPowerState(node, ch, dimmMap, mailbox->PowerState, mMpstSupport->MemoryPowerNodeStatus[memoryPowerNodeId] );
        mailbox->Response.Status = (UINT8)EFI_SUCCESS;
        if (status != MPST_STATUS_BUSY) {
          mailbox->Response.BusyStatus = MPST_RESPONSE_COMPLETED;
        }
        mMpstSupport->MemoryPowerNodeStatus[memoryPowerNodeId]= mailbox->PowerState;

      } else { // if (mMpstSupport->MemoryPowerNodeStatus[Node] == MPST_POWER_STATE_ACTIVE)
        //
        // Set Command from SR --> Standby; SR --> Offline and Standby --> Offline are NOT supported as of now
        //
        mailbox->PowerState = mMpstSupport->MemoryPowerNodeStatus[memoryPowerNodeId];
        mailbox->Response.Status = EFI_SUCCESS;
        mailbox->Response.BusyStatus = MPST_RESPONSE_COMPLETED;

      } // if (mMpstSupport->MemoryPowerNodeStatus[Node] == MPST_POWER_STATE_ACTIVE)
    } // if (Mailbox->PowerState == MPST_POWER_STATE_ACTIVE)
    status = EFI_SUCCESS;
    break;

  default:
    mailbox->Response.Status = MPST_STATUS_INVALID;
    mailbox->Response.BusyStatus = MPST_RESPONSE_COMPLETED;
    status = EFI_SUCCESS;
    break;
  } // switch mailbox->Command

  return status;

}

VOID
EnableDisableSRandC6(
  UINT8 Flag      // 0 - Disable, 1 - Enable
  )
/*++

Routine Description:
    Enables / Disable SR and C6 based on input; Note the vatriable ch refers to ha channel

Arguments:
  Flag - 0: Disable SR and C6
         1: Enable SR and C6

Returns:
  None
--*/
{
  UINT8 node, ch, i;
  PM_SREF_MCDDC_CTL_STRUCT pm_sref;
  for (node = 0; node < MAX_MEM_NODE; node++) {
    if (mSystemMemoryMap->NodeInfo[node].Enabled == 0) continue;
    for (i = 0; i < MAX_CH; i++) {
      ch = NodeChToDdrCh(node,i); // input - node, i; output HaCh#
      if(ch == 0xFF) continue;
      if(mSystemMemoryMap->NodeInfo[node].ChannelInfo[ch].Enabled == 0) continue;
      if (OppSelfRefresh == 1) {
        pm_sref.Data = mCpuCsrAccess->ReadMemCpuCsr (node, ch, PM_SREF_MCDDC_CTL_REG);
        pm_sref.Bits.sref_en = Flag;
        mCpuCsrAccess->WriteMemCpuCsr (node, ch, PM_SREF_MCDDC_CTL_REG, pm_sref.Data);
      }
    } // ch loop
  } // node loop
}

void
FlushCache(
  void *node
)
{
  AsmWbinvd ();
}

VOID
UpdateNodeChEnergy(
  )
/*++

Routine Description:
    Update Energy data for each DDR CH;
    Note: In this function variable ch refers Ha channel.

Arguments:
  None

Returns:
  None
--*/
{
  EFI_STATUS  status;
  UINTN       numberOfCpus;
  UINTN       numberOfEnabledCpus;
  UINT8       node, haNum, ch, ChEn[MAX_CH], i;
  UINTN	      thread;
  UINT64      Temp;
  BOOLEAN     InSmm;
  DRAM_ENERGY_STATUS_CH0_PCU_FUN2_STRUCT ChEnergy1[MAX_CH] = {0,0,0,0}, ChEnergy2[MAX_CH] = {0,0,0,0};
  EFI_MP_SERVICES_PROTOCOL  *mpServices = NULL;


  //
  // Check if we are executing in SMM
  //
  mSmmBase->InSmm (mSmmBase, &InSmm);

  //
  // Get MP info for this system
  //
  if(InSmm){
    numberOfCpus = mSmst->NumberOfCpus;
  } else {
    //
    // Get the MP protocol
    //
    status = gBS->LocateProtocol (
        &gEfiMpServiceProtocolGuid,
        NULL,
        &mpServices
    );
    ASSERT (mpServices);
    mpServices->GetNumberOfProcessors(mpServices, &numberOfCpus, &numberOfEnabledCpus);
  }

  for (thread = 0; thread < numberOfCpus ; thread++) {
    if(InSmm){
      mSmst->SmmStartupThisAp (FlushCache, thread, NULL);
    } else {
      mpServices->StartupThisAP(
          mpServices,
          (EFI_AP_PROCEDURE) FlushCache,
          thread,
          NULL,
          0,
          NULL,
          NULL);
    }
  }

  AsmWbinvd ();

  EnableDisableSRandC6(DISABLE);

  gBS->Stall(2000);

  for (node = 0; node < MAX_MEM_NODE; node++) {
    if (mSystemMemoryMap->NodeInfo[node].Enabled == 0) continue;

    //
    // Get energy consumed by DDR CH at T1
    //
    for (i = 0; i < MAX_CH; i++) {
      haNum = NodeToHaNum(node);
      ch = NodeChToDdrCh(node,i); // input - node, i; output ha
      if(ch == 0xFF) {
        ChEn[ch]  = 00;
        ChEnergy1[ch].Data  = 00;
        continue;
      }
      ChEn[ch] = (mSystemMemoryMap->NodeInfo[node].ChannelInfo[ch].Enabled) ? 01 : 00;
      ChEnergy1[ch].Data = (ChEn[ch]) ? mCpuCsrAccess->ReadCpuCsr(NodeToSkt(node), 0, mDramEnergyStatusRegOffset[haNum][ch]) : 00;
    }

    gBS->Stall(250000);

    //
    // Get energy consumed by DDR CH at T2
    //
    for(i = 0; i < MAX_CH; i++) {
      haNum = NodeToHaNum(node);
      ch = NodeChToDdrCh(node,i); // input - node, i; output ha
      if(ch == 0xFF) {
        ChEnergy2[ch].Data  = 00;
        continue;
      }
      ChEnergy2[ch].Data = (ChEn[ch]) ? mCpuCsrAccess->ReadCpuCsr(NodeToSkt(node), 0, mDramEnergyStatusRegOffset[haNum][ch]) : 00;
    }

    //
    // Calculate energy consumed from T1 to T2
    //
    for (i = 0; i < MAX_CH; i++) {
      mMpstSupport->ChPwrConsumed[node][i]= 00;
      ch = NodeChToDdrCh(node,i); // input - node, i; output haCH
      if (ChEn[ch] == 00) continue;
      if(ChEnergy2[ch].Data > ChEnergy1[ch].Data)
        mMpstSupport->ChPwrConsumed[node][ch]  = (((UINT64)ChEnergy2[ch].Data - ChEnergy1[ch].Data) * 100) / 1634;  // value in mj
      else  {
        Temp = (0xffffffff - ChEnergy2[ch].Data) + ChEnergy1[ch].Data;
        mMpstSupport->ChPwrConsumed[node][ch]  = (Temp * 100) / 1634;  // value in mj
      }
    } // ch loop
  } // node loop

  EnableDisableSRandC6(ENABLE);
}

EFI_STATUS
EnableEntryExitMpstCallback(
  IN EFI_HANDLE        DispatchHandle,
  IN CONST VOID        *DispatchContext, // optional
  IN OUT   VOID        *CommBuffer,      // optional
  IN OUT   UINTN       *CommBufferSize   // optional
  )
/*++

Routine Description:
    MPST call back function

Arguments:
  DispatchHandle - Dispatch handle (unused for now!)
  Dispatch Context - Context information (unused for now!)
  CommBuffer     - Buffer used for communication between caller/callback function (unused for now!)
  CommBufferSize - size of communication buffer (unused for now!)


Returns:
  status - 0: Success, non-zero: error code
--*/
{
  EFI_STATUS  Status;

  EFI_ACPI_MPST_MAILBOX                    *MpstMailboxSMRAM;
  EFI_ACPI_PCC_SHARED_MEMORY_REGION_MPST   *SharedMemRegionSMRAM;

  ASSERT(mMpstSupport);

  MpstMailboxSMRAM = (EFI_ACPI_MPST_MAILBOX *) mMpstSupport->MpstMailbox;
  SharedMemRegionSMRAM = (EFI_ACPI_PCC_SHARED_MEMORY_REGION_MPST *) mMpstSupport->SharedMemRegion;

  if (!SmmIsBufferOutsideSmmValid ((UINTN)mMpstSupport, sizeof (EFI_MPST_SUPPORT_PROTOCOL))) {
    DEBUG ((EFI_D_ERROR, "mMpstSupport buffer range is in SMRAM!\n"));
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  if (!SmmIsBufferOutsideSmmValid ((UINTN)MpstMailboxSMRAM, sizeof (EFI_ACPI_MPST_MAILBOX))) {
    DEBUG ((EFI_D_ERROR, "MpstMailbox buffer range is in SMRAM!\n"));
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  if (!SmmIsBufferOutsideSmmValid ((UINTN)SharedMemRegionSMRAM, sizeof (EFI_ACPI_PCC_SHARED_MEMORY_REGION_MPST))) {
    DEBUG ((EFI_D_ERROR, "SharedMemRegion buffer range is in SMRAM!\n"));
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  if (!SmmIsBufferOutsideSmmValid ((UINTN)mMpstSupport->MpstTable, sizeof (EFI_ACPI_MEMORY_POWER_STATE_TABLE))) {
    DEBUG ((EFI_D_ERROR, "MpstTable buffer range is in SMRAM!\n"));
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  Status  =  MpstSmmPwrStateFunc();

  return  Status;
}

EFI_STATUS
PcctSwSmiCallBack (
  IN EFI_HANDLE        DispatchHandle,
  IN CONST VOID        *DispatchContext, // optional
  IN OUT   VOID        *CommBuffer,      // optional
  IN OUT   UINTN       *CommBufferSize   // optional
  )
/*++

Routine Description:
    PCCT call back function that handles PCCT requests

Arguments:
  DispatchHandle - Dispatch handle (unused for now!)
  Dispatch Context - Context information (unused for now!)
  CommBuffer     - Buffer used for communication between caller/callback function (unused for now!)
  CommBufferSize - size of communication buffer (unused for now!)


Returns:
  status - 0: Success, non-zero: error code
--*/
{
  EFI_STATUS Status = EFI_SUCCESS;

  EFI_ACPI_MPST_MAILBOX                    *MpstMailboxSMRAM;
  EFI_ACPI_PCC_SHARED_MEMORY_REGION_MPST   *SharedMemRegionSMRAM;

  MpstMailboxSMRAM = (EFI_ACPI_MPST_MAILBOX *) mMpstSupport->MpstMailbox;
  SharedMemRegionSMRAM = (EFI_ACPI_PCC_SHARED_MEMORY_REGION_MPST *) mMpstSupport->SharedMemRegion;

  if (!SmmIsBufferOutsideSmmValid ((UINTN)mMpstSupport, sizeof (EFI_MPST_SUPPORT_PROTOCOL))) {
    DEBUG ((EFI_D_ERROR, "mMpstSupport buffer range is in SMRAM!\n"));
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  if (!SmmIsBufferOutsideSmmValid ((UINTN)MpstMailboxSMRAM, sizeof (EFI_ACPI_MPST_MAILBOX))) {
    DEBUG ((EFI_D_ERROR, "MpstMailbox buffer range is in SMRAM!\n"));
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  if (!SmmIsBufferOutsideSmmValid ((UINTN)SharedMemRegionSMRAM, sizeof (EFI_ACPI_PCC_SHARED_MEMORY_REGION_MPST))) {
    DEBUG ((EFI_D_ERROR, "SharedMemRegion buffer range is in SMRAM!\n"));
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  if (!SmmIsBufferOutsideSmmValid ((UINTN)mMpstSupport->MpstTable, sizeof (EFI_ACPI_MEMORY_POWER_STATE_TABLE))) {
    DEBUG ((EFI_D_ERROR, "MpstTable buffer range is in SMRAM!\n"));
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  SharedMemRegionSMRAM->Status.Bits.SciDoorBell = 00;
  SharedMemRegionSMRAM->Status.Data = 0;

  if(&mMpstSupport == NULL) {
    SharedMemRegionSMRAM->Status.Bits.CmdComplete = 0;
    SharedMemRegionSMRAM->Status.Bits.Error = 1;
    SharedMemRegionSMRAM->MemPowerStateSts  = PCC_MEM_POWER_STATE_STS_ABORTED;
    return Status;
  }

  if(SharedMemRegionSMRAM->Command.Bits.Cmd !=03) {
    SharedMemRegionSMRAM->Status.Bits.CmdComplete = 1;
    SharedMemRegionSMRAM->Status.Bits.Error = 1;
    SharedMemRegionSMRAM->MemPowerStateSts  = PCC_MEM_POWER_STATE_STS_NOT_VALID;
    return Status;
  }

  SharedMemRegionSMRAM->Status.Bits.CmdComplete = 1;
  SharedMemRegionSMRAM->MemPowerStateSts  = PCC_MEM_POWER_STATE_STS_SUCCESS;

  MpstMailboxSMRAM->MemoryPowerNodeId  = (UINT16)SharedMemRegionSMRAM->MemPowerNodeId;

  if(SharedMemRegionSMRAM->MemPowerStateCmd == PCC_SBSTANCE_MPST_GET_PWR_STATE) {

    MpstMailboxSMRAM->Command  = MPST_COMMAND_GET;
    MpstSmmPwrStateFunc();
    SharedMemRegionSMRAM->PowerStateId = MpstMailboxSMRAM->PowerState;

    if(MpstMailboxSMRAM->Response.Status == MPST_STATUS_PSINPROOG_SUCCESS) {
      SharedMemRegionSMRAM->MemPowerStateSts  = MPST_STATUS_PSINPROOG_SUCCESS;
      MpstMailboxSMRAM->Response.Status    = MPST_STATUS_SUCCESS;
    }

  } else if(SharedMemRegionSMRAM->MemPowerStateCmd == PCC_SBSTANCE_MPST_SET_PWR_STATE) {

    MpstMailboxSMRAM->Command  = MPST_COMMAND_SET;
    MpstMailboxSMRAM->PowerState  = (UINT8)SharedMemRegionSMRAM->PowerStateId;
    MpstSmmPwrStateFunc();

  }  else if(SharedMemRegionSMRAM->MemPowerStateCmd == PCC_SBSTANCE_MPST_GET_MEM_ENERGY_CONSUMED) {

    MpstMailboxSMRAM->Command  = MPST_COMMAND_GET_MEM_ENERGY_CONSUMED;
    MpstMailboxSMRAM->PowerState  = (UINT8)SharedMemRegionSMRAM->PowerStateId;
    MpstSmmPwrStateFunc();
    SharedMemRegionSMRAM->MemEnergyConsumed  = mMpstSupport->NodeEnergyConsumed.EnergyValue;
    if(MpstMailboxSMRAM->Response.Status == MPST_STATUS_PSINPROOG_SUCCESS) {
      SharedMemRegionSMRAM->MemPowerStateSts  = MPST_STATUS_PSINPROOG_SUCCESS;
      MpstMailboxSMRAM->Response.Status    = MPST_STATUS_SUCCESS;
    }

  } else if( SharedMemRegionSMRAM->MemPowerStateCmd == PCC_SBSTANCE_MPST_GET_AVG_PWR_CONSUMED) {
    MpstMailboxSMRAM->Command  = MPST_COMMAND_GET_AVG_PWR_CONSUMED;
    MpstMailboxSMRAM->PowerState  = (UINT8)SharedMemRegionSMRAM->PowerStateId;
    MpstSmmPwrStateFunc();
    SharedMemRegionSMRAM->ExpectedAvgPwrConsumed  = mMpstSupport->NodeEnergyConsumed.AvgPwr;
    if(mMpstSupport->NodeEnergyConsumed.AvgPwr == (UINT64)-1) {
      SharedMemRegionSMRAM->MemPowerStateSts        = PCC_MEM_POWER_STATE_STS_INVALID_DATA;
      SharedMemRegionSMRAM->Status.Bits.Error = 1;
      SharedMemRegionSMRAM->ExpectedAvgPwrConsumed  = (UINT64)-1;
    } else {
      if(MpstMailboxSMRAM->Response.Status == MPST_STATUS_PSINPROOG_SUCCESS) {
        SharedMemRegionSMRAM->MemPowerStateSts  = MPST_STATUS_PSINPROOG_SUCCESS;
      }
      MpstMailboxSMRAM->Response.Status    = MPST_STATUS_SUCCESS;
    }

  } else  {
      SharedMemRegionSMRAM->MemPowerStateSts  = PCC_MEM_POWER_STATE_STS_NOT_SUPPORTED;
      SharedMemRegionSMRAM->Status.Bits.Error = 1;
  }

  if(MpstMailboxSMRAM->Response.Status != MPST_STATUS_SUCCESS)
    SharedMemRegionSMRAM->Status.Bits.Error = 1;

  // Notify SCI
  if( SharedMemRegionSMRAM->Command.Bits.GenerateSci == 1)  {
    SharedMemRegionSMRAM->Status.Bits.SciDoorBell = 1;
    GenerateSci();
  }
  return Status;
}

VOID
UpdateMpst (
 )
/*++

Routine Description:
    Update MPST data structures

Arguments:
  None

Returns:
  None

--*/
{
  UINT8 data8;
  //
  // Port 0x74 is alias of port 0x70 which is write only
  //
  mSmst->SmmIo.Io.Read (&mSmst->SmmIo, SMM_IO_UINT8, 0x74, 1, &data8 );
  data8 &= 0x80;      // read seconds
  mSmst->SmmIo.Io.Write (&mSmst->SmmIo, SMM_IO_UINT8, 0x70, 1, &data8);
  //
  // Data is at port 0x71
  //
  mSmst->SmmIo.Io.Read (&mSmst->SmmIo, SMM_IO_UINT8, 0x71, 1, &data8 );

  mTimeElapsed  = BCD2DEC(data8);  // in seconds.
  if(mTimeElapsed == 0) {
    mTimeElapsed = 1;
  }
  UpdateNodeChEnergy();
}

VOID
EFIAPI
InitializeMpstCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
/*++

Routine Description:
   Call back function that will be called when
   MPST protocol is installed

Arguments:
  Event - Event data (unused here)
  Context - ptr to context data (unused here)

Returns:
  None

--*/
{

  EFI_STATUS                    Status;

  //
  // Get the MPST Mailbox address in a module variable in SMRAM for the runtime SMI handler.
  //
  if(mMpstSupport == NULL){
    Status = gBS->LocateProtocol (&gEfiMpstSupportProtocolGuid, NULL, &mMpstSupport);
    ASSERT_EFI_ERROR (Status);
  }

  UpdateMpst();

}

EFI_STATUS
InitializeMpst(
    VOID
 )
/*++

Routine Description:
    Master Function that initializes MPST data structures

Arguments:
  None

Returns:
  None

--*/
{
  EFI_EVENT   mpstProtocolEvent;
  VOID        *mpstProtocolEventRegistration = NULL;
  EFI_STATUS                    status;
  EFI_SMM_SW_DISPATCH2_PROTOCOL *swDispatch = NULL;
  EFI_SMM_SW_REGISTER_CONTEXT   *swContext = NULL;
  EFI_HANDLE                    swHandle = NULL;

  //
  // Find the SMM base protocol
  //
  status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, &mSmmBase);
  ASSERT_EFI_ERROR (status);
  mSmmBase->GetSmstLocation (mSmmBase, &mSmst);

  //
  // Install the SMI handler for the SW SMI with MPST command.
  //
  status = mSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, &swDispatch);
  ASSERT_EFI_ERROR (status);

  status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (EFI_SMM_SW_REGISTER_CONTEXT), &swContext);
  ASSERT_EFI_ERROR (status);
  swContext->SwSmiInputValue = EFI_GET_MEMORY_POWER_STAGES_ENTRY_EXIT;
  swHandle = NULL;
  status = swDispatch->Register (swDispatch, EnableEntryExitMpstCallback, swContext, &swHandle);
  ASSERT_EFI_ERROR (status);

  status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (EFI_SMM_SW_REGISTER_CONTEXT), &swContext);
  ASSERT_EFI_ERROR (status);
  swContext->SwSmiInputValue = EFI_SET_MEMORY_POWER_STAGES_ENTRY_EXIT;
  swHandle = NULL;
  status = swDispatch->Register (swDispatch, EnableEntryExitMpstCallback, swContext, &swHandle);
  ASSERT_EFI_ERROR (status);

  status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (EFI_SMM_SW_REGISTER_CONTEXT), &swContext);
  ASSERT_EFI_ERROR (status);
  swContext->SwSmiInputValue = PCCT_SWSMI_VALUE;
  swHandle = NULL;
  status = swDispatch->Register (swDispatch, PcctSwSmiCallBack, swContext, &swHandle);
  ASSERT_EFI_ERROR (status);

  //
  // Get the MPST Mailbox address in a module variable in SMRAM for the runtime SMI handler.
  //
  mMpstSupport = NULL;
  status = gBS->LocateProtocol (&gEfiMpstSupportProtocolGuid, NULL, &mMpstSupport);

  if(mMpstSupport == NULL){
    //
    // Register a notify event on MPST Protocol. The callback function will be
    // invoked when the MPST protocol is installed
    //
    status = gBS->CreateEvent (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, InitializeMpstCallback, NULL, &mpstProtocolEvent);
    ASSERT_EFI_ERROR (status);
    status = gBS->RegisterProtocolNotify(&gEfiMpstSupportProtocolGuid, mpstProtocolEvent, &mpstProtocolEventRegistration);
    ASSERT_EFI_ERROR (status);
  } else {
    UpdateMpst();
  }

  return status;
}
