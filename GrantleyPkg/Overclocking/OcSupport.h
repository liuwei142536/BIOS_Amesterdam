/** @file
  Header file for Overclocking Support implementation.

@copyright
  Copyright (c) 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
**/
#ifndef _OVERCLOCKING_LIBRARY_H_
#define _OVERCLOCKING_LIBRARY_H_


#include <Base.h>
#include <PiPei.h>
#include <Ppi/CpuIo.h>
#include <Ppi/PciCfg.h>
#include <Ppi/SmBus2.h>
#include <Ppi/Stall.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/PchReset.h>
#include <Ppi/PchPlatformPolicy.h>
#include <Ppi/PchInit.h>
#include <Ppi/Wdt.h>
#include <Library/BaseLib.h>
#include <Library/PciLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/PlatformHooksLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PchPlatformLib.h>
#include <Library/CmosAccessLib.h>
#include <Library/CmosAccessLib/CmosMap.h>
#include <Guid/PlatformInfo.h>
#include <Guid/SetupVariable.h>
#include <Cpu\CpuRegs.h>
#include <PchRegs.h>
#include <PchRegs/PchRegsRcrb.h>
#include <Platform.h>
#include <PchRegs/PchRegsSpi.h>
#include <PchRegs/PchRegsLpc.h>
#include <PchAccess.h>
#include <Iio/IioRegs.h>

//
// VccIO SMBUS definition
//
#define EFI_VCCIO_SMBUS_ADDRESS    0x60
#define EFI_VCCIO_SMBUS_REG        0x01

//
//  Memory/VCCin Voltage SMBUS Device Address and Registers
//
#define EFI_C01VSM_SMBUS_ADDRESS   0x42
#define EFI_C23VSM_SMBUS_ADDRESS   0x44
#define EFI_VCCIN_SMBUS_ADDRESS    0x76

#define EFI_VR_GAMER_CMD           0xD2
#define  EFI_VR_RESERVED           BIT5
#define  EFI_VR_GAMER_MODE_EN_MASK BIT4
#define  EFI_VR_GAMER_TYPE_MASK    BIT3
#define  EFI_VR_LOAD_LINE_MASK     BIT2 | BIT1
#define   EFI_VR_LOAD_LINE_50      BIT1
#define   EFI_VR_LOAD_LINE_75      BIT2
#define   EFI_VR_LOAD_LINE_00      BIT1 | BIT2
#define  EFI_VR_GAMER_VID_MASK     BIT15 | BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8 | BIT0
#define  EFI_VR_GAMER_MODE_DEF     (~EFI_VR_GAMER_MODE_MASK) | EFI_VR_RESERVED

#define EFI_VID_PROCESS_CALL_WRITE_CMD     0xD0
#define EFI_VID_PROCESS_CALL_READ_CMD      0xD1


#define MSR_PCIE_PLLGEN3                    0x61E

#define PEG_DMI_RATIO_MASK                  0x3
#define PCIE_PLL_MASK                       (1 << 2)
#define PCIE_PLL_OFFSET                     2
#define FW_DMI_RATIO_MASK                   0x7
#define LONG_RESET_BIT                      BIT3


#define ME_BUS                              0
#define ME_DEVICE_NUMBER                    22
#define HECI_FUNCTION_NUMBER                0x00
#define FWSTATUS3_OFFSET                    0x60
#define FWSTATUS1_OFFSET                    0x40         // ME FW Status Register in HECI controller
#define H2M_MSG1_OFFSET                     0x4C         // Host to ME register in HECI controller
#define H2M_ICC_MESSAGE                     0x60000000   // In H2M register, bits[31..28] indicates message type. 6- ICC message
#define H2M_BCLK_RAMP_EN_BIT                BIT0         // Bits[27..0]  indicate message. Bit 0 enable ramp programming
#define M2H_MSG_ACK_MASK                    0xF0000000   // In FWSTS register, bit[31..28] indicate ack type.
#define M2H_ICC_MSG_ACK                     0x60000000   // ICC message Ack reports 6(ICC Ack) in the type
#define R_ME_FWSTS3_CORE_RESET_MASK         0x00000010
#define CPU_FILTER_PLL_MASK                 0x0020
#define CPU_FILTER_PLL_OFFSET               5
#define CPU_OCS_GPIO_MASK                   0x00000100
#define CPU_OCS_GPIO_OFFSET                 8
#define PCIE_RATIO_DIS_MASK                 0x00040000
#define PCIE_RATIO_DIS_OFFSET               18

