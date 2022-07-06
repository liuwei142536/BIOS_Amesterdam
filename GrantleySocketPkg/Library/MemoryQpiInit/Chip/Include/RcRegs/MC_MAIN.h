/* Date Stamp: 8/23/2014 */

#ifndef MC_MAIN_h
#define MC_MAIN_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* MC_MAIN_IVT_DEV 15                                                         */
/* MC_MAIN_IVT_FUN 0                                                          */
/* For HSX_HOST:                                                              */
/* MC_MAIN_HSX_DEV 19                                                         */
/* MC_MAIN_HSX_FUN 0                                                          */
/* For BDX_HOST:                                                              */
/* MC_MAIN_BDX_DEV 19                                                         */
/* MC_MAIN_BDX_FUN 0                                                          */

/* VID_MC_MAIN_REG supported on:                                              */
/*       IVT_EP (0x20178000)                                                  */
/*       IVT_EX (0x20178000)                                                  */
/*       HSX (0x20198000)                                                     */
/*       BDX (0x20198000)                                                     */
/* Register default value:              0x8086                                */
#define VID_MC_MAIN_REG 0x0B002000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
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
} VID_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* DID_MC_MAIN_REG supported on:                                              */
/*       IVT_EP (0x20178002)                                                  */
/*       IVT_EX (0x20178002)                                                  */
/*       HSX (0x20198002)                                                     */
/*       BDX (0x20198002)                                                     */
/* Register default value on IVT_EP:    0x0EA8                                */
/* Register default value on IVT_EX:    0x0EA8                                */
/* Register default value on HSX:       0x2FA8                                */
/* Register default value on BDX:       0x6FA8                                */
#define DID_MC_MAIN_REG 0x0B002002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6FA8 
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
} DID_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* PCICMD_MC_MAIN_REG supported on:                                           */
/*       IVT_EP (0x20178004)                                                  */
/*       IVT_EX (0x20178004)                                                  */
/*       HSX (0x20198004)                                                     */
/*       BDX (0x20198004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_MC_MAIN_REG 0x0B002004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
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
} PCICMD_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* PCISTS_MC_MAIN_REG supported on:                                           */
/*       IVT_EP (0x20178006)                                                  */
/*       IVT_EX (0x20178006)                                                  */
/*       HSX (0x20198006)                                                     */
/*       BDX (0x20198006)                                                     */
/* Register default value:              0x0010                                */
#define PCISTS_MC_MAIN_REG 0x0B002006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.0.CFG.xml.
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
} PCISTS_MC_MAIN_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_MC_MAIN_REG supported on:                                              */
/*       IVT_EP (0x10178008)                                                  */
/*       IVT_EX (0x10178008)                                                  */
/*       HSX (0x10198008)                                                     */
/*       BDX (0x10198008)                                                     */
/* Register default value:              0x00                                  */
#define RID_MC_MAIN_REG 0x0B001008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
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
} RID_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_MC_MAIN_REG supported on:                                           */
/*       IVT_EP (0x10178009)                                                  */
/*       IVT_EX (0x10178009)                                                  */
/*       HSX (0x10198009)                                                     */
/*       BDX (0x10198009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_MC_MAIN_REG 0x0B001009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.0.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_MC_MAIN_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_MC_MAIN_REG supported on:                                           */
/*       IVT_EP (0x2017800A)                                                  */
/*       IVT_EX (0x2017800A)                                                  */
/*       HSX (0x2019800A)                                                     */
/*       BDX (0x2019800A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_MC_MAIN_REG 0x0B00200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
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
} CCR_N1_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* CLSR_MC_MAIN_REG supported on:                                             */
/*       IVT_EP (0x1017800C)                                                  */
/*       IVT_EX (0x1017800C)                                                  */
/*       HSX (0x1019800C)                                                     */
/*       BDX (0x1019800C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_MC_MAIN_REG 0x0B00100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
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
} CLSR_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* PLAT_MC_MAIN_REG supported on:                                             */
/*       IVT_EP (0x1017800D)                                                  */
/*       IVT_EX (0x1017800D)                                                  */
/*       HSX (0x1019800D)                                                     */
/*       BDX (0x1019800D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_MC_MAIN_REG 0x0B00100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
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
} PLAT_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* HDR_MC_MAIN_REG supported on:                                              */
/*       IVT_EP (0x1017800E)                                                  */
/*       IVT_EX (0x1017800E)                                                  */
/*       HSX (0x1019800E)                                                     */
/*       BDX (0x1019800E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_MC_MAIN_REG 0x0B00100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
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
} HDR_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* BIST_MC_MAIN_REG supported on:                                             */
/*       IVT_EP (0x1017800F)                                                  */
/*       IVT_EX (0x1017800F)                                                  */
/*       HSX (0x1019800F)                                                     */
/*       BDX (0x1019800F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_MC_MAIN_REG 0x0B00100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
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
} BIST_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* SVID_MC_MAIN_REG supported on:                                             */
/*       IVT_EP (0x2017802C)                                                  */
/*       IVT_EX (0x2017802C)                                                  */
/*       HSX (0x2019802C)                                                     */
/*       BDX (0x2019802C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_MC_MAIN_REG 0x0B00202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x02c
 */
typedef union {
  struct {
    UINT16 subsystem_vendor_identification_number : 16;
    /* subsystem_vendor_identification_number - Bits[15:0], RW_O, default = 16'b1000000010000110 
       The default value specifies Intel but can be set to any value once after reset.
     */
  } Bits;
  UINT16 Data;
} SVID_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* SDID_MC_MAIN_REG supported on:                                             */
/*       IVT_EP (0x2017802E)                                                  */
/*       IVT_EX (0x2017802E)                                                  */
/*       HSX (0x2019802E)                                                     */
/*       BDX (0x2019802E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_MC_MAIN_REG 0x0B00202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x02e
 */
typedef union {
  struct {
    UINT16 subsystem_device_identification_number : 16;
    /* subsystem_device_identification_number - Bits[15:0], RW_O, default = 16'b0000000000000000 
       Assigned by the subsystem vendor to uniquely identify the subsystem
     */
  } Bits;
  UINT16 Data;
} SDID_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_MC_MAIN_REG supported on:                                           */
/*       IVT_EP (0x10178034)                                                  */
/*       IVT_EX (0x10178034)                                                  */
/*       HSX (0x10198034)                                                     */
/*       BDX (0x10198034)                                                     */
/* Register default value on IVT_EP:    0x40                                  */
/* Register default value on IVT_EX:    0x40                                  */
/* Register default value on HSX:       0x00                                  */
/* Register default value on BDX:       0x40                                  */
#define CAPPTR_MC_MAIN_REG 0x0B001034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
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
} CAPPTR_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* INTL_MC_MAIN_REG supported on:                                             */
/*       IVT_EP (0x1017803C)                                                  */
/*       IVT_EX (0x1017803C)                                                  */
/*       HSX (0x1019803C)                                                     */
/*       BDX (0x1019803C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_MC_MAIN_REG 0x0B00103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
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
} INTL_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* INTPIN_MC_MAIN_REG supported on:                                           */
/*       IVT_EP (0x1017803D)                                                  */
/*       IVT_EX (0x1017803D)                                                  */
/*       HSX (0x1019803D)                                                     */
/*       BDX (0x1019803D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_MC_MAIN_REG 0x0B00103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
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
} INTPIN_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* MINGNT_MC_MAIN_REG supported on:                                           */
/*       IVT_EP (0x1017803E)                                                  */
/*       IVT_EX (0x1017803E)                                                  */
/*       HSX (0x1019803E)                                                     */
/*       BDX (0x1019803E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_MC_MAIN_REG 0x0B00103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
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
} MINGNT_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_MC_MAIN_REG supported on:                                           */
/*       IVT_EP (0x1017803F)                                                  */
/*       IVT_EX (0x1017803F)                                                  */
/*       HSX (0x1019803F)                                                     */
/*       BDX (0x1019803F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_MC_MAIN_REG 0x0B00103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
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
} MAXLAT_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* PXPCAP_MC_MAIN_REG supported on:                                           */
/*       IVT_EP (0x40178040)                                                  */
/*       IVT_EX (0x40178040)                                                  */
/*       HSX (0x40198040)                                                     */
/*       BDX (0x40198040)                                                     */
/* Register default value:              0x00910010                            */
#define PXPCAP_MC_MAIN_REG 0x0B004040
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x040
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
} PXPCAP_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* MCMTR_MC_MAIN_REG supported on:                                            */
/*       IVT_EP (0x4017807C)                                                  */
/*       IVT_EX (0x4017807C)                                                  */
/*       HSX (0x4019807C)                                                     */
/*       BDX (0x4019807C)                                                     */
/* Register default value:              0x00000000                            */
#define MCMTR_MC_MAIN_REG 0x0B00407C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x07c
 */
