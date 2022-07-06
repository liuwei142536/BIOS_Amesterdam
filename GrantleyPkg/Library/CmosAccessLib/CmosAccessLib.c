/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/*++

Copyright (c) 2010 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  CmosMapDxe.c

Abstract:

--*/
#ifndef _CMOS_ACCESS_LIB_C_
#define _CMOS_ACCESS_LIB_C_

#include "CmosAccessLibInternals.h"

#define CMOS_LOW_MEM_ST  0
#define CMOS_LOW_MEM_END  127

#define CMOS_HIGH_MEM_ST  128
#define CMOS_HIGH_MEM_END  255

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
  IN UINT8 PortAdd,
  IN UINT8 Value
)
{
  return IoWrite8 (PortAdd, Value);
}


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
)
{
  return IoRead8 (PortAdd);
}


/**

  Fill CMOS registers with the default values

  @param  none

  @return Status

**/

EFI_STATUS
CmosFillDefaults(
  VOID
)
{
  EFI_STATUS         Status = EFI_SUCCESS;
  UINT16             CmosAddress;
  UINT16             Count;
  UINT8              TabLen;
  UINT8              DefVal;
  struct CMOS_ENTRY  *TabPtr;


  // Traverse thro entire CMOS location and fill it with zero

  for( CmosAddress = CMOS_START_ADDR ; CmosAddress <= CMOS_END_ADDR ; CmosAddress++ ) {
    DefVal =0;
    WriteCmos ((UINT8)CmosAddress,&DefVal);
  }

  // Now fill only the CMOS location specified in the table
  // with default value from the table

  TabLen = ( sizeof(CMOS_TABLE) / sizeof(struct CMOS_ENTRY) );
  for ( Count = 0; Count < TabLen; Count++) {

    TabPtr = &CMOS_TABLE[Count];
    CmosAddress = (UINT8) (TabPtr->CmosAddress);
    DefVal = TabPtr->DefaultValue;
    WriteCmos ((UINT8)CmosAddress,&DefVal);
  }

  // Calculate and update the new checksum

  Status = UpdateCheckSum();
  return Status;
}


/**
  Returns the value from a CMOS location.

  If CMOSAddress is NULL, then return ERROR.
  If the passed address is beyond the max address return EFI_NOT_FOUND.
  If the function completed successfully return EFI_SUCCESS

  @param [in]      CMOSAddress    Location to read from CMOS
  @param [out]    Data    Contains the value read from the CMOS

  @return Status.

**/

EFI_STATUS
ReadCmos(
  IN  UINT8 CmosAddress,
  OUT UINT8 *Data
)
{

  EFI_STATUS  Status = EFI_SUCCESS;

  // Initial validation of the CMOS address

  if ( CmosAddress < CMOS_START_ADDR || CmosAddress > CMOS_END_ADDR || \
       CmosAddress == CMOS_CHECKSUM_ADDR_LOW || CmosAddress == CMOS_CHECKSUM_ADDR_HIGH ) {

   return EFI_NOT_FOUND;
  }

  if( CmosAddress >= CMOS_LOW_MEM_ST && CmosAddress <= CMOS_LOW_MEM_END ) {

    WriteIoPort  (PORT_70, CmosAddress);
    *Data = (UINT8)ReadIoPort  (PORT_71);

  } else if ( CmosAddress >= CMOS_HIGH_MEM_ST && CmosAddress <= CMOS_HIGH_MEM_END ) {

    WriteIoPort  (PORT_72, CmosAddress);
    *Data= (UINT8)ReadIoPort  (PORT_73);

  } else {
    return EFI_NOT_FOUND;
  }

return Status;
}


/**
  Write the value from a CMOS location.

  If CMOSAddress is NULL, then return ERROR.
  If the passed address is beyond the max address return EFI_NOT_FOUND.
  If the function completed successfully return EFI_SUCCESS

  @param  CMOSAddress  Location to write to CMOS
  @param  Data         Value to be written to the CMOS

**/

