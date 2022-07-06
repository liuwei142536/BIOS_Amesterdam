/* Date Stamp: 8/23/2014 */

#ifndef HA_PMON_h
#define HA_PMON_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* HA_PMON_IVT_DEV 14                                                         */
/* HA_PMON_IVT_FUN 1                                                          */
/* For HSX_HOST:                                                              */
/* HA_PMON_HSX_DEV 18                                                         */
/* HA_PMON_HSX_FUN 1                                                          */
/* For BDX_HOST:                                                              */
/* HA_PMON_BDX_DEV 18                                                         */
/* HA_PMON_BDX_FUN 1                                                          */

/* VID_HA_PMON_REG supported on:                                              */
/*       IVT_EP (0x20171000)                                                  */
/*       IVT_EX (0x20171000)                                                  */
/*       HSX (0x20191000)                                                     */
/*       BDX (0x20191000)                                                     */
/* Register default value:              0x8086                                */
#define VID_HA_PMON_REG 0x03012000


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.1.CFG.xml.
 * PCI Vendor ID Register
 */
typedef union {
  struct {
    UINT16 vendor_identification_number : 16;
    /* vendor_identification_number - Bits[15:0], RO, default = 16'h8086 
       The value is assigned by PCI-SIG to Intel.
     */
  } Bits;
  UINT16 Data;
} VID_HA_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* DID_HA_PMON_REG supported on:                                              */
/*       IVT_EP (0x20171002)                                                  */
/*       IVT_EX (0x20171002)                                                  */
/*       HSX (0x20191002)                                                     */
/*       BDX (0x20191002)                                                     */
/* Register default value on IVT_EP:    0x0E30                                */
/* Register default value on IVT_EX:    0x0E30                                */
/* Register default value on HSX:       0x2F30                                */
/* Register default value on BDX:       0x6F30                                */
#define DID_HA_PMON_REG 0x03012002


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.1.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h2F30 (HSX), 16'h6F30 (BDX) 
       Device ID values vary from function to function. Bits 15:8 are equal to 0x2F. 
       The following list is a breakdown of the function groups. 
         0x2F00 - 0x2F1F : PCI Express and DMI2 ports
         0x2F20 - 0x2F3F : IO Features (Intel QuickData Technology, APIC, VT, RAS, 
       Intel TXT) 
         0x2F40 - 0x2F5F : Performance Monitors
         0x2F80 - 0x2F9F : Intel QPI
         0x2FA0 - 0x2FBF : Home Agent/Memory Controller
         0x2FC0 - 0x2FDF : Power Management
         0x2FE0 - 0x2FFF : Cbo/Ring
       Default value may vary based on bus, device, and function of this CSR location.
     */
  } Bits;
  UINT16 Data;
} DID_HA_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PCICMD_HA_PMON_REG supported on:                                           */
/*       IVT_EP (0x20171004)                                                  */
/*       IVT_EX (0x20171004)                                                  */
/*       HSX (0x20191004)                                                     */
/*       BDX (0x20191004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_HA_PMON_REG 0x03012004


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.1.CFG.xml.
 * PCI Command Register
 */
typedef union {
  struct {
    UINT16 io_space_enable : 1;
    /* io_space_enable - Bits[0:0], RO, default = 1'b0 
       Hardwired to 0 since these devices don't decode any IO BARs
     */
    UINT16 memory_space_enable : 1;
    /* memory_space_enable - Bits[1:1], RO, default = 1'b0 
       Hardwired to 0 since these devices don't decode any memory BARs
     */
    UINT16 bus_master_enable : 1;
    /* bus_master_enable - Bits[2:2], RO, default = 1'b0 
       Hardwired to 0 since these devices don't generate any transactions
     */
    UINT16 special_cycle_enable : 1;
    /* special_cycle_enable - Bits[3:3], RO, default = 1'b0 
       Not applicable. Hardwired to 0.
     */
    UINT16 memory_write_and_invalidate_enable : 1;
    /* memory_write_and_invalidate_enable - Bits[4:4], RO, default = 1'b0 
       Not applicable to internal devices. Hardwired to 0.
     */
    UINT16 vga_palette_snoop_enable : 1;
    /* vga_palette_snoop_enable - Bits[5:5], RO, default = 1'b0 
       Not applicable to internal devices. Hardwired to 0.
     */
    UINT16 parity_error_response : 1;
    /* parity_error_response - Bits[6:6], RO, default = 1'b0 
       This bit has no impact on error reporting from these devices
     */
    UINT16 idsel_stepping_wait_cycle_control : 1;
    /* idsel_stepping_wait_cycle_control - Bits[7:7], RO, default = 1'b0 
       Not applicable to internal devices. Hardwired to 0.
     */
    UINT16 serr_enable : 1;
    /* serr_enable - Bits[8:8], RO, default = 1'b0 
       This bit has no impact on error reporting from these devices
     */
    UINT16 fast_back_to_back_enable : 1;
    /* fast_back_to_back_enable - Bits[9:9], RO, default = 1'b0 
       Not applicable to PCI Express and is hardwired to 0
     */
    UINT16 intx_disable : 1;
    /* intx_disable - Bits[10:10], RO, default = 1'b0 
       N/A for these devices
     */
    UINT16 rsvd : 5;
    /* rsvd - Bits[15:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT16 Data;
} PCICMD_HA_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PCISTS_HA_PMON_REG supported on:                                           */
/*       IVT_EP (0x20171006)                                                  */
/*       IVT_EX (0x20171006)                                                  */
/*       HSX (0x20191006)                                                     */
/*       BDX (0x20191006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_HA_PMON_REG 0x03012006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.1.CFG.xml.
 * PCI Status Register
 */
typedef union {
  struct {
    UINT16 rsvd : 3;
    /* rsvd - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 intx_status : 1;
    /* intx_status - Bits[3:3], RO, default = 1'b0 
       Reflects the state of the INTA# signal at the input of the enable/disable 
       circuit.  This bit is set by HW to 1 when the INTA# is asserted.  This bit is 
       reset by HW to 0 after the interrupt is cleared (independent of the state of the 
       Interrupt Disable bit in the PCICMD register). 
       Hardwired to 0 on the processor.
     */
    UINT16 capabilities_list : 1;
    /* capabilities_list - Bits[4:4], RO, default = 1'h0 
       This bit indicates the presence of a capabilities list structure. When set to 1, 
       indicates the register at 34h provides an offset into the function. 
     */
    UINT16 x66mhz_capable : 1;
    /* x66mhz_capable - Bits[5:5], RO, default = 1'b0 
       Not applicable to PCI Express. Hardwired to 0.
     */
    UINT16 reserved : 1;
    /* reserved - Bits[6:6], RO, default = 1'b0 
       Reserved
     */
    UINT16 fast_back_to_back : 1;
    /* fast_back_to_back - Bits[7:7], RO, default = 1'b0 
       Not applicable to PCI Express. Hardwired to 0.
     */
    UINT16 master_data_parity_error : 1;
    /* master_data_parity_error - Bits[8:8], RO, default = 1'b0 
       Hardwired to 0
     */
    UINT16 devsel_timing : 2;
    /* devsel_timing - Bits[10:9], RO, default = 2'b00 
       Not applicable to PCI Express. Hardwired to 0.
     */
    UINT16 signaled_target_abort : 1;
    /* signaled_target_abort - Bits[11:11], RO, default = 1'b0 
       Hardwired to 0
     */
    UINT16 received_target_abort : 1;
    /* received_target_abort - Bits[12:12], RO, default = 1'b0 
       Hardwired to 0
     */
    UINT16 received_master_abort : 1;
    /* received_master_abort - Bits[13:13], RO, default = 1'b0 
       Hardwired to 0
     */
    UINT16 signaled_system_error : 1;
    /* signaled_system_error - Bits[14:14], RO, default = 1'b0 
       Hardwired to 0
     */
    UINT16 detected_parity_error : 1;
    /* detected_parity_error - Bits[15:15], RO_V, default = 1'b0 
       This bit is set when the device receives a packet on the primary side with an 
       uncorrectable data error (including a packet with poison bit set) or an 
       uncorrectable address/control parity error. The setting of this bit is 
       regardless of the Parity Error Response bit (PERRE) in the PCICMD register. 
     */
  } Bits;
  UINT16 Data;
} PCISTS_HA_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_HA_PMON_REG supported on:                                              */
/*       IVT_EP (0x10171008)                                                  */
/*       IVT_EX (0x10171008)                                                  */
/*       HSX (0x10191008)                                                     */
/*       BDX (0x10191008)                                                     */
/* Register default value:              0x00                                  */
#define RID_HA_PMON_REG 0x03011008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * "PCIe header Revision ID register"
 */
typedef union {
  struct {
    UINT8 revision_id : 8;
    /* revision_id - Bits[7:0], ROS_V, default = 8'b00000000 
       Reflects the Uncore Revision ID after reset.
       Reflects the Compatibility Revision ID after BIOS writes 0x69 to any RID 
       register in the processor uncore. 
       
     */
  } Bits;
  UINT8 Data;
} RID_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_HA_PMON_REG supported on:                                           */
/*       IVT_EP (0x10171009)                                                  */
/*       IVT_EX (0x10171009)                                                  */
/*       HSX (0x10191009)                                                     */
/*       BDX (0x10191009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_HA_PMON_REG 0x03011009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.1.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_HA_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_HA_PMON_REG supported on:                                           */
/*       IVT_EP (0x2017100A)                                                  */
/*       IVT_EX (0x2017100A)                                                  */
/*       HSX (0x2019100A)                                                     */
/*       BDX (0x2019100A)                                                     */
/* Register default value:              0x1101                                */
#define CCR_N1_HA_PMON_REG 0x0301200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT16 sub_class : 8;
    /* sub_class - Bits[7:0], RO_V, default = 8'b00000001 
       The value changes dependent upon the dev/func accessed. A table of the values 
       can be found in the Class-code tab of the msgch spread-sheet  
          Most dev-func will return 8'h80 for this field except for the following 
       dev-func0,func1,... combinations. The following exceptions will return 8'h01. 
                 dev-0x0 through 0x7 (return 0x4, d0f0 return 0x0 under default 
       settings) 
       	  dev-0x8 func-2
       	  dev-0x9 func-2
       	  dev-0xA func-2
       	  dev-0xB func-1,2,5,6
       	  dev-0x10 func-1,6
       	  dev-0x12 func-1,5
       	  
     */
    UINT16 base_class : 8;
    /* base_class - Bits[15:8], RO_V, default = 8'b00010001 
       The value changes dependent upon the dev-func accessed. A table of the values 
       can be found in the Class-code tab of the msgch spread-sheet  
          Most dev-func will return 8'h08 for this field except for the following 
       dev-func0,func1,... combinations. The following exceptions will return 8'h11. 
                 dev-0x0 through 0x7 (return 0x6)
       	  dev-0x8 func-2
       	  dev-0x9 func-2
       	  dev-0xA func-2
       	  dev-0xB func-1,2,5,6
       	  dev-0x10 func-1,6
       	  dev-0x12 func-1,5
       	  
     */
  } Bits;
  UINT16 Data;
} CCR_N1_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* CLSR_HA_PMON_REG supported on:                                             */
/*       HSX (0x1019100C)                                                     */
/*       BDX (0x1019100C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_HA_PMON_REG 0x0301100C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * PCI Cache Line Size Register
 */
