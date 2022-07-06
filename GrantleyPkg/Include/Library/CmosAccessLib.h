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

  CmosAccess.h

Abstract:

--*/

#ifndef _CMOS_ACCESS_
#define _CMOS_ACCESS_


/**
  Returns the value from a CMOS location. 

  If CMOSAddress is NULL, then return ERROR.
  If the passed address is beyond the max address return EFI_NOT_FOUND.
  If the function completed successfully return EFI_SUCCESS
  
  @param [in]    CMOSAddress  Location to read from CMOS
  @param [out]   Data         The Value read from CMOS location

  @return Status.

**/


EFI_STATUS 
ReadCmos(
  IN UINT8 CMOSAddress,
  OUT UINT8 *Data
  );
 
 
/**
  Write the value from a CMOS location. 

  If CMOSAddress is NULL, then return ERROR.
  If the passed address is beyond the max address return EFI_NOT_FOUND.
  If the function completed successfully return EFI_SUCCESS
  
  @param [in]  CMOSAddress  Location to write to CMOS
  @param [in]  Data         Value to be written to the CMOS

**/


EFI_STATUS 
WriteCmos( 
  IN UINT8 CMOSAddress, 
  IN UINT8 *Data
);

 
/**
  Read 32-bit value from a CMOS location. 

  If ReadCmos returns error, return that error.
  If the function completed successfully return EFI_SUCCESS
  
  @param  CMOSAddress  Location to write to CMOS
  @param[out]  Data         Value read from the CMOS

**/
EFI_STATUS 
ReadCmos32(
  IN UINT8 CmosAddress,
  OUT UINT32 *Data
);
 
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
);

 
/**
  Funtion to Initialize the CMOS.

  Checks the presence of CMOS battery, else it initialize CMOS to default.
  Perform a checksum computation and verify if the checksum is correct.
  If the input parameter ForceInit is TRUE, initialize all the CMOS 
  location to their default values 

  @param [in]  ForceInit  A boolean variable to initialize the CMOS to its default
                          without checking the RTC_PWR_STS or verifying the checksum.

  @return [out]  DefaultsRestored  A boolean variable to indicate if the defaults were restored   

**/


EFI_STATUS 
InitCmos(
  IN BOOLEAN ForceInit, 
  OUT BOOLEAN *DefaultsRestored OPTIONAL
);


/**
  Wrapper for ReadCmos, provided for ease of use.

  return value read from Cmos
  
  @param  CMOSAddress  Cmos address to be read.
  
**/
UINT8 
CmosRead(
  IN  UINT8 CmosAddress
);


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
);


#endif // _CMOS_ACCESS_
