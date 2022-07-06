/**
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c) 2005-2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

  @file MemoryInit.c

  Memory Initialization Module.

**/

#include "ProcMemInit.h"
#include "MemFunc.h"


/**

  MRC wrapper code.

    @param host  - Pointer to sysHost

    @retval N/A

**/
VOID
MemoryInit (
  struct sysHost             *host
  )
{

  UINT32  MrcStatus;

  //
  // OemHook Before Memory Init
  //
  MrcStatus = OemPreMemoryInit (host);

  //
  // Invoke MRC
  //
  MrcStatus = MemStart (host);

#ifdef  BDAT_SUPPORT
  //
  // Compatible BIOS Data Structure
  //
  if (host->var.common.resetRequired == 0) {
    FillBDATStructure(host);
  }
#endif  //  BDAT_SUPPORT

  UncoreEnableMeshMode (host);

  OemPostMemoryInit (host, MrcStatus);

}
