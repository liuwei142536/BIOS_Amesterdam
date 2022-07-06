/** @file
  Initializes all LPTHC0 Hsio structures

@copyright
  Copyright (c) 2014 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement.

**/

#include "PchHsio.h"

#ifdef TRAD_FLAG
IOBP_MMIO_TABLE_STRUCT PchSataSharedHsioLptH_DT_C0_BeverlyCove[] = {
  { 0xEA002150, (UINT32)~(0x3F000000), 0x0A000000 },
  { 0xEA002350, (UINT32)~(0x3F000000), 0x0A000000 },
  { 0xEA002154, (UINT32)~(0x003F0000), 0x000A0000 },
  { 0xEA002354, (UINT32)~(0x003F0000), 0x000A0000 },
  { 0xEA002090, (UINT32)~(0x0000FFFF), 0x00003E67 },
  { 0xEA002290, (UINT32)~(0x0000FFFF), 0x00003E67 }
};

IOBP_MMIO_TABLE_STRUCT PchSataHsioLptH_DT_C0_BeverlyCove[] = {
  { 0xEA002550, (UINT32)~(0x3F000000), 0x0A000000 },
  { 0xEA002750, (UINT32)~(0x3F000000), 0x0A000000 },
  { 0xEA000950, (UINT32)~(0x3F000000), 0x0A000000 },
  { 0xEA000B50, (UINT32)~(0x3F000000), 0x0A000000 },
  { 0xEA002554, (UINT32)~(0x003F0000), 0x000A0000 },
  { 0xEA002754, (UINT32)~(0x003F0000), 0x000A0000 },
  { 0xEA000954, (UINT32)~(0x003F0000), 0x000A0000 },
  { 0xEA000B54, (UINT32)~(0x003F0000), 0x000A0000 },
  { 0xEA002490, (UINT32)~(0x0000FFFF), 0x00003E67 },
  { 0xEA002690, (UINT32)~(0x0000FFFF), 0x00003E67 },
  { 0xEA000890, (UINT32)~(0x0000FFFF), 0x00003E67 },
  { 0xEA000A90, (UINT32)~(0x0000FFFF), 0x00003E67 }
};
#endif // TRAD_FLAG


