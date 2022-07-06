//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/** @file
  This library class get the PCIE MMCFG address.

  Copyright (c) 2011 - 2013, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

**/

#include <SysFunc.h>
#include "PcieAddress.h"

#pragma optimize ("",off)
//////////////////////////////////////////////////////////////////////////
//
// Common Silicon Address Library
// This Lib provide the way use platform Library instance
//
//////////////////////////////////////////////////////////////////////////

 PCIE_MMCFG_TABLE_TYPE *gMmcfgTable = NULL;


PCIE_MMCFG_TABLE_TYPE mMmcfgTable =\
  {
    {
      {'M', 'C', 'F', 'G'},   // Signature
      0x00000090,             // Length
      0x01,                   // Revision
      0x08,                   // The Maximum number of Segments
      0x00FF,                 // Valid Segment Bit Map, LSB Bit0 for Seg0, bit1 for seg1 ...
      {0x00,0x00,0x00,0x00}   // Reserved
    },
    {
      0x80000000,             // Base Address Low
      0x00000000,             // Base Address High
      0x0000,                 // Segment 0 
      0x00,                   // Start Bus
      0xFF,                   // End Bus
      {0x00,0x00,0x00,0x00}   // Reserved
    }
};
//
// Segment 1 ~ 7
//
PCIE_MMCFG_BASE_ADDRESS_TYPE mMmcfgAddr[] = \
{
  {
    0x00000000,             // Base Address Low
    0x00000000,             // Base Address High
    0x0001,                 // Segment 1
    0x00,                   // Start Bus
    0xFF,                   // End Bus
    {0x00,0x00,0x00,0x00}   // Reserved
  },
  {
    0x00000000,             // Base Address Low
    0x00000000,             // Base Address High
    0x0002,                 // Segment 2
    0x00,                   // Start Bus
    0xFF,                   // End Bus
    {0x00,0x00,0x00,0x00}   // Reserved
  },
  {
    0x00000000,             // Base Address Low
    0x00000000,             // Base Address High
    0x0003,                 // Segment 3
    0x00,                   // Start Bus
    0xFF,                   // End Bus
    {0x00,0x00,0x00,0x00}   // Reserved
  },

  {
    0x00000000,             // Base Address Low
    0x00000000,             // Base Address High
    0x0004,                 // Segment 4
    0x00,                   // Start Bus
    0xFF,                   // End Bus
    {0x00,0x00,0x00,0x00}   // Reserved
  },
  {
    0x00000000,             // Base Address Low
    0x00000000,             // Base Address High
    0x0005,                 // Segment 5
    0x00,                   // Start Bus
    0xFF,                   // End Bus
    {0x00,0x00,0x00,0x00}   // Reserved
  },

  {
    0x00000000,             // Base Address Low
    0x00000000,             // Base Address High
    0x0006,                 // Segment 6
    0x00,                   // Start Bus
    0xFF,                   // End Bus
    {0x00,0x00,0x00,0x00}   // Reserved
  },
  {
    0x00000000,             // Base Address Low
    0x00000000,             // Base Address High
    0x0007,                 // Segment 7
    0x00,                   // Start Bus
    0xFF,                   // End Bus
    {0x00,0x00,0x00,0x00}   // Reserved
  }
};



/**
  This Lib is used for platfor to set platform specific Pcie MMCFG Table

  @param  MmcfgTable: A pointer of the MMCFG Table structure for PCIE_MMCFG_TABLE_TYPE type.
  @param  NumOfSeg: Sumber of Segments in the table.

  @retval <>NULL The function completed successfully.
  @retval NULL Returen Error
**/
UINTN
EFIAPI
SetPcieSegMmcfgTable (
  IN PCIE_MMCFG_TABLE_TYPE *MmcfgTable,
  IN UINT32                 NumOfSeg
  )
{
  return 0;
};


/**
  This Lib return PCIE MMCFG Base Address 

  @param  Address: A pointer of the address of the Common Address Structure for PCIE type.
  @param  Buffer: A pointer of buffer for the value read from platform.

  @retval <>NULL The function completed successfully.
  @retval NULL Returen Error
  **/

UINTN
EFIAPI
GetPcieSegMmcfgBaseAddress (
  IN COMMON_ADDRESS_STRUCT    *Address
  )
{
  UINT32                        BaseAddressL=0;       // Processor-relative Base Address (Lower 32-bit) for the Enhanced Configuration Access Mechanism
  UINT32                        BaseAddressH=0; 
  UINTN SegMmcfgBase;

#if !defined(MINIBIOS_BUILD) && !defined(KTI_SW_SIMULATION)
  union {
    UINTN   D64;
    UINT32  D32[2];
  } Data;
#endif

#ifdef SKX_HOST
  BaseAddressH = ((PSYSHOST)(Address->Attribute.HostPtr))->var.common.mmCfgBaseH[Address->Pcie.Seg]; 
  BaseAddressL = ((PSYSHOST)(Address->Attribute.HostPtr))->var.common.mmCfgBaseL[Address->Pcie.Seg];
#endif

  if((BaseAddressL == 0) && (BaseAddressH == 0))
  {

#if defined(MINIBIOS_BUILD) || defined(KTI_SW_SIMULATION)
    BaseAddressL = 0x80000000;
    BaseAddressH = 0;
#else
    //
    // The first time default should be the PcdPciExpressBaseAddress
    //
    Data.D32[0] = Data.D32[1] = 0;
    Data.D64 = (UINTN) PcdGet64 (PcdPciExpressBaseAddress);
    BaseAddressL = Data.D32[0];
    BaseAddressH = Data.D32[1];
#endif
  }
  return SegMmcfgBase = BaseAddressL;

};

