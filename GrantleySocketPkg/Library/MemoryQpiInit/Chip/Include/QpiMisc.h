//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/*++
// **************************************************************************
// *                                                                        *
// *      Intel Restricted Secret                                           *
// *                                                                        *
// *      QPI Reference Code                                                *
// *                                                                        *
// *      ESS - Enterprise Silicon Software                                 *
// *                                                                        *
// *      Copyright (c) 2004 - 2013 Intel Corp.                             *
// *                                                                        *
// *      This program has been developed by Intel Corporation.             *
// *      Licensee has Intel's permission to incorporate this source code   *
// *      into their product, royalty free.  This source code may NOT be    *
// *      redistributed to anyone without Intel's written permission.       *
// *                                                                        *
// *      Intel specifically disclaims all warranties, express or           *
// *      implied, and all liability, including consequential and other     *
// *      indirect damages, for the use of this code, including liability   *
// *      for infringement of any proprietary rights, and including the     *
// *      warranties of merchantability and fitness for a particular        *
// *      purpose.  Intel does not assume any responsibility for any        *
// *      errors which may appear in this code nor any responsibility to    *
// *      update it.                                                        *
// *                                                                        *
// **************************************************************************
// **************************************************************************
// *                                                                        *
// *  PURPOSE:                                                              *
// *                                                                        *
// *      This file contains platfrom specific routines which might need    *
// *      porting for a particular platform.                                *
// *                                                                        *
// **************************************************************************
--*/
#ifndef _QPI_MISC_H_
#define _QPI_MISC_H_

#include "DataTypes.h"
#include "PlatformHost.h"

#define QPI_DEBUG_ERROR (1 << 0)
#define QPI_DEBUG_WARN  (1 << 1)
#define QPI_DEBUG_INFO0 (1 << 2)
#define QPI_DEBUG_INFO1 (1 << 3)

#define QPI_DEBUG_LEVEL (QPI_DEBUG_ERROR | QPI_DEBUG_WARN | QPI_DEBUG_INFO0 | QPI_DEBUG_INFO1)

//
// Separate this out so we can exclude them from compilation if not needed, to reduce some size
//
#if (QPI_DEBUG_LEVEL & QPI_DEBUG_ERROR)
#define QpiDebugPrintFatal(DbgInfo) OemDebugPrintQpi DbgInfo
#else
#define QpiDebugPrintFatal(DbgInfo)
#endif

#if (QPI_DEBUG_LEVEL & QPI_DEBUG_WARN)
#define QpiDebugPrintWarn(DbgInfo)  OemDebugPrintQpi DbgInfo
#else
#define QpiDebugPrintWarn(DbgInfo)
#endif

#if (QPI_DEBUG_LEVEL & QPI_DEBUG_INFO0)
#define QpiDebugPrintInfo0(DbgInfo) OemDebugPrintQpi DbgInfo
#else
#define QpiDebugPrintInfo0(DbgInfo)
#endif

#if (QPI_DEBUG_LEVEL & QPI_DEBUG_INFO1)
#define QpiDebugPrintInfo1(DbgInfo) OemDebugPrintQpi DbgInfo
#else
#define QpiDebugPrintInfo1(DbgInfo)
#endif


