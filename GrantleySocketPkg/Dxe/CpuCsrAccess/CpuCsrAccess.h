//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license
// agreement with Intel or your vendor.  This file may be
// modified by the user, subject to additional terms of the
// license agreement
//
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
#ifndef  _CPU_CSR_ACCESS_H_
#define  _CPU_CSR_ACCESS_H_

#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/CpuCsrAccess.h>
#include <Protocol/IioUds.h>
#include <Protocol\SmmBase2.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include "MemHost.h"
#include "PSEUDO_OFFSET.h"

//
// CPU Index for MC function look-up
//
#define IDX_CPU_HSX_1HA      0
#define IDX_CPU_HSX_2HA      1

#define IDX_CPU_IVT_2HA      1
#define IDX_CPU_IVT_EX       2

#define IDX_CPU_BDX_1HA      0
#define IDX_CPU_BDX_2HA      1

#define MAX_CPU_INDEX        3

//
// Box Types; this needs to be contiguous to assist in table look up
//
#define  BOX_CBO             0
#define  BOX_CBOBC           1
#define  BOX_RSVD1           2
#define  BOX_HA              3
#define  BOX_RSVD2           4
#define  BOX_R3QPI           5
#define  BOX_QPIAGENT        6
#define  BOX_R2PCIE          7
#define  BOX_UBOX            8
#define  BOX_IIO             9
#define  BOX_PCU             10
#define  BOX_MC              11
#define  BOX_MCDDC           12
#define  BOX_MCIO            13
#define  BOX_IIO_PCIE_DMI    14
#define  BOX_IIO_PCIE_RP     15
#define  BOX_IIO_PCIE_NTB    16
#define  BOX_IIO_CB          17
#define  BOX_IIO_DFX         18
#define  BOX_VCU             19
#define  BOX_SBO             20
#define  MAX_BOX_TYPES       21
#define  MAX_HSX_SBO         4
#define  MAX_BDX_SBO         4
//
// Maximum Number of Instances supported by each box type. Note that if the number of instances
// are same for all supported CPUs, then we will have only one #define here (i.e MAX_ALL_XXXXX)
//
#define  MAX_IVT_CBO         15
#define  MAX_HSX_CBO         18
#define  MAX_BDX_CBO         24

#define  MAX_ALL_CBOBC       1

#define  MAX_IVT_HA          2
#define  MAX_HSX_HA          2
#define  MAX_BDX_HA          2

#define  MAX_IVT_R3QPI       2
#define  MAX_HSX_R3QPI       2
#define  MAX_BDX_R3QPI       2

#define  MAX_IVT_QPIAGENT    3
#define  MAX_HSX_QPIAGENT    3
#define  MAX_BDX_QPIAGENT    3

#define  MAX_ALL_R2PCIE      1
#define  MAX_ALL_UBOX        1
#define  MAX_ALL_IIO         1
#define  MAX_ALL_PCU         1

//
// MAX instances of MC functional
// blocks for different CPU Sub-types.
// These #defines are not used in
// the code.  They are included here to
// aid understanding the Instance -> function mapping
// implementation in CpuPciAccess.c
//
#define  MAX_HSX_1HA_MCMAIN         1
#define  MAX_HSX_1HA_MCCTL          4
#define  MAX_HSX_1HA_MCDP           4
#define  MAX_HSX_1HA_MCDECS         4
#define  MAX_HSX_1HA_MCDDRIOGBC     1
#define  MAX_HSX_1HA_MCDDRIOEXTGBC  1
#define  MAX_HSXT_1HA_MCDDRIO        4
#define  MAX_HSX_1HA_MCDDRIOEXT     4
#define  MAX_HSX_1HA_MCDDRIOMCC     4
#define  MAX_HSX_1HA_MCDDRIOEXTMCC  4

#define  MAX_HSX_2HA_MCMAIN         2
#define  MAX_HSX_2HA_MCMAINEXT      2
#define  MAX_HSX_2HA_MCCTL          4
#define  MAX_HSX_2HA_MCDP           4
#define  MAX_HSX_2HA_MCDECS         4
#define  MAX_HSX_2HA_MCDDRIO        4
#define  MAX_HSX_2HA_MCDDRIOEXT     4
#define  MAX_HSX_2HA_MCDDRIOMCC     4
#define  MAX_HSX_2HA_MCDDRIOEXTMCC  4

