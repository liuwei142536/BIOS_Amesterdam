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

#ifndef  _mem_defaults_h
#define  _mem_defaults_h

#include <Token.h> // AptioV server override

#define AUTO                  0

#define TEMPHIGH_EN           BIT0
#define PDWN_MODE_APD         BIT1
#define PDWN_MODE_PPD         BIT2
#define PDWN_MODE_SLOW_EXIT   BIT3
#define PDWN_MODE_IBT_OFF_EN  BIT4
#define PDWN_IDLE_CNTR        0x40

//
// RAS Mode
//
#define MIRROR_EN             BIT0    // Mirror Mode
#define LOCKSTEP_EN           BIT1    // Lockstep Mode


//
// A7 Mode
//
#define A7_MODE_DISABLE       0
#define A7_MODE_ENABLE        1
#define A7_MODE_AUTO          2
#define A7_MODE_DEFAULT       A7_MODE_ENABLE

//
// MC_ODT
//
#define MC_ODT_50_OHMS        0
#define MC_ODT_100_OHMS       1
#define MC_ODT_AUTO           2
#define MC_ODT_DEFAULT        MC_ODT_50_OHMS

//
// ALLOW_CORRECTABLE_ERRORS
//
#define ALLOW_CORRECTABLE_ERROR_DISABLE     0
#define ALLOW_CORRECTABLE_ERROR_ENABLE      1
#define ALLOW_CORRECTABLE_ERROR_AUTO        ALLOW_CORRECTABLE_ERROR_ENABLE
#define ALLOW_CORRECTABLE_ERROR_DEFAULT     ALLOW_CORRECTABLE_ERROR_AUTO

//
// Memory Power Savings Mode
//
#define MEM_PWR_SAV_DISABLE   0       // Disable the PPM control and force this to be off all the time
#define MEM_PWR_SAV_SLOW      1       // PPDS with processor control of parameters
#define MEM_PWR_SAV_FAST      2       // PPDF with processor control of parameters
#define MEM_PWR_SAV_APD       3       // PPDF with processor control of parameters
#define MEM_PWR_SAV_USER      4       // Disable processor control of parameters, set values manually
#define MEM_PWR_SAV_AUTO      5
#define MEM_PWR_SAV_DEFAULT_DDR3   MEM_PWR_SAV_FAST
#define MEM_PWR_SAV_DEFAULT_DDR4   MEM_PWR_SAV_FAST
#define MEM_PWR_SAV_DEFAULT_HSX    MEM_PWR_SAV_APD
#define ENERGY_PERF_MODE     0

//
// CKE Throttling
//
#define CKE_MODE_OFF          0       // CKE Throttling Disabled
#define CKE_APD               1       // Advanced Power Down Enabled, Pre-charge Power Down Disabled
#define CKE_PPDF              2       // Advanced Power Down Disabled, Pre-charge Power Down Fast Enabled
#define CKE_PPDS              3       // Advanced Power Down Disabled, Pre-charge Power Slow Enabled
#define CKE_APD_PPDF          4       // Advanced Power Down Enabled, Pre-charge Power Down Enabled
#define CKE_APD_PPDS          5       // Advanced Power Down Enabled, Pre-charge Power Slow Enabled
#define CKE_MODE_AUTO         6
#define CKE_MODE_DEFAULT   CKE_APD

//
// CK/CK# OFF for Self Refresh
//
#define CK_SELF_REF_DRIVEN    0x00    // CK driven during self refresh
#define CK_SELF_REF_TRI_STATE 0x1     // CK tri-stated during self refresh
#define CK_SELF_REF_LOW       0x2     // CK pulled low during self refresh
#define CK_SELF_REF_HIGH      0x3     // CK pulled high during self refresh
#define CK_SELF_REF_AUTO      0x4     // setting depends on the DIMM type

//
// Opportunistic Self-Refresh
//
#define OPP_SR_DIS            0
#define OPP_SR_EN             1
#define OPP_SR_AUTO           2
#define OPP_SR_DEFAULT        OPP_SR_EN

//
// Opportunistic Reads in WMM
//
#define OPP_RD_WMM_DIS            0
#define OPP_RD_WMM_EN             1
#define OPP_RD_WMM_AUTO           2
#define OPP_RD_WMM_DEFAULT        OPP_RD_WMM_EN

//
// MDLL Shut Down Enable
//
#define MDLL_SDEN_DIS         0
#define MDLL_SDEN_EN          1
#define MDLL_SDEN_AUTO        2
#define MDLL_SDEN_DEFAULT     MDLL_SDEN_DIS

//
// MEMHOT Throttling Mode
//
#define MEMHOT_DIS            0
#define MEMHOT_OUTPUT_ONLY    1
#define MEMHOT_INPUT_ONLY     2
#define MEMHOT_INPUT_OUTPUT_EN 3

