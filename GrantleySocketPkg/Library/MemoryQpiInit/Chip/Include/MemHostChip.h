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
 *      This file contains memory detection and initialization for
 *      IMC and DDR3 modules compliant with JEDEC specification.
 *
 ************************************************************************/

#ifndef _memhostchip_h
#define _memhostchip_h

#include "DataTypes.h"
#include "PlatformHost.h"
#include "SysRegs.h"
#include "MemRegs.h"
#include "MemDefaults.h"

#ifndef ASM_INC

// BDX Si constants
#define CDIEVSSHI_BDW       2000          //Constant CdieVssHi = 2000 (value in pF).
#define PANICCOMPUPMULT     0x0000007F
#define PANICCOMPDNMULT     0x0000007F

// Define to include core code for Advanced Memtest feature
#define ADV_MEMTEST_SUPPORT  1

#define NODE_TO_SKT(socket)       (socket >> 1)
#define NODE_TO_MC(socket)        (socket & BIT0)
#define SKTMC_TO_NODE(socket, mc)  ((socket << 1) | (mc & BIT0))

//
// MemDisplay
//
#define DISPLAY_RX_VREF_SUPPORT 1  // undefined if unsupported

//
// MemTraining
//
#define EYE_HEIGHT_SIM_DEFAULT 10
#define RX_VREF_TUNING         1
#define DESKEW_DELAY            RxDqsPBitDelay  // Set this to either RxDqsBitDelay or RxDqsPBitDelay
#define MIN_EARLYVREF_EYE       6
#define MAX_VREF_SETTINGS       10

//
// Array sizes
//
#ifdef DE_SKU
#define MAX_IMC             1
#else
#define MAX_IMC             2                   // Max memory controllers per socket
#endif // DE_SKU

#define MAX_HA_CH           4                   // Max number of channels per HA/IMC
#define MC_MAX_NODE         (MAX_SOCKET * MAX_IMC) // Max number of memory nodes
#define MAX_CH              4                   // Max channels per socket.
#define MAX_SYS_CH          (MAX_CH * MAX_SOCKET) // Max channels in the system
#define MAX_DIMM            3                   // Max DIMM per channel
#define MAX_UNIQUE_NGN_DIMM_INTERLEAVE 2            // Max number of unique interleaves for NGN DIMM
#define MAX_BITS            72                  // Max number of data bits per rank
#define MAX_TECH            19                  // Number of entries in DRAM technology table
#define MAX_TECH_DDR4       12                  // Number of entries in DRAM technology table
#define MAX_TECH_AEP        8
#define MAX_SUP_FREQ        19                  // 19 frequencies are supported (800, 1067, 1333, 1600, 1867, 2133, 2400, 2667, 2933, 3200)
#define MAX_DENSITY         4
#define MAX_RM              2
#define MAX_RIR             5                   // Number of Rank Interleave Register rules
#define MAX_RIR_WAYS        8                   // Number of interleave ways for RIR
#define TAD_RULES           12                  // Number of TAD rule registers
#define MAX_TAD_WAYS        4                   // Number of interleave ways for TAD RULES
#define SAD_RULES           20                  // Number of SAD rule registers
#define MAX_SAD_RULES       20                  // Number of SAD rule registers
#define MAX_SAD_WAYS        8                   // Number of interleave ways for SAD RULES
#define SAD_ATTR_DRAM       0                   // DRAM attribute for SAD rule
#define MAX_PAD             1                   // Number of PAD registers
#define MAX_RAW_CARD        5                   // Number of supported Raw Cards
#define MAX_STROBE          18                  // Number of strobe groups
#define MAX_SPD_BYTE        256                 // Number of bytes in Serial EEPROM
#define MAX_SPD_BYTE_DDR4   512                 // Number of bytes in Serial EEPROM on DDR4
#define PARITY_INTERLEAVE   1                   // SAD interleave with merged HAs (pseudo-hemisphere mode)
#define MAX_SEEDS           10                  // Maximum
#if   QR_DIMM_SUPPORT
#define MAX_RANK_DIMM       4                   // Max ranks per DIMM
#else
#define MAX_RANK_DIMM       2                   // Max ranks per DIMM
#endif
#define MAX_RANK_CH         8                   // Max ranks per channel
#ifdef LRDIMM_SUPPORT
#define MAX_SUBRANK         2                   // Max subranks per logical rank
#endif
#define SPD_MODULE_PART     18                  // Number of bytes of module part - DDR3
#define SPD_MODULE_PART_DDR4 20                 // Number of bytes of module part - DDR4
#define MAX_PB_DESKEW_GROUP 4                   // Max Per-Bit Deskew Groups
#define SAD_RULES_ADDR_RANGE  4                 // Max IOT rules = 4, Total address limits (lower(4) entries each)
#define MAX_PARTIAL_MIRROR      1               //Maximum number of partial mirror regions that can be created

#define MAX_CMD_GROUP0_A0   2
#define MAX_CMD_GROUP1_A0   2
#define MAX_CMD_GROUP2_A0   2
#define MAX_CMD_GROUPALL_A0 6
#define MAX_CMD_GROUP0      4
#define MAX_CMD_GROUP1      4
#define MAX_CMD_GROUP2      4
#define MAX_CMD_GROUPALL    12

#define MAX_CTL_GROUP0    4
#define MAX_CTL_GROUP1    3
#define MAX_CTL_GROUP2    5
#define MAX_CTL_GROUP3    3
#define MAX_CTL_GROUP4    6
#define MAX_CTL_GROUPALL  21
#define NUM_CTL_PLATFORM_GROUPS 5

