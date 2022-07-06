/* Date Stamp: 8/23/2014 */

#ifndef UBOX_MISC_h
#define UBOX_MISC_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* UBOX_MISC_IVT_DEV 11                                                       */
/* UBOX_MISC_IVT_FUN 3                                                        */
/* For HSX_HOST:                                                              */
/* UBOX_MISC_HSX_DEV 16                                                       */
/* UBOX_MISC_HSX_FUN 7                                                        */
/* For BDX_HOST:                                                              */
/* UBOX_MISC_BDX_DEV 16                                                       */
/* UBOX_MISC_BDX_FUN 7                                                        */

/* VID_UBOX_MISC_REG supported on:                                            */
/*       IVT_EP (0x2015B000)                                                  */
/*       IVT_EX (0x2015B000)                                                  */
/*       HSX (0x20187000)                                                     */
/*       BDX (0x20187000)                                                     */
/* Register default value:              0x8086                                */
#define VID_UBOX_MISC_REG 0x08022000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
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
} VID_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* DID_UBOX_MISC_REG supported on:                                            */
/*       IVT_EP (0x2015B002)                                                  */
/*       IVT_EX (0x2015B002)                                                  */
/*       HSX (0x20187002)                                                     */
/*       BDX (0x20187002)                                                     */
/* Register default value on IVT_EP:    0x0E1F                                */
/* Register default value on IVT_EX:    0x0E1F                                */
/* Register default value on HSX:       0x2F1F                                */
/* Register default value on BDX:       0x6F1F                                */
#define DID_UBOX_MISC_REG 0x08022002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6F1F 
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
} DID_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* PCICMD_UBOX_MISC_REG supported on:                                         */
/*       IVT_EP (0x2015B004)                                                  */
/*       IVT_EX (0x2015B004)                                                  */
/*       HSX (0x20187004)                                                     */
/*       BDX (0x20187004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_UBOX_MISC_REG 0x08022004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
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
} PCICMD_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* PCISTS_UBOX_MISC_REG supported on:                                         */
/*       IVT_EP (0x2015B006)                                                  */
/*       IVT_EX (0x2015B006)                                                  */
/*       HSX (0x20187006)                                                     */
/*       BDX (0x20187006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_UBOX_MISC_REG 0x08022006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.7.CFG.xml.
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
} PCISTS_UBOX_MISC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_UBOX_MISC_REG supported on:                                            */
/*       IVT_EP (0x1015B008)                                                  */
/*       IVT_EX (0x1015B008)                                                  */
/*       HSX (0x10187008)                                                     */
/*       BDX (0x10187008)                                                     */
/* Register default value:              0x00                                  */
#define RID_UBOX_MISC_REG 0x08021008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
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
} RID_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_UBOX_MISC_REG supported on:                                         */
/*       IVT_EP (0x1015B009)                                                  */
/*       IVT_EX (0x1015B009)                                                  */
/*       HSX (0x10187009)                                                     */
/*       BDX (0x10187009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_UBOX_MISC_REG 0x08021009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.7.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_UBOX_MISC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_UBOX_MISC_REG supported on:                                         */
/*       IVT_EP (0x2015B00A)                                                  */
/*       IVT_EX (0x2015B00A)                                                  */
/*       HSX (0x2018700A)                                                     */
/*       BDX (0x2018700A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_UBOX_MISC_REG 0x0802200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
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
} CCR_N1_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* CLSR_UBOX_MISC_REG supported on:                                           */
/*       IVT_EP (0x1015B00C)                                                  */
/*       IVT_EX (0x1015B00C)                                                  */
/*       HSX (0x1018700C)                                                     */
/*       BDX (0x1018700C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_UBOX_MISC_REG 0x0802100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
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
} CLSR_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* PLAT_UBOX_MISC_REG supported on:                                           */
/*       IVT_EP (0x1015B00D)                                                  */
/*       IVT_EX (0x1015B00D)                                                  */
/*       HSX (0x1018700D)                                                     */
/*       BDX (0x1018700D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_UBOX_MISC_REG 0x0802100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
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
} PLAT_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* HDR_UBOX_MISC_REG supported on:                                            */
/*       IVT_EP (0x1015B00E)                                                  */
/*       IVT_EX (0x1015B00E)                                                  */
/*       HSX (0x1018700E)                                                     */
/*       BDX (0x1018700E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_UBOX_MISC_REG 0x0802100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
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
} HDR_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIST_UBOX_MISC_REG supported on:                                           */
/*       IVT_EP (0x1015B00F)                                                  */
/*       IVT_EX (0x1015B00F)                                                  */
/*       HSX (0x1018700F)                                                     */
/*       BDX (0x1018700F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_UBOX_MISC_REG 0x0802100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
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
} BIST_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOS_SV_PATCH_SKPD_UBOX_MISC_REG supported on:                             */
/*       IVT_EP (0x4015B0D4)                                                  */
/*       IVT_EX (0x4015B0D4)                                                  */
/*       HSX (0x40187028)                                                     */
/*       BDX (0x40187028)                                                     */
/* Register default value:              0x00000000                            */
#define BIOS_SV_PATCH_SKPD_UBOX_MISC_REG 0x0802C000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS to Pcode communication for SLICE_DISABLE/LLC_WAY_DISABLE
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RWS_LB, default = 32'b00000000000000000000000000000000 
       BIOS SV PATCH register
     */
  } Bits;
  UINT32 Data;
} BIOS_SV_PATCH_SKPD_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* SVID_UBOX_MISC_REG supported on:                                           */
/*       IVT_EP (0x2015B02C)                                                  */
/*       IVT_EX (0x2015B02C)                                                  */
/*       HSX (0x2018702C)                                                     */
/*       BDX (0x2018702C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_UBOX_MISC_REG 0x0802202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 */
typedef union {
  struct {
    UINT16 subsystem_vendor_identification_number : 16;
    /* subsystem_vendor_identification_number - Bits[15:0], RW_O, default = 16'b1000000010000110 
       The default value specifies Intel but can be set to any value once after reset.
     */
  } Bits;
  UINT16 Data;
} SVID_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* SDID_UBOX_MISC_REG supported on:                                           */
/*       IVT_EP (0x2015B02E)                                                  */
/*       IVT_EX (0x2015B02E)                                                  */
/*       HSX (0x2018702E)                                                     */
/*       BDX (0x2018702E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_UBOX_MISC_REG 0x0802202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 */
typedef union {
  struct {
    UINT16 subsystem_device_identification_number : 16;
    /* subsystem_device_identification_number - Bits[15:0], RW_O, default = 16'b0000000000000000 
       Assigned by the subsystem vendor to uniquely identify the subsystem
     */
  } Bits;
  UINT16 Data;
} SDID_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_UBOX_MISC_REG supported on:                                         */
/*       IVT_EP (0x1015B034)                                                  */
/*       IVT_EX (0x1015B034)                                                  */
/*       HSX (0x10187034)                                                     */
/*       BDX (0x10187034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_UBOX_MISC_REG 0x08021034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
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
} CAPPTR_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* INTL_UBOX_MISC_REG supported on:                                           */
/*       IVT_EP (0x1015B03C)                                                  */
/*       IVT_EX (0x1015B03C)                                                  */
/*       HSX (0x1018703C)                                                     */
/*       BDX (0x1018703C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_UBOX_MISC_REG 0x0802103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
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
} INTL_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* INTPIN_UBOX_MISC_REG supported on:                                         */
/*       IVT_EP (0x1015B03D)                                                  */
/*       IVT_EX (0x1015B03D)                                                  */
/*       HSX (0x1018703D)                                                     */
/*       BDX (0x1018703D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_UBOX_MISC_REG 0x0802103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
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
} INTPIN_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* MINGNT_UBOX_MISC_REG supported on:                                         */
/*       IVT_EP (0x1015B03E)                                                  */
/*       IVT_EX (0x1015B03E)                                                  */
/*       HSX (0x1018703E)                                                     */
/*       BDX (0x1018703E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_UBOX_MISC_REG 0x0802103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
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
} MINGNT_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_UBOX_MISC_REG supported on:                                         */
/*       IVT_EP (0x1015B03F)                                                  */
/*       IVT_EX (0x1015B03F)                                                  */
/*       HSX (0x1018703F)                                                     */
/*       BDX (0x1018703F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_UBOX_MISC_REG 0x0802103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
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
} MAXLAT_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSSCRATCHPAD0_UBOX_MISC_REG supported on:                                */
/*       IVT_EP (0x4015B040)                                                  */
/*       IVT_EX (0x4015B040)                                                  */
/*       HSX (0x40187040)                                                     */
/*       BDX (0x40187040)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSSCRATCHPAD0_UBOX_MISC_REG 0x08024040
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RWS, default = 32'b00000000000000000000000000000000 
       Set by BIOS, sticky across RESET
     */
  } Bits;
  UINT32 Data;
} BIOSSCRATCHPAD0_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSSCRATCHPAD1_UBOX_MISC_REG supported on:                                */
/*       IVT_EP (0x4015B044)                                                  */
/*       IVT_EX (0x4015B044)                                                  */
/*       HSX (0x40187044)                                                     */
/*       BDX (0x40187044)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSSCRATCHPAD1_UBOX_MISC_REG 0x08024044
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RWS, default = 32'b00000000000000000000000000000000 
       Set by BIOS, sticky across RESET
     */
  } Bits;
  UINT32 Data;
} BIOSSCRATCHPAD1_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSSCRATCHPAD2_UBOX_MISC_REG supported on:                                */
/*       IVT_EP (0x4015B048)                                                  */
/*       IVT_EX (0x4015B048)                                                  */
/*       HSX (0x40187048)                                                     */
/*       BDX (0x40187048)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSSCRATCHPAD2_UBOX_MISC_REG 0x08024048
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RWS, default = 32'b00000000000000000000000000000000 
       Set by BIOS, sticky across RESET
     */
  } Bits;
  UINT32 Data;
} BIOSSCRATCHPAD2_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSSCRATCHPAD3_UBOX_MISC_REG supported on:                                */
/*       IVT_EP (0x4015B04C)                                                  */
/*       IVT_EX (0x4015B04C)                                                  */
/*       HSX (0x4018704C)                                                     */
/*       BDX (0x4018704C)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSSCRATCHPAD3_UBOX_MISC_REG 0x0802404C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RWS, default = 32'b00000000000000000000000000000000 
       Set by BIOS, sticky across RESET
     */
  } Bits;
  UINT32 Data;
} BIOSSCRATCHPAD3_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSSCRATCHPAD4_UBOX_MISC_REG supported on:                                */
/*       IVT_EP (0x4015B050)                                                  */
/*       IVT_EX (0x4015B050)                                                  */
/*       HSX (0x40187050)                                                     */
/*       BDX (0x40187050)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSSCRATCHPAD4_UBOX_MISC_REG 0x08024050
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RWS, default = 32'b00000000000000000000000000000000 
       Set by BIOS, sticky across RESET
     */
  } Bits;
  UINT32 Data;
} BIOSSCRATCHPAD4_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSSCRATCHPAD5_UBOX_MISC_REG supported on:                                */
/*       IVT_EP (0x4015B054)                                                  */
/*       IVT_EX (0x4015B054)                                                  */
/*       HSX (0x40187054)                                                     */
/*       BDX (0x40187054)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSSCRATCHPAD5_UBOX_MISC_REG 0x08024054
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RWS, default = 32'b00000000000000000000000000000000 
       Set by BIOS, sticky across RESET
     */
  } Bits;
  UINT32 Data;
} BIOSSCRATCHPAD5_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSSCRATCHPAD6_UBOX_MISC_REG supported on:                                */
/*       IVT_EP (0x4015B058)                                                  */
/*       IVT_EX (0x4015B058)                                                  */
/*       HSX (0x40187058)                                                     */
/*       BDX (0x40187058)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSSCRATCHPAD6_UBOX_MISC_REG 0x08024058
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RWS, default = 32'b00000000000000000000000000000000 
       Set by BIOS, sticky across RESET
     */
  } Bits;
  UINT32 Data;
} BIOSSCRATCHPAD6_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSSCRATCHPAD7_UBOX_MISC_REG supported on:                                */
/*       IVT_EP (0x4015B05C)                                                  */
/*       IVT_EX (0x4015B05C)                                                  */
/*       HSX (0x4018705C)                                                     */
/*       BDX (0x4018705C)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSSCRATCHPAD7_UBOX_MISC_REG 0x0802405C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RWS, default = 32'b00000000000000000000000000000000 
       Set by BIOS, sticky across RESET
     */
  } Bits;
  UINT32 Data;
} BIOSSCRATCHPAD7_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSNONSTICKYSCRATCHPAD0_UBOX_MISC_REG supported on:                       */
/*       IVT_EP (0x4015B060)                                                  */
/*       IVT_EX (0x4015B060)                                                  */
/*       HSX (0x40187060)                                                     */
/*       BDX (0x40187060)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSNONSTICKYSCRATCHPAD0_UBOX_MISC_REG 0x08024060
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad register
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RW, default = 32'b00000000000000000000000000000000 
       BIOS Scratchpad register
     */
  } Bits;
  UINT32 Data;
} BIOSNONSTICKYSCRATCHPAD0_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSNONSTICKYSCRATCHPAD1_UBOX_MISC_REG supported on:                       */
/*       IVT_EP (0x4015B064)                                                  */
/*       IVT_EX (0x4015B064)                                                  */
/*       HSX (0x40187064)                                                     */
/*       BDX (0x40187064)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSNONSTICKYSCRATCHPAD1_UBOX_MISC_REG 0x08024064
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad register
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RW, default = 32'b00000000000000000000000000000000 
       BIOS Scratchpad register
     */
  } Bits;
  UINT32 Data;
} BIOSNONSTICKYSCRATCHPAD1_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG supported on:                       */
/*       IVT_EP (0x4015B068)                                                  */
/*       IVT_EX (0x4015B068)                                                  */
/*       HSX (0x40187068)                                                     */
/*       BDX (0x40187068)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG 0x08024068
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad register
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RW, default = 32'b00000000000000000000000000000000 
       BIOS Scratchpad register
     */
  } Bits;
  UINT32 Data;
} BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSNONSTICKYSCRATCHPAD3_UBOX_MISC_REG supported on:                       */
/*       IVT_EP (0x4015B06C)                                                  */
/*       IVT_EX (0x4015B06C)                                                  */
/*       HSX (0x4018706C)                                                     */
/*       BDX (0x4018706C)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSNONSTICKYSCRATCHPAD3_UBOX_MISC_REG 0x0802406C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad register
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RW, default = 32'b00000000000000000000000000000000 
       BIOS Scratchpad register
     */
  } Bits;
  UINT32 Data;
} BIOSNONSTICKYSCRATCHPAD3_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSNONSTICKYSCRATCHPAD4_UBOX_MISC_REG supported on:                       */
/*       IVT_EP (0x4015B070)                                                  */
/*       IVT_EX (0x4015B070)                                                  */
/*       HSX (0x40187070)                                                     */
/*       BDX (0x40187070)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSNONSTICKYSCRATCHPAD4_UBOX_MISC_REG 0x08024070
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad register
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RW, default = 32'b00000000000000000000000000000000 
       BIOS Scratchpad register
     */
  } Bits;
  UINT32 Data;
} BIOSNONSTICKYSCRATCHPAD4_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSNONSTICKYSCRATCHPAD5_UBOX_MISC_REG supported on:                       */
/*       IVT_EP (0x4015B074)                                                  */
/*       IVT_EX (0x4015B074)                                                  */
/*       HSX (0x40187074)                                                     */
/*       BDX (0x40187074)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSNONSTICKYSCRATCHPAD5_UBOX_MISC_REG 0x08024074
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad register
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RW, default = 32'b00000000000000000000000000000000 
       BIOS Scratchpad register
     */
  } Bits;
  UINT32 Data;
} BIOSNONSTICKYSCRATCHPAD5_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSNONSTICKYSCRATCHPAD6_UBOX_MISC_REG supported on:                       */
/*       IVT_EP (0x4015B078)                                                  */
/*       IVT_EX (0x4015B078)                                                  */
/*       HSX (0x40187078)                                                     */
/*       BDX (0x40187078)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSNONSTICKYSCRATCHPAD6_UBOX_MISC_REG 0x08024078
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad register
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RW, default = 32'b00000000000000000000000000000000 
       BIOS Scratchpad register
     */
  } Bits;
  UINT32 Data;
} BIOSNONSTICKYSCRATCHPAD6_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSNONSTICKYSCRATCHPAD7_UBOX_MISC_REG supported on:                       */
/*       IVT_EP (0x4015B07C)                                                  */
/*       IVT_EX (0x4015B07C)                                                  */
/*       HSX (0x4018707C)                                                     */
/*       BDX (0x4018707C)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSNONSTICKYSCRATCHPAD7_UBOX_MISC_REG 0x0802407C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad register
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RW, default = 32'b00000000000000000000000000000000 
       BIOS Scratchpad register
     */
  } Bits;
  UINT32 Data;
} BIOSNONSTICKYSCRATCHPAD7_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSNONSTICKYSCRATCHPAD8_UBOX_MISC_REG supported on:                       */
/*       IVT_EP (0x4015B080)                                                  */
/*       IVT_EX (0x4015B080)                                                  */
/*       HSX (0x40187080)                                                     */
/*       BDX (0x40187080)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSNONSTICKYSCRATCHPAD8_UBOX_MISC_REG 0x08024080
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad register
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RW, default = 32'b00000000000000000000000000000000 
       BIOS Scratchpad register
     */
  } Bits;
  UINT32 Data;
} BIOSNONSTICKYSCRATCHPAD8_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSNONSTICKYSCRATCHPAD9_UBOX_MISC_REG supported on:                       */
/*       IVT_EP (0x4015B084)                                                  */
/*       IVT_EX (0x4015B084)                                                  */
/*       HSX (0x40187084)                                                     */
/*       BDX (0x40187084)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSNONSTICKYSCRATCHPAD9_UBOX_MISC_REG 0x08024084
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad register
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RW, default = 32'b00000000000000000000000000000000 
       BIOS Scratchpad register
     */
  } Bits;
  UINT32 Data;
} BIOSNONSTICKYSCRATCHPAD9_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSNONSTICKYSCRATCHPAD10_UBOX_MISC_REG supported on:                      */
/*       IVT_EP (0x4015B088)                                                  */
/*       IVT_EX (0x4015B088)                                                  */
/*       HSX (0x40187088)                                                     */
/*       BDX (0x40187088)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSNONSTICKYSCRATCHPAD10_UBOX_MISC_REG 0x08024088
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad register
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RW, default = 32'b00000000000000000000000000000000 
       BIOS Scratchpad register
     */
  } Bits;
  UINT32 Data;
} BIOSNONSTICKYSCRATCHPAD10_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSNONSTICKYSCRATCHPAD11_UBOX_MISC_REG supported on:                      */
/*       IVT_EP (0x4015B08C)                                                  */
/*       IVT_EX (0x4015B08C)                                                  */
/*       HSX (0x4018708C)                                                     */
/*       BDX (0x4018708C)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSNONSTICKYSCRATCHPAD11_UBOX_MISC_REG 0x0802408C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad register
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RW, default = 32'b00000000000000000000000000000000 
       BIOS Scratchpad register
     */
  } Bits;
  UINT32 Data;
} BIOSNONSTICKYSCRATCHPAD11_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSNONSTICKYSCRATCHPAD12_UBOX_MISC_REG supported on:                      */
/*       IVT_EP (0x4015B090)                                                  */
/*       IVT_EX (0x4015B090)                                                  */
/*       HSX (0x40187090)                                                     */
/*       BDX (0x40187090)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSNONSTICKYSCRATCHPAD12_UBOX_MISC_REG 0x08024090
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad register
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RW, default = 32'b00000000000000000000000000000000 
       BIOS Scratchpad register
     */
  } Bits;
  UINT32 Data;
} BIOSNONSTICKYSCRATCHPAD12_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSNONSTICKYSCRATCHPAD13_UBOX_MISC_REG supported on:                      */
/*       IVT_EP (0x4015B094)                                                  */
/*       IVT_EX (0x4015B094)                                                  */
/*       HSX (0x40187094)                                                     */
/*       BDX (0x40187094)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSNONSTICKYSCRATCHPAD13_UBOX_MISC_REG 0x08024094
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad register
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RW, default = 32'b00000000000000000000000000000000 
       BIOS Scratchpad register
     */
  } Bits;
  UINT32 Data;
} BIOSNONSTICKYSCRATCHPAD13_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSNONSTICKYSCRATCHPAD14_UBOX_MISC_REG supported on:                      */
/*       IVT_EP (0x4015B098)                                                  */
/*       IVT_EX (0x4015B098)                                                  */
/*       HSX (0x40187098)                                                     */
/*       BDX (0x40187098)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSNONSTICKYSCRATCHPAD14_UBOX_MISC_REG 0x08024098
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad register
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RW, default = 32'b00000000000000000000000000000000 
       BIOS Scratchpad register
     */
  } Bits;
  UINT32 Data;
} BIOSNONSTICKYSCRATCHPAD14_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* BIOSNONSTICKYSCRATCHPAD15_UBOX_MISC_REG supported on:                      */
/*       IVT_EP (0x4015B09C)                                                  */
/*       IVT_EX (0x4015B09C)                                                  */
/*       HSX (0x4018709C)                                                     */
/*       BDX (0x4018709C)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSNONSTICKYSCRATCHPAD15_UBOX_MISC_REG 0x0802409C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * BIOS Scratchpad register
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RW, default = 32'b00000000000000000000000000000000 
       BIOS Scratchpad register
     */
  } Bits;
  UINT32 Data;
} BIOSNONSTICKYSCRATCHPAD15_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* LOCALSEMAPHORE0_UBOX_MISC_REG supported on:                                */
/*       IVT_EP (0x4015B0A0)                                                  */
/*       IVT_EX (0x4015B0A0)                                                  */
/*       HSX (0x401870A0)                                                     */
/*       BDX (0x401870A0)                                                     */
/* Register default value:              0x00000000                            */
#define LOCALSEMAPHORE0_UBOX_MISC_REG 0x080240A0


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.7.CFG.xml.
 * unCore Semaphore register is a resource shared by all threads even though it has 
 * access and view for each one of the threads. Each one of the fields is 
 * identified to  
 *  be a shared or a dedicated element.
 */