//
// Throttling Register Values for CLTT per the Thermal Throttling whitepaper v0.1
// (Grantley CLTT whitepaper 112812-04.doc)
//
#define CLTT_THRT_MID_DEFAULT 255
#define CLTT_THRT_HI_DEFAULT 26   // = 255 * 10% * 1 rounded up
#define CLTT_THRT_CRIT_DEFAULT 3  // 0.1GB/s/DIMM

#define OLTT_THRT_MID_DEFAULT 255
#define OLTT_THRT_HI_DEFAULT 204   // = 255 * 80% * 1 rounded up

#define CLTT_TEMP_LO_SINGLE_REFRESH_DEFAULT 82
#define CLTT_TEMP_MID_SINGLE_REFRESH_DEFAULT 82
#define CLTT_TEMP_HI_SINGLE_REFRESH_DEFAULT 100

#define CLTT_TEMP_LO_DOUBLE_REFRESH_DEFAULT 84
#define CLTT_TEMP_MID_DOUBLE_REFRESH_DEFAULT 93
#define CLTT_TEMP_HI_DOUBLE_REFRESH_DEFAULT 100

#define OLTT_TEMP_LO_DEFAULT 82
#define OLTT_TEMP_MID_DEFAULT 82
#define OLTT_TEMP_HI_DEFAULT 255

#define TT300M_DIMM_TEMP_OFFSET_DEFAULT 27
#define TT900M_DIMM_TEMP_OFFSET_DEFAULT 27
#define TT1500M_DIMM_TEMP_OFFSET_DEFAULT 27
#define TT_MAX_WORST_CASE_DIMM_TEMP_OFFSET 15

//
// Dram Power Table Default Values
//
#define UDIMM_MAX_POWER_DEFAULT 93
#define UDIMM_DRAM_TDP_DEFAULT 47
#define UDIMM_MIN_PWR_CLOSED_PAGE_DEFAULT 15
#define UDIMM_WRITE_PWR_SCALE_CLOSED_PAGE_DEFAULT 943
#define UDIMM_MAX_PWR_OPEN_PAGE_DEFAULT 88
#define UDIMM_TDP_OPEN_PAGE_DEFAULT 49
#define UDIMM_MIN_PWR_OPEN_PAGE_DEFAULT 18
#define UDIMM_WRITE_PWR_SCALE_OPEN_PAGE_DEFAULT 1066
#define UDIMM_REFRESH_RATE_SLOPE_DEFAULT 1663
#define UDIMM_SELF_REFRESH_POWER_DEFAULT 584

#define LRDIMM_MAX_POWER_DEFAULT 212
#define LRDIMM_DRAM_TDP_DEFAULT 141
#define LRDIMM_MIN_PWR_CLOSED_PAGE_DEFAULT 52
#define LRDIMM_WRITE_PWR_SCALE_CLOSED_PAGE_DEFAULT 466
#define LRDIMM_MAX_PWR_OPEN_PAGE_DEFAULT 203
#define LRDIMM_TDP_OPEN_PAGE_DEFAULT 157
#define LRDIMM_MIN_PWR_OPEN_PAGE_DEFAULT 75
#define LRDIMM_WRITE_PWR_SCALE_OPEN_PAGE_DEFAULT 506
#define LRDIMM_REFRESH_RATE_SLOPE_DEFAULT 6650
#define LRDIMM_SELF_REFRESH_POWER_DEFAULT 2332

#define RDIMM_MAX_POWER_DEFAULT 191
#define RDIMM_DRAM_TDP_DEFAULT 116
#define RDIMM_MIN_PWR_CLOSED_PAGE_DEFAULT 49
#define RDIMM_WRITE_PWR_SCALE_CLOSED_PAGE_DEFAULT 710
#define RDIMM_MAX_PWR_OPEN_PAGE_DEFAULT 182
#define RDIMM_TDP_OPEN_PAGE_DEFAULT 137
#define RDIMM_MIN_PWR_OPEN_PAGE_DEFAULT 72
#define RDIMM_WRITE_PWR_SCALE_OPEN_PAGE_DEFAULT 776
#define RDIMM_REFRESH_RATE_SLOPE_DEFAULT 6650
#define RDIMM_SELF_REFRESH_POWER_DEFAULT 2369

#define DRAM_RAPL_REFRESH_BASE_DEFAULT 10

//
// DRAM RAPL Extended Range Options
//
#define DRAM_RAPL_EXTENDED_RANGE_DISABLE 0
#define DRAM_RAPL_EXTENDED_RANGE_ENABLE 1
#define DRAM_RAPL_EXTENDED_RANGE_DEFAULT DRAM_RAPL_EXTENDED_RANGE_ENABLE

