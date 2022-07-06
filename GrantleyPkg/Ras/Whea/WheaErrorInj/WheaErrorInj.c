/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

Copyright (c) 2009-2021 Intel Corporation.  All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license.  Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:
    WheaErrorInj.c

Abstract:
    This is an implementation of the Core Whea error injection method.

-----------------------------------------------------------------------------*/

  #include "WheaErrorInj.h"
  #include <Library\TimerLib.h>

#pragma optimize("", off)

// SSG requested to validate the plug-in with error type 0x1000(bit12 set) to inject memory error
#define WHEA_TEMP_MEM_ERROR_TYPE      0x1000  // for IFU , DCU error injections
#define WHEA_VENDOR_EXT_TYPE          0x80000000  // Intel specified OEM defined errro types
#define WHEA_MEM_ERROR_REC_TYPE     0x1
#define WHEA_PATROL_SCRUB_ERROR     0x2

// PCI-EX error injection bits
#define CAUSE_CTOERR_BIT        04    // UnCorrectable error injection test
#define CAUSE_RCVERR_BIT        02    //  Correctable error injection test
#define ERRINJDIS_BIT           01    //  Error injection disable bit

// Error consume types
#define WHEA_NO_CONSUME                 0x00
#define WHEA_REC_PATROLSCRUB_ERROR      0x01
#define WHEA_CORE_IFU_DCU_ERR           0x02
#define WHEA_MEM_COR_ERROR              0x03

// EINJ - SET_ERROR_TYPE_WITH_ADDRESS Data Structure flags
#define WHEA_SET_ERROR_TYPE_WITH_PROC_APIC_VALID      0x01
#define WHEA_SET_ERROR_TYPE_WITH_MEMORY_ADDRESS_VALID 0x02
#define WHEA_SET_ERROR_TYPE_WITH_SBDF_VALID           0x04

// BIOS runtime Address range used for error injection
#define WHEA_EINJ_ADDR_RANGE        0x4000

// GLobal variables and protocols
UINT8                                   gUnlockMsr;  //AptioV server override -IDK tool on-die error injection failure after Memory CE injected using EINJ method
BOOLEAN                                 mAcpi5Support = FALSE;
BOOLEAN                                 mPcieErrInjActionTable = FALSE;
UINT8                                   mErrorExpected = WHEA_NO_CONSUME; // Memory Error consume
UINT8                                   *aMemoryLlcErrInjAddr;
UINT64                                  mBiosRsvdInjAddr;
EFI_SMM_BASE2_PROTOCOL                  *mSmmBase = NULL;
EFI_SMM_CPU_SERVICE_PROTOCOL            *mSmmCpuServiceProtocol; // 10770
EFI_SMM_SYSTEM_TABLE2                   *mSmst = NULL;
STATIC  VOID                            *mWheaEventReg;
STATIC  EFI_EVENT                       mWheaEvent;
STATIC  BOOLEAN                         mWheaPlatformSupportAvailable = FALSE;
STATIC  EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL  *mWheaPlatformSupport = NULL;
STATIC  WHEA_EINJ_PARAM_BUFFER          *mEinjParam = NULL;
STATIC  VOID                            *mEinjEsts = NULL;
STATIC  WHEA_EINJ_TRIGGER_ACTION_TABLE  *mEinjAction = NULL;
SOCKET_NODE_INFO                        SocketNodeInfo;
IIO_UDS                                 *mIioUds          = NULL;
EFI_MEM_RAS_PROTOCOL                    *mMemRas = NULL;
EFI_CPU_CSR_ACCESS_PROTOCOL             *mCpuCsrAccess;
UINT32                                  mInjAddrValue = 0xDEADBEEF;
SPIN_LOCK                               mApLock; //10770
UINT64                                  mTsegBase;
UINT64                                  mTsegSize;
UINT64                                  mPhyAddr;
//
// WHEA Serialization Table
//
STATIC
EFI_ACPI_WHEA_SERIALIZATION_ACTION mSimEinj[9] = {
  //
  // Serialization Action Table
  //
  {   //Action0
    WHEA_EINJ_BEGIN_INJECT_OP,          // BEGIN_INJECTION_OPERATION
    INSTRUCTION_WRITE_REGISTER_VALUE,   // WRITE_REGISTER_VALUE
    FLAG_PRESERVE_REGISTER,             // Flags
    0x00,                               // Reserved
    EinjRegisterFiller,                 // {0x00, 0x20, 0x00, 0x03, -1},    // GAS (DWORD Memory) Address will be filled during boot
    EINJ_BEGIN_OPERATION,               // Value for InjectError()
    0xffffffff                          // Mask is only valid for 32-bits
  },
  {   //Action1
    WHEA_EINJ_GET_TRIGGER_ACTION_TABLE, // GET_TRIGGER_ERROR_STRUC
    INSTRUCTION_READ_REGISTER,          // READ_REGISTER
    FLAG_NOTHING,                       // Flags
    0x00,                               // Reserved
    EinjRegisterFiller,                 // {0x00, 0x40, 0x00, 0x04, -1},    // GAS (QWORD Memory) Address will be filled during boot
    0,                                  // Value for InjectError()
    0xffffffffffffffff                  // Mask is only valid for 32-bits
  },
  {   //Action2
    WHEA_EINJ_SET_ERROR_TYPE,           // SET_ERROR_TYPE
    INSTRUCTION_WRITE_REGISTER,         // WRITE_REGISTER
    FLAG_PRESERVE_REGISTER,             // Flags
    0x00,                               // Reserved
    EinjRegisterFiller,                 // {0x00, 0x20, 0x00, 0x03, -1},    // GAS (QWORD Memory) Address will be filled during boot
    0,                                  // Value for InjectError()
    0xffffffff                          // Mask is only valid for 32-bits
  },
  {   //Action3
    WHEA_EINJ_GET_ERROR_TYPE,           // GET_ERROR_TYPE
    INSTRUCTION_READ_REGISTER,          // READ_REGISTER
    FLAG_NOTHING,                       // Flags
    0x00,                               // Reserved
    EinjRegisterFiller,                 //{0x00, 0x20, 0x00, 0x03, -1},    // GAS (QWORD Memory) Address will be filled during boot
    0,                                  // Value for InjectError()
    0xffffffff                          // Mask is only valid for 32-bits
  },
  {   //Action4
    WHEA_EINJ_END_INJECT_OP,            // END_OPERATION
    INSTRUCTION_WRITE_REGISTER_VALUE,   // WRITE_REGISTER_VALUE
    FLAG_PRESERVE_REGISTER,             // Flags
    0x00,                               // Reserved
    EinjRegisterFiller,                 //{0x00, 0x20, 0x00, 0x03, -1},    // GAS (QWORD Memory) Address will be filled during boot
    EINJ_END_OPERATION,                 // Value for InjectError()
    0xffffffff                          // Mask is only valid for 32-bits
  },
  {   //Action5
    WHEA_EINJ_EXECUTE_INJECT_OP,        // EXECUTE_OPERATION
    INSTRUCTION_WRITE_REGISTER_VALUE,   // WRITE_REGISTER_VALUE
    FLAG_PRESERVE_REGISTER,             // Flags
    0x00,                               // Reserved
  {0x01, 0x10, 0x00, 0x02, R_APM_CNT},  // GAS (WORD IO). Address will be filled in runtime
  EFI_WHEA_EINJ_EXECUTE_SWSMI,          // Value for InjectError()
  0xffff                                // Mask is only valid for 16-bits
  },
  {   //Action6
    WHEA_EINJ_CHECK_BUSY_STATUS,        // CHECK_BUSY_STATUS
    INSTRUCTION_READ_REGISTER_VALUE,    // READ_REGISTER
    FLAG_NOTHING,                       // Flags
    0x00,                               // Reserved
    EinjRegisterFiller,                 // {0x00, 0x30, 0x00, 0x03, -1},    // GAS (QWORD Memory) Address will be filled during boot
    0x00000001,                         // Value for InjectError()
    0x00000001                          // Mask is only valid for 32-bits
  },
  {   //Action7
    WHEA_EINJ_GET_CMD_STATUS,           // GET_OPERATION_STATUS
    INSTRUCTION_READ_REGISTER,          // READ_REGISTER
    FLAG_PRESERVE_REGISTER,             // Flags
    0x00,                               // Reserved
    EinjRegisterFiller,                 // {0x00, 0x40, 0x00, 0x03, -1},    // GAS (QWORD Memory) Address will be filled during boot
    0,                                  // Value for InjectError()
    0x000001fe                          // Mask is only valid for 32-bits
  },
  {   //Action8
    WHEA_EINJ_SET_ERROR_TYPE_WITH_ADDRSS,  // SET_ERROR_TYPE_WITH_ADDRESS
    INSTRUCTION_WRITE_REGISTER,         // WRITE_REGISTER
    FLAG_PRESERVE_REGISTER,             // Flags
    0x00,                               // Reserved
    EinjRegisterFiller,                 // {0x00, 0x20, 0x00, 0x03, -1},    // GAS (QWORD Memory) Address will be filled during boot
    0,                                  // Value for InjectError()
    0xffffffff                          // Mask is only valid for 32-bits
  },
};

STATIC
WHEA_EINJ_TRIGGER_ACTION_TABLE mSimEinjAction = \
{
  {sizeof (WHEA_ERROR_TRIGGER_ACTION_HEADER), 0, sizeof (WHEA_EINJ_TRIGGER_ACTION_TABLE), 4},
  { //Action0
    WHEA_EINJ_TRIGGER_ERROR_ACTION,
    INSTRUCTION_NO_OPERATION,         // Intialized as NOP. To be filled by the runtime injection code
    FLAG_NOTHING,                     // Flags
    0x00,                             // Reserved
    EinjRegisterFiller,               // {0x00, 0x20, 0x00, 0x03, -1},    // GAS (DWORD Memory) Address will be filled during boot
    0,                                // Value for InjectError()
    0xffffffffffffffff                // Mask is only valid for 32-bits
  },
  { //Action1
    WHEA_EINJ_TRIGGER_ERROR_ACTION,
    INSTRUCTION_NO_OPERATION,         // Intialized as NOP. To be filled by the runtime injection code
    FLAG_NOTHING,                     // Flags
    0x00,                             // Reserved
    EinjRegisterFiller,               // {0x00, 0x20, 0x00, 0x03, -1},    // GAS (DWORD Memory) Address will be filled during boot
    0,                                // Value for InjectError()
    0xffffffff                        // Mask is only valid for 32-bits
  },
  { //Action2
    WHEA_EINJ_TRIGGER_ERROR_ACTION,
    INSTRUCTION_NO_OPERATION,         // Intialized as NOP. To be filled by the runtime injection code
    FLAG_NOTHING,                     // Flags
    0x00,                             // Reserved
    EinjRegisterFiller,               // {0x00, 0x20, 0x00, 0x03, -1},    // GAS (DWORD Memory) Address will be filled during boot
    0,                                // Value for InjectError()
    0xffffffff                        // Mask is only valid for 32-bits
  },
  { //Action3
    WHEA_EINJ_TRIGGER_ERROR_ACTION,
    INSTRUCTION_NO_OPERATION,         // Write register
    FLAG_NOTHING,                     // Flags
    0x00,                             // Reserved
    EinjRegisterFiller,               // {0x00, 0x20, 0x00, 0x03, -1},    // GAS (DWORD Memory) Address will be filled during boot
    0,                                // Value for InjectError()
    0xffffffff                        // Mask is only valid for 32-bits
  }
};

