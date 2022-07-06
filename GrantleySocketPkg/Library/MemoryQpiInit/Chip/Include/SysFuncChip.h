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
#ifndef  _sysfuncchip_h
#define  _sysfuncchip_h

#include "QpiLib.h"
#include "OemMemoryQpiInit.h"

//
// CpuPciAccess.c
//
#ifndef ASM_INC
UINT32 GetRegisterOffset (PSYSHOST host, CSR_OFFSET RegOffset);                                      // CHIP
UINT32 GetMmcfgAddress(PSYSHOST Host, UINT8 Flag);

//
// PrintfChip.c
//
void   debugPrintMmrc (UINT32 dbgLevel, char* Format, ...);            // MMRC

//
// MemoryQpiInit.c / MailBox.c
//
UINT64_STRUCT WriteBios2VcuMailboxCommand (struct sysHost *host, UINT8 socket, UINT32 dwordCommand, UINT32 dworddata);       // CHIP

#endif

UINT8
IsMccChop (
 PSYSHOST    host                              // Pointer to the system host (root) structure
 );


UINT8
IsHccChop (
 PSYSHOST    host                              // Pointer to the system host (root) structure
 );


UINT8
IsDEHccSku (
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );
 
void 
EnableErrorFlow(PSYSHOST host, UINT8 socket);

void EnableWrCRC (PSYSHOST host, UINT8 socket);

#endif   // _sysfuncchip_h
