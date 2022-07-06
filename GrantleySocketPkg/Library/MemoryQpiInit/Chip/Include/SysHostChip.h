//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license
// agreement with Intel or your vendor.  This file may be
// modified by the user, subject to additional terms of the
// license agreement
//
/*************************************************************************
 *
 * Reference Code
 *
 * ESS - Enterprise Silicon Software
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2006 - 2020 Intel Corporation All Rights Reserved.
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
 *
 ************************************************************************/

#ifndef _syshostchip_h
#define _syshostchip_h

//
// Moved from SysRegs.h. These #defines must remain in situ.
//
#define GRANTLEY_BIOS_HEADER_OVERRIDE 1
#define GRANGEVILLE_BIOS_HEADER_OVERRIDE  1

#include "CpuHost.h"
#include "MemHost.h"
#include "QpiHost.h"
#include "UsbDebugPort.h"

//
// -----------------------------------------------------------------------------
// Setup structures
//

//
// -------------------------------------
// Declarations and directives
// -------------------------------------
// Reference Code (RC) revision in BCD format:
// [31:24] = Major revision number
// [23:16] = Minor revision number
// [15:8] = Release Candidate number
//
#ifdef DE_SKU
#define RC_REVISION  0x02090000
#else
#define RC_REVISION  0x04030000
#endif

#define MRC_SYNC_CL  244071

//
// Steppings
//
#define A0_REV  0x00
#define B0_REV  0x01
#define C0_REV  0x05
#define C1_REV  0x06

//
// BDX Steppings
//
#define A0_REV_BDX  0x00  //DE->T0,ML->A0,A1
#define B0_REV_BDX  0x01  //DE->U0,ML->B0
#define C0_REV_BDX  0x02  //DE->V0,V1,ML->C0
#define D0_REV_BDX  0x03  //DE->V2,V3
#define Y0_REV_BDX  0x04  //DE->Y0,Y1
#define A0_REV_BDX_NS  0x05  //DE-NS->A0

//
// HSX Steppings
//
#define A0_REV_HSX  0x00
#define B0_REV_HSX  0x01
#define C0_REV_HSX  0x02
#define D0_REV_HSX  0xFF    // To be determined

//
// IVT Steppings
//
#define A0_REV_IVT  0x00
#define A1_REV_IVT  0x01
#define B0_REV_IVT  0x02
#define B3_REV_IVT  0x03
#define C0_REV_IVT  0x04
#define D0_REV_IVT  0x06

//
// Defines for socketType
//
#define SOCKET_EP   0
#define SOCKET_EN   1
#define SOCKET_HEDT 2
#define SOCKET_DE   3

#ifndef ASM_INC
#pragma pack(1)

//
// -----------------------------------------------------------------------------
// Nvram structures
//
#pragma pack(1)
struct commonNvram {
  UINT64_STRUCT         cpuFreq;
  UINT32                rcVersion;      // Version of the reference code last executed
  UINT8                 sbspSocketId;
  UINT8                 platformType;
  UINT32                platformRevId;
  TSEG_N0_IIO_VTD_STRUCT            TsegBase;
  TSEG_N1_IIO_VTD_STRUCT            TsegLimit;
  MESEG_BASE_N0_CBOBC_MEMSAD_STRUCT MeMemLowBaseAddr;
  MESEG_BASE_N1_CBOBC_MEMSAD_STRUCT MeMemHighBaseAddr;
  MESEG_LIMIT_N0_CBOBC_MEMSAD_HSX_BDX_STRUCT  MeMemLowLimit;
  MESEG_LIMIT_N1_CBOBC_MEMSAD_STRUCT  MeMemHighLimit;
  MENCMEM_BASE_0_IIO_VTD_STRUCT     MeNcMemLowBaseAddr;
  MENCMEM_BASE_1_IIO_VTD_STRUCT     MeNcMemHighBaseAddr;
  MENCMEM_LIMIT_0_IIO_VTD_STRUCT    MeNcMemLowLimit;
  MENCMEM_LIMIT_1_IIO_VTD_STRUCT    MeNcMemHighLimit;
};
#pragma pack()

//
// System previous boot error structure
//
#define MAX_PREV_BOOT_ERR_ENTRIES   15

