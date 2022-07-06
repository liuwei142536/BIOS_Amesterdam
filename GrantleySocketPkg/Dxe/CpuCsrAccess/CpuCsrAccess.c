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
 * Copyright 2006 - 2012 Intel Corporation All Rights Reserved.
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

#include "CpuCsrAccess.h"
//
//4987134 start
//

// Virtual address map runtime service
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/EventGroup.h>

//
//4987134 end
//
#pragma optimize("",off)

EFI_CPU_CSR_ACCESS_PROTOCOL     mCpuCsrAccessProtocol;
EFI_IIO_UDS_PROTOCOL            *mIioUds;
IIO_UDS                        *mIioUdsData;

EFI_SMM_BASE2_PROTOCOL          *mSmmBase = NULL;
EFI_SMM_SYSTEM_TABLE2           *mSmst = NULL;


#define PCIE_REG_ADDR(Bus,Device,Function,Offset) \
  (((Offset) & 0xfff) | (((Function) & 0x07) << 12) | (((Device) & 0x1f) << 15) | (((Bus) & 0xff) << 20))

UINT32 socketPresentBitMap;
UINT8 busIio[MAX_SOCKET];
UINT8 busUncore[MAX_SOCKET];
//
////4987134 start
//

// support virtual addressing
UINTN mmCfgBase;

//
////4987134 end
//
UINT8 CpuType;
UINT8 CpuSubType;

