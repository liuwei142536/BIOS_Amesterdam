//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c) 2007 - 2016 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  ProcessorErrorLog.h

Abstract:

  Processor based component hooks for event logging.

---------------------------------------------------------------------*/

#ifndef _PROCESSOR_ERROR_HANDLER_H_
#define _PROCESSOR_ERROR_HANDLER_H_

#include <Library\PciExpressLib.h>
#include "SmmErrorHandler.h"
#include <Library\emcaplatformhookslib.h>


//
// Constants, macros and other useful definitions...
//
#define MAX_NR_CPU_SOCKETS                    1
#define MAX_NR_CPU_CSI_LINKS                  1

#define DEF_OEM_COR_ERR_THRESHOLD             0x0A

#define EFI_EPSD_PCIe_MAX_NR_BUS              63
#define EFI_DEF_IOH_MAX_NR_BUS                255

//
// CPUID 
// 
#define CPUID_TOPO_INFO                       0x0b

//
// CSR Definitions...
//
#define NHLM_UNCORE_DEV                       0x00
#define NHLM_UNCORE_FUNC                      0x00

#define NHLM_UNCORE_GEN_DIDVID                0x2C408086

#define NHLM_SOCKET_Lp_DEV                    0x02
#define NHLM_SOCKET_Lp_FUNC(p)                (0x00 + (p << 2))   // 'p' represents QPI port...

#define R_QPI_QPILCP_Lp                       0x40
#define   B_QPI_QPILCP_Lp_CRC_SUPPORTED         (1 << 11)

#define NHLM_IMC_DEV                          0x03
#define NHLM_IMC_FUNC                         0x00
#define NHLM_IMC_RAS_FUNC                     0x02

#define NHLM_IMC_CH_DEV(c)                    (0x04 + c)  // 'c' represents channel
#define NHLM_IMC_CH_FUNC                      0x01

#define R_MC_SMI_SPARE_CNTRL                  0x54
#define   B_MC_SMI_SPARE_CNTRL_INT_SEL_SMI      (1 << 15)
#define   M_MC_SMI_SPARE_CONTROL_THRESHOLD      0x07FFF

#define R_MC_SMI_SPARE_DIMM_ERROR_STATUS      0x50
#define   M_DIMM_ERROR_OVERFLOW_STATUS          0x0777

#define R_MC_SSRCONTROL                       0x48
#define   M_MC_SSRCONTROL_SCRATCHPAD            0x07F80
#define   S_MC_SSRCONTROL_SCRATCHPAD            7
#define   B_MC_SSRCONTROL_DEMAND_SCRUB_EN       0x40
#define   M_MC_SSRCONTROL_SSR_MODE              0x03
#define   V_MC_SSRCONTROL_SSR_MODE_IDLE         0x00
#define   V_MC_SSRCONTROL_SSR_MODE_SCRUB        0x01
#define   V_MC_SSRCONTROL_SSR_MODE_SPARE        0x02
#define   M_MC_SSRCONTROL_SSR_MODE              0x03
#define   M_MC_SSRCONTROL_SRC_CHAN              (0x03 << 2)
#define   S_MC_SSRCONTROL_SRC_CHAN              2
#define   M_MC_SSRCONTROL_DEST_CHAN             (0x03 << 4)
#define   S_MC_SSRCONTROL_DEST_CHAN             4

#define R_MC_COR_ECC_CNT                      0x80
#define   M_MC_COR_ECC_CNT_DIMM_COR_ERR         0x7FFF
#define   B_MC_COR_ECC_CNT_DIMM_ERR_OVERFLOW    (1 << 15)
#define   S_MC_COR_ECC_CNT_DIMM1_ADJUST         16

#define R_MC_SSRSTATUS                        0x60
#define   B_MC_SSRSTATUS_INPROGRESS             0x02
#define   B_MC_SSRSTATUS_COMPLT                 0x01

#define R_MC_RAS_STATUS                       0x54
#define   B_MC_RAS_STATUS_REDUNDANCY_LOSS       0x01

#define R_MC_RAS_ENABLES                      0x50
#define   B_MC_RAS_ENABLES_MIRROREN             0x01

