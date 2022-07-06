//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license 
// agreement with Intel or your vendor.  This file may be      
// modified by the user, subject to additional terms of the    
// license agreement                                           
//
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
  PlatformInfo.h
  
Abstract:
  Platform Info Driver.

--*/

#ifndef _PLATFORM_INFO_H_
#define _PLATFORM_INFO_H_

#include <Token.h> // AptioV server override

#include <PiPei.h>
#include <Ppi/CpuIo.h>
#include <Ppi/PciCfg.h>
#include <Ppi/SmBus.h>
#include <Ppi/Spi.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PlatformHooksLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/PlatformInfo.h>
#include <IndustryStandard/Pci22.h>
#include <Cpu\CpuRegs.h>
#include <PchRegs.h>
#include <Platform.h>
#include "SioRegs.h"
#include <PchAccess.h>
#include <Iio/IioRegs.h>
#include <FlashMap.h.>
//#include <platform/spifvbservices/SpiFlashDevice.h> AptioV server override: Removed file not available in aptioV
#include <Ppi/SpiSoftStraps.h>
#include <IchRegsAliases.h>


#define EFI_PLATFORMINFO_DRIVER_PRIVATE_SIGNATURE SIGNATURE_32 ('P', 'I', 'N', 'F')

//
// CPU Model
//
#define  INVALID_MODEL             0x0

#define R_SB_SPI_FDOC         0xB0
#define R_SB_SPI_FDOD         0xB4
//AptioV server override start:
// to avoid redefinition error from token.h
#ifndef SPI_OPCODE_READ_INDEX
#define SPI_OPCODE_READ_INDEX            4
#endif
//AptioV server override end
#define PDR_REGION_START_OFFSET		0x0

typedef union BOARD_ID
{
   struct{             
      UINT8  BoardID0            :1;
      UINT8  BoardID1            :1;
      UINT8  BoardID2            :1;
      UINT8  BoardID3            :1;
      UINT8  BoardID4            :1;
      UINT8  BoardRev0           :1;
      UINT8  BoardRev1           :1;
      UINT8  Rsvd                :1;
   }BoardID;
}BOARD_ID;

typedef union RISER_ID
{
   struct{             
      UINT8  RiserID0            :1;
      UINT8  RiserID1            :1;
      UINT8  RiserID2            :1;
      UINT8  RiserID3            :1;
      UINT8  Rsvd                :4;
   }RiserID;
}RISER_ID;



EFI_STATUS
PdrGetPlatformInfo (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT EFI_PLATFORM_INFO   *PlatformInfoHob
  );

EFI_STATUS
GPIOGetPlatformInfo (    
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT EFI_PLATFORM_INFO   *PlatformInfoHob
);

#endif
