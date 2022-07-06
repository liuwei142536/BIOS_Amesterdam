/*++
 Copyright (c) 2009 Nationz Technologies Inc. All rights reserved

Module Name:

  TisPc.h

Abstract:

  TIS (TCM Interface Specification) functions

   Product:    TCM DXE Driver for Nationz V1.04  
   Author:     Nosa

   $Revision:  01.04.11.09 
   $Date:      12/14/2009

--*/
#ifndef TIS_PC_H
#define TIS_PC_H

#define _CRT_SECURE_NO_WARNINGS
#include <Uefi.h>
#include <AmiTcg/Tcm.h>

//#define TCMINTERNALAPI              __stdcall
//#define TCMINTERNALAPIV             __cdecl

typedef EFI_HANDLE      TIS_TCM_HANDLE;

#pragma pack (push)
#pragma pack (1)


//
// Define bits of ACCESS and STATUS registers
//
#define _BIT_OF_BYTE(n)             ((INT8) (1u << (n)))

#define TIS_PC_VALID                _BIT_OF_BYTE (7)

#define TIS_PC_ACC_ACTIVE           _BIT_OF_BYTE (5)
#define TIS_PC_ACC_SEIZED           _BIT_OF_BYTE (4)
#define TIS_PC_ACC_SEIZE            _BIT_OF_BYTE (3)
#define TIS_PC_ACC_PENDIND          _BIT_OF_BYTE (2)
#define TIS_PC_ACC_RQUUSE           _BIT_OF_BYTE (1)
#define TIS_PC_ACC_ESTABLISH        _BIT_OF_BYTE (0)

#define TIS_PC_STS_READY            _BIT_OF_BYTE (6)
#define TIS_PC_STS_GO               _BIT_OF_BYTE (5)
#define TIS_PC_STS_DATA             _BIT_OF_BYTE (4)
#define TIS_PC_STS_EXPECT           _BIT_OF_BYTE (3)
#define TIS_PC_STS_RETRY            _BIT_OF_BYTE (1)

#define TIS_TIMEOUT(reg,wait)       ((wait) && ((INT8)(reg)) >= 0)

#define TCM_H2NS(x) \
  (((UINT16)(x) << 8) | ((UINT16)(x) >> 8))

#define TCM_H2NL(x) \
  (UINT32)((((UINT32)(x)) >> 24) |  \
           (((x) >> 8) & 0xff00) |  \
           ((x) << 24) |  \
           (((x) & 0xff00) << 8))

#define TCM_N2HS(x)                 TCM_H2NS(x)
#define TCM_N2HL(x)                 TCM_H2NL(x)

//
// Bits affected by the TIS_PC_VALID in ACCESS register
//
#define TIS_PC_ACC_MASK             \
  (TIS_PC_ACC_ACTIVE | TIS_PC_ACC_SEIZED |  \
   TIS_PC_ACC_PENDIND | TIS_PC_ACC_ESTABLISH)

//
// Bits affected by the TIS_PC_VALID in STATUS register
//
#define TIS_PC_STS_MASK             (TIS_PC_STS_DATA | TIS_PC_STS_EXPECT)

//
// Restore original structure alignment
//
typedef union td_TCM_VARIANT {
  UINT8                             b;
  UINT16                            w;
  UINT32                            d;
  UINT64                            q;
} _TCM_VARIANT;

#pragma pack (pop)

#if 0
//
// Initialize TCM Procedure.
//
typedef EFI_STATUS (EFIAPI *MP_INITTCM) (IN TIS_PC_REGISTERS_PTR TisReg);

//
// Close TCM Procedure.
//
typedef EFI_STATUS (EFIAPI *MP_CLOSETCM) (IN TIS_PC_REGISTERS_PTR TisReg);

//
// Get TCM info. Procedure.
//
typedef EFI_STATUS (EFIAPI *MP_GETTCMINFO) (IN TIS_PC_REGISTERS_PTR TisReg);

//
// TCM transmit Procedure.
//
typedef EFI_STATUS (EFIAPI *MP_TCMTRANSMIT) (
  IN      TIS_TCM_HANDLE            TisReg,
  IN      const UINT8               *Fmt,
  ...
  );

typedef struct _EFI_TCMMP_PROTOCOL {
    MP_INITTCM      mpInitTCM;
    MP_CLOSETCM     mpCloseTCM;
    MP_GETTCMINFO   mpGetTCMStatusInfo;
    MP_TCMTRANSMIT  TcmPerform;
} EFI_TCMMP_PROTOCOL;
 

EFI_STATUS
mpInitTCM(
	IN      TCM_PC_REGISTERS_PTR      TisReg
);

EFI_STATUS
mpCloseTCM(
	IN      TCM_PC_REGISTERS_PTR      TisReg
);

EFI_STATUS
mpGetTCMStatusInfo(
	IN      TCM_PC_REGISTERS_PTR      TisReg
);
#endif

EFI_STATUS
EFIAPI
TcmLibPassThrough (
  IN    TCM_PC_REGISTERS_PTR    TisReg,
  IN    UINT8                   *TcmInputParameterBlock,
  IN    UINT32                  TcmInputParameterBlockSize,
  IN    UINT8                   *TcmOutputParameterBlock,
  IN    UINT32                  TcmOutputParameterBlockSize
);


