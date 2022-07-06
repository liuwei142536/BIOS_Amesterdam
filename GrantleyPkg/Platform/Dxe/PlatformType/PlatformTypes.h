//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license 
// agreement with Intel or your vendor.  This file may be      
// modified by the user, subject to additional terms of the    
// license agreement                                           
//
/*++

Copyright (c)  1999 - 2005 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
  PlatformTypes.h
  
Abstract:
  Platform Type Driver for Harwich.

--*/

#ifndef _PLATFORM_TYPES_H_
#define _PLATFORM_TYPES_H_

#include <PiDxe.h>
#include <Protocol/PlatformType.h>
#include <Library\DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library\HobLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol\Variable.h>
#include <Protocol\CpuIo.h>
#include <Protocol\Spi.h>
#include <Protocol\IioUds.h>

#include <Guid/HobList.h>
#include <Cpu\CpuRegs.h>
#include <PchRegs.h>
#include <Platform.h>


#define BLOCK_SIZE_32KB  				 0x8000
#define BLOCK_SIZE_64KB  				 0x10000
//AptioV server override start: WA to avoid redefinition errors when including Token.h
#ifndef  SPI_OPCODE_READ_INDEX
#define SPI_OPCODE_READ_INDEX            4
#endif
//AptioV server override end: WA to avoid redefinition errors when including Token.h
#define R_PCH_SPI_FREG4_PLATFORM_DATA (R_PCH_RCRB_SPI_BASE + 0x64)  // Flash Region 4(Platform Data)(32bits)
//AptioV server override start: WA to avoid redefinition errors when including Token.h
#ifndef  SB_RCBA
#define SB_RCBA             			 PCH_RCBA_ADDRESS 
#endif
//AptioV server override end: WA to avoid redefinition errors when including Token.h
#define VarMfgUuid           			 L"MfgSystemUuid"
#define EFI_PLATFORM_TYPE_DRIVER_PRIVATE_SIGNATURE  SIGNATURE_32 ('T', 'Y', 'P', 'P')
#define EFI_IIO_UDS_DRIVER_PRIVATE_SIGNATURE  SIGNATURE_32 ('S', 'D', 'U', 'I')

extern EFI_GUID gEfiMfgUUIDHobGuid;

typedef unsigned char BYTE;     //!<  8-bit quantities
typedef unsigned short WORD;    //!< 16-bit quantities
typedef unsigned long DWORD;    //!< 32-bit quantities

typedef enum
{
    SUCCESS = 0x00,             //!< Packet it good! .data[] is valid
    DEFER = 0x01,               //!< Packet is defered. .data[1] = BufID
    W_EARLY_NACK = 0x02,        //!< Packet mastered on the SMBus by the MCU was NACKed earlier than expected
    NOT_RESP = 0x03,            //!< Packet mastered on the SMBus by the MCU was NACKed during the address byte
    BUFFER_OVERRUN = 0x04,      //!< Too many BYTE s were stuffed into the buffer.
    NO_BUFFER = 0x05,           //!< All the buffers are used
    INVALID_BUF = 0x06,         //!< Command passed a buffer id that was not in range
    BUF_NOT_IN_QUEUE = 0x07,    //!< Command passed a buffer id is not being used.
    ARBITRATION_LOST = 0x08,    //!< While the MCU was mastering a packet on the SMBus it lost arbitration.
    TIMEOUT = 0x0B,             //!< SMBus timed out.
    CHECKSUM_ERR = 0x0C,        //!< Operation encountered a checksum mismatch
    DATA_NACK = 0x0D,           //!< Still don't know what these mean?
    BUS_ERR = 0x0E,             //!< ?
    FAIL = 0x0F,                //!< Generic error
    BUSY = 0x10,                //!< ?
    R_EARLY_NACK = 0x11,        //!< ?
    INVALID_LCD_COL_OFF = 0x12, //!< The cursor on the LCD was set to a column that was out of range.
    INVALID_LCD_ROW_OFF = 0x13, //!< The cursor on the LCD was set to a row that was out of range.
    INVALID_CK410_SEL = 0x14,   //!< ?
    CMD_NOT_SUPPORTED = 0x15,   //!< This command is not supported
    MORE_DATA_AVAILABLE = 0x16, //!< Do the command again to get more data
} STATUS;

typedef struct {
  BYTE byte_count; 
  STATUS status;
  BYTE data[31];
} BUFFER_RSLT;

VOID                              *mEfiVariableProtocolNotifyReg;
EFI_EVENT                         mEfiVariableProtocolEvent;

typedef struct {
  UINTN                               Signature;
  EFI_HANDLE                          Handle;               // Handle for protocol this driver installs on
  EFI_PLATFORM_TYPE_PROTOCOL          PlatformType;         // Policy protocol this driver installs
} EFI_PLATFORM_DATA_DRIVER_PRIVATE;

typedef struct {
  UINTN                               Signature;
  EFI_HANDLE                          Handle;         // Handle for protocol this driver installs on
  EFI_IIO_UDS_PROTOCOL                IioUds;         // Policy protocol this driver installs
} EFI_IIO_UDS_DRIVER_PRIVATE;

EFI_STATUS
GetPDRUUID (
  );

#endif