#define MAX_CMDCTL_GROUPALL  (MAX_CMD_GROUPALL + MAX_CTL_GROUPALL)

// SPD Defines
//-----------------------------------------------------------------------------
// page size equates for tFAW table
#define PAGESIZE1K   0
#define PAGESIZE2K   1

//
// End SPD Defines
//

// NVM DIMMs
#define MAX_MANUFACTURER_STRLEN 19
#define MAX_SERIALNUMBER_STRLEN 19

typedef struct {
  UINT8 effBusDelaySR;
  UINT8 effBusDelayDR;
  UINT8 effBusDelayDD;
  UINT8 cas2RecEnDR;
  UINT8 cas2RecEnDD;
  UINT8 cas2DrvEnDR;
  UINT8 cas2DrvEnDD;
  UINT8 odtRdLeading;
  UINT8 odtRdTrailing;
  UINT8 odtWrLeading;
  UINT8 odtWrTrailing;
  UINT8 clkDiffDR;
  UINT8 clkDiffDD;
} TT_VARS;


#define CMD_GRP_DELAY         0
#define CMD_SIGNAL_DELAY      1
#define CMDCTL_SIGNAL_DELAY   2

//Uncomment this line to enabled CA Loopback
//#define CA_LOOPBACK     1
#define NUM_CS                    8
#define NUM_CKE                   4
#define NUM_ODT                   4
#ifdef CA_LOOPBACK
#define NUM_CA_SIGNALS            25
#else
#define NUM_CA_SIGNALS            0
#endif
#define NUM_CTL_SIGNALS           (NUM_CS + NUM_CKE + NUM_ODT)
#define NUM_SIGNALS_TO_SWEEP_LB   (NUM_CTL_SIGNALS + NUM_CA_SIGNALS)

#define CH_BITMASK        0xF       // For HSX: 0000 1111 (0F) when all channels are present on a socket ..as this is needed outside of BSSA also

//
// Per Bit De-Skew for IVT
//
#define MAX_PER_BIT_DESKEW_RANKS      4

//
// Xover Calib Samples
//
#define XOVER_CALIB_SAMPLE_COUNT      50


#define MAX_CMD_CAVREF            127
#define MAX_RD_VREF_OFFSET        48
#define MAX_WR_VREF_OFFSET        31
#define MAX_WR_VREF_OFFSET_BACKSIDE    50
#define MAX_TIMING_OFFSET         31
#define MAX_FAN_STEPS             3
#define FAN_RIGHT                 0
#define FAN_LEFT                  1
#define FAN_CENTER                2
#define FAN_ENABLE                1
#define FAN_DISABLE               0

//
// Thermal equates
//
#define DRAM_TCASE_DEFAULT  (85 << 4)   // 85 degrees C
#define DRAM_TCASE_MAX      (95 << 4)   // 95 degrees C

#pragma pack(1)

struct ChannelTestGroup
{
  UINT8                       channelBitMask;
#ifdef ASM_INC
  UINT32                      maxMtr;
#else
  MAXMTR_MC_MAINEXT_STRUCT    maxMtr;
#endif
};

//
// Platform structures
//
// --------------------------------------------------------------------
// MRC setup structures
// --------------------------------------------------------------------
//

//
// -----------------------------------------------------------------------------
//
// Host enumerated modes
//
#define CH_INDEPENDENT  0
#define CH_MIRROR       BIT0
#define CH_LOCKSTEP     BIT1
#define RK_SPARE        BIT2
#define CH_ML          (CH_MIRROR | CH_LOCKSTEP)
#define CH_SL          (RK_SPARE  | CH_LOCKSTEP)
#define CH_MS          (CH_MIRROR | RK_SPARE)
#define CH_MLS         (CH_MIRROR | CH_LOCKSTEP | RK_SPARE)

//
// Host extended RAS Modes
//
#define DEVTAGGING_EN       BIT0
#define DMNDSCRB_EN         BIT1
#define PTRLSCRB_EN         BIT2
#define A7_MODE_EN          BIT4

