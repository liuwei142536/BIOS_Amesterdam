/* Date Stamp: 8/23/2014 */

#ifndef CBOBC_COH_h
#define CBOBC_COH_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* CBOBC_COH_IVT_DEV 22                                                       */
/* CBOBC_COH_IVT_FUN 2                                                        */
/* For HSX_HOST:                                                              */
/* CBOBC_COH_HSX_DEV 15                                                       */
/* CBOBC_COH_HSX_FUN 6                                                        */
/* For BDX_HOST:                                                              */
/* CBOBC_COH_BDX_DEV 15                                                       */
/* CBOBC_COH_BDX_FUN 6                                                        */

/* VID_CBOBC_COH_REG supported on:                                            */
/*       IVT_EP (0x201B2000)                                                  */
/*       IVT_EX (0x201B2000)                                                  */
/*       HSX (0x2017E000)                                                     */
/*       BDX (0x2017E000)                                                     */
/* Register default value:              0x8086                                */
#define VID_CBOBC_COH_REG 0x01022000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
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
} VID_CBOBC_COH_STRUCT;
#endif /* ASM_INC */


/* DID_CBOBC_COH_REG supported on:                                            */
/*       IVT_EP (0x201B2002)                                                  */
/*       IVT_EX (0x201B2002)                                                  */
/*       HSX (0x2017E002)                                                     */
/*       BDX (0x2017E002)                                                     */
/* Register default value on IVT_EP:    0x0ECA                                */
/* Register default value on IVT_EX:    0x0ECA                                */
/* Register default value on HSX:       0x2FFE                                */
/* Register default value on BDX:       0x6FFE                                */
#define DID_CBOBC_COH_REG 0x01022002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6FFE 
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
} DID_CBOBC_COH_STRUCT;
#endif /* ASM_INC */


/* PCICMD_CBOBC_COH_REG supported on:                                         */
/*       HSX (0x2017E004)                                                     */
/*       BDX (0x2017E004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_CBOBC_COH_REG 0x01022004

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
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
} PCICMD_CBOBC_COH_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PCISTS_CBOBC_COH_REG supported on:                                         */
/*       HSX (0x2017E006)                                                     */
/*       BDX (0x2017E006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_CBOBC_COH_REG 0x01022006

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
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
} PCISTS_CBOBC_COH_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RID_CBOBC_COH_REG supported on:                                            */
/*       IVT_EP (0x101B2008)                                                  */
/*       IVT_EX (0x101B2008)                                                  */
/*       HSX (0x1017E008)                                                     */
/*       BDX (0x1017E008)                                                     */
/* Register default value:              0x00                                  */
#define RID_CBOBC_COH_REG 0x01021008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
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
} RID_CBOBC_COH_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_CBOBC_COH_REG supported on:                                         */
/*       IVT_EP (0x101B2009)                                                  */
/*       IVT_EX (0x101B2009)                                                  */
/*       HSX (0x1017E009)                                                     */
/*       BDX (0x1017E009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_CBOBC_COH_REG 0x01021009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.15.6.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_CBOBC_COH_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_CBOBC_COH_REG supported on:                                         */
/*       IVT_EP (0x201B200A)                                                  */
/*       IVT_EX (0x201B200A)                                                  */
/*       HSX (0x2017E00A)                                                     */
/*       BDX (0x2017E00A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_CBOBC_COH_REG 0x0102200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
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
} CCR_N1_CBOBC_COH_STRUCT;
#endif /* ASM_INC */


/* CLSR_CBOBC_COH_REG supported on:                                           */
/*       IVT_EP (0x101B200C)                                                  */
/*       IVT_EX (0x101B200C)                                                  */
/*       HSX (0x1017E00C)                                                     */
/*       BDX (0x1017E00C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_CBOBC_COH_REG 0x0102100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
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
} CLSR_CBOBC_COH_STRUCT;
#endif /* ASM_INC */