///////////////////////////////////////////////////////////////////////////////////////
//
// Driver code/functions that installs all Error Sources (Common) in the platform & chipset
//
///////////////////////////////////////////////////////////////////////////////////////
void
UnLockInjLogic(
  UINT8 *node
)
{
  UINT64      Data64;

  Data64 = AsmReadMsr64 (MC_ERR_INJ_LCK);
  Data64 &= ~(1);
  AsmWriteMsr64 (MC_ERR_INJ_LCK, Data64);

}

void
LockInjLogic(
  UINT8 *node
)
{
  UINT64      Data64;

  Data64 = AsmReadMsr64 (MC_ERR_INJ_LCK);
  Data64 |= 1;
  AsmWriteMsr64 (MC_ERR_INJ_LCK, Data64);

}

void
DisablePsmiAndPrefetch (
    UINT8 *node
)
{

  DisablePsmi();
  DisablePrefetch();

}

void
DisablePoison(
  UINT8 *node
)
{
  UINT64      Data;
  Data = AsmReadMsr64(0x178);
  Data &=  ~(BIT0);      // just enable poison
  AsmWriteMsr64 (0x178, Data);

}

VOID
SeeErrorAp(
  UINT8 *Skt)
{

  // Write data to memory to seed error in memeory
  *( UINT32 *)(UINTN)(mPhyAddr) = mInjAddrValue;
  AsmWbinvd();
  //AsmFlushCacheLine ((VOID*)PhyAddr);
  //AsmMfence();
  ReleaseSpinLock(&mApLock);
}

void
EnablePoison(
  UINT8 *node
)
{
  UINT64      Data;
  Data = AsmReadMsr64(0x178);
  Data |=  BIT0;        // just enable poison
  AsmWriteMsr64 (0x178, Data);

}

void
DisableCMCI(
  UINT8 *node
)
{
  UINT64      Data;
  Data = AsmReadMsr64(0x17F);

}

void
ClearDisableCMCI(
  UINT8 *node
)
{
  UINT64      Data;

  Data = AsmReadMsr64(0x17F);
  Data &= (~BIT4);
  AsmWriteMsr64 (0x17f, Data);

}

VOID
InjectMemoryError (
 UINT32   ErrorToInject
)

