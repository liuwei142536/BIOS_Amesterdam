/* Date Stamp: 8/23/2014 */

#ifndef CBOBC_MEMSAD_h
#define CBOBC_MEMSAD_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* CBOBC_MEMSAD_IVT_DEV 22                                                    */
/* CBOBC_MEMSAD_IVT_FUN 0                                                     */
/* For HSX_HOST:                                                              */
/* CBOBC_MEMSAD_HSX_DEV 15                                                    */
/* CBOBC_MEMSAD_HSX_FUN 4                                                     */
/* For BDX_HOST:                                                              */
/* CBOBC_MEMSAD_BDX_DEV 15                                                    */
/* CBOBC_MEMSAD_BDX_FUN 4                                                     */

/* VID_CBOBC_MEMSAD_REG supported on:                                         */
/*       IVT_EP (0x201B0000)                                                  */
/*       IVT_EX (0x201B0000)                                                  */
/*       HSX (0x2017C000)                                                     */
/*       BDX (0x2017C000)                                                     */
/* Register default value:              0x8086                                */
#define VID_CBOBC_MEMSAD_REG 0x01002000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
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
} VID_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DID_CBOBC_MEMSAD_REG supported on:                                         */
/*       IVT_EP (0x201B0002)                                                  */
/*       IVT_EX (0x201B0002)                                                  */
/*       HSX (0x2017C002)                                                     */
/*       BDX (0x2017C002)                                                     */
/* Register default value on IVT_EP:    0x0EC8                                */
/* Register default value on IVT_EX:    0x0EC8                                */
/* Register default value on HSX:       0x2FFC                                */
/* Register default value on BDX:       0x6FFC                                */
#define DID_CBOBC_MEMSAD_REG 0x01002002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6FFC 
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
} DID_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* PCICMD_CBOBC_MEMSAD_REG supported on:                                      */
/*       HSX (0x2017C004)                                                     */
/*       BDX (0x2017C004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_CBOBC_MEMSAD_REG 0x01002004

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
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
} PCICMD_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PCISTS_CBOBC_MEMSAD_REG supported on:                                      */
/*       HSX (0x2017C006)                                                     */
/*       BDX (0x2017C006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_CBOBC_MEMSAD_REG 0x01002006

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
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
} PCISTS_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RID_CBOBC_MEMSAD_REG supported on:                                         */
/*       IVT_EP (0x101B0008)                                                  */
/*       IVT_EX (0x101B0008)                                                  */
/*       HSX (0x1017C008)                                                     */
/*       BDX (0x1017C008)                                                     */
/* Register default value:              0x00                                  */
#define RID_CBOBC_MEMSAD_REG 0x01001008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
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
} RID_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_CBOBC_MEMSAD_REG supported on:                                      */
/*       IVT_EP (0x101B0009)                                                  */
/*       IVT_EX (0x101B0009)                                                  */
/*       HSX (0x1017C009)                                                     */
/*       BDX (0x1017C009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_CBOBC_MEMSAD_REG 0x01001009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.15.4.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_CBOBC_MEMSAD_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_CBOBC_MEMSAD_REG supported on:                                      */
/*       IVT_EP (0x201B000A)                                                  */
/*       IVT_EX (0x201B000A)                                                  */
/*       HSX (0x2017C00A)                                                     */
/*       BDX (0x2017C00A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_CBOBC_MEMSAD_REG 0x0100200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
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
} CCR_N1_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* CLSR_CBOBC_MEMSAD_REG supported on:                                        */
/*       IVT_EP (0x101B000C)                                                  */
/*       IVT_EX (0x101B000C)                                                  */
/*       HSX (0x1017C00C)                                                     */
/*       BDX (0x1017C00C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_CBOBC_MEMSAD_REG 0x0100100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
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
} CLSR_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* PLAT_CBOBC_MEMSAD_REG supported on:                                        */
/*       HSX (0x1017C00D)                                                     */
/*       BDX (0x1017C00D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_CBOBC_MEMSAD_REG 0x0100100D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
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
} PLAT_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* HDR_CBOBC_MEMSAD_REG supported on:                                         */
/*       IVT_EP (0x101B000E)                                                  */
/*       IVT_EX (0x101B000E)                                                  */
/*       HSX (0x1017C00E)                                                     */
/*       BDX (0x1017C00E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_CBOBC_MEMSAD_REG 0x0100100E


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.15.4.CFG.xml.
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
} HDR_CBOBC_MEMSAD_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* BIST_CBOBC_MEMSAD_REG supported on:                                        */
/*       HSX (0x1017C00F)                                                     */
/*       BDX (0x1017C00F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_CBOBC_MEMSAD_REG 0x0100100F

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
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
} BIST_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SVID_CBOBC_MEMSAD_REG supported on:                                        */
/*       IVT_EP (0x201B002C)                                                  */
/*       IVT_EX (0x201B002C)                                                  */
/*       HSX (0x2017C02C)                                                     */
/*       BDX (0x2017C02C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_CBOBC_MEMSAD_REG 0x0100202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
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
} SVID_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* SDID_CBOBC_MEMSAD_REG supported on:                                        */
/*       IVT_EP (0x201B002E)                                                  */
/*       IVT_EX (0x201B002E)                                                  */
/*       HSX (0x2017C02E)                                                     */
/*       BDX (0x2017C02E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_CBOBC_MEMSAD_REG 0x0100202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
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
} SDID_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_CBOBC_MEMSAD_REG supported on:                                      */
/*       HSX (0x1017C034)                                                     */
/*       BDX (0x1017C034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_CBOBC_MEMSAD_REG 0x01001034

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
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
} CAPPTR_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTL_CBOBC_MEMSAD_REG supported on:                                        */
/*       HSX (0x1017C03C)                                                     */
/*       BDX (0x1017C03C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_CBOBC_MEMSAD_REG 0x0100103C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
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
} INTL_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTPIN_CBOBC_MEMSAD_REG supported on:                                      */
/*       HSX (0x1017C03D)                                                     */
/*       BDX (0x1017C03D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_CBOBC_MEMSAD_REG 0x0100103D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
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
} INTPIN_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MINGNT_CBOBC_MEMSAD_REG supported on:                                      */
/*       HSX (0x1017C03E)                                                     */
/*       BDX (0x1017C03E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_CBOBC_MEMSAD_REG 0x0100103E

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
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
} MINGNT_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MAXLAT_CBOBC_MEMSAD_REG supported on:                                      */
/*       HSX (0x1017C03F)                                                     */
/*       BDX (0x1017C03F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_CBOBC_MEMSAD_REG 0x0100103F

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
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
} MAXLAT_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PAM0123_CBOBC_MEMSAD_REG supported on:                                     */
/*       IVT_EP (0x401B0040)                                                  */
/*       IVT_EX (0x401B0040)                                                  */
/*       HSX (0x4017C040)                                                     */
/*       BDX (0x4017C040)                                                     */
/* Register default value:              0x00000000                            */
#define PAM0123_CBOBC_MEMSAD_REG 0x01004040


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.15.4.CFG.xml.
 * PAM0123 Configuration register.  All fields of this register are locked when 
 * PAM0123.Lock is set.  This lock is bypassable with SMM writes. 
 *       
 */
