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

  MmioAccess.h

Abstract:

  Macros to simplify and abstract the interface to PCI configuration.

--*/

#ifndef _MMIOACCESS_H_
#define _MMIOACCESS_H_

#include "DataTypes.h"
#include <Library/IioPlatformData.h>
#ifndef IA32
#include <Library/S3BootScriptLib.h>
#endif // IA32


#define IIO_ASSERT_INVALID_ADDRESS(IioGlobalData, A) \
  if (((A) & ~0xfffffff) == 0) { \
    IIO_ASSERT(IioGlobalData, IIO_INVALID_PARAMETER, A); \
  }

VOID
IioMmioWrite32(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT32 Data
);

VOID
IioMmioWrite16(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT16 Data
);

VOID
IioMmioWrite8(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT8 Data
);

UINT32
IioMmioRead32(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address
);

UINT16
IioMmioRead16(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address
);

UINT8
IioMmioRead8(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address
);

VOID
IioMioOr32(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT32  DataOr
);

VOID
IioMmioOr16(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,

  UINT16  DataOr
);

VOID
IioMmioOr8(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT8  DataOr
);

VOID
IioMmioAnd32(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT32  DataAnd
);

VOID
IioMmioAnd16(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT16  DataAnd
);

VOID
IioMmioAnd8(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT8  DataAnd
);

VOID
IioMmioAndThenOr32(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT32  DataAnd,
  UINT32  DataOr
);

VOID
IioMmioAndThenOr16(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT16  DataAnd,
  UINT16  DataOr
);

VOID
IioMmioAndThenOr8(
  IIO_GLOBALS *IioGlobalData,
  UINT32 Address,
  UINT8  DataAnd,
  UINT8  DataOr
);

#endif // _MMIOACCESS_H_
