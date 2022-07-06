/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//

Copyright (c) 2009 - 2017 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  SmmErrorHandler.h

Abstract:


------------------------------------------------------------------*/

#ifndef _SMM_ERROR_HANDLER_H
#define _SMM_ERROR_HANDLER_H

#ifndef EFI_RUNTIMESERVICE
#define EFI_RUNTIMESERVICE
#endif
#ifndef __FFS__H__
#define __FFS__H__
#endif
#ifndef __PEI__H__
#define __PEI__H__
#endif
#ifndef __EFI__H__
#define __EFI__H__
#endif

//
// UBOX SMI Signalling Threshold
//
#define UBOX_SMI_THRESHOLD_DEFAULT        0x0FFF

// All #defines related to SMMErrorLog should go here
#define EFI_MEMORY_SENSOR_TYPE  0x0C
#define SEL_SYSTEM_RECORD       0x2

#define EFI_CRITICAL_INT_SENSOR_TYPE  0x13

#define EFI_PCI_BUS_NUM_MASK          0xFF
#define EFI_PCI_DEV_NUM_MASK          0xF8
#define EFI_PCI_FUNC_NUM_MASK         0x03

#define R_ICH_PCIEXP_DEVICE_CONTROL     0x48    // Device Control Register
#define R_ICH_PCIEXP_SYSTEM_CONTROL     0x5C    // System Control Register

//
// PCI-Ex AER registers
//
#define R_PCIEXP_AER_UES              0x104   // Uncorrectable error status
#define V_PCIEXP_AER_UES              (BIT4+BIT12+BIT14+BIT15+BIT16+BIT17+BIT18+BIT20)   // Uncorrectable error status or value
#define R_PCIEXP_AER_UEM              0x108   // Uncorrectable error mask
#define V_PCIEXP_AER_UEM              (BIT4+BIT12+BIT14+BIT15+BIT16+BIT17+BIT18+BIT20)   // Uncorrectable error mask
#define R_PCIEXP_AER_UEV              0x10C   // Uncorrectable error severity
//#define V_PCIEXP_AER_UEV              (BIT0+BIT4+BIT12+BIT13+BIT14+BIT15+BIT16+BIT17+BIT18+BIT19+BIT20)   // Uncorrectable error severity
#define V_PCIEXP_AER_UEV              (BIT4+BIT5+BIT12+BIT13+BIT14+BIT15+BIT16+BIT17+BIT18+BIT20+BIT21)   // Uncorrectable error severity

#define R_PCIEXP_AER_CES              0x110   // Correctable error status
#define V_PCIEXP_AER_CES              (BIT0+BIT6+BIT7+BIT8+BIT12+BIT13) // Correctable error status or value
#define R_PCIEXP_AER_CEM              0x114   // Correctable error mask
#define V_PCIEXP_AER_CEM              (BIT0+BIT6+BIT7+BIT8+BIT12+BIT13)   // Correctable error mask

#define R_PCIEXP_AER_REC              0x12c   // Root error command
#define R_PCIEXP_AER_RES              0x130   // Root error status
#define R_PCIEXP_AER_CAPID            0x0001

#define PCIE_AER_OFFSET_REG_AER_CTRL  0x18
//
// ESS_OVERRIDE_START
//
#define EFI_PCI_STATUS_DETECTED_PARITY_ERROR        BIT15 // 0x8000
#define EFI_PCI_STATUS_SIGNALED_SYSTEM_ERROR        BIT14 // 0x4000
#define EFI_PCI_STATUS_RECEIVED_MASTER_ABORT        BIT13 // 0x2000
#define EFI_PCI_STATUS_RECEIVED_TARGET_ABORT        BIT12 // 0x1000
#define EFI_PCI_STATUS_DATA_PARITY_ERROR_DETECTED   BIT8  // 0x0100
//
// ESS_OVERRIDE_END
//