/*++

Routine Description: This function injects Memory ECC error in the cache

Arguments:

    ErrType : 0x08 - for correctable error injection
              0x10 - for uncorectable error injection

Returns:

    none

--*/
{

  UINT8         node=0, thread;
  UINT8         SocketId;
  UINT8         MemoryControllerId;
  UINT8         ch;
  UINT8         DimmSlot;
  UINT8         DimmRank, PhyRank;
  UINT8         Bank;
  UINT8         BankGroup;
  UINT8         ChipSelect;
  UINT32        RankAddr = 00;
  UINT32        Row;
  UINT32        Col;
  UINT32        SCRUBCTL_BDF_Offset = 0x990A0;
  UINT32        DevOffset = 0;
  RSP_FUNC_CRC_ERR_INJ_EXTRA_MCDDC_DP_STRUCT    EnjExtra;
  //RSP_FUNC_CRC_ERR_INJ_DEV0_XOR_MSK_MCDDC_DP_STRUCT  Dev0XorMsk;
  //RSP_FUNC_CRC_ERR_INJ_DEV1_XOR_MSK_MCDDC_DP_STRUCT  Dev1XorMsk;
  RSP_FUNC_ADDR_MATCH_LO_MCDDC_DESC_STRUCT      MatchLoReg;
  RSP_FUNC_ADDR_MATCH_HI_MCDDC_DESC_STRUCT      MatchHiReg;
  RSP_FUNC_ADDR_MASK_LO_MCDDC_DESC_STRUCT       MaskLoReg;
  RSP_FUNC_ADDR_MASK_HI_MCDDC_DESC_STRUCT       MaskHiReg;
  SCRUBADDRESSHI_MC_MAINEXT_STRUCT              imcScrubHi;
    SCRUBADDRESSLO_MC_MAINEXT_HSX_BDX_STRUCT    imcScrubLo;
  SCRUBCTL_MC_MAINEXT_STRUCT                    imcSCRUBCTL, restoreImcScrubctl;
  //EMASK_JCK_STRUCT                              emask;
  //EMASK_JCK_STRUCT                              emaskSaved[MAX_MEM_NODE][MAX_CH];
  EFI_STATUS                                    Status = EFI_SUCCESS;
  BOOLEAN                                       FoundThread = FALSE; //10770
  EFI_PROCESSOR_INFORMATION                     ProcInfo;   //10770

  //Step 1.  Unlock error injection mechanism on socket basis
  // Scope is package level. No need to do on each thread.
  for (thread = 0; thread < mSmst->NumberOfCpus; thread++) {
    mSmst->SmmStartupThisAp (UnLockInjLogic,
                  thread,
                  &node);
  }
  UnLockInjLogic(&node);

  // mPhyAddr contains the error injection Address
  mPhyAddr = mEinjParam->EinjAddr;
  mInjAddrValue  = 0xDEADBEEF;
  if (mErrorExpected == WHEA_MEM_COR_ERROR) {
    mInjAddrValue = *( UINT32 *)(UINTN)(mPhyAddr);
  }

  if(mMemRas == NULL){
    mEinjParam->CmdStatus  = WHEA_EINJ_CMD_INVALID_ACCESS;
    return;
  }
/*
    // Set vmseweccerr mask bit
  for (node = 0; node < MAX_MEM_NODE; node++) {
    if ( SocketNodeInfo.imc[node].enabled == 0 ) continue;

    for (ch = 0; ch < MAX_CH; ch=ch+2) {
      if (SocketNodeInfo.imc[node].channelList[ch].chEnabled == 0) continue;

        emask.Data    = mMemRas->ReadJckCfg(node, (ch & 0xfe), DDRGLOBAL, EMASK_JCK_REG);
        emaskSaved[node][ch].Data = emask.Data;
        Enable other registers if needed. Not changing the others.
        emask.Bits.vmseweccerr = 1;
       mMemRas->WriteJckCfg ( node, (ch & 0xfe), DDRGLOBAL, EMASK_JCK_REG, emask.Data);
    }
  }*/

  //
  // Check if the address belongs to SMRAM
  //
  if ((mPhyAddr >= mTsegBase) && (mPhyAddr <= mTsegBase + mTsegSize)) {
    mEinjParam->CmdStatus = WHEA_EINJ_CMD_INVALID_ACCESS;
    DEBUG((DEBUG_ERROR, " InjectMemoryError - Address belongs to SMRAM.  EINJ tool cannot perform Error injection in SMRAM\n"));
    return;
  }

  //
  // Translate physical address to DIMM
  //
    DEBUG ((DEBUG_ERROR, "InjectMemoryError -Addr = %lx \n",mPhyAddr));

  Status = mMemRas->SystemAddressToDimmAddress (mPhyAddr,&SocketId,&MemoryControllerId,&ch,&DimmSlot,&DimmRank,&Row,&Col,&Bank,&BankGroup,&ChipSelect);
  if(EFI_ERROR(Status))   {
    mEinjParam->CmdStatus  = WHEA_EINJ_CMD_INVALID_ACCESS;
    DEBUG ((DEBUG_ERROR, "InjectMemoryError - Address tarnslation error \n"));
    return;
  }
  DEBUG ((DEBUG_ERROR, "InjectMemoryError: SktId = 0x%x, mc =0x%x \n",SocketId,MemoryControllerId));
  DEBUG ((DEBUG_ERROR,"InjectMemoryError: ch = 0x%x, dimmslot = 0x%x, DimmRank = 0x%x,\n",ch,DimmSlot,DimmRank));
  DEBUG ((DEBUG_ERROR,"InjectMemoryError: Row = 0x%x, Col = 0x%x,Bank = 0x%x \n",Row,Col,Bank));

  PhyRank = (DimmSlot << 2) + DimmRank;
  node = SKTMC_TO_NODE(SocketId, MemoryControllerId);
  restoreImcScrubctl.Data = mMemRas->MemReadPciCfg(node, PARAM_UNUSED, SCRUBCTL_MC_MAINEXT_REG);
  // Disable Patrol scrub
  mMemRas->DisablePatrolScrubEngine(node);
  DEBUG ((DEBUG_ERROR, " InjectMemoryError -Addr = %lx, Node = %x, Ch = %x , PhyRank = %x, \n", mPhyAddr, node, ch, PhyRank ));

  // Invalidate the line in memory
  *(volatile  UINT32 *)(UINTN)(mPhyAddr) = 0x12345678;
  AsmFlushCacheLine ((VOID*)mPhyAddr);

  imcSCRUBCTL.Data = 00;

  if( mErrorExpected == WHEA_REC_PATROLSCRUB_ERROR ) {
    imcSCRUBCTL.Data = mMemRas->MemReadPciCfg(node, PARAM_UNUSED, SCRUBCTL_MC_MAINEXT_REG);
    // Set Patrol Scrub scrubinterval to run scrubbing faster.
    imcSCRUBCTL.Bits.scrub_en     = 1;
    imcSCRUBCTL.Bits.startscrub   = 1;
    imcSCRUBCTL.Bits.scrubinterval= 0x350;

    imcScrubHi.Data = mMemRas->MemReadPciCfg(node, PARAM_UNUSED, SCRUBADDRESSHI_MC_MAINEXT_REG);
    imcScrubHi.Bits.rank = PhyRank;
    imcScrubHi.Bits.chnl = ch;
    mMemRas->MemWritePciCfg(node, PARAM_UNUSED, SCRUBADDRESSHI_MC_MAINEXT_REG, imcScrubHi.Data);

    imcScrubLo.Data     = mMemRas->MemReadPciCfg(node, PARAM_UNUSED, SCRUBADDRESSLO_MC_MAINEXT_REG);
    imcScrubLo.Bits.rankadd = RankAddr;
    mMemRas->MemWritePciCfg(node, PARAM_UNUSED, SCRUBADDRESSLO_MC_MAINEXT_REG, imcScrubLo.Data);

  }

  EnjExtra.Data = mMemRas->MemReadPciCfg (node, ch, RSP_FUNC_CRC_ERR_INJ_EXTRA_MCDDC_DP_REG);
  EnjExtra.Bits.crc_err_inj_dev0_5_bits = 01;
  EnjExtra.Bits.crc_err_inj_chunk_dev0 = 3; // TO be clarified value 3 - inject on both the chunks.
  EnjExtra.Bits.crc_err_inj_chunk_dev1 = 0; // TO be clarified value 0 - no injection.

  if( (ErrorToInject == INJECT_ERROR_MEMORY_UE_NON_FATAL) || (ErrorToInject == INJECT_ERROR_MEMORY_UE_FATAL)) {
    EnjExtra.Bits.crc_err_inj_dev1_5_bits = 05;
    EnjExtra.Bits.crc_err_inj_chunk_dev1 = 3; // TO be clarified value 3 - inject on both the chunks.
    mMemRas->MemWritePciCfg (node, ch, RSP_FUNC_CRC_ERR_INJ_DEV1_XOR_MSK_MCDDC_DP_REG,04);
  } else {
    mMemRas->MemWritePciCfg (node, ch, RSP_FUNC_CRC_ERR_INJ_DEV1_XOR_MSK_MCDDC_DP_REG,0);
  }
  mMemRas->MemWritePciCfg (node, ch, RSP_FUNC_CRC_ERR_INJ_DEV0_XOR_MSK_MCDDC_DP_REG, 01);
  mMemRas->MemWritePciCfg (node, ch, RSP_FUNC_CRC_ERR_INJ_EXTRA_MCDDC_DP_REG, EnjExtra.Data);

  MaskHiReg.Data = mMemRas->MemReadPciCfg (node, ch, RSP_FUNC_ADDR_MASK_HI_MCDDC_DESC_REG);
  MaskHiReg.Bits.addr_mask_higher    = 00;
  mMemRas->MemWritePciCfg (node, ch, RSP_FUNC_ADDR_MASK_HI_MCDDC_DESC_REG, MaskHiReg.Data);

  MaskLoReg.Data = mMemRas->MemReadPciCfg (node, ch, RSP_FUNC_ADDR_MASK_LO_MCDDC_DESC_REG);
  MaskLoReg.Bits.addr_mask_lower    = 07;
  mMemRas->MemWritePciCfg (node, ch, RSP_FUNC_ADDR_MASK_LO_MCDDC_DESC_REG, MaskLoReg.Data);

  // Addr Match Lower: 32-bits (Match Addr[34:3])
  MatchLoReg.Data = mMemRas->MemReadPciCfg (node, ch, RSP_FUNC_ADDR_MATCH_LO_MCDDC_DESC_REG);
  MatchLoReg.Bits.addr_match_lower = (UINT32)(mPhyAddr >> 03);
  mMemRas->MemWritePciCfg (node, ch, RSP_FUNC_ADDR_MATCH_LO_MCDDC_DESC_REG, MatchLoReg.Data);

  // RSP_FUNC_ADDR_MATCH_EN RSP_FUNC_ADDR_MATCH_EN 11:11 Enabling the Address Match Response Function when set.
  // The enable bit is self cleared after match and the lock is driven from the AND output of EPMCMAIN_DFX_LCK_CNTL.RSPLCK (uCR) AND
  // MC_ERR_INJ_LCK.MC_ERR_INJ_LCK (MSR) registers.
  // Bits 10:0 Match Addr[45:35]
  MatchHiReg.Data = mMemRas->MemReadPciCfg (node, ch, RSP_FUNC_ADDR_MATCH_HI_MCDDC_DESC_REG);
  MatchHiReg.Bits.addr_match_higher  = mPhyAddr >> 0x23;
  MatchHiReg.Bits.rsp_func_addr_match_en  = 01;
  mMemRas->MemWritePciCfg (node, ch, RSP_FUNC_ADDR_MATCH_HI_MCDDC_DESC_REG, MatchHiReg.Data );
  DEBUG ((DEBUG_ERROR, " InjectMemoryError Before mem write - RSP_FUNC_ADDR_MATCH_HI.rsp_func_addr_match_en = %x, \n", MatchHiReg.Bits.rsp_func_addr_match_en ));

  if( mErrorExpected == WHEA_REC_PATROLSCRUB_ERROR ) {
      DevOffset = (node & 01) * 3;
      SCRUBCTL_BDF_Offset = SCRUBCTL_BDF_Offset + (DevOffset << 15);
      mEinjAction->Trigger0.Operation                  = WHEA_EINJ_TRIGGER_ERROR_ACTION;
      mEinjAction->Trigger0.Instruction                = INSTRUCTION_WRITE_REGISTER_VALUE;
      mEinjAction->Trigger0.Flags                      = FLAG_NOTHING;
      mEinjAction->Trigger0.Reserved8                  = 00;
      mEinjAction->Trigger0.Register.AddressSpaceId    = 00;
      mEinjAction->Trigger0.Register.RegisterBitWidth  = 0x20;
      mEinjAction->Trigger0.Register.RegisterBitOffset = 0x00;
      mEinjAction->Trigger0.Register.AccessSize        = 0x03;
      mEinjAction->Trigger0.Register.Address           = ( (UINT32)SocketNodeInfo.mmCfgBase + ((SocketNodeInfo.busUncore[NODE_TO_SKT(node)]) << 20) + (SCRUBCTL_BDF_Offset & 0x000FFFFF));
      mEinjAction->Trigger0.Value                      = imcSCRUBCTL.Data;
      mEinjAction->Trigger0.Mask                       = 0xffffffff;

    }
  if ( (mErrorExpected == WHEA_MEM_COR_ERROR) || (ErrorToInject == INJECT_ERROR_MEMORY_UE_FATAL) )  {
      // This is for WHQL testing
      mEinjAction->Trigger0.Operation                  = WHEA_EINJ_TRIGGER_ERROR_ACTION;
      mEinjAction->Trigger0.Instruction                = INSTRUCTION_WRITE_REGISTER_VALUE;
      mEinjAction->Trigger0.Flags                      = FLAG_PRESERVE_REGISTER;
      mEinjAction->Trigger0.Reserved8                  = 00;
      mEinjAction->Trigger0.Register.AddressSpaceId    = 00;
      mEinjAction->Trigger0.Register.RegisterBitWidth  = 0x40;
      mEinjAction->Trigger0.Register.RegisterBitOffset = 0x00;
      mEinjAction->Trigger0.Register.AccessSize        = 0x03;
      mEinjAction->Trigger0.Register.Address           = mPhyAddr;
      mEinjAction->Trigger0.Value                      = mInjAddrValue;
      mEinjAction->Trigger0.Mask                       = 0xffffffff;
  }
  if(mErrorExpected == WHEA_MEM_COR_ERROR) {
      // Renable patrol scrubber after corrected error injection
      DevOffset = (node & 01) * 3;
      SCRUBCTL_BDF_Offset = SCRUBCTL_BDF_Offset + (DevOffset << 15);
      mEinjAction->Trigger1.Operation                  = WHEA_EINJ_TRIGGER_ERROR_ACTION;
      mEinjAction->Trigger1.Instruction                = INSTRUCTION_WRITE_REGISTER_VALUE;
      mEinjAction->Trigger1.Flags                      = FLAG_NOTHING;
      mEinjAction->Trigger1.Reserved8                  = 00;
      mEinjAction->Trigger1.Register.AddressSpaceId    = 00;
      mEinjAction->Trigger1.Register.RegisterBitWidth  = 0x20;
      mEinjAction->Trigger1.Register.RegisterBitOffset = 0x00;
      mEinjAction->Trigger1.Register.AccessSize        = 0x03;
      mEinjAction->Trigger1.Register.Address           = ( (UINT32)SocketNodeInfo.mmCfgBase + ((SocketNodeInfo.busUncore[NODE_TO_SKT(node)]) << 20) + (SCRUBCTL_BDF_Offset & 0x000FFFFF));
      mEinjAction->Trigger1.Value                      = restoreImcScrubctl.Data;
      mEinjAction->Trigger1.Mask                       = 0xffffffff;
  }
  AsmWbinvd();

  if(SocketId == 00) {
    // Write data to memory to seed error in memeory
    *( UINT32 *)(UINTN)(mPhyAddr) = mInjAddrValue;
    AsmWbinvd();
  } else {
    InitializeSpinLock(&mApLock);
    FoundThread = FALSE;
    for(thread = 0; thread < mSmst->NumberOfCpus; thread++) {
      if(mSmmCpuServiceProtocol->GetProcessorInfo(mSmmCpuServiceProtocol,thread, &ProcInfo) == EFI_SUCCESS) {
        if((ProcInfo.Location.Package == SocketId) && (ProcInfo.StatusFlag & PROCESSOR_ENABLED_BIT) != 0) {
        FoundThread = TRUE;
        break;
        }
      }
    }
    if(FoundThread) {
    AcquireSpinLock(&mApLock);
    Status = mSmst->SmmStartupThisAp(SeeErrorAp, thread, &SocketId);
    if(Status == EFI_SUCCESS) {
      while ( (AcquireSpinLockOrFail(&mApLock)) != TRUE  ) {
        CpuPause();
      }
     }
    ReleaseSpinLock(&mApLock);
   }
  }


/*
  // TBD - uncomment the restoring of vmseweccerr bits that are set.
  // Restore the vmseweccerr bits.
  for (node = 0; node < MAX_MEM_NODE; node++) {
    if ( SocketNodeInfo.imc[node].enabled == 0 ) continue;

    for (ch = 0; ch < MAX_CH; ch=ch+2) {
      if (SocketNodeInfo.imc[node].channelList[ch].chEnabled == 0) continue;
        mMemRas->WriteJckCfg ( node, (ch & 0xfe), DDRGLOBAL, EMASK_JCK_REG, emaskSaved[node][ch].Data);
    }
  }
*/

  MatchHiReg.Data = mMemRas->MemReadPciCfg (node, ch, RSP_FUNC_ADDR_MATCH_HI_MCDDC_DESC_REG);
  DEBUG ((DEBUG_ERROR, " InjectMemoryError After mem write - RSP_FUNC_ADDR_MATCH_HI.rsp_func_addr_match_en = %x, \n", MatchHiReg.Bits.rsp_func_addr_match_en ));
// AptioV server override start - IDK tool on-die error injection failure after Memory CE injected using EINJ method
  if(gUnlockMsr==1) {
   // Skip locking of MSR 0x790 if UnlockMsr setup option is enabled
   return;   
  } else {
  // Lock the error injection mechanism
  for (thread = 0; thread < mSmst->NumberOfCpus; thread++) {
   mSmst->SmmStartupThisAp (LockInjLogic,
                 thread,
                 &node);
  }
  LockInjLogic(&node);
  }
// AptioV server override end - IDK tool on-die error injection failure after Memory CE injected using EINJ method
}