//
// Platform specific value to be programmed into the TSOD CONFIG register
//
//#define OEM_MTS_CONFIG_VALUE    MTS_CFG_TCRIT_ONLY + MTS_CFG_EVENT_CTRL
#define OEM_MTS_CONFIG_VALUE    0

//
// Electrical Throttling Options
//
#define ET_DISABLE  0
#define ET_ENABLE   1
#define ET_AUTO     2
// TODO: Make sure this is disabled for PO

//
// High Temperature Enable
//
#define HTE_DISABLE  0
#define HTE_ENABLE   1
#define HTE_AUTO     2
#define HTE_DEFAULT  HTE_DISABLE

//
// Allow 2X Refresh enable
//
#define A2R_DISABLE  0
#define A2R_ENABLE   1
#define A2R_AUTO     2
#define A2R_DEFAULT  A2R_ENABLE

//
// CustomRefreshRate
//
#define Cust_Refresh_Rate_MIN  20
#define Cust_Refresh_Rate_MAX  40
#define Cust_Refresh_Rate_Default  Cust_Refresh_Rate_MIN

//
// Altitude
//
#define UNKNOWN   0
#define ALT_300M  1
#define ALT_900M  2
#define ALT_1500M 3
#define ALT_3000M 4
//
// Page Policy
//
#define CLOSED_PAGE_DIS       0       // Closed Page mode disabled (Open page enabled)
#define CLOSED_PAGE_EN        1       // Closed Page mode enabled
#define OPEN_PAGE_ADAPTIVE    2       // Adaptive Open page mode enabled
#define CLOSED_PAGE_AUTO      3
#define CLOSED_PAGE_DEFAULT   OPEN_PAGE_ADAPTIVE

// Memory Type
#define RDIMM_ONLY            0       // RDIMMs only
#define UDIMM_ONLY            1       // UDIMMs (and SODIMMs) only
#define RDIMMandUDIMM         2       // Both RDIMMs and UDIMMs supported
#define MEM_TYPE_AUTO         RDIMMandUDIMM

// DDR3 Frequency
#define DDR3_FREQ_AUTO        AUTO
#define DDR3_FREQ_DEFAULT     DDR3_FREQ_AUTO

// DDR3 Voltage
#define DDR3_VDD_AUTO         0
#define DDR3_VDD_150          1
#define DDR3_VDD_135          2
#define DDR3_VDD_DEFAULT      DDR3_VDD_AUTO

// ECC Support
#define ECC_DIS               0       // Disable ECC support
#define ECC_EN                1       // Enable ECC support
#define ECC_AUTO              2
#define ECC_DEFAULT           ECC_EN

  // 3 month timeout for re-training of memory
#define ENFORCE_TIMEOUT_DIS               0       // Disable 3 month timeout for re-training of memory
#define ENFORCE_TIMEOUT_EN                1       // Enable 3 month timeout for re-training of memory
#define ENFORCE_TIMEOUT_AUTO              2
#define ENFORCE_TIMEOUT_DEFAULT           ENFORCE_TIMEOUT_EN


// Log Parsing - option of enabling additional information in the log to make parsing it more generic and easier in MT
#define LOG_PARSING_DIS     0     // Disabled
#define LOG_PARSING_EN      1     // Enabled

// LRDIMM Rank Multiplication
#define RANK_MULT_AUTO      0     // Choose RM factor based on #DPC
#define RANK_MULT_EN        1     // Force RM enabled

// LRDIMM Module Delay
#define MODULE_DELAY_AUTO   0       // Boundary check SPD MD and use if good, else defaults
#define MODULE_DELAY_DIS    1       // Do NOT use SPD MD. Use Defaults

// Memory test enable
#define MEM_TEST_DIS       0          // Disable memory test
#define MEM_TEST_EN        1          // Enable memory test
#define MEM_TEST_AUTO      2
#define MEM_TEST_DEFAULT   MEM_TEST_EN

// Software Memory test enable
#define SW_MEM_TEST_DIS    0          // Disable memory test
#define SW_MEM_TEST_EN     1          // Enable memory test
#define SW_MEM_TEST_AUTO      SW_MEM_TEST_DIS

// Memory test enable for fast boot scenarios
#define MEM_TEST_FAST_BOOT_DIS      0      // Disable memory test during fast boot
#define MEM_TEST_FAST_BOOT_EN       1      // Enable memory test during fast boot
#define MEM_TEST_FAST_BOOT_AUTO     2
#define MEM_TEST_FAST_BOOT_DEFAULT  MEM_TEST_FAST_BOOT_DIS

