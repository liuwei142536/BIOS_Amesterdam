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

  PciAccess.h

Abstract:

  Macros to simplify and abstract the interface to PCI configuration.

--*/

#ifndef _PCIACCESS_H_
#define _PCIACCESS_H_

#include "DataTypes.h"
#include <Library/IioPlatformData.h>

VOID
IioPciExpressWrite32( 
  IIO_GLOBALS   *IioGlobalData,
  UINT64        Address,
  UINT32        Data
  );
  
VOID
IioPciExpressWrite16( 
  IIO_GLOBALS       *IioGlobalData,
  UINT64        Address,
  UINT16        Data
  );

VOID
IioPciExpressWrite8( 
  IIO_GLOBALS       *IioGlobalData,
  UINT64        Address,
  UINT8         Data
  );

UINT32
IioPciExpressRead32( 
  IIO_GLOBALS       *IioGlobalData,
  UINT64        Address
  );
  
UINT16
IioPciExpressRead16( 
  IIO_GLOBALS       *IioGlobalData,
  UINT64        Address
  );
  
UINT8
IioPciExpressRead8( 
  IIO_GLOBALS       *IioGlobalData,
  UINT64        Address
  );

UINTN
IioGetCpuCsrAddress (
  IIO_GLOBALS    *IioGlobalData,
  UINT8          SocId,
  UINT8          BoxInst,
  UINT32         Offset,
  UINT8          *Size
  );


VOID
IioWriteCpuCsr32 (
  IIO_GLOBALS       *IioGlobalData,
  UINT8             SocId,
  UINT8             BoxInst,
  UINT32            RegOffset,
  UINT32            Data
  );
  
VOID
IioWriteCpuCsr16( 
  IIO_GLOBALS       *IioGlobalData,
  UINT8             SocId,
  UINT8             BoxInst,
  UINT32            RegOffset,
  UINT16            Data
  );

VOID
IioWriteCpuCsr8( 
  IIO_GLOBALS       *IioGlobalData,
  UINT8             IioIndex,
  UINT8             BoxInst,
  UINT32            RegOffset,
  UINT8             Data
  );
  
UINT32
IioReadCpuCsr32( 
  IIO_GLOBALS       *IioGlobalData,
  UINT8             SocId,
  UINT8             BoxInst,
  UINT32            Offset
  );

UINT16
IioReadCpuCsr16( 
  IIO_GLOBALS       *IioGlobalData,
  UINT8             IioIndex,
  UINT8             BoxInst,
  UINT32            Offset
  );
  
UINT8
IioReadCpuCsr8( 
  IIO_GLOBALS       *IioGlobalData,
  UINT8             IioIndex,
  UINT8             BoxInst,
  UINT32            Offset
  );

UINT32
IioBios2PcodeMailBoxWrite(
  IIO_GLOBALS *IioGlobalData,
  UINT8 Iio, 
  UINT32 MailBoxCommand, 
  UINT32 MailboxData
);
#endif // _PCIACCESS_H_