//
// Host bit-mapped memSetup.options
//
//                   ; TEMPHIGH_EN enables support for 95 degree DIMMs
//                   ; ATTEMPT_FAST_BOOT_COLD enables support for taking the fast path through the MRC on a cold boot.
//                   ; PDWN_SR_CKE_MODE enables CKE to be tri-stated during register clock off power down self-refresh
//                   ; OPP_SELF_REF_EN enables the opportunistic self refresh mechanism
//                   ; MDLL_SHUT_DOWN_EN enables MDLL shutdown
//                   ; PAGE_POLICY: Clear for open page, set for closed page. Open page has better performance and
//                   ;  power usage in general. Close page may benefit some applications with poor locality
//                   ; ALLOW2XREF_EN enables 2X refresh if needed for extended operating temperature range (95degrees)
//                   ;  If TEMPHIGH_EN is also set, setting this bit will result in 2X refresh timing for the IMC
//                   ;  refresh control register
//                   ; MULTI_THREAD_MRC_EN enables multithreaded MRC. This reduces boot time for systems with multiple
//                   ;  processor sockets
//                   ; ADAPTIVE_PAGE_EN enables adaptive page mode. The memory controller will dynamically determine how long
//                   ;  to keep pages open to improve performance
//                   ; CMD_CLK_TRAINING_EN enables command to clock training step in BIOS
//                   ; SCRAMBLE_EN Set to enable data scrambling. This should always be enabled except for debug purposes.
//                   ; BANK_XOR_EN enables bank XOR memory mapping mode which is targeted at workloads with bank thrashing
//                   ;  caused by certain stride or page mappings
//                   ; RCOMP_TYPE Determines whether RCOMPs are controlled by pcode of the hardware
//                   ; DDR_RESET_LOOP enables infinite channel reset loop without retries for gathering of margin data
//                   ; NUMA_AWARE enables configuring memory interleaving appropriately for NUMA aware OS
//                   ; DISABLE_WMM_OPP_READ disables issuing read commands opportunistically during WMM
//                   ; ECC_CHECK_EN Enables ECC checking
//                   ; ECC_MIX_EN enables ECC in a system with mixed ECC and non-ECC memory in a channel by disabling ECC when
//                   ;  this configuration is detected.
//                   ; BALANCED_4WAY_EN < NOT CURRENTLY USED IN FUNCTIONAL CODE >
//                   ; SPLIT_BELOW_4GB_EN Forces memory address space below 4GB(or TOLM) to be split across CPU sockets when applicable
//                   ; RAS_TO_INDP_EN switches from lockstep or mirror mode to independenct channel mode when memory is present
//                   ;  on channel 2 and this is enabled
//                   ; MARGIN_RANKS_EN Enables the rank margin tool
//                   ; MEM_OVERRIDE_EN enables use of inputMemTime inputs as hard overrides
//                   ; DRAMDLL_OFF_PD_EN < NOT CURRENTLY USED IN FUNCTIONAL CODE >
//                   ; MEMORY_TEST_EN enables execution of MemTest if on cold boot
//                   ; MEMORY_TEST_FAST_BOOT_EN enables the memory test when going through a fast boot path (fast warm, fast cold, etc)
//                   ; ATTEMPT_FAST_BOOT attempts to take a fast boot path if the NVRAM structure is good and the memory config
//                   ;  hasn't changed.  For example, on a warm boot, this will take the "fast warm" path through MRC which attempts
//                   ;  to make it as close as possible to the S3 path.
//                   ; SW_MEMORY_TEST_EN < NOT CURRENTLY USED IN FUNCTIONAL CODE >
//
#define TEMPHIGH_EN           BIT0
#define ATTEMPT_FAST_BOOT_COLD BIT1
#define PDWN_SR_CKE_MODE      BIT2
#define OPP_SELF_REF_EN       BIT3
#define MDLL_SHUT_DOWN_EN     BIT4
#define PAGE_POLICY           BIT5
#define ALLOW2XREF_EN         BIT6
#define MULTI_THREAD_MRC_EN   BIT7
#define ADAPTIVE_PAGE_EN      BIT8
#define CMD_CLK_TRAINING_EN   BIT9
#define SCRAMBLE_EN           BIT10
#define BANK_XOR_EN           BIT11
#define DISPLAY_EYE_EN        BIT12
#define DDR_RESET_LOOP        BIT13
#define NUMA_AWARE            BIT14
#define DISABLE_WMM_OPP_READ  BIT15
#define RMT_COLD_FAST_BOOT    BIT16
#define ECC_CHECK_EN          BIT17
#define ECC_MIX_EN            BIT18
#define BALANCED_4WAY_EN      BIT19
#define CA_PARITY_EN          BIT20
#define SPLIT_BELOW_4GB_EN    BIT21
#define PER_NIBBLE_EYE_EN     BIT22
#define RAS_TO_INDP_EN        BIT23
#define MARGIN_RANKS_EN       BIT25
#ifdef XMP_SUPPORT
#define MEM_OVERRIDE_EN       BIT26
#endif
#define DRAMDLL_OFF_PD_EN     BIT27
#define MEMORY_TEST_EN            BIT28
#define MEMORY_TEST_FAST_BOOT_EN  BIT29
#define ATTEMPT_FAST_BOOT         BIT30
#define SW_MEMORY_TEST_EN     BIT31
#define FASTBOOT_MASK         (BIT30 | BIT1)

//
// optionsExt
//
// Note: these options are redundant with memflows. They should be consolidated.
#define EARLY_CMD_CLK_TRAINING_EN BIT0
#define RD_VREF_EN                BIT1
#define WR_VREF_EN                BIT2
#define XOVER_EN                  BIT3
#define SENSE_EN                  BIT4
#define PDA_EN                    BIT5
#define TURNAROUND_OPT_EN         BIT6
#define ROUND_TRIP_LATENCY_EN     BIT7
#define ALLOW_CORRECTABLE_ERROR   BIT8
#define EARLY_CTL_CLK_TRAINING_EN BIT9
#define TX_EQ_EN                  BIT10
#define IMODE_EN                  BIT11
#define CMD_NORMAL_EN             BIT12
#define PER_BIT_MARGINS           BIT13
#define DUTY_CYCLE_EN             BIT14
#define RCOMP_TYPE                BIT15
#define CMD_VREF_EN               BIT16
#define LRDIMM_BACKSIDE_VREF_EN   BIT17
#define MC_RON_EN                 BIT18
#define DRAM_RON_EN               BIT19
#define RX_ODT_EN                 BIT20
#define RTT_WR_EN                 BIT21
#define RX_CTLE_TRN_EN            BIT22
#define WR_CRC                    BIT23
#define NON_TGT_ODT_EN            BIT24
#define LRDIMM_WR_VREF_EN         BIT25
#define LRDIMM_RD_VREF_EN         BIT26
#define LRDIMM_TX_DQ_CENTERING    BIT27
#define LRDIMM_RX_DQ_CENTERING    BIT28
#define DDR4_PLATFORM             BIT29 
#define CAP_ERR_FLOW              BIT30
#define CECC_WA                   BIT31

