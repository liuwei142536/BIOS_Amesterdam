/** @file
  This is eMCA platform hooks library interface 

  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

  Copyright (c) 2009-2014 Intel Corporation.  All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license.  Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

**/

#ifndef _EMCA_PLATFORM_HOOKS_LIB_H_
#define _EMCA_PLATFORM_HOOKS_LIB_H_

//
// Includes
//
#include <mca_msr.h>
#include <Uefi.h>
#include <emca.h>
#include <Library\BaseLib.h>
#include <Library/DebugLib.h>
#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include "Platform.h"
#include <Protocol/MpService.h>
#include <Cpu/CpuRegs.h>
#include <Library/SynchronizationLib.h> 




//
// Defines
//
#define MC_SCOPE_TD    0x0
#define MC_SCOPE_CORE  0x1
#define MC_SCOPE_PKG   0x2
#define MCSCOPE_MASK  (BIT0 | BIT1)

#define EMCA_LOG_SUPPORT (BIT2)

#define EMCA_SECTYPE_NONE       0x0
#define EMCA_SECTYPE_PROCGEN    0x1
#define EMCA_SECTYPE_PROCIA32   0x2
#define EMCA_SECTYPE_PROCIPF    0x3
#define EMCA_SECTYPE_PLATMEM    0x4
#define EMCA_SECTYPE_PCIE       0x5
#define EMCA_SECTYPE_FWERR      0x6
#define EMCA_SECTYPE_PCIBUS     0x7
#define EMCA_SECTYPE_PCIDEV     0x8
#define EMCA_SECTYPE_DMAR       0x9
#define EMCA_SECTYPE_VTIO       0xA
#define EMCA_SECTYPE_IOMMU      0xB
#define EMCA_SECTYPE_RAW        0xC

#define EMCA_ENABLE_SWSMI       0x8E
#define EMCA_DISABLE_SWSMI      0x8D

#define MCA_UNIT_TYPE_IFU  (BIT0)
#define MCA_UNIT_TYPE_DCU  (BIT1)
#define MCA_UNIT_TYPE_DTLB (BIT2)
#define MCA_UNIT_TYPE_MLC  (BIT3)
#define MCA_UNIT_TYPE_PCU  (BIT4)
#define MCA_UNIT_TYPE_QPI  (BIT5)
#define MCA_UNIT_TYPE_IIO  (BIT6)
#define MCA_UNIT_TYPE_HA   (BIT7)
#define MCA_UNIT_TYPE_IMC  (BIT8)
#define MCA_UNIT_TYPE_CBO  (BIT9)
#define MCA_UNIT_TYPE_EDRAM    (BIT10)

#define MCA_UNIT_IFU0         (MCA_UNIT_TYPE_IFU)
#define MCA_UNIT_DCU0         (MCA_UNIT_TYPE_DCU)
#define MCA_UNIT_DTLB0        (MCA_UNIT_TYPE_DTLB)
#define MCA_UNIT_MLC0         (MCA_UNIT_TYPE_MLC)

#define MCA_UNIT_PCU0_QPI2    (MCA_UNIT_TYPE_PCU | MCA_UNIT_TYPE_QPI)
#define MCA_UNIT_QPI0_QPI1    (MCA_UNIT_TYPE_QPI)

#define MCA_UNIT_PCU0         (MCA_UNIT_TYPE_PCU)
#define MCA_UNIT_QPI0         (MCA_UNIT_TYPE_QPI)
#define MCA_UNIT_QPI1         (MCA_UNIT_TYPE_QPI)
#define MCA_UNIT_QPI2         (MCA_UNIT_TYPE_QPI)
#define MCA_UNIT_EDRAM0       (MCA_UNIT_TYPE_EDRAM)
#define MCA_UNIT_EDRAM1       (MCA_UNIT_EDRAM0)
#define MCA_UNIT_EDRAM2       (MCA_UNIT_EDRAM0)
#define MCA_UNIT_EDRAM3       (MCA_UNIT_EDRAM0)
#define MCA_UNIT_HA2          (MCA_UNIT_TYPE_HA)
#define MCA_UNIT_HA3          (MCA_UNIT_TYPE_HA)

