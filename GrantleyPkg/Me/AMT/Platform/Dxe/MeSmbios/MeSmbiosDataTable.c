/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c) 2011 - 2013 Intel Corporation.  All rights reserved.
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  MeSmbiosDataTable.c
  
Abstract: 

  This driver parses the mMiscSubclassDataTable structure and reports
  any generated data to the DataHub. 

--*/

#include "MeSmbios.h"

//
// External definitions referenced by Data Table entries.
//
MISC_SUBCLASS_TABLE_EXTERNS (
  EFI_MISC_OEM_TYPE_0x82,
  MiscOemType0x82
  );
MISC_SUBCLASS_TABLE_EXTERNS (
  EFI_MISC_OEM_TYPE_0x83,
  MiscOemType0x83
  );

//
// Data Table
//
EFI_MISC_SUBCLASS_DATA_TABLE  mMiscSubclassDataTable[] = {
  MISC_SUBCLASS_TABLE_ENTRY_DATA_AND_FUNCTION(EFI_MISC_OEM_TYPE_0x82, MiscOemType0x82, MiscOemType0x82),
  MISC_SUBCLASS_TABLE_ENTRY_DATA_AND_FUNCTION(EFI_MISC_OEM_TYPE_0x83, MiscOemType0x83, MiscOemType0x83)
};

//
// Number of Data Table entries.
//
UINTN  mSmbiosMiscDataTableEntries = sizeof (mMiscSubclassDataTable) / sizeof (EFI_MISC_SUBCLASS_DATA_TABLE);
