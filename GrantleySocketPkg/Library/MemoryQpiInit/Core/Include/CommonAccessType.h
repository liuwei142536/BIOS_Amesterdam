//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/** @file
  Unified Silicon Register Access Types

  Copyright (c) 2011 - 2015, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.
**/

#ifndef __COMMON_HW_ACCESS_TYPE_H__
#define __COMMON_HW_ACCESS_TYPE_H__
#include "DataTypes.h"

//////////////////////////////////////////////////////////////////////////
//
// CSR Boxes Definitions
//
//////////////////////////////////////////////////////////////////////////

typedef enum {
  CpuCsrCbox = 0,
  CpuCsrSbox = 1,
  CpuCsrBbox = 2,
  CpuCsrZbox = 3,
  CpuCsrRbox = 4,
  CpuCsrDbox = 5,
  CpuCsrTbox = 6,
  CpuCsrUbox = 7,
  CpuCsrWbox = 8,
  CpuCsrKbox = 9,
  CpuCsrPbox = 10,
  CpuCsrJbox = 11,
  CpuCsrBoxMaximum
} CPUCSR_BOX;


//////////////////////////////////////////////////////////////////////////
//
// CSR Ports of Boxes Definitions
//
//////////////////////////////////////////////////////////////////////////
typedef enum {
  PORT_NA= 0,
  PORT0  = 0,
  PORT1  = 1,
  PORT2  = 2,
  PORT3  = 3,
  PORT4  = 4,
  PORT5  = 5,
  PORT6  = 6,
  PORT7  = 7,
  PORT8  = 8,
  PORT9  = 9,
  PORT10 = 10,
  PORT11 = 11,
  PortMaximum
} CPUCSR_BOX_PORT;
//////////////////////////////////////////////////////////////////////////
//
// CSR Access Definitions
//
//////////////////////////////////////////////////////////////////////////

typedef enum {
  AddrTypePCIE    = 0,
  AddrTypeCSR     = 1,
  AddrTypeCSREX   = 2,
  AddrTypeMMIO    = 3,
  AddrTypeMMIO64  = 4,
  AddrTypeIO      = 5,
  AddrTypeMaximum
} USRA_ADDR_TYPE;

typedef enum {
  ComAccessWidth8  = 0,
  ComAccessWidth16 = 1,
  ComAccessWidth32 = 2,
  ComAccessWidth64 = 3,
  ComAccessWidthMaximum
} USRA_ACCESS_WIDTH;

#define USRA_ENABLE 1;
#define USRA_DISABLE 0;

#pragma pack (1)

typedef struct 
  {
    UINT32              RawData32[2];       // RawData of two UINT32 type, place holder
    UINT32              AddrType:8;         // Address type: CSR, PCIE, MMIO, IO, SMBus ... 
    UINT32              AccessWidth:4;      // The Access width for 8, 16,32,64 -bit access
    UINT32              FastBootEn:1;       // Fast Boot Flag, can be used to log register access trace for fast boot
    UINT32              S3Enable:1;         // S3 Enable bit, when enabled, it will save the write to script to support S3
    UINT32              HptrType:1;         // Host Pointer type, below or above 4GB
    UINT32              ConvertedType:1;    // The address type was from converted type, use this field for address migration support
    UINT32              RFU3:16;            // Reserved for User use or Future Use

    UINT32              HostPtr:32;         // The Host Pointer, to point to Attribute buffer etc.
} ADDR_ATTRIBUTE_TYPE;

typedef struct 
  {
    UINT32              Offset:12;          // The PCIE Register Offset 
    UINT32              Func:3;             // The PCIE Function
    UINT32              Dev:5;              // The PCIE Device
    UINT32              Bus:8;              // The PCIE Bus
    UINT32              RFU1:4;             // Reserved for User use or Future Ues

    UINT32              Seg:16;             // The PCI Segment
    UINT32              RFU2:16;            // Reserved for User use or Future Ues

} COMMON_PCIE_ADDR_TYPE;

