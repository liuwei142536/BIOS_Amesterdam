/* Date Stamp: 8/23/2014 */

#ifndef MC_MAINEXT_h
#define MC_MAINEXT_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* MC_MAINEXT_IVT_DEV 15                                                      */
/* MC_MAINEXT_IVT_FUN 1                                                       */
/* For HSX_HOST:                                                              */
/* MC_MAINEXT_HSX_DEV 19                                                      */
/* MC_MAINEXT_HSX_FUN 1                                                       */
/* For BDX_HOST:                                                              */
/* MC_MAINEXT_BDX_DEV 19                                                      */
/* MC_MAINEXT_BDX_FUN 1                                                       */

/* VID_MC_MAINEXT_REG supported on:                                           */
/*       IVT_EP (0x20179000)                                                  */
/*       IVT_EX (0x20179000)                                                  */
/*       HSX (0x20199000)                                                     */
/*       BDX (0x20199000)                                                     */
/* Register default value:              0x8086                                */
#define VID_MC_MAINEXT_REG 0x0B012000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
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
} VID_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* DID_MC_MAINEXT_REG supported on:                                           */
/*       IVT_EP (0x20179002)                                                  */
/*       IVT_EX (0x20179002)                                                  */
/*       HSX (0x20199002)                                                     */
/*       BDX (0x20199002)                                                     */
/* Register default value on IVT_EP:    0x0E71                                */
/* Register default value on IVT_EX:    0x0E71                                */
/* Register default value on HSX:       0x2F71                                */
/* Register default value on BDX:       0x6F71                                */
#define DID_MC_MAINEXT_REG 0x0B012002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6F71 
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
} DID_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* PCICMD_MC_MAINEXT_REG supported on:                                        */
/*       IVT_EP (0x20179004)                                                  */
/*       IVT_EX (0x20179004)                                                  */
/*       HSX (0x20199004)                                                     */
/*       BDX (0x20199004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_MC_MAINEXT_REG 0x0B012004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
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
} PCICMD_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* PCISTS_MC_MAINEXT_REG supported on:                                        */
/*       IVT_EP (0x20179006)                                                  */
/*       IVT_EX (0x20179006)                                                  */
/*       HSX (0x20199006)                                                     */
/*       BDX (0x20199006)                                                     */
/* Register default value:              0x0010                                */
#define PCISTS_MC_MAINEXT_REG 0x0B012006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
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
} PCISTS_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_MC_MAINEXT_REG supported on:                                           */
/*       IVT_EP (0x10179008)                                                  */
/*       IVT_EX (0x10179008)                                                  */
/*       HSX (0x10199008)                                                     */
/*       BDX (0x10199008)                                                     */
/* Register default value:              0x00                                  */
#define RID_MC_MAINEXT_REG 0x0B011008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
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
} RID_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_MC_MAINEXT_REG supported on:                                        */
/*       IVT_EP (0x10179009)                                                  */
/*       IVT_EX (0x10179009)                                                  */
/*       HSX (0x10199009)                                                     */
/*       BDX (0x10199009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_MC_MAINEXT_REG 0x0B011009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_MC_MAINEXT_REG supported on:                                        */
/*       IVT_EP (0x2017900A)                                                  */
/*       IVT_EX (0x2017900A)                                                  */
/*       HSX (0x2019900A)                                                     */
/*       BDX (0x2019900A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_MC_MAINEXT_REG 0x0B01200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
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
} CCR_N1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CLSR_MC_MAINEXT_REG supported on:                                          */
/*       IVT_EP (0x1017900C)                                                  */
/*       IVT_EX (0x1017900C)                                                  */
/*       HSX (0x1019900C)                                                     */
/*       BDX (0x1019900C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_MC_MAINEXT_REG 0x0B01100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
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
} CLSR_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* PLAT_MC_MAINEXT_REG supported on:                                          */
/*       IVT_EP (0x1017900D)                                                  */
/*       IVT_EX (0x1017900D)                                                  */
/*       HSX (0x1019900D)                                                     */
/*       BDX (0x1019900D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_MC_MAINEXT_REG 0x0B01100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
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
} PLAT_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* HDR_MC_MAINEXT_REG supported on:                                           */
/*       IVT_EP (0x1017900E)                                                  */
/*       IVT_EX (0x1017900E)                                                  */
/*       HSX (0x1019900E)                                                     */
/*       BDX (0x1019900E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_MC_MAINEXT_REG 0x0B01100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
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
} HDR_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* BIST_MC_MAINEXT_REG supported on:                                          */
/*       IVT_EP (0x1017900F)                                                  */
/*       IVT_EX (0x1017900F)                                                  */
/*       HSX (0x1019900F)                                                     */
/*       BDX (0x1019900F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_MC_MAINEXT_REG 0x0B01100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
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
} BIST_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* SVID_MC_MAINEXT_REG supported on:                                          */
/*       IVT_EP (0x2017902C)                                                  */
/*       IVT_EX (0x2017902C)                                                  */
/*       HSX (0x2019902C)                                                     */
/*       BDX (0x2019902C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_MC_MAINEXT_REG 0x0B01202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * generated by critter 19_1_0x02c
 */
typedef union {
  struct {
    UINT16 subsystem_vendor_identification_number : 16;
    /* subsystem_vendor_identification_number - Bits[15:0], RW_O, default = 16'b1000000010000110 
       The default value specifies Intel but can be set to any value once after reset.
     */
  } Bits;
  UINT16 Data;
} SVID_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* SDID_MC_MAINEXT_REG supported on:                                          */
/*       IVT_EP (0x2017902E)                                                  */
/*       IVT_EX (0x2017902E)                                                  */
/*       HSX (0x2019902E)                                                     */
/*       BDX (0x2019902E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_MC_MAINEXT_REG 0x0B01202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * generated by critter 19_1_0x02e
 */
typedef union {
  struct {
    UINT16 subsystem_device_identification_number : 16;
    /* subsystem_device_identification_number - Bits[15:0], RW_O, default = 16'b0000000000000000 
       Assigned by the subsystem vendor to uniquely identify the subsystem
     */
  } Bits;
  UINT16 Data;
} SDID_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_MC_MAINEXT_REG supported on:                                        */
/*       IVT_EP (0x10179034)                                                  */
/*       IVT_EX (0x10179034)                                                  */
/*       HSX (0x10199034)                                                     */
/*       BDX (0x10199034)                                                     */
/* Register default value on IVT_EP:    0x40                                  */
/* Register default value on IVT_EX:    0x40                                  */
/* Register default value on HSX:       0x00                                  */
/* Register default value on BDX:       0x40                                  */
#define CAPPTR_MC_MAINEXT_REG 0x0B011034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
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
} CAPPTR_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* INTL_MC_MAINEXT_REG supported on:                                          */
/*       IVT_EP (0x1017903C)                                                  */
/*       IVT_EX (0x1017903C)                                                  */
/*       HSX (0x1019903C)                                                     */
/*       BDX (0x1019903C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_MC_MAINEXT_REG 0x0B01103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
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
} INTL_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* INTPIN_MC_MAINEXT_REG supported on:                                        */
/*       IVT_EP (0x1017903D)                                                  */
/*       IVT_EX (0x1017903D)                                                  */
/*       HSX (0x1019903D)                                                     */
/*       BDX (0x1019903D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_MC_MAINEXT_REG 0x0B01103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
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
} INTPIN_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* MINGNT_MC_MAINEXT_REG supported on:                                        */
/*       IVT_EP (0x1017903E)                                                  */
/*       IVT_EX (0x1017903E)                                                  */
/*       HSX (0x1019903E)                                                     */
/*       BDX (0x1019903E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_MC_MAINEXT_REG 0x0B01103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
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
} MINGNT_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_MC_MAINEXT_REG supported on:                                        */
/*       IVT_EP (0x1017903F)                                                  */
/*       IVT_EX (0x1017903F)                                                  */
/*       HSX (0x1019903F)                                                     */
/*       BDX (0x1019903F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_MC_MAINEXT_REG 0x0B01103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
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
} MAXLAT_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* PXPCAP_MC_MAINEXT_REG supported on:                                        */
/*       IVT_EP (0x40179040)                                                  */
/*       IVT_EX (0x40179040)                                                  */
/*       HSX (0x40199040)                                                     */
/*       BDX (0x40199040)                                                     */
/* Register default value:              0x00910010                            */
#define PXPCAP_MC_MAINEXT_REG 0x0B014040
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * generated by critter 19_1_0x040
 */
