/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Copyright (c) 2011 - 2017 Intel Corporation.  All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license.  Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:
    MemRasRoutines.c

Abstract:
    Mem Ras functions and platform data

-----------------------------------------------------------------------------*/

#include "MemRas.h"

extern struct sysHost mHost;
SYSHOST                                   mHost;
typedef struct {
  UINT8 DdrCh;
  UINT8 Dimm;
  UINT8 Rank;
}LAST_ERR_INFO;

#define   PAM_DRAM_DISABLED   0x0
#define   PAM_DRAM_READ_ONLY  0x1
#define   PAM_DRAM_WRITE_ONLY 0x2
#define   PAM_DRAM_RW         0x3

//
// Macro to calculate SAD register offsets
//
#define DRAM_RULE(x) ( DRAM_RULE_CBOBC_MEMSAD_REG + ((x) * 2 * 4))
#define INTERLEAVE_LIST(x) (INTERLEAVE_LIST_0_CBOBC_MEMSAD_REG + ((x) * 2 * 4))

//
// Global variables to keep track of event status across
// memory nodes.  BIT0 represents Memory Node 0, BIT1 is NODE1
// and so on
//
static UINT32   mEventProgressBitmap;   // Keeps track of memory nodes that have events in progress
static UINT32   mEventPendingBitmap;    // Keeps track of memory nodes that have new errors
static
LAST_ERR_INFO   mLastErrInfo[MC_MAX_NODE];

static  BOOLEAN             IsPamOpen = FALSE;
static  UINT32              OldPam0123RegVal;
static  UINT32              OldPam456RegVal;
static  UINT8               origSktIdOfMigTarget;  // Used to preserve original Skt Id of migration target

//
// Row bits[11:0], Col bits[9:0], bank bits[2:0] have a fixed mapping irrespective
// of # of column address/row address bits. The following
// gives the mapping
//
static const
UINT8 OpenPageAddrMapForBankBits2_0[MAX_BANK_BITS_FIXED_MAP] = {
    // B0,  B1, B2
      13, 17, 18
};

//
//When bank xor is enabled, bank needs to be xor with this bits
//
static const
UINT8 OpenPageAddrMapForBankXorBits2_0[MAX_BANK_BITS_FIXED_MAP] = {
    // B0,  B1, B2
      19, 20, 21
};




static const
UINT8 OpenPageAddrMapForRowBits11_0[MAX_ROW_BITS_FIXED_MAP] = {
    // R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11
       14, 15, 16, 19, 27, 20, 21, 22, 23, 24, 25,  26
};

static const
UINT8 OpenPageAddrMapForColBits9_0[2][MAX_COL_BITS_FIXED_MAP] = {
    // Independent CH mapping
    // C0,  C1, C2, C3, C4, C5, C6, C7, C8, C9
    {  3,   4,  5,  6,  7,  8,  9,  10, 11, 12 },

    // Lockstep CH mapping
    // C0,  C1, C2, C3, C4, C5, C6, C7, C8, C9
    {   4,  5,  28, 6,  7,  8,  9,  10, 11, 12 },
};
static const
UINT8 ClosedPageAddrMapForBankBits2_0[MAX_BANK_BITS_FIXED_MAP] = {
    // B0,  B1, B2
       6,   7,  8
};

//
//When bank xor is enabled, bank needs to be xor with this bits
//
static const
UINT8 ClosedPageAddrMapForBankXorBits2_0[MAX_BANK_BITS_FIXED_MAP] = {
    // B0,  B1, B2
    19,   20,  21
};



static const
UINT8 ClosedPageAddrMapForRowBits11_0[MAX_ROW_BITS_FIXED_MAP] = {
    // R0,  R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11
       14,  15, 16, 17, 19, 20, 21, 27, 9,  10, 11,  12
};

static const
UINT8 ClosedPageAddrMapForColBits9_0[2][MAX_COL_BITS_FIXED_MAP] = {
    // Independent CH mapping
    // C0,  C1, C2, C3, C4, C5, C6, C7, C8, C9
    {  3,   4,  5,  13, 18, 22, 23, 24, 25, 26 },

    // Lockstep CH mapping
    // C0,  C1, C2, C3, C4, C5, C6, C7, C8, C9
    {   4,  5,  28, 13, 18, 22, 23, 24, 25, 26 }
};

//
// Row bits[17:12], Col bits[11:10] are dynamically mapped based on
// # of column address/row address bits. The following
// gives the mapping. The mapping is same for both Closed and Open
// page modes
//
// ??? Close page Lockstep 8GB 8r LRDIMM RM=4
static const
UINT8 AddrMapForRowBits17_12[2][4][MAX_ROW_BITS_DYNAMIC_MAP] = {
    //
    // Independent CH
    //
    {
      // R12  R13 R14 R15 R16 R17
      {  28,  29, 30, 31, 32, 33 },     // # of col bits = 10
      {  28,  30, 31, 32, 33, 34 },     // # of col bits = 11
      {  28,  30, 31, 32, 33, 34},      // # of col bits = 12
      {  28,  30, 31, 32, 33, 34},      // # of col bits = 13
    },

    //
    // Lock-step CH
    //
    {
      // R12  R13 R14 R15 R16 R17
      {  29,  30, 31, 32, 33, 34 },     // # of col bits = 10
      {  29,  31, 32, 33, 34, 35 },     // # of col bits = 11
      {  29,  31, 32, 33, 34, 35},      // # of col bits = 12  
      {  29,  31, 32, 33, 34, 35},      // # of col bits = 13 
    },
};

static const
UINT8 AddrMapForColBits11_10[2][7][MAX_COL_BITS_DYNAMIC_MAP] = {
    //
    // Independent CH mapping
    //
    {
        // C10  C11
        {  0xff,  0xFF, 0xff,0xff}, // # of Row bits = 12
        {  0xFF,  0xFF, 0xFF,0xFF}, // # of Row bits = 13
        {  0xFF,  29,   0xff,0xff}, // # of Row bits = 14
        {  0xFF,  29,   0xFF,  33}, // # of Row bits = 15
        {  0xFF,  29,   0xFF,  34}, // # of Row bits = 16
        {  0xFF,  29,   0xFF,  35}, // # of Row bits = 17
        {  0xFF,  29,   0xFF,0xFF}, // # of Row bits = 18,
    },

    //
    // Lockstep CH mapping
    //
    {
        // C10    C11   C12  C13
        {  0xff,  0xFF, 0xff,0xff}, // # of Row bits = 12
        {  0xFF,  0xFF, 0xFF,0xFF}, // # of Row bits = 13
        {  0xff,  30,   0xff,0xff}, // # of Row bits = 14
        {  0xFF,  30,   0xFF,  34}, // # of Row bits = 15
        {  0xFF,  30,   0xFF,  35}, // # of Row bits = 16
        {  0xFF,  30,   0xFF,  36}, // # of Row bits = 17     //  ??? open page Lockstep 8GB 8r LRDIMM RM=4  ~ row bits 10
        {  0xFF,  30,   0xFF,0xFF}, // # of Row bits = 18     //  ??? open page Lockstep 8GB 8r LRDIMM RM=4  ~ row bits 11
    }
};

// DDR4

//
// Row bits[11:0], Col bits[9:0], bank bits[2:0] have a fixed mapping irrespective
// of # of column address/row address bits. The following
// gives the mapping
//
static const
UINT8 OpenPageAddrMapForBankBits1_0_DDR4[2][MAX_BANK_BITS_FIXED_MAP_DDR4] = {
    // B0,  B1
     { 18,  19 },
     { 18,  19 }
};

//
// Bank Address needs to be xored with this bits from the rank address
// The following gives the mapping
//
static const
UINT8 OpenPageAddrMapForBankXorBits1_0_DDR4[2][MAX_BANK_BITS_FIXED_MAP_DDR4] = {
  // B0,  B1
  { 22, 23 },
  { 22, 23 }
};

// followed secondary address mapping.
static const
UINT8 OpenPageAddrMapForBGBits1_0_DDR4[2][MAX_BG_BITS_FIXED_MAP_DDR4] = {
    // B0,  B1
      {13,  17 },
      {6,   17 }
};

//
// Bank Group needs to be xored with this bits from the rank address
// The following gives the mapping
//

static const
UINT8 OpenPageAddrMapForBGXorBits1_0_DDR4[2][MAX_BG_BITS_FIXED_MAP_DDR4] = {
  // B0,  B1
  {20,  21 },
  {20,  21 }
};



static const
UINT8 OpenPageAddrMapForRowBits11_0_DDR4[2][MAX_ROW_BITS_FIXED_MAP_DDR4] = {
    // R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11
      { 14, 15, 16, 20, 28, 21, 22, 23, 24, 25,  26, 27 },
      { 14, 15, 16, 20, 28, 21, 22, 23, 24, 25,  26, 27 }
};

static const
UINT8 OpenPageAddrMapForColBits9_0_DDR4[2][2][MAX_COL_BITS_FIXED_MAP_DDR4] = {
   {  // normal mode
      // Independent CH mapping
      // C0,  C1, C2, C3, C4, C5, C6, C7, C8, C9
      {  3,   4,  5,  6,  7,  8,  9,  10, 11, 12 },

      // Lockstep CH mapping
      // C0,  C1, C2, C3, C4, C5, C6, C7, C8, C9
      {   4,  5,  29, 6,  7,  8,  9,  10, 11, 12 },
    },
    { // secondary mode
      // Independent CH mapping
      // C0,  C1, C2, C3, C4, C5, C6, C7, C8, C9
      {  3,   4,  5,  7,  8,  9,  10, 11, 12, 13 },

      // Lockstep CH mapping
      // C0,  C1, C2, C3, C4, C5, C6, C7, C8, C9
      {   4,  5,  29, 7,  8,  9,  10, 11, 12, 13 },
    }
};
static const
UINT8 ClosedPageAddrMapForBankBits1_0_DDR4[MAX_BANK_BITS_FIXED_MAP_DDR4] = {
    // B0,  B1
       8,   9
};

static const
UINT8 ClosedPageAddrMapForBankXorBits1_0_DDR4[MAX_BANK_BITS_FIXED_MAP_DDR4] = {
  // B0,  B1
  22, 28
};


static const
UINT8 ClosedPageAddrMapForBGBits1_0_DDR4[MAX_BG_BITS_FIXED_MAP_DDR4] = {
    // BG0,  BG1
       6,   7
};

static const
UINT8 ClosedPageAddrMapForBGXorBits1_0_DDR4[MAX_BG_BITS_FIXED_MAP_DDR4] = {
    // BG0,  BG1
    20,   21
};

static const
UINT8 ClosedPageAddrMapForRowBits11_0_DDR4[MAX_ROW_BITS_FIXED_MAP_DDR4] = {
    // R0,  R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11
      15, 16, 17, 18, 20, 21, 22, 28, 10, 11, 12, 13
};

static const
UINT8 ClosedPageAddrMapForColBits9_0_DDR4[2][MAX_COL_BITS_FIXED_MAP_DDR4] = {
    // Independent CH mapping
    // C0,  C1, C2, C3, C4, C5, C6, C7, C8, C9
    {  3,   4,  5,  14, 19, 23, 24, 25, 26, 27 },

    // Lockstep CH mapping
    // C0,  C1, C2, C3, C4, C5, C6, C7, C8, C9
    {   4,  5,  29, 14, 19, 23, 24, 25, 26, 27 },
};

//
// Row bits[17:12], Col bits[11:10] are dynamically mapped based on
// # of column address/row address bits. The following
// gives the mapping. The mapping is same for both Closed and Open
// page modes
//
// ??? Close page Lockstep 8GB 8r LRDIMM RM=4
static const
UINT8 AddrMapForRowBits17_12_DDR4[2][1][MAX_ROW_BITS_DYNAMIC_MAP_DDR4] = {
    //
    // Independent CH
    //
    {
      // R12  R13 R14 R15 R16 R17
      {  29,  30, 31, 32, 33, 34 },     // # of col bits = 10
    },

    //
    // Lock-step CH
    //
    {
      // R12  R13 R14 R15 R16 R17
      {  30, 31, 32, 33, 34,  35 },     // # of col bits = 10
    },
};
static const
UINT8 AddrMapForCSBits2_0_DDR4[2][5][MAX_CS_BITS_SUPPORTED_DDR4] = {
    //
    // Independent CH
    //
    {
      // C0  C1 C2
      {  31,  32, 33 },     // # of row bits = 14
      {  32,  33, 34 },     // # of row bits = 15
      {  33,  34, 35 },     // # of row bits = 16
      {  34,  35, 36 },     // # of row bits = 17
      {  35,  36, 37 },     // # of row bits = 18
    },

    //
    // Lock-step CH
    //
    {
      // C0  C1 C2
      {  32,  33, 34 },     // # of row bits = 14
      {  33,  34, 35 },     // # of row bits = 15
      {  34,  35, 36 },     // # of row bits = 16
      {  35,  36, 37 },     // # of row bits = 17
      {  36,  37, 38 },     // # of row bits = 18
    },
};

// DDR4 dimm 
UINT8 mDdr4Dimm;  // 1 - DDR4 dimm, 0- ddr3



UINT8
NodeToSkt(
  UINT8 Node
)
/*++

Routine Description:
   this function returns the socket number

Arguments:

  i/p  : node number

Returns:
  o/p : Socket number for valid node; 0xff for invalid node

--*/
{

  return NODE_TO_SKT(Node);

}

UINT8
NodeToHaNum
(
  UINT8 Node
)
/*++

Routine Description:
   this function returns the socket number

Arguments:

  i/p  : node number

Returns:
  o/p : HaNum

--*/
{
  UINT8 HaNum;

  if(mSystemMemoryMap->NodeInfo[Node].Enabled == 00 ) return 0xff;

  if(mSystemMemoryMap->NodeInfo[Node].NumChPerHA == 2)       {
    HaNum = (Node & BIT0);
  } else {
    HaNum  = 00;
  }
  return HaNum;
}

UINT8
SktHaToNode
(
  UINT8 Skt,
  UINT8 HaNum
)
{
  UINT8 Node =0;

  Node   = (Skt << 1) | (HaNum & BIT0);
  
  return Node;
}

UINT8
SktHaToHaNodeId
(
  UINT8 Skt,
  UINT8 HaNum
)
{
  UINT8 haNodeId;

  haNodeId = ((Skt & 0x4) << 1) + (Skt & 0x3) + (4 * HaNum);

  return haNodeId;
}

UINT8
HaNodeIdToNode (
  UINT8 HaNodeId
)
{
  UINT8 Ha,SktId,Node;

  Ha    = (HaNodeId & BIT2) >> 2;
  SktId = (HaNodeId & 3);
  Node  = (SktId << 1) + Ha;

  return Node;

}

UINT8
HaNodeIdToSkt (
  UINT8 haNodeId
)
{
  UINT8 ha,SktId;

  ha     = (haNodeId & BIT2) >> 2;
  SktId = (haNodeId & 3);

  return SktId;

}

UINT8
HaNodeIdToHa (
  UINT8 haNodeId
)
{
UINT8 ha;

ha     = (haNodeId & BIT2) >> 2;

return ha;

}

UINT8
NodeChToDdrCh(
  UINT8 node,
  UINT8 ch
)
/*++

Routine Description:
   this function returns the DDR Ch number

Arguments:

  i/p  : node number and Channel number(Node Based)

Returns:
  o/p :DdrCh number; 0xff for invalid node or ch

--*/
{
  UINT8 DdrCh , haNum;

  haNum   = 0;
  DdrCh   = ch;

  if(mSystemMemoryMap->NodeInfo[node].Enabled == 00 ) return 0xff;

  // 1ha : Ddrch0,1,2,3 -> ha0ch0,1 2,3
  // 2ha : Ddrch0,1,2,3 -> ha0ch0,1 and Ha1ch0,1
  if(mSystemMemoryMap->NodeInfo[node].NumChPerHA == 2) { // 2 HA case.
    haNum   = node & 01;
    if(ch >= 2) return 0xff;
    if( haNum == 1) DdrCh  = ch + 2;    
  }

  return DdrCh;
}