//
// General Info related to SEL Record
//
#define EFI_EVM_REVISION    0x04
#define EFI_BIOS_ID         0x18
#define EFI_FORMAT_REV      0x00
#define EFI_FORMAT_REV1     0x01
#define EFI_SOFTWARE_ID     0x01
//#define EFI_GENERATOR_ID(a) ((EFI_BIOS_ID << 1) | (a << 1) | EFI_SOFTWARE_ID) //AptioV Server override: Comment to resolve redefinition error
//
// This is a discrete sensor
//
#define EFI_SENSOR_TYPE_EVENT_CODE_DISCRETE 0x6F

//
// catagorized error Types for logging and viewing.
//
#define INVALID_ERROR_TYPE        0x00
#define NONFATAL_ERROR_TYPE       0x01
#define FATAL_ERROR_TYPE          0x02
#define COR_ERROR_TYPE            0x03
#define SERROR_TYPE               0x04
#define PERROR_TYPE               0x05
#define DPERROR_TYPE              0x06
#define REC_MASTER_ABORT_TYPE     0x07
#define REC_TARGET_ABORT_TYPE     0x08
#define REC_SERROR_TYPE           0x09
#define REDUNDANCY_ERROR_TYPE     0x0A
#define LEGACY_ERROR_TYPE         0x0B
#define EXTENDED_ERROR_TYPE       0x0C
#define RECOVERABLE_ERROR_TYPE    0x0D
#define MISCERROR_TYPE            0xEE

//
// Dallas DS12C887 Real Time Clock
//
#define RTC_ADDRESS_SECONDS           0   // R/W  Range 0..59
#define RTC_ADDRESS_MINUTES           2   // R/W  Range 0..59
#define RTC_ADDRESS_HOURS             4   // R/W  Range 1..12 or 0..23 Bit 7 is AM/PM
#define RTC_ADDRESS_DAY_OF_THE_MONTH  7   // R/W  Range 1..31
#define RTC_ADDRESS_MONTH             8   // R/W  Range 1..12
#define RTC_ADDRESS_YEAR              9   // R/W  Range 0..99
#define RTC_ADDRESS_REGISTER_A        10  // R/W[0..6]  R0[7]
#define RTC_ADDRESS_REGISTER_B        11  // R/W
#define RTC_ADDRESS_REGISTER_C        12  // RO

#define IIO_CORRECTABLE 0x0
#define IIO_RECOVERABLE 0x1
#define IIO_FATAL       0x2

//
// SMI delay relative to MCA for UMC errors
//

#define SMM_ELOG_UMCSMI_DELAY 0xFFF 

//Offset for SIGNAL_MCE field in SMM save state 
#define SIGNAL_MCE_OFFSET           0xFE38
#define CMCI_LVT_OFFSET             0x2f0
#define EVENTS_CTRL_OFFSET                      0xFF04
#define EFI_MSR_HASWELL_EVENT_CTL_HLT_IO        0xC1F

#define INVALID_FLAG                0xFF
#define DEFAULT                     0x02	// match with default value of the Setup option

// Bit mask
#define SPARE_INT_NONE 0
#define SPARE_INT_SMI  1
#define SPARE_INT_PIN  2
#define SPARE_INT_CMCI 4

//
#define SKIP_MC_CE_CNT_FLAG  BIT1
#define MAX_ERROR_EVENTS MaxErrSrc


#define MAXCOREBITS (5)

#define MAXCOREID (1<<MAXCOREBITS)
// AptioV Server Override Start: Support added for Mode1 Memory Error Reporting
#define MC_ERR_INJ_LCK                             0x790
#define MC_ERROR_CONTROL                   0x17f
// AptioV Server Override End: Support added for Mode1 Memory Error Reporting
/*
#ifdef  CRB_FLAG
#define REMAP1_OFFSET	0x10
#define REMAP2_OFFSET   0x14
#define REMAP3_OFFSET   0x18
#define REMAP4LOW_OFFSET  0x1c
#define REMAP4HIGH_OFFSET  0x20
#define PCIINTERRUPT_LINE__OFFSET  0x3c
#define PCIEXP_LINKCTRL_OFFSET 0x50
#define LINK_DBG_CTRL_OFFSET 0x230
#define DFS1REGISTER_OFFSET  0x6DC
#define TLCOMPLETION_TIMEOUT_OFFSET 0x248
#define MAX_PCI_DEVICES 32
#define MAX_PCI_FUNCTIONS 8
#define EFI_LER_ENABLE_SWSMI  0x8e
#endif
*/

