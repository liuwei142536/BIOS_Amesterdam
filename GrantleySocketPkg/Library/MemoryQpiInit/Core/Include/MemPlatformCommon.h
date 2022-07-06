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
 * Copyright 2006 - 2019 Intel Corporation All Rights Reserved.
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

#ifndef  _mem_platformcommon_h
#define  _mem_platformcommon_h


#include <Token.h> // AptioV server override



//
// Major Checkpoint progress indicators written to debug port
//
#define  STS_DIMM_DETECT            0xB0
  #define SUB_DIMM_DETECT               0x00
  #define SUB_DISPATCH_SLAVES           0x01
  #define SUB_INIT_SMB                  0x02
  #define SUB_GET_SLAVE_DATA            0x03
#define  STS_CLOCK_INIT             0xB1
  #define SUB_SEND_FREQ                 0x01
  #define SUB_SEND_STATUS               0x02
  #define SUB_SET_FREQ                  0x03
  #define SUB_SET_CLK_VDD               0x04
  #define SUB_XMP_INIT                  0x05
#define  STS_SPD_DATA               0xB2
  #define SUB_SEND_DATA                 0x01
#define  STS_GLOBAL_EARLY           0xB3
  #define SUB_PROG_TIMINGS              0x01
#define  STS_RANK_DETECT            0xB4
  #define SUB_SEND_DATA_SPD             0x01
    #define DATA_TYPE_VAR                 0x01
    #define DATA_TYPE_NVRAM_COMMON        0x02
    #define DATA_TYPE_NVRAM_DATA          0x03
#define  STS_CHANNEL_EARLY          0xB5
#define  STS_DDRIO_INIT             0xB6
  #define EARLY_INIT                      0x01
  #define LATE_INIT                       0x02
#define  STS_CHANNEL_TRAINING       0xB7
  // These minor checkpoints are defined in MemHost.h
  // #define RECEIVE_ENABLE_BASIC  0
  // #define WRITE_LEVELING_BASIC  1
  // #define WR_FLY_BY             2
  #define SUB_WR_FLY_BY_TRAINING        0x01
  #define SUB_WR_FLY_BY_EVALUATE        0x02
  // #define RX_DQ_DQS_BASIC       3
  // #define TX_DQ_BASIC           4
  // #define LATE_CMD_CLK          5
  // #define TRAIN_RD_DQS          6
  // #define TRAIN_WR_DQS          7
  // #define TRAIN_RD_VREF         8
  // #define TRAIN_WR_VREF         9
  // #define CROSSOVER_CALIBRATION 0x0A
  // #define SENSE_AMP             0x0B
  // #define EARLY_CMD_CLK         0x0C
  #define SUB_ENABLE_PARITY             0x03
  // #define PER_BIT_DESKEW_RX     0x0D
  // #define PER_BIT_DESKEW_TX     0x0E
  // #define RX_VREF_TRAINING      0x0F
  #define  STS_RMT                  0x10
    #define SUB_LATE_CONFIG             0x01
    #define SUB_INIT_THROTTLING         0x02
  #define  STS_MINOR_END_TRAINING   0x11
  #define  STS_MINOR_NORMAL_MODE    0x12
    #define SUB_DATA_SYNC               0x01
#define  STS_INIT_THROTTLING        0xB8
#define  STS_MEMBIST                0xB9
  #define SUB_MT_STEP1               0x01
  #define SUB_MT_STEP2               0x02
  #define SUB_MT_STEP3               0x03
  #define SUB_MT_STEP4               0x04
  #define SUB_MT_STEP5               0x05
  #define SUB_MT_STEP6               0x06
  #define SUB_MT_STEP7               0x07
  #define SUB_MT_STEP8               0x08
  #define SUB_MT_STEP9               0x09
  #define SUB_MT_STEP10              0x0A
