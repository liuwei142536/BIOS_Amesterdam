/*++
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2010-2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  MeAccess.h

Abstract:

  Macros to simplify and abstract the interface to PCI configuration.

--*/
#ifndef _ME_ACCESS_H_
#define _ME_ACCESS_H_

#include "MeChipset.h"

//
// HECI PCI Access Macro
//
#define HeciPciRead32(Register) PciRead32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, Register))

#define HeciPciWrite32(Register, Data) \
  PciWrite32 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI_FUNCTION_NUMBER, \
  Register), \
  (UINT32) Data \
  )

#define HeciPciOr32(Register, Data) \
  PciOr32 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI_FUNCTION_NUMBER, \
  Register), \
  (UINT32) Data \
  )

#define HeciPciAnd32(Register, Data) \
  PciAnd32 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI_FUNCTION_NUMBER, \
  Register), \
  (UINT32) Data \
  )

#define HeciPciAndThenOr32(Register, AndData, OrData) \
  PciAndThenOr32 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI_FUNCTION_NUMBER, \
  Register), \
  (UINT32) AndData, \
  (UINT32) OrData \
  )

#define HeciPciRead16(Register) PciRead16 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, Register))

#define HeciPciWrite16(Register, Data) \
  PciWrite16 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI_FUNCTION_NUMBER, \
  Register), \
  (UINT16) Data \
  )

#define HeciPciOr16(Register, Data) \
  PciOr16 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI_FUNCTION_NUMBER, \
  Register), \
  (UINT16) Data \
  )

#define HeciPciAnd16(Register, Data) \
  PciAnd16 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI_FUNCTION_NUMBER, \
  Register), \
  (UINT16) Data \
  )

#define HeciPciAndThenOr16(Register, AndData, OrData) \
  PciAndThenOr16 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI_FUNCTION_NUMBER, \
  Register), \
  (UINT16) AndData, \
  (UINT16) OrData \
  )

#define HeciPciRead8(Register)  PciRead8 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, Register))

#define HeciPciWrite8(Register, Data) \
  PciWrite8 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI_FUNCTION_NUMBER, \
  Register), \
  (UINT8) Data \
  )

#define HeciPciOr8(Register, Data) \
  PciOr8 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI_FUNCTION_NUMBER, \
  Register), \
  (UINT8) Data \
  )

#define HeciPciAnd8(Register, Data) \
  PciAnd8 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI_FUNCTION_NUMBER, \
  Register), \
  (UINT8) Data \
  )

#define HeciPciAndThenOr8(Register, AndData, OrData) \
  PciAndThenOr8 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI_FUNCTION_NUMBER, \
  Register), \
  (UINT8) AndData, \
  (UINT8) OrData \
  )

//
// HECI2 PCI Access Macro
//
#define Heci2PciRead32(Register)  PciRead32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, Register))

#define Heci2PciWrite32(Register, Data) \
  PciWrite32 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI2_FUNCTION_NUMBER, \
  Register), \
  (UINT32) Data \
  )

#define Heci2PciOr32(Register, Data) \
  PciOr32 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI2_FUNCTION_NUMBER, \
  Register), \
  (UINT32) Data \
  )

#define Heci2PciAnd32(Register, Data) \
  PciAnd32 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI2_FUNCTION_NUMBER, \
  Register), \
  (UINT32) Data \
  )

#define Heci2PciAndThenOr32(Register, AndData, OrData) \
  PciAndThenOr32 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI2_FUNCTION_NUMBER, \
  Register), \
  (UINT32) AndData, \
  (UINT32) OrData \
  )

#define Heci2PciRead16(Register)  PciRead16 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, Register))

#define Heci2PciWrite16(Register, Data) \
  PciWrite16 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI2_FUNCTION_NUMBER, \
  Register), \
  (UINT16) Data \
  )

#define Heci2PciOr16(Register, Data) \
  PciOr16 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI2_FUNCTION_NUMBER, \
  Register), \
  (UINT16) Data \
  )

#define Heci2PciAnd16(Register, Data) \
  PciAnd16 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI2_FUNCTION_NUMBER, \
  Register), \
  (UINT16) Data \
  )

#define Heci2PciAndThenOr16(Register, AndData, OrData) \
  PciAndThenOr16 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI2_FUNCTION_NUMBER, \
  Register), \
  (UINT16) AndData, \
  (UINT16) OrData \
  )

#define Heci2PciRead8(Register) PciRead8 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, Register))

#define Heci2PciWrite8(Register, Data) \
  PciWrite8 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI2_FUNCTION_NUMBER, \
  Register), \
  (UINT8) Data \
  )

#define Heci2PciOr8(Register, Data) \
  PciOr8 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI2_FUNCTION_NUMBER, \
  Register), \
  (UINT8) Data \
  )

#define Heci2PciAnd8(Register, Data) \
  PciAnd8 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI2_FUNCTION_NUMBER, \
  Register), \
  (UINT8) Data \
  )

