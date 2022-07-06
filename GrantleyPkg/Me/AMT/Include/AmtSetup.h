/*++

This file contains a 'Sample Driver' and is licensed as such
under the terms of your license agreement with Intel or your
vendor.  This file may be modified by the user, subject to
the additional terms of the license agreement

--*/

/*++
Copyright (c)  2008-2011 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

 AmtSetup.h

Abstract:

  Header file for AMT Setup.

--*/
#ifndef __AMT_SETUP_H__
#define __AMT_SETUP_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#if defined(AMT_SUPPORT) && AMT_SUPPORT

#define ME_IMAGE_1_5MB_FW                       0x03
#define ME_IMAGE_5MB_FW                         0x04

#define STR_ME_FW_SKU_1_5MB                     "1.5MB"
#define STR_ME_FW_SKU_5MB                       "5MB"
#define STR_ME_FW_SKU_UNIDENTIFIED              "Unidentified"

#define AMT_LOCAL_FW_UPDATE_QUESTION_ID         0x3000
#define AMT_KEY_ME1_QUESTION_ID                 0x3001
#define AMT_KEY_AMT1_QUESTION_ID                0x3002

#endif // AMT_SUPPORT

#ifndef ME_CONFIG_DATA_FILE
//This structure is used for setup.
typedef struct {
  UINT8 GrayOut;
} SETUP_AMT_FEATURES;
#endif // ME_CONFIG_DATA_FILE


/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __AMT_SETUP_H__
