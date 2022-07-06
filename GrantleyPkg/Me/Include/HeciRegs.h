/*++
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2010-2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  HeciRegs.h

Abstract:

  Register definitions for HECI.

--*/
#ifndef _HECI_REGS_H
#define _HECI_REGS_H
#define SPS_FC                          0x01

#define HECI_BUS                        ME_BUS
#ifndef HECI_DEV
#define HECI_DEV                        ME_DEVICE_NUMBER
#endif
#ifndef HECI_FUN
#define HECI_FUN                        HECI_FUNCTION_NUMBER
#endif
//#define HECI2_FUN                       HECI2_FUNCTION_NUMBER

#define REVID_MSK                       0x000000FF

#define BRNGUP_HMRFPO_DISABLE_CMD_MASK  0x0F
#define BRNGUP_HMRFPO_DISABLE_CMD       0x30
#define BRNGUP_HMRFPO_DISABLE_OVR_MASK  0xF0000000
#define BRNGUP_HMRFPO_DISABLE_OVR_RSP   0x30000000

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

#define PCI_VENDOR_ID_OFFSET              0x00 // AptioV override

//
// HECIMBAR register definition
//
#define H_CB_WW   0x00
//#define H_CSR     0x04
#define ME_CB_RW  0x08
#define ME_CSR_HA 0x0C

/**
 * HECI functions location on PCI bus.
 */
#define HECI1_BUS   0
#define HECI1_DEV   22
#define HECI1_FUN   0

#define HECI2_BUS   0
#define HECI2_DEV   22
#define HECI2_FUN   1


/**
 * HECI PCI config space registers list.
 *
 * Note that HECI Device Id varies between chipset generations and also between
 * client and server chipset SKU.
 * Note that H_GSx registers are host status registers writable for host,
 * read only for ME, and HSF + GS_SHDWx registers are ME status writeable
 * for ME, read only for host side. HFS in HECI-1 is the well known ME Firmware
 * Status 1 register.
 * The definition of the status registers functionality depends on the type of
 * firmwere running in ME.
 */
#define HECI_R_VENDORID  0x00
#define HECI_R_DEVICEID  0x02
#define HECI_R_COMMAND   0x04
#define HECI_R_REVID     0x08
#define HECI_R_MBAR      0x10
#define HECI_R_IRQ       0x3C
#define HECI_R_HIDM      0xA0
#define HECI_R_HFS       0x40
#define HECI_R_MISC_SHDW 0x44
#define HECI_R_GS_SHDW   0x48
#define HECI_R_H_GS      0x4C
#define HECI_R_GS_SHDW2  0x60
#define HECI_R_GS_SHDW3  0x64
#define HECI_R_GS_SHDW4  0x68
#define HECI_R_GS_SHDW5  0x6C
#define HECI_R_H_GS2     0x70
#define HECI_R_H_GS3     0x74
#define HECI_R_MEFS1     HECI_R_HFS // HFS in HECI-1 is ME Firmware Status 1
#define HECI_R_MEFS2     HECI_R_GS_SHDW // GS_SHDW in HECI-1 is ME Firmware Status 2

#define HeciPciReadMefs1()                                                    \
        PciRead32(PCI_LIB_ADDRESS(HECI1_BUS, HECI1_DEV, HECI1_FUN, HECI_R_MEFS1))
#define HeciPciReadMefs2()                                                    \
        PciRead32(PCI_LIB_ADDRESS(HECI1_BUS, HECI1_DEV, HECI1_FUN, HECI_R_MEFS2))

/**
 * HECI command register bits.
 */
#define HECI_CMD_BME     0x04  // Bus Master Enable
#define HECI_CMD_MSE     0x02  // Memory Space Enable

/**
 * Default values to be used in HECI_R_MBAR before PCI enumeration.
 */
#define HECI1_MBAR_DEFAULT 0xFEDB0000
#define HECI2_MBAR_DEFAULT 0xFEDB0010


/**
 * HECI Interrupt Delivery Mode to be set in HECI_R_HIDM.
 */
#define HECI_HIDM_MSI 0
#define HECI_HIDM_SCI 1
#define HECI_HIDM_SMI 2
#define HECI_HIDM_LAST HECI_HIDM_SMI

#pragma pack(1)

/****************** REGISTER EQUATES ****************************************************/

//
// ME_CSR_HA - ME Control Status Host Access
//
typedef union {
  UINT32  ul;
  struct {
    UINT32  ME_IE_HRA : 1;    // 0 - ME Interrupt Enable (Host Read Access)
    UINT32  ME_IS_HRA : 1;    // 1 - ME Interrupt Status (Host Read Access)
    UINT32  ME_IG_HRA : 1;    // 2 - ME Interrupt Generate (Host Read Access)
    UINT32  ME_RDY_HRA : 1;   // 3 - ME Ready (Host Read Access)
    UINT32  ME_RST_HRA : 1;   // 4 - ME Reset (Host Read Access)
    UINT32  Reserved : 3;     // 7:5
    UINT32  ME_CBRP_HRA : 8;  // 15:8 - ME CB Read Pointer (Host Read Access)
    UINT32  ME_CBWP_HRA : 8;  // 23:16 - ME CB Write Pointer (Host Read Access)
    UINT32  ME_CBD_HRA : 8;   // 31:24 - ME Circular Buffer Depth (Host Read Access)
  } r;
} HECI_ME_CONTROL_REGISTER;

