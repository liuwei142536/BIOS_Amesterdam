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

  IioVData.h

Abstract:

  This file defines everything that's needed for IIO VData struct.

--*/

#ifndef _IIOVDATA_H_
#define _IIOVDATA_H_

#include "DataTypes.h"
#include <Library/IioPlatformData.h>

VOID
IioVData_DisablePciePortConfigure(
    IIO_GLOBALS                     *IioGlobalData,
    UINT8                           Iio,
    UINT8                           PortIndex
    );

BOOLEAN
IioVData_ConfigurePciePort(
    IIO_GLOBALS                     *IioGlobalData,
    UINT8                           Iio,
    UINT8                           PortIndex
    );

VOID
IioVDataPostLinkInit(
  IIO_GLOBALS                       *IioGlobalData,
  UINT8                             Iio
  );

VOID
UpdateVDataAfterApplyWorkAround(
  IIO_GLOBALS                       *IioGlobalData,
  UINT8                             Iio
  );

BOOLEAN
IsThisUplinkPort(
  IIO_GLOBALS                       *IioGlobalData,
  UINT8                             Iio,
  UINT8                             PortIndex
);

BOOLEAN
CheckVarPortEnable(
  IIO_GLOBALS                       *IioGlobalData,
  UINT8                             Iio,
  UINT8                             PortIndex
);

UINT8
CheckVarPortLinkSpeed(
  IIO_GLOBALS                       *IioGlobalData,
  UINT8                             Iio,
  UINT8                             PortIndex
);


BOOLEAN
ChangeVarPortLinkSpeed(
  IIO_GLOBALS                       *IioGlobalData,
  UINT8                             Iio,
  UINT8                             PortIndex
);

UINT8
CheckVarPortASPM(
  IIO_GLOBALS                       *IioGlobalData,
  UINT8                             Iio,
  UINT8                             PortIndex
);

UINT8
GetAssignedPortAddressWidth(
  IIO_GLOBALS                                    *IioGlobalData,
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
);





#endif // _IIOVDATA_H_

