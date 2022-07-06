//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c)  1999 - 2016 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  Platform.h

Abstract:

  Platform specific information

--*/
 //AptioV server override start: Program GPIO, PMBASE and SMBUS base address from SDL values                            
#include <Token.h>
 //AptioV server override end: Program GPIO, PMBASE and SMBUS base address from SDL values                            
#include "BackCompatible.h"
#include "Uefi.h"
#include "Guid/SetupVariable.h"
#include "UncoreCommonIncludes.h"

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#define EFI_VARIABLE_RUNTIME_ALLOW_RECLAIM 0x00001000

//#define EFI_FIRMWARE_VENDOR L"Intel Corporation"

//
// MMIO space allocation
// 0x00000000 -- 0xC0000000 (TOLM)
// 0xC0000000 -- 0xD0000000 (PCIe map  space)
// 0xD0000000 -- 0xFE000000 (PCI space)
//
//
// Onboard / Integrated Graphics Device
//

#define IGD_VID                         0x102B  // Matrox
#define IGD_DID                         0x0522  // iBMC Video

#define ATS_VID                         0x1A03  // ATS 
#define ATS_DID                         0x2000  // Video

#define VIDEO_DEV_NUM                   0x00
#define VIDEO_FUNC_NUM                  0x00
#define VIDEO_ROOTPORT_DEV_NUM          PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS
#define VIDEO_ROOTPORT_FUNC_NUM         PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_5

//
// Onboard NIC device's root port info; the IO riser on SR has two NICs each supporting two LAN ports.
//
#define SR_NIC0_RP_DEV_NUM              PCIE_PORT_1_DEV
#define SR_NIC0_RP_FUNC_NUM             PCIE_PORT_1A_FUNC
#define SR_NIC1_RP_DEV_NUM              PCIE_PORT_1_DEV
#define SR_NIC1_RP_FUNC_NUM             PCIE_PORT_1B_FUNC

//
// 82576 (Kawela) & 82598 (Oplin) NIC Device IDs
//
#define NIC_82598EB_XFSR_DUAL_DIDVID    0x10C68086
#define NIC_82598EB_XFSR_SNGL_DIDVID    0x10C78086
#define NIC_82598EB_AT_SNGL_DIDVID      0x10C88086
#define NIC_82598EB_CX4_DUAL_DIDVID     0x10EC8086
#define NIC_82598EB_AFDA_DUAL_DIDVID    0x10F18086
#define NIC_82598EB_XFLR_SNGL_DIDVID    0x10F48086
#define NIC_82576_ET_DUAL_DIDVID        0x10C98086
#define NIC_82576_EF_DUAL_DIDVID        0x10E68086

//
// HECI BAR space, 16B for each HECI interface. However, a limitation in the
// hardware cause that not all valid addresses actualy work for HECI MBARs.
// The below defined addresses have been tested and work.
// NOTE: The HECI2_BASE_ADDRESS value is used in the me.asi for HEC2 object.
//
// the address that worked for Tylersburd doesnt work for Boxboro-EX
//#define HECI1_BASE_ADDRESS              0xFE900000 // HECI-1
//#define HECI2_BASE_ADDRESS              0xFEA00000 // HECI-2
// the address that worked for Tylersburd doesnt work for Boxboro-EX, below are recommended by ME SV folks
#define HECI1_BASE_ADDRESS              0xFED12000 // HECI-1 
#define HECI2_BASE_ADDRESS              0xFED12010 // HECI-2

//
// Number of P & T states supported
//
#define NPTM_P_STATES_SUPPORTED         16
#define NPTM_T_STATES_SUPPORTED         0

//
// IO APICs
//


#define PCH_IOAPIC                      (1 << 0)
#define PCH_IOAPIC_ID                   0x08
#define PCH_IOAPIC_ADDRESS              0xFEC00000 // This must get range from Legacy IIO

#define IIO0_IOAPIC                      (1 << 1)
#define IIO0_IOAPIC_ID                   0x09
#define IIO0_IOAPIC_ADDRESS              0xFEC01000

#define IIO1_IOAPIC                     (1 << 2)
#define IIO1_IOAPIC_ID                  0x0A
#define IIO1_IOAPIC_ADDRESS             0xFEC40000

#define IIO2_IOAPIC                     (1 << 3)
#define IIO2_IOAPIC_ID                  0x0B
#define IIO2_IOAPIC_ADDRESS             0xFEC80000

#define IIO3_IOAPIC                     (1 << 4)
#define IIO3_IOAPIC_ID                  0x0C
#define IIO3_IOAPIC_ADDRESS             0xFECC0000

//
// Define platform base
// Note: All the PCH devices must get Legacy IO resources within first 16KB 
// since QPIRC allcoates range 0-16KB for the legacy IIO.
//
#define BMC_KCS_BASE_ADDRESS            0x0CA0
#define BMC_KCS_BASE_MASK               0xFFF0
#define PCH_ACPI_BASE_ADDRESS           0x0400
//AptioV server override start:
// commented to avoid redefinition error from token.h
//#define PM_BASE_ADDRESS                 PCH_ACPI_BASE_ADDRESS
 //AptioV server override end
#define PCH_ACPI_TIMER_ADDRESS          (PCH_ACPI_BASE_ADDRESS + 0x08)
#define PCH_ACPI_TIMER_MAX_VALUE        0x1000000   // The timer is 24 bit overflow
 //AptioV server override start: Program GPIO, PMBASE and SMBUS base address from SDL values     
//#define PCH_GPIO_BASE_ADDRESS           0x0500
#define PCH_GPIO_BASE_ADDRESS           GPBS
//#define PCH_RCBA_ADDRESS                0xFED1C000  // PCH Root complex base address
#ifndef PCH_RCBA_ADDRESS
#define PCH_RCBA_ADDRESS                SB_RCBA  // PCH Root complex base address
#endif
//AptioV server override end: Program GPIO, PMBASE and SMBUS base address from SDL values     

#define PCH_TBARB_ADDRESS               0xFED08000  // 4K for Pch Thermal Sensor
#define HPET_BASE_ADDRESS	             0xFED00000
#define PCH_IDE_BASE_ADDRESS            0x0580
#define SIO_BASE_ADDRESS                0x0680
#define SIO_BASE_MASK                   0xFFF0
// Aptio V Server Override Start
// Changed to avoid redefinition error
//#define SIO_GPIO_BASE_ADDRESS           0x0800
#define SIO_GPIO_BASE_ADDRESS			0xb00
// Aptio V Server Override End
#define PILOTII_MAILBOX_BASE_ADDRESS    0x0600
#define PILOTII_MAILBOX_MASK            0xFFE0

//
// Local and I/O APIC addresses.
//
#define EFI_APIC_LOCAL_ADDRESS          0xFEE00000
#define LOCAL_APIC_ADDRESS              0xFEE00000

//
// Constant definition
//
#define  MAX_SMRAM_RANGES    4
//
// This structure stores the base and size of the ACPI reserved memory used when
// resuming from S3.  This region must be allocated by the platform code.
//
typedef struct {
  UINT32  AcpiReservedMemoryBase;
  UINT32  AcpiReservedMemorySize;
  UINT32  SystemMemoryLength;
} RESERVED_ACPI_S3_RANGE;

#define RESERVED_ACPI_S3_RANGE_OFFSET (EFI_PAGE_SIZE - sizeof (RESERVED_ACPI_S3_RANGE))

//
// SMBUS Data
//
#define PCH_SMBUS_BUS_DEV_FUNC          0x1F0300
 //AptioV server override start: Program GPIO, PMBASE and SMBUS base address from SDL values                            
//#define PCH_SMBUS_BASE_ADDRESS          0x0780
#define PCH_SMBUS_BASE_ADDRESS          SMBS
//AptioV server override end: Program GPIO, PMBASE and SMBUS base address from SDL values                            
#define PLATFORM_NUM_SMBUS_RSVD_ADDRESSES   4
#define SMBUS_ADDR_CH_A_1                   0xA0
#define SMBUS_ADDR_CH_A_2                   0xA2
#define SMBUS_ADDR_CH_B_1                   0xA4
#define SMBUS_ADDR_CH_B_2                   0xA6

//
// CMOS usage
//
#define CMOS_CPU_BSP_SELECT             0x10
#define CMOS_CPU_UP_MODE                0x11
#define CMOS_CPU_RATIO_OFFSET           0x12

#define CMOS_CPU_CORE_HT_OFFSET         0x13
#define CMOS_CPU1_CORE_HT_OFFSET        0x14
#define CMOS_CPU2_CORE_HT_OFFSET        0x15
#define CMOS_CPU3_CORE_HT_OFFSET        0x16

#define CMOS_CPU0_CORE_DISABLE       	 0x1B            //Processor0Core Offset
#define CMOS_CPU1_CORE_DISABLE           0x1C            //Processor1Core Offset
#define CMOS_CPU2_CORE_DISABLE           0x1D            //Processor2Core Offset
#define CMOS_CPU3_CORE_DISABLE           0x1E            //Processor3Core Offset
#define CMOS_CPUx_CORE8_9_DISABLE        0x1F            //Processor[i]Core[9:8] Offset, Proc[i].Core[9:8] at this CMOS bits[x+1:x], where x = i * 2

// Second bank
//
#define CMOS_CPU_HT_OFFSET              0x14             //Processor Hyperthreading Offset
#define CMOS_CPU_BIST_OFFSET            0x15
#define CMOS_CPU_VMX_OFFSET             0x16
#define CMOS_EFI_DEBUG                  0x17
#define CMOS_PLATFORM_ID_LO             0x18            // Second bank CMOS location of Platform ID
#define CMOS_PLATFORM_ID_HI             0x19            //
#define CMOS_PCH_PORT80_OFFSET          0x1A
#define CMOS_WHEA_OS_ENABLE             0x1B            // Must match WIWH definition in Whea.asi
#define TCG_CMOS_AREA_OFFSET            0x60                          // Also Change in Universal\Security\Tpm\PhysicalPresence\Dxe\PhysicalPresence.c &
                                                                      // PlatformPkg AcpiTables\Dsdt\Tpm.asi
#define TCG_CMOS_MOR_AREA_OFFSET        (TCG_CMOS_AREA_OFFSET + 0x05) // Also Change in // PlatformPkg AcpiTables\Dsdt\Tpm.asi

#define CMOS_DEBUG_PRINT_LEVEL_REG      0x4C             // Debug Mask saved in CMOS
#define CMOS_DEBUG_PRINT_LEVEL_1_REG    0x4D
#define CMOS_DEBUG_PRINT_LEVEL_2_REG    0x4E
#define CMOS_DEBUG_PRINT_LEVEL_3_REG    0x4F


#define CMOS_WARM_RESET_COUNTER_OFFSET       0xBD    // 1 byte CMOS Space for passing warm reset counter to Dxe
                                                     //   due to reset in MRC Dxe always thinks that warm reset occurs
                                                     //   counter > 1 -> means WarmReset
#define CMOS_DRAM_SHARED_MAILBOX_ADDR_REG    0xF0    // 2 bytes CMOS Space for the DRAM Share Mailbox address [31:16]

//
// GPIO Index Data Structure
//
typedef struct {
  UINT32  Value;
  UINT8   Register;
} PCH_GPIO_INDEX_DATA;

//
// Memory information display for setup
//
typedef struct memInfoSetup {
  UINT16  MemFreq;  // in MHz
  UINT32  TotalMem; // in MB
};

//
// Define platform type register
// feature space location for GPI register in FWH
//
#define FWH_GPI_BASE_ADDRESS  0xFFBC0100
#define PLATFORM_TYPE_REG     (*(UINT8 volatile *) FWH_GPI_BASE_ADDRESS)

//
// COM (Serial) Port Base address
//
#define  COM1_BASE                      0x3F8
#define  COM2_BASE                      0x2F8

//
// LPT (Parallel) Port Base address
//
#define  LPT1_BASE                      0x378


#ifndef MAX_DIMM
#define MAX_DIMM                        3
#endif
#define MEM64_LEN                       0x00100000000
#define RES_MEM64_39_BASE               0x08000000000 - MEM64_LEN   // 2^39
#define RES_MEM64_39_LIMIT              0x08000000000 - 1           // 2^39
#define RES_MEM64_40_BASE               0x10000000000 - MEM64_LEN   // 2^40
#define RES_MEM64_40_LIMIT              0x10000000000 - 1           // 2^40

//
// Misc. platform definitions
//
#define CPU_GENERIC_UNCORE_DEV_FUNC     0
#define PCI_REVISION_ID_REG             8

//
// Need min. of 24 MB PEI phase
//
#define PEI_MIN_MEMORY_SIZE             (EFI_PHYSICAL_ADDRESS) ((300 * 0x100000))

#define HPET_BLOCK_ADDRESS                            0x0FED00000
#define PCI_BUS_NUMBER_PCH_HPET                       0xF0
#define PCI_DEVICE_NUMBER_PCH_HPET                    0x0F
#define PCI_FUNCTION_NUMBER_PCH_HPET0                 0x00
#define PCI_FUNCTION_NUMBER_PCH_HPET1                 0x01
#define PCI_FUNCTION_NUMBER_PCH_HPET2                 0x02
#define PCI_FUNCTION_NUMBER_PCH_HPET3                 0x03
#define PCI_FUNCTION_NUMBER_PCH_HPET4                 0x04
#define PCI_FUNCTION_NUMBER_PCH_HPET5                 0x05
#define PCI_FUNCTION_NUMBER_PCH_HPET6                 0x06
#define PCI_FUNCTION_NUMBER_PCH_HPET7                 0x07
#define R_PCH_LPC_H0BDF                               0x70
#define R_PCH_LPC_H1BDF                               0x72
#define R_PCH_LPC_H2BDF                               0x74
#define R_PCH_LPC_H3BDF                               0x76
#define R_PCH_LPC_H4BDF                               0x78
#define R_PCH_LPC_H5BDF                               0x7A
#define R_PCH_LPC_H6BDF                               0x7C
#define R_PCH_LPC_H7BDF                               0x7E

#define PCI_BUS_NUMBER_PCH_IOAPIC                     0xF0
#define PCI_DEVICE_NUMBER_PCH_IOAPIC                  0x1F
#define PCI_FUNCTION_NUMBER_PCH_IOAPIC                0x07

#define PXE_BASE_OPTION_ROM_FILE_GUID \
  { \
    0x6f2bc426, 0x8d83, 0x4f17, 0xbe, 0x2c, 0x6b, 0x1f, 0xcb, 0xd1, 0x4c, 0x80 \
  }

//
// LT Equates
//
#ifdef LT_FLAG
  #define ACM_BASE                      ACM_BIN_BASE
  #define ACM_SIZE                      0x00020000
  #define LT_PLATFORM_DPR_SIZE          0x00600000
  #define CMOS_LTSX_OFFSET              0x2A
#endif

//EDK2_TODO Need to check with CCB to see if this can be moved to PiBootmode.h
#define BOOT_WITH_MFG_MODE_SETTINGS                   0x07


//
// Equates for SRP Platform
//
// Signal                  UsedAs                       USE     I/O      LVL     INV USE:IO:LVL
// ---------------------------------------------------------------------------------
// BMBUSY#/GPIO0           IRQ_SSB_SCI_WHEA_N           G       I        -       -   1  1
// TACH1/GPIO1             FM_TACH1                     N       I        -       -   0  1
// PIRQE#/GPIO2            FM_ERR0_N                    G       I        -       -   1  1
// PIRQF#/GPIO3            FM_ERR1_N                    G       I        -       -   1  1
// TOTAL                                                                             D  F  0

// PIRQG#/GPIO4            FM_BMC_PCH_SMI_LPC_N        	G       I        -       -   1 1
// PIRQH#/GPIO5            FM_CATERR_N                  G       I        -       -   1 1
// TACH2/GPIO6             FM_TACH2                     N       I        -       -   0 1
// TACH3/GPIO7             FM_TACH3                     N       I        -       -   0 1
// TOTAL                                                                             3 F 0

// GPIO8                   IRQ_BMC_PCH_NMI              G       I        -       -   1 1
// USB_OC#5/GPIO9          OC#5                         N       -        -       -   0 1
// USB_OC#6/GPIO10         OC#6                         N       -        -       -   0 1
// SMB_ALERT#/GPIO11       FM_CPU_PCIE_SLOT_RST_N       G       O        H       -   1 0
// TOTAL                                                                             9 7 8

// LANPHYPC/GPIO12         FM_LINK_WIDTH_ID0            G       I        -       -   1 1
// DOCK_AZ_RST#/GPIO13     FM_BMC_PCH_SCI_LPC_N         G       I        -       -   1 1
// USB_OC#7/GPIO14         FM_LINK_WIDTH_ID4            G       I        -       -   1 1
// GPIO15                  FM_PCH_SATA_RAID_KEY         G       I        -       -   1 1
// TOTAL                                                                             F F 0

// SATA4GP/GP16/MGPIO9     FM_THROTTLE_N                G       O        H       -   1 0
// Tach0/GPIO17            FM_TACH0                     N       I        -       -   0 1
// SRC1CLKRQ#/GP18         FM_PCIE_MUX_0                G       I        -       -   1 1
// SATA1GP/GPIO19          FM_PCH_PCIE_SLOT_RST_N       G       O        H       -   0 0
// TOTAL                                                                             5 6 9

// SRC2CLKRQ#/GP20/SMI#    FM_SMI_ACTIVE_N              G       O        H       -   1 0
// SATA0GP/GPIO21          FM_PCIE_MUX_1              	G       I        -       -   1 I
// SCLOCK/GPIO22           SCLOCK                       N       -        -       -   0 1
// LDRQ1#/GPIO23           LDRQ1#                       N       I        -       -   0 1
// TOTAL                                                                             3 E 1

// GP24/MGPIO0             FM_PCH_DYN_SKU_KEY           G       I        -       -   1 1
// SRC3CLKRQ#/GPIO25       FM_PLD_JTAG_TDI              G       O        H       -   1 0
// S4_STATE#/GPIO26        PLD_JTAG_TCK                 G       O        H       -   1 0
// GPIO27/MGPIO6           FM_GPIO_EXP_INT_N            G       I        -       -   1 1
// TOTAL                                                                             F 9 6

// GPIO28/MGPIO7           FM_MIC_MUX_RST_N             G       O        H       -   1 0
// SLP_LAN#/GPIO29/MGPIO3  FM_BIOS_IMAGE_SWAP_N         G       I        -       -   1 1
// SUSWARN#/SUSPWRDNACK
// GPIO30/MGPIO1           FM_EUP_LOT6                  G       O        H       -   1 0
// ACPRESENT/GP31/MGPIO2   SML1ALERT_N                  G       I        -       -   1 1
//                                                                                   F A 5
// TOTAL                                                                      FF35F93D A9E6F7FF 56190800
// Registers affected by this group of signals (GPIO[63:32])are:
// GPIO_USE_SEL[63:32] offset 0x30
// GPIO_IO_SEL [63:32] offset 0x34
// GPIO_LVL    [63:32] offset 0x38
// Signal                  UsedAs                       USE     I/O      LVL     INV
// ---------------------------------------------------------------------------------
// CLKRUN#/GPIO32          FM_BIOS_POST_CMPTL_N         G       O        H       -   1 0
// HDA_DOCK_EN#/GPIO33     FM_QPI_SLOW_MODE_N          	G       I        -       -   1 1
// STPPCI#/GPIO34          FP_PWR_LED_N            	    G       O        H       -   1 0
// SATACLKREQ#/GPIO35/NMI# FM_NMI_EVENT                 G       O        L       -   1 0
//TOTAL                                                                              F 2  4

// SATA2GP/GPIO36          FM_BIOS_ADV_FUNCTIONS        G       I        -       -   1 1
// SATA3GP/GPIO37          FM_ADR_TRIGGER_N             G       I        -       -   1 1
// SLOAD/GPIO38            SLOAD                        N       -        -       -   0 1
// SDATAOUT[0]/GPIO39      SDATAOUT[0]                  N       -        -       -   0 1
// TOTAL                                                                             3 F  0

// OC1#/GPIO40             OC#1                         N       -        -       -   0 0
// OC2#/GPIO41             OC#2                         N       -        -       -   0 0
// OC3#/GPIO42             OC#3                         N       -        -       -   0 0
// OC4#/GPIO43             OC#4                         N       -        -       -   0 0
//TOTAL                                                                              0 0  0

// SRC5CLKRQ#/GPIO44       FM_LAN0_DISABLE_N            G       0        H       -   1 0
// SRC6CLKRQ#/GPIO45       FM_LAN1_DISABLE_N            G       0        H       -   1 0
// SRC7CLKRQ#/GPIO46       FM_LAN2_DISABLE_N            G       0        H       -   1 0
// PEGA_CLKRQ#/GPIO47      FM_LAN3_DISABLE_N            G       0        H       -   1 0
//TOTAL                                                                              F 0 F

// SDATAOUT1/GPIO48        SDATAOUT[1]                  N       -        -       -   0 1
// SATA5GP/GPIO49/MGPIO10  FM_CPU_PROCHOT_N             G       I        -       -   1 1
// REQ1#/GPIO50            GSXCLK                       N       -        -       -   0 1
// GNT1#/GPIO51            GSXDOUT                      N       -        -       -   0 1
//TOTAL                                                                              2 F 0

// REQ2#/GPIO52            GSX_SLOAD                    N       -        -       -   0 1
// GNT2#/GPIO53            GSXDIN                       N       -        -       -   0 1
// REQ3#/GPIO54            GSXRESET                     N       -        -       -   0 1
// GNT3#/GPIO55            FM_BIOS_RCVR_BOOT_N          G       I        -       -   1 1
//TOTAL                                                                              8 F 0

// PEGB_CLKRQ#/GPIO56      FM_PLD_JTAG_TDO              G       I        H       -   1 1
// GPIO57/MGPIO5           FM_ME_RCVR_N                 G       I        -       -   1 1
// ML1CLK/GP58/MGPIO11     SML1CLK                      N       -        -       -   0 1
// OC0#/GPIO59             OC[0]#                       N       -        -       -   0 1
//TOTAL                                                                              3 F 1

// SML0ALERT#/GPIO60/MGPIO4SML0_ALERT_N                 N       -        -       -   0 1
// SUS_STAT#/GPIO61        SUS_STAT_N                   G       O        H       -   1 0 
// SUSCLK/GPIO62           CLK_33K_SUSCLK               N       -        -       -   0 1
// SLP_S5#/GPIO63          SLP_S5#                     	N       -        -       -   0 1
// TOTAL                                                                             2 D 2
// TOTAL                                                                      2382F03F DFFF00F2 2100F004
// Registers affected by this group of signals (GPIO[75:64])are:
// GPIO_USE_SEL[95:64] offset 0x40
// GPIO_IO_SEL [95:64] offset 0x44
// GPIO_LVL    [95:64] offset 0x48
// Signal                  UsedAs                       USE     I/O      LVL     INV
// ---------------------------------------------------------------------------------

