/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/*++

Copyright (c)  1999 - 2010 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  SpsSiEn.h

Abstract:

  SPS SIEN Protocol

--*/
#ifndef _EFI_SPS_SIEN_H_
#define _EFI_SPS_SIEN_H_

#define SPS_SIEN_PROTOCOL_GUID \
{ \
    0xb88fc07f, 0x44b3, 0x48e0, {0xa1, 0x21, 0xcc, 0x8e, 0x7a, 0x28, 0x13, 0xde} \
}

typedef struct
{
  UINT8     SiEnEnable;
} EFI_SPS_SIEN_PROTOCOL;

extern EFI_GUID gEfiSpsSiEnProtocolGuid;

#endif // _EFI_SPS_SIEN_H_