#define OCLA_TOR_ENTRY_MIN        0
#define OCLA_TOR_ENTRY_MAX        0x10
#define OCLA_TOR_ENTRY_DEFAULT    1
#define OCLA_WAY_MIN              0
#define OCLA_WAY_MAX              20


#pragma pack()

//
// Chip specific section of struct sysRasSetup
//
#define SYSRASSETUP_CHIP     \

//
// Chip specific section of struct sysRASVar
//
#define SYSRASVAR_CHIP       \

//
// Chip specific section of struct commonVar
//
#define COMMONVAR_CHIP       \
  UINT8                     busUncore[MAX_SOCKET];  /* array for CPU Uncore bus#, indexed by NodeID */      \
  UINT8                     mctpEn;                 /* Enable PCI Mctp frames */                            \

//
// Chip specific section of struct sysVar
//
#define SYSVAR_CHIP       \
  QPI_HOST_OUT              qpi;     \

//
// Chip specific section of struct sysNvram
//
#define SYSNVRAM_CHIP     \
  QPI_HOST_NVRAM      qpi;           \

//
// Chip specific section of struct sysSetup
//
#define SYSSETUP_CHIP     \
  QPI_HOST_IN         qpi;           \

//
// Move to a separate header file
// -----------------------------------------------------------------------------
// BIOS to PCode Mailbox CSR Registers
//
#define BIOS_MAILBOX_DATA       BIOS_MAILBOX_DATA_PCU_FUN1_REG
#define BIOS_MAILBOX_DATA_MMIO  0x4015108c  // BIOS_MAILBOX_DATA_1_10_1_CFG_REG
#define BIOS_MAILBOX_INTERFACE  BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG
#define BIOS_MAILBOX_INTERFACE_MMIO  0x40151090 // BIOS_MAILBOX_INTERFACE_1_10_1_CFG_REG
//
// -----------------------------------------------------------------------------
// BIOS to PCode Mailbox Commands
//
#define MAILBOX_BIOS_CMD_ZERO                       0x00  // NOP
#define MAILBOX_BIOS_CMD_WRITE_PCS                  0x02  // Write Package Configuration Space
#define MAILBOX_BIOS_CMD_READ_PCS                   0x03  // Read Package Configuration Space
#define MAILBOX_BIOS_CMD_VR_INTERFACE               0x04  // New in HXS. SVID command for accessing VR regs.
  #define MAILBOX_BIOS_DATA_VR_REG_VENDOR_ID          0x00  //  Vendor ID
  #define MAILBOX_BIOS_DATA_VR_REG_PRODUCT_ID         0x01  //  Product ID
  #define MAILBOX_BIOS_DATA_VR_REG_PRODUCT_REV        0x02  //  Product Revision
  #define MAILBOX_BIOS_DATA_VR_REG_PRODUCT_DC         0x03  //  Product Date Code
  #define MAILBOX_BIOS_DATA_VR_REG_Lot_CODE           0x04  //  Lot Code
  #define MAILBOX_BIOS_DATA_VR_REG_PROTOCOL_ID        0x05  //  Protocol ID
  #define MAILBOX_BIOS_DATA_VR_REG_CAPABILITY         0x06  //  Capability
  #define MAILBOX_BIOS_VR_CMD_GET_REG                 0x07  //  GetRegs VR Command
#define MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG       0x05  // New in HSX. Returns the value from the MISC_CONFIG memory variable.
#define MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG      0x06  // New in HSX. HSX Changes: Adding chicken bits for bios to program
#define MAILBOX_BIOS_CMD_IO_SA_CONFIG               0x7C
#define MAILBOX_BIOS_CMD_GV_PTR_OVERIDE             0x7D  // New in IVT. Override the default pointer separation values for BGFs in the GV domain
  #define MAILBOX_BIOS_BGF_ADDR_QPI_R3QPI             0x00  //   QPI/R3QPI BGF
  #define MAILBOX_BIOS_BGF_ADDR_IIO_R2PCIE            0x01  //   IIO/R2PCIe BGF
  #define MAILBOX_BIOS_BGF_ADDR_IVT_HA_MC             0x02  //   HA/MC BGF for IVT
  #define MAILBOX_BIOS_BGF_ADDR_QPI2_R3QPI            0x03  //   QPI2/R3QPI BGF for IVT
  #define MAILBOX_BIOS_BGF_ADDR_HSX_HA_MC_133         0x02  //   HA/MC - 133MHz ref clk for HSX
  #define MAILBOX_BIOS_BGF_ADDR_HSX_HA_MC_100         0x03  //   HA/MC - 100Mhz ref clk for HSX
  #define MAILBOX_BIOS_BGF_ADDR_HSX_PCU_R3QPI         0x04  //   PCU/R3QPI BGF for HSX
