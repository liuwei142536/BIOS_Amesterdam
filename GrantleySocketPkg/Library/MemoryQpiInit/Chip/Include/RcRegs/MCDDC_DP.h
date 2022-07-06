/* Date Stamp: 8/23/2014 */

#ifndef MCDDC_DP_h
#define MCDDC_DP_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* MCDDC_DP_IVT_DEV 16                                                        */
/* MCDDC_DP_IVT_FUN 6                                                         */
/* For HSX_HOST:                                                              */
/* MCDDC_DP_HSX_DEV 20                                                        */
/* MCDDC_DP_HSX_FUN 2                                                         */
/* For BDX_HOST:                                                              */
/* MCDDC_DP_BDX_DEV 20                                                        */
/* MCDDC_DP_BDX_FUN 2                                                         */

/* VID_MCDDC_DP_REG supported on:                                             */
/*       IVT_EP (0x20186000)                                                  */
/*       IVT_EX (0x20186000)                                                  */
/*       HSX (0x201A2000)                                                     */
/*       BDX (0x201A2000)                                                     */
/* Register default value:              0x8086                                */
#define VID_MCDDC_DP_REG 0x0C012000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
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
} VID_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* DID_MCDDC_DP_REG supported on:                                             */
/*       IVT_EP (0x20186002)                                                  */
/*       IVT_EX (0x20186002)                                                  */
/*       HSX (0x201A2002)                                                     */
/*       BDX (0x201A2002)                                                     */
/* Register default value on IVT_EP:    0x0EB6                                */
/* Register default value on IVT_EX:    0x0EB6                                */
/* Register default value on HSX:       0x2FB2                                */
/* Register default value on BDX:       0x6FB2                                */
#define DID_MCDDC_DP_REG 0x0C012002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6FB2 
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
} DID_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* PCICMD_MCDDC_DP_REG supported on:                                          */
/*       IVT_EP (0x20186004)                                                  */
/*       IVT_EX (0x20186004)                                                  */
/*       HSX (0x201A2004)                                                     */
/*       BDX (0x201A2004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_MCDDC_DP_REG 0x0C012004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
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
} PCICMD_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* PCISTS_MCDDC_DP_REG supported on:                                          */
/*       IVT_EP (0x20186006)                                                  */
/*       IVT_EX (0x20186006)                                                  */
/*       HSX (0x201A2006)                                                     */
/*       BDX (0x201A2006)                                                     */
/* Register default value:              0x0010                                */
#define PCISTS_MCDDC_DP_REG 0x0C012006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.2.CFG.xml.
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
} PCISTS_MCDDC_DP_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_MCDDC_DP_REG supported on:                                             */
/*       IVT_EP (0x10186008)                                                  */
/*       IVT_EX (0x10186008)                                                  */
/*       HSX (0x101A2008)                                                     */
/*       BDX (0x101A2008)                                                     */
/* Register default value:              0x00                                  */
#define RID_MCDDC_DP_REG 0x0C011008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
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
} RID_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_MCDDC_DP_REG supported on:                                          */
/*       IVT_EP (0x10186009)                                                  */
/*       IVT_EX (0x10186009)                                                  */
/*       HSX (0x101A2009)                                                     */
/*       BDX (0x101A2009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_MCDDC_DP_REG 0x0C011009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.2.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_MCDDC_DP_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_MCDDC_DP_REG supported on:                                          */
/*       IVT_EP (0x2018600A)                                                  */
/*       IVT_EX (0x2018600A)                                                  */
/*       HSX (0x201A200A)                                                     */
/*       BDX (0x201A200A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_MCDDC_DP_REG 0x0C01200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
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
} CCR_N1_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CLSR_MCDDC_DP_REG supported on:                                            */
/*       IVT_EP (0x1018600C)                                                  */
/*       IVT_EX (0x1018600C)                                                  */
/*       HSX (0x101A200C)                                                     */
/*       BDX (0x101A200C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_MCDDC_DP_REG 0x0C01100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
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
} CLSR_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* PLAT_MCDDC_DP_REG supported on:                                            */
/*       IVT_EP (0x1018600D)                                                  */
/*       IVT_EX (0x1018600D)                                                  */
/*       HSX (0x101A200D)                                                     */
/*       BDX (0x101A200D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_MCDDC_DP_REG 0x0C01100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
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
} PLAT_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* HDR_MCDDC_DP_REG supported on:                                             */
/*       IVT_EP (0x1018600E)                                                  */
/*       IVT_EX (0x1018600E)                                                  */
/*       HSX (0x101A200E)                                                     */
/*       BDX (0x101A200E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_MCDDC_DP_REG 0x0C01100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
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
} HDR_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* BIST_MCDDC_DP_REG supported on:                                            */
/*       IVT_EP (0x1018600F)                                                  */
/*       IVT_EX (0x1018600F)                                                  */
/*       HSX (0x101A200F)                                                     */
/*       BDX (0x101A200F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_MCDDC_DP_REG 0x0C01100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
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
} BIST_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* SVID_MCDDC_DP_REG supported on:                                            */
/*       IVT_EP (0x2018602C)                                                  */
/*       IVT_EX (0x2018602C)                                                  */
/*       HSX (0x201A202C)                                                     */
/*       BDX (0x201A202C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_MCDDC_DP_REG 0x0C01202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * generated by critter 20_2_0x02c
 */
typedef union {
  struct {
    UINT16 subsystem_vendor_identification_number : 16;
    /* subsystem_vendor_identification_number - Bits[15:0], RW_O, default = 16'b1000000010000110 
       The default value specifies Intel but can be set to any value once after reset.
     */
  } Bits;
  UINT16 Data;
} SVID_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* SDID_MCDDC_DP_REG supported on:                                            */
/*       IVT_EP (0x2018602E)                                                  */
/*       IVT_EX (0x2018602E)                                                  */
/*       HSX (0x201A202E)                                                     */
/*       BDX (0x201A202E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_MCDDC_DP_REG 0x0C01202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * generated by critter 20_2_0x02e
 */
typedef union {
  struct {
    UINT16 subsystem_device_identification_number : 16;
    /* subsystem_device_identification_number - Bits[15:0], RW_O, default = 16'b0000000000000000 
       Assigned by the subsystem vendor to uniquely identify the subsystem
     */
  } Bits;
  UINT16 Data;
} SDID_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_MCDDC_DP_REG supported on:                                          */
/*       IVT_EP (0x10186034)                                                  */
/*       IVT_EX (0x10186034)                                                  */
/*       HSX (0x101A2034)                                                     */
/*       BDX (0x101A2034)                                                     */
/* Register default value on IVT_EP:    0x40                                  */
/* Register default value on IVT_EX:    0x40                                  */
/* Register default value on HSX:       0x00                                  */
/* Register default value on BDX:       0x40                                  */
#define CAPPTR_MCDDC_DP_REG 0x0C011034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
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
} CAPPTR_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* INTL_MCDDC_DP_REG supported on:                                            */
/*       IVT_EP (0x1018603C)                                                  */
/*       IVT_EX (0x1018603C)                                                  */
/*       HSX (0x101A203C)                                                     */
/*       BDX (0x101A203C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_MCDDC_DP_REG 0x0C01103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
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
} INTL_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* INTPIN_MCDDC_DP_REG supported on:                                          */
/*       IVT_EP (0x1018603D)                                                  */
/*       IVT_EX (0x1018603D)                                                  */
/*       HSX (0x101A203D)                                                     */
/*       BDX (0x101A203D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_MCDDC_DP_REG 0x0C01103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
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
} INTPIN_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* MINGNT_MCDDC_DP_REG supported on:                                          */
/*       IVT_EP (0x1018603E)                                                  */
/*       IVT_EX (0x1018603E)                                                  */
/*       HSX (0x101A203E)                                                     */
/*       BDX (0x101A203E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_MCDDC_DP_REG 0x0C01103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
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
} MINGNT_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_MCDDC_DP_REG supported on:                                          */
/*       IVT_EP (0x1018603F)                                                  */
/*       IVT_EX (0x1018603F)                                                  */
/*       HSX (0x101A203F)                                                     */
/*       BDX (0x101A203F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_MCDDC_DP_REG 0x0C01103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
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
} MAXLAT_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* PXPCAP_MCDDC_DP_REG supported on:                                          */
/*       IVT_EP (0x40186040)                                                  */
/*       IVT_EX (0x40186040)                                                  */
/*       HSX (0x401A2040)                                                     */
/*       BDX (0x401A2040)                                                     */
/* Register default value:              0x00910010                            */
#define PXPCAP_MCDDC_DP_REG 0x0C014040
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * generated by critter 20_2_0x040
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
} PXPCAP_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* ROUNDTRIP0_MCDDC_DP_REG supported on:                                      */
/*       IVT_EP (0x40186080)                                                  */
/*       IVT_EX (0x40186080)                                                  */
/*       HSX (0x401A2080)                                                     */
/*       BDX (0x401A2080)                                                     */
/* Register default value:              0x0B0B0B0B                            */
#define ROUNDTRIP0_MCDDC_DP_REG 0x0C014080
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * generated by critter 20_2_0x080
 */
typedef union {
  struct {
    UINT32 rt_rank0 : 8;
    /* rt_rank0 - Bits[7:0], RW, default = 8'b00001011 
       Rank 0 round trip latency in QCLK, 88 max configurable value
     */
    UINT32 rt_rank1 : 8;
    /* rt_rank1 - Bits[15:8], RW, default = 8'b00001011 
       Rank 1 round trip latency in QCLK
     */
    UINT32 rt_rank2 : 8;
    /* rt_rank2 - Bits[23:16], RW, default = 8'b00001011 
       Rank 2 round trip latency in QCLK
     */
    UINT32 rt_rank3 : 8;
    /* rt_rank3 - Bits[31:24], RW, default = 8'b00001011 
       Rank 3 round trip latency in QCLK
     */
  } Bits;
  UINT32 Data;
} ROUNDTRIP0_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* ROUNDTRIP1_MCDDC_DP_REG supported on:                                      */
/*       IVT_EP (0x40186084)                                                  */
/*       IVT_EX (0x40186084)                                                  */
/*       HSX (0x401A2084)                                                     */
/*       BDX (0x401A2084)                                                     */
/* Register default value:              0x0B0B0B0B                            */
#define ROUNDTRIP1_MCDDC_DP_REG 0x0C014084
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * generated by critter 20_2_0x084
 */
typedef union {
  struct {
    UINT32 rt_rank4 : 8;
    /* rt_rank4 - Bits[7:0], RW, default = 8'b00001011 
       Rank 4 round trip latency in QCLK
     */
    UINT32 rt_rank5 : 8;
    /* rt_rank5 - Bits[15:8], RW, default = 8'b00001011 
       Rank 5 round trip latency in QCLK
     */
    UINT32 rt_rank6 : 8;
    /* rt_rank6 - Bits[23:16], RW, default = 8'b00001011 
       Rank 6 round trip latency in QCLK
     */
    UINT32 rt_rank7 : 8;
    /* rt_rank7 - Bits[31:24], RW, default = 8'b00001011 
       Rank 7 round trip latency in QCLK
     */
  } Bits;
  UINT32 Data;
} ROUNDTRIP1_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* IOLATENCY0_MCDDC_DP_REG supported on:                                      */
/*       IVT_EP (0x4018608C)                                                  */
/*       IVT_EX (0x4018608C)                                                  */
/*       HSX (0x401A208C)                                                     */
/*       BDX (0x401A208C)                                                     */
/* Register default value:              0x00000000                            */
#define IOLATENCY0_MCDDC_DP_REG 0x0C01408C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * generated by critter 20_2_0x08c
 */
typedef union {
  struct {
    UINT32 io_lat_rank0 : 4;
    /* io_lat_rank0 - Bits[3:0], RW, default = 4'b0000 
       Rank 0 IO latency in QCLK
     */
    UINT32 io_lat_rank1 : 4;
    /* io_lat_rank1 - Bits[7:4], RW, default = 4'b0000 
       Rank 1 IO latency in QCLK
     */
    UINT32 io_lat_rank2 : 4;
    /* io_lat_rank2 - Bits[11:8], RW, default = 4'b0000 
       Rank 2 IO latency in QCLK
     */
    UINT32 io_lat_rank3 : 4;
    /* io_lat_rank3 - Bits[15:12], RW, default = 4'b0000 
       Rank 3 IO latency in QCLK
     */
    UINT32 io_lat_rank4 : 4;
    /* io_lat_rank4 - Bits[19:16], RW, default = 4'b0000 
       Rank 4 IO latency in QCLK
     */
    UINT32 io_lat_rank5 : 4;
    /* io_lat_rank5 - Bits[23:20], RW, default = 4'b0000 
       Rank 5 IO latency in QCLK
     */
    UINT32 io_lat_rank6 : 4;
    /* io_lat_rank6 - Bits[27:24], RW, default = 4'b0000 
       Rank 6 IO latency in QCLK
     */
    UINT32 io_lat_rank7 : 4;
    /* io_lat_rank7 - Bits[31:28], RW, default = 4'b0000 
       Rank 7 IO latency in QCLK
     */
  } Bits;
  UINT32 Data;
} IOLATENCY0_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* IOLATENCY1_MCDDC_DP_REG supported on:                                      */
/*       IVT_EP (0x40186090)                                                  */
/*       IVT_EX (0x40186090)                                                  */
/*       HSX (0x401A2090)                                                     */
/*       BDX (0x401A2090)                                                     */
/* Register default value:              0x00000000                            */
#define IOLATENCY1_MCDDC_DP_REG 0x0C014090
#if defined(IVT_HOST) || defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file IVX\1.16.6.CFG.xml.
 */