/*++

Routine Description:
    Initializes all the global data structures/variables
    required for MemRas routines

Arguments:

  None

Returns:
  EFI_SUCCESS - if no errors
  non-zero error code on any errors

--*/
EFI_STATUS
EFIAPI
InitializeMemRasData(
 )
{
  EFI_STATUS status = EFI_SUCCESS;

  return status;
}

/*++

Routine Description:
    Saves the DdrCh/Dimm/Rank information

Arguments:

  NodeId    - Memory controller ID
  Ch        - Channel ID
  Rank      - Rank ID

Returns:
  None
--*/
static
void
SaveErrInfo(
    IN UINT8                          NodeId,
    IN UINT8                          DdrCh,
    IN UINT8                          Rank
 )
{
  UINT8                             failDevId;
  UINT8                             tgtDdrCh;
  UINT8                             dimmSlot;
  UINT32                            devTagCntlRegBaseAddr;
  DEVTAG_CNTL_0_MCDDC_DP_STRUCT     devTagCntlReg;
  UINT8                             PhysicalRank;

  devTagCntlRegBaseAddr = DEVTAG_CNTL_0_MCDDC_DP_REG;

  //
  // Convert logical rank to DIMM/Physical rank
  //
  PhysicalRank = GetLogicalToPhysicalRankId(NodeId, DdrCh, Rank);
  dimmSlot = PhysicalRank / MAX_RANK_DIMM;
  mLastErrInfo[NodeId].Dimm = dimmSlot;
  mLastErrInfo[NodeId].Rank = PhysicalRank % MAX_RANK_DIMM;

  //
  // If lock-step Mode is enabled, determine target
  // DDR CH ID from fail device ID and DIMM type
  //
  tgtDdrCh = DdrCh;
  if(mMemRas->SystemMemInfo->NodeInfo[NodeId].RasModesEnabled & LOCKSTEP_EN){
    //
    // Get Fail device ID
    // Fail device is captured in DEVTAG_CNTL reg
    //
    devTagCntlReg.Data = (UINT8) MemReadPciCfg( NodeId, DdrCh, devTagCntlRegBaseAddr + Rank );
    failDevId = devTagCntlReg.Bits.faildevice;

    //
    // Device to DDRCH mapping
    // x4 -> DEV0-17 DDRCH0; DEV18-35 DDRCH1;
    // x8 -> DEV0-8  DDRCH0; DEV09-17 DDRCH1;
    //
    if(mMemRas->SystemMemInfo->NodeInfo[NodeId].ChannelInfo[DdrCh].DimmInfo[dimmSlot].X4Present){
      //
      // X4 DIMM
      //
      if(failDevId > 17){
        tgtDdrCh++;
      }
    } else {
      //
      // X8 DIMM
      //
      if(failDevId > 8){
        tgtDdrCh++;
      }
    } // if x4
  } // if LOCKSTEP_EN

  mLastErrInfo[NodeId].DdrCh = tgtDdrCh;
}

/*++

Routine Description:
    Return DDRCH/DIMM/Rank that had the most recent error

Arguments:

  NodeId    - Memory controller ID
  *DdrCh    - ptr to buffer to hold DdrCh
  *Dimm     - ptr to buffer to hold Dimm
  *Rank     - ptr to buffer to hold Rank

Returns:
  DdrCh
  Dimm
  Rank
--*/
void
EFIAPI
GetLastErrInfo(
    IN UINT8                          NodeId,
    IN UINT8                          *DdrCh,
    IN UINT8                          *Dimm,
    IN UINT8                          *Rank
 )
{
  *DdrCh  = mLastErrInfo[NodeId].DdrCh;
  *Dimm   = mLastErrInfo[NodeId].Dimm;
  *Rank   = mLastErrInfo[NodeId].Rank;
}


/*++

Routine Description:
    Checks if the specified rank has error overflow
    If the CPU error counter overflows clear overflow
    status in CPU; counters are not cleared

    Note: In lock-step mode, CPU logs all rank ECC errors in
    registers of Primary channel only (i.e., DDR CH0 or DDR CH2)

Arguments:

  NodeId    - Memory controller ID
  Ch        - Channel ID
  Rank      - Rank ID

Returns:
  TRUE      - if the specified rank has error overflow
  FALSE     - specified rank has no error overflow

--*/
BOOLEAN
IsErrorExcdForThisRank(
    IN UINT8 NodeId,
    IN UINT8 DdrCh,
    IN UINT8 Rank
 )
{
  BOOLEAN                         cpuErrOverflow = FALSE;
  BOOLEAN                         isErrorExcd    = FALSE;
  CORRERRORSTATUS_MCDDC_DP_BDX_STRUCT corrErrStsReg;
  UBOXERRSTS_UBOX_CFG_STRUCT      uboxErrorStatusReg;

  //
  // Check error status in CPU for this rank
  //
  corrErrStsReg.Data = MemReadPciCfg( NodeId, DdrCh, CORRERRORSTATUS_MCDDC_DP_REG );
  if( corrErrStsReg.Bits.err_overflow_stat & (1 << Rank)){
    cpuErrOverflow = TRUE;
  }

  if( cpuErrOverflow ){

    isErrorExcd = TRUE;

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
      RAS_DEBUG((4, "ch err_overflow:%lx    rank_err_overflow:%x ",
        MemReadPciCfg( NodeId, DdrCh, CORRERRORSTATUS_MCDDC_DP_REG ),
        MemReadPciCfg( NodeId, DdrCh, (UINT32)(CORRERRCNT_0_MCDDC_DP_REG) + (Rank/2) * 4 )
        ));

      corrErrStsReg.Data = 0;
      corrErrStsReg.Bits.err_overflow_stat = 1 << Rank; // clear only for the rank that is handled
      MemWritePciCfg(  NodeId, DdrCh, CORRERRORSTATUS_MCDDC_DP_REG, corrErrStsReg.Data );
    }

    //
    // Clear SMI status
    //
    uboxErrorStatusReg.Data = MemReadPciCfg( NodeToSkt(NodeId), 0, UBOXERRSTS_UBOX_CFG_REG );
    uboxErrorStatusReg.Bits.smi_delivery_valid  = 1;
    uboxErrorStatusReg.Bits.smisrcimc           = 0;
    MemWritePciCfg( NodeToSkt(NodeId), 0, UBOXERRSTS_UBOX_CFG_REG, uboxErrorStatusReg.Data );

  }


  //
  // Save error Info
  //
  if( isErrorExcd ){
    //
    // If in lock-step mode, then the use the fail device ID & device
    // width to determine the correct DDR CH the failed rank falls
    //
    SaveErrInfo(  NodeId, DdrCh, Rank );
  }

  return isErrorExcd;
}

/*++

Routine Description:
    Clears error counters and over flow status for specified
    rank in CPU

Arguments:

  
  NodeId    - Memory controller ID
  Ch        - Channel ID
  Rank      - Rank ID

Returns:
  None
--*/
void
ClearErrorsForThisRank(
    IN UINT8 NodeId,
    IN UINT8 DdrCh,
    IN UINT8 Rank
 )
{
  CORRERRCNT_0_MCDDC_DP_STRUCT  corrErrCntReg;
  UINT32                        corrErrCntRegBaseAddr = CORRERRCNT_0_MCDDC_DP_REG;

  //
  // Clear error counters and status in CPU for this rank
  //
  corrErrCntReg.Data = MemReadPciCfg(  NodeId, DdrCh, corrErrCntRegBaseAddr + (Rank/2) * 4 );
  if( (Rank % 2) == 0) {
    corrErrCntReg.Bits.overflow_0     = 1;
    corrErrCntReg.Bits.overflow_1     = 0;
    corrErrCntReg.Bits.cor_err_cnt_0  = 0;
  } else {
    corrErrCntReg.Bits.overflow_1     = 1;
    corrErrCntReg.Bits.overflow_0     = 0;
    corrErrCntReg.Bits.cor_err_cnt_1  = 0;
  }
  MemWritePciCfg(  NodeId, DdrCh, corrErrCntRegBaseAddr + (Rank/2) * 4, corrErrCntReg.Data );

}
/*++

Routine Description:
  Checks the error status registers and returns a bitmap of
  Memory NodeIDs that have errors. A copy of the bitmap
  is also stored for internal use

Arguments:

  *ErrorBitmap      - ptr to buffer to return 32-bit bitmap of Nodes with error

Returns:
  *ErrorBitmap      -  32 bit bitmap of Nodes with error. If a bit is set, then
                       that Memory controller has an error

--*/
void
EFIAPI
GetBitmapOfNodeWithErrors(
    IN OUT UINT32                     * ErrorBitmap
  )
{
  CORRERRORSTATUS_MCDDC_DP_BDX_STRUCT corrErrorStatusReg;
  UBOXERRSTS_UBOX_CFG_STRUCT      uboxErrorStatusReg;
  UINT8                           nodeId;
  UINT8                           ch;
  UINT32                          data;
  UINT8                           socket;
  BOOLEAN                         clr_smi_sts;

  *ErrorBitmap = 0;

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    clr_smi_sts = FALSE;
    for (nodeId = SKTMC_TO_NODE(socket, 0); nodeId <= SKTMC_TO_NODE(socket, (MAX_HA-1)); nodeId++) {
      //
      // If this node is not present, move to next node
      //
      if (!mMemRas->SystemMemInfo->NodeInfo[nodeId].Enabled) {
        continue;
      }
      uboxErrorStatusReg.Data = MemReadPciCfg (socket, 0, UBOXERRSTS_UBOX_CFG_REG);
      RAS_DEBUG((4,"   GetBitmapOfNodeWithErrors-- mc:%d  imc status :%x\n", nodeId, uboxErrorStatusReg.Data));

      if (uboxErrorStatusReg.Bits.smisrcimc && uboxErrorStatusReg.Bits.smi_delivery_valid) {
        //
        // Check if any of the channels have rank error overflow
        //
        for (ch = 0; ch < MAX_CH; ch++) {
          if (mSystemMemoryMap->NodeInfo[nodeId].ChannelInfo[ch].Enabled == 0) {
            continue;
          }
          corrErrorStatusReg.Data = MemReadPciCfg (nodeId, ch, CORRERRORSTATUS_MCDDC_DP_REG);
          RAS_DEBUG((4,"   correct error overflow channel:%d  status :%x\n", ch, corrErrorStatusReg.Data));
          if (corrErrorStatusReg.Bits.err_overflow_stat != 0) {
            *ErrorBitmap |= (1 << nodeId);
            //
            // Set pending event only if there is no other event already pending
            // RAS code is non re-entant. We can handle only 1 event at a time
            //
            GetOrSetErrorHandlingStatus (nodeId, GetEventPendingStatus, &data);
            if (!data) {
              data = 1;
              GetOrSetErrorHandlingStatus (nodeId, UpdateEventPendingStatus, &data); // Save the error bitmap for internal use
            }
            clr_smi_sts = TRUE;
            break;    // Move to next Node
          }
        } // for (ch)
      }
    } // for (nodeId)
    if (clr_smi_sts) {
      //
      // Clear SMI status
      //
      uboxErrorStatusReg.Bits.smi_delivery_valid  = 1;
      uboxErrorStatusReg.Bits.smisrcimc           = 0;
      MemWritePciCfg (socket, 0, UBOXERRSTS_UBOX_CFG_REG, uboxErrorStatusReg.Data);
    }
  } // for (socket)
}

/*++

Routine Description:
  Returns a bitmap of  Memory NodeIDs with event handling already
  in progress

Arguments:

  *EventProgressBitmap  - ptr to buffer to return 32-bit bitmap of Nodes with events in progress

Returns:
  *EventProgressBitmap  - 32 bit bitmap of Nodes with events in progress. If a bit is set, then
                          that Memory controller has event handling in progress

--*/
void
EFIAPI
GetBitMapOfNodeWithEventInProgress(
    IN OUT UINT32                     * EventProgressBitmap
  )
{
  *EventProgressBitmap = mEventProgressBitmap;
}

/*++

Routine Description:
  Updates / Gets error handling status for specified NodeID based on
  Opcode. Function will soft-hang if it encounters any error

Arguments:

  NodeId            - Memory Node Id
  Opcode            - indicates the operation to be performed on
                      error/event status variables (mEventProgressBitmap, mEventPendingBitmap)
  *Value            - caller passes the value in this buffer to update the status

Returns:
  *Value            -  value of event status when Opcode = GetEventProgressStatus or GetEventPendingStatus

--*/
void
EFIAPI
GetOrSetErrorHandlingStatus (
  IN UINT8                           NodeId,
  IN ERR_STATUS_OPCODE               Opcode,
  IN OUT UINT32                      *Value
 )
{
  switch( Opcode ){
  case UpdateEventProgressStatus:
    switch( *Value ){
    case 0:
      ASSERT( (mEventProgressBitmap & (1 << NodeId)) != 0 );        // Make sure we have an event in progress
      mEventProgressBitmap &= ~(1 << NodeId);
      RAS_DEBUG((2, "GetOrSetErrorHandlingStatus - Clear Event Progress status for Node%d\n", NodeId));
      break;

    case 1:
      //
      // We can move an event to in-progress only if there is a
      // pending error already for this node. Assert it
      //
      ASSERT( (mEventPendingBitmap & (1 << NodeId)) != 0 );
      mEventProgressBitmap |= (1 << NodeId);
      RAS_DEBUG((2, "GetOrSetErrorHandlingStatus - Set Event Progress status for Node%d\n", NodeId));
      break;

    default:
      ASSERT_EFI_ERROR(EFI_UNSUPPORTED);
      break;
    }
    break;

  case UpdateEventPendingStatus:
    switch( *Value ){
    case 0:
      ASSERT( (mEventProgressBitmap & (1 << NodeId)) == 0 );        // Make sure we don't have an event in progress
      mEventPendingBitmap &= ~(1 << NodeId);
      RAS_DEBUG((2, "GetOrSetErrorHandlingStatus - Clear Event Pending status for Node%d\n", NodeId));
      break;

    case 1:
      //
      // Make sure there are no events that are already
      // pending i.e., yet to be handled.  RAS code is not
      // designed to handle new events when there are pending
      // events
      //
      ASSERT( (mEventPendingBitmap & (1 << NodeId)) == 0 );
      mEventPendingBitmap |= (1 << NodeId);
      RAS_DEBUG((2, "GetOrSetErrorHandlingStatus - Set Event Pending status for Node%d\n", NodeId));
      break;

    default:
      ASSERT_EFI_ERROR(EFI_UNSUPPORTED);
      break;
    }
    break;

  case GetEventProgressStatus:
    *Value = (mEventProgressBitmap >> NodeId) & 1;
    RAS_DEBUG((2, "GetOrSetErrorHandlingStatus - Get Event Progress status for Node%d = %d\n", NodeId, *Value));
    break;

  case GetEventPendingStatus:
    *Value = (mEventPendingBitmap >> NodeId) & 1;
    RAS_DEBUG((2, "GetOrSetErrorHandlingStatus - Get Event Pending status for Node%d = %d\n", NodeId, *Value));
    break;

  default:
    ASSERT_EFI_ERROR(EFI_UNSUPPORTED);
    break;
  } // switch( opcode )
}

