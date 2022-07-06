/**
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
**/
/*************************************************************************
 *
 * Reference Code
 *
 * ESS - Enterprise Silicon Software
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2006 - 2015 Intel Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents
 * related to the source code ("Material") are owned by Intel
 * Corporation or its suppliers or licensors. Title to the Material
 * remains with Intel Corporation or its suppliers and licensors.
 * The Material contains trade secrets and proprietary and confidential
 * information of Intel or its suppliers and licensors. The Material
 * is protected by worldwide copyright and trade secret laws and treaty
 * provisions.  No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other
 * intellectual property right is granted to or conferred upon you
 * by disclosure or delivery of the Materials, either expressly,
 * by implication, inducement, estoppel or otherwise. Any license
 * under such intellectual property rights must be express and
 * approved by Intel in writing.
 *
 ************************************************************************/

#include "SysFunc.h"
#include "CpuPciAccess.h"

//
// Local Prototypes
//
UINT32 GetMemPCIAddr (PSYSHOST host, UINT8 socket, UINT8 ChIdOrBoxInst, UINT32 Offset);

/**

  Converts NodeId, ChannelId to instance of type BoxType/FuncBlk based on
  Cpu type and cpu sub type

  @param host      - Pointer to the system host (root) structure
  @param NodeId    - Memory controller index
  @param BoxType   - Box Type; values come from CpuPciAccess.h
  @param ChannelId - DDR/VMSE channel Id within a Memory controller, 0 based, 0xFF if not used
  @param FuncBlk   - Functional Block; values come from CpuPciAccess.h

  @retval Box Instance

**/
STATIC
UINT8
MemGetBoxInst (
    PSYSHOST host,
    UINT8    NodeId,
    UINT8    BoxType,
    UINT8    ChannelId,
    UINT8    FuncBlk
  )
{
  UINT8 BoxInst = 0xFF;
  UINT8 CpuType;

  // Get the CPU type and sub-type
  CpuType     = host->var.common.cpuType;

  switch (BoxType) {
  case BOX_HA:
    BoxInst = NodeId;
    break;

  case BOX_MC:
    BoxInst = NODE_TO_MC(NodeId);                                   // One instance per Memory controller
    break;

  case BOX_MCDDC:
    if (ChannelId < MAX_CH) {
      BoxInst = ChannelId;             // One instance per DDR Channel
    } else {
#ifdef SERIAL_DBG_MSG
      rcPrintf ((host, "Wrong Ch ID: 0x%08x passed with BOX_MCDDC\n", (UINT32) ChannelId));
#endif
      RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
    }
    break;

  case BOX_MCIO:
    switch (FuncBlk) {
    case MCIO_DDRIOGBC:
    case MCIO_DDRIOEXTGBC:
      BoxInst = NODE_TO_MC(NodeId);                                 // One instance per Memory controller
      break;

    case MCIO_DDRIO:
    case MCIO_DDRIOEXT:
    case MCIO_DDRIOMCC:
    case MCIO_DDRIOEXTMCC:
      switch (CpuType) {
      case CPU_BDX:
      case CPU_HSX:
        BoxInst = ChannelId;                                        // one instance per DDR Channel
        break;
      } // cpu type
      break;
    default:
#ifdef SERIAL_DBG_MSG
      rcPrintf ((host, "Invalid FuncBlk: 0x%08x passed with BOX_MCIO\n", (UINT32) FuncBlk));
#endif
      RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
    } // funcblk
    break;

  default:
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Invalid BoxType: 0x%08x passed\n", (UINT32) BoxType));
#endif
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
  }
  return BoxInst;
}

/**

  Returns regBase with true offset (after processing pseudo offset, if needed)

  @param host          - Pointer to the system host (root) structure
  @param NodeId        - Memory controller index
  @param ChIdOrBoxInst - DDR/VMSE channel Id within a memory controller
                         or Box Instance (in case of non-MC boxes), 0 based, 0xFF if not used
  @param regBase       - Register offset; values come from the auto generated header file
                       - may be a pseudo offset

  @retval Updated Register OFfset

**/
UINT32
MemPciCfgOffset (
  PSYSHOST host,
  UINT8    NodeId,
  UINT8    ChIdOrBoxInst,
  UINT32   regBase
  )
{
  CSR_OFFSET RegOffset;
  CSR_OFFSET TempReg;

  RegOffset.Data = regBase;

  //
  // Adjust offset if pseudo
  //
  if (RegOffset.Bits.pseudo) {
    TempReg.Data = GetRegisterOffset (host, RegOffset);
    RegOffset.Bits.offset = TempReg.Bits.offset;
    RegOffset.Bits.size = TempReg.Bits.size;
    //
    // Clear offset bit
    //
    RegOffset.Bits.pseudo = 0;
  }

  //
  // Return updated reg offset
  //
  return RegOffset.Data;
}