#define R_MC_RAS_ENABLES                      0x50
#define   B_MC_RAS_ENABLES_MIRROREN             0x01

#define R_MC_CHANNEL_MAPPER                   0x60
#define   M_MC_CHANNEL_MAPPER_LCH               0x07
#define   S_MC_CHANNEL_MAPPER_WR                0
#define   S_MC_CHANNEL_MAPPER_RD                3

#define R_MC_DOD_CHc_d(d)                     0x48  // 'd' represents DIMM slot...
#define   B_MC_DOD_CHc_d_DIMMPRESENT            (1 << 9)  // 'c' represents channel
                                                          // 'd' represents DIMM slot
#define   IA32_MCI_STATUS_OFFSET 1

#define HA_MCBANK_TYPE						02
#define IMC_MCBANK_TYPE						01


//
// MSR Definitions...
//

//
// Number of mc banks in core on jkt
//
#define MCA_CORE_MCBANKS                      4

// TODO: check bank nmber
#define MCA_BANK_HA                           7
#define MCA_BANK_HA1_IMC3					  16	
#define MSR_MCG_CONTAIN						  0x178

#define IA32_MC_I_BANK_SIZE                   4
#define IA32_MC_I_STATUS_OFFSET               1
#define IA32_MC_I_ADDR_OFFSET                 2
#define IA32_MC_I_MISC_OFFSET                 3

#define MSR_IA32_MCG_CAP                      0x179

#define MSR_IVT_IIO_MC12_CTL                  0x418
#define MSR_HSX_IIO_MC12_CTL                  0x430

#define IA32_MC0_CTL                          0x400

#define IA32_MC8_CTL                          0x420
#define   V_IA32_MC8_CTL                        0x07FF

#define IA32_MC8_STATUS                       0x421
#define   B_IA32_MC8_STATUS_VAL                 ((UINT64)1 << 63)
#define   B_IA32_MC8_STATUS_UC                  ((UINT64)1 << 61)
#define   B_IA32_MC8_STATUS_MISCV               ((UINT64)1 << 59)
#define   B_IA32_MC8_STATUS_ADDRV               ((UINT64)1 << 58)
#define   M_IA32_MC8_STATUS_MSCOD               0xFFFF0000
#define     B_IA32_MC8_STATUS_MSCOD_SPARE_FAIL    (1 << 20)
#define     S_IA32_MC8_STATUS_MSCOD               16

#define IA32_MC8_MISC                         0x423
#define   M_IA32_MC8_MISC_DIMM                  (0x03 << 16)
#define   M_IA32_MC8_MISC_CHANNEL               (0x03 << 18)
#define   M_IA32_MC8_MISC_SYNDROME              ((UINT64)0xFFFFFFFF << 32)

#define   S_IA32_MC8_MISC_DIMM                  16
#define   S_IA32_MC8_MISC_CHANNEL               18
#define   S_IA32_MC8_MISC_SYNDROME              32

#define IA32_MC8_ADDRESS                      0x422

//
// EPSD Spare Information Packet stored in SSRCONTROL scratchpad...
// Bit  7       - Spare Enabled
// Bits 8       - Spare Copy Operation was started on last SMI...
// Bits 10:9    - Failed Channel for which sparing was initiated.
// Bits 14:11   - Reserved/Unused/Available.
//
#define B_SCRATCHPAD_SPARE_EN                 (1 << S_MC_SSRCONTROL_SCRATCHPAD)
#define M_SCRATCHPAD_FAILED_CHANNEL           (0x0C << S_MC_SSRCONTROL_SCRATCHPAD)
#define B_SCRATCHPAD_SPARE_STARTED            (2 << S_MC_SSRCONTROL_SCRATCHPAD)
#define S_SCRATCHPAD_FAILED_CHANNEL           (2 + S_MC_SSRCONTROL_SCRATCHPAD)
//
// Channel mapping macros...
//
#define DEF_SPARE_CHANNEL                     2
#define L2P(l,p)                              (1 << p)  // 'l' represents logical channel
                                                        // 'p' represents physical channel

#define DONT_CARE                             0xFF

#define EFI_INVALID_APICID  0xFF

//
// Platform Memory Changes
//

