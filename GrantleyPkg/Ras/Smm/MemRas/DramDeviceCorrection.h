//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c) 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  DramDeviceCorrection.h

Abstract:

  This file defines the macros and function prototypes used for DRAM device correction

--*/

#ifndef _DRAM_DEVICE_CORRECTION_H_
#define _DRAM_DEVICE_CORRECTION_H_

#include <Protocol/MemRasProtocol.h>
#include "MemRas.h"



EFI_STATUS
EFIAPI
InitializeDevTagging(
 );


VOID
EFIAPI	
HandleDevTagForPendingEvent(
	IN  UINT8 NodeId,
	IN  UINT8 Ch,
	IN  UINT8 Rank
);


EFI_STATUS
EFIAPI
CheckAndHandleDevTagging(
    IN UINT8 NodeId,
    IN EVENT_TYPE EventType,
    OUT BOOLEAN *IsEventHandled
 );

#endif //_DRAM_DEVICE_CORRECTION_H_