/*++

Routine Description:
    Read a CPU Register

Arguments:

  SktOrNodeId   - Memory Node Id if the targeted function block is  MC
                  for all non-MC blocks it is treated as Socket ID
  ChIdOrBoxInst - Channel ID if the targeted function block is  MC
                  for all non-MC blocks it is treated as box instance
  Offset        - encoded register offset (box ID, function block info
                  embedded along with reg offset)

Returns:
  Data          - value of register

  If this function encounters an error, it will soft-hang
--*/
UINT32
EFIAPI
MemReadPciCfg (
    IN UINT8    SktOrNodeId,
    IN UINT8    ChIdOrBoxInst,
    IN UINT32   Offset
)
{
  CSR_OFFSET  RegOffset;
  UINT8       boxType;

  RegOffset.Data = Offset;
  boxType = (UINT8)RegOffset.Bits.boxtype;

  //
  // MemRead/WritePciCfgEx takes only NodeId as param and derives socket ID
  // from that. Make sure we convert SktOrNodeId to Node ID for non-MC box types
  //
  if( boxType == BOX_MC || boxType == BOX_MCDDC || boxType == BOX_MCIO ){
    return mCpuCsrAccess->ReadMemCpuCsr(SktOrNodeId, ChIdOrBoxInst, Offset);
  } else {
    return mCpuCsrAccess->ReadMemCpuCsr(SktHaToNode(SktOrNodeId, 0), ChIdOrBoxInst, Offset);
  }
}

/*++

Routine Description:
    Gets MMCFG address of CPU Register

Arguments:

  SktId         - Socket ID
  BoxInst       - Box Instance
  Offset        - encoded register offset (box ID, function block info
                  embedded along with reg offset)

Returns:
  Data          - value of mmcfg address

  If this function encounters an error, it will soft-hang, this function will always return offset for dword boundaries
--*/
//
//4987134 start
//

UINTN
EFIAPI
GetCpuCfgPciAddress (
    IN UINT8    SktId,
    IN UINT8    BoxInst,
    IN UINT32   Offset
)

//
//4987134 end
//

{
  UINT8 Size = 4;

  return mCpuCsrAccess->GetCpuCsrAddress(SktId, BoxInst, Offset, &Size);
}

/*++

Routine Description:
    Write data to CPU Register

Arguments:

  SktOrNodeId   - Memory Node Id if the targeted function block is  MC
                  for all non-MC blocks it is treated as Socket ID
  ChIdOrBoxInst - Channel ID if the targeted function block is  MC
                  for all non-MC blocks it is treated as box instance
  Offset        - encoded register offset (box ID, function block info
                  embedded along with reg offset)
  Data          - Data to be written

Returns:
  None

  If this function encounters an error, it will soft-hang
--*/
void
EFIAPI
MemWritePciCfg (
    IN UINT8    SktOrNodeId,
    IN UINT8    ChIdOrBoxInst,
    IN UINT32   Offset,
    IN UINT32   Data
)
{
  CSR_OFFSET  RegOffset;
  UINT8       boxType;

  RegOffset.Data = Offset;
  boxType = (UINT8)RegOffset.Bits.boxtype;

  //
  // MemRead/WritePciCfgEx takes only NodeId as param and derives socket ID
  // from that. Make sure we convert SktOrNodeId to Node ID for non-MC box types
  //
  if( boxType == BOX_MC || boxType == BOX_MCDDC || boxType == BOX_MCIO ){
    mCpuCsrAccess->WriteMemCpuCsr(SktOrNodeId, ChIdOrBoxInst, Offset, Data);
  } else {
    mCpuCsrAccess->WriteMemCpuCsr(SktHaToNode(SktOrNodeId, 0), ChIdOrBoxInst, Offset, Data);
  }
}


/*++

Routine Description:
    Read a PCI Register

Arguments:

  SktId         - SocketId
  Offset        - Offset

Returns:
  Data          - value of register

  If this function encounters an error, it will soft-hang
--*/
UINT32
EFIAPI
PciReadCfg (
    IN UINT8    SktId,
    IN UINT32   Offset
)
{
  //
  // To be implemented
  //
  return 0;
}

/*++

Routine Description:
    Write data to PCI Register

Arguments:

  SktId         - Socket ID
  Offset        - Offset
  Data          - Data to be written

Returns:
  None

--*/
void
EFIAPI
PciWriteCfg (
    IN UINT8    SktId,
    IN UINT32   Offset,
    IN UINT32   Data
)
{
  //
  // To be implemented
  //
}

/*++

Routine Description:
   Open PAM region for DRAM RW access

Arguments:

  SocketId - Socket ID for which PAM has to be opened

Returns:
  EFI_SUCCESS / Error code

  If this function encounters an error, it will soft-hang
--*/
EFI_STATUS
EFIAPI
OpenPamRegion(
    IN UINT8 SocketId
 )
{
  PAM0123_CBOBC_MEMSAD_HSX_BDX_STRUCT pam0123Reg;
  PAM456_CBOBC_MEMSAD_STRUCT  pam456Reg;
  EFI_STATUS                  status = EFI_SUCCESS;

  if(!IsPamOpen){
    pam0123Reg.Data   = 0;
    pam456Reg.Data    = 0;

    //
    // Save current register values
    //
    OldPam0123RegVal  = MemReadPciCfg(SocketId, 0, PAM0123_CBOBC_MEMSAD_REG);
    OldPam456RegVal   = MemReadPciCfg(SocketId, 0, PAM456_CBOBC_MEMSAD_REG);

    //
    // Open-up PAM regions
    //
    pam0123Reg.Bits.pam0_hienable = PAM_DRAM_RW;  // 0F0000-0FFFFF
    pam0123Reg.Bits.pam1_hienable = PAM_DRAM_RW;  // 0C4000-0C7FFF
    pam0123Reg.Bits.pam1_loenable = PAM_DRAM_RW;  // 0C0000-0C3FFF
    pam0123Reg.Bits.pam2_hienable = PAM_DRAM_RW;  // 0CC000-0CFFFF
    pam0123Reg.Bits.pam2_loenable = PAM_DRAM_RW;  // 0C8000-0CBFFF
    pam0123Reg.Bits.pam3_hienable = PAM_DRAM_RW;  // 0D4000-0D7FFF
    pam0123Reg.Bits.pam3_loenable = PAM_DRAM_RW;  // 0D0000-0D3FFF

    pam456Reg.Bits.pam4_hienable  = PAM_DRAM_RW;  // 0DC000-0DFFFF
    pam456Reg.Bits.pam4_loenable  = PAM_DRAM_RW;  // 0D8000-0DBFFF
    pam456Reg.Bits.pam5_hienable  = PAM_DRAM_RW;  // 0E4000-0E7FFF
    pam456Reg.Bits.pam5_loenable  = PAM_DRAM_RW;  // 0E0000-0E3FFF
    pam456Reg.Bits.pam6_hienable  = PAM_DRAM_RW;  // 0EC000-0EFFFF
    pam456Reg.Bits.pam6_loenable  = PAM_DRAM_RW;  // 0E8000-0EBFFF

    MemWritePciCfg(SocketId, 0, PAM0123_CBOBC_MEMSAD_REG, pam0123Reg.Data);
    MemWritePciCfg(SocketId, 0, PAM456_CBOBC_MEMSAD_REG, pam456Reg.Data);

    IsPamOpen = TRUE;

  }
  return status;
}

/*++

Routine Description:
   Restore PAM region with previously saved settings

Arguments:

  SocketId - Socket ID for which PAM has to be opened

Returns:
  EFI_SUCCESS / Error code

  If this function encounters an error, it will soft-hang
--*/
EFI_STATUS
EFIAPI
RestorePamRegion(
    IN UINT8 SocketId
 )
{
  EFI_STATUS                  status = EFI_SUCCESS;

  if(IsPamOpen){
    //
    // Restore previous register values
    //
    MemWritePciCfg(SocketId, 0, PAM0123_CBOBC_MEMSAD_REG, OldPam0123RegVal);
    MemWritePciCfg(SocketId, 0, PAM456_CBOBC_MEMSAD_REG, OldPam456RegVal);

    IsPamOpen = FALSE;

  }
  return status;
}

/*++

Routine Description:
   Translates Channel Address to Rank Address

Arguments:

  PTRANSLATED_ADDRESS  - ptr to Address Translation structure
  Following fields in TRANSLATED_ADDRESS structure is expected
  to be valid:
    SocketId
    MemoryControllerId
    HaNodeId
    ChannelAddress
    SystemAddress
    ChannelId

Modifies:
  Following fields in ADDRESS_TRANSATION structure are updated:
    PhysicalRankId
    DimmSlot
    DimmRank
    RankAddress

Returns:
  EFI_SUCCESS / Error code


--*/
EFI_STATUS
TranslateRIR (
    PTRANSLATED_ADDRESS TranslatedAddress
 )
{
  UINT64  physicalAddress;
  UINT64  channelAddress;
  UINT64  rankAddress;
  UINT64  rirLimit;
  UINT64  tempAddress;
  UINT8   rir;
  UINT8   node;
  UINT8   rirWays;
  UINT8   rirVal;
  UINT8   rankInterleave;
  UINT8   channel;
  UINT8   match;
  UINT8   pageShift;
  UINT32  rirRnkTgt0;
  UINT32  rirOffset0;
  RIRWAYNESSLIMIT_0_MCDDC_DESC_BDX_STRUCT imcRIRWaynessLimitBdx;
  RIRILV0OFFSET_0_MCDDC_DESC_BDX_STRUCT   imcRIRIlv0OffsetBdx;
  RIRWAYNESSLIMIT_0_MCDDC_DESC_HSX_STRUCT imcRIRWaynessLimitHsx;
  RIRILV0OFFSET_0_MCDDC_DESC_HSX_STRUCT   imcRIRIlv0OffsetHsx;
  MCMTR_MC_MAIN_HSX_BDX_STRUCT            imcMcmtr;
  node  = TranslatedAddress->Node;
  channelAddress  = TranslatedAddress->ChannelAddress;
  physicalAddress = TranslatedAddress->SystemAddress;
  channel         = TranslatedAddress->ChannelId;

  //
  // Initialize return values
  //
  TranslatedAddress->RankAddress    = (UINT64)-1;
  TranslatedAddress->PhysicalRankId = (UINT8)-1;
  TranslatedAddress->DimmSlot       = (UINT8)-1;
  TranslatedAddress->DimmRank       = (UINT8)-1;

  imcMcmtr.Data = MemReadPciCfg( node, PARAM_UNUSED, MCMTR_MC_MAIN_REG );

  match = 0;
  for (rir = 0; rir < MAX_RIR; rir++) {
    if (match == 1) break;
    if (mHost.var.common.cpuType == CPU_BDX) {
      imcRIRWaynessLimitBdx.Data = MemReadPciCfg( node, channel, RIRWAYNESSLIMIT_0_MCDDC_DESC_REG + (rir * 4));
      rirVal = (UINT8) imcRIRWaynessLimitBdx.Bits.rir_val;
      rirLimit = (UINT64) imcRIRWaynessLimitBdx.Bits.rir_limit;
      rirWays = (UINT8) imcRIRWaynessLimitBdx.Bits.rir_way;
    } else {
      imcRIRWaynessLimitHsx.Data = MemReadPciCfg( node, channel, RIRWAYNESSLIMIT_0_MCDDC_DESC_REG + (rir * 4));
      rirVal = (UINT8) imcRIRWaynessLimitHsx.Bits.rir_val;
      rirLimit = (UINT64) imcRIRWaynessLimitHsx.Bits.rir_limit;
      rirWays = (UINT8) imcRIRWaynessLimitHsx.Bits.rir_way;
    }

    if (rirVal == 0) continue;

    //
    // Does channel address match current RIR rule limt
    //
    // limit is 512MB granularity (bits 45:29)
    //
    tempAddress = channelAddress >> 29;
    if (tempAddress <= rirLimit) {
      match = 1;
      RAS_DEBUG((1, "\nTranslateRIR: Matching Rir Entry: %d", rir));
      RAS_DEBUG((1, "\nTranslateRIR: Rir Ways: %d", rirWays));

      //
      // Determine target rank interleave index for the channel address
      // Close_page uses bits 8:6; Open_page uses bits 15:13
      //
      pageShift = (UINT8)(imcMcmtr.Bits.close_pg == 1 ? 6 : 13);
      rankInterleave = (UINT8)((channelAddress >> pageShift) % (1i64 << rirWays));

      //
      // Get target physical rank id & rank address
      //
      if (mHost.var.common.cpuType == CPU_BDX) {
        imcRIRIlv0OffsetBdx.Data = MemReadPciCfg(  node, channel,
                                                       RIRILV0OFFSET_0_MCDDC_DESC_REG + (rir * 8 * 4) + (rankInterleave * 4));
        rirRnkTgt0 = imcRIRIlv0OffsetBdx.Bits.rir_rnk_tgt0;
        rirOffset0 = imcRIRIlv0OffsetBdx.Bits.rir_offset0;
      } else {
        imcRIRIlv0OffsetHsx.Data = MemReadPciCfg(  node, channel,
                                                     RIRILV0OFFSET_0_MCDDC_DESC_REG + (rir * 8 * 4) + (rankInterleave * 4));
        rirRnkTgt0 = imcRIRIlv0OffsetHsx.Bits.rir_rnk_tgt0;
        rirOffset0 = imcRIRIlv0OffsetHsx.Bits.rir_offset0;
      }

      TranslatedAddress->PhysicalRankId = (UINT8) rirRnkTgt0;
      RAS_DEBUG((1, "\nTranslateRIR: rir_rnk_tgt0: %d", rirRnkTgt0));
      RAS_DEBUG((1, "\nTranslateRIR: rir_offset0: %x", rirOffset0));
      RAS_DEBUG((1, "\nTranslateRIR: shiftVal: %d", pageShift));
      if (TranslatedAddress->PhysicalRankId > 9) {
        //
        // error
        //
        return EFI_INVALID_PARAMETER;
      }
      TranslatedAddress->DimmSlot = TranslatedAddress->PhysicalRankId / MAX_RANK_DIMM;
      TranslatedAddress->DimmRank = TranslatedAddress->PhysicalRankId % MAX_RANK_DIMM;

      //
      // Determine RankAddress
      //
      rankAddress = channelAddress & ((1 << pageShift) - 1);    // low order rank address bits
      rankAddress |= ((channelAddress >> pageShift)/(1i64 << rirWays) - ((UINT64)rirOffset0 << (26-pageShift))) << pageShift;
      TranslatedAddress->RankAddress = rankAddress;
      RAS_DEBUG((1, "\nTranslateRIR: PhysicalRankId: %x", TranslatedAddress->PhysicalRankId));
      RAS_DEBUG((1, "\nTranslateRIR: DimmSlot: %x", TranslatedAddress->DimmSlot));
      RAS_DEBUG((1, "\nTranslateRIR: DimmRank: %x", TranslatedAddress->DimmRank));
      RAS_DEBUG((1, "\nTranslateRIR - RankAddress = %08x:%08x\n", (UINT32)(rankAddress >> 32), (UINT32)rankAddress));
    } // if rirLimit
  } // for

  if (match == 1) {
    return EFI_SUCCESS;
  } else {
    return EFI_INVALID_PARAMETER;
  }
} // TranslateRIR