typedef union {
  struct {
    UINT32 close_pg : 1;
    /* close_pg - Bits[0:0], RW_LB, default = 1'b0 
       Use close page address mapping if set; otherwise, open page. (EX processor does 
       not support open page) 
     */
    UINT32 ls_en : 1;
    /* ls_en - Bits[1:1], RW_LBV, default = 1'b0 
       Use lock-step channel mode if set; otherwise, independent channel mode.
       This field should only be set for native ddr lockstep.
       Intel SMI 2 Lockstep 1:1 mode should always set this bit to 0
     */
    UINT32 ecc_en : 1;
    /* ecc_en - Bits[2:2], RW_LBV, default = 1'b0 
       ECC enable.
     */
    UINT32 dir_en : 1;
    /* dir_en - Bits[3:3], RW_LBV, default = 1'b0 
       Directory Cache Enable. Read-Only (RO) with 0 value if not supported. (Only 
       supported on processors which support directory cache) 
       Should always be set with Intel SMI 2 for the EX processor.
     */
    UINT32 rsvd_4 : 4;
    /* rsvd_4 - Bits[7:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 normal : 1;
    /* normal - Bits[8:8], RW_LB, default = 1'b0 
       0: Training mode
       1: Normal Mode
     */
    UINT32 bank_xor_enable : 1;
    /* bank_xor_enable - Bits[9:9], RW_LB, default = 1'b0 
       When set, this bit will enable bank XOR'ing. This is targeted at workloads that 
       bank thrashing caused by certain stride or page mappings. If one detects 
       unexpectedly poor page hit rates, one can attempt to flip this bit to see if it 
       helps. 
       
       [0]: Our base configuration. Bank selection is done using rank address bits 
       12:17:18 for open page mapping and bits 6:7:8 for close page mapping. 
       
       [1]: Bank XOR'ing enabled. Bank selection is done using rank address bits:
       (12^19):(17^20):(18^21) for open page mapping
       (6^19):(7^20):(8^21) for close page mapping
     */
    UINT32 trng_mode : 2;
    /* trng_mode - Bits[11:10], RW_LB, default = 2'b00 
       00: reserved
       01: Native CPGC Mode and Setting to be used post Intel SMI 2 CMD training 
       (including EV). (mcmtr.normal must be zero for this mode)(EX processor only) 
       10: CPGC Intel SMI 2 CMD training mode - Setting till Intel SMI 2 CMD 
       (coarse/fine) bus is trained (mcmtr.normal must be zero for this mode)(EX 
       processor only) 
       11: Normal Mode (mcmtr.normal is a don't care for this mode)
     */
    UINT32 imc_mode : 2;
    /* imc_mode - Bits[13:12], RW_LB, default = 2'b00 
       Memory mode:
       00: Native DDR
       01: Reserved
       10: Intel SMI 2 1:1 Subchannel Lockstep Mode (EX processor only)
       11: Intel SMI 2 2:1 Performance Mode (EX processor only)
     */
    UINT32 ddr4 : 1;
    /* ddr4 - Bits[14:14], RW_LB, default = 1'b0 
       DDR4 mode
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pass76 : 2;
    /* pass76 - Bits[17:16], RW_LB, default = 2'b00 
       00: do not alter ChnAdd calculation
       01: replace ChnAdd[6] with SysAdd[6]
       10: Reserved
       11: replace ChnAdd[7:6] with SysAdd[7:6]
     */
    UINT32 chn_disable : 4;
    /* chn_disable - Bits[21:18], RW_LB, default = 4'b0000 
       Channel disable control. When set, the corresponding channel is disabled. Note: 
       Message Channel may not work if all channels are set to disable in this field. 
     */
    UINT32 ch23cmd_ctl_delay : 1;
    /* ch23cmd_ctl_delay - Bits[22:22], RW_LB, default = 1'b0 
       Set to one to enable extra pipe state delay for channel 2 and 3 command and 
       control signals out to DDRIO. 
     */
    UINT32 rsvd_23 : 9;
    /* rsvd_23 - Bits[31:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MCMTR_MC_MAIN_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* TADWAYNESS_0_MC_MAIN_REG supported on:                                     */
/*       IVT_EP (0x40178080)                                                  */
/*       IVT_EX (0x40178080)                                                  */
/*       HSX (0x40198080)                                                     */
/*       BDX (0x40198080)                                                     */
/* Register default value:              0x00000000                            */
#define TADWAYNESS_0_MC_MAIN_REG 0x0B004080
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * There are total of 12 TAD ranges (N+P+1= number of TAD ranges; P= how many times 
 * channel interleave changes within the SAD ranges.). 
 * 
 * Note for mirroring configuration:
 * For 1-way interleave, channel 0-2 mirror pair: target list = <0,2,x,x>, TAD ways 
 * = "00". 
 * For 1-way interleave, channel 1-3 mirror pair: target list = <1,3,x,x>, TAD ways 
 * = "00". 
 * For 2-way interleave, 0-2 mirror pair and 1-3 mirror pair: target list = 
 * <0,1,2,3>, TAD ways = "01". 
 * For 1-way interleave, lockstep + mirroring, target list = <0,2,x,x>, TAD ways = 
 * "00". 
 */
typedef union {
  struct {
    UINT32 tad_ch_tgt0 : 2;
    /* tad_ch_tgt0 - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt1 : 2;
    /* tad_ch_tgt1 - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt2 : 2;
    /* tad_ch_tgt2 - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt3 : 2;
    /* tad_ch_tgt3 - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-way TAD interleaving).
       
       This register is used in the IMC only for reverse address translation for 
       logging spare/patrol errors, converting a rank address back to a system address. 
     */
    UINT32 tad_ch_way : 2;
    /* tad_ch_way - Bits[9:8], RW_LB, default = 2'b00 
       channel interleave wayness
       00 - interleave across 1 channel or mirror pair
       01 - interleave across 2 channels or mirror pairs
       10 - interleave across 3 channels
       11 - interleave across 4 channels
       Note: This parameter effectively tells IMC how much to divide the system address 
       by when adjusting for the channel interleave. Since both channels in a pair 
       store every line of data, we want to divide by 1 when interleaving across one 
       pair and 2 when interleaving across two pairs. For HA, it tells how may channels 
       to distribute the read requests across. When we interleaving across 1 pair, we 
       want to distribute the reads to two channels, when interleaving across 2 pairs, 
       we distribute the reads across 4 pairs. Writes always go to both channels in the 
       pair when the read target is either channel. 
     */
    UINT32 tad_skt_way : 2;
    /* tad_skt_way - Bits[11:10], RW_LB, default = 2'b00 
       socket interleave wayness
       00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 tad_limit : 20;
    /* tad_limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       highest address of the range in system address space, 64MB granularity, i.e. 
       TADRANGLIMIT[45:26]. 
     */
  } Bits;
  UINT32 Data;
} TADWAYNESS_0_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* TADWAYNESS_1_MC_MAIN_REG supported on:                                     */
/*       IVT_EP (0x40178084)                                                  */
/*       IVT_EX (0x40178084)                                                  */
/*       HSX (0x40198084)                                                     */
/*       BDX (0x40198084)                                                     */
/* Register default value:              0x00000000                            */
#define TADWAYNESS_1_MC_MAIN_REG 0x0B004084
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * There are total of 12 TAD ranges (N+P+1= number of TAD ranges; P= how many times 
 * channel interleave changes within the SAD ranges.). 
 * 
 * Note for mirroring configuration:
 * For 1-way interleave, channel 0-2 mirror pair: target list = <0,2,x,x>, TAD ways 
 * = "00" 
 * For 1-way interleave, channel 1-3 mirror pair: target list = <1,3,x,x>, TAD ways 
 * = "00" 
 * For 2-way interleave, 0-2 mirror pair and 1-3 mirror pair: target list = 
 * <0,1,2,3>, TAD ways = "01" 
 * For 1-way interleave, lockstep + mirroring, target list = <0,2,x,x>, TAD ways = 
 * "00" 
 */
typedef union {
  struct {
    UINT32 tad_ch_tgt0 : 2;
    /* tad_ch_tgt0 - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt1 : 2;
    /* tad_ch_tgt1 - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt2 : 2;
    /* tad_ch_tgt2 - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt3 : 2;
    /* tad_ch_tgt3 - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-way TAD interleaving).
       
       This register is used in the IMC only for reverse address translation for 
       logging spare/patrol errors, converting a rank address back to a system address. 
     */
    UINT32 tad_ch_way : 2;
    /* tad_ch_way - Bits[9:8], RW_LB, default = 2'b00 
       channel interleave wayness
       00 - interleave across 1 channel or mirror pair
       01 - interleave across 2 channels or mirror pairs
       10 - interleave across 3 channels
       11 - interleave across 4 channels
       Note: This parameter effectively tells IMC how much to divide the system address 
       by when adjusting for the channel interleave. Since both channels in a pair 
       store every line of data, we want to divide by 1 when interleaving across one 
       pair and 2 when interleaving across two pairs. For HA, it tells how may channels 
       to distribute the read requests across. When we interleaving across 1 pair, we 
       want to distribute the reads to two channels, when interleaving across 2 pairs, 
       we distribute the reads across 4 pairs. Writes always go to both channels in the 
       pair when the read target is either channel. 
     */
    UINT32 tad_skt_way : 2;
    /* tad_skt_way - Bits[11:10], RW_LB, default = 2'b00 
       socket interleave wayness
       00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 tad_limit : 20;
    /* tad_limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       highest address of the range in system address space, 64MB granularity, i.e. 
       TADRANGLIMIT[45:26]. 
     */
  } Bits;
  UINT32 Data;
} TADWAYNESS_1_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* TADWAYNESS_2_MC_MAIN_REG supported on:                                     */
/*       IVT_EP (0x40178088)                                                  */
/*       IVT_EX (0x40178088)                                                  */
/*       HSX (0x40198088)                                                     */
/*       BDX (0x40198088)                                                     */
/* Register default value:              0x00000000                            */
#define TADWAYNESS_2_MC_MAIN_REG 0x0B004088
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * There are total of 12 TAD ranges (N+P+1= number of TAD ranges; P= how many times 
 * channel interleave changes within the SAD ranges.). 
 * 
 * Note for mirroring configuration:
 * For 1-way interleave, channel 0-2 mirror pair: target list = <0,2,x,x>, TAD ways 
 * = "00" 
 * For 1-way interleave, channel 1-3 mirror pair: target list = <1,3,x,x>, TAD ways 
 * = "00" 
 * For 2-way interleave, 0-2 mirror pair and 1-3 mirror pair: target list = 
 * <0,1,2,3>, TAD ways = "01" 
 * For 1-way interleave, lockstep + mirroring, target list = <0,2,x,x>, TAD ways = 
 * "00" 
 */
typedef union {
  struct {
    UINT32 tad_ch_tgt0 : 2;
    /* tad_ch_tgt0 - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt1 : 2;
    /* tad_ch_tgt1 - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt2 : 2;
    /* tad_ch_tgt2 - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt3 : 2;
    /* tad_ch_tgt3 - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-way TAD interleaving).
       
       This register is used in the IMC only for reverse address translation for 
       logging spare/patrol errors, converting a rank address back to a system address. 
     */
    UINT32 tad_ch_way : 2;
    /* tad_ch_way - Bits[9:8], RW_LB, default = 2'b00 
       channel interleave wayness
       00 - interleave across 1 channel or mirror pair
       01 - interleave across 2 channels or mirror pairs
       10 - interleave across 3 channels
       11 - interleave across 4 channels
       Note: This parameter effectively tells IMC how much to divide the system address 
       by when adjusting for the channel interleave. Since both channels in a pair 
       store every line of data, we want to divide by 1 when interleaving across one 
       pair and 2 when interleaving across two pairs. For HA, it tells how may channels 
       to distribute the read requests across. When we interleaving across 1 pair, we 
       want to distribute the reads to two channels, when interleaving across 2 pairs, 
       we distribute the reads across 4 pairs. Writes always go to both channels in the 
       pair when the read target is either channel. 
     */
    UINT32 tad_skt_way : 2;
    /* tad_skt_way - Bits[11:10], RW_LB, default = 2'b00 
       socket interleave wayness
       00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 tad_limit : 20;
    /* tad_limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       highest address of the range in system address space, 64MB granularity, i.e. 
       TADRANGLIMIT[45:26]. 
     */
  } Bits;
  UINT32 Data;
} TADWAYNESS_2_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* TADWAYNESS_3_MC_MAIN_REG supported on:                                     */
/*       IVT_EP (0x4017808C)                                                  */
/*       IVT_EX (0x4017808C)                                                  */
/*       HSX (0x4019808C)                                                     */
/*       BDX (0x4019808C)                                                     */
/* Register default value:              0x00000000                            */
#define TADWAYNESS_3_MC_MAIN_REG 0x0B00408C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * There are total of 12 TAD ranges (N+P+1= number of TAD ranges; P= how many times 
 * channel interleave changes within the SAD ranges.). 
 * 
 * Note for mirroring configuration:
 * For 1-way interleave, channel 0-2 mirror pair: target list = <0,2,x,x>, TAD ways 
 * = "00" 
 * For 1-way interleave, channel 1-3 mirror pair: target list = <1,3,x,x>, TAD ways 
 * = "00" 
 * For 2-way interleave, 0-2 mirror pair and 1-3 mirror pair: target list = 
 * <0,1,2,3>, TAD ways = "01" 
 * For 1-way interleave, lockstep + mirroring, target list = <0,2,x,x>, TAD ways = 
 * "00" 
 */
typedef union {
  struct {
    UINT32 tad_ch_tgt0 : 2;
    /* tad_ch_tgt0 - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt1 : 2;
    /* tad_ch_tgt1 - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt2 : 2;
    /* tad_ch_tgt2 - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt3 : 2;
    /* tad_ch_tgt3 - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-way TAD interleaving).
       
       This register is used in the IMC only for reverse address translation for 
       logging spare/patrol errors, converting a rank address back to a system address. 
     */
    UINT32 tad_ch_way : 2;
    /* tad_ch_way - Bits[9:8], RW_LB, default = 2'b00 
       channel interleave wayness
       00 - interleave across 1 channel or mirror pair
       01 - interleave across 2 channels or mirror pairs
       10 - interleave across 3 channels
       11 - interleave across 4 channels
       Note: This parameter effectively tells IMC how much to divide the system address 
       by when adjusting for the channel interleave. Since both channels in a pair 
       store every line of data, we want to divide by 1 when interleaving across one 
       pair and 2 when interleaving across two pairs. For HA, it tells how may channels 
       to distribute the read requests across. When we interleaving across 1 pair, we 
       want to distribute the reads to two channels, when interleaving across 2 pairs, 
       we distribute the reads across 4 pairs. Writes always go to both channels in the 
       pair when the read target is either channel. 
     */
    UINT32 tad_skt_way : 2;
    /* tad_skt_way - Bits[11:10], RW_LB, default = 2'b00 
       socket interleave wayness
       00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 tad_limit : 20;
    /* tad_limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       highest address of the range in system address space, 64MB granularity, i.e. 
       TADRANGLIMIT[45:26]. 
     */
  } Bits;
  UINT32 Data;
} TADWAYNESS_3_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* TADWAYNESS_4_MC_MAIN_REG supported on:                                     */
/*       IVT_EP (0x40178090)                                                  */
/*       IVT_EX (0x40178090)                                                  */
/*       HSX (0x40198090)                                                     */
/*       BDX (0x40198090)                                                     */
/* Register default value:              0x00000000                            */
#define TADWAYNESS_4_MC_MAIN_REG 0x0B004090
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * There are total of 12 TAD ranges (N+P+1= number of TAD ranges; P= how many times 
 * channel interleave changes within the SAD ranges.). 
 * 
 * Note for mirroring configuration:
 * For 1-way interleave, channel 0-2 mirror pair: target list = <0,2,x,x>, TAD ways 
 * = "00" 
 * For 1-way interleave, channel 1-3 mirror pair: target list = <1,3,x,x>, TAD ways 
 * = "00" 
 * For 2-way interleave, 0-2 mirror pair and 1-3 mirror pair: target list = 
 * <0,1,2,3>, TAD ways = "01" 
 * For 1-way interleave, lockstep + mirroring, target list = <0,2,x,x>, TAD ways = 
 * "00" 
 */
typedef union {
  struct {
    UINT32 tad_ch_tgt0 : 2;
    /* tad_ch_tgt0 - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt1 : 2;
    /* tad_ch_tgt1 - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt2 : 2;
    /* tad_ch_tgt2 - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt3 : 2;
    /* tad_ch_tgt3 - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-way TAD interleaving).
       
       This register is used in the IMC only for reverse address translation for 
       logging spare/patrol errors, converting a rank address back to a system address. 
     */
    UINT32 tad_ch_way : 2;
    /* tad_ch_way - Bits[9:8], RW_LB, default = 2'b00 
       channel interleave wayness
       00 - interleave across 1 channel or mirror pair
       01 - interleave across 2 channels or mirror pairs
       10 - interleave across 3 channels
       11 - interleave across 4 channels
       Note: This parameter effectively tells IMC how much to divide the system address 
       by when adjusting for the channel interleave. Since both channels in a pair 
       store every line of data, we want to divide by 1 when interleaving across one 
       pair and 2 when interleaving across two pairs. For HA, it tells how may channels 
       to distribute the read requests across. When we interleaving across 1 pair, we 
       want to distribute the reads to two channels, when interleaving across 2 pairs, 
       we distribute the reads across 4 pairs. Writes always go to both channels in the 
       pair when the read target is either channel. 
     */
    UINT32 tad_skt_way : 2;
    /* tad_skt_way - Bits[11:10], RW_LB, default = 2'b00 
       socket interleave wayness
       00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 tad_limit : 20;
    /* tad_limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       highest address of the range in system address space, 64MB granularity, i.e. 
       TADRANGLIMIT[45:26]. 
     */
  } Bits;
  UINT32 Data;
} TADWAYNESS_4_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* TADWAYNESS_5_MC_MAIN_REG supported on:                                     */
/*       IVT_EP (0x40178094)                                                  */
/*       IVT_EX (0x40178094)                                                  */
/*       HSX (0x40198094)                                                     */
/*       BDX (0x40198094)                                                     */
/* Register default value:              0x00000000                            */
#define TADWAYNESS_5_MC_MAIN_REG 0x0B004094
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * There are total of 12 TAD ranges (N+P+1= number of TAD ranges; P= how many times 
 * channel interleave changes within the SAD ranges.). 
 * 
 * Note for mirroring configuration:
 * For 1-way interleave, channel 0-2 mirror pair: target list = <0,2,x,x>, TAD ways 
 * = "00" 
 * For 1-way interleave, channel 1-3 mirror pair: target list = <1,3,x,x>, TAD ways 
 * = "00" 
 * For 2-way interleave, 0-2 mirror pair and 1-3 mirror pair: target list = 
 * <0,1,2,3>, TAD ways = "01" 
 * For 1-way interleave, lockstep + mirroring, target list = <0,2,x,x>, TAD ways = 
 * "00" 
 */
typedef union {
  struct {
    UINT32 tad_ch_tgt0 : 2;
    /* tad_ch_tgt0 - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt1 : 2;
    /* tad_ch_tgt1 - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt2 : 2;
    /* tad_ch_tgt2 - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt3 : 2;
    /* tad_ch_tgt3 - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-way TAD interleaving).
       
       This register is used in the IMC only for reverse address translation for 
       logging spare/patrol errors, converting a rank address back to a system address. 
     */
    UINT32 tad_ch_way : 2;
    /* tad_ch_way - Bits[9:8], RW_LB, default = 2'b00 
       channel interleave wayness
       00 - interleave across 1 channel or mirror pair
       01 - interleave across 2 channels or mirror pairs
       10 - interleave across 3 channels
       11 - interleave across 4 channels
       Note: This parameter effectively tells IMC how much to divide the system address 
       by when adjusting for the channel interleave. Since both channels in a pair 
       store every line of data, we want to divide by 1 when interleaving across one 
       pair and 2 when interleaving across two pairs. For HA, it tells how may channels 
       to distribute the read requests across. When we interleaving across 1 pair, we 
       want to distribute the reads to two channels, when interleaving across 2 pairs, 
       we distribute the reads across 4 pairs. Writes always go to both channels in the 
       pair when the read target is either channel. 
     */
    UINT32 tad_skt_way : 2;
    /* tad_skt_way - Bits[11:10], RW_LB, default = 2'b00 
       socket interleave wayness
       00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 tad_limit : 20;
    /* tad_limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       highest address of the range in system address space, 64MB granularity, i.e. 
       TADRANGLIMIT[45:26]. 
     */
  } Bits;
  UINT32 Data;
} TADWAYNESS_5_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* TADWAYNESS_6_MC_MAIN_REG supported on:                                     */
/*       IVT_EP (0x40178098)                                                  */
/*       IVT_EX (0x40178098)                                                  */
/*       HSX (0x40198098)                                                     */
/*       BDX (0x40198098)                                                     */
/* Register default value:              0x00000000                            */
#define TADWAYNESS_6_MC_MAIN_REG 0x0B004098
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * There are total of 12 TAD ranges (N+P+1= number of TAD ranges; P= how many times 
 * channel interleave changes within the SAD ranges.). 
 * 
 * Note for mirroring configuration:
 * For 1-way interleave, channel 0-2 mirror pair: target list = <0,2,x,x>, TAD ways 
 * = "00" 
 * For 1-way interleave, channel 1-3 mirror pair: target list = <1,3,x,x>, TAD ways 
 * = "00" 
 * For 2-way interleave, 0-2 mirror pair and 1-3 mirror pair: target list = 
 * <0,1,2,3>, TAD ways = "01" 
 * For 1-way interleave, lockstep + mirroring, target list = <0,2,x,x>, TAD ways = 
 * "00" 
 */
typedef union {
  struct {
    UINT32 tad_ch_tgt0 : 2;
    /* tad_ch_tgt0 - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt1 : 2;
    /* tad_ch_tgt1 - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt2 : 2;
    /* tad_ch_tgt2 - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt3 : 2;
    /* tad_ch_tgt3 - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-way TAD interleaving).
       
       This register is used in the IMC only for reverse address translation for 
       logging spare/patrol errors, converting a rank address back to a system address. 
     */
    UINT32 tad_ch_way : 2;
    /* tad_ch_way - Bits[9:8], RW_LB, default = 2'b00 
       channel interleave wayness
       00 - interleave across 1 channel or mirror pair
       01 - interleave across 2 channels or mirror pairs
       10 - interleave across 3 channels
       11 - interleave across 4 channels
       Note: This parameter effectively tells IMC how much to divide the system address 
       by when adjusting for the channel interleave. Since both channels in a pair 
       store every line of data, we want to divide by 1 when interleaving across one 
       pair and 2 when interleaving across two pairs. For HA, it tells how may channels 
       to distribute the read requests across. When we interleaving across 1 pair, we 
       want to distribute the reads to two channels, when interleaving across 2 pairs, 
       we distribute the reads across 4 pairs. Writes always go to both channels in the 
       pair when the read target is either channel. 
     */
    UINT32 tad_skt_way : 2;
    /* tad_skt_way - Bits[11:10], RW_LB, default = 2'b00 
       socket interleave wayness
       00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 tad_limit : 20;
    /* tad_limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       highest address of the range in system address space, 64MB granularity, i.e. 
       TADRANGLIMIT[45:26]. 
     */
  } Bits;
  UINT32 Data;
} TADWAYNESS_6_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* TADWAYNESS_7_MC_MAIN_REG supported on:                                     */
/*       IVT_EP (0x4017809C)                                                  */
/*       IVT_EX (0x4017809C)                                                  */
/*       HSX (0x4019809C)                                                     */
/*       BDX (0x4019809C)                                                     */
/* Register default value:              0x00000000                            */
#define TADWAYNESS_7_MC_MAIN_REG 0x0B00409C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * There are total of 12 TAD ranges (N+P+1= number of TAD ranges; P= how many times 
 * channel interleave changes within the SAD ranges.). 
 * 
 * Note for mirroring configuration:
 * For 1-way interleave, channel 0-2 mirror pair: target list = <0,2,x,x>, TAD ways 
 * = "00" 
 * For 1-way interleave, channel 1-3 mirror pair: target list = <1,3,x,x>, TAD ways 
 * = "00" 
 * For 2-way interleave, 0-2 mirror pair and 1-3 mirror pair: target list = 
 * <0,1,2,3>, TAD ways = "01" 
 * For 1-way interleave, lockstep + mirroring, target list = <0,2,x,x>, TAD ways = 
 * "00" 
 */
