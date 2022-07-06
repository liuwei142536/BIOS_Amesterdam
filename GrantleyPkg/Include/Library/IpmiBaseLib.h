//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/** @file
  IPMI Command Library Header File.

  Copyright (c) 2011 - 2014, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.
**/

#ifndef _IPMI_BASE_LIB_H_
#define _IPMI_BASE_LIB_H_

#include <IpmiDefinitions.h>

//
// Prototype definitions for IPMI Library
//
EFI_STATUS
EfiInitializeIpmiBase (
  VOID
  )
/*++

Routine Description:

  Initialize the global varible with the pointer of IpmiTransport Protocol

Arguments:

  None

Returns:

  EFI_SUCCESS - Always return success

--*/
;

EFI_STATUS
EfiSendCommandToBMC (
  IN UINT8     NetFunction,
  IN UINT8     Command,
  IN UINT8     *CommandData,
  IN UINT8     CommandDataSize,
  OUT UINT8    *ResponseData,
  OUT UINT8    *ResponseDataSize
  )
/*++

Routine Description:

  Routine to send commands to BMC

Arguments:

  NetFunction       - Net function of the command
  Command           - IPMI Command
  CommandData       - Command Data
  CommandDataSize   - Size of CommandData
  ResponseData      - Response Data
  ResponseDataSize  - Response Data Size

Returns:

  EFI_NOT_AVAILABLE_YET - IpmiTransport Protocol is not installed yet

--*/
;

#endif

