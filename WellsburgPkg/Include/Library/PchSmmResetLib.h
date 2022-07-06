/** @file
  Header file for PCH RESET Runtime Driver

@copyright
  Copyright (c) 2011 - 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
#ifndef _PCH_RESET_SMM_H
#define _PCH_RESET_SMM_H

///
/// External include files do NOT need to be explicitly specified in real EDKII
/// environment
///


#include "Protocol/PchReset.h"
#include "Library/DxeRuntimePciLibPciExpress.h"
#include "Library/PchPlatformLib.h"
#include <Library/PciLib.h>
#include <Library/BaseLib.h>

#define EFI_INTERNAL_POINTER 0x00000004

/**
  Execute call back function for Pch Reset.

  @param[in] PchResetType         Pch Reset Types which includes PowerCycle, Globalreset.

  @retval EFI_SUCCESS             The callback function has been done successfully
  @retval EFI_NOT_FOUND           Failed to find Pch Reset Callback protocol. Or, none of
                                  callback protocol is installed.
  @retval Others                  Do not do any reset from PCH
**/
EFI_STATUS
EFIAPI
PchResetSmm (
  IN     PCH_RESET_TYPE           PchResetType
  );

#endif