typedef union {
  struct {
    UINT32 tad_ch_tgt0 : 2;
    /* tad_ch_tgt0 - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt1 : 2;
    /* tad_ch_tgt1 - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt2 : 2;
    /* tad_ch_tgt2 - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt3 : 2;
    /* tad_ch_tgt3 - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-way TAD interleaving).
       
       This register is used in the IMC only for reverse address translation for 
       logging spare/patrol errors, converting a rank address back to a system address. 
     */
    UINT32 tad_ch_way : 2;
    /* tad_ch_way - Bits[9:8], RW_LB, default = 2'b00 
       channel interleave wayness
       00 - interleave across 1 channel or mirror pair
       01 - interleave across 2 channels or mirror pairs
       10 - interleave across 3 channels
       11 - interleave across 4 channels
       Note: This parameter effectively tells IMC how much to divide the system address 
       by when adjusting for the channel interleave. Since both channels in a pair 
       store every line of data, we want to divide by 1 when interleaving across one 
       pair and 2 when interleaving across two pairs. For HA, it tells how may channels 
       to distribute the read requests across. When we interleaving across 1 pair, we 
       want to distribute the reads to two channels, when interleaving across 2 pairs, 
       we distribute the reads across 4 pairs. Writes always go to both channels in the 
       pair when the read target is either channel. 
     */
    UINT32 tad_skt_way : 2;
    /* tad_skt_way - Bits[11:10], RW_LB, default = 2'b00 
       socket interleave wayness
       00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 tad_limit : 20;
    /* tad_limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       highest address of the range in system address space, 64MB granularity, i.e. 
       TADRANGLIMIT[45:26]. 
     */
  } Bits;
  UINT32 Data;
} TADWAYNESS_7_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* TADWAYNESS_8_MC_MAIN_REG supported on:                                     */
/*       IVT_EP (0x401780A0)                                                  */
/*       IVT_EX (0x401780A0)                                                  */
/*       HSX (0x401980A0)                                                     */
/*       BDX (0x401980A0)                                                     */
/* Register default value:              0x00000000                            */
#define TADWAYNESS_8_MC_MAIN_REG 0x0B0040A0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * There are total of 12 TAD ranges (N+P+1= number of TAD ranges; P= how many times 
 * channel interleave changes within the SAD ranges.). 
 * 
 * Note for mirroring configuration:
 * For 1-way interleave, channel 0-2 mirror pair: target list = <0,2,x,x>, TAD ways 
 * = "00" 
 * For 1-way interleave, channel 1-3 mirror pair: target list = <1,3,x,x>, TAD ways 
 * = "00" 
 * For 2-way interleave, 0-2 mirror pair and 1-3 mirror pair: target list = 
 * <0,1,2,3>, TAD ways = "01" 
 * For 1-way interleave, lockstep + mirroring, target list = <0,2,x,x>, TAD ways = 
 * "00" 
 */
typedef union {
  struct {
    UINT32 tad_ch_tgt0 : 2;
    /* tad_ch_tgt0 - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt1 : 2;
    /* tad_ch_tgt1 - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt2 : 2;
    /* tad_ch_tgt2 - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt3 : 2;
    /* tad_ch_tgt3 - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-way TAD interleaving).
       
       This register is used in the IMC only for reverse address translation for 
       logging spare/patrol errors, converting a rank address back to a system address. 
     */
    UINT32 tad_ch_way : 2;
    /* tad_ch_way - Bits[9:8], RW_LB, default = 2'b00 
       channel interleave wayness
       00 - interleave across 1 channel or mirror pair
       01 - interleave across 2 channels or mirror pairs
       10 - interleave across 3 channels
       11 - interleave across 4 channels
       Note: This parameter effectively tells IMC how much to divide the system address 
       by when adjusting for the channel interleave. Since both channels in a pair 
       store every line of data, we want to divide by 1 when interleaving across one 
       pair and 2 when interleaving across two pairs. For HA, it tells how may channels 
       to distribute the read requests across. When we interleaving across 1 pair, we 
       want to distribute the reads to two channels, when interleaving across 2 pairs, 
       we distribute the reads across 4 pairs. Writes always go to both channels in the 
       pair when the read target is either channel. 
     */
    UINT32 tad_skt_way : 2;
    /* tad_skt_way - Bits[11:10], RW_LB, default = 2'b00 
       socket interleave wayness
       00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 tad_limit : 20;
    /* tad_limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       highest address of the range in system address space, 64MB granularity, i.e. 
       TADRANGLIMIT[45:26]. 
     */
  } Bits;
  UINT32 Data;
} TADWAYNESS_8_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* TADWAYNESS_9_MC_MAIN_REG supported on:                                     */
/*       IVT_EP (0x401780A4)                                                  */
/*       IVT_EX (0x401780A4)                                                  */
/*       HSX (0x401980A4)                                                     */
/*       BDX (0x401980A4)                                                     */
/* Register default value:              0x00000000                            */
#define TADWAYNESS_9_MC_MAIN_REG 0x0B0040A4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * There are total of 12 TAD ranges (N+P+1= number of TAD ranges; P= how many times 
 * channel interleave changes within the SAD ranges.). 
 * 
 * Note for mirroring configuration:
 * For 1-way interleave, channel 0-2 mirror pair: target list = <0,2,x,x>, TAD ways 
 * = "00" 
 * For 1-way interleave, channel 1-3 mirror pair: target list = <1,3,x,x>, TAD ways 
 * = "00" 
 * For 2-way interleave, 0-2 mirror pair and 1-3 mirror pair: target list = 
 * <0,1,2,3>, TAD ways = "01" 
 * For 1-way interleave, lockstep + mirroring, target list = <0,2,x,x>, TAD ways = 
 * "00" 
 */
typedef union {
  struct {
    UINT32 tad_ch_tgt0 : 2;
    /* tad_ch_tgt0 - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt1 : 2;
    /* tad_ch_tgt1 - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt2 : 2;
    /* tad_ch_tgt2 - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt3 : 2;
    /* tad_ch_tgt3 - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-way TAD interleaving).
       
       This register is used in the IMC only for reverse address translation for 
       logging spare/patrol errors, converting a rank address back to a system address. 
     */
    UINT32 tad_ch_way : 2;
    /* tad_ch_way - Bits[9:8], RW_LB, default = 2'b00 
       channel interleave wayness
       00 - interleave across 1 channel or mirror pair
       01 - interleave across 2 channels or mirror pairs
       10 - interleave across 3 channels
       11 - interleave across 4 channels
       Note: This parameter effectively tells IMC how much to divide the system address 
       by when adjusting for the channel interleave. Since both channels in a pair 
       store every line of data, we want to divide by 1 when interleaving across one 
       pair and 2 when interleaving across two pairs. For HA, it tells how may channels 
       to distribute the read requests across. When we interleaving across 1 pair, we 
       want to distribute the reads to two channels, when interleaving across 2 pairs, 
       we distribute the reads across 4 pairs. Writes always go to both channels in the 
       pair when the read target is either channel. 
     */
    UINT32 tad_skt_way : 2;
    /* tad_skt_way - Bits[11:10], RW_LB, default = 2'b00 
       socket interleave wayness
       00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 tad_limit : 20;
    /* tad_limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       highest address of the range in system address space, 64MB granularity, i.e. 
       TADRANGLIMIT[45:26]. 
     */
  } Bits;
  UINT32 Data;
} TADWAYNESS_9_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* TADWAYNESS_10_MC_MAIN_REG supported on:                                    */
/*       IVT_EP (0x401780A8)                                                  */
/*       IVT_EX (0x401780A8)                                                  */
/*       HSX (0x401980A8)                                                     */
/*       BDX (0x401980A8)                                                     */
/* Register default value:              0x00000000                            */
#define TADWAYNESS_10_MC_MAIN_REG 0x0B0040A8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * There are total of 12 TAD ranges (N+P+1= number of TAD ranges; P= how many times 
 * channel interleave changes within the SAD ranges.). 
 * 
 * Note for mirroring configuration:
 * For 1-way interleave, channel 0-2 mirror pair: target list = <0,2,x,x>, TAD ways 
 * = "00" 
 * For 1-way interleave, channel 1-3 mirror pair: target list = <1,3,x,x>, TAD ways 
 * = "00" 
 * For 2-way interleave, 0-2 mirror pair and 1-3 mirror pair: target list = 
 * <0,1,2,3>, TAD ways = "01" 
 * For 1-way interleave, lockstep + mirroring, target list = <0,2,x,x>, TAD ways = 
 * "00" 
 */
typedef union {
  struct {
    UINT32 tad_ch_tgt0 : 2;
    /* tad_ch_tgt0 - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt1 : 2;
    /* tad_ch_tgt1 - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt2 : 2;
    /* tad_ch_tgt2 - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt3 : 2;
    /* tad_ch_tgt3 - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-way TAD interleaving).
       
       This register is used in the IMC only for reverse address translation for 
       logging spare/patrol errors, converting a rank address back to a system address. 
     */
    UINT32 tad_ch_way : 2;
    /* tad_ch_way - Bits[9:8], RW_LB, default = 2'b00 
       channel interleave wayness
       00 - interleave across 1 channel or mirror pair
       01 - interleave across 2 channels or mirror pairs
       10 - interleave across 3 channels
       11 - interleave across 4 channels
       Note: This parameter effectively tells IMC how much to divide the system address 
       by when adjusting for the channel interleave. Since both channels in a pair 
       store every line of data, we want to divide by 1 when interleaving across one 
       pair and 2 when interleaving across two pairs. For HA, it tells how may channels 
       to distribute the read requests across. When we interleaving across 1 pair, we 
       want to distribute the reads to two channels, when interleaving across 2 pairs, 
       we distribute the reads across 4 pairs. Writes always go to both channels in the 
       pair when the read target is either channel. 
     */
    UINT32 tad_skt_way : 2;
    /* tad_skt_way - Bits[11:10], RW_LB, default = 2'b00 
       socket interleave wayness
       00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 tad_limit : 20;
    /* tad_limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       highest address of the range in system address space, 64MB granularity, i.e. 
       TADRANGLIMIT[45:26]. 
     */
  } Bits;
  UINT32 Data;
} TADWAYNESS_10_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* TADWAYNESS_11_MC_MAIN_REG supported on:                                    */
/*       IVT_EP (0x401780AC)                                                  */
/*       IVT_EX (0x401780AC)                                                  */
/*       HSX (0x401980AC)                                                     */
/*       BDX (0x401980AC)                                                     */
/* Register default value:              0x00000000                            */
#define TADWAYNESS_11_MC_MAIN_REG 0x0B0040AC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * There are total of 12 TAD ranges (N+P+1= number of TAD ranges; P= how many times 
 * channel interleave changes within the SAD ranges.). 
 * 
 * Note for mirroring configuration:
 * For 1-way interleave, channel 0-2 mirror pair: target list = <0,2,x,x>, TAD ways 
 * = "00" 
 * For 1-way interleave, channel 1-3 mirror pair: target list = <1,3,x,x>, TAD ways 
 * = "00" 
 * For 2-way interleave, 0-2 mirror pair and 1-3 mirror pair: target list = 
 * <0,1,2,3>, TAD ways = "01" 
 * For 1-way interleave, lockstep + mirroring, target list = <0,2,x,x>, TAD ways = 
 * "00" 
 */
typedef union {
  struct {
    UINT32 tad_ch_tgt0 : 2;
    /* tad_ch_tgt0 - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt1 : 2;
    /* tad_ch_tgt1 - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt2 : 2;
    /* tad_ch_tgt2 - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-way TAD interleaving).
     */
    UINT32 tad_ch_tgt3 : 2;
    /* tad_ch_tgt3 - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-way TAD interleaving).
       
       This register is used in the IMC only for reverse address translation for 
       logging spare/patrol errors, converting a rank address back to a system address. 
     */
    UINT32 tad_ch_way : 2;
    /* tad_ch_way - Bits[9:8], RW_LB, default = 2'b00 
       channel interleave wayness
       00 - interleave across 1 channel or mirror pair
       01 - interleave across 2 channels or mirror pairs
       10 - interleave across 3 channels
       11 - interleave across 4 channels
       Note: This parameter effectively tells IMC how much to divide the system address 
       by when adjusting for the channel interleave. Since both channels in a pair 
       store every line of data, we want to divide by 1 when interleaving across one 
       pair and 2 when interleaving across two pairs. For HA, it tells how may channels 
       to distribute the read requests across. When we interleaving across 1 pair, we 
       want to distribute the reads to two channels, when interleaving across 2 pairs, 
       we distribute the reads across 4 pairs. Writes always go to both channels in the 
       pair when the read target is either channel. 
     */
    UINT32 tad_skt_way : 2;
    /* tad_skt_way - Bits[11:10], RW_LB, default = 2'b00 
       socket interleave wayness
       00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 tad_limit : 20;
    /* tad_limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       highest address of the range in system address space, 64MB granularity, i.e. 
       TADRANGLIMIT[45:26]. 
     */
  } Bits;
  UINT32 Data;
} TADWAYNESS_11_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* MCMTR2_MC_MAIN_REG supported on:                                           */
/*       IVT_EP (0x401780B0)                                                  */
/*       IVT_EX (0x401780B0)                                                  */
/*       HSX (0x401980B0)                                                     */
/*       BDX (0x401980B0)                                                     */
/* Register default value:              0x00000000                            */
#define MCMTR2_MC_MAIN_REG 0x0B0040B0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * MC Memory Technology Register 2.
 */
