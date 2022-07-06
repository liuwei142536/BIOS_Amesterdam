/** @file
  PCH Thermal Levels Protocol

@copyright
  Copyright (c) 2014 Intel Corporation. All rights reserved
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
#ifndef _PCH_THERMAL_LEVELS_H_
#define _PCH_THERMAL_LEVELS_H_

#include <Token.h> // AptioV server override

#ifdef DE_SKU

#define EFI_CAPSULE_VARIABLE_NAME           L"CapsuleUpdateData"

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                             gPchThermalLevelsProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _PCH_THERMAL_LEVELS_PROTOCOL          PCH_THERMAL_LEVELS_PROTOCOL;

//
// Related Definitions
//
///
/// PCH THermal Levels in 8bit format/Centigrade
///
typedef struct
{
  UINT8 T1;
  UINT8 T2;
  UINT8 T3;
  UINT8 Phl;
  UINT8 Ctt;
  UINT8 TTEnable          :1;
  UINT8 TTState13Enable   :1;
} PCH_THERMAL_LEVELS;

//
// Member functions
//

/**
  Get PCH Thermal Leveles.
  Retrieve thermal levels from module glpbal variable put there by InitPchThermalLevels

  @param[in] This                      Pointer to the PCH_THERMAL_LEVELS_PROTOCOL instance.
  @param[out] ThermalLevels     Output data
  @retval EFI_SUCCESS                     Data retrieved Successfully.
  @retval EFI_INVALID_PARAMETER    If ThermalLevels or This is invalid.
**/
typedef
EFI_STATUS
(EFIAPI *PCH_THERMAL_LEVELS_REQUEST) (
  IN     PCH_THERMAL_LEVELS_PROTOCOL       *This,
  OUT    PCH_THERMAL_LEVELS                *ThermalLevels
  );

///
/// Interface structure for the Pch Thermal Levels Protocoll
///
struct _PCH_THERMAL_LEVELS_PROTOCOL {
  PCH_THERMAL_LEVELS_REQUEST GetPchThermalLevels;
};

#endif //DE_SKU
#endif //_PCH_THERMAL_LEVELS_H_
