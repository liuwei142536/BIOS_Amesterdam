/**
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

@copyright
  Copyright (c) 2004-2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

@file
  AtAmHelper.h

@brief
  AT authetication module for using AT DXE driver.
  This library is utilized by the AT and related drivers to implement Theft Deterrence Technology AM module.

**/

#ifndef _ATAM_HELPER_H_
#define _ATAM_HELPER_H_

#define PEM_LINE_SIZE         64
#define PEM_INPUT_LINE_SIZE   48
#define PEM_UNITS_PER_LINE    (PEM_INPUT_LINE_SIZE / 3)
#define PEM_DECODED_LINE_SIZE 48
#define PEM_INVALID_CHAR      255
#define PEM_PAD_CHAR          64

EFI_STATUS
Base32Encode (
  UINT8 *encodedStr,
  UINTN *encodedLen,
  UINT8 *rawData,
  UINTN rawDataLen
);

VOID
Uint8ToUnicode (
  IN  CHAR8     *AsciiString,
  OUT CHAR16    *UnicodeString
);

VOID
DecimalToHexString (
  CHAR8 *decStr,
  UINT8 *u8Hex,
  UINTN hexIndex
);

INTN
Base32Decode (
  UINT8 *encodedStr,
  UINT8 *decodedData
);

VOID
ShowBuffer (
  UINT8  *Message,
  UINT32 Length
);

#endif // _ATAM_HELPER_H_
