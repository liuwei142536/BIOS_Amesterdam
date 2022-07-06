/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/

/**

Copyright (c) 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:

  MeTypeLib.h

@brief:

  HECI Library

**/
#ifndef _ME_TYPE_LIB_H_
#define _ME_TYPE_LIB_H_

// definitions for use in hfr files
#define METYPE_UNDEF    0
#define METYPE_SPS      1
#define METYPE_ME9      2



#ifndef ME_CONFIG_DATA_FILE
BOOLEAN    MeTypeIsSps(void);
BOOLEAN    MeTypeIsSpsReady(void);
BOOLEAN    MeTypeIsAmt(void);
BOOLEAN    MeTypeIsAmtReady(void);
EFI_STATUS MeTypeSet(void);
void       MeTypeShowDebug(void);
#endif // ME_CONFIG_DATA_FILE

#endif // _ME_TYPE_LIB_H_
