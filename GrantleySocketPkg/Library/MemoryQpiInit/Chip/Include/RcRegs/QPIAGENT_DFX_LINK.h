/* Date Stamp: 8/23/2014 */

#ifndef QPIAGENT_DFX_LINK_h
#define QPIAGENT_DFX_LINK_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For all target CPUs:                                                       */
/* QPIAGENT_DFX_LINK_DEV 8                                                    */
/* QPIAGENT_DFX_LINK_FUN 6                                                    */

/* VID_QPIAGENT_DFX_LINK_REG supported on:                                    */
/*       IVT_EP (0x20146000)                                                  */
/*       IVT_EX (0x20146000)                                                  */
/*       HSX (0x20146000)                                                     */
/*       BDX (0x20146000)                                                     */
/* Register default value:              0x8086                                */
#define VID_QPIAGENT_DFX_LINK_REG 0x06052000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.6.CFG.xml.
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
} VID_QPIAGENT_DFX_LINK_STRUCT;
#endif /* ASM_INC */


/* DID_QPIAGENT_DFX_LINK_REG supported on:                                    */
/*       IVT_EP (0x20146002)                                                  */
/*       IVT_EX (0x20146002)                                                  */
/*       HSX (0x20146002)                                                     */
/*       BDX (0x20146002)                                                     */
/* Register default value on IVT_EP:    0x0E86                                */
/* Register default value on IVT_EX:    0x0E86                                */
/* Register default value on HSX:       0x2F86                                */
/* Register default value on BDX:       0x6F86                                */
#define DID_QPIAGENT_DFX_LINK_REG 0x06052002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.6.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6F86 
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
} DID_QPIAGENT_DFX_LINK_STRUCT;
#endif /* ASM_INC */


/* PCICMD_QPIAGENT_DFX_LINK_REG supported on:                                 */
/*       IVT_EP (0x20146004)                                                  */
/*       IVT_EX (0x20146004)                                                  */
/*       HSX (0x20146004)                                                     */
/*       BDX (0x20146004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_QPIAGENT_DFX_LINK_REG 0x06052004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.6.CFG.xml.
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
} PCICMD_QPIAGENT_DFX_LINK_STRUCT;
#endif /* ASM_INC */


/* PCISTS_QPIAGENT_DFX_LINK_REG supported on:                                 */
/*       IVT_EP (0x20146006)                                                  */
/*       IVT_EX (0x20146006)                                                  */
/*       HSX (0x20146006)                                                     */
/*       BDX (0x20146006)                                                     */
/* Register default value:              0x0010                                */
#define PCISTS_QPIAGENT_DFX_LINK_REG 0x06052006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.6.CFG.xml.
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
} PCISTS_QPIAGENT_DFX_LINK_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_QPIAGENT_DFX_LINK_REG supported on:                                    */
/*       IVT_EP (0x10146008)                                                  */
/*       IVT_EX (0x10146008)                                                  */
/*       HSX (0x10146008)                                                     */
/*       BDX (0x10146008)                                                     */
/* Register default value:              0x00                                  */
#define RID_QPIAGENT_DFX_LINK_REG 0x06051008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.6.CFG.xml.
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
} RID_QPIAGENT_DFX_LINK_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_QPIAGENT_DFX_LINK_REG supported on:                                 */
/*       IVT_EP (0x10146009)                                                  */
/*       IVT_EX (0x10146009)                                                  */
/*       HSX (0x10146009)                                                     */
/*       BDX (0x10146009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_QPIAGENT_DFX_LINK_REG 0x06051009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.6.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_QPIAGENT_DFX_LINK_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_QPIAGENT_DFX_LINK_REG supported on:                                 */
/*       IVT_EP (0x2014600A)                                                  */
/*       IVT_EX (0x2014600A)                                                  */
/*       HSX (0x2014600A)                                                     */
/*       BDX (0x2014600A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_QPIAGENT_DFX_LINK_REG 0x0605200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.6.CFG.xml.
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
} CCR_N1_QPIAGENT_DFX_LINK_STRUCT;
#endif /* ASM_INC */


/* CLSR_QPIAGENT_DFX_LINK_REG supported on:                                   */
/*       IVT_EP (0x1014600C)                                                  */
/*       IVT_EX (0x1014600C)                                                  */
/*       HSX (0x1014600C)                                                     */
/*       BDX (0x1014600C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_QPIAGENT_DFX_LINK_REG 0x0605100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.6.CFG.xml.
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
} CLSR_QPIAGENT_DFX_LINK_STRUCT;
#endif /* ASM_INC */


/* PLAT_QPIAGENT_DFX_LINK_REG supported on:                                   */
/*       IVT_EP (0x1014600D)                                                  */
/*       IVT_EX (0x1014600D)                                                  */
/*       HSX (0x1014600D)                                                     */
/*       BDX (0x1014600D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_QPIAGENT_DFX_LINK_REG 0x0605100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.6.CFG.xml.
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
} PLAT_QPIAGENT_DFX_LINK_STRUCT;
#endif /* ASM_INC */


/* HDR_QPIAGENT_DFX_LINK_REG supported on:                                    */
/*       IVT_EP (0x1014600E)                                                  */
/*       IVT_EX (0x1014600E)                                                  */
/*       HSX (0x1014600E)                                                     */
/*       BDX (0x1014600E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_QPIAGENT_DFX_LINK_REG 0x0605100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.6.CFG.xml.
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
} HDR_QPIAGENT_DFX_LINK_STRUCT;
#endif /* ASM_INC */


/* BIST_QPIAGENT_DFX_LINK_REG supported on:                                   */
/*       IVT_EP (0x1014600F)                                                  */
/*       IVT_EX (0x1014600F)                                                  */
/*       HSX (0x1014600F)                                                     */
/*       BDX (0x1014600F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_QPIAGENT_DFX_LINK_REG 0x0605100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.6.CFG.xml.
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
} BIST_QPIAGENT_DFX_LINK_STRUCT;
#endif /* ASM_INC */






/* CAPPTR_QPIAGENT_DFX_LINK_REG supported on:                                 */
/*       IVT_EP (0x10146034)                                                  */
/*       IVT_EX (0x10146034)                                                  */
/*       HSX (0x10146034)                                                     */
/*       BDX (0x10146034)                                                     */
/* Register default value on IVT_EP:    0x40                                  */
/* Register default value on IVT_EX:    0x40                                  */
/* Register default value on HSX:       0x00                                  */
/* Register default value on BDX:       0x40                                  */
#define CAPPTR_QPIAGENT_DFX_LINK_REG 0x06051034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.6.CFG.xml.
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
} CAPPTR_QPIAGENT_DFX_LINK_STRUCT;
#endif /* ASM_INC */


/* INTL_QPIAGENT_DFX_LINK_REG supported on:                                   */
/*       IVT_EP (0x1014603C)                                                  */
/*       IVT_EX (0x1014603C)                                                  */
/*       HSX (0x1014603C)                                                     */
/*       BDX (0x1014603C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_QPIAGENT_DFX_LINK_REG 0x0605103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.6.CFG.xml.
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
} INTL_QPIAGENT_DFX_LINK_STRUCT;
#endif /* ASM_INC */


