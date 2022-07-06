//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c) 1996 - 2015, Intel Corporation.

This source code and any documentation accompanying it ("Material") is furnished
under license and may only be used or copied in accordance with the terms of that
license.  No license, express or implied, by estoppel or otherwise, to any
intellectual property rights is granted to you by disclosure or delivery of these
Materials.  The Materials are subject to change without notice and should not be
construed as a commitment by Intel Corporation to market, license, sell or support
any product or technology.  Unless otherwise provided for in the license under which
this Material is provided, the Material is provided AS IS, with no warranties of
any kind, express or implied, including without limitation the implied warranties
of fitness, merchantability, or non-infringement.  Except as expressly permitted by
the license for the Material, neither Intel Corporation nor its suppliers assumes
any responsibility for any errors or inaccuracies that may appear herein.  Except
as expressly permitted by the license for the Material, no part of the Material
may be reproduced, stored in a retrieval system, transmitted in any form, or
distributed by any means without the express written consent of Intel Corporation.


Module Name:

SoftAdr.h

Abstract:

This file defines SoftAdr related equates.

--*/

#ifndef _SOFT_ADR_H_
#define _SOFT_ADR_H_

#include <AmiDxeLib.h> // AptioV Server Override

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/PciLib.h>
#include <Library\PciExpressLib.h>

#include <Protocol/SmmBase2.h>
#include <Protocol/SmmStatusCode.h>
#include <Protocol/SmmGpiDispatch2.h>
#include <Protocol/SmmVariable.h>
#include <Protocol/IioUds.h>
#include <Protocol/MpService.h>
#include <Protocol/SmmVariable.h>

#include <Protocol/CpuCsrAccess.h>
#include <Protocol/SmmCpuService.h>





#include <Guid/SetupVariable.h>
#include <Guid/MemoryConfigData.h>

#include "PchRegs.h"
#include "SysHost.h"


#include "Syshost.h"
#include "Platform.h"

#define ICH_GPI_SMI_SOFTADR           12 // GPIO 12 is used as the trigger for SoftADR..

#define B_GBL_SMI_EN 0
#endif
