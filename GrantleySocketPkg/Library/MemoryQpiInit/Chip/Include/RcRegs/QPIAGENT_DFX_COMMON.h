/* Date Stamp: 8/23/2014 */

#ifndef QPIAGENT_DFX_COMMON_h
#define QPIAGENT_DFX_COMMON_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For all target CPUs:                                                       */
/* QPIAGENT_DFX_COMMON_DEV 8                                                  */
/* QPIAGENT_DFX_COMMON_FUN 7                                                  */

/* VID_QPIAGENT_DFX_COMMON_REG supported on:                                  */
/*       IVT_EP (0x20147000)                                                  */
/*       IVT_EX (0x20147000)                                                  */
/*       HSX (0x20147000)                                                     */
/*       BDX (0x20147000)                                                     */
/* Register default value:              0x8086                                */
#define VID_QPIAGENT_DFX_COMMON_REG 0x06062000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
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
} VID_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */


/* DID_QPIAGENT_DFX_COMMON_REG supported on:                                  */
/*       IVT_EP (0x20147002)                                                  */
/*       IVT_EX (0x20147002)                                                  */
/*       HSX (0x20147002)                                                     */
/*       BDX (0x20147002)                                                     */
/* Register default value on IVT_EP:    0x0E87                                */
/* Register default value on IVT_EX:    0x0E87                                */
/* Register default value on HSX:       0x2F87                                */
/* Register default value on BDX:       0x6F87                                */
#define DID_QPIAGENT_DFX_COMMON_REG 0x06062002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6F87 
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
} DID_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */


/* PCICMD_QPIAGENT_DFX_COMMON_REG supported on:                               */
/*       IVT_EP (0x20147004)                                                  */
/*       IVT_EX (0x20147004)                                                  */
/*       HSX (0x20147004)                                                     */
/*       BDX (0x20147004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_QPIAGENT_DFX_COMMON_REG 0x06062004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
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
} PCICMD_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */


/* PCISTS_QPIAGENT_DFX_COMMON_REG supported on:                               */
/*       IVT_EP (0x20147006)                                                  */
/*       IVT_EX (0x20147006)                                                  */
/*       HSX (0x20147006)                                                     */
/*       BDX (0x20147006)                                                     */
/* Register default value:              0x0010                                */
#define PCISTS_QPIAGENT_DFX_COMMON_REG 0x06062006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
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
    /* capabilities_list - Bits[4:4], RO, default = 1'h1 
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
} PCISTS_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_QPIAGENT_DFX_COMMON_REG supported on:                                  */
/*       IVT_EP (0x10147008)                                                  */
/*       IVT_EX (0x10147008)                                                  */
/*       HSX (0x10147008)                                                     */
/*       BDX (0x10147008)                                                     */
/* Register default value:              0x00                                  */
#define RID_QPIAGENT_DFX_COMMON_REG 0x06061008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
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
} RID_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_QPIAGENT_DFX_COMMON_REG supported on:                               */
/*       IVT_EP (0x10147009)                                                  */
/*       IVT_EX (0x10147009)                                                  */
/*       HSX (0x10147009)                                                     */
/*       BDX (0x10147009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_QPIAGENT_DFX_COMMON_REG 0x06061009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_QPIAGENT_DFX_COMMON_REG supported on:                               */
/*       IVT_EP (0x2014700A)                                                  */
/*       IVT_EX (0x2014700A)                                                  */
/*       HSX (0x2014700A)                                                     */
/*       BDX (0x2014700A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_QPIAGENT_DFX_COMMON_REG 0x0606200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
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
} CCR_N1_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */


/* CLSR_QPIAGENT_DFX_COMMON_REG supported on:                                 */
/*       IVT_EP (0x1014700C)                                                  */
/*       IVT_EX (0x1014700C)                                                  */
/*       HSX (0x1014700C)                                                     */
/*       BDX (0x1014700C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_QPIAGENT_DFX_COMMON_REG 0x0606100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
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
} CLSR_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */


/* PLAT_QPIAGENT_DFX_COMMON_REG supported on:                                 */
/*       IVT_EP (0x1014700D)                                                  */
/*       IVT_EX (0x1014700D)                                                  */
/*       HSX (0x1014700D)                                                     */
/*       BDX (0x1014700D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_QPIAGENT_DFX_COMMON_REG 0x0606100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
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
} PLAT_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */


/* HDR_QPIAGENT_DFX_COMMON_REG supported on:                                  */
/*       IVT_EP (0x1014700E)                                                  */
/*       IVT_EX (0x1014700E)                                                  */
/*       HSX (0x1014700E)                                                     */
/*       BDX (0x1014700E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_QPIAGENT_DFX_COMMON_REG 0x0606100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
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
} HDR_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */


/* BIST_QPIAGENT_DFX_COMMON_REG supported on:                                 */
/*       IVT_EP (0x1014700F)                                                  */
/*       IVT_EX (0x1014700F)                                                  */
/*       HSX (0x1014700F)                                                     */
/*       BDX (0x1014700F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_QPIAGENT_DFX_COMMON_REG 0x0606100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
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
} BIST_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */






/* CAPPTR_QPIAGENT_DFX_COMMON_REG supported on:                               */
/*       IVT_EP (0x10147034)                                                  */
/*       IVT_EX (0x10147034)                                                  */
/*       HSX (0x10147034)                                                     */
/*       BDX (0x10147034)                                                     */
/* Register default value on IVT_EP:    0x40                                  */
/* Register default value on IVT_EX:    0x40                                  */
/* Register default value on HSX:       0x00                                  */
/* Register default value on BDX:       0x40                                  */
#define CAPPTR_QPIAGENT_DFX_COMMON_REG 0x06061034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
 * PCI Capability Pointer Register
 */
typedef union {
  struct {
    UINT8 capability_pointer : 8;
    /* capability_pointer - Bits[7:0], RO, default = 8'h40 
       Points to the first capability structure for the device which is the PCIe 
       capability. 
     */
  } Bits;
  UINT8 Data;
} CAPPTR_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */


/* INTL_QPIAGENT_DFX_COMMON_REG supported on:                                 */
/*       IVT_EP (0x1014703C)                                                  */
/*       IVT_EX (0x1014703C)                                                  */
/*       HSX (0x1014703C)                                                     */
/*       BDX (0x1014703C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_QPIAGENT_DFX_COMMON_REG 0x0606103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
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
} INTL_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */


