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

  SpsNm.h

Abstract:

  SPS NM Protocol

--*/
#ifndef _EFI_SPS_NM_H_
#define _EFI_SPS_NM_H_

#define SPS_NM_PROTOCOL_GUID \
{ \
    0x74eab115, 0xc17, 0x422d, {0xbc, 0x21, 0x7f, 0xc7, 0x26, 0xdc, 0x92, 0x18 } \
}

typedef struct
{
  UINT8     NmEnable;
} EFI_SPS_NM_PROTOCOL;

extern EFI_GUID gEfiSpsNmProtocolGuid;

#endif // _EFI_SPS_NM_H_

