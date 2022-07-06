/**

Copyright (c) 2005 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


@file
  MpCommon.h

@brief
  some definitions for MP and HT driver.

**/
#ifndef _MP_COMMON_
#define _MP_COMMON_

#define VacantFlag          0x00
#define NotVacantFlag       0xff

#define MAXIMUM_CPU_NUMBER  0x40
#define STACK_SIZE_PER_PROC 0x1000

#pragma pack(1)
typedef struct {
  UINT32                Lock;
  UINT32                StackStart;
  UINT32                StackSize;
  UINT32                ApFunction;
  UINT32                ApFunctionArgument;
  IA32_DESCRIPTOR       GdtrProfile;
  IA32_DESCRIPTOR       IdtrProfile;
  UINT32                BufferStart;
  UINT32                PmodeOffset;
  volatile UINT32       FinishedCount;
  UINT32                WakeupCount;
  UINT32                SerializeLock;
  UINT32                StartState;
  VOID (EFIAPI *SemaphoreCheck) (UINT32 *SemaphoreAddress);
  UINT32                CFunction;
  UINT32                NumApsExecuting;
} MP_CPU_EXCHANGE_INFO;
#pragma pack()

VOID  *
AsmGetPmodeOffset (
  VOID
  )
/**

@brief

  Get protected mode code offset

  @param[in] None

  @retval Offset of protected mode code

**/
;

UINT32
AsmGetSemaphoreCheckOffset (
  VOID
  )
/**

@brief

  Get code offset of SemaphoreCheck

  @param[in] None

  @retval Offset of SemaphoreCheck

**/
;

VOID
AsmAcquireMPLock (
  IN   UINT8            *Lock
  )
/**

@brief

  Lock APs

  @param[in] Lock  - Lock state


**/
;

VOID
AsmReleaseMPLock (
  IN   UINT8            *Lock
  )
/**

@brief

  Release APs

  @param[in] Lock  - Lock state


**/
;

VOID  *
AsmGetAddressMap (
  VOID
  )
/**

@brief

  Get address map of RendezvousFunnelProc.

  @param[in] None

  @retval AddressMap  - Output buffer for address map information

**/
;

VOID
AsmCliHltLoop (
  VOID
  )
/**

@brief

  Do CLI and HALT processor

  @param[in] None


**/
;

#endif
