/** @file
  This protocol is used to synchronize between the components to decide if TMCSRCCLK needs to be set in BDS Phase.

  This protocol is EFI compatible.

  E.G. For current generation ICH, the 4GB-16MB to 4GB range can be mapped
  to PCI, SPI, or FWH.

@copyright
  Copyright (c) 2012 Intel Corporation. All rights reserved
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
#ifndef _EFI_PCH_SET_TMCSRCCLK_PROTOCOL_H_
#define _EFI_PCH_SET_TMCSRCCLK_PROTOCOL_H_

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                           gEfiPchSetTmcSrcClkProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _EFI_PCH_SET_TMCSRCCLK_PROTOCOL  EFI_PCH_SET_TMCSRCCLK_PROTOCOL;

struct _EFI_PCH_SET_TMCSRCCLK_PROTOCOL {
  UINT8   Revision;
};

#endif