// CLKOUTFLEX0/GPIO64      FLEXCLK                      N       -        -       -   0 1
// CLKOUTFLEX1/GPIO65      FM_LINK_WIDTH_ID1            G       I        -       -   1 1
// CLKOUTFLEX2/GPIO66      FM_LINK_WIDTH_ID2            G       I        -       -   1 1
// CLKOUTFLEX3/GPIO67      FM_LINK_WIDTH_ID3            G       I        -       -   1 1
// TOTAL                                                                             E F  0

// TACH4/GPIO68            TACH 4                     	N       -        -       -   0 1
// TACH5/GPIO69            TACH 5                     	N       -        -       -   0 1
// TACH6/GPIO70            TACH 6                     	N       -        -       -   0 1
// TACH7/GPIO71            TACH 7                     	N       -        -       -   0 1
// TOTAL                                                                             0 F  0

// BATLOW/GPIO72           FM_LOM_DEV_OFF_N             G       O        H       -   1 0
// SRC0CLKRQ#/GP73         PLD_JTAG_TMS                 G       O        H       -   1 0
// SML1ALERT#/GPIO74       FM_PCH_BMC_THERMTRIP_N      	G       O        H       -   1 0
// SML1DATA/GPIO75         SML1DATA                     N       -        -       -   0 1
// TOTAL                                                                             7 8  7
// TOTAL                                                                      FFFFF70E FFFFF8FF 00000700

#define PLATFORM_SRP_GPIO_USE_SEL_VAL                 0xFF35F93D      
#define PLATFORM_SRP_GPIO_USE_SEL2_VAL                0x2382F03F      
#define PLATFORM_SRP_GPIO_USE_SEL3_VAL                0xFFFFF70E      
#define PLATFORM_SRP_GPIO_IO_SEL_VAL                  0xA9E6F7FF      
#define PLATFORM_SRP_GPIO_IO_SEL2_VAL                 0xDFFF00F2      
#define PLATFORM_SRP_GPIO_IO_SEL3_VAL                 0xFFFFF8FF      
#define PLATFORM_SRP_GPIO_LVL_VAL                     0x56190800      
#define PLATFORM_SRP_GPIO_LVL2_VAL                    0x2100F004      
#define PLATFORM_SRP_GPIO_LVL3_VAL                    0x00000700
#define PLATFORM_SRP_GPIO_INV_VAL                     0x00000000
#define PLATFORM_SRP_GPIO_BLINK_VAL                   0x00000000





//
// Grantley Mayan City SRP1 Config
//
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP0 / BMBUSY#                            [I] IRQ_PCH_SCI_WHEA_N                      G  I       1   1  0
// GP1 / TACH1                              [I]FM_TACH1 - RESERVED                      N  I          0   1  0
// GP2 / PIRQ[E]#                           [I] FM_ERR0_N                               G  I       1   1  0
// GP3 / PIRQ[F]#                           [I] FM_ERR1_N                               G  I       1   1  0
// TOTAL                                                                                           D   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP4 / PIRQ[G]#                           [I] FM_BMC_PCH_SMI_LPC_N                    G  I       1   1  0
// GP5 / PIRQ[H]#                           [I] FM_CATERR_N                             G  I       1   1  0
// GP6 / TACH2                              [I] FM_TACH2 - RESERVED                     N  I          0   1  0
// GP7 / TACH3                              [I]FM_TACH3 - RESERVED                      N  I       0   1  0
// TOTAL                                                                                              3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP8 / OCS                                [I] IRQ_BMC_PCH_NMI                         G  I       1   1  0
// GP9 / OC[5]#                             OC[5]# - RESERVED                           N  I       0   1  0
// GP10 / OC[6]#                            OC[6]# - RESERVED                           N  I       0   1  0
// GP11 / SMBALERT#                         [O] FM_CPU_PCIE_SLOT_RST_N                  G  O  H       1   0  1
// TOTAL                                                                                           9   7  8
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP12 / LANPHYPC                          [I] FM_LINK_WIDTH_ID0                       G  I       1   1  0
// GP13 / DOCKAZRST#                        [I] FM_BMC_PCH_SCI_LPC_N                    G  I       1   1  0
// GP14 / OC7#                              [I] FM_LINK_WIDTH_ID4                       G  I       1   1  0
// GP15                                     [I/O] FM_PCH_SATA_RAID_KEY                  G  I       1   1  0
// TOTAL                                                                                           F   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP16  / SATA4GP_PCIEP1 / MGPIO9          [O] FM_NM _THROTTLE_N                       G  O  H    1   0  1
// GP17 / TACH0                             [I]FM_TACH0 - RESERVED                      N  I          0   1  0
// GP18 / SRC1CLKRQ#                        Unused                                      G  I       1   1  0
// GP19 / SATA1GP                           [O] FM_PCH_PCIE_SLOT_RST_N                  G  O  H    1   0  1
// TOTAL                                                                                           D   6  9
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP20 / SMI#                              [O] FM_SMI_ACTIVE_N                         G  O  H    1   0  1
// SATA0GP / GP21                           Unused                                      G  I          1   1  0
// GP22 / SCLOCK                            SCLOCK  - SATA SGPIO SCLOCK - RESERVED      N  I       0   1  0
// GP23 / LDRQ1#                            [I] LDRQ[1]_N                               N  I       0   1  0
// TOTAL                                                                                           3   E  1
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// MGPIO0 / GP24                            Unused                                      G  I       1   1  0
// GP25                                     [O] FP_PWR_LED_N                            G  O  L    1   0  0
// GP26                                     [O] FM_LA_TRIGGER_N                         G  O  H    1   0  1
// GP27 / MGPIO6                            Unused                                      G  I          1   1  0
// TOTAL                                                                                           F   9  4
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP28 / MGPIO7                            [O] FM_MIC_MUX_RST_N                        G  O  H    1   0  1
// GP29 / SLP_WLAN# / MGPIO3                [I] FM_BIOS_IMAGE_SWAP_N                    G  I       1   1  0
// GP30 / SUSWARN# / SUSPWRDNACK / MGPIO1   Unused                                      G  I          1   1  0
// GP31 / ACPRESENT / MGPIO2                [I] SML1ALERT_N                             G  I       1   1  0
// TOTAL                                                                                           F   E  1
//
//                   MAYAN_GPIO_USE_SEL_VAL FF3DF93D
//                    MAYAN_GPIO_IO_SEL_VAL E9E6F7FF
//                       MAYAN_GPIO_LVL_VAL 14190800
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP32                                     [O] FM_BIOS_POST_COMPLETE_N                 G  O  H    1   0  1
// GP33 / DOCKEN#                           [I] FM_QPI_SLOW_MODE_N                      G  I       1   1  0
// GP34 / STPPCI#                           SPARE                                       G  I       1   1  0
// GP35 / SATACLKREQ# / NMI#                [O] FM_NMI_EVENT_N                          G  O  H    1   0  1
// TOTAL                                                                                           F   6  9
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP36 / SATA2GP                           [I] FM_BIOS_ADV_FUNCTIONS                   G  I       1   1  0
// GP37 / SATA3GP                           [I] FM_ADR_TRIGGER_N                        G  I       1   1  0
// GP38 / SLOAD                             [O] SLOAD - SATA SGPIO LOAD  - RESERVED     N  I       0   1  0
// GP39 / SDATAOUT[0]                       [O] SDATAOUT[0] - RESERVED                  N  I       0   1  0
// TOTAL                                                                                           3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP40 / OC[1]#                            OC[1]# - RESERVED                           N  I       0   1  0
// GP41 / OC[2]#                            OC[2]# - RESERVED                           N  I       0   1  0
// GP42 / OC[3]#                            OC[3]# - RESERVED                           N  I       0   1  0
// GP43 / OC[4]#                            OC[4]# - RESERVED                           N  I       0   1  0
// TOTAL                                                                                           0   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP44                                     [O] FM_LAN0_DISABLE_N                       G  O  H    1   0  1
// GP45                                     [O] FM_LAN1_DISABLE_N                       G  O  H    1   0  1
// GP46                                     Unused                                      G  I       1   1  0
// GP47                                     Unused                                      G  I       1   1  0
// TOTAL                                                                                           F   C  3
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP48 / SDATAOUT[1]                       SDATAOUT[1] - RESERVED                      N  I       0   1  0
// GP49 / SATA5GP_PCIE_P2 / MGPIO10         [I] FM_CPU_PROCHOT_N                        G  I       1   1  0
// GP50 / GSXCLK                            [O] Serial GPIO Xpansion Interface: CLOCK   N  I       0   1  0
// GP51 /GSXDOUT                            [O] Serial GPIO Xpansion Interface: DOUT    N  I       0   1  0
// TOTAL                                                                                           2   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP52/GSX_SLOAD                           [O] Serial GPIO Xpansion Interface: LOAD    N  I       0   1  0
// GP53 / GSXDIN                            [I] Serial GPIO Xpansion Interface: DIN     N  I       0   1  0
// GP54 / GSXSRESET#                        [O] Serial GPIO Xpansion Interface: RESET   N  I       0   1  0
// GP55                                     [I] FM_BIOS_RCVR_BOOT_N                     G  I       1   1  0
// TOTAL                                                                                           8   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP56                                     [I] FM_PLD_JTAG_TCK                         G  I          1   1  0
// GP57 / MGPIO5                            [I] FM_ME_RCVR_N                            G  I       1   1  0
// GP58 / SML1CLK / MGPIO11                 [I/O]SML1CLK - RESERVED                     N  I       0   1  0
// GP59 / OC[0]#                            OC[0]# - RESERVED                           N  I       0   1  0
// TOTAL                                                                                   I       3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP60 / SML0ALERT# / MGPIO4               [O] SML0_ALERT_N [NM / BMC SKU]             N  O          0   0  0
// GP61                                     (O) FM_PLD_JTAG_TDI                         G  O          1   0  0
// GP62 / SUSCLK                            [O]SUSCLK -RESERVED-                        N  I       0   1  0
// GP63 / SLP_S5#                           [O] SLP_S5# - RESERVED                      N  I       0   1  0
// TOTAL                                                                                              2   C  0
//
//                  MAYAN_GPIO_USE_SEL2_VAL 2382F03F
//                   MAYAN_GPIO_IO_SEL2_VAL CFFFCFF6
//                      MAYAN_GPIO_LVL2_VAL 00003009
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP64 / FLEXCLOCK[0]                      [I] FM_LINK_WIDTH_ID3                       G  I          1   1  0
// GP65 / FLEXCLOCK[1]                      [I] FM_LINK_WIDTH_ID1                       G  I       1   1  0
// GP66 / FLEXCLOCK[2]                      [I] FM_LINK_WIDTH_ID2                       G  I       1   1  0
// GP67 / FLEXCLOCK[3]                      FLEXCLK[3] 48- MHz                          N  I          0   1  0
// TOTAL                                                                                              7   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP68 / TACH4                             [I] TACH 4 - RESERVED                       N  O       0   0  0
// GP69 / TACH5                             [I] TACH 5 - RESERVED                       N  I       0   1  0
// GP70 / TACH6 / USB3_P5#                  [I] TACH 6 - RESERVED                       N  I       0   1  0
// GP71 / TACH7 / USB3_P6#                  [I] TACH 7 - RESERVED                       N  I       0   1  0
// TOTAL                                                                                           0   E  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP72                                     Unused                                      G  I       1   1  0
// GP73                                     (O) PLD_JTAG_TMS                            G  I          1   1  0
// GP74 / SML1ALERT# /PCH_HOT#/ MGPIO8      [O]FM_PCH_BMC_THERMTRIP_N [BMC SKU]         G  O  H       1   0  1
// GP75 / SML1DATA / MGPIO12                [O] SML1DATA -RESERVED-                     N          0   0  0
// TOTAL                                                                                           7   3  4
//
//
//                  MAYAN_GPIO_USE_SEL3_VAL FFFFF707
//                   MAYAN_GPIO_IO_SEL3_VAL FFFFF3EF
//                      MAYAN_GPIO_LVL3_VAL 00000400

#define MAYAN_GPIO_USE_SEL_VAL           0xFF3DF93D
#define MAYAN_GPIO_IO_SEL_VAL   0xE9E6F7FF
#define MAYAN_GPIO_LVL_VAL      0x14190800
#define MAYAN_GPIO_USE_SEL2_VAL 0x2382F03F
#define MAYAN_GPIO_IO_SEL2_VAL           0xCFFFCFF6
#define MAYAN_GPIO_LVL2_VAL     0x00003009
#define MAYAN_GPIO_USE_SEL3_VAL          0xFFFFF707
#define MAYAN_GPIO_IO_SEL3_VAL           0xFFFFF3EF
#define MAYAN_GPIO_LVL3_VAL     0x00000400
#define MAYAN_GPIO_BLINK_VAL    0x00000000
#define MAYAN_GPIO_INV_VAL      0x00000000

//
// GPIO differences from Mayan City to Wildcat Pass
// Pin Becomes					Was					Name
// 74  Native					Output High			FM_PCH_BMC_THERMTRIP_N
// 72          Output  High		Input				FM_NIC_I350_DEV_OFF_LVC3_N
// 69  GPIO    Input			FAN_PCH_TACH5		Unused
// 68  GPIO    Input			FAN_PCH_TACH4		Unused
// 54  GPIO    Input			Unused				Unused
// 53  GPIO    Output  High		GSXDIN				Unused
// 52  GPIO    Input			GSXLOAD				Unused
// 51  GPIO    Output  High		GSXDOUT				Unused
// 50  GPIO    Input			GSXCLK				Unused
// 48  GPIO    Input			SGPIO_SATA_DATAOUT1 Unused
// 35  Native					NMI					NMI
// 30  Native					FP_ID_BTN			Unused
// 28                  Low		High				Unused
// 26                  Low		FM_LA_TRIGGER		Unused
// 24          Output  Low		FM_PMBUS_MUX		Unused
// 20  Native					Output High			SMI_ACTIVE
// 18          Output  High		Unused				Unused
// 17  GPIO    Input			FAN_PCH_TACH0		FM_ADR_TRIGGER_GPIO17 (unused)
//  7  GPIO    Input			FAN_PCH_TACH3		Unused
//  6  GPIO    Input			FAN_PCH_TACH2		Unused
//  1  GPIO    Input			FAN_PCH_TACH1		Unused
//
// 72 needs code if you want to disable lan
// 4 and 13 need code to get SMI/SCI from BMC - EPSD special, will not do
// All others are don't-care/unused
//
#define WILDCAT_GPIO_USE_SEL_VAL           0xBF2FF9FF
#define WILDCAT_GPIO_IO_SEL_VAL   0xE8A2F1FF
#define WILDCAT_GPIO_LVL_VAL      0x00BD0800
#define WILDCAT_GPIO_USE_SEL2_VAL 0x23FFF037
#define WILDCAT_GPIO_IO_SEL2_VAL           0x03D7CC36
#define WILDCAT_GPIO_LVL2_VAL     0x807E3005
#define WILDCAT_GPIO_USE_SEL3_VAL          0x00000337
#define WILDCAT_GPIO_IO_SEL3_VAL           0x000002F7
#define WILDCAT_GPIO_LVL3_VAL     0x000005C0
#define WILDCAT_GPIO_BLINK_VAL    0x00000000
#define WILDCAT_GPIO_INV_VAL      0x00002010

//
// Grantley Mayan City SRP1 Config
//
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP0 / BMBUSY#                            [I] IRQ_PCH_SCI_WHEA_N                      G  I          1   1  0
// GP1 / TACH1                              [I]FM_TACH1 - RESERVED                      N  I          0   1  0
// GP2 / PIRQ[E]#                           [I] FM_ERR0_N                               G  I          1   1  0
// GP3 / PIRQ[F]#                           [I] FM_ERR1_N                               G  I          1   1  0
// TOTAL                                                                                              D   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP4 / PIRQ[G]#                           [I] FM_BMC_PCH_SMI_LPC_N                    G  I          1   1  0
// GP5 / PIRQ[H]#                           [I] FM_CATERR_N                             G  I          1   1  0
// GP6 / TACH2                              [I] FM_TACH2 - RESERVED                     N  I          0   1  0
// GP7 / TACH3                              [I]FM_TACH3 - RESERVED                      N  I          0   1  0
// TOTAL                                                                                              3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP8 / OCS                                [I] IRQ_BMC_PCH_NMI                         G  I          1   1  0
// GP9 / OC[5]#                             OC[5]# - RESERVED                           N  I          0   1  0
// GP10 / OC[6]#                            OC[6]# - RESERVED                           N  I          0   1  0
// GP11 / SMBALERT#                         [O] FM_CPU_PCIE_SLOT_RST_N                  G  O  H       1   0  1
// TOTAL                                                                                              9   7  8
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP12 / LANPHYPC                          [I] FM_LINK_WIDTH_ID0                       G  I          1   1  0
// GP13 / DOCKAZRST#                        [I] FM_BMC_PCH_SCI_LPC_N                    G  I          1   1  0
// GP14 / OC7#                              [I] FM_LINK_WIDTH_ID4                       G  I          1   1  0
// GP15                                     [I/O] FM_PCH_SATA_RAID_KEY                  G  I          1   1  0
// TOTAL                                                                                              F   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP16  / SATA4GP_PCIEP1 / MGPIO9          [O] FM_NM _THROTTLE_N                       G  O  H       1   0  1
// GP17 / TACH0                             [I]FM_TACH0 - RESERVED                      N  I          0   1  0
// GP18 / SRC1CLKRQ#                        Unused                                      G  I          1   1  0
// GP19 / SATA1GP                           [O] FM_PCH_PCIE_SLOT_RST_N                  G  O  H       1   0  1
// TOTAL                                                                                              D   6  9
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP20 / SMI#                              [O] FM_SMI_ACTIVE_N                         G  O  H       1   0  1
// SATA0GP / GP21                           Unused                                      G  I          1   1  0
// GP22 / SCLOCK                            SCLOCK  - SATA SGPIO SCLOCK - RESERVED      N  I          0   1  0
// GP23 / LDRQ1#                            [I] LDRQ[1]_N                               N  I          0   1  0
// TOTAL                                                                                              3   E  1
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// MGPIO0 / GP24                            Unused                                      G  I          1   1  0
// GP25                                     [O] FP_PWR_LED_N                            G  O  L       1   0  0
// GP26                                     [O] FM_LA_TRIGGER_N                         G  O  H       1   0  1
// GP27 / MGPIO6                            [I] FM_GPIO_EXP_INT_N [BMC-less/DCMI SKU]   G  I          1   1  0
// TOTAL                                                                                              F   9  4
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP28 / MGPIO7                            [O] FM_MIC_MUX_RST_N                        G  O  H       1   0  1
// GP29 / SLP_WLAN# / MGPIO3                [I] FM_BIOS_IMAGE_SWAP_N                    G  I          1   1  0
// GP30 / SUSWARN# / SUSPWRDNACK / MGPIO1   [I] FP_ID_BT_N (DCMI BMC LESS SKU)          G  I          1   1  0
// GP31 / ACPRESENT / MGPIO2                [I] SML1ALERT_N                             G  I          1   1  0
// TOTAL                                                                                              F   E  1
//
//               MAYAN_DCMI_GPIO_USESEL_VAL FF3DF93D
//                MAYAN_DCMI_GPIO_IOSEL_VAL E9E6F7FF
//                  MAYAN_DCMI_GPIO_LVL_VAL 14190800
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP32                                     [O] FM_BIOS_POST_COMPLETE_N                 G  O  H       1   0  1
// GP33 / DOCKEN#                           [I] FM_QPI_SLOW_MODE_N                      G  I          1   1  0
// GP34 / STPPCI#                           SPARE                                       G  I          1   1  0
// GP35 / SATACLKREQ# / NMI#                [O] FM_NMI_EVENT_N                          G  O  H       1   0  1
// TOTAL                                                                                              F   6  9
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP36 / SATA2GP                           [I] FM_BIOS_ADV_FUNCTIONS                   G  I          1   1  0
// GP37 / SATA3GP                           [I] FM_ADR_TRIGGER_N                        G  I          1   1  0
// GP38 / SLOAD                             [O] SLOAD - SATA SGPIO LOAD  - RESERVED     N  I          0   1  0
// GP39 / SDATAOUT[0]                       [O] SDATAOUT[0] - RESERVED                  N  I          0   1  0
// TOTAL                                                                                              3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP40 / OC[1]#                            OC[1]# - RESERVED                           N  I          0   1  0
// GP41 / OC[2]#                            OC[2]# - RESERVED                           N  I          0   1  0
// GP42 / OC[3]#                            OC[3]# - RESERVED                           N  I          0   1  0
// GP43 / OC[4]#                            OC[4]# - RESERVED                           N  I          0   1  0
// TOTAL                                                                                              0   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP44                                     [O] FM_LAN0_DISABLE_N                       G  O  H       1   0  1
// GP45                                     [O] FM_LAN1_DISABLE_N                       G  O  H       1   0  1
// GP46                                     Unused                                      G  I          1   1  0
// GP47                                     Unused                                      G  I          1   1  0
// TOTAL                                                                                              F   C  3
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP48 / SDATAOUT[1]                       SDATAOUT[1] - RESERVED                      N  I          0   1  0
// GP49 / SATA5GP_PCIE_P2 / MGPIO10         [I] FM_CPU_PROCHOT_N                        G  I          1   1  0
// GP50 / GSXCLK                            [O] Serial GPIO Xpansion Interface: CLOCK   N  I          0   1  0
// GP51 /GSXDOUT                            [O] Serial GPIO Xpansion Interface: DOUT    N  I          0   1  0
// TOTAL                                                                                              2   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP52/GSX_SLOAD                           [O] Serial GPIO Xpansion Interface: LOAD    N  I          0   1  0
// GP53 / GSXDIN                            [I] Serial GPIO Xpansion Interface: DIN     N  I          0   1  0
// GP54 / GSXSRESET#                        [O] Serial GPIO Xpansion Interface: RESET   N  I          0   1  0
// GP55                                     [I] FM_BIOS_RCVR_BOOT_N                     G  I          1   1  0
// TOTAL                                                                                              8   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP56                                     [I] FM_PLD_JTAG_TCK                         G  I          1   1  0
// GP57 / MGPIO5                            [I] FM_ME_RCVR_N                            G  I          1   1  0
// GP58 / SML1CLK / MGPIO11                 [I/O]SML1CLK - RESERVED                     N  I          0   1  0
// GP59 / OC[0]#                            OC[0]# - RESERVED                           N  I          0   1  0
// TOTAL                                                                                   I          3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP60 / SML0ALERT# / MGPIO4               [I] SML0_ALERT_N [DCMI SKU]                 N  I          0   1  0
// GP61                                     (O) FM_PLD_JTAG_TDI                         G  O          1   0  0
// GP62 / SUSCLK                            [O]SUSCLK -RESERVED-                        N  I          0   1  0
// GP63 / SLP_S5#                           [O] SLP_S5# - RESERVED                      N  I          0   1  0
// TOTAL                                                                                              2   D  0
//
//             MAYAN_DCMI_GPIO_USE_SEL2_VAL 2382F03F
//              MAYAN_DCMI_GPIO_IO_SEL2_VAL DFFFCFF6
//                 MAYAN_DCMI_GPIO_LVL2_VAL 00003009
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP64 / FLEXCLOCK[0]                      [I] FM_LINK_WIDTH_ID3                       G  I          1   1  0
// GP65 / FLEXCLOCK[1]                      [I] FM_LINK_WIDTH_ID1                       G  I          1   1  0
// GP66 / FLEXCLOCK[2]                      [I] FM_LINK_WIDTH_ID2                       G  I          1   1  0
// GP67 / FLEXCLOCK[3]                      FLEXCLK[3] - 48 Mhz                         N  I          0   1  0
// TOTAL                                                                                              7   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP68 / TACH4                             [I] TACH 4 - RESERVED                       N  O          0   0  0
// GP69 / TACH5                             [I] TACH 5 - RESERVED                       N  I          0   1  0
// GP70 / TACH6 / USB3_P5#                  [I] TACH 6 - RESERVED                       N  I          0   1  0
// GP71 / TACH7 / USB3_P6#                  [I] TACH 7 - RESERVED                       N  I          0   1  0
// TOTAL                                                                                              0   E  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP72                                     Unused                                      G  I          1   1  0
// GP73                                     (O) PLD_JTAG_TMS                            G  I          1   1  0
// GP74 / SML1ALERT# /PCH_HOT#/ MGPIO8      [O] FM_ME_CMM_N (DCMI BMC LESS SKU)         G  O  H       1   0  1
// GP75 / SML1DATA / MGPIO12                [O] SML1DATA -RESERVED-                     N             0   0  0
// TOTAL                                                                                              7   3  4
//
//
//             MAYAN_DCMI_GPIO_USE_SEL3_VAL FFFFF707
//              MAYAN_DCMI_GPIO_IO_SEL3_VAL FFFFF3EF
//                 MAYAN_DCMI_GPIO_LVL3_VAL 00000400

   #define MAYAN_DCMI_GPIO_USE_SEL_VAL      0xFF3DF93D
   #define MAYAN_DCMI_GPIO_IO_SEL_VAL       0xE9E6F7FF
   #define MAYAN_DCMI_GPIO_LVL_VAL          0x14190800
   #define MAYAN_DCMI_GPIO_USE_SEL2_VAL     0x2382F03F
   #define MAYAN_DCMI_GPIO_IO_SEL2_VAL      0xDFFFCFF6
   #define MAYAN_DCMI_GPIO_LVL2_VAL         0x00003009
   #define MAYAN_DCMI_GPIO_USE_SEL3_VAL     0xFFFFF707
   #define MAYAN_DCMI_GPIO_IO_SEL3_VAL      0xFFFFF3EF
   #define MAYAN_DCMI_GPIO_LVL3_VAL         0x00000400
   #define MAYAN_DCMI_GPIO_BLINK_VAL        0x00000000
   #define MAYAN_DCMI_GPIO_INV_VAL          0x00000000

   //
