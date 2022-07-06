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
#include <Token.h> // AptioV server override
//#include "RcRegs.h"
#include "CpuPciAccess.h"
#include "SysHost.h"
#include "SysFunc.h"


#include "PSEUDO_OFFSET.h"


//
// Disable warning for unsued input parameters
//
#pragma warning(disable : 4100)
#pragma warning(disable : 4013)

//
// Local Prototypes
//
UINT8  GetLegacyBusNumber(VOID);


//
// PCI function translation table; note that this table doesn't capture the function
// information for all instacnes of a box. It only captures for the first instance.
// It has to be translated for other instances after the look up is done.
//
STATIC UINT8  FunTbl[MAX_CPU_TYPES][MAX_BOX_TYPES][8] = {
        {
          {0,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // CBO
          {4,    5,    6,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // CBOBC
          {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // RSVD1
          {0,    1,     2,    3,   0xFF, 0xFF, 0xFF, 0xFF}, // HA
          {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // RSVD2
          {0,    1,    2,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // R3QPI
          {0,    2,    3,    4,    5,    6,    7,    0xFF}, // QPIAGENT
          {0,    1,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // R2PCIE
          {5,    6,    7,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // UBOX
          {0,    1,    2,    0xFF, 4,    0xFF, 6,    0   }, // IIO
          {0,    1,    2,    3,    4,    5,    6,    7   }, // PCU
          {0,    1,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // MC
          {0,    1,    0,    1   , 0xFF, 0xFF, 0xFF, 0xFF}, // MCDDC
          {4,    5,    4,    5,    6,    7,    0xFF, 0xFF}, // MCIO
          {0   , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO PCIE DMI
          {0   , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO PCIE
          {0   , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO PCIE NTB
          {0   , 1   , 2   , 3   , 4   , 5   , 6   , 7   }, // IIO CB
          {0   , 1   , 2   , 0xFF, 0xFF, 0xFF, 0xFF,0xFF }, // IIO DFX
          {0   , 0xFF, 2   , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // VCU
          {0   , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // SBO
        }, // HSX
        {
          {0,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // CBO
          {0,    1,    2,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // CBOBC
          {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // RSVD1
          {0,    1,     2,    3,   0xFF, 0xFF, 0xFF, 0xFF}, // HA
          {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // RSVD2
          {4,    5,    6,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // R3QPI
          {0,    2,    3,    4,    5,    6,    7,    0xFF}, // QPIAGENT
          {0,    1,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // R2PCIE
          {0,    2,    3,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // UBOX
          {0,    1,    2,    3,    4,    5,    6,    0xFF}, // IIO
          {0,    1,    2,    3,    4,    5,    6,    7   }, // PCU
          {0,    1,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // MC
          {4,    6,    2,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // MCDDC
          {6,    7,    4,    5,    6,    7,    0xFF, 0xFF}, // MCIO
          {0,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIOPCIE_DMI
          {0,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIOPCIE_RP
          {0,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIOPCIE_NTB
          {0,    1,    2,    3,    4,    5,    6,    7   }, // IIOCB
          {0,    1,    2,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIODFX
          {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // VCU - not present for IVT
          {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // SBO - not present for IVT
        }, // IVT
        {
          {0,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // CBO
          {4,    5,    6,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // CBOBC
          {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // RSVD1
          {0,    1,     2,    3,   0xFF, 0xFF, 0xFF, 0xFF}, // HA
          {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // RSVD2
          {0,    1,    2,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // R3QPI
          {0,    2,    3,    4,    5,    6,    7,    0xFF}, // QPIAGENT
          {0,    1,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // R2PCIE
          {5,    6,    7,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // UBOX
          {0,    1,    2,    0xFF, 4,    0xFF, 6,    0   }, // IIO
          {0,    1,    2,    3,    4,    5,    6,    7   }, // PCU
          {0,    1,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // MC
          {0,    1,    0,    1   , 0xFF, 0xFF, 0xFF, 0xFF}, // MCDDC
          {4,    5,    4,    5,    6,    7,    0xFF, 0xFF}, // MCIO
          {0   , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO PCIE DMI
          {0   , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO PCIE
          {0   , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO PCIE NTB
          {0   , 1   , 2   , 3   , 4   , 5   , 6   , 7   }, // IIO CB
          {0   , 1   , 2   , 0xFF, 0xFF, 0xFF, 0xFF,0xFF }, // IIO DFX
          {0   , 0xFF, 2   , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // VCU
          {0   , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // SBO
        }, // BDX

    };

//
// Memory Controller Instance translation table; These table map
// the instance of a functional block to its PCI function
//
STATIC UINT8  McFuncTbl[MAX_CPU_INDEX][MAX_MCDDC_FUNC_BLKS][MAX_INST_IN_MC] = {
    {
        {0,   0xFF, 0xFF, 0xFF},    // MC_MAIN
        {1,   0xFF, 0xFF, 0xFF},    // MC_MAINEXT

    }, // HSX
    {
        {0,   0,    0xFF, 0xFF},    // MC_MAIN
        {1,   1,    0xFF, 0xFF},    // MC_MAINEXT

    }, // HSX 2HA
    {
        {0,   0,    0xFF, 0xFF},    // MC_MAIN
        {1,   1,    0xFF, 0xFF},    // MC_MAINEXT

    }, // IVT EX
};

STATIC UINT8  McddcFuncTbl[MAX_CPU_INDEX][MAX_MCDDC_FUNC_BLKS][MAX_INST_IN_MCDDC] = {
    {
        {0,   1,    0,    1,    0xFF, 0xFF, 0xFF, 0xFF},    // MCDDC_CTL
        {2,   3,    2,    3,    0xFF, 0xFF, 0xFF, 0xFF},    // MCDDC_DP
        {2,   3,    4,    5,    0xFF, 0xFF, 0xFF, 0xFF},    // MCDDC_DESC
    }, // HSX
    {
        {0,   1,    0,    1,    0xFF, 0xFF, 0xFF, 0xFF},    // MCDDC_CTL
        {2,   3,    2,    3,    0xFF, 0xFF, 0xFF, 0xFF},    // MCDDC_DP
        {2,   3,    2,    3,    0xFF, 0xFF, 0xFF, 0xFF},    // MCDDC_DESC
    }, // HSX 2HA
    {
        {4,   5,    0,    1,    4,    5,    0,    1},       // MCDDC_CTL
        {6,   7,    2,    3,    6,    7,    2,    3},       // MCDDC_DP
        {2,   3,    4,    5,    2,    3,    4,    5},       // MCDDC_DESC
    }, // IVT EX
};

STATIC UINT8  McioFuncTbl[MAX_CPU_INDEX][MAX_MCIO_FUNC_BLKS][MAX_INST_IN_MCIO] = {
    {
        {6,     0xFF, 0xFF, 0xFF},                            // MCIO_DDRIOGBC
        {7,     0xFF, 0xFF, 0xFF},                            // MCIO_DDRIOEXTGBC
        {4,     6,    4,    6   },                            // MCIO_DDRIO
        {5,     7,    5,    7   },                            // MCIO_DDRIOEXT
        {6,     6,    6,    6   },                            // MCIO_DDRIOMCC
        {7,     7,    7,    7   },                            // MCIO_DDRIOEXTMCC
    }, // HSX

    {
        {0xFF,  0xFF, 0xFF, 0xFF},                            // MCIO_DDRIOGBC
        {0xFF,  0xFF, 0xFF, 0xFF},                            // MCIO_DDRIOEXTGBC
        {4,     6,    4,    6   },                            // MCIO_DDRIO
        {5,     7,    5,    7   },                            // MCIO_DDRIOEXT
        {6,     6,    6,    6   },                            // MCIO_DDRIOMCC
        {7,     7,    7,    7   },                            // MCIO_DDRIOEXTMCC
   }, // HSX 2HA

   {
       {0xFF,  0xFF, 0xFF, 0xFF},                            // MCIO_DDRIOGBC
       {0xFF,  0xFF, 0xFF, 0xFF},                            // MCIO_DDRIOEXTGBC
       {4,     0,    4,    0   },                            // MCIO_DDRIO
       {5,     1,    5,    1   },                            // MCIO_DDRIOEXT
       {2,     2,    6,    6   },                            // MCIO_DDRIOMCC
       {2,     2,    7,    7   },                            // MCIO_DDRIOEXTMCC
   }, // IVT EX
};

#define PCIE_REG_ADDR(Bus,Device,Function,Offset) \
  (((Offset) & 0xfff) | (((Function) & 0x07) << 12) | (((Device) & 0x1f) << 15) | (((Bus) & 0xff) << 20))

//#ifndef SIM_BUILD
/**

    Stall execution after internal assertion fails

    @param haltOnError - 1 stalls in infinite loop; 0 returns to caller

    @retval None

**/
VOID RcDeadLoop (
  UINT8 haltOnError
  )
{
  //
  // Prevent from optimizing out
  //
  while (*(volatile UINT8 *) &haltOnError);
}
//#endif // SIM_BUILD

STATIC
UINT8
GetCpuIndex (
    PSYSHOST host
    )
/*++

  Returns the CPU Index for MC func tbl lookup based on CPU type and CPU sub type.
  This index will be used for MC box instance -> function mapping look-up

  @param host      - Pointer to the system host (root) structure

  @retval Index for CPU type

--*/
{
  UINT8  cpuIndex = 0xFF;

  switch(host->var.common.cpuType){
  case CPU_HSX:
    switch(host->var.common.cpuSubType){
    case CPU_HSX_1HA:
      cpuIndex = IDX_CPU_HSX_1HA;
      break;

    case CPU_HSX_2HA:
      cpuIndex = IDX_CPU_HSX_2HA;
      break;

    default:
#ifdef SERIAL_DBG_MSG
      rcPrintf ((host, "\nInvalid HSX CPU Sub type  %d. \n", host->var.common.cpuSubType));
#endif
      RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
    }
    break;


   case CPU_BDX:
    switch(host->var.common.cpuSubType){
    case CPU_BDX_1HA:
      cpuIndex = IDX_CPU_BDX_1HA;
      break;

    case CPU_BDX_2HA:
    case CPU_BDX_EX:
      cpuIndex = IDX_CPU_BDX_2HA;
      break;

    default:
#ifdef SERIAL_DBG_MSG
      rcPrintf ((host, "\nInvalid HSX CPU Sub type  %d. \n", host->var.common.cpuSubType));
#endif
      RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
    }
    break;

  case CPU_IVT:
    switch(host->var.common.cpuSubType){
    case CPU_IVT_EX:
      cpuIndex = IDX_CPU_IVT_EX;
      break;

    default:
#ifdef SERIAL_DBG_MSG
      rcPrintf ((host, "\nInvalid IVT CPU Sub type  %d. \n", host->var.common.cpuSubType));
#endif
      RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
    }
    break;

  default:
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "\nInvalid Cpu Type  %d. \n", host->var.common.cpuType));
#endif
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
  }

  return cpuIndex;
}

STATIC
UINT32
GetBusNumber (
  PSYSHOST host,
  UINT8    SocId,
  UINT8    BoxType
  )
/*++

  Indetifies the bus number for given SocId & BoxType

  @param host      - Pointer to the system host (root) structure
  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxType   - Box Type; values come from CpuPciAccess.h

  @retval PCI bus number

--*/
{
  UINT32   Bus;


  // Make sure SocId is valid
  if (!(host->var.common.socketPresentBitMap & (1 << SocId))) {
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "\nInvalid Socket Id %d. \n", SocId));
#endif // SERIAL_DBG_MSG
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
  }

  //
  // Each socket is assigned two buses - one for all Uncore devices and one for all IIO devices.
  // Check the box type and return the appropriate bus number.
  //
  if ((BoxType == BOX_IIO)          ||
      (BoxType == BOX_IIO_PCIE_DMI) ||
      (BoxType == BOX_IIO_PCIE_NTB) ||
      (BoxType == BOX_IIO_PCIE)     ||
      (BoxType == BOX_IIO_DFX)      ||
      (BoxType == BOX_IIO_CB)) {

    Bus = host->var.common.busIio[SocId];
  } else {
    Bus = host->var.common.busUncore[SocId];
  }

  return Bus;
}

UINT32
GetDeviceNumber (
  PSYSHOST host,
  UINT8    BoxType,
  UINT8    BoxInst,
  UINT8    FuncBlk
  )
/*++

  Indetifies the device number for given Box Type & Box Instance

  @param host      - Pointer to the system host (root) structure
  @param BoxType   - Box Type; values come from CpuPciAccess.h
  @param BoxInst   - Box Instance, 0 based
  @param FuncBlk   - Functional Block; values come from CpuPciAccess.h

  @retval PCI Device number

--*/
{
  UINT32   Dev = 0;
  UINT8    CpuType;
  UINT8    CpuSubType;

  // Get the CPU type, sub type
  CpuType = host->var.common.cpuType;
  CpuSubType = host->var.common.cpuSubType;

  //
  // Translate the Box Type & Instance into PCI Device number.
  //
  switch (BoxType) {
    case BOX_CBO:
      if (((CpuType == CPU_HSX) && (BoxInst < MAX_HSX_CBO)) || ((CpuType == CPU_BDX) && (BoxInst < MAX_BDX_CBO))) {
        if      (BoxInst < 8)  { Dev = 12; }
        else if (BoxInst < 16) { Dev = 13; }
        else                   { Dev = 14; }
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_IVT_CBO)) {
        Dev = (BoxInst & 1) + 12; // Even Cbos are on Dev 12, Odd Cbos are on Dev 13
      } else {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "\nInvalid CBo Box Instance %d. \n", BoxInst));
#endif // SERIAL_DBG_MSG
        RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
      }
      break;

    case BOX_HA:
      if (((CpuType == CPU_HSX) && (BoxInst < MAX_HSX_HA)) || ((CpuType == CPU_BDX) && (BoxInst < MAX_BDX_HA))) {
        Dev = 18;
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_IVT_HA)) {
        Dev = 14 * (BoxInst + 1); // IVT HA0 - Device 14; HA1 - Device 28
      } else {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "\nInvalid HA Box Instance %d. \n", BoxInst));
#endif // SERIAL_DBG_MSG
        RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
      }
      break;

    case BOX_R3QPI:
      if (((CpuType == CPU_HSX) && (BoxInst < MAX_HSX_R3QPI)) || ((CpuType == CPU_BDX) && (BoxInst < MAX_BDX_R3QPI))) {
        Dev = 11;
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_IVT_R3QPI)) {
        (BoxInst == 0) ? (Dev = 19) : (Dev = 18); // IVT R3QPI01 - Device 19; R3QPI23 -  Device 18
      } else {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "\nInvalid R3QPI Box Instance %d. \n", BoxInst));
#endif // SERIAL_DBG_MSG
        RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
      }
      break;

    case BOX_QPIAGENT:
      if (((CpuType == CPU_HSX) && (BoxInst < MAX_HSX_QPIAGENT)) || ((CpuType == CPU_BDX) && (BoxInst < MAX_BDX_QPIAGENT))) {
        //Dev = 8 + BoxInst; // QPIAgent0 - Device 8; QPIAgent1 - Device 9
        (Dev = 8 + BoxInst); // QPIAgent0 - Device 8; QPIAgent1 - Device 9;  QPIAgent2 - Device 10
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_IVT_QPIAGENT)) {
        (BoxInst < 2) ? (Dev = 8 + BoxInst) : (Dev = 24); // QPIAgent0 - Device 8; QPIAgent1 - Device 9;  QPIAgent2 - Device 24
      } else {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "\nInvalid QPIAgent Box Instance %d. \n", BoxInst));
#endif // SERIAL_DBG_MSG
        RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
      }
      break;

    case BOX_CBOBC:
      if (((CpuType == CPU_HSX) || (CpuType == CPU_BDX)) && (BoxInst < MAX_ALL_CBOBC) ) {
        Dev = 15;
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_ALL_CBOBC)) {
        Dev = 22;
      } else {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "\nInvalid CBOBC Box Instance %d. \n", BoxInst));
#endif // SERIAL_DBG_MSG
        RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
      }
      break;

    case BOX_R2PCIE:
      if (((CpuType == CPU_HSX) || (CpuType == CPU_BDX)) && (BoxInst < MAX_ALL_R2PCIE) ) {
        Dev = 16;
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_ALL_R2PCIE)) {
        Dev = 19;
      } else {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "\nInvalid R2PCIe Box Instance %d. \n", BoxInst));
#endif // SERIAL_DBG_MSG
        RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
      }
      break;

    case BOX_UBOX:
      if (((CpuType == CPU_HSX) || (CpuType == CPU_BDX)) && (BoxInst < MAX_ALL_UBOX) ) {
        Dev = 16;
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_ALL_UBOX)) {
        Dev = 11;
      } else {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "\nInvalid Ubox Instance %d. \n", BoxInst));
#endif // SERIAL_DBG_MSG
        RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
      }
      break;

    case BOX_IIO:
      if (((CpuType == CPU_HSX) || (CpuType == CPU_BDX)) && (BoxInst < MAX_ALL_IIO) ) {
        Dev = 5;
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_ALL_IIO)) {
        Dev = 5;
      } else {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "\nInvalid IIO Box Instance %d. \n", BoxInst));
#endif // SERIAL_DBG_MSG
        RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
      }
      break;

  case BOX_IIO_DFX:
      if (((CpuType == CPU_HSX) || (CpuType == CPU_BDX)) && (BoxInst < MAX_ALL_IIO_DFX) ) {
        if(BoxInst < 8){
          Dev = 6;
        } else {
          Dev = 7;
        }
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_ALL_IIO_DFX)) {
        if(BoxInst < 8){
          Dev = 6;
        } else {
          Dev = 7;
        }
      } else {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "\nInvalid IIO DFX Box Instance %d. \n", BoxInst));
#endif // SERIAL_DBG_MSG
        RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
      }
      break;

  case BOX_IIO_PCIE_DMI:
    if (((CpuType == CPU_HSX) || (CpuType == CPU_BDX)) && (BoxInst < MAX_ALL_IIO) ) {
      Dev = 0;
    } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_ALL_IIO)) {
      Dev = 0;
    } else {
#ifdef SERIAL_DBG_MSG
      rcPrintf ((host, "\nInvalid IIO PCIE DMI Box Instance %d. \n", BoxInst));
#endif // SERIAL_DBG_MSG
      RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
    }
    break;

    case BOX_PCU:
      if (((CpuType == CPU_HSX) || (CpuType == CPU_BDX)) && (BoxInst < MAX_ALL_PCU) ) {
        Dev = 30;
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_ALL_PCU)) {
        Dev = 10;
      } else {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "\nInvalid PCU Box Instance %d. \n", BoxInst));
#endif // SERIAL_DBG_MSG
        RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
      }
      break;

    case BOX_MC:
      switch (CpuType) {
      case CPU_BDX:
      case CPU_HSX:
        switch (CpuSubType) {
        case CPU_HSX_1HA:
          Dev = 19;
          break;

        case CPU_HSX_2HA:
          Dev = 19 + 3 * BoxInst;
          break;

        default:
#ifdef SERIAL_DBG_MSG
          rcPrintf ((host, "\nInvalid HSX CPU Sub type %d. \n", CpuSubType));
#endif // SERIAL_DBG_MSG
          RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
        } // switch (CpuSubType)
        break;

      case CPU_IVT:
        switch (CpuSubType) {
        case CPU_IVT_EX:
          Dev = 15 + 14 * BoxInst;
          break;

        default:
#ifdef SERIAL_DBG_MSG
          rcPrintf ((host, "\nInvalid IVT CPU Sub type %d. \n", CpuSubType));
#endif // SERIAL_DBG_MSG
          RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
        } // switch (CpuSubType)
        break;

      default:
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "\nInvalid Cpu %d. \n", CpuType));
#endif // SERIAL_DBG_MSG
        RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
      } // switch (CpuType)
      break;

    case BOX_MCDDC:
      if (CpuType == CPU_HSX || CpuType == CPU_BDX ) {
        switch (FuncBlk) {
          case MCDDC_CTL:
          case MCDDC_DP:
            switch (CpuType) {
            case CPU_BDX:
            case CPU_HSX:
              switch (CpuSubType) {
              case CPU_HSX_1HA:
                Dev = 20 + 1 * (BoxInst >> 1);
                break;

              case CPU_HSX_2HA:
                Dev = 20 + 3 * (BoxInst >> 1);
                break;

              default:
#ifdef SERIAL_DBG_MSG
                rcPrintf ((host, "\nInvalid HSX CPU Sub type %d. \n", CpuSubType));
#endif // SERIAL_DBG_MSG
                RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
              } // switch (CpuSubType)
              break;

            case CPU_IVT:
              switch (CpuSubType) {
              case CPU_IVT_EX:
                Dev = 16 + 14 * (BoxInst >> 2);
                break;

              default:
#ifdef SERIAL_DBG_MSG
                rcPrintf ((host, "\nInvalid IVT CPU Sub type %d. \n", CpuSubType));
#endif // SERIAL_DBG_MSG
                RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
              } // switch (CpuSubType)
              break;


            default:
#ifdef SERIAL_DBG_MSG
              rcPrintf ((host, "\nInvalid Cpu %d. \n", CpuType));
#endif // SERIAL_DBG_MSG
              RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
            } // switch (CpuType)
            break;

          case MCDDC_DESC:
            switch (CpuType) {
            case CPU_BDX:
            case CPU_HSX:
              switch (CpuSubType) {
              case CPU_HSX_1HA:
                Dev = 19;
                break;

              case CPU_HSX_2HA:
                Dev = 19 + 3 * (BoxInst >> 1);
                break;

              default:
#ifdef SERIAL_DBG_MSG
                rcPrintf ((host, "\nInvalid HSX CPU Sub type %d.\n", CpuSubType));
#endif // SERIAL_DBG_MSG
                RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
              } // switch (CpuSubType)
              break;

            case CPU_IVT:
              switch (CpuSubType) {
              case CPU_IVT_EX:
                Dev = 15 + 14 * (BoxInst >> 2);
                break;

              default:
#ifdef SERIAL_DBG_MSG
                rcPrintf ((host, "\nInvalid IVT CPU Sub type %d. \n", CpuSubType));
#endif // SERIAL_DBG_MSG
                RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
              } // switch (CpuSubType)
              break;

              default:
#ifdef SERIAL_DBG_MSG
                rcPrintf ((host, "\nInvalid Cpu %d. \n", CpuType));
#endif // SERIAL_DBG_MSG
                RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
            } // switch (CpuType)
            break;

          default:
#ifdef SERIAL_DBG_MSG
            rcPrintf ((host, "\nInvalid FuncBlk for BOX_MCDDC %d. \n", FuncBlk));
#endif // SERIAL_DBG_MSG
            RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
        }
      } else if (CpuType == CPU_IVT) {
        switch (FuncBlk) {
          case MCDDC_CTL:
          case MCDDC_DP:
            switch (CpuSubType) {
            case CPU_IVT_EX:
              Dev = 16 + 14 * (BoxInst >> 2);
              break;

            default:
#ifdef SERIAL_DBG_MSG
              rcPrintf ((host, "\nInvalid IVT CPU Sub type %d. \n", CpuSubType));
#endif // SERIAL_DBG_MSG
              RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
            } // switch (CpuSubType)

          case MCDDC_DESC:
            switch (CpuSubType) {
            case CPU_IVT_EX:
              Dev = 15 + 14 * (BoxInst >> 2);
              break;

            default:
#ifdef SERIAL_DBG_MSG
              rcPrintf ((host, "\nInvalid IVT CPU Sub type %d. \n", CpuSubType));
#endif // SERIAL_DBG_MSG
              RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
            } // switch (CpuSubType)
            break;

            default:
#ifdef SERIAL_DBG_MSG
              rcPrintf ((host, "\nInvalid Cpu %d. \n", CpuType));
#endif // SERIAL_DBG_MSG
              RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
        } // switch (CpuType)
      } else {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "\nInvalid Cpu %d. \n", CpuType));
#endif // SERIAL_DBG_MSG
        RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
      }
      break;

    case BOX_MCIO:
      if (CpuType == CPU_HSX || CpuType == CPU_BDX ) {
        switch (FuncBlk) {
          case MCIO_DDRIOGBC:
          case MCIO_DDRIOEXTGBC:
            Dev = 19;
            break;

          case MCIO_DDRIO:
          case MCIO_DDRIOEXT:
          case MCIO_DDRIOMCC:
          case MCIO_DDRIOEXTMCC:
            switch (CpuType) {
            case CPU_BDX:
            case CPU_HSX:
              switch (CpuSubType) {
              case CPU_HSX_1HA:
              case CPU_HSX_2HA:

                if (BoxInst < 2) {
                  Dev = 20;
                } else {
                  Dev = 23;
                }
                break;

              default:
#ifdef SERIAL_DBG_MSG
                rcPrintf ((host, "\nInvalid HSX CPU Sub type %d. \n", CpuSubType));
#endif // SERIAL_DBG_MSG
                RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
              } // switch (CpuSubType)
              break;

              default:
#ifdef SERIAL_DBG_MSG
                rcPrintf ((host, "\nInvalid Cpu %d. \n", CpuType));
#endif // SERIAL_DBG_MSG
                RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
            } // switch (CpuType)
            break;

          default:
#ifdef SERIAL_DBG_MSG
            rcPrintf ((host, "\nInvalid FuncBlk for BOX_MCIO %d. \n", FuncBlk));
#endif // SERIAL_DBG_MSG
            RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
        }
      } else if (CpuType == CPU_IVT) {
        switch (FuncBlk) {
          case MCIO_DDRIOGBC:
          case MCIO_DDRIOEXTGBC:
            Dev = 15;
            break;

          case MCIO_DDRIO:
          case MCIO_DDRIOEXT:
          case MCIO_DDRIOMCC:
          case MCIO_DDRIOEXTMCC:
            switch (CpuType) {
            case CPU_IVT:
              switch (CpuSubType) {
              case CPU_IVT_EX:
                Dev = 17 + 14 * (BoxInst >> 1);
                break;

              default:
#ifdef SERIAL_DBG_MSG
                rcPrintf ((host, "\nInvalid IVT CPU Sub type %d. \n", CpuSubType));
#endif // SERIAL_DBG_MSG
                RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
              } // switch (CpuSubType)
              break;

              default:
#ifdef SERIAL_DBG_MSG
                rcPrintf ((host, "\nInvalid Cpu %d. \n", CpuType));
#endif // SERIAL_DBG_MSG
                RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
            } // switch (CpuType)
            break;

          default:
#ifdef SERIAL_DBG_MSG
            rcPrintf ((host, "\nInvalid FuncBlk for BOX_MCIO %d. \n", FuncBlk));
#endif // SERIAL_DBG_MSG
            RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
        }
      } else {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "\nInvalid Cpu %d. \n", CpuType));
#endif // SERIAL_DBG_MSG
        RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
      }
      break;

    case BOX_VCU:
      if (CpuType == CPU_HSX || CpuType == CPU_BDX ) {
        Dev = 31;
      } else {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "\nVCU reg only present on HSX, not on %d. \n", CpuType));