//
// Host bit-mapped Memory Thermal Throttling Related options (mem.thermalThrottlingOptions)
//
#define MH_OUTPUT_EN          BIT0  // Enable for MEM_HOT output generation logic
#define MH_SENSE_EN           BIT1  // Enable for MEM_HOT sense logic
#define OLTT_EN               BIT2  // Enable open loop thermal throttling control
#define CLTT_EN               BIT3  // Enable closed loop thermal throttling control

#define RDIMM             0
#define UDIMM             1
#define RDIMMandUDIMM     2
#define SODIMM            2
#define VLPRDIMM          0
#define VLPUDIMM          1

#define LRDIMM            9
#define QRDIMM            10

//
// --------------------------------------------------------------------
// MRC runtime structures
// --------------------------------------------------------------------
//
typedef union {
   struct{
      UINT16 freq:4;      // Frequency enum - DDR3_800, 1067, 1333, 1600, 1867, 2133
      UINT16 rsvd:3;      // Reserved for now
      UINT16 slot0:3;     // DIMM slot 0 enum - Not present, Empty, SR, DR, QR, LR
      UINT16 slot1:3;     // DIMM slot 1 enum - Not present, Empty, SR, DR, QR, LR
      UINT16 slot2:3;     // DIMM slot 2 enum - Not present, Empty, SR, DR, QR, LR
   } Bits;
   UINT16 Data;
} ODT_VALUE_INDEX;

typedef union {
   struct{
      UINT8 rttNom:3;      // Rtt_nom value matching JEDEC spec
      UINT8 rsvd_3:1;
      UINT8 rttWr:2;       // Rtt_park value matching JEDEC spec
      UINT8 rsvd_6:2;
   } Bits;
   UINT8 Data;
} ODT_VALUE_RTT_DDR3;

struct odtValueStruct {
   UINT16 config;                            // ODT_VALUE_INDEX
   UINT8 mcOdt;                              // bit[0] = 0 for 50 ohms; 1 for 100 ohms
   UINT8 dramOdt[MAX_DIMM][MAX_RANK_DIMM];   // ODT_VALUE_RTT_DDR3
};

typedef union {
   struct{
      UINT8 rttNom:3;      // Rtt_nom value matching JEDEC spec
      UINT8 rttPrk:3;      // Rtt_park value matching JEDEC spec
      UINT8 rttWr:2;       // Rtt_wr value matching JEDEC spec
   } Bits;
   UINT8 Data;
} ODT_VALUE_RTT_DDR4;

struct ddr4OdtValueStruct {
   UINT16 config;                            // ODT_VALUE_INDEX
   UINT8 mcOdt;                              // bit[0] = 0 for 50 ohms; 1 for 100 ohms
   UINT8 mcVref;                             // Percent Vddq formula by MC spec
                                             // ReadVrefSel = (percent * 192 / Vdd) - 128
   UINT8 dramVref;                           // Percent Vddq as defined by JEDEC spec
   UINT8 dramOdt[MAX_DIMM][MAX_RANK_DIMM];   // ODT_VALUE_RTT_DDR4
};

typedef union {
   struct{
      UINT16 dimmNum:2;    // Target DIMM number on the channel
      UINT16 rankNum:2;    // Target Rank number on the DIMM
      UINT16 rsvd:3;
      UINT16 slot0:3;      // DIMM slot 0 enum - Not present, Empty, SR, DR, QR, LR
      UINT16 slot1:3;      // DIMM slot 1 enum - Not present, Empty, SR, DR, QR, LR
      UINT16 slot2:3;      // DIMM slot 2 enum - Not present, Empty, SR, DR, QR, LR
   } Bits;
   UINT16 Data;
} ODT_ACT_INDEX;

struct firmwareRev {
  UINT8  majorVersion;
  UINT8  minorVersion;
  UINT8  hotfixVersion;
  UINT16 buildVersion;
};

struct  prevBootDimmList {
  // NVM DIMM Information - Valid only if aepDimmPresent
  UINT8           manufacturer[MAX_MANUFACTURER_STRLEN];  // Manufacturer
  UINT8           serialNumber[MAX_SERIALNUMBER_STRLEN];  // Serial Number
  UINT16          VendorID;           // Vendor ID for CR Mgmt driver thru Nfit
  UINT16          DeviceID;           // Device ID for CR Mgmt driver thru Nfit
  UINT16          rawCap;             // Raw Capacity
  UINT16          volCap;             // Volatile Capacity (2LM region)
  UINT16          nonVolCap;          // NonVolatile Capacity (PMEM/PMEM$ region + Blk region)
  UINT16          perCap;             // Persistent Capcity (PMEM/PMEM$). This size is not obtained from NVMCTLR. This is derived data.
  UINT16          blkCap;             // Block Capcity (BLK) This size is not obtained from NVMCTLR. This is derived data.
  UINT32          volRegionDPA;       // DPA start address of 2LM Region
  UINT32          perRegionDPA;       // DPA start address of PMEM Region
  struct firmwareRev firmwareRevision; // firmware Revision
};

struct  prevBootChannelList
{
  struct  prevBootDimmList dimmList[MAX_DIMM];
};

struct  prevBootSocket
{
  struct  prevBootChannelList channelList[MAX_CH];
};

struct prevBootNGNDimmCfg
{
  struct  prevBootSocket  socket[MAX_SOCKET];
};

