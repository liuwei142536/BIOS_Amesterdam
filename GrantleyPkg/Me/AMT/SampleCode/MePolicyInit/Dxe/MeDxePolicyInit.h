/**
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
**/
/**

Copyright (c)  1999 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:

  MeDxePolicyInit.h

@brief:

  Header file for the MeDxePolicyInit Driver.

**/
#ifndef _ME_DXE_PLATFORM_POLICY_H_
#define _ME_DXE_PLATFORM_POLICY_H_


//#include EFI_PROTOCOL_PRODUCER (MePlatformPolicy)
#include "PchAccess.h"
//#include "MeLib.h"
#include "HeciRegs.h"
#include <MePlatformPolicy.h>
#include <MePlatformPolicyUpdateDxeLib.h>
#include <MkhiMsgs.h>
#include "Library/HeciMsgLib.h"
#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/CpuLib.h>
#include <Library/DebugLib.h>
#include <Library/PciLib.h>
#include <Library/IoLib.h>


#include <Library/UefiLib.h>
#include <Library/PciLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>


#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Library/DebugLib.h>
#include <Library/PrintLib.h>

#include <Protocol/Smmbase.h>
#include <IndustryStandard/Pci22.h>

#include <PchAccess.h>
//#include "HeciMsgLib.h"

EFI_STATUS
EFIAPI
MeDxePolicyInitEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
/**

  @brief
  Initilize Intel ME DXE Platform Policy

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval EFI_SUCCESS             Initialization complete.
  @exception EFI_UNSUPPORTED      The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.

**/
;

VOID
ShowMeReportError (
  IN ME_ERROR_MSG_ID            MsgId
  )
/**

  @brief
  Show Me Error message.

  @param[in] MsgId                Me error message ID.


**/
;

#endif