// Masks for the combined GEN_PMCON_1 & GEN_PMCON_2 registers (PCICfgSpace at B=0:D=1F:F=0:Off=A0)
#define GEN_PMCON_12_DISB_MASK (1<<(16+7)) // DISB actually in GEN_PMCON_2 bit 7 (but code expects 32 DWORD)
#define GEN_PMCON_12_SR_MASK   (1<<(16+5)) // SR   actually in GEN_PMCON_2 bit 5 (but code expects 32 DWORD)

#define WDT_TIMEOUT                         60
	
// Bios to Pcode Mailbox

#define PCUBUS 0xFF
#define PCUDEV 0x1E
#define PCUFUN 0x01
#define MAILCOMM 0x90
#define MAILDATA 0x8C





//
//  OC Mailbox MSR
//
#define MSR_OC_MAILBOX        0x00000150
#define OC_LIB_WAIT_TIMEOUT   5000                     ///< 5 milliseconds

//
//  OC Mailbox commands
//
#define OC_LIB_CMD_GET_OC_CAPABILITIES              0x01
#define OC_LIB_CMD_GET_PER_CORE_RATIO_LIMIT         0x02
#define OC_LIB_CMD_GET_VOLTAGE_FREQUENCY            0x10
#define OC_LIB_CMD_SET_VOLTAGE_FREQUENCY            0x11
#define OC_LIB_CMD_GET_SVID_CONFIG                  0x12
#define OC_LIB_CMD_SET_SVID_CONFIG                  0x13
#define OC_LIB_CMD_GET_GLOBAL_CONFIG                0x14
#define OC_LIB_CMD_SET_GLOBAL_CONFIG                0x15

//
//VCU commands HSX
//
#define OPEN_SEQ_HSX	0x02
#define PVDRATIO_HSX	0x40008
#define ADJUST_PLL_HSX	0x3005
#define CLOSE_SEQ_HSX	0x03
#define SFRTRIM_HSX		0x02
#define PLLTRIM_HSX     0x01

//
//VCU Commands BDX
//

#define OPEN_SEQ_BDX	0x02
#define PLL_SEQ_BDX		0x50001
#define ADJUST_PLL_BDX	0x4001
#define CLOSE_SEQ_BDX	0x03
#define PLLTRIM_INDX	0x01




//
//  OC Mailbox completion codes
//
#define OC_LIB_COMPLETION_CODE_SUCCESS              0x00
#define OC_LIB_COMPLETION_CODE_OC_LOCKED            0x01
#define OC_LIB_COMPLETION_CODE_INVALID_DOMAIN       0x02
#define OC_LIB_COMPLETION_CODE_MAX_RATIO_EXCEEDED   0x03
#define OC_LIB_COMPLETION_CODE_MAX_VOLTAGE_EXCEEDED 0x04
#define OC_LIB_COMPLETION_CODE_OC_NOT_SUPPORTED     0x05
#define OC_LIB_COMPLETION_CODE_WRITE_FAILED         0x06
#define OC_LIB_COMPLETION_CODE_READ_FAILED          0x07

//
// Domain ID definitions
//
#define OC_LIB_DOMAIN_ID_IA_CORE    0x00
#define OC_LIB_DOMAIN_ID_GT         0x01
#define OC_LIB_DOMAIN_ID_CLR        0x02
#define OC_LIB_DOMAIN_ID_UNCORE     0x03
#define OC_LIB_DOMAIN_ID_VCCU       0x04
#define OC_LIB_DOMAIN_ID_IOA        0x05
#define OC_LIB_DOMAIN_ID_IOD        0x06

//
// Bit 10 is the S11.0.10V sign bit
//
#define FIXED_POINT_SIGN_BIT_MASK 0x0400
#define INT16_SIGN_BIT_MASK       0x8000

//
//  Voltage Conversion defines
//
#define MILLIVOLTS_PER_VOLT  1000
#define MAX_TARGET_MV      4095
#define MAX_OFFSET_MV      500
#define SVID_DEFAULT_VCC   0x79A