#define  STS_MEMINIT                0xBA
#define  STS_DDR_MEMMAP             0xBB
  #define SUB_SAD_INTERLEAVE            0x01
  #define SUB_TAD_INTERLEAVE            0x02
  #define SUB_SAD_NONINTER              0x03
  #define SUB_TAD_NONINTER              0x04
  #define SUB_RANK_INTER                0x05
  #define SUB_WRITE_SAD                 0x06
  #define SUB_WRITE_TAD                 0x07
  #define SUB_WRITE_RIR                 0x08
  #define SUB_WRITE_SPARE_RANK 10
#define  STS_RAS_CONFIG             0xBC
  #define SUB_MIRROR_MODE               0x01
  #define SUB_SPARE_MODE                0x02
  #define SUB_DEVICE_TAG                0x03
  #define SUB_ERR_THRESH                0x04
  #define SUB_LOCKSTEP_MODE             0x05
#define STS_GET_MARGINS             0xBD
#ifdef SSA_FLAG
#define  SSA_API_INIT               0xBE
  #define  SSA_API_INIT_START           0x01
  #define  SSA_API_INIT_EV_LOADER       0x02 //SSA API has been initialized; EV loader to be called
#endif //SSA_FLAG
#define  STS_MRC_DONE               0xBF
// The following do not appear as check points but are used to set currentMemPhase for performance tracking.
#define  STS_JEDEC_INIT             0xC0

//
// Major Fatal error codes written to port 80h
//
#define  ERR_NO_MEMORY              0xE8
 #define  ERR_NO_MEMORY_MINOR_NO_MEMORY             0x01
 #define  ERR_NO_MEMORY_MINOR_ALL_CH_DISABLED       0x02
 #define  ERR_NO_MEMORY_MINOR_ALL_CH_DISABLED_MIXED 0x03
#define  ERR_LT_LOCK                0xE9
#define  ERR_DDR_INIT               0xEA
 #define  ERR_RD_DQ_DQS             0x01
 #define  ERR_RC_EN                 0x02
 #define  ERR_WR_LEVEL              0x03
 #define  ERR_WR_DQ_DQS             0x04
#define  ERR_MEM_TEST               0xEB
 #define  ERR_MEM_TEST_MINOR_SOFTWARE               0x01
 #define  ERR_MEM_TEST_MINOR_HARDWARE               0x02
 #define  ERR_MEM_TEST_MINOR_LOCKSTEP_MODE          0x03
#define  ERR_VENDOR_SPECIFIC        0xEC
#define  ERR_DIMM_COMPAT            0xED
 #define  ERR_MIXED_MEM_TYPE        0x01
 #define  ERR_INVALID_POP           0x02
 #define  ERR_INVALID_POP_MINOR_QR_AND_3RD_SLOT        0x03
 #define  ERR_INVALID_POP_MINOR_UDIMM_AND_3RD_SLOT     0x04
 #define  ERR_INVALID_POP_MINOR_UNSUPPORTED_VOLTAGE    0x05
 #define  ERR_DDR3_DDR4_MIXED                          0x06
 #define  ERR_MIXED_SPD_TYPE                           0x07
 #define  ERR_MISMATCH_DIMM_TYPE                       0x08
 #define  ERR_DIMM_COMPAT_16Gb_X4_NOT_SUPPORTED        0x09
#define  ERR_MRC_COMPATIBILITY      0XEE
#define    ERR_MRC_DIR_NONECC       0X01
#define  ERR_MRC_STRUCT             0xEF
 #define   ERR_INVALID_BOOT_MODE                       0x01
 #define   ERR_INVALID_SUB_BOOT_MODE                   0x02
#define ERR_SET_VDD                 0xF0
 #define ERR_UNKNOWN_VR_MODE                           0x01