/**

  Reads CPU Memory Controller configuration space using the MMIO mechanism

  @param host          - Pointer to the system host (root) structure
  @param NodeId        - Memory controller index
  @param ChIdOrBoxInst - DDR/VMSE channel Id within a memory controller
                         or Box Instance (in case of non-MC boxes), 0 based, 0xFF if not used
  @param Offset        - Register offset; values come from the auto generated header file

  @retval Register value

**/
UINT32
MemReadPciCfgEx (
  PSYSHOST host,
  UINT8    NodeId,
  UINT8    ChIdOrBoxInst,
  UINT32   Offset
  )
{
  UINT8 BoxInst;
  UINT8 SocId;
  UINT8 BoxType;
  UINT8 FuncBlk;
  CSR_OFFSET RegOffset;

  SocId   = NODE_TO_SKT(NodeId);

  RegOffset.Data = Offset;
  BoxType = (UINT8)RegOffset.Bits.boxtype;
  FuncBlk = (UINT8)RegOffset.Bits.funcblk;

  if (BoxType == BOX_MCIO) {
    if (RegOffset.Bits.pseudo) {
      MemDebugPrint ((host, SDBG_MAX, NodeId, ChIdOrBoxInst, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "MemReadPciCfgEx: WARNING MCIO with Pseudo Offset = 0x%x ", Offset));
    }
  }

  if( BoxType == BOX_MC || BoxType == BOX_MCDDC || BoxType == BOX_MCIO ){
    //
    // Need to convert the NodeId/DDR channel ID to box Instance for
    // MC boxes
    //
    BoxInst = MemGetBoxInst (host, NodeId, BoxType, ChIdOrBoxInst, FuncBlk);
  } else {
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Invalid BoxType: 0x%08x passed\n", (UINT32) BoxType));
#endif
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
    //
    // For non-MC boxes pass the Box Instance directly
    //
    BoxInst = ChIdOrBoxInst;
  }

  return ReadCpuPciCfgEx (host, SocId, BoxInst, Offset);
}

/**

  Writes CPU Memory Controller configuration space using the MMIO mechanism

  @param host          - Pointer to the system host (root) structure
  @param NodeId        - Memory controller index
  @param ChIdOrBoxInst - DDR/VMSE channel Id within a memory controller
                         or Box Instance (in case of non-MC boxes), 0 based, 0xFF if not used
  @param Offset        - Register offset; values come from the auto generated header file
  @param Data          - Register data to be written

  @retval None

**/
void
MemWritePciCfgEx (
  PSYSHOST host,
  UINT8    NodeId,
  UINT8    ChIdOrBoxInst,
  UINT32   Offset,
  UINT32   Data
  )
{
  UINT8 BoxInst;
  UINT8 SocId;
  UINT8 BoxType;
  UINT8 FuncBlk;
  CSR_OFFSET RegOffset;

  SocId   = NODE_TO_SKT(NodeId);

  RegOffset.Data = Offset;
  BoxType = (UINT8)RegOffset.Bits.boxtype;
  FuncBlk = (UINT8)RegOffset.Bits.funcblk;

  if( BoxType == BOX_MC || BoxType == BOX_MCDDC || BoxType == BOX_MCIO ){
    //
    // Need to convert the NodeId/DDR channel ID to box Instance for
    // MC boxes
    //
    BoxInst = MemGetBoxInst (host, NodeId, BoxType, ChIdOrBoxInst, FuncBlk);
  } else {
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Invalid BoxType: 0x%08x passed\n", (UINT32) BoxType));
#endif
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
    //
    // For non-MC boxes pass the Box Instance directly
    //
    BoxInst = ChIdOrBoxInst;
  }
  WriteCpuPciCfgEx (host, SocId, BoxInst, Offset, Data);
}

