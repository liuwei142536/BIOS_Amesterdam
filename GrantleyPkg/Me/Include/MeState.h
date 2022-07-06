/*++
  This file contains an 'Intel Peripheral Driver' and uniquely  
  identified as "Intel Reference Module" and is                 
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/

/*++

Copyright (c) 2009-2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  MeState.h

Abstract:

  This file contains HeciRegs.h extension specific to AMT firmware.

--*/
#ifndef _ME_STATE_H
#define _ME_STATE_H

#ifndef VFRCOMPILE
//
// HECI PCI register definition
//
#define R_VENDORID  0x00
#define R_DEVICEID  0x02
#define R_COMMAND   0x04
#define  B_BME       0x04
#define  B_MSE       0x02
#define R_REVID     0x08
#define R_HECIMBAR  0x10
#define R_IRQ       0x3C
#define R_FWSTATE   0x40
#define R_GEN_STS   0x4C
#define R_HIDM      0xA0
//
// Firmware Status
//
typedef union {
  UINT32  ul;
  struct {
    UINT32  CurrentState : 4;         // 0:3 - Current State
    UINT32  ManufacturingMode : 1;    // 4 Manufacturing Mode
    UINT32  FptBad : 1;               // 5 FPT(Flash Partition Table ) Bad
    UINT32  MeOperationState : 3;     // 6:8 - ME Operation State
    UINT32  FwInitComplete : 1;       // 9
    UINT32  FtBupLdFlr : 1;           // 10 - This bit is set when firmware is not able to load BRINGUP from the fault tolerant (FT) code.
    UINT32  FwUpdateInprogress : 1;   // 11
    UINT32  ErrorCode : 4;            // 12:15 - Error Code
    UINT32  MeOperationMode : 4;      // 16:19 - Management Engine Current Operation Mode
    UINT32  Reserved2 : 4;            // 20:23
    UINT32  MeBootOptionsPresent : 1; // 24 - If this bit is set, a Boot Options is present
    UINT32  AckData : 3;              // 25:27 Ack Data
    UINT32  BiosMessageAck : 4;       // 28:31 BIOS Message Ack
  } r;
} HECI_FWS_REGISTER;
#endif // VFRCOMPILE

//
// ME Current State Values
//
#define ME_STATE_RESET        0x00
#define ME_STATE_INIT         0x01
#define ME_STATE_RECOVERY     0x02
#define ME_STATE_NORMAL       0x05
#define ME_STATE_DISABLE_WAIT 0x06
#define ME_STATE_TRANSITION   0x07
#define ME_STATE_INVALID      0x0F

//
// ME Operation State Values
//
#define ME_OPERATION_STATE_PREBOOT  0x00
#define ME_OPERATION_STATE_M0_UMA   0x01
#define ME_OPERATION_STATE_M3       0x04
#define ME_OPERATION_STATE_M0       0x05
#define ME_OPERATION_STATE_BRINGUP  0x06
#define ME_OPERATION_STATE_M0_ERROR 0x07

#define ME_FIRMWARE_COMPLETED       1
//
// ME Error Code Values
//
#define ME_ERROR_CODE_NO_ERROR      0x00
#define ME_ERROR_CODE_UNKNOWN       0x01
#define ME_ERROR_CODE_IMAGE_FAILURE 0x03
#define ME_ERROR_CODE_DEBUG_FAILURE 0x04

//
// Management Engine Current Operation Mode
//
#define ME_OPERATION_MODE_NORMAL            0x00
#define ME_OPERATION_MODE_DEBUG             0x02
#define ME_OPERATION_MODE_SOFT_TEMP_DISABLE 0x03
#define ME_OPERATION_MODE_SECOVR_JMPR       0x04
#define ME_OPERATION_MODE_SECOVR_HECI_MSG   0x05
#define ME_OPERATION_MODE_SPS               0x0F

