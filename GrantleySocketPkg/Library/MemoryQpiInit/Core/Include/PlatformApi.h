//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license
// agreement with Intel or your vendor.  This file may be
// modified by the user, subject to additional terms of the
// license agreement
//
/*************************************************************************
 *
 * Memory Reference Code
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
 ************************************************************************
 *
 *  PURPOSE:
 *
 *      This file contains declarations for the MRC Core to Platform API
 *
 ************************************************************************/
#ifndef  _PLATFORM_API_H_
#define  _PLATFORM_API_H_

#include "DataTypes.h"
#include "SysHost.h"

void
PlatformOutputWarning(
    PSYSHOST host,                // pointer to sysHost
    UINT8    warningCode,         // Major warning code
    UINT8    minorWarningCode,    // Minor warning code
    UINT8    socket,              // socket number
    UINT8    ch,                  // channel number
    UINT8    dimm,                // dimm number
    UINT8    rank                 // rank number
    );

void
PlatformOutputError(
    PSYSHOST host,                // pointer to sysHost
    UINT8    ErrorCode,           // Major error code
    UINT8    minorErrorCode,      // Minor error code
    UINT8    socket,              // socket number
    UINT8    ch,                  // channel number
    UINT8    dimm,                // dimm number
    UINT8    rank                 // rank number
    );

void
OemPlatformFatalError(
    PSYSHOST host,          // Pointer to the system host (root) structure
    UINT8    majorCode,     // Major error code
    UINT8    minorCode,     // Minor error code
    UINT8    *haltOnError   // Halt on Error input/output value
    );

UINT32
OemPreMemoryInit(
    PSYSHOST host           // Pointer to the system host (root) structure
    );

VOID
OemPostMemoryInit(
    struct sysHost   *host,        // Pointer to the system host (root) structure
    UINT32           MrcStatus     // Return status from MRC code execution
    );

VOID
OemPreProcInit(
    struct sysHost   *host    // Pointer to the system host (root) structure
    );

VOID
OemPostProcInit(
    struct sysHost   *host,        // Pointer to the system host (root) structure
    UINT32           ProcStatus    // Return status
    );

/* // Aptio V Server Override - Start :Fix for the OemGetPlatformVariableTagPtr Corruption 109314
VOID
OemGetPlatformVariableTagPtr(
  struct sysHost             *host,
  VOID                       **VariableTagPtr
  );
// Aptio V Server Override - End : Fix for the OemGetPlatformVariableTagPtr Corruption 109314 */

VOID
OemPostCpuInit(
    struct sysHost *host,    // Pointer to the system host (root) structure
    UINT8          socket    // memory controller number ( 0 based)
    );

#ifdef SERIAL_DBG_MSG
VOID
OemInitSerialDebug(
    PSYSHOST host           // Pointer to the system host (root) structure
    );
#endif

void
PlatformCheckpoint(
    PSYSHOST host           // Pointer to the system host (root) structure
    );

void
PlatformLogWarning(
    PSYSHOST host,        // Pointer to the system host (root) structure
    UINT8    majorCode,   // major error/warning code
    UINT8    minorCode,   // minor error/warning code
    UINT32   logData      // data log
    );

void
PlatformFatalError(
    PSYSHOST host,            // pointer to sysHost structure
    UINT8    majorCode,       // major error/warning code
    UINT8    minorCode,       // minor error/warning code
    UINT8    *haltOnError     // pointer to variable that selects policy
    );

UINT8
PlatformReadSmb(
    PSYSHOST         host,            // Pointer to the system host (root) structure
    UINT8            socket,          // socket number
    struct smbDevice dev,             // SMB device structure
    UINT16            byteOffset,      // offset of data to read
    UINT8            *data            // Pointer to data to be returned
    );

UINT8
PlatformWriteSmb(
    PSYSHOST         host,        // Pointer to the system host (root) structure
    struct smbDevice dev,         // SMB device structure
    UINT8            byteOffset,  // offset of data to write
    UINT8            *data        // Pointer to data to write
    );

void
PlatformInitGpio(
    PSYSHOST host           // Pointer to the system host (root) structure
    );

UINT32
PlatformReadGpio(
    PSYSHOST host,        // Pointer to the system host (root) structure
    UINT16   address,     // GPIO device address
    UINT32   *data        // Pointer to data
    );

UINT32
PlatformWriteGpio(
    PSYSHOST host,        // Pointer to the system host (root) structure
    UINT16   address,     // GPIO device address
    UINT32   data         // Data to write
    );

UINT32
PlatformMemInitWarning(
    PSYSHOST host           // Pointer to the system host (root) structure
    );

void
PlatformMemLogWarning(
    PSYSHOST host,        // Pointer to the system host (root) structure
    UINT8    majorCode,   // Major warning code
    UINT8    minorCode,   // Minor warning code
    UINT32   logData      // Data log
    );

UINT32
PlatformMemReadDimmVref(
    PSYSHOST host,    // Pointer to the system host (root) structure
    UINT8    socket,  // processor number
    UINT8    *vref    // pointer to UINT8 where the vref value is stored
    );

UINT32
PlatformMemWriteDimmVref(
    PSYSHOST host,    // Pointer to the system host (root) structure
    UINT8    socket,  // processor number
    UINT8    vref     // pointer to UINT8 where the vref value is stored
    );

UINT8
PlatformSetVdd(
    PSYSHOST host,    // Pointer to the system host (root) structure
    UINT8    socket   // processor number
    );

void
PlatformReleaseADRClamps(
    PSYSHOST host,    // Pointer to the system host (root) structure
    UINT8    socket   // processor number
    );

UINT32
PlatformDetectADR(
    PSYSHOST host           // Pointer to the system host (root) structure
    );

void
PlatformCheckPORCompat(
    PSYSHOST host           // Pointer to the system host (root) structure
    );

void
PlatformHookMst(
    PSYSHOST  host,               // pointer to sysHost
    UINT8     socket,             // socket number
    struct    smbDevice dev,      // SMB Device
    UINT8     byteOffset,         // byte Offset
    UINT8     *data               // data
    );

void
OemLookupDdr4OdtTable(
    PSYSHOST host,                                        // pointer to sysHost
    struct ddr4OdtValueStruct **ddr4OdtValueStructPtr,    // Pointer to Intel DDR4 ODT Value array
    UINT16 *ddr4OdtValueStructSize                        // Pointer to size of Intel DDR4 ODT Value array in number of bytes
    );

void
OemLookupFreqTable(
    PSYSHOST host,                                // Pointer to the system host (root) structure
    struct DimmPOREntryStruct **freqTablePtr,     // Pointer to Intel POR memory frequency table
    UINT16 *freqTableLength                       // Pointer to number of DimmPOREntryStruct entries in POR Frequency Table
    );

void
GetSetupOptionsEx(
    PSYSHOST host           // Pointer to the system host (root) structure
    );

#endif // _PLATFORM_API_H_