typedef union {
  struct {
    UINT32 aqu_override : 1;
    /* aqu_override - Bits[0:0], WO, default = 1'b0 
       The bit has different meanings for read and write.
        Write - Override acquisition :
         0 : no effect
         1 : Override. Take ownership of semaphore ignoring any other setting
             or requests.
       Note that this bit has no stat directly associated with it. There are different 
        operations associated with the read and write operations.
     */
    UINT32 bus_aqu_release : 1;
    /* bus_aqu_release - Bits[1:1], RW_V, default = 1'b0 
       Read - Bus status :
         0 : the semaphore s currently free.
         1 : the semaphore is currently busy by one of the threads.
        Write - Acquire request :
         0 : Release the ownership/request of the semaphore. It cause the pending bit 
       for the 
             thread to be cleared.
         1 : Request the ownership of the semaphore. It cause the pending bit for the 
       thread 
              to be set. 
              
     */
    UINT32 currentthread : 1;
    /* currentthread - Bits[2:2], RO_V, default = 1'b0 
       This bit indicate the TID of the thread that currently owns the semaphore.
        When the semaphore is free (Busy==0), this bit is undefined.
         0 : Thread 0 
         1 : Thread 1
     */
    UINT32 currentcore : 5;
    /* currentcore - Bits[7:3], RO_V, default = 5'b00000 
       This field indicates the CID of the thread that currently owns the semaphore.
        When the semaphore is free (Busy==0), this bit is undefined.
         'n' - Core number
     */
    UINT32 pendingrequest : 16;
    /* pendingrequest - Bits[23:8], RO_V, default = 16'b0000000000000000 
       Indicates the threads that have a pending request for Semaphore ownership for 
       core 7:0. 
        bit 'i' in the field indicate a pending request for 2*CID+TID   (C0T0, C0T1, 
       C1T0,...) 
     */
    UINT32 pendingrequest_rsv : 2;
    /* pendingrequest_rsv - Bits[25:24], RO, default = 2'b00  */
    UINT32 requesterthread : 1;
    /* requesterthread - Bits[26:26], WO, default = 1'b0 
       The thread id of the requester
     */
    UINT32 requestercore : 5;
    /* requestercore - Bits[31:27], WO, default = 5'b00000 
       Core ID of the requesting core
     */
  } Bits;
  UINT32 Data;
} LOCALSEMAPHORE0_UBOX_MISC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* LOCALSEMAPHORE1_UBOX_MISC_REG supported on:                                */
/*       IVT_EP (0x4015B0A4)                                                  */
/*       IVT_EX (0x4015B0A4)                                                  */
/*       HSX (0x401870A4)                                                     */
/*       BDX (0x401870A4)                                                     */
/* Register default value:              0x00000000                            */
#define LOCALSEMAPHORE1_UBOX_MISC_REG 0x080240A4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.7.CFG.xml.
 * unCore Semaphore register is a resource shared by all threads even though it has 
 * access and view for each one of the threads. Each one of the fields is 
 * identified to  
 *  be a shared or a dedicated element.
 */