//
// PCI function translation table; note that this table doesn't capture the function
// information for all instnaces of a box. It only captures for the first instance.
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
          {0,    1,    2,    0xFF, 4,    0xFF, 6,    0xFF}, // IIO
          {0,    1,    2,    3,    4,    5,    6,    7   }, // PCU
          {0,    1,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // MC
          {0,    1,    0,    1   , 0xFF, 0xFF, 0xFF, 0xFF}, // MCDDC
          {4,    5,    4,    5,    6,    7,    0xFF, 0xFF}, // MCIO
          {0   , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO PCIE DMI
          {0   , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO PCIE
          {0   , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO PCIE NTB
          {0   , 1   , 2   , 3   , 4   , 5   , 6   , 7   }, // IIO CB
          {0,    1,    2,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO DFX
          {0,    0xFF, 2,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // VCU
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
          {0,    1,    2,    0xFF, 4,    0xFF, 6,    0xFF}, // IIO
          {0,    1,    2,    3,    4,    5,    6,    7   }, // PCU
          {0,    1,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // MC
          {4,    6,    2,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // MCDDC
          {6,    7,    4,    5,    6,    7,    0xFF, 0xFF}, // MCIO
          {0,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO PCIE_DMI
          {0,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO PCIE_RP
          {0,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO PCIE_NTB
          {0,    1,    2,    3,    4,    5,    6,    7   }, // IIO CB
          {0,    1,    2,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO DFX
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
          {0,    1,    2,    0xFF, 4,    0xFF, 6,    0xFF}, // IIO
          {0,    1,    2,    3,    4,    5,    6,    7   }, // PCU
          {0,    1,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // MC
          {0,    1,    0,    1   , 0xFF, 0xFF, 0xFF, 0xFF}, // MCDDC
          {4,    5,    4,    5,    6,    7,    0xFF, 0xFF}, // MCIO
          {0   , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO PCIE DMI
          {0   , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO PCIE
          {0   , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO PCIE NTB
          {0   , 1   , 2   , 3   , 4   , 5   , 6   , 7   }, // IIO CB
          {0,    1,    2,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO DFX
          {0,    0xFF, 2,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // VCU
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
        {0,   1,    0,    1,    0,   1,    0,    1},        // MCDDC_CTL
        {2,   3,    2,    3,    2,   3,    2,    3},        // MCDDC_DP
        {2,   3,    4,    5,    2,   3,    4,    5},        // MCDDC_DESC
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

//
////4987134 start 
//

//
// Event for Runtime Address Conversion.
//
EFI_EVENT             mVirtualAddressChangeEvent;

VOID
RuntimeCpuCsrAccessAddressChangeEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
/*++
	RuntimeCpuCsrAccessAddressChangeEvent

Routine Description:
  Fixup internal data pointers so that the services can be called in virtual mode.

Arguments:
  Event                   The event registered.
  Context                 Event context. Not used in this event handler.

Returns:
  None

--*/
{
  gRT->ConvertPointer (0x04, (VOID *) &(mmCfgBase));   
  gRT->ConvertPointer (0x04, (VOID *) &(mIioUdsData));   
}

//
////4987134 end
//

STATIC
UINT8
GetCpuIndex (
  VOID
  )
/*++

Routine Description:
  Returns the CPU Index for MC func tbl lookup based on CPU type and CPU sub type.
  This index will be used for MC box instance -> function mapping look-up

Arguments:

Returns:
  Index for CPU type

--*/
{
  UINT8  cpuIndex = 0xFF;

  switch(CpuType){
  case CPU_HSX:
    switch(CpuSubType){
    case CPU_HSX_1HA:
      cpuIndex = IDX_CPU_HSX_1HA;
      break;

    case CPU_HSX_2HA:
      cpuIndex = IDX_CPU_HSX_2HA;
      break;

    default:
#ifdef SERIAL_DBG_MSG
      DEBUG ((EFI_D_ERROR, "Invalid HSX CPU Sub type %d.\n", CpuSubType));
#endif
      CpuDeadLoop(); //AptioV server override: Added to resolve build error when SERIAL_DBG_MSG is not defined.  
    }
    break;
  case CPU_BDX:
      switch(CpuSubType){
      case CPU_BDX_1HA:
        cpuIndex = IDX_CPU_BDX_1HA;
        break;

      case CPU_BDX_2HA:
        cpuIndex = IDX_CPU_BDX_2HA;
        break;

      default:
#ifdef SERIAL_DBG_MSG
      DEBUG ((EFI_D_ERROR, "Invalid BDX CPU Sub type %d.\n", CpuSubType));
#endif
      CpuDeadLoop(); //AptioV server override: Added to resolve build error when SERIAL_DBG_MSG is not defined.  
    }
    break;
    
  case CPU_IVT:
    switch(CpuSubType){
    case CPU_IVT_EX:
      cpuIndex = IDX_CPU_IVT_EX;
      break;

    default:
      DEBUG ((EFI_D_ERROR, "Invalid IVT CPU Sub type  %d. \n", CpuSubType));
      CpuDeadLoop();
    }
    break;

  default:
    DEBUG ((EFI_D_ERROR, "Invalid Cpu Type  %d. \n", CpuType));
    CpuDeadLoop();
  }

  return cpuIndex;
}

STATIC
UINT32
GetBusNumber (
  UINT8    SocId,
  UINT8    BoxType
  )
/*++

Routine Description:
  Indetifies the bus number for given SocId & BoxType

Arguments:
  SocId     - CPU Socket Node number (Socket ID)
  BoxType   - Box Type; values come from CpuPciAccess.h

Returns:
  PCI bus number

--*/
{
  UINT32   Bus;

  socketPresentBitMap = mIioUdsData->SystemStatus.socketPresentBitMap;
  busIio[SocId] = mIioUdsData->PlatformData.IioQpiInfo[SocId].BusNum;
  busUncore[SocId] = mIioUdsData->PlatformData.CpuQpiInfo[SocId].BusNum;


  // Make sure SocId is valid
  if (!(socketPresentBitMap & (1 << SocId))) {
    DEBUG ((EFI_D_ERROR, "Invalid Socket Id %d. \n", SocId));
    CpuDeadLoop();
  }

  //
  // Each socket is assigned two buses - one for all Uncore devices and one for all IIO devices.
  // Check the box type and return the appropriate bus number.
  //
  if ((BoxType == BOX_IIO)          ||
      (BoxType == BOX_IIO_PCIE_DMI) ||
      (BoxType == BOX_IIO_PCIE_NTB) ||
      (BoxType == BOX_IIO_PCIE_RP)  ||
      (BoxType == BOX_IIO_DFX)      ||
      (BoxType == BOX_IIO_CB)) {

    Bus = busIio[SocId];
  } else {
    Bus = busUncore[SocId];
  }

  return Bus;
}

UINT32
GetDeviceNumber (
  UINT8    BoxType,
  UINT8    BoxInst,
  UINT8    FuncBlk
  )
/*++

Routine Description:
  Indetifies the device number for given Box Type & Box Instance

Arguments:
  BoxType   - Box Type; values come from CpuPciAccess.h
  BoxInst   - Box Instance, 0 based
  FuncBlk   - Functional Block; values come from CpuPciAccess.h

Returns:
  PCI Device number

--*/
{
  UINT32   Dev = 0;

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
        DEBUG ((EFI_D_ERROR, "Invalid CBo Box Instance Number %d. \n", BoxInst));
        CpuDeadLoop();
      }
      break;

    case BOX_HA:
      if (((CpuType == CPU_HSX) && (BoxInst < MAX_HSX_HA)) || ((CpuType == CPU_BDX) && (BoxInst < MAX_BDX_HA))) {
        Dev = 18;
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_IVT_HA)) {
        Dev = 14 * (BoxInst + 1); // IVT HA0 - Device 14; HA1 - Device 28
      } else {
        DEBUG ((EFI_D_ERROR, "Invalid HA Box Instance Number %d. \n", BoxInst));
        CpuDeadLoop();
      }
      break;

    case BOX_R3QPI:
      if (((CpuType == CPU_HSX) && (BoxInst < MAX_HSX_R3QPI)) || ((CpuType == CPU_BDX) && (BoxInst < MAX_BDX_R3QPI))) {
        Dev = 11;
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_IVT_R3QPI)) {
        (BoxInst == 0) ? (Dev = 19) : (Dev = 18); // IVT R3QPI01 - Device 19; R3QPI23 -  Device 18
      } else {
        DEBUG ((EFI_D_ERROR, "Invalid R3QPI Box Instance Number %d. \n", BoxInst));
        CpuDeadLoop();
      }
      break;

    case BOX_QPIAGENT:
      if (((CpuType == CPU_HSX) && (BoxInst < MAX_HSX_QPIAGENT)) || ((CpuType == CPU_BDX) && (BoxInst < MAX_BDX_QPIAGENT))) {
        //Dev = 8 + BoxInst; // QPIAgent0 - Device 8; QPIAgent1 - Device 9
        (Dev = 8 + BoxInst); // QPIAgent0 - Device 8; QPIAgent1 - Device 9;  QPIAgent2 - Device 10
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_IVT_QPIAGENT)) {
        (BoxInst < 2) ? (Dev = 8 + BoxInst) : (Dev = 24); // QPIAgent0 - Device 8; QPIAgent1 - Device 9;  QPIAgent2 - Device 24
      } else {
        DEBUG ((EFI_D_ERROR, "Invalid QPIAgent Box Instance Number %d. \n", BoxInst));
        CpuDeadLoop();
      }
      break;

    case BOX_CBOBC:
      if (((CpuType == CPU_HSX) || (CpuType == CPU_BDX)) && (BoxInst < MAX_ALL_CBOBC) ) {
        Dev = 15;
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_ALL_CBOBC)) {
        Dev = 22;
      } else {
        DEBUG ((EFI_D_ERROR, "Invalid CBOBC Box Instance Number %d. \n", BoxInst));
        CpuDeadLoop();
      }
      break;

    case BOX_R2PCIE:
      if (((CpuType == CPU_HSX) || (CpuType == CPU_BDX)) && (BoxInst < MAX_ALL_R2PCIE) ) {
        Dev = 16;
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_ALL_R2PCIE)) {
        Dev = 19;
      } else {
        DEBUG ((EFI_D_ERROR, "Invalid R2PCIe Box Instance Number %d. \n", BoxInst));
        CpuDeadLoop();
      }
      break;

    case BOX_UBOX:
      if (((CpuType == CPU_HSX) || (CpuType == CPU_BDX)) && (BoxInst < MAX_ALL_UBOX) ) {
        Dev = 16;
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_ALL_UBOX)) {
        Dev = 11;
      } else {
        DEBUG ((EFI_D_ERROR, "Invalid Ubox Instance Number %d. \n", BoxInst));
        CpuDeadLoop();
      }
      break;

    case BOX_IIO:
      if (((CpuType == CPU_HSX) || (CpuType == CPU_BDX)) && (BoxInst < MAX_ALL_IIO) ) {
        Dev = 5;
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_ALL_IIO)) {
        Dev = 5;
      } else {
        DEBUG ((EFI_D_ERROR, "Invalid IIO Box Instance Number %d. \n", BoxInst));
        CpuDeadLoop();
      }
      break;

    case BOX_IIO_PCIE_DMI:
      if (((CpuType == CPU_HSX) || (CpuType == CPU_BDX)) && (BoxInst < MAX_ALL_IIO)) {
        Dev = 0;
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_ALL_IIO)) {
        Dev = 0;
      } else {
        DEBUG ((EFI_D_ERROR, "BOX_IIOPCIE_DMI - Invalid CpuType %d. \n", CpuType));
        CpuDeadLoop();
      }
      break;

    case BOX_IIO_PCIE_RP:
      if ((CpuType == CPU_HSX) || (CpuType == CPU_BDX) || (CpuType == CPU_IVT)) {
        switch(BoxInst){
        case 0:         //box instance 0 for PCIe port 0 [D0,F0]
          Dev = 0;
          break;
        case 1:         //box instance 1 for PCIe port 1A [D1,F0]
        case 2:         //box instance 2 for PCIe port 1B [D1,F1]
          Dev = 1;
          break;
        case 3:         //box instance 3 for PCIe port 2A [D2,F0]
        case 4:         //box instance 4 for PCIe port 2B [D2,F1]
        case 5:         //box instance 5 for PCIe port 2C [D2,F2]
        case 6:         //box instance 6 for PCIe port 2D [D2,F3]
          Dev = 2;
          break;
        case 7:         //box instance 7 for PCIe port 3A [D3,F0]
        case 8:         //box instance 8 for PCIe port 3B [D3,F1]
        case 9:         //box instance 9 for PCIe port 3C [D3,F2]
        case 10:        //box instance 10 for PCIe port 3D [D3,F3]
          Dev = 3;
          break;
        default:
          DEBUG ((EFI_D_ERROR, "BOX_IIOPCIE_RP - Unsupported port index #%d for CpuType %d. \n", BoxInst, CpuType));
          CpuDeadLoop();
          break;
        }
      } else {
        DEBUG ((EFI_D_ERROR, "BOX_IIOPCIE_RP - Invalid CpuType %d. \n", CpuType));
        CpuDeadLoop();
      }
      break;

    case BOX_IIO_PCIE_NTB:
      if ((CpuType == CPU_HSX) || (CpuType == CPU_BDX) || (CpuType == CPU_IVT)) {
        Dev = 3;
      } else {
        DEBUG ((EFI_D_ERROR, "BOX_IIOPCIE_NTB - Invalid CpuType %d. \n", CpuType));
        CpuDeadLoop();
      }
      break;

    case BOX_IIO_CB:
      if ((CpuType == CPU_HSX) || (CpuType == CPU_BDX) || (CpuType == CPU_IVT)) {
        Dev = 4;
      } else {
        DEBUG ((EFI_D_ERROR, "BOX_IIOCB - Invalid CpuType %d. \n", CpuType));
        CpuDeadLoop();
      }
      break;

    case BOX_IIO_DFX:
      if ((CpuType == CPU_HSX) || (CpuType == CPU_BDX) || (CpuType == CPU_IVT)) {
        switch(FuncBlk){
        case  IIO_DFX:          // Functional block for [D6,F0-6], [D7,F0-3]
          Dev = (BoxInst < 7) ? 6 : 7;
          break;
        case  IIO_DFX_GLOBAL:   // Functional block for [D6, F7]
          Dev = 6;
          break;
        case  IIO_DFX_VTD:      // Functional block for [D7, F4]
          Dev = 7;
          break;
        default:
          DEBUG ((EFI_D_ERROR, "Unsupported/invalid functional block #%d found for CpuType %d. \n", FuncBlk, CpuType));
          CpuDeadLoop();
          break;
        }
      } else {
        DEBUG ((EFI_D_ERROR, "BOX_IIOCB - Invalid CpuType %d. \n", CpuType));
        CpuDeadLoop();
      }
      break;

    case BOX_PCU:
      if (((CpuType == CPU_HSX) || (CpuType == CPU_BDX)) && (BoxInst < MAX_ALL_PCU) ) {
        Dev = 30;
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_ALL_PCU)) {
        Dev = 10;
      } else {
        DEBUG ((EFI_D_ERROR, "Invalid PCU Box Instance Number %d. \n", BoxInst));
        CpuDeadLoop();
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
            DEBUG ((EFI_D_ERROR, "\nInvalid HSX CPU Sub type %d. \n", CpuSubType));
            CpuDeadLoop();
        } // switch (CpuSubType)
        break;

      case CPU_IVT:
        switch (CpuSubType) {
        case CPU_IVT_EX:
          Dev = 15 + 14 * BoxInst;
          break;

        default:
          DEBUG ((EFI_D_ERROR, "Invalid IVT CPU Sub type %d. \n", CpuSubType));
          CpuDeadLoop();
        } // switch (CpuSubType)
        break;

      default:
        DEBUG ((EFI_D_ERROR, "Invalid Cpu %d. \n", CpuType));
        CpuDeadLoop();
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
                    Dev = 20 + (1 * (BoxInst >> 1)) + (1 * (BoxInst >> 2));
                    break;

                  default:
                    DEBUG ((EFI_D_ERROR, "\nInvalid HSX CPU Sub type %d. \n", CpuSubType));
                    CpuDeadLoop();
                } // switch (CpuSubType)
                break;

              case CPU_IVT:
                switch (CpuSubType) {
                  case CPU_IVT_EX:
                    Dev = 16 + 14 * (BoxInst >> 2);
                    break;
                  default:
                    DEBUG ((EFI_D_ERROR, "\nInvalid IVT CPU Sub type %d. \n", CpuSubType));
                    CpuDeadLoop();
                } // switch (CpuSubType)
              break;


              default:
                DEBUG ((EFI_D_ERROR, "\nInvalid Cpu %d. \n", CpuType));
                CpuDeadLoop();
            } //switch (CpuType)
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
                Dev = 19 + 3 * (BoxInst >> 2);
                break;

              default:
                DEBUG ((EFI_D_ERROR, "\nInvalid HSX CPU Sub type %d. \n", CpuSubType));
                CpuDeadLoop();
                break;
              } //swich (CpuSuType)
              break;

            case CPU_IVT:
              switch (CpuSubType) {
              case CPU_IVT_EX:
                Dev = 15 + 14 * (BoxInst >> 2);
                break;

              default:
                  DEBUG ((EFI_D_ERROR, "\nInvalid IVT CPU Sub type %d. \n", CpuSubType));
                  CpuDeadLoop();
              }  // switch (CpuSubType)
                break;

              default:
                  DEBUG ((EFI_D_ERROR, "\nInvalid Cpu %d. \n", CpuType));
                  CpuDeadLoop();
              } //switch(CpuType)
              break;

          default:
              DEBUG ((EFI_D_ERROR, "\nInvalid FunncBlk for BOX_MCDDC %d. \n", FuncBlk));
              CpuDeadLoop();
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
                DEBUG ((EFI_D_ERROR, "Invalid IVT CPU Sub type %d. \n", CpuSubType));
                CpuDeadLoop();
            } // switch (CpuSubType)

          case MCDDC_DESC:
            switch (CpuSubType) {
            case CPU_IVT_EX:
              Dev = 15 + 14 * (BoxInst >> 2);
              break;

            default:
              DEBUG ((EFI_D_ERROR, "Invalid IVT CPU Sub type %d. \n", CpuSubType));
              CpuDeadLoop();
            } // switch (CpuType)

          default:
            DEBUG ((EFI_D_ERROR, "Invalid Cpu %d. \n", CpuType));
            CpuDeadLoop();
          } // switch (CpuType)

      } else {
              DEBUG ((EFI_D_ERROR, "Invalid Cpu %d. \n", CpuType));
              CpuDeadLoop();
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
                DEBUG ((EFI_D_ERROR, "Invalid HSX CPU Sub type %d. \n", CpuSubType));
                CpuDeadLoop();
              } // switch (CpuSubType)
              break;

              default:
                DEBUG ((EFI_D_ERROR, "Invalid Cpu %d. \n", CpuType));
                CpuDeadLoop();
            } // switch (CpuType)
            break;

          default:
            DEBUG ((EFI_D_ERROR, "Invalid FuncBlk for BOX_MCIO %d. \n", FuncBlk));
            CpuDeadLoop();
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
                DEBUG ((EFI_D_ERROR, "Invalid IVT CPU Sub type %d. \n", CpuSubType));
                CpuDeadLoop();
              } // switch (CpuSubType)
              break;

              default:
                DEBUG ((EFI_D_ERROR, "Invalid Cpu %d. \n", CpuType));
                CpuDeadLoop();
            } // switch (CpuType)
            break;

          default:
            DEBUG ((EFI_D_ERROR, "Invalid FuncBlk for BOX_MCIO %d. \n", FuncBlk));
            CpuDeadLoop();
        }
      } else {
        DEBUG ((EFI_D_ERROR, "Invalid Cpu %d. \n", CpuType));
        CpuDeadLoop();
      }
      break;
    
    case BOX_VCU:
      if (CpuType == CPU_HSX || CpuType == CPU_BDX ) {
        Dev = 31;
      } else {
        DEBUG ((EFI_D_ERROR, "\nVCU reg only present on HSX, not on %d. \n", CpuType));
        CpuDeadLoop();
      }
      break;
      
    case BOX_SBO:
      if (CpuType == CPU_HSX || CpuType == CPU_BDX ) {
        Dev = 15;
      } else {
        DEBUG ((EFI_D_ERROR, "\nSBO reg only present on HSX, not on %d. \n", CpuType));
        CpuDeadLoop();
      }
      break;
    
    default:
      DEBUG ((EFI_D_ERROR, "Invalid Box Type %d. \n", BoxType));
      CpuDeadLoop();
  }

  if (Dev > 31) {
    DEBUG ((EFI_D_ERROR, "Invalid Device %d accessed for Box Type %d and Box Instance %d. \n", Dev, BoxType, BoxInst));
    CpuDeadLoop();

  }
  return Dev;
}

