//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c) 1996 - 2016, Intel Corporation.

This source code and any documentation accompanying it ("Material") is furnished
under license and may only be used or copied in accordance with the terms of that
license.  No license, express or implied, by estoppel or otherwise, to any
intellectual property rights is granted to you by disclosure or delivery of these
Materials.  The Materials are subject to change without notice and should not be
construed as a commitment by Intel Corporation to market, license, sell or support
any product or technology.  Unless otherwise provided for in the license under which
this Material is provided, the Material is provided AS IS, with no warranties of
any kind, express or implied, including without limitation the implied warranties
of fitness, merchantability, or non-infringement.  Except as expressly permitted by
the license for the Material, neither Intel Corporation nor its suppliers assumes
any responsibility for any errors or inaccuracies that may appear herein.  Except
as expressly permitted by the license for the Material, no part of the Material
may be reproduced, stored in a retrieval system, transmitted in any form, or
distributed by any means without the express written consent of Intel Corporation.


Module Name:  

CommonErrorHandlerIncludes.h

Abstract:

  This file defines common equates.

--*/

#ifndef _COMMON_ERRORHANDLER_INCLUDES_H_
#define _COMMON_ERRORHANDLER_INCLUDES_H_

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/PciLib.h>
#include <Library\PciExpressLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/HobLib.h> // AptioV Server Override: Added to skip LastBoot Error Handler for cold boot.

#include <Library/LocalApicLib.h>
#include <Register/LocalApic.h>
#include <Library/PcdLib.h>

#include <Protocol/SmmBase2.h>
#include <Protocol/WheaPlatformSupport.h>
#include <Protocol/SmmRtProtocol.h>
#include <Protocol/SmmSxDispatch2.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/SmmStatusCode.h>
#include <Protocol/SmmGpiDispatch2.h>
#include <Protocol/SmmVariable.h>
#include <Protocol/IioUds.h>
#include <Protocol/MpService.h>
#include <Protocol/SmmVariable.h>
#include <Protocol/MemRasProtocol.h>
#include <Protocol/CpuCsrAccess.h>
#include <Protocol/QpiRasProtocol.h>
#include <Protocol/Smbios.h>
#include <Protocol/GlobalNvsArea.h>
#include <Protocol/CpuIo.h>
#include <Protocol\SmmSwDispatch.h>
#include <Protocol/SmmCpuService.h>
#include <Protocol/PlatformSlotInfoGuid.h>
#include <Protocol/SmmErrorLogProtocol.h>

#include <IndustryStandard/SmBios.h>

#include <Guid/SetupVariable.h>
#include <Guid/MemoryConfigData.h>
#include <Guid/PprVariable.h>
#include <Guid/MemoryMapData.h> // AptioV Server Override: Added to skip LastBoot Error Handler for cold boot.
#include "PchRegs.h"
#include "SysHost.h"


#include <Iio/IioRegs.h>
#include "Syshost.h"
#include "Platform.h"
#include "SmmErrorHandler.h"
#include "PlatformErrorHandler.h"
#include "ProcessorErrorHandler.h"
#include "PcieErrorHandler.h"
#include "PCHErrorHandler.h"
#include "IIoErrorHandler.h"
#include "MemoryErrorHandler.h"
#include "OemErrorHandler.h"
#include "McaHandler.h"
#include "IohErrorHandler.h" //AptioV Server Override: Changes done to Support the LastBoot Error (Memory/PCIE/IOH/Core/QPI/VTD Handlers
#include "RtErrorLog.h" //AptioV Server Override: Changes done to use AMI RuntimeError (RtErrorLogBMC/Whea) Listener code.
#include "RtErrorLogBoard.h" //AptioV Server Override: Changes done to use AMI RuntimeError (RtErrorLogBMC/Whea) Listener code.

#ifdef WHEA_SUPPORT
  #include "WheaPlatformDefs.h"
