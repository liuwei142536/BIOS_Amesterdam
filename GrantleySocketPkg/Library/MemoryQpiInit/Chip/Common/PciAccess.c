/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
--*/
/*************************************************************************
 *
 * Reference Code
 *
 * ESS - Enterprise Silicon Software
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2006 - 2014 Intel Corporation All Rights Reserved.
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

//
// Disable warning for unsued input parameters
//
#pragma warning(disable : 4100)


UINT32
ReadPciCfg (
  PSYSHOST host,
  UINT8    socket,
  UINT32   reg
  )
/*++

  Reads PCI configuration space using the MMIO mechanism

  @param host    - Pointer to the system host (root) structure
  @param socket  - CPU Socket Node number (Socket ID)
  @param reg     - input parameter "reg" uses the format in the Bus_Dev_Func_CFG.H files

  @retval Value in requested reg

--*/
{
  UINT8   *regAddr;
  UINT32  data;
  UINT32  bus;

  data = 0;

  //
  // determine if targeting IIO bus or Uncore Bus (see if bus# filed of the register address = 0)
  //
  switch ((reg & 0x0ff00000) >> 20) {
  case 0:
    //
    // IIO Bus
    //
    bus = host->var.common.busIio[socket];
    break;

  case 1:
    //
    // Uncore Bus
    //
    bus = host->var.common.busUncore[socket];
    break;

  case 2:
    //
    // PCH on Bus 0
    //
    bus = 0;
    break;

  default:
    //
    // non-defined value
    //
    bus = 0;
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Attempt to access undefined bus number.\n"));
#endif
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
  }
  //
  // Form address
  //
  regAddr = (UINT8 *) (host->var.common.mmCfgBase + (bus << 20) + (reg & 0x000FFFFF));

  //
  // bus + Dev:Func:Offset
  // Check register size and read data
  //
  switch (reg >> 28) {
  case sizeof (UINT32):
    data = *(volatile UINT32 *) regAddr;
    break;

  case sizeof (UINT16):
    data = *(volatile UINT16 *) regAddr;
    break;

  case sizeof (UINT8):
    data = *(volatile UINT8 *) regAddr;
    break;

  default:
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Invalid register size in reg = 0x%X.\n", reg));
#endif
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
  }

#ifdef MRC_TRACE
#ifdef SERIAL_DBG_MSG
  /*
   *Socket, Type, Address, Direction, Size, Data

   Where:
   - Socket = processor socket # that executes the instruction
   - Type = 0 for IO, or 1 for Memory
   - Address = 16-bit IO or 32-bit Memory address (the MMIO address will encode bus, device, function, offset according to MMCFG format)
   - Direction = 0 for Rd, or 1 for Wr
   - Size = 1, 2, or 4 bytes
   - Data = hex data corresponding to the size

   For example:

   *0, 0, 0x80, 1, 1, 0xA0
   *0, 1, 0x100800dc, 1, 4, 0x00000055
   *0, 1, 0x10000000, 0, 4, 0x36008086
   *0, 1, 0x10000008, 0, 1, 0x00
   */
  if (checkMsgLevel(host, SDBG_TRACE)) {
    if (!((reg  == SR_POST_CODE_CSR)||(reg  == SR_BIOS_SERIAL_DEBUG_CSR)||(reg  == SMBCNTL_0_MC_MAIN_REG)||(reg  == SMBCNTL_1_MC_MAIN_REG) \
         ||(reg  == SMBCMD_0_MC_MAIN_REG)||(reg  == SMBCMD_1_MC_MAIN_REG)||(reg  == SR_PRINTF_SYNC_CSR))) {
    getPrintFControl(host);
    rcPrintf ((host, "*%d, 1, 0x%x, 0, %d, ", socket, regAddr, reg >> 28));

    //
    // Determine register size
    //
    switch (reg >> 28) {
    case sizeof (UINT32):
      rcPrintf ((host, "0x%08x\n", (UINT32) data));
      break;

    case sizeof (UINT16):
      rcPrintf ((host, "0x%04x\n", (UINT16) data));
      break;

    case sizeof (UINT8):
      rcPrintf ((host, "0x%02x\n", (UINT8) data));
      break;

    default:
      rcPrintf ((host, "Invalid register size in reg = 0x%X.\n", reg));
      RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
    }
    releasePrintFControl(host);
    }
  }
  //
  // if SDBG_TRACE
  //
#endif
#endif
  //
  // Return data
  //
  return data;
}

