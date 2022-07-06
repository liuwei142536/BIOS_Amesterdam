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
// *      Copyright (c) 2004 - 2015 Intel Corp.                             *
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
// *      This file contains all the Si specific QPI information.           *
// *                                                                        *
// **************************************************************************
--*/

#ifndef _QPI_SI_H_
#define _QPI_SI_H_

#pragma warning (disable: 4127 4214 4100)     // disable C4127: constant conditional expression
#include "DataTypes.h"
#include "PlatformHost.h"
#include "RcRegs.h"

/*********************************************************
                QPI Topology Related
*********************************************************/
#define SI_MAX_CPU_SOCKETS  8    // Maximum CPU sockets  supported by Si
#define SI_MAX_QPI_PORTS    3    // Maximum QPI ports supported by Si
#define LEGACY_CPU          0    // Legacy CPU socket id
#define SOCKET_TYPE_CPU     0


/*********************************************************
                      CBO
*********************************************************/
#define MAX_CBO             24

//
// Cbo RTID Pool modes
//
#define SINGLE_POOL01_MODE  0
#define DOUBLE_POOL10_MODE  1
#define DOUBLE_POOL21_MODE  2
#define DOUBLE_POOL20_MODE  3
#define DOUBLE_POOL8S_MODE  4
#define DOUBLE_POOLIVT_MODE  6

//
// Cbo RTID Pools
//
#define MAX_POOLS_HSX       18
#define MAX_POOLS_BDX       20
// Using no of Cbo RTID pools of BDX as the maximum supported
#define MAX_CBO_POOLS       MAX_POOLS_BDX

//
#define MAX_POOL_SETS_HSX   2
#define MAX_POOL_SETS_BDX   3
// Using no of RTID pool sets of BDX as the maximum supported
#define MAX_POOL_SETS       MAX_POOL_SETS_BDX

//
// RTIDs related. Each Cbo is allowed to use 2 pools (aka Double Pool Mode) to communicate with 
// local HA and also remote HA in certain BT modes. The following is total number of RTIDs
// available in each pool to all CBos of a socket.
// 


#define TOT_RTIDS_PER_POOL   64 // Local, remote, first, second pools all use this value for all BT modes except for mode 2

#define TOT_RTIDS_PER_LOCAL_POOL_HSX  48     // Local pool for HSX mode 2
#define TOT_RTIDS_PER_LOCAL_POOL_ALT_HSX  64 // Applicable to alt mode 2 only (Overlap between extended and non-extended RTID for local in RBT)
#define TOT_RTIDS_PER_REMOTE_POOL_HSX   48   // Applicable to HSX BT mode 2 only
#define TOT_RTIDS_PER_SECOND_POOL_HSX   64   // Applicable to HSX BT mode 2 only

#define TOT_RTIDS_PER_LOCAL_FIRST_POOL_BDX      54     // Local pool xRTID=0 for BDX BT Mode 2
#define TOT_RTIDS_PER_LOCAL_SECOND_POOL_BDX     64     // Local pool xRTID=1 for BDX BT mode 2
#define TOT_RTIDS_PER_LOCAL_SPECIAL_POOL_BDX    10     // Local pool xRTID=0 and RTIDs in range 54-63 for BDX BT mode 2
#define TOT_RTIDS_PER_REMOTE_POOL_BDX           54     // Remote pool for BDX BT Mode 2


#define MAX_RTID_PER_CBO_PER_POOL   8

#define MAX_RTID_PER_POOL   8
#define MAX_RTID_PER_SOC    64
#define MAX_RTID_SYSTEM     128
#define MAX_ISOC_RTID       12
#define MAX_ISOC_VCP_RTID   6
#define MAX_ISOC_VC1_RTID   6
#define CBO_WB_RTIDS        1
#define UBOX_RTIDS_HSX      4 // Including IIO for HSX
#define UBOX_RTIDS_IVT      3 // NOT include IIO for IVT

#define TOT_CBO_CREDIT_IVT  30
#define TOT_CBO_CREDIT_HSX  36
#define TOT_CBO_CREDIT_BDX  48
#define MAX_LEFT_CBO        8
#define MAX_RIGHT_CBO       10

//
// SAD/System Address Map related
//
#define MAX_SAD_TARGETS     8
#define SADTGT_SIZE         3 // SAD target is 3 bits in size
#define SAD_BUS_GRANTY      32
#define SAD_LEG_IO_GRANTY   0x2000          // 8KB
#define SAD_IOAPIC_GRANTY   0x20000         // 128KB
#define SAD_MMIOL_GRANTY    0x4000000       // 64MB
#define MAX_ADDR_LINE       46              // 46 Address bits are supported
#define IOAPIC_BASE         0xFEC00000
#define MMIOL_LIMIT         0xFBFFFFFF      // (4GB - 64MB - 1)
#define MMIOH_BASE          0x3800          // address above 4GB, so the 46 bits of physical address [45:0] is 0x3800_0000_0000  (65536G - 8192G)


/*********************************************************
                      HA
*********************************************************/
#define SI_MAX_HA           2
#define SOCID_TO_NID(SOCID, BIT2)   ((((SOCID) & 0x4) << 1) | ((SOCID) & 0x3) | ((BIT2) << 2))