#endif
#include <Protocol/WheaPlatformSupport.h>
#include "UncoreCommonIncludes.h"
#include <Protocol/SmmReadyToLock.h>


#define V_INTEL_VID               0x8086
#define CLEAR_ERROR_SWSMI         0x95


//
// Min Max
//
#define V_MIN(a, b)       (((a) < (b)) ? (a) : (b))
#define V_MAX(a, b)       (((a) > (b)) ? (a) : (b))

//
// Bit map macro
//
#ifndef BIT0

#define BIT63   0x8000000000000000
#define BIT62   0x4000000000000000
#define BIT61   0x2000000000000000
#define BIT60   0x1000000000000000
#define BIT59   0x0800000000000000
#define BIT58   0x0400000000000000
#define BIT57   0x0200000000000000
#define BIT56   0x0100000000000000
#define BIT55   0x0080000000000000
#define BIT54   0x0040000000000000
#define BIT53   0x0020000000000000
#define BIT52   0x0010000000000000
#define BIT51   0x0008000000000000
#define BIT50   0x0004000000000000
#define BIT49   0x0002000000000000
#define BIT48   0x0001000000000000
#define BIT47   0x0000800000000000
#define BIT46   0x0000400000000000
#define BIT45   0x0000200000000000
#define BIT44   0x0000100000000000
#define BIT43   0x0000080000000000
#define BIT42   0x0000040000000000
#define BIT41   0x0000020000000000
#define BIT40   0x0000010000000000
#define BIT39   0x0000008000000000
#define BIT38   0x0000004000000000
#define BIT37   0x0000002000000000
#define BIT36   0x0000001000000000
#define BIT35   0x0000000800000000
#define BIT34   0x0000000400000000
#define BIT33   0x0000000200000000
#define BIT32   0x0000000100000000

#define BIT31   0x80000000
#define BIT30   0x40000000
#define BIT29   0x20000000
#define BIT28   0x10000000
#define BIT27   0x08000000
#define BIT26   0x04000000
#define BIT25   0x02000000
#define BIT24   0x01000000
#define BIT23   0x00800000
#define BIT22   0x00400000
#define BIT21   0x00200000
#define BIT20   0x00100000
#define BIT19   0x00080000
#define BIT18   0x00040000
#define BIT17   0x00020000
#define BIT16   0x00010000
#define BIT15   0x00008000
#define BIT14   0x00004000
#define BIT13   0x00002000
#define BIT12   0x00001000
#define BIT11   0x00000800
#define BIT10   0x00000400
#define BIT9    0x00000200
#define BIT8    0x00000100
#define BIT7    0x00000080
#define BIT6    0x00000040
#define BIT5    0x00000020
#define BIT4    0x00000010
#define BIT3    0x00000008
#define BIT2    0x00000004
#define BIT1    0x00000002
#define BIT0    0x00000001
#endif

#define MceHndApTimeOut		50000				// 10,000 usecs 

#define BITS(x)  (1<<(x))
#define CPU_TOTAL_THREAD_COUNT (MAX_CORE * MAX_THREAD)

/*
Notes : 
  1.  Bit position always starts at 0.
  2.  Following macros are applicable only for Word alligned integers. 
*/
#define BIT(Pos, Value)              ( 1<<(Pos) & (Value))
#define BITRANGE(From,Width,Value)   (((Value) >> (From)) & (( 1<<(Width)) -1))

//
// Define macros for including Protocols and Guids.
//
#define EFI_STRINGIZE(a)            #a
#define EFI_PROTOCOL_DEFINITION(a)  EFI_STRINGIZE (Protocol/a/a.h)
#define EFI_GUID_DEFINITION(a)      EFI_STRINGIZE (Guid/a/a.h)
#define GUID_VARIABLE_DECLARATION(Variable, Guid) extern EFI_GUID Variable
//
// These should be used to include protocols.  If they are followed,
// intelligent build tools can be created to check dependencies at build
// time.
//
#define EFI_PROTOCOL_CONSUMER(a)    EFI_PROTOCOL_DEFINITION (a)