#endif // SERIAL_DBG_MSG
        RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
      }
      break;

    case BOX_SBO:
      if (CpuType == CPU_HSX || CpuType == CPU_BDX ) {
        Dev = 15;
      } else {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "\nSBO reg only present on HSX, not on %d. \n", CpuType));
#endif // SERIAL_DBG_MSG
        RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
      }
      break;

    default:
#ifdef SERIAL_DBG_MSG
      rcPrintf ((host, "\nInvalid Box Type %d. \n", BoxType));
#endif // SERIAL_DBG_MSG
      RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
  }

  if (Dev > 31) {
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "\nInvalid Device %d accessed for Box Type %d and Box Instance %d. \n", Dev, BoxType, BoxInst));
#endif // SERIAL_DBG_MSG
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
  }
  return Dev;
}

UINT32
GetFunctionNumber (
  PSYSHOST host,
  UINT8    BoxType,
  UINT8    BoxInst,
  UINT8    FuncBlk
  )
/*++

  Indetifies the function number for given BoxType, BoxInst & Functional Block

  @param host      - Pointer to the system host (root) structure
  @param BoxType   - Box Type; values come from CpuPciAccess.h
  @param BoxInst   - Box Instance, 0 based
  @param FuncBlk   - Functional Block; values come from CpuPciAccess.h

  @retval PCI Function number

--*/
{
  UINT32   Fun = 0;
  UINT8    CpuType;
  UINT8    CpuIndex;

  // Get the CPU type
  CpuType   = host->var.common.cpuType;
  CpuIndex  = GetCpuIndex(host);

  //
  // Translate the Box Type & Functional Block into PCI function number. Note that the box type & instance number
  // passed to this routine are assumed to be valid; here we only need to validate if the function number is correct
  // after the look up is done.
  //

  switch (BoxType) {
    case BOX_CBO:
      //
      // For Cbo, no table look up is needed; the function number can be obtained from instance number.
      //
      if (((CpuType == CPU_HSX) && (BoxInst < MAX_HSX_CBO)) || ((CpuType == CPU_BDX) && (BoxInst < MAX_BDX_CBO))) {
        Fun = (BoxInst % 8);
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_IVT_CBO)) {
        Fun = (BoxInst / 2);
      }
      break;

    case BOX_HA:
      //
      // For HA, the function numbers are same for all instances
      //
      if (((CpuType == CPU_HSX) && (BoxInst < MAX_HSX_HA)) || ((CpuType == CPU_BDX) && (BoxInst < MAX_BDX_HA))) {
        Fun = FunTbl[CpuType][BoxType][FuncBlk] + (4 * BoxInst);
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_IVT_HA)) {
        Fun = FunTbl[CpuType][BoxType][FuncBlk];
      }
      break;

    case BOX_QPIAGENT:
      //
      // For QPIAGENT, instance 1 doesn't have function 7.
      //
      if (((CpuType == CPU_HSX) && (BoxInst < MAX_HSX_QPIAGENT)) || ((CpuType == CPU_BDX) && (BoxInst < MAX_BDX_QPIAGENT))) {
        Fun = FunTbl[CpuType][BoxType][FuncBlk];
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_IVT_QPIAGENT)) {
        Fun = FunTbl[CpuType][BoxType][FuncBlk];
      }

      if ((BoxInst == 1) && (Fun == 7)) {
        Fun = 0xFF;
      }
      break;

    //
    // All boxes that have only one instance go here
    //
    case BOX_CBOBC:
    case BOX_R2PCIE:
    case BOX_UBOX:
    case BOX_IIO:
    case BOX_IIO_PCIE:
    case BOX_IIO_PCIE_DMI:
    case BOX_IIO_PCIE_NTB:
    case BOX_VCU:
    case BOX_PCU:
      Fun = FunTbl[CpuType][BoxType][FuncBlk];
      break;

    case BOX_IIO_DFX:
      if((BoxInst < MAX_ALL_IIO_DFX) && (FunTbl[CpuType][BoxType][FuncBlk] != 0xFF)) {
        if(BoxInst < 8){
          Fun = BoxInst;    // Instances 0..7 map to Func 0..7
        } else {
          Fun = BoxInst - 8;// Instances 8..12 map to Func 0..4
        }
      } else {
        Fun = 0xFF;
      }
      break;

    case BOX_R3QPI:
      if (((CpuType == CPU_HSX) && (BoxInst < MAX_HSX_R3QPI)) || ((CpuType == CPU_BDX) && (BoxInst < MAX_BDX_R3QPI))) {
        Fun = FunTbl[CpuType][BoxType][FuncBlk] + (4 * BoxInst);
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_IVT_R3QPI)) {
        Fun = FunTbl[CpuType][BoxType][FuncBlk];
      }
      break;

    case BOX_MC:
      if (BoxInst < MAX_INST_IN_MC) {
        Fun = McFuncTbl[CpuIndex][FuncBlk][BoxInst];
      } else {
        Fun = 0xFF;
      }
      break;

    case BOX_MCDDC:
      if (BoxInst < MAX_INST_IN_MCDDC) {
        Fun = McddcFuncTbl[CpuIndex][FuncBlk][BoxInst];
      } else {
        Fun = 0xFF;
      }
      break;

    case BOX_MCIO:
      if (BoxInst < MAX_INST_IN_MCIO) {
        Fun = McioFuncTbl[CpuIndex][FuncBlk][BoxInst];
      } else {
        Fun = 0xFF;
      }
      break;

    case BOX_SBO:
      if (CpuType == CPU_HSX || CpuType == CPU_BDX ) {
        if (BoxInst < MAX_HSX_SBO) {
          Fun = FunTbl[CpuType][BoxType][FuncBlk] + BoxInst;
        } else {
          Fun = 0xFF;
        }
      } else {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "\nSBO reg only present on HSX, not on %d. \n", CpuType));