// Do we attempt a fast boot path through MRC?
#define FAST_BOOT_DIS         0       // Disable fast boot (i.e. take the normal cold boot path through MRC)
#define FAST_BOOT_EN          1       // Enable fast boot (i.e. skip parts of MRC when possible)
#define FAST_BOOT_AUTO        2
#define FAST_BOOT_DEFAULT     FAST_BOOT_DIS

//
// Fast Cold Boot
//
#define FAST_BOOT_COLD_DIS         0       // Disable fast boot (i.e. take the normal cold boot path through MRC)
#define FAST_BOOT_COLD_EN          1       // Enable fast boot (i.e. skip parts of MRC when possible)
#define FAST_BOOT_COLD_AUTO        2
#define FAST_BOOT_COLD_DEFAULT     FAST_BOOT_COLD_DIS

#define RMT_COLD_FAST_BOOT_DIS     0
#define RMT_COLD_FAST_BOOT_EN      1
#define RMT_COLD_FAST_BOOT_AUTO    2
#define RMT_COLD_FAST_BOOT_DEFAULT RMT_COLD_FAST_BOOT_DIS

//
//Memory test loops
//
#define MEM_TEST_LOOPS_DEFAULT      1          // Number of MemTests to execute

//
// Advanced Memory Test algorithms using CPGC
//
#define ADV_MEM_TEST_DIS            0
#define ADV_MEM_TEST_EN             1
#define ADV_MEM_TEST_DEFAULT        ADV_MEM_TEST_DIS

//
// DRAM Maintenance Test
//
#define DMT_DIS               0
#define DMT_EN                1
#define DMT_AUTO              DMT_DIS
#define DMT_DIRECTION_UP      0
#define DMT_DIRECTION_DOWN    1
#define DMT_INVERT_DIS        0
#define DMT_INVERT_EN         1
#define DMT_REPETITIONS       3
#define DMT_ITERATIONS        1
#define DMT_SWIZZLE_DIS       0
#define DMT_SWIZZLE_EN        1
#define DMT_SWIZZLE_AUTO      2
#define DMT_REFRESH_DIS       0
#define DMT_REFRESH_EN        1


//
// DRAM Maintenance
//
#define DRAM_MAINT_DIS         0      // Disable DRAM Maintenance
#define DRAM_MAINT_MANUAL      1      // Manually configure DRAM Maintenance
#define DRAM_MAINT_AUTO        2      // Auto configure DRAM Maintenance

#define DRAM_MAINT_PTRR        0      // pTRR mode
#define DRAM_MAINT_TRR         1      // TRR mode

#define DRAM_MAINT_MODE_A      0      // TRR mode A
#define DRAM_MAINT_MODE_B      1      // TRR mode B
#define DRAM_MAINT_DEFAULT     DRAM_MAINT_MODE_B // Default to mode B

// DDR Cycling Support
#define DDR_CYCLING_DIS       0       // Disable DDR Cycling
#define DDR_CYCLING_EN        1       // Enable DDR Cycling

//
// Refresh parameter timing constraints
//
#define OREFNI                63
#define REF_HI_WM             8
#define REF_PANIC_WM          9

//
// ZQ Calibration Timing Parameter
//
#define ZQCS_PERIOD           128

//
// Self-refresh idle counter
//
#define SREF_IDLE_CNTR        0x3e800
//
// WDB Watermarks
//
#define WMM_ENTRY             0x1A
#define PWMM_ENTRY            0x24
#define WMM_ENTRY_AUTO        0xFF

#define WMM_EXIT              0x12
#define PWMM_EXIT             0x12
#define WMM_EXIT_AUTO         0xFF

#define STARVATION_COUNTER    0x80
#define PSTARVATION_COUNTER   0x80
#define STARVATION_AUTO       0xFFFF
//
// NUMA Mode
//
#define NUMA_DIS              0
#define NUMA_EN               1
#define NUMA_AUTO             2
#define NUMA_DEFAULT          NUMA_EN
#define SOCKET_1WAY           1
#define SOCKET_2WAY           2
#define SOCKET_4WAY           4
#define SOCKET_INTER_AUTO     SOCKET_2WAY

//
// Channel Interleave
//
#define CH_1WAY               1
#define CH_2WAY               2
#define CH_3WAY               3
#define CH_4WAY               4
#define CH_INTER_AUTO         AUTO
#define CH_INTER_DEFAULT      CH_4WAY

//
// Rank Interleave
//
#define RANK_1WAY             1
#define RANK_2WAY             2
#define RANK_4WAY             4
#define RANK_8WAY             8
#define RANK_INTER_AUTO       AUTO
#define RANK_INTER_DEFAULT    RANK_8WAY

