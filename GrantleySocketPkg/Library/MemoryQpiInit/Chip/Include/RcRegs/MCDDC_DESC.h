/* Date Stamp: 8/23/2014 */

#ifndef MCDDC_DESC_h
#define MCDDC_DESC_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* MCDDC_DESC_IVT_DEV 15                                                      */
/* MCDDC_DESC_IVT_FUN 2                                                       */
/* For HSX_HOST:                                                              */
/* MCDDC_DESC_HSX_DEV 19                                                      */
/* MCDDC_DESC_HSX_FUN 2                                                       */
/* For BDX_HOST:                                                              */
/* MCDDC_DESC_BDX_DEV 19                                                      */
/* MCDDC_DESC_BDX_FUN 2                                                       */

/* VID_MCDDC_DESC_REG supported on:                                           */
/*       IVT_EP (0x2017A000)                                                  */
/*       IVT_EX (0x2017A000)                                                  */
/*       HSX (0x2019A000)                                                     */
/*       BDX (0x2019A000)                                                     */
/* Register default value:              0x8086                                */
#define VID_MCDDC_DESC_REG 0x0C022000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
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
} VID_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* DID_MCDDC_DESC_REG supported on:                                           */
/*       IVT_EP (0x2017A002)                                                  */
/*       IVT_EX (0x2017A002)                                                  */
/*       HSX (0x2019A002)                                                     */
/*       BDX (0x2019A002)                                                     */
/* Register default value on IVT_EP:    0x0EAA                                */
/* Register default value on IVT_EX:    0x0EAA                                */
/* Register default value on HSX:       0x2FAA                                */
/* Register default value on BDX:       0x6FAA                                */
#define DID_MCDDC_DESC_REG 0x0C022002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6FAA 
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
} DID_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* PCICMD_MCDDC_DESC_REG supported on:                                        */
/*       IVT_EP (0x2017A004)                                                  */
/*       IVT_EX (0x2017A004)                                                  */
/*       HSX (0x2019A004)                                                     */
/*       BDX (0x2019A004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_MCDDC_DESC_REG 0x0C022004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
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
} PCICMD_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* PCISTS_MCDDC_DESC_REG supported on:                                        */
/*       IVT_EP (0x2017A006)                                                  */
/*       IVT_EX (0x2017A006)                                                  */
/*       HSX (0x2019A006)                                                     */
/*       BDX (0x2019A006)                                                     */
/* Register default value:              0x0010                                */
#define PCISTS_MCDDC_DESC_REG 0x0C022006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
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
} PCISTS_MCDDC_DESC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_MCDDC_DESC_REG supported on:                                           */
/*       IVT_EP (0x1017A008)                                                  */
/*       IVT_EX (0x1017A008)                                                  */
/*       HSX (0x1019A008)                                                     */
/*       BDX (0x1019A008)                                                     */
/* Register default value:              0x00                                  */
#define RID_MCDDC_DESC_REG 0x0C021008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
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
} RID_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_MCDDC_DESC_REG supported on:                                        */
/*       IVT_EP (0x1017A009)                                                  */
/*       IVT_EX (0x1017A009)                                                  */
/*       HSX (0x1019A009)                                                     */
/*       BDX (0x1019A009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_MCDDC_DESC_REG 0x0C021009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_MCDDC_DESC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_MCDDC_DESC_REG supported on:                                        */
/*       IVT_EP (0x2017A00A)                                                  */
/*       IVT_EX (0x2017A00A)                                                  */
/*       HSX (0x2019A00A)                                                     */
/*       BDX (0x2019A00A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_MCDDC_DESC_REG 0x0C02200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
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
} CCR_N1_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* CLSR_MCDDC_DESC_REG supported on:                                          */
/*       IVT_EP (0x1017A00C)                                                  */
/*       IVT_EX (0x1017A00C)                                                  */
/*       HSX (0x1019A00C)                                                     */
/*       BDX (0x1019A00C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_MCDDC_DESC_REG 0x0C02100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
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
} CLSR_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* PLAT_MCDDC_DESC_REG supported on:                                          */
/*       IVT_EP (0x1017A00D)                                                  */
/*       IVT_EX (0x1017A00D)                                                  */
/*       HSX (0x1019A00D)                                                     */
/*       BDX (0x1019A00D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_MCDDC_DESC_REG 0x0C02100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
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
} PLAT_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* HDR_MCDDC_DESC_REG supported on:                                           */
/*       IVT_EP (0x1017A00E)                                                  */
/*       IVT_EX (0x1017A00E)                                                  */
/*       HSX (0x1019A00E)                                                     */
/*       BDX (0x1019A00E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_MCDDC_DESC_REG 0x0C02100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
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
} HDR_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* BIST_MCDDC_DESC_REG supported on:                                          */
/*       IVT_EP (0x1017A00F)                                                  */
/*       IVT_EX (0x1017A00F)                                                  */
/*       HSX (0x1019A00F)                                                     */
/*       BDX (0x1019A00F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_MCDDC_DESC_REG 0x0C02100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
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
} BIST_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* SVID_MCDDC_DESC_REG supported on:                                          */
/*       IVT_EP (0x2017A02C)                                                  */
/*       IVT_EX (0x2017A02C)                                                  */
/*       HSX (0x2019A02C)                                                     */
/*       BDX (0x2019A02C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_MCDDC_DESC_REG 0x0C02202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x02c
 */
typedef union {
  struct {
    UINT16 subsystem_vendor_identification_number : 16;
    /* subsystem_vendor_identification_number - Bits[15:0], RW_O, default = 16'b1000000010000110 
       The default value specifies Intel but can be set to any value once after reset.
     */
  } Bits;
  UINT16 Data;
} SVID_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* SDID_MCDDC_DESC_REG supported on:                                          */
/*       IVT_EP (0x2017A02E)                                                  */
/*       IVT_EX (0x2017A02E)                                                  */
/*       HSX (0x2019A02E)                                                     */
/*       BDX (0x2019A02E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_MCDDC_DESC_REG 0x0C02202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x02e
 */