#endif // SERIAL_DBG_MSG
        RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
      }
      break;

    default:
#ifdef SERIAL_DBG_MSG
      rcPrintf ((host, "\nInvalid Box Type %d. \n", BoxType));
#endif // SERIAL_DBG_MSG
      RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
  }

  if (Fun > 7) {
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "\nInvalid Functional Block %d accessed for CPUType %d CPUIndex %d Box Type %d and Box Instance %d. \n",
              FuncBlk, CpuType, CpuIndex, BoxType, BoxInst));
#endif // SERIAL_DBG_MSG
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
  }

  return Fun;
}



UINT32
GetRegisterOffset (
  PSYSHOST host,
  CSR_OFFSET   RegOffset
  )
/*++

  Indetifies the read hardware register for given BoxType, unctional Block, & pseudo table offset

  @param host      - Pointer to the system host (root) structure
  @param Offset:  Bits <31:24> Bits <23:16>  Bit 15  Bits <14:12>  Bits <11:0>
                  Box Number   Func Number   Pseudo      Size      pseudo table Offset


  @retval Bits <31:24>  Bits <23:16>  Bit 15  Bits <14:12>  Bits <11:0>
          Box Number    Func Number       0       Size      PCICfg Offset

--*/
{
  UINT32   PseudoOffset, Reg = 0;
  UINT8    Error = 0, CpuType, BoxType, FuncBlk, CpuIdx;

  // Get the CPU type, pseduo table offset, Box Type, FunBlk
  CpuType = host->var.common.cpuType;

  if (CpuType == CPU_HSX) {
    CpuIdx = 2;
  } else if (CpuType == CPU_BDX){
    CpuIdx = 3;
  } else {
    CpuIdx = 1;
  }

  PseudoOffset = RegOffset.Bits.offset;
  BoxType = (UINT8)RegOffset.Bits.boxtype;
  FuncBlk = (UINT8)RegOffset.Bits.funcblk;


  //
  // Translate the Box Type & Functional Block into PCI function number. Note that the box type & instance number
  // passed to this routine are assumed to be valid; here we only need to validate if the function number is correct
  // after the look up is done.
  //

  switch (BoxType) {
    case BOX_QPIAGENT:
      switch (FuncBlk) {
            case QPI_LL:
                 Reg = QPIAGENT_LL_Offset[PseudoOffset][CpuIdx];
                 break;

            case QPI_PMON:
                 Reg = QPIAGENT_PMON_Offset[PseudoOffset][CpuIdx];
                 break;

            case QPI_DFX_PHY:
                 Reg = QPIAGENT_DFX_PHY_Offset[PseudoOffset][CpuIdx];
                 break;

            case QPI_DFX_LINK:
                 Reg = QPIAGENT_DFX_LINK_Offset[PseudoOffset][CpuIdx];
                 break;

            case QPI_DFX_COMMON:
                 Reg = QPIAGENT_DFX_COMMON_Offset[PseudoOffset][CpuIdx];
                 break;

            default:
                 Error = TRUE;
      }
      break;

    case BOX_R3QPI:
      switch (FuncBlk) {
            case R3QPI_CFG:
                 Reg = R3QPI_CFG_Offset[PseudoOffset][CpuIdx];
                 break;
            default:
                 Error = TRUE;
      }
      break;

    case BOX_IIO:
      switch (FuncBlk) {
            case IIO_VTD:
                 Reg = IIO_VTD_Offset[PseudoOffset][CpuIdx];
                 break;
            case IIO_PERFMON:
                 Reg = IIO_PERFMON_Offset[PseudoOffset][CpuIdx];
                 break;
            default:
                 Error = TRUE;
       }
     break;

    case BOX_IIO_DFX:
      switch (FuncBlk) {
            case IIO_DFX:
                 Reg = IIO_DFX_Offset[PseudoOffset][CpuIdx];
                 break;
            case IIO_DFX_GLOBAL:
                 Reg = IIO_DFX_GLOBAL_Offset[PseudoOffset][CpuIdx];
                 break;
            default:
                 Error = TRUE;
       }
     break;

    case BOX_PCU:
      switch (FuncBlk) {
//            case PCU_FUN0:
//                 Reg = PCU_FUN0_Offset[PseudoOffset][CpuIdx];
//                 break;
            case PCU_FUN1:
                 Reg = PCU_FUN1_Offset[PseudoOffset][CpuIdx];
                 break;
            default:
                 Error = TRUE;
      }
      break;

    case BOX_MCDDC:
      switch (FuncBlk) {
            case MCDDC_CTL:
                 Reg = MCDDC_CTL_Offset[PseudoOffset][CpuIdx];
                 break;
            default:
                 Error = TRUE;
      }
      break;

    case BOX_MC:
      switch (FuncBlk) {
            case MC_MAIN:
                 //Reg = MC_MAIN_Offset[PseudoOffset][CpuIdx];
                 break;
            default:
                 Error = TRUE;
      }
      break;

    default:
      Error = TRUE;
  }

  if (Error == TRUE) {
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "\nInvalid Pseudo Register Table for Box Type=%d, Functional Block=%d.\n", BoxType, FuncBlk));
#endif // SERIAL_DBG_MSG
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
  }

  return Reg;
}


