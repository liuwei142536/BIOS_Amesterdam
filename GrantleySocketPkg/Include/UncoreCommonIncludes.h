//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c) 2011 - 2014, Intel Corporation.

This source code and any documentation accompanying it ("Material") is furnished
under license and may only be used or copied in accordance with the terms of that
license.  No license, express or implied, by estoppel or otherwise, to any
intellectual property rights is granted to you by disclosure or delivery of these
Materials.  The Materials are subject to change without notice and should not be
construed as a commitment by Intel Corporation to market, license, sell or support
any product or technology.  Unless otherwise provided for in the license under which
this Material is provided, the Material is provided AS IS, with no warranties of
any kind, express or implied, including without limitation the implied warranties
of fitness, merchantability, or non-infringement.  Except as expressly permitted by
the license for the Material, neither Intel Corporation nor its suppliers assumes
any responsibility for any errors or inaccuracies that may appear herein.  Except
as expressly permitted by the license for the Material, no part of the Material
may be reproduced, stored in a retrieval system, transmitted in any form, or
distributed by any means without the express written consent of Intel Corporation.


Module Name:  

  UncoreCommonIncludes.h

Abstract:

  This file defines common equates.

--*/

#ifndef _UNCORE_COMMON_INCLUDES_H_
#define _UNCORE_COMMON_INCLUDES_H_

#ifndef V_INTEL_VID
#define V_INTEL_VID               0x8086
#endif

#ifndef MAX_CPU_SOCKET
#define MAX_CPU_SOCKET                  4
#endif
#define MAX_CPU_CORES_PER_SOCKET       24    // for BDX CPU
#define MAX_CORE_BITMASK          0xFFFFFF    // for BDX CPU

//
// Defines used for variables to be range checked before consumption.
// For MiniBIOS support, these are also defined in 
// GrantleySocketPkg\Library\MemoryQpiInit\CpuInit.c 
// GrantleySocketPkg\Library\MemoryQpiInit\Mem\InitMem.C
// so any changes here need to be updated in these files as well.
//
#define MAX_PROCESSOR_CORE_RATIO               100
#define DEFAULT_PROCESSOR_CORE_RATIO           23
#define MAX_SCRAMBLE_SEED_LOW                  65535
#define MAX_SCRAMBLE_SEED_HIGH                 65535             
#define ITURBO_MODE_TRADITIONAL                0
#define ITURBO_MODE_POWER_OPTIMIZED            4
#define MAX_PROCESSOR_TSEG                     4

//
// CPU Equates
//
#define MAX_THREAD                      2
#ifndef MAX_CORE
#define MAX_CORE                       24
#endif
#define MAX_DIE                         1
#ifndef MAX_SOCKET
#define MAX_SOCKET                      MAX_CPU_SOCKET
#endif
#define MAX_CPU_NUM                     (MAX_THREAD * MAX_CORE * MAX_DIE * MAX_SOCKET)
#define MAX_BR							2
#ifndef MAX_HA_CH
#define MAX_HA_CH                       4
#endif
#ifndef MAX_HA
#define MAX_HA                          2
#endif
#ifndef MAX_CH
#define MAX_CH                          4
#endif
#define MAX_MEM_NODE                    MAX_SOCKET*MAX_HA        // Two memory controllers per socket
#define MAX_CRS_ENTRIES_PER_NODE        4                        // Max number of ranges allowed on a memory node
#endif

//
//  CPU ExtFamily/Family/Model  bit[19:4] of cpuid(1)_eax
//
#define CPU_FAMILY_IVT     0x306e      // Ivytown CPU
#define CPU_FAMILY_HSX     0x306F      // Haswell CPU
#define CPU_FAMILY_BDX_DE  0x5066		// Broadwell Server CPU - BDX-DE
#define CPU_FAMILY_BDX     0x406F      // Broadwell Server CPU - BDX-EP

//
// CPU Types; this needs to be contiguous to assist in table look up
//
#define CPU_HSX              0
#define CPU_IVT              1
#define CPU_BDX              2
#define MAX_CPU_TYPES        3

//
// HSX CPU Sub types
//
#define CPU_HSX_1HA          0
#define CPU_HSX_2HA          1
#define CPU_HSX_EX           2

//
// BDX CPU Sub types
//
#define CPU_BDX_1HA          0
#define CPU_BDX_2HA          1
#define CPU_BDX_EX           2

// Define the different HSX package Sku's
#define HSX_PHY_CHOP_HCC 0x2
#define HSX_PHY_CHOP_MCC 0x1
#define HSX_PHY_CHOP_LCC 0x0

// Define the different BDX package Sku's
#define BDX_PHY_CHOP_HCC 0x2
#define BDX_PHY_CHOP_MCC 0x1
#define BDX_PHY_CHOP_LCC 0x0

//
// IVT CPU Sub types
//
#define CPU_IVT_EX           0

//
// Define the NS SKU Identification
// 0: DE, 1: NS Storage Sku, 2: NS Comms Sku
//
#define DE_SKU_ID            0
#define NS_STORAGE_SKU_ID    1
#define NS_COMMS_SKU_ID      2
