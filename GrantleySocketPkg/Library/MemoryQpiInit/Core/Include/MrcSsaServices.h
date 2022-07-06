/** @file
  This file contains the definition of the SSA services PPI.

@copyright
  Copyright (c) 2012 - 2014 Intel Corporation. All rights reserved.
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
#ifndef __MRC_SSA_SERVICES_H__
#define __MRC_SSA_SERVICES_H__

#include <Token.h> // AptioV server override

#ifdef SSA_FLAG
#define SSA_SERVER_FLAG
#pragma pack (push, 1)

typedef struct _SSA_BIOS_SERVICES_PPI SSA_BIOS_SERVICES_PPI;

#ifdef SSA_CLIENT_FLAG
#if 0 // Change this to #if [0|1] to turn [off|on] additional SSA debug messages.
#define SSA_DEBUG_PRINT                          (1)
#define SSA_DEBUG_MSG(DEBUG, LEVEL, FORMAT, ...) MrcPrintf (DEBUG, LEVEL, FORMAT, ##__VA_ARGS__)
#define SSA_PRINT_MEMORY(DEBUG, START, SIZE)     MrcPrintMemory (DEBUG, START, SIZE)
#else
#define SSA_DEBUG_MSG(DEBUG, LEVEL, FORMAT, ...)
#define SSA_PRINT_MEMORY(DEBUG, START, SIZE)
#endif
#ifdef SSA_DEBUG_PRINT
#define SSA_PARAM_ERROR_CHECK           (1)
#endif
#endif // SSA_CLIENT_FLAG

#include "MrcCommonTypes.h"

///
/// SSA function return error codes.
///
typedef enum {
  Success,                                ///< The function completed successfully.
  Failure,                                ///< Generic, non specified failure.
  NotYetAvailable,                        ///< The function is not yet available.
  NotAvailable,                           ///< The function or selected item is not available .
  UnsupportedValue,                       ///< A function parameter is incorrect.
  SsaStatusMax,                           ///< SSA_STATUS enumeration maximum value.
  SsaStatusDelim = INT32_MAX              ///< This value ensures the enum size is consistent on both sides of the PPI.
} SSA_STATUS;

#pragma pack (pop)



#ifdef SSA_CLIENT_FLAG
#ifdef MRC_MINIBIOS_BUILD
#include "MrcMiniBiosEfiDefs.h"
#else
#include <Library/DebugLib.h>
#include "SsaCallbackPeim.h"
#endif // MRC_MINIBIOS_BUILD
#include "MrcInterface.h"
#define SSA_CONST const
#endif // SSA_CLIENT_FLAG

#include "SsaCommonConfig.h"
#include "SsaResultsConfig.h"
#include "SsaMemoryConfig.h"
#include "SsaMemoryClientConfig.h"
#include "SsaMemoryServerConfig.h"
#include "SsaMemoryDdrtConfig.h"

#pragma pack (push, 1)

///
/// SSA BIOS Services PPI
///
struct _SSA_BIOS_SERVICES_PPI {
  UINT32                   Revision;                  ///< Incremented when a backwards compatible binary change is made to the PPI.
  SSA_COMMON_CONFIG        *SsaCommonConfig;          ///< Pointer to the BIOS-SSA common functions.
  SSA_RESULTS_CONFIG       *SsaResultsConfig;         ///< Pointer to the BIOS results related functions.
  SSA_MEMORY_CONFIG        *SsaMemoryConfig;          ///< Pointer to the BIOS-SSA memory related functions.
  SSA_MEMORY_CLIENT_CONFIG *SsaMemoryClientConfig;    ///< Pointer to the BIOS-SSA client memory related functions.
  SSA_MEMORY_SERVER_CONFIG *SsaMemoryServerConfig;    ///< Pointer to the BIOS-SSA server memory related functions.
  SSA_MEMORY_DDRT_CONFIG   *SsaMemoryDdrtConfig;      ///< Pointer to the BIOS-SSA NVMDIMM memory related functions.
};

#pragma pack (pop)
#else
// Future placeholder: BSSA code intentionally left out for now
#endif //SSA_FLAG

#endif // __MRC_SSA_SERVICES_H__