UINT32
GetFunctionNumber (
  UINT8    BoxType,
  UINT8    BoxInst,
  UINT8    FuncBlk
  )
/*++

Routine Description:
  Indetifies the function number for given BoxType, BoxInst & Functional Block

Arguments:
  BoxType   - Box Type; values come from CpuPciAccess.h
  BoxInst   - Box Instance, 0 based
  FuncBlk   - Functional Block; values come from CpuPciAccess.h

Returns:
  PCI Function number

--*/
{
  UINT32   Fun = 0;
  UINT8    CpuIndex;

  // Get the CPU type
  CpuIndex  = GetCpuIndex();

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
    case BOX_IIO_CB:
    case BOX_IIO_PCIE_DMI:
      if(BoxInst < 1){
        Fun = FunTbl[CpuType][BoxType][FuncBlk];
      } else {
        Fun = 0xFF;
      }
      break;

    case BOX_IIO_PCIE_RP:
      if((BoxInst < MAX_ALL_IIOPCIE_PCIERP) && (FunTbl[CpuType][BoxType][FuncBlk] != 0xFF)){
        switch(BoxInst){
        case 0:         //box instance 0 for PCIe port 0 [D0,F0]
          Fun = 0;
          break;
        case 1:         //box instance 1 for PCIe port 1A [D1,F0]
        case 2:         //box instance 2 for PCIe port 1B [D1,F1]
          Fun = BoxInst - 1;
          break;
        case 3:         //box instance 3 for PCIe port 2A [D2,F0]
        case 4:         //box instance 4 for PCIe port 2B [D2,F1]
        case 5:         //box instance 5 for PCIe port 2C [D2,F2]
        case 6:         //box instance 6 for PCIe port 2D [D2,F3]
          Fun = BoxInst - 3;
          break;
        case 7:         //box instance 7 for PCIe port 3A [D3,F0]
        case 8:         //box instance 8 for PCIe port 3B [D3,F1]
        case 9:         //box instance 9 for PCIe port 3C [D3,F2]
        case 10:        //box instance 10 for PCIe port 3D [D3,F3]
          Fun = BoxInst - 7;
          break;
        default:
          DEBUG ((EFI_D_ERROR, "BOX_IIOPCIE_RP - Unsupported port index #%d for CpuType %d. \n", BoxInst, CpuType));
          break;
        }
      } else {
        Fun = 0xFF;
      }
      break;

    case BOX_IIO_PCIE_NTB:
      if((BoxInst < MAX_ALL_IIOPCIE_PCIENTB) && (FunTbl[CpuType][BoxType][FuncBlk] != 0xFF)){
        Fun = BoxInst;      // Instances 0..3 map to Func 0..3
      } else {
        Fun = 0xFF;
      }
      break;

    case BOX_IIO_DFX:
      if((BoxInst < MAX_ALL_IIO_DFX) && (FunTbl[CpuType][BoxType][FuncBlk] != 0xFF)) {
        switch(FuncBlk){
        case  IIO_DFX:          // Functional block for [D6,F0-6], [D7,F0-3]
          Fun = (BoxInst < 7) ? BoxInst : (BoxInst - 7);
          break;
        case  IIO_DFX_GLOBAL:   // Functional block for [D6, F7]
          Fun = 7;
          break;
        case  IIO_DFX_VTD:      // Functional block for [D7, F4]
          Fun = 4;
          break;
        default:
          DEBUG ((EFI_D_ERROR, "Unsupported/invalid functional block #%d found for CpuType %d. \n", FuncBlk, CpuType));
          CpuDeadLoop();
          break;
        }
      } else {
        Fun = 0xFF;
      }
      break;

    case BOX_PCU:
        Fun = FunTbl[CpuType][BoxType][FuncBlk];
      break;

    case BOX_R3QPI:
      if (((CpuType == CPU_HSX) && (BoxInst < MAX_HSX_R3QPI)) || ((CpuType == CPU_BDX) && (BoxInst < MAX_BDX_R3QPI))) {
        Fun = FunTbl[CpuType][BoxType][FuncBlk] + (4 * BoxInst);
      } else if ((CpuType == CPU_IVT) && (BoxInst < MAX_IVT_R3QPI)) {
        Fun = FunTbl[CpuType][BoxType][FuncBlk];
      }
      break;

    case BOX_MC:
      if (BoxInst < MAX_INST_IN_MC && CpuIndex < MAX_CPU_INDEX) {
        Fun = McFuncTbl[CpuIndex][FuncBlk][BoxInst];
      } else {
        Fun = 0xFF;
      }
      break;
    
    case BOX_MCDDC:
      if (BoxInst < MAX_INST_IN_MCDDC && CpuIndex < MAX_CPU_INDEX) {
        Fun = McddcFuncTbl[CpuIndex][FuncBlk][BoxInst];
      } else {
        Fun = 0xFF;
      }
      break;

    case BOX_MCIO:
      if (BoxInst < MAX_INST_IN_MCIO && CpuIndex < MAX_CPU_INDEX) {
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
        DEBUG((EFI_D_ERROR, "\nSBO reg only present on HSX, not on %d. \n", CpuType));
        CpuDeadLoop();
      }
      break;
    
    default:
      DEBUG ((EFI_D_ERROR, "Invalid Box Type %d. \n", BoxType));
      CpuDeadLoop();
  }

  if (Fun > 7) {
    DEBUG ((EFI_D_ERROR, "Invalid Functional Block %d accessed for CPUType %d CPUIndex %d Box Type %d and Box Instance %d. \n",
        CpuType, CpuIndex, FuncBlk, BoxType, BoxInst));
    CpuDeadLoop();

  }

  return Fun;
}



