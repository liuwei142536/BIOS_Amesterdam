//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c) 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  SparingEngine.h

Abstract:

  This file defines the macros and function prototypes used by SparingEngine code

--*/

#ifndef _SPARING_ENGINE_H_
#define _SPARING_ENGINE_H_

#include <Protocol/MemRasProtocol.h>
#include "MemRas.h"

#define MAILBOX_BIOS_CMD_RAS_START                  0x9A  // Cmd to RAS Start HSX_TODO
#define MAILBOX_BIOS_CMD_EXIT_RAS                   0x9B  // Cmd to Exit RAS HSX_TODO

typedef enum {
  RankSparing       // Rank Sparing
} SPARING_MODE;


EFI_STATUS
AcquireSparingEngine(
    IN UINT8 NodeId
 );


/*++

Routine Description:
  Checks the internal status and releases ownership of the sparing engine.
  It is expected that only caller who has successfully acquired ownership
  using AcquireSparingEngine calls this function

Arguments:
  NodeId - Memory controller ID

Returns:

  EFI_SUCCESS if sparing engine is released
  EFI_UNSUPPORTED any other error

--*/
EFI_STATUS
ReleaseSparingEngine(
    IN UINT8 NodeId
 );


EFI_STATUS
StartSparing(
    IN UINT8        NodeId,
    IN UINT8        SrcRank,
    IN UINT8        DstRank,
    IN UINT8        Ch,
    IN SPARING_MODE SparingMode
 );

EFI_STATUS
IsCopyComplete(
    IN UINT8        NodeId,
    IN SPARING_MODE SparingMode
 );

void 
DisablePkgC6(
    IN UINT8 NodeId
);
	
void 
EnablePkgC6(
    IN UINT8 NodeId
);
#endif // _SPARING_ENGINE_H_
