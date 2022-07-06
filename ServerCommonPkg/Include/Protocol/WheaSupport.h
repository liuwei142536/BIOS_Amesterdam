/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
Copyright (c) 2007 Intel Corporation.  All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license.  Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

    WheaSupport.h

Abstract:

    Definition of the Whea Core Support protocol.

-----------------------------------------------------------------------------*/
//
// EPSD_IP_START
//

#ifndef _WHEA_SUPPORT_PROTOCOL_H_
#define _WHEA_SUPPORT_PROTOCOL_H_


#include <Uefi.h>
#include <IndustryStandard\WheaDefs.h>
#include "WheaPlatformDefs.h"


typedef struct _EFI_WHEA_SUPPORT_PROTOCOL EFI_WHEA_SUPPORT_PROTOCOL;

//
// Add a new error source to for Whea Interface
//
typedef
EFI_STATUS
(EFIAPI *EFI_ADD_ERROR_SOURCE) (
  IN EFI_WHEA_SUPPORT_PROTOCOL          *This,
  IN WHEA_ERROR_TYPE                    Type,
  IN UINTN 	                            Flags,
  IN BOOLEAN 	                          EnableError,
  OUT UINT16                            *SourceID,
  IN UINTN	                            NoOfRecords,
  IN UINTN                              MaxSections,
  IN VOID 	                            *SourceData
  );

//
// This funtion will install serialization instruction for error injection method for an error type (e.g. memory UE).
// If error injection method already exist for the error type, the old method will be replced with new one.
//
typedef
EFI_STATUS
(EFIAPI *EFI_INSTALL_ERROR_INJECTION_METHOD) (
  IN EFI_WHEA_SUPPORT_PROTOCOL          *This,
  IN UINTN		                        ErrorType,
  IN UINTN		                        InstCount,
  IN VOID		                        *InstEntry
  );

//
// Tis function will get the current error injection capability installed in a bitmap.
//
typedef
EFI_STATUS
(EFIAPI *EFI_GET_ERROR_INJECTION_CAPABILITY) (
  IN EFI_WHEA_SUPPORT_PROTOCOL          *This,
  OUT UINTN                             *InjectCapability
  );

//
// Returns the Error log Address Range allocated for WHEA
//
typedef
EFI_STATUS
(EFIAPI *EFI_GET_ELAR) (
  IN EFI_WHEA_SUPPORT_PROTOCOL          *This,
  OUT UINTN                             *ElarSize,
  OUT VOID                              **LogAddress
  );

//
// This installs the serialization actions for accessing Error Record persitant Storage.
//
typedef
EFI_STATUS
(EFIAPI *EFI_INSTALL_ERROR_RECORD_METHOD) (
  IN EFI_WHEA_SUPPORT_PROTOCOL          *This,
  IN UINTN		                        InstCount,
  IN VOID		                        *InstEntry
  );

//
// WHEA Support Protocol
//
typedef struct _EFI_WHEA_SUPPORT_PROTOCOL {
  EFI_ADD_ERROR_SOURCE                  AddErrorSource;
  EFI_INSTALL_ERROR_INJECTION_METHOD    InstallErrorInjectionMethod;
  EFI_GET_ERROR_INJECTION_CAPABILITY    GetErrorInjectionCapability;
  EFI_GET_ELAR                          GetElar;
  EFI_INSTALL_ERROR_RECORD_METHOD       InstallErrorRecordMethod;
} EFI_WHEA_SUPPORT_PROTOCOL;



extern EFI_GUID         gEfiWheaSupportProtocolGuid;
extern EFI_GUID         gEfiWheaProcessorGenericErrorSectionGuid;
extern EFI_GUID         gEfiWheaProcessorSpecificErrorSectionGuid;
extern EFI_GUID         gEfiWheaPlatformMemoryErrorSectionGuid;
extern EFI_GUID         gEfiWheaPcieErrorSectionGuid;
extern EFI_GUID         gEfiWheaPciBusErrorSectionGuid;
extern EFI_GUID         gEfiWheaPciDevErrorSectionGuid;
extern EFI_GUID         gEfiWheaFirmwareErrorSectionGuid;





#endif  // _WHEA_SUPPORT_PROTOCOL_H_

//
// EPSD_IP_END
//