#define ERR_IOT_MEM_BUFFER          0xF1
#define ERR_RC_INTERNAL             0xF2
#define ERR_INVALID_REG_ACCESS      0xF3
# define ERR_INVALID_WRITE_REG_BDF                      0x01
# define ERR_INVALID_WRITE_REG_OFFSET                   0x02
# define ERR_INVALID_READ_REG_BDF                       0x03
# define ERR_INVALID_READ_REG_OFFSET                    0x04
#define ERR_SET_MC_FREQ                    0xF3
 #define ERR_UNSUPPORTED_MC_FREQ            0x01
#define ERR_READ_MC_FREQ                   0xF4
 #define ERR_NOT_ABLE_READ_MC_FREQ          0x01


//
// Major Warning codes
//
#define  WARN_RDIMM_ON_UDIMM        0x01
#define  WARN_UDIMM_ON_RDIMM        0x02
#define  WARN_SODIMM_ON_RDIMM       0x03
#define  WARN_4Gb_FUSE              0x04
#define  WARN_8Gb_FUSE              0x05
#define  WARN_IMC_DISABLED          0x06
#define  WARN_DIMM_COMPAT           0x07
 #define  WARN_DIMM_COMPAT_MINOR_X16_C0MBO      0x01
 #define  WARN_DIMM_COMPAT_MINOR_MAX_RANKS      0x02
 #define  WARN_DIMM_COMPAT_MINOR_QR             0x03
 #define  WARN_DIMM_COMPAT_MINOR_NOT_SUPPORTED  0x04
 #define  WARN_RANK_NUM                         0x05
 #define  WARN_TOO_SLOW                         0x06
 #define  WARN_DIMM_COMPAT_MINOR_ROW_ADDR_ORDER 0x07
 #define  WARN_CHANNEL_CONFIG_NOT_SUPPORTED     0x08
 #define  WARN_CHANNEL_MIX_ECC_NONECC           0x09
 #define  WARN_DIMM_VOLTAGE_NOT_SUPPORTED       0x0A
 #define  WARN_DIMM_COMPAT_TRP_NOT_SUPPORTED    0x0B
 #define  WARN_DIMM_NONECC                      0x0C
 #define  WARN_DIMM_COMPAT_3DS_RDIMM_NOT_SUPPORTED    0x0D //3DS
#define  WARN_LOCKSTEP_DISABLE      0x09
 #define  WARN_LOCKSTEP_DISABLE_MINOR_RAS_MODE         0x01
 #define  WARN_LOCKSTEP_DISABLE_MINOR_MISMATCHED       0x02
 #define  WARN_LOCKSTEP_DISABLE_MINOR_MEMTEST_FAILED   0x03
#define  WARN_USER_DIMM_DISABLE     0x0a
 #define  WARN_USER_DIMM_DISABLE_QUAD_AND_3DPC         0x01
 #define  WARN_USER_DIMM_DISABLE_MEMTEST               0x02
#define  WARN_MEMTEST_DIMM_DISABLE  0x0b
#define  WARN_MIRROR_DISABLE        0x0c
#define  WARN_PMIRROR_DISABLE       0x0d
 #define  WARN_MIRROR_DISABLE_MINOR_RAS_DISABLED       0x01
 #define  WARN_MIRROR_DISABLE_MINOR_MISMATCH           0x02
 #define  WARN_MIRROR_DISABLE_MINOR_MEMTEST            0x03
 #define  WARN_PMIRROR_DISABLE_MINOR_RAS_DISABLED      0x03