#define EFI_WMAA_UR_SUPPORT_SWSMI    0x97

// All data type definitions related to SMMErrorLog should go here

//
// Defines Related to Memory
//
typedef enum {
  EFI_MEMORY_SENSOR_OFFSET_ECC_CORRECTABLE    = 0,
  EFI_MEMORY_SENSOR_OFFSET_ECC_UNCORRECTABLE,
  EFI_MEMORY_SENSOR_OFFSET_PARITY,
  EFI_MEMORY_SENSOR_OFFSET_SCRUB_FAILED,
  EFI_MEMORY_SENSOR_OFFSET_MEM_DEVICE_DISABLED,
  EFI_MEMORY_SENSOR_OFFSET_ECC_CORRECTABLE_MAX,
} EFI_SEL_SENSOR_MEMORY_OFFSET;


//
// Defines related to Critical Interrupt.  All the PCI Errors belong to this.
//
typedef enum {
  EFI_CRITICAL_INT_FRONT_PANEL_NMI            = 0,
  EFI_CRITICAL_INT_BUS_TIME_OUT,
  EFI_CRITICAL_INT_IOCHK_NMI,
  EFI_CRITICAL_INT_SOFTWARE_NMI,
  EFI_CRITICAL_INT_PCI_PERR,
  EFI_CRITICAL_INT_PCI_SERR,
  EFI_CRITICAL_INT_EISA_FAIL_SAFE,
  EFI_CRITICAL_INT_BUS_CORR_ERROR,
  EFI_CRITICAL_INT_BUS_UNCORR,
  EFI_CRITICAL_INT_FATAL_NMI,
} EFI_SEL_SENSOR_CRITICAL_INT_OFFSET;

//
// Defines for CB (DMA) Errors.
//
typedef enum {
  EFI_DMA_TRANSADD_ERR                        = 0,  // * Transfer Soruce Addres Error
  EFI_DMA_TRANSDST_ERR,                             // * Transfer Destination Address Error
  EFI_DMA_NDESADDR_ERR,                             // * Next Descriptor Address Error
  EFI_DMA_DESCRPT_ERR,                              // * Descriptor Error
  EFI_DMA_CHADDVAL_ERR,                             // * Chain Addres Value Error
  EFI_DMA_CHANCMD_ERR,                              // * Channel Command Error
  EFI_DMA_CDPAR_ERR,                                // * Chipset Data Parity Error
  EFI_DMA_DMAPAR_ERR,                               // * DMA Data Parity Error
  EFI_DMA_RDATA_ERR,                                // * Read Data Error
  EFI_DMA_WRTDATA_ERR,                              // * Write Data Error
  EFI_DMA_DESCONT_ERR,                              // * Descriptor Control Error
  EFI_DMA_DESLEN_ERR,                               // * Descriptor Length Error
  EFI_DMA_COMPADD_ERR,                              // * Completion Address Error
  EFI_DMA_INTCFG_ERR,                               // * Interrupt Configuration Error
  EFI_DMA_SFTERROR_ERR,                             // * Soft Error
  EFI_INT_UNAFFIL_ERR                               // * Unaffiliated error
} EFI_DMA_ERROR_SOURCE;

//
// Defines for IOH Internal Errors.
//
typedef enum {
  EFI_INT_CEPE_ERR                            = 0,  // * CE Parity Error
  EFI_INT_MULTAGHIT_ERR,                            // * Multi-Tag Hit SF
  EFI_INT_COHVIOL_ERR,                              // * Coherency Violation Error for EWB
  EFI_INT_VIRPIN_ERR,                               // * Virtual Pin Error (VPP_PEX, VPP_FBD)
  EFI_INT_ADDMAP_ERR,                               // * Address Map Error (COH)
  EFI_INT_SNGECC_ERR,                               // * Single ECC Error on SF lookup (SF)
  EFI_INT_MULECC_ERR,                               // * Multiple ECC Error in ano of the ways during SF lookup
  EFI_INT_SFCOHBIL_ERR,                             // * SF Coherency Error for BIL (SF)
  EFI_INT_B12_ERR,
  EFI_INT_B14_ERR
} EFI_INT_ERROR_SOURCE;



