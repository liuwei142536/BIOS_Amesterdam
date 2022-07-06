/* Date Stamp: 8/23/2014 */

#ifndef CBOBC_IOSAD_h
#define CBOBC_IOSAD_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* CBOBC_IOSAD_IVT_DEV 22                                                     */
/* CBOBC_IOSAD_IVT_FUN 1                                                      */
/* For HSX_HOST:                                                              */
/* CBOBC_IOSAD_HSX_DEV 15                                                     */
/* CBOBC_IOSAD_HSX_FUN 5                                                      */
/* For BDX_HOST:                                                              */
/* CBOBC_IOSAD_BDX_DEV 15                                                     */
/* CBOBC_IOSAD_BDX_FUN 5                                                      */

/* VID_CBOBC_IOSAD_REG supported on:                                          */
/*       IVT_EP (0x201B1000)                                                  */
/*       IVT_EX (0x201B1000)                                                  */
/*       HSX (0x2017D000)                                                     */
/*       BDX (0x2017D000)                                                     */
/* Register default value:              0x8086                                */
#define VID_CBOBC_IOSAD_REG 0x01012000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
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
} VID_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* DID_CBOBC_IOSAD_REG supported on:                                          */
/*       IVT_EP (0x201B1002)                                                  */
/*       IVT_EX (0x201B1002)                                                  */
/*       HSX (0x2017D002)                                                     */
/*       BDX (0x2017D002)                                                     */
/* Register default value on IVT_EP:    0x0EC9                                */
/* Register default value on IVT_EX:    0x0EC9                                */
/* Register default value on HSX:       0x2FFD                                */
/* Register default value on BDX:       0x6FFD                                */
#define DID_CBOBC_IOSAD_REG 0x01012002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6FFD 
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
} DID_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* PCICMD_CBOBC_IOSAD_REG supported on:                                       */
/*       HSX (0x2017D004)                                                     */
/*       BDX (0x2017D004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_CBOBC_IOSAD_REG 0x01012004

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
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
} PCICMD_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PCISTS_CBOBC_IOSAD_REG supported on:                                       */
/*       HSX (0x2017D006)                                                     */
/*       BDX (0x2017D006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_CBOBC_IOSAD_REG 0x01012006

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
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
} PCISTS_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RID_CBOBC_IOSAD_REG supported on:                                          */
/*       IVT_EP (0x101B1008)                                                  */
/*       IVT_EX (0x101B1008)                                                  */
/*       HSX (0x1017D008)                                                     */
/*       BDX (0x1017D008)                                                     */
/* Register default value:              0x00                                  */
#define RID_CBOBC_IOSAD_REG 0x01011008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
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
} RID_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_CBOBC_IOSAD_REG supported on:                                       */
/*       IVT_EP (0x101B1009)                                                  */
/*       IVT_EX (0x101B1009)                                                  */
/*       HSX (0x1017D009)                                                     */
/*       BDX (0x1017D009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_CBOBC_IOSAD_REG 0x01011009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.15.5.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_CBOBC_IOSAD_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_CBOBC_IOSAD_REG supported on:                                       */
/*       IVT_EP (0x201B100A)                                                  */
/*       IVT_EX (0x201B100A)                                                  */
/*       HSX (0x2017D00A)                                                     */
/*       BDX (0x2017D00A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_CBOBC_IOSAD_REG 0x0101200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
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
} CCR_N1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* CLSR_CBOBC_IOSAD_REG supported on:                                         */
/*       IVT_EP (0x101B100C)                                                  */
/*       IVT_EX (0x101B100C)                                                  */
/*       HSX (0x1017D00C)                                                     */
/*       BDX (0x1017D00C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_CBOBC_IOSAD_REG 0x0101100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
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
} CLSR_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* PLAT_CBOBC_IOSAD_REG supported on:                                         */
/*       HSX (0x1017D00D)                                                     */
/*       BDX (0x1017D00D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_CBOBC_IOSAD_REG 0x0101100D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
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
} PLAT_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* HDR_CBOBC_IOSAD_REG supported on:                                          */
/*       IVT_EP (0x101B100E)                                                  */
/*       IVT_EX (0x101B100E)                                                  */
/*       HSX (0x1017D00E)                                                     */
/*       BDX (0x1017D00E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_CBOBC_IOSAD_REG 0x0101100E


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.15.5.CFG.xml.
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
} HDR_CBOBC_IOSAD_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* BIST_CBOBC_IOSAD_REG supported on:                                         */
/*       HSX (0x1017D00F)                                                     */
/*       BDX (0x1017D00F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_CBOBC_IOSAD_REG 0x0101100F

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
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
} BIST_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SVID_CBOBC_IOSAD_REG supported on:                                         */
/*       IVT_EP (0x201B102C)                                                  */
/*       IVT_EX (0x201B102C)                                                  */
/*       HSX (0x2017D02C)                                                     */
/*       BDX (0x2017D02C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_CBOBC_IOSAD_REG 0x0101202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
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
} SVID_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* SDID_CBOBC_IOSAD_REG supported on:                                         */
/*       IVT_EP (0x201B102E)                                                  */
/*       IVT_EX (0x201B102E)                                                  */
/*       HSX (0x2017D02E)                                                     */
/*       BDX (0x2017D02E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_CBOBC_IOSAD_REG 0x0101202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
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
} SDID_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_CBOBC_IOSAD_REG supported on:                                       */
/*       HSX (0x1017D034)                                                     */
/*       BDX (0x1017D034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_CBOBC_IOSAD_REG 0x01011034

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
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
} CAPPTR_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTL_CBOBC_IOSAD_REG supported on:                                         */
/*       HSX (0x1017D03C)                                                     */
/*       BDX (0x1017D03C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_CBOBC_IOSAD_REG 0x0101103C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
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
} INTL_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTPIN_CBOBC_IOSAD_REG supported on:                                       */
/*       HSX (0x1017D03D)                                                     */
/*       BDX (0x1017D03D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_CBOBC_IOSAD_REG 0x0101103D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
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
} INTPIN_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MINGNT_CBOBC_IOSAD_REG supported on:                                       */
/*       HSX (0x1017D03E)                                                     */
/*       BDX (0x1017D03E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_CBOBC_IOSAD_REG 0x0101103E

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
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
} MINGNT_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MAXLAT_CBOBC_IOSAD_REG supported on:                                       */
/*       HSX (0x1017D03F)                                                     */
/*       BDX (0x1017D03F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_CBOBC_IOSAD_REG 0x0101103F

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
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
} MAXLAT_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MMIO_RULE_0_N0_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B1040)                                                  */
/*       IVT_EX (0x401B1040)                                                  */
/*       HSX (0x4017D040)                                                     */
/*       BDX (0x4017D040)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_0_N0_CBOBC_IOSAD_REG 0x01014040
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this MMIO rule.
     */
    UINT32 baseaddress : 20;
    /* baseaddress - Bits[20:1], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 64MB. 
     */
    UINT32 baseaddress_lsb_bits : 2;
    /* baseaddress_lsb_bits - Bits[22:21], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 16MB. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limitaddress_lsb_bits : 2;
    /* limitaddress_lsb_bits - Bits[25:24], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 limitaddress : 6;
    /* limitaddress - Bits[31:26], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_0_N0_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_0_N1_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B1044)                                                  */