#define  WARN_INTERLEAVE_FAILURE    0x0e
 #define  WARN_SAD_RULES_EXCEEDED                      0x01
 #define  WARN_TAD_RULES_EXCEEDED                      0x02
 #define  WARN_RIR_RULES_EXCEEDED                      0x03
 #define  WARN_TAD_OFFSET_NEGATIVE                     0x04
 #define  WARN_TAD_LIMIT_ERROR                         0x05
 #define  WARN_INTERLEAVE_3WAY                         0x06
 #define  WARN_A7_MODE_AND_3WAY_CH_INTRLV              0x07
 #define  WARN_INTERLEAVE_EXCEEDED                     0x08
 #define  WARN_DIMM_CAPACITY_MISMATCH                  0x09
 #define  WARN_DIMM_POPULATION_MISMATCH                0x0A
 #define  WARN_NM_MAX_SIZE_EXCEEDED                    0x0B
 #define  WARN_NM_SIZE_BELOW_MIN_LIMIT                 0x0C
 #define  WARN_NM_SIZE_NOT_POWER_OF_TWO                0x0D
 #define  WARN_MAX_INTERLEAVE_SETS_EXCEEDED            0x0E
 #define  WARN_NGN_DIMM_COMM_FAILED                    0x0F
#define  WARN_SPARE_DISABLE         0x10
#define  WARN_PTRLSCRB_DISABLE      0x11
#define  WARN_UNUSED_MEMORY         0x12
 #define  WARN_UNUSED_MEMORY_MINOR_MIRROR              0x01
 #define  WARN_UNUSED_MEMORY_MINOR_LOCKSTEP            0x02
#define  WARN_RD_DQ_DQS             0x13
#define  WARN_RD_RCVEN              0x14
 #define  WARN_ROUNDTRIP_EXCEEDED                      0x01
#define  WARN_WR_LEVEL              0x15
 #define  WARN_WR_FLYBY_CORR                           0x00
 #define  WARN_WR_FLYBY_UNCORR                         0x01
 #define  WARN_WR_FLYBY_DELAY                          0x02
#define  WARN_WR_DQ_DQS             0x16
#define  WARN_DIMM_POP_RUL          0x17
 #define  WARN_DIMM_POP_RUL_MINOR_OUT_OF_ORDER         0x01
 #define  WARN_DIMM_POP_RUL_MINOR_INDEPENDENT_MODE     0x02
 #define  WARM_DIMM_POP_RUL_2_AEP_FOUND_ON_SAME_CH     0x03
 #define  WARM_DIMM_POP_RUL_MINOR_MIXED_RANKS_FOUND    0x04
#define  WARN_CLTT_DISABLE          0x18
 #define  WARN_CLTT_MINOR_NO_TEMP_SENSOR               0x01
 #define  WARN_CLTT_MINOR_CIRCUIT_TST_FAILED           0x02
#define  WARN_THROT_INSUFFICIENT    0x19
#define  WARN_CLTT_DIMM_UNKNOWN     0x1a
#define  WARN_DQS_TEST              0x1b
#define  WARN_MEM_TEST              0x1c
#define  WARN_CLOSED_PAGE_OVERRIDE  0x1d
#define  WARN_DIMM_VREF_NOT_PRESENT 0x1e
#define  WARN_EARLY_RID             0x1f
 #define  WARN_EARLY_RID_UNCORR                        0x01
 #define  WARN_EARLY_RID_CYCLE_FAIL                    0x02
#define  WARN_LV_STD_DIMM_MIX       0x20
#define  WARN_LV_2QR_DIMM           0x21
#define  WARN_LV_3DPC               0x22
#define  WARN_CMD_ADDR_PARITY_ERR   0x23
#define  WARN_DQ_SWIZZLE_DISC       0x24
 #define  WARN_DQ_SWIZZLE_DISC_UNCORR                  0x01
#define  WARN_COD_HA_NOT_ACTIVE     0x25
#define  WARN_CMD_CLK_TRAINING      0x26
#define  WARN_INVALID_BUS           0x27
#define  WARN_INVALID_FNV_OPCODE    0x28
#define  WARN_MEMORY_TRAINING       0x29
 #define  WARN_CTL_CLK_LOOPBACK_TRAINING               0x02
 #define  WARN_ODT_TIMING_OVERFLOW                     0x03