VOID
InjectLLCError(
)
/*++

Routine Description: This function injects LLC error in the cache

Arguments:

    None

Returns:

    none

--*/
{

  UINT8   CboId;
  UINT8   SocketId = 00;
  UINT8   thread = 0;
  UINT32  Data32;
  UINT64  PhyAddr;
  UINT32 CboMap=0;
  UINT8 CboCount=0;

  SADDBGMM0_CBO_CFG_STRUCT      SadDebugMm0;
  SADDBGMM1_CBO_CFG_STRUCT      SadDebugMm1;
  SADDBGMM2_CBO_CFG_HSX_BDX_STRUCT  SadDebugMm2;
  LLCERRINJ_CBO_CFG_STRUCT      LLCErrInj;

    // Unlock the error injection logic
    for (thread = 0; thread < mSmst->NumberOfCpus; thread++) {
      mSmst->SmmStartupThisAp (UnLockInjLogic,
                               thread,
                               &SocketId);
    }
  UnLockInjLogic(&SocketId);

  PhyAddr = mEinjParam->EinjAddr;

  // Unlock the error injection logic
    for (thread = 0; thread < mSmst->NumberOfCpus; thread++) {
      mSmst->SmmStartupThisAp (DisablePsmiAndPrefetch,
                               thread,
                               &SocketId);
    }

    // Disable PSMI and Disable Prefetch
    DisablePsmiAndPrefetch(&SocketId);

    // Bring the cache line to M state
    Data32 = *(volatile  UINT32 *)(UINTN)(PhyAddr);
    *(volatile  UINT32 *)(UINTN)(PhyAddr) = 0x12345678;
    AsmFlushCacheLine ((VOID*)PhyAddr);
    AsmMfence();
    //Commenting out quiesce not supported in EP Parts
    // Turn on queisce
    //TurnOnQuiesce();
    Data32 = *(volatile  UINT32 *)(UINTN)(PhyAddr);
    *(volatile  UINT32 *)(UINTN)(PhyAddr) = 0xdeadbeef;
    // Turn off queisce
    //TurnOffQuiesce();
  GetCboCountAndListWhea(SocketId,&CboCount,&CboMap);
  for (CboId = 0; CboId < CboCount; CboId++) {
    DEBUG((DEBUG_INFO,"CboCount: %d,CboId: %d socketId: %d\n",CboCount,CboId,SocketId));
    // Set ErrInjMode bit to inject once only and set inj3bdataerr bit for triple bit data error in LLC when Address match happens
      LLCErrInj.Data = mMemRas->MemReadPciCfg( SocketId, CboId, LLCERRINJ_CBO_CFG_REG );
      LLCErrInj.Bits.errinjmode = 01;
      LLCErrInj.Bits.inj3bdataerr = 01;
      DEBUG((DEBUG_INFO,"LLC Debug data %x\n",LLCErrInj.Data));
      mMemRas->MemWritePciCfg( SocketId, CboId, LLCERRINJ_CBO_CFG_REG, LLCErrInj.Data);

      // Set address Match and Mask bits
      // SADDBGMM0[31:0]   - mask field for address[37:6]
      SadDebugMm0.Data = mMemRas->MemReadPciCfg( SocketId, CboId, SADDBGMM0_CBO_CFG_REG );
      SadDebugMm0.Bits.mask_37_6 = 00;
      mMemRas->MemWritePciCfg( SocketId, CboId, SADDBGMM0_CBO_CFG_REG, SadDebugMm0.Data);

      SadDebugMm1.Data = mMemRas->MemReadPciCfg( SocketId, CboId, SADDBGMM1_CBO_CFG_REG );
      SadDebugMm1.Bits.mask_45_38 = 00;
      SadDebugMm1.Bits.match_29_6 = (UINT32)(PhyAddr >> 06 );
      mMemRas->MemWritePciCfg( SocketId, CboId, SADDBGMM1_CBO_CFG_REG, SadDebugMm1.Data);

      // SADDBDMM2[15:0] - match field for address[45:30]
      SadDebugMm2.Data = mMemRas->MemReadPciCfg( SocketId, CboId, SADDBGMM2_CBO_CFG_REG );
      SadDebugMm2.Bits.match_45_30 = (UINT32)(PhyAddr >> 30 );
      SadDebugMm2.Bits.errinjaddrvalid = 1;
      SadDebugMm2.Bits.errinjopcodevalid = 1;
      mMemRas->MemWritePciCfg( SocketId, CboId, SADDBGMM2_CBO_CFG_REG, SadDebugMm2.Data);

  }
  DEBUG((DEBUG_INFO,"Err inj bits set \n"));
  mEinjAction->Trigger0.Operation                  = WHEA_EINJ_TRIGGER_ERROR_ACTION;
  mEinjAction->Trigger0.Instruction                = 05;  // CLFLUSH
  mEinjAction->Trigger0.Flags                      = FLAG_NOTHING;
  mEinjAction->Trigger0.Reserved8                  = 00;
  mEinjAction->Trigger0.Register.AddressSpaceId    = 00;
  mEinjAction->Trigger0.Register.RegisterBitWidth  = 0x40;
  mEinjAction->Trigger0.Register.RegisterBitOffset = 0x00;
  mEinjAction->Trigger0.Register.AccessSize        = 0x03;
  mEinjAction->Trigger0.Register.Address           = PhyAddr;
  mEinjAction->Trigger0.Value                      = 0x12345678;
  mEinjAction->Trigger0.Mask                       = 0xffffffff;
  DEBUG((DEBUG_INFO,"Physical addr: %x \n",mEinjAction->Trigger0.Value));
  DEBUG((DEBUG_INFO,"Physical addr Given: %x \n",PhyAddr));

}


QPI_STATUS
GetCboCountAndListWhea (
  UINT8            SocId,
  UINT8            *CboCount,
  UINT32           *CboMap
  )
{
  UINT8   Ctr, MaxCbo;
  UINT8   Count = 0;
  UINT32  LlcMap = 0;
  CAPID5_PCU_FUN3_BDX_STRUCT    Capid5;

  //
  // Make sure we are not accessing more than what is supported for the given CPU
  //
  if (mIioUds->SystemStatus.cpuType == CPU_BDX) {
    MaxCbo = MAX_BDX_CBO;
  } else {
    MaxCbo = MAX_HSX_CBO;
  }

  Capid5.Data = mMemRas->MemReadPciCfg (SocId, 0, CAPID5_PCU_FUN3_REG);
  LlcMap = Capid5.Bits.llc_slice_en;
  for (Ctr = 0; Ctr < MaxCbo; ++Ctr) {
    if (LlcMap & (1 << Ctr)) {
      Count++;
    }
  }

  *CboCount = Count;
  *CboMap   = LlcMap;

  return QPI_SUCCESS;
}

EFI_STATUS
SetBiosRsvdInjAddress(

)
{

  UINT64      BiosRsvdStartAddr;
  UINT64      BiosRsvdEndAddr;

  BiosRsvdStartAddr  = (UINT64)(UINTN)aMemoryLlcErrInjAddr + 0x100;
  BiosRsvdStartAddr  &= ~(0xff);
  BiosRsvdEndAddr    = (UINT64)(UINTN)aMemoryLlcErrInjAddr + WHEA_EINJ_ADDR_RANGE - 0x400;  // Safe side reduced 200h bytes from the limit.
  mBiosRsvdInjAddr += 0x400;
  if(mBiosRsvdInjAddr >= BiosRsvdEndAddr)  {
      return EFI_OUT_OF_RESOURCES;
  }
  mEinjParam->EinjAddr = mBiosRsvdInjAddr;
  mEinjParam->AddrMask = 0xfffffffffffff000;

  return EFI_SUCCESS;
}

void
IntDownGrade(
  UINT8 *node
)
{
  UINT64      Data64;

  // bit 2 of MSR 0x17f
  Data64 = AsmReadMsr64 (MC_PS_INT_DOWNGRADE);
  Data64 |= V_PS_INT_DOWNGRADE;
  AsmWriteMsr64 (MC_PS_INT_DOWNGRADE, Data64);

}

STATIC
VOID
DownGradePatrolInterrupt (

  )
{
  UINT8       Node =00, thread = 00;

    // Scope is package level. No need to do on each thread.
    for (thread = 0; thread < mSmst->NumberOfCpus; thread++) {
      mSmst->SmmStartupThisAp (IntDownGrade,
                               thread,
                               &Node);
    }
    IntDownGrade(&Node); // AptioV Server Override: Added the missing function call.
}

STATIC
EFI_STATUS
WheaHandler_IntDownGrade (
  IN  EFI_HANDLE                          DispatchHandle,
  IN  CONST EFI_SMM_SW_REGISTER_CONTEXT   *DispatchContext, OPTIONAL
  IN OUT VOID                             *CommBuffer,     OPTIONAL
  IN OUT UINTN                            *CommBufferSize  OPTIONAL
  )
{
  DownGradePatrolInterrupt();

  return EFI_SUCCESS;
}

EFI_STATUS
InjectPciExError (
 IN  UINT32      PcieSBDF,
 IN  UINT32   ErrorToInject
 )
