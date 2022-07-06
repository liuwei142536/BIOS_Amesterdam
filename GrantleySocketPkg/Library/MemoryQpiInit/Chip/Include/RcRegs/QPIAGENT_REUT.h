/* Date Stamp: 8/23/2014 */

#ifndef QPIAGENT_REUT_h
#define QPIAGENT_REUT_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For all target CPUs:                                                       */
/* QPIAGENT_REUT_DEV 8                                                        */
/* QPIAGENT_REUT_FUN 3                                                        */

/* VID_QPIAGENT_REUT_REG supported on:                                        */
/*       IVT_EP (0x20143000)                                                  */
/*       IVT_EX (0x20143000)                                                  */
/*       HSX (0x20143000)                                                     */
/*       BDX (0x20143000)                                                     */
/* Register default value:              0x8086                                */
#define VID_QPIAGENT_REUT_REG 0x06022000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
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
} VID_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* DID_QPIAGENT_REUT_REG supported on:                                        */
/*       IVT_EP (0x20143002)                                                  */
/*       IVT_EX (0x20143002)                                                  */
/*       HSX (0x20143002)                                                     */
/*       BDX (0x20143002)                                                     */
/* Register default value on IVT_EP:    0x0E83                                */
/* Register default value on IVT_EX:    0x0E83                                */
/* Register default value on HSX:       0x2F83                                */
/* Register default value on BDX:       0x6F83                                */
#define DID_QPIAGENT_REUT_REG 0x06022002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6F83 
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
} DID_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* PCICMD_QPIAGENT_REUT_REG supported on:                                     */
/*       IVT_EP (0x20143004)                                                  */
/*       IVT_EX (0x20143004)                                                  */
/*       HSX (0x20143004)                                                     */
/*       BDX (0x20143004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_QPIAGENT_REUT_REG 0x06022004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
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
} PCICMD_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* PCISTS_QPIAGENT_REUT_REG supported on:                                     */
/*       IVT_EP (0x20143006)                                                  */
/*       IVT_EX (0x20143006)                                                  */
/*       HSX (0x20143006)                                                     */
/*       BDX (0x20143006)                                                     */
/* Register default value:              0x0010                                */
#define PCISTS_QPIAGENT_REUT_REG 0x06022006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.3.CFG.xml.
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
} PCISTS_QPIAGENT_REUT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_QPIAGENT_REUT_REG supported on:                                        */
/*       IVT_EP (0x10143008)                                                  */
/*       IVT_EX (0x10143008)                                                  */
/*       HSX (0x10143008)                                                     */
/*       BDX (0x10143008)                                                     */
/* Register default value:              0x00                                  */
#define RID_QPIAGENT_REUT_REG 0x06021008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
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
} RID_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_QPIAGENT_REUT_REG supported on:                                     */
/*       IVT_EP (0x10143009)                                                  */
/*       IVT_EX (0x10143009)                                                  */
/*       HSX (0x10143009)                                                     */
/*       BDX (0x10143009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_QPIAGENT_REUT_REG 0x06021009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.3.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_QPIAGENT_REUT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_QPIAGENT_REUT_REG supported on:                                     */
/*       IVT_EP (0x2014300A)                                                  */
/*       IVT_EX (0x2014300A)                                                  */
/*       HSX (0x2014300A)                                                     */
/*       BDX (0x2014300A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_QPIAGENT_REUT_REG 0x0602200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
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
} CCR_N1_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* CLSR_QPIAGENT_REUT_REG supported on:                                       */
/*       IVT_EP (0x1014300C)                                                  */
/*       IVT_EX (0x1014300C)                                                  */
/*       HSX (0x1014300C)                                                     */
/*       BDX (0x1014300C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_QPIAGENT_REUT_REG 0x0602100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
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
} CLSR_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* PLAT_QPIAGENT_REUT_REG supported on:                                       */
/*       IVT_EP (0x1014300D)                                                  */
/*       IVT_EX (0x1014300D)                                                  */
/*       HSX (0x1014300D)                                                     */
/*       BDX (0x1014300D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_QPIAGENT_REUT_REG 0x0602100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
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
} PLAT_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* HDR_QPIAGENT_REUT_REG supported on:                                        */
/*       IVT_EP (0x1014300E)                                                  */
/*       IVT_EX (0x1014300E)                                                  */
/*       HSX (0x1014300E)                                                     */
/*       BDX (0x1014300E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_QPIAGENT_REUT_REG 0x0602100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
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
} HDR_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* BIST_QPIAGENT_REUT_REG supported on:                                       */
/*       IVT_EP (0x1014300F)                                                  */
/*       IVT_EX (0x1014300F)                                                  */
/*       HSX (0x1014300F)                                                     */
/*       BDX (0x1014300F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_QPIAGENT_REUT_REG 0x0602100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
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
} BIST_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* SVID_F3_QPIAGENT_REUT_REG supported on:                                    */
/*       HSX (0x2014302C)                                                     */
/*       BDX (0x2014302C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_F3_QPIAGENT_REUT_REG 0x0602202C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
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
} SVID_F3_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SDID_F3_QPIAGENT_REUT_REG supported on:                                    */
/*       HSX (0x2014302E)                                                     */
/*       BDX (0x2014302E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_F3_QPIAGENT_REUT_REG 0x0602202E

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
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
} SDID_F3_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CAPPTR_QPIAGENT_REUT_REG supported on:                                     */
/*       IVT_EP (0x10143034)                                                  */
/*       IVT_EX (0x10143034)                                                  */
/*       HSX (0x10143034)                                                     */
/*       BDX (0x10143034)                                                     */
/* Register default value on IVT_EP:    0x40                                  */
/* Register default value on IVT_EX:    0x40                                  */
/* Register default value on HSX:       0x00                                  */
/* Register default value on BDX:       0x40                                  */
#define CAPPTR_QPIAGENT_REUT_REG 0x06021034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
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
} CAPPTR_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* INTL_QPIAGENT_REUT_REG supported on:                                       */
/*       IVT_EP (0x1014303C)                                                  */
/*       IVT_EX (0x1014303C)                                                  */
/*       HSX (0x1014303C)                                                     */
/*       BDX (0x1014303C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_QPIAGENT_REUT_REG 0x0602103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
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
} INTL_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* INTPIN_QPIAGENT_REUT_REG supported on:                                     */
/*       IVT_EP (0x1014303D)                                                  */
/*       IVT_EX (0x1014303D)                                                  */
/*       HSX (0x1014303D)                                                     */
/*       BDX (0x1014303D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_QPIAGENT_REUT_REG 0x0602103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
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
} INTPIN_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* MINGNT_QPIAGENT_REUT_REG supported on:                                     */
/*       IVT_EP (0x1014303E)                                                  */
/*       IVT_EX (0x1014303E)                                                  */
/*       HSX (0x1014303E)                                                     */
/*       BDX (0x1014303E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_QPIAGENT_REUT_REG 0x0602103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
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
} MINGNT_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_QPIAGENT_REUT_REG supported on:                                     */
/*       IVT_EP (0x1014303F)                                                  */
/*       IVT_EX (0x1014303F)                                                  */
/*       HSX (0x1014303F)                                                     */
/*       BDX (0x1014303F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_QPIAGENT_REUT_REG 0x0602103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
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
} MAXLAT_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* PXPCAP_F3_QPIAGENT_REUT_REG supported on:                                  */
/*       HSX (0x40143040)                                                     */
/*       BDX (0x40143040)                                                     */
/* Register default value:              0x00910010                            */
#define PXPCAP_F3_QPIAGENT_REUT_REG 0x06024040

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * PCIE Capability
 */