//
// Task priority level
//
#define EFI_TPL_APPLICATION 4
#define EFI_TPL_CALLBACK    8
#define EFI_TPL_NOTIFY      16
#define EFI_TPL_HIGH_LEVEL  31

#define GES_SIZE SIZE_4KB
#define MAX_ERROR_EVENTS MaxErrSrc

// ELOG Host Structure
//
typedef struct {
  UINT8	maxDimm;
  UINT8	dimmEnabled;
  UINT8 dimmNum;
}DIMM_INFO;

typedef struct _SMMELOG_CHANNELLIST {
  UINT8 chEnabled;
  UINT8 chFailed;
  //UINT8 spareDimm;
  //UINT8 spareLogicalRank;
  //UINT8 spareInUse;
  UINT8 oldLogicalRank;
  //UINT8 devTag[MAX_RANK_CH];
  UINT32 leakyBucket2ndCounter;
  UINT32 rankErrCountInfo[MAX_RANK_CH];
  UINT8  maxDimm;
  DIMM_INFO dimmInfo[MAX_DIMM];
} SMMELOG_CHANNELLIST;

typedef struct _SMMELOG_IMC {
  UINT8 enabled;
  SMMELOG_CHANNELLIST channelList[MAX_CH];
  //UINT8	vmseChannelEnabled[MAX_VMSE_CH];
} SMMELOG_IMC;

typedef struct _SMMELOG_CPUINFO {
  BOOLEAN Valid[MAXCOREID][MAX_THREAD];
  UINTN cpuNumber[MAXCOREID][MAX_THREAD];
  UINT32 ApicId[MAXCOREID][MAX_THREAD];
  UINT8 ApicVer[MAXCOREID][MAX_THREAD];
} SMMELOG_CPUINFO;

typedef struct {
  UINT16                    SmBiosType17Handle;
 // UINT16                    MemoryErrorInformationHandle;
  SMBIOS_TABLE_STRING       DeviceLocator;
  SMBIOS_TABLE_STRING       BankLocator;
  UINT8                     MemoryType;                     ///< The enumeration value from MEMORY_DEVICE_TYPE.
  SMBIOS_TABLE_STRING       Manufacturer;
  SMBIOS_TABLE_STRING       PartNumber;

} ELOG_SMBIOS_TYPE17;

typedef struct _SMMELOG_HOST {
  UINT8 bootMode;
  UINT8 RASmode[MAX_MEM_NODE];
  UINT8 RASModesEx[MAX_MEM_NODE];
  UINTN cpuCount;
  UINTN cpuCountEnabled;
  //UINT8	vmseOptions;
  UINT32 socketPresentBitMap;
  UINT8 changedSocketBitmap;	
  UINT16 spareErrTh;
  UINT32 options;
  UINT32 mmCfgBase;
  UINT8 busIio[MAX_SOCKET];
  UINT8 busUncore[MAX_SOCKET];
  UINT16 CpuFamily;
  UINT16 CpuStepping;
  SMMELOG_IMC imc[MAX_MEM_NODE];
  SMMELOG_CPUINFO cpuInfo[MAX_SOCKET];
  ELOG_SMBIOS_TYPE17	ElogSmBiosType17[MAX_MEM_NODE][MAX_CH][MAX_DIMM];
  UINT16  ElogType16Hanlde;
  UINT64  UncoreSmiErrSrc[MAX_SOCKET];
} SMMELOG_HOST;

extern SMMELOG_HOST mELogHost;
extern EFI_CPU_CSR_ACCESS_PROTOCOL     *mCpuCsrAccess;
extern SMM_ERROR_MP_SYNC_DATA mSmmErrorMpSyncData;
extern EFI_SMM_CPU_SERVICE_PROTOCOL  *mSmmCpuService;

#endif