/*++

Routine Description:
   Translates System Address to Channel Address

Arguments:

  PTRANSLATED_ADDRESS  - ptr to Address Translation structure
  Following fields in TRANSLATED_ADDRESS structure is expected
  to be valid:
    SocketId
    MemoryControllerId
    HaNodeId
    SystemAddress

Modifies:
  Following fields in ADDRESS_TRANSATION structure are updated:
    ChannelAddress
    ChannelId

Returns:
  EFI_SUCCESS / Error code


--*/
EFI_STATUS
TranslateTAD (
  PTRANSLATED_ADDRESS TranslatedAddress)
{
  UINT64  systemAddress;
  UINT64  channelAddress;
  UINT64  tempAddress;
  UINT64  tadLimit;
  UINT64  chOffset;
  UINT8   tad;
  UINT8   node;
  UINT8   sktWays;
  UINT8   sktWaysXchWays;
  UINT8   chInterleave;
  UINT8   channel;
  UINT8   chXorBitPos = 0;
  BOOLEAN match;
  UINT8   lowBits;
  UINT8   shiftVal;
  UINT8   secCh;
  MCMTR_MC_MAIN_HSX_BDX_STRUCT          imcMcmtr;
  TADCHNILVOFFSET_0_MCDDC_DESC_HSX_BDX_STRUCT imcTADChnlIlvOffset;
  HASYSDEFEATURE2_HA_CFG_HSX_BDX_STRUCT       haSysDefeature2;
  HASYSDEFEATURE_HA_CFG_STRUCT            haSysDefeature;
  TADWAYNESS_0_MC_MAIN_STRUCT       tadWayness;
  HACHFAILSTS_HA_CFG_STRUCT haChFailStsReg;
  
  node  = TranslatedAddress->Node;
  systemAddress = TranslatedAddress->SystemAddress;
  haSysDefeature2.Data = MemReadPciCfg(NodeToSkt(node), TranslatedAddress->MemoryControllerId, HASYSDEFEATURE2_HA_CFG_REG);
  imcMcmtr.Data = MemReadPciCfg(node, PARAM_UNUSED, MCMTR_MC_MAIN_REG);
haSysDefeature.Data           = MemReadPciCfg (NodeToSkt(node), TranslatedAddress->MemoryControllerId, HASYSDEFEATURE_HA_CFG_REG);

  //
  // Initialize return values
  //
  TranslatedAddress->ChannelId      = (UINT8)-1;
  TranslatedAddress->ChannelAddress = (UINT64)-1;
  TranslatedAddress->LockStepEn     = (UINT8)imcMcmtr.Bits.ls_en;  // or haSYSDEFEATURE.Bits.lockstep_en

  match = FALSE;
  for (tad = 0; tad < TAD_RULES; tad++) {
    if (match == TRUE) break;

    tadWayness.Data = MemReadPciCfg(  node, PARAM_UNUSED, TADWAYNESS_0_MC_MAIN_REG + (tad * 4));
    tadLimit = tadWayness.Bits.tad_limit;

    //
    // Does address match current tad rule limt
    //
    // limit is 64MB granularity (bits 45:26)
    //
    tempAddress = systemAddress >> 26;

    if (tempAddress <= tadLimit) {
      match = TRUE;

      sktWays =(UINT8) (1 << tadWayness.Bits.tad_skt_way);
      sktWaysXchWays = sktWays * ((UINT8)tadWayness.Bits.tad_ch_way+1);
      RAS_DEBUG((1, "\nTranslateTAD: sktWays: %d", sktWays));
      RAS_DEBUG((1, "\nTranslateTAD: sktWaysXchWays: %d", sktWaysXchWays));

      //
      // Determine target channel(s)
      //
      if(haSysDefeature2.Bits.mcchanshiftupenable){
        //
        // A7 mode is enabled, bits 6:0 are not used for CH selection
        //
        shiftVal = 7;
      } else {
        //
        // A7 mode is disabled, bits 5:0 are not used in CH selection
        //
        shiftVal = 6;
      }
      RAS_DEBUG((1, "\nTranslateTAD: Tad Entry: %d", tad));
      RAS_DEBUG((1, "\nTranslateTAD: shiftVal: %d", shiftVal));
      RAS_DEBUG((1, "\nTranslateTAD: channel way interleave: %d, Sktays : %x \n",(tadWayness.Bits.tad_ch_way + 1), sktWays)); 
      
      if((tadWayness.Bits.tad_ch_way + 1) == 3) {
        //For 3-way ch interleave channel hashing has no effect
        //Apply mod ch_ways(3) to System Address 

        RAS_DEBUG((1, "\nTranslateTAD: 3 way interleave \n")); 
        chInterleave = (UINT8)((systemAddress >> 6) % (tadWayness.Bits.tad_ch_way + 1)); 
      } else {
        //get Ch Interleave index
        chInterleave = (UINT8)((systemAddress >> (shiftVal + tadWayness.Bits.tad_skt_way)) & 0x3);
        RAS_DEBUG((1, "\nTranslateTAD: channel index before hash: %d \n", chInterleave)); 

        if(haSysDefeature2.Bits.mcchanhashen == 1) {
          //If channel hashing enabled 
          //Apply hashing to Ch Interleave index, before applying Mod
          //XOR Bit 0 for 2-way
          //XOR bit 0-1 for 4 way
          //Hash_ChIdx[0] = ChIdx[0] ^ ((A12 ^ A14 ^ A16 ^ A18 ^ A20 ^ A22 ^A24 ^ A26))  
          //Hash_ChIdx[1] = ChIdx[1] ^ ((A13 ^ A15 ^ A17 ^ A19 ^ A21 ^ A23 ^A25 ^ A27))
          for(chXorBitPos=12; chXorBitPos<27; chXorBitPos += 2) {
            chInterleave ^= ((systemAddress >> chXorBitPos) & 0x3);
          }
          RAS_DEBUG((1, "\nTranslateTAD: channel index after hash: %d \n", chInterleave));
        }
        //Apply Mod ch_ways to index
        chInterleave %= (tadWayness.Bits.tad_ch_way + 1);  
        
      }
      
      switch (chInterleave) {
      case 0:
        TranslatedAddress->ChannelId = (UINT8)tadWayness.Bits.tad_ch_tgt0;
        break;
      case 1:
        TranslatedAddress->ChannelId = (UINT8)tadWayness.Bits.tad_ch_tgt1;
        break;
      case 2:
        TranslatedAddress->ChannelId = (UINT8)tadWayness.Bits.tad_ch_tgt2;
        break;
      case 3:
        TranslatedAddress->ChannelId = (UINT8)tadWayness.Bits.tad_ch_tgt3;
        break;
      }

      // if Failover occurs (TBD) use non failover channel.
      secCh = TranslatedAddress->ChannelId ^ (UINT8)(haSysDefeature.Bits.mirrormod);
      haChFailStsReg.Data = MemReadPciCfg(  NODE_TO_SKT(node), TranslatedAddress->MemoryControllerId, HACHFAILSTS_HA_CFG_REG);
      if ( ((haChFailStsReg.Bits.failch0 ==  1) && ( TranslatedAddress->ChannelId == 0)) ||
           ((haChFailStsReg.Bits.failch1 ==  1) && ( TranslatedAddress->ChannelId == 1)) ||
           ((haChFailStsReg.Bits.failch2 ==  1) && ( TranslatedAddress->ChannelId == 2)) ||
           ((haChFailStsReg.Bits.failch3 ==  1) && ( TranslatedAddress->ChannelId == 3)) )
        TranslatedAddress->ChannelId = secCh;

      //
      // Channel Address from Channel Offset
      //
      channel = TranslatedAddress->ChannelId;
      RAS_DEBUG((1, "\nTranslateTAD: tad_ch_tgt(TranslatedAddress->ChannelId) : %lx, chInterleave: %x", TranslatedAddress->ChannelId, chInterleave));

      imcTADChnlIlvOffset.Data = MemReadPciCfg(  node, channel, TADCHNILVOFFSET_0_MCDDC_DESC_REG + tad*4);
      chOffset = (UINT64)imcTADChnlIlvOffset.Bits.tad_offset << 26;

      //
      // Extract Low order bits
      //
      lowBits = (UINT8)(systemAddress & ((1 << shiftVal)-1));

      channelAddress = (systemAddress - chOffset);
      RAS_DEBUG((1, "\nTranslateTAD: channelAddress: %lx = systemAddress: %lx = chOffset: %x:%x", channelAddress, systemAddress, chOffset));
      channelAddress  >>= shiftVal;
      RAS_DEBUG((1, "\nTranslateTAD: channelAddress : %lx >>= shiftVal: %x", channelAddress, shiftVal));
      channelAddress  /= sktWaysXchWays;
      RAS_DEBUG((1, "\nTranslateTAD: channelAddress %lx /= sktWaysXchWays: %x", channelAddress, sktWaysXchWays));
      channelAddress  <<= shiftVal;
      RAS_DEBUG((1, "\nTranslateTAD: channelAddress : %lx <<=  shiftVal: %x", channelAddress, shiftVal));
      channelAddress  |= lowBits;
      RAS_DEBUG((1, "\nTranslateTAD: channelAddress : %lx |= lowBits: %x", channelAddress, lowBits));
      //
      // Compute channel address from physical address and channel offset
      //
      TranslatedAddress->ChannelAddress = channelAddress;
      RAS_DEBUG((1, "\nTranslateTAD: DDRCH: %0d, channelAddress: %x:%x", channel, (UINT32)(channelAddress >> 32), (UINT32)channelAddress));
    } // if match
  } // for

  if (match == TRUE) {
    return EFI_SUCCESS;
  } else {
    return EFI_INVALID_PARAMETER;
  }
} // TranslateTAD

/**

Routine Description:
   Determines NodeID that maps given system Address

Arguments:

  PTRANSLATED_ADDRESS  - ptr to Address Translation structure
  Following fields in TRANSLATED_ADDRESS structure is expected
  to be valid:
    SystemAddress

Modifies:
  Following fields in ADDRESS_TRANSATION structure are updated:
  HaNodeId
  SocketId
  MemoryControllerId

Returns:
  EFI_SUCCESS / Error code


--*/
EFI_STATUS
TranslateSAD (
  PTRANSLATED_ADDRESS TranslatedAddress
 )
{
  UINT64  systemAddress;
  UINT64  tempAddress;
  UINT32  limit;
  UINT8   sad;
  UINT8   interleaveListIndex;
  UINT8   haNodeId;
  UINT8   node;
  BOOLEAN isNodeActive;
  BOOLEAN match;
  UINT8   rule_enable;
  UINT8   dram_attribute;
  UINT8   interleave_mode;
  DRAM_RULE_0_CBOBC_MEMSAD_STRUCT       cboDramRule;
  INTERLEAVE_LIST_0_CBOBC_MEMSAD_STRUCT cboInterleaveList;

  //
  // Find matching DRAM SAD
  // (SAD rules same on all nodes)
  //
  systemAddress = TranslatedAddress->SystemAddress;
  match = FALSE;
  haNodeId = 0xFF;

  //
  // Initialize return values
  //
  TranslatedAddress->HaNodeId = (UINT8)-1;
  TranslatedAddress->SocketId = (UINT8)-1;
  TranslatedAddress->MemoryControllerId = (UINT8)-1;
  TranslatedAddress->Node = (UINT8)-1;

  //
  // Determine the first node that is populated
  //
  for(node = 0; node < MC_MAX_NODE; node++){
    IsMemNodeEnabled(node, &isNodeActive);
    if(isNodeActive){
      break;
    }
  }

  for (sad = 0; sad < SAD_RULES; sad++) {
    if (match == TRUE) break;

    cboDramRule.Data = MemReadPciCfg(NodeToSkt(node), 0, DRAM_RULE(sad));
    rule_enable     = (UINT8)cboDramRule.Bits.rule_enable;
    dram_attribute  = (UINT8)cboDramRule.Bits.attr;
    limit           = cboDramRule.Bits.limit;
    interleave_mode = (UINT8)cboDramRule.Bits.interleave_mode;

    //
    // Skip rule if not enabled, or attribute not DRAM
    //
    if ((rule_enable == 0) || (dram_attribute != 0) ) continue;

    //
    // Does address match current sad rule limt
    //
    // limit is 64MB granularity (bits 45:26)
    //
    tempAddress = systemAddress >> 26;

    if (tempAddress <= limit) {
      match = TRUE;

      if(cboDramRule.Bits.a7mode == 0){
        // system address bits low order bits 8,7,6 are used in calculating index
        interleaveListIndex = (UINT8)((systemAddress >> 6) & 0x7);
      } else {
        // In A7 Mode, low order address bits used for calculating index are [8:7, 9]
        interleaveListIndex = (UINT8)((((systemAddress >> 7) & 3) << 1) | ((systemAddress >> 9) & 1));
      }

      //
      // If XOR mode is enabled, then XOR with bits 18:16
      //
      if (interleave_mode == 0){
        interleaveListIndex ^= (UINT8)((systemAddress >> 16) & 7);
      }

      cboInterleaveList.Data = MemReadPciCfg(  NodeToSkt(node), 0, INTERLEAVE_LIST(sad));
      RAS_DEBUG((1, "\nTranslateSAD: interleaveListIndex: %d", interleaveListIndex));

      switch (interleaveListIndex) {
      case 0:
        haNodeId = (UINT8)cboInterleaveList.Bits.package0;
        break;
      case 1:
        haNodeId = (UINT8)cboInterleaveList.Bits.package1;
        break;
      case 2:
        haNodeId = (UINT8)cboInterleaveList.Bits.package2;
        break;
      case 3:
        haNodeId = (UINT8)cboInterleaveList.Bits.package3;
        break;
      case 4:
        haNodeId = (UINT8)cboInterleaveList.Bits.package4;
        break;
      case 5:
        haNodeId = (UINT8)cboInterleaveList.Bits.package5;
        break;
      case 6:
        haNodeId = (UINT8)cboInterleaveList.Bits.package6;
        break;
      case 7:
        haNodeId = (UINT8)cboInterleaveList.Bits.package7;
        break;
      } // switch
    } // if
  } // for

  if (match == TRUE) {
    TranslatedAddress->HaNodeId = haNodeId;
    TranslatedAddress->SocketId = HaNodeIdToSkt(haNodeId);
    TranslatedAddress->Node = HaNodeIdToNode(haNodeId);
    TranslatedAddress->MemoryControllerId = HaNodeIdToHa(haNodeId);
    RAS_DEBUG((1, "\nTranslateSAD: node: %d", TranslatedAddress->Node));
    RAS_DEBUG((1, "\nTranslateSAD: HaNodeId: %d", haNodeId));
    return EFI_SUCCESS;
  } else {
    return EFI_INVALID_PARAMETER;
  }
} // TranslateSAD

/**

Routine Description:
   Extract Bank address bits from Rank Address or
   Stuff bank address bits to form Rank Address based on flag

Arguments:
  UINT64*  RankAddress
  UINT8*  BankAddr
  UINT8   Open/Close page - 0 -> Open page, 1 -> Closed page
  UINT8   Opcode = EXTRACT_ADDR_BITS or STUFF_ADDR_BITS

Modifies:
  *BankAddr if Opcode = EXTRACT_ADDR_BITS
  *RankAddress if Opcode = STUFF_ADDR_BITS

Returns:
  EFI_SUCCESS / Error code


--*/
static
EFI_STATUS
ExtractOrStuffBankGroupBits(
    UINT64  *RankAddress,
    UINT8   *BankGroupBits,
    UINT8   BankXorEn,
    UINT8   ClosedPageMode,
    UINT8   Opcode,
    UINT8   SecMode
 )
{
  UINT8   bankGroup;
  UINT64  rankAddr;
  UINT8   maxBankBits = 2;
  UINT8   i;
  UINT8   bitPos;
  UINT8   bitXorPos;
 
  RAS_DEBUG((1, "\n\nExtracting RankAddress Closed PageMode : %x, BankXor : %x, Sec Mode : %x \n",ClosedPageMode, BankXorEn, SecMode ));
  if(Opcode == EXTRACT_ADDR_BITS){
    rankAddr = *RankAddress;
    bankGroup = 0;
#ifdef WIN32
    RAS_DEBUG((1, "\n\nExtracting Bank Address: b00 | b01 | b02\n"));
    RAS_DEBUG((1, "Rank Address:   %x\n", rankAddr));
#endif // WIN32
  } else {
    rankAddr = *RankAddress;
    bankGroup = *BankGroupBits;
#ifdef WIN32
    RAS_DEBUG((1, "\n\nStuffing Bank Address:   b01 | b01 | b02\n"));
    RAS_DEBUG((1, "Rank Address:   %x\n", rankAddr));
#endif // WIN32
  }

  //
  // Extract / stuff bank addr bits
  //
  for(i = 0; i < maxBankBits; i++){

    if(ClosedPageMode){
      bitPos = ClosedPageAddrMapForBGBits1_0_DDR4[i];
      bitXorPos =  ClosedPageAddrMapForBGXorBits1_0_DDR4[i];
    } else {
      bitPos = OpenPageAddrMapForBGBits1_0_DDR4[SecMode][i];
      bitXorPos = OpenPageAddrMapForBGXorBits1_0_DDR4[SecMode][i];
    }

#ifdef WIN32
    RAS_DEBUG((1, "R%02d | ", bitPos));
#endif // WIN32

    if(bitPos == 0xFF){
      return EFI_INVALID_PARAMETER;
    }

    if(Opcode == EXTRACT_ADDR_BITS){
      bankGroup |= ((rankAddr >> bitPos) & 1) << i;
      RAS_DEBUG((1, "Extracting RankAddress : bankGroup : %d, rankAddr bitpos : %d, BG BitPos : %d \n",bankGroup, bitPos, i ));
      if(BankXorEn) {
        bankGroup ^= ((rankAddr >> bitXorPos) & 1) << i;
      RAS_DEBUG((1, "Extracting RankAddress : bankGroup : %d, rankAddr bitXorPos : %d, BG BitPos : %d \n",bankGroup, bitXorPos, i ));
      }
    } else {
      if(BankXorEn) {
        bankGroup ^= ((rankAddr >> bitXorPos) & 1) << i;
        RAS_DEBUG((1, "Extracting RankAddress : rankAddr : %d, rankAddr bitXorpos : %d, BG BitPos : %d \n",rankAddr, bitXorPos, i ));
      }     
      rankAddr |= (((UINT64)bankGroup >> i) & 1) << bitPos;
      RAS_DEBUG((1, "Extracting RankAddress : rankAddr : %d, rankAddr bitpos : %d, BG BitPos : %d \n",rankAddr, bitPos, i ));
    }
  }

  if(Opcode == EXTRACT_ADDR_BITS){
    *BankGroupBits = bankGroup;
  } else {
    *RankAddress |= rankAddr;
  }
  return EFI_SUCCESS;
}

