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

  IioIoAccess.c

Abstract:

  Implement IO access for IioEarLyInitLib

--*/

#include "IoAccess.h"
#include "SysFunc.h"
#ifndef IA32
#include <Library/IoLib.h>
#endif // IA32

/*++

Routine Description:
  Writes 32-bits IO address memory

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated
  Data               - New Data

Returns:
  None

--*/
VOID
IioIoWrite32(
  IIO_GLOBALS   *IioGlobalData,
  UINT16        Address,
  UINT32        Data
  )
{
#ifdef IA32
  OutPort32(IioGlobalData->IioOutData.host, Address, Data);
#else
  IoWrite32(Address, Data);
#endif //IA32
}

/*++

Routine Description:
  Writes 16-bits IO address memory

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated
  Data               - New Data

Returns:
  None

--*/
VOID
IioIoWrite16(
  IIO_GLOBALS *IioGlobalData,
  UINT16      Address,
  UINT16      Data
  )
{
#ifdef IA32
  OutPort16(IioGlobalData->IioOutData.host, Address, Data);
#else
  IoWrite16(Address, Data);
#endif
}

/*++

Routine Description:
  Writes 8-bits IO address memory

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated
  Data               - New Data

Returns:
  None

--*/
VOID
IioIoWrite8(
  IIO_GLOBALS *IioGlobalData,
  UINT16       Address,
  UINT8        Data
  )
{
#ifdef IA32
  OutPort8(IioGlobalData->IioOutData.host, Address, Data);
#else
  IoWrite8(Address, Data);
#endif //IA32
}

/*++

Routine Description:
  Read 32-bits IO address memory

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated


Returns:
  Data               - Data Read

--*/
UINT32
IioIoRead32(
  IIO_GLOBALS   *IioGlobalData,
  UINT16        Address
  )
{
  UINT32 Data;

#ifdef IA32
  Data = InPort32(IioGlobalData->IioOutData.host, Address);
#else
  Data = IoRead32(Address);
#endif //IA32

  return Data;
}

/*++

Routine Description:
  Reads 16-bits IO address memory

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated


Returns:
  Data               - Data Read

--*/
UINT16
IioIoRead16(
  IIO_GLOBALS   *IioGlobalData,
  UINT16         Address
  )
{
  UINT16 Data;

#ifdef IA32
  Data = InPort16(IioGlobalData->IioOutData.host, Address);
#else
  Data = IoRead16(Address);
#endif //IA32

  return Data;
}

/*++

Routine Description:
  Reads 8-bits IO address memory

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated


Returns:
  Data               - Data Read

--*/
UINT8
IioIoRead8(
  IIO_GLOBALS   *IioGlobalData,
  UINT16        Address
  )
{
  UINT8 Data;

#ifdef IA32
  Data = InPort8(IioGlobalData->IioOutData.host, Address);
#else
  Data = IoRead8(Address);
#endif //IA32

  return Data;
}