UINT32
GetRegisterOffset (
  CSR_OFFSET   RegOffset
  )
/*++

Routine Description:
  Indetifies the read hardware register for given BoxType, unctional Block, & pseudo table offset

Arguments:
  Offset:  Bits <31:24> Bits <23:16>  Bit 15  Bits <14:12>  Bits <11:0>
            Box Number  Func Number     Pseudo      Size      pseudo table Offset
 

Returns:
  Bits <31:24>  Bits <23:16>  Bit 15  Bits <14:12>  Bits <11:0>
   Box Number Func Number       0       Size          PCICfg Offset


--*/
{
  UINT32   PseudoOffset, Reg = 0;
  UINT8    Error = 0, BoxType, FuncBlk, CpuIdx;

 /* Columns of each table:                                                    */
 /*        column 0: HSX                                                      */
 /*        column 1: IVT_EX                                                   */

  if (CpuType == CPU_HSX) {
    CpuIdx = 2;
  }
  else if (CpuType == CPU_BDX){
    CpuIdx = 3;
  }
  else {
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

    default:
      Error = TRUE;  
  }

  if (Error == TRUE) {
    DEBUG ((EFI_D_ERROR, "Invalid Pesedo Register Table for Box Type = %d Functional Block=  %d   \n", BoxType, FuncBlk));
    CpuDeadLoop();
  }

  return Reg;
}

