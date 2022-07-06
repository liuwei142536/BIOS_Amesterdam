//
// This file contains a 'Sample Driver' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may be modified by the user, subject to
// the additional terms of the license agreement
//
/*++

Copyright (c) 2008 - 2010 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  CmosAccessLib.h

Abstract:

--*/

#ifndef _CMOS_ACCESS_LIB_
#define _CMOS_ACCESS_LIB_


#include <Base.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/CmosAccessLib.h>

#include "CmosTable.h"


// CMOS access Port address

#define PORT_70  0x70
#define PORT_71  0x71
#define PORT_72  0x72
#define PORT_73  0x73

#define CMOS_START_ADDR  40
#define CMOS_END_ADDR  255



/* Checksum is 16 bit which must be either In upper CMOS or lower CMOS

 MSB                                              LSB
 16                      8 7                        0
 ----------------------------------------------------
 | CMOS_CHECKSUM_ADDR_HIGH | CMOS_CHECKSUM_ADDR_LOW |
 ------------------------------------------------ ---

*/

// Dont set this value more than 254 and equal to 127

#define CMOS_CHECKSUM_ADDR_LOW  254  
#define CMOS_CHECKSUM_ADDR_HIGH (CMOS_CHECKSUM_ADDR_LOW + 1)



/**
  Reads from the checksum address.

  Reads the 8-bit checksum from specified CheckSum address. The 8-bit read value is returned.
  If 8-bit I/O port operations are not supported, then ASSERT().

  @param  None

  @return The value read.

**/

UINT16
ReadCheckSum(
  VOID
);

/**
  Write the Checksum to appropriate address.

  Write the 8-bit checksum to specified CheckSum address. 
  If 8-bit I/O port operations are not supported, then ASSERT().

  @param [in]    ChecksumValue 

  @return Status EFI_SUCCESS

**/

EFI_STATUS 
WriteCheckSum(
  IN UINT16 ChecksumValue
);


/**
  Calculates and update the Checksum to appropriate address.

  Write the 8-bit checksum to specified CheckSum address. 


  @param  None 

  @return Status EFI_SUCCESS

**/

EFI_STATUS
UpdateCheckSum(
  VOID
);


/**
  Check if the CMOS is corrupted by verifing the Checksum value .

  @param  None 

  @return Status EFI_SUCCESS/EFI_CRC_ERROR

**/

EFI_STATUS
VerifyCmosCheckSum(
  VOID
);


/**
  Performs the calculation of Checksum .

  @param  None 

  @return calculated CheckSum value

**/

UINT16
CalculateCmosCheckSum(
  VOID
);


/**
  Check if the CMOS location  has to be included for the Checksum calculation .

  @param [in]  CmosAddCnt Cmos address to be checked 

  @return TRUE / FALSE

**/

BOOLEAN
CheckIfSkipChkSum(
  IN UINT8 CmosAddCnt
);


/**
 
  Fill CMOS registers with the default values

  @param  none 

  @return Status 

**/

EFI_STATUS
CmosFillDefaults(
  VOID
);


/**
  Writes an 8-bit to I/O port.

  Writes the 8-bit to I/O port specified by PortAdd with the value specified by Value
  and returns Value.

  If 8-bit I/O port operations are not supported, then ASSERT().

  @param [in]    PortAdd  The I/O port to write.
  @param [in]    Value    The value to write to the I/O port.

  @return The value written to the I/O port.

**/

UINT8
WriteIoPort (
  IN  UINT8 PortAdd,
  IN  UINT8 Value
);



/**
  Reads an 8-bit from I/O port.

  Reads the 8-bit from I/O port specified by PortAdd and returns value.

  If 8-bit I/O port operations are not supported, then ASSERT().

  @param[in]   PortAdd  The I/O port to read.

  @return The value read from the I/O port.

**/

UINT8
ReadIoPort (
 IN UINT8 PortAdd
);



#endif // _CMOS_ACCESS_LIB_