typedef union {
  struct {
    UINT32 capability_id : 8;
    /* capability_id - Bits[7:0], RO, default = 8'b00010000 
       Provides the PCI Express capability ID assigned by PCI-SIG.
     */
    UINT32 next_ptr : 8;
    /* next_ptr - Bits[15:8], RO, default = 8'b00000000 
       Pointer to the next capability. Set to 0 to indicate there are no more 
       capability structures. 
     */
    UINT32 capability_version : 4;
    /* capability_version - Bits[19:16], RO, default = 4'b0001 
       PCI Express Capability is Compliant with Version 1.0 of the PCI Express Spec.
       
       Note:
       This capability structure is not compliant with Versions beyond 1.0, since they 
       require additional capability registers to be reserved. The only purpose for 
       this capability structure is to make enhanced configuration space available. 
       Minimizing the size of this structure is accomplished by reporting version 1.0 
       compliancy and reporting that this is an integrated root port device. As such, 
       only three Dwords of configuration space are required for this structure. 
     */
    UINT32 device_port_type : 4;
    /* device_port_type - Bits[23:20], RO, default = 4'b1001 
       Device type is Root Complex Integrated Endpoint
     */
    UINT32 slot_implemented : 1;
    /* slot_implemented - Bits[24:24], RO, default = 1'b0 
       N/A for integrated endpoints
     */
    UINT32 interrupt_message_number : 5;
    /* interrupt_message_number - Bits[29:25], RO, default = 5'b00000 
       N/A for this device
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PXPCAP_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* SPAREADDRESSLO_MC_MAINEXT_REG supported on:                                */
/*       IVT_EP (0x40179080)                                                  */
/*       IVT_EX (0x40179080)                                                  */
/*       HSX (0x40199080)                                                     */
/*       BDX (0x40199080)                                                     */
/* Register default value:              0x00000000                            */
#define SPAREADDRESSLO_MC_MAINEXT_REG 0x0B014080


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * Always points to the lower address for the next sparing operation. This register 
 * will not be affected by the HA access to the spare source rank during the HA 
 * window. 
 */
typedef union {
  struct {
    UINT32 rankadd : 32;
    /* rankadd - Bits[31:0], RW_LV, default = 32'b00000000000000000000000000000000 
       Always points to the lower address for the next sparing operation. This register 
       will not be affected by the HA access to the spare source rank during the HA 
       window. 
     */
  } Bits;
  UINT32 Data;
} SPAREADDRESSLO_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* SPARECTL_MC_MAINEXT_REG supported on:                                      */
/*       IVT_EP (0x40179090)                                                  */
/*       IVT_EX (0x40179090)                                                  */
/*       HSX (0x40199090)                                                     */
/*       BDX (0x40199090)                                                     */
/* Register default value:              0x00000000                            */
#define SPARECTL_MC_MAINEXT_REG 0x0B014090
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * generated by critter 19_1_0x090
 */
typedef union {
  struct {
    UINT32 spare_enable : 1;
    /* spare_enable - Bits[0:0], RW_LBV, default = 1'b0 
       Spare enable when set to 1. Hardware clears after the sparing completion. This 
       bit is mutex with CSR DDDCSPARECTL field SPARE_ENABLE. 
     */
    UINT32 rsvd_1 : 1;
    /* rsvd_1 - Bits[1:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 chn_sel : 2;
    /* chn_sel - Bits[3:2], RW_LB, default = 2'b00 
       Since there is only one spare-copy logic for all channels, this field selects 
       the channel or channel-pair for the spare-copy operation. 
       
       For independent channel operation:
       00 = channel 0 is selected for the spare-copy operation
       01 = channel 1 is selected for the spare-copy operation
       10 = channel 2 is selected for the spare-copy operation
       11 = channel 3 is selected for the spare-copy operation
       
       For lock-step channel operation:
       0x = channel 0 and channel 1 are selected for the spare-copy operation
       1x = channel 2 and channel 3 are selected for the spare-copy operation
     */
    UINT32 src_rank : 3;
    /* src_rank - Bits[6:4], RW_LB, default = 3'b000 
       Source logical rank that provides the data to be copied.
     */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 dst_rank : 3;
    /* dst_rank - Bits[10:8], RW_LB, default = 3'b000 
       Destination logical rank used for the memory copy.
     */
    UINT32 rsvd_11 : 5;
    /* rsvd_11 - Bits[15:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 scratch_pad : 8;
    /* scratch_pad - Bits[23:16], RW, default = 8'b00000000 
       This field is available as a scratch pad for SSR operations
     */
    UINT32 hafifowm : 5;
    /* hafifowm - Bits[28:24], RW_LB, default = 5'b00000 
       minimum water mark for HA writes to failed rank. Actual wm is max of WPQ credit 
       level and HaFifoWM. When wm is hit the HA is backpressured and a sparing window 
       is started. 
       
       If DisWPQWM is clear, the spare window is started when the number of hits to the 
       failed DIMM exceed max(# of credits in WPQ not yet returned to the HA, HaFifoWM) 
       
       If DisWPQWM is set, the spare window starts when the number of hits to the 
       failed DIMM exceed HaFifoWM. 
     */
    UINT32 diswpqwm : 1;
    /* diswpqwm - Bits[29:29], RW_LB, default = 1'b0 
       Disable WPQ level based water mark, so that sparing wm is only based on 
       HaFifoWM. 
       
       If DisWPQWM is clear, the spare window is started when the number of hits to the 
       failed DIMM exceed max(# of credits in WPQ not yet returned to the HA, HaFifoWM) 
       
       If DisWPQWM is set, the spare window starts when the number of hits to the 
       failed DIMM exceed HaFifoWM. 
       
       In either case, if the number of hits to the failed DIMM do not hit the WM, the 
       spare window will still start after SPAREINTERVAL.NORMOPDUR timer expiration. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SPARECTL_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* SSRSTATUS_MC_MAINEXT_REG supported on:                                     */
/*       IVT_EP (0x40179094)                                                  */
/*       IVT_EX (0x40179094)                                                  */
/*       HSX (0x40199094)                                                     */
/*       BDX (0x40199094)                                                     */
/* Register default value:              0x00000000                            */
#define SSRSTATUS_MC_MAINEXT_REG 0x0B014094
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * Provides the status of a spare-copy/memory Init operation.
 */
typedef union {
  struct {
    UINT32 sprinprogress : 1;
    /* sprinprogress - Bits[0:0], RO_V, default = 1'b0 
       Spare Operation in progress. This bit is set by hardware once operation has 
       started. It is cleared once operation is complete or fails. 
     */
    UINT32 sprcmplt : 1;
    /* sprcmplt - Bits[1:1], RO_V, default = 1'b0 
       Spare Operation Complete. Set by hardware once operation is complete. Bit is 
       cleared by hardware when a new operation is enabled. 
       
       Note: just before MC release the HA block prior to the completion of the sparing 
       operation, IMC logic will automatically update the corresponding RIR_RNK_TGT 
       target to reflect new DST_RANK. 
     */
    UINT32 patcmplt : 1;
    /* patcmplt - Bits[2:2], RW1C, default = 1'b0 
       All memory has been scrubbed. Hardware sets this bit each time the patrol engine 
       steps through all memory locations. If software wants to monitor 0 --> 1 
       transition after the bit has been set, the software will need to clear the bit 
       by writing a one to clear this bit in order to distinguish the next patrol scrub 
       completion. Clearing the bit will not affect the patrol scrub operation. 
     */
    UINT32 rsvd : 29;
    /* rsvd - Bits[31:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SSRSTATUS_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* SCRUBADDRESSLO_MC_MAINEXT_REG supported on:                                */
/*       IVT_EP (0x40179098)                                                  */
/*       IVT_EX (0x40179098)                                                  */
/*       HSX (0x40199098)                                                     */
/*       BDX (0x40199098)                                                     */
/* Register default value:              0x00000000                            */
#define SCRUBADDRESSLO_MC_MAINEXT_REG 0x0B014098


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * This register contains part of the address of the last patrol scrub request 
 * issued. When running memtest, the failing address is logged in this register on 
 * memtest errors. Software can write the next address to be scrubbed into this 
 * register. The STARTSCRUB bit will then trigger the specified address to be 
 * scrubbed. Patrol scrubs must be disabled to reliably write this register. 
 */
typedef union {
  struct {
    UINT32 rankadd : 32;
    /* rankadd - Bits[31:0], RW_LBV, default = 32'b00000000000000000000000000000000 
       Contains the rank address of the last scrub issued. Can be written to specify 
       the next scrub address with STARTSCRUB. RESTRICTIONS: Patrol Scrubs must be 
       disabled when writing to this field. Bit 0 of this register maps to bit 6 of 
       address. 
     */
  } Bits;
  UINT32 Data;
} SCRUBADDRESSLO_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* SCRUBADDRESSHI_MC_MAINEXT_REG supported on:                                */
/*       IVT_EP (0x4017909C)                                                  */
/*       IVT_EX (0x4017909C)                                                  */
/*       HSX (0x4019909C)                                                     */
/*       BDX (0x4019909C)                                                     */
/* Register default value:              0x00000000                            */
#define SCRUBADDRESSHI_MC_MAINEXT_REG 0x0B01409C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * This register pair contains part of the address of the last patrol scrub request 
 * issued. Software can write the next address into this register. Scrubbing must 
 * be disabled to reliably read and write this register. The STARTSCRUB bit will 
 * then trigger the specified address to be scrubbed. 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 4;
    /* rsvd_0 - Bits[3:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank : 4;
    /* rank - Bits[7:4], RW_LBV, default = 4'b0000 
       Contains the physical rank ID of the last scrub issued. Can be written to 
       specify the next scrub address with STARTSCRUB. RESTRICTION: Patrol Scrubs must 
       be disabled when writing to this field. 
     */
    UINT32 rsvd_8 : 2;
    /* rsvd_8 - Bits[9:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 chnl : 2;
    /* chnl - Bits[11:10], RW_LBV, default = 2'b00 
       Can be written to specify the next scrub address with STARTSCRUB. This register 
       is updated with channel address of the last scrub address issued. Restriction: 
       Patrol Scrubs must be disabled when writing to this field. 
     */
    UINT32 rsvd_12 : 20;
    /* rsvd_12 - Bits[31:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SCRUBADDRESSHI_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* SCRUBCTL_MC_MAINEXT_REG supported on:                                      */
/*       IVT_EP (0x401790A0)                                                  */
/*       IVT_EX (0x401790A0)                                                  */
/*       HSX (0x401990A0)                                                     */
/*       BDX (0x401990A0)                                                     */
/* Register default value:              0x00000000                            */
#define SCRUBCTL_MC_MAINEXT_REG 0x0B0140A0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * This register contains the Scrub control parameters and status.
 */
typedef union {
  struct {
    UINT32 scrubinterval : 24;
    /* scrubinterval - Bits[23:0], RW_LB, default = 24'b000000000000000000000000 
       Defines the interval in DCLKS between patrol scrub requests. The calculation for 
       this register to get a scrub to every line in 24 hours is: ((86400)/(memory 
       capacity/64))/cycle time of DCLK. RESTRICTIONS: Can only be changed when patrol 
       scrubs are disabled. 
     */
    UINT32 startscrub : 1;
    /* startscrub - Bits[24:24], RW_LBV, default = 1'b0 
       When Set, the Patrol scrub engine will start from the address in the scrub 
       address registers. Once the scrub is issued this bit is reset. 
     */
    UINT32 issueonce : 1;
    /* issueonce - Bits[25:25], RW_LB, default = 1'b0 
       When Set, the patrol scrub engine will issue the address in the scrub address 
       registers only once and stop. 
     */
    UINT32 scrubissued : 1;
    /* scrubissued - Bits[26:26], RW_LBV, default = 1'b0 
       When Set, the scrub address registers contain the last scrub address issued
     */
    UINT32 ptl_stopped : 1;
    /* ptl_stopped - Bits[27:27], RW_LBV, default = 1'b0 
       When stop_on_err is set, patrol will stop on error and set this bit.
       Patrol will resume at the next address when this bit or stop_on_err is cleared 
       by BIOS and patrol scrub is still enabled by scrub_en. 
     */
    UINT32 stop_on_err : 1;
    /* stop_on_err - Bits[28:28], RW_LB, default = 1'b0 
       Stop patrol scrub on poison or uncorrectable. On poison, patrol will log error 
       then stop. On uncorr, patrol will convert to poison (if enabled) then stop. 
       This mode is meant to be used as part of memory migration flow. SMI is signalled 
       by default. 
     */
    UINT32 ptl_cmpl : 1;
    /* ptl_cmpl - Bits[29:29], RW_LBV, default = 1'b0 
       When stop_on_cmpl is enabled, patrol will stop at the end of the address range 
       and set this bit. 
       Patrol will resume from beginning of address range when this bit or stop_on_cmpl 
       is cleared by BIOS and patrol scrub is still enabled by scrub_en. 
     */
    UINT32 stop_on_cmpl : 1;
    /* stop_on_cmpl - Bits[30:30], RW_LB, default = 1'b0 
       Stop patrol scrub at end of memory range. This mode is meant to be used as part 
       of memory migration flow. SMI is signalled by default. 
     */
    UINT32 scrub_en : 1;
    /* scrub_en - Bits[31:31], RW_L, default = 1'b0 
       Scrub Enable when set.
     */
  } Bits;
  UINT32 Data;
} SCRUBCTL_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */




/* SPAREINTERVAL_MC_MAINEXT_REG supported on:                                 */
/*       IVT_EP (0x401790A8)                                                  */
/*       IVT_EX (0x401790A8)                                                  */
/*       HSX (0x401990A8)                                                     */
/*       BDX (0x401990A8)                                                     */
/* Register default value:              0x03200C80                            */
#define SPAREINTERVAL_MC_MAINEXT_REG 0x0B0140A8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * Defines the interval between normal and sparing operations. Interval is defined 
 * in dclk. 
 */
typedef union {
  struct {
    UINT32 normopdur : 16;
    /* normopdur - Bits[15:0], RW_LB, default = 16'b0000110010000000 
       Normal operation duration. System requests will be serviced during this 
       interval. 
     */
    UINT32 numspare : 13;
    /* numspare - Bits[28:16], RW_LB, default = 13'b0001100100000 
       Sparing operation duration. System requests will be blocked during this interval 
       and only sparing copy operations will be serviced. 
     */
    UINT32 rsvd : 3;
    /* rsvd - Bits[31:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SPAREINTERVAL_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* RASENABLES_MC_MAINEXT_REG supported on:                                    */
/*       IVT_EP (0x401790AC)                                                  */
/*       IVT_EX (0x401790AC)                                                  */
/*       HSX (0x401990AC)                                                     */
/*       BDX (0x401990AC)                                                     */
/* Register default value:              0x00000000                            */
#define RASENABLES_MC_MAINEXT_REG 0x0B0140AC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * RAS Enables Register
 */
typedef union {
  struct {
    UINT32 mirroren : 1;
    /* mirroren - Bits[0:0], RW_LB, default = 1'b0 
       Mirror mode enable. The channel mapping must be set up before this bit will have 
       an effect on IMC operation. This changes the error policy. 
     */
    UINT32 rsvd : 31;
    /* rsvd - Bits[31:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RASENABLES_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* SMISPARECTL_MC_MAINEXT_REG supported on:                                   */
/*       IVT_EP (0x401790B4)                                                  */
/*       IVT_EX (0x401790B4)                                                  */
/*       HSX (0x401990B4)                                                     */
/*       BDX (0x401990B4)                                                     */
/* Register default value:              0x00000000                            */
#define SMISPARECTL_MC_MAINEXT_REG 0x0B0140B4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * System Management Interrupt and Spare control register.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 15;
    /* rsvd_0 - Bits[14:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 intrpt_sel_smi : 1;
    /* intrpt_sel_smi - Bits[15:15], RW_LB, default = 1'b0 
       SMI enable. Set to enable SMI signaling. Clear to disable SMI signaling.
     */
    UINT32 intrpt_sel_cmci : 1;
    /* intrpt_sel_cmci - Bits[16:16], RW_LB, default = 1'b0 
       (CMCI used as a proxy for NMI signaling). Set to enable NMI signaling. Clear to 
       disable NMI signaling. If both NMI and SMI enable bits are set then only SMI is 
       sent 
     */
    UINT32 intrpt_sel_pin : 1;
    /* intrpt_sel_pin - Bits[17:17], RW_LB, default = 1'b0 
       Enable pin signaling. When set the interrupt is signaled via the err[0] pin to 
       get the attention of a BMC. 
     */
    UINT32 rsvd_18 : 14;
    /* rsvd_18 - Bits[31:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SMISPARECTL_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* LEAKY_BUCKET_CFG_MC_MAINEXT_REG supported on:                              */
/*       IVT_EP (0x401790B8)                                                  */
/*       IVT_EX (0x401790B8)                                                  */
/*       HSX (0x401990B8)                                                     */
/*       BDX (0x401990B8)                                                     */
/* Register default value:              0x00000000                            */
#define LEAKY_BUCKET_CFG_MC_MAINEXT_REG 0x0B0140B8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * generated by critter 19_1_0x0b8
 */
typedef union {
  struct {
    UINT32 leaky_bkt_cfg_lo : 6;
    /* leaky_bkt_cfg_lo - Bits[5:0], RW_LB, default = 6'b000000 
       This is the lower order bit select mask of the two hot encoding threshold. The 
       value of this field specify the bit position of the mask: 
       00h: reserved
       01h: LEAKY_BUCKET_CNTR_LO bit 1, i.e. bit 12 of the full 53b counter
       ...
       1Fh: LEAKY_BUCKET_CNTR_LO bit 31, i.e. bit 42 of the full 53b counter
       20h: LEAKY_BUCKET_CNTR_HI bit 0, i.e. bit 43 of the full 53b counter
       ...
       29h: LEAKY_BUCKET_CNTR_HI bit 9, i.e. bit 52 of the full 53b counter
       2Ah - 3F: reserved
       
       When both counter bits selected by the LEAKY_BKT_CFG_HI and LEAKY_BKT_CFG_LO are 
       set, the 53b leaky bucket counter will be reset and the logic will generate a 
       LEAK pulse to decrement the correctable error counter by 1. 
       
       MRC BIOS must program this register to any non-zero value before switching to 
       NORMAL mode. 
     */
    UINT32 leaky_bkt_cfg_hi : 6;
    /* leaky_bkt_cfg_hi - Bits[11:6], RW_LB, default = 6'b000000 
       This is the higher order bit select mask of the two hot encoding threshold. The 
       value of this field specify the bit position of the mask: 
       00h: reserved
       01h: LEAKY_BUCKET_CNTR_LO bit 1, i.e. bit 12 of the full 53b counter
       ...
       1Fh: LEAKY_BUCKET_CNTR_LO bit 31, i.e. bit 42 of the full 53b counter
       20h: LEAKY_BUCKET_CNTR_HI bit 0, i.e. bit 43 of the full 53b counter
       ...
       29h: LEAKY_BUCKET_CNTR_HI bit 9, i.e. bit 52 of the full 53b counter
       2Ah - 3F: reserved
       
       When both counter bits selected by the LEAKY_BKT_CFG_HI and LEAKY_BKT_CFG_LO are 
       set, the 53b leaky bucket counter will be reset and the logic will generate a 
       LEAK pulse to decrement the correctable error counter by 1. 
       
       MRC BIOS must program this register to any non-zero value before switching to 
       NORMAL mode. 
     */
    UINT32 rsvd : 20;
    /* rsvd - Bits[31:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} LEAKY_BUCKET_CFG_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* LEAKY_BUCKET_CNTR_LO_MC_MAINEXT_REG supported on:                          */
/*       IVT_EP (0x401790C0)                                                  */
/*       IVT_EX (0x401790C0)                                                  */
/*       HSX (0x401990C0)                                                     */
/*       BDX (0x401990C0)                                                     */
/* Register default value:              0x00000000                            */
#define LEAKY_BUCKET_CNTR_LO_MC_MAINEXT_REG 0x0B0140C0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * generated by critter 19_1_0x0c0
 */
typedef union {
  struct {
    UINT32 leaky_bkt_cntr_lo : 32;
    /* leaky_bkt_cntr_lo - Bits[31:0], RW_V, default = 32'b00000000000000000000000000000000 
       This is the lower half of the leaky bucket counter. The full counter is actually 
       a 53b "DCLK" counter. There is a least significant 11b of the 53b counter is not 
       captured in CSR. The carry "strobe" from the not-shown least significant 11b 
       counter will trigger this 42b counter pair to count. The 42b counter-pair is 
       compared with the two-hot encoding threshold specified by the 
       LEAKY_BUCKET_CFG_HI and LEAKY_BUCKET_CFG_LO pair. When the counter bits 
       specified by the LEAKY_BUCKET_CFG_HI and LEAKY_BUCKET_CFG_LO are both set, the 
       53b counter is reset and the leaky bucket logic will generate a LEAK strobe last 
       for 1 DCLK. 
     */
  } Bits;
  UINT32 Data;
} LEAKY_BUCKET_CNTR_LO_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* LEAKY_BUCKET_CNTR_HI_MC_MAINEXT_REG supported on:                          */
/*       IVT_EP (0x401790C4)                                                  */
/*       IVT_EX (0x401790C4)                                                  */
/*       HSX (0x401990C4)                                                     */
/*       BDX (0x401990C4)                                                     */
/* Register default value:              0x00000000                            */
#define LEAKY_BUCKET_CNTR_HI_MC_MAINEXT_REG 0x0B0140C4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * generated by critter 19_1_0x0c4
 */
typedef union {
  struct {
    UINT32 leaky_bkt_cntr_hi : 10;
    /* leaky_bkt_cntr_hi - Bits[9:0], RW_V, default = 10'b0000000000 
       This is the upper half of the leaky bucket counter. The full counter is actually 
       a 53b "DCLK" counter. There is a least significant 11b of the 53b counter is not 
       captured in CSR. The carry "strobe" from the not-shown least significant 11b 
       counter will trigger this 42b counter pair to count. The 42b counter-pair is 
       compared with the two-hot encoding threshold specified by the 
       LEAKY_BUCKET_CFG_HI and LEAKY_BUCKET_CFG_LO pair. When the counter bits 
       specified by the LEAKY_BUCKET_CFG_HI and LEAKY_BUCKET_CFG_LO are both set, the 
       53b counter is reset and the leaky bucket logic will generate a LEAK strobe last 
       for 1 DCLK. 
     */
    UINT32 rsvd : 22;
    /* rsvd - Bits[31:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} LEAKY_BUCKET_CNTR_HI_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* MAXMTR_MC_MAINEXT_REG supported on:                                        */
/*       IVT_EP (0x401790D4)                                                  */
/*       IVT_EX (0x401790D4)                                                  */
/*       HSX (0x401990D4)                                                     */
/*       BDX (0x401990D4)                                                     */
/* Register default value:              0x00000000                            */
#define MAXMTR_MC_MAINEXT_REG 0x0B0140D4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * generated by critter 19_1_0x0d4
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 maxnumrow : 3;
    /* maxnumrow - Bits[8:6], RW, default = 3'b000 
       Max number of rows.
       000: 2^12 rows
       001: 2^13 rows
       010: 2^14 rows
       011: 2^15 rows
       100: 2^16 rows
       101: 2^17 rows
       110: 2^18 rows
       Others: reserved
     */
    UINT32 maxnumcol : 2;
    /* maxnumcol - Bits[10:9], RW, default = 2'b00 
       Max number of columns.
       00: 2^10 columns
       01: 2^11 columns
       10: 2^12 columns
       11: 2^13 columns (Reserved)
     */
    UINT32 rsvd_11 : 21;
    /* rsvd_11 - Bits[31:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MAXMTR_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* MTLFSR_MC_MAINEXT_REG supported on:                                        */
/*       IVT_EP (0x401790D8)                                                  */
/*       IVT_EX (0x401790D8)                                                  */
/*       HSX (0x401990D8)                                                     */
/*       BDX (0x401990D8)                                                     */
/* Register default value:              0x00000000                            */
#define MTLFSR_MC_MAINEXT_REG 0x0B0140D8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * generated by critter 19_1_0x0d8
 */
typedef union {
  struct {
    UINT32 fbvec : 27;
    /* fbvec - Bits[26:0], RW, default = 27'b000000000000000000000000000 
       LFSR XOR feedback tap points mask position.LFSR Example:
       MTLFSR.NUMBITS = 0x5 5 flop lsfr, with feedback taken lfsr[4] output
       MTLFSR.FBVEC = 0x9 Feedback tap points at X^3 and X^0
       Generating function: G(X) = X^5 + X^3 + 1
     */
    UINT32 numbits : 5;
    /* numbits - Bits[31:27], RW, default = 5'b00000 
       Number of bits in the LFSR. Max is 30 decimal.
     */
  } Bits;
  UINT32 Data;
} MTLFSR_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* MTLFSRSEED_MC_MAINEXT_REG supported on:                                    */
/*       IVT_EP (0x401790DC)                                                  */
/*       IVT_EX (0x401790DC)                                                  */
/*       HSX (0x401990DC)                                                     */
/*       BDX (0x401990DC)                                                     */
/* Register default value:              0x00000000                            */
#define MTLFSRSEED_MC_MAINEXT_REG 0x0B0140DC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * generated by critter 19_1_0x0dc
 */
typedef union {
  struct {
    UINT32 seed : 30;
    /* seed - Bits[29:0], RW, default = 30'b000000000000000000000000000000 
       Start value for LFSR address sequence.
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MTLFSRSEED_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* DDDCSPARECTL_MC_MAINEXT_REG supported on:                                  */
/*       IVT_EP (0x101790E4)                                                  */
/*       IVT_EX (0x101790E4)                                                  */
/*       HSX (0x101990E4)                                                     */
/*       BDX (0x101990E4)                                                     */
/* Register default value:              0x00                                  */
#define DDDCSPARECTL_MC_MAINEXT_REG 0x0B0110E4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * This register is an extension of SPARECTL register for DDDC sparing controls.
 */
typedef union {
  struct {
    UINT8 spare_enable : 1;
    /* spare_enable - Bits[0:0], RW_LBV, default = 1'b0 
       DDDC Spare enable when set to 1. Hardware clear after the sparing completion. 
       This register is also locked by the DISABLE_SPARING bit from IMC_FUSE_DOWNLOAD 
       register. 
       This bit shall be mutex with SPARECTL:SPARE_ENABLE.
     */
    UINT8 rsvd_1 : 1;
    UINT8 rsvd : 6;
    /* rsvd - Bits[7:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} DDDCSPARECTL_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* BDDDCSPARECTL_MC_MAINEXT_REG supported on:                                 */
/*       HSX (0x401990E8)                                                     */
/*       BDX (0x401990E8)                                                     */
/* Register default value:              0x0004FFFF                            */
#define BDDDCSPARECTL_MC_MAINEXT_REG 0x0B0140E8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * Bank DDDC Sparing Control. The reset values are programmed for legacy (rank) 
 * operation.  If bank-level and rank-level sparing are planned to be used 
 * together, care should be taken to set these values correctly for rank-level 
 * operation in the BIOS handler. 
 */
typedef union {
  struct {
    UINT32 bank_vec : 16;
    /* bank_vec - Bits[15:0], RW_LB, default = 16'b1111111111111111 
       One-hot Bank Vector of sparing banks for DDR4, banks [15:0]
       For DDR3, program bits [15:8] same as [7:0]
       Care should be taken to set this back to 0xffff if legacy sparing will be 
       initiated after a bank-level sparing event. 
     */
    UINT32 bank0 : 1;
    /* bank0 - Bits[16:16], RW_LB, default = 1'b0 
       bank sparing mode with bank id 0
     */
    UINT32 bank1 : 1;
    /* bank1 - Bits[17:17], RW_LB, default = 1'b0 
       bank sparing mode with bank id 1
     */
    UINT32 rank : 1;
    /* rank - Bits[18:18], RW_LB, default = 1'b1 
       Full-rank or full-rank upgrade mode. Set to 1 for legacy (rank) mode.
     */
    UINT32 rsvd_19 : 4;
    UINT32 rsvd : 9;
    /* rsvd - Bits[31:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} BDDDCSPARECTL_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* EMCALTCTL_MC_MAINEXT_REG supported on:                                     */
/*       IVT_EP (0x101790EC)                                                  */
/*       IVT_EX (0x101790EC)                                                  */
/*       HSX (0x101990EC)                                                     */
/*       BDX (0x101990EC)                                                     */
/* Register default value:              0x00                                  */
#define EMCALTCTL_MC_MAINEXT_REG 0x0B0110EC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * eMCA lite control register.
 */
typedef union {
  struct {
    UINT8 emcalt_sel_smi : 1;
    /* emcalt_sel_smi - Bits[0:0], RW_LB, default = 1'b0 
       SMI enable. Set to enable SMI signaling for each correctable error. Clear to 
       disable SMI signaling and signal CMCI when correctable error threshold is 
       reached. 
     */
    UINT8 rsvd : 7;
    /* rsvd - Bits[7:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} EMCALTCTL_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */




/* HOTPLUG_MC_MAINEXT_REG supported on:                                       */
/*       IVT_EP (0x101790F8)                                                  */
/*       IVT_EX (0x101790F8)                                                  */
/*       HSX (0x101990F8)                                                     */
/*       BDX (0x101990F8)                                                     */
/* Register default value:              0x00                                  */
#define HOTPLUG_MC_MAINEXT_REG 0x0B0110F8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * Memory Hot Plug Control and Status register. (EX processor only)
 */
typedef union {
  struct {
    UINT8 active : 1;
    /* active - Bits[0:0], RW_LB, default = 1'b0 
       Status bit for hot plug. BIOS to write this bit to indicate to MC that it is 
       currently in the memory hot add/ hot remove flow. 
     */
    UINT8 wpqdrained : 1;
    /* wpqdrained - Bits[1:1], RO_V, default = 1'b0 
       This bit will be 1 when all WDB entries are drained on all channels, 0 when any 
       WDB has valid entries. 
     */
    UINT8 rsvd : 6;
    /* rsvd - Bits[7:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} HOTPLUG_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* SCRUBMASK_MC_MAINEXT_REG supported on:                                     */
/*       IVT_EP (0x401790FC)                                                  */
/*       IVT_EX (0x401790FC)                                                  */
/*       HSX (0x401990FC)                                                     */
/*       BDX (0x401990FC)                                                     */
/* Register default value:              0x00000000                            */
#define SCRUBMASK_MC_MAINEXT_REG 0x0B0140FC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * This register contains fields to skip channels/ranks from patrol scrub engine. 
 * Patrol must be disabled before making any changes to this register. Locked by 
 * scrub_en bit in scrubctl register. 
 */
typedef union {
  struct {
    UINT32 ch_mask : 4;
    /* ch_mask - Bits[3:0], RW_L, default = 4'b0000 
       Patrol scrub channel mask, one bit per channel. Setting this bit will skip 
       patrol scrub on that channel according to the all_ranks and chx_rank fields. 
     */
    UINT32 all_ranks : 4;
    /* all_ranks - Bits[7:4], RW_L, default = 4'b0000 
       Setting this bit will skip all ranks on the channel when the channel mask is 
       set. Else it will skip the rank specified on chx_rank. 
     */
    UINT32 ch0_rank : 3;
    /* ch0_rank - Bits[10:8], RW_L, default = 3'b000 
       Logical Rank to skip for channel 0 when all_ranks is set to 0.
       lock bit is the scrub_en in scrubctl register.
     */
    UINT32 ch1_rank : 3;
    /* ch1_rank - Bits[13:11], RW_L, default = 3'b000 
       Logical Rank to skip for channel 1 when all_ranks is set to 0.
       lock bit is the scrub_en in scrubctl register.
     */
    UINT32 ch2_rank : 3;
    /* ch2_rank - Bits[16:14], RW_L, default = 3'b000 
       Logical Rank to skip for channel 2 when all_ranks is set to 0.
       lock bit is the scrub_en in scrubctl register.
     */
    UINT32 ch3_rank : 3;
    /* ch3_rank - Bits[19:17], RW_L, default = 3'b000 
       Logical Rank to skip for channel 3 when all_ranks is set to 0.
       lock bit is the scrub_en in scrubctl register.
     */
    UINT32 link_fail : 4;
    /* link_fail - Bits[23:20], RW_L, default = 4'b0000 
       Link fail ID.
       When a VMSE or native link fails, BIOS must program patrol engine to skip the 
       affected MC channels via LF, 
       CH_MASK, and ALL_RANKS field in this register. link_fail[3:0] is one bit per MC 
       channel. When in Intel SMI 2 mode 
       set bits 1:0 for VMSE channel 0 and set bits 3:2 for VMSE channel 1. When in 
       native DDR mode, set the bit 
       corresponding to the failing channel.
       LF:0x1 CH_MASK:0x3 ALL_RANKS:0x3
       LF:0x2 CH_MASK:0xC ALL_RANKS:0xC
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SCRUBMASK_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PXPENHCAP_MC_MAINEXT_REG supported on:                                     */
/*       IVT_EP (0x40179100)                                                  */
/*       IVT_EX (0x40179100)                                                  */
/*       HSX (0x40199100)                                                     */
/*       BDX (0x40199100)                                                     */
/* Register default value:              0x00000000                            */
#define PXPENHCAP_MC_MAINEXT_REG 0x0B014100
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * This field points to the next Capability in extended configuration space.
 */
typedef union {
  struct {
    UINT32 capability_id : 16;
    /* capability_id - Bits[15:0], RO, default = 16'b0000000000000000 
       Indicates there are no capability structures in the enhanced configuration 
       space. 
     */
    UINT32 capability_version : 4;
    /* capability_version - Bits[19:16], RO, default = 4'b0000 
       Indicates there are no capability structures in the enhanced configuration 
       space. 
     */
    UINT32 next_capability_offset : 12;
    /* next_capability_offset - Bits[31:20], RO, default = 12'b000000000000  */
  } Bits;
  UINT32 Data;
} PXPENHCAP_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */








































/* CPGC_GLOBALCTRL_MC_MAINEXT_REG supported on:                               */
/*       IVT_EP (0x40179200)                                                  */
/*       IVT_EX (0x40179200)                                                  */
/*       HSX (0x40199200)                                                     */
/*       BDX (0x40199200)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_GLOBALCTRL_MC_MAINEXT_REG 0x0B014200
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Global Control -
 * This register is used to control SEQ0/SEQ1 CPGC CH# Logic
 */
typedef union {
  struct {
    UINT32 start_test : 1;
    /* start_test - Bits[0:0], RW_LBV, default = 1'b0 
       Setting to 1 will immediately initiate a transition to Loopback.Pattern on all 
       channels that have their global_control bit set. 
       
       This field is cleared when all channels transition to Loopback.Marker from 
       loopback.Pattern for channels that have their global_control bit set. 
       If channels are not expected to transition to Loopback.Marker at the same time 
       (because of programming or other differences) 
       then start_test bit will not clear by itself.
       To start the test again, BIOS will need to clear this bit field followed by 
       setting it to 1 again. 
     */
    UINT32 stop_test : 1;
    /* stop_test - Bits[1:1], RW_LBV, default = 1'b0 
       Setting Global_Stop_Test to 1 will force a transition to Loopback.Marker on all 
       channels that have their Global_Control bit set. 
       Global_Stop_Test will immediately clear to 0 after forcing a transition to 
       Loopback.Marker on all channels that have their Global_Control bit set. 
     */
    UINT32 clear_errs : 1;
    /* clear_errs - Bits[2:2], RW_LBV, default = 1'b0 
       Setting Global_Clear_Errors to 1 will immediately clear all error status 
       registers (the same effect as asserting assert the Local_Clear_Errors) 
       on any channels that have their Global Control bit is set.
       Global_Clear_Errors will immediately be reset to 0 after all Channels_Err Status 
       Registers are cleared that have their Global Control bit Set 
     */
    UINT32 rsvd_3 : 1;
    /* rsvd_3 - Bits[3:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 stop_test_on_any_err : 1;
    /* stop_test_on_any_err - Bits[4:4], RW_LB, default = 1'b0 
       Setting Global_Stop_Test_On_Any_Error to 1 will immediately force a transition 
       to Loopback.Marker after any error on any channels 
       (indicated by a no zero value in Channel_Error_Status) that also have their 
       Global Control bit set. 
     */
    UINT32 use_bl4_col_addr : 1;
    /* use_bl4_col_addr - Bits[5:5], RW_LB, default = 1'b0 
       Currently Address bits [2:0] of the Column Addr are hard tied to zero, Setting 
       this bit will 
       enable bit control over Addres bit [2] as well thus enabling BL4 mode full 
       addressing. 
     */
    UINT32 cpgc_credits : 6;
    /* cpgc_credits - Bits[11:6], RW_LB, default = 6'b000000 
       Programmable credits for CPGC Multi Credit Scheme -
       6'd-zero: Use hardcoded defaults of 6'd16 (Intel SMI 2 2:1) and 6'd32 (Intel SMI 
       2 1:1) (EX processor only) 
       6'd-nonzero: Programmable credits for CPGC Multi-credit scheme
     */
    UINT32 cpgc_link_train : 1;
    /* cpgc_link_train - Bits[12:12], RW_LB, default = 1'b0 
       CPGC in Intel SMI 2 link training mode, where scalable memory controller uses 
       column bits 7:3 as buffer ID (EX processor only) 
     */
    UINT32 cpgc_crdt_wait : 1;
    /* cpgc_crdt_wait - Bits[13:13], RW_LB, default = 1'b0 
       When enabled, CPGC sequencer will wait for all credits to be returned before 
       moving to the next sub-sequence. 
       Usage of this de-feature bit is
       - limited to WAIT states between two sub-sequences.
       - limited to when CPGC is using multi-credit scheme.
     */
    UINT32 multiwr_singlerd_mode : 1;
    /* multiwr_singlerd_mode - Bits[14:14], RW_LB, default = 1'b0 
       Mode bit used for DIMM testing:
       1'b0: Multi credit mode when enabled via CPGC_SEQCTL[3:0].vmse_wrtrn_enb is 
       enabled for both Writes as well as Reads 
       1'b1: Multi credit mode when enabled via CPGC_SEQCTL[3:0].vmse_wrtrn_enb is 
       enabled for Writes only. 
       (Reads are issued using single credit mode).
       
       This mode is to be enabled only when b2b RD's are to be issued for DIMM testing.
       As blocking maintenance commands can block outstanding RD's, if this mode is not 
       enabled, RD's can be issued out of order. 
       
       "cpgc_crdt_wait" should also be enabled to make sure all commands in previous 
       sub-sequence have drained. 
     */
    UINT32 rsvd_15 : 17;
    /* rsvd_15 - Bits[31:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_GLOBALCTRL_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_GLOBALERR_MC_MAINEXT_REG supported on:                                */
/*       IVT_EP (0x40179204)                                                  */
/*       IVT_EX (0x40179204)                                                  */
/*       HSX (0x40199204)                                                     */
/*       BDX (0x40199204)                                                     */
/* Register default value:              0x000F0000                            */
#define CPGC_GLOBALERR_MC_MAINEXT_REG 0x0B014204
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Global Err -
 * This register is used to globally monitor all CPGC CH# Logic.
 */
typedef union {
  struct {
    UINT32 channel_err_status_0 : 1;
    /* channel_err_status_0 - Bits[0:0], RO_V, default = 1'b0 
       Channel_Error_Status_0 corresponds to Channel 0. When set this indicated Channel 
       0 has at least one Data (Data_Error_Status) or ECC 
       (Ecc_Error_Status) error status bit set.
       Channel_Error_Status_0 will be cleared when setting Local_Clear_Errors or 
       (Global_Control and Global_Clear_Errors) . 
     */
    UINT32 channel_err_status_1 : 1;
    /* channel_err_status_1 - Bits[1:1], RO_V, default = 1'b0 
       Channel_Error_Status_1 corresponds to Channel 1. When set this indicated Channel 
       1 has at least one Data (Data_Error_Status) or ECC 
       (Ecc_Error_Status) error status bit set.
       Channel_Error_Status_1 will be cleared when setting Local_Clear_Errors or 
       (Global_Control and Global_Clear_Errors) . 
     */
    UINT32 channel_err_status_2 : 1;
    /* channel_err_status_2 - Bits[2:2], RO_V, default = 1'b0 
       Channel_Error_Status_2 corresponds to Channel 2. When set this indicated Channel 
       2 has at least one Data (Data_Error_Status) or ECC 
       (Ecc_Error_Status) error status bit set.
       Channel_Error_Status_2 will be cleared when setting Local_Clear_Errors or 
       (Global_Control and Global_Clear_Errors) . 
     */
    UINT32 channel_err_status_3 : 1;
    /* channel_err_status_3 - Bits[3:3], RO_V, default = 1'b0 
       Channel_Error_Status_3 corresponds to Channel 3. When set this indicated Channel 
       3 has at least one Data (Data_Error_Status) or ECC 
       (Ecc_Error_Status) error status bit set.
       Channel_Error_Status_3 will be cleared when setting Local_Clear_Errors or 
       (Global_Control and Global_Clear_Errors) . 
     */
    UINT32 rsvd_4 : 12;
    /* rsvd_4 - Bits[15:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 channel_test_done_status_0 : 1;
    /* channel_test_done_status_0 - Bits[16:16], RO_V, default = 1'b1 
       Channel_Test_Done_Status_0 corresponds to Channel 0 and is set in 
       Loopback.Marker and cleared in Loopback.Pattern 
     */
    UINT32 channel_test_done_status_1 : 1;
    /* channel_test_done_status_1 - Bits[17:17], RO_V, default = 1'b1 
       Channel_Test_Done_Status_1 corresponds to Channel 1 and is set in 
       Loopback.Marker and cleared in Loopback.Pattern 
     */
    UINT32 channel_test_done_status_2 : 1;
    /* channel_test_done_status_2 - Bits[18:18], RO_V, default = 1'b1 
       Channel_Test_Done_Status_2 corresponds to Channel 2 and is set in 
       Loopback.Marker and cleared in Loopback.Pattern 
     */
    UINT32 channel_test_done_status_3 : 1;
    /* channel_test_done_status_3 - Bits[19:19], RO_V, default = 1'b1 
       Channel_Test_Done_Status_3 corresponds to Channel 3 and is set in 
       Loopback.Marker and cleared in Loopback.Pattern 
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_GLOBALERR_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQCTL0_MC_MAINEXT_REG supported on:                                  */
/*       IVT_EP (0x40179208)                                                  */
/*       IVT_EX (0x40179208)                                                  */
/*       HSX (0x40199208)                                                     */
/*       BDX (0x40199208)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQCTL0_MC_MAINEXT_REG 0x0B014208
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sequence Control -
 * SEQ_CTL0 defines a variety of controls available for configuring a Sequence for 
 * CH-0. 
 */
typedef union {
  struct {
    UINT32 local_start_test : 1;
    /* local_start_test - Bits[0:0], RW_LBV, default = 1'b0 
       Setting Local_Start_Test or (Global_Control and Global_Start_Test) to 1 will 
       immediately initiate a transition to Loopback.Pattern 
       from Loopback.Marker.
       Local_Start_Test must be cleared when a test transitions from Loopback.Pattern 
       to Loopback.Marker. 
     */
    UINT32 local_stop_test : 1;
    /* local_stop_test - Bits[1:1], RW_LBV, default = 1'b0 
       If Local_Stop_Test or (Global_Control and Global_Stop_Test) is set to 1 will 
       force a transition on this Channel # to Loopback.Marker 
       from Loopback.Pattern.
       Local_Start_Test must be cleared when a test transitions from Loopback.Pattern 
       to Loopback.Marker. 
     */
    UINT32 local_clear_errs : 1;
    /* local_clear_errs - Bits[2:2], RW_LBV, default = 1'b0 
       Setting Local_Clear_Errors or (Global_Control and Global_Clear_Errors) to 1 will 
       immediately clear all local channel errors status registers 
       (CPGC#_CH#_ERR_* registers).
       Local_Clear_Errors will immediately clear to 0 after clearing all error status 
       registers 
     */
    UINT32 stop_base_seq_on_wrap_trigger : 1;
    /* stop_base_seq_on_wrap_trigger - Bits[3:3], RW_LB, default = 1'b0 
       If Stop_Base_Seq_On_Wrap_Trigger is set then any Base Address Wrap Trigger that 
       occurs will immediately force a test to stop 
       (i.e. creates a transition from Loopback.Pattern to Loopback.Marker).
       Stop_On_Wrap_Trigger at the sequence level only makes sense when the 
       Address_Update_Rate_Mode is set to 0x1 where the four Address Update Rate 
       fields (Rank, Bank, Row, and Column) are updated by based on the Loopcount (the 
       # of loops through from the start of the test). 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
    UINT32 address_update_rate_mode : 1;
    /* address_update_rate_mode - Bits[4:4], RW_LB, default = 1'b0 
       Address_Update_Rate_Mode dictates whether the address increment is based on # of 
       cachelines written and read or by Loopcount (# of sequences performed). 
       
       Address_Update_Rate_Mode = 0 means that the four Address Update Rate fields 
       (Rank, Bank, Row, and Column) are updated based on the # of read and 
       write cacheline transactions.
       Address_Update_Rate_Mode = 1 means that the four Address Update Rate fields 
       (Rank, Bank, Row, and Column) are updated by based on the Loopcount 
       (# of sequences perfromed).
     */
    UINT32 enable_dummy_reads : 1;
    /* enable_dummy_reads - Bits[5:5], RW_LB, default = 1'b0 
       If set then the Dummy Read state machine is Enabled during this sequence 
       creating ACT and PRE in the same Rank that is being targeted. 
       See CPGC_CH#_SEQ#_DUMMYREAD_*
     */
    UINT32 enable_constant_write_strobe : 1;
    /* enable_constant_write_strobe - Bits[6:6], RW_LB, default = 1'b0 
       This bit is UNUSED in design. Refer to be4536804 for details.
       Setting Enable_Constant_Write_Strobe will cause the Tx write Strobe to be driven 
       out constantly while a test is in progress 
       (while in Loopback.Pattern).
       This is most often used to help train the correct setting for per Rank write 
       leveling using the following steps. 
     */
    UINT32 global_control : 1;
    /* global_control - Bits[7:7], RW_LB, default = 1'b0 
       Enables the CPGC_GLOBAL_CTRL register to control this CPGC#_CH# engine in 
       multiple ways 
       (i.e. Global_Start_Test, Global_Stop_Test, Global_Clear_Errors, 
       Global_Clear_Errors_On_Start_Test, and Global_Stop_Test_on_Any_Error). 
     */
    UINT32 initialization_mode : 2;
    /* initialization_mode - Bits[9:8], RW_LBV, default = 2'b00 
       A change to Initialization Mode occurs immediately and does not require a 
       Physical Layer Reset to be asserted. 
       00 - Idle on MC Reset
       01 - CPGC Testing Once the Initialization_Mode is set to 01 then the channel 
       enters immediately 
       into the Loopback.Marker state and the ability to start a test becomes enabled.
       10 - MRS Mode.
       11 - RSVD.
     */
    UINT32 loopcount : 5;
    /* loopcount - Bits[14:10], RW_LB, default = 5'b00000 
       2^(Loopcount-1) determines how many iterations of the complete Sequence loop 
       takes place before a test stops, where a 
       Sequence loop is defined by the completion of all SubSequences indicated by the 
       SubSequence Start pointer to 
       SubSequence End pointer.
       
       Loopcount = 0 means the test is infinite.
       Loopcount = 1 means the test will end after one loop through the sub sequences.
       Loopcount = 2 means the test will end after two Loops through the sub sequences.
       Loopcount = 3 means the test will end after four Loops through the sub 
       sequences. 
     */
    UINT32 subseq_start_pointer : 4;
    /* subseq_start_pointer - Bits[18:15], RW_LB, default = 4'b0000 
       Initial SubSequence pointer when beginning a Sequence. Subseq_Start_Pointer 
       values of 0 9 are valid 
     */
    UINT32 subseq_end_pointer : 4;
    /* subseq_end_pointer - Bits[22:19], RW_LB, default = 4'b0000 
       Subsequence pointer when completed the Loopcount is incremented by 1 and the 
       Current_Subseq_Pointer is reset to the Subseq_Start_Pointer. 
       Subseq_End_Pointer values of 0 9 are valid
       The Subseq_End_Pointer must always be set to equal or greater than the 
       Subseq_Start_Pointer. 
     */
    UINT32 start_test_delay : 8;
    /* start_test_delay - Bits[30:23], RW_LB, default = 8'b00000000 
       Delay period (in DClks) between the setting of Local_Start_Test or 
       (Global_Control and Global_Start_Test) and the actual entry 
       to Loopback.Pattern where the first SubSequence is executed immediately.
     */
    UINT32 vmse_wrtrn_enb : 1;
    /* vmse_wrtrn_enb - Bits[31:31], RW_LB, default = 1'b0 
       This bit enables CPGC mode wherein more then 1 CPGC WR credits are available at 
       a time. 
       1'b0: Default CPGC Wr credit of 1
       1'b1: 32 (Intel SMI 2 1:1) or 16 (Intel SMI 2 2:1) Wr-credits per scheduler to 
       enable Intel SMI 2 Write and Write leveling training steps 
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQCTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQCTL1_MC_MAINEXT_REG supported on:                                  */
/*       IVT_EP (0x4017920C)                                                  */
/*       IVT_EX (0x4017920C)                                                  */
/*       HSX (0x4019920C)                                                     */
/*       BDX (0x4019920C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQCTL1_MC_MAINEXT_REG 0x0B01420C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sequence Control -
 * SEQ_CTL1 defines a variety of controls available for configuring a Sequence for 
 * CH-1. 
 */
typedef union {
  struct {
    UINT32 local_start_test : 1;
    /* local_start_test - Bits[0:0], RW_LBV, default = 1'b0 
       Setting Local_Start_Test or (Global_Control and Global_Start_Test) to 1 will 
       immediately initiate a transition to Loopback.Pattern 
       from Loopback.Marker.
       Local_Start_Test must be cleared when a test transitions from Loopback.Pattern 
       to Loopback.Marker. 
     */
    UINT32 local_stop_test : 1;
    /* local_stop_test - Bits[1:1], RW_LBV, default = 1'b0 
       If Local_Stop_Test or (Global_Control and Global_Stop_Test) is set to 1 will 
       force a transition on this Channel # to Loopback.Marker 
       from Loopback.Pattern.
       Local_Start_Test must be cleared when a test transitions from Loopback.Pattern 
       to Loopback.Marker. 
     */
    UINT32 local_clear_errs : 1;
    /* local_clear_errs - Bits[2:2], RW_LBV, default = 1'b0 
       Setting Local_Clear_Errors or (Global_Control and Global_Clear_Errors) to 1 will 
       immediately clear all local channel errors status registers 
       (CPGC#_CH#_ERR_* registers).
       Local_Clear_Errors will immediately clear to 0 after clearing all error status 
       registers 
     */
    UINT32 stop_base_seq_on_wrap_trigger : 1;
    /* stop_base_seq_on_wrap_trigger - Bits[3:3], RW_LB, default = 1'b0 
       If Stop_Base_Seq_On_Wrap_Trigger is set then any Base Address Wrap Trigger that 
       occurs will immediately force a test to stop 
       (i.e. creates a transition from Loopback.Pattern to Loopback.Marker).
       Stop_On_Wrap_Trigger at the sequence level only makes sense when the 
       Address_Update_Rate_Mode is set to 0x1 where the four Address Update Rate 
       fields (Rank, Bank, Row, and Column) are updated by based on the Loopcount (the 
       # of loops through from the start of the test). 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
    UINT32 address_update_rate_mode : 1;
    /* address_update_rate_mode - Bits[4:4], RW_LB, default = 1'b0 
       Address_Update_Rate_Mode dictates whether the address increment is based on # of 
       cachelines written and read or by Loopcount (# of sequences performed). 
       
       Address_Update_Rate_Mode = 0 means that the four Address Update Rate fields 
       (Rank, Bank, Row, and Column) are updated based on the # of read and 
       write cacheline transactions.
       Address_Update_Rate_Mode = 1 means that the four Address Update Rate fields 
       (Rank, Bank, Row, and Column) are updated by based on the Loopcount 
       (# of sequences perfromed).
     */
    UINT32 enable_dummy_reads : 1;
    /* enable_dummy_reads - Bits[5:5], RW_LB, default = 1'b0 
       If set then the Dummy Read state machine is Enabled during this sequence 
       creating ACT and PRE in the same Rank that is being targeted. 
       See CPGC_CH#_SEQ#_DUMMYREAD_*
     */
    UINT32 rsvd_6 : 1;
    UINT32 global_control : 1;
    /* global_control - Bits[7:7], RW_LB, default = 1'b0 
       Enables the CPGC_GLOBAL_CTRL register to control this CPGC#_CH# engine in 
       multiple ways 
       (i.e. Global_Start_Test, Global_Stop_Test, Global_Clear_Errors, 
       Global_Clear_Errors_On_Start_Test, and Global_Stop_Test_on_Any_Error). 
     */
    UINT32 initialization_mode : 2;
    /* initialization_mode - Bits[9:8], RW_LBV, default = 2'b00 
       A change to Initialization Mode occurs immediately and does not require a 
       Physical Layer Reset to be asserted. 
       00 - Idle on MC Reset
       01 - CPGC Testing Once the Initialization_Mode is set to 01 then the channel 
       enters immediately 
       into the Loopback.Marker state and the ability to start a test becomes enabled.
       10 - MRS Mode.
       11 - Normal Operation. This is the normal protocol mode for the Link. (i.e. CPGC 
       is disabled). 
       If currently in one of the other modes, an exit will occur as soon as possible.
     */
    UINT32 loopcount : 5;
    /* loopcount - Bits[14:10], RW_LB, default = 5'b00000 
       2^(Loopcount-1) determines how many iterations of the complete Sequence loop 
       takes place before a test stops, where a 
       Sequence loop is defined by the completion of all SubSequences indicated by the 
       SubSequence Start pointer to 
       SubSequence End pointer.
       
       Loopcount = 0 means the test is infinite.
       Loopcount = 1 means the test will end after one loop through the sub sequences.
       Loopcount = 2 means the test will end after two Loops through the sub sequences.
       Loopcount = 3 means the test will end after four Loops through the sub 
       sequences. 
     */
    UINT32 subseq_start_pointer : 4;
    /* subseq_start_pointer - Bits[18:15], RW_LB, default = 4'b0000 
       Initial SubSequence pointer when beginning a Sequence. Subseq_Start_Pointer 
       values of 0 9 are valid 
     */
    UINT32 subseq_end_pointer : 4;
    /* subseq_end_pointer - Bits[22:19], RW_LB, default = 4'b0000 
       Subsequence pointer when completed the Loopcount is incremented by 1 and the 
       Current_Subseq_Pointer is reset to the Subseq_Start_Pointer. 
       Subseq_End_Pointer values of 0 9 are valid
       The Subseq_End_Pointer must always be set to equal or greater than the 
       Subseq_Start_Pointer. 
     */
    UINT32 start_test_delay : 8;
    /* start_test_delay - Bits[30:23], RW_LB, default = 8'b00000000 
       Delay period (in DClks) between the setting of Local_Start_Test or 
       (Global_Control and Global_Start_Test) and the actual entry 
       to Loopback.Pattern where the first SubSequence is executed immediately.
     */
    UINT32 vmse_wrtrn_enb : 1;
    /* vmse_wrtrn_enb - Bits[31:31], RW_LB, default = 1'b0 
       This bit enables CPGC mode wherein more then 1 CPGC WR credits are available at 
       a time. 
       1'b0: Default CPGC Wr credit of 1
       1'b1: 32 (Intel SMI 2 1:1) or 16 (Intel SMI 2 2:1) Wr-credits per scheduler to 
       enable Intel SMI 2 Write and Write leveling training steps 
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQCTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQCTL2_MC_MAINEXT_REG supported on:                                  */
/*       IVT_EP (0x40179210)                                                  */
/*       IVT_EX (0x40179210)                                                  */
/*       HSX (0x40199210)                                                     */
/*       BDX (0x40199210)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQCTL2_MC_MAINEXT_REG 0x0B014210
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sequence Control -
 * SEQ_CTL2 defines a variety of controls available for configuring a Sequence for 
 * CH-2. 
 */
typedef union {
  struct {
    UINT32 local_start_test : 1;
    /* local_start_test - Bits[0:0], RW_LBV, default = 1'b0 
       Setting Local_Start_Test or (Global_Control and Global_Start_Test) to 1 will 
       immediately initiate a transition to Loopback.Pattern 
       from Loopback.Marker.
       Local_Start_Test must be cleared when a test transitions from Loopback.Pattern 
       to Loopback.Marker. 
     */
    UINT32 local_stop_test : 1;
    /* local_stop_test - Bits[1:1], RW_LBV, default = 1'b0 
       If Local_Stop_Test or (Global_Control and Global_Stop_Test) is set to 1 will 
       force a transition on this Channel # to Loopback.Marker 
       from Loopback.Pattern.
       Local_Start_Test must be cleared when a test transitions from Loopback.Pattern 
       to Loopback.Marker. 
     */
    UINT32 local_clear_errs : 1;
    /* local_clear_errs - Bits[2:2], RW_LBV, default = 1'b0 
       Setting Local_Clear_Errors or (Global_Control and Global_Clear_Errors) to 1 will 
       immediately clear all local channel errors status registers 
       (CPGC#_CH#_ERR_* registers).
       Local_Clear_Errors will immediately clear to 0 after clearing all error status 
       registers 
     */
    UINT32 stop_base_seq_on_wrap_trigger : 1;
    /* stop_base_seq_on_wrap_trigger - Bits[3:3], RW_LB, default = 1'b0 
       If Stop_Base_Seq_On_Wrap_Trigger is set then any Base Address Wrap Trigger that 
       occurs will immediately force a test to stop 
       (i.e. creates a transition from Loopback.Pattern to Loopback.Marker).
       Stop_On_Wrap_Trigger at the sequence level only makes sense when the 
       Address_Update_Rate_Mode is set to 0x1 where the four Address Update Rate 
       fields (Rank, Bank, Row, and Column) are updated by based on the Loopcount (the 
       # of loops through from the start of the test). 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
    UINT32 address_update_rate_mode : 1;
    /* address_update_rate_mode - Bits[4:4], RW_LB, default = 1'b0 
       Address_Update_Rate_Mode dictates whether the address increment is based on # of 
       cachelines written and read or by Loopcount (# of sequences performed). 
       
       Address_Update_Rate_Mode = 0 means that the four Address Update Rate fields 
       (Rank, Bank, Row, and Column) are updated based on the # of read and 
       write cacheline transactions.
       Address_Update_Rate_Mode = 1 means that the four Address Update Rate fields 
       (Rank, Bank, Row, and Column) are updated by based on the Loopcount 
       (# of sequences perfromed).
     */
    UINT32 enable_dummy_reads : 1;
    /* enable_dummy_reads - Bits[5:5], RW_LB, default = 1'b0 
       If set then the Dummy Read state machine is Enabled during this sequence 
       creating ACT and PRE in the same Rank that is being targeted. 
       See CPGC_CH#_SEQ#_DUMMYREAD_*
     */
    UINT32 rsvd_6 : 1;
    UINT32 global_control : 1;
    /* global_control - Bits[7:7], RW_LB, default = 1'b0 
       Enables the CPGC_GLOBAL_CTRL register to control this CPGC#_CH# engine in 
       multiple ways 
       (i.e. Global_Start_Test, Global_Stop_Test, Global_Clear_Errors, 
       Global_Clear_Errors_On_Start_Test, and Global_Stop_Test_on_Any_Error). 
     */
    UINT32 initialization_mode : 2;
    /* initialization_mode - Bits[9:8], RW_LBV, default = 2'b00 
       A change to Initialization Mode occurs immediately and does not require a 
       Physical Layer Reset to be asserted. 
       00 - Idle on MC Reset
       01 - CPGC Testing Once the Initialization_Mode is set to 01 then the channel 
       enters immediately 
       into the Loopback.Marker state and the ability to start a test becomes enabled.
       10 - MRS Mode.
       11 - Normal Operation. This is the normal protocol mode for the Link. (i.e. CPGC 
       is disabled). 
       If currently in one of the other modes, an exit will occur as soon as possible.
     */
    UINT32 loopcount : 5;
    /* loopcount - Bits[14:10], RW_LB, default = 5'b00000 
       2^(Loopcount-1) determines how many iterations of the complete Sequence loop 
       takes place before a test stops, where a 
       Sequence loop is defined by the completion of all SubSequences indicated by the 
       SubSequence Start pointer to 
       SubSequence End pointer.
       
       Loopcount = 0 means the test is infinite.
       Loopcount = 1 means the test will end after one loop through the sub sequences.
       Loopcount = 2 means the test will end after two Loops through the sub sequences.
       Loopcount = 3 means the test will end after four Loops through the sub 
       sequences. 
     */
    UINT32 subseq_start_pointer : 4;
    /* subseq_start_pointer - Bits[18:15], RW_LB, default = 4'b0000 
       Initial SubSequence pointer when beginning a Sequence. Subseq_Start_Pointer 
       values of 0 9 are valid 
     */
    UINT32 subseq_end_pointer : 4;
    /* subseq_end_pointer - Bits[22:19], RW_LB, default = 4'b0000 
       Subsequence pointer when completed the Loopcount is incremented by 1 and the 
       Current_Subseq_Pointer is reset to the Subseq_Start_Pointer. 
       Subseq_End_Pointer values of 0 9 are valid
       The Subseq_End_Pointer must always be set to equal or greater than the 
       Subseq_Start_Pointer. 
     */
    UINT32 start_test_delay : 8;
    /* start_test_delay - Bits[30:23], RW_LB, default = 8'b00000000 
       Delay period (in DClks) between the setting of Local_Start_Test or 
       (Global_Control and Global_Start_Test) and the actual entry 
       to Loopback.Pattern where the first SubSequence is executed immediately.
     */
    UINT32 vmse_wrtrn_enb : 1;
    /* vmse_wrtrn_enb - Bits[31:31], RW_LB, default = 1'b0 
       This bit enables CPGC mode wherein more then 1 CPGC WR credits are available at 
       a time. 
       1'b0: Default CPGC Wr credit of 1
       1'b1: 32 (Intel SMI 2 1:1) or 16 (Intel SMI 2 2:1) Wr-credits per scheduler to 
       enable Intel SMI 2 Write and Write leveling training steps 
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQCTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQCTL3_MC_MAINEXT_REG supported on:                                  */
/*       IVT_EP (0x40179214)                                                  */
/*       IVT_EX (0x40179214)                                                  */
/*       HSX (0x40199214)                                                     */
/*       BDX (0x40199214)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQCTL3_MC_MAINEXT_REG 0x0B014214
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sequence Control -
 * SEQ_CTL3 defines a variety of controls available for configuring a Sequence for 
 * CH-3. 
 */
typedef union {
  struct {
    UINT32 local_start_test : 1;
    /* local_start_test - Bits[0:0], RW_LBV, default = 1'b0 
       Setting Local_Start_Test or (Global_Control and Global_Start_Test) to 1 will 
       immediately initiate a transition to Loopback.Pattern 
       from Loopback.Marker.
       Local_Start_Test must be cleared when a test transitions from Loopback.Pattern 
       to Loopback.Marker. 
     */
    UINT32 local_stop_test : 1;
    /* local_stop_test - Bits[1:1], RW_LBV, default = 1'b0 
       If Local_Stop_Test or (Global_Control and Global_Stop_Test) is set to 1 will 
       force a transition on this Channel # to Loopback.Marker 
       from Loopback.Pattern.
       Local_Start_Test must be cleared when a test transitions from Loopback.Pattern 
       to Loopback.Marker. 
     */
    UINT32 local_clear_errs : 1;
    /* local_clear_errs - Bits[2:2], RW_LBV, default = 1'b0 
       Setting Local_Clear_Errors or (Global_Control and Global_Clear_Errors) to 1 will 
       immediately clear all local channel errors status registers 
       (CPGC#_CH#_ERR_* registers).
       Local_Clear_Errors will immediately clear to 0 after clearing all error status 
       registers 
     */
    UINT32 stop_base_seq_on_wrap_trigger : 1;
    /* stop_base_seq_on_wrap_trigger - Bits[3:3], RW_LB, default = 1'b0 
       If Stop_Base_Seq_On_Wrap_Trigger is set then any Base Address Wrap Trigger that 
       occurs will immediately force a test to stop 
       (i.e. creates a transition from Loopback.Pattern to Loopback.Marker).
       Stop_On_Wrap_Trigger at the sequence level only makes sense when the 
       Address_Update_Rate_Mode is set to 0x1 where the four Address Update Rate 
       fields (Rank, Bank, Row, and Column) are updated by based on the Loopcount (the 
       # of loops through from the start of the test). 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
    UINT32 address_update_rate_mode : 1;
    /* address_update_rate_mode - Bits[4:4], RW_LB, default = 1'b0 
       Address_Update_Rate_Mode dictates whether the address increment is based on # of 
       cachelines written and read or by Loopcount (# of sequences performed). 
       
       Address_Update_Rate_Mode = 0 means that the four Address Update Rate fields 
       (Rank, Bank, Row, and Column) are updated based on the # of read and 
       write cacheline transactions.
       Address_Update_Rate_Mode = 1 means that the four Address Update Rate fields 
       (Rank, Bank, Row, and Column) are updated by based on the Loopcount 
       (# of sequences perfromed).
     */
    UINT32 enable_dummy_reads : 1;
    /* enable_dummy_reads - Bits[5:5], RW_LB, default = 1'b0 
       If set then the Dummy Read state machine is Enabled during this sequence 
       creating ACT and PRE in the same Rank that is being targeted. 
       See CPGC_CH#_SEQ#_DUMMYREAD_*
     */
    UINT32 rsvd_6 : 1;
    UINT32 global_control : 1;
    /* global_control - Bits[7:7], RW_LB, default = 1'b0 
       Enables the CPGC_GLOBAL_CTRL register to control this CPGC#_CH# engine in 
       multiple ways 
       (i.e. Global_Start_Test, Global_Stop_Test, Global_Clear_Errors, 
       Global_Clear_Errors_On_Start_Test, and Global_Stop_Test_on_Any_Error). 
     */
    UINT32 initialization_mode : 2;
    /* initialization_mode - Bits[9:8], RW_LBV, default = 2'b00 
       A change to Initialization Mode occurs immediately and does not require a 
       Physical Layer Reset to be asserted. 
       00 - Idle on MC Reset
       01 - CPGC Testing Once the Initialization_Mode is set to 01 then the channel 
       enters immediately 
       into the Loopback.Marker state and the ability to start a test becomes enabled.
       10 - MRS Mode.
       11 - Normal Operation. This is the normal protocol mode for the Link. (i.e. CPGC 
       is disabled). 
       If currently in one of the other modes, an exit will occur as soon as possible.
     */
    UINT32 loopcount : 5;
    /* loopcount - Bits[14:10], RW_LB, default = 5'b00000 
       2^(Loopcount-1) determines how many iterations of the complete Sequence loop 
       takes place before a test stops, where a 
       Sequence loop is defined by the completion of all SubSequences indicated by the 
       SubSequence Start pointer to 
       SubSequence End pointer.
       
       Loopcount = 0 means the test is infinite.
       Loopcount = 1 means the test will end after one loop through the sub sequences.
       Loopcount = 2 means the test will end after two Loops through the sub sequences.
       Loopcount = 3 means the test will end after four Loops through the sub 
       sequences. 
     */
    UINT32 subseq_start_pointer : 4;
    /* subseq_start_pointer - Bits[18:15], RW_LB, default = 4'b0000 
       Initial SubSequence pointer when beginning a Sequence. Subseq_Start_Pointer 
       values of 0 9 are valid 
     */
    UINT32 subseq_end_pointer : 4;
    /* subseq_end_pointer - Bits[22:19], RW_LB, default = 4'b0000 
       Subsequence pointer when completed the Loopcount is incremented by 1 and the 
       Current_Subseq_Pointer is reset to the Subseq_Start_Pointer. 
       Subseq_End_Pointer values of 0 9 are valid
       The Subseq_End_Pointer must always be set to equal or greater than the 
       Subseq_Start_Pointer. 
     */
    UINT32 start_test_delay : 8;
    /* start_test_delay - Bits[30:23], RW_LB, default = 8'b00000000 
       Delay period (in DClks) between the setting of Local_Start_Test or 
       (Global_Control and Global_Start_Test) and the actual entry 
       to Loopback.Pattern where the first SubSequence is executed immediately.
     */
    UINT32 vmse_wrtrn_enb : 1;
    /* vmse_wrtrn_enb - Bits[31:31], RW_LB, default = 1'b0 
       This bit enables CPGC mode wherein more then 1 CPGC WR credits are available at 
       a time. 
       1'b0: Default CPGC Wr credit of 1
       1'b1: 32 (Intel SMI 2 1:1) or 16 (Intel SMI 2 2:1) Wr-credits per scheduler to 
       enable Intel SMI 2 Write and Write leveling training steps 
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQCTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_LPCNT_STS0_MC_MAINEXT_REG supported on:                           */
/*       IVT_EP (0x40179218)                                                  */
/*       IVT_EX (0x40179218)                                                  */
/*       HSX (0x40199218)                                                     */
/*       BDX (0x40199218)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_LPCNT_STS0_MC_MAINEXT_REG 0x0B014218
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Loopcount Status -
 * CPGC#_CH#_SEQ_LOOPCOUNT_STATUS (Specific Channel Loopcount Status)
 */
typedef union {
  struct {
    UINT32 current_loopcount : 32;
    /* current_loopcount - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Current_Loopcount indicates how many times the full Sequence has been executed 
       completely. 
       Current_Loopcount is used to -
       1) Drive the Base Address Increment Logic (CPGC#_CH#_SEQ_BASE_ADDR_INC_CTL) when 
       CPGC#_CH#_SEQ_CTL.Address_Update_Rate_Mode is set to 1. 
       2) Help determine what the failing pattern is in conjunction with the control 
       signal to Stop_On_Nth_Error and the status provided by the 
       Physical Layer Address logged for the Nth Error (CPGC#_CH#_ERROR_ADDR).
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_LPCNT_STS0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_LPCNT_STS1_MC_MAINEXT_REG supported on:                           */
/*       IVT_EP (0x4017921C)                                                  */
/*       IVT_EX (0x4017921C)                                                  */
/*       HSX (0x4019921C)                                                     */
/*       BDX (0x4019921C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_LPCNT_STS1_MC_MAINEXT_REG 0x0B01421C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Loopcount Status -
 * CPGC#_CH#_SEQ_LOOPCOUNT_STATUS (Specific Channel Loopcount Status)
 */
typedef union {
  struct {
    UINT32 current_loopcount : 32;
    /* current_loopcount - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Current_Loopcount indicates how many times the full Sequence has been executed 
       completely. 
       Current_Loopcount is used to -
       1) Drive the Base Address Increment Logic (CPGC#_CH#_SEQ_BASE_ADDR_INC_CTL) when 
       CPGC#_CH#_SEQ_CTL.Address_Update_Rate_Mode is set to 1. 
       2) Help determine what the failing pattern is in conjunction with the control 
       signal to Stop_On_Nth_Error and the status provided by the 
       Physical Layer Address logged for the Nth Error (CPGC#_CH#_ERROR_ADDR).
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_LPCNT_STS1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_LPCNT_STS2_MC_MAINEXT_REG supported on:                           */
/*       IVT_EP (0x40179220)                                                  */
/*       IVT_EX (0x40179220)                                                  */
/*       HSX (0x40199220)                                                     */
/*       BDX (0x40199220)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_LPCNT_STS2_MC_MAINEXT_REG 0x0B014220
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Loopcount Status -
 * CPGC#_CH#_SEQ_LOOPCOUNT_STATUS (Specific Channel Loopcount Status)
 */
typedef union {
  struct {
    UINT32 current_loopcount : 32;
    /* current_loopcount - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Current_Loopcount indicates how many times the full Sequence has been executed 
       completely. 
       Current_Loopcount is used to -
       1) Drive the Base Address Increment Logic (CPGC#_CH#_SEQ_BASE_ADDR_INC_CTL) when 
       CPGC#_CH#_SEQ_CTL.Address_Update_Rate_Mode is set to 1. 
       2) Help determine what the failing pattern is in conjunction with the control 
       signal to Stop_On_Nth_Error and the status provided by the 
       Physical Layer Address logged for the Nth Error (CPGC#_CH#_ERROR_ADDR).
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_LPCNT_STS2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_LPCNT_STS3_MC_MAINEXT_REG supported on:                           */
/*       IVT_EP (0x40179224)                                                  */
/*       IVT_EX (0x40179224)                                                  */
/*       HSX (0x40199224)                                                     */
/*       BDX (0x40199224)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_LPCNT_STS3_MC_MAINEXT_REG 0x0B014224
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Loopcount Status -
 * CPGC#_CH#_SEQ_LOOPCOUNT_STATUS (Specific Channel Loopcount Status)
 */
typedef union {
  struct {
    UINT32 current_loopcount : 32;
    /* current_loopcount - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Current_Loopcount indicates how many times the full Sequence has been executed 
       completely. 
       Current_Loopcount is used to -
       1) Drive the Base Address Increment Logic (CPGC#_CH#_SEQ_BASE_ADDR_INC_CTL) when 
       CPGC#_CH#_SEQ_CTL.Address_Update_Rate_Mode is set to 1. 
       2) Help determine what the failing pattern is in conjunction with the control 
       signal to Stop_On_Nth_Error and the status provided by the 
       Physical Layer Address logged for the Nth Error (CPGC#_CH#_ERROR_ADDR).
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_LPCNT_STS3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_SUBSEQ_PNTR_MC_MAINEXT_REG supported on:                          */
/*       IVT_EP (0x40179228)                                                  */
/*       IVT_EX (0x40179228)                                                  */
/*       HSX (0x40199228)                                                     */
/*       BDX (0x40199228)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_SUBSEQ_PNTR_MC_MAINEXT_REG 0x0B014228
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Current Sub-Sequence Pointer -
 * CPGC#_CH#_SEQ_SUBSEQ_PNTR (Current_Subsequence_Pointer)
 */
typedef union {
  struct {
    UINT32 currentpntr0 : 4;
    /* currentpntr0 - Bits[3:0], RO_V, default = 4'b0000 
       Indicates what the current Subsequence value is in the Sequence for ch-0.
     */
    UINT32 rsvd_4 : 2;
    /* rsvd_4 - Bits[5:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 currentpntr1 : 4;
    /* currentpntr1 - Bits[9:6], RO_V, default = 4'b0000 
       Indicates what the current Subsequence value is in the Sequence for ch-1.
     */
    UINT32 rsvd_10 : 2;
    /* rsvd_10 - Bits[11:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 currentpntr2 : 4;
    /* currentpntr2 - Bits[15:12], RO_V, default = 4'b0000 
       Indicates what the current Subsequence value is in the Sequence for ch-2.
     */
    UINT32 rsvd_16 : 2;
    /* rsvd_16 - Bits[17:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 currentpntr3 : 4;
    /* currentpntr3 - Bits[21:18], RO_V, default = 4'b0000 
       Indicates what the current Subsequence value is in the Sequence for ch-3.
     */
    UINT32 rsvd_22 : 10;
    /* rsvd_22 - Bits[31:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_SUBSEQ_PNTR_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_CLSTS0_MC_MAINEXT_REG supported on:                               */
/*       IVT_EP (0x4017922C)                                                  */
/*       IVT_EX (0x4017922C)                                                  */
/*       HSX (0x4019922C)                                                     */
/*       BDX (0x4019922C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_CLSTS0_MC_MAINEXT_REG 0x0B01422C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Cache-Line Status -
 * CPGC#_CH#_SEQ_CACHELINE_STATUS indicates the current Cacheline value is in the 
 * Subsequence 
 */
typedef union {
  struct {
    UINT32 current_cl : 32;
    /* current_cl - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Current_Cacheline indicates the number of cachelines executed within the current 
       subsequence (CPGC#_CH#_SEQ_SUBSEQ_PNTR). 
       Current_Cacheline is used to -
       1) Drive the Base Address Increment Logic (CPGC#_CH#_SEQ_BASE_ADDR_INC_CTL) when 
       CPGC#_CH#_SEQ_CTL.Address_Update_Rate_Mode 
       is set to 0.
       2) Help determine what the failing pattern is in conjunction with the control 
       signal to Stop_On_Nth_Error and the status provided 
       by the Physical Layer Address logged for the Nth Error (CPGC#_CH#_ERROR_ADDR).
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_CLSTS0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_CLSTS1_MC_MAINEXT_REG supported on:                               */
/*       IVT_EP (0x40179230)                                                  */
/*       IVT_EX (0x40179230)                                                  */
/*       HSX (0x40199230)                                                     */
/*       BDX (0x40199230)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_CLSTS1_MC_MAINEXT_REG 0x0B014230
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Cache-Line Status -
 * CPGC#_CH#_SEQ_CACHELINE_STATUS indicates the current Cacheline value is in the 
 * Subsequence 
 */
typedef union {
  struct {
    UINT32 current_cl : 32;
    /* current_cl - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Current_Cacheline indicates the number of cachelines executed within the current 
       subsequence (CPGC#_CH#_SEQ_SUBSEQ_PNTR). 
       Current_Cacheline is used to -
       1) Drive the Base Address Increment Logic (CPGC#_CH#_SEQ_BASE_ADDR_INC_CTL) when 
       CPGC#_CH#_SEQ_CTL.Address_Update_Rate_Mode 
       is set to 0.
       2) Help determine what the failing pattern is in conjunction with the control 
       signal to Stop_On_Nth_Error and the status provided 
       by the Physical Layer Address logged for the Nth Error (CPGC#_CH#_ERROR_ADDR).
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_CLSTS1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_CLSTS2_MC_MAINEXT_REG supported on:                               */
/*       IVT_EP (0x40179234)                                                  */
/*       IVT_EX (0x40179234)                                                  */
/*       HSX (0x40199234)                                                     */
/*       BDX (0x40199234)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_CLSTS2_MC_MAINEXT_REG 0x0B014234
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Cache-Line Status -
 * CPGC#_CH#_SEQ_CACHELINE_STATUS indicates the current Cacheline value is in the 
 * Subsequence 
 */
typedef union {
  struct {
    UINT32 current_cl : 32;
    /* current_cl - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Current_Cacheline indicates the number of cachelines executed within the current 
       subsequence (CPGC#_CH#_SEQ_SUBSEQ_PNTR). 
       Current_Cacheline is used to -
       1) Drive the Base Address Increment Logic (CPGC#_CH#_SEQ_BASE_ADDR_INC_CTL) when 
       CPGC#_CH#_SEQ_CTL.Address_Update_Rate_Mode 
       is set to 0.
       2) Help determine what the failing pattern is in conjunction with the control 
       signal to Stop_On_Nth_Error and the status provided 
       by the Physical Layer Address logged for the Nth Error (CPGC#_CH#_ERROR_ADDR).
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_CLSTS2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_CLSTS3_MC_MAINEXT_REG supported on:                               */
/*       IVT_EP (0x40179238)                                                  */
/*       IVT_EX (0x40179238)                                                  */
/*       HSX (0x40199238)                                                     */
/*       BDX (0x40199238)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_CLSTS3_MC_MAINEXT_REG 0x0B014238
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Cache-Line Status -
 * CPGC#_CH#_SEQ_CACHELINE_STATUS indicates the current Cacheline value is in the 
 * Subsequence 
 */
typedef union {
  struct {
    UINT32 current_cl : 32;
    /* current_cl - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Current_Cacheline indicates the number of cachelines executed within the current 
       subsequence (CPGC#_CH#_SEQ_SUBSEQ_PNTR). 
       Current_Cacheline is used to -
       1) Drive the Base Address Increment Logic (CPGC#_CH#_SEQ_BASE_ADDR_INC_CTL) when 
       CPGC#_CH#_SEQ_CTL.Address_Update_Rate_Mode 
       is set to 0.
       2) Help determine what the failing pattern is in conjunction with the control 
       signal to Stop_On_Nth_Error and the status provided 
       by the Physical Layer Address logged for the Nth Error (CPGC#_CH#_ERROR_ADDR).
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_CLSTS3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_BASEADDRSTART_LO0_MC_MAINEXT_REG supported on:                    */
/*       HSX (0x4019923C)                                                     */
/*       BDX (0x4019923C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSTART_LO0_MC_MAINEXT_REG 0x0B01423C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence Starting Address -
 * CPGC#_CH#_SEQ_BASE_ADDR_START defines the starting base address of the sequence.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_addr : 10;
    /* column_addr - Bits[12:3], RW_LB, default = 10'b0000000000 
       Starting Column Address. Normally the lowest three column bits are reserved 
       since all transfers are done on a cacheline basis (8 chunks 0-7), 
       but for implementations that address to a sub cacheline then these reserved bits 
       might be used as needed. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_13 : 11;
    /* rsvd_13 - Bits[23:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_addr : 4;
    /* bank_addr - Bits[27:24], RW_LB, default = 4'b0000 
       Starting Bank Address
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRSTART_LO0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRSTART_LO1_MC_MAINEXT_REG supported on:                    */
/*       HSX (0x40199240)                                                     */
/*       BDX (0x40199240)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSTART_LO1_MC_MAINEXT_REG 0x0B014240

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence Starting Address -
 * CPGC#_CH#_SEQ_BASE_ADDR_START defines the starting base address of the sequence.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_addr : 10;
    /* column_addr - Bits[12:3], RW_LB, default = 10'b0000000000 
       Starting Column Address. Normally the lowest three column bits are reserved 
       since all transfers are done on a cacheline basis (8 chunks 0-7), 
       but for implementations that address to a sub cacheline then these reserved bits 
       might be used as needed. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_13 : 11;
    /* rsvd_13 - Bits[23:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_addr : 4;
    /* bank_addr - Bits[27:24], RW_LB, default = 4'b0000 
       Starting Bank Address
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRSTART_LO1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRSTART_LO2_MC_MAINEXT_REG supported on:                    */
/*       HSX (0x40199244)                                                     */
/*       BDX (0x40199244)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSTART_LO2_MC_MAINEXT_REG 0x0B014244

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence Starting Address -
 * CPGC#_CH#_SEQ_BASE_ADDR_START defines the starting base address of the sequence.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_addr : 10;
    /* column_addr - Bits[12:3], RW_LB, default = 10'b0000000000 
       Starting Column Address. Normally the lowest three column bits are reserved 
       since all transfers are done on a cacheline basis (8 chunks 0-7), 
       but for implementations that address to a sub cacheline then these reserved bits 
       might be used as needed. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_13 : 11;
    /* rsvd_13 - Bits[23:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_addr : 4;
    /* bank_addr - Bits[27:24], RW_LB, default = 4'b0000 
       Starting Bank Address
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRSTART_LO2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRSTART_LO3_MC_MAINEXT_REG supported on:                    */
/*       HSX (0x40199248)                                                     */
/*       BDX (0x40199248)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSTART_LO3_MC_MAINEXT_REG 0x0B014248

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence Starting Address -
 * CPGC#_CH#_SEQ_BASE_ADDR_START defines the starting base address of the sequence.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_addr : 10;
    /* column_addr - Bits[12:3], RW_LB, default = 10'b0000000000 
       Starting Column Address. Normally the lowest three column bits are reserved 
       since all transfers are done on a cacheline basis (8 chunks 0-7), 
       but for implementations that address to a sub cacheline then these reserved bits 
       might be used as needed. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_13 : 11;
    /* rsvd_13 - Bits[23:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_addr : 4;
    /* bank_addr - Bits[27:24], RW_LB, default = 4'b0000 
       Starting Bank Address
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRSTART_LO3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x4019924C)                                                     */
/*       BDX (0x4019924C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAINEXT_REG 0x0B01424C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence Wrap Address -
 * CPGC#_CH#_SEQ_BASE_ADDR_WRAP defines the wrap base address of the sequence.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_addr : 10;
    /* column_addr - Bits[12:3], RW_LB, default = 10'b0000000000 
       If the CPGC#_CH#_SEQ_BASE_ADDR_CURRENT.Column Address = 
       CPGC#_CH#_SEQ_BASE_ADDR_WRAP.Column_Address then any further request to modify 
       (increment or decrement) the CPGC#_CH#_SEQ_BASE_ADDR_CURRENT.Column Address 
       immediately causes a Wrap to occur with the following actions: 
       1. CPGC#_CH#_SEQ_BASE_ADDR_CURRENT.Column Address must be set equal to the 
       CPGC#_CH#_SEQ_BASE_ADDR_Start.Column_Address: 
       2. If Column_Base_Wrap_Carry_Enable is set bit to 1 a carry increment from the 
       Column Address (order N) will propagate to the N+1 order 
       Address field(s).
       3. If Column_Base_Wrap_Trigger_Enable is set to 1 then an Address Wrap Trigger 
       has occurred which can be used in conjunction with Stop_On_Wrap_Trigger. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_13 : 11;
    /* rsvd_13 - Bits[23:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_addr : 4;
    /* bank_addr - Bits[27:24], RW_LB, default = 4'b0000 
       Same as field [6:0] for Bank-Addr
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRWRAP_LO1_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x40199250)                                                     */
/*       BDX (0x40199250)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRWRAP_LO1_MC_MAINEXT_REG 0x0B014250

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence Wrap Address -
 * CPGC#_CH#_SEQ_BASE_ADDR_WRAP defines the wrap base address of the sequence.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_addr : 10;
    /* column_addr - Bits[12:3], RW_LB, default = 10'b0000000000 
       If the CPGC#_CH#_SEQ_BASE_ADDR_CURRENT.Column Address = 
       CPGC#_CH#_SEQ_BASE_ADDR_WRAP.Column_Address then any further request to modify 
       (increment or decrement) the CPGC#_CH#_SEQ_BASE_ADDR_CURRENT.Column Address 
       immediately causes a Wrap to occur with the following actions: 
       1. CPGC#_CH#_SEQ_BASE_ADDR_CURRENT.Column Address must be set equal to the 
       CPGC#_CH#_SEQ_BASE_ADDR_Start.Column_Address: 
       2. If Column_Base_Wrap_Carry_Enable is set bit to 1 a carry increment from the 
       Column Address (order N) will propagate to the N+1 order 
       Address field(s).
       3. If Column_Base_Wrap_Trigger_Enable is set to 1 then an Address Wrap Trigger 
       has occurred which can be used in conjunction with Stop_On_Wrap_Trigger. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_13 : 11;
    /* rsvd_13 - Bits[23:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_addr : 4;
    /* bank_addr - Bits[27:24], RW_LB, default = 4'b0000 
       Same as field [6:0] for Bank-Addr
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRWRAP_LO1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRWRAP_LO2_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x40199254)                                                     */
/*       BDX (0x40199254)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRWRAP_LO2_MC_MAINEXT_REG 0x0B014254

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence Wrap Address -
 * CPGC#_CH#_SEQ_BASE_ADDR_WRAP defines the wrap base address of the sequence.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_addr : 10;
    /* column_addr - Bits[12:3], RW_LB, default = 10'b0000000000 
       If the CPGC#_CH#_SEQ_BASE_ADDR_CURRENT.Column Address = 
       CPGC#_CH#_SEQ_BASE_ADDR_WRAP.Column_Address then any further request to modify 
       (increment or decrement) the CPGC#_CH#_SEQ_BASE_ADDR_CURRENT.Column Address 
       immediately causes a Wrap to occur with the following actions: 
       1. CPGC#_CH#_SEQ_BASE_ADDR_CURRENT.Column Address must be set equal to the 
       CPGC#_CH#_SEQ_BASE_ADDR_Start.Column_Address: 
       2. If Column_Base_Wrap_Carry_Enable is set bit to 1 a carry increment from the 
       Column Address (order N) will propagate to the N+1 order 
       Address field(s).
       3. If Column_Base_Wrap_Trigger_Enable is set to 1 then an Address Wrap Trigger 
       has occurred which can be used in conjunction with Stop_On_Wrap_Trigger. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_13 : 11;
    /* rsvd_13 - Bits[23:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_addr : 4;
    /* bank_addr - Bits[27:24], RW_LB, default = 4'b0000 
       Same as field [6:0] for Bank-Addr
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRWRAP_LO2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRWRAP_LO3_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x40199258)                                                     */
/*       BDX (0x40199258)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRWRAP_LO3_MC_MAINEXT_REG 0x0B014258

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence Wrap Address -
 * CPGC#_CH#_SEQ_BASE_ADDR_WRAP defines the wrap base address of the sequence.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_addr : 10;
    /* column_addr - Bits[12:3], RW_LB, default = 10'b0000000000 
       If the CPGC#_CH#_SEQ_BASE_ADDR_CURRENT.Column Address = 
       CPGC#_CH#_SEQ_BASE_ADDR_WRAP.Column_Address then any further request to modify 
       (increment or decrement) the CPGC#_CH#_SEQ_BASE_ADDR_CURRENT.Column Address 
       immediately causes a Wrap to occur with the following actions: 
       1. CPGC#_CH#_SEQ_BASE_ADDR_CURRENT.Column Address must be set equal to the 
       CPGC#_CH#_SEQ_BASE_ADDR_Start.Column_Address: 
       2. If Column_Base_Wrap_Carry_Enable is set bit to 1 a carry increment from the 
       Column Address (order N) will propagate to the N+1 order 
       Address field(s).
       3. If Column_Base_Wrap_Trigger_Enable is set to 1 then an Address Wrap Trigger 
       has occurred which can be used in conjunction with Stop_On_Wrap_Trigger. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_13 : 11;
    /* rsvd_13 - Bits[23:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_addr : 4;
    /* bank_addr - Bits[27:24], RW_LB, default = 4'b0000 
       Same as field [6:0] for Bank-Addr
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRWRAP_LO3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRCURR_LO0_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x4019925C)                                                     */
/*       BDX (0x4019925C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRCURR_LO0_MC_MAINEXT_REG 0x0B01425C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence Current Base Address -
 * CPGC#_CH#_SEQ_BASE_ADDR_CURRENT defines the current base address of the 
 * sequence. 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_addr : 10;
    /* column_addr - Bits[12:3], RO_V, default = 10'b0000000000 
       Current Column Address. Normally the lowest three column bits are reserved since 
       all transfers are done on a cacheline basis (8 chunks 0-7), 
       but for implementations that address to a sub cacheline then these reserved bits 
       might be used as needed. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_13 : 11;
    /* rsvd_13 - Bits[23:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_addr : 4;
    /* bank_addr - Bits[27:24], RO_V, default = 4'b0000 
       Current Bank Base Address
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRCURR_LO0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRCURR_LO1_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x40199260)                                                     */
/*       BDX (0x40199260)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRCURR_LO1_MC_MAINEXT_REG 0x0B014260

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence Current Base Address -
 * CPGC#_CH#_SEQ_BASE_ADDR_CURRENT defines the current base address of the 
 * sequence. 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_addr : 10;
    /* column_addr - Bits[12:3], RO_V, default = 10'b0000000000 
       Current Column Address. Normally the lowest three column bits are reserved since 
       all transfers are done on a cacheline basis (8 chunks 0-7), 
       but for implementations that address to a sub cacheline then these reserved bits 
       might be used as needed. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_13 : 11;
    /* rsvd_13 - Bits[23:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_addr : 4;
    /* bank_addr - Bits[27:24], RO_V, default = 4'b0000 
       Current Bank Base Address
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRCURR_LO1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRCURR_LO2_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x40199264)                                                     */
/*       BDX (0x40199264)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRCURR_LO2_MC_MAINEXT_REG 0x0B014264

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence Current Base Address -
 * CPGC#_CH#_SEQ_BASE_ADDR_CURRENT defines the current base address of the 
 * sequence. 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_addr : 10;
    /* column_addr - Bits[12:3], RO_V, default = 10'b0000000000 
       Current Column Address. Normally the lowest three column bits are reserved since 
       all transfers are done on a cacheline basis (8 chunks 0-7), 
       but for implementations that address to a sub cacheline then these reserved bits 
       might be used as needed. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_13 : 11;
    /* rsvd_13 - Bits[23:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_addr : 4;
    /* bank_addr - Bits[27:24], RO_V, default = 4'b0000 
       Current Bank Base Address
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRCURR_LO2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRCURR_LO3_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x40199268)                                                     */
/*       BDX (0x40199268)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRCURR_LO3_MC_MAINEXT_REG 0x0B014268

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence Current Base Address -
 * CPGC#_CH#_SEQ_BASE_ADDR_CURRENT defines the current base address of the 
 * sequence. 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_addr : 10;
    /* column_addr - Bits[12:3], RO_V, default = 10'b0000000000 
       Current Column Address. Normally the lowest three column bits are reserved since 
       all transfers are done on a cacheline basis (8 chunks 0-7), 
       but for implementations that address to a sub cacheline then these reserved bits 
       might be used as needed. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_13 : 11;
    /* rsvd_13 - Bits[23:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_addr : 4;
    /* bank_addr - Bits[27:24], RO_V, default = 4'b0000 
       Current Bank Base Address
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRCURR_LO3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BAOCIC0_MC_MAINEXT_REG supported on:                              */
/*       IVT_EP (0x4017926C)                                                  */
/*       IVT_EX (0x4017926C)                                                  */
/*       HSX (0x4019926C)                                                     */
/*       BDX (0x4019926C)                                                     */
/* Register default value:              0xF00000E4                            */
#define CPGC_SEQ_BAOCIC0_MC_MAINEXT_REG 0x0B01426C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Subsequence Address Ordering, Carry Masking, and Invert Control -
 * CPGC#_CH#_SEQ_BASE_ADDR_ORDER_CARRY_INVERT_CTL defines a series of enables that 
 * related to propagating carry bits for each address field, logic for inverting 
 * the current Base address, and how the various address fields will be ordered in 
 * the nested loop of LSB to MSB. 
 */
typedef union {
  struct {
    UINT32 column_addr_order : 2;
    /* column_addr_order - Bits[1:0], RW_LB, default = 2'b00 
       Determines the relative ordering of the Column address update logic in the 
       nested for loop in relation to the Rank, Bank, and Row 
       address update logic. Any address fields set to the same order that roll over 
       will cause a distinct carry of 1 to the next higher order 
       address field (multiple simultaneous carries will cause only one increment). All 
       fields can be arbitrarily placed in any order as long 
       as all address order fields are set in a continuous order starting from 0 and 
       may not skip over an order number. 
     */
    UINT32 row_addr_order : 2;
    /* row_addr_order - Bits[3:2], RW_LB, default = 2'b01 
       Same as Column_Addr_Order for Row
     */
    UINT32 bank_addr_order : 2;
    /* bank_addr_order - Bits[5:4], RW_LB, default = 2'b10 
       Same as Column_Addr_Order for Bank
     */
    UINT32 rank_addr_order : 2;
    /* rank_addr_order - Bits[7:6], RW_LB, default = 2'b11 
       Same as Column_Addr_Order for Rank
     */
    UINT32 rsvd_8 : 5;
    /* rsvd_8 - Bits[12:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 addr_inv_rate : 4;
    /* addr_inv_rate - Bits[16:13], RW_LB, default = 4'b0000 
       Address_Update_Rate_Mode dictates whether the Base address inversion logic is 
       based on # of cachelines written and read or by 
       Loopcount (# of sequences perfromed).
       Address_Update_Rate_Mode = 0 means that the CPGC#_CH#_SEQ_BASE_ADDR_CURRENT is 
       inverted based on 2^(Base_Address_Invert_Rate) read and 
       write cacheline transactions.
       Address_Update_Rate_Mode = 1 means that the CPGC#_CH#_SEQ_BASE_ADDR_CURRENT is 
       inverted based on 2^(Base_Address_Invert_Rate) Loops through 
       the Sequence.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_ba_inven : 1;
    /* column_ba_inven - Bits[20:20], RW_LB, default = 1'b0 
       Setting Column_Base_Address_Invert_Enable bit = 1 will cause the 
       CPGC#_CH#_SEQ_BASE_ADDR_CURRENT.Column Address to be inverted 
       every time 2^Base_Address_Invert_Rate cachelines are written or read or by 
       Loopcount (see Base_Address_Invert_Rate). 
       
       One must consider the address update rate when using this field.
       This feature is most often used for Memory array testing.
     */
    UINT32 row_ba_inven : 1;
    /* row_ba_inven - Bits[21:21], RW_LB, default = 1'b0 
       Same as Column_Ba_Inven for Row
     */
    UINT32 bank_ba_inven : 1;
    /* bank_ba_inven - Bits[22:22], RW_LB, default = 1'b0 
       Same as Column_Ba_Inven for Bank
     */
    UINT32 rank_ba_inven : 1;
    /* rank_ba_inven - Bits[23:23], RW_LB, default = 1'b0 
       Same as Column_Ba_Inven for Rank
     */
    UINT32 column_bw_trigen : 1;
    /* column_bw_trigen - Bits[24:24], RW_LB, default = 1'b0 
       If a Base Column Address Wrap occurs (see 
       CPGC#_CH#_SEQ_BASE_ADDR_WRAP.Column_Address) and Column_Base_Wrap_Trigger_Enable 
       is set to 1 then an Address Wrap Trigger has occurred which can be used in 
       conjunction with Stop_On_Wrap_Trigger to stop a 
       Subsequence or Sequence.
     */
    UINT32 row_bw_trigen : 1;
    /* row_bw_trigen - Bits[25:25], RW_LB, default = 1'b0 
       Same as Column_Bw_Trigen for Row
     */
    UINT32 bank_bw_trigen : 1;
    /* bank_bw_trigen - Bits[26:26], RW_LB, default = 1'b0 
       Same as Column_Bw_Trigen for Bank
     */
    UINT32 rank_bw_trigen : 1;
    /* rank_bw_trigen - Bits[27:27], RW_LB, default = 1'b0 
       Same as Column_Bw_Trigen for Rank
     */
    UINT32 column_bw_cryen : 1;
    /* column_bw_cryen - Bits[28:28], RW_LB, default = 1'b1 
       If a Base Column Address Wrap occurs (see 
       CPGC#_CH#_SEQ_BASE_ADDR_WRAP.Column_Address) and Column_Base_Wrap_Carry_Enable 
       is set to 1 then a carry increment from the Column Address (order N) will 
       propagate to the N+1 order Address field(s) . 
     */
    UINT32 row_bw_cryen : 1;
    /* row_bw_cryen - Bits[29:29], RW_LB, default = 1'b1 
       Same as Column_Bw_Cryen for Row
     */
    UINT32 bank_bw_cryen : 1;
    /* bank_bw_cryen - Bits[30:30], RW_LB, default = 1'b1 
       Same as Column_Bw_Cryen for Bank
     */
    UINT32 rank_bw_cryen : 1;
    /* rank_bw_cryen - Bits[31:31], RW_LB, default = 1'b1 
       Same as Column_Bw_Cryen for Rank
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BAOCIC0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_BAOCIC1_MC_MAINEXT_REG supported on:                              */
/*       IVT_EP (0x40179270)                                                  */
/*       IVT_EX (0x40179270)                                                  */
/*       HSX (0x40199270)                                                     */
/*       BDX (0x40199270)                                                     */
/* Register default value:              0xF00000E4                            */
#define CPGC_SEQ_BAOCIC1_MC_MAINEXT_REG 0x0B014270
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Subsequence Address Ordering, Carry Masking, and Invert Control -
 * CPGC#_CH#_SEQ_BASE_ADDR_ORDER_CARRY_INVERT_CTL defines a series of enables that 
 * related to propagating carry bits for each address field, logic for inverting 
 * the current Base address, and how the various address fields will be ordered in 
 * the nested loop of LSB to MSB. 
 */
typedef union {
  struct {
    UINT32 column_addr_order : 2;
    /* column_addr_order - Bits[1:0], RW_LB, default = 2'b00 
       Determines the relative ordering of the Column address update logic in the 
       nested for loop in relation to the Rank, Bank, and Row 
       address update logic. Any address fields set to the same order that roll over 
       will cause a distinct carry of 1 to the next higher order 
       address field (multiple simultaneous carries will cause only one increment). All 
       fields can be arbitrarily placed in any order as long 
       as all address order fields are set in a continuous order starting from 0 and 
       may not skip over an order number. 
     */
    UINT32 row_addr_order : 2;
    /* row_addr_order - Bits[3:2], RW_LB, default = 2'b01 
       Same as Column_Addr_Order for Row
     */
    UINT32 bank_addr_order : 2;
    /* bank_addr_order - Bits[5:4], RW_LB, default = 2'b10 
       Same as Column_Addr_Order for Bank
     */
    UINT32 rank_addr_order : 2;
    /* rank_addr_order - Bits[7:6], RW_LB, default = 2'b11 
       Same as Column_Addr_Order for Rank
     */
    UINT32 rsvd_8 : 5;
    /* rsvd_8 - Bits[12:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 addr_inv_rate : 4;
    /* addr_inv_rate - Bits[16:13], RW_LB, default = 4'b0000 
       Address_Update_Rate_Mode dictates whether the Base address inversion logic is 
       based on # of cachelines written and read or by 
       Loopcount (# of sequences perfromed).
       Address_Update_Rate_Mode = 0 means that the CPGC#_CH#_SEQ_BASE_ADDR_CURRENT is 
       inverted based on 2^(Base_Address_Invert_Rate) read and 
       write cacheline transactions.
       Address_Update_Rate_Mode = 1 means that the CPGC#_CH#_SEQ_BASE_ADDR_CURRENT is 
       inverted based on 2^(Base_Address_Invert_Rate) Loops through 
       the Sequence.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_ba_inven : 1;
    /* column_ba_inven - Bits[20:20], RW_LB, default = 1'b0 
       Setting Column_Base_Address_Invert_Enable bit = 1 will cause the 
       CPGC#_CH#_SEQ_BASE_ADDR_CURRENT.Column Address to be inverted 
       every time 2^Base_Address_Invert_Rate cachelines are written or read or by 
       Loopcount (see Base_Address_Invert_Rate). 
       
       One must consider the address update rate when using this field.
       This feature is most often used for Memory array testing.
     */
    UINT32 row_ba_inven : 1;
    /* row_ba_inven - Bits[21:21], RW_LB, default = 1'b0 
       Same as Column_Ba_Inven for Row
     */
    UINT32 bank_ba_inven : 1;
    /* bank_ba_inven - Bits[22:22], RW_LB, default = 1'b0 
       Same as Column_Ba_Inven for Bank
     */
    UINT32 rank_ba_inven : 1;
    /* rank_ba_inven - Bits[23:23], RW_LB, default = 1'b0 
       Same as Column_Ba_Inven for Rank
     */
    UINT32 column_bw_trigen : 1;
    /* column_bw_trigen - Bits[24:24], RW_LB, default = 1'b0 
       If a Base Column Address Wrap occurs (see 
       CPGC#_CH#_SEQ_BASE_ADDR_WRAP.Column_Address) and Column_Base_Wrap_Trigger_Enable 
       is set to 1 then an Address Wrap Trigger has occurred which can be used in 
       conjunction with Stop_On_Wrap_Trigger to stop a 
       Subsequence or Sequence.
     */
    UINT32 row_bw_trigen : 1;
    /* row_bw_trigen - Bits[25:25], RW_LB, default = 1'b0 
       Same as Column_Bw_Trigen for Row
     */
    UINT32 bank_bw_trigen : 1;
    /* bank_bw_trigen - Bits[26:26], RW_LB, default = 1'b0 
       Same as Column_Bw_Trigen for Bank
     */
    UINT32 rank_bw_trigen : 1;
    /* rank_bw_trigen - Bits[27:27], RW_LB, default = 1'b0 
       Same as Column_Bw_Trigen for Rank
     */
    UINT32 column_bw_cryen : 1;
    /* column_bw_cryen - Bits[28:28], RW_LB, default = 1'b1 
       If a Base Column Address Wrap occurs (see 
       CPGC#_CH#_SEQ_BASE_ADDR_WRAP.Column_Address) and Column_Base_Wrap_Carry_Enable 
       is set to 1 then a carry increment from the Column Address (order N) will 
       propagate to the N+1 order Address field(s) . 
     */
    UINT32 row_bw_cryen : 1;
    /* row_bw_cryen - Bits[29:29], RW_LB, default = 1'b1 
       Same as Column_Bw_Cryen for Row
     */
    UINT32 bank_bw_cryen : 1;
    /* bank_bw_cryen - Bits[30:30], RW_LB, default = 1'b1 
       Same as Column_Bw_Cryen for Bank
     */
    UINT32 rank_bw_cryen : 1;
    /* rank_bw_cryen - Bits[31:31], RW_LB, default = 1'b1 
       Same as Column_Bw_Cryen for Rank
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BAOCIC1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_BAOCIC2_MC_MAINEXT_REG supported on:                              */
/*       IVT_EP (0x40179274)                                                  */
/*       IVT_EX (0x40179274)                                                  */
/*       HSX (0x40199274)                                                     */
/*       BDX (0x40199274)                                                     */
/* Register default value:              0xF00000E4                            */
#define CPGC_SEQ_BAOCIC2_MC_MAINEXT_REG 0x0B014274
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Subsequence Address Ordering, Carry Masking, and Invert Control -
 * CPGC#_CH#_SEQ_BASE_ADDR_ORDER_CARRY_INVERT_CTL defines a series of enables that 
 * related to propagating carry bits for each address field, logic for inverting 
 * the current Base address, and how the various address fields will be ordered in 
 * the nested loop of LSB to MSB. 
 */
typedef union {
  struct {
    UINT32 column_addr_order : 2;
    /* column_addr_order - Bits[1:0], RW_LB, default = 2'b00 
       Determines the relative ordering of the Column address update logic in the 
       nested for loop in relation to the Rank, Bank, and Row address update logic. Any 
       address fields set to the same order that roll over will cause a distinct carry 
       of 1 to the next higher order address field (multiple simultaneous carries will 
       cause only one increment). All fields can be arbitrarily placed in any order as 
       long as all address order fields are set in a continuous order starting from 0 
       and may not skip over an order number. 
     */
    UINT32 row_addr_order : 2;
    /* row_addr_order - Bits[3:2], RW_LB, default = 2'b01 
       Same as Column_Addr_Order for Row
     */
    UINT32 bank_addr_order : 2;
    /* bank_addr_order - Bits[5:4], RW_LB, default = 2'b10 
       Same as Column_Addr_Order for Bank
     */
    UINT32 rank_addr_order : 2;
    /* rank_addr_order - Bits[7:6], RW_LB, default = 2'b11 
       Same as Column_Addr_Order for Rank
     */
    UINT32 rsvd_8 : 5;
    /* rsvd_8 - Bits[12:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 addr_inv_rate : 4;
    /* addr_inv_rate - Bits[16:13], RW_LB, default = 4'b0000 
       Address_Update_Rate_Mode dictates whether the Base address inversion logic is 
       based on # of cachelines written and read or by Loopcount (# of sequences 
       perfromed). 
       Address_Update_Rate_Mode = 0 means that the CPGC#_CH#_SEQ_BASE_ADDR_CURRENT is 
       inverted based on 2^(Base_Address_Invert_Rate) read and write cacheline 
       transactions. 
       Address_Update_Rate_Mode = 1 means that the CPGC#_CH#_SEQ_BASE_ADDR_CURRENT is 
       inverted based on 2^(Base_Address_Invert_Rate) Loops through the Sequence. 
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_ba_inven : 1;
    /* column_ba_inven - Bits[20:20], RW_LB, default = 1'b0 
       Setting Column_Base_Address_Invert_Enable bit = 1 will cause the 
       CPGC#_CH#_SEQ_BASE_ADDR_CURRENT.Column Address to be inverted every time 
       2^Base_Address_Invert_Rate cachelines are written or read or by Loopcount (see 
       Base_Address_Invert_Rate). 
       One must consider the address update rate when using this field.
       This feature is most often used for Memory array testing.
     */
    UINT32 row_ba_inven : 1;
    /* row_ba_inven - Bits[21:21], RW_LB, default = 1'b0 
       Same as Column_Ba_Inven for Row
     */
    UINT32 bank_ba_inven : 1;
    /* bank_ba_inven - Bits[22:22], RW_LB, default = 1'b0 
       Same as Column_Ba_Inven for Bank
     */
    UINT32 rank_ba_inven : 1;
    /* rank_ba_inven - Bits[23:23], RW_LB, default = 1'b0 
       Same as Column_Ba_Inven for Rank
     */
    UINT32 column_bw_trigen : 1;
    /* column_bw_trigen - Bits[24:24], RW_LB, default = 1'b0 
       If a Base Column Address Wrap occurs (see 
       CPGC#_CH#_SEQ_BASE_ADDR_WRAP.Column_Address) and Column_Base_Wrap_Trigger_Enable 
       is set to 1 then an Address Wrap Trigger has occurred which can be used in 
       conjunction with Stop_On_Wrap_Trigger to stop a Subsequence or Sequence. 
     */
    UINT32 row_bw_trigen : 1;
    /* row_bw_trigen - Bits[25:25], RW_LB, default = 1'b0 
       Same as Column_Bw_Trigen for Row
     */
    UINT32 bank_bw_trigen : 1;
    /* bank_bw_trigen - Bits[26:26], RW_LB, default = 1'b0 
       Same as Column_Bw_Trigen for Bank
     */
    UINT32 rank_bw_trigen : 1;
    /* rank_bw_trigen - Bits[27:27], RW_LB, default = 1'b0 
       Same as Column_Bw_Trigen for Rank
     */
    UINT32 column_bw_cryen : 1;
    /* column_bw_cryen - Bits[28:28], RW_LB, default = 1'b1 
       If a Base Column Address Wrap occurs (see 
       CPGC#_CH#_SEQ_BASE_ADDR_WRAP.Column_Address) and Column_Base_Wrap_Carry_Enable 
       is set to 1 then a carry increment from the Column Address (order N) will 
       propagate to the N+1 order Address field(s). 
     */
    UINT32 row_bw_cryen : 1;
    /* row_bw_cryen - Bits[29:29], RW_LB, default = 1'b1 
       Same as Column_Bw_Cryen for Row
     */
    UINT32 bank_bw_cryen : 1;
    /* bank_bw_cryen - Bits[30:30], RW_LB, default = 1'b1 
       Same as Column_Bw_Cryen for Bank
     */
    UINT32 rank_bw_cryen : 1;
    /* rank_bw_cryen - Bits[31:31], RW_LB, default = 1'b1 
       Same as Column_Bw_Cryen for Rank
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BAOCIC2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_BAOCIC3_MC_MAINEXT_REG supported on:                              */
/*       IVT_EP (0x40179278)                                                  */
/*       IVT_EX (0x40179278)                                                  */
/*       HSX (0x40199278)                                                     */
/*       BDX (0x40199278)                                                     */
/* Register default value:              0xF00000E4                            */
#define CPGC_SEQ_BAOCIC3_MC_MAINEXT_REG 0x0B014278
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Subsequence Address Ordering, Carry Masking, and Invert Control -
 * CPGC#_CH#_SEQ_BASE_ADDR_ORDER_CARRY_INVERT_CTL defines a series of enables that 
 * related to propagating carry bits for each address field, logic for inverting 
 * the current Base address, and how the various address fields will be ordered in 
 * the nested loop of LSB to MSB. 
 */
typedef union {
  struct {
    UINT32 column_addr_order : 2;
    /* column_addr_order - Bits[1:0], RW_LB, default = 2'b00 
       Determines the relative ordering of the Column address update logic in the 
       nested for loop in relation to the Rank, Bank, and Row 
       address update logic. Any address fields set to the same order that roll over 
       will cause a distinct carry of 1 to the next higher order 
       address field (multiple simultaneous carries will cause only one increment). All 
       fields can be arbitrarily placed in any order as long 
       as all address order fields are set in a continuous order starting from 0 and 
       may not skip over an order number. 
     */
    UINT32 row_addr_order : 2;
    /* row_addr_order - Bits[3:2], RW_LB, default = 2'b01 
       Same as Column_Addr_Order for Row
     */
    UINT32 bank_addr_order : 2;
    /* bank_addr_order - Bits[5:4], RW_LB, default = 2'b10 
       Same as Column_Addr_Order for Bank
     */
    UINT32 rank_addr_order : 2;
    /* rank_addr_order - Bits[7:6], RW_LB, default = 2'b11 
       Same as Column_Addr_Order for Rank
     */
    UINT32 rsvd_8 : 5;
    /* rsvd_8 - Bits[12:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 addr_inv_rate : 4;
    /* addr_inv_rate - Bits[16:13], RW_LB, default = 4'b0000 
       Address_Update_Rate_Mode dictates whether the Base address inversion logic is 
       based on # of cachelines written and read or by 
       Loopcount (# of sequences perfromed).
       Address_Update_Rate_Mode = 0 means that the CPGC#_CH#_SEQ_BASE_ADDR_CURRENT is 
       inverted based on 2^(Base_Address_Invert_Rate) read and 
       write cacheline transactions.
       Address_Update_Rate_Mode = 1 means that the CPGC#_CH#_SEQ_BASE_ADDR_CURRENT is 
       inverted based on 2^(Base_Address_Invert_Rate) Loops through 
       the Sequence.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_ba_inven : 1;
    /* column_ba_inven - Bits[20:20], RW_LB, default = 1'b0 
       Setting Column_Base_Address_Invert_Enable bit = 1 will cause the 
       CPGC#_CH#_SEQ_BASE_ADDR_CURRENT.Column Address to be inverted 
       every time 2^Base_Address_Invert_Rate cachelines are written or read or by 
       Loopcount (see Base_Address_Invert_Rate). 
       
       One must consider the address update rate when using this field.
       This feature is most often used for Memory array testing.
     */
    UINT32 row_ba_inven : 1;
    /* row_ba_inven - Bits[21:21], RW_LB, default = 1'b0 
       Same as Column_Ba_Inven for Row
     */
    UINT32 bank_ba_inven : 1;
    /* bank_ba_inven - Bits[22:22], RW_LB, default = 1'b0 
       Same as Column_Ba_Inven for Bank
     */
    UINT32 rank_ba_inven : 1;
    /* rank_ba_inven - Bits[23:23], RW_LB, default = 1'b0 
       Same as Column_Ba_Inven for Rank
     */
    UINT32 column_bw_trigen : 1;
    /* column_bw_trigen - Bits[24:24], RW_LB, default = 1'b0 
       If a Base Column Address Wrap occurs (see 
       CPGC#_CH#_SEQ_BASE_ADDR_WRAP.Column_Address) and Column_Base_Wrap_Trigger_Enable 
       is set to 1 then an Address Wrap Trigger has occurred which can be used in 
       conjunction with Stop_On_Wrap_Trigger to stop a 
       Subsequence or Sequence.
     */
    UINT32 row_bw_trigen : 1;
    /* row_bw_trigen - Bits[25:25], RW_LB, default = 1'b0 
       Same as Column_Bw_Trigen for Row
     */
    UINT32 bank_bw_trigen : 1;
    /* bank_bw_trigen - Bits[26:26], RW_LB, default = 1'b0 
       Same as Column_Bw_Trigen for Bank
     */
    UINT32 rank_bw_trigen : 1;
    /* rank_bw_trigen - Bits[27:27], RW_LB, default = 1'b0 
       Same as Column_Bw_Trigen for Rank
     */
    UINT32 column_bw_cryen : 1;
    /* column_bw_cryen - Bits[28:28], RW_LB, default = 1'b1 
       If a Base Column Address Wrap occurs (see 
       CPGC#_CH#_SEQ_BASE_ADDR_WRAP.Column_Address) and Column_Base_Wrap_Carry_Enable 
       is set to 1 then a carry increment from the Column Address (order N) will 
       propagate to the N+1 order Address field(s) . 
     */
    UINT32 row_bw_cryen : 1;
    /* row_bw_cryen - Bits[29:29], RW_LB, default = 1'b1 
       Same as Column_Bw_Cryen for Row
     */
    UINT32 bank_bw_cryen : 1;
    /* bank_bw_cryen - Bits[30:30], RW_LB, default = 1'b1 
       Same as Column_Bw_Cryen for Bank
     */
    UINT32 rank_bw_cryen : 1;
    /* rank_bw_cryen - Bits[31:31], RW_LB, default = 1'b1 
       Same as Column_Bw_Cryen for Rank
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BAOCIC3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_BAINCCTL00_MC_MAINEXT_REG supported on:                           */
/*       IVT_EP (0x4017927C)                                                  */
/*       IVT_EX (0x4017927C)                                                  */
/*       HSX (0x4019927C)                                                     */
/*       BDX (0x4019927C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BAINCCTL00_MC_MAINEXT_REG 0x0B01427C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Subsequence Base Address Increment Control -
 * Part0 - CPGC#_CH#_SEQ_BASE_ADDR_INC_CTL defines how the Base address increment 
 * will behave during a sequence. 
 */
typedef union {
  struct {
    UINT32 column_ba_inc : 8;
    /* column_ba_inc - Bits[7:0], RW_LB, default = 8'b00000000 
       When dictated by Column Address Update Rate the current Column Address will be 
       modified by the Column Address Increment (which is a 2's compliment field). 
       A value of 0x0 means the Column Address Increment is effectively disabled since 
       no offset will be applied. 
       Disabling the Column Address Increment by setting Column Address Increment to 
       0x0 does not affect a carry from a lower order field rolling over to a higher 
       order field which will also result in an increment to the higher order field of 
       1. 
     */
    UINT32 column_ba_updrate : 5;
    /* column_ba_updrate - Bits[12:8], RW_LB, default = 5'b00000 
       In linear mode (see Column_Address_Update_Rate_Scale setting), this field +1 
       defines how many times a specific Column address is repeated before adding the 
       Column_Base_Address_Increment. 
       In exponential mode, 2^(value in this field) defines how many times a specific 
       Rank address is repeated before adding the Column_Base_Address_Increment. 
       CPGC#_CH#_SEQ_CTL.Address_Update_Rate_Mode dictates whether the address 
       increment logic is driven by either the # of cachelines written and read or by 
       Loopcount (# of sequences performed). 
     */
    UINT32 column_ba_updscale : 1;
    /* column_ba_updscale - Bits[13:13], RW_LB, default = 1'b0 
       1= Column Address Update Rate is a linear number, 0 = Column Address Update Rate 
       is an exponential number 
     */
    UINT32 row_ba_inc : 12;
    /* row_ba_inc - Bits[25:14], RW_LB, default = 12'b000000000000 
       Same as Column_Ba_Inc for Row
     */
    UINT32 row_ba_updrate : 5;
    /* row_ba_updrate - Bits[30:26], RW_LB, default = 5'b00000 
       Same as Column_Ba_Updrate for Row
     */
    UINT32 row_ba_updscale : 1;
    /* row_ba_updscale - Bits[31:31], RW_LB, default = 1'b0 
       Same as Column_Ba_Updscale for Row
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BAINCCTL00_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_BAINCCTL10_MC_MAINEXT_REG supported on:                           */
/*       IVT_EP (0x40179280)                                                  */
/*       IVT_EX (0x40179280)                                                  */
/*       HSX (0x40199280)                                                     */
/*       BDX (0x40199280)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BAINCCTL10_MC_MAINEXT_REG 0x0B014280


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-0 Subsequence Base Address Increment Control -
 * Part1 - CPGC#_CH#_SEQ_BASE_ADDR_INC_CTL defines how the Base address increment 
 * will behave during a sequence. 
 */
typedef union {
  struct {
    UINT32 bank_ba_inc : 4;
    /* bank_ba_inc - Bits[3:0], RW_LB, default = 4'b0000 
       Same as Column_Ba_Inc for Bank
     */
    UINT32 bank_ba_updrate : 5;
    /* bank_ba_updrate - Bits[8:4], RW_LB, default = 5'b00000 
       Same as Column_Ba_Updrate for Bank
     */
    UINT32 bank_ba_updscale : 1;
    /* bank_ba_updscale - Bits[9:9], RW_LB, default = 1'b0 
       Same as Column_Ba_Updscale for Bank
     */
    UINT32 rank_ba_inc : 3;
    /* rank_ba_inc - Bits[12:10], RW_LB, default = 3'b000 
       Same as Column_Ba_Inc for Rank
     */
    UINT32 rank_ba_updrate : 5;
    /* rank_ba_updrate - Bits[17:13], RW_LB, default = 5'b00000 
       Same as Column_Ba_Updrate for Rank
     */
    UINT32 rank_ba_updscale : 1;
    /* rank_ba_updscale - Bits[18:18], RW_LB, default = 1'b0 
       Same as Column_Ba_Updscale for Rank
     */
    UINT32 column_ba_inc_msb : 2;
    /* column_ba_inc_msb - Bits[20:19], RW_LB, default = 2'b00 
       MSB of CPGC_SEQ_BAINCCTL00
     */
    UINT32 rsvd : 11;
    /* rsvd - Bits[31:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BAINCCTL10_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_BAINCCTL01_MC_MAINEXT_REG supported on:                           */
/*       IVT_EP (0x40179284)                                                  */
/*       IVT_EX (0x40179284)                                                  */
/*       HSX (0x40199284)                                                     */
/*       BDX (0x40199284)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BAINCCTL01_MC_MAINEXT_REG 0x0B014284
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Subsequence Base Address Increment Control -
 * Part0 - CPGC#_CH#_SEQ_BASE_ADDR_INC_CTL defines how the Base address increment 
 * will behave during a sequence. 
 */
typedef union {
  struct {
    UINT32 column_ba_inc : 8;
    /* column_ba_inc - Bits[7:0], RW_LB, default = 8'b00000000 
       When dictated by Column Address Update Rate the current Column Address will be 
       modified by the Column Address Increment (which is a 2's compliment field). 
       A value of 0x0 means the Column Address Increment is effectively disabled since 
       no offset will be applied. 
       Disabling the Column Address Increment by setting Column Address Increment to 
       0x0 does not affect a carry from a lower order field rolling over to a higher 
       order field which will also result in an increment to the higher order field of 
       1. 
     */
    UINT32 column_ba_updrate : 5;
    /* column_ba_updrate - Bits[12:8], RW_LB, default = 5'b00000 
       In linear mode (see Column_Address_Update_Rate_Scale setting), this field +1 
       defines how many times a specific Column address is repeated before adding the 
       Column_Base_Address_Increment. 
       In exponential mode, 2^(value in this field) defines how many times a specific 
       Rank address is repeated before adding the Column_Base_Address_Increment. 
       CPGC#_CH#_SEQ_CTL.Address_Update_Rate_Mode dictates whether the address 
       increment logic is driven by either the # of cachelines written and read or by 
       Loopcount (# of sequences performed). 
     */
    UINT32 column_ba_updscale : 1;
    /* column_ba_updscale - Bits[13:13], RW_LB, default = 1'b0 
       1= Column Address Update Rate is a linear number, 0 = Column Address Update Rate 
       is an exponential number 
     */
    UINT32 row_ba_inc : 12;
    /* row_ba_inc - Bits[25:14], RW_LB, default = 12'b000000000000 
       Same as Column_Ba_Inc for Row
     */
    UINT32 row_ba_updrate : 5;
    /* row_ba_updrate - Bits[30:26], RW_LB, default = 5'b00000 
       Same as Column_Ba_Updrate for Row
     */
    UINT32 row_ba_updscale : 1;
    /* row_ba_updscale - Bits[31:31], RW_LB, default = 1'b0 
       Same as Column_Ba_Updscale for Row
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BAINCCTL01_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_BAINCCTL11_MC_MAINEXT_REG supported on:                           */
/*       IVT_EP (0x40179288)                                                  */
/*       IVT_EX (0x40179288)                                                  */
/*       HSX (0x40199288)                                                     */
/*       BDX (0x40199288)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BAINCCTL11_MC_MAINEXT_REG 0x0B014288


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-1 Subsequence Base Address Increment Control -
 * Part1 - CPGC#_CH#_SEQ_BASE_ADDR_INC_CTL defines how the Base address increment 
 * will behave during a sequence. 
 */
typedef union {
  struct {
    UINT32 bank_ba_inc : 4;
    /* bank_ba_inc - Bits[3:0], RW_LB, default = 4'b0000 
       Same as Column_Ba_Inc for Bank
     */
    UINT32 bank_ba_updrate : 5;
    /* bank_ba_updrate - Bits[8:4], RW_LB, default = 5'b00000 
       Same as Column_Ba_Updrate for Bank
     */
    UINT32 bank_ba_updscale : 1;
    /* bank_ba_updscale - Bits[9:9], RW_LB, default = 1'b0 
       Same as Column_Ba_Updscale for Bank
     */
    UINT32 rank_ba_inc : 3;
    /* rank_ba_inc - Bits[12:10], RW_LB, default = 3'b000 
       Same as Column_Ba_Inc for Rank
     */
    UINT32 rank_ba_updrate : 5;
    /* rank_ba_updrate - Bits[17:13], RW_LB, default = 5'b00000 
       Same as Column_Ba_Updrate for Rank
     */
    UINT32 rank_ba_updscale : 1;
    /* rank_ba_updscale - Bits[18:18], RW_LB, default = 1'b0 
       Same as Column_Ba_Updscale for Rank
     */
    UINT32 column_ba_inc_msb : 2;
    /* column_ba_inc_msb - Bits[20:19], RW_LB, default = 2'b00 
       MSB of CPGC_SEQ_BAINCCTL00
     */
    UINT32 rsvd : 11;
    /* rsvd - Bits[31:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BAINCCTL11_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_BAINCCTL02_MC_MAINEXT_REG supported on:                           */
/*       IVT_EP (0x4017928C)                                                  */
/*       IVT_EX (0x4017928C)                                                  */
/*       HSX (0x4019928C)                                                     */
/*       BDX (0x4019928C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BAINCCTL02_MC_MAINEXT_REG 0x0B01428C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Subsequence Base Address Increment Control -
 * Part0 - CPGC#_CH#_SEQ_BASE_ADDR_INC_CTL defines how the Base address increment 
 * will behave during a sequence. 
 */
typedef union {
  struct {
    UINT32 column_ba_inc : 8;
    /* column_ba_inc - Bits[7:0], RW_LB, default = 8'b00000000 
       When dictated by Column Address Update Rate the current Column Address will be 
       modified by the Column Address Increment (which is a 2's compliment field). 
       A value of 0x0 means the Column Address Increment is effectively disabled since 
       no offset will be applied. 
       Disabling the Column Address Increment by setting Column Address Increment to 
       0x0 does not affect a carry from a lower order field rolling over to a higher 
       order field which will also result in an increment to the higher order field of 
       1. 
     */
    UINT32 column_ba_updrate : 5;
    /* column_ba_updrate - Bits[12:8], RW_LB, default = 5'b00000 
       In linear mode (see Column_Address_Update_Rate_Scale setting), this field +1 
       defines how many times a specific Column address is repeated before adding the 
       Column_Base_Address_Increment. 
       In exponential mode, 2^(value in this field) defines how many times a specific 
       Rank address is repeated before adding the Column_Base_Address_Increment. 
       CPGC#_CH#_SEQ_CTL.Address_Update_Rate_Mode dictates whether the address 
       increment logic is driven by either the # of cachelines written and read or by 
       Loopcount (# of sequences performed). 
     */
    UINT32 column_ba_updscale : 1;
    /* column_ba_updscale - Bits[13:13], RW_LB, default = 1'b0 
       1= Column Address Update Rate is a linear number, 0 = Column Address Update Rate 
       is an exponential number 
     */
    UINT32 row_ba_inc : 12;
    /* row_ba_inc - Bits[25:14], RW_LB, default = 12'b000000000000 
       Same as Column_Ba_Inc for Row
     */
    UINT32 row_ba_updrate : 5;
    /* row_ba_updrate - Bits[30:26], RW_LB, default = 5'b00000 
       Same as Column_Ba_Updrate for Row
     */
    UINT32 row_ba_updscale : 1;
    /* row_ba_updscale - Bits[31:31], RW_LB, default = 1'b0 
       Same as Column_Ba_Updscale for Row
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BAINCCTL02_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_BAINCCTL12_MC_MAINEXT_REG supported on:                           */
/*       IVT_EP (0x40179290)                                                  */
/*       IVT_EX (0x40179290)                                                  */
/*       HSX (0x40199290)                                                     */
/*       BDX (0x40199290)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BAINCCTL12_MC_MAINEXT_REG 0x0B014290


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-2 Subsequence Base Address Increment Control -
 * Part1 - CPGC#_CH#_SEQ_BASE_ADDR_INC_CTL defines how the Base address increment 
 * will behave during a sequence. 
 */
typedef union {
  struct {
    UINT32 bank_ba_inc : 4;
    /* bank_ba_inc - Bits[3:0], RW_LB, default = 4'b0000 
       Same as Column_Ba_Inc for Bank
     */
    UINT32 bank_ba_updrate : 5;
    /* bank_ba_updrate - Bits[8:4], RW_LB, default = 5'b00000 
       Same as Column_Ba_Updrate for Bank
     */
    UINT32 bank_ba_updscale : 1;
    /* bank_ba_updscale - Bits[9:9], RW_LB, default = 1'b0 
       Same as Column_Ba_Updscale for Bank
     */
    UINT32 rank_ba_inc : 3;
    /* rank_ba_inc - Bits[12:10], RW_LB, default = 3'b000 
       Same as Column_Ba_Inc for Rank
     */
    UINT32 rank_ba_updrate : 5;
    /* rank_ba_updrate - Bits[17:13], RW_LB, default = 5'b00000 
       Same as Column_Ba_Updrate for Rank
     */
    UINT32 rank_ba_updscale : 1;
    /* rank_ba_updscale - Bits[18:18], RW_LB, default = 1'b0 
       Same as Column_Ba_Updscale for Rank
     */
    UINT32 column_ba_inc_msb : 2;
    /* column_ba_inc_msb - Bits[20:19], RW_LB, default = 2'b00 
       MSB of CPGC_SEQ_BAINCCTL00
     */
    UINT32 rsvd : 11;
    /* rsvd - Bits[31:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BAINCCTL12_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_BAINCCTL03_MC_MAINEXT_REG supported on:                           */
/*       IVT_EP (0x40179294)                                                  */
/*       IVT_EX (0x40179294)                                                  */
/*       HSX (0x40199294)                                                     */
/*       BDX (0x40199294)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BAINCCTL03_MC_MAINEXT_REG 0x0B014294
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Subsequence Base Address Increment Control -
 * Part0 - CPGC#_CH#_SEQ_BASE_ADDR_INC_CTL defines how the Base address increment 
 * will behave during a sequence. 
 */
typedef union {
  struct {
    UINT32 column_ba_inc : 8;
    /* column_ba_inc - Bits[7:0], RW_LB, default = 8'b00000000 
       When dictated by Column Address Update Rate the current Column Address will be 
       modified by the Column Address Increment (which is a 2's compliment field). 
       A value of 0x0 means the Column Address Increment is effectively disabled since 
       no offset will be applied. 
       Disabling the Column Address Increment by setting Column Address Increment to 
       0x0 does not affect a carry from a lower order field rolling over to a higher 
       order field which will also result in an increment to the higher order field of 
       1. 
     */
    UINT32 column_ba_updrate : 5;
    /* column_ba_updrate - Bits[12:8], RW_LB, default = 5'b00000 
       In linear mode (see Column_Address_Update_Rate_Scale setting), this field +1 
       defines how many times a specific Column address is repeated before adding the 
       Column_Base_Address_Increment. 
       In exponential mode, 2^(value in this field) defines how many times a specific 
       Rank address is repeated before adding the Column_Base_Address_Increment. 
       CPGC#_CH#_SEQ_CTL.Address_Update_Rate_Mode dictates whether the address 
       increment logic is driven by either the # of cachelines written and read or by 
       Loopcount (# of sequences performed). 
     */
    UINT32 column_ba_updscale : 1;
    /* column_ba_updscale - Bits[13:13], RW_LB, default = 1'b0 
       1= Column Address Update Rate is a linear number, 0 = Column Address Update Rate 
       is an exponential number 
     */
    UINT32 row_ba_inc : 12;
    /* row_ba_inc - Bits[25:14], RW_LB, default = 12'b000000000000 
       Same as Column_Ba_Inc for Row
     */
    UINT32 row_ba_updrate : 5;
    /* row_ba_updrate - Bits[30:26], RW_LB, default = 5'b00000 
       Same as Column_Ba_Updrate for Row
     */
    UINT32 row_ba_updscale : 1;
    /* row_ba_updscale - Bits[31:31], RW_LB, default = 1'b0 
       Same as Column_Ba_Updscale for Row
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BAINCCTL03_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_BAINCCTL13_MC_MAINEXT_REG supported on:                           */
/*       IVT_EP (0x40179298)                                                  */
/*       IVT_EX (0x40179298)                                                  */
/*       HSX (0x40199298)                                                     */
/*       BDX (0x40199298)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BAINCCTL13_MC_MAINEXT_REG 0x0B014298


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-3 Subsequence Base Address Increment Control -
 * Part1 - CPGC#_CH#_SEQ_BASE_ADDR_INC_CTL defines how the Base address increment 
 * will behave during a sequence. 
 */
typedef union {
  struct {
    UINT32 bank_ba_inc : 4;
    /* bank_ba_inc - Bits[3:0], RW_LB, default = 4'b0000 
       Same as Column_Ba_Inc for Bank
     */
    UINT32 bank_ba_updrate : 5;
    /* bank_ba_updrate - Bits[8:4], RW_LB, default = 5'b00000 
       Same as Column_Ba_Updrate for Bank
     */
    UINT32 bank_ba_updscale : 1;
    /* bank_ba_updscale - Bits[9:9], RW_LB, default = 1'b0 
       Same as Column_Ba_Updscale for Bank
     */
    UINT32 rank_ba_inc : 3;
    /* rank_ba_inc - Bits[12:10], RW_LB, default = 3'b000 
       Same as Column_Ba_Inc for Rank
     */
    UINT32 rank_ba_updrate : 5;
    /* rank_ba_updrate - Bits[17:13], RW_LB, default = 5'b00000 
       Same as Column_Ba_Updrate for Rank
     */
    UINT32 rank_ba_updscale : 1;
    /* rank_ba_updscale - Bits[18:18], RW_LB, default = 1'b0 
       Same as Column_Ba_Updscale for Rank
     */
    UINT32 column_ba_inc_msb : 2;
    /* column_ba_inc_msb - Bits[20:19], RW_LB, default = 2'b00 
       MSB of CPGC_SEQ_BAINCCTL00
     */
    UINT32 rsvd : 11;
    /* rsvd - Bits[31:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BAINCCTL13_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_OFFADDRCUR0_MC_MAINEXT_REG supported on:                          */
/*       IVT_EP (0x4017929C)                                                  */
/*       IVT_EX (0x4017929C)                                                  */
/*       HSX (0x4019929C)                                                     */
/*       BDX (0x4019929C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_OFFADDRCUR0_MC_MAINEXT_REG 0x0B01429C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-0 Subsequence Offset Address -
 * Only Row and Column bits are used in an Offset Sequence Type 
 * (CPGC#_CH#_SUBSEQ#_CTL.subsequence_type = (0x0100 or 0x0101)) and thus the 
 *         Rank and Bank address will always be equal to and Row and Column address 
 * will never be equal to their respective fields in 
 * CPGC_CH#_SEQ_BASE_ADDR_CURRENT. 
 */
typedef union {
  struct {
    UINT32 col_addr : 10;
    /* col_addr - Bits[9:0], RO_V, default = 10'b0000000000 
       Current Column Offset Address. Normally, the lowest three column bits are 
       reserved since all transfers are done on a 
       cacheline basis (8 chunks 0-7), but for implementations that address to a sub 
       cacheline then these reserved bits might be used as needed. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_10 : 2;
    /* rsvd_10 - Bits[11:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_addr : 17;
    /* row_addr - Bits[28:12], RO_V, default = 17'b00000000000000000 
       Current Row Offset Address
     */
    UINT32 rsvd_29 : 3;
    /* rsvd_29 - Bits[31:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_OFFADDRCUR0_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_OFFADDRCUR1_MC_MAINEXT_REG supported on:                          */
/*       IVT_EP (0x401792A0)                                                  */
/*       IVT_EX (0x401792A0)                                                  */
/*       HSX (0x401992A0)                                                     */
/*       BDX (0x401992A0)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_OFFADDRCUR1_MC_MAINEXT_REG 0x0B0142A0


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-1 Subsequence Offset Address -
 * Only Row and Column bits are used in an Offset Sequence Type 
 * (CPGC#_CH#_SUBSEQ#_CTL.subsequence_type = (0x0100 or 0x0101)) and thus the 
 *         Rank and Bank address will always be equal to and Row and Column address 
 * will never be equal to their respective fields in 
 * CPGC_CH#_SEQ_BASE_ADDR_CURRENT. 
 */
typedef union {
  struct {
    UINT32 col_addr : 10;
    /* col_addr - Bits[9:0], RO_V, default = 10'b0000000000 
       Current Column Offset Address. Normally, the lowest three column bits are 
       reserved since all transfers are done on a 
       cacheline basis (8 chunks 0-7), but for implementations that address to a sub 
       cacheline then these reserved bits might be used as needed. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_10 : 2;
    /* rsvd_10 - Bits[11:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_addr : 17;
    /* row_addr - Bits[28:12], RO_V, default = 17'b00000000000000000 
       Current Row Offset Address
     */
    UINT32 rsvd_29 : 3;
    /* rsvd_29 - Bits[31:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_OFFADDRCUR1_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_OFFADDRCUR2_MC_MAINEXT_REG supported on:                          */
/*       IVT_EP (0x401792A4)                                                  */
/*       IVT_EX (0x401792A4)                                                  */
/*       HSX (0x401992A4)                                                     */
/*       BDX (0x401992A4)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_OFFADDRCUR2_MC_MAINEXT_REG 0x0B0142A4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-2 Subsequence Offset Address -
 * Only Row and Column bits are used in an Offset Sequence Type 
 * (CPGC#_CH#_SUBSEQ#_CTL.subsequence_type = (0x0100 or 0x0101)) and thus the 
 *         Rank and Bank address will always be equal to and Row and Column address 
 * will never be equal to their respective fields in 
 * CPGC_CH#_SEQ_BASE_ADDR_CURRENT. 
 */
typedef union {
  struct {
    UINT32 col_addr : 10;
    /* col_addr - Bits[9:0], RO_V, default = 10'b0000000000 
       Current Column Offset Address. Normally, the lowest three column bits are 
       reserved since all transfers are done on a 
       cacheline basis (8 chunks 0-7), but for implementations that address to a sub 
       cacheline then these reserved bits might be used as needed. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_10 : 2;
    /* rsvd_10 - Bits[11:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_addr : 17;
    /* row_addr - Bits[28:12], RO_V, default = 17'b00000000000000000 
       Current Row Offset Address
     */
    UINT32 rsvd_29 : 3;
    /* rsvd_29 - Bits[31:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_OFFADDRCUR2_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_OFFADDRCUR3_MC_MAINEXT_REG supported on:                          */
/*       IVT_EP (0x401792A8)                                                  */
/*       IVT_EX (0x401792A8)                                                  */
/*       HSX (0x401992A8)                                                     */
/*       BDX (0x401992A8)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_OFFADDRCUR3_MC_MAINEXT_REG 0x0B0142A8


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-3 Subsequence Offset Address -
 * Only Row and Column bits are used in an Offset Sequence Type 
 * (CPGC#_CH#_SUBSEQ#_CTL.subsequence_type = (0x0100 or 0x0101)) and thus the 
 *         Rank and Bank address will always be equal to and Row and Column address 
 * will never be equal to their respective fields in 
 * CPGC_CH#_SEQ_BASE_ADDR_CURRENT. 
 */
typedef union {
  struct {
    UINT32 col_addr : 10;
    /* col_addr - Bits[9:0], RO_V, default = 10'b0000000000 
       Current Column Offset Address. Normally, the lowest three column bits are 
       reserved since all transfers are done on a 
       cacheline basis (8 chunks 0-7), but for implementations that address to a sub 
       cacheline then these reserved bits might be used as needed. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_10 : 2;
    /* rsvd_10 - Bits[11:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_addr : 17;
    /* row_addr - Bits[28:12], RO_V, default = 17'b00000000000000000 
       Current Row Offset Address
     */
    UINT32 rsvd_29 : 3;
    /* rsvd_29 - Bits[31:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_OFFADDRCUR3_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_RANKLG2PHYMAP0_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x401792AC)                                                  */
/*       IVT_EX (0x401792AC)                                                  */
/*       HSX (0x401992AC)                                                     */
/*       BDX (0x401992AC)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_RANKLG2PHYMAP0_MC_MAINEXT_REG 0x0B0142AC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Subsequence Logical to Physical Rank Lookup Table -
 * CPGC#_CH#_SEQ_RANK_LOGICAL_TO_PHYSICAL_MAPPING is used to program the Logical to 
 * Physical Rank Mapping. 
 * 
 * The internal hardware is always is in the world of the Logical Ranks starting 
 * from Logical_to_Physical_Rank0_Mapping and will linearly increment through the 
 * Logical Ranks up to the maximum value (defined by 
 * CPGC#_CH#_SEQ_BASE_ADDR_WRAP.Rank_Address) 
 * 
 * The user must define what Physical Ranks are associated with each Logical Rank, 
 * and that only valid physical ranks are programmed into this register. 
 */
typedef union {
  struct {
    UINT32 rank0_mapping : 3;
    /* rank0_mapping - Bits[2:0], RW_LB, default = 3'b000 
       Defines what Physical Rank Address (Memory controller Rank Mapping) is mapped to 
       this logical Rank Address-0 (Sequence Address logic doamin). 
       
       It is possible to use more Logical_to_Physical_Rank#_Mapping fileds than actual 
       Physical Ranks which is useful for more complex tests where non linear Physical 
       Rank Sequences are needed. 
     */
    UINT32 rsvd_3 : 1;
    /* rsvd_3 - Bits[3:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank1_mapping : 3;
    /* rank1_mapping - Bits[6:4], RW_LB, default = 3'b000 
       Rank Address-1 mapping
     */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank2_mapping : 3;
    /* rank2_mapping - Bits[10:8], RW_LB, default = 3'b000 
       Rank Address-2 mapping
     */
    UINT32 rsvd_11 : 1;
    /* rsvd_11 - Bits[11:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank3_mapping : 3;
    /* rank3_mapping - Bits[14:12], RW_LB, default = 3'b000 
       Rank Address-3 mapping
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank4_mapping : 3;
    /* rank4_mapping - Bits[18:16], RW_LB, default = 3'b000 
       Rank Address-4 mapping
     */
    UINT32 rsvd_19 : 1;
    /* rsvd_19 - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank5_mapping : 3;
    /* rank5_mapping - Bits[22:20], RW_LB, default = 3'b000 
       Rank Address-5 mapping
     */
    UINT32 rsvd_23 : 1;
    /* rsvd_23 - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank6_mapping : 3;
    /* rank6_mapping - Bits[26:24], RW_LB, default = 3'b000 
       Rank Address-6 mapping
     */
    UINT32 rsvd_27 : 1;
    /* rsvd_27 - Bits[27:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank7_mapping : 3;
    /* rank7_mapping - Bits[30:28], RW_LB, default = 3'b000 
       Rank Address-7 mapping
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_RANKLG2PHYMAP0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_RANKLG2PHYMAP1_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x401792B0)                                                  */
/*       IVT_EX (0x401792B0)                                                  */
/*       HSX (0x401992B0)                                                     */
/*       BDX (0x401992B0)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_RANKLG2PHYMAP1_MC_MAINEXT_REG 0x0B0142B0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Subsequence Logical to Physical Rank Lookup Table -
 * CPGC#_CH#_SEQ_RANK_LOGICAL_TO_PHYSICAL_MAPPING is used to program the Logical to 
 * Physical Rank Mapping. 
 * 
 * The internal hardware is always is in the world of the Logical Ranks starting 
 * from Logical_to_Physical_Rank0_Mapping and will linearly increment through the 
 * Logical Ranks up to the maximum value (defined by 
 * CPGC#_CH#_SEQ_BASE_ADDR_WRAP.Rank_Address) 
 * 
 * The user must define what Physical Ranks are associated with each Logical Rank, 
 * and that only valid physical ranks are programmed into this register. 
 */
typedef union {
  struct {
    UINT32 rank0_mapping : 3;
    /* rank0_mapping - Bits[2:0], RW_LB, default = 3'b000 
       Defines what Physical Rank Address (Memory controller Rank Mapping) is mapped to 
       this logical Rank Address-0 (Sequence Address logic doamin). 
       
       It is possible to use more Logical_to_Physical_Rank#_Mapping fileds than actual 
       Physical Ranks which is useful for more complex tests where non linear Physical 
       Rank Sequences are needed. 
     */
    UINT32 rsvd_3 : 1;
    /* rsvd_3 - Bits[3:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank1_mapping : 3;
    /* rank1_mapping - Bits[6:4], RW_LB, default = 3'b000 
       Rank Address-1 mapping
     */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank2_mapping : 3;
    /* rank2_mapping - Bits[10:8], RW_LB, default = 3'b000 
       Rank Address-2 mapping
     */
    UINT32 rsvd_11 : 1;
    /* rsvd_11 - Bits[11:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank3_mapping : 3;
    /* rank3_mapping - Bits[14:12], RW_LB, default = 3'b000 
       Rank Address-3 mapping
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank4_mapping : 3;
    /* rank4_mapping - Bits[18:16], RW_LB, default = 3'b000 
       Rank Address-4 mapping
     */
    UINT32 rsvd_19 : 1;
    /* rsvd_19 - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank5_mapping : 3;
    /* rank5_mapping - Bits[22:20], RW_LB, default = 3'b000 
       Rank Address-5 mapping
     */
    UINT32 rsvd_23 : 1;
    /* rsvd_23 - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank6_mapping : 3;
    /* rank6_mapping - Bits[26:24], RW_LB, default = 3'b000 
       Rank Address-6 mapping
     */
    UINT32 rsvd_27 : 1;
    /* rsvd_27 - Bits[27:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank7_mapping : 3;
    /* rank7_mapping - Bits[30:28], RW_LB, default = 3'b000 
       Rank Address-7 mapping
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_RANKLG2PHYMAP1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_RANKLG2PHYMAP2_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x401792B4)                                                  */
/*       IVT_EX (0x401792B4)                                                  */
/*       HSX (0x401992B4)                                                     */
/*       BDX (0x401992B4)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_RANKLG2PHYMAP2_MC_MAINEXT_REG 0x0B0142B4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Subsequence Logical to Physical Rank Lookup Table -
 * CPGC#_CH#_SEQ_RANK_LOGICAL_TO_PHYSICAL_MAPPING is used to program the Logical to 
 * Physical Rank Mapping. 
 * 
 * The internal hardware is always is in the world of the Logical Ranks starting 
 * from Logical_to_Physical_Rank0_Mapping and will linearly increment through the 
 * Logical Ranks up to the maximum value (defined by 
 * CPGC#_CH#_SEQ_BASE_ADDR_WRAP.Rank_Address) 
 * 
 * The user must define what Physical Ranks are associated with each Logical Rank, 
 * and that only valid physical ranks are programmed into this register. 
 */
typedef union {
  struct {
    UINT32 rank0_mapping : 3;
    /* rank0_mapping - Bits[2:0], RW_LB, default = 3'b000 
       Defines what Physical Rank Address (Memory controller Rank Mapping) is mapped to 
       this logical Rank Address-0 (Sequence Address logic doamin). 
       
       It is possible to use more Logical_to_Physical_Rank#_Mapping fileds than actual 
       Physical Ranks which is useful for more complex tests where non linear Physical 
       Rank Sequences are needed. 
     */
    UINT32 rsvd_3 : 1;
    /* rsvd_3 - Bits[3:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank1_mapping : 3;
    /* rank1_mapping - Bits[6:4], RW_LB, default = 3'b000 
       Rank Address-1 mapping
     */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank2_mapping : 3;
    /* rank2_mapping - Bits[10:8], RW_LB, default = 3'b000 
       Rank Address-2 mapping
     */
    UINT32 rsvd_11 : 1;
    /* rsvd_11 - Bits[11:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank3_mapping : 3;
    /* rank3_mapping - Bits[14:12], RW_LB, default = 3'b000 
       Rank Address-3 mapping
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank4_mapping : 3;
    /* rank4_mapping - Bits[18:16], RW_LB, default = 3'b000 
       Rank Address-4 mapping
     */
    UINT32 rsvd_19 : 1;
    /* rsvd_19 - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank5_mapping : 3;
    /* rank5_mapping - Bits[22:20], RW_LB, default = 3'b000 
       Rank Address-5 mapping
     */
    UINT32 rsvd_23 : 1;
    /* rsvd_23 - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank6_mapping : 3;
    /* rank6_mapping - Bits[26:24], RW_LB, default = 3'b000 
       Rank Address-6 mapping
     */
    UINT32 rsvd_27 : 1;
    /* rsvd_27 - Bits[27:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank7_mapping : 3;
    /* rank7_mapping - Bits[30:28], RW_LB, default = 3'b000 
       Rank Address-7 mapping
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_RANKLG2PHYMAP2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_RANKLG2PHYMAP3_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x401792B8)                                                  */
/*       IVT_EX (0x401792B8)                                                  */
/*       HSX (0x401992B8)                                                     */
/*       BDX (0x401992B8)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_RANKLG2PHYMAP3_MC_MAINEXT_REG 0x0B0142B8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Subsequence Logical to Physical Rank Lookup Table -
 * CPGC#_CH#_SEQ_RANK_LOGICAL_TO_PHYSICAL_MAPPING is used to program the Logical to 
 * Physical Rank Mapping. 
 * 
 * The internal hardware is always is in the world of the Logical Ranks starting 
 * from Logical_to_Physical_Rank0_Mapping and will linearly increment through the 
 * Logical Ranks up to the maximum value (defined by 
 * CPGC#_CH#_SEQ_BASE_ADDR_WRAP.Rank_Address) 
 * 
 * The user must define what Physical Ranks are associated with each Logical Rank, 
 * and that only valid physical ranks are programmed into this register. 
 */
typedef union {
  struct {
    UINT32 rank0_mapping : 3;
    /* rank0_mapping - Bits[2:0], RW_LB, default = 3'b000 
       Defines what Physical Rank Address (Memory controller Rank Mapping) is mapped to 
       this logical Rank Address-0 (Sequence Address logic doamin). 
       
       It is possible to use more Logical_to_Physical_Rank#_Mapping fileds than actual 
       Physical Ranks which is useful for more complex tests where non linear Physical 
       Rank Sequences are needed. 
     */
    UINT32 rsvd_3 : 1;
    /* rsvd_3 - Bits[3:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank1_mapping : 3;
    /* rank1_mapping - Bits[6:4], RW_LB, default = 3'b000 
       Rank Address-1 mapping
     */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank2_mapping : 3;
    /* rank2_mapping - Bits[10:8], RW_LB, default = 3'b000 
       Rank Address-2 mapping
     */
    UINT32 rsvd_11 : 1;
    /* rsvd_11 - Bits[11:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank3_mapping : 3;
    /* rank3_mapping - Bits[14:12], RW_LB, default = 3'b000 
       Rank Address-3 mapping
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank4_mapping : 3;
    /* rank4_mapping - Bits[18:16], RW_LB, default = 3'b000 
       Rank Address-4 mapping
     */
    UINT32 rsvd_19 : 1;
    /* rsvd_19 - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank5_mapping : 3;
    /* rank5_mapping - Bits[22:20], RW_LB, default = 3'b000 
       Rank Address-5 mapping
     */
    UINT32 rsvd_23 : 1;
    /* rsvd_23 - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank6_mapping : 3;
    /* rank6_mapping - Bits[26:24], RW_LB, default = 3'b000 
       Rank Address-6 mapping
     */
    UINT32 rsvd_27 : 1;
    /* rsvd_27 - Bits[27:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank7_mapping : 3;
    /* rank7_mapping - Bits[30:28], RW_LB, default = 3'b000 
       Rank Address-7 mapping
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_RANKLG2PHYMAP3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_RASL0_MC_MAINEXT_REG supported on:                                */
/*       IVT_EP (0x401792BC)                                                  */
/*       IVT_EX (0x401792BC)                                                  */
/*       HSX (0x401992BC)                                                     */
/*       BDX (0x401992BC)                                                     */
/* Register default value:              0x0A418820                            */
#define CPGC_SEQ_RASL0_MC_MAINEXT_REG 0x0B0142BC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-0 Subsequence Logical to Physical Row Address Swizzling Lower -
 * This register is used to swizzle the Lower Logial to Physical Row Bits.
 * The Lower bits swizzling is desired when testing of DIMM arrays is the goal and 
 * when the DRAM DUT internally swizles the logical to 
 *         physical row bits within the DRAM.
 * By remapping the lower bits to undo the swizzling within the DRAM adjacent row 
 * DIMM array testing can take place which is a major goal in DIMM array testing. 
 */
typedef union {
  struct {
    UINT32 log2phy_row0_swzl : 5;
    /* log2phy_row0_swzl - Bits[4:0], RW_LB, default = 5'b00000 
       Defines how a particular logical Row Bit is remapped to a Physical Row Address. 
       The bits are essentially swizzled with each other. 
       
       Remapping the lower Row bits allows one to more effectively test adjacent rows 
       in a DRAM when the Row swizzling is used within the DRAM. 
       Remapping any two or more row bits to the same value is not allowed and the 
       behavior is undefined. 
       Remapping is a 2 step process with a write required to both 
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER and CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER 
       
       Example using Logical Row Bit X:
       If a swizzle between X and Y is desired then
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER.Logical_to_Physical_RowX_Swizzle must be 
       set to Y 
       And
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER.Logical_to_Physical_RowY_Swizzle must be 
       set to X 
       Note:
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER.Logical_to_Physical_RowY_Swizzle can be 
       only be set to equal itself or the largest 
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER.Logical_to_Physical_RowX_Swizzle value
     */
    UINT32 log2phy_row1_swzl : 5;
    /* log2phy_row1_swzl - Bits[9:5], RW_LB, default = 5'b00001 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 log2phy_row2_swzl : 5;
    /* log2phy_row2_swzl - Bits[14:10], RW_LB, default = 5'b00010 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 log2phy_row3_swzl : 5;
    /* log2phy_row3_swzl - Bits[19:15], RW_LB, default = 5'b00011 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 log2phy_row4_swzl : 5;
    /* log2phy_row4_swzl - Bits[24:20], RW_LB, default = 5'b00100 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 log2phy_row5_swzl : 5;
    /* log2phy_row5_swzl - Bits[29:25], RW_LB, default = 5'b00101 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_RASL0_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_RASL1_MC_MAINEXT_REG supported on:                                */
/*       IVT_EP (0x401792C0)                                                  */
/*       IVT_EX (0x401792C0)                                                  */
/*       HSX (0x401992C0)                                                     */
/*       BDX (0x401992C0)                                                     */
/* Register default value:              0x0A418820                            */
#define CPGC_SEQ_RASL1_MC_MAINEXT_REG 0x0B0142C0


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-1 Subsequence Logical to Physical Row Address Swizzling Lower -
 * This register is used to swizzle the Lower Logial to Physical Row Bits.
 * The Lower bits swizzling is desired when testing of DIMM arrays is the goal and 
 * when the DRAM DUT internally swizles the logical to 
 *         physical row bits within the DRAM.
 * By remapping the lower bits to undo the swizzling within the DRAM adjacent row 
 * DIMM array testing can take place which is a major goal in DIMM array testing. 
 */
typedef union {
  struct {
    UINT32 log2phy_row0_swzl : 5;
    /* log2phy_row0_swzl - Bits[4:0], RW_LB, default = 5'b00000 
       Defines how a particular logical Row Bit is remapped to a Physical Row Address. 
       The bits are essentially swizzled with each other. 
       
       Remapping the lower Row bits allows one to more effectively test adjacent rows 
       in a DRAM when the Row swizzling is used within the DRAM. 
       Remapping any two or more row bits to the same value is not allowed and the 
       behavior is undefined. 
       Remapping is a 2 step process with a write required to both 
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER and CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER 
       
       Example using Logical Row Bit X:
       If a swizzle between X and Y is desired then
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER.Logical_to_Physical_RowX_Swizzle must be 
       set to Y 
       And
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER.Logical_to_Physical_RowY_Swizzle must be 
       set to X 
       Note:
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER.Logical_to_Physical_RowY_Swizzle can be 
       only be set to equal itself or the largest 
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER.Logical_to_Physical_RowX_Swizzle value
     */
    UINT32 log2phy_row1_swzl : 5;
    /* log2phy_row1_swzl - Bits[9:5], RW_LB, default = 5'b00001 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 log2phy_row2_swzl : 5;
    /* log2phy_row2_swzl - Bits[14:10], RW_LB, default = 5'b00010 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 log2phy_row3_swzl : 5;
    /* log2phy_row3_swzl - Bits[19:15], RW_LB, default = 5'b00011 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 log2phy_row4_swzl : 5;
    /* log2phy_row4_swzl - Bits[24:20], RW_LB, default = 5'b00100 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 log2phy_row5_swzl : 5;
    /* log2phy_row5_swzl - Bits[29:25], RW_LB, default = 5'b00101 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_RASL1_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_RASL2_MC_MAINEXT_REG supported on:                                */
/*       IVT_EP (0x401792C4)                                                  */
/*       IVT_EX (0x401792C4)                                                  */
/*       HSX (0x401992C4)                                                     */
/*       BDX (0x401992C4)                                                     */
/* Register default value:              0x0A418820                            */
#define CPGC_SEQ_RASL2_MC_MAINEXT_REG 0x0B0142C4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-2 Subsequence Logical to Physical Row Address Swizzling Lower -
 * This register is used to swizzle the Lower Logial to Physical Row Bits.
 * The Lower bits swizzling is desired when testing of DIMM arrays is the goal and 
 * when the DRAM DUT internally swizles the logical to 
 *         physical row bits within the DRAM.
 * By remapping the lower bits to undo the swizzling within the DRAM adjacent row 
 * DIMM array testing can take place which is a major goal in DIMM array testing. 
 */
typedef union {
  struct {
    UINT32 log2phy_row0_swzl : 5;
    /* log2phy_row0_swzl - Bits[4:0], RW_LB, default = 5'b00000 
       Defines how a particular logical Row Bit is remapped to a Physical Row Address. 
       The bits are essentially swizzled with each other. 
       
       Remapping the lower Row bits allows one to more effectively test adjacent rows 
       in a DRAM when the Row swizzling is used within the DRAM. 
       Remapping any two or more row bits to the same value is not allowed and the 
       behavior is undefined. 
       Remapping is a 2 step process with a write required to both 
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER and CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER 
       
       Example using Logical Row Bit X:
       If a swizzle between X and Y is desired then
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER.Logical_to_Physical_RowX_Swizzle must be 
       set to Y 
       And
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER.Logical_to_Physical_RowY_Swizzle must be 
       set to X 
       Note:
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER.Logical_to_Physical_RowY_Swizzle can be 
       only be set to equal itself or the largest 
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER.Logical_to_Physical_RowX_Swizzle value
     */
    UINT32 log2phy_row1_swzl : 5;
    /* log2phy_row1_swzl - Bits[9:5], RW_LB, default = 5'b00001 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 log2phy_row2_swzl : 5;
    /* log2phy_row2_swzl - Bits[14:10], RW_LB, default = 5'b00010 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 log2phy_row3_swzl : 5;
    /* log2phy_row3_swzl - Bits[19:15], RW_LB, default = 5'b00011 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 log2phy_row4_swzl : 5;
    /* log2phy_row4_swzl - Bits[24:20], RW_LB, default = 5'b00100 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 log2phy_row5_swzl : 5;
    /* log2phy_row5_swzl - Bits[29:25], RW_LB, default = 5'b00101 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_RASL2_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_RASL3_MC_MAINEXT_REG supported on:                                */
/*       IVT_EP (0x401792C8)                                                  */
/*       IVT_EX (0x401792C8)                                                  */
/*       HSX (0x401992C8)                                                     */
/*       BDX (0x401992C8)                                                     */
/* Register default value:              0x0A418820                            */
#define CPGC_SEQ_RASL3_MC_MAINEXT_REG 0x0B0142C8


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-3 Subsequence Logical to Physical Row Address Swizzling Lower -
 * This register is used to swizzle the Lower Logial to Physical Row Bits.
 * The Lower bits swizzling is desired when testing of DIMM arrays is the goal and 
 * when the DRAM DUT internally swizles the logical to 
 *         physical row bits within the DRAM.
 * By remapping the lower bits to undo the swizzling within the DRAM adjacent row 
 * DIMM array testing can take place which is a major goal in DIMM array testing. 
 */
typedef union {
  struct {
    UINT32 log2phy_row0_swzl : 5;
    /* log2phy_row0_swzl - Bits[4:0], RW_LB, default = 5'b00000 
       Defines how a particular logical Row Bit is remapped to a Physical Row Address. 
       The bits are essentially swizzled with each other. 
       
       Remapping the lower Row bits allows one to more effectively test adjacent rows 
       in a DRAM when the Row swizzling is used within the DRAM. 
       Remapping any two or more row bits to the same value is not allowed and the 
       behavior is undefined. 
       Remapping is a 2 step process with a write required to both 
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER and CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER 
       
       Example using Logical Row Bit X:
       If a swizzle between X and Y is desired then
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER.Logical_to_Physical_RowX_Swizzle must be 
       set to Y 
       And
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER.Logical_to_Physical_RowY_Swizzle must be 
       set to X 
       Note:
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER.Logical_to_Physical_RowY_Swizzle can be 
       only be set to equal itself or the largest 
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER.Logical_to_Physical_RowX_Swizzle value
     */
    UINT32 log2phy_row1_swzl : 5;
    /* log2phy_row1_swzl - Bits[9:5], RW_LB, default = 5'b00001 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 log2phy_row2_swzl : 5;
    /* log2phy_row2_swzl - Bits[14:10], RW_LB, default = 5'b00010 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 log2phy_row3_swzl : 5;
    /* log2phy_row3_swzl - Bits[19:15], RW_LB, default = 5'b00011 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 log2phy_row4_swzl : 5;
    /* log2phy_row4_swzl - Bits[24:20], RW_LB, default = 5'b00100 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 log2phy_row5_swzl : 5;
    /* log2phy_row5_swzl - Bits[29:25], RW_LB, default = 5'b00101 
       Refer to Log2phy_Row1_Swzl for details
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_RASL3_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_RASU00_MC_MAINEXT_REG supported on:                               */
/*       IVT_EP (0x401792CC)                                                  */
/*       IVT_EX (0x401792CC)                                                  */
/*       HSX (0x401992CC)                                                     */
/*       BDX (0x401992CC)                                                     */
/* Register default value on IVT_EP:    0xDCBA9876                            */
/* Register default value on IVT_EX:    0xDCBA9876                            */
/* Register default value on HSX:       0x16A4A0E6                            */
/* Register default value on BDX:       0x16A4A0E6                            */
#define CPGC_SEQ_RASU00_MC_MAINEXT_REG 0x0B0142CC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-0 Subsequence Logical to Physical Row Address Swizzling Upper -
 * Part-0: This register is used to swizzle the Upper Logial to Physical Row Bits.
 */
typedef union {
  struct {
    UINT32 log2phy_row6_swzl : 5;
    /* log2phy_row6_swzl - Bits[4:0], RW_LB, default = 5'b00110 
       Defines how a particular logical Row Bit is remapped to a Physical Row Address. 
       The bits are essentially swizzled with each other. 
       
       Example using Logical Row Bit X:
       If a Logical_to_Physical_RowX_Swizzle = Y then this implies that
       Physical Row bit Y = X
       Physical Row bit X = Y
       
       Remapping Row bits allows one to more effectively test adjacent rows in a DRAM 
       when the Row swizzling is used within the DRAM. 
       Remapping any two or more row bits to the same value is not allowed and the 
       behavior is undefined. 
     */
    UINT32 log2phy_row7_swzl : 5;
    /* log2phy_row7_swzl - Bits[9:5], RW_LB, default = 5'b00111 
       Refer to Log2phy_Row6_Swzl for details
     */
    UINT32 log2phy_row8_swzl : 5;
    /* log2phy_row8_swzl - Bits[14:10], RW_LB, default = 5'b01000 
       Refer to Log2phy_Row6_Swzl for details
     */
    UINT32 log2phy_row9_swzl : 5;
    /* log2phy_row9_swzl - Bits[19:15], RW_LB, default = 5'b01001 
       Refer to Log2phy_Row6_Swzl for details
     */
    UINT32 log2phy_row10_swzl : 5;
    /* log2phy_row10_swzl - Bits[24:20], RW_LB, default = 5'b01010 
       Refer to Log2phy_Row6_Swzl for details
     */
    UINT32 log2phy_row11_swzl : 5;
    /* log2phy_row11_swzl - Bits[29:25], RW_LB, default = 5'b01011 
       Defines how a particular logical Row Bit is remapped to a Physical Row Address. 
       The bits are essentially swizzled with each other. 
       Remapping the lower Row bits allows one to more effectively test adjacent rows 
       in a DRAM when the Row swizzling is used within the DRAM. 
       Remapping any two or more row bits to the same value is not allowed and the 
       behavior is undefined. 
       Remapping is a 2 step process with a write required to both 
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER and CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER 
       
       Example using Logical Row Bit X:
       If a swizzle between X and Y is desired then
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER.Logical_to_Physical_RowX_Swizzle must be 
       set to Y 
       And
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER.Logical_to_Physical_RowY_Swizzle must be 
       set to X 
       Note:
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER.Logical_to_Physical_RowY_Swizzle can be 
       only be set to equal itself or the largest 
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER.Logical_to_Physical_RowX_Swizzle value
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_RASU00_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_RASU10_MC_MAINEXT_REG supported on:                               */
/*       IVT_EP (0x401792D0)                                                  */
/*       IVT_EX (0x401792D0)                                                  */
/*       HSX (0x401992D0)                                                     */
/*       BDX (0x401992D0)                                                     */
/* Register default value on IVT_EP:    0x000000FE                            */
/* Register default value on IVT_EX:    0x000000FE                            */
/* Register default value on HSX:       0x0107B9AC                            */
/* Register default value on BDX:       0x0107B9AC                            */
#define CPGC_SEQ_RASU10_MC_MAINEXT_REG 0x0B0142D0


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-0 Subsequence Logical to Physical Row Address Swizzling Upper -
 * Part-1: This register is used to swizzle the Upper Logial to Physical Row Bits.
 */
typedef union {
  struct {
    UINT32 log2phy_row12_swzl : 5;
    /* log2phy_row12_swzl - Bits[4:0], RW_LB, default = 5'b01100 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 log2phy_row13_swzl : 5;
    /* log2phy_row13_swzl - Bits[9:5], RW_LB, default = 5'b01101 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 log2phy_row14_swzl : 5;
    /* log2phy_row14_swzl - Bits[14:10], RW_LB, default = 5'b01110 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 log2phy_row15_swzl : 5;
    /* log2phy_row15_swzl - Bits[19:15], RW_LB, default = 5'b01111 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 log2phy_row16_swzl : 5;
    /* log2phy_row16_swzl - Bits[24:20], RW_LB, default = 5'b10000 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 rsvd : 7;
    /* rsvd - Bits[31:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_RASU10_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_RASU01_MC_MAINEXT_REG supported on:                               */
/*       IVT_EP (0x401792D4)                                                  */
/*       IVT_EX (0x401792D4)                                                  */
/*       HSX (0x401992D4)                                                     */
/*       BDX (0x401992D4)                                                     */
/* Register default value on IVT_EP:    0xDCBA9876                            */
/* Register default value on IVT_EX:    0xDCBA9876                            */
/* Register default value on HSX:       0x16A4A0E6                            */
/* Register default value on BDX:       0x16A4A0E6                            */
#define CPGC_SEQ_RASU01_MC_MAINEXT_REG 0x0B0142D4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * Part-0: This register is used to swizzle the Upper Logial to Physical Row Bits.
 */
typedef union {
  struct {
    UINT32 log2phy_row6_swzl : 5;
    /* log2phy_row6_swzl - Bits[4:0], RW_LB, default = 5'b00110 
       Defines how a particular logical Row Bit is remapped to a Physical Row Address. 
       The bits are essentially swizzled with each other. 
       
       Example using Logical Row Bit X:
       If a Logical_to_Physical_RowX_Swizzle = Y then this implies that
       Physical Row bit Y = X
       Physical Row bit X = Y
       
       Remapping Row bits allows one to more effectively test adjacent rows in a DRAM 
       when the Row swizzling is used within the DRAM. 
       Remapping any two or more row bits to the same value is not allowed and the 
       behavior is undefined. 
     */
    UINT32 log2phy_row7_swzl : 5;
    /* log2phy_row7_swzl - Bits[9:5], RW_LB, default = 5'b00111 
       Refer to Log2phy_Row6_Swzl for details
     */
    UINT32 log2phy_row8_swzl : 5;
    /* log2phy_row8_swzl - Bits[14:10], RW_LB, default = 5'b01000 
       Refer to Log2phy_Row6_Swzl for details
     */
    UINT32 log2phy_row9_swzl : 5;
    /* log2phy_row9_swzl - Bits[19:15], RW_LB, default = 5'b01001 
       Refer to Log2phy_Row6_Swzl for details
     */
    UINT32 log2phy_row10_swzl : 5;
    /* log2phy_row10_swzl - Bits[24:20], RW_LB, default = 5'b01010 
       Refer to Log2phy_Row6_Swzl for details
     */
    UINT32 log2phy_row11_swzl : 5;
    /* log2phy_row11_swzl - Bits[29:25], RW_LB, default = 5'b01011 
       Defines how a particular logical Row Bit is remapped to a Physical Row Address. 
       The bits are essentially swizzled with each other. 
       Remapping the lower Row bits allows one to more effectively test adjacent rows 
       in a DRAM when the Row swizzling is used within the DRAM. 
       Remapping any two or more row bits to the same value is not allowed and the 
       behavior is undefined. 
       Remapping is a 2 step process with a write required to both 
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER and CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER 
       
       Example using Logical Row Bit X:
       If a swizzle between X and Y is desired then
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER.Logical_to_Physical_RowX_Swizzle must be 
       set to Y 
       And
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER.Logical_to_Physical_RowY_Swizzle must be 
       set to X 
       Note:
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER.Logical_to_Physical_RowY_Swizzle can be 
       only be set to equal itself or the largest 
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER.Logical_to_Physical_RowX_Swizzle value
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_RASU01_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_RASU11_MC_MAINEXT_REG supported on:                               */
/*       IVT_EP (0x401792D8)                                                  */
/*       IVT_EX (0x401792D8)                                                  */
/*       HSX (0x401992D8)                                                     */
/*       BDX (0x401992D8)                                                     */
/* Register default value on IVT_EP:    0x000000FE                            */
/* Register default value on IVT_EX:    0x000000FE                            */
/* Register default value on HSX:       0x0107B9AC                            */
/* Register default value on BDX:       0x0107B9AC                            */
#define CPGC_SEQ_RASU11_MC_MAINEXT_REG 0x0B0142D8


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-1 Subsequence Logical to Physical Row Address Swizzling Upper -
 * Part-1: This register is used to swizzle the Upper Logial to Physical Row Bits.
 */
typedef union {
  struct {
    UINT32 log2phy_row12_swzl : 5;
    /* log2phy_row12_swzl - Bits[4:0], RW_LB, default = 5'b01100 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 log2phy_row13_swzl : 5;
    /* log2phy_row13_swzl - Bits[9:5], RW_LB, default = 5'b01101 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 log2phy_row14_swzl : 5;
    /* log2phy_row14_swzl - Bits[14:10], RW_LB, default = 5'b01110 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 log2phy_row15_swzl : 5;
    /* log2phy_row15_swzl - Bits[19:15], RW_LB, default = 5'b01111 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 log2phy_row16_swzl : 5;
    /* log2phy_row16_swzl - Bits[24:20], RW_LB, default = 5'b10000 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 rsvd : 7;
    /* rsvd - Bits[31:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_RASU11_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_RASU02_MC_MAINEXT_REG supported on:                               */
/*       IVT_EP (0x401792DC)                                                  */
/*       IVT_EX (0x401792DC)                                                  */
/*       HSX (0x401992DC)                                                     */
/*       BDX (0x401992DC)                                                     */
/* Register default value on IVT_EP:    0xDCBA9876                            */
/* Register default value on IVT_EX:    0xDCBA9876                            */
/* Register default value on HSX:       0x16A4A0E6                            */
/* Register default value on BDX:       0x16A4A0E6                            */
#define CPGC_SEQ_RASU02_MC_MAINEXT_REG 0x0B0142DC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * Part-0: This register is used to swizzle the Upper Logial to Physical Row Bits.
 */
typedef union {
  struct {
    UINT32 log2phy_row6_swzl : 5;
    /* log2phy_row6_swzl - Bits[4:0], RW_LB, default = 5'b00110 
       Defines how a particular logical Row Bit is remapped to a Physical Row Address. 
       The bits are essentially swizzled with each other. 
       
       Example using Logical Row Bit X:
       If a Logical_to_Physical_RowX_Swizzle = Y then this implies that
       Physical Row bit Y = X
       Physical Row bit X = Y
       
       Remapping Row bits allows one to more effectively test adjacent rows in a DRAM 
       when the Row swizzling is used within the DRAM. 
       Remapping any two or more row bits to the same value is not allowed and the 
       behavior is undefined. 
     */
    UINT32 log2phy_row7_swzl : 5;
    /* log2phy_row7_swzl - Bits[9:5], RW_LB, default = 5'b00111 
       Refer to Log2phy_Row6_Swzl for details
     */
    UINT32 log2phy_row8_swzl : 5;
    /* log2phy_row8_swzl - Bits[14:10], RW_LB, default = 5'b01000 
       Refer to Log2phy_Row6_Swzl for details
     */
    UINT32 log2phy_row9_swzl : 5;
    /* log2phy_row9_swzl - Bits[19:15], RW_LB, default = 5'b01001 
       Refer to Log2phy_Row6_Swzl for details
     */
    UINT32 log2phy_row10_swzl : 5;
    /* log2phy_row10_swzl - Bits[24:20], RW_LB, default = 5'b01010 
       Refer to Log2phy_Row6_Swzl for details
     */
    UINT32 log2phy_row11_swzl : 5;
    /* log2phy_row11_swzl - Bits[29:25], RW_LB, default = 5'b01011 
       Defines how a particular logical Row Bit is remapped to a Physical Row Address. 
       The bits are essentially swizzled with each other. 
       Remapping the lower Row bits allows one to more effectively test adjacent rows 
       in a DRAM when the Row swizzling is used within the DRAM. 
       Remapping any two or more row bits to the same value is not allowed and the 
       behavior is undefined. 
       Remapping is a 2 step process with a write required to both 
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER and CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER 
       
       Example using Logical Row Bit X:
       If a swizzle between X and Y is desired then
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER.Logical_to_Physical_RowX_Swizzle must be 
       set to Y 
       And
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER.Logical_to_Physical_RowY_Swizzle must be 
       set to X 
       Note:
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER.Logical_to_Physical_RowY_Swizzle can be 
       only be set to equal itself or the largest 
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER.Logical_to_Physical_RowX_Swizzle value
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_RASU02_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_RASU12_MC_MAINEXT_REG supported on:                               */
/*       IVT_EP (0x401792E0)                                                  */
/*       IVT_EX (0x401792E0)                                                  */
/*       HSX (0x401992E0)                                                     */
/*       BDX (0x401992E0)                                                     */
/* Register default value on IVT_EP:    0x000000FE                            */
/* Register default value on IVT_EX:    0x000000FE                            */
/* Register default value on HSX:       0x0107B9AC                            */
/* Register default value on BDX:       0x0107B9AC                            */
#define CPGC_SEQ_RASU12_MC_MAINEXT_REG 0x0B0142E0


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-2 Subsequence Logical to Physical Row Address Swizzling Upper -
 * Part-1: This register is used to swizzle the Upper Logial to Physical Row Bits.
 */
typedef union {
  struct {
    UINT32 log2phy_row12_swzl : 5;
    /* log2phy_row12_swzl - Bits[4:0], RW_LB, default = 5'b01100 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 log2phy_row13_swzl : 5;
    /* log2phy_row13_swzl - Bits[9:5], RW_LB, default = 5'b01101 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 log2phy_row14_swzl : 5;
    /* log2phy_row14_swzl - Bits[14:10], RW_LB, default = 5'b01110 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 log2phy_row15_swzl : 5;
    /* log2phy_row15_swzl - Bits[19:15], RW_LB, default = 5'b01111 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 log2phy_row16_swzl : 5;
    /* log2phy_row16_swzl - Bits[24:20], RW_LB, default = 5'b10000 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 rsvd : 7;
    /* rsvd - Bits[31:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_RASU12_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_RASU03_MC_MAINEXT_REG supported on:                               */
/*       IVT_EP (0x401792E4)                                                  */
/*       IVT_EX (0x401792E4)                                                  */
/*       HSX (0x401992E4)                                                     */
/*       BDX (0x401992E4)                                                     */
/* Register default value on IVT_EP:    0xDCBA9876                            */
/* Register default value on IVT_EX:    0xDCBA9876                            */
/* Register default value on HSX:       0x16A4A0E6                            */
/* Register default value on BDX:       0x16A4A0E6                            */
#define CPGC_SEQ_RASU03_MC_MAINEXT_REG 0x0B0142E4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * Part-0: This register is used to swizzle the Upper Logial to Physical Row Bits.
 */
typedef union {
  struct {
    UINT32 log2phy_row6_swzl : 5;
    /* log2phy_row6_swzl - Bits[4:0], RW_LB, default = 5'b00110 
       Defines how a particular logical Row Bit is remapped to a Physical Row Address. 
       The bits are essentially swizzled with each other. 
       
       Example using Logical Row Bit X:
       If a Logical_to_Physical_RowX_Swizzle = Y then this implies that
       Physical Row bit Y = X
       Physical Row bit X = Y
       
       Remapping Row bits allows one to more effectively test adjacent rows in a DRAM 
       when the Row swizzling is used within the DRAM. 
       Remapping any two or more row bits to the same value is not allowed and the 
       behavior is undefined. 
     */
    UINT32 log2phy_row7_swzl : 5;
    /* log2phy_row7_swzl - Bits[9:5], RW_LB, default = 5'b00111 
       Refer to Log2phy_Row6_Swzl for details
     */
    UINT32 log2phy_row8_swzl : 5;
    /* log2phy_row8_swzl - Bits[14:10], RW_LB, default = 5'b01000 
       Refer to Log2phy_Row6_Swzl for details
     */
    UINT32 log2phy_row9_swzl : 5;
    /* log2phy_row9_swzl - Bits[19:15], RW_LB, default = 5'b01001 
       Refer to Log2phy_Row6_Swzl for details
     */
    UINT32 log2phy_row10_swzl : 5;
    /* log2phy_row10_swzl - Bits[24:20], RW_LB, default = 5'b01010 
       Refer to Log2phy_Row6_Swzl for details
     */
    UINT32 log2phy_row11_swzl : 5;
    /* log2phy_row11_swzl - Bits[29:25], RW_LB, default = 5'b01011 
       Defines how a particular logical Row Bit is remapped to a Physical Row Address. 
       The bits are essentially swizzled with each other. 
       Remapping the lower Row bits allows one to more effectively test adjacent rows 
       in a DRAM when the Row swizzling is used within the DRAM. 
       Remapping any two or more row bits to the same value is not allowed and the 
       behavior is undefined. 
       Remapping is a 2 step process with a write required to both 
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER and CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER 
       
       Example using Logical Row Bit X:
       If a swizzle between X and Y is desired then
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER.Logical_to_Physical_RowX_Swizzle must be 
       set to Y 
       And
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER.Logical_to_Physical_RowY_Swizzle must be 
       set to X 
       Note:
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_UPPER.Logical_to_Physical_RowY_Swizzle can be 
       only be set to equal itself or the largest 
       CPGC#_CH#_SEQ_ROW_ADDR_SWIZZLE_LOWER.Logical_to_Physical_RowX_Swizzle value
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_RASU03_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_RASU13_MC_MAINEXT_REG supported on:                               */
/*       IVT_EP (0x401792E8)                                                  */
/*       IVT_EX (0x401792E8)                                                  */
/*       HSX (0x401992E8)                                                     */
/*       BDX (0x401992E8)                                                     */
/* Register default value on IVT_EP:    0x000000FE                            */
/* Register default value on IVT_EX:    0x000000FE                            */
/* Register default value on HSX:       0x0107B9AC                            */
/* Register default value on BDX:       0x0107B9AC                            */
#define CPGC_SEQ_RASU13_MC_MAINEXT_REG 0x0B0142E8


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-3 Subsequence Logical to Physical Row Address Swizzling Upper -
 * Part-1: This register is used to swizzle the Upper Logial to Physical Row Bits.
 */
typedef union {
  struct {
    UINT32 log2phy_row12_swzl : 5;
    /* log2phy_row12_swzl - Bits[4:0], RW_LB, default = 5'b01100 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 log2phy_row13_swzl : 5;
    /* log2phy_row13_swzl - Bits[9:5], RW_LB, default = 5'b01101 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 log2phy_row14_swzl : 5;
    /* log2phy_row14_swzl - Bits[14:10], RW_LB, default = 5'b01110 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 log2phy_row15_swzl : 5;
    /* log2phy_row15_swzl - Bits[19:15], RW_LB, default = 5'b01111 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 log2phy_row16_swzl : 5;
    /* log2phy_row16_swzl - Bits[24:20], RW_LB, default = 5'b10000 
       Refer to Log2phy_Row11_Swzl for details
     */
    UINT32 rsvd : 7;
    /* rsvd - Bits[31:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_RASU13_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_DMYRDADDR_LO_MC_MAINEXT_REG supported on:                         */
/*       HSX (0x401992EC)                                                     */
/*       BDX (0x401992EC)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_DMYRDADDR_LO_MC_MAINEXT_REG 0x0B0142EC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * Defines what Banks and the Starting Row Address for the Dummy Reads to occur(for 
 * chs 0 and 1).  
 * Dummy Reads are defined as a qualified read request which will generate the 
 * necessary Activates and Precharges but no Read will occur. 
 */
typedef union {
  struct {
    UINT32 bank_mask0 : 16;
    /* bank_mask0 - Bits[15:0], RW_LB, default = 16'b0000000000000000 
       A one hot Mask that defines which Banks will be used in generating the Dummy 
       Reads which create Activates and pre-charges according to the functional 
       behavior of the scheduler. 
       
       A 1 means that a Bank will not be used to issue the Dummy Read to cause a 
       precharge and activate. 
       Bit 0 Bank 0
       Bit 1 Bank 1
       Bit 15 Bank 15
     */
    UINT32 bank_mask1 : 16;
    /* bank_mask1 - Bits[31:16], RW_LB, default = 16'b0000000000000000 
       Refer to Bank_Mask0 for details
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_DMYRDADDR_LO_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_DMYRDSTS0_MC_MAINEXT_REG supported on:                            */
/*       IVT_EP (0x401792F0)                                                  */
/*       IVT_EX (0x401792F0)                                                  */
/*       HSX (0x401992F0)                                                     */
/*       BDX (0x401992F0)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_DMYRDSTS0_MC_MAINEXT_REG 0x0B0142F0


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-0 Susequence Dummy Activate Address Status -
 * Indicates the Dummy Read Current Row Address.
 * Dummy Reads are defined as a qualified read request which will generate the 
 * necessary Activates and Precharges but no Read will occur. 
 */
typedef union {
  struct {
    UINT32 row_curraddr : 17;
    /* row_curraddr - Bits[16:0], RW_V, default = 17'b00000000000000000 
       Dummy_Read_Row_Current_Address defines the current Row Address for the Dummy 
       Reads which is updated while a test is in progress. 
       
       The Dummy_Read_Row_Current_Address will be increment by 1 everytime a Dummy Read 
       is generated. 
       
       One may program Dummy_Read_Row_Current_Address to a desired starting value if a 
       certain initial condition is desired otherwise 
       the Dummy_Read_Row_Current_Address will resume from the current value as no 
       hardware mechanism exists to explicitly 
       clear or modify this register.
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_curraddr : 4;
    /* bank_curraddr - Bits[27:24], RW_V, default = 4'b0000 
       Refer to Row_Curraddr for details
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_DMYRDSTS0_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_DMYRDSTS1_MC_MAINEXT_REG supported on:                            */
/*       IVT_EP (0x401792F4)                                                  */
/*       IVT_EX (0x401792F4)                                                  */
/*       HSX (0x401992F4)                                                     */
/*       BDX (0x401992F4)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_DMYRDSTS1_MC_MAINEXT_REG 0x0B0142F4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-1 Susequence Dummy Activate Address Status -
 * Indicates the Dummy Read Current Row Address.
 * Dummy Reads are defined as a qualified read request which will generate the 
 * necessary Activates and Precharges but no Read will occur. 
 */
typedef union {
  struct {
    UINT32 row_curraddr : 17;
    /* row_curraddr - Bits[16:0], RW_V, default = 17'b00000000000000000 
       Dummy_Read_Row_Current_Address defines the current Row Address for the Dummy 
       Reads which is updated while a test is in progress. 
       
       The Dummy_Read_Row_Current_Address will be increment by 1 everytime a Dummy Read 
       is generated. 
       
       One may program Dummy_Read_Row_Current_Address to a desired starting value if a 
       certain initial condition is desired otherwise 
       the Dummy_Read_Row_Current_Address will resume from the current value as no 
       hardware mechanism exists to explicitly 
       clear or modify this register.
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_curraddr : 4;
    /* bank_curraddr - Bits[27:24], RW_V, default = 4'b0000 
       Refer to Row_Curraddr for details
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_DMYRDSTS1_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_DMYRDSTS2_MC_MAINEXT_REG supported on:                            */
/*       IVT_EP (0x401792F8)                                                  */
/*       IVT_EX (0x401792F8)                                                  */
/*       HSX (0x401992F8)                                                     */
/*       BDX (0x401992F8)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_DMYRDSTS2_MC_MAINEXT_REG 0x0B0142F8


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-2 Susequence Dummy Activate Address Status -
 * Indicates the Dummy Read Current Row Address.
 * Dummy Reads are defined as a qualified read request which will generate the 
 * necessary Activates and Precharges but no Read will occur. 
 */
typedef union {
  struct {
    UINT32 row_curraddr : 17;
    /* row_curraddr - Bits[16:0], RW_V, default = 17'b00000000000000000 
       Dummy_Read_Row_Current_Address defines the current Row Address for the Dummy 
       Reads which is updated while a test is in progress. 
       
       The Dummy_Read_Row_Current_Address will be increment by 1 everytime a Dummy Read 
       is generated. 
       
       One may program Dummy_Read_Row_Current_Address to a desired starting value if a 
       certain initial condition is desired otherwise 
       the Dummy_Read_Row_Current_Address will resume from the current value as no 
       hardware mechanism exists to explicitly 
       clear or modify this register.
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_curraddr : 4;
    /* bank_curraddr - Bits[27:24], RW_V, default = 4'b0000 
       Refer to Row_Curraddr for details
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_DMYRDSTS2_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_DMYRDSTS3_MC_MAINEXT_REG supported on:                            */
/*       IVT_EP (0x401792FC)                                                  */
/*       IVT_EX (0x401792FC)                                                  */
/*       HSX (0x401992FC)                                                     */
/*       BDX (0x401992FC)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_DMYRDSTS3_MC_MAINEXT_REG 0x0B0142FC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC Channel-3 Susequence Dummy Activate Address Status -
 * Indicates the Dummy Read Current Row Address.
 * Dummy Reads are defined as a qualified read request which will generate the 
 * necessary Activates and Precharges but no Read will occur. 
 */
typedef union {
  struct {
    UINT32 row_curraddr : 17;
    /* row_curraddr - Bits[16:0], RW_V, default = 17'b00000000000000000 
       Dummy_Read_Row_Current_Address defines the current Row Address for the Dummy 
       Reads which is updated while a test is in progress. 
       
       The Dummy_Read_Row_Current_Address will be increment by 1 everytime a Dummy Read 
       is generated. 
       
       One may program Dummy_Read_Row_Current_Address to a desired starting value if a 
       certain initial condition is desired otherwise 
       the Dummy_Read_Row_Current_Address will resume from the current value as no 
       hardware mechanism exists to explicitly 
       clear or modify this register.
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_curraddr : 4;
    /* bank_curraddr - Bits[27:24], RW_V, default = 4'b0000 
       Refer to Row_Curraddr for details
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_DMYRDSTS3_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_DMYRDCTL0_MC_MAINEXT_REG supported on:                            */
/*       IVT_EP (0x40179300)                                                  */
/*       IVT_EX (0x40179300)                                                  */
/*       HSX (0x40199300)                                                     */
/*       BDX (0x40199300)                                                     */
/* Register default value:              0x01010100                            */
#define CPGC_SEQ_DMYRDCTL0_MC_MAINEXT_REG 0x0B014300
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Susequence Dummy Activate Control -
 * Defines how often a Dummy Reads will occur.
 * Dummy Reads are defined as a qualified read request which will generate the 
 * necessary Activates and Precharges but no Read will occur. 
 */
typedef union {
  struct {
    UINT32 l_dmyrd_select : 1;
    /* l_dmyrd_select - Bits[0:0], RW_LB, default = 1'b0 
       L_DummyRead_Select indicates whether Dummy Reads will be generated during the 
       first L Dclks after a test is started (entering Loopback.Pattern) 
       0x0 = Dummy Reads must be driven during the initial L counter Dclk delay with 
       minimum spacing as possible 
       0x1=Dummy Reads must be not be driven during the initial L counter Dclk delay
     */
    UINT32 sweep_freq : 1;
    /* sweep_freq - Bits[1:1], RW_LB, default = 1'b0 
       After L_Counter Dclk cycles of Dummy Reads (L_DummyRead_Select =1) or no Dummy 
       Reads (DummyRead_Select = 0) are performed at the beginning of a test (entering 
       Loopback.Pattern) then the following dummy Read periodic pattern is continuously 
       repeated depending on the value of Sweep Frequency. 
     */
    UINT32 rsvd : 6;
    /* rsvd - Bits[7:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 l_counter : 8;
    /* l_counter - Bits[15:8], RW_LB, default = 8'b00000001 
       After L_Counter Dclk cycles of Dummy Reads (L_DummyRead_Select =1) or no Dummy 
       Reads (DummyRead_Select = 0) are performed at the beginning of a test (entering 
       Loopback.Pattern) then the following dummy Read periodic pattern is continuously 
       repeated depending on the value of Sweep Frequency. 
       
       If Sweep_Frequency = 0 then a steady state frequency of Dummy Activiates driven 
       (see Sweep_Frequency for exact behavior) 
       If Sweep_Frequency = 1 then a frequency sweep of Dummy Activates is continuously 
       generated ranging from 2*(M) to 2*(N) driven (see Sweep_Frequency for exact 
       behavior) 
     */
    UINT32 m_counter : 8;
    /* m_counter - Bits[23:16], RW_LB, default = 8'b00000001 
       Refer to L_Counter for details
     */
    UINT32 n_counter : 8;
    /* n_counter - Bits[31:24], RW_LB, default = 8'b00000001 
       Refer to L_Counter for details
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_DMYRDCTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_DMYRDCTL1_MC_MAINEXT_REG supported on:                            */
/*       IVT_EP (0x40179304)                                                  */
/*       IVT_EX (0x40179304)                                                  */
/*       HSX (0x40199304)                                                     */
/*       BDX (0x40199304)                                                     */
/* Register default value:              0x01010100                            */
#define CPGC_SEQ_DMYRDCTL1_MC_MAINEXT_REG 0x0B014304
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Susequence Dummy Activate Control -
 * Defines how often a Dummy Reads will occur.
 * Dummy Reads are defined as a qualified read request which will generate the 
 * necessary Activates and Precharges but no Read will occur. 
 */
typedef union {
  struct {
    UINT32 l_dmyrd_select : 1;
    /* l_dmyrd_select - Bits[0:0], RW_LB, default = 1'b0 
       L_DummyRead_Select indicates whether Dummy Reads will be generated during the 
       first L Dclks after a test is started (entering Loopback.Pattern) 
       0x0 = Dummy Reads must be driven during the initial L counter Dclk delay with 
       minimum spacing as possible 
       0x1=Dummy Reads must be not be driven during the initial L counter Dclk delay
     */
    UINT32 sweep_freq : 1;
    /* sweep_freq - Bits[1:1], RW_LB, default = 1'b0 
       After L_Counter Dclk cycles of Dummy Reads (L_DummyRead_Select =1) or no Dummy 
       Reads (DummyRead_Select = 0) are performed at the beginning of a test (entering 
       Loopback.Pattern) then the following dummy Read periodic pattern is continuously 
       repeated depending on the value of Sweep Frequency. 
     */
    UINT32 rsvd : 6;
    /* rsvd - Bits[7:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 l_counter : 8;
    /* l_counter - Bits[15:8], RW_LB, default = 8'b00000001 
       After L_Counter Dclk cycles of Dummy Reads (L_DummyRead_Select =1) or no Dummy 
       Reads (DummyRead_Select = 0) are performed at the beginning of a test (entering 
       Loopback.Pattern) then the following dummy Read periodic pattern is continuously 
       repeated depending on the value of Sweep Frequency. 
       
       If Sweep_Frequency = 0 then a steady state frequency of Dummy Activiates driven 
       (see Sweep_Frequency for exact behavior) 
       If Sweep_Frequency = 1 then a frequency sweep of Dummy Activates is continuously 
       generated ranging from 2*(M) to 2*(N) driven (see Sweep_Frequency for exact 
       behavior) 
     */
    UINT32 m_counter : 8;
    /* m_counter - Bits[23:16], RW_LB, default = 8'b00000001 
       Refer to L_Counter for details
     */
    UINT32 n_counter : 8;
    /* n_counter - Bits[31:24], RW_LB, default = 8'b00000001 
       Refer to L_Counter for details
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_DMYRDCTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_DMYRDCTL2_MC_MAINEXT_REG supported on:                            */
/*       IVT_EP (0x40179308)                                                  */
/*       IVT_EX (0x40179308)                                                  */
/*       HSX (0x40199308)                                                     */
/*       BDX (0x40199308)                                                     */
/* Register default value:              0x01010100                            */
#define CPGC_SEQ_DMYRDCTL2_MC_MAINEXT_REG 0x0B014308
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Susequence Dummy Activate Control -
 * Defines how often a Dummy Reads will occur.
 * Dummy Reads are defined as a qualified read request which will generate the 
 * necessary Activates and Precharges but no Read will occur. 
 */
typedef union {
  struct {
    UINT32 l_dmyrd_select : 1;
    /* l_dmyrd_select - Bits[0:0], RW_LB, default = 1'b0 
       L_DummyRead_Select indicates whether Dummy Reads will be generated during the 
       first L Dclks after a test is started (entering Loopback.Pattern) 
       0x0 = Dummy Reads must be driven during the initial L counter Dclk delay with 
       minimum spacing as possible 
       0x1=Dummy Reads must be not be driven during the initial L counter Dclk delay
     */
    UINT32 sweep_freq : 1;
    /* sweep_freq - Bits[1:1], RW_LB, default = 1'b0 
       After L_Counter Dclk cycles of Dummy Reads (L_DummyRead_Select =1) or no Dummy 
       Reads (DummyRead_Select = 0) are performed at the beginning of a test (entering 
       Loopback.Pattern) then the following dummy Read periodic pattern is continuously 
       repeated depending on the value of Sweep Frequency. 
     */
    UINT32 rsvd : 6;
    /* rsvd - Bits[7:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 l_counter : 8;
    /* l_counter - Bits[15:8], RW_LB, default = 8'b00000001 
       After L_Counter Dclk cycles of Dummy Reads (L_DummyRead_Select =1) or no Dummy 
       Reads (DummyRead_Select = 0) are performed at the beginning of a test (entering 
       Loopback.Pattern) then the following dummy Read periodic pattern is continuously 
       repeated depending on the value of Sweep Frequency. 
       
       If Sweep_Frequency = 0 then a steady state frequency of Dummy Activiates driven 
       (see Sweep_Frequency for exact behavior) 
       If Sweep_Frequency = 1 then a frequency sweep of Dummy Activates is continuously 
       generated ranging from 2*(M) to 2*(N) driven (see Sweep_Frequency for exact 
       behavior) 
     */
    UINT32 m_counter : 8;
    /* m_counter - Bits[23:16], RW_LB, default = 8'b00000001 
       Refer to L_Counter for details
     */
    UINT32 n_counter : 8;
    /* n_counter - Bits[31:24], RW_LB, default = 8'b00000001 
       Refer to L_Counter for details
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_DMYRDCTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_DMYRDCTL3_MC_MAINEXT_REG supported on:                            */
/*       IVT_EP (0x4017930C)                                                  */
/*       IVT_EX (0x4017930C)                                                  */
/*       HSX (0x4019930C)                                                     */
/*       BDX (0x4019930C)                                                     */
/* Register default value:              0x01010100                            */
#define CPGC_SEQ_DMYRDCTL3_MC_MAINEXT_REG 0x0B01430C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Susequence Dummy Activate Control -
 * Defines how often a Dummy Reads will occur.
 * Dummy Reads are defined as a qualified read request which will generate the 
 * necessary Activates and Precharges but no Read will occur. 
 */
typedef union {
  struct {
    UINT32 l_dmyrd_select : 1;
    /* l_dmyrd_select - Bits[0:0], RW_LB, default = 1'b0 
       L_DummyRead_Select indicates whether Dummy Reads will be generated during the 
       first L Dclks after a test is started (entering Loopback.Pattern) 
       0x0 = Dummy Reads must be driven during the initial L counter Dclk delay with 
       minimum spacing as possible 
       0x1=Dummy Reads must be not be driven during the initial L counter Dclk delay
     */
    UINT32 sweep_freq : 1;
    /* sweep_freq - Bits[1:1], RW_LB, default = 1'b0 
       After L_Counter Dclk cycles of Dummy Reads (L_DummyRead_Select =1) or no Dummy 
       Reads (DummyRead_Select = 0) are performed at the beginning of a test (entering 
       Loopback.Pattern) then the following dummy Read periodic pattern is continuously 
       repeated depending on the value of Sweep Frequency. 
     */
    UINT32 rsvd : 6;
    /* rsvd - Bits[7:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 l_counter : 8;
    /* l_counter - Bits[15:8], RW_LB, default = 8'b00000001 
       After L_Counter Dclk cycles of Dummy Reads (L_DummyRead_Select =1) or no Dummy 
       Reads (DummyRead_Select = 0) are performed at the beginning of a test (entering 
       Loopback.Pattern) then the following dummy Read periodic pattern is continuously 
       repeated depending on the value of Sweep Frequency. 
       
       If Sweep_Frequency = 0 then a steady state frequency of Dummy Activiates driven 
       (see Sweep_Frequency for exact behavior) 
       If Sweep_Frequency = 1 then a frequency sweep of Dummy Activates is continuously 
       generated ranging from 2*(M) to 2*(N) driven (see Sweep_Frequency for exact 
       behavior) 
     */
    UINT32 m_counter : 8;
    /* m_counter - Bits[23:16], RW_LB, default = 8'b00000001 
       Refer to L_Counter for details
     */
    UINT32 n_counter : 8;
    /* n_counter - Bits[31:24], RW_LB, default = 8'b00000001 
       Refer to L_Counter for details
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_DMYRDCTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ0_CTL0_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179310)                                                  */
/*       IVT_EX (0x40179310)                                                  */
/*       HSX (0x40199310)                                                     */
/*       BDX (0x40199310)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ0_CTL0_MC_MAINEXT_REG 0x0B014310
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-0 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
       
       The interpretation of Number_Of_Cachelines is based on 
       Number_Of_Cachelines_Scale. 
       
       If Number_Of_Cachelines_Scale is set to 1 then Number_Of_Cachelines operates as 
       a linear counter with one notable exception: 
       
       Setting Number_Of_Cachelines_Scale to 1 and Number_of_Cachelines to 0 will 
       result in an infinite # of cachelines being transmitted. 
       
       If Number_Of_Cachelines_Scale is set to 0 then Number_Of_Cachelines operates as 
       an exponential counter with a value equal to (2^ Number_Of_Cachelines_Scale). 
       
       Only bits 0:4 are supported Number_Of_Cachelines_Scale is set to 0 (exponential 
       counter mode). 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ0_CTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ0_OFFSETCTL0_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x40179314)                                                  */
/*       IVT_EX (0x40179314)                                                  */
/*       HSX (0x40199314)                                                     */
/*       BDX (0x40199314)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ0_OFFSETCTL0_MC_MAINEXT_REG 0x0B014314
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-0 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offadd_updrate determines the rate that the Current Offset Address is updated.
       
       When the Subsequence_Type is set to an offset operation then any read or write 
       cacheline operation of any type (Base or Offset) counts toward determining 
       whether the Offadd_updrate is reached. 
       Based on Row_inc_enb, Column_Increment_Enable, Row_inc_order, and 
       Column_Incrment_Order a series of 1D Current Address sweeps are performed 
       relative to the Current Base Address at the rate dictated by Offadd_updrate. 
       Each 1D offset operation ends based on a finite # Number_Of_Cachelines and/or a 
       Stop_OnWrap_Trigger_Event (using the coloff_wrptrig_enb and/or 
       rowoff_wrptrig_enb). 
       1D Offset Address incrementing rules that apply to calculating the Current 
       Offset Address: 
       1) At the beginning of a Read_Offset or Write_Offset before any Read or Write 
       operation are allowed to occur the starting SEQ_OFFSET_ADDR_CURRENT is 
       calculated by adding the current increment/decrement value (depending on the Row 
       and Column Increment_Enable and Increment_Order) to the row and/or columns 
       address field(s) in SEQ_BASE_ADDR_CURRENT. 
       2) After Offadd_updrate cachelines are read or written while in the same 
       subsequence the SEQ_OFFSET_ADDR_CURRENT is adding the current 
       increment/decrement value (depending on the Row and Column Increment_Enable and 
       Increment_Order) to the row and/or columns address field(s). If Offadd_updrate = 
       0 then SEQ_OFFSET_ADDR_CURRENT will never increment/decrement in value beyond 
       the initial value set in rule 1). 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then base_subseq_rptrate 
       determines how often the base_subseq_type operation occurs for 1 cacheline. 
       Any Offset read or write cacheline operation counts toward determining whether 
       the base_subseq_rptrate is reached. 
       The actual Base_Subsequnce_Type does not increment base_subseq_rptrate.
       0 No Base Subsequence cacheline operations will occur during the Offset_Read or 
       Offset_Write. 
       1 Reserved value.
       2 Repeat the Base_subseq_type cacheline operation after every cacheline 
       operation. 
       ...
       31 Repeat the Base_subseq_type cacheline operation after 30 Offset cacheline 
       operations. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       If coloff_wrptrig_enb is set to 1 and (Subsequence_Type is set to either 
       Offset_Read or Offset_Write) then: 
       If SEQ_OFFSET_ADDR_CURRENT.Column_Address = (SEQ_BASE_ADDR_WRAP.Column_Address 
       or SEQ_BASE_ADDR_START.Column_Address) then any further request to modify 
       (increment or decrement) the SEQ_OFFSET_ADDR_CURRENT.Column_Address immediately 
       causes a Column Offset Wrap Trigger to occur where the current incrementing 
       Column and Row offset sweep is considered complete. 
       Else, If coloff_wrptrig_enb is set to 0 and (Subsequence_Type is set to either 
       Offset_Read or Offset_Write) then: 
       If SEQ_OFFSET_ADDR_CURRENT.Column_Address = SEQ_BASE_ADDR_WRAP.Column_Address 
       then any further request to modify (increment or decrement) the 
       SEQ_OFFSET_ADDR_CURRENT.Column_Address causes 
       SEQ_OFFSET_ADDR_CURRENT.Column_Address to be set equal to 
       SEQ_BASE_ADDR_START.Column_Address. 
       Else, If SEQ_OFFSET_ADDR_CURRENT.Column_Address = 
       SEQ_BASE_ADDR_START.Column_Address then any further request to modify (increment 
       or decrement) the SEQ_OFFSET_ADDR_CURRENT.Column_Address causes 
       SEQ_OFFSET_ADDR_CURRENT.Column_Address to be set equal to 
       SEQ_BASE_ADDR_WRAP.Column_Address. 
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       If rowoff_wrptrig_enb is set to 1 and (Subsequence_Type is set to either 
       Offset_Read or Offset_Write) then: 
       If SEQ_OFFSET_ADDR_CURRENT.Row_Address = (SEQ_BASE_ADDR_WRAP.Row_Address or 
       SEQ_BASE_ADDR_START.Row_Address) then any further request to modify (increment 
       or decrement) the SEQ_OFFSET_ADDR_CURRENT.Row_Address immediately causes a Row 
       Offset Wrap Trigger to occur where the current incrementing row and column 
       offset sweep is considered complete. 
       Else, If rowoff_wrptrig_enb is set to 0 and (Subsequence_Type is set to either 
       Offset_Read or Offset_Write) then: 
       If SEQ_OFFSET_ADDR_CURRENT.Row_Address = SEQ_BASE_ADDR_WRAP.Row_Address then any 
       further request to modify (increment or decrement) the 
       SEQ_OFFSET_ADDR_CURRENT.Row_Address causes SEQ_OFFSET_ADDR_CURRENT.Row_Address 
       to be set equal to SEQ_BASE_ADDR_START.Row_Address. 
       Else, If SEQ_OFFSET_ADDR_CURRENT.Row_Address = SEQ_BASE_ADDR_START.Row_Address 
       then any further request to modify (increment or decrement) the 
       SEQ_OFFSET_ADDR_CURRENT.Row_Address causes SEQ_OFFSET_ADDR_CURRENT.Row_Address 
       to be set equal to SEQ_BASE_ADDR_WRAP.Row_Address. 
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Every time the base_subseq_rptrate is reached a Read or Write is performed for 
       one command based on the following fields: 
       base_subseq_type indicates whether a Read (base_subseq_type =0) or Write 
       (base_subseq_type =1) operation will occur at the Current Base Address 
       (SEQ_OFFSET_ADDR_CURRENT). 
       base_inv_dataecc indicates whether the Read or Write (base_subseq_type) 
       operation at the Current Base Address (SEQ_OFFSET_ADDR_CURRENT) is inverted 
       (base_inv_dataecc=1) or not (base_inv_dataecc=0) 
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to bit 29 in this CSR.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to bit 29 in this CSR.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to bit 29 in this CSR.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       The following is a list of all possible 1D offset sweeps:
       1) Row Sweep - col_inc_enb= 0 and row_inc_enb = 1 and col_inc_order = Do not 
       Care and row_inc_order = 0. 
       a. 1st offset sweep - Offset Address Row Increment = +1
       b. 2nd offset sweep- Offset Address Row Decrement = -1
       2) Column Sweep - col_inc_enb= 1 and row_inc_enb = 0 and col_inc_order = 0 and 
       row_inc_order = Don not Care. 
       a. 1st offset sweep - Offset Address Columns Increment = +1
       b. 2nd offset sweep- Offset Address Column Decrement = -1
       3) Row and then Column Sweep - col_inc_enb= 1 and row_inc_enb = 1 and 
       col_inc_order = 1 and row_inc_order = 0. 
       a. 1st offset sweep - Offset Address Row Increment = +1
       b. 2nd offset sweep - Offset Address Row Decrement = -1
       c. 3rd offset sweep - Offset Address Columns Increment = +1
       d. 4th offset sweep - Offset Address Column Decrement = -1
       4) Column and then Row Sweep - col_inc_enb= 1 and row_inc_enb = 1 and 
       col_inc_order = 0 and row_inc_order = 1. 
       a. 1st offset sweep - Offset Address Column Increment = +1
       b. 2nd offset sweep - Offset Address Column Decrement = -1
       c. 3rd offset sweep - Offset Address Row Increment = +1
       d. 4th offset sweep - Offset Address Row Decrement = -1
       5) Row and Column Diagonal Sweep - col_inc_enb= 1 and row_inc_enb = 1 and 
       col_inc_order = 0 and row_inc_order = 0. 
       a. 1st offset sweep - Use both Address Row Increment = +1 and Address Column 
       Increment = +1. 
       b. 2nd offset sweep - Use both Address Row Increment = +1 and Address Column 
       Decrement = -1. 
       c. 3rd offset sweep - Use both Address Row Decrement = -1 and Address Column 
       Increment = +1. 
       d. 4th offset sweep - Use both Address Row Decrement = -1 and Address Column 
       Decrement = -1. 
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Every time the base_subseq_rptrate is reached a Base Read or Base Write is 
       performed for one command at the Current Base Address based on the following 
       fields. 
       base_subseq_type indicates whether a Read (base_subseq_type = 0) or Write 
       (base_subseq_type = 1) operation will occur at the Current Base Address 
       (SEQ_BASE_ADDR_CURRENT). 
       base_inv_dataecc indicates whether the Read or Write (base_subseq_type) 
       operation at the Current Base Address (SEQ_BASE_ADDR_CURRENT) is inverted 
       (base_inv_dataecc=1) or not (base_inv_dataecc=0). 
       A base_inv_dataecc is always XOR'd after the calculation of the Address_Invert 
       to create the final polarity of the current write or read command which must 
       then be XOR'd to the output of the PAT_WDB_DATA_INV.Data_Inv_or_DC_Enable and 
       PAT_WDB_INV. ECC_Inv_or_DC_Enable. 
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ0_OFFSETCTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ1_CTL0_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179318)                                                  */
/*       IVT_EX (0x40179318)                                                  */
/*       HSX (0x40199318)                                                     */
/*       BDX (0x40199318)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ1_CTL0_MC_MAINEXT_REG 0x0B014318
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-1 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE ). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ1_CTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ1_OFFSETCTL0_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x4017931C)                                                  */
/*       IVT_EX (0x4017931C)                                                  */
/*       HSX (0x4019931C)                                                     */
/*       BDX (0x4019931C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ1_OFFSETCTL0_MC_MAINEXT_REG 0x0B01431C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-1 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ1_OFFSETCTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ2_CTL0_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179320)                                                  */
/*       IVT_EX (0x40179320)                                                  */
/*       HSX (0x40199320)                                                     */
/*       BDX (0x40199320)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ2_CTL0_MC_MAINEXT_REG 0x0B014320
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-2 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE ). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ2_CTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ2_OFFSETCTL0_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x40179324)                                                  */
/*       IVT_EX (0x40179324)                                                  */
/*       HSX (0x40199324)                                                     */
/*       BDX (0x40199324)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ2_OFFSETCTL0_MC_MAINEXT_REG 0x0B014324
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-2 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ2_OFFSETCTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ3_CTL0_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179328)                                                  */
/*       IVT_EX (0x40179328)                                                  */
/*       HSX (0x40199328)                                                     */
/*       BDX (0x40199328)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ3_CTL0_MC_MAINEXT_REG 0x0B014328
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-3 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ3_CTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ3_OFFSETCTL0_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x4017932C)                                                  */
/*       IVT_EX (0x4017932C)                                                  */
/*       HSX (0x4019932C)                                                     */
/*       BDX (0x4019932C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ3_OFFSETCTL0_MC_MAINEXT_REG 0x0B01432C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-3 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ3_OFFSETCTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ4_CTL0_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179330)                                                  */
/*       IVT_EX (0x40179330)                                                  */
/*       HSX (0x40199330)                                                     */
/*       BDX (0x40199330)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ4_CTL0_MC_MAINEXT_REG 0x0B014330
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-4 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ4_CTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ4_OFFSETCTL0_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x40179334)                                                  */
/*       IVT_EX (0x40179334)                                                  */
/*       HSX (0x40199334)                                                     */
/*       BDX (0x40199334)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ4_OFFSETCTL0_MC_MAINEXT_REG 0x0B014334
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-4 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ4_OFFSETCTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ5_CTL0_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179338)                                                  */
/*       IVT_EX (0x40179338)                                                  */
/*       HSX (0x40199338)                                                     */
/*       BDX (0x40199338)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ5_CTL0_MC_MAINEXT_REG 0x0B014338
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-5 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ5_CTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ5_OFFSETCTL0_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x4017933C)                                                  */
/*       IVT_EX (0x4017933C)                                                  */
/*       HSX (0x4019933C)                                                     */
/*       BDX (0x4019933C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ5_OFFSETCTL0_MC_MAINEXT_REG 0x0B01433C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-5 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ5_OFFSETCTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ6_CTL0_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179340)                                                  */
/*       IVT_EX (0x40179340)                                                  */
/*       HSX (0x40199340)                                                     */
/*       BDX (0x40199340)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ6_CTL0_MC_MAINEXT_REG 0x0B014340
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-6 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ6_CTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ6_OFFSETCTL0_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x40179344)                                                  */
/*       IVT_EX (0x40179344)                                                  */
/*       HSX (0x40199344)                                                     */
/*       BDX (0x40199344)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ6_OFFSETCTL0_MC_MAINEXT_REG 0x0B014344
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-6 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ6_OFFSETCTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ7_CTL0_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179348)                                                  */
/*       IVT_EX (0x40179348)                                                  */
/*       HSX (0x40199348)                                                     */
/*       BDX (0x40199348)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ7_CTL0_MC_MAINEXT_REG 0x0B014348
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-7 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ7_CTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ7_OFFSETCTL0_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x4017934C)                                                  */
/*       IVT_EX (0x4017934C)                                                  */
/*       HSX (0x4019934C)                                                     */
/*       BDX (0x4019934C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ7_OFFSETCTL0_MC_MAINEXT_REG 0x0B01434C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-7 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ7_OFFSETCTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ8_CTL0_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179350)                                                  */
/*       IVT_EX (0x40179350)                                                  */
/*       HSX (0x40199350)                                                     */
/*       BDX (0x40199350)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ8_CTL0_MC_MAINEXT_REG 0x0B014350
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-8 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ8_CTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ8_OFFSETCTL0_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x40179354)                                                  */
/*       IVT_EX (0x40179354)                                                  */
/*       HSX (0x40199354)                                                     */
/*       BDX (0x40199354)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ8_OFFSETCTL0_MC_MAINEXT_REG 0x0B014354
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-8 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ8_OFFSETCTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ9_CTL0_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179358)                                                  */
/*       IVT_EX (0x40179358)                                                  */
/*       HSX (0x40199358)                                                     */
/*       BDX (0x40199358)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ9_CTL0_MC_MAINEXT_REG 0x0B014358
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-9 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ9_CTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ9_OFFSETCTL0_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x4017935C)                                                  */
/*       IVT_EX (0x4017935C)                                                  */
/*       HSX (0x4019935C)                                                     */
/*       BDX (0x4019935C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ9_OFFSETCTL0_MC_MAINEXT_REG 0x0B01435C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence-9 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ9_OFFSETCTL0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ0_CTL1_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179360)                                                  */
/*       IVT_EX (0x40179360)                                                  */
/*       HSX (0x40199360)                                                     */
/*       BDX (0x40199360)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ0_CTL1_MC_MAINEXT_REG 0x0B014360
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-0 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ0_CTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ0_OFFSETCTL1_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x40179364)                                                  */
/*       IVT_EX (0x40179364)                                                  */
/*       HSX (0x40199364)                                                     */
/*       BDX (0x40199364)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ0_OFFSETCTL1_MC_MAINEXT_REG 0x0B014364
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-0 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ0_OFFSETCTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ1_CTL1_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179368)                                                  */
/*       IVT_EX (0x40179368)                                                  */
/*       HSX (0x40199368)                                                     */
/*       BDX (0x40199368)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ1_CTL1_MC_MAINEXT_REG 0x0B014368
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-1 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ1_CTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ1_OFFSETCTL1_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x4017936C)                                                  */
/*       IVT_EX (0x4017936C)                                                  */
/*       HSX (0x4019936C)                                                     */
/*       BDX (0x4019936C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ1_OFFSETCTL1_MC_MAINEXT_REG 0x0B01436C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-1 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ1_OFFSETCTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ2_CTL1_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179370)                                                  */
/*       IVT_EX (0x40179370)                                                  */
/*       HSX (0x40199370)                                                     */
/*       BDX (0x40199370)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ2_CTL1_MC_MAINEXT_REG 0x0B014370
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-2 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ2_CTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ2_OFFSETCTL1_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x40179374)                                                  */
/*       IVT_EX (0x40179374)                                                  */
/*       HSX (0x40199374)                                                     */
/*       BDX (0x40199374)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ2_OFFSETCTL1_MC_MAINEXT_REG 0x0B014374
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-2 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ2_OFFSETCTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ3_CTL1_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179378)                                                  */
/*       IVT_EX (0x40179378)                                                  */
/*       HSX (0x40199378)                                                     */
/*       BDX (0x40199378)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ3_CTL1_MC_MAINEXT_REG 0x0B014378
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-3 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ3_CTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ3_OFFSETCTL1_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x4017937C)                                                  */
/*       IVT_EX (0x4017937C)                                                  */
/*       HSX (0x4019937C)                                                     */
/*       BDX (0x4019937C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ3_OFFSETCTL1_MC_MAINEXT_REG 0x0B01437C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-3 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ3_OFFSETCTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ4_CTL1_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179380)                                                  */
/*       IVT_EX (0x40179380)                                                  */
/*       HSX (0x40199380)                                                     */
/*       BDX (0x40199380)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ4_CTL1_MC_MAINEXT_REG 0x0B014380
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-4 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ4_CTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ4_OFFSETCTL1_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x40179384)                                                  */
/*       IVT_EX (0x40179384)                                                  */
/*       HSX (0x40199384)                                                     */
/*       BDX (0x40199384)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ4_OFFSETCTL1_MC_MAINEXT_REG 0x0B014384
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-4 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ4_OFFSETCTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ5_CTL1_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179388)                                                  */
/*       IVT_EX (0x40179388)                                                  */
/*       HSX (0x40199388)                                                     */
/*       BDX (0x40199388)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ5_CTL1_MC_MAINEXT_REG 0x0B014388
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-5 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ5_CTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ5_OFFSETCTL1_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x4017938C)                                                  */
/*       IVT_EX (0x4017938C)                                                  */
/*       HSX (0x4019938C)                                                     */
/*       BDX (0x4019938C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ5_OFFSETCTL1_MC_MAINEXT_REG 0x0B01438C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-5 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ5_OFFSETCTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ6_CTL1_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179390)                                                  */
/*       IVT_EX (0x40179390)                                                  */
/*       HSX (0x40199390)                                                     */
/*       BDX (0x40199390)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ6_CTL1_MC_MAINEXT_REG 0x0B014390
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-6 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ6_CTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ6_OFFSETCTL1_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x40179394)                                                  */
/*       IVT_EX (0x40179394)                                                  */
/*       HSX (0x40199394)                                                     */
/*       BDX (0x40199394)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ6_OFFSETCTL1_MC_MAINEXT_REG 0x0B014394
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-6 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often 
       the Base_Subsequence_Type operation occurs for 1 cacheline.
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ6_OFFSETCTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ7_CTL1_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179398)                                                  */
/*       IVT_EX (0x40179398)                                                  */
/*       HSX (0x40199398)                                                     */
/*       BDX (0x40199398)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ7_CTL1_MC_MAINEXT_REG 0x0B014398
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-7 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ7_CTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ7_OFFSETCTL1_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x4017939C)                                                  */
/*       IVT_EX (0x4017939C)                                                  */
/*       HSX (0x4019939C)                                                     */
/*       BDX (0x4019939C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ7_OFFSETCTL1_MC_MAINEXT_REG 0x0B01439C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-7 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ7_OFFSETCTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ8_CTL1_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x401793A0)                                                  */
/*       IVT_EX (0x401793A0)                                                  */
/*       HSX (0x401993A0)                                                     */
/*       BDX (0x401993A0)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ8_CTL1_MC_MAINEXT_REG 0x0B0143A0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-8 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ8_CTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ8_OFFSETCTL1_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x401793A4)                                                  */
/*       IVT_EX (0x401793A4)                                                  */
/*       HSX (0x401993A4)                                                     */
/*       BDX (0x401993A4)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ8_OFFSETCTL1_MC_MAINEXT_REG 0x0B0143A4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-8 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ8_OFFSETCTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ9_CTL1_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x401793A8)                                                  */
/*       IVT_EX (0x401793A8)                                                  */
/*       HSX (0x401993A8)                                                     */
/*       BDX (0x401993A8)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ9_CTL1_MC_MAINEXT_REG 0x0B0143A8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-9 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ9_CTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ9_OFFSETCTL1_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x401793AC)                                                  */
/*       IVT_EX (0x401793AC)                                                  */
/*       HSX (0x401993AC)                                                     */
/*       BDX (0x401993AC)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ9_OFFSETCTL1_MC_MAINEXT_REG 0x0B0143AC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence-9 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ9_OFFSETCTL1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ0_CTL2_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x401793B0)                                                  */
/*       IVT_EX (0x401793B0)                                                  */
/*       HSX (0x401993B0)                                                     */
/*       BDX (0x401993B0)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ0_CTL2_MC_MAINEXT_REG 0x0B0143B0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-0 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ0_CTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ0_OFFSETCTL2_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x401793B4)                                                  */
/*       IVT_EX (0x401793B4)                                                  */
/*       HSX (0x401993B4)                                                     */
/*       BDX (0x401993B4)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ0_OFFSETCTL2_MC_MAINEXT_REG 0x0B0143B4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-0 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ0_OFFSETCTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ1_CTL2_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x401793B8)                                                  */
/*       IVT_EX (0x401793B8)                                                  */
/*       HSX (0x401993B8)                                                     */
/*       BDX (0x401993B8)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ1_CTL2_MC_MAINEXT_REG 0x0B0143B8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-1 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ1_CTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ1_OFFSETCTL2_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x401793BC)                                                  */
/*       IVT_EX (0x401793BC)                                                  */
/*       HSX (0x401993BC)                                                     */
/*       BDX (0x401993BC)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ1_OFFSETCTL2_MC_MAINEXT_REG 0x0B0143BC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-1 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ1_OFFSETCTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ2_CTL2_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x401793C0)                                                  */
/*       IVT_EX (0x401793C0)                                                  */
/*       HSX (0x401993C0)                                                     */
/*       BDX (0x401993C0)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ2_CTL2_MC_MAINEXT_REG 0x0B0143C0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-2 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ2_CTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ2_OFFSETCTL2_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x401793C4)                                                  */
/*       IVT_EX (0x401793C4)                                                  */
/*       HSX (0x401993C4)                                                     */
/*       BDX (0x401993C4)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ2_OFFSETCTL2_MC_MAINEXT_REG 0x0B0143C4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-2 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ2_OFFSETCTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ3_CTL2_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x401793C8)                                                  */
/*       IVT_EX (0x401793C8)                                                  */
/*       HSX (0x401993C8)                                                     */
/*       BDX (0x401993C8)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ3_CTL2_MC_MAINEXT_REG 0x0B0143C8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-3 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ3_CTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ3_OFFSETCTL2_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x401793CC)                                                  */
/*       IVT_EX (0x401793CC)                                                  */
/*       HSX (0x401993CC)                                                     */
/*       BDX (0x401993CC)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ3_OFFSETCTL2_MC_MAINEXT_REG 0x0B0143CC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-3 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ3_OFFSETCTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ4_CTL2_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x401793D0)                                                  */
/*       IVT_EX (0x401793D0)                                                  */
/*       HSX (0x401993D0)                                                     */
/*       BDX (0x401993D0)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ4_CTL2_MC_MAINEXT_REG 0x0B0143D0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-4 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ4_CTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ4_OFFSETCTL2_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x401793D4)                                                  */
/*       IVT_EX (0x401793D4)                                                  */
/*       HSX (0x401993D4)                                                     */
/*       BDX (0x401993D4)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ4_OFFSETCTL2_MC_MAINEXT_REG 0x0B0143D4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-4 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ4_OFFSETCTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ5_CTL2_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x401793D8)                                                  */
/*       IVT_EX (0x401793D8)                                                  */
/*       HSX (0x401993D8)                                                     */
/*       BDX (0x401993D8)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ5_CTL2_MC_MAINEXT_REG 0x0B0143D8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-5 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ5_CTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ5_OFFSETCTL2_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x401793DC)                                                  */
/*       IVT_EX (0x401793DC)                                                  */
/*       HSX (0x401993DC)                                                     */
/*       BDX (0x401993DC)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ5_OFFSETCTL2_MC_MAINEXT_REG 0x0B0143DC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-5 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ5_OFFSETCTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ6_CTL2_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x401793E0)                                                  */
/*       IVT_EX (0x401793E0)                                                  */
/*       HSX (0x401993E0)                                                     */
/*       BDX (0x401993E0)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ6_CTL2_MC_MAINEXT_REG 0x0B0143E0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-6 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ6_CTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ6_OFFSETCTL2_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x401793E4)                                                  */
/*       IVT_EX (0x401793E4)                                                  */
/*       HSX (0x401993E4)                                                     */
/*       BDX (0x401993E4)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ6_OFFSETCTL2_MC_MAINEXT_REG 0x0B0143E4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-6 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ6_OFFSETCTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ7_CTL2_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x401793E8)                                                  */
/*       IVT_EX (0x401793E8)                                                  */
/*       HSX (0x401993E8)                                                     */
/*       BDX (0x401993E8)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ7_CTL2_MC_MAINEXT_REG 0x0B0143E8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-7 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ7_CTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ7_OFFSETCTL2_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x401793EC)                                                  */
/*       IVT_EX (0x401793EC)                                                  */
/*       HSX (0x401993EC)                                                     */
/*       BDX (0x401993EC)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ7_OFFSETCTL2_MC_MAINEXT_REG 0x0B0143EC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-7 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ7_OFFSETCTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ8_CTL2_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x401793F0)                                                  */
/*       IVT_EX (0x401793F0)                                                  */
/*       HSX (0x401993F0)                                                     */
/*       BDX (0x401993F0)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ8_CTL2_MC_MAINEXT_REG 0x0B0143F0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-8 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ8_CTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ8_OFFSETCTL2_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x401793F4)                                                  */
/*       IVT_EX (0x401793F4)                                                  */
/*       HSX (0x401993F4)                                                     */
/*       BDX (0x401993F4)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ8_OFFSETCTL2_MC_MAINEXT_REG 0x0B0143F4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-8 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ8_OFFSETCTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ9_CTL2_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x401793F8)                                                  */
/*       IVT_EX (0x401793F8)                                                  */
/*       HSX (0x401993F8)                                                     */
/*       BDX (0x401993F8)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ9_CTL2_MC_MAINEXT_REG 0x0B0143F8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-9 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ9_CTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ9_OFFSETCTL2_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x401793FC)                                                  */
/*       IVT_EX (0x401793FC)                                                  */
/*       HSX (0x401993FC)                                                     */
/*       BDX (0x401993FC)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ9_OFFSETCTL2_MC_MAINEXT_REG 0x0B0143FC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence-9 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ9_OFFSETCTL2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ0_CTL3_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179400)                                                  */
/*       IVT_EX (0x40179400)                                                  */
/*       HSX (0x40199400)                                                     */
/*       BDX (0x40199400)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ0_CTL3_MC_MAINEXT_REG 0x0B014400
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-0 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ0_CTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ0_OFFSETCTL3_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x40179404)                                                  */
/*       IVT_EX (0x40179404)                                                  */
/*       HSX (0x40199404)                                                     */
/*       BDX (0x40199404)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ0_OFFSETCTL3_MC_MAINEXT_REG 0x0B014404
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-0 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ0_OFFSETCTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ1_CTL3_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179408)                                                  */
/*       IVT_EX (0x40179408)                                                  */
/*       HSX (0x40199408)                                                     */
/*       BDX (0x40199408)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ1_CTL3_MC_MAINEXT_REG 0x0B014408
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-1 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ1_CTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ1_OFFSETCTL3_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x4017940C)                                                  */
/*       IVT_EX (0x4017940C)                                                  */
/*       HSX (0x4019940C)                                                     */
/*       BDX (0x4019940C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ1_OFFSETCTL3_MC_MAINEXT_REG 0x0B01440C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-1 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ1_OFFSETCTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ2_CTL3_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179410)                                                  */
/*       IVT_EX (0x40179410)                                                  */
/*       HSX (0x40199410)                                                     */
/*       BDX (0x40199410)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ2_CTL3_MC_MAINEXT_REG 0x0B014410
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-2 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ2_CTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ2_OFFSETCTL3_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x40179414)                                                  */
/*       IVT_EX (0x40179414)                                                  */
/*       HSX (0x40199414)                                                     */
/*       BDX (0x40199414)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ2_OFFSETCTL3_MC_MAINEXT_REG 0x0B014414
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-2 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ2_OFFSETCTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ3_CTL3_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179418)                                                  */
/*       IVT_EX (0x40179418)                                                  */
/*       HSX (0x40199418)                                                     */
/*       BDX (0x40199418)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ3_CTL3_MC_MAINEXT_REG 0x0B014418
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-3 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ3_CTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ3_OFFSETCTL3_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x4017941C)                                                  */
/*       IVT_EX (0x4017941C)                                                  */
/*       HSX (0x4019941C)                                                     */
/*       BDX (0x4019941C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ3_OFFSETCTL3_MC_MAINEXT_REG 0x0B01441C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-3 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ3_OFFSETCTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ4_CTL3_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179420)                                                  */
/*       IVT_EX (0x40179420)                                                  */
/*       HSX (0x40199420)                                                     */
/*       BDX (0x40199420)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ4_CTL3_MC_MAINEXT_REG 0x0B014420
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-4 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ4_CTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ4_OFFSETCTL3_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x40179424)                                                  */
/*       IVT_EX (0x40179424)                                                  */
/*       HSX (0x40199424)                                                     */
/*       BDX (0x40199424)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ4_OFFSETCTL3_MC_MAINEXT_REG 0x0B014424
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-4 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ4_OFFSETCTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ5_CTL3_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179428)                                                  */
/*       IVT_EX (0x40179428)                                                  */
/*       HSX (0x40199428)                                                     */
/*       BDX (0x40199428)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ5_CTL3_MC_MAINEXT_REG 0x0B014428
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-5 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ5_CTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ5_OFFSETCTL3_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x4017942C)                                                  */
/*       IVT_EX (0x4017942C)                                                  */
/*       HSX (0x4019942C)                                                     */
/*       BDX (0x4019942C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ5_OFFSETCTL3_MC_MAINEXT_REG 0x0B01442C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-5 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ5_OFFSETCTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ6_CTL3_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179430)                                                  */
/*       IVT_EX (0x40179430)                                                  */
/*       HSX (0x40199430)                                                     */
/*       BDX (0x40199430)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ6_CTL3_MC_MAINEXT_REG 0x0B014430
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-6 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ6_CTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ6_OFFSETCTL3_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x40179434)                                                  */
/*       IVT_EX (0x40179434)                                                  */
/*       HSX (0x40199434)                                                     */
/*       BDX (0x40199434)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ6_OFFSETCTL3_MC_MAINEXT_REG 0x0B014434
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-6 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ6_OFFSETCTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ7_CTL3_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179438)                                                  */
/*       IVT_EX (0x40179438)                                                  */
/*       HSX (0x40199438)                                                     */
/*       BDX (0x40199438)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ7_CTL3_MC_MAINEXT_REG 0x0B014438
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-7 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ7_CTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ7_OFFSETCTL3_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x4017943C)                                                  */
/*       IVT_EX (0x4017943C)                                                  */
/*       HSX (0x4019943C)                                                     */
/*       BDX (0x4019943C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ7_OFFSETCTL3_MC_MAINEXT_REG 0x0B01443C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-7 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ7_OFFSETCTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ8_CTL3_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179440)                                                  */
/*       IVT_EX (0x40179440)                                                  */
/*       HSX (0x40199440)                                                     */
/*       BDX (0x40199440)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ8_CTL3_MC_MAINEXT_REG 0x0B014440
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-8 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ8_CTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ8_OFFSETCTL3_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x40179444)                                                  */
/*       IVT_EX (0x40179444)                                                  */
/*       HSX (0x40199444)                                                     */
/*       BDX (0x40199444)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ8_OFFSETCTL3_MC_MAINEXT_REG 0x0B014444
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-8 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ8_OFFSETCTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ9_CTL3_MC_MAINEXT_REG supported on:                             */
/*       IVT_EP (0x40179448)                                                  */
/*       IVT_EX (0x40179448)                                                  */
/*       HSX (0x40199448)                                                     */
/*       BDX (0x40199448)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ9_CTL3_MC_MAINEXT_REG 0x0B014448
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-9 Control -
 * CPGC#_CH#_SUBSEQ#_CTL defines the controls for a particular subsequence.
 */
typedef union {
  struct {
    UINT32 num_cachelines : 7;
    /* num_cachelines - Bits[6:0], RW_LB, default = 7'b0000000 
       Number_ of_Cachelines defines how many cachelines must be transmitted before a 
       base subsequence or a particular offset direction (row and/or column) is 
       considered completed. 
     */
    UINT32 num_cachelines_scale : 1;
    /* num_cachelines_scale - Bits[7:7], RW_LB, default = 1'b0 
       Number_of_Cachelines operates as a linear or exponential counter based on the 
       value of Number_Of_Cachelines_Scale. 
     */
    UINT32 subseq_wait : 8;
    /* subseq_wait - Bits[15:8], RW_LB, default = 8'b00000000 
       # of DCLK cycles between completion of the current sub-sequence and beginning 
       the next sub-sequence. 
       The wait period is considered to be part of this subsequence for considering any 
       logical action. 
     */
    UINT32 rsvd_16 : 4;
    /* rsvd_16 - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 subseq_type : 4;
    /* subseq_type - Bits[23:20], RW_LB, default = 4'b0000 
       Defines what type of operation a particular Subsequence will be:
       0000: Base Read
       0001: Base Write
       0010: Base Read Write
       0011: Base Write Read
       0100: Offset Read
       0101: Offset Write
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 savecurr_ba_start : 1;
    /* savecurr_ba_start - Bits[25:25], RW_LB, default = 1'b0 
       Upon the entry to a subsequence where Save_Current_Base_Address_To_Start = 1 the 
       following actions must occur: 
       1) All the fields in Base Current Address (REUT_CH#_SEQ_BASE_ADDR_CURRENT) must 
       be written into the corresponding fields in Base Save Address 
       (REUT#_CH#_SEQ_BASE_ADDR_SAVE). 
       2) In the case where both Save_Current_Base_Address_To_Start = 1 and 
       Reset_Current_Base_Address_To_Start = 1 then only 
       Save_Current_Base_Address_To_Start = 1 will take effect and 
       Reset_Current_Base_Address_To_Start = 1 will be ignored. 
     */
    UINT32 rstcurr_ba_start : 1;
    /* rstcurr_ba_start - Bits[26:26], RW_LB, default = 1'b0 
       Upon entry to a subsequence where Reset_Current Base_Address_To_Start is set the 
       following actions must occur:Base Save Address (CPGC_CH#_SEQ_BASE_ADDR_SAVE ) 
       gets immediately re initialized to the Base Starting Address 
       (CPGC#_CH#_SEQ_BASE_ADDR_START). 
     */
    UINT32 dataecc_addrinv : 2;
    /* dataecc_addrinv - Bits[28:27], RW_LB, default = 2'b00 
       Data_and_ECC_Address_Inversion allows for data backgrounds based on the LSB of 
       the Column and Row Base address fields. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 inv_dataecc : 1;
    /* inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Invert_Data_and_ECC is used to toggle the polarity of the data and ECC for the 
       given subsequence which is then applied to the logical output of any 
       Address_Invesion to determine the final polarity for the current write or read. 
     */
    UINT32 stop_baseseq_wrptrig : 1;
    /* stop_baseseq_wrptrig - Bits[31:31], RW_LB, default = 1'b0 
       If Stop_Base_Subsequence_On_Wrap_Trigger is set then any Base Address Wrap 
       Trigger that occurs will immediately force a transition to the next Subsequence 
       after the Subsequence_Wait. 
       
       Please note the difference between CPGC#_CH#_SUBSEQ#_CTL.Stop_OnWrap_Trigger and 
       CPGC#_CH#_SEQ_CTL.Stop_OnWrap_Trigger as the former causes the subsequence to 
       end and the latter causes the sequence to end. 
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ9_CTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SUBSEQ9_OFFSETCTL3_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x4017944C)                                                  */
/*       IVT_EX (0x4017944C)                                                  */
/*       HSX (0x4019944C)                                                     */
/*       BDX (0x4019944C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SUBSEQ9_OFFSETCTL3_MC_MAINEXT_REG 0x0B01444C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence-9 Offset Control -
 * CPGC#_CH#_SUBSEQ#_OFFSET_CTL defines the controls for a particular subsequence 
 * in the case where the Subsequence_Type is set to an be an OFFSET (0x100 or 
 * 0x101). 
 */
typedef union {
  struct {
    UINT32 offadd_updrate : 5;
    /* offadd_updrate - Bits[4:0], RW_LB, default = 5'b00000 
       Offset_Address_Update_Rate determines the rate that the Current Offset Address 
       is updated. 
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_rptrate : 5;
    /* base_subseq_rptrate - Bits[12:8], RW_LB, default = 5'b00000 
       When the Subsequence_Type is set to an offset operation then 
       Base_Subsequence_Repeat_Rate determines how often the Base_Subsequence_Type 
       operation occurs for 1 cacheline. 
     */
    UINT32 rsvd_13 : 2;
    /* rsvd_13 - Bits[14:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 coloff_wrptrig_enb : 1;
    /* coloff_wrptrig_enb - Bits[15:15], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rowoff_wrptrig_enb : 1;
    /* rowoff_wrptrig_enb - Bits[16:16], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 base_subseq_type : 1;
    /* base_subseq_type - Bits[20:20], RW_LB, default = 1'b0 
       Everytime the Base_Subsequence_Repeat_Rate is reached a Read or Write is 
       performed for one command based on other fields. 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_order : 1;
    /* col_inc_order - Bits[24:24], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 col_inc_enb : 1;
    /* col_inc_enb - Bits[26:26], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 row_inc_order : 1;
    /* row_inc_order - Bits[27:27], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 row_inc_enb : 1;
    /* row_inc_enb - Bits[29:29], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 base_inv_dataecc : 1;
    /* base_inv_dataecc - Bits[30:30], RW_LB, default = 1'b0 
       Refer to CPGC_SUBSEQ0_OFFSETCTL0 for details.
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SUBSEQ9_OFFSETCTL3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */


/* CPGC_SEQ_LRRANKLG2PHYMAP0_MC_MAINEXT_REG supported on:                     */
/*       IVT_EP (0x40179450)                                                  */
/*       IVT_EX (0x40179450)                                                  */
/*       HSX (0x40199450)                                                     */
/*       BDX (0x40199450)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_LRRANKLG2PHYMAP0_MC_MAINEXT_REG 0x0B014450


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC LRDIMM Rank Mapping: Channel-0 Subsequence Logical to Physical Rank Lookup 
 * Table - 
 * LRDIMM Rank Mapping
 */
typedef union {
  struct {
    UINT32 lr0_mapping : 3;
    /* lr0_mapping - Bits[2:0], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-0 mapping
     */
    UINT32 lr1_mapping : 3;
    /* lr1_mapping - Bits[5:3], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-1 mapping
     */
    UINT32 lr2_mapping : 3;
    /* lr2_mapping - Bits[8:6], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-2 mapping
     */
    UINT32 lr3_mapping : 3;
    /* lr3_mapping - Bits[11:9], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-3 mapping
     */
    UINT32 lr4_mapping : 3;
    /* lr4_mapping - Bits[14:12], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-4 mapping
     */
    UINT32 lr5_mapping : 3;
    /* lr5_mapping - Bits[17:15], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-5 mapping
     */
    UINT32 lr6_mapping : 3;
    /* lr6_mapping - Bits[20:18], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-6 mapping
     */
    UINT32 lr7_mapping : 3;
    /* lr7_mapping - Bits[23:21], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-7 mapping
     */
    UINT32 lrdimm_rank_mapping : 3;
    /* lrdimm_rank_mapping - Bits[26:24], RW_LB, default = 3'b000 
       lrdimm_rank_mapping is a 3-bit field that maps which type of LRDIMM/3ds devices 
       have been populated in the system. 
       3'b000: No subranks. ddr3 will use row_addr[15:0], ddr4 will use row_addr[16:0]
       3'b001: subranks appended after A[14]
       3'b010: subranks appended after A[15]
       3'b011: subranks appended after A[13]; for ddr4 only
       3'b100: subranks appended after A[16]; for ddr4 only
       3'b101-3'b111: Reserved
       Note: Subrank information is obtained from lr*_mapping fields in this register.
     */
    UINT32 rsvd : 5;
    /* rsvd - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_LRRANKLG2PHYMAP0_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_LRRANKLG2PHYMAP1_MC_MAINEXT_REG supported on:                     */
/*       IVT_EP (0x40179454)                                                  */
/*       IVT_EX (0x40179454)                                                  */
/*       HSX (0x40199454)                                                     */
/*       BDX (0x40199454)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_LRRANKLG2PHYMAP1_MC_MAINEXT_REG 0x0B014454


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC LRDIMM Rank Mapping: Channel-1 Subsequence Logical to Physical Rank Lookup 
 * Table - 
 * LRDIMM Rank Mapping
 */
typedef union {
  struct {
    UINT32 lr0_mapping : 3;
    /* lr0_mapping - Bits[2:0], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-0 mapping
     */
    UINT32 lr1_mapping : 3;
    /* lr1_mapping - Bits[5:3], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-1 mapping
     */
    UINT32 lr2_mapping : 3;
    /* lr2_mapping - Bits[8:6], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-2 mapping
     */
    UINT32 lr3_mapping : 3;
    /* lr3_mapping - Bits[11:9], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-3 mapping
     */
    UINT32 lr4_mapping : 3;
    /* lr4_mapping - Bits[14:12], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-4 mapping
     */
    UINT32 lr5_mapping : 3;
    /* lr5_mapping - Bits[17:15], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-5 mapping
     */
    UINT32 lr6_mapping : 3;
    /* lr6_mapping - Bits[20:18], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-6 mapping
     */
    UINT32 lr7_mapping : 3;
    /* lr7_mapping - Bits[23:21], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-7 mapping
     */
    UINT32 lrdimm_rank_mapping : 3;
    /* lrdimm_rank_mapping - Bits[26:24], RW_LB, default = 3'b000 
       lrdimm_rank_mapping is a 3-bit field that maps which type of LRDIMM/3ds devices 
       have been populated in the system. 
       3'b000: No subranks. ddr3 will use row_addr[15:0], ddr4 will use row_addr[16:0]
       3'b001: subranks appended after A[14]
       3'b010: subranks appended after A[15]
       3'b011: subranks appended after A[13]; for ddr4 only
       3'b100: subranks appended after A[16]; for ddr4 only
       3'b101-3'b111: Reserved
       Note: Subrank information is obtained from lr*_mapping fields in this register.
     */
    UINT32 rsvd : 5;
    /* rsvd - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_LRRANKLG2PHYMAP1_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_LRRANKLG2PHYMAP2_MC_MAINEXT_REG supported on:                     */
/*       IVT_EP (0x40179458)                                                  */
/*       IVT_EX (0x40179458)                                                  */
/*       HSX (0x40199458)                                                     */
/*       BDX (0x40199458)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_LRRANKLG2PHYMAP2_MC_MAINEXT_REG 0x0B014458


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC LRDIMM Rank Mapping: Channel-2 Subsequence Logical to Physical Rank Lookup 
 * Table - 
 * LRDIMM Rank Mapping
 */
typedef union {
  struct {
    UINT32 lr0_mapping : 3;
    /* lr0_mapping - Bits[2:0], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-0 mapping
     */
    UINT32 lr1_mapping : 3;
    /* lr1_mapping - Bits[5:3], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-1 mapping
     */
    UINT32 lr2_mapping : 3;
    /* lr2_mapping - Bits[8:6], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-2 mapping
     */
    UINT32 lr3_mapping : 3;
    /* lr3_mapping - Bits[11:9], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-3 mapping
     */
    UINT32 lr4_mapping : 3;
    /* lr4_mapping - Bits[14:12], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-4 mapping
     */
    UINT32 lr5_mapping : 3;
    /* lr5_mapping - Bits[17:15], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-5 mapping
     */
    UINT32 lr6_mapping : 3;
    /* lr6_mapping - Bits[20:18], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-6 mapping
     */
    UINT32 lr7_mapping : 3;
    /* lr7_mapping - Bits[23:21], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-7 mapping
     */
    UINT32 lrdimm_rank_mapping : 3;
    /* lrdimm_rank_mapping - Bits[26:24], RW_LB, default = 3'b000 
       lrdimm_rank_mapping is a 3-bit field that maps which type of LRDIMM/3ds devices 
       have been populated in the system. 
       3'b000: No subranks. ddr3 will use row_addr[15:0], ddr4 will use row_addr[16:0]
       3'b001: subranks appended after A[14]
       3'b010: subranks appended after A[15]
       3'b011: subranks appended after A[13]; for ddr4 only
       3'b100: subranks appended after A[16]; for ddr4 only
       3'b101-3'b111: Reserved
       Note: Subrank information is obtained from lr*_mapping fields in this register.
     */
    UINT32 rsvd : 5;
    /* rsvd - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_LRRANKLG2PHYMAP2_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_SEQ_LRRANKLG2PHYMAP3_MC_MAINEXT_REG supported on:                     */
/*       IVT_EP (0x4017945C)                                                  */
/*       IVT_EX (0x4017945C)                                                  */
/*       HSX (0x4019945C)                                                     */
/*       BDX (0x4019945C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_LRRANKLG2PHYMAP3_MC_MAINEXT_REG 0x0B01445C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.1.CFG.xml.
 * CPGC LRDIMM Rank Mapping: Channel-3 Subsequence Logical to Physical Rank Lookup 
 * Table - 
 * LRDIMM Rank Mapping
 */
typedef union {
  struct {
    UINT32 lr0_mapping : 3;
    /* lr0_mapping - Bits[2:0], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-0 mapping
     */
    UINT32 lr1_mapping : 3;
    /* lr1_mapping - Bits[5:3], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-1 mapping
     */
    UINT32 lr2_mapping : 3;
    /* lr2_mapping - Bits[8:6], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-2 mapping
     */
    UINT32 lr3_mapping : 3;
    /* lr3_mapping - Bits[11:9], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-3 mapping
     */
    UINT32 lr4_mapping : 3;
    /* lr4_mapping - Bits[14:12], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-4 mapping
     */
    UINT32 lr5_mapping : 3;
    /* lr5_mapping - Bits[17:15], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-5 mapping
     */
    UINT32 lr6_mapping : 3;
    /* lr6_mapping - Bits[20:18], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-6 mapping
     */
    UINT32 lr7_mapping : 3;
    /* lr7_mapping - Bits[23:21], RW_LB, default = 3'b000 
       ddr3_lrdim/ddr4 subrank-7 mapping
     */
    UINT32 lrdimm_rank_mapping : 3;
    /* lrdimm_rank_mapping - Bits[26:24], RW_LB, default = 3'b000 
       lrdimm_rank_mapping is a 3-bit field that maps which type of LRDIMM/3ds devices 
       have been populated in the system. 
       3'b000: No subranks. ddr3 will use row_addr[15:0], ddr4 will use row_addr[16:0]
       3'b001: subranks appended after A[14]
       3'b010: subranks appended after A[15]
       3'b011: subranks appended after A[13]; for ddr4 only
       3'b100: subranks appended after A[16]; for ddr4 only
       3'b101-3'b111: Reserved
       Note: Subrank information is obtained from lr*_mapping fields in this register.
     */
    UINT32 rsvd : 5;
    /* rsvd - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_LRRANKLG2PHYMAP3_MC_MAINEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */






/* CPGC_SEQ_DMYRDADDR_HI_MC_MAINEXT_REG supported on:                         */
/*       HSX (0x40199464)                                                     */
/*       BDX (0x40199464)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_DMYRDADDR_HI_MC_MAINEXT_REG 0x0B014464

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * Defines what Banks and the Starting Row Address for the Dummy Reads to occur.  
 * (for chs 2 and 3) 
 * Dummy Reads are defined as a qualified read request which will generate the 
 * necessary Activates and Precharges but no Read will occur. 
 */
typedef union {
  struct {
    UINT32 bank_mask2 : 16;
    /* bank_mask2 - Bits[15:0], RW_LB, default = 16'b0000000000000000 
       Refer to Bank_Mask0 for details
     */
    UINT32 bank_mask3 : 16;
    /* bank_mask3 - Bits[31:16], RW_LB, default = 16'b0000000000000000 
       Refer to Bank_Mask0 for details
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_DMYRDADDR_HI_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRSTART_HI0_MC_MAINEXT_REG supported on:                    */
/*       HSX (0x40199468)                                                     */
/*       BDX (0x40199468)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSTART_HI0_MC_MAINEXT_REG 0x0B014468

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC#_CH#_SEQ_BASE_ADDR_START defines the starting base address of the sequence.
 */
typedef union {
  struct {
    UINT32 row_addr : 17;
    /* row_addr - Bits[16:0], RW_LB, default = 17'b00000000000000000 
       Starting Row Address
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_addr : 3;
    /* rank_addr - Bits[26:24], RW_LB, default = 3'b000 
       Starting Logical Rank Address
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRSTART_HI0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRSTART_HI1_MC_MAINEXT_REG supported on:                    */
/*       HSX (0x4019946C)                                                     */
/*       BDX (0x4019946C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSTART_HI1_MC_MAINEXT_REG 0x0B01446C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC#_CH#_SEQ_BASE_ADDR_START defines the starting base address of the sequence.
 */
typedef union {
  struct {
    UINT32 row_addr : 17;
    /* row_addr - Bits[16:0], RW_LB, default = 17'b00000000000000000 
       Starting Row Address
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_addr : 3;
    /* rank_addr - Bits[26:24], RW_LB, default = 3'b000 
       Starting Logical Rank Address
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRSTART_HI1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRSTART_HI2_MC_MAINEXT_REG supported on:                    */
/*       HSX (0x40199470)                                                     */
/*       BDX (0x40199470)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSTART_HI2_MC_MAINEXT_REG 0x0B014470

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC#_CH#_SEQ_BASE_ADDR_START defines the starting base address of the sequence.
 */
typedef union {
  struct {
    UINT32 row_addr : 17;
    /* row_addr - Bits[16:0], RW_LB, default = 17'b00000000000000000 
       Starting Row Address
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_addr : 3;
    /* rank_addr - Bits[26:24], RW_LB, default = 3'b000 
       Starting Logical Rank Address
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRSTART_HI2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRSTART_HI3_MC_MAINEXT_REG supported on:                    */
/*       HSX (0x40199474)                                                     */
/*       BDX (0x40199474)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSTART_HI3_MC_MAINEXT_REG 0x0B014474

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC#_CH#_SEQ_BASE_ADDR_START defines the starting base address of the sequence.
 */
typedef union {
  struct {
    UINT32 row_addr : 17;
    /* row_addr - Bits[16:0], RW_LB, default = 17'b00000000000000000 
       Starting Row Address
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_addr : 3;
    /* rank_addr - Bits[26:24], RW_LB, default = 3'b000 
       Starting Logical Rank Address
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRSTART_HI3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x40199478)                                                     */
/*       BDX (0x40199478)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAINEXT_REG 0x0B014478

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC#_CH#_SEQ_BASE_ADDR_WRAP defines the wrap base address of the sequence.
 */
typedef union {
  struct {
    UINT32 row_addr : 17;
    /* row_addr - Bits[16:0], RW_LB, default = 17'b00000000000000000 
       Same as field [6:0] for Row-Addr
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_addr : 3;
    /* rank_addr - Bits[26:24], RW_LB, default = 3'b000 
       Same as field [6:0] for Rank-Addr
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRWRAP_HI1_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x4019947C)                                                     */
/*       BDX (0x4019947C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRWRAP_HI1_MC_MAINEXT_REG 0x0B01447C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC#_CH#_SEQ_BASE_ADDR_WRAP defines the wrap base address of the sequence.
 */
typedef union {
  struct {
    UINT32 row_addr : 17;
    /* row_addr - Bits[16:0], RW_LB, default = 17'b00000000000000000 
       Same as field [6:0] for Row-Addr
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_addr : 3;
    /* rank_addr - Bits[26:24], RW_LB, default = 3'b000 
       Same as field [6:0] for Rank-Addr
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRWRAP_HI1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRWRAP_HI2_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x40199480)                                                     */
/*       BDX (0x40199480)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRWRAP_HI2_MC_MAINEXT_REG 0x0B014480

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC#_CH#_SEQ_BASE_ADDR_WRAP defines the wrap base address of the sequence.
 */
typedef union {
  struct {
    UINT32 row_addr : 17;
    /* row_addr - Bits[16:0], RW_LB, default = 17'b00000000000000000 
       Same as field [6:0] for Row-Addr
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_addr : 3;
    /* rank_addr - Bits[26:24], RW_LB, default = 3'b000 
       Same as field [6:0] for Rank-Addr
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRWRAP_HI2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRWRAP_HI3_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x40199484)                                                     */
/*       BDX (0x40199484)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRWRAP_HI3_MC_MAINEXT_REG 0x0B014484

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC#_CH#_SEQ_BASE_ADDR_WRAP defines the wrap base address of the sequence.
 */
typedef union {
  struct {
    UINT32 row_addr : 17;
    /* row_addr - Bits[16:0], RW_LB, default = 17'b00000000000000000 
       Same as field [6:0] for Row-Addr
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_addr : 3;
    /* rank_addr - Bits[26:24], RW_LB, default = 3'b000 
       Same as field [6:0] for Rank-Addr
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRWRAP_HI3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRCURR_HI0_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x40199488)                                                     */
/*       BDX (0x40199488)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRCURR_HI0_MC_MAINEXT_REG 0x0B014488

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC#_CH#_SEQ_BASE_ADDR_CURRENT defines the current base address of the 
 * sequence. 
 */
typedef union {
  struct {
    UINT32 row_addr : 17;
    /* row_addr - Bits[16:0], RO_V, default = 17'b00000000000000000 
       Current Base Row Address
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_addr : 3;
    /* rank_addr - Bits[26:24], RO_V, default = 3'b000 
       Current Base Logical Rank Address
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRCURR_HI0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRCURR_HI1_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x4019948C)                                                     */
/*       BDX (0x4019948C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRCURR_HI1_MC_MAINEXT_REG 0x0B01448C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC#_CH#_SEQ_BASE_ADDR_CURRENT defines the current base address of the 
 * sequence. 
 */
typedef union {
  struct {
    UINT32 row_addr : 17;
    /* row_addr - Bits[16:0], RO_V, default = 17'b00000000000000000 
       Current Base Row Address
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_addr : 3;
    /* rank_addr - Bits[26:24], RO_V, default = 3'b000 
       Current Base Logical Rank Address
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRCURR_HI1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRCURR_HI2_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x40199490)                                                     */
/*       BDX (0x40199490)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRCURR_HI2_MC_MAINEXT_REG 0x0B014490

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC#_CH#_SEQ_BASE_ADDR_CURRENT defines the current base address of the 
 * sequence. 
 */
typedef union {
  struct {
    UINT32 row_addr : 17;
    /* row_addr - Bits[16:0], RO_V, default = 17'b00000000000000000 
       Current Base Row Address
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_addr : 3;
    /* rank_addr - Bits[26:24], RO_V, default = 3'b000 
       Current Base Logical Rank Address
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRCURR_HI2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRCURR_HI3_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x40199494)                                                     */
/*       BDX (0x40199494)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRCURR_HI3_MC_MAINEXT_REG 0x0B014494

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC#_CH#_SEQ_BASE_ADDR_CURRENT defines the current base address of the 
 * sequence. 
 */
typedef union {
  struct {
    UINT32 row_addr : 17;
    /* row_addr - Bits[16:0], RO_V, default = 17'b00000000000000000 
       Current Base Row Address
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_addr : 3;
    /* rank_addr - Bits[26:24], RO_V, default = 3'b000 
       Current Base Logical Rank Address
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRCURR_HI3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRSAVE_LO0_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x40199498)                                                     */
/*       BDX (0x40199498)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSAVE_LO0_MC_MAINEXT_REG 0x0B014498

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-0 Sub-Sequence Save Address -
 * CPGC#_CH#_SEQ_BASE_ADDR_SAVE defines the Save base address of the sequence.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_addr : 10;
    /* column_addr - Bits[12:3], RW_LBV, default = 10'b0000000000 
       Save Column Address. Normally the lowest three column bits are reserved since 
       all transfers are done on a cacheline basis (8 chunks 0-7), 
       but for implementations that address to a sub cacheline then these reserved bits 
       might be used as needed. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_13 : 11;
    /* rsvd_13 - Bits[23:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_addr : 4;
    /* bank_addr - Bits[27:24], RW_LBV, default = 4'b0000 
       Save Bank Address
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRSAVE_LO0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRSAVE_LO1_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x4019949C)                                                     */
/*       BDX (0x4019949C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSAVE_LO1_MC_MAINEXT_REG 0x0B01449C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-1 Sub-Sequence Save Address -
 * CPGC#_CH#_SEQ_BASE_ADDR_SAVE defines the Save base address of the sequence.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_addr : 10;
    /* column_addr - Bits[12:3], RW_LBV, default = 10'b0000000000 
       Save Column Address. Normally the lowest three column bits are reserved since 
       all transfers are done on a cacheline basis (8 chunks 0-7), 
       but for implementations that address to a sub cacheline then these reserved bits 
       might be used as needed. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_13 : 11;
    /* rsvd_13 - Bits[23:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_addr : 4;
    /* bank_addr - Bits[27:24], RW_LBV, default = 4'b0000 
       Save Bank Address
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRSAVE_LO1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRSAVE_LO2_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x401994A0)                                                     */
/*       BDX (0x401994A0)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSAVE_LO2_MC_MAINEXT_REG 0x0B0144A0

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-2 Sub-Sequence Save Address -
 * CPGC#_CH#_SEQ_BASE_ADDR_SAVE defines the Save base address of the sequence.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_addr : 10;
    /* column_addr - Bits[12:3], RW_LBV, default = 10'b0000000000 
       Save Column Address. Normally the lowest three column bits are reserved since 
       all transfers are done on a cacheline basis (8 chunks 0-7), 
       but for implementations that address to a sub cacheline then these reserved bits 
       might be used as needed. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_13 : 11;
    /* rsvd_13 - Bits[23:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_addr : 4;
    /* bank_addr - Bits[27:24], RW_LBV, default = 4'b0000 
       Save Bank Address
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRSAVE_LO2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRSAVE_LO3_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x401994A4)                                                     */
/*       BDX (0x401994A4)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSAVE_LO3_MC_MAINEXT_REG 0x0B0144A4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC Channel-3 Sub-Sequence Save Address -
 * CPGC#_CH#_SEQ_BASE_ADDR_SAVE defines the Save base address of the sequence.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 column_addr : 10;
    /* column_addr - Bits[12:3], RW_LBV, default = 10'b0000000000 
       Save Column Address. Normally the lowest three column bits are reserved since 
       all transfers are done on a cacheline basis (8 chunks 0-7), 
       but for implementations that address to a sub cacheline then these reserved bits 
       might be used as needed. 
       expanded to 10 bits (cpgcbl4 support+14bit column ddr3)
     */
    UINT32 rsvd_13 : 11;
    /* rsvd_13 - Bits[23:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank_addr : 4;
    /* bank_addr - Bits[27:24], RW_LBV, default = 4'b0000 
       Save Bank Address
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRSAVE_LO3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRSAVE_HI0_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x401994A8)                                                     */
/*       BDX (0x401994A8)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSAVE_HI0_MC_MAINEXT_REG 0x0B0144A8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC#_CH#_SEQ_BASE_ADDR_SAVE defines the Save base address of the sequence.
 */
typedef union {
  struct {
    UINT32 row_addr : 17;
    /* row_addr - Bits[16:0], RW_LBV, default = 17'b00000000000000000 
       Save Row Address
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_addr : 3;
    /* rank_addr - Bits[26:24], RW_LBV, default = 3'b000 
       Save Logical Rank Address
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRSAVE_HI0_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRSAVE_HI1_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x401994AC)                                                     */
/*       BDX (0x401994AC)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSAVE_HI1_MC_MAINEXT_REG 0x0B0144AC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC#_CH#_SEQ_BASE_ADDR_SAVE defines the Save base address of the sequence.
 */
typedef union {
  struct {
    UINT32 row_addr : 17;
    /* row_addr - Bits[16:0], RW_LBV, default = 17'b00000000000000000 
       Save Row Address
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_addr : 3;
    /* rank_addr - Bits[26:24], RW_LBV, default = 3'b000 
       Save Logical Rank Address
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRSAVE_HI1_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRSAVE_HI2_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x401994B0)                                                     */
/*       BDX (0x401994B0)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSAVE_HI2_MC_MAINEXT_REG 0x0B0144B0

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC#_CH#_SEQ_BASE_ADDR_SAVE defines the Save base address of the sequence.
 */
typedef union {
  struct {
    UINT32 row_addr : 17;
    /* row_addr - Bits[16:0], RW_LBV, default = 17'b00000000000000000 
       Save Row Address
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_addr : 3;
    /* rank_addr - Bits[26:24], RW_LBV, default = 3'b000 
       Save Logical Rank Address
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRSAVE_HI2_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_SEQ_BASEADDRSAVE_HI3_MC_MAINEXT_REG supported on:                     */
/*       HSX (0x401994B4)                                                     */
/*       BDX (0x401994B4)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSAVE_HI3_MC_MAINEXT_REG 0x0B0144B4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.1.CFG.xml.
 * CPGC#_CH#_SEQ_BASE_ADDR_SAVE defines the Save base address of the sequence.
 */
typedef union {
  struct {
    UINT32 row_addr : 17;
    /* row_addr - Bits[16:0], RW_LBV, default = 17'b00000000000000000 
       Save Row Address
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_addr : 3;
    /* rank_addr - Bits[26:24], RW_LBV, default = 3'b000 
       Save Logical Rank Address
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_SEQ_BASEADDRSAVE_HI3_MC_MAINEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MTCTL_MC_MAINEXT_REG supported on:                                         */
/*       IVT_EP (0x401790D0)                                                  */
/*       IVT_EX (0x401790D0)                                                  */
/* Register default value:              0x00000000                            */
#define MTCTL_MC_MAINEXT_REG 0x0B0140D0











/* MCIOTCTL_MC_MAINEXT_REG supported on:                                      */
/*       IVT_EP (0x101791A4)                                                  */
/*       IVT_EX (0x101791A4)                                                  */
/* Register default value:              0x00                                  */
#define MCIOTCTL_MC_MAINEXT_REG 0x0B0111A4



/* CPGC_SEQ_BASEADDRSTART0_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x4017923C)                                                  */
/*       IVT_EX (0x4017923C)                                                  */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSTART0_MC_MAINEXT_REG 0x0B01423C



/* CPGC_SEQ_BASEADDRSTART1_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x40179240)                                                  */
/*       IVT_EX (0x40179240)                                                  */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSTART1_MC_MAINEXT_REG 0x0B014240



/* CPGC_SEQ_BASEADDRSTART2_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x40179244)                                                  */
/*       IVT_EX (0x40179244)                                                  */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSTART2_MC_MAINEXT_REG 0x0B014244



/* CPGC_SEQ_BASEADDRSTART3_MC_MAINEXT_REG supported on:                       */
/*       IVT_EP (0x40179248)                                                  */
/*       IVT_EX (0x40179248)                                                  */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRSTART3_MC_MAINEXT_REG 0x0B014248



/* CPGC_SEQ_BASEADDRWRAP0_MC_MAINEXT_REG supported on:                        */
/*       IVT_EP (0x4017924C)                                                  */
/*       IVT_EX (0x4017924C)                                                  */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRWRAP0_MC_MAINEXT_REG 0x0B01424C



/* CPGC_SEQ_BASEADDRWRAP1_MC_MAINEXT_REG supported on:                        */
/*       IVT_EP (0x40179250)                                                  */
/*       IVT_EX (0x40179250)                                                  */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRWRAP1_MC_MAINEXT_REG 0x0B014250



/* CPGC_SEQ_BASEADDRWRAP2_MC_MAINEXT_REG supported on:                        */
/*       IVT_EP (0x40179254)                                                  */
/*       IVT_EX (0x40179254)                                                  */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRWRAP2_MC_MAINEXT_REG 0x0B014254



/* CPGC_SEQ_BASEADDRWRAP3_MC_MAINEXT_REG supported on:                        */
/*       IVT_EP (0x40179258)                                                  */
/*       IVT_EX (0x40179258)                                                  */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRWRAP3_MC_MAINEXT_REG 0x0B014258



/* CPGC_SEQ_BASEADDRCURR0_MC_MAINEXT_REG supported on:                        */
/*       IVT_EP (0x4017925C)                                                  */
/*       IVT_EX (0x4017925C)                                                  */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRCURR0_MC_MAINEXT_REG 0x0B01425C



/* CPGC_SEQ_BASEADDRCURR1_MC_MAINEXT_REG supported on:                        */
/*       IVT_EP (0x40179260)                                                  */
/*       IVT_EX (0x40179260)                                                  */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRCURR1_MC_MAINEXT_REG 0x0B014260



/* CPGC_SEQ_BASEADDRCURR2_MC_MAINEXT_REG supported on:                        */
/*       IVT_EP (0x40179264)                                                  */
/*       IVT_EX (0x40179264)                                                  */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRCURR2_MC_MAINEXT_REG 0x0B014264



/* CPGC_SEQ_BASEADDRCURR3_MC_MAINEXT_REG supported on:                        */
/*       IVT_EP (0x40179268)                                                  */
/*       IVT_EX (0x40179268)                                                  */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_BASEADDRCURR3_MC_MAINEXT_REG 0x0B014268



/* CPGC_SEQ_DMYRDADDR_MC_MAINEXT_REG supported on:                            */
/*       IVT_EP (0x401792EC)                                                  */
/*       IVT_EX (0x401792EC)                                                  */
/* Register default value:              0x00000000                            */
#define CPGC_SEQ_DMYRDADDR_MC_MAINEXT_REG 0x0B0142EC





















#endif /* MC_MAINEXT_h */