/**

  Reads CPU Memory Controller configuration space using the MMIO mechanism

  @param host          - Pointer to the system host (root) structure
  @param socket          - Socket number
  @param ChIdOrBoxInst - DDR/VMSE channel Id within a memory controller
                         or Box Instance (in case of non-MC boxes), 0 based, 0xFF if not used
  @param Offset        - Register offset; values come from the auto generated header file

  @retval Register value

**/
UINT32
MemReadPciCfgEp (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ChIdOrBoxInst,
  UINT32   Offset
  )
{
  UINT8 BoxInst;
  UINT8 SocId;
  UINT8 McId;
  UINT8 BoxType;
  UINT8 FuncBlk;
  CSR_OFFSET RegOffset;
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  SocId = socket;
  McId = GetMCID(host, socket, ChIdOrBoxInst);

  RegOffset.Data = Offset;
  BoxType = (UINT8)RegOffset.Bits.boxtype;
  FuncBlk = (UINT8)RegOffset.Bits.funcblk;

  if( BoxType == BOX_MC || BoxType == BOX_MCDDC || BoxType == BOX_MCIO || BoxType == BOX_HA){
    //
    // Need to convert the NodeId/DDR channel ID to box Instance for
    // MC boxes
    //
    BoxInst = MemGetBoxInst (host, McId, BoxType, ChIdOrBoxInst, FuncBlk);
  } else {
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Invalid BoxType: 0x%08x passed\n", (UINT32) BoxType));
#endif
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
    //
    // For non-MC boxes pass the Box Instance directly
    //
    BoxInst = ChIdOrBoxInst;
  }

  return ReadCpuPciCfgEx (host, SocId, BoxInst, Offset);
}

/**

  Reads CPU Memory Controller configuration space using the MMIO mechanism

  @param host          - Pointer to the system host (root) structure
  @param socket        - Socket number
  @param mdID          - Memory controller ID
  @param Offset        - Register offset; values come from the auto generated header file

  @retval Register value

**/
UINT32
MemReadPciCfgMC (
  PSYSHOST host,
  UINT8    socket,
  UINT8    mcId,
  UINT32   Offset
  )
{
  UINT8 BoxInst;
  UINT8 SocId;
  UINT8 BoxType;
  UINT8 FuncBlk;
  CSR_OFFSET RegOffset;
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  SocId = socket;

  RegOffset.Data = Offset;
  BoxType = (UINT8)RegOffset.Bits.boxtype;
  FuncBlk = (UINT8)RegOffset.Bits.funcblk;

  if (BoxType == BOX_MCIO) {
    if (RegOffset.Bits.pseudo) {
      MemDebugPrint ((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "\nMemReadPciCfgMC: WARNING MCIO with Pseudo Offset = 0x%x ", Offset));
    }
  }

  if( BoxType == BOX_MC || BoxType == BOX_MCDDC || BoxType == BOX_MCIO ){
    //
    // Need to convert the NodeId/DDR channel ID to box Instance for
    // MC boxes
    //
    BoxInst = MemGetBoxInst (host, mcId, BoxType, 0, FuncBlk);
  } else if (BoxType == BOX_HA) {
      BoxInst =  mcId;
    } else {
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Invalid BoxType: 0x%08x passed\n", (UINT32) BoxType));
#endif
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
    //
    // For non-MC boxes pass the Box Instance directly
    //
    BoxInst = mcId;
  }

  return ReadCpuPciCfgEx (host, SocId, BoxInst, Offset);
} // MemReadPciCfgMC

/**

  Writes CPU Memory Controller configuration space using the MMIO mechanism

  @param host          - Pointer to the system host (root) structure
  @param socket          - Socket number
  @param ChIdOrBoxInst - DDR/VMSE channel Id within a memory controller
                         or Box Instance (in case of non-MC boxes), 0 based, 0xFF if not used
  @param Offset        - Register offset; values come from the auto generated header file
  @param Data          - Register data to be written

  @retval None

**/
void
MemWritePciCfgEp (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ChIdOrBoxInst,
  UINT32   Offset,
  UINT32   Data
  )
{
  UINT8 BoxInst;
  UINT8 SocId;
  UINT8 BoxType;
  UINT8 McId;
  UINT8 FuncBlk;
  CSR_OFFSET RegOffset;
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  SocId = socket;
  McId = GetMCID(host, socket, ChIdOrBoxInst);

  RegOffset.Data = Offset;
  BoxType = (UINT8)RegOffset.Bits.boxtype;
  FuncBlk = (UINT8)RegOffset.Bits.funcblk;

  if( BoxType == BOX_MC || BoxType == BOX_MCDDC || BoxType == BOX_MCIO || BoxType == BOX_HA){
    //
    // Need to convert the NodeId/DDR channel ID to box Instance for
    // MC boxes
    //
    BoxInst = MemGetBoxInst (host, McId, BoxType, ChIdOrBoxInst, FuncBlk);
  } else {
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Invalid BoxType: 0x%08x passed\n", (UINT32) BoxType));
#endif
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
    //
    // For non-MC boxes pass the Box Instance directly
    //
    BoxInst = ChIdOrBoxInst;
  }
  WriteCpuPciCfgEx (host, SocId, BoxInst, Offset, Data);
}