#define CONVERT_TO_FIXED_POINT_VOLTS  0
#define CONVERT_TO_BINARY_MILLIVOLT   1
#define NEGATIVEOFFSET 1
#define POSITIVEOFFSET 0
//
// Masks and offsets
//
#define BIT0_MASK             0x1
#define MAX_RATIO_MASK        0x000000FF
#define VOLTAGE_TARGET_MASK   0x000FFF00
#define VOLTAGE_TARGET_OFFSET 8
#define VOLTAGE_MODE_MASK     0x00100000
#define VOLTAGE_MODE_OFFSET   20
#define VOLTAGE_OFFSET_MASK   0xFFE00000
#define VOLTAGE_OFFSET_OFFSET 21

#define SVID_DISABLE_MASK     0x80000000
#define SVID_DISABLE_OFFSET   31
#define SVID_VOLTAGE_MASK     0x00000FFF

#define FIVR_FAULTS_MASK        0x00000001
#define FIVR_EFFICIENCY_MASK    0x00000002
#define FIVR_EFFICIENCY_OFFSET  1

#define OC_CAPS_MAX_RATIO_MASK          0x000000FF
#define OC_CAPS_RATIO_SUPPORT_MASK      0x00000100
#define OC_CAPS_RATIO_SUPPORT_OFFSET    8
#define OC_CAPS_OVERRIDE_SUPPORT_MASK   0x00000200
#define OC_CAPS_OVERRIDE_SUPPORT_OFFSET 9
#define OC_CAPS_OFFSET_SUPPORT_MASK     0x00000400
#define OC_CAPS_OFFSET_SUPPORT_OFFSET   10

//
// Voltage offset definitions
//
#define OC_LIB_OFFSET_ADAPTIVE  0
#define OC_LIB_OFFSET_OVERRIDE  1
#define OC_LIB_VOLTAGE_DO_NOT_UPDATE  0xFFFF
#define PCU_CR_RATIO_MSR	0x620
#define RATIO_LIMIT_MASK	0xFFFFFF80


//
//  Mailbox Related Definitions
//
#define MAILBOX_TYPE_PCODE              0x00000001
#define MAILBOX_TYPE_OC                 0x00000002
#define PCODE_MAILBOX_INTERFACE_OFFSET  0x5DA4
#define PCODE_MAILBOX_DATA_OFFSET       0x5DA0
#define OC_MAILBOX_MSR                  0x00000150
#define MCHBAR_OFFSET                   0x48

//
// Mailbox commands
//
#define READ_ICC_MAX_CMD                  0x80002A03
#define SAMPLE_TSC_24AND100_CMD           0x80000009
#define READ_TSC24_LOWER_CMD              0x80000109
#define READ_TSC24_UPPER_CMD              0x80000209
#define READ_TSC100_LOWER_CMD             0x80000309
#define READ_TSC100_UPPER_CMD             0x80000409
#define READ_PCODE_CALIBRATED_CMD         0x80000509
#define WRITE_CONVERTION_RATIO_CMD        0x80000609
#define WRITE_PREVENT_BCLKOFF_CMD         0x80000709
#define WRITE_FSM_MEASURE_INTVL_CMD       0x80009009
#define START_CAL_VALUE                   0x85000000
#define READ_PL1_DUTY_CYCLE_CLAMP_ENABLE  0x00000015
#define WRITE_PL1_DUTY_CYCLE_CLAMP_ENABLE 0x00000016
#define READ_DDR_FORCE_2X_REFRESH         0x00000017
#define WRITE_DDR_FORCE_2X_REFRESH        0x00000018

#ifndef MAILBOX_WAIT_TIMEOUT
#define MAILBOX_WAIT_TIMEOUT  1000  ///< 1 millisecond
#endif
#ifndef MAILBOX_WAIT_STALL
#define MAILBOX_WAIT_STALL    1     ///< 1 microsecond
#endif
#ifndef MAILBOX_READ_TIMEOUT
#define MAILBOX_READ_TIMEOUT  10    ///< 10 microseconds
#endif

typedef UINT32 CPU_RESET_TYPE;

#define NO_RESET              0
#define CPU_ONLY_RESET        1
#define WARM_RESET            2
#define COLDRESET             3

#define RESET_PPI_WARM_RESET  0
#define RESET_PPI_COLD_RESET  3