void
WritePciCfg (
  PSYSHOST host,
  UINT8    socket,
  UINT32   reg,
  UINT32   data
  )
/*++

  Writes specified data to PCI configuration space using the MMIO mechanism

  @param host    - Pointer to the system host (root) structure
  @param socket  - CPU Socket Node number (Socket ID)
  @param reg     - input parameter "reg" uses the format in the Bus_Dev_Func_CFG.H files
  @param data    - Value to write

  @retval N/A

--*/
{
  UINT8   *regAddr;
  UINT32  bus;
#ifdef MRC_TRACE
  char *tag = "*";
#endif


  //
  // determine if targeting IIO bus or Uncore Bus (see if bus# filed of the register address = 0)
  //
  switch ((reg & 0x0ff00000) >> 20) {
  case 0:
    //
    // IIO Bus
    //
    bus = host->var.common.busIio[socket];
    break;

  case 1:
    //
    // Uncore Bus
    //
    bus = host->var.common.busUncore[socket];
    break;

  case 2:
    //
    // PCH on Bus 0
    //
    bus = 0;
    break;

  default:
    //
    // non-defined value
    //
    bus = 0;
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Attempt to access undefined bus number.\n"));
#endif
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
  }
  //
  // Form address
  //
  regAddr = (UINT8 *) (host->var.common.mmCfgBase + (bus << 20) + (reg & 0x000FFFFF));
  //
  // bus + Dev:Func:Offset
  // Check register size and write data
  //
  switch (reg >> 28) {
  case sizeof (UINT32):
    *(volatile UINT32 *) regAddr = data;
    break;

  case sizeof (UINT16):
    *(volatile UINT16 *) regAddr = (UINT16) data;
    break;

  case sizeof (UINT8):
    *(volatile UINT8 *) regAddr = (UINT8) data;
    break;

  default:
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Invalid register size in reg = 0x%X.\n", reg));
#endif
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
  }

#ifdef MRC_TRACE
#ifdef SERIAL_DBG_MSG

  if (checkMsgLevel(host, SDBG_TRACE|SDBG_RCWRITETAG) ) {
    if (!((reg  == SR_POST_CODE_CSR)||(reg  == SR_BIOS_SERIAL_DEBUG_CSR)||(reg  == SMBCNTL_0_MC_MAIN_REG)||(reg  == SMBCNTL_1_MC_MAIN_REG) \
         ||(reg  == SMBCMD_0_MC_MAIN_REG)||(reg  == SMBCMD_1_MC_MAIN_REG)||(reg  == SR_PRINTF_SYNC_CSR))) {
    getPrintFControl(host);

    if ( checkMsgLevel(host, SDBG_RCWRITETAG ) ) {
       tag = "RCREGWRITE:";
    }

    rcPrintf ((host, "\n%s%d, 1, 0x%x, 1, %d, ", tag, socket, regAddr, reg >> 28));

    //
    // Determine register size
    //
    switch (reg >> 28) {
    case sizeof (UINT32):
      rcPrintf ((host, "0x%08x\n", (UINT32) data));
      break;

    case sizeof (UINT16):
      rcPrintf ((host, "0x%04x\n", (UINT16) data));
      break;

    case sizeof (UINT8):
      rcPrintf ((host, "0x%02x\n", (UINT8) data));
      break;

    default:
      rcPrintf ((host, "Invalid register size in reg = 0x%X.\n", reg));
      RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
    }
    releasePrintFControl(host);
    }
  }
  //
  // if SDBG_TRACE
  //
#endif
#endif

}

UINT32
WaitForPciCfg (
  PSYSHOST host,
  UINT8    socket,
  UINT32   reg,
  UINT32   mask,
  UINT32   data
  )
/*++

  Wait for specified bits of a PCI Cfg register to equal the required data

  @param host    - Pointer to the system host (root) structure
  @param socket  - CPU Socket Node number (Socket ID)
  @param reg     - input parameter "reg" uses the format in the Bus_Dev_Func_CFG.H files
  @param mask    - Bit patterns to test for wait condition
  @param data    - value required to stop waiting

  @retval 0 - Success
  @retval 1 - Failure, it timed out

--*/
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
    tempData = ReadPciCfg (host, socket, reg);
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

#define FUNC_SHIFT    12
#define DEV_SHIFT     15
#define BUS_SHIFT     20

#define BDF(Bus, Dev, Func)      ((Bus << BUS_SHIFT) | (Dev << DEV_SHIFT) | (Func < FUNC_SHIFT))

