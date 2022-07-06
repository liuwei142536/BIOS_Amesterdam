//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license
// agreement with Intel or your vendor.  This file may be
// modified by the user, subject to additional terms of the
// license agreement
//
/*************************************************************************
 *
 * Memory Reference Code
 *
 * ESS - Enterprise Silicon Software
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2006 - 2015 Intel Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents
 * related to the source code ("Material") are owned by Intel
 * Corporation or its suppliers or licensors. Title to the Material
 * remains with Intel Corporation or its suppliers and licensors.
 * The Material contains trade secrets and proprietary and confidential
 * information of Intel or its suppliers and licensors. The Material
 * is protected by worldwide copyright and trade secret laws and treaty
 * provisions.  No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other
 * intellectual property right is granted to or conferred upon you
 * by disclosure or delivery of the Materials, either expressly,
 * by implication, inducement, estoppel or otherwise. Any license
 * under such intellectual property rights must be express and
 * approved by Intel in writing.
 ************************************************************************
 *
 *  PURPOSE:
 *
 *      This file contains reference code data structures
 *
 ************************************************************************/

#ifndef _syshost_h
#define _syshost_h

#include <Token.h> // AptioV server override


#if defined(RC_SIM) || defined(HEADLESS_MRC)
#include <string.h>
#define MAX_LINE 256
#endif

#ifdef YAM_ENV
#define RC_SIM_FASTCADB 0
#endif

#ifdef RC_SIM
#define RC_SIM_FEEDBACK 1
//#define RC_SIM_EYE_DATA_FEEDBACK 1
#define RC_SIM_SMBUS    1
//#define SCRIPT_CSR      1
#define RC_SIM_FASTCADB 0
#endif



//
// Host reset states (0-7 are definable)
//
typedef enum BootMode {
  NormalBoot  = 0,  // Normal path through RC with full init, mem detection, init, training, etc.
                    //    Some of these MRC specific init routines can be skipped based on MRC input params
                    //    in addition to the sub-boot type (WarmBoot, WarmBootFast, etc).
  S3Resume    = 1   // S3 flow through RC. Should do the bare minimum required for S3
                    //    init and be optimized for speed.
} BootMode;




#include "DataTypes.h"
#include "Printf.h"
#include "PlatformHost.h"
#include "SysHostChip.h"
#include "SysRegs.h"
#include "IioPlatformData.h"
#ifdef BDAT_SUPPORT
#include "bdat.h"
#endif // BDAT_SUPPORT
#include "Printf.h"

//
// -------------------------------------
// Declarations and directives
// -------------------------------------
// Reference Code (RC) revision in BCD format:
// [31:24] = Major revision number
// [23:16] = Minor revision number
// [15:8] = Release Candidate number
//
#define CCMRC_REVISION  0x00500000

#define SDBG_MIN        BIT0
#define SDBG_MAX        BIT1
#define SDBG_TRACE      BIT2
#define SDBG_MEM_TRAIN  BIT3 + SDBG_MAX
#define SDBG_TST        BIT4
#define SDBG_CPGC       BIT5
#define SDBG_RCWRITETAG BIT6
#define SDBG_REG_ACCESS BIT6                                  // Displays all register accesses.
#define SDBG_MINMAX     SDBG_MIN + SDBG_MAX

//
// nvramPtr DWORD ?     ; Linear pointer to hostNvram structure (optional for S3 resume path)
//                      ; The NVRAM image is returned in the nvram field of the sysHost structure
//                      ; on the normal boot path. The OEM BIOS should use SIZEOF (struct sysNvram)
//                      ; to determine NVRAM size requirements.
//                      ; NULL indicates the NVRAM pointer is not valid and internal NVRAM
//                      ; hooks will be used instead. See OemReadNvram() and OemWriteNvram() hooks
//                      ; in oemhook.c for more details.
//
#pragma pack(1)
#ifdef SERIAL_DBG_MSG
struct usbDebugPortSetup {
    UINT8   Enable;
    UINT8   Bus;
    UINT8   Device;
    UINT8   Function;
};
#endif // SERIAL_DBG_MSG