typedef union {
  struct {
    UINT16 subsystem_device_identification_number : 16;
    /* subsystem_device_identification_number - Bits[15:0], RW_O, default = 16'b0000000000000000 
       Assigned by the subsystem vendor to uniquely identify the subsystem
     */
  } Bits;
  UINT16 Data;
} SDID_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_MCDDC_DESC_REG supported on:                                        */
/*       IVT_EP (0x1017A034)                                                  */
/*       IVT_EX (0x1017A034)                                                  */
/*       HSX (0x1019A034)                                                     */
/*       BDX (0x1019A034)                                                     */
/* Register default value on IVT_EP:    0x40                                  */
/* Register default value on IVT_EX:    0x40                                  */
/* Register default value on HSX:       0x00                                  */
/* Register default value on BDX:       0x40                                  */
#define CAPPTR_MCDDC_DESC_REG 0x0C021034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
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
} CAPPTR_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* INTL_MCDDC_DESC_REG supported on:                                          */
/*       IVT_EP (0x1017A03C)                                                  */
/*       IVT_EX (0x1017A03C)                                                  */
/*       HSX (0x1019A03C)                                                     */
/*       BDX (0x1019A03C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_MCDDC_DESC_REG 0x0C02103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
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
} INTL_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* INTPIN_MCDDC_DESC_REG supported on:                                        */
/*       IVT_EP (0x1017A03D)                                                  */
/*       IVT_EX (0x1017A03D)                                                  */
/*       HSX (0x1019A03D)                                                     */
/*       BDX (0x1019A03D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_MCDDC_DESC_REG 0x0C02103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
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
} INTPIN_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* MINGNT_MCDDC_DESC_REG supported on:                                        */
/*       IVT_EP (0x1017A03E)                                                  */
/*       IVT_EX (0x1017A03E)                                                  */
/*       HSX (0x1019A03E)                                                     */
/*       BDX (0x1019A03E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_MCDDC_DESC_REG 0x0C02103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
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
} MINGNT_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_MCDDC_DESC_REG supported on:                                        */
/*       IVT_EP (0x1017A03F)                                                  */
/*       IVT_EX (0x1017A03F)                                                  */
/*       HSX (0x1019A03F)                                                     */
/*       BDX (0x1019A03F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_MCDDC_DESC_REG 0x0C02103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
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
} MAXLAT_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* PXPCAP_MCDDC_DESC_REG supported on:                                        */
/*       IVT_EP (0x4017A040)                                                  */
/*       IVT_EX (0x4017A040)                                                  */
/*       HSX (0x4019A040)                                                     */
/*       BDX (0x4019A040)                                                     */
/* Register default value:              0x00910010                            */
#define PXPCAP_MCDDC_DESC_REG 0x0C024040
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x040
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
} PXPCAP_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* DIMMMTR_0_MCDDC_DESC_REG supported on:                                     */
/*       IVT_EP (0x4017A080)                                                  */
/*       IVT_EX (0x4017A080)                                                  */
/*       HSX (0x4019A080)                                                     */
/*       BDX (0x4019A080)                                                     */
/* Register default value on IVT_EP:    0x00000000                            */
/* Register default value on IVT_EX:    0x00000000                            */
/* Register default value on HSX:       0x0000000C                            */
/* Register default value on BDX:       0x0000000C                            */
#define DIMMMTR_0_MCDDC_DESC_REG 0x0C024080


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * The option 1 mapping is automatically selected when 3rd DIMM slot (DS#2) is
 * unpopulated (DIMMMTR2), including single, dual, and quad-rank DIMMs in any of 
 * the 2 slots. Default 
 * mapping is automatically selected any time a DIMM is loaded in the 3th slot 
 * (DS#2). 
 */
typedef union {
  struct {
    UINT32 ca_width : 2;
    /* ca_width - Bits[1:0], RW_LB, default = 2'b00 
       00 - 10 bits
       01 - 11 bits
       10 - 12 bits
       11 - reserved
     */
    UINT32 ra_width : 3;
    /* ra_width - Bits[4:2], RW_LB, default = 3'b011 
       000 - reserved (512Mb DDR3 not supported)
       001 - 13 bits
       010 - 14 bits
       011 - 15 bits
       100 - 16 bits
       101 - 17 bits
       110 - 18 bits
       111: reserved
     */
    UINT32 ddr3_dnsty : 2;
    /* ddr3_dnsty - Bits[6:5], RW_LB, default = 2'b00 
       00 - Reserved
       01 - 2Gb
       10 - 4Gb
       11 - 8Gb
       
     */
    UINT32 ddr3_width : 2;
    /* ddr3_width - Bits[8:7], RW_LB, default = 2'b00 
       00 - x4
       01 - x8
       10 - x16
       11 - reserved
       
       Used to determine if a configuration is capable of supporting DDDC.
     */
    UINT32 rsvd_9 : 3;
    /* rsvd_9 - Bits[11:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_cnt : 2;
    /* rank_cnt - Bits[13:12], RW_LB, default = 2'b00 
       00 - SR
       01 - DR
       10 - QR
       11 - 8R
     */
    UINT32 dimm_pop : 1;
    /* dimm_pop - Bits[14:14], RW_LB, default = 1'b0 
       DIMM populated if set; otherwise, unpopulated.
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_disable : 4;
    /* rank_disable - Bits[19:16], RW_LB, default = 4'b0000 
       RANK Disable Control to disable refresh and ZQCAL operation. This bit setting 
       must be set consistently with TERM_RNK_MSK, i.e. both corresponding bits cannot 
       be set at the same time. In the other word, a disabled rank must not be selected 
       for the terminaton rank. 
       
       RANK_DISABLE[3], i.e. bit 19: rank 3 disable. Note DIMMMTR_2.RANK_DISABLE[3] is 
       don't care since DIMM 2 must not be quad-rank 
       RANK_DISABLE[2], i.e. bit 18: rank 2 disable. Note DIMMMTR_2.RANK_DISABLE[2] is 
       don't care since DIMM 2 must not be quad-rank 
       RANK_DISABLE[1], i.e. bit 17: rank 1 disable
       RANK_DISABLE[0], i.e. bit 16: rank 0 disable
       
       when set, no refresh will be perform on this rank. ODT termination is not 
       affected by this bit. 
       Note that patrols are disabled by dimm*_pat_rank_disable of amap register now 
       and not affected by this bit field. 
     */
    UINT32 ddr4_mode : 1;
    /* ddr4_mode - Bits[20:20], RW_LB, default = 1'b0 
       When set, indicating DDR4 DIMM type is used. Channel 0 and 1, and channel 2 and 
       3 must have matching values even if both ddr channels are not poplulated. 
     */
    UINT32 hdrl : 1;
    /* hdrl - Bits[21:21], RW_LB, default = 1'b0 
       When set, will enable High Density Reduced Load mode which will transmit Row 
       address bits 17:16 on chip select lines 
       7:6 and 3:2.
     */
    UINT32 hdrl_parity : 1;
    /* hdrl_parity - Bits[22:22], RW_LB, default = 1'b0 
       When set, will enable parity calculation to include address bits 17:16 which are 
       sent on chip select lines 
       7:6 and 3:2.
     */
    UINT32 ddr4_3dsnumranks_cs : 2;
    /* ddr4_3dsnumranks_cs - Bits[24:23], RW_LB, default = 2'b00 
       Number of sub ranks per chip select per dimm of DDR4 3DS and non3ds_lrdimm (this 
       is a dimm 
       specific field. There are restriction on rank_cnt field if this field is nonzero 
       fro 3ds devices. 
       Can not mix 2 chip_select parts and 1 chip_select part on the same channel)
       00- 3ds/non3ds_lrdimm ddr4 multiple ranks/chip select disabled
       01- 2 ranks per chip select
       10- 4 ranks per chip select
       11- 8 ranks per chip select
     */
    UINT32 rsvd_25 : 7;
    /* rsvd_25 - Bits[31:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DIMMMTR_0_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * The option 1 mapping is automatically selected when 3rd DIMM slot (DS#2) is 
 * unpopulated (DIMMMTR2), including single, dual, and quad-rank DIMMs in any of 
 * the 2 slots. Default mapping is automatically selected any time a DIMM is loaded 
 * in the 3th slot (DS#2). 
 */
typedef union {
  struct {
    UINT32 ca_width : 2;
    /* ca_width - Bits[1:0], RW_LB, default = 2'b00 
       00 - 10 bits
       01 - 11 bits
       10 - 12 bits
       11 - reserved
     */
    UINT32 ra_width : 3;
    /* ra_width - Bits[4:2], RW_LB, default = 3'b011 
       000 - reserved (512Mb DDR3 not supported)
       001 - 13 bits
       010 - 14 bits
       011 - 15 bits
       100 - 16 bits
       // errbdx. Understand the fuse implications.
       101 - 17 bits
       110 - 18 bits
       111: reserved
     */
    UINT32 ddr3_dnsty : 3;
    /* ddr3_dnsty - Bits[7:5], RW_LB, default = 3'b000 
       00 - Reserved
       01 - 2Gb
       10 - 4Gb
       11 - 8Gb
       100 - 16Gb for BDX DDR4
       
     */
    UINT32 ddr3_width : 2;
    /* ddr3_width - Bits[9:8], RW_LB, default = 2'b00 
       00 - x4
       01 - x8
       10 - x16
       11 - reserved
       
       Used to determine if a configuration is capable of supporting DDDC.
     */
    UINT32 rsvd_10 : 2;
    /* rsvd_10 - Bits[11:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_cnt : 2;
    /* rank_cnt - Bits[13:12], RW_LB, default = 2'b00 
       00 - SR
       01 - DR
       10 - QR
       11 - 8R
     */
    UINT32 dimm_pop : 1;
    /* dimm_pop - Bits[14:14], RW_LB, default = 1'b0 
       DIMM populated if set; otherwise, unpopulated.
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_disable : 4;
    /* rank_disable - Bits[19:16], RW_LB, default = 4'b0000 
       RANK Disable Control to disable refresh and ZQCAL operation. This bit setting 
       must be set consistently with TERM_RNK_MSK, i.e. both corresponding bits cannot 
       be set at the same time. In the other word, a disabled rank must not be selected 
       for the terminaton rank. 
       
       RANK_DISABLE[3], i.e. bit 19: rank 3 disable. Note DIMMMTR_2.RANK_DISABLE[3] is 
       don't care since DIMM 2 must not be quad-rank 
       RANK_DISABLE[2], i.e. bit 18: rank 2 disable. Note DIMMMTR_2.RANK_DISABLE[2] is 
       don't care since DIMM 2 must not be quad-rank 
       RANK_DISABLE[1], i.e. bit 17: rank 1 disable
       RANK_DISABLE[0], i.e. bit 16: rank 0 disable
       
       when set, no refresh will be perform on this rank. ODT termination is not 
       affected by this bit. 
       Note that patrols are disabled by dimm*_pat_rank_disable of amap register now 
       and not affected by this bit field. 
     */
    UINT32 ddr4_mode : 1;
    /* ddr4_mode - Bits[20:20], RW_LB, default = 1'b0 
       When set, indicating DDR4 DIMM type is used. Channel 0 and 1, and channel 2 and 
       3 must have matching values even if both ddr channels are not poplulated. 
     */
    UINT32 hdrl : 1;
    /* hdrl - Bits[21:21], RW_LB, default = 1'b0 
       When set, will enable High Density Reduced Load mode which will transmit Row 
       address bits 17:16 on chip select lines 7:6 and 3:2. 
     */
    UINT32 hdrl_parity : 1;
    /* hdrl_parity - Bits[22:22], RW_LB, default = 1'b0 
       When set, will enable parity calculation to include address bits 17:16 which are 
       sent on chip select lines 7:6 and 3:2. 
     */
    UINT32 ddr4_3dsnumranks_cs : 2;
    /* ddr4_3dsnumranks_cs - Bits[24:23], RW_LB, default = 2'b00 
       Number of sub ranks per chip select per dimm of DDR4 3DS and non3DS_LRDIMM (This 
       is a dimm specific field. There are restriction on rank_cnt field if this field 
       is nonzero fro 3DS devices. Can not mix 2 chip_select parts and 1 chip_select 
       part on the same channel.) 
       00- 3DS/non3DS_LRDIMM DDR4 multiple ranks/chip select disabled
       01- 2 ranks per chip select
       10- 4 ranks per chip select
       11- 8 ranks per chip select
     */
    UINT32 rsvd_25 : 7;
    /* rsvd_25 - Bits[31:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DIMMMTR_0_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* DIMMMTR_1_MCDDC_DESC_REG supported on:                                     */
/*       IVT_EP (0x4017A084)                                                  */
/*       IVT_EX (0x4017A084)                                                  */
/*       HSX (0x4019A084)                                                     */
/*       BDX (0x4019A084)                                                     */
/* Register default value on IVT_EP:    0x00000000                            */
/* Register default value on IVT_EX:    0x00000000                            */
/* Register default value on HSX:       0x0000000C                            */
/* Register default value on BDX:       0x0000000C                            */
#define DIMMMTR_1_MCDDC_DESC_REG 0x0C024084


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * The option 1 mapping is automatically selected when 3rd DIMM slot (DS#2) is 
 * unpopulated (DIMMMTR2), including single, dual, and quad-rank DIMMs in any of 
 * the 2 slots. Default 
 * mapping is automatically selected any time a DIMM is loaded in the 3th slot 
 * (DS#2). 
 */
typedef union {
  struct {
    UINT32 ca_width : 2;
    /* ca_width - Bits[1:0], RW_LB, default = 2'b00 
       00 - 10 bits
       01 - 11 bits
       10 - 12 bits
       11 - reserved
     */
    UINT32 ra_width : 3;
    /* ra_width - Bits[4:2], RW_LB, default = 3'b011 
       000 - reserved (512Mb DDR3 not supported)
       001 - 13 bits
       010 - 14 bits
       011 - 15 bits
       100 - 16 bits
       101 - 17 bits
       110 - 18 bits
       111: reserved
     */
    UINT32 ddr3_dnsty : 2;
    /* ddr3_dnsty - Bits[6:5], RW_LB, default = 2'b00 
       00 - Reserved
       01 - 2Gb
       10 - 4Gb
       11 - 8Gb
       
       Not used.
     */
    UINT32 ddr3_width : 2;
    /* ddr3_width - Bits[8:7], RW_LB, default = 2'b00 
       00 - x4
       01 - x8
       10 - x16
       11 - reserved
       
       Used to determine if a configuration is capable of supporting DDDC.
     */
    UINT32 rsvd_9 : 3;
    /* rsvd_9 - Bits[11:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_cnt : 2;
    /* rank_cnt - Bits[13:12], RW_LB, default = 2'b00 
       00 - SR
       01 - DR
       10 - QR
       11 - reserved
     */
    UINT32 dimm_pop : 1;
    /* dimm_pop - Bits[14:14], RW_LB, default = 1'b0 
       DIMM populated if set; otherwise, unpopulated.
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_disable : 4;
    /* rank_disable - Bits[19:16], RW_LB, default = 4'b0000 
       RANK Disable Control to disable refresh and ZQCAL operation. This bit setting 
       must be set consistently with TERM_RNK_MSK, i.e. both corresponding bits cannot 
       be set at the same time. In the other word, a disabled rank must not be selected 
       for the terminaton rank. 
       
       RANK_DISABLE[3], i.e. bit 19: rank 3 disable. Note DIMMMTR_2.RANK_DISABLE[3] is 
       don't care since DIMM 2 must not be quad-rank 
       RANK_DISABLE[2], i.e. bit 18: rank 2 disable. Note DIMMMTR_2.RANK_DISABLE[2] is 
       don't care since DIMM 2 must not be quad-rank 
       RANK_DISABLE[1], i.e. bit 17: rank 1 disable
       RANK_DISABLE[0], i.e. bit 16: rank 0 disable
       
       when set, no refresh will be perform on this rank. ODT termination is not 
       affected by this bit. 
       Note that patrols are disabled by dimm*_pat_rank_disable of amap register now 
       and not affected by this bit field. 
     */
    UINT32 rsvd_20 : 3;
    /* rsvd_20 - Bits[22:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ddr4_3dsnumranks_cs : 2;
    /* ddr4_3dsnumranks_cs - Bits[24:23], RW_LB, default = 2'b00  */
    UINT32 rsvd_25 : 7;
    /* rsvd_25 - Bits[31:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DIMMMTR_1_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * The option 1 mapping is automatically selected when 3rd DIMM slot (DS#2) is 
 * unpopulated (DIMMMTR2), including single, dual, and quad-rank DIMMs in any of 
 * the 2 slots. Default mapping is automatically selected any time a DIMM is loaded 
 * in the 3th slot (DS#2). 
 */
typedef union {
  struct {
    UINT32 ca_width : 2;
    /* ca_width - Bits[1:0], RW_LB, default = 2'b00 
       00 - 10 bits
       01 - 11 bits
       10 - 12 bits
       11 - reserved
     */
    UINT32 ra_width : 3;
    /* ra_width - Bits[4:2], RW_LB, default = 3'b011 
       000 - reserved (512Mb DDR3 not supported)
       001 - 13 bits
       010 - 14 bits
       011 - 15 bits
       100 - 16 bits
       101 - 17 bits
       110 - 18 bits
       111: reserved
     */
    UINT32 ddr3_dnsty : 3;
    /* ddr3_dnsty - Bits[7:5], RW_LB, default = 3'b000 
       000 - Reserved
       001 - 2Gb
       010 - 4Gb
       011 - 8Gb
       100 - 16Gb for BDX DDR4
       
     */
    UINT32 ddr3_width : 2;
    /* ddr3_width - Bits[9:8], RW_LB, default = 2'b00 
       00 - x4
       01 - x8
       10 - x16
       11 - reserved
       
       Used to determine if a configuration is capable of supporting DDDC.
     */
    UINT32 rsvd_10 : 2;
    /* rsvd_10 - Bits[11:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_cnt : 2;
    /* rank_cnt - Bits[13:12], RW_LB, default = 2'b00 
       00 - SR
       01 - DR
       10 - QR
       11 - reserved
     */
    UINT32 dimm_pop : 1;
    /* dimm_pop - Bits[14:14], RW_LB, default = 1'b0 
       DIMM populated if set; otherwise, unpopulated.
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_disable : 4;
    /* rank_disable - Bits[19:16], RW_LB, default = 4'b0000 
       RANK Disable Control to disable refresh and ZQCAL operation. This bit setting 
       must be set consistently with TERM_RNK_MSK, i.e. both corresponding bits cannot 
       be set at the same time. In the other word, a disabled rank must not be selected 
       for the terminaton rank. 
       
       RANK_DISABLE[3], i.e. bit 19: rank 3 disable. Note DIMMMTR_2.RANK_DISABLE[3] is 
       don't care since DIMM 2 must not be quad-rank 
       RANK_DISABLE[2], i.e. bit 18: rank 2 disable. Note DIMMMTR_2.RANK_DISABLE[2] is 
       don't care since DIMM 2 must not be quad-rank 
       RANK_DISABLE[1], i.e. bit 17: rank 1 disable
       RANK_DISABLE[0], i.e. bit 16: rank 0 disable
       
       when set, no refresh will be perform on this rank. ODT termination is not 
       affected by this bit. 
       Note that patrols are disabled by dimm*_pat_rank_disable of amap register now 
       and not affected by this bit field. 
     */
    UINT32 rsvd_20 : 3;
    /* rsvd_20 - Bits[22:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ddr4_3dsnumranks_cs : 2;
    /* ddr4_3dsnumranks_cs - Bits[24:23], RW_LB, default = 2'b00  */
    UINT32 rsvd_25 : 7;
    /* rsvd_25 - Bits[31:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DIMMMTR_1_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* DIMMMTR_2_MCDDC_DESC_REG supported on:                                     */
/*       IVT_EP (0x4017A088)                                                  */
/*       IVT_EX (0x4017A088)                                                  */
/*       HSX (0x4019A088)                                                     */
/*       BDX (0x4019A088)                                                     */
/* Register default value on IVT_EP:    0x00000000                            */
/* Register default value on IVT_EX:    0x00000000                            */
/* Register default value on HSX:       0x0000000C                            */
/* Register default value on BDX:       0x0000000C                            */
#define DIMMMTR_2_MCDDC_DESC_REG 0x0C024088


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * The option 1 mapping is automatically selected when 3rd DIMM slot (DS#2) is
 * unpopulated (DIMMMTR2), including single, dual, and quad-rank DIMMs in any of 
 * the 2 slots. Default 
 * mapping is automatically selected any time a DIMM is loaded in the 3th slot 
 * (DS#2). 
 */
typedef union {
  struct {
    UINT32 ca_width : 2;
    /* ca_width - Bits[1:0], RW_LB, default = 2'b00 
       00 - 10 bits
       01 - 11 bits
       10 - 12 bits
       11 - reserved
     */
    UINT32 ra_width : 3;
    /* ra_width - Bits[4:2], RW_LB, default = 3'b011 
       000 - reserved (512Mb DDR3 not supported)
       001 - 13 bits
       010 - 14 bits
       011 - 15 bits
       100 - 16 bits
       101 - 17 bits
       110 - 18 bits
       111: reserved
     */
    UINT32 ddr3_dnsty : 2;
    /* ddr3_dnsty - Bits[6:5], RW_LB, default = 2'b00 
       00 - Reserved
       01 - 2Gb
       10 - 4Gb
       11 - 8Gb
       
       Not used.
     */
    UINT32 ddr3_width : 2;
    /* ddr3_width - Bits[8:7], RW_LB, default = 2'b00 
       00 - x4
       01 - x8
       10 - x16
       11 - reserved
       
       Used to determine if a configuration is capable of supporting DDDC.
     */
    UINT32 rsvd_9 : 3;
    /* rsvd_9 - Bits[11:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_cnt : 2;
    /* rank_cnt - Bits[13:12], RW_LB, default = 2'b00 
       00 - SR
       01 - DR
       10 - QR
       11 - reserved
     */
    UINT32 dimm_pop : 1;
    /* dimm_pop - Bits[14:14], RW_LB, default = 1'b0 
       DIMM populated if set; otherwise, unpopulated.
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_disable : 4;
    /* rank_disable - Bits[19:16], RW_LB, default = 4'b0000 
       RANK Disable Control to disable refresh and ZQCAL operation. This bit setting 
       must be set consistently with TERM_RNK_MSK, i.e. both corresponding bits cannot 
       be set at the same time. In the other word, a disabled rank must not be selected 
       for the terminaton rank. 
       
       RANK_DISABLE[3], i.e. bit 19: rank 3 disable. Note DIMMMTR_2.RANK_DISABLE[3] is 
       don't care since DIMM 2 must not be quad-rank 
       RANK_DISABLE[2], i.e. bit 18: rank 2 disable. Note DIMMMTR_2.RANK_DISABLE[2] is 
       don't care since DIMM 2 must not be quad-rank 
       RANK_DISABLE[1], i.e. bit 17: rank 1 disable
       RANK_DISABLE[0], i.e. bit 16: rank 0 disable
       
       when set, no refresh will be perform on this rank. ODT termination is not 
       affected by this bit. 
       Note that patrols are disabled by dimm*_pat_rank_disable of amap register now 
       and not affected by this bit field. 
     */
    UINT32 rsvd_20 : 3;
    /* rsvd_20 - Bits[22:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ddr4_3dsnumranks_cs : 2;
    /* ddr4_3dsnumranks_cs - Bits[24:23], RW_LB, default = 2'b00  */
    UINT32 rsvd_25 : 7;
    /* rsvd_25 - Bits[31:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DIMMMTR_2_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * The option 1 mapping is automatically selected when 3rd DIMM slot (DS#2) is 
 * unpopulated (DIMMMTR2), including single, dual, and quad-rank DIMMs in any of 
 * the 2 slots. Default mapping is automatically selected any time a DIMM is loaded 
 * in the 3th slot (DS#2). 
 */
typedef union {
  struct {
    UINT32 ca_width : 2;
    /* ca_width - Bits[1:0], RW_LB, default = 2'b00 
       00 - 10 bits
       01 - 11 bits
       10 - 12 bits
       11 - reserved
     */
    UINT32 ra_width : 3;
    /* ra_width - Bits[4:2], RW_LB, default = 3'b011 
       000 - reserved (512Mb DDR3 not supported)
       001 - 13 bits
       010 - 14 bits
       011 - 15 bits
       100 - 16 bits
       101 - 17 bits
       110 - 18 bits
       111: reserved
     */
    UINT32 ddr3_dnsty : 3;
    /* ddr3_dnsty - Bits[7:5], RW_LB, default = 3'b000 
       00 - Reserved
       01 - 2Gb
       10 - 4Gb
       11 - 8Gb
       100 - 16Gb for BDX DDR4
       
     */
    UINT32 ddr3_width : 2;
    /* ddr3_width - Bits[9:8], RW_LB, default = 2'b00 
       00 - x4
       01 - x8
       10 - x16
       11 - reserved
       
       Used to determine if a configuration is capable of supporting DDDC.
     */
    UINT32 rsvd_10 : 2;
    /* rsvd_10 - Bits[11:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_cnt : 2;
    /* rank_cnt - Bits[13:12], RW_LB, default = 2'b00 
       00 - SR
       01 - DR
       10 - QR
       11 - reserved
     */
    UINT32 dimm_pop : 1;
    /* dimm_pop - Bits[14:14], RW_LB, default = 1'b0 
       DIMM populated if set; otherwise, unpopulated.
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rank_disable : 4;
    /* rank_disable - Bits[19:16], RW_LB, default = 4'b0000 
       RANK Disable Control to disable refresh and ZQCAL operation. This bit setting 
       must be set consistently with TERM_RNK_MSK, i.e. both corresponding bits cannot 
       be set at the same time. In the other word, a disabled rank must not be selected 
       for the terminaton rank. 
       
       RANK_DISABLE[3], i.e. bit 19: rank 3 disable. Note DIMMMTR_2.RANK_DISABLE[3] is 
       don't care since DIMM 2 must not be quad-rank 
       RANK_DISABLE[2], i.e. bit 18: rank 2 disable. Note DIMMMTR_2.RANK_DISABLE[2] is 
       don't care since DIMM 2 must not be quad-rank 
       RANK_DISABLE[1], i.e. bit 17: rank 1 disable
       RANK_DISABLE[0], i.e. bit 16: rank 0 disable
       
       when set, no refresh will be perform on this rank. ODT termination is not 
       affected by this bit. 
       Note that patrols are disabled by dimm*_pat_rank_disable of amap register now 
       and not affected by this bit field. 
     */
    UINT32 rsvd_20 : 3;
    /* rsvd_20 - Bits[22:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ddr4_3dsnumranks_cs : 2;
    /* ddr4_3dsnumranks_cs - Bits[24:23], RW_LB, default = 2'b00  */
    UINT32 rsvd_25 : 7;
    /* rsvd_25 - Bits[31:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DIMMMTR_2_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* AMAP_MCDDC_DESC_REG supported on:                                          */
/*       HSX (0x4019A08C)                                                     */
/*       BDX (0x4019A08C)                                                     */
/* Register default value:              0x00000000                            */
#define AMAP_MCDDC_DESC_REG 0x0C02408C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * Address map variations. If all ranks behind an MC are disabled via AMAP, patrol 
 * scrub must be disabled via scrubctl.scrub_en=0 on that MC. 
 */
typedef union {
  struct {
    UINT32 hsxplus : 1;
    /* hsxplus - Bits[0:0], RW_LB, default = 1'b0 
       If set to 1, a different address map will be used for ddr4 open page mode. 
       Setting this to 1 provides the address map called "secondary map". 
     */
    UINT32 rsvd_1 : 14;
    /* rsvd_1 - Bits[14:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 dimm0_pat_rank_disable : 4;
    /* dimm0_pat_rank_disable - Bits[18:15], RW_LB, default = 4'b0000 
       This field is similar to dimmmtr_*.rank_disable; but it affects only patrol 
       operations. 
       Each bit controls 1 Chip_select's patrol in a given dimm. If 1, patrol engine 
       will skip that ChipSelect; other wise it will patrol it if dimm is populted and 
       all other patrol conditions are true. This is intended to be used after 
       rank_sparing to skip patrol of the spared rank. 
       It is invalid to disable patrol scrub on all ranks behind an MC via 
       amap.dimm*_pat_rank_disable fields. 
     */
    UINT32 dimm1_pat_rank_disable : 4;
    /* dimm1_pat_rank_disable - Bits[22:19], RW_LB, default = 4'b0000 
       This field is similar to dimmmtr_*.rank_disable; but it affects only patrol 
       operations. 
       Each bit controls 1 Chip_select's patrol in a given dimm. If 1, patrol engine 
       will skip that ChipSelect; other wise it will patrol it if dimm is populted and 
       all other patrol conditions are true. This is intended to be used after 
       rank_sparing to skip patrol of the spared rank. 
       It is invalid to disable patrol scrub on all ranks behind an MC via 
       amap.dimm*_pat_rank_disable fields. 
     */
    UINT32 dimm2_pat_rank_disable : 4;
    /* dimm2_pat_rank_disable - Bits[26:23], RW_LB, default = 4'b0000 
       This field is similar to dimmmtr_*.rank_disable; but it affects only patrol 
       operations. 
       Each bit controls 1 Chip_select's patrol in a given dimm. If 1, patrol engine 
       will skip that ChipSelect; other wise it will patrol it if dimm is populted and 
       all other patrol conditions are true. This is intended to be used after 
       rank_sparing to skip patrol of the spared rank. 
       It is invalid to disable patrol scrub on all ranks behind an MC via 
       amap.dimm*_pat_rank_disable fields. 
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} AMAP_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TADCHNILVOFFSET_0_MCDDC_DESC_REG supported on:                             */
/*       IVT_EP (0x4017A090)                                                  */
/*       IVT_EX (0x4017A090)                                                  */
/*       HSX (0x4019A090)                                                     */
/*       BDX (0x4019A090)                                                     */
/* Register default value:              0x00000000                            */
#define TADCHNILVOFFSET_0_MCDDC_DESC_REG 0x0C024090


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x090
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tad_offset : 20;
    /* tad_offset - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       channel interleave 0 offset, i.e. CHANNELOFFSET[45:26] == channel interleave i 
       offset, 64MB granularity . 
     */
    UINT32 rsvd_26 : 2;
    /* rsvd_26 - Bits[27:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 chn_idx_offset : 2;
    /* chn_idx_offset - Bits[29:28], RW_LB, default = 2'b00 
       Reverse Address Translation Channel Index OffsetBIOS programmed this field by 
       calculating: 
       (TAD[N].BASE / TAD[N].TAD_SKT_WAY) % TAD[N].TAD_CH_WAY
       where % is the modulo function.
       CHN_IDX_OFFSET can have a value of 0, 1, or 2
       In this equation, the BASE is the lowest address in the TAD range. The 
       TAD_SKT_WAY is 1, 2, 4, or 8, and TAD_CH_WAY is 1, 2, 3, or 4. CHN_IDX_OFFSET 
       will always end up being zero if TAD_CH_WAY is not equal to 3. If TAD_CH_WAY is 
       3, CHN_IDX_OFFSET can be 0, 1, or 2. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} TADCHNILVOFFSET_0_MCDDC_DESC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* TADCHNILVOFFSET_1_MCDDC_DESC_REG supported on:                             */
/*       IVT_EP (0x4017A094)                                                  */
/*       IVT_EX (0x4017A094)                                                  */
/*       HSX (0x4019A094)                                                     */
/*       BDX (0x4019A094)                                                     */
/* Register default value:              0x00000000                            */
#define TADCHNILVOFFSET_1_MCDDC_DESC_REG 0x0C024094


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x094
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tad_offset : 20;
    /* tad_offset - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       channel interleave 0 offset, i.e. CHANNELOFFSET[45:26] == channel interleave i 
       offset, 64MB granularity . 
     */
    UINT32 rsvd_26 : 2;
    /* rsvd_26 - Bits[27:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 chn_idx_offset : 2;
    /* chn_idx_offset - Bits[29:28], RW_LB, default = 2'b00 
       Reverse Address Translation Channel Index OffsetBIOS programmed this field by 
       calculating: 
       (TAD[N].BASE / TAD[N].TAD_SKT_WAY) % TAD[N].TAD_CH_WAY
       where % is the modulo function.
       CHN_IDX_OFFSET can have a value of 0, 1, or 2
       In this equation, the BASE is the lowest address in the TAD range. The 
       TAD_SKT_WAY is 1, 2, 4, or 8, and TAD_CH_WAY is 1, 2, 3, or 4. CHN_IDX_OFFSET 
       will always end up being zero if TAD_CH_WAY is not equal to 3. If TAD_CH_WAY is 
       3, CHN_IDX_OFFSET can be 0, 1, or 2. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} TADCHNILVOFFSET_1_MCDDC_DESC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* TADCHNILVOFFSET_2_MCDDC_DESC_REG supported on:                             */
/*       IVT_EP (0x4017A098)                                                  */
/*       IVT_EX (0x4017A098)                                                  */
/*       HSX (0x4019A098)                                                     */
/*       BDX (0x4019A098)                                                     */
/* Register default value:              0x00000000                            */
#define TADCHNILVOFFSET_2_MCDDC_DESC_REG 0x0C024098


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x098
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tad_offset : 20;
    /* tad_offset - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       channel interleave 0 offset, i.e. CHANNELOFFSET[45:26] == channel interleave i 
       offset, 64MB granularity . 
     */
    UINT32 rsvd_26 : 2;
    /* rsvd_26 - Bits[27:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 chn_idx_offset : 2;
    /* chn_idx_offset - Bits[29:28], RW_LB, default = 2'b00 
       Reverse Address Translation Channel Index OffsetBIOS programmed this field by 
       calculating: 
       (TAD[N].BASE / TAD[N].TAD_SKT_WAY) % TAD[N].TAD_CH_WAY
       where % is the modulo function.
       CHN_IDX_OFFSET can have a value of 0, 1, or 2
       In this equation, the BASE is the lowest address in the TAD range. The 
       TAD_SKT_WAY is 1, 2, 4, or 8, and TAD_CH_WAY is 1, 2, 3, or 4. CHN_IDX_OFFSET 
       will always end up being zero if TAD_CH_WAY is not equal to 3. If TAD_CH_WAY is 
       3, CHN_IDX_OFFSET can be 0, 1, or 2. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} TADCHNILVOFFSET_2_MCDDC_DESC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* TADCHNILVOFFSET_3_MCDDC_DESC_REG supported on:                             */
/*       IVT_EP (0x4017A09C)                                                  */
/*       IVT_EX (0x4017A09C)                                                  */
/*       HSX (0x4019A09C)                                                     */
/*       BDX (0x4019A09C)                                                     */
/* Register default value:              0x00000000                            */
#define TADCHNILVOFFSET_3_MCDDC_DESC_REG 0x0C02409C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x09c
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tad_offset : 20;
    /* tad_offset - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       channel interleave 0 offset, i.e. CHANNELOFFSET[45:26] == channel interleave i 
       offset, 64MB granularity . 
     */
    UINT32 rsvd_26 : 2;
    /* rsvd_26 - Bits[27:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 chn_idx_offset : 2;
    /* chn_idx_offset - Bits[29:28], RW_LB, default = 2'b00 
       Reverse Address Translation Channel Index OffsetBIOS programmed this field by 
       calculating: 
       (TAD[N].BASE / TAD[N].TAD_SKT_WAY) % TAD[N].TAD_CH_WAY
       where % is the modulo function.
       CHN_IDX_OFFSET can have a value of 0, 1, or 2
       In this equation, the BASE is the lowest address in the TAD range. The 
       TAD_SKT_WAY is 1, 2, 4, or 8, and TAD_CH_WAY is 1, 2, 3, or 4. CHN_IDX_OFFSET 
       will always end up being zero if TAD_CH_WAY is not equal to 3. If TAD_CH_WAY is 
       3, CHN_IDX_OFFSET can be 0, 1, or 2. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} TADCHNILVOFFSET_3_MCDDC_DESC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* TADCHNILVOFFSET_4_MCDDC_DESC_REG supported on:                             */
/*       IVT_EP (0x4017A0A0)                                                  */
/*       IVT_EX (0x4017A0A0)                                                  */
/*       HSX (0x4019A0A0)                                                     */
/*       BDX (0x4019A0A0)                                                     */
/* Register default value:              0x00000000                            */
#define TADCHNILVOFFSET_4_MCDDC_DESC_REG 0x0C0240A0


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x0a0
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tad_offset : 20;
    /* tad_offset - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       channel interleave 0 offset, i.e. CHANNELOFFSET[45:26] == channel interleave i 
       offset, 64MB granularity . 
     */
    UINT32 rsvd_26 : 2;
    /* rsvd_26 - Bits[27:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 chn_idx_offset : 2;
    /* chn_idx_offset - Bits[29:28], RW_LB, default = 2'b00 
       Reverse Address Translation Channel Index OffsetBIOS programmed this field by 
       calculating: 
       (TAD[N].BASE / TAD[N].TAD_SKT_WAY) % TAD[N].TAD_CH_WAY
       where % is the modulo function.
       CHN_IDX_OFFSET can have a value of 0, 1, or 2
       In this equation, the BASE is the lowest address in the TAD range. The 
       TAD_SKT_WAY is 1, 2, 4, or 8, and TAD_CH_WAY is 1, 2, 3, or 4. CHN_IDX_OFFSET 
       will always end up being zero if TAD_CH_WAY is not equal to 3. If TAD_CH_WAY is 
       3, CHN_IDX_OFFSET can be 0, 1, or 2. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} TADCHNILVOFFSET_4_MCDDC_DESC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* TADCHNILVOFFSET_5_MCDDC_DESC_REG supported on:                             */
/*       IVT_EP (0x4017A0A4)                                                  */
/*       IVT_EX (0x4017A0A4)                                                  */
/*       HSX (0x4019A0A4)                                                     */
/*       BDX (0x4019A0A4)                                                     */
/* Register default value:              0x00000000                            */
#define TADCHNILVOFFSET_5_MCDDC_DESC_REG 0x0C0240A4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x0a4
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tad_offset : 20;
    /* tad_offset - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       channel interleave 0 offset, i.e. CHANNELOFFSET[45:26] == channel interleave i 
       offset, 64MB granularity . 
     */
    UINT32 rsvd_26 : 2;
    /* rsvd_26 - Bits[27:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 chn_idx_offset : 2;
    /* chn_idx_offset - Bits[29:28], RW_LB, default = 2'b00 
       Reverse Address Translation Channel Index OffsetBIOS programmed this field by 
       calculating: 
       (TAD[N].BASE / TAD[N].TAD_SKT_WAY) % TAD[N].TAD_CH_WAY
       where % is the modulo function.
       CHN_IDX_OFFSET can have a value of 0, 1, or 2
       In this equation, the BASE is the lowest address in the TAD range. The 
       TAD_SKT_WAY is 1, 2, 4, or 8, and TAD_CH_WAY is 1, 2, 3, or 4. CHN_IDX_OFFSET 
       will always end up being zero if TAD_CH_WAY is not equal to 3. If TAD_CH_WAY is 
       3, CHN_IDX_OFFSET can be 0, 1, or 2. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} TADCHNILVOFFSET_5_MCDDC_DESC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* TADCHNILVOFFSET_6_MCDDC_DESC_REG supported on:                             */
/*       IVT_EP (0x4017A0A8)                                                  */
/*       IVT_EX (0x4017A0A8)                                                  */
/*       HSX (0x4019A0A8)                                                     */
/*       BDX (0x4019A0A8)                                                     */
/* Register default value:              0x00000000                            */
#define TADCHNILVOFFSET_6_MCDDC_DESC_REG 0x0C0240A8


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x0a8
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tad_offset : 20;
    /* tad_offset - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       channel interleave 0 offset, i.e. CHANNELOFFSET[45:26] == channel interleave i 
       offset, 64MB granularity . 
     */
    UINT32 rsvd_26 : 2;
    /* rsvd_26 - Bits[27:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 chn_idx_offset : 2;
    /* chn_idx_offset - Bits[29:28], RW_LB, default = 2'b00 
       Reverse Address Translation Channel Index OffsetBIOS programmed this field by 
       calculating: 
       (TAD[N].BASE / TAD[N].TAD_SKT_WAY) % TAD[N].TAD_CH_WAY
       where % is the modulo function.
       CHN_IDX_OFFSET can have a value of 0, 1, or 2
       In this equation, the BASE is the lowest address in the TAD range. The 
       TAD_SKT_WAY is 1, 2, 4, or 8, and TAD_CH_WAY is 1, 2, 3, or 4. CHN_IDX_OFFSET 
       will always end up being zero if TAD_CH_WAY is not equal to 3. If TAD_CH_WAY is 
       3, CHN_IDX_OFFSET can be 0, 1, or 2. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} TADCHNILVOFFSET_6_MCDDC_DESC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* TADCHNILVOFFSET_7_MCDDC_DESC_REG supported on:                             */
/*       IVT_EP (0x4017A0AC)                                                  */
/*       IVT_EX (0x4017A0AC)                                                  */
/*       HSX (0x4019A0AC)                                                     */
/*       BDX (0x4019A0AC)                                                     */
/* Register default value:              0x00000000                            */
#define TADCHNILVOFFSET_7_MCDDC_DESC_REG 0x0C0240AC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x0ac
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tad_offset : 20;
    /* tad_offset - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       channel interleave 0 offset, i.e. CHANNELOFFSET[45:26] == channel interleave i 
       offset, 64MB granularity . 
     */
    UINT32 rsvd_26 : 2;
    /* rsvd_26 - Bits[27:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 chn_idx_offset : 2;
    /* chn_idx_offset - Bits[29:28], RW_LB, default = 2'b00 
       Reverse Address Translation Channel Index OffsetBIOS programmed this field by 
       calculating: 
       (TAD[N].BASE / TAD[N].TAD_SKT_WAY) % TAD[N].TAD_CH_WAY
       where % is the modulo function.
       CHN_IDX_OFFSET can have a value of 0, 1, or 2
       In this equation, the BASE is the lowest address in the TAD range. The 
       TAD_SKT_WAY is 1, 2, 4, or 8, and TAD_CH_WAY is 1, 2, 3, or 4. CHN_IDX_OFFSET 
       will always end up being zero if TAD_CH_WAY is not equal to 3. If TAD_CH_WAY is 
       3, CHN_IDX_OFFSET can be 0, 1, or 2. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} TADCHNILVOFFSET_7_MCDDC_DESC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* TADCHNILVOFFSET_8_MCDDC_DESC_REG supported on:                             */
/*       IVT_EP (0x4017A0B0)                                                  */
/*       IVT_EX (0x4017A0B0)                                                  */
/*       HSX (0x4019A0B0)                                                     */
/*       BDX (0x4019A0B0)                                                     */
/* Register default value:              0x00000000                            */
#define TADCHNILVOFFSET_8_MCDDC_DESC_REG 0x0C0240B0


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x0b0
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tad_offset : 20;
    /* tad_offset - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       channel interleave 0 offset, i.e. CHANNELOFFSET[45:26] == channel interleave i 
       offset, 64MB granularity . 
     */
    UINT32 rsvd_26 : 2;
    /* rsvd_26 - Bits[27:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 chn_idx_offset : 2;
    /* chn_idx_offset - Bits[29:28], RW_LB, default = 2'b00 
       Reverse Address Translation Channel Index OffsetBIOS programmed this field by 
       calculating: 
       (TAD[N].BASE / TAD[N].TAD_SKT_WAY) % TAD[N].TAD_CH_WAY
       where % is the modulo function.
       CHN_IDX_OFFSET can have a value of 0, 1, or 2
       In this equation, the BASE is the lowest address in the TAD range. The 
       TAD_SKT_WAY is 1, 2, 4, or 8, and TAD_CH_WAY is 1, 2, 3, or 4. CHN_IDX_OFFSET 
       will always end up being zero if TAD_CH_WAY is not equal to 3. If TAD_CH_WAY is 
       3, CHN_IDX_OFFSET can be 0, 1, or 2. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} TADCHNILVOFFSET_8_MCDDC_DESC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* TADCHNILVOFFSET_9_MCDDC_DESC_REG supported on:                             */
/*       IVT_EP (0x4017A0B4)                                                  */
/*       IVT_EX (0x4017A0B4)                                                  */
/*       HSX (0x4019A0B4)                                                     */
/*       BDX (0x4019A0B4)                                                     */
/* Register default value:              0x00000000                            */
#define TADCHNILVOFFSET_9_MCDDC_DESC_REG 0x0C0240B4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x0b4
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tad_offset : 20;
    /* tad_offset - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       channel interleave 0 offset, i.e. CHANNELOFFSET[45:26] == channel interleave i 
       offset, 64MB granularity . 
     */
    UINT32 rsvd_26 : 2;
    /* rsvd_26 - Bits[27:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 chn_idx_offset : 2;
    /* chn_idx_offset - Bits[29:28], RW_LB, default = 2'b00 
       Reverse Address Translation Channel Index OffsetBIOS programmed this field by 
       calculating: 
       (TAD[N].BASE / TAD[N].TAD_SKT_WAY) % TAD[N].TAD_CH_WAY
       where % is the modulo function.
       CHN_IDX_OFFSET can have a value of 0, 1, or 2
       In this equation, the BASE is the lowest address in the TAD range. The 
       TAD_SKT_WAY is 1, 2, 4, or 8, and TAD_CH_WAY is 1, 2, 3, or 4. CHN_IDX_OFFSET 
       will always end up being zero if TAD_CH_WAY is not equal to 3. If TAD_CH_WAY is 
       3, CHN_IDX_OFFSET can be 0, 1, or 2. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} TADCHNILVOFFSET_9_MCDDC_DESC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* TADCHNILVOFFSET_10_MCDDC_DESC_REG supported on:                            */
/*       IVT_EP (0x4017A0B8)                                                  */
/*       IVT_EX (0x4017A0B8)                                                  */
/*       HSX (0x4019A0B8)                                                     */
/*       BDX (0x4019A0B8)                                                     */
/* Register default value:              0x00000000                            */
#define TADCHNILVOFFSET_10_MCDDC_DESC_REG 0x0C0240B8


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x0b8
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tad_offset : 20;
    /* tad_offset - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       channel interleave 0 offset, i.e. CHANNELOFFSET[45:26] == channel interleave i 
       offset, 64MB granularity . 
     */
    UINT32 rsvd_26 : 2;
    /* rsvd_26 - Bits[27:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 chn_idx_offset : 2;
    /* chn_idx_offset - Bits[29:28], RW_LB, default = 2'b00 
       Reverse Address Translation Channel Index OffsetBIOS programmed this field by 
       calculating: 
       (TAD[N].BASE / TAD[N].TAD_SKT_WAY) % TAD[N].TAD_CH_WAY
       where % is the modulo function.
       CHN_IDX_OFFSET can have a value of 0, 1, or 2
       In this equation, the BASE is the lowest address in the TAD range. The 
       TAD_SKT_WAY is 1, 2, 4, or 8, and TAD_CH_WAY is 1, 2, 3, or 4. CHN_IDX_OFFSET 
       will always end up being zero if TAD_CH_WAY is not equal to 3. If TAD_CH_WAY is 
       3, CHN_IDX_OFFSET can be 0, 1, or 2. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} TADCHNILVOFFSET_10_MCDDC_DESC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* TADCHNILVOFFSET_11_MCDDC_DESC_REG supported on:                            */
/*       IVT_EP (0x4017A0BC)                                                  */
/*       IVT_EX (0x4017A0BC)                                                  */
/*       HSX (0x4019A0BC)                                                     */
/*       BDX (0x4019A0BC)                                                     */
/* Register default value:              0x00000000                            */
#define TADCHNILVOFFSET_11_MCDDC_DESC_REG 0x0C0240BC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x0bc
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tad_offset : 20;
    /* tad_offset - Bits[25:6], RW_LB, default = 20'b00000000000000000000 
       channel interleave 0 offset, i.e. CHANNELOFFSET[45:26] == channel interleave i 
       offset, 64MB granularity . 
     */
    UINT32 rsvd_26 : 2;
    /* rsvd_26 - Bits[27:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 chn_idx_offset : 2;
    /* chn_idx_offset - Bits[29:28], RW_LB, default = 2'b00 
       Reverse Address Translation Channel Index OffsetBIOS programmed this field by 
       calculating: 
       (TAD[N].BASE / TAD[N].TAD_SKT_WAY) % TAD[N].TAD_CH_WAY
       where % is the modulo function.
       CHN_IDX_OFFSET can have a value of 0, 1, or 2
       In this equation, the BASE is the lowest address in the TAD range. The 
       TAD_SKT_WAY is 1, 2, 4, or 8, and TAD_CH_WAY is 1, 2, 3, or 4. CHN_IDX_OFFSET 
       will always end up being zero if TAD_CH_WAY is not equal to 3. If TAD_CH_WAY is 
       3, CHN_IDX_OFFSET can be 0, 1, or 2. 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} TADCHNILVOFFSET_11_MCDDC_DESC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PXPENHCAP_MCDDC_DESC_REG supported on:                                     */
/*       IVT_EP (0x4017A100)                                                  */
/*       IVT_EX (0x4017A100)                                                  */
/*       HSX (0x4019A100)                                                     */
/*       BDX (0x4019A100)                                                     */
/* Register default value:              0x00000000                            */
#define PXPENHCAP_MCDDC_DESC_REG 0x0C024100
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
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
} PXPENHCAP_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* RIRWAYNESSLIMIT_0_MCDDC_DESC_REG supported on:                             */
/*       IVT_EP (0x4017A108)                                                  */
/*       IVT_EX (0x4017A108)                                                  */
/*       HSX (0x4019A108)                                                     */
/*       BDX (0x4019A108)                                                     */
/* Register default value:              0x00000000                            */
#define RIRWAYNESSLIMIT_0_MCDDC_DESC_REG 0x0C024108


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * There are total of 5 RIR ranges (represents how many rank interleave ranges 
 * supported to cover customer DIMM configuration.). 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    /* rsvd_0 - Bits[0:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_limit : 11;
    /* rir_limit - Bits[11:1], RW_LB, default = 11'b00000000000 
       RIR[4:0].LIMIT[39:29] == highest address of the range in channel address space, 
       384GB in lock-step/192GB in independent channel, 512MB granularity. M= How many 
       rank interleave ranges supported to cover customer DIMM configuration. In 
       Haswell Server M=5. 
     */
    UINT32 rsvd_12 : 16;
    /* rsvd_12 - Bits[27:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_way : 2;
    /* rir_way - Bits[29:28], RW_LB, default = 2'b00 
       rank interleave wayness
       00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 rsvd_30 : 1;
    /* rsvd_30 - Bits[30:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_val : 1;
    /* rir_val - Bits[31:31], RW_LB, default = 1'b0 
       Range Valid when set; otherwise, invalid
     */
  } Bits;
  UINT32 Data;
} RIRWAYNESSLIMIT_0_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * There are total of 5 RIR ranges (represents how many rank interleave ranges 
 * supported to cover customer DIMM configuration.). 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    /* rsvd_0 - Bits[0:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_limit : 13;
    /* rir_limit - Bits[13:1], RW_LB, default = 13'b0000000000000 
       RIR[4:0].LIMIT[41:29] == highest address of the range in channel address space, 
       3072GB in lock-step/1536GB in independent channel, 512MB granularity. M= How 
       many rank interleave ranges supported to cover customer DIMM configuration. In 
       HSX M=5. 
     */
    UINT32 rsvd_14 : 14;
    /* rsvd_14 - Bits[27:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_way : 2;
    /* rir_way - Bits[29:28], RW_LB, default = 2'b00 
       rank interleave wayness
       00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 rsvd_30 : 1;
    /* rsvd_30 - Bits[30:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_val : 1;
    /* rir_val - Bits[31:31], RW_LB, default = 1'b0 
       Range Valid when set; otherwise, invalid
     */
  } Bits;
  UINT32 Data;
} RIRWAYNESSLIMIT_0_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRWAYNESSLIMIT_1_MCDDC_DESC_REG supported on:                             */
/*       IVT_EP (0x4017A10C)                                                  */
/*       IVT_EX (0x4017A10C)                                                  */
/*       HSX (0x4019A10C)                                                     */
/*       BDX (0x4019A10C)                                                     */
/* Register default value:              0x00000000                            */
#define RIRWAYNESSLIMIT_1_MCDDC_DESC_REG 0x0C02410C


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * There are total of 6 RIR ranges (represents how many rank interleave ranges 
 * supported to cover customer DIMM configuration.). 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    /* rsvd_0 - Bits[0:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_limit : 11;
    /* rir_limit - Bits[11:1], RW_LB, default = 11'b00000000000 
       RIR[4:0].LIMIT[39:29] == highest address of the range in channel address space, 
       384GB in lock-step/192GB in independent channel, 512MB granularity. M= How many 
       rank interleave ranges supported to cover customer DIMM configuration. In 
       Haswell Server M=5. 
     */
    UINT32 rsvd_12 : 16;
    /* rsvd_12 - Bits[27:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_way : 2;
    /* rir_way - Bits[29:28], RW_LB, default = 2'b00 
       rank interleave wayness00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 rsvd_30 : 1;
    /* rsvd_30 - Bits[30:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_val : 1;
    /* rir_val - Bits[31:31], RW_LB, default = 1'b0 
       Range Valid when set; otherwise, invalid
     */
  } Bits;
  UINT32 Data;
} RIRWAYNESSLIMIT_1_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * There are total of 6 RIR ranges (represents how many rank interleave ranges 
 * supported to cover customer DIMM configuration.). 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    /* rsvd_0 - Bits[0:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_limit : 13;
    /* rir_limit - Bits[13:1], RW_LB, default = 13'b0000000000000 
       RIR[4:0].LIMIT[41:29] == highest address of the range in channel address space, 
       3072GB in lock-step/1536GB in independent channel, 512MB granularity. M= How 
       many rank interleave ranges supported to cover customer DIMM configuration. In 
       HSX M=5. 
     */
    UINT32 rsvd_14 : 14;
    /* rsvd_14 - Bits[27:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_way : 2;
    /* rir_way - Bits[29:28], RW_LB, default = 2'b00 
       rank interleave wayness00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 rsvd_30 : 1;
    /* rsvd_30 - Bits[30:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_val : 1;
    /* rir_val - Bits[31:31], RW_LB, default = 1'b0 
       Range Valid when set; otherwise, invalid
     */
  } Bits;
  UINT32 Data;
} RIRWAYNESSLIMIT_1_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRWAYNESSLIMIT_2_MCDDC_DESC_REG supported on:                             */
/*       IVT_EP (0x4017A110)                                                  */
/*       IVT_EX (0x4017A110)                                                  */
/*       HSX (0x4019A110)                                                     */
/*       BDX (0x4019A110)                                                     */
/* Register default value:              0x00000000                            */
#define RIRWAYNESSLIMIT_2_MCDDC_DESC_REG 0x0C024110


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * There are total of 6 RIR ranges (represents how many rank interleave ranges 
 * supported to cover customer DIMM configuration.). 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    /* rsvd_0 - Bits[0:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_limit : 11;
    /* rir_limit - Bits[11:1], RW_LB, default = 11'b00000000000 
       RIR[4:0].LIMIT[39:29] == highest address of the range in channel address space, 
       384GB in lock-step/192GB in independent channel, 512MB granularity. M= How many 
       rank interleave ranges supported to cover customer DIMM configuration. In 
       Haswell Server M=5. 
     */
    UINT32 rsvd_12 : 16;
    /* rsvd_12 - Bits[27:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_way : 2;
    /* rir_way - Bits[29:28], RW_LB, default = 2'b00 
       rank interleave wayness00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 rsvd_30 : 1;
    /* rsvd_30 - Bits[30:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_val : 1;
    /* rir_val - Bits[31:31], RW_LB, default = 1'b0 
       Range Valid when set; otherwise, invalid
     */
  } Bits;
  UINT32 Data;
} RIRWAYNESSLIMIT_2_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * There are total of 6 RIR ranges (represents how many rank interleave ranges 
 * supported to cover customer DIMM configuration.). 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    /* rsvd_0 - Bits[0:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_limit : 13;
    /* rir_limit - Bits[13:1], RW_LB, default = 13'b0000000000000 
       RIR[4:0].LIMIT[41:29] == highest address of the range in channel address space, 
       3072GB in lock-step/1536GB in independent channel, 512MB granularity. M= How 
       many rank interleave ranges supported to cover customer DIMM configuration. In 
       HSX M=5. 
     */
    UINT32 rsvd_14 : 14;
    /* rsvd_14 - Bits[27:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_way : 2;
    /* rir_way - Bits[29:28], RW_LB, default = 2'b00 
       rank interleave wayness00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 rsvd_30 : 1;
    /* rsvd_30 - Bits[30:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_val : 1;
    /* rir_val - Bits[31:31], RW_LB, default = 1'b0 
       Range Valid when set; otherwise, invalid
     */
  } Bits;
  UINT32 Data;
} RIRWAYNESSLIMIT_2_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRWAYNESSLIMIT_3_MCDDC_DESC_REG supported on:                             */
/*       IVT_EP (0x4017A114)                                                  */
/*       IVT_EX (0x4017A114)                                                  */
/*       HSX (0x4019A114)                                                     */
/*       BDX (0x4019A114)                                                     */
/* Register default value:              0x00000000                            */
#define RIRWAYNESSLIMIT_3_MCDDC_DESC_REG 0x0C024114


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * There are total of 6 RIR ranges (represents how many rank interleave ranges 
 * supported to cover customer DIMM configuration.). 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    /* rsvd_0 - Bits[0:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_limit : 11;
    /* rir_limit - Bits[11:1], RW_LB, default = 11'b00000000000 
       RIR[4:0].LIMIT[39:29] == highest address of the range in channel address space, 
       384GB in lock-step/192GB in independent channel, 512MB granularity. M= How many 
       rank interleave ranges supported to cover customer DIMM configuration. In 
       Haswell Server M=5. 
     */
    UINT32 rsvd_12 : 16;
    /* rsvd_12 - Bits[27:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_way : 2;
    /* rir_way - Bits[29:28], RW_LB, default = 2'b00 
       rank interleave wayness00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 rsvd_30 : 1;
    /* rsvd_30 - Bits[30:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_val : 1;
    /* rir_val - Bits[31:31], RW_LB, default = 1'b0 
       Range Valid when set; otherwise, invalid
     */
  } Bits;
  UINT32 Data;
} RIRWAYNESSLIMIT_3_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * There are total of 6 RIR ranges (represents how many rank interleave ranges 
 * supported to cover customer DIMM configuration.). 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    /* rsvd_0 - Bits[0:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_limit : 13;
    /* rir_limit - Bits[13:1], RW_LB, default = 13'b0000000000000 
       RIR[4:0].LIMIT[41:29] == highest address of the range in channel address space, 
       3072GB in lock-step/1536GB in independent channel, 512MB granularity. M= How 
       many rank interleave ranges supported to cover customer DIMM configuration. In 
       HSX M=5. 
     */
    UINT32 rsvd_14 : 14;
    /* rsvd_14 - Bits[27:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_way : 2;
    /* rir_way - Bits[29:28], RW_LB, default = 2'b00 
       rank interleave wayness00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 rsvd_30 : 1;
    /* rsvd_30 - Bits[30:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_val : 1;
    /* rir_val - Bits[31:31], RW_LB, default = 1'b0 
       Range Valid when set; otherwise, invalid
     */
  } Bits;
  UINT32 Data;
} RIRWAYNESSLIMIT_3_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRWAYNESSLIMIT_4_MCDDC_DESC_REG supported on:                             */
/*       IVT_EP (0x4017A118)                                                  */
/*       IVT_EX (0x4017A118)                                                  */
/*       HSX (0x4019A118)                                                     */
/*       BDX (0x4019A118)                                                     */
/* Register default value:              0x00000000                            */
#define RIRWAYNESSLIMIT_4_MCDDC_DESC_REG 0x0C024118


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * There are total of 6 RIR ranges (represents how many rank interleave ranges 
 * supported to cover customer DIMM configuration.). 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    /* rsvd_0 - Bits[0:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_limit : 11;
    /* rir_limit - Bits[11:1], RW_LB, default = 11'b00000000000 
       RIR[4:0].LIMIT[39:29] == highest address of the range in channel address space, 
       384GB in lock-step/192GB in independent channel, 512MB granularity. M= How many 
       rank interleave ranges supported to cover customer DIMM configuration. In 
       Haswell Server M=5. 
     */
    UINT32 rsvd_12 : 16;
    /* rsvd_12 - Bits[27:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_way : 2;
    /* rir_way - Bits[29:28], RW_LB, default = 2'b00 
       rank interleave wayness00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 rsvd_30 : 1;
    /* rsvd_30 - Bits[30:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_val : 1;
    /* rir_val - Bits[31:31], RW_LB, default = 1'b0 
       Range Valid when set; otherwise, invalid
     */
  } Bits;
  UINT32 Data;
} RIRWAYNESSLIMIT_4_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * There are total of 6 RIR ranges (represents how many rank interleave ranges 
 * supported to cover customer DIMM configuration.). 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    /* rsvd_0 - Bits[0:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_limit : 13;
    /* rir_limit - Bits[13:1], RW_LB, default = 13'b0000000000000 
       RIR[4:0].LIMIT[41:29] == highest address of the range in channel address space, 
       3072GB in lock-step/1536GB in independent channel, 512MB granularity. M= How 
       many rank interleave ranges supported to cover customer DIMM configuration. In 
       HSX M=5. 
     */
    UINT32 rsvd_14 : 14;
    /* rsvd_14 - Bits[27:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_way : 2;
    /* rir_way - Bits[29:28], RW_LB, default = 2'b00 
       rank interleave wayness00 = 1 way,
       01 = 2 way,
       10 = 4 way,
       11 = 8 way.
     */
    UINT32 rsvd_30 : 1;
    /* rsvd_30 - Bits[30:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_val : 1;
    /* rir_val - Bits[31:31], RW_LB, default = 1'b0 
       Range Valid when set; otherwise, invalid
     */
  } Bits;
  UINT32 Data;
} RIRWAYNESSLIMIT_4_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV0OFFSET_0_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A120)                                                  */
/*       IVT_EX (0x4017A120)                                                  */
/*       HSX (0x4019A120)                                                     */
/*       BDX (0x4019A120)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV0OFFSET_0_MCDDC_DESC_REG 0x0C024120


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x120
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset0 : 14;
    /* rir_offset0 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET0[39:26] == rank interleave 0 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt0 : 4;
    /* rir_rnk_tgt0 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 0 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV0OFFSET_0_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x120
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset0 : 16;
    /* rir_offset0 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET0[41:26] == rank interleave 0 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt0 : 4;
    /* rir_rnk_tgt0 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 0 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV0OFFSET_0_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV1OFFSET_0_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A124)                                                  */