#define RESET_PORT            0x0CF9
#define CLEAR_RESET_BITS      0x0F1
#define PSIX_THRESHOLD_MASK   0x3FFFFFFF  ///< Bits 61:32 - Mask value respect to Dword.High
#define PSI1_THRESHOLD_VALUE  0x14
#define PSI2_THRESHOLD_VALUE  0x05
#define PSI3_THRESHOLD_VALUE  0x01
#define MAX_CLR_MASK          0x7F
#define AVX_NEG_WRITE         0x1B


#define MAX_OVERCLOCKING_BINS 0x7

//
//  OC Mailbox Structures
//
typedef union _OC_MAILBOX_INTERFACE {
  UINT32 InterfaceData;
  struct {
    UINT8 CommandCompletion : 8;
    UINT8 Param1            : 8;
    UINT8 Param2            : 8;
    UINT8 Reserved          : 7;
    UINT8 RunBusy           : 1;
  } Fields;
} OC_MAILBOX_INTERFACE;

typedef struct _OC_MAILBOX_FULL {
  UINT32               Data;
  OC_MAILBOX_INTERFACE Interface;
} OC_MAILBOX_FULL;

//
//  OC Mailbox completion codes
//
#define OC_MAILBOX_CC_SUCCESS              0
#define OC_MAILBOX_CC_OC_LOCKED            1
#define OC_MAILBOX_CC_INVALID_DOMAIN       2
#define OC_MAILBOX_CC_MAX_RATIO_EXCEEDED   3
#define OC_MAILBOX_CC_MAX_VOLTAGE_EXCEEDED 4
#define OC_MAILBOX_CC_OC_NOT_SUPPORTED     5

//
//  PCODE Mailbox Structures
//
typedef union _PCODE_MAILBOX_INTERFACE {
  UINT32 InterfaceData;
  struct {
    UINT32 Command  : 8;
    UINT32 Address  : 21;
    UINT32 Reserved : 2;
    UINT32 RunBusy  : 1;
  } Fields;
} PCODE_MAILBOX_INTERFACE;

typedef struct _PCODE_MAILBOX_FULL {
  PCODE_MAILBOX_INTERFACE Interface;
  UINT32                  Data;
} PCODE_MAILBOX_FULL;

//
//  Pcode Mailbox completion codes
//
#define PCODE_MAILBOX_CC_SUCCESS              0
#define PCODE_MAILBOX_CC_ILLEGAL_CMD          1
#define PCODE_MAILBOX_CC_TIMEOUT              2
#define PCODE_MAILBOX_CC_ILLEGAL_DATA         3
#define PCODE_MAILBOX_CC_RESERVED             4
#define PCODE_MAILBOX_CC_ILLEGAL_VR_ID        5
#define PCODE_MAILBOX_CC_VR_INTERFACE_LOCKED  6
#define PCODE_MAILBOX_CC_VR_ERROR             7

#define READ_PCH_POWER_LEVELS_CMD             0x8000000A

///
///  OC Library structures
///
typedef struct {
  UINT32 CommandData;
  UINT8  CommandCompletion : 8;
  UINT8  Param1            : 8;
  UINT8  Param2            : 8;
  UINT8  Reserved          : 7;
  UINT8  RunBusy           : 1;
} OC_LIBRARY_COMMAND;

typedef union _OC_MAILBOX_COMMAND {
  UINT32 InterfaceData;
  struct {
    UINT8 CommandCompletion : 8;
    UINT8 Param1            : 8;
    UINT8 Param2            : 8;
    UINT8 Reserved          : 7;
    UINT8 RunBusy           : 1;
  } Fields;
} OC_MAILBOX_COMMAND;

typedef struct _OC_MAILBOX_ITEM {
  UINT32             Data;
  OC_MAILBOX_COMMAND Interface;
} OC_MAILBOX_ITEM;

typedef struct {
  UINT8  MaxOcRatio;
  UINT8  VoltageTargetMode;
  UINT16 VoltageTarget;
  INT16  VoltageOffset;
} VOLTAGE_FREQUENCY_SETTINGS;

typedef struct {
  VOLTAGE_FREQUENCY_SETTINGS VfSettings;
  UINT8                      DomainId;
} VOLTAGE_FREQUENCY_ITEM;

typedef enum {
  IaCore,
  Gt,
  Clr,
  Uncore
} CPU_DOMAIN_ID;

typedef struct {
  UINT16  VoltageTarget;
  BOOLEAN SvidDisable;
} SVID_CONFIG_ITEM;

