/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
Copyright (c) 2007 - 2009 Intel Corporation.  All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license.  Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:
    WheaPlatformSupport.h

Abstract:
    Definition of the Whea Platfrom Support protocol.

-----------------------------------------------------------------------------*/
//
// EPSD_IP_START
//

#ifndef _WHEA_PLATFORM_SUPPORT_PROTOCOL_H_
#define _WHEA_PLATFORM_SUPPORT_PROTOCOL_H_


#include  <Protocol\WheaSupport.h>

typedef struct _EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL;


//
// Extern the GUID for protocol users.
//
extern EFI_GUID         gEfiWheaPlatformSupportProtocolGuid;

//
// Protocol Data Definitions
// This installs the serialization actions for accessing Error Record persitant Storage.
//

typedef
EFI_STATUS
(EFIAPI *WHEA_PLATFORM_ELOG_RESTART_ALL) (
    IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL   *This
  );

typedef
EFI_STATUS
(EFIAPI *WHEA_PLATFORM_ELOG_RESTART) (
    IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL   *This,
    IN UINT16                               Type
  );

typedef
EFI_STATUS
(EFIAPI *WHEA_PLATFORM_ELOG_END) (
    IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL   *This
  );

typedef
EFI_STATUS
(EFIAPI *WHEA_PLATFORM_ELOG_MEMORY) (
    IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL   *This,
    IN UINT16                               ErrorType,
    IN MEMORY_DEV_INFO                      *MemInfo
  );

typedef
EFI_STATUS
(EFIAPI *WHEA_PLATFORM_ELOG_PCIE_ROOT_DEV_BRIDGE) (
    IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL   *This,
    IN UINT16                               ErrorType,
    IN PCIE_PCI_DEV_INFO                    *ErrPcieDev
  );

typedef
EFI_STATUS
(EFIAPI *WHEA_PLATFORM_ELOG_PCI_DEV) (
    IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL   *This,
    IN UINT16                               ErrorType,
    IN PCIE_PCI_DEV_INFO                    *ErrPcieDev
  );

typedef
EFI_STATUS
(EFIAPI *WHEA_PLATFORM_ELOG_IOH_DEV) (
    IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL   *This,
    IN UINT16                               ErrorType,
    IN PCIE_PCI_DEV_INFO                    *ErrPcieDev,
    IN UINT32                               ErrorSource
  );

//
// WHEA Platform Support Protocol
//
struct _EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL {
  WHEA_PLATFORM_ELOG_RESTART_ALL            WheaPlatformRestartAllElog;
  WHEA_PLATFORM_ELOG_RESTART                WheaPlatformRestartElog;
  WHEA_PLATFORM_ELOG_END                    WheaPlatformEndElog;
  WHEA_PLATFORM_ELOG_MEMORY                 WheaPlatformElogMemory;
  WHEA_PLATFORM_ELOG_PCIE_ROOT_DEV_BRIDGE   WheaPlatformElogPcieRootDevBridge;
  WHEA_PLATFORM_ELOG_PCI_DEV                WheaPlatformElogPciDev;
  WHEA_PLATFORM_ELOG_IOH_DEV                WheaPlatformElogIohDev;
};

#endif  // _WHEA_PLATFORM_SUPPORT_PROTOCOL_H_