/*       IVT_EX (0x4017A124)                                                  */
/*       HSX (0x4019A124)                                                     */
/*       BDX (0x4019A124)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV1OFFSET_0_MCDDC_DESC_REG 0x0C024124


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x124
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset1 : 14;
    /* rir_offset1 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET1[39:26] == rank interleave 1 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt1 : 4;
    /* rir_rnk_tgt1 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 1 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV1OFFSET_0_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x124
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset1 : 16;
    /* rir_offset1 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET1[41:26] == rank interleave 1 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt1 : 4;
    /* rir_rnk_tgt1 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 1 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV1OFFSET_0_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV2OFFSET_0_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A128)                                                  */
/*       IVT_EX (0x4017A128)                                                  */
/*       HSX (0x4019A128)                                                     */
/*       BDX (0x4019A128)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV2OFFSET_0_MCDDC_DESC_REG 0x0C024128


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x128
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset2 : 14;
    /* rir_offset2 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET2[39:26] == rank interleave 2 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt2 : 4;
    /* rir_rnk_tgt2 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 2 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV2OFFSET_0_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x128
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset2 : 16;
    /* rir_offset2 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET2[41:26] == rank interleave 2 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt2 : 4;
    /* rir_rnk_tgt2 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 2 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV2OFFSET_0_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV3OFFSET_0_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A12C)                                                  */
