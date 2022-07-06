//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license 
// agreement with Intel or your vendor.  This file may be      
// modified by the user, subject to additional terms of the    
// license agreement                                           
//
/*++

Copyright (c)  1999 - 2016 Intel Corporation. All rights reserved
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
#include <Library\PciExpressLib.h>
#include <Library\PciLib.h>
#include <Guid/PlatformInfo.h>
#include <IndustryStandard/Pci22.h>
#include <Cpu\CpuRegs.h>
#include <PchRegs.h>
#include <PchRegs/PchRegsRcrb.h>
#include <Platform.h>
#include "SioRegs.h"
#include <PchRegs/PchRegsSpi.h>
#include <PchAccess.h>
#include <Iio/IioRegs.h>
#include <FlashMap.h.>
#include <PchRegs/PchRegsLpc.h>
//#include <platform/spifvbservices/SpiFlashDevice.h> AptioV server override: Removed file not available in aptioV
#include <Ppi/SpiSoftStraps.h>
#include <IchRegsAliases.h>
#include "Guid/ServerMgmtSetupVariable.h"
#include "RcRegs.h"


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
//
// EEPROM Device Address
//
#define PLATFORM_FRU_SMB_ADDRESS      0xAC

#define FRU_BOARD_MFG_OFFSET          6

#define FRU_BOARD_AREA_START_OFFSET   3
#define FRU_BOARD_AREA_LENGTH_MASK    0x1F

//
// Comparing only first 6 digits of Board Part Number 
//
#define FRU_BOARD_PART_NUM_CMP_LENGTH 6

//
// FRU Board Part Number Length should not be exceed to 32 bytes
//
#define FRU_BOARD_PART_NUM_MAX_LENGTH 32

#define  NUM_OF_RETRIES                  0x03

typedef struct {
  UINT8 FormatVersionNumber;
  UINT8 InternalUseStartingOffset;
  UINT8 ChassisInfoStartingOffset;
  UINT8 BoardAreaStartingOffset;
  UINT8 ProductInfoStartingOffset;
  UINT8 MultiRecInfoStartingOffset;
  UINT8 Pad;
  UINT8 Checksum;
} FRU_COMMON_HEADER;

typedef struct {
  CHAR16    BoardInfoVersion[1];           
  CHAR16    BoardAreaLength[1];            
  CHAR16    LanguageCode[1];               
  CHAR16    DateTime[3];               
  CHAR16    MfrTypeLength[1];          
  CHAR16    MfrName[8];        
  CHAR16    NameTypeLength[1];                   
  CHAR16    BoardProductName[10];              
  CHAR16    SerialNumTypeLength[1];        
  CHAR16    PartNumTypeLength[1];          
  CHAR16    BoardPartNumber[10]; 
  CHAR16    FruFileIdTypeLength[1];        
  CHAR16    EndOfField[1];          
  CHAR16    Pad[7];
  CHAR16    BoardAreaChecksum[1];         
} BOARD_INFO_AREA;

//
//Board id by their PBA Number
//
#define DURANGODVP  	"H15180"
#define BCVRDIMM    	"H10083"
#define BCVSODIMM   	"H10084"
#define BCVCOMPACT      "H34774"  // Will take same path as BCV -S for BCV-C board too
#define CBYBASEBOARD	"H10087"
#define CBYMIDPLANE		"H10088"
#define	CBYSTORAGE		"H10089"
#define SPKSPDIMM     "G97179"
#define TBCCANYON     "H15712"
#define CBMNTN        "H29761"
#define TLAQPQ        "H15953"
#define PTWPEAK_SOLD_down         "H15868"  
#define PTWPEAK         "H15872" 
#define DURANGOPONS     "J24025"
#define DURANGOPPVNS    "J24329"

#define ECHOCANYON		"J16971"

typedef enum {
  SKU_EN1     = 0,
  SKU_EN2,                     
  SKU_EP,
  SKU_EXB,
  SKU_EXA,
  SKU_DE,
  SKU_NS = 13,
  SKU_MAX
} SKU_TYPE;

// Indication about whether it is Storage Sku or Comms Sku in BDX-NS
#define NS_STORAGE_SEG_SEL    BIT16

#define ONLY_REGISTER_OFFSET(x) (x & 0xFFF)

EFI_STATUS
PdrGetPlatformInfo (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT EFI_PLATFORM_INFO   *PlatformInfoHob
  );

EFI_STATUS
GetGnvBoardPartNum (    
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT CHAR8 *BoardPartNumStr
);

EFI_STATUS
GetPlatformInfo (    
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT EFI_PLATFORM_INFO   *PlatformInfoHob
);

#endif
