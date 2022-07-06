/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2014   Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  IioMisc.c

Abstract:

  Implement IIO Misc functions.

--*/
#include "IioMisc.h"
#include "SysFunc.h"


/*++

Routine Description:
  Generates a warm reset

Arguments:
  IioGlobalData - Global data structure

Returns:
  None

--*/
VOID
IioWarmReset (
  IIO_GLOBALS *IioGlobalData
  )
{
  IioIoWrite8 (IioGlobalData, 0xcf9, 6);
  IioCpuDeadLoop();
}

/*++

Routine Description:
  Enters in a Infinite loop to stop BIOS flow

Arguments:
  None

Returns:
  None

--*/
VOID
IioCpuDeadLoop(
 )
{
//  volatile UINT32  Index;
//
//  for (Index = 0; Index != 0xFF; Index++ ) {
//    if (Index >= 0xFE) {
//      Index = 0;
//    }
//  }
}


/*++

Routine Description:
  Copy memory from source to destination parameters

Arguments:
  *dest              - Destination Address
  *src               - Source Address

Returns:
  None

--*/
VOID
IioCopyMem(
  VOID *dest,
  VOID *src,
  UINT32 size
  )
{

#ifdef IA32
  MemCopy((UINT8 *)dest, (UINT8 *)src, size);
#else
  CopyMem((VOID*)dest, (VOID *)src, size);
#endif

}

/*++

Routine Description:
  Routine to assert
  
Arguments:
  None
  
Returns:
  Does not return

--*/
VOID
IioAssert (
    IIO_GLOBALS     *IioGlobalData,
    UINT32          Data
  )

{
  IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR, "\n Reading/Writing: %x ", Data);
  IioCpuDeadLoop(); 
}

#ifdef IA32
VOID
IioDebugPrint (
  IIO_GLOBALS *IioGlobalData,
  UINT32 DbgLevel,
  char* Format,
  ...
  )
/*++

Routine Description:

  Specific print to serial output

Arguments:

  host      - Pointer to the system host (root) structure
  DbgLevel  - specific debug level
  Format    - string format

Returns:

  VOID

--*/
{

  va_list  Marker;
  va_start (Marker, Format);
#ifdef SERIAL_DBG_MSG
  rcVprintf (IioGlobalData->IioOutData.host, Format, Marker);
#endif // SERIAL_DBG_MSG
//Grangeville AptioV server override - CPP Check
  va_end( Marker );
}
#endif // IA32