/*++                                                                                                                          
Routine Description:                                                
  Get some delay
Arguments:                
  None                 
Returns:                                                            
  None                                               
--*/
extern VOID IODelay (VOID);

//el2015 >>>
#define TPM_RESPONSE_HEADER_SIZE 10
#define MAX_BUFFER_LEN	4096

/* TPM values */
#define TPM_BASE           0xfed40000
#define TPM_LOCALITY_SIZE  0x00001000
#define TPM_LOCALITY_COUNT 5

/* per-locality registers (locality-relative offset) */
#define TPM_REG_ACCESS     0x00000000
#define TPM_REG_INT_ENABLE 0x00000008
#define TPM_REG_INT_VECTOR 0x0000000c
#define TPM_REG_INT_STATUS 0x00000010
#define TPM_REG_INT_CAP    0x00000014
#define TPM_REG_STATUS     0x00000018
#define TPM_REG_BURST      0x00000019
#define TPM_REG_DATA       0x00000024
#define TPM_REG_VID_L      0x00000f00
#define TPM_REG_VID_H      0x00000f01
#define TPM_REG_DID_L      0x00000f02
#define TPM_REG_DID_H      0x00000f03
#define TPM_REG_RID        0x00000f04

/* locality#4 specific registers (locality#4-absolute offset) */
#define TPM_REG_HASH_END   0x00004020
#define TPM_REG_HASH_DATA  0x00004024
#define TPM_REG_HASH_START 0x00004028

/* access register bit masks */
#define TPM_ACCESS_VALID     0x80 /* Register is valid (R) */
#define TPM_ACCESS_RESERVED  0x40
#define TPM_ACCESS_ACTIVE    0x20 /* Locality active (R/W) */
#define TPM_ACCESS_SEIZED    0x10 /* Some locality has seized control (R/W) */
#define TPM_ACCESS_SEIZE     0x08 /* Seize control (W) */
#define TPM_ACCESS_REQUESTED 0x04 /* Some locality has requested control (R) */
#define TPM_ACCESS_REQUEST   0x02 /* Request control (R/W) */
#define TPM_ACCESS_EST       0x01 /* Secure environment pending (R) */
#define TPM_ACCESS_MASK      0x3e /* active, seize(d), request(ed) */

/* status register bit masks */
#define TPM_STATUS_VALID     0x80 /* AVAIL and EXPECT bit are correct (R) */
#define TPM_STATUS_READY     0x40 /* Ready for a new command (R/W) */
#define TPM_STATUS_GO        0x20 /* Execute command (W) */
#define TPM_STATUS_AVAIL     0x10 /* Response data available (R) */
#define TPM_STATUS_EXPECT    0x08 /* TPM expects more data (R) */
#define TPM_STATUS_RESERVED1 0x04
#define TPM_STATUS_RETRY     0x02 /* Resend response (W) */
#define TPM_STATUS_RESERVED2 0x01
#define TPM_STATUS_MASK      0x7a /* ready, go, avail, expect, retry */

#define TIS_TIMEOUT_A               50000  /* 750ms */
#define TIS_TIMEOUT_B               50000 /* 2s */
#define TIS_TIMEOUT_C               50000  /* 750ms */
#define TIS_TIMEOUT_D               50000  /* 750ms */

#define TPM_ACCESS_ADDR(locality)		((UINTN)TPM_BASE + (UINTN)(locality * TPM_LOCALITY_SIZE) + TPM_REG_ACCESS)
#define TPM_STATUS_ADDR(locality)		((UINTN)TPM_BASE + (UINTN)(locality * TPM_LOCALITY_SIZE) + TPM_REG_STATUS)
#define TPM_BURST_ADDR(locality)		((UINTN)TPM_BASE + (UINTN)(locality * TPM_LOCALITY_SIZE) + TPM_REG_BURST)
#define TPM_VID_L_ADDR(locality)		((UINTN)TPM_BASE + (UINTN)(locality * TPM_LOCALITY_SIZE) + TPM_REG_VID_L)
#define TPM_VID_H_ADDR(locality)		((UINTN)TPM_BASE + (UINTN)(locality * TPM_LOCALITY_SIZE) + TPM_REG_VID_H)
#define TPM_DID_L_ADDR(locality)		((UINTN)TPM_BASE + (UINTN)(locality * TPM_LOCALITY_SIZE) + TPM_REG_DID_L)
#define TPM_DID_H_ADDR(locality)		((UINTN)TPM_BASE + (UINTN)(locality * TPM_LOCALITY_SIZE) + TPM_REG_DID_H)
#define TPM_DATA_ADDR(locality)			((UINTN)TPM_BASE + (UINTN)(locality * TPM_LOCALITY_SIZE) + TPM_REG_DATA)

/**
**/
typedef enum TIS_STATUS
{
    TIS_SUCCESS = 0,
    TIS_TIMEOUT = 1,
    TIS_INVALID_PARAMETER = 2,
    TIS_NOT_FOUND = 3,
    TIS_SEND_BUFFER_TOO_SMALL = 4,
    TIS_RECEIVE_BUFFER_TOO_SMALL = 5,
    TIS_LOCALITY_NOT_ENABLE = 6
} TIS_STATUS;


TIS_STATUS TisTpmCommand (UINT8 locality, UINT8 *BufferIn, UINT32 SizeIn, UINT8 *BufferOut, UINT32 *SizeOut);
//el2015 <<<

#endif