//
// EFI SMM Time Abstraction:
//  Year:       2000 - 20XX
//  Month:      1 - 12
//  Day:        1 - 31
//  Hour:       0 - 23
//  Minute:     0 - 59
//  Second:     0 - 59
//
typedef struct {
  UINT32  Year;
  UINT8   Month;
  UINT8   Day;
  UINT8   Hour;
  UINT8   Minute;
  UINT8   Second;
} EFI_SMM_TIME;

//AptioV Server Override Start: Changes done to Support IOHCore/QPI/VTD Errors
/*typedef enum {
  Unknown       = 0,
  PciSource ,
  CsiSource,
  MemSource,
  ProcSource,
  UncoreSource
} ERROR_SOURCE;*/

typedef enum {
  Unknown       = 0,
  PciSource ,
  CsiSource,
  MemSource,
  ProcSource,
  UncoreSource,
  IohQpiSource,
  IohCoreSource,
  IohVtdSource,
  CoreSource,
  QpiSource,
  CboSource
} ERROR_SOURCE; 
// AptioV Server Override End: Changes done to Support IOHCore/QPI/VTD Errors

typedef struct {
  UINT8       ErrorSource;        // PCI or CSI or Mem or Processor etc..
  UINT8       ErrorType;          // PCI
  UINT8       Segment;            // Segment
  UINT8       Bus;                // Bus
  UINT8       Device;             // Device
  UINT8       Function;           // Func
  UINT16      Reg;                // Register Number
  UINT64      Value;              // Value
  UINT16      OEMEvData1;         // More error info
} PCI_PCIE_ERROR_LOCAL_STRUC;

typedef struct {
  UINT8       ErrorSource;        // PCI or CSI or Mem or Processor etc..
  UINT8       ErrorType;          // PCI
  //
  // Add CSI required error display info for setup display
  //
} CSI_ERROR_LOCAL_STRUC;

typedef struct {
  UINT8       ErrorSource;        // PCI or CSI or Mem or Processor etc..
  UINT8       ErrorType;          // PCI
  //
  // Add CSI required error display info for setup display
  //
} MEM_ERROR_LOCAL_STRUC;

typedef struct {
  UINT8       ErrorSource;        // PCI or CSI or Mem or Processor etc..
  UINT8       ErrorType;          // PCI
  //
  // Add Proc required error display info for setup display
  //
} PROC_ERROR_LOCAL_STRUC;


//
// ICH device structure
//
typedef struct  {
  UINT8                   Segment;
  UINT8                   Bus;
  UINT8                   Device;
  UINT8                   Function;
  BOOLEAN                 Bridge;

} ICH_PCI_DEVICES;

typedef struct  {
  UINT8                   Segment;
  UINT8                   Bus;
  UINT8                   Device;
  UINT8                   Function;
} ICH_PCIE_DEVICES;

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


typedef enum {
  PCIExpressEndPoint  = 0,
  LegacyPCIEndPointDevice,
  RootPort,
  UpstreamSwitchPort,
  DownstreamSwitchPort,
  PCIExpresstoPCI_PCIXBridge,
  PCI_PCIXtoPCIExpressBridge,
  RootComplexIntegratedEndpointDevice,
  RootComplexEventCollector
} EFI_PCIE_ERROR_SOURCE_PORT;


//
// CMCI LVT
//
/*This structure is similer to LVT struct found in LocalApic.h*/
typedef union {
  struct {
    UINT32  Vector:8;          ///< The vector number of the interrupt being sent.
    UINT32  DeliveryMode:3;
    UINT32  Reserved0:1;       ///< Reserved.
    UINT32  DeliveryStatus:1;  ///< 0: Idle, 1: send pending.
    UINT32  Reserved1:3;       ///< Reserved.
    UINT32  Mask:1;            ///< 0: Not masked, 1: Masked.
    UINT32  Reserved2:14;      ///< Reserved.
  } Bits;
  UINT32    Uint32;
} LOCAL_APIC_LVT_CMCI;

// Whea Variables 