UINT32
ReadCpuPciCfg (
  PSYSHOST host,
  UINT8    SocId,
  UINT8    BoxType,
  UINT8    BoxInst,
  UINT8    FuncBlk,
  UINT32   RegOffset
  )
/*++

  Reads CPU Uncore & IIO PCI configuration space using the MMIO mechanism

  @param host      - Pointer to the system host (root) structure
  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxType   - Box Type; values come from CpuPciAccess.h
  @param BoxInst   - Box Instance, 0 based
  @param FuncBlk   - Functional Block; values come from CpuPciAccess.h
  @param RegOffset - Register offset; values come from the auto generated header file

  @retval Register value

--*/
{
  UINT32  Bus, Dev, Fun;
  UINT32  Data = 0;
  UINT8   *RegAddr;


  //
  // Identify the PCI Bus/Device/Function number for the access
  //
  Bus = GetBusNumber (host, SocId, BoxType);
  Dev = GetDeviceNumber (host, BoxType, BoxInst, FuncBlk);
  Fun = GetFunctionNumber (host, BoxType, BoxInst, FuncBlk);

  //
  // Form address
  //
  RegAddr = (UINT8 *) (host->var.common.mmCfgBase | PCIE_REG_ADDR(Bus, Dev, Fun, RegOffset));

  //
  // Check register size and read data
  //
  switch (RegOffset >> 28) {
  case sizeof (UINT32):
    Data = *(volatile UINT32 *) RegAddr;
    break;

  case sizeof (UINT16):
    Data = *(volatile UINT16 *) RegAddr;
    break;

  case sizeof (UINT8):
    Data = *(volatile UINT8 *) RegAddr;
    break;

  default:
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
  }

#ifdef MRC_TRACE
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
#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_TRACE)) {
    rcPrintf ((host, "*%d, 1, 0x%x, 0, %d, ", SocId, RegAddr, (RegOffset >> 28)));

    //
    // Determine register size
    //
    switch (RegOffset >> 28) {
    case sizeof (UINT32):
      rcPrintf ((host, "0x%08x\n", (UINT32) Data));
      break;

    case sizeof (UINT16):
      rcPrintf ((host, "0x%04x\n", (UINT16) Data));
      break;

    case sizeof (UINT8):
      rcPrintf ((host, "0x%02x\n", (UINT8) Data));
      break;

    default:
      RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
    }
  }