struct odtActStruct {
   UINT16 config;          // ODT_ACT_INDEX
   UINT16 actBits[2];      // Bits[3:0]  = D0_R[3:0]
                           // Bits[7:4]  = D1_R[3:0]
                           // Bits[11:8] = D2_R[3:0]
                           // Bit[15] = MCODT
};

extern struct odtValueStruct odtValueTable[];
extern struct odtActStruct odtActTable[];

//
// Define the WDB line. The WDB line is like the cache line.
//
#define MRC_WDB_LINES                 32
#define MRC_WDB_LINE_SIZE             64
#define MRC_WDB_PATTERN_P             0x55555555
#define MRC_WDB_PATTERN_N             0xAAAAAAAA
#define CADB_LINES                    16
#define NORMAL_MRS                    0           //NORMAL_MRS  - How writeCADBcmd usually works
#define LOAD_MRS                      BIT0        //LOAD_MRS - writeCADBcmd will load commands into cadb buffer but not execute
#define EXECUTE_MRS                   BIT1        //EXECUTE_MRS - writeCADBcmd will execute the stack of commands
#ifdef MRS_STACKING
#define CADB_CACHELINE_LIMIT          15
#endif

#if defined(HSX_HOST) || defined(BDX_HOST)
struct CpgcSaveStruct {
  UINT8                                     DQPat;
  UINT8                                     DQPatLC;
  CPGC_SEQ_RANKLG2PHYMAP0_MC_MAINEXT_STRUCT SeqRankLg2PhyMap0;
  CPGC_SEQ_LRRANKLG2PHYMAP0_MC_MAINEXT_HSX_BDX_STRUCT seqLrRankLg2PhyMap0;
  CPGC_MISCREFCTL_MCDDC_CTL_STRUCT          miscRefCtl;
  CPGC_MISCZQCTL_MCDDC_CTL_STRUCT           miscZqCtl;
  CPGC_MISCCKECTL_MCDDC_CTL_HSX_BDX_STRUCT      miscCKECtl;
  CPGC_PATWDB_RDWR_PNTR_MCDDC_DP_STRUCT     patWDBRdWrPntr;
  CPGC_PATWDB_WR0_MCDDC_DP_STRUCT           patWDBWr0;
  CPGC_PATWDB_WR1_MCDDC_DP_STRUCT           patWDBWr1;
  DATACONTROL0_0_MCIO_DDRIO_STRUCT          dataControl0[MAX_STROBE / 2];
  CPGC_SEQCTL0_MC_MAINEXT_STRUCT            seqCtl0;
  CPGC_SUBSEQ0_CTL0_MC_MAINEXT_STRUCT       subSeqCtl0;
  CPGC_SUBSEQ1_CTL0_MC_MAINEXT_STRUCT       subSeqCtl1;
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAINEXT_STRUCT seqBaseAddrStartLo0;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAINEXT_STRUCT seqBaseAddrStartHi0;
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAINEXT_STRUCT seqBaseAddrWrapLo0;
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAINEXT_STRUCT seqBaseAddrWrapHi0;
  CPGC_SEQ_BAOCIC0_MC_MAINEXT_STRUCT        seqBAOCIC0;
  CPGC_SEQ_BAINCCTL00_MC_MAINEXT_STRUCT     seqBAIncCtl00;
  CPGC_SEQ_BAINCCTL10_MC_MAINEXT_HSX_BDX_STRUCT seqBAIncCtl10;
  CPGC_PATWDBCLCTL_MCDDC_CTL_HSX_BDX_STRUCT     patWDBCLCtl;
  CPGC_PATWDBCL_MUXCTL_MCDDC_DP_HSX_BDX_STRUCT  patWDBCLMuxCtl;
  CPGC_ERR_CTL_MCDDC_CTL_STRUCT         errCtl;
  CPGC_SEQ_DMYRDADDR_LO_MC_MAINEXT_STRUCT   seqDmyRdAddr;
  CPGC_SEQ_DMYRDCTL0_MC_MAINEXT_STRUCT      seqDmyRdCtl;
  CPGC_PATWDB_INV_MCDDC_DP_STRUCT           patWdbInv;
  CPGC_PATWDBCL_MUX0_PBWR_MCDDC_DP_STRUCT   patWdbClMux0Pbwr;
  CPGC_PATWDBCL_MUX1_PBWR_MCDDC_DP_STRUCT   patWdbClMux1Pbwr;
  CPGC_PATWDBCL_MUX2_PBWR_MCDDC_DP_STRUCT   patWdbClMux2Pbwr;
  CPGC_PATWDBCL_MUX0_PBRD_MCDDC_DP_STRUCT   patWdbClMux0Pbrd;
  CPGC_PATWDBCL_MUX1_PBRD_MCDDC_DP_STRUCT   patWdbClMux1Pbrd;
  CPGC_PATWDBCL_MUX2_PBRD_MCDDC_DP_STRUCT   patWdbClMux2Pbrd;
  CPGC_ERR_DATA0_MCDDC_CTL_STRUCT           errData0;
  CPGC_ERR_DATA1_MCDDC_CTL_STRUCT           errData1;
  CPGC_ERR_ECC_MCDDC_CTL_STRUCT             errEcc;
};
#endif

#define  MAX_CMD        3
#define  MAX_CLK        4

#define SIDE_A  0
#define SIDE_B  1

#ifdef DEBUG_PERFORMANCE_STATS
#define  MAX_NOZONE     20
#endif  // DEBUG_PERFORMANCE_STATS

//
// Define in the Critical Section function on what to wait.
//
typedef enum {
  DoneAndRefDrained,
  Done,
  Immediate
} EWaitOn;