typedef union {
  struct {
    UINT32 capability_id : 8;
    /* capability_id - Bits[7:0], RO, default = 8'h10 
       Provides the PCI Express capability ID assigned by PCI-SIG.
     */
    UINT32 next_ptr : 8;
    /* next_ptr - Bits[15:8], RO, default = 8'b0 
       Pointer to the next capability. Set to 0 to indicate there are no more 
       capability structures. 
     */
    UINT32 capability_version : 4;
    /* capability_version - Bits[19:16], RO, default = 4'b1 
       
       	PCI Express Capability is Compliant with Version 1.0 of the PCI Express Spec.
        Note: This capability structure is not compliant with Versions beyond 1.0, 
       since they require additional capability registers to be reserved. The only 
       purpose for this capability structure is to make enhanced configuration space 
       available. Minimizing the size of this structure is accomplished by reporting 
       version 1.0 compliancy and reporting that this is an integrated root port 
       device. As such, only three Dwords of configuration space are required for this 
       structure. 
             
     */
    UINT32 device_port_type : 4;
    /* device_port_type - Bits[23:20], RO, default = 4'h9 
       Device type is Root Complex Integrated Endpoint
     */
    UINT32 slot_implemented : 1;
    /* slot_implemented - Bits[24:24], RO, default = 1'b0 
       N/A for integrated endpoints
     */
    UINT32 interrupt_message_number : 5;
    /* interrupt_message_number - Bits[29:25], RO, default = 5'b0 
       N/A for this device
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PXPCAP_F3_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* QPIREUT_HDR_EXT_QPIAGENT_REUT_REG supported on:                            */
/*       IVT_EP (0x40143100)                                                  */
/*       IVT_EX (0x40143100)                                                  */
/*       HSX (0x40143100)                                                     */
/*       BDX (0x40143100)                                                     */
/* Register default value:              0x0000000B                            */
#define QPIREUT_HDR_EXT_QPIAGENT_REUT_REG 0x06024100
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * REUT PCIE Header Extended Register
 * This register is not supported in the processor and is listed to be compliant 
 * with the REUT specification. 
 */
typedef union {
  struct {
    UINT32 pciecapid : 16;
    /* pciecapid - Bits[15:0], RO, default = 16'hB 
        PCIE Extended CAP ID 
     */
    UINT32 pciecapversion : 4;
    /* pciecapversion - Bits[19:16], RO, default = 4'b0 
        Capability version 
     */
    UINT32 pcienextptr : 12;
    /* pcienextptr - Bits[31:20], RO, default = 12'b0 
        Next Capability offset 
     */
  } Bits;
  UINT32 Data;
} QPIREUT_HDR_EXT_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_HDR_CAP_QPIAGENT_REUT_REG supported on:                            */
/*       IVT_EP (0x40143104)                                                  */
/*       IVT_EX (0x40143104)                                                  */
/*       HSX (0x40143104)                                                     */
/*       BDX (0x40143104)                                                     */
/* Register default value:              0x0F000001                            */
#define QPIREUT_HDR_CAP_QPIAGENT_REUT_REG 0x06024104
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * REUT Header Capability Register
 * This register is not supported in the processor and is listed to be compliant 
 * with the REUT specification. 
 */
typedef union {
  struct {
    UINT32 vsecid : 16;
    /* vsecid - Bits[15:0], RO, default = 16'h1 
       
        This field is a Intel-defined ID number that indicates the nature and format of 
       the VSEC structure.  
        '01h' is the ID Council defined for REUT engines. Software must qualify the 
       Vendor ID before interpreting this field.  
             
     */
    UINT32 vsecidrev : 4;
    /* vsecidrev - Bits[19:16], RO, default = 4'b0 
       
        This field is defined as the version number that indicates the nature and 
       format of the VSEC structure.  
       	Software must quality the Vendor ID before interpreting this field.
             
     */
    UINT32 vseclength : 12;
    /* vseclength - Bits[31:20], RO, default = 12'hF0 
       
       	This field defines the length of the REUT 'capability body'. 
       	Currently the size of the body is 184 bytes or 46 DWORDs.
             
     */
  } Bits;
  UINT32 Data;
} QPIREUT_HDR_CAP_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_ENG_OVR_QPIAGENT_REUT_REG supported on:                            */
/*       IVT_EP (0x40143108)                                                  */
/*       IVT_EX (0x40143108)                                                  */
/*       HSX (0x40143108)                                                     */
/*       BDX (0x40143108)                                                     */
/* Register default value:              0x00400000                            */
#define QPIREUT_ENG_OVR_QPIAGENT_REUT_REG 0x06024108
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * REUT Engine Overview
 * This register is not supported in the processor and is listed to be compliant 
 * with the REUT specification. 
 */
typedef union {
  struct {
    UINT32 txrxonmultiplelinks : 1;
    /* txrxonmultiplelinks - Bits[0:0], RO, default = 1'b0 
       1 : REUT Engine can transmit and/or receive on multiple Links simultaneously.
     */
    UINT32 canconrlpartialsellinks : 1;
    /* canconrlpartialsellinks - Bits[1:1], RO, default = 1'b0 
       1 : REUT engine can control any possible subset of multiple Links 
       simultaneously.  
     */
    UINT32 separatesmctrl : 1;
    /* separatesmctrl - Bits[2:2], RO, default = 1'b0 
       
             Separate PHY state machine control
             0: Only one LTSSM for each link.
             1: There is more than one LTSSM for each link.
             
     */
    UINT32 rsvd_3 : 8;
    /* rsvd_3 - Bits[10:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 reutengineid : 6;
    /* reutengineid - Bits[16:11], RO, default = 6'b0 
       REUT Engine ID[5:0]
     */
    UINT32 reutspecrev : 8;
    /* reutspecrev - Bits[24:17], RO, default = 8'h20 
       
       	REUT revision[5:0][7:5]: major revision = 001b
       	[4:0]: minor revision = 0 0000b
             
     */
    UINT32 numlinks : 5;
    /* numlinks - Bits[29:25], RO, default = 5'b0 
       
       	Number of links supported. 
        The number of links supported +1 is the total number of separate Links that 
       this REUT Engine is designed to support. 
             
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_ENG_OVR_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_ENG_LD_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x4014310C)                                                  */
/*       IVT_EX (0x4014310C)                                                  */
/*       HSX (0x4014310C)                                                     */
/*       BDX (0x4014310C)                                                     */
/* Register default value on IVT_EP:    0x00100273                            */
/* Register default value on IVT_EX:    0x00100273                            */
/* Register default value on HSX:       0x00086273                            */
/* Register default value on BDX:       0x00086273                            */
#define QPIREUT_ENG_LD_QPIAGENT_REUT_REG 0x0602410C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * REUT Engine Link Description
 * This register is not supported in the processor and is listed to be compliant 
 * with the REUT specification. 
 */
typedef union {
  struct {
    UINT32 txlinkwidth : 5;
    /* txlinkwidth - Bits[4:0], RO, default = 5'h13 
       
        Tx Link Width Width of the Link indicated by LinkSelect in REUT-ENG-LS. Actual 
       width is N+1. 13h = 19dec.  
        Actual width is 19 + 1 = 20 lanes. If link is asymmetric then only the RX width 
       is reported here. 
             
     */
    UINT32 rxlinkwidth : 5;
    /* rxlinkwidth - Bits[9:5], RO, default = 5'h13 
       
        Rx Link Width Width of the Link indicated by LinkSelect in REUT-ENG-LS. Actual 
       width is N+1. 
        13h = 19dec. Actual width is 19 + 1 = 20 lanes. If link is asymmetric then only 
       the RX width is reported here. 
             
     */
    UINT32 rsvd_10 : 3;
    /* rsvd_10 - Bits[12:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 reutdevid : 8;
    /* reutdevid - Bits[20:13], RO, default = 8'h43 
       
        Device/Function number. Describes the unique Device/Fn number for the selected 
       Link (indicated by the LinkSelect setting). 
        Device/Fn number is a unique identifier that is component specific that is not 
       repeated for any two Links of within the component (i.e. every Intel QPI Link 
       must have a unique device number but an Intel QPI link can share a similar 
       device number.  
        Intel QPI port 0 located at: Device8  = 01000b, function3 = 011b. ReutDevID = 
       0100 0011b. 
        Intel QPI port 1 located at: Device9  = 01000b, function3 = 011b. ReutDevID = 
       0100 1011b. 
        Intel QPI port 2 located at: Device10 = 01010b, function3 = 011b. ReutDevID = 
       0101 0011b. 
             
     */
    UINT32 rsvd_21 : 2;
    /* rsvd_21 - Bits[22:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 linkinactive : 1;
    /* linkinactive - Bits[23:23], RO_V, default = 1'b0 
       
        Link Inactive: Indicates whether the current Link (indicated by the Link 
       Select) is in use. 
        In general this bit is used to indicate that this link is disabled and no 
       testing can be performed. 
       	Intel QPI Link Behavior: Indicates whether the selected Link is disabled.
             
     */
    UINT32 linktype : 6;
    /* linktype - Bits[29:24], RO, default = 6'b0 
       
       	Link TypeIndicates the type of Link selected by Link Select.
       	000000: Intel QPI
             
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_ENG_LD_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_ENG_LSCAP_QPIAGENT_REUT_REG supported on:                          */
/*       IVT_EP (0x40143110)                                                  */
/*       IVT_EX (0x40143110)                                                  */
/*       HSX (0x40143110)                                                     */
/*       BDX (0x40143110)                                                     */
/* Register default value:              0x00000000                            */
#define QPIREUT_ENG_LSCAP_QPIAGENT_REUT_REG 0x06024110
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * REUT Link Speed Capability
 * This register is not supported in the processor and is listed to be compliant 
 * with the REUT specification. 
 */
typedef union {
  struct {
    UINT32 currpllratio : 7;
    /* currpllratio - Bits[6:0], RO, default = 7'b0 
       Current PLL Ratio This is current PLL ratio.
     */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pllratiomask : 6;
    /* pllratiomask - Bits[13:8], RO, default = 6'b0 
       
       	This indicates the range of options for programming the I/O PLL ratio.
       	Examples:
        001100 - Indicates that only ratio multiples of four and eight can be 
       programmed. 
        010001 - Indicates that only ratio multiples of sixteen and one can be 
       programmed. 
             
     */
    UINT32 rsvd_14 : 2;
    /* rsvd_14 - Bits[15:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 minclkratio : 6;
    /* minclkratio - Bits[21:16], RO, default = 6'b0 
       Minimum Clock Ratio This is the minimum value usable for the PLL ratio. 
     */
    UINT32 rsvd_22 : 2;
    /* rsvd_22 - Bits[23:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 maxclkratio : 6;
    /* maxclkratio - Bits[29:24], RO, default = 6'b0 
       Maximum Clock Ratio This is the maximum value usable for the PLL ratio. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_ENG_LSCAP_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_ENG_LSCTR_QPIAGENT_REUT_REG supported on:                          */
/*       IVT_EP (0x40143114)                                                  */
/*       IVT_EX (0x40143114)                                                  */
/*       HSX (0x40143114)                                                     */
/*       BDX (0x40143114)                                                     */
/* Register default value:              0x00000000                            */
#define QPIREUT_ENG_LSCTR_QPIAGENT_REUT_REG 0x06024114
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * REUT Link Speed Control
 * This register is not supported in the processor and is listed to be compliant 
 * with the REUT specification. 
 */
typedef union {
  struct {
    UINT32 nextpllratio : 7;
    /* nextpllratio - Bits[6:0], RO, default = 7'b0 
       
       	Next PLL Ratio after link reset.
        Allows for controlling the speed of the Link by setting the next PLL ratio to 
       be used after an Inband Reset. 
             
     */
    UINT32 rsvd : 25;
    /* rsvd - Bits[31:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_ENG_LSCTR_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_ENG_LTRCAP_QPIAGENT_REUT_REG supported on:                         */
/*       IVT_EP (0x40143118)                                                  */
/*       IVT_EX (0x40143118)                                                  */
/*       HSX (0x40143118)                                                     */
/*       BDX (0x40143118)                                                     */
/* Register default value:              0x00000000                            */
#define QPIREUT_ENG_LTRCAP_QPIAGENT_REUT_REG 0x06024118
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * REUT Engine Link Transmit and Receive Control
 * This register is not supported in the processor and is listed to be compliant 
 * with the REUT specification. 
 */
typedef union {
  struct {
    UINT32 linkcapability : 8;
    /* linkcapability - Bits[7:0], RO, default = 8'b0 
       
        Link Capability - Link Capability represents each of the Links indicated by # 
       of Links. 
       	Supported by using a one hot it for each of the Links.
       	Intel QPI link Behavior
       	Every even bit represents a Link.
       	[0]: Link 0
       	[1]: Reserved
       	[2]: Link 1
       	[3]: Reserved
       	[4]: Link 2
       	Etc..
       	Odd bits are reserved.
             
     */
    UINT32 rsvd : 24;
    /* rsvd - Bits[31:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_ENG_LTRCAP_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_ENG_LTRCON_QPIAGENT_REUT_REG supported on:                         */
/*       IVT_EP (0x4014311C)                                                  */
/*       IVT_EX (0x4014311C)                                                  */
/*       HSX (0x4014311C)                                                     */
/*       BDX (0x4014311C)                                                     */
/* Register default value:              0x00000000                            */
#define QPIREUT_ENG_LTRCON_QPIAGENT_REUT_REG 0x0602411C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * REUT Engine Link Transmit and Receive Configuration
 * This register is not supported in the processor and is listed to be compliant 
 * with the REUT specification. 
 */
typedef union {
  struct {
    UINT32 linkstocontrol : 16;
    /* linkstocontrol - Bits[15:0], RO, default = 16'b0 
       
        Link Control Dependency on Link Control is called out explicitly throughout 
       various REUT registers definition when it occurs. 
       	Every even bit in Link Control represents a Link.
       	Bit 0 = Link 0
       	Bit 2 = Link 1
       	Bit 4 = Link 2 Etc.
        The following Rules apply to the Even bits of Link Control. If Can Transmit or 
       Receive on Multiple Links? = 0 then the even Bits of Link Control are not 
       writable and must always be equal to Link Capability. Link Select must be the 
       sole method to determine which Link is being accessed and controlled. 
        If Can Transmit or Receive on Multiple Links? = 1 then the REUT engine can 
       transmit and/or receive on multiple links based on the following rules. 
        If Can Select a Partial Set Of Link Control is set to 0 then the even Bits of 
       Link Control are not writable and must always be equal to Link Capability  
       	Else,
        If Can Select a Partial Set Of Link Control is set to 1 then the even bits of 
       Link Control can be set to 0 or 1 for any combination of Links which are set to 
       1 in the Link Capability. Writing to register dependent on Link Control will 
       cause only the selected Links to be written to while leaving non selected Link 
       register values unchanged. 
       	The following Rules apply to the odd bits of Link Control.
       	Intel QPI link Behavior: Odd bits are reserved.
             
     */
    UINT32 rsvd_16 : 7;
    /* rsvd_16 - Bits[22:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 linkautoconfiguration : 1;
    /* linkautoconfiguration - Bits[23:23], RO, default = 1'b0 
       
       	Link Auto Configuration:  
        If Link Auto Configuration Capability is set to 0 then Link Auto Configuration 
       is always reserved as 0.  
       	Else,
        If Link Auto Configuration is set to 0 then all odd Link Control bits that are 
       set to 1 will condition how the Pre Configuration Lane ID values are set.  
        If Link Auto Configuration is set to 1 then all odd Link Control bits are 
       ignored and are assumed to be 0. 
       	See Link Control for more details.
             
     */
    UINT32 reutdisable : 1;
    /* reutdisable - Bits[24:24], RO, default = 1'b0 
       
       	REUT disable
       	0: REUT enable
       	1: REUT disable
        By writing this bit the whole REUT PCI Function will disappear from the 
       perspective of software (i.e. PCI CSR space). 
        The REUT engine will only be made visible again upon the next Cold Reset or 
       some other implementation specific means. 
        This is intended to act as a security mechanism to prevent any unwanted usage 
       of the REUT registers. 
       	Note: Support for this feature is optional. ( It was RW-O).
             
     */
    UINT32 linkselect : 4;
    /* linkselect - Bits[28:25], RO, default = 4'b0 
       
        Link Select -This bit is used in conjunction with many other REUT register bits 
       which are explicitly called out within the description field of the particular 
       bit where it has a potential affect. 
       	Note: We there is only one Intel QPI link so this bit field cannot be written.
             
     */
    UINT32 rsvd_29 : 3;
    /* rsvd_29 - Bits[31:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_ENG_LTRCON_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIBURST_ERR_CNTR_QPIAGENT_REUT_REG supported on:                          */
/*       BDX (0x40143120)                                                     */
/* Register default value:              0x00000000                            */
#define QPIBURST_ERR_CNTR_QPIAGENT_REUT_REG 0x06024120

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * QPI BURST ERROR COUNTER
 */
typedef union {
  struct {
    UINT32 error_burst_mode : 2;
    /* error_burst_mode - Bits[1:0], RWS_L, default = 2'b00 
       
          QPI Burst Error Mode
       	Specifies which mode of burst is needed
       	00 - Reserved
       	01 - Burst Mode of 2
       	10 - Burst Mode of 3
       	11 - Burst Mode of 4
       	  
     */
    UINT32 error_burst_mode_enable : 1;
    /* error_burst_mode_enable - Bits[2:2], RWS_L, default = 1'b0 
       
          QPI Burst Error Mode Enable
          Burst Mode Error Counter will be enabled if this bit is SET, this makes use 
       of the REUT defined Selected lane of interest to count the burst mode of error 
       on that specific lane. 
       	 
     */
    UINT32 rsvd_3 : 5;
    /* rsvd_3 - Bits[7:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 err_burst_cnt_value : 8;
    /* err_burst_cnt_value - Bits[15:8], RW1CS_L, default = 8'd0 
       
          QPI Burst Error Counter Value
          Specifies the number of burst error count value for that particular  burst 
       mode and if the error_burst_mode _enable is 1. Cleared by writing 1 or PowerGood 
       Reset. 
       	 
     */
    UINT32 err_burst_cnt_overflow : 1;
    /* err_burst_cnt_overflow - Bits[16:16], RW1CS_L, default = 1'b0 
       
          QPI Burst Error Counter Overflow
          Specifies the overflow of the counter and stalls/stops the counter further. 
       Cleared by writing 1 or PowerGood Reset. 
       	 
     */
    UINT32 rsvd_17 : 15;
    /* rsvd_17 - Bits[31:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIBURST_ERR_CNTR_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* QPIREUT_PH_CPR_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x40143128)                                                  */
/*       IVT_EX (0x40143128)                                                  */
/*       HSX (0x40143128)                                                     */
/*       BDX (0x40143128)                                                     */
/* Register default value:              0x10C73901                            */
#define QPIREUT_PH_CPR_QPIAGENT_REUT_REG 0x06024128


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.3.CFG.xml.
 * REUT Physical Layer Capability
 */
typedef union {
  struct {
    UINT32 phyversion : 4;
    /* phyversion - Bits[3:0], RO, default = 4'b1 
       
       	Intel QPI Version
       	0h: QPI v1.0
       	1h: QPI v1.1
       	all other encoding are reserved.
             
     */
    UINT32 retraindurationgranulariity : 1;
    /* retraindurationgranulariity - Bits[4:4], RO, default = 1'b0 
       
       	Retraining duration granularity
       	0: No support for retraining on a 16UI granularity
       	1: Support for retraining on a 16UI granularity
             
     */
    UINT32 rsvd_5 : 1;
    /* rsvd_5 - Bits[5:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 limited_comp : 1;
    /* limited_comp - Bits[6:6], RO, default = 1'b0 
       
       	Limited compliance
        If set, an implemention is not capable of running in Compliance slave mode as 
       well as transitioning to Loopback.Pattern from Compliance state. 
             
     */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 linkwidthcap : 3;
    /* linkwidthcap - Bits[10:8], RO, default = 3'h1 
       
       	Link Width Capability
       	XX1: Full Width
       	X1X: Half Width (EX processor only)
       	1XX: Quarter Width (not supported on the processor)
             
     */
    UINT32 scrmblstaggerl0 : 1;
    /* scrmblstaggerl0 - Bits[11:11], RO, default = 1'b1 
       
       	Staggered Scrambling During L0
        If set, implentation is capable of using lane respective staggered patterns for 
       data scrambling/descrambling in L0. 
             
     */
    UINT32 scrmblstaggerinit : 1;
    /* scrmblstaggerinit - Bits[12:12], RO, default = 1'b1 
       
       	Staggering and Scrambling During Init
        If set, implementation is capable of using lane respective staggered patterns 
       in bitlock/retraining and for scrambling TS. 
             
     */
    UINT32 rxadapt : 1;
    /* rxadapt - Bits[13:13], RO, default = 1'b1 
       
       	If set, implementation is capable of supporting perlane adaptation.
             
     */
    UINT32 rsvd_14 : 2;
    /* rsvd_14 - Bits[15:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 determinismsupport : 2;
    /* determinismsupport - Bits[17:16], RO, default = 2'b11 
       
       	Determinism Support.
       	1X: Master mode capability
       	X1: Slave mode capability
             
     */
    UINT32 cont_retrain : 1;
    /* cont_retrain - Bits[18:18], RO, default = 1'b1 
       
       	Continuous Retraining
       	If set, Rx is capable of continous retraining
             
     */
    UINT32 rsvd_19 : 1;
    /* rsvd_19 - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rascap : 2;
    /* rascap - Bits[21:20], RO, default = 2'b0 
       
       	RAS capability
       	Bit 20: If set, RAS capable with Alternate Clock 1.
       	Bit 21: If set, RAS capable with Alternate Clock 2.
        Any of these bits set indicates Alternate Clock RAS capability available and 
       that corresponding control bits in QPIPHCTR and status bits in QPIPHPLS are 
       implemented. 
        Note: Clock failover will force forwarded clock speed to half in both 
       directions. 
             
     */
    UINT32 datscramblelfsr : 1;
    /* datscramblelfsr - Bits[22:22], RO, default = 1'b1 
       
       	Data Scramble with LFSR 
       	If set, implementation capable of data scrambling/descrambling with LFSR
             
     */
    UINT32 bitlockretrainpatt : 1;
    /* bitlockretrainpatt - Bits[23:23], RO, default = 1'b1 
       
       	Bit-lock and Retrain with pattern.
        If set, the implementation supports using a specified patter in 
       bit-lock/retraining 
             
     */
    UINT32 numtxlanes : 5;
    /* numtxlanes - Bits[28:24], RO, default = 5'h10 
       
       	Number of Tx lanes with which an implementation can operate for full width. 
       	Bit 28 - If set, 20 lanes.
       	Others Reserved.
        The bit indicating the maximum lanes will determine the number of 
       control/status bits implemented in TX/RX Data lane Control/Status Registers. 
             
     */
    UINT32 rsvd_29 : 3;
    /* rsvd_29 - Bits[31:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_CPR_QPIAGENT_REUT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIREUT_PH_CTR_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x4014312C)                                                  */
/*       IVT_EX (0x4014312C)                                                  */
/*       HSX (0x4014312C)                                                     */
/*       BDX (0x4014312C)                                                     */
/* Register default value on IVT_EP:    0x00002020                            */
/* Register default value on IVT_EX:    0x00002020                            */
/* Register default value on HSX:       0x04000020                            */
/* Register default value on BDX:       0x04000020                            */
#define QPIREUT_PH_CTR_QPIAGENT_REUT_REG 0x0602412C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.3.CFG.xml.
 * REUT Physical Layer Control
 */
typedef union {
  struct {
    UINT32 phylayerreset : 1;
    /* phylayerreset - Bits[0:0], RW1SS, default = 1'b0 
       
       	1 - Soft Reset.
        Writing 1 will initiate soft reset, which will cause reinitialization of 
       Physical layer. 
       	This field will be cleared to 0 on entering Reset.Calibrate state.
       	A Soft Reset can be modified to Default Reset with Reset Modifier.
             
     */
    UINT32 resetmodifier : 1;
    /* resetmodifier - Bits[1:1], RWS_L, default = 1'b0 
       
       	Reset Modifier
        1 - Modifies Soft Reset (through bit 0) to Default Reset - State machine 
       transitions to Reset.Default which will override sticky bits to 
       	Default values
       	0 - Sticky bits are safe through Reset
             
     */
    UINT32 bypasscalib : 1;
    /* bypasscalib - Bits[2:2], RWS_L, default = 1'b0 
       
        1 - Bypass I/O Calibration and use electrical parameters through CSR. Used for 
       debug. 
        Note: Setting this bit does not prevent any necessary timing training from 
       occurring. 
             
     */
    UINT32 latencyfixctrl : 1;
    /* latencyfixctrl - Bits[3:3], RWS_L, default = 1'b0 
       
        1 - Fix the latency at remote Rx. This bit is copied to TS.Param to indicate 
       remote port to fix the latency. 
             
     */
    UINT32 singlestepmode : 1;
    /* singlestepmode - Bits[4:4], RWS_L, default = 1'b0 
       
        1 - Physical layer state machine is in single step mode - Only one state 
       advanced at a time. 
             
     */
    UINT32 phyinitbegin : 1;
    /* phyinitbegin - Bits[5:5], RWS_L, default = 1'b1 
       
        Writing 0 to PhyInitBegin will stall the state machine transition from 
       Reset.Calibrate to Detect.ClkTerm. 
        Note: there are ot her internal/reset flows control which also is used to block 
       or stall Intel QPI training. 
             
     */
    UINT32 rsvd_6 : 1;
    UINT32 link_speed : 1;
    /* link_speed - Bits[7:7], RWS_L, default = 1'b0 
        
       	Link Speed
       	1 - Force direct operational speed initialization (6.4 GT/s)
       	0 - Slow speed initialization (25 MT/s)
             
     */
    UINT32 initializemode : 3;
    /* initializemode - Bits[10:8], RWS_L, default = 3'b0 
       
        Initialization mode determines altered initialization modes of Physical layer   
         
       	000 - Normal initialization towards L0 state  
       	001 - Initialization towards Loopback master state   
        010 - ATE L0 - Enable altered initialization towards L0 state (for debug use 
       only)   
        011 - ATE Loopback Master - Enable altered initialization towards Loopback 
       state as a Loopback Master for test/debug environment   
       	100 - Forced entry to Compliance Master  
       	101 - Forced entry to Compliance Slave  
       	110 - 111 - Reserved      
             
     */
    UINT32 disablesicheck : 1;
    /* disablesicheck - Bits[11:11], RWS_L, default = 1'b0 
       
       	Disable ISI checking
       	0 - Enable ISI checking in Polling.LaneDeskew
       	1 - Disable ISI checking in Polling.LaneDeskew
             
     */
    UINT32 initabortfreeze : 1;
    /* initabortfreeze - Bits[12:12], RWS_L, default = 1'b0 
       
       	Initialization Abort Freeze
       	0 - Retry after initialization abort 
       	1 - Freeze the state machine at the point of initialization abort 
             
     */
    UINT32 disableautocompliance : 1;
    /* disableautocompliance - Bits[13:13], RWS_L, default = 1'b0 
       
       	Disable autocompliance
       	0 - path from Detect.ClkTerm to Compliance is allowed
       	1 - path from detect.clkterm to compliance is disabled
             
     */
    UINT32 determinismmode : 2;
    /* determinismmode - Bits[15:14], RWS_L, default = 2'b0 
       
       	Determinism mode of operation      
        10 - Master mode of initialization - valid only if a component can generate its 
       PhyL0Sync   
       	01 - Slave mode of initialization  
       	00 - Non-deterministic initialization   
       	11 - Reserved     
             
     */
    UINT32 enstgrscrmbll0 : 1;
    /* enstgrscrmbll0 - Bits[16:16], RWS_L, default = 1'b0 
       
       	Enable Staggered Scrambling During L0
       	0 - L0 data is scrambled/descrambled with non staggered LFSR sequence
        1 - L0 data is scrambled/descrambled with staggered LFSR sequence Treated as 
       not enabled during Slow Mode. 
        If enabled in operational speed, software should also set Staggered and 
       Scrambling During Init QPIPHCTR(17) 
             
     */
    UINT32 enstgrscrmblinit : 1;
    /* enstgrscrmblinit - Bits[17:17], RWS_L, default = 1'b0 
       
       	Enable Staggering and Scrambling During Init.
       	1 - use staggered pattern for bitlock/retraining and scrambling TS
        0 - use non staggered pattern in bitlock/retraining and no TS scrambling 
       Treated as not enabled during Slow Mode. 
        If enabled in operational speed, software should also set Staggered Scrambling 
       During LO QPIPHCTR(16). 
             
     */
    UINT32 enadaptrxeq : 1;
    /* enadaptrxeq - Bits[18:18], RWS_L, default = 1'b0 
       
       	Enable Adaptation
       	0 - Do not Adapt
       	1 - Adapt
       	Set by software. this bit is used for any adaptation enabled in QPIPHCTR1.
             
     */
    UINT32 use_back_ch : 1;
    /* use_back_ch - Bits[19:19], RW1CS_L, default = 1'b0 
       
       	Use BackChannel
       	Reset to 0 at Cold Reset.
       	Set to 1 when any Tx Adaptation is complete.
             
     */
    UINT32 restore_non_failover : 1;
    /* restore_non_failover - Bits[20:20], RW1SS_L, default = 1'b0 
       
       	Reset to 0 at cold reset. 
        Set to 1 by software to restore non-failover conditions of full (not half) 
       speed and all clock lanes (not disabled by software) available.  
       	Cleared to 0 for both LAR and effective bit on entering Reset.Calibrate. 
       	Used during hot-plug and hot-removal.
             
     */
    UINT32 dropbadclkenabled : 1;
    /* dropbadclkenabled - Bits[21:21], RWS_L, default = 1'b0 
       
       	Enable clock fail-safe
       	0 - clock fail-safe not enabled 
       	1 - clock fail-safe lanes enabled
             
     */
    UINT32 enablescramblelfsr : 1;
    /* enablescramblelfsr - Bits[22:22], RWS_L, default = 1'b0 
        
       	Enable Data Scramble with LFSR
       	1 - data scrambled/descrambled with LFSR
       	0 - data not scrambled/descrambled
             
     */
    UINT32 enablebitlockretrainwithpatt : 1;
    /* enablebitlockretrainwithpatt - Bits[23:23], RWS_L, default = 1'b0 
       
       	Enable Bitlock and Retraining with Pattern
       	1 - use pattern in bitlock/retraining 
       	0 - use clock pattern for bitlock/retraining
             
     */
    UINT32 altclklanedisable : 3;
    /* altclklanedisable - Bits[26:24], RWS_L, default = 3'h4 
       
       	Rx Alternate Clock Lane Disable.
        A bit mask for selectively enabling/disabling clock lanes on the RX side of the 
       link (for debug use only). A bit value of 1 indicates 
        the corresponding clock lane is disabled Rx clock terminations are selected in 
       Detect.ClkTerm state, depending on this mask.    
       	bit 24 - Primary clock  
       	bit 25 - Alternate clock 1  
       	bit 26 - Alternate clock 2       
       	Bit Values 
       	1 - Disable clock lane
       	0 - Enable clock lane
        A value of 0x7 must not be programmed in this field. The behavior is undefined 
       with that value.   
        Also when clock failover is enabled, alt clock 1 must be enabled. When clock 
       failover is disabled, primary clock lane must be enabled. 
       	Note: Enabling Alt clock 2 in this CSR field has no effect to logic.
             
     */
    UINT32 lateactloaddis : 1;
    /* lateactloaddis - Bits[27:27], RWS_L, default = 1'b0 
       
       	LALoadDisable
       	1 - Disable loading of CSR Effective registers from late action registers 
       	0 - Enable loading of CSR Effective registers from late action registers
             
     */
    UINT32 la_read_modifier : 1;
    /* la_read_modifier - Bits[28:28], RWS_L, default = 1'b0 
       
       	1 - Return CSR Effective value on read
       	0 - Return CSR value on read
             
     */
    UINT32 tx_aclk_lane_dis : 3;
    /* tx_aclk_lane_dis - Bits[31:29], RWS_L, default = 3'b0 
       
       	TX Alternate Clock Lane Disable
        Applies to TX side of the link. A bit mask for selectively enabling/disabling 
       clock lanes for validation/test purposes only.  
       	A bit value of 1 indicates the corresponding clock lane is disabled.
       	A bit value of 0 indicates the corresponding clock lane is enabled.
       	Bit 29 - Primary clock
       	Bit 30 - Alternate clock 1
       	Bit 31 - Alternate clock 2
             
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_CTR_QPIAGENT_REUT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIREUT_PH_TDC_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x40143130)                                                  */
/*       IVT_EX (0x40143130)                                                  */
/*       HSX (0x40143130)                                                     */
/*       BDX (0x40143130)                                                     */
/* Register default value:              0x00000000                            */
#define QPIREUT_PH_TDC_QPIAGENT_REUT_REG 0x06024130
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * REUT Tx Data Lane Control
 */
typedef union {
  struct {
    UINT32 txdatalanedisable : 20;
    /* txdatalanedisable - Bits[19:0], RWS_L, default = 20'b0 
       
       	Tx Data Lane Disable
       	A bit mask used for selectively enabling/disabling data TX Lanes.
       	Used for debug and validation purposes.
       	A bit value of 1 indicates the corresponding lane is disabled. 
       	Bit 0: Controls Lane 0.
       	Bit 1: Controls Lane 1.
       	.. and so on.
       	Dual use data lanes are disabled only for data operation, not for clock use.
       	Separate registers are provided for clock disable.
             
     */
    UINT32 rsvd : 12;
    /* rsvd - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_TDC_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_PH_TDS_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x40143134)                                                  */
/*       IVT_EX (0x40143134)                                                  */
/*       HSX (0x40143134)                                                     */
/*       BDX (0x40143134)                                                     */
/* Register default value on IVT_EP:    0x800FFFFF                            */
/* Register default value on IVT_EX:    0x800FFFFF                            */
/* Register default value on HSX:       0xFFFFFFFF                            */
/* Register default value on BDX:       0xFFFFFFFF                            */
#define QPIREUT_PH_TDS_QPIAGENT_REUT_REG 0x06024134


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.3.CFG.xml.
 * REUT Tx Data Lane Ready Status
 */
typedef union {
  struct {
    UINT32 txlanedetectstat : 20;
    /* txlanedetectstat - Bits[19:0], RO_V, default = 20'hFFFFF 
       
       	Tx Data Lane Detection Status
       	A bit value of 0 indicates the TX lane is dropped.
       	Bit 0: Status of lane 0.
       	Bit 1: Status of lane 1.
       	.. and so on..
             
     */
    UINT32 reserved : 11;
    /* reserved - Bits[30:20], RV, default = 11'h7FF 
       Reserved
     */
    UINT32 txfwdclkdetectsta : 1;
    /* txfwdclkdetectsta - Bits[31:31], RO_V, default = 1'b1 
       
       	Tx Forwarded clock lane detection status
       	A bit value of 0 indicates forwarded clock lane is dropped 
             
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_TDS_QPIAGENT_REUT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIREUT_PH_RDS_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x40143138)                                                  */
/*       IVT_EX (0x40143138)                                                  */
/*       HSX (0x40143138)                                                     */
/*       BDX (0x40143138)                                                     */
/* Register default value on IVT_EP:    0x000FFFFF                            */
/* Register default value on IVT_EX:    0x000FFFFF                            */
/* Register default value on HSX:       0xFFFFFFFF                            */
/* Register default value on BDX:       0xFFFFFFFF                            */
#define QPIREUT_PH_RDS_QPIAGENT_REUT_REG 0x06024138


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.3.CFG.xml.
 * REUT Rx Data Lane Ready Status
 */
typedef union {
  struct {
    UINT32 rxdatareadystat : 20;
    /* rxdatareadystat - Bits[19:0], RO_V, default = 20'hFFFFF 
       
       	Rx Data Lane Ready Status
       	A bit value of 0 indicates Rx lane is dropped.
       	Bit 0: Status of Lane 0
       	Bit 1: Status of Lane 1.
       	.. and so on.
             
     */
    UINT32 reserved : 12;
    /* reserved - Bits[31:20], RV, default = 12'hFFF 
       Reserved
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_RDS_QPIAGENT_REUT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIREUT_PH_RDC_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x4014313C)                                                  */
/*       IVT_EX (0x4014313C)                                                  */
/*       HSX (0x4014313C)                                                     */
/*       BDX (0x4014313C)                                                     */
/* Register default value:              0x00000000                            */
#define QPIREUT_PH_RDC_QPIAGENT_REUT_REG 0x0602413C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * REUT Rx Data Lane Ready Control
 */
typedef union {
  struct {
    UINT32 rxdatlanedisable : 20;
    /* rxdatlanedisable - Bits[19:0], RWS_L, default = 20'b0 
       
       	Rx Data Lane Disable
       	A bit mask used for selectively enabling/disabling data RX Lanes.
       	Used for debug and validation purposes.
       	A bit value of 1 indicates the corresponding RX lane is disabled.
       	Bit 0: Controls RX on Lane 0.
       	Bit 1: Controls RX on Lane 1.
       	.. and so on.
       	Dual use data lanes are disabled only for data operation, not for clock use.
       	Separate registers are provided for clock disable.
             
     */
    UINT32 rsvd : 12;
    /* rsvd - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_RDC_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_PH_PIS_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x40143140)                                                  */
/*       IVT_EX (0x40143140)                                                  */
/*       HSX (0x40143140)                                                     */
/*       BDX (0x40143140)                                                     */
/* Register default value:              0x00000000                            */
#define QPIREUT_PH_PIS_QPIAGENT_REUT_REG 0x06024140


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.3.CFG.xml.
 * REUT Physical Layer Initialization Status
 */
typedef union {
  struct {
    UINT32 linkupidentifier : 1;
    /* linkupidentifier - Bits[0:0], RW1CS, default = 1'b0 
       
       	Link-up Identifier 
        Set to 0 during Default Reset. Set to 1 when initialization completes and link 
       enters L0 
             
     */
    UINT32 calibratedone : 1;
    /* calibratedone - Bits[1:1], RW1CS, default = 1'b0 
       
       	Calibration Done
       	Reset to 0 at Default Reset  
       	Set to 1 once calibration is complete  
       	Clearing this bit will force calibration in next re-initialization  
             
     */
    UINT32 adaptrxeqdone : 1;
    /* adaptrxeqdone - Bits[2:2], RW1C, default = 1'b0 
       
       	Adapt ive EQ Done
       	Reset to 0 at Cold Reset.
       	Hardware will set to 1 once Adaptive EQ is complete.
             
     */
    UINT32 chg_half_speed : 1;
    /* chg_half_speed - Bits[3:3], ROS_V, default = 1'b0 
       
       	Changed to Half Speed
       	Reset to 0 on cold reset. 
       	1 indicates that clock change to half speed has occurred. 
       	This bit is typically routed to the clock unit.
             
     */
    UINT32 rsvd_4 : 4;
    /* rsvd_4 - Bits[7:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 txstatetracker : 5;
    /* txstatetracker - Bits[12:8], RO_V, default = 5'b0 
       
       	Tx state tracker
       Indicates the current encoded state of local Tx 
       	0 0000 - Reset.Soft and Reset.Default  
       	0 0001 - Reset.Calibrate  
       	0 0010 - Detect.ClkTerm  
       	0 0011 - Detect.FwdClk  
       	0 0100 - Detect.DCPattern  
       	0 0101 - Polling.BitLock  
       	0 0110 - Polling.LaneDeskew  
       	0 0111 - Polling.Param  
       	0 1000 - Config.LinkWidth  
       	0 1001 - Config.FlitLock  
       	0 1010 - L1
       	0 1100 - L0s
       	0 1101 - L0p
       	0 1110 - L0R (Periodic Retraining in process)  
       	0 1111 - L0  
       	1 0010 - Loopback.Marker Master  
       	1 0011 - Loopback.Marker Slave  
       	1 0000 - Loopback.Pattern Master  
       	1 0001 - Loopback.Pattern Slave  
       	1 0100 - Adapt
       	1 1111 - Compliance  
       	Others - Reserved
             
     */
    UINT32 rsvd_13 : 3;
    /* rsvd_13 - Bits[15:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rxstatetracker : 5;
    /* rxstatetracker - Bits[20:16], RO_V, default = 5'b0 
       
       	rx state tracker
       	Indicates the current state of local Rx.   
       	Refer to tx_state_tracker for encoding description.
             
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ackstat : 2;
    /* ackstat - Bits[25:24], RO_V, default = 2'b0 
       
       	ACK status
       	Bit 24 - If set, PortTxReady attribute set  
       	Bit 25 - If set, PortRemoteAck attribute set  
             
     */
    UINT32 initializespeed : 1;
    /* initializespeed - Bits[26:26], RO_V, default = 1'b0 
       
       	Initialization Speed
       	0 - Slow Speed Initialization 
       	1 - Operational Speed Initialization  
             
     */
    UINT32 statemachinehold : 1;
    /* statemachinehold - Bits[27:27], RW1CS, default = 1'b0 
       
       	State Machine Hold
        1 - Physical layer state machine is holding on single step or freezing on 
       initialization abort.   
       	Clearing this bit will cause state machine to advance.  
             
     */
    UINT32 tstbsy : 1;
    /* tstbsy - Bits[28:28], RO_V, default = 1'b0 
       
       	Test Busy
        The field indicates a test is in progress - It is set in transition from 
       Polling.Param/Loopback.Pattern to Loopback.Marker.    
        It is cleared either in Loopback.Marker when receiving a Master NOP or by 
       entering Reset.Soft.    
       	It is intended to be polled while in Loopback.   
        When it is equal to 0, the user knows any previous Loopback test has completed 
       and that a new test is ready to be initiated using Start Test.   
             
     */
    UINT32 rsvd_29 : 3;
    /* rsvd_29 - Bits[31:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_PIS_QPIAGENT_REUT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIREUT_PH_PPS_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x40143144)                                                  */
/*       IVT_EX (0x40143144)                                                  */
/*       HSX (0x40143144)                                                     */
/*       BDX (0x40143144)                                                     */
/* Register default value:              0x00000000                            */
#define QPIREUT_PH_PPS_QPIAGENT_REUT_REG 0x06024144


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.3.CFG.xml.
 * REUT Phy Layer Previous Init Status
 */
typedef union {
  struct {
    UINT32 prevlinkupident : 1;
    /* prevlinkupident - Bits[0:0], RW1CS, default = 1'b0 
       
       	Previous Linkup Identifier.
       	The Link-up Identifier field from previous initialization abort.  
             
     */
    UINT32 prevcaldone : 1;
    /* prevcaldone - Bits[1:1], RW1CS, default = 1'b0 
       
       	Previous Calibration Done.
       	The Calibration done field from previous initialization abort.  
             
     */
    UINT32 prevadaptrxeqdone : 1;
    /* prevadaptrxeqdone - Bits[2:2], RW1CS, default = 1'b0 
       
       	Previous Rx Adoption Done.
             
     */
    UINT32 prev_chg_half_speed : 1;
    /* prev_chg_half_speed - Bits[3:3], RW1CS, default = 1'b0 
       
       	The changed to half speed field from previous initialization abort.
             
     */
    UINT32 rsvd_4 : 1;
    /* rsvd_4 - Bits[4:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 previnitfailtype : 3;
    /* previnitfailtype - Bits[7:5], RW1CS, default = 3'b0 
       
       	Previous Initialization Failure Type.
       	The Initialization failure type from previous initialization abort.  
       	This field is updated when the state is exited on an init failure. 
       	000 - Link width negotiation failed (if state tracker indicates config)  
       	010 - Timed out and all lanes/Rx bad  
        101 - Link-up Identifier mismatch, re-initialized by clearing Link-up 
       Identifier   
       	111 - No good clock lanes
       	Other values are reserved 
             
     */
    UINT32 prevtxstatetracker : 5;
    /* prevtxstatetracker - Bits[12:8], RW1CS, default = 5'b0 
       
       	Previous Tx State Tracker.
        Most Recent Tx state before entering Reset from previous initialization abort.  
        
       	Refer to tx_state_tracker field in PQ_CSR_PHPIS for encoding description.  
             
     */
    UINT32 rsvd_13 : 3;
    /* rsvd_13 - Bits[15:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 prevrxstatetracker : 5;
    /* prevrxstatetracker - Bits[20:16], RW1CS, default = 5'b0 
       
       	Previous Rx State Tracker.
        Most recent/farthest Rx state before entering Reset from previous 
       initialization abort.    
       	Refer to tx_state_tracker field in PQ_CSR_PHPIS for encoding description.  
             
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 prevackstat : 2;
    /* prevackstat - Bits[25:24], RW1CS, default = 2'b0 
       
       	Previous ACK Status.
       	ACK status of the most recent state from previous initialization abort.  
             
     */
    UINT32 previnitialspeed : 1;
    /* previnitialspeed - Bits[26:26], RW1CS, default = 1'b0 
       
       	Previous Initialization Speed.
       	Initialization speed of previous initialization abort.  
             
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_PPS_QPIAGENT_REUT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIREUT_PH_WCI_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x40143148)                                                  */
/*       IVT_EX (0x40143148)                                                  */
/*       HSX (0x40143148)                                                     */
/*       BDX (0x40143148)                                                     */
/* Register default value on IVT_EP:    0x00000043                            */
/* Register default value on IVT_EX:    0x00000043                            */
/* Register default value on HSX:       0x00000001                            */
/* Register default value on BDX:       0x00000001                            */
#define QPIREUT_PH_WCI_QPIAGENT_REUT_REG 0x06024148
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * REUT Width Capability Indicator Register
 */
typedef union {
  struct {
    UINT32 configdtxwci : 11;
    /* configdtxwci - Bits[10:0], RWS_L, default = 11'h1 
       
       	Configured TxWCI. 
        This field specifies supported configurations and is used as a mask with the 
       dropped lanes to compute the TxWCI sent in the TS. Encoding is in the Intel QPI 
       Specification. 
       	Supported width modes:
       	XXX_XXXX_XXX1 - Q3, Q2, Q1, Q0
       	XXX_XXXX_XX1X - Q1, Q0
       	XXX_XXXX_X1XX - Q2, Q0
       	XXX_XXXX_1XXX - Q3, Q0
       	XXX_XXX1_XXXX - Q2, Q1
       	XXX_XX1X_XXXX - Q3, Q1
       	XXX_X1XX_XXXX - Q3, Q2
       	XXX_1XXX_XXXX - Q0
       	XX1_XXXX_XXXX - Q1
       	X1X_XXXX_XXXX - Q2
       	1XX_XXXX_XXXX - Q3
             
     */
    UINT32 rsvd : 21;
    /* rsvd - Bits[31:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_WCI_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_PH_LMS_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x4014314C)                                                  */
/*       IVT_EX (0x4014314C)                                                  */
/*       HSX (0x4014314C)                                                     */
/*       BDX (0x4014314C)                                                     */
/* Register default value:              0x00000000                            */
#define QPIREUT_PH_LMS_QPIAGENT_REUT_REG 0x0602414C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * Lane Map Status
 */
typedef union {
  struct {
    UINT32 computedrxwci : 11;
    /* computedrxwci - Bits[10:0], RO_V, default = 11'b0 
       
       	Computed Rx WCI.
        A list of LMs supported by Rx link. This WCI is combination of remote TxWCI and 
       local RxWCI.  
       	Rx link picks up RxCLM from this field according to selection policy.
       	Refer to the Intel QPI specification for decoding of this field.
             
     */
    UINT32 rsvd_11 : 5;
    /* rsvd_11 - Bits[15:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 localrxclm : 4;
    /* localrxclm - Bits[19:16], RO_V, default = 4'b0 
       
       	Local RxCLM
        LM used by RX link (after reversal if lane reversal detected, that is, will be 
       reverse of remote TxCLM). 
             
     */
    UINT32 rsvd_20 : 4;
    /* rsvd_20 - Bits[23:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 localtxclm : 4;
    /* localtxclm - Bits[27:24], RO_V, default = 4'b0 
       
       	Local TxCLM
       	LM used by Tx link.
             
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_LMS_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_PH_PLS_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x40143150)                                                  */
/*       IVT_EX (0x40143150)                                                  */
/*       HSX (0x40143150)                                                     */
/*       BDX (0x40143150)                                                     */
/* Register default value on IVT_EP:    0x00000001                            */
/* Register default value on IVT_EX:    0x00000001                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define QPIREUT_PH_PLS_QPIAGENT_REUT_REG 0x06024150


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.3.CFG.xml.
 * REUT Physical Link Status
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    /* rsvd_0 - Bits[0:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rcvdclkstat : 1;
    /* rcvdclkstat - Bits[1:1], RO_V, default = 1'b0 
       
       	Received Clock Status
       	0: No received clock (on RX)
       	1: Received clock stable (on RX)
        NOTE: Received clock is monitored on a continuous basis and this bit updated 
       accordingly. 
             
     */
    UINT32 latencybuffrollowver : 1;
    /* latencybuffrollowver - Bits[2:2], RO_V, default = 1'b0 
       
       	Latency Buffer Rollover
        1 - Indicates the latency buffer has Rolled over while attempting to fix the 
       link latency. 
        The buffer rollover occurs if requested received target link latency needs a 
       depth beyond available latency buffer. 
             
     */
    UINT32 driftbufferr : 1;
    /* driftbufferr - Bits[3:3], RW1CS, default = 1'b0 
       
       	Drift Buffer Error
        When set to 1 indicates when the RX drift buffer has overflown or underflown 
       during the normal operation (not during inband reset) in any Rx lane. 
             
     */
    UINT32 driftbuffalarm : 1;
    /* driftbuffalarm - Bits[4:4], RW1CS, default = 1'b0 
       
       	Drift Buffer Alarm
        A value of 1 indicates the RX drift buffer depth (difference between read and 
       write pointers in equal to or less than the underflow and overflow) is less than 
       the drift buffer alarm threshold depth in any Rx lane. 
        Note: An implementation may do an in-band reset which will re-initialize the 
       drift buffer. 
       	The drift buffer alarm cant be set during Reset.
             
     */
    UINT32 rsvd_5 : 2;
    UINT32 rxinbandreset : 1;
    /* rxinbandreset - Bits[7:7], RW1CS, default = 1'b0 
       
       	Rx Inband Reset
       	1: Indicates that inband reset was detected at the local RX.
             
     */
    UINT32 rsvd_8 : 8;
    /* rsvd_8 - Bits[15:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 driftbufflanealarm : 5;
    /* driftbufflanealarm - Bits[20:16], ROS_V, default = 5'b0 
       
       	Drift Buffer Alarm Lane
        The lane ID of first lane that caused Drift Buffer Alarm to set. Valid only 
       when alarm is set. 
             
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fwdclklaneinuse : 3;
    /* fwdclklaneinuse - Bits[26:24], RO_V, default = 3'b0 
       
       	Forwarded Clock Lane in Use Applies to Tx.
       	001: Primary
       	010: Alternate 1
       	100: Alternate 2
       	others: Reserved
             
     */
    UINT32 rsvd_27 : 1;
    /* rsvd_27 - Bits[27:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 recdclklaneinuse : 3;
    /* recdclklaneinuse - Bits[30:28], RO_V, default = 3'b0 
       
       	Received Clock Lane in Use Applies to RX.
       	001: Primary
       	010: Alternate 1
       	100: Alternate 2
       	others - Reserved
             
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_PLS_QPIAGENT_REUT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIREUT_PH_PTV_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x40143154)                                                  */
/*       IVT_EX (0x40143154)                                                  */
/*       HSX (0x40143154)                                                     */
/*       BDX (0x40143154)                                                     */
/* Register default value on IVT_EP:    0x00000102                            */
/* Register default value on IVT_EX:    0x00000102                            */
/* Register default value on HSX:       0x01030102                            */
/* Register default value on BDX:       0x01030102                            */
#define QPIREUT_PH_PTV_QPIAGENT_REUT_REG 0x06024154
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * REUT Physical Primary Time-Out Value
 */
typedef union {
  struct {
    UINT32 etdebounce : 4;
    /* etdebounce - Bits[3:0], RWS_L, default = 4'h2 
       
        Exponential count for TDEBOUNCE Time-out value is 2^(count in this field) * 1 
       TSL.   
             
     */
    UINT32 rsvd_4 : 4;
    /* rsvd_4 - Bits[7:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 etinbandrstinit : 4;
    /* etinbandrstinit - Bits[11:8], RWS_L, default = 4'h1 
       
        Exponential count for TINBAND_RESET_INIT Time-out value is 2^(count in field) * 
       128 TSL.   
             
     */
    UINT32 rsvd_12 : 4;
    /* rsvd_12 - Bits[15:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 etpollingbitlock : 4;
    /* etpollingbitlock - Bits[19:16], RWS_L, default = 4'h3 
       
          Exponential count for TPOLLING.BitLock Time-out value is 2^(count in this 
       field) * 128 TSL.   
             
     */
    UINT32 etadapt_next : 4;
    /* etadapt_next - Bits[23:20], RWS_L, default = 4'b0 
       
       	Exponential Time Adapt Time
        Tx will source PRBS pattern and Rx will evaluate eye for this period for each 
       coefficient / index for 2^(count in this field) * 128*TSL. 
        Note: default value will not be suitable for all platforms; appropriate value 
       must be determined for each platform. 
             
     */
    UINT32 etignore : 4;
    /* etignore - Bits[27:24], RWS_L, default = 4'h1 
       
       	Exponential Time Ignore Pattern
        Count not 0: Rx will ignore pattern at beginning and end of adapt period for 
       each coefficient / index for 2^(count in this field)*TSL UI.  
       	Count is 0: ignore period is 0.
             
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_PTV_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_PH_STV_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x40143158)                                                  */
/*       IVT_EX (0x40143158)                                                  */
/*       HSX (0x40143158)                                                     */
/*       BDX (0x40143158)                                                     */
/* Register default value on IVT_EP:    0x88880808                            */
/* Register default value on IVT_EX:    0x88880808                            */
/* Register default value on HSX:       0x08880808                            */
/* Register default value on BDX:       0x08880808                            */
#define QPIREUT_PH_STV_QPIAGENT_REUT_REG 0x06024158


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.3.CFG.xml.
 * REUT Physical Secondary Time-Out Value
 */
typedef union {
  struct {
    UINT32 etdetectfwdclk : 4;
    /* etdetectfwdclk - Bits[3:0], RWS_L, default = 4'h8 
       
        Exponential count for TDETECT.FwdClk Time-out value is 2^(count in this field) 
       * 128 TSL.   
             
     */
    UINT32 rsvd_4 : 4;
    /* rsvd_4 - Bits[7:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 etpolllanedeskew : 4;
    /* etpolllanedeskew - Bits[11:8], RWS_L, default = 4'h8 
       
        Exponential count for TPOLLING.LaneDeskew Time-out value is 2^(count in this 
       field) * 128 TSL.   
             
     */
    UINT32 rsvd_12 : 4;
    /* rsvd_12 - Bits[15:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 etpollingparam : 4;
    /* etpollingparam - Bits[19:16], RWS_L, default = 4'h8 
       
        Exponential count for TPOLLING.Param Time-out value is 2^(count in this field) 
       * 128 TSL.   
             
     */
    UINT32 etconfiglinkwidth : 4;
    /* etconfiglinkwidth - Bits[23:20], RWS_L, default = 4'h8 
       
        Exponential count for TCONFIG.LinkWidth Time-out value is 2^(count in this 
       field) * 128 TSL.   
             
     */
    UINT32 etconfigflitlock : 4;
    /* etconfigflitlock - Bits[27:24], RWS_L, default = 4'h8 
       
        Exponential count for TCONFIG.FlitLock Time-out value is 2^(count in this 
       field) * 128 TSL.   
             
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_STV_QPIAGENT_REUT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIREUT_PH_LDC_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x4014315C)                                                  */
/*       IVT_EX (0x4014315C)                                                  */
/*       HSX (0x4014315C)                                                     */
/*       BDX (0x4014315C)                                                     */
/* Register default value:              0x00000804                            */
#define QPIREUT_PH_LDC_QPIAGENT_REUT_REG 0x0602415C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * REUT Physical Determinism Control
 */
typedef union {
  struct {
    UINT32 driftbufalarmthr : 6;
    /* driftbufalarmthr - Bits[5:0], RWS_L, default = 6'h4 
       
       	Drift Buffer Alarm Threshold
        When the difference (both underflow and overflow) between drift buffer read and 
       write pointers (depth) 
       	is less than the value in this field, drift buffer alarm status will be set.
        Note: For processor, must be less than drift buffer latency and multiple of 
       4UI. 
             
     */
    UINT32 rsvd_6 : 2;
    /* rsvd_6 - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inidriftbufflatency : 6;
    /* inidriftbufflatency - Bits[13:8], RWS_L, default = 6'h8 
       
       	Initial Drift Buffer Latency
        Drift Buffer Depth refers to the initialized difference between read and writer 
       pointers in RX drift buffer. 
        The field indicates the difference between the drift buffer read and write 
       pointers in UI, to be set during Polling.BitLock, in all lanes. 
        Note: For processor, the drift buffer depth is 32UI and must be programmed to 
       multiple of 4UI 
             
     */
    UINT32 rsvd_14 : 2;
    /* rsvd_14 - Bits[15:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 targetlinklatency : 8;
    /* targetlinklatency - Bits[23:16], RWS_L, default = 8'b0 
       
        The Target Link Latency field Specifies the target link latency value in UI 
       that the remote port needs to adjust to. 
        TX will introduce these bits in TS.Param training sequence as the Target Link 
       Latency field. 
             
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_LDC_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_PH_LDS_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x40143160)                                                  */
/*       IVT_EX (0x40143160)                                                  */
/*       HSX (0x40143160)                                                     */
/*       BDX (0x40143160)                                                     */
/* Register default value:              0x00000000                            */
#define QPIREUT_PH_LDS_QPIAGENT_REUT_REG 0x06024160
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * REUT Physical Determinism Status
 */
typedef union {
  struct {
    UINT32 remotereqfixrxlatency : 8;
    /* remotereqfixrxlatency - Bits[7:0], RO_V, default = 8'b0 
       
       	Received Target Link Latency
        Target Link Latency value received in TS.Param. Remote port requests local Rx 
       to fix the link latency for this value. 
             
     */
    UINT32 tsphasediff : 6;
    /* tsphasediff - Bits[13:8], RO_V, default = 6'b0 
       
       	TS Phase Difference
        The Phase difference in UI, between received TS.Param and local TS.Param. This 
       field provides observed link latency. 
             
     */
    UINT32 tscntdiff : 8;
    /* tscntdiff - Bits[21:14], RO_V, default = 8'b0 
       
       	TSCount Difference 
       	The Difference of Rx TSCount received in TS.Param and current local TSCount. 
       	This field along with TS phase difference provides observed link latency.
             
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[23:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 latbuffdepth : 8;
    /* latbuffdepth - Bits[31:24], RO_V, default = 8'b0 
       
       	Latency Buffer depth Used
       	Amount of Latency buffer used in UI, for latency fixing. 
       	The result is latched after the completion of latency fixing operation. 
       	If latency fixing is disabled, value is zero.
             
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_LDS_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_PH_PRT_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x40143164)                                                  */
/*       IVT_EX (0x40143164)                                                  */
/*       HSX (0x40143164)                                                     */
/*       BDX (0x40143164)                                                     */
/* Register default value:              0x00322808                            */
#define QPIREUT_PH_PRT_QPIAGENT_REUT_REG 0x06024164


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.3.CFG.xml.
 * Intel QPI Periodic Retraining Timer Register
 */
typedef union {
  struct {
    UINT32 retraininterval : 8;
    /* retraininterval - Bits[7:0], RWS_L, default = 8'h8 
       
       	TRetraining_Interval
       	Periodic Retraining Interval - Each count represents 1024 UI (16 TSL).  
       	A value of 0 indicates periodic retraining is disabled.  
       	Retraining must be disabled in slow mode. 
       	Value to be programmed by firmware. 
             
     */
    UINT32 rsvd_8 : 2;
    /* rsvd_8 - Bits[9:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 expcntretraininterval : 4;
    /* expcntretraininterval - Bits[13:10], RWS_L, default = 4'hA 
       
       	ETRetraining_Interval.
        Exponential count for Retraining Interval  Interval value is multiplied by 
       2^(count in this field).   
             
     */
    UINT32 rsvd_14 : 2;
    /* rsvd_14 - Bits[15:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 retrainpacketcnt : 6;
    /* retrainpacketcnt - Bits[21:16], RWS_L, default = 6'h32 
       
       	Retraining Packet Count.
       	Number of retraining packets sent for each retraining duration.   
       	Retraining duration = (count + 1) * TSL UI.
        A retraining packet is 1 TSL of clock pattern (..1010..) or PRBS pattern, 
       starting with a 0.    
        Each Tx lane generates clock/PRBS pattern and each Rx lane expects clock/PRBS 
       pattern.   
       	This field must be greater than or equal to 2. 
             
     */
    UINT32 durationgranularity : 1;
    /* durationgranularity - Bits[22:22], RO, default = 1'b0 
       
       	Duration granularity
       	1 Reserved
       	0 indicates agent is using 64 UI granularity 
             
     */
    UINT32 rsvd_23 : 9;
    /* rsvd_23 - Bits[31:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_PRT_QPIAGENT_REUT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIREUT_PAT_GCTR_QPIAGENT_REUT_REG supported on:                           */
/*       IVT_EP (0x40143168)                                                  */
/*       IVT_EX (0x40143168)                                                  */
/*       HSX (0x40143168)                                                     */
/*       BDX (0x40143168)                                                     */
/* Register default value on IVT_EP:    0x06800084                            */
/* Register default value on IVT_EX:    0x06800084                            */
/* Register default value on HSX:       0x06000000                            */
/* Register default value on BDX:       0x06000000                            */
#define QPIREUT_PAT_GCTR_QPIAGENT_REUT_REG 0x06024168


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.3.CFG.xml.
 * REUT Pattern Generator
 */
typedef union {
  struct {
    UINT32 strttst : 1;
    /* strttst - Bits[0:0], RW1S, default = 1'b0 
       
       	Start test
       	If QPITTCAP(0) is 0, test starts if this bit is set.
        1 - Start the test - Initiates transition from Loopback.Marker to 
       Loopback.Pattern. 
       	The bit is cleared when Loopback.Pattern is exited.
             
     */
    UINT32 stponerr : 1;
    /* stponerr - Bits[1:1], RWS_L, default = 1'b0 
       
       	Stop test on Error.
       	Exit Loopback.Pattern to Loopback.Marker on error - Flag.
       	0 - Do not stop test on error
       	1 - Stop test and latch Loop Status on first error in pattern checker.
             
     */
    UINT32 autoinv : 1;
    /* autoinv - Bits[2:2], RWS_L, default = 1'b0 
       
       	Auto inversion rotate enable.
        If set, this bit will enable logical rotation of the QPITTPIS register during 
       the test. 
        Each lane begins by selecting the appropriate bit in the Pattern Invert Select 
       field, and sequentially the next higher bit, wrapping back to bit 0, following 
       each loop. 
       	1 - Logical rotation of Buffer Invert Select
       	0 - No logical rotation of Buffer INvert Select
             
     */
    UINT32 psuedorandomen1 : 1;
    /* psuedorandomen1 - Bits[3:3], RWS_L, default = 1'b0 
       
       	Pseudo-random Enable1.
       	Enable Pseudo random pattern generation in Pattern Generator Buffer 1.
       	0 - Use Pattern Buffer 1 as 128 bit pattern.
       	1 - Use Pattern Buffer 1 XORed with the LFSR.
             
     */
    UINT32 psuedorandomen2 : 1;
    /* psuedorandomen2 - Bits[4:4], RWS_L, default = 1'b0 
       
       	Pseudo-random Enable2.
       	Enable Pseudo random pattern generation in Pattern Generator Buffer 2.
       	0 - Use Pattern Buffer 2 as 128 bit pattern.
       	1 - Use Pattern Buffer 2 XORed with the LFSR.
             
     */
    UINT32 slaveignorremoteparam : 1;
    /* slaveignorremoteparam - Bits[5:5], RO, default = 1'b0 
       
       	Ignore Remote Parameter
        If set, Remote parameter received in TS.Loopback with electrical parameter 
       during loop back slave is ignored. Instead, local parameters are applied in 
       Loopback.Pattern. 
             
     */
    UINT32 rsvd_6 : 1;
    /* rsvd_6 - Bits[6:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 incldroppedlanes : 1;
    /* incldroppedlanes - Bits[7:7], RWS_L, default = 1'b0 
       
       	Include Dropped Lanes.
        If set, include Tx lanes marked TxLaneDropped and Rx lanes marked RxLaneDropped 
       in Loopback.Marker except during re-locking operation) and Loopback.Pattern with 
       deskew setting of 0. 
             
     */
    UINT32 rsvd_8 : 1;
    /* rsvd_8 - Bits[8:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pattbuffsel : 2;
    /* pattbuffsel - Bits[10:9], RWS_L, default = 2'b0 
       
        Pattern buffer selection used to index pattern buffers through the QPITTPGB 
       register. 
       	00 - Access pattern buffer 1.
       	01 - Access pattern buffer 2.
       	10 - Access Loopback LFSR1 seed.
       	11 - Access Loopback LFSR2 seed.
             
     */
    UINT32 rsvd_11 : 1;
    /* rsvd_11 - Bits[11:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pattbuffwordsel : 2;
    /* pattbuffwordsel - Bits[13:12], RWS_L, default = 2'b0 
       
        Pattern buffer word selection used to index each of 32 bits in pattern buffers 
       through QPITTPGB. 
       	00 - Access [31:0] in selected pattern buffer.
       	01 - Access [63:32] in selected pattern buffer.
       	10 - Access [95:64] in selected pattern buffer.
       	11 - Access [127:96] in selected pattern buffer.
       	(this field does not apply if Loopback LFSR seed selected)
             
     */
    UINT32 rsvd_14 : 2;
    /* rsvd_14 - Bits[15:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 loop_cnt_inhibit : 1;
    /* loop_cnt_inhibit - Bits[16:16], RO, default = 1'b0 
       
        Valid only if the Loop Counter Inhibit Capability is 1. This field may be used 
       to inhibit the counting in the Loop Counter when it is set. Normally this would 
       be used when setting Error Counter Inhibit, so there is an accurate reading of 
       the Loop Counter Status.  
       	0 - Normal counting of pattern check loops
       	1 - Inhibit counting of pattern check loops
             
     */
    UINT32 lfsr1_stagger_en : 1;
    /* lfsr1_stagger_en - Bits[17:17], RWS_L, default = 1'b0 
       
        Selects a staggered scrambling pattern to be generated when LFSR1 is selected 
       for a pseudorandom pattern. 
       	0 - Do not stagger the LFSR1 pattern
       	1 - Enable staggering of the LFSR1 pattern
             
     */
    UINT32 lfsr2_stagger_en : 1;
    /* lfsr2_stagger_en - Bits[18:18], RWS_L, default = 1'b0 
       
        Selects a staggered scrambling pattern to be generated when LFSR2 is selected 
       for a pseudorandom pattern. 
       	0 - Do not stagger the LFSR2 pattern
       	1 - Enable staggering of the LFSR2 pattern
             
     */
    UINT32 drive0or1 : 1;
    /* drive0or1 - Bits[19:19], RO, default = 1'b0 
       
       	Drive DC Zero Instead of One.
       	Selects the DC Logic Value to use when QPITTDDC selects a DC pattern override.
       	0 - Drive a logic 1 for all lanes selected in QPITTDDC
       	1 - Drive a logic 0 for all lanes selected in QPITTDDC
             
     */
    UINT32 stoptest : 1;
    /* stoptest - Bits[20:20], RW1S, default = 1'b0 
       
       	if QPITTCAP(0) is 0, setting this bit stops the test. 
       	1 - Stops the test - forces an exit from Loopback.Pattern.
       	The bit is cleared when Loopback.Pattern is exited
             
     */
    UINT32 errcntstall : 1;
    /* errcntstall - Bits[21:21], RWS_L, default = 1'b0 
       
       	Error Counters Inhibit.
       	Inhibits the counting of errors when in Loopback.Pattern.
       	0 - Normal Error counting
       	1 - Inhibit Error counting
             
     */
    UINT32 lfsrpolynomialsel : 1;
    /* lfsrpolynomialsel - Bits[22:22], RO, default = 1'b0 
       
       	Polynomial Select.
       	0 - Use the reciprocal of the ITU polynomial
       	1 - Use the standard ITU polynomial
             
     */
    UINT32 autobufselenable : 1;
    /* autobufselenable - Bits[23:23], RWS_L, default = 1'b0 
       
       	Rotate Pattern Buffer Selection.
        If set, this bit will enable rotation of the QPITTPG2S register during the 
       test. The actual register is not rotated, but an operating copy is made at the 
       beginning of the test, and then the copy is rotated across all lanes of the 
       interface. 
       	0 - Rotation is disabled
       	1 - Rotation is enabled
             
     */
    UINT32 exploopcnt : 5;
    /* exploopcnt - Bits[28:24], RWS_L, default = 5'h6 
       
       	Exponential Loop Count.
       	Exponential Test Loop Count.
       	Loop count is 2^(count in this field).
        Each increment in loop counter indicates 128 UI of Pattern Buffer Register 
       content. 
       	0 - Infinite count. Use Soft Reset to exit test.
             
     */
    UINT32 rsvd_29 : 3;
    /* rsvd_29 - Bits[31:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PAT_GCTR_QPIAGENT_REUT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIREUT_PAT_CCTR_QPIAGENT_REUT_REG supported on:                           */
/*       IVT_EP (0x4014316C)                                                  */
/*       IVT_EX (0x4014316C)                                                  */
/*       HSX (0x4014316C)                                                     */
/*       BDX (0x4014316C)                                                     */
/* Register default value on IVT_EP:    0x00000F02                            */
/* Register default value on IVT_EX:    0x00000F02                            */
/* Register default value on HSX:       0x00000002                            */
/* Register default value on BDX:       0x00000002                            */
#define QPIREUT_PAT_CCTR_QPIAGENT_REUT_REG 0x0602416C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.3.CFG.xml.
 * Pattern Checker Control
 */
typedef union {
  struct {
    UINT32 init_check_mask : 4;
    /* init_check_mask - Bits[3:0], RWS_L, default = 4'h2 
       
       	TInitialCheckMask.
        The Initial Check Mask is used for a settling time for any Electrical Parameter 
       offsets that are applied upon entry to Loopback.Pattern and for settling time 
       for any Electrical Parameter Global values applied upon re-entry to 
       Loopback.Marker.  
        Upon entry to Loopback.Pattern, no RX lane errors will be logged during the 
       Initial Check Mask period. 
       	Initial check mask period is 2^(Count in this field) * 1TSL.
       	0 - Indicates no masking.
             
     */
    UINT32 rsvd_4 : 1;
    /* rsvd_4 - Bits[4:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 sel_err_check_int : 3;
    /* sel_err_check_int - Bits[7:5], RWS_L, default = 3'b0 
       
       	Select error check interval.
       	The processor will keep using 1.0 spec definition for this field.
        Selective Error Check Interval = 2^(Count in this Field). Checks error for 1UI 
       at every Error Check Interval starting from Error Check Start. For Example, 
       	(Error Check Start, Error Check Interval) - Expected behavior.
       	(0, 0) - Checks errors on every UI.
       	(0, 1) - Checks errors on even UI.
       	(1, 1) - Checks errors on odd UI.
       	(0, 2) - Checks errors starting with bit 0 repeat every 4UI.
       	(1, 2) - Checks errors starting with bit 1 repeat every 4UI.
       	..
        (34, 7) - Checks errors starting with bit 34 repeat every 128UI.  ... and so 
       on. 
       	Values 2-6 are optional, but encouraged.
             
     */
    UINT32 sel_err_check_start : 7;
    /* sel_err_check_start - Bits[14:8], RWS_L, default = 7'b0 
       
       	Select Error Check Start. 
       	The processor will keep using 1.0 spec definition for this field.
        The initial UI in the pattern buffer to start checking errors in the arriving 
       pattern. 
             
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rx_err_counter_lane_sel : 5;
    /* rx_err_counter_lane_sel - Bits[20:16], RWS_L, default = 5'b0 
       
        The value of RX Error Counter Lane Select is used to indicate which Lane will 
       have the full 32 bit error counter and the other Lanes will have only 8 bit 
       error counters that are observable using RX Error Counter Lane Select. 
       	0: Rx Lane 0 Selected.
       	1: Rx Lane 1 Selected.
       	Etc..
             
     */
    UINT32 rsvd_21 : 5;
    /* rsvd_21 - Bits[25:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ext_err_ctr_lane_assign : 1;
    /* ext_err_ctr_lane_assign - Bits[26:26], RO, default = 1'b0 
       
        This field is valid only if Extended Count Lane Assign Capability is set. This 
       bit is used to force a reassignment of the 31-bit error counter. Entry into 
       Loopback.Pattern continues to also assign the long error counter lane. 
        1 - Use the current RxErrCntLaneSel value for assigning the lane that will use 
       the 31-bit error counter  
        0 - Only normal entry to Loopback.Pattern will cause the lane assignment to use 
       the 31-bit error counter 
             
     */
    UINT32 ext_err_ctr_all_lanes : 1;
    /* ext_err_ctr_all_lanes - Bits[27:27], RO, default = 1'b0 
       
        This field is only valid if Extended Count All Lanes Capability is 1. The user 
       can enable counting of all bit errors for the complete set of lanes for the 
       tested lanes, starting from the lane selected in Extended Error Counter Lane 
       Select and including any  consecutive higher lanes included in the testing. 
        These errors are accumulated using the Extended Error Counter. Any per-lane 
       error counters are invalid for the tested lanes. 
       	1 - Accumulate all errors for all lanes
       	0 - Per-lane error counters are used
             
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PAT_CCTR_QPIAGENT_REUT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIREUT_PAT_BA_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x40143170)                                                  */
/*       IVT_EX (0x40143170)                                                  */
/*       HSX (0x40143170)                                                     */
/*       BDX (0x40143170)                                                     */
/* Register default value:              0x4DAE1577                            */
#define QPIREUT_PAT_BA_QPIAGENT_REUT_REG 0x06024170
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * Pattern Buffer Access Register
 */
typedef union {
  struct {
    UINT32 reutpattbuff : 32;
    /* reutpattbuff - Bits[31:0], RWS_L, default = 32'h4DAE1577 
       
       	All words of Pattern generator buffers are accessed through this register.
        A particular pattern buffer is indexed using Pattern buffer selection bits in 
       the QPIREUT_PAT_GCTR register  
        Patterns larger than 32 bits are indexed using pattern buffer word selection 
       field in the QPIREUT_PAT_GCTR register. 
        Each pattern buffer is 128 bit, accessed 32 bit at a time. Least significant 
       bit is sent out first but pattern is not shifted. 
        PRBS pattern is generated by the 23 bit LFSR using ITU-T PRBS23 polynomial x^23 
       + x^18 + 1 and the initial seed is programmed using bits 0:23.  
       	PatBuf1 Default
       	(msdw = most significant Dword)
       	msdw (D3): ACE3_C3E0h
       	msdw-1 (D2): 100F_F7FFh
       	msdw-2 (D1): ACBC_B211h
       	lsdw (D0): 4DAE_1577h
             
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PAT_BA_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_PAT_BS_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x40143174)                                                  */
/*       IVT_EX (0x40143174)                                                  */
/*       HSX (0x40143174)                                                     */
/*       BDX (0x40143174)                                                     */
/* Register default value:              0x00000000                            */
#define QPIREUT_PAT_BS_QPIAGENT_REUT_REG 0x06024174
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * Pattern Generator Buffer 2 Selection Register
 */
typedef union {
  struct {
    UINT32 pattbuflanesel : 20;
    /* pattbuflanesel - Bits[19:0], RWS_L, default = 20'b0 
       
       	Lane selects Pattern buffer 2 for its testing.
       	One bit per each lane
       	Bit 0 - Controls Lane 0
       	Bit 1 - Controls Lane 1
       	..., and so on.
       	1 - Corresponding lane will use Pattern Buffer 2 for test.
             
     */
    UINT32 rsvd : 12;
    /* rsvd - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PAT_BS_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_PAT_REC_QPIAGENT_REUT_REG supported on:                            */
/*       IVT_EP (0x40143178)                                                  */
/*       IVT_EX (0x40143178)                                                  */
/*       HSX (0x40143178)                                                     */
/*       BDX (0x40143178)                                                     */
/* Register default value:              0x00000000                            */
#define QPIREUT_PAT_REC_QPIAGENT_REUT_REG 0x06024178
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * Rx Lane Error Check Disable Register
 */
typedef union {
  struct {
    UINT32 rxerrchdisable : 20;
    /* rxerrchdisable - Bits[19:0], RWS_L, default = 20'b0 
       
        Rx Lane Error Check Disable One bit for each Rx lane, if set, will disable 
       error check and accumulation during loop back test. 
       	Bit 0 - Rx Lane 0.
       	Bit 1 - Rx Lane 1.
       	...
       	Bit 19 - Rx Lane 19.
             
     */
    UINT32 rsvd : 12;
    /* rsvd - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PAT_REC_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_PAT_IS_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x4014317C)                                                  */
/*       IVT_EX (0x4014317C)                                                  */
/*       HSX (0x4014317C)                                                     */
/*       BDX (0x4014317C)                                                     */
/* Register default value:              0x00000001                            */
#define QPIREUT_PAT_IS_QPIAGENT_REUT_REG 0x0602417C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * Pattern Invert Selection
 */
typedef union {
  struct {
    UINT32 patterninvert : 20;
    /* patterninvert - Bits[19:0], RWS_L, default = 20'b1 
       
       	Pattern Invert Selection
       	One bit for each Tx lane.
       	Bit 0 - Tx Lane 0
       	Bit 1 - Tx Lane 1
       	.., and so on.
       	1 - Invert the pattern in corresponding Tx lane.
        This register sets the initial inversion phase and is not dynamically updated 
       as the pattern progresses. 
             
     */
    UINT32 rsvd : 12;
    /* rsvd - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PAT_IS_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_PAT_DCD_QPIAGENT_REUT_REG supported on:                            */
/*       IVT_EP (0x40143180)                                                  */
/*       IVT_EX (0x40143180)                                                  */
/*       HSX (0x40143180)                                                     */
/*       BDX (0x40143180)                                                     */
/* Register default value:              0x00000000                            */
#define QPIREUT_PAT_DCD_QPIAGENT_REUT_REG 0x06024180
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * Tx testability DC Data Lane Control Register
 */
typedef union {
  struct {
    UINT32 txdatlanctr : 20;
    /* txdatlanctr - Bits[19:0], RWS_L, default = 20'b0 
       
       	TX DC Data Lane Control
        A bit mask used for selectively overriding the data TX to output a DC logical 1 
       condition during Loopback.Pattern. 
        Used for debug and validation purposes to quiet neighboring crosstalk lines, 
       and is only applied during Loopback.Pattern. 
       	Only the QPIREUT_PH_TDC register will override the behavior of this register.
       	A bit value of 1 indicates the corresponding lane is transmitting a logical 1.
       	Bit 0: Controls Lane 0.
       	Bit 1: Controls Lane 1.
       	.. and so on.
             
     */
    UINT32 rsvd : 12;
    /* rsvd - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PAT_DCD_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_PAT_LEC_QPIAGENT_REUT_REG supported on:                            */
/*       IVT_EP (0x40143184)                                                  */
/*       IVT_EX (0x40143184)                                                  */
/*       HSX (0x40143184)                                                     */
/*       BDX (0x40143184)                                                     */
/* Register default value:              0x00000000                            */
#define QPIREUT_PAT_LEC_QPIAGENT_REUT_REG 0x06024184
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * Lane Error Counter
 */
typedef union {
  struct {
    UINT32 laneerrcnt : 31;
    /* laneerrcnt - Bits[30:0], RW1CS, default = 31'b0 
       
       	The processor uses Intel QPI 1.0 spec format.
        Lane Error Counter is the accumulation of errors in a participating Rx lane. 
       The lane being observed is indicated by Rx Lane Selection bit in the QPITTPCC 
       register. 
       	Counter rolls over after setting Error Overflow bit.
       	Cleared with writing 1 or PowerGood Reset.
       	Error count accumulates during the test.
             
     */
    UINT32 errovrflow : 1;
    /* errovrflow - Bits[31:31], RW1CS, default = 1'b0 
       
       	Error Overflow
       	If set, indicates that the error counter has overflown in selected lane.
       	Cleared with writing 1 or PowerGood Reset.
             
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PAT_LEC_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_PAT_LCS_QPIAGENT_REUT_REG supported on:                            */
/*       IVT_EP (0x40143188)                                                  */
/*       IVT_EX (0x40143188)                                                  */
/*       HSX (0x40143188)                                                     */
/*       BDX (0x40143188)                                                     */
/* Register default value:              0x00000000                            */
#define QPIREUT_PAT_LCS_QPIAGENT_REUT_REG 0x06024188
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * Loop Counter Status
 */
typedef union {
  struct {
    UINT32 loopcntstat : 32;
    /* loopcntstat - Bits[31:0], RO_V, default = 32'b0 
       
        This register reflects the loop count in a test. When the test mode is stop on 
       error, this provides final loop count. With this count and the seed used in the 
       LFSR, the fail pattern can be recreated. 
        The Loopcount register assumes an incrementing Loopcount with rollover when it 
       reaches the max value. 
             
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PAT_LCS_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_PAT_RES_QPIAGENT_REUT_REG supported on:                            */
/*       IVT_EP (0x4014318C)                                                  */
/*       IVT_EX (0x4014318C)                                                  */
/*       HSX (0x4014318C)                                                     */
/*       BDX (0x4014318C)                                                     */
/* Register default value:              0x00000000                            */
#define QPIREUT_PAT_RES_QPIAGENT_REUT_REG 0x0602418C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * Rx Error Status
 */
typedef union {
  struct {
    UINT32 rxlaneerrstat : 20;
    /* rxlaneerrstat - Bits[19:0], RW1CS, default = 20'b0 
       
       	Rx Lane Error Status
       	One bit per each Rx lane 
       	Bit 0 - Status of Lane 0
       	Bit 1 - Status of Lane 1
       	..., and so on.
        Cleared with writing 1. Clearing a lane status also clears the lane error 
       counter for that lane (but not the extended error lane counter/overflow).  
       	1 - Corresponding lane has received error.
             
     */
    UINT32 rsvd : 12;
    /* rsvd - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PAT_RES_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_PM_R0_QPIAGENT_REUT_REG supported on:                              */
/*       IVT_EP (0x40143190)                                                  */
/*       IVT_EX (0x40143190)                                                  */
/*       HSX (0x40143190)                                                     */
/*       BDX (0x40143190)                                                     */
/* Register default value on IVT_EP:    0x00040412                            */
/* Register default value on IVT_EX:    0x00040412                            */
/* Register default value on HSX:       0x0004041A                            */
/* Register default value on BDX:       0x0004041A                            */
#define QPIREUT_PM_R0_QPIAGENT_REUT_REG 0x06024190


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.3.CFG.xml.
 * Power Management Register for L0s
 */
typedef union {
  struct {
    UINT32 tl0swake : 6;
    /* tl0swake - Bits[5:0], RWS_L, default = 6'h1A 
       
        Local L0s Wake-up time the remote agent must not violate. Set by firmware on 
       both link ports prior to entering L0s. 
        This field is at 16 UI granularity and the value of this field is (count + 
       1)*16 UI 
       	A value is 0 indicates that L0s is not supported on the local agent.
       	Min setting for HSX should be 'h1A
             
     */
    UINT32 rsvd_6 : 4;
    /* rsvd_6 - Bits[9:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tl0ssleepmin : 2;
    /* tl0ssleepmin - Bits[11:10], RWS_L, default = 2'b1 
       
       	TL0s_SLEEP_MIN
        QPI Specific Field Minimum time remote TX on a port initiating L0s entry should 
       stay in L0s. This 
        corresponds to the time required by local Rx to respond to L0s exit signal by 
       remote port. 
       	This field is decoded in the following way.
       	00: 32UI
       	01: 48 UI
       	10: 64UI
       	11: 96UI
             
     */
    UINT32 rsvd_12 : 4;
    /* rsvd_12 - Bits[15:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tl0s_sleep_mult : 4;
    /* tl0s_sleep_mult - Bits[19:16], RWS_L, default = 4'h4 
       
        Minimum time that remote TX must drive differential on non-sense lanes on L0s 
       entry is (128 * TL0s_SLEEP_MULT) UI 
             
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PM_R0_QPIAGENT_REUT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIREUT_PM_R1_QPIAGENT_REUT_REG supported on:                              */
/*       IVT_EP (0x40143194)                                                  */
/*       IVT_EX (0x40143194)                                                  */
/*       HSX (0x40143194)                                                     */
/*       BDX (0x40143194)                                                     */
/* Register default value on IVT_EP:    0x40080840                            */
/* Register default value on IVT_EX:    0x40080840                            */
/* Register default value on HSX:       0x40160850                            */
/* Register default value on BDX:       0x40160850                            */
#define QPIREUT_PM_R1_QPIAGENT_REUT_REG 0x06024194
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * Power Management Register for L0p TX
 */
typedef union {
  struct {
    UINT32 tl0penter2null : 8;
    /* tl0penter2null - Bits[7:0], RWS_L, default = 8'h50 
       
       	Period after PM.LinkWidthConfig flit where data flits are sent (A1 to A2).
       	This parameter value is derived from field value as (field)*4 UI.
             
     */
    UINT32 tl0pnull2tran : 8;
    /* tl0pnull2tran - Bits[15:8], RWS_L, default = 8'h8 
       
       	Period where null flits are sent (A2 to A3).
       	This parameter value is derived from field value as (field)*4 UI.
             
     */
    UINT32 tl0ptran2nullstop : 8;
    /* tl0ptran2nullstop - Bits[23:16], RWS_L, default = 8'h16 
       
       	Period where null flits are sent at new width (A3 to A4).
       	This parameter value is derived from field value as (field)*4 UI.
             
     */
    UINT32 tl0pnullstop2done : 8;
    /* tl0pnullstop2done - Bits[31:24], RWS_L, default = 8'h40 
       
       	Period after which a new width change can be initiated (A4 to A5).
       	This parameter value is derived from field value as (field)*4 UI.
             
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PM_R1_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */




/* QPIREUT_PH_CPR1_QPIAGENT_REUT_REG supported on:                            */
/*       IVT_EP (0x401431B8)                                                  */
/*       IVT_EX (0x401431B8)                                                  */
/*       HSX (0x401431B8)                                                     */
/*       BDX (0x401431B8)                                                     */
/* Register default value on IVT_EP:    0x01100013                            */
/* Register default value on IVT_EX:    0x01100013                            */
/* Register default value on HSX:       0x01100012                            */
/* Register default value on BDX:       0x01100012                            */
#define QPIREUT_PH_CPR1_QPIAGENT_REUT_REG 0x060241B8


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.3.CFG.xml.
 * Physical Layer Adaptation Capability Register
 */
typedef union {
  struct {
    UINT32 txdataadapt : 1;
    /* txdataadapt - Bits[0:0], RO, default = 1'b0 
       
       	If 1, agent can support Tx Data adaptation.
             
     */
    UINT32 rxdataadapt : 1;
    /* rxdataadapt - Bits[1:1], RO, default = 1'b1 
       
       	If 1, agent can support Rx Data adaptation
             
     */
    UINT32 txclkadapt : 1;
    /* txclkadapt - Bits[2:2], RO, default = 1'b0 
       
       	If 1, agent can support Tx Clk adaptation
             
     */
    UINT32 rxclkadapt : 1;
    /* rxclkadapt - Bits[3:3], RO, default = 1'b0 
       
       	If 1, agent can support Rx Clk adaptation
             
     */
    UINT32 txadaptsettings : 16;
    /* txadaptsettings - Bits[19:4], RO, default = 16'b1 
       
       	Number of Tx coefficients / indices.
             
     */
    UINT32 slowspeedbackchannel : 1;
    /* slowspeedbackchannel - Bits[20:20], RO, default = 1'b1 
       
        If 1, agent can also use Slow Mode as backchannel for adaptations that need a 
       backchannel. 
             
     */
    UINT32 pattern : 7;
    /* pattern - Bits[27:21], RO, default = 7'h8 
       
       	one-hot encoding
       	1 = PRBS7 without stagger
       	2 = PRBS7 with stagger
       	4 = PRBS23 without stagger
       	8 = PRBS23 with stagger
       	16 = clock pattern;
       	32 = custom pattern1 
       	64 = custom pattern2
             
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_CPR1_QPIAGENT_REUT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIREUT_PH_CTR1_QPIAGENT_REUT_REG supported on:                            */
/*       IVT_EP (0x401431BC)                                                  */
/*       IVT_EX (0x401431BC)                                                  */
/*       HSX (0x401431BC)                                                     */
/*       BDX (0x401431BC)                                                     */
/* Register default value on IVT_EP:    0x01100012                            */
/* Register default value on IVT_EX:    0x01100012                            */
/* Register default value on HSX:       0x01000010                            */
/* Register default value on BDX:       0x01000010                            */
#define QPIREUT_PH_CTR1_QPIAGENT_REUT_REG 0x060241BC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.3.CFG.xml.
 * Physical Layer Adaptation Control Register
 */
typedef union {
  struct {
    UINT32 txdataadapt : 1;
    /* txdataadapt - Bits[0:0], RWS_L, default = 1'b0 
       
       	If 1, agent can support Tx Data adaptation
             
     */
    UINT32 rxdataadapt : 1;
    /* rxdataadapt - Bits[1:1], RWS_L, default = 1'b0 
       
       	If 1, agent can support Rx Data adaptation
             
     */
    UINT32 txclkadapt : 1;
    /* txclkadapt - Bits[2:2], RO, default = 1'b0 
       
       	If 1, agent can support Tx Clk adaptation
             
     */
    UINT32 rxclkadapt : 1;
    /* rxclkadapt - Bits[3:3], RO, default = 1'b0 
       
       	If 1, agent can support Rx Clk adaptation
             
     */
    UINT32 txadaptsettings : 16;
    /* txadaptsettings - Bits[19:4], RO, default = 16'b1 
       
       	Number of Tx coefficients / indices.
             
     */
    UINT32 slowspeedbackchannel : 1;
    /* slowspeedbackchannel - Bits[20:20], RWS_L, default = 1'b0 
       
        If 1, agent can also use Slow Mode as backchannel for adaptations that need a 
       backchannel. 
             
     */
    UINT32 pattern : 7;
    /* pattern - Bits[27:21], RO, default = 7'h8 
       
       	one-hot encoding
       	1 = PRBS7 without stagger
       	2 = PRBS7 with stagger
       	4 = PRBS23 without stagger
       	8 = PRBS23 with stagger
       	16 = clock pattern;
       	32 = custom pattern1 
       	64 = custom pattern2 
             
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_CTR1_QPIAGENT_REUT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIREUT_PH_CTR2_QPIAGENT_REUT_REG supported on:                            */
/*       IVT_EP (0x401431C0)                                                  */
/*       IVT_EX (0x401431C0)                                                  */
/*       HSX (0x401431C0)                                                     */
/*       BDX (0x401431C0)                                                     */
/* Register default value:              0x00000000                            */
#define QPIREUT_PH_CTR2_QPIAGENT_REUT_REG 0x060241C0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * Physical Layer Adaptation LFSR Re-Init Control Register
 */
typedef union {
  struct {
    UINT32 reinitlen : 16;
    /* reinitlen - Bits[15:0], RWS_L, default = 16'b0 
       
       	0 : LFSR not re-initialized
       	Other : LFSR re-initialized after
       	(TReinit_Length * 128) UI
             
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PH_CTR2_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_PAT_CAP_QPIAGENT_REUT_REG supported on:                            */
/*       IVT_EP (0x401431CC)                                                  */
/*       IVT_EX (0x401431CC)                                                  */
/*       HSX (0x401431CC)                                                     */
/*       BDX (0x401431CC)                                                     */
/* Register default value on IVT_EP:    0x00CC0008                            */
/* Register default value on IVT_EX:    0x00CC0008                            */
/* Register default value on HSX:       0x00EC0000                            */
/* Register default value on BDX:       0x00EC0000                            */
#define QPIREUT_PAT_CAP_QPIAGENT_REUT_REG 0x060241CC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.3.CFG.xml.
 * Test Pattern Capability Register
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    UINT32 test_exe_delay_cap : 1;
    /* test_exe_delay_cap - Bits[1:1], RO, default = 1'b0 
       
        If set, this field indicates that the Test Execution Delay register is 
       available to set the delay between 
        setting Start Test and the actual start of the test. This bit MUST be a 1 when 
       the Start Coordination Capability is 1. 
       	0 - Test Execution Delay register (QPITTTED) is not implemented
       	1 - Test Execution Delay register is implemented
             
     */
    UINT32 test_exe_status : 1;
    /* test_exe_status - Bits[2:2], RO, default = 1'b0 
       
        If set, this field indicates that the Test Execution Status register is 
       implemented. 
       	0 - Test Execution Status register (QPITTTES) is not implemented
       	1 - Test Execution Status register is implemented
             
     */
    UINT32 selerrmasktype : 1;
    /* selerrmasktype - Bits[3:3], RO, default = 1'b0 
       
        1 - Symbol/Bit and Masking Modes Single bit checks are specified in terms of a 
       pattern buffer symbol number and the bit position within that symbol. For 
       interval testing, the mask logic provides a bit mask that is always aligned to 
       the receive interpolator for isolation. 
       	0 - Start_bit/Interval mode 
        Single bit checks are specified in terms of the pattern buffer bit to check. 
       Interval checking specifies the starting pattern buffer bit and the interval 
       between bit checks. 
             
     */
    UINT32 rsvd_4 : 1;
    /* rsvd_4 - Bits[4:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 slave_ib_para_extract : 1;
    /* slave_ib_para_extract - Bits[5:5], RO, default = 1'b0 
       
        If set, slave is capable of extracting electrical parameter from masters 
       TS.Loopback and applying during the test. 
             
     */
    UINT32 startcoordcap : 1;
    /* startcoordcap - Bits[6:6], RO, default = 1'b0 
       
        If set, this field indicates that the Start Test field has the ability to be 
       further qualified by an implementation specific control signal from a component 
       wide Global Event. 
             
     */
    UINT32 txerrorinjcap : 1;
    /* txerrorinjcap - Bits[7:7], RO, default = 1'b0 
       
        Only valid if Selective Error Mask Type is 1. If set, this field indicates that 
       the transmit side of the Loopback Master can inject bit errors. 
             
     */
    UINT32 rsvd_8 : 1;
    /* rsvd_8 - Bits[8:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 flatpatbufcap : 1;
    /* flatpatbufcap - Bits[9:9], RO, default = 1'b0 
       
        If set, this field indicates that the Pattern Buffer Access registers are not 
       indexed, but are flattened. 
             
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 loop_cnt_inhibit_cap : 1;
    /* loop_cnt_inhibit_cap - Bits[16:16], RO, default = 1'b0 
       
        If set, this field indicates that the Loop Counter Inhibit is implemented and 
       the user can stall the Loop Counter. 
       	0 - Loop Counter Inhibit is not implemented
       	1 - Loop Counter Inhibit is implemented
             
     */
    UINT32 rsvd_17 : 1;
    /* rsvd_17 - Bits[17:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 lfsrstaggercap : 1;
    /* lfsrstaggercap - Bits[18:18], RO, default = 1'b1 
       
        If set, this field indicates that the LFSR1 Stagger Enable and LFSR2 Stagger 
       Enable fields are implemented and the user can choose between staggereing of 
       each LFSR or not. 
       	1 - LRSR1 Stagger Enable and LFSR2 Stagger Enable fields exist
       	0 - LFSR1 Stagger Enable and LFSR2 Stagger Enable fields are missing
             
     */
    UINT32 dcpolarityselcap : 1;
    /* dcpolarityselcap - Bits[19:19], RO, default = 1'b1 
       
        If set, this field indicates that the Drive DC Zero or DC One field is 
       implemented and the user can choose between driving a 0 or a 1 on those lanes 
       set to drive a DC value in. 
       	1 - Drive DC Zero or DC One field exists
       	0 - Drive DC Zero or DC One field is missing
             
     */
    UINT32 rsvd_20 : 1;
    /* rsvd_20 - Bits[20:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 err_cnt_inhibit : 1;
    /* err_cnt_inhibit - Bits[21:21], RO, default = 1'b1 
       
       	If set, design is capable of inhibing error counters.
             
     */
    UINT32 polyselectcap : 1;
    /* polyselectcap - Bits[22:22], RO, default = 1'b1 
       
        If set, this field indicates that the LFSR Polynomial Select field is 
       implemented and the user can choose between the two LFSR polynomials. 
        1 - LFSR Polynomial can be selected between the ITU polynomial and the 
       historical Intel QPI polynomial (reciprocal of ITU polynomial). 
       	0 - Only the reciprocal polynomial is available.
             
     */
    UINT32 patbufselrotcap : 1;
    /* patbufselrotcap - Bits[23:23], RO, default = 1'b1 
       
        If set, user can enable rotation of the QPITTPG2S register selects during 
       testing. 
       	1 - Buffer Select can rotate
       	0 - Buffer Select is static
             
     */
    UINT32 rsvd_24 : 2;
    /* rsvd_24 - Bits[25:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ext_err_cnt_lane_reassign_cap : 1;
    /* ext_err_cnt_lane_reassign_cap - Bits[26:26], RO, default = 1'b0 
       
        This field indicates that the Extended Error Counter Lane Assign field is 
       implemented and the user can update the assigned lane for the Extended Error 
       Counters(s). 
       	0 - Feature not implemented
       	1 - Extended Error Counter can be reassigned
             
     */
    UINT32 extcntalllanescap : 1;
    /* extcntalllanescap - Bits[27:27], RO, default = 1'b0 
       
        If set, this field indicates that the Extended Error Count All Lanes field is 
       implemented and the user can enable counting of all bit errors for the complete 
       set of lanes for the tested lanes, starting from the lane selected in Extended 
       Error Counter Lane Select and including any consecutive higher lanes included in 
       the testing. 
       	1 - Extended Error Counter can accumulate all errors
       	0 - Feature not implemented
             
     */
    UINT32 numextcntcap : 4;
    /* numextcntcap - Bits[31:28], RO, default = 4'b0 
       
        This field is used to enumerate the total number of Extended Error Counters. 
       Each will have a QPITTPCC and QPITTLECR. If the quantity of Extended Error 
       Counters is other than 1 then that number is recorded in this field as a N-1 
       value. 
             
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PAT_CAP_QPIAGENT_REUT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */






/* QPIREUT_SLPS_QPIAGENT_REUT_REG supported on:                               */
/*       IVT_EP (0x401431D4)                                                  */
/*       IVT_EX (0x401431D4)                                                  */
/*       HSX (0x401431D4)                                                     */
/*       BDX (0x401431D4)                                                     */
/* Register default value:              0x00000000                            */
#define QPIREUT_SLPS_QPIAGENT_REUT_REG 0x060241D4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * REUT Slave Loopback Path Select
 */
typedef union {
  struct {
    UINT32 lpbkpath : 20;
    /* lpbkpath - Bits[19:0], RWS_L, default = 20'b0 
       
       	Lane selects for loopback path from Rx
       	Bit 0 - Controls Lane 0
       	Bit 1 - Controls Lane 1
       	..., and so on.
       	0 - Corresponding lane will use RX loopback path
       	1 - Corresponding lane will use path from pattern generator
             
     */
    UINT32 rsvd : 12;
    /* rsvd - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_SLPS_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_PM_R2_QPIAGENT_REUT_REG supported on:                              */
/*       IVT_EP (0x401431D8)                                                  */
/*       IVT_EX (0x401431D8)                                                  */
/*       HSX (0x401431D8)                                                     */
/*       BDX (0x401431D8)                                                     */
/* Register default value on IVT_EP:    0x00400848                            */
/* Register default value on IVT_EX:    0x00400848                            */
/* Register default value on HSX:       0x00401658                            */
/* Register default value on BDX:       0x00401658                            */
#define QPIREUT_PM_R2_QPIAGENT_REUT_REG 0x060241D8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.3.CFG.xml.
 * Power Management Register for L0p RX
 */
typedef union {
  struct {
    UINT32 tl0penter2tran : 8;
    /* tl0penter2tran - Bits[7:0], RWS_L, default = 8'h58 
       
        Period when PM.LinkWidthConfig flit is processed and link width change 
       initiated(B1 to B2). 
       	This parameter value is derived from field value as (field)*4 UI.
             
     */
    UINT32 tl0ptran2unblock : 8;
    /* tl0ptran2unblock - Bits[15:8], RWS_L, default = 8'h16 
       
        Period when link width change is accomplished ; so no flits sent to link 
       layer(B2 to B3). 
       	This parameter value is derived from field value as (field)*4 UI.
             
     */
    UINT32 tl0punblock2done : 8;
    /* tl0punblock2done - Bits[23:16], RWS_L, default = 8'h40 
       
       	Period after which new link width change can be initiated ; (B3 to B4).
       	This parameter value is derived from field value as (field)*4 UI.
             
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PM_R2_QPIAGENT_REUT_STRUCT;
#endif /* ASM_INC */


/* QPIREUT_PM_R3_QPIAGENT_REUT_REG supported on:                              */
/*       IVT_EP (0x401431DC)                                                  */
/*       IVT_EX (0x401431DC)                                                  */
/*       HSX (0x401431DC)                                                     */
/*       BDX (0x401431DC)                                                     */
/* Register default value on IVT_EP:    0x00000054                            */
/* Register default value on IVT_EX:    0x00000054                            */
/* Register default value on HSX:       0x00000015                            */
/* Register default value on BDX:       0x00000015                            */
#define QPIREUT_PM_R3_QPIAGENT_REUT_REG 0x060241DC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.3.CFG.xml.
 * Power Management Register for L0p Retraining
 */
typedef union {
  struct {
    UINT32 txl0pexitrtrnalarmsel : 4;
    /* txl0pexitrtrnalarmsel - Bits[3:0], RWS_L, default = 4'h5 
       
        Exp window value that indices how much time before retraining interval the phy 
       forces a Tx L0p exit to L0. 
       	Field value of 0 means this timer is not supported.
        User should program this timer to allow sufficient time for the remote RX to 
       detect the L0p exit flit + B1 to B3 time defined in the QPI spec.  
        Also, if remote port has L0p RX exit alarm feature, it needs to include this 
       alarm timeout in the equation. 
       	Timeout Value = 2^value * TSL UI
             
     */
    UINT32 rxl0pexitrtrnalarmsel : 4;
    /* rxl0pexitrtrnalarmsel - Bits[7:4], RWS_L, default = 4'h1 
       
        Exp window value that indices how much time before retraining interval the phy 
       forces an inband reset to remote port when the rx is in L0p power state. 
       	Field value of 0 means this timer is not supported.
       	Timeout Value = 2^value * TSL UI
             
     */
    UINT32 rsvd : 24;
    /* rsvd - Bits[31:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIREUT_PM_R3_QPIAGENT_REUT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* SVID_QPIAGENT_REUT_REG supported on:                                       */
/*       IVT_EP (0x2014302C)                                                  */
/*       IVT_EX (0x2014302C)                                                  */
/* Register default value:              0x8086                                */
#define SVID_QPIAGENT_REUT_REG 0x0602202C



/* SDID_QPIAGENT_REUT_REG supported on:                                       */
/*       IVT_EP (0x2014302E)                                                  */
/*       IVT_EX (0x2014302E)                                                  */
/* Register default value:              0x0000                                */
#define SDID_QPIAGENT_REUT_REG 0x0602202E



/* PXPCAP_QPIAGENT_REUT_REG supported on:                                     */
/*       IVT_EP (0x40143040)                                                  */
/*       IVT_EX (0x40143040)                                                  */
/* Register default value:              0x00910010                            */
#define PXPCAP_QPIAGENT_REUT_REG 0x06024040



/* QPIREUT_ERR_CED_QPIAGENT_REUT_REG supported on:                            */
/*       IVT_EP (0x40143120)                                                  */
/*       IVT_EX (0x40143120)                                                  */
/* Register default value:              0x00000000                            */
#define QPIREUT_ERR_CED_QPIAGENT_REUT_REG 0x06024120



/* QPIREUT_ERR_ECCRCV0_QPIAGENT_REUT_REG supported on:                        */
/*       IVT_EP (0x40143124)                                                  */
/*       IVT_EX (0x40143124)                                                  */
/* Register default value:              0x00000000                            */
#define QPIREUT_ERR_ECCRCV0_QPIAGENT_REUT_REG 0x06024124



/* QPIREUT_PM_R2_GENA_QPIAGENT_REUT_REG supported on:                         */
/*       IVT_EP (0x40143198)                                                  */
/*       IVT_EX (0x40143198)                                                  */
/* Register default value:              0x00000000                            */
#define QPIREUT_PM_R2_GENA_QPIAGENT_REUT_REG 0x06024198



/* QPIREUT_EP_OVR_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x4014319C)                                                  */
/*       IVT_EX (0x4014319C)                                                  */
/* Register default value:              0x00060000                            */
#define QPIREUT_EP_OVR_QPIAGENT_REUT_REG 0x0602419C



/* QPIREUT_EP_SR_QPIAGENT_REUT_REG supported on:                              */
/*       IVT_EP (0x401431A0)                                                  */
/*       IVT_EX (0x401431A0)                                                  */
/* Register default value:              0x000000FF                            */
#define QPIREUT_EP_SR_QPIAGENT_REUT_REG 0x060241A0



/* QPIREUT_EP_SS_QPIAGENT_REUT_REG supported on:                              */
/*       IVT_EP (0x401431A4)                                                  */
/*       IVT_EX (0x401431A4)                                                  */
/* Register default value:              0x00000000                            */
#define QPIREUT_EP_SS_QPIAGENT_REUT_REG 0x060241A4



/* QPIREUT_EP_CAP_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x401431A8)                                                  */
/*       IVT_EX (0x401431A8)                                                  */
/* Register default value:              0x00000000                            */
#define QPIREUT_EP_CAP_QPIAGENT_REUT_REG 0x060241A8



/* QPIREUT_EP_OVD_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x401431AC)                                                  */
/*       IVT_EX (0x401431AC)                                                  */
/* Register default value:              0x00000000                            */
#define QPIREUT_EP_OVD_QPIAGENT_REUT_REG 0x060241AC



/* QPIREUT_EP_OFT_QPIAGENT_REUT_REG supported on:                             */
/*       IVT_EP (0x401431B0)                                                  */
/*       IVT_EX (0x401431B0)                                                  */
/* Register default value:              0x00000000                            */
#define QPIREUT_EP_OFT_QPIAGENT_REUT_REG 0x060241B0



/* QPIREUT_EP_MCTR_QPIAGENT_REUT_REG supported on:                            */
/*       IVT_EP (0x401431B4)                                                  */
/*       IVT_EX (0x401431B4)                                                  */
/* Register default value:              0x00000000                            */
#define QPIREUT_EP_MCTR_QPIAGENT_REUT_REG 0x060241B4



/* QPIREUT_PH_CPRADP_QPIAGENT_REUT_REG supported on:                          */
/*       IVT_EP (0x401431C4)                                                  */
/*       IVT_EX (0x401431C4)                                                  */
/* Register default value:              0x0000000C                            */
#define QPIREUT_PH_CPRADP_QPIAGENT_REUT_REG 0x060241C4



/* QPIREUT_PH_CTRADP_QPIAGENT_REUT_REG supported on:                          */
/*       IVT_EP (0x401431C8)                                                  */
/*       IVT_EX (0x401431C8)                                                  */
/* Register default value:              0x00000004                            */
#define QPIREUT_PH_CTRADP_QPIAGENT_REUT_REG 0x060241C8



/* QPIREUT_PAT_TESTSTAT0_QPIAGENT_REUT_REG supported on:                      */
/*       IVT_EP (0x401431D0)                                                  */
/*       IVT_EX (0x401431D0)                                                  */
/* Register default value:              0x00000000                            */
#define QPIREUT_PAT_TESTSTAT0_QPIAGENT_REUT_REG 0x060241D0



/* QPIREUT_PM_R1_EXIT_QPIAGENT_REUT_REG supported on:                         */
/*       IVT_EP (0x401431E0)                                                  */
/*       IVT_EX (0x401431E0)                                                  */
/* Register default value:              0x40080840                            */
#define QPIREUT_PM_R1_EXIT_QPIAGENT_REUT_REG 0x060241E0



/* QPIREUT_PM_R2_EXIT_QPIAGENT_REUT_REG supported on:                         */
/*       IVT_EP (0x401431E4)                                                  */
/*       IVT_EX (0x401431E4)                                                  */
/* Register default value:              0x00400848                            */
#define QPIREUT_PM_R2_EXIT_QPIAGENT_REUT_REG 0x060241E4



#endif /* QPIAGENT_REUT_h */