//
// Socket Interleave Below 4G
//
#define SOCKET_INTLV_BELOW_4G_DIS 0
#define SOCKET_INTLV_BELOW_4G_EN  1


//
// RAS Mode
//
#define RAS_MODE_DISABLE      0
#define MIRROR_ENABLED        1
#define LOCKSTEP_ENABLED      2
#define SPARING_DISABLED      0
#define SPARING_ENABLED       1
#define ONE_RANK_SPARE        1
#define TWO_RANK_SPARE        2
#define THREE_RANK_SPARE      3
#define AUTO_RANK_SPARE       4

//
//  XMP Support
//
#define XMP_AUTO              0
#define XMP_MANUAL            1
#define XMP_PROFILE_1         2
#define XMP_PROFILE_2         3

//
//  BGF Threshold
//
#define BGF_THRESHOLD         4

//
// Scrambling support
//
#define SCRAMBLE_DISABLE        0       // Disable Data Scrambling
#define SCRAMBLE_ENABLE         1       // Enable Data Scrambling
#define SCRAMBLE_AUTO           2
#define SCRAMBLE_DEFAULT        SCRAMBLE_ENABLE
#define SCRAMBLE_SEED_LOW       41003
#define SCRAMBLE_SEED_HIGH      54165
#define MAX_SCRAMBLE_SEED_LOW   65535
#define MAX_SCRAMBLE_SEED_HIGH  65535

//
// Page Policy and Timing parameters
//
#define IDLE_PAGE_RST_VAL     8
#define WIN_SIZE              64
#define PPC_TH                6
#define OPC_TH                6

//
// Partial Write Starvation Counter
//
#define WPQ_IS                28
#define PWPQ_IS               41

//
// Channel PPDS idle counter
//
#define PPDS_IDLE_TIMER       0x200

//
// Rank Margin Tool
//
#define RMT_DIS               0
#define RMT_EN                1
#define RMT_AUTO              2
#define RMT_DEFAULT           RMT_DIS
#define RMT_PATTERN_LENGTH    32767
#define CMD_PATTERN_LENGTH    32767

//
// Pattern length for advanced training steps
//
#define TRAIN_RD_DQS_PL       64
#define TRAIN_WR_DQS_PL       64

//
// Minimum timing check
//
#define MIN_TIMING_LIMIT      5

//
// Multi-Threaded MRC
//
#define MULTI_THREADED_DIS    0
#define MULTI_THREADED_EN     1
#define MULTI_THREADED_AUTO   2
#define MULTI_THREADED_DEFAULT MULTI_THREADED_EN

//
// RPQAGE defaults
//
#define IO_COUNT              0x40
#define CPU_GT_COUNT          0x40

//
// Cross over Mode
//
#define XOVER_MODE_AUTO   0
#define XOVER_MODE_2TO2   1
#define XOVER_MODE_1TO1   2

//
// Cross over calibration
//
#define XOVER_CALIB_DIS       0
#define XOVER_CALIB_EN        1
#define XOVER_CALIB_AUTO      2
#define XOVER_CALIB_DEFAULT   XOVER_CALIB_EN

//
// Sense amp calibration
//
#define SENSE_AMP_DISABLE     0
#define SENSE_AMP_EN          1
#define SENSE_AMP_AUTO        2
#define SENSE_AMP_DEFAULT     SENSE_AMP_EN

//
// DRAM Ron
//
#define DRAM_RON_DISABLE      0
#define DRAM_RON_ENABLE       1
#define DRAM_RON_AUTO         2
#define DRAM_RON_DEFAULT      DRAM_RON_DISABLE

//
// RX ODT
//
#define RX_ODT_DISABLE        0
#define RX_ODT_ENABLE         1
#define RX_ODT_AUTO           2
#define RX_ODT_DEFAULT        RX_ODT_DISABLE

//
// RTT WR
//
#define RTT_WR_AUTO           0
#define RTT_WR_DISABLE        1
#define RTT_WR_ENABLE         2
#define RTT_WR_DEFAULT        RTT_WR_DISABLE

//
// MC RON
//
#define MC_RON_AUTO           0
#define MC_RON_DISABLE        1
#define MC_RON_ENABLE         2
#define MC_RON_DEFAULT        MC_RON_DISABLE

//
// Tx Equalization calibration
//
#define TX_EQ_DISABLE         0
#define TX_EQ_ENABLE          1
#define TX_EQ_AUTO            2
#define TX_EQ_DEFAULT         TX_EQ_ENABLE
#define TX_EQ_DEFAULT_HSX     TX_EQ_ENABLE

//
// iMode training
//
#define IMODE_DISABLE         0
#define IMODE_ENABLE          1
#define IMODE_AUTO            2
#define IMODE_DEFAULT         IMODE_DISABLE
#define IMODE_DEFAULT_HSX     IMODE_DISABLE

