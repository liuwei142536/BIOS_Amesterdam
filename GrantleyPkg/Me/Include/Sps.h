/*++
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2012-2021 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  Sps.h

Abstract:

 SPS definitions common for various SPS modules.

--*/
#ifndef _SPS_H_
#define _SPS_H_

#include "MkhiMsgs.h"

//
// TODO: Definitions below are obsolete and will be removed in future.
//
#define ME_FUNCTIONAL               0x1
#define ME_NONFUNCTIONAL            0x2
#define ME_NOT_SPS                  0xF
#define ME_DEV_NOT_PRESENT          0x4
#define LPC_DEV_NOT_PRESENT         0x8
#define ME_AVAILABLE_INVALID_STATUS 0x10

#define SPS_MAX_NUMBER_OF_RESEND_MESSAGE 3


/*****************************************************************************
 * SPS  HECI registers addresses
 */
#define SPS_R_MEFS1     HECI_R_MEFS1   // HFS in HECI-1 config space
#define SPS_R_MEFS2     HECI_R_MEFS2   // H_GS in HECI-1 config space
#define SPS_R_NMFS      HECI_R_HFS     // HFS in HECI-2 config space

///
/// Defines for GroupID
///
#define MKHI_MCA_GROUP_ID         0x0A

///
/// Defines for MCA_GROUP Command
///
#define MCA_CORE_BIOS_DONE_CMD    0x05

/******************************************************************************
 * SPS interface on HECI status registers
 */
//
// SPS ME Firmware Status #1 Register
// This register is located in HECI-1 PCI config space at offset 40h.
// Most fields map to HECI_FWS_REGISTER in client firmware.
//
typedef union
{
  UINT32   DWord;
  struct
  {
    UINT32 CurrentState      : 4,  // 0:3   Current ME firmware state
           ManufacturingMode : 1,  // 4     Platform is in Manufacturing Mode
           FptBad            : 1,  // 5     Flash Partition Table or factory defaults bad
           OperatingState    : 3,  // 6:8   ME operatiing state
           InitComplete      : 1,  // 9     Set when firmware finished initialization
           FtBupLdFlr        : 1,  // 10    ME is not able to load BRINGUP from recovery code
           UpdateInprogress  : 1,  // 11
           ErrorCode         : 4,  // 12:15 ME is not running because of fatal error
           OperatingMode     : 4,  // 16:19 This field describes the current ME operating mode
           Reserved          : 5,  // 20:24
           MsgAckData        : 3,  // 25:27 MSG ACK Data specific for acknowledged BIOS message
           MsgAck            : 4;  // 28:31 Acknowledge for register based BIOS message
  } Bits;
} SPS_MEFS1;

//
// SPS_MEFS1::CurrentState Values
// This field describes the current operation state of the firmware.
// The values are shared with client ME firmware.
//
#define ME_CURSTATE_RESET        0  // ME is in reset, will exit this state within 1 milisecond
#define ME_CURSTATE_INIT         1  // ME is initializing, will exit this state within 2 seconds
#define ME_CURSTATE_RECOVERY     2  // ME is in recovery mode, check other bits to determine cause
#define ME_CURSTATE_DISABLED     4  // ME functionality has been disabled - not used with SPS
#define ME_CURSTATE_NORMAL       5  // ME is in normal operational state
#define ME_CURSTATE_DISABLE_WAIT 6  // Not used with SPS
#define ME_CURSTATE_TRANSITION   7  // ME is transitioning to a new Operating State

//
// SPS_MEFS1::OperatingState Values
// This field describes the current operating state of ME.
//
#define ME_OPSTATE_PREBOOT       0  // ME in pre-boot
#define ME_OPSTATE_M0_UMA        1  // ME runs using UMA - not used with SPS
#define ME_OPSTATE_M3            4  // ME runs without UMA, host in Sx
#define ME_OPSTATE_M0            5  // ME runs without UMA, host in S0 -normal state for SPS
#define ME_OPSTATE_BRINGUP       6  // ME in bringup
#define ME_OPSTATE_M0_ERROR      7  // M0 without UMA but with error

//
// SPS_MEFS1::ErrorCode Values
// If set to non zero value the ME firmware has encountered a fatal error and stopped normal operation.
//
#define ME_ERRCODE_NOERROR       0
#define ME_ERRCODE_UNKNOWN       1
#define ME_ERRCODE_DISABLED      2  // Not used with SPS
#define ME_ERRCODE_IMAGE_FAIL    3  // No valid ME firmware image found in ME region