typedef struct _ERROR_LOG_SETUP_DATA {
  UINT8   SystemErrorEn;
  UINT8   WheaSupport;
  UINT8   WheaLogging;
  UINT8   ErrorPinProgrammingForIIO;
#if defined(BDX_HOST) && defined(DE_SKU) 
  UINT8   VtdErrorEn;
  UINT8   DmiErrorEn;
  UINT8   MiErrorEn;
  UINT8   IioCoreErrorEn;
  UINT8   DmaErrorEn;
  UINT8   CoherentInterfaceErrorEn;
#endif
  UINT8   UncorrectedErrorsEn;
  UINT8   CorrectedMemoryErrors;
//UINT8	  MemErrorsEn;
  UINT8   IohErrorEn;
  UINT8   PchErrorEn;
  UINT8   PcieErrorsEn;
  UINT8   PropagateSerr;
  UINT8   PropagatePerr;
  UINT8   PCIEFatalErrorEn;
  UINT8   PCIEUnCorrErrorEn;
  UINT8   PCIECorrErrorEn;
  UINT8   PcieCeErrThreshold;
  UINT8   PcieExtndErrorsEn;
  //UINT8   LerEnable;
  //UINT8   DisableC4LoggingDueToLER;
  UINT8   UmcSmiEnable;
  UINT8   Cmci;
  UINT8	  EMcaLogEn;
  UINT8   EMcaCSmiEn;
  UINT8   EMcaForceOptin;
#ifdef BDX_HOST 
  UINT8   CloakingEn;
#endif
  UINT8   MceSmiMorphingEn;
  UINT8   UboxUmcSmiEn;
  UINT16  WheaErrorRecordFormat;
#ifndef GRANGEVILLE_FLAG
  UINT8   MirrorFailOver;
#endif  // GRANGEVILLE_FLAG
  UINT8	  UnlockMsr; // Aptio V Server Override - Unlocking the MSR 0x790 for the ErrorInjection to work from RAS tools
  UINT8	  MirrCorrected;
  UINT8   HaCorrErrSmiEn;
  UINT8   iMcCorrErrSmiEn;
  UINT8	  MceToSmiEn;
  /*UINT8	  VmseLinkFailure;
  UINT8	  VmseErrorObserved;
  UINT8	  VmseDdrParityError;
  UINT8	  VmseNbPersistentError;
  UINT8	  VmseSbPersistentError;*/
  UINT8	  PoisonEn;
  //UINT8   ViralEn;
  UINT16  SmiMcaDelay;
  UINT8   SpareInt;

  //UINT8   IoMcaEn;
  UINT8   QPIFailoverSmiEn;
  UINT8   irpp0_parityError;
  UINT8   irpp0_qtOverflow;
  UINT8   irpp0_unexprsp;
  UINT8   irpp0_csraccunaligned;
  UINT8   irpp0_uncecc;
  UINT8   irpp0_rcvdpoison;
  UINT8   irpp0_crrecc;
  UINT8   irpp1_parityError;
  UINT8   irpp1_qtOverflow;
  UINT8   irpp1_unexprsp;
  UINT8   irpp1_csraccunaligned;
  UINT8   irpp1_uncecc;
  UINT8   irpp1_rcvdpoison;
  UINT8   irpp1_crrecc;
// Aptio V Server Override Start : Changes done to handle the Memory Correctable Erros (Default Memory Handler) when RankSparing/DevTagging setup option is disabled. 
  UINT8   dddcSupport;
  UINT8   DeviceTaggingMode;
  UINT8   enableSparing;  
// Aptio V Server Override End   : Changes done to handle the Memory Correctable Erros (Default Memory Handler) when RankSparing/DevTagging setup option is disabled.  
}ERROR_LOG_SETUP_DATA;
// end of Whea variables

typedef enum {
  McbankSrc,
  CsrSrc,
  PcieSrc,
  PlatformSrc,
  MaxErrSrc  
} SMM_ERROR_SRC;

typedef enum {
  No_Signal,
  SignalMce,
  SignalCmci,
  SignalNmi,
  SignalSci,
  MaxSignalTypes
} SMM_ERROR_EVENT_SIG;

