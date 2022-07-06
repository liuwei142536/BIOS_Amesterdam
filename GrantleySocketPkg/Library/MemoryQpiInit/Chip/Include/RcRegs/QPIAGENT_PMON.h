/* Date Stamp: 8/23/2014 */

#ifndef QPIAGENT_PMON_h
#define QPIAGENT_PMON_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For all target CPUs:                                                       */
/* QPIAGENT_PMON_DEV 8                                                        */
/* QPIAGENT_PMON_FUN 2                                                        */

/* VID_QPIAGENT_PMON_REG supported on:                                        */
/*       IVT_EP (0x20142000)                                                  */
/*       IVT_EX (0x20142000)                                                  */
/*       HSX (0x20142000)                                                     */
/*       BDX (0x20142000)                                                     */
/* Register default value:              0x8086                                */
#define VID_QPIAGENT_PMON_REG 0x06012000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
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
} VID_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* DID_QPIAGENT_PMON_REG supported on:                                        */
/*       IVT_EP (0x20142002)                                                  */
/*       IVT_EX (0x20142002)                                                  */
/*       HSX (0x20142002)                                                     */
/*       BDX (0x20142002)                                                     */
/* Register default value on IVT_EP:    0x0E32                                */
/* Register default value on IVT_EX:    0x0E32                                */
/* Register default value on HSX:       0x2F32                                */
/* Register default value on BDX:       0x6F32                                */
#define DID_QPIAGENT_PMON_REG 0x06012002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6F32 
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
} DID_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* PCICMD_QPIAGENT_PMON_REG supported on:                                     */
/*       IVT_EP (0x20142004)                                                  */
/*       IVT_EX (0x20142004)                                                  */
/*       HSX (0x20142004)                                                     */
/*       BDX (0x20142004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_QPIAGENT_PMON_REG 0x06012004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
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
} PCICMD_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* PCISTS_QPIAGENT_PMON_REG supported on:                                     */
/*       IVT_EP (0x20142006)                                                  */
/*       IVT_EX (0x20142006)                                                  */
/*       HSX (0x20142006)                                                     */
/*       BDX (0x20142006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_QPIAGENT_PMON_REG 0x06012006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.2.CFG.xml.
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
} PCISTS_QPIAGENT_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_QPIAGENT_PMON_REG supported on:                                        */
/*       IVT_EP (0x10142008)                                                  */
/*       IVT_EX (0x10142008)                                                  */
/*       HSX (0x10142008)                                                     */
/*       BDX (0x10142008)                                                     */
/* Register default value:              0x00                                  */
#define RID_QPIAGENT_PMON_REG 0x06011008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
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
} RID_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_QPIAGENT_PMON_REG supported on:                                     */
/*       IVT_EP (0x10142009)                                                  */
/*       IVT_EX (0x10142009)                                                  */
/*       HSX (0x10142009)                                                     */
/*       BDX (0x10142009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_QPIAGENT_PMON_REG 0x06011009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.2.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_QPIAGENT_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_QPIAGENT_PMON_REG supported on:                                     */
/*       IVT_EP (0x2014200A)                                                  */
/*       IVT_EX (0x2014200A)                                                  */
/*       HSX (0x2014200A)                                                     */
/*       BDX (0x2014200A)                                                     */
/* Register default value:              0x1101                                */
#define CCR_N1_QPIAGENT_PMON_REG 0x0601200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
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
} CCR_N1_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* CLSR_QPIAGENT_PMON_REG supported on:                                       */
/*       IVT_EP (0x1014200C)                                                  */
/*       IVT_EX (0x1014200C)                                                  */
/*       HSX (0x1014200C)                                                     */
/*       BDX (0x1014200C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_QPIAGENT_PMON_REG 0x0601100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
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
} CLSR_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* PLAT_QPIAGENT_PMON_REG supported on:                                       */
/*       IVT_EP (0x1014200D)                                                  */
/*       IVT_EX (0x1014200D)                                                  */
/*       HSX (0x1014200D)                                                     */
/*       BDX (0x1014200D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_QPIAGENT_PMON_REG 0x0601100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
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
} PLAT_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* HDR_QPIAGENT_PMON_REG supported on:                                        */
/*       IVT_EP (0x1014200E)                                                  */
/*       IVT_EX (0x1014200E)                                                  */
/*       HSX (0x1014200E)                                                     */
/*       BDX (0x1014200E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_QPIAGENT_PMON_REG 0x0601100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
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
} HDR_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* BIST_QPIAGENT_PMON_REG supported on:                                       */
/*       IVT_EP (0x1014200F)                                                  */
/*       IVT_EX (0x1014200F)                                                  */
/*       HSX (0x1014200F)                                                     */
/*       BDX (0x1014200F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_QPIAGENT_PMON_REG 0x0601100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
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
} BIST_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* SVID_F2_QPIAGENT_PMON_REG supported on:                                    */
/*       HSX (0x2014202C)                                                     */
/*       BDX (0x2014202C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_F2_QPIAGENT_PMON_REG 0x0601202C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * PCIE Subsystem vendor identification number
 */
typedef union {
  struct {
    UINT16 subsystem_vendor_identification_number : 16;
    /* subsystem_vendor_identification_number - Bits[15:0], RW_O, default = 16'h8086 
       The default value specifies Intel but can be set to any value once after reset.
     */
  } Bits;
  UINT16 Data;
} SVID_F2_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SDID_F2_QPIAGENT_PMON_REG supported on:                                    */
/*       HSX (0x2014202E)                                                     */
/*       BDX (0x2014202E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_F2_QPIAGENT_PMON_REG 0x0601202E

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * PCIE Subsystem Device ID
 */
