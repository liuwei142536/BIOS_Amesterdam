/*++
Copyright (c)  2011 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  MdesStatusCodeDxe.h

Abstract:


--*/
#ifndef _MDES_STATUS_CODE_DXE_H_
#define _MDES_STATUS_CODE_DXE_H_

#define MDES_STATUS_CODE_PROTOCOL_GUID \
  { \
    0xe5d0875a, 0xf647, 0x4e16, 0xbe, 0x4d, 0x95, 0x02, 0x40, 0x29, 0xcc, 0x44 \
  }

typedef
EFI_STATUS
(EFIAPI *SEND_STATUS_CODE) (
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN EFI_GUID                 * CallerId OPTIONAL,
  IN EFI_STATUS_CODE_DATA     * Data OPTIONAL
  );

typedef struct _MDES_STATUS_CODE_PROTOCOL {
  SEND_STATUS_CODE  SendMdesStatusCode;
} MDES_STATUS_CODE_PROTOCOL;

extern EFI_GUID  gMdesStatusCodeProtocolGuid;
#endif