#define  WARN_NO_DDR_MEMORY         0x2a
 #define  WARN_NO_MEMORY_MINOR_NO_MEMORY              0x01
 #define  WARN_NO_MEMORY_MINOR_ALL_CH_DISABLED        0x02
 #define  WARN_NO_MEMORY_MINOR_ALL_CH_DISABLED_MIXED  0x03
#define  WARN_ROUNDTRIP_ERROR       0x2b
 #define  WARN_RCVNTAP_CMDDELAY_EXCEEDED              0x01
#define  WARN_MEMORY_MODEL_ERROR    0x2c
 #define  WARN_SNC24_MODEL_ERROR                      0x01
 #define  WARN_QUAD_HEMI_MODEL_ERROR                  0x02
 #define  WARN_SNC24_DIMM_POPULATION_MISMATCH         0x03
 #define  WARN_SNC24_INCOMPATIBLE_DDR_CAPACITY        0x04
 #define  WARN_SNC24_INCOMPATIBLE_MCDRAM_CAPACITY     0x05
 #define  WARN_MCDRAM_CONFIG_NOT_SUPPORTED            0x06
#define  WARN_OVERRIDE_MEMORY_MODE  0x2d
 #define  WARN_OVERRIDE_TO_FLAT_NO_MCDRAM_MEMORY      0x01
 #define  WARN_OVERRIDE_TO_FLAT_NO_DDR_MEMORY         0x02
 #define  WARN_OVERRIDE_TO_HYBRID_CACHESIZE_1_2       0x03
#define  WARN_MEM_INIT              0x2e
#define  WARN_SENS_AMP_TRAINING     0x2f
  #define  WARN_SENS_AMP_CH_FAILIURE 0x01
#define  WARN_SWIZZLE_DISCOVERY_TRAINING  0x30
  #define  WARN_SWIZZLE_PATTERN_MISMATCH  0x01
#define  WARN_WRCRC_DISABLE               0x31
  #define  WARN_TRAIL_ODT_LIMIT_REACHED   0x01

#define  WARN_FPT_CORRECTABLE_ERROR    0x30
#define  WARN_FPT_UNCORRECTABLE_ERROR  0x31
  #define  WARN_FPT_MINOR_RD_DQ_DQS             0x13
  #define  WARN_FPT_MINOR_RD_RCVEN              0x14
  #define  WARN_FPT_MINOR_WR_LEVEL              0x15
  #define  WARN_FPT_MINOR_WR_FLYBY              0x00
  #define  WARN_FPT_MINOR_WR_DQ_DQS             0x16
  #define  WARN_FPT_MINOR_DQS_TEST              0x1b
  #define  WARN_FPT_MINOR_MEM_TEST              0x1c
  #define  WARN_FPT_MINOR_LRDIMM_TRAINING       0x24
  #define  WARN_FPT_MINOR_VREF_TRAINING         0x25

#define  WARN_CH_DISABLED              0x32
  #define  WARN_TWR_LIMIT_REACHED               0x01
  #define  WARN_TWR_LIMIT_ON_LOCKSTEP_CH        0x02

#define  WARN_MEM_LIMIT             0x33
#define WARN_RT_DIFF_EXCEED         0x34

#define  WARN_PPR_FAILED            0x35

#ifdef XMP_SUPPORT
#define  WARN_MEM_CONFIG_CHANGED    0x40
 #define  WARN_MEM_OVERRIDE_DISABLED                   0x01
#endif // XMP_SUPPORT

#define  WARN_MCA_UNCORRECTABLE_ERROR   0x50

#define  WARN_DM_TEST_ERROR_CODE        0x60
  #define WARN_DM_TEST_PARSE_ERROR_MINOR_CODE         0x01
  #define WARN_DM_TEST_CONFIGURATION_ERROR_MINOR_CODE 0x02
  #define WARN_DM_TEST_EXECUTION_ERROR_MINOR_CODE     0x03

#endif // _mem_platformcommon_h