// Grantley Yakima City SRP1 Config
//
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP0 / BMBUSY#                            [I] IRQ_PCH_SCI_WHEA_N                      G  I          1   1  0
// GP1 / TACH1                              [I] FM_TACH1 - RESERVED                     N  I          0   1  0
// GP2 / PIRQ[E]#                           [I] FM_ERR0_N                               G  I          1   1  0
// GP3 / PIRQ[F]#                           [I] FM_ERR1_N                               G  I          1   1  0
// TOTAL                                                                                              D   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP4 / PIRQ[G]#                           [I] FM_BMC_PCH_SMI_LPC_N                    G  I          1   1  0
// GP5 / PIRQ[H]#                           [I] FM_CATERR_N                             G  I          1   1  0
// GP6 / TACH2                              [I] FM_TACH2 - RESERVED                     N  I          0   1  0
// GP7 / TACH3                              [I]FM_TACH3 - RESERVED                      N  I          0   1  0
// TOTAL                                                                                              3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP8 / OCS                                [I] IRQ_BMC_PCH_NMI                         G  I          1   1  0
// GP9 / OC[5]#                             OC[5]# - RESERVED                           N  I          0   1  0
// GP10 / OC[6]#                            OC[6]# - RESERVED                           N  I          0   1  0
// GP11 / SMBALERT#                         [O] FM_PCIE_GEN3_SLOT_RST_N                 G  O  H       1   0  1
// TOTAL                                                                                              9   7  8
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP12 / LANPHYPC                          Unused                                      G  I          1   1  0
// GP13 / DOCKAZRST#                        [I] FM_BMC_PCH_SCI_LPC_N                    G  I          1   1  0
// GP14 / OC7#                              Unused                                      G  I          1   1  0
// GP15                                     [I/O] FM_PCH_SATA_RAID_KEY                  G  I          1   1  0
// TOTAL                                                                                              F   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP16  / SATA4GP_PCIEP1 / MGPIO9          [O] FM_NM _THROTTLE_N                       G  O  H       1   0  1
// GP17 / TACH0                             [I] FM_TACH0 - RESERVED                     N  I          0   1  0
// GP18 / SRC1CLKRQ#                        Unused                                      G  I          1   1  0
// GP19 / SATA1GP                           [O] FM_PCH_PCIE_SLOT_RST_N                  G  O  H       1   0  1
// TOTAL                                                                                              D   6  9
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP20 / SMI#                              [O] FM_SMI_ACTIVE_N                         G  O  H       1   0  1
// SATA0GP / GP21                           Unused                                      G  I          1   1  0
// GP22 / SCLOCK                            SCLOCK  - SATA SGPIO SCLOCK - RESERVED      N  I          0   1  0
// GP23 / LDRQ1#                            [I] LDRQ[1]_N                               N  I          0   1  0
// TOTAL                                                                                              3   E  1
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// MGPIO0 / GP24                            Unused                                      G  I          1   1  0
// GP25                                     [O] FP_PWR_LED_N                            G  O  L       1   0  0
// GP26                                     [O] FM_LA_TRIGGER_N                         G  O  H       1   0  1
// GP27 / MGPIO6                            Unused                                      G  I          1   1  0
// TOTAL                                                                                              F   9  4
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP28 / MGPIO7                            [O] FM_MIC_MUX_RST_N                        G  O  H       1   0  1
// GP29 / SLP_WLAN# / MGPIO3                [I] FM_BIOS_IMAGE_SWAP_N                    G  I          1   1  0
// GP30 / SUSWARN# / SUSPWRDNACK / MGPIO1   [O] FM_EUP_LOT6                             G  O          1   0  0
// GP31 / ACPRESENT / MGPIO2                [I] SML1ALERT_N                             G  I          1   1  0
// TOTAL                                                                                              F   A  1
//
//                  YAKIMA_GPIO_USE_SEL_VAL FF3DF93D
//                   YAKIMA_GPIO_IO_SEL_VAL A9E6F7FF
//                      YAKIMA_GPIO_LVL_VAL 14190800
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP32                                     [O] FM_BIOS_POST_COMPLETE_N                 G  O  H       1   0  1
// GP33 / DOCKEN#                           [I] FM_QPI_SLOW_MODE_N                      G  I          1   1  0
// GP34 / STPPCI#                           SPARE                                       G  I          1   1  0
// GP35 / SATACLKREQ# / NMI#                [O] FM_NMI_EVENT_N                          G  O  H       1   0  1
// TOTAL                                                                                              F   6  9
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP36 / SATA2GP                           [I] FM_BIOS_ADV_FUNCTIONS                   G  I          1   1  0
// GP37 / SATA3GP                           [I] FM_ADR_TRIGGER_N                        G  I          1   1  0
// GP38 / SLOAD                             [O] SLOAD - SATA SGPIO LOAD  - RESERVED     N  I          0   1  0
// GP39 / SDATAOUT[0]                       [O] SDATAOUT[0] - RESERVED                  N  I          0   1  0
// TOTAL                                                                                              3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP40 / OC[1]#                            OC[1]# - RESERVED                           N  I          0   1  0
// GP41 / OC[2]#                            OC[2]# - RESERVED                           N  I          0   1  0
// GP42 / OC[3]#                            OC[3]# - RESERVED                           N  I          0   1  0
// GP43 / OC[4]#                            OC[4]# - RESERVED                           N  I          0   1  0
// TOTAL                                                                                              0   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP44                                     [O] FM_LAN0_DISABLE_N                       G  O  H       1   0  1
// GP45                                     [O] FM_LAN1_DISABLE_N                       G  O  H       1   0  1
// GP46                                     Unused                                      G  I          1   1  0
// GP47                                     Unused                                      G  I          1   1  0
// TOTAL                                                                                              F   C  3
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP48 / SDATAOUT[1]                       SDATAOUT[1] - RESERVED                      N  I          0   1  0
// GP49 / SATA5GP_PCIE_P2 / MGPIO10         [I] FM_CPU_PROCHOT_N                        G  I          1   1  0
// GP50 / GSXCLK                            [O] Serial GPIO Xpansion Interface: CLOCK   N  I          0   1  0
// GP51 /GSXDOUT                            [O] Serial GPIO Xpansion Interface: DOUT    N  I          0   1  0
// TOTAL                                                                                              2   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP52/GSX_SLOAD                           [O] Serial GPIO Xpansion Interface: LOAD    N  I          0   1  0
// GP53 / GSXDIN                            [I] Serial GPIO Xpansion Interface: DIN     N  I          0   1  0
// GP54 / GSXSRESET#                        [O] Serial GPIO Xpansion Interface: RESET   N  I          0   1  0
// GP55                                     [I] FM_BIOS_RCVR_BOOT_N                     G  I          1   1  0
// TOTAL                                                                                              8   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP56                                     [I] FM_PLD_JTAG_TCK                         G  I          1   1  0
// GP57 / MGPIO5                            [I] FM_ME_RCVR_N                            G  I          1   1  0
// GP58 / SML1CLK / MGPIO11                 [I/O]SML1CLK - RESERVED                     N  I          0   1  0
// GP59 / OC[0]#                            OC[0]# - RESERVED                           N  I          0   1  0
// TOTAL                                                                                   I          3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP60 / SML0ALERT# / MGPIO4               [O] SML0_ALERT_N [NM / BMC SKU]             N  I          0   1  0* TODO Check usage with BMC less vs BMC with
// GP61                                     [O] FM_PLD_JTAG_TDI                         G  O          1   0  0
// GP62 / SUSCLK                            [O]SUSCLK -RESERVED-                        N  I          0   1  0
// GP63 / SLP_S5#                           [O] SLP_S5# - RESERVED                      N  I          0   1  0
// TOTAL                                                                                              2   D  0
//
//                 YAKIMA_GPIO_USE_SEL2_VAL 2382F03F
//                  YAKIMA_GPIO_IO_SEL2_VAL DFFFCFF6
//                     YAKIMA_GPIO_LVL2_VAL 00003009
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP64 / FLEXCLOCK[0]                      Unused                                      G  I          1   1  0
// GP65 / FLEXCLOCK[1]                      Unused                                      G  I          1   1  0
// GP66 / FLEXCLOCK[2]                      Unused                                      G  I          1   1  0
// GP67 / FLEXCLOCK[3]                      FLEXCLK[3] - 48 Mhz                         N  I          0   1  0
// TOTAL                                                                                              7   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP68 / TACH4                             [I] TACH 4 - RESERVED                       N  O          0   0  0
// GP69 / TACH5                             [I] TACH 5 - RESERVED                       N  I          0   1  0
// GP70 / TACH6 / USB3_P5#                  [I] TACH 6 - RESERVED                       N  I          0   1  0
// GP71 / TACH7 / USB3_P6#                  [I] TACH 7 - RESERVED                       N  I          0   1  0
// TOTAL                                                                                              0   E  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP72                                     Unused                                      G  I          1   1  0
// GP73                                     [O] PLD_JTAG_TMS                            G  O          1   0  0
// GP74 / SML1ALERT# /PCH_HOT#/ MGPIO8      [O]FM_PCH_BMC_THERMTRIP_N [BMC SKU]         G  O  H       1   0  1
// GP75 / SML1DATA / MGPIO12                [O] SML1DATA -RESERVED-                     N             0   0  0
// TOTAL                                                                                              7   1  4
//
//
//                 YAKIMA_GPIO_USE_SEL3_VAL FFFFF707
//                  YAKIMA_GPIO_IO_SEL3_VAL FFFFF1EF
//                     YAKIMA_GPIO_LVL3_VAL 00000400

   #define YAKIMA_GPIO_USE_SEL_VAL          0xFF3DF93D
   #define YAKIMA_GPIO_IO_SEL_VAL           0xA9E6F7FF
#define YAKIMA_GPIO_LVL_VAL      0x14190800
#define YAKIMA_GPIO_USE_SEL2_VAL 0x2382F03F
   #define YAKIMA_GPIO_IO_SEL2_VAL          0xDFFFCFF6
#define YAKIMA_GPIO_LVL2_VAL     0x00003009
   #define YAKIMA_GPIO_USE_SEL3_VAL         0xFFFFF707
   #define YAKIMA_GPIO_IO_SEL3_VAL          0xFFFFF1EF
#define YAKIMA_GPIO_LVL3_VAL     0x00000400
#define YAKIMA_GPIO_BLINK_VAL    0x00000000
#define YAKIMA_GPIO_INV_VAL      0x00000000
//
// Grantley Yakima City SRP1 Config
//
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP0 / BMBUSY#                            [I] IRQ_PCH_SCI_WHEA_N                      G  I          1   1  0
// GP1 / TACH1                              [I] FM_TACH1 - RESERVED                     N  I          0   1  0
// GP2 / PIRQ[E]#                           [I] FM_ERR0_N                               G  I          1   1  0
// GP3 / PIRQ[F]#                           [I] FM_ERR1_N                               G  I          1   1  0
// TOTAL                                                                                              D   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP4 / PIRQ[G]#                           [I] FM_BMC_PCH_SMI_LPC_N                    G  I          1   1  0
// GP5 / PIRQ[H]#                           [I] FM_CATERR_N                             G  I          1   1  0
// GP6 / TACH2                              [I] FM_TACH2 - RESERVED                     N  I          0   1  0
// GP7 / TACH3                              [I]FM_TACH3 - RESERVED                      N  I          0   1  0
// TOTAL                                                                                              3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP8 / OCS                                [I] IRQ_BMC_PCH_NMI                         G  I          1   1  0
// GP9 / OC[5]#                             OC[5]# - RESERVED                           N  I          0   1  0
// GP10 / OC[6]#                            OC[6]# - RESERVED                           N  I          0   1  0
// GP11 / SMBALERT#                         [O] FM_PCIE_GEN3_SLOT_RST_N                 G  O  H       1   0  1
// TOTAL                                                                                              9   7  8
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP12 / LANPHYPC                          Unused                                      G  I          1   1  0
// GP13 / DOCKAZRST#                        [I] FM_BMC_PCH_SCI_LPC_N                    G  I          1   1  0
// GP14 / OC7#                              Unused                                      G  I          1   1  0
// GP15                                     [I/O] FM_PCH_SATA_RAID_KEY                  G  I          1   1  0
// TOTAL                                                                                              F   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP16  / SATA4GP_PCIEP1 / MGPIO9          [O] FM_NM _THROTTLE_N                       G  O  H       1   0  1
// GP17 / TACH0                             [I] FM_TACH0 - RESERVED                     N  I          0   1  0
// GP18 / SRC1CLKRQ#                        Unused                                      G  I          1   1  0
// GP19 / SATA1GP                           [O] FM_PCH_PCIE_SLOT_RST_N                  G  O  H       1   0  1
// TOTAL                                                                                              D   6  9
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP20 / SMI#                              [O] FM_SMI_ACTIVE_N                         G  O  H       1   0  1
// SATA0GP / GP21                           Unused                                      G  I          1   1  0
// GP22 / SCLOCK                            SCLOCK  - SATA SGPIO SCLOCK - RESERVED      N  I          0   1  0
// GP23 / LDRQ1#                            [I] LDRQ[1]_N                               N  I          0   1  0
// TOTAL                                                                                              3   E  1
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// MGPIO0 / GP24                            Unused                                      G  I          1   1  0
// GP25                                     [O] FP_PWR_LED_N                            G  O  L       1   0  0
// GP26                                     [O] FM_LA_TRIGGER_N                         G  O  H       1   0  1
// GP27 / MGPIO6                            [I] FM_GPIO_EXP_INT_N [BMC-less/DCMI SKU]   G  I          1   1  0
// TOTAL                                                                                              F   9  4
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP28 / MGPIO7                            [O] FM_MIC_MUX_RST_N                        G  O  H       1   0  1
// GP29 / SLP_WLAN# / MGPIO3                [I] FM_BIOS_IMAGE_SWAP_N                    G  I          1   1  0
// GP30 / SUSWARN# / SUSPWRDNACK / MGPIO1   [I] FP_ID_BT_N (DCMI BMC LESS SKU)          G  I          1   1  0
// GP31 / ACPRESENT / MGPIO2                [I] SML1ALERT_N                             G  I          1   1  0
// TOTAL                                                                                              F   E  1
//
//             YAKIMA_DCMI_GPIO_USE_SEL_VAL FF3DF93D
//              YAKIMA_DCMI_GPIO_IO_SEL_VAL E9E6F7FF
//                 YAKIMA_DCMI_GPIO_LVL_VAL 14190800
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP32                                     [O] FM_BIOS_POST_COMPLETE_N                 G  O  H       1   0  1
// GP33 / DOCKEN#                           [I] FM_QPI_SLOW_MODE_N                      G  I          1   1  0
// GP34 / STPPCI#                           SPARE                                       G  I          1   1  0
// GP35 / SATACLKREQ# / NMI#                [O] FM_NMI_EVENT_N                          G  O  H       1   0  1
// TOTAL                                                                                              F   6  9
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP36 / SATA2GP                           [I] FM_BIOS_ADV_FUNCTIONS                   G  I          1   1  0
// GP37 / SATA3GP                           [I] FM_ADR_TRIGGER_N                        G  I          1   1  0
// GP38 / SLOAD                             [O] SLOAD - SATA SGPIO LOAD  - RESERVED     N  I          0   1  0
// GP39 / SDATAOUT[0]                       [O] SDATAOUT[0] - RESERVED                  N  I          0   1  0
// TOTAL                                                                                              3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP40 / OC[1]#                            OC[1]# - RESERVED                           N  I          0   1  0
// GP41 / OC[2]#                            OC[2]# - RESERVED                           N  I          0   1  0
// GP42 / OC[3]#                            OC[3]# - RESERVED                           N  I          0   1  0
// GP43 / OC[4]#                            OC[4]# - RESERVED                           N  I          0   1  0
// TOTAL                                                                                              0   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP44                                     [O] FM_LAN0_DISABLE_N                       G  O  H       1   0  1
// GP45                                     [O] FM_LAN1_DISABLE_N                       G  O  H       1   0  1
// GP46                                     Unused                                      G  I          1   1  0
// GP47                                     Unused                                      G  I          1   1  0
// TOTAL                                                                                              F   C  3
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP48 / SDATAOUT[1]                       SDATAOUT[1] - RESERVED                      N  I          0   1  0
// GP49 / SATA5GP_PCIE_P2 / MGPIO10         [I] FM_CPU_PROCHOT_N                        G  I          1   1  0
// GP50 / GSXCLK                            [O] Serial GPIO Xpansion Interface: CLOCK   N  I          0   1  0
// GP51 /GSXDOUT                            [O] Serial GPIO Xpansion Interface: DOUT    N  I          0   1  0
// TOTAL                                                                                              2   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP52/GSX_SLOAD                           [O] Serial GPIO Xpansion Interface: LOAD    N  I          0   1  0
// GP53 / GSXDIN                            [I] Serial GPIO Xpansion Interface: DIN     N  I          0   1  0
// GP54 / GSXSRESET#                        [O] Serial GPIO Xpansion Interface: RESET   N  I          0   1  0
// GP55                                     [I] FM_BIOS_RCVR_BOOT_N                     G  I          1   1  0
// TOTAL                                                                                              8   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP56                                     [I] FM_PLD_JTAG_TCK                         G  I          1   1  0
// GP57 / MGPIO5                            [I] FM_ME_RCVR_N                            G  I          1   1  0
// GP58 / SML1CLK / MGPIO11                 [I/O]SML1CLK - RESERVED                     N  I          0   1  0
// GP59 / OC[0]#                            OC[0]# - RESERVED                           N  I          0   1  0
// TOTAL                                                                                   I          3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP60 / SML0ALERT# / MGPIO4               [I] SML0_ALERT_N (DCMI SKU]                 N  I          0   1  0
// GP61                                     [O] FM_PLD_JTAG_TDI                         G  O          1   0  0
// GP62 / SUSCLK                            [O]SUSCLK -RESERVED-                        N  I          0   1  0
// GP63 / SLP_S5#                           [O] SLP_S5# - RESERVED                      N  I          0   1  0
// TOTAL                                                                                              2   D  0
//
//            YAKIMA_DCMI_GPIO_USE_SEL2_VAL 2382F03F
//             YAKIMA_DCMI_GPIO_IO_SEL2_VAL DFFFCFF6
//                YAKIMA_DCMI_GPIO_LVL2_VAL 00003009
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP64 / FLEXCLOCK[0]                      Unused                                      G  I          1   1  0
// GP65 / FLEXCLOCK[1]                      Unused                                      G  I          1   1  0
// GP66 / FLEXCLOCK[2]                      Unused                                      G  I          1   1  0
// GP67 / FLEXCLOCK[3]                      FLEXCLOCK[3]                                N  I          0   1  0
// TOTAL                                                                                              7   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP68 / TACH4                             [I] TACH 4 - RESERVED                       N  O          0   0  0
// GP69 / TACH5                             [I] TACH 5 - RESERVED                       N  I          0   1  0
// GP70 / TACH6 / USB3_P5#                  [I] TACH 6 - RESERVED                       N  I          0   1  0
// GP71 / TACH7 / USB3_P6#                  [I] TACH 7 - RESERVED                       N  I          0   1  0
// TOTAL                                                                                              0   E  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP72                                     Unused                                      G  I          1   1  0
// GP73                                     [O] PLD_JTAG_TMS                            G  O          1   0  0
// GP74 / SML1ALERT# /PCH_HOT#/ MGPIO8      [O] FM_ME_CMM_N (DCMI BMC LESS SKU)         G  O  H       1   0  1
// GP75 / SML1DATA / MGPIO12                [O] SML1DATA -RESERVED-                     N             0   0  0
// TOTAL                                                                                              7   1  4
//
//
//            YAKIMA_DCMI_GPIO_USE_SEL3_VAL FFFFF707
//             YAKIMA_DCMI_GPIO_IO_SEL3_VAL FFFFF1EF
//                YAKIMA_DCMI_GPIO_LVL3_VAL 00000400

   #define YAKIMA_DCMI_GPIO_USE_SEL_VAL     0xFF3DF93D
   #define YAKIMA_DCMI_GPIO_IO_SEL_VAL      0xE9E6F7FF
   #define YAKIMA_DCMI_GPIO_LVL_VAL         0x14190800
   #define YAKIMA_DCMI_GPIO_USE_SEL2_VAL    0x2382F03F
   #define YAKIMA_DCMI_GPIO_IO_SEL2_VAL     0xDFFFCFF6
   #define YAKIMA_DCMI_GPIO_LVL2_VAL        0x00003009
   #define YAKIMA_DCMI_GPIO_USE_SEL3_VAL    0xFFFFF707
   #define YAKIMA_DCMI_GPIO_IO_SEL3_VAL     0xFFFFF1EF
   #define YAKIMA_DCMI_GPIO_LVL3_VAL        0x00000400
   #define YAKIMA_DCMI_GPIO_BLINK_VAL       0x00000000
   #define YAKIMA_DCMI_GPIO_INV_VAL         0x00000000
   
