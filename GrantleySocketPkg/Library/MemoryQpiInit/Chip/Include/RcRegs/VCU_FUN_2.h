/* Date Stamp: 8/23/2014 */

#ifndef VCU_FUN_2_h
#define VCU_FUN_2_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For HSX_HOST:                                                              */
/* VCU_FUN_2_HSX_DEV 31                                                       */
/* VCU_FUN_2_HSX_FUN 2                                                        */
/* For BDX_HOST:                                                              */
/* VCU_FUN_2_BDX_DEV 31                                                       */
/* VCU_FUN_2_BDX_FUN 2                                                        */

/* VID_VCU_FUN_2_REG supported on:                                            */
/*       HSX (0x201FA000)                                                     */
/*       BDX (0x201FA000)                                                     */
/* Register default value:              0x8086                                */
#define VID_VCU_FUN_2_REG 0x13022000

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
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
} VID_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DID_VCU_FUN_2_REG supported on:                                            */
/*       HSX (0x201FA002)                                                     */
/*       BDX (0x201FA002)                                                     */
/* Register default value on HSX:       0x2F8A                                */
/* Register default value on BDX:       0x6F8A                                */
#define DID_VCU_FUN_2_REG 0x13022002

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6F8A 
       Device ID values vary from function to function. Bits 15:8 are equal to 0x6F for 
       the processor. The following list is a breakdown of the function groups.0x6F00 - 
       0x6F1F : PCI Express and DMI2 ports 
         0x6F20 - 0x6F3F : IO Features (CBDMA, APIC, VT, RAS, Intel TXT)
         0x6F40 - 0x6F5F : Performance Monitors
         0x6F80 - 0x6F9F : Intel Quick Path Interface
         0x6FA0 - 0x6FBF : Home Agent/Memory Controller
         0x6FC0 - 0x6FDF : Power Management
         0x6FE0 - 0x6FFF : Cbo/Ring
     */
  } Bits;
  UINT16 Data;
} DID_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PCICMD_VCU_FUN_2_REG supported on:                                         */
/*       HSX (0x201FA004)                                                     */
/*       BDX (0x201FA004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_VCU_FUN_2_REG 0x13022004

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
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
} PCICMD_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PCISTS_VCU_FUN_2_REG supported on:                                         */
/*       HSX (0x201FA006)                                                     */
/*       BDX (0x201FA006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_VCU_FUN_2_REG 0x13022006

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
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
} PCISTS_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RID_VCU_FUN_2_REG supported on:                                            */
/*       HSX (0x101FA008)                                                     */
/*       BDX (0x101FA008)                                                     */
/* Register default value:              0x00                                  */
#define RID_VCU_FUN_2_REG 0x13021008

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
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
} RID_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CCR_N0_VCU_FUN_2_REG supported on:                                         */
/*       HSX (0x101FA009)                                                     */
/*       BDX (0x101FA009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_VCU_FUN_2_REG 0x13021009

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CCR_N1_VCU_FUN_2_REG supported on:                                         */
/*       HSX (0x201FA00A)                                                     */
/*       BDX (0x201FA00A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_VCU_FUN_2_REG 0x1302200A

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT16 sub_class : 8;
    /* sub_class - Bits[7:0], RO_V, default = 8'b10000000 
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
    /* base_class - Bits[15:8], RO_V, default = 8'b00001000 
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
} CCR_N1_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CLSR_VCU_FUN_2_REG supported on:                                           */
/*       HSX (0x101FA00C)                                                     */
/*       BDX (0x101FA00C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_VCU_FUN_2_REG 0x1302100C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
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
} CLSR_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PLAT_VCU_FUN_2_REG supported on:                                           */
/*       HSX (0x101FA00D)                                                     */
/*       BDX (0x101FA00D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_VCU_FUN_2_REG 0x1302100D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
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
} PLAT_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* HDR_VCU_FUN_2_REG supported on:                                            */
/*       HSX (0x101FA00E)                                                     */
/*       BDX (0x101FA00E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_VCU_FUN_2_REG 0x1302100E

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
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
} HDR_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* BIST_VCU_FUN_2_REG supported on:                                           */
/*       HSX (0x101FA00F)                                                     */
/*       BDX (0x101FA00F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_VCU_FUN_2_REG 0x1302100F

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
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
} BIST_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CAPPTR_VCU_FUN_2_REG supported on:                                         */
/*       HSX (0x101FA034)                                                     */
/*       BDX (0x101FA034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_VCU_FUN_2_REG 0x13021034

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
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
} CAPPTR_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTL_VCU_FUN_2_REG supported on:                                           */
/*       HSX (0x101FA03C)                                                     */
/*       BDX (0x101FA03C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_VCU_FUN_2_REG 0x1302103C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
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
} INTL_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTPIN_VCU_FUN_2_REG supported on:                                         */
/*       HSX (0x101FA03D)                                                     */
/*       BDX (0x101FA03D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_VCU_FUN_2_REG 0x1302103D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
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
} INTPIN_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MINGNT_VCU_FUN_2_REG supported on:                                         */
/*       HSX (0x101FA03E)                                                     */
/*       BDX (0x101FA03E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_VCU_FUN_2_REG 0x1302103E

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
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
} MINGNT_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MAXLAT_VCU_FUN_2_REG supported on:                                         */
/*       HSX (0x101FA03F)                                                     */
/*       BDX (0x101FA03F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_VCU_FUN_2_REG 0x1302103F

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
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
} MAXLAT_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DEVHIDE0_VCU_FUN_2_REG supported on:                                       */
/*       HSX (0x401FA0B0)                                                     */
/*       BDX (0x401FA0B0)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE0_VCU_FUN_2_REG 0x130240B0

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 disfn : 32;
    /* disfn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDEuCodeCtrl4, then it means that 
       in device 5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE0_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DEVHIDE1_VCU_FUN_2_REG supported on:                                       */
