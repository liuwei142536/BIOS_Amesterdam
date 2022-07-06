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
 * Copyright 2006 - 2016 Intel Corporation All Rights Reserved.
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
#ifndef  _platformhost_h
#define  _platformhost_h

#include <Token.h> // Aptio V server override
//
// MAX Number of Processor supported by Intel platform
//
#ifndef MAX_SOCKET
#define MAX_SOCKET                      4  // Maximum CPU sockets supported
#endif

#define EP_FEATURES_ONLY                1

#define MAX_CORE_IVT                   15 // Maximum cores per CPU (IVT)
#define MAX_CORE_HSX                   18 // Maximum cores per CPU (HSX)
#define MAX_CORE_BDX                   24 // Maximum cores per CPU (BDX)

#ifndef MAX_CORE
#define MAX_CORE                       24 // Maximum cores per CPU (BDX)
#endif
#define MAX_CORES_PER_CPU               MAX_CORE


#ifndef MAX_QPI_PORTS
#define MAX_QPI_PORTS                   2  // Max QPI ports supported
#endif

#ifndef SPARING_SCRATCHPAD_SUPPORT
#define SPARING_SCRATCHPAD_SUPPORT      1 // Scratchpad support for Rank Sparing
#endif

//
// Post Package Repair
//
#define PPR_SUPPORT                    1
#define MAX_PPR_ADDR_ENTRIES           20

// Select one of the following platforms
//#define DVP_PLATFORM  1  // DVP
//
#define CRB_PLATFORM  1    // SRP

#ifndef DVP_PLATFORM
#ifndef CRB_PLATFORM

//#define ASM_INC       1

/*
 MULTIPLE_PLATFORM_SUPPORT should be defined when the user KNOWS that inputs
  (the ones defined to zero under this flag below) are given via sysSetup structure
*/
#define MULTIPLE_PLATFORM_SUPPORT  1
#endif
#endif

#if  defined(SV_HOOKS) || defined(DISABLE_TRUE_RELEASE) 
#define SERIAL_DBG_MSG  1
#else
#endif
// AptioV server override - use values from Token.h
//#ifdef SDP_FLAG
//#define SERIAL_DBG_MSG  1
//#endif
#define RC_BEGIN_END_DEBUG_HOOKS


//TODO CORE remove obsolete flags
#define SOFT_SDV_FLAG   BIT0      // flag to indicate running on Soft SDV
#define VP_FLAG         BIT1      // flag to indicate running on VP
#define SIMICS_FLAG     BIT2      // flag to indicate running on Simics
#define RTL_SIM_FLAG    BIT3      // flag to indicate running on the RTL simulator
#define QUIET_MODE      BIT16     // flag to enable minimal debug messages
#define RANDOM_TRAINING BIT17     // flag to enable random training responses from SoftSdv
#define FORCE_SETUP     BIT18     // flag to force BIOS setup
#define DDR_TRAINING_EN BIT19     // flag to tell the BIOS to execute DDR training
//
// Enumerated Platform SMBUS controllers
//
#define PROCESSOR                       0
#define PCH                             1

#ifndef TypePlatformDefault
//
// Platform types - if not defined already
//
#ifndef GRANGEVILLE_FLAG
typedef enum {
  TypeMayanCityCRB = 0x00,
  TypeMayanCitySRP,
  TypeMayanCitySRPDDR3,
  TypeYakimaCityCRB = 0x03,
  TypeYakimaCitySRP,
  TypeYakimaCitySTHI,
  TypeMayanCityCRBDDR3,
  TypeAztecCityCRBSRP = 0x07,
  TypeAztecCitySTHI,
  TypeHedtReversedIdBits,
  TypeIncaCityCRB = 0x0A,
  TypeIncaCitySRP,
  TypeHalfWidthCitySRP = 0x0C,
  TypeKahunaDVP2SPbg = 0x0F,
  TypeKahunaDVP2S,
  TypeArandasVVP2S = 0x11,
  TypeArandasVVP4S,
  TypeHedtSKU1CRB = 0x13,
  TypeHedtEv,
  TypeHedtDDR3,
  TypeSaxtonRock = 0x18,
  TypeEchoRock,
  TypeBalanceRock,
  TypeEmeraldPoint,
  TypeLongBrookCRB = 0x1C,
  TypeWildcatPass = 0x32
} EFI_PLATFORM_TYPE;


typedef enum {
    HEDT_FAB1 = 0x00,
    HEDT_FAB2
} EFI_HEDT_PLATFORM_REVISION_ID;


#define TypeKahunaDVP TypeKahunaDVP2S

