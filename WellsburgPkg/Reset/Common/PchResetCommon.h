/** @file
  Header file for PCH RESET Common Library.

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
#ifndef _RESET_COMMON_H_
#define _RESET_COMMON_H_

//
// External include files do NOT need to be explicitly specified in real EDKII
// environment
//
//AptioV server override start: dynamic MMCFG base change support.
#ifndef DYNAMIC_MMCFG_BASE_FLAG
#include <PchAccess.h>
#endif
//AptioV server override end: dynamic MMCFG base change support.
#include "Library/PchPlatformLib.h"
#include <Library/PciLib.h>

///
/// Private data structure definitions for the driver
///
#define PCH_RESET_SIGNATURE SIGNATURE_32 ('I', 'E', 'R', 'S')

typedef struct {
  UINT32              Signature;
  EFI_HANDLE          Handle;
  PCH_RESET_PROTOCOL  PchResetProtocol;
  UINTN               PchRootComplexBar;
} PCH_RESET_INSTANCE;

#define PCH_RESET_INSTANCE_FROM_THIS(a) \
  CR ( \
  a, \
  PCH_RESET_INSTANCE, \
  PchResetProtocol, \
  PCH_RESET_SIGNATURE \
  )

//
// Function prototypes used by the Pch Reset protocol.
//

/**
  Initialize an Pch Reset protocol instance.
  The function will assert in debug if PCH RCBA has not been initialized

  @param[in] PchResetInstance     Pointer to PchResetInstance to initialize

  @retval EFI_SUCCESS             The protocol instance was properly initialized
  @exception EFI_UNSUPPORTED      The PCH is not supported by this module
**/
EFI_STATUS
PchResetProtocolConstructor (
  PCH_RESET_INSTANCE          *PchResetInstance
  );

/**
  Execute Pch Reset from the host controller.

  @param[in] This                 Pointer to the PCH_RESET_PROTOCOL instance.
  @param[in] PchResetType         Pch Reset Types which includes ColdReset, WarmReset, ShutdownReset,
                                  PowerCycleReset, GlobalReset, GlobalResetWithEc

  @retval EFI_SUCCESS             Successfully completed.
  @retval EFI_INVALID_PARAMETER   If ResetType is invalid.
**/
EFI_STATUS
EFIAPI
PchReset (
  IN     PCH_RESET_PROTOCOL       *This,
  IN     PCH_RESET_TYPE           PchResetType
  );
#endif