#define MCA_UNIT_IIO0         (MCA_UNIT_TYPE_IIO)
#define MCA_UNIT_HA0          (MCA_UNIT_TYPE_HA)
#define MCA_UNIT_HA1          (MCA_UNIT_TYPE_HA)
#define MCA_UNIT_IMC0         (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_IMC1         (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_IMC2         (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_IMC3         (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_IMC4         (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_IMC5         (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_IMC6         (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_IMC7         (MCA_UNIT_TYPE_IMC)
#define MCA_UNIT_CBO00        (MCA_UNIT_TYPE_CBO)
#define MCA_UNIT_CBO01        (MCA_UNIT_TYPE_CBO)
#define MCA_UNIT_CBO02        (MCA_UNIT_TYPE_CBO)
#define MCA_UNIT_CBO03        (MCA_UNIT_TYPE_CBO)
#define MCA_UNIT_CBO04        (MCA_UNIT_TYPE_CBO)
#define MCA_UNIT_CBO05        (MCA_UNIT_TYPE_CBO)
#define MCA_UNIT_CBO06        (MCA_UNIT_TYPE_CBO)
#define MCA_UNIT_CBO07        (MCA_UNIT_TYPE_CBO)
#define MCA_UNIT_CBO08        (MCA_UNIT_TYPE_CBO)
#define MCA_UNIT_CBO09        (MCA_UNIT_TYPE_CBO)
#define MCA_UNIT_CBO10        (MCA_UNIT_TYPE_CBO)
#define MCA_UNIT_CBO11        (MCA_UNIT_TYPE_CBO)
#define MCA_UNIT_CBO12        (MCA_UNIT_TYPE_CBO)
#define MCA_UNIT_CBO13        (MCA_UNIT_TYPE_CBO)
#define MCA_UNIT_CBO14        (MCA_UNIT_TYPE_CBO)


///
/// Forward declaration.
///
typedef struct _MCA_BANK_HOOKS MCA_BANK_HOOKS;

typedef
EFI_STATUS
(EFIAPI *MCA_AGREGATE_CSR_INFO_HOOK)
(
  IN MCA_BANK_HOOKS *This,
  IN UINT32 McBankIdx,
  IN UINT8 *Skt,
  IN EMCA_MC_SIGNATURE_ERR_RECORD *McSig,
  IN OUT VOID *Section 
);

typedef
EFI_STATUS
(EFIAPI *MCA_READ_MCBANK_SIG_HOOK) 
(
  IN UINT32 McBankIdx,
  IN UINT8 skt,    
  OUT EMCA_MC_SIGNATURE_ERR_RECORD * McSig
);


struct _MCA_BANK_HOOKS {
  MCA_AGREGATE_CSR_INFO_HOOK  CsrInfoHook;
};

typedef
BOOLEAN
(EFIAPI *MCA_RECOVERY_HANDLER_HOOK) 
(
  IN UINT32 McBankIdx,    
  OUT EMCA_MC_SIGNATURE_ERR_RECORD * McSig
);

typedef struct {
  SPIN_LOCK             ApBusy;
} EMCAMSMI_ARGS_STRUCT;


//
// Type Definitions
//
//
typedef struct {
  UINT32 CtlMsr;
  UINT32 Ctl2Msr;
  UINT16 Attrib;
  UINT16 SecType;
  UINT32 UnitType;
  MCA_READ_MCBANK_SIG_HOOK ReadMcBankSig;
  MCA_BANK_HOOKS Hooks;
} EMCA_MCBANK_DESC; 

typedef struct {
  UINT16 McaCod;
  UINT16 McaCodMsk;
  UINT16 MsCod;
  UINT16 MsCodMsk;
  MCA_RECOVERY_HANDLER_HOOK RecoveryHdr;
} EMCA_MCRECOVERY_DESC;


typedef struct {
  UINT32 McBankNum;
  UINT8 Instance;
} HA_LOOKUP;

typedef struct {
  UINT32 McBankNum;
  UINT8 MemoryControllerId;
} IMC_NODE_LOOKUP;

typedef struct {
  UINT16 McaCod;
  UINT16 McaCodMsk;
  UINT16 MsCod;
  UINT16 MsCodMsk;
  UINT8 ErrorType;
} EMCA_CSR_INFO_DESC;

typedef struct {
  SPIN_LOCK Busy;
} CMCI_GEN2_PARAMS;
  
EFI_STATUS
MCBankCsrInfoHook(
  IN UINT32 McBankIdx,
  IN UINT8 *Skt,
  IN EMCA_MC_SIGNATURE_ERR_RECORD *McSig,
  IN OUT VOID *Section 
);

EFI_STATUS
EFIAPI 
ReadMcBankSigHook(
  IN UINT32 McBankIdx,
  IN UINT8 skt,    
  OUT EMCA_MC_SIGNATURE_ERR_RECORD * McSig
);

BOOLEAN
AttemptRecovery(
IN UINT32 McBankIdx,
IN UINT8 Skt,
IN OUT EMCA_MC_SIGNATURE_ERR_RECORD *McSig
);

VOID
WriteMcBank
(
  IN UINT32 McBankIdx,
  IN OUT EMCA_MC_SIGNATURE_ERR_RECORD *McSig
);




//
// Type Definitions
//
//


//
// Functions prototype declarations 
//