/*
ValidateRootPortForInjection:

    This Function valiadtes the input passed for pci-ex error inejction.

    i/p : PcieSBDF -    BYTE 3 - PCIe Segment
                        BYTE 2 - Bus Number
                        BYTE 1 - Device Number[BITs 7:3], Function Number BITs[2:0]
                        BYTE 0 - RESERVED
        ErrorToInject - INJECT_ERROR_PCIE_UE_NON_FATAL or

    o/p : EFI_SUCCESS - in injetcion sucess
          EFI_UNSUPPORTED - If injection capabilitied not found.
*/
{

  UINT8   Bus;
  UINT8   Dev;
  UINT8   Func;
  UINT16                        ErrInjCapId = 0xB;
  UINT32                        PciInjdevAddr = 00;
  UINT32                        PciInjdevAddr1 = 00;
  ERRINJCAP_IIO_PCIE_STRUCT     ErrInjCap;
  ERRINJCON_IIO_PCIE_STRUCT     ErrInjCon;
  UNCERRSEV_IIO_PCIE_BDX_STRUCT UncErrSev;
  EFI_STATUS                    Status;

    Bus = (PcieSBDF >> 16) & (0xFF);
    Dev = (PcieSBDF >> 11) & (0x1F);
    Func= (PcieSBDF >> 8) & (0x7);

    PciInjdevAddr = (UINT32)mIioUds->PlatformData.PciExpressBase | PCI_PCIE_ADDR(Bus, Dev, Func, ERRINJCAP_IIO_PCIE_REG);   // ERRINJCAP 
    ErrInjCap.Data = *((volatile UINT32 *) ((UINTN) PciInjdevAddr));
    if ( (ErrInjCap.Bits.extcapid  & 0xffff) & ErrInjCapId) {   //  Verify the capability of the injection device
        PciInjdevAddr =  (UINT32)mIioUds->PlatformData.PciExpressBase | PCI_PCIE_ADDR(Bus, Dev, Func, ERRINJCON_IIO_PCIE_REG);   // ERRINJCAP 
        ErrInjCon.Data = *((volatile UINT16 *) ((UINTN) PciInjdevAddr));

    Status = IioErrInjUnLock();
    if (Status) {
      DEBUG((DEBUG_ERROR,"Failed to unlock Error injection registers \n"));
      return EFI_UNSUPPORTED;
    }
    ErrInjCon.Data = *((volatile UINT16 *) ((UINTN) PciInjdevAddr));
    ErrInjCon.Bits.errinjdis  = 0;
    ErrInjCon.Bits.cause_rcverr = 0;
    ErrInjCon.Bits.cause_ctoerr = 0;
      *((volatile UINT16 *) ((UINTN) PciInjdevAddr)) = ErrInjCon.Data;
    ErrInjCon.Data = *((volatile UINT16 *) ((UINTN) PciInjdevAddr));
      PciInjdevAddr1 = (UINT32)mIioUds->PlatformData.PciExpressBase | PCI_PCIE_ADDR(Bus, Dev, Func, UNCERRSEV_IIO_PCIE_REG);    // UNCERRSEV_IIO_PCIE_REG 
    UncErrSev.Data = *((volatile UINT32 *) ((UINTN) PciInjdevAddr1));

    if (ErrorToInject == INJECT_ERROR_PCIE_UE_FATAL ) {
        UncErrSev.Bits.completion_time_out_severity  = 1;
        ErrInjCon.Bits.errinjdis    = 0;
        ErrInjCon.Bits.cause_rcverr = 0;
        ErrInjCon.Bits.cause_ctoerr = 1;


    } else if (ErrorToInject == INJECT_ERROR_PCIE_UE_NON_FATAL ) {
        UncErrSev.Bits.completion_time_out_severity  = 0;
        ErrInjCon.Bits.errinjdis    = 0;
        ErrInjCon.Bits.cause_rcverr = 0;
        ErrInjCon.Bits.cause_ctoerr = 1;


    } else if (ErrorToInject == INJECT_ERROR_PCIE_CE ){
        ErrInjCon.Bits.errinjdis    = 0;
        ErrInjCon.Bits.cause_rcverr = 1;
        ErrInjCon.Bits.cause_ctoerr = 0;


    } else {
        ErrInjCon.Bits.cause_rcverr = 00;
        ErrInjCon.Bits.cause_ctoerr = 0;
    }

    *((volatile UINT32 *) ((UINTN) PciInjdevAddr1))  = UncErrSev.Data;

    if(mPcieErrInjActionTable == TRUE) {
      mEinjAction->Trigger0.Operation                  = WHEA_EINJ_TRIGGER_ERROR_ACTION;
      mEinjAction->Trigger0.Instruction                = INSTRUCTION_WRITE_REGISTER_VALUE;
      mEinjAction->Trigger0.Flags                      = FLAG_NOTHING;
      mEinjAction->Trigger0.Reserved8                  = 00;
      mEinjAction->Trigger0.Register.AddressSpaceId    = 00;
      mEinjAction->Trigger0.Register.RegisterBitWidth  = 0x20;
      mEinjAction->Trigger0.Register.RegisterBitOffset = 0x00;
      mEinjAction->Trigger0.Register.AccessSize        = 0x03;
      mEinjAction->Trigger0.Register.Address           = PciInjdevAddr;
      mEinjAction->Trigger0.Value                      = ErrInjCon.Data;
      mEinjAction->Trigger0.Mask                       = 0xffffffff;

     //Trigger action 1 is added to lock iio_err_inj lck after pcie error injection
      mEinjAction->Trigger1.Operation                   = WHEA_EINJ_TRIGGER_ERROR_ACTION;
      mEinjAction->Trigger1.Instruction                = INSTRUCTION_WRITE_REGISTER_VALUE;
      mEinjAction->Trigger1.Flags                      = FLAG_NOTHING;
      mEinjAction->Trigger1.Reserved8                  = 00;
      mEinjAction->Trigger1.Register.AddressSpaceId    = 01;
      mEinjAction->Trigger1.Register.RegisterBitWidth  = 0x08;
      mEinjAction->Trigger1.Register.RegisterBitOffset = 0x00;
      mEinjAction->Trigger1.Register.AccessSize        = 0x01;
      mEinjAction->Trigger1.Register.Address           = 0xB2;
      mEinjAction->Trigger1.Value                      = EFI_WHEA_EINJ_IIO_ERR_INJ_LCK_SWSMI;
      mEinjAction->Trigger1.Mask                       = 0xffffffff;
    } else {
    *((volatile UINT16 *) ((UINTN) PciInjdevAddr)) = ErrInjCon.Data;
    Status = IioErrInjLock();
    if (Status) {
      DEBUG((DEBUG_ERROR,"Failed to lock Error injection registers \n"));
      return EFI_UNSUPPORTED;
    }

    }


        return EFI_SUCCESS;

    }
    return EFI_UNSUPPORTED;
}

BOOLEAN
ValidateRootPortForInjection (
 IN    UINT32    PcieSBDF
 )
/*
ValidateRootPortForInjection:

    This Function valiadtes the input passed for pci-ex error inejction.

    i/p : PcieSBDF -    BYTE 3 - PCIe Segment
                        BYTE 2 - Bus Number
                        BYTE 1 - Device Number[BITs 7:3], Function Number BITs[2:0]
                        BYTE 0 - RESERVED
    o/p : TRUE - If it is valid PCI-ex Port for Injection
          FALSE - Not a valid port for injection

*/
{
  UINT8   Port;
  UINT8   RootBridgeLoop;
  UINT8   Bus;
  UINT8   Dev;
  UINT8   Func;
  UINT8   PxpCap_Offset = 0x90;
  UINT8   LnkCon_Offset = 0xA0;
  UINT32  PciAddr = 0;
  UINT32  Data = 0;
  UINT32  Data1 = 0;

    Bus = (PcieSBDF >> 16) & (0xFF);
    Dev = (PcieSBDF >> 11) & (0x1F);
    Func = (PcieSBDF >> 8) & (0x7);

    // Verfiy the  segment number
    if ( (PcieSBDF >> 24) != 00) return FALSE;
    // DMI port skip it
    if ( (Bus == 0) && (Dev == 00) && (Func == 00) )  return FALSE;

    for ( RootBridgeLoop =0; RootBridgeLoop < MaxIIO; RootBridgeLoop++) {
        if(mIioUds->PlatformData.IIO_resource[RootBridgeLoop].Valid != 1) continue;

        for(Port = 0; Port < NUMBER_PORTS_PER_SOCKET; Port++) {
            if( (mIioUds->PlatformData.IIO_resource[RootBridgeLoop].BusBase == Bus) &&
                (mIioUds->PlatformData.IIO_resource[RootBridgeLoop].PcieInfo.PortInfo[Port].Device == Dev) && 
                (mIioUds->PlatformData.IIO_resource[RootBridgeLoop].PcieInfo.PortInfo[Port].Function == Func) ) {
        // Check if device is present or not
            PciAddr =  (UINT32)mIioUds->PlatformData.PciExpressBase | PCI_PCIE_ADDR(Bus, Dev, Func, 00);   // VID 
            Data = *((volatile UINT32 *) ((UINTN) PciAddr));
        if ((Data & 0xFFFF) != 0x8086)  return FALSE;
        // Check if SLot connected to a card
            PciAddr =  (UINT32)mIioUds->PlatformData.PciExpressBase | PCI_PCIE_ADDR(Bus, Dev, Func, PxpCap_Offset);   // PXP capability
            Data = *((volatile UINT32 *) ((UINTN) PciAddr));
            PciAddr =  (UINT32)mIioUds->PlatformData.PciExpressBase | PCI_PCIE_ADDR(Bus, Dev, Func, LnkCon_Offset);   // PXP capability
        Data1 = *((volatile UINT32 *) ((UINTN) PciAddr));
        if ((Data & 0xFF) == 0x10) {// Indicates PXP capability.
          // offset 0x92 of the root port bit 8 gives slot implemented or not (== BIT 24 from offset 0x90)
          // offset 0xa2 of the root port bit 13 gives link active or not (== BIT 29 from offset 0xA0)
          if( (Data & 0x1000000) && (Data1 & 0x20000000) )  {
                      return TRUE;
          } else   {
            return FALSE;  // Slot not implemented or not connected.
          }
              } else   {
          return FALSE;
        }
          }
      }
    }

  return FALSE;
}

UINT32
GetRootPortForInjection (
 VOID
 )