typedef union {
  struct {
    UINT32 lock : 1;
    /* lock - Bits[0:0], RW_LB, default = 2'b00 
       If this bit is set and LTCtrlSts.ConfigLock is set, all fields in PAM0123_CFG 
       and PAM456_CFG are locked.  The lock is bypassed by SMM writes. 
               
     */
    UINT32 rsvd_1 : 3;
    /* rsvd_1 - Bits[3:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pam0_hienable : 2;
    /* pam0_hienable - Bits[5:4], RW_LB, default = 2'b00 
       0F0000-0FFFFF Attribute (HIENABLE) This field controls the steering of read and 
       write cycles that address the BIOS area from 0F0000 to 0FFFFF.  
        00: DRAM Disabled: All accesses are directed to DMI. 
        01: Read Only: All reads are sent to DRAM. All writes are forwarded to DMI. 
        10: Write Only: All writes are send to DRAM. Reads are serviced by DMI. 
        11: Normal DRAM Operation: All reads and writes are serviced by DRAM.
       
       
       Note that PAM0 covers BIOS regions (F0000-FFFFF range). An INIT may send the 
       processor to jump to FFFF0 address. Therefore, it is a good idea to protect PAM0 
       range. Generally, BIOS does not need to change F region after it has locked 
       configuration. 
       
       Other PAM regions are used for shadowing option ROMs. That process happens after 
       config locking and requires changes to these PAM regs.  
     */
    UINT32 rsvd_6 : 2;
    /* rsvd_6 - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pam1_loenable : 2;
    /* pam1_loenable - Bits[9:8], RW_LB, default = 2'b00 
       0C0000-0C3FFF Attribute (LOENABLE) This field controls the steering of read and 
       write cycles that address the BIOS area from 0C0000 to 0C3FFF.  
        00: DRAM Disabled: All accesses are directed to DMI. 
        01: Read Only: All reads are sent to DRAM. All writes are forwarded to DMI. 
        10: Write Only: All writes are send to DRAM. Reads are serviced by DMI. 
        11: Normal DRAM Operation: All reads and writes are serviced by DRAM.
     */
    UINT32 rsvd_10 : 2;
    /* rsvd_10 - Bits[11:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pam1_hienable : 2;
    /* pam1_hienable - Bits[13:12], RW_LB, default = 2'b00 
       0C4000-0C7FFF Attribute (HIENABLE) This field controls the steering of read and 
       write cycles that address the BIOS area from 0C4000 to 0C7FFF.  
        00: DRAM Disabled: All accesses are directed to DMI. 
        01: Read Only: All reads are sent to DRAM. All writes are forwarded to DMI. 
        10: Write Only: All writes are send to DRAM. Reads are serviced by DMI. 
        11: Normal DRAM Operation: All reads and writes are serviced by DRAM.
     */
    UINT32 rsvd_14 : 2;
    /* rsvd_14 - Bits[15:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pam2_loenable : 2;
    /* pam2_loenable - Bits[17:16], RW_LB, default = 2'b00 
       0C8000-0CBFFF Attribute (LOENABLE) This field controls the steering of read and 
       write cycles that address the BIOS area from 0C8000 to 0CBFFF.  
        00: DRAM Disabled: All accesses are directed to DMI. 
        01: Read Only: All reads are sent to DRAM. All writes are forwarded to DMI. 
        10: Write Only: All writes are send to DRAM. Reads are serviced by DMI. 
        11: Normal DRAM Operation: All reads and writes are serviced by DRAM.
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pam2_hienable : 2;
    /* pam2_hienable - Bits[21:20], RW_LB, default = 2'b00 
       0CC000-0CFFFF Attribute (HIENABLE) This field controls the steering of read and 
       write cycles that address the BIOS area from 0CC000 to 0CFFFF.  
        00: DRAM Disabled: All accesses are directed to DMI. 
        01: Read Only: All reads are sent to DRAM. All writes are forwarded to DMI. 
        10: Write Only: All writes are send to DRAM. Reads are serviced by DMI. 
        11: Normal DRAM Operation: All reads and writes are serviced by DRAM.
     */
    UINT32 rsvd_22 : 2;
    /* rsvd_22 - Bits[23:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pam3_loenable : 2;
    /* pam3_loenable - Bits[25:24], RW_LB, default = 2'b00 
       0D0000-0D3FFF Attribute (LOENABLE) This field controls the steering of read and 
       write cycles that address the BIOS area from 0D0000 to 0D3FFF.  
        00: DRAM Disabled: All accesses are directed to DMI. 
        01: Read Only: All reads are sent to DRAM. All writes are forwarded to DMI. 
        10: Write Only: All writes are send to DRAM. Reads are serviced by DMI. 
        11: Normal DRAM Operation: All reads and writes are serviced by DRAM.
     */
    UINT32 rsvd_26 : 2;
    /* rsvd_26 - Bits[27:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pam3_hienable : 2;
    /* pam3_hienable - Bits[29:28], RW_LB, default = 2'b00 
       0D4000-0D7FFF Attribute (HIENABLE) This field controls the steering of read and 
       write cycles that address the BIOS area from 0D4000 to 0D7FFF.  
        00: DRAM Disabled: All accesses are directed to DMI. 
        01: Read Only: All reads are sent to DRAM. All writes are forwarded to DMI. 
        10: Write Only: All writes are send to DRAM. Reads are serviced by DMI. 
        11: Normal DRAM Operation: All reads and writes are serviced by DRAM.
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PAM0123_CBOBC_MEMSAD_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PAM456_CBOBC_MEMSAD_REG supported on:                                      */
/*       IVT_EP (0x401B0044)                                                  */
/*       IVT_EX (0x401B0044)                                                  */
/*       HSX (0x4017C044)                                                     */
/*       BDX (0x4017C044)                                                     */
/* Register default value:              0x00000000                            */
#define PAM456_CBOBC_MEMSAD_REG 0x01004044
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * PAM456 Configuration register.  All fields of this register is locked with both 
 * LTCtrlSts.ConfigLock and PAM0123_CFG.Lock are set.  This lock is bypassable with 
 * SMM writes. 
 *       
 */
typedef union {
  struct {
    UINT32 pam4_loenable : 2;
    /* pam4_loenable - Bits[1:0], RW_LB, default = 2'b00 
       0D8000-0DBFFF Attribute (LOENABLE) This field controls the steering of read and 
       write cycles that address the BIOS area from 0D8000 to 0DBFFF. 00: DRAM 
       Disabled: All accesses are directed to DMI.  
        01: Read Only: All reads are sent to DRAM. All writes are forwarded to DMI. 
        10: Write Only: All writes are send to DRAM. Reads are serviced by DMI. 
        11: Normal DRAM Operation: All reads and writes are serviced by DRAM.
     */
    UINT32 rsvd_2 : 2;
    /* rsvd_2 - Bits[3:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pam4_hienable : 2;
    /* pam4_hienable - Bits[5:4], RW_LB, default = 2'b00 
       0DC000-0DFFFF Attribute (HIENABLE) This field controls the steering of read and 
       write cycles that address the BIOS area from 0DC000 to 0DFFFF. 00: DRAM 
       Disabled: All accesses are directed to DMI.  
        01: Read Only: All reads are sent to DRAM. All writes are forwarded to DMI. 
        10: Write Only: All writes are send to DRAM. Reads are serviced by DMI. 
        11: Normal DRAM Operation: All reads and writes are serviced by DRAM.
     */
    UINT32 rsvd_6 : 2;
    /* rsvd_6 - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pam5_loenable : 2;
    /* pam5_loenable - Bits[9:8], RW_LB, default = 2'b00 
       0E0000-0E3FFF Attribute (LOENABLE) This field controls the steering of read and 
       write cycles that address the BIOS area from 0E0000 to 0E3FFF. 00: DRAM 
       Disabled: All accesses are directed to DMI.  
        01: Read Only: All reads are sent to DRAM. All writes are forwarded to DMI. 
        10: Write Only: All writes are send to DRAM. Reads are serviced by DMI. 
        11: Normal DRAM Operation: All reads and writes are serviced by DRAM.
     */
    UINT32 rsvd_10 : 2;
    /* rsvd_10 - Bits[11:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pam5_hienable : 2;
    /* pam5_hienable - Bits[13:12], RW_LB, default = 2'b00 
       0E4000-0E7FFF Attribute (HIENABLE) This field controls the steering of read and 
       write cycles that address the BIOS area from 0E4000 to 0E7FFF. 00: DRAM 
       Disabled: All accesses are directed to DMI.  
        01: Read Only: All reads are sent to DRAM. All writes are forwarded to DMI. 
        10: Write Only: All writes are send to DRAM. Reads are serviced by DMI. 
        11: Normal DRAM Operation: All reads and writes are serviced by DRAM.
     */
    UINT32 rsvd_14 : 2;
    /* rsvd_14 - Bits[15:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pam6_loenable : 2;
    /* pam6_loenable - Bits[17:16], RW_LB, default = 2'b00 
       0E8000-0EBFFF Attribute (LOENABLE) This field controls the steering of read and 
       write cycles that address the BIOS area from 0E8000 to 0EBFFF. 00: DRAM 
       Disabled: All accesses are directed to DMI.  
        01: Read Only: All reads are sent to DRAM. All writes are forwarded to DMI. 
        10: Write Only: All writes are send to DRAM. Reads are serviced by DMI. 
        11: Normal DRAM Operation: All reads and writes are serviced by DRAM.
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pam6_hienable : 2;
    /* pam6_hienable - Bits[21:20], RW_LB, default = 2'b00 
       0EC000-0EFFFF Attribute (HIENABLE) This field controls the steering of read and 
       write cycles that address the BIOS area from 0EC000 to 0EFFFF. 00: DRAM 
       Disabled: All accesses are directed to DMI.  
        01: Read Only: All reads are sent to DRAM. All writes are forwarded to DMI. 
        10: Write Only: All writes are send to DRAM. Reads are serviced by DMI. 
        11: Normal DRAM Operation: All reads and writes are serviced by DRAM.
     */
    UINT32 rsvd_22 : 10;
    /* rsvd_22 - Bits[31:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PAM456_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* MESEG_BASE_N0_CBOBC_MEMSAD_REG supported on:                               */
/*       IVT_EP (0x401B0050)                                                  */
/*       IVT_EX (0x401B0050)                                                  */
/*       HSX (0x4017C050)                                                     */
/*       BDX (0x4017C050)                                                     */
/* Register default value:              0x00000000                            */
#define MESEG_BASE_N0_CBOBC_MEMSAD_REG 0x01004050
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rsvd : 19;
    /* rsvd - Bits[18:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 mebase : 13;
    /* mebase - Bits[31:19], RW_LB, default = 27'b000000000000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       Corresponds to A[45:19] of the base address memory range that is allocated to 
       the ME. 
     */
  } Bits;
  UINT32 Data;
} MESEG_BASE_N0_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* MESEG_BASE_N1_CBOBC_MEMSAD_REG supported on:                               */
/*       IVT_EP (0x401B0054)                                                  */
/*       IVT_EX (0x401B0054)                                                  */
/*       HSX (0x4017C054)                                                     */
/*       BDX (0x4017C054)                                                     */
/* Register default value:              0x00000000                            */
#define MESEG_BASE_N1_CBOBC_MEMSAD_REG 0x01004054
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 mebase : 14;
    /* mebase - Bits[13:0], RW_LB, default = 27'b000000000000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       Corresponds to A[45:19] of the base address memory range that is allocated to 
       the ME. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MESEG_BASE_N1_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* MESEG_LIMIT_N0_CBOBC_MEMSAD_REG supported on:                              */
/*       IVT_EP (0x401B0058)                                                  */
/*       IVT_EX (0x401B0058)                                                  */
/*       HSX (0x4017C058)                                                     */
/*       BDX (0x4017C058)                                                     */
/* Register default value:              0x00000000                            */
#define MESEG_LIMIT_N0_CBOBC_MEMSAD_REG 0x01004058


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 11;
    /* rsvd_0 - Bits[10:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 en : 1;
    /* en - Bits[11:11], RW_LB, default = 1'b0 
       Indicates whether the ME Stolen Memory range is enabled or not. When enabled, 
       all IA access to this range must be aborted. 
     */
    UINT32 rsvd_12 : 7;
    /* rsvd_12 - Bits[18:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 melimit : 13;
    /* melimit - Bits[31:19], RW_LB, default = 27'b000000000000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       Corresponds to A[45:19] of the limit address memory range that is allocated to 
       the ME. Minimum granularity is 1MB for this region. 
     */
  } Bits;
  UINT32 Data;
} MESEG_LIMIT_N0_CBOBC_MEMSAD_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* MESEG_LIMIT_N1_CBOBC_MEMSAD_REG supported on:                              */
/*       IVT_EP (0x401B005C)                                                  */
/*       IVT_EX (0x401B005C)                                                  */
/*       HSX (0x4017C05C)                                                     */
/*       BDX (0x4017C05C)                                                     */
/* Register default value:              0x00000000                            */
#define MESEG_LIMIT_N1_CBOBC_MEMSAD_REG 0x0100405C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 melimit : 14;
    /* melimit - Bits[13:0], RW_LB, default = 27'b000000000000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       Corresponds to A[45:19] of the limit address memory range that is allocated to 
       the ME. Minimum granularity is 1MB for this region. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MESEG_LIMIT_N1_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_0_CBOBC_MEMSAD_REG supported on:                                 */