EFI_STATUS
WriteCmos (
  IN UINT8 CmosAddress,
  IN UINT8 *Data
)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT16      OldChkSum;
  UINT16      NewChkSum ;
  UINT8       OldCmosVal = 0 ;
  BOOLEAN     Attrib =  FALSE;


  if ( CmosAddress < CMOS_START_ADDR || CmosAddress > CMOS_END_ADDR || \
       CmosAddress == CMOS_CHECKSUM_ADDR_LOW || CmosAddress == CMOS_CHECKSUM_ADDR_HIGH ) {

   return EFI_NOT_FOUND;
  }

  // Read the actual value in the CMOS location to Write
  Attrib = CheckIfSkipChkSum(CmosAddress);

  if ( Attrib == FALSE ) {

    Status = ReadCmos(CmosAddress,(&OldCmosVal));
    if (Status != EFI_SUCCESS) {
      return Status;
    }
  }
  // Write data to CMOS address given

  if( CmosAddress >= CMOS_LOW_MEM_ST && CmosAddress <= CMOS_LOW_MEM_END ) {

    WriteIoPort  (PORT_70, CmosAddress);
    WriteIoPort  (PORT_71, *Data);

 } else if ( CmosAddress >= CMOS_HIGH_MEM_ST && CmosAddress <= CMOS_HIGH_MEM_END ) {

    WriteIoPort  (PORT_72, CmosAddress);
    WriteIoPort  (PORT_73, *Data);

  } else {
    return EFI_NOT_FOUND;
  }

  // Update the checksum optimization
  if ( Attrib == FALSE ) {
    OldChkSum = ReadCheckSum();
    NewChkSum = ( OldChkSum - (UINT16)OldCmosVal + (UINT16) (*Data));
    Status = WriteCheckSum(NewChkSum);
    if (Status != EFI_SUCCESS) {
      return Status;
    }
  }

return Status;
}


/**
  Read 32-bit value from a CMOS location.

  If ReadCmos returns error, return that error.
  If the function completed successfully return EFI_SUCCESS

  @param  CMOSAddress  Location to write to CMOS
  @param[out]  Data         Value read from the CMOS

**/
EFI_STATUS
ReadCmos32(
  IN  UINT8 CmosAddress,
  OUT UINT32 *Data
)
{
  UINT8 OutDataByte3, OutDataByte2, OutDataByte1, OutDataByte0;

  EFI_STATUS  Status = EFI_SUCCESS;

  Status = ReadCmos(CmosAddress, &OutDataByte0);
  Status |= ReadCmos(CmosAddress + 1, &OutDataByte1);
  Status |= ReadCmos(CmosAddress + 2, &OutDataByte2);
  Status |= ReadCmos(CmosAddress + 3, &OutDataByte3);

  *Data = (OutDataByte3 << 24) | (OutDataByte2 << 16) | (OutDataByte1 << 8) | (OutDataByte0);

  return Status;
}

/**
  Write 32-bit value to a CMOS location.

  If WriteCmos returns error, return that error.
  If the function completed successfully return EFI_SUCCESS

  @param  CMOSAddress  Location to write to CMOS
  @param[in]  Data         Value to be written to the CMOS

**/
EFI_STATUS
WriteCmos32 (
  IN UINT8 CmosAddress,
  IN UINT32 *Data
)
{
  UINT8 InDataByte3, InDataByte2, InDataByte1, InDataByte0;

  EFI_STATUS  Status = EFI_SUCCESS;

  InDataByte3 = (UINT8)( ((*Data) & 0xff000000) >> 24);
  InDataByte2 = (UINT8)( ((*Data) & 0x00ff0000) >> 16);
  InDataByte1 = (UINT8)( ((*Data) & 0x0000ff00) >> 8);
  InDataByte0 = (UINT8)( ((*Data) & 0x000000ff) >> 0);

  Status = WriteCmos(CmosAddress, &InDataByte0);
  Status |= WriteCmos(CmosAddress + 1, &InDataByte1);
  Status |= WriteCmos(CmosAddress + 2, &InDataByte2);
  Status |= WriteCmos(CmosAddress + 3, &InDataByte3);

  return Status;
}