typedef union {
  struct {
    UINT32 aqu_override : 1;
    /* aqu_override - Bits[0:0], WO, default = 1'b0 
       The bit has different meanings for read and write.
       Note that this bit has no stat directly associated with it. There are different 
        operations associated with the read and write operations.
        Write - Override acquisition :
         0 : no effect
         1 : Override. Take ownership of semaphore ignoring any other setting
             or requests.
     */
    UINT32 bus_aqu_release : 1;
    /* bus_aqu_release - Bits[1:1], RW_V, default = 1'b0 
       Read - Bus status :
         0 : the semaphore s currently free.
         1 : the semaphore is currently busy by one of the threads.
        Write - Acquire request :
         0 : Release the ownership/request of the semaphore. It cause the pending bit 
       for the 
             thread to be cleared.
         1 : Request the ownership of the semaphore. It cause the pending bit for the 
       thread 
              to be set. 
              
     */
    UINT32 currentthread : 1;
    /* currentthread - Bits[2:2], RO_V, default = 1'b0 
       This bit indicate the TID of the thread that currently owns the semaphore.
        When the semaphore is free (Busy==0), this bit is undefined.
         0 : Thread 0 
         1 : Thread 1
     */
    UINT32 currentcore : 5;
    /* currentcore - Bits[7:3], RO_V, default = 5'b00000 
       This field indicates the CID of the thread that currently owns the semaphore.
        When the semaphore is free (Busy==0), this bit is undefined.
         'n' - Core number
     */
    UINT32 pendingrequest : 16;
    /* pendingrequest - Bits[23:8], RO_V, default = 16'b0000000000000000 
       Indicates the threads that have a pending request for Semaphore ownership for 
       core 7:0. 
        bit 'i' in the field indicate a pending request for 2*CID+TID   (C0T0, C0T1, 
       C1T0,...) 
     */
    UINT32 pendingrequest_rsv : 2;
    /* pendingrequest_rsv - Bits[25:24], RO, default = 2'b00  */
    UINT32 requesterthread : 1;
    /* requesterthread - Bits[26:26], WO, default = 1'b0 
       The thread id of the requester
     */
    UINT32 requestercore : 5;
    /* requestercore - Bits[31:27], WO, default = 5'b00000 
       Core ID of the requesting core
     */
  } Bits;
  UINT32 Data;
} LOCALSEMAPHORE1_UBOX_MISC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* SYSTEMSEMAPHORE0_UBOX_MISC_REG supported on:                               */
/*       IVT_EP (0x4015B0A8)                                                  */
/*       IVT_EX (0x4015B0A8)                                                  */
/*       HSX (0x401870A8)                                                     */
/*       BDX (0x401870A8)                                                     */
/* Register default value:              0x00000000                            */
#define SYSTEMSEMAPHORE0_UBOX_MISC_REG 0x080240A8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * Uncore Semaphore register is a resource shared by all threads even though it has 
 * access and view for each one of the threads. Each one of the fields is 
 * identified to  
 *  be a shared or a dedicated element.
 */