//
// options DWORD  ?  ; Bit-mapped host options defined above in commonSetup.options section above
//
struct commonSetup {
  UINT32  options;
#ifdef SERIAL_DBG_MSG
  UINT8   serialDebugMsgLvl;
  UINT16  bsdBreakpoint;
#endif
  UINT32  maxAddrMem;
  UINT16  debugPort;
  UINT32  nvramPtr;
  UINT32  sysHostBufferPtr;
  UINT32  mmCfgBase; // MMCFG Base address, must be 64MB (SKX, HSX, BDX) / 256MB (GROVEPORT) aligned
  UINT32  mmCfgSize; // MMCFG Size address, must be 64M, 128M or 256M
  UINT32  mmiolBase; // MMIOL Base address, must be 64MB aligned
  UINT32  mmiolSize; // MMIOL Size address
  UINT16  mmiohBase; // Address bits above 4GB, i,e, the hex value here is address Bit[45:32]
  UINT8   pchumaEn; // 1 - Enable; 0 - Disable
  UINT8   numaEn; // 1 - Enable; 0 - Disable (i.e use UMA); N/A for 4S
  UINT8   DmiVcm;
  UINT8   bdatEn; // 1 - Enable; 0 - Disable
#ifdef SERIAL_DBG_MSG
  UINT16  consoleComPort;   // Base address of Console ComPort if in system
  struct  usbDebugPortSetup usbDebug;
#endif
  UINT8   PfatEnabled;
  UINT8   lowGap;
  UINT8   highGap;
  UINT16  mmiohSize; // Number of 1GB contiguous regions to be assigned for MMIOH space per CPU.  Range 1-1024
  UINT8   isocEn;    // 1 - Enable; 0 - Disable (BIOS will force this for 4S)
  UINT8   mesegEn;   // 1 - Enable; 0 - Disable (Mutually exclusive with 'isocEn')
  UINT8   dcaEn;     // 1 - Enable; 0 - Disable
  UINT8   PSMIEnabledSupport; // 1 - Enable; 0 - Disable
  UINT8   socketType;
  UINT8   ddrtXactor; // 1 - Enable; 0 - Disable
  UINT8   skipInitThrottling;
  UINT8   skipIioEarlyInit;
}; //struct commonSetup

//
// -----------------------------------------------------------------------------
//
struct sysSetup {
  struct cpuSetup     cpu;
  struct memSetup     mem;
  struct commonSetup  common;
  SYSSETUP_CHIP        // Chip hook to enable sysSetup fields
}; //struct sysSetup


struct sysNvram {
  struct cpuNvram     cpu;
  struct memNvram     mem;
  struct commonNvram  common;
  SYSNVRAM_CHIP        // Chip hook to enable sysNvram fields
}; //struct sysNvram

#pragma pack()

//
// -----------------------------------------------------------------------------
// Variable structures
//

//
// Warning log
//
#define MAX_LOG 64

struct warningEntry {
  UINT32  code;
  UINT32  data;
};
struct warningLog {
  UINT32              index;
  struct warningEntry log[MAX_LOG];
};

struct processorCommon {
  UINT32  capid0;
  UINT32  capid1;
  UINT32  capid2;
  UINT32  capid3;
  UINT32  capid4;
  UINT32  capid5;
  UINT32  capid6;
};

struct commonVar {
  UINT32                    rcVersion;              // Version of the reference code last executed
  UINT8                     resetRequired;          // Indicate type of reset required. 0 = no reset, 2 = Power Good, 3 = Warm reset.
  BootMode                  bootMode;               // NormalBoot or S3Resume indication
  UINT8                     emulation;              // indicate if running on emulation software. 0 = No emulation software. 4 = Simics
                                                    // it is a copy of uncore Ubox CSR device 11, function 0, offset 0xB0.
  UINT8                     rcWriteRegDump;
  UINT8                     memHpSupport;           // 1 - if platform supports memory HP. 0 - otherwise
  UINT8                     CurrentReset;           // Reset type witnessed by the system (viz PWRGD, WARM)