/* PLAT_CBOBC_COH_REG supported on:                                           */
/*       HSX (0x1017E00D)                                                     */
/*       BDX (0x1017E00D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_CBOBC_COH_REG 0x0102100D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
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
} PLAT_CBOBC_COH_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* HDR_CBOBC_COH_REG supported on:                                            */
/*       IVT_EP (0x101B200E)                                                  */
/*       IVT_EX (0x101B200E)                                                  */
/*       HSX (0x1017E00E)                                                     */
/*       BDX (0x1017E00E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_CBOBC_COH_REG 0x0102100E


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.15.6.CFG.xml.
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
} HDR_CBOBC_COH_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* BIST_CBOBC_COH_REG supported on:                                           */
/*       HSX (0x1017E00F)                                                     */
/*       BDX (0x1017E00F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_CBOBC_COH_REG 0x0102100F

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
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
} BIST_CBOBC_COH_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SVID_CBOBC_COH_REG supported on:                                           */
/*       IVT_EP (0x201B202C)                                                  */
/*       IVT_EX (0x201B202C)                                                  */
/*       HSX (0x2017E02C)                                                     */
/*       BDX (0x2017E02C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_CBOBC_COH_REG 0x0102202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
 * PCI Subsystem Vendor ID Register
 */
typedef union {
  struct {
    UINT16 subsystem_vendor_identification_number : 16;
    /* subsystem_vendor_identification_number - Bits[15:0], RW_O, default = 16'b1000000010000110 
       The default value specifies Intel but can be set to any value once after reset.
     */
  } Bits;
  UINT16 Data;
} SVID_CBOBC_COH_STRUCT;
#endif /* ASM_INC */


/* SDID_CBOBC_COH_REG supported on:                                           */
/*       IVT_EP (0x201B202E)                                                  */
/*       IVT_EX (0x201B202E)                                                  */
/*       HSX (0x2017E02E)                                                     */
/*       BDX (0x2017E02E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_CBOBC_COH_REG 0x0102202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
 * PCI Subsystem device ID Register
 */
typedef union {
  struct {
    UINT16 subsystem_device_identification_number : 16;
    /* subsystem_device_identification_number - Bits[15:0], RW_O, default = 16'b0000000000000000 
       Assigned by the subsystem vendor to uniquely identify the subsystem
     */
  } Bits;
  UINT16 Data;
} SDID_CBOBC_COH_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_CBOBC_COH_REG supported on:                                         */
/*       HSX (0x1017E034)                                                     */
/*       BDX (0x1017E034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_CBOBC_COH_REG 0x01021034

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
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
} CAPPTR_CBOBC_COH_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTL_CBOBC_COH_REG supported on:                                           */
/*       HSX (0x1017E03C)                                                     */
/*       BDX (0x1017E03C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_CBOBC_COH_REG 0x0102103C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
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
} INTL_CBOBC_COH_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTPIN_CBOBC_COH_REG supported on:                                         */
/*       HSX (0x1017E03D)                                                     */
/*       BDX (0x1017E03D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_CBOBC_COH_REG 0x0102103D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
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
} INTPIN_CBOBC_COH_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MINGNT_CBOBC_COH_REG supported on:                                         */
/*       HSX (0x1017E03E)                                                     */
/*       BDX (0x1017E03E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_CBOBC_COH_REG 0x0102103E

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
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
} MINGNT_CBOBC_COH_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MAXLAT_CBOBC_COH_REG supported on:                                         */
/*       HSX (0x1017E03F)                                                     */
/*       BDX (0x1017E03F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_CBOBC_COH_REG 0x0102103F

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
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
} MAXLAT_CBOBC_COH_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CBO_ISOC_CONFIG_CBOBC_COH_REG supported on:                                */
/*       IVT_EP (0x401B2044)                                                  */
/*       IVT_EX (0x401B2044)                                                  */
/*       HSX (0x4017E044)                                                     */
/*       BDX (0x4017E044)                                                     */
/* Register default value:              0x00000000                            */
#define CBO_ISOC_CONFIG_CBOBC_COH_REG 0x01024044
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
 */