typedef enum {
  SmmMceClearedState = 0,
  SmmMceReadingState,
  SmmMceProcessingState,
  SmmMceWritingState,
  SmmMceLoggingState,
  SmmMceReadingInterrupted,
  SmmMceWritingInterrupted,
  SmmMceProcessingInterrupted,
} SMM_MCE_SYNC_STATE;

typedef enum {
  MceHdrRestart= 0,
  MceHdrReset,
  MceHdrAssert
} SMM_MCE_ENDING_ACTION;


typedef struct {
  BOOLEAN ErrorFound;
  UINT32 Severity;
  UINT32 Source;
  UINT32 BankIdx;
  UINT32 ApicId;
} SMM_ERROR_EVENT_DATA;


typedef struct {
  BOOLEAN ErrorsFound;
  SMM_ERROR_EVENT_DATA Events[MAX_ERROR_EVENTS];
} SMM_ERROR_EVENT_LOG;

/* OPTION 2
typedef struct {
  BOOLEAN ErrorsFound;
  UINT32 FatalCount;
  UINT32 RecoverableCount;
  UINT32 CorrectedCount;
} SMM_ERROR_EVENT_COUNT;

typedef struct {
  BOOLEAN ErrorsFound;
  SMM_ERROR_EVENT_COUNT Events[MaxErrSrc];
}SMM_ERROR_EVENT_LOG;
*/

typedef struct {
  SPIN_LOCK ApBusy;
  volatile BOOLEAN Present;
  volatile BOOLEAN Pcc;
  volatile BOOLEAN Ripv;
  volatile UINT64 CoreProcessingBank;
  volatile UINT64 UncoreProcessingBank;
  volatile UINT64 CoreSmiSrcCp;
  volatile UINT64 UncoreSmiSrcCp;
  volatile UINT64 SmmMceSyncState;  
  volatile SMM_ERROR_EVENT_LOG EventLog;
} SMM_ERROR_CPU_DATA_BLOCK;

typedef struct {
// AptioV Server Override Start: Modified/Hardcoded to resolve build error for now.
// Should be modified as per the value of PcdCpuMaxLogicalProcessorNumber
//  SMM_ERROR_CPU_DATA_BLOCK            CpuData[FixedPcdGet32 (PcdCpuMaxLogicalProcessorNumber)];
  SMM_ERROR_CPU_DATA_BLOCK            CpuData[512];
// AptioV Server Override End
  SPIN_LOCK                     LoggingLock;
  SPIN_LOCK                     SerialLock; //For Debug Purpose. 
  volatile UINT32               Counter;
  volatile UINT32               MonarchIndex;
  volatile BOOLEAN              MceInProgress;
  volatile BOOLEAN              InEmcaHander;
  volatile UINT32               ApExec;
  volatile BOOLEAN              MceSignal;
} SMM_ERROR_MP_SYNC_DATA;


typedef struct _MEMORY_CFG_DATA {
  struct sysNvram nvram;
  UINT64 TempByte;  // To adjust Padding Bits added for gEfiMemoryConfigDataGuid
} MEMORY_CFG_DATA;
/*
#ifdef  CRB_FLAG
typedef struct {
  UINT8 RootPortBus;
  UINT8 RootPortDevice;
  UINT8 RootPortFunction;
  UINT8 PcieSlotBus;
  UINT8 PcieSlotDevice;
  UINT8 PcieSlotFunction;
  UINT8 Dummy1;
  UINT8 Dummy2;
  UINT32 PCICommand;
  UINT32 Remap1;
  UINT32 Remap2;
  UINT32 Remap3;
  UINT32 Remap4Low;
  UINT32 Remap4High;
  UINT32 PCIInterruptLine;
  UINT32 PCIExpLinkCtrl;
  UINT32 LinkDbgCtrl;
  UINT32 DFS1Register;
  UINT32 TLCompletionTimeout;
} CARD_DATA;
#endif
*/
/*Structure for passing variables in smmstartthisapp across thread
Making it Global for now*/
typedef struct {
  SPIN_LOCK             ApBusy;
  UINT32                BankIndex;
  UINT32                ApicId;
} CMCI_ARGS_STRUCT;