/* INTPIN_QPIAGENT_DFX_LINK_REG supported on:                                 */
/*       IVT_EP (0x1014603D)                                                  */
/*       IVT_EX (0x1014603D)                                                  */
/*       HSX (0x1014603D)                                                     */
/*       BDX (0x1014603D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_QPIAGENT_DFX_LINK_REG 0x0605103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.6.CFG.xml.
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
} INTPIN_QPIAGENT_DFX_LINK_STRUCT;
#endif /* ASM_INC */


/* MINGNT_QPIAGENT_DFX_LINK_REG supported on:                                 */
/*       IVT_EP (0x1014603E)                                                  */
/*       IVT_EX (0x1014603E)                                                  */
/*       HSX (0x1014603E)                                                     */
/*       BDX (0x1014603E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_QPIAGENT_DFX_LINK_REG 0x0605103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.6.CFG.xml.
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
} MINGNT_QPIAGENT_DFX_LINK_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_QPIAGENT_DFX_LINK_REG supported on:                                 */
/*       IVT_EP (0x1014603F)                                                  */
/*       IVT_EX (0x1014603F)                                                  */
/*       HSX (0x1014603F)                                                     */
/*       BDX (0x1014603F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_QPIAGENT_DFX_LINK_REG 0x0605103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.6.CFG.xml.
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
} MAXLAT_QPIAGENT_DFX_LINK_STRUCT;
#endif /* ASM_INC */






/* QPILKSCRATCH_QPIAGENT_DFX_LINK_REG supported on:                           */
/*       IVT_EP (0x201461F0)                                                  */
/*       IVT_EX (0x201461F0)                                                  */
/*       HSX (0x401461F0)                                                     */
/*       BDX (0x401461F0)                                                     */
/* Register default value on IVT_EP:    0x0000                                */
/* Register default value on IVT_EX:    0x0000                                */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define QPILKSCRATCH_QPIAGENT_DFX_LINK_REG 0x0605C000


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.6.CFG.xml.
 * Intel QPI Link Scratch Register
 */