//
// SPS_MEFS1::OperatingMode Values
// This field describes the current operating mode of ME.
//
#define ME_OPMODE_NORMAL         0  // Client firmware is running in ME
#define ME_OPMODE_IGNITION       1  // Ignition firmware is running in ME
#define ME_OPMODE_ALT_DISABLED   2  // ME is disabled
#define ME_OPMODE_TEMP_DISABLE   3  //
#define ME_OPMODE_SECOVR_JMPR    4  // Security Override activated with jumper
#define ME_OPMODE_SECOVR_MSG     5  // Security Override activated with HMRFPO_ENABLE request
#define ME_OPMODE_SPS            15 // SPS firmware is running in ME

//
// SPS ME Firmware Status #2 Register
// This register is located in HECI-1 PCI config space at offset 48h.
//
typedef union
{
  UINT32   DWord;
  struct
  {
    UINT32 BistInProgress         : 1,  // 0     Built-in self test is running
           RecoveryCause          : 3,  // 1:3   If MEFS1.CurrentState says recovery here is reason
           ProcessorSwapped       : 1,  // 4     Set to 1 if ME detected processor swap
           Reserved0              : 1,  // 5
           MfsFailure             : 1,  // 6     ME detected error in its configuration
           WarmResetRequest       : 1,  // 7     ME informs that host warm reset is necessary
           InitDataValid          : 1,  // 8     Early ME status bits are valid
           Reserved1              : 3,  // 9:11
           MeTargetImageBootFault : 1,  // 12    ME could not boot active image, backup is running
           FirmwareHeartbeat      : 3,  // 13:15 ME heartbeat, changes each second
           ExtendedStatusData     :12,  // 16:27
           ProgressCode           : 3,  // 28:30 ME firmware progress status
           EopStatus              : 1;  // 31    ME notion of EOP status
  } Bits;
} SPS_MEFS2;

//
// SPS_MEFS2::RecoveryCause Values
// If SPS_MEFS1::CurrentState bits indicate that ME firmware is running in recovery mode
// these values provide the cause of this mode.
//
#define SPS_RCAUSE_RCVJMPR  0  // ME recovery jumper asserted
#define SPS_RCAUSE_MFGJMPR  1  // Security strap override jumper asserted (aka mfg jumper)
#define SPS_RCAUSE_IPMICMD  2  // Recovery forced with IPMI command
#define SPS_RCAUSE_FLASHCFG 3  // Invalid flash configuration (see ME-BIOS spec. for details)
#define SPS_RCAUSE_MEERROR  4  // ME internal error, check SEL

//
// SPS NM Firmware Status Register.
// This register is located in HECI-2 PCI config space at offset 40h.
//
typedef union
{
  UINT32   DWord;
  struct
  {
    UINT32 PerfOptBoot          : 1,  // 0     Power vs performance optimized boot mode
           Cores2Disable        : 7,  // 1:7   Number of processor cores to disable in each processor
           PwrLimiting          : 1,  // 8     Set to 1 if ME is activly limiting power consumption
           SmartClstUVolt       : 1,  // 9     Set to 1 if under-voltage was seen
           SmartClstOCurr       : 1,  // 10    Set to 1 if over-current was seen
           SmartClstOTemp       : 1,  // 11    Set to 1 if over-temperature was seen
           DirectPtuExecution   : 1,  // 12    Set to 1 if ME requests PTU execution
           Reserved1            : 18, // 13:30
           NmEnabled            : 1;  // 31    Set to 1 if Node Manager is suported in ME
  } Bits;
} SPS_NMFS;

/*****************************************************************************
 * SPS interface using HECI messaging
 */
#define SPS_CLIENTID_BIOS 0x00

#pragma pack(1)
typedef union
{
  UINT8   Data;
  struct
  {
    UINT8 Command   :7,
          IsResponse:1;
  } Fields;
} HBM_COMMAND;

typedef union
{
  UINT32   Data;
  struct
  {
    UINT32 GroupId   : 8,
           Command   : 7,
           IsResponse: 1,
           Reserved  : 8,
           Result    : 8;
  } Fields;
} MKHI_MSG_HEADER;
#pragma pack()


/*****************************************************************************
 * ME Kernel Host Interface
 *****************************************************************************/
