/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2005-2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  QpiInit.c

Abstract:

  Reference Code Module.

--*/
#include "ProcMemInit.h"

VOID
QpiInit (
  struct sysHost             *host
  )
/*++

Routine Description:

  Entry point for QPI initialization

Arguments:

  host  - Pointer to the system host (root) structure


Returns:

  VOID

--*/
{
  UINT32  QpiStatus;

  OemPreQpiInit (host);

  //
  // Execute QPI initialization
  //
  QpiStatus = QpiMain (host);

  OemPostQpiInit (host, QpiStatus);

}