/**

Routine Description:
   Extract Chip Select address bits from Rank Address or
   Stuff Chip Select address bits to form Rank Address based on flag

Arguments:
  UINT64*  RankAddress
  UINT8*  ChipSelectAddr
  UINT8   Open/Close page - 0 -> Open page, 1 -> Closed page
  UINT8   LockStepMode - 0 -> Independent mode, 1 -> Lock Step mode
  UINT8   MaxRowBits
  UINT8   Opcode = EXTRACT_ADDR_BITS or STUFF_ADDR_BITS

Modifies:
  *ChipSelectBits if Opcode = EXTRACT_ADDR_BITS
  *RankAddress if Opcode = STUFF_ADDR_BITS

Returns:
  EFI_SUCCESS / Error code


--*/
static
EFI_STATUS
ExtractOrStuffChipSelectBits(
    UINT64  *RankAddress,
    UINT8   *ChipSelectBits,
    UINT8   ClosedPageMode,
    UINT8   LockStepMode,
    UINT8   MaxRowBits,
    UINT8   Opcode
 )
{
  UINT8   chipSelect;
  UINT64  rankAddr;
  UINT8   maxCSBits = 3;
  UINT8   i;
  UINT8   bitPos;

  if(Opcode == EXTRACT_ADDR_BITS){
    rankAddr = *RankAddress;
    chipSelect = 0;
#ifdef WIN32
    RAS_DEBUG((1, "\n\nExtracting Chip Select Address: cs00 | cs01 | cs02\n"));
    RAS_DEBUG((1, "Chip Select Address:            "));
#endif // WIN32
  } else {
    rankAddr = 0;
    chipSelect = *ChipSelectBits;
#ifdef WIN32
    RAS_DEBUG((1, "\n\nStuffing Chip Select Address: cs00 | cs01 | cs02\n"));
    RAS_DEBUG((1, "Chip Select:            "));
#endif // WIN32
  }

  //
  // Extract / stuff chip select addr bits
  //
  for(i = 0; i < maxCSBits; i++){

    bitPos = AddrMapForCSBits2_0_DDR4[LockStepMode][MaxRowBits - 14][i]; 

#ifdef WIN32
    RAS_DEBUG((1, "R%02d | ", bitPos));
#endif // WIN32

    if(bitPos == 0xFF){
      return EFI_INVALID_PARAMETER;
    }

    if(Opcode == EXTRACT_ADDR_BITS){
      chipSelect |= ((rankAddr >> bitPos) & 1) << i;
    } else {
      rankAddr |= (((UINT64)chipSelect >> i) & 1) << bitPos;
    }
  }

  if(Opcode == EXTRACT_ADDR_BITS){
    *ChipSelectBits = chipSelect;
  } else {
    *RankAddress |= rankAddr;
  }
  return EFI_SUCCESS;
}

/**

Routine Description:
   Extract Bank address bits from Rank Address or
   Stuff bank address bits to form Rank Address based on flag

Arguments:
  UINT64*  RankAddress
  UINT8*  BankAddr
  UINT8   Open/Close page - 0 -> Open page, 1 -> Closed page
  UINT8   Opcode = EXTRACT_ADDR_BITS or STUFF_ADDR_BITS

Modifies:
  *BankAddr if Opcode = EXTRACT_ADDR_BITS
  *RankAddress if Opcode = STUFF_ADDR_BITS

Returns:
  EFI_SUCCESS / Error code


--*/
static
EFI_STATUS
ExtractOrStuffBankBits(
    UINT64  *RankAddress,
    UINT8   *BankAddrBits,
    UINT8   BankXorEn,
    UINT8   ClosedPageMode,
    UINT8   Opcode,
    UINT8   SecMode
)
{

  UINT8   bankAddr;
  UINT64  rankAddr;
  UINT8   maxBankBits = 2;
  UINT8   i;
  UINT8   bitPos;
  UINT8   bitXorPos;


  if(Opcode == EXTRACT_ADDR_BITS){
    rankAddr = *RankAddress;
    bankAddr = 0;
#ifdef WIN32
    RAS_DEBUG((1, "\n\nExtracting Bank Address: b00 | b01 | b02\n"));
    RAS_DEBUG((1, "Rank Address:   %x\n", rankAddr));
#endif // WIN32
  } else {
    rankAddr = *RankAddress;
    bankAddr = *BankAddrBits;

#ifdef WIN32
    RAS_DEBUG((1, "\n\nStuffing Bank Address:   b01 | b01 | b02\n"));
    RAS_DEBUG((1, "Rank Address:   %x\n", rankAddr));
#endif // WIN32

  }

  if(mDdr4Dimm) {
    maxBankBits = MAX_BANK_BITS_SUPPORTED_DDR4;
  } else {
    maxBankBits = MAX_BANK_BITS_SUPPORTED;
  }

  //
  // Extract / stuff bank addr bits
  //
  for(i = 0; i < maxBankBits; i++){

    if(ClosedPageMode){
      if(mDdr4Dimm) {
        bitPos = ClosedPageAddrMapForBankBits1_0_DDR4[i];
        bitXorPos = ClosedPageAddrMapForBankXorBits1_0_DDR4[i];
      } else {
        bitPos = ClosedPageAddrMapForBankBits2_0[i];
        bitXorPos = ClosedPageAddrMapForBankXorBits2_0[i];
      }
    } else {
      if(mDdr4Dimm)  {
        bitPos = OpenPageAddrMapForBankBits1_0_DDR4[SecMode][i];
        bitXorPos = OpenPageAddrMapForBankXorBits1_0_DDR4[SecMode][i];
      } else {
        bitPos = OpenPageAddrMapForBankBits2_0[i];
        bitXorPos = OpenPageAddrMapForBankXorBits2_0[i];
      }
    }

#ifdef WIN32
    RAS_DEBUG((1, "R%02d | ", bitPos));
#endif // WIN32

    if(bitPos == 0xFF){
      return EFI_INVALID_PARAMETER;
    }

    if(Opcode == EXTRACT_ADDR_BITS){
      bankAddr |= ((rankAddr >> bitPos) & 1) << i;
      if(BankXorEn) {
        bankAddr ^= ((rankAddr >> bitXorPos) & 1) << i;  
      }
    } else {
      if (BankXorEn) {
        bankAddr ^= ((rankAddr >> bitXorPos) & 1) << i;
      }
      rankAddr |= (((UINT64)bankAddr >> i) & 1) << bitPos;
    }
  }

  if(Opcode == EXTRACT_ADDR_BITS){
    *BankAddrBits = bankAddr;
  } else {
    *RankAddress |= rankAddr;
  }
  return EFI_SUCCESS;
}

/**

Routine Description:
   Extract Row address bits from Rank Address or
   Stuff row address bits in Rank Address based on Opcode

Arguments:
  UINT64* RankAddress
  UINT32* RowAddr
  UINT8   Open/Close page - 0 -> Open page, 1 -> Closed page
  UINT8   LockStepMode - 0 -> Independent mode, 1 -> Lock Step mode
  UINT8   MaxRowBits
  UINT8   MaxColBits
  UINT8   BankXorEnable - 0 -> Bank XOR disabled, 1 -> Bank XOR enabled
  UINT8   BankAddrBits - used only if BankXorEnable = 1
  UINT8   Opcode = EXTRACT_ADDR_BITS or STUFF_ADDR_BITS

Modifies:
  *RowAddr if Opcode = EXTRACT_ADDR_BITS
  *RankAddress if Opcode = STUFF_ADDR_BITS

Returns:
  EFI_SUCCESS / Error code


--*/
static
EFI_STATUS
ExtractOrStuffRowBits(
    UINT64  *RankAddress,
    UINT32  *RowAddrBits,
    UINT8   ClosedPageMode,
    UINT8   LockStepMode,
    UINT8   MaxRowBits,
    UINT8   MaxColBits,
    UINT8   Opcode,
    UINT8   SecMode
 )
{
  UINT32  rowAddr;
  UINT64  rankAddr;
  UINT8   i;
  UINT8   bitPos;

  //
  // If bank xor is enabled, XOR bank bits with Rank Address bits[21:19]
  // In secondary mode 
  //
  if(Opcode == EXTRACT_ADDR_BITS){
    rankAddr = *RankAddress;
    rowAddr  = 0;
#ifdef WIN32
    RAS_DEBUG((1, "\n\nExtracting Row Address:  r00 | r01 | r02 | r03 | r04 | r05 | r06 | r07 | r08 | r09 | r10 | r11 | r12 | r13 | r14 | r15 | r16 | r17\n"));
    RAS_DEBUG((1, "Rank Address:   %x\n", rankAddr));
#endif // WIN32

  } else {
    rankAddr = 0;
    rowAddr  = *RowAddrBits;
#ifdef WIN32
    RAS_DEBUG((1, "\n\nStuffing Row Address:    r00 | r01 | r02 | r03 | r04 | r05 | r06 | r07 | r08 | r09 | r10 | r11 | r12 | r13 | r14 | r15 | r16 | r17\n"));
    RAS_DEBUG((1, "Rank Address:   %x\n", rankAddr));
#endif // WIN32
  }

  //
  // Extract or stuff Row Addr Bits
  //
  for(i = 0; i < MaxRowBits; i++){
    if(i < MAX_ROW_BITS_FIXED_MAP){
      if(ClosedPageMode){
        //
        // Closed page Mode
        // Get Row bits [11:0] using Closed page fixed map
        //
        if(mDdr4Dimm)
          bitPos = ClosedPageAddrMapForRowBits11_0_DDR4[i];
        else
        bitPos = ClosedPageAddrMapForRowBits11_0[i];
      } else {
        //
        // Open Page mode
        // Get Row bits [11:0] using Open page fixed map
        //
        if(mDdr4Dimm)  {
          bitPos = OpenPageAddrMapForRowBits11_0_DDR4[SecMode][i];
        } else
          bitPos = OpenPageAddrMapForRowBits11_0[i];
      }
    } else {
      //
      // Mapping for Row bits [17:12] is same for Open / Closed page
      // but dependent on # of column address bits and lockstep/independent mode
      //
      if(mDdr4Dimm)
        bitPos = AddrMapForRowBits17_12_DDR4[LockStepMode][MaxColBits - MAX_COL_BITS_FIXED_MAP][i - MAX_ROW_BITS_FIXED_MAP];
      else
        bitPos = AddrMapForRowBits17_12[LockStepMode][MaxColBits - MAX_COL_BITS_FIXED_MAP][i - MAX_ROW_BITS_FIXED_MAP];
    } // if i < MAX_ROW_BITS_FIXED_MAP

#ifdef WIN32
    RAS_DEBUG((1, "R%02d | ", bitPos));
#endif // WIN32

    if(bitPos == 0xFF){
      return EFI_INVALID_PARAMETER;
    }

    if(Opcode == EXTRACT_ADDR_BITS){
      rowAddr |= ((rankAddr >> bitPos) & 1) << i;
    } else {
      rankAddr |= (((UINT64)rowAddr >> i) & 1) << bitPos;
    }
  } // row bit loop

  if(Opcode == EXTRACT_ADDR_BITS){
    *RowAddrBits = rowAddr;
  } else {
    *RankAddress |= rankAddr;
  }

  return EFI_SUCCESS;
}

/**

Routine Description:
   Extract Col address bits from Rank Address or
   Stuff Col address bits in Rank Address based on Opcode

Arguments:
  UINT64* RankAddress
  UINT16* ColAddr
  UINT8   Open/Close page - 0 -> Open page, 1 -> Closed page
  UINT8   LockStepMode - 0 -> Independent mode, 1 -> Lock Step mode
  UINT8   MaxRowBits
  UINT8   MaxColBits
  UINT8   Opcode = EXTRACT_ADDR_BITS or STUFF_ADDR_BITS

Modifies:
  *ColAddr if Opcode = EXTRACT_ADDR_BITS
  *RankAddress if Opcode = STUFF_ADDR_BITS

Returns:
  EFI_SUCCESS / Error code


--*/
static
EFI_STATUS
ExtractOrStuffColBits(
    UINT64  *RankAddress,
    UINT32  *ColAddrBits,
    UINT8   ClosedPageMode,
    UINT8   LockStepMode,
    UINT8   MaxRowBits,
    UINT8   MaxColBits,
    UINT8   Opcode,
    UINT8   SecMode
 )
{
  UINT32  colAddr;
  UINT64  rankAddr;
  UINT8   i;
  UINT8   bitPos =0xFF;

  if(Opcode == EXTRACT_ADDR_BITS){
    rankAddr = *RankAddress;
    colAddr  = 0;
#ifdef WIN32
    RAS_DEBUG((1, "\n\nExtracting Col Address:  c00 | c01 | c02 | c03 | c04 | c05 | c06 | c07 | c08 | c09 | c10 | c11\n"));
    RAS_DEBUG((1, "Rank Address:   %x\n", rankAddr));
#endif // WIN32
  } else {
    rankAddr = 0;
    colAddr  = *ColAddrBits;
#ifdef WIN32
    RAS_DEBUG((1, "\n\nStuffing Col Address:    c00 | c01 | c02 | c03 | c04 | c05 | c06 | c07 | c08 | c09 | c10 | c11\n"));
    RAS_DEBUG((1, "Rank Address:   %x\n", rankAddr));
#endif // WIN32
  }

  //
  // Extract Col Bits
  //
  for(i = 0; i < MaxColBits; i++){
    if(i < MAX_COL_BITS_FIXED_MAP){
      if(ClosedPageMode){
        //
        // Closed page Mode
        // Get Col bits [9:0]using Closed page fixed map; depends on independent / lock-step mode
        //
        if(mDdr4Dimm)
          bitPos = ClosedPageAddrMapForColBits9_0_DDR4[LockStepMode][i];
        else
          bitPos = ClosedPageAddrMapForColBits9_0[LockStepMode][i];
      } else {
        //
        // Open Page mode
        // Get Col bits [11:0] using Open page fixed map; depends on independent/lock-step mode
        //
        if(mDdr4Dimm) {
          bitPos = OpenPageAddrMapForColBits9_0_DDR4[SecMode][LockStepMode][i];
        } else
          bitPos = OpenPageAddrMapForColBits9_0[LockStepMode][i];
      }
    } else {
      //
      // Mapping for Col bits [11:10] is same for Open / Closed page
      // but dependent on # of Row address bits and lockstep/independent mode
      //
      if(mDdr4Dimm == FALSE)
        bitPos = AddrMapForColBits11_10[LockStepMode][MaxRowBits - MAX_ROW_BITS_FIXED_MAP][i - MAX_COL_BITS_FIXED_MAP];

    } // if i < MAX_COL_BITS_FIXED_MAP

#ifdef WIN32
    RAS_DEBUG((1, "R%02d | ", bitPos));
#endif // WIN32

    if(bitPos == 0xFF){
      MaxColBits++;
      if(MaxColBits > 14)
        return EFI_INVALID_PARAMETER;;
      continue;
    }

    if(Opcode == EXTRACT_ADDR_BITS){
      colAddr |= ((rankAddr >> bitPos) & 1) << i;
    } else {
      rankAddr |= (((UINT64)colAddr >> i) & 1) << bitPos;
    }
    if(Opcode == EXTRACT_ADDR_BITS){
      // #c10 is the autoprecharge bit and will always be set in closed page
      // #c10 is the autoprecharge bit and wont be used in open page mode
      if(ClosedPageMode)
        colAddr |=  1 << 10;
       else
        colAddr &= (~BIT10);
     }
  } // col bit loop

  if(Opcode == EXTRACT_ADDR_BITS){
    *ColAddrBits = colAddr;
  } else {
    *RankAddress |= rankAddr;
  }

  return EFI_SUCCESS;
}