typedef struct 
  {
    UINT32              Offset:12;          // The CSR_OFFSET.offset 
    UINT32              Size:3;             // The CSR_OFFSET.size
    UINT32              Pseudo:1;           // The CSR_OFFSET.pseudo
    UINT32              FuncBlk:8;          // The CSR_OFFSET.funcblk
    UINT32              BoxType:8;          // For CSR_OFFSET.boxtype
    

    UINT32              BoxInst:8;          // The Box Instance, 0 based, Index/Port within the box, Set Index as 0 if the box has only one instances
    UINT32              SocketId:8;         // The socket Id
    UINT32              RFU:16;             // Reserved for User use or Future Ues

} COMMON_CSR_ADDR_TYPE;

typedef struct 
  {
    UINT32              Offset:16;          // The CSR Offset 
    UINT32              FuncBlk:8;          // The Functional Blocks
    UINT32              BoxInst:8;          // The Box Instance, 0 based, Index/Port within the box, Set Index as 0 if the box has only one instances

    UINT32              BoxType:8;          // For Box Type, this stands for different Box Type as defined in enum
    UINT32              SocketId:8;         // The socket Id
    UINT32              RFU:16;             // Reserved for User use or Future Ues

} COMMON_CSR_IPF_ADDR_TYPE;


typedef struct 
  {
    UINT32              Offset:32;          // The MMIO Offset

    UINT32              RFU1:8;             // Reserved for User use or Future Ues
    UINT32              RFU2:24;            // Reserved for User use or Future Ues

} COMMON_MMIO_ADDR_TYPE;

typedef struct 
  {
    UINT32              Offset:32;          // The MMIO Offset Lower 32-bit

    UINT32              OffsetH:32;         // The MMIO Offset Higher 32-bit
    UINT32              RFU2:24;            // Reserved for User use or Future Ues

} COMMON_MMIO64_ADDR_TYPE;

typedef struct 
  {
    UINT32              Offset:16;          // The CSR Offset 
    UINT32              RFU1:16;            // Reserved for User use or Future Ues

    UINT32              RFU2:32;            // Reserved for User use or Future Ues

} COMMON_IO_ADDR_TYPE;

#pragma pack()

typedef union {
    UINT32                    dwRawData[4];
    ADDR_ATTRIBUTE_TYPE       Attribute;          // The Common Attribute type.
    COMMON_PCIE_ADDR_TYPE     Pcie;
    COMMON_CSR_ADDR_TYPE      Csr;
    COMMON_CSR_IPF_ADDR_TYPE  CsrIpf;
    COMMON_MMIO_ADDR_TYPE     Mmio;
    COMMON_MMIO64_ADDR_TYPE   Mmio64;
    COMMON_IO_ADDR_TYPE       Io;
} COMMON_ADDRESS_STRUCT;

