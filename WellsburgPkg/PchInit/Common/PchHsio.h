/** @file
 
  Header file with all common Hsio information
 
@copyright
  Copyright (c) 2014 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.
 
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
 
**/

#ifndef _PCH_HSIO_H_
#define _PCH_HSIO_H_

#include <Token.h> // AptioV server override

#include "PchAccess.h"
#include <Library\PchPlatformLib.h>
#include "IobpDefinitions.h"
#ifdef TRAD_FLAG
#ifdef PCH_SERVER_BIOS_FLAG
#include "PchHsioWbgA0.h"
#include "PchHsioWbgB0.h"
#else
#include "PchHsioLptHB0.h"
#include "PchHsioLptHC0.h"
#include "PchHsioLptHC0_BeverlyCove.h"
#ifdef ULT_FLAG
#include "PchHsioLptLpBx.h"
#endif //ULT_FLAG
#endif // PCH_SERVER_BIOS_FLAG
#endif //TRAD_FLAG


#ifdef ULT_FLAG
extern IOBP_MMIO_TABLE_STRUCT PchSerialIoSnoopLptLp[8];
extern IOBP_MMIO_TABLE_STRUCT PchSerialIoIntsLptLp[7];
#endif // ULT_FLAG

#ifndef C_ASSERT
#if defined(_DOS) || defined(_LINUX)
#define C_ASSERT(x)
#else
#define C_ASSERT(e) typedef char __C_ASSERT__[(e)?1:-1]
#endif
#endif


//
// Declarations for mphy debug options
//

// Invalid endpoint, not equal to: 
#define USB3_MPHY_ENDPOINT 0xE9 // 233 IOSF Endpoint address of USB3 MPHY
#define SATA_MPHY_ENDPOINT 0xEA // 234 IOSF Endpoint address of SATA MPHY
#define DMI_MPHY_ENDPOINT  0xEB // 235 IOSF Endpoint address of DMI MPHY
#define INVALID_MPHY_ENDPOINT 0x01

#pragma pack(1)


 /** 
 * Defines the MPHY Tables entry used by PCH Platform Policy
 */ 
typedef struct _MPHY_TABLE_PTR_W_SIZE{
  IOBP_MMIO_TABLE_STRUCT *Table;
  UINT16 Size;
}MPHY_TABLE_PTR_W_SIZE;

typedef struct _BOARD_DEPENDENT_MPHY_TABLES{
  MPHY_TABLE_PTR_W_SIZE PchSataHsio_DT;
  MPHY_TABLE_PTR_W_SIZE PchSataSharedHsio_DT;
}BOARD_DEPENDENT_MPHY_TABLES;
  
/** 
* Defines the MPHY Table entry used by the PMC 
*/ 
typedef struct _ICC_MPHY_TABLE_ENTRY 
{ 
   UINT16 Address;    ///< IOSF Address in MPHY to be programmed. 
   UINT32 Data;       ///< Data value to be written 
   UINT8  Endpoint;   ///< IOSF Target MPHY Endpoint 
}ICC_MPHY_TABLE_ENTRY; 
 
C_ASSERT(sizeof(ICC_MPHY_TABLE_ENTRY) == 7); 

/** 
* Defines the MPHY Table used by the PMC 
*/ 
typedef struct _ICC_MPHY_PMC_TABLE 
{ 
   UINT8                NumEntries;  ///< Number of MPHY entries in the table 
   ICC_MPHY_TABLE_ENTRY Entries[1];  ///< MPHY entries, actually expect more than one 
                                     ///  but expected to only be used as a pointer structure 
}ICC_MPHY_PMC_TABLE; 


/** 
* Defines the MPHY Table as used in BIOS 
*/ 
typedef struct _ICC_MPHY_BIOS_TABLE 
{ 
   UINT16               Crc16;       ///< Table checksum CRC-16 
   UINT16               Version;     ///< Table version               
   UINT8                NumEntries;  ///< Number of MPHY entries in the table 
   ICC_MPHY_TABLE_ENTRY Entries[1];  ///< MPHY entries, actually expect more than one 
                                     ///  but expected to only be used as a pointer structure 
}ICC_MPHY_BIOS_TABLE,*PTR_ICC_MPHY_BIOS_TABLE; 

#pragma pack()

#endif //_PCH_HSIO_H_
