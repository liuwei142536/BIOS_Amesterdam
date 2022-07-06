//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c)  1999 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
  IioConfig.h
    
Abstract:
  Data format for Universal Data Structure

--*/

#ifndef _IIO_CONFIG_H
#define _IIO_CONFIG_H
#pragma pack(1)  //to align members on byte boundary
typedef struct {
  #include    "IioConfigData.h"
} IIO_CONFIG;
#pragma pack()

#endif // _IIO_CONFIG_H