/**
  Get the number of MCBANKS_PER_LP.
  
  MCBANKS_PER_LP = MCBANK_CORE + MCBANKS_UNCORE = MCG_CAP.Count
  
  @return MCBANKS_PER_LP

**/
UINT64 
GetNumMcBanksPerLp(
  VOID
  );

/**
  Get the number of L1 Dir Entries required based on the MC Banks available and number of sockets.

  Number of L1 Dir Entries = SOCKET_COUNT * ( 2 ** APICID_BITS_PER_SOCKET) * MCBANKS_PER_LP where: 
  
  SOCKET_COUNT is  the maximum number  of  processor  sockets in  the platform,
  APICID_BITS_PER_SOCKET is the number of APIC ID bits used up by each socket  
  MCBANKS_PER_LP = MCBANK_CORE + MCBANKS_UNCORE = MCG_CAP.Count
  
  @return number of L1 Dir Entries  

**/
UINT64 
GetNumL1DirEntries(
  VOID
  );

/**
  Get the number of Elog Dir(L2) Entries required based on the MC Banks in the system.

 NumElogentries = number of physical MC BANKS in the system
                      = NumSKTSupported * NumMCBanksPerSKT
                      = NumSKTSupported * (NumCoreMcBanks *  CORES_PER_SOCKET + NumUncoreMcBanks ) 
  
  @return Number of Elog Entries 

**/
UINT64 
GetNumElogEntries(
  VOID
  );

/**
  Get if emca log is supported by a particular bank  
  
  @return True or False

**/
BOOLEAN
isEMCALogSupported(
  UINT32 McBankIdx
  );

BOOLEAN
isEmcaMemoryBank(
  UINT32 McBankIdx
  );

/**
  Get the scope of MCi Bank. The scope can be thread level, core level and package level  
  
  @return MCi Bank Scope

**/
UINT16
GetMcBankScope(
  UINT32 McBankIdx
  );

UINT32
  GetAddrMcBankCtl
(
  UINT32 McBankIdx
);

UINT32
GetAddrMcBankSts
(
  UINT32 McBankIdx
);

UINT32
GetAddrMcBankAddr
(
  UINT32 McBankIdx
);

UINT32
GetAddrMcBankMisc
(
  UINT32 McBankIdx
);


UINT32
GetAddrMcBankCtl2
(
  UINT32 McBankIdx
);

UINT16
GetMcBankSecType
(
  UINT32 McBankIdx
);


/**
  Get the apicid for logical processor location. (Socket, Core, thread).   
  
  @return apicid

**/
UINT32 
GetApicIdByLoc (
  IN EFI_CPU_PHYSICAL_LOCATION *Location
  );

EFI_STATUS 
GetCoreAndSMTBits (
  IN UINTN *ThreadBits,
  IN UINTN *CoreBits
);

VOID
DisableCSMIGen1(
UINT8 *node
);

VOID 
DisableCSMIGen2(
CMCI_GEN2_PARAMS *Params
);

void
EnableCSMIGen1(
UINT8 *node
);

VOID
EnableCSMIGen2(
CMCI_GEN2_PARAMS *Params
);


VOID
EmcaEnableMSMI(
  UINT32 *McBankIdx
);

VOID
EmcaMSMICoreBanks(
  VOID  *Buffer
);

VOID
EmcaMSMIPkgBanks(
  VOID  *Buffer
);


EFI_STATUS
EFIAPI
EmcaPlatformHooksLibConstructor (
IN EFI_HANDLE ImageHandle,
IN EFI_SYSTEM_TABLE *SystemTable
);

EFI_STATUS
ReadEnhancedMCALogMsr(
IN OUT VOID *Buffer
);


EFI_STATUS
GetEMCALogProcCap(
IN OUT BOOLEAN *EMcaLiteCap
);

BOOLEAN
isEMCAGen2Cap(
VOID
);

UINT64
GetEMcaBankSupp(
VOID
);


EFI_STATUS
InitL1PtrForCores(
  IN EMCA_L1_HEADER *L1DirHdr,
  IN UINT32 McBank,
  IN OUT VOID **NxtElog
);

EFI_STATUS
InitL1PtrForThreads(
  IN EMCA_L1_HEADER *L1DirHdr,
  IN UINT32 McBank, 
  IN OUT VOID **NxtElog
);

EFI_STATUS
InitL1PtrForSockets(
  IN EMCA_L1_HEADER *L1DirHdr,
  IN UINT32 McBank, 
  IN OUT VOID **NxtElog
);

UINT64
ReadCoreSmiSrc(
VOID
);

VOID
ClearCoreSmiSrc(
IN VOID *Buffer
);

UINT64
ReadUncoreSmiSrc(
VOID
);

VOID
ClearUncoreSmiSrc(
IN VOID *Buffer
);

#endif  // _EMCA_PLATFORM_HOOKS_LIB_H_