typedef union {
  struct {
    UINT32 isoc_enable : 1;
    /* isoc_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable ISOC mode. This will be used for TOR pipeline to reserve TOR entries for 
       ISOC. 
     */
    UINT32 enablemesegonly : 1;
    /* enablemesegonly - Bits[1:1], RW_LB, default = 1'b0 
       Enable the MESEG configuration with ISOC disabled.
     */
    UINT32 rsvd : 30;
    /* rsvd - Bits[31:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CBO_ISOC_CONFIG_CBOBC_COH_STRUCT;
#endif /* ASM_INC */


/* CBO_COH_CONFIG_CBOBC_COH_REG supported on:                                 */
/*       IVT_EP (0x401B2050)                                                  */
/*       IVT_EX (0x401B2050)                                                  */
/*       HSX (0x4017E050)                                                     */
/*       BDX (0x4017E050)                                                     */
/* Register default value on IVT_EP:    0x400FA000                            */
/* Register default value on IVT_EX:    0x400FA000                            */
/* Register default value on HSX:       0x400BA000                            */
/* Register default value on BDX:       0x400BA000                            */
#define CBO_COH_CONFIG_CBOBC_COH_REG 0x01024050


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.15.6.CFG.xml.
 */
typedef union {
  struct {
    UINT32 ego : 1;
    /* ego - Bits[0:0], RW_LB, default = 1'b0 
       Enable Cbo Early GO mode
     */
    UINT32 sleeptagsel : 2;
    /* sleeptagsel - Bits[2:1], RW_LB, default = 2'b00 
       Determines which NID bit is used in created the Sleep Tag.
     */
    UINT32 rsvd_3 : 4;
    /* rsvd_3 - Bits[6:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 dpsrcsnoop : 1;
    /* dpsrcsnoop - Bits[7:7], RW_LB, default = 1'b0 
       Enable DP Early Snoop optimization. (EP processor only)  
     */
    UINT32 drdgosonem : 1;
    /* drdgosonem - Bits[8:8], RW_LB, default = 1'b0 
       Enable GOS on E/M state for DRD
     */
    UINT32 rsvd_9 : 1;
    /* rsvd_9 - Bits[9:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 mtoibias : 1;
    /* mtoibias - Bits[10:10], RW_LB, default = 1'b0 
       Use MtoI policy as opposed to MtoS policy
     */
    UINT32 downgradeftos : 1;
    /* downgradeftos - Bits[11:11], RW_LB, default = 1'b0 
       Downgrade all F state to S state
     */
    UINT32 biasfwd : 1;
    /* biasfwd - Bits[12:12], RW_LB, default = 1'b0 
       Enable RspFwdIWB mode, BiasFwdDoubleData &amp; BiasFwdLocalHome are used for 
       further qualifications. Table below shows the qualifications and 
       behaviors:BiasFwd   BiasFwdDoubleData   BiasFwdLocalHome   Behaviour for 
       SnpData/Code with LLC=M    
          0   x   x   Original behaviour. No C2C. Implicit writeback to HA   
          1   0   0   Fwd only when Home!=Requestor AND Home!=Local   
          1   0   1   Fwd only when Home!=Requestor   
          1   1   0   Fwd only when Home!=Local   
          1   1   1   Fwd for all cases
     */
    UINT32 waitfordatacmp : 1;
    /* waitfordatacmp - Bits[13:13], RW_LB, default = 1'b1 
       Wait for Data+Cmp before sending through Cpipe. if 0h, will do it separately.
     */
    UINT32 rsvd_14 : 1;
    /* rsvd_14 - Bits[14:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 biasfwddoubledata : 1;
    /* biasfwddoubledata - Bits[15:15], RW_LB, default = 1'b1 
       RspFwdIWB when HOME!=Local (BiasFwd must be enabled)
     */
    UINT32 biasfwdlocalhome : 1;
    /* biasfwdlocalhome - Bits[16:16], RW_LB, default = 1'b1 
       RspFwdIWB when HOME!=Requestor (BiasFwd must be enabled).
     */
    UINT32 earlyrtidrelease : 1;
    /* earlyrtidrelease - Bits[17:17], RW_LB, default = 1'b1 
       Enables the releasing of RTID before core snoops due to Cmp_Fwd*.  When clear, 
       the Cbo will return the RTID after snooping the cores for a Cmp_Fwd*.  When set, 
       the RTID will be returned before sending the core snoops. 
     */
    UINT32 llcmissendrd : 1;
    /* llcmissendrd - Bits[18:18], RW_LB, default = 1'b0 
       Enable LLC Miss message for DRd
     */
    UINT32 llcmissendrdpte : 1;
    /* llcmissendrdpte - Bits[19:19], RW_LB, default = 1'b1 
       Enable LLC Miss message for DRdPTE
     */
    UINT32 disisoctorres : 1;
    /* disisoctorres - Bits[20:20], RW_LB, default = 1'b0 
       Disable TOR ISOC reservation
     */
    UINT32 disisocegrres : 1;
    /* disisocegrres - Bits[21:21], RW_LB, default = 1'b0 
       Disable ISOC Egress Reservation
     */
    UINT32 rsvd_22 : 1;
    /* rsvd_22 - Bits[22:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 disisocrtidres : 1;
    /* disisocrtidres - Bits[23:23], RW_LB, default = 1'b0 
       Disable ISOC RTID Reservation
     */
    UINT32 sendrddataasmigratory : 1;
    /* sendrddataasmigratory - Bits[24:24], RW_LB, default = 1'b0 
       Send RdDataMigratory instead of RdData, so that a modified line can be returned
     */
    UINT32 enablemp : 1;
    /* enablemp - Bits[25:25], RW_LB, default = 1'b0 
       Cache forwarded modified line in M-prime state instead of M-state
     */
    UINT32 cbotraffictoqpilinksintlv : 1;
    /* cbotraffictoqpilinksintlv - Bits[26:26], RW_LB, default = 1'b0 
       This is the cbo_interleave enable bit for any traffic that goes from Cbos to the 
       QPI0/1 ports in 2S configuration with 2 links (QPI0/QPI1) between the sockets  
     */
    UINT32 conddisablef : 1;
    /* conddisablef - Bits[27:27], RW_LB, default = 1'b0 
       When set, downgrade from F to S without forwarding on 
       SnpData/Code/Cur/DataMigratory from a remote home 
     */
    UINT32 localkeepffwds : 1;
    /* localkeepffwds - Bits[28:28], RW_LB, default = 1'b0 
       When set, keep the line in F state and forward S state on 
       SnpData/Code/Cur/DataMigratory from a local home 
     */
    UINT32 enablelochtcred : 1;
    /* enablelochtcred - Bits[29:29], RW_LB, default = 1'b0 
       Local home tracker credits are needed for all explicit writeback requests to a 
       local HA 
     */
    UINT32 uselochtcredforread : 1;
    /* uselochtcredforread - Bits[30:30], RW_LB, default = 1'b1 
       Local home tracker credits can be used for reads if more than 1 credit is 
       available. If EnableLocHTCred is 0, this bit has no effect. 
     */
    UINT32 disablemesegtorres : 1;
    /* disablemesegtorres - Bits[31:31], RW_LB, default = 1'b0 
       When it is set, do not reserve TOR entry for VCm in non-ISOC mode
     */
  } Bits;
  UINT32 Data;
} CBO_COH_CONFIG_CBOBC_COH_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPI_TIMEOUT_CTRL_CBOBC_COH_REG supported on:                               */
/*       IVT_EP (0x401B205C)                                                  */
/*       IVT_EX (0x401B205C)                                                  */
/*       HSX (0x4017E05C)                                                     */
/*       BDX (0x4017E05C)                                                     */
/* Register default value:              0x007FFFFF                            */
#define QPI_TIMEOUT_CTRL_CBOBC_COH_REG 0x0102405C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
 * Intel QPI/TOR Timeout Configuration
 */
typedef union {
  struct {
    UINT32 entry_ctr_inc_ctl : 3;
    /* entry_ctr_inc_ctl - Bits[2:0], RWS_LB, default = 3'b111 
       Indication of which bit of upper base counter increments entry counter.
                          000: no upper-base counter is used ( ~ 17 us timeout 
       @2.67Ghz). 
                          001: 2-bit upper-base counter is used. ( ~ 70 us timeout 
       @2.67Ghz). 
                          010: 5-bit upper-base counter is used. ( ~ 558 us timeout 
       @2.67Ghz).  
                          011: 8-bit upper-base counter is used. (~ 4.5 ms timeout 
       @2.67Ghz). 
                          100: 11-bit upper-base counter is used. (~ 35 ms timeout 
       @2.67Ghz). 
                          101: 14-bit upper-base counter is used. (~ 285 ms timeout 
       @2.67Ghz). 
                          110: 16-bit upper-base counter is used. (~ 1.1 second timeout 
       @2.67Ghz). 
                          111: 18-bit upper-base counter is used. (~ 4.6 second timeout 
       @2.67Ghz).   
     */
    UINT32 level1_offset : 5;
    /* level1_offset - Bits[7:3], RWS_LB, default = 5'd31 
        Encoded Intel QPI Level 1 Timeout offset. Offsets must be > 1. 
     */
    UINT32 level3_offset : 5;
    /* level3_offset - Bits[12:8], RWS_LB, default = 5'd31 
        Encoded Intel QPI Level 3 Timeout offset. Offsets must be > 1. 
               Level6, Level4, Level3 offsets must be >= Level1 offset 
     */
    UINT32 level4_offset : 5;
    /* level4_offset - Bits[17:13], RWS_LB, default = 5'd31 
        Encoded Intel QPI Level 4 Timeout offset. Offsets must be > 1. 
               Level6, Level4, Level3 offsets must be >= Level1 offset 
     */
    UINT32 level6_offset : 5;
    /* level6_offset - Bits[22:18], RWS_LB, default = 5'd31 
        Encoded Intel QPI Level 6 Timeout offset. Offsets must be > 1. 
               Level6, Level4, Level3 offsets must be >= Level1 offset. 
     */
    UINT32 rsvd_23 : 6;
    /* rsvd_23 - Bits[28:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 enabletortimeout : 1;
    /* enabletortimeout - Bits[29:29], RWS_LB, default = 1'b0 
       Enable Intel QPI Timeout for certain classes of Intel QPI transactions.
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPI_TIMEOUT_CTRL_CBOBC_COH_STRUCT;
#endif /* ASM_INC */


/* RTID_RECONFIG_CBOBC_COH_REG supported on:                                  */
/*       IVT_EP (0x401B2070)                                                  */
/*       IVT_EX (0x401B2070)                                                  */
/*       HSX (0x4017E06C)                                                     */
/*       BDX (0x4017E06C)                                                     */
/* Register default value:              0x00000000                            */
#define RTID_RECONFIG_CBOBC_COH_REG 0x0102C000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
 * RTID Reconfiguration CSR
 */
typedef union {
  struct {
    UINT32 loadsharedtoreal : 1;
    /* loadsharedtoreal - Bits[0:0], RW_LB, default = 1'b0 
       Load Shared base RTID CSR to Real CSR. Setting this bit will cause Cbo to copy 
       Shared base RTID CSR to base RTID CSR when Cbo has no outstanding RTID.  If 
       RTID_Pool_Config_CFG.swapRTID is set, it first must be cleared before 
       reconfiguring the RTID pools. 
       Flow:
         a. Set rtid_pool_config_shadow.swaprtid = 0
         b. Set rtid_reconfig.loadsharedtoreal = 1
         c. Set rtid_reconfig.loadsharedtoreal = 0
         d. Update all RTID config shadow registers and the RTID pool size registers
         e. Set rtid_pool_config_shadow.swaprtid back to 1
         f. Wait 256 Uclk cycles
         g. Set rtid_reconfig.loadsharedtoreal = 1
         h. Set rtid_reconfig.loadsharedtoreal = 0
       
     */
    UINT32 rsvd : 31;
    /* rsvd - Bits[31:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_RECONFIG_CBOBC_COH_STRUCT;
#endif /* ASM_INC */


/* NUM_OF_HOPS_CBOBC_COH_REG supported on:                                    */
/*       HSX (0x4017E080)                                                     */
/*       BDX (0x4017E080)                                                     */
/* Register default value:              0x00000000                            */
#define NUM_OF_HOPS_CBOBC_COH_REG 0x01024080

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.6.CFG.xml.
 *  Core responses use the number of Hops. This register is used by the Performance 
 * Monitoring Logic in the core for some specific PMU events. 
 */
typedef union {
  struct {
    UINT32 ha0 : 2;
    /* ha0 - Bits[1:0], RW, default = 2'b00 
       HOPS for HNID[3,1:0]=0
     */
    UINT32 ha1 : 2;
    /* ha1 - Bits[3:2], RW, default = 2'b00 
       HOPS for HNID[3,1:0]=1
     */
    UINT32 ha2 : 2;
    /* ha2 - Bits[5:4], RW, default = 2'b00 
       HOPS for HNID[3,1:0]=2
     */
    UINT32 ha3 : 2;
    /* ha3 - Bits[7:6], RW, default = 2'b00 
       HOPS for HNID[3,1:0]=3
     */
    UINT32 ha4 : 2;
    /* ha4 - Bits[9:8], RW, default = 2'b00 
       HOPS for HNID[3,1:0]=4
     */
    UINT32 ha5 : 2;
    /* ha5 - Bits[11:10], RW, default = 2'b00 
       HOPS for HNID[3,1:0]=5
     */
    UINT32 ha6 : 2;
    /* ha6 - Bits[13:12], RW, default = 2'b00 
       HOPS for HNID[3,1:0]=6
     */
    UINT32 ha7 : 2;
    /* ha7 - Bits[15:14], RW, default = 2'b00 
       HOPS for HNID[3,1:0]=7
     */
    UINT32 ca0 : 2;
    /* ca0 - Bits[17:16], RW, default = 2'b00 
       HOPS for SID[2:0]=0
     */
    UINT32 ca1 : 2;
    /* ca1 - Bits[19:18], RW, default = 2'b00 
       HOPS for SID[2:0]=1
     */
    UINT32 ca2 : 2;
    /* ca2 - Bits[21:20], RW, default = 2'b00 
       HOPS for SID[2:0]=2
     */
    UINT32 ca3 : 2;
    /* ca3 - Bits[23:22], RW, default = 2'b00 
       HOPS for SID[2:0]=3
     */
    UINT32 ca4 : 2;
    /* ca4 - Bits[25:24], RW, default = 2'b00 
       HOPS for SID[2:0]=4
     */
    UINT32 ca5 : 2;
    /* ca5 - Bits[27:26], RW, default = 2'b00 
       HOPS for SID[2:0]=5
     */
    UINT32 ca6 : 2;
    /* ca6 - Bits[29:28], RW, default = 2'b00 
       HOPS for SID[2:0]=6
     */
    UINT32 ca7 : 2;
    /* ca7 - Bits[31:30], RW, default = 2'b00 
       HOPS for SID[2:0]=7
     */
  } Bits;
  UINT32 Data;
} NUM_OF_HOPS_CBOBC_COH_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CBO_MCA_CONFIG_CBOBC_COH_REG supported on:                                 */
/*       IVT_EP (0x401B2058)                                                  */
/*       IVT_EX (0x401B2058)                                                  */
/* Register default value:              0x00000000                            */
#define CBO_MCA_CONFIG_CBOBC_COH_REG 0x01024058



/* CBO_MISC_CONFIG_CBOBC_COH_REG supported on:                                */
/*       IVT_EP (0x401B2068)                                                  */
/*       IVT_EX (0x401B2068)                                                  */
/* Register default value:              0x00000000                            */
#define CBO_MISC_CONFIG_CBOBC_COH_REG 0x01024068



/* TOLM_CBOBC_COH_REG supported on:                                           */
/*       IVT_EP (0x401B2080)                                                  */
/*       IVT_EX (0x401B2080)                                                  */
/* Register default value:              0x00000001                            */
#define TOLM_CBOBC_COH_REG 0x01024080



/* TOHM_CBOBC_COH_REG supported on:                                           */
/*       IVT_EP (0x401B2084)                                                  */
/*       IVT_EX (0x401B2084)                                                  */
/* Register default value:              0x00007FFF                            */
#define TOHM_CBOBC_COH_REG 0x01024084



#endif /* CBOBC_COH_h */