/*       IVT_EX (0x401B1044)                                                  */
/*       HSX (0x4017D044)                                                     */
/*       BDX (0x4017D044)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_0_N1_CBOBC_IOSAD_REG 0x01014044
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 limitaddress : 14;
    /* limitaddress - Bits[13:0], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_0_N1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_1_N0_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B1048)                                                  */
/*       IVT_EX (0x401B1048)                                                  */
/*       HSX (0x4017D048)                                                     */
/*       BDX (0x4017D048)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_1_N0_CBOBC_IOSAD_REG 0x01014048
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this MMIO rule.
     */
    UINT32 baseaddress : 20;
    /* baseaddress - Bits[20:1], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 64MB. 
     */
    UINT32 baseaddress_lsb_bits : 2;
    /* baseaddress_lsb_bits - Bits[22:21], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 16MB. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limitaddress_lsb_bits : 2;
    /* limitaddress_lsb_bits - Bits[25:24], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 limitaddress : 6;
    /* limitaddress - Bits[31:26], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_1_N0_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_1_N1_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B104C)                                                  */
/*       IVT_EX (0x401B104C)                                                  */
/*       HSX (0x4017D04C)                                                     */
/*       BDX (0x4017D04C)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_1_N1_CBOBC_IOSAD_REG 0x0101404C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 limitaddress : 14;
    /* limitaddress - Bits[13:0], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_1_N1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_2_N0_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B1050)                                                  */
/*       IVT_EX (0x401B1050)                                                  */
/*       HSX (0x4017D050)                                                     */
/*       BDX (0x4017D050)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_2_N0_CBOBC_IOSAD_REG 0x01014050
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this MMIO rule.
     */
    UINT32 baseaddress : 20;
    /* baseaddress - Bits[20:1], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 64MB. 
     */
    UINT32 baseaddress_lsb_bits : 2;
    /* baseaddress_lsb_bits - Bits[22:21], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 16MB. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limitaddress_lsb_bits : 2;
    /* limitaddress_lsb_bits - Bits[25:24], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 limitaddress : 6;
    /* limitaddress - Bits[31:26], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_2_N0_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_2_N1_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B1054)                                                  */
/*       IVT_EX (0x401B1054)                                                  */
/*       HSX (0x4017D054)                                                     */
/*       BDX (0x4017D054)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_2_N1_CBOBC_IOSAD_REG 0x01014054
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 limitaddress : 14;
    /* limitaddress - Bits[13:0], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_2_N1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_3_N0_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B1058)                                                  */
