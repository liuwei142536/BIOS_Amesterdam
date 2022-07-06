
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//

Copyright (c) 2009-2020 Intel Corporation.  All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license.  Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:
    WheaErrorInj.h

Abstract:
    This is an implementation of the Whea Support protocol.

-----------------------------------------------------------------------*/


#ifndef _WHEA_CORE_EINJ_
#define _WHEA_CORE_EINJ_

//
// Includes
//
//
// Consumed protocols
//
#include <Uefi.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/WheaPlatformHooksLib.h>
#include <Library/MemoryQpiInitLib.h>

#include <Protocol\IioUds.h>
#include <Protocol\SmmSwDispatch2.h>
#include <Protocol\SmmBase2.h>
#include <Protocol\WheaPlatformSupport.h>
#include <Protocol/MemRasProtocol.h>
#include <Protocol/CpuCsrAccess.h>
#include <Guid/MemoryConfigData.h>
#include "SmmELogHost.h"
#include <Guid/SetupVariable.h>
#include <Library/SynchronizationLib.h>
#include <Protocol/SmmCpuService.h>
#include <Protocol/SmmCpu.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Guid/SmramMemoryReserve.h>
#include <Include/Platform.h>

//
// Data & structure definition
//

// WHEA error injection action definitions
#define WHEA_EINJ_BEGIN_INJECT_OP                               0x00
#define WHEA_EINJ_GET_TRIGGER_ACTION_TABLE                      0x01
#define WHEA_EINJ_SET_ERROR_TYPE                                0x02
#define WHEA_EINJ_GET_ERROR_TYPE                                0x03
#define WHEA_EINJ_END_INJECT_OP                                 0x04
#define WHEA_EINJ_EXECUTE_INJECT_OP                             0x05
#define WHEA_EINJ_CHECK_BUSY_STATUS                             0x06
#define WHEA_EINJ_GET_CMD_STATUS                                0x07
#define WHEA_EINJ_SET_ERROR_TYPE_WITH_ADDRSS                    0x08
#define WHEA_EINJ_TRIGGER_ERROR_ACTION                          0xFF

// Error injection operation definitions
#define EINJ_BEGIN_OPERATION                                    0x55AA55AA
#define EINJ_END_OPERATION                                      0x00000000

// Error injection operation status
#define WHEA_EINJ_OPERATION_BUSY                                0x01
#define WHEA_EINJ_CMD_SUCCESS                                   0x00
#define WHEA_EINJ_CMD_UNKNOWN_FAILURE                           0x01
#define WHEA_EINJ_CMD_INVALID_ACCESS                            0x02

// Port for triggering SMI
#define R_APM_CNT       0xB2
// Error injection SW SMI defintions
#define EFI_WHEA_EINJ_EXECUTE_SWSMI                             0x9A
#define EFI_WHEA_EINJ_PATROL_SCRUB_SWSMI                        0x9B
#define EFI_WHEA_EINJ_IIO_ERR_INJ_LCK_SWSMI                     0x99


#define B0_REV_IVT  0x02
#define B0_REV_HSX  0x01

#define   VID_1_16_2_CFG_REG   0x20182000
#define   RID_1_16_2_CFG_REG   0x10182008
#define   DID_1_16_2_CFG_REG   0x20182002
/********** NEED TO BE PORTED FOR PLATFORM ****
#define UBOX_DEV_NUM    0x00
#define WBOX_DEV_NUM    0x01

#define MBOX0_DEV_NUM   0x05
#define MBOX1_DEV_NUM   0x07
UINT8 MBOX_DEV_NUM[] =  {MBOX0_DEV_NUM, MBOX1_DEV_NUM};
**********/

#define MC_ERR_INJ_LCK          0x790
#define MC_PS_INT_DOWNGRADE     0x17f
#define V_PS_INT_DOWNGRADE     0x4  // BIT2