typedef union {
  struct {
    UINT8 cacheline_size : 8;
    /* cacheline_size - Bits[7:0], RO, default = 8'b00000000 
       Size of Cacheline
     */
  } Bits;
  UINT8 Data;
} CLSR_HA_PMON_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PLAT_HA_PMON_REG supported on:                                             */
/*       HSX (0x1019100D)                                                     */
/*       BDX (0x1019100D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_HA_PMON_REG 0x0301100D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * PCI Latency Timer
 */
typedef union {
  struct {
    UINT8 primary_latency_timer : 8;
    /* primary_latency_timer - Bits[7:0], RO, default = 8'b00000000 
       Not applicable to PCI-Express. Hardwired to 00h.
     */
  } Bits;
  UINT8 Data;
} PLAT_HA_PMON_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* HDR_HA_PMON_REG supported on:                                              */
/*       IVT_EP (0x1017100E)                                                  */
/*       IVT_EX (0x1017100E)                                                  */
/*       HSX (0x1019100E)                                                     */
/*       BDX (0x1019100E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_HA_PMON_REG 0x0301100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * PCI Header Type
 */
typedef union {
  struct {
    UINT8 configuration_layout : 7;
    /* configuration_layout - Bits[6:0], RO, default = 7'b0 
       Type 0 header
     */
    UINT8 multi_function_device : 1;
    /* multi_function_device - Bits[7:7], RO, default = 1'b1 
       This bit defaults to 1b since all these devices are multi-function
     */
  } Bits;
  UINT8 Data;
} HDR_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* BIST_HA_PMON_REG supported on:                                             */
/*       IVT_EP (0x1017100F)                                                  */
/*       IVT_EX (0x1017100F)                                                  */
/*       HSX (0x1019100F)                                                     */
/*       BDX (0x1019100F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_HA_PMON_REG 0x0301100F


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.1.CFG.xml.
 * PCI BIST Register
 */
typedef union {
  struct {
    UINT8 bist_tests : 8;
    /* bist_tests - Bits[7:0], RO, default = 8'b0 
       Not supported. Hardwired to 00h
     */
  } Bits;
  UINT8 Data;
} BIST_HA_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CAPPTR_HA_PMON_REG supported on:                                           */
/*       HSX (0x10191034)                                                     */
/*       BDX (0x10191034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_HA_PMON_REG 0x03011034

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * PCI Capability Pointer Register
 */
typedef union {
  struct {
    UINT8 capability_pointer : 8;
    /* capability_pointer - Bits[7:0], RO, default = 8'h0 
       Points to the first capability structure for the device which is the PCIe 
       capability. 
     */
  } Bits;
  UINT8 Data;
} CAPPTR_HA_PMON_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTL_HA_PMON_REG supported on:                                             */
/*       HSX (0x1019103C)                                                     */
/*       BDX (0x1019103C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_HA_PMON_REG 0x0301103C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * PCI Interrupt Line Register
 */
typedef union {
  struct {
    UINT8 interrupt_line : 8;
    /* interrupt_line - Bits[7:0], RO, default = 8'b0 
       N/A for these devices
     */
  } Bits;
  UINT8 Data;
} INTL_HA_PMON_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTPIN_HA_PMON_REG supported on:                                           */
/*       HSX (0x1019103D)                                                     */
/*       BDX (0x1019103D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_HA_PMON_REG 0x0301103D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * PCI Interrupt Pin Register
 */
typedef union {
  struct {
    UINT8 interrupt_pin : 8;
    /* interrupt_pin - Bits[7:0], RO, default = 8'b0 
       N/A since these devices do not generate any interrupt on their own
     */
  } Bits;
  UINT8 Data;
} INTPIN_HA_PMON_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MINGNT_HA_PMON_REG supported on:                                           */
/*       IVT_EP (0x1017103E)                                                  */
/*       IVT_EX (0x1017103E)                                                  */
/*       HSX (0x1019103E)                                                     */
/*       BDX (0x1019103E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_HA_PMON_REG 0x0301103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * PCI Min Grant Register
 */
typedef union {
  struct {
    UINT8 mgv : 8;
    /* mgv - Bits[7:0], RO, default = 8'b0 
       The device does not burst as a PCI compliant master.
     */
  } Bits;
  UINT8 Data;
} MINGNT_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_HA_PMON_REG supported on:                                           */
/*       IVT_EP (0x1017103F)                                                  */
/*       IVT_EX (0x1017103F)                                                  */
/*       HSX (0x1019103F)                                                     */
/*       BDX (0x1019103F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_HA_PMON_REG 0x0301103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * PCI Max Latency Register
 */
typedef union {
  struct {
    UINT8 mlv : 8;
    /* mlv - Bits[7:0], RO, default = 8'b00000000 
       The device has no specific requirements for how often it needs to access the PCI 
       bus. 
     */
  } Bits;
  UINT8 Data;
} MAXLAT_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* HAPERFMONADDRMATCH0_HA_PMON_REG supported on:                              */
/*       IVT_EP (0x40171040)                                                  */
/*       IVT_EX (0x40171040)                                                  */
/*       HSX (0x40191040)                                                     */
/*       BDX (0x40191040)                                                     */
/* Register default value:              0x00000000                            */
#define HAPERFMONADDRMATCH0_HA_PMON_REG 0x03014040
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * These registers are used to dump the contents of the home agent tracker contents 
 * and control states. 
 */
typedef union {
  struct {
    UINT32 rsvd : 6;
    /* rsvd - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 lophyaddr : 26;
    /* lophyaddr - Bits[31:6], RWS, default = 26'b00000000000000000000000000 
       This contains 26 bits of low physical address [31:6] of a cache line. The low 26 
       bits address of a architectural event imatch address are in the register. 
     */
  } Bits;
  UINT32 Data;
} HAPERFMONADDRMATCH0_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* HAPERFMONADDRMATCH1_HA_PMON_REG supported on:                              */
/*       IVT_EP (0x40171044)                                                  */
/*       IVT_EX (0x40171044)                                                  */
/*       HSX (0x40191044)                                                     */
/*       BDX (0x40191044)                                                     */
/* Register default value:              0x00000000                            */
#define HAPERFMONADDRMATCH1_HA_PMON_REG 0x03014044
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * These registers are used to dump the contents of the home agent tracker contents 
 * and control states. 
 */
