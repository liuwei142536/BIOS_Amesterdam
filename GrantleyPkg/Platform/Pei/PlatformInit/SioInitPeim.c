/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
  
    SioInitPeim.c
   
Abstract:

    Functions for LpcSio initilization
    
--*/

#include "PlatformEarlyInit.h"


STATIC SIO_INDEX_DATA AST2400Table[] = {
  //
  // Init UART1
  //

  {
    AST2400_LOGICAL_DEVICE,
    0x02
  },
  {
    AST2400_ACTIVATE,
    0x01
  },
  {
    AST2400_BASE_ADDRESS_HIGH0,
    0x2
  },
  {
    AST2400_BASE_ADDRESS_LOW0,
    (UINT8) (0x2F8 & 0x00ff)
  }
};



STATIC SIO_INDEX_DATA mSioInitTable[] = {
  //
  // Init GPIO
  //
  {
    PILOTIII_LOGICAL_DEVICE,
    PILOTIII_SIO_GPIO
  },
  {
    PILOTIII_ACTIVATE,
    0x01
  },
  {
    PILOTIII_BASE_ADDRESS_HIGH0,
    (UINT8) ((UINT16) SIO_GPIO_BASE_ADDRESS >> 8)
  },
  {
    PILOTIII_BASE_ADDRESS_LOW0,
    (UINT8) (SIO_GPIO_BASE_ADDRESS & 0x00ff)
  }

  //
  //IPMI_START
  //
  //
  // Init KCS4 for SMS
  //
  ,
  {
    PILOTIII_LOGICAL_DEVICE,
    PILOTIII_SIO_KCS4
  },
  {
    PILOTIII_ACTIVATE,
    0x01
  },
  {
    PILOTIII_BASE_ADDRESS_HIGH0,
    (UINT8) ((UINT16) PILOTIII_KCS4_DATA_BASE_ADDRESS >> 8)
  },
  {
    PILOTIII_BASE_ADDRESS_LOW0,
    (UINT8) (PILOTIII_KCS4_DATA_BASE_ADDRESS & 0x00ff)
  },
  {
    PILOTIII_BASE_ADDRESS_HIGH1,
    (UINT8) ((UINT16) PILOTIII_KCS4_DATA_BASE_ADDRESS >> 8)
  },
  {
    PILOTIII_BASE_ADDRESS_LOW1,
    (UINT8) ((PILOTIII_KCS4_DATA_BASE_ADDRESS + 1) & 0x00ff)
  },

  //
  // Init KCS3 for SMM
  //
  {
    PILOTIII_LOGICAL_DEVICE,
    PILOTIII_SIO_KCS3
  },
  {
    PILOTIII_ACTIVATE,
    0x01
  },
  {
    PILOTIII_BASE_ADDRESS_HIGH0,
    (UINT8) ((UINT16) PILOTIII_KCS3_DATA_BASE_ADDRESS >> 8)
  },
  {
    PILOTIII_BASE_ADDRESS_LOW0,
    (UINT8) (PILOTIII_KCS3_DATA_BASE_ADDRESS & 0x00ff)
  },
  {
    PILOTIII_BASE_ADDRESS_HIGH1,
    (UINT8) ((UINT16) PILOTIII_KCS3_DATA_BASE_ADDRESS >> 8)
  },
  {
    PILOTIII_BASE_ADDRESS_LOW1,
    (UINT8) ((PILOTIII_KCS3_DATA_BASE_ADDRESS +1) & 0x00ff)
  }

  //
  //IPMI_END
  //
};

STATIC SIO_INDEX_DATA  mW83527Table [] = {  
  //
  // Enter Configuration Mode
  //
  { W83527_SIO_INDEX_PORT, 0x87 },
  { W83527_SIO_INDEX_PORT, 0x87 },
  //
  // GPIO Config
  //
  { W83527_SIO_INDEX_PORT, W83527_LOGICAL_DEVICE },       // 0x7
  { W83527_SIO_DATA_PORT,  W83527_SIO_GPIO },             // 0x9
  //
  // Disable  GPIO 2 to make Keyboard work
  //
  { W83527_SIO_INDEX_PORT, W83527_ACTIVATE_REG }, //CR 0x60
  { W83527_SIO_DATA_PORT,  0x0 },                 // 0x00
  //
  // Exit Configuration Mode
  //
  { W83527_SIO_INDEX_PORT, 0xAA }
};


VOID
LpcSioEarlyInit (
  VOID
  )
/*++

Routine Description:

  Initialize the GPIO IO selection, GPIO USE selection, and GPIO signal inversion registers.

Arguments:

  PeiServices  -  PeiService point.
  CpuIo        -  CpuIo PPI to read/write IO ports.

Returns: 

  EFI_SUCCESS  -  Init succeed.

--*/
{
  UINT8 Index;
  //
  //IPMI_START
  //
  IoWrite8 (
      PILOTIII_SIO_INDEX_PORT,
      0x5A
      );   
  //
  // Do early SIO GPIO init
  //
  for (Index = 0; Index < sizeof (mSioInitTable) / sizeof (SIO_INDEX_DATA); Index++) {
    IoWrite8 (
      PILOTIII_SIO_INDEX_PORT,
      mSioInitTable[Index].Index
      );

    IoWrite8 (
      PILOTIII_SIO_DATA_PORT,
      mSioInitTable[Index].Value
      );
  }
  
  IoWrite8 (
      PILOTIII_SIO_INDEX_PORT,
      0xA5
      );   
  //
  //IPMI_END
  //
}


VOID
AST2400EarlyInit (
  VOID
  )
  {
  	 UINT8 Index;
  //	 UINT8 Value;
  	 //unlock
  	 IoWrite8 (
      AST2400_SIO_INDEX_PORT,
      0xA5
      );   
     IoWrite8 (
      AST2400_SIO_INDEX_PORT,
      0xA5
      );       
  	 
  	 //Init SUART1(logical Device-2)
  	 for (Index = 0; Index < sizeof (AST2400Table) / sizeof (SIO_INDEX_DATA); Index++) {
  
     IoWrite8 (
      AST2400_SIO_INDEX_PORT,
      AST2400Table[Index].Index
      );

    IoWrite8 (
      AST2400_SIO_DATA_PORT,
      AST2400Table[Index].Value
      );   
     }
     
     IoWrite8 (
      AST2400_SIO_INDEX_PORT,
      0x60
      );

     //exit
      IoWrite8 (
      AST2400_SIO_INDEX_PORT,
      0xAA
      );  
      
  }


VOID
W83527SioEarlyInit (
  VOID
  )
/*++

Routine Description:

  Initialize the GPIO IO selection, GPIO USE selection, and GPIO signal inversion registers.

Arguments:

  PeiServices  -  PeiService point.
  CpuIo        -  CpuIo PPI to read/write IO ports.

Returns: 

  EFI_SUCCESS  -  Init succeed.

--*/
{
  UINT8 Index;
  //
  // Do early SIO GPIO init
  //
  for (Index = 0; Index < sizeof (mW83527Table) / sizeof (SIO_INDEX_DATA); Index++) {
    IoWrite8 (
      mW83527Table[Index].Index,
      mW83527Table[Index].Value
      );
  }
}