/*
GetRootPortForInjection:

    This function gets the first available port that is connected with slot . This function is for SV usage.

    i/p : none
    o/p : PcieSBDF -    BYTE 3 - PCIe Segment
                        BYTE 2 - Bus Number
                        BYTE 1 - Device Number[BITs 7:3], Function Number BITs[2:0]
                        BYTE 0 - RESERVED
         = 00 if no slot is populated

*/
{

    UINT8     Bus;
    UINT8     Dev;
    UINT8     Func;
    UINT8     Port;
    UINT8   RootBridgeLoop;
  UINT32    PcieSBDF = 0;
  BOOLEAN Found = FALSE;

    for ( RootBridgeLoop =0; RootBridgeLoop < MaxIIO; RootBridgeLoop++) {
        if(mIioUds->PlatformData.IIO_resource[RootBridgeLoop].Valid != 1) continue;

    Bus = mIioUds->PlatformData.IIO_resource[RootBridgeLoop].BusBase;

        for (Port = 0; Port < NUMBER_PORTS_PER_SOCKET; Port++) {
        //if( mIioUds->SetupInfo.Config.PEXPHIDE[(RootBridgeLoop * NUMBER_PORTS_PER_SOCKET) + Port] == 1) continue;

        Dev = mIioUds->PlatformData.IIO_resource[RootBridgeLoop].PcieInfo.PortInfo[Port].Device;
      Func  = mIioUds->PlatformData.IIO_resource[RootBridgeLoop].PcieInfo.PortInfo[Port].Function;

      // Ignore the DMI port
      if((Bus ==0 ) && (Dev == 0) && (Func == 0)) continue;

      PcieSBDF  = (Bus << 16) | (Dev << 11) | (Func << 0x08);
      // Check if the root port connected to the  slot is populated
      Found = ValidateRootPortForInjection(PcieSBDF);
      if(Found == TRUE)  return PcieSBDF;

    }
  }

  return 00;
}

STATIC
EFI_STATUS
IioErrInjLckHandler (
  IN  EFI_HANDLE                          DispatchHandle,
  IN  CONST EFI_SMM_SW_REGISTER_CONTEXT   *DispatchContext, OPTIONAL
  IN OUT VOID                             *CommBuffer,     OPTIONAL
  IN OUT UINTN                            *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS Status;

  Status = IioErrInjLock();
  if (Status) {
      DEBUG((DEBUG_ERROR,"Failed to lock Error injection registers \n"));
      return EFI_UNSUPPORTED;
    }

  return Status;
}

EFI_STATUS
IioErrInjLock(
VOID
)
{
  UINT8  Socket;
  UINT8  thread =0, node = 0;
  UINT32  MailBoxCommand, MailboxData;
  EFI_STATUS  Status;


  Status = mSmst->SmmLocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess);
  if (EFI_ERROR(Status))return Status;

  for (thread = 0; thread < mSmst->NumberOfCpus; thread++) {
      mSmst->SmmStartupThisAp (LockInjLogic,
                                     thread,
                                      &node);
  }
  LockInjLogic(&node);

  // workaround s4987051-Clone from HSX Sighting:WHEA injection on PCIE blocked due to iio_err_inj_lck
  // being set after cpl3
  for ( Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (mIioUds->PlatformData.IIO_resource[Socket].Valid != 1)continue;
        MailBoxCommand = (UINT32)MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG;
        MailboxData = mCpuCsrAccess->Bios2PcodeMailBoxWrite(Socket, MailBoxCommand, 0);
         if ((MailboxData & 0x000000ff) != 0x01) {
            MailboxData = mCpuCsrAccess->ReadCpuCsr(Socket, 0, BIOS_MAILBOX_DATA_PCU_FUN1_REG);
            MailboxData &= ~BIT21;
         }else return EFI_UNSUPPORTED;
         MailBoxCommand = (UINT32)MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG;
         MailboxData = (UINT32)MailboxData;
         MailboxData = mCpuCsrAccess->Bios2PcodeMailBoxWrite(Socket, MailBoxCommand, MailboxData);
   }
  MicroSecondDelay(1000);
  return EFI_SUCCESS;
}

EFI_STATUS
IioErrInjUnLock(
VOID
)
{
  UINT8  Socket;
  UINT8  thread =0, node = 0;
  UINT32  MailBoxCommand, MailboxData;
  EFI_STATUS  Status;

  Status = mSmst->SmmLocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess);
  if (EFI_ERROR(Status))return Status;

  // workaround s4987051-Clone from HSX Sighting:WHEA injection on PCIE blocked due to iio_err_inj_lck
  // being set after cpl3
  for ( Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (mIioUds->PlatformData.IIO_resource[Socket].Valid != 1)continue;
        MailBoxCommand = (UINT32)MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG;
        MailboxData = mCpuCsrAccess->Bios2PcodeMailBoxWrite(Socket, MailBoxCommand, 0);
         if ((MailboxData & 0x000000ff) != 0x01) {
            MailboxData = mCpuCsrAccess->ReadCpuCsr(Socket, 0, BIOS_MAILBOX_DATA_PCU_FUN1_REG);
            MailboxData |= BIT21;
         }else return EFI_UNSUPPORTED;
         MailBoxCommand = (UINT32)MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG;
         MailboxData = (UINT32)MailboxData;
         MailboxData = mCpuCsrAccess->Bios2PcodeMailBoxWrite(Socket, MailBoxCommand, MailboxData);
  }
  MicroSecondDelay(1000);

    for (thread = 0; thread < mSmst->NumberOfCpus; thread++) {
      mSmst->SmmStartupThisAp (UnLockInjLogic,
                                     thread,
                                      &node);
     }
  UnLockInjLogic(&node);

  return EFI_SUCCESS;
}


STATIC
EFI_STATUS
WheaEinjHandler (
  IN  EFI_HANDLE                          DispatchHandle,
  IN  CONST EFI_SMM_SW_REGISTER_CONTEXT   *DispatchContext, OPTIONAL
  IN OUT VOID                             *CommBuffer,     OPTIONAL
  IN OUT UINTN                            *CommBufferSize  OPTIONAL
  )
/*

WheaEinjHandler:

SW SMI Handler for Error Injection.
This handler supports patorl scrub injection, EWB error injection and correctable error injections.

*/
{
  UINT8           node=0,thread=0;
  UINT32          PcieSBDF = 00;
  UINT32          ErrorToInject;
  BOOLEAN         PoisonEnable = FALSE;
  BOOLEAN         Einj5ValidErr = FALSE;
  BOOLEAN         PortValidatedForInjection = FALSE;
  EFI_STATUS      Status;

  mEinjParam->OpStatus = 0;
  mEinjParam->CmdStatus = 0;

  if (mWheaPlatformSupport == NULL) {
    mEinjParam->CmdStatus = 1; // unknown failure
    return EFI_INTERRUPT_PENDING;
  }

    // This code is needed for sv debuggign purpose.
  /*if(mEinjParam->SetAddressTable.MemAddress == 0) {
    mEinjParam->SetAddressTable.MemAddress  = PcieSBDF + 0x100000000;
    mEinjParam->SetAddressTable.MemAddressRange = 0x0f;
    mEinjParam->SetAddressTable.ErrType =  WHEA_TEMP_MEM_ERROR_TYPE;
  }
  mEinjParam->SetAddressTable.MemAddress += 0x100000; */

  mEinjParam->EinjAddr              = 00;  // Set address
  mEinjParam->AddrMask              = 00;  // Address Mask
  //
  // Check what type of error to be injected and create log accordingly.
  //
  CopyMem (mEinjAction, &mSimEinjAction, sizeof(WHEA_EINJ_TRIGGER_ACTION_TABLE));

  // From SetError type field.
  ErrorToInject = (UINT32)mEinjParam->ErrorToInject[0] & 0x7fffffff;

  mErrorExpected = WHEA_NO_CONSUME;

  if(ErrorToInject == INJECT_ERROR_MEMORY_UE_NON_FATAL) {
    PoisonEnable    = TRUE;
    mErrorExpected  = WHEA_REC_PATROLSCRUB_ERROR;
  }

  if(ErrorToInject == INJECT_ERROR_MEMORY_CE)
    mErrorExpected = WHEA_MEM_COR_ERROR;

  if(mAcpi5Support == TRUE)  {
    // Included Temp error type also for testing by SSG
    if((mEinjParam->SetAddressTable.ErrType & WHEA_TEMP_MEM_ERROR_TYPE )  ||
       ((mEinjParam->SetAddressTable.ErrType & WHEA_VENDOR_EXT_TYPE) && (mEinjParam->VendorExt.OemDefType1 == WHEA_MEM_ERROR_REC_TYPE) )) {
        ErrorToInject   = INJECT_ERROR_MEMORY_UE_NON_FATAL;
        mErrorExpected  = WHEA_CORE_IFU_DCU_ERR;
        PoisonEnable    = TRUE;
    }

    if( (mEinjParam->SetAddressTable.ErrType & WHEA_VENDOR_EXT_TYPE) && (mEinjParam->VendorExt.OemDefType1 == WHEA_PATROL_SCRUB_ERROR) ){
        ErrorToInject = INJECT_ERROR_MEMORY_UE_NON_FATAL;
        mErrorExpected  = WHEA_REC_PATROLSCRUB_ERROR;
        PoisonEnable    = TRUE;
    }

    if( (mEinjParam->SetAddressTable.ErrType & WHEA_VENDOR_EXT_TYPE) != WHEA_VENDOR_EXT_TYPE) {

      if( mEinjParam->SetAddressTable.ErrType & ( INJECT_ERROR_PCIE_CE | INJECT_ERROR_PCIE_UE_NON_FATAL | INJECT_ERROR_PCIE_UE_FATAL | INJECT_ERROR_MEMORY_CE |
                                                                                    INJECT_ERROR_PROCESSOR_UE_NON_FATAL | INJECT_ERROR_MEMORY_UE_NON_FATAL | INJECT_ERROR_MEMORY_UE_FATAL)) {
        ErrorToInject = mEinjParam->SetAddressTable.ErrType;
        if(mEinjParam->SetAddressTable.ErrType & INJECT_ERROR_MEMORY_UE_NON_FATAL ) {
          PoisonEnable    = TRUE;
          mErrorExpected  = WHEA_REC_PATROLSCRUB_ERROR;
        }
        if( mEinjParam->SetAddressTable.ErrType & INJECT_ERROR_MEMORY_CE) {
          mErrorExpected = WHEA_MEM_COR_ERROR;
        }
      }
    }

    if((mEinjParam->SetAddressTable.Flags & WHEA_SET_ERROR_TYPE_WITH_MEMORY_ADDRESS_VALID) == WHEA_SET_ERROR_TYPE_WITH_MEMORY_ADDRESS_VALID) {
    mEinjParam->EinjAddr              = mEinjParam->SetAddressTable.MemAddress;  // Set address
    mEinjParam->AddrMask              = mEinjParam->SetAddressTable.MemAddressRange;  // Address Mask
    Einj5ValidErr  = TRUE;
    }
    mEinjParam->SetAddressTable.ErrType = 00;  // Bios has to clear this field after we consumed it.
  }
/*
  if(Einj5ValidErr == FALSE && mEinjParam->ErrorToInject[3] && mEinjParam->ErrorToInject[4] ) {  //Check the old type of address is paassed in the set error type structure.
    mEinjParam->EinjAddr              = mEinjParam->ErrorToInject[3];  // Set address
    mEinjParam->AddrMask              = mEinjParam->ErrorToInject[4];  // Address Mask
  }
*/
  if((ErrorToInject != INJECT_ERROR_PCIE_UE_NON_FATAL) && (ErrorToInject != INJECT_ERROR_PCIE_CE) ) {
  if((!(mEinjParam->EinjAddr)) || (!(mEinjParam->AddrMask)) ) {

    Status = SetBiosRsvdInjAddress( );

    if (EFI_ERROR(Status)) {
      mEinjParam->CmdStatus  = WHEA_EINJ_CMD_INVALID_ACCESS;
      return EFI_INTERRUPT_PENDING;
    }
  }
  }

  switch (ErrorToInject) {

    case INJECT_ERROR_PROCESSOR_UE_NON_FATAL:           //Explicit Write Back Error

      for (thread = 0; thread < mSmst->NumberOfCpus; thread++) {
          mSmst->SmmStartupThisAp (EnablePoison,
                                   thread,
                                   &node);
        }
      EnablePoison(&node);

          InjectLLCError(INJECT_ERROR_PROCESSOR_UE_NON_FATAL);
        break;

    case INJECT_ERROR_MEMORY_CE:

          InjectMemoryError( INJECT_ERROR_MEMORY_CE);
          break;

    case INJECT_ERROR_MEMORY_UE_NON_FATAL:

      if( PoisonEnable == TRUE) {
          for (thread = 0; thread < mSmst->NumberOfCpus; thread++) {
            mSmst->SmmStartupThisAp (EnablePoison,
                                     thread,
                                     &node);
          }
        EnablePoison(&node);
      }
          InjectMemoryError(INJECT_ERROR_MEMORY_UE_NON_FATAL);
          break;

    case INJECT_ERROR_MEMORY_UE_FATAL:

      // For WHQL testing , Disabling the poison and injecting uncorrected memeory error to cause system reset with blue screen.
      for (thread = 0; thread < mSmst->NumberOfCpus; thread++) {
            mSmst->SmmStartupThisAp (DisablePoison,
                                     thread,
                                     &node);
      }
      DisablePoison(&node);

          InjectMemoryError(INJECT_ERROR_MEMORY_UE_FATAL);
          break;
    case INJECT_ERROR_PCIE_UE_FATAL :
    case INJECT_ERROR_PCIE_CE :
    case INJECT_ERROR_PCIE_UE_NON_FATAL :
      // Check Set addresstable has valid bus, dev, func numbers.

      if((mEinjParam->SetAddressTable.PcieSBDF) && ( ((mEinjParam->SetAddressTable.Flags & WHEA_SET_ERROR_TYPE_WITH_SBDF_VALID) == WHEA_SET_ERROR_TYPE_WITH_SBDF_VALID) ) ) {
        PcieSBDF  = mEinjParam->SetAddressTable.PcieSBDF;
            PortValidatedForInjection = ValidateRootPortForInjection(PcieSBDF);
      } else  {
        PcieSBDF = GetRootPortForInjection();
        if(PcieSBDF) PortValidatedForInjection =1;
        else  PortValidatedForInjection = 00;
      }

          if(PortValidatedForInjection) {
  Status = InjectPciExError ( PcieSBDF, ErrorToInject);
            if(EFI_ERROR(Status))    mEinjParam->CmdStatus  = WHEA_EINJ_CMD_INVALID_ACCESS;
        } else {
                mEinjParam->CmdStatus  = WHEA_EINJ_CMD_INVALID_ACCESS;
      }

        break;

      default:
          break;

  }
  return EFI_SUCCESS;

}