/*Structure for passing variables in smmstartthisapp across thread
Making it Global for now*/
typedef struct {
  UINTN  LogicalCpu;
  BOOLEAN Sync;
} MCEMSR_ARGS_STRUCT;


// --------------------------- External Globals ---------------------------------------

//
// Host Structure pointer
//

extern PSYSHOST mHostPtr;
extern IIO_UDS  *mIioUds;
//extern EFI_QPI_RAS_PROTOCOL *mQpiRas;

//
// Processor error logging globals
//
extern BOOLEAN mLogUncorrProcErrors;
extern BOOLEAN mLogCorrMemErrors;

//
// IIO error logging globals
//
extern BOOLEAN mLogIohErrors;

//
// SMI settings
//
extern UINT32 mSmiMcaDelay;
extern BOOLEAN mUmcSmiEnabled;

//
// Spare Interrupt Select: none, smi, err0, NMI/CMCI
//
extern UINT8 mSpareIntSelect;

//
// Poison enabled
//
extern BOOLEAN mPoisonEnabled;
//extern BOOLEAN mViralEnabled; 
extern BOOLEAN mVtdEnable; //AptioV Server Override: Changes done to Support IOHCore/QPI/VTD Errors

//
// PCI/PCIe error settings and status
//
extern BOOLEAN  mNmiOnSerr;
extern BOOLEAN  mNmiOnPerr;
extern BOOLEAN  mPciSerrDetected;
extern BOOLEAN  mPciPerrDetected;
extern BOOLEAN  mPcieFatalErrDetected;
extern BOOLEAN  mPcieNonFatalErrDetected;
extern BOOLEAN  mPcieCorErrDetected;
extern BOOLEAN  mNumCorrectablePcieErrs;
extern BOOLEAN  mAllowClearingOfPcieDeviceStatus;
extern BOOLEAN  mAllowClearingOfUEStatus;
extern BOOLEAN  mWheaPlatformSupportAvailable;
extern BOOLEAN  mPcieCorrErrorEn;
extern BOOLEAN  mPcieUnCorrErrorEn;
extern BOOLEAN  mPcieFatalErrorEn;

extern EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL *mWheaPlatformSupport;

//
// Global export to chipset modules
//
extern EFI_SMM_BASE2_PROTOCOL *SmmBase;
extern EFI_SMM_SYSTEM_TABLE2* mSmst;
extern EFI_GUID 			  gEfiMpServiceProtocolGuid;
extern EFI_GUID               gIioVVarGuid;
extern EFI_HANDLE			  mImageHandle;
extern IOH_INFO               mIohInfo[MAX_SOCKET];
extern UINT8				  mIvtCpuEx;
extern UINT8                  mHaCmciOnCorrecetdThr;
extern BOOLEAN                mErrorPinEnabled;
extern BIOS_ACPI_PARAM *mAcpiParameter;
//extern BOOLEAN                mErrorIOMCAEnabled;
//
// All function definitions should follow here
//

//
// Error Enabling Routine Prototypes
//

EFI_STATUS
EnableElogMemory (
  VOID
  );



VOID
DisableCmci_s4030032(
  VOID);

EFI_STATUS
EnDisUncorrectableErrRep (
  IN ERROR_LOG_SETUP_DATA  *ErrorLogSetupData
  );

EFI_STATUS
ClearUboxAndMiscErrorStatus (
  VOID
  );
//
// Enable Cloaking 
//
#ifdef BDX_HOST 
VOID 
EnableCloaking (
  VOID
);
#endif

//
// Error Enabling QPI related
//
EFI_STATUS
EnableElogQPI (
   VOID
  );

//
// Enable Ubox related errors
//
VOID
EnableUboxError (
  VOID
  );

//
// Error Enabling Routine Prototypes
//
EFI_STATUS
EnableElogIoHub (
  VOID
  );

EFI_STATUS
EnableElogPCH (
  VOID
  );

EFI_STATUS
EnableAsyncSmiLogic (
  ERROR_LOG_SETUP_DATA  *ErrorLogSetupData
  );


EFI_STATUS
InitializeMcRas (
  ERROR_LOG_SETUP_DATA  *ErrorLogSetupData
  );

VOID
EnableDisableGlobalSmiGeneration(
  );

