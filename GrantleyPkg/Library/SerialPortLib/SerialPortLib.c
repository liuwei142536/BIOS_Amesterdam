/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed  
  for Intel CPUs and Chipsets under the terms of your license   
  agreement with Intel or your vendor.  This file may be        
  modified by the user, subject to additional terms of the      
  license agreement                                             
--*/
/** @file
  Serial I/O Port library functions with no library constructor/destructor

  Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation
  (if any) is furnished under a license and may only be used or
  copied in accordance with the terms of the license.  Except as
  permitted by such license, no part of this software or
  documentation may be reproduced, stored in a retrieval system, or
  transmitted in any form or by any means without the express written
  consent of Intel Corporation.

  Module Name:  SerialPortLib.c

**/

#include <Base.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>
#include <Library/BaseLib.h>
#include <IchRegs.h>
#include <Library/PciLib.h>
#include <SioRegs.h>
#include "Ns16550.h"

#define COM1_BASE 0x3f8

#ifdef GRANGEVILLE_FLAG
#define EFI_CPUID_VERSION_INFO                 0x1
#define CPU_BDX_DE_SERVER                      0x56  //AS CPUID-0x5066x
#define C0_REV_BDX                             0x02  //V0
#define BDX_DE_V1_DIVISOR                      4
#define BDX_DE_DIVISOR                         1
#define BDX_DE_V1_BAUDRATE						38400
#define BDX_DE_BAUDRATE							115200
#define GNV_OTHER		  BIT3
#endif

typedef struct {
  UINT8   Index;
  UINT8   Data;
} SIO_REG_TABLE;

STATIC  SIO_REG_TABLE  mPilotIIITable [] = {  
  { PILOTIII_LOGICAL_DEVICE,            PILOTIII_SIO_COM1 },
  { PILOTIII_BASE_ADDRESS_HIGH0,        (UINT8)(COM1_BASE >> 8) },
  { PILOTIII_BASE_ADDRESS_LOW0,         (UINT8)(COM1_BASE & 0xFF) },
  { PILOTIII_PRIMARY_INTERRUPT_SELECT,   0x04 },
  { PILOTIII_ACTIVATE,                   0x01}
};

STATIC  SIO_REG_TABLE  mPc8374Table [] = {  
  { PC8374_LOGICAL_DEVICE,        PC8374_SIO_COM1 },
  { PC8374_BASE_ADDRESS_HIGH0,    (UINT8)(COM1_BASE >> 8) },
  { PC8374_BASE_ADDRESS_LOW0,     (UINT8)(COM1_BASE & 0xFF) },
  { WAKEUP_ON_IRQ_EN,             0x04},
  { PC8374_ACTIVATE,              0x01},
  { PC8374_CLOCK_SELECT,          0},
  { PC8374_CLOCK_CONFIG,          0x80}
};

UINT16 gComBase  = 0x3f8;
UINTN  gBps      = 115200;
UINT8  gData     = 8;
UINT8  gStop     = 1;
UINT8  gParity   = 0;
UINT8  gBreakSet = 0;

UINT32 
IsSioExist (
  VOID
)
{
  //
  // IBMC will decode 0x4E/0x4F
  // 
  UINT32   SioExist;

#ifdef GRANGEVILLE_FLAG
  
  //For grangeville, SIO is not exist but  based on IsSIOExist status  only Legacy CR is loaded
  //we are installing the Console redirection. So we hacked the IsSIOExist funciton only for grangeville  instead of changing in AMI Restricted Code in LegacySredirec.c file
  // Check the LegacySredirEntryPoint in LegacySredir.c 

  SioExist= GNV_OTHER;
#else
  SioExist = 0;
#endif

  //
  //pilot 3 will decode 0x4E/0x4F
  // 
  IoWrite8 (PILOTIII_SIO_INDEX_PORT, PILOTIII_SIO_UNLOCK);
  IoWrite8 (PILOTIII_SIO_INDEX_PORT , PILOTIII_CHIP_ID_REG);
  if (IoRead8  (PILOTIII_SIO_DATA_PORT) == PILOTIII_CHIP_ID) {
  SioExist |= PILOTIII_EXIST;

  }
  IoWrite8 (PILOTIII_SIO_INDEX_PORT, PILOTIII_SIO_LOCK);

  //
  // PC SIO will decode 0x2E/0x2F
  //
  IoWrite8 (PC8374_SIO_INDEX_PORT, PC8374_LOGICAL_DEVICE);
  IoWrite8 (PC8374_SIO_DATA_PORT , PC8374_SIO_COM1);
  if (IoRead8  (PC8374_SIO_DATA_PORT) == PC8374_SIO_COM1) {
    SioExist |= PC8374_EXIST;
  }
  return SioExist;

  
}