typedef union {
  struct {
    UINT32 scratch0 : 8;
    /* scratch0 - Bits[7:0], RW_LB, default = 8'b0 
        
               Scratch register with LT Lock.  (Note:  write is allowed when force 
       write = 0?) 
             
     */
    UINT32 scratch1 : 8;
    /* scratch1 - Bits[15:8], RW, default = 8'b0 
        
               Scratch register
             
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPILKSCRATCH_QPIAGENT_DFX_LINK_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */












































/* QPIINBDPCTL_QPIAGENT_DFX_LINK_REG supported on:                            */
/*       IVT_EP (0x40146250)                                                  */
/*       IVT_EX (0x40146250)                                                  */
/*       HSX (0x40146250)                                                     */
/*       BDX (0x40146250)                                                     */
/* Register default value:              0x00000000                            */
#define QPIINBDPCTL_QPIAGENT_DFX_LINK_REG 0x06054250


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.6.CFG.xml.
 * 
 *       Intel QPI In-band Debug Packet Control
 *       This register contains the bits to control the in-band debug packets. 
 * There are three types supported; debug event packets, marker debug packets and 
 * timing exposure packets 
 *     
 */
typedef union {
  struct {
    UINT32 ibdevten : 1;
    /* ibdevten - Bits[0:0], RWS, default = 1'b0 
       
       	enables transmission of Inband Debug Event flits when a 'debug event' occurs.  
       	The set of debug events is programmable (QPIINBDSEL).
             
     */
    UINT32 ibdrtpen : 1;
    /* ibdrtpen - Bits[1:1], RWS, default = 1'b0 
       
       	enables transmission of RTP Debug flits.
             
     */
    UINT32 ibdmrken : 1;
    /* ibdmrken - Bits[2:2], RWS, default = 1'b0 
       
        enables transmission of 'marker event' Debug Flits (Inband Debug Event ctrl 
       flit).   
        When enabled, Intel QPI will send an Inband Debug Event flit when "PmSuperSync" 
       from PMA is asserted. 
             
     */
    UINT32 evtfhigh : 1;
    /* evtfhigh - Bits[3:3], RWS, default = 1'b0 
       
        forces Inband Debug Event flits to be sent as high priority (will pre-empt  
       protocol traffic -  
       	otherwise these flits will be sent only opportunistically)
             
     */
    UINT32 evtwaken : 1;
    /* evtwaken - Bits[4:4], RWS, default = 1'b0 
       
       	enables Tx PM wakeup due to Inband Debug Event flits
             
     */
    UINT32 mrkfhigh : 1;
    /* mrkfhigh - Bits[5:5], RWS, default = 1'b0 
       
        forces 'marker event' Inband Debug Event flits to be sent as high priority 
       (will pre-empt protocol traffic -  
       	otherwise these flits will be sent only opportunistically)
             
     */
    UINT32 mrkwaken : 1;
    /* mrkwaken - Bits[6:6], RWS, default = 1'b0 
       
       	enables Tx PM wakeup due to 'marker event' Inband Debug Event flits
             
     */
    UINT32 rtpfhigh : 1;
    /* rtpfhigh - Bits[7:7], RWS, default = 1'b0 
       
        forces RTP Debug flits to be sent as high priority (will pre-empt protocol 
       traffic -  
       	otherwise these flits will be sent only opportunistically)
             
     */
    UINT32 rtpwaken : 1;
    /* rtpwaken - Bits[8:8], RWS, default = 1'b0 
       
       	enables Tx PM wakeup due to RTP Debug flits 
             
     */
    UINT32 forcetimer : 3;
    /* forcetimer - Bits[11:9], RWS, default = 3'b0 
       
        amount of time before an opportunistic Debug flit is converted to a high 
       priority request 
       	000: 64 flit clocks
       	001: 128 flit clocks
       	010: 512 flit clocks
       	011: 2048 flit clocks
       	100: 4096 flit clocks
       	101: 16384 flit clocks
       	110: 65536 flit clocks
       	111: never
             
     */
    UINT32 rtpfreq : 3;
    /* rtpfreq - Bits[14:12], RWS, default = 3'b0 
       
       	configures the frequency of transmission for 'master timer' RTP Debug flits:
       	000: 32 flit clocks
       	001: 64 flit clocks
       	010: 256 flit clocks
       	011: 1024 flit clocks
       	100: 2048 flit clocks
       	101: 4096 flit clocks
       	110: 32768 flit clocks
       	111: master timer RTP Debug flits disabled
             
     */
    UINT32 rtpslave : 1;
    /* rtpslave - Bits[15:15], RWS, default = 1'b0 
       
       	Selects slave mode generation of RTP Debug flits:
       	1:  Slave mode - generate RTP Debug flit upon reception of RTP Debug flit
       	0:  Master mode - generate RTP Debug flit upon expiration of 'master timer'
             
     */
    UINT32 rsvd_16 : 6;
    /* rsvd_16 - Bits[21:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rtp_on_masterslave_en : 1;
    /* rtp_on_masterslave_en - Bits[22:22], RWS, default = 1'b0 
       
       	higher level enable for master/slave generation of RTP Debug flits:
       	master:  'master timer' expiration
       	slave:      reception of RTP flits
             
     */
    UINT32 rtp_on_phy_rst_en : 1;
    /* rtp_on_phy_rst_en - Bits[23:23], RWS, default = 1'b0 
       
       	enables generation of RTP Debug flit upon PHY exit from Reset
             
     */
    UINT32 rtp_on_ll_rst_en : 1;
    /* rtp_on_ll_rst_en - Bits[24:24], RWS, default = 1'b0 
       
        enables generation of RTP Debug flit upon 'solid' entry into Normal Op by the 
       Link Layer Init FSM 
             
     */
    UINT32 rtp_on_localevent_en : 1;
    /* rtp_on_localevent_en - Bits[25:25], RWS, default = 1'b0 
       
        enables generation of RTP Debug flits upon occurance of a 'local event' (CTO[1] 
       - programmed in QPICTOSEL) 
             
     */
    UINT32 rtp_on_csr_write : 1;
    /* rtp_on_csr_write - Bits[26:26], RWS, default = 1'b0 
       
       	when written to 1 - immediately generates an RTP Debug flit
             
     */
    UINT32 ignore_llctrl_enable_rtp : 1;
    /* ignore_llctrl_enable_rtp - Bits[27:27], RWS, default = 1'b0 
       
        bypasses the RTP enable in QPILLCTRL (when set, allows generation of RTP Debug 
       flits - regardless of enable in QPILLCTRL) 
             
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIINBDPCTL_QPIAGENT_DFX_LINK_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */












/* QPIACKCNT_QPIAGENT_DFX_LINK_REG supported on:                              */
/*       IVT_EP (0x40146268)                                                  */
/*       IVT_EX (0x40146268)                                                  */
/*       HSX (0x40146268)                                                     */
/*       BDX (0x40146268)                                                     */
/* Register default value:              0x00000000                            */
#define QPIACKCNT_QPIAGENT_DFX_LINK_REG 0x06054268


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.6.CFG.xml.
 * 
 *       Intel QPI Ack Counts
 *       This register contains the ack counts and link layer read/write pointers. 
 * The values stored are either the current value or a snapshot captured value 
 * depending on the control bit. 
 *     
 */
typedef union {
  struct {
    UINT32 ackcnt : 8;
    /* ackcnt - Bits[7:0], ROS_V, default = 8'b0 
        
               Acknowledge count[7:0]
             
     */
    UINT32 llrptr : 8;
    /* llrptr - Bits[15:8], ROS_V, default = 8'b0 
        
               Link layer retry read pointer[6:0]
             
     */
    UINT32 llwptr : 8;
    /* llwptr - Bits[23:16], ROS_V, default = 8'b0 
        
               Link layer retry write pointer[6:0]
             
     */
    UINT32 crdcptmd : 1;
    /* crdcptmd - Bits[24:24], RWS, default = 1'b0 
        
               0: then loads continuously (ie. every cycle). 
        1: then loads based on DFX cluster trigger out bit1 (which itself is a 
       selectable field that can use mBPs, response functions, etc.)  
             
     */
    UINT32 rsvd : 7;
    /* rsvd - Bits[31:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIACKCNT_QPIAGENT_DFX_LINK_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */








/* QPIPARERRLOG_QPIAGENT_DFX_LINK_REG supported on:                           */
/*       IVT_EP (0x40146278)                                                  */
/*       IVT_EX (0x40146278)                                                  */
/*       HSX (0x40146278)                                                     */
/*       BDX (0x40146278)                                                     */
/* Register default value:              0x00000000                            */
#define QPIPARERRLOG_QPIAGENT_DFX_LINK_REG 0x06054278
#if defined(IVT_HOST) || defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file IVX\1.8.6.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 32;
  } Bits;
  UINT32 Data;
} QPIPARERRLOG_QPIAGENT_DFX_LINK_IVT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(HSX_HOST) */



#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.6.CFG.xml.
 * 
 *       Intel QPI Phy/Link Error Injection Log (Parity Error Log?)
 *     
 */
typedef union {
  struct {
    UINT32 pelset0 : 8;
    /* pelset0 - Bits[7:0], RW1CS, default = 8'b0 
        
               indicates which (if any) parity bit failed on a read from the RxQ
             
     */
    UINT32 pelset1 : 8;
    /* pelset1 - Bits[15:8], RW1CS, default = 8'b0 
        
               indicates which (if any) parity bit failed on a read from the LLRB
             
     */
    UINT32 pelset2 : 4;
    /* pelset2 - Bits[19:16], RW1CS, default = 4'b0 
        
               indicates which (if any) parity bit failed on a read from the TxQ
             
     */
    UINT32 pelset3 : 1;
    /* pelset3 - Bits[20:20], RW1CS, default = 1'b0 
        
               indicates parity check failed on a read from the RBT
             
     */
    UINT32 pelset4 : 1;
    /* pelset4 - Bits[21:21], RW1CS, default = 1'b0 
        
               indicates parity check failed on a data coming from R3QPI
             
     */
    UINT32 rsvd : 10;
    /* rsvd - Bits[31:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIPARERRLOG_QPIAGENT_DFX_LINK_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* QPIRXEINJCTL0_QPIAGENT_DFX_LINK_REG supported on:                          */
/*       IVT_EP (0x40146288)                                                  */
/*       IVT_EX (0x40146288)                                                  */
/*       HSX (0x40146288)                                                     */
/*       BDX (0x40146288)                                                     */
/* Register default value:              0xCACA0000                            */
#define QPIRXEINJCTL0_QPIAGENT_DFX_LINK_REG 0x06054288


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.6.CFG.xml.
 * 
 *       Intel QPI Rx Error Injection Control 0
 *       This register contains the error injection control register that applies 
 * the mask registers across the width of the Intel QPI flit for error detection 
 * testing.  
 *     
 */
typedef union {
  struct {
    UINT32 rsvd : 2;
    /* rsvd - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 superr : 1;
    /* superr - Bits[2:2], RWS_L, default = 1'b0 
        
               0: Do not suppress error injection in replay. Warning: using this 
       setting makes it more likely to hit the abort state.  
        1: Suppress errors on the replay that reached the escalation counters according 
       to the bit settings in EinjRetryActiveMode, EinjNumRetry, and EinjNumPhyReInit 
             
     */
    UINT32 maten : 1;
    /* maten - Bits[3:3], RWS_L, default = 1'b0 
        
               0: No Header match, inject error on any header (IIB==1), except exclude 
       NULLs if bit[5] EXCLUDE_NULL==1.  
        1: Inject error based on Header match MATF1 hit signal (ignores MATF2 and 
       HDmatOut hit signals)  
             
     */
    UINT32 crcdis : 1;
    /* crcdis - Bits[4:4], RWS_L, default = 1'b0 
        
               0: Enable CRC checking 
       	1: Disable CRC checking 
             
     */
    UINT32 suppress_st : 1;
    /* suppress_st - Bits[5:5], RW1CS, default = 1'b0 
        
               Error Injection suppression status
             
     */
    UINT32 errinjst : 1;
    /* errinjst - Bits[6:6], RW1CS, default = 1'b0 
        
               0: No error injection occurred. 
       	1: An error was injected 
             
     */
    UINT32 einjretryactivemode : 2;
    /* einjretryactivemode - Bits[8:7], RWS_L, default = 2'b0 
        
               This determines how the link will respond to error injection during 
       replay.  
       	00: Retry mode 
       	01: Phy init mode 
       	10: Retry and Phy init mode 
       	11: Abort mode 
             
     */
    UINT32 einjnumretry : 5;
    /* einjnumretry - Bits[13:9], RWS_L, default = 5'b0 
        
               Number of retries [4:0]
             
     */
    UINT32 einjnumphyreinit : 2;
    /* einjnumphyreinit - Bits[15:14], RWS_L, default = 2'b0 
        
               Number of phy retries before re-initialization [1:0]
             
     */
    UINT32 openloop_str_sel : 4;
    /* openloop_str_sel - Bits[19:16], RWS_L, default = 4'hA 
        
        Start selection for injecting in open-loop response function. Open-loop means 
       that the response function will only watch for the stop condition and ignores 
       the response from the affected logic.  
       	0000: Micro Break Point 0 (MBP[0]) 
       	0001: Micro Break Point 1 (MBP[1]) 
       	0010: Cluster Trigger Out 0 (CTO[0]) 
       	0011: Cluster Trigger Out 1 (CTO[1]) 
       	0100: Artificial Starvation Counter 0 (ASC[0]) 
       	0101: Artificial Starvation Counter 1 (ASC[1]) 
       	0110: Reserved 
       	0111: Reserved 
       	1000: MCA Correctable Error 
       	1001: MCA Uncorrectable Error 
       	1010: Response never occurs 
       	1011: Response occurs immediately 
       	1100-1111: Reserved
             
     */
    UINT32 openloop_stp_sel : 4;
    /* openloop_stp_sel - Bits[23:20], RWS_L, default = 4'hC 
        
        Stop selection for injecting in open-loop response function. Open-loop means 
       that the response function will only watch for the stop condition and ignores 
       the response from the affected logic.  
       	0000: Micro Break Point 0 (MBP[0]) 
       	0001: Micro Break Point 1 (MBP[1]) 
       	0010: Cluster Trigger Out 0 (CTO[0]) 
       	0011: Cluster Trigger Out 1 (CTO[1]) 
       	0100: Artificial Starvation Counter 0 (ASC[0]) 
       	0101: Artificial Starvation Counter 1 (ASC[1]) 
       	0110: Reserved 
       	0111: Reserved 
       	1000: MCA Correctable Error 
       	1001: MCA Uncorrectable Error 
        1010: Deactivate one clock after activation. Error injection on one transaction 
       will occur on the interface that is enabled.  
       	1011: Response is never deactivated. 
       	1100: Deactivate the response when the activating signal becomes false. 
       	1101-1111: Reserved
             
     */
    UINT32 once_str_sel : 4;
    /* once_str_sel - Bits[27:24], RWS_L, default = 4'hA 
        
        Start selection for injecting in standard closed-loop response function. 
       Closed-loop means that once activated,  
        the response function will not stop until both the stop condition has been met 
       *and* the logic affected by the response function  
        has been able to do the throttling/injecting on actual traffic. Closed-loop is 
       the most common response function.  
       	0000: Micro Break Point 0 (MBP[0]) 
       	0001: Micro Break Point 1 (MBP[1]) 
       	0010: Cluster Trigger Out 0 (CTO[0]) 
       	0011: Cluster Trigger Out 1 (CTO[1]) 
       	0100: Artificial Starvation Counter 0 (ASC[0]) 
       	0101: Artificial Starvation Counter 1 (ASC[1]) 
       	0110: Reserved 
       	0111: Reserved 
       	1000: MCA Correctable Error 
       	1001: MCA Uncorrectable Error 
       	1010: Response never occurs 
       	1011: Response occurs immediately 
       	1100-1111: Reserved
             
     */
    UINT32 once_stp_sel : 4;
    /* once_stp_sel - Bits[31:28], RWS_L, default = 4'hC 
        
               Stop selection for injecting in standard closed-loop response function. 
       Closed-loop means that once activated, the response function  
        will not stop until both the stop condition has been met *and* the logic 
       affected by the response function has been able to do the  
        throttling/injecting on actual traffic. Closed-loop is the most common response 
       function.  
       	0000: Micro Break Point 0 (MBP[0]) 
       	0001: Micro Break Point 1 (MBP[1]) 
       	0010: Cluster Trigger Out 0 (CTO[0]) 
       	0011: Cluster Trigger Out 1 (CTO[1]) 
       	0100: Artificial Starvation Counter 0 (ASC[0]) 
       	0101: Artificial Starvation Counter 1 (ASC[1]) 
       	0110: Reserved 
       	0111: Reserved 
       	1000: MCA Correctable Error 
       	1001: MCA Uncorrectable Error 
        1010: Deactivate one clock after activation. Error injection on one transaction 
       will occur on the interface that is enabled.  
       	1011: Response is never deactivated. 
       	1100: Deactivate the response when the activating signal becomes false. 
       	1101-1111: Reserved
             
     */
  } Bits;
  UINT32 Data;
} QPIRXEINJCTL0_QPIAGENT_DFX_LINK_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIRXEINJCTL1_QPIAGENT_DFX_LINK_REG supported on:                          */
/*       IVT_EP (0x4014628C)                                                  */
/*       IVT_EX (0x4014628C)                                                  */
/*       HSX (0x4014628C)                                                     */
/*       BDX (0x4014628C)                                                     */
/* Register default value:              0x00000000                            */
#define QPIRXEINJCTL1_QPIAGENT_DFX_LINK_REG 0x0605428C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.6.CFG.xml.
 * 
 *       Intel QPI Rx Error Injection Control 1
 *       This register defines what types of flits to exclude from error injection 
 * Any bits that are prefixed with include, cause Inject a CRC error onto all 
 * flit-type 
 *       flits that are received on the CSI Link Layer RX. 
 *       Any bit that is prefixed with Exclude results in exclusion of all 
 * flit-type flits from injection of CRC error that are received on the CSI Link 
 * Layer RX.  
 *       Note that QPIRXEINJCTL0 must be configured for actual error injection to 
 * occur.  
 *       Note that the include enables take precedence over the exclude enables.
 *     
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 exclude_data : 1;
    /* exclude_data - Bits[2:2], RWS_L, default = 1'b0 
        
               Exclude Data Flits
             
     */
    UINT32 exclude_spc_null : 1;
    /* exclude_spc_null - Bits[3:3], RWS_L, default = 1'b0 
        
               Exclude Special NULL
             
     */
    UINT32 rsvd_4 : 1;
    /* rsvd_4 - Bits[4:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 exclude_spc_all : 1;
    /* exclude_spc_all - Bits[5:5], RWS_L, default = 1'b0 
        
               Exclude Special All
             
     */
    UINT32 exclude_spc_pe_rfi : 1;
    /* exclude_spc_pe_rfi - Bits[6:6], RWS_L, default = 1'b0 
        
               Exclude Special Parameter Ready For Init
             
     */
    UINT32 exclude_spc_pe_all : 1;
    /* exclude_spc_pe_all - Bits[7:7], RWS_L, default = 1'b0 
        
               Exclude Special Parameter Exchange All
             
     */
    UINT32 exclude_spc_llr_req : 1;
    /* exclude_spc_llr_req - Bits[8:8], RWS_L, default = 1'b0 
        
               Exclude Special LLR Request
             
     */
    UINT32 exclude_spc_llr_ack : 1;
    /* exclude_spc_llr_ack - Bits[9:9], RWS_L, default = 1'b0 
        
               Exclude Special LLR Ack
             
     */
    UINT32 exclude_spc_pm_linkwidth : 1;
    /* exclude_spc_pm_linkwidth - Bits[10:10], RWS_L, default = 1'b0 
        
               Exclude Special Power Mangagment Link Width
             
     */
    UINT32 exclude_spc_pm_enterl0s : 1;
    /* exclude_spc_pm_enterl0s - Bits[11:11], RWS_L, default = 1'b0 
        
               Exclude Special Power Management L0sEnter
             
     */
    UINT32 rsvd_12 : 1;
    /* rsvd_12 - Bits[12:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 exclude_spc_pm_l1ack : 1;
    /* exclude_spc_pm_l1ack - Bits[13:13], RWS_L, default = 1'b0 
        
               Exclude Special Power Management L1 Ack
             
     */
    UINT32 rsvd_14 : 6;
    /* rsvd_14 - Bits[19:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 include_sp_idle : 1;
    /* include_sp_idle - Bits[20:20], RWS_L, default = 1'b0 
        
               Include Special Idle
             
     */
    UINT32 rsvd_21 : 3;
    /* rsvd_21 - Bits[23:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 include_spc_llr_req : 1;
    /* include_spc_llr_req - Bits[24:24], RWS_L, default = 1'b0 
        
               Include Special LLR Req
             
     */
    UINT32 include_spc_llr_ack : 1;
    /* include_spc_llr_ack - Bits[25:25], RWS_L, default = 1'b0 
        
               Include Special LLR Ack
             
     */
    UINT32 include_spc_pm_linkwidth : 1;
    /* include_spc_pm_linkwidth - Bits[26:26], RWS_L, default = 1'b0 
        
               Include Special Power-Management Link Width
             
     */
    UINT32 include_spc_pm_enterl0s : 1;
    /* include_spc_pm_enterl0s - Bits[27:27], RWS_L, default = 1'b0 
        
               Include Special Power Management Enter L0s
             
     */
    UINT32 include_spc_pm_enterl1 : 1;
    /* include_spc_pm_enterl1 - Bits[28:28], RWS_L, default = 1'b0 
        
               Include Special Power Management Enter L1
             
     */
    UINT32 include_spc_pm_l1ack : 1;
    /* include_spc_pm_l1ack - Bits[29:29], RWS_L, default = 1'b0 
        
               Include Special Power Management L1 Ack
             
     */
    UINT32 rsvd_30 : 1;
    /* rsvd_30 - Bits[30:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 include_pe_nonzero : 1;
    /* include_pe_nonzero - Bits[31:31], RWS_L, default = 1'b0 
        
               Include Parameter Exchange non-zero
             
     */
  } Bits;
  UINT32 Data;
} QPIRXEINJCTL1_QPIAGENT_DFX_LINK_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */


























/* QPIATMCTL0_QPIAGENT_DFX_LINK_REG supported on:                             */
/*       IVT_EP (0x401462F0)                                                  */
/*       IVT_EX (0x401462F0)                                                  */
/*       HSX (0x401462F0)                                                     */
/*       BDX (0x401462F0)                                                     */
/* Register default value:              0x00000000                            */
#define QPIATMCTL0_QPIAGENT_DFX_LINK_REG 0x060542F0


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.6.CFG.xml.
 * 
 *       Intel QPI ATM Control Register 0
 *       This register configures the ATM in Intel QPI (Digital Near-End Loopback)
 *     
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ll_atmmode_enable : 1;
    /* ll_atmmode_enable - Bits[2:2], RWS_L, default = 1'b0 
        
               Enable ATM operation.
             
     */
    UINT32 rsvd_3 : 3;
    /* rsvd_3 - Bits[5:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 flip_addr_31_6 : 26;
    /* flip_addr_31_6 - Bits[31:6], RWS_L, default = 26'b0 
        
               Address bit control for ATM. A 1 in any bit position will flip the 
       corresponding address bit in the packet before it is looped back inot the RX 
       path. 
             
     */
  } Bits;
  UINT32 Data;
} QPIATMCTL0_QPIAGENT_DFX_LINK_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIATMCTL1_QPIAGENT_DFX_LINK_REG supported on:                             */
/*       IVT_EP (0x401462F4)                                                  */
/*       IVT_EX (0x401462F4)                                                  */
/*       HSX (0x401462F4)                                                     */
/*       BDX (0x401462F4)                                                     */
/* Register default value:              0x00000000                            */
#define QPIATMCTL1_QPIAGENT_DFX_LINK_REG 0x060542F4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.6.CFG.xml.
 * 
 *       Intel QPI ATM Control Register 1
 *       This register has additional configuration bits for the ATM in Intel QPI
 *     
 */
typedef union {
  struct {
    UINT32 flip_addr_45_32 : 14;
    /* flip_addr_45_32 - Bits[13:0], RWS_L, default = 14'b0  */
    UINT32 flip_dnid_3_0 : 4;
    /* flip_dnid_3_0 - Bits[17:14], RWS_L, default = 3'b0  */
    UINT32 flip_rhnid_3_0 : 4;
    /* flip_rhnid_3_0 - Bits[21:18], RWS_L, default = 3'b0  */
    UINT32 flip_rsnid_3_0 : 4;
    /* flip_rsnid_3_0 - Bits[25:22], RWS_L, default = 3'b0  */
    UINT32 rsvd : 6;
    /* rsvd - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIATMCTL1_QPIAGENT_DFX_LINK_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIVIRAL_QPIAGENT_DFX_LINK_REG supported on:                               */
/*       HSX (0x401462F8)                                                     */
/*       BDX (0x401462F8)                                                     */
/* Register default value:              0x00000000                            */
#define QPIVIRAL_QPIAGENT_DFX_LINK_REG 0x060542F8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.6.CFG.xml.
 * 
 *       Intel QPI Viral Control and Status Register
 *       This register has control/status bits for Intel QPI Viral
 *     
 */
typedef union {
  struct {
    UINT32 qpi_fatal_viral_alert_enable : 1;
    /* qpi_fatal_viral_alert_enable - Bits[0:0], RW, default = 1'b0 
       
       	Masks assertion of Global Viral by Intel QPI Agent.
             
     */
    UINT32 qpi_signal_global_fatal : 1;
    /* qpi_signal_global_fatal - Bits[1:1], RW, default = 1'b0 
       
        Enables assertion of Global Fatal by Intel QPI Agent due to internal detection 
       of an uncorrectable error. 
             
     */
    UINT32 qpi_global_viral_mask : 1;
    /* qpi_global_viral_mask - Bits[2:2], RW, default = 1'b0 
       
       	Masks generation of outgoing packet headers with Viral asserted.
             
     */
    UINT32 rsvd_3 : 19;
    /* rsvd_3 - Bits[21:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 qpi_failover_status : 1;
    /* qpi_failover_status - Bits[22:22], RW1CS, default = 1'b0 
       
       	Indicates an SMI was generated due to a clk/data failover condition.
             
     */
    UINT32 rsvd_23 : 3;
    /* rsvd_23 - Bits[25:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 qpi_unc_err_set : 1;
    /* qpi_unc_err_set - Bits[26:26], RW1C, default = 1'b0 
       
        Status that indicates the Intel QPI Agent has detected an uncorrectable error.  
        
        Note 1 - if this bit is set, the Intel QPI Agent will continue to assert Global 
       Fatal/Global Viral (if enabled) 
        Note 2 - uncorrectable error used here includes QPIERRDIS masking, but does NOT 
       include QPI_MC_CTL masking 
             
     */
    UINT32 rsvd_27 : 1;
    /* rsvd_27 - Bits[27:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 qpi_pkt_viral_set : 1;
    /* qpi_pkt_viral_set - Bits[28:28], RW1C, default = 1'b0 
       
        Status that indicates the Intel QPI Agent has received a packet header with the 
       Viral bit asserted. 
        Note 1 - if this bit is set, the Intel QPI Agetn will continue to assert Global 
       Viral (if enabled) 
             
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 qpi_viral_status : 1;
    /* qpi_viral_status - Bits[30:30], RW1CS, default = 1'b0 
       
       	Indicates previous detection of a Viral condition.
             
     */
    UINT32 qpi_viral_state : 1;
    /* qpi_viral_state - Bits[31:31], RW1C, default = 1'b0 
       
        Indicates that the Intel QPI Agent is currently asserting Viral in outgoing 
       packet headers. 
             
     */
  } Bits;
  UINT32 Data;
} QPIVIRAL_QPIAGENT_DFX_LINK_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






/* QPILKPMHFRC_QPIAGENT_DFX_LINK_REG supported on:                            */
/*       IVT_EP (0x40146348)                                                  */
/*       IVT_EX (0x40146348)                                                  */
/*       HSX (0x40146348)                                                     */
/*       BDX (0x40146348)                                                     */
/* Register default value on IVT_EP:    0xCACACACA                            */
/* Register default value on IVT_EX:    0xCACACACA                            */
/* Register default value on HSX:       0x00CACACA                            */
/* Register default value on BDX:       0x00CACACA                            */
#define QPILKPMHFRC_QPIAGENT_DFX_LINK_REG 0x06054348


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.6.CFG.xml.
 * QPILKPMHFRC
 */
typedef union {
  struct {
    UINT32 force_l0p_str_sel : 4;
    /* force_l0p_str_sel - Bits[3:0], RWS_L, default = 4'hA 
        
               Start Select encodings: 
       	0000: Micro Break Point 0 (MBP[0]) 
       	0001: Micro Break Point 1 (MBP[1]) 
       	0010: Cluster Trigger Out 0 (CTO[0]) 
       	0011: Cluster Trigger Out 1 (CTO[1]) 
       	0100: Artificial Starvation Counter 0 (ASC[0]) 
       	0101: Artificial Starvation Counter 1 (ASC[1]) 
       	0110: Reserved 
       	0111: Reserved 
       	1000: MCA Correctable Error 
       	1001: MCA Uncorrectable Error 
       	1010: Response never occurs 
       	1011: Response occurs immediately 
       	1100-1111: Reserved
             
     */
    UINT32 force_l0p_stp_sel : 4;
    /* force_l0p_stp_sel - Bits[7:4], RWS_L, default = 4'hC 
        
       	Stop Select encoding: 
       	0000: Micro Break Point 0 (MBP[0]) 
       	0001: Micro Break Point 1 (MBP[1]) 
       	0010: Cluster Trigger Out 0 (CTO[0]) 
       	0011: Cluster Trigger Out 1 (CTO[1]) 
       	0100: Artificial Starvation Counter 0 (ASC[0]) 
       	0101: Artificial Starvation Counter 1 (ASC[1]) 
       	0110: Reserved 
       	0111: Reserved 
       	1000: MCA Correctable Error 
       	1001: MCA Uncorrectable Error 
        1010: Deactivate one clock after activation. Error injection on one transaction 
       will occur on the interface that is enabled.  
       	1011: Response is never deactivated. 
       	1100: Deactivate the response when the activating signal becomes false. 
       	1101-1111: Reserved
             
     */
    UINT32 force_l0s_str_sel : 4;
    /* force_l0s_str_sel - Bits[11:8], RWS_L, default = 4'hA 
        
               Start Select encodings: 
       	0000: Micro Break Point 0 (MBP[0]) 
       	0001: Micro Break Point 1 (MBP[1]) 
       	0010: Cluster Trigger Out 0 (CTO[0]) 
       	0011: Cluster Trigger Out 1 (CTO[1]) 
       	0100: Artificial Starvation Counter 0 (ASC[0]) 
       	0101: Artificial Starvation Counter 1 (ASC[1]) 
       	0110: Reserved 
       	0111: Reserved 
       	1000: MCA Correctable Error 
       	1001: MCA Uncorrectable Error 
       	1010: Response never occurs 
       	1011: Response occurs immediately 
       	1100-1111: Reserved
             
     */
    UINT32 force_l0s_stp_sel : 4;
    /* force_l0s_stp_sel - Bits[15:12], RWS_L, default = 4'hC 
        
       	Stop Select encoding: 
       	0000: Micro Break Point 0 (MBP[0]) 
       	0001: Micro Break Point 1 (MBP[1]) 
       	0010: Cluster Trigger Out 0 (CTO[0]) 
       	0011: Cluster Trigger Out 1 (CTO[1]) 
       	0100: Artificial Starvation Counter 0 (ASC[0]) 
       	0101: Artificial Starvation Counter 1 (ASC[1]) 
       	0110: Reserved 
       	0111: Reserved 
       	1000: MCA Correctable Error 
       	1001: MCA Uncorrectable Error 
        1010: Deactivate one clock after activation. Error injection on one transaction 
       will occur on the interface that is enabled.  
       	1011: Response is never deactivated. 
       	1100: Deactivate the response when the activating signal becomes false. 
       	1101-1111: Reserved
             
     */
    UINT32 force_l1_str_sel : 4;
    /* force_l1_str_sel - Bits[19:16], RWS_L, default = 4'hA 
        
               Start Select encodings: 
       	0000: Micro Break Point 0 (MBP[0]) 
       	0001: Micro Break Point 1 (MBP[1]) 
       	0010: Cluster Trigger Out 0 (CTO[0]) 
       	0011: Cluster Trigger Out 1 (CTO[1]) 
       	0100: Artificial Starvation Counter 0 (ASC[0]) 
       	0101: Artificial Starvation Counter 1 (ASC[1]) 
       	0110: Reserved 
       	0111: Reserved 
       	1000: MCA Correctable Error 
       	1001: MCA Uncorrectable Error 
       	1010: Response never occurs 
       	1011: Response occurs immediately 
       	1100-1111: Reserved
             
     */
    UINT32 force_l1_stp_sel : 4;
    /* force_l1_stp_sel - Bits[23:20], RWS_L, default = 4'hC 
        
       	Stop Select encoding: 
       	0000: Micro Break Point 0 (MBP[0]) 
       	0001: Micro Break Point 1 (MBP[1]) 
       	0010: Cluster Trigger Out 0 (CTO[0]) 
       	0011: Cluster Trigger Out 1 (CTO[1]) 
       	0100: Artificial Starvation Counter 0 (ASC[0]) 
       	0101: Artificial Starvation Counter 1 (ASC[1]) 
       	0110: Reserved 
       	0111: Reserved 
       	1000: MCA Correctable Error 
       	1001: MCA Uncorrectable Error 
        1010: Deactivate one clock after activation. Error injection on one transaction 
       will occur on the interface that is enabled.  
       	1011: Response is never deactivated. 
       	1100: Deactivate the response when the activating signal becomes false. 
       	1101-1111: Reserved
             
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPILKPMHFRC_QPIAGENT_DFX_LINK_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPILKPMHFRCST_QPIAGENT_DFX_LINK_REG supported on:                          */
/*       IVT_EP (0x4014634C)                                                  */
/*       IVT_EX (0x4014634C)                                                  */
/*       HSX (0x4014634C)                                                     */
/*       BDX (0x4014634C)                                                     */
/* Register default value:              0x00000000                            */
#define QPILKPMHFRCST_QPIAGENT_DFX_LINK_REG 0x0605434C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.6.CFG.xml.
 * qpilkpmhfrcst
 */
typedef union {
  struct {
    UINT32 force_l0p_st : 1;
    /* force_l0p_st - Bits[0:0], RW1CS, default = 1'b0  */
    UINT32 force_l0s_st : 1;
    /* force_l0s_st - Bits[1:1], RW1CS, default = 1'b0  */
    UINT32 force_l1_st : 1;
    /* force_l1_st - Bits[2:2], RW1CS, default = 1'b0  */
    UINT32 rsvd : 29;
    /* rsvd - Bits[31:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPILKPMHFRCST_QPIAGENT_DFX_LINK_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */






/* QPIDBGERRSTDIS0_QPIAGENT_DFX_LINK_REG supported on:                        */
/*       IVT_EP (0x40146354)                                                  */
/*       IVT_EX (0x40146354)                                                  */
/*       HSX (0x40146354)                                                     */
/*       BDX (0x40146354)                                                     */
/* Register default value:              0x00000000                            */
#define QPIDBGERRSTDIS0_QPIAGENT_DFX_LINK_REG 0x06054354


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.6.CFG.xml.
 * qpidbgerrstdis0
 */
typedef union {
  struct {
    UINT32 disable_err : 32;
    /* disable_err - Bits[31:0], RWS, default = 32'b0 
        
               Disable bits for each of the corresponding error types defined in 
       QPIDBGERRST0 
             
     */
  } Bits;
  UINT32 Data;
} QPIDBGERRSTDIS0_QPIAGENT_DFX_LINK_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */
















/* QPIDBGERRST4_QPIAGENT_DFX_LINK_REG supported on:                           */
/*       IVT_EP (0x40146370)                                                  */
/*       IVT_EX (0x40146370)                                                  */
/*       HSX (0x40146370)                                                     */
/*       BDX (0x40146370)                                                     */
/* Register default value:              0x00000000                            */
#define QPIDBGERRST4_QPIAGENT_DFX_LINK_REG 0x06054370


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.6.CFG.xml.
 * qpidbgerrst4
 */
typedef union {
  struct {
    UINT32 ctrl_illegal_llrst : 1;
    /* ctrl_illegal_llrst - Bits[0:0], RW1CS, default = 1'b0  */
    UINT32 ctrl_rf_parity_err : 1;
    /* ctrl_rf_parity_err - Bits[1:1], RW1CS, default = 1'b0  */
    UINT32 ctrl_spare : 30;
    /* ctrl_spare - Bits[31:2], RW1CS, default = 30'b0  */
  } Bits;
  UINT32 Data;
} QPIDBGERRST4_QPIAGENT_DFX_LINK_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */






/* QPIDBGSMST_QPIAGENT_DFX_LINK_REG supported on:                             */
/*       IVT_EP (0x40146378)                                                  */
/*       IVT_EX (0x40146378)                                                  */
/*       HSX (0x40146378)                                                     */
/*       BDX (0x40146378)                                                     */
/* Register default value:              0x00000000                            */
#define QPIDBGSMST_QPIAGENT_DFX_LINK_REG 0x06054378


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.6.CFG.xml.
 * qpidbgsmst
 */
typedef union {
  struct {
    UINT32 llrstsm : 3;
    /* llrstsm - Bits[2:0], ROS_V, default = 3'b0  */
    UINT32 lrsm : 3;
    /* lrsm - Bits[5:3], ROS_V, default = 3'b0  */
    UINT32 rsvd_6 : 2;
    /* rsvd_6 - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rrsm : 2;
    /* rrsm - Bits[9:8], ROS_V, default = 1'b0  */
    UINT32 pmtsm : 4;
    /* pmtsm - Bits[13:10], ROS_V, default = 4'b0  */
    UINT32 rsvd_14 : 1;
    /* rsvd_14 - Bits[14:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 dbgtsm : 3;
    /* dbgtsm - Bits[17:15], ROS_V, default = 3'b0  */
    UINT32 rx_active : 1;
    /* rx_active - Bits[18:18], ROS_V, default = 1'b0  */
    UINT32 tx_active : 1;
    /* tx_active - Bits[19:19], ROS_V, default = 1'b0  */
    UINT32 tx_llr_numfreebuf_le148 : 1;
    /* tx_llr_numfreebuf_le148 - Bits[20:20], ROS_V, default = 1'b0  */
    UINT32 pmrsm : 2;
    /* pmrsm - Bits[22:21], ROS_V, default = 2'b0  */
    UINT32 rsvd_23 : 9;
    /* rsvd_23 - Bits[31:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIDBGSMST_QPIAGENT_DFX_LINK_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */






/* QPILKDELS_QPIAGENT_DFX_LINK_REG supported on:                              */
/*       IVT_EP (0x401463A4)                                                  */
/*       IVT_EX (0x401463A4)                                                  */
/*       HSX (0x401463A4)                                                     */
/*       BDX (0x401463A4)                                                     */
/* Register default value:              0x00000000                            */
#define QPILKDELS_QPIAGENT_DFX_LINK_REG 0x060543A4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.6.CFG.xml.
 * 
 *       QPI Link Layer Debug Event Lane Select
 *       This register selects which debug events are placed onto the debug ring 
 * for the QPI0 cluster. 
 *     
 */
typedef union {
  struct {
    UINT32 debugbus_lane0_sel : 7;
    /* debugbus_lane0_sel - Bits[6:0], RWS_L, default = 7'b0 
       Intel QPI Link Debug bus lane 0 select
     */
    UINT32 debugbus_lane1_sel : 7;
    /* debugbus_lane1_sel - Bits[13:7], RWS_L, default = 7'b0 
       Intel QPI Link Debug bus lane 1 select
     */
    UINT32 debugbus_lane2_sel : 7;
    /* debugbus_lane2_sel - Bits[20:14], RWS_L, default = 7'b0 
       Intel QPI Link Debug bus lane 2 select
     */
    UINT32 debugbus_lane3_sel : 7;
    /* debugbus_lane3_sel - Bits[27:21], RWS_L, default = 7'b0 
       Intel QPI Link Debug bus lane 3 select
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPILKDELS_QPIAGENT_DFX_LINK_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPILKDELS1_QPIAGENT_DFX_LINK_REG supported on:                             */
/*       IVT_EP (0x401463A8)                                                  */
/*       IVT_EX (0x401463A8)                                                  */
/*       HSX (0x401463A8)                                                     */
/*       BDX (0x401463A8)                                                     */
/* Register default value:              0x00000000                            */
#define QPILKDELS1_QPIAGENT_DFX_LINK_REG 0x060543A8


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.6.CFG.xml.
 * 
 *       Intel QPI Link Layer Debug Event Lane Select 1
 *       This register selects which set of debug event are available from this 
 * cluster for both display on the debug ring and for use in performance 
 * monitoring. 
 *       Which set of debug events is selected is controllable on a lane-by-lane 
 * basis, meaning that each 8-bit lane can have a different set of debug events 
 * selected from any other lane 
 *     
 */
typedef union {
  struct {
    UINT32 debugbus_lane4_sel : 7;
    /* debugbus_lane4_sel - Bits[6:0], RWS_L, default = 7'b0 
       Intel QPI Link Debug bus lane 4 select
     */
    UINT32 debugbus_lane5_sel : 7;
    /* debugbus_lane5_sel - Bits[13:7], RWS_L, default = 7'b0 
       Intel QPI Link Debug bus lane 5 select
     */
    UINT32 debugbus_lane6_sel : 7;
    /* debugbus_lane6_sel - Bits[20:14], RWS_L, default = 7'b0 
       Intel QPI Link Debug bus lane 6 select
     */
    UINT32 debugbus_lane7_sel : 7;
    /* debugbus_lane7_sel - Bits[27:21], RWS_L, default = 7'b0 
       Intel QPI Link Debug bus lane 7 select
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPILKDELS1_QPIAGENT_DFX_LINK_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPILKDELS2_QPIAGENT_DFX_LINK_REG supported on:                             */
/*       IVT_EP (0x401463AC)                                                  */
/*       IVT_EX (0x401463AC)                                                  */
/*       HSX (0x401463AC)                                                     */
/*       BDX (0x401463AC)                                                     */
/* Register default value:              0x00000000                            */
#define QPILKDELS2_QPIAGENT_DFX_LINK_REG 0x060543AC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.6.CFG.xml.
 * 
 *       Intel QPI Link Layer Debug Event Lane Select 2
 *       This register selects which set of debug event are available from this 
 * cluster for both display on the debug ring and for use in performance 
 * monitoring.  
 *       Which set of debug events is selected is controllable on a lane-by-lane 
 * basis, meaning that each 8-bit lane can have a different set of debug events 
 * selected from any other lane 
 *     
 */
typedef union {
  struct {
    UINT32 debugbus_lane8_sel : 7;
    /* debugbus_lane8_sel - Bits[6:0], RWS_L, default = 7'b0 
       Intel QPI Link Debug bus lane 8 select
     */
    UINT32 rsvd : 25;
    /* rsvd - Bits[31:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPILKDELS2_QPIAGENT_DFX_LINK_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */














/* SVID_QPIAGENT_DFX_LINK_REG supported on:                                   */
/*       IVT_EP (0x2014602C)                                                  */
/*       IVT_EX (0x2014602C)                                                  */
/* Register default value:              0x8086                                */
#define SVID_QPIAGENT_DFX_LINK_REG 0x0605202C



/* SDID_QPIAGENT_DFX_LINK_REG supported on:                                   */
/*       IVT_EP (0x2014602E)                                                  */
/*       IVT_EX (0x2014602E)                                                  */
/* Register default value:              0x0000                                */
#define SDID_QPIAGENT_DFX_LINK_REG 0x0605202E



/* PXPCAP_QPIAGENT_DFX_LINK_REG supported on:                                 */
/*       IVT_EP (0x40146040)                                                  */
/*       IVT_EX (0x40146040)                                                  */
/* Register default value:              0x00910010                            */
#define PXPCAP_QPIAGENT_DFX_LINK_REG 0x06054040



/* PXPENHCAP_QPIAGENT_DFX_LINK_REG supported on:                              */
/*       IVT_EP (0x40146100)                                                  */
/*       IVT_EX (0x40146100)                                                  */
/* Register default value:              0x00000000                            */
#define PXPENHCAP_QPIAGENT_DFX_LINK_REG 0x06054100













/* QPILKPRIVC14_QPIAGENT_DFX_LINK_REG supported on:                           */
/*       IVT_EP (0x401462E8)                                                  */
/*       IVT_EX (0x401462E8)                                                  */
/* Register default value:              0x00000000                            */
#define QPILKPRIVC14_QPIAGENT_DFX_LINK_REG 0x060542E8



/* QPIL0PFRCSTCTRL_QPIAGENT_DFX_LINK_REG supported on:                        */
/*       IVT_EP (0x40146380)                                                  */
/*       IVT_EX (0x40146380)                                                  */
/* Register default value:              0x00000000                            */
#define QPIL0PFRCSTCTRL_QPIAGENT_DFX_LINK_REG 0x06054380



/* QPIL0PFRCSTDATA_QPIAGENT_DFX_LINK_REG supported on:                        */
/*       IVT_EP (0x40146384)                                                  */
/*       IVT_EX (0x40146384)                                                  */
/* Register default value:              0x00000000                            */
#define QPIL0PFRCSTDATA_QPIAGENT_DFX_LINK_REG 0x06054384



/* QPILKRNG_QPIAGENT_DFX_LINK_REG supported on:                               */
/*       IVT_EP (0x401463A0)                                                  */
/*       IVT_EX (0x401463A0)                                                  */
/* Register default value:              0x036DB6DB                            */
#define QPILKRNG_QPIAGENT_DFX_LINK_REG 0x060543A0



/* QPILKDELS3_QPIAGENT_DFX_LINK_REG supported on:                             */
/*       IVT_EP (0x401463B0)                                                  */
/*       IVT_EX (0x401463B0)                                                  */
/* Register default value:              0x00000000                            */
#define QPILKDELS3_QPIAGENT_DFX_LINK_REG 0x060543B0



/* QPIDBGFLITCOMP8B_RX_QPIAGENT_DFX_LINK_REG supported on:                    */
/*       IVT_EP (0x401463C0)                                                  */
/*       IVT_EX (0x401463C0)                                                  */
/* Register default value:              0x00000000                            */
#define QPIDBGFLITCOMP8B_RX_QPIAGENT_DFX_LINK_REG 0x060543C0



/* QPIDBGFLITCOMP18B_RX_QPIAGENT_DFX_LINK_REG supported on:                   */
/*       IVT_EP (0x401463C4)                                                  */
/*       IVT_EX (0x401463C4)                                                  */
/* Register default value:              0x00000000                            */
#define QPIDBGFLITCOMP18B_RX_QPIAGENT_DFX_LINK_REG 0x060543C4



/* QPIDBGFLITCOMP36B_RX_QPIAGENT_DFX_LINK_REG supported on:                   */
/*       IVT_EP (0x401463C8)                                                  */
/*       IVT_EX (0x401463C8)                                                  */
/* Register default value:              0x00000000                            */
#define QPIDBGFLITCOMP36B_RX_QPIAGENT_DFX_LINK_REG 0x060543C8



/* QPIDBGFLITCOMP8B_TX_QPIAGENT_DFX_LINK_REG supported on:                    */
/*       IVT_EP (0x401463CC)                                                  */
/*       IVT_EX (0x401463CC)                                                  */
/* Register default value:              0x00000000                            */
#define QPIDBGFLITCOMP8B_TX_QPIAGENT_DFX_LINK_REG 0x060543CC



/* QPIDBGFLITCOMP18B_TX_QPIAGENT_DFX_LINK_REG supported on:                   */
/*       IVT_EP (0x401463D0)                                                  */
/*       IVT_EX (0x401463D0)                                                  */
/* Register default value:              0x00000000                            */
#define QPIDBGFLITCOMP18B_TX_QPIAGENT_DFX_LINK_REG 0x060543D0



/* QPIDBGFLITCOMP36B_TX_QPIAGENT_DFX_LINK_REG supported on:                   */
/*       IVT_EP (0x401463D4)                                                  */
/*       IVT_EX (0x401463D4)                                                  */
/* Register default value:              0x00000000                            */
#define QPIDBGFLITCOMP36B_TX_QPIAGENT_DFX_LINK_REG 0x060543D4



#endif /* QPIAGENT_DFX_LINK_h */
