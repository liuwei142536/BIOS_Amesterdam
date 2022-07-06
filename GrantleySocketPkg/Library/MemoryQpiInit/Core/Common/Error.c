/**
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
**/
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
#ifdef _MSC_VER
#pragma warning(disable:4740)
#endif

#include "SysFunc.h"


//
// Local Prototypes
//
void   Exit(PSYSHOST host, UINT8 majorCode, UINT8 minorCode);

/**

  Log warning to the warning log

  @param host              - pointer to sysHost
  @param warningCode       - Major warning code
  @param minorWarningCode  - Minor warning code
  @param socket              - socket number
  @param ch                - channel number
  @param dimm              - dimm number
  @param rank              - rank number

**/
void
OutputWarning (
  PSYSHOST host,
  UINT8    warningCode,
  UINT8    minorWarningCode,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank
  )
{
  PlatformOutputWarning(host, warningCode, minorWarningCode, socket, ch, dimm, rank);
  //
  // Log the warning
  //
  LogWarning (host, warningCode, minorWarningCode, (socket << 24) | (ch << 16) | (dimm << 8) | rank);
}


/**

  Log a warning to the host structure

  @param host      - Pointer to the system host (root) structure
  @param majorCode - Major error code to log
  @param minorCode - Minor error code to log
  @param logData   - 32 bit error specific data to log

  @retval N/A

**/
void
LogWarning (
           PSYSHOST host,
           UINT8    majorCode,
           UINT8    minorCode,
           UINT32   logData
           )
{
  UINT8   promoteWarning;
  UINT32  i;

  promoteWarning = 0;
  //
  // Append warningCode to the status log
  //
  i = host->var.common.status.index;
  if (i < MAX_LOG) {
    host->var.common.status.log[i].code = (UINT32) ((majorCode << 8) | minorCode);
    host->var.common.status.log[i].data = logData;
    host->var.common.status.index++;
  }

  rcPrintf ((
           host,
           "\nA warning has been logged! Warning Code = 0x%X, Minor Warning Code = 0x%X, Data = 0x%X\n",
           majorCode,
           minorCode,
           logData
           ));

  if (((logData >> 24) & 0xFF) != 0xFF) {
    rcPrintf ((host, "S%d", (logData >> 24) & 0xFF));
  }

  if (((logData >> 16) & 0xFF) != 0xFF) {
    rcPrintf ((host, " Ch%d", (logData >> 16) & 0xFF));
  }

  if (((logData >> 8) & 0xFF) != 0xFF) {
    rcPrintf ((host, " DIMM%d", (logData >> 8) & 0xFF));
  }

  if ((logData & 0xFF) != 0xFF) {
    rcPrintf ((host, " Rank%d", logData & 0xFF));
  }

  rcPrintf ((host, "\n\n"));

  //
  // Promote warning based on setup option
  //
  if (host->setup.common.options & PROMOTE_WARN_EN) {
    promoteWarning = 1;

    //
    // Search platform exception list
    //
    for (i = 0; i < host->var.common.promoteWarnLimit; i++) {
      if ((host->var.common.promoteWarnList[i] == (UINT16) ((majorCode << 8) | minorCode)) ||
          (host->var.common.promoteWarnList[i] == (UINT16) ((majorCode << 8) | WARN_MINOR_WILDCARD))
         ) {
        promoteWarning = 0;
        break;
      }
    }
  }

  if ((majorCode >= WARN_FPT_CORRECTABLE_ERROR) && (majorCode <= WARN_FPT_UNCORRECTABLE_ERROR)) {
    if (host->setup.common.options & PROMOTE_MRC_WARN_EN) {
      promoteWarning = 1;
    } else {
      promoteWarning = 0;
    }
  }

  //
  // Call platform hook to handle warnings
  //
  PlatformLogWarning (host, majorCode, minorCode, logData);

  //
  // Promote warning to fatal error
  //
  if (promoteWarning) {

    rcPrintf ((host, "Warning upgraded to Fatal Error!\n"));
    FatalError (host, majorCode, minorCode);
  }
}