//
// Assemble macro for ADDR_ATTRIBUTE_TYPE
//
#define CONSTRUCT_ADDR_ATTRIBUTE(Address, AddrType, Width, Cf8CfcEn, S3Enable, HptrType, HostPtr)  \
      ((UINT32 *)&Address)[3] = (UINT32)HostPtr;\
      ((UINT32 *)&Address)[2] = (UINT32) (\
      ((UINT32)((HptrType) & 0x01)<<14) | \
      ((UINT32)((S3Enable) & 0x1)<<13) | \
      ((UINT32)(((f8CfcEn) & 0x1)<<12) | \
      ((UINT32)((Width) & 0xF)<<8) | \
      (UINT32)(AddrType & 0x00FF)  )


//
// Assemble macro for COMMON_PCIE_ADDR_TYPE
//
#define CONSTRUCT_COMMON_PCIE_SEG_ADDRESS(Address, Width, Seg, Bus, Dev, Func, Offset)  \
      ((UINT32 *)&Address)[3] = (UINT32)0;\
      ((UINT32 *)&Address)[2] = (UINT32)( \
      ((((UINT32 *)&Address)[2] & 0x00000000) | \
      ((UINT32)((UINT8)Width & 0x0F)<<8) ) | \
      ((UINT32)((UINT8)AddrTypePCIE & 0x0FF)) ); \
      ((UINT32 *)&Address)[1] = (UINT32)( \
      (((UINT32 *)&Address)[1] & 0xFFFF0000) | \
      (UINT32)((Seg) & 0xFFFF)) ;\
      ((UINT32 *)&Address)[0] = (UINT32)( \
      ((UINT32)((Bus) & 0x0FF)<<20) | \
      ((UINT32)((Dev) & 0x01F)<<15) | \
      ((UINT32)((Func) & 0x07)<<12)| \
      ((UINT32)((Offset) & 0x000FFF)) )

//
// Assemble macro for COMMON_PCIE_SEG_ADDR_TYPE
//

#define CONSTRUCT_COMMON_PCIE_ADDRESS(Address, Width, Bus, Dev, Func, Offset)  \
      CONSTRUCT_COMMON_PCIE_SEG_ADDRESS(Address, Width, 0, Bus, Dev, Func, Offset)

//
// Assemble macro for COMMON_CSR_ADDR_TYPE
//
#define CONSTRUCT_COMMOM_CSR_ADDRESS(Address, Width, SocketId, BoxInst, BoxType, FuncBlk, Pseudo, Size, Offset)  \
      ((UINT32 *)&Address)[3] = (UINT32)0;\
      ((UINT32 *)&Address)[2] = (UINT32)( \
      (((UINT32 *)&Address)[2] & 0x00000000) | \
      ((UINT32)(Width & 0x0F)<<8) | \
      ((UINT32)(AddrTypeCSR & 0x0FF))) ; \
      ((UINT32 *)&Address)[1] = (UINT32)( \
      (((UINT32 *)&Address)[1] & 0xFFFF0000) | \
      ((UINT32)((SocketId) & 0xFF)<<8) | \
      (UINT32)((BoxInst) & 0xFF)); \
      ((UINT32 *)&Address)[0] = (UINT32)( \
      ((UINT32)((BoxType) & 0xFF)<<24) | \
      ((UINT32)((FuncBlk) & 0xFF)<<16) | \
      ((UINT32)((Pseudo) & 0x01)<<15) | \
      ((UINT32)((Size) & 0x07)<<12) | \
      (UINT32)((Offset) & 0x000FFF) );

//
// Assemble macro for COMMON_CSR_ADDR_TYPE
//
#define CONSTRUCT_COMMOM_CSR_OFFSET_ADDRESS(Address, Width, SocketId, BoxInst, CsrOffset)  \
      ((UINT32 *)&Address)[3] = (UINT32)0;\
      ((UINT32 *)&Address)[2] = (UINT32)( \
      (((UINT32 *)&Address)[2] & 0x00000000) | \
      ((UINT32)(Width & 0x0F)<<8) | \
      ((UINT32)(AddrTypeCSR & 0x0FF))) ; \
      ((UINT32 *)&Address)[1] = (UINT32)( \
      (((UINT32 *)&Address)[1] & 0xFFFF0000) | \
      ((UINT32)((SocketId) & 0xFF)<<8) | \
      (UINT32)((BoxInst) & 0xFF)); \
      ((UINT32 *)&Address)[0] = (UINT32)(CsrOffset)


//
// Assemble macro for COMMON_CSR_IPF_ADDR_TYPE
//
#define CONSTRUCT_COMMOM_CSR_IPF_ADDRESS(Address, Width, SocketId, BoxType, BoxInst, FuncBlk, Offset)  \
      ((UINT32 *)&Address)[3] = (UINT32)0;\
      ((UINT32 *)&Address)[2] = (UINT32)( \
      (((UINT32 *)&Address)[2] & 000000000) | \
      ((UINT32)(Width & 0x0F)<<8) | \
      ((UINT32)(AddrTypeCSR & 0x0FF))) ; \
      ((UINT32 *)&Address)[1] = (UINT32)( \
      (((UINT32 *)&Address)[1] & 0xFFFF0000) | \
      ((UINT32)((SocketId) & 0xFF)<<8) | \
      (UINT32)((BoxType) & 0xFF)); \
      ((UINT32 *)&Address)[0] = (UINT32)( \
      ((UINT32)((BoxInst) & 0xFF)<<24) | \
      ((UINT32)((FuncBlk) & 0xFF)<<16) | \
      (UINT32)((Offset) & 0x00FFFF) );

//
// Assemble macro for ZERO_ADDR_TYPE
//
#define CONSTRUCT_COMMOM_ZERO_ADDRESS(Address, AddressType)  \
      ((UINT32 *)&Address)[3] = (UINT32)0;\
      ((UINT32 *)&Address)[2] = (UINT32)((AddressType) & 0x0FF);\
      ((UINT32 *)&Address)[1] = (UINT32)0;\
      ((UINT32 *)&Address)[0] = (UINT32)0;


#endif

