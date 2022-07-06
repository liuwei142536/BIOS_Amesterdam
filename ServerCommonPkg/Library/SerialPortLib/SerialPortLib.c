/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed  
  for Intel CPUs and Chipsets under the terms of your license   
  agreement with Intel or your vendor.  This file may be        
  modified by the user, subject to additional terms of the      
  license agreement                                             
--*/
/** @file
  Serial I/O Port library functions with no library constructor/destructor

  Copyright (c) 2006 - 2010, Intel Corporation. All rights reserved.<BR>
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
#include <Library/PlatformHooksLib.h>
#include "Ns16550.h"


#define COM1_BASE 0x3f8


UINT16 gComBase  = 0x3f8;
UINTN  gBps      = 115200;
UINT8  gData     = 8;
UINT8  gStop     = 1;
UINT8  gParity   = 0;
UINT8  gBreakSet = 0;


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
  UINTN Divisor;
  UINT8 OutputData;
  UINT8 Data;

  InitializeSio();
  //
  // Some init is done by the platform status code initialization.
  //
  //
  // Map 5..8 to 0..3
  //
  Data = (UINT8) (gData - (UINT8) 5);

  //
  // Calculate divisor for baud generator
  //
  Divisor = 115200 / gBps;

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