#define QPI_ASSERT(assertion, majorCode, minorCode) \
  if (!(assertion)) { \
    OemDebugPrintQpi (host, QPI_DEBUG_ERROR, "\n\n %s: %u   %s ", __FILE__, __LINE__, #assertion); \
    QpiAssert(host, majorCode, minorCode);\
  }

/* QPI POST code - Major */
#define STS_DATA_STRUCTURE_INIT               0xA0  // Initialize QPI inuput structure default values
#define STS_COLLECT_EARLY_SYSTEM_INFO         0xA1  // Collect info such as SBSP, Boot Mode, Reset type etc
#define STS_OPEN_SBSP_CONFIG_ACCESS           0xA2  // Setup IO SADs in SBSP to access the config space
#define STS_SETUP_MINIMUM_PATH                0xA3  // Setup up minimum path between SBSP & other sockets
#define STS_OPEN_PBSP_CONFIG_ACCESS           0xA4  // Setup IO SADs in PBSP to access the config space
#define STS_EARLY_RESET_CONFIG                0xA5  // System configurations that require some kind of reset
#define STS_PBSP_SYNC_UP                      0xA6  // Sync up with PBSPs
#define STS_TOPOLOGY_DISCOVERY                0xA7  // Topology discovery and route calculation
#define STS_PROGRAM_FINAL_ROUTE               0xA8  // Program final route
#define STS_PROGRAM_FINAL_IO_SAD              0xA9  // Program final IO SAD setting
#define STS_PROTOCOL_LAYER_SETTING            0xAA  // Protocol layer and other Uncore settings
#define STS_FULL_SPEED_TRANSITION             0xAB  // Transition links to full speed opeartion
#define STS_PHY_LAYER_SETTING                 0xAC  // Phy layer settings
#define STS_LINK_LAYER_SETTING                0xAD  // Link layer settings
#define STS_SYSTEM_COHERENCY_SETUP            0xAE  // Coherency Settings
#define STS_QPI_COMPLETE                      0xAF  // QPI is done

/* Subcodes for STS_QPI_COMPLETE */
#define MINOR_STS_ABOUT_TO_RESET              0x42  

/* Subcodes for STS_SETUP_MINIMUM_PATH */
#define MINOR_STS_ADD_SOCKET_TO_MIN_PATH_TREE 0x01
#define MINOR_STS_COLLECT_LEP                 0x02
#define MINOR_STS_CHK_PBSP_CHKIN              0x03
#define MINOR_STS_SET_PBSP_BOOT_PATH          0x04
#define MINOR_STS_SET_SBSP_CONFIG_PATH        0x05

/* Subcodes for STS_PBSP_SYNC_UP */
#define MINOR_STS_ISSUE_BOOT_GO               0x01
#define MINOR_STS_ISSUE_PHY_RST               0x02

/* Subcodes for STS_TOPOLOGY_DISCOVERY */
#define MINOR_STS_CHK_TOPOLOGY                0x01
#define MINOR_STS_DETECT_RING                 0x02
#define MINOR_STS_CONSTRUCT_TOPOLOGY_TREE     0x03
#define MINOR_STS_CALCULATE_ROUTE             0x04

/* Subcodes for STS_PROGRAM_FINAL_IO_SAD */
#define MINOR_STS_ALLOCATE_CPU_RESOURCE       0x01
#define MINOR_STS_FILL_SAD_TGTLST             0x02
#define MINOR_STS_PROGRAM_CPU_SAD_ENTRIES     0x03

/* Subcodes for STS_PROTOCOL_LAYER_SETTING */
#define MINOR_STS_PROGRAM_RTID                0x01
#define MINOR_STS_PROGRAM_RING_CRDT           0x02
#define MINOR_STS_PROGRAM_HA_CRDT             0x03
#define MINOR_STS_PROGRAM_HTRBT               0x04
#define MINOR_STS_QPI_MISC_SETUP              0x05


/* QPI Error code - Major */
#define ERR_BOOT_MODE                         0xD8
#define ERR_MINIMUM_PATH_SETUP                0xD9
#define ERR_TOPOLOGY_DISCOVERY                0xDA
#define ERR_SAD_SETUP                         0xDB
#define ERR_UNSUPPORTED_TOPOLOGY              0xDC
#define ERR_FULL_SPEED_TRANSITION             0xDD
#define ERR_S3_RESUME                         0xDE
#define ERR_GENERAL_FAILURE                   0xDF //TODO update grantleypostcodes doc w/ this

/* Subcodes for ERR_BOOT_MODE */
#define MINOR_ERR_UNSUPPORTED_BOOT_MODE       0x01
#define MINOR_ERR_PBSP_CHKIN_FAILURE          0x02

/* Subcodes for ERR_MINIMUM_PATH_SETUP/ERR_TOPOLOGY_DISCOVERY */
#define MINOR_ERR_ADD_SOCKET_TO_TOPOLOGY_TREE 0x01
#define MINOR_ERR_TOPOLOGY_TREE               0x02
#define MINOR_ERR_INTERNAL_DATA_STRUCTURE     0x03
#define MINOR_ERR_NONCPU_PARENT_NODE          0x04
#define MINOR_ERR_INVALID_PORT_CONNECTION     0x05

/* Subcodes for ERR_IO_SAD_SETUP */
#define MINOR_ERR_INSUFFICIENT_RESOURCE_SPACE 0x01

/* Subcodes for ERR_UNSUPPORTED_TOPOLOGY */
#define MINOR_ERR_INVALID_SOCKET_TYPE         0x01
#define MINOR_ERR_INVALID_CPU_SOCKET_ID       0x02
#define MINOR_ERR_CBO_COUNT_MISMATCH          0x03
#define MINOR_ERR_HA_COUNT_MISMATCH           0x04
#define MINOR_ERR_R3QPI_COUNT_MISMATCH        0x05
#define MINOR_ERR_SKU_MISMATCH                0x06
#define MINOR_ERR_LEGACY_PCH_MISMATCH         0x07
#define MINOR_ERR_SBO_COUNT_MISMATCH          0x08
#define MINOR_ERR_INVALID_TOPOLOGY            0x09

/* Subcodes for ERR_FULL_SPEED_TRANSITION */
#define MINOR_ERR_LINK_SPEED_UNSUPPORTED      0x01
#define MINOR_ERR_MAX_CLK_RATIO_UNSUPPORTED   0x02
#define MINOR_ERR_QPI_ELEC_PARAM_NOT_FOUND    0x03

/* Subcodes for ERR_S3_RESUME */
#define MINOR_ERR_S3_RESUME_TOPOLOGY_MISMATCH 0x01

/* Subcodes for ERR_GENERAL_FAILURE */
#define MINOR_ERR_GENERAL                     0x01
#define MINOR_ERR_TIMEOUT                     0x02



/* QPI Warning Code - available thru QPI output structure only */

//
// Phy/Link layer
//
#define WARN_LINK_SLOW_SPEED_MODE             0x01
#define WARN_UNSUPPORTED_LINK_SPEED           0x02
#define WARN_LINK_FAILURE                     0x03
#define WARN_PER_LINK_OPTION_MISMATCH         0x04

//
// All other layers
//
#define WARN_MMIOH_BASE_UNSUPPORTED           0x09
#define WARN_MMIOH_SIZE_UNSUPPORTED           0x0A
#define WARN_RESOURCE_NOT_REQUESTED_FOR_CPU   0x0C
#define WARN_RESOURCE_REQUEST_NOT_MET         0x0D
#define WARN_VGA_TARGET_SOC_NOT_PRESENT       0x0E

#endif // _QPI_MISC_H_