typedef enum {
 BT_MODE_0    = 0,
 BT_MODE_1,
 BT_MODE_2,
 BT_MODE_3,
 BT_MODE_MAX
} BT_MODE;

typedef enum {
 EARLY_SNOOP_MODE    = 0,
 HOME_SNOOP_MODE,    
 HOME_DIR_SNOOP_MODE, 
 HOME_DIR_OSB_SNOOP_MODE,
 SNOOP_MODE_MAX
} SNOOP_MODE;


/*********************************************************
                      R3QPI
*********************************************************/
#define MAX_R3QPI            2
#define MAX_PORT_IN_R3QPI    2
#define PORT_TO_R3QPI(PORT)   (PORT >> 1)


/*********************************************************
                      QPI Agent Link Layer
*********************************************************/

// Route Back Table (RBT) modes
#define ROUTE_BACK_DYNAMIC    0
#define ROUTE_BACK_STATIC     1

// RBT Entries supported by QPIAgent
#define MAX_RBT_ENTRIES       128

#define RING_STOP_ID_UBOX_IVT   15
#define RING_STOP_ID_UBOX_HSX   31

#define QPI_INVALID_PORT      0xFF

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
#define CSIPHYPORT_STATE_RESET_CALIB        0x1
#define CSIPHYPORT_STATE_DETECT_CLKTERM     0x2
#define CSIPHYPORT_STATE_L0                 0xf
#define CSIPHYPORT_STATE_LOOPBACK_MASTER    0x12


#define CSI_LINK_DEFAULT_RESET_MODE         0x1
#define CSI_LINK_SOFT_RESET_MODE            0x0

#define MAX_QPI_BUNDLES      10                                                      
#define MAX_QPI_LANES        (2*MAX_QPI_BUNDLES)

/*********************************************************
                      UBOX
*********************************************************/
//
// Supported System configuration. This clasification is based on RTID/Credit setting, coherency setting 
// and not based on topology alone. For example, a 3S topology will be treated as 4S config so that it
// can have same RTID, Credit and other coherency setting as 4S topology.
//
typedef enum {
  SYS_CONFIG_1S = 0,  // EP & EX
  SYS_CONFIG_2SEN,    // EP only; for EX 2S with 1 QPI will be treated as 4S config
  SYS_CONFIG_2SEP,    // EP & EX
  SYS_CONFIG_4S,      // EP & EX; no need to distinguish between Ring and Fully connected topologies
  SYS_CONFIG_8S,      // EX only
  SYS_CONFIG_MAX
} SYS_CONFIG;

//
// Macro to create mask. Ex. FLD_MASK(3, 5) --> 0000 0000 1110 0000
//
#define FLD_MASK(Size, Pos) ((~((~0) << (Size))) << (Pos))


//
// Should be removed once the ConfigDB issue with LEP0/1/2/3 is fixed and use the definition from auto generated header file
//
#pragma pack(1)
typedef union {
  struct {
    UINT32  Port : 5;       // [4:0]   - Port#
    UINT32  NumNodeID : 3;  // [7:5]   - #Number of NIDs in peer socket
    UINT32  NodeID : 7;     // [14:8]  - NodeID[9:3] of the peer socket
    UINT32  Rsvd1 : 1;      // [15:15]
    UINT32  LlrWrap : 8;    // [23:16] - Link Level Retry buffer length supported by Peer
    UINT32  Rsvd2 : 8;      // [31:24]
  } Fields;
  UINT32  RawData;
} QPI_LEP0;

typedef union {
  struct {
    UINT8 ConfigAgent : 1;  // [0] - Config Agent Present
    UINT8 FwAgent : 1;      // [1] - Firmware Agent Present
    UINT8 RouterAgent : 1;  // [2] - Router Agent Present
    UINT8 Rsvd1 : 1;        // [3]
    UINT8 LtAgent : 1;      // [4] - LT Agent Present
    UINT8 IoProxyAgent : 1; // [5] - IO Proxy Agent Present
    UINT8 HomeAgent : 1;    // [6] - Home Agent Present
    UINT8 CachingAgent : 1; // [7] - Caching Agent Present
  } Fields;
  UINT8 RawData;
} QPI_LEP2;
#pragma pack()

/*********************************************************
                           PCU
*********************************************************/

typedef enum {
  SKU_EN1     = 0,
  SKU_EN2,                     
  SKU_EP,
  SKU_EXB,
  SKU_EXA,
  SKU_DE,
  SKU_NS = 13,
  SKU_MAX
} SKU_TYPE;

typedef enum {
  EXA_SUB_SKU_UP     = 0,
  EXA_SUB_SKU_2S,
  EXA_SUB_SKU_4S,
  EXA_SUB_SKU_8S 
} EXA_SUB_SKU_TYPE;

// Indication about whether it is Storage Sku or Comms Sku in BDX-NS
#define NS_STORAGE_SEG_SEL    BIT16

// Default pointer separation values for BGF overrides
//
#define PTR_SEP_VAL_HSX_A0     2
#define PTR_SEP_VAL_HSX_BDX    0

#endif // _QPI_SI_H_
