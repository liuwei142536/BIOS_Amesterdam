/**
 This file contains an 'Intel Pre-EFI Module' and is licensed
 for Intel CPUs and Chipsets under the terms of your license
 agreement with Intel or your vendor.  This file may be
 modified by the user, subject to additional terms of the
 license agreement
**/
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

#include "SysFunc.h"
#include "Cpgc.h"

//
// Place any project specific defines or structures here.
//
#define MAX_STROBE_IMODE  (MAX_STROBE / 2)

//------------------------------------------------------------------------

#ifndef MEM_WR_LEVELINGHOOKS_H
#define MEM_WR_LEVELINGHOOKS_H

#define CHIP_TX_PI_SAMPLE_YAMDELAY 30*16

typedef struct
{
  DATACONTROL0_0_MCIO_DDRIO_STRUCT      dataControl0;
  DATACONTROL2_0_MCIO_DDRIO_BDX_STRUCT  dataControl2;
}CHIP_DDRIO_STRUCT, *PCHIP_DDRIO_STRUCT;

#define CHIP_CPG_MISCODTCTL_MCDDC_CTL_STRUCT CPGC_MISCODTCTL_MCDDC_CTL_HSX_BDX_STRUCT

#define FLY_BY_CACHELINES   4
#define WR_FLY_BY_PATTERN   0x3CC300FF
#define TX_DQS_DQ_OFFSET       32
#define TX_DQS_DQ_OFFSET_2TO2  -32

void UncoreEnableMeshMode(PSYSHOST host);

typedef struct _CHIP_WL_CLEANUP_STRUCT
{
  TCOTHP_MCDDC_CTL_STRUCT tCOTHP;
}CHIP_WL_CLEANUP_STRUCT, *PCHIP_WL_CLEANUP_STRUCT;

#define CHIP_OFFSET_ARRAY_SIZE 6
#define CHIP_OFFSET_ARRAY_VALUES {-1, 0, 1, 2, 3, 4}

#endif //MEM_WR_LEVELINGHOOKS_H

//------------------------------------------------------------------------

#ifndef MEM_CMD_CLKHOOKS_H
#define MEM_CMD_CLKHOOKS_H

#define CHIP_GSM_CSN_SIGNALSTOSWEEP {PAR, CAS_N, A13, RAS_N, WE_N, A10, BA1, A0, BA0, A1, A3, A2, A4, A5, A6, A7, A8, A9, A12, A11, BG1, ACT_N, BG0, C0, C1, C2};
#define NUM_SIGNALS_TO_SWEEP 26

#define CHIP_CMD_CLK_TEST_START  108
#define CHIP_CMD_CLK_TEST_RANGE  216

#define CHIP_CMD_GRP_STOP CmdGrp2

#define TX_RON_14 14   
#define TX_RON_18 18
#define TX_RON_24 24

typedef struct _CHIP_ELY_CMD_CK_STRUCT
{
  INT16                               le[MAX_CH];
  INT16                               re[MAX_CH];
  TCDBP_MCDDC_CTL_HSX_BDX_STRUCT      tcdbp;
}CHIP_ERLY_CMD_CK_STRUCT, *PCHIP_ERLY_CMD_CK_STRUCT;

typedef struct _CHIP_GET_CMD_MGN_SWP_STRUCT
{
  UINT16              eyeSize[MAX_CH];
  UINT8               chEyemask;
  UINT8               chDonemask;
}CHIP_GET_CMD_MGN_SWP_STRUCT, *PCHIP_GET_CMD_MGN_SWP_STRUCT;

typedef struct _CHIP_CLEAR_PARITY_RESULTS_STRUCT
{
  TCRAP_MCDDC_CTL_HSX_BDX_STRUCT  tCrap;
}CHIP_CLEAR_PARITY_RESULTS_STRUCT, *PCHIP_CLEAR_PARITY_RESULTS_STRUCT;

typedef struct _CHIP_PARITY_CHECKING_STRUCT
{
  UINT32                                          tCrapOrg;
  TCRAP_MCDDC_CTL_HSX_BDX_STRUCT                  tCrap;
  DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_BDX_STRUCT   ddrCRCtlControls;
  DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXT_BDX_STRUCT  ddrCRCmdControls1;
  DDR4_CA_CTL_MCDDC_DP_STRUCT                     ddr4cactl;
  VMSE_ERROR_MCDDC_DP_HSX_BDX_STRUCT              vmseError;
}CHIP_PARITY_CHECKING_STRUCT, *PCHIP_PARITY_CHECKING_STRUCT;

typedef struct _CHIP_SET_NORMAL_CMD_TIMING_STRUCT
{
  TCRAP_MCDDC_CTL_HSX_BDX_STRUCT  tCrap;
}CHIP_SET_NORMAL_CMD_TIMING_STRUCT, *PCHIP_SET_NORMAL_CMD_TIMING_STRUCT;

#define CHIP_RNDTRP_DIFF_MPR_OFF_3N_TO_1N 4
#define CHIP_RNDTRP_DIFF_MPR_OFF_3N_TO_2N 2
#define CHIP_RNDTRP_DIFF_MPR_OFF_2N_TO_1N 2
#define CHIP_RNDTRP_DIFF_MPR_OFF_2N_TO_2N 0
#define CHIP_RNDTRP_DIFF_MPR_OFF_1N_TO_1N 0

#define CPGC_MISCODTCTL_MCDDC_CHIP_STRUCT CPGC_MISCODTCTL_MCDDC_CTL_HSX_BDX_STRUCT
#define INITIAL_CHIP_REC_EN_OFFSET 32
#define TEARDOWN_CHIP_REC_EN_OFFSET -INITIAL_CHIP_REC_EN_OFFSET

typedef struct
{
  CPGC_MISCODTCTL_MCDDC_CHIP_STRUCT     cpgcMiscODTCtl;
  DATACONTROL0_0_MCIO_DDRIO_STRUCT      dataControl0;
}EXECUTE_CTL_CLK_TEST_CHIP_STRUCT, *PEXECUTE_CTL_CLK_TEST_CHIP_STRUCT;

#define CHIP_GET_CTL_CLK_RES_MAX_STROBE MAX_STROBE / 2

#endif //MEM_CMD_CLKHOOKS_H

//-----------------------------------------------------------------------