//
// RXCTLE training
//
#define RX_CTLE_DISABLE      0
#define RX_CTLE_ENABLE       1
#define RX_CTLE_AUTO         2
#define RX_CTLE_DEFAULT      RX_CTLE_ENABLE

//
// Early CMD/CLK Training
//
#define EARLY_CMD_CLK_DIS           0
#define EARLY_CMD_CLK_EN            1
#define EARLY_CMD_CLK_AUTO          2
#define EARLY_CMD_CLK_DEFAULT       EARLY_CMD_CLK_EN

//
// Early CTL/CLK Training
//
#define EARLY_CTL_CLK_DIS           0
#define EARLY_CTL_CLK_EN            1
#define EARLY_CTL_CLK_AUTO          2
#define EARLY_CTL_CLK_DEFAULT       EARLY_CTL_CLK_EN

//
// CMD Normalization
//
#define CMD_NORMAL_DISABLE          0
#define CMD_NORMAL_ENABLE           1
#define CMD_NORMAL_AUTO             2
#define CMD_NORMAL_DEFAULT          CMD_NORMAL_DISABLE // Applies to BDX only.

//
// Round Trip Latency
//
#define ROUND_TRIP_LATENCY_DIS      0
#define ROUND_TRIP_LATENCY_ENABLE   1
#define ROUND_TRIP_LATENCY_AUTO     2
#define ROUND_TRIP_LATENCY_DEFAULT  ROUND_TRIP_LATENCY_DIS

//
// CMD/CLK Training
//
#define CMD_CLK_DIS           0
#define CMD_CLK_EN            1
#define CMD_CLK_AUTO          2
#define CMD_CLK_DEFAULT       CMD_CLK_EN

//
// Rx Vref Training
//
#define RX_VREF_DISABLE       0
#define RX_VREF_ENABLE        1
#define RX_VREF_AUTO          2
#define RX_VREF_DEFAULT       RX_VREF_ENABLE

//
// Tx Vref Training
//
#define TX_VREF_DISABLE       0
#define TX_VREF_ENABLE        1
#define TX_VREF_AUTO          2
#define TX_VREF_DEFAULT       TX_VREF_ENABLE

//
// CMD Vref Training(BDX Only)
//
#define CMD_VREF_DISABLE       0
#define CMD_VREF_ENABLE        1
#define CMD_VREF_AUTO          2
#define CMD_VREF_DEFAULT       CMD_VREF_ENABLE 

//
// LRDIMM Backside Vref Training
//
#define LRDIMM_BACKSIDE_VREF_DISABLE         0
#define LRDIMM_BACKSIDE_VREF_ENABLE          1
#define LRDIMM_BACKSIDE_VREF_AUTO            2
#define LRDIMM_BACKSIDE_VREF_DEFAULT         LRDIMM_BACKSIDE_VREF_DISABLE  //HSX Default
#define LRDIMM_BACKSIDE_VREF_DEFAULT_BDX     LRDIMM_BACKSIDE_VREF_ENABLE   //BDX ML Default

#define LRDIMM_WR_VREF_DISABLE               0
#define LRDIMM_WR_VREF_ENABLE                1
#define LRDIMM_WR_VREF_AUTO                  2
#define LRDIMM_WR_VREF_DEFAULT               LRDIMM_WR_VREF_ENABLE  //Permanently disabled for HSX, applies to BDX only.

#define LRDIMM_RD_VREF_DISABLE               0
#define LRDIMM_RD_VREF_ENABLE                1
#define LRDIMM_RD_VREF_AUTO                  2
#define LRDIMM_RD_VREF_DEFAULT               LRDIMM_RD_VREF_ENABLE  //Permanently disabled for HSX, applies to BDX only.

#define LRDIMM_RX_DQ_CENTERING_DISABLE       0
#define LRDIMM_RX_DQ_CENTERING_ENABLE        1
#define LRDIMM_RX_DQ_CENTERING_AUTO          2
#define LRDIMM_RX_DQ_CENTERING_DEFAULT       LRDIMM_RX_DQ_CENTERING_ENABLE  //Permanently disabled for HSX, applies to BDX only.

#define LRDIMM_TX_DQ_CENTERING_DISABLE       0
#define LRDIMM_TX_DQ_CENTERING_ENABLE        1
#define LRDIMM_TX_DQ_CENTERING_AUTO          2
#define LRDIMM_TX_DQ_CENTERING_DEFAULT       LRDIMM_TX_DQ_CENTERING_ENABLE  //Permanently disabled for HSX, applies to BDX only.

//
// PDA
//
#define PDA_DISABLE           0
#define PDA_ENABLE            1
#define PDA_AUTO              2
#define PDA_DEFAULT           PDA_ENABLE

