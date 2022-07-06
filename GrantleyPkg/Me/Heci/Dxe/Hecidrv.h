/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  2007 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:
  Hecidrv.h

@brief:
  HECI driver

**/
#ifndef _HECIDRV_H_
#define _HECIDRV_H_

#define EFI_DEADLOOP() { volatile UINTN __DeadLoopVar__ = 1; while (__DeadLoopVar__); }
#define EVENT_REC_TOK                   "EventRec"        ///< GUID or String Event
#define SCRIPT_MEM_WRITE PCH_INIT_COMMON_SCRIPT_MEM_WRITE

//
// HECI driver private data structure
//
typedef struct {
  UINT16                  DeviceInfo;
  UINT32                  RevisionInfo;
  EFI_HECI_PROTOCOL       HeciCtlr;
  volatile UINT32         *HpetTimer;
#if defined(AMT_SUPPORT) && AMT_SUPPORT
  EFI_ME_RC_INFO_PROTOCOL MeRcInfo;
  UINT8                   MeFwImageType;
  // events
  EFI_EVENT               ExitBootServicesEvent;
  EFI_EVENT               ReadyToBootEvent;
  EFI_EVENT               EfiShellEvent;
  EFI_EVENT               EfiPmEvent;
  EFI_EVENT               LegacyBootEvent;
  EFI_EVENT               MpbClearEvent;
#endif // AMT_SUPPORT
  EFI_EVENT               SetupEnterEvent;
} HECI_INSTANCE;


VOID DeviceStatusSave(VOID);
VOID MeReadyToBootEventFunction(IN EFI_EVENT Event, IN VOID *ParentImageHandle);
VOID MeScriptSaveEventFunction(IN EFI_EVENT Event, IN VOID *ParentImageHandle);
EFI_STATUS InstallMbpDataProtocol(VOID);
VOID PerformIderWorkaround(IN EFI_HANDLE Handle);

EFI_STATUS EFIAPI HeciResetInterface(VOID);
EFI_STATUS EFIAPI HeciSendwACK(IN OUT UINT32 *Msg, IN UINT32 SendLen, IN OUT UINT32 *RcvLen,
                               IN UINT8 HostAddress, IN UINT8 MeAddress);
EFI_STATUS EFIAPI HeciReceive(IN UINT32 Block, IN OUT UINT32 *Msg, IN OUT UINT32 *Len);
EFI_STATUS EFIAPI HeciSend(IN UINT32 *Msg, IN UINT32 Len, IN UINT8 HAddr, IN UINT8 MeAddr);
EFI_STATUS EFIAPI HeciGetMeStatus(OUT UINT32 *MeStatus);
EFI_STATUS EFIAPI HeciGetMeMode(OUT UINT32 *MeMode);
EFI_STATUS EFIAPI HeciInitialize(VOID);
EFI_STATUS EFIAPI HeciReInitialize(VOID);
EFI_STATUS EFIAPI HeciMeResetWait(IN UINT32 Delay);
EFI_STATUS EFIAPI HeciReadAndFlush(IN UINT32 Blocking);

#endif // _HECIDRV_H_

