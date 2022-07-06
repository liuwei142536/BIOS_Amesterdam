/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

  Copyright (c) 2012 - 2020, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  Mpst.h

Abstract:

--*/

#ifndef _MPST_H
#define _MPST_H

#include <Acpi\Mpst.h>
#include <Library\DebugLib.h>
#include <Library\HobLib.h>
#include <Library\PcdLib.h>
#include <Protocol\SmmBase2.h>
#include <Protocol\SmmSxDispatch2.h>
#include <Protocol\SmmSwDispatch2.h>
#include <Protocol\MpstSupport.h>
#include <Protocol\MpService.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include "IchRegs.h"
#include <Library/SmmMemLib.h>

#endif