#define PLATFORM_MAX_NODES     2
#define PLATFORM_MAX_CHANNELS  3
#define PLATFORM_MAX_DIMMS     3   // DIMMs per Channel

#define PLATFORM_MAX_ARRAYS    PLATFORM_MAX_NODES
#define PLATFORM_MAX_BANKS     3
#define PLATFORM_MAX_DEVICES   3

//
// Corrected error thresholds
//

#define PLATFORM_PER_RANK_THRESHOLD_DURATION_HOURS 24
#define PLATFORM_ERROR_COUNT_THRESHOLD 80

//
// MCA memory error code TODO
//
#define MCA_MEMORY_ERROR                          0xFFF
//
// iBMC Mailbox Object Interface...
//
#define SZ_GENERIC_BMC_MAILBOX_OBJECT       8

#define EFI_NULL_TIMESTAMP              0xFFFFFFFF
#define EFI_BMC_SEL_COUNTER_THRESHOLD   (1 << 6)
#define EFI_NV_COUNTER_ENTRIES          50

#define SZ_GENERIC_BMC_MAILBOX_MEM_DATA_OBJ   0

typedef union {
   struct{
      UINT64 bank_count:8;
                        /* number of MCA banks */
      UINT64 mcg_ctl_p:1;
                        /* control MSR present */
      UINT64 mcg_ext_p:1;
                        /* extended MSRs present */
      UINT64 mcg_cmci_p:1;
                        /* corrected MC error counting/signaling present */
      UINT64 mcg_tes_p:1;
                        /* threshold based error status present */
      UINT64 rsvd1:4;
      UINT64 mcg_ext_cnt:8;
      UINT64 mcg_ser_p:1;
                        /* software error recovery support present */
      UINT64 rsvd2:39;     
   } Bits;
   UINT64 Data;
} IA32_MCG_CAP_MSR_STRUCT;


typedef union {
   struct{
      UINT16 McBankMsrType;   // 01 - HA McBank, 02 - iMc Mcbank; 00 - default;
      UINT16 McaCod;
	  UINT16 MsCod;
   } Bits;
   UINT64 Data;
} DIMM_ISOLATION_ERROR_TYPE_STRUCT;



typedef struct _EFI_LOCAL_APICID_INFO {
  UINT8     ApicId;
  BOOLEAN   Valid;
} EFI_LOCAL_APICID_INFO;

#pragma pack(1)

typedef struct _ADDRESS_TRANSLATION {
  UINT64 physicalAddress;
  UINT64 channel_address;
  UINT8 nodeId;
  UINT8 channel;
  UINT8 channel_2;
  UINT8 physicalRankId;
  UINT8 dimmSlot;
} ADDRESS_TRANSLATION, *PADDRESS_TRANSLATION;

#pragma pack()

typedef struct _EFI_ERROR_TIME_STAMP {
  UINT32  TimeStamp[EFI_NV_COUNTER_ENTRIES];
  UINT32  Count;
} EFI_ERROR_TIME_STAMP;


typedef enum {
  EfiGenericMailboxMemoryAddressObject = 0,
  EfiGenericMailboxInvalidObject = 0xFFFFFFFF
} EFI_GENERIC_BMC_MAILBOX_OBJECT_TYPE;

#pragma pack(1)
typedef union _EFI_MEMORY_MACHINE_CHECK_ADDR_OBJECT {
  UINT64    Value;
} EFI_MEMORY_MACHINE_CHECK_ADDR_OBJECT;

typedef union _EFI_GENERIC_BMC_MAILBOX_OBJECT_DATA {
  UINT8   Raw[SZ_GENERIC_BMC_MAILBOX_OBJECT];
  EFI_MEMORY_MACHINE_CHECK_ADDR_OBJECT  McMemAddrMsrObj;
} EFI_GENERIC_BMC_MAILBOX_OBJECT_DATA;

typedef struct _EFI_GENERIC_BMC_MAILBOX_OBJECT {
  EFI_GENERIC_BMC_MAILBOX_OBJECT_DATA   Data;
  UINT8                         Size;
  UINT8                         Type;
} EFI_GENERIC_BMC_MAILBOX_OBJECT;

#pragma pack ()