#define MAILBOX_BIOS_CMD_MASK_FW_AGENT              0x7E  // New in IVT. BIOS can use this command to clear the FW agent link layer attribute in QPI parameter 2 on the neighboring socket
#define MAILBOX_BIOS_CMD_IIO_DISABLE                0x7F  // New in IVT. Disable unconnected PCIE/DMI links as well as clock gating entire IIO
#define MAILBOX_BIOS_CMD_VDD_RAMP                   0x80  // Triggers the Vdd ramp up sequence
  #define MAILBOX_BIOS_120V_RAMP_ENCODING_VR12        0xbf  // VR12 Ramp Value Encoding for 1.20v
  #define MAILBOX_BIOS_125V_RAMP_ENCODING_VR12        0xc9  // VR12 Ramp Value Encoding for 1.25v
  #define MAILBOX_BIOS_135V_RAMP_ENCODING_VR12        0xdd  // VR12 Ramp Value Encoding for 1.35v
  #define MAILBOX_BIOS_150V_RAMP_ENCODING_VR12        0xfb  // VR12 Ramp Value Encoding for 1.50v
  #define MAILBOX_BIOS_120V_RAMP_ENCODING_VR12_5      0x47  // VR12.5 Ramp Value Encoding for 1.20v
  #define MAILBOX_BIOS_125V_RAMP_ENCODING_VR12_5      0x4c  // VR12.5 Ramp Value Encoding for 1.25v
  #define MAILBOX_BIOS_135V_RAMP_ENCODING_VR12_5      0x56  // VR12.5 Ramp Value Encoding for 1.35v
  #define MAILBOX_BIOS_150V_RAMP_ENCODING_VR12_5      0x65  // VR12.5 Ramp Value Encoding for 1.50v
#define MAILBOX_BIOS_CMD_DIMM_VR_PHASE_SHED         0x81  // Triggers the DIMM VR Phase Shedding flow in PCODE
  #define MAILBOX_BIOS_DATA_PS0                       0x00  //   PS0: full-phase
  #define MAILBOX_BIOS_DATA_PS1                       0x01  //   PS1: single-phase, typically <20A load
  #define MAILBOX_BIOS_DATA_PS2                       0x02  //   PS2: fixed loss, typically <5A load
#define MAILBOX_BIOS_CMD_DRAM_PM_RESET_CTR          0x82  // Resets an energy counter for DRAM Power Meter
#define MAILBOX_BIOS_CMD_DRAM_PM_START_CTR          0x83  // Starts an energy counter for DRAM Power Meter
#define MAILBOX_BIOS_CMD_DRAM_PM_STOP_CTR           0x84  // Stops an energy counter for DRAM Power Meter
#define MAILBOX_BIOS_CMD_DRAM_PM_READ_CTR           0x85  // Reads an energy counter for DRAM Power Meter
#define MAILBOX_BIOS_CMD_DRAM_PM_WRITE_POWER_COEFF  0x86  // Writes power coefficients
#define MAILBOX_BIOS_CMD_ACTIVE_VR_MASK             0x87  // Acquire a mask of active VRs discovered by PCODE
#define MAILBOX_BIOS_CMD_SET_QPI_FREQ               0x88  // Set QPI frequency value for next reset
#define MAILBOX_BIOS_CMD_READ_QPI_FREQ              0x89  // Read QPI frequency value for next reset
#define MAILBOX_BIOS_CMD_WRITE_PWR_BW_SCALE         0x8A  // Program the DRAM pbm conversion from remaining energy to transaction count for bandwidth throttling.
#define MAILBOX_BIOS_WRITE_DRAM_PBM_THRT_ADJUST     0x8B  // Program the DRAM pbm factor
#define MAILBOX_BIOS_CMD_QUIESCE_PECI               0x8C  // Quiesce PECI
#define MAILBOX_BIOS_CMD_UNQUIESCE_PECI             0x8D  // Removed the blocks on PECI
#define MAILBOX_BIOS_CMD_ASSERT_CPU_ONLY_RESET      0x8F  // New for IVT. Used as the trigger mechanism for TSC synchronization of hot added/onlined sockets
#define MAILBOX_BIOS_CMD_READ_PCIE_FREQ             0x90  // Read the current PCIE frequency
#define MAILBOX_BIOS_CMD_PROGRAM_QPI_PORTS          0x91  // New in HSX. Identify Ports in L1 mode.  Value: Bit[0]=Port0, Bit[1]=Port1,..
#define MAILBOX_BIOS_CMD_DDR_RANKS_PRESENT          0x92  // DDR Ranks Present
#define MAILBOX_BIOS_CMD_READ_CPU_VR_INFO           0x93  // Read CPU VR Info
  #define MAILBOX_BIOS_DATA_CORE_VR_ADDR              0x00  //  VR Address for CPU core VR
  #define MAILBOX_BIOS_DATA_SA_VR_ADDR                0x01  //  VR Address for CPU SA VR
  #define MAILBOX_BIOS_DATA_DRAM0_VR_ADDR             0x02  //  VR Address for DRAM0 VR
  #define MAILBOX_BIOS_DATA_DRAM1_VR_ADDR             0x03  //  VR Address for DRAM1 VR