//
////4987134 start 
//

// Support virtual addressing
UINTN
GetCpuCsrAddress (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset,
  UINT8    *Size
  )
  
//
////4987134 end
//
/*++

Routine Description:
  Computes address of CPU Uncore & IIO PCI configuration space using the MMIO mechanism

Arguments:
  SocId     - CPU Socket Node number (Socket ID)
  BoxInst   - Box Instance, 0 based
  Offset    - Register offset; values come from the auto generated header file
  Size      - Register size in bytes (may be updated if pseudo-offset)

Returns:
  Address

--*/
{
  UINT32 Bus, Dev, Fun;
//
//HSD_4756680_START
//

// Support virtual addressing
  
  UINTN RegAddr;
  
//
//HSD_4756680_END
// 
  UINT8 BoxType;
  UINT8 FuncBlk;
  CSR_OFFSET RegOffset;
  
  RegOffset.Data = Offset;
  
  BoxType = (UINT8)RegOffset.Bits.boxtype;  
  FuncBlk = (UINT8)RegOffset.Bits.funcblk;

  /*********************************************************************************************************
    ToDo: 
        For now, this implementation only covers the Bus/Dev/Fun number generation for IVT and HSX CPUs.
        Register offset and size information comes from the HSX style register offset passed to this function.
        When the auto generation of header files using the new format is available, then we need to implement
        the logic to translate the register pseudo offset into real offset.
   *********************************************************************************************************/
  if(RegOffset.Bits.pseudo) {
    RegOffset.Data = GetRegisterOffset (RegOffset);
    *Size = (UINT8)RegOffset.Bits.size;
  }  

  //
  // Identify the PCI Bus/Device/Function number for the access
  //
  Bus = GetBusNumber (SocId, BoxType);
  Dev = GetDeviceNumber (BoxType, BoxInst, FuncBlk);
  Fun = GetFunctionNumber (BoxType, BoxInst, FuncBlk);

  //
  // Form address
  //
  
  //
  ////4987134 start
  //

  //Support virtual addressing
  
  RegAddr = (UINTN) (mmCfgBase | PCIE_REG_ADDR(Bus, Dev, Fun, RegOffset.Bits.offset));

  //
  ////4987134 end
  //
  return RegAddr;
} 


UINT32
ReadCpuCsr (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset
  )