/*       HSX (0x4017C060)                                                     */
/*       BDX (0x4017C060)                                                     */
/* Register default value:              0x00000000                            */
#define DRAM_RULE_0_CBOBC_MEMSAD_REG 0x01004060

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_0_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTERLEAVE_LIST_0_CBOBC_MEMSAD_REG supported on:                           */
/*       HSX (0x4017C064)                                                     */
/*       BDX (0x4017C064)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_0_CBOBC_MEMSAD_REG 0x01004064

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_0_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DRAM_RULE_1_CBOBC_MEMSAD_REG supported on:                                 */
/*       IVT_EP (0x401B0068)                                                  */
/*       IVT_EX (0x401B0068)                                                  */
/*       HSX (0x4017C068)                                                     */
/*       BDX (0x4017C068)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_1_CBOBC_MEMSAD_REG 0x01004068
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_1_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_1_CBOBC_MEMSAD_REG supported on:                           */
/*       IVT_EP (0x401B006C)                                                  */
/*       IVT_EX (0x401B006C)                                                  */
/*       HSX (0x4017C06C)                                                     */
/*       BDX (0x4017C06C)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_1_CBOBC_MEMSAD_REG 0x0100406C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_1_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_2_CBOBC_MEMSAD_REG supported on:                                 */
/*       IVT_EP (0x401B0070)                                                  */
/*       IVT_EX (0x401B0070)                                                  */
/*       HSX (0x4017C070)                                                     */
/*       BDX (0x4017C070)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_2_CBOBC_MEMSAD_REG 0x01004070
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_2_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_2_CBOBC_MEMSAD_REG supported on:                           */
/*       IVT_EP (0x401B0074)                                                  */
/*       IVT_EX (0x401B0074)                                                  */
/*       HSX (0x4017C074)                                                     */
/*       BDX (0x4017C074)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_2_CBOBC_MEMSAD_REG 0x01004074
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_2_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_3_CBOBC_MEMSAD_REG supported on:                                 */
/*       IVT_EP (0x401B0078)                                                  */
/*       IVT_EX (0x401B0078)                                                  */
/*       HSX (0x4017C078)                                                     */
/*       BDX (0x4017C078)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_3_CBOBC_MEMSAD_REG 0x01004078
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_3_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_3_CBOBC_MEMSAD_REG supported on:                           */
/*       IVT_EP (0x401B007C)                                                  */
/*       IVT_EX (0x401B007C)                                                  */
/*       HSX (0x4017C07C)                                                     */
/*       BDX (0x4017C07C)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_3_CBOBC_MEMSAD_REG 0x0100407C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_3_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_4_CBOBC_MEMSAD_REG supported on:                                 */
/*       IVT_EP (0x401B0080)                                                  */
/*       IVT_EX (0x401B0080)                                                  */
/*       HSX (0x4017C080)                                                     */
/*       BDX (0x4017C080)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_4_CBOBC_MEMSAD_REG 0x01004080
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_4_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_4_CBOBC_MEMSAD_REG supported on:                           */
/*       IVT_EP (0x401B0084)                                                  */
/*       IVT_EX (0x401B0084)                                                  */
/*       HSX (0x4017C084)                                                     */
/*       BDX (0x4017C084)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_4_CBOBC_MEMSAD_REG 0x01004084
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_4_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_5_CBOBC_MEMSAD_REG supported on:                                 */
/*       IVT_EP (0x401B0088)                                                  */
/*       IVT_EX (0x401B0088)                                                  */
/*       HSX (0x4017C088)                                                     */
/*       BDX (0x4017C088)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_5_CBOBC_MEMSAD_REG 0x01004088
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_5_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_5_CBOBC_MEMSAD_REG supported on:                           */
/*       IVT_EP (0x401B008C)                                                  */
/*       IVT_EX (0x401B008C)                                                  */
/*       HSX (0x4017C08C)                                                     */
/*       BDX (0x4017C08C)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_5_CBOBC_MEMSAD_REG 0x0100408C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_5_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_6_CBOBC_MEMSAD_REG supported on:                                 */
/*       IVT_EP (0x401B0090)                                                  */
/*       IVT_EX (0x401B0090)                                                  */
/*       HSX (0x4017C090)                                                     */
/*       BDX (0x4017C090)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_6_CBOBC_MEMSAD_REG 0x01004090
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_6_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_6_CBOBC_MEMSAD_REG supported on:                           */
/*       IVT_EP (0x401B0094)                                                  */
/*       IVT_EX (0x401B0094)                                                  */
/*       HSX (0x4017C094)                                                     */
/*       BDX (0x4017C094)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_6_CBOBC_MEMSAD_REG 0x01004094
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_6_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_7_CBOBC_MEMSAD_REG supported on:                                 */
/*       IVT_EP (0x401B0098)                                                  */
/*       IVT_EX (0x401B0098)                                                  */
/*       HSX (0x4017C098)                                                     */
/*       BDX (0x4017C098)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_7_CBOBC_MEMSAD_REG 0x01004098
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_7_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_7_CBOBC_MEMSAD_REG supported on:                           */
/*       IVT_EP (0x401B009C)                                                  */
/*       IVT_EX (0x401B009C)                                                  */
/*       HSX (0x4017C09C)                                                     */
/*       BDX (0x4017C09C)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_7_CBOBC_MEMSAD_REG 0x0100409C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_7_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_8_CBOBC_MEMSAD_REG supported on:                                 */
/*       IVT_EP (0x401B00A0)                                                  */
/*       IVT_EX (0x401B00A0)                                                  */
/*       HSX (0x4017C0A0)                                                     */
/*       BDX (0x4017C0A0)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_8_CBOBC_MEMSAD_REG 0x010040A0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_8_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_8_CBOBC_MEMSAD_REG supported on:                           */
/*       IVT_EP (0x401B00A4)                                                  */
/*       IVT_EX (0x401B00A4)                                                  */
/*       HSX (0x4017C0A4)                                                     */
/*       BDX (0x4017C0A4)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_8_CBOBC_MEMSAD_REG 0x010040A4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_8_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_9_CBOBC_MEMSAD_REG supported on:                                 */
/*       IVT_EP (0x401B00A8)                                                  */
/*       IVT_EX (0x401B00A8)                                                  */
/*       HSX (0x4017C0A8)                                                     */
/*       BDX (0x4017C0A8)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_9_CBOBC_MEMSAD_REG 0x010040A8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_9_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_9_CBOBC_MEMSAD_REG supported on:                           */
/*       IVT_EP (0x401B00AC)                                                  */
/*       IVT_EX (0x401B00AC)                                                  */
/*       HSX (0x4017C0AC)                                                     */
/*       BDX (0x4017C0AC)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_9_CBOBC_MEMSAD_REG 0x010040AC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_9_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_10_CBOBC_MEMSAD_REG supported on:                                */
/*       IVT_EP (0x401B00B0)                                                  */
/*       IVT_EX (0x401B00B0)                                                  */
/*       HSX (0x4017C0B0)                                                     */
/*       BDX (0x4017C0B0)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_10_CBOBC_MEMSAD_REG 0x010040B0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_10_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_10_CBOBC_MEMSAD_REG supported on:                          */
/*       IVT_EP (0x401B00B4)                                                  */
/*       IVT_EX (0x401B00B4)                                                  */
/*       HSX (0x4017C0B4)                                                     */
/*       BDX (0x4017C0B4)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_10_CBOBC_MEMSAD_REG 0x010040B4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_10_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_11_CBOBC_MEMSAD_REG supported on:                                */
/*       IVT_EP (0x401B00B8)                                                  */
/*       IVT_EX (0x401B00B8)                                                  */
/*       HSX (0x4017C0B8)                                                     */
/*       BDX (0x4017C0B8)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_11_CBOBC_MEMSAD_REG 0x010040B8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_11_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_11_CBOBC_MEMSAD_REG supported on:                          */
/*       IVT_EP (0x401B00BC)                                                  */
/*       IVT_EX (0x401B00BC)                                                  */
/*       HSX (0x4017C0BC)                                                     */
/*       BDX (0x4017C0BC)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_11_CBOBC_MEMSAD_REG 0x010040BC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_11_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_12_CBOBC_MEMSAD_REG supported on:                                */
/*       IVT_EP (0x401B00C0)                                                  */
/*       IVT_EX (0x401B00C0)                                                  */
/*       HSX (0x4017C0C0)                                                     */
/*       BDX (0x4017C0C0)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_12_CBOBC_MEMSAD_REG 0x010040C0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_12_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_12_CBOBC_MEMSAD_REG supported on:                          */
/*       IVT_EP (0x401B00C4)                                                  */
/*       IVT_EX (0x401B00C4)                                                  */
/*       HSX (0x4017C0C4)                                                     */
/*       BDX (0x4017C0C4)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_12_CBOBC_MEMSAD_REG 0x010040C4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_12_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_13_CBOBC_MEMSAD_REG supported on:                                */
/*       IVT_EP (0x401B00C8)                                                  */
/*       IVT_EX (0x401B00C8)                                                  */
/*       HSX (0x4017C0C8)                                                     */
/*       BDX (0x4017C0C8)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_13_CBOBC_MEMSAD_REG 0x010040C8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_13_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_13_CBOBC_MEMSAD_REG supported on:                          */
/*       IVT_EP (0x401B00CC)                                                  */
/*       IVT_EX (0x401B00CC)                                                  */
/*       HSX (0x4017C0CC)                                                     */
/*       BDX (0x4017C0CC)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_13_CBOBC_MEMSAD_REG 0x010040CC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_13_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_14_CBOBC_MEMSAD_REG supported on:                                */
/*       IVT_EP (0x401B00D0)                                                  */
/*       IVT_EX (0x401B00D0)                                                  */
/*       HSX (0x4017C0D0)                                                     */
/*       BDX (0x4017C0D0)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_14_CBOBC_MEMSAD_REG 0x010040D0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_14_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_14_CBOBC_MEMSAD_REG supported on:                          */
/*       IVT_EP (0x401B00D4)                                                  */
/*       IVT_EX (0x401B00D4)                                                  */
/*       HSX (0x4017C0D4)                                                     */
/*       BDX (0x4017C0D4)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_14_CBOBC_MEMSAD_REG 0x010040D4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_14_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_15_CBOBC_MEMSAD_REG supported on:                                */
/*       IVT_EP (0x401B00D8)                                                  */
/*       IVT_EX (0x401B00D8)                                                  */
/*       HSX (0x4017C0D8)                                                     */
/*       BDX (0x4017C0D8)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_15_CBOBC_MEMSAD_REG 0x010040D8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_15_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_15_CBOBC_MEMSAD_REG supported on:                          */
/*       IVT_EP (0x401B00DC)                                                  */
/*       IVT_EX (0x401B00DC)                                                  */
/*       HSX (0x4017C0DC)                                                     */
/*       BDX (0x4017C0DC)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_15_CBOBC_MEMSAD_REG 0x010040DC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_15_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_16_CBOBC_MEMSAD_REG supported on:                                */
/*       IVT_EP (0x401B00E0)                                                  */
/*       IVT_EX (0x401B00E0)                                                  */
/*       HSX (0x4017C0E0)                                                     */
/*       BDX (0x4017C0E0)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_16_CBOBC_MEMSAD_REG 0x010040E0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_16_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_16_CBOBC_MEMSAD_REG supported on:                          */
/*       IVT_EP (0x401B00E4)                                                  */
/*       IVT_EX (0x401B00E4)                                                  */
/*       HSX (0x4017C0E4)                                                     */
/*       BDX (0x4017C0E4)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_16_CBOBC_MEMSAD_REG 0x010040E4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_16_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_17_CBOBC_MEMSAD_REG supported on:                                */
/*       IVT_EP (0x401B00E8)                                                  */
/*       IVT_EX (0x401B00E8)                                                  */
/*       HSX (0x4017C0E8)                                                     */
/*       BDX (0x4017C0E8)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_17_CBOBC_MEMSAD_REG 0x010040E8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_17_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_17_CBOBC_MEMSAD_REG supported on:                          */
/*       IVT_EP (0x401B00EC)                                                  */
/*       IVT_EX (0x401B00EC)                                                  */
/*       HSX (0x4017C0EC)                                                     */
/*       BDX (0x4017C0EC)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_17_CBOBC_MEMSAD_REG 0x010040EC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_17_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_18_CBOBC_MEMSAD_REG supported on:                                */
/*       IVT_EP (0x401B00F0)                                                  */
/*       IVT_EX (0x401B00F0)                                                  */
/*       HSX (0x4017C0F0)                                                     */
/*       BDX (0x4017C0F0)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_18_CBOBC_MEMSAD_REG 0x010040F0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_18_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_18_CBOBC_MEMSAD_REG supported on:                          */
/*       IVT_EP (0x401B00F4)                                                  */
/*       IVT_EX (0x401B00F4)                                                  */
/*       HSX (0x4017C0F4)                                                     */
/*       BDX (0x4017C0F4)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_18_CBOBC_MEMSAD_REG 0x010040F4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_18_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* DRAM_RULE_19_CBOBC_MEMSAD_REG supported on:                                */
/*       IVT_EP (0x401B00F8)                                                  */
/*       IVT_EX (0x401B00F8)                                                  */
/*       HSX (0x4017C0F8)                                                     */
/*       BDX (0x4017C0F8)                                                     */
/* Register default value on IVT_EP:    0x00000002                            */
/* Register default value on IVT_EX:    0x00000002                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define DRAM_RULE_19_CBOBC_MEMSAD_REG 0x010040F8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 * Cbo coherency configuration control
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this DRAM rule.
     */
    UINT32 interleave_mode : 1;
    /* interleave_mode - Bits[1:1], RW_LB, default = 1'b0 
       DRAM rule interleave mode. If a dram_rule hits a 3 bit number is used to index 
       into the corresponding interleave_list to determine which package the DRAM 
       belongs to. This mode selects how that number is computed.  
       1: Address bits {8,7,6}. 
       0: Address bits {8,7,6} XORed with {18,17,16}.
     */
    UINT32 attr : 2;
    /* attr - Bits[3:2], RW_LB, default = 2'b00 
       00 - DRAM, 01 - MMCFG
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limit : 20;
    /* limit - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       This correspond to Addr[45:26] of the DRAM rule top limit address. Must be 
       strictly greater than previous rule, even if this rule is disabled, unless this 
       rule and all following rules are disabled. Lower limit is the previous rule (or 
       0 if this is the first rule) 
     */
    UINT32 a7mode : 1;
    /* a7mode - Bits[26:26], RW_LB, default = 1'b0 
       Swap PA6 with PA9 in the XOR logic that selects the interleave list
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_RULE_19_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */


/* INTERLEAVE_LIST_19_CBOBC_MEMSAD_REG supported on:                          */
/*       IVT_EP (0x401B00FC)                                                  */
/*       IVT_EX (0x401B00FC)                                                  */
/*       HSX (0x4017C0FC)                                                     */
/*       BDX (0x4017C0FC)                                                     */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_19_CBOBC_MEMSAD_REG 0x010040FC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.15.4.CFG.xml.
 */
typedef union {
  struct {
    UINT32 package0 : 4;
    /* package0 - Bits[3:0], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package1 : 4;
    /* package1 - Bits[7:4], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package2 : 4;
    /* package2 - Bits[11:8], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package3 : 4;
    /* package3 - Bits[15:12], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package4 : 4;
    /* package4 - Bits[19:16], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package5 : 4;
    /* package5 - Bits[23:20], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package6 : 4;
    /* package6 - Bits[27:24], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
    UINT32 package7 : 4;
    /* package7 - Bits[31:28], RW_LB, default = 4'b0000 
       NodeID of the Interleave List target.
     */
  } Bits;
  UINT32 Data;
} INTERLEAVE_LIST_19_CBOBC_MEMSAD_STRUCT;
#endif /* ASM_INC */




/* SMRAMC_CBOBC_MEMSAD_REG supported on:                                      */
/*       IVT_EP (0x401B004C)                                                  */
/*       IVT_EX (0x401B004C)                                                  */
/* Register default value:              0x00000002                            */
#define SMRAMC_CBOBC_MEMSAD_REG 0x0100404C



/* DRAM_RULE_CBOBC_MEMSAD_REG supported on:                                   */
/*       IVT_EP (0x401B0060)                                                  */
/*       IVT_EX (0x401B0060)                                                  */
/* Register default value:              0x00000002                            */
#define DRAM_RULE_CBOBC_MEMSAD_REG 0x01004060



/* INTERLEAVE_LIST_CBOBC_MEMSAD_REG supported on:                             */
/*       IVT_EP (0x401B0064)                                                  */
/*       IVT_EX (0x401B0064)                                                  */
/* Register default value:              0x00000000                            */
#define INTERLEAVE_LIST_CBOBC_MEMSAD_REG 0x01004064



#endif /* CBOBC_MEMSAD_h */