/*       IVT_EX (0x4017A12C)                                                  */
/*       HSX (0x4019A12C)                                                     */
/*       BDX (0x4019A12C)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV3OFFSET_0_MCDDC_DESC_REG 0x0C02412C


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x12c
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset3 : 14;
    /* rir_offset3 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET3[39:26] == rank interleave 3 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt3 : 4;
    /* rir_rnk_tgt3 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 3 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV3OFFSET_0_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x12c
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset3 : 16;
    /* rir_offset3 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET3[41:26] == rank interleave 3 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt3 : 4;
    /* rir_rnk_tgt3 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 3 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV3OFFSET_0_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV4OFFSET_0_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A130)                                                  */
/*       IVT_EX (0x4017A130)                                                  */
/*       HSX (0x4019A130)                                                     */
/*       BDX (0x4019A130)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV4OFFSET_0_MCDDC_DESC_REG 0x0C024130


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x130
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset4 : 14;
    /* rir_offset4 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET4[39:26] == rank interleave 4 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt4 : 4;
    /* rir_rnk_tgt4 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 4 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV4OFFSET_0_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x130
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset4 : 16;
    /* rir_offset4 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET4[41:26] == rank interleave 4 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt4 : 4;
    /* rir_rnk_tgt4 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 4 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV4OFFSET_0_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV5OFFSET_0_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A134)                                                  */
