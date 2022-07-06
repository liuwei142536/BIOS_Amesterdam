/** @file
  This is an implementation of the eMCA.

  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

  Copyright (c) 2009-2012 Intel Corporation.  All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license.  Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

**/



#ifndef _EMCA_LIB_H_
#define _EMCA_LIB_H_

//
// Includes
//
//
// Consumed protocols
//
#include <Uefi.h>
#include "emca.h"
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/IioUds.h>
#include <PiDxe.h>
#include <Protocol\SmmSwDispatch2.h>
#include <IndustryStandard\WheaDefs.h>

//Get L1 Directory pointer and skip the header to get the BASE to get Elog directory entry pointers base. 
#define EMCA_GET_L1_BASE(A) \
    ((UINT8 *)((UINT64)(A) + (UINT64)((EMCA_L1_HEADER *)(A))->HdrLen))

//A - L1 directory pointer
//B - ApicId
//C - McBank Number
// OFFSET byte base = apicid * (Number of L1 Entries per logical processor * 8) + McBankNum
#define EMCA_GET_L1_OFFSET(A,B,C) \
    ( (UINT64)(B) * ( (((EMCA_L1_HEADER *)(A))->NumL1EntryLp) * sizeof(UINT64) ) + ((C) * sizeof(UINT64)) )

//A - Elog Directory Entry Pointers Base
//B - Offset for elog entry
#define EMCA_GET_ELOG_ENTRY(A,B) \
    (GENERIC_ERROR_STATUS *)(*((UINT64 *)((UINT64)(A) + (UINT64)(B))))


BOOLEAN
isEmcaOptInFlagSet(
VOID
);

EFI_STATUS
InitEMCA( 
IN OUT VOID                         **L1Dir,
IN OUT VOID                         **ElogDir
);

EFI_STATUS
LogEmcaRecord(
  IN UINT32                                 McBankNum,
  IN CONST EMCA_MC_SIGNATURE_ERR_RECORD     *McSig,
  IN CONST GENERIC_ERROR_STATUS             *Log
);

#endif  //_EMCA_LIB_H_