typedef enum {
  ssOne             = 0,
  ssTwo,
  ssThree,
  ssFour,
} TSubSequencesNumber;

//
// Module definitions
//
#define RE_LOGIC_DELAY_MAX_VALUE        2   // define receive enable logic delay max value
#define RE_EDGE_SIZE_THRESHOLD          3   // define the min of valid one edge size
#define RE_MAX_LOGIC_DELAY              7   // max logic delay value
#define RE_TRAINING_RESULT_HIGH_IN_BITS 32  // bit start of the training result
#define MAX_PHASE_IN_FINE_ADJUSTMENT    64
#define MAX_PHASE_IN_READ_ADJUSTMENT    72
#define MAX_PHASE_IN_READ_ADJUSTMENT_DQ 120 // larger range for added DQ 1/16 PI adjustments
#define RAS_ROW_BITS                    6   // (16 row bits) define the number of row bits from RAS command
#define RAS_LR_ROW_BITS                 1   // (18 row bits) define the number of row bits from RAS command
#define CAS_ROW_BITS                    0   // (10 column bits) for CAS command
#define MRC_ROUND_TRIP_DEFAULT_VALUE    73  // Round trip default starting value
#define MRC_ROUND_TRIP_MAX_VALUE        88  // Round trip max value according mas
#define MRC_IO_LATENCY_DEFAULT_VALUE    4   // IO latency default value according mas
#define MRC_ROUND_TRIP_IO_COMPENSATION  14  // Roundtrip - IO compensation
#define MRC_ROUND_TRIP_IO_COMP_START    14  // Roundtrip - IO compensation starting point
#define TX_LOGIC_DELAY_MAX_VALUE        2   // define write max logic delay
#define ALL_STROBES                     0xFF
#define ALL_BITS                        0xFF
#define MRC_ROUND_TRIP_MAX_SCR_B0_VALUE 47  // MAX Round Trip value scrambler can handle - Fixed in C0 (with Throttling)
#define TX_PER_BIT_SETTLE_TIME          38
#define WR_DQ_DQS_BASIC_OFFSET_BDX      64
#define WR_DQ_DQS_BASIC_OFFSET_HSX      0


#define   REC_EN_STEP_SIZE  1
#define   REC_EN_LOOP_COUNT 7
#define   REC_EN_PI_START   0
#define   REC_EN_PI_RANGE   128

#define   LRDIMM_BACKSIDE_PI_START   0
#define   LRDIMM_BACKSIDE_PI_RANGE   128
#define   LRDIMM_BACKSIDE_STEP_SIZE  2

#define   LRDIMM_BACKSIDE_READ_DELAY_START   -15
#define   LRDIMM_BACKSIDE_READ_DELAY_END      15
#define   LRDIMM_BACKSIDE_READ_DELAY_STEP_SIZE  1

#define   LRDIMM_BACKSIDE_WRITE_DELAY_START   -15
#define   LRDIMM_BACKSIDE_WRITE_DELAY_END      15
#define   LRDIMM_BACKSIDE_WRITE_DELAY_STEP_SIZE  1

#define   WR_LVL_STEP_SIZE  1
#define   WR_LVL_LOOP_COUNT 1
#define   WR_LVL_PI_START   128
#define   WR_LVL_PI_RANGE   128

#define   WR_DQ_STEP_SIZE  1
#define   WR_DQ_LOOP_COUNT 1
#define   WR_DQ_PI_START   0
#define   WR_DQ_PI_RANGE   128

//
// define the WDB line size.
//
#define MRC_WDB_BYTES_PER_TRANSFER    8
#define MRC_WDB_TRANSFERS_PER_LINE    8

#define SUB_SEQUENCES_NUMBER  4
#ifdef SSA_FLAG
#define MRC_WDB_NUM_MUX_SEEDS   3  // Number of WDB Mux
#define MRC_CADB_NUM_MUX_SEEDS  4  // Number of CADB Mux
#endif  //SSA_FLAG

#pragma pack()

#define CMD_STEP_SIZE   4
#define EARLY_CMD_STEP_SIZE   2
#define MAX_CMD_MARGIN  255
#define MAX_CMD_VREF    63
#define PAR_TEST_START  0
#define PAR_TEST_RANGE  256
#define MAX_SAMPLES     128

// CMD Normalization
#define DCLKPITICKS 128
#define PITICKRANGE 511
#define EXTRAPIMARGIN 32
#define DENORMALIZE       -1
#define RENORMALIZE        1

//
// TX EQ equates
//
#define MAX_TX_EQ_BDX   16


//
// Timing mode Equates
//
#define TIMING_1N             0
#define TIMING_2N             2
#define TIMING_3N             3

//#define MAX_GROUPS            5


//
// defines for Receive enable
//
#define   RT_GUARDBAND               4
#define   MIN_IO_LATENCY             0
#define   DECREMENT_ROUNDTRIP_VALUE  2 // DCLK
#define   FINE_DELAY_FEEDBACK_MSK    0x1ff
#define   RESULT_STROBE_INDEX        MAX_STROBE

//
// defines for mem ODT
//
#define  DDR_FREQ_LIMIT     DDR_2400

// MemTiming.c
// Min Timings (HSD 4168317)
#define MIN_WWDR    3
#define MIN_WWDD    3

#endif // ASM_INC

#define HSX_PCKG_TYPE 4

#define CHIP_IOGPDLY_PSECS HSX_PCKG_TYPE //CMD_CTL_DELAY_H