/*       IVT_EX (0x4017A134)                                                  */
/*       HSX (0x4019A134)                                                     */
/*       BDX (0x4019A134)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV5OFFSET_0_MCDDC_DESC_REG 0x0C024134


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x134
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset5 : 14;
    /* rir_offset5 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET5[39:26] == rank interleave 5 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt5 : 4;
    /* rir_rnk_tgt5 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 5 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV5OFFSET_0_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x134
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset5 : 16;
    /* rir_offset5 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET5[41:26] == rank interleave 5 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt5 : 4;
    /* rir_rnk_tgt5 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 5 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV5OFFSET_0_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV6OFFSET_0_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A138)                                                  */
/*       IVT_EX (0x4017A138)                                                  */
/*       HSX (0x4019A138)                                                     */
/*       BDX (0x4019A138)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV6OFFSET_0_MCDDC_DESC_REG 0x0C024138


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x138
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset6 : 14;
    /* rir_offset6 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET6[39:26] == rank interleave 6 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt6 : 4;
    /* rir_rnk_tgt6 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 6 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV6OFFSET_0_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x138
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset6 : 16;
    /* rir_offset6 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET6[41:26] == rank interleave 6 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt6 : 4;
    /* rir_rnk_tgt6 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 6 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV6OFFSET_0_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV7OFFSET_0_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A13C)                                                  */
/*       IVT_EX (0x4017A13C)                                                  */
/*       HSX (0x4019A13C)                                                     */
/*       BDX (0x4019A13C)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV7OFFSET_0_MCDDC_DESC_REG 0x0C02413C


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x13c
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset7 : 14;
    /* rir_offset7 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET7[39:26] == rank interleave 0 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt7 : 4;
    /* rir_rnk_tgt7 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 7 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV7OFFSET_0_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x13c
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset7 : 16;
    /* rir_offset7 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET7[41:26] == rank interleave 0 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt7 : 4;
    /* rir_rnk_tgt7 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 7 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV7OFFSET_0_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV0OFFSET_1_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A140)                                                  */
/*       IVT_EX (0x4017A140)                                                  */
/*       HSX (0x4019A140)                                                     */
/*       BDX (0x4019A140)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV0OFFSET_1_MCDDC_DESC_REG 0x0C024140


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x140
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset0 : 14;
    /* rir_offset0 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET0[39:26] == rank interleave 0 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt0 : 4;
    /* rir_rnk_tgt0 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 0 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV0OFFSET_1_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x140
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset0 : 16;
    /* rir_offset0 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET0[41:26] == rank interleave 0 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt0 : 4;
    /* rir_rnk_tgt0 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 0 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV0OFFSET_1_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV1OFFSET_1_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A144)                                                  */
/*       IVT_EX (0x4017A144)                                                  */
/*       HSX (0x4019A144)                                                     */
/*       BDX (0x4019A144)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV1OFFSET_1_MCDDC_DESC_REG 0x0C024144


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x144
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset1 : 14;
    /* rir_offset1 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET1[39:26] == rank interleave 1 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt1 : 4;
    /* rir_rnk_tgt1 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 1 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV1OFFSET_1_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x144
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset1 : 16;
    /* rir_offset1 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET1[41:26] == rank interleave 1 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt1 : 4;
    /* rir_rnk_tgt1 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 1 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV1OFFSET_1_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV2OFFSET_1_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A148)                                                  */
