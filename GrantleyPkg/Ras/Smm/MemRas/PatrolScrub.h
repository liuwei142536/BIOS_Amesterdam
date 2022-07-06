//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c) 2011 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PatrolScrub.h

Abstract:

  This file defines the macros and function prototypes used by Patrol Scrub code

--*/

#ifndef _PATROL_SCRUB_H_
#define _PATROL_SCRUB_H_

#include <Protocol/MemRasProtocol.h>
#include "MemRas.h"


#define NON_SPARE_CE    1
#define NON_SPARE_CE_EVENT 2
#define SPARE_UC   3
#define SPARE_CE   4

#define FULL_SIZE_RANK_CHECK 0x100
#define HALF_SIZE_RANK_CHECK 0x010
#define QUARTER_SIZE_RANK_CHECK 0x001

//
// Protocol definitions
//
typedef enum {
  SCRUBBING_NON_SPARERANK,
  SCRUBBING_SPARERANK
} SSRSTATETYPE;


/*++

Routine Description:
  Check if patrol scrub is enabled or not.

Arguments:
  none

Returns:

  TRUE -- patrol scrub is enabled.
  FALSE -- patrol scrub is disabled.

--*/
BOOLEAN
EFIAPI
CheckPatrolScrubEnable( 
);


//
// Patrol Scrub Functions
//
EFI_STATUS
EFIAPI
ReEnablePatrolScrubEngine (
  IN UINT8 NodeId
  );

/**

Routine Description:
  Get Next available non-Spare rank to scrub

Arguments:
    NodeId
    DdrCh - ptr that holds current ddr ch
    Logical Rank - ptr that holds current rank

  Note:
  If *DDRCH >= MAX_CH and *Logical rank >= MAX_RANK_CH, then this routine will
  return the first available ch/rank

Returns:
   None

--*/
static
VOID
GetNextAvailableNonSprRankForPatrol(
    IN  UINT8   NodeId,
    OUT UINT8  *DdrCh,
    OUT UINT8  *LogicalRank
 );


/*++

Routine Description:
  re-enable Spare Scrub engine when there is nothing to scrub spare rank or do spare copying.

Arguments:
  NodeId - Memory controller ID

Returns:

  EFI_SUCCESS - Always

--*/
EFI_STATUS
ReEnableSSR(
    IN UINT8   NodeId,
    IN BOOLEAN CheckSSRStateFlag
);


EFI_STATUS
EFIAPI
DisablePatrolScrubEngine (
  IN UINT8    NodeId
  );


VOID
EFIAPI
ScrubNextRank(
    IN UINT8 NodeId,	
    IN UINT8 Ch,
    IN UINT8 Rank,
    IN UINT8 ScrubSource
);


EFI_STATUS
InstallPatrolSmiHandler(
    VOID
 );

static
EFI_STATUS
HandleAsymmetricDIMMPatrolEvent(
    IN  UINT8 nodeId
);

VOID
Execute_AsymmetricDIMM_WA(
  IN UINT8 nodeId,
  IN UINT8 ch,
  IN UINT8 dimm
);

EFI_STATUS
InstallAsymmetricDIMMPatrolScrubHandler(
    VOID
 );


#endif // _PATROL_SCRUB_H_