//
// Grantley Aztec City SRP1 Config
//
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP0 / BMBUSY#                            [I] IRQ_PCH_SCI_WHEA_N                      G  I          1   1  0
// GP1 / TACH1                              [I] FM_TACH1 - RESERVED                     N  I          0   1  0
// GP2 / PIRQ[E]#                           [I] FM_ERR0_N                               G  I          1   1  0
// GP3 / PIRQ[F]#                           [I] FM_ERR1_N                               G  I          1   1  0
// TOTAL                                                                                              D   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP4 / PIRQ[G]#                           [I] FM_BMC_PCH_SMI_LPC_N                    G  I          1   1  0
// GP5 / PIRQ[H]#                           [I] FM_CATERR_N                             G  I          1   1  0
// GP6 / TACH2                              [I]FM_TACH2_RESERVED                        N  I          0   1  0
// GP7 / TACH3                              [I]FM_TACH3 - RESERVED                      N  I          0   1  0
// TOTAL                                                                                              3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP8 / OCS                                [I] IRQ_BMC_PCH_NMI                         G  I          1   1  0
// GP9 / OC[5]#                             OC[5]# - RESERVED                           N  I          0   1  0
// GP10 / OC[6]#                            OC[6]# - RESERVED                           N  I          0   1  0
// GP11 / SMBALERT#                         [O] FM_CPU_PCIE_SLOT_RST_N                  G  O  H       1   0  1
// TOTAL                                                                                              9   7  8
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP12 / LANPHYPC                          [N][O] LAN_DIASABLE_N                       N  O          0   0  0
// GP13 / DOCKAZRST#                        [I] FM_BMC_PCH_SCI_LPC_N                    G  I          1   1  0
// GP14 / OC7#                              [I] FM_TBT_HOT_PLUG_EVENT                   G  I          1   1  0
// GP15                                     [I/O] FM_PCH_SATA_RAID_KEY                  G  I          1   1  0
// TOTAL                                                                                              E   E  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP16  / SATA4GP_PCIEP1 / MGPIO9          [O] FM_NM _THROTTLE_N                       G  O  H       1   0  1
// GP17 / TACH0                             [I] FM_TACH0 - RESERVED                     N  I          0   1  0
// GP18 / SRC1CLKRQ#                        Unused                                      G  O  H       1   0  1
// GP19 / SATA1GP                           [O] FM_PCH_PCIE_SLOT_RST_N                  G  O  H       1   0  1
// TOTAL                                                                                              D   2  D
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP20 / SMI#                              Unused                                      G  O          1   0  0
// SATA0GP / GP21                           Unused                                      G  I  H       1   1  1
// GP22 / SCLOCK                            SCLOCK  - SATA SGPIO SCLOCK                 N  I          0   1  0
// GP23 / LDRQ1#                            [I] LDRQ[1]_N                               N  I          0   1  0
// TOTAL                                                                                              3   E  2
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// MGPIO0 / GP24                            [I] FM_SKTOCC_N                             G  I  H       1   1  1
// GP25                                     [O] FP_PWR_LED_N                            G  O  L       1   0  0
// GP26                                     [O] FM_LA_TRIGGER_N                         G  O  H       1   0  1
// GP27 / MGPIO6                            [I] FM_LAN_WAKE_N                           G  I  H       1   1  1
// TOTAL                                                                                              F   9  D
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP28 / MGPIO7                            [O] FM_MIC_MUX_RST_N                        G  O  H       1   0  1
// GP29 / SLP_WLAN# / MGPIO3                [I] FM_BIOS_IMAGE_SWAP_N                    G  I  H       1   1  1
// GP30 / SUSWARN# / SUSPWRDNACK / MGPIO1   [O] FM_EUP_LOT6                             G  O          1   0  0
// GP31 / ACPRESENT / MGPIO2                [I] SML1ALERT_N                             G  I  H       1   1  1
// TOTAL                                                                                              F   A  B
//
//                   AZTEC_GPIO_USE_SEL_VAL FF3DE93D
//                    AZTEC_GPIO_IO_SEL_VAL A9E2E7FF
//                       AZTEC_GPIO_LVL_VAL BD2D0800
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP32                                     [O] FM_BIOS_POST_COMPLETE_N                 G  O  L       1   0  0
// GP33 / DOCKEN#                           [I] FM_QPI_SLOW_MODE_N                      G  I  L       1   1  0
// GP34 / STPPCI#                           [I] FP_AUD_DETECT_N                         G  I          1   1  0
// GP35 / SATACLKREQ# / NMI#                [O] FM_NMI_EVENT_N                          G  O  H       1   0  1
// TOTAL                                                                                              F   6  8
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP36 / SATA2GP                           [I] FM_BIOS_ADV_FUNCTIONS                   G  I          1   1  0
// GP37 / SATA3GP                           [I] FM_ADR_TRIGGER_N                        G  I          1   1  0
// GP38 / SLOAD                             [O] SLOAD - SATA SGPIO LOAD  - RESERVED     N  I          0   1  0
// GP39 / SDATAOUT[0]                       [O] SDATAOUT[0] - RESERVED                  N  I          0   1  0
// TOTAL                                                                                              3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP40 / OC[1]#                            OC[1]# - RESERVED                           N  I          0   1  0
// GP41 / OC[2]#                            OC[2]# - RESERVED                           N  I          0   1  0
// GP42 / OC[3]#                            OC[3]# - RESERVED                           N  I          0   1  0
// GP43 / OC[4]#                            OC[4]# - RESERVED                           N  I          0   1  0
// TOTAL                                                                                              0   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP44                                     [O] FM_LAN0_DISABLE_N                       G  O  L       1   0  0
// GP45                                     Unused                                      G  I          1   1  0
// GP46                                     Unused                                      G  I          1   1  0
// GP47                                     Unused                                      G  I          1   1  0
// TOTAL                                                                                              F   E  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP48 / SDATAOUT[1]                       SDATAOUT[1] - RESERVED                      N  I          0   1  0
// GP49 / SATA5GP_PCIE_P2 / MGPIO10         Unused (Pulled Up)                          G  I  H       1   1  1
// GP50 / GSXCLK                            [O] Serial GPIO Xpansion Interface: CLOCK   N  I          0   1  0
// GP51 /GSXDOUT                            [O] Serial GPIO Xpansion Interface: DOUT    N  O  H       0   0  1
// TOTAL                                                                                              2   7  A
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP52/GSX_SLOAD                           [O] Serial GPIO Xpansion Interface: LOAD    N  I          0   1  0
// GP53 / GSXDIN                            [I] Serial GPIO Xpansion Interface: DIN     N  I          0   1  0
// GP54 / GSXSRESET#                        [O] Serial GPIO Xpansion Interface: RESET   N  I          0   1  0
// GP55                                     [I] FM_BIOS_RCVR_BOOT_N                     G  I          1   1  0
// TOTAL                                                                                              8   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP56                                     [I] FM_PLD_JTAG_TCK                         G  O          1   0  0
// GP57 / MGPIO5                            [I] FM_ME_RCVR_N                            G  I  H       1   1  1
// GP58 / SML1CLK / MGPIO11                 [I/O]SML1CLK - RESERVED                     N  I          0   1  0
// GP59 / OC[0]#                            OC[0]# - RESERVED                           N  I          0   1  0
// TOTAL                                                                                   I          3   E  2
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP60 / SML0ALERT# / MGPIO4               [O] SML0_ALERT_N [NM / BMC SKU]             N  I          0   1  0
// GP61                                     [O] FM_PLD_JTAG_TDI                         G  O          1   0  0
// GP62 / SUSCLK                            [O]SUSCLK -RESERVED-                        N  I          0   1  0
// GP63 / SLP_S5#                           [O] SLP_S5# - RESERVED                      N  I          0   1  0
// TOTAL                                                                                              2   D  0
//
//                  AZTEC_GPIO_USE_SEL2_VAL 2382F03F
//                   AZTEC_GPIO_IO_SEL2_VAL DEF7EFF6
//                      AZTEC_GPIO_LVL2_VAL 020A0008
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP64 / FLEXCLOCK[0]                      Unused                                      G  O          1   0  0
// GP65 / FLEXCLOCK[1]                      Unused                                      G  O          1   0  0
// GP66 / FLEXCLOCK[2]                      FLEXCLK[2]                                  N  O          0   0  0
// GP67 / FLEXCLOCK[3]                      FLEXCLK[3] - 48Mhz                          N  O          0   0  0
// TOTAL                                                                                              3   0  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP68 / TACH4                             [I] TACH 4 - RESERVED                       N  I          0   1  0
// GP69 / TACH5                             [I] TACH 5 - RESERVED                       N  I          0   1  0
// GP70 / TACH6 / USB3_P5#                  [I] TACH 6 - RESERVED                       N  I          0   1  0
// GP71 / TACH7 / USB3_P6#                  [I] TACH 7 - RESERVED                       N  I          0   1  0
// TOTAL                                                                                              0   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP72                                     Unused                                      G  I          1   1  0
// GP73                                     [O] PLF_JTAG+TMS                            G  O          1   0  0
// GP74 / SML1ALERT# /PCH_HOT#/ MGPIO8      [O]FM_PCH_BMC_THERMTRIP_N [BMC SKU]         G  O  H       1   0  1
// GP75 / SML1DATA / MGPIO12                [O] SML1DATA -RESERVED-                     N  I          0   1  0
// TOTAL                                                                                              7   9  4
//
//
//                  AZTEC_GPIO_USE_SEL3_VAL FFFFF703
//                   AZTEC_GPIO_IO_SEL3_VAL FFFFF9F0
//                      AZTEC_GPIO_LVL3_VAL 00000400

#define AZTEC_GPIO_USE_SEL_VAL           0xFF3DE93D
#define AZTEC_GPIO_IO_SEL_VAL            0xA9E2E7FF
#define AZTEC_GPIO_LVL_VAL               0xBD2D0800
#define AZTEC_GPIO_USE_SEL2_VAL          0x2382F03F
#define AZTEC_GPIO_IO_SEL2_VAL           0xDEF7EFF6
#define AZTEC_GPIO_LVL2_VAL              0x020A0008
#define AZTEC_GPIO_USE_SEL3_VAL          0xFFFFF703
#define AZTEC_GPIO_IO_SEL3_VAL           0xFFFFF9F0
#define AZTEC_GPIO_LVL3_VAL     0x00000400
#define AZTEC_GPIO_BLINK_VAL    0x00000000
#define AZTEC_GPIO_INV_VAL      0x00000000

//
// Grantley Inca City SRP1 Config
//
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP0 / BMBUSY#                            [I] IRQ_PCH_SCI_WHEA_N                      G  I          1   1  0
// GP1 / TACH1                              [I] FM_TACH1 - RESERVED                     N  I          0   1  0
// GP2 / PIRQ[E]#                           [I] FM_ERR0_N                               G  I          1   1  0
// GP3 / PIRQ[F]#                           [I] FM_ERR1_N                               G  I          1   1  0
// TOTAL                                                                                              D   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP4 / PIRQ[G]#                           [I] FM_BMC_PCH_SMI_LPC_N                    G  I          1   1  0
// GP5 / PIRQ[H]#                           [I] FM_CATERR_N                             G  I          1   1  0
// GP6 / TACH2                              [I] FM_TACH2 - RESERVED                     N  I          0   1  0
// GP7 / TACH3                              [I]FM_TACH3 - RESERVED                      N  I          0   1  0
// TOTAL                                                                                              3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP8 / OCS                                [I] IRQ_BMC_PCH_NMI                         G  I          1   1  0
// GP9 / OC[5]#                             OC[5]# - RESERVED                           N  I          0   1  0
// GP10 / OC[6]#                            OC[6]# - RESERVED                           N  I          0   1  0
// GP11 / SMBALERT#                         [O] FM_CPU_PCIE_SLOT_RST_N                  G  O  H       1   0  1
// TOTAL                                                                                              9   7  8
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP12 / LANPHYPC                          [I] FM_LINK_WIDTH_ID0                       G  I          1   1  0
// GP13 / DOCKAZRST#                        [I] FM_BMC_PCH_SCI_LPC_N                    G  I          1   1  0
// GP14 / OC7#                              [I] FM_RISER1_PRSNT_N                       G  I          1   1  0
// GP15                                     [I/O] FM_PCH_SATA_RAID_KEY                  G  I          1   1  0
// TOTAL                                                                                              F   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP16  / SATA4GP_PCIEP1 / MGPIO9          [O] FM_NM _THROTTLE_N                       G  O  H       1   0  1
// GP17 / TACH0                             [I] FM_TACH0 - RESERVED                     N  I          0   1  0
// GP18 / SRC1CLKRQ#                        Unused                                      G  I          1   1  0
// GP19 / SATA1GP                           Unused                                      G  I          1   1  0
// TOTAL                                                                                              D   E  1
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP20 / SMI#                              [O] FM_SMI_ACTIVE_N                         G  O  H       1   0  1
// SATA0GP / GP21                           Unused                                      G  I          1   1  0
// GP22 / SCLOCK                            SCLOCK  - SATA SGPIO SCLOCK - RESERVED      N  I          0   1  0
// GP23 / LDRQ1#                            [I] LDRQ[1]_N                               N  I          0   1  0
// TOTAL                                                                                              3   E  1
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// MGPIO0 / GP24                            Unused                                      G  I          1   1  0
// GP25                                     [O] FP_PWR_LED_N                            G  O  L       1   0  0
// GP26                                     [O] FM_LA_TRIGGER_N                         G  O  H       1   0  1
// GP27 / MGPIO6                            Unused                                      G  I          1   1  0
// TOTAL                                                                                              F   9  4
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP28 / MGPIO7                            [O] FM_MIC_MUX_RST_N                        G  O  H       1   0  1
// GP29 / SLP_WLAN# / MGPIO3                [I] FM_BIOS_IMAGE_SWAP_N                    G  I          1   1  0
// GP30 / SUSWARN# / SUSPWRDNACK / MGPIO1   Unused                                      G  I          1   1  0
// GP31 / ACPRESENT / MGPIO2                [I] SML1ALERT_N                             G  I          1   1  0
// TOTAL                                                                                              F   E  1
//
//                     INCA_GPIO_USESEL_VAL FF3DF93D
//                      INCA_GPIO_IOSEL_VAL E9EEF7FF
//                        INCA_GPIO_LVL_VAL 14110800
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP32                                     [O] FM_BIOS_POST_COMPLETE_N                 G  O  H       1   0  1
// GP33 / DOCKEN#                           [I] FM_QPI_SLOW_MODE_N                      G  I          1   1  0
// GP34 / STPPCI#                           [I] FM_RISER2_PRSNT_N                       G  I          1   1  0
// GP35 / SATACLKREQ# / NMI#                [O] FM_NMI_EVENT_N                          G  O  H       1   0  1
// TOTAL                                                                                              F   6  9
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP36 / SATA2GP                           [I] FM_BIOS_ADV_FUNCTIONS                   G  I          1   1  0
// GP37 / SATA3GP                           [I] FM_ADR_TRIGGER_N                        G  I          1   1  0
// GP38 / SLOAD                             [O] SLOAD - SATA SGPIO LOAD  - RESERVED     N  I          0   1  0
// GP39 / SDATAOUT[0]                       [O] SDATAOUT[0] - RESERVED                  N  I          0   1  0
// TOTAL                                                                                              3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP40 / OC[1]#                            OC[1]# - RESERVED                           N  I          0   1  0
// GP41 / OC[2]#                            OC[2]# - RESERVED                           N  I          0   1  0
// GP42 / OC[3]#                            OC[3]# - RESERVED                           N  I          0   1  0
// GP43 / OC[4]#                            OC[4]# - RESERVED                           N  I          0   1  0
// TOTAL                                                                                              0   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP44                                     [O] FM_LAN0_DISABLE_N                       G  O  H       1   0  1
// GP45                                     [O] FM_LAN1_DISABLE_N                       G  O  H       1   0  1
// GP46                                     Unused                                      G  I          1   1  0
// GP47                                     Unused                                      G  I          1   1  0
// TOTAL                                                                                              F   C  3
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP48 / SDATAOUT[1]                       SDATAOUT[1] - RESERVED                      N  I          0   1  0
// GP49 / SATA5GP_PCIE_P2 / MGPIO10         [I] FM_CPU_PROCHOT_N                        G  I          1   1  0
// GP50 / GSXCLK                            [O] Serial GPIO Xpansion Interface: CLOCK   N  I          0   1  0
// GP51 /GSXDOUT                            [O] Serial GPIO Xpansion Interface: DOUT    N  I          0   1  0
// TOTAL                                                                                              2   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP52/GSX_SLOAD                           [O] Serial GPIO Xpansion Interface: LOAD    N  I          0   1  0
// GP53 / GSXDIN                            [I] Serial GPIO Xpansion Interface: DIN     N  I          0   1  0
// GP54 / GSXSRESET#                        [O] Serial GPIO Xpansion Interface: RESET   N  I          0   1  0
// GP55                                     [I] FM_BIOS_RCVR_BOOT_N                     G  I          1   1  0
// TOTAL                                                                                              8   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP56                                     [I] FM_PLF_JTAG_TCK                         G  I          1   1  0
// GP57 / MGPIO5                            [I] FM_ME_RCVR_N                            G  I          1   1  0
// GP58 / SML1CLK / MGPIO11                 [I/O]SML1CLK - RESERVED                     N  I          0   1  0
// GP59 / OC[0]#                            OC[0]# - RESERVED                           N  I          0   1  0
// TOTAL                                                                                   I          3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP60 / SML0ALERT# / MGPIO4               [O] SML0_ALERT_N [NM / BMC SKU]             N  I          0   1  0
// GP61                                     [O] FM_PLD_JTAG_TDI                         G  O          1   0  0
// GP62 / SUSCLK                            [O]SUSCLK -RESERVED-                        N  I          0   1  0
// GP63 / SLP_S5#                           [O] SLP_S5# - RESERVED                      N  I          0   1  0
// TOTAL                                                                                              2   D  0
//
//                   INCA_GPIO_USE_SEL2_VAL 2382F03F
//                    INCA_GPIO_IO_SEL2_VAL DFFFCFF6
//                       INCA_GPIO_LVL2_VAL 00003009
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP64 / FLEXCLOCK[0]                      [I] FM_LINK_WIDTH_ID3                       G  I          1   1  0
// GP65 / FLEXCLOCK[1]                      [I] FM_LINK_WIDTH_ID1                       G  I          1   1  0
// GP66 / FLEXCLOCK[2]                      [I] FM_LINK_WIDTH_ID2                       G  I          1   1  0
// GP67 / FLEXCLOCK[3]                      FLEXCLK[3]                                  N  O          0   0  0
// TOTAL                                                                                              7   7  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP68 / TACH4                             [I] TACH 4 - RESERVED                       N  O          0   0  0
// GP69 / TACH5                             [I] TACH 5 - RESERVED                       N  I          0   1  0
// GP70 / TACH6 / USB3_P5#                  [I] TACH 6 - RESERVED                       N  I          0   1  0
// GP71 / TACH7 / USB3_P6#                  [I] TACH 7 - RESERVED                       N  I          0   1  0
// TOTAL                                                                                              0   E  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP72                                     Unused                                      G  I          1   1  0
// GP73                                     [O] PLD_JTAG_TMS                            G  O          1   0  0
// GP74 / SML1ALERT# /PCH_HOT#/ MGPIO8      [O]FM_PCH_BMC_THERMTRIP_N [BMC SKU]         G  O  H       1   0  1
// GP75 / SML1DATA / MGPIO12                [O] SML1DATA -RESERVED-                     N             0   0  0
// TOTAL                                                                                              7   1  4
//
//
//                   INCA_GPIO_USE_SEL3_VAL FFFFF707
//                    INCA_GPIO_IO_SEL3_VAL FFFFF1E7
//                       INCA_GPIO_LVL3_VAL 00000400

