/* Date Stamp: 8/23/2014 */

#ifndef R3QPI_CFG_h
#define R3QPI_CFG_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* R3QPI_CFG_IVT_DEV 19                                                       */
/* R3QPI_CFG_IVT_FUN 4                                                        */
/* For HSX_HOST:                                                              */
/* R3QPI_CFG_HSX_DEV 11                                                       */
/* R3QPI_CFG_HSX_FUN 0                                                        */
/* For BDX_HOST:                                                              */
/* R3QPI_CFG_BDX_DEV 11                                                       */
/* R3QPI_CFG_BDX_FUN 0                                                        */

/* VID_R3QPI_CFG_REG supported on:                                            */
/*       IVT_EP (0x2019C000)                                                  */
/*       IVT_EX (0x2019C000)                                                  */
/*       HSX (0x20158000)                                                     */
/*       BDX (0x20158000)                                                     */
/* Register default value:              0x8086                                */
#define VID_R3QPI_CFG_REG 0x05002000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
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
} VID_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* DID_R3QPI_CFG_REG supported on:                                            */
/*       IVT_EP (0x2019C002)                                                  */
/*       IVT_EX (0x2019C002)                                                  */
/*       HSX (0x20158002)                                                     */
/*       BDX (0x20158002)                                                     */
/* Register default value on IVT_EP:    0x0E81                                */
/* Register default value on IVT_EX:    0x0E81                                */
/* Register default value on HSX:       0x2F81                                */
/* Register default value on BDX:       0x6F81                                */
#define DID_R3QPI_CFG_REG 0x05002002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6F81 
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
} DID_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* PCICMD_R3QPI_CFG_REG supported on:                                         */
/*       IVT_EP (0x2019C004)                                                  */
/*       IVT_EX (0x2019C004)                                                  */
/*       HSX (0x20158004)                                                     */
/*       BDX (0x20158004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_R3QPI_CFG_REG 0x05002004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
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
} PCICMD_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* PCISTS_R3QPI_CFG_REG supported on:                                         */
/*       IVT_EP (0x2019C006)                                                  */
/*       IVT_EX (0x2019C006)                                                  */
/*       HSX (0x20158006)                                                     */
/*       BDX (0x20158006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_R3QPI_CFG_REG 0x05002006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.0.CFG.xml.
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
} PCISTS_R3QPI_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_R3QPI_CFG_REG supported on:                                            */
/*       IVT_EP (0x1019C008)                                                  */
/*       IVT_EX (0x1019C008)                                                  */
/*       HSX (0x10158008)                                                     */
/*       BDX (0x10158008)                                                     */
/* Register default value:              0x00                                  */
#define RID_R3QPI_CFG_REG 0x05001008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
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
} RID_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_R3QPI_CFG_REG supported on:                                         */
/*       IVT_EP (0x1019C009)                                                  */
/*       IVT_EX (0x1019C009)                                                  */
/*       HSX (0x10158009)                                                     */
/*       BDX (0x10158009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_R3QPI_CFG_REG 0x05001009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.0.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_R3QPI_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_R3QPI_CFG_REG supported on:                                         */
/*       IVT_EP (0x2019C00A)                                                  */
/*       IVT_EX (0x2019C00A)                                                  */
/*       HSX (0x2015800A)                                                     */
/*       BDX (0x2015800A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_R3QPI_CFG_REG 0x0500200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
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
} CCR_N1_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* CLSR_R3QPI_CFG_REG supported on:                                           */
/*       IVT_EP (0x1019C00C)                                                  */
/*       IVT_EX (0x1019C00C)                                                  */
/*       HSX (0x1015800C)                                                     */
/*       BDX (0x1015800C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_R3QPI_CFG_REG 0x0500100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
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
} CLSR_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* PLAT_R3QPI_CFG_REG supported on:                                           */
/*       IVT_EP (0x1019C00D)                                                  */
/*       IVT_EX (0x1019C00D)                                                  */
/*       HSX (0x1015800D)                                                     */
/*       BDX (0x1015800D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_R3QPI_CFG_REG 0x0500100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
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
} PLAT_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* HDR_R3QPI_CFG_REG supported on:                                            */
/*       IVT_EP (0x1019C00E)                                                  */
/*       IVT_EX (0x1019C00E)                                                  */
/*       HSX (0x1015800E)                                                     */
/*       BDX (0x1015800E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_R3QPI_CFG_REG 0x0500100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
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
} HDR_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* BIST_R3QPI_CFG_REG supported on:                                           */
/*       IVT_EP (0x1019C00F)                                                  */
/*       IVT_EX (0x1019C00F)                                                  */
/*       HSX (0x1015800F)                                                     */
/*       BDX (0x1015800F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_R3QPI_CFG_REG 0x0500100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
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
} BIST_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* SVID_R3QPI_CFG_REG supported on:                                           */
/*       IVT_EP (0x2019C02C)                                                  */
/*       IVT_EX (0x2019C02C)                                                  */
/*       HSX (0x2015802C)                                                     */
/*       BDX (0x2015802C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_R3QPI_CFG_REG 0x0500202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * Subsystem Vendor Identification number
 */
typedef union {
  struct {
    UINT16 subsystem_vendor_identification_number : 16;
    /* subsystem_vendor_identification_number - Bits[15:0], RW_O, default = 16'b1000000010000110 
       Subsystem Vendor Indentification Number
     */
  } Bits;
  UINT16 Data;
} SVID_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* SDID_R3QPI_CFG_REG supported on:                                           */
/*       IVT_EP (0x2019C02E)                                                  */
/*       IVT_EX (0x2019C02E)                                                  */
/*       HSX (0x2015802E)                                                     */
/*       BDX (0x2015802E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_R3QPI_CFG_REG 0x0500202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * Subsystem Device Identification number
 */
typedef union {
  struct {
    UINT16 subsystem_device_identification_number : 16;
    /* subsystem_device_identification_number - Bits[15:0], RW_O, default = 16'b0000000000000000 
       Subsystem Device Indentification Number
     */
  } Bits;
  UINT16 Data;
} SDID_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_R3QPI_CFG_REG supported on:                                         */
/*       IVT_EP (0x1019C034)                                                  */
/*       IVT_EX (0x1019C034)                                                  */
/*       HSX (0x10158034)                                                     */
/*       BDX (0x10158034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_R3QPI_CFG_REG 0x05001034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
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
} CAPPTR_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* INTL_R3QPI_CFG_REG supported on:                                           */
/*       IVT_EP (0x1019C03C)                                                  */
/*       IVT_EX (0x1019C03C)                                                  */
/*       HSX (0x1015803C)                                                     */
/*       BDX (0x1015803C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_R3QPI_CFG_REG 0x0500103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
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
} INTL_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* INTPIN_R3QPI_CFG_REG supported on:                                         */
/*       IVT_EP (0x1019C03D)                                                  */
/*       IVT_EX (0x1019C03D)                                                  */
/*       HSX (0x1015803D)                                                     */
/*       BDX (0x1015803D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_R3QPI_CFG_REG 0x0500103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
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
} INTPIN_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* MINGNT_R3QPI_CFG_REG supported on:                                         */
/*       IVT_EP (0x1019C03E)                                                  */
/*       IVT_EX (0x1019C03E)                                                  */
/*       HSX (0x1015803E)                                                     */
/*       BDX (0x1015803E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_R3QPI_CFG_REG 0x0500103E


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.0.CFG.xml.
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
} MINGNT_R3QPI_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* MAXLAT_R3QPI_CFG_REG supported on:                                         */
/*       IVT_EP (0x1019C03F)                                                  */
/*       IVT_EX (0x1019C03F)                                                  */
/*       HSX (0x1015803F)                                                     */
/*       BDX (0x1015803F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_R3QPI_CFG_REG 0x0500103F


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.0.CFG.xml.
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
} MAXLAT_R3QPI_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* R3QQRT_R3QPI_CFG_REG supported on:                                         */
/*       IVT_EP (0x4019C040)                                                  */
/*       IVT_EX (0x4019C040)                                                  */
/*       HSX (0x40158040)                                                     */
/*       BDX (0x40158040)                                                     */
/* Register default value:              0x00000000                            */
#define R3QQRT_R3QPI_CFG_REG 0x05004040
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * Intel QPI Routing Configuration for both Ports on this R3QPI (00=Local, 
 * 01=CoLocated, 10=Remote-Agent0, 11=Remote-Agent1). A Local port is the port 
 * being configured. A CoLocated port is the adjacent port which shares a ring stop 
 * with the port being configured.  
 *     
 */
typedef union {
  struct {
    UINT32 port0routeskt0 : 2;
    /* port0routeskt0 - Bits[1:0], RWS_LB, default = 2'b00 
       Port0 Intel QPI Packet Route for SocketID = 0
     */
    UINT32 port0routeskt1 : 2;
    /* port0routeskt1 - Bits[3:2], RWS_LB, default = 2'b00 
       Port0 Intel QPI Packet Route for SocketID = 1
     */
    UINT32 port0routeskt2 : 2;
    /* port0routeskt2 - Bits[5:4], RWS_LB, default = 2'b00 
       Port0 Intel QPI Packet Route for SocketID = 2
     */
    UINT32 port0routeskt3 : 2;
    /* port0routeskt3 - Bits[7:6], RWS_LB, default = 2'b00 
       Port0 Intel QPI Packet Route for SocketID = 3
     */
    UINT32 port0routeskt4 : 2;
    /* port0routeskt4 - Bits[9:8], RWS_LB, default = 2'b00 
       Port0 Intel QPI Packet Route for SocketID = 4
     */
    UINT32 port0routeskt5 : 2;
    /* port0routeskt5 - Bits[11:10], RWS_LB, default = 2'b00 
       Port0 Intel QPI Packet Route for SocketID = 5
     */
    UINT32 port0routeskt6 : 2;
    /* port0routeskt6 - Bits[13:12], RWS_LB, default = 2'b00 
       Port0 Intel QPI Packet Route for SocketID = 6
     */
    UINT32 port0routeskt7 : 2;
    /* port0routeskt7 - Bits[15:14], RWS_LB, default = 2'b00 
       Port0 Intel QPI Packet Route for SocketID = 7
     */
    UINT32 port1routeskt0 : 2;
    /* port1routeskt0 - Bits[17:16], RWS_LB, default = 2'b00 
       Port1 Intel QPI Packet Route for SocketID = 0
     */
    UINT32 port1routeskt1 : 2;
    /* port1routeskt1 - Bits[19:18], RWS_LB, default = 2'b00 
       Port1 Intel QPI Packet Route for SocketID = 1
     */
    UINT32 port1routeskt2 : 2;
    /* port1routeskt2 - Bits[21:20], RWS_LB, default = 2'b00 
       Port1 Intel QPI Packet Route for SocketID = 2
     */
    UINT32 port1routeskt3 : 2;
    /* port1routeskt3 - Bits[23:22], RWS_LB, default = 2'b00 
       Port1 Intel QPI Packet Route for SocketID = 3
     */
    UINT32 port1routeskt4 : 2;
    /* port1routeskt4 - Bits[25:24], RWS_LB, default = 2'b00 
       Port1 Intel QPI Packet Route for SocketID = 4
     */
    UINT32 port1routeskt5 : 2;
    /* port1routeskt5 - Bits[27:26], RWS_LB, default = 2'b00 
       Port1 Intel QPI Packet Route for SocketID = 5
     */
    UINT32 port1routeskt6 : 2;
    /* port1routeskt6 - Bits[29:28], RWS_LB, default = 2'b00 
       Port1 Intel QPI Packet Route for SocketID = 6
     */
    UINT32 port1routeskt7 : 2;
    /* port1routeskt7 - Bits[31:30], RWS_LB, default = 2'b00 
       Port1 Intel QPI Packet Route for SocketID = 7
     */
  } Bits;
  UINT32 Data;
} R3QQRT_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* R3QRTE0CR_R3QPI_CFG_REG supported on:                                      */
/*       IVT_EP (0x4019C044)                                                  */
/*       IVT_EX (0x4019C044)                                                  */
/*       HSX (0x40158044)                                                     */
/*       BDX (0x40158044)                                                     */
/* Register default value:              0x01500150                            */
#define R3QRTE0CR_R3QPI_CFG_REG 0x05004044
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI RouteThrough Egress Credit Port0 
 */