/*       IVT_EX (0x401B1058)                                                  */
/*       HSX (0x4017D058)                                                     */
/*       BDX (0x4017D058)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_3_N0_CBOBC_IOSAD_REG 0x01014058
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this MMIO rule.
     */
    UINT32 baseaddress : 20;
    /* baseaddress - Bits[20:1], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 64MB. 
     */
    UINT32 baseaddress_lsb_bits : 2;
    /* baseaddress_lsb_bits - Bits[22:21], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 16MB. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limitaddress_lsb_bits : 2;
    /* limitaddress_lsb_bits - Bits[25:24], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 limitaddress : 6;
    /* limitaddress - Bits[31:26], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_3_N0_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_3_N1_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B105C)                                                  */
/*       IVT_EX (0x401B105C)                                                  */
/*       HSX (0x4017D05C)                                                     */
/*       BDX (0x4017D05C)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_3_N1_CBOBC_IOSAD_REG 0x0101405C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 limitaddress : 14;
    /* limitaddress - Bits[13:0], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_3_N1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_4_N0_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B1060)                                                  */
/*       IVT_EX (0x401B1060)                                                  */
/*       HSX (0x4017D060)                                                     */
/*       BDX (0x4017D060)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_4_N0_CBOBC_IOSAD_REG 0x01014060
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this MMIO rule.
     */
    UINT32 baseaddress : 20;
    /* baseaddress - Bits[20:1], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 64MB. 
     */
    UINT32 baseaddress_lsb_bits : 2;
    /* baseaddress_lsb_bits - Bits[22:21], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 16MB. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limitaddress_lsb_bits : 2;
    /* limitaddress_lsb_bits - Bits[25:24], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 limitaddress : 6;
    /* limitaddress - Bits[31:26], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_4_N0_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_4_N1_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B1064)                                                  */
/*       IVT_EX (0x401B1064)                                                  */
/*       HSX (0x4017D064)                                                     */
/*       BDX (0x4017D064)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_4_N1_CBOBC_IOSAD_REG 0x01014064
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 limitaddress : 14;
    /* limitaddress - Bits[13:0], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_4_N1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_5_N0_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B1068)                                                  */
/*       IVT_EX (0x401B1068)                                                  */
/*       HSX (0x4017D068)                                                     */
/*       BDX (0x4017D068)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_5_N0_CBOBC_IOSAD_REG 0x01014068
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this MMIO rule.
     */
    UINT32 baseaddress : 20;
    /* baseaddress - Bits[20:1], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 64MB. 
     */
    UINT32 baseaddress_lsb_bits : 2;
    /* baseaddress_lsb_bits - Bits[22:21], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 16MB. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limitaddress_lsb_bits : 2;
    /* limitaddress_lsb_bits - Bits[25:24], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 limitaddress : 6;
    /* limitaddress - Bits[31:26], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_5_N0_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_5_N1_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B106C)                                                  */
/*       IVT_EX (0x401B106C)                                                  */
/*       HSX (0x4017D06C)                                                     */
/*       BDX (0x4017D06C)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_5_N1_CBOBC_IOSAD_REG 0x0101406C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 limitaddress : 14;
    /* limitaddress - Bits[13:0], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_5_N1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_6_N0_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B1070)                                                  */
/*       IVT_EX (0x401B1070)                                                  */
/*       HSX (0x4017D070)                                                     */
/*       BDX (0x4017D070)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_6_N0_CBOBC_IOSAD_REG 0x01014070
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this MMIO rule.
     */
    UINT32 baseaddress : 20;
    /* baseaddress - Bits[20:1], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 64MB. 
     */
    UINT32 baseaddress_lsb_bits : 2;
    /* baseaddress_lsb_bits - Bits[22:21], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 16MB. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limitaddress_lsb_bits : 2;
    /* limitaddress_lsb_bits - Bits[25:24], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 limitaddress : 6;
    /* limitaddress - Bits[31:26], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_6_N0_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_6_N1_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B1074)                                                  */
/*       IVT_EX (0x401B1074)                                                  */
/*       HSX (0x4017D074)                                                     */
/*       BDX (0x4017D074)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_6_N1_CBOBC_IOSAD_REG 0x01014074
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 limitaddress : 14;
    /* limitaddress - Bits[13:0], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_6_N1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_7_N0_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B1078)                                                  */
/*       IVT_EX (0x401B1078)                                                  */
/*       HSX (0x4017D078)                                                     */
/*       BDX (0x4017D078)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_7_N0_CBOBC_IOSAD_REG 0x01014078
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this MMIO rule.
     */
    UINT32 baseaddress : 20;
    /* baseaddress - Bits[20:1], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 64MB. 
     */
    UINT32 baseaddress_lsb_bits : 2;
    /* baseaddress_lsb_bits - Bits[22:21], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 16MB. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limitaddress_lsb_bits : 2;
    /* limitaddress_lsb_bits - Bits[25:24], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 limitaddress : 6;
    /* limitaddress - Bits[31:26], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_7_N0_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_7_N1_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B107C)                                                  */
