/**
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement

@copyright
  Copyright (c)  2005 - 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

@file
  IderControllerDriver.h

@brief
  IDE-R Controller Driver Protocol definitions.
  We can specify normal IDE contorller driver and IDE-R controller by checking this GUID

**/
#ifndef _EFI_IDER_CONTROLLER_DRIVER_PROTOCOL_H_
#define _EFI_IDER_CONTROLLER_DRIVER_PROTOCOL_H_

#define EFI_IDER_CONTROLLER_DRIVER_PROTOCOL_GUID \
  { \
    0x956a2ed0, 0xa6cf, 0x409a, 0xb8, 0xf5, 0x35, 0xf1, 0x4c, 0x3e, 0x3c, 0x2 \
  }

extern EFI_GUID gEfiIderControllerDriverProtocolGuid;

#endif