#define SPS_CLIENTID_ME_MKHI 0x07
//
// MKHI protocol supports several groups and several commands in each group
//
#define MKHI_GRP_GEN    0xFF  // Generic group
#define MKHI_GRP_HMRFPO 0x05  // Host ME Region Flash Protection Override
#define MKHI_GRP_NM     0x11  // Node Manager extension to MKHI
#define MKHI_GRP_DFUSE  0x13  // Dynamic Fusing


/******************************************************************************
 * Generic MKHI messages group
 */
#define MKHI_CMD_GET_MKHI_VERSION 0x01
#define MKHI_CMD_GET_FW_VERSION   0x02
#define MKHI_CMD_END_OF_POST      0x0C

//
// MKHI Get Version message structure
// This request provides MKHI protocol definition version.
//
#pragma pack(1)
typedef struct
{
  MKHI_MSG_HEADER  Mkhi;
} MKHI_MSG_GET_MKHI_VERSION_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MSG_HEADER  Mkhi;
  UINT16           Minor;
  UINT16           Major;
} MKHI_MSG_GET_MKHI_VERSION_RSP;
#pragma pack()


//
// MKHI Get Firmware Version message structure
// Backup firmware version is optional, response may not contain this field
// if ME does not implement dual-image configuration.
//
//
// ME firmware version numbers structure
//
typedef struct
{
  UINT16  Minor;
  UINT8   Major;
  UINT8   ProdId;
  UINT16  Build;
  UINT16  Patch;
} MKHI_FW_VERSION;

#pragma pack(1)
typedef struct
{
  MKHI_MSG_HEADER  Mkhi;
} MKHI_MSG_GET_FW_VERSION_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MSG_HEADER  Mkhi;
  MKHI_FW_VERSION  Act;  // Active operatinal firmware
  MKHI_FW_VERSION  Rcv;  // Recovery firmware
  MKHI_FW_VERSION  Bkp;  // Backup operational firmwar (optional)
} MKHI_MSG_GET_FW_VERSION_RSP;
#pragma pack()


//
// MKHI End Of POST message structure
//
#pragma pack(1)
typedef struct
{
  MKHI_MSG_HEADER  Mkhi;
} MKHI_MSG_END_OF_POST_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MSG_HEADER  Mkhi;
  UINT32           Action;
} MKHI_MSG_END_OF_POST_RSP;
#pragma pack()


/******************************************************************************
 * Host ME Region Flash Protoction Override MKHI messages group
 */
#define HMRFPO_CMD_ENABLE    0x01
#define HMRFPO_CMD_LOCK      0x02
#define HMRFPO_CMD_GETSTATUS 0x03

//Aptio V server override Start: Me update changes.
#define HMRFPO_CMD_MERESET 0x00

#pragma pack(1)
typedef struct
{
  MKHI_MSG_HEADER Mkhi;
  UINT64          Nonce;
} MKHI_MSG_HMRFPO_MERESET_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{ 
  MKHI_MSG_HEADER Mkhi;
  UINT8           Status;
  UINT8           Reserved[3];
} MKHI_MSG_HMRFPO_MERESET_RSP;
#pragma pack()
//Aptio V server override End: Me update changes.

#pragma pack(1)
typedef struct
{
  MKHI_MSG_HEADER Mkhi;
  UINT64          Nonce;
} MKHI_MSG_HMRFPO_ENABLE_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MSG_HEADER Mkhi;
  UINT32          FactoryBase;
  UINT32          FactoryLimit;
  UINT8           Status;
  UINT8           Reserved[3];
} MKHI_MSG_HMRFPO_ENABLE_RSP;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MSG_HEADER Mkhi;
} MKHI_MSG_HMRFPO_LOCK_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MSG_HEADER Mkhi;
  UINT64          Nonce;
  UINT32          FactoryBase;
  UINT32          FactoryLimit;
  UINT8           Status;
  UINT8           Reserved[3];
} MKHI_MSG_HMRFPO_LOCK_RSP;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MSG_HEADER Mkhi;
} MKHI_MSG_HMRFPO_GETSTATUS_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MSG_HEADER Mkhi;
  UINT8           Status;
  UINT8           Reserved[3];
} MKHI_MSG_HMRFPO_GETSTATUS_RSP;
#pragma pack()

/******************************************************************************
 * ISVT (Silicon View Technology) PCH DEBUG CAPABILITIES enable
   MKHI messages group
 */