  UINT8                     cpuType;                // CPU Type
  UINT8                     cpuSubType;             // CPU Sub Type 1HA/2HA/EXA
  UINT8                     chopType[MAX_SOCKET];   // HCC, MCC, LCC, MCC-DE, LCC-DE (HSX family only)
  UINT32                    cpuFamily;              // Bit[19:4] of cpuid(1)_eax.  CPU_FAMILY_SKX
  UINT8                     stepping;               // Indicate current CPU stepping (a copy of RID register of PCI header offset 06h)
  UINT8                     socketId;               // socketID / NodeID of the current package BSP (0,1,2,3)
  UINT8                     sbsp;                   // Non-zero value indicates that the socket is System BSP
  UINT8                     numCpus;                // 1,..4. Total number of CPU packages installed and detected (1..4)by KTI RC
  UINT8                     MaxCoreCount;           // Max number of cores. 8 for JKT, 15 for IVT. Initialized based on cpuFamily field.
  UINT32                    FusedCores[MAX_SOCKET]; // Fused Core Mask in the package
  UINT8                     FusedCoreCount[MAX_SOCKET];//  array for CPU package Core count, indexed by SocketID
  UINT32                    socketPresentBitMap;    // bitmap of socket Id with CPUs present detected by KTI RC
  UINT8                     MaxCoreToBusRatio[MAX_SOCKET];  // Package Max Non-turbo Ratio (per socket)
  UINT8                     MinCoreToBusRatio[MAX_SOCKET];  // Package Maximum Efficiency Ratio (per socket)
  UINT8                     printfSync;
  UINT8                     smbSync;
  UINT8                     SmbusAPSemaphoreAcquired;    //SMBus AP System Semaphore has been acquired, ensure to release it
  UINT16                    pmBase;                 // Power Management Base Address
#ifdef PCH_SPT
  UINT32                    pwrmBase;
#endif //PCH_SPT
  UINT32                    rcbaBase;
  UINT16                    gpioBase;               // GPIO Base Address
  UINT16                    smbBase;                // SMB Base Address
  UINT16                    tolmLimit;
  UINT32                    tohmLimit;
  UINT32                    mmCfgBase;
  UINT32                    checkpoint;
  UINT32                    exitFrame;
  UINT32                    exitVector;
  UINT32                    rcEntryPoint;
  UINT32                    heapBase;
  UINT32                    heapSize;
  UINT64_STRUCT             cpuFreq;
  UINT64_STRUCT             startTsc;

#define USB_BUF_LIMIT 512
#define USB_BUF_SIZE  (USB_BUF_LIMIT + 160)
  USB2_DEBUG_PORT_INSTANCE  usbDebugPort;
  UINT8                     usbBuffer[USB_BUF_SIZE];
  UINT32                    usbBufCount;
  UINT32                    usbStartCount;
  UINT32                    serialBufEnable;
  UINT8                     serialInBuf[8];
  UINT32                    serialInCount;
  UINT32                    serialInByte;
#ifdef SERIAL_DBG_MSG
  UINT16                    globalComPort;   // Base address of Global ComPort currently in use
#endif

#define MAX_DEV 16
  UINT32                    dev[MAX_DEV];
  UINT32                    numDev;
  UINT8                     binMode;
  UINT32                    oemVariable;
  struct warningLog         status;                 // Warning log
  struct processorCommon    procCom[MAX_SOCKET];

#define MAX_PROMOTE_WARN_LIMIT  32
  UINT32                    promoteWarnLimit;
  UINT16                    promoteWarnList[MAX_PROMOTE_WARN_LIMIT];
  UINT8                     skuType;                // CPU SKU (EX , EP , DE, self boot, leverage boot )
  UINT8                     busIio[MAX_SOCKET];     // array for CPU IIO bus#, indexed by SocketID.
  UINT8                     EVMode;
  UINT8                     socketType;             // Socket P0
  UINT16                    LlcWays;                // Number of LLC way as determined by cpuid(eax=4)
  UINT32                    printfDepth;            // handle nested calls to get/releasePrintFControl 
  UINT32                    smbusDepth;             // handles nested calls to get/releaseSmbus
  UINT8                     rdrandSupported;
  UINT8                     numActiveHAperSocket;   // Number active HA per socket (1 or 2)
  UINT8                     currentReset;           // Reset type witnessed by the system (viz PWRGD, WARM)
#ifdef RC_SIM
  FILE *inFile;
#endif

  COMMONVAR_CHIP           // Chip hook to enable commonVar fields
  UINT8                     nsSkuId;                 // NS SKU Identification. 0: DE, 1: NS Storage Sku, 2: NS Comms Sku
};  //struct commonVar

struct sysVar {
  struct cpuVar     cpu;
  struct memVar     mem;
  IIO_GLOBALS       iio;
  struct commonVar  common;
  SYSVAR_CHIP              // Chip hook to enable sysVar fields
};  //struct sysVar