/*       IVT_EX (0x4017A148)                                                  */
/*       HSX (0x4019A148)                                                     */
/*       BDX (0x4019A148)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV2OFFSET_1_MCDDC_DESC_REG 0x0C024148


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x148
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset2 : 14;
    /* rir_offset2 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET2[39:26] == rank interleave 2 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt2 : 4;
    /* rir_rnk_tgt2 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 2 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV2OFFSET_1_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x148
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset2 : 16;
    /* rir_offset2 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET2[41:26] == rank interleave 2 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt2 : 4;
    /* rir_rnk_tgt2 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 2 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV2OFFSET_1_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV3OFFSET_1_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A14C)                                                  */
/*       IVT_EX (0x4017A14C)                                                  */
/*       HSX (0x4019A14C)                                                     */
/*       BDX (0x4019A14C)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV3OFFSET_1_MCDDC_DESC_REG 0x0C02414C


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x14c
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset3 : 14;
    /* rir_offset3 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET3[39:26] == rank interleave 3 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt3 : 4;
    /* rir_rnk_tgt3 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 3 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV3OFFSET_1_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x14c
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset3 : 16;
    /* rir_offset3 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET3[41:26] == rank interleave 3 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt3 : 4;
    /* rir_rnk_tgt3 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 3 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV3OFFSET_1_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV4OFFSET_1_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A150)                                                  */
/*       IVT_EX (0x4017A150)                                                  */
/*       HSX (0x4019A150)                                                     */
/*       BDX (0x4019A150)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV4OFFSET_1_MCDDC_DESC_REG 0x0C024150


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x150
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset4 : 14;
    /* rir_offset4 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET4[39:26] == rank interleave 4 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt4 : 4;
    /* rir_rnk_tgt4 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 4 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV4OFFSET_1_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x150
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset4 : 16;
    /* rir_offset4 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET4[41:26] == rank interleave 4 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt4 : 4;
    /* rir_rnk_tgt4 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 4 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV4OFFSET_1_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV5OFFSET_1_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A154)                                                  */
/*       IVT_EX (0x4017A154)                                                  */
/*       HSX (0x4019A154)                                                     */
/*       BDX (0x4019A154)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV5OFFSET_1_MCDDC_DESC_REG 0x0C024154


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x154
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset5 : 14;
    /* rir_offset5 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET5[39:26] == rank interleave 5 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt5 : 4;
    /* rir_rnk_tgt5 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 5 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV5OFFSET_1_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x154
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset5 : 16;
    /* rir_offset5 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET5[41:26] == rank interleave 5 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt5 : 4;
    /* rir_rnk_tgt5 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 5 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV5OFFSET_1_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV6OFFSET_1_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A158)                                                  */
/*       IVT_EX (0x4017A158)                                                  */
/*       HSX (0x4019A158)                                                     */
/*       BDX (0x4019A158)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV6OFFSET_1_MCDDC_DESC_REG 0x0C024158


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x158
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset6 : 14;
    /* rir_offset6 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET6[39:26] == rank interleave 6 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt6 : 4;
    /* rir_rnk_tgt6 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 6 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV6OFFSET_1_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x158
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset6 : 16;
    /* rir_offset6 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET6[41:26] == rank interleave 6 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt6 : 4;
    /* rir_rnk_tgt6 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 6 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV6OFFSET_1_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV7OFFSET_1_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A15C)                                                  */
/*       IVT_EX (0x4017A15C)                                                  */
/*       HSX (0x4019A15C)                                                     */
/*       BDX (0x4019A15C)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV7OFFSET_1_MCDDC_DESC_REG 0x0C02415C


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x15c
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset7 : 14;
    /* rir_offset7 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET7[39:26] == rank interleave 0 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt7 : 4;
    /* rir_rnk_tgt7 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 7 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV7OFFSET_1_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x15c
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset7 : 16;
    /* rir_offset7 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET7[41:26] == rank interleave 0 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt7 : 4;
    /* rir_rnk_tgt7 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 7 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV7OFFSET_1_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV0OFFSET_2_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A160)                                                  */
/*       IVT_EX (0x4017A160)                                                  */
/*       HSX (0x4019A160)                                                     */
/*       BDX (0x4019A160)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV0OFFSET_2_MCDDC_DESC_REG 0x0C024160


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x160
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset0 : 14;
    /* rir_offset0 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET0[39:26] == rank interleave 0 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt0 : 4;
    /* rir_rnk_tgt0 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 0 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV0OFFSET_2_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x160
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset0 : 16;
    /* rir_offset0 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET0[41:26] == rank interleave 0 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt0 : 4;
    /* rir_rnk_tgt0 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 0 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV0OFFSET_2_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV1OFFSET_2_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A164)                                                  */
/*       IVT_EX (0x4017A164)                                                  */
/*       HSX (0x4019A164)                                                     */
/*       BDX (0x4019A164)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV1OFFSET_2_MCDDC_DESC_REG 0x0C024164


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x164
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset1 : 14;
    /* rir_offset1 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET1[39:26] == rank interleave 1 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt1 : 4;
    /* rir_rnk_tgt1 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 1 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV1OFFSET_2_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x164
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset1 : 16;
    /* rir_offset1 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET1[41:26] == rank interleave 1 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt1 : 4;
    /* rir_rnk_tgt1 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 1 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV1OFFSET_2_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV2OFFSET_2_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A168)                                                  */
/*       IVT_EX (0x4017A168)                                                  */
/*       HSX (0x4019A168)                                                     */
/*       BDX (0x4019A168)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV2OFFSET_2_MCDDC_DESC_REG 0x0C024168


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x168
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset2 : 14;
    /* rir_offset2 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET2[39:26] == rank interleave 2 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt2 : 4;
    /* rir_rnk_tgt2 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 2 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV2OFFSET_2_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x168
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset2 : 16;
    /* rir_offset2 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET2[41:26] == rank interleave 2 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt2 : 4;
    /* rir_rnk_tgt2 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 2 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV2OFFSET_2_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV3OFFSET_2_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A16C)                                                  */
/*       IVT_EX (0x4017A16C)                                                  */
/*       HSX (0x4019A16C)                                                     */
/*       BDX (0x4019A16C)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV3OFFSET_2_MCDDC_DESC_REG 0x0C02416C


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x16c
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset3 : 14;
    /* rir_offset3 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET3[39:26] == rank interleave 3 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt3 : 4;
    /* rir_rnk_tgt3 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 3 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV3OFFSET_2_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x16c
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset3 : 16;
    /* rir_offset3 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET3[41:26] == rank interleave 3 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt3 : 4;
    /* rir_rnk_tgt3 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 3 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV3OFFSET_2_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV4OFFSET_2_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A170)                                                  */
/*       IVT_EX (0x4017A170)                                                  */
/*       HSX (0x4019A170)                                                     */
/*       BDX (0x4019A170)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV4OFFSET_2_MCDDC_DESC_REG 0x0C024170


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x170
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset4 : 14;
    /* rir_offset4 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET4[39:26] == rank interleave 4 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt4 : 4;
    /* rir_rnk_tgt4 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 4 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV4OFFSET_2_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x170
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset4 : 16;
    /* rir_offset4 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET4[41:26] == rank interleave 4 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt4 : 4;
    /* rir_rnk_tgt4 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 4 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV4OFFSET_2_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV5OFFSET_2_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A174)                                                  */
/*       IVT_EX (0x4017A174)                                                  */
/*       HSX (0x4019A174)                                                     */
/*       BDX (0x4019A174)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV5OFFSET_2_MCDDC_DESC_REG 0x0C024174


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x174
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset5 : 14;
    /* rir_offset5 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET5[39:26] == rank interleave 5 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt5 : 4;
    /* rir_rnk_tgt5 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 5 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV5OFFSET_2_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x174
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset5 : 16;
    /* rir_offset5 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET5[41:26] == rank interleave 5 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt5 : 4;
    /* rir_rnk_tgt5 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 5 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV5OFFSET_2_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV6OFFSET_2_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A178)                                                  */
/*       IVT_EX (0x4017A178)                                                  */
/*       HSX (0x4019A178)                                                     */
/*       BDX (0x4019A178)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV6OFFSET_2_MCDDC_DESC_REG 0x0C024178


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x178
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset6 : 14;
    /* rir_offset6 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET6[39:26] == rank interleave 6 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt6 : 4;
    /* rir_rnk_tgt6 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 6 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV6OFFSET_2_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x178
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset6 : 16;
    /* rir_offset6 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET6[41:26] == rank interleave 6 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt6 : 4;
    /* rir_rnk_tgt6 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 6 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV6OFFSET_2_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV7OFFSET_2_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A17C)                                                  */
/*       IVT_EX (0x4017A17C)                                                  */
/*       HSX (0x4019A17C)                                                     */
/*       BDX (0x4019A17C)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV7OFFSET_2_MCDDC_DESC_REG 0x0C02417C


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x17c
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset7 : 14;
    /* rir_offset7 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET7[39:26] == rank interleave 0 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt7 : 4;
    /* rir_rnk_tgt7 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 7 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV7OFFSET_2_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x17c
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset7 : 16;
    /* rir_offset7 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET7[41:26] == rank interleave 0 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt7 : 4;
    /* rir_rnk_tgt7 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 7 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV7OFFSET_2_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV0OFFSET_3_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A180)                                                  */
/*       IVT_EX (0x4017A180)                                                  */
/*       HSX (0x4019A180)                                                     */
/*       BDX (0x4019A180)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV0OFFSET_3_MCDDC_DESC_REG 0x0C024180


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x180
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset0 : 14;
    /* rir_offset0 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET0[39:26] == rank interleave 0 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt0 : 4;
    /* rir_rnk_tgt0 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 0 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV0OFFSET_3_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x180
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset0 : 16;
    /* rir_offset0 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET0[41:26] == rank interleave 0 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt0 : 4;
    /* rir_rnk_tgt0 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 0 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV0OFFSET_3_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV1OFFSET_3_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A184)                                                  */
/*       IVT_EX (0x4017A184)                                                  */
/*       HSX (0x4019A184)                                                     */
/*       BDX (0x4019A184)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV1OFFSET_3_MCDDC_DESC_REG 0x0C024184


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x184
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset1 : 14;
    /* rir_offset1 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET1[39:26] == rank interleave 1 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt1 : 4;
    /* rir_rnk_tgt1 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 1 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV1OFFSET_3_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x184
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset1 : 16;
    /* rir_offset1 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET1[41:26] == rank interleave 1 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt1 : 4;
    /* rir_rnk_tgt1 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 1 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV1OFFSET_3_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV2OFFSET_3_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A188)                                                  */
/*       IVT_EX (0x4017A188)                                                  */
/*       HSX (0x4019A188)                                                     */
/*       BDX (0x4019A188)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV2OFFSET_3_MCDDC_DESC_REG 0x0C024188


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x188
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset2 : 14;
    /* rir_offset2 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET2[39:26] == rank interleave 2 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt2 : 4;
    /* rir_rnk_tgt2 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 2 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV2OFFSET_3_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x188
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset2 : 16;
    /* rir_offset2 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET2[41:26] == rank interleave 2 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt2 : 4;
    /* rir_rnk_tgt2 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 2 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV2OFFSET_3_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV3OFFSET_3_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A18C)                                                  */
/*       IVT_EX (0x4017A18C)                                                  */
/*       HSX (0x4019A18C)                                                     */
/*       BDX (0x4019A18C)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV3OFFSET_3_MCDDC_DESC_REG 0x0C02418C


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x18c
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset3 : 14;
    /* rir_offset3 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET3[39:26] == rank interleave 3 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt3 : 4;
    /* rir_rnk_tgt3 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 3 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV3OFFSET_3_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x18c
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset3 : 16;
    /* rir_offset3 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET3[41:26] == rank interleave 3 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt3 : 4;
    /* rir_rnk_tgt3 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 3 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV3OFFSET_3_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV4OFFSET_3_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A190)                                                  */
/*       IVT_EX (0x4017A190)                                                  */
/*       HSX (0x4019A190)                                                     */
/*       BDX (0x4019A190)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV4OFFSET_3_MCDDC_DESC_REG 0x0C024190


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x190
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset4 : 14;
    /* rir_offset4 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET4[39:26] == rank interleave 4 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt4 : 4;
    /* rir_rnk_tgt4 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 4 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV4OFFSET_3_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x190
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset4 : 16;
    /* rir_offset4 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET4[41:26] == rank interleave 4 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt4 : 4;
    /* rir_rnk_tgt4 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 4 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV4OFFSET_3_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV5OFFSET_3_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A194)                                                  */
/*       IVT_EX (0x4017A194)                                                  */
/*       HSX (0x4019A194)                                                     */
/*       BDX (0x4019A194)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV5OFFSET_3_MCDDC_DESC_REG 0x0C024194


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x194
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset5 : 14;
    /* rir_offset5 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET5[39:26] == rank interleave 5 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt5 : 4;
    /* rir_rnk_tgt5 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 5 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV5OFFSET_3_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x194
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset5 : 16;
    /* rir_offset5 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET5[41:26] == rank interleave 5 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt5 : 4;
    /* rir_rnk_tgt5 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 5 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV5OFFSET_3_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV6OFFSET_3_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A198)                                                  */
/*       IVT_EX (0x4017A198)                                                  */
/*       HSX (0x4019A198)                                                     */
/*       BDX (0x4019A198)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV6OFFSET_3_MCDDC_DESC_REG 0x0C024198


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x198
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset6 : 14;
    /* rir_offset6 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET6[39:26] == rank interleave 6 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt6 : 4;
    /* rir_rnk_tgt6 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 6 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV6OFFSET_3_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x198
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset6 : 16;
    /* rir_offset6 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET6[41:26] == rank interleave 6 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt6 : 4;
    /* rir_rnk_tgt6 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 6 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV6OFFSET_3_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV7OFFSET_3_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A19C)                                                  */
/*       IVT_EX (0x4017A19C)                                                  */
/*       HSX (0x4019A19C)                                                     */
/*       BDX (0x4019A19C)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV7OFFSET_3_MCDDC_DESC_REG 0x0C02419C


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x19c
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset7 : 14;
    /* rir_offset7 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET7[39:26] == rank interleave 0 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt7 : 4;
    /* rir_rnk_tgt7 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 7 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV7OFFSET_3_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x19c
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset7 : 16;
    /* rir_offset7 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET7[41:26] == rank interleave 0 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt7 : 4;
    /* rir_rnk_tgt7 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 7 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV7OFFSET_3_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV0OFFSET_4_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A1A0)                                                  */
/*       IVT_EX (0x4017A1A0)                                                  */
/*       HSX (0x4019A1A0)                                                     */
/*       BDX (0x4019A1A0)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV0OFFSET_4_MCDDC_DESC_REG 0x0C0241A0


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x1a0
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset0 : 14;
    /* rir_offset0 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET0[39:26] == rank interleave 0 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt0 : 4;
    /* rir_rnk_tgt0 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 0 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV0OFFSET_4_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x1a0
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset0 : 16;
    /* rir_offset0 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET0[41:26] == rank interleave 0 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt0 : 4;
    /* rir_rnk_tgt0 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 0 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV0OFFSET_4_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV1OFFSET_4_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A1A4)                                                  */
