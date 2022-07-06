/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

Copyright (c)2009-2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  OemErrorHandler.c

Abstract:

  Hooks for OEM error handler

------------------------------------------------------------------*/
#include "CommonErrorHandlerIncludes.h"


EFI_STATUS
ConfigOEMSetupData(
  ERROR_LOG_SETUP_DATA	*ErrorLogSetupData
  )
/*++
	Routine Description:
		OEM specific Setup data
	Arguments:
		Setup data

	Returns:
		EFI_SUCCESS if succeed

--*/
{

  return EFI_SUCCESS;
}

VOID
OemCoreFatalErrorHandler (
  UINT8 node
  )
{
  
  return;
}

VOID
OemViralErrorHandler (VOID)
{
  return;
}
