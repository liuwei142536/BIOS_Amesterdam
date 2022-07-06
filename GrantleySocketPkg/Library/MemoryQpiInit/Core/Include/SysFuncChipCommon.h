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
#ifndef  _sysfuncchipcommon_h
#define  _sysfuncchipcommon_h

#include "SysHost.h"

#include "Uefi.h"   // We need this for the EFI_XXXXX errorcode macros
#include <Cpu\CpuBaseLib.h>

#include "PlatformFuncCommon.h"
#include "PlatformApi.h"
#include "MemFunc.h"
#include "CpuPciAccess.h"
#include "SysFunc.h"

//
// Global defines
//
#define  SUCCESS  0
#define  FAILURE  1
#define  RETRY    2

#ifndef  NULL
#define  NULL ((void *) 0)
#endif


#define  BUS_PCH  2
//
// This macro must be used to access only ICH/PCH regs using
// PCI access routines in PciAccess.c. Note that bit[27:20] is encoded with 2 to indicate PCH/ICH bus
// to distinguish with IIO/Uncore buses.
//
#define REG_ADDR( bus, dev, func, reg, size ) ((size << 28) + ((bus + BUS_PCH) << 20) + (dev << 15) + (func << 12) + reg)

//
// DebugCmd.c
//
#define SERIAL_CONSOLE_PIPE      0
#define SERIAL_BSD_PIPE          1

#if !defined(SIM_BUILD) && defined(IA32) || defined(RC_SIM)

#define PUSH_EXIT_FRAME(exitFrame, exitVector) \
   _asm {pushfd} \
   _asm {push  esi} \
   _asm {push  edi} \
   _asm {push  ebp} \
   _asm {push  edi} \
   _asm {mov   exitFrame, esp} \
   _asm {add   exitFrame, 4} \
   _asm {mov   exitVector, OFFSET mrcExit} \
   _asm {pop   edi}

#define POP_EXIT_FRAME(status) \
   _asm {mov   eax, status} \
   _asm {mrcExit:} \
   _asm {pop   ebp} \
   _asm {pop   edi} \
   _asm {pop   esi} \
   _asm {popfd} \
   _asm {mov   status, eax}

#else //  !defined(SIM_BUILD) && defined(IA32)

#define PUSH_EXIT_FRAME(host)
#define POP_EXIT_FRAME(status)

#endif //  !defined(SIM_BUILD) && defined(IA32)


//
// CpuPciAccess.c
//
UINT8  *GetCpuPciCfgAddress (PSYSHOST host, UINT8 SocId, UINT8 BoxInst, UINT32 Offset, UINT8 *Size); // CHIP / QPI

#define ISNODEONLINED(host) \
  (host->var.common.memHpSupport && \
  host->var.mem.hotPlugMode)

#define ISMEMINITED(host, node) \
  (host->var.mem.memNodeHP == node && \
  (host->var.mem.actionHP & BIT4) == BIT4)

#define ISMEMMAPPED(host, node) \
  (host->var.mem.memNodeHP == node && \
  (host->var.mem.actionHP & BIT5) == BIT5)

#define GETONLINEACTION(host, act)   ((host->var.mem.actionHP & act) == act)

#define SKIPTHISNODE(host, node) \
  ((ISNODEONLINED(host) == TRUE) ? ((ISMEMINITED(host, node) || ISMEMMAPPED(host, node)) ? FALSE : TRUE) : \
  (((host->var.mem.memRiserPresent & (1 << node)) == FALSE) ? TRUE : FALSE))

#endif   // _sysfuncchip_h