VOID
EFIAPI
WheaPlatformCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
/*++

Routine Description:

  Updates the gCpuIo variable upon the availability of CPU IO Protocol.

Arguments:

  Event    - Event type
  *Context - Context for the event

Returns:

  None

--*/
{
  EFI_STATUS                        Status;
  Status = mSmst->SmmLocateProtocol(&gEfiWheaPlatformSupportProtocolGuid, NULL, &mWheaPlatformSupport);
}

VOID
UpdatSckNodeInfoStruc (VOID)

{
  UINT8       socket;
  UINT8       node;
  UINT8       ch;
  EFI_STATUS  Status;
  EFI_IIO_UDS_PROTOCOL *IioUdsProtocol;
  //
  // Locate the IIO Protocol Interface
  //
  Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid,NULL,&IioUdsProtocol);
  ASSERT_EFI_ERROR (Status);
  mIioUds = IioUdsProtocol->IioUdsPtr;

  //
  // Initialize module globals from host structure
  //
  SocketNodeInfo.socketPresentBitMap    = mIioUds->SystemStatus.socketPresentBitMap;
  SocketNodeInfo.mmCfgBase              = mIioUds->SystemStatus.mmCfgBase;
  SocketNodeInfo.CpuStepping            = mIioUds->SystemStatus.MinimumCpuStepping;
  SocketNodeInfo.ExistingNodeNum        = 0xaa;

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    SocketNodeInfo.busUncore[socket]    = mIioUds->PlatformData.CpuQpiInfo[socket].BusNum;
    SocketNodeInfo.busIio[socket]        = mIioUds->PlatformData.IioQpiInfo[socket].BusNum;
  }

  for (node = 0; node < MAX_MEM_NODE; node++) {
    SocketNodeInfo.imc[node].enabled    = mIioUds->PlatformData.MemInfo[node].enabled;
    if (SocketNodeInfo.imc[node].enabled == 0 ) continue;
    if ( SocketNodeInfo.ExistingNodeNum == 0xaa)    SocketNodeInfo.ExistingNodeNum = node;
    SocketNodeInfo.RASmode[node]        = mIioUds->SystemStatus.RASmode[node];
    SocketNodeInfo.RASModesEx[node]     = mIioUds->SystemStatus.RASmodeEx[node];

    for (ch = 0; ch < MAX_CH; ch++) {
      SocketNodeInfo.imc[node].channelList[ch].chEnabled = mIioUds->PlatformData.MemInfo[node].channelList[ch].chEnabled;
    } // ch
  } // node

  return;
}