/*       IVT_EX (0x4017A1A4)                                                  */
/*       HSX (0x4019A1A4)                                                     */
/*       BDX (0x4019A1A4)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV1OFFSET_4_MCDDC_DESC_REG 0x0C0241A4


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x1a4
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset1 : 14;
    /* rir_offset1 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET1[39:26] == rank interleave 1 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt1 : 4;
    /* rir_rnk_tgt1 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 1 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV1OFFSET_4_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x1a4
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset1 : 16;
    /* rir_offset1 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET1[41:26] == rank interleave 1 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt1 : 4;
    /* rir_rnk_tgt1 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 1 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV1OFFSET_4_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV2OFFSET_4_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A1A8)                                                  */
/*       IVT_EX (0x4017A1A8)                                                  */
/*       HSX (0x4019A1A8)                                                     */
/*       BDX (0x4019A1A8)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV2OFFSET_4_MCDDC_DESC_REG 0x0C0241A8


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x1a8
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset2 : 14;
    /* rir_offset2 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET2[39:26] == rank interleave 2 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt2 : 4;
    /* rir_rnk_tgt2 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 2 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV2OFFSET_4_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x1a8
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset2 : 16;
    /* rir_offset2 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET2[41:26] == rank interleave 2 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt2 : 4;
    /* rir_rnk_tgt2 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 2 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV2OFFSET_4_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV3OFFSET_4_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A1AC)                                                  */
/*       IVT_EX (0x4017A1AC)                                                  */
/*       HSX (0x4019A1AC)                                                     */
/*       BDX (0x4019A1AC)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV3OFFSET_4_MCDDC_DESC_REG 0x0C0241AC


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x1ac
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset3 : 14;
    /* rir_offset3 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET3[39:26] == rank interleave 3 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt3 : 4;
    /* rir_rnk_tgt3 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 3 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV3OFFSET_4_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x1ac
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset3 : 16;
    /* rir_offset3 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET3[41:26] == rank interleave 3 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt3 : 4;
    /* rir_rnk_tgt3 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 3 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV3OFFSET_4_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV4OFFSET_4_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A1B0)                                                  */
/*       IVT_EX (0x4017A1B0)                                                  */
/*       HSX (0x4019A1B0)                                                     */
/*       BDX (0x4019A1B0)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV4OFFSET_4_MCDDC_DESC_REG 0x0C0241B0


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x1b0
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset4 : 14;
    /* rir_offset4 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET4[39:26] == rank interleave 4 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt4 : 4;
    /* rir_rnk_tgt4 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 4 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV4OFFSET_4_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x1b0
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset4 : 16;
    /* rir_offset4 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET4[41:26] == rank interleave 4 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt4 : 4;
    /* rir_rnk_tgt4 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 4 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV4OFFSET_4_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV5OFFSET_4_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A1B4)                                                  */
/*       IVT_EX (0x4017A1B4)                                                  */
/*       HSX (0x4019A1B4)                                                     */
/*       BDX (0x4019A1B4)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV5OFFSET_4_MCDDC_DESC_REG 0x0C0241B4


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x1b4
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset5 : 14;
    /* rir_offset5 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET5[39:26] == rank interleave 5 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt5 : 4;
    /* rir_rnk_tgt5 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 5 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV5OFFSET_4_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x1b4
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset5 : 16;
    /* rir_offset5 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET5[41:26] == rank interleave 5 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt5 : 4;
    /* rir_rnk_tgt5 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 5 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV5OFFSET_4_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV6OFFSET_4_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A1B8)                                                  */
/*       IVT_EX (0x4017A1B8)                                                  */
/*       HSX (0x4019A1B8)                                                     */
/*       BDX (0x4019A1B8)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV6OFFSET_4_MCDDC_DESC_REG 0x0C0241B8


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x1b8
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset6 : 14;
    /* rir_offset6 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET6[39:26] == rank interleave 6 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt6 : 4;
    /* rir_rnk_tgt6 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 6 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV6OFFSET_4_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x1b8
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset6 : 16;
    /* rir_offset6 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET6[41:26] == rank interleave 6 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt6 : 4;
    /* rir_rnk_tgt6 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 6 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV6OFFSET_4_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RIRILV7OFFSET_4_MCDDC_DESC_REG supported on:                               */
/*       IVT_EP (0x4017A1BC)                                                  */
/*       IVT_EX (0x4017A1BC)                                                  */
/*       HSX (0x4019A1BC)                                                     */
/*       BDX (0x4019A1BC)                                                     */
/* Register default value:              0x00000000                            */
#define RIRILV7OFFSET_4_MCDDC_DESC_REG 0x0C0241BC


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x1bc
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset7 : 14;
    /* rir_offset7 - Bits[15:2], RW_LB, default = 14'b00000000000000 
       RIR[5:0].RANKOFFSET7[39:26] == rank interleave 0 offset, 64MB granularity (The 
       processors minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rir_rnk_tgt7 : 4;
    /* rir_rnk_tgt7 - Bits[19:16], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 7 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV7OFFSET_4_MCDDC_DESC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * generated by critter 19_2_0x1bc
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_offset7 : 16;
    /* rir_offset7 - Bits[17:2], RW_LB, default = 16'b0000000000000000 
       RIR[4:0].RANKOFFSET7[41:26] == rank interleave 0 offset, 64MB granularity 
       (Jaketown's minimum rank size is 512MB. 512MB/8 interleave = 64MB per 8-way 
       interleave.) 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rir_rnk_tgt7 : 4;
    /* rir_rnk_tgt7 - Bits[23:20], RW_LB, default = 4'b0000 
       target rank ID for rank interleave 7 (used for 1/2/4/8-way RIR interleaving).
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RIRILV7OFFSET_4_MCDDC_DESC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RSP_FUNC_ADDR_MATCH_LO_MCDDC_DESC_REG supported on:                        */
/*       IVT_EP (0x4017A1C0)                                                  */
/*       IVT_EX (0x4017A1C0)                                                  */
/*       HSX (0x4019A1C0)                                                     */
/*       BDX (0x4019A1C0)                                                     */
/* Register default value:              0x00000000                            */
#define RSP_FUNC_ADDR_MATCH_LO_MCDDC_DESC_REG 0x0C0241C0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * Complete address match (Addr[45:3]) and mask is supported for all HA writes. The 
 * error injection logic uses the address match mask logic output to determine 
 * which memory writes need to get error injection. Users can program up to two x4 
 * device masks (8-bits per chunk - 64 bits per cacheline). In Lockstep mode, only 
 * one device can be programmed in each lockstep channel. 
 * Note that since rsp_func_addr_match and rsp_func_addr_mask registers are applied 
 * to HA addresses, use of  these registers while sparing or scrubs are turned on 
 * will produce unpredictable results and is not supported. 
 */
typedef union {
  struct {
    UINT32 addr_match_lower : 32;
    /* addr_match_lower - Bits[31:0], RWS, default = 32'b00000000000000000000000000000000 
       Addr Match Lower: 32-bits (Match Addr[34:3])
     */
  } Bits;
  UINT32 Data;
} RSP_FUNC_ADDR_MATCH_LO_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* RSP_FUNC_ADDR_MATCH_HI_MCDDC_DESC_REG supported on:                        */
/*       IVT_EP (0x4017A1C4)                                                  */
/*       IVT_EX (0x4017A1C4)                                                  */
/*       HSX (0x4019A1C4)                                                     */
/*       BDX (0x4019A1C4)                                                     */
/* Register default value:              0x00000000                            */
#define RSP_FUNC_ADDR_MATCH_HI_MCDDC_DESC_REG 0x0C0241C4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * Complete address match (Addr[45:3]) and mask is supported for all HA writes. The 
 * error injection logic uses the address match mask logic output to determine 
 * which memory writes need to get error injection. Users can program up to two x4 
 * device masks (8-bits per chunk - 64 bits per cacheline). In Lockstep mode, only 
 * one device can be programmed in each lockstep channel. 
 * Note that since rsp_func_addr_match and rsp_func_addr_mask registers are applied 
 * to HA addresses, use of  these registers while sparing or scrubs are turned on 
 * will produce unpredictable results and is not supported. 
 */