#define MAILBOX_BIOS_CMD_VR12_WA_ENABLE             0x94  // Read DRAM VR Info
  #define MAILBOX_BIOS_DATA_VR12WA_ENC_SET_VID_AUTOACK   0x00    // VR12 WA enable - WA encoding 0
  #define MAILBOX_BIOS_DATA_VR12WA_GVFSM_DISABLE_PREEMPT 0x01    // VR12 WA enable - WA encoding 1
  #define MAILBOX_BIOS_DATA_VR12WA_ENC_MORE_THAN_10MV    0x02    // VR12 WA enable - WA encoding 2
  #define MAILBOX_BIOS_DATA_VR12WA_ENC_DISABLE_SA_ITD    0x03    // VR12 WA enable - WA encoding 3
  #define MAILBOX_BIOS_DATA_VR12WA_ENC_IDLE_CLK_ON       0x04    // VR12 WA enable - WA encoding 4
#define MAILBOX_BIOS_CMD_DRAM_RAPL_MODE             0x96  // DRAM RAPL Mode
#define MAILBOX_BIOS_CMD_DRAM_PM_CHN_VR_MAP         0x97  // DRAM PM Chn VR Map
#define MAILBOX_BIOS_CMD_PROBE_DRAM_VR              0x98  // New in IVT. Request that Pcode re-poll the DRAM VRs
#define MAILBOX_BIOS_CMD_RAS_START                  0x9A  // New in IVT. Stop deep C state (non C0 state) during RAS events
#define MAILBOX_BIOS_CMD_RAS_EXIT                   0x9B  // New in IVT. Re-start C state (no clipping)  RAS events
#define MAILBOX_BIOS_CMD_QPI_DISABLE                0x9C  // New in HSX. Request Pcode to turn off FIVRs on disabled QPI ports
#define MAILBOX_BIOS_CMD_FAST_RAPL_DUTY_CYCLE       0x9D  // New in HSX. Bios programs the Nstrike PL2 duty cycle for fast rapl. Pcode will clip the data to a programmable range.
#define MAILBOX_BIOS_CMD_B273548_WA_CHANNELS        0xA0  // New in HSX. ZQCal workaround for 3DPC DDR4 LRDIMM config.
#define MAILBOX_BIOS_CMD_CLR_RING                   0xA4  // CLR ring ratio
#ifdef DE_SKU
#define MAILBOX_BIOS_CMD_WRITE_PCH_TEMP             0xB0  //New for BDX-DE that provides TJMAX and THROTTLE_TEMP for PCH.
#define MAILBOX_BIOS_CMD_READ_PMC_LAN_PORT_SEL      0xB1  // BDX-DE , PMC_LAN_PORT PCI read
#define MAILBOX_BIOS_CMD_WRITE_PMC_LAN_PORT_SEL     0xB2  // BDX-DE , PMC_LAN_PORT PCI write
#define MAILBOX_BIOS_CMD_SOFTWARE_ADR_TRIGGER       0xB3  // BDX-DE , Trigger Software ADR
#define MAILBOX_BIOS_CMD_ADR_IIO_BLOCK              0xB5 //  BDX-DE , Disable SC traffic on ADR
#endif // DE_SKU