/**

  Log Error to the warning log

  @param host              - pointer to sysHost
  @param ErrorCode         - Major warning code
  @param minorErrorCode    - Minor warning code
  @param socket            - socket number
  @param ch                - channel number
  @param dimm              - dimm number
  @param rank              - rank number

**/
void
OutputError (
  PSYSHOST host,
  UINT8    ErrorCode,
  UINT8    minorErrorCode,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank
  )
{
  PlatformOutputError (host, ErrorCode, minorErrorCode, socket, ch, dimm, rank);
  //
  // Log the error
  //
  LogError (host, ErrorCode, minorErrorCode, (socket << 24) | (ch << 16) | (dimm << 8) | rank);
}

/**

  Log an Error to the host structure

  @param host      - Pointer to the system host (root) structure
  @param majorCode - Major error code to log
  @param minorCode - Minor error code to log
  @param logData   - 32 bit error specific data to log

  @retval N/A

**/
void
LogError (
         PSYSHOST host,
         UINT8    majorCode,
         UINT8    minorCode,
         UINT32   logData
         )
{

  UINT32  i;

  //
  // Append errorCode to the status log
  //
  i = host->var.common.status.index;
  if (i < MAX_LOG) {
    host->var.common.status.log[i].code = (UINT32) ((majorCode << 8) | minorCode);
    host->var.common.status.log[i].data = logData;
    host->var.common.status.index++;
  }

  //
  // Call platform hook to handle warnings
  //
  PlatformLogWarning (host, majorCode, minorCode, logData);


  rcPrintf ((
           host,
           "\nA Error logged! Error Code = 0x%X, Minor Error Code = 0x%X, Data = 0x%X\n",
           majorCode,
           minorCode,
           logData
           ));

  if (((logData >> 24) & 0xFF) != 0xFF) {
    rcPrintf ((host, "S%d", (logData >> 24) & 0xFF));
  }

  if (((logData >> 16) & 0xFF) != 0xFF) {
    rcPrintf ((host, " Ch%d", (logData >> 16) & 0xFF));
  }

  if (((logData >> 8) & 0xFF) != 0xFF) {
    rcPrintf ((host, " DIMM%d", (logData >> 8) & 0xFF));
  }

  if ((logData & 0xFF) != 0xFF) {
    rcPrintf ((host, " Rank%d", logData & 0xFF));
  }

  rcPrintf ((host, "\n\n"));

  //
  // Promote fatal error
  //
  FatalError (host, majorCode, minorCode);

}



/**

  Halts the platform

  @param host      - Pointer to the system host (root) structure

  @retval N/A

**/
void
HaltOnError (
            PSYSHOST host,
            UINT8    majorCode,
            UINT8    minorCode
            )
{
  UINT8   haltOnError;

  //
  // Default halt on error based on setup option
  //
  if (host->setup.common.options & HALT_ON_ERROR_EN) {
    haltOnError = 1;
  } else {
    haltOnError = 0;
  }

  //
  // Loop forever
  //
  RcDeadLoop (haltOnError);

  //
  // Exit now!
  //
  Exit (host, majorCode, minorCode);
} // HaltOnError


/**

  Exits reference code

  @param host      - Pointer to the system host (root) structure
  @param majorCode - Major error code to log
  @param minorCode - Minor error code to log

  @retval N/A

**/
void
Exit (
     PSYSHOST host,
     UINT8    majorCode,
     UINT8    minorCode
     )
{
#if defined (IA32) || defined (RC_SIM)
  UINT32  exitFrame;
  UINT32  exitVector;

  exitFrame   = host->var.common.exitFrame;
  exitVector  = host->var.common.exitVector;

  _asm
  {
    xor eax, eax
    mov ah, majorCode
    mov al, minorCode
    mov esp, exitFrame
    mov edi, exitVector
    jmp edi
  }
#endif // defined (IA32) || defined (RC_SIM)
}