/*       HSX (0x401FA0B4)                                                     */
/*       BDX (0x401FA0B4)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE1_VCU_FUN_2_REG 0x130240B4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 disfn : 32;
    /* disfn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDEuCodeCtrl4, then it means that 
       in device 5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE1_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DEVHIDE2_VCU_FUN_2_REG supported on:                                       */
/*       HSX (0x401FA0B8)                                                     */
/*       BDX (0x401FA0B8)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE2_VCU_FUN_2_REG 0x130240B8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 disfn : 32;
    /* disfn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDEuCodeCtrl4, then it means that 
       in device 5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE2_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DEVHIDE3_VCU_FUN_2_REG supported on:                                       */
/*       HSX (0x401FA0BC)                                                     */
/*       BDX (0x401FA0BC)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE3_VCU_FUN_2_REG 0x130240BC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 disfn : 32;
    /* disfn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDEuCodeCtrl4, then it means that 
       in device 5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE3_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DEVHIDE4_VCU_FUN_2_REG supported on:                                       */
/*       HSX (0x401FA0C0)                                                     */
/*       BDX (0x401FA0C0)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE4_VCU_FUN_2_REG 0x130240C0

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 disfn : 32;
    /* disfn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDEuCodeCtrl4, then it means that 
       in device 5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE4_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DEVHIDE5_VCU_FUN_2_REG supported on:                                       */
/*       HSX (0x401FA0C4)                                                     */
/*       BDX (0x401FA0C4)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE5_VCU_FUN_2_REG 0x130240C4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 disfn : 32;
    /* disfn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDEuCodeCtrl4, then it means that 
       in device 5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE5_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DEVHIDE6_VCU_FUN_2_REG supported on:                                       */
/*       HSX (0x401FA0C8)                                                     */
/*       BDX (0x401FA0C8)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE6_VCU_FUN_2_REG 0x130240C8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 disfn : 32;
    /* disfn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDEuCodeCtrl4, then it means that 
       in device 5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE6_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DEVHIDE7_VCU_FUN_2_REG supported on:                                       */