VOID
InitializeSio (
  VOID
  )
{
    UINT32   Decode;
    UINT32   Enable;
    UINT32 SioExist=0;
    UINT32 SioEnable=0;
    UINT32 Index;

    //
    // Enable LPC decode on ICH9
    // Set COMA/COMB base
    //
    Decode =  (V_ICH_LPC_COMA_3F8 | V_ICH_LPC_COMB_2F8);
    Enable =  ( B_ICH_LPC_ENABLES_CNF2_EN | B_ICH_LPC_ENABLES_CNF1_EN  | B_ICH_LPC_ENABLES_KBC_EN \
              | B_ICH_LPC_ENABLES_FDD_EN  | B_ICH_LPC_ENABLES_LPT_EN   | B_ICH_LPC_ENABLES_COMB_EN \
              | B_ICH_LPC_ENABLES_COMA_EN);
    IoWrite32 (R_ICH_IOPORT_PCI_INDEX, (UINT32) (ICH_LPC_CF8_ADDR (R_ICH_LPC_IO_DEC)));
    IoWrite32 (R_ICH_IOPORT_PCI_DATA, Decode | (Enable << 16));
	
    SioExist = IsSioExist ();

    if ((SioExist & (PILOTIII_EXIST | PC8374_EXIST)) == (PILOTIII_EXIST | PC8374_EXIST) ) {
      //
      // Both are there, we use DEFAULT_SIO as debug port anyway
      //
      if (DEFAULT_SIO == PILOTIII_EXIST) {
        SioEnable = PILOTIII_EXIST;
      } else {
        SioEnable = PC8374_EXIST;
      }
    } else {
      SioEnable = SioExist;
    }

    if (SioEnable == PILOTIII_EXIST) {
      IoWrite8 (PILOTIII_SIO_INDEX_PORT, PILOTIII_SIO_UNLOCK);
      for (Index = 0; Index < sizeof(mPilotIIITable)/sizeof(SIO_REG_TABLE); Index++) {
        IoWrite8 (PILOTIII_SIO_INDEX_PORT, mPilotIIITable[Index].Index);
        IoWrite8 (PILOTIII_SIO_DATA_PORT, mPilotIIITable[Index].Data);
      }
     IoWrite8 (PILOTIII_SIO_INDEX_PORT, PILOTIII_SIO_LOCK);
    }

    if (SioEnable == PC8374_EXIST) {
      for (Index = 0; Index < sizeof(mPc8374Table)/sizeof(SIO_REG_TABLE); Index++) {
        IoWrite8 (PC8374_SIO_INDEX_PORT, mPc8374Table[Index].Index);
        IoWrite8 (PC8374_SIO_DATA_PORT,  mPc8374Table[Index].Data);
      }
    }
}

#ifdef GRANGEVILLE_FLAG

VOID
GetCpuInfo (
  UINT8     *CpuType,
  UINT8     *CpuStepping
  )
/*++

Routine Description:
   Returns the Model ID of the CPU. 
   Model ID = EAX[7:4] 

Returns:

--*/

{
  UINT32                    RegEax =0;
  UINT32 					FamilyId =0;
  UINT32 					ExtendedFamilyId =0;
  UINT32 					ModelId =0;
  UINT32 					ExtendedModelId = 0;

  AsmCpuid (EFI_CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);

  *CpuStepping = (UINT8)(RegEax & 0x0F);

  
  	  //
	  // The Extended Family ID needs to be examined only when the Family ID is 0FH.
	  // If Family ID is 0FH, Family ID = Family ID + Extended Family ID.
	  //
	  FamilyId = BitFieldRead32 (RegEax, 8, 11);
	  if (FamilyId == 0x0f) {
		  ExtendedFamilyId = BitFieldRead32 (RegEax, 20, 27);
		  //FamilyId += ExtendedFamilyId;
	  }
	  
	  //
	  // The Extended Model ID needs to be examined only when the Family ID is 06H or 0FH.
	  // If Family ID is 06H or 0FH, Model ID = Model ID + (Extended Model ID << 4).
	  //BDX-model if is 0x56
	  //
	  ModelId = BitFieldRead32 (RegEax, 4, 7);
	  if (FamilyId == 0x06 || FamilyId == 0x0f) {
		  ExtendedModelId = BitFieldRead32 (RegEax, 16, 19);
		  ModelId += (ExtendedModelId << 4);
	  }
	  FamilyId += ExtendedFamilyId;
      *CpuType     = (UINT8)ModelId;
}



