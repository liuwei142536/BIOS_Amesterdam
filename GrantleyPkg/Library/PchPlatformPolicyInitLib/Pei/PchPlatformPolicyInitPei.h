/*++
  This file contains a 'Sample Driver' and is licensed as such  
  under the terms of your license agreement with Intel or your  
  vendor.  This file may be modified by the user, subject to    
  the additional terms of the license agreement 
--*/
/*++

Copyright (c) 2009 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  PchPlatformPolicyUpdatePei.h

Abstract:

  Header file for PEI PchPlatformPolicyUpdate.

--*/
#ifndef _PCH_PLATFORM_POLICY_UPDATE_PEI_H_
#define _PCH_PLATFORM_POLICY_UPDATE_PEI_H_

#include <Token.h> // AptioV server override

#include "PchAccess.h"
#include "Platform.h"
#include "PchHsio.h"
#include <Library/PlatformHooksLib.h>
#include <Library/PchPlatformLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PchPlatformPolicyInitPeiLib.h>
#include <Guid/PlatformInfo.h>
#include <Library/HobLib.h>
#include "HeciRegs.h"
#include "MeAccess.h"
#include "Library/PciLib.h"
#include "Library/PchChipsetInitHobLib.h"

//
// ChipsetInit settings defines
//
#define H2M_HSIO_MESSAGE        (0x7 << 28)///< Master type for all H2M Hsio messages
#define H2M_HSIO_CMD_GETHSIOVER 1          ///< Triggers Hsio version to be sent through ME/Host FW Status registers
#define H2M_HSIO_CMD_CLOSE      0          ///< Triggers H2M Hsio interface to close and revert FW Status registers
#define M2H_HSIO_MSG_ACK        0x7        ///< Ack sent in response to any H2M Hsio messages
#define STALL_100_MICROSECONDS  100
#define MAX_ME_MSG_ACK_TIMEOUT  100000     /// Wait max of 100ms (100 000us) for FW to acknowledge.
#define MAX_ME_READY_TIMEOUT    2000000    /// Wait max of 2s (2 000 000us) for FW to become ready


EFI_STATUS
EFIAPI
MphyHeciTrigger_GETHsioVER (
  IN      PCH_PLATFORM_POLICY_PPI *This,
  OUT     UINT32                  *Response
  )
/*++
  
  Routine Description:
  
    This function triggers GETHsioVER command to ME.
  
  Arguments:
  
    This             The PCH Platform Policy PPI instance
    Response     Response time
  
  Returns:
  
    EFI_SUCCESS             The command was issued successfuly to ME.
    EFI_TIMEOUT              Timeout occured, no ack from ME.

  
--*/
;

EFI_STATUS
EFIAPI
MphyHeciReceive_GETHsioVER(
  IN      PCH_PLATFORM_POLICY_PPI *This,
  OUT     UINT32                  *Msg
  )
/*++
  
  Routine Description:
  
    This function receives the response for GETHsioVER command .
  
  Arguments:
  
    This             The PCH Platform Policy PPI instance
  
  Returns:
  
    EFI_SUCCESS             The command was issued successfuly to ME.

  
--*/
;


EFI_STATUS
EFIAPI
MphyHeciClose_GETHsioVER(
  IN      PCH_PLATFORM_POLICY_PPI *This 
  )
/*++
  
  Routine Description:
  
    This function closes the GETHsioVER command.
  
  Arguments:
  
    This             The PCH Platform Policy PPI instance
  
  Returns:
  
    EFI_SUCCESS             The command was issued successfuly to ME.

  
--*/
;
#endif