/**

  Writes CPU Memory Controller configuration space using the MMIO mechanism

  @param host          - Pointer to the system host (root) structure
  @param socket        - Socket number
  @param mcId          - Memory controller ID
  @param Offset        - Register offset; values come from the auto generated header file
  @param Data          - Register data to be written

  @retval None

**/
void
MemWritePciCfgMC (
  PSYSHOST host,
  UINT8    socket,
  UINT8    mcId,
  UINT32   Offset,
  UINT32   Data
  )
{
  UINT8 BoxInst;
  UINT8 SocId;
  UINT8 BoxType;
  UINT8 FuncBlk;
  CSR_OFFSET RegOffset;
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  SocId = socket;

  RegOffset.Data = Offset;
  BoxType = (UINT8)RegOffset.Bits.boxtype;
  FuncBlk = (UINT8)RegOffset.Bits.funcblk;

  if (BoxType == BOX_MCIO) {
    if (RegOffset.Bits.pseudo) {
      MemDebugPrint ((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "MemWritePciCfgMC: WARNING MCIO with Pseudo Offset = 0x%x ", Offset));
    }
  }

  if( BoxType == BOX_MC || BoxType == BOX_MCDDC || BoxType == BOX_MCIO ){
    //
    // Need to convert the NodeId/DDR channel ID to box Instance for
    // MC boxes
    //
    BoxInst = MemGetBoxInst (host, mcId, BoxType, 0, FuncBlk);
  } else if (BoxType == BOX_HA) {
      BoxInst =  mcId;
    } else {
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Invalid BoxType: 0x%08x passed\n", (UINT32) BoxType));
#endif
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
    //
    // For non-MC boxes pass the Box Instance directly
    //
    BoxInst = mcId;
  }
  WriteCpuPciCfgEx (host, SocId, BoxInst, Offset, Data);
} // MemWritePciCfgMC

/**

  Wait for specified bits of a CPU PCI Cfg register to equal the required data

  @param host          - Pointer to the system host (root) structure
  @param NodeId        - Memory controller index
  @param ChIdOrBoxInst - DDR/VMSE channel Id within a memory controller
                         or Box Instance (in case of non-MC boxes), 0 based, 0xFF if not used
  @param Offset        - Register offset; values come from the auto generated header file
  @param mask          - Bit patterns to test for wait condition
  @param data          - value required to stop waiting

  @retval 0 - Success
  @retval 1 - Failure, it timed out

**/
UINT32
WaitForCpuPciCfgEx(
  PSYSHOST host,
  UINT8    NodeId,
  UINT8    ChIdOrBoxInst,
  UINT32   Offset,
  UINT32   mask,
  UINT32   data
  )
{
  UINT32  tempData;
  UINT32  startCount;
  UINT32  timeout;

  timeout = 0;
  //
  // Wait for command completion
  //
  startCount = GetCount (host);
  do {
    tempData = MemReadPciCfgEx(host,NodeId,ChIdOrBoxInst,Offset);
    if ((tempData & mask) == data) {
      break;
    }

    timeout = GetDelay (host, startCount);
  } while (timeout < CATCHALL_TIMEOUT);

  //
  // Check for failure
  //
  if (timeout >= CATCHALL_TIMEOUT) {
    return FAILURE;
  }

  return SUCCESS;
}

/**

Arguments:

  ch      - Channel to read
  RegBase - MMIO Reg address of first base device

  @retval EPMC main value

**/
UINT32
MemReadPciCfgMain (
  PSYSHOST  host,
  UINT8     socket,
  UINT32    regBase
  )
{
  UINT32  data = 0;

  switch (host->var.common.cpuType) {
  case CPU_HSX:
    switch (host->var.common.cpuSubType) {
    case CPU_HSX_1HA:
      data = MemReadPciCfgMC (host, socket, 0, regBase);
      break;

    case CPU_HSX_2HA:
      if(host->var.mem.socket[socket].imcEnabled[0]) {
        data = MemReadPciCfgMC (host, socket, 0, regBase);
      }
      if(host->var.mem.socket[socket].imcEnabled[1]) {
        data = MemReadPciCfgMC (host, socket, 1, regBase);
      }
      break;

    } // Cpu sub type
    break;
  case CPU_BDX:
    switch (host->var.common.cpuSubType) {
    case CPU_BDX_1HA:
      data = MemReadPciCfgMC (host, socket, 0, regBase);
      break;

    case CPU_BDX_2HA:
      if(host->var.mem.socket[socket].imcEnabled[0]) {
        data = MemReadPciCfgMC (host, socket, 0, regBase);
      }
      if(host->var.mem.socket[socket].imcEnabled[1]) {
        data = MemReadPciCfgMC (host, socket, 1, regBase);
      }
      break;

    } // Cpu sub type
    break;
  } // cpu type

  return data;
} // MemReadPciCfgMain

