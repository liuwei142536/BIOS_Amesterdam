/*++
//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//

Copyright (c)  1999 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
   IioGen3.h

Abstract:


Revision History:

--*/

#ifndef _IIOGEN3_H_
#define _IIOGEN3_H_
#ifdef __cplusplus
extern "C" {
#endif

#define GEN3_TX_PRESET_AUTO                0xFF
#define GEN3_RX_PRESET_AUTO                0xFF

#define GEN3_EQ_MODE_AUTO                      0
#define GEN3_EQ_MODE_ENABLE_P0_P1_P2_P3        1
#define GEN3_EQ_MODE_DISABLE_P0_P1_P2_P3       2
#define GEN3_EQ_MODE_ENABLE_P1_ONLY            4
#define GEN3_EQ_MODE_ENABLE_P0_P1_ONLY         5
#define GEN3_EQ_MODE_DEBUG                     8
#define GEN3_EQ_MODE_ADVANCED                 12
#define GEN3_EQ_MODE_FIXED                    13
#define GEN3_EQ_MODE_ENABLE_MMM_OFF_WEST      14
#define GEN3_EQ_MODE_ALT_SHORT_CHANNEL        15

#define GEN3_SPEC_MODE_AUTO                   0
#define GEN3_SPEC_MODE_0P7_JULY               1
#define GEN3_SPEC_MODE_0P7_SEPT               2
#define GEN3_SPEC_MODE_0P71_SEPT              3

#define GEN3_EQ_NEG_MODE_AUTO                 0
#define GEN3_EQ_NEG_MODE_FULL                 1
#define GEN3_EQ_NEG_MODE_TOTAL_BYPASS         2
#define GEN3_EQ_NEG_MODE_RESERVED             3
#define GEN3_EQ_NEG_MODE_ENABLE_P1_ONLY       4
#define GEN3_EQ_NEG_MODE_ENABLE_P0_P1_ONLY    5
#define GEN3_EQ_NEG_MODE_BYPASS_P2            6
#define GEN3_EQ_NEG_MODE_BYPASS_P3            7
#define GEN3_EQ_NEG_MODE_DEBUG                8
#define GEN3_EQ_NEG_MODE_BYPASS_P2_P3         9
#define GEN3_EQ_NEG_MODE_OVERRIDE_P3          10
#define GEN3_EQ_NEG_MODE_OVERRIDE_P2_P3       11

#define GEN3_SPEC_MODE_AUTO                   0
#define GEN3_SPEC_MODE_0P7_JULY               1
#define GEN3_SPEC_MODE_0P7_SEPT               2
#define GEN3_SPEC_MODE_0P71_SEPT              3

#define GEN3_EQ_MODE_HW_ADAPTIVE           0
#define GEN3_EQ_MODE_LAGUNA                1
#define GEN3_EQ_MODE_MANUAL                2
#define GEN3_EQ_MODE_LOOKUP_TABLE          3
//#define GEN3_EQ_MODE_MAXBOOST_DETECT       4      //this option is retired, replaced by mid@max boost SW override
#define GEN3_EQ_MODE_MAXBOOST_LOOKUP_TABLE 5
#define GEN3_EQ_MODE_TEST                  6
#define GEN3_EQ_MODE_NTB                   9
#define GEN3_EQ_MODE_MAXBOOST_SW           10
#define GEN3_EQ_MODE_MAXBOOST_HW           11
#define GEN3_EQ_MODE_MAXBOOST_HWADAPT      12
#define GEN3_EQ_MODE_MAXBOOST_BYPASS       13

#define GEN3_LINKUP_FLOW_DISABLED          0
#define GEN3_LINKUP_FLOW_G1_G3             1
#define GEN3_LINKUP_FLOW_G1_G2_G3          2

#define GEN3_RETRAIN_TYPE_SBR              1
#define GEN3_RETRAIN_TYPE_LINK_DISABLE     2
#define GEN3_RETRAIN_TYPE_PHY_RESET        3
#define GEN3_RETRAIN_TYPE_SKIP             4

#define GEN3_LINK_SPEED_GEN1       1
#define GEN3_LINK_SPEED_GEN2       2
#define GEN3_LINK_SPEED_GEN3       3

#define GEN3_REUT_LTSSM_L0         11

#define GEN3_LINK_POLL_INFINITE                 0

#define IIO_REUT_LTSSM_DISABLED    0
#define IIO_REUT_LTSSM_L0         11

#define GEN3_CAPID_PCIE 0x10

#define GEN3_RESET_PHYRESET_ENTER  0
#define GEN3_RESET_PHYRESET_EXIT   1
#define GEN3_RESET_PHYRESET_TOGGLE 2

#define MAX_LINK_DEGRADE_RETRIES            3

static UINT8 MaxGen3EqRegs[] = {0, 8, 4, 16, 4, 8, 4, 16, 4, 8, 4};
static UINT32 BifurcationMap[5][4] = {{ 4, 4, 4, 4},
                                     { 8, 0, 4, 4},
                                     { 4, 4, 8, 0},
                                     { 8, 0, 8, 0},
                                     {16, 0, 0, 0} };


struct TxEqCoefficient
{
  UINT8 Precursor;
  UINT8 Cursor;
  UINT8 Postcursor;
};

typedef UINT32 GEN3_STATUS;

typedef struct  {
  UINT16  Vid;
  UINT16  Did;
  UINT8   Rsvd1;
  UINT8   Platform;
  UINT8   Socket;
  UINT16  Port;
  UINT8   UpstreamTxEqValid;
  struct  TxEqCoefficient UpstreamTxEq;
  UINT8   DownstreamTxEqValid;
  struct  TxEqCoefficient DownstreamTxEq;
} PORTPARAMS_STRUCT;

typedef union {
  struct {
  UINT16 Vid;
  UINT16 Did;
  UINT8 FsLfValid;
  UINT16 Fs;
  UINT16 Lf;
  UINT8 UpstreamTxEqValid;
  struct TxEqCoefficient UpstreamTxEq;
  UINT8 DownstreamTxEqValid;
  struct TxEqCoefficient DownstreamTxEq;
  } LinkParams;
  PORTPARAMS_STRUCT PortParams;
} GEN3OVERRIDE_STRUCT;

VOID
ProgramGen3Equalization (
    IIO_GLOBALS       *IioGlobalData,
    UINT8 Socket
    );

VOID
CheckGen3Override (
    IIO_GLOBALS       *IioGlobalData,
    UINT8 Socket
);

GEN3_STATUS
Gen3Override(
    IIO_GLOBALS                      *IioGlobalData,
    UINT8                             Socket,
    UINT8                             Port,
    UINT8                             Phase2EqMode,
    UINT8                             Phase3EqMode
    );

GEN3_STATUS
Gen3PollLinkStatus (
  IIO_GLOBALS  *IioGlobalData,
  UINT8 Socket,
  UINT8 Port,
  UINT32 LtssmState,
  UINT16 LinkSpeed,
  UINT16 LinkWidth,
  UINT32 Timeout,
  UINT32 *OutLtssmState,
  UINT16 *OutLinkSpeed,
  UINT16 *OutLinkWidth
  );

GEN3_STATUS
Gen3PhyReset (
  IIO_GLOBALS                      *IioGlobalData,
  UINT8                             Socket,
  UINT8                             Port,
  UINT8                             ResetType 
  );


VOID
FixLinkDegrades (
  IIO_GLOBALS                      *IioGlobalData,
  UINT8                             Socket,
  UINT8                             Port
  );

VOID
CheckGen3PreLinkOverride (
    IIO_GLOBALS       *IioGlobalData,
    UINT8 Socket
);

VOID
MiscWA_PreLinkData(
    IIO_GLOBALS                       *IioGlobalData,
    UINT8                             Socket,
    UINT8                             Port
);

VOID
Gen3WaitTillLinkRetrains(
    IIO_GLOBALS *IioGlobalData,
    UINT8 BusBase,
    UINT8 Device,
    UINT8 Function
);

VOID
AlignGen3PreLinkConfigurationSettings (
	IIO_GLOBALS       *IioGlobalData,
	UINT8             Socket
);
/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif // _IIO_GEN3_H_