#define   RSP_FUNC_ADDR_MATCH_LO_1_15_2_CFG_REG  0x4017a1c0
#define   RSP_FUNC_ADDR_MATCH_HI_1_15_2_CFG_REG  0x4017a1c4
#define   RSP_FUNC_ADDR_MASK_LO_1_15_2_CFG_REG   0x4017a1c8
#define   RSP_FUNC_ADDR_MASK_HI_1_15_2_CFG_REG   0x4017a1cc

#define   RSP_FUNC_ADDR_MATCH_LO_1_15_3_CFG_REG  0x4017b1c0
#define   RSP_FUNC_ADDR_MATCH_HI_1_15_3_CFG_REG  0x4017b1c4
#define   RSP_FUNC_ADDR_MASK_LO_1_15_3_CFG_REG   0x4017b1c8
#define   RSP_FUNC_ADDR_MASK_HI_1_15_3_CFG_REG   0x4017b1cc

#define   RSP_FUNC_ADDR_MATCH_LO_1_15_4_CFG_REG  0x4017c1c0
#define   RSP_FUNC_ADDR_MATCH_HI_1_15_4_CFG_REG  0x4017c1c4
#define   RSP_FUNC_ADDR_MASK_LO_1_15_4_CFG_REG   0x4017c1c8
#define   RSP_FUNC_ADDR_MASK_HI_1_15_4_CFG_REG   0x4017c1cc

#define   RSP_FUNC_ADDR_MATCH_LO_1_15_5_CFG_REG  0x4017d1c0
#define   RSP_FUNC_ADDR_MATCH_HI_1_15_5_CFG_REG  0x4017d1c4
#define   RSP_FUNC_ADDR_MASK_LO_1_15_5_CFG_REG   0x4017d1c8
#define   RSP_FUNC_ADDR_MASK_HI_1_15_5_CFG_REG   0x4017d1cc

#define   RSP_FUNC_CRC_ERR_INJ_EXTRA_1_16_2_CFG_REG   0x40182208
#define   RSP_FUNC_CRC_ERR_INJ_EXTRA_1_16_3_CFG_REG   0x40183208
#define   RSP_FUNC_CRC_ERR_INJ_EXTRA_1_16_6_CFG_REG   0x40186208
#define   RSP_FUNC_CRC_ERR_INJ_EXTRA_1_16_7_CFG_REG   0x40187208

#define   RSP_FUNC_CRC_ERR_INJ_DEV0_XOR_MSK_1_16_2_CFG_REG   0x40182200
#define   RSP_FUNC_CRC_ERR_INJ_DEV0_XOR_MSK_1_16_3_CFG_REG   0x40183200
#define   RSP_FUNC_CRC_ERR_INJ_DEV0_XOR_MSK_1_16_6_CFG_REG   0x40186200
#define   RSP_FUNC_CRC_ERR_INJ_DEV0_XOR_MSK_1_16_7_CFG_REG   0x40187200

#define   RSP_FUNC_CRC_ERR_INJ_DEV1_XOR_MSK_1_16_2_CFG_REG   0x40182204
#define   RSP_FUNC_CRC_ERR_INJ_DEV1_XOR_MSK_1_16_3_CFG_REG   0x40183204
#define   RSP_FUNC_CRC_ERR_INJ_DEV1_XOR_MSK_1_16_6_CFG_REG   0x40186204
#define   RSP_FUNC_CRC_ERR_INJ_DEV1_XOR_MSK_1_16_7_CFG_REG   0x40187204