//
// Turnaround Time Optimization
//
#define TURNAROUND_DISABLE    0
#define TURNAROUND_ENABLE     1
#define TURNAROUND_AUTO       2
#define TURNAROUND_DEFAULT    TURNAROUND_ENABLE

//
// IOT MEMORY BUFFER RESERVATION
//
#define IOT_BUFFER_DEFAULT    0  // 1 = 64MB, 2= 128MB, so on till 256 = 16384MB (16GB)

//
// Enable Backside RMT
//
#define BACKSIDE_RMT_DISABLE     0
#define BACKSIDE_RMT_ENABLE      1
#define BACKSIDE_RMT_AUTO        2
#define BACKSIDE_RMT_DEFAULT     BACKSIDE_RMT_DISABLE

#ifdef SSA_FLAG
//
// Enabling the BIOS SSA loader
//
#ifdef RC_SIM
#define BIOS_SSA_LOADER_ENABLE            1
#define BIOS_SSA_DEFAULT                          BIOS_SSA_LOADER_ENABLE
#else
#define BIOS_SSA_LOADER_DISABLE           0
#define BIOS_SSA_LOADER_ENABLE            1
#define BIOS_SSA_DEFAULT                          BIOS_SSA_LOADER_DISABLE
#endif //RC_SIM
#endif //SSA_FLAG

//
// Option to offset the final memory training results
//
#define TRNG_RESULT_OFFSET_DISABLE     0
#define TRNG_RESULT_OFFSET_ENABLE      1
#define TRNG_RESULT_OFFSET_DEFAULT     TRNG_RESULT_OFFSET_DISABLE

//
// Per Bit DeSkew Training
//
#define BIT_DESKEW_DISABLE       0
#define BIT_DESKEW_ENABLE        1
#define BIT_DESKEW_AUTO          2
#define BIT_DESKEW_DEFAULT       BIT_DESKEW_DISABLE    //HSX
#define BIT_DESKEW_DEFAULT_BDX   BIT_DESKEW_ENABLE     //BDX ML
#define BIT_DESKEW_DEFAULT_DE    BIT_DESKEW_ENABLE     //DE

// AptioV Server Override
#define BIT_DESDKEW_DISABLE   0
#define BIT_DESDKEW_ENABLE    1
#define BIT_DESDKEW_AUTO      2
#define BIT_DESDKEW_DEFAULT   BIT_DESDKEW_DISABLE
//
// RCV Enable after CMD/CLK Training
//
#define RCV_EN_DIS            0
#define RCV_EN_EN             1
#define RCV_EN_AUTO           RCV_EN_EN

//
// Duty Cycle Training
//
#define DUTY_CYCLE_DISABLE    0
#define DUTY_CYCLE_ENABLE     1
#define DUTY_CYCLE_AUTO       2 // AptioV Server Override
#define DUTY_CYCLE_DEFAULT    DUTY_CYCLE_ENABLE // AptioV server override

//
// Patrol scrubbing
//
#define PATROL_SCRUB_DURATION_DEFAULT 24
#define PATROL_SCRUB_DURATION_MAX 24
#define PATROL_SCRUB_DIS      0
#define PATROL_SCRUB_EN       1
#define PATROL_SCRUB_INTERVAL_MIN 1500

//
// Demand Scrub
//
#define DEMAND_SCRUB_DIS      0
#define DEMAND_SCRUB_EN       1


//
// Leaky Bucket Default Values
//
#define LEAKY_BUCKET_LO       0x28
#define LEAKY_BUCKET_HI       0x29

//
// Phase Shedding Control
//
#define PHASE_SHEDDING_DIS    0
#define PHASE_SHEDDING_EN     1
#define PHASE_SHEDDING_AUTO   2
#define PHASE_SHEDDING_DEFAULT PHASE_SHEDDING_EN

//
// Multi-Vref algorithm defaults
//
#define RX_MULTI_VREF_POINTS  7
#define TX_MULTI_VREF_POINTS  7
#define MAX_VREF_POINTS       10    // Maximum number of points possible
#define RX_MULTI_VREF_STEP    4
#define TX_MULTI_VREF_STEP    6
#define WEIGHT_COEF_A         0
#define WEIGHT_COEF_B         8
#define WEIGHT_COEF_C         100

//
// Enforce POR
//
#define ENFORCE_POR_EN        0
#define ENFORCE_STRETCH_EN    1
#define ENFORCE_POR_DIS       2
#define ENFORCE_POR_AUTO      3
#define ENFORCE_POR_DEFAULT   ENFORCE_POR_EN