/**

Routine Description:
   Translate Rank Address to Row/Dimm/Col

Arguments:

  PTRANSLATED_ADDRESS  - ptr to Address Translation structure
  Following fields in TRANSLATED_ADDRESS structure is expected
  to be valid:
    RankAddress
    SocketId
    MemoryControllerId
    ChannelId
    DimmId

Modifies:
  Following fields in ADDRESS_TRANSATION structure are updated:
  Row
  Col
  Bank

Returns:
  EFI_SUCCESS / Error code


--*/
EFI_STATUS
TranslateRankAddress (
  PTRANSLATED_ADDRESS TranslatedAddress
 )
{
  EFI_STATUS  status = EFI_SUCCESS;
  UINT8       maxRowBits;
  UINT8       maxColBits;
  UINT8       node;
  UINT32      rowAddrBits;
  UINT32      colAddrBits;
  UINT8       bankAddrBits;
  UINT8       bankGroupBits =0;
  UINT8       chipSelectBits =0;
  UINT8       closedPageEn;
  UINT8       lockStepEn;
  UINT8       bankXorEn;
  DIMMMTR_0_MCDDC_DESC_BDX_STRUCT dimmMtrReg;
  MCMTR_MC_MAIN_HSX_BDX_STRUCT    mcmtrReg;
  UINT8       SecMode = 0;
  AMAP_MCDDC_DESC_STRUCT altAddMap;

  node  = TranslatedAddress->Node;
  //
  // Init. return values
  //
  TranslatedAddress->Row = (UINT32)-1;
  TranslatedAddress->Col = (UINT32)-1;
  TranslatedAddress->Bank = (UINT8)-1;

  //
  // Get Page Mode / lock-step config from MCMTR register
  //
  mcmtrReg.Data = MemReadPciCfg(node, PARAM_UNUSED, MCMTR_MC_MAIN_REG);

  //
  // Get the max row/col/bank bits from DIMMMTR register
  //
  dimmMtrReg.Data = MemReadPciCfg(node, TranslatedAddress->ChannelId, DIMMMTR_0_MCDDC_DESC_REG + TranslatedAddress->DimmSlot * 4);

  if(!dimmMtrReg.Bits.dimm_pop){
    return EFI_INVALID_PARAMETER;
  }


  maxRowBits    = (UINT8)dimmMtrReg.Bits.ra_width + 12;
  maxColBits    = (UINT8)dimmMtrReg.Bits.ca_width + 10;
  closedPageEn  = (UINT8)mcmtrReg.Bits.close_pg;
  lockStepEn    = (UINT8)mcmtrReg.Bits.ls_en;
  bankXorEn     = (UINT8)mcmtrReg.Bits.bank_xor_enable;
  RAS_DEBUG((1, "\nTranslateRankAddress: maxRowBits:%d, maxColBits:%d, closedPageEn:%d, lockStepEn:%d, bankXorEn:%d",
    maxRowBits, maxColBits, closedPageEn, lockStepEn, bankXorEn));

#ifndef WIN32
  //
  // Check for unsupported configs
  // Row = 12, unsupported
  // Row = 13, col = 11, unsupported
  //
  if(maxRowBits == 12 || (maxRowBits == 13 && maxColBits == 11)){
    return EFI_UNSUPPORTED;
  }
#endif // WIN32

  altAddMap.Data = MemReadPciCfg(TranslatedAddress->Node, TranslatedAddress->ChannelId, AMAP_MCDDC_DESC_REG);
  SecMode = (UINT8)altAddMap.Bits.hsxplus;
  
  status |= ExtractOrStuffRowBits(&TranslatedAddress->RankAddress, &rowAddrBits,
              closedPageEn, lockStepEn, maxRowBits, maxColBits, EXTRACT_ADDR_BITS, SecMode);
  //
  // Extract row/col/bank bits from Rank Address
  //
  status =  ExtractOrStuffBankBits(&TranslatedAddress->RankAddress, &bankAddrBits, 
               bankXorEn, closedPageEn, EXTRACT_ADDR_BITS, SecMode);
  if(mDdr4Dimm) {
    status =  ExtractOrStuffBankGroupBits(&TranslatedAddress->RankAddress, &bankGroupBits,
                 bankXorEn, closedPageEn, EXTRACT_ADDR_BITS, SecMode);
  }

  status |= ExtractOrStuffColBits(&TranslatedAddress->RankAddress, &colAddrBits,
              closedPageEn, lockStepEn, maxRowBits, maxColBits, EXTRACT_ADDR_BITS, SecMode);

  if(mDdr4Dimm)
    status =  ExtractOrStuffChipSelectBits(&TranslatedAddress->RankAddress, &chipSelectBits, closedPageEn, lockStepEn, maxRowBits, EXTRACT_ADDR_BITS);

  RAS_DEBUG((1, "\nTranslateRankAddress: rowAddrBits:%x, colAddrBits:%x ", rowAddrBits, colAddrBits ));
  RAS_DEBUG((1, "\n bankAddrBits:%x, bankGroupBits : %x \n", bankAddrBits, bankGroupBits));
  
  if(status == EFI_SUCCESS){
    TranslatedAddress->Row  = rowAddrBits;
    TranslatedAddress->Col  = colAddrBits;
    TranslatedAddress->Bank = bankAddrBits;
    if (mDdr4Dimm )  {
      TranslatedAddress->BankGroup = bankGroupBits;
      TranslatedAddress->ChipSelect = chipSelectBits;
    } else  {
      TranslatedAddress->BankGroup  = (UINT8)-1;
    }
  }

  return status;
}

/**

Routine Description:
   Translate System Address to DIMM Address

Arguments:
    SystemAddress
    SocketId ptr
    MemoryControllerId ptr
    ChannelId ptr
    DimmSlot ptr
    DimmRank ptr
    Row ptr
    Col ptr
    Bank ptr

Modifies:
    SocketId
    MemoryControllerId
    ChannelId
    DimmSlot
    DimmRank
    Row
    Col
    Bank

Returns:
  EFI_SUCCESS / Error code


--*/
EFI_STATUS
EFIAPI
SystemAddressToDimmAddress (
        IN  UINT64  SystemAddress,
        OUT UINT8*  SocketId,
        OUT UINT8*  MemoryControllerId,
        OUT UINT8*  ChannelId,
        OUT UINT8*  DimmSlot,
        OUT UINT8*  DimmRank,
        OUT UINT32* Row,
        OUT UINT32* Col,
        OUT UINT8*  Bank,
        OUT UINT8*  BankGroup,
        OUT UINT8*  ChipSelect
 )
{
  EFI_STATUS  status = EFI_SUCCESS;
  TRANSLATED_ADDRESS  fwdTranslatedAddress;
  //
  // Make sure the system address is cache-line aligned
  // clear bits 5:0
  //
  fwdTranslatedAddress.SystemAddress = SystemAddress & (~(UINT64)0x3F);
  fwdTranslatedAddress.SocketId = 0;
  fwdTranslatedAddress.MemoryControllerId = 0;
  fwdTranslatedAddress.ChannelId = 0;
  fwdTranslatedAddress.DimmSlot = 0;
  fwdTranslatedAddress.DimmRank = 0;
  fwdTranslatedAddress.Row = 0;
  fwdTranslatedAddress.Col = 0;
  fwdTranslatedAddress.Bank = 0;
  fwdTranslatedAddress.BankGroup= 0;
  fwdTranslatedAddress.ChipSelect = 0;

  RAS_DEBUG((1, "\n SystemAddressToDimmAddress: SystemAddress: %lx \n\n", fwdTranslatedAddress.SystemAddress));

  status = TranslateSAD(&fwdTranslatedAddress);
  if(!status){
    status = TranslateTAD(&fwdTranslatedAddress);
  }

  if (fwdTranslatedAddress.Node < MAX_MEM_NODE) {
  mDdr4Dimm = (mSystemMemoryMap->NodeInfo[fwdTranslatedAddress.Node].DramType == SPD_TYPE_DDR4)? 1 :0;
  } else {
    mDdr4Dimm = 0;
  }

  if(!status){
    status = TranslateRIR(&fwdTranslatedAddress);
  }
  if(!status){
    status = TranslateRankAddress(&fwdTranslatedAddress);
  }

  //
  // Update return values
  //
  *SocketId           = fwdTranslatedAddress.SocketId;
  *MemoryControllerId = fwdTranslatedAddress.MemoryControllerId;
  *ChannelId          = fwdTranslatedAddress.ChannelId;
  *DimmSlot           = fwdTranslatedAddress.DimmSlot;
  *DimmRank           = fwdTranslatedAddress.DimmRank;
  *Row                = fwdTranslatedAddress.Row;
  *Col                = fwdTranslatedAddress.Col;
  *Bank               = fwdTranslatedAddress.Bank;
  *BankGroup          = fwdTranslatedAddress.BankGroup;
  *ChipSelect         = fwdTranslatedAddress.ChipSelect;

  RAS_DEBUG((1, "\n SystemAddressToDimmAddress conversion (Fwd translation) results \n:"));
        RAS_DEBUG((1,  "\n SocketId = %x \n", *SocketId));
        RAS_DEBUG((1,  "MemoryControllerId = %x \n", *MemoryControllerId));
        RAS_DEBUG((1,  "ChannelId = %x \n", *ChannelId));
        RAS_DEBUG((1,  "DimmSlot = %x \n", *DimmSlot));
        RAS_DEBUG((1,  "DimmRank= %x \n", *DimmRank));
        RAS_DEBUG((1,  "Row = %x \n", *Row));
        RAS_DEBUG((1,  "Col = %x \n", *Col));
        RAS_DEBUG((1,  "Bank = %x \n", *Bank));
        RAS_DEBUG((1,  "BankGroup = %x \n", *BankGroup));
        RAS_DEBUG((1,  "ChipSelect = %x \n", *ChipSelect));

  return status;
}

/**

Routine Description:
   Maps given DIMM Address to Rank Address

Arguments:

  PTRANSLATED_ADDRESS  - ptr to Address Translation structure
  Following fields in TRANSLATED_ADDRESS structure is expected
  to be valid:
    SocketId
    MemoryControllerId
    ChannelId
    DimmSlot
    DimmRank
    Row
    Col
    Bank

Modifies:
  Following fields in ADDRESS_TRANSATION structure are updated:
  RankAddress
  PhysicalRankId

Returns:
  EFI_SUCCESS / Error code


--*/
EFI_STATUS
MapDimmAddressToRankAddress (
    PTRANSLATED_ADDRESS TranslatedAddress
 )
{
  EFI_STATUS  status = EFI_SUCCESS;
  UINT8       maxRowBits;
  UINT8       maxColBits;
  UINT8       node;
  UINT8       closedPageEn;
  UINT8       lockStepEn;
  UINT8       bankXorEn;
  UINT64      rankAddr;
  DIMMMTR_0_MCDDC_DESC_BDX_STRUCT     dimmMtrReg;
  MCMTR_MC_MAIN_HSX_BDX_STRUCT        mcmtrReg;
  UINT8       SecMode;
  AMAP_MCDDC_DESC_STRUCT altAddMap;

  TranslatedAddress->Node = SktHaToNode(TranslatedAddress->SocketId, TranslatedAddress->MemoryControllerId);
  node  = TranslatedAddress->Node;
  if(node >= MAX_MEM_NODE){
    ASSERT_EFI_ERROR(EFI_INVALID_PARAMETER);
  }
  //
  // Init. return values
  //
  TranslatedAddress->RankAddress = (UINT64)-1;
  TranslatedAddress->PhysicalRankId = TranslatedAddress->DimmSlot * MAX_RANK_DIMM + TranslatedAddress->DimmRank;

  //
  // Get Page Mode / lock-step config from MCMTR register
  //
  mcmtrReg.Data = MemReadPciCfg(node, PARAM_UNUSED, MCMTR_MC_MAIN_REG);

  //
  // Get the max row/col/bank bits from DIMMMTR register
  //
  dimmMtrReg.Data = MemReadPciCfg(TranslatedAddress->Node, TranslatedAddress->ChannelId, DIMMMTR_0_MCDDC_DESC_REG + TranslatedAddress->DimmSlot * 4);

  if(!dimmMtrReg.Bits.dimm_pop){
    return EFI_INVALID_PARAMETER;
  }

  maxRowBits    = (UINT8)dimmMtrReg.Bits.ra_width + 12;
  maxColBits    = (UINT8)dimmMtrReg.Bits.ca_width + 10;
  closedPageEn  = (UINT8)mcmtrReg.Bits.close_pg;
  lockStepEn    = (UINT8)mcmtrReg.Bits.ls_en;
  bankXorEn     = (UINT8)mcmtrReg.Bits.bank_xor_enable;
  RAS_DEBUG((1, "\nMapDimmAddressToRankAddress: maxRowBits:%d, maxColBits:%d, closedPageEn:%d, lockStepEn:%d, bankXorEn%d ",
      maxRowBits, maxColBits, closedPageEn, lockStepEn, bankXorEn));

#ifndef WIN32
  //
  // Check for unsupported configs
  // Row = 12, unsupported
  // Row = 13, col = 11, unsupported
  //
  if(maxRowBits == 12 || (maxRowBits == 13 && maxColBits == 11)){
    return EFI_UNSUPPORTED;
  }
#endif // WIN32

  altAddMap.Data = MemReadPciCfg(TranslatedAddress->Node, TranslatedAddress->ChannelId, AMAP_MCDDC_DESC_REG);
  SecMode = (UINT8)altAddMap.Bits.hsxplus;
  
  //
  // Map row/col/bank bits to Rank Address
  //
  rankAddr = 0;
  status |= ExtractOrStuffRowBits(&rankAddr, &TranslatedAddress->Row,
              closedPageEn, lockStepEn, maxRowBits, maxColBits, STUFF_ADDR_BITS, SecMode);
  
  status =  ExtractOrStuffBankBits(&rankAddr, &TranslatedAddress->Bank,
               bankXorEn, closedPageEn, STUFF_ADDR_BITS,SecMode);
  if(mDdr4Dimm) {
    status =  ExtractOrStuffBankGroupBits(&rankAddr, &TranslatedAddress->BankGroup, 
                 bankXorEn,  closedPageEn, STUFF_ADDR_BITS, SecMode);
  }
  status |= ExtractOrStuffColBits(&rankAddr, &TranslatedAddress->Col,
              closedPageEn, lockStepEn, maxRowBits, maxColBits, STUFF_ADDR_BITS,SecMode);
  RAS_DEBUG((1, "\nMapDimmAddressToRankAddress - RankAddress = %08x:%08x\n", (UINT32)(rankAddr >> 32), (UINT32)rankAddr));

  if(mDdr4Dimm)
    status =  ExtractOrStuffChipSelectBits(&rankAddr, &TranslatedAddress->ChipSelect, closedPageEn, lockStepEn,  maxRowBits, STUFF_ADDR_BITS);


  if(status == EFI_SUCCESS){
    TranslatedAddress->RankAddress = rankAddr;
  }

  return status;
}