/**
  Funtion to Initialize the CMOS.

  Checks the presence of CMOS battery, else it initialize CMOS to default.
  Perform a checksum computation and verify if the checksum is correct.
  If the input parameter ForceInit is TRUE, initialize all the CMOS
  location to their default values

  @param [in]      ForceInit  A boolean variable to initialize the CMOS to its default
                           without checking the RTC_PWR_STS or verifying the checksum.
  @param [out]    DefaultsRestored  A boolean variable to indicate if the defaults were restored

  @return    Status

**/

EFI_STATUS
InitCmos (
  IN BOOLEAN ForceInit,
  OUT BOOLEAN *DefaultsRestored OPTIONAL
){
  EFI_STATUS Status = EFI_SUCCESS;
  BOOLEAN FilledDefaults = FALSE;

  //
  // Check if ForceInit is True.
  //
  if (ForceInit == TRUE) {
    Status = CmosFillDefaults();
    FilledDefaults = TRUE;
  } else {
    //
    // Check if CMOS is BAD.
    //
    Status = VerifyCmosCheckSum();

    if (Status != EFI_SUCCESS) {
      Status = CmosFillDefaults();
      FilledDefaults = TRUE;
    }
  }

  if (DefaultsRestored != NULL) {
    *DefaultsRestored = FilledDefaults;
  }

  return Status;
}


/**
  Reads from the checksum address.

  Reads the 8-bit checksum from specified CheckSum address. The 8-bit read value is returned.
  If 8-bit I/O port operations are not supported, then ASSERT().

  @param  None

  @return The value read.

**/

UINT16
ReadCheckSum (
  VOID
)
{
  UINT16 ChkSumVal = 0;
  UINT8  LVal;
  UINT8  HVal ;
  UINT8  APort = PORT_72;
  UINT8  DPort = PORT_73;
  UINT8  TCmosAddress = CMOS_CHECKSUM_ADDR_HIGH;

   /* Check if Check sum address lies in the CMOS Lower memory or CMOS upper memory */

  if (CMOS_CHECKSUM_ADDR_LOW >= CMOS_LOW_MEM_ST && CMOS_CHECKSUM_ADDR_LOW < CMOS_LOW_MEM_END ) {
    APort = PORT_70;
    DPort = PORT_71;

  } else {
    APort = PORT_72;
    DPort = PORT_73;
  }


  TCmosAddress  = CMOS_CHECKSUM_ADDR_HIGH;
  WriteIoPort  (APort, TCmosAddress);
  HVal = ReadIoPort  (DPort);

  TCmosAddress = CMOS_CHECKSUM_ADDR_LOW;
  WriteIoPort  (APort, TCmosAddress);
  LVal = ReadIoPort  (DPort);

  ChkSumVal = HVal;
  ChkSumVal = ChkSumVal << 8;
  ChkSumVal = ChkSumVal + LVal;

return ChkSumVal;
}


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
)
{

  UINT8 LVal;
  UINT8 HVal ;
  UINT8 APort = PORT_72;
  UINT8 DPort = PORT_73;
  UINT8 TCmosAddress = CMOS_CHECKSUM_ADDR_HIGH;

  /* spliting the 16 bit checksum  */

  LVal = ( UINT8 )(ChecksumValue & 0x00FF);
  HVal = ( UINT8 )((ChecksumValue >> 8 ) & 0x00FF);

  /* Check if Check sum address lies in the CMOS Lower memory or CMOS upper memory */

  if (CMOS_CHECKSUM_ADDR_LOW >= CMOS_LOW_MEM_ST && CMOS_CHECKSUM_ADDR_LOW < CMOS_LOW_MEM_END ) {
    APort = PORT_70;
    DPort = PORT_71;
  } else {
    APort = PORT_72;
    DPort = PORT_73;
  }

  TCmosAddress = CMOS_CHECKSUM_ADDR_LOW ;
  WriteIoPort  (APort, TCmosAddress);
  WriteIoPort  (DPort, LVal);

  TCmosAddress = CMOS_CHECKSUM_ADDR_HIGH ;
  WriteIoPort  (APort, TCmosAddress);
  WriteIoPort  (DPort, HVal);


return EFI_SUCCESS;
}