typedef union {
  struct {
    UINT32 io_lat_io_comp : 8;
    /* io_lat_io_comp - Bits[7:0], RW, default = 8'b00000000  */
    UINT32 rsvd : 24;
    /* rsvd - Bits[31:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} IOLATENCY1_MCDDC_DP_IVT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(HSX_HOST) */



#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * generated by critter 20_2_0x090
 */
typedef union {
  struct {
    UINT32 io_lat_io_comp : 8;
    /* io_lat_io_comp - Bits[7:0], RW, default = 8'b00000000  */
    UINT32 cr_wrcrc_enable : 1;
    /* cr_wrcrc_enable - Bits[8:8], RW, default = 1'b0 
       1 = DDR4 write CRC is enabled // chgbdx
       0 = DDR4 write CRC is disabled // chgbdx
     */
    UINT32 cr_crc_err_inj : 8;
    /* cr_crc_err_inj - Bits[16:9], RW, default = 8'b00000000 
       All the bits those are high, lead to inversion of correct CRC in the 8 // chgbdx
       bit CRC generated per device (be it x4 or x8) // chgbdx
       Note that there is no provision for injecting error in the all-ones of the // 
       chgbdx 
       9th beat of X8 device CRC // chgbdx
     */
    UINT32 cr_oneshot_err_inj : 1;
    /* cr_oneshot_err_inj - Bits[17:17], RW, default = 1'b0 
       When this bit is set high, then the first write operation is injected with // 
       chgbdx 
       a CRC error as defined by cr_crc_err_inj field. The subsequent writes are // 
       chgbdx 
       not injected with errors. // chgbdx
     */
    UINT32 rsvd : 14;
    /* rsvd - Bits[31:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} IOLATENCY1_MCDDC_DP_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */







/* PXPENHCAP_MCDDC_DP_REG supported on:                                       */
/*       IVT_EP (0x40186100)                                                  */
/*       IVT_EX (0x40186100)                                                  */
/*       HSX (0x401A2100)                                                     */
/*       BDX (0x401A2100)                                                     */
/* Register default value:              0x00000000                            */
#define PXPENHCAP_MCDDC_DP_REG 0x0C014100
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
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
} PXPENHCAP_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CORRERRCNT_0_MCDDC_DP_REG supported on:                                    */
/*       IVT_EP (0x40186104)                                                  */
/*       IVT_EX (0x40186104)                                                  */
/*       HSX (0x401A2104)                                                     */
/*       BDX (0x401A2104)                                                     */
/* Register default value:              0x00000000                            */
#define CORRERRCNT_0_MCDDC_DP_REG 0x0C014104
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Per Rank corrected error counters.
 */
typedef union {
  struct {
    UINT32 cor_err_cnt_0 : 15;
    /* cor_err_cnt_0 - Bits[14:0], RWS_LV, default = 15'b000000000000000 
       The corrected error count for this rank. Hardware automatically clear this field 
       when the corresponding OVERFLOW_x bit is changing from 0 to 1. 
       
       This counter increments in number of cacheline accesses - not by codewords. On a 
       read access, if either of the codewords or both codewords have a corrected 
       error, this counter increments by 1. 
       Register: DEVTAG_CNTL<Rank>, Field FAILDEVICE: This field is updated once per 
       cacheline access not by codeword. On a read access, the device is logged as 
       follows 
       Corr_Err_On_CodeWord_0 \ Corr_Err_On_CoreWord_1 Device Logged
       Yes\No__Corrected Device from CodeWord0
       No\Yes__Corrected Device from CodeWord1
       Yes\Yes__Corrected Device from CodeWord0
     */
    UINT32 overflow_0 : 1;
    /* overflow_0 - Bits[15:15], RW1CS, default = 1'b0 
       The corrected error count for this rank has been overflowed. Once set it can 
       only be cleared via a write from BIOS. 
     */
    UINT32 cor_err_cnt_1 : 15;
    /* cor_err_cnt_1 - Bits[30:16], RWS_LV, default = 15'b000000000000000 
       The corrected error count for this rank. Hardware automatically clear this field 
       when the corresponding OVERFLOW_x bit is changing from 0 to 1. 
       
       This counter increments in number of cacheline accesses - not by codewords. On a 
       read access, if either of the codewords or both codewords have a corrected 
       error, this counter increments by 1. 
       Register: DEVTAG_CNTL<Rank>, Field FAILDEVICE: This field is updated once per 
       cacheline access not by codeword. On a read access, the device is logged as 
       follows 
       Corr_Err_On_CodeWord_0 \ Corr_Err_On_CoreWord_1 Device Logged
       Yes\No___Corrected Device from CodeWord0
       No\Yes___Corrected Device from CodeWord1
       Yes\Yes__Corrected Device from CodeWord0
     */
    UINT32 overflow_1 : 1;
    /* overflow_1 - Bits[31:31], RW1CS, default = 1'b0 
       The corrected error count for this rank has been overflowed. Once set it can 
       only be cleared via a write from BIOS. 
     */
  } Bits;
  UINT32 Data;
} CORRERRCNT_0_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CORRERRCNT_1_MCDDC_DP_REG supported on:                                    */
/*       IVT_EP (0x40186108)                                                  */
/*       IVT_EX (0x40186108)                                                  */
/*       HSX (0x401A2108)                                                     */
/*       BDX (0x401A2108)                                                     */
/* Register default value:              0x00000000                            */
#define CORRERRCNT_1_MCDDC_DP_REG 0x0C014108
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Per Rank corrected error counters.
 */
typedef union {
  struct {
    UINT32 cor_err_cnt_2 : 15;
    /* cor_err_cnt_2 - Bits[14:0], RWS_LV, default = 15'b000000000000000 
       The corrected error count for this rank.
     */
    UINT32 overflow_2 : 1;
    /* overflow_2 - Bits[15:15], RW1CS, default = 1'b0 
       The corrected error count has crested over the limit for this rank. Once set it 
       can only be cleared via a write from BIOS. 
     */
    UINT32 cor_err_cnt_3 : 15;
    /* cor_err_cnt_3 - Bits[30:16], RWS_LV, default = 15'b000000000000000 
       The corrected error count for this rank.
     */
    UINT32 overflow_3 : 1;
    /* overflow_3 - Bits[31:31], RW1CS, default = 1'b0 
       The corrected error count has crested over the limit for this rank. Once set it 
       can only be cleared via a write from BIOS. 
     */
  } Bits;
  UINT32 Data;
} CORRERRCNT_1_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CORRERRCNT_2_MCDDC_DP_REG supported on:                                    */
/*       IVT_EP (0x4018610C)                                                  */
/*       IVT_EX (0x4018610C)                                                  */
/*       HSX (0x401A210C)                                                     */
/*       BDX (0x401A210C)                                                     */
/* Register default value:              0x00000000                            */
#define CORRERRCNT_2_MCDDC_DP_REG 0x0C01410C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Per Rank corrected error counters.
 */
typedef union {
  struct {
    UINT32 cor_err_cnt_4 : 15;
    /* cor_err_cnt_4 - Bits[14:0], RWS_LV, default = 15'b000000000000000 
       The corrected error count for this rank.
     */
    UINT32 overflow_4 : 1;
    /* overflow_4 - Bits[15:15], RW1CS, default = 1'b0 
       The corrected error count has crested over the limit for this rank. Once set it 
       can only be cleared via a write from BIOS. 
     */
    UINT32 cor_err_cnt_5 : 15;
    /* cor_err_cnt_5 - Bits[30:16], RWS_LV, default = 15'b000000000000000 
       The corrected error count for this rank.
     */
    UINT32 overflow_5 : 1;
    /* overflow_5 - Bits[31:31], RW1CS, default = 1'b0 
       The corrected error count has crested over the limit for this rank. Once set it 
       can only be cleared via a write from BIOS. 
     */
  } Bits;
  UINT32 Data;
} CORRERRCNT_2_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CORRERRCNT_3_MCDDC_DP_REG supported on:                                    */
/*       IVT_EP (0x40186110)                                                  */
/*       IVT_EX (0x40186110)                                                  */
/*       HSX (0x401A2110)                                                     */
/*       BDX (0x401A2110)                                                     */
/* Register default value:              0x00000000                            */
#define CORRERRCNT_3_MCDDC_DP_REG 0x0C014110
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Per Rank corrected error counters.
 */
typedef union {
  struct {
    UINT32 cor_err_cnt_6 : 15;
    /* cor_err_cnt_6 - Bits[14:0], RWS_LV, default = 15'b000000000000000 
       The corrected error count for this rank.
     */
    UINT32 overflow_6 : 1;
    /* overflow_6 - Bits[15:15], RW1CS, default = 1'b0 
       The corrected error count has crested over the limit for this rank. Once set it 
       can only be cleared via a write from BIOS. 
     */
    UINT32 cor_err_cnt_7 : 15;
    /* cor_err_cnt_7 - Bits[30:16], RWS_LV, default = 15'b000000000000000 
       The corrected error count for this rank.
     */
    UINT32 overflow_7 : 1;
    /* overflow_7 - Bits[31:31], RW1CS, default = 1'b0 
       The corrected error count has crested over the limit for this rank. Once set it 
       can only be cleared via a write from BIOS. 
     */
  } Bits;
  UINT32 Data;
} CORRERRCNT_3_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CORRERRTHRSHLD_0_MCDDC_DP_REG supported on:                                */
/*       IVT_EP (0x4018611C)                                                  */
/*       IVT_EX (0x4018611C)                                                  */
/*       HSX (0x401A211C)                                                     */
/*       BDX (0x401A211C)                                                     */
/* Register default value:              0x7FFF7FFF                            */
#define CORRERRTHRSHLD_0_MCDDC_DP_REG 0x0C01411C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * This register holds the per rank corrected error thresholding value.
 */
typedef union {
  struct {
    UINT32 cor_err_th_0 : 15;
    /* cor_err_th_0 - Bits[14:0], RW_LB, default = 15'b111111111111111 
       The corrected error threshold for this rank that will be compared to the per 
       rank corrected error counter. 
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 cor_err_th_1 : 15;
    /* cor_err_th_1 - Bits[30:16], RW_LB, default = 15'b111111111111111 
       The corrected error threshold for this rank that will be compared to the per 
       rank corrected error counter. 
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CORRERRTHRSHLD_0_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CORRERRTHRSHLD_1_MCDDC_DP_REG supported on:                                */
/*       IVT_EP (0x40186120)                                                  */
/*       IVT_EX (0x40186120)                                                  */
/*       HSX (0x401A2120)                                                     */
/*       BDX (0x401A2120)                                                     */
/* Register default value:              0x7FFF7FFF                            */
#define CORRERRTHRSHLD_1_MCDDC_DP_REG 0x0C014120
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * This register holds the per rank corrected error thresholding value.
 */
typedef union {
  struct {
    UINT32 cor_err_th_2 : 15;
    /* cor_err_th_2 - Bits[14:0], RW_LB, default = 15'b111111111111111 
       The corrected error threshold for this rank that will be compared to the per 
       rank corrected error counter. 
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 cor_err_th_3 : 15;
    /* cor_err_th_3 - Bits[30:16], RW_LB, default = 15'b111111111111111 
       The corrected error threshold for this rank that will be compared to the per 
       rank corrected error counter. 
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CORRERRTHRSHLD_1_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CORRERRTHRSHLD_2_MCDDC_DP_REG supported on:                                */
/*       IVT_EP (0x40186124)                                                  */
/*       IVT_EX (0x40186124)                                                  */
/*       HSX (0x401A2124)                                                     */
/*       BDX (0x401A2124)                                                     */
/* Register default value:              0x7FFF7FFF                            */
#define CORRERRTHRSHLD_2_MCDDC_DP_REG 0x0C014124
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * This register holds the per rank corrected error thresholding value.
 */
typedef union {
  struct {
    UINT32 cor_err_th_4 : 15;
    /* cor_err_th_4 - Bits[14:0], RW_LB, default = 15'b111111111111111 
       The corrected error threshold for this rank that will be compared to the per 
       rank corrected error counter. 
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 cor_err_th_5 : 15;
    /* cor_err_th_5 - Bits[30:16], RW_LB, default = 15'b111111111111111 
       The corrected error threshold for this rank that will be compared to the per 
       rank corrected error counter. 
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CORRERRTHRSHLD_2_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CORRERRTHRSHLD_3_MCDDC_DP_REG supported on:                                */
/*       IVT_EP (0x40186128)                                                  */
/*       IVT_EX (0x40186128)                                                  */
/*       HSX (0x401A2128)                                                     */
/*       BDX (0x401A2128)                                                     */
/* Register default value:              0x7FFF7FFF                            */
#define CORRERRTHRSHLD_3_MCDDC_DP_REG 0x0C014128
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * This register holds the per rank corrected error thresholding value.
 */
typedef union {
  struct {
    UINT32 cor_err_th_6 : 15;
    /* cor_err_th_6 - Bits[14:0], RW_LB, default = 15'b111111111111111 
       The corrected error threshold for this rank that will be compared to the per 
       rank corrected error counter. 
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 cor_err_th_7 : 15;
    /* cor_err_th_7 - Bits[30:16], RW_LB, default = 15'b111111111111111 
       The corrected error threshold for this rank that will be compared to the per 
       rank corrected error counter. 
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CORRERRTHRSHLD_3_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CORRERRORSTATUS_MCDDC_DP_REG supported on:                                 */
/*       IVT_EP (0x40186134)                                                  */
/*       IVT_EX (0x40186134)                                                  */
/*       HSX (0x401A2134)                                                     */
/*       BDX (0x401A2134)                                                     */
/* Register default value on IVT_EP:    0x00000000                            */
/* Register default value on IVT_EX:    0x00000000                            */
/* Register default value on HSX:       0x00043000                            */
/* Register default value on BDX:       0x00043000                            */
#define CORRERRORSTATUS_MCDDC_DP_REG 0x0C014134


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.2.CFG.xml.
 * Per rank corrected error status. These bits are reset by bios.
 */
typedef union {
  struct {
    UINT32 err_overflow_stat : 8;
    /* err_overflow_stat - Bits[7:0], RW1C, default = 8'b00000000 
       This 8 bit field is the per rank error over-threshold status bits. The 
       organization is as follows: 
       Bit 0 : Rank 0
       Bit 1 : Rank 1
       Bit 2 : Rank 2
       Bit 3 : Rank 3
       Bit 4 : Rank 4
       Bit 5 : Rank 5
       Bit 6 : Rank 6
       Bit 7 : Rank 7
       
       Note: The register tracks which rank has reached or exceeded the corresponding 
       CORRERRTHRSHLD threshold settings. 
     */
    UINT32 dimm_alert : 3;
    /* dimm_alert - Bits[10:8], RW_V, default = 3'b000 
       This field holds which DIMM has seen an alert after the last alert was detected. 
       Subsequent alerts will 
       update this field with the new failing DIMM. The organization is as follows:
       Bit 0 : DIMM 0
       Bit 1 : DIMM 1
       Bit 2 : DIMM 2
     */
    UINT32 dimm_alert_select : 3;
    /* dimm_alert_select - Bits[13:11], RW, default = 3'b110 
       This field is used to select which of the 8-bits returned has the alert bit from 
       the RDIMM. This is usually set to 6. 
     */
    UINT32 dimm_alert2 : 3;
    /* dimm_alert2 - Bits[16:14], RW_V, default = 3'b000 
       This field holds which DIMM on the second channel in VMSE 1:1 mode has seen an 
       alert after the last alert 
       was detected. Subsequent alerts will update this field with the new failing 
       DIMM. The organization is as follows: 
       Bit 0 : DIMM 0
       Bit 1 : DIMM 1
       Bit 2 : DIMM 2
     */
    UINT32 dimm_alert_select2 : 3;
    /* dimm_alert_select2 - Bits[19:17], RW, default = 3'b010 
       This field is used to select which of the 8-bits returned has the alert bit from 
       the RDIMM on the second 
       channel. This is used for VMSE 1:1 mode since two channels are returned with a 
       single read. 
       This is usually set to 2.
     */
    UINT32 rsvd : 12;
    /* rsvd - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CORRERRORSTATUS_MCDDC_DP_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Per rank corrected error status. These bits are reset by bios.
 */
typedef union {
  struct {
    UINT32 err_overflow_stat : 8;
    /* err_overflow_stat - Bits[7:0], RW1C, default = 8'b00000000 
       This 8 bit field is the per rank error over-threshold status bits. The 
       organization is as follows: 
       Bit 0 : Rank 0
       Bit 1 : Rank 1
       Bit 2 : Rank 2
       Bit 3 : Rank 3
       Bit 4 : Rank 4
       Bit 5 : Rank 5
       Bit 6 : Rank 6
       Bit 7 : Rank 7
       
       Note: The register tracks which rank has reached or exceeded the corresponding 
       CORRERRTHRSHLD threshold settings. 
     */
    UINT32 dimm_alert : 3;
    /* dimm_alert - Bits[10:8], RW_V, default = 3'b000 
       This field holds which DIMM has seen an alert after the last alert was detected. 
       Subsequent alerts will 
       update this field with the new failing DIMM. The organization is as follows:
       Bit 0 : DIMM 0
       Bit 1 : DIMM 1
       Bit 2 : DIMM 2
     */
    UINT32 dimm_alert_select : 3;
    /* dimm_alert_select - Bits[13:11], RW, default = 3'b110 
       This field is used to select which of the 8-bits returned has the alert bit from 
       the RDIMM. This is usually set to 6. 
     */
    UINT32 dimm_alert2 : 3;
    /* dimm_alert2 - Bits[16:14], RW_V, default = 3'b000 
       This field holds which DIMM on the second channel in VMSE 1:1 mode has seen an 
       alert after the last alert was detected. Subsequent alerts will update this 
       field with the new failing DIMM. The organization is as follows: 
       Bit 0 : DIMM 0
       Bit 1 : DIMM 1
       Bit 2 : DIMM 2
     */
    UINT32 dimm_alert_select2 : 3;
    /* dimm_alert_select2 - Bits[19:17], RW, default = 3'b010 
       This field is used to select which of the 8-bits returned has the alert bit from 
       the RDIMM on the second 
       channel. This is used for VMSE 1:1 mode since two channels are returned with a 
       single read. 
       This is usually set to 2.
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[23:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ddr4crc_rank_log : 8;
    /* ddr4crc_rank_log - Bits[31:24], RW_V, default = 8'b00000000 
       This field get set with 1'b1 , if the corresponding rank detected ddr4 crc in 
       one of its write data. This will be cleared by BIOS 
     */
  } Bits;
  UINT32 Data;
} CORRERRORSTATUS_MCDDC_DP_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* LEAKY_BKT_2ND_CNTR_REG_MCDDC_DP_REG supported on:                          */
/*       IVT_EP (0x40186138)                                                  */
/*       IVT_EX (0x40186138)                                                  */
/*       HSX (0x401A2138)                                                     */
/*       BDX (0x401A2138)                                                     */
/* Register default value:              0x00000000                            */
#define LEAKY_BKT_2ND_CNTR_REG_MCDDC_DP_REG 0x0C014138
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * generated by critter 20_2_0x138
 */
typedef union {
  struct {
    UINT32 leaky_bkt_2nd_cntr : 16;
    /* leaky_bkt_2nd_cntr - Bits[15:0], RW_V, default = 16'b0000000000000000 
       Per rank secondary leaky bucket counter (2b per rank)
       bit 15:14: rank 7 secondary leaky bucket counter
       bit 13:12: rank 6 secondary leaky bucket counter
       bit 11:10: rank 5 secondary leaky bucket counter
       bit 9:8: rank 4 secondary leaky bucket counter
       bit 7:6: rank 3 secondary leaky bucket counter
       bit 5:4: rank 2 secondary leaky bucket counter
       bit 3:2: rank 1 secondary leaky bucket counter
       bit 1:0: rank 0 secondary leaky bucket counter
     */
    UINT32 leaky_bkt_2nd_cntr_limit : 16;
    /* leaky_bkt_2nd_cntr_limit - Bits[31:16], RW, default = 16'b0000000000000000 
       Secondary Leaky Bucket Counter Limit (2b per DIMM). This register defines 
       secondary leaky bucket counter limit for all 8 logical ranks within channel. The 
       counter logic will generate the secondary LEAK pulse to decrement the rank's 
       correctable error counter by 1 when the corresponding rank leaky bucket rank 
       counter roll over at the predefined counter limit. The counter increment at the 
       primary leak pulse from the LEAKY_BUCKET_CNTR_LO and LEAKY_BUCKET_CNTR_HI logic. 
       
       Bit[31:30]: Rank 7 Secondary Leaky Bucket Counter Limit
       Bit[29:28]: Rank 6 Secondary Leaky Bucket Counter Limit
       Bit[27:26]: Rank 5 Secondary Leaky Bucket Counter Limit
       Bit[25:24]: Rank 4 Secondary Leaky Bucket Counter Limit
       Bit[23:22]: Rank 3 Secondary Leaky Bucket Counter Limit
       Bit[21:20]: Rank 2 Secondary Leaky Bucket Counter Limit
       Bit[19:18]: Rank 1 Secondary Leaky Bucket Counter Limit
       Bit[17:16]: Rank 0 Secondary Leaky Bucket Counter Limit
       
       The value of the limit is defined as the following:
       0: the LEAK pulse is generated one DCLK after the primary LEAK pulse is 
       asserted. 
       1: the LEAK pulse is generated one DCLK after the counter roll over at 1.
       2: the LEAK pulse is generated one DCLK after the counter roll over at 2.
       3: the LEAK pulse is generated one DCLK after the counter roll over at 3.
     */
  } Bits;
  UINT32 Data;
} LEAKY_BKT_2ND_CNTR_REG_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* DEVTAG_CNTL_0_MCDDC_DP_REG supported on:                                   */
/*       IVT_EP (0x10186140)                                                  */
/*       IVT_EX (0x10186140)                                                  */
/*       HSX (0x101A2140)                                                     */
/*       BDX (0x101A2140)                                                     */
/* Register default value:              0x3F                                  */
#define DEVTAG_CNTL_0_MCDDC_DP_REG 0x0C011140
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * SDDC Usage model -
 * When the number of correctable errors (CORRERRCNT_x) from a particular rank 
 * exceeds the corresponding threshold (CORRERRTHRSHLD_y), hardware will generate a 
 * SMI interrupt preserve the failing device in the FailDevice field. 
 * SMM software will read the failing device on the particular rank. Software then 
 * set the EN bit to enable substituion of the failing device/rank with the parity 
 * from the rest of the devices inline. 
 * For independent channel configuration, each rank can tag once. Up to 8 ranks can 
 * be tagged. 
 * For lock-step channel configuration, only one x8 device can be tagged per 
 * rank-pair. SMM software must copy the faildevice log from Channel 0/2 to the 
 * corresponding register in Channel 1/3, then set EN for both channels in 
 * lockstepped pair. 
 * There is no hardware logic to report incorrect programming error. Unpredicable 
 * error and/or silent data corruption will be the consequence of such programming 
 * error. 
 * If the rank-sparing is enabled, it is recommend to prioritize the rank-sparing 
 * before triggering the device tagging due to the nature of the device tagging 
 * would drop the correction capability and any subsequent ECC error from this rank 
 * would cause uncorrectable error. 
 */
typedef union {
  struct {
    UINT8 faildevice : 6;
    /* faildevice - Bits[5:0], RWS_V, default = 6'b111111 
       Hardware will capture the fail device ID of the rank in the FailDevice field 
       upon successfull correction from the device correction engine. After SDDC is 
       enabled HW may not update this field. 
       In DDR3 lockstep mode, the faildevice will only be logged by hardware on Channel 
       0/2 of lockstepped pair. SMM must copy faildevice to the other lockstepped 
       channel before enabling SDDC. 
       
       Native DDR/Intel SMI 2 2:1/Intel SMI 2 1:1x8:
       Valid Range is decimal 0-17 to indicate which x4 device (independent channel) or 
       x8 device (lock-step mode) has failed. 
       
       Intel SMI 2 1:1x4(DDDC):
       Valid Range is decimal 0-35 to indicate which x4 device has failed.
       Note that when DDDC has been enabled on the non-spare device, and a subsequent 
       failure of the spare device occurs, the value logged here will be equal to the 
       DDDC faildevice. 
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[6:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT8 en : 1;
    /* en - Bits[7:7], RWS_L, default = 1'b0 
       Device tagging (SDDC) enable for this rank. Once set, the parity device of the 
       rank is used for the replacement device content. After tagging, the rank will no 
       longer have the "correction" capability. ECC error "detection" capability will 
       not degrade after setting this bit. 
       Warning: For DDR3 lock-step channel configuration, only one x8 device can be 
       tagged per rank-pair. SMM software must copy the faildevice log from Channel 0/2 
       to the corresponding register in Channel 1/3, then set EN for both channels in 
       lockstepped pair. 
       DDDC: (EX processor only)
       On DDDC supported systems, BIOS has the option to enable SDDC in conjuntion with 
       DDDC_CNTL:SPARING to enable faster sparing with SDDC substitution. This field is 
       cleared by HW on completion of DDDC sparing. 
     */
  } Bits;
  UINT8 Data;
} DEVTAG_CNTL_0_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* DEVTAG_CNTL_1_MCDDC_DP_REG supported on:                                   */
/*       IVT_EP (0x10186141)                                                  */
/*       IVT_EX (0x10186141)                                                  */
/*       HSX (0x101A2141)                                                     */
/*       BDX (0x101A2141)                                                     */
/* Register default value:              0x3F                                  */
#define DEVTAG_CNTL_1_MCDDC_DP_REG 0x0C011141
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * See DEVTAG_CNTL_0 for description.
 */
typedef union {
  struct {
    UINT8 faildevice : 6;
    /* faildevice - Bits[5:0], RWS_V, default = 6'b111111 
       See DEVTAG_CNTL_0 for description.
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[6:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT8 en : 1;
    /* en - Bits[7:7], RWS_L, default = 1'b0 
       See DEVTAG_CNTL_0 for description.
     */
  } Bits;
  UINT8 Data;
} DEVTAG_CNTL_1_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* DEVTAG_CNTL_2_MCDDC_DP_REG supported on:                                   */
/*       IVT_EP (0x10186142)                                                  */
/*       IVT_EX (0x10186142)                                                  */
/*       HSX (0x101A2142)                                                     */
/*       BDX (0x101A2142)                                                     */
/* Register default value:              0x3F                                  */
#define DEVTAG_CNTL_2_MCDDC_DP_REG 0x0C011142
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * See DEVTAG_CNTL_0 for description.
 */
typedef union {
  struct {
    UINT8 faildevice : 6;
    /* faildevice - Bits[5:0], RWS_V, default = 6'b111111 
       See DEVTAG_CNTL_0 for description.
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[6:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT8 en : 1;
    /* en - Bits[7:7], RWS_L, default = 1'b0 
       See DEVTAG_CNTL_0 for description.
     */
  } Bits;
  UINT8 Data;
} DEVTAG_CNTL_2_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* DEVTAG_CNTL_3_MCDDC_DP_REG supported on:                                   */
/*       IVT_EP (0x10186143)                                                  */
/*       IVT_EX (0x10186143)                                                  */
/*       HSX (0x101A2143)                                                     */
/*       BDX (0x101A2143)                                                     */
/* Register default value:              0x3F                                  */
#define DEVTAG_CNTL_3_MCDDC_DP_REG 0x0C011143
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * See DEVTAG_CNTL_0 for description.
 */
typedef union {
  struct {
    UINT8 faildevice : 6;
    /* faildevice - Bits[5:0], RWS_V, default = 6'b111111 
       See DEVTAG_CNTL_0 for description.
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[6:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT8 en : 1;
    /* en - Bits[7:7], RWS_L, default = 1'b0 
       See DEVTAG_CNTL_0 for description.
     */
  } Bits;
  UINT8 Data;
} DEVTAG_CNTL_3_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* DEVTAG_CNTL_4_MCDDC_DP_REG supported on:                                   */
/*       IVT_EP (0x10186144)                                                  */
/*       IVT_EX (0x10186144)                                                  */
/*       HSX (0x101A2144)                                                     */
/*       BDX (0x101A2144)                                                     */
/* Register default value:              0x3F                                  */
#define DEVTAG_CNTL_4_MCDDC_DP_REG 0x0C011144
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * See DEVTAG_CNTL_0 for description.
 */
typedef union {
  struct {
    UINT8 faildevice : 6;
    /* faildevice - Bits[5:0], RWS_V, default = 6'b111111 
       See DEVTAG_CNTL_0 for description.
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[6:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT8 en : 1;
    /* en - Bits[7:7], RWS_L, default = 1'b0 
       See DEVTAG_CNTL_0 for description.
     */
  } Bits;
  UINT8 Data;
} DEVTAG_CNTL_4_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* DEVTAG_CNTL_5_MCDDC_DP_REG supported on:                                   */
/*       IVT_EP (0x10186145)                                                  */
/*       IVT_EX (0x10186145)                                                  */
/*       HSX (0x101A2145)                                                     */
/*       BDX (0x101A2145)                                                     */
/* Register default value:              0x3F                                  */
#define DEVTAG_CNTL_5_MCDDC_DP_REG 0x0C011145
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * See DEVTAG_CNTL_0 for description.
 */
typedef union {
  struct {
    UINT8 faildevice : 6;
    /* faildevice - Bits[5:0], RWS_V, default = 6'b111111 
       See DEVTAG_CNTL_0 for description.
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[6:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT8 en : 1;
    /* en - Bits[7:7], RWS_L, default = 1'b0 
       See DEVTAG_CNTL_0 for description.
     */
  } Bits;
  UINT8 Data;
} DEVTAG_CNTL_5_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* DEVTAG_CNTL_6_MCDDC_DP_REG supported on:                                   */
/*       IVT_EP (0x10186146)                                                  */
/*       IVT_EX (0x10186146)                                                  */
/*       HSX (0x101A2146)                                                     */
/*       BDX (0x101A2146)                                                     */
/* Register default value:              0x3F                                  */
#define DEVTAG_CNTL_6_MCDDC_DP_REG 0x0C011146
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * See DEVTAG_CNTL_0 for description.
 */
typedef union {
  struct {
    UINT8 faildevice : 6;
    /* faildevice - Bits[5:0], RWS_V, default = 6'b111111 
       See DEVTAG_CNTL_0 for description.
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[6:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT8 en : 1;
    /* en - Bits[7:7], RWS_L, default = 1'b0 
       See DEVTAG_CNTL_0 for description.
     */
  } Bits;
  UINT8 Data;
} DEVTAG_CNTL_6_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* DEVTAG_CNTL_7_MCDDC_DP_REG supported on:                                   */
/*       IVT_EP (0x10186147)                                                  */
/*       IVT_EX (0x10186147)                                                  */
/*       HSX (0x101A2147)                                                     */
/*       BDX (0x101A2147)                                                     */
/* Register default value:              0x3F                                  */
#define DEVTAG_CNTL_7_MCDDC_DP_REG 0x0C011147
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * See DEVTAG_CNTL_0 for description.
 */
typedef union {
  struct {
    UINT8 faildevice : 6;
    /* faildevice - Bits[5:0], RWS_V, default = 6'b111111 
       See DEVTAG_CNTL_0 for description.
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[6:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT8 en : 1;
    /* en - Bits[7:7], RWS_L, default = 1'b0 
       See DEVTAG_CNTL_0 for description.
     */
  } Bits;
  UINT8 Data;
} DEVTAG_CNTL_7_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* RETRY_RD_ERR_LOG_EXT_MCDDC_DP_REG supported on:                            */
/*       IVT_EP (0x40186150)                                                  */
/*       IVT_EX (0x40186150)                                                  */
/*       HSX (0x401A2150)                                                     */
/*       BDX (0x401A2150)                                                     */
/* Register default value:              0x00000000                            */
#define RETRY_RD_ERR_LOG_EXT_MCDDC_DP_REG 0x0C014150
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Logs info on retried reads.  The contents of this register are valid with 
 * RETRY_RD_ERR_LOG 
 */
typedef union {
  struct {
    UINT32 cw0 : 16;
    /* cw0 - Bits[15:0], RWS_V, default = 16'b0000000000000000 
       Not valid when UC is set.
       When PLUS1 is 0, this register holds the xor correction mask used by retry state 
       machine 
       When PLUS1 is 1, this register holds the encoding of the bit position as 
       follows: 
       [15:9]: 7'b0
       [8:7]: chunk index
       [6:0]: chunk offset
     */
    UINT32 cw1 : 16;
    /* cw1 - Bits[31:16], RWS_V, default = 16'b0000000000000000 
       Not valid when UC is set.
       When PLUS1 is 0, this register holds the xor correction mask used by retry state 
       machine 
       When PLUS1 is 1, this register holds the encoding of the bit position as 
       follows: 
       [15:9]: 7'b0
       [8:7]: chunk index
       [6:0]: chunk offset
     */
  } Bits;
  UINT32 Data;
} RETRY_RD_ERR_LOG_EXT_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* RETRY_RD_ERR_LOG_MCDDC_DP_REG supported on:                                */
/*       IVT_EP (0x40186154)                                                  */
/*       IVT_EX (0x40186154)                                                  */
/*       HSX (0x401A2154)                                                     */
/*       BDX (0x401A2154)                                                     */
/* Register default value:              0x80000000                            */
#define RETRY_RD_ERR_LOG_MCDDC_DP_REG 0x0C014154
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Logs debug info for retried reads.  HA may lock this register when it determines 
 * an uncorrectable error has occured. EN,NOOVER,and EN_PATSPR are control bits, 
 * the rest are logs. Extra info is also logged in RETRY_RD_ERR_LOG_EXT. 
 */
typedef union {
  struct {
    UINT32 v : 1;
    /* v - Bits[0:0], RWS_V, default = 1'b0 
       The contents of this register have valid data. This is also the valid bit for 
       RETRY_RD_ERR_LOG_EXT. 
     */
    UINT32 uc : 1;
    /* uc - Bits[1:1], RWS_V, default = 1'b0 
       Uncorrectable error. Logs cannot be overwritten while UC is set. This bit is not 
       controlled by MC, but by a lock indication from HA coming from the serial 
       interface. 
     */
    UINT32 over : 1;
    /* over - Bits[2:2], RWS_V, default = 1'b0 
       The log has been overwritten at least once.
     */
    UINT32 cw0corr : 1;
    /* cw0corr - Bits[3:3], RWS_V, default = 1'b0 
       During the last log, cw0 retured no error indication to HA. The error was 
       corrected or no error was present before entering correction. 
     */
    UINT32 cw1corr : 1;
    /* cw1corr - Bits[4:4], RWS_V, default = 1'b0 
       During the last log, cw1 returned no error indication to HA. The error was 
       corrected or no error was present before entering correction. 
     */
    UINT32 cw0noerr : 1;
    /* cw0noerr - Bits[5:5], RWS_V, default = 1'b0 
       During the last log, cw0 entered correction with no CRC error
     */
    UINT32 cw1noerr : 1;
    /* cw1noerr - Bits[6:6], RWS_V, default = 1'b0 
       During the last log, cw1 entered correction with no CRC error
     */
    UINT32 plus1 : 1;
    /* plus1 - Bits[7:7], RWS_V, default = 1'b0 
       When this bit is set to 1, log is from plus1 correction. When this bit is set to 
       0, log is from device correction. 
     */
    UINT32 dimm : 3;
    /* dimm - Bits[10:8], RWS_V, default = 3'b000 
       DIMM ID of retried read
     */
    UINT32 rank : 3;
    /* rank - Bits[13:11], RWS_V, default = 3'b000 
       RANK of retried read
     */
    UINT32 cw0dev : 6;
    /* cw0dev - Bits[19:14], RWS_V, default = 6'b000000 
       DEVICE ID of retried read, not valid if UC or PLUS1 bit is set
     */
    UINT32 cw1dev : 6;
    /* cw1dev - Bits[25:20], RWS_V, default = 6'b000000 
       DEVICE ID of retried read, not valid if UC or PLUS1 bit is set
     */
    UINT32 patspr : 1;
    /* patspr - Bits[26:26], RWS_V, default = 1'b0 
       When en_patspr is set, this bit will indicate that the log is from a 
       patrol/spare generated retry. When clear, this bit indicates the retry was HA 
       generated. 
     */
    UINT32 rsvd_27 : 1;
    /* rsvd_27 - Bits[27:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 en_patspr : 1;
    /* en_patspr - Bits[28:28], RWS, default = 1'b0 
       Default operation is to log only HA generated retries. Setting this bit will 
       enable logging of patrol and sparing retries. 
       Note: Use of this bit may cause a patrol or spare log to be locked by HA lock 
       signal. 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 noover : 1;
    /* noover - Bits[30:30], RWS, default = 1'b0 
       Lock register after the first error, do not overflow.
     */
    UINT32 en : 1;
    /* en - Bits[31:31], RWS, default = 1'b1 
       Enable error logging.
       Note:
       When a retry happens via device correction, this register will only log 
       corrected errors and uncorrectable errors. A transient which causes a retry, but 
       data is clean on the retry, will not be logged. 
       When a retry happens via plus1 correction, this register will log on any retry, 
       even if the data is clean before correction. 
     */
  } Bits;
  UINT32 Data;
} RETRY_RD_ERR_LOG_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_MCA_CTL_MCDDC_DP_REG supported on:                                    */
/*       IVT_EP (0x40186158)                                                  */
/*       IVT_EX (0x40186158)                                                  */
/*       HSX (0x401A2158)                                                     */
/*       BDX (0x401A2158)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_MCA_CTL_MCDDC_DP_REG 0x0C014158
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * MC bank and signalling control for Intel SMI 2 errors. (EX processor only)
 * err0: Intel SMI 2 Link Failure
 * err1: Intel SMI 2 Err Observed
 * err2: Intel SMI 2 DIMM Register Parity Error
 * err3: Nb Persistent Counter Reached
 * err4: Sb Persistent Counter Reached
 * 
 * Note: There are 4 instances of this register per MC, however only 2 VMSE links.  
 * Only instances for channel 0/2 are functional. 
 */
typedef union {
  struct {
    UINT32 err0_en : 1;
    /* err0_en - Bits[0:0], RW_LB, default = 1'b0 
       VMSE Link failure, Enable CSR logging. If set, the error will be logged in the 
       corresponding log field in this register. 
     */
    UINT32 err0_log : 1;
    /* err0_log - Bits[1:1], RW1CS, default = 1'b0 
       Error was logged by HW. BIOS clears by writing 0.
     */
    UINT32 err0_cmci : 1;
    /* err0_cmci - Bits[2:2], RW_LB, default = 1'b0 
       Enable/Disable CMCI signaling. When disabled, these errors will not be included 
       in the Corrected Error Count. Note: for corrected errors in general, CMCI is 
       only signaled when corr_err_cnt==corr_err_threshold. 
     */
    UINT32 err0_smi : 1;
    /* err0_smi - Bits[3:3], RW_LB, default = 1'b0 
       Enable/Disable SMI signaling each time error occurs.
     */
    UINT32 err1_en : 1;
    /* err1_en - Bits[4:4], RW_LB, default = 1'b0 
       VMSE Err Observed, Enable CSR logging. If set, the error will be logged in the 
       corresponding log field in this register. 
     */
    UINT32 err1_log : 1;
    /* err1_log - Bits[5:5], RW1CS, default = 1'b0 
       Error was logged by HW. BIOS clears by writing 0.
     */
    UINT32 err1_cmci : 1;
    /* err1_cmci - Bits[6:6], RW_LB, default = 1'b0 
       Enable/Disable CMCI signaling. When disabled, these errors will not be included 
       in the Corrected Error Count. Note: for corrected errors in general, CMCI is 
       only signaled when corr_err_cnt==corr_err_threshold. 
     */
    UINT32 err1_smi : 1;
    /* err1_smi - Bits[7:7], RW_LB, default = 1'b0 
       Enable/Disable SMI signaling each time error occurs.
     */
    UINT32 err2_en : 1;
    /* err2_en - Bits[8:8], RW_LB, default = 1'b0 
       DIMM Register Parity, Enable CSR logging. If set, the error will be logged in 
       the corresponding log field in this register. 
     */
    UINT32 err2_log : 1;
    /* err2_log - Bits[9:9], RW1CS, default = 1'b0 
       Error was logged by HW. BIOS clears by writing 0.
     */
    UINT32 err2_cmci : 1;
    /* err2_cmci - Bits[10:10], RW_LB, default = 1'b0 
       Enable/Disable CMCI signaling. When disabled, these errors will not be included 
       in the Corrected Error Count. Note: for corrected errors in general, CMCI is 
       only signaled when corr_err_cnt==corr_err_threshold. 
     */
    UINT32 err2_smi : 1;
    /* err2_smi - Bits[11:11], RW_LB, default = 1'b0 
       Enable/Disable SMI signaling each time error occurs.
     */
    UINT32 err3_en : 1;
    /* err3_en - Bits[12:12], RW_LB, default = 1'b0 
       SB Persistent, Enable CSR logging. If set, the error will be logged in the 
       corresponding log field in this register. 
     */
    UINT32 err3_log : 1;
    /* err3_log - Bits[13:13], RW1CS, default = 1'b0 
       Error was logged by HW. BIOS clears by writing 0.
     */
    UINT32 err3_cmci : 1;
    /* err3_cmci - Bits[14:14], RW_LB, default = 1'b0 
       Enable/Disable CMCI signaling. When disabled, these errors will not be included 
       in the Corrected Error Count. Note: for corrected errors in general, CMCI is 
       only signaled when corr_err_cnt==corr_err_threshold. 
     */
    UINT32 err3_smi : 1;
    /* err3_smi - Bits[15:15], RW_LB, default = 1'b0 
       Enable/Disable SMI signaling each time error occurs.
     */
    UINT32 err4_en : 1;
    /* err4_en - Bits[16:16], RW_LB, default = 1'b0 
       NB Persistent, Enable CSR logging. If set, the error will be logged in the 
       corresponding log field in this register. 
     */
    UINT32 err4_log : 1;
    /* err4_log - Bits[17:17], RW1CS, default = 1'b0 
       Error was logged by HW. BIOS clears by writing 0.
     */
    UINT32 err4_cmci : 1;
    /* err4_cmci - Bits[18:18], RW_LB, default = 1'b0 
       Enable/Disable CMCI signaling. When disabled, these errors will not be included 
       in the Corrected Error Count. Note: for corrected errors in general, CMCI is 
       only signaled when corr_err_cnt==corr_err_threshold. 
     */
    UINT32 err4_smi : 1;
    /* err4_smi - Bits[19:19], RW_LB, default = 1'b0 
       Enable/Disable SMI signaling each time error occurs.
     */
    UINT32 rsvd : 12;
    /* rsvd - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} VMSE_MCA_CTL_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* RETRY_RD_ERR_LOG_EXT2_MCDDC_DP_REG supported on:                           */
/*       IVT_EP (0x4018615C)                                                  */
/*       IVT_EX (0x4018615C)                                                  */
/*       HSX (0x401A215C)                                                     */
/*       BDX (0x401A215C)                                                     */
/* Register default value:              0x00000000                            */
#define RETRY_RD_ERR_LOG_EXT2_MCDDC_DP_REG 0x0C01415C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.2.CFG.xml.
 * Logs info on retried reads.  The contents of this register are valid with 
 * RETRY_RD_ERR_LOG. 
 * The values in this register may be used to determine the case of multiple valid 
 * corrections. 
 */
typedef union {
  struct {
    UINT32 cw0 : 9;
    /* cw0 - Bits[8:0], RWS_V, default = 9'b000000000 
       Holds number of valid corrections found on last retry for cw0
     */
    UINT32 rsvd_9 : 7;
    /* rsvd_9 - Bits[15:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 cw1 : 9;
    /* cw1 - Bits[24:16], RWS_V, default = 9'b000000000 
       Holds number of valid corrections found on last retry for cw1
     */
    UINT32 rsvd_25 : 3;
    /* rsvd_25 - Bits[27:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bank : 4;
    /* bank - Bits[31:28], RWS_V, default = 4'b0000 
       Bank ID for last retry (used with bank DDDC feature)
     */
  } Bits;
  UINT32 Data;
} RETRY_RD_ERR_LOG_EXT2_MCDDC_DP_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */






/* ALERTSIGNAL_MCDDC_DP_REG supported on:                                     */
/*       HSX (0x101A2164)                                                     */
/*       BDX (0x101A2164)                                                     */
/* Register default value:              0x00                                  */
#define ALERTSIGNAL_MCDDC_DP_REG 0x0C011164

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Alert status
 */
typedef union {
  struct {
    UINT8 seen : 1;
    /* seen - Bits[0:0], RW1C, default = 1'b0 
       If 1, alert signal (DDR4 parity error) has been observed. This bit is intended 
       to be used as feedback during CMD/address training. A write 1 will clear this 
       field. 
     */
    UINT8 rsvd : 7;
    /* rsvd - Bits[7:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} ALERTSIGNAL_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_PDA_MCDDC_DP_REG supported on:                                        */
/*       HSX (0x401A2168)                                                     */
/*       BDX (0x401A2168)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_PDA_MCDDC_DP_REG 0x0C014168

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Register for one Per Dram Addressable transaction
 */
typedef union {
  struct {
    UINT32 device_select : 18;
    /* device_select - Bits[17:0], RW_LB, default = 18'b000000000000000000 
       This register will be driven on DQ lines during Per Dram Addressability Mode 
       Register Set operations according to the following table. A 0 in a bit position 
       will select the corresponding device for Per Dram addressability Transaction. 
       for x8 devices:
       bit [0] will be driven on DQ[7:0]
       bit [1] will be driven on DQ[15:8]
       bit [2] will be driven on DQ[23:16]
       bit [3] will be driven on DQ[31:24]
       bit [4] will be driven on DQ[39:32]
       bit [5] will be driven on DQ[47:40]
       bit [6] will be driven on DQ[55:48]
       bit [7] will be driven on DQ[63:56]
       bit [8] will be driven on DQ[71:64]
       for x4 devices:
       bit [0] will be driven on DQ[3:0]
       bit [1] will be driven on DQ[7:4]
       bit [2] will be driven on DQ[11:8]
       bit [3] will be driven on DQ[15:12]
       bit [4] will be driven on DQ[19:16]
       bit [5] will be driven on DQ[23:20]
       bit [6] will be driven on DQ[27:24]
       bit [7] will be driven on DQ[31:28]
       bit [8] will be driven on DQ[35:32]
       bit [9] will be driven on DQ[39:36]
       bit [10] will be driven on DQ[43:40]
       bit [11] will be driven on DQ[47:44]
       bit [12] will be driven on DQ[51:48]
       bit [13] will be driven on DQ[55:52]
       bit [14] will be driven on DQ[59:56]
       bit [15] will be driven on DQ[63:60]
       bit [16] will be driven on DQ[67:64]
       bit [17] will be driven on DQ[71:68]
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_select : 3;
    /* rank_select - Bits[22:20], RW_LB, default = 3'b000 
       This field is the logical rank select for PDA transaction and should match the 
       same field in CPGC_PDA_SHADOW register and should be set to match Chip Select 
       fields programmed into CADB for this transaction (see Rank Naming Convention in 
       MAS for mapping of Chip selects to logical ranks) 
     */
    UINT32 rsvd_23 : 9;
    /* rsvd_23 - Bits[31:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_PDA_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






/* MR_READ_RESULT_DQ12TO0_MCDDC_DP_REG supported on:                          */
/*       HSX (0x401A2178)                                                     */
/*       BDX (0x401A2178)                                                     */
/* Register default value:              0x00000000                            */
#define MR_READ_RESULT_DQ12TO0_MCDDC_DP_REG 0x0C014178

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Saves information from last Mode Register read
 */
typedef union {
  struct {
    UINT32 dq : 32;
    /* dq - Bits[31:0], RO, default = 32'b00000000000000000000000000000000 
       For by 8 parts: device 0 result: MR_READ_RESULT_DQ12TO0[7:0]
       device 1 result: MR_READ_RESULT_DQ12TO0[23:16]
       For by 4 parts: device 0 result: MR_READ_RESULT_DQ12TO0[7:0]
       device 1 result: MR_READ_RESULT_DQ12TO0[15:8]
       device 2 result: MR_READ_RESULT_DQ12TO0[23:16]
       device 3 result: MR_READ_RESULT_DQ12TO0[31:24]
       note that this register does not have a power up default and reads from it will 
       return garbage if a Mode Register read has not been issued prior to it. 
     */
  } Bits;
  UINT32 Data;
} MR_READ_RESULT_DQ12TO0_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MR_READ_RESULT_DQ28TO16_MCDDC_DP_REG supported on:                         */
/*       HSX (0x401A217C)                                                     */
/*       BDX (0x401A217C)                                                     */
/* Register default value:              0x00000000                            */
#define MR_READ_RESULT_DQ28TO16_MCDDC_DP_REG 0x0C01417C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Saves information from last Mode Register read
 */
typedef union {
  struct {
    UINT32 dq : 32;
    /* dq - Bits[31:0], RO, default = 32'b00000000000000000000000000000000 
       For by 8 parts: device 2 result: MR_READ_RESULT_DQ28TO16[7:0]
       device 3 result: MR_READ_RESULT_DQ28TO16[23:16]
       For by 4 parts: device 4 result: MR_READ_RESULT_DQ28TO16[7:0]
       device 5 result: MR_READ_RESULT_DQ28TO16[15:8]
       device 6 result: MR_READ_RESULT_DQ28TO16[23:16]
       device 7 result: MR_READ_RESULT_DQ28TO16[31:24]
     */
  } Bits;
  UINT32 Data;
} MR_READ_RESULT_DQ28TO16_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MR_READ_RESULT_DQ44TO32_MCDDC_DP_REG supported on:                         */
/*       HSX (0x401A2180)                                                     */
/*       BDX (0x401A2180)                                                     */
/* Register default value:              0x00000000                            */
#define MR_READ_RESULT_DQ44TO32_MCDDC_DP_REG 0x0C014180

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * saves information from last Mode Register read
 */
typedef union {
  struct {
    UINT32 dq : 32;
    /* dq - Bits[31:0], RO, default = 32'b00000000000000000000000000000000 
       For by 8 parts: device 4 result: MR_READ_RESULT_DQ44TO32[7:0]
       device 5 result: MR_READ_RESULT_DQ44TO32[23:16]
       For by 4 parts: device 8 result: MR_READ_RESULT_DQ44TO32[7:0]
       device 9 result: MR_READ_RESULT_DQ44TO32[15:8]
       device 10 result: MR_READ_RESULT_DQ44TO32[23:16]
       device 11 result: MR_READ_RESULT_DQ44TO32[31:24]
     */
  } Bits;
  UINT32 Data;
} MR_READ_RESULT_DQ44TO32_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MR_READ_RESULT_DQ60TO48_MCDDC_DP_REG supported on:                         */
/*       HSX (0x401A2184)                                                     */
/*       BDX (0x401A2184)                                                     */
/* Register default value:              0x00000000                            */
#define MR_READ_RESULT_DQ60TO48_MCDDC_DP_REG 0x0C014184

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * saves information from last Mode Register read
 */
typedef union {
  struct {
    UINT32 dq : 32;
    /* dq - Bits[31:0], RO, default = 32'b00000000000000000000000000000000 
       For by 8 parts: device 6 result: MR_READ_RESULT_DQ60To48[7:0]
       device 7 result: MR_READ_RESULT_DQ60To48[23:16]
       For by 4 parts: device 12 result: MR_READ_RESULT_DQ60To48[7:0]
       device 13 result: MR_READ_RESULT_DQ60To48[15:8]
       device 14 result: MR_READ_RESULT_DQ60To48[23:16]
       device 15 result: MR_READ_RESULT_DQ60To48[31:24]
     */
  } Bits;
  UINT32 Data;
} MR_READ_RESULT_DQ60TO48_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MR_READ_RESULT_DQ68TO64_MCDDC_DP_REG supported on:                         */
/*       HSX (0x401A2188)                                                     */
/*       BDX (0x401A2188)                                                     */
/* Register default value:              0x00000000                            */
#define MR_READ_RESULT_DQ68TO64_MCDDC_DP_REG 0x0C014188

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * saves information from last Mode Register read
 */
typedef union {
  struct {
    UINT32 dq : 16;
    /* dq - Bits[15:0], RO, default = 16'b0000000000000000 
       For by 8 parts: device 8 result: MR_READ_RESULT_DQ68TO64[7:0]
       For by 4 parts: device 16 result: MR_READ_RESULT_DQ68TO64[7:0]
       device 17 result: MR_READ_RESULT_DQ68TO64[15:8]
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MR_READ_RESULT_DQ68TO64_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RETRY_RD_ERR_LOG_EXT3_MCDDC_DP_REG supported on:                           */
/*       HSX (0x401A218C)                                                     */
/*       BDX (0x401A218C)                                                     */
/* Register default value:              0x00000000                            */
#define RETRY_RD_ERR_LOG_EXT3_MCDDC_DP_REG 0x0C01418C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Logs info on retried reads.  The contents of this register are valid with 
 * RETRY_RD_ERR_LOG. 
 * Row and truncated column address
 */
typedef union {
  struct {
    UINT32 col : 12;
    /* col - Bits[11:0], RWS_V, default = 12'b000000000000 
       Truncated Col Address
     */
    UINT32 row : 20;
    /* row - Bits[31:12], RWS_V, default = 20'b00000000000000000000 
       Row Address
     */
  } Bits;
  UINT32 Data;
} RETRY_RD_ERR_LOG_EXT3_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MCSCRAMBLECONFIG_MCDDC_DP_REG supported on:                                */
/*       IVT_EP (0x401861E0)                                                  */
/*       IVT_EX (0x401861E0)                                                  */
/*       HSX (0x401A21E0)                                                     */
/*       BDX (0x401A21E0)                                                     */
/* Register default value:              0x00000000                            */
#define MCSCRAMBLECONFIG_MCDDC_DP_REG 0x0C0141E0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * This register is used to scramble and unscramble the IMC to DDR Pad data using 
 * the DDR command address and the scramble seed. All the fields CH_ENABLE, 
 * TX_ENABLE and RX_ENABLE must be set to 1 to enable scrambling, and must be 
 * cleared to disable scrambling. 
 */
typedef union {
  struct {
    UINT32 rx_enable : 1;
    /* rx_enable - Bits[0:0], RWS_L, default = 1'b0 
       RX_ENABLE: Hooked up to Transmit De-scramble in the design - setting this bit 
       would cause MC Tx data to the DDR pads to be scrambled. 
     */
    UINT32 tx_enable : 1;
    /* tx_enable - Bits[1:1], RWS_L, default = 1'b0 
       TX_ENABLE: Hooked up to Receive De-scramble in the design - setting this bit 
       would cause MC Rx data from the DDR pads to be descrambled. 
     */
    UINT32 ch_enable : 1;
    /* ch_enable - Bits[2:2], RWS_L, default = 1'b0 
       Channel enable
     */
    UINT32 seed_lock : 1;
    /* seed_lock - Bits[3:3], RWS_O, default = 1'b0 
       lock bit for the seed update.
       1b = lock
       0b = unlock
     */
    UINT32 rsvd : 28;
    /* rsvd - Bits[31:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MCSCRAMBLECONFIG_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG supported on:                             */
/*       IVT_EP (0x401861E4)                                                  */
/*       IVT_EX (0x401861E4)                                                  */
/*       HSX (0x401A21E4)                                                     */
/*       BDX (0x401A21E4)                                                     */
/* Register default value:              0x00000000                            */
#define MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG 0x0C0141E4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * This register is locked by SEED_LOCK bit in MCSCRAMBLECONFIG register.
 */
typedef union {
  struct {
    UINT32 scrb_lower_seed_sel : 16;
    /* scrb_lower_seed_sel - Bits[15:0], RWS_L, default = 16'b0000000000000000 
       Reordering the lower srambling seed select control.
     */
    UINT32 scrb_upper_seed_sel : 16;
    /* scrb_upper_seed_sel - Bits[31:16], RWS_L, default = 16'b0000000000000000 
       Reordering the upper srambling seed select control.
     */
  } Bits;
  UINT32 Data;
} MCSCRAMBLE_SEED_SEL_MCDDC_DP_STRUCT;
#endif /* ASM_INC */




/* RSP_FUNC_CRC_ERR_INJ_DEV0_XOR_MSK_MCDDC_DP_REG supported on:               */
/*       IVT_EP (0x40186200)                                                  */
/*       IVT_EX (0x40186200)                                                  */
/*       HSX (0x401A2200)                                                     */
/*       BDX (0x401A2200)                                                     */
/* Register default value:              0x00000000                            */
#define RSP_FUNC_CRC_ERR_INJ_DEV0_XOR_MSK_MCDDC_DP_REG 0x0C014200
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Error Injection Response Function on Address Match Write Data Error Injection. 
 * Associating registers: RSP_FUNC_ADDR_MATCH_LO&HI, RSP_FUNC_ADDR_MATCH_LO&HI, 
 * RSP_FUNC_CRC_ERR_INJ_EXTRA.CRC_ERR_INJ_DEV0_5_BITS and CRC_ERR_INJ_DEV1_5_BITS 
 * 
 * This register is locked by EPMCMAIN_DFX_LCK_CNTL.RSPLCK (uCR).
 */
typedef union {
  struct {
    UINT32 dev0_xor_msk : 32;
    /* dev0_xor_msk - Bits[31:0], RW_L, default = 32'b00000000000000000000000000000000 
       device 0 data inversion mask for error injection. Eight 4-bit values specify 
       which bits of the nibble are inverted on each data cycle of a BL8 write. Bits 
       3:0 correspond to the first data cycle. 
       
       This register is locked by EPMCMAIN_DFX_LCK_CNTL.RSPLCK (uCR).
     */
  } Bits;
  UINT32 Data;
} RSP_FUNC_CRC_ERR_INJ_DEV0_XOR_MSK_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* RSP_FUNC_CRC_ERR_INJ_DEV1_XOR_MSK_MCDDC_DP_REG supported on:               */
/*       IVT_EP (0x40186204)                                                  */
/*       IVT_EX (0x40186204)                                                  */
/*       HSX (0x401A2204)                                                     */
/*       BDX (0x401A2204)                                                     */
/* Register default value:              0x00000000                            */
#define RSP_FUNC_CRC_ERR_INJ_DEV1_XOR_MSK_MCDDC_DP_REG 0x0C014204
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Error Injection Response Function on Address Match Write Data Error Injection. 
 * Associating registers: RSP_FUNC_ADDR_MATCH_LO&HI, RSP_FUNC_ADDR_MATCH_LO&HI, 
 * RSP_FUNC_CRC_ERR_INJ_EXTRA.CRC_ERR_INJ_DEV0_5_BITS and CRC_ERR_INJ_DEV1_5_BITS 
 * 
 * This register is locked by EPMCMAIN_DFX_LCK_CNTL.RSPLCK (uCR).
 */
typedef union {
  struct {
    UINT32 dev1_xor_msk : 32;
    /* dev1_xor_msk - Bits[31:0], RW_L, default = 32'b00000000000000000000000000000000 
       device 1 data inversion mask for error injection. Eight 4-bit values specify 
       which bits of the nibble are inverted on each data cycle of a BL8 write. Bits 
       3:0 correspond to the first data cycle. 
       
       This register is locked by EPMCMAIN_DFX_LCK_CNTL.RSPLCK (uCR).
     */
  } Bits;
  UINT32 Data;
} RSP_FUNC_CRC_ERR_INJ_DEV1_XOR_MSK_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* RSP_FUNC_CRC_ERR_INJ_EXTRA_MCDDC_DP_REG supported on:                      */
/*       IVT_EP (0x40186208)                                                  */
/*       IVT_EX (0x40186208)                                                  */
/*       HSX (0x401A2208)                                                     */
/*       BDX (0x401A2208)                                                     */
/* Register default value:              0x3C000000                            */
#define RSP_FUNC_CRC_ERR_INJ_EXTRA_MCDDC_DP_REG 0x0C014208
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Error Injection Response Function
 * 
 * This register is locked by EPMCMAIN_DFX_LCK_CNTL.RSPLCK (uCR).
 */
typedef union {
  struct {
    UINT32 crc_err_inj_dev0_5_bits : 5;
    /* crc_err_inj_dev0_5_bits - Bits[4:0], RW_L, default = 5'b00000 
       Error Injection Response Function on Address Match Write Data Error Injection. 
       Associating registers: RSP_FUNC_ADDR_MATCH_LO&HI, RSP_FUNC_ADDR_MATCH_LO&HI, 
       RSP_FUNC_CRC_ERR_INJ_DEV0_XOR_MSK and RSP_FUNC_CRC_ERR_INJ_DEV1_XOR_MSK 
       Selects nibble of data bus for error injection. 0x0 selects DQ[3:0], 0x1 selects 
       DQ[7:4], 0x17 selects ECC[7:4] etc... 
       0x18 - 0x31 are reserved.
       
       This register is locked by EPMCMAIN_DFX_LCK_CNTL.RSPLCK (uCR).
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 crc_err_inj_dev1_5_bits : 5;
    /* crc_err_inj_dev1_5_bits - Bits[12:8], RW_L, default = 5'b00000 
       Error Injection Response Function on Address Match Write Data Error Injection. 
       Associating registers: RSP_FUNC_ADDR_MATCH_LO&HI, RSP_FUNC_ADDR_MATCH_LO&HI, 
       RSP_FUNC_CRC_ERR_INJ_DEV0_XOR_MSK and RSP_FUNC_CRC_ERR_INJ_DEV1_XOR_MSK 
       Selects nibble of data bus for device 1 error injection. 0x0 selects DQ[3:0], 
       0x1 selects DQ[7:4], 0x17 selects ECC[7:4] etc... 
       0x18 - 0x31 are reserved.
       
       This register is locked by EPMCMAIN_DFX_LCK_CNTL.RSPLCK (uCR).
     */
    UINT32 rsvd_13 : 3;
    /* rsvd_13 - Bits[15:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_16 : 2;
    UINT32 rsvd_18 : 6;
    /* rsvd_18 - Bits[23:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_24 : 2;
    UINT32 crc_err_inj_chunk_dev0 : 2;
    /* crc_err_inj_chunk_dev0 - Bits[27:26], RW_L, default = 2'b11 
       Error Injection Response Function on Address Match Write Data Error Injection. 
       Associating registers: RSP_FUNC_ADDR_MATCH_LO&HI, RSP_FUNC_ADDR_MATCH_LO&HI, 
       
       These bits control whether the injection is performed on the upper or lower 64 
       chunk per Dclk. This is to allow device targetting for DDDC. 
       11: Default, same injection on both chunks
       01: Inject only on lower chunk
       10: Inject only on upper chunk
       00: Will disable injection
     */
    UINT32 crc_err_inj_chunk_dev1 : 2;
    /* crc_err_inj_chunk_dev1 - Bits[29:28], RW_L, default = 2'b11 
       Error Injection Response Function on Address Match Write Data Error Injection. 
       Associating registers: RSP_FUNC_ADDR_MATCH_LO&HI, RSP_FUNC_ADDR_MATCH_LO&HI, 
       
       These bits control whether the injection is performed on the upper or lower 64 
       chunk per Dclk. This is to allow device targetting for DDDC. 
       11: Default, same injection on both chunks
       01: Inject only on lower chunk
       10: Inject only on upper chunk
       00: Will disable injection
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RSP_FUNC_CRC_ERR_INJ_EXTRA_MCDDC_DP_STRUCT;
#endif /* ASM_INC */














/* X4MODESEL_MCDDC_DP_REG supported on:                                       */
/*       IVT_EP (0x40186268)                                                  */
/*       IVT_EX (0x40186268)                                                  */
/*       HSX (0x401A2268)                                                     */
/*       BDX (0x401A2268)                                                     */
/* Register default value:              0x00000000                            */
#define X4MODESEL_MCDDC_DP_REG 0x0C014268
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * x4ModelSel - MCDP x4 Mode Select
 */
typedef union {
  struct {
    UINT32 dimm0_mode : 1;
    /* dimm0_mode - Bits[0:0], RW, default = 1'b0 
       Controls the DDRIO x4 (if set) / x8 (if cleared) DIMM0 DQS select.
     */
    UINT32 dimm1_mode : 1;
    /* dimm1_mode - Bits[1:1], RW, default = 1'b0 
       Controls the DDRIO x4 (if set) / x8 (if cleared) DIMM1 DQS select.
     */
    UINT32 dimm2_mode : 1;
    /* dimm2_mode - Bits[2:2], RW, default = 1'b0 
       Controls the DDRIO x4 (if set) / x8 (if cleared) DIMM2 DQS select.
     */
    UINT32 rsvd : 29;
    /* rsvd - Bits[31:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} X4MODESEL_MCDDC_DP_STRUCT;
#endif /* ASM_INC */




/* VMSE_TIMING_MCDDC_DP_REG supported on:                                     */
/*       IVT_EP (0x40186300)                                                  */
/*       IVT_EX (0x40186300)                                                  */
/*       HSX (0x401A2300)                                                     */
/*       BDX (0x401A2300)                                                     */
/* Register default value:              0x1870860C                            */
#define VMSE_TIMING_MCDDC_DP_REG 0x0C014300


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.2.CFG.xml.
 * This register holds the timing parameters for Intel SMI 2. (EX processor only)
 */
typedef union {
  struct {
    UINT32 push_to_cas_delay : 7;
    /* push_to_cas_delay - Bits[6:0], RWS_L, default = 7'b0001100 
       The minimum delay (in DCLKs) between a WritePush and Write CAS command
     */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 push_to_data_delay : 5;
    /* push_to_data_delay - Bits[12:8], RWS_L, default = 5'b00110 
       The minimum delay (in DCLKs) between a WritePush command on the Intel SMI 2 
       Cmd/Address bus and the Write data on the Intel SMI 2 Data bus. 
       Expectation is that this is 24ui as observed at scalable memory buffers. In 2:1 
       mode this is a register setting of 6 DCLKs, in 1:1 mode 
       this is a setting of 12 DCLKS. May need to be trained based on I/O or platform 
       delays. 
     */
    UINT32 rsvd_13 : 1;
    /* rsvd_13 - Bits[13:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 vmse_rw_turnaround : 5;
    /* vmse_rw_turnaround - Bits[18:14], RWS_L, default = 5'b00010 
       The minimum delay (in DCLKs) between read data and write data on the Intel SMI 2 
       data bus. 
       In 2:1 mode: Expected 1, conservative 2;
       In 1:1 mode: Expected 2
     */
    UINT32 rsvd_19 : 1;
    /* rsvd_19 - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 vmse_wr_turnaround : 5;
    /* vmse_wr_turnaround - Bits[24:20], RWS_L, default = 5'b00111 
       The minimum delay (in DCLKs) between write data and read data on the Intel SMI 2 
       data bus. 
       In 2:1 mode: Expected 5, conservative 6;
       In 1:1 mode: Expected 7
     */
    UINT32 rcven_ext_en : 1;
    /* rcven_ext_en - Bits[25:25], RWS_L, default = 1'b0 
       Enable receiveEnable pulse extension
     */
    UINT32 scr_to_data_delay : 5;
    /* scr_to_data_delay - Bits[30:26], RWS_L, default = 5'b00110 
       The minimum delay (in DCLKs) between a scrambler sync command on the VMSE 
       Cmd/Address bus and the 0x5555 pattern on the VMSE Data bus. 
       Expectation is that this is 24ui as observed at MXB. In 2:1 mode this is a 
       register setting of 6 DCLKs, in 1:1 mode 
       this is a setting of 12 DCLKS. May need to be trained based on I/O or platform 
       delays. 
     */
    UINT32 mxb_qrtr_rate_en : 1;
    /* mxb_qrtr_rate_en - Bits[31:31], RWS_L, default = 1'b0 
       Enable scalable memory buffers quarter rate core clock mode (for Intel SMI 2 1:1 
       Sub-channel Lockstep mode) 
     */
  } Bits;
  UINT32 Data;
} VMSE_TIMING_MCDDC_DP_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */






/* VMSE_ERROR_MCDDC_DP_REG supported on:                                      */
/*       IVT_EP (0x40186308)                                                  */
/*       IVT_EX (0x40186308)                                                  */
/*       HSX (0x401A2308)                                                     */
/*       BDX (0x401A2308)                                                     */
/* Register default value on IVT_EP:    0x00020020                            */
/* Register default value on IVT_EX:    0x00020020                            */
/* Register default value on HSX:       0x00110020                            */
/* Register default value on BDX:       0x00110020                            */
#define VMSE_ERROR_MCDDC_DP_REG 0x0C014308


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.2.CFG.xml.
 * This register holds the status and configuration of the VMSE_ERR# signal. (EX 
 * processor only) 
 */
typedef union {
  struct {
    UINT32 vmse_err_latency : 7;
    /* vmse_err_latency - Bits[6:0], RWS_L, default = 7'b0100000 
       The maximum delay (in DCLKs) between a command or data with an error and the 
       observed assertion of VMSE_ERR#. 
       This field is also used for DDR4 C/A Parity error latency when the error flow 
       engine is enabled in native mode. 
     */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 vmse_err_state : 1;
    /* vmse_err_state - Bits[8:8], RO_V, default = 1'b0 
       The current state of the VMSE_ERR# signal as seen in the MC. This field follows 
       Active High polarity. 
       A 1 in this field means that VMSE_ERR# pin is active. The logging of this field 
       is qualified by ignore_vmse_err bit. 
       If ignore_vmse_err is set, then this field will not be set even if VMSE_ERR pin 
       asserts. 
       This field is for debug purposes. BIOS should use fields current_vmse_err and 
       observed_vmse_err_assert for 
       training flows.
     */
    UINT32 rsvd_9 : 7;
    /* rsvd_9 - Bits[15:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ignore_vmse_err : 1;
    /* ignore_vmse_err - Bits[16:16], RWS_L, default = 1'b1 
       Set this bit to 1 to mask the VMSE_ERR# in the IMC.
       VMSE_ERROR.vmse_err_state will still report the current state of VMSE_ERR#, but 
       the Intel SMI 2 retry flow will not be 
       initiated when VMSE_ERR# is asserted.
       For all processor types this bit is used for preventing error flow from kicking 
       in when in native ddr4 mode, and setting it to 1 will prevent ddr4 C/A parity 
       starting the error engine. 
     */
    UINT32 rsvd_17 : 1;
    /* rsvd_17 - Bits[17:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_18 : 1;
    UINT32 self_refresh_on_err_off : 1;
    /* self_refresh_on_err_off - Bits[19:19], RW_LB, default = 1'b0 
       Set this bit to 1 to put the error flow machine in a mode where the VMSE chip 
       will not put the DIMMs into self 
       refresh on error. This will make the error flow machine issue the PREALLs to get 
       the DIMMs into a known state. 
     */
    UINT32 self_refresh_off_cke_wait : 2;
    /* self_refresh_off_cke_wait - Bits[21:20], RW_LB, default = 2'b01 
       This field is only valide when self_refresh_on_err_off is set to 1. These two 
       bits set the delay from CKE on to 
       when it is safe to send a new command. The delay settings are as follows:
       0: 256 D clocks
       1: 512 D clocks (default self refresh exit time)
       2: 1024 D clocks
       3: 2048 D clocks
     */
    UINT32 rsvd_22 : 2;
    /* rsvd_22 - Bits[23:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 observed_vmse_err_assert : 1;
    /* observed_vmse_err_assert - Bits[24:24], RW1C, default = 1'b0 
       Hardware will set this bit when it observes that VMSE_ERR# has been active.
       The logging in this field is not affected by ignore_vmse_err bit.
       Once set, hardware will keep the bit set even if VMSE_ERR# is not active until 
       the bit is cleared by software. 
       It is the responsibility of software to clear this bit.
       If the error condition is still in effect, then hardware will re-assert the bit 
       after software clears it. 
     */
    UINT32 current_vmse_err : 1;
    /* current_vmse_err - Bits[25:25], RW1C, default = 1'b0 
       This bit reflects the current status of VMSE Error as seen in the memory 
       controller. 
       This field uses an Active High polarity. The logging in this field is not 
       affected by ignore_vmse_err 
       bit.
       If the error condition is still in effect, then hardware will re-assert the bit 
       after software clears it. 
     */
    UINT32 rsvd_26 : 1;
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} VMSE_ERROR_MCDDC_DP_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* VMSE_ERROR_STATUS_MCDDC_DP_REG supported on:                               */
/*       IVT_EP (0x4018630C)                                                  */
/*       IVT_EX (0x4018630C)                                                  */
/*       HSX (0x401A230C)                                                     */
/*       BDX (0x401A230C)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_ERROR_STATUS_MCDDC_DP_REG 0x0C01430C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * This register holds the most recent status response from a Channel Restart 
 * command. (EX processor only) 
 */
typedef union {
  struct {
    UINT32 restart_status : 32;
    /* restart_status - Bits[31:0], RWS_V, default = 32'b00000000000000000000000000000000 
       The most recent status response from a Channel Restart command indicating the 
       source of the VMSE_ERR# assertion. 
       0: CMD CRC Error
       1: Write Push ECC Error
       2: DIMM Register Parity Error on Bus A
       3: DIMM Register Parity Error on Bus B
       4: Software Error (error bit of the power management Intel SMI 2 command)
       5: Catastrophic Error
     */
  } Bits;
  UINT32 Data;
} VMSE_ERROR_STATUS_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_ERROR_COUNTS_0_MCDDC_DP_REG supported on:                             */
/*       IVT_EP (0x40186310)                                                  */
/*       IVT_EX (0x40186310)                                                  */
/*       HSX (0x401A2310)                                                     */
/*       BDX (0x401A2310)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_ERROR_COUNTS_0_MCDDC_DP_REG 0x0C014310
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * This register holds the counts of status response types from Channel Restart 
 * commands. (EX processor only) 
 */
typedef union {
  struct {
    UINT32 cmd_crc_count : 16;
    /* cmd_crc_count - Bits[15:0], RWS_V, default = 16'b0000000000000000 
       Count of the number of observed VMSE_ERR# assertions where the error type was a 
       Cmd/Addr CRC error. 
       Counter saturates at 0xffff.
     */
    UINT32 wr_data_ecc_count : 16;
    /* wr_data_ecc_count - Bits[31:16], RWS_V, default = 16'b0000000000000000 
       Count of the number of observed VMSE_ERR# assertions where the error type was a 
       write data ECC error. 
       Counter saturates at 0xffff.
     */
  } Bits;
  UINT32 Data;
} VMSE_ERROR_COUNTS_0_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_ERROR_COUNTS_1_MCDDC_DP_REG supported on:                             */
/*       IVT_EP (0x40186314)                                                  */
/*       IVT_EX (0x40186314)                                                  */
/*       HSX (0x401A2314)                                                     */
/*       BDX (0x401A2314)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_ERROR_COUNTS_1_MCDDC_DP_REG 0x0C014314
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * This register holds the counts of status response types from a Channel Restart 
 * commands. (EX processor only) 
 */
typedef union {
  struct {
    UINT32 forced_err_count : 16;
    /* forced_err_count - Bits[15:0], RWS_V, default = 16'b0000000000000000 
       Count of the number of observed VMSE_ERR# assertions where the error was forced 
       by the IMC. 
       Counter saturates at 0xffff.
     */
    UINT32 register_parity_a : 1;
    /* register_parity_a - Bits[16:16], RWS_V, default = 1'b0 
       Count of the number of observed VMSE_ERR# assertions where the error type was a 
       DIMM register parity error to bus A 
       Counter saturates at 0x1.
     */
    UINT32 register_parity_b : 1;
    /* register_parity_b - Bits[17:17], RWS_V, default = 1'b0 
       Count of the number of observed VMSE_ERR# assertions where the error type was a 
       DIMM register parity error to bus B 
       Counter saturates at 0x1.
     */
    UINT32 rsvd : 14;
    /* rsvd - Bits[31:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} VMSE_ERROR_COUNTS_1_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_CFG_READ_0_MCDDC_DP_REG supported on:                                 */
/*       IVT_EP (0x40186320)                                                  */
/*       IVT_EX (0x40186320)                                                  */
/*       HSX (0x401A2320)                                                     */
/*       BDX (0x401A2320)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_CFG_READ_0_MCDDC_DP_REG 0x0C014320
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * This register holds the read data from the most recent Intel SMI 2 config read. 
 * (EX processor only) 
 */
typedef union {
  struct {
    UINT32 read_data : 32;
    /* read_data - Bits[31:0], RW_LBV, default = 32'b00000000000000000000000000000000 
       Read data from the most recent Intel SMI 2 config read.
     */
  } Bits;
  UINT32 Data;
} VMSE_CFG_READ_0_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_CFG_READ_1_MCDDC_DP_REG supported on:                                 */
/*       IVT_EP (0x40186324)                                                  */
/*       IVT_EX (0x40186324)                                                  */
/*       HSX (0x401A2324)                                                     */
/*       BDX (0x401A2324)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_CFG_READ_1_MCDDC_DP_REG 0x0C014324
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * (EX processor only)
 */
typedef union {
  struct {
    UINT32 read_data : 32;
    /* read_data - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Bit 0: Set to 1 only when running 2N mode under Intel SMI 2. This bit should not 
       be set for native mode or when MC is running Intel SMI 2 in 1N or 3N mode. 
       Bit 1: Set to 1 only when running 3N mode under Intel SMI 2. This bit should not 
       be set for native mode or when MC is running Intel SMI 2 in 1N or 2N mode. 
       Bit 2: If this bit is set to 1 then the MC will issue a Training Reset to DDRIO 
       after the VMSE Error FSM issues a channel reset. This bit should be set to 1 
       when VMSE is runnning in 2N or 3N mode. 
       Bit 5: Set to 1 only when running 2N mode under VMSE. This bit should not be set 
       for native mode or when MC is running VMSE in 1N or 3N mode. This bit enables 
       the B0 bug fixes for scrambling and channel restart under 2N. 
       Bit 6: Set to 1 only when running 3N mode under VMSE. This bit should not be set 
       for native mode or when MC is running VMSE in 1N or 2N mode. This bit enables 
       the B0 bug fixes for scrambling and channel restart under 3N. 
       Bit 30: cpgc_novmse1to1swizzle. If zero, data in vmse1to1 will be swizzled. If 
       1, data in 
       vmse1to1 mode will not be swizzled. This should be set to 1 only for cpgc test 
       cases which 
       require tight control on data patterns for victim/aggressor margining cases.
       Bit 31: chkn_ignore_spr_fsm : set to 1 when we are ready to start spare copy, on 
       the vmse channel where spare copy 
       is not on-going (i.e. on the channel that link_fail is not set to 1; this is the 
       channel that spare copy is going on) 
       After spare copy is done, BIOS needs to clear the bit back to 0.
       All other bits are reserved and should be left at 0.
     */
  } Bits;
  UINT32 Data;
} VMSE_CFG_READ_1_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_CFG_WRITE_MCDDC_DP_REG supported on:                                  */
/*       IVT_EP (0x40186328)                                                  */
/*       IVT_EX (0x40186328)                                                  */
/*       HSX (0x401A2328)                                                     */
/*       BDX (0x401A2328)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_CFG_WRITE_MCDDC_DP_REG 0x0C014328
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * This register holds the write data payload for the next Intel SMI 2 config 
 * write. (EX processor only) 
 */
typedef union {
  struct {
    UINT32 write_data : 32;
    /* write_data - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Write data payload for the next Intel SMI 2 config write.
     */
  } Bits;
  UINT32 Data;
} VMSE_CFG_WRITE_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_CFG_MCDDC_DP_REG supported on:                                        */
/*       IVT_EP (0x4018632C)                                                  */
/*       IVT_EX (0x4018632C)                                                  */
/*       HSX (0x401A232C)                                                     */
/*       BDX (0x401A232C)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_CFG_MCDDC_DP_REG 0x0C01432C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * This register ceontrols Intel SMI 2 config accesses sent to the scalable memory 
 * buffers. (EX processor only) 
 */
typedef union {
  struct {
    UINT32 command : 28;
    /* command - Bits[27:0], RW_LB, default = 28'b0000000000000000000000000000 
       Intel SMI 2 Command to transmit. Does not include CRC bits and Write push bits.
       See command encoding in the Intel SMI 2 protocol spec for specific commands.
       
       Recommended Intel SMI 2 Config Read flow:
       1. Write VMSE_Cfg.command & VMSE_Cfg.read_execute & VMSE_Cfg.cfg_cmd_status=0
       2. Read VMSE_Cfg until VMSE_Cfg.read_execute is 0.
       VMSE_Cfg.cfg_cmd_status from this same register is used to check integrity of 
       read data. 
       3. If VMSE_Cfg.cfg_cmd_status == 1, BIOS should either re-issue the read
       4. Read VMSE_Cfg_Read_0.read_data
       
       Recommended Intel SMI 2 Config Write flow:
       1. Write VMSE_Cfg_Write.write_data
       2. Write VMSE_Cfg.command & VMSE_Cfg.write_execute
       3. Read VMSE_Cfg.cfg_cmd_status to make sure it is 0.
       
       This field can also be used to drive a Channel Restart command.
       Bits [7:0] should be set to 8'b01110000
       Bits [23:16] should be set to 8'b01110000
       Bits [27:24] & Bits [11:8] should be set to the desired link width mode as per 
       the Intel SMI 2 spec. 
       Hardware will recognize the command and take care of driving it twice on the 
       command bus. 
       
     */
    UINT32 write_execute : 1;
    /* write_execute - Bits[28:28], RW_LBV, default = 1'b0 
       Software should set this bit to 1 to send the contents of vmse_cfg.command to 
       the Intel SMI 2 Command/Address bus, 
       followed by the data from vmse_cfg_write.write_data in two commands, formatted 
       as per the "Write Config Data A" 
       and "Write Config Data B" commands in the Intel SMI 2 spec.
       This bit will be cleared by hardware after the contents of "Write Config Data B" 
       have been sent and the 
       Intel SMI 2 Error window (vmse_error.vmse_err_latency) has been observed without 
       a retry. 
       Byte enables are transmitted in the Intel SMI 2 Write Config Data A & B 
       commands, however there is not enough 
       space in the vmse_cfg_write register to hold the byte enables and it is not 
       desirable to increase the number of 
       message channel writes necessary for a Intel SMI 2 config write. Therefor the 
       four byte enable bits are encoded in the 
       "don't care" bits of the VMSE_Cfg.command field for the "Write Config Reg" 
       command, bits [27:24]. These bits 
       will be forced to zero by hardware when the "Write Config Reg" command is 
       transmitted. 
     */
    UINT32 read_execute : 1;
    /* read_execute - Bits[29:29], RW_LBV, default = 1'b0 
       Software should set this bit to 1 to send the contents of vmse_cfg.command to 
       the Intel SMI 2 Command/Address bus. 
       This bit will be cleared by hardware after the contents of vmse_cfg.command have 
       been sent and the Intel SMI 2 Error window (vmse_error.vmse_err_latency) has 
       been observed. 
       
     */
    UINT32 cfg_cmd_status : 1;
    /* cfg_cmd_status - Bits[30:30], RW_LBV, default = 1'b0 
       Set to 1 by hardware to indicate that the last config command did not complete 
       successfully. 
       This could be due to either a parity error on the config read data that is 
       returned or an assertion of VMSE_ERR#. 
       Software should clear this bit while writing this register.
       When software clears this bit, ignore_parity_for_read_exec should be equal to 0.
       Clearing this bit will also clear vmse_cfg_status.read_parity.
       This bit will be set to 1 when a Scrambler Sync command is issued by software. 
       This is expected and doesn't mean the Scrambler Sync command is not successful. 
     */
    UINT32 ignore_parity_for_read_exec : 1;
    /* ignore_parity_for_read_exec - Bits[31:31], RW_LB, default = 1'b0 
       Software should set this bit to 1 if the Intel SMI 2 command programmed in the 
       "command" field will not return data from scalable memory buffers. 
       Software is expected to set or clear the bit each time it writes the 
       read_execute bit. 
       Hardware will not clear the bit after issuing the config command.
       Software is expected to clear the bit each time it clears the cfg_cmd_status 
       bit. 
       This bit should be set when sending any VMSE commands other than: VMSE Config 
       Read, Scrambler Sync, Channel Restart 
     */
  } Bits;
  UINT32 Data;
} VMSE_CFG_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_CFG_STATUS_MCDDC_DP_REG supported on:                                 */
/*       IVT_EP (0x40186330)                                                  */
/*       IVT_EX (0x40186330)                                                  */
/*       HSX (0x401A2330)                                                     */
/*       BDX (0x401A2330)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_CFG_STATUS_MCDDC_DP_REG 0x0C014330


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.2.CFG.xml.
 * This register holds the status of the most recent scalable memory buffers config 
 * read access. (EX processor only) 
 */
typedef union {
  struct {
    UINT32 read_parity : 8;
    /* read_parity - Bits[7:0], RW_LBV, default = 8'b00000000 
       Parity for the contents of vmse_cfg_read_0.read_data and 
       vmse_cfg_read_1.read_data. 
       This field can be cleared by clearing vmse_cfg.cfg_cmd_status bit.
     */
    UINT32 vmse_pdamrs_interlock : 1;
    /* vmse_pdamrs_interlock - Bits[8:8], RW_LBV, default = 1'b0 
       This bit should be set to 1 only before doing "kicking off cadb_mrs with a dummy 
       MRS command" and 
       "a config_write to DCALCSR to initiate a PDA MRS " during normal mode (for os 
       vref margining) in 
       this order. It should be 0 at all other times.
       With this bit set, transmission of config_write will be postponed until refresh 
       state machine indicates 
       it is safe to do so.
       Hardware will clear this bit simultanous with vmse_cfgwrite_execute bit
     */
    UINT32 rsvd : 17;
    /* rsvd - Bits[25:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 qrlrdimm2cs0 : 3;
    /* qrlrdimm2cs0 - Bits[28:26], RW_LB, default = 3'b000 
       Set to one for DDR4 quad rank LRDIMM with 2 chip selects. Bit 2 is DIMM2. Bit 1 
       is DIMM1. Bit 0 is DIMM0. 
       Note that tddr4.qrlrdimm2cs[2:0] must be set to 0 in VMSE mode. This field is 
       used for VMSE only 
     */
    UINT32 qrlrdimm2cs1 : 3;
    /* qrlrdimm2cs1 - Bits[31:29], RW_LB, default = 3'b000 
       Set to one for DDR4 quad rank LRDIMM with 2 chip selects. Bit 2 is DIMM2. Bit 1 
       is DIMM1. Bit 0 is DIMM0. 
       Note that tddr4.qrlrdimm2cs[2:0] must be set to 0 in VMSE mode. This field is 
       used for VMSE only 
     */
  } Bits;
  UINT32 Data;
} VMSE_CFG_STATUS_MCDDC_DP_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* DDR4_CA_CTL_MCDDC_DP_REG supported on:                                     */
/*       HSX (0x401A23FC)                                                     */
/*       BDX (0x401A23FC)                                                     */
/* Register default value:              0x00000000                            */
#define DDR4_CA_CTL_MCDDC_DP_REG 0x0C0143FC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * This register holds theControl bits for DDR4 C/A parity error flow logic. (EP, 
 * EP4S and EN processor only) 
 */
typedef union {
  struct {
    UINT32 erf_en0 : 1;
    /* erf_en0 - Bits[0:0], RW, default = 1'b0 
       This should be set to 1 to enable error flow
     */
    UINT32 erf_regpart0 : 1;
    /* erf_regpart0 - Bits[1:1], RW, default = 1'b0 
       This should be set to 1 if RDIMM/LRDIMMs are populated in the channel. Leaving 
       this bit set to zero 
       will result in the machine hanging if no MRS writes are programmed for the error 
       flow. Also, if 
       this bit is zero and there are more than one MRS write programmed, the machine 
       may malfunction. In 
       non-VMSE mode, the error flow engine may only be enabled with DDR4 registered 
       DIMMs so this bit must be 1. 
       In VMSE mode, if DDR4 DIMMs are used they must be registered parts or if not 
       (which is not supported) there 
       must be one and only one MRS write programmed up.
     */
    UINT32 erf_en1 : 1;
    /* erf_en1 - Bits[2:2], RW, default = 1'b0 
       This should be set to 1 to enable error flow
     */
    UINT32 erf_regpart1 : 1;
    /* erf_regpart1 - Bits[3:3], RW, default = 1'b0 
       This should be set to 1 if RDIMM/LRDIMMs are populated in the channel. Leaving 
       this bit set to zero 
       will result in the machine hanging if no MRS writes are programmed for the error 
       flow. Also, if 
       this bit is zero and there are more than one MRS write programmed, the machine 
       may malfunction. In 
       non-VMSE mode, the error flow engine may only be enabled with DDR4 registered 
       DIMMs so this bit must be 1. 
       In VMSE mode, if DDR4 DIMMs are used they must be registered parts or if not 
       (which is not supported) there 
       must be one and only one MRS write programmed up.
     */
    UINT32 tpar_recov_ch0 : 8;
    /* tpar_recov_ch0 - Bits[11:4], RW, default = 8'b00000000 
       largest ( tPAR_ALERT_ON + tPAR_ALERT_PW ) value of a rank populated in channel 
       0. In DCLK. 
     */
    UINT32 tpar_recov_ch1 : 8;
    /* tpar_recov_ch1 - Bits[19:12], RW, default = 8'b00000000 
       largest ( tPAR_ALERT_ON + tPAR_ALERT_PW ) value of a rank populated in channel 
       1. In DCLK. 
     */
    UINT32 rsvd : 12;
    /* rsvd - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DDR4_CA_CTL_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* VMSE_RETRY_SB_ERR_COUNT_MCDDC_DP_REG supported on:                         */
/*       IVT_EP (0x40186400)                                                  */
/*       IVT_EX (0x40186400)                                                  */
/*       HSX (0x401A2400)                                                     */
/*       BDX (0x401A2400)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_RETRY_SB_ERR_COUNT_MCDDC_DP_REG 0x0C014400
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Counts of transient and persistent errors on the Intel SMI 2 Command/Address bus 
 * and Intel SMI 2 Write Data bus. (EX processor only) 
 */
typedef union {
  struct {
    UINT32 transient_err_cnt : 16;
    /* transient_err_cnt - Bits[15:0], RWS_V, default = 16'b0000000000000000 
       Count of transient errors on the Intel SMI 2 Command/Address bus.
       Cleared by hardware when the vmse_retry_sb_err_limit.sb_err_limit is reached and 
       persistent_err_cnt is incremented. 
       Cleared by hardware when the vmse_retry_timer.sb_err_observation_time timer 
       expires. 
     */
    UINT32 persistent_err_cnt : 8;
    /* persistent_err_cnt - Bits[23:16], RWS_V, default = 8'b00000000 
       Count of persistent errors on the Intel SMI 2 Command/Address bus and Write Data 
       Bus errors. 
       Counter saturates at 0xff.
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} VMSE_RETRY_SB_ERR_COUNT_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_RETRY_NB_ERR_COUNT_MCDDC_DP_REG supported on:                         */
/*       IVT_EP (0x40186408)                                                  */
/*       IVT_EX (0x40186408)                                                  */
/*       HSX (0x401A2408)                                                     */
/*       BDX (0x401A2408)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_RETRY_NB_ERR_COUNT_MCDDC_DP_REG 0x0C014408
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Counts of transient and persistent errors on the northbound Intel SMI 2 data bus 
 * (read data bus). (EX processor only) 
 */
typedef union {
  struct {
    UINT32 transient_err_cnt : 16;
    /* transient_err_cnt - Bits[15:0], RWS_V, default = 16'b0000000000000000 
       Count of transient errors on the northbound Intel SMI 2 data bus.
       Cleared by hardware when the vmse_retry_nb_err_limit.nb_err_limit is reached and 
       persistent_err_cnt is incremented. 
       Cleared by hardware when the vmse_retry_timer.nb_err_observation_time timer 
       expires. 
     */
    UINT32 persistent_err_cnt : 8;
    /* persistent_err_cnt - Bits[23:16], RWS_V, default = 8'b00000000 
       Count of persistent errors on the northbound Intel SMI 2 Data bus.
       Counter saturates at 0xff.
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} VMSE_RETRY_NB_ERR_COUNT_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_RETRY_ERR_LIMITS_MCDDC_DP_REG supported on:                           */
/*       IVT_EP (0x4018640C)                                                  */
/*       IVT_EX (0x4018640C)                                                  */
/*       HSX (0x401A240C)                                                     */
/*       BDX (0x401A240C)                                                     */
/* Register default value:              0x00100010                            */
#define VMSE_RETRY_ERR_LIMITS_MCDDC_DP_REG 0x0C01440C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Thresholds at which transient errors are considered persistent and trigger a 
 * link width change. (EX processor only) 
 */
typedef union {
  struct {
    UINT32 nb_err_limit : 15;
    /* nb_err_limit - Bits[14:0], RWS_L, default = 15'b000000000010000 
       Number of northbound VMSE transient errors 
       (vmse_retry_nb_err_count.transient_err_cnt) allowed before the NB Data error is 
       declared persistent. 
       The persistent error can be programmed to signal a SMI or CMCI. See VMSE_MCA_CTL 
       register. 
     */
    UINT32 nb_err_enable : 1;
    /* nb_err_enable - Bits[15:15], RWS_L, default = 1'b0 
       This bit must be set to a 1 to enable the NB transient and NB persistent error 
       counters. Setting this bit to 0 clears NB transient error counter 
       (vmse_retry_nb_err_count.transient_err_cnt) and NB persistent error counter 
       (vmse_retry_nb_err_count.persistent_err_cnt). 
     */
    UINT32 sb_err_limit : 15;
    /* sb_err_limit - Bits[30:16], RWS_L, default = 15'b000000000010000 
       Number of VMSE Cmd/Addr and Write Data transient errors 
       (vmse_retry_sb_err_count.transient_err_cnt) allowed before the SB error is 
       declared persistent. 
       The persistent error can trigger a link width change.
       Note that this field must be set to twice the desired limit or the time window
       (vmse_retry_timer.sb_err_observation_time) needs to be programmed to half the 
       normal sampling rate (one value less). 
     */
    UINT32 sb_err_enable : 1;
    /* sb_err_enable - Bits[31:31], RWS_L, default = 1'b0 
       Enable SB persistent condition. This bit must be set to 1 to enable the SB 
       transient error counter. Setting this bit to 0 clears SB transient error counter 
       (vmse_retry_sb_err_count.transient_err_cnt). 
     */
  } Bits;
  UINT32 Data;
} VMSE_RETRY_ERR_LIMITS_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_RETRY_TIMER_MCDDC_DP_REG supported on:                                */
/*       IVT_EP (0x40186410)                                                  */
/*       IVT_EX (0x40186410)                                                  */
/*       HSX (0x401A2410)                                                     */
/*       BDX (0x401A2410)                                                     */
/* Register default value:              0x80000400                            */
#define VMSE_RETRY_TIMER_MCDDC_DP_REG 0x0C014410
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Configuration settings for the timers for Intel SMI 2 Retry. (EX processor only)
 */
typedef union {
  struct {
    UINT32 vmse_err_flow_time_unit : 10;
    /* vmse_err_flow_time_unit - Bits[9:0], RWS_L, default = 10'b0000000000 
       Holds the 'tick' amount (specified in DCLKs) for the error observation window 
       timers. 
     */
    UINT32 tick_timer_en : 1;
    /* tick_timer_en - Bits[10:10], RWS_L, default = 1'b1 
       Enable the tick timer. Hardware resets the SB and NB error transient and 
       persistent counters as well as the link fail counters when this bit is set to 0. 
     */
    UINT32 sb_err_observation_time : 5;
    /* sb_err_observation_time - Bits[15:11], RWS_L, default = 5'b00000 
       Error observation window (in units of vmse_err_flow_time_unit * 
       2^sb_err_observation_time) during which Intel SMI 2 Command and SB Data 
       transient errors are counted. 
     */
    UINT32 rsvd : 5;
    /* rsvd - Bits[20:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 nb_err_observation_time : 5;
    /* nb_err_observation_time - Bits[25:21], RWS_L, default = 5'b00000 
       Error observation window (in units of vmse_err_flow_time_unit * 
       2^nb_err_observation_time) during which NB transient errors are counted. 
     */
    UINT32 link_fail_observation_time : 5;
    /* link_fail_observation_time - Bits[30:26], RWS_L, default = 5'b00000 
       Error observation window (in units of vmse_err_flow_time_unit * 
       2^link_fail_observation_time) during which link fail events are counted. 
     */
    UINT32 reset_verr_err_cnt : 1;
    /* reset_verr_err_cnt - Bits[31:31], RW_LB, default = 1'b1 
       Hardware resets the SB and NB error transient and persistent counters as well as 
       the link fail 
       counters when this bit changes from a 0 to 1.
     */
  } Bits;
  UINT32 Data;
} VMSE_RETRY_TIMER_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_LINK_FAIL_MCDDC_DP_REG supported on:                                  */
/*       IVT_EP (0x40186414)                                                  */
/*       IVT_EX (0x40186414)                                                  */
/*       HSX (0x401A2414)                                                     */
/*       BDX (0x401A2414)                                                     */
/* Register default value:              0x00000100                            */
#define VMSE_LINK_FAIL_MCDDC_DP_REG 0x0C014414


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.2.CFG.xml.
 * Configuration settings for the Intel SMI 2 Retry link fail state. (EX processor 
 * only) 
 */
typedef union {
  struct {
    UINT32 fail_count : 8;
    /* fail_count - Bits[7:0], RWS_V, default = 8'b00000000 
       Count of persistent southbound errors observed. This counter holds the same 
       value as VMSE_RETRY_SB_ERR_COUNT.persistent_err_cnt. 
     */
    UINT32 fail_threshold : 8;
    /* fail_threshold - Bits[15:8], RWS_L, default = 8'b00000001 
       Threshold of vmse_link_fail.fail_count for which the link will go to the "Link 
       fail" state. 
       fail_threshold = 0 means the first persistent failure in the "Half Width C" 
       state will cause "Link Fail". 
     */
    UINT32 cfg_wr_delay : 8;
    /* cfg_wr_delay - Bits[23:16], RWS_L, default = 8'b00000000 
       Amount of D clock cycles times 16 to delay after issuing a config write before 
       either the next config write 
       or the continuation of the error flow cycle. This setting only affects DDR4 VMSE 
       mode where config writes 
       are required to reset the off chip VMSE logic in the event of the C/A parity 
       error. Setting this field to zero 
       disables the error flow config writes.
     */
    UINT32 cfg_wr_shadow : 7;
    /* cfg_wr_shadow - Bits[30:24], RWS_L, default = 7'b0000000 
       This field holds the shadow copy of bits [30:24] of the DDRIO_WINDOWDATA 
       register in the VMSE interface chip. 
       Bits 30:27 are rdptrinitb0 while bits 26:24 are wrptrinitb0. This shadow 
       register will be used in DDR4 VMSE 
       mode in order to reset the dqfifo during error recovery.
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} VMSE_LINK_FAIL_MCDDC_DP_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* VMSE_RETRY_SELF_REFRESH_ENTER_MCDDC_DP_REG supported on:                   */
/*       IVT_EP (0x40186418)                                                  */
/*       IVT_EX (0x40186418)                                                  */
/*       HSX (0x401A2418)                                                     */
/*       BDX (0x401A2418)                                                     */
/* Register default value:              0x20000000                            */
#define VMSE_RETRY_SELF_REFRESH_ENTER_MCDDC_DP_REG 0x0C014418
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Timer and settings for the Intel SMI 2 Retry flow's Self Refresh Enter for the 
 * scalable memory buffers. (EX processor only) 
 */
typedef union {
  struct {
    UINT32 sre_timer : 14;
    /* sre_timer - Bits[13:0], RO_V, default = 14'b00000000000000 
       Current value of the timer for scalable memory buffers to enter self refresh.
       This timer will start counting when VMSE_ERR# is active.
       The timer will be cleared by hardware when sre_timer == mxb_sre_time.
     */
    UINT32 rsvd_14 : 2;
    /* rsvd_14 - Bits[15:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 mxb_sre_time : 14;
    /* mxb_sre_time - Bits[29:16], RWS_L, default = 14'b10000000000000 
       Time (in DCLKs) necessary for the scalable memory buffers to put the DIMMs in 
       Self Refresh. 
       When sre_timer == mxb_sre_time, the timer will be cleared and the Intel SMI 2 
       Retry flow may proceed. 
       Max expected value for this register is 5305 DCLKs based on scalable memory 
       buffers design. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} VMSE_RETRY_SELF_REFRESH_ENTER_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_LINK_WIDTH_MCDDC_DP_REG supported on:                                 */
/*       IVT_EP (0x4018641C)                                                  */
/*       IVT_EX (0x4018641C)                                                  */
/*       HSX (0x401A241C)                                                     */
/*       BDX (0x401A241C)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_LINK_WIDTH_MCDDC_DP_REG 0x0C01441C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Intel SMI 2 Link width status and override settings. (EX processor only)
 */
typedef union {
  struct {
    UINT32 current_width : 4;
    /* current_width - Bits[3:0], RO_V, default = 4'b0000 
       Current VMSE Link width state:
       0100 Full Width
       0011 Half Width A
       0010 Half Width B
       0001 Half Width C
       1xxx Link Fail
     */
    UINT32 rsvd_4 : 3;
    /* rsvd_4 - Bits[6:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 override_link_width : 4;
    /* override_link_width - Bits[10:7], RW_LB, default = 4'b0000 
       State to force the link width to, via override_forever or override_once. 
       Encoding: 
       0000 Full Width
       0011 Half Width A
       0010 Half Width B
       0001 Half Width C
       Others Not supported
     */
    UINT32 rsvd_11 : 5;
    /* rsvd_11 - Bits[15:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 override_once : 1;
    /* override_once - Bits[16:16], RW_LBV, default = 1'b0 
       When this bit is set, on the next Intel SMI 2 Retry (either naturally occuring 
       or forced by software), the link 
       will restart in the state specified by "override_link_width", at which point 
       hardware will clear this bit. 
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 override_forever : 1;
    /* override_forever - Bits[24:24], RW_LB, default = 1'b0 
       When this bit is set, on the next Intel SMI 2 Retry (either naturally occuring 
       or forced by software), the link 
       will restart in the state specified by "override_link_width". This bit will 
       never be cleared by hardware, 
       so this could force the link into a perpetually bad state.
     */
    UINT32 enable_verr_cmci : 4;
    /* enable_verr_cmci - Bits[28:25], RW_LB, default = 4'b0000 
       This field is not used and can be treated as spare bits. The control for 
       signalling 
       errors due VMSE Error FSM resides in VMSE_MCA_CTL register.
     */
    UINT32 ena_cpgc_verr_fsm : 1;
    /* ena_cpgc_verr_fsm - Bits[29:29], RW_LB, default = 1'b0 
       This field enables VERR_FSM in CPGC mode.
       1'b0: VERR_FSM is not enabled in CPGC mode.
       1'b1: VERR_FSM is enabled in CPGC mode.
       
       BIOS is expected to enable this mode if VMSE_ERR is to be observed in CPGC mode.
       Specific use case is during MEM_INIT -
       a. BIOS enables this field prior to MEM_INIT
       b. VMSE_ERR asserts during MEM_INIT
       c. With this field set - VMSE ERR FSM is enabled and brings DIMM's out of 
       self-refresh, issues channel-restart. 
       
       Additionally, CPGC sequencer will stop sending transactions if 
       cpgc_err_ctl.stop_on_errcontrol[2:0] == {3'b100, 3'b101} 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} VMSE_LINK_WIDTH_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_ERR_FSM_STATE_MCDDC_DP_REG supported on:                              */
/*       IVT_EP (0x40186420)                                                  */
/*       IVT_EX (0x40186420)                                                  */
/*       HSX (0x401A2420)                                                     */
/*       BDX (0x401A2420)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_ERR_FSM_STATE_MCDDC_DP_REG 0x0C014420
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * State of the Error flow FSM.
 * Note this is used by the EN, EP, EP 4S and EX processor.
 */
typedef union {
  struct {
    UINT32 current_state : 4;
    /* current_state - Bits[3:0], RO_V, default = 4'b0000 
       The current state of the Intel SMI 2 Error flow FSM.
     */
    UINT32 rsvd_4 : 12;
    /* rsvd_4 - Bits[15:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 previous_state : 4;
    /* previous_state - Bits[19:16], RO_V, default = 4'b0000 
       The state of the Intel SMI 2 Error flow FSM before the current state.
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} VMSE_ERR_FSM_STATE_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_ERR_TIMEOUT_MCDDC_DP_REG supported on:                                */
/*       IVT_EP (0x40186424)                                                  */
/*       IVT_EX (0x40186424)                                                  */
/*       HSX (0x401A2424)                                                     */
/*       BDX (0x401A2424)                                                     */
/* Register default value:              0x02000000                            */
#define VMSE_ERR_TIMEOUT_MCDDC_DP_REG 0x0C014424
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Timeout counter and controls for the Intel SMI 2 Error flow. (EX processor only)
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 8;
    /* rsvd_0 - Bits[7:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 timeout_link_width_state : 4;
    /* timeout_link_width_state - Bits[11:8], RO_V, default = 4'b0000 
       The state of the Link Width Mode when the timeout_limit was hit.
     */
    UINT32 rsvd_12 : 4;
    /* rsvd_12 - Bits[15:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 timeout_limit : 6;
    /* timeout_limit - Bits[21:16], RWS_L, default = 6'b000000 
       Value of timeout_counter at which the timeout_hit event is logged. Maximum time 
       window (in units of vmse_err_flow_time_unit * 2^timeout_limit) for successful 
       completion of the Intel SMI 2 Retry flow. 
     */
    UINT32 rsvd_22 : 2;
    /* rsvd_22 - Bits[23:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 timeout_hit : 1;
    /* timeout_hit - Bits[24:24], RW_V, default = 1'b0 
       This bit is set to 1 by hardware when the timeout_counter reaches the 
       timeout_limit value. 
       Do not use. THe above definition can not be guaranteed
       //HSD273247
     */
    UINT32 timeout_smi_en : 1;
    /* timeout_smi_en - Bits[25:25], RW_LB, default = 1'b1 
       When this bit is set an SMI will be generated if the timeout_limit is hit.
       Do not use. THis bit currently does not do anything
       //HSD273247
     */
    UINT32 rsvd_26 : 2;
    /* rsvd_26 - Bits[27:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 timeout_fsm_state : 4;
    /* timeout_fsm_state - Bits[31:28], RO_V, default = 4'b0000 
       The state of the Intel SMI 2 Error flow FSM when the timeout_limit was hit.
     */
  } Bits;
  UINT32 Data;
} VMSE_ERR_TIMEOUT_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_RDWR_SPACING_MCDDC_DP_REG supported on:                               */
/*       IVT_EP (0x40186434)                                                  */
/*       IVT_EX (0x40186434)                                                  */
/*       HSX (0x401A2434)                                                     */
/*       BDX (0x401A2434)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_RDWR_SPACING_MCDDC_DP_REG 0x0C014434
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Force space between successive read CAS or write push
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 7;
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_8 : 7;
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 vmse_wp_start_dec : 5;
    /* vmse_wp_start_dec - Bits[20:16], RW_LB, default = 5'b00000 
       Decrement start location of write push; this is used to account for flyby and 
       other delay in DDRIO; in unit of qclk 
     */
    UINT32 rsvd_21 : 1;
    /* rsvd_21 - Bits[21:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 vmse_wp_start_inc : 5;
    /* vmse_wp_start_inc - Bits[26:22], RW_LB, default = 5'b00000 
       Increment start location of write push; this is used to account for flyby and 
       other delay in DDRIO; in unit of qclk 
     */
    UINT32 rsvd_27 : 1;
    /* rsvd_27 - Bits[27:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 mxb_free_delay : 4;
    /* mxb_free_delay - Bits[31:28], RW_LB, default = 4'b0000 
       The time (in DCLKs) to delay freeing of MXB buffer after write push is done.
       When this value is 0, freeing happens immediately.
     */
  } Bits;
  UINT32 Data;
} VMSE_RDWR_SPACING_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_RDWR_SPACING_2_MCDDC_DP_REG supported on:                             */
/*       IVT_EP (0x40186438)                                                  */
/*       IVT_EX (0x40186438)                                                  */
/*       HSX (0x401A2438)                                                     */
/*       BDX (0x401A2438)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_RDWR_SPACING_2_MCDDC_DP_REG 0x0C014438
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Force space between read CAS or write push
 */
typedef union {
  struct {
    UINT32 read_pos0_ch0 : 7;
    /* read_pos0_ch0 - Bits[6:0], RW_LB, default = 7'b0000000 
       When read counter is equal to this value, allow channel 0 to send read CAS
     */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 read_pos1_ch0 : 7;
    /* read_pos1_ch0 - Bits[14:8], RW_LB, default = 7'b0000000 
       When read counter is equal to this value, allow channel 0 to send read CAS
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 read_pos0_ch1 : 7;
    /* read_pos0_ch1 - Bits[22:16], RW_LB, default = 7'b0000000 
       When read counter is equal to this value, allow channel 1 to send read CAS
     */
    UINT32 rsvd_23 : 1;
    /* rsvd_23 - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 read_pos1_ch1 : 7;
    /* read_pos1_ch1 - Bits[30:24], RW_LB, default = 7'b0000000 
       When read counter is equal to this value, allow channel 1 to send read CAS
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} VMSE_RDWR_SPACING_2_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_CLKSTP_TIMER_MCDDC_DP_REG supported on:                               */
/*       IVT_EP (0x4018643C)                                                  */
/*       IVT_EX (0x4018643C)                                                  */
/*       HSX (0x401A243C)                                                     */
/*       BDX (0x401A243C)                                                     */
/* Register default value:              0x04000400                            */
#define VMSE_CLKSTP_TIMER_MCDDC_DP_REG 0x0C01443C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Force space between read CAS or write push
 */
typedef union {
  struct {
    UINT32 clkstp_sre_delay : 16;
    /* clkstp_sre_delay - Bits[15:0], RW_LB, default = 16'b0000010000000000 
       Delay in DCLKS for DDR SRE in PC6. The counter is used by the Clock Stop FSM
       while while entering DRAM Clock Stop during PC6.
     */
    UINT32 clkstp_tstab_delay : 16;
    /* clkstp_tstab_delay - Bits[31:16], RW_LB, default = 16'b0000010000000000 
       Delay in DCLKS for Tstab. The counter is used by the Clock Stop FSM
       while exiting DRAM Clock Stop during PC6.
     */
  } Bits;
  UINT32 Data;
} VMSE_CLKSTP_TIMER_MCDDC_DP_STRUCT;
#endif /* ASM_INC */




/* CPGC_PATWDBCLMUX_LMN_MCDDC_DP_REG supported on:                            */
/*       IVT_EP (0x40186500)                                                  */
/*       IVT_EX (0x40186500)                                                  */
/*       HSX (0x401A2500)                                                     */
/*       BDX (0x401A2500)                                                     */
/* Register default value:              0x01010100                            */
#define CPGC_PATWDBCLMUX_LMN_MCDDC_DP_REG 0x0C014500
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * CPGC Pattern WDB Cache-Line Mux LMN Control -
 * Template for the LMN Counter used for creating periodic pattern (M = HighChunk 
 * transfers + N= Low Chunk transfers with an initial delay = L Chunk transfers) 
 */
typedef union {
  struct {
    UINT32 l_datsel : 1;
    /* l_datsel - Bits[0:0], RW_LB, default = 1'b0 
       0x0 = logic 0 must be driven during the initial L counter delay
       0x1=logic 1 must be driven during the initial L counter delay
     */
    UINT32 sweep_freq : 1;
    /* sweep_freq - Bits[1:1], RW_LB, default = 1'b0 
       After L_Counter Chunk transfers are driven at the beginning of a test then 
       periodic pattern is continuously repeated depending on the value of Sweep 
       Frequency. 
       
       If Sweep_Frequency = 0 then the following pattern is continuously repeated:
       
       1) The opposite polarity as L (i.e. If L is high M is then Low) must be driven 
       for M+1 Qclk cycles 
       2) Followed immediately by the same polarity as L (i.e. If L is high N is then 
       High) must be driven for N+1 Qclk cycles 
       3) Repeat step 1)
       
       If Sweep_Frequency = 0 and and if either L, M, or N = 0 then the the state will 
       freeze in whoever is programmed to 0 starting with L then M and then N in terms 
       of priority. 
       If Sweep_Frequency =1 then the following pattern is continuously repeated:
       
       1) X = M
       2) X is driven opposite polarity as L (i.e. If L is high then the current 
       polarity is Low) for X Qclk cycles. 
       3) The same polarity as L (i.e. If L is high then the current polarity is High) 
       must be driven for X Qclk cycles. 
       4) The opposite polarity as L is driven for X Qclk cycles.
       5) X = X +1
       6) The Same polarity as L is driven for X Qclk cycles.
       7) If X = N then go to step 1 else go to step 2
       
       L, M, and N must never be programmed to 0 if Sweep_Frequency = 1 and is 
       considered undefined. 
     */
    UINT32 rsvd : 6;
    /* rsvd - Bits[7:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 l_cnt : 8;
    /* l_cnt - Bits[15:8], RW_LB, default = 8'b00000001 
       After L_Counter Chunk transfers are driven at the beginning of a test then 
       periodic pattern is continuously repeated depending on the value of Sweep 
       Frequency. 
       
       If Sweep_Frequency = 0 then a steady state frequency is driven (see 
       Sweep_Frequency for exact behavior). 
       If Sweep_Frequency = 1 then a frequency sweep is continuously generated ranging 
       from 2*(M) to 2*(N) driven (see Sweep_Frequency for exact behavior). 
     */
    UINT32 m_cnt : 8;
    /* m_cnt - Bits[23:16], RW_LB, default = 8'b00000001 
       After L_Counter Chunk transfers are driven at the beginning of a test then 
       periodic pattern is continuously repeated depending on the value of Sweep 
       Frequency. 
       
       If Sweep_Frequency = 0 then a steady state frequency is driven (see 
       Sweep_Frequency for exact behavior). 
       If Sweep_Frequency = 1 then a frequency sweep is continuously generated ranging 
       from 2*(M) to 2*(N) driven (see Sweep_Frequency for exact behavior). 
     */
    UINT32 n_cnt : 8;
    /* n_cnt - Bits[31:24], RW_LB, default = 8'b00000001 
       After L_Counter Chunk transfers are driven at the beginning of a test then 
       periodic pattern is continuously repeated depending on the value of Sweep 
       Frequency. 
       
       If Sweep_Frequency = 0 then a steady state frequency is driven (see 
       Sweep_Frequency for exact behavior). 
       If Sweep_Frequency = 1 then a frequency sweep is continuously generated ranging 
       from 2*(M) to 2*(N) driven (see Sweep_Frequency for exact behavior). 
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDBCLMUX_LMN_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CPGC_PATWDBCL_MUX0_PBWR_MCDDC_DP_REG supported on:                         */
/*       IVT_EP (0x4018650C)                                                  */
/*       IVT_EX (0x4018650C)                                                  */
/*       HSX (0x401A250C)                                                     */
/*       BDX (0x401A250C)                                                     */
/* Register default value:              0x00AAAAAA                            */
#define CPGC_PATWDBCL_MUX0_PBWR_MCDDC_DP_REG 0x0C01450C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * CPGC Pattern WDB Cache-Line Mux0 PB Write -
 * Template for the 24 Bit Buffer, also used for the LFSR seed.
 */
typedef union {
  struct {
    UINT32 patbuffer : 24;
    /* patbuffer - Bits[23:0], RW_V, default = 24'b101010101010101010101010 
       Used to program the initial value for of the Pattern/LFSR Buffer in the Read 
       comparison domain. 
       If the LFSR reload and save registrers are being used then this register will 
       not hold the initial value, but instead will hold the last saved value. 
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDBCL_MUX0_PBWR_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CPGC_PATWDBCL_MUX1_PBWR_MCDDC_DP_REG supported on:                         */
/*       IVT_EP (0x40186510)                                                  */
/*       IVT_EX (0x40186510)                                                  */
/*       HSX (0x401A2510)                                                     */
/*       BDX (0x401A2510)                                                     */
/* Register default value:              0x00CCCCCC                            */
#define CPGC_PATWDBCL_MUX1_PBWR_MCDDC_DP_REG 0x0C014510
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * CPGC Pattern WDB Cache-Line Mux1 PB Write -
 * Template for the 24 Bit Buffer, also used for the LFSR seed.
 */
typedef union {
  struct {
    UINT32 patbuffer : 24;
    /* patbuffer - Bits[23:0], RW_V, default = 24'b110011001100110011001100 
       Used to program the initial value for of the Pattern/LFSR Buffer in the Read 
       comparison domain. 
       If the LFSR reload and save registrers are being used then this register will 
       not hold the initial value, but instead will hold the last saved value. 
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDBCL_MUX1_PBWR_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CPGC_PATWDBCL_MUX2_PBWR_MCDDC_DP_REG supported on:                         */
/*       IVT_EP (0x40186514)                                                  */
/*       IVT_EX (0x40186514)                                                  */
/*       HSX (0x401A2514)                                                     */
/*       BDX (0x401A2514)                                                     */
/* Register default value:              0x00F0F0F0                            */
#define CPGC_PATWDBCL_MUX2_PBWR_MCDDC_DP_REG 0x0C014514
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * CPGC Pattern WDB Cache-Line Mux2 PB Write -
 * Template for the 24 Bit Buffer, also used for the LFSR seed.
 */
typedef union {
  struct {
    UINT32 patbuffer : 24;
    /* patbuffer - Bits[23:0], RW_V, default = 24'b111100001111000011110000 
       Used to program the initial value for of the Pattern/LFSR Buffer in the Read 
       comparison domain. 
       If the LFSR reload and save registrers are being used then this register will 
       not hold the initial value, but instead will hold the last saved value. 
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDBCL_MUX2_PBWR_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CPGC_PATWDBCL_MUX0_PBRD_MCDDC_DP_REG supported on:                         */
/*       IVT_EP (0x40186518)                                                  */
/*       IVT_EX (0x40186518)                                                  */
/*       HSX (0x401A2518)                                                     */
/*       BDX (0x401A2518)                                                     */
/* Register default value:              0x00AAAAAA                            */
#define CPGC_PATWDBCL_MUX0_PBRD_MCDDC_DP_REG 0x0C014518
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * CPGC Pattern WDB Cache-Line Mux0 PB Read -
 * Template for the 24 Bit Buffer, also used for the LFSR seed.
 */
typedef union {
  struct {
    UINT32 patbuffer : 24;
    /* patbuffer - Bits[23:0], RW_V, default = 24'b101010101010101010101010 
       Used to program the initial value for of the Pattern/LFSR Buffer in the Read 
       comparison domain. 
       If the LFSR reload and save registrers are being used then this register will 
       not hold the initial value, but instead will hold the last saved value. 
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDBCL_MUX0_PBRD_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CPGC_PATWDBCL_MUX1_PBRD_MCDDC_DP_REG supported on:                         */
/*       IVT_EP (0x4018651C)                                                  */
/*       IVT_EX (0x4018651C)                                                  */
/*       HSX (0x401A251C)                                                     */
/*       BDX (0x401A251C)                                                     */
/* Register default value:              0x00CCCCCC                            */
#define CPGC_PATWDBCL_MUX1_PBRD_MCDDC_DP_REG 0x0C01451C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * CPGC Pattern WDB Cache-Line Mux1 PB Read -
 * Template for the 24 Bit Buffer, also used for the LFSR seed.
 */
typedef union {
  struct {
    UINT32 patbuffer : 24;
    /* patbuffer - Bits[23:0], RW_V, default = 24'b110011001100110011001100 
       Used to program the initial value for of the Pattern/LFSR Buffer in the Read 
       comparison domain. 
       If the LFSR reload and save registrers are being used then this register will 
       not hold the initial value, but instead will hold the last saved value. 
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDBCL_MUX1_PBRD_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CPGC_PATWDBCL_MUX2_PBRD_MCDDC_DP_REG supported on:                         */
/*       IVT_EP (0x40186520)                                                  */
/*       IVT_EX (0x40186520)                                                  */
/*       HSX (0x401A2520)                                                     */
/*       BDX (0x401A2520)                                                     */
/* Register default value:              0x00F0F0F0                            */
#define CPGC_PATWDBCL_MUX2_PBRD_MCDDC_DP_REG 0x0C014520
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * CPGC Pattern WDB Cache-Line Mux2 PB Read -
 * Template for the 24 Bit Buffer, also used for the LFSR seed.
 */
typedef union {
  struct {
    UINT32 patbuffer : 24;
    /* patbuffer - Bits[23:0], RW_V, default = 24'b111100001111000011110000 
       Used to program the initial value for of the Pattern/LFSR Buffer in the Read 
       comparison domain. 
       If the LFSR reload and save registrers are being used then this register will 
       not hold the initial value, but instead will hold the last saved value. 
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDBCL_MUX2_PBRD_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CPGC_PATWDB_INV_MCDDC_DP_REG supported on:                                 */
/*       IVT_EP (0x40186524)                                                  */
/*       IVT_EX (0x40186524)                                                  */
/*       HSX (0x401A2524)                                                     */
/*       BDX (0x401A2524)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_PATWDB_INV_MCDDC_DP_REG 0x0C014524
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * CPGC Pattern WDB Inversion and DC Control -
 * Control Logic used to program and control the WDB Inversion Mask
 */
typedef union {
  struct {
    UINT32 eccinv_or_dcenb : 8;
    /* eccinv_or_dcenb - Bits[7:0], RW_LB, default = 8'b00000000 
       Each bit in ECC_Inv_or_DC_Enable corresponds to a specific ECC Lane.
       When any bit is set this indicates that the selected data lane must be inverted 
       or drive a DC signal based on Data_INV_or_DC_Control. 
       
       When Enable_Inv_or_DC_Rotate is set the Data_Inv_or_DC_Enable and 
       ECC_Inv_or_DC_Enable are considered one continuous shift register with 
       Data_Inv_or_DC_Enable making up bits bits 0:63 and ECC_Inv_or_DC_Enable making 
       up bits 64:71 
       
       Bit 0 of ECC_Inv_or_DC_Enable corresponds to ECC 0.
     */
    UINT32 rsvd_8 : 8;
    /* rsvd_8 - Bits[15:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 invordc_shft_rate : 4;
    /* invordc_shft_rate - Bits[19:16], RW_LB, default = 4'b0000 
       If Inv_or_DC_Shift_Rate is enabled then the Data_Inv_or_DC_Enable and 
       ECC_Inv_or_DC_Enable will be left shifted every time 2^(Inv_or_DC_Shift_Rate) 
       cachelines are executed. 
       
       When a rotation happens:
       Bit 0 of Data_Inv_or_DC_Enable rotates into Bit 1 of Data_Inv_or_DC_Enable
       Bit 63 of Data_Inv_or_DC_Enable rotates into Bit 0 of the ECC_Inv_or_DC_Enable
       Bit 7 of ECC_Inv_or_DC_Enable rotates into Bit 0 of the Data_Inv_or_DC_Enable
     */
    UINT32 dc_polarity : 1;
    /* dc_polarity - Bits[20:20], RW_LB, default = 1'b0 
       DC_Polarity_Control indicates whether the polarity of the DC signal driven when 
       Inv_or_DC_Control = 1 will be logic high or logic low. 
       
       DC_Polarity_Control = 0 indicates a logic low will be driven.
       DC_Polarity_Control = 1 indicates a logic high will be driven.
     */
    UINT32 rsvd_21 : 9;
    /* rsvd_21 - Bits[29:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 invordc_ctl : 1;
    /* invordc_ctl - Bits[30:30], RW_LB, default = 1'b0 
       Inv_or_DC_Control controls whether a Lane is inverted in polarity or a DC signal 
       will be driven out for any bits set in the ECC_Inv_or_DC_Enable and 
       Lane_Inv_or_DC_Enable. 
       
       Inv_or_DC_Control = 0 indicates the selected lane will be inverted
       Inv_or_DC_Control = 1 indicates the selected lane will drive a DC signal with a 
       logic value dictated by the DC_Polarity. 
     */
    UINT32 datainv_or_dcshftenb : 1;
    /* datainv_or_dcshftenb - Bits[31:31], RW_LB, default = 1'b0 
       When Inv_or_DC_Shift_Enable is set to 1 then the Data_Inv_or_DC_Enable and 
       ECC_Inv_or_DC_Enable will be left shifted every time 2^(Inv_or_DC_Shift_Rate) 
       cachelines are executed. 
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDB_INV_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CPGC_PATWDB_INV0_MCDDC_DP_REG supported on:                                */
/*       IVT_EP (0x40186528)                                                  */
/*       IVT_EX (0x40186528)                                                  */
/*       HSX (0x401A2528)                                                     */
/*       BDX (0x401A2528)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_PATWDB_INV0_MCDDC_DP_REG 0x0C014528
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * CPGC Pattern WDB Inversion Mask Lower -
 * Control Logic used to program and control the WDB Inversion Mask - Bits 31:0
 */
typedef union {
  struct {
    UINT32 datainv_or_dcenb : 32;
    /* datainv_or_dcenb - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Each bit in Data_Inv_or_DC_Enable corresponds to a specific data Lane. [31:0]
       When any bit is set this indicates that the selected data lane must be inverted 
       or drive a DC signal based on Data_INV_or_DC_Control. 
       
       When Enable_Inv_or_DC_Rotate is set the Data_Inv_or_DC_Enable and 
       ECC_Inv_or_DC_Enable are considered one continuous shift register with 
       Data_Inv_or_DC_Enable making up bits bits 0:63 and ECC_Inv_or_DC_Enable making 
       up bits 64:71. 
       
       Bit 0 of Data_Inv_or_DC_Enable corresponds to lane 0.
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDB_INV0_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CPGC_PATWDB_INV1_MCDDC_DP_REG supported on:                                */
/*       IVT_EP (0x4018652C)                                                  */
/*       IVT_EX (0x4018652C)                                                  */
/*       HSX (0x401A252C)                                                     */
/*       BDX (0x401A252C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_PATWDB_INV1_MCDDC_DP_REG 0x0C01452C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * CPGC Pattern WDB Inversion Mask Upper -
 * Control Logic used to program and control the WDB Inversion Mask - Bits 63:32
 */
typedef union {
  struct {
    UINT32 datainv_or_dcenb : 32;
    /* datainv_or_dcenb - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Each bit in Data_Inv_or_DC_Enable corresponds to a specific data Lane. [63:32]
       When any bit is set this indicates that the selected data lane must be inverted 
       or drive a DC signal based on Data_INV_or_DC_Control. 
       
       When Enable_Inv_or_DC_Rotate is set the Data_Inv_or_DC_Enable and 
       ECC_Inv_or_DC_Enable are considered one continuous shift register with 
       Data_Inv_or_DC_Enable making up bits bits 0:63 and ECC_Inv_or_DC_Enable making 
       up bits 64:71. 
       
       Bit 0 of Data_Inv_or_DC_Enable corresponds to lane 0.
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDB_INV1_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CPGC_PATWDB_RDWR_PNTR_MCDDC_DP_REG supported on:                           */
/*       HSX (0x401A2530)                                                     */
/*       BDX (0x401A2530)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_PATWDB_RDWR_PNTR_MCDDC_DP_REG 0x0C014530

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Used to indicate what entry of the WDB is being read and written
 */
typedef union {
  struct {
    UINT32 rdwr_pntr : 6;
    /* rdwr_pntr - Bits[5:0], RW_V, default = 6'b000000 
       WDB_Read_Write_Pointer indicates what cacheline in the WDB that is being indexed 
       in the REUT_CH#_PAT_WDB_PROG#.WDB_Data registers for writing of the contents of 
       the WDB (See WDB_Data for more details). 
       Programming sequence : write a value to PAT_WDB_RD_WR_PNTR;write a value to 
       CPGC_PATWDB_WR0; write a value to CPGC_PATWDB_WR1; at this point the 64-bit 
       value {CPGC_PATWDB_WR1, CPGC_PATWDB_WR0} will be transfered to the chunk 
       Rdwr_Subpntr of WDB_entry rdwr_pntr. Rdwr_subpntr will be auto_incremented after 
       each transfer like this and rdwr_pntr will be auto_incremented after all 8 
       chunks of a cacheline have been updated. 
     */
    UINT32 rdwr_subpntr : 3;
    /* rdwr_subpntr - Bits[8:6], RW_V, default = 3'b000 
       WDB_Read_Write_Sub_Pointer indicates what portion of a cacheline(64-bits) in the 
       WDB that is being indexed in the REUT_CH#_PAT_WDB_READ.WDB_Data and 
       REUT_CH#_PAT_WDB_WRITE.WDB_Data registers for writing of the contents of the 
       WDB. 
     */
    UINT32 rsvd : 23;
    /* rsvd - Bits[31:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDB_RDWR_PNTR_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_PATWDBCL_MUXCTL_MCDDC_DP_REG supported on:                            */
/*       IVT_EP (0x40186534)                                                  */
/*       IVT_EX (0x40186534)                                                  */
/*       HSX (0x401A2534)                                                     */
/*       BDX (0x401A2534)                                                     */
/* Register default value:              0x00004049                            */
#define CPGC_PATWDBCL_MUXCTL_MCDDC_DP_REG 0x0C014534


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.2.CFG.xml.
 * CPGC Pattern WDB Cache-Line Mux Control -
 * Control Logic used to determine which chunk of cacheline data will be sent out 
 * each Q clk (8:1 Mux) 
 */
typedef union {
  struct {
    UINT32 mux0 : 2;
    /* mux0 - Bits[1:0], RW_LB, default = 2'b01 
       0x00 = Select LMN counter
       0x01 = Select 32 bit buffer
       0x10 = Select LFSR32
       0x11 = Reserved
     */
    UINT32 rsvd_2 : 1;
    /* rsvd_2 - Bits[2:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 mux1 : 2;
    /* mux1 - Bits[4:3], RW_LB, default = 2'b01 
       0x00 = Reserved
       0x01 = Select 32 bit buffer
       0x10 = Select LFSR32
       0x11 = Reserved
     */
    UINT32 rsvd_5 : 1;
    /* rsvd_5 - Bits[5:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 mux2 : 2;
    /* mux2 - Bits[7:6], RW_LB, default = 2'b01 
       0x00 = Reserved
       0x01 = Select 32 bit buffer
       0x10 = Select LFSR32
       0x11 = Reserved
     */
    UINT32 rsvd_8 : 6;
    /* rsvd_8 - Bits[13:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ecc_replace_byte : 1;
    /* ecc_replace_byte - Bits[14:14], RW_LB, default = 1'b1 
       ecc_replace_byte will allow the capture ECC cacheline errors into the WDB when 
       Enable_WDB_Error_Capture is set to 1. 
       
       ecc_replace_byte is used by the Read data path to Mux the received ECC byte into 
       the same byte group indicated by ecc_datasrc_sel. 
       
       If ecc_replace_byte is set to 0 then ECC XOR data (expected vs. received) will 
       never be captured in the WDB. 
       If ecc_replace_byte is set to 1 then ECC XOR data (expected vs. received) will 
       replace the XOR results of a selected data group 
       (indicated by ecc_datasrc_sel) when cacheline errors are written into the WDB.
     */
    UINT32 ecc_datasrc_sel : 1;
    /* ecc_datasrc_sel - Bits[15:15], RW_LB, default = 1'b0 
       Indicates whether byte 0 (Dq0-Dq7) or byte 7 (dq56-dq63) is transmitted and 
       compared for the ECC byte. 
       
       0= byte group 0 will be transmitted and compared for the ECC byte.
       1= byte group 7 will be transmitted and compared for the ECC byte.
       
       Hardware directs if ECC is included within Generation and Checking.
     */
    UINT32 save_lfsr_seedrate : 6;
    /* save_lfsr_seedrate - Bits[21:16], RW_LB, default = 6'b000000 
       save_lfsr_seedrate * ^(rld_lfsr_seedrate-1) defines the periodic cacheline 
       interval that the current LFSR value REUT#_CH#_PAT_CL_MUX#_PB_STATUS 
       is saved into the REUT#_CH#_PAT_CL_MUX#_PB register.
       
       When the save_lfsr_seedrate and rld_lfsr_seedrate is reached in the same cycle 
       only the saving of the LFSR seed takes place. 
       
       Examples:
       0 = Disable - the REUT#_CH#_PAT_CL_MUX#_PB_STATUS is never saved into the 
       REUT#_CH#_PAT_CL_MUX#_PB register. 
       1 = The REUT#_CH#_PAT_CL_MUX#_PB_STATUS is saved into the 
       REUT#_CH#_PAT_CL_MUX#_PB register every 2^(Reload_LFSR_Seed_Rate-1) cachelines. 
       2 = The REUT#_CH#_PAT_CL_MUX#_PB_STATUS is saved into the 
       REUT#_CH#_PAT_CL_MUX#_PB register every other 2^(Reload_LFSR_Seed_Rate-1) 
       cachelines. 
       3 = The REUT#_CH#_PAT_CL_MUX#_PB_STATUS is saved into the 
       REUT#_CH#_PAT_CL_MUX#_PB register every fourth 2^(Reload_LFSR_Seed_Rate-1) 
       cachelines. 
     */
    UINT32 rsvd_22 : 2;
    /* rsvd_22 - Bits[23:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rld_lfsr_seedrate : 3;
    /* rld_lfsr_seedrate - Bits[26:24], RW_LB, default = 3'b000 
       2^(rld_lfsr_seedrate-1) defines the periodic cacheline interval that the current 
       LFSR Seed is reloaded from REUT#_CH#_PAT_CL_MUX#_PB. 
       
       When the save_lfsr_seedrate and rld_lfsr_seedrate is reached in the same cycle 
       only the saving of the LFSR seed takes place. 
       
       Examples:
       0 = Disable - the REUT#_CH#_PAT_CL_MUX#_PB_STATUS is never reloaded into the 
       REUT#_CH#_PAT_CL_MUX#_PB register. 
       1 = The LFSR seed is reloaded every cacheline operation from 
       REUT#_CH#_PAT_CL_MUX#_PB. 
       2 = The LFSR seed is reloaded every other cacheline operation from 
       REUT#_CH#_PAT_CL_MUX#_PB 
       3 = The LFSR seed is reloaded every fourth cacheline operation from 
       REUT#_CH#_PAT_CL_MUX#_PB 
     */
    UINT32 cpgc_lfsr_2code_mode : 1;
    /* cpgc_lfsr_2code_mode - Bits[27:27], RW_LB, default = 1'b0 
       Set this bit fr CPGC training or DIMM testing under Intel SMI 2 2:1 LFSR mode. 
       This bit will allow the LFSR to generate 
       4 LFSR codes per DCLK. The default mode is 2 codes per DCLK. The default mode is 
       used in native mode and Intel SMI 2 1:1 
       modes.
     */
    UINT32 cpgc_pb_2code_modedis : 1;
    /* cpgc_pb_2code_modedis - Bits[28:28], RW_LB, default = 1'b0 
       if 0, pattern Buffer will work in 2 step mode when cpgc_lfsr_2code_mode=1.
       if 1, pattern Buffer will not wrok in 2step mode when cpgc_lfsr_2code_mode=1 
       (legacy behaviour) 
     */
    UINT32 cpgc_lmn_2code_modedis : 1;
    /* cpgc_lmn_2code_modedis - Bits[29:29], RW_LB, default = 1'b0 
       if 0, LMN counters will work in 2 step mode when cpgc_lfsr_2code_mode=1. Note 
       that if cpgc engines 
       are not programmed exactly the same in both channel pairs, when write_spacing is 
       less than 3 
       there will be scenarios that this functionality will not work.
       //HSD272362
       if 1, LMN counters will not wrok in 2step mode when cpgc_lfsr_2code_mode=1 
       (legacy behaviour) 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDBCL_MUXCTL_MCDDC_DP_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CPGC_PATWDBCLMUX0_PBSTS_WR_MCDDC_DP_REG supported on:                      */
/*       IVT_EP (0x40186538)                                                  */
/*       IVT_EX (0x40186538)                                                  */
/*       HSX (0x401A2538)                                                     */
/*       BDX (0x401A2538)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_PATWDBCLMUX0_PBSTS_WR_MCDDC_DP_REG 0x0C014538
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * CPGC Pattern WDB Cache-Line Mux0 PB Status Write -
 * Status of the final Pattern Buffer/LFSR/MN following a test. Not valid during a 
 * test. 
 */
typedef union {
  struct {
    UINT32 patbuffer : 24;
    /* patbuffer - Bits[23:0], RO_V, default = 24'b000000000000000000000000 
       Current Status of the Pattern/LFSR Buffer in the Write domain.
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDBCLMUX0_PBSTS_WR_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CPGC_PATWDBCLMUX1_PBSTS_WR_MCDDC_DP_REG supported on:                      */
/*       IVT_EP (0x4018653C)                                                  */
/*       IVT_EX (0x4018653C)                                                  */
/*       HSX (0x401A253C)                                                     */
/*       BDX (0x401A253C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_PATWDBCLMUX1_PBSTS_WR_MCDDC_DP_REG 0x0C01453C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * CPGC Pattern WDB Cache-Line Mux1 PB Status Write -
 * Status of the final Pattern Buffer/LFSR/MN following a test. Not valid during a 
 * test. 
 */
typedef union {
  struct {
    UINT32 patbuffer : 24;
    /* patbuffer - Bits[23:0], RO_V, default = 24'b000000000000000000000000 
       Current Status of the Pattern/LFSR Buffer in the Write domain.
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDBCLMUX1_PBSTS_WR_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CPGC_PATWDBCLMUX2_PBSTS_WR_MCDDC_DP_REG supported on:                      */
/*       IVT_EP (0x40186540)                                                  */
/*       IVT_EX (0x40186540)                                                  */
/*       HSX (0x401A2540)                                                     */
/*       BDX (0x401A2540)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_PATWDBCLMUX2_PBSTS_WR_MCDDC_DP_REG 0x0C014540
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * CPGC Pattern WDB Cache-Line Mux2 PB Status Write -
 * Status of the final Pattern Buffer/LFSR/MN following a test. Not valid during a 
 * test. 
 */
typedef union {
  struct {
    UINT32 patbuffer : 24;
    /* patbuffer - Bits[23:0], RO_V, default = 24'b000000000000000000000000 
       Current Status of the Pattern/LFSR Buffer in the Write domain.
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDBCLMUX2_PBSTS_WR_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CPGC_PATWDBCLMUX0_PBSTS_RD_MCDDC_DP_REG supported on:                      */
/*       IVT_EP (0x40186544)                                                  */
/*       IVT_EX (0x40186544)                                                  */
/*       HSX (0x401A2544)                                                     */
/*       BDX (0x401A2544)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_PATWDBCLMUX0_PBSTS_RD_MCDDC_DP_REG 0x0C014544
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * CPGC Pattern WDB Cache-Line Mux0 PB Status Read -
 * Status of the final Pattern Buffer/LFSR/MN following a test. Not valid during a 
 * test. 
 */
typedef union {
  struct {
    UINT32 patbuffer : 24;
    /* patbuffer - Bits[23:0], RO_V, default = 24'b000000000000000000000000 
       Current Status of the Pattern/LFSR Buffer in the Read comparison domain.
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDBCLMUX0_PBSTS_RD_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CPGC_PATWDBCLMUX1_PBSTS_RD_MCDDC_DP_REG supported on:                      */
/*       IVT_EP (0x40186548)                                                  */
/*       IVT_EX (0x40186548)                                                  */
/*       HSX (0x401A2548)                                                     */
/*       BDX (0x401A2548)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_PATWDBCLMUX1_PBSTS_RD_MCDDC_DP_REG 0x0C014548
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * CPGC Pattern WDB Cache-Line Mux1 PB Status Read -
 * Status of the final Pattern Buffer/LFSR/MN following a test. Not valid during a 
 * test. 
 */
typedef union {
  struct {
    UINT32 patbuffer : 24;
    /* patbuffer - Bits[23:0], RO_V, default = 24'b000000000000000000000000 
       Current Status of the Pattern/LFSR Buffer in the Read comparison domain.
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDBCLMUX1_PBSTS_RD_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CPGC_PATWDBCLMUX2_PBSTS_RD_MCDDC_DP_REG supported on:                      */
/*       IVT_EP (0x4018654C)                                                  */
/*       IVT_EX (0x4018654C)                                                  */
/*       HSX (0x401A254C)                                                     */
/*       BDX (0x401A254C)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_PATWDBCLMUX2_PBSTS_RD_MCDDC_DP_REG 0x0C01454C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * CPGC Pattern WDB Cache-Line Mux2 PB Status Read -
 * Status of the final Pattern Buffer/LFSR/MN following a test. Not valid during a 
 * test. 
 */
typedef union {
  struct {
    UINT32 patbuffer : 24;
    /* patbuffer - Bits[23:0], RO_V, default = 24'b000000000000000000000000 
       Current Status of the Pattern/LFSR Buffer in the Read comparison domain.
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDBCLMUX2_PBSTS_RD_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* CPGC_PATWDB_WR0_MCDDC_DP_REG supported on:                                 */
/*       HSX (0x401A2550)                                                     */
/*       BDX (0x401A2550)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_PATWDB_WR0_MCDDC_DP_REG 0x0C014550

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Buffer for writing the lower 32 bits of 1/8 of a  cacheline of the WDB buffer.
 */
typedef union {
  struct {
    UINT32 wdb_data : 32;
    /* wdb_data - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Writes can only happen while in Loopback.Marker (Test Done is set) or 
       (Enable_WDB_Error_Capture is not set while in Loopback.Pattern). 
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDB_WR0_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CPGC_PATWDB_WR1_MCDDC_DP_REG supported on:                                 */
/*       HSX (0x401A2554)                                                     */
/*       BDX (0x401A2554)                                                     */
/* Register default value:              0x00000000                            */
#define CPGC_PATWDB_WR1_MCDDC_DP_REG 0x0C014554

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Buffer for writing the upper 32 bits of 1/8 of a  cacheline of the WDB buffer.
 */
typedef union {
  struct {
    UINT32 wdb_data : 32;
    /* wdb_data - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Writes can only happen while in Loopback.Marker (Test Done is set) or 
       (Enable_WDB_Error_Capture is not set while in Loopback.Pattern). 
     */
  } Bits;
  UINT32 Data;
} CPGC_PATWDB_WR1_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SDDC_CNTL_0_MCDDC_DP_REG supported on:                                     */
/*       HSX (0x401A2558)                                                     */
/*       BDX (0x401A2558)                                                     */
/* Register default value:              0x03FF03FF                            */
#define SDDC_CNTL_0_MCDDC_DP_REG 0x0C014558

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Bank SDDC Control. Used with Bank DDDC feature.
 */
typedef union {
  struct {
    UINT32 fd0 : 6;
    /* fd0 - Bits[5:0], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 0. Logged after corrected error.
     */
    UINT32 bank0 : 4;
    /* bank0 - Bits[9:6], RWS_LV, default = 4'b1111 
       Bank ID 0. This is shared with BDDDC logic. Logged after corrected error.
       DDR3: This register should be programmed with the Bank Address, range 7-0.
       DDR4: The Bank ID should be programmed as {Bank Address, Bank Group}
     */
    UINT32 bank0v : 1;
    /* bank0v - Bits[10:10], RWS_LV, default = 1'b0 
       Bank ID 0 is valid/enabled. Can get set/cleared on the fly by BIOS.
       Can be set by BIOS during BDDDC sparing, and HW will clear at end of sparing 
       event. 
     */
    UINT32 rsvd_11 : 5;
    /* rsvd_11 - Bits[15:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fd1 : 6;
    /* fd1 - Bits[21:16], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 1. Logged after corrected error.
     */
    UINT32 bank1 : 4;
    /* bank1 - Bits[25:22], RWS_LV, default = 4'b1111 
       Bank ID 1. This is shared with BDDDC logic. Logged after corrected error.
       DDR3: This register should be programmed with the Bank Address, range 7-0.
       DDR4: The Bank ID should be programmed as {Bank Address, Bank Group}
     */
    UINT32 bank1v : 1;
    /* bank1v - Bits[26:26], RWS_LV, default = 1'b0 
       Bank ID 1 is valid/enabled. Can get set/cleared on the fly by BIOS.
       Can be set by BIOS during BDDDC sparing, and HW will clear at end of sparing 
       event. 
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SDDC_CNTL_0_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SDDC_CNTL_1_MCDDC_DP_REG supported on:                                     */
/*       HSX (0x401A255C)                                                     */
/*       BDX (0x401A255C)                                                     */
/* Register default value:              0x03FF03FF                            */
#define SDDC_CNTL_1_MCDDC_DP_REG 0x0C01455C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Bank SDDC Control.  Used with Bank DDDC feature.
 */
typedef union {
  struct {
    UINT32 fd0 : 6;
    /* fd0 - Bits[5:0], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 0. Logged after corrected error.
     */
    UINT32 bank0 : 4;
    /* bank0 - Bits[9:6], RWS_LV, default = 4'b1111 
       Bank ID 0. This is shared with BDDDC logic. Logged after corrected error.
       DDR3: This register should be programmed with the Bank Address, range 7-0.
       DDR4: The Bank ID should be programmed as {Bank Address, Bank Group}
     */
    UINT32 bank0v : 1;
    /* bank0v - Bits[10:10], RWS_LV, default = 1'b0 
       Bank ID 0 is valid/enabled. Can get set/cleared on the fly by BIOS.
       Can be set by BIOS during BDDDC sparing, and HW will clear at end of sparing 
       event. 
     */
    UINT32 rsvd_11 : 5;
    /* rsvd_11 - Bits[15:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fd1 : 6;
    /* fd1 - Bits[21:16], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 1. Logged after corrected error.
     */
    UINT32 bank1 : 4;
    /* bank1 - Bits[25:22], RWS_LV, default = 4'b1111 
       Bank ID 1. This is shared with BDDDC logic. Logged after corrected error.
       DDR3: This register should be programmed with the Bank Address, range 7-0.
       DDR4: The Bank ID should be programmed as {Bank Address, Bank Group}
     */
    UINT32 bank1v : 1;
    /* bank1v - Bits[26:26], RWS_LV, default = 1'b0 
       Bank ID 1 is valid/enabled. Can get set/cleared on the fly by BIOS.
       Can be set by BIOS during BDDDC sparing, and HW will clear at end of sparing 
       event. 
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SDDC_CNTL_1_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SDDC_CNTL_2_MCDDC_DP_REG supported on:                                     */
/*       HSX (0x401A2560)                                                     */
/*       BDX (0x401A2560)                                                     */
/* Register default value:              0x03FF03FF                            */
#define SDDC_CNTL_2_MCDDC_DP_REG 0x0C014560

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Bank SDDC Control.  Used with Bank DDDC feature.
 */
typedef union {
  struct {
    UINT32 fd0 : 6;
    /* fd0 - Bits[5:0], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 0. Logged after corrected error.
     */
    UINT32 bank0 : 4;
    /* bank0 - Bits[9:6], RWS_LV, default = 4'b1111 
       Bank ID 0. This is shared with BDDDC logic. Logged after corrected error.
       DDR3: This register should be programmed with the Bank Address, range 7-0.
       DDR4: The Bank ID should be programmed as {Bank Address, Bank Group}
     */
    UINT32 bank0v : 1;
    /* bank0v - Bits[10:10], RWS_LV, default = 1'b0 
       Bank ID 0 is valid/enabled. Can get set/cleared on the fly by BIOS.
       Can be set by BIOS during BDDDC sparing, and HW will clear at end of sparing 
       event. 
     */
    UINT32 rsvd_11 : 5;
    /* rsvd_11 - Bits[15:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fd1 : 6;
    /* fd1 - Bits[21:16], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 1. Logged after corrected error.
     */
    UINT32 bank1 : 4;
    /* bank1 - Bits[25:22], RWS_LV, default = 4'b1111 
       Bank ID 1. This is shared with BDDDC logic. Logged after corrected error.
       DDR3: This register should be programmed with the Bank Address, range 7-0.
       DDR4: The Bank ID should be programmed as {Bank Address, Bank Group}
     */
    UINT32 bank1v : 1;
    /* bank1v - Bits[26:26], RWS_LV, default = 1'b0 
       Bank ID 1 is valid/enabled. Can get set/cleared on the fly by BIOS.
       Can be set by BIOS during BDDDC sparing, and HW will clear at end of sparing 
       event. 
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SDDC_CNTL_2_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SDDC_CNTL_3_MCDDC_DP_REG supported on:                                     */
/*       HSX (0x401A2564)                                                     */
/*       BDX (0x401A2564)                                                     */
/* Register default value:              0x03FF03FF                            */
#define SDDC_CNTL_3_MCDDC_DP_REG 0x0C014564

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Bank SDDC Control.  Used with Bank DDDC feature.
 */
typedef union {
  struct {
    UINT32 fd0 : 6;
    /* fd0 - Bits[5:0], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 0. Logged after corrected error.
     */
    UINT32 bank0 : 4;
    /* bank0 - Bits[9:6], RWS_LV, default = 4'b1111 
       Bank ID 0. This is shared with BDDDC logic. Logged after corrected error.
       DDR3: This register should be programmed with the Bank Address, range 7-0.
       DDR4: The Bank ID should be programmed as {Bank Address, Bank Group}
     */
    UINT32 bank0v : 1;
    /* bank0v - Bits[10:10], RWS_LV, default = 1'b0 
       Bank ID 0 is valid/enabled. Can get set/cleared on the fly by BIOS.
       Can be set by BIOS during BDDDC sparing, and HW will clear at end of sparing 
       event. 
     */
    UINT32 rsvd_11 : 5;
    /* rsvd_11 - Bits[15:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fd1 : 6;
    /* fd1 - Bits[21:16], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 1. Logged after corrected error.
     */
    UINT32 bank1 : 4;
    /* bank1 - Bits[25:22], RWS_LV, default = 4'b1111 
       Bank ID 1. This is shared with BDDDC logic. Logged after corrected error.
       DDR3: This register should be programmed with the Bank Address, range 7-0.
       DDR4: The Bank ID should be programmed as {Bank Address, Bank Group}
     */
    UINT32 bank1v : 1;
    /* bank1v - Bits[26:26], RWS_LV, default = 1'b0 
       Bank ID 1 is valid/enabled. Can get set/cleared on the fly by BIOS.
       Can be set by BIOS during BDDDC sparing, and HW will clear at end of sparing 
       event. 
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SDDC_CNTL_3_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SDDC_CNTL_4_MCDDC_DP_REG supported on:                                     */
/*       HSX (0x401A2568)                                                     */
/*       BDX (0x401A2568)                                                     */
/* Register default value:              0x03FF03FF                            */
#define SDDC_CNTL_4_MCDDC_DP_REG 0x0C014568

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Bank SDDC Control  Used with Bank DDDC feature..
 */
typedef union {
  struct {
    UINT32 fd0 : 6;
    /* fd0 - Bits[5:0], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 0. Logged after corrected error.
     */
    UINT32 bank0 : 4;
    /* bank0 - Bits[9:6], RWS_LV, default = 4'b1111 
       Bank ID 0. This is shared with BDDDC logic. Logged after corrected error.
       DDR3: This register should be programmed with the Bank Address, range 7-0.
       DDR4: The Bank ID should be programmed as {Bank Address, Bank Group}
     */
    UINT32 bank0v : 1;
    /* bank0v - Bits[10:10], RWS_LV, default = 1'b0 
       Bank ID 0 is valid/enabled. Can get set/cleared on the fly by BIOS.
       Can be set by BIOS during BDDDC sparing, and HW will clear at end of sparing 
       event. 
     */
    UINT32 rsvd_11 : 5;
    /* rsvd_11 - Bits[15:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fd1 : 6;
    /* fd1 - Bits[21:16], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 1. Logged after corrected error.
     */
    UINT32 bank1 : 4;
    /* bank1 - Bits[25:22], RWS_LV, default = 4'b1111 
       Bank ID 1. This is shared with BDDDC logic. Logged after corrected error.
       DDR3: This register should be programmed with the Bank Address, range 7-0.
       DDR4: The Bank ID should be programmed as {Bank Address, Bank Group}
     */
    UINT32 bank1v : 1;
    /* bank1v - Bits[26:26], RWS_LV, default = 1'b0 
       Bank ID 1 is valid/enabled. Can get set/cleared on the fly by BIOS.
       Can be set by BIOS during BDDDC sparing, and HW will clear at end of sparing 
       event. 
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SDDC_CNTL_4_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SDDC_CNTL_5_MCDDC_DP_REG supported on:                                     */
/*       HSX (0x401A256C)                                                     */
/*       BDX (0x401A256C)                                                     */
/* Register default value:              0x03FF03FF                            */
#define SDDC_CNTL_5_MCDDC_DP_REG 0x0C01456C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Bank SDDC Control.  Used with Bank DDDC feature.
 */
typedef union {
  struct {
    UINT32 fd0 : 6;
    /* fd0 - Bits[5:0], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 0. Logged after corrected error.
     */
    UINT32 bank0 : 4;
    /* bank0 - Bits[9:6], RWS_LV, default = 4'b1111 
       Bank ID 0. This is shared with BDDDC logic. Logged after corrected error.
       DDR3: This register should be programmed with the Bank Address, range 7-0.
       DDR4: The Bank ID should be programmed as {Bank Address, Bank Group}
     */
    UINT32 bank0v : 1;
    /* bank0v - Bits[10:10], RWS_LV, default = 1'b0 
       Bank ID 0 is valid/enabled. Can get set/cleared on the fly by BIOS.
       Can be set by BIOS during BDDDC sparing, and HW will clear at end of sparing 
       event. 
     */
    UINT32 rsvd_11 : 5;
    /* rsvd_11 - Bits[15:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fd1 : 6;
    /* fd1 - Bits[21:16], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 1. Logged after corrected error.
     */
    UINT32 bank1 : 4;
    /* bank1 - Bits[25:22], RWS_LV, default = 4'b1111 
       Bank ID 1. This is shared with BDDDC logic. Logged after corrected error.
       DDR3: This register should be programmed with the Bank Address, range 7-0.
       DDR4: The Bank ID should be programmed as {Bank Address, Bank Group}
     */
    UINT32 bank1v : 1;
    /* bank1v - Bits[26:26], RWS_LV, default = 1'b0 
       Bank ID 1 is valid/enabled. Can get set/cleared on the fly by BIOS.
       Can be set by BIOS during BDDDC sparing, and HW will clear at end of sparing 
       event. 
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SDDC_CNTL_5_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SDDC_CNTL_6_MCDDC_DP_REG supported on:                                     */
/*       HSX (0x401A2570)                                                     */
/*       BDX (0x401A2570)                                                     */
/* Register default value:              0x03FF03FF                            */
#define SDDC_CNTL_6_MCDDC_DP_REG 0x0C014570

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Bank SDDC Control.  Used with Bank DDDC feature.
 */
typedef union {
  struct {
    UINT32 fd0 : 6;
    /* fd0 - Bits[5:0], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 0. Logged after corrected error.
     */
    UINT32 bank0 : 4;
    /* bank0 - Bits[9:6], RWS_LV, default = 4'b1111 
       Bank ID 0. This is shared with BDDDC logic. Logged after corrected error.
       DDR3: This register should be programmed with the Bank Address, range 7-0.
       DDR4: The Bank ID should be programmed as {Bank Address, Bank Group}
     */
    UINT32 bank0v : 1;
    /* bank0v - Bits[10:10], RWS_LV, default = 1'b0 
       Bank ID 0 is valid/enabled. Can get set/cleared on the fly by BIOS.
       Can be set by BIOS during BDDDC sparing, and HW will clear at end of sparing 
       event. 
     */
    UINT32 rsvd_11 : 5;
    /* rsvd_11 - Bits[15:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fd1 : 6;
    /* fd1 - Bits[21:16], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 1. Logged after corrected error.
     */
    UINT32 bank1 : 4;
    /* bank1 - Bits[25:22], RWS_LV, default = 4'b1111 
       Bank ID 1. This is shared with BDDDC logic. Logged after corrected error.
       DDR3: This register should be programmed with the Bank Address, range 7-0.
       DDR4: The Bank ID should be programmed as {Bank Address, Bank Group}
     */
    UINT32 bank1v : 1;
    /* bank1v - Bits[26:26], RWS_LV, default = 1'b0 
       Bank ID 1 is valid/enabled. Can get set/cleared on the fly by BIOS.
       Can be set by BIOS during BDDDC sparing, and HW will clear at end of sparing 
       event. 
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SDDC_CNTL_6_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SDDC_CNTL_7_MCDDC_DP_REG supported on:                                     */
/*       HSX (0x401A2574)                                                     */
/*       BDX (0x401A2574)                                                     */
/* Register default value:              0x03FF03FF                            */
#define SDDC_CNTL_7_MCDDC_DP_REG 0x0C014574

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Bank SDDC Control.  Used with Bank DDDC feature.
 */
typedef union {
  struct {
    UINT32 fd0 : 6;
    /* fd0 - Bits[5:0], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 0. Logged after corrected error.
     */
    UINT32 bank0 : 4;
    /* bank0 - Bits[9:6], RWS_LV, default = 4'b1111 
       Bank ID 0. This is shared with BDDDC logic. Logged after corrected error.
       DDR3: This register should be programmed with the Bank Address, range 7-0.
       DDR4: The Bank ID should be programmed as {Bank Address, Bank Group}
     */
    UINT32 bank0v : 1;
    /* bank0v - Bits[10:10], RWS_LV, default = 1'b0 
       Bank ID 0 is valid/enabled. Can get set/cleared on the fly by BIOS.
       Can be set by BIOS during BDDDC sparing, and HW will clear at end of sparing 
       event. 
     */
    UINT32 rsvd_11 : 5;
    /* rsvd_11 - Bits[15:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fd1 : 6;
    /* fd1 - Bits[21:16], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 1. Logged after corrected error.
     */
    UINT32 bank1 : 4;
    /* bank1 - Bits[25:22], RWS_LV, default = 4'b1111 
       Bank ID 1. This is shared with BDDDC logic. Logged after corrected error.
       DDR3: This register should be programmed with the Bank Address, range 7-0.
       DDR4: The Bank ID should be programmed as {Bank Address, Bank Group}
     */
    UINT32 bank1v : 1;
    /* bank1v - Bits[26:26], RWS_LV, default = 1'b0 
       Bank ID 1 is valid/enabled. Can get set/cleared on the fly by BIOS.
       Can be set by BIOS during BDDDC sparing, and HW will clear at end of sparing 
       event. 
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SDDC_CNTL_7_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* BDDDC_CNTL_0_MCDDC_DP_REG supported on:                                    */
/*       HSX (0x401A2578)                                                     */
/*       BDX (0x401A2578)                                                     */
/* Register default value:              0x003F003F                            */
#define BDDDC_CNTL_0_MCDDC_DP_REG 0x0C014578

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Bank DDDC Control.  Corresponding Bank IDs are in sddc_cntl_<rank>.
 */
typedef union {
  struct {
    UINT32 fd0 : 6;
    /* fd0 - Bits[5:0], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 0.
     */
    UINT32 bank0v : 1;
    /* bank0v - Bits[6:6], RWS_LV, default = 1'b0 
       Bank ID 0 is DDDC valid/enabled.
     */
    UINT32 rsvd_7 : 9;
    /* rsvd_7 - Bits[15:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fd1 : 6;
    /* fd1 - Bits[21:16], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 1.
     */
    UINT32 bank1v : 1;
    /* bank1v - Bits[22:22], RWS_LV, default = 1'b0 
       Bank ID 1 is DDDC valid/enabled. This will be set by HW at end of sparing.
     */
    UINT32 rsvd_23 : 2;
    /* rsvd_23 - Bits[24:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_25 : 3;
    UINT32 bank0sp : 1;
    /* bank0sp - Bits[28:28], RW_LBV, default = 1'b0 
       Bank ID 0 is sparing.
     */
    UINT32 bank1sp : 1;
    /* bank1sp - Bits[29:29], RW_LBV, default = 1'b0 
       Bank ID 1 is sparing. Set before starting sparing FSM. This is only used to set 
       the enable at end of sparing. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} BDDDC_CNTL_0_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* BDDDC_CNTL_1_MCDDC_DP_REG supported on:                                    */
/*       HSX (0x401A257C)                                                     */
/*       BDX (0x401A257C)                                                     */
/* Register default value:              0x003F003F                            */
#define BDDDC_CNTL_1_MCDDC_DP_REG 0x0C01457C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Bank DDDC Control.  Corresponding Bank IDs are in sddc_cntl_<rank>.
 */
typedef union {
  struct {
    UINT32 fd0 : 6;
    /* fd0 - Bits[5:0], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 0.
     */
    UINT32 bank0v : 1;
    /* bank0v - Bits[6:6], RWS_LV, default = 1'b0 
       Bank ID 0 is DDDC valid/enabled.
     */
    UINT32 rsvd_7 : 9;
    /* rsvd_7 - Bits[15:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fd1 : 6;
    /* fd1 - Bits[21:16], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 1.
     */
    UINT32 bank1v : 1;
    /* bank1v - Bits[22:22], RWS_LV, default = 1'b0 
       Bank ID 1 is DDDC valid/enabled. This will be set by HW at end of sparing.
     */
    UINT32 rsvd_23 : 2;
    /* rsvd_23 - Bits[24:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_25 : 3;
    UINT32 bank0sp : 1;
    /* bank0sp - Bits[28:28], RW_LBV, default = 1'b0 
       Bank ID 0 is sparing.
     */
    UINT32 bank1sp : 1;
    /* bank1sp - Bits[29:29], RW_LBV, default = 1'b0 
       Bank ID 1 is sparing. Set before starting sparing FSM. This is only used to set 
       the enable at end of sparing. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} BDDDC_CNTL_1_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* BDDDC_CNTL_2_MCDDC_DP_REG supported on:                                    */
/*       HSX (0x401A2580)                                                     */
/*       BDX (0x401A2580)                                                     */
/* Register default value:              0x003F003F                            */
#define BDDDC_CNTL_2_MCDDC_DP_REG 0x0C014580

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Bank DDDC Control.  Corresponding Bank IDs are in sddc_cntl_<rank>.
 */
typedef union {
  struct {
    UINT32 fd0 : 6;
    /* fd0 - Bits[5:0], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 0.
     */
    UINT32 bank0v : 1;
    /* bank0v - Bits[6:6], RWS_LV, default = 1'b0 
       Bank ID 0 is DDDC valid/enabled.
     */
    UINT32 rsvd_7 : 9;
    /* rsvd_7 - Bits[15:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fd1 : 6;
    /* fd1 - Bits[21:16], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 1.
     */
    UINT32 bank1v : 1;
    /* bank1v - Bits[22:22], RWS_LV, default = 1'b0 
       Bank ID 1 is DDDC valid/enabled. This will be set by HW at end of sparing.
     */
    UINT32 rsvd_23 : 2;
    /* rsvd_23 - Bits[24:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_25 : 3;
    UINT32 bank0sp : 1;
    /* bank0sp - Bits[28:28], RW_LBV, default = 1'b0 
       Bank ID 0 is sparing.
     */
    UINT32 bank1sp : 1;
    /* bank1sp - Bits[29:29], RW_LBV, default = 1'b0 
       Bank ID 1 is sparing. Set before starting sparing FSM. This is only used to set 
       the enable at end of sparing. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} BDDDC_CNTL_2_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* BDDDC_CNTL_3_MCDDC_DP_REG supported on:                                    */
/*       HSX (0x401A2584)                                                     */
/*       BDX (0x401A2584)                                                     */
/* Register default value:              0x003F003F                            */
#define BDDDC_CNTL_3_MCDDC_DP_REG 0x0C014584

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Bank DDDC Control.  Corresponding Bank IDs are in sddc_cntl_<rank>.
 */
typedef union {
  struct {
    UINT32 fd0 : 6;
    /* fd0 - Bits[5:0], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 0.
     */
    UINT32 bank0v : 1;
    /* bank0v - Bits[6:6], RWS_LV, default = 1'b0 
       Bank ID 0 is DDDC valid/enabled.
     */
    UINT32 rsvd_7 : 9;
    /* rsvd_7 - Bits[15:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fd1 : 6;
    /* fd1 - Bits[21:16], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 1.
     */
    UINT32 bank1v : 1;
    /* bank1v - Bits[22:22], RWS_LV, default = 1'b0 
       Bank ID 1 is DDDC valid/enabled. This will be set by HW at end of sparing.
     */
    UINT32 rsvd_23 : 2;
    /* rsvd_23 - Bits[24:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_25 : 3;
    UINT32 bank0sp : 1;
    /* bank0sp - Bits[28:28], RW_LBV, default = 1'b0 
       Bank ID 0 is sparing.
     */
    UINT32 bank1sp : 1;
    /* bank1sp - Bits[29:29], RW_LBV, default = 1'b0 
       Bank ID 1 is sparing. Set before starting sparing FSM. This is only used to set 
       the enable at end of sparing. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} BDDDC_CNTL_3_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* BDDDC_CNTL_4_MCDDC_DP_REG supported on:                                    */
/*       HSX (0x401A2588)                                                     */
/*       BDX (0x401A2588)                                                     */
/* Register default value:              0x003F003F                            */
#define BDDDC_CNTL_4_MCDDC_DP_REG 0x0C014588

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Bank DDDC Control.  Corresponding Bank IDs are in sddc_cntl_<rank>.
 */
typedef union {
  struct {
    UINT32 fd0 : 6;
    /* fd0 - Bits[5:0], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 0.
     */
    UINT32 bank0v : 1;
    /* bank0v - Bits[6:6], RWS_LV, default = 1'b0 
       Bank ID 0 is DDDC valid/enabled.
     */
    UINT32 rsvd_7 : 9;
    /* rsvd_7 - Bits[15:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fd1 : 6;
    /* fd1 - Bits[21:16], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 1.
     */
    UINT32 bank1v : 1;
    /* bank1v - Bits[22:22], RWS_LV, default = 1'b0 
       Bank ID 1 is DDDC valid/enabled. This will be set by HW at end of sparing.
     */
    UINT32 rsvd_23 : 2;
    /* rsvd_23 - Bits[24:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_25 : 3;
    UINT32 bank0sp : 1;
    /* bank0sp - Bits[28:28], RW_LBV, default = 1'b0 
       Bank ID 0 is sparing.
     */
    UINT32 bank1sp : 1;
    /* bank1sp - Bits[29:29], RW_LBV, default = 1'b0 
       Bank ID 1 is sparing. Set before starting sparing FSM. This is only used to set 
       the enable at end of sparing. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} BDDDC_CNTL_4_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* BDDDC_CNTL_5_MCDDC_DP_REG supported on:                                    */
/*       HSX (0x401A258C)                                                     */
/*       BDX (0x401A258C)                                                     */
/* Register default value:              0x003F003F                            */
#define BDDDC_CNTL_5_MCDDC_DP_REG 0x0C01458C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Bank DDDC Control.  Corresponding Bank IDs are in sddc_cntl_<rank>.
 */
typedef union {
  struct {
    UINT32 fd0 : 6;
    /* fd0 - Bits[5:0], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 0.
     */
    UINT32 bank0v : 1;
    /* bank0v - Bits[6:6], RWS_LV, default = 1'b0 
       Bank ID 0 is DDDC valid/enabled.
     */
    UINT32 rsvd_7 : 9;
    /* rsvd_7 - Bits[15:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fd1 : 6;
    /* fd1 - Bits[21:16], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 1.
     */
    UINT32 bank1v : 1;
    /* bank1v - Bits[22:22], RWS_LV, default = 1'b0 
       Bank ID 1 is DDDC valid/enabled. This will be set by HW at end of sparing.
     */
    UINT32 rsvd_23 : 2;
    /* rsvd_23 - Bits[24:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_25 : 3;
    UINT32 bank0sp : 1;
    /* bank0sp - Bits[28:28], RW_LBV, default = 1'b0 
       Bank ID 0 is sparing.
     */
    UINT32 bank1sp : 1;
    /* bank1sp - Bits[29:29], RW_LBV, default = 1'b0 
       Bank ID 1 is sparing. Set before starting sparing FSM. This is only used to set 
       the enable at end of sparing. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} BDDDC_CNTL_5_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* BDDDC_CNTL_6_MCDDC_DP_REG supported on:                                    */
/*       HSX (0x401A2590)                                                     */
/*       BDX (0x401A2590)                                                     */
/* Register default value:              0x003F003F                            */
#define BDDDC_CNTL_6_MCDDC_DP_REG 0x0C014590

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Bank DDDC Control.  Corresponding Bank IDs are in sddc_cntl_<rank>.
 */
typedef union {
  struct {
    UINT32 fd0 : 6;
    /* fd0 - Bits[5:0], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 0.
     */
    UINT32 bank0v : 1;
    /* bank0v - Bits[6:6], RWS_LV, default = 1'b0 
       Bank ID 0 is DDDC valid/enabled.
     */
    UINT32 rsvd_7 : 9;
    /* rsvd_7 - Bits[15:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fd1 : 6;
    /* fd1 - Bits[21:16], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 1.
     */
    UINT32 bank1v : 1;
    /* bank1v - Bits[22:22], RWS_LV, default = 1'b0 
       Bank ID 1 is DDDC valid/enabled. This will be set by HW at end of sparing.
     */
    UINT32 rsvd_23 : 2;
    /* rsvd_23 - Bits[24:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_25 : 3;
    UINT32 bank0sp : 1;
    /* bank0sp - Bits[28:28], RW_LBV, default = 1'b0 
       Bank ID 0 is sparing.
     */
    UINT32 bank1sp : 1;
    /* bank1sp - Bits[29:29], RW_LBV, default = 1'b0 
       Bank ID 1 is sparing. Set before starting sparing FSM. This is only used to set 
       the enable at end of sparing. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} BDDDC_CNTL_6_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* BDDDC_CNTL_7_MCDDC_DP_REG supported on:                                    */
/*       HSX (0x401A2594)                                                     */
/*       BDX (0x401A2594)                                                     */
/* Register default value:              0x003F003F                            */
#define BDDDC_CNTL_7_MCDDC_DP_REG 0x0C014594

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Bank DDDC Control.  Corresponding Bank IDs are in sddc_cntl_<rank>.
 */
typedef union {
  struct {
    UINT32 fd0 : 6;
    /* fd0 - Bits[5:0], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 0.
     */
    UINT32 bank0v : 1;
    /* bank0v - Bits[6:6], RWS_LV, default = 1'b0 
       Bank ID 0 is DDDC valid/enabled.
     */
    UINT32 rsvd_7 : 9;
    /* rsvd_7 - Bits[15:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fd1 : 6;
    /* fd1 - Bits[21:16], RWS_LV, default = 6'b111111 
       Fail Device ID for Bank ID 1.
     */
    UINT32 bank1v : 1;
    /* bank1v - Bits[22:22], RWS_LV, default = 1'b0 
       Bank ID 1 is DDDC valid/enabled. This will be set by HW at end of sparing.
     */
    UINT32 rsvd_23 : 2;
    /* rsvd_23 - Bits[24:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_25 : 3;
    UINT32 bank0sp : 1;
    /* bank0sp - Bits[28:28], RW_LBV, default = 1'b0 
       Bank ID 0 is sparing.
     */
    UINT32 bank1sp : 1;
    /* bank1sp - Bits[29:29], RW_LBV, default = 1'b0 
       Bank ID 1 is sparing. Set before starting sparing FSM. This is only used to set 
       the enable at end of sparing. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} BDDDC_CNTL_7_MCDDC_DP_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






/* DDDC_CNTL_0_MCDDC_DP_REG supported on:                                     */
/*       IVT_EP (0x10186608)                                                  */
/*       IVT_EX (0x10186608)                                                  */
/*       HSX (0x101A2608)                                                     */
/*       BDX (0x101A2608)                                                     */
/* Register default value:              0x3F                                  */
#define DDDC_CNTL_0_MCDDC_DP_REG 0x0C011608
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * DDDC Control Register for Intel SMI 2 1:1x4 Channels 0/2 (EX processor only).
 * Usage model - BIOS may only enable DDDC when operating in Intel SMI 2 1:1x4 
 * mode.  The first device failure will enable DDDC, the second device failure may 
 * enable SDDC. 
 */
typedef union {
  struct {
    UINT8 faildevice : 6;
    /* faildevice - Bits[5:0], RWS_V, default = 6'b111111 
       Hardware will capture the fail device ID on a correctable error until EN field 
       has been set, after which hardware may no longer update. Any further fail device 
       ID will be captured in DEVTAG_CNTL. Valid range is 0-35 decimal. Above 35 means 
       no device failure has occurred on this rank. 
       Device ID mapping:
       35: Parity
       34: Spare
       33-18: Data
       17: CRC
       16: CRC
       15-0: Data
     */
    UINT8 sparing : 1;
    /* sparing - Bits[6:6], RWS_L, default = 1'b0 
       This field will be set by BIOS before the DDDC sparing flow begins to indicate 
       that this rank is currently in the sparing flow. The field is cleared by 
       hardware at the completion of DDDC sparing. SPARING and EN shall be mutually 
       exclusive. 
     */
    UINT8 en : 1;
    /* en - Bits[7:7], RWS_L, default = 1'b0 
       DDDC enable for this rank. When set, this rank will use the device mapping 
       specified by FAILDEVICE. This field is written by hardware at the completion of 
       DDDC sparing. SPARING and EN shall be mutually exclusive. 
     */
  } Bits;
  UINT8 Data;
} DDDC_CNTL_0_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* DDDC_CNTL_1_MCDDC_DP_REG supported on:                                     */
/*       IVT_EP (0x10186609)                                                  */
/*       IVT_EX (0x10186609)                                                  */
/*       HSX (0x101A2609)                                                     */
/*       BDX (0x101A2609)                                                     */
/* Register default value:              0x3F                                  */
#define DDDC_CNTL_1_MCDDC_DP_REG 0x0C011609
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * See DDDC_CNTL_0 for description.
 */
typedef union {
  struct {
    UINT8 faildevice : 6;
    /* faildevice - Bits[5:0], RWS_V, default = 6'b111111 
       See DDDC_CNTL_0 for description.
     */
    UINT8 sparing : 1;
    /* sparing - Bits[6:6], RWS_L, default = 1'b0 
       See DDDC_CNTL_0 for description.
     */
    UINT8 en : 1;
    /* en - Bits[7:7], RWS_L, default = 1'b0 
       See DDDC_CNTL_0 for description.
     */
  } Bits;
  UINT8 Data;
} DDDC_CNTL_1_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* DDDC_CNTL_2_MCDDC_DP_REG supported on:                                     */
/*       IVT_EP (0x1018660A)                                                  */
/*       IVT_EX (0x1018660A)                                                  */
/*       HSX (0x101A260A)                                                     */
/*       BDX (0x101A260A)                                                     */
/* Register default value:              0x3F                                  */
#define DDDC_CNTL_2_MCDDC_DP_REG 0x0C01160A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * See DDDC_CNTL_0 for description.
 */
typedef union {
  struct {
    UINT8 faildevice : 6;
    /* faildevice - Bits[5:0], RWS_V, default = 6'b111111 
       See DDDC_CNTL_0 for description.
     */
    UINT8 sparing : 1;
    /* sparing - Bits[6:6], RWS_L, default = 1'b0 
       See DDDC_CNTL_0 for description.
     */
    UINT8 en : 1;
    /* en - Bits[7:7], RWS_L, default = 1'b0 
       See DDDC_CNTL_0 for description.
     */
  } Bits;
  UINT8 Data;
} DDDC_CNTL_2_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* DDDC_CNTL_3_MCDDC_DP_REG supported on:                                     */
/*       IVT_EP (0x1018660B)                                                  */
/*       IVT_EX (0x1018660B)                                                  */
/*       HSX (0x101A260B)                                                     */
/*       BDX (0x101A260B)                                                     */
/* Register default value:              0x3F                                  */
#define DDDC_CNTL_3_MCDDC_DP_REG 0x0C01160B
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * See DDDC_CNTL_0 for description.
 */
typedef union {
  struct {
    UINT8 faildevice : 6;
    /* faildevice - Bits[5:0], RWS_V, default = 6'b111111 
       See DDDC_CNTL_0 for description.
     */
    UINT8 sparing : 1;
    /* sparing - Bits[6:6], RWS_L, default = 1'b0 
       See DDDC_CNTL_0 for description.
     */
    UINT8 en : 1;
    /* en - Bits[7:7], RWS_L, default = 1'b0 
       See DDDC_CNTL_0 for description.
     */
  } Bits;
  UINT8 Data;
} DDDC_CNTL_3_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* DDDC_CNTL_4_MCDDC_DP_REG supported on:                                     */
/*       IVT_EP (0x1018660C)                                                  */
/*       IVT_EX (0x1018660C)                                                  */
/*       HSX (0x101A260C)                                                     */
/*       BDX (0x101A260C)                                                     */
/* Register default value:              0x3F                                  */
#define DDDC_CNTL_4_MCDDC_DP_REG 0x0C01160C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * See DDDC_CNTL_0 for description.
 */
typedef union {
  struct {
    UINT8 faildevice : 6;
    /* faildevice - Bits[5:0], RWS_V, default = 6'b111111 
       See DDDC_CNTL_0 for description.
     */
    UINT8 sparing : 1;
    /* sparing - Bits[6:6], RWS_L, default = 1'b0 
       See DDDC_CNTL_0 for description.
     */
    UINT8 en : 1;
    /* en - Bits[7:7], RWS_L, default = 1'b0 
       See DDDC_CNTL_0 for description.
     */
  } Bits;
  UINT8 Data;
} DDDC_CNTL_4_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* DDDC_CNTL_5_MCDDC_DP_REG supported on:                                     */
/*       IVT_EP (0x1018660D)                                                  */
/*       IVT_EX (0x1018660D)                                                  */
/*       HSX (0x101A260D)                                                     */
/*       BDX (0x101A260D)                                                     */
/* Register default value:              0x3F                                  */
#define DDDC_CNTL_5_MCDDC_DP_REG 0x0C01160D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * See DDDC_CNTL_0 for description.
 */
typedef union {
  struct {
    UINT8 faildevice : 6;
    /* faildevice - Bits[5:0], RWS_V, default = 6'b111111 
       See DDDC_CNTL_0 for description.
     */
    UINT8 sparing : 1;
    /* sparing - Bits[6:6], RWS_L, default = 1'b0 
       See DDDC_CNTL_0 for description.
     */
    UINT8 en : 1;
    /* en - Bits[7:7], RWS_L, default = 1'b0 
       See DDDC_CNTL_0 for description.
     */
  } Bits;
  UINT8 Data;
} DDDC_CNTL_5_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* DDDC_CNTL_6_MCDDC_DP_REG supported on:                                     */
/*       IVT_EP (0x1018660E)                                                  */
/*       IVT_EX (0x1018660E)                                                  */
/*       HSX (0x101A260E)                                                     */
/*       BDX (0x101A260E)                                                     */
/* Register default value:              0x3F                                  */
#define DDDC_CNTL_6_MCDDC_DP_REG 0x0C01160E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * See DDDC_CNTL_0 for description.
 */
typedef union {
  struct {
    UINT8 faildevice : 6;
    /* faildevice - Bits[5:0], RWS_V, default = 6'b111111 
       See DDDC_CNTL_0 for description.
     */
    UINT8 sparing : 1;
    /* sparing - Bits[6:6], RWS_L, default = 1'b0 
       See DDDC_CNTL_0 for description.
     */
    UINT8 en : 1;
    /* en - Bits[7:7], RWS_L, default = 1'b0 
       See DDDC_CNTL_0 for description.
     */
  } Bits;
  UINT8 Data;
} DDDC_CNTL_6_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* DDDC_CNTL_7_MCDDC_DP_REG supported on:                                     */
/*       IVT_EP (0x1018660F)                                                  */
/*       IVT_EX (0x1018660F)                                                  */
/*       HSX (0x101A260F)                                                     */
/*       BDX (0x101A260F)                                                     */
/* Register default value:              0x3F                                  */
#define DDDC_CNTL_7_MCDDC_DP_REG 0x0C01160F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * See DDDC_CNTL_0 for description.
 */
typedef union {
  struct {
    UINT8 faildevice : 6;
    /* faildevice - Bits[5:0], RWS_V, default = 6'b111111 
       See DDDC_CNTL_0 for description.
     */
    UINT8 sparing : 1;
    /* sparing - Bits[6:6], RWS_L, default = 1'b0 
       See DDDC_CNTL_0 for description.
     */
    UINT8 en : 1;
    /* en - Bits[7:7], RWS_L, default = 1'b0 
       See DDDC_CNTL_0 for description.
     */
  } Bits;
  UINT8 Data;
} DDDC_CNTL_7_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_PC6_CNTL_0_MCDDC_DP_REG supported on:                                 */
/*       IVT_EP (0x40186620)                                                  */
/*       IVT_EX (0x40186620)                                                  */
/*       HSX (0x401A2620)                                                     */
/*       BDX (0x401A2620)                                                     */
/* Register default value:              0xC42800FF                            */
#define VMSE_PC6_CNTL_0_MCDDC_DP_REG 0x0C014620
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * This register holds the control bits and the timing parameters for Intel SMI 2 
 * Clock Stop function. (EX processor only) 
 */
typedef union {
  struct {
    UINT32 self_refresh_enter : 9;
    /* self_refresh_enter - Bits[8:0], RWS_L, default = 9'b011111111 
       Minimum duration to wait for scalable memory buffer to put DRAM into self 
       refresh during scalable memory buffers clock stop enter process; in unit of dclk 
     */
    UINT32 rsvd_9 : 3;
    /* rsvd_9 - Bits[11:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pc6_inj_clksync : 1;
    /* pc6_inj_clksync - Bits[12:12], RW_LB, default = 1'b0 
       Set this bit to force the PC6 MXB Clock Stop FSM to inject a Clock Sync command 
       during clock stop 
       exit. This bit is primarily meant to be used for debug and should not be enabled 
       when Half Width Mode or 
       Quarter Rate Clock Mode is enabled
     */
    UINT32 rsvd_13 : 3;
    /* rsvd_13 - Bits[15:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 sync_wait : 9;
    /* sync_wait - Bits[24:16], RW_LB, default = 9'b000101000 
       Time to wait after issuing sync command and before issuing scrambler sync 
       command during scalable memory buffers clock stop exit process; in unit of dclk 
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pc6_dis_clk : 1;
    /* pc6_dis_clk - Bits[26:26], RW_LB, default = 1'b1 
       When this bit is set to 0, during Pkg C6, output clock is not disabled.
     */
    UINT32 rsvd_27 : 1;
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 enter_pc6_cfg : 2;
    /* enter_pc6_cfg - Bits[30:29], RWS_L, default = 2'b10 
       Criteria for entering Clock Stop mode.
       Expectation is that this will always be 2.
     */
    UINT32 disble_vmse_pc6 : 1;
    /* disble_vmse_pc6 - Bits[31:31], RWS_L, default = 1'b1 
       Disable Clock Stop function during PC6
     */
  } Bits;
  UINT32 Data;
} VMSE_PC6_CNTL_0_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_PC6_CNTL_1_MCDDC_DP_REG supported on:                                 */
/*       IVT_EP (0x40186624)                                                  */
/*       IVT_EX (0x40186624)                                                  */
/*       HSX (0x401A2624)                                                     */
/*       BDX (0x401A2624)                                                     */
/* Register default value:              0x002800FF                            */
#define VMSE_PC6_CNTL_1_MCDDC_DP_REG 0x0C014624
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * This register holds the timing parameters for Intel SMI 2 Clock Stop function. 
 * (EX processor only) 
 */
typedef union {
  struct {
    UINT32 clk_stp_nop_duration : 9;
    /* clk_stp_nop_duration - Bits[8:0], RWS_L, default = 9'b011111111 
       Duration to issue NOPs, after issue Clock Stop command before disable scrambler 
       during scalable memory buffers clock stop enter process; in unit of dclk 
     */
    UINT32 rsvd_9 : 7;
    /* rsvd_9 - Bits[15:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 clk_stp_duration : 9;
    /* clk_stp_duration - Bits[24:16], RWS_L, default = 9'b000101000 
       Minimum duration for scalable memory buffer to enter Clock Stop; this counting 
       starts after all output buffers are disabled; in unit of dclk 
     */
    UINT32 rsvd_25 : 7;
    /* rsvd_25 - Bits[31:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} VMSE_PC6_CNTL_1_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_PC6_CNTL_2_MCDDC_DP_REG supported on:                                 */
/*       IVT_EP (0x40186628)                                                  */
/*       IVT_EX (0x40186628)                                                  */
/*       HSX (0x401A2628)                                                     */
/*       BDX (0x401A2628)                                                     */
/* Register default value:              0x0FF7FCFF                            */
#define VMSE_PC6_CNTL_2_MCDDC_DP_REG 0x0C014628
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * This register holds the timing parameters for Intel SMI 2 Clock Stop function. 
 * (EX processor only) 
 */
typedef union {
  struct {
    UINT32 en_dll_duration : 9;
    /* en_dll_duration - Bits[8:0], RWS_L, default = 9'b011111111 
       Duration to wait for DLL/PLL to be enabled before exit Clock Stop during 
       scalable memory buffers clock stop exit process; in unit of dclk 
     */
    UINT32 rsvd_9 : 1;
    /* rsvd_9 - Bits[9:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 scr_sync_resp : 9;
    /* scr_sync_resp - Bits[18:10], RWS_L, default = 9'b111111111 
       Time to wait for scrambler sync response starting from issuing a scrambler sync 
       command during scalable memory buffers clock stop exit process; in unit of dclk 
     */
    UINT32 rsvd_19 : 1;
    /* rsvd_19 - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 dis_dll_duration : 9;
    /* dis_dll_duration - Bits[28:20], RWS_L, default = 9'b011111111 
       Duration to wait for DLL/PLL to be disabled after scalable memory buffer enters 
       Clock Stop during scalable memory buffers clock stop enter process; in unit of 
       dclk 
     */
    UINT32 rsvd_29 : 3;
    /* rsvd_29 - Bits[31:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} VMSE_PC6_CNTL_2_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_PC6_CNTL_3_MCDDC_DP_REG supported on:                                 */
/*       IVT_EP (0x4018662C)                                                  */
/*       IVT_EX (0x4018662C)                                                  */
/*       HSX (0x401A262C)                                                     */
/*       BDX (0x401A262C)                                                     */
/* Register default value:              0x0FF100C0                            */
#define VMSE_PC6_CNTL_3_MCDDC_DP_REG 0x0C01462C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * This register holds the timing parameters for Intel SMI 2 Clock Stop function. 
 * (EX processor only) 
 */
typedef union {
  struct {
    UINT32 nop_duration : 9;
    /* nop_duration - Bits[8:0], RWS_L, default = 9'b011000000 
       Duration to drive NOPs after asserting Intel SMI 2 Exit Lane during scalable 
       memory buffers clock stop exit process; in unit of dclk 
     */
    UINT32 rsvd_9 : 1;
    /* rsvd_9 - Bits[9:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 assert_exln_duration : 9;
    /* assert_exln_duration - Bits[18:10], RWS_L, default = 9'b001000000 
       Duration to assert Intel SMI 2 Exit Lane during scalable memory buffers clock 
       stop exit process; in unit of dclk 
     */
    UINT32 rsvd_19 : 1;
    /* rsvd_19 - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 mxb_stbl : 9;
    /* mxb_stbl - Bits[28:20], RWS_L, default = 9'b011111111 
       Duration to wait before command is stable at scalable memory buffers; this is 
       the time from deasserting PARK1 signal to the time asserting exit lane; in unit 
       of dclk 
     */
    UINT32 rsvd_29 : 3;
    /* rsvd_29 - Bits[31:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} VMSE_PC6_CNTL_3_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* VMSE_PC6_FSM_STATE_MCDDC_DP_REG supported on:                              */
/*       IVT_EP (0x40186630)                                                  */
/*       IVT_EX (0x40186630)                                                  */
/*       HSX (0x401A2630)                                                     */
/*       BDX (0x401A2630)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_PC6_FSM_STATE_MCDDC_DP_REG 0x0C014630
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * State of the Intel SMI 2 scalable memory buffers clock stop FSM. (EX processor 
 * only) 
 */
typedef union {
  struct {
    UINT32 current_state : 5;
    /* current_state - Bits[4:0], RO_V, default = 5'b00000 
       The current state of the Intel SMI 2 scalable memory buffers clock stop flow 
       FSM. 
     */
    UINT32 rsvd_5 : 11;
    /* rsvd_5 - Bits[15:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 previous_state : 5;
    /* previous_state - Bits[20:16], RO_V, default = 5'b00000 
       The state of the Intel SMI 2 scalable memory buffers clock stop flow FSM before 
       the current state. 
     */
    UINT32 rsvd_21 : 11;
    /* rsvd_21 - Bits[31:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} VMSE_PC6_FSM_STATE_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* MC0_DP_CHKN_BIT_MCDDC_DP_REG supported on:                                 */
/*       IVT_EP (0x40186700)                                                  */
/*       IVT_EX (0x40186700)                                                  */
/*       HSX (0x401A2700)                                                     */
/*       BDX (0x401A2700)                                                     */
/* Register default value:              0x28000002                            */
#define MC0_DP_CHKN_BIT_MCDDC_DP_REG 0x0C014700
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * Note: some defeatures are only present in MC0 register
 */
typedef union {
  struct {
    UINT32 dis_ecc_chk : 1;
    /* dis_ecc_chk - Bits[0:0], RW_LB, default = 1'b0 
       Disable ECC checking
     */
    UINT32 dis_rdimm_par_chk : 1;
    /* dis_rdimm_par_chk - Bits[1:1], RW_LB, default = 1'b1 
       Disable rdimm par check
     */
    UINT32 dis_ha_par_chk : 1;
    /* dis_ha_par_chk - Bits[2:2], RW_LB, default = 1'b0 
       Disable HA par check
     */
    UINT32 dis_ha_par_gen : 1;
    /* dis_ha_par_gen - Bits[3:3], RW_LB, default = 1'b0 
       Disable HA par gen
     */
    UINT32 sw_qck_for_scn_data : 1;
    /* sw_qck_for_scn_data - Bits[4:4], RW_LB, default = 1'b0 
       Switch Qclk for scan data
     */
    UINT32 frc_odd_cyc_rt_dly : 1;
    /* frc_odd_cyc_rt_dly - Bits[5:5], RW_LB, default = 1'b0 
       Force odd cycle RT delay
     */
    UINT32 jkt_addrv : 1;
    /* jkt_addrv - Bits[6:6], RW_LB, default = 1'b0 
       Setting this bit will use the A0 method of logging ADDRV bit immediately in the 
       MCA bank. 
       Clearing this bit will allow the logging of ADDRV only on successful completion 
       of reverse address translation enabled in IVT B0 
     */
    UINT32 dis_err_det_prtl_wr_underfl : 1;
    /* dis_err_det_prtl_wr_underfl - Bits[7:7], RW_LB, default = 1'b0 
       Disable error detection for partial write underfills
     */
    UINT32 ovrd_trng_rank_to_io : 1;
    /* ovrd_trng_rank_to_io - Bits[8:8], RW_LB, default = 1'b0 
       Override training rank output to IO
     */
    UINT32 ena_err_when_poison : 1;
    /* ena_err_when_poison - Bits[9:9], RW_LB, default = 1'b0 
       Error signaling to Home Agent when MC sees poison line and applies correction 
       trials during retry. 
       1: sending both poison and error indication (for poison line and correction 
       trials). 
       0: Don't send error indication for poison line when correction trials were 
       applied. Only assert poison. This behavior is necessary to correctly deal with 
       poison lines (in poison mode) under vmse error flows or under channel mirroring 
       flows. 
       Value of this bit is only relevant when in poison mode.
     */
    UINT32 dis_ck_gate_dp : 1;
    /* dis_ck_gate_dp - Bits[10:10], RW_LB, default = 1'b0 
       Disable clock gating - read and write data path
     */
    UINT32 sel_rank0_training : 1;
    /* sel_rank0_training - Bits[11:11], RW_LB, default = 1'b0 
       Select training rank to be rank 0; not used yet
     */
    UINT32 cfglateerr : 1;
    /* cfglateerr - Bits[12:12], RW_LB, default = 1'b0 
       Configure Err return to HA one cycle late
     */
    UINT32 mcforcercvend3nnh : 1;
    /* mcforcercvend3nnh - Bits[13:13], RW_LB, default = 1'b0 
       Always force Receive enable to be on
     */
    UINT32 uselateqclkrfrdnnn : 1;
    /* uselateqclkrfrdnnn - Bits[14:14], RW_LB, default = 1'b0 
       Use one Qclk late for iscan read fifo read enable
     */
    UINT32 ign_ptrl_uc : 1;
    /* ign_ptrl_uc - Bits[15:15], RW_LB, default = 1'b0 
       Downgrades patrol scrubbing uncorrectable errors to correactable.
     */
    UINT32 ign_mca_ovrd : 1;
    /* ign_mca_ovrd - Bits[16:16], RW_LB, default = 1'b0 
       When set to 1, disables the MCA regular override mechanism and keeps current 
       logs 
     */
    UINT32 log_sec_err_dev : 1;
    /* log_sec_err_dev - Bits[17:17], RW_LB, default = 1'b0 
       When set to 1, Logs the error device from second codeword in a cacheline access 
       if both codewords are in error. By default, MC logs the first codeword device as 
       the failing device in a cacheline (because there are two code words in each 
       cacheline). 
     */
    UINT32 en_uc_err_log : 1;
    /* en_uc_err_log - Bits[18:18], RW_LB, default = 1'b0 
       Enables logging of uncorrectable read errors to MCA banks. When this field is 
       set to 1 along with en_corr_rd_err_log, MC is in mode-2 
     */
    UINT32 en_corr_err_log : 1;
    /* en_corr_err_log - Bits[19:19], RW_LB, default = 1'b0 
       Enables logging of correctable read errors to MCA banks. When set to 1, this 
       puts MC in mode-1. 
     */
    UINT32 dis_wdb_par_chk : 1;
    /* dis_wdb_par_chk - Bits[20:20], RW_LB, default = 1'b0 
       Disable Write Data Buffer parity checking
     */
    UINT32 dis_spare_dir : 1;
    /* dis_spare_dir - Bits[21:21], RW_LB, default = 1'b0 
       Disable spare device directory storage.
     */
    UINT32 force_scls_ecc : 1;
    /* force_scls_ecc - Bits[22:22], RW_LB, default = 1'b0 
       Force the use the Intel SMI 2 1:1 subchannel lockstep ECC layout. For testing on 
       native DDR3. 
     */
    UINT32 dis_vmse_link_fail : 1;
    /* dis_vmse_link_fail - Bits[23:23], RW_LB, default = 1'b0 
       Disable VMSE Link Fail state. If this bit is state the VMSE Error FSM will never 
       enter link fail state. This bit must be set to 0 to enable the SB persistent 
       error counter. Setting this bit to 1 clears SB persistent error counter 
       (vmse_retry_sb_err_count.persistent_err_cnt). 
     */
    UINT32 en_rdimm_par_err_log : 1;
    /* en_rdimm_par_err_log - Bits[24:24], RW_LB, default = 1'b0 
       Enable RDIMM parity error logging. To be set by BIOS after switch to Normal mode 
       to prevent false logging of RDIMM parity error during reset. JKT B0 b3246253 
     */
    UINT32 rsvd_25 : 7;
  } Bits;
  UINT32 Data;
} MC0_DP_CHKN_BIT_MCDDC_DP_STRUCT;
#endif /* ASM_INC */


/* MC1_DP_CHKN_BIT_MCDDC_DP_REG supported on:                                 */
/*       IVT_EP (0x40186704)                                                  */
/*       IVT_EX (0x40186704)                                                  */
/*       HSX (0x401A2704)                                                     */
/*       BDX (0x401A2704)                                                     */
/* Register default value:              0x00000002                            */
#define MC1_DP_CHKN_BIT_MCDDC_DP_REG 0x0C014704
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.2.CFG.xml.
 * generated by critter 20_2_0x704
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 21;
    UINT32 dis_spare_dir : 1;
    /* dis_spare_dir - Bits[21:21], RW_LB, default = 1'b0 
       Disable spare device directory storage under Intel SMI 2 1:1 mode with x4 
       devices. 
     */
    UINT32 rsvd_22 : 1;
    UINT32 rsvd_23 : 9;
    /* rsvd_23 - Bits[31:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MC1_DP_CHKN_BIT_MCDDC_DP_STRUCT;
#endif /* ASM_INC */










/* WDBPRELOADREG0_MCDDC_DP_REG supported on:                                  */
/*       IVT_EP (0x40186098)                                                  */
/*       IVT_EX (0x40186098)                                                  */
/* Register default value:              0x00000000                            */
#define WDBPRELOADREG0_MCDDC_DP_REG 0x0C014098



/* WDBPRELOADREG1_MCDDC_DP_REG supported on:                                  */
/*       IVT_EP (0x4018609C)                                                  */
/*       IVT_EX (0x4018609C)                                                  */
/* Register default value:              0x00000000                            */
#define WDBPRELOADREG1_MCDDC_DP_REG 0x0C01409C



/* WDBPRELOADCTRL_MCDDC_DP_REG supported on:                                  */
/*       IVT_EP (0x401860A0)                                                  */
/*       IVT_EX (0x401860A0)                                                  */
/* Register default value:              0x00000000                            */
#define WDBPRELOADCTRL_MCDDC_DP_REG 0x0C0140A0



/* IOSAV_CH_B0_B3_BW_SERR_MCDDC_DP_REG supported on:                          */
/*       IVT_EP (0x401861A0)                                                  */
/*       IVT_EX (0x401861A0)                                                  */
/* Register default value:              0x00000000                            */
#define IOSAV_CH_B0_B3_BW_SERR_MCDDC_DP_REG 0x0C0141A0



/* IOSAV_CH_B4_B7_BW_SERR_MCDDC_DP_REG supported on:                          */
/*       IVT_EP (0x401861A4)                                                  */
/*       IVT_EX (0x401861A4)                                                  */
/* Register default value:              0x00000000                            */
#define IOSAV_CH_B4_B7_BW_SERR_MCDDC_DP_REG 0x0C0141A4



/* IOSAV_CH_B8_BW_SERR_MCDDC_DP_REG supported on:                             */
/*       IVT_EP (0x401861A8)                                                  */
/*       IVT_EX (0x401861A8)                                                  */
/* Register default value:              0x00000000                            */
#define IOSAV_CH_B8_BW_SERR_MCDDC_DP_REG 0x0C0141A8



/* IOSAV_CH_B0_B3_BW_MASK_MCDDC_DP_REG supported on:                          */
/*       IVT_EP (0x401861B0)                                                  */
/*       IVT_EX (0x401861B0)                                                  */
/* Register default value:              0xFFFFFFFF                            */
#define IOSAV_CH_B0_B3_BW_MASK_MCDDC_DP_REG 0x0C0141B0



/* IOSAV_CH_B4_B7_BW_MASK_MCDDC_DP_REG supported on:                          */
/*       IVT_EP (0x401861B4)                                                  */
/*       IVT_EX (0x401861B4)                                                  */
/* Register default value:              0xFFFFFFFF                            */
#define IOSAV_CH_B4_B7_BW_MASK_MCDDC_DP_REG 0x0C0141B4



/* IOSAV_CH_B8_BW_MASK_MCDDC_DP_REG supported on:                             */
/*       IVT_EP (0x401861B8)                                                  */
/*       IVT_EX (0x401861B8)                                                  */
/* Register default value:              0x000000FF                            */
#define IOSAV_CH_B8_BW_MASK_MCDDC_DP_REG 0x0C0141B8



/* IOSAV_DQ_LFSR0_MCDDC_DP_REG supported on:                                  */
/*       IVT_EP (0x401861C0)                                                  */
/*       IVT_EX (0x401861C0)                                                  */
/* Register default value:              0x00000000                            */
#define IOSAV_DQ_LFSR0_MCDDC_DP_REG 0x0C0141C0



/* IOSAV_DQ_LFSRSEED0_MCDDC_DP_REG supported on:                              */
/*       IVT_EP (0x401861C4)                                                  */
/*       IVT_EX (0x401861C4)                                                  */
/* Register default value:              0x00000000                            */
#define IOSAV_DQ_LFSRSEED0_MCDDC_DP_REG 0x0C0141C4



/* IOSAV_DQ_LFSR1_MCDDC_DP_REG supported on:                                  */
/*       IVT_EP (0x401861C8)                                                  */
/*       IVT_EX (0x401861C8)                                                  */
/* Register default value:              0x00000000                            */
#define IOSAV_DQ_LFSR1_MCDDC_DP_REG 0x0C0141C8



/* IOSAV_DQ_LFSRSEED1_MCDDC_DP_REG supported on:                              */
/*       IVT_EP (0x401861CC)                                                  */
/*       IVT_EX (0x401861CC)                                                  */
/* Register default value:              0x00000000                            */
#define IOSAV_DQ_LFSRSEED1_MCDDC_DP_REG 0x0C0141CC



/* IOSAV_DQ_LFSR2_MCDDC_DP_REG supported on:                                  */
/*       IVT_EP (0x401861D0)                                                  */
/*       IVT_EX (0x401861D0)                                                  */
/* Register default value:              0x00000000                            */
#define IOSAV_DQ_LFSR2_MCDDC_DP_REG 0x0C0141D0



/* IOSAV_DQ_LFSRSEED2_MCDDC_DP_REG supported on:                              */
/*       IVT_EP (0x401861D4)                                                  */
/*       IVT_EX (0x401861D4)                                                  */
/* Register default value:              0x00000000                            */
#define IOSAV_DQ_LFSRSEED2_MCDDC_DP_REG 0x0C0141D4









#endif /* MCDDC_DP_h */
