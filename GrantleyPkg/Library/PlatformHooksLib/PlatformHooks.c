/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

    PlatformHooks.c

Abstract:

    Platform Hooks file

--*/
#include <Base.h>
#include <PchRegs.h>
#include <Platform.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/BaseLib.h>

BOOLEAN
IsSoftSdvPlatform(VOID)
{
  UINT8        EmulationType;
  UINT32       CpuFamilyModelStepping;
  UINT32       msrData;
  UINT8        UncoreBus;
  UINT8        IioBus;


  AsmCpuid (1, &CpuFamilyModelStepping, NULL, NULL, NULL);

  msrData = (UINT32)AsmReadMsr32(0x300);
  UncoreBus = (UINT8) ((msrData >> 8) & 0xff);
  IioBus = 0;  // For BDX check emulation flag on S0 IIO_BUS
  
  if ((((CpuFamilyModelStepping & 0xFFFFFFF0) >> 4) == CPU_FAMILY_BDX) || (((CpuFamilyModelStepping & 0xFFFFFFF0) >> 4) == CPU_FAMILY_BDX_DE)) {
    EmulationType = PciRead8 ( PCI_LIB_ADDRESS(IioBus, 0, 0, 0xfc));
  } else {
    EmulationType = PciRead8 ( PCI_LIB_ADDRESS(UncoreBus, 16, 5, 0xb0));
  }

  if (EmulationType == BIT2) {
    return TRUE;
  } else {
    return FALSE;
  }
}

UINT8 
IsSimPlatform(VOID)
{
  UINT8        i;
  UINT8        EmulationType = 0xff;
    
  //CSR_EMULATION_FLAG_1_16_5_CFG_REG
  for (i = 0; i < MAX_SOCKET; i++) {
    EmulationType = PciRead8 ( PCI_LIB_ADDRESS((i*0x40 + 0x3F), 16, 5, 0xb0));
    if (EmulationType != 0xff)
      break;
  }

  if (EmulationType != 0xff) {
    EmulationType &= 7;
  } else {
    EmulationType = 0;
  }

  return EmulationType;
}

BOOLEAN 
IsManufacturingMode(
	VOID
)
{
  BOOLEAN ManufacturingMode;

  //
  // -BEGIN- HSD 3612389: Manufacturing Mode Jumper Re-Route
  // The new method for detection of manufacturing mode is to
  // read the Flash Descriptor Override Pin-Strap Status HSFS BIT13.
  //
  ManufacturingMode = TRUE;

  if (B_PCH_SPI_HSFS_FDOPSS & MmioRead32 (PCH_RCBA_ADDRESS + R_PCH_SPI_HSFS)) {
    ManufacturingMode = FALSE;
  }
  //
  // -END- HSD 3612389: Manufacturing Mode Jumper Re-Route
  //

  return ManufacturingMode;
}
