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

    OemIioInitCommon.h

Abstract:

    Oem Hooks file for IioInit Module

--*/


#ifndef _OEMIIOINITCOMMON_H_
#define _OEMIIOINITCOMMON_H_

#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/PciExpressLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmbusLib.h>
#include <Library/PlatformHooksLib.h>
#include <Guid/PlatformInfo.h>
#include <Guid/SetupVariable.h>
#ifdef IA32
#include <Ppi/Smbus2.h>
#else
#include <Protocol/SmbusHc.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Guid/PlatformInfo.h>
#include "Platform.h"
#endif // IA32




#define NTB_XLINK_CTL_AUTO   		1
#define NTB_XLINK_CTL_DSD_USP		2
#define NTB_XLINK_CTL_USD_DSP		3
#define IDT89HT0816AP_VID           0x111D

#define IDT89HT0816AP_SMBUS_ADDR	0xE0
#define IDT89HT0816AP_OFFSET_VID	0x00
#define IDT89HT0816AP_OFFSET_DID	0x02
#define IDT89HT0816AP_OFFSET_RID	0x04
#define IDT89HT0816AP_OFFSET_DCR	0x08

typedef enum {
	Upstream=1,
	Downstream,
	Unknown=0xff
}IDT_RETIMER_DIR;

EFI_STATUS
SetIDTRetimerDirection(
  IDT_RETIMER_DIR
  );




EFI_STATUS
GetSystemVariable(
  SYSTEM_CONFIGURATION  *SetupData
);

EFI_STATUS
SetSystemVariable(
  SYSTEM_CONFIGURATION  *SetupData,
  UINT32                VarSize
);

EFI_STATUS
LocateSmBus (
  VOID                     **SmBus
);

#ifndef IA32
#endif // IA32
#endif // _OEMIIOINITCOMMON_H_