//UpdateEinj
// Driver entry function
//
EFI_STATUS
InitWheaErrorInj (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
/*++

Routine Description:

  Entry point of the Whea support driver.

Arguments:

  ImageHandle   EFI_HANDLE: A handle for the image that is initializing this driver
  SystemTable   EFI_SYSTEM_TABLE: A pointer to the EFI system table

Returns:

  EFI_SUCCESS:              Driver initialized successfully
  EFI_LOAD_ERROR:           Failed to Initialize or has been loaded
  EFI_OUT_OF_RESOURCES:     Could not allocate needed resources

--*/
{
  EFI_STATUS                        Status;
  EFI_HANDLE                        SwHandle;
  EFI_SMM_SW_REGISTER_CONTEXT       SwContext;
  EFI_SMM_SW_DISPATCH2_PROTOCOL     *SwDispatch  = 0;
  BOOLEAN                           InSmm;
  VOID                              *GuidHob;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK    *DescriptorBlock;
  UINTN                             TsegIndex = 0;
// AptioV server override start - IDK tool on-die error injection failure after Memory CE injected using EINJ method
  SYSTEM_CONFIGURATION	setupData;
  UINTN varSize;
  
  Status = gRT->GetVariable( L"IntelSetup", &gEfiSetupVariableGuid, NULL, &varSize, &setupData );
  if (EFI_ERROR(Status)) {
    gUnlockMsr=0;  
  } else {
  gUnlockMsr= setupData.UnlockMsr;
  }
// AptioV server override end - IDK tool on-die error injection failure after Memory CE injected using EINJ method

  //
  // Get Hob list for SMRAM desc
  //
  GuidHob = GetFirstGuidHob(&gEfiSmmPeiSmramMemoryReserveGuid);
  ASSERT(GuidHob);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  DescriptorBlock = GET_GUID_HOB_DATA(GuidHob);

  //
  // Use the hob to get SMRAM capabilities
  //
  if (DescriptorBlock->NumberOfSmmReservedRegions > 0) {
    TsegIndex = DescriptorBlock->NumberOfSmmReservedRegions - 1;
  }
  else {
    ASSERT(FALSE); // System halts if there is no SMM reserved region exist
  }
  ASSERT(TsegIndex <= (MAX_SMRAM_RANGES - 1));
  mTsegBase = DescriptorBlock->Descriptor[TsegIndex].PhysicalStart;
  mTsegSize = DescriptorBlock->Descriptor[TsegIndex].PhysicalSize;

  //
  // Retrieve SMM Base Protocol
  //
  Status = gBS->LocateProtocol (
                  &gEfiSmmBase2ProtocolGuid,
                  NULL,
                  &mSmmBase
                  );
  if (mSmmBase == NULL) {
    InSmm = FALSE;
  } else {
      mSmmBase->InSmm (mSmmBase, &InSmm);
    mSmmBase->GetSmstLocation (mSmmBase, &mSmst);
  }

  if (InSmm) {
    //
    // Init WHEA globals
    //
    UpdatSckNodeInfoStruc ();

    // Update EINJ table entries.
    Status = UpdateEinj();
    if (EFI_ERROR(Status)) {
      return Status;
    }

    //
    // Register our SMI handlers
    //
    Status = mSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, &SwDispatch);
    ASSERT_EFI_ERROR (Status);

    Status = mSmst->SmmLocateProtocol (&gEfiSmmCpuServiceProtocolGuid, NULL, &mSmmCpuServiceProtocol);
    ASSERT_EFI_ERROR (Status);

    //
    // Install SW SMI handler to build error data for simulated error injection
    //
    SwContext.SwSmiInputValue = EFI_WHEA_EINJ_EXECUTE_SWSMI;
    Status                    = SwDispatch->Register (SwDispatch, WheaEinjHandler, &SwContext, &SwHandle);
    ASSERT_EFI_ERROR (Status);

    SwContext.SwSmiInputValue = EFI_WHEA_EINJ_PATROL_SCRUB_SWSMI;
    Status                    = SwDispatch->Register (SwDispatch, WheaHandler_IntDownGrade, &SwContext, &SwHandle);
    ASSERT_EFI_ERROR (Status);

    SwContext.SwSmiInputValue = EFI_WHEA_EINJ_IIO_ERR_INJ_LCK_SWSMI;
    Status                    = SwDispatch->Register (SwDispatch, IioErrInjLckHandler, &SwContext, &SwHandle);
    ASSERT_EFI_ERROR (Status);

    //
    // Find Whea Platform protocol instance
    //
    mWheaPlatformSupport = NULL;
    Status = mSmst->SmmLocateProtocol(&gEfiWheaPlatformSupportProtocolGuid, NULL, &mWheaPlatformSupport);
    if (Status != EFI_SUCCESS) {
      //
      // Set a notification if protocol not available yet.
      //

      //
      // Register for protocol notifications on this event
      //
      Status = mSmst->SmmRegisterProtocolNotify(
                      &gEfiWheaPlatformSupportProtocolGuid,
                      (EFI_SMM_NOTIFY_FN)WheaPlatformCallback,
                      &mWheaEventReg
                      );
      ASSERT_EFI_ERROR (Status);

    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
UpdateEinj (
  VOID
  )
{

  UINT8                             *Ests;
  UINT32                                          Offset;
  UINT32                                          UncoreBDF;
  UINTN                                           varSize;
  SYSTEM_CONFIGURATION                            setupData;
  EFI_WHEA_SUPPORT_PROTOCOL         *WheaDrv;
  EFI_STATUS                        Status;
  //
  // DXE/PST initialization - create all error sources
  //
  Status = gBS->LocateProtocol (&gEfiWheaSupportProtocolGuid, NULL, &WheaDrv);
  if (EFI_ERROR(Status))return Status;

  varSize = sizeof(SYSTEM_CONFIGURATION);
  // AptioV Server Override: Changed variable name to IntelSetup
  Status = gRT->GetVariable( L"IntelSetup", &gEfiSetupVariableGuid, NULL, &varSize, &setupData );

  if (EFI_ERROR(Status))return Status;

  // Locate MemRas protocol
  Status = mSmst->SmmLocateProtocol (&gEfiMemRasProtocolGuid, NULL, &mMemRas);
  if (EFI_ERROR(Status))return Status;

  // Locate CPUCSR access protocol
  Status = gBS->LocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess);
  if (EFI_ERROR(Status))return Status;

  //
  // Allocate Error status block for Error Injection - we should change this to use
  // Error status block assigned for the error source
  //
  Status = gBS->AllocatePool (EfiReservedMemoryType, 0x900, (VOID **)&Ests);
  ASSERT_EFI_ERROR (Status);
  ZeroMem (Ests, 0x800);

  //
  // Init buffer pointers and data for Einj parameter and Error log address range.
  //
  mEinjParam    = (WHEA_EINJ_PARAM_BUFFER *)Ests;
  mEinjAction   = (WHEA_EINJ_TRIGGER_ACTION_TABLE *)(Ests + ((sizeof(WHEA_EINJ_PARAM_BUFFER)+0x3F)/0x40)*0x40);
  mEinjEsts     = ((UINT8 *)mEinjAction) + ((sizeof(WHEA_EINJ_TRIGGER_ACTION_TABLE)+0x3F)/0x40)*0x40;

  //
  // Init EINJ action table in the allocated reserved memory
  // Update Error injection Action table - Static initialization is sufficient
  //
  gBS->CopyMem(mEinjAction, &mSimEinjAction, sizeof(WHEA_EINJ_TRIGGER_ACTION_TABLE));

  // Allocate memory for 6000h bytes for Error injection structure.
  Status = gBS->AllocatePool (EfiRuntimeServicesData, WHEA_EINJ_ADDR_RANGE, &aMemoryLlcErrInjAddr);
  // Make it cache line boundary - This is the incremantal injection address Bios uses for error injection
  mBiosRsvdInjAddr  = (UINT64)(UINTN)aMemoryLlcErrInjAddr + 0x800;
  mBiosRsvdInjAddr  &= ~(0xff);
  //
  // Init EINJ parameters
  //
  mEinjParam->TriggerActionTable    = (UINT64) (UINTN)mEinjAction;
// Aptio V Server Override Start: Commented the Memory Uncorrectable Non-Fatal Error as we are not supporting the same.
#ifdef AMI_WHEA_EINJ_OVERRIDE
  mEinjParam->ErrorInjectCap        = INJECT_ERROR_MEMORY_CE | INJECT_ERROR_PROCESSOR_UE_NON_FATAL | INJECT_ERROR_MEMORY_UE_FATAL;
#else
  mEinjParam->ErrorInjectCap        = INJECT_ERROR_MEMORY_UE_NON_FATAL | INJECT_ERROR_MEMORY_CE | INJECT_ERROR_PROCESSOR_UE_NON_FATAL | INJECT_ERROR_MEMORY_UE_FATAL;
#endif  
// Aptio V Server Override End: Commented the Memory Uncorrectable Non-Fatal Error as we are not supporting the same.

  // Check Acpi 5.0 support enabled.
  if (setupData.WheaEinj_05Extn)
    mAcpi5Support  = TRUE;
// AptioV Server Override: Commented to make corrected error injection to work for now.
#ifndef AMI_WHEA_EINJ_OVERRIDE
  if (setupData.PciExpressErrInj) {
    mEinjParam->ErrorInjectCap  |= INJECT_ERROR_PCIE_CE  | INJECT_ERROR_PCIE_UE_NON_FATAL | INJECT_ERROR_PCIE_UE_FATAL ;
  }
#endif  

// AptioV Server End: Commented to make corrected error injection to work for now.
// AptioV Server Override: Commented to make corrected error injection to work for now.
  if(setupData.PcieErrInjActionTable == 1) {
    mPcieErrInjActionTable = TRUE;
  }

  if(mAcpi5Support == TRUE) {
#ifndef AMI_WHEA_EINJ_OVERRIDE
    mEinjParam->ErrorInjectCap      |= WHEA_VENDOR_EXT_TYPE | WHEA_TEMP_MEM_ERROR_TYPE;
#endif  
  }
  mEinjParam->ErrorToInject[3]      = 0;
  mEinjParam->ErrorToInject[4]      = 0;
  mEinjParam->OpState               = 0;
  mEinjParam->OpStatus              = 0;
  mEinjParam->CmdStatus             = 0;
  mEinjParam->ErrorToInject[0]      = 0;
  mEinjParam->EinjAddr              = 0;
  mEinjParam->AddrMask              = 0;


  //
  // Update Error injection serialization informations
  //
  mSimEinj[0].Register.Address = (UINT64) (UINTN)&mEinjParam->OpState;              // Update address for BEGIN_OPERATION
  mSimEinj[1].Register.Address = (UINT64) (UINTN)&mEinjParam->TriggerActionTable;   // Update address for GET_TRIGGER_TABLE
  mSimEinj[2].Register.Address = (UINT64) (UINTN)&mEinjParam->ErrorToInject[0];     // Update address for SET_ERROR
  mSimEinj[3].Register.Address = (UINT64) (UINTN)&mEinjParam->ErrorInjectCap;       // Update address for GET_ERROR_CAP
  mSimEinj[4].Register.Address = (UINT64) (UINTN)&mEinjParam->OpState;              // Update address for END_OPERATION
  mSimEinj[6].Register.Address = (UINT64) (UINTN)&mEinjParam->OpStatus;             // Update address for CHECK_BUSY_STATUS
  mSimEinj[7].Register.Address = (UINT64) (UINTN)&mEinjParam->CmdStatus;            // Update address for GET_CMD_STATUS

  if(mAcpi5Support == TRUE)  {
    mSimEinj[8].Register.Address = (UINT64) (UINTN)&mEinjParam->SetAddressTable;        // Update address for SET_ERROR_TYPE_WITH_ADDRESS

    Offset = (UINT32)((UINTN)&mEinjParam->VendorExt - (UINTN)&mEinjParam->SetAddressTable);
    mEinjParam->SetAddressTable.VendorErrTypeExtOffset = Offset;
    mEinjParam->SetAddressTable.PcieSBDF  = 00;

    UncoreBDF = (SocketNodeInfo.busUncore[00] << 20) + (0x20179000 & 0x000FFFFF);     // VID_MC_MAINEXT_REG = 0x20179000
    UncoreBDF = UncoreBDF >> 0x0C;  // BYTE 2 - PCIe Segment, BYTE 1 - Bus Number,
                  // BYTE 01 - Device Number[BITs 7:3], Function Number BITs[2:0]
    mEinjParam->VendorExt.Length        = sizeof (WHEA_EINJ_VENDOR_EXT);
    mEinjParam->VendorExt.SBDF          = UncoreBDF;
    mEinjParam->VendorExt.VendorId        = (UINT16)mMemRas->MemReadPciCfg (SocketNodeInfo.ExistingNodeNum, PARAM_UNUSED, VID_MC_MAINEXT_REG);
    mEinjParam->VendorExt.DeviceId      = (UINT16)mMemRas->MemReadPciCfg (SocketNodeInfo.ExistingNodeNum, PARAM_UNUSED, DID_MC_MAINEXT_REG);
    mEinjParam->VendorExt.RevId            = (UINT8)mMemRas->MemReadPciCfg ( SocketNodeInfo.ExistingNodeNum, PARAM_UNUSED, RID_MC_MAINEXT_REG);
  }

  //
  // Register core Whea Error injection method
  //
  Status = WheaDrv->InstallErrorInjectionMethod (
             WheaDrv,
             // AptioV Server Override Start: Added support for Memory Uncorrectable Fatal Error and removed Memory Uncorrectable Non-Fatal Error
             //INJECT_ERROR_PROCESSOR_UE_NON_FATAL | INJECT_ERROR_MEMORY_UE_NON_FATAL | INJECT_ERROR_MEMORY_CE,
             INJECT_ERROR_PROCESSOR_UE_NON_FATAL | INJECT_ERROR_MEMORY_CE | INJECT_ERROR_MEMORY_UE_FATAL,
             // AptioV Server Override End: Added support for Memory Uncorrectable Fatal Error and removed Memory Uncorrectable Non-Fatal Error
             00,         // WheaSupport.c file updates the entries.
             &mSimEinj[0]
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