typedef union {
  struct {
    UINT32 aqu_override : 1;
    /* aqu_override - Bits[0:0], WO, default = 1'b0 
       The bit has different meanings for read and write.
       Note that this bit has no stat directly associated with it. There are different 
        operations associated with the read and write operations.
        Write - Override acquisition :
         0 : no effect
         1 : Override. Take ownership of semaphore ignoring any other setting
             or requests.
     */
    UINT32 bus_aqu_release : 1;
    /* bus_aqu_release - Bits[1:1], RW_V, default = 1'b0 
       Read - Bus status :
         0 : the semaphore s currently free.
         1 : the semaphore is currently busy by one of the threads.
        Write - Acquire request :
         0 : Release the ownership/request of the semaphore. It cause the pending bit 
       for the 
             thread to be cleared.
         1 : Request the ownership of the semaphore. It cause the pending bit for the 
       thread 
              to be set. 
              
     */
    UINT32 currentnode : 3;
    /* currentnode - Bits[4:2], RO_V, default = 3'b000 
       This bit indicate the Node id of the node that currently owns the semaphore.
        When the semaphore is free (Busy==0), this bit is undefined.
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pendingrequestnode : 8;
    /* pendingrequestnode - Bits[15:8], RO_V, default = 8'b00000000 
       Pending request vector. Debug only usage
     */
    UINT32 rsvd_16 : 8;
    /* rsvd_16 - Bits[23:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 requesternode : 3;
    /* requesternode - Bits[26:24], WO, default = 3'b000 
       The requestor writes his own node id to the added into the pending vector
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SYSTEMSEMAPHORE0_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* SYSTEMSEMAPHORE1_UBOX_MISC_REG supported on:                               */
/*       IVT_EP (0x4015B0AC)                                                  */
/*       IVT_EX (0x4015B0AC)                                                  */
/*       HSX (0x401870AC)                                                     */
/*       BDX (0x401870AC)                                                     */
/* Register default value:              0x00000000                            */
#define SYSTEMSEMAPHORE1_UBOX_MISC_REG 0x080240AC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * unCore Semaphore register is a resource shared by all threads even though it has 
 * access and view for each one of the threads. Each one of the fields is 
 * identified to  
 *  be a shared or a dedicated element.
 */
typedef union {
  struct {
    UINT32 aqu_override : 1;
    /* aqu_override - Bits[0:0], WO, default = 1'b0 
       The bit has different meanings for read and write.
       Note that this bit has no stat directly associated with it. There are different 
        operations associated with the read and write operations. 
        Write - Override acquisition :
         0 : no effect
         1 : Override. Take ownership of semaphore ignoring any other setting
             or requests.
     */
    UINT32 bus_aqu_release : 1;
    /* bus_aqu_release - Bits[1:1], RW_V, default = 1'b0 
       Read - Bus status :
         0 : the semaphore s currently free.
         1 : the semaphore is currently busy by one of the threads.
        Write - Acquire request :
         0 : Release the ownership/request of the semaphore. It cause the pending bit 
       for the 
             thread to be cleared.
         1 : Request the ownership of the semaphore. It cause the pending bit for the 
       thread 
              to be set. 
              
     */
    UINT32 currentnode : 3;
    /* currentnode - Bits[4:2], RO_V, default = 3'b000 
       This bit indicate the Node id of the node that currently owns the semaphore.
        When the semaphore is free (Busy==0), this bit is undefined.
     */
    UINT32 rsvd_5 : 3;
    /* rsvd_5 - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pendingrequestnode : 8;
    /* pendingrequestnode - Bits[15:8], RO_V, default = 8'b00000000 
       Pending request vector. Debug only usage
     */
    UINT32 rsvd_16 : 8;
    /* rsvd_16 - Bits[23:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 requesternode : 3;
    /* requesternode - Bits[26:24], WO, default = 3'b000 
       The requestor writes his own node id to the added into the pending vector
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SYSTEMSEMAPHORE1_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* DEVHIDE0_UBOX_MISC_REG supported on:                                       */
/*       IVT_EP (0x4015B0B0)                                                  */
/*       IVT_EX (0x4015B0B0)                                                  */
/*       HSX (0x401870B0)                                                     */
/*       BDX (0x401870B0)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE0_UBOX_MISC_REG 0x080240B0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 disfn : 32;
    /* disfn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDEuCodeCtrl4, then it means that 
       in device 5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE0_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* DEVHIDE1_UBOX_MISC_REG supported on:                                       */
/*       IVT_EP (0x4015B0B4)                                                  */
/*       IVT_EX (0x4015B0B4)                                                  */
/*       HSX (0x401870B4)                                                     */
/*       BDX (0x401870B4)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE1_UBOX_MISC_REG 0x080240B4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 disfn : 32;
    /* disfn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDEuCodeCtrl4, then it means that 
       in device 5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE1_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* DEVHIDE2_UBOX_MISC_REG supported on:                                       */
/*       IVT_EP (0x4015B0B8)                                                  */
/*       IVT_EX (0x4015B0B8)                                                  */
/*       HSX (0x401870B8)                                                     */
/*       BDX (0x401870B8)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE2_UBOX_MISC_REG 0x080240B8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 disfn : 32;
    /* disfn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDEuCodeCtrl4, then it means that 
       in device 5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE2_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* DEVHIDE3_UBOX_MISC_REG supported on:                                       */
/*       IVT_EP (0x4015B0BC)                                                  */
/*       IVT_EX (0x4015B0BC)                                                  */
/*       HSX (0x401870BC)                                                     */
/*       BDX (0x401870BC)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE3_UBOX_MISC_REG 0x080240BC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 disfn : 32;
    /* disfn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDEuCodeCtrl4, then it means that 
       in device 5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE3_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* DEVHIDE4_UBOX_MISC_REG supported on:                                       */
/*       IVT_EP (0x4015B0C0)                                                  */
/*       IVT_EX (0x4015B0C0)                                                  */
/*       HSX (0x401870C0)                                                     */
/*       BDX (0x401870C0)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE4_UBOX_MISC_REG 0x080240C0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 disfn : 32;
    /* disfn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDEuCodeCtrl4, then it means that 
       in device 5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE4_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* DEVHIDE5_UBOX_MISC_REG supported on:                                       */
/*       IVT_EP (0x4015B0C4)                                                  */
/*       IVT_EX (0x4015B0C4)                                                  */
/*       HSX (0x401870C4)                                                     */
/*       BDX (0x401870C4)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE5_UBOX_MISC_REG 0x080240C4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 disfn : 32;
    /* disfn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDEuCodeCtrl4, then it means that 
       in device 5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE5_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* DEVHIDE6_UBOX_MISC_REG supported on:                                       */
/*       IVT_EP (0x4015B0C8)                                                  */
/*       IVT_EX (0x4015B0C8)                                                  */
/*       HSX (0x401870C8)                                                     */
/*       BDX (0x401870C8)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE6_UBOX_MISC_REG 0x080240C8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 disfn : 32;
    /* disfn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDEuCodeCtrl4, then it means that 
       in device 5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE6_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* DEVHIDE7_UBOX_MISC_REG supported on:                                       */
/*       IVT_EP (0x4015B0CC)                                                  */
/*       IVT_EX (0x4015B0CC)                                                  */
/*       HSX (0x401870CC)                                                     */
/*       BDX (0x401870CC)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE7_UBOX_MISC_REG 0x080240CC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 disfn : 32;
    /* disfn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDEuCodeCtrl4, then it means that 
       in device 5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE7_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* CPUBUSNO_UBOX_MISC_REG supported on:                                       */
/*       IVT_EP (0x4015B0D0)                                                  */
/*       IVT_EX (0x4015B0D0)                                                  */
/*       HSX (0x401870D0)                                                     */
/*       BDX (0x401870D0)                                                     */
/* Register default value:              0x00000000                            */
#define CPUBUSNO_UBOX_MISC_REG 0x080240D0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * Bus Number Configuration for Jaketown
 */
typedef union {
  struct {
    UINT32 cpubusno0 : 8;
    /* cpubusno0 - Bits[7:0], RW_LB, default = 8'b00000000 
       Bus Number for IIO devices
     */
    UINT32 cpubusno1 : 8;
    /* cpubusno1 - Bits[15:8], RW_LB, default = 8'b00000000 
       Bus Number for non IIO devices in the uncore
     */
    UINT32 rsvd : 15;
    /* rsvd - Bits[30:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 valid : 1;
    /* valid - Bits[31:31], RW_LB, default = 1'b0 
       Indicates whether the bus numbers have been initialized or not
     */
  } Bits;
  UINT32 Data;
} CPUBUSNO_UBOX_MISC_STRUCT;
#endif /* ASM_INC */


/* CPUBUSNO2_UBOX_MISC_REG supported on:                                      */
/*       BDX (0x401870D4)                                                     */
/* Register default value:              0x00000000                            */
#define CPUBUSNO2_UBOX_MISC_REG 0x080240D4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * Bus Number Configuration for GBE and CBDMA3.3 in BDX-DE
 */
typedef union {
  struct {
    UINT32 gbebusno : 8;
    /* gbebusno - Bits[7:0], RW_LB, default = 8'b00000000 
       GBE Bus Number in BDX-DE
     */
    UINT32 cbdma3_3busno : 8;
    /* cbdma3_3busno - Bits[15:8], RW_LB, default = 8'b00000000 
       CBDMA3.3 Bus Number in BDX-DE
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPUBUSNO2_UBOX_MISC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* SMICTRL_UBOX_MISC_REG supported on:                                        */
/*       IVT_EP (0x4015B0D8)                                                  */
/*       IVT_EX (0x4015B0D8)                                                  */
/*       HSX (0x401870D8)                                                     */
/*       BDX (0x401870D8)                                                     */
/* Register default value:              0x04000000                            */
#define SMICTRL_UBOX_MISC_REG 0x080240D8


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.7.CFG.xml.
 * SMI generation control
 */
typedef union {
  struct {
    UINT32 threshold : 20;
    /* threshold - Bits[19:0], RW_LB, default = 20'b00000000000000000000 
       This is the  countdown that happens in the hardware before an SMI is generated 
       due to a UMC 
     */
    UINT32 rsvd_20 : 4;
    /* rsvd_20 - Bits[23:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 umcsmien : 1;
    /* umcsmien - Bits[24:24], RW_LB, default = 1'b0 
       This is the enable bit that enables SMI generation due to a UMC
       1 -> Generate SMI after the threshold counter expires.
       0 -> Disable generation of SMI
     */
    UINT32 smidis : 1;
    /* smidis - Bits[25:25], RW_LB, default = 1'b0 
       Disable generation of SMI
     */
    UINT32 smidis2 : 1;
    /* smidis2 - Bits[26:26], RW_LB, default = 1'b1 
       Disable generation of SMI forlocktimeout, cfg write mis-align access, and cfg 
       read mis-aslign access. 
     */
    UINT32 smidis3 : 1;
    /* smidis3 - Bits[27:27], RW_LB, default = 1'b0 
       Disable Generation of SMI from message channel
     */
    UINT32 smidis4 : 1;
    /* smidis4 - Bits[28:28], RW_LB, default = 1'b0 
       Disable Generation of SMI from CSMI from MsgCh
     */
    UINT32 rsvd_29 : 3;
    /* rsvd_29 - Bits[31:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SMICTRL_UBOX_MISC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* MMCFG_RULE_UBOX_MISC_REG supported on:                                     */
/*       IVT_EP (0x4015B0DC)                                                  */
/*       IVT_EX (0x4015B0DC)                                                  */
/*       HSX (0x401870DC)                                                     */
/*       BDX (0x401870DC)                                                     */
/* Register default value:              0x00000000                            */
#define MMCFG_RULE_UBOX_MISC_REG 0x080240DC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * Use addr[27:26] to enforce maximum bus numbers
 */
typedef union {
  struct {
    UINT32 rule_enable : 1;
    /* rule_enable - Bits[0:0], RW_LB, default = 1'b0 
       Enable for this MMCFG rule.
     */
    UINT32 length : 2;
    /* length - Bits[2:1], RW_LB, default = 2'b00 
       
         This two bit field masks address 27:26 (BUS) to enforce MaxBusNumber as 
       following encodings; 
       
           2'b10 : MaxBusNumber = 63 (ie. 64MB MMCFG range)
           2'b01 : MaxBusNumber = 127 (ie 128MB MMCFG range)
           2'b00 : MaxBusNunber = 256 (ie 256MB MMCFG range)
       
        Minimum granularity of MMCFG range will be 64MB.
     */
    UINT32 rsvd : 29;
    /* rsvd - Bits[31:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MMCFG_RULE_UBOX_MISC_STRUCT;
#endif /* ASM_INC */






/* LOCALSEMAPHOREPEND0_UBOX_MISC_REG supported on:                            */
/*       IVT_EP (0x4015B0F0)                                                  */
/*       IVT_EX (0x4015B0F0)                                                  */
/*       HSX (0x401870F0)                                                     */
/*       BDX (0x401870F0)                                                     */
/* Register default value:              0x00000000                            */
#define LOCALSEMAPHOREPEND0_UBOX_MISC_REG 0x080240F0


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.7.CFG.xml.
 * Local semaphore0 pending bits used for debug.
 */
typedef union {
  struct {
    UINT32 pendingrequest : 20;
    /* pendingrequest - Bits[19:0], RO_V, default = 20'b00000000000000000000 
       Indicates the threads that have a pending request for Semaphore ownership for 
       cores 17:8. 
        bit 'i' in the field indicate a pending request for 2*CID+TID   (C8T0, C8T1, 
       C9T0,...C17T1) 
     */
    UINT32 pendingrequest_rsv : 12;
    /* pendingrequest_rsv - Bits[31:20], RO, default = 12'b000000000000  */
  } Bits;
  UINT32 Data;
} LOCALSEMAPHOREPEND0_UBOX_MISC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * Local semaphore0 pending bits used for debug.
 */
typedef union {
  struct {
    UINT32 pendingrequest : 32;
    /* pendingrequest - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Indicates the threads that have a pending request for Semaphore ownership for 
       cores 23:8. 
        bit 'i' in the field indicate a pending request for 2*CID+TID   (C8T0, C8T1, 
       C9T0,...C23T1) 
     */
  } Bits;
  UINT32 Data;
} LOCALSEMAPHOREPEND0_UBOX_MISC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* LOCALSEMAPHOREPEND1_UBOX_MISC_REG supported on:                            */
/*       IVT_EP (0x4015B0F4)                                                  */
/*       IVT_EX (0x4015B0F4)                                                  */
/*       HSX (0x401870F4)                                                     */
/*       BDX (0x401870F4)                                                     */
/* Register default value:              0x00000000                            */
#define LOCALSEMAPHOREPEND1_UBOX_MISC_REG 0x080240F4


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.7.CFG.xml.
 * Local semaphore1 pending bits used for debug.
 */
typedef union {
  struct {
    UINT32 pendingrequest : 20;
    /* pendingrequest - Bits[19:0], RO_V, default = 20'b00000000000000000000 
       Indicates the threads that have a pending request for Semaphore ownership for 
       cores 17:8. 
        bit 'i' in the field indicate a pending request for 2*CID+TID   (C8T0, C8T1, 
       C9T0,...C17T1) 
     */
    UINT32 pendingrequest_rsv : 12;
    /* pendingrequest_rsv - Bits[31:20], RO, default = 12'b000000000000  */
  } Bits;
  UINT32 Data;
} LOCALSEMAPHOREPEND1_UBOX_MISC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * Local semaphore1 pending bits used for debug.
 */
typedef union {
  struct {
    UINT32 pendingrequest : 32;
    /* pendingrequest - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Indicates the threads that have a pending request for Semaphore ownership for 
       cores 23:8. 
        bit 'i' in the field indicate a pending request for 2*CID+TID   (C8T0, C8T1, 
       C9T0,...C23T1) 
     */
  } Bits;
  UINT32 Data;
} LOCALSEMAPHOREPEND1_UBOX_MISC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* UART_ENABLE_UBOX_MISC_REG supported on:                                    */
/*       BDX (0x401870F8)                                                     */
/* Register default value:              0x00000000                            */
#define UART_ENABLE_UBOX_MISC_REG 0x080240F8

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.7.CFG.xml.
 * To enable uart in BDX-DE
 */
typedef union {
  struct {
    UINT32 enableuartport0 : 1;
    /* enableuartport0 - Bits[0:0], RW_LB, default = 1'b0 
       Enables uart port0 for bdx-de (only should be done for bdx-de, decodes range 
       0x3f8-0x3ff) 
     */
    UINT32 enableuartport1 : 1;
    /* enableuartport1 - Bits[1:1], RW_LB, default = 1'b0 
       >Enables uart port1 for bdx-de (only should be done for bdx-de, decodes range 
       0x2f8-0x2ff) 
     */
    UINT32 rsvd : 30;
    /* rsvd - Bits[31:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} UART_ENABLE_UBOX_MISC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


#endif /* UBOX_MISC_h */