/*       HSX (0x401FA0CC)                                                     */
/*       BDX (0x401FA0CC)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE7_VCU_FUN_2_REG 0x130240CC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 disfn : 32;
    /* disfn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDEuCodeCtrl4, then it means that 
       in device 5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE7_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* GLOBAL_NID_SOCKET_0_TO_3_MAP_VCU_FUN_2_REG supported on:                   */
/*       HSX (0x401FA0D0)                                                     */
/*       BDX (0x401FA0D0)                                                     */
/* Register default value:              0x00000000                            */
#define GLOBAL_NID_SOCKET_0_TO_3_MAP_VCU_FUN_2_REG 0x130240D0

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
 * This register is in the VCU CR space.  It contains NID information for sockets 
 * 0-3 in the platform.  Expectation is that BIOS will write this register during 
 * the Reset/Init flow. 
 */
typedef union {
  struct {
    UINT32 skt0_nid : 3;
    /* skt0_nid - Bits[2:0], RW, default = 3'b000 
       Socket0 NID
     */
    UINT32 rsvd_3 : 1;
    UINT32 skt1_nid : 3;
    /* skt1_nid - Bits[6:4], RW, default = 3'b000 
       Socket1 NID
     */
    UINT32 rsvd_7 : 1;
    UINT32 skt2_nid : 3;
    /* skt2_nid - Bits[10:8], RW, default = 3'b000 
       Socket2 NID
     */
    UINT32 rsvd_11 : 1;
    UINT32 skt3_nid : 3;
    /* skt3_nid - Bits[14:12], RW, default = 3'b000 
       Socket3 NID
     */
    UINT32 rsvd_15 : 13;
    UINT32 skt_valid : 4;
    /* skt_valid - Bits[31:28], RW, default = 4'b0000 
       Valid bits indicating whether NID has been programmed by BIOS.  If bit is 0 
       after the CST/SST ready bit is set, then it implies that the socket is not 
       populated. 
     */
  } Bits;
  UINT32 Data;
} GLOBAL_NID_SOCKET_0_TO_3_MAP_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* GLOBAL_NID_SOCKET_4_TO_7_MAP_VCU_FUN_2_REG supported on:                   */
/*       HSX (0x401FA0D4)                                                     */
/*       BDX (0x401FA0D4)                                                     */
/* Register default value:              0x00000000                            */
#define GLOBAL_NID_SOCKET_4_TO_7_MAP_VCU_FUN_2_REG 0x130240D4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.2.CFG.xml.
 * This reister is in the VCU CR space.  It contains NID information for sockets 
 * 4-7 in the platform.  Expectation is that BIOS will write this register during 
 * the Reset/Init flow. 
 */
typedef union {
  struct {
    UINT32 skt4_nid : 3;
    /* skt4_nid - Bits[2:0], RW, default = 3'b000 
       Socket4 NID
     */
    UINT32 rsvd_3 : 1;
    UINT32 skt5_nid : 3;
    /* skt5_nid - Bits[6:4], RW, default = 3'b000 
       Socket5 NID
     */
    UINT32 rsvd_7 : 1;
    UINT32 skt6_nid : 3;
    /* skt6_nid - Bits[10:8], RW, default = 3'b000 
       Socket6 NID
     */
    UINT32 rsvd_11 : 1;
    UINT32 skt7_nid : 3;
    /* skt7_nid - Bits[14:12], RW, default = 3'b000 
       Socket7 NID
     */
    UINT32 rsvd_15 : 13;
    UINT32 skt_valid : 4;
    /* skt_valid - Bits[31:28], RW, default = 4'b0000 
       Valid bits indicating whether NID has been programmed by BIOS.  If bit is 0 
       after the CST/SST ready bit is set, then it implies that the socket is not 
       populated. 
     */
  } Bits;
  UINT32 Data;
} GLOBAL_NID_SOCKET_4_TO_7_MAP_VCU_FUN_2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


#endif /* VCU_FUN_2_h */