/*       IVT_EX (0x401B107C)                                                  */
/*       HSX (0x4017D07C)                                                     */
/*       BDX (0x4017D07C)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_7_N1_CBOBC_IOSAD_REG 0x0101407C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 limitaddress : 14;
    /* limitaddress - Bits[13:0], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_7_N1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_8_N0_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B1080)                                                  */
/*       IVT_EX (0x401B1080)                                                  */
/*       HSX (0x4017D080)                                                     */
/*       BDX (0x4017D080)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_8_N0_CBOBC_IOSAD_REG 0x01014080
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this MMIO rule.
     */
    UINT32 baseaddress : 20;
    /* baseaddress - Bits[20:1], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 64MB. 
     */
    UINT32 baseaddress_lsb_bits : 2;
    /* baseaddress_lsb_bits - Bits[22:21], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 16MB. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limitaddress_lsb_bits : 2;
    /* limitaddress_lsb_bits - Bits[25:24], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 limitaddress : 6;
    /* limitaddress - Bits[31:26], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_8_N0_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_8_N1_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B1084)                                                  */
/*       IVT_EX (0x401B1084)                                                  */
/*       HSX (0x4017D084)                                                     */
/*       BDX (0x4017D084)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_8_N1_CBOBC_IOSAD_REG 0x01014084
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 limitaddress : 14;
    /* limitaddress - Bits[13:0], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_8_N1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_9_N0_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B1088)                                                  */
/*       IVT_EX (0x401B1088)                                                  */
/*       HSX (0x4017D088)                                                     */
/*       BDX (0x4017D088)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_9_N0_CBOBC_IOSAD_REG 0x01014088
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this MMIO rule.
     */
    UINT32 baseaddress : 20;
    /* baseaddress - Bits[20:1], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 64MB. 
     */
    UINT32 baseaddress_lsb_bits : 2;
    /* baseaddress_lsb_bits - Bits[22:21], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 16MB. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limitaddress_lsb_bits : 2;
    /* limitaddress_lsb_bits - Bits[25:24], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 limitaddress : 6;
    /* limitaddress - Bits[31:26], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_9_N0_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_9_N1_CBOBC_IOSAD_REG supported on:                               */
/*       IVT_EP (0x401B108C)                                                  */
/*       IVT_EX (0x401B108C)                                                  */
/*       HSX (0x4017D08C)                                                     */
/*       BDX (0x4017D08C)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_9_N1_CBOBC_IOSAD_REG 0x0101408C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 limitaddress : 14;
    /* limitaddress - Bits[13:0], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_9_N1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_10_N0_CBOBC_IOSAD_REG supported on:                              */
/*       IVT_EP (0x401B1090)                                                  */
/*       IVT_EX (0x401B1090)                                                  */
/*       HSX (0x4017D090)                                                     */
/*       BDX (0x4017D090)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_10_N0_CBOBC_IOSAD_REG 0x01014090
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this MMIO rule.
     */
    UINT32 baseaddress : 20;
    /* baseaddress - Bits[20:1], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 64MB. 
     */
    UINT32 baseaddress_lsb_bits : 2;
    /* baseaddress_lsb_bits - Bits[22:21], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 16MB. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limitaddress_lsb_bits : 2;
    /* limitaddress_lsb_bits - Bits[25:24], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 limitaddress : 6;
    /* limitaddress - Bits[31:26], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_10_N0_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_10_N1_CBOBC_IOSAD_REG supported on:                              */
/*       IVT_EP (0x401B1094)                                                  */
/*       IVT_EX (0x401B1094)                                                  */
/*       HSX (0x4017D094)                                                     */
/*       BDX (0x4017D094)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_10_N1_CBOBC_IOSAD_REG 0x01014094
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 limitaddress : 14;
    /* limitaddress - Bits[13:0], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_10_N1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_11_N0_CBOBC_IOSAD_REG supported on:                              */
/*       IVT_EP (0x401B1098)                                                  */
/*       IVT_EX (0x401B1098)                                                  */
/*       HSX (0x4017D098)                                                     */
/*       BDX (0x4017D098)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_11_N0_CBOBC_IOSAD_REG 0x01014098
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this MMIO rule.
     */
    UINT32 baseaddress : 20;
    /* baseaddress - Bits[20:1], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 64MB. 
     */
    UINT32 baseaddress_lsb_bits : 2;
    /* baseaddress_lsb_bits - Bits[22:21], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 16MB. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limitaddress_lsb_bits : 2;
    /* limitaddress_lsb_bits - Bits[25:24], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 limitaddress : 6;
    /* limitaddress - Bits[31:26], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_11_N0_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_11_N1_CBOBC_IOSAD_REG supported on:                              */
/*       IVT_EP (0x401B109C)                                                  */
/*       IVT_EX (0x401B109C)                                                  */
/*       HSX (0x4017D09C)                                                     */
/*       BDX (0x4017D09C)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_11_N1_CBOBC_IOSAD_REG 0x0101409C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 limitaddress : 14;
    /* limitaddress - Bits[13:0], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_11_N1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_12_N0_CBOBC_IOSAD_REG supported on:                              */
