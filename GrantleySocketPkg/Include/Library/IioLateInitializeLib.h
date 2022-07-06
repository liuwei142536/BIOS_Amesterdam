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

  IioEarlyInitializeLib.h

Abstract:

  This file provides required function headers publish for IioEarlyInitializeLib

--*/

#ifndef _IIO_LATEINITIALIZE_LIB_H_
#define _IIO_LATEINITIALIZE_LIB_H_

EFI_STATUS
IioLateInitializeEntry (
  IIO_GLOBALS  *IioGlobalData,
  EFI_HANDLE   DriverImageHandle
);

#endif // _IIO_LATEINITIALIZE_LIB_H_