//
// Platform Assigned CSRs for PIPE/SOCKET/MRC MILESTONES, from Pipe
//
#define SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_MMIO 0x4015b094 // BIOSNONSTICKYSCRATCHPAD13_1_11_3_CFG_REG
#define SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR  (BIOSNONSTICKYSCRATCHPAD13_UBOX_MISC_REG)  // UBOX scratchpad CSR13
#define SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR_MMIO  0x4015b098 // BIOSNONSTICKYSCRATCHPAD14_1_11_3_CFG_REG
#define SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR   (BIOSNONSTICKYSCRATCHPAD14_UBOX_MISC_REG)  // UBOX scratchpad CSR14
#define SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_MMIO     0x4015b09c // BIOSNONSTICKYSCRATCHPAD15_1_11_3_CFG_REG
#define SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR      (BIOSNONSTICKYSCRATCHPAD15_UBOX_MISC_REG)  // UBOX scratchpad CSR15

//
// Originally of SysRegs.h and heavily consumed.
//
#define GRANTLEY_BIOS_HEADER_OVERRIDE 1
#define GRANGEVILLE_BIOS_HEADER_OVERRIDE  1

//
// HSX specific defines
//
#define TCDBP_MCDDC_CTL_HSX_STRUCT             TCDBP_MCDDC_CTL_HSX_BDX_STRUCT
#define TCRFP_MCDDC_CTL_HSX_STRUCT             TCRFP_MCDDC_CTL_HSX_BDX_STRUCT
#define TCOTHP_MCDDC_CTL_HSX_STRUCT            TCOTHP_MCDDC_CTL_STRUCT
#define TCSTAGGER_REF_MCDDC_CTL_HSX_STRUCT     TCSTAGGER_REF_MCDDC_CTL_HSX_BDX_STRUCT
#define TCRWP_MCDDC_CTL_HSX_STRUCT             TCRWP_MCDDC_CTL_STRUCT
#define TCRAP_MCDDC_CTL_STRUCT                 TCRAP_MCDDC_CTL_HSX_BDX_STRUCT
#define TCSRFTP_MCDDC_CTL_STRUCT               TCSRFTP_MCDDC_CTL_STRUCT
#define TCZQCAL_MCDDC_CTL_STRUCT               TCZQCAL_MCDDC_CTL_STRUCT
#define RDIMMTIMINGCNTL2_MCDDC_CTL_STRUCT      RDIMMTIMINGCNTL2_MCDDC_CTL_HSX_BDX_STRUCT
#define TCLRDP_MCDDC_CTL_STRUCT                TCLRDP_MCDDC_CTL_HSX_BDX_STRUCT
#define TCMR2SHADOW_MCDDC_CTL_HSX_STRUCT       TCMR2SHADOW_MCDDC_CTL_HSX_BDX_STRUCT

extern UINT32  rdOdtTableRegs[MAX_DIMM];
extern UINT32  wrOdtTableRegs[MAX_DIMM];

// MemTraining.c
#define RD_WR_CMD_INIT_MARG  5
#define RX_TX_DQ_INIT_MARG   15

typedef union {
struct  {
   UINT32 selector:1;
   UINT32 index:4;
   UINT32 rsvd_8:11;
   UINT32 datavalue:16;
}Bits;
UINT32 Data;
}CMD_DRAM_PM_WRITE_PWR_COEFF;

// if GROVEPORT_FLAG
#ifndef MAX_RIR
#define MAX_RIR                  1
#endif
#ifndef MAX_RIR_WAYS
#define MAX_RIR_WAYS             1
#endif
#ifndef MC_MAX_NODE
#define MC_MAX_NODE              1
#endif
#ifndef MAX_HA
#define MAX_HA                   1
#endif
// end

// if GROVEPORT_FLAG || HSX_HOST || BDX_HOST
#ifndef MAX_RIR_DDRT
#define MAX_RIR_DDRT             1
#endif
#ifndef MAX_IIO_STACK
#define MAX_IIO_STACK            1
#endif
// end

#pragma pack(1)
struct TADTable {
  UINT8   Enable;         // Rule enable
  UINT32  Limit;          // Limit of the current TAD entry

  // if SKX_HOST || HSX_HOST || BDX_HOST
  UINT8   Mode;           // Interleave Mode
  UINT8   SocketWays;     // Socket Interleave ways for TAD
  UINT8   ChannelWays;    // Channel Interleave ways for TAD
  UINT32  ChannelAddressLow;  // Base channel address (in channel address space)
  UINT32  ChannelAddressHigh; // Base channel address (in channel address space)
  // end

  // if GROVEPORT_FLAG
  UINT8   Ways;           // Interleave Ways
  UINT8   Cache;          // Used as cache
  UINT32  Offset;         // Channel Offset in 64MB unit (used in SA to CA calculation)
  UINT8   ChMask;         // Channel paticipation list
}; // TADTable

struct SADTable {
  UINT8   Enable;         // Rule enable
  UINT32  Limit;          // Limit of the current SAD entry
  UINT8   ways;           // Interleave ways for SAD
  UINT8   IotEnabled;     // To indicate if IOT is enabled
  UINT8   mirrored;       //To Indicate the SAD is mirrored while enabling partial mirroring

  // if HSX_HOST || BDX_HOST
  UINT8   Mode;           // Interleave Mode
  // end

  // if GROVEPORT_FLAG
  UINT8   Mod3Mode;       // Mod3 Interleave Mode
  UINT8   Cacheable;      // Indicates if this region cached in MCDRAM
  UINT8   McdramRegion;   // Indicates if this region belongs to MCDRAM
  UINT8   ClusterNum;     // Cluster number that this range belongs to
  UINT8   McChMask[MAX_IMC]; // Channel paticipation list on each MC
  // end