//
// H_CSR - Host Control Status
//
typedef union {
  UINT32  ul;
  struct {
    UINT32  H_IE : 1;     // 0 - Host Interrupt Enable ME
    UINT32  H_IS : 1;     // 1 - Host Interrupt Status ME
    UINT32  H_IG : 1;     // 2 - Host Interrupt Generate
    UINT32  H_RDY : 1;    // 3 - Host Ready
    UINT32  H_RST : 1;    // 4 - Host Reset
    UINT32  Reserved : 3; // 7:5
    UINT32  H_CBRP : 8;   // 15:8 - Host CB Read Pointer
    UINT32  H_CBWP : 8;   // 23:16 - Host CB Write Pointer
    UINT32  H_CBD : 8;    // 31:24 - Host Circular Buffer Depth
  } r;
} HECI_HOST_CONTROL_REGISTER;

#define MAX_BUFFER_DEPTH 0x80

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
// DRAM Initiailization Response Codes.
//
#define CBM_DIR_NON_PCR            0x01
#define CBM_DIR_PCR                0x02
#define CBM_DIR_GLOBAL_RESET       0x06
#define CBM_DIR_CONTINUE_POST      0x07
// Additional code for UMA support
#define CBM_DIR_DO_NOTHING         0x00
#define CBM_DIR_NON_PCR_CLEARDISB  0xFF

//
// ME SPS Specific States
//
#define ME_STATE_HARD_ME_DISABLED          0x00
#define ME_STATE_INITIALIZING	           0x01
#define ME_STATE_ISPS_INACTIVE		   0x04
/* BROMOLOW_TODO: Verify the new spec */
#define ME_STATE_ISPS_ACTIVE		   0x05

//
// ME Boot Options Present
//
#define ME_BOOT_OPTIONS_PRESENT     0x01
#define ME_BOOT_OPTIONS_NOT_PRESENT 0x00

//
// ME Operation State Values
//
#define ME_OPERATION_STATE_PREBOOT  0x00
#define ME_OPERATION_STATE_M0_UMA   0x01
#define ME_OPERATION_STATE_M3       0x04
#define ME_OPERATION_STATE_M0       0x05
#define ME_OPERATION_STATE_BRINGUP  0x06
#define ME_OPERATION_STATE_M0_ERROR 0x07

//
// ME Error Code Values
//
#define ME_ERROR_CODE_NO_ERROR      0x00
#define ME_ERROR_CODE_UNKNOWN       0x01
#define ME_ERROR_CODE_IMAGE_FAILURE 0x03
#define ME_ERROR_CODE_DEBUG_FAILURE 0x04

//
// ME SPS Specific States
//
#define ME_ERROR_CODE_UNCATEGORIZED       0x01
#define ME_ERROR_CODE_DISABLED		  0x02

//
// Management Engine Current Operation Mode
//
#define ME_OPERATION_MODE_NORMAL            0x00
#define ME_OPERATION_MODE_DEBUG             0x02
#define ME_OPERATION_MODE_SOFT_TEMP_DISABLE 0x03
#define ME_OPERATION_MODE_SECOVR_JMPR       0x04
#define ME_OPERATION_MODE_SECOVR_HECI_MSG   0x05
#define ME_OPERATION_MODE_SPS               0x0F

// SPS FW Specific Booting Mode Options for NM/DCMI
#define HECI_INTERRUPT_DELIVERY_MODE_MASK	0xFC	// RESERVED Bits.

#define HECI_INTERRUPUT_GENERATE_LEGACY_MSI	0x00
#define HECI_INTERRUPUT_GENERATE_SCI		0x01
#define HECI_INTERRUPUT_GENERATE_SMI		0x10	

#define ME_BOOT_MODE_MASK		0x1

#define ME_BOOT_MODE_POWER_OPTIMIZED	0x0
#define ME_BOOT_MODE_PERFORMANCE_OPTIMIZED		0x1

#define DCMI_HOST_CLIENT_ADDRESS 0x02
#define FLOW_CONTROL_CMD         0x08

// HBM Structures
typedef struct _HBM_COMMAND_FC
{
   UINT8                   Command:7;
   UINT8                   IsResponse:1;
} HBM_COMMAND_FC;

typedef struct _HBM_FLOW_CONTROL
{
   HBM_COMMAND_FC             Command;
   UINT8                   MEAddress;
   UINT8                   HostAddress;
   UINT8                   Reserved[5];
} HBM_FLOW_CONTROL;

#pragma pack()

/**
 * ME Firmware Status 1 register basics.
 *
 * ME Firmware Status 1 register is in HECI-1 configuration space at offset 40h.
 * Full definition of the register depends on type of the firmwere running in ME.
 * The structure HECI_MEFS1 defines only common, basic part.
 */
