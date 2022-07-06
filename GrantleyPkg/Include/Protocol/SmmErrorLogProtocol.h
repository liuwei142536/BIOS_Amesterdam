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

  SmmErrorLogProtocol.h

Abstract:

  Smm Error Log  Driver functions to be used in Memras driver
--*/

#ifndef _SMM_ERROR_LOG_PROTOCOL_H_
#define _SMM_ERROR_LOG_PROTOCOL_H_


#define EFI_SMM_ERROR_LOG_PROTOCOL_GUID \
	{0x234f52e5, 0xd01c, 0x41c4, 0xb4, 0x37, 0x27, 0xdc, 0xd3, 0x89, 0x8d, 0xd5};


#define EFI_FORWARD_DECLARATION(x) typedef struct _##x x
EFI_FORWARD_DECLARATION (EFI_SMM_ERROR_LOG_PROTOCOL);

typedef
EFI_STATUS
(EFIAPI * EFI_PROPAGATE_SIGNALS)(
  VOID
 );

typedef
EFI_STATUS
(EFIAPI * EFI_ENABLE_CSMI)(
  VOID
 );

typedef
EFI_STATUS
(EFIAPI * EFI_DISABLE_CSMI)(
  VOID
 );

typedef
VOID
(EFIAPI * EFI_ENABLE_SMI_FOR_CORRECTED_MEMORY_ERRORS)(
  UINT8 node
 );

//
// Protocol Definition
//
typedef struct _EFI_SMM_ERROR_LOG_PROTOCOL {
  EFI_PROPAGATE_SIGNALS                  PropagateSignals;
  EFI_ENABLE_CSMI                        EnableCSMI;
  EFI_DISABLE_CSMI                       DisableCSMI;
  EFI_ENABLE_SMI_FOR_CORRECTED_MEMORY_ERRORS  EnableSMIForCorrectedMemoryErrors;
} EFI_SMM_ERROR_LOG_PROTOCOL;

extern EFI_GUID         gEfiSmmErrorLogProtocolGuid;


#endif
