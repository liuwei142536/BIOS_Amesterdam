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

  This file contains the Misc Oem Data (SMBIOS data type 0x82)

--*/

#include "MeSmbios.h"


//
// Static (possibly build generated) Oem data.
//
MISC_SUBCLASS_TABLE_DATA(EFI_MISC_OEM_TYPE_0x82, MiscOemType0x82) = {
  {
    0x82,                         // Smbios type
    0x14,                         // Data Length
    0x00                          // handle value is dummy and is filled by Smbios Core driver
  },
  '$','A','M','T',                // AMT Signature
  1,                              // AMT Supported?
  0,                              // AMT Enabled?
  0,                              // IDE-r Enabled?
  0,                              // Serial Over Lan Enabled?
  0,                              // Network interface of the Intel AMT part is enabled?
  0xA5,                           // 0xA5 (for backward compatibility)
  0x2F,                           // 1st Byte of OEM capabilities (bitmap)
  0x02,                           // 2nd Byte of OEM capabilities (Terminal Emulation), 0x02 = VT100+
  0,                              // 3rd Byte of OEM capabilities (reserved ?must be 0x00)
  0,                              // 4th Byte of OEM capabilities (reserved ?must be 0x00)
  0,                              // Kvm Enabled?
  0,                              // Reserved
  0                               // Terminator
};