#define INCA_GPIO_USE_SEL_VAL            0xFF3DF93D
#define INCA_GPIO_IO_SEL_VAL             0xE9EEF7FF
#define INCA_GPIO_LVL_VAL                0x14110800
#define INCA_GPIO_USE_SEL2_VAL 0x2382F03F
#define INCA_GPIO_IO_SEL2_VAL            0xDFFFCFF6
#define INCA_GPIO_LVL2_VAL     0x00003009
#define INCA_GPIO_USE_SEL3_VAL           0xFFFFF707
#define INCA_GPIO_IO_SEL3_VAL            0xFFFFF1E7
#define INCA_GPIO_LVL3_VAL     0x00000400
#define INCA_GPIO_BLINK_VAL    0x00000000
#define INCA_GPIO_INV_VAL      0x00000000
//
// Grantley Half Width City SRP1 Config
//
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP0 / BMBUSY#                            [I] IRQ_PCH_SCI_WHEA_N                      G  I          1   1  0
// GP1 / TACH1                              [I]FM_TACH1 - RESERVED                      N  I          0   1  0
// GP2 / PIRQ[E]#                           [I] FM_ERR0_N                               G  I          1   1  0
// GP3 / PIRQ[F]#                           [I] FM_ERR1_N                               G  I          1   1  0
// TOTAL                                                                                              D   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP4 / PIRQ[G]#                           [I] FM_BMC_PCH_SMI_LPC_N                    G  I          1   1  0
// GP5 / PIRQ[H]#                           [I] FM_CATERR_N                             G  I          1   1  0
// GP6 / TACH2                              [I]FM_TACH2 - RESERVED                      G  I          1   1  0
// GP7 / TACH3                              [I]FM_TACH3 - RESERVED                      N  I          0   1  0
// TOTAL                                                                                              7   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP8 / OCS                                [I] IRQ_BMC_PCH_NMI                         G  I          1   1  0
// GP9 / OC[5]#                             OC[5]# - RESERVED                           N  I          0   1  0
// GP10 / OC[6]#                            OC[6]# - RESERVED                           N  I          0   1  0
// GP11 / SMBALERT#                         [O] FM_PCIE_GEN3_SLOT_RST_N                 G  O  H       1   0  1
// TOTAL                                                                                              9   7  8
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP12 / LANPHYPC                          [I] FM_LINK_WIDTH_ID0                       G  I          1   1  0
// GP13 / DOCKAZRST#                        [I] FM_BMC_PCH_SCI_LPC_N                    G  I          1   1  0
// GP14 / OC7#                              [I] FM_LINK_WIDTH_ID4                       G  I          1   1  0
// GP15                                     [I/O] FM_PCH_SATA_RAID_KEY                  G  I          1   1  0
// TOTAL                                                                                              F   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP16  / SATA4GP_PCIEP1 / MGPIO9          [O] FM_NM _THROTTLE_N                       G  O  H       1   0  1
// GP17 / TACH0                             [I] FM_TACH0 - RESERVED                     N  I          0   1  0
// GP18 / SRC1CLKRQ#                        Unused                                      G  I          1   1  0
// GP19 / SATA1GP                           [O] FM_PCH_PCIE_SLOT_RST_N                  G  O  H       1   0  1
// TOTAL                                                                                              D   6  9
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP20 / SMI#                              [O] FM_SMI_ACTIVE_N                         G  O  H       1   0  1
// SATA0GP / GP21                           Unused                                      G  I          1   1  0
// GP22 / SCLOCK                            SCLOCK  - SATA SGPIO SCLOCK - RESERVED      N  I          0   1  0
// GP23 / LDRQ1#                            [I] LDRQ[1]_N                               N  I          0   1  0
// TOTAL                                                                                              3   E  1
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// MGPIO0 / GP24                            Unused                                      G  I          1   1  0
// GP25                                     [O] FP_PWR_LED_N                            G  O          1   0  0
// GP26                                     [O] FM_LA_TRIGGER_N                         G  O  H       1   0  1
// GP27 / MGPIO6                            Unused                                      G  I          1   1  0
// TOTAL                                                                                              F   9  4
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP28 / MGPIO7                            [O] FM_MIC_MUX_RST_N                        G  O  H       1   0  1
// GP29 / SLP_WLAN# / MGPIO3                [I] FM_BIOS_IMAGE_SWAP_N                    G  I          1   1  0
// GP30 / SUSWARN# / SUSPWRDNACK / MGPIO1   Unused                                      G  I          1   1  0
// GP31 / ACPRESENT / MGPIO2                [I] SML1ALERT_N                             G  I          1   1  0
// TOTAL                                                                                              F   E  1
//
//               HALFWIDTH_GPIO_USE_SEL_VAL FF3DF97D
//                HALFWIDTH_GPIO_IO_SEL_VAL E9E6F7FF
//                   HALFWIDTH_GPIO_LVL_VAL 14190800
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP32                                     [O] FM_BIOS_POST_COMPLETE_N                 G  O  H       1   0  1
// GP33 / DOCKEN#                           [I] FM_QPI_SLOW_MODE_N                      G  I          1   1  0
// GP34 / STPPCI#                           SPARE                                       G  I          1   1  0
// GP35 / SATACLKREQ# / NMI#                [O] FM_NMI_EVENT_N                          G  O  H       1   0  1
// TOTAL                                                                                              F   6  9
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP36 / SATA2GP                           [I] FM_BIOS_ADV_FUNCTIONS                   G  I          1   1  0
// GP37 / SATA3GP                           [I] FM_ADR_TRIGGER_N                        G  I          1   1  0
// GP38 / SLOAD                             [O] SLOAD - SATA SGPIO LOAD  - RESERVED     N  I          0   1  0
// GP39 / SDATAOUT[0]                       [O] SDATAOUT[0] - RESERVED                  N  I          0   1  0
// TOTAL                                                                                              3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP40 / OC[1]#                            OC[1]# - RESERVED                           N  I          0   1  0
// GP41 / OC[2]#                            OC[2]# - RESERVED                           N  I          0   1  0
// GP42 / OC[3]#                            OC[3]# - RESERVED                           N  I          0   1  0
// GP43 / OC[4]#                            OC[4]# - RESERVED                           N  I          0   1  0
// TOTAL                                                                                              0   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP44                                     [O] FM_LAN0_DISABLE_N                       G  O  H       1   0  1
// GP45                                     [O] FM_LAN1_DISABLE_N                       G  O  H       1   0  1
// GP46                                     Unused                                      G  I          1   1  0
// GP47                                     Unused                                      G  I          1   1  0
// TOTAL                                                                                              F   C  3
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP48 / SDATAOUT[1]                       SDATAOUT[1] - RESERVED                      N  I          0   1  0
// GP49 / SATA5GP_PCIE_P2 / MGPIO10         [I] FM_CPU_PROCHOT_N                        G  I          1   1  0
// GP50 / GSXCLK                            [O] Serial GPIO Xpansion Interface: CLOCK   N  I          0   1  0
// GP51 /GSXDOUT                            [O] Serial GPIO Xpansion Interface: DOUT    N  I          0   1  0
// TOTAL                                                                                              2   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP52/GSX_SLOAD                           [O] Serial GPIO Xpansion Interface: LOAD    N  I          0   1  0
// GP53 / GSXDIN                            [I] Serial GPIO Xpansion Interface: DIN     N  I          0   1  0
// GP54 / GSXSRESET#                        [O] Serial GPIO Xpansion Interface: RESET   N  I          0   1  0
// GP55                                     [I] FM_BIOS_RCVR_BOOT_N                     G  I          1   1  0
// TOTAL                                                                                              8   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP56                                     [I] FM_PLD_JTAG_TCK                         G  I          1   1  0
// GP57 / MGPIO5                            [I] FM_ME_RCVR_N                            G  I          1   1  0
// GP58 / SML1CLK / MGPIO11                 [I/O]SML1CLK - RESERVED                     N  I          0   1  0
// GP59 / OC[0]#                            OC[0]# - RESERVED                           N  I          0   1  0
// TOTAL                                                                                   I          3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP60 / SML0ALERT# / MGPIO4               [O] SML0_ALERT_N [NM / BMC SKU]             N  I          0   1  0
// GP61                                     [O] FM_PLD_JTAG_TDI                         G  O          1   0  0
// GP62 / SUSCLK                            [O]SUSCLK -RESERVED-                        N  I          0   1  0
// GP63 / SLP_S5#                           [O] SLP_S5# - RESERVED                      N  I          0   1  0
// TOTAL                                                                                              2   D  0
//
//              HALFWIDTH_GPIO_USE_SEL2_VAL 2382F03F
//               HALFWIDTH_GPIO_IO_SEL2_VAL DFFFCFF6
//                  HALFWIDTH_GPIO_LVL2_VAL 00003009
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP64 / FLEXCLOCK[0]                      [I] FM_LINK_WIDTH_ID3                       G  I          1   1  0
// GP65 / FLEXCLOCK[1]                      [I] FM_LINK_WIDTH_ID1                       G  I          1   1  0
// GP66 / FLEXCLOCK[2]                      [I] FM_LINK_WIDTH_ID2                       G  I          1   1  0
// GP67 / FLEXCLOCK[3]                      FLEXCLK[3] -48Mhz                           N             0   0  0
// TOTAL                                                                                              7   7  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP68 / TACH4                             [I] TACH 4 - RESERVED                       N  O          0   0  0
// GP69 / TACH5                             [I] TACH 5 - RESERVED                       N  I          0   1  0
// GP70 / TACH6 / USB3_P5#                  [I] TACH 6 - RESERVED                       N  I          0   1  0
// GP71 / TACH7 / USB3_P6#                  [I] TACH 7 - RESERVED                       N  I          0   1  0
// TOTAL                                                                                              0   E  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP72                                     Unused                                      G  I          1   1  0
// GP73                                     [O] PLD_JTAG_TMS                            G  O          1   0  0
// GP74 / SML1ALERT# /PCH_HOT#/ MGPIO8      [O]FM_PCH_BMC_THERMTRIP_N [BMC SKU]         G  O  H       1   0  1
// GP75 / SML1DATA / MGPIO12                [O] SML1DATA -RESERVED-                     N             0   0  0
// TOTAL                                                                                              7   1  4
//
//
//              HALFWIDTH_GPIO_USE_SEL3_VAL FFFFF707
//               HALFWIDTH_GPIO_IO_SEL3_VAL FFFFF1E7
//                  HALFWIDTH_GPIO_LVL3_VAL 00000400

#define HALFWIDTH_GPIO_USE_SEL_VAL  0xFF3DF97D
   #define HALFWIDTH_GPIO_IO_SEL_VAL        0xE9E6F7FF
#define HALFWIDTH_GPIO_LVL_VAL      0x14190800
#define HALFWIDTH_GPIO_USE_SEL2_VAL 0x2382F03F
   #define HALFWIDTH_GPIO_IO_SEL2_VAL       0xDFFFCFF6
#define HALFWIDTH_GPIO_LVL2_VAL     0x00003009
   #define HALFWIDTH_GPIO_USE_SEL3_VAL      0xFFFFF707
   #define HALFWIDTH_GPIO_IO_SEL3_VAL       0xFFFFF1E7
#define HALFWIDTH_GPIO_LVL3_VAL     0x00000400
#define HALFWIDTH_GPIO_BLINK_VAL    0x00000000
#define HALFWIDTH_GPIO_INV_VAL      0x00000000
//
// Grantley HEDT Config
//
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP0 / BMBUSY#                            Unused                                      G  I          1   1  0
// GP1 / TACH1                              Unused                                      N  I          0   1  0
// GP2 / PIRQ[E]#                           [I] ERROR0_N                                G  I          1   1  0
// GP3 / PIRQ[F]#                           [I] ERROR1_N                                G  I          1   1  0
// TOTAL                                                                                              D   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP4 / PIRQ[G]#                           Unused                                      G  I          1   1  0
// GP5 / PIRQ[H]#                           [I] FM_CATERR_N                             G  I          1   1  0
// GP6 / TACH2                              Unused                                      N  I          0   1  0
// GP7 / TACH3                              Unused                                      N  I          0   1  0
// TOTAL                                                                                              3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP8 / OCS                                [O] PCH_OVERCLOCK_STRAP_N                   G  O  H       1   0  1
// GP9 / OC[5]#                             OC[5]# - RESERVED                           N  I          0   1  0
// GP10 / OC[6]#                            OC[6]# - RESERVED                           N  I          0   1  0
// GP11 / SMBALERT#                         [O] FM_PCIE_GEN3_SLOT_RST_N                 G  O  H       1   0  1
// TOTAL                                                                                              9   6  9
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP12 / LANPHYPC                          [N] LAN_DISABLE_N                           N  I          0   1  0
// GP13 / DOCKAZRST#                        Unused                                      G  I          1   1  0
// GP14 / OC7#                              [I] TBT_HOTPLUG                             G  I          1   1  0
// GP15                                     [O] V_SM_C01_LOT26                          G  O  L       1   0  0
// TOTAL                                                                                              E   7  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP16  / SATA4GP_PCIEP1 / MGPIO9          [I] SATA_EXPRESS_DETECT_0                   G  I          1   1  0
// GP17 / TACH0                             Unused                                      N  I          0   1  0
// GP18 / SRC1CLKRQ#                        Unused                                      G  I          1   1  0
// GP19 / SATA1GP                           [O] PCIE_PCH_SLOT_GEN2_RST_N                G  O  H       1   0  1
// TOTAL                                                                                              D   7  8
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP20 / SMI#                              Unused                                      G  O  H       1   0  1
// SATA0GP / GP21                           Unused                                      G  I          1   1  0
// GP22 / SCLOCK                            SCLOCK  - SATA SGPIO SCLOCK - RESERVED      N  I          0   1  0
// GP23 / LDRQ1#                            [I] LDRQ[1]_N                               N  I          0   1  0
// TOTAL                                                                                              3   E  1
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// MGPIO0 / GP24                            [I] SKTOCC_N                                G  I          1   1  0
// GP25                                     [O] FP_PWR_LED_N                            G  O  L       1   0  0
// GP26                                     [O] FM_LA_TRIGGER_N                         G  O  H       1   0  1
// GP27 / MGPIO6                            [I] LANWAKE_N                               G  I          1   1  0
// TOTAL                                                                                              F   9  4
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP28 / MGPIO7                            [O] V_SM_C23_LOT26                          G  O          1   0  0
// GP29 / SLP_WLAN# / MGPIO3                [N] SLP_WLAN_N                              N  O          0   0  0
// GP30 / SUSWARN# / SUSPWRDNACK / MGPIO1   [N][O] SUS_WARN_N                           N  O          0   0  0
// GP31 / ACPRESENT / MGPIO2                Unused                                      G  I          1   1  0
// TOTAL                                                                                              9   8  0
//
//                    HEDT_GPIO_USE_SEL_VAL 9F3DE93D
//                     HEDT_GPIO_IO_SEL_VAL 89E776FF
//                        HEDT_GPIO_LVL_VAL 04180900
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP32                                     [O] BIOS_POST_COMPLETE_N                    G  O  H       1   0  1
// GP33 / DOCKEN#                           Unused                                      G  I          1   1  0
// GP34 / STPPCI#                           [I] FP_AUD_DETECT_N                         G  I          1   1  0
// GP35 / SATACLKREQ# / NMI#                Unused                                      G  I          1   1  0
// TOTAL                                                                                              F   E  1
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP36 / SATA2GP                           [I] BIOS_ADVANCE_FUNCTIONS                  G  I          1   1  0
// GP37 / SATA3GP                           Unused                                      G  I          1   1  0
// GP38 / SLOAD                             Unused                                      N  I          0   1  0
// GP39 / SDATAOUT[0]                       Unused                                      N  I          0   1  0
// TOTAL                                                                                              3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP40 / OC[1]#                            OC[1]# - RESERVED                           N  I          0   1  0
// GP41 / OC[2]#                            OC[2]# - RESERVED                           N  I          0   1  0
// GP42 / OC[3]#                            OC[3]# - RESERVED                           N  I          0   1  0
// GP43 / OC[4]#                            OC[4]# - RESERVED                           N  I          0   1  0
// TOTAL                                                                                              0   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP44                                     Unused                                      G  I          1   1  0
// GP45                                     Unused                                      G  I          1   1  0
// GP46                                     Unused                                      G  I          1   1  0
// GP47                                     [I] OVERCLOCK_SAFE_CONFIG                   G  I          1   1  0
// TOTAL                                                                                              F   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP48 / SDATAOUT[1]                       Unused                                      N  I          0   1  0
// GP49 / SATA5GP_PCIE_P2 / MGPIO10         [I] SATA_EXPRESS_DETECT_0                   G  I          1   1  0
// GP50 / GSXCLK                            [O] Serial GPIO Xpansion Interface: CLOCK   N  I          0   1  0
// GP51 /GSXDOUT                            [O] Serial GPIO Xpansion Interface: DOUT    N  I          0   1  0
// TOTAL                                                                                              2   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP52/GSX_SLOAD                           [O] Serial GPIO Xpansion Interface: LOAD    N  I          0   1  0
// GP53 / GSXDIN                            [I] Serial GPIO Xpansion Interface: DIN     N  I          0   1  0
// GP54 / GSXSRESET#                        [O] Serial GPIO Xpansion Interface: RESET   N  I          0   1  0
// GP55                                     [I] BIOS_RECOVERY_N                         G  I          1   1  0
// TOTAL                                                                                              8   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP56                                     [O] VIN_BOOST                               G  O  L       1   0  0
// GP57 / MGPIO5                            [I] FM_ME_RCVR_N                            G  I          1   1  0
// GP58 / SML1CLK / MGPIO11                 Unused                                      N  I          0   1  0
// GP59 / OC[0]#                            OC[0]# - RESERVED                           N  I          0   1  0
// TOTAL                                                                                              3   E  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP60 / SML0ALERT# / MGPIO4               [I] SML0_ALERT_N                            N  I          0   1  0
// GP61                                     Unused                                      G  I          1   1  0
// GP62 / SUSCLK                            [O]SUSCLK -RESERVED-                        N  I          0   1  0
// GP63 / SLP_S5#                           [O] SLP_S5# - RESERVED                      N  I          0   1  0
// TOTAL                                                                                              2   F  0
//
//                   HEDT_GPIO_USE_SEL2_VAL 2382F03F
//                    HEDT_GPIO_IO_SEL2_VAL FEFFFFFE
//                       HEDT_GPIO_LVL2_VAL 00000001
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP64 / FLEXCLOCK[0]                      [N][O] 14.318Mhz FLEX CLK                   N  O          0   0  0
// GP65 / FLEXCLOCK[1]                      Unused                                      G  I          1   1  0
// GP66 / FLEXCLOCK[2]                      Unused                                      G  I          1   1  0
// GP67 / FLEXCLOCK[3]                      Unused                                      G  I          1   1  0
// TOTAL                                                                                              E   E  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP68 / TACH4                             [I] TACH 4 - RESERVED                       N  O          0   0  0
// GP69 / TACH5                             [I] TACH 5 - RESERVED                       N  I          0   1  0
// GP70 / TACH6 / USB3_P5#                  [I] TACH 6 - RESERVED                       N  I          0   1  0
// GP71 / TACH7 / USB3_P6#                  [I] TACH 7 - RESERVED                       N  I          0   1  0
// TOTAL                                                                                              0   E  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP72                                     Unused                                      G  I          1   1  0
// GP73                                     Unused                                      G  I          1   1  0
// GP74 / SML1ALERT# /PCH_HOT#/ MGPIO8      [N] PCH_HOT_N                               N  O          0   0  0Todo: Check softstrap for HEDT
// GP75 / SML1DATA / MGPIO12                [O] SML1DATA -RESERVED-                     N             0   0  0
// TOTAL                                                                                              3   3  0
//
//
//                   HEDT_GPIO_USE_SEL3_VAL FFFFF30E
//                    HEDT_GPIO_IO_SEL3_VAL FFFFF3EE
//                       HEDT_GPIO_LVL3_VAL 00000000

