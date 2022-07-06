/*++
  This file contains an 'Intel Peripheral Driver' and uniquely  
  identified as "Intel Reference Module" and is                 
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/

/*++

Copyright (c)  1999 - 2011 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  PpmAslDefines.h

Abstract:

  IST/PPM reference code definitions.

  Acronyms:
    PPM   Platform Power Management

--*/
#ifndef _PPM_ASL_DEFINES_H_
#define _PPM_ASL_DEFINES_H_
//
// Platform Power Management Flags Bit Definitions:
//   These defines are also used in CPU0CST.ASL to check platform configuration
//   and build C-state table accordingly.
//
#define PPM_EIST                  0x1     // BIT 0 : Enhanced Intel Speed Step Technology.
#define PPM_C1                    0x2     // BIT 1 : C1 enabled, supported.
#define PPM_C1E                   0x4     // BIT 2 : C1E enabled.
#define PPM_C3                    0x8     // BIT 3 : C3 enabled, supported.
#define PPM_C6                    0x10    // BIT 4 : C6 enabled, supported.
#define PPM_C7                    0x20    // BIT 5 : C7 enabled, supported.
#define PPM_TM                    0x40    // BIT 6 : Adaptive Thermal Monitor.
#define PPM_TURBO                 0x80   // BIT 7 : Long duration turbo mode
#define PPM_SHORT_DURATION_TURBO  0x100   // BIT 8 : Short duration turbo mode
#define PPM_PROC_HOT              0x200   // BIT 9: Bi-directional ProcHot.
#define PPM_CMP                   0x400  // BIT 10: CMP.
#define PPM_TSTATES               0x800  // BIT 11: CPU throttling states
#define PPM_MWAIT_EXT             0x1000  // BIT 12: MONITIOR/MWAIT Extensions supported.
#define PPM_C1_AUTO_DEMOTION      0x2000  // BIT 13: C1 Auto demotion enabled
#define PPM_C3_AUTO_DEMOTION      0x4000 // BIT 14: C3 Auto demotion enabled
#define PPM_EEPST                 0x8000 // BIT 15: Energy efficient P-State Feature enabled
#define PPM_C7S                   0x10000 // BIT 16: C7S enabled, supported
#define PPM_C1_UNDEMOTION         0x20000 // BIT 17: C1 Undemotion enable
#define PPM_C3_UNDEMOTION         0x40000 // BIT 18: C3 Undemotion enable
#define PPM_TSTATE_FINE_GRAINED   0x80000 // BTI 19: Fine grained CPU Throttling states

#define PPM_C_STATES              (PPM_C1 + PPM_C3 + PPM_C6 + PPM_C7 + PPM_C7S)
#define PPM_ENHANCED_C_STATES     (PPM_C1E)

#define PPM_PKG_CX    (7 << 29) // Bit[31:29] Package C State Limit
#define PPM_CONTROL   (1 << 28) // Bit[28] PPM Control
#define PPM_OSCX                  0x08000000    // Bit[27] Map to OS ACPI C2 or OS ACPI C3
#define PPM_PSDF                  0x06000000    // Bit[26:25] PSD Func for HW_ALL, SW_ALL, SW_ANY
#define PPM_ME_DETECT             0x01000000    // Bit[24] 
#define PPM_C_3_6_7_S             0x10038	//(PPM_C3 + PPM_C6 + PPM_C7 + PPM_C7S)
#define PPM_C_3_6_7               0x38		//(PPM_C3 + PPM_C6 + PPM_C7)
#define PPM_C_3_6                 0x18		//(PPM_C3 + PPM_C6)
#define PPM_C_3_7                 0x28		//(PPM_C3 + PPM_C7)
#define PPM_C_3_7_S               0x10028	//(PPM_C3 + PPM_C7 + PPM_C7S)
#define PPM_C_6_7                 0x30		//(PPM_C6 + PPM_C7)
#define PPM_C_6_7_S               0x10030	//(PPM_C6 + PPM_C7 + PPM_C7S)
#define PPM_C_7_S                 0x10020	//(PPM_C7 + PPM_C7S)


//
// <TODO>-The following definitions are based on assumed location for the  ACPI
// Base Address.  Modify as necessary base on platform-specific requirements.
//
#define PCH_ACPI_LV2  0x414
#define PCH_ACPI_LV3  0x415
#define PCH_ACPI_LV4  0x416
#define PCH_ACPI_LV6  0x418

#define C1_LATENCY    3
#define C1_POWER      0x3E8

#define C3_LATENCY    0x3B
#define C3_POWER      0x1F4

#define C6_LATENCY    0x5D
#define C6_POWER      0x15E

#define C7_LATENCY    0x5D
#define C7_POWER      0xC8

#define C7S_LATENCY   0xA0     // Need to change to the correct value!!!
#define C7S_POWER     0x96

#define ACPI_C2       0x2
#define ACPI_C3       0x3

#endif
