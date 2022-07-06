//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c)  1999 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  IioEarlyInitializeLib.h

Abstract:

  This file provides required function headers publish for IioEarlyInitializeLib

--*/

#ifndef _IIO_EARLYINITIALIZE_LIB_H_
#define _IIO_EARLYINITIALIZE_LIB_H_

#ifndef IA32
EFI_STATUS
IioEarlyInitializeEntry (
  IIO_GLOBALS  *IioGlobalData
);
#endif // IA32


VOID
InitializeIioGlobalData (
  IIO_GLOBALS  *IioGlobalData
  );

VOID
IioOtherVariableInit (
  IIO_GLOBALS  *IioGlobalData,
  UINT8                 Iio
  );

VOID
IioEarlyPreLinkTrainingPhase(
  IIO_GLOBALS                         *IioGlobalData,
  UINT8                               Iio
);

VOID
IioEarlyPcieLinkTrainingPhase(
  IIO_GLOBALS                         *IioGlobalData,
  UINT8                               Iio
);

VOID
IioEarlyPostLinkTrainingPhase(
  IIO_GLOBALS                         *IioGlobalData,
  UINT8                               Iio
);

VOID
DumpIioPciePortPcieLinkStatus(
  IIO_GLOBALS              *IioGlobalData
  );


IIO_STATUS
IioDmiTcVcSetup (
  IIO_GLOBALS                 *IioGlobalData
);

VOID 
IioIsocConfiguration(
  IIO_GLOBALS                         *IioGlobalData,
  UINT8                               Iio
);



#endif // _IIO_EARLYINITIALIZE_LIB_H_