/* INTPIN_QPIAGENT_DFX_COMMON_REG supported on:                               */
/*       IVT_EP (0x1014703D)                                                  */
/*       IVT_EX (0x1014703D)                                                  */
/*       HSX (0x1014703D)                                                     */
/*       BDX (0x1014703D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_QPIAGENT_DFX_COMMON_REG 0x0606103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
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
} INTPIN_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */


/* MINGNT_QPIAGENT_DFX_COMMON_REG supported on:                               */
/*       IVT_EP (0x1014703E)                                                  */
/*       IVT_EX (0x1014703E)                                                  */
/*       HSX (0x1014703E)                                                     */
/*       BDX (0x1014703E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_QPIAGENT_DFX_COMMON_REG 0x0606103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
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
} MINGNT_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_QPIAGENT_DFX_COMMON_REG supported on:                               */
/*       IVT_EP (0x1014703F)                                                  */
/*       IVT_EX (0x1014703F)                                                  */
/*       HSX (0x1014703F)                                                     */
/*       BDX (0x1014703F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_QPIAGENT_DFX_COMMON_REG 0x0606103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
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
} MAXLAT_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */












/* QPITRACECAPCTL_QPIAGENT_DFX_COMMON_REG supported on:                       */
/*       IVT_EP (0x40147178)                                                  */
/*       IVT_EX (0x40147178)                                                  */
/*       HSX (0x40147178)                                                     */
/*       BDX (0x40147178)                                                     */
/* Register default value:              0x00000000                            */
#define QPITRACECAPCTL_QPIAGENT_DFX_COMMON_REG 0x06064178


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
 * These bits are locked by PRIVDBGLCK bit in QPI_DFX_LCK_CTL uCR.
 *     
 */
