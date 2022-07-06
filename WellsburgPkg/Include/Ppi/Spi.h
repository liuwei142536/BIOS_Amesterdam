/** @file
  This file defines the EFI SPI PPI which implements the
  Intel(R) PCH SPI Host Controller Compatibility Interface.

@copyright
  Copyright (c) 2006 - 2012 Intel Corporation. All rights reserved
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
#ifndef _PEI_SPI_H_
#define _PEI_SPI_H_

#include <Protocol/Spi.h>

//
// Extern the GUID for PPI users.
//
extern EFI_GUID           gPeiSpiPpiGuid;

///
/// Reuse the EFI_SPI_PROTOCOL definitions
/// This is possible becaues the PPI implementation does not rely on a PeiService pointer,
/// as it uses EDKII Glue Lib to do IO accesses
///
typedef EFI_SPI_PROTOCOL  PEI_SPI_PPI;

#endif
