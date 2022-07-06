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
#ifndef _QPI_ADAPTATION_H_
#define _QPI_ADAPTATION_H_

#include "DataTypes.h"
#include "PlatformHost.h"
#include "QpiDisc.h"
#include "QpiHost.h"
#include "SysHost.h"

// Commands for AP communication
#define  SYNC_HOST_PTR     0xADA01
#define  SYNC_HOST_DATA    0xADA02
#define  SYNC_SKT_DATA     0xADA03
#define  START_ADAPTATION  0xADA04
#define  AP_CLI_HLT        0xADAED

// 
// Usage: 
// BIT[08]: Adaptation Enabled
// BIT[09 - 14]: Indicates if 5.6/6.4/7.2/8.0/8.8/9.6 speed adaptation done
// BIT[15]: Indicates if this socket is done with adaptation
// 
#define SR_QPI_ADAPTATION_CSR             BIOSSCRATCHPAD0_UBOX_MISC_REG
#define SR_QPI_ADAPTATION_ENABLE_BIT      8
#define SR_56G_QPI_ADAPTATION_DONE_BIT    9
#define SOCKET_ADAPTATION_DONE_BIT        15

#define SR_QPI_ADAPTATION_LINK_0_CSR              (BIOSSTICKYLOCKBYPASSSCRATCHPAD0_UBOX_CFG_REG)
#define SR_QPI_ADAPTATION_LINK_1_CSR              (BIOSSTICKYLOCKBYPASSSCRATCHPAD1_UBOX_CFG_REG)
#define SR_QPI_ADAPTATION_LINK_2_CSR              (BIOSSTICKYLOCKBYPASSSCRATCHPAD2_UBOX_CFG_REG)

#define DIRECTION_INFO_WIDTH     2            // each bundle has 2-bit information. 

#define MAX_AGC_VALUE        15
#define MIN_AGC_VALUE        (UINT8)(-15)
#define MAX_VSWING_VALUE     7
#define MIN_VSWING_VALUE     0

#define MAX_FIR_VALUE        15
#define MIN_FIR_VALUE        (UINT8)(-15)
#define MAX_TXEQ_RANGE       10

#define ADAPTATION_START_READY_BIT 31             //  indicate adaptation ready to start  for each link parallel process
#define NEED_LINK_RESET_BIT        30             //  indicate this link need to be reset for each link parallel process
#define AJUST_OPERATION_DONE_BIT   29             //  indicate step 2 of this link is completely for each link parallel process
#define DONE_PHY_RESET_STEP_BIT    28             //  indicate done phy reset step for each link parallel process


typedef enum {
  ADAPTATION_NOT_REQUIRED,
  ADAPTATION_REQUIRED,
  ADAPTATION_REQUIRED_SPEED_NOT_SUPPORTED,
  ADAPTATION_FEATURE_NOT_SUPPORTED
}ADAPTATION_OPERATION;

typedef enum {
  TXEQ_ADAPTATION,  
  VSWING_ADAPTATION
 }ADAPTATION_TYPE;

#pragma pack(1)

//
// Data structure that contains the adapted TX parameters and its corresponding RX metrics
// Note that we don't use signed numbers for easy register manupulation. The sign bits in the
// hw registers not MSbit. So by storing it as unsigned, we don't need to do processing.
// But care should be taken to make sure the comparisions carried out as signed instead of unsigned.
// The metrics (AGC & FIR) are per lane while the corresponding knobs (Vswing & TXEQ) are per 
// bundle. So we will have the data structure also defined at that granularity for easy data manupulation.
//
typedef struct {
  UINT8     Vswing[MAX_QPI_BUNDLES];        // Per bundle TX Vswing setting
  UINT16    Agc[MAX_QPI_BUNDLES * 2];       // Per lane peer Rx AGC metrics
  UINT32    TxEqLvl[MAX_QPI_BUNDLES];       // Per bundle TXEQ level setting for all 4 levels; Byte0 - Level 0, etc
  UINT8     FirDelta[MAX_QPI_BUNDLES * 2];  // Per lane peer Rx FIR metrics; 1:0 - precursor, 3:2 - cursor, 5:4 - postcursor
}QPI_ADAPT_METRICS; 


QPI_STATUS
ProgramSlowBackChannel(
  struct sysHost            *host,
  UINT8                     Cpu,
  UINT8                     Link,
  UINT8                     Enable
);

UINT8 
GetCurrentQpiSpeed(
  struct sysHost            *host,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal,
  UINT8                     cpu
  );

QPI_STATUS
QpiAdaptationRendezvousPbsp(
  PSYSHOST                  host,
  QPI_SOCKET_DATA           *SocketData
);

VOID
TransferData (
   struct sysHost             *host,
   UINT8   ApSocId,
   VOID    *Buffer,
   UINT32  BufSize
 );

VOID
ReceiveData (
   UINT8   SocId,
   UINT32  MmCfgBdf,
   VOID    *Buffer,
   UINT32  BufSize
 );

VOID
DispatchAPExecuteAdaptation (
  struct sysHost             *host,
  QPI_SOCKET_DATA    *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

VOID
HaltAllAps (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

#define AP_READ_PCI_CFG(ApSocId, MmCfgBdf, RegOffset)  *((volatile UINT32 *) ((MmCfgBdf) | ((RegOffset) & 0xFFF)))
#define AP_WRITE_PCI_CFG(ApSocId, MmCfgBdf, RegOffset, Data32)  *((volatile UINT32 *) ((MmCfgBdf) | ((RegOffset) & 0xFFF))) = Data32


#pragma pack()

#endif // _QPI_ADAPTATION_H_