#define   SADDBGMM0_1_12_0_CFG_REG   0x40160064
#define   SADDBGMM1_1_12_0_CFG_REG   0x40160068
#define   SADDBGMM2_1_12_0_CFG_REG   0x4016006c
#define   SADDBGMM0_1_12_1_CFG_REG   0x40161064
#define   SADDBGMM1_1_12_1_CFG_REG   0x40161068
#define   SADDBGMM2_1_12_1_CFG_REG   0x4016106c
#define   SADDBGMM0_1_12_2_CFG_REG   0x40162064
#define   SADDBGMM1_1_12_2_CFG_REG   0x40162068
#define   SADDBGMM2_1_12_2_CFG_REG   0x4016206c
#define   SADDBGMM0_1_12_3_CFG_REG   0x40163064
#define   SADDBGMM1_1_12_3_CFG_REG   0x40163068
#define   SADDBGMM2_1_12_3_CFG_REG   0x4016306c
#define   SADDBGMM0_1_13_0_CFG_REG   0x40168064
#define   SADDBGMM1_1_13_0_CFG_REG   0x40168068
#define   SADDBGMM2_1_13_0_CFG_REG   0x4016806c
#define   SADDBGMM0_1_13_1_CFG_REG   0x40169064
#define   SADDBGMM1_1_13_1_CFG_REG   0x40169068
#define   SADDBGMM2_1_13_1_CFG_REG   0x4016906c
#define   SADDBGMM0_1_13_2_CFG_REG   0x4016a064
#define   SADDBGMM1_1_13_2_CFG_REG   0x4016a068
#define   SADDBGMM2_1_13_2_CFG_REG   0x4016a06c
#define   SADDBGMM0_1_13_3_CFG_REG   0x4016b064
#define   SADDBGMM1_1_13_3_CFG_REG   0x4016b068
#define   SADDBGMM2_1_13_3_CFG_REG   0x4016b06c

#define   CBO_DBG_BUS_MATCH_1_12_0_CFG_REG   0x40160084
#define   CBO_DBG_BUS_MATCH_1_12_1_CFG_REG   0x40161084
#define   CBO_DBG_BUS_MATCH_1_12_2_CFG_REG   0x40162084
#define   CBO_DBG_BUS_MATCH_1_12_3_CFG_REG   0x40163084
#define   CBO_DBG_BUS_MATCH_1_13_0_CFG_REG   0x40168084
#define   CBO_DBG_BUS_MATCH_1_13_1_CFG_REG   0x40169084
#define   CBO_DBG_BUS_MATCH_1_13_2_CFG_REG   0x4016a084
#define   CBO_DBG_BUS_MATCH_1_13_3_CFG_REG   0x4016b084

#define   CBO_GLB_RSP_CNTRL_1_12_0_CFG_REG   0x40160094
#define   CBO_GLB_RSP_CNTRL_1_12_1_CFG_REG   0x40161094
#define   CBO_GLB_RSP_CNTRL_1_12_2_CFG_REG   0x40162094
#define   CBO_GLB_RSP_CNTRL_1_12_3_CFG_REG   0x40163094
#define   CBO_GLB_RSP_CNTRL_1_13_0_CFG_REG   0x40168094
#define   CBO_GLB_RSP_CNTRL_1_13_1_CFG_REG   0x40169094
#define   CBO_GLB_RSP_CNTRL_1_13_2_CFG_REG   0x4016a094
#define   CBO_GLB_RSP_CNTRL_1_13_3_CFG_REG   0x4016b094

#define ENTRY_INVALID   0xFF

#pragma pack (1)

#define EinjRegisterExecute { \
  EFI_ACPI_3_0_SYSTEM_IO,     \
  16                          \
  0,                          \
  EFI_ACPI_3_0_BYTE,          \
  0xB2}

#define EinjRegisterFiller  { \
  EFI_ACPI_3_0_SYSTEM_MEMORY, \
  64,                         \
  0,                          \
  EFI_ACPI_3_0_QWORD,         \
  0}

typedef struct {
  UINT32                HeaderSize;
  UINT32                Revision;
  UINT32                TableSize;
  UINT32                EntryCount;
} WHEA_ERROR_TRIGGER_ACTION_HEADER;