typedef union {
  struct {
    UINT32 hiphyaddr : 14;
    /* hiphyaddr - Bits[13:0], RWS, default = 14'b00000000000000 
       This contains 14 bits of physical address [45:32] of a cache line. The high 14 
       bits address of a architectural event address match are in the register. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HAPERFMONADDRMATCH1_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* HAPERFMONOPCODEMATCH_HA_PMON_REG supported on:                             */
/*       IVT_EP (0x40171048)                                                  */
/*       IVT_EX (0x40171048)                                                  */
/*       HSX (0x40191048)                                                     */
/*       BDX (0x40191048)                                                     */
/* Register default value:              0x00000000                            */
#define HAPERFMONOPCODEMATCH_HA_PMON_REG 0x03014048
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * These registers are used to identify and record the transaction opcode from the 
 * home agent tracker. 
 */
typedef union {
  struct {
    UINT32 opcode : 6;
    /* opcode - Bits[5:0], RWS, default = 6'b000000 
       Home Agent Opcode Match Register (HaPerfmonOpcodeMatch): This field is used to 
       match the transaction opcode for identifying an architectural event. 
     */
    UINT32 rsvd : 26;
    /* rsvd - Bits[31:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HAPERFMONOPCODEMATCH_HA_PMON_STRUCT;
#endif /* ASM_INC */








/* MC5_ADDR_SHADOW0_HA_PMON_REG supported on:                                 */
/*       IVT_EP (0x40171060)                                                  */
/*       IVT_EX (0x40171060)                                                  */
/*       HSX (0x40191060)                                                     */
/*       BDX (0x40191060)                                                     */
/* Register default value:              0x00000000                            */
#define MC5_ADDR_SHADOW0_HA_PMON_REG 0x03014060
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * Low Physical Address of a cache line (LoPhyAddr)
 */
typedef union {
  struct {
    UINT32 rsvd : 6;
    /* rsvd - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 lophyaddr : 26;
    /* lophyaddr - Bits[31:6], RWS_V, default = 26'b00000000000000000000000000 
       This contains 26 bits of low physical address [31:6] of a cache line. It is used 
       to capture track address. 
     */
  } Bits;
  UINT32 Data;
} MC5_ADDR_SHADOW0_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* MC5_ADDR_SHADOW1_HA_PMON_REG supported on:                                 */
/*       IVT_EP (0x40171064)                                                  */
/*       IVT_EX (0x40171064)                                                  */
/*       HSX (0x40191064)                                                     */
/*       BDX (0x40191064)                                                     */
/* Register default value:              0x00000000                            */
#define MC5_ADDR_SHADOW1_HA_PMON_REG 0x03014064
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * High Physical Address of a cache line (HiPhyAddr)
 */
typedef union {
  struct {
    UINT32 hiphyaddr : 14;
    /* hiphyaddr - Bits[13:0], RWS_V, default = 14'b00000000000000 
       This contains 14 bits of physical address [45:32] of a cache line. It is used to 
       capture track address. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MC5_ADDR_SHADOW1_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* MC5_STATUS_SHADOW_N0_HA_PMON_REG supported on:                             */
/*       IVT_EP (0x40171068)                                                  */
/*       IVT_EX (0x40171068)                                                  */
/*       HSX (0x40191068)                                                     */
/*       BDX (0x40191068)                                                     */
/* Register default value:              0x00000000                            */
#define MC5_STATUS_SHADOW_N0_HA_PMON_REG 0x03014068


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.1.CFG.xml.
 * The MC5_Stat_Shadow register is a 64 bit CSR register. It copies MC5_Status 
 * register and contains information related to a machine check error if its VAL 
 * (valid) flag is set. SMM software is responsible for clearing the 
 * MC5_Stat_Shadow register by writing it with all 0's. Writing 1's to this 
 * register will cause a general protection fault to be generated. 
 * This CSR is a shadow of the respective HA Machine Check Bank MSR. Refer to MSR 
 * IA32_MC[7/8]_STATUS in this document for field Description details.      
 * Note: The Machine Check Bank for HA are actually MC7 (HA0) and MC8 (HA1), the 
 * MC5 name is legacy. 
 */
typedef union {
  struct {
    UINT32 mcacod : 16;
    /* mcacod - Bits[15:0], RWS_V, default = 16'b0000000000000000 
       Specifies the machine check architecture defined error code for the machine 
       check error condition detectedData Read Error: 0000 0000 1001 CCCC; 
       Generic Error: 0000 0000 1000 CCCC; HADB Parity, RTID out Range, BGF, etc.
       Write Error: 0000 0000 1010 CCCC; Partial Writ, etc.
       Replace CCCC with channel number.
       Ch 0 = 0000b
       Ch 1 = 0001b
       Ch 2 = 0010b
       Ch 3 = 0011b
       If the channel number can not be specified, such as request from CBo and RTID 
       out of range, use encode 1111. 
       Default: 0h
     */
    UINT32 mscod_bit_for_read_error : 1;
    /* mscod_bit_for_read_error - Bits[16:16], RWS_V, default = 1'b0 
       Specifies a model specific error code that uniquely identifies the machine check 
       error condition that has been detected. 
       1=Error logged was a HA read data error
       0=No error logged for HA read data error
     */
    UINT32 mscod_bit_for_the_data_buffer_parity_error : 1;
    /* mscod_bit_for_the_data_buffer_parity_error - Bits[17:17], RWS_V, default = 1'b0 
       Specifies a model specific error code that uniquely identifies the machine check 
       error condition that has been detected. 
       1=Error logged was a HA data buffer parity error
       0=No error logged for HADB parity error
     */
    UINT32 mscod_bit_for_partial_write_error : 1;
    /* mscod_bit_for_partial_write_error - Bits[18:18], RWS_V, default = 1'b0 
       Specifies a model specific error code that uniquely identifies the machine check 
       error condition that has been detected. 
       1=Error logged was a partial write error
       0=No error logged for partial write error
     */
    UINT32 mscod_bit_for_ha_receives_mc_data_timing_error : 1;
    /* mscod_bit_for_ha_receives_mc_data_timing_error - Bits[19:19], RWS_V, default = 1'b0 
       Specifies a model specific error code that uniquely identifies the machine check 
       error condition that has been detected. 
       
       1=Error logged was a error of HA receiving data from MC with timing violation
       0=No error logged for HA receives data from MC with timing violation
     */
    UINT32 mscod_bit_for_rtid_error : 1;
    /* mscod_bit_for_rtid_error - Bits[20:20], RWS_V, default = 1'b0 
       Specifies a model specific error code that uniquely identifies the machine check 
       error condition that has been detected. 
       
       1=Error logged was a RTID out of range error
       0=No error logged for RTID out of range error
     */
    UINT32 mscod_bit_for_bt_cfltarray_parity_error : 1;
    /* mscod_bit_for_bt_cfltarray_parity_error - Bits[21:21], RWS_V, default = 1'b0 
       Specifies a model specific error code that uniquely identifies the machine check 
       error condition that has been detected. 
       
       1=Error logged was BT cflt array parity error
       0=No error logged for BT cflt array parity error
     */
    UINT32 mscod_bit_for_bt_fifoarray_parity_error : 1;
    /* mscod_bit_for_bt_fifoarray_parity_error - Bits[22:22], RWS_V, default = 1'b0 
       Specifies a model specific error code that uniquely identifies the machine check 
       error condition that has been detected. 
       
       1=Error logged was BT fifo array parity error
       0=No error logged for BT fifo array parity error
     */
    UINT32 mscod_bit_for_bt_maparray_parity_error : 1;
    /* mscod_bit_for_bt_maparray_parity_error - Bits[23:23], RWS_V, default = 1'b0 
       Specifies a model specific error code that uniquely identifies the machine check 
       error condition that has been detected. 
       
       1=Error logged was BT map array parity error
       0=No error logged for BT map array parity error
     */
    UINT32 mscod_bit_for_bt_pldarray_parity_error : 1;
    /* mscod_bit_for_bt_pldarray_parity_error - Bits[24:24], RWS_V, default = 1'b0 
       Specifies a model specific error code that uniquely identifies the machine check 
       error condition that has been detected. 
       
       1=Error logged was BT payload array parity error
       0=No error logged for BT payload array parity error
     */
    UINT32 mscod_bit_for_bt_timeout_error : 1;
    /* mscod_bit_for_bt_timeout_error - Bits[25:25], RWS_V, default = 1'b0 
       Specifies a model specific error code that uniquely identifies the machine check 
       error condition that has been detected. 
       
       1=Error logged was BT timeout error
       0=No error logged for BT timeout error
     */
    UINT32 mscod_bit_for_hitme_parity_error : 1;
    /* mscod_bit_for_hitme_parity_error - Bits[26:26], RWS_V, default = 1'b0 
       Specifies a model specific error code that uniquely identifies the machine check 
       error condition that has been detected. 
       
       1=Error logged was a Hitme cache parity error
       0=No error logged for Hitme cache parity error
     */
    UINT32 enh_mca_avail0 : 5;
    /* enh_mca_avail0 - Bits[31:27], RWS_V, default = 5'b00000 
       These bits are read/writeable by software, and set to 0 when hardware writes the 
       register. 
     */
  } Bits;
  UINT32 Data;
} MC5_STATUS_SHADOW_N0_HA_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* MC5_STATUS_SHADOW_N1_HA_PMON_REG supported on:                             */
/*       IVT_EP (0x4017106C)                                                  */
/*       IVT_EX (0x4017106C)                                                  */
/*       HSX (0x4019106C)                                                     */
/*       BDX (0x4019106C)                                                     */
/* Register default value:              0x00000000                            */
#define MC5_STATUS_SHADOW_N1_HA_PMON_REG 0x0301406C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.1.CFG.xml.
 * The MC5_Stat_Shadow register is a 64 bit CSR register. It copies MC5_Status 
 * register and contains information related to a machine check error if its VAL 
 * (valid) flag is set. SMM software is responsible for clearing the 
 * MC5_Stat_Shadow register by writing it with all 0's. Writing 1's to this 
 * register will cause a general protection fault to be generated. 
 * This CSR is a shadow of the respective HA Machine Check Bank MSR. Refer to MSR 
 * IA32_MC[7/8]_STATUS in this document for field Description details.      
 * Note: The Machine Check Bank for HA are actually MC7 (HA0) and MC8 (HA1), the 
 * MC5 name is legacy. 
 */
typedef union {
  struct {
    UINT32 other_info : 6;
    /* other_info - Bits[5:0], RWS_V, default = 6'b000000 
       The functions of the bits in this field are implementation specific and are not 
       part of the machine check architecture. 
       Other_Info[5:4] - Unused (2'b0)
       Other_Info[3]: Trigger Event for History of Error (bit 35): Trigger event is 
       defined as 0 default. Turns to 1 when mc5 register are being updated while mc5 
       shadow status's valid bit still on (indicating we have not read the previous 
       error info from the shadow register yet) -> aka triggering event. 
       Other_Info[2]: History of Fatal Error (bit 34): history of fatal error 
       indication. 0 is default. When triggering event happens, turns to 1 if the 
       previous error is fatal error. 
       Other_Info[1]: History of recoverable Error (bit 33): history of recoverable 
       error indication. 0 is default. When triggering event happens, turns to 1 if the 
       previous error is a recoverable error 
       Other Info[0]: History of Correct Error (bit 32): history of corrected error 
       indication. 0 is default. When triggering event happens, turns to 1 if the 
       previous error is corrected error. 
     */
    UINT32 corrected_error_count : 15;
    /* corrected_error_count - Bits[20:6], RWS_V, default = 15'b000000000000000 
       A 15 bit counter that is incremented each time a corrected
       (or uncorrected but ignorable) error is observed by the MCA recording bank.
       Note: HA uses this counter to count its correctable occurrence.
     */
    UINT32 corrected_error_status_indicator : 2;
    /* corrected_error_status_indicator - Bits[22:21], RWS_V, default = 2'b00 
       These bits are used to indicate when the number of corrected errors has exceeded 
       the safe threshold to the point where an uncorrected error has become more 
       likely to happen 
       Note: The current MCA architecture uses this indicator for cache correctable 
       error 
       exclusively.
     */
    UINT32 ar : 1;
    /* ar - Bits[23:23], RWS_V, default = 1'b0 
       When set, indicates immediate recovery action is required. HA always sets this 
       bit to 0. 
     */
    UINT32 s : 1;
    /* s - Bits[24:24], RWS_V, default = 1'b0 
       When set, indicates an MCE/MSMI was generated for a recoverable error; when 
       clear, indicates a CMC/CSMI was generated. 
     */
    UINT32 pcc : 1;
    /* pcc - Bits[25:25], RWS_V, default = 1'b0 
       Indicates (when set) that the state of the processor might have been corrupted 
       by the error condition detected and that reliable restarting of the processor 
       may not be possible; 
       1= (legacy MCA error and Poison disabled) OR (Poisson enabled and non-poison 
       error) 
       0 = Poison enabled and uncorrectable error from memory
     */
    UINT32 addrv : 1;
    /* addrv - Bits[26:26], RWS_V, default = 1'b0 
       indicates (when set) that the MC5_ADDR register contains the address where the 
       error occurred 
     */
    UINT32 miscv : 1;
    /* miscv - Bits[27:27], RWS_V, default = 1'b0 
       Indicates (when set) that the MC5_MISC register contains additional information 
       regarding the error. 
     */
    UINT32 en : 1;
    /* en - Bits[28:28], RWS_V, default = 1'b0 
       Indicates (when set) that signaling of the machine check exception for this 
       error was enabled by an associated flag bit of the MCi_CTL register 
     */
    UINT32 uc : 1;
    /* uc - Bits[29:29], RWS_V, default = 1'b0 
       Error logged in this banks is an uncorrected error.
     */
    UINT32 over : 1;
    /* over - Bits[30:30], RWS_V, default = 1'b0 
       (Overflow) Indicates (when set) that a machine check error occurred while the 
       results of a previous error were still in the register bank 
     */
    UINT32 valid : 1;
    /* valid - Bits[31:31], RWS_V, default = 1'b0 
       Error logged in this bank is valid
       0= invalid; 1=valid
       Default 0b=invalid
       Note: When MC5 Shadow Mode is disabled, this bit should be clear.
     */
  } Bits;
  UINT32 Data;
} MC5_STATUS_SHADOW_N1_HA_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* MC5_MISC_SHADOW_N0_HA_PMON_REG supported on:                               */
/*       IVT_EP (0x40171070)                                                  */
/*       IVT_EX (0x40171070)                                                  */
/*       HSX (0x40191070)                                                     */
/*       BDX (0x40191070)                                                     */
/* Register default value:              0x00000086                            */
#define MC5_MISC_SHADOW_N0_HA_PMON_REG 0x03014070


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.1.CFG.xml.
 * This register is used for shadowing the MCA miscellaneous register. Some of the 
 * errors are architectural errors, other errors are not. 
 * - The errors of a group of node controller transactions can not be exclusively 
 * recorded due to complexity of the design since we record information of the 
 * original 
 * request instead of the responses. These transactions are: RspFwdIWb, RspFwdSWb, 
 * RspCnfltWbI. 
 * - The failed transaction node ID and QPI link ID can be obtained from Request 
 * node ID of failed transaction field at this status register. 
 * This is a 64 bits register.
 * This CSR is a shadow of the respective HA Machine Check Bank MSR. Refer to MSR 
 * IA32_MC[7/8]_MISC in this document for field Description details.      
 * Note that the Machine Check Bank for HA is actually MC7(HA0) and MC8(HA1), the 
 * MC5 name is legacy.  
 */
typedef union {
  struct {
    UINT32 addrlsb : 6;
    /* addrlsb - Bits[5:0], RWS, default = 6'b000110 
       Recoverable address LSB. The lowest valid recoverable address bit. Indicates the 
       position of the least significant bit (LSB) of the recoverable error address. 
       This should be set to 6. 
     */
    UINT32 addrmode : 3;
    /* addrmode - Bits[8:6], RWS, default = 3'b010 
       Address mode for the address logged in IA32_MCi_ADDR. The supported address 
       modes are: 000 = Segment Offset; 001 = Linear Address; 010 = Physical Address; 
       011 = Memory Address; 100 to 110 = Reserved; 111 = Generic 
     */
    UINT32 htid : 7;
    /* htid - Bits[15:9], RWS_V, default = 7'b0000000 
       HTID of a failed transaction
     */
    UINT32 enh_mca_avail0 : 1;
    /* enh_mca_avail0 - Bits[16:16], RWS_V, default = 1'b0 
       These bits are read/writeable by software, and set to 0 when hardware writes the 
       register. 
     */
    UINT32 rtid : 6;
    /* rtid - Bits[22:17], RWS_V, default = 6'b000000 
       HTID of a failed transaction
     */
    UINT32 enh_mca_avail1 : 2;
    /* enh_mca_avail1 - Bits[24:23], RWS_V, default = 2'b00 
       These bits are read/writeable by software, and set to 0 when hardware writes the 
       register. 
     */
    UINT32 rnid : 4;
    /* rnid - Bits[28:25], RWS_V, default = 4'b0000  */
    UINT32 reqopcode : 3;
    /* reqopcode - Bits[31:29], RWS_V, default = 6'b000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       Opcode of failed transaction
       [34] bit5: Read Current Request (RDCUR)
       [33] bit4: NonSnp Request (NONSNPRD, NONSNPWR0
       [32] bit3: Read Data Request (RDDATA, RDDATAMIG)
       [31] bit2: Write Request (NONSNPWR, WRMTO, ACKCNFLT, ACKCNFLTWBI)
       [30] bit1: Read Operation (RDCODE, RDDATA, NONSNPRD, RDINVOWN, RDCUR,
       RDDATAMIG)
       [29] bit0: Invalidate Operation (INVITOE)
     */
  } Bits;
  UINT32 Data;
} MC5_MISC_SHADOW_N0_HA_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* MC5_MISC_SHADOW_N1_HA_PMON_REG supported on:                               */
/*       IVT_EP (0x40171074)                                                  */
/*       IVT_EX (0x40171074)                                                  */
/*       HSX (0x40191074)                                                     */
/*       BDX (0x40191074)                                                     */
/* Register default value:              0x00000000                            */
#define MC5_MISC_SHADOW_N1_HA_PMON_REG 0x03014074


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.1.CFG.xml.
 * This register is used for shadowing the MCA miscellaneous register. Some of the 
 * errors are architectural errors, other errors are not. 
 * - The errors of a group of node controller transactions can not be exclusively 
 * recorded due to complexity of the design since we record information of the 
 * original 
 * request instead of the responses. These transactions are: RspFwdIWb, RspFwdSWb, 
 * RspCnfltWbI. 
 * - The failed transaction node ID and QPI link ID can be obtained from Request 
 * node ID of failed transaction field at this status register. 
 * This is a 64 bits register.
 * This CSR is a shadow of the respective HA Machine Check Bank MSR. Refer to MSR 
 * IA32_MC[7/8]_MISC in this document for field Description details.      
 * Note that the Machine Check Bank for HA is actually MC7(HA0) and MC8(HA1), the 
 * MC5 name is legacy.  
 */
typedef union {
  struct {
    UINT32 reqopcode : 3;
    /* reqopcode - Bits[2:0], RWS_V, default = 6'b000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       Opcode of failed transaction
       [34] bit5: Read Current Request (RDCUR)
       [33] bit4: NonSnp Request (NONSNPRD, NONSNPWR0
       [32] bit3: Read Data Request (RDDATA, RDDATAMIG)
       [31] bit2: Write Request (NONSNPWR, WRMTO, ACKCNFLT, ACKCNFLTWBI)
       [30] bit1: Read Operation (RDCODE, RDDATA, NONSNPRD, RDINVOWN, RDCUR,
       RDDATAMIG)
       [29] bit0: Invalidate Operation (INVITOE)
     */
    UINT32 enh_mca_avail2 : 2;
    /* enh_mca_avail2 - Bits[4:3], RWS_V, default = 2'b00 
       These bits are read/writeable by software, and set to 0 when hardware writes the 
       register. 
     */
    UINT32 pe : 2;
    /* pe - Bits[6:5], RWS_V, default = 2'b00 
       Priority of failed transaction
       00: low priority
       11: critical priority
     */
    UINT32 hadbbank : 2;
    /* hadbbank - Bits[8:7], RWS_V, default = 2'b00 
       The parity error location of a HA data bank ID.
       00b = Reserved
       01b = Critical half data bank
       10b = Non-critical half data bank
       11b = Both banks
     */
    UINT32 failover : 1;
    /* failover - Bits[9:9], RWS_V, default = 1'b0 
       An error occurs at a pair of mirrored memory channels. Error was corrected by 
       mirroring with channel failover. 
     */
    UINT32 mirrorcorr : 1;
    /* mirrorcorr - Bits[10:10], RWS_V, default = 1'b0 
       Error was corrected by mirroring and primary channel scrubbed successfully.
     */
    UINT32 btidto : 9;
    /* btidto - Bits[19:11], RWS_V, default = 9'b000000000 
       Backup tracker index of the first timed out transaction.
       On a time out (logged in MC5_STATUS.MCACOD), the MISCV bit will not get set and 
       this BTIDTO field will be the only field that is accurate in the MC5_MISC 
       register. All other fields will contain bogus information. 
     */
    UINT32 enh_mca_avail3 : 12;
    /* enh_mca_avail3 - Bits[31:20], RWS_V, default = 12'b000000000000 
       These bits are read/writeable by software, and set to 0 when hardware writes the 
       register. 
     */
  } Bits;
  UINT32 Data;
} MC5_MISC_SHADOW_N1_HA_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CNTR_0_N0_HA_PMON_REG supported on:                                        */
/*       IVT_EP (0x401710A0)                                                  */
/*       IVT_EX (0x401710A0)                                                  */
/*       HSX (0x401910A0)                                                     */
/*       BDX (0x401910A0)                                                     */
/* Register default value:              0x00000000                            */
#define CNTR_0_N0_HA_PMON_REG 0x030140A0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * This register is performance monitor counter #0.  Software can both read it and 
 * write it. 
 */
typedef union {
  struct {
    UINT32 counter_value : 32;
    /* counter_value - Bits[31:0], RW_V, default = 48'b000000000000000000000000000000000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This is the current value of the counter.
     */
  } Bits;
  UINT32 Data;
} CNTR_0_N0_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* CNTR_0_N1_HA_PMON_REG supported on:                                        */
/*       IVT_EP (0x401710A4)                                                  */
/*       IVT_EX (0x401710A4)                                                  */
/*       HSX (0x401910A4)                                                     */
/*       BDX (0x401910A4)                                                     */
/* Register default value:              0x00000000                            */
#define CNTR_0_N1_HA_PMON_REG 0x030140A4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * This register is performance monitor counter #0.  Software can both read it and 
 * write it. 
 */
typedef union {
  struct {
    UINT32 counter_value : 16;
    /* counter_value - Bits[15:0], RW_V, default = 48'b000000000000000000000000000000000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This is the current value of the counter.
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CNTR_0_N1_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* CNTR_1_N0_HA_PMON_REG supported on:                                        */
/*       IVT_EP (0x401710A8)                                                  */
/*       IVT_EX (0x401710A8)                                                  */
/*       HSX (0x401910A8)                                                     */
/*       BDX (0x401910A8)                                                     */
/* Register default value:              0x00000000                            */
#define CNTR_1_N0_HA_PMON_REG 0x030140A8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * This register is performance monitor counter #1.  Software can both read it and 
 * write it. 
 */
typedef union {
  struct {
    UINT32 counter_value : 32;
    /* counter_value - Bits[31:0], RW_V, default = 48'b000000000000000000000000000000000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This is the current value of the counter.
     */
  } Bits;
  UINT32 Data;
} CNTR_1_N0_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* CNTR_1_N1_HA_PMON_REG supported on:                                        */
/*       IVT_EP (0x401710AC)                                                  */
/*       IVT_EX (0x401710AC)                                                  */
/*       HSX (0x401910AC)                                                     */
/*       BDX (0x401910AC)                                                     */
/* Register default value:              0x00000000                            */
#define CNTR_1_N1_HA_PMON_REG 0x030140AC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * This register is performance monitor counter #1.  Software can both read it and 
 * write it. 
 */
typedef union {
  struct {
    UINT32 counter_value : 16;
    /* counter_value - Bits[15:0], RW_V, default = 48'b000000000000000000000000000000000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This is the current value of the counter.
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CNTR_1_N1_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* CNTR_2_N0_HA_PMON_REG supported on:                                        */
/*       IVT_EP (0x401710B0)                                                  */
/*       IVT_EX (0x401710B0)                                                  */
/*       HSX (0x401910B0)                                                     */
/*       BDX (0x401910B0)                                                     */
/* Register default value:              0x00000000                            */
#define CNTR_2_N0_HA_PMON_REG 0x030140B0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * This register is performance monitor counter #2.  Software can both read it and 
 * write it. 
 */
typedef union {
  struct {
    UINT32 counter_value : 32;
    /* counter_value - Bits[31:0], RW_V, default = 48'b000000000000000000000000000000000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This is the current value of the counter.
     */
  } Bits;
  UINT32 Data;
} CNTR_2_N0_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* CNTR_2_N1_HA_PMON_REG supported on:                                        */
/*       IVT_EP (0x401710B4)                                                  */
/*       IVT_EX (0x401710B4)                                                  */
/*       HSX (0x401910B4)                                                     */
/*       BDX (0x401910B4)                                                     */
/* Register default value:              0x00000000                            */
#define CNTR_2_N1_HA_PMON_REG 0x030140B4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * This register is performance monitor counter #2.  Software can both read it and 
 * write it. 
 */
typedef union {
  struct {
    UINT32 counter_value : 16;
    /* counter_value - Bits[15:0], RW_V, default = 48'b000000000000000000000000000000000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This is the current value of the counter.
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CNTR_2_N1_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* CNTR_3_N0_HA_PMON_REG supported on:                                        */
/*       IVT_EP (0x401710B8)                                                  */
/*       IVT_EX (0x401710B8)                                                  */
/*       HSX (0x401910B8)                                                     */
/*       BDX (0x401910B8)                                                     */
/* Register default value:              0x00000000                            */
#define CNTR_3_N0_HA_PMON_REG 0x030140B8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * This register is performance monitor counter #3.  Software can both read it and 
 * write it. 
 */
typedef union {
  struct {
    UINT32 counter_value : 32;
    /* counter_value - Bits[31:0], RW_V, default = 48'b000000000000000000000000000000000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This is the current value of the counter.
     */
  } Bits;
  UINT32 Data;
} CNTR_3_N0_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* CNTR_3_N1_HA_PMON_REG supported on:                                        */
/*       IVT_EP (0x401710BC)                                                  */
/*       IVT_EX (0x401710BC)                                                  */
/*       HSX (0x401910BC)                                                     */
/*       BDX (0x401910BC)                                                     */
/* Register default value:              0x00000000                            */
#define CNTR_3_N1_HA_PMON_REG 0x030140BC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * This register is performance monitor counter #3.  Software can both read it and 
 * write it. 
 */
typedef union {
  struct {
    UINT32 counter_value : 16;
    /* counter_value - Bits[15:0], RW_V, default = 48'b000000000000000000000000000000000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This is the current value of the counter.
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CNTR_3_N1_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* CNTR_4_N0_HA_PMON_REG supported on:                                        */
/*       IVT_EP (0x401710C0)                                                  */
/*       IVT_EX (0x401710C0)                                                  */
/*       HSX (0x401910C0)                                                     */
/*       BDX (0x401910C0)                                                     */
/* Register default value:              0x00000000                            */
#define CNTR_4_N0_HA_PMON_REG 0x030140C0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * This register is performance monitor counter #4.  Software can both read it and 
 * write it. 
 */
typedef union {
  struct {
    UINT32 counter_value : 32;
    /* counter_value - Bits[31:0], RW_V, default = 48'b000000000000000000000000000000000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This is the current value of the counter.
     */
  } Bits;
  UINT32 Data;
} CNTR_4_N0_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* CNTR_4_N1_HA_PMON_REG supported on:                                        */
/*       IVT_EP (0x401710C4)                                                  */
/*       IVT_EX (0x401710C4)                                                  */
/*       HSX (0x401910C4)                                                     */
/*       BDX (0x401910C4)                                                     */
/* Register default value:              0x00000000                            */
#define CNTR_4_N1_HA_PMON_REG 0x030140C4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * This register is performance monitor counter #4.  Software can both read it and 
 * write it. 
 */
typedef union {
  struct {
    UINT32 counter_value : 16;
    /* counter_value - Bits[15:0], RW_V, default = 48'b000000000000000000000000000000000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This is the current value of the counter.
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CNTR_4_N1_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* CNTRCFG_0_HA_PMON_REG supported on:                                        */
/*       IVT_EP (0x401710D8)                                                  */
/*       IVT_EX (0x401710D8)                                                  */
/*       HSX (0x401910D8)                                                     */
/*       BDX (0x401910D8)                                                     */
/* Register default value:              0x00000000                            */
#define CNTRCFG_0_HA_PMON_REG 0x030140D8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * Perfmon Counter #0 Control Register
 */
typedef union {
  struct {
    UINT32 event_select : 8;
    /* event_select - Bits[7:0], RW_V, default = 8'b00000000 
       This field is used to decode the PerfMon event which is selected. The encodings 
       for each of the valid UnCore PerfMon events can be found in the respective 
       Performance Monitoring documentation. 
     */
    UINT32 unit_mask : 8;
    /* unit_mask - Bits[15:8], RW_V, default = 8'b00000000 
       This mask selects the sub-events to be selected for creation of the event. The 
       selected sub-events are bitwise OR-ed together to create event. At least one 
       sub-event must be selected otherwise the PerfMon event signals will not ever get 
       asserted. Events with no sub-events listed effectively have only one sub-event 
       =event -1 bit 8 must be set to 1 in this case. 
     */
    UINT32 queue_occupancy_reset : 1;
    /* queue_occupancy_reset - Bits[16:16], WO, default = 1'b0 
       This write only bit causes the queue occupancy counter of the PerfMon counter 
       for which this Perf event select register is associated to be cleared to all 
       zeroes when a '1' is written to it. No action is taken when a '0' is written. 
     */
    UINT32 counter_reset : 1;
    /* counter_reset - Bits[17:17], WO, default = 1'b0 
       When this bit is set, the corresponding counter will be reset to 0. This allows 
       for a quick reset of the counter when changing event encodings. 
     */
    UINT32 edge_detect : 1;
    /* edge_detect - Bits[18:18], RW_V, default = 1'b0 
       Edge Detect allows one to count either 0 to 1 or 1 to 0 transitions of a given 
       event. For example, we have an event that counts the number of cycles in L1 mode 
       in Intel QPI. By using edge detect, one can count the number of times that we 
       entered L1 mode (by detecting the rising edge). 
       
       Edge detect only works in conjunction with threshholding. This is true even for 
       events that can only increment by 1 in a given cycle (like the L1 example 
       above). In this case, one should set a threshhold of 1. One can also use Edge 
       Detect with queue occupancy events. For example, if one wanted to count the 
       number of times when the TOR occupancy was larger than 5, one would selet the 
       TOR occupancy event with a threshold of 5 and set the Edge Detect bit. 
       
       Edge detect can also be used with the invert. This is generally not particularly 
       useful, as the count of falling edges compared to rising edges will always on 
       differ by 1. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 overflow_enable : 1;
    /* overflow_enable - Bits[20:20], RW_V, default = 1'b0 
       Setting this bit will enable the counter to send an overflow signal. If this bit 
       is not set, the counter will wrap around when it overflows without triggering 
       anything. If this bit is set and the Unit's configuration register has Overflow 
       enabled, then a signal will be transmitted to the Ubox. 
     */
    UINT32 rsvd_21 : 1;
    UINT32 counter_enable : 1;
    /* counter_enable - Bits[22:22], RW_V, default = 1'b0 
       This field is the local enable for the PerfMon Counter. This bit must be 
       asserted in order for the PerfMon counter to begin counting the events selected 
       by the 'event select', 'unit mask', and 'internal' bits (see the fields below). 
       There is one bit per PerfMon Counter. Note that if this bit is set to 1 but the 
       Unit Control Registers have determined that counting is disabled, then the 
       counter will not count. 
     */
    UINT32 invert : 1;
    /* invert - Bits[23:23], RW_V, default = 1'b0 
       This bit indicates how the threshold field will be compared to the incoming 
       event. When 0, the comparison that will be done is threshold >= event. When set 
       to 1, the comparison that will be done is inverted from the case where this bit 
       is set to 0, i.e., threshold < event. The invert bit only works when Threshhold 
       != 0. So, if one would like to invert a non-occupancy event (like LLC Hit), one 
       needs to set the threshhold to 1. 
     */
    UINT32 threshold : 8;
    /* threshold - Bits[31:24], RW_V, default = 8'b00000000 
       This field is compared directly against an incoming event value for events that 
       can increment by 1 or more in a given cycle. Since the widest event from the 
       UnCore is 7bits (queue occupancy), bit 31 is unused. The result of the 
       comparison is effectively a 1 bit wide event, i.e., the counter will be 
       incremented by 1 when the comparison is true (the type of comparison depends on 
       the setting of the 'invert' bit - see bit 23 below) no matter how wide the 
       original event was. When this field is zero, threshold comparison is disabled 
       and the event is passed without modification. 
     */
  } Bits;
  UINT32 Data;
} CNTRCFG_0_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* CNTRCFG_1_HA_PMON_REG supported on:                                        */
/*       IVT_EP (0x401710DC)                                                  */
/*       IVT_EX (0x401710DC)                                                  */
/*       HSX (0x401910DC)                                                     */
/*       BDX (0x401910DC)                                                     */
/* Register default value:              0x00000000                            */
#define CNTRCFG_1_HA_PMON_REG 0x030140DC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * Perfmon Counter #1 Control Register
 */
typedef union {
  struct {
    UINT32 event_select : 8;
    /* event_select - Bits[7:0], RW_V, default = 8'b00000000 
       This field is used to decode the PerfMon event which is selected. The encodings 
       for each of the valid UnCore PerfMon events can be found in the respective 
       Performance Monitoring documentation. 
     */
    UINT32 unit_mask : 8;
    /* unit_mask - Bits[15:8], RW_V, default = 8'b00000000 
       This mask selects the sub-events to be selected for creation of the event. The 
       selected sub-events are bitwise OR-ed together to create event. At least one 
       sub-event must be selected otherwise the PerfMon event signals will not ever get 
       asserted. Events with no sub-events listed effectively have only one sub-event 
       =event -1 bit 8 must be set to 1 in this case. 
     */
    UINT32 queue_occupancy_reset : 1;
    /* queue_occupancy_reset - Bits[16:16], WO, default = 1'b0 
       This write only bit causes the queue occupancy counter of the PerfMon counter 
       for which this Perf event select register is associated to be cleared to all 
       zeroes when a '1' is written to it. No action is taken when a '0' is written. 
     */
    UINT32 counter_reset : 1;
    /* counter_reset - Bits[17:17], WO, default = 1'b0 
       When this bit is set, the corresponding counter will be reset to 0. This allows 
       for a quick reset of the counter when changing event encodings. 
     */
    UINT32 edge_detect : 1;
    /* edge_detect - Bits[18:18], RW_V, default = 1'b0 
       Edge Detect allows one to count either 0 to 1 or 1 to 0 transitions of a given 
       event. For example, we have an event that counts the number of cycles in L1 mode 
       in Intel QPI. By using edge detect, one can count the number of times that we 
       entered L1 mode (by detecting the rising edge). 
       >
       > Edge detect only works in conjunction with threshholding. This is true even 
       for events that can only increment by 1 in a given cycle (like the L1 example 
       above). In this case, one should set a threshhold of 1. One can also use Edge 
       Detect with queue occupancy events. For example, if one wanted to count the 
       number of times when the TOR occupancy was larger than 5, one would selet the 
       TOR occupancy event with a threshold of 5 and set the Edge Detect bit. 
       >
       > Edge detect can also be used with the invert. This is generally not 
       particularly useful, as the count of falling edges compared to rising edges will 
       always on differ by 1. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 overflow_enable : 1;
    /* overflow_enable - Bits[20:20], RW_V, default = 1'b0 
       Setting this bit will enable the counter to send an overflow signal. If this bit 
       is not set, the counter will wrap around when it overflows without triggering 
       anything. If this bit is set and the Unit's configuration register has Overflow 
       enabled, then a signal will be transmitted to the Ubox. 
     */
    UINT32 rsvd_21 : 1;
    UINT32 counter_enable : 1;
    /* counter_enable - Bits[22:22], RW_V, default = 1'b0 
       This field is the local enable for the PerfMon Counter. This bit must be 
       asserted in order for the PerfMon counter to begin counting the events selected 
       by the 'event select', 'unit mask', and 'internal' bits (see the fields below). 
       There is one bit per PerfMon Counter. Note that if this bit is set to 1 but the 
       Unit Control Registers have determined that counting is disabled, then the 
       counter will not count. 
     */
    UINT32 invert : 1;
    /* invert - Bits[23:23], RW_V, default = 1'b0 
       This bit indicates how the threshold field will be compared to the incoming 
       event. When 0, the comparison that will be done is threshold >= event. When set 
       to 1, the comparison that will be done is inverted from the case where this bit 
       is set to 0, i.e., threshold < event. The invert bit only works when Threshhold 
       != 0. So, if one would like to invert a non-occupancy event (like LLC Hit), one 
       needs to set the threshhold to 1. 
     */
    UINT32 threshold : 8;
    /* threshold - Bits[31:24], RW_V, default = 8'b00000000 
       This field is compared directly against an incoming event value for events that 
       can increment by 1 or more in a given cycle. Since the widest event from the 
       UnCore is 7bits (queue occupancy), bit 31 is unused. The result of the 
       comparison is effectively a 1 bit wide event, i.e., the counter will be 
       incremented by 1 when the comparison is true (the type of comparison depends on 
       the setting of the 'invert' bit - see bit 23 below) no matter how wide the 
       original event was. When this field is zero, threshold comparison is disabled 
       and the event is passed without modification. 
     */
  } Bits;
  UINT32 Data;
} CNTRCFG_1_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* CNTRCFG_2_HA_PMON_REG supported on:                                        */
/*       IVT_EP (0x401710E0)                                                  */
/*       IVT_EX (0x401710E0)                                                  */
/*       HSX (0x401910E0)                                                     */
/*       BDX (0x401910E0)                                                     */
/* Register default value:              0x00000000                            */
#define CNTRCFG_2_HA_PMON_REG 0x030140E0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * Perfmon Counter #2 Control Register
 */
typedef union {
  struct {
    UINT32 event_select : 8;
    /* event_select - Bits[7:0], RW_V, default = 8'b00000000 
       This field is used to decode the PerfMon event which is selected. The encodings 
       for each of the valid UnCore PerfMon events can be found in the respective 
       Performance Monitoring documentation. 
     */
    UINT32 unit_mask : 8;
    /* unit_mask - Bits[15:8], RW_V, default = 8'b00000000 
       This field is used to decode the PerfMon event which is selected. The encodings 
       for each of the valid UnCore PerfMon events can be found in the respective 
       Performance Monitoring documentation. 
     */
    UINT32 queue_occupancy_reset : 1;
    /* queue_occupancy_reset - Bits[16:16], WO, default = 1'b0 
       This write only bit causes the queue occupancy counter of the PerfMon counter 
       for which this Perf event select register is associated to be cleared to all 
       zeroes when a '1' is written to it. No action is taken when a '0' is written. 
     */
    UINT32 counter_reset : 1;
    /* counter_reset - Bits[17:17], WO, default = 1'b0 
       When this bit is set, the corresponding counter will be reset to 0. This allows 
       for a quick reset of the counter when changing event encodings. 
     */
    UINT32 edge_detect : 1;
    /* edge_detect - Bits[18:18], RW_V, default = 1'b0 
       Edge Detect allows one to count either 0 to 1 or 1 to 0 transitions of a given 
       event. For example, we have an event that counts the number of cycles in L1 mode 
       in Intel QPI. By using edge detect, one can count the number of times that we 
       entered L1 mode (by detecting the rising edge). 
       >
       > Edge detect only works in conjunction with threshholding. This is true even 
       for events that can only increment by 1 in a given cycle (like the L1 example 
       above). In this case, one should set a threshhold of 1. One can also use Edge 
       Detect with queue occupancy events. For example, if one wanted to count the 
       number of times when the TOR occupancy was larger than 5, one would selet the 
       TOR occupancy event with a threshold of 5 and set the Edge Detect bit. 
       >
       > Edge detect can also be used with the invert. This is generally not 
       particularly useful, as the count of falling edges compared to rising edges will 
       always on differ by 1. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 overflow_enable : 1;
    /* overflow_enable - Bits[20:20], RW_V, default = 1'b0 
       Setting this bit will enable the counter to send an overflow signal. If this bit 
       is not set, the counter will wrap around when it overflows without triggering 
       anything. If this bit is set and the Unit's configuration register has Overflow 
       enabled, then a signal will be transmitted to the Ubox. 
     */
    UINT32 rsvd_21 : 1;
    UINT32 counter_enable : 1;
    /* counter_enable - Bits[22:22], RW_V, default = 1'b0 
       This field is the local enable for the PerfMon Counter. This bit must be 
       asserted in order for the PerfMon counter to begin counting the events selected 
       by the 'event select', 'unit mask', and 'internal' bits (see the fields below). 
       There is one bit per PerfMon Counter. Note that if this bit is set to 1 but the 
       Unit Control Registers have determined that counting is disabled, then the 
       counter will not count. 
     */
    UINT32 invert : 1;
    /* invert - Bits[23:23], RW_V, default = 1'b0 
       This bit indicates how the threshold field will be compared to the incoming 
       event. When 0, the comparison that will be done is threshold >= event. When set 
       to 1, the comparison that will be done is inverted from the case where this bit 
       is set to 0, i.e., threshold < event. The invert bit only works when Threshhold 
       != 0. So, if one would like to invert a non-occupancy event (like LLC Hit), one 
       needs to set the threshhold to 1. 
     */
    UINT32 threshold : 8;
    /* threshold - Bits[31:24], RW_V, default = 8'b00000000 
       This field is compared directly against an incoming event value for events that 
       can increment by 1 or more in a given cycle. Since the widest event from the 
       UnCore is 7bits (queue occupancy), bit 31 is unused. The result of the 
       comparison is effectively a 1 bit wide event, i.e., the counter will be 
       incremented by 1 when the comparison is true (the type of comparison depends on 
       the setting of the 'invert' bit - see bit 23 below) no matter how wide the 
       original event was. When this field is zero, threshold comparison is disabled 
       and the event is passed without modification. 
     */
  } Bits;
  UINT32 Data;
} CNTRCFG_2_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* CNTRCFG_3_HA_PMON_REG supported on:                                        */
/*       IVT_EP (0x401710E4)                                                  */
/*       IVT_EX (0x401710E4)                                                  */
/*       HSX (0x401910E4)                                                     */
/*       BDX (0x401910E4)                                                     */
/* Register default value:              0x00000000                            */
#define CNTRCFG_3_HA_PMON_REG 0x030140E4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * Perfmon Counter #3 Control Register
 */
typedef union {
  struct {
    UINT32 event_select : 8;
    /* event_select - Bits[7:0], RW_V, default = 8'b00000000 
       This field is used to decode the PerfMon event which is selected. The encodings 
       for each of the valid UnCore PerfMon events can be found in the respective 
       Performance Monitoring documentation. 
     */
    UINT32 unit_mask : 8;
    /* unit_mask - Bits[15:8], RW_V, default = 8'b00000000 
       This mask selects the sub-events to be selected for creation of the event. The 
       selected sub-events are bitwise OR-ed together to create event. At least one 
       sub-event must be selected otherwise the PerfMon event signals will not ever get 
       asserted. Events with no sub-events listed effectively have only one sub-event 
       =event -1 bit 8 must be set to 1 in this case. 
     */
    UINT32 queue_occupancy_reset : 1;
    /* queue_occupancy_reset - Bits[16:16], WO, default = 1'b0 
       This write only bit causes the queue occupancy counter of the PerfMon counter 
       for which this Perf event select register is associated to be cleared to all 
       zeroes when a '1' is written to it. No action is taken when a '0' is written. 
     */
    UINT32 counter_reset : 1;
    /* counter_reset - Bits[17:17], WO, default = 1'b0 
       When this bit is set, the corresponding counter will be reset to 0. This allows 
       for a quick reset of the counter when changing event encodings. 
     */
    UINT32 edge_detect : 1;
    /* edge_detect - Bits[18:18], RW_V, default = 1'b0 
       Edge Detect allows one to count either 0 to 1 or 1 to 0 transitions of a given 
       event. For example, we have an event that counts the number of cycles in L1 mode 
       in Intel QPI. By using edge detect, one can count the number of times that we 
       entered L1 mode (by detecting the rising edge). 
       >
       > Edge detect only works in conjunction with threshholding. This is true even 
       for events that can only increment by 1 in a given cycle (like the L1 example 
       above). In this case, one should set a threshhold of 1. One can also use Edge 
       Detect with queue occupancy events. For example, if one wanted to count the 
       number of times when the TOR occupancy was larger than 5, one would selet the 
       TOR occupancy event with a threshold of 5 and set the Edge Detect bit. 
       >
       > Edge detect can also be used with the invert. This is generally not 
       particularly useful, as the count of falling edges compared to rising edges will 
       always on differ by 1. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 overflow_enable : 1;
    /* overflow_enable - Bits[20:20], RW_V, default = 1'b0 
       Setting this bit will enable the counter to send an overflow signal. If this bit 
       is not set, the counter will wrap around when it overflows without triggering 
       anything. If this bit is set and the Unit's configuration register has Overflow 
       enabled, then a signal will be transmitted to the Ubox. 
     */
    UINT32 rsvd_21 : 1;
    UINT32 counter_enable : 1;
    /* counter_enable - Bits[22:22], RW_V, default = 1'b0 
       This field is the local enable for the PerfMon Counter. This bit must be 
       asserted in order for the PerfMon counter to begin counting the events selected 
       by the 'event select', 'unit mask', and 'internal' bits (see the fields below). 
       There is one bit per PerfMon Counter. Note that if this bit is set to 1 but the 
       Unit Control Registers have determined that counting is disabled, then the 
       counter will not count. 
     */
    UINT32 invert : 1;
    /* invert - Bits[23:23], RW_V, default = 1'b0 
       This bit indicates how the threshold field will be compared to the incoming 
       event. When 0, the comparison that will be done is threshold >= event. When set 
       to 1, the comparison that will be done is inverted from the case where this bit 
       is set to 0, i.e., threshold < event. The invert bit only works when Threshhold 
       != 0. So, if one would like to invert a non-occupancy event (like LLC Hit), one 
       needs to set the threshhold to 1. 
     */
    UINT32 threshold : 8;
    /* threshold - Bits[31:24], RW_V, default = 8'b00000000 
       This field is compared directly against an incoming event value for events that 
       can increment by 1 or more in a given cycle. Since the widest event from the 
       UnCore is 7bits (queue occupancy), bit 31 is unused. The result of the 
       comparison is effectively a 1 bit wide event, i.e., the counter will be 
       incremented by 1 when the comparison is true (the type of comparison depends on 
       the setting of the 'invert' bit - see bit 23 below) no matter how wide the 
       original event was. When this field is zero, threshold comparison is disabled 
       and the event is passed without modification. 
     */
  } Bits;
  UINT32 Data;
} CNTRCFG_3_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* CNTRCFG_4_HA_PMON_REG supported on:                                        */
/*       IVT_EP (0x401710E8)                                                  */
/*       IVT_EX (0x401710E8)                                                  */
/*       HSX (0x401910E8)                                                     */
/*       BDX (0x401910E8)                                                     */
/* Register default value:              0x00000000                            */
#define CNTRCFG_4_HA_PMON_REG 0x030140E8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * Perfmon Counter #4 Control Register
 */
typedef union {
  struct {
    UINT32 event_select : 8;
    /* event_select - Bits[7:0], RW_V, default = 8'b00000000 
       This field is used to decode the PerfMon event which is selected. The encodings 
       for each of the valid UnCore PerfMon events can be found in the respective 
       Performance Monitoring documentation. 
     */
    UINT32 unit_mask : 8;
    /* unit_mask - Bits[15:8], RW_V, default = 8'b00000000 
       This mask selects the sub-events to be selected for creation of the event. The 
       selected sub-events are bitwise OR-ed together to create event. At least one 
       sub-event must be selected otherwise the PerfMon event signals will not ever get 
       asserted. Events with no sub-events listed effectively have only one sub-event 
       =event -1 bit 8 must be set to 1 in this case. 
     */
    UINT32 queue_occupancy_reset : 1;
    /* queue_occupancy_reset - Bits[16:16], WO, default = 1'b0 
       This write only bit causes the queue occupancy counter of the PerfMon counter 
       for which this Perf event select register is associated to be cleared to all 
       zeroes when a '1' is written to it. No action is taken when a '0' is written. 
     */
    UINT32 counter_reset : 1;
    /* counter_reset - Bits[17:17], WO, default = 1'b0 
       When this bit is set, the corresponding counter will be reset to 0. This allows 
       for a quick reset of the counter when changing event encodings. 
     */
    UINT32 edge_detect : 1;
    /* edge_detect - Bits[18:18], RW_V, default = 1'b0 
       Edge Detect allows one to count either 0 to 1 or 1 to 0 transitions of a given 
       event. For example, we have an event that counts the number of cycles in L1 mode 
       in Intel QPI. By using edge detect, one can count the number of times that we 
       entered L1 mode (by detecting the rising edge). 
       
       Edge detect only works in conjunction with threshholding. This is true even for 
       events that can only increment by 1 in a given cycle (like the L1 example 
       above). In this case, one should set a threshhold of 1. One can also use Edge 
       Detect with queue occupancy events. For example, if one wanted to count the 
       number of times when the TOR occupancy was larger than 5, one would selet the 
       TOR occupancy event with a threshold of 5 and set the Edge Detect bit. 
       
       Edge detect can also be used with the invert. This is generally not particularly 
       useful, as the count of falling edges compared to rising edges will always on 
       differ by 1. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 overflow_enable : 1;
    /* overflow_enable - Bits[20:20], RW_V, default = 1'b0 
       Setting this bit will enable the counter to send an overflow signal. If this bit 
       is not set, the counter will wrap around when it overflows without triggering 
       anything. If this bit is set and the Unit's configuration register has Overflow 
       enabled, then a signal will be transmitted to the Ubox. 
     */
    UINT32 rsvd_21 : 1;
    UINT32 counter_enable : 1;
    /* counter_enable - Bits[22:22], RW_V, default = 1'b0 
       This field is the local enable for the PerfMon Counter. This bit must be 
       asserted in order for the PerfMon counter to begin counting the events selected 
       by the 'event select', 'unit mask', and 'internal' bits (see the fields below). 
       There is one bit per PerfMon Counter. Note that if this bit is set to 1 but the 
       Unit Control Registers have determined that counting is disabled, then the 
       counter will not count. 
     */
    UINT32 invert : 1;
    /* invert - Bits[23:23], RW_V, default = 1'b0 
       This bit indicates how the threshold field will be compared to the incoming 
       event. When 0, the comparison that will be done is threshold >= event. When set 
       to 1, the comparison that will be done is inverted from the case where this bit 
       is set to 0, i.e., threshold < event. The invert bit only works when Threshhold 
       != 0. So, if one would like to invert a non-occupancy event (like LLC Hit), one 
       needs to set the threshhold to 1. 
     */
    UINT32 threshold : 8;
    /* threshold - Bits[31:24], RW_V, default = 8'b00000000 
       This field is compared directly against an incoming event value for events that 
       can increment by 1 or more in a given cycle. Since the widest event from the 
       UnCore is 7bits (queue occupancy), bit 31 is unused. The result of the 
       comparison is effectively a 1 bit wide event, i.e., the counter will be 
       incremented by 1 when the comparison is true (the type of comparison depends on 
       the setting of the 'invert' bit - see bit 23 below) no matter how wide the 
       original event was. When this field is zero, threshold comparison is disabled 
       and the event is passed without modification. 
     */
  } Bits;
  UINT32 Data;
} CNTRCFG_4_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* UNITCTRL_HA_PMON_REG supported on:                                         */
/*       IVT_EP (0x401710F4)                                                  */
/*       IVT_EX (0x401710F4)                                                  */
/*       HSX (0x401910F4)                                                     */
/*       BDX (0x401910F4)                                                     */
/* Register default value:              0x00030000                            */
#define UNITCTRL_HA_PMON_REG 0x030140F4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * This register controls the unit-level performance monitor behavior.
 */
typedef union {
  struct {
    UINT32 reset_counter_configs : 1;
    /* reset_counter_configs - Bits[0:0], WO, default = 1'b0 
       When this bit is written to, the counter configuration registers will be reset. 
       This does not effect the values in the counters. To reset the counters, this bit 
       need only be set by one of the unit control registers. 
     */
    UINT32 reset_counters : 1;
    /* reset_counters - Bits[1:1], WO, default = 1'b0 
       When this bit is written to, the counters data fields will be reset. The 
       configuration values will not be reset. To reset the counters, this bit need 
       only be set by one of the unit control registers. 
     */
    UINT32 rsvd_2 : 6;
    /* rsvd_2 - Bits[7:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 freeze_counters : 1;
    /* freeze_counters - Bits[8:8], RW_V, default = 1'b0 
       This bit is written to when the counters should be frozen. If this bit is 
       written to and freeze is enabled, the counters in the unit will stop counting. 
       To freeze the counters, this bit need only be set by one of the unit control 
       registers. 
     */
    UINT32 rsvd_9 : 7;
    /* rsvd_9 - Bits[15:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 freeze_enable : 1;
    /* freeze_enable - Bits[16:16], RW_L, default = 1'b1 
       This bit controls what the counters in the unit will do when they receive a 
       freeze signal. When set, the counters will be allowed to freeze. When not set, 
       the counters will ignore the freeze signal. For freeze to be enabled for a given 
       unit, all of the unit control registers must have this bit set. 
     */
    UINT32 overflow_enable : 1;
    /* overflow_enable - Bits[17:17], RW_L, default = 1'b1 
       This bit controls the behavior of counters when they overflow. When set, the 
       system will trigger the overflow handling process throughout the rest of the 
       uncore, potentially triggering a PMI and freezing counters. When it is not set, 
       the counters will simply wrap around and continue to count. For overflow to be 
       enabled for a given unit, all of the unit control registers must have this bit 
       set. 
     */
    UINT32 rsvd_18 : 14;
    /* rsvd_18 - Bits[31:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} UNITCTRL_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* UNITSTATUS_HA_PMON_REG supported on:                                       */
/*       IVT_EP (0x401710F8)                                                  */
/*       IVT_EX (0x401710F8)                                                  */
/*       HSX (0x401910F8)                                                     */
/*       BDX (0x401910F8)                                                     */
/* Register default value:              0x00000000                            */
#define UNITSTATUS_HA_PMON_REG 0x030140F8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.1.CFG.xml.
 * This register reports the unit-level performance monitor status.
 */
typedef union {
  struct {
    UINT32 counter_overflow_bitmask : 5;
    /* counter_overflow_bitmask - Bits[4:0], RW1C, default = 5'b00000 
       This is a bitmask that specifies which counter (or counters) have overflowed. If 
       the unit has a fixed counter, it's corresponding bitmask will be stored at 
       position 0. 
     */
    UINT32 rsvd : 27;
    /* rsvd - Bits[31:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} UNITSTATUS_HA_PMON_STRUCT;
#endif /* ASM_INC */


/* CLS_HA_PMON_REG supported on:                                              */
/*       IVT_EP (0x1017100C)                                                  */
/*       IVT_EX (0x1017100C)                                                  */
/* Register default value:              0x00                                  */
#define CLS_HA_PMON_REG 0x0301100C



/* MLT_HA_PMON_REG supported on:                                              */
/*       IVT_EP (0x1017100D)                                                  */
/*       IVT_EX (0x1017100D)                                                  */
/* Register default value:              0x00                                  */
#define MLT_HA_PMON_REG 0x0301100D



/* TMBAR_N0_HA_PMON_REG supported on:                                         */
/*       IVT_EP (0x40171010)                                                  */
/*       IVT_EX (0x40171010)                                                  */
/* Register default value:              0x00000000                            */
#define TMBAR_N0_HA_PMON_REG 0x03014010



/* TMBAR_N1_HA_PMON_REG supported on:                                         */
/*       IVT_EP (0x40171014)                                                  */
/*       IVT_EX (0x40171014)                                                  */
/* Register default value:              0x00000000                            */
#define TMBAR_N1_HA_PMON_REG 0x03014014



/* SVID_HA_PMON_REG supported on:                                             */
/*       IVT_EP (0x2017102C)                                                  */
/*       IVT_EX (0x2017102C)                                                  */
/* Register default value:              0x0000                                */
#define SVID_HA_PMON_REG 0x0301202C



/* SID_HA_PMON_REG supported on:                                              */
/*       IVT_EP (0x2017102E)                                                  */
/*       IVT_EX (0x2017102E)                                                  */
/* Register default value:              0x0000                                */
#define SID_HA_PMON_REG 0x0301202E



/* CAPPOINT_HA_PMON_REG supported on:                                         */
/*       IVT_EP (0x10171034)                                                  */
/*       IVT_EX (0x10171034)                                                  */
/* Register default value:              0x00                                  */
#define CAPPOINT_HA_PMON_REG 0x03011034



/* INTRLINE_HA_PMON_REG supported on:                                         */
/*       IVT_EP (0x1017103C)                                                  */
/*       IVT_EX (0x1017103C)                                                  */
/* Register default value:              0x00                                  */
#define INTRLINE_HA_PMON_REG 0x0301103C



/* INTRPIN_HA_PMON_REG supported on:                                          */
/*       IVT_EP (0x1017103D)                                                  */
/*       IVT_EX (0x1017103D)                                                  */
/* Register default value:              0x00                                  */
#define INTRPIN_HA_PMON_REG 0x0301103D



#endif /* HA_PMON_h */
