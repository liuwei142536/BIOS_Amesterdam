/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
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

  IioEarlyInit.h

Abstract:

  Houses all function definition to Initialize IIO before Port Initialization

Revision History:

--*/
#ifndef _IIO_EARLYINIT_H_
#define _IIO_EARLYINIT_H_

#include <Library/IioPlatformData.h>

VOID
InitializeIioGlobalDataPerSocket (
  struct sysHost       *host,
  UINT8                 Iio
);

VOID
IioEarlyLinkInit(
  struct sysHost             *host
);

VOID
IioEarlyPostLinkInit(
  struct sysHost             *host
);

VOID
IioEarlyPreDataLinkInit(
  struct sysHost             *host
);

VOID
InitializeIioDefaultData(
  struct sysHost             *host
);
#endif