/**

  Get a Memory channel's EPMC Main value

  @param ch      - Channel to read
  @param RegBase - MMIO Reg address of first base device

  @retval EPMC main value

**/
void
MemWritePciCfgMain (
  PSYSHOST  host,
  UINT8     socket,
  UINT32    regBase,
  UINT32    data
  )
{
  switch (host->var.common.cpuType) {
  case CPU_HSX:
    switch (host->var.common.cpuSubType) {
    case CPU_HSX_1HA:
      MemWritePciCfgMC (host, socket, 0, regBase, data);
      break;

    case CPU_HSX_2HA:
      if(host->var.mem.socket[socket].imcEnabled[0]) {
        MemWritePciCfgMC (host, socket, 0, regBase, data);
      }
      if(host->var.mem.socket[socket].imcEnabled[1]) {
        MemWritePciCfgMC (host, socket, 1, regBase, data);
      }
      break;

    } // Cpu sub type
    break;
  case CPU_BDX:
    switch (host->var.common.cpuSubType) {
    case CPU_BDX_1HA:
      MemWritePciCfgMC (host, socket, 0, regBase, data);
      break;

    case CPU_BDX_2HA:
      if(host->var.mem.socket[socket].imcEnabled[0]) {
        MemWritePciCfgMC (host, socket, 0, regBase, data);
      }
      if(host->var.mem.socket[socket].imcEnabled[1]) {
        MemWritePciCfgMC (host, socket, 1, regBase, data);
      }
      break;

    } // Cpu sub type
    break;
  } // cpu type

} // WritePciCfgMCMain

/**

  Writes 64 bits

  @param ch      - Channel to read
  @param RegBase - MMIO Reg address of first base device

**/
void
MemWritePci64 (
  PSYSHOST      host,
  UINT8         socket,
  UINT8         ChIdOrBoxInst,
  UINT32        Offset,
  UINT64_STRUCT Data
  )
{
  UINT32 addr;

  addr = GetMemPCIAddr (host, socket, ChIdOrBoxInst, Offset);

  //MmioWrite64 (addr, Data);

} // MemWritePci64

/**

  Gets PCI MMIO address

  @param host          - Pointer to the system host (root) structure
  @param socket          - Socket number
  @param ChIdOrBoxInst - DDR/VMSE channel Id within a memory controller
                         or Box Instance (in case of non-MC boxes), 0 based, 0xFF if not used
  @param Offset        - Register offset; values come from the auto generated header file

  @retval None

**/
UINT32
GetMemPCIAddr (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ChIdOrBoxInst,
  UINT32   Offset
  )
{
  UINT8 BoxInst;
  UINT8 SocId;
  UINT8 BoxType;
  UINT8 McId;
  UINT8 FuncBlk;
  UINT8 Size;
  CSR_OFFSET RegOffset;
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  SocId = socket;
  McId = GetMCID(host, socket, ChIdOrBoxInst);

  RegOffset.Data = Offset;
  BoxType = (UINT8)RegOffset.Bits.boxtype;
  FuncBlk = (UINT8)RegOffset.Bits.funcblk;

  if( BoxType == BOX_MC || BoxType == BOX_MCDDC || BoxType == BOX_MCIO ){
    //
    // Need to convert the NodeId/DDR channel ID to box Instance for
    // MC boxes
    //
    BoxInst = MemGetBoxInst (host, McId, BoxType, ChIdOrBoxInst, FuncBlk);
  } else {
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Invalid BoxType: 0x%08x passed\n", (UINT32) BoxType));
#endif
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
    //
    // For non-MC boxes pass the Box Instance directly
    //
    BoxInst = 0;
  }

  return (UINT32)GetCpuPciCfgAddress (host, SocId, BoxInst, Offset, &Size);
}


