/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//

Copyright (c) 2009 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  MemoryErrorHandler.h

Abstract:
  This file will contain all definitions related to Memory Error Handler
  
------------------------------------------------------------------*/
	
#ifndef _MEMORY_ERROR_HANDLER_H
#define _MEMORY_ERROR_HANDLER_H

/*++
mRasEventHndlrTable[]
    This table is an array of memory RAS event handler
    functions.  The entry # in the table determines the
    priority of event handling. Entry 0 has the highest
    priority. The priority of event handling can be changed
    by re-ordering the entries in this table.
--*/
#define END_OF_TABLE  ((MEM_RAS_EVENT_HNDLR)((UINTN)(-1)))

#include <Library\emcaplatformhookslib.h>
#include <IndustryStandard/WheaDefs.h>

typedef struct {
  UINT16 McaCod;
  UINT16 McaCodMsk;
  UINT16 MsCod;
  UINT16 MsCodMsk;
} DIMM_ISOLATION_SKIP_DESC;

typedef
EFI_STATUS (*MEM_RAS_EVENT_HNDLR)(
    IN UINT8        NodeId,
    IN EVENT_TYPE   EventType,
    IN OUT BOOLEAN  *IsEventHandled
 );

static
EFI_STATUS
HardErrorDetection(
  IN  UINT8     NodeId,
  IN  EVENT_TYPE  EventType,
  IN OUT BOOLEAN  *IsEventHandled
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
ClearUnhandledCorrError(
  IN  UINT8 		NodeId,
  IN  EVENT_TYPE	EventType,
  IN OUT BOOLEAN	*IsEventHandled
  );
// Aptio V Server Override Start : Changes done to handle the Memory Correctable Erros (Default Memory Handler) when RankSparing/DevTagging setup option is disabled. 
static
EFI_STATUS
HandleDefaultMemoryError (
  IN  UINT8       NodeId,
	IN  EVENT_TYPE  EventType,
	IN OUT BOOLEAN  *IsEventHandled
	);
// Aptio V Server Override End : Changes done to handle the Memory Correctable Erros (Default Memory Handler) when RankSparing/DevTagging setup option is disabled. 
VOID LogMemError (
	UINT8 type, 
	UINT8 SubType, 
	UINT8 node, 
	UINT8 channel, 
	UINT8 rank,
	UINT8 dimm,
	UINT8 mcacode
	);

UINT64
MCAddress (
  UINT64 Addr,
  UINT64 Misc
  );

EFI_STATUS
FillPlatformMemorySection
(
  UINT8 McBank,
  UINT8 *skt,
  UINT32 *Sev,
  EMCA_MC_SIGNATURE_ERR_RECORD *McErrSig,
  PLATFORM_MEMORY_ERROR_DATA_UEFI_231 *MemSection
);


BOOLEAN
McErrNodeFn(
  );

EFI_STATUS
EFIAPI
EnableCSMI(
VOID
);

EFI_STATUS
EFIAPI
DisableCSMI(
VOID
);
VOID
CAPErrorHandler (
  UINT8   Node
  );


VOID
CorrectedMemoryErrorHandler (
  UINT8   Socket,
  UINT32  ImcEventsProgress,
  UINT32  ImcNewEvents
  );

VOID
InitIMCCorrectableError (
  UINT8 Node
  );

/*VOID
ClearVmseErrors (
  UINT8 node
  );

VOID
EnableVmseChErrors (
  UINT8 node
  );

VOID
VmseErrorHandler (
  UINT8   Socket
  );*/

BOOLEAN
ProcessCoreFatalError(
  VOID
  );

VOID
ClearMemoryErrors(
  VOID
  );

VOID
ClearImcRankCntrsPerNode (
  UINT8 Node
  );

UINT64
MCAddress (
  UINT64 Addr,
  UINT64 Misc);

extern UINT64 mUncoreSmiErrSrc[MAX_SOCKET];

#endif