typedef union {
  struct {
    UINT32 trace_eye_cap : 1;
    /* trace_eye_cap - Bits[0:0], RW_L, default = 1'b0 
       
             
     */
    UINT32 trace_ignore_zeros : 1;
    /* trace_ignore_zeros - Bits[1:1], RW_L, default = 1'b0 
       
             
     */
    UINT32 csr_enable_trace_capture : 1;
    /* csr_enable_trace_capture - Bits[2:2], RW_L, default = 1'b0 
       
             
     */
    UINT32 rsvd : 29;
    /* rsvd - Bits[31:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPITRACECAPCTL_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIDBGBUSCNTRL0_QPIAGENT_DFX_COMMON_REG supported on:                      */
/*       IVT_EP (0x40147180)                                                  */
/*       IVT_EX (0x40147180)                                                  */
/*       HSX (0x40147180)                                                     */
/*       BDX (0x40147180)                                                     */
/* Register default value:              0x00000000                            */
#define QPIDBGBUSCNTRL0_QPIAGENT_DFX_COMMON_REG 0x06064180


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
 * This register is contolled by lock bit DBGBUSLCK in QPI_DFX_LCK_CNTL register. 
 * The register may be readable with the lock bit set but no writes will take 
 * effect unless the lock bit is set to 0. 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 30;
    /* rsvd_0 - Bits[29:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 maskmatch2dmon : 1;
    /* maskmatch2dmon - Bits[30:30], RWS_L, default = 1'b0 
       
       	0 - mask match to GPIO
       	1 - mask match to Domain_agent_in
             
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIDBGBUSCNTRL0_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIDBGBUSCNTRL1_QPIAGENT_DFX_COMMON_REG supported on:                      */
/*       IVT_EP (0x20147184)                                                  */
/*       IVT_EX (0x20147184)                                                  */
/*       HSX (0x20147184)                                                     */
/*       BDX (0x20147184)                                                     */
/* Register default value on IVT_EP:    0x0001                                */
/* Register default value on IVT_EX:    0x0001                                */
/* Register default value on HSX:       0x0000                                */
/* Register default value on BDX:       0x0000                                */
#define QPIDBGBUSCNTRL1_QPIAGENT_DFX_COMMON_REG 0x06062184


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
 * This register is contolled by lock bit DBGBUSLCK in QPI_DFX_LCK_CNTL register. 
 * The register may be readable with the lock bit set but no writes will take 
 * effect unless the lock bit is set to 0. 
 */
typedef union {
  struct {
    UINT16 rsvd_0 : 8;
    /* rsvd_0 - Bits[7:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 dbssen : 1;
    /* dbssen - Bits[8:8], RWS_L, default = 1'b0 
       
       	This bit will enable capture in snapshot register.
       	0: Snapshot register disable
       	1: Snapshot register enable
             
     */
    UINT16 dbssmode : 1;
    /* dbssmode - Bits[9:9], RWS_L, default = 1'b0 
       
        Setting this bit will put the register in coverage mode. The register bits are 
       updated to 1 when trigger selected by the DBSSTrig is asserted and the 
       corresponding bit on the debug is 1. 
        0: Capture on the first rising edge of the selected trigger. All subsequent 
       triggers are ignored. 
       	1: Capture as long as the trigger is asserted.
             
     */
    UINT16 dbssclr : 1;
    /* dbssclr - Bits[10:10], RWS_L, default = 1'b0 
       
        Setting this bit will clear the snapshot register and the trigger so that 
       another snapshot can occur. 
       	0: Do not clear.
       	1: Clear all.
             
     */
    UINT16 dbsstrig : 3;
    /* dbsstrig - Bits[13:11], RWS_L, default = 3'b0 
       Select the trigger source for the snapshot register.
       	000: MBP_OUT[0]
       	001: MBP_OUT[1]
       	010: Reserved
       	011: Reserved
        111: Immediate trigger. If DBSSMODE is 0 then it will enable one snapshot of 
       the debug bus. If DBSSMODE is 1 then the snpashot register will be continuously 
       updated. 
             
     */
    UINT16 dbssacc : 1;
    /* dbssacc - Bits[14:14], RWS_L, default = 1'b0 
       
       	0 - Normal mode
       	1 - Accumulate mode
             
     */
    UINT16 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT16 Data;
} QPIDBGBUSCNTRL1_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */














/* QPIASC0LDVALHI_QPIAGENT_DFX_COMMON_REG supported on:                       */
/*       HSX (0x401471AC)                                                     */
/*       BDX (0x401471AC)                                                     */
/* Register default value:              0x00000000                            */
#define QPIASC0LDVALHI_QPIAGENT_DFX_COMMON_REG 0x060641AC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
 * This register is contolled by lock bit ASCLCK in QPI_DFX_LCK_CNTL register. The 
 * register may be readable with the lock bit set but no writes will take effect 
 * unless the lock bit is set to 0. 
 */
typedef union {
  struct {
    UINT32 ldhival : 32;
    /* ldhival - Bits[31:0], RWS_L, default = 32'b0 
       This value will be loaded into the counter when the ouput state transitions to 
       1. 
     */
  } Bits;
  UINT32 Data;
} QPIASC0LDVALHI_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* QPIASC0LDVALLO_QPIAGENT_DFX_COMMON_REG supported on:                       */
/*       HSX (0x401471B0)                                                     */
/*       BDX (0x401471B0)                                                     */
/* Register default value:              0x00000000                            */
#define QPIASC0LDVALLO_QPIAGENT_DFX_COMMON_REG 0x060641B0

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
 * This register is contolled by lock bit ASCLCK in QPI_DFX_LCK_CNTL register. The 
 * register may be readable with the lock bit set but no writes will take effect 
 * unless the lock bit is set to 0. 
 */
typedef union {
  struct {
    UINT32 ldlowval : 32;
    /* ldlowval - Bits[31:0], RWS_L, default = 32'b0 
       This value will be loaded into the counter when the ouput state transitions to 
       0. 
     */
  } Bits;
  UINT32 Data;
} QPIASC0LDVALLO_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* QPIASC1LDVALHI_QPIAGENT_DFX_COMMON_REG supported on:                       */
/*       HSX (0x401471B4)                                                     */
/*       BDX (0x401471B4)                                                     */
/* Register default value:              0x00000000                            */
#define QPIASC1LDVALHI_QPIAGENT_DFX_COMMON_REG 0x060641B4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
 * This register is contolled by lock bit ASCLCK in QPI_DFX_LCK_CNTL register. The 
 * register may be readable with the lock bit set but no writes will take effect 
 * unless the lock bit is set to 0. 
 */
typedef union {
  struct {
    UINT32 ldhival : 32;
    /* ldhival - Bits[31:0], RWS_L, default = 32'b0 
       This value will be loaded into the counter when the ouput state transitions to 
       1. 
     */
  } Bits;
  UINT32 Data;
} QPIASC1LDVALHI_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* QPIASC1LDVALLO_QPIAGENT_DFX_COMMON_REG supported on:                       */
/*       HSX (0x401471B8)                                                     */
/*       BDX (0x401471B8)                                                     */
/* Register default value:              0x00000000                            */
#define QPIASC1LDVALLO_QPIAGENT_DFX_COMMON_REG 0x060641B8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
 * This register is contolled by lock bit ASCLCK in QPI_DFX_LCK_CNTL register. The 
 * register may be readable with the lock bit set but no writes will take effect 
 * unless the lock bit is set to 0. 
 */
typedef union {
  struct {
    UINT32 ldlowval : 32;
    /* ldlowval - Bits[31:0], RWS_L, default = 32'b0 
       This value will be loaded into the counter when the ouput state transitions to 
       0. 
     */
  } Bits;
  UINT32 Data;
} QPIASC1LDVALLO_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* QPIMISCDBGMUXSEL_QPIAGENT_DFX_COMMON_REG supported on:                     */
/*       IVT_EP (0x401471C8)                                                  */
/*       IVT_EX (0x401471C8)                                                  */
/*       HSX (0x401471C8)                                                     */
/*       BDX (0x401471C8)                                                     */
/* Register default value:              0x00000000                            */
#define QPIMISCDBGMUXSEL_QPIAGENT_DFX_COMMON_REG 0x060641C8


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
 * This selects the signals from various sources within the MISC block. This 
 * register is contolled by lock bit DBGBUSLCK in QPI_DFX_LCK_CNTL register. 
 */
typedef union {
  struct {
    UINT32 dbg_mux_sel_0 : 3;
    /* dbg_mux_sel_0 - Bits[2:0], RWS, default = 2'b0  */
    UINT32 dbg_mux_sel_1 : 3;
    /* dbg_mux_sel_1 - Bits[5:3], RWS, default = 2'b0  */
    UINT32 dbg_mux_sel_2 : 3;
    /* dbg_mux_sel_2 - Bits[8:6], RWS, default = 2'b0  */
    UINT32 dbg_mux_sel_3 : 3;
    /* dbg_mux_sel_3 - Bits[11:9], RWS, default = 2'b0  */
    UINT32 dbg_mux_sel_4 : 3;
    /* dbg_mux_sel_4 - Bits[14:12], RWS, default = 2'b0  */
    UINT32 dbg_mux_sel_5 : 3;
    /* dbg_mux_sel_5 - Bits[17:15], RWS, default = 2'b0  */
    UINT32 dbg_mux_sel_6 : 3;
    /* dbg_mux_sel_6 - Bits[20:18], RWS, default = 2'b0  */
    UINT32 dbg_mux_sel_7 : 3;
    /* dbg_mux_sel_7 - Bits[23:21], RWS, default = 2'b0  */
    UINT32 dbg_mux_sel_8 : 3;
    /* dbg_mux_sel_8 - Bits[26:24], RWS, default = 2'b0  */
    UINT32 rsvd : 5;
    /* rsvd - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIMISCDBGMUXSEL_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */












/* QPILKDFXCTL_QPIAGENT_DFX_COMMON_REG supported on:                          */
/*       IVT_EP (0x201471DC)                                                  */
/*       IVT_EX (0x201471DC)                                                  */
/*       HSX (0x201471DC)                                                     */
/*       BDX (0x201471DC)                                                     */
/* Register default value on IVT_EP:    0x0000                                */
/* Register default value on IVT_EX:    0x0000                                */
/* Register default value on HSX:       0x0080                                */
/* Register default value on BDX:       0x0080                                */
#define QPILKDFXCTL_QPIAGENT_DFX_COMMON_REG 0x060621DC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
 * generated by critter 08_7_0x1dc
 */
typedef union {
  struct {
    UINT16 rsvd_0 : 4;
    /* rsvd_0 - Bits[3:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 rbt_initialized_mode : 1;
    /* rbt_initialized_mode - Bits[4:4], RWS_L, default = 1'b0 
        
               Mode Controls how RBT parity bit is used: 
       	0-parity 
       	1-Initialized Data 
        Initialized Data mode is used to track which RBT entries have been written. 
       This is used to debug BIOS (or hardware) to make sure that anytime  
        an RBT is read that it has recieve a prioir write to initailize the entry. See 
       QPIRBTSD for more info on how this mode is used. 
             
     */
    UINT16 rsvd_5 : 2;
    /* rsvd_5 - Bits[6:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 iodfxlck : 1;
    /* iodfxlck - Bits[7:7], RWS_LB, default = 1'b1 
       This locks access to all non architected UNIPHY control registers.
     */
    UINT16 rsvd_8 : 8;
    /* rsvd_8 - Bits[15:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT16 Data;
} QPILKDFXCTL_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */






/* CC_BIAS_MISC_QPIAGENT_DFX_COMMON_REG supported on:                         */
/*       IVT_EP (0x40147204)                                                  */
/*       IVT_EX (0x40147204)                                                  */
/*       HSX (0x40147204)                                                     */
/*       BDX (0x40147204)                                                     */
/* Register default value:              0x00000000                            */
#define CC_BIAS_MISC_QPIAGENT_DFX_COMMON_REG 0x06064204


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 20;
    /* rsvd_0 - Bits[19:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 dft_iref_ctl : 5;
    /* dft_iref_ctl - Bits[24:20], RWS_L, default = 5'b0  */
    UINT32 rsvd_25 : 7;
    /* rsvd_25 - Bits[31:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CC_BIAS_MISC_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CC_BIAS_RX_QPIAGENT_DFX_COMMON_REG supported on:                           */
/*       IVT_EP (0x40147208)                                                  */
/*       IVT_EX (0x40147208)                                                  */
/*       HSX (0x40147208)                                                     */
/*       BDX (0x40147208)                                                     */
/* Register default value on IVT_EP:    0x08000010                            */
/* Register default value on IVT_EX:    0x08000010                            */
/* Register default value on HSX:       0x08000010                            */
/* Register default value on BDX:       0x05000010                            */
#define CC_BIAS_RX_QPIAGENT_DFX_COMMON_REG 0x06064208


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 4;
    /* rsvd_0 - Bits[3:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rx_ibias_scale : 2;
    /* rx_ibias_scale - Bits[5:4], RWS_L, default = 2'b1  */
    UINT32 rsvd_6 : 18;
    /* rsvd_6 - Bits[23:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rx_incm_sel : 6;
    /* rx_incm_sel - Bits[29:24], RWS_L, default = 6'h8 (HSX), 6'h5 (BDX)  */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CC_BIAS_RX_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */








/* CC_DFX_MISC_QPIAGENT_DFX_COMMON_REG supported on:                          */
/*       IVT_EP (0x40147214)                                                  */
/*       IVT_EX (0x40147214)                                                  */
/*       HSX (0x40147214)                                                     */
/*       BDX (0x40147214)                                                     */
/* Register default value:              0x00078000                            */
#define CC_DFX_MISC_QPIAGENT_DFX_COMMON_REG 0x06064214


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
 */
typedef union {
  struct {
    UINT32 hvm_enable : 1;
    /* hvm_enable - Bits[0:0], RWS_L, default = 1'b0  */
    UINT32 burn_in_en : 1;
    /* burn_in_en - Bits[1:1], RWS_L, default = 1'b0  */
    UINT32 rsvd_2 : 12;
    /* rsvd_2 - Bits[13:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tx_vref_hvm : 5;
    /* tx_vref_hvm - Bits[18:14], RWS_L, default = 5'h1E  */
    UINT32 rsvd_19 : 2;
    /* rsvd_19 - Bits[20:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pi_tst : 3;
    /* pi_tst - Bits[23:21], RWS_L, default = 3'b0  */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CC_DFX_MISC_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CC_DFX_MON0_QPIAGENT_DFX_COMMON_REG supported on:                          */
/*       IVT_EP (0x40147218)                                                  */
/*       IVT_EX (0x40147218)                                                  */
/*       HSX (0x40147218)                                                     */
/*       BDX (0x40147218)                                                     */
/* Register default value on IVT_EP:    0x01000001                            */
/* Register default value on IVT_EX:    0x01000001                            */
/* Register default value on HSX:       0x00900001                            */
/* Register default value on BDX:       0x00900001                            */
#define CC_DFX_MON0_QPIAGENT_DFX_COMMON_REG 0x06064218


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
 */
typedef union {
  struct {
    UINT32 dmon_sel : 7;
    /* dmon_sel - Bits[6:0], RWS_L, default = 7'b1  */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 dfxp_sel : 6;
    /* dfxp_sel - Bits[13:8], RWS_L, default = 6'b0  */
    UINT32 rsvd_14 : 2;
    /* rsvd_14 - Bits[15:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 dfxp0_function_sel : 2;
    /* dfxp0_function_sel - Bits[17:16], RWS_L, default = 2'b0  */
    UINT32 dfxp1_function_sel : 2;
    /* dfxp1_function_sel - Bits[19:18], RWS_L, default = 2'b0  */
    UINT32 dfxp0_dmon_module_sel : 3;
    /* dfxp0_dmon_module_sel - Bits[22:20], RWS_L, default = 3'b1  */
    UINT32 dfxp1_dmon_module_sel : 3;
    /* dfxp1_dmon_module_sel - Bits[25:23], RWS_L, default = 3'b1  */
    UINT32 dfxp0_module_sel : 2;
    /* dfxp0_module_sel - Bits[27:26], RWS_L, default = 2'b0  */
    UINT32 dfxp1_module_sel : 2;
    /* dfxp1_module_sel - Bits[29:28], RWS_L, default = 2'b0  */
    UINT32 dfx_lane_sel : 2;
    /* dfx_lane_sel - Bits[31:30], RWS_L, default = 2'b0  */
  } Bits;
  UINT32 Data;
} CC_DFX_MON0_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CC_DFX_MON1_QPIAGENT_DFX_COMMON_REG supported on:                          */
/*       IVT_EP (0x4014721C)                                                  */
/*       IVT_EX (0x4014721C)                                                  */
/*       HSX (0x4014721C)                                                     */
/*       BDX (0x4014721C)                                                     */
/* Register default value:              0x00000000                            */
#define CC_DFX_MON1_QPIAGENT_DFX_COMMON_REG 0x0606421C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
 */
typedef union {
  struct {
    UINT32 trace_sel : 6;
    /* trace_sel - Bits[5:0], RWS_L, default = 6'b0  */
    UINT32 rsvd_6 : 2;
    /* rsvd_6 - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 trace_data_module_sel : 2;
    /* trace_data_module_sel - Bits[9:8], RWS_L, default = 2'b0  */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 eyemon_samplecnt : 4;
    /* eyemon_samplecnt - Bits[19:16], RWS_L, default = 4'b0  */
    UINT32 eyemon_mode : 2;
    /* eyemon_mode - Bits[21:20], RWS_L, default = 2'b0  */
    UINT32 rsvd_22 : 2;
    /* rsvd_22 - Bits[23:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 dmon2_sel : 7;
    /* dmon2_sel - Bits[30:24], RWS_L, default = 7'b0  */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CC_DFX_MON1_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */






/* CC_ICOMP_QPIAGENT_DFX_COMMON_REG supported on:                             */
/*       IVT_EP (0x40147228)                                                  */
/*       IVT_EX (0x40147228)                                                  */
/*       HSX (0x40147228)                                                     */
/*       BDX (0x40147228)                                                     */
/* Register default value:              0x00000001                            */
#define CC_ICOMP_QPIAGENT_DFX_COMMON_REG 0x06064228
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
 */
typedef union {
  struct {
    UINT32 tx_icomp_rc_time : 2;
    /* tx_icomp_rc_time - Bits[1:0], RWS_L, default = 2'b1  */
    UINT32 rsvd_2 : 2;
    /* rsvd_2 - Bits[3:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tx_icomp_gain_sel : 4;
    /* tx_icomp_gain_sel - Bits[7:4], RWS_L, default = 4'b0  */
    UINT32 tx_icomp_votes : 2;
    /* tx_icomp_votes - Bits[9:8], RWS_L, default = 2'b0  */
    UINT32 rsvd_10 : 22;
    /* rsvd_10 - Bits[31:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CC_ICOMP_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */


/* CC_RCOMP_QPIAGENT_DFX_COMMON_REG supported on:                             */
/*       IVT_EP (0x4014722C)                                                  */
/*       IVT_EX (0x4014722C)                                                  */
/*       HSX (0x4014722C)                                                     */
/*       BDX (0x4014722C)                                                     */
/* Register default value:              0x00000000                            */
#define CC_RCOMP_QPIAGENT_DFX_COMMON_REG 0x0606422C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rx_rt_contcal_en : 1;
    /* rx_rt_contcal_en - Bits[3:3], RWS_L, default = 1'b0  */
    UINT32 rsvd_4 : 28;
    /* rsvd_4 - Bits[31:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CC_RCOMP_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CC_MISC_CTL_QPIAGENT_DFX_COMMON_REG supported on:                          */
/*       IVT_EP (0x40147230)                                                  */
/*       IVT_EX (0x40147230)                                                  */
/*       HSX (0x40147230)                                                     */
/*       BDX (0x40147230)                                                     */
/* Register default value on IVT_EP:    0x10000000                            */
/* Register default value on IVT_EX:    0x10000000                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define CC_MISC_CTL_QPIAGENT_DFX_COMMON_REG 0x06064230


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rsvd : 31;
    /* rsvd - Bits[30:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 reg_override_en : 1;
    /* reg_override_en - Bits[31:31], RWS_L, default = 1'b0  */
  } Bits;
  UINT32 Data;
} CC_MISC_CTL_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CC_RX_CTL_QPIAGENT_DFX_COMMON_REG supported on:                            */
/*       IVT_EP (0x40147234)                                                  */
/*       IVT_EX (0x40147234)                                                  */
/*       HSX (0x40147234)                                                     */
/*       BDX (0x40147234)                                                     */
/* Register default value on IVT_EP:    0x00630711                            */
/* Register default value on IVT_EX:    0x00630711                            */
/* Register default value on HSX:       0x00630411                            */
/* Register default value on BDX:       0x00630411                            */
#define CC_RX_CTL_QPIAGENT_DFX_COMMON_REG 0x06064234


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rx_dc_ac : 1;
    /* rx_dc_ac - Bits[0:0], RWS_L, default = 1'b1  */
    UINT32 rsvd_1 : 3;
    /* rsvd_1 - Bits[3:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rx_ckdly_ctl : 2;
    /* rx_ckdly_ctl - Bits[5:4], RWS_L, default = 2'b1  */
    UINT32 rsvd_6 : 4;
    /* rsvd_6 - Bits[9:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 sq_en_dc : 1;
    /* sq_en_dc - Bits[10:10], RWS_L, default = 1'b1  */
    UINT32 sq_bonus : 1;
    /* sq_bonus - Bits[11:11], RWS_L, default = 1'b0  */
    UINT32 rsvd_12 : 4;
    /* rsvd_12 - Bits[15:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rx_ctle_ctl : 8;
    /* rx_ctle_ctl - Bits[23:16], RWS_L, default = 8'h63  */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CC_RX_CTL_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CC_TX_CTL_QPIAGENT_DFX_COMMON_REG supported on:                            */
/*       IVT_EP (0x40147238)                                                  */
/*       IVT_EX (0x40147238)                                                  */
/*       HSX (0x40147238)                                                     */
/*       BDX (0x40147238)                                                     */
/* Register default value on IVT_EP:    0x00000112                            */
/* Register default value on IVT_EX:    0x00000112                            */
/* Register default value on HSX:       0x00000010                            */
/* Register default value on BDX:       0x00000010                            */
#define CC_TX_CTL_QPIAGENT_DFX_COMMON_REG 0x06064238


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
 */
typedef union {
  struct {
    UINT32 tx_rxdet_ctl : 2;
    /* tx_rxdet_ctl - Bits[1:0], RWS_L, default = 2'b0  */
    UINT32 rsvd_2 : 2;
    /* rsvd_2 - Bits[3:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 txfifo_depth : 2;
    /* txfifo_depth - Bits[5:4], RWS_L, default = 2'b1  */
    UINT32 rsvd_6 : 6;
    /* rsvd_6 - Bits[11:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 dcc_ctl : 3;
    /* dcc_ctl - Bits[14:12], RWS_L, default = 3'b0  */
    UINT32 rsvd_15 : 17;
    /* rsvd_15 - Bits[31:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CC_TX_CTL_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CC_PWR_QPIAGENT_DFX_COMMON_REG supported on:                               */
/*       IVT_EP (0x4014723C)                                                  */
/*       IVT_EX (0x4014723C)                                                  */
/*       HSX (0x4014723C)                                                     */
/*       BDX (0x4014723C)                                                     */
/* Register default value:              0x00000000                            */
#define CC_PWR_QPIAGENT_DFX_COMMON_REG 0x0606423C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rx_pwr_cfg : 9;
    /* rx_pwr_cfg - Bits[8:0], RWS_L, default = 9'b0  */
    UINT32 rsvd_9 : 7;
    /* rsvd_9 - Bits[15:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tx_pwr_cfg : 6;
    /* tx_pwr_cfg - Bits[21:16], RWS_L, default = 6'b0  */
    UINT32 rsvd_22 : 9;
    /* rsvd_22 - Bits[30:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pwr_debug_mode : 1;
    /* pwr_debug_mode - Bits[31:31], RWS_L, default = 1'b0  */
  } Bits;
  UINT32 Data;
} CC_PWR_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */


/* CC_MISC_CTL_1_QPIAGENT_DFX_COMMON_REG supported on:                        */
/*       IVT_EP (0x40147244)                                                  */
/*       IVT_EX (0x40147244)                                                  */
/*       HSX (0x40147244)                                                     */
/*       BDX (0x40147244)                                                     */
/* Register default value on IVT_EP:    0x00360000                            */
/* Register default value on IVT_EX:    0x00360000                            */
/* Register default value on HSX:       0x00360100                            */
/* Register default value on BDX:       0x00360100                            */
#define CC_MISC_CTL_1_QPIAGENT_DFX_COMMON_REG 0x06064244


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 8;
    /* rsvd_0 - Bits[7:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 icomp_track : 1;
    /* icomp_track - Bits[8:8], RWS_L, default = 1'b1  */
    UINT32 rsvd_9 : 6;
    /* rsvd_9 - Bits[14:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 blnclegs_ctl : 6;
    /* blnclegs_ctl - Bits[20:15], RWS_L, default = 6'h2C  */
    UINT32 blnclegs_dis : 1;
    /* blnclegs_dis - Bits[21:21], RWS_L, default = 1'b1  */
    UINT32 rsvd_22 : 2;
    /* rsvd_22 - Bits[23:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 hvm_mode : 8;
    /* hvm_mode - Bits[31:24], RWS_L, default = 8'b0  */
  } Bits;
  UINT32 Data;
} CC_MISC_CTL_1_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CC_MISC_CTL_2_QPIAGENT_DFX_COMMON_REG supported on:                        */
/*       IVT_EP (0x40147248)                                                  */
/*       IVT_EX (0x40147248)                                                  */
/*       HSX (0x40147248)                                                     */
/*       BDX (0x40147248)                                                     */
/* Register default value:              0x00000200                            */
#define CC_MISC_CTL_2_QPIAGENT_DFX_COMMON_REG 0x06064248


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
 */
typedef union {
  struct {
    UINT32 tx_cascdisable : 1;
    /* tx_cascdisable - Bits[0:0], RWS_L, default = 1'b0  */
    UINT32 rsvd_1 : 7;
    /* rsvd_1 - Bits[7:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rt_onehot : 1;
    /* rt_onehot - Bits[8:8], RWS_L, default = 1'b0  */
    UINT32 rxincmgen_pulldn : 2;
    /* rxincmgen_pulldn - Bits[10:9], RWS_L, default = 2'b1  */
    UINT32 bclk_buf_powerdown : 1;
    /* bclk_buf_powerdown - Bits[11:11], RWS_L, default = 1'b0  */
    UINT32 rsvd_12 : 20;
    /* rsvd_12 - Bits[31:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CC_MISC_CTL_2_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */






/* DFX_NTL_CTL_QPIAGENT_DFX_COMMON_REG supported on:                          */
/*       IVT_EP (0x40147260)                                                  */
/*       IVT_EX (0x40147260)                                                  */
/*       HSX (0x40147260)                                                     */
/*       BDX (0x40147260)                                                     */
/* Register default value:              0x00000000                            */
#define DFX_NTL_CTL_QPIAGENT_DFX_COMMON_REG 0x06064260


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.7.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 8;
    /* rsvd_0 - Bits[7:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ntl_capt_mode : 2;
    /* ntl_capt_mode - Bits[9:8], RWS_L, default = 2'b0  */
    UINT32 rsvd_10 : 2;
    /* rsvd_10 - Bits[11:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ntl_comp_all : 1;
    /* ntl_comp_all - Bits[12:12], RO_V, default = 1'b0  */
    UINT32 rsvd_13 : 19;
    /* rsvd_13 - Bits[31:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DFX_NTL_CTL_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */










/* CC_TXDCC_FWDC_QPIAGENT_DFX_COMMON_REG supported on:                        */
/*       BDX (0x40147270)                                                     */
/* Register default value:              0x00000000                            */
#define CC_TXDCC_FWDC_QPIAGENT_DFX_COMMON_REG 0x06064270

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.7.CFG.xml.
 */
typedef union {
  struct {
    UINT32 txdcc_calstatic : 1;
    /* txdcc_calstatic - Bits[0:0], RWS_L, default = 1'b0 
       Tx DCC Calibration static enable
     */
    UINT32 txdcc_gainsel : 3;
    /* txdcc_gainsel - Bits[3:1], RWS_L, default = 2'b00 
       Tx DCC Gain select
     */
    UINT32 txdcc_votes : 2;
    /* txdcc_votes - Bits[5:4], RWS_L, default = 2'b00 
       Tx DCC Votes
     */
    UINT32 sel_vcm_cml2cmos : 2;
    /* sel_vcm_cml2cmos - Bits[7:6], RWS_L, default = 2'b00 
       Select Common Mode for CML2CMOS
     */
    UINT32 txdcc_short_cal : 1;
    /* txdcc_short_cal - Bits[8:8], RWS_L, default = 1'b0 
       Tx DCC short calibration
     */
    UINT32 txdcc_extload_en : 1;
    /* txdcc_extload_en - Bits[9:9], RWS_L, default = 1'b0 
       Tx DCC External Load Enable
     */
    UINT32 txdcc_extload_bin : 5;
    /* txdcc_extload_bin - Bits[14:10], RWS_L, default = 5'h00 
       Tx DCC External Load Binary Value
     */
    UINT32 txdcc_extload_therm : 5;
    /* txdcc_extload_therm - Bits[19:15], RWS_L, default = 5'h00 
       Tx DCC External Load Thermo-code Value
     */
    UINT32 dll_lockdetecten : 1;
    /* dll_lockdetecten - Bits[20:20], RWS_L, default = 1'b0 
       DLL Lock Detect Enable
     */
    UINT32 canary_capen : 2;
    /* canary_capen - Bits[22:21], RWS_L, default = 2'b00 
       Canary Capacitor Enable
     */
    UINT32 rsvd : 9;
    /* rsvd - Bits[31:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CC_TXDCC_FWDC_QPIAGENT_DFX_COMMON_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* SVID_QPIAGENT_DFX_COMMON_REG supported on:                                 */
/*       IVT_EP (0x2014702C)                                                  */
/*       IVT_EX (0x2014702C)                                                  */
/* Register default value:              0x8086                                */
#define SVID_QPIAGENT_DFX_COMMON_REG 0x0606202C



/* SDID_QPIAGENT_DFX_COMMON_REG supported on:                                 */
/*       IVT_EP (0x2014702E)                                                  */
/*       IVT_EX (0x2014702E)                                                  */
/* Register default value:              0x0000                                */
#define SDID_QPIAGENT_DFX_COMMON_REG 0x0606202E



/* PXPCAP_QPIAGENT_DFX_COMMON_REG supported on:                               */
/*       IVT_EP (0x40147040)                                                  */
/*       IVT_EX (0x40147040)                                                  */
/* Register default value:              0x00910010                            */
#define PXPCAP_QPIAGENT_DFX_COMMON_REG 0x06064040



/* PXPENHCAP_QPIAGENT_DFX_COMMON_REG supported on:                            */
/*       IVT_EP (0x40147100)                                                  */
/*       IVT_EX (0x40147100)                                                  */
/* Register default value:              0x00000000                            */
#define PXPENHCAP_QPIAGENT_DFX_COMMON_REG 0x06064100



/* QPITRACERAMCFG0_QPIAGENT_DFX_COMMON_REG supported on:                      */
/*       IVT_EP (0x20147140)                                                  */
/*       IVT_EX (0x20147140)                                                  */
/* Register default value:              0x0000                                */
#define QPITRACERAMCFG0_QPIAGENT_DFX_COMMON_REG 0x06062140



/* QPITRACERAMCFG1_QPIAGENT_DFX_COMMON_REG supported on:                      */
/*       IVT_EP (0x20147142)                                                  */
/*       IVT_EX (0x20147142)                                                  */
/* Register default value:              0x0000                                */
#define QPITRACERAMCFG1_QPIAGENT_DFX_COMMON_REG 0x06062142



/* QPITRACERAMCFG2_QPIAGENT_DFX_COMMON_REG supported on:                      */
/*       IVT_EP (0x20147144)                                                  */
/*       IVT_EX (0x20147144)                                                  */
/* Register default value:              0x0000                                */
#define QPITRACERAMCFG2_QPIAGENT_DFX_COMMON_REG 0x06062144



/* QPITRACEGLOBAL_QPIAGENT_DFX_COMMON_REG supported on:                       */
/*       IVT_EP (0x10147146)                                                  */
/*       IVT_EX (0x10147146)                                                  */
/* Register default value:              0x10                                  */
#define QPITRACEGLOBAL_QPIAGENT_DFX_COMMON_REG 0x06061146



/* QPITRACETRIG0_QPIAGENT_DFX_COMMON_REG supported on:                        */
/*       IVT_EP (0x10147147)                                                  */
/*       IVT_EX (0x10147147)                                                  */
/* Register default value:              0x00                                  */
#define QPITRACETRIG0_QPIAGENT_DFX_COMMON_REG 0x06061147



/* QPITRACETRIG1_QPIAGENT_DFX_COMMON_REG supported on:                        */
/*       IVT_EP (0x10147148)                                                  */
/*       IVT_EX (0x10147148)                                                  */
/* Register default value:              0x00                                  */
#define QPITRACETRIG1_QPIAGENT_DFX_COMMON_REG 0x06061148



/* QPITRACETRIG2_QPIAGENT_DFX_COMMON_REG supported on:                        */
/*       IVT_EP (0x10147149)                                                  */
/*       IVT_EX (0x10147149)                                                  */
/* Register default value:              0x00                                  */
#define QPITRACETRIG2_QPIAGENT_DFX_COMMON_REG 0x06061149



/* QPITRACEMISC_QPIAGENT_DFX_COMMON_REG supported on:                         */
/*       IVT_EP (0x2014714A)                                                  */
/*       IVT_EX (0x2014714A)                                                  */
/* Register default value:              0x000F                                */
#define QPITRACEMISC_QPIAGENT_DFX_COMMON_REG 0x0606214A



/* QPITRACEMASK0_QPIAGENT_DFX_COMMON_REG supported on:                        */
/*       IVT_EP (0x4014714C)                                                  */
/*       IVT_EX (0x4014714C)                                                  */
/* Register default value:              0x00000000                            */
#define QPITRACEMASK0_QPIAGENT_DFX_COMMON_REG 0x0606414C



/* QPITRACEMASK1_QPIAGENT_DFX_COMMON_REG supported on:                        */
/*       IVT_EP (0x40147150)                                                  */
/*       IVT_EX (0x40147150)                                                  */
/* Register default value:              0x00000000                            */
#define QPITRACEMASK1_QPIAGENT_DFX_COMMON_REG 0x06064150



/* QPITRACEMASK2_QPIAGENT_DFX_COMMON_REG supported on:                        */
/*       IVT_EP (0x40147154)                                                  */
/*       IVT_EX (0x40147154)                                                  */
/* Register default value:              0x00000000                            */
#define QPITRACEMASK2_QPIAGENT_DFX_COMMON_REG 0x06064154



/* QPITRACEMATCH0_QPIAGENT_DFX_COMMON_REG supported on:                       */
/*       IVT_EP (0x40147158)                                                  */
/*       IVT_EX (0x40147158)                                                  */
/* Register default value:              0x00000000                            */
#define QPITRACEMATCH0_QPIAGENT_DFX_COMMON_REG 0x06064158



/* QPITRACEMATCH1_QPIAGENT_DFX_COMMON_REG supported on:                       */
/*       IVT_EP (0x4014715C)                                                  */
/*       IVT_EX (0x4014715C)                                                  */
/* Register default value:              0x00000000                            */
#define QPITRACEMATCH1_QPIAGENT_DFX_COMMON_REG 0x0606415C



/* QPITRACEMATCH2_QPIAGENT_DFX_COMMON_REG supported on:                       */
/*       IVT_EP (0x40147160)                                                  */
/*       IVT_EX (0x40147160)                                                  */
/* Register default value:              0x00000000                            */
#define QPITRACEMATCH2_QPIAGENT_DFX_COMMON_REG 0x06064160



/* QPITRACEDUMPCTRL_QPIAGENT_DFX_COMMON_REG supported on:                     */
/*       IVT_EP (0x40147164)                                                  */
/*       IVT_EX (0x40147164)                                                  */
/* Register default value:              0x00000000                            */
#define QPITRACEDUMPCTRL_QPIAGENT_DFX_COMMON_REG 0x06064164



/* QPITRACEDUMPDATA_QPIAGENT_DFX_COMMON_REG supported on:                     */
/*       IVT_EP (0x40147168)                                                  */
/*       IVT_EX (0x40147168)                                                  */
/* Register default value:              0x00000000                            */
#define QPITRACEDUMPDATA_QPIAGENT_DFX_COMMON_REG 0x06064168



/* QPITRACEHALT_QPIAGENT_DFX_COMMON_REG supported on:                         */
/*       IVT_EP (0x4014717C)                                                  */
/*       IVT_EX (0x4014717C)                                                  */
/* Register default value:              0x000000CA                            */
#define QPITRACEHALT_QPIAGENT_DFX_COMMON_REG 0x0606417C



/* QPIDBGBUSCNTRL2_QPIAGENT_DFX_COMMON_REG supported on:                      */
/*       IVT_EP (0x20147186)                                                  */
/*       IVT_EX (0x20147186)                                                  */
/* Register default value:              0x0000                                */
#define QPIDBGBUSCNTRL2_QPIAGENT_DFX_COMMON_REG 0x06062186



/* QPIDBGBUSCNTRL3_QPIAGENT_DFX_COMMON_REG supported on:                      */
/*       IVT_EP (0x40147188)                                                  */
/*       IVT_EX (0x40147188)                                                  */
/* Register default value:              0x00000000                            */
#define QPIDBGBUSCNTRL3_QPIAGENT_DFX_COMMON_REG 0x06064188



/* QPIDBGBUSMATCHLOW_QPIAGENT_DFX_COMMON_REG supported on:                    */
/*       IVT_EP (0x40147198)                                                  */
/*       IVT_EX (0x40147198)                                                  */
/* Register default value:              0x00000000                            */
#define QPIDBGBUSMATCHLOW_QPIAGENT_DFX_COMMON_REG 0x06064198



/* QPIDBGBUSMATCHMID_QPIAGENT_DFX_COMMON_REG supported on:                    */
/*       IVT_EP (0x4014719C)                                                  */
/*       IVT_EX (0x4014719C)                                                  */
/* Register default value:              0x00000000                            */
#define QPIDBGBUSMATCHMID_QPIAGENT_DFX_COMMON_REG 0x0606419C



/* QPIDBGBUSMATCHHIGH_QPIAGENT_DFX_COMMON_REG supported on:                   */
/*       IVT_EP (0x101471A0)                                                  */
/*       IVT_EX (0x101471A0)                                                  */
/* Register default value:              0x00                                  */
#define QPIDBGBUSMATCHHIGH_QPIAGENT_DFX_COMMON_REG 0x060611A0



/* QPIDBGBUSMASKLOW_QPIAGENT_DFX_COMMON_REG supported on:                     */
/*       IVT_EP (0x401471A4)                                                  */
/*       IVT_EX (0x401471A4)                                                  */
/* Register default value:              0x00000000                            */
#define QPIDBGBUSMASKLOW_QPIAGENT_DFX_COMMON_REG 0x060641A4



/* QPIDBGBUSMASKMID_QPIAGENT_DFX_COMMON_REG supported on:                     */
/*       IVT_EP (0x401471A8)                                                  */
/*       IVT_EX (0x401471A8)                                                  */
/* Register default value:              0x00000000                            */
#define QPIDBGBUSMASKMID_QPIAGENT_DFX_COMMON_REG 0x060641A8



/* QPIDBGBUSMASKHIGH_QPIAGENT_DFX_COMMON_REG supported on:                    */
/*       IVT_EP (0x101471AC)                                                  */
/*       IVT_EX (0x101471AC)                                                  */
/* Register default value:              0x00                                  */
#define QPIDBGBUSMASKHIGH_QPIAGENT_DFX_COMMON_REG 0x060611AC



/* QPIASC0LDVAL_QPIAGENT_DFX_COMMON_REG supported on:                         */
/*       IVT_EP (0x401471B4)                                                  */
/*       IVT_EX (0x401471B4)                                                  */
/* Register default value:              0x00000000                            */
#define QPIASC0LDVAL_QPIAGENT_DFX_COMMON_REG 0x060641B4



/* QPIASC1LDVAL_QPIAGENT_DFX_COMMON_REG supported on:                         */
/*       IVT_EP (0x401471B8)                                                  */
/*       IVT_EX (0x401471B8)                                                  */
/* Register default value:              0x00000000                            */
#define QPIASC1LDVAL_QPIAGENT_DFX_COMMON_REG 0x060641B8



/* QPIMISCRNG_QPIAGENT_DFX_COMMON_REG supported on:                           */
/*       IVT_EP (0x401471C0)                                                  */
/*       IVT_EX (0x401471C0)                                                  */
/* Register default value:              0x036DB6DB                            */
#define QPIMISCRNG_QPIAGENT_DFX_COMMON_REG 0x060641C0



/* QPIMISCDELS_QPIAGENT_DFX_COMMON_REG supported on:                          */
/*       IVT_EP (0x401471C4)                                                  */
/*       IVT_EX (0x401471C4)                                                  */
/* Register default value:              0x00000000                            */
#define QPIMISCDELS_QPIAGENT_DFX_COMMON_REG 0x060641C4



/* CC_BANDGAP_QPIAGENT_DFX_COMMON_REG supported on:                           */
/*       IVT_EP (0x40147200)                                                  */
/*       IVT_EX (0x40147200)                                                  */
/* Register default value:              0x00000000                            */
#define CC_BANDGAP_QPIAGENT_DFX_COMMON_REG 0x06064200



/* CC_COMP_STS_QPIAGENT_DFX_COMMON_REG supported on:                          */
/*       IVT_EP (0x40147224)                                                  */
/*       IVT_EX (0x40147224)                                                  */
/* Register default value:              0x00000000                            */
#define CC_COMP_STS_QPIAGENT_DFX_COMMON_REG 0x06064224



/* CC_PLL_MISC_QPIAGENT_DFX_COMMON_REG supported on:                          */
/*       IVT_EP (0x4014724C)                                                  */
/*       IVT_EX (0x4014724C)                                                  */
/* Register default value:              0x00000000                            */
#define CC_PLL_MISC_QPIAGENT_DFX_COMMON_REG 0x0606424C



#endif /* QPIAGENT_DFX_COMMON_h */
