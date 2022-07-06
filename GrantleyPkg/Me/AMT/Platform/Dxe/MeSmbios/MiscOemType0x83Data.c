/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c) 2005 - 2012 Intel Corporation.  All rights reserved.

This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  MiscOemType0x81Data.c
  
Abstract: 

  This file contains the Misc Oem Data (SMBIOS data type 0x83)

--*/

#include "MeSmbios.h"


//
// Static (possibly build generated) Oem data.
//
MISC_SUBCLASS_TABLE_DATA(EFI_MISC_OEM_TYPE_0x83, MiscOemType0x83) = {
  {
    0x83,                         // Smbios type
    0x40,                         // Data Length
    0x00                          // handle value is dummy and is filled by Smbios Core driver
  },
  {
    0                             // Cpu Capabilities
  },
  {
    0x00,                         // MEBx Version.Major
    0x00,                         // MEBx Version.Minor
    0x00,                         // MEBx Version.Hotfix
    0x00                          // MEBx Version.Build
  },
  0,                              // Pch Capabilities
  0,                              // Me Capabilities
  0,                              // Me Fw Configured
  {
    0                             // Network Device
  },
  0,                              // Bios Capabilities
  'v','P','r','o',                // Structure Identifier
  0,                              // Reserved
  0                               // Terminator
};