//
// VR Addresses
//
#define MAILBOX_BIOS_CORE_VR_ADDR               0  // Core  VR Addr encoded with the SVID VR address
#define MAILBOX_BIOS_SA_VR_ADDR                 1  // SA    VR Addr encoded with the SVID VR address
#define MAILBOX_BIOS_DRAM0_VR_ADDR              2  // DRAM0 VR Addr encoded with the SVID VR address
#define MAILBOX_BIOS_DRAM1_VR_ADDR              4  // DRAM1 VR Addr encoded with the SVID VR address
#define MAILBOX_BIOS_DRAM2_VR_ADDR              6  // DRAM2 VR Addr encoded with the SVID VR address
#define MAILBOX_BIOS_DRAM3_VR_ADDR              8  // DRAM3 VR Addr encoded with the SVID VR address

//
// VR Error Codes
//
#define MAILBOX_BIOS_ERROR_CODE_NO_ERROR               0x00
#define MAILBOX_BIOS_ERROR_CODE_INVALID_COMMAND        0x01
#define MAILBOX_BIOS_ERROR_CODE_BAD_VR_ADDR            0x02
#define MAILBOX_BIOS_ERROR_CODE_SVID_ERROR             0x03
#define MAILBOX_BIOS_ERROR_CODE_UNSUPPORTED_QPI_FREQ   0x04
#define MAILBOX_BIOS_ERROR_CODE_UNSUPPORTED_IIO_FREQ   0x05
#define MAILBOX_BIOS_ERROR_CODE_BAD_DIMM_INDEX         0x06
#define MAILBOX_BIOS_ERROR_CODE_BAD_BW_SCALE           0x07
#define MAILBOX_BIOS_ERROR_CODE_COUNTER_NOT_ENABLED    0x08
#define MAILBOX_BIOS_ERROR_CODE_BAD_CHANNEL_INDEX      0x09
#define MAILBOX_BIOS_ERROR_CODE_BAD_RANK_INDEX         0x0a
#define MAILBOX_BIOS_ERROR_CODE_INVALID_VR_WA_CMD      0x0b
#define MAILBOX_BIOS_ERROR_CODE_VR_REG_READ_ERROR      0x0c

//
// VR Voltage Modes
//
#define VR_MODE_12          1
#define VR_MODE_12_5        2


//
// -----------------------------------------------------------------------------
// BIOS to VCode Mailbox Commands
//

#define VCODE_API_OPCODE_NOP  0x0
#define VCODE_API_OPCODE_OPEN_SECURE_SEQUENCE  0x18
#define VCODE_API_OPCODE_OPEN_SEQUENCE  0x2
#define VCODE_API_OPCODE_SET_PARAMETER  0x6
#define VCODE_API_OPCODE_CLOSE_SECURE_SEQUENCE  0x19
#define VCODE_API_OPCODE_CLOSE_SEQUENCE  0x3
#define VCODE_API_OPCODE_CPU_STATUS  0x1000
#define VCODE_API_OPCODE_IOT_LLC_SETUP  0x100f
#define VCODE_API_OPCODE_IOT_REG_READ  0x1010
#define VCODE_API_OPCODE_IOT_REG_WRITE  0x1011
#define VCODE_API_OPCODE_IOT_SETUP  0x100c


#define VCODE_API_SEQ_ID_IOT_LLC_SETUP  0x1000a
#define VCODE_API_SEQ_ID_OPEN_SEQUENCE  0x0

#define VCODE_API_SEQ_ID_IOT_TRACE_DUMP_SETUP  0x10010
#define VCODE_API_OPCODE_IOT_TRACE_DUMP_SETUP  0x1014

// command return code
#define VCODE_MAILBOX_CC_SUCCESS             0x40     // Successful completion
#define VCODE_MAILBOX_CC_TIMEOUT             0x80     // Time-out.
#define VCODE_MAILBOX_CC_THREAD_UNAVAILABLE    0x82     // Thread Unavailable.
#define VCODE_MAILBOX_CC_ILLEGAL             0x90     // Illegal.