//
// System previous boot error structure
//
#define MC_BANK_STATUS_REG    1
#define MC_BANK_ADDRESS_REG   2
#define MC_BANK_MISC_REG      3

#define MSR_LOG_VALID   BIT31
#define MSR_LOG_UC      BIT29
#define MSR_LOG_EN      BIT28

struct mcBankTableEntry {
  UINT8   mcBankNum;
  UINT16  mcBankAddress;
};

struct mcBankStruct {
  UINT8           socketId;
  UINT8           mcBankNum;
  UINT64_STRUCT   mcStatus;
  UINT64_STRUCT   mcAddress;
  UINT64_STRUCT   mcMisc;
};

struct prevBootErrInfo {
  UINT32 validEntries;
  struct mcBankStruct mcRegs[MAX_PREV_BOOT_ERR_ENTRIES];
};

//
// ----------------------------------------------------------------
// System Ras Host structure
//
struct sysRasSetup {
  struct cpuSetup     cpu;
  struct commonSetup  common;
  SYSRASSETUP_CHIP              // Chip hook to enable sysRasSetup fields
};

struct sysRASVar {
  struct cpuVar     cpu;
  struct commonVar  common;
  SYSRASVAR_CHIP                // Chip hook to enable sysRasVar fields
};

typedef struct sysRasHost {
  const struct sysRasSetup setup;  // constant input data   
  struct sysRASVar         var;    // variable, volatile data
}
SYSRASHOST, *PSYSRASHOST;

// Bit definitions for commonSetup.options
//                   ; PROMOTE_WARN_EN enables warnings to be treated as fatal error
//                   ; PROMOTE_MRC_WARN_EN enables MRC warnings to be treated as fatal error
//                   ; HALT_ON_ERROR_EN enables errors to loop forever
#define PROMOTE_WARN_EN           BIT0
#define PROMOTE_MRC_WARN_EN       BIT1
#define HALT_ON_ERROR_EN          BIT2


// -----------------------------------------------------------------------------
// System host (root structure)
//
typedef struct sysHost {
  struct sysSetup       setup;  // constant input data
  struct sysVar         var;    // variable, volatile data
  struct sysNvram       nvram;  // variable, non-volatile data for S3 and fast path
  struct prevBootErrInfo prevBootErr;
#ifdef  BDAT_SUPPORT
  BDAT_MEMORY_DATA_STRUCTURE    bdat;
  BDAT_RMT_STRUCTURE            bdatRmt;
#endif  // BDAT_SUPPORT
  UINT32  cpuFlows; // Which flows to take
  UINT32  ktiFlows;
  UINT32  qpiFlows;
  UINT32  opioFlows;
  UINT32  memFlows;
  UINT32  memFlowsExt;
}
SYSHOST, *PSYSHOST;


//
// Handle assertions with RC_ASSERT
//
#if defined(SIM_BUILD) || defined(IA32)