typedef union {
  struct {
    UINT32 advnaqpi : 4;
    /* advnaqpi - Bits[3:0], RWS, default = 4'b0000 
        Q2Q AD VNA credit. It is Only used in systems that enable Route-Through
     */
    UINT32 vn0snpqpi : 2;
    /* vn0snpqpi - Bits[5:4], RWS, default = 2'b01 
       Q2Q SNP VN0 credit. Only used in systems that enable Route-Through
     */
    UINT32 vn0homqpi : 2;
    /* vn0homqpi - Bits[7:6], RWS, default = 2'b01 
       Q2Q HOM Vn0 credit. Only used in systems that enable Route-Through
     */
    UINT32 vn0ndrqpi : 2;
    /* vn0ndrqpi - Bits[9:8], RWS, default = 2'b01 
       Q2Q NDR Vn0 credit. Only used in systems that enable Route-Through
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 blvnaqpi : 4;
    /* blvnaqpi - Bits[19:16], RWS, default = 4'b0000 
       Q2Q BL VNA credit. Only used in systems that enable Route-Through
     */
    UINT32 vn0drsqpi : 2;
    /* vn0drsqpi - Bits[21:20], RWS, default = 2'b01 
       Q2Q DRS Vn0 Credit. Only used in systems that enable Route-Through
     */
    UINT32 vn0ncsqpi : 2;
    /* vn0ncsqpi - Bits[23:22], RWS, default = 2'b01 
       Q2Q NCS VN0 credit. Only used in systems that enable Route-Through
     */
    UINT32 vn0ncbqpi : 2;
    /* vn0ncbqpi - Bits[25:24], RWS, default = 2'b01 
       Q2Q NCB VN0 Credit. Only used in systems that enable Route-Through
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R3QRTE0CR_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* R3QRTE1CR_R3QPI_CFG_REG supported on:                                      */
/*       IVT_EP (0x4019C048)                                                  */
/*       IVT_EX (0x4019C048)                                                  */
/*       HSX (0x40158048)                                                     */
/*       BDX (0x40158048)                                                     */
/* Register default value:              0x01500150                            */
#define R3QRTE1CR_R3QPI_CFG_REG 0x05004048
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI RouteThrough Egress Credit Port1 
 */
typedef union {
  struct {
    UINT32 advnaqpi : 4;
    /* advnaqpi - Bits[3:0], RWS, default = 4'b0000 
       Q2Q AD VNA credit. Only used in systems that enable Route-Through
     */
    UINT32 vn0snpqpi : 2;
    /* vn0snpqpi - Bits[5:4], RWS, default = 2'b01 
       Q2Q SNP VN0 credit. Only used in systems that enable Route-Through
     */
    UINT32 vn0homqpi : 2;
    /* vn0homqpi - Bits[7:6], RWS, default = 2'b01 
       Q2Q HOM VN0 Credit. Only used in systems that enable Route-Through
     */
    UINT32 vn0ndrqpi : 2;
    /* vn0ndrqpi - Bits[9:8], RWS, default = 2'b01 
       Q2Q NDR VN0 Credit. Only used in systems that enable Route-Through
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 blvnaqpi : 4;
    /* blvnaqpi - Bits[19:16], RWS, default = 4'b0000 
       Q2Q BL VNA Credit. Only used in systems that enable Route-Through
     */
    UINT32 vn0drsqpi : 2;
    /* vn0drsqpi - Bits[21:20], RWS, default = 2'b01 
       Q2Q DRS VN0 Credit. Only used in systems that enable Route-Through
     */
    UINT32 vn0ncsqpi : 2;
    /* vn0ncsqpi - Bits[23:22], RWS, default = 2'b01 
       Q2Q NCS VN0 Credit. Only used in systems that enable Route-Through
     */
    UINT32 vn0ncbqpi : 2;
    /* vn0ncbqpi - Bits[25:24], RWS, default = 2'b01 
       Q2Q NCB VN0 Credit. Only used in systems that enable Route-Through
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R3QRTE1CR_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* R3QCBOHACR_R3QPI_CFG_REG supported on:                                     */
/*       IVT_EP (0x4019C04C)                                                  */
/*       IVT_EX (0x4019C04C)                                                  */
/*       HSX (0x4015804C)                                                     */
/*       BDX (0x4015804C)                                                     */
/* Register default value on IVT_EP:    0x00124444                            */
/* Register default value on IVT_EX:    0x00124444                            */
/* Register default value on HSX:       0x22008888                            */
/* Register default value on BDX:       0x22008888                            */
#define R3QCBOHACR_R3QPI_CFG_REG 0x0500404C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.0.CFG.xml.
 * Credit configuration between the R3QPI and the Cbo's + HA
 */
typedef union {
  struct {
    UINT32 ha0creditr0 : 4;
    /* ha0creditr0 - Bits[3:0], RWS, default = 4'b1000 
       Credits for Port0 to HA0 and HA2
     */
    UINT32 ha1creditr0 : 4;
    /* ha1creditr0 - Bits[7:4], RWS, default = 4'b1000 
       Credits for Port0 to HA1 and HA3
     */
    UINT32 ha0creditr1 : 4;
    /* ha0creditr1 - Bits[11:8], RWS, default = 4'b1000 
       Credits for Port1 to HA0 and HA2
     */
    UINT32 ha1creditr1 : 4;
    /* ha1creditr1 - Bits[15:12], RWS, default = 4'b1000 
       Credits for Port1 to HA1 and HA3
     */
    UINT32 hacrediten : 1;
    /* hacrediten - Bits[16:16], RWS, default = 1'b0 
       Dynamic HT mode is on. Q2H DRS is no longer always-sink. It is credited.
     */
    UINT32 rsvd : 7;
    /* rsvd - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 numsnpcreditsl0 : 4;
    /* numsnpcreditsl0 - Bits[27:24], RWS, default = 4'b0010 
       SNP Credits from Port0 on this R3QPI to each CBo
     */
    UINT32 numsnpcreditsl1 : 4;
    /* numsnpcreditsl1 - Bits[31:28], RWS, default = 4'b0010 
       SNP Credits from Port1 on this R3QPI to each CBo
     */
  } Bits;
  UINT32 Data;
} R3QCBOHACR_R3QPI_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* R3QE0CR_R3QPI_CFG_REG supported on:                                        */
/*       IVT_EP (0x4019C050)                                                  */
/*       IVT_EX (0x4019C050)                                                  */
/*       HSX (0x40158050)                                                     */
/*       BDX (0x40158050)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00011CE3                            */
/* Register default value on BDX:       0x00011CE3                            */
#define R3QE0CR_R3QPI_CFG_REG 0x05004050


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.0.CFG.xml.
 * R3QPI Egress Control Port 0.
 */
typedef union {
  struct {
    UINT32 akegresscreditreturn : 3;
    /* akegresscreditreturn - Bits[2:0], RWS, default = 3'b011 
       AK EGRQ has 8 entries. It is hard-partitioning into NDR and credit Return. This 
       field specifies how many entries are used for credit return. The number of NDR 
       entries is defined in another QPI CSR. However, the sum of those two should be 
       less than or equal to 8  
     */
    UINT32 rsvd_3 : 1;
    /* rsvd_3 - Bits[3:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_4 : 28;
  } Bits;
  UINT32 Data;
} R3QE0CR_R3QPI_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* R3QE1CR_R3QPI_CFG_REG supported on:                                        */
/*       IVT_EP (0x4019C054)                                                  */
/*       IVT_EX (0x4019C054)                                                  */
/*       HSX (0x40158054)                                                     */
/*       BDX (0x40158054)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00011CE3                            */
/* Register default value on BDX:       0x00011CE3                            */
#define R3QE1CR_R3QPI_CFG_REG 0x05004054


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.0.CFG.xml.
 * R3QPI Egress Control Port 1.
 */
typedef union {
  struct {
    UINT32 akegresscreditreturn : 3;
    /* akegresscreditreturn - Bits[2:0], RWS, default = 3'b011 
       AK EGRQ has 8 entries. It is hard-partitioning into NDR and credit Return. This 
       field specifies how many entries are used for credit return. The number of NDR 
       entries is defined in another QPI CSR. However, the sum of those two should be 
       less than or equal to 8  
     */
    UINT32 rsvd_3 : 1;
    /* rsvd_3 - Bits[3:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_4 : 28;
  } Bits;
  UINT32 Data;
} R3QE1CR_R3QPI_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* R3QPCIR0CR_R3QPI_CFG_REG supported on:                                     */
/*       IVT_EP (0x4019C058)                                                  */
/*       IVT_EX (0x4019C058)                                                  */
/*       HSX (0x40158058)                                                     */
/*       BDX (0x40158058)                                                     */
/* Register default value:              0x00000110                            */
#define R3QPCIR0CR_R3QPI_CFG_REG 0x05004058
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI PCI Ring Credit Port 0. Register defines credits allocated for R3QPI to 
 * send messages to R2PCI ring stop. 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 4;
    /* rsvd_0 - Bits[3:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pcincscredit : 3;
    /* pcincscredit - Bits[6:4], RWS, default = 3'b001 
       Q2I NCS Credits. This is the credits allocated in R2 Ingress for R3QPI
     */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pcincbcredit : 3;
    /* pcincbcredit - Bits[10:8], RWS, default = 3'b001 
       Q2I NCB Credits. This is the credits allocated in R2 Ingress for R3QPI
     */
    UINT32 rsvd_11 : 21;
    /* rsvd_11 - Bits[31:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R3QPCIR0CR_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* R3QPCIR1CR_R3QPI_CFG_REG supported on:                                     */
/*       IVT_EP (0x4019C05C)                                                  */
/*       IVT_EX (0x4019C05C)                                                  */
/*       HSX (0x4015805C)                                                     */
/*       BDX (0x4015805C)                                                     */
/* Register default value:              0x00000110                            */
#define R3QPCIR1CR_R3QPI_CFG_REG 0x0500405C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI PCI Ring Credit Port 1. Register defines credits allocated for R3QPI to 
 * send messages to R2PCI ring stop. 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 4;
    /* rsvd_0 - Bits[3:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pcincscredit : 3;
    /* pcincscredit - Bits[6:4], RWS, default = 3'b001 
       Q2I NCS Credits. This is the credits allocated in R2 Ingress for R3QPI
     */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pcincbcredit : 3;
    /* pcincbcredit - Bits[10:8], RWS, default = 3'b001 
       Q2I NCB Credits. This is the credits allocated in R2 Ingress for R3QPI
     */
    UINT32 rsvd_11 : 21;
    /* rsvd_11 - Bits[31:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R3QPCIR1CR_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* R3QCTRL_R3QPI_CFG_REG supported on:                                        */
/*       IVT_EP (0x4019C060)                                                  */
/*       IVT_EX (0x4019C060)                                                  */
/*       HSX (0x40158060)                                                     */
/*       BDX (0x40158060)                                                     */
/* Register default value on IVT_EP:    0x00000000                            */
/* Register default value on IVT_EX:    0x00000000                            */
/* Register default value on HSX:       0x00000180                            */
/* Register default value on BDX:       0x00000180                            */
#define R3QCTRL_R3QPI_CFG_REG 0x05004060


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.0.CFG.xml.
 * R3QPI Control Register 
 */
typedef union {
  struct {
    UINT32 extendedrtidmode : 1;
    /* extendedrtidmode - Bits[0:0], RWS, default = 1'b0 
       Unlike previous processors, the processor will never use Rtid64 in either 
       xRtid=0 or xRtid=1. Hence this bit is redundant.  
     */
    UINT32 snoopfanoutenable : 1;
    /* snoopfanoutenable - Bits[1:1], RWS, default = 1'b0 
       Enable fanout snoop
     */
    UINT32 socketid : 3;
    /* socketid - Bits[4:2], RWS, default = 3'b000 
       SocketID[2:0]
     */
    UINT32 vn1enable : 1;
    /* vn1enable - Bits[5:5], RWS, default = 1'b0 
       Enable VN1 (EX processor only)
     */
    UINT32 codenable : 1;
    /* codenable - Bits[6:6], RWS, default = 1'b0 
       Enable COD (EP processor only, select SKU)
     */
    UINT32 flowq_vn1enable : 1;
    /* flowq_vn1enable - Bits[7:7], RWS, default = 1'b1 
       Additional VN1 enable for FlowQ. The combined VN1 enable input to FlowQ equals 
       to (FlowQ_VN1Enable | VN1Enable).  
        If system is in vn0-only mode, by setting FlowQ_VN1Enable, flowQ could still 
       work under vn0+vn1 mode which avoids a flowQ bug under vn0-only mode 
     */
    UINT32 rsvd_8 : 1;
    UINT32 rsvd_9 : 15;
    /* rsvd_9 - Bits[23:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} R3QCTRL_R3QPI_CFG_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI Control Register 
 */
typedef union {
  struct {
    UINT32 extendedrtidmode : 1;
    /* extendedrtidmode - Bits[0:0], RWS, default = 1'b0 
       Unlike previous processors, the processor will never use Rtid64 in either 
       xRtid=0 or xRtid=1. Hence this bit is redundant.  
     */
    UINT32 snoopfanoutenable : 1;
    /* snoopfanoutenable - Bits[1:1], RWS, default = 1'b0 
       Enable fanout snoop
     */
    UINT32 socketid : 3;
    /* socketid - Bits[4:2], RWS, default = 3'b000 
       SocketID[2:0]
     */
    UINT32 vn1enable : 1;
    /* vn1enable - Bits[5:5], RWS, default = 1'b0 
       Enable VN1 (EX processor only)
     */
    UINT32 codenable : 1;
    /* codenable - Bits[6:6], RWS, default = 1'b0 
       Enable COD (EP processor only, select SKU)
     */
    UINT32 flowq_vn1enable : 1;
    /* flowq_vn1enable - Bits[7:7], RWS, default = 1'b1 
       Additional VN1 enable for FlowQ. The combined VN1 enable input to FlowQ equals 
       to (FlowQ_VN1Enable | VN1Enable).  
        If system is in vn0-only mode, by setting FlowQ_VN1Enable, flowQ could still 
       work under vn0+vn1 mode which avoids a flowQ bug under vn0-only mode 
     */
    UINT32 akstarvfixenable : 1;
    /* akstarvfixenable - Bits[8:8], RWS, default = 1'b1 
       Chicken bit for removing ArbRetryOK from Arblost equation in AK
     */
    UINT32 config_4s : 1;
    /* config_4s - Bits[9:9], RWS, default = 1'b0 
       
               Identifies 4S configuration.
       	0 = Non-4S config
       	1 = 4S config
             
     */
    UINT32 rsvd_10 : 14;
    /* rsvd_10 - Bits[23:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 port0clkdisable : 1;
    /* port0clkdisable - Bits[24:24], RWS, default = 1'b0 
       IGR Port 0 Clock Disable
     */
    UINT32 port1clkdisable : 1;
    /* port1clkdisable - Bits[25:25], RWS, default = 1'b0 
       IGR Port 1 Clock Disable for Ingress
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R3QCTRL_R3QPI_CFG_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* R3SNPFANOUT_P0_R3QPI_CFG_REG supported on:                                 */
/*       IVT_EP (0x4019C064)                                                  */
/*       IVT_EX (0x4019C064)                                                  */
/*       HSX (0x40158064)                                                     */
/*       BDX (0x40158064)                                                     */
/* Register default value:              0x00000000                            */
#define R3SNPFANOUT_P0_R3QPI_CFG_REG 0x05004064
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * Snoop Fanout Table for Port0 on this R3QPI. There are three bits for each 
 * scoket.  
 *                  bit[2]=1 corresponds fanout to cross-ring agent1, 
 *                  bit[1]=1 corresponds fanout to cross-ring agent0 and 
 *                  bit[0]=1 corresponds fanout to colocated port 
 */
typedef union {
  struct {
    UINT32 fanouttableskt0 : 3;
    /* fanouttableskt0 - Bits[2:0], RWS, default = 3'b000 
       Snoop Fanout Table for Socket ID 0
     */
    UINT32 fanouttableskt1 : 3;
    /* fanouttableskt1 - Bits[5:3], RWS, default = 3'b000 
       Snoop Fanout Table for Socket ID 1
     */
    UINT32 fanouttableskt2 : 3;
    /* fanouttableskt2 - Bits[8:6], RWS, default = 3'b000 
       Snoop Fanout Table for Socket ID 2
     */
    UINT32 fanouttableskt3 : 3;
    /* fanouttableskt3 - Bits[11:9], RWS, default = 3'b000 
       Snoop Fanout Table for Socket ID 3
     */
    UINT32 fanouttableskt4 : 3;
    /* fanouttableskt4 - Bits[14:12], RWS, default = 3'b000 
       Snoop Fanout Table for Socket ID 4
     */
    UINT32 fanouttableskt5 : 3;
    /* fanouttableskt5 - Bits[17:15], RWS, default = 3'b000 
       Snoop Fanout Table for Socket ID 5
     */
    UINT32 fanouttableskt6 : 3;
    /* fanouttableskt6 - Bits[20:18], RWS, default = 3'b000 
       Snoop Fanout Table for Socket ID 6
     */
    UINT32 fanouttableskt7 : 3;
    /* fanouttableskt7 - Bits[23:21], RWS, default = 3'b000 
       Snoop Fanout Table for Socket ID 7
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R3SNPFANOUT_P0_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* R3SNPFANOUT_P1_R3QPI_CFG_REG supported on:                                 */
/*       IVT_EP (0x4019C068)                                                  */
/*       IVT_EX (0x4019C068)                                                  */
/*       HSX (0x40158068)                                                     */
/*       BDX (0x40158068)                                                     */
/* Register default value:              0x00000000                            */
#define R3SNPFANOUT_P1_R3QPI_CFG_REG 0x05004068
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * Snoop Fanout Table for Port1 on this R3QPI. There are three bits for each 
 * scoket.  
 *                  bit[2]=1 corresponds fanout to cross-ring agent1, 
 *                  bit[1]=1 corresponds fanout to cross-ring agent0 and 
 *                  bit[0]=1 corresponds fanout to colocated port 
 */
typedef union {
  struct {
    UINT32 fanouttableskt0 : 3;
    /* fanouttableskt0 - Bits[2:0], RWS, default = 3'b000 
       Snoop Fanout Table for Socket ID 0
     */
    UINT32 fanouttableskt1 : 3;
    /* fanouttableskt1 - Bits[5:3], RWS, default = 3'b000 
       Snoop Fanout Table for Socket ID 1
     */
    UINT32 fanouttableskt2 : 3;
    /* fanouttableskt2 - Bits[8:6], RWS, default = 3'b000 
       Snoop Fanout Table for Socket ID 2
     */
    UINT32 fanouttableskt3 : 3;
    /* fanouttableskt3 - Bits[11:9], RWS, default = 3'b000 
       Snoop Fanout Table for Socket ID 3
     */
    UINT32 fanouttableskt4 : 3;
    /* fanouttableskt4 - Bits[14:12], RWS, default = 3'b000 
       Snoop Fanout Table for Socket ID 4
     */
    UINT32 fanouttableskt5 : 3;
    /* fanouttableskt5 - Bits[17:15], RWS, default = 3'b000 
       Snoop Fanout Table for Socket ID 5
     */
    UINT32 fanouttableskt6 : 3;
    /* fanouttableskt6 - Bits[20:18], RWS, default = 3'b000 
       Snoop Fanout Table for Socket ID 6
     */
    UINT32 fanouttableskt7 : 3;
    /* fanouttableskt7 - Bits[23:21], RWS, default = 3'b000 
       Snoop Fanout Table for Socket ID 7
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R3SNPFANOUT_P1_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* R3QTXTRH_R3QPI_CFG_REG supported on:                                       */
/*       IVT_EP (0x4019C06C)                                                  */
/*       IVT_EX (0x4019C06C)                                                  */
/*       HSX (0x4015806C)                                                     */
/*       BDX (0x4015806C)                                                     */
/* Register default value:              0x00000630                            */
#define R3QTXTRH_R3QPI_CFG_REG 0x0500406C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI Tx Threshold Register 
 */
typedef union {
  struct {
    UINT32 l0threshold : 6;
    /* l0threshold - Bits[5:0], RWS, default = 6'b110000 
       Threshold value used when in L0 state
     */
    UINT32 rsvd_6 : 2;
    /* rsvd_6 - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 l0pthreshold : 6;
    /* l0pthreshold - Bits[13:8], RWS, default = 6'b000110 
       Threshold value used when in L0p state
     */
    UINT32 rsvd_14 : 18;
    /* rsvd_14 - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R3QTXTRH_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* R3QRRC_R3QPI_CFG_REG supported on:                                         */
/*       IVT_EP (0x4019C070)                                                  */
/*       IVT_EX (0x4019C070)                                                  */
/*       HSX (0x40158070)                                                     */
/*       BDX (0x40158070)                                                     */
/* Register default value:              0x00000000                            */
#define R3QRRC_R3QPI_CFG_REG 0x05004070
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI Ingress RR Control Register 
 */
typedef union {
  struct {
    UINT32 txadweight : 6;
    /* txadweight - Bits[5:0], RW, default = 6'b000000 
       Weight given in RR arb to the AD ring
     */
    UINT32 txblweight : 6;
    /* txblweight - Bits[11:6], RW, default = 6'b000000 
       Weight given in RR arb to the BL ring
     */
    UINT32 rsvd : 20;
    /* rsvd - Bits[31:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R3QRRC_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* R3GLERRCFG_R3QPI_CFG_REG supported on:                                     */
/*       HSX (0x40158074)                                                     */
/*       BDX (0x40158074)                                                     */
/* Register default value:              0x0003E660                            */
#define R3GLERRCFG_R3QPI_CFG_REG 0x05004074

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI global viral/fatal error configuration register.  
 *     Note viral is only supported on the EX processor. 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 resetglobalfatalerror : 1;
    /* resetglobalfatalerror - Bits[3:3], RW, default = 1'b0 
       Set 1 to force clear global fatal error status.
     */
    UINT32 resetglobalviral : 1;
    /* resetglobalviral - Bits[4:4], RW, default = 1'b0 
       Set 1 to force clear global viral status.
     */
    UINT32 maskr3fatalerror : 4;
    /* maskr3fatalerror - Bits[8:5], RWS, default = 4'b0011 
       Set 1 to block fatal error status of R3QPI to global fatal status
     */
    UINT32 maskqpifatalerrorin : 2;
    /* maskqpifatalerrorin - Bits[10:9], RWS, default = 2'b11 
       Set 1 to block fatal error status from Intel QPI to global fatal status
     */
    UINT32 rsvd_11 : 2;
    /* rsvd_11 - Bits[12:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 maskqpiviralout : 2;
    /* maskqpiviralout - Bits[14:13], RWS, default = 2'b11 
       Set 1 to block viral status from global viral status to Intel QPI
     */
    UINT32 maskqpiviralin : 2;
    /* maskqpiviralin - Bits[16:15], RWS, default = 2'b11 
       Set 1 to block viral status from Intel QPI to global viral status
     */
    UINT32 maskpcufatalerrorout : 1;
    /* maskpcufatalerrorout - Bits[17:17], RWS, default = 1'b1 
       Set 1 to block fatal to PCU
     */
    UINT32 localerrorstatus : 1;
    /* localerrorstatus - Bits[18:18], RO, default = 1'b0 
       Read only R3QPI error status
     */
    UINT32 viralstatustoqpi0 : 1;
    /* viralstatustoqpi0 - Bits[19:19], RO, default = 1'b0 
       Read only to QPI0 viral status
     */
    UINT32 viralstatustoqpi1 : 1;
    /* viralstatustoqpi1 - Bits[20:20], RO, default = 1'b0 
       Read only to QPI1 viral status
     */
    UINT32 viralstatusfromqpi0 : 1;
    /* viralstatusfromqpi0 - Bits[21:21], RO, default = 1'b0 
       Read only from QPI0 viral status
     */
    UINT32 viralstatusfromqpi1 : 1;
    /* viralstatusfromqpi1 - Bits[22:22], RO, default = 1'b0 
       Read only from QPI1 viral status
     */
    UINT32 fatalstatusfromqpi0 : 1;
    /* fatalstatusfromqpi0 - Bits[23:23], RO, default = 1'b0 
       Read only from QPI0 fatal status
     */
    UINT32 fatalstatusfromqpi1 : 1;
    /* fatalstatusfromqpi1 - Bits[24:24], RO, default = 1'b0 
       Read only from QPI1 fatal status
     */
    UINT32 fatalstatustopcu : 1;
    /* fatalstatustopcu - Bits[25:25], RO, default = 1'b0 
       Read only to PCU fatal status
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R3GLERRCFG_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* R3QIGRVNSEL0_R3QPI_CFG_REG supported on:                                   */
/*       IVT_EP (0x4019C078)                                                  */
/*       IVT_EX (0x4019C078)                                                  */
/*       HSX (0x40158078)                                                     */
/*       BDX (0x40158078)                                                     */
/* Register default value:              0x00000000                            */
#define R3QIGRVNSEL0_R3QPI_CFG_REG 0x05004078
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI Ingress VN Switch Table for Port0
 */
typedef union {
  struct {
    UINT32 igrvnswtchtable0 : 32;
    /* igrvnswtchtable0 - Bits[31:0], RWS, default = 32'h00000000 
       The usage of VN mapping:
       	           1) Orignal sender:           VN = SocketID[0]
       	           2) IGR and EGR between hops: VN = Same as orignal sender
       	           3) Destination Egr:          VN = VN0  
             
     */
  } Bits;
  UINT32 Data;
} R3QIGRVNSEL0_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* R3QIGRVNSEL1_R3QPI_CFG_REG supported on:                                   */
/*       IVT_EP (0x4019C07C)                                                  */
/*       IVT_EX (0x4019C07C)                                                  */
/*       HSX (0x4015807C)                                                     */
/*       BDX (0x4015807C)                                                     */
/* Register default value:              0x00000000                            */
#define R3QIGRVNSEL1_R3QPI_CFG_REG 0x0500407C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI Ingress VN Switch Table for Port1
 */
typedef union {
  struct {
    UINT32 igrvnswtchtable1 : 32;
    /* igrvnswtchtable1 - Bits[31:0], RWS, default = 32'h00000000 
       The usage of VN mapping:
       	           1) Orignal sender:           VN = SocketID[0]
       	           2) IGR and EGR between hops: VN = Same as orignal sender
       	           3) Destination Egr:          VN = VN0  
             
     */
  } Bits;
  UINT32 Data;
} R3QIGRVNSEL1_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* NUMOFHOPSCR_P0_R3QPI_CFG_REG supported on:                                 */
/*       HSX (0x40158080)                                                     */
/*       BDX (0x40158080)                                                     */
/* Register default value:              0x00000000                            */
#define NUMOFHOPSCR_P0_R3QPI_CFG_REG 0x05004080

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * Number of hops Core responses use. This field is used for D2C response encode 
 * calculation. Because colocated ports never use the same routing paths, Two ports 
 * can shared a Register. 
 */
typedef union {
  struct {
    UINT32 hops0 : 16;
    /* hops0 - Bits[15:0], RW, default = 16'h0000 
       HOPS 0, NodeID based table, 2bit for each socket
     */
    UINT32 hops1 : 16;
    /* hops1 - Bits[31:16], RW, default = 16'h0000 
       HOPS 1, Sid based table, 2bit for each socket
     */
  } Bits;
  UINT32 Data;
} NUMOFHOPSCR_P0_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* R3INGERRMASK2_R3QPI_CFG_REG supported on:                                  */
/*       HSX (0x40158084)                                                     */
/*       BDX (0x40158084)                                                     */
/* Register default value:              0x000000C0                            */
#define R3INGERRMASK2_R3QPI_CFG_REG 0x05004084

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3 Ingress Error Mask Register2
 */
typedef union {
  struct {
    UINT32 csi0parerringmask : 1;
    /* csi0parerringmask - Bits[0:0], RWS, default = 1'b0 
       Port0 Parity Error Mask
     */
    UINT32 csi1parerringmask : 1;
    /* csi1parerringmask - Bits[1:1], RWS, default = 1'b0 
       Port1 Ingress Parity Error Mask
     */
    UINT32 csi0parerregrmask : 2;
    /* csi0parerregrmask - Bits[3:2], RWS, default = 2'b00 
       Port0 Egress Parity Error Mask
     */
    UINT32 csi1parerregrmask : 2;
    /* csi1parerregrmask - Bits[5:4], RWS, default = 2'b00 
       Port1 Egress Parity Error Mask
     */
    UINT32 csi0parerre2emask : 1;
    /* csi0parerre2emask - Bits[6:6], RWS, default = 1'b1 
       Port0 End to End Parity Error Mask
     */
    UINT32 csi1parerre2emask : 1;
    /* csi1parerre2emask - Bits[7:7], RWS, default = 1'b1 
       Port1 End to End Parity Error Mask
     */
    UINT32 rsvd : 24;
    /* rsvd - Bits[31:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R3INGERRMASK2_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* R3EGRCTRL_R3QPI_CFG_REG supported on:                                      */
/*       IVT_EP (0x4019C088)                                                  */
/*       IVT_EX (0x4019C088)                                                  */
/*       HSX (0x40158088)                                                     */
/*       BDX (0x40158088)                                                     */
/* Register default value on IVT_EP:    0x000086BD                            */
/* Register default value on IVT_EX:    0x000086BD                            */
/* Register default value on HSX:       0x001896BD                            */
/* Register default value on BDX:       0x005896BD                            */
#define R3EGRCTRL_R3QPI_CFG_REG 0x05004088


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.0.CFG.xml.
 * R3QPI Egress Control
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 7;
    UINT32 homsnpspawnenable : 1;
    /* homsnpspawnenable - Bits[7:7], RW, default = 1'b1 
       When set, allows the R3 egress to generate an additional snoop message directly 
       to the target Cbo (on the local socket) upon receipt of a HOM message targeting 
       the local socket.  Note that this optimization may not always occur, depending 
       on the availability of queuing resources. 
     */
    UINT32 rsvd_8 : 9;
    UINT32 pe_mode : 2;
    /* pe_mode - Bits[18:17], RW, default = 2'b00 
         
        This modifies the DNID[4:3] field of a Tx message that comes from AD/BL Ring 
       when isoch is enabled.   
       	  If (pe_mode[1] == 1) QPI_DNID[4]=Ring_PE[1] else QPI_DNID[4]=0.   
       	  If (pe_mode[0] == 1) QPI_DNID[3]=Ring_PE[0] else QPI_DNID[3]=Ring_DNID[3].  
               Similarly, This re-maps the DNID[4:3] field of a Rx message into PE[1:0] 
       and DNID[3] fields of a Ring message. 
       	  If (pe_mode[1] == 1) Ring_PE[1]=QPI_DNID[4] else Ring_PE[1]=0.   
       	  If (pe_mode[0] == 1) Ring_PE[0]=QPI_DNID[3] else Ring_DNID[3]=QPI_DNID[3].  
               The usage of PE_Mode[1:0] is to take the value of either 2'b00 or 2'b11. 
        
             
     */
    UINT32 advnacreditreturncoalescingenable : 1;
    /* advnacreditreturncoalescingenable - Bits[19:19], RW, default = 1'b1 
         
              enable AD vna credit return coalescing on AK ring for performance. MAX AD 
       coalescing value equals to 7. This bit[19] and bit[20] should set or cleared 
       together due to a bug   
             
     */
    UINT32 blvnacreditreturncoalescingenable : 1;
    /* blvnacreditreturncoalescingenable - Bits[20:20], RW, default = 1'b1 
         
              enable BL vna credit return coalescing on AK ring for performance. Max BL 
       coalescing value equals to 7. This bit[19] and bit[20] shuld be set or cleared 
       together due to a bug 
             
     */
    UINT32 rsvd_21 : 11;
    /* rsvd_21 - Bits[31:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R3EGRCTRL_R3QPI_CFG_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI Egress Control
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 7;
    UINT32 homsnpspawnenable : 1;
    /* homsnpspawnenable - Bits[7:7], RW, default = 1'b1 
       When set, allows the R3 egress to generate an additional snoop message directly 
       to the target Cbo (on the local socket) upon receipt of a HOM message targeting 
       the local socket.  Note that this optimization may not always occur, depending 
       on the availability of queuing resources. 
     */
    UINT32 rsvd_8 : 9;
    UINT32 pe_mode : 2;
    /* pe_mode - Bits[18:17], RW, default = 2'b00 
         
        This modifies the DNID[4:3] field of a Tx message that comes from AD/BL Ring 
       when isoch is enabled.   
       	  If (pe_mode[1] == 1) QPI_DNID[4]=Ring_PE[1] else QPI_DNID[4]=0.   
       	  If (pe_mode[0] == 1) QPI_DNID[3]=Ring_PE[0] else QPI_DNID[3]=Ring_DNID[3].  
               Similarly, This re-maps the DNID[4:3] field of a Rx message into PE[1:0] 
       and DNID[3] fields of a Ring message. 
       	  If (pe_mode[1] == 1) Ring_PE[1]=QPI_DNID[4] else Ring_PE[1]=0.   
       	  If (pe_mode[0] == 1) Ring_PE[0]=QPI_DNID[3] else Ring_DNID[3]=QPI_DNID[3].  
               The usage of PE_Mode[1:0] is to take the value of either 2'b00 or 2'b11. 
        
             
     */
    UINT32 advnacreditreturncoalescingenable : 1;
    /* advnacreditreturncoalescingenable - Bits[19:19], RW, default = 1'b1 
         
              enable AD vna credit return coalescing on AK ring for performance. MAX AD 
       coalescing value equals to 7. This bit[19] and bit[20] should set or cleared 
       together due to a bug   
             
     */
    UINT32 blvnacreditreturncoalescingenable : 1;
    /* blvnacreditreturncoalescingenable - Bits[20:20], RW, default = 1'b1 
         
              enable BL vna credit return coalescing on AK ring for performance. Max BL 
       coalescing value equals to 7. This bit[19] and bit[20] shuld be set or cleared 
       together due to a bug 
             
     */
    UINT32 iodc_mode : 1;
    /* iodc_mode - Bits[21:21], RW, default = 1'b0 
         
       	This allows PE[1] to pass through.  
       	If (iodc_mode == 0)  PE[1]'s fate is dedcided by pe_mode[1] 
        If (iodc_mode == 1)  Allows PE[1] to pass through despite of the value of 
       pe_mode[1] 
             
     */
    UINT32 hom1flowqperffixenable : 1;
    /* hom1flowqperffixenable - Bits[22:22], RW, default = 1'b1 
         
              Enable hom1 message being put on the fast flowq path under non early 
       snoop mode for performance.   HSD 272397 
             
     */
    UINT32 rsvd_23 : 9;
    /* rsvd_23 - Bits[31:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R3EGRCTRL_R3QPI_CFG_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* R3QEGR1VNSEL_R3QPI_CFG_REG supported on:                                   */
/*       IVT_EP (0x4019C08C)                                                  */
/*       IVT_EX (0x4019C08C)                                                  */
/*       HSX (0x4015808C)                                                     */
/*       BDX (0x4015808C)                                                     */
/* Register default value:              0x00000000                            */
#define R3QEGR1VNSEL_R3QPI_CFG_REG 0x0500408C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * VN Selection table for switching between VNs in Egress port 0 and 1
 */
typedef union {
  struct {
    UINT32 egrvnswtchtable0 : 16;
    /* egrvnswtchtable0 - Bits[15:0], RWS, default = 16'h0000 
       Intel QPI Port0 VN to Ring VN mapping table
     */
    UINT32 egrvnswtchtable1 : 16;
    /* egrvnswtchtable1 - Bits[31:16], RWS, default = 16'h0000 
       Intel QPI Port1 VN to Ring VN mapping table
     */
  } Bits;
  UINT32 Data;
} R3QEGR1VNSEL_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* R3QEGRDBG_R3QPI_CFG_REG supported on:                                      */
/*       IVT_EP (0x4019C090)                                                  */
/*       IVT_EX (0x4019C090)                                                  */
/*       HSX (0x40158090)                                                     */
/*       BDX (0x40158090)                                                     */
/* Register default value:              0x00000000                            */
#define R3QEGRDBG_R3QPI_CFG_REG 0x05004090


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.0.CFG.xml.
 * Egress Debug
 */
typedef union {
  struct {
    UINT32 dbgbuspresel0 : 5;
    /* dbgbuspresel0 - Bits[4:0], RW_L, default = 5'b00000 
       Port0 debug bus pre-select
     */
    UINT32 dbgbuspresel1 : 5;
    /* dbgbuspresel1 - Bits[9:5], RW_L, default = 5'b00000 
       Port1 debug bus pre-select
     */
    UINT32 dbgbusen0 : 1;
    /* dbgbusen0 - Bits[10:10], RW_L, default = 1'b0 
       Port0 debug bus enable
     */
    UINT32 dbgbusen1 : 1;
    /* dbgbusen1 - Bits[11:11], RW_L, default = 1'b0 
       Port1 debug bus enable
     */
    UINT32 rspthrotad0 : 1;
    /* rspthrotad0 - Bits[12:12], RW_L, default = 1'b0 
       Port0 throttle AD
     */
    UINT32 rspthrotad1 : 1;
    /* rspthrotad1 - Bits[13:13], RW_L, default = 1'b0 
       Port1 throttle AD
     */
    UINT32 rspthrotbl0 : 1;
    /* rspthrotbl0 - Bits[14:14], RW_L, default = 1'b0 
       Port0 throttle BL
     */
    UINT32 rspthrotbl1 : 1;
    /* rspthrotbl1 - Bits[15:15], RW_L, default = 1'b0 
       Port1 throttle BL
     */
    UINT32 rspthrotak0 : 1;
    /* rspthrotak0 - Bits[16:16], RW_L, default = 1'b0 
       Port0 throttle AK
     */
    UINT32 rspthrotak1 : 1;
    /* rspthrotak1 - Bits[17:17], RW_L, default = 1'b0 
       Port1 throttle AK
     */
    UINT32 rspthrotcrd0 : 1;
    /* rspthrotcrd0 - Bits[18:18], RW_L, default = 1'b0 
       Port0 throttle credit
     */
    UINT32 rspthrotcrd1 : 1;
    /* rspthrotcrd1 - Bits[19:19], RW_L, default = 1'b0 
       Port1 throttle credit
     */
    UINT32 rspthrotup0 : 1;
    /* rspthrotup0 - Bits[20:20], RW_L, default = 1'b0 
       Port0 throttle Up direction
     */
    UINT32 rspthrotup1 : 1;
    /* rspthrotup1 - Bits[21:21], RW_L, default = 1'b0 
       Port1 throttle Up direction
     */
    UINT32 rspthrotdn0 : 1;
    /* rspthrotdn0 - Bits[22:22], RW_L, default = 1'b0 
       Port0 throttle Down direction
     */
    UINT32 rspthrotdn1 : 1;
    /* rspthrotdn1 - Bits[23:23], RW_L, default = 1'b0 
       Port1 throttle Down direction
     */
    UINT32 rspfuncsel0 : 1;
    /* rspfuncsel0 - Bits[24:24], RW_L, default = 1'b0 
       Port0 function select
     */
    UINT32 rspfuncsel1 : 1;
    /* rspfuncsel1 - Bits[25:25], RW_L, default = 1'b0 
       Port1 function select
     */
    UINT32 rsvd : 6;
    /* rsvd - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R3QEGRDBG_R3QPI_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* R3INGERRLOG2_R3QPI_CFG_REG supported on:                                   */
/*       HSX (0x40158094)                                                     */
/*       BDX (0x40158094)                                                     */
/* Register default value:              0x00000000                            */
#define R3INGERRLOG2_R3QPI_CFG_REG 0x05004094

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI Ingress Error Log Register2
 */
typedef union {
  struct {
    UINT32 csi0parerring : 1;
    /* csi0parerring - Bits[0:0], RW1CS, default = 1'b0 
       Port0 Ingress Parity Error
     */
    UINT32 csi1parerring : 1;
    /* csi1parerring - Bits[1:1], RW1CS, default = 1'b0 
       Port1 Ingress Parity Error
     */
    UINT32 csi0parerregr : 2;
    /* csi0parerregr - Bits[3:2], RW1CS, default = 2'b00 
       Port0 Egress Parity Error
     */
    UINT32 csi1parerregr : 2;
    /* csi1parerregr - Bits[5:4], RW1CS, default = 2'b00 
       Port1 Egress Parity Error
     */
    UINT32 csi0parerre2e : 1;
    /* csi0parerre2e - Bits[6:6], RW1CS, default = 1'b0 
       Port0 End to End Parity Error
     */
    UINT32 csi1parerre2e : 1;
    /* csi1parerre2e - Bits[7:7], RW1CS, default = 1'b0 
       Port1 End to End Parity Error
     */
    UINT32 rsvd : 24;
    /* rsvd - Bits[31:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R3INGERRLOG2_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* R3QINGCTL_R3QPI_CFG_REG supported on:                                      */
/*       IVT_EP (0x4019C098)                                                  */
/*       IVT_EX (0x4019C098)                                                  */
/*       HSX (0x40158098)                                                     */
/*       BDX (0x40158098)                                                     */
/* Register default value:              0x00000012                            */
#define R3QINGCTL_R3QPI_CFG_REG 0x05004098


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.0.CFG.xml.
 * R3QPI Ingress Control Register
 */
typedef union {
  struct {
    UINT32 maxpktsizechkdis : 1;
    /* maxpktsizechkdis - Bits[0:0], RWS, default = 1'b0 
       Disable Max packet size check
     */
    UINT32 ncycb4minpktsizecsi0 : 3;
    /* ncycb4minpktsizecsi0 - Bits[3:1], RWS, default = 3'b001 
       For last number of cylces, there is no winning packet. This is used for 
       selecting the mix max credit threshold for IGR arbitration on Port0.  
     */
    UINT32 ncycb4minpktsizecsi1 : 3;
    /* ncycb4minpktsizecsi1 - Bits[6:4], RWS, default = 3'b001 
       For last number of cylces, there is no winning packet. This is used for 
       selecting the mix max credit threshold for IGR arbitration on Port1.  
     */
    UINT32 ensmcreditchkunnnh : 1;
    /* ensmcreditchkunnnh - Bits[7:7], RWS, default = 1'b0 
       Enable state machine credit check
     */
    UINT32 usea0accmodetsv : 1;
    /* usea0accmodetsv - Bits[8:8], RWS, default = 1'b0 
       Use Accmulated mode TSV
     */
    UINT32 rsvd : 23;
    /* rsvd - Bits[31:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R3QINGCTL_R3QPI_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */






/* R3QINGADVNACTL_R3QPI_CFG_REG supported on:                                 */
/*       IVT_EP (0x4019C0A0)                                                  */
/*       IVT_EX (0x4019C0A0)                                                  */
/*       HSX (0x401580A0)                                                     */
/*       BDX (0x401580A0)                                                     */
/* Register default value:              0x100E080E                            */
#define R3QINGADVNACTL_R3QPI_CFG_REG 0x050040A0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI Ingress AD/BL VNA Credit Control Register
 */
typedef union {
  struct {
    UINT32 maxadvnacrdthr : 8;
    /* maxadvnacrdthr - Bits[7:0], RW, default = 8'h0e 
       Max AD Vna Credit Threshold for Igress Arb
     */
    UINT32 minadvnacrdthr : 8;
    /* minadvnacrdthr - Bits[15:8], RW, default = 8'h08 
       Min AD Vna Credit Threshold for Igress Arb
     */
    UINT32 blvnacrdthr : 8;
    /* blvnacrdthr - Bits[23:16], RW, default = 8'h0e 
       BL Vna Credit Threshold for Igress Arb
     */
    UINT32 adbp1vnacrdthr : 8;
    /* adbp1vnacrdthr - Bits[31:24], RW, default = 8'h10 
       AD Bypass1 Vna Credit Threshold for Igress Arb
     */
  } Bits;
  UINT32 Data;
} R3QINGADVNACTL_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* R3INGADTXQCTL_R3QPI_CFG_REG supported on:                                  */
/*       IVT_EP (0x4019C0A4)                                                  */
/*       IVT_EX (0x4019C0A4)                                                  */
/*       HSX (0x401580A4)                                                     */
/*       BDX (0x401580A4)                                                     */
/* Register default value:              0x10100810                            */
#define R3INGADTXQCTL_R3QPI_CFG_REG 0x050040A4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI Ingress TXQ Control Register
 */
typedef union {
  struct {
    UINT32 maxadtxqcrdthr : 8;
    /* maxadtxqcrdthr - Bits[7:0], RW, default = 8'h10 
       Max AD Txq Credit Threshold for Igress Arb
     */
    UINT32 minadtxqcrdthr : 8;
    /* minadtxqcrdthr - Bits[15:8], RW, default = 8'h08 
       Min AD Txq Credit Threshold for Igress Arb
     */
    UINT32 bltxqcrdthr : 8;
    /* bltxqcrdthr - Bits[23:16], RW, default = 8'h10 
       BL Txq Credit Threshold for Igress Arb
     */
    UINT32 adbp1txqcrdthr : 8;
    /* adbp1txqcrdthr - Bits[31:24], RW, default = 8'h10 
       AD bypass1 Txq Credit Threshold for Igress Arb
     */
  } Bits;
  UINT32 Data;
} R3INGADTXQCTL_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */






/* R3EGRERRLOG0_R3QPI_CFG_REG supported on:                                   */
/*       IVT_EP (0x4019C0B0)                                                  */
/*       IVT_EX (0x4019C0B0)                                                  */
/*       HSX (0x401580B0)                                                     */
/*       BDX (0x401580B0)                                                     */
/* Register default value:              0x00000000                            */
#define R3EGRERRLOG0_R3QPI_CFG_REG 0x050040B0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * Port0 Egress Error Log Register
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    /* rsvd_0 - Bits[0:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 adegress_bp_qualify : 1;
    /* adegress_bp_qualify - Bits[1:1], RW1CS, default = 1'b0 
       AD Egress BP1 Up and Down qualifier asserted at the same time
     */
    UINT32 akegress_write_to_valid_entry : 1;
    /* akegress_write_to_valid_entry - Bits[2:2], RW1CS, default = 1'b0 
       New AK message writes to an existing valid entry
     */
    UINT32 adegress_write_to_valid_entry : 1;
    /* adegress_write_to_valid_entry - Bits[3:3], RW1CS, default = 1'b0 
       New AD message writes to an existing valid entry
     */
    UINT32 blegress_write_to_valid_entry : 1;
    /* blegress_write_to_valid_entry - Bits[4:4], RW1CS, default = 1'b0 
       New BL message writes to an existing valid entry
     */
    UINT32 akegress_overflow : 1;
    /* akegress_overflow - Bits[5:5], RW1CS, default = 1'b0 
       AK egress overflow
     */
    UINT32 adegress_overflow : 1;
    /* adegress_overflow - Bits[6:6], RW1CS, default = 1'b0 
       AD egress overflow
     */
    UINT32 blegress_overflow : 1;
    /* blegress_overflow - Bits[7:7], RW1CS, default = 1'b0 
       BL egress overflow
     */
    UINT32 akegressq_rdwrsamelocation : 1;
    /* akegressq_rdwrsamelocation - Bits[8:8], RW1CS, default = 1'b0 
       AK egress read and write to the same entry
     */
    UINT32 advn0ndrcrdunderflow : 1;
    /* advn0ndrcrdunderflow - Bits[9:9], RW1CS, default = 1'b0 
       AD NDR VN0 credit underflow
     */
    UINT32 advn0snpcrdunderflow : 1;
    /* advn0snpcrdunderflow - Bits[10:10], RW1CS, default = 1'b0 
       AD SNP VN0 credit underflow
     */
    UINT32 advn0homcrdunderflow : 1;
    /* advn0homcrdunderflow - Bits[11:11], RW1CS, default = 1'b0 
       AD HOM VN0 credit underflow
     */
    UINT32 advnacrdunderflow : 1;
    /* advnacrdunderflow - Bits[12:12], RW1CS, default = 1'b0 
       AD VNA credit underflow
     */
    UINT32 advn0ndrcrdoverflow : 1;
    /* advn0ndrcrdoverflow - Bits[13:13], RW1CS, default = 1'b0 
       AD NDR VN0 credit Overflow
     */
    UINT32 advn0snpcrdoverflow : 1;
    /* advn0snpcrdoverflow - Bits[14:14], RW1CS, default = 1'b0 
       AD SNP VN0 credit Overflow
     */
    UINT32 advn0homcrdoverflow : 1;
    /* advn0homcrdoverflow - Bits[15:15], RW1CS, default = 1'b0 
       AD HOM VN0 credit Overflow
     */
    UINT32 advnacrdoverflow : 1;
    /* advnacrdoverflow - Bits[16:16], RW1CS, default = 1'b0 
       AD VNA credit Overflow
     */
    UINT32 adcbocrdunderflow : 1;
    /* adcbocrdunderflow - Bits[17:17], RW1CS, default = 1'b0 
       AD Cbo Credit Underflow
     */
    UINT32 blpcivn0ncscrdoverflow : 1;
    /* blpcivn0ncscrdoverflow - Bits[18:18], RW1CS, default = 1'b0 
       BL PCI NCS VN0 credit Overflow
     */
    UINT32 blpcivn0ncbcrdoverflow : 1;
    /* blpcivn0ncbcrdoverflow - Bits[19:19], RW1CS, default = 1'b0 
       BL PCI NCB VN0 credit Overflow
     */
    UINT32 rsvd_20 : 1;
    /* rsvd_20 - Bits[20:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 blpcivn0ncscrdunderflow : 1;
    /* blpcivn0ncscrdunderflow - Bits[21:21], RW1CS, default = 1'b0 
       BL PCI NCS VN0 credit Underflow
     */
    UINT32 blpcivn0ncbcrdunderflow : 1;
    /* blpcivn0ncbcrdunderflow - Bits[22:22], RW1CS, default = 1'b0 
       BL PCI NCB VN0 credit Underflow
     */
    UINT32 rsvd_23 : 1;
    /* rsvd_23 - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 blvn0ncscrdunderflow : 1;
    /* blvn0ncscrdunderflow - Bits[24:24], RW1CS, default = 1'b0 
       BL NCS VN0 credit Underflow
     */
    UINT32 blvn0ncbcrdunderflow : 1;
    /* blvn0ncbcrdunderflow - Bits[25:25], RW1CS, default = 1'b0 
       BL NCB VN0 credit Underflow
     */
    UINT32 blvn0drscrdunderflow : 1;
    /* blvn0drscrdunderflow - Bits[26:26], RW1CS, default = 1'b0 
       BL DRS VN0 credit Underflow
     */
    UINT32 blvnacrdunderflow : 1;
    /* blvnacrdunderflow - Bits[27:27], RW1CS, default = 1'b0 
       BL VNA credit Underflow
     */
    UINT32 blvn0ncscrdoverflow : 1;
    /* blvn0ncscrdoverflow - Bits[28:28], RW1CS, default = 1'b0 
       BL NCS VN0 credit Overflow
     */
    UINT32 blvn0ncbcrdoverflow : 1;
    /* blvn0ncbcrdoverflow - Bits[29:29], RW1CS, default = 1'b0 
       BL NCB VN0 credit Overflow
     */
    UINT32 blvn0drscrdoverflow : 1;
    /* blvn0drscrdoverflow - Bits[30:30], RW1CS, default = 1'b0 
       BL DRS VN0 credit Overflow
     */
    UINT32 blvnacrdoverflow : 1;
    /* blvnacrdoverflow - Bits[31:31], RW1CS, default = 1'b0 
       BL VNA credit Overflow
     */
  } Bits;
  UINT32 Data;
} R3EGRERRLOG0_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* R3EGRERRMSK0_R3QPI_CFG_REG supported on:                                   */
/*       IVT_EP (0x4019C0B4)                                                  */
/*       IVT_EX (0x4019C0B4)                                                  */
/*       HSX (0x401580B4)                                                     */
/*       BDX (0x401580B4)                                                     */
/* Register default value:              0x00000000                            */
#define R3EGRERRMSK0_R3QPI_CFG_REG 0x050040B4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI Egreess Error Mask Register Port0
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    /* rsvd_0 - Bits[0:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 adegress_bp_qualifymask : 1;
    /* adegress_bp_qualifymask - Bits[1:1], RWS, default = 1'b0 
       Mask AD Egress BP Qualify Error
     */
    UINT32 akegress_write_to_valid_entrymask : 1;
    /* akegress_write_to_valid_entrymask - Bits[2:2], RWS, default = 1'b0 
       Mask AK Egress Write to an existing Valid Entry Error
     */
    UINT32 adegress_write_to_valid_entrymask : 1;
    /* adegress_write_to_valid_entrymask - Bits[3:3], RWS, default = 1'b0 
       Mask AD Egress Write to an existing Valid Entry Error
     */
    UINT32 blegress_write_to_valid_entrymask : 1;
    /* blegress_write_to_valid_entrymask - Bits[4:4], RWS, default = 1'b0 
       Mask BL Egress Write to an existing Valid Entry Error
     */
    UINT32 akegress_overflowmask : 1;
    /* akegress_overflowmask - Bits[5:5], RWS, default = 1'b0 
       Mask AK Egress Credit Overflow Error
     */
    UINT32 adegress_overflowmask : 1;
    /* adegress_overflowmask - Bits[6:6], RWS, default = 1'b0 
       Mask AD Egress Credit Overflow Error
     */
    UINT32 blegress_overflowmask : 1;
    /* blegress_overflowmask - Bits[7:7], RWS, default = 1'b0 
       Mask BL Egress Overflow Error
     */
    UINT32 akegressq_rdwrsamelocationmask : 1;
    /* akegressq_rdwrsamelocationmask - Bits[8:8], RWS, default = 1'b0 
       Mask AK Egress Read write to the same entry Error
     */
    UINT32 advn0ndrcrdunderflowmask : 1;
    /* advn0ndrcrdunderflowmask - Bits[9:9], RWS, default = 1'b0 
       Mask AD NDR VN0 Credit Underflow Error
     */
    UINT32 advn0snpcrdunderflowmask : 1;
    /* advn0snpcrdunderflowmask - Bits[10:10], RWS, default = 1'b0 
       Mask AD SNP VN0 Credit Underflow Error
     */
    UINT32 advn0homcrdunderflowmask : 1;
    /* advn0homcrdunderflowmask - Bits[11:11], RWS, default = 1'b0 
       Mask AD HOM VN0 Credit Underflow Error
     */
    UINT32 advnacrdunderflowmask : 1;
    /* advnacrdunderflowmask - Bits[12:12], RWS, default = 1'b0 
       Mask AD VNA Credit Underflow Error
     */
    UINT32 advn0ndrcrdoverflowmask : 1;
    /* advn0ndrcrdoverflowmask - Bits[13:13], RWS, default = 1'b0 
       Mask AD NDR VN0 Credit Underflow Error
     */
    UINT32 advn0snpcrdoverflowmask : 1;
    /* advn0snpcrdoverflowmask - Bits[14:14], RWS, default = 1'b0 
       Mask AD SNP VN0 Credit Overflow Error
     */
    UINT32 advn0homcrdoverflowmask : 1;
    /* advn0homcrdoverflowmask - Bits[15:15], RWS, default = 1'b0 
       Mask AD HOM VN0 Credit Overflow Error
     */
    UINT32 advnacrdoverflowmask : 1;
    /* advnacrdoverflowmask - Bits[16:16], RWS, default = 1'b0 
       Mask AD VNA Credit Overflow Error
     */
    UINT32 adcbocrdunderflowmask : 1;
    /* adcbocrdunderflowmask - Bits[17:17], RWS, default = 1'b0 
       Mask AD Cbo Credit Underflow Error
     */
    UINT32 blpcivn0ncscrdoverflowmask : 1;
    /* blpcivn0ncscrdoverflowmask - Bits[18:18], RWS, default = 1'b0 
       Mask BL PCI NCS VN0 Credit Overflow Error
     */
    UINT32 blpcivn0ncbcrdoverflowmask : 1;
    /* blpcivn0ncbcrdoverflowmask - Bits[19:19], RWS, default = 1'b0 
       Mask BL PCI NCB VN0 Credit Overflow Error
     */
    UINT32 rsvd_20 : 1;
    /* rsvd_20 - Bits[20:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 blpcivn0ncscrdunderflowmask : 1;
    /* blpcivn0ncscrdunderflowmask - Bits[21:21], RWS, default = 1'b0 
       Mask BL PCI NCS VN0 Credit Underflow Error
     */
    UINT32 blpcivn0ncbcrdunderflowmask : 1;
    /* blpcivn0ncbcrdunderflowmask - Bits[22:22], RWS, default = 1'b0 
       Mask BL PCI NCB VN0 Credit Underflow Error
     */
    UINT32 rsvd_23 : 1;
    /* rsvd_23 - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 blvn0ncscrdunderflowmask : 1;
    /* blvn0ncscrdunderflowmask - Bits[24:24], RWS, default = 1'b0 
       Mask BL BCS VN0 Credit Underflow Error
     */
    UINT32 blvn0ncbcrdunderflowmask : 1;
    /* blvn0ncbcrdunderflowmask - Bits[25:25], RWS, default = 1'b0 
       Mask BL BCB VN0 Credit Underflow Error
     */
    UINT32 blvn0drscrdunderflowmask : 1;
    /* blvn0drscrdunderflowmask - Bits[26:26], RWS, default = 1'b0 
       Mask BL DRS VN0 Credit Underflow Error
     */
    UINT32 blvnacrdunderflowmask : 1;
    /* blvnacrdunderflowmask - Bits[27:27], RWS, default = 1'b0 
       Mask BL VNA Credit Underflow Error
     */
    UINT32 blvn0ncscrdoverflowmask : 1;
    /* blvn0ncscrdoverflowmask - Bits[28:28], RWS, default = 1'b0 
       Mask BL NCS VN0 Credit Overflow Error
     */
    UINT32 blvn0ncbcrdoverflowmask : 1;
    /* blvn0ncbcrdoverflowmask - Bits[29:29], RWS, default = 1'b0 
       Mask BL NCB VN0 Credit Overflow Error
     */
    UINT32 blvn0drscrdoverflowmask : 1;
    /* blvn0drscrdoverflowmask - Bits[30:30], RWS, default = 1'b0 
       Mask BL DRS VN0 Credit Overflow Error
     */
    UINT32 blvnacrdoverflowmask : 1;
    /* blvnacrdoverflowmask - Bits[31:31], RWS, default = 1'b0 
       Mask BL VNA Credit Overflow Error
     */
  } Bits;
  UINT32 Data;
} R3EGRERRMSK0_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* R3EGRERRLOG1_R3QPI_CFG_REG supported on:                                   */
/*       IVT_EP (0x4019C0B8)                                                  */
/*       IVT_EX (0x4019C0B8)                                                  */
/*       HSX (0x401580B8)                                                     */
/*       BDX (0x401580B8)                                                     */
/* Register default value:              0x00000000                            */
#define R3EGRERRLOG1_R3QPI_CFG_REG 0x050040B8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI Egress Error Log Register port 1
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    /* rsvd_0 - Bits[0:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 adegress_bp_qualify : 1;
    /* adegress_bp_qualify - Bits[1:1], RW1CS, default = 1'b0 
       AD Egress BP1 Up and Down qualifier asserted at the same time
     */
    UINT32 akegress_write_to_valid_entry : 1;
    /* akegress_write_to_valid_entry - Bits[2:2], RW1CS, default = 1'b0 
       New AK message writes to an existing valid entry
     */
    UINT32 adegress_write_to_valid_entry : 1;
    /* adegress_write_to_valid_entry - Bits[3:3], RW1CS, default = 1'b0 
       New AD message writes to an existing valid entry
     */
    UINT32 blegress_write_to_valid_entry : 1;
    /* blegress_write_to_valid_entry - Bits[4:4], RW1CS, default = 1'b0 
       New BL message writes to an existing valid entry
     */
    UINT32 akegress_overflow : 1;
    /* akegress_overflow - Bits[5:5], RW1CS, default = 1'b0 
       AK egress overflow
     */
    UINT32 adegress_overflow : 1;
    /* adegress_overflow - Bits[6:6], RW1CS, default = 1'b0 
       AD egress overflow
     */
    UINT32 blegress_overflow : 1;
    /* blegress_overflow - Bits[7:7], RW1CS, default = 1'b0 
       BL egress overflow
     */
    UINT32 akegressq_rdwrsamelocation : 1;
    /* akegressq_rdwrsamelocation - Bits[8:8], RW1CS, default = 1'b0 
       AK egress read and write to the same entry
     */
    UINT32 advn0ndrcrdunderflow : 1;
    /* advn0ndrcrdunderflow - Bits[9:9], RW1CS, default = 1'b0 
       AD NDR VN0 credit underflow
     */
    UINT32 advn0snpcrdunderflow : 1;
    /* advn0snpcrdunderflow - Bits[10:10], RW1CS, default = 1'b0 
       AD SNP VN0 credit underflow
     */
    UINT32 advn0homcrdunderflow : 1;
    /* advn0homcrdunderflow - Bits[11:11], RW1CS, default = 1'b0 
       AD HOM VN0 credit underflow
     */
    UINT32 advnacrdunderflow : 1;
    /* advnacrdunderflow - Bits[12:12], RW1CS, default = 1'b0 
       AD VNA credit underflow
     */
    UINT32 advn0ndrcrdoverflow : 1;
    /* advn0ndrcrdoverflow - Bits[13:13], RW1CS, default = 1'b0 
       AD NDR VN0 credit Overflow
     */
    UINT32 advn0snpcrdoverflow : 1;
    /* advn0snpcrdoverflow - Bits[14:14], RW1CS, default = 1'b0 
       AD SNP VN0 credit Overflow
     */
    UINT32 advn0homcrdoverflow : 1;
    /* advn0homcrdoverflow - Bits[15:15], RW1CS, default = 1'b0 
       AD HOM VN0 credit Overflow
     */
    UINT32 advnacrdoverflow : 1;
    /* advnacrdoverflow - Bits[16:16], RW1CS, default = 1'b0 
       AD VNA credit Overflow
     */
    UINT32 adcbocrdunderflow : 1;
    /* adcbocrdunderflow - Bits[17:17], RW1CS, default = 1'b0 
       AD Cbo Credit Underflow
     */
    UINT32 blpcivn0ncscrdoverflow : 1;
    /* blpcivn0ncscrdoverflow - Bits[18:18], RW1CS, default = 1'b0 
       BL PCI NCS VN0 credit Overflow
     */
    UINT32 blpcivn0ncbcrdoverflow : 1;
    /* blpcivn0ncbcrdoverflow - Bits[19:19], RW1CS, default = 1'b0 
       BL PCI NCB VN0 credit Overflow
     */
    UINT32 rsvd_20 : 1;
    /* rsvd_20 - Bits[20:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 blpcivn0ncscrdunderflow : 1;
    /* blpcivn0ncscrdunderflow - Bits[21:21], RW1CS, default = 1'b0 
       BL PCI NCS VN0 credit Underflow
     */
    UINT32 blpcivn0ncbcrdunderflow : 1;
    /* blpcivn0ncbcrdunderflow - Bits[22:22], RW1CS, default = 1'b0 
       BL PCI NCB VN0 credit Underflow
     */
    UINT32 rsvd_23 : 1;
    /* rsvd_23 - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 blvn0ncscrdunderflow : 1;
    /* blvn0ncscrdunderflow - Bits[24:24], RW1CS, default = 1'b0 
       BL NCS VN0 credit Underflow
     */
    UINT32 blvn0ncbcrdunderflow : 1;
    /* blvn0ncbcrdunderflow - Bits[25:25], RW1CS, default = 1'b0 
       BL NCB VN0 credit Underflow
     */
    UINT32 blvn0drscrdunderflow : 1;
    /* blvn0drscrdunderflow - Bits[26:26], RW1CS, default = 1'b0 
       BL DRS VN0 credit Underflow
     */
    UINT32 blvnacrdunderflow : 1;
    /* blvnacrdunderflow - Bits[27:27], RW1CS, default = 1'b0 
       BL VNA credit Underflow
     */
    UINT32 blvn0ncscrdoverflow : 1;
    /* blvn0ncscrdoverflow - Bits[28:28], RW1CS, default = 1'b0 
       BL NCS VN0 credit Overflow
     */
    UINT32 blvn0ncbcrdoverflow : 1;
    /* blvn0ncbcrdoverflow - Bits[29:29], RW1CS, default = 1'b0 
       BL NCB VN0 credit Overflow
     */
    UINT32 blvn0drscrdoverflow : 1;
    /* blvn0drscrdoverflow - Bits[30:30], RW1CS, default = 1'b0 
       BL DRS VN0 credit Overflow
     */
    UINT32 blvnacrdoverflow : 1;
    /* blvnacrdoverflow - Bits[31:31], RW1CS, default = 1'b0 
       BL VNA credit Overflow
     */
  } Bits;
  UINT32 Data;
} R3EGRERRLOG1_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* R3EGRERRMSK1_R3QPI_CFG_REG supported on:                                   */
/*       IVT_EP (0x4019C0BC)                                                  */
/*       IVT_EX (0x4019C0BC)                                                  */
/*       HSX (0x401580BC)                                                     */
/*       BDX (0x401580BC)                                                     */
/* Register default value:              0x00000000                            */
#define R3EGRERRMSK1_R3QPI_CFG_REG 0x050040BC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI Egreess Error Mask Register Port 1
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    /* rsvd_0 - Bits[0:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 adegress_bp_qualifymask : 1;
    /* adegress_bp_qualifymask - Bits[1:1], RWS, default = 1'b0 
       Mask AD Egress BP Qualify Error
     */
    UINT32 akegress_write_to_valid_entrymask : 1;
    /* akegress_write_to_valid_entrymask - Bits[2:2], RWS, default = 1'b0 
       Mask AK Egress Write to an existing Valid Entry Error
     */
    UINT32 adegress_write_to_valid_entrymask : 1;
    /* adegress_write_to_valid_entrymask - Bits[3:3], RWS, default = 1'b0 
       Mask AD Egress Write to an existing Valid Entry Error
     */
    UINT32 blegress_write_to_valid_entrymask : 1;
    /* blegress_write_to_valid_entrymask - Bits[4:4], RWS, default = 1'b0 
       Mask BL Egress Write to an existing Valid Entry Error
     */
    UINT32 akegress_overflowmask : 1;
    /* akegress_overflowmask - Bits[5:5], RWS, default = 1'b0 
       Mask AK Egress Credit Overflow Error
     */
    UINT32 adegress_overflowmask : 1;
    /* adegress_overflowmask - Bits[6:6], RWS, default = 1'b0 
       Mask AD Egress Credit Overflow Error
     */
    UINT32 blegress_overflowmask : 1;
    /* blegress_overflowmask - Bits[7:7], RWS, default = 1'b0 
       Mask BL Egress Overflow Error
     */
    UINT32 akegressq_rdwrsamelocationmask : 1;
    /* akegressq_rdwrsamelocationmask - Bits[8:8], RWS, default = 1'b0 
       Mask AK Egress Read write to the same entry Error
     */
    UINT32 advn0ndrcrdunderflowmask : 1;
    /* advn0ndrcrdunderflowmask - Bits[9:9], RWS, default = 1'b0 
       Mask AD NDR VN0 Credit Underflow Error
     */
    UINT32 advn0snpcrdunderflowmask : 1;
    /* advn0snpcrdunderflowmask - Bits[10:10], RWS, default = 1'b0 
       Mask AD SNP VN0 Credit Underflow Error
     */
    UINT32 advn0homcrdunderflowmask : 1;
    /* advn0homcrdunderflowmask - Bits[11:11], RWS, default = 1'b0 
       Mask AD HOM VN0 Credit Underflow Error
     */
    UINT32 advnacrdunderflowmask : 1;
    /* advnacrdunderflowmask - Bits[12:12], RWS, default = 1'b0 
       Mask AD VNA Credit Underflow Error
     */
    UINT32 advn0ndrcrdoverflowmask : 1;
    /* advn0ndrcrdoverflowmask - Bits[13:13], RWS, default = 1'b0 
       Mask AD NDR VN0 Credit Underflow Error
     */
    UINT32 advn0snpcrdoverflowmask : 1;
    /* advn0snpcrdoverflowmask - Bits[14:14], RWS, default = 1'b0 
       Mask AD SNP VN0 Credit Overflow Error
     */
    UINT32 advn0homcrdoverflowmask : 1;
    /* advn0homcrdoverflowmask - Bits[15:15], RWS, default = 1'b0 
       Mask AD HOM VN0 Credit Overflow Error
     */
    UINT32 advnacrdoverflowmask : 1;
    /* advnacrdoverflowmask - Bits[16:16], RWS, default = 1'b0 
       Mask AD VNA Credit Overflow Error
     */
    UINT32 adcbocrdunderflowmask : 1;
    /* adcbocrdunderflowmask - Bits[17:17], RWS, default = 1'b0 
       Mask AD Cbo Credit Underflow Error
     */
    UINT32 blpcivn0ncscrdoverflowmask : 1;
    /* blpcivn0ncscrdoverflowmask - Bits[18:18], RWS, default = 1'b0 
       Mask BL PCI NCS VN0 Credit Overflow Error
     */
    UINT32 blpcivn0ncbcrdoverflowmask : 1;
    /* blpcivn0ncbcrdoverflowmask - Bits[19:19], RWS, default = 1'b0 
       Mask BL PCI NCB VN0 Credit Overflow Error
     */
    UINT32 rsvd_20 : 1;
    /* rsvd_20 - Bits[20:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 blpcivn0ncscrdunderflowmask : 1;
    /* blpcivn0ncscrdunderflowmask - Bits[21:21], RWS, default = 1'b0 
       Mask BL PCI NCS VN0 Credit Underflow Error
     */
    UINT32 blpcivn0ncbcrdunderflowmask : 1;
    /* blpcivn0ncbcrdunderflowmask - Bits[22:22], RWS, default = 1'b0 
       Mask BL PCI NCB VN0 Credit Underflow Error
     */
    UINT32 rsvd_23 : 1;
    /* rsvd_23 - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 blvn0ncscrdunderflowmask : 1;
    /* blvn0ncscrdunderflowmask - Bits[24:24], RWS, default = 1'b0 
       Mask BL BCS VN0 Credit Underflow Error
     */
    UINT32 blvn0ncbcrdunderflowmask : 1;
    /* blvn0ncbcrdunderflowmask - Bits[25:25], RWS, default = 1'b0 
       Mask BL BCB VN0 Credit Underflow Error
     */
    UINT32 blvn0drscrdunderflowmask : 1;
    /* blvn0drscrdunderflowmask - Bits[26:26], RWS, default = 1'b0 
       Mask BL DRS VN0 Credit Underflow Error
     */
    UINT32 blvnacrdunderflowmask : 1;
    /* blvnacrdunderflowmask - Bits[27:27], RWS, default = 1'b0 
       Mask BL VNA Credit Underflow Error
     */
    UINT32 blvn0ncscrdoverflowmask : 1;
    /* blvn0ncscrdoverflowmask - Bits[28:28], RWS, default = 1'b0 
       Mask BL NCS VN0 Credit Overflow Error
     */
    UINT32 blvn0ncbcrdoverflowmask : 1;
    /* blvn0ncbcrdoverflowmask - Bits[29:29], RWS, default = 1'b0 
       Mask BL NCB VN0 Credit Overflow Error
     */
    UINT32 blvn0drscrdoverflowmask : 1;
    /* blvn0drscrdoverflowmask - Bits[30:30], RWS, default = 1'b0 
       Mask BL DRS VN0 Credit Overflow Error
     */
    UINT32 blvnacrdoverflowmask : 1;
    /* blvnacrdoverflowmask - Bits[31:31], RWS, default = 1'b0 
       Mask BL VNA Credit Overflow Error
     */
  } Bits;
  UINT32 Data;
} R3EGRERRMSK1_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */




/* R3INGERRLOG0_R3QPI_CFG_REG supported on:                                   */
/*       IVT_EP (0x4019C0C4)                                                  */
/*       IVT_EX (0x4019C0C4)                                                  */
/*       HSX (0x401580C4)                                                     */
/*       BDX (0x401580C4)                                                     */
/* Register default value:              0x00000000                            */
#define R3INGERRLOG0_R3QPI_CFG_REG 0x050040C4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.0.CFG.xml.
 * R3QPI Ingress Error Log Register0 
 */
typedef union {
  struct {
    UINT32 csi0txqcrdmaxchk : 1;
    /* csi0txqcrdmaxchk - Bits[0:0], RW1CS, default = 1'b0 
       Port0 TxQ Credit Max Check
     */
    UINT32 csi0ncscrdmaxchk : 1;
    /* csi0ncscrdmaxchk - Bits[1:1], RW1CS, default = 1'b0 
       Port0 NCS Credit Max Check
     */
    UINT32 csi0ncbcrdmaxchk : 1;
    /* csi0ncbcrdmaxchk - Bits[2:2], RW1CS, default = 1'b0 
       Port0 NCB Credit Max Check
     */
    UINT32 csi0drscrdmaxchk : 1;
    /* csi0drscrdmaxchk - Bits[3:3], RW1CS, default = 1'b0 
       Port0 DRS Credit Max Check
     */
    UINT32 csi0ndrcrdmaxchk : 1;
    /* csi0ndrcrdmaxchk - Bits[4:4], RW1CS, default = 1'b0 
       Port0 Ndr Credit Max Check
     */
    UINT32 csi0snpcrdmaxchk : 1;
    /* csi0snpcrdmaxchk - Bits[5:5], RW1CS, default = 1'b0 
       Port0 Snp Credit Max Check
     */
    UINT32 csi0homcrdmaxchk : 1;
    /* csi0homcrdmaxchk - Bits[6:6], RW1CS, default = 1'b0 
       Port0 HOM Credit Max Check
     */
    UINT32 csi0vnacrdmaxchk : 1;
    /* csi0vnacrdmaxchk - Bits[7:7], RW1CS, default = 1'b0 
       Port0 Vna Credit Max Check
     */
    UINT32 csi0blingroverflow : 1;
    /* csi0blingroverflow - Bits[8:8], RW1CS, default = 1'b0 
       Port0 BL IngressQ Overflow
     */
    UINT32 csi0adingroverflow : 1;
    /* csi0adingroverflow - Bits[9:9], RW1CS, default = 1'b0 
       Port0 AD IngressQ Overflow
     */
    UINT32 rsvd_10 : 1;
    /* rsvd_10 - Bits[10:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 csi0remncscrdunderflow : 1;
    /* csi0remncscrdunderflow - Bits[11:11], RW1CS, default = 1'b0 
       Port0 remote NCS credit Underflow
     */
    UINT32 csi0remncbcrdunderflow : 1;
    /* csi0remncbcrdunderflow - Bits[12:12], RW1CS, default = 1'b0 
       Port0 remote NCB credit Underflow
     */
    UINT32 csi0remdrscrdunderflow : 1;
    /* csi0remdrscrdunderflow - Bits[13:13], RW1CS, default = 1'b0 
       Port0 remote DRS credit Underflow
     */
    UINT32 csi0remndrcrdunderflow : 1;
    /* csi0remndrcrdunderflow - Bits[14:14], RW1CS, default = 1'b0 
       Port0 remote Ndr Credit Underflow
     */
    UINT32 csi0remsnpcrdunderflow : 1;
    /* csi0remsnpcrdunderflow - Bits[15:15], RW1CS, default = 1'b0 
       Port0 remote Snp Credit Underflow
     */
    UINT32 csi0remhomcrdunderflow : 1;
    /* csi0remhomcrdunderflow - Bits[16:16], RW1CS, default = 1'b0 
       Port0 remote HOM Credit Underflow
     */
    UINT32 csi0remvnacrdunderflow : 1;
    /* csi0remvnacrdunderflow - Bits[17:17], RW1CS, default = 1'b0 
       Port0 remote Vna Credit Underflow
     */
    UINT32 csi0txqcrdoverflow : 1;
    /* csi0txqcrdoverflow - Bits[18:18], RW1CS, default = 1'b0 
       Port0 Txq Credit Overflow
     */
    UINT32 csi0txqcrdunderflow : 1;
    /* csi0txqcrdunderflow - Bits[19:19], RW1CS, default = 1'b0 
       Port0 Txq Credit Underflow
     */
    UINT32 csi0llresetrxnotidle : 1;
    /* csi0llresetrxnotidle - Bits[20:20], RW1CS, default = 1'b0 
       Port0 Rx Not Idle during LL Reset
     */
    UINT32 csi0llresettxnotidle : 1;
    /* csi0llresettxnotidle - Bits[21:21], RW1CS, default = 1'b0 
       Port0 Tx Not Idle during LL Reset
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 csi0ingrdrsptrinvalid : 1;
    /* csi0ingrdrsptrinvalid - Bits[25:25], RW1CS, default = 1'b0 
       Port0 Ingress DRS Ptr is Invalid
     */
    UINT32 csi0ingrncbptrinvalid : 1;
    /* csi0ingrncbptrinvalid - Bits[26:26], RW1CS, default = 1'b0 
       Port0 Ingress Ncb Ptr is Invalid
     */
    UINT32 csi0ingrncsptrinvalid : 1;
    /* csi0ingrncsptrinvalid - Bits[27:27], RW1CS, default = 1'b0 
       Port0 Ingress Ncs Ptr is Invalid
     */
    UINT32 csi0ingrhomptrinvalid : 1;
    /* csi0ingrhomptrinvalid - Bits[28:28], RW1CS, default = 1'b0 
       Port0 Ingress HOM Ptr is Invalid
     */
    UINT32 csi0ingrsnpptrinvalid : 1;
    /* csi0ingrsnpptrinvalid - Bits[29:29], RW1CS, default = 1'b0 
       Port0 Ingress Snp Ptr is Invalid
     */
    UINT32 csi0ingrndrptrinvalid : 1;
    /* csi0ingrndrptrinvalid - Bits[30:30], RW1CS, default = 1'b0 
       Port0 Ingress Ndr Ptr is Invalid
     */
    UINT32 csi0invalidadarb : 1;
    /* csi0invalidadarb - Bits[31:31], RW1CS, default = 1'b0 
       Port0 Invalid AD Arb
     */
  } Bits;
  UINT32 Data;
} R3INGERRLOG0_R3QPI_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* R3INGERRMASK0_R3QPI_CFG_REG supported on:                                  */
/*       IVT_EP (0x4019C0C8)                                                  */
/*       IVT_EX (0x4019C0C8)                                                  */
/*       HSX (0x401580C8)                                                     */
/*       BDX (0x401580C8)                                                     */
/* Register default value:              0x00000000                            */
#define R3INGERRMASK0_R3QPI_CFG_REG 0x050040C8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI Ingress Error Mask Register0
 */
typedef union {
  struct {
    UINT32 csi0txqcrdmaxchkmask : 1;
    /* csi0txqcrdmaxchkmask - Bits[0:0], RWS, default = 1'b0 
       Port0 TxQ Credit Max Check Error Mask
     */
    UINT32 csi0ncscrdmaxchkmask : 1;
    /* csi0ncscrdmaxchkmask - Bits[1:1], RWS, default = 1'b0 
       Port0 Ncs Credit Max Check Error Mask
     */
    UINT32 csi0ncbcrdmaxchkmask : 1;
    /* csi0ncbcrdmaxchkmask - Bits[2:2], RWS, default = 1'b0 
       Port0 Ncb Credit Max Check Error Mask
     */
    UINT32 csi0drscrdmaxchkmask : 1;
    /* csi0drscrdmaxchkmask - Bits[3:3], RWS, default = 1'b0 
       Port0 Drs Credit Max Check Error Mask
     */
    UINT32 csi0ndrcrdmaxchkmask : 1;
    /* csi0ndrcrdmaxchkmask - Bits[4:4], RWS, default = 1'b0 
       Port0 Ndr Credit Max Check Error Mask
     */
    UINT32 csi0snpcrdmaxchkmask : 1;
    /* csi0snpcrdmaxchkmask - Bits[5:5], RWS, default = 1'b0 
       Port0 Snp Credit Max Check Error Mask
     */
    UINT32 csi0homcrdmaxchkmask : 1;
    /* csi0homcrdmaxchkmask - Bits[6:6], RWS, default = 1'b0 
       Port0 HOM Credit Max Check Error Mask
     */
    UINT32 csi0vnacrdmaxchkmask : 1;
    /* csi0vnacrdmaxchkmask - Bits[7:7], RWS, default = 1'b0 
       Port0 Vna Credit Max Check Error Mask
     */
    UINT32 csi0blingroverflowmask : 1;
    /* csi0blingroverflowmask - Bits[8:8], RWS, default = 1'b0 
       Port0 BL Ingress Overflow Error Mask
     */
    UINT32 csi0adingroverflowmask : 1;
    /* csi0adingroverflowmask - Bits[9:9], RWS, default = 1'b0 
       Port0 AD Ingress Overflow Error Mask
     */
    UINT32 rsvd_10 : 1;
    /* rsvd_10 - Bits[10:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 csi0remncscrdunderflowmask : 1;
    /* csi0remncscrdunderflowmask - Bits[11:11], RWS, default = 1'b0 
       Port0 Remote Ncs Credit Underflow Error Mask
     */
    UINT32 csi0remncbcrdunderflowmask : 1;
    /* csi0remncbcrdunderflowmask - Bits[12:12], RWS, default = 1'b0 
       Port0 Remote Ncb Credit Underflow Error Mask
     */
    UINT32 csi0remdrscrdunderflowmask : 1;
    /* csi0remdrscrdunderflowmask - Bits[13:13], RWS, default = 1'b0 
       Port0 Remote Drs Credit Underflow Error Mask
     */
    UINT32 csi0remndrcrdunderflowmask : 1;
    /* csi0remndrcrdunderflowmask - Bits[14:14], RWS, default = 1'b0 
       Port0 Remote Ndr Credit Underflow Error Mask
     */
    UINT32 csi0remsnpcrdunderflowmask : 1;
    /* csi0remsnpcrdunderflowmask - Bits[15:15], RWS, default = 1'b0 
       Port0 Remote Snp Credit Underflow Error Mask
     */
    UINT32 csi0remhomcrdunderflowmask : 1;
    /* csi0remhomcrdunderflowmask - Bits[16:16], RWS, default = 1'b0 
       Port0 Remote HOM Credit Underflow Error Mask
     */
    UINT32 csi0remvnacrdunderflowmask : 1;
    /* csi0remvnacrdunderflowmask - Bits[17:17], RWS, default = 1'b0 
       Port0 Remote Vna Credit Underflow Error Mask
     */
    UINT32 csi0txqcrdoverflowmask : 1;
    /* csi0txqcrdoverflowmask - Bits[18:18], RWS, default = 1'b0 
       Port0 Txq Credit Overflow Error Mask
     */
    UINT32 csi0txqcrdunderflowmask : 1;
    /* csi0txqcrdunderflowmask - Bits[19:19], RWS, default = 1'b0 
       Port0 TxQ Credit Underflow Error Mask
     */
    UINT32 csi0llresetrxnotidlemask : 1;
    /* csi0llresetrxnotidlemask - Bits[20:20], RWS, default = 1'b0 
       Port0 link layer Rest while Rx Not Idle Error Mask
     */
    UINT32 csi0llresettxnotidlemask : 1;
    /* csi0llresettxnotidlemask - Bits[21:21], RWS, default = 1'b0 
       Port0 link layer Rest while Tx Not Idle Error Mask
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 csi0ingrdrsptrinvalidmask : 1;
    /* csi0ingrdrsptrinvalidmask - Bits[25:25], RWS, default = 1'b0 
       Port0 Ingress Drs Ptr Invalid Error Mask
     */
    UINT32 csi0ingrncbptrinvalidmask : 1;
    /* csi0ingrncbptrinvalidmask - Bits[26:26], RWS, default = 1'b0 
       Port0 Ingress Ncb Ptr Invalid Error Mask
     */
    UINT32 csi0ingrncsptrinvalidmask : 1;
    /* csi0ingrncsptrinvalidmask - Bits[27:27], RWS, default = 1'b0 
       Port0 Ingress Ncs Ptr Invalid Error Mask
     */
    UINT32 csi0ingrhomptrinvalidmask : 1;
    /* csi0ingrhomptrinvalidmask - Bits[28:28], RWS, default = 1'b0 
       Port0 Ingress Hom Ptr Invalid Error Mask
     */
    UINT32 csi0ingrsnpptrinvalidmask : 1;
    /* csi0ingrsnpptrinvalidmask - Bits[29:29], RWS, default = 1'b0 
       Port0 Ingress Snp Ptr Invalid Error Mask
     */
    UINT32 csi0ingrndrptrinvalidmask : 1;
    /* csi0ingrndrptrinvalidmask - Bits[30:30], RWS, default = 1'b0 
       Port0 Ingress Ndr Ptr Invalid Error Mask
     */
    UINT32 csi0invalidadarbmask : 1;
    /* csi0invalidadarbmask - Bits[31:31], RWS, default = 1'b0 
       Port0 Invalid AD Arb Error Mask
     */
  } Bits;
  UINT32 Data;
} R3INGERRMASK0_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


/* R3EGRERRMSK_VN1_R3QPI_CFG_REG supported on:                                */
/*       IVT_EP (0x4019C0CC)                                                  */
/*       IVT_EX (0x4019C0CC)                                                  */
/*       HSX (0x401580CC)                                                     */
/*       BDX (0x401580CC)                                                     */
/* Register default value:              0x00000000                            */
#define R3EGRERRMSK_VN1_R3QPI_CFG_REG 0x050040CC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.0.CFG.xml.
 * R3QPI Q2Q EGR VN1 Error Mask. Each two bit fields corresponding to the two 
 * possible Q2Q combinations.  
 *                  for left  R3 ring stop, bit[0] = colocated,    bit[1] = cross 
 * agent0. 
 *                  for right R3 ring stop, bit[0] = cross agent0, bit[1] = cross 
 * agent1. 
 *                  It is only supported on the EX processor.
 */
typedef union {
  struct {
    UINT32 advn1homcrdoverflowmask : 2;
    /* advn1homcrdoverflowmask - Bits[1:0], RWS, default = 2'b00 
       Mask Q2Q AD HOM VN1 Credit Overflow Error
     */
    UINT32 advn1homcrdunderflowmask : 2;
    /* advn1homcrdunderflowmask - Bits[3:2], RWS, default = 2'b00 
       Mask AD HOM VN1 Credit Underflow Error
     */
    UINT32 advn1ndrcrdoverflowmask : 2;
    /* advn1ndrcrdoverflowmask - Bits[5:4], RWS, default = 2'b00 
       Mask AD NDR VN1 Credit Overflow Error
     */
    UINT32 advn1ndrcrdunderflowmask : 2;
    /* advn1ndrcrdunderflowmask - Bits[7:6], RWS, default = 2'b00 
       Mask AD NDR VN1 Credit Underflow Error
     */
    UINT32 advn1snpcrdoverflowmask : 2;
    /* advn1snpcrdoverflowmask - Bits[9:8], RWS, default = 2'b00 
       Mask AD SNP VN1 Credit Overflow Error
     */
    UINT32 advn1snpcrdunderflowmask : 2;
    /* advn1snpcrdunderflowmask - Bits[11:10], RWS, default = 2'b00 
       Mask AD SNP VN1 Credit Underflow Error
     */
    UINT32 blpcivn1ncbcrdoverflowmask : 2;
    /* blpcivn1ncbcrdoverflowmask - Bits[13:12], RWS, default = 2'b00 
       Unused
     */
    UINT32 blpcivn1ncbcrdunderflowmask : 2;
    /* blpcivn1ncbcrdunderflowmask - Bits[15:14], RWS, default = 2'b00 
       Unused
     */
    UINT32 blpcivn1ncscrdoverflowmask : 2;
    /* blpcivn1ncscrdoverflowmask - Bits[17:16], RWS, default = 2'b00 
       Unused
     */
    UINT32 blpcivn1ncscrdunderflowmask : 2;
    /* blpcivn1ncscrdunderflowmask - Bits[19:18], RWS, default = 2'b00 
       Unuse
     */
    UINT32 blvn1drscrdoverflowmask : 2;
    /* blvn1drscrdoverflowmask - Bits[21:20], RWS, default = 2'b00 
       Mask BL DRS VN1 Credit Overflow Error
     */
    UINT32 blvn1drscrdunderflowmask : 2;
    /* blvn1drscrdunderflowmask - Bits[23:22], RWS, default = 2'b00 
       Mask BL DRS VN1 Credit Underflow Error
     */
    UINT32 blvn1ncbcrdoverflowmask : 2;
    /* blvn1ncbcrdoverflowmask - Bits[25:24], RWS, default = 2'b00 
       Mask BL NCB VN1 Credit Overflow Error
     */
    UINT32 blvn1ncbcrdunderflowmask : 2;
    /* blvn1ncbcrdunderflowmask - Bits[27:26], RWS, default = 2'b00 
       Mask BL NCB VN1 Credit Underflow Error
     */
    UINT32 blvn1ncscrdoverflowmask : 2;
    /* blvn1ncscrdoverflowmask - Bits[29:28], RWS, default = 2'b00 
       Mask BL NCS VN1 Credit Overflow Error
     */
    UINT32 blvn1ncscrdunderflowmask : 2;
    /* blvn1ncscrdunderflowmask - Bits[31:30], RWS, default = 2'b00 
       Mask BL NCS VN1 Credit Underflow Error
     */
  } Bits;
  UINT32 Data;
} R3EGRERRMSK_VN1_R3QPI_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */






/* R3INGERRLOG1_R3QPI_CFG_REG supported on:                                   */
/*       IVT_EP (0x4019C0D4)                                                  */
/*       IVT_EX (0x4019C0D4)                                                  */
/*       HSX (0x401580D4)                                                     */
/*       BDX (0x401580D4)                                                     */
/* Register default value:              0x00000000                            */
#define R3INGERRLOG1_R3QPI_CFG_REG 0x050040D4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.0.CFG.xml.
 * R3QPI Ingress Error Log Register1
 */
typedef union {
  struct {
    UINT32 csi1txqcrdmaxchk : 1;
    /* csi1txqcrdmaxchk - Bits[0:0], RW1CS, default = 1'b0 
       Port1 TxQ Credit Max Check
     */
    UINT32 csi1ncscrdmaxchk : 1;
    /* csi1ncscrdmaxchk - Bits[1:1], RW1CS, default = 1'b0 
       Port1 Ncs Credit Max Check
     */
    UINT32 csi1ncbcrdmaxchk : 1;
    /* csi1ncbcrdmaxchk - Bits[2:2], RW1CS, default = 1'b0 
       Port1 Ncb Credit Max Check
     */
    UINT32 csi1drscrdmaxchk : 1;
    /* csi1drscrdmaxchk - Bits[3:3], RW1CS, default = 1'b0 
       Port1 Drs Credit Max Check
     */
    UINT32 csi1ndrcrdmaxchk : 1;
    /* csi1ndrcrdmaxchk - Bits[4:4], RW1CS, default = 1'b0 
       Port1 Ndr Credit Max Check
     */
    UINT32 csi1snpcrdmaxchk : 1;
    /* csi1snpcrdmaxchk - Bits[5:5], RW1CS, default = 1'b0 
       Port1 Snp Credit Max Check
     */
    UINT32 csi1homcrdmaxchk : 1;
    /* csi1homcrdmaxchk - Bits[6:6], RW1CS, default = 1'b0 
       Port1 Hom Credit Max Check
     */
    UINT32 csi1vnacrdmaxchk : 1;
    /* csi1vnacrdmaxchk - Bits[7:7], RW1CS, default = 1'b0 
       Port1 Vna Credit Max Check
     */
    UINT32 csi1blingroverflow : 1;
    /* csi1blingroverflow - Bits[8:8], RW1CS, default = 1'b0 
       Port1 BL Ingress Overflow Error
     */
    UINT32 csi1adingroverflow : 1;
    /* csi1adingroverflow - Bits[9:9], RW1CS, default = 1'b0 
       Port1 AD Ingress Overflow Error
     */
    UINT32 rsvd_10 : 1;
    /* rsvd_10 - Bits[10:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 csi1remncscrdunderflow : 1;
    /* csi1remncscrdunderflow - Bits[11:11], RW1CS, default = 1'b0 
       Port1 Remote Ncs Credit Underflow Error
     */
    UINT32 csi1remncbcrdunderflow : 1;
    /* csi1remncbcrdunderflow - Bits[12:12], RW1CS, default = 1'b0 
       Port1 Remote Ncb Credit Underflow Error
     */
    UINT32 csi1remdrscrdunderflow : 1;
    /* csi1remdrscrdunderflow - Bits[13:13], RW1CS, default = 1'b0 
       Port1 Remote Drs Credit Underflow Error
     */
    UINT32 csi1remndrcrdunderflow : 1;
    /* csi1remndrcrdunderflow - Bits[14:14], RW1CS, default = 1'b0 
       Port1 Remote Ndr Credit Underflow Error
     */
    UINT32 csi1remsnpcrdunderflow : 1;
    /* csi1remsnpcrdunderflow - Bits[15:15], RW1CS, default = 1'b0 
       Port1 Remote Snp Credit Underflow Error
     */
    UINT32 csi1remhomcrdunderflow : 1;
    /* csi1remhomcrdunderflow - Bits[16:16], RW1CS, default = 1'b0 
       Port1 Remote Hom Credit Underflow Error
     */
    UINT32 csi1remvnacrdunderflow : 1;
    /* csi1remvnacrdunderflow - Bits[17:17], RW1CS, default = 1'b0 
       Port1 Remote Vna Credit Underflow Error
     */
    UINT32 csi1txqcrdoverflow : 1;
    /* csi1txqcrdoverflow - Bits[18:18], RW1CS, default = 1'b0 
       Port1 TxQ Credit Overflow Error
     */
    UINT32 csi1txqcrdunderflow : 1;
    /* csi1txqcrdunderflow - Bits[19:19], RW1CS, default = 1'b0 
       Port1 TxQ Credit Underflow Error
     */
    UINT32 csi1llresetrxnotidle : 1;
    /* csi1llresetrxnotidle - Bits[20:20], RW1CS, default = 1'b0 
       Port1 link layer Reset while Rx is not Idle Error
     */
    UINT32 csi1llresettxnotidle : 1;
    /* csi1llresettxnotidle - Bits[21:21], RW1CS, default = 1'b0 
       Port1 link layer Reset while Tx is not Idle Error
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 csi1ingrdrsptrinvalid : 1;
    /* csi1ingrdrsptrinvalid - Bits[25:25], RW1CS, default = 1'b0 
       Port1 Ingress Drs Ptr Invalid Error
     */
    UINT32 csi1ingrncbptrinvalid : 1;
    /* csi1ingrncbptrinvalid - Bits[26:26], RW1CS, default = 1'b0 
       Port1 Ingress Ncb Ptr Invalid Error
     */
    UINT32 csi1ingrncsptrinvalid : 1;
    /* csi1ingrncsptrinvalid - Bits[27:27], RW1CS, default = 1'b0 
       Port1 Ingress Ncs Ptr Invalid Error
     */
    UINT32 csi1ingrhomptrinvalid : 1;
    /* csi1ingrhomptrinvalid - Bits[28:28], RW1CS, default = 1'b0 
       Port1 Ingress Hom Ptr Invalid Error
     */
    UINT32 csi1ingrsnpptrinvalid : 1;
    /* csi1ingrsnpptrinvalid - Bits[29:29], RW1CS, default = 1'b0 
       Port1 Ingress Snp Ptr Invalid Error
     */
    UINT32 csi1ingrndrptrinvalid : 1;
    /* csi1ingrndrptrinvalid - Bits[30:30], RW1CS, default = 1'b0 
       Port1 Ingress Ndr Ptr Invalid Error
     */
    UINT32 csi1invalidadarb : 1;
    /* csi1invalidadarb - Bits[31:31], RW1CS, default = 1'b0 
       Port1 Invalid AD Arb Error
     */
  } Bits;
  UINT32 Data;
} R3INGERRLOG1_R3QPI_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* R3INGERRMASK1_R3QPI_CFG_REG supported on:                                  */
/*       IVT_EP (0x4019C0D8)                                                  */
/*       IVT_EX (0x4019C0D8)                                                  */
/*       HSX (0x401580D8)                                                     */
/*       BDX (0x401580D8)                                                     */
/* Register default value:              0x00000000                            */
#define R3INGERRMASK1_R3QPI_CFG_REG 0x050040D8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.0.CFG.xml.
 * R3QPI Ingress Error Mask Register1
 */
typedef union {
  struct {
    UINT32 csi1txqcrdmaxchkmask : 1;
    /* csi1txqcrdmaxchkmask - Bits[0:0], RWS, default = 1'b0 
       Port1 TxQ Credit Max Check Error Mask
     */
    UINT32 csi1ncscrdmaxchkmask : 1;
    /* csi1ncscrdmaxchkmask - Bits[1:1], RWS, default = 1'b0 
       Port1 Ncs Credit Max Check Error Mask
     */
    UINT32 csi1ncbcrdmaxchkmask : 1;
    /* csi1ncbcrdmaxchkmask - Bits[2:2], RWS, default = 1'b0 
       Port1 Ncb Credit Max Check Error Mask
     */
    UINT32 csi1drscrdmaxchkmask : 1;
    /* csi1drscrdmaxchkmask - Bits[3:3], RWS, default = 1'b0 
       Port1 Drs Credit Max Check Error Mask
     */
    UINT32 csi1ndrcrdmaxchkmask : 1;
    /* csi1ndrcrdmaxchkmask - Bits[4:4], RWS, default = 1'b0 
       Port1 Ndr Credit Max Check Error Mask
     */
    UINT32 csi1snpcrdmaxchkmask : 1;
    /* csi1snpcrdmaxchkmask - Bits[5:5], RWS, default = 1'b0 
       Port1 Snp Credit Max Check Error Mask
     */
    UINT32 csi1homcrdmaxchkmask : 1;
    /* csi1homcrdmaxchkmask - Bits[6:6], RWS, default = 1'b0 
       Port1 Hom Credit Max Check Error Mask
     */
    UINT32 csi1vnacrdmaxchkmask : 1;
    /* csi1vnacrdmaxchkmask - Bits[7:7], RWS, default = 1'b0 
       Port1 Vna Credit Max Check Error Mask
     */
    UINT32 csi1blingroverflowmask : 1;
    /* csi1blingroverflowmask - Bits[8:8], RWS, default = 1'b0 
       Port1 BL Ingress Overflow Error Mask
     */
    UINT32 csi1adingroverflowmask : 1;
    /* csi1adingroverflowmask - Bits[9:9], RWS, default = 1'b0 
       Port1 AD Ingress Overflow Error Mask
     */
    UINT32 rsvd_10 : 1;
    /* rsvd_10 - Bits[10:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 csi1remncscrdunderflowmask : 1;
    /* csi1remncscrdunderflowmask - Bits[11:11], RWS, default = 1'b0 
       Port1 Remote Ncs Credit Underflow Error Mask
     */
    UINT32 csi1remncbcrdunderflowmask : 1;
    /* csi1remncbcrdunderflowmask - Bits[12:12], RWS, default = 1'b0 
       Port1 Remote Ncb Credit Underflow Error Mask
     */
    UINT32 csi1remdrscrdunderflowmask : 1;
    /* csi1remdrscrdunderflowmask - Bits[13:13], RWS, default = 1'b0 
       Port1 Remote Drs Credit Underflow Error Mask
     */
    UINT32 csi1remndrcrdunderflowmask : 1;
    /* csi1remndrcrdunderflowmask - Bits[14:14], RWS, default = 1'b0 
       Port1 Remote Ndr Credit Underflow Error Mask
     */
    UINT32 csi1remsnpcrdunderflowmask : 1;
    /* csi1remsnpcrdunderflowmask - Bits[15:15], RWS, default = 1'b0 
       Port1 Remote Snp Credit Underflow Error Mask
     */
    UINT32 csi1remhomcrdunderflowmask : 1;
    /* csi1remhomcrdunderflowmask - Bits[16:16], RWS, default = 1'b0 
       Port1 Remote Hom Credit Underflow Error Mask
     */
    UINT32 csi1remvnacrdunderflowmask : 1;
    /* csi1remvnacrdunderflowmask - Bits[17:17], RWS, default = 1'b0 
       Port1 Remote Vna Credit Underflow Error Mask
     */
    UINT32 csi1txqcrdoverflowmask : 1;
    /* csi1txqcrdoverflowmask - Bits[18:18], RWS, default = 1'b0 
       Port1 TxQ Credit Overflow Error Mask
     */
    UINT32 csi1txqcrdunderflowmask : 1;
    /* csi1txqcrdunderflowmask - Bits[19:19], RWS, default = 1'b0 
       Port1 TxQ Credit Underflow Error Mask
     */
    UINT32 csi1llresetrxnotidlemask : 1;
    /* csi1llresetrxnotidlemask - Bits[20:20], RWS, default = 1'b0 
       Port1 Link Layer Reset while Rx is not Idle Error Mask
     */
    UINT32 csi1llresettxnotidlemask : 1;
    /* csi1llresettxnotidlemask - Bits[21:21], RWS, default = 1'b0 
       Port1 Link Layer Reset while Tx is not Idle Error Mask
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 csi1ingrdrsptrinvalidmask : 1;
    /* csi1ingrdrsptrinvalidmask - Bits[25:25], RWS, default = 1'b0 
       Port1 Ingress Drs Ptr Invalid Error Mask
     */
    UINT32 csi1ingrncbptrinvalidmask : 1;
    /* csi1ingrncbptrinvalidmask - Bits[26:26], RWS, default = 1'b0 
       Port1 Ingress Ncb Ptr Invalid Error Mask
     */
    UINT32 csi1ingrncsptrinvalidmask : 1;
    /* csi1ingrncsptrinvalidmask - Bits[27:27], RWS, default = 1'b0 
       Port1 Ingress Ncs Ptr Invalid Error Mask
     */
    UINT32 csi1ingrhomptrinvalidmask : 1;
    /* csi1ingrhomptrinvalidmask - Bits[28:28], RWS, default = 1'b0 
       Port1 Ingress Hom Ptr Invalid Error Mask
     */
    UINT32 csi1ingrsnpptrinvalidmask : 1;
    /* csi1ingrsnpptrinvalidmask - Bits[29:29], RWS, default = 1'b0 
       Port1 Ingress Snp Ptr Invalid Error Mask
     */
    UINT32 csi1ingrndrptrinvalidmask : 1;
    /* csi1ingrndrptrinvalidmask - Bits[30:30], RWS, default = 1'b0 
       Port1 Ingress Ndr Ptr Invalid Error Mask
     */
    UINT32 csi1invalidadarbmask : 1;
    /* csi1invalidadarbmask - Bits[31:31], RWS, default = 1'b0 
       Port1 Invalid AD Arb Error Mask
     */
  } Bits;
  UINT32 Data;
} R3INGERRMASK1_R3QPI_CFG_STRUCT;
#endif /* ASM_INC */


















/* R3QDEBUG_R3QPI_CFG_REG supported on:                                       */
/*       IVT_EP (0x4019C0FC)                                                  */
/*       IVT_EX (0x4019C0FC)                                                  */
/*       HSX (0x401580FC)                                                     */
/*       BDX (0x401580FC)                                                     */
/* Register default value:              0x00000000                            */
#define R3QDEBUG_R3QPI_CFG_REG 0x050040FC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.0.CFG.xml.
 * R3QPI Debug defeature Regrister
 */
typedef union {
  struct {
    UINT32 srciden : 1;
    /* srciden - Bits[0:0], RW, default = 1'b0 
       Enable encoding source information on Addr[45:41]. When enable, R3QPI will copy 
       TORID[4:0] onto Addr[45:41] 
     */
    UINT32 rsvd : 31;
    /* rsvd - Bits[31:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R3QDEBUG_R3QPI_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* R2IGRSPARECSR_R3QPI_CFG_REG supported on:                                  */
/*       IVT_EP (0x4019C074)                                                  */
/*       IVT_EX (0x4019C074)                                                  */
/* Register default value:              0x00000000                            */
#define R2IGRSPARECSR_R3QPI_CFG_REG 0x05004074



/* R3_GL_ERR_CFG_R3QPI_CFG_REG supported on:                                  */
/*       IVT_EP (0x4019C080)                                                  */
/*       IVT_EX (0x4019C080)                                                  */
/* Register default value:              0x0003FE60                            */
#define R3_GL_ERR_CFG_R3QPI_CFG_REG 0x05004080



/* SRC_THROTTLE_CTL2_R3QPI_CFG_REG supported on:                              */
/*       IVT_EP (0x4019C094)                                                  */
/*       IVT_EX (0x4019C094)                                                  */
/* Register default value:              0x00000000                            */
#define SRC_THROTTLE_CTL2_R3QPI_CFG_REG 0x05004094



/* SRC_THROTTLE_CTL_R3QPI_CFG_REG supported on:                               */
/*       IVT_EP (0x4019C0A8)                                                  */
/*       IVT_EX (0x4019C0A8)                                                  */
/* Register default value:              0x01000380                            */
#define SRC_THROTTLE_CTL_R3QPI_CFG_REG 0x050040A8



/* R3CSI_DBG_BUS_CONTROL_R3QPI_CFG_REG supported on:                          */
/*       IVT_EP (0x1019C0E4)                                                  */
/*       IVT_EX (0x1019C0E4)                                                  */
/* Register default value:              0x04                                  */
#define R3CSI_DBG_BUS_CONTROL_R3QPI_CFG_REG 0x050010E4



/* R3CSI_DBG_BUS_MATCH_N0_R3QPI_CFG_REG supported on:                         */
/*       IVT_EP (0x1019C0E5)                                                  */
/*       IVT_EX (0x1019C0E5)                                                  */
/* Register default value:              0x00                                  */
#define R3CSI_DBG_BUS_MATCH_N0_R3QPI_CFG_REG 0x050010E5



/* R3CSI_DBG_BUS_MATCH_N1_R3QPI_CFG_REG supported on:                         */
/*       IVT_EP (0x1019C0E6)                                                  */
/*       IVT_EX (0x1019C0E6)                                                  */
/* Register default value:              0x00                                  */
#define R3CSI_DBG_BUS_MATCH_N1_R3QPI_CFG_REG 0x050010E6



/* R3CSI_DBG_BUS_MASK_R3QPI_CFG_REG supported on:                             */
/*       IVT_EP (0x2019C0E8)                                                  */
/*       IVT_EX (0x2019C0E8)                                                  */
/* Register default value:              0x0000                                */
#define R3CSI_DBG_BUS_MASK_R3QPI_CFG_REG 0x050020E8



/* R3CSI_ASC_LDVAL_R3QPI_CFG_REG supported on:                                */
/*       IVT_EP (0x4019C0EC)                                                  */
/*       IVT_EX (0x4019C0EC)                                                  */
/* Register default value:              0x00000000                            */
#define R3CSI_ASC_LDVAL_R3QPI_CFG_REG 0x050040EC



/* R3UTLSPARECSR_R3QPI_CFG_REG supported on:                                  */
/*       IVT_EP (0x4019C0F4)                                                  */
/*       IVT_EX (0x4019C0F4)                                                  */
/* Register default value:              0x00000000                            */
#define R3UTLSPARECSR_R3QPI_CFG_REG 0x050040F4



/* R3INGERRLOG_MISC_R3QPI_CFG_REG supported on:                               */
/*       IVT_EP (0x4019C0F8)                                                  */
/*       IVT_EX (0x4019C0F8)                                                  */
/* Register default value:              0x00000000                            */
#define R3INGERRLOG_MISC_R3QPI_CFG_REG 0x050040F8



#endif /* R3QPI_CFG_h */