//
// C/A Parity
//
#define CA_PARITY_DISABLE     0
#define CA_PARITY_ENABLE      1
#define CA_PARITY_AUTO        2
#define CA_PARITY_DEFAULT     CA_PARITY_ENABLE

//
// Bank XOR
//
#define BANK_XOR_DISABLE      0       // Disable Bank XOR
#define BANK_XOR_ENABLE       1       // Enable Bank XOR
#define BANK_XOR_AUTO         2
#define BANK_XOR_DEFAULT      BANK_XOR_ENABLE

//
// Alternate Address Map
//
#define ALTERNATE_ADDRESS_MAP_DIS     0
#define ALTERNATE_ADDRESS_MAP_EN      1
#define ALTERNATE_ADDRESS_MAP_AUTO    2
#define ALTERNATE_ADDRESS_MAP_DEFAULT ALTERNATE_ADDRESS_MAP_EN


//
// Periodic RCOMP type
//
#define RCOMP_TYPE_PCODE              0
#define RCOMP_TYPE_HW                 1
#define RCOMP_TYPE_AUTO               2
#define RCOMP_TYPE_DEFAULT            RCOMP_TYPE_PCODE
#define RCOMP_TYPE_DEFAULT_BDX        RCOMP_TYPE_HW

//
// Per Bit Margin Data
//
#define PER_BIT_MARGIN_DISABLE       0
#define PER_BIT_MARGIN_ENABLE        1
#define PER_BIT_MARGIN_AUTO          2
#define PER_BIT_MARGIN_DEFAULT       PER_BIT_MARGIN_DISABLE    //HSX
#define PER_BIT_MARGIN_DEFAULT_BDX   PER_BIT_MARGIN_ENABLE     //BDX ML
#define PER_BIT_MARGIN_DEFAULT_DE    PER_BIT_MARGIN_ENABLE     //DE

//
// Sparing Transactions / Normal Operation Interval
//
#define SPARING_TRANS_INTERVAL    4
#define NORMAL_OPPERATION_INTERVAL 1024


//
// BCLK Freq
//
#define BCLK_133    133

//
// Lockstep x4 DIMMs Disable Option
//
#define LOCKSTEPX4_DISABLE          0
#define LOCKSTEPX4_ENABLE           1
#define LOCKSTEPX4_AUTO             2
#define LOCKSTEPX4_DEFAULT          LOCKSTEPX4_ENABLE

//
// SMBUS Clk Period
//
#define SMB_CLK_400K                0
#define SMB_CLK_1M                  1
#define SMB_CLK_DEFAULT             SMB_CLK_400K

//
// DLL Reset Test
//
#define DLL_RESET_TEST_LOOPS        0

//
// Read/WritePreamble TCLK
//
#define PREAMBLE_1TCLK              0
#define PREAMBLE_2TCLK              1
#define WRITE_PREAMBLE_DEFAULT      PREAMBLE_1TCLK
#define READ_PREAMBLE_DEFAULT       PREAMBLE_1TCLK


#if !defined(DE_SKU) || defined(MEM_SV_HOOKS)
#define WR_CRC_DISABLE           0
#define WR_CRC_ENABLE            1
#define WR_CRC_AUTO              2
#define WR_CRC_DEFAULT           WR_CRC_DISABLE
#endif

#define DDR4_PLATFORM_DISABLE   0
#define DDR4_PLATFORM_ENABLE    1
#define DDR4_PLATFORM_AUTO      2
#define DDR4_PLATFORM_DEFAULT   DDR4_PLATFORM_DISABLE 

#define CAP_ERR_FLOW_DISABLE     0
#define CAP_ERR_FLOW_ENABLE      1
#define CAP_ERR_FLOW_AUTO        2
#ifndef DE_SKU
#define CAP_ERR_FLOW_DEFAULT     CAP_ERR_FLOW_ENABLE
#else
#define CAP_ERR_FLOW_DEFAULT     CAP_ERR_FLOW_DISABLE
#endif

#define CECCWA_DISABLE           0
#define CECCWA_ENABLE            1
#define CECCWA_AUTO              2
#define CECCWA_DEFAULT           CECCWA_AUTO

//
// mem flows
//
#define MEM_FLOWS                   0xFFFFFFFF

//
// CECC WA options
//
#define CECC_WA_CHMASK_DEFAULT  0xA
#define CECC_WA_CHMASK_ALL      0xF

// 
// ADR related options
//
#define ADR_HARDWARE_TRIGGER 1
#define ADR_SOFTWARE_TRIGGER 2

#define ADR_BBU          1 
#define ADR_C2F          2
#define ADR_NVDIMM       3

//
// PPR definitions
//
#define PPR_DISABLED      0
#define PPR_TYPE_HARD     1
#define PPR_TYPE_SOFT     2

#endif   // _mem_platform_h