#endif // SERIAL_DBG_MSG
  //
  // if SDBG_TRACE
  //
#endif

  return Data;
}


void
WriteCpuPciCfg (
  PSYSHOST host,
  UINT8    SocId,
  UINT8    BoxType,
  UINT8    BoxInst,
  UINT8    FuncBlk,
  UINT32   RegOffset,
  UINT32   Data
  )
/*++

  Writes CPU Uncore & IIO PCI configuration space using the MMIO mechanism

  @param host      - Pointer to the system host (root) structure
  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxType   - Box Type; values come from CpuPciAccess.h
  @param BoxInst   - Box Instance, 0 based
  @param FuncBlk   - Functional Block; values come from CpuPciAccess.h
  @param RegOffset - Register offset; values come from the auto generated header file
  @param Data      - Register data to be written

  @retval N/A

--*/
{
  UINT32  Bus, Dev, Fun;
  UINT8   *RegAddr;



  //
  // Identify the PCI Bus/Device/Function number for the access
  //
  Bus = GetBusNumber (host, SocId, BoxType);
  Dev = GetDeviceNumber (host, BoxType, BoxInst, FuncBlk);
  Fun = GetFunctionNumber (host, BoxType, BoxInst, FuncBlk);

  //
  // Form address
  //
  RegAddr = (UINT8 *) (host->var.common.mmCfgBase | PCIE_REG_ADDR(Bus, Dev, Fun, RegOffset));

  //
  // Check register size and write data
  //
  switch (RegOffset >> 28) {
  case sizeof (UINT32):
    *(volatile UINT32 *) RegAddr = Data;
    break;

  case sizeof (UINT16):
    *(volatile UINT16 *) RegAddr = (UINT16) Data;
    break;

  case sizeof (UINT8):
    *(volatile UINT8 *) RegAddr = (UINT8) Data;
    break;

  default:
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
  }

#ifdef SERIAL_DBG_MSG
#ifdef MRC_TRACE
  if (checkMsgLevel(host, SDBG_TRACE)) {
    if (!((RegOffset  == SR_POST_CODE_CSR)||(RegOffset  == SR_BIOS_SERIAL_DEBUG_CSR)||(RegOffset  == SMBCNTL_0_MC_MAIN_REG)||(RegOffset  == SMBCNTL_1_MC_MAIN_REG) \
         ||(RegOffset  == SMBCMD_0_MC_MAIN_REG)||(RegOffset  == SMBCMD_1_MC_MAIN_REG)||(RegOffset  == SR_PRINTF_SYNC_CSR) \
         ||(RegOffset == BIOSNONSTICKYSCRATCHPAD13_UBOX_MISC_REG) ||(RegOffset == BIOSNONSTICKYSCRATCHPAD14_UBOX_MISC_REG) ||(RegOffset == BIOSNONSTICKYSCRATCHPAD15_UBOX_MISC_REG) \
         ||(RegOffset == SYSTEMSEMAPHORE0_UBOX_MISC_REG) ||(RegOffset == SYSTEMSEMAPHORE1_UBOX_MISC_REG) )) {
    rcPrintf ((host, "*%d, 1, 0x%x, 1, %d, ", SocId, RegAddr, (RegOffset >> 28)));

    //
    // Determine register size
    //
    switch (RegOffset >> 28) {
    case sizeof (UINT32):
      rcPrintf ((host, "0x%08x\n", (UINT32) Data));
      break;

    case sizeof (UINT16):
      rcPrintf ((host, "0x%04x\n", (UINT16) Data));
      break;

    case sizeof (UINT8):
      rcPrintf ((host, "0x%02x\n", (UINT8) Data));
      break;

    default:
      RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
    }
    }
  }
  //
  // if SDBG_TRACE
  //
#endif
#endif // SERIAL_DBG_MSG


  return;
}