typedef union {
  struct {
    UINT16 subsystem_device_identification_number : 16;
    /* subsystem_device_identification_number - Bits[15:0], RW_O, default = 16'b0 
       Assigned by the subsystem vendor to uniquely identify the subsystem
     */
  } Bits;
  UINT16 Data;
} SDID_F2_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CAPPTR_QPIAGENT_PMON_REG supported on:                                     */
/*       IVT_EP (0x10142034)                                                  */
/*       IVT_EX (0x10142034)                                                  */
/*       HSX (0x10142034)                                                     */
/*       BDX (0x10142034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_QPIAGENT_PMON_REG 0x06011034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
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
} CAPPTR_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* INTL_QPIAGENT_PMON_REG supported on:                                       */
/*       IVT_EP (0x1014203C)                                                  */
/*       IVT_EX (0x1014203C)                                                  */
/*       HSX (0x1014203C)                                                     */
/*       BDX (0x1014203C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_QPIAGENT_PMON_REG 0x0601103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
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
} INTL_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* INTPIN_QPIAGENT_PMON_REG supported on:                                     */
/*       IVT_EP (0x1014203D)                                                  */
/*       IVT_EX (0x1014203D)                                                  */
/*       HSX (0x1014203D)                                                     */
/*       BDX (0x1014203D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_QPIAGENT_PMON_REG 0x0601103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
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
} INTPIN_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* MINGNT_QPIAGENT_PMON_REG supported on:                                     */
/*       IVT_EP (0x1014203E)                                                  */
/*       IVT_EX (0x1014203E)                                                  */
/*       HSX (0x1014203E)                                                     */
/*       BDX (0x1014203E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_QPIAGENT_PMON_REG 0x0601103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
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
} MINGNT_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_QPIAGENT_PMON_REG supported on:                                     */
/*       IVT_EP (0x1014203F)                                                  */
/*       IVT_EX (0x1014203F)                                                  */
/*       HSX (0x1014203F)                                                     */
/*       BDX (0x1014203F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_QPIAGENT_PMON_REG 0x0601103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
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
} MAXLAT_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */










/* QPIPMONCNTR_0_N0_QPIAGENT_PMON_REG supported on:                           */
/*       IVT_EP (0x401420A0)                                                  */
/*       IVT_EX (0x401420A0)                                                  */
/*       HSX (0x401420A0)                                                     */
/*       BDX (0x401420A0)                                                     */
/* Register default value:              0x00000000                            */
#define QPIPMONCNTR_0_N0_QPIAGENT_PMON_REG 0x060140A0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * 
 * Perfmon Counter 0 CSR.
 * This register is a perfmon counter. Software can both read it and write it.
 *     
 */
typedef union {
  struct {
    UINT32 countervalue : 32;
    /* countervalue - Bits[31:0], RW_V, default = 48'b0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
        
       	This is the current value of the counter.
             
     */
  } Bits;
  UINT32 Data;
} QPIPMONCNTR_0_N0_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* QPIPMONCNTR_0_N1_QPIAGENT_PMON_REG supported on:                           */
/*       IVT_EP (0x401420A4)                                                  */
/*       IVT_EX (0x401420A4)                                                  */
/*       HSX (0x401420A4)                                                     */
/*       BDX (0x401420A4)                                                     */
/* Register default value:              0x00000000                            */
#define QPIPMONCNTR_0_N1_QPIAGENT_PMON_REG 0x060140A4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * 
 * Perfmon Counter 0 CSR.
 * This register is a perfmon counter. Software can both read it and write it.
 *     
 */
