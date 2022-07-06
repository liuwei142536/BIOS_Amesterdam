/** @file
  PCH Firmware Version Info implementation.

@copyright
  Copyright (c) 2011 - 2013 Intel Corporation. All rights reserved
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
  license agreement
**/
#define _EFI_IIO_UDS_PROTOCOL_H_	//Aptio V Server Override: Including "IioUds.h" in PchInit.h file, is causing build error. Defining _EFI_IIO_UDS_PROTOCOL_H_ will not include "IioUds.h" here.
#include "PchInit.h"

FVI_ELEMENT_AND_FUNCTION mPchFviElementsData[] = {
  {
    DEFAULT_FVI_ELEMENT_DATA (PCH),
    NULL
  },
  {
    {
      1,
      2,
      PCH_CRID_VERSION,
      PCH_CRID_STATUS,
      PCH_CRID_DISABLED,
    },
    NULL
  },
  {
    {
      1,
      0,
      PCH_CRID_VERSION,
      PCH_CRID_ORIGINAL_VALUE,
      {
        0
      },
    },
    NULL
  },
  {
    {
      1,
      0,
      PCH_CRID_VERSION,
      PCH_CRID_NEW_VALUE,
      {
        0
      },
    },
    NULL
  },
  {
    {
      1,
      0,
      RAID_RC_VERSION,
      RAID_FVI_STRING,
      {
        0
      },
    },
    NULL
  },
#ifdef ULT_FLAG
  {
    {
      1,
      0,
      {
        PCH_LPTLP_HSIO_VER_BX,
        0,
        0,
        0,
      },
      PCH_LPTLPBX_HSIO_STRING,
      {
        0
      },
    },
    NULL
  },
#endif //ULT_FLAG
#ifdef TRAD_FLAG
  {
    {
      1,
      0,
      {
#ifdef PCH_SERVER_BIOS_FLAG
        PCH_WBG_HSIO_VER_A0,
#else
        PCH_LPTH_HSIO_VER_B0,
#endif // PCH_SERVER_BIOS_FLAG
        0,
        0,
        0,
      },
#ifdef PCH_SERVER_BIOS_FLAG
      PCH_WBGA0_HSIO_STRING,
#else
      PCH_LPTHB0_HSIO_STRING,
#endif // PCH_SERVER_BIOS_FLAG
      {
        0
      },
    },
    NULL
  },
  {
    {
      1,
      0,
      {
#ifdef PCH_SERVER_BIOS_FLAG
        PCH_WBG_HSIO_VER_B0,
#else
        PCH_LPTH_HSIO_VER_C0,
#endif // PCH_SERVER_BIOS_FLAG
        0,
        0,
        0,
      },
#ifdef PCH_SERVER_BIOS_FLAG
      PCH_WBGB0_HSIO_STRING,
#else
      PCH_LPTHC0_HSIO_STRING,
#endif // PCH_SERVER_BIOS_FLAG
      {
        0
      },
    },
    NULL
  },
#endif //TRAD_FLAG
};

FVI_DATA_HUB_CALLBACK_CONTEXT mPchFviVersionData = {
  MISC_SUBCLASS_FVI_HEADER_ENTRY (PCH),
  mPchFviElementsData,
};

UINTN mPchFviElements = sizeof (mPchFviElementsData)/ sizeof (FVI_ELEMENT_AND_FUNCTION);