typedef union {
  struct {
    UINT32 monroe_chn_force_sr : 4;
    /* monroe_chn_force_sr - Bits[3:0], RW_L, default = 4'b0000 
       Channel Software Self-Refresh (Monroe) software channel force SRcontrol. When 
       set, the corresponding channel is ignoring the ForceSRExit. A new transaction 
       arrive at this channel will still cause the SR exit. 
     */
    UINT32 rsvd : 28;
    /* rsvd - Bits[31:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MCMTR2_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* MC_INIT_STATE_G_MC_MAIN_REG supported on:                                  */
/*       IVT_EP (0x401780B4)                                                  */
/*       IVT_EX (0x401780B4)                                                  */
/*       HSX (0x401980B4)                                                     */
/*       BDX (0x401980B4)                                                     */
/* Register default value on IVT_EP:    0x00000132                            */
/* Register default value on IVT_EX:    0x00000132                            */
/* Register default value on HSX:       0x00000122                            */
/* Register default value on BDX:       0x00000122                            */
#define MC_INIT_STATE_G_MC_MAIN_REG 0x0B0040B4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.0.CFG.xml.
 * This register defines the high-level behavior in CPGC mode. It defines the DDR 
 * reset pin value, DCLK enable, refresh enable and bits indicating the MRC status 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    UINT32 ddr_reset : 1;
    /* ddr_reset - Bits[1:1], RW_L, default = 1'b1 
       DDR reset for all DIMMs from all channels within this socket. No IMC/DDRIO logic 
       is reset by asserting this register. 
       It is important to note that this bit is negative logic! i.e. writing 0 to 
       induce a reset and write 1 for not reset. 
     */
    UINT32 dclk_enable : 1;
    /* dclk_enable - Bits[2:2], RW_L, default = 1'b0 
       DCLK Enable (for all channels)
     */
    UINT32 refresh_enable : 1;
    /* refresh_enable - Bits[3:3], RW_L, default = 1'b0 
       Refresh enable
       If cold reset, this bit should be set by bios after
       1) Initializing the refresh timing parameters
       2) Running DDR through reset and init sequence
       If warm reset or S3 exit, this bit should be set immediately after SR exit
     */
    UINT32 rsvd_4 : 1;
    /* rsvd_4 - Bits[4:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 reset_io : 1;
    /* reset_io - Bits[5:5], RW_L, default = 1'b1 
       DDR IO reset
       In order to reset the IO this bit has to be set for 20 DCLKs and then cleared. 
       Setting this bit will reset the DDRIO receive FIFO registers only. 
       It is required in some of the training steps
       It goes to both the left and right DDRIO blocks on MC on EN, EP, and EP4S 
       processor and only the left side DDRIO block on EX processor 
       Wait 40 nS before issuing and traffic after setting or clearing this bit.
     */
    UINT32 rsvd_6 : 1;
    /* rsvd_6 - Bits[6:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 mrc_done : 1;
    /* mrc_done - Bits[7:7], RW_L, default = 1'b0  */
    UINT32 safe_sr : 1;
    /* safe_sr - Bits[8:8], RWS_L, default = 1'b1 
       This bit indicates if it is safe to keep the MC in SR during MC-reset. If it is 
       clear when reset occurs, it means that the reset is without warning and the 
       DDR-reset should be asserted. If set when reset occurs, it indicates that DDR is 
       already in SR and it can keep it this way. This bit can also indicate MRC if 
       reset without warning has occured, and if it has, cold-reset flow should be 
       selected 
       Note to MRC BIOS: clear this bit at MRC entry.
     */
    UINT32 cs_oe_en : 4;
    /* cs_oe_en - Bits[12:9], RWS_L, default = 4'b0000  */
    UINT32 reset_vmse2to1 : 1;
    /* reset_vmse2to1 - Bits[13:13], RWS_L, default = 1'b0 
       Reset is used to set up Intel SMI 2 2:1 mode correctly
       in DDRIO. The register must be set and reset
       after the IMC mode register is configured to
       Intel SMI 2 2:1 mode.
     */
    UINT32 reset_io_vmse_rhs : 1;
    /* reset_io_vmse_rhs - Bits[14:14], RWS_L, default = 1'b0 
       Training Reset for DDRIO. It reset TX/RX FIFO pointers,
       and some read related FSms inside MCIO.
       Goes to channel 1 on EX processor. Make sure this bit is cleared before
       enabling DDRIO in Intel SMI 2 mode.
     */
    UINT32 rsvd_15 : 17;
    /* rsvd_15 - Bits[31:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MC_INIT_STATE_G_MC_MAIN_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RCOMP_TIMER_MC_MAIN_REG supported on:                                      */
/*       IVT_EP (0x401780C0)                                                  */
/*       IVT_EX (0x401780C0)                                                  */
/*       HSX (0x401980C0)                                                     */
/*       BDX (0x401980C0)                                                     */
/* Register default value:              0x00000C00                            */
#define RCOMP_TIMER_MC_MAIN_REG 0x0B0040C0


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.0.CFG.xml.
 * Defines the time from IO starting to run RCOMP evaluation until RCOMP results 
 * are defenetly ready. This counter is added in order to keep determinism of the 
 * process if operated in different modes 
 * The register also indicates that first RCOMP has been done - required by BIOS
 */
typedef union {
  struct {
    UINT32 count : 16;
    /* count - Bits[15:0], RW, default = 16'b0000110000000000 
       DCLK cycle count that MC needs to wait from the point it has triggered RCOMP 
       evaluation until it can trigger the load to registers 
       The setting need be doubled if any associated DDRIO has its CSR 
       GDCRCompOvr5.GDCPerChanCompCR set to 1 
       The setting need be further doubled if any associated DDRIO has its CSR 
       GDCRMscCtl.RCompFilterFreqCfg set to a non-zero value 
       Notice only in DDR1/DDR3 (EP) or VMSE0/1/3 the CSR settings are used. DDR0/DDR2 
       (EP) or VMSE2 (EX) are insignificant. 
     */
    UINT32 first_rcomp_done : 1;
    /* first_rcomp_done - Bits[16:16], RW_LV, default = 1'b0 
       This is a status bit that indicates the first RCOMP has been completed. It is 
       cleared on reset, and set by MC HW when the first RCOMP is completed. Bios 
       should wait until this bit is set before executing any DDR command 
       Locked by the inverted output of MCMAIN.PSMI_QSC_CNTL.FORCERW
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 no_mdll_fsm_override : 1;
    /* no_mdll_fsm_override - Bits[20:20], RW, default = 1'b0 
       Do not force DDRIO MDLL on during rcomp when set
     */
    UINT32 ignore_mdll_locked_bit : 1;
    /* ignore_mdll_locked_bit - Bits[21:21], RW, default = 1'b0 
       Ignore DDRIO MDLL lock status during rcomp when set
     */
    UINT32 rsvd_22 : 8;
    /* rsvd_22 - Bits[29:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rcomp : 1;
    /* rcomp - Bits[30:30], RW_LB, default = 1'b0 
       RCOMP start via message channel control for bios.
       RCOMP start only triggered when the register bit output is changing from 0 -> 1
       MC will not be responsible for clearing this bit. MC already provides feedback 
       to bias 
       when Rcomp is done via first_rcomp_done bit field.
     */
    UINT32 rcomp_in_progress : 1;
    /* rcomp_in_progress - Bits[31:31], RW_V, default = 1'b0 
       rcomp in progress status bit
     */
  } Bits;
  UINT32 Data;
} RCOMP_TIMER_MC_MAIN_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */






/* IMC_FUSE_DOWNLOAD2_SHADOW_MC_MAIN_REG supported on:                        */
/*       IVT_EP (0x401780CC)                                                  */
/*       IVT_EX (0x401780CC)                                                  */
/*       HSX (0x401980CC)                                                     */
/*       BDX (0x401980CC)                                                     */
/* Register default value:              0x00000000                            */
#define IMC_FUSE_DOWNLOAD2_SHADOW_MC_MAIN_REG 0x0B0040CC


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x0cc
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    /* rsvd_0 - Bits[0:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fuse_shadow_dis_uncore_ucr : 1;
    /* fuse_shadow_dis_uncore_ucr - Bits[1:1], RO_V, default = 1'b0 
       Disable UC recovery in the uncore
     */
    UINT32 rsvd_2 : 8;
    /* rsvd_2 - Bits[9:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fuse_shadow_physical_chop : 2;
    /* fuse_shadow_physical_chop - Bits[11:10], RO_V, default = 2'b00 
       Sets the physical chop of the die. LCC=0, MCC=1, HCC=2. MCC or HCC implies two 
       home agents. 
     */
    UINT32 rsvd_12 : 17;
    /* rsvd_12 - Bits[28:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fuse_shadow_disable_mem_ddr3 : 1;
    /* fuse_shadow_disable_mem_ddr3 - Bits[29:29], RO_V, default = 1'b0 
       Disable the use of ddr3 devices
     */
    UINT32 fuse_shadow_disable_ddr4_caparity : 1;
    /* fuse_shadow_disable_ddr4_caparity - Bits[30:30], RO_V, default = 1'b0 
       Force ddr4 command address parity to be zero. This requires DDR4 DRAMs to be 
       initialized to disable parity checking. 
     */
    UINT32 fuse_shadow_disable_mem_ddr4 : 1;
    /* fuse_shadow_disable_mem_ddr4 - Bits[31:31], RO_V, default = 1'b0 
       Disable the use of ddr4 devices
     */
  } Bits;
  UINT32 Data;
} IMC_FUSE_DOWNLOAD2_SHADOW_MC_MAIN_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x0cc
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    /* rsvd_0 - Bits[0:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fuse_shadow_dis_uncore_ucr : 1;
    /* fuse_shadow_dis_uncore_ucr - Bits[1:1], RO_V, default = 1'b0 
       Disable UC recovery in the uncore
     */
    UINT32 rsvd_2 : 7;
    /* rsvd_2 - Bits[8:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fuse_shadow_physical_chop : 3;
    /* fuse_shadow_physical_chop - Bits[11:9], RO_V, default = 3'b000 
       Sets the physical chop of the die. LCC=0, MCC=1, HCC=2, LCC-DE=4, HCC-DE=5. MCC 
       or HCC implies two home agents. 
     */
    UINT32 rsvd_12 : 17;
    /* rsvd_12 - Bits[28:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fuse_shadow_disable_mem_ddr3 : 1;
    /* fuse_shadow_disable_mem_ddr3 - Bits[29:29], RO_V, default = 1'b0 
       Disable the use of ddr3 devices
     */
    UINT32 fuse_shadow_disable_ddr4_caparity : 1;
    /* fuse_shadow_disable_ddr4_caparity - Bits[30:30], RO_V, default = 1'b0 
       Force ddr4 command address parity to be zero. This requires DDR4 DRAMs to be 
       initialized to disable parity checking. 
     */
    UINT32 fuse_shadow_disable_mem_ddr4 : 1;
    /* fuse_shadow_disable_mem_ddr4 - Bits[31:31], RO_V, default = 1'b0 
       Disable the use of ddr4 devices
     */
  } Bits;
  UINT32 Data;
} IMC_FUSE_DOWNLOAD2_SHADOW_MC_MAIN_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* MCBGFTUNE_MC_MAIN_REG supported on:                                        */
/*       IVT_EP (0x401780D0)                                                  */
/*       IVT_EX (0x401780D0)                                                  */
/*       HSX (0x401980D0)                                                     */
/*       BDX (0x401980D0)                                                     */
/* Register default value:              0x00000000                            */
#define MCBGFTUNE_MC_MAIN_REG 0x0B0040D0


#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * Usage model:  BIOS or ITP or device driver programs HABGFTUNE and MCBGFTUNE 
 * registers.  The values are Memory Frequency dependent.  This feature must be 
 * disabled by programming URatio field to zero if Memory frequency is going to 
 * change.  The values are calculated to minimize delay through bgf while meeting 
 * setup time requirements.  (design team has a program which can do those 
 * calculations).  It is programmed for the sweet-spot URatio for a particular 
 * system.  The parameters in the tuning register override the default values (from 
 * PMA) if the URATIO matches and are sampled on the assertion edge of BgfRun.  
 * Both HABGFTUNE and MCBGFTUNE must be programmed correctly and consistently to 
 * avoid possible malfunction. 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 19;
    UINT32 ha_bandwidth_fix : 1;
    /* ha_bandwidth_fix - Bits[19:19], RWS_L, default = 1'b0 
       Bit needs to be set high along with HA programming. // chgbdx
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MCBGFTUNE_MC_MAIN_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* PXPENHCAP_MC_MAIN_REG supported on:                                        */
/*       IVT_EP (0x40178100)                                                  */
/*       IVT_EX (0x40178100)                                                  */
/*       HSX (0x40198100)                                                     */
/*       BDX (0x40198100)                                                     */
/* Register default value:              0x00000000                            */
#define PXPENHCAP_MC_MAIN_REG 0x0B004100
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
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
} PXPENHCAP_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* MH_MAINCNTL_MC_MAIN_REG supported on:                                      */
/*       IVT_EP (0x40178104)                                                  */
/*       IVT_EX (0x40178104)                                                  */
/*       HSX (0x40198104)                                                     */
/*       BDX (0x40198104)                                                     */
/* Register default value:              0x0000E01F                            */
#define MH_MAINCNTL_MC_MAIN_REG 0x0B004104


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x104
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 15;
    UINT32 mh_output_en : 1;
    /* mh_output_en - Bits[15:15], RW_LB, default = 1'b1 
       MEM_HOT output generation logic enable control.
       
       When 0, the MEM_HOT output generation logic is disabled, i.e. MEM_HOT[1:0]# 
       outputs are in de-asserted state, no assertion regardless of the memory 
       temperature. Sensing of externally asserted MEM_HOT[1:0]# is not affected by 
       this bit. IMC will always reset the MH1_DIMM_VAL and MH0_DIMM_VAL bits in the 
       next DCLK. 
       When 1, the MEM_HOT output generation logic is enabled.
     */
    UINT32 mh_sense_en : 1;
    /* mh_sense_en - Bits[16:16], RW_LB, default = 1'b0 
       Externally asserted MEM_HOTsense control enable bit.
       
       When set, the MEM_HOT sense logic is enabled.
     */
    UINT32 mhot_smi_en : 1;
    /* mhot_smi_en - Bits[17:17], RW, default = 1'b0 
       This bit is enables the MHOT_SMI_EXT_EN.
       Generate SMI when either MEMHOT[1:0]# is EXTERNALLY asserted.
     */
    UINT32 mhot_ext_smi_en : 1;
    /* mhot_ext_smi_en - Bits[18:18], RW, default = 1'b0 
       This bit is enables the MHOT_SMI_EN.
       Generate SMI during INTERNAL MEMHOT# event assertion.
     */
    UINT32 smbus_01src_sel_0 : 1;
    /* smbus_01src_sel_0 - Bits[19:19], RW, default = 1'b0 
       When set to one selects SMBus source input 0 for channels 0/1. When zero selects 
       SMBus source input 1A. 
     */
    UINT32 smbus_23src_sel_1a : 1;
    /* smbus_23src_sel_1a - Bits[20:20], RW, default = 1'b0 
       When set to one selects SMBus source input 1A for channels 2/3. When zero 
       selects SMBus source input 1B. 
     */
    UINT32 rsvd : 11;
    /* rsvd - Bits[31:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MH_MAINCNTL_MC_MAIN_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */






/* MH_SENSE_500NS_CFG_MC_MAIN_REG supported on:                               */
/*       IVT_EP (0x4017810C)                                                  */
/*       IVT_EX (0x4017810C)                                                  */
/*       HSX (0x4019810C)                                                     */
/*       BDX (0x4019810C)                                                     */
/* Register default value:              0x00C84190                            */
#define MH_SENSE_500NS_CFG_MC_MAIN_REG 0x0B00410C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x10c
 */