/*       IVT_EP (0x401B10A0)                                                  */
/*       IVT_EX (0x401B10A0)                                                  */
/*       HSX (0x4017D0A0)                                                     */
/*       BDX (0x4017D0A0)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_12_N0_CBOBC_IOSAD_REG 0x010140A0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this MMIO rule.
     */
    UINT32 baseaddress : 20;
    /* baseaddress - Bits[20:1], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 64MB. 
     */
    UINT32 baseaddress_lsb_bits : 2;
    /* baseaddress_lsb_bits - Bits[22:21], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 16MB. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limitaddress_lsb_bits : 2;
    /* limitaddress_lsb_bits - Bits[25:24], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 limitaddress : 6;
    /* limitaddress - Bits[31:26], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_12_N0_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_12_N1_CBOBC_IOSAD_REG supported on:                              */
/*       IVT_EP (0x401B10A4)                                                  */
/*       IVT_EX (0x401B10A4)                                                  */
/*       HSX (0x4017D0A4)                                                     */
/*       BDX (0x4017D0A4)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_12_N1_CBOBC_IOSAD_REG 0x010140A4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 limitaddress : 14;
    /* limitaddress - Bits[13:0], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_12_N1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_13_N0_CBOBC_IOSAD_REG supported on:                              */
/*       IVT_EP (0x401B10A8)                                                  */
/*       IVT_EX (0x401B10A8)                                                  */
/*       HSX (0x4017D0A8)                                                     */
/*       BDX (0x4017D0A8)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_13_N0_CBOBC_IOSAD_REG 0x010140A8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this MMIO rule.
     */
    UINT32 baseaddress : 20;
    /* baseaddress - Bits[20:1], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 64MB. 
     */
    UINT32 baseaddress_lsb_bits : 2;
    /* baseaddress_lsb_bits - Bits[22:21], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 16MB. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limitaddress_lsb_bits : 2;
    /* limitaddress_lsb_bits - Bits[25:24], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 limitaddress : 6;
    /* limitaddress - Bits[31:26], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_13_N0_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_13_N1_CBOBC_IOSAD_REG supported on:                              */
/*       IVT_EP (0x401B10AC)                                                  */
/*       IVT_EX (0x401B10AC)                                                  */
/*       HSX (0x4017D0AC)                                                     */
/*       BDX (0x4017D0AC)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_13_N1_CBOBC_IOSAD_REG 0x010140AC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 limitaddress : 14;
    /* limitaddress - Bits[13:0], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_13_N1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_14_N0_CBOBC_IOSAD_REG supported on:                              */
/*       IVT_EP (0x401B10B0)                                                  */
/*       IVT_EX (0x401B10B0)                                                  */
/*       HSX (0x4017D0B0)                                                     */
/*       BDX (0x4017D0B0)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_14_N0_CBOBC_IOSAD_REG 0x010140B0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this MMIO rule.
     */
    UINT32 baseaddress : 20;
    /* baseaddress - Bits[20:1], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 64MB. 
     */
    UINT32 baseaddress_lsb_bits : 2;
    /* baseaddress_lsb_bits - Bits[22:21], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 16MB. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limitaddress_lsb_bits : 2;
    /* limitaddress_lsb_bits - Bits[25:24], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 limitaddress : 6;
    /* limitaddress - Bits[31:26], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_14_N0_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_14_N1_CBOBC_IOSAD_REG supported on:                              */
/*       IVT_EP (0x401B10B4)                                                  */
/*       IVT_EX (0x401B10B4)                                                  */
/*       HSX (0x4017D0B4)                                                     */
/*       BDX (0x4017D0B4)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_14_N1_CBOBC_IOSAD_REG 0x010140B4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 limitaddress : 14;
    /* limitaddress - Bits[13:0], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_14_N1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_15_N0_CBOBC_IOSAD_REG supported on:                              */
/*       IVT_EP (0x401B10B8)                                                  */
/*       IVT_EX (0x401B10B8)                                                  */
/*       HSX (0x4017D0B8)                                                     */
/*       BDX (0x4017D0B8)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_15_N0_CBOBC_IOSAD_REG 0x010140B8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this MMIO rule.
     */
    UINT32 baseaddress : 20;
    /* baseaddress - Bits[20:1], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 64MB. 
     */
    UINT32 baseaddress_lsb_bits : 2;
    /* baseaddress_lsb_bits - Bits[22:21], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule base address. Both base and 
       limit must match to declare a match to this MMIO rule. The granularity of MMIO 
       rule is 16MB. 
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limitaddress_lsb_bits : 2;
    /* limitaddress_lsb_bits - Bits[25:24], RW_LB, default = 2'b00 
       This correspond to Addr[25:24] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 limitaddress : 6;
    /* limitaddress - Bits[31:26], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_15_N0_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_RULE_15_N1_CBOBC_IOSAD_REG supported on:                              */