/**

  Adds error/exception to the promoteWarningList in host structure if possible

  @param host      - Pointer to the system host (root) structure
  @param majorCode - Major error code to log
  @param minorCode - Minor error code to log

  @retval 0 - Successfully added to log
  @retval 1 - Log full

**/
UINT32
SetPromoteWarningException (
                           PSYSHOST host,
                           UINT8    majorCode,
                           UINT8    minorCode
                           )
{
  UINT32  status;
  UINT32  i;

  status = SUCCESS;
  //
  // Search warning list for unused entry
  //
  for (i = 0; i < MAX_PROMOTE_WARN_LIMIT; i++) {
    if (host->var.common.promoteWarnList[i] == 0) {
      host->var.common.promoteWarnList[i] = (UINT16) ((majorCode << 8) | minorCode);
      if (i == host->var.common.promoteWarnLimit) {
        host->var.common.promoteWarnLimit++;
      }
      break;
    }
  }

  if (i == MAX_PROMOTE_WARN_LIMIT) {
    status = FAILURE;
  }

  return status;
}

/**

  Removes a warning from the promoteWarningList in host structure if present

  @param host      - Pointer to the system host (root) structure
  @param majorCode - Major error code to log
  @param minorCode - Minor error code to log

  @retval 0 - Successfully cleared from log
  @retval 1 - Warning/Error not in log

**/
UINT32
ClearPromoteWarningException (
                             PSYSHOST host,
                             UINT8    majorCode,
                             UINT8    minorCode
                             )
{
  UINT32  status;
  UINT32  i;

  status = SUCCESS;
  //
  // Search warning list for unused entry
  //
  for (i = 0; i < MAX_PROMOTE_WARN_LIMIT; i++) {
    if (host->var.common.promoteWarnList[i] == (UINT16) ((majorCode << 8) | minorCode)) {
      host->var.common.promoteWarnList[i] = 0;
      if (i == host->var.common.promoteWarnLimit) {
        host->var.common.promoteWarnLimit--;
      }
      break;
    }
  }

  if (i == MAX_PROMOTE_WARN_LIMIT) {
    status = FAILURE;
  }

  return status;
}

/**

  Displays the Fatal Error

  @param host      - Pointer to the system host (root) structure
  @param majorCode - Major error code to log
  @param minorCode - Minor error code to log

  @retval 0 - Successfully added to log
  @retval 1 - Log full

**/
void
PrintFatalError (
                PSYSHOST host,
                UINT8    majorCode,
                UINT8    minorCode
                )
{
} // PrintFatalError


void
FatalErrorHook(
    PSYSHOST host,
    UINT8    majorCode,
    UINT8    minorCode,
    UINT8    *haltOnError
    )
{
  PlatformFatalError (host, majorCode, minorCode, haltOnError);
}

/**

  Used to add warnings to the promote warning exception list. Directs call to Chip layer.

  @param host  - pointer to sysHost

  @retval N/A

**/
UINT32
MemInitWarning(
  PSYSHOST host
  )
{
  return PlatformMemInitWarning(host);
}

void
MemCheckBoundary(
   PSYSHOST host,
   UINT32 boundary,
   UINT32 arrayBoundary
   )
/*++

Routine Description:

  Checks that boundary is less than arrayBoundary to avoid array out of bounds access

Arguments:

  boundary
  arrayBoundary

Returns:

  N/A


--*/
{
  if (boundary > arrayBoundary) {
#ifdef SERIAL_DBG_MSG
    MemDebugPrint ((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "\nERROR: array out of bounds\n"));
#endif
    FatalError (host, ERR_MRC_STRUCT, 0);
  }
} // MemCheckBoundary

void
MemCheckIndex(
   PSYSHOST host,
   UINT32 index,
   UINT32 arraySize
   )
/*++

Routine Description:

  Checks that index is less than arraySize to avoid array out of bounds access

Arguments:

  index
  arraySize

Returns:

  N/A


--*/
{
  if (index >= arraySize) {
#ifdef SERIAL_DBG_MSG
    MemDebugPrint ((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "\nERROR: array out of bounds\n"));
#endif
    FatalError (host, ERR_MRC_STRUCT, 0);
  }
} // MemCheckIndex
 