#define ENABLE_MASK_MDES     (0x01)
#define ENABLE_MASK_PCH_SVT  (0x02)
#define PCH_SVT_RSP_SUCCESS  (0x00)

#pragma pack(1)
typedef struct
{
  MKHI_MSG_HEADER  Mkhi;
  UINT8            EnableMask;
} MKHI_MSG_ISVT_ENABLE_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MSG_HEADER  Mkhi;
  UINT8            Result;
} MKHI_MSG_ISVT_ENABLE_RSP;
#pragma pack()


/******************************************************************************
 * Dynamic Fusing interface
 */
#define SPS_CLIENTID_ME_DYNFUSE 0x7

#pragma pack(1)
typedef struct
{
  MKHI_MSG_HEADER Mkhi;
} SPS_DYNAMIC_FUSING_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MSG_HEADER Mkhi;
  UINT8           Result;
} SPS_DYNAMIC_FUSING_RSP;
#pragma pack()

typedef enum {
   CPU_DYNAMIC_FUSING_SUCCESS ,
   CPU_DYNAMIC_FUSING_GRST_REQUIRED ,
   CPU_DYNAMIC_FUSING_END_OF_POST_RECEIVED ,
   CPU_DYNAMIC_FUSING_ROL_SOFT_DISABLED
 } DYNAMIC_FUSING_RESPONSE_TYPE ;


/******************************************************************************
 * SPS Node Manager messages
 ******************************************************************************/
#define NM_CMD_HOSTCFG  0x00
#define MAX_ACPI_PSTATES 16

#pragma pack(1)
typedef struct
{
  MKHI_MSG_HEADER Mkhi;
  union
  {
    UINT16        Uint16;
    struct
    {
      UINT16      TurboEn   : 1,
                  SmiOptim  : 1,
                  PowerMsmt : 1,
                  HwChange  : 1,
                  Resrerve0 :12;
    } Bits;
  } Capabilities;
  UINT8           PStatesNumber;
  UINT8           TStatesNumber;
  UINT16          MaxPower;
  UINT16          MinPower;
  UINT8           ProcNumber;
  UINT8           ProcCoresNumber;
  UINT8           ProcCoresEnabled;
  UINT8           ProcThreadsEnabled;
  UINT64          TurboRatioLimit;
  UINT64          PlatformInfo;
  INT32           Altitude;
  UINT8           PStatesRatio[MAX_ACPI_PSTATES];
} MKHI_MSG_NM_HOST_CFG_REQ;
#pragma pack()

//Thermal throttling messages
#define SPS_CMD_SET_PCH_TEMP_REPORTING_CFG_REQ 0x0B
#define SPS_CMD_SET_PCH_TEMP_REPORTING_CFG_RSP (0x80 | SPS_CMD_SET_PCH_TEMP_REPORTING_CFG_REQ)

#pragma pack(1)
typedef struct
{
  UINT8           Command;
  UINT8           Reserved;
  UINT16          PchTempReportingInterval;
  UINT16          PchTempMaximumLowPowerInterval;
} HECI_MSG_SET_PCH_TEMP_REPORTING_CFG_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{
  UINT8           Command;
  UINT16          Reserved;
  UINT8           Result;
} HECI_MSG_SET_PCH_TEMP_REPORTING_CFG_RSP;
#pragma pack()

/*****************************************************************************
 * ICC interface
 *****************************************************************************/
#define SPS_CLIENTID_ME_ICC 0x8

#define ICC_API_3_0 0x00030000
#define ICC_API_2_0 0x00020000
#define ICC_API_1_0 0x00010000

#define ICC_DEFAULT_CLOCK_ENABLES      0x80FF0F8F
#ifdef DE_SKU
#define ICC_DEFAULT_CLOCL_ENABLE_MASK  0x69FF0F8F // "Clock Mask Before POST" in XML
#else
#define ICC_DEFAULT_CLOCL_ENABLE_MASK  0x6DFF0F8F // "Clock Mask Before POST" in XML
#endif

//
// ICC HECI message commands
//
#define ICC_CMD_SET_CLOCK_ENABLES   0x03
#define ICC_CMD_SET_SPREAD_SPECTRUM 0x0c

//
// SPS ICC Status Codes
//
#define ICC_STS_SUCCESS                      0
#define ICC_STS_INCORRECT_API_VERSION        2
#define ICC_STS_INVALID_FUNCTION             3
#define ICC_STS_INVALID_BUFFER_LENGTH        4
#define ICC_STS_INVALID_PARAMETERS           5
#define ICC_STS_ENABLED_CLOCK_MASK_VIOLATION 12

