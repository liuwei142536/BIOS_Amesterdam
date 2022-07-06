/* Date Stamp: 8/23/2014 */

#ifndef VCU_FUN_0_h
#define VCU_FUN_0_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For HSX_HOST:                                                              */
/* VCU_FUN_0_HSX_DEV 31                                                       */
/* VCU_FUN_0_HSX_FUN 0                                                        */
/* For BDX_HOST:                                                              */
/* VCU_FUN_0_BDX_DEV 31                                                       */
/* VCU_FUN_0_BDX_FUN 0                                                        */

/* VID_VCU_FUN_0_REG supported on:                                            */
/*       HSX (0x201F8000)                                                     */
/*       BDX (0x201F8000)                                                     */
/* Register default value:              0x8086                                */
#define VID_VCU_FUN_0_REG 0x13002000

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
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
} VID_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DID_VCU_FUN_0_REG supported on:                                            */
/*       HSX (0x201F8002)                                                     */
/*       BDX (0x201F8002)                                                     */
/* Register default value on HSX:       0x2F88                                */
/* Register default value on BDX:       0x6F88                                */
#define DID_VCU_FUN_0_REG 0x13002002

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6F88 
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
} DID_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PCICMD_VCU_FUN_0_REG supported on:                                         */
/*       HSX (0x201F8004)                                                     */
/*       BDX (0x201F8004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_VCU_FUN_0_REG 0x13002004

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
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
} PCICMD_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PCISTS_VCU_FUN_0_REG supported on:                                         */
/*       HSX (0x201F8006)                                                     */
/*       BDX (0x201F8006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_VCU_FUN_0_REG 0x13002006

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
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
} PCISTS_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RID_VCU_FUN_0_REG supported on:                                            */
/*       HSX (0x101F8008)                                                     */
/*       BDX (0x101F8008)                                                     */
/* Register default value:              0x00                                  */
#define RID_VCU_FUN_0_REG 0x13001008

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
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
} RID_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CCR_N0_VCU_FUN_0_REG supported on:                                         */
/*       HSX (0x101F8009)                                                     */
/*       BDX (0x101F8009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_VCU_FUN_0_REG 0x13001009

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CCR_N1_VCU_FUN_0_REG supported on:                                         */
/*       HSX (0x201F800A)                                                     */
/*       BDX (0x201F800A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_VCU_FUN_0_REG 0x1300200A

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
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
} CCR_N1_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CLSR_VCU_FUN_0_REG supported on:                                           */
/*       HSX (0x101F800C)                                                     */
/*       BDX (0x101F800C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_VCU_FUN_0_REG 0x1300100C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
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
} CLSR_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PLAT_VCU_FUN_0_REG supported on:                                           */
/*       HSX (0x101F800D)                                                     */
/*       BDX (0x101F800D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_VCU_FUN_0_REG 0x1300100D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
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
} PLAT_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* HDR_VCU_FUN_0_REG supported on:                                            */
/*       HSX (0x101F800E)                                                     */
/*       BDX (0x101F800E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_VCU_FUN_0_REG 0x1300100E

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
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
} HDR_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* BIST_VCU_FUN_0_REG supported on:                                           */
/*       HSX (0x101F800F)                                                     */
/*       BDX (0x101F800F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_VCU_FUN_0_REG 0x1300100F

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
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
} BIST_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CAPPTR_VCU_FUN_0_REG supported on:                                         */
/*       HSX (0x101F8034)                                                     */
/*       BDX (0x101F8034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_VCU_FUN_0_REG 0x13001034

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
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
} CAPPTR_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTL_VCU_FUN_0_REG supported on:                                           */
/*       HSX (0x101F803C)                                                     */
/*       BDX (0x101F803C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_VCU_FUN_0_REG 0x1300103C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
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
} INTL_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTPIN_VCU_FUN_0_REG supported on:                                         */
/*       HSX (0x101F803D)                                                     */
/*       BDX (0x101F803D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_VCU_FUN_0_REG 0x1300103D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
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
} INTPIN_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MINGNT_VCU_FUN_0_REG supported on:                                         */
/*       HSX (0x101F803E)                                                     */
/*       BDX (0x101F803E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_VCU_FUN_0_REG 0x1300103E

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
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
} MINGNT_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MAXLAT_VCU_FUN_0_REG supported on:                                         */
/*       HSX (0x101F803F)                                                     */
/*       BDX (0x101F803F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_VCU_FUN_0_REG 0x1300103F

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
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
} MAXLAT_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CSR_MAILBOX_INTERFACE_VCU_FUN_0_REG supported on:                          */
/*       HSX (0x401F8040)                                                     */
/*       BDX (0x401F8040)                                                     */
/* Register default value:              0x00000000                            */
#define CSR_MAILBOX_INTERFACE_VCU_FUN_0_REG 0x13004040

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
 * Command register of CSR API mailbox to the VCU.
 */
