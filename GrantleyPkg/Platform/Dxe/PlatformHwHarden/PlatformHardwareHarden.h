/**

Copyright (c)  1999 - 2022 Intel Corporation. All rights
reserved This software and associated documentation (if any) is 
furnished under a license and may only be used or copied in 
accordance with the terms of the license. Except as permitted by 
such license, no part of this software or documentation may be 
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


  @file PlatformHwHardening.h

  
**/


#include <Cpu/CpuRegs.h>
#include <Guid/PlatformTxt.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/MmPciBaseLib.h>
#include <Library/PcdLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/MpService.h>
#include "RcRegs.h"
#include <PchRegs.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/PciEnumerationComplete.h>
#include <Protocol/IioUds.h>
#include <Library/ReportStatusCodeLib.h>

#define MAX_INDIRECT_DEVICE_COUNT   16
#define SMBUS_MMIO_BAR_MAX          0xC0000000
#define ALIGN_64_BYTES              0x40
#define ALIGN_4KB_POWER_OF_2        0x0C  // 2 to the 12 power (or 12 bits for
                                          // a binary value) = 4KBytes = 0x1000
                                          // = 0001 0000 0000 0000)B
#define SIZE_32B                    0x20
#define ALIGN_32B_POWER_OF_2        0x05  // 2 to the 5 power (or 5 bits for
                                          // a binary value) = 32Bytes = 0x20
                                          // = 0010 0000)B

//Debug only
#define EFI_DEADLOOP()    { volatile int __iii; __iii = 1; while (__iii); }

#define PCH_GPIO_BASE_ADDRESS           0x0500

#define PCH_PADCFGLOCK_GPP_A          0x60
#define PCH_PADCFGLOCKTX_GPP_A        0x64
#define PCH_PADCFGLOCK_GPP_B          0x68
#define PCH_PADCFGLOCK_GPPTX_B        0x6C
#define PCH_PADCFGLOCK_GPP_F          0x70
#define PCH_PADCFGLOCK_GPPTX_F        0x74

typedef struct {
  UINT64  PcieDeviceMmCfgBase;
  UINT32  PcieDeviceMmCfgMask;
  UINT32  PcieDeviceMmioRegAddress;
  UINT32  PcieDeviceIoRegAddress;
  UINT8   IndirectDeviceAddressCount;
  UINT8   IndirectDeviceAddress[MAX_INDIRECT_DEVICE_COUNT];
  UINT32  GpioPadNumberToTxLock;
} EFI_HHT_TABLE;


typedef struct {
  UINT64  IndirectDevicesListStatusLo;
  UINT64  IndirectDevicesListStatusHi;
  UINT64  PchSmbusMmCfgBaseAddress;
  UINT64  PchSmbusMmioBAR;
  UINT64  PchSmbusIoBar;
  UINT64  ShadowBufferAddress;
  UINT64  Reserved1;
  UINT64  Reserved2;
} PHH_MSR_REGISTER_STRUCT;


EFI_STATUS
EFIAPI
PlatformHardwareHardenEntry ( 
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
);
