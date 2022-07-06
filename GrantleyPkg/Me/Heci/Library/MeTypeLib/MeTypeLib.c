/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  2013-2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:
  MeTypeLib.c

@brief:
  Me Type Lib implementation.

**/
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <PiDxe.h>
#include <Library/IoLib.h>
#include <Library/PciExpressLib.h>
#include "Library/PciLib.h"
#include "Library/PchPlatformLib.h"
#include <Library/DebugLib.h>
#include <MeAccess.h>
#include <PchAccess.h>
#include <HeciRegs.h>
#include <Library/MeTypeLib.h>
#include <Sps.h>

#include "MeTypeLibInternal.h"


/**
 * Get ME firmware type.
 *
 * retval ME_TYPE_UNDEF - simulation without ME support
 * retval ME_TYPE_SPS   - server firmware is running in ME
 * retval ME_TYPE_ME9   - workstation firmware is running in ME
 */
ON_BOARD_ME_TYPE
GetOnBoardMeType(VOID)
{
  ON_BOARD_ME_TYPE  retVal = ME_TYPE_UNDEF;
  HECI_MEFS1 Mefs1;

#ifdef SOFTSDV_FLAG
  if (IsSoftSdvPlatform()) {
    // Currently ME is not emulated
    return retVal;
  }
#endif // SOFTSDV_FLAG

  Mefs1.DWord = HeciPciReadMefs1();
  if (Mefs1.Bits.OperatingMode == MEFS1_OPMODE_SPS)
  {
    retVal = ME_TYPE_SPS;
  }
  else if (Mefs1.Bits.CurrentState > MEFS1_CURSTATE_RESET)
  {
    retVal = ME_TYPE_ME9;
  }
  return retVal;
}


/**

  @brief
  Checks if Me FW is AMT type

  @param[in] None

  Returns:
    On Error:                FALSE
    On Success:              FALSE or TRUE

**/
BOOLEAN MeTypeIsAmt(VOID)
{
 return (GetOnBoardMeType() == ME_TYPE_ME9);
}


/**

  @brief
  Checks if Me FW is AMT type and Me9 is ready

  @param[in] None

  Returns:
    On Error:                FALSE
    On Success:              FALSE or TRUE

**/
BOOLEAN MeTypeIsAmtReady(VOID)
{
 BOOLEAN    retVal = FALSE;
 HECI_MEFS1 Mefs1;

 if (GetOnBoardMeType() == ME_TYPE_ME9)
 {
   Mefs1.DWord = HeciPciReadMefs1();
   retVal = (Mefs1.Bits.InitComplete != 0);
 }
 return retVal;
}


/**

  @brief
  Checks if Me FW is SPS type

  @param[in] None

  Returns:
    On Error:                FALSE
    On Success:              FALSE or TRUE

**/
BOOLEAN MeTypeIsSps(VOID)
{
  return (GetOnBoardMeType() == ME_TYPE_SPS);
}


/**

  @brief
  Checks if Me FW is SPS type and Me is ready

  @param[in] None

  Returns:
    On Error:                FALSE
    On Success:              FALSE or TRUE

**/
BOOLEAN MeTypeIsSpsReady(VOID)
{
  BOOLEAN    retVal = FALSE;
  HECI_MEFS1 Mefs1;
  
  if (GetOnBoardMeType () == ME_TYPE_SPS)
  {
    Mefs1.DWord = HeciPciReadMefs1();
    retVal = (Mefs1.Bits.InitComplete != 0);
  }
  return retVal;
}


/**

  @brief
  Displays debug information about detected Me FW type

  @param[in] None

  Returns:
             None

**/
VOID MeTypeShowDebug(VOID)
{
  ON_BOARD_ME_TYPE  MeType = GetOnBoardMeType();
  
  DEBUG((EFI_D_ERROR, "HECI: MEFS1 0x%08X, ME type is ", HeciPciReadMefs1()));
  switch (MeType)
  {
  case ME_TYPE_UNDEF:
    DEBUG((EFI_D_ERROR, "ME_TYPE_UNDEF"));
  break;
  case ME_TYPE_SPS:
    DEBUG((EFI_D_ERROR, "ME_TYPE_SPS"));
  break;
  case ME_TYPE_ME9:
    DEBUG((EFI_D_ERROR, "ME_TYPE_ME9"));
  break;
  default:
    DEBUG((EFI_D_ERROR, "UNKNOWN"));;
  }
  DEBUG((EFI_D_ERROR, "\n"));
}