/*       IVT_EP (0x401B10BC)                                                  */
/*       IVT_EX (0x401B10BC)                                                  */
/*       HSX (0x4017D0BC)                                                     */
/*       BDX (0x4017D0BC)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_RULE_15_N1_CBOBC_IOSAD_REG 0x010140BC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 limitaddress : 14;
    /* limitaddress - Bits[13:0], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMIO rule top limit address. Both base and 
       limit must match to declare a match to this MMIO rule. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMIO_RULE_15_N1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMCFG_RULE_N0_CBOBC_IOSAD_REG supported on:                                */
/*       IVT_EP (0x401B10C0)                                                  */
/*       IVT_EX (0x401B10C0)                                                  */
/*       HSX (0x4017D0C0)                                                     */
/*       BDX (0x4017D0C0)                                                     */
/* Register default value:              0x00000000                            */
#define MMCFG_RULE_N0_CBOBC_IOSAD_REG 0x010140C0


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this MMCFG interleave decoder.
     */
    UINT32 length : 2;
    /* length - Bits[2:1], RW_LB, default = 2'b00 
       The following MMCFG range configurations are supported by the interleave 
       decoder: 
           2'b11 : Reserved
           2'b10 : 64MB MMCFG range, MaxBusNumber = 63.
           2'b01 : 128MB MMCFG range, MaxBusNumber = 127.
           2'b00 : 256MB MMCFG range, MaxBusNumber = 255.
       
     */
    UINT32 rsvd : 23;
    /* rsvd - Bits[25:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_address : 6;
    /* base_address - Bits[31:26], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMCFG rule base address. The granularity 
       of MMCFG rule is 64MB. Note that this interleave decoder can be used for higher 
       segments of the MMCFG and is not restricted to Segment 0 of MMCFG. 
       Check MMCFG_TargetList for Interleaved target list used by this decoder.
     */
  } Bits;
  UINT32 Data;
} MMCFG_RULE_N0_CBOBC_IOSAD_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* MMCFG_RULE_N1_CBOBC_IOSAD_REG supported on:                                */
/*       IVT_EP (0x401B10C4)                                                  */
/*       IVT_EX (0x401B10C4)                                                  */
/*       HSX (0x4017D0C4)                                                     */
/*       BDX (0x4017D0C4)                                                     */
/* Register default value:              0x00000000                            */
#define MMCFG_RULE_N1_CBOBC_IOSAD_REG 0x010140C4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 base_address : 14;
    /* base_address - Bits[13:0], RW_LB, default = 20'b00000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This correspond to Addr[45:26] of the MMCFG rule base address. The granularity 
       of MMCFG rule is 64MB. Note that this interleave decoder can be used for higher 
       segments of the MMCFG and is not restricted to Segment 0 of MMCFG. 
       Check MMCFG_TargetList for Interleaved target list used by this decoder.
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMCFG_RULE_N1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* LT_CONTROL_CBOBC_IOSAD_REG supported on:                                   */
/*       IVT_EP (0x401B10D0)                                                  */
/*       IVT_EX (0x401B10D0)                                                  */
/*       HSX (0x4017D0D0)                                                     */
/*       BDX (0x4017D0D0)                                                     */
/* Register default value:              0x00000000                            */
#define LT_CONTROL_CBOBC_IOSAD_REG 0x010140D0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 lt_target : 3;
    /* lt_target - Bits[2:0], RW_L, default = 3'b000 
       Target NodeID of the Intel TXT Target
     */
    UINT32 lt_target_enable : 1;
    /* lt_target_enable - Bits[3:3], RW1S, default = 1'b0 
       when '1 is written, bit[2:0] below becomes Read-only.
     */
    UINT32 rsvd : 28;
    /* rsvd - Bits[31:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} LT_CONTROL_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* IOPORT_TARGET_LIST_CBOBC_IOSAD_REG supported on:                           */