  // if SKX_HOST
  UINT8   type;           // Bit0: 1LM  Bit1: 2LM  Bit2: PMEM  Bit3: PMEM-cache  Bit4: BLK Window  Bit5: CSR/Mailbox/Ctrl region
  UINT8   granularity;    // Interleave granularity for current SAD entry.  Possible interleave granularity options depend on the SAD entry type.  Note that SAD entry type BLK Window and CSR/Mailbox/Ctrl region do not support any granularity options
  UINT8   channelInterBitmap;   //Bit map to denote which channels are interleaved eg: 111b - Ch 2,1 & 0 are interleaved; 011b denotes Ch1 & 0 are interleaved
  UINT8   FMchannelInterBitmap;   //Bit map to denote which channels are interleaved in FM Only valid for 2LM eg: 111b - Ch 2,1 & 0 are interleaved; 011b denotes Ch1 & 0 are interleaved
  UINT8   imcInterBitmap;       //Bit map to denote which channels are interleaved eg: 11b both IMcs in the socket are interleaved 01 denotes only IMC0 is interleaved.
  BOOLEAN local;          //0 - Remote 1- Local
  // end
}; // SADTable

struct PADTable {
  UINT8   Enable;             // Rule enable
  UINT8   Length;             // Length of directed-pcommit range (64B granularity)
  UINT32  SystemAddressLow;   // Start of directed-pcommit range in system address space
  UINT32  SystemAddressHigh;  // Start of directed-pcommit range in system address space
};

struct homeAgent {
  UINT8             haNum;                                  // ha Number
  UINT16            memSize;                                // Memory size of this ha
#ifdef NVMEM_FEATURE_EN
  UINT16            NVmemSize;                              // Memory size of this ha
#endif
  UINT16            remSize;                                // Remaing memory for this ha
  UINT8             TADintList[TAD_RULES][MAX_TAD_WAYS];    // TAD interleave list for this socket
  UINT8             TADChnIndex[TAD_RULES][MAX_TAD_WAYS];   // Corresponding TAD channel indexes (per channel)
  UINT32            TADOffset[TAD_RULES][MAX_TAD_WAYS];     // Corresponding TAD offsets (per channel)
  struct TADTable   TAD[TAD_RULES];                         // TAD table
  UINT8             haChannelListStartIndex;                // Index in channel list of first channel on this HA
  struct PADTable   PAD[MAX_PAD];                           // PAD table for NG DIMMs
};

struct haNvram {
  UINT8                 enabled;                    // 0 - HA disabled, 1 - HA enabled
  UINT8                 haNodeId;                   //  System wide socket id for HA
  UINT8                 haNum;                      // HA number within socket
  UINT8                 haNvChannelListStartIndex;  // Index in nvram channel list of first channel on this HA
};

#pragma pack()

//
// Chip specific for now until this parameter gets optimized
//
#define MAX_ODT_SETTINGS   20

//
// Chip specific section of struct memSetup
//
#define MEMSETUP_CHIP_COMMON   \


#ifdef DE_SKU 
#define MEMSETUP_CHIP     \
  UINT8                 BlockScTrafficOnAdr;     /* If set to 1, then send b2p message to block all PCIe/SC traffic on ADR trigger.*/   \
  MEMSETUP_CHIP_COMMON  \
 

#else
#define MEMSETUP_CHIP     \
  MEMSETUP_CHIP_COMMON  \
 

#endif

//
// Chip specific section of struct memVar
//
#define MEMVAR_CHIP     \
  UINT8                   numChPerHA;       /* Number of Channels per HA */             \
  UINT8                   InterleaveMode;   /* Interleave mode (1 = xor, 0 = none) */   \

//
// Chip specific section of struct Socket
//
#define SOCKET_CHIP     \
  struct homeAgent  haList[MAX_HA];

//
// Chip specific section of struct ddrChannel
//
#define DDRCHANNEL_CHIP     \

//
// Chip specific section of struct dimmDevice
//
#define DIMMDEVICE_CHIP     \

//
// Chip specific section of struct rankDevice
//
#define RANKDEVICE_CHIP     \

//
// Chip specific section of struct socketNvram
//
#define SOCKETNVRAM_CHIP     \
  struct haNvram      haList[MAX_HA];           \

//
// Chip specific section of struct socketNvram
//
#define MEMNVRAM_CHIP     \
  UINT8           lvlshft_holden;           \

//
// Chip specific section of struct imcNvram
//
#define IMCNVRAM_CHIP     \

//
// Chip specific section of struct channelNvram
//
#define CHANNELNVRAM_CHIP     \
  UINT8             twoXRefreshSetPerCh;  /* Saves the refreshRate value for each channel */                     \
  UINT32            tclrdp1;                                                                                     \
  UINT32            cmdCsr[MAX_SIDE][3];                                                                         \
  UINT32            ctlCsr[MAX_SIDE][3];                                                                         \

//
// Chip specific section of struct dimmNvram
//
#define DIMMNVRAM_CHIP     \

//
// Chip specific section of struct ddrRank
//
#define DDRRANK_CHIP     \

//
// Chip specific section of struct comTime
//
#define COMTIME_CHIP     \

//
// Power training defines
//
#define MAX_DRAMRON     2
#define MAX_MCODT       2
#define MAX_NONTGTODT   9
#define MAX_RTTWR       3
#define MAX_MCRON       3
#define MAX_TX_EQ       15
#define MAX_IMODE       8
#define MAX_CTLE        10

#endif // _memhostchip_h