#define VCODE_MAILBOX_PECI_CC_SUCCESS           0x40     // Successful completion
#define VCODE_MAILBOX_PECI_CC_ILLEGAL           0x90     // Illegal

#define BIOS_VCU_MAILBOX_TIMEOUT_RETRY           10      // Retry count used by BIOS


// Move to a separate header file
// Scratch register usage
//
#define CSR_EMULATION_FLAG_1_11_0_CFG_REG 0x401580b0  // register at B:D:F: 1:11:0, offset 0xB0. with SoftSimulation flag
// -----------------------------------------------------------------------------
// Sticky Scratch Pad regsiter usage
//  B:D:F:  n:11:3: offset 0x40, 0x44, ...,  total 8 registers
//
#define SR_WARMBOOT_MMCFG_DATA_CSR  BIOSSCRATCHPAD7_UBOX_MISC_REG

//
// -----------------------------------------------------------------------------
// Non-Sticky Scratch Pad regsiter usage
//  B:D:F:  n:11:3: offset 0x60, 0x64, ...,  total 16 registers

//
// CSR for maintaining a pointer to the Compatible BIOS Data structure.
//
#ifdef  BDAT_SUPPORT
#define SR_BDAT_STRUCT_PTR_CSR                    (BIOSNONSTICKYSCRATCHPAD5_UBOX_MISC_REG)   // UBOX scratchpad CSR5
#define SR_BDAT_STRUCT_PTR_CSR_MMIO               0x4015b074  // BIOSNONSTICKYSCRATCHPAD5_1_11_3_CFG_REG
#endif //BDAT_SUPPORT

//
// CSRs for PIPE/SOCKET/MRC MILESTONES
//
#define SR_PBSP_CHECKIN_CSR                       (BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG)   // UBOX scratchpad CSR02
#define SR_BIOS_SERIAL_DEBUG_CSR                  (BIOSSCRATCHPAD6_UBOX_MISC_REG)            // UBOX scratchpad CSR6
#define SR_POST_CODE_CSR                          (BIOSNONSTICKYSCRATCHPAD7_UBOX_MISC_REG)   // UBOX scratchpad CSR7
#define SR_ERROR_CODE_CSR                         (BIOSNONSTICKYSCRATCHPAD8_UBOX_MISC_REG)   // UBOX scratchpad CSR8
#define SR_PRINTF_SYNC_CSR                        (BIOSNONSTICKYSCRATCHPAD9_UBOX_MISC_REG)   // UBOX scratchpad CSR9
#define SR_RC_REVISION_CSR                        (BIOSNONSTICKYSCRATCHPAD12_UBOX_MISC_REG)  // UBOX scratchpad CSR12
#define PIPE_DISPATCH_SYNCH_PSYSHOST  4
#define SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR  (BIOSNONSTICKYSCRATCHPAD13_UBOX_MISC_REG)  // UBOX scratchpad CSR13
#define SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR   (BIOSNONSTICKYSCRATCHPAD14_UBOX_MISC_REG)  // UBOX scratchpad CSR14
#define SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR      (BIOSNONSTICKYSCRATCHPAD15_UBOX_MISC_REG)  // UBOX scratchpad CSR15

//Register location from base register
#define SR_POST_CODE                               7 //SR_POST_CODE_CSR
#define SR_BIOS_SERIAL_DEBUG                       6 //SR_BIOS_SERIAL_DEBUG_CSR


//
// bit definitions for the PCU_BIOS_SPARE2 register
//
#define JKT_PCU_BIOS_SPARE2_UDIMM                 BIT14
#define JKT_PCU_BIOS_SPARE2_LV_RDIMM              BIT13
#define HSX_PCU_BIOS_SPARE2_UDIMM                 0
#define HSX_PCU_BIOS_SPARE2_RDIMM                 BIT12
#define HSX_PCU_BIOS_SPARE2_LRDIMM                BIT13
#define HSX_PCU_BIOS_SPARE2_125V_DIMM             0
#define HSX_PCU_BIOS_SPARE2_135V_DIMM             BIT14
#define HSX_PCU_BIOS_SPARE2_150V_DIMM             BIT15

#endif // ASM_INC

#endif // _syshostchip_h