/**#define  MAX_IVT_2HA_MCMAIN         2
#define  MAX_IVT_2HA_MCMAINEXT      2
#define  MAX_IVT_2HA_MCCTL          4
#define  MAX_IVT_2HA_MCDP           4
#define  MAX_IVT_2HA_MCDECS         4
#define  MAX_IVT_2HA_MCDDRIO        4
#define  MAX_IVT_2HA_MCDDRIOEXT     4
#define  MAX_IVT_2HA_MCDDRIOMCC     4
#define  MAX_IVT_2HA_MCDDRIOEXTMCC  4
**/
#define  MAX_IVT_EX_MCMAIN          2
#define  MAX_IVT_EX_MCMAINEXT       2
#define  MAX_IVT_EX_MCCTL           8
#define  MAX_IVT_EX_MCDP            8
#define  MAX_IVT_EX_MCDECS          8
#define  MAX_IVT_EX_MCDDRIO         4
#define  MAX_IVT_EX_MCDDRIOEXT      4
#define  MAX_IVT_EX_MCDDRIOMCC      4
#define  MAX_IVT_EX_MCDDRIOEXTMCC   4

#define  MAX_BOX_INSTANCES      8


//
// Functional Blocks supported by each box type; there is a 1:1 maping between these definitons and
// PCI function supported by a box. But the valude of the define itself doesn't have any relationship
// with the actual PCI Function number. That translation will be done by the helper routine for
// different supported CPU types.
// Note: These values need  to be contiguos for each box type to assist in table look up
// 
#define  CBO_CFG             0

#define  CBO_MEMSAD          0
#define  CBO_IOSAD           1
#define  CBO_COH             2

#define  HA_CFG              0
#define  HA_PMON             1
#define  HA_DFX              2
#define  HA_GDXC             3

#define  R3QPI_CFG           0
#define  R3QPI_LNK0MON       1
#define  R3QPI_LNK1MON       2

#define  QPI_LL              0
#define  QPI_PMON            1
#define  QPI_REUT            2
#define  QPI_DFX_CIO         3
#define  QPI_DFX_PHY         4
#define  QPI_DFX_LINK        5
#define  QPI_DFX_COMMON      6

#define  R2PCIE_CFG          0
#define  R2PCIE_PMON         1

#define  UBOX_CFG            0
#define  UBOX_DFX            1
#define  UBOX_MISC           2

//
// BOX_IIO function blocks
//
#define  IIO_VTD                  0   //D5,F0
#define  IIO_MEMHP                1   //D5,F1
#define  IIO_RAS                  2   //D5,F2
#define  IIO_THRMTHR              3
#define  IIO_IOAPIC               4   //D5,F4
#define  IIO_LT                   5
#define  IIO_PERFMON              6   //D5,F6
#define  MAX_ALL_IIO              1   // 1 instance per functional block in this box

//
// BOX_IIO_PCIE_DMI function blocks
//
#define  IIOPCIE_PCIEDMI          0
#define  MAX_ALL_IIOPCIE_DMI      1 // 0:0:0

//
// BOX_IIO_PCIE_RP function blocks ([D0,F0], [D1,F0-1], [D2,F0-3], [D3,F0-3])
//
#define  IIOPCIE_PCIERP           0
#define  MAX_ALL_IIOPCIE_PCIERP   11 // 11 instances in PCIE function block

//
// BOX_IIO_PCIE_NTB function block
//
#define  IIOPCIE_PCIENTB          0
#define  MAX_ALL_IIOPCIE_PCIENTB  4 // 4 instances in PCIE_NTB (0:3:0/1/2/3)

//
// BOX_IIO_CB function blocks
//
#define  IIO_CB0                  0  // D4, F0
#define  IIO_CB1                  1  // D4, F1
#define  IIO_CB2                  2  // D4, F2
#define  IIO_CB3                  3  // D4, F3
#define  IIO_CB4                  4  // D4, F4
#define  IIO_CB5                  5  // D4, F5
#define  IIO_CB6                  6  // D4, F6
#define  IIO_CB7                  7  // D4, F7
#define  MAX_ALL_IIOCB            1  // 1 instance per CB function block

//
// BOX_IIO_DFX function blocks
//
#define  IIO_DFX                  0   // Functional block for [D6,F0-6], [D7,F0-3]
#define  MAX_ALL_IIO_DFX          11  // 11 instances in IIO_DFX block for [D6,F0-6], [D7,F0-3]

#define  IIO_DFX_GLOBAL           1   // Functional block for [D6, F7]
#define  MAX_ALL_IIO_DFX_GLOBAL   1   // 1 instance only for IIO_DFX_GLOBAL

#define  IIO_DFX_VTD              2   // Functional block for [D7, F4]
#define  MAX_ALL_IIO_DFX_VTD      1   // 1 instance only for IIO_DFX_VTD