typedef union
{
  UINT32   DWord;
  struct
  {
    UINT32 CurrentState : 4,  //< 0:3   Current ME firmware state
           Reserved0    : 5,  //  4:8
           InitComplete : 1,  //< 9     ME firmware finished initialization
           Reserved1    : 2,  // 10:11
           ErrorCode    : 4,  //< 12:15 If set means fatal error
           OperatingMode: 4,  //< 16:19 Current ME operating mode
           Reserved2    : 5,  //< 20:24
           MsgAckData   : 3,  //< 25:27 MSG ACK Data specific for acknowledged BIOS message
           MsgAck       : 4;  //< 28:31 Acknowledge for register based BIOS message
  } Bits;
} HECI_MEFS1;

/**
 * HECI_MEFS::CurrentState values list.
 *
 * This field describes the current state of the firmware.
 */
#define MEFS1_CURSTATE_RESET        0  //< ME is in reset, will exit this state within 1 ms
#define MEFS1_CURSTATE_INIT         1  //< ME is initializing, will exit this state within 2 s
#define MEFS1_CURSTATE_RECOVERY     2  //< ME is in recovery mode
#define MEFS1_CURSTATE_DISABLED     4  //< ME functionality has been disabled
#define MEFS1_CURSTATE_NORMAL       5  //< ME is in normal operational state
#define MEFS1_CURSTATE_DISABLE_WAIT 6  //< Not used with SPS
#define MEFS1_CURSTATE_TRANSITION   7  //< ME is transitioning to a new operating State

/**
 * HECI_MEFS::ErrorCode values list.
 *
 * If HECI_MEFS::ErrorCode is set to non zero value the ME firmware has
 * encountered a fatal error and stopped normal operation.
 */
#define MEFS1_ERRCODE_NOERROR       0  //< ME firmware does not report errors
#define MEFS1_ERRCODE_UNKNOWN       1  //< Unkategorized error occured
#define MEFS1_ERRCODE_DISABLED      2  //< ME firmware disabled with for (debug)
#define MEFS1_ERRCODE_IMAGEFAIL     3  //< No valid firmware image in ME region

/**
 * HECI_MEFS1::OperatingMode values list.
 *
 * This field describes the current operating mode of ME.
 */
#define MEFS1_OPMODE_NORMAL         0  // Client firmware is running in ME
#define MEFS1_OPMODE_IGNITION       1  // Ignition firmware is running in ME
#define MEFS1_OPMODE_DEBUG          2  // 
#define MEFS1_OPMODE_TEMP_DISABLE   3  // 
#define MEFS1_OPMODE_SECOVR_JMPR    4  // Security Override activated with jumper
#define MEFS1_OPMODE_SECOVR_MSG     5  // Security Override activated with HMRFPO_ENABLE request
#define MEFS1_OPMODE_SPS           15  // SPS firmware is running in ME


/*
 * MISC_SHDW register in HECI-1 config space at offset 44h
 *
 * If MSVLD bit is not set the register is not implemented.
 * Only in HECI-1 this register is implemented.
 */
typedef union {
  UINT32   DWord;
  struct {
    UINT32 MUSZ     : 6,  // 0:5 - ME UMA Size
           Reserved0:10,  // 6:15 - Reserved
           MUSZV    : 1,  // 16:16 - ME UMA Size Valid
           Reserved1:14,  // 17:30 - Reserved
           MSVLD    : 1;  // 31:31 - Miscellaneous Shadow Valid
  } Bits;
} HECI_MISC_SHDW;

/**
 * HECI message header.
 *
 * HECI massage header is one double word long. It identifies communication
 * entities (subsystems) on ME and host side and provides the exact number
 * of bytes in the message body following this header.
 */
typedef union
{
  UINT32   DWord;
  struct
  {
    UINT32 MeAddress  : 8,  ///< Addressee on ME side
           HostAddress: 8,  ///< Addressee on host siede, zero for BIOS
           Length     : 9,  ///< Number of bytes following the header
           Reserved   : 6,
           MsgComplete: 1;  ///< Whether this is last fragment of a message
  } Bits;
} HECI_MSG_HEADER;

/**
 * Macro for building HECI message header dword using given ingredients.
 */
#define HeciMsgHeader(MeAdr, HstAdr, Len, Cpl) \
        (UINT32)(((MeAdr) & 0xFF) | ((HstAdr) & 0xFF) << 8 | ((Len) & 0x1FF) << 16 | ((Cpl) << 31))

/**
 * Maximum length of HECI message.
 *
 * Actually it is maximum length of HECI queue that can be configured by ME.
 * Single message must not exceed HECI queue size. Current HECI queue size is
 * configured by ME, so this macro does not define current maximum message
 * length. It defines maximum length in case ME configures maximum queue size.
 */
#define HECI_MSG_MAXLEN (0x80 * sizeof(UINT32))

#endif // HECI_REGS_H

