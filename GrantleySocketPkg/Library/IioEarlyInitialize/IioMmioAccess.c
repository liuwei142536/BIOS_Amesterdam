//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c)  1999 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

 
Module Name:

  IioMmioAccess.c

Abstract:

  Macros to simplify and abstract the interface to PCI configuration.

--*/
#pragma warning (disable: 4127)     // disable C4127: constant conditional expression
#include "MmioAccess.h"
#include "IioMisc.h"

/*++

Routine Description:
  Writes 32-bits Memory address memory

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated
  Data               - Data value

Returns:
  None

--*/
VOID
IioMmioWrite32(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT32 Data
 )
{
  IIO_ASSERT_INVALID_ADDRESS (IioGlobalData, Address);
  *(volatile UINT32 *) (UINTN)Address =  Data;
#ifndef IA32
  S3BootScriptSaveMemWrite(S3BootScriptWidthUint32, Address, 1, &Data);
#endif // IA32
}

/*++

Routine Description:
  Writes 16-bits Memory address memory

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated
  Data               - Data value

Returns:
  None

--*/
VOID
IioMmioWrite16(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT16 Data
){

  IIO_ASSERT_INVALID_ADDRESS (IioGlobalData, Address);
  *(volatile UINT16 *) (UINTN) Address =  Data;
#ifndef IA32
  S3BootScriptSaveMemWrite(S3BootScriptWidthUint16, Address, 1, &Data);
#endif // IA32
}

/*++

Routine Description:
  Writes 8-bits Memory address memory

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated
  Data               - Data value

Returns:
  None

--*/
VOID
IioMmioWrite8(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT8 Data
  )
{

  IIO_ASSERT_INVALID_ADDRESS (IioGlobalData, Address);
  *(volatile UINT8 *) (UINTN)Address = Data;
#ifndef IA32
  S3BootScriptSaveMemWrite(S3BootScriptWidthUint8, Address, 1, &Data);
#endif // IA32
}

/*++

Routine Description:
  Reads 32-bits Memory address memory

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated

Returns:
  Data               - Data value

--*/
UINT32
IioMmioRead32(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address
 )
{
  UINT32 Data;

  IIO_ASSERT_INVALID_ADDRESS (IioGlobalData, Address);

  Data = *(volatile UINT32 *)(UINTN) Address;

  return Data;
}

/*++

Routine Description:
  Reads 16-bits Memory address memory

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated

Returns:
  Data               - Data value

--*/
UINT16
IioMmioRead16(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address
 )
{
  UINT16 Data;

  IIO_ASSERT_INVALID_ADDRESS (IioGlobalData, Address);

  Data = *(volatile UINT16 *)(UINTN) Address;

  return Data;
}

/*++

Routine Description:
  Reads 8-bits Memory address memory

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated

Returns:
  Data               - Data value

--*/
UINT8
IioMmioRead8(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address
 )
{
  UINT8 Data;

  IIO_ASSERT_INVALID_ADDRESS (IioGlobalData, Address);

  Data = *(volatile UINT8 *) (UINTN)Address;

  return Data;
}

/*++

Routine Description:
  Write 8-bits Memory address memory and ORs the value with new Data

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated
  DataOr             - Or Data

Returns:
  None

--*/
VOID
IioMmioOr32(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT32  DataOr
  )
{
  IioMmioWrite32(IioGlobalData, Address, (IioMmioRead32( IioGlobalData, Address) | DataOr));

}

/*++

Routine Description:
  Write 16-bits Memory address memory and ORs the value with new Data

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated
  DataOr             - Or Data

Returns:
  None

--*/
VOID
IioMmioOr16(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT16  DataOr
  )
{
  IioMmioWrite16(IioGlobalData, Address, (IioMmioRead16( IioGlobalData, Address) | DataOr));
}

/*++

Routine Description:
  Write 8-bits Memory address memory and ORs the value with new Data

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated
  DataOr             - Or Data

Returns:
  None

--*/
VOID
IioMmioOr8(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT8  DataOr
  )
{
  IioMmioWrite8(IioGlobalData, Address, (IioMmioRead8( IioGlobalData, Address) | DataOr));

}

/*++

Routine Description:
  Write 32-bits Memory address memory and ANDs the value with new Data

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated
  DataAnd            - And Data

Returns:
  None

--*/
VOID
IioMmioAnd32(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT32  DataAnd
  )
{
  IioMmioWrite32(IioGlobalData, Address, (IioMmioRead32( IioGlobalData, Address) & DataAnd));

}


/*++

Routine Description:
  Write 16-bits Memory address memory and ANDs the value with new Data

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated
  DataAnd            - And Data

Returns:
  None

--*/
VOID
IioMmioAnd16(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT16  DataAnd
  )
{
  IioMmioWrite16(IioGlobalData, Address, (IioMmioRead16( IioGlobalData, Address) & DataAnd));

}


/*++

Routine Description:
  Write 8-bits Memory address memory and ANDs the value with new Data

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated
  DataAnd            - And Data

Returns:
  None

--*/
VOID
IioMmioAnd8(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT8  DataAnd
  )
{
  IioMmioWrite8(IioGlobalData, Address,  (IioMmioRead8( IioGlobalData, Address) & DataAnd));
}


/*++

Routine Description:
  Write 32-bits Memory address memory. ANDs then ORs the value with new Data.

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated
  DataAnd            - And Data
  DataOr             - Or Data

Returns:
  None

--*/
VOID
IioMmioAndThenOr32(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT32  DataAnd,
  UINT32  DataOr
  )
{
  IioMmioWrite32(IioGlobalData, Address,((IioMmioRead32( IioGlobalData, Address) & DataAnd) | DataOr));
}

/*++

Routine Description:
  Write 32-bits Memory address memory. ANDs then ORs the value with new Data.

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated
  DataAnd            - And Data
  DataOr             - Or Data

Returns:
  None

--*/
VOID
IioMmioAndThenOr16(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT16  DataAnd,
  UINT16  DataOr
  )
{
  IioMmioWrite16(IioGlobalData, Address, ((IioMmioRead16( IioGlobalData, Address) & DataAnd) | DataOr));
}

/*++

Routine Description:
  Write 32-bits Memory address memory. ANDs then ORs the value with new Data.

Arguments:
  IioGlobalData      - Pointer to IIO_GLOBALS
  Address            - Address that needs to be updated
  DataAnd            - And Data
  DataOr             - Or Data

Returns:
  None

--*/
VOID
IioMmioAndThenOr8(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT8  DataAnd,
  UINT8  DataOr
  )
{
  IioMmioWrite8(IioGlobalData, Address, ((IioMmioRead8( IioGlobalData, Address) & DataAnd) | DataOr));
}