#define  PCU_FUN0            0
#define  PCU_FUN1            1
#define  PCU_FUN2            2
#define  PCU_FUN3            3
#define  PCU_FUN4            4
#define  PCU_FUN5            5
#define  PCU_FUN6            6
#define  PCU_FUN7            7

#define  MC_MAIN             0
#define  MC_MAINEXT          1
#define  MAX_MC_FUNC_BLKS    2
#define  MAX_INST_IN_MC      4

#define  MCDDC_CTL           0
#define  MCDDC_DP            1
#define  MCDDC_DESC          2
#define  MAX_MCDDC_FUNC_BLKS 4
#define  MAX_INST_IN_MCDDC   8

#define  MCIO_DDRIOGBC       0
#define  MCIO_DDRIOEXTGBC    1
#define  MCIO_DDRIO          2
#define  MCIO_DDRIOEXT       3
#define  MCIO_DDRIOMCC       4
#define  MCIO_DDRIOEXTMCC    5
#define  MAX_MCIO_FUNC_BLKS  6
#define  MAX_INST_IN_MCIO    4

#define MEM_MAIN             ((BOX_MC << 8)    | (MC_MAIN))
#define MEM_MAINEXT          ((BOX_MC << 8)    | (MC_MAINEXT))
#define MEM_CTL              ((BOX_MCDDC << 8) | (MCDDC_CTL))
#define MEM_DP               ((BOX_MCDDC << 8) | (MCDDC_DP))
#define MEM_DESC             ((BOX_MCDDC << 8) | (MCDDC_DESC))
#define MEM_DDRIOGBC         ((BOX_MCIO << 8)  | (MCIO_DDRIOGBC))
#define MEM_DDRIOEXTGBC      ((BOX_MCIO << 8)  | (MCIO_DDRIOEXTGBC))
#define MEM_DDRIO            ((BOX_MCIO << 8)  | (MCIO_DDRIO))
#define MEM_DDRIOEXT         ((BOX_MCIO << 8)  | (MCIO_DDRIOEXT))
#define MEM_DDRIOMCC         ((BOX_MCIO << 8)  | (MCIO_DDRIOMCC))
#define MEM_DDRIOEXTMCC      ((BOX_MCIO << 8)  | (MCIO_DDRIOEXTMCC))

//
// legacy PCI address for CSRs
//
#define LEGACY_CSR_MMCFG_BASE_IIO   0x80000000 | (0 << 16) |  (5 << 11)  | (0 << 8) | 0x84  // CSR 0:5:0:0x84
#define LEGACY_CSR_MMCFG_LIMIT_IIO  0x80000000 | (0 << 16) |  (5 << 11)  | (0 << 8) | 0x88  // CSR 0:5:0:0x88
#define SAD_TARGET_CBO_IOSAD        0x80000000 | (0x3F << 16) |  (22 << 11) | (1 << 8) | 0xF0  // CSR 1:22:1:0xF0

//
// Format of CSR register offset passed to helper functions. 
// This must be kept in sync with the CSR XML parser tool that generates CSR offset definitions in the CSR header files.
//

//
//4987134 start
//

UINTN GetCpuCsrAddress (
  UINT8 SocId, 
  UINT8 BoxInst, 
  UINT32 Offset, 
  UINT8* Size
  );
  
//
//4987134 end
//

UINT32 ReadCpuCsr (
  UINT8 SocId, 
  UINT8 BoxInst, 
  UINT32 Offset
  );

VOID WriteCpuCsr (
  UINT8 SocId, 
  UINT8 BoxInst, 
  UINT32 Offset, 
  UINT32 Data
  );

UINT32 ReadMemCpuCsr (
  UINT8 NodeId, 
  UINT8 ChIdOrBoxInst, 
  UINT32 Offset
  );

VOID WriteMemCpuCsr (
  UINT8 NodeId, 
  UINT8 ChIdOrBoxInst, 
  UINT32 Offset, 
  UINT32 Data
  );

UINT32 ReadPciCsr (
  UINT8 bus, 
  UINT8 dev, 
  UINT8 func, 
  UINT32 reg,
  UINT8 regSize
  );

VOID WritePciCsr (
  UINT8 bus, 
  UINT8 dev, 
  UINT8 func, 
  UINT32 reg, 
  UINT8 regSize, 
  UINT32 data
  );

UINT32 Bios2PcodeMailBoxWrite (
  UINT8  socket, 
  UINT32 dwordCommand, 
  UINT32 dworddata
  );

#endif   // _CPU_CSR_ACCESS_H_