typedef union {
  struct {
    UINT32 countervalue : 16;
    /* countervalue - Bits[15:0], RW_V, default = 48'b0 
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
} QPIPMONCNTR_0_N1_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* QPIPMONCNTR_1_N0_QPIAGENT_PMON_REG supported on:                           */
/*       IVT_EP (0x401420A8)                                                  */
/*       IVT_EX (0x401420A8)                                                  */
/*       HSX (0x401420A8)                                                     */
/*       BDX (0x401420A8)                                                     */
/* Register default value:              0x00000000                            */
#define QPIPMONCNTR_1_N0_QPIAGENT_PMON_REG 0x060140A8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * 
 * Perfmon Counter 1 CSR.
 * This register is a perfmon counter. Software can both read it and write it.
 *     
 */
typedef union {
  struct {
    UINT32 countervalue : 32;
    /* countervalue - Bits[31:0], RW_V, default = 48'b0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
        
               This is the current value of the counter.
             
     */
  } Bits;
  UINT32 Data;
} QPIPMONCNTR_1_N0_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* QPIPMONCNTR_1_N1_QPIAGENT_PMON_REG supported on:                           */
/*       IVT_EP (0x401420AC)                                                  */
/*       IVT_EX (0x401420AC)                                                  */
/*       HSX (0x401420AC)                                                     */
/*       BDX (0x401420AC)                                                     */
/* Register default value:              0x00000000                            */
#define QPIPMONCNTR_1_N1_QPIAGENT_PMON_REG 0x060140AC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * 
 * Perfmon Counter 1 CSR.
 * This register is a perfmon counter. Software can both read it and write it.
 *     
 */
typedef union {
  struct {
    UINT32 countervalue : 16;
    /* countervalue - Bits[15:0], RW_V, default = 48'b0 
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
} QPIPMONCNTR_1_N1_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* QPIPMONCNTR_2_N0_QPIAGENT_PMON_REG supported on:                           */
/*       IVT_EP (0x401420B0)                                                  */
/*       IVT_EX (0x401420B0)                                                  */
/*       HSX (0x401420B0)                                                     */
/*       BDX (0x401420B0)                                                     */
/* Register default value:              0x00000000                            */
#define QPIPMONCNTR_2_N0_QPIAGENT_PMON_REG 0x060140B0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * 
 * Perfmon Counter 2 CSR.
 * This register is a perfmon counter. Software can both read it and write it.
 *     
 */
typedef union {
  struct {
    UINT32 countervalue : 32;
    /* countervalue - Bits[31:0], RW_V, default = 48'b0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
        
               This is the current value of the counter.
             
     */
  } Bits;
  UINT32 Data;
} QPIPMONCNTR_2_N0_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* QPIPMONCNTR_2_N1_QPIAGENT_PMON_REG supported on:                           */
/*       IVT_EP (0x401420B4)                                                  */
/*       IVT_EX (0x401420B4)                                                  */
/*       HSX (0x401420B4)                                                     */
/*       BDX (0x401420B4)                                                     */
/* Register default value:              0x00000000                            */
#define QPIPMONCNTR_2_N1_QPIAGENT_PMON_REG 0x060140B4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * 
 * Perfmon Counter 2 CSR.
 * This register is a perfmon counter. Software can both read it and write it.
 *     
 */
typedef union {
  struct {
    UINT32 countervalue : 16;
    /* countervalue - Bits[15:0], RW_V, default = 48'b0 
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
} QPIPMONCNTR_2_N1_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* QPIPMONCNTR_3_N0_QPIAGENT_PMON_REG supported on:                           */
/*       IVT_EP (0x401420B8)                                                  */
/*       IVT_EX (0x401420B8)                                                  */
/*       HSX (0x401420B8)                                                     */
/*       BDX (0x401420B8)                                                     */
/* Register default value:              0x00000000                            */
#define QPIPMONCNTR_3_N0_QPIAGENT_PMON_REG 0x060140B8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * 
 * Perfmon Counter 3 CSR.
 * This register is a perfmon counter. Software can both read it and write it.
 *     
 */
typedef union {
  struct {
    UINT32 countervalue : 32;
    /* countervalue - Bits[31:0], RW_V, default = 48'b0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
        
               This is the current value of the counter.
             
     */
  } Bits;
  UINT32 Data;
} QPIPMONCNTR_3_N0_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* QPIPMONCNTR_3_N1_QPIAGENT_PMON_REG supported on:                           */
/*       IVT_EP (0x401420BC)                                                  */
/*       IVT_EX (0x401420BC)                                                  */
/*       HSX (0x401420BC)                                                     */
/*       BDX (0x401420BC)                                                     */
/* Register default value:              0x00000000                            */
#define QPIPMONCNTR_3_N1_QPIAGENT_PMON_REG 0x060140BC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * 
 * Perfmon Counter 3 CSR.
 * This register is a perfmon counter. Software can both read it and write it.
 *     
 */
typedef union {
  struct {
    UINT32 countervalue : 16;
    /* countervalue - Bits[15:0], RW_V, default = 48'b0 
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
} QPIPMONCNTR_3_N1_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* QPIPMONCNTR_4_N0_QPIAGENT_PMON_REG supported on:                           */
/*       IVT_EP (0x401420C0)                                                  */
/*       IVT_EX (0x401420C0)                                                  */
/*       HSX (0x401420C0)                                                     */
/*       BDX (0x401420C0)                                                     */
/* Register default value:              0x00000000                            */
#define QPIPMONCNTR_4_N0_QPIAGENT_PMON_REG 0x060140C0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * 
 * Perfmon Counter 4 CSR.
 * This register is a perfmon counter. Software can both read it and write it.
 *     
 */
typedef union {
  struct {
    UINT32 countervalue : 32;
    /* countervalue - Bits[31:0], RW_V, default = 48'b0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
        
               This is the current value of the counter.
             
     */
  } Bits;
  UINT32 Data;
} QPIPMONCNTR_4_N0_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* QPIPMONCNTR_4_N1_QPIAGENT_PMON_REG supported on:                           */
/*       IVT_EP (0x401420C4)                                                  */
/*       IVT_EX (0x401420C4)                                                  */
/*       HSX (0x401420C4)                                                     */
/*       BDX (0x401420C4)                                                     */
/* Register default value:              0x00000000                            */
#define QPIPMONCNTR_4_N1_QPIAGENT_PMON_REG 0x060140C4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * 
 * Perfmon Counter 4 CSR.
 * This register is a perfmon counter. Software can both read it and write it.
 *     
 */
typedef union {
  struct {
    UINT32 countervalue : 16;
    /* countervalue - Bits[15:0], RW_V, default = 48'b0 
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
} QPIPMONCNTR_4_N1_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* QPILKIDLECNTR_N0_QPIAGENT_PMON_REG supported on:                           */
/*       HSX (0x401420C8)                                                     */
/*       BDX (0x401420C8)                                                     */
/* Register default value:              0x00000000                            */
#define QPILKIDLECNTR_N0_QPIAGENT_PMON_REG 0x060140C8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * 
 * This register counts Rx Idle/Null cycles + L1 cycles.
 *     
 */
typedef union {
  struct {
    UINT32 countervalue : 32;
    /* countervalue - Bits[31:0], RO_V, default = 36'b0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
        
               This is the current value of the counter.
             
     */
  } Bits;
  UINT32 Data;
} QPILKIDLECNTR_N0_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* QPILKIDLECNTR_N1_QPIAGENT_PMON_REG supported on:                           */
/*       HSX (0x401420CC)                                                     */
/*       BDX (0x401420CC)                                                     */
/* Register default value:              0x00000000                            */
#define QPILKIDLECNTR_N1_QPIAGENT_PMON_REG 0x060140CC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * 
 * This register counts Rx Idle/Null cycles + L1 cycles.
 *     
 */
typedef union {
  struct {
    UINT32 countervalue : 4;
    /* countervalue - Bits[3:0], RO_V, default = 36'b0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
        
               This is the current value of the counter.
             
     */
    UINT32 rsvd : 28;
    /* rsvd - Bits[31:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPILKIDLECNTR_N1_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* QPILKLLRCNTR_N0_QPIAGENT_PMON_REG supported on:                            */
/*       HSX (0x401420D0)                                                     */
/*       BDX (0x401420D0)                                                     */
/* Register default value:              0x00000000                            */
#define QPILKLLRCNTR_N0_QPIAGENT_PMON_REG 0x060140D0

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * 
 * This register counts Rx cycle in LLR.
 *     
 */
typedef union {
  struct {
    UINT32 countervalue : 32;
    /* countervalue - Bits[31:0], RO_V, default = 36'b0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
        
               This is the current value of the counter.
             
     */
  } Bits;
  UINT32 Data;
} QPILKLLRCNTR_N0_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* QPILKLLRCNTR_N1_QPIAGENT_PMON_REG supported on:                            */
/*       HSX (0x401420D4)                                                     */
/*       BDX (0x401420D4)                                                     */
/* Register default value:              0x00000000                            */
#define QPILKLLRCNTR_N1_QPIAGENT_PMON_REG 0x060140D4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * 
 * This register counts Rx cycle in LLR.
 *     
 */
typedef union {
  struct {
    UINT32 countervalue : 4;
    /* countervalue - Bits[3:0], RO_V, default = 36'b0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
        
               This is the current value of the counter.
             
     */
    UINT32 rsvd : 28;
    /* rsvd - Bits[31:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPILKLLRCNTR_N1_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* QPIPMONCNTRCFG_0_QPIAGENT_PMON_REG supported on:                           */
/*       IVT_EP (0x401420D8)                                                  */
/*       IVT_EX (0x401420D8)                                                  */
/*       HSX (0x401420D8)                                                     */
/*       BDX (0x401420D8)                                                     */
/* Register default value:              0x00000000                            */
#define QPIPMONCNTRCFG_0_QPIAGENT_PMON_REG 0x060140D8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * 
 * Intel QPI Pmon Counter Config 0 Perfmon Counter Control Register.
 *     
 */
typedef union {
  struct {
    UINT32 eventselect : 8;
    /* eventselect - Bits[7:0], RW_V, default = 8'b0 
        
               This field is used to decode the PerfMon event which is selected. The 
       encodings for each of the valid UnCore PerfMon events can be found in the 
       respective performance monitoring documentation. 
             
     */
    UINT32 unitmask : 8;
    /* unitmask - Bits[15:8], RW_V, default = 8'b0 
        
               This mask selects the sub-events to be selected for creation of the 
       event. The selected sub-events are bitwise OR-ed together to create event. At 
       least one sub-event must be selected otherwise the PerfMon event signals will 
       not ever get asserted. Events with no sub-events listed effectively have only 
       one sub-event -- bit 8 must be set to 1 in this case. 
             
     */
    UINT32 queueoccupancyreset : 1;
    /* queueoccupancyreset - Bits[16:16], WO, default = 1'b0 
        
               This write only bit causes the queue occupancy counter of the PerfMon 
       counter for which this Perf event select register is associated to be cleared to 
       all zeroes when a 1 is written to it. No action is taken when a 0 is written.  
        Note: Since the queue occupancy counters never drop below zero, it is possible 
       for the counters to catch up with the real occupancy of the queue in question 
       when the real occupancy drop to zero.  
             
     */
    UINT32 counterreset : 1;
    /* counterreset - Bits[17:17], WO, default = 1'b0 
        
               When this bit is set, the corresponding counter will be reset to 0. This 
       allows for a quick reset of the counter when changing event encodings. 
               This field can be written to 0x1, but does not self clear. To clear 
       immediately write the bit to 0x0 after writing it to 0x1. 
             
     */
    UINT32 edgedetect : 1;
    /* edgedetect - Bits[18:18], RW_V, default = 1'b0 
        
               Edge Detect allows one to count either 0 to 1 or 1 to 0 transitions of a 
       given event. For example, we have an event that counts the number of cycles in 
       L1 mode in Intel QPI. By using edge detect, one can count the number of times 
       that we entered L1 mode (by detecting the rising edge).  
        Edge detect only works in conjunction with threshholding. This is true even for 
       events that can only increment by 1 in a given cycle (like the L1 example 
       above). In this case, one should set a threshhold of 1. One can also use Edge 
       Detect with queue occupancy events. For example, if one wanted to count the 
       number of times when the TOR occupancy was larger than 5, one would select the 
       TOR occupancy event with a threshold of 5 and set the Edge Detect bit.  
        Edge detect can also be used with the invert. This is generally not 
       particularly useful, as the count of falling edges compared to rising edges will 
       always on differ by 1. 
             
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 overflowenable : 1;
    /* overflowenable - Bits[20:20], RW_V, default = 1'b0 
        
               Setting this bit will enable the counter to send an overflow signal. If 
       this bit is not set, the counter will wrap around when it overflows without 
       triggering anything.  
        If this bit is set and the Unit's configuration register has Overflow enabled, 
       then a signal will be transmitted to the Ubox.  
             
     */
    UINT32 rsvd_21 : 1;
    UINT32 counterenable : 1;
    /* counterenable - Bits[22:22], RW_V, default = 1'b0 
        
               This field is the local enable for the PerfMon Counter. This bit must be 
       asserted in order for the PerfMon counter to begin counting the events selected 
       by the event select, unit mask, and internal bits (see the fields below). There 
       is one bit per PerfMon Counter. Note that if this bit is set to 1 but the Unit 
       Control Registers have determined that counting is disabled, then the counter 
       will not count.  
             
     */
    UINT32 invert : 1;
    /* invert - Bits[23:23], RW_V, default = 1'b0 
        
               This bit indicates how the threshold field will be compared to the 
       incoming event. When 0, the comparison that will be done is threshold greater 
       than or euqall to event. When set to 1, the comparison that will be done is 
       inverted from the case where this bit is set to 0, i.e., threshold less than 
       event. The invert bit only works when Threshhold != 0.  
        So, if one would like to invert a non-occupancy event (like LLC Hit), one needs 
       to set the threshhold to 1.  
             
     */
    UINT32 threshold : 8;
    /* threshold - Bits[31:24], RW_V, default = 8'b0 
        
               This field is compared directly against an incoming event value for 
       events that can increment by 1 or more in a given cycle. Since the widest event 
       from the UnCore is 7bits (queue occupancy), bit 31 is unused. The result of the 
       comparison is effectively a 1 bit wide event, i.e., the counter will be 
       incremented by 1 when the comparison is true (the type of comparison depends on 
       the setting of the invert bit - see bit 23 below) no matter how wide the 
       original event was. When this field is zero, threshold comparison is disabled 
       and the event is passed without modification. 
             
     */
  } Bits;
  UINT32 Data;
} QPIPMONCNTRCFG_0_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* QPIPMONCNTRCFG_1_QPIAGENT_PMON_REG supported on:                           */
/*       IVT_EP (0x401420DC)                                                  */
/*       IVT_EX (0x401420DC)                                                  */
/*       HSX (0x401420DC)                                                     */
/*       BDX (0x401420DC)                                                     */
/* Register default value:              0x00000000                            */
#define QPIPMONCNTRCFG_1_QPIAGENT_PMON_REG 0x060140DC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * 
 * Intel QPI Pmon Counter Config 1 Perfmon Counter Control Register
 *     
 */
typedef union {
  struct {
    UINT32 eventselect : 8;
    /* eventselect - Bits[7:0], RW_V, default = 8'b0 
        
               This field is used to decode the PerfMon event which is selected. The 
       encodings for each of the valid UnCore PerfMon events can be found in the 
       respective performance monitoring documentation. 
             
     */
    UINT32 unitmask : 8;
    /* unitmask - Bits[15:8], RW_V, default = 8'b0 
        
               This mask selects the sub-events to be selected for creation of the 
       event. The selected sub-events are bitwise OR-ed together to create event. At 
       least one sub-event must be selected otherwise the PerfMon event signals will 
       not ever get asserted. Events with no sub-events listed effectively have only 
       one sub-event -- bit 8 must be set to 1 in this case. 
             
     */
    UINT32 queueoccupancyreset : 1;
    /* queueoccupancyreset - Bits[16:16], WO, default = 1'b0 
        
               This write only bit causes the queue occupancy counter of the PerfMon 
       counter for which this Perf event select register is associated to be cleared to 
       all zeroes when a 1 is written to it. No action is taken when a 0 is written. 
       Note: Since the queue occupancy counters never drop below zero, it is possible 
       for the counters to catch up with the real occupancy of the queue in question 
       when the real occupancy drop to zero.  
             
     */
    UINT32 counterreset : 1;
    /* counterreset - Bits[17:17], WO, default = 1'b0 
        
               When this bit is set, the corresponding counter will be reset to 0. This 
       allows for a quick reset of the counter when changing event encodings. 
               This field can be written to 0x1, but does not self clear. To clear 
       immediately write the bit to 0x0 after writing it to 0x1. 
             
     */
    UINT32 edgedetect : 1;
    /* edgedetect - Bits[18:18], RW_V, default = 1'b0 
        
               Edge Detect allows one to count either 0 to 1 or 1 to 0 transitions of a 
       given event. For example, we have an event that counts the number of cycles in 
       L1 mode in Intel QPI.  
        By using edge detect, one can count the number of times that we entered L1 mode 
       (by detecting the rising edge).  
        Edge detect only works in conjunction with threshholding. This is true even for 
       events that can only increment by 1 in a given cycle (like the L1 example 
       above).  
        In this case, one should set a threshhold of 1. One can also use Edge Detect 
       with queue occupancy events. For example, if one wanted to count the number of 
       times when the TOR occupancy was larger than 5, one would selet the TOR 
       occupancy event with a threshold of 5 and set the Edge Detect bit.  
        Edge detect can also be used with the invert. This is generally not 
       particularly useful, as the count of falling edges compared to rising edges will 
       always on differ by 1. 
             
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 overflowenable : 1;
    /* overflowenable - Bits[20:20], RW_V, default = 1'b0 
        
               Setting this bit will enable the counter to send an overflow signal. If 
       this bit is not set, the counter will wrap around when it overflows without 
       triggering anything.  
        If this bit is set and the Unit's configuration register has Overflow enabled, 
       then a signal will be transmitted to the Ubox.  
             
     */
    UINT32 rsvd_21 : 1;
    UINT32 counterenable : 1;
    /* counterenable - Bits[22:22], RW_V, default = 1'b0 
        
               This field is the local enable for the PerfMon Counter. This bit must be 
       asserted in order for the PerfMon counter to begin counting the events selected 
       by the event select, unit mask, and internal bits (see the fields below). There 
       is one bit per PerfMon Counter. Note that if this bit is set to 1 but the Unit 
       Control Registers have determined that counting is disabled, then the counter 
       will not count.  
             
     */
    UINT32 invert : 1;
    /* invert - Bits[23:23], RW_V, default = 1'b0 
        
               This bit indicates how the threshold field will be compared to the 
       incoming event. When 0, the comparison that will be done is threshold greater 
       than or equal to event.  
        When set to 1, the comparison that will be done is inverted from the case where 
       this bit is set to 0, i.e., threshold less than event. The invert bit only works 
       when Threshhold != 0.  
        So, if one would like to invert a non-occupancy event (like LLC Hit), one needs 
       to set the threshhold to 1.  
             
     */
    UINT32 threshold : 8;
    /* threshold - Bits[31:24], RW_V, default = 8'b0 
        
               This field is compared directly against an incoming event value for 
       events that can increment by 1 or more in a given cycle. Since the widest event 
       from the UnCore is 7bits (queue occupancy), bit 31 is unused. The result of the 
       comparison is effectively a 1 bit wide event, i.e., the counter will be 
       incremented by 1 when the comparison is true (the type of comparison depends on 
       the setting of the invert bit - see bit 23 below) no matter how wide the 
       original event was. When this field is zero, threshold comparison is disabled 
       and the event is passed without modification. 
             
     */
  } Bits;
  UINT32 Data;
} QPIPMONCNTRCFG_1_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* QPIPMONCNTRCFG_2_QPIAGENT_PMON_REG supported on:                           */
/*       IVT_EP (0x401420E0)                                                  */
/*       IVT_EX (0x401420E0)                                                  */
/*       HSX (0x401420E0)                                                     */
/*       BDX (0x401420E0)                                                     */
/* Register default value:              0x00000000                            */
#define QPIPMONCNTRCFG_2_QPIAGENT_PMON_REG 0x060140E0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * 
 * Intel QPI Pmon Counter Config 2 Perfmon Counter Control Register
 *     
 */
typedef union {
  struct {
    UINT32 eventselect : 8;
    /* eventselect - Bits[7:0], RW_V, default = 8'b0 
        
               This field is used to decode the PerfMon event which is selected. The 
       encodings for each of the valid UnCore PerfMon events can be found in the 
       respective performance monitoring documentation. 
             
     */
    UINT32 unitmask : 8;
    /* unitmask - Bits[15:8], RW_V, default = 8'b0 
        
               This mask selects the sub-events to be selected for creation of the 
       event. The selected sub-events are bitwise OR-ed together to create event. At 
       least one sub-event must be selected otherwise the PerfMon event signals will 
       not ever get asserted. Events with no sub-events listed effectively have only 
       one sub-event -- bit 8 must be set to 1 in this case. 
             
     */
    UINT32 queueoccupancyreset : 1;
    /* queueoccupancyreset - Bits[16:16], WO, default = 1'b0 
        
               This write only bit causes the queue occupancy counter of the PerfMon 
       counter for which this Perf event select register is associated to be cleared to 
       all zeroes when a 1 is written to it.  
        No action is taken when a 0 is written. Note: Since the queue occupancy 
       counters never drop below zero, it is possible for the counters to catch up with 
       the real occupancy of the queue in question when the real occupancy drop to 
       zero.  
             
     */
    UINT32 counterreset : 1;
    /* counterreset - Bits[17:17], WO, default = 1'b0 
        
               When this bit is set, the corresponding counter will be reset to 0. This 
       allows for a quick reset of the counter when changing event encodings. 
               This field can be written to 0x1, but does not self clear. To clear 
       immediately write the bit to 0x0 after writing it to 0x1. 
             
     */
    UINT32 edgedetect : 1;
    /* edgedetect - Bits[18:18], RW_V, default = 1'b0 
        
               Edge Detect allows one to count either 0 to 1 or 1 to 0 transitions of a 
       given event. For example, we have an event that counts the number of cycles in 
       L1 mode in Intel QPI.  
        By using edge detect, one can count the number of times that we entered L1 mode 
       (by detecting the rising edge).  
        Edge detect only works in conjunction with threshholding. This is true even for 
       events that can only increment by 1 in a given cycle (like the L1 example 
       above).  
        In this case, one should set a threshhold of 1. One can also use Edge Detect 
       with queue occupancy events. For example, if one wanted to count the number of 
       times when the TOR occupancy was larger than 5, one would selet the TOR 
       occupancy event with a threshold of 5 and set the Edge Detect bit.  
        Edge detect can also be used with the invert. This is generally not 
       particularly useful, as the count of falling edges compared to rising edges will 
       always on differ by 1. 
             
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 overflowenable : 1;
    /* overflowenable - Bits[20:20], RW_V, default = 1'b0 
        
               Setting this bit will enable the counter to send an overflow signal. If 
       this bit is not set, the counter will wrap around when it overflows without 
       triggering anything.  
        If this bit is set and the Unit's configuration register has Overflow enabled, 
       then a signal will be transmitted to the Ubox.  
             
     */
    UINT32 rsvd_21 : 1;
    UINT32 counterenable : 1;
    /* counterenable - Bits[22:22], RW_V, default = 1'b0 
        
               This field is the local enable for the PerfMon Counter. This bit must be 
       asserted in order for the PerfMon counter to begin counting the events selected 
       by the event select, unit mask, and internal bits (see the fields below). There 
       is one bit per PerfMon Counter. Note that if this bit is set to 1 but the Unit 
       Control Registers have determined that counting is disabled, then the counter 
       will not count.  
             
     */
    UINT32 invert : 1;
    /* invert - Bits[23:23], RW_V, default = 1'b0 
        
               This bit indicates how the threshold field will be compared to the 
       incoming event. When 0, the comparison that will be done is threshold greater 
       than or equal to event.  
        When set to 1, the comparison that will be done is inverted from the case where 
       this bit is set to 0, i.e., threshold less than event. The invert bit only works 
       when Threshhold != 0.  
        So, if one would like to invert a non-occupancy event (like LLC Hit), one needs 
       to set the threshhold to 1.  
             
     */
    UINT32 threshold : 8;
    /* threshold - Bits[31:24], RW_V, default = 8'b0 
        
               This field is compared directly against an incoming event value for 
       events that can increment by 1 or more in a given cycle. Since the widest event 
       from the UnCore is 7bits (queue occupancy), bit 31 is unused. The result of the 
       comparison is effectively a 1 bit wide event, i.e., the counter will be 
       incremented by 1 when the comparison is true (the type of comparison depends on 
       the setting of the invert bit - see bit 23 below) no matter how wide the 
       original event was. When this field is zero, threshold comparison is disabled 
       and the event is passed without modification. 
             
     */
  } Bits;
  UINT32 Data;
} QPIPMONCNTRCFG_2_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* QPIPMONCNTRCFG_3_QPIAGENT_PMON_REG supported on:                           */
/*       IVT_EP (0x401420E4)                                                  */
/*       IVT_EX (0x401420E4)                                                  */
/*       HSX (0x401420E4)                                                     */
/*       BDX (0x401420E4)                                                     */
/* Register default value:              0x00000000                            */
#define QPIPMONCNTRCFG_3_QPIAGENT_PMON_REG 0x060140E4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * 
 * Intel QPI Pmon Counter Config 3 Perfmon Counter Control Register.
 *     
 */
typedef union {
  struct {
    UINT32 eventselect : 8;
    /* eventselect - Bits[7:0], RW_V, default = 8'b0 
        
               This field is used to decode the PerfMon event which is selected. The 
       encodings for each of the valid UnCore PerfMon events can be found in the 
       respective performance monitoring documentation. 
             
     */
    UINT32 unitmask : 8;
    /* unitmask - Bits[15:8], RW_V, default = 8'b0 
        
               This mask selects the sub-events to be selected for creation of the 
       event. The selected sub-events are bitwise OR-ed together to create event. At 
       least one sub-event must be selected otherwise the PerfMon event signals will 
       not ever get asserted. Events with no sub-events listed effectively have only 
       one sub-event -- bit 8 must be set to 1 in this case. 
             
     */
    UINT32 queueoccupancyreset : 1;
    /* queueoccupancyreset - Bits[16:16], WO, default = 1'b0 
        
               This write only bit causes the queue occupancy counter of the PerfMon 
       counter for which this Perf event select register is associated to be cleared to 
       all zeroes when a 1 is written to it.  
        No action is taken when a 0 is written. Note: Since the queue occupancy 
       counters never drop below zero, it is possible for the counters to catch up with 
       the real occupancy of the queue in question when the real occupancy drop to 
       zero.  
             
     */
    UINT32 counterreset : 1;
    /* counterreset - Bits[17:17], WO, default = 1'b0 
        
               When this bit is set, the corresponding counter will be reset to 0. This 
       allows for a quick reset of the counter when changing event encodings. 
               This field can be written to 0x1, but does not self clear. To clear 
       immediately write the bit to 0x0 after writing it to 0x1. 
             
     */
    UINT32 edgedetect : 1;
    /* edgedetect - Bits[18:18], RW_V, default = 1'b0 
        
               Edge Detect allows one to count either 0 to 1 or 1 to 0 transitions of a 
       given event. For example, we have an event that counts the number of cycles in 
       L1 mode in Intel QPI.  
        By using edge detect, one can count the number of times that we entered L1 mode 
       (by detecting the rising edge).  
        Edge detect only works in conjunction with threshholding. This is true even for 
       events that can only increment by 1 in a given cycle (like the L1 example 
       above).  
        In this case, one should set a threshhold of 1. One can also use Edge Detect 
       with queue occupancy events. For example, if one wanted to count the number of 
       times when the TOR occupancy was larger than 5, one would selet the TOR 
       occupancy event with a threshold of 5 and set the Edge Detect bit.  
        Edge detect can also be used with the invert. This is generally not 
       particularly useful, as the count of falling edges compared to rising edges will 
       always on differ by 1. 
             
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 overflowenable : 1;
    /* overflowenable - Bits[20:20], RW_V, default = 1'b0 
        
               Setting this bit will enable the counter to send an overflow signal. If 
       this bit is not set, the counter will wrap around when it overflows without 
       triggering anything.  
        If this bit is set and the Unit's configuration register has Overflow enabled, 
       then a signal will be transmitted to the Ubox.  
             
     */
    UINT32 rsvd_21 : 1;
    UINT32 counterenable : 1;
    /* counterenable - Bits[22:22], RW_V, default = 1'b0 
        
               This field is the local enable for the PerfMon Counter. This bit must be 
       asserted in order for the PerfMon counter to begin counting the events selected 
       by the event select, unit mask, and internal bits (see the fields below). There 
       is one bit per PerfMon Counter. Note that if this bit is set to 1 but the Unit 
       Control Registers have determined that counting is disabled, then the counter 
       will not count.  
             
     */
    UINT32 invert : 1;
    /* invert - Bits[23:23], RW_V, default = 1'b0 
        
               This bit indicates how the threshold field will be compared to the 
       incoming event. When 0, the comparison that will be done is threshold greater 
       than or equal to event.  
        When set to 1, the comparison that will be done is inverted from the case where 
       this bit is set to 0, i.e., threshold less than event. The invert bit only works 
       when Threshhold != 0.  
        So, if one would like to invert a non-occupancy event (like LLC Hit), one needs 
       to set the threshhold to 1.  
             
     */
    UINT32 threshold : 8;
    /* threshold - Bits[31:24], RW_V, default = 8'b0 
        
               This field is compared directly against an incoming event value for 
       events that can increment by 1 or more in a given cycle. Since the widest event 
       from the UnCore is 7bits (queue occupancy), bit 31 is unused. The result of the 
       comparison is effectively a 1 bit wide event, i.e., the counter will be 
       incremented by 1 when the comparison is true (the type of comparison depends on 
       the setting of the invert bit - see bit 23 below) no matter how wide the 
       original event was. When this field is zero, threshold comparison is disabled 
       and the event is passed without modification. 
             
     */
  } Bits;
  UINT32 Data;
} QPIPMONCNTRCFG_3_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */




/* QPIPMONUNITCTRL_QPIAGENT_PMON_REG supported on:                            */
/*       IVT_EP (0x401420F4)                                                  */
/*       IVT_EX (0x401420F4)                                                  */
/*       HSX (0x401420F4)                                                     */
/*       BDX (0x401420F4)                                                     */
/* Register default value on IVT_EP:    0x00000000                            */
/* Register default value on IVT_EX:    0x00000000                            */
/* Register default value on HSX:       0x00030000                            */
/* Register default value on BDX:       0x00030000                            */
#define QPIPMONUNITCTRL_QPIAGENT_PMON_REG 0x060140F4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * Intel QPI Pmon Unit Control Register.
 */
typedef union {
  struct {
    UINT32 resetcounterconfigs : 1;
    /* resetcounterconfigs - Bits[0:0], WO, default = 1'b0 
        
        When this bit is written to, the counter configuration registers will be reset. 
       This does not effect the values in the counters. 
             
     */
    UINT32 resetcounters : 1;
    /* resetcounters - Bits[1:1], WO, default = 1'b0 
        
               When this bit is written to, the counters data fields will be reset. The 
       configuration values will not be reset.  
             
     */
    UINT32 rsvd_2 : 6;
    /* rsvd_2 - Bits[7:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 freezecounters : 1;
    /* freezecounters - Bits[8:8], RW_V, default = 1'b0 
        
               This bit is written to when the counters should be frozen. If this bit 
       is written to and freeze is enabled, the counters in the unit will stop 
       counting. 
             
     */
    UINT32 rsvd_9 : 7;
    /* rsvd_9 - Bits[15:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 freezeenable : 1;
    /* freezeenable - Bits[16:16], RO, default = 1'b1 
        
               This bit controls what the counters in the unit will do when they 
       receive a freeze signal. When set, the counters will be allowed to freeze. When 
       not set, the counters will ignore the freeze signal. 
             
     */
    UINT32 overflowenable : 1;
    /* overflowenable - Bits[17:17], RO, default = 1'b1 
        
               This bit controls the behavior of counters when they overflow. When set, 
       the system will trigger the overflow handling process throughout the rest of the 
       uncore, potentially triggering a PMI and freezing counters. When it is not set, 
       the counters will simply wrap around and continue to count. 
             
     */
    UINT32 rsvd_18 : 14;
    /* rsvd_18 - Bits[31:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIPMONUNITCTRL_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* QPIPMONUNITSTATUS_QPIAGENT_PMON_REG supported on:                          */
/*       IVT_EP (0x401420F8)                                                  */
/*       IVT_EX (0x401420F8)                                                  */
/*       HSX (0x401420F8)                                                     */
/*       BDX (0x401420F8)                                                     */
/* Register default value:              0x00000000                            */
#define QPIPMONUNITSTATUS_QPIAGENT_PMON_REG 0x060140F8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.2.CFG.xml.
 * Intel QPI Pmon Unit Status Register.
 */
typedef union {
  struct {
    UINT32 counteroverflowbitmask : 5;
    /* counteroverflowbitmask - Bits[4:0], RW1C, default = 5'b0 
        
       	This field shows which registers have overflowed in the unit. 
        Whenever a register overflows, it should set the relevant bit to 1. An overflow 
       should not effect the other status bits. This status should only be cleared by 
       software.  
        We have defined 7 bits for this status. This is overkill for many units. See 
       below for the bits that are used in the different units.  
        In general, if the unit has a fixed counter, it will use bit 0. Counter 0 would 
       use the next LSB, and the largest counter would use the MSB.  
       	HA: [4:0] w/ [4] = Counter4 and [0] = Counter 0 
       	IMC: [5:0] w/ [0] = Fixed; [1] = Counter0 and [5] = Counter4 
       	Intel QPI: [4:0] (same as HA) 
       	PCU: [3:0]: [0] = Counter0 and [3] = Counter 3 
       	IO IRP0: [0] = Counter0; [1] = Counter1 
       	IO IRP1: [2] = Counter0; [3] = Counter1
             
     */
    UINT32 rsvd : 27;
    /* rsvd - Bits[31:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIPMONUNITSTATUS_QPIAGENT_PMON_STRUCT;
#endif /* ASM_INC */


/* SVID_QPIAGENT_PMON_REG supported on:                                       */
/*       IVT_EP (0x2014202C)                                                  */
/*       IVT_EX (0x2014202C)                                                  */
/* Register default value:              0x8086                                */
#define SVID_QPIAGENT_PMON_REG 0x0601202C



/* SDID_QPIAGENT_PMON_REG supported on:                                       */
/*       IVT_EP (0x2014202E)                                                  */
/*       IVT_EX (0x2014202E)                                                  */
/* Register default value:              0x0000                                */
#define SDID_QPIAGENT_PMON_REG 0x0601202E



#endif /* QPIAGENT_PMON_h */
