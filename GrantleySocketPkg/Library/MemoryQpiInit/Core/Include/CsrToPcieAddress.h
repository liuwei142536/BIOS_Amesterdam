//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/** @file

  Copyright (c) 2011 - 2015, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.
**/

#ifndef __CSR_TO_PCIE_ADDRESS_H__
#define __CSR_TO_PCIE_ADDRESS_H__


#include <CommonAccessType.h>

//////////////////////////////////////////////////////////////////////////
//
// Common Silicon Address Library
// This Lib provide the way use platform Library instance
//
//////////////////////////////////////////////////////////////////////////

/**
  This Lib Convert the CPU Boxtype, FuncBlock, Offset into PCIE (Seg, Bus, Device, Function, RegOffset) tyepe

  @param  Address: A pointer of the address of the Common Address Structure with Csr or CsrEx type
          e.g. COMMON_CSR_EX_ADDR_TYPE or COMMON_CSR_ADDR_TYPE.
  @param  A pointer of Common Address Structure with Pcie type.

  @retval PCIE address with COMMON_PCIE_ADDR_TYPE type.
**/


IN COMMON_PCIE_ADDR_TYPE
EFIAPI
GetPcieAddress (
  IN COMMON_ADDRESS_STRUCT    *Address
  );


#endif
