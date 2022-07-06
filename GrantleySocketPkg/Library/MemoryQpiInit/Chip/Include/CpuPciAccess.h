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
#ifndef  _CPU_PCI_ACCESS_H_
#define  _CPU_PCI_ACCESS_H_

#include "DataTypes.h"
#include "SysRegs.h"

//
// CPU Types; this needs to be contiguous to assist in table look up
//
#define CPU_HSX              0
#define CPU_IVT              1
#define CPU_BDX              2
#define MAX_CPU_TYPES        3

//
//  CPU ExtFamily/Family/Model  bit[19:4] of cpuid(1)_eax
//
#define CPU_FAMILY_IVT     0x306e      // Ivytown CPU
#define CPU_FAMILY_HSX     0x306F      // Haswell CPU
#define CPU_FAMILY_BDX_DE  0x5066   // Broadwell Server CPU - BDX-DE
#define CPU_FAMILY_BDX     0x406F      // Broadwell Server CPU - BDX-EP

//
// HSX CPU Sub types
//
#define CPU_HSX_1HA          0
#define CPU_HSX_2HA          1
#define CPU_HSX_EX           2

// BDX CPU Sub types
//
#define CPU_BDX_1HA          0
#define CPU_BDX_2HA          1
#define CPU_BDX_EX           2

// IVT CPU Sub types
//
#define CPU_IVT_EX           0

//
// CPU Index for MC function look-up
//
#define IDX_CPU_HSX_1HA      0
#define IDX_CPU_HSX_2HA      1
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
#define  BOX_IIO_PCIE        15
#define  BOX_IIO_PCIE_NTB    16
#define  BOX_IIO_CB          17
#define  BOX_IIO_DFX         18
#define  BOX_VCU             19
#define  BOX_SBO             20
#define  MAX_BOX_TYPES       21

//
// Maximum Number of Instances supported by each box type. Note that if the number of instances
// are same for all supported CPUs, then we will have only one #define here (i.e MAX_ALL_XXXXX)
//
#define  MAX_JKT_CBO          8
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

#define  MAX_JKT_QPIAGENT    2
#define  MAX_IVT_QPIAGENT    3
#define  MAX_HSX_QPIAGENT    3
#define  MAX_BDX_QPIAGENT    3

#define  MAX_ALL_R2PCIE      1
#define  MAX_ALL_UBOX        1
#define  MAX_ALL_IIO         1
#define  MAX_ALL_PCU         1
#define  MAX_ALL_IIO_DFX     11  // 1 instances in IIO_DFX block
#define  MAX_HSX_SBO         4
#define  MAX_BDX_SBO         4
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
#define  MAX_HSX_1HA_MCDDRIO        4
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

#define  QPI0_DEV_NUM        8
#define  QPI1_DEV_NUM        9
#define  QPI2_IVT_DEV_NUM    24
#define  QPI2_HSX_DEV_NUM    10

#define  R2PCIE_CFG          0
#define  R2PCIE_PMON         1

#define  UBOX_CFG            0
#define  UBOX_DFX            1
#define  UBOX_MISC           2

#define  IIO_CFG             0 // temporary
#define  IIO_VTD             0
#define  IIO_MEMHP           1
#define  IIO_RAS             2
#define  IIO_THRMTHR         3
#define  IIO_IOAPIC          4
#define  IIO_LT              5
#define  IIO_PERFMON         6

#define  IIO_PCIEDMI         0

#define  IIO_PCIE            0

#define  IIO_PCIENTB         0

#define  IIO_CBFUNC0         0
#define  IIO_CBFUNC1         1
#define  IIO_CBFUNC2         2
#define  IIO_CBFUNC3         3
#define  IIO_CBFUNC4         4
#define  IIO_CBFUNC5         5
#define  IIO_CBFUNC6         6
#define  IIO_CBFUNC7         7

#define  IIO_DFX             0
#define  IIO_DFX_GLOBAL      1
#define  IIO_DFX_VTD         2

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

#endif   // _CPU_PCI_ACCESS_H_