typedef union {
  struct {
    UINT32 opcode : 16;
    /* opcode - Bits[15:0], RW_V, default = 16'h0000 
       Command issued to VCU.
     */
    UINT32 index : 8;
    /* index - Bits[23:16], RW_V, default = 8'h00 
       Index field of request to VCU.
     */
    UINT32 rsvd : 7;
    /* rsvd - Bits[30:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 run_busy : 1;
    /* run_busy - Bits[31:31], RW1S, default = 1'b0 
       Runbusy bit is set when request to VCU is ready.
     */
  } Bits;
  UINT32 Data;
} CSR_MAILBOX_INTERFACE_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CSR_MAILBOX_DATA_VCU_FUN_0_REG supported on:                               */
/*       HSX (0x401F8044)                                                     */
/*       BDX (0x401F8044)                                                     */
/* Register default value:              0x00000000                            */
#define CSR_MAILBOX_DATA_VCU_FUN_0_REG 0x13004044

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
 * Data register of CSR API mailbox to the VCU.
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RW_V, default = 32'h00000000 
       Data issued to VCU.
     */
  } Bits;
  UINT32 Data;
} CSR_MAILBOX_DATA_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* EXT_IOT_ENABLE_VCU_FUN_0_REG supported on:                                 */
/*       HSX (0x401F8048)                                                     */
/*       BDX (0x401F8048)                                                     */
/* Register default value:              0x00000000                            */
#define EXT_IOT_ENABLE_VCU_FUN_0_REG 0x13004048

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
 * External IOT enable
 */
typedef union {
  struct {
    UINT32 enable : 1;
    /* enable - Bits[0:0], RW_V, default = 1'b0  */
    UINT32 rsvd : 31;
    /* rsvd - Bits[31:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} EXT_IOT_ENABLE_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* FEATURES_VCU_FUN_0_REG supported on:                                       */
/*       HSX (0x401F804C)                                                     */
/*       BDX (0x401F804C)                                                     */
/* Register default value:              0x00000000                            */
#define FEATURES_VCU_FUN_0_REG 0x1300404C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
 * Feature configuration
 */
typedef union {
  struct {
    UINT32 allow_peci_vcode_error_rsp : 1;
    /* allow_peci_vcode_error_rsp - Bits[0:0], RW_V, default = 1'b0 
       allow vcode to attempt to respond to peci requests after
       a vcode MCA, should be set the same as 
       PCU_CR_DYNAMIC_PERF_POWER_CTL_CFG.allow_peci_pcode_error_rsp 
               
     */
    UINT32 rsvd_1 : 15;
    /* rsvd_1 - Bits[15:1], RW_V, default = 15'h0 
       Reserved feature config bits
     */
    UINT32 rsvd_16 : 16;
    /* rsvd_16 - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} FEATURES_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* VISA_CTL_VCUIOREGS_VCU_FUN_0_REG supported on:                             */
/*       HSX (0x401F8050)                                                     */
/*       BDX (0x401F8050)                                                     */
/* Register default value:              0x00000000                            */
#define VISA_CTL_VCUIOREGS_VCU_FUN_0_REG 0x13004050

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.31.0.CFG.xml.
 * Register to control VISA mux vcuioregs
 */
typedef union {
  struct {
    UINT32 data : 18;
    /* data - Bits[17:0], RWS, default = 18'h0 
       VISA control
     */
    UINT32 rsvd : 13;
    /* rsvd - Bits[30:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 vorange : 1;
    /* vorange - Bits[31:31], ROS_VFW, default = 1'b0 
       Reserved.
     */
  } Bits;
  UINT32 Data;
} VISA_CTL_VCUIOREGS_VCU_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


#endif /* VCU_FUN_0_h */