/*       IVT_EP (0x401B10E0)                                                  */
/*       IVT_EX (0x401B10E0)                                                  */
/*       HSX (0x4017D0E0)                                                     */
/*       BDX (0x4017D0E0)                                                     */
/* Register default value:              0x00000000                            */
#define IOPORT_TARGET_LIST_CBOBC_IOSAD_REG 0x010140E0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 3;
    /* package0 - Bits[2:0], RW_LB, default = 3'b000 
       NodeID of the IOAPIC target.
     */
    UINT32 package1 : 3;
    /* package1 - Bits[5:3], RW_LB, default = 3'b000 
       NodeID of the IOAPIC target.
     */
    UINT32 package2 : 3;
    /* package2 - Bits[8:6], RW_LB, default = 3'b000 
       NodeID of the IOAPIC target.
     */
    UINT32 package3 : 3;
    /* package3 - Bits[11:9], RW_LB, default = 3'b000 
       NodeID of the IOAPIC target.
     */
    UINT32 package4 : 3;
    /* package4 - Bits[14:12], RW_LB, default = 3'b000 
       NodeID of the IOAPIC target.
     */
    UINT32 package5 : 3;
    /* package5 - Bits[17:15], RW_LB, default = 3'b000 
       NodeID of the IOAPIC target.
     */
    UINT32 package6 : 3;
    /* package6 - Bits[20:18], RW_LB, default = 3'b000 
       NodeID of the IOAPIC target.
     */
    UINT32 package7 : 3;
    /* package7 - Bits[23:21], RW_LB, default = 3'b000 
       NodeID of the IOAPIC target.
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} IOPORT_TARGET_LIST_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMCFG_TARGET_LIST_CBOBC_IOSAD_REG supported on:                            */
/*       IVT_EP (0x401B10E4)                                                  */
/*       IVT_EX (0x401B10E4)                                                  */
/*       HSX (0x4017D0E4)                                                     */
/*       BDX (0x4017D0E4)                                                     */
/* Register default value:              0x00000000                            */
#define MMCFG_TARGET_LIST_CBOBC_IOSAD_REG 0x010140E4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 3;
    /* package0 - Bits[2:0], RW_LB, default = 3'b000 
       NodeID of the MMCFG target.
     */
    UINT32 package1 : 3;
    /* package1 - Bits[5:3], RW_LB, default = 3'b000 
       NodeID of the MMCFG target.
     */
    UINT32 package2 : 3;
    /* package2 - Bits[8:6], RW_LB, default = 3'b000 
       NodeID of the MMCFG target.
     */
    UINT32 package3 : 3;
    /* package3 - Bits[11:9], RW_LB, default = 3'b000 
       NodeID of the MMCFG target.
     */
    UINT32 package4 : 3;
    /* package4 - Bits[14:12], RW_LB, default = 3'b000 
       NodeID of the MMCFG target.
     */
    UINT32 package5 : 3;
    /* package5 - Bits[17:15], RW_LB, default = 3'b000 
       NodeID of the MMCFG target.
     */
    UINT32 package6 : 3;
    /* package6 - Bits[20:18], RW_LB, default = 3'b000 
       NodeID of the MMCFG target.
     */
    UINT32 package7 : 3;
    /* package7 - Bits[23:21], RW_LB, default = 3'b000 
       NodeID of the MMCFG target.
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMCFG_TARGET_LIST_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_TARGET_LIST_0_CBOBC_IOSAD_REG supported on:                           */
/*       HSX (0x4017D0E8)                                                     */
/*       BDX (0x4017D0E8)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_TARGET_LIST_0_CBOBC_IOSAD_REG 0x010140E8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 3;
    /* package0 - Bits[2:0], RW_LB, default = 3'b000 
       NodeID of the MMIO target.
     */
    UINT32 package1 : 3;
    /* package1 - Bits[5:3], RW_LB, default = 3'b000 
       NodeID of the MMIO target.
     */
    UINT32 package2 : 3;
    /* package2 - Bits[8:6], RW_LB, default = 3'b000 
       NodeID of the MMIO target.
     */
    UINT32 package3 : 3;
    /* package3 - Bits[11:9], RW_LB, default = 3'b000 
       NodeID of the MMIO target.
     */
    UINT32 package4 : 3;
    /* package4 - Bits[14:12], RW_LB, default = 3'b000 
       NodeID of the MMIO target.
     */
    UINT32 package5 : 3;
    /* package5 - Bits[17:15], RW_LB, default = 3'b000 
       NodeID of the MMIO target.
     */
    UINT32 package6 : 3;
    /* package6 - Bits[20:18], RW_LB, default = 3'b000 
       NodeID of the MMIO target.
     */
    UINT32 package7 : 3;
    /* package7 - Bits[23:21], RW_LB, default = 3'b000 
       NodeID of the MMIO target.
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMIO_TARGET_LIST_0_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* IOAPIC_TARGET_LIST_CBOBC_IOSAD_REG supported on:                           */
/*       IVT_EP (0x401B10EC)                                                  */
/*       IVT_EX (0x401B10EC)                                                  */
/*       HSX (0x4017D0EC)                                                     */
/*       BDX (0x4017D0EC)                                                     */
/* Register default value:              0x00000000                            */
#define IOAPIC_TARGET_LIST_CBOBC_IOSAD_REG 0x010140EC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 3;
    /* package0 - Bits[2:0], RW_LB, default = 3'b000 
       NodeID of the IOAPIC target.
     */
    UINT32 package1 : 3;
    /* package1 - Bits[5:3], RW_LB, default = 3'b000 
       NodeID of the IOAPIC target.
     */
    UINT32 package2 : 3;
    /* package2 - Bits[8:6], RW_LB, default = 3'b000 
       NodeID of the IOAPIC target.
     */
    UINT32 package3 : 3;
    /* package3 - Bits[11:9], RW_LB, default = 3'b000 
       NodeID of the IOAPIC target.
     */
    UINT32 package4 : 3;
    /* package4 - Bits[14:12], RW_LB, default = 3'b000 
       NodeID of the IOAPIC target.
     */
    UINT32 package5 : 3;
    /* package5 - Bits[17:15], RW_LB, default = 3'b000 
       NodeID of the IOAPIC target.
     */
    UINT32 package6 : 3;
    /* package6 - Bits[20:18], RW_LB, default = 3'b000 
       NodeID of the IOAPIC target.
     */
    UINT32 package7 : 3;
    /* package7 - Bits[23:21], RW_LB, default = 3'b000 
       NodeID of the IOAPIC target.
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} IOAPIC_TARGET_LIST_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* SAD_TARGET_CBOBC_IOSAD_REG supported on:                                   */
/*       IVT_EP (0x401B10F0)                                                  */
/*       IVT_EX (0x401B10F0)                                                  */
/*       HSX (0x4017D0F0)                                                     */
/*       BDX (0x4017D0F0)                                                     */
/* Register default value:              0x00000000                            */
#define SAD_TARGET_CBOBC_IOSAD_REG 0x010140F0


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 flash_target : 3;
    /* flash_target - Bits[2:0], RW_LB, default = 3'b000 
       Target NodeID of the Flash Target
     */
    UINT32 legacy_pch_target : 3;
    /* legacy_pch_target - Bits[5:3], RW_LB, default = 3'b000 
       Target NodeID of the Legacy PCH Target
     */
    UINT32 vga_node_id : 3;
    /* vga_node_id - Bits[8:6], RW_LB, default = 3'b000 
       Target NodeID of the VGA Target
     */
    UINT32 sourceid : 3;
    /* sourceid - Bits[11:9], RW_LB, default = 3'b000 
       SourceID of the Socket. Programmable by BIOS. By default, the value should be 
       part of the APICID that represent the socket. 
     */
    UINT32 sourceiden : 1;
    /* sourceiden - Bits[12:12], RW_LB, default = 1'b0 
       Enable SourceID feature.
     */
    UINT32 rsvd : 19;
    /* rsvd - Bits[31:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SAD_TARGET_CBOBC_IOSAD_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* SAD_CONTROL_CBOBC_IOSAD_REG supported on:                                  */
