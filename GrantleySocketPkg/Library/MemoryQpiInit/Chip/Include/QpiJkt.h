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
// *      Copyright (c) 2004 - 2011 Intel Corp.                             *
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
// *      This file contains all the QPI specific information.   *
// *                                                                        *
// **************************************************************************
--*/

#ifndef _QPI_JKT_H_
#define _QPI_JKT_H_

#pragma warning (disable: 4127 4214 4100)     // disable C4127: constant conditional expression
#include "DataTypes.h"
#include "RcRegs.h"
#ifndef ASM_INC

/*********************************************************
                      CBO
*********************************************************/
#define JKT_MAX_CPU_SOCKETS      4 // Maximum CPU sockets  supported by Si
#define JKT_LEGACY_CPU      0
#define MAX_CBO             8
#define CBO_TO_DEV(CBO)     (((CBO) & 1) + 12)  // Convert CBO number into Device Number
#define CBO_TO_NBC_FUN(CBO) ((CBO) / 2)         // Convert CBO number into Non-Broadcast Function Number
#define MAX_RTID_PER_POOL   8
#define MAX_RTID_PER_SOC    64
#define MAX_RTID_SYSTEM     128
#define MAX_ISOC_RTID       8
#define MAX_ISOC_VCP_RTID   4
#define MAX_ISOC_VC1_RTID   4
#define CBO_WB_RTIDS        1
#define UBOX_RTIDS          3

//
// SAD/System Address Map related
//
#define MAX_SAD_TARGETS     8
#define SADTGT_SIZE               3 // SAD target is 3bits in size
#define SAD_BUS_GRANTY      32
#define SAD_LEG_IO_GRANTY   0x2000          // 8KB
#define SAD_IOAPIC_GRANTY   0x20000          // 128KB
#define SAD_MMIOL_GRANTY    0x4000000      // 64MB
#define MAX_JKT_ADDR_LINE   46              // 46 Address bits are supported by JKT
#define IOAPIC_BASE         0xFEC00000
#define MMIOL_LIMIT         0xFBFFFFFF      // (4GB - 64MB - 1)
#define MMIOH_BASE          0x3C0000000000  // (65536G - 4096G)

#define EARLY_SNOOP_MODE    0
#define HOME_SNOOP_MODE     1
#define HOME_DIR_SNOOP_MODE 2

#define SINGLE_POOL01_MODE  0
#define DOUBLE_POOL10_MODE  1
#define DOUBLE_POOL21_MODE  2
#define DOUBLE_POOL20_MODE  3


/*********************************************************
                      QPI Agent Link Layer
*********************************************************/

// Route Back Table (RBT) modes
#define ROUTE_BACK_DYNAMIC    0
#define ROUTE_BACK_STATIC     1

// RBT Entries supported by QPIAgent
#define MAX_RBT_ENTRIES       128

#define RING_STOP_ID_UBOX     15

// Constants related to QPI Link
#define QPI_JKT_PORTS         2
#define QPI_JKT_INVALID_PORT  0xFF

//
// Link Layer Init State Machine
//
typedef enum {
  LINK_SM_WAIT_FOR_PHY_READY        = 0,
  LINK_SM_RSVD1,
  LINK_SM_SENDING_READY_FOR_INIT,
  LINK_SM_PARAMETER_EXCHANGE,
  LINK_SM_SENDING_READY_FOR_NORMALOP,
  LINK_SM_RSVD5,
  LINK_SM_NORMAL_OPERATION,
  LINK_SM_LINK_LEVEL_RETRY,
  LINK_SM_LINK_ABORT,
  LINK_SM_PARAM_EX_STALLED,
  LINK_SM_PARAM_EX_PROCEEDING,
  LINK_SM_WAIT_FOR_NORMAL,
  LINK_SM_LOCAL_LINK_RESET,
  LINK_SM_MAX
} LINK_SM;

//
// QPI Agent Type Definition
//
typedef enum {
  QPI_CONFIG_AGENT                  = 0,
  QPI_FIRMWARE_AGENT,
  QPI_ROUTER_AGENT,
  QPI_AGENT_RSVD,
  QPI_LT_AGENT,
  QPI_IO_PROXY_AGENT,
  QPI_HOME_AGENT,
  QPI_CACHING_AGENT,
  QPI_MAX_AGENT
} QPI_AGENT_TYPE;

/*********************************************************
                      QPI Agent Phy Layer
*********************************************************/

/*********************************************************
                      UBOX
*********************************************************/
//
// Supported Topologies; note that QPIRC is totally platform agnostic and it will construe a system
// QPI topology as one of these purely based on Link Exchanged Parameter info.
//
typedef enum {
  SYS_CONFIG_1S                     = 0,
  SYS_CONFIG_2SEN,
  SYS_CONFIG_2SEP,
  SYS_CONFIG_4S,
  SYS_CONFIG_MAX
} SYS_CONFIG;

//
// Macro to create mask. Ex. FLD_MASK(3, 5) --> 0000 0000 1110 0000
//
#define FLD_MASK(Size, Pos) ((~((~0) << (Size))) << (Pos))
#endif// ASM_INC

#endif // _QPI_JKT_H_