#define TypePlatformUnknown       0xFF
#define TypePlatformMin           TypeMayanCityCRB
#define TypePlatformMax           TypeWildcatPass
// HSD 4987690 : Simics BDX fail to run MiniBios in 4s Configuration - Changing the default platform type from MayanCityCRB to IncaCityCRB
#define TypePlatformDefault       TypeIncaCityCRB
#define TypePlatformRevIdDefault  0x00

// TODO We are commenting out the Grangeville support flag until porting done

#else
  typedef enum {
    TypeBeverlyCoveCRB = 1,
    TypeBeverlyCoveSODimmCRB,
    TypeTrabucoCanyon,
    TypePiestewaPeak,
    TypeCamelbackMountain,
    TypeTlaquepaque,
	TypeEchoCanyon
  } EFI_PLATFORM_TYPE;


#define TypePlatformUnknown       0xFF
#ifndef TypePlatformMin
#define TypePlatformMin           TypeBeverlyCoveCRB
#define TypePlatformDefault       TypeBeverlyCoveCRB
#endif


#define TypePlatformMax           TypeTlaquepaque


#endif
#define TypePlatformRevIdDefault  0x00
#endif

//
// Enumerated Unique Platform Component IDs
//
#define NOT_INITIALIZED                 0
#define SPD                             1
#define DCP_ISL9072X                    2
#define DCP_AD5247                      3
#define MTS                             4  //TSOD
#define NO_DEV                          0xFFFF
//
// Default I/O base addresses
//
#define  REG_ACPI_BASE_ADDRESS          0x40
#define  PM_ENABLE                      0x44
#define  IO_REG_ACPI_TIMER              0x08
#define  ICH_PMBASE_ADDR                0x400
#define  REG_GPIO_BASE_ADDRESS          0x48
//
//#define  ICH_GPIOBASE_ADDR              0x500
#define  ICH_SMBBASE_ADDR               0x700
#define  R_PCH_SMBUS_PCICMD             0x04
#define  B_PCH_SMBUS_PCICMD_IOSE        BIT0
#define  R_PCH_SMBUS_BASE               0x20
#define  B_PCH_SMBUS_BASE_BAR           0x0000FFE0
#define  R_PCH_SMBUS_HOSTC              0x40
#define  B_PCH_SMBUS_HOSTC_HST_EN       BIT0
//
#define  SERIAL_DBG_COM_BASE            0x3F8
//
// Platform SMBUS definitions
//
#define SMB_SEG0                        0
#define SMB_SEG1                        1
#define SMB_SEG2                        2
#define SMB_SEG3                        3

//
// Major Warning codes
//
#define  WARN_SETUP_INVALID         0x01
#define  WARN_MINOR_WILDCARD  0xff

#define  SMB_TIMEOUT    100000     // 100 ms
#define  SMB_RETRY_LIMIT   20       // 2 sec
#define  CATCHALL_TIMEOUT  100000    // 100 ms
#ifndef  MMCFG_BASE_ADDRESS
// AptioV server override - use SDL defined PCIEX_BASE_ADDRESS, if available
#ifdef PCIEX_BASE_ADDRESS
#define MMCFG_BASE_ADDRESS PCIEX_BASE_ADDRESS
#else
#define  MMCFG_BASE_ADDRESS  0x80000000
#endif // #ifdef PCIEX_BASE_ADDRESS
#endif // #ifndef  MMCFG_BASE_ADDRESS
#define FOUR_GB_MEM         0x40                // 4GB in 64MB units
#define MAX_MEM_ADDR        0x40000             // 46-bit addressing (256MB units)
#define HIGH_GAP            1                   // High gap (256 MB units)
// AptioV server override - use SDL defined PCIEX_LENGTH, if available
#ifdef PCIEX_LENGTH
#define MMCFG_SIZE          PCIEX_LENGTH
#else
#define MMCFG_SIZE          256*1024*1024       // MMCFG Size (in Bytes)
#endif

#define MMIOH_SIZE          256                 // MMIOH Size (in GB)
#define DEFAULT_COM_PORT    0x80

// NS SKU Identification
// 0: DE, 1: NS Storage Sku, 2: NS Comms Sku
typedef enum {
  DESkuID = 0,
  NSStorageSkuID,
  NSCommsSkuID,
  DefaultSkuID = 0xFF  
} NS_SKU_ID;

//
//-----------------------------------------------------------------------------
//
#include "MemPlatform.h"
#include "QpiPlatform.h"


#ifndef ASM_INC
#endif  // ASM_INC

#endif   // _platformhost_h