/**

Routine Description:
   Maps Rank Address to Channel Address

Arguments:

  PTRANSLATED_ADDRESS  - ptr to Address Translation structure
  Following fields in TRANSLATED_ADDRESS structure is expected
  to be valid:
    SocketId
    MemoryControllerId
    ChannelId
    RankAddress
    PhysicalRankId

Modifies:
  Following fields in ADDRESS_TRANSATION structure are updated:
  ChannelAddress

Returns:
  EFI_SUCCESS / Error code


--*/
EFI_STATUS
MapRankAddressToChannelAddress (
    PTRANSLATED_ADDRESS TranslatedAddress
 )
{
  EFI_STATUS  status = EFI_SUCCESS;
  UINT8       node;
  UINT64      rankAddr;
  UINT64      channelAddr;
  UINT64      lowAddrBits;
  UINT32      rirLimit;
  UINT8       rirWays;
  UINT8       rirVal;
  UINT8       shiftVal;
  UINT8       matchFound;
  UINT8       rir;
  UINT8       index;
  UINT32  rirRnkTgt0;
  UINT32  rirOffset0;
  RIRWAYNESSLIMIT_0_MCDDC_DESC_BDX_STRUCT imcRIRWaynessLimitBdx;
  RIRILV0OFFSET_0_MCDDC_DESC_BDX_STRUCT   imcRIRIlv0OffsetBdx;
  RIRWAYNESSLIMIT_0_MCDDC_DESC_HSX_STRUCT imcRIRWaynessLimitHsx;
  RIRILV0OFFSET_0_MCDDC_DESC_HSX_STRUCT   imcRIRIlv0OffsetHsx;
  MCMTR_MC_MAIN_HSX_BDX_STRUCT            imcMcmtr;

  TranslatedAddress->Node = SktHaToNode(TranslatedAddress->SocketId, TranslatedAddress->MemoryControllerId);
  node  = TranslatedAddress->Node;
  rankAddr  = TranslatedAddress->RankAddress;

  //
  // Init. return values
  //
  TranslatedAddress->ChannelAddress = (UINT64)-1;

  //
  // Get Page Mode and determine # of low-order address bits
  // that are not touched by interleave
  //
  imcMcmtr.Data = MemReadPciCfg(node, PARAM_UNUSED, MCMTR_MC_MAIN_REG);
  TranslatedAddress->LockStepEn = (UINT8)imcMcmtr.Bits.ls_en;
  if(imcMcmtr.Bits.close_pg){
    shiftVal = 6;
  } else {
    shiftVal = 13;
  }

  RAS_DEBUG((1, "\nMapRankAddressToChannelAddress - shiftVal:%d, locksteEn:%d\n", shiftVal, TranslatedAddress->LockStepEn));

  //
  // Determine low address bits from RankAddress
  //
  lowAddrBits = rankAddr & ((1 << shiftVal)-1);

  matchFound = 0;
  for (rir = 0; rir < MAX_RIR; rir++) {
    if (matchFound == 1) break;

    if (mHost.var.common.cpuType == CPU_BDX) {
      imcRIRWaynessLimitBdx.Data = MemReadPciCfg( node, TranslatedAddress->ChannelId, RIRWAYNESSLIMIT_0_MCDDC_DESC_REG + (rir * 4));
      rirVal = (UINT8) imcRIRWaynessLimitBdx.Bits.rir_val;
      rirLimit = imcRIRWaynessLimitBdx.Bits.rir_limit;
      rirWays = (UINT8) imcRIRWaynessLimitBdx.Bits.rir_way;
    } else {
      imcRIRWaynessLimitHsx.Data = MemReadPciCfg( node, TranslatedAddress->ChannelId, RIRWAYNESSLIMIT_0_MCDDC_DESC_REG + (rir * 4));
      rirVal = (UINT8) imcRIRWaynessLimitHsx.Bits.rir_val;
      rirLimit = imcRIRWaynessLimitHsx.Bits.rir_limit;
      rirWays = (UINT8) imcRIRWaynessLimitHsx.Bits.rir_way;
    }

    if (rirVal == 0) continue;

    for(index = 0; index < (1 << rirWays); index++){
      //
      // Get target physical rank id & rank address
      //
      if (mHost.var.common.cpuType == CPU_BDX) {
        imcRIRIlv0OffsetBdx.Data = MemReadPciCfg(  node, TranslatedAddress->ChannelId,
                                  RIRILV0OFFSET_0_MCDDC_DESC_REG + (rir * 8 * 4) + (index * 4));
        rirRnkTgt0 = imcRIRIlv0OffsetBdx.Bits.rir_rnk_tgt0;
        rirOffset0 = imcRIRIlv0OffsetBdx.Bits.rir_offset0;
      } else {
        imcRIRIlv0OffsetHsx.Data = MemReadPciCfg(  node, TranslatedAddress->ChannelId,
                                RIRILV0OFFSET_0_MCDDC_DESC_REG + (rir * 8 * 4) + (index * 4));
        rirRnkTgt0 = imcRIRIlv0OffsetHsx.Bits.rir_rnk_tgt0;
        rirOffset0 = imcRIRIlv0OffsetHsx.Bits.rir_offset0;
      }

      if(rirRnkTgt0 == TranslatedAddress->PhysicalRankId){
        RAS_DEBUG((1, "\nMapRankAddressToChannelAddress - RIR Rule:%d, Target Idx:%d, RIR Offset:%x, Tgt Rnk:%d\n",
            rir, index, rirOffset0, rirRnkTgt0));
        //
        // Determine high order bits of CH address i.e, BITS[38:shiftVal] that could map this rank
        //
        channelAddr = ((((rankAddr >> shiftVal) + (rirOffset0 << (26-shiftVal))) * (1i64 << rirWays)) + index) << shiftVal;
        if((channelAddr >> 29) <= rirLimit){
          //
          // The approx. channel address is within the limits of this RIR entry.
          // This RIR entry maps the given Rank Address.
          matchFound = 1;
          break;  // exit from index loop
        }
      }
    } // index loop
  } // rir loop

  if(matchFound){
    //
    // Determine full channel address
    //
    channelAddr |= lowAddrBits;
    TranslatedAddress->ChannelAddress = channelAddr;
    RAS_DEBUG((1, "MapRankAddressToChannelAddress - ChannelAddress = %08x:%08x\n", (UINT32)(channelAddr >> 32), (UINT32)channelAddr));
 
  } else {
    status = EFI_INVALID_PARAMETER;
  }

  return status;
}

/**

Routine Description:
   Maps Channel Address to system Address

Arguments:

  PTRANSLATED_ADDRESS  - ptr to Address Translation structure
  Following fields in TRANSLATED_ADDRESS structure is expected
  to be valid:
    SocketId
    MemoryControllerId
    ChannelId
    ChannelAddress

Modifies:
  Following fields in ADDRESS_TRANSATION structure are updated:
  SystemAddress

Returns:
  EFI_SUCCESS / Error code


--*/
EFI_STATUS
MapChannelAddressToSystemAddress (
    PTRANSLATED_ADDRESS TranslatedAddress
 )
{
  EFI_STATUS  status = EFI_SUCCESS;
  UINT8       node;
  UINT64      channelAddr;
  UINT64      systemAddr;
  UINT64      missingBits;
  UINT64      matchMB;
  UINT64      matchLB;
  UINT64      lowAddrBits;
  UINT64      lowerBits;
  UINT64      andResult;
  UINT64      tolmLimit;
  UINT64      chOffset;
  UINT32      tadLimit;
  UINT32      sadLimit;
  UINT32      chn_idx_offset;
  UINT8       tad;
  UINT8       sad;
  UINT8       sktWays;
  UINT8       chWays;
  UINT8       tgtChIndex;
  UINT8       chInterleave;
  UINT8       chXorBitPos;
  UINT32      correctedChIndex = 0;
  UINT8       packageNum = 0;
  UINT8       targetCh;
  UINT8       targetHa;
  UINT8       matchFound;
  UINT8       shiftVal;
  UINT8       haNodeId;
  MCMTR_MC_MAIN_HSX_BDX_STRUCT                imcMcmtr;
  TADCHNILVOFFSET_0_MCDDC_DESC_HSX_BDX_STRUCT imcTADChnlIlvOffset;
  HASYSDEFEATURE2_HA_CFG_HSX_BDX_STRUCT       haSysDefeature2;
  HASYSDEFEATURE_HA_CFG_STRUCT            haSysDefeature;
  DRAM_RULE_0_CBOBC_MEMSAD_STRUCT         cboDramRule;
  TADWAYNESS_0_MC_MAIN_STRUCT             tadWayness;
  INTERLEAVE_LIST_0_CBOBC_MEMSAD_STRUCT   cboInterleaveList;
  TOLM_IIO_VTD_STRUCT                     tolm;

  TranslatedAddress->Node = SktHaToNode(TranslatedAddress->SocketId, TranslatedAddress->MemoryControllerId);
  node  = TranslatedAddress->Node;
  if(node >= MAX_MEM_NODE){
    ASSERT_EFI_ERROR(EFI_INVALID_PARAMETER);
  }
  channelAddr = TranslatedAddress->ChannelAddress;
  sktWays     = 0;
  systemAddr  = 0;
  targetCh    = 0xFF;
  targetHa    = 0xFF;
  cboDramRule.Data = 0;
  cboInterleaveList.Data = 0;

  //
  // Ha Node Id is derived from Skt ID and Memory controller Id
  // HaNodeId.bit[1:0] = SocketId.bit[1:0]
  // HaNodeId.bit[2] = MemoryControllerId.bit[0]
  // HaNodeId.bit[3] = SocketId.bit[2]
  haNodeId    = SktHaToHaNodeId(TranslatedAddress->SocketId, TranslatedAddress->MemoryControllerId);

  //
  // Init. return values
  //
  TranslatedAddress->SystemAddress = (UINT64)-1;

  //
  // Get A7 mode and determine # of low-order address bits
  // that are not touched by interleave
  //
  imcMcmtr.Data         = MemReadPciCfg(node, PARAM_UNUSED, MCMTR_MC_MAIN_REG);
  haSysDefeature2.Data  = MemReadPciCfg(NodeToSkt(node), TranslatedAddress->MemoryControllerId, HASYSDEFEATURE2_HA_CFG_REG);
  imcMcmtr.Data         = MemReadPciCfg(node, PARAM_UNUSED, MCMTR_MC_MAIN_REG);
  tolm.Data          = MemReadPciCfg(NodeToSkt(node), 0, TOLM_IIO_VTD_REG);
  haSysDefeature.Data           = MemReadPciCfg (NodeToSkt(node), TranslatedAddress->MemoryControllerId, HASYSDEFEATURE_HA_CFG_REG);

  tolmLimit = ((tolm.Bits.addr+1) << 26) - 1;

  if(haSysDefeature2.Bits.mcchanshiftupenable){
    //
    // A7 mode is enabled
    // bits 6:0 do not participate in interleave
    //
    shiftVal = 7;
  } else {
    //
    // A7 mode is disabled
    // bits 5:0 do not participate in interleave
    shiftVal = 6;
  }
  RAS_DEBUG((1, "\nMapChannelAddressToSystemAddress - shiftVal:%d\n", shiftVal));

  //
  // Determine low address bits from RankAddress
  //
  lowAddrBits = channelAddr & ((1 << shiftVal)-1);
 
  matchFound = 0;
  for (tad = 0; tad < TAD_RULES; tad++) {
    if (matchFound == 1) break;

    tadWayness.Data = MemReadPciCfg(  node, PARAM_UNUSED, TADWAYNESS_0_MC_MAIN_REG + (tad * 4));
    sktWays         = 1 << tadWayness.Bits.tad_skt_way;
    chWays          = (UINT8)tadWayness.Bits.tad_ch_way + 1;
    tadLimit        = tadWayness.Bits.tad_limit;
    imcTADChnlIlvOffset.Data = MemReadPciCfg(  node, TranslatedAddress->ChannelId, TADCHNILVOFFSET_0_MCDDC_DESC_REG + tad*4);
    chOffset = (UINT64)imcTADChnlIlvOffset.Bits.tad_offset << 26;
    chn_idx_offset = imcTADChnlIlvOffset.Bits.chn_idx_offset;

    for(tgtChIndex = 0; tgtChIndex < chWays; tgtChIndex++){
      //
      // Check if any of the target channels match the channel id
      // Bits 7:0 give target CH; 2 bits per target CH
      //
      switch (tgtChIndex) {
      case 0:
        targetCh = (UINT8)tadWayness.Bits.tad_ch_tgt0;
        break;
      case 1:
        targetCh = (UINT8)tadWayness.Bits.tad_ch_tgt1;
        break;
      case 2:
        targetCh = (UINT8)tadWayness.Bits.tad_ch_tgt2;
        break;
      case 3:
        targetCh = (UINT8)tadWayness.Bits.tad_ch_tgt3;
        break;
      }
      
      if( targetCh == TranslatedAddress->ChannelId) break;
    }

    chInterleave  = tgtChIndex;
    //
    // Determine approx. system Address
    //
    systemAddr = ((((channelAddr >> shiftVal) * chWays * sktWays)  << shiftVal)  + chOffset)  | lowAddrBits;

    RAS_DEBUG((1, "\nMapChannelAddressToSystemAddress - Potential systemAddr = %lx. TargetCh = %x \n", systemAddr, TranslatedAddress->ChannelId));
  
    //
    // Calculate corrected index
    //
    if(chWays == 03)  {
      correctedChIndex = ((chInterleave + chWays - imcTADChnlIlvOffset.Bits.chn_idx_offset) * sktWays) % chWays;
    } else {
      if(haSysDefeature2.Bits.mcchanhashen == 1) {
        for(chXorBitPos=12; chXorBitPos<27; chXorBitPos += 2) {
          chInterleave ^= ((systemAddr >> chXorBitPos) & 0x3);
        }
        chInterleave  %= chWays;
      }
      if( (haSysDefeature.Bits.mirrormod == 0) || ((tgtChIndex!=0) && (haSysDefeature2.Bits.ptlmiren)) )
          correctedChIndex = chInterleave;
      else
              correctedChIndex = chInterleave % chWays;
    }
  
    correctedChIndex  = correctedChIndex << (shiftVal + tadWayness.Bits.tad_skt_way );
    systemAddr        += correctedChIndex;
  
    RAS_DEBUG((1, "\nMapChannelAddressToSystemAddress - Potential systemAddr = %lx. correctedChIndex = %x \n", systemAddr, correctedChIndex));


    //
    // systemAddr should be > 4G if it is > TOLM else move to next TAD entry
    // TOLM to 4G is MMCFG/MMIO hole
    //
    if(systemAddr > tolmLimit && systemAddr < ((UINT64)FOUR_GB_MEM << 26)) continue;
  
    //
    // If system address is within the tad limit, then we've found the
    // TAD entry that maps this address!
    //
    if(tadLimit >= (systemAddr >> 26)){
      matchFound = 1;
      RAS_DEBUG((1, "\nMapChannelAddressToSystemAddress - TadRule:%d, tgtChIndex:%d, correctedChIndex:%d, TadOffset:%x\n",
                        tad, tgtChIndex, correctedChIndex, imcTADChnlIlvOffset.Bits.tad_offset));
      RAS_DEBUG((1, "\nMapChannelAddressToSystemAddress - Potential systemAddr = %lx with addition of correctedChIndex\n", systemAddr));
      break;  // This channel in the TAD rule didn't map to address; move to next TAD entry
    }
        
  } // tad loop

  if(matchFound){
    //
    // Determine SAD entry that maps this system address
    //
    for(sad = 0; sad < SAD_RULES; sad++){
      cboDramRule.Data = MemReadPciCfg(NodeToSkt(node), 0, DRAM_RULE(sad));
      if(cboDramRule.Bits.rule_enable == 0) continue;

      sadLimit = cboDramRule.Bits.limit;
      if((systemAddr >> 26) <= sadLimit){
        break; // we found the SAD entry that maps the address
      }
    } // sad loop

    status = EFI_INVALID_PARAMETER;
    if(sad < SAD_RULES){

      cboInterleaveList.Data = MemReadPciCfg(NodeToSkt(node), 0, INTERLEAVE_LIST(sad));

      packageNum  =  (UINT8)-1;
      for(packageNum = 0; packageNum < MAX_SAD_WAYS; packageNum++){
        //
        // Determine the target index
        // Bits 31:0 give the target Node, 4-bits per Node
        //
        switch (packageNum) {
        case 0:
          targetHa = (UINT8)cboInterleaveList.Bits.package0;
          break;
        case 1:
          targetHa = (UINT8)cboInterleaveList.Bits.package1;
          break;
        case 2:
          targetHa = (UINT8)cboInterleaveList.Bits.package2;
          break;
        case 3:
          targetHa = (UINT8)cboInterleaveList.Bits.package3;
          break;
        case 4:
          targetHa = (UINT8)cboInterleaveList.Bits.package4;
          break;
        case 5:
          targetHa = (UINT8)cboInterleaveList.Bits.package5;
          break;
        case 6:
          targetHa = (UINT8)cboInterleaveList.Bits.package6;
          break;
        case 7:
          targetHa = (UINT8)cboInterleaveList.Bits.package7;
          break;
        } // switch
        if(targetHa == haNodeId){
          break;
        }
      } // packageNum loop

        //
        // Determine the full system Address
        //

      if(cboDramRule.Bits.a7mode == 1){
        lowerBits  = ((systemAddr >> 7) & 0x3) << 1;
        lowerBits |=  (systemAddr >> 9) & 0x1;

      } else {
        lowerBits  =  (systemAddr >> 6) & 0x7;
      }

      if(packageNum !=  (UINT8)-1){

        if(cboDramRule.Bits.interleave_mode == 1) {
          //XOR mode disable case.
          for(packageNum = 0; packageNum < 8; packageNum++){
            //
            // Determine the target index
            // Bits 31:0 give the target Node, 4-bits per Node
            //
            switch (packageNum) {
              case 0:
                targetHa = (UINT8)cboInterleaveList.Bits.package0;
                break;
              case 1:
                targetHa = (UINT8)cboInterleaveList.Bits.package1;
                break;
              case 2:
                targetHa = (UINT8)cboInterleaveList.Bits.package2;
                break;
              case 3:
                targetHa = (UINT8)cboInterleaveList.Bits.package3;
                break;
              case 4:
                targetHa = (UINT8)cboInterleaveList.Bits.package4;
                break;
              case 5:
                targetHa = (UINT8)cboInterleaveList.Bits.package5;
                break;
              case 6:
                targetHa = (UINT8)cboInterleaveList.Bits.package6;
                break;
              case 7:
                targetHa = (UINT8)cboInterleaveList.Bits.package7;
                break;
            } // switch
            andResult = lowerBits & packageNum;
            if(andResult != lowerBits) continue;

            if(targetHa == haNodeId) {

              if(cboDramRule.Bits.a7mode == 1){
                //
                // A7 mode is enabled, SystemAddr[9] = Index[0]
                // SystemAddr[8:7] = Index[2:1]
                //
                systemAddr |= (packageNum & 1) << 9;
                systemAddr |=  (packageNum>> 1) << 7;

              } else {
                //
                // A7 mode is disabled, SystemAddr[8:6] = Index[2:0]
                //
                systemAddr|= (packageNum << 6);
              }
              RAS_DEBUG((1, "\nMapChannelAddressToSystemAddress - systemAddr = %x \n", systemAddr));
              TranslatedAddress->SystemAddress = systemAddr;
              return EFI_SUCCESS;
            } //   if(targetHa == haNodeId) {
          }  // for(packageNum = 0; packageNum < 8; packageNum++){

        } else {   //  if(cboDramRule.Bits.interleave_mode == 1) {

          //
          // XOR enbale case.
          // For loop to get the missing bits
          //
          for(packageNum = 0; packageNum < 8; packageNum++){
            //
            // Determine the target index
            // Bits 31:0 give the target Node, 4-bits per Node
            //
            switch (packageNum) {
            case 0:
              targetHa = (UINT8)cboInterleaveList.Bits.package0;
              break;
            case 1:
              targetHa = (UINT8)cboInterleaveList.Bits.package1;
              break;
            case 2:
              targetHa = (UINT8)cboInterleaveList.Bits.package2;
              break;
            case 3:
              targetHa = (UINT8)cboInterleaveList.Bits.package3;
              break;
            case 4:
              targetHa = (UINT8)cboInterleaveList.Bits.package4;
              break;
            case 5:
              targetHa = (UINT8)cboInterleaveList.Bits.package5;
              break;
            case 6:
              targetHa = (UINT8)cboInterleaveList.Bits.package6;
              break;
            case 7:
              targetHa = (UINT8)cboInterleaveList.Bits.package7;
              break;
            } // switch
            if(targetHa == haNodeId){
  
              missingBits = packageNum ^ (UINT8)((systemAddr >> 16) & 0x7);
              if(cboDramRule.Bits.a7mode == 1){
                //
                // A7 mode is enabled, SystemAddr[9] = Index[0]
                // SystemAddr[8:7] = Index[2:1]
                //
                matchMB = (missingBits & 1);
                matchMB |= (((missingBits>>1) / sktWays)<<1);
                matchLB = lowerBits & 0x1;
                matchLB |= (((lowerBits>>1)   / sktWays)<<1);
              } else {
                //
                // A7 mode is disabled, SystemAddr[8:6] = Index[2:0]
                //
                matchMB = (UINT8)(missingBits / sktWays);
                matchLB = (UINT8)(lowerBits  / sktWays);
              } // if(cboDramRule.Bits.a7mode == 1){

              if(matchMB == matchLB) {

                if(cboDramRule.Bits.a7mode == 1){
                  //
                  // A7 mode is enabled, SystemAddr[9] = Index[0]
                  // SystemAddr[8:7] = Index[2:1]
                  //
                  systemAddr |= (missingBits & 1) << 9;
                  systemAddr |= (missingBits & 6) << 6;
                } else {
                  //
                  // A7 mode is disabled, SystemAddr[8:6] = Index[2:0]
                  //
                  systemAddr|= (missingBits << 6);
                }  // if(cboDramRule.Bits.a7mode == 1){
                TranslatedAddress->SystemAddress = systemAddr;
                RAS_DEBUG((1, "\nMapChannelAddressToSystemAddress - SadRule:%x, A7Mode :%x, Ch shiftVal:%x, XorMode : %x(0- Enable, 1- disable), \n", sad, cboDramRule.Bits.a7mode, shiftVal, cboDramRule.Bits.interleave_mode));
                RAS_DEBUG((1, "\nMapChannelAddressToSystemAddress - SystemAddress : %lx \n",  TranslatedAddress->SystemAddress));
                return EFI_SUCCESS;

              } // if(matchMB == matchLB) {

            } // if(targetHa == haNodeId){
          } // //  if(cboDramRule.Bits.interleave_mode == 1) XOR mode
        }  // for(packageNum = 0; packageNum < 8; packageNum++)packageNum }   /packageNumamRulepackageNumrleave_mode == 1) {
        TranslatedAddress->SystemAddress = systemAddr;
      } // if(packageNum !=  (UINT8)-1){
    } // if(sad == SAD_RULES)
  }

  return status;
}