/*++

Routine Description:
  Reads CPU Uncore & IIO PCI configuration space using the MMIO mechanism

Arguments:
  SocId     - CPU Socket Node number (Socket ID)
  BoxInst   - Box Instance, 0 based
  RegOffset - Register offset; values come from the auto generated header file

Returns:
  Register value

--*/
{
  UINT32 Data = 0;
  
  //
  ////4987134 start
  //

  // Support virtual addressing
  UINTN RegAddr;
  
  //
  ////4987134 end
  //  
  UINT8 Size;
  CSR_OFFSET RegOffset;
  
  RegOffset.Data = Offset;
  
  Size = (UINT8)RegOffset.Bits.size;

  //
  // Form address
  //
  RegAddr = GetCpuCsrAddress (SocId, BoxInst, Offset, &Size);

  //
  // Check register size and read data
  //
  switch (Size) {
  case sizeof (UINT32):
    Data = *(volatile UINT32 *) (UINTN)RegAddr;
    break;

  case sizeof (UINT16):
    Data = *(volatile UINT16 *) (UINTN)RegAddr;
    break;

  case sizeof (UINT8):
    Data = *(volatile UINT8 *) (UINTN)RegAddr;
    break;

  default:
    DEBUG ((EFI_D_ERROR, "Invalid Register Size in RegOffset = 0x%X\n", RegOffset.Data));
    CpuDeadLoop();
  }

  return Data;
}


void
WriteCpuCsr (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset,
  UINT32   Data
  )
/*++

Routine Description:
  Writes CPU Uncore & IIO PCI configuration space using the MMIO mechanism

Arguments:
  SocId     - CPU Socket Node number (Socket ID)
  BoxInst   - Box Instance, 0 based
  Offset    - Register offset; values come from the auto generated header file
  Data      - Register data to be written

Returns:
  None

--*/
{
  //
  //4987134 start
  //

  // Support virtual addressing
  
  UINTN RegAddr;
  
  //
  //4987134 end
  //  
  UINT8 Size;
  CSR_OFFSET RegOffset;
  
  RegOffset.Data = Offset;
  
  Size = (UINT8)RegOffset.Bits.size;

  //
  // Form address
  //
  RegAddr = GetCpuCsrAddress (SocId, BoxInst, Offset, &Size);

  //
  // Check register size and write data
  //
  switch (Size) {
  case sizeof (UINT32):
    *(volatile UINT32 *) (UINTN)RegAddr = Data;
    break;

  case sizeof (UINT16):
    *(volatile UINT16 *) (UINTN)RegAddr = (UINT16) Data;
    break;

  case sizeof (UINT8):
    *(volatile UINT8 *) (UINTN)RegAddr = (UINT8) Data;
    break;

  default:
    DEBUG ((EFI_D_ERROR, "Invalid Register Size in RegOffset = 0x%X\n", RegOffset.Data));
    CpuDeadLoop();
  }

  return;
}

UINT8
GetLegacyBusNumber(
  VOID
  )
{
  // Assumption is legacy socket will always be on bus 0.
  return 0;
}

UINT8
GetValidIioBusNumber(
  VOID
  )
{
  UINT8 busNum = 0;
  UINT32 data = 0;

  for (busNum = 0; busNum < 256; busNum += 0x10) {
    IoWrite32 (0x0cf8, (0x80000000 | (busNum << 16) |  (0 << 11)  | (0 << 8) | 0));
    data = IoRead32 (0x0cfc);
    if ((data != 0xFFFFFFFF) && (data == 0x0E018086))
      break;
  }
  return busNum;
}

UINT8
GetValidUncoreBusNumber(
  VOID
  )
{
  UINT8 busNum = 0;
  UINT32 data = 0;
  for (busNum = 0xF; busNum < 256; busNum += 0x10) {
    IoWrite32 (0x0cf8, (0x80000000 | (busNum << 16) |  (11 << 11)  | (0 << 8) | 0));
    data = IoRead32 (0x0cfc);
    if ((data != 0xFFFFFFFF) && (data == 0x0E1E8086))
      break;
  }
  return busNum;
}

UINT32
GetMmcfgAddress(
  UINT8 Flag          
  )
/*++

Routine Description:
  Get MMCFG base and limit

Arguments:
  Flag      - 0 - Base, 1 - Limit

Returns:
  Value

--*/
{
  UINT32 MmcfgAddress = 0;
  UINT8 IioBusNum = 0;
  //
  // Use CF8/CFC to read legacy socket's IIO MMCFG CSR at bus0:dev5:Func0:0x84  and 0x88.
  //
  IioBusNum = GetLegacyBusNumber();
  if (Flag == 1) {
    //
    // Get MmcfgLimit
    //
    IoWrite32 (0x0cf8, (0x80000000 | (IioBusNum << 16) |  (5 << 11)  | (0 << 8) | 0x88));
    MmcfgAddress = IoRead32 (0x0cfc);
    MmcfgAddress &= 0xfc000000;              // Mask Bits[25:0]
    MmcfgAddress |= 0x3FFFFFF;
  } else {
    //
    // Get MmcfgBase
    //
    IoWrite32 (0x0cf8, (0x80000000 | (IioBusNum << 16) |  (5 << 11)  | (0 << 8) | 0x84));
    MmcfgAddress = IoRead32 (0x0cfc);
    MmcfgAddress &= 0xfc000000;              // Mask Bits[25:0]
  }
  return MmcfgAddress;
}

UINT8
GetSbspSktId (
  )
/*++

Routine Description:

  Get CPU socket id for the system boot strap processor

Arguments:


Returns:

  system boot strap processor socket ID

--*/
{
   UINT32 data;
   UINT8 uncoreBusNum = 0;
   data = *(volatile UINT32 *) (UINTN)((GetMmcfgAddress(0)) | PCIE_REG_ADDR(0, 5, 0, 0x108));  // CSR 0:5:0:0x108
   uncoreBusNum = (UINT8)((data >> 8) & 0xFF);
   IoWrite32 (0x0cf8, (0x80000000 | (uncoreBusNum << 16) | (15 << 11) | (5 << 8) | 0xF0));  // CSR 1:15:5:0xF0
   data = IoRead32 (0x0cfc);
   //
   // SAD_TARGET.Legacy_PCH_Target (updated by pcode)
   //
   return (UINT8)((data >> 3) & 0x7); 
}

UINT32
Bios2PcodeMailBoxWrite (
  UINT8  socket,
  UINT32 dwordCommand,
  UINT32 dworddata
  )