/**
  Calculates and update the Checksum to appropriate address.

  Write the 8-bit checksum to specified CheckSum address.


  @param  None

  @return Status EFI_SUCCESS

**/

EFI_STATUS
UpdateCheckSum(
  VOID
)
{
  EFI_STATUS  Status;
  UINT16      NewChkSum;

  // Calculate the new checksum
  NewChkSum = CalculateCmosCheckSum();

  // Write the New checksum to the Checksum field
  Status = WriteCheckSum(NewChkSum);

return Status;
}


/**
  Check if the CMOS is corrupted by verifing the Checksum value .

  @param  None

  @return Status EFI_SUCCESS/EFI_CRC_ERROR

**/

EFI_STATUS
VerifyCmosCheckSum(
  VOID
)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT16      StoredChkSum = 0x0;
  UINT16      CurrentChkSum = 0xFF;

  StoredChkSum = (UINT16)ReadCheckSum();
  CurrentChkSum = (UINT16)CalculateCmosCheckSum();


  if ( CurrentChkSum == StoredChkSum ) {
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_CRC_ERROR;
  }

return Status;
}


/**
  Performs the calculation of Checksum .

  @param  None

  @return calculated CheckSum value

**/

UINT16
CalculateCmosCheckSum(
  VOID
)
{

  UINT16  ChkSumVal = 0;
  UINT16  CmosReg = 0;
  UINT8	  Data = 0;
  BOOLEAN Attrib = TRUE;


  for( CmosReg = CMOS_START_ADDR ; CmosReg <= CMOS_END_ADDR ; CmosReg ++ ) {

    Attrib = CheckIfSkipChkSum((UINT8)CmosReg );

    /*
       Skip these during checksum calculation.
       If the CMOS table Attribute is 1 and
       If the place of saving checksum
    */

    if ( Attrib == TRUE || CmosReg == CMOS_CHECKSUM_ADDR_LOW || CmosReg == CMOS_CHECKSUM_ADDR_HIGH ) {
      continue;
     }

    ReadCmos((UINT8)CmosReg, &Data);
    ChkSumVal= ChkSumVal + Data;

  }

return ChkSumVal;

}


/**
  Check if the CMOS location  has to be included for the Checksum calculation .

  @param [in]  CmosAddCnt Cmos address to be checked

  @return TRUE / FALSE

**/

BOOLEAN
CheckIfSkipChkSum(
  IN UINT8 CmosAddCnt
)
{

  UINT8              TabLen;
  UINT16             Count;
  struct CMOS_ENTRY  *TabPtr;

  // calculate the CMOS table length
  TabLen = ( sizeof(CMOS_TABLE)/sizeof(struct CMOS_ENTRY));

  for ( Count = 0; Count < TabLen; Count++) {

    TabPtr = &CMOS_TABLE[Count];

    if ( TabPtr->CmosAddress == CmosAddCnt ) {
      if ( TabPtr->Attribute & EXCLUDE_FROM_CHECKSUM ) {
        return TRUE;
      } else {
        return FALSE;
      }
    }
  }

return TRUE;
}


/**
  Wrapper for ReadCmos, provided for ease of use.

  return value read from Cmos

  @param  CMOSAddress  Cmos address to be read.

**/
UINT8
CmosRead(
  IN  UINT8 CmosAddress
)
{
  UINT8  ReadValue;
  EFI_STATUS Status = EFI_SUCCESS;

  Status = ReadCmos(CmosAddress, &ReadValue);

  if(Status != EFI_SUCCESS) {
    return 0;
  }

  return ReadValue;
}

/**
  Wrapper for WriteCmos, provided for ease of use.

  Write value to Cmos. Return nothing.

  @param  CMOSAddress  Location to write to CMOS
  @param  Data         Value to be written to the CMOS

**/
VOID
CmosWrite (
  IN UINT8 CmosAddress,
  IN UINT8 Data
)
{
  EFI_STATUS Status = EFI_SUCCESS;

  Status = WriteCmos(CmosAddress, &Data);
}


#endif
