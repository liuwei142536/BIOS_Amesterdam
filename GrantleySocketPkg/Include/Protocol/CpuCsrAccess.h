//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Copyright (c) 2007 Intel Corporation.  All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license.  Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

    CpuCsrAccess.h

Abstract:

    Header file for IOX access APIs.

-----------------------------------------------------------------------------*/
#ifndef _CPUCSRACCESS_PROTOCOL_H_
#define _CPUCSRACCESS_PROTOCOL_H_

//
// CPU CSR Access Protocol GUID
//
// {0067835F-9A50-433a-8CBB-852078197814}
#define EFI_CPU_CSR_ACCESS_GUID \
  { \
    0x67835f, 0x9a50, 0x433a, 0x8c, 0xbb, 0x85, 0x20, 0x78, 0x19, 0x78, 0x14 \
  }

//
//4987134 start
//

typedef
UINTN
(EFIAPI *GET_CPU_CSR_ADDRESS) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   Offset,
  IN OUT UINT8 *Size
  );
  
//
//4987134 end
//
typedef
UINT32
(EFIAPI *READ_CPU_CSR) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   Offset
  );

typedef
VOID
(EFIAPI *WRITE_CPU_CSR) (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   RegOffset,
  IN UINT32   Data
  );

typedef
UINT32
(EFIAPI *READ_MEM_CPU_CSR) (
  IN UINT8    NodeId,
  IN UINT8    ChIdOrBoxInst,
  IN UINT32   Offset
  );

typedef
VOID
(EFIAPI *WRITE_MEM_CPU_CSR) (
  IN UINT8    NodeId,
  IN UINT8    ChIdOrBoxInst,
  IN UINT32   RegOffset,
  IN UINT32   Data
  );

typedef
UINT32
(EFIAPI *READ_PCI_CSR) (
  IN UINT8    bus,
  IN UINT8    dev,
  IN UINT8    func,
  IN UINT32   reg,
  IN UINT8    regSize
  );

typedef
VOID
(EFIAPI *WRITE_PCI_CSR) (
  IN UINT8    bus,
  IN UINT8    dev,
  IN UINT8    func,
  IN UINT32   reg,
  IN UINT8    regSize,
  IN UINT32   data
  );

typedef
UINT32
(EFIAPI *BIOS_2_PCODE_MAILBOX_WRITE) (
  IN UINT8  socket,
  IN UINT32 dwordCommand,
  IN UINT32 dworddata
  );

typedef struct _EFI_CPU_CSR_ACCESS_PROTOCOL {
  GET_CPU_CSR_ADDRESS         GetCpuCsrAddress;
  READ_CPU_CSR                ReadCpuCsr;
  WRITE_CPU_CSR               WriteCpuCsr;
  BIOS_2_PCODE_MAILBOX_WRITE  Bios2PcodeMailBoxWrite;
  READ_MEM_CPU_CSR            ReadMemCpuCsr;
  WRITE_MEM_CPU_CSR           WriteMemCpuCsr;
  READ_PCI_CSR                ReadPciCsr;
  WRITE_PCI_CSR               WritePciCsr;
} EFI_CPU_CSR_ACCESS_PROTOCOL;

extern EFI_GUID         gEfiCpuCsrAccessGuid;

#endif

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Copyright (c) 2007 Intel Corporation.  All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license.  Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

-----------------------------------------------------------------------------*/