/*++

Routine Description:
   Writes the given command to BIOS to PCU Mailbox Interface CSR register

Arguments:

  socket  - CPU Socket number (Socket ID)
  dwordCommand - Pcu mailbox command to write
  dworddata - Pcu mailbox data

Returns:

  error code from the Pcu mailbox (0 = NO ERROR)

--*/
{
  UINT32  MAILBOXSTATUS;
  //
  // Poll Mailbox BUSY_RUN flag until clear (ready)
  //
  while (ReadCpuCsr (socket, 0, BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG) & BIT31) {}

  //
  // Write the given command to mailbox
  //
  WriteCpuCsr (socket, 0, BIOS_MAILBOX_DATA_PCU_FUN1_REG, (UINT32) dworddata);
  WriteCpuCsr (socket, 0, BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG, (UINT32) (dwordCommand | BIT31));

  //
  // Poll Mailbox BUSY_RUN flag until clear (ready)
  //

  while (ReadCpuCsr (socket, 0, BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG) & BIT31) {}
  
  //
  // Read Mailbox data
  //

  //MAILBOXDATA = ReadCpuCsr (socket, 0, BIOS_MAILBOX_DATA_PCU_FUN1_REG);
  MAILBOXSTATUS = ReadCpuCsr (socket, 0, BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG);

  return MAILBOXSTATUS;
}


STATIC
UINT8
MemGetBoxInst (
    UINT8    NodeId,
    UINT8    BoxType,
    UINT8    ChannelId,
    UINT8    FuncBlk
  )
/*++

Routine Description:
  Converts NodeId, ChannelId to instance of type BoxType/FuncBlk based on
  Cpu type and cpu sub type

Arguments:
  NodeId    - Memory controller index
  BoxType   - Box Type; values come from CpuPciAccess.h
  ChannelId - DDR/VMSE channel Id within a Memory controller, 0 based, 0xFF if not used
  FuncBlk   - Functional Block; values come from CpuPciAccess.h

Returns:
  Box Instance

--*/
{
  UINT8 BoxInst = 0xFF;
  UINT8 SocId;

  SocId = NODE_TO_SKT(NodeId);
  switch (BoxType) {
  case BOX_MC:
    BoxInst = NODE_TO_MC(NodeId);                                   // One instance per Memory controller
    break;

  case BOX_MCDDC:
    if (ChannelId < MAX_CH) {
      BoxInst = NODE_TO_MC(NodeId)* MAX_CH + ChannelId;             // One instance per DDR Channel
    } else {
      DEBUG ((EFI_D_ERROR, "Wrong Channel ID parameter: 0x%08x passed with BOX_MCDDC\n", (UINT32) ChannelId));
      CpuDeadLoop();
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
        if (ChannelId < MAX_CH) {
          BoxInst = ChannelId;                                        // one instance per DDR Channel
        } else {
          DEBUG ((EFI_D_ERROR, "Invalid DDR Channel ID parameter: 0x%08x passed with BOX_MCIO\n", (UINT32) ChannelId));
          CpuDeadLoop();
        }
        break;

      case CPU_IVT:
        switch (CpuSubType) {
        case CPU_IVT_EX:
          if (ChannelId < MAX_CH/2) {
            BoxInst = NODE_TO_MC(NodeId) * MAX_CH/2 + ChannelId;       // one instance per  VMSE Channel
          } else {
            DEBUG ((EFI_D_ERROR, "Invalid VMSE Channel ID parameter: 0x%08x passed with BOX_MCIO for IVT EX\n", (UINT32) ChannelId));
            CpuDeadLoop();
          }
          break;
        } // Cpu sub type
        break;
      } // cpu type
      break;
    default:
      DEBUG ((EFI_D_ERROR, "Invalid FuncBlk: 0x%08x passed with BOX_MCIO\n", (UINT32) FuncBlk));
      CpuDeadLoop();
    } // funcblk
    break;

  default:
    DEBUG ((EFI_D_ERROR, "Invalid BoxType: 0x%08x passed\n", (UINT32) BoxType));
    CpuDeadLoop();
  }
  return BoxInst;
}


UINT32
MemPciCfgOffset (
  UINT8    NodeId,
  UINT8    ChIdOrBoxInst,
  UINT32   regBase
  )
/*++

Routine Description:
  Returns regBase with true offset (after processing pseudo offset, if needed)

Arguments:
  NodeId        - Memory controller index
  ChIdOrBoxInst - DDR/VMSE channel Id within a memory controller
                  or Box Instance (in case of non-MC boxes), 0 based, 0xFF if not used
  regBase       - Register offset; values come from the auto generated header file 
                - may be a pseudo offset

Returns:
  Updated Register OFfset

--*/
{
  CSR_OFFSET RegOffset;
  CSR_OFFSET TempReg;

  RegOffset.Data = regBase;

  //
  // Adjust offset if pseudo
  //
  if (RegOffset.Bits.pseudo) {
    TempReg.Data = GetRegisterOffset (RegOffset);
    RegOffset.Bits.offset = TempReg.Bits.offset;
    RegOffset.Bits.size = TempReg.Bits.size;
    //
    // Clear the pseudo flag after processing
    //
    RegOffset.Bits.pseudo = 0;
  }

  //
  // Return updated reg offset
  //
  return RegOffset.Data;
}




UINT32
ReadMemCpuCsr (
  UINT8    NodeId,
  UINT8    ChIdOrBoxInst,
  UINT32   Offset
  )
/*++

Routine Description:
  Reads CPU Memory Controller configuration space using the MMIO mechanism

Arguments:
  NodeId        - Memory controller index
  ChIdOrBoxInst - DDR/VMSE channel Id within a memory controller
                  or Box Instance (in case of non-MC boxes), 0 based, 0xFF if not used
  Offset        - Register offset; values come from the auto generated header file

Returns:
  Register value

--*/
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
      DEBUG ((EFI_D_ERROR, "ReadMemCpuCsr: WARNING MCIO with Pseudo Offset = 0x%x \n", Offset));
    }
  }

  if( BoxType == BOX_MC || BoxType == BOX_MCDDC || BoxType == BOX_MCIO ){
    //
    // Need to convert the NodeId/DDR channel ID to box Instance for
    // MC boxes
    //
    BoxInst = MemGetBoxInst (NodeId, BoxType, ChIdOrBoxInst, FuncBlk);
  } else {
    //
    // For non-MC boxes pass the Box Instance directly
    //
    BoxInst = ChIdOrBoxInst;
  }

  return ReadCpuCsr (SocId, BoxInst, Offset);
}

void
WriteMemCpuCsr (
  UINT8    NodeId,
  UINT8    ChIdOrBoxInst,
  UINT32   Offset,
  UINT32   Data
  )