#ifndef VFRCOMPILE
//
// MISC_SHDW
//
typedef union {
  UINT32  ul;
  struct {
    UINT32  MUSZ : 6;       // 0:5 - ME UMA Size
    UINT32  Reserved : 8;   // 6:13 - Reserved
    UINT32  Reserved2 : 2;  // 14:15 - Reserved
    UINT32  MUSZV : 1;      // 16:16 - ME UMA Size Valid
    UINT32  Reserved3 : 8;  // 17:24 - Reserved
    UINT32  Reserved4 : 6;  // 25:30 - Reserved
    UINT32  MSVLD : 1;      // 31:31 - Miscellaneous Shadow Valid
  } r;
} HECI_MISC_SHDW_REGISTER;

//
// GS_SHDW
//
typedef union {
  UINT32  ul;
  struct {
    UINT32  BistInProg : 1;       // 0 - BIST in progress
    UINT32  IccProgSts : 2;       // 1:2 - ICC Prog STS
    UINT32  InvokeMEBx : 1;       // 3 - Invoke MEBX
    UINT32  CpuReplacedSts : 1;   // 4 - CPU Replaced STS
    UINT32  MbpRdy : 1;           // 5 - MBP RDY
    UINT32  MfsFailure : 1;       // 6 - MFS Failure
    UINT32  WarmRstReqForDF : 1;  // 7 - Warm Reset Required for Dynamic Fusing
    UINT32  CpuReplacedValid : 1; // 8 - CPU Replaced Valid
    UINT32  Reserved : 2;         // 9:10 - Reserved
    UINT32  FwUpdIpu : 1;         // 11 - FW UPD IPU Needed
    UINT32  Reserved2 : 4;        // 12:15 - Reserved
    UINT32  ExtStatCode1 : 8;     // 16:23 - EXT Status Code 1
    UINT32  ExtStatCode2 : 4;     // 24:27 - EXT Status Code 2
    UINT32  InfPhaseCode : 4;     // 31:28 - Infra. Phase code
  } r;
} HECI_GS_SHDW_REGISTER;
#endif // VFRCOMPILE


#define BRNGUP_HMRFPO_DISABLE_CMD_MASK  0x0F
#define BRNGUP_HMRFPO_DISABLE_CMD       0x30
#define BRNGUP_HMRFPO_DISABLE_OVR_MASK  0xF0000000
#define BRNGUP_HMRFPO_DISABLE_OVR_RSP   0x30000000

///
/// Ignore ME_FW_INIT_COMPLETE status Macro
///
#define ME_STATUS_ME_STATE_ONLY(a)  ((a) & (~(ME_FW_INIT_COMPLETE | ME_FW_BOOT_OPTIONS_PRESENT)))
//
// Ignore ME_FW_INIT_COMPLETE status Macro
//
#define ME_STATUS_IGNORE_FW_INIT_COMPLETE(a)  ((a) & (~ME_FW_INIT_COMPLETE))

//
// Macro to check if ME FW INIT is completed
//
#define ME_STATUS_IS_ME_FW_INIT_COMPLETE(a) (((a) & ME_FW_INIT_COMPLETE) == ME_FW_INIT_COMPLETE)

//
// Marco to combind the complete bit to status
//
#define ME_STATUS_WITH_ME_INIT_COMPLETE(a)  ((a) | ME_FW_INIT_COMPLETE)

//
// Macro to check ME Boot Option Present
//
#define ME_STATUS_IS_ME_FW_BOOT_OPTIONS_PRESENT(a)  (((a) & ME_FW_BOOT_OPTIONS_PRESENT) == ME_FW_BOOT_OPTIONS_PRESENT)

//
// Abstract ME Mode Definitions
//
#define ME_MODE_NORMAL        0x00
#define ME_MODE_DEBUG         0x02
#define ME_MODE_TEMP_DISABLED 0x03
#define ME_MODE_SECOVER       0x04
#define ME_MODE_MASK          0x0F

#define ME_MODE_FAILED  0x06
//
// Abstract ME Status definitions
//
#define ME_READY                    0x00
#define ME_INITIALIZING             0x01
#define ME_IN_RECOVERY_MODE         0x02
#define ME_DISABLE_WAIT             0x06
#define ME_TRANSITION               0x07
#define ME_NOT_READY                0x0F
#define ME_INVALID                  0xFF
#define ME_FW_INIT_COMPLETE         0x80
#define ME_FW_BOOT_OPTIONS_PRESENT  0x100

#define ME_FW_UPDATES_IN_PROGRESS   0x200

#endif // ME_STATE_H

