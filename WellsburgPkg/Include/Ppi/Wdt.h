/** @file
  Watchdog Timer PPI

@copyright
  Copyright (c) 2010 - 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
**/
#ifndef _PEI_WDT_H_
#define _PEI_WDT_H_

#include <Protocol/Wdt.h>

//
// Extern the GUID for PPI users.
//
extern EFI_GUID       gWdtPpiGuid;

///
/// Reuse WDT_PROTOCOL definition
///
typedef WDT_PROTOCOL  WDT_PPI;

#endif
