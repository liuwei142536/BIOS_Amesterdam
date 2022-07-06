//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c)  1999 - 2002 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  TcgLogHob.h

Abstract:

  GUIDs used for TCG-defined wake event data

--*/

#ifndef _TCG_DATA_HOB_GUID_H_
#define _TCG_DATA_HOB_GUID_H_

//
// Definitions for TCG-Defined Wake Event Data
//
#define EFI_TCG_WAKE_EVENT_DATA_HOB_GUID \
  { 0xBBB810BB, 0x5EF0, 0x4E8F, {0xB2, 0x98, 0xAD, 0x74, 0xAA, 0x50, 0xEF, 0x0A }}

extern EFI_GUID gEfiTcgWakeEventDataHobGuid;

#endif // _TCG_DATA_HOB_GUID_H_