#define RC_ASSERT(assertion, majorCode, minorCode) \
  if (!(assertion)) { \
    DebugPrintRc (host, 0xFF, "\n\nRC_ASSERT! %s: %u   %s ", __FILE__, __LINE__, #assertion);\
    FatalError (host, majorCode, minorCode);\
  }

#else

#define RC_ASSERT(assertion, majorCode, minorCode) \
  if (!(assertion)) { \
    CpuCsrAccessError (host, "\n\nRC_ASSERT! %s: %u   %s ", __FILE__, __LINE__, #assertion);\
  }

#endif


//
// Decompression algorithm begins here (move this to separate core header)
//
#define BITBUFSIZ 32
#define MAXMATCH  256
#define THRESHOLD 3
#define CODE_BIT  16
#ifndef UINT8_MAX
#define UINT8_MAX 0xff
#endif
#define BAD_TABLE - 1

//
// C: Char&Len Set; P: Position Set; T: exTra Set
//
#define NC      (0xff + MAXMATCH + 2 - THRESHOLD)
#define CBIT    9
#define MAXPBIT 5
#define TBIT    5
#define MAXNP   ((1U << MAXPBIT) - 1)
#define _NT      (CODE_BIT + 3)
#if _NT > MAXNP
#define NPT _NT
#else
#define NPT MAXNP
#endif

typedef struct {
  UINT8   *mSrcBase;  // Starting address of compressed data
  UINT8   *mDstBase;  // Starting address of decompressed data
  UINT32  mOutBuf;
  UINT32  mInBuf;

  UINT16  mBitCount;
  UINT32  mBitBuf;
  UINT32  mSubBitBuf;
  UINT16  mBlockSize;
  UINT32  mCompSize;
  UINT32  mOrigSize;

  UINT16  mBadTableFlag;

  UINT16  mLeft[2 * NC - 1];
  UINT16  mRight[2 * NC - 1];
  UINT8   mCLen[NC];
  UINT8   mPTLen[NPT];
  UINT16  mCTable[4096];
  UINT16  mPTTable[256];

  //
  // The length of the field 'Position Set Code Length Array Size' in Block Header.
  // For EFI 1.1 de/compression algorithm, mPBit = 4
  // For Tiano de/compression algorithm, mPBit = 5
  //
  UINT8   mPBit;
} SCRATCH_DATA;

#define ACQUIRE_SEMAPHORE   1
#define RELEASE_SEMAPHORE   0
#define SYSTEMSEMAPHORE0    0
#define SYSTEMSEMAPHORE1    1

#if defined(SIM_BUILD) || defined(IA32)

#define RC_ASSERT(assertion, majorCode, minorCode) \
  if (!(assertion)) { \
  DebugPrintRc (host, 0xFF, "\n\nRC_ASSERT! %s: %u   %s ", __FILE__, __LINE__, #assertion);\
    FatalError (host, majorCode, minorCode);\
  }

#else

#define RC_ASSERT(assertion, majorCode, minorCode) \
  if (!(assertion)) { \
    CpuCsrAccessError (host, "\n\nRC_ASSERT! %s: %u   %s ", __FILE__, __LINE__, #assertion);\
  }

#endif

#ifdef SSA_FLAG
//BIOS SSA PRE-PROCESSOR DEFINES

#ifndef MAX_HOB_ENTRY_SIZE
#define MAX_HOB_ENTRY_SIZE  60*1024  //Needed for BiosSaveToBdat()
#endif //MAX_HOB_ENTRY_SIZE

#define MRC_SSA_SERVICES_SET  1  //1. Used to Include a BIOS specific header file in "SsaCommonConfig.h" for the BSSA code(Servers), but to exclude that file from the Test Content side of the code as the same*
//*header is shared on both sides. Not declaring #define in a Header file as both "MrcSsaServices.h" and the other "Headers" declared in them need to be exact copies on both sides..
// 2. An #ifndef in the "header", has the required structs redefined so that they are available only on the Test Content Side

#ifdef RC_SIM
#ifdef SERIAL_DBG_MSG
//#define ENBL_BIOS_SSA_DEBUG_MSGS  1  //To Enable BSSA Debug Messages
#endif //SERIAL_DBG_MSG
#endif //RC_SIM

//BSSA 1.0 GUID.. #define SSA_BIOS_SERVICES_PPI_GUID  {0xBAE29D7B, 0x89BB, 0x4223, 0xAF, 0x76, 0x96, 0xD0, 0xB3, 0x24, 0x9B, 0x36}
//{0xBAE29D7B, 0x89BB, 0x4223, { 0xAF, 0x76, 0x96, 0xD0, 0xB3, 0x24, 0x9B, 0x36 }}

#define EXPONENTIAL                     (0)
#define LINEAR                          (1)

//MRC_SYSTEM_INFO
#define MAX_CH_IMC                      (MAX_CH / MAX_IMC)     //Max of 3 Channels per MC
#define CPU_SOCKET_BIT_MASK             (((UINT32) (~0)) >> (32 - MAX_SOCKET))
#define BUS_WIDTH                       (64)
#define BUS_FREQ                        (1333)

//MRC_SERVER_CPGC_INFO
#define MAX_NUMBER_DQDB_CACHELINES                 64
#define MAX_NUMBER_DQDB_UNISEQS                    3
#define MAX_DQDB_UNISEQ_SEED_VALUE                 0xFFFFFF
#define MAX_DQDB_UNISEQ_L_VALUE                    0xFF
#define MAX_DQDB_UNISEQ_M_VALUE                    0xFF
#define MAX_DQDB_UNISEQ_N_VALUE                    0xFF
#define MAX_DQDB_INC_RATE_EXP_VALUE                7
#define MAX_DQDB_INC_RATE_LINEAR_VALUE             31
#define MAX_DQDB_UNISEQ_SEED_RELOAD_RATE_VAL       0x7
#define MAX_DQDB_UNISEQ_SEED_SAVE_RATE_VAL         0x3F
#define MAX_DQDB_INV_DC_SHIFT_RATE_EXP_VAL         0xF
#define MAX_NUMBER_CADB_ROWS                       16
#define MAX_NUMBER_CADB_UNISEQS                    4
#define MAX_NUMBER_CADB_UNISEQ_SEED_VALUE          0xFFFF
#define MAX_CADB_UNISEQ_L_VALUE                    0xFF
#define MAX_CADB_UNISEQ_M_VALUE                    0xFF
#define MAX_CADB_UNISEQ_N_VALUE                    0xFF
#define MAX_DUMMY_READ_L_VAL                       0xFF
#define MAX_DUMMY_READ_M_VAL                       0xFF
#define MAX_DUMMY_READ_N_VAL                       0xFF
//BOOLEAN IsLoopCountExponential;                  TRUE
#define MAX_LOOP_COUNT_VAL                         0x1F
#define MAX_START_DELAY_VAL                        0xFF
#define MAX_NUMBER_SUBSEQS                         10
#define MAX_BURST_LENGHT_EXP_VAL                   0x1F
#define MAX_BURST_LENGHT_LIN_VAL                   0x3F
#define MAX_INTER_SUBSEQ_WAIT_VAL                  0xFF
#define MAX_OFFSET_ADDR_UPDATE_RATE_VAL            0x1F
#define MAX_ADDRESS_INV_RATE_VAL                   0xF
#define MAX_RANK_ADDRESS_INC_RATE_EXP_VAL          0x1F
#define MAX_RANK_ADDRESS_INC_RATE_LIN_VAL          0x1F
#define MIN_RANK_ADDRESS_INC_VAL                   -4
#define MAX_RANK_ADDRESS_INC_VAL                   3
#define MAX_BANK_ADDRESS_INC_RATE_EXP_VAL          0x1F
#define MAX_BANK_ADDRESS_INC_RATE_LIN_VAL          0x1F
#define MIN_BANK_ADDRESS_INC_VAL                   -8
#define MAX_BANK_ADDRESS_INC_VAL                   7
#define MAX_ROW_ADDRESS_INC_RATE_EXP_VAL           0x1F
#define MAX_ROW_ADDRESS_INC_RATE_LIN_VAL           0x1F
#define MIN_ROW_ADDRESS_INC_VAL                    -2048
#define MAX_ROW_ADDRESS_INC_VAL                    2047
#define MAX_COL_ADDRESS_INC_RATE_EXP_VAL           0x1F
#define MAX_COL_ADDRESS_INC_RATE_LIN_VAL           0x1F
#define MIN_COL_ADDRESS_INC_VAL                   -128
#define MAX_COL_ADDRESS_INC_VAL                    127
#define MAX_NUMBER_RANK_MAP_ENTRIES                8
#define MAX_NUMBER_BANK_MAP_ENTRIES                0
#define MAX_STOP_ON_NTH_ERR_CNT_VAL                0x1F
#define MAX_NUMBER_ERROR_COUNTERS                  9
#define MAX_NUMBER_ROW_ADD_SWIZZLE_ENTRIES         6
#define MAX_ROW_ADD_SWIZZLE_VAL                    16
//MRC_SERVER_CPGC_INFO -END

//min/max limit for the margin parameter offset
//used for override the theoretical min/max limits
#define MIN_TIMING_MARGIN_PAMRAM_OFFSET         -32
#define MAX_TIMING_MARGIN_PAMRAM_OFFSET          31
#define MIN_VOLTAGE_MARGIN_PAMRAM_OFFSET        -64
#define MAX_VOLTAGE_MARGIN_PAMRAM_OFFSET         63

#define LOWER_ROW_ADDR_SWIZZLE_CNT      6   //Number of lower row address swizzle parameters
#define UPPER_ROW_ADDR_SWIZZLE_CNT      12  //Number of upper row address swizzle parameters in first upper register
#define MAX_ROW_ADDR_SWIZZLE_VAL        16  //Maximum swizzle value
#endif //SSA_FLAG

#endif // _syshost_h