#define HEDT_GPIO_USE_SEL_VAL            0x9F3DE93D
#define HEDT_GPIO_IO_SEL_VAL             0x89E776FF
#define HEDT_GPIO_LVL_VAL                0x04180900
#define HEDT_GPIO_USE_SEL2_VAL           0x2382F03F
#define HEDT_GPIO_IO_SEL2_VAL            0xFEFFFFFE
#define HEDT_GPIO_LVL2_VAL               0x00000001
#define HEDT_GPIO_USE_SEL3_VAL           0xFFFFF30E
#define HEDT_GPIO_IO_SEL3_VAL            0xFFFFF3EE
#define HEDT_GPIO_LVL3_VAL               0x00000000
#define HEDT_GPIO_BLINK_VAL              0x00000000
#define HEDT_GPIO_INV_VAL                0x00000000
//
// Arandas VVP Config
//
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP0 / BMBUSY#                            C33_SINAI_PCH_GPIO0                         G  I          1   1  0
// GP1 / TACH1                              Unused                                      N  I          0   1  0
// GP2 / PIRQ[E]#                           C33_CPU_ERROR0_N                            G  I          1   1  0
// GP3 / PIRQ[F]#                           C33_CPU_ERROR1_N                            G  I          1   1  0
// TOTAL                                                                                              D   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP4 / PIRQ[G]#                           C033_IBMC_PCH_SMI_LPC_N                     G  I          1   1  0
// GP5 / PIRQ[H]#                           C33_CPU_CATERR_N                            G  I          1   1  0
// GP6 / TACH2                              Unused                                      N  I          0   1  0
// GP7 / TACH3                              Unused                                      N  I          0   1  0
// TOTAL                                                                                              3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP8 / OCS                                C033_IBMC_PCH_SMI_LPC_N                     G  I          1   1  0
// GP9 / OC[5]#                             C033_USB_OC5B_N                             N  I          0   1  0
// GP10 / OC[6]#                            C033_USB_OC6B_N                             N  I          0   1  0
// GP11 / SMBALERT#                         Unused                                      G  I          1   1  0
// TOTAL                                                                                              9   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP12 / LANPHYPC                          Unused                                      G  I          1   1  0
// GP13 / DOCKAZRST#                        C033_IBMC_PCH_SMI_LPC_N                     G  I          1   1  0
// GP14 / OC7#                              Unused                                      G  I          1   1  0
// GP15                                     Unused                                      G  I          1   1  0
// TOTAL                                                                                              F   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP16  / SATA4GP_PCIEP1 / MGPIO9          C33_QPI_SLOW_MODE_N                         G  I          1   1  0* TODO: Verify with ARANDAS schematic/engineer
// GP17 / TACH0                             Unused                                      N  I          0   1  0
// GP18 / SRC1CLKRQ#                        Unused                                      G  I          1   1  0
// GP19 / SATA1GP                           Unused                                      G  I          1   1  0
// TOTAL                                                                                              D   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP20 / SMI#                              C33_PCH_IBMC_SMI_ACTIVE_N                   G  O  H       1   0  1
// SATA0GP / GP21                           Unused                                      G  I          1   1  0
// GP22 / SCLOCK                            Unused                                      N  I          0   1  0
// GP23 / LDRQ1#                            C33_PCH_LPC_DRQ1_N                          N  I          0   1  0
// TOTAL                                                                                              3   E  1
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// MGPIO0 / GP24                            Unused                                      G  I          1   1  0
// GP25                                     Unused                                      G  I          1   1  0
// GP26                                     Spare NC                                    G  I          1   1  0
// GP27 / MGPIO6                            Unused                                      G  I          1   1  0
// TOTAL                                                                                              F   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP28 / MGPIO7                            Unused                                      G  I          1   1  0
// GP29 / SLP_WLAN# / MGPIO3                C33_PCH_NM_THROTTLE                         G  O          1   0  0
// GP30 / SUSWARN# / SUSPWRDNACK / MGPIO1   C33_SUS_WARN_N                              N  I          0   1  0
// GP31 / ACPRESENT / MGPIO2                CO33_SML1ALERT_N                            G  I          1   1  0
// TOTAL                                                                                              B   D  0
//
//                 ARANDAS_GPIO_USE_SEL_VAL BF3DF93D
//                  ARANDAS_GPIO_IO_SEL_VAL DFEFFFFF
//                     ARANDAS_GPIO_LVL_VAL 00100000
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP32                                     CO33_PCH_MULTIPLE_PCI_MASTER_N              G  O  H       1   0  1
// GP33 / DOCKEN#                           Unused                                      G  I          1   1  0
// GP34 / STPPCI#                           Unused                                      G  I          1   1  0
// GP35 / SATACLKREQ# / NMI#                C33_PCH_IBMC_NMI_EVENT_N                    G  O  H       1   0  1
// TOTAL                                                                                              F   6  9
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP36 / SATA2GP                           C33_BIOS_ADV_FUNCTIONS                      G  I          1   1  0
// GP37 / SATA3GP                           C033_PS_PWRGD                               G  I          1   1  0
// GP38 / SLOAD                             Unused                                      N  I          0   1  0
// GP39 / SDATAOUT[0]                       Unused                                      N  I          0   1  0
// TOTAL                                                                                              3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP40 / OC[1]#                            CO33_USB_OC1B_N                             N  I          0   1  0
// GP41 / OC[2]#                            CO33_USB_OC2B_N                             N  I          0   1  0
// GP42 / OC[3]#                            CO33_USB_OC3B_N                             N  I          0   1  0
// GP43 / OC[4]#                            CO33_USB_OC4B_N                             N  I          0   1  0
// TOTAL                                                                                              0   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP44                                     Unused                                      G  I          1   1  0
// GP45                                     Unused                                      G  I          1   1  0
// GP46                                     Unused                                      G  I          1   1  0
// GP47                                     Unused                                      G  I          1   1  0
// TOTAL                                                                                              F   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP48 / SDATAOUT[1]                       Unused                                      N             0   0  0
// GP49 / SATA5GP_PCIE_P2 / MGPIO10         C33_PCH_IBMC_BIOS_POST_CMPLT_N              G  O          1   0  0
// GP50 / GSXCLK                            C33_PCI_REQ1_N_GSXCLK                       N             0   0  0
// GP51 /GSXDOUT                            Unused                                      N             0   0  0
// TOTAL                                                                                              2   0  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP52/GSX_SLOAD                           C33_PCI_REQ2_N_GSXSLOAD                     N             0   0  0
// GP53 / GSXDIN                            C33_PCI_GNT2_N_GSXDIN                       N             0   0  0
// GP54 / GSXSRESET#                        Unused                                      N             0   0  0
// GP55                                     C33_BIOS_RCVR_BOOT_N                        G  I          1   1  0
// TOTAL                                                                                              8   8  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP56                                     Unused                                      G  I          1   1  0
// GP57 / MGPIO5                            C33_PCH_ME_RCVR_N                           G  I          1   1  0
// GP58 / SML1CLK / MGPIO11                 CO33_PRI_SCL                                N  I          0   1  0
// GP59 / OC[0]#                            CO33_USB_OC0B_N                             N  I          0   1  0
// TOTAL                                                                                   I          3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP60 / SML0ALERT# / MGPIO4               CO33_PCH_IBMC_SML0_ALERT_N                  N             0   0  0
// GP61                                     C33_PCH_SUS_STAT_N                          G  O          1   0  0
// GP62 / SUSCLK                            To RSTPLD                                   N  I          0   1  0
// GP63 / SLP_S5#                           To RSTPLD and FP LA                         N  I          0   1  0
// TOTAL                                                                                              2   C  0
//
//                ARANDAS_GPIO_USE_SEL2_VAL 2382F03F
//                 ARANDAS_GPIO_IO_SEL2_VAL CF80FFF6
//                    ARANDAS_GPIO_LVL2_VAL 00000009
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP64 / FLEXCLOCK[0]                      Unused                                      G  I          1   1  0
// GP65 / FLEXCLOCK[1]                      Unused                                      G  I          1   1  0
// GP66 / FLEXCLOCK[2]                      Unused                                      G  I          1   1  0
// GP67 / FLEXCLOCK[3]                      C33_PCH_SUS_STAT_N                          G  I          1   1  0
// TOTAL                                                                                              F   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP68 / TACH4                             [I] TACH 4 - RESERVED                       N  I          0   1  0
// GP69 / TACH5                             [I] TACH 5 - RESERVED                       N  I          0   1  0
// GP70 / TACH6 / USB3_P5#                  [I] TACH 6 - RESERVED                       N  I          0   1  0
// GP71 / TACH7 / USB3_P6#                  [I] TACH 7 - RESERVED                       N  I          0   1  0
// TOTAL                                                                                              0   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP72                                     Unused                                      G  I          1   1  0
// GP73                                     Unused                                      G  I          1   1  0
// GP74 / SML1ALERT# /PCH_HOT#/ MGPIO8      CO33_PCH_IBMC_THERMTRIP_N                   G  O  H       1   0  1
// GP75 / SML1DATA / MGPIO12                C033_PRI_SDA                                N             0   0  0
// TOTAL                                                                                              7   3  4
//
//
//                ARANDAS_GPIO_USE_SEL3_VAL FFFFF70F
//                 ARANDAS_GPIO_IO_SEL3_VAL FFFFF3FF
//                    ARANDAS_GPIO_LVL3_VAL 00000400

   #define ARANDAS_GPIO_USE_SEL_VAL         0xBF2DF93D
   #define ARANDAS_GPIO_IO_SEL_VAL          0xE9E6F7FF
   #define ARANDAS_GPIO_LVL_VAL             0x14190800
   #define ARANDAS_GPIO_USE_SEL2_VAL        0x2382F03F
   #define ARANDAS_GPIO_IO_SEL2_VAL         0xDFFFFFF6
   #define ARANDAS_GPIO_LVL2_VAL            0x00000009
   #define ARANDAS_GPIO_USE_SEL3_VAL        0xFFFFF707
   #define ARANDAS_GPIO_IO_SEL3_VAL         0xFFFFF3FF
   #define ARANDAS_GPIO_LVL3_VAL            0x00000400
   #define ARANDAS_GPIO_BLINK_VAL           0x00000000
   #define ARANDAS_GPIO_INV_VAL             0x00000000

//
// Grantley Emerald Point CRB Config
//
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP0 / BMBUSY#                            [I] IRQ_PCH_SCI_WHEA_N                      G  I       1   1  0
// GP1 / TACH1                              [I]FM_TACH1 - RESERVED                      N  I       0   1  0
// GP2 / PIRQ[E]#                           [I] FM_ERR0_N                               G  I       1   1  0
// GP3 / PIRQ[F]#                           [I] FM_ERR1_N                               G  I       1   1  0
// TOTAL                                                                                           D   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP4 / PIRQ[G]#                           [I] FM_BMC_PCH_SMI_LPC_N                    G  I       1   1  0
// GP5 / PIRQ[H]#                           [I] FM_CATERR_N                             G  I       1   1  0
// GP6 / TACH2                              [I] FM_TACH2 - RESERVED                     N  I       0   1  0
// GP7 / TACH3                              [I]FM_TACH3 - RESERVED                      N  I       0   1  0
// TOTAL                                                                                           3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP8 / OCS                                [I] IRQ_BMC_PCH_NMI                         G  I       1   1  0
// GP9 / OC[5]#                             [I] FM_SMI_INT_CPLD_N                       G  I       1   1  0
// GP10 / OC[6]#                            OC[6]# - RESERVED                           N  I       0   1  0
// GP11 / SMBALERT#                         [O] FM_CPU_PCIE_SLOT_RST_N                  G  O  H    1   0  1
// TOTAL                                                                                           B   7  8
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP12 / LANPHYPC                          [I] AC_GOOD_LATCHED                         G  I       1   1  0
// GP13 / DOCKAZRST#                        [I] FM_BMC_PCH_SCI_LPC_N                    G  I       1   1  0
// GP14 / OC7#                              [O] PBG_CPLD_CNTL0                             G  O   H   1   0  0
// GP15                                     [O] PBG_CPLD_CNTL1                 G  O   H   1   0  0
// TOTAL                                                                                           F   3  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP16  / SATA4GP_PCIEP1 / MGPIO9          [O] FM_NM _THROTTLE_N                       G  O  H    1   0  1
// GP17 / TACH0                             [I]FM_TACH0 - RESERVED                      N  I       0   1  0
// GP18 / SRC1CLKRQ#                        Unused                                      G  I       1   1  0
// GP19 / SATA1GP                           [O] FM_PCH_PCIE_SLOT_RST_N                  G  O  H    1   0  1
// TOTAL                                                                                           D   6  9
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP20 / SMI#                              [O] FM_SMI_ACTIVE_N                         G  O  H    1   0  1
// SATA0GP / GP21                           Unused                                      G  I       1   1  0
// GP22 / SCLOCK                            SCLOCK  - SATA SGPIO SCLOCK - RESERVED      N  I       0   1  0
// GP23 / LDRQ1#                            [I] LDRQ[1]_N                               N  I       0   1  0
// TOTAL                                                                                           3   E  1
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// MGPIO0 / GP24                            Unused                                      G  I       1   1  0
// GP25                                     [O] FP_PWR_LED_N                            G  O  L    1   0  0
// GP26                                     [O] FM_LA_TRIGGER_N                         G  O  H    1   0  1
// GP27 / MGPIO6                            Unused                                      G  I       1   1  0
// TOTAL                                                                                           F   9  4
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP28 / MGPIO7                            [O] FM_MIC_MUX_RST_N                        G  O  H    1   0  1
// GP29 / SLP_WLAN# / MGPIO3                [I] FM_BIOS_IMAGE_SWAP_N                    G  I       1   1  0
// GP30 / SUSWARN# / SUSPWRDNACK / MGPIO1   [O] FP_ID_BTN_PCH_N                         G  I  H    1   1  0
// GP31 / ACPRESENT / MGPIO2                [I] SML1ALERT_N                             G  I       1   1  0
// TOTAL                                                                                           F   E  1
//
//                   EPT_GPIO_USE_SEL_VAL FF3DFB3D
//                   EPT_GPIO_IO_SEL_VAL E9E637FF
//                   EPT_GPIO_LVL_VAL 14190800
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP32                                     [O] FM_BIOS_POST_COMPLETE_N                 G  O  H    1   0  1
// GP33 / DOCKEN#                           [I] FM_QPI_SLOW_MODE_N                      G  I       1   1  0
// GP34 / STPPCI#                           SPARE                                       G  I       1   1  0
// GP35 / SATACLKREQ# / NMI#                [O] FM_NMI_EVENT_N                          G  O  H    1   0  1
// TOTAL                                                                                           F   6  9
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP36 / SATA2GP                           [I] FM_BIOS_ADV_FUNCTIONS                   G  I       1   1  0
// GP37 / SATA3GP                           [I] FM_ADR_TRIGGER_N                        G  I       1   1  0
// GP38 / SLOAD                             [O] SLOAD - SATA SGPIO LOAD  - RESERVED     N  I       0   1  0
// GP39 / SDATAOUT[0]                       [O] SDATAOUT[0] - RESERVED                  N  I       0   1  0
// TOTAL                                                                                           3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP40 / OC[1]#                            OC[1]# - RESERVED                           N  I       0   1  0
// GP41 / OC[2]#                            [I] ADR_STATUS                              G  I   H   1   1  0
// GP42 / OC[3]#                            OC[3]# - RESERVED                           N  I       0   1  0
// GP43 / OC[4]#                            [O] MCU_INIT                                G  O   H   1   0  0
// TOTAL                                                                                          A   7  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP44                                     [O] FM_LAN0_DISABLE_N                       G  O  H    1   0  1
// GP45                                     [O] FM_LAN1_DISABLE_N                       G  O  H    1   0  1
// GP46                                     [O] FM_LAN2_DISABLE_N                       G  O  H    1   0  1
// GP47                                     [O] FM_LAN2_DISABLE_N                       G  O  H    1   0  1
// TOTAL                                                                                           F   0  F
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP48 / SDATAOUT[1]                       SDATAOUT[1] - RESERVED                      N  I       0   1  0
// GP49 / SATA5GP_PCIE_P2 / MGPIO10         [I] FM_CPU_PROCHOT_N                        G  I       1   1  0
// GP50 / GSXCLK                            [O] Serial GPIO Xpansion Interface: CLOCK   N  I       0   1  0
// GP51 /GSXDOUT                            [O] Serial GPIO Xpansion Interface: DOUT    N  I       0   1  0
// TOTAL                                                                                           2   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP52/GSX_SLOAD                           [O] Serial GPIO Xpansion Interface: LOAD    N  I       0   1  0
// GP53 / GSXDIN                            [I] Serial GPIO Xpansion Interface: DIN     N  I       0   1  0
// GP54 / GSXSRESET#                        [O] Serial GPIO Xpansion Interface: RESET   N  I       0   1  0
// GP55                                     [I] FM_BIOS_RCVR_BOOT_N                     G  I       1   1  0
// TOTAL                                                                                           8   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP56                                     [I] FM_PLD_JTAG_TCK                         G  I       1   1  0
// GP57 / MGPIO5                            [I] FM_ME_RCVR_N                            G  I       1   1  0
// GP58 / SML1CLK / MGPIO11                 [I/O]SML1CLK - RESERVED                     N  I       0   1  0
// GP59 / OC[0]#                            OC[0]# - RESERVED                           N  I       0   1  0
// TOTAL                                                                                   I       3   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP60 / SML0ALERT# / MGPIO4               [O] SML0_ALERT_N [NM / BMC SKU]             N  O       0   0  0
// GP61                                     (O) FM_PLD_JTAG_TDI                         G  O       1   0  0
// GP62 / SUSCLK                            [O]SUSCLK -RESERVED-                        N  I       0   1  0
// GP63 / SLP_S5#                           [O] SLP_S5# - RESERVED                      N  I       0   1  0
// TOTAL                                                                                           2   C  0
//
//                  EPT_GPIO_USE_SEL2_VAL 2382FA3F
//                  EPT_GPIO_IO_SEL2_VAL CFFF07F6
//                  EPT_GPIO_LVL2_VAL 0000F009
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP64 / FLEXCLOCK[0]                      [I] FM_LINK_WIDTH_ID3                       G  I       1   1  0
// GP65 / FLEXCLOCK[1]                      [I] FM_LINK_WIDTH_ID1                       G  I       1   1  0
// GP66 / FLEXCLOCK[2]                      [I] FM_LINK_WIDTH_ID2                       G  I       1   1  0
// GP67 / FLEXCLOCK[3]                      FLEXCLK[3] 48- MHz                          N  I       0   1  0
// TOTAL                                                                                           7   F  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP68 / TACH4                             [I] TACH 4 - RESERVED                       N  O       0   0  0
// GP69 / TACH5                             [I] TACH 5 - RESERVED                       N  I       0   1  0
// GP70 / TACH6 / USB3_P5#                  [I] TACH 6 - RESERVED                       N  I       0   1  0
// GP71 / TACH7 / USB3_P6#                  [I] TACH 7 - RESERVED                       N  I       0   1  0
// TOTAL                                                                                           0   E  0
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO  LVL
// GP72                                     [O] FM_CACHE_FLUSH_COMPLETE                 G  O  H    1   1  0
// GP73                                     (O) PLD_JTAG_TMS                            G  I       1   1  0
// GP74 / SML1ALERT# /PCH_HOT#/ MGPIO8      [O]FM_PCH_BMC_THERMTRIP_N [BMC SKU]         G  O  H    1   0  1
// GP75 / SML1DATA / MGPIO12                [O] SML1DATA -RESERVED-                     N          0   0  0
// TOTAL                                                                                           7   3  4
//
//
//                  EPT_GPIO_USE_SEL3_VAL FFFFF707
//                   EPT_GPIO_IO_SEL3_VAL FFFFF3EF
//                      EPT_GPIO_LVL3_VAL 00000400

#define EPT_GPIO_USE_SEL_VAL  0xFF3DFB3D
#define EPT_GPIO_IO_SEL_VAL   0xE9E637FF
#define EPT_GPIO_LVL_VAL      0x14190800
#define EPT_GPIO_USE_SEL2_VAL 0x2382FA3F
#define EPT_GPIO_IO_SEL2_VAL  0xCFFF07F6
#define EPT_GPIO_LVL2_VAL     0x0000F009
#define EPT_GPIO_USE_SEL3_VAL 0xFFFFF707
#define EPT_GPIO_IO_SEL3_VAL  0xFFFFF3EF
#define EPT_GPIO_LVL3_VAL     0x00000400
#define EPT_GPIO_BLINK_VAL    0x00000000
#define EPT_GPIO_INV_VAL      0x00000000


#ifndef TypePlatformDefault



#ifdef GRANGEVILLE_FLAG


//
// Grangeville BeverlyCove(CRB) Config
//
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
// GP0 / BMBUSY#                            [I] IRQ_PCH_SCI_WHEA_N (10K PU 3.3V)        G  I  H    1   1   1
// GP1 / TACH1                              [I] Not Assigned                            G  O       1   0   0
// GP2 / PIRQ[E]#                           [I] ERROR0_N                                G  I       1   1   0
// GP3 / PIRQ[F]#                           [I] ERROR1_N                                G  I       1   1   0
// TOTAL                                                                                           F   D   1
//                                 
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
// GP4 / PIRQ[G]#                           [I] CPU2PCH_THROT                           G   I   H  1   1   1
// GP5 / PIRQ[H]#                           [I] CPU_CATERR_N                            G   I      1   1   0
// GP6 / TACH2                              Not Assigned                                G   O      1   0   0
// GP7 / TACH3                              Not assigned                                G   O      1   0   0
// TOTAL                                                                                           F   3   1
//                                 
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
// GP8 / OCS                                Not Assigned                                G   O      1   0   0
// GP9 / OC[5]#                             Not Assigned                                G   O      1   0   0
// GP10 / OC[6]#                            Not Assigned                                G   O      1   0   0
// GP11 / SMBALERT#                         [I] SMBAlert                                N   I      0   1   0
// TOTAL                                                                                           7   8   0
//                                 
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
// GP12 / LANPHYPC                          [O] Lan_Phy_pwr_CTRL                        N   O      0   0   0
// GP13 / DOCKAZRST#                        Unused                                      G   I      1   1   0
// GP14 / OC7#                              Not Assigned                                G   I      1   1   0
// GP15                                     [O] Vddq_voltage_sel                        G   I      1   1   0
// TOTAL                                                                                           E   E   0
//                                 
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
// GP16 / SATA4                             [O] CPU_THROTTLE                            G   O   L  1   0   0
// TACH0                                    Not Assigned                                G   O      1   0   0
// SRC1CLKRQ                                Not assigned                                G   O   L  1   0   0
// SATA1                                    Not assigned                                G   I   L  1   1   0
// TOTAL                                                                                           F   8   0
//                                 
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
// SMI / SRC2CLKRQ                          [O] SMI_Active_N                            N   O   H  0   0   1
// SATA0                                    Not Assigned                                G   O      1   0   0
// SGPIO_SCLK                               [I] PU_SGPIO22_SATA_CLOCK                   G   I      1   1   0
// LDRQ1#                                   Not Assigned                                G   O      1   0   0
// TOTAL                                                                                           E   4   1
//                                 
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
// MGPIO0 / GP24                            [I] Fast Prochot RR#                        G   I      1   1   0
// PCIECLKRQ3                               Not Assigned                                G   O   L  1   0   0
// PCIECLKRQ4                               Not Assigned                                G   O      1   0   0
// GP27 / MGPIO6                            Not Assigned                                G   O   L  1   0   0
// TOTAL                                                                                           F   1   0
//                                 
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
// GP28 / MGPIO7                            [O] ME_Drive#                               G   O   H  1   0   1
// SLP_WLAN                                 [O] Disable Prochot                         G   O      1   0   0
// SUSWARN# / SUSPWRDNACK                   [I] SUSWARN_N                               N   I      0   1   0
// GP31                                     [I] SMB_PWR_ALERT_N                         G   I      1   1   0
// TOTAL                                                                                           B   C   1
//                                 
// BEVERLYCOVE_USESEL_VAL = BFEFE7FF                           
// BEVERLYCOVE_IOSEL_VAL  = C148E83D                           
// BEVERLYCOVE_LVL_VAL    = 10100011                           
//                                 
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
// CLKRUN#                                  Not Assigned                                G   O      1   0   0
// DOCKEN#                                  Not Assigned                                G   I   H  1   1   1
// NONE                                     Unused                                      G   I      1   1   0
// NMI#                                     [O] NMI_N                                   N   O      0   0   0
// TOTAL                                                                                           7   6   2
//                                 
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
// SATA2GP                                  ADR_STATUS_RD                               G   I   H  1   1   1
// SATA3GP                                  ADR_STATUS_CLR                              G   I   H  1   1   1
// GP38 / SLOAD                             [I] PU_SGPIO38_PCH_SLOAD_R                  G   I      1   1   0
// SDATAOUT[0]                              [I] PU_SGPIO39_SATA_DATAOUT0                G   I      1   1   0
// TOTAL                                                                                           F   F   3
//                                 
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
// GP40 / OC[1]#                            Not Assigned                                G   O      1   0   0
// GP41 / OC[2]#                            Not Assigned                                G   O      1   0   0
// GP42 / OC[3]#                            Not Assigned                                G   O      1   0   0
// GP43 / OC[4]#                            Not Assigned                                G   O      1   0   0
// TOTAL                                                                                           F   0   0
//                                 
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
//  PCIECLKRQ5                              Not Assigned                                G   O      1   0   0
//  PCIECLKRQ6                              Not Assigned                                G   O      1   0   0
//  PCIECLKRQ7                              Not Assigned                                G   O      1   0   0
//  N/C                                     Unused                                      N   I      0   1   0
//  TOTAL                                                                                          7   8   0
//                                 
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
//  SDATAOUT[1]                             [I] PU_SGPIO48_SATA_DATAOUT1                G   I   L  1   1   0
//  SATA5GP                                 [I] CPU_PROCHOT_N                           G   0      1   0   0
//  GP50 / GSXCLK                           Not Assigned                                G   O      1   0   0
//  GP51 /GSXDOUT                           Not Assigned                                G   I      1   1   0
//  TOTAL                                                                                          F   9   0
//                                 
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
// GP52 / GSX_SLOAD                         [I] CPU_SV                                  G   I   H  1   1   1
// GP53 / GSXDIN                            Not Assigned                                G   I   H  1   1   1
// GP54 / GSXSRESET#                        Not Assigned                                G   O      1   0   0
// GP55                                     [I] BIOS_RCVR_BOOT_N                        G   I      1   1   0
// TOTAL                                                                                           F   B   3
//                                 
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
// N/C                                      Unused                                      N   I      0   1   0
// GP57 / MGPIO5                            [I] ME_RCVR_N                               G   I   H  1   1   1
// SML1CLK / MGPIO11                        [O] SMB_SML1_3V3SB_CLK                      N   O      0   0   0
// GP59 / OC[0]#                            Not Assigned                                G   O      1   0   0
// TOTAL                                                                                           A   3   2
//                                 
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
// SML0ALERT#                               [I] SML0_Alert_N (or other interrupt need)  N   1      0   1   0
// SUS_STAT#                                Not Assigned                                N   O      0   0   0
// SUSCLK                                   [I] SUSCLK                                  N   O      0   0   0
// SLP_S5                                   Unused                                      N   O      0   0   0
// TOTAL                                                                                           0   1   0
//                                 
// BEVERLYCOVE_USE_SEL2_VAL =   0AFF7FF7                           
// BEVERLYCOVE_IO_SEL2_VAL  =   13B980F6                           
// BEVERLYCOVE_LVL2_VAL     =   02300032                           
//                                 
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
// GP64 / FLEXCLOCK[0]                      Pin removed                                 N   O      0   0   0
// GP65 / FLEXCLOCK[1]                      Not Assigned                                G   O      1   0   0
// GP66 / FLEXCLOCK[2]                      Pin removed                                 N   O      0   0   0
// GP67 / FLEXCLOCK[3]                      Not Assigned                                G   O      1   0   0
// TOTAL                                                                                           A   0   0
//                                 
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
// TACH4                                    Not Assigned                                G   O      1   0   0
// TACH5                                    Not Assigned                                G   O      1   0   0
// TACH6                                    Not Assigned                                G   O      1   0   0
// TACH7                                    Not Assigned                                G   O      1   0   0
// TOTAL                                                                                           F   0   0
//                                 
// Signal                                   UsedAs                                      UseI/OLVLInUSE IO LVL
// GP72 / BATLOW#                           Not Assigned                                N   I      0   1   0
// SRC0CLKRQ#                               Unused                                      N   I      0   1   0
// SML1ALERT / PCH_HOT#                     [O] PCH_HOT_N                               N   O   H  0   0   1
// SML1DATA / MGPIO12                       [O] SMB_SML1_3V3SB_DAT                      N   O      0   0   0
// TOTAL                                                                                           0   3   4
//                                 
//                                 
// BEVERLYCOVE_USE_SEL3_VAL =   FFFFF0FA                           
// BEVERLYCOVE_IO_SEL3_VAL  =   FFFFF300                           
// BEVERLYCOVE_LVL3_VAL     =   00000400                           