UINT8 *
GetCpuPciCfgAddress (
  PSYSHOST host,
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset,
  UINT8    *Size
  )
/*++

  Computes address of CPU Uncore & IIO PCI configuration space using the MMIO mechanism

  @param host      - Pointer to the system host (root) structure
  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based
  @param Offset    - Register offset; values come from the auto generated header file
  @param Size      - Register size in bytes (may be updated if pseudo-offset)

  @retval Address

--*/
{
  UINT32 Bus, Dev, Fun;
  UINT8 *RegAddr;
  UINT8 BoxType;
  UINT8 FuncBlk;
  CSR_OFFSET RegOffset;

  RegOffset.Data = Offset;

  BoxType = (UINT8)RegOffset.Bits.boxtype;
  FuncBlk = (UINT8)RegOffset.Bits.funcblk;

  if(RegOffset.Bits.pseudo) {
        RegOffset.Data = GetRegisterOffset (host, RegOffset);
        *Size = (UINT8)RegOffset.Bits.size;
  }

  //
  // Identify the PCI Bus/Device/Function number for the access
  //
  Bus = GetBusNumber (host, SocId, BoxType);
  Dev = GetDeviceNumber (host, BoxType, BoxInst, FuncBlk);
  Fun = GetFunctionNumber (host, BoxType, BoxInst, FuncBlk);

  //
  // Form address
  //
  RegAddr = (UINT8 *) (host->var.common.mmCfgBase | PCIE_REG_ADDR(Bus, Dev, Fun, RegOffset.Bits.offset));
  return RegAddr;
} // GetCpuPciCfgAddress