UINT64 GetBaudrate()
{
	UINT8	  CpuType;
	UINT8	  CpuStepping;

	
	  GetCpuInfo(&CpuType,&CpuStepping);
	
	if( ( CpuType == CPU_BDX_DE_SERVER) && (CpuStepping  == C0_REV_BDX))
	  return BDX_DE_V1_BAUDRATE;
	else
	  return BDX_DE_BAUDRATE;// = 153600/gBps;
}

#endif


RETURN_STATUS
EFIAPI
SerialPortInitialize (
  VOID
  )
/*++

Routine Description:

  Initialize Serial Port

    The Baud Rate Divisor registers are programmed and the LCR
    is used to configure the communications format. Hard coded
    UART config comes from globals in DebugSerialPlatform lib.

Arguments:

  None

Returns:

  None

--*/
{


  UINTN Divisor;//,CapID;
  UINT8 OutputData;
  UINT8 Data;//,dummy;
  
#ifdef GRANGEVILLE_FLAG
  UINT8     CpuType;
  UINT8     CpuStepping;
  
  
#endif

  InitializeSio();
  //
  // Some init is done by the platform status code initialization.
  //
  //
  // Map 5..8 to 0..3
  //
  Data = (UINT8) (gData - (UINT8) 5);

#ifdef GRANGEVILLE_FLAG
  //
  // Calculate divisor for baud generator
  //
	  GetCpuInfo(&CpuType,&CpuStepping);

    	if( ( CpuType == CPU_BDX_DE_SERVER) && (CpuStepping == C0_REV_BDX))
    	  Divisor = BDX_DE_V1_DIVISOR;
    	else
    	  Divisor = BDX_DE_DIVISOR;//153600/gBps;

#else
      Divisor =115200/gBps;
#endif

  //
  // Set communications format
  //
  OutputData = (UINT8) ((DLAB << 7) | ((gBreakSet << 6) | ((gParity << 3) | ((gStop << 2) | Data))));
  IoWrite8 (gComBase + LCR_OFFSET, OutputData);

  //
  // Configure baud rate
  //
  IoWrite8 (gComBase + BAUD_HIGH_OFFSET, (UINT8) (Divisor >> 8));
  IoWrite8 (gComBase + BAUD_LOW_OFFSET, (UINT8) (Divisor & 0xff));

  //
  // Switch back to bank 0
  //
  OutputData = (UINT8) ((~DLAB << 7) | ((gBreakSet << 6) | ((gParity << 3) | ((gStop << 2) | Data))));
  IoWrite8 (gComBase + LCR_OFFSET, OutputData);

  return RETURN_SUCCESS;
}

/**
  Write data to serial device.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is zero, then return 0.

  @param  Buffer           Point of data buffer which need to be writed.
  @param  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval 0                Write data failed.
  @retval !0               Actual number of bytes writed to serial device.

**/
UINTN
EFIAPI
SerialPortWrite (
  IN UINT8     *Buffer,
  IN UINTN     NumberOfBytes
)
{
  UINTN Result;
  UINT8 Data;

  if (NULL == Buffer) {
    return 0;
  }

  Result = NumberOfBytes;

  while (NumberOfBytes--) {
    //
    // Wait for the serail port to be ready.
    //
    do {
      Data = IoRead8 ((UINT16) PcdGet64 (PcdSerialRegisterBase) + LSR_OFFSET);
    } while ((Data & LSR_TXRDY) == 0);
    IoWrite8 ((UINT16) PcdGet64 (PcdSerialRegisterBase), *Buffer++);
  }

  return Result;
}


/*
  Read data from serial device and save the datas in buffer.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is zero, then return 0.

  @param  Buffer           Point of data buffer which need to be writed.
  @param  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval 0                Read data failed.
  @retval !0               Actual number of bytes raed to serial device.

**/
UINTN
EFIAPI
SerialPortRead (
  OUT UINT8     *Buffer,
  IN  UINTN     NumberOfBytes
)
{
  UINTN Result;
  UINT8 Data;

  if (NULL == Buffer) {
    return 0;
  }

  Result = NumberOfBytes;

  while (NumberOfBytes--) {
    //
    // Wait for the serail port to be ready.
    //
    do {
      Data = IoRead8 ((UINT16) PcdGet64 (PcdSerialRegisterBase) + LSR_OFFSET);
    } while ((Data & LSR_RXDA) == 0);

    *Buffer++ = IoRead8 ((UINT16) PcdGet64 (PcdSerialRegisterBase));
  }

  return Result;
}

