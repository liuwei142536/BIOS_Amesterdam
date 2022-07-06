//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c) 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  HardErrorDetection.h

Abstract:

  This file defines the macros and function prototypes used for Hard Error Detection

--*/
	
#ifndef GRANGEVILLE_FLAG


#ifndef _HARD_ERROR_DETECTION_H_
#define _HARD_ERROR_DETECTION_H_

#include <Protocol/MemRasProtocol.h>

#include "MemRas.h"
#include <Library/emcaplatformhookslib.h>
#include <Guid\SetupVariable.h>
#include <Library/SynchronizationLib.h>
#include <Protocol\SmmBase2.h>

typedef struct {
  UINT32  Year : 4;
  UINT32  Month : 4;
  UINT32  Day : 5;
  UINT32  Hour : 5;
  UINT32  Minute : 6;
  UINT32  Second : 6;
} TIME_STAMP;

#define BCD2DEC(x)  ((x & 0x0F) + ((x >> 4) * 10))
#define PCAT_RTC_ADDRESS_REGISTER 0x70
#define PCAT_RTC_DATA_REGISTER    0x71
//
// Dallas DS12C887 Real Time Clock
//
#define RTC_ADDRESS_SECONDS           0   // R/W  Range 0..59
#define RTC_ADDRESS_MINUTES           2   // R/W  Range 0..59
#define RTC_ADDRESS_HOURS             4   // R/W  Range 1..12 or 0..23 Bit 7 is AM/PM
#define RTC_ADDRESS_DAY_OF_THE_MONTH  7   // R/W  Range 1..31
#define RTC_ADDRESS_MONTH             8   // R/W  Range 1..12
#define RTC_ADDRESS_YEAR              9   // R/W  Range 0..99
#define RTC_ADDRESS_REGISTER_A        10  // R/W[0..6]  R0[7]
#define RTC_ADDRESS_REGISTER_B        11  // R/W
#define RTC_ADDRESS_REGISTER_C        12  // RO


typedef
EFI_STATUS (*MEM_RAS_EVENT_HNDLR)(
    IN UINT8        NodeId,
    IN EVENT_TYPE   EventType,
    IN OUT BOOLEAN  *IsEventHandled
 );

#define END_OF_TABLE  ((MEM_RAS_EVENT_HNDLR)((UINTN)(-1)))

typedef struct {
  UINT32 Ctl2Msr;
} HED_EMCA_MCBANK_DESC; 

EFI_STATUS
EFIAPI
InitializeHardErrorDetection(
 );

static
EFI_STATUS
HandleRankSparing(
  IN  UINT8         NodeId,
  IN  EVENT_TYPE    EventType,
  IN OUT BOOLEAN    *IsEventHandled
  );

static
EFI_STATUS
HandleDevTagging(
  IN  UINT8         NodeId,
  IN  EVENT_TYPE    EventType,
  IN OUT BOOLEAN    *IsEventHandled
  );

static
EFI_STATUS
HEDClearUnhandledCorrError(
  IN  UINT8 		NodeId,
  IN  EVENT_TYPE	EventType,
  IN OUT BOOLEAN	*IsEventHandled
  );

EFI_STATUS
HedSmmGetTime (
  IN OUT TIME_STAMP *Time
  );

VOID
HEDRegisterPeriodicTimer(
  VOID
  );

static
EFI_STATUS
HandleNewHardErrorDetectionEventsIfAny(
	   IN UINT8 NodeId,
	   OUT BOOLEAN *IsEventHandled
	);

BOOLEAN
EFIAPI
SuppressCMCI(
VOID
);

EFI_STATUS
EFIAPI
CheckAndHandleHardErrorDetection(
    IN UINT8 NodeId,
    IN EVENT_TYPE EventType,
    OUT BOOLEAN *IsEventHandled
 );

#endif //_CORRECTED_ERROR_STORM_H_

#endif