#pragma pack(1)

typedef struct {
  UINT32 ApiVersion;
  UINT32 IccCommand;
  UINT32 IccStatus;
  UINT32 BufferLength;
  UINT32 Reserved;
} ICC_MSG_HEADER;

typedef struct {
  ICC_MSG_HEADER Header;
  UINT32         ClockEnables;
  UINT32         ClockEnablesMask;
  UINT32         Params;
} ICC_MSG_SET_CLK_ENABLES_REQ;

typedef struct {
  ICC_MSG_HEADER        Header;
} ICC_MSG_SET_CLK_ENABLES_RSP;

typedef struct {
  ICC_MSG_HEADER Header;
  UINT32         ModeSelect;
  UINT32         Params;
} ICC_MSG_SET_SPREAD_SPECTRUM_REQ;

typedef struct {
  ICC_MSG_HEADER Header;
} ICC_MSG_SET_SPREAD_SPECTRUM_RSP;


/*****************************************************************************
 * SPS messages for ME-BIOS interface definition version
 *****************************************************************************/
#define SPS_CLIENTID_ME_SPS   0x20

#define SPS_CMD_GET_MEBIOS_INTERFACE_REQ 1
#define SPS_CMD_GET_MEBIOS_INTERFACE_RSP (0x80 | SPS_CMD_GET_MEBIOS_INTERFACE_REQ)

#pragma pack(1)
typedef struct {
  HBM_COMMAND       Command;
} SPS_MSG_GET_MEBIOS_INTERFACE_REQ;

typedef struct {
    HBM_COMMAND     Command;
    UINT8           VerMajor;
    UINT8           VerMinor;
    UINT16          FeatureSet;
} SPS_MSG_GET_MEBIOS_INTERFACE_RSP;
#pragma pack()

//
// BIOS must verify the SPS ME-BIOS Interface Specification version to make
// sure BIOS and ME firmware will talk the sam laguage.
// There can be different versions for recovery and operational ME firmware
// after update of operational firmware.
//
#define SPS_MEBIOS_OPR_VERSION_MIN ((0 << 8) | 0)
#define SPS_MEBIOS_OPR_VERSION_MAX ((0xFF << 8) | 0xFF)
#define SPS_MEBIOS_RCV_VERSION_MIN ((0 << 8) | 0)
#define SPS_MEBIOS_RCV_VERSION_MAX ((0xFF << 8) | 0xFF)
#define SpsMeBiosOprVersionVerify(Major, Minor)                      \
        ((((Major) << 8) | (Minor)) >= SPS_MEBIOS_OPR_VERSION_MIN && \
         (((Major) << 8) | (Minor)) <= SPS_MEBIOS_OPR_VERSION_MAX)
#define SpsMeBiosRcvVersionVerify(Major, Minor)                      \
        ((((Major) << 8) | (Minor)) >= SPS_MEBIOS_RCV_VERSION_MIN && \
         (((Major) << 8) | (Minor)) <= SPS_MEBIOS_RCV_VERSION_MAX)

/*****************************************************************************
* MCTP Bus Owner Proxy Configuration
*****************************************************************************/
#define SPS_CLIENTID_ME_MCTP   0x21

#define MCPT_CMD_SET_BUS_OWNER_REQ   0x01
#define MCPT_CMD_SET_BUS_OWNER_RSP (0x80 | MCPT_CMD_SET_BUS_OWNER_REQ)
#define MCPT_CMD_SET_BUS_OWNER_RSP_SUCCESS (0x00)

#pragma pack(1)
typedef struct {
  UINT8           Command;
  UINT8           Reserved0[3];
  UINT16          PCIeAddress;
  UINT8           Location;
  UINT8           Reserved1;
} MCTP_SET_BUS_OWNER_REQ;

typedef struct {
  UINT8           Command;
  UINT8           Reserved0[2];
  UINT8           Result;
} MCTP_SET_BUS_OWNER_RSP;
#pragma pack()

//
// Core Bios Done
//
typedef struct {
  MKHI_MSG_HEADER MkhiHeader;
} CORE_BIOS_DONE;

typedef struct {
  MKHI_MSG_HEADER MkhiHeader;
} CORE_BIOS_DONE_ACK;

#endif // _SPS_H_