#define BEVERLYCOVE_GPIO_USE_SEL_VAL         0xBFEFE7FF
#define BEVERLYCOVE_GPIO_IO_SEL_VAL          0xC148E83D
#define BEVERLYCOVE_GPIO_LVL_VAL             0x10100011
#define BEVERLYCOVE_GPIO_USE_SEL2_VAL        0x0AFF7FF7
#define BEVERLYCOVE_GPIO_IO_SEL2_VAL         0x13B980F6
#define BEVERLYCOVE_GPIO_LVL2_VAL            0x02300032
#define BEVERLYCOVE_GPIO_USE_SEL3_VAL        0xFFFFF0FA
#define BEVERLYCOVE_GPIO_IO_SEL3_VAL         0xFFFFF300
#define BEVERLYCOVE_GPIO_LVL3_VAL            0x00000400
#define BEVERLYCOVE_GPIO_BLINK_VAL           0x00000000
#define BEVERLYCOVE_GPIO_INV_VAL             0x00000000

//
// Grangeville TrabucoCanyon(CRB) Config
//
// Signal                                     UsedAs                                    Use I/O LVL InUSE IO LVL
// GP0 / BMBUSY#                              [I] IRQ_PCH_WHEA_N(10K PU 3.3V)           G   I         1   1   0
// GP1 / TACH1                                [I] PU_TACH1 (10K PU 3.3V)                G   I         1   1   0
// GP2 / PIRQ[E]#                              Unused                                   G   I         1   0   0
// GP3 / PIRQ[F]#                             Unused                                    G   I         1   0   0
// TOTAL                                                                                              F   3   0

// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP4 / PIRQ[G]#                           [I] CPU2PCH_THROT                           N   I        0   0   0
// GP5 / PIRQ[H]#                           Unused                                      G   I        1   1   0
// GP6 / TACH2                              Unused                                      G   I        1   1   0
// GP7 / TACH3                              Unused                                      G   I        1   1   0
// TOTAL                                                                                             E   E   0

// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP8 / OCS                                Not Assigned                                G   O         1   0   0
// GP9 / OC[5]#                             Not Assigned                                N   I         1   1   0
// GP10 / OC[6]#                            Not Assigned                                N   I         1   0   0
// GP11 / SMBALERT#                         [I] SMBAlert                                N   I         0   1   0
// TOTAL                                                                                              7   A   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP12 / LANPHYPC                          [O] Lan_Phy_pwr_CTRL                        I   I         1   0   0
// GP13 / DOCKAZRST#                        Unused                                      G   I         1   1   0
// GP14 / OC7#                              CPLD_JTAG_TCK_R                             G   I         1   1   0
// GP15                                     SoftADR Trigger                             G   O         1   0   0
// TOTAL                                                                                              F   6   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP16 / SATA4                             [O] CPU_THROTTLE                            G   O   H     1   0   1
// TACH0                                    Not Assigned                                N   I         0   1   0
// SRC1CLKRQ                                [O] CPLD_JTAG_R_TMS                         G   O   H     1   0   1
// SATA1                                    Not Assigned                                N   I   L     1   1   0
// TOTAL                                                                                              D   A   5
//                   
// Signal                                   UsedAs                                      Use I/O LVL  InUSE IO LVL
// SMI / SRC2CLKRQ                          [O] SMI_Active_N                            N   O   H      0   0   1
// SATA0                                    Not Assigned                                G   I          1   1   0
// SGPIO_SCLK                               [I] SGPIO_PCH_SCLK_R                        N   O          0   0   0
// LDRQ1#                                   Unused                                      N   I          0   1   0
// TOTAL                                                                                               2   A   1
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// MGPIO0 / GP24                            SLOT_ID                                     G   1         1   1   0
// PCIECLKRQ3                               Unused                                      N   O   L     0   0   0
// PCIECLKRQ4                               Unused                                      N   O   H     0   0   0
// GP27 / MGPIO6                            Unused                                      G   I         0   0   0
// TOTAL                                                                                              1   1   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP28 / MGPIO7                            Unused                                      G   O   H    1   0   1
// SLP_WLAN                                 Unused                                      N   O   H    0   0   1
// SUSWARN# / SUSPWRDNACK                   [O] SUSWARN_N                               N   I        0   1   0
// GP31                                     [I] SMB_PWR_ALERT_N                         G   I        1   1   0
// TOTAL                                                                                             9   C   3
//                   
// TRABUCOCANYON_USESEL_VAL = 912DF7EF
// TRABUCOCANYON_IOSEL_VAL  = C1AA6AE3
// TRABUCOCANYON_LVL_VAL    = 30150000               
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// CLKRUN#                                  Unused                                      G   O   H     1   0   1
// DOCKEN#                                  [I] FPRR#                                   G   I         1   1   0
// NONE                                     Unused                                      G   I         1   1   0
// NMI#                                     [O] NMI_N                                   N   O         0   0   0
// TOTAL                                                                                              7   6   1
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// SATA2GP                                  [I] ADR_STATUS_RD_R                         G   I     H   1     1   1
// SATA3GP                                  [I] ADR_STATUS_CLR_R(AC_GOOD_LATCH)         G   I         1     1   1
// GP38 / SLOAD                             [O] SGPIO_PCH_SLOAD_                        N   O         0     0   0
// SDATAOUT[0]                              [O] SGPIO_PCH_SDATAOUT0_R                   N   O         0     0   0
// TOTAL                                                                                              3     3   3
//                                 
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP40 / OC[1]#                            Not Assigned                                N   I        0   1   0
// GP41 / OC[2]#                            CPLD_CONFIG_CLK_R                           G   O    H   1   0   1
// GP42 / OC[3]#                            CPLD_CONFIG_DATA_R                          G   O    H   1   0   1
// GP43 / OC[4]#                            MCU_INIT                                    G   O    H   1   0   0
// TOTAL                                                                                             E   1   6
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
//  PCIECLKRQ5                              Unused                                      N   I        0   1   0
//  PCIECLKRQ6                              Unused                                      N   I        0   1   0
//  PCIECLKRQ7                              Unused                                      N   I        0   1   0
//  N/C                                     Unused                                      N   I        0   1   0
//  TOTAL                                                                                            0   F   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
//  SDATAOUT[1]                             [O] SGPIO_PCH_SDATA0                        N   O   L    0   0   0
//  SATA5GP                                 [I] CPU_PROCHOT_N                           G   I        1   1   0
//  GP50 / GSXCLK                           Not Assigned                                G   O        1   0   0
//  GP51 /GSXDOUT                           Not Assigned                                G   I        1   1   0
//  TOTAL                                                                                            E   A   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP52 / GSX_SLOAD                         [I] ROW_Enable (Reserved)                   G   I   L    1   1   0
// GP53 / GSXDIN                            Not Assigned                                G   O   H    0   0   0
// GP54 / GSXSRESET#                        Not Assigned                                G   O   H    0   0   0
// GP55                                     [I] BIOS_RCVR_BOOT_N                        G   I        1   1   0
// TOTAL                                                                                             9   9   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// N/C                                      Unused                                      N   I         0   1   0
// GP57 / MGPIO5                            [I] ME_RCVR_N                               G   I         1   1   0
// SML1CLK / MGPIO11                        [I] SMB_SML1_3V3SB_CLK                      N   I         0   1   0
// GP59 / OC[0]#                            Not Assigned                                N   I         0   1   0
// TOTAL                                                                                              2   F   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// SML0ALERT#                               [O] SML0_Alert_N (or other interrupt need)  N   O        0   0   0
// SUS_STAT#                                Unused                                      N   O        0   0   0
// SUSCLK                                   SUSCLK                                      N   O        0   0   0
// SLP_S5                                   Unused                                      N   O        0   0   0
// TOTAL                                                                                             0   0   0
//                   
// TRABUCOCANYON_USE_SEL2_VAL =   029E0E37               
// TRABUCOCANYON_IO_SEL2_VAL  =   0F9AF136               
// TRABUCOCANYON_LVL2_VAL     =   00000631
//                                 
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP64 / FLEXCLOCK[0]                      Pin removed                                 N   O        0   0   0
// GP65 / FLEXCLOCK[1]                      [O] CLKOUTFLEX1_GP65                        N   O        0   0   0
// GP66 / FLEXCLOCK[2]                      Pin removed                                 N   O        0   0   0
// GP67 / FLEXCLOCK[3]                      [O] CLKOUTFLEX3_48MHz                       N   O        0   0   0
// TOTAL                                                                                             0   0   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// TACH4                                    Unused                                      G   I         1   1   0
// TACH5                                    Unused                                      G   I         1   1   0
// TACH6                                    Unused                                      N   I         0   1   1
// TACH7                                    Unused                                      N   I         0   1   1
// TOTAL                                                                                              3   F   C
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP72 / BATLOW#                           Unused                                      N   O    H    0   0   1
// SRC0CLKRQ#                               Unused                                      N   I         0   1   0
// SML1ALERT / PCH_HOT#                     [O] PCH_HOT_N                               N   O   H     0   0   1
// SML1DATA / MGPIO12                       [O] SMB_SML1_3V3SB_DAT                      N   O         0   0   0
// TOTAL                                                                                              0   2   5
//                   
//                   
// TRABUCOCANYON_USE_SEL3_VAL =   FFFFF030               
// TRABUCOCANYON_IO_SEL3_VAL  =   FFFFF2F0               
// TRABUCOCANYON_LVL3_VAL     =   000005C0               

#define TRABUCOCANYON_GPIO_USE_SEL_VAL         0x912DF7EF
#define TRABUCOCANYON_GPIO_IO_SEL_VAL          0xC1AA6AE3
#define TRABUCOCANYON_GPIO_LVL_VAL             0x30150000
#define TRABUCOCANYON_GPIO_USE_SEL2_VAL        0x029E0E37 
#define TRABUCOCANYON_GPIO_IO_SEL2_VAL         0x0F9AF136
#define TRABUCOCANYON_GPIO_LVL2_VAL            0x00000631
#define TRABUCOCANYON_GPIO_USE_SEL3_VAL        0xFFFFF030
#define TRABUCOCANYON_GPIO_IO_SEL3_VAL         0xFFFFF2F0
#define TRABUCOCANYON_GPIO_LVL3_VAL            0x000005C0
#define TRABUCOCANYON_GPIO_BLINK_VAL           0x00000000
#define TRABUCOCANYON_GPIO_INV_VAL             0x00000000

//
// Grangeville CamelbackMountain(CRB) Config
//
// Signal                                     UsedAs                                    Use I/O LVL InUSE IO LVL
// GP0 / BMBUSY#                              [I] IRQ_PCH_WHEA_N(10K PU 3.3V)           G   I         1   1   0
// GP1 / TACH1                                [I] PU_TACH1 (10K PU 3.3V)                G   I         1   1   0
// GP2 / PIRQ[E]#                             Unused                                    G   I         1   0   0
// GP3 / PIRQ[F]#                             Unused                                    G   I         1   0   0
// TOTAL                                                                                              F   3   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP4 / PIRQ[G]#                           [I] CPU2PCH_THROT                           G   I         1   1   0
// GP5 / PIRQ[H]#                           Unused                                      G   I         1   1   0
// GP6 / TACH2                              Unused                                      G   I         1   1   0
// GP7 / TACH3                              Unused                                      G   I         1   1   0
// TOTAL                                                                                              F   F   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP8 / OCS                                Not Assigned                                G   O         1   0   0
// GP9 / OC[5]#                             Not Assigned                                N   I         1   1   0
// GP10 / OC[6]#                            Not Assigned                                N   I         1   0   0
// GP11 / SMBALERT#                         [I] SMBAlert                                N   I         0   1   0
// TOTAL                                                                                              7   A   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP12 / LANPHYPC                          [O] Lan_Phy_pwr_CTRL                        N   O         0   0   0
// GP13 / DOCKAZRST#                        Unused                                      G   I         1   1   0
// GP14 / OC7#                              CPLD_JTAG_TCK_R                             G   I         1   1   0
// GP15                                     SOC_FPGA_CLK                                G   O         1   0   0
// TOTAL                                                                                              E   6   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP16 / SATA4                             [O] CPU_THROTTLE                            G   O   H     1   0   1
// TACH0                                    Not Assigned                                N   I         0   1   0
// SRC1CLKRQ                                [O] CPLD_JTAG_R_TMS                         G   O   H     1   0   1
// SATA1                                    Not Assigned                                N   I   L     1   1   0
// TOTAL                                                                                              D   A   5
//                   
// Signal                                   UsedAs                                      Use I/O LVL  InUSE IO LVL
// SMI / SRC2CLKRQ                          [O] SMI_Active_N                            N   O   H      0   0   1
// SATA0                                    Not Assigned                                G   I          1   1   0
// SGPIO_SCLK                               [I] SGPIO_PCH_SCLK_R                        N   O          0   0   0
// LDRQ1#                                   Unused                                      N   I          0   1   0
// TOTAL                                                                                               2   A   1
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// MGPIO0 / GP24                            Unused                                      G   O         0   0   0
// PCIECLKRQ3                               Unused                                      N   O   L     0   0   0
// PCIECLKRQ4                               LA_TRIGGER                                  G   O   H     1   0   0
// GP27 / MGPIO6                            SOC_FPGA_DATA_IN                            G   I         1   1   0
// TOTAL                                                                                              C   8   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP28 / MGPIO7                            SOC_FPGA_DATA_OUT                           G   O   H    1   0   0
// SLP_WLAN                                 Unused                                      N   O   H    0   0   1
// SUSWARN# / SUSPWRDNACK                   [O] SUSWARN_N                               N   I        0   1   0
// GP31                                     [I] SMB_PWR_ALERT_N                         G   I        1   1   0
// TOTAL                                                                                             9   C   3
//                   
// CAMELBACKMOUNTAIN_USESEL_VAL = 9C2DE7FF               
// CAMELBACKMOUNTAIN_IOSEL_VAL  = C8AA6AF3               
// CAMELBACKMOUNTAIN_LVL_VAL    = 30150000               
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// CLKRUN#                                  Unused                                      G   O   H     1   0   1
// DOCKEN#                                  [I] FPRR#                                   G   I         1   1   0
// NONE                                     Unused                                      G   I         1   1   0
// NMI#                                     [O] NMI_N                                   N   O         0   0   0
// TOTAL                                                                                              7   6   1
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// SATA2GP                                  [O] ADR_STATUS_RD_R                         G   O   H   1     0   1
// SATA3GP                                  [I] ADR_STATUS_CLR_R                        G   I       1     1   1
// GP38 / SLOAD                             [O] SGPIO_PCH_SLOAD_                        N   O       0     0   0
// SDATAOUT[0]                              [O] SGPIO_PCH_SDATAOUT0_R                   N   O       0     0   0
// TOTAL                                                                                            3     2   3
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP40 / OC[1]#                            Not Assigned                                N   I        0   1   0
// GP41 / OC[2]#                            CPLD_CONFIG_CLK_R                           G   O    H   1   0   1
// GP42 / OC[3]#                            CPLD_CONFIG_DATA_R                          G   O    H   1   0   1
// GP43 / OC[4]#                            Not Assigned                                N   I        0   1   0
// TOTAL                                                                                             6   9   6
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
//  PCIECLKRQ5                              Unused                                      N   I        0   1   0
//  PCIECLKRQ6                              Unused                                      N   I        0   1   0
//  PCIECLKRQ7                              Unused                                      N   I        0   1   0
//  N/C                                     Unused                                      N   I        0   1   0
//  TOTAL                                                                                            0   F   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
//  SDATAOUT[1]                             [O] SGPIO_PCH_SDATA0                        N   O   L    0   0   0
//  SATA5GP                                 [I] CPU_PROCHOT_N                           G   I        1   1   0
//  GP50 / GSXCLK                           Not Assigned                                G   O        1   0   0
//  GP51 /GSXDOUT                           SLOT_ID                                     G   I        1   1   0
//  TOTAL                                                                                            E   A   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP52 / GSX_SLOAD                         [I] CPU_SV                                  G   I   L    1   1   0
// GP53 / GSXDIN                            Not Assigned                                G   O   H    0   0   0
// GP54 / GSXSRESET#                        Not Assigned                                G   O   H    0   0   0
// GP55                                     [I] BIOS_RCVR_BOOT_N                        G   I        1   1   0
// TOTAL                                                                                             9   9   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// N/C                                      Unused                                      N   I         0   1   0
// GP57 / MGPIO5                            [I] ME_RCVR_N                               G   I         1   1   0
// SML1CLK / MGPIO11                        [I] SMB_SML1_3V3SB_CLK                      N   I         0   1   0
// GP59 / OC[0]#                            Not Assigned                                N   I         0   1   0
// TOTAL                                                                                              2   F   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// SML0ALERT#                               [O] SML0_Alert_N (or other interrupt need)  N   O        0   0   0
// SUS_STAT#                                Unused                                      N   O        0   0   0
// SUSCLK                                   SUSCLK                                      N   O        0   0   0
// SLP_S5                                   Unused                                      N   O        0   0   0
// TOTAL                                                                                             0   0   0
//                   
// CAMELBACKMOUNTAIN_USE_SEL2_VAL =   029E0637               
// CAMELBACKMOUNTAIN_IO_SEL2_VAL  =   0F9AF926               
// CAMELBACKMOUNTAIN_LVL2_VAL     =   00000631               
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP64 / FLEXCLOCK[0]                      Pin removed                                 N   O        0   0   0
// GP65 / FLEXCLOCK[1]                      [O] CLKOUTFLEX1_GP65                        N   O        0   0   0
// GP66 / FLEXCLOCK[2]                      Pin removed                                 N   O        0   0   0
// GP67 / FLEXCLOCK[3]                      [O] CLKOUTFLEX3_48MHz                       N   O        0   0   0
// TOTAL                                                                                             0   0   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// TACH4                                    Unused                                      G   I         1   1   0
// TACH5                                    Unused                                      G   I         1   1   0
// TACH6                                    Unused                                      N   I         0   1   1
// TACH7                                    Unused                                      N   I         0   1   1
// TOTAL                                                                                              3   F   C
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP72 / BATLOW#                           ADR_MCU_INIT                                G   O    H    1   0   1
// SRC0CLKRQ#                               Unused                                      N   I         0   1   0
// SML1ALERT / PCH_HOT#                     [O] PCH_HOT_N                               N   O   H     0   0   1
// SML1DATA / MGPIO12                       [O] SMB_SML1_3V3SB_DAT                      N   O         0   0   0
// TOTAL                                                                                              1   2   5
//                   
//                   
// CAMELBACKMOUNTAIN_USE_SEL3_VAL =   FFFFF130               
// CAMELBACKMOUNTAIN_IO_SEL3_VAL  =   FFFFF2F0               
// CAMELBACKMOUNTAIN_LVL3_VAL     =   000005C0               