#define Heci2PciAndThenOr8(Register, AndData, OrData) \
  PciAndThenOr8 ( \
  PCI_LIB_ADDRESS (ME_BUS, \
  ME_DEVICE_NUMBER, \
  HECI2_FUNCTION_NUMBER, \
  Register), \
  (UINT8) AndData, \
  (UINT8) OrData \
  )
// AptioV server override start: dynamic MMCFG base support
#ifndef DYNAMIC_MMCFG_BASE_FLAG 
#ifndef MmPciAddress
#define MmPciAddress(Segment, Bus, Device, Function, Register) \
  ( (UINTN) PcdGet64 (PcdPciExpressBaseAddress) + \
    (UINTN)(Bus << 20) + \
    (UINTN)(Device << 15) + \
    (UINTN)(Function << 12) + \
    (UINTN)(Register) \
  )
#endif
#endif
//AptioV server override end: dynamic MMCFG base change support.
#define MmPci32Ptr(Segment, Bus, Device, Function, Register) \
    ((volatile UINT32 *) MmPciAddress (Segment, Bus, Device, Function, Register))
#define MmPci32(Segment, Bus, Device, Function, Register) *MmPci32Ptr (Segment, Bus, Device, Function, Register)

//
// ME Macro Setting
//
#define McD0PciCfg64(Register)              MmPci64 (0, MC_BUS, 0, 0, Register)
#define McD0PciCfg32And(Register, AndData)  PciAnd32 (PCI_LIB_ADDRESS (MC_BUS, 0, 0, Register), (UINT32) AndData)
#define McD0PciCfg16And(Register, AndData)  PciAnd16 (PCI_LIB_ADDRESS (MC_BUS, 0, 0, Register), (UINT16) AndData)

///
/// Device 22, Function 0
///
#define PchD22PciCfg32(Register)                            MmPci32 (0, 0, 22, 0, Register)
#define PchD22PciCfg32Or(Register, OrData)                  MmPci32Or (0, 0, 22, 0, Register, OrData)
#define PchD22PciCfg32And(Register, AndData)                MmPci32And (0, 0, 22, 0, Register, AndData)
#define PchD22PciCfg32AndThenOr(Register, AndData, OrData)  MmPci32AndThenOr (0, 0, 22, 0, Register, AndData, OrData)

//
//  MMIO access macros
//
#define MmioAddress(BaseAddr, Register) ((UINTN) BaseAddr + (UINTN) (Register))

//
// 16 bit MMIO access
//
#define Mmio16Ptr(BaseAddr, Register) ((volatile UINT16 *) MmioAddress (BaseAddr, Register))

#define Mmio16(BaseAddr, Register)    *Mmio16Ptr (BaseAddr, Register)

#define Mmio16Or(BaseAddr, Register, OrData) \
  Mmio16 (BaseAddr, Register) = (UINT16) \
    (Mmio16 (BaseAddr, Register) | (UINT16) (OrData))

#define Mmio16And(BaseAddr, Register, AndData) \
  Mmio16 (BaseAddr, Register) = (UINT16) \
    (Mmio16 (BaseAddr, Register) & (UINT16) (AndData))

#define Mmio16AndThenOr(BaseAddr, Register, AndData, OrData) \
  Mmio16 (BaseAddr, Register) = (UINT16) \
    ((Mmio16 (BaseAddr, Register) & (UINT16) (AndData)) | (UINT16) (OrData))

//
// 32 bit MMIO access
//
#define Mmio32Ptr(BaseAddr, Register) ((volatile UINT32 *) MmioAddress (BaseAddr, Register))

#define Mmio32(BaseAddr, Register)    *Mmio32Ptr (BaseAddr, Register)

#define Mmio32Or(BaseAddr, Register, OrData) \
  Mmio32 (BaseAddr, Register) = (UINT32) \
    (Mmio32 (BaseAddr, Register) | (UINT32) (OrData))

#define Mmio32And(BaseAddr, Register, AndData) \
  Mmio32 (BaseAddr, Register) = (UINT32) \
    (Mmio32 (BaseAddr, Register) & (UINT32) (AndData))

#define Mmio32AndThenOr(BaseAddr, Register, AndData, OrData) \
  Mmio32 (BaseAddr, Register) = (UINT32) \
    ((Mmio32 (BaseAddr, Register) & (UINT32) (AndData)) | (UINT32) (OrData))

//
// Memory Controller PCI access macros
//
#define MCH_REGION_BASE               (McD0PciCfg64 (MC_MCHBAR_OFFSET) &~BIT0)
#define McMmioAddress(Register)       ((UINTN) MCH_REGION_BASE + (UINTN) (Register))

#define McMmio8Ptr(Register)          ((volatile UINT8 *) McMmioAddress (Register))
#define McMmio8(Register)             *McMmio8Ptr (Register)
#define McMmio8Or(Register, OrData)   (McMmio8 (Register) |= (UINT8) (OrData))
#define McMmio8And(Register, AndData) (McMmio8 (Register) &= (UINT8) (AndData))
#define McMmio8AndThenOr(Register, AndData, OrData) \
    (McMmio8 (Register) = (McMmio8 (Register) & (UINT8) (AndData)) | (UINT8) (OrData))

#endif