/**

Routine Description:
   Translate DIMM Address to System Address

Arguments:

    SocketId
    MemoryControllerId
    ChannelId
    DimmSlot
    DimmRank
    Row
    Col
    Bank
    ptr to SystemAddress

Modifies:
    SystemAddress

Returns:
  EFI_SUCCESS / Error code


--*/
EFI_STATUS
EFIAPI
DimmAddressToSystemAddress(
    IN  UINT8   SocketId,
    IN  UINT8   MemoryControllerId,
    IN  UINT8   ChannelId,
    IN  UINT8   DimmSlot,
    IN  UINT8   DimmRank,
    IN  UINT32  Row,
    IN  UINT32  Col,
    IN  UINT8   Bank,
    IN  UINT8   BankGroup,
    IN  UINT8   ChipSelect,
    OUT UINT64* SystemAddress
 )
{
  UINT8   socketId;
  UINT8   memoryControllerId;
  UINT8   channelId;
  UINT8   dimmSlot;
  UINT8   dimmRank;
  UINT32  row;
  UINT32  col;
  UINT8   bank;
  UINT8   bankGroup;
  UINT8   chipSelect;
  UINT8   node;
  EFI_STATUS          status = EFI_SUCCESS;
  TRANSLATED_ADDRESS  revTranslatedAddress;

  *SystemAddress = (UINT64)-1;

  //
  // Fill params
  //
  revTranslatedAddress.SocketId            = SocketId;
  revTranslatedAddress.MemoryControllerId  = MemoryControllerId;
  revTranslatedAddress.ChannelId           = ChannelId;
  revTranslatedAddress.DimmSlot            = DimmSlot;
  revTranslatedAddress.DimmRank            = DimmRank;
  revTranslatedAddress.Row                 = Row;
  revTranslatedAddress.Col                 = Col;
  revTranslatedAddress.Bank                = Bank;
  revTranslatedAddress.BankGroup           = BankGroup;
  revTranslatedAddress.ChipSelect          = ChipSelect;

  node  = SktHaToNode(SocketId, MemoryControllerId);
  mDdr4Dimm = (mSystemMemoryMap->NodeInfo[node].DramType == SPD_TYPE_DDR4)? 1 :0;

  RAS_DEBUG((1, "\n DimmAddressToSystemAddress (Reverse Address Translation) : \n\n"));


  status = MapDimmAddressToRankAddress(&revTranslatedAddress);
  if(!status){
    status = MapRankAddressToChannelAddress(&revTranslatedAddress);
  }
  if(!status){
    status = MapChannelAddressToSystemAddress(&revTranslatedAddress);
  }

  RAS_DEBUG((1, "\n DimmAddressToSystemAddress : revTranslatedAddress %lx \n", revTranslatedAddress.SystemAddress));

  if(!status){
    //
    // Translate the the calculated system address and check if it matches
    //
    SystemAddressToDimmAddress(revTranslatedAddress.SystemAddress, &socketId,
        &memoryControllerId, &channelId, &dimmSlot, &dimmRank, &row, &col, &bank, &bankGroup, &chipSelect);

    if(socketId != SocketId || memoryControllerId != MemoryControllerId || channelId != ChannelId || dimmSlot != DimmSlot ||
          dimmRank != DimmRank || row != Row || col != Col || bank != Bank || bankGroup != BankGroup || chipSelect != ChipSelect) {
      status = EFI_INVALID_PARAMETER;
    }

    *SystemAddress = revTranslatedAddress.SystemAddress;
  }
  RAS_DEBUG((1, "\n DimmAddressToSystemAddress : revTranslatedAddress end  status %x \n\n", status));
  return status;
}


/**

Routine Description:
   Check if current memory node is enabled

Arguments:

    Memory Node ID
    ptr to memory node enabled flag

Modifies:
    Memory node enabled flag

Returns:
  EFI_SUCCESS / Error code


--*/
EFI_STATUS
EFIAPI
IsMemNodeEnabled(
    IN UINT8 Node,
    OUT BOOLEAN *IsMemNodeEnabledFlag
)
{
  UINT8  State;

  GetCurrentMemoryNodeState (Node, &State);
  *IsMemNodeEnabledFlag = State;

  return EFI_SUCCESS;
}

/**

Routine Description:
   Enable RAS error signaling to BIOS

Arguments:

    Memory Node ID

Modifies:
    None

Returns:
  EFI_SUCCESS / Error code


--*/
EFI_STATUS
EFIAPI
EnableErrorSignallingtoBIOS(
    IN UINT8 Node
)
{
  return EFI_SUCCESS;
}

/*++

Routine Description:
   Get System Address from Error Log

Arguments:

  NodeId          - Memory Node ID
  ErrorLogSysAddr - ptr to buffer to hold sysaddress that
                    has error

Returns:
  EFI_SUCCESS / Error code
  ErrorLogSysAddr - System Address that has error

--*/
EFI_STATUS
EFIAPI
GetErrorLogSysAddr(
    IN  UINT8    NodeId,
    OUT UINT64   *ErrorLogSysAddr
 )
{
  // TODO:
  //
  // Check MC log to see if address can be used
  // If yes, then return that address
  //
  //
  // If MC log doesn't have a valid address,
  // reverse translate the address
  // from patrol scrub lo/hi addr registers
  //

  return EFI_NOT_AVAILABLE_YET;
}

/**

Routine Description:
   Get Logical to Physical rank ID

Arguments:
    NodeId
    DdrCh
    Logical Rank

Returns:
  Physical Rank Id

--*/
UINT8
GetLogicalToPhysicalRankId(
    IN UINT8  NodeId,
    IN UINT8  DdrCh,
    IN UINT8  LogicalRank
 )
{
  UINT8   csMapping2DPC[] = {0, 1, 2, 3, 4, 5, 6, 7};
  UINT8   csMapping3DPC[] = {0, 1, 2, 3, 4, 5, 8, 9};
  UINT8   phyRank = 0xFF;

  if( mMemRas->SystemMemInfo->NodeInfo[NodeId].ChannelInfo[DdrCh].MaxDimm == 3){
    if (LogicalRank >= 0 && LogicalRank < 8) {
      phyRank = csMapping3DPC[LogicalRank];
    }
  } else {
    if (LogicalRank >= 0 && LogicalRank < 8) {
      phyRank = csMapping2DPC[LogicalRank];
    }
  }

  RAS_DEBUG((4,"GetLogicalToPhysicalRankId  logical rank:%d -->physical rank:%d\n",LogicalRank,phyRank));
  return phyRank;
}



/*++

Routine Description:
   Get Memory node state

Arguments:

  Memory Node ID
  ptr to memory node state

Returns:
  SUCCESS or error code

--*/
EFI_STATUS
EFIAPI
GetCurrentMemoryNodeState (
    IN UINT8      Node,
    IN OUT UINT8 *MemState
)
{
  *MemState = mMemRas->SystemMemInfo->NodeInfo[Node].Enabled;
  return EFI_SUCCESS;
}

/*++

Routine Description:
    Check correctable errors for independent mode and handle them

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
EFI_STATUS
EFIAPI
ClearUnhandledCorrError(
    IN UINT8 NodeId,
    IN EVENT_TYPE EventType,
    OUT BOOLEAN *IsEventHandled
 ){
  UINT8                             ddrCh;
  UINT8                             rank = 0;
  BOOLEAN                           OverflowFound = FALSE;
  EFI_STATUS                        status = EFI_SUCCESS;

  *IsEventHandled       = FALSE;

  if(EventType == EVENTS_IN_PROGRESS){
    //
    // clear unhandled correct error events cannot be 'IN PROGRESS'
    // indicate that event as not handled always
    //
    status = EFI_SUCCESS;
    *IsEventHandled = FALSE;

    return status;
  }

  for( ddrCh = 0; (ddrCh < MAX_CH) && (OverflowFound == FALSE); ddrCh++ ){
    if (mSystemMemoryMap->NodeInfo[NodeId].ChannelInfo[ddrCh].Enabled == 0) {
      continue;
    }
    //
    // Check if there is any rank in this channel
    // that has overflow bit set
    //
    for( rank = 0; (rank < MAX_RANK_CH) ; rank++ ){

      if( IsErrorExcdForThisRank( NodeId, ddrCh, rank) ){

        OverflowFound = TRUE;

        ClearErrorsForThisRank( NodeId, ddrCh, rank );

        //
        // Update event handled status
        //
        *IsEventHandled = TRUE;

        break;
      }
    } // for(rank ...)
  } // for(ddrCh ...)

  return status;
}