#define CAMELBACKMOUNTAIN_GPIO_USE_SEL_VAL         0x9C2DE7FF
#define CAMELBACKMOUNTAIN_GPIO_IO_SEL_VAL          0xC8AA6AF3
#define CAMELBACKMOUNTAIN_GPIO_LVL_VAL             0x30150000
#define CAMELBACKMOUNTAIN_GPIO_USE_SEL2_VAL        0x029E0637
#define CAMELBACKMOUNTAIN_GPIO_IO_SEL2_VAL         0x0F9AF926
#define CAMELBACKMOUNTAIN_GPIO_LVL2_VAL            0x00000631
#define CAMELBACKMOUNTAIN_GPIO_USE_SEL3_VAL        0xFFFFF130
#define CAMELBACKMOUNTAIN_GPIO_IO_SEL3_VAL         0xFFFFF2F0
#define CAMELBACKMOUNTAIN_GPIO_LVL3_VAL            0x000005C0
#define CAMELBACKMOUNTAIN_GPIO_BLINK_VAL           0x00000000
#define CAMELBACKMOUNTAIN_GPIO_INV_VAL             0x00000000

//
// Grangeville Tlaquepaque(CRB) Config
//
// Signal                                     UsedAs                                    Use I/O LVL InUSE IO LVL
// GP0 / BMBUSY#                              [I] IRQ_PCH_WHEA_N(10K PU 3.3V)           G   I         1   1   0
// GP1 / TACH1                                [I] PU_TACH1 (10K PU 3.3V)                G   I         1   1   0
// GP2 / PIRQ[E]#                             Unused                                    G   I         1   0   0
// GP3 / PIRQ[F]#                             Unused                                    G   I         1   0   0
// TOTAL                                                                                              F   3   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP4 / PIRQ[G]#                           [I] CPU2PCH_THROT                           G   I         1   1   0
// GP5 / PIRQ[H]#                           Unused                                      G   I         1   1   0
// GP6 / TACH2                              Unused                                      G   I         1   1   0
// GP7 / TACH3                              Unused                                      G   I         1   1   0
// TOTAL                                                                                              F   F   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP8 / OCS                                Not Assigned                                G   O         1   0   0
// GP9 / OC[5]#                             Not Assigned                                N   I         1   1   0
// GP10 / OC[6]#                            Not Assigned                                N   I         1   0   0
// GP11 / SMBALERT#                         [I] SMBAlert                                N   I         0   1   0
// TOTAL                                                                                              7   A   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP12 / LANPHYPC                          [O] Lan_Phy_pwr_CTRL                        N   O         0   0   0
// GP13 / DOCKAZRST#                        Unused                                      G   I         1   1   0
// GP14 / OC7#                              CPLD_JTAG_TCK_R                             G   I         1   1   0
// GP15                                     Unused                                      G   O         1   0   0
// TOTAL                                                                                              E   6   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP16 / SATA4                             [O] CPU_THROTTLE                            G   O   H     1   0   1
// TACH0                                    Not Assigned                                N   I         0   1   0
// SRC1CLKRQ                                [O] CPLD_JTAG_R_TMS                         G   O   H     1   0   1
// SATA1                                    Not Assigned                                N   I   L     1   1   0
// TOTAL                                                                                              D   A   5
//                   
// Signal                                   UsedAs                                      Use I/O LVL  InUSE IO LVL
// SMI / SRC2CLKRQ                          [O] SMI_Active_N                            N   O   H      0   0   1
// SATA0                                    Not Assigned                                G   I          1   1   0
// SGPIO_SCLK                               [I] SGPIO_PCH_SCLK_R                        N   O          0   0   0
// LDRQ1#                                   Unused                                      N   I          0   1   0
// TOTAL                                                                                               2   A   1
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// MGPIO0 / GP24                            Unused                                      G   O         0   0   0
// PCIECLKRQ3                               Unused                                      N   O   L     0   0   0
// PCIECLKRQ4                               Unused                                      N   O   H     0   0   0
// GP27 / MGPIO6                            Unused                                      G   I         0   0   0
// TOTAL                                                                                              0   0   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP28 / MGPIO7                            Unused                                      G   O   H    1   0   1
// SLP_WLAN                                 Unused                                      N   O   H    0   0   1
// SUSWARN# / SUSPWRDNACK                   [O] SUSWARN_N                               N   I        0   1   0
// GP31                                     [I] SMB_PWR_ALERT_N                         G   I        1   1   0
// TOTAL                                                                                             9   C   3
//                   
// TLAQUEPAQUE_USESEL_VAL = 902DE7FF               
// TLAQUEPAQUE_IOSEL_VAL  = C0AA6AF3               
// TLAQUEPAQUE_LVL_VAL    = 30150000               
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// CLKRUN#                                  Unused                                      G   O   H     1   0   1
// DOCKEN#                                  [I] FPRR#                                   G   I         1   1   0
// NONE                                     Unused                                      G   I         1   1   0
// NMI#                                     [O] NMI_N                                   N   O         0   0   0
// TOTAL                                                                                              7   6   1
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// SATA2GP                                  [I] ADR_STATUS_RD_R                         G   I   H   1     1   0
// SATA3GP                                  [I] ADR_STATUS_CLR_R(AC_GOOD_LATCH)         G   I       1     1   1
// GP38 / SLOAD                             [O] SGPIO_PCH_SLOAD_                        N   O       0     0   0
// SDATAOUT[0]                              [O] SGPIO_PCH_SDATAOUT0_R                   N   O       0     0   0
// TOTAL                                                                                            3     3   2
//  
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP40 / OC[1]#                            Not Assigned                                N   I        0   1   0
// GP41 / OC[2]#                            CPLD_CONFIG_CLK_R                           G   O   H    1   0   1
// GP42 / OC[3]#                            CPLD_CONFIG_DATA_R                          G   O   H    1   0   1
// GP43 / OC[4]#                            MCU_INIT                                    G   O   H    1   0   0
// TOTAL                                                                                             E   1   6
//   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
//  PCIECLKRQ5                              Unused                                      N   I        0   1   0
//  PCIECLKRQ6                              Unused                                      N   I        0   1   0
//  PCIECLKRQ7                              Unused                                      N   I        0   1   0
//  N/C                                     Unused                                      N   I        0   1   0
//  TOTAL                                                                                            0   F   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
//  SDATAOUT[1]                             [O] SGPIO_PCH_SDATA0                        N   O   L    0   0   0
//  SATA5GP                                 [I] CPU_PROCHOT_N                           G   I        1   1   0
//  GP50 / GSXCLK                           Not Assigned                                G   O        1   0   0
//  GP51 /GSXDOUT                           Not Assigned                                G   I        1   1   0
//  TOTAL                                                                                            E   A   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP52 / GSX_SLOAD                         [I] ROW_Enable (Reserved)                   G   I   L    1   1   0
// GP53 / GSXDIN                            Not Assigned                                G   O   H    0   0   0
// GP54 / GSXSRESET#                        Not Assigned                                G   O   H    0   0   0
// GP55                                     [I] BIOS_RCVR_BOOT_N                        G   I        1   1   0
// TOTAL                                                                                             9   9   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// N/C                                      Unused                                      N   I         0   1   0
// GP57 / MGPIO5                            [I] ME_RCVR_N                               G   I         1   1   0
// SML1CLK / MGPIO11                        [I] SMB_SML1_3V3SB_CLK                      N   I         0   1   0
// GP59 / OC[0]#                            Not Assigned                                N   I         0   1   0
// TOTAL                                                                                              2   F   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// SML0ALERT#                               [O] SML0_Alert_N (or other interrupt need)  N   O        0   0   0
// SUS_STAT#                                Unused                                      N   O        0   0   0
// SUSCLK                                   SUSCLK                                      N   O        0   0   0
// SLP_S5                                   Unused                                      N   O        0   0   0
// TOTAL                                                                                             0   0   0
//   
// TLAQUEPAQUE_USE_SEL2_VAL =   029E0E37
// TLAQUEPAQUE_IO_SEL2_VAL  =   0F9AF136
// TLAQUEPAQUE_LVL2_VAL     =   00000621
//   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP64 / FLEXCLOCK[0]                      Pin removed                                 N   O          0   0   0
// GP65 / FLEXCLOCK[1]                      [O] CLKOUTFLEX1_GP65                        N   O          0   0   0
// GP66 / FLEXCLOCK[2]                      Pin removed                                 N   O          0   0   0
// GP67 / FLEXCLOCK[3]                      [O] CLKOUTFLEX3_48MHz                       N   O          0   0   0
// TOTAL                                                                                             0   0   0
//   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// TACH4                                    Unused                                      G   I         1   1   0
// TACH5                                    Unused                                      G   I         1   1   0
// TACH6                                    Unused                                      N   I         0   1   1
// TACH7                                    Unused                                      N   I         0   1   1
// TOTAL                                                                                              3   F   C
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP72 / BATLOW#                           ADR_MCU_INIT                                G   O    H    1   0   1
// SRC0CLKRQ#                               Unused                                      N   I         0   1   0
// SML1ALERT / PCH_HOT#                     [O] PCH_HOT_N                               N   O    H    0   0   1
// SML1DATA / MGPIO12                       [O] SMB_SML1_3V3SB_DAT                      N   O         0   0   0
// TOTAL                                                                                              1   2   5
//                   
//                   
// TLAQUEPAQUE_USE_SEL3_VAL =   FFFFF130               
// TLAQUEPAQUE_IO_SEL3_VAL  =   FFFFF2F0               
// TLAQUEPAQUE_LVL3_VAL     =   000005C0               

#define TLAQUEPAQUE_GPIO_USE_SEL_VAL         0x902DE7FF
#define TLAQUEPAQUE_GPIO_IO_SEL_VAL          0xC0AA6AF3
#define TLAQUEPAQUE_GPIO_LVL_VAL             0x30150000
#define TLAQUEPAQUE_GPIO_USE_SEL2_VAL        0x029E0E37
#define TLAQUEPAQUE_GPIO_IO_SEL2_VAL         0x0F9AF136
#define TLAQUEPAQUE_GPIO_LVL2_VAL            0x00000621
#define TLAQUEPAQUE_GPIO_USE_SEL3_VAL        0xFFFFF130
#define TLAQUEPAQUE_GPIO_IO_SEL3_VAL         0xFFFFF2F0
#define TLAQUEPAQUE_GPIO_LVL3_VAL            0x000005C0
#define TLAQUEPAQUE_GPIO_BLINK_VAL           0x00000000
#define TLAQUEPAQUE_GPIO_INV_VAL             0x00000000

//
// Grangeville PiestewaPeak(CRB) Config
//
// Signal                                     UsedAs                                    Use I/O LVL InUSE IO LVL
// GP0 / BMBUSY#                              [I] IRQ_PCH_WHEA_N(10K PU 3.3V)           G   I         1   1   0
// GP1 / TACH1                                [I] PU_TACH1 (10K PU 3.3V)                G   I         1   1   0
// GP2 / PIRQ[E]#                             Unused                                    G   I         1   0   0
// GP3 / PIRQ[F]#                             Unused                                    G   I         1   0   0
// TOTAL                                                                                              F   3   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP4 / PIRQ[G]#                           [I] CPU2PCH_THROT                           G   I         1   1   0
// GP5 / PIRQ[H]#                           Unused                                      G   I         1   1   0
// GP6 / TACH2                              Unused                                      G   I         1   1   0
// GP7 / TACH3                              Unused                                      G   I         1   1   0
// TOTAL                                                                                              F   F   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP8 / OCS                                Not Assigned                                G   O         1   0   0
// GP9 / OC[5]#                             Not Assigned                                N   I         1   1   0
// GP10 / OC[6]#                            Not Assigned                                N   I         1   0   0
// GP11 / SMBALERT#                         [I] SMBAlert                                N   I         0   1   0
// TOTAL                                                                                              7   A   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP12 / NMI#                              [O] Lan_Phy_pwr_CTRL                        G   I         1   1   0
// GP13 / DOCKAZRST#                        Unused                                      G   I         1   1   0
// GP14 / OC7#                              CPLD_JTAG_TCK_R                             G   I         1   1   0
// GP15                                     Unused                                      G   O         1   0   0
// TOTAL                                                                                              F   7   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP16 / SATA4                             [O] CPU_THROTTLE                            G   O   H     1   0   1
// TACH0                                    Not Assigned                                N   I         0   1   0
// SRC1CLKRQ                                [O] CPLD_JTAG_R_TMS                         G   O   H     1   0   1
// SATA1                                    Not Assigned                                N   I   L     1   1   0
// TOTAL                                                                                              D   A   5
//                   
// Signal                                   UsedAs                                      Use I/O LVL  InUSE IO LVL
// SMI / SRC2CLKRQ                          [O] SMI_Active_N                            N   O   H      0   0   1
// SATA0                                    Not Assigned                                G   I          1   1   0
// SGPIO_SCLK                               [I] SGPIO_PCH_SCLK_R                        N   O          0   0   0
// LDRQ1#                                   Unused                                      N   I          0   1   0
// TOTAL                                                                                               2   A   1
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// MGPIO0 / GP24                            Unused                                      G   O         0   0   0
// PCIECLKRQ3                               Unused                                      N   O   L     0   0   0
// PCIECLKRQ4                               Unused                                      N   O   H     0   0   0
// GP27 / MGPIO6                            Unused                                      G   I         0   0   0
// TOTAL                                                                                              0   0   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP28 / MGPIO7                            Unused                                      G   O   H    1   0   1
// SLP_WLAN                                 Unused                                      N   O   H    0   0   1
// SUSWARN# / SUSPWRDNACK                   [O] SUSWARN_N                               N   I        0   1   0
// GP31                                     [I] SMB_PWR_ALERT_N                         G   I        1   1   0
// TOTAL                                                                                             9   C   3
//                   
// PiestewaPeak_USESEL_VAL = 902DF7FF                
// PiestewaPeak_IOSEL_VAL  = C0AA7AF3                
// PiestewaPeak_LVL_VAL    = 30150000                
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// CLKRUN#                                  Unused                                      G   O   H     1   0   1
// DOCKEN#                                  [I] FPRR#                                   G   I         1   1   0
// NONE                                     Unused                                      G   I         1   1   0
// NMI#                                     [O] NMI_N                                   N   O         0   0   0
// TOTAL                                                                                              7   6   1
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// SATA2GP                                  Not Assigned                                N   O       0     0   0
// SATA3GP                                  Not Assigned                                N   O       0     0   0
// GP38 / SLOAD                             [O] SGPIO_PCH_SLOAD_                        N   O       0     0   0
// SDATAOUT[0]                              [O] SGPIO_PCH_SDATAOUT0_R                   N   O       0     0   0
// TOTAL                                                                                            0     0   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP40 / OC[1]#                            Not Assigned                                N   I        0   1   0
// GP41 / OC[2]#                            CPLD_CONFIG_CLK_R                           G   O    H   1   0   1
// GP42 / OC[3]#                            CPLD_CONFIG_DATA_R                          G   O    H   1   0   1
// GP43 / OC[4]#                            Not Assigned                                N   I        0   1   0
// TOTAL                                                                                             6   9   6
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
//  PCIECLKRQ5                              Unused                                      N   I        0   1   0
//  PCIECLKRQ6                              Unused                                      N   I        0   1   0
//  PCIECLKRQ7                              Unused                                      N   I        0   1   0
//  N/C                                     Unused                                      N   I        0   1   0
//  TOTAL                                                                                            0   F   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
//  SDATAOUT[1]                             [O] SGPIO_PCH_SDATA0                        N   O   L    0   0   0
//  SATA5GP                                 [I] CPU_PROCHOT_N                           G   I        1   1   0
//  GP50 / GSXCLK                           Not Assigned                                G   O        1   0   0
//  GP51 /GSXDOUT                           Not Assigned                                G   I        1   1   0
//  TOTAL                                                                                            E   A   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP52 / GSX_SLOAD                         [I] ROW_Enable (Reserved)                   G   I   L    1   1   0
// GP53 / GSXDIN                            Not Assigned                                G   O   H    0   0   0
// GP54 / GSXSRESET#                        Not Assigned                                G   O   H    0   0   0
// GP55                                     [I] BIOS_RCVR_BOOT_N                        G   I        1   1   0
// TOTAL                                                                                             9   9   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// N/C                                      Unused                                      N   I         0   1   0
// GP57 / MGPIO5                            [I] ME_RCVR_N                               G   I         1   1   0
// SML1CLK / MGPIO11                        [I] SMB_SML1_3V3SB_CLK                      N   I         0   1   0
// GP59 / OC[0]#                            Not Assigned                                N   I         0   1   0
// TOTAL                                                                                              2   F   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// SML0ALERT#                               [O] SML0_Alert_N (or other interrupt need)  N   O        0   0   0
// SUS_STAT#                                Unused                                      N   O        0   0   0
// SUSCLK                                   SUSCLK                                      N   O        0   0   0
// SLP_S5                                   Unused                                      N   O        0   0   0
// TOTAL                                                                                             0   0   0
//                   
// PiestewaPeak_USE_SEL2_VAL =   029E0607                
// PiestewaPeak_IO_SEL2_VAL  =   0F9AF906                
// PiestewaPeak_LVL2_VAL     =   00000601                
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP64 / FLEXCLOCK[0]                      Pin removed                                 N   O        0   0   0
// GP65 / FLEXCLOCK[1]                      [O] CLKOUTFLEX1_GP65                        N   O        0   0   0
// GP66 / FLEXCLOCK[2]                      Pin removed                                 N   O        0   0   0
// GP67 / FLEXCLOCK[3]                      [O] CLKOUTFLEX3_48MHz                       N   O        0   0   0
// TOTAL                                                                                             0   0   0
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// TACH4                                    Unused                                      G   I         1   1   0
// TACH5                                    Unused                                      G   I         1   1   0
// TACH6                                    Unused                                      N   I         0   1   1
// TACH7                                    Unused                                      N   I         0   1   1
// TOTAL                                                                                              3   F   C
//                   
// Signal                                   UsedAs                                      Use I/O LVL InUSE IO LVL
// GP72 / BATLOW#                           ADR_MCU_INIT                                G   O    H    1   0   1
// SRC0CLKRQ#                               Unused                                      N   I         0   1   0
// SML1ALERT / PCH_HOT#                     [O] PCH_HOT_N                               N   O   H     0   0   1
// SML1DATA / MGPIO12                       [O] SMB_SML1_3V3SB_DAT                      N   O         0   0   0
// TOTAL                                                                                              1   2   5
//                   
//                   
// PiestewaPeak_USE_SEL3_VAL =   FFFFF130                
// PiestewaPeak_IO_SEL3_VAL  =   FFFFF2F0                
// PiestewaPeak_LVL3_VAL     =   000005C0           

#define PiestewaPeak_GPIO_USE_SEL_VAL         0x902DF7FF
#define PiestewaPeak_GPIO_IO_SEL_VAL          0xC0AA7AF3
#define PiestewaPeak_GPIO_LVL_VAL             0x30150000
#define PiestewaPeak_GPIO_USE_SEL2_VAL        0x029E0627
#define PiestewaPeak_GPIO_IO_SEL2_VAL         0x0F9AF926
#define PiestewaPeak_GPIO_LVL2_VAL            0x00000621
#define PiestewaPeak_GPIO_USE_SEL3_VAL        0xFFFFF130
#define PiestewaPeak_GPIO_IO_SEL3_VAL         0xFFFFF2F0
#define PiestewaPeak_GPIO_LVL3_VAL            0x000005C0
#define PiestewaPeak_GPIO_BLINK_VAL           0x00000000
#define PiestewaPeak_GPIO_INV_VAL             0x00000000



typedef enum {
  TypeBeverlyCoveCRB = 1,
  TypeBeverlyCoveSODimmCRB,
  TypeTrabucoCanyon,
  TypePiestewaPeak,
  TypeCamelbackMountain,
  TypeTlaquepaque,
  TypeEchoCanyon
  } EFI_EXPLATFORM_TYPE;

#define TypePlatformUnknown       0xFF


#ifndef TypePlatformMin
#define TypePlatformMin           TypeBeverlyCoveCRB
#define TypePlatformDefault       TypeBeverlyCoveCRB
#endif


#define TypePlatformMax           TypeTlaquepaque



#else


//
// Platform types - if not already defined
//
typedef enum {
  TypeMayanCityCRB = 0x00,
  TypeMayanCitySRP,
  TypeMayanCitySRPDDR3,
  TypeYakimaCityCRB = 0x03, 
  TypeYakimaCitySRP, 
  TypeYakimaCitySTHI,
  TypeMayanCityCRBDDR3, 
  TypeAztecCityCRBSRP = 0x07,
  TypeAztecCitySTHI,
  TypeHedtReversedIdBits,
  TypeIncaCityCRB = 0x0A,
  TypeIncaCitySRP,
  TypeHalfWidthCitySRP = 0x0C, 
  TypeKahunaDVP2SPbg = 0x0F,
  TypeKahunaDVP2S,
  TypeArandasVVP2S = 0x11, 
  TypeArandasVVP4S,
  TypeHedtSKU1CRB = 0x13,
  TypeHedtEv,
  TypeHedtDDR3,
  TypeSaxtonRock = 0x18,
  TypeEchoRock,
  TypeBalanceRock,
  TypeEmeraldPoint,
  TypeWildcatPass = 0x32
} EFI_EXPLATFORM_TYPE;

typedef enum {
    HEDT_FAB1 = 0x00,
    HEDT_FAB2
} EFI_HEDT_PLATFORM_REVISION_ID;

#define TypeKahunaDVP TypeKahunaDVP2S

#define TypePlatformUnknown       0xFF
#define TypePlatformMin           TypeMayanCityCRB
#define TypePlatformMax           TypeWildcatPass
#define TypePlatformDefault       TypeMayanCitySRP
#define TypePlatformRevIdDefault  0x00
#endif

typedef enum {
  TypeSrpPcieRiserHp = 0,      // !!! SHOULD BE THE FIRST ENTRY !!!
  TypeSrpPcieRiserNonHp,
} EFI_EXPCIERISER_TYPE;

#endif


//
// ACPI table information used to initialize tables.
//
#define EFI_ACPI_OEM_ID           "INTEL "
#define EFI_ACPI_OEM_TABLE_ID     0x59454C544E415247  // "GRANTLEY"
#define EFI_ACPI_OEM_REVISION     0x00000002
#define EFI_ACPI_CREATOR_ID       0x4C544E49          // "INTL"
#define EFI_ACPI_CREATOR_REVISION 0x20091013          // Oct 13 2009

#define SW_SMI_OS_REQUEST         0x83  // OS transition request.
#define MEM_ADDR_SHFT_VAL         26    // For 64 MB granularity

#endif