typedef union {
  struct {
    UINT32 addr_match_higher : 11;
    /* addr_match_higher - Bits[10:0], RWS, default = 11'b00000000000 
       Addr Match Higher : 11-Bits (Match Addr[45:35])
     */
    UINT32 rsp_func_addr_match_en : 1;
    /* rsp_func_addr_match_en - Bits[11:11], RWS_LV, default = 1'b0 
       Enabling the Address Match Response Function when set.
     */
    UINT32 rsp_func_addr_match_always : 1;
    /* rsp_func_addr_match_always - Bits[12:12], RWS, default = 1'b0 
       The enable bit RSP_FUNC_ADDR_MATCH_EN will not be self cleared after match if 
       this field is set. Continuous address matching. 
     */
    UINT32 rsvd_13 : 3;
    /* rsvd_13 - Bits[15:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_16 : 8;
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RSP_FUNC_ADDR_MATCH_HI_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* RSP_FUNC_ADDR_MASK_LO_MCDDC_DESC_REG supported on:                         */
/*       IVT_EP (0x4017A1C8)                                                  */
/*       IVT_EX (0x4017A1C8)                                                  */
/*       HSX (0x4019A1C8)                                                     */
/*       BDX (0x4019A1C8)                                                     */
/* Register default value:              0x00000000                            */
#define RSP_FUNC_ADDR_MASK_LO_MCDDC_DESC_REG 0x0C0241C8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * Complete address match (Addr[45:3]) and mask is supported for all HA writes. 
 * Error injection does not use the response logic triggers and uses the match mask 
 * logic output to determine which writes need to get error injection. Users can 
 * program up to two x4 device masks (8-bits per chunk - 64 bits per cacheline). In 
 * Lockstep mode, only one device can be programmed in each lockstep channel. 
 */
typedef union {
  struct {
    UINT32 addr_mask_lower : 32;
    /* addr_mask_lower - Bits[31:0], RWS, default = 32'b00000000000000000000000000000000 
       Address Mask to deselect (when set) the corresponding Addr[34:3] for the address 
       match. 
     */
  } Bits;
  UINT32 Data;
} RSP_FUNC_ADDR_MASK_LO_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* RSP_FUNC_ADDR_MASK_HI_MCDDC_DESC_REG supported on:                         */
/*       IVT_EP (0x4017A1CC)                                                  */
/*       IVT_EX (0x4017A1CC)                                                  */
/*       HSX (0x4019A1CC)                                                     */
/*       BDX (0x4019A1CC)                                                     */
/* Register default value:              0x00000000                            */
#define RSP_FUNC_ADDR_MASK_HI_MCDDC_DESC_REG 0x0C0241CC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * Complete address match (Addr[45:3]) and mask is supported for all HA writes. 
 * Error injection does not use the response logic triggers and uses the match mask 
 * logic output to determine which writes need to get error injection. Users can 
 * program up to two x4 device masks (8-bits per chunk - 64 bits per cacheline). In 
 * Lockstep mode, only one device can be programmed in each lockstep channel. 
 * Note that since rsp_func_addr_match and rsp_func_addr_mask registers are applied 
 * to HA addresses, use of  these registers while sparing or scrubs are turned on 
 * will produce unpredictable results and is not supported. 
 */
typedef union {
  struct {
    UINT32 addr_mask_higher : 11;
    /* addr_mask_higher - Bits[10:0], RWS, default = 11'b00000000000 
       Address Mask to deselect (when set) the corresponding Addr[45:35] for the 
       address match. 
     */
    UINT32 rsvd_11 : 5;
    /* rsvd_11 - Bits[15:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_16 : 8;
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RSP_FUNC_ADDR_MASK_HI_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */




/* VMSE_WRITE_PUSH_ECC_MCDDC_DESC_REG supported on:                           */
/*       IVT_EP (0x4017A200)                                                  */
/*       IVT_EX (0x4017A200)                                                  */
/*       HSX (0x4019A200)                                                     */
/*       BDX (0x4019A200)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_WRITE_PUSH_ECC_MCDDC_DESC_REG 0x0C024200
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * The ecc mode bits to use for a write push or read CAS for each rank of this 
 * channel. (EX processor only) 
 */
typedef union {
  struct {
    UINT32 rank0_ecc_mode : 2;
    /* rank0_ecc_mode - Bits[1:0], RW_LB, default = 2'b00 
       The ecc mode bits to use for a write push or read CAS to rank 0.
     */
    UINT32 rank1_ecc_mode : 2;
    /* rank1_ecc_mode - Bits[3:2], RW_LB, default = 2'b00 
       The ecc mode bits to use for a write push or read CAS to rank 1.
     */
    UINT32 rank2_ecc_mode : 2;
    /* rank2_ecc_mode - Bits[5:4], RW_LB, default = 2'b00 
       The ecc mode bits to use for a write push or read CAS to rank 2.
     */
    UINT32 rank3_ecc_mode : 2;
    /* rank3_ecc_mode - Bits[7:6], RW_LB, default = 2'b00 
       The ecc mode bits to use for a write push or read CAS to rank 3.
     */
    UINT32 rank4_ecc_mode : 2;
    /* rank4_ecc_mode - Bits[9:8], RW_LB, default = 2'b00 
       The ecc mode bits to use for a write push or read CAS to rank 4.
     */
    UINT32 rank5_ecc_mode : 2;
    /* rank5_ecc_mode - Bits[11:10], RW_LB, default = 2'b00 
       The ecc mode bits to use for a write push or read CAS to rank 5.
     */
    UINT32 rank6_ecc_mode : 2;
    /* rank6_ecc_mode - Bits[13:12], RW_LB, default = 2'b00 
       The ecc mode bits to use for a write push or read CAS to rank 6.
     */
    UINT32 rank7_ecc_mode : 2;
    /* rank7_ecc_mode - Bits[15:14], RW_LB, default = 2'b00 
       The ecc mode bits to use for a write push or read CAS to rank 7.
     */
    UINT32 sparing_ecc_mode : 2;
    /* sparing_ecc_mode - Bits[17:16], RW_LB, default = 2'b00 
       The ecc mode bits to use during the dddc device sparing flow. This field is 
       unused for this channels 1/3. 
     */
    UINT32 rsvd : 14;
    /* rsvd - Bits[31:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} VMSE_WRITE_PUSH_ECC_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */


/* VMSE_WRITE_PUSH_ECC_BANK0_MCDDC_DESC_REG supported on:                     */
/*       HSX (0x2019A204)                                                     */
/*       BDX (0x2019A204)                                                     */
/* Register default value:              0x0000                                */
#define VMSE_WRITE_PUSH_ECC_BANK0_MCDDC_DESC_REG 0x0C022204

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * support up to 2 unique bank ECC modes per rank, this is for bank 0, first bank
 */
typedef union {
  struct {
    UINT16 rank0 : 2;
    /* rank0 - Bits[1:0], RW_LB, default = 2'b00  */
    UINT16 rank1 : 2;
    /* rank1 - Bits[3:2], RW_LB, default = 2'b00  */
    UINT16 rank2 : 2;
    /* rank2 - Bits[5:4], RW_LB, default = 2'b00  */
    UINT16 rank3 : 2;
    /* rank3 - Bits[7:6], RW_LB, default = 2'b00  */
    UINT16 rank4 : 2;
    /* rank4 - Bits[9:8], RW_LB, default = 2'b00  */
    UINT16 rank5 : 2;
    /* rank5 - Bits[11:10], RW_LB, default = 2'b00  */
    UINT16 rank6 : 2;
    /* rank6 - Bits[13:12], RW_LB, default = 2'b00  */
    UINT16 rank7 : 2;
    /* rank7 - Bits[15:14], RW_LB, default = 2'b00  */
  } Bits;
  UINT16 Data;
} VMSE_WRITE_PUSH_ECC_BANK0_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* VMSE_WRITE_PUSH_ECC_BANK1_MCDDC_DESC_REG supported on:                     */
/*       HSX (0x2019A206)                                                     */
/*       BDX (0x2019A206)                                                     */
/* Register default value:              0x0000                                */
#define VMSE_WRITE_PUSH_ECC_BANK1_MCDDC_DESC_REG 0x0C022206

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * support up to 2 unique bank ECC modes per rank, this is for bank 1, second bank
 */
typedef union {
  struct {
    UINT16 rank0 : 2;
    /* rank0 - Bits[1:0], RW_LB, default = 2'b00  */
    UINT16 rank1 : 2;
    /* rank1 - Bits[3:2], RW_LB, default = 2'b00  */
    UINT16 rank2 : 2;
    /* rank2 - Bits[5:4], RW_LB, default = 2'b00  */
    UINT16 rank3 : 2;
    /* rank3 - Bits[7:6], RW_LB, default = 2'b00  */
    UINT16 rank4 : 2;
    /* rank4 - Bits[9:8], RW_LB, default = 2'b00  */
    UINT16 rank5 : 2;
    /* rank5 - Bits[11:10], RW_LB, default = 2'b00  */
    UINT16 rank6 : 2;
    /* rank6 - Bits[13:12], RW_LB, default = 2'b00  */
    UINT16 rank7 : 2;
    /* rank7 - Bits[15:14], RW_LB, default = 2'b00  */
  } Bits;
  UINT16 Data;
} VMSE_WRITE_PUSH_ECC_BANK1_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* VMSE_WRITE_PUSH_ECC_BANKV_MCDDC_DESC_REG supported on:                     */
/*       HSX (0x2019A208)                                                     */
/*       BDX (0x2019A208)                                                     */
/* Register default value:              0x0000                                */
#define VMSE_WRITE_PUSH_ECC_BANKV_MCDDC_DESC_REG 0x0C022208

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * Valid bits for the bank0/1 IDs per rank.  If the valid bit is set, then the 
 * corresponding bank-ECC mode is used for the valid bank0/1 ID per rank. 
 * Valid bits are set and cleared automatically by the hardware in response to the 
 * end of different types of sparing events. 
 */
typedef union {
  struct {
    UINT16 rank0bank0 : 1;
    /* rank0bank0 - Bits[0:0], RW_LBV, default = 1'b0 
       per rank, bank valid for bank Y
       specifies if the correspond ecc_mode in bank 0 reg is valid
     */
    UINT16 rank1bank0 : 1;
    /* rank1bank0 - Bits[1:1], RW_LBV, default = 1'b0 
       per rank, bank valid for bank Y
       specifies if the correspond ecc_mode in bank 0 reg is valid
     */
    UINT16 rank2bank0 : 1;
    /* rank2bank0 - Bits[2:2], RW_LBV, default = 1'b0 
       per rank, bank valid for bank Y
       specifies if the correspond ecc_mode in bank 0 reg is valid
     */
    UINT16 rank3bank0 : 1;
    /* rank3bank0 - Bits[3:3], RW_LBV, default = 1'b0 
       per rank, bank valid for bank Y
       specifies if the correspond ecc_mode in bank 0 reg is valid
     */
    UINT16 rank4bank0 : 1;
    /* rank4bank0 - Bits[4:4], RW_LBV, default = 1'b0 
       per rank, bank valid for bank Y
       specifies if the correspond ecc_mode in bank 0 reg is valid
     */
    UINT16 rank5bank0 : 1;
    /* rank5bank0 - Bits[5:5], RW_LBV, default = 1'b0 
       per rank, bank valid for bank Y
       specifies if the correspond ecc_mode in bank 0 reg is valid
     */
    UINT16 rank6bank0 : 1;
    /* rank6bank0 - Bits[6:6], RW_LBV, default = 1'b0 
       per rank, bank valid for bank Y
       specifies if the correspond ecc_mode in bank 0 reg is valid
     */
    UINT16 rank7bank0 : 1;
    /* rank7bank0 - Bits[7:7], RW_LBV, default = 1'b0 
       per rank, bank valid for bank Y
       specifies if the correspond ecc_mode in bank 0 reg is valid
     */
    UINT16 rank0bank1 : 1;
    /* rank0bank1 - Bits[8:8], RW_LBV, default = 1'b0 
       per rank, bank valid for bank Y
       specifies if the correspond ecc_mode in bank 1 reg is valid
     */
    UINT16 rank1bank1 : 1;
    /* rank1bank1 - Bits[9:9], RW_LBV, default = 1'b0 
       per rank, bank valid for bank Y
       specifies if the correspond ecc_mode in bank 1 reg is valid
     */
    UINT16 rank2bank1 : 1;
    /* rank2bank1 - Bits[10:10], RW_LBV, default = 1'b0 
       per rank, bank valid for bank Y
       specifies if the correspond ecc_mode in bank 1 reg is valid
     */
    UINT16 rank3bank1 : 1;
    /* rank3bank1 - Bits[11:11], RW_LBV, default = 1'b0 
       per rank, bank valid for bank Y
       specifies if the correspond ecc_mode in bank 1 reg is valid
     */
    UINT16 rank4bank1 : 1;
    /* rank4bank1 - Bits[12:12], RW_LBV, default = 1'b0 
       per rank, bank valid for bank Y
       specifies if the correspond ecc_mode in bank 1 reg is valid
     */
    UINT16 rank5bank1 : 1;
    /* rank5bank1 - Bits[13:13], RW_LBV, default = 1'b0 
       per rank, bank valid for bank Y
       specifies if the correspond ecc_mode in bank 1 reg is valid
     */
    UINT16 rank6bank1 : 1;
    /* rank6bank1 - Bits[14:14], RW_LBV, default = 1'b0 
       per rank, bank valid for bank Y
       specifies if the correspond ecc_mode in bank 1 reg is valid
     */
    UINT16 rank7bank1 : 1;
    /* rank7bank1 - Bits[15:15], RW_LBV, default = 1'b0 
       per rank, bank valid for bank Y
       specifies if the correspond ecc_mode in bank 1 reg is valid
     */
  } Bits;
  UINT16 Data;
} VMSE_WRITE_PUSH_ECC_BANKV_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* VMSE_WRITE_PUSH_ECC_BANK0_ID_MCDDC_DESC_REG supported on:                  */
/*       HSX (0x4019A20C)                                                     */
/*       BDX (0x4019A20C)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_WRITE_PUSH_ECC_BANK0_ID_MCDDC_DESC_REG 0x0C02420C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * These are the bank ID's for bank 0. Valid with corresponding bit in 
 * vmse_write_push_ecc_bankv  
 * DDR3: This register should be programmed with the Bank Address, range 7-0.
 * DDR4: The Bank ID should be programmed as {Bank Address, Bank Group}
 */
typedef union {
  struct {
    UINT32 rank0 : 4;
    /* rank0 - Bits[3:0], RW_LB, default = 4'b0000  */
    UINT32 rank1 : 4;
    /* rank1 - Bits[7:4], RW_LB, default = 4'b0000  */
    UINT32 rank2 : 4;
    /* rank2 - Bits[11:8], RW_LB, default = 4'b0000  */
    UINT32 rank3 : 4;
    /* rank3 - Bits[15:12], RW_LB, default = 4'b0000  */
    UINT32 rank4 : 4;
    /* rank4 - Bits[19:16], RW_LB, default = 4'b0000  */
    UINT32 rank5 : 4;
    /* rank5 - Bits[23:20], RW_LB, default = 4'b0000  */
    UINT32 rank6 : 4;
    /* rank6 - Bits[27:24], RW_LB, default = 4'b0000  */
    UINT32 rank7 : 4;
    /* rank7 - Bits[31:28], RW_LB, default = 4'b0000  */
  } Bits;
  UINT32 Data;
} VMSE_WRITE_PUSH_ECC_BANK0_ID_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* VMSE_WRITE_PUSH_ECC_BANK1_ID_MCDDC_DESC_REG supported on:                  */
/*       HSX (0x4019A210)                                                     */
/*       BDX (0x4019A210)                                                     */
/* Register default value:              0x00000000                            */
#define VMSE_WRITE_PUSH_ECC_BANK1_ID_MCDDC_DESC_REG 0x0C024210

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * These are the bank ID's for bank 1. Valid with corresponding bit in 
 * vmse_write_push_ecc_bankv  
 * DDR3: This register should be programmed with the Bank Address, range 7-0.
 * DDR4: The Bank ID should be programmed as {Bank Address, Bank Group}
 */
typedef union {
  struct {
    UINT32 rank0 : 4;
    /* rank0 - Bits[3:0], RW_LB, default = 4'b0000  */
    UINT32 rank1 : 4;
    /* rank1 - Bits[7:4], RW_LB, default = 4'b0000  */
    UINT32 rank2 : 4;
    /* rank2 - Bits[11:8], RW_LB, default = 4'b0000  */
    UINT32 rank3 : 4;
    /* rank3 - Bits[15:12], RW_LB, default = 4'b0000  */
    UINT32 rank4 : 4;
    /* rank4 - Bits[19:16], RW_LB, default = 4'b0000  */
    UINT32 rank5 : 4;
    /* rank5 - Bits[23:20], RW_LB, default = 4'b0000  */
    UINT32 rank6 : 4;
    /* rank6 - Bits[27:24], RW_LB, default = 4'b0000  */
    UINT32 rank7 : 4;
    /* rank7 - Bits[31:28], RW_LB, default = 4'b0000  */
  } Bits;
  UINT32 Data;
} VMSE_WRITE_PUSH_ECC_BANK1_ID_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RSP_FUNC_RANK_BANK_MATCH_MCDDC_DESC_REG supported on:                      */
/*       HSX (0x4019A214)                                                     */
/*       BDX (0x4019A214)                                                     */
/* Register default value:              0x00000000                            */
#define RSP_FUNC_RANK_BANK_MATCH_MCDDC_DESC_REG 0x0C024214

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.2.CFG.xml.
 * Enables targetting injection to a particular bank in a particular logical rank.  
 * When enabled, programmed rank and bank will be matched, this is ORed with system 
 * address based match and mask. 
 */
typedef union {
  struct {
    UINT32 bank : 4;
    /* bank - Bits[3:0], RW, default = 4'b0000  */
    UINT32 rank : 3;
    /* rank - Bits[6:4], RW, default = 3'b000  */
    UINT32 rsvd : 24;
    /* rsvd - Bits[30:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 en : 1;
    /* en - Bits[31:31], RW, default = 1'b0 
       Enable rank and bank matching
     */
  } Bits;
  UINT32 Data;
} RSP_FUNC_RANK_BANK_MATCH_MCDDC_DESC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MCDECS_CHICKEN_BITS_MCDDC_DESC_REG supported on:                           */
/*       IVT_EP (0x1017A300)                                                  */
/*       IVT_EX (0x1017A300)                                                  */
/*       HSX (0x1019A300)                                                     */
/*       BDX (0x1019A300)                                                     */
/* Register default value:              0x00                                  */
#define MCDECS_CHICKEN_BITS_MCDDC_DESC_REG 0x0C021300


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.2.CFG.xml.
 * MCDECS_CHICKEN_BITS
 */
typedef union {
  struct {
    UINT8 dis_ck_gate_addr_dec : 1;
    /* dis_ck_gate_addr_dec - Bits[0:0], RW_LB, default = 1'b0 
       Disable clock gating addr_decode
     */
    UINT8 dis_isoch_wr : 1;
    /* dis_isoch_wr - Bits[1:1], RW_LB, default = 1'b0 
       Converts isoch writes to non isoch in decoder
     */
    UINT8 dis_isoch_rd : 1;
    /* dis_isoch_rd - Bits[2:2], RW_LB, default = 1'b0 
       Converts isoch reads to non isoch in decoder
     */
    UINT8 defeature_2 : 1;
    /* defeature_2 - Bits[3:3], RW_LB, default = 1'b0 
       Reserved Unused
     */
    UINT8 defeature_3 : 1;
    /* defeature_3 - Bits[4:4], RW_LB, default = 1'b0 
       Reserved Unused
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} MCDECS_CHICKEN_BITS_MCDDC_DESC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RDLN_CTL_THRESHOLD0_MCDDC_DESC_REG supported on:                           */
/*       IVT_EP (0x4017A310)                                                  */
/*       IVT_EX (0x4017A310)                                                  */
/* Register default value:              0x00000000                            */
#define RDLN_CTL_THRESHOLD0_MCDDC_DESC_REG 0x0C024310



/* RDLN_CTL_THRESHOLD1_MCDDC_DESC_REG supported on:                           */
/*       IVT_EP (0x4017A314)                                                  */
/*       IVT_EX (0x4017A314)                                                  */
/* Register default value:              0x00000000                            */
#define RDLN_CTL_THRESHOLD1_MCDDC_DESC_REG 0x0C024314



/* RDLN_SEED_LO_MCDDC_DESC_REG supported on:                                  */
/*       IVT_EP (0x4017A318)                                                  */
/*       IVT_EX (0x4017A318)                                                  */
/* Register default value:              0x00000000                            */
#define RDLN_SEED_LO_MCDDC_DESC_REG 0x0C024318



/* RDLN_SEED_HI_MCDDC_DESC_REG supported on:                                  */
/*       IVT_EP (0x4017A31C)                                                  */
/*       IVT_EX (0x4017A31C)                                                  */
/* Register default value:              0x00000000                            */
#define RDLN_SEED_HI_MCDDC_DESC_REG 0x0C02431C



/* RDLN_CTL_MCDDC_DESC_REG supported on:                                      */
/*       IVT_EP (0x4017A320)                                                  */
/*       IVT_EX (0x4017A320)                                                  */
/* Register default value:              0x00000000                            */
#define RDLN_CTL_MCDDC_DESC_REG 0x0C024320







/* RDLN_CTL2_MCDDC_DESC_REG supported on:                                     */
/*       IVT_EP (0x4017A32C)                                                  */
/*       IVT_EX (0x4017A32C)                                                  */
/* Register default value:              0x00000000                            */
#define RDLN_CTL2_MCDDC_DESC_REG 0x0C02432C



#endif /* MCDDC_DESC_h */