typedef struct {
  WHEA_ERROR_TRIGGER_ACTION_HEADER              Header;
  EFI_ACPI_WHEA_SERIALIZATION_ACTION            Trigger0;
  EFI_ACPI_WHEA_SERIALIZATION_ACTION            Trigger1;
  EFI_ACPI_WHEA_SERIALIZATION_ACTION            Trigger2;
  EFI_ACPI_WHEA_SERIALIZATION_ACTION            Trigger3;
} WHEA_EINJ_TRIGGER_ACTION_TABLE;


typedef struct {
  UINT64                Busy:1;
  UINT64                CmdSts:8;
  UINT64                Resv1:55;
} WHEA_EINJ_OP_STATUS;

typedef struct {
  UINT32		ErrType;
  UINT32		VendorErrTypeExtOffset;
  UINT32		Flags;
  UINT32		ApicId;
  UINT64		MemAddress;
  UINT64		MemAddressRange;
  UINT32		PcieSBDF;
} WHEA_EINJ_SET_ERROR_TYPE_WITH_ADDRESS;

typedef struct {
  UINT32                Length;		
  UINT32                SBDF;		// This provides a PCIe Segment, Bus, Device and Function number which can be used to read the Vendor ID, Device ID and Rev ID, 
					// so that software can identify the system for error injection purposes            
  UINT16                VendorId;
  UINT16                DeviceId;
  UINT8                 RevId;
  UINT8                 Resvd[3];
  UINT64		OemDefType1;
  UINT64		OemDefType2;
} WHEA_EINJ_VENDOR_EXT;

typedef struct {
  UINT64                OpState;            // opeartion state, begin/end/etc
  UINT64                ErrorToInject[5];
  UINT64                TriggerActionTable;
  UINT64                ErrorInjectCap;    // Qowrd0 -  Eroor injection cap.
  UINT64                OpStatus;
  UINT64                CmdStatus;
  UINT64                EinjAddr;
  UINT64                AddrMask;
  WHEA_EINJ_SET_ERROR_TYPE_WITH_ADDRESS  SetAddressTable;
  WHEA_EINJ_VENDOR_EXT  VendorExt;
} WHEA_EINJ_PARAM_BUFFER;

typedef struct {
  UINT8 chEnabled;
} CH_PRESENT_INFO;

typedef struct {
  UINT8 enabled;
  CH_PRESENT_INFO channelList[MAX_CH];
} IMC_PRESENT_INFO;

typedef struct {
  UINT32	socketPresentBitMap;
  UINT8		ExistingNodeNum;
  UINT32	mmCfgBase;
  UINT8		busIio[MAX_SOCKET];
  UINT8		busUncore[MAX_SOCKET];
  UINT16	CpuStepping;
  UINT8		RASmode[MAX_MEM_NODE];
  UINT8		RASModesEx[MAX_MEM_NODE];
  IMC_PRESENT_INFO imc[MAX_MEM_NODE];
  EFI_STATUS OperationSts;  
} SOCKET_NODE_INFO;

VOID *
EFIAPI
AsmFlushCacheLine (
  IN      VOID                      *LinearAddress
  );

VOID
AsmWbinvd (
  VOID
  );

VOID
AsmNop (
  VOID
  );

VOID
TurnOffCache (
  VOID
  );

VOID
TurnOnCache (
  VOID
  );

VOID
InjectionFlow (
  VOID
  );

EFI_STATUS
UpdateEinj (
  VOID
  );

VOID
DisablePsmi (
  VOID
  );

VOID
DisablePrefetch (
  VOID
  );

VOID
EnablePrefetch (
  VOID
  );

VOID
TurnOnQuiesce (
  VOID
  );

VOID
TurnOffQuiesce (
  VOID
  );

VOID
AsmMfence (
  VOID
  );

QPI_STATUS
GetCboCountAndListWhea (
  UINT8            SocId,
  UINT8            *CboCount,
  UINT32           *CboMap 
  );

EFI_STATUS
IioErrInjLock(
  VOID
  );

EFI_STATUS
IioErrInjUnLock(
  VOID
  );


#pragma pack ()

#endif  //_WHEA_CORE_EINJ_
