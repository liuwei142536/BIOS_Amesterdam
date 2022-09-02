/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
--*/
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

#include "SysFunc.h"
#include "MemdownSpdTable.h"
#include <Library/OemMemoryQpiInit.h>
// AptioV server override start: Report MRC progress/error codes to status code listener
#include <AmiStatusCodes.h>
#include <Library/ReportStatusCodeLib.h>
// AptioV server override end: Report MRC progress/error codes to status code listener

//
// Disable warning for unsued input parameters
//
#pragma warning(disable : 4100)


void
PlatformCheckpoint (
  PSYSHOST host
  )
/*++

  PlatformCheckpoint - OEM hook to provide major checkpoint output
  to the user interface. By default a byte is written to port 80h.

  @param host  - Pointer to the system host (root) structure

  @retval N/A

--*/
{
  //
  // Output major checkpoint
  //
  OutPort8 (host, host->setup.common.debugPort, (UINT8) (host->var.common.checkpoint >> 24));
}


void
PlatformLogWarning (
  PSYSHOST host,
  UINT8    majorCode,
  UINT8    minorCode,
  UINT32   logData
  )
/*++

  PlatformLogWarning - OEM hook to provide common warning output to the
  user interface

  @param host      - pointer to sysHost structure
  @param majorCode - major error/warning code
  @param minorCode - minor error/warning code
  @param logData   - data log

  @retval N/A

--*/
{
  return ;
}

void
PlatformFatalError (
  PSYSHOST host,
  UINT8    majorCode,
  UINT8    minorCode,
  UINT8    *haltOnError
  )
/*++

  PlatformFatalError - OEM hook to provide fatal error output to the
  user interface and to override the halt on error policy.

  @param host - pointer to sysHost structure
  @param majorCode - major error/warning code
  @param minorCode - minor error/warning code
  @param haltOnError - pointer to variable that selects policy

  @retval N/A

--*/
{
// AptioV server override start: Report MRC progress/error codes to status code listener
  //
  // Report Memory not detected Error code.
  //
  if ( majorCode == ERR_NO_MEMORY ) {
	REPORT_STATUS_CODE (
	  EFI_ERROR_CODE|EFI_ERROR_MAJOR,
	  PEI_MEMORY_NOT_DETECTED
	  );
  }
  //
  // Report Memory not useful Error code.
  //
  if ( majorCode == ERR_MEM_TEST ) {
	REPORT_STATUS_CODE (
	  EFI_ERROR_CODE|EFI_ERROR_MAJOR,
	  PEI_MEMORY_NONE_USEFUL
	  );
  }
// AptioV server override end: Report MRC progress/error codes to status code listener

  OemPlatformFatalError(host, majorCode, minorCode, haltOnError);
  return ;
}

UINT8
PlatformReadSmb (
  PSYSHOST         host,
  UINT8            socket,
  struct smbDevice dev,
  UINT16            byteOffset,
  UINT8            *data
  )
/*++

  OEM hook to read data from a device on the SMBbus.
  Return 1 or 2 if this routine should replace ReadSmb

  @param host - pointer to sysHost structure
  @param socket - socket number
  @param dev - SMB device structure
  @param byteOffset - offset of data to read
  @param data - Pointer to data to be returned

  @retval 0 - Hook not implemented (default)
  @retval 1 - Hook implemented and read was successful
  @retval 2 - Hook implemented and read failed / slave device not present

--*/
{
      // Same data for all channels.
      *data = MT9ASF51272_SPD[byteOffset];
      return 1;
}

UINT8
PlatformWriteSmb (
  PSYSHOST         host,
  struct smbDevice dev,
  UINT8            byteOffset,
  UINT8            *data
  )
/*++

  OEM hook to write data to a device on the SMBbus.

  @param host - pointer to sysHost structure
  @param dev - SMB device structure
  @param byteOffset - offset of data to write
  @param data - Pointer to data to write

  @retval N/A

--*/
{
  //
  // Hook not implemented
  //
  return 0;
}

void
PlatformInitGpio (
  PSYSHOST host
  )
/*++

  OEM hook for one-time initialization of GPIO parameters such
  as decode, direction, and polarity init.

  @param host - pointer to sysHost structure

  @retval N/A

--*/
{
  return ;
}

UINT32
PlatformReadGpio (
  PSYSHOST host,
  UINT16   address,
  UINT32   *data
  )
/*++

  OEM hook to read a byte from a platform GPIO device that is
  not recognized by ReadGpio().  Return 1 or 2 if this
  routine should replace ReadGpio

  @param host      - pointer to sysHost structure
  @param address   - GPIO device address
  @param data      - Pointer to data

  @retval 0         - Hook not implemented (default)
  @retval 1         - Hook implemented and read was successful
  @retval 2         - Hook implemented and read failed / slave device not present

--*/
{
  //
  // Hook not implemented
  //
  return 0;
}

UINT32
PlatformWriteGpio (
  PSYSHOST host,
  UINT16   address,
  UINT32   data
  )
/*++

  OEM hook to write a byte to a platform GPIO device that is
  not recognized by WriteGpio().
  Return 1 or 2 if this routine should replace WriteGpio

  @param host      - pointer to sysHost structure
  @param address   - GPIO device address
  @param data      - Data to write

  @retval 0         - Hook not implemented (default)
  @retval 1         - Hook implemented and write was successful
  @retval 2         - Hook implemented and write failed / slave device not present

--*/
{
  //
  // Hook not implemented
  //
  return 0;
}