typedef union {
  struct {
    UINT32 cnfg_500_nanosec : 10;
    /* cnfg_500_nanosec - Bits[9:0], RWS_L, default = 10'b0110010000 
       500ns equivalent in DCLK. BIOS calculate number of DCLK to be equvalent to 500 
       nanoseconds. This value is loaded into CNTR_500_NANOSEC when it is decremented 
       to zero. 
       The following are the recommended CNFG_500_NANOSEC values based from each DCLK 
       frequency: 
       DCLK=400MHz, CNFG_500_NANOSEC=0C8h
       DCLK=533MHz, CNFG_500_NANOSEC=10Ah
       DCLK=667MHz, CNFG_500_NANOSEC=14Dh
       DCLK=800MHz, CNFG_500_NANOSEC=190h
       DCLK=933MHz, CNFG_500_NANOSEC=1D2h
     */
    UINT32 rsvd_10 : 3;
    /* rsvd_10 - Bits[12:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 mh_in_sense_assert : 3;
    /* mh_in_sense_assert - Bits[15:13], RW, default = 3'b010 
       MEMHOT Input Sense Assertion Time in number of CNTR_500_NANOSEC. BIOS calculate 
       number of CNFG_500_NANOSEC for 1usec/2usec input_sense duration 
       
       Here is MH_IN_SENSE_ASSERT ranges:
       0 or 1 Reserved
       2 - 7 1usec - 3.5usec sense assertion time in 500nsec increment
     */
    UINT32 mh_sense_period : 10;
    /* mh_sense_period - Bits[25:16], RW, default = 10'b0011001000 
       MEMHOT Input Sense Period in number of CNTR_500_NANOSEC. BIOS calculate number 
       of CNTR_500_NANOSEC for 50usec/100usec/200usec/400usec. 
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MH_SENSE_500NS_CFG_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* MH_DTYCYC_MIN_ASRT_CNTR_0_MC_MAIN_REG supported on:                        */
/*       IVT_EP (0x40178110)                                                  */
/*       IVT_EX (0x40178110)                                                  */
/*       HSX (0x40198110)                                                     */
/*       BDX (0x40198110)                                                     */
/* Register default value:              0x00000000                            */
#define MH_DTYCYC_MIN_ASRT_CNTR_0_MC_MAIN_REG 0x0B004110
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x110
 */
typedef union {
  struct {
    UINT32 mh_duty_cyc_prd_cntr : 20;
    /* mh_duty_cyc_prd_cntr - Bits[19:0], RW_LV, default = 20'b00000000000000000000 
       MEM_HOT[1:0]# DUTY Cycle Period Current Count in number of CNTR_500_NANOSEC 
       (decrement by 1 every CNTR_500_NANOSEC). When the counter is zero, the next 
       cycle is loaded with MH_DUTY_CYC_PRD. PMSI pause (at quiencense) and resume (at 
       wipe) 
     */
    UINT32 mh_min_asrtn_cntr : 12;
    /* mh_min_asrtn_cntr - Bits[31:20], RO_V, default = 12'b000000000000 
       MEM_HOT[1:0]# Minimum Assertion Time Current Count in number of CNTR_500_NANOSEC 
       (decrement by 1 every CNTR_500_NANOSEC). When the counter is zero, the counter 
       is remain at zero and it is only loaded with MH_MIN_ASRTN only when 
       MH_DUTY_CYC_PRD_CNTR is reloaded. 
     */
  } Bits;
  UINT32 Data;
} MH_DTYCYC_MIN_ASRT_CNTR_0_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* MH_DTYCYC_MIN_ASRT_CNTR_1_MC_MAIN_REG supported on:                        */
/*       IVT_EP (0x40178114)                                                  */
/*       IVT_EX (0x40178114)                                                  */
/*       HSX (0x40198114)                                                     */
/*       BDX (0x40198114)                                                     */
/* Register default value:              0x00000000                            */
#define MH_DTYCYC_MIN_ASRT_CNTR_1_MC_MAIN_REG 0x0B004114
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x114
 */
typedef union {
  struct {
    UINT32 mh_duty_cyc_prd_cntr : 20;
    /* mh_duty_cyc_prd_cntr - Bits[19:0], RW_LV, default = 20'b00000000000000000000 
       MEM_HOT[1:0]# DUTY Cycle Period Current Count in number of CNTR_500_NANOSEC 
       (decrement by 1 every CNTR_500_NANOSEC). When the counter is zero, the next 
       cycle is loaded with MH_DUTY_CYC_PRD. PMSI pause (at quiencense) and resume (at 
       wipe) 
     */
    UINT32 mh_min_asrtn_cntr : 12;
    /* mh_min_asrtn_cntr - Bits[31:20], RO_V, default = 12'b000000000000 
       MEM_HOT[1:0]# Minimum Assertion Time Current Count in number of CNTR_500_NANOSEC 
       (decrement by 1 every CNTR_500_NANOSEC). When the counter is zero, the counter 
       is remain at zero and it is only loaded with MH_MIN_ASRTN only when 
       MH_DUTY_CYC_PRD_CNTR is reloaded. 
     */
  } Bits;
  UINT32 Data;
} MH_DTYCYC_MIN_ASRT_CNTR_1_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* MH_IO_500NS_CNTR_MC_MAIN_REG supported on:                                 */
/*       IVT_EP (0x40178118)                                                  */
/*       IVT_EX (0x40178118)                                                  */
/*       HSX (0x40198118)                                                     */
/*       BDX (0x40198118)                                                     */
/* Register default value:              0x00000000                            */
#define MH_IO_500NS_CNTR_MC_MAIN_REG 0x0B004118
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x118
 */
typedef union {
  struct {
    UINT32 cntr_500_nanosec : 10;
    /* cntr_500_nanosec - Bits[9:0], RW_LV, default = 10'b0000000000 
       500ns base counters used for the MEM_HOT counters and the SMBus counters. BIOS 
       calculate number of DCLK to be equvalent to 500 nanoseconds. CNTR_500_NANOSEC 
       (hardware will decrement this register by 1 every CNTR_500_NANOSEC). When the 
       counter is zero, the next CNTR_500_NANOSEC count is loaded with 
       CNFG_500_NANOSEC. This counter is subject to PMSI pause (at quiencense) and 
       resume (at wipe). 
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[11:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 mh0_io_cntr : 10;
    /* mh0_io_cntr - Bits[21:12], RW_LV, default = 10'b0000000000 
       MEM_HOT[1:0]# Input Output Counter in number of CNTR_500_NANOSEC. When 
       MH_IO_CNTR is zero, the counter is loaded with MH_SENSE_PERIOD in the next 
       CNTR_500_NANOSEC. When count is greater than MH_IN_SENSE_ASSERT, the 
       MEM_HOT[1:0]# output driver may be turn on if the corresponding MEM_HOT#event is 
       asserted. The receiver is turned off during this time. When count is equal or 
       less than MH_IN_SENSE_ASSERT, MEM_HOT[1:0]# output is disabled and receiver is 
       turned on. BIOS calculate number of CNTR_500_NANOSEC (hardware will decrement 
       this register by 1 every CNTR_500_NANOSEC). When the counter is zero, the next 
       CNTR_500_NANOSEC count is loaded with MH_IN_SENSE_ASSERT. This counter is 
       subject to PMSI pause (at quiencense) and resume (at wipe). 
     */
    UINT32 mh1_io_cntr : 10;
    /* mh1_io_cntr - Bits[31:22], RW_LV, default = 10'b0000000000 
       MEM_HOT[1:0]# Input Output Counter in number of CNTR_500_NANOSEC. When 
       MH0_IO_CNTR is zero, the counter is loaded with MH_SENSE_PERIOD in the next 
       CNTR_500_NANOSEC. When count is greater than MH_IN_SENSE_ASSERT, the MEM_HOT[1]# 
       output driver may be turn on if the corresponding MEM_HOT#event is asserted. The 
       receiver is turned off during this time. When count is equal or less than 
       MH_IN_SENSE_ASSERT, MEM_HOT[1:0]# output is disabled and receiver is turned on. 
       Hardware will decrement this counter by 1 every time CNTR_500_NANOSEC is 
       decremented to zero. When the counter is zero, the next CNFG_500_NANOSEC count 
       is loaded with MH_IN_SENSE_ASSERT. This counter is subject to PMSI pause (at 
       quiencense) and resume (at wipe). 
     */
  } Bits;
  UINT32 Data;
} MH_IO_500NS_CNTR_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* MH_CHN_ASTN_MC_MAIN_REG supported on:                                      */
/*       IVT_EP (0x4017811C)                                                  */
/*       IVT_EX (0x4017811C)                                                  */
/*       HSX (0x4019811C)                                                     */
/*       BDX (0x4019811C)                                                     */
/* Register default value:              0x00BA0098                            */
#define MH_CHN_ASTN_MC_MAIN_REG 0x0B00411C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x11c
 */
typedef union {
  struct {
    UINT32 mh0_1st_chn_astn : 4;
    /* mh0_1st_chn_astn - Bits[3:0], RO, default = 4'b1000 
       MemHot[0]# 1st Channel Associationbit 3: is valid bit. Note: Valid bit means the 
       association is valid and it does not implies the channel is populated or exist. 
       bit 2-0: 1st channel ID within this MEMHOT domain
     */
    UINT32 mh0_2nd_chn_astn : 4;
    /* mh0_2nd_chn_astn - Bits[7:4], RO, default = 4'b1001 
       MemHot[0]# 2nd Channel Associationbit 7: is valid bit. Note: Valid bit means the 
       association is valid and it does not implies the channel is populated. 
       bit 6-4: 2nd channel ID within this MEMHOT domain
     */
    UINT32 rsvd_8 : 8;
    /* rsvd_8 - Bits[15:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 mh1_1st_chn_astn : 4;
    /* mh1_1st_chn_astn - Bits[19:16], RO, default = 4'b1010 
       MemHot[1]# 1st Channel Associationbit 19: is valid bit. Note: Valid bit means 
       the association is valid and it does not implies the channel is populated. 
       bit 18-16: 1st channel ID within this MEMHOT domain
     */
    UINT32 mh1_2nd_chn_astn : 4;
    /* mh1_2nd_chn_astn - Bits[23:20], RO, default = 4'b1011 
       MemHot[1]# 2nd Channel Associationbit 23: is valid bit. Note: Valid bit means 
       the association is valid and it does not implies the channel is populated. 
       bit 22-20: 2nd channel ID within this MEMHOT domain.
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MH_CHN_ASTN_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* MH_TEMP_STAT_MC_MAIN_REG supported on:                                     */
/*       IVT_EP (0x40178120)                                                  */
/*       IVT_EX (0x40178120)                                                  */
/*       HSX (0x40198120)                                                     */
/*       BDX (0x40198120)                                                     */
/* Register default value:              0x00000000                            */
#define MH_TEMP_STAT_MC_MAIN_REG 0x0B004120
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x120
 */
typedef union {
  struct {
    UINT32 mh0_temp : 8;
    /* mh0_temp - Bits[7:0], RW, default = 8'b00000000 
       Hottest DIMM Sensor Reading for MEM_HOT[0]# - This reading represents the 
       temperature of the hottest DIMM. PCODE search the hottest DIMM temperature and 
       write the hottest temperature and the corresponding Hottest DIMM CID/ID. Note: 
       IMC hardware load this value into the MEM_HOT duty cycle generator counter since 
       PCode may update this field at different rate/time. This field is ranged from 0 
       to 127, i.e. the most significant bit is always zero. 
     */
    UINT32 mh0_dimm_id : 4;
    /* mh0_dimm_id - Bits[11:8], RW, default = 4'b0000 
       Hottest DIMM ID for MEM_HOT[0]#. PCODE search the hottest DIMM temperature and 
       write the hottest temperature and the corresponding Hottest DIMM CID/ID. 
     */
    UINT32 mh0_dimm_cid : 3;
    /* mh0_dimm_cid - Bits[14:12], RW, default = 3'b000 
       Hottest DIMM Channel ID for MEM_HOT[0]#. PCODE search the hottest DIMM 
       temperature and write the hottest temperature and the corresponding Hottest DIMM 
       CID/ID. 
     */
    UINT32 mh0_dimm_val : 1;
    /* mh0_dimm_val - Bits[15:15], RW_V, default = 1'b0 
       Valid if set. PCODE search the hottest DIMM temperature and write the hottest 
       temperature and the corresponding Hottest DIMM CID/ID and set the valid bit. 
       MEMHOT hardware logic process the corresponding MEMHOT data when there is a 
       MEMHOT event. Upon processing, the valid bit is reset. PCODE can write over 
       existing valid temparature since a valid temperature may not occur during a 
       MEMHOT event. If PCODE set the valid bit occur at the same cycle that the MEMHOT 
       logic processing and try to clear, the PCODE set will dominate since it is a new 
       temperature is updated while processing logic tries to clear an existing 
       temperature. 
     */
    UINT32 mh1_temp : 8;
    /* mh1_temp - Bits[23:16], RW, default = 8'b00000000 
       Hottest DIMM Sensor Reading for MEM_HOT[1]# - This reading represents the 
       temperature of the hottest DIMM. PCODE search the hottest DIMM temperature and 
       write the hottest temperature and the corresponding Hottest DIMM CID/ID. Note: 
       IMC hardware load this value into the MEM_HOT duty cycle generator counter since 
       PCode may update this field at different rate/time. This field is ranged from 0 
       to 127, i.e. the most significant bit is always zero. 
     */
    UINT32 mh1_dimm_id : 4;
    /* mh1_dimm_id - Bits[27:24], RW, default = 4'b0000 
       Hottest DIMM ID for MEM_HOT[1]#. PCODE search the hottest DIMM temperature and 
       write the hottest temperature and the corresponding Hottest DIMM CID/ID. 
     */
    UINT32 mh1_dimm_cid : 3;
    /* mh1_dimm_cid - Bits[30:28], RW, default = 3'b000 
       Hottest DIMM Channel ID for MEM_HOT[1]#. PCODE search the hottest DIMM 
       temperature and write the hottest temperature and the corresponding Hottest DIMM 
       CID/ID. 
     */
    UINT32 mh1_dimm_val : 1;
    /* mh1_dimm_val - Bits[31:31], RW_V, default = 1'b0 
       Valid if set. PCODE search the hottest DIMM temperature and write the hottest 
       temperature and the corresponding Hottest DIMM CID/ID and set the valid bit. 
       MEMHOT hardware logic process the corresponding MEMHOT data when there is a 
       MEMHOT event. Upon processing, the valid bit is reset. PCODE can write over 
       existing valid temparature since a valid temperature may not occur during a 
       MEMHOT event. If PCODE set the valid bit occur at the same cycle that the MEMHOT 
       logic processing and try to clear, the PCODE set will dominate since it is a new 
       temperature is updated while processing logic tries to clear an existing 
       temperature. 
     */
  } Bits;
  UINT32 Data;
} MH_TEMP_STAT_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* MH_EXT_STAT_MC_MAIN_REG supported on:                                      */
/*       IVT_EP (0x40178124)                                                  */
/*       IVT_EX (0x40178124)                                                  */
/*       HSX (0x40198124)                                                     */
/*       BDX (0x40198124)                                                     */
/* Register default value:              0x00000000                            */
#define MH_EXT_STAT_MC_MAIN_REG 0x0B004124
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * Capture externally asserted MEM_HOT[1:0]# assertion detection.
 */
typedef union {
  struct {
    UINT32 mh_ext_stat_0 : 1;
    /* mh_ext_stat_0 - Bits[0:0], RW1C, default = 1'b0 
       MEM_HOT[0]# assertion status at this sense period.
       Set if MEM_HOT[0]# is asserted externally for this sense period, this running 
       status bit will automatically updated with the next sensed value in the next 
       MEM_HOT input sense phase. 
     */
    UINT32 mh_ext_stat_1 : 1;
    /* mh_ext_stat_1 - Bits[1:1], RW1C, default = 1'b0 
       MEM_HOT[1]# assertion status at this sense period.
       Set if MEM_HOT[1]# is asserted externally for this sense period, this running 
       status bit will automatically updated with the next sensed value in the next 
       MEM_HOT input sense phase. 
     */
    UINT32 rsvd : 30;
    /* rsvd - Bits[31:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MH_EXT_STAT_MC_MAIN_STRUCT;
#endif /* ASM_INC */






/* SMB_STAT_0_MC_MAIN_REG supported on:                                       */
/*       IVT_EP (0x40178180)                                                  */
/*       IVT_EX (0x40178180)                                                  */
/*       HSX (0x40198180)                                                     */
/*       BDX (0x40198180)                                                     */
/* Register default value:              0x07000000                            */
#define SMB_STAT_0_MC_MAIN_REG 0x0B004180
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * This register provides the interface to the SMBus/I2C (SCL and SDA signals) that 
 * is used to access the Serial Presence Detect EEPROM or Thermal Sensor on DIMM 
 * (TSOD) that defines the technology, configuration, and speed of the DIMMs 
 * controlled by IMC. 
 */
typedef union {
  struct {
    UINT32 smb_rdata : 16;
    /* smb_rdata - Bits[15:0], RO_V, default = 16'b0000000000000000 
       Read DataHolds data read from SMBus Read commands.
       
       Since TSOD/EEPROM are I2C devices and the byte order is MSByte first in a word 
       read, reading of I2C using word read should return SMB_RDATA[15:8]=I2C_MSB and 
       SMB_RDATA[7:0]=I2C_LSB. If reading of I2C using byte read, the 
       SMB_RDATA[15:8]=dont care; SMB_RDATA[7:0]=read_byte. 
       If we have a SMB slave connected on the bus , reading of the SMBus slave using 
       word read should return SMB_RDATA[15:8]=SMB_LSB and SMB_RDATA[7:0]=SMB_MSB. 
       
       If the software is not sure whether the target is I2C or SMBus slave, please use 
       byte access. 
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[23:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tsod_sa : 4;
    /* tsod_sa - Bits[27:24], RO_V, default = 4'b0111 
       This field captures the last issued TSOD slave address. Here is the slave 
       address and the DDR CHN and DIMM slot mapping: 
       Slave Address: 0 -- Channel: Even Chn; Slot #: 0
       Slave Address: 1 -- Channel: Even Chn; Slot #: 1
       Slave Address: 2 -- Channel: Even Chn; Slot #: 2
       Slave Address: 3 -- Channel: Even Chn; Slot #: 3 (reserved for future use)
       Slave Address: 4 -- Channel: Odd Chn; Slot #: 0
       Slave Address: 5 -- Channel: Odd Chn; Slot #: 1
       Slave Address: 6 -- Channel: Odd Chn; Slot #: 2
       Slave Address: 7 -- Channel: Odd Chn; Slot #: 3 (reserved for future use)
       A value of 8 in this register indicates to poll scalable memory buffer 
       temperature rather than a DIMM temperature, values above 0x8 are invalid. 
       Since this field only captures the TSOD polling slave address. During SMB error 
       handling, software should check the hung SMB_TSOD_POLL_EN state before disabling 
       the SMB_TSOD_POLL_EN in order to qualify whether this field is valid. 
     */
    UINT32 smb_busy : 1;
    /* smb_busy - Bits[28:28], ROS_V, default = 1'b0 
       SMBus Busy state. This bit is set by IMC while an SMBus/I2C command (including 
       TSOD command issued from IMC hardware) is executing. Any transaction that is 
       completed normally or gracefully will clear this bit automatically. By setting 
       the SMB_SOFT_RST will also clear this bit. 
       
       This register bit is sticky across reset so any surprise reset during pending 
       SMBus operation will sustain the bit assertion across surprised warm-reset. BIOS 
       reset handler can read this bit before issuing any SMBus transaction to 
       determine whether a slave device may need special care to force the slave to 
       idle state (e.g. via clock override toggling (SMB_CKOVRD) and/or via induced 
       time-out by asserting SMB_CKOVRD for 25-35ms). 
     */
    UINT32 smb_sbe : 1;
    /* smb_sbe - Bits[29:29], RO_V, default = 1'b0 
       SMBus Error
       This bit is set by IMC if an SMBus transaction (including the TSOD polling or 
       message channel initiated SMBus access) that does not complete successfully 
       (non-Ack has been received from slave at expected Ack slot of the transfer). If 
       a slave device is asserting clock stretching, IMC does not have logic to detect 
       this condition to set the SBE bit directly; however, the SMBus master will 
       detect the error at the corresponding transaction's expected ACK slot. 
       Once SMBUS_SBE bit is set, IMC stops issuing hardware initiated TSOD polling 
       SMBUS transactions until the SMB_SBE is cleared. IMC will not increment the 
       SMB_STAT_x.TSOD_SA until the SMB_SBE is cleared. Manual SMBus command interface 
       is not affected, i.e. new command issue will clear the SMB_SBE. 
     */
    UINT32 smb_wod : 1;
    /* smb_wod - Bits[30:30], RO_V, default = 1'b0 
       Write Operation Done
       This bit is set by IMC when a SMBus Write command has been completed on the 
       SMBus. It is cleared by IMC when a subsequent SMBus Write command is issued. 
     */
    UINT32 smb_rdo : 1;
    /* smb_rdo - Bits[31:31], RO_V, default = 1'b0 
       Read Data Valid
       This bit is set by IMC when the Data field of this register receives read data 
       from the SPD/TSOD after completion of an SMBus read command. It is cleared by 
       IMC when a subsequent SMBus read command is issued. 
     */
  } Bits;
  UINT32 Data;
} SMB_STAT_0_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* SMBCMD_0_MC_MAIN_REG supported on:                                         */
/*       IVT_EP (0x40178184)                                                  */
/*       IVT_EX (0x40178184)                                                  */
/*       HSX (0x40198184)                                                     */
/*       BDX (0x40198184)                                                     */
/* Register default value:              0x00000000                            */
#define SMBCMD_0_MC_MAIN_REG 0x0B004184
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * A write to this register initiates a DIMM EEPROM access through the SMBus/I2C.
 */
typedef union {
  struct {
    UINT32 smb_wdata : 16;
    /* smb_wdata - Bits[15:0], RWS, default = 16'b0000000000000000 
       Write Data: Holds data to be written by SPDW commands.
       
       Since TSOD/EEPROM are I2C devices and the byte order is MSByte first in a word 
       write, writing of I2C using word write should use SMB_WDATA[15:8]=I2C_MSB and 
       SMB_WDATA[7:0]=I2C_LSB. If writing of I2C using byte write, the 
       SMB_WDATA[15:8]=dont care; SMB_WDATA[7:0]=write_byte. 
       If we have a SMB slave connected on the bus , writing of the SMBus slave using 
       word write should use SMB_WDATA[15:8]=SMB_LSB and SMB_WDATA[7:0]=SMB_MSB. 
       It is software responsibility to figure out the byte order of the slave access.
     */
    UINT32 smb_ba : 8;
    /* smb_ba - Bits[23:16], RWS, default = 8'b00000000 
       Bus Txn Address: This field identifies the bus transaction address to be 
       accessed. 
       Note: in WORD access, 23:16 specifies 2B access address. In Byte access, 23:16 
       specified 1B access address. 
     */
    UINT32 smb_sa : 3;
    /* smb_sa - Bits[26:24], RWS, default = 3'b000 
       Slave Address: This field identifies the DIMM SPD/TSOD to be accessed.
     */
    UINT32 smb_wrt_cmd : 1;
    /* smb_wrt_cmd - Bits[27:27], RWS, default = 1'b0 
       When '0', it's a read command
       When '1', it's a write command
     */
    UINT32 smb_wrt_pntr : 1;
    /* smb_wrt_pntr - Bits[28:28], RWS, default = 1'b0 
       Bit[28:27]=00: SMBus Read
       
       Bit[28:27]=01: SMBus Write
       
       Bit[28:27]=10: illegal combination
       
       Bit[28:27]=11: Write to pointer registerSMBus/I2C pointer update (byte). bit 30, 
       and 29 are ignored. Note: SMBCntl_[0:1][26] will NOT disable WrtPntr update 
       command. 
     */
    UINT32 smb_word_access : 1;
    /* smb_word_access - Bits[29:29], RWS, default = 1'b0 
       word access: SMBus/I2C word (2B) access when set; otherwise, it is a byte 
       access. 
     */
    UINT32 smb_pntr_sel : 1;
    /* smb_pntr_sel - Bits[30:30], RWS, default = 1'b0 
       Pointer Selection: SMBus/I2C present pointer based access enable when set; 
       otherwise, use random access protocol. Hardware based TSOD polling will also use 
       this bit to enable the pointer word read. 
       Important Note: The processor hardware based TSOD polling can be configured with 
       pointer based access. If software manually issue SMBus transaction to other 
       address, i.e. changing the pointer in the slave device, it is software's 
       responsibility to restore the pointer in each TSOD before returning to hardware 
       based TSOD polling while keeping the SMB_PNTR_SEL=1. 
     */
    UINT32 smb_cmd_trigger : 1;
    /* smb_cmd_trigger - Bits[31:31], RW_V, default = 1'b0 
       CMD trigger: After setting this bit to 1, the SMBus master will issue the SMBus 
       command using the other fields written in SMBCMD_[0:1] and SMBCntl_[0:1]. Note: 
       the '-V' in the attribute implies the hardware will reset this bit when the 
       SMBus command is being started. 
     */
  } Bits;
  UINT32 Data;
} SMBCMD_0_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* SMBCNTL_0_MC_MAIN_REG supported on:                                        */
/*       IVT_EP (0x40178188)                                                  */
/*       IVT_EX (0x40178188)                                                  */
/*       HSX (0x40198188)                                                     */
/*       BDX (0x40198188)                                                     */
/* Register default value:              0xAC000000                            */
#define SMBCNTL_0_MC_MAIN_REG 0x0B004188


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x188
 */
typedef union {
  struct {
    UINT32 tsod_present : 8;
    /* tsod_present - Bits[7:0], RW_LB, default = 8'b00000000 
       DIMM slot mask to indicate whether the DIMM is equipped with TSOD sensor.
       Bit 7: must be programmed to zero. Upper channel slot #3 is not supported
       Bit 6: TSOD PRESENT at upper channel (ch 1 or ch 3) slot #2
       Bit 5: TSOD PRESENT at upper channel (ch 1 or ch 3) slot #1
       Bit 4: TSOD PRESENT at upper channel (ch 1 or ch 3) slot #0
       Bit 3: must be programmed to zero. Lower channel slot #3 is not supported
       Bit 2: TSOD PRESENT at lower channel (ch 0 or ch 2) slot #2
       Bit 1: TSOD PRESENT at lower channel (ch 0 or ch 2) slot #1
       Bit 0: TSOD PRESENT at lower channel (ch 0 or ch 2) slot #0
     */
    UINT32 smb_tsod_poll_en : 1;
    /* smb_tsod_poll_en - Bits[8:8], RW_LB, default = 1'b0 
       TSOD polling enable
       '0': disable TSOD polling and enable SPDCMD accesses.
       '1': disable SPDCMD access and enable TSOD polling.
       It is important to make sure no pending SMBus transaction and the TSOD polling 
       must be disabled (and pending TSOD polling must be drained) before changing the 
       TSODPOLLEN. 
     */
    UINT32 start_tsod_poll : 1;
    /* start_tsod_poll - Bits[9:9], RW_V, default = 1'b0 
       This bit wil start the reading of all enabled devices
       Note that the hardware will reset this bit when the SMBus polling has started.
     */
    UINT32 smb_soft_rst : 1;
    /* smb_soft_rst - Bits[10:10], RW, default = 1'b0 
       SMBus software reset strobe to graceful terminate pending transaction (after 
       ACK) and keep the SMB from issuing any transaction until this bit is cleared. If 
       slave device is hung, software can write this bit to 1 and the SMB_CKOVRD to 0 
       (for more than 35ms) to force hung the SMB slaves to time-out and put it in idle 
       state without using power good reset or warm reset. 
       Note: software need to set the SMB_CKOVRD back to 1 after 35ms in order to force 
       slave devices to time-out in case there is any pending transaction. The 
       corresponding SMB_STAT_x.SMB_SBE error status bit may be set if there was such 
       pending transaction time-out (non-graceful termination). If the pending 
       transaction was a write operation, the slave device content may be corrupted by 
       this clock overide operation. A subsequent SMB command will automatically 
       cleared the SMB_SBE. 
       The IMC H/W will perform SMB time-out (if the SMB_SBE_EN=1). Software should 
       simply clear the SMB_SBE and SMB_SOFT_RST sequentially after writing the 
       SMB_CKOVRD=0 and SMB_SOFT_RST=1 (asserting clock override and perform graceful 
       txn termination). Hardware will automatically de-assert the SMB_CKOVRD (update 
       to 1) after the pre-configured 35ms/65ms time-out. 
     */
    UINT32 spd_rst_on_forcesr : 1;
    /* spd_rst_on_forcesr - Bits[11:11], RW_LB, default = 1'b0 
       This bit is used to control how force self refresh enter affects the SMBus 
       controller 
       '0': force_sr_enter does not affect SMBus actions
       '1': force_sr_enter will reset SMBus controller
     */
    UINT32 rsvd_12 : 9;
    /* rsvd_12 - Bits[20:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_21 : 5;
    UINT32 smb_dis_wrt : 1;
    /* smb_dis_wrt - Bits[26:26], RW_LB, default = 1'b1 
       Disable SMBus Write
       Writing a '0' to this bit enables CMD to be set to 1; Writing a 1 to force CMD 
       bit to be always 0, i.e. disabling SMBus write. This bit can only be written in 
       SMMode. SMBus Read is not affected. I2C Write Pointer Update Command is not 
       affected. 
       Important Note to BIOS: Since BIOS is the source to update SMBCNTL_x register 
       initially after reset, it is important to determine wheter the SMBus can have 
       write capabilty before writing any upper bits (bit24-31) via byte-enable config 
       write (or writing any bit within this register via 32b config write) within the 
       SMBCNTL register. 
     */
    UINT32 smb_ckovrd : 1;
    /* smb_ckovrd - Bits[27:27], RWS_V, default = 1'b1 
       Clock Override
       '0' = Clock signal is driven low, overriding writing a '1' to CMD.
       '1' = Clock signal is released high, allowing normal operation of CMD.
       Toggling this bit can be used to 'budge' the port out of a 'stuck' state.
       Software can write this bit to 0 and the SMB_SOFT_RST to 1 to force hung SMBus 
       controller and the SMB slaves to idle state without using power good reset or 
       warm reset. 
       Note: software need to set the SMB_CKOVRD back to 1 after 35ms in order to force 
       slave devices to time-out in case there is any pending transaction. The 
       corresponding SMB_STAT_x.SMB_SBE error status bit may be set if there was such 
       pending transaction time-out (non-graceful termination). If the pending 
       transaction was a write operation, the slave device content may be corrupted by 
       this clock overide operation. A subsequent SMB command will automatically 
       cleared the SMB_SBE. 
       IMC added SMBus time-out control timer. When the time-out control timer expired, 
       the SMB_CKOVRD# will "de-assert", i.e. return to 1 value. 
     */
    UINT32 smb_dti : 4;
    /* smb_dti - Bits[31:28], RWS, default = 4'b1010 
       Device Type Identifier: This field specifies the device type identifier. Only 
       devices with this device-type will respond to commands. 
       '0011' specifies TSOD.
       '1010' specifies EEPROM's.
       '0110' specifies a write-protect operation for an EEPROM.
       Other identifiers can be specified to target non-EEPROM devices on the SMBus.
       
       Note: IMC based hardware TSOD polling uses hardcoded DTI. Changing this field 
       has no effect on the hardware based TSOD polling. 
     */
  } Bits;
  UINT32 Data;
} SMBCNTL_0_MC_MAIN_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* SMB_TSOD_POLL_RATE_CNTR_0_MC_MAIN_REG supported on:                        */
/*       IVT_EP (0x4017818C)                                                  */
/*       IVT_EX (0x4017818C)                                                  */
/*       HSX (0x4019818C)                                                     */
/*       BDX (0x4019818C)                                                     */
/* Register default value:              0x00000000                            */
#define SMB_TSOD_POLL_RATE_CNTR_0_MC_MAIN_REG 0x0B00418C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x18c
 */
typedef union {
  struct {
    UINT32 smb_tsod_poll_rate_cntr : 18;
    /* smb_tsod_poll_rate_cntr - Bits[17:0], RW_LV, default = 18'b000000000000000000 
       TSOD poll rate counter. When it is decremented to zero, reset to zero or written 
       to zero, SMB_TSOD_POLL_RATE value is loaded into this counter and appear the 
       updated value in the next DCLK. 
     */
    UINT32 rsvd : 14;
    /* rsvd - Bits[31:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SMB_TSOD_POLL_RATE_CNTR_0_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* SMB_STAT_1_MC_MAIN_REG supported on:                                       */
/*       IVT_EP (0x40178190)                                                  */
/*       IVT_EX (0x40178190)                                                  */
/*       HSX (0x40198190)                                                     */
/*       BDX (0x40198190)                                                     */
/* Register default value:              0x07000000                            */
#define SMB_STAT_1_MC_MAIN_REG 0x0B004190
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * This register provides the interface to the SMBus/I2C (SCL and SDA signals) that 
 * is used to access the Serial Presence Detect EEPROM or Thermal Sensor on DIMM 
 * (TSOD) that defines the technology, configuration, and speed of the DIMMs 
 * controlled by IMC. 
 */
typedef union {
  struct {
    UINT32 smb_rdata : 16;
    /* smb_rdata - Bits[15:0], RO_V, default = 16'b0000000000000000 
       Read DataHolds data read from SMBus Read commands.
       
       Since TSOD/EEPROM are I2C devices and the byte order is MSByte first in a word 
       read, reading of I2C using word read should return SMB_RDATA[15:8]=I2C_MSB and 
       SMB_RDATA[7:0]=I2C_LSB. If reading of I2C using byte read, the 
       SMB_RDATA[15:8]=dont care; SMB_RDATA[7:0]=read_byte. 
       If we have a SMB slave connected on the bus , reading of the SMBus slave using 
       word read should return SMB_RDATA[15:8]=SMB_LSB and SMB_RDATA[7:0]=SMB_MSB. 
       
       If the software is not sure whether the target is I2C or SMBus slave, please use 
       byte access. 
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[23:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tsod_sa : 4;
    /* tsod_sa - Bits[27:24], RO_V, default = 4'b0111 
       This field captures the last issued TSOD slave address. Here is the slave 
       address and the DDR CHN and DIMM slot mapping: 
       
       Slave Address: 0 -- Channel: Even Chn; Slot #: 0
       Slave Address: 1 -- Channel: Even Chn; Slot #: 1
       Slave Address: 2 -- Channel: Even Chn; Slot #: 2
       Slave Address: 3 -- Channel: Even Chn; Slot #: 3 (reserved for future use)
       Slave Address: 4 -- Channel: Odd Chn; Slot #: 0
       Slave Address: 5 -- Channel: Odd Chn; Slot #: 1
       Slave Address: 6 -- Channel: Odd Chn; Slot #: 2
       Slave Address: 7 -- Channel: Odd Chn; Slot #: 3 (reserved for future use)
       A value of 8 in this register indicates to poll scalable memory buffer 
       temperature rather than a DIMM temperature, values above 0x8 are invalid. 
       Since this field only captures the TSOD polling slave address. During SMB error 
       handling, software should check the hung SMB_TSOD_POLL_EN state before disabling 
       the SMB_TSOD_POLL_EN in order to qualify whether this field is valid. 
     */
    UINT32 smb_busy : 1;
    /* smb_busy - Bits[28:28], ROS_V, default = 1'b0 
       SMBus Busy state. This bit is set by IMC while an SMBus/I2C command (including 
       TSOD command issued from IMC hardware) is executing. Any transaction that is 
       completed normally or gracefully will clear this bit automatically. By setting 
       the SMB_SOFT_RST will also clear this bit. 
       
       This register bit is sticky across reset so any surprise reset during pending 
       SMBus operation will sustain the bit assertion across surprised warm-reset. BIOS 
       reset handler can read this bit before issuing any SMBus transaction to 
       determine whether a slave device may need special care to force the slave to 
       idle state (e.g. via clock override toggling (SMB_CKOVRD) and/or via induced 
       time-out by asserting SMB_CKOVRD for 25-35ms). 
     */
    UINT32 smb_sbe : 1;
    /* smb_sbe - Bits[29:29], RO_V, default = 1'b0 
       SMBus Error
       This bit is set by IMC if an SMBus transaction (including the TSOD polling or 
       message channel initiated SMBus access) that does not complete successfully 
       (non-Ack has been received from slave at expected Ack slot of the transfer). If 
       a slave device is asserting clock stretching, IMC does not have logic to detect 
       this condition to set the SBE bit directly; however, the SMBus master will 
       detect the error at the corresponding transaction's expected ACK slot. 
       Once SMBUS_SBE bit is set, IMC stops issuing hardware initiated TSOD polling 
       SMBUS transactions until the SMB_SBE is cleared. IMC will not increment the 
       SMB_STAT_x.TSOD_SA until the SMB_SBE is cleared. Manual SMBus command interface 
       is not affected, i.e. new command issue will clear the SMB_SBE. 
     */
    UINT32 smb_wod : 1;
    /* smb_wod - Bits[30:30], RO_V, default = 1'b0 
       Write Operation Done
       This bit is set by IMC when a SMBus Write command has been completed on the 
       SMBus. It is cleared by IMC when a subsequent SMBus Write command is issued. 
     */
    UINT32 smb_rdo : 1;
    /* smb_rdo - Bits[31:31], RO_V, default = 1'b0 
       Read Data Valid
       This bit is set by IMC when the Data field of this register receives read data 
       from the SPD/TSOD after completion of an SMBus read command. It is cleared by 
       IMC when a subsequent SMBus read command is issued. 
     */
  } Bits;
  UINT32 Data;
} SMB_STAT_1_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* SMBCMD_1_MC_MAIN_REG supported on:                                         */
/*       IVT_EP (0x40178194)                                                  */
/*       IVT_EX (0x40178194)                                                  */
/*       HSX (0x40198194)                                                     */
/*       BDX (0x40198194)                                                     */
/* Register default value:              0x00000000                            */
#define SMBCMD_1_MC_MAIN_REG 0x0B004194
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * A write to this register initiates a DIMM EEPROM access through the SMBus/I2C.
 */
typedef union {
  struct {
    UINT32 smb_wdata : 16;
    /* smb_wdata - Bits[15:0], RWS, default = 16'b0000000000000000 
       Write Data: Holds data to be written by SPDW commands.
       
       Since TSOD/EEPROM are I2C devices and the byte order is MSByte first in a word 
       write, writing of I2C using word write should use SMB_WDATA[15:8]=I2C_MSB and 
       SMB_WDATA[7:0]=I2C_LSB. If writing of I2C using byte write, the 
       SMB_WDATA[15:8]=dont care; SMB_WDATA[7:0]=write_byte. 
       If we have a SMB slave connected on the bus , writing of the SMBus slave using 
       word write should use SMB_WDATA[15:8]=SMB_LSB and SMB_WDATA[7:0]=SMB_MSB. 
       It is software responsibility to figure out the byte order of the slave access.
     */
    UINT32 smb_ba : 8;
    /* smb_ba - Bits[23:16], RWS, default = 8'b00000000 
       Bus Txn Address: This field identifies the bus transaction address to be 
       accessed. 
       Note: in WORD access, 23:16 specifies 2B access address. In Byte access, 23:16 
       specified 1B access address. 
     */
    UINT32 smb_sa : 3;
    /* smb_sa - Bits[26:24], RWS, default = 3'b000 
       Slave Address: This field identifies the DIMM SPD/TSOD to be accessed.
     */
    UINT32 smb_wrt_cmd : 1;
    /* smb_wrt_cmd - Bits[27:27], RWS, default = 1'b0 
       When '0', it's a read command
       When '1', it's a write command
     */
    UINT32 smb_wrt_pntr : 1;
    /* smb_wrt_pntr - Bits[28:28], RWS, default = 1'b0 
       Bit[28:27]=00: SMBus Read
       
       Bit[28:27]=01: SMBus Write
       
       Bit[28:27]=10: illegal combination
       
       Bit[28:27]=11: Write to pointer registerSMBus/I2C pointer update (byte). bit 30, 
       and 29 are ignored. Note: SMBCntl_[0:1][26] will NOT disable WrtPntr update 
       command. 
     */
    UINT32 smb_word_access : 1;
    /* smb_word_access - Bits[29:29], RWS, default = 1'b0 
       word access: SMBus/I2C word (2B) access when set; otherwise, it is a byte 
       access. 
     */
    UINT32 smb_pntr_sel : 1;
    /* smb_pntr_sel - Bits[30:30], RWS, default = 1'b0 
       Pointer Selection: SMBus/I2C present pointer based access enable when set; 
       otherwise, use random access protocol. Hardware based TSOD polling will also use 
       this bit to enable the pointer word read. 
       
       Important Note: The processor hardware based TSOD polling can be configured with 
       pointer based access. If software manually issue SMBus transaction to other 
       address, i.e. changing the pointer in the slave device, it is software's 
       responsibility to restore the pointer in each TSOD before returning to hardware 
       based TSOD polling while keeping the SMB_PNTR_SEL=1. 
     */
    UINT32 smb_cmd_trigger : 1;
    /* smb_cmd_trigger - Bits[31:31], RW_V, default = 1'b0 
       CMD trigger: After setting this bit to 1, the SMBus master will issue the SMBus 
       command using the other fields written in SMBCMD_[0:1] and SMBCntl_[0:1]. Note: 
       the '-V' in the attribute implies the hardware will reset this bit when the 
       SMBus command is being started. 
     */
  } Bits;
  UINT32 Data;
} SMBCMD_1_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* SMBCNTL_1_MC_MAIN_REG supported on:                                        */
/*       IVT_EP (0x40178198)                                                  */
/*       IVT_EX (0x40178198)                                                  */
/*       HSX (0x40198198)                                                     */
/*       BDX (0x40198198)                                                     */
/* Register default value:              0xAC000000                            */
#define SMBCNTL_1_MC_MAIN_REG 0x0B004198


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x198
 */
typedef union {
  struct {
    UINT32 tsod_present : 8;
    /* tsod_present - Bits[7:0], RW_LB, default = 8'b00000000 
       DIMM slot mask to indicate whether the DIMM is equipped with TSOD sensor.
       Bit 7: must be programmed to zero. Upper channel slot #3 is not supported
       Bit 6: TSOD PRESENT at upper channel (ch 1 or ch 3) slot #2
       Bit 5: TSOD PRESENT at upper channel (ch 1 or ch 3) slot #1
       Bit 4: TSOD PRESENT at upper channel (ch 1 or ch 3) slot #0
       Bit 3: must be programmed to zero. Lower channel slot #3 is not supported
       Bit 2: TSOD PRESENT at lower channel (ch 0 or ch 2) slot #2
       Bit 1: TSOD PRESENT at lower channel (ch 0 or ch 2) slot #1
       Bit 0: TSOD PRESENT at lower channel (ch 0 or ch 2) slot #0
     */
    UINT32 smb_tsod_poll_en : 1;
    /* smb_tsod_poll_en - Bits[8:8], RW_LB, default = 1'b0 
       TSOD polling enable
       '0': disable TSOD polling and enable SPDCMD accesses.
       '1': disable SPDCMD access and enable TSOD polling.
       It is important to make sure no pending SMBus transaction and the TSOD polling 
       must be disabled (and pending TSOD polling must be drained) before changing the 
       TSODPOLLEN. 
     */
    UINT32 start_tsod_poll : 1;
    /* start_tsod_poll - Bits[9:9], RW_V, default = 1'b0 
       This bit wil start the reading of all enabled devices
       Note that the hardware will reset this bit when the SMBus polling has started.
     */
    UINT32 smb_soft_rst : 1;
    /* smb_soft_rst - Bits[10:10], RW, default = 1'b0 
       SMBus software reset strobe to graceful terminate pending transaction (after 
       ACK) and keep the SMB from issuing any transaction until this bit is cleared. If 
       slave device is hung, software can write this bit to 1 and the SMB_CKOVRD to 0 
       (for more than 35ms) to force hung the SMB slaves to time-out and put it in idle 
       state without using power good reset or warm reset. 
       Note: software need to set the SMB_CKOVRD back to 1 after 35ms in order to force 
       slave devices to time-out in case there is any pending transaction. The 
       corresponding SMB_STAT_x.SMB_SBE error status bit may be set if there was such 
       pending transaction time-out (non-graceful termination). If the pending 
       transaction was a write operation, the slave device content may be corrupted by 
       this clock overide operation. A subsequent SMB command will automatically 
       cleared the SMB_SBE. 
       The IMC H/W will perform SMB time-out (if the SMB_SBE_EN=1). Software should 
       simply clear the SMB_SBE and SMB_SOFT_RST sequentially after writing the 
       SMB_CKOVRD=0 and SMB_SOFT_RST=1 (asserting clock override and perform graceful 
       txn termination). Hardware will automatically de-assert the SMB_CKOVRD (update 
       to 1) after the pre-configured 35ms/65ms time-out. 
     */
    UINT32 spd_rst_on_forcesr : 1;
    /* spd_rst_on_forcesr - Bits[11:11], RW_LB, default = 1'b0 
       This bit is used to control how force self refresh enter affects the SMBus 
       controller 
       '0': force_sr_enter does not affect SMBus actions
       '1': force_sr_enter will reset SMBus controller
     */
    UINT32 rsvd_12 : 9;
    /* rsvd_12 - Bits[20:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_21 : 5;
    UINT32 smb_dis_wrt : 1;
    /* smb_dis_wrt - Bits[26:26], RW_LB, default = 1'b1 
       Disable SMBus Write
       
       Writing a '0' to this bit enables CMD to be set to 1; Writing a 1 to force CMD 
       bit to be always 0, i.e. disabling SMBus write. This bit can only be written 0/1 
       once to enable SMB write disable feature. SMBus Read is not affected. I2C Write 
       Pointer Update Command is not affected. 
       SMB_DIS_WRT=1 will disable the SMBus Write Capability if enabled on SKU.
       Important Note to BIOS: Since BIOS is the source to update SMBCNTL_x register 
       initially after reset, it is important to determine wheter the SMBus can have 
       write capabilty before writing any upper bits (bit24-31) via byte-enable config 
       write (or writing any bit within this register via 32b config write) within the 
       SMBCNTL register. 
     */
    UINT32 smb_ckovrd : 1;
    /* smb_ckovrd - Bits[27:27], RWS_V, default = 1'b1 
       Clock Override
       '0' = Clock signal is driven low, overriding writing a '1' to CMD.
       '1' = Clock signal is released high, allowing normal operation of CMD.
       Toggling this bit can be used to 'budge' the port out of a 'stuck' state.
       
       Software can write this bit to 0 and the SMB_SOFT_RST to 1 to force hung SMBus 
       controller and the SMB slaves to idle state without using power good reset or 
       warm reset. 
       
       Note: software need to set the SMB_CKOVRD back to 1 after 35ms in order to force 
       slave devices to time-out in case there is any pending transaction. The 
       corresponding SMB_STAT_x.SMB_SBE error status bit may be set if there was such 
       pending transaction time-out (non-graceful termination). If the pending 
       transaction was a write operation, the slave device content may be corrupted by 
       this clock overide operation. A subsequent SMB command will automatically 
       cleared the SMB_SBE. 
       IMC added SMBus time-out control timer in B0. When the time-out control timer 
       expired, the SMB_CKOVRD# will "de-assert", i.e. return to 1 value. 
     */
    UINT32 smb_dti : 4;
    /* smb_dti - Bits[31:28], RWS, default = 4'b1010 
       Device Type Identifier: This field specifies the device type identifier. Only 
       devices with this device-type will respond to commands. 
       '0011' specifies TSOD.
       '1010' specifies EEPROM's.
       '0110' specifies a write-protect operation for an EEPROM.
       Other identifiers can be specified to target non-EEPROM devices on the SMBus.
       
       Note: IMC based hardware TSOD polling uses hardcoded DTI. Changing this field 
       has no effect on the hardware based TSOD polling. 
     */
  } Bits;
  UINT32 Data;
} SMBCNTL_1_MC_MAIN_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* SMB_TSOD_POLL_RATE_CNTR_1_MC_MAIN_REG supported on:                        */
/*       IVT_EP (0x4017819C)                                                  */
/*       IVT_EX (0x4017819C)                                                  */
/*       HSX (0x4019819C)                                                     */
/*       BDX (0x4019819C)                                                     */
/* Register default value:              0x00000000                            */
#define SMB_TSOD_POLL_RATE_CNTR_1_MC_MAIN_REG 0x0B00419C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x19c
 */
typedef union {
  struct {
    UINT32 smb_tsod_poll_rate_cntr : 18;
    /* smb_tsod_poll_rate_cntr - Bits[17:0], RW_LV, default = 18'b000000000000000000 
       TSOD poll rate counter. When it is decremented to zero, reset to zero or written 
       to zero, SMB_TSOD_POLL_RATE value is loaded into this counter and appear the 
       updated value in the next DCLK. 
     */
    UINT32 rsvd : 14;
    /* rsvd - Bits[31:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SMB_TSOD_POLL_RATE_CNTR_1_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* SMB_PERIOD_CFG_MC_MAIN_REG supported on:                                   */
/*       IVT_EP (0x401781A0)                                                  */
/*       IVT_EX (0x401781A0)                                                  */
/*       HSX (0x401981A0)                                                     */
/*       BDX (0x401981A0)                                                     */
/* Register default value:              0x445C0FA0                            */
#define SMB_PERIOD_CFG_MC_MAIN_REG 0x0B0041A0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x1a0
 */
typedef union {
  struct {
    UINT32 smb_clk_prd : 16;
    /* smb_clk_prd - Bits[15:0], RWS, default = 16'b0000111110100000 
       This field specifies both SMBus Clock in number of DCLK. Note: In order to 
       generate a 50% duty cycle SCL, half of the SMB_CLK_PRD is used to generate SCL 
       high. SCL must stay low for at least another half of the SMB_CLK_PRD before 
       pulling high. It is recommend to program an even value in this field since the 
       hardware is simply doing a right shift for the divided by 2 operation. 
       Note the 100KHz SMB_CLK_PRD default value is calculated based on 800MT/s 
       (400MHz) DCLK. 
     */
    UINT32 rsvd_16 : 16;
  } Bits;
  UINT32 Data;
} SMB_PERIOD_CFG_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* SMB_PERIOD_CNTR_MC_MAIN_REG supported on:                                  */
/*       IVT_EP (0x401781A4)                                                  */
/*       IVT_EX (0x401781A4)                                                  */
/*       HSX (0x401981A4)                                                     */
/*       BDX (0x401981A4)                                                     */
/* Register default value:              0x00000000                            */
#define SMB_PERIOD_CNTR_MC_MAIN_REG 0x0B0041A4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x1a4
 */
typedef union {
  struct {
    UINT32 smb0_clk_prd_cntr : 16;
    /* smb0_clk_prd_cntr - Bits[15:0], RO_V, default = 16'b0000000000000000 
       SMBus #0 Clock Period Counter for Ch 01This field is the current SMBus Clock 
       Period Counter Value. 
     */
    UINT32 smb1_clk_prd_cntr : 16;
    /* smb1_clk_prd_cntr - Bits[31:16], RO_V, default = 16'b0000000000000000 
       SMBus #1 Clock Period Counter for Ch 23This field is the current SMBus Clock 
       Period Counter Value. 
     */
  } Bits;
  UINT32 Data;
} SMB_PERIOD_CNTR_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* SMB_TSOD_POLL_RATE_MC_MAIN_REG supported on:                               */
/*       IVT_EP (0x401781A8)                                                  */
/*       IVT_EX (0x401781A8)                                                  */
/*       HSX (0x401981A8)                                                     */
/*       BDX (0x401981A8)                                                     */
/* Register default value:              0x0003E800                            */
#define SMB_TSOD_POLL_RATE_MC_MAIN_REG 0x0B0041A8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * generated by critter 19_0_0x1a8
 */
typedef union {
  struct {
    UINT32 smb_tsod_poll_rate : 18;
    /* smb_tsod_poll_rate - Bits[17:0], RWS, default = 18'b111110100000000000 
       TSOD poll rate configuration between consecutive TSOD accesses to the TSOD 
       devices on the same SMBus segment. This field specifies the TSOD poll rate in 
       number of 500ns per CNFG_500_NANOSEC register field definition. 
     */
    UINT32 rsvd : 14;
    /* rsvd - Bits[31:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SMB_TSOD_POLL_RATE_MC_MAIN_STRUCT;
#endif /* ASM_INC */














/* MXB_SMB_ACCESS_MC_MAIN_REG supported on:                                   */
/*       IVT_EP (0x40178204)                                                  */
/*       IVT_EX (0x40178204)                                                  */
/*       HSX (0x40198204)                                                     */
/*       BDX (0x40198204)                                                     */
/* Register default value:              0x0000E160                            */
#define MXB_SMB_ACCESS_MC_MAIN_REG 0x0B004204
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * Programmable DTI, SA, BA for accessing MXB thermal sensors
 */
typedef union {
  struct {
    UINT32 sa : 3;
    /* sa - Bits[2:0], RWS, default = 3'b000 
       Slave address, may be a don't care.
     */
    UINT32 dti : 4;
    /* dti - Bits[6:3], RWS, default = 4'b1100 
       Device type identifier, current default value 0xC.
     */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ba : 8;
    /* ba - Bits[15:8], RWS, default = 8'b11100001 
       BA, aka command code for MXB TSOD access. Bit position 5 needs to be set to 1.
     */
    UINT32 rsvd_16 : 16;
    /* rsvd_16 - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MXB_SMB_ACCESS_MC_MAIN_STRUCT;
#endif /* ASM_INC */


/* MCMAIN_CHKN_BITS_MC_MAIN_REG supported on:                                 */
/*       IVT_EP (0x40178300)                                                  */
/*       IVT_EX (0x40178300)                                                  */
/*       HSX (0x40198300)                                                     */
/*       BDX (0x40198300)                                                     */
/* Register default value:              0x00000200                            */
#define MCMAIN_CHKN_BITS_MC_MAIN_REG 0x0B004300
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * Chicken bit register in MCMAIN.
 */
typedef union {
  struct {
    UINT32 frc_sngl_spr : 1;
    /* frc_sngl_spr - Bits[0:0], RW_LB, default = 1'b0 
       Force sparing to issue one transaction at a time
     */
    UINT32 rsvd_chkn_b1 : 1;
    /* rsvd_chkn_b1 - Bits[1:1], RW_LB, default = 1'b0 
       Disable signalling of SMI from patrol scrub engine. Patrol scrub is able to 
       signal SMI when it stops on error or reaches end of address range. See SCRUBCTL 
       for details. 
     */
    UINT32 dis_spr_err_log : 1;
    /* dis_spr_err_log - Bits[2:2], RW_LB, default = 1'b0 
       Disable spare error logging
     */
    UINT32 frc_spr_strt : 1;
    /* frc_spr_strt - Bits[3:3], RW_LB, default = 1'b0 
       Force spare start
     */
    UINT32 frc_spr_end : 1;
    /* frc_spr_end - Bits[4:4], RW_LB, default = 1'b0 
       Force spare end
     */
    UINT32 dis_spr_rir_updt : 1;
    /* dis_spr_rir_updt - Bits[5:5], RW_LB, default = 1'b0 
       Disable spare RIR update
     */
    UINT32 dis_ptrl_rty : 1;
    /* dis_ptrl_rty - Bits[6:6], RW_LB, default = 1'b0 
       Disable patrol retry
     */
    UINT32 dis_ptrl_err_log : 1;
    /* dis_ptrl_err_log - Bits[7:7], RW_LB, default = 1'b0 
       Disable patrol error logging
     */
    UINT32 frc_sngl_ptrl : 1;
    /* frc_sngl_ptrl - Bits[8:8], RW_LB, default = 1'b0 
       Force single patrol
     */
    UINT32 dis_mxb_tsod : 1;
    /* dis_mxb_tsod - Bits[9:9], RW_LB, default = 1'b1 
       Disable mxb tsod polling when in Intel SMI 2 modes
     */
    UINT32 frc_sngl_mtst : 1;
    /* frc_sngl_mtst - Bits[10:10], RW_LB, default = 1'b0 
       Force mem test to issue one transaction at a time
     */
    UINT32 dis_rcomp : 1;
    /* dis_rcomp - Bits[11:11], RW_LB, default = 1'b0 
       Disable rcomp
     */
    UINT32 frc_rcomp_cmp_ack : 1;
    /* frc_rcomp_cmp_ack - Bits[12:12], RW_LB, default = 1'b0 
       Force rcomp completion ack
     */
    UINT32 dis_lcl_ck_gate_mcmain : 1;
    /* dis_lcl_ck_gate_mcmain - Bits[13:13], RW_LB, default = 1'b0 
       Disable local clock gating - mcmains
     */
    UINT32 increase_rcomp : 1;
    /* increase_rcomp - Bits[14:14], RW_LB, default = 1'b0 
       Increase RCOMP quiet time
     */
    UINT32 dis_msg_ch_ck_gate_mc : 1;
    /* dis_msg_ch_ck_gate_mc - Bits[15:15], RW_LB, default = 1'b0 
       Disable IMC message channel clock gate when set.
     */
    UINT32 dis_msg_ch_ck_gate_ddrio : 1;
    /* dis_msg_ch_ck_gate_ddrio - Bits[16:16], RW_LB, default = 1'b0 
       Disable DDRIO message channel clock gate when set.
     */
    UINT32 dis_dbg_bgf_push : 1;
    /* dis_dbg_bgf_push - Bits[17:17], RW_LB, default = 1'b0 
       Disable Debug BGF Push when set
     */
    UINT32 defeature_12 : 1;
    /* defeature_12 - Bits[18:18], RW_LB, default = 1'b0 
       Patrol engine to stop at the completion of rank and set SCRUBCTL ptl_cmpl
     */
    UINT32 bgftune_lock : 1;
    /* bgftune_lock - Bits[19:19], RWS_L, default = 1'b0 
       Lock bit for MCBGFTUNE register
     */
    UINT32 freq_chng_flow : 1;
    /* freq_chng_flow - Bits[20:20], RW_LB, default = 1'b0 
       Bug fix for HSD b3123628:
       When set, tells MC that this is a frequency_change flow. When the bit is set, 
       following changes take place 
       
       1. When PmRsBlock is asserted, we block message channel to HA
       2. PkgC Empty and FullEmpty will ignore msgChEmpty when this bit is set
     */
    UINT32 dis_leak_cntr : 1;
    /* dis_leak_cntr - Bits[21:21], RW_LB, default = 1'b0 
       Disable leaky bucket counter in mc_dec
     */
    UINT32 dis_pma_blockack_hold : 1;
    /* dis_pma_blockack_hold - Bits[22:22], RW_LB, default = 1'b0 
       When set to 1, disables the "hold" logic on the BlockAck going to the MC PMA, 
       that keeps the BlockAck asserted once set, until the BlockReq de-asserts 
     */
    UINT32 sda_dbg : 2;
    /* sda_dbg - Bits[24:23], RW_LB, default = 2'b00 
       Enable send debug trigger to scalable memory buffer via SMbus SDA wire, per 
       SMbus master 
     */
    UINT32 frc_not_occ_0 : 1;
    /* frc_not_occ_0 - Bits[25:25], RW_LB, default = 1'b0 
       MC PMA interface override for channel 3 when not occupied. When set to 1, 
       MCActive will de-assert and InSRD will assert. 
     */
    UINT32 frc_not_occ_1 : 1;
    /* frc_not_occ_1 - Bits[26:26], RW_LB, default = 1'b0 
       MC PMA interface override for channel 3 when not occupied. When set to 1, 
       MCActive will de-assert and InSRD will assert. 
     */
    UINT32 frc_not_occ_2 : 1;
    /* frc_not_occ_2 - Bits[27:27], RW_LB, default = 1'b0 
       MC PMA interface override for channel 3 when not occupied. When set to 1, 
       MCActive will de-assert and InSRD will assert. 
     */
    UINT32 frc_not_occ_3 : 1;
    /* frc_not_occ_3 - Bits[28:28], RW_LB, default = 1'b0 
       MC PMA interface override for channel 3 when not occupied. When set to 1, 
       MCActive will de-assert and InSRD will assert. 
     */
    UINT32 dis_ptrl_psn : 1;
    /* dis_ptrl_psn - Bits[29:29], RW_LB, default = 1'b0 
       Disable patrol scrub poison line on finding uncorr error
     */
    UINT32 dis_asyncsr_habackpress : 1;
    /* dis_asyncsr_habackpress - Bits[30:30], RW_LB, default = 1'b0 
       When set to 1, the MC will not backpressure the HA during an ADR sequence.
       When set to 0 and the MC receives the AsyncSR signal from the PMA, the MC will 
       immediately backpressure the HA, stopping any new transactions from being sent 
       to the MC. 
     */
    UINT32 defeature_10 : 1;
    /* defeature_10 - Bits[31:31], RW_LB, default = 1'b0 
       This bit must be set for Romley and VMSE 1:1 before enabling RCOMP.
       This bit must be set to turn-off a hardware fix
       for RCOMP that is applicable only to VMSE 2:1. See be4571504
     */
  } Bits;
  UINT32 Data;
} MCMAIN_CHKN_BITS_MC_MAIN_STRUCT;
#endif /* ASM_INC */






/* MCMAIN_VEN_MUX_SEL_MC_MAIN_REG supported on:                               */
/*       HSX (0x40198340)                                                     */
/*       BDX (0x40198340)                                                     */
/* Register default value:              0x00000000                            */
#define MCMAIN_VEN_MUX_SEL_MC_MAIN_REG 0x0B004340

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.0.CFG.xml.
 * Register which creates mux selects for mclbvend
 */
typedef union {
  struct {
    UINT32 cross_block_ch3sel_rx0 : 1;
    /* cross_block_ch3sel_rx0 - Bits[0:0], RWS, default = 1'b0 
       mux C control to select from Rx0 VMSE/debug data to MC ch3
     */
    UINT32 cross_block_ch3sel_tx0 : 1;
    /* cross_block_ch3sel_tx0 - Bits[1:1], RWS, default = 1'b0 
       mux C control to select from Tx0 VMSE/debug data to MC ch3
     */
    UINT32 cross_block_ch2sel_tx0 : 1;
    /* cross_block_ch2sel_tx0 - Bits[2:2], RWS, default = 1'b0 
       mux C control to select from Tx0 VMSE/debug data to MC ch2
     */
    UINT32 cross_block_ch2sel_tx1 : 1;
    /* cross_block_ch2sel_tx1 - Bits[3:3], RWS, default = 1'b0 
       mux C control to select from Tx1 VMSE/debug data to MC ch2
     */
    UINT32 cross_block_sel : 1;
    /* cross_block_sel - Bits[4:4], RWS, default = 1'b0 
       mux B control to select either CrossBlock VMSE data or sameblock ddrx data to 
       ddrio 
     */
    UINT32 cross_ch_sel_high : 1;
    /* cross_ch_sel_high - Bits[5:5], RWS, default = 1'b0 
       Obsolete mux A control to select either the higher portion of VMSE/debug data or 
       ddrx data to MC. 
       This upper portion of mux A has been removed an now only the VMSE/debug data is 
       passed on the 
       hight bits. Native channel 0 DDR data tracing into channel 1 is no longer 
       supported. 
     */
    UINT32 rsvd_6 : 1;
    /* rsvd_6 - Bits[6:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 cross_trace_en : 1;
    /* cross_trace_en - Bits[7:7], RWS, default = 1'b0 
       Enable transmission of samples every clock from left veneer to right dp (channel 
       2) for tracing. 
     */
    UINT32 rsvd_8 : 24;
    /* rsvd_8 - Bits[31:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MCMAIN_VEN_MUX_SEL_MC_MAIN_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


#endif /* MC_MAIN_h */
