/*++
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  SpsMisc.h

Abstract:

 SPS definitions shared between PEI and DXE drivers.

--*/
#ifndef _SPS_MISC_
#define _SPS_MISC_

#include <Token.h>   //AptioV server override

typedef enum {
  LOADING_PEI = 0, 
  PEI_PRE_MRC,
  PEI_POST_MRC_NONE_INTERACTION,
  PEI_POST_MRC_INTERACTION,
  SPSMODE_EN,
  SPSMODE_DIS, 
  TIMESTAMP_TYPES
} TimeStampType;

#define CMOS_ADDR_PORT          0x70
#define CMOS_DATA_PORT          0x71
#define UPPER_CMOS_ADDR_PORT    0x72
#define UPPER_CMOS_DATA_PORT    0x73
//AptioV server override start: WA to avoid redefinition errors when including Token.h
#ifndef CMOS_BAD_REG
#define CMOS_BAD_REG            0x0E
#endif
//AptioV server override start: WA to avoid redefinition errors when including Token.h


#endif  //_SPS_MISC_