//
// Error Handling Routine Prototypes
//
EFI_STATUS
ElogIohErrorHandler (
  VOID
  );

EFI_STATUS
ElogIchErrorHandler (
  VOID
  );

EFI_STATUS
ElogProcessorCorrectableErrorHandler (
  IN  EFI_HANDLE    DispatchHandle,
  IN OUT VOID       *CommunicationBuffer,
  IN OUT UINTN      *SourceSize
  );

EFI_STATUS
ElogProcessorCatastrophicErrorHandler (
  IN  EFI_HANDLE    DispatchHandle,
  IN CONST VOID     *GpiContext,
  IN OUT VOID       *CommBuffer      OPTIONAL,
  IN OUT UINTN      *CommBufferSize  OPTIONAL
  );

VOID
ElogGenerateNmiNow (
  VOID
  );

BOOLEAN
IsPciPrimaryStatusError (
  IN UINT8        Bus,
  IN UINT8        Device,
  IN UINT8        Func
  );

BOOLEAN
IsPciSecondaryStatusError (
  IN UINT8        Bus,
  IN UINT8        Device,
  IN UINT8        Func
  );

//
// Auxiliary routines, chipset dependent
//

VOID
ClearGpiSmiStatus (
  VOID
  );


BOOLEAN
IchPcieErrorDetected (
  VOID
  );

VOID
ElogPrepareForErrorHandling (
  VOID
  )
;
VOID
DisableMemSmiGerneration (
  VOID
  )
;

EFI_STATUS
ELogRASS3EntryCallback (
  IN  EFI_HANDLE  Hanndle,
  IN CONST VOID     *Context         OPTIONAL,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
;

//EFI_STATUS
//EnableViralPoison (
  //IN ERROR_LOG_SETUP_DATA  *ErrorLogSetupData
  //);

EFI_STATUS
EnableElogMemoryRuntime( 
  UINT8  Node
);


EFI_STATUS
ConfigOEMSetupData(
  ERROR_LOG_SETUP_DATA	*ErrorLogSetupData
  );

VOID
ClearCoreErrors(
  VOID
  );

VOID
ClearUnCoreErrors(
  IN UINT32 SkipFlag
  );

VOID
ClearIIOErrorRegisterStatus (
  UINT8 SktId
);


VOID
ClearCBOErrors(
  VOID
);

VOID
ClearIIOErrors(
  VOID
);

EFI_STATUS
EnableElogMemory (
  VOID
  );


EFI_STATUS
ErrorHandlerMain 
(
  IN  EFI_HANDLE    DispatchHandle,
  IN CONST VOID     *Context         OPTIONAL,
  IN OUT VOID       *CommunicationBuffer,
  IN OUT UINTN      *SourceSize
);

VOID
ProcessUncorrectedError
(
VOID
);

EFI_STATUS
RegisterErrorSMIHandler (
  VOID
  );

BOOLEAN
CheckForSystemError(
  VOID
);

VOID
UpdateSetupGlobalVariables(
IN ERROR_LOG_SETUP_DATA  *ErrorLogSetupData
);

EFI_STATUS
SignalMCEtoOS (
  UINT32 ApicId
);

VOID
SignalMceMsr (
  MCEMSR_ARGS_STRUCT *MceArgs
);

/*
return value: 0 MMIO
                  1 MSR
*/
UINT32 
GetSaveStateType(
VOID
);

EFI_STATUS
SignalMceMmio (
  UINTN LogicalCpu
);


VOID
SignalCMCItoOS (
  UINT32 BankIndex, 
  UINT32 ApicId
);

VOID
SignalCMCI (
CMCI_ARGS_STRUCT *CMCIVarPtr
);
//AptioV Server Override Start: Clear the Errors which are coming in Cold Boot.
VOID
ClearIohExtPcieDeviceStatus(
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  );

EFI_STATUS
MaskIIOErrorReporting(
   VOID
   );

EFI_STATUS
UnMaskIIOErrorReporting(
   VOID
   );
//AptioV Server Override End: Clear the Errors which are coming in Cold Boot.
EFI_STATUS 
EFIAPI
PropagateSignals(
VOID
);
#endif