/*       IVT_EP (0x401B10F4)                                                  */
/*       IVT_EX (0x401B10F4)                                                  */
/*       HSX (0x4017D0F4)                                                     */
/*       BDX (0x4017D0F4)                                                     */
/* Register default value:              0x00000000                            */
#define SAD_CONTROL_CBOBC_IOSAD_REG 0x010140F4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 local_nodeid : 4;
    /* local_nodeid - Bits[3:0], RW_L, default = 4'b0000 
       NodeID of the local Socket. This register can not be overwritten (Read only) if 
       LT_Target_Enable ==1 (Under LT_Control). Same as LT_Target, the value can't 
       change to prevent any attack to Intel TXT. 
     */
    UINT32 rsvd : 28;
    /* rsvd - Bits[31:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SAD_CONTROL_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_TARGET_LIST_1_CBOBC_IOSAD_REG supported on:                           */
/*       IVT_EP (0x401B10F8)                                                  */
/*       IVT_EX (0x401B10F8)                                                  */
/*       HSX (0x4017D0F8)                                                     */
/*       BDX (0x4017D0F8)                                                     */
/* Register default value:              0x00000000                            */
#define MMIO_TARGET_LIST_1_CBOBC_IOSAD_REG 0x010140F8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.5.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 3;
    /* package0 - Bits[2:0], RW_LB, default = 3'b000 
       NodeID of the MMIO target.
     */
    UINT32 package1 : 3;
    /* package1 - Bits[5:3], RW_LB, default = 3'b000 
       NodeID of the MMIO target.
     */
    UINT32 package2 : 3;
    /* package2 - Bits[8:6], RW_LB, default = 3'b000 
       NodeID of the MMIO target.
     */
    UINT32 package3 : 3;
    /* package3 - Bits[11:9], RW_LB, default = 3'b000 
       NodeID of the MMIO target.
     */
    UINT32 package4 : 3;
    /* package4 - Bits[14:12], RW_LB, default = 3'b000 
       NodeID of the MMIO target.
     */
    UINT32 package5 : 3;
    /* package5 - Bits[17:15], RW_LB, default = 3'b000 
       NodeID of the MMIO target.
     */
    UINT32 package6 : 3;
    /* package6 - Bits[20:18], RW_LB, default = 3'b000 
       NodeID of the MMIO target.
     */
    UINT32 package7 : 3;
    /* package7 - Bits[23:21], RW_LB, default = 3'b000 
       NodeID of the MMIO target.
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMIO_TARGET_LIST_1_CBOBC_IOSAD_STRUCT;
#endif /* ASM_INC */


/* MMIO_TARGET_LIST_CBOBC_IOSAD_REG supported on:                             */
/*       IVT_EP (0x401B10E8)                                                  */
/*       IVT_EX (0x401B10E8)                                                  */
/* Register default value:              0x00000000                            */
#define MMIO_TARGET_LIST_CBOBC_IOSAD_REG 0x010140E8



#endif /* CBOBC_IOSAD_h */
