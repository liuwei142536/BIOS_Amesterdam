/** @file
  Header file for PCH RESET PEIM Driver.

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
#ifndef _PCH_RESET_H
#define _PCH_RESET_H

#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/PciLib.h>
#include <Library/ioLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/MemoryDiscovered.h>
#include <Ppi/PchReset.h>
#include "PchResetCommon.h"
#include "PchAccess.h"

typedef struct {
  EFI_PEI_PPI_DESCRIPTOR  PpiDescriptor;
  PCH_RESET_INSTANCE      PchResetInstance;
} PEI_PCH_RESET_INSTANCE;


/**
  Execute call back function for Pch Reset.

  @param[in] PchResetType         Pch Reset Types which includes PowerCycle, Globalreset.

  @retval EFI_SUCCESS             The callback function has been done successfully
  @exception EFI_UNSUPPORTED      Do not do any reset from PCH
**/
EFI_STATUS
EFIAPI
PchResetCallback (
  IN     PCH_RESET_TYPE           PchResetType
  );
#endif
