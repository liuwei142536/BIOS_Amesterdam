/*++
Copyright (c) 2007 - 2010 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  LastBootErrorLog.h

Abstract:

  

---------------------------------------------------------------------*/

#ifndef _LAST_BOOT_ERROR_LOG_H_
#define _LAST_BOOT_ERROR_LOG_H_

#include <Library/BaseLib.h>
//#include <Library/DebugLib.h>
//#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
//#include <Protocol/SmmBase2.h>
#include <Library\PciExpressLib.h>
#include <IndustryStandard\Pci22.h>
#include <Cpu\CpuBaseLib.h>
#include "WheaPlatformDefs.h"
#include <Protocol/WheaBootProtocol.h>
#include <Library/HobLib.h>
#include <Guid\HobList.h>
#include <Guid/SetupVariable.h>
#include <Protocol/MemRasProtocol.h>
#include <Protocol/Smbios.h>
#include <Protocol/IioUds.h>
#include <Protocol/CpuCsrAccess.h>
// AptioV Server Override Start: Added to skip LastBoot Error Handler for cold boot.
#include <IndustryStandard/SmBios.h>
#include <Guid/MemoryMapData.h>
// AptioV Server Override End: Added to skip LastBoot Error Handler for cold boot.
//TODO: TBD is a table to contain both IVT and HSX values. Need to re-route code to use the table instead.
#define IVT_CORE_MCBANK_NUM_START	0
#define IVT_CORE_MCBANK_NUM_END		3
#define IVT_QPI_MCBANK_NUM_START	4
#define IVT_QPI_MCBANK_NUM_END		5
#define IVT_MEM_MCBANK_NUM_START	7
#define IVT_MEM_MCBANK_NUM_END		16
#define IVT_CBO_MCBANK_NUM_START	17
#define IVT_CBO_MCBANK_NUM_END		31

#define PCIE_PORT_1A_DEV    0x01
#define PCIE_PORT_1A_FUNC   0x00
#define PCIE_PORT_1B_DEV    0x01
#define PCIE_PORT_1B_FUNC   0x01

#define PCIE_PORT_2A_DEV    0x02
#define PCIE_PORT_2A_FUNC   0x00
#define PCIE_PORT_2B_DEV    0x02
#define PCIE_PORT_2B_FUNC   0x01
#define PCIE_PORT_2C_DEV    0x02
#define PCIE_PORT_2C_FUNC   0x02
#define PCIE_PORT_2D_DEV    0x02
#define PCIE_PORT_2D_FUNC   0x03

#define PCIE_PORT_3A_DEV    0x03
#define PCIE_PORT_3A_FUNC   0x00
#define PCIE_PORT_3B_DEV    0x03
#define PCIE_PORT_3B_FUNC   0x01
#define PCIE_PORT_3C_DEV    0x03
#define PCIE_PORT_3C_FUNC   0x02
#define PCIE_PORT_3D_DEV    0x03
#define PCIE_PORT_3D_FUNC   0x03

#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS     28
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_8  7
#define PCH_PCIE_RROT_PORT_AER_UES   0x104

#define UEFI_PROC_UNKNOWN_ERR_TYPE    00
#define UEFI_PROC_CACHE_ERR_TYPE		  01
#define UEFI_PROC_TLB_ERR_TYPE        02
#define UEFI_PROC_BUS_ERR_TYPE        04
#define UEFI_PROC_MICRO_ARCH_ERR_TYPE 0x8

#ifndef PCI_HEADER_TYPE_OFFSET
  #define PCI_HEADER_TYPE_OFFSET      0x0E
#endif

#define PCI_ADDRESS(seg, bus, dev, func, reg) \
    ((UINTN) ((((UINTN) bus) << 20) + (((UINTN) dev) << 15) + (((UINTN) func) << 12) + ((UINTN) reg)))
#define ONLY_REGISTER_OFFSET(x)  (x & 0xFFF)

typedef struct {
  UINT16          WheaErrorRecordFormat;
  BOOLEAN         SetupMcBankStsClear;
  BOOLEAN         PciexErrFound;
} PREV_BOOT_ERR_GLOBAL_DATA;

typedef struct {
  UINT8 Enabled;
  UINT8 Device;
  UINT8 Function;
} PCIE_PORT_INFO;

typedef struct {
  UINT8 BusNum;
  UINT8 SocketId;
  UINT8 Core20BusNum;
  UINT8 Valid;
  PCIE_PORT_INFO Port[NUMBER_PORTS_PER_SOCKET];
} IOH_INFO;
// AptioV Server Override Start: Added to log Error details for Last Boot Whea Memory Errors.
typedef struct {
  UINT16  LastBootType17Handle[MAX_MEM_NODE][MAX_CH][MAX_DIMM];
  UINT16  LastBootType16Hanlde;
} LASTBOOT_SMBIOS_HANDLE;
// AptioV Server Override End: Added to log Error details for Last Boot Whea Memory Errors.
#endif