/*++

Routine Description:
  Writes CPU Memory Controller configuration space using the MMIO mechanism

Arguments:
  NodeId        - Memory controller index
  ChIdOrBoxInst - DDR/VMSE channel Id within a memory controller
                  or Box Instance (in case of non-MC boxes), 0 based, 0xFF if not used
  Offset        - Register offset; values come from the auto generated header file
  Data          - Register data to be written

Returns:
  None

--*/
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
      DEBUG ((EFI_D_ERROR, "ReadMemCpuCsr: WARNING MCIO with Pseudo Offset = 0x%x \n", Offset));
    }
  }

  if( BoxType == BOX_MC || BoxType == BOX_MCDDC || BoxType == BOX_MCIO ){
    //
    // Need to convert the NodeId/DDR channel ID to box Instance for
    // MC boxes
    //
    BoxInst = MemGetBoxInst (NodeId, BoxType, ChIdOrBoxInst, FuncBlk);
  } else {
    //
    // For non-MC boxes pass the Box Instance directly
    //
    BoxInst = ChIdOrBoxInst;
  }
  WriteCpuCsr (SocId, BoxInst, Offset, Data);
}


UINT32
ReadPciCsr (
  UINT8    bus,
  UINT8    dev,
  UINT8    func,
  UINT32   reg,
  UINT8    regSize
  )
/*++

Routine Description:

  Reads PCI configuration space using the MMIO mechanism

Arguments:

  reg   - input parameter "reg" uses the format in the Bus_Dev_Func_CFG.H files

Returns:

  Value in requested reg

--*/
{
  UINT32  regAddr;
  UINT32  data;

  data = 0;

  //
  // Form address
  //
  regAddr = (UINT32) (mmCfgBase + PCIE_REG_ADDR(bus, dev, func, reg));

  //
  // bus + Dev:Func:Offset
  // Check register size and read data
  //
  switch (regSize) {
  case sizeof (UINT32):
    data = *(volatile UINT32 *) (UINTN)regAddr;
    break;

  case sizeof (UINT16):
    data = *(volatile UINT16 *) (UINTN)regAddr;
    break;

  case sizeof (UINT8):
    data = *(volatile UINT8 *) (UINTN)regAddr;
    break;

  default:
    DEBUG ((EFI_D_ERROR, "Invalid register size in reg = 0x%X.\n", reg));
    CpuDeadLoop();
  }
  //
  // Return data
  //
  return data;
}

void
WritePciCsr (
  UINT8    bus,
  UINT8    dev,
  UINT8    func,
  UINT32   reg,
  UINT8    regSize,
  UINT32   data
  )
/*++

Routine Description:

  Writes specified data to PCI configuration space using the MMIO mechanism

Arguments:

  reg   - input parameter "reg" uses the format in the Bus_Dev_Func_CFG.H files
  data  - Value to write

Returns:

  VOID

--*/
{
  UINT32   regAddr;

  //
  // Form address
  //
  regAddr = (UINT32) (mmCfgBase + PCIE_REG_ADDR(bus, dev, func, reg));
  //
  // bus + Dev:Func:Offset
  // Check register size and write data
  //
  switch (regSize) {
  case sizeof (UINT32):
    *(volatile UINT32 *) (UINTN)regAddr = data;
    break;

  case sizeof (UINT16):
    *(volatile UINT16 *) (UINTN)regAddr = (UINT16) data;
    break;

  case sizeof (UINT8):
    *(volatile UINT8 *) (UINTN)regAddr = (UINT8) data;
    break;

  default:
    DEBUG ((EFI_D_ERROR, "Invalid register size in reg = 0x%X.\n", reg));
    CpuDeadLoop();
  }

}

//
// Driver entry point
//
EFI_STATUS
CpuCsrAccessStart (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS                     Status;
  EFI_HANDLE                     Handle = NULL;
  UINT8                          Socket = 0;
  BOOLEAN                        InSmm;

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

  // Locate the IIO Protocol Interface
  Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid, NULL, &mIioUds);
  ASSERT_EFI_ERROR (Status);

  //
  // Allocate the buffer for protocol data structure and 
  // Install HP IOX ACCESS protocol
  //
  ZeroMem (&mCpuCsrAccessProtocol, sizeof (EFI_CPU_CSR_ACCESS_PROTOCOL));

  mIioUdsData = (IIO_UDS *)mIioUds->IioUdsPtr;

  mCpuCsrAccessProtocol.GetCpuCsrAddress       = GetCpuCsrAddress;
  mCpuCsrAccessProtocol.ReadCpuCsr             = ReadCpuCsr;
  mCpuCsrAccessProtocol.WriteCpuCsr            = WriteCpuCsr;
  mCpuCsrAccessProtocol.Bios2PcodeMailBoxWrite = Bios2PcodeMailBoxWrite;
  mCpuCsrAccessProtocol.ReadMemCpuCsr          = ReadMemCpuCsr;
  mCpuCsrAccessProtocol.WriteMemCpuCsr         = WriteMemCpuCsr;
  mCpuCsrAccessProtocol.ReadPciCsr             = ReadPciCsr;
  mCpuCsrAccessProtocol.WritePciCsr            = WritePciCsr;

  CpuType = mIioUds->IioUdsPtr->SystemStatus.cpuType;
  CpuSubType = mIioUds->IioUdsPtr->SystemStatus.cpuSubType;
  socketPresentBitMap = mIioUds->IioUdsPtr->SystemStatus.socketPresentBitMap;
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    busIio[Socket] = mIioUds->IioUdsPtr->PlatformData.IioQpiInfo[Socket].BusNum;
    busUncore[Socket] = mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].BusNum;
  }
  mmCfgBase = (UINT32)mIioUds->IioUdsPtr->PlatformData.PciExpressBase;

  if (InSmm) {
    Status = mSmst->SmmInstallProtocolInterface (
            &Handle,
            &gEfiCpuCsrAccessGuid,
            EFI_NATIVE_INTERFACE,
            &mCpuCsrAccessProtocol
            );
  } else {
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEfiCpuCsrAccessGuid,
                  EFI_NATIVE_INTERFACE,
                  &mCpuCsrAccessProtocol
                  );
  //4987134 start

  ASSERT_EFI_ERROR (Status);

 
  
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  RuntimeCpuCsrAccessAddressChangeEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &mVirtualAddressChangeEvent
                  );
   //4987134 end
  }
 
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