UINT32
ReadCpuPciCfgEx (
  PSYSHOST host,
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset
  )
/*++

  Reads CPU Uncore & IIO PCI configuration space using the MMIO mechanism

  @param host      - Pointer to the system host (root) structure
  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based
  @param RegOffset - Register offset; values come from the auto generated header file

  @retval Register value

--*/
{
  UINT32 Data = 0;
  UINT8 *RegAddr;
  UINT8 Size;
  CSR_OFFSET RegOffset;

  RegOffset.Data = Offset;

  Size = (UINT8)RegOffset.Bits.size;


  //
  // Form address
  //
  RegAddr = GetCpuPciCfgAddress (host, SocId, BoxInst, Offset, &Size);

  //
  // Check register size and read data
  //
  switch (Size) {
  case sizeof (UINT32):
    Data = *(volatile UINT32 *) RegAddr;
    break;

  case sizeof (UINT16):
    Data = *(volatile UINT16 *) RegAddr;
    break;

  case sizeof (UINT8):
    Data = *(volatile UINT8 *) RegAddr;
    break;

  default:
#ifdef SERIAL_DBG_MSG
     if (Offset != SR_PRINTF_SYNC_CSR) rcPrintf ((host, "\nInvalid Register Size in RegOffset = 0x%X\n", RegOffset.Data));
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
  if (  checkMsgLevel(host, SDBG_TRACE )  && (host->var.common.rcWriteRegDump) &&
         /* skip trace to avoid the infinite calls loop in
          * ...releasePrintFControl() -> WriteCpuPciCfgEx() ...*/
         (Offset != SR_PRINTF_SYNC_CSR)  && (Offset != SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR)
     ) {
    if (!((RegOffset.Data  == SR_POST_CODE_CSR)||(RegOffset.Data  == SR_BIOS_SERIAL_DEBUG_CSR)||(RegOffset.Data  == SMBCNTL_0_MC_MAIN_REG)||(RegOffset.Data  == SMBCNTL_1_MC_MAIN_REG)
    	 ||(RegOffset.Data  == SMBCMD_0_MC_MAIN_REG)||(RegOffset.Data  == SMBCMD_1_MC_MAIN_REG)||(RegOffset.Data  == SR_PRINTF_SYNC_CSR) || (RegOffset.Data == SYSTEMSEMAPHORE0_UBOX_MISC_REG)
         ||(RegOffset.Data == SYSTEMSEMAPHORE1_UBOX_MISC_REG))) {
    //
    // Determine register size
    //
    switch (Size) {
    case sizeof (UINT32):
      if (Offset != SR_PRINTF_SYNC_CSR) rcPrintf ((host, "*%d, 1, 0x%x, 0, %d, 0x%08x\n", SocId, RegAddr, Size, (UINT32) Data));
      break;

    case sizeof (UINT16):
      if (Offset != SR_PRINTF_SYNC_CSR) rcPrintf ((host, "*%d, 1, 0x%x, 0, %d, 0x%04x\n", SocId, RegAddr, Size, (UINT16) Data));
      break;

    case sizeof (UINT8):
      if (Offset != SR_PRINTF_SYNC_CSR) rcPrintf ((host, "*%d, 1, 0x%x, 0, %d, 0x%02x\n", SocId, RegAddr, Size, (UINT8) Data));
      break;

    default:
      if (Offset != SR_PRINTF_SYNC_CSR) rcPrintf ((host, "\nInvalid Register Size in RegOffset = 0x%X\n", RegOffset.Data));
      RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
    }

    }
  }
  //
  // if SDBG_TRACE
  //
#endif
#endif

  return Data;
}


void
WriteCpuPciCfgEx (
  PSYSHOST host,
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset,
  UINT32   Data
  )
/*++

  Writes CPU Uncore & IIO PCI configuration space using the MMIO mechanism

  @param host      - Pointer to the system host (root) structure
  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based
  @param Offset    - Register offset; values come from the auto generated header file
  @param Data      - Register data to be written

  @retval N/A

--*/
{
  UINT8 *RegAddr;
  UINT8 Size;
  CSR_OFFSET RegOffset;
#ifdef MRC_TRACE
  char *tag = "*";
#endif

  RegOffset.Data = Offset;

  Size = (UINT8)RegOffset.Bits.size;


  //
  // Form address
  //
  RegAddr = GetCpuPciCfgAddress (host, SocId, BoxInst, Offset, &Size);


  //
  // Check register size and write data
  //
  switch (Size) {
  case sizeof (UINT32):
    *(volatile UINT32 *) RegAddr = Data;
    break;

  case sizeof (UINT16):
    *(volatile UINT16 *) RegAddr = (UINT16) Data;
    break;

  case sizeof (UINT8):
    *(volatile UINT8 *) RegAddr = (UINT8) Data;
    break;

  default:
#ifdef SERIAL_DBG_MSG

     if (Offset != SR_PRINTF_SYNC_CSR) rcPrintf ((host, "\nInvalid Register Size in RegOffset = 0x%X\n", RegOffset.Data));
#endif
    RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
  }

#ifdef MRC_TRACE
#ifdef SERIAL_DBG_MSG
  //
  // skip printing Scratch pads, SMBUS, and system semaphores
  //
  if (checkMsgLevel(host, SDBG_TRACE | SDBG_RCWRITETAG)) {
    if (!(  (RegOffset.Data == SR_BIOS_SERIAL_DEBUG_CSR)                ||
            (RegOffset.Data == SMBCNTL_0_MC_MAIN_REG)                   ||
            (RegOffset.Data == SMBCNTL_1_MC_MAIN_REG)                   ||
            (RegOffset.Data == SMBCMD_0_MC_MAIN_REG)                    ||
            (RegOffset.Data == SMBCMD_1_MC_MAIN_REG)                    ||
            (RegOffset.Data == SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR)    ||
            (RegOffset.Data == SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR)||
            (RegOffset.Data == SR_PRINTF_SYNC_CSR) ||
            (RegOffset.Data == BIOSNONSTICKYSCRATCHPAD13_UBOX_MISC_REG) ||
            (RegOffset.Data == BIOSNONSTICKYSCRATCHPAD14_UBOX_MISC_REG) ||
            (RegOffset.Data == BIOSNONSTICKYSCRATCHPAD15_UBOX_MISC_REG) ||
            (RegOffset.Data == SYSTEMSEMAPHORE0_UBOX_MISC_REG) ||
            (RegOffset.Data == SYSTEMSEMAPHORE1_UBOX_MISC_REG) )) {

      if ( checkMsgLevel(host, SDBG_RCWRITETAG )) {
        tag = "RCREGWRITE:";

      }
      rcPrintf ((host, "\n%s%d, 1, 0x%x, 1, %d, ", tag, SocId, RegAddr, Size));

      //
      // Determine register size
      //
      switch (Size) {
        case sizeof (UINT32):
        rcPrintf ((host, "0x%08x\n", (UINT32) Data));
        break;

        case sizeof (UINT16):
        rcPrintf ((host, "0x%04x\n", (UINT16) Data));
        break;

        case sizeof (UINT8):
        rcPrintf ((host, "0x%02x\n", (UINT8) Data));
        break;

        default:
        rcPrintf ((host, "\nInvalid Register Size in RegOffset = 0x%X\n", RegOffset.Data));
        RC_ASSERT(FALSE, ERR_INVALID_REG_ACCESS, 0);
      }
    }
  }
  //
  // if SDBG_TRACE
  //
#endif
#endif



  return;
}

UINT8
IsDESku (
  PSYSHOST  host
  )
  /*++

    Checks if the processor is DE

    @param host - Pointer to the system host (root) structure

    @retval TRUE if DE SKU is detected
    @retval FALSE if not DE SKU detected.

  --*/
{

  return ((host->var.common.cpuType == CPU_BDX) && (host->var.common.procCom[0].capid0 & BIT5));

}//IsDESku

UINT8
IsDEHccSku (
  PSYSHOST  host
  )
  /*++

    Checks if the processor is DE-HCC

    @param host - Pointer to the system host (root) structure

    @retval TRUE if DE SKU is detected
    @retval FALSE if not DE SKU detected.

  --*/
{
  CAPID4_PCU_FUN3_BDX_STRUCT                       capid4;
  capid4.Data = host->var.common.procCom[0].capid4;
  return ((host->var.common.cpuType == CPU_BDX) && (host->var.common.procCom[0].capid0 & BIT5) && (capid4.Bits.physical_chop == 5));

}//IsDEHccSku

UINT8
IsBDXEP (
  PSYSHOST  host
  )
  /*++

    Checks if the processor is BDX EP

    @param host - Pointer to the system host (root) structure

    @retval TRUE if BDX EP SKU is detected
    @retval FALSE if not BDX EP SKU detected.

  --*/
{

  return ((host->var.common.cpuType == CPU_BDX) && (!IsDESku(host)));

}//IsBDXEP


UINT8
IsMccChop (
  PSYSHOST  host
  )
  /*++
  
    Checks if the processor is EP MCC L0
  
    @param host - Pointer to the system host (root) structure
  
    @retval TRUE if BDX EP L0 SKU is detected
    @retval FALSE if not BDX EP L0 SKU detected.
  
  --*/
{
  CAPID4_PCU_FUN3_BDX_STRUCT                       capid4;
  capid4.Data = host->var.common.procCom[0].capid4;
  return (capid4.Bits.physical_chop == 1);

}//IsMccChop

UINT8
IsHccChop (
  PSYSHOST  host
  )
  /*++

    Checks if the processor is EP HCC

    @param host - Pointer to the system host (root) structure

    @retval TRUE if BDX EP HCC SKU is detected
    @retval FALSE if not BDX EP HCC SKU detected.

  --*/
{
  CAPID4_PCU_FUN3_BDX_STRUCT                       capid4;
  capid4.Data = host->var.common.procCom[0].capid4;
  return (capid4.Bits.physical_chop == 2);

}//IsHccChop


UINT8
Is2HA (
  PSYSHOST  host
  )
  /*++

  Test if the system is 2HA

  @param host    - Pointer to the system host (root) structure

  @retval TRUE if 2HA
  @retval FALSE if not 2HA

  --*/
{

  BOOLEAN                       isHsx2HA;
  BOOLEAN                       isBdx2HA;

  isHsx2HA = FALSE;
  isBdx2HA = FALSE;

  isHsx2HA = ((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType == CPU_HSX_2HA));
  isBdx2HA = ((host->var.common.cpuType == CPU_BDX) && (host->var.common.cpuSubType == CPU_BDX_2HA));
  return (isHsx2HA || isBdx2HA);
}//Is2HA

UINT8
Is1HA (
  PSYSHOST  host
  )
  /*++

  Test if the system is 1HA

  @param host    - Pointer to the system host (root) structure

  @retval TRUE if 1HA
  @retval FALSE if not 1HA

  --*/
{

  BOOLEAN                       isHsx1HA;
  BOOLEAN                       isBdx1HA;

  isHsx1HA = FALSE;
  isBdx1HA = FALSE;

  isHsx1HA = ((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType == CPU_HSX_1HA));
  isBdx1HA = ((host->var.common.cpuType == CPU_BDX) && (host->var.common.cpuSubType == CPU_BDX_1HA));
  return (isHsx1HA || isBdx1HA);
}//Is1HA

UINT8
GetLegacyBusNumber(
  VOID
  )
/*++
  Get the Legacy Bus Number

  @param VOID

  @retval Legacy Bus Number (Assumption is legacy socket will always be on bus 0)
--*/
{
  // Assumption is legacy socket will always be on bus 0.
  return 0;
}

UINT32
GetMmcfgAddressHSX(
   UINT8 Flag
  )
/*++

  Get MMCFG base and limit

  @param Flag      - 0 (Base, 1 - Limit)

  @retval Value

--*/
{
   PSYSHOST Host;
   Host = NULL;
   return GetMmcfgAddress( Host, Flag);
}

UINT32
GetMmcfgAddress(
  PSYSHOST Host,
  UINT8 Flag
  )
/*++

  Get MMCFG base and limit

  @param Flag      - 0 (Base, 1 - Limit)

  @retval Value

--*/
{
  UINT32 MmcfgAddress = 0;
  UINT8 IioBusNum = 0;
  UINT32 CPUType;
  UINT8 Stepping;
  UINT32 RegEax;
  UINT32 RegEbx, RegEcx, RegEdx;

  //
  // Use CF8/CFC to read legacy socket's IIO MMCFG CSR at bus0:dev5:Func0:0x84  and 0x88.
  //
  IioBusNum = GetLegacyBusNumber();

  RegEax = 1; RegEcx = 0;
  GetCpuID(&RegEax, &RegEbx, &RegEcx, &RegEdx);
  CPUType = (UINT32)(RegEax >> 4);
  Stepping = (UINT8)(RegEax & 0x0F);

  if (Flag == 1) {
    //
    // Get MmcfgLimit
    //
    if ((CPUType == CPU_FAMILY_HSX && Stepping >= B0_REV) || (CPUType == CPU_FAMILY_BDX) || (CPUType == CPU_FAMILY_BDX_DE))
      OutPort32 (NULL, 0x0cf8, (0x80000000 | (IioBusNum << 16) |  (5 << 11)  | (0 << 8) | 0x98));
    else
      OutPort32 (NULL, 0x0cf8, (0x80000000 | (IioBusNum << 16) |  (5 << 11)  | (0 << 8) | 0x88));
    MmcfgAddress = InPort32 (NULL, 0x0cfc);
    MmcfgAddress &= 0xfc000000;              // Mask Bits[25:0]
    MmcfgAddress |= 0x3FFFFFF;
  } else {
    //
    // Get MmcfgBase
    //
    if ((CPUType == CPU_FAMILY_HSX && Stepping >= B0_REV) || (CPUType == CPU_FAMILY_BDX) || (CPUType == CPU_FAMILY_BDX_DE))
      OutPort32 (NULL, 0x0cf8, (0x80000000 | (IioBusNum << 16) |  (5 << 11)  | (0 << 8) | 0x90));
    else
      OutPort32 (NULL, 0x0cf8, (0x80000000 | (IioBusNum << 16) |  (5 << 11)  | (0 << 8) | 0x84));
    MmcfgAddress = InPort32 (NULL, 0x0cfc);
    MmcfgAddress &= 0xfc000000;              // Mask Bits[25:0]
  }
  return MmcfgAddress;
}

UINT8
GetSbspSktId (
  PSYSHOST host
  )
/*++

Routine Description:

  Get CPU socket id for the system boot strap processor

Arguments:

  host  - Pointer to the system host (root) structure

Returns:

  system boot strap processor socket ID

--*/
{
   return host->nvram.common.sbspSocketId;
}
