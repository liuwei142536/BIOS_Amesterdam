/** @file
  Header file for the PCH USB Common Driver

@copyright
  Copyright (c) 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
**/
#ifndef _USB_HC_PORT_PRECONDITION_H_
#define _USB_HC_PORT_PRECONDITION_H_



typedef struct _EFI_USB_HC_LOCATION {
  UINTN SegmentNumber;
  UINTN BusNumber;
  UINTN DeviceNumber;
  UINTN FunctionNumber;
} EFI_USB_HC_LOCATION;

typedef struct _EFI_USB_PORT_ENUM_TIMING_TABLE {
  UINTN ResetRecovery;
} EFI_USB_PORT_ENUM_TIMING_TABLE;

#ifndef EFI_FORWARD_DECLARATION
#define EFI_FORWARD_DECLARATION(x) typedef struct _##x x
#endif

EFI_FORWARD_DECLARATION (EFI_USB_HC_PORT_PRECONDITION);

typedef
BOOLEAN
(EFIAPI *EFI_USB_HC_PORT_RESET_STATUS) (
  IN EFI_USB_HC_PORT_PRECONDITION *This,
  IN UINT8                        PortNumber
  );

typedef struct _EFI_USB_HC_PORT_PRECONDITION {
  UINT8                           Revision;
  EFI_USB_HC_PORT_PRECONDITION    *Next;
  EFI_USB_HC_LOCATION             Location;
  EFI_USB_HC_PORT_RESET_STATUS    IsRootPortReset;
  EFI_USB_PORT_ENUM_TIMING_TABLE  Timing;
} EFI_USB_HC_PORT_PRECONDITION;

#endif