typedef struct {
  UINT8   MaxOcRatioLimit;
  BOOLEAN RatioOcSupported;
  BOOLEAN VoltageOverridesSupported;
  BOOLEAN VoltageOffsetSupported;
  UINT8   DomainId;
} OC_CAPABILITIES_ITEM;

typedef struct {
  UINT8 MaxOcRatioLimit1C;
  UINT8 MaxOcRatioLimit2C;
  UINT8 MaxOcRatioLimit3C;
  UINT8 MaxOcRatioLimit4C;
  UINT8 Index;
} CORE_RATIO_LIMITS_ITEM;

typedef struct {
  UINT8 DisableFivrFaults;
  UINT8 DisableFivrEfficiency;
} GLOBAL_CONFIG_ITEM;

typedef struct {
  UINT32 MaxOcRatio       : 8;
  UINT32 VoltageTargetU12 : 12;
  UINT32 TargetMode       : 1;
  UINT32 VoltageOffsetS11 : 11;
} VF_MAILBOX_COMMAND_DATA;

typedef union _AVX_DATA {
  UINT32 Data;
 struct {
	UINT32 IaSSE		:5;
	UINT32 Avx2			:5;
	UINT32 Avx3			:5;
	UINT32 Resrv		:17;
 }DataFields;
}AVX_DATA;

///
/// OC Library Function Prototypes
///
EFI_STATUS
EFIAPI GetVoltageFrequencyItem (
  OUT VOLTAGE_FREQUENCY_ITEM *VfSettings,
  OUT UINT32                 *LibStatus
  )
/**
  Gets the Voltage and Frequency information for a given CPU domain

  @param[OUT] *VfSettings
  @param[OUT] *LibStatus

  @retval EFI_STATUS
**/
;

EFI_STATUS
EFIAPI SetVoltageFrequencyItem (
  IN VOLTAGE_FREQUENCY_ITEM VfSettings,
  OUT UINT32                *LibStatus
  )
/**
  Sets the Voltage and Frequency information for a given CPU domain

  @param[IN]  *VfSettings
  @param[OUT] *LibStatus

  @retval EFI_STATUS
**/
;

EFI_STATUS
EFIAPI GetFivrConfig (
  OUT GLOBAL_CONFIG_ITEM *FivrConfig,
  OUT UINT32             *LibStatus
  )
/**
  Get the global FIVR Configuration information

  @param[OUT] *FivrConfig
  @param[OUT] *LibStatus

  @retval EFI_STATUS
**/
;

EFI_STATUS
EFIAPI SetFivrConfig (
  IN GLOBAL_CONFIG_ITEM FivrConfig,
  OUT UINT32            *LibStatus
  )
/**
  Set the Global FIVR Configuration information

  @param[IN]  FivrConfig
  @param[OUT] *LibStatus

  @retval EFI_STATUS
**/
;

EFI_STATUS
EFIAPI GetSvidConfig (
  OUT SVID_CONFIG_ITEM *SvidConfig,
  OUT UINT32           *LibStatus
  )
/**
  Get the SVID Configuration information

  @param[OUT] *SvidConfig
  @param[OUT] *LibStatus

  @retval EFI_STATUS
**/
;

EFI_STATUS
EFIAPI SetSvidConfig (
  IN SVID_CONFIG_ITEM SvidConfig,
  OUT UINT32          *LibStatus
  )
/**
  Set the SVID Configuration information

  @param[IN]  SvidConfig
  @param[OUT] *LibStatus

  @retval EFI_STATUS
**/
;

EFI_STATUS
EFIAPI GetOcCapabilities (
  OUT OC_CAPABILITIES_ITEM *OcCapabilities,
  OUT UINT32               *LibStatus
  )
/**
  Get the overclocking capabilities for a given CPU Domain

  @param[OUT] *OcCapabilities
  @param[OUT] *LibStatus

  @retval EFI_STATUS
**/
;

VOID
ConvertVoltageTarget (
  IN UINT16  InputVoltageTarget,
  OUT UINT16 *OutputVoltageTarget,
  IN UINT8   ConversionType
  )
/**
  Converts the input voltage target to the fixed point U12.2.10 Volt format or
  the Binary millivolts representation based on the ConversionType

@param[IN]  InputVoltageTarget
@param[OUT] *OutputVoltageTarget
@param[IN]  ConversionType - 0:fixed point, 1:Binary millivolts
**/
;