extern ERROR_LOG_SETUP_DATA  *mErrLogSetupData;
extern EFI_MEM_RAS_PROTOCOL  *mMemRas;
extern EFI_SMBIOS_PROTOCOL   *mSmbiosProtocol;
extern UINT8 mNumIoh;


EFI_STATUS
ProcessFatalAndViralError (
  VOID
);

EFI_STATUS
ElogProcessorErrorHandler (
  VOID
);


VOID
PrepeareElogRecordForMemoryCorrError(
  UINT8 SocketBusNum
);

BOOLEAN
DetectNodeSocketChannelChange (
VOID
);

VOID
UpdateSmmELogHostGlobals (
	VOID
);

VOID
EnableDisableGlobalSMIGenerationPerSocket (
	UINT8 Socket
);

//BOOLEAN SocketInViralState(
	//void
	//);

VOID
UncorrectedMCBankErrorHandler ( 
  UINT8 McBank,
  UINT8 *skt,
  SMM_ERROR_CPU_DATA_BLOCK *CpuData,
  GENERIC_ERROR_STATUS *ErrSts
  );

UINT32
MapErrorSeverity
(
 EMCA_MC_SIGNATURE_ERR_RECORD *McErrSig
);

VOID
ProcessCoreUncorrectedError
(
  UINT8 *skt
);

VOID
ProcessUncoreUncorrectedError
(
  UINT8 *skt
);

EFI_STATUS
ProcessSocketUncorrectedError
(
IN UINT8 skt
);

VOID
ExtractProcessorLocation(
  IN UINT32 ApicId,
  OUT EFI_CPU_PHYSICAL_LOCATION *Location
);



VOID
ElogMcExecuteUncorrectableErrorHandlingPolicies (
  MEMORY_COMBINED_ERROR_RECORD    *MemoryErrRecord
  );

VOID
EnableSMIForCorrectedMemoryErrors (
  UINT8 node
  );

VOID
EnableDisableINTForEccCorrectedMemErrors (
  UINT8 Node
  );

VOID
ClearUboxAndMiscErrorStatusPerSkt (
  UINT8 Socket
  );

VOID
EnableDisableSMIForUncorrectedErrors (
  UINT8 Socket
  );

VOID
ProgramViralPoison(
  );

VOID
ProcessorCatastrophicErrorHandler (
  UINT8 node
  );

/*VOID
ClearJcErrors (
  UINT8 node, 
  UINT8	VmseCh
  );

VOID
ClearJcLogREccError (
  UINT8 node, 
  UINT8	VmseCh
  );*/

VOID
ExtractNodeCoreFromApicId (
  OUT UINT8 *Socket,
  OUT UINT8 *Core
  );

BOOLEAN
ExcutingOnLocalNode (
  UINT8 Socket
  );

UINTN
GetCpuNumber (
  UINT8 Socket,
  UINT8 Core,
  UINT8 Thread
);

BOOLEAN
ExcutingOnLocalNodeCore (
  UINT8 Socket,
  UINT8 Core
  );


VOID
ClearPCUErrors(
  UINT8 Socket
  );

VOID
ClearQPIErrors(
  VOID
);	

/*VOID
EnableQPIViral(
  VOID
);

VOID
EnableUBoxViral(
	VOID
);*/

EFI_STATUS
EnablePoisonIoh (
  IN UINT8 ioh
  );

/*EFI_STATUS
IIOEnableViral (
  IN UINT8 IIO
  );

VOID
ProgramViralPoison(
  VOID
  );*/

BOOLEAN
PendingProcessorErrorExists (
  UINT32 *UmcSockets,
	UINT32 *ImcSockets,
  UINT32 *ImcNodesEventProgress,
  UINT32 *ImcNodesNewEvents
  );

UINT32
GetApicID (
  VOID
);

EFI_STATUS
FindFirstSet(
UINT64 McBankBitField,
UINT8 *Idx
);

VOID
ClearUncoreMcBankBitField(
  UINT64 McBankBitField
);

VOID
ClearCoreMcBankBitField(
  UINT64 McBankBitField
);



UINT32
ReadLocalApicRegrep (
  IN UINTN  MmioOffset
  );


VOID
ClearPCUErrorsEp();

#endif  // _PROCESSOR_ERROR_LOG_H_