VOID
ConvertVoltageOffset (
  IN INT16  InputVoltageOffset,
  OUT INT16 *OutputVoltageOffset,
  IN UINT8  ConversionType
  )
/**
  Converts the input votlage Offset to the fixed point S11.0.10 Volt format or
  to Binary illivolts representation based on the ConversionType.

@param[IN]  InputVoltageTarget
@param[OUT] *OutputVoltageTarget
@param[IN]  ConversionType - 0:fixed point, 1:Signed Binary millivolts
**/
;

VOID
ConvertToMailboxFormat (
  IN VOID             *InputData,
  OUT OC_MAILBOX_ITEM *MailboxData,
  IN UINT32           CommandId
  )
/**
  Converts the input data to valid mailbox command format based on CommandID

@param[IN]  InputData
@param[OUT] *MailboxData
@param[IN]  CommandId
**/
;


EFI_STATUS
EFIAPI
MailboxWrite (
  IN UINT32  MailboxType,
  IN UINT32  MailboxCommand,
  IN UINT32  MailboxData,
  OUT UINT32 *MailboxStatus
  )
/**
  Generic Mailbox function for mailbox write commands. This function will
  poll the mailbox interface for control, issue the write request, poll
  for completion, and verify the write was succussful.

  @param[IN]  MailboxType,
  @param[IN]  MailboxCommand,
  @param[IN]  MailboxData,
  @param[OUT] *MailboxStatus

  @retval EFI_STATUS
**/
;

EFI_STATUS
EFIAPI
MailboxRead (
  IN UINT32  MailboxType,
  IN UINT32  MailboxCommand,
  OUT UINT32 *MailboxDataPtr,
  OUT UINT32 *MailboxStatus
  )
/**
  Generic Mailbox function for mailbox read commands. This function will write
  the read request, and populate the read results in the output data.

  @param[IN]  MailboxType,
  @param[IN]  MailboxCommand,
  @param[OUT] *MailboxDataPtr,
  @param[OUT] *MailboxStatus

  @retval EFI_STATUS
**/
;

EFI_STATUS
EFIAPI
PollMailboxReady (
  IN UINT32 MailboxType
  )
/**
  Poll the run/busy bit of the mailbox until available or timeout expires.

  @param[IN]  MailboxType,

  @retval EFI_STATUS
**/
;

/**
  Based on ResetType, perform warm or cold reset using PCH Reset PPI

  @param[in] PeiServices       - Indirect reference to the PEI Services Table.
  @param[in] ResetType         - CPU_RESET_TYPE to indicate which reset shoudl be performed.

  @exception EFI_UNSUPPORTED - Reset type unsupported
  @retval EFI_SUCCESS     - function successfully (system should already reset)
**/
EFI_STATUS
PerformWarmORColdReset (
  IN EFI_PEI_SERVICES CONST **PeiServices,
  IN CPU_RESET_TYPE         ResetType
  );


VOID
CpuVccInOverride (
  IN EFI_PEI_SERVICES CONST  **PeiServices,
  IN SYSTEM_CONFIGURATION    *SystemConfiguration
  );

VOID
VccioOverride (
  IN EFI_PEI_SERVICES CONST  **PeiServices,
  IN SYSTEM_CONFIGURATION    *SystemConfiguration
  );

VOID
VsmOverride (
  IN EFI_PEI_SERVICES CONST  **PeiServices,
  IN SYSTEM_CONFIGURATION    *SystemConfiguration
  );

EFI_STATUS
PerformBclkOcFlow (
  IN  EFI_PEI_SERVICES CONST  **PeiServices,
  IN  SYSTEM_CONFIGURATION    *SystemConfiguration
  );

EFI_STATUS
ProgramFilterPll (
  IN  EFI_PEI_SERVICES CONST  **PeiServices,
  IN  SYSTEM_CONFIGURATION    *SystemConfiguration
  );

EFI_STATUS
StartTimer (
  IN  EFI_PEI_SERVICES CONST  **PeiServices,
  IN  UINT32                  TimeoutValue
  );

EFI_STATUS RingMinRatio(UINT8 Ratio);
EFI_STATUS WriteTjMax(UINT8 TjMaxoffset);
UINT32 WriteVCUMailbox(UINT32 Command, UINT32 Data, UINT32 Indx);
EFI_STATUS AVXOffset (UINT8 AVX2Offset );

#endif
