/* Date Stamp: 8/23/2014 */

#ifndef QPIAGENT_LL_h
#define QPIAGENT_LL_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For all target CPUs:                                                       */
/* QPIAGENT_LL_DEV 8                                                          */
/* QPIAGENT_LL_FUN 0                                                          */

/* VID_QPIAGENT_LL_REG supported on:                                          */
/*       IVT_EP (0x20140000)                                                  */
/*       IVT_EX (0x20140000)                                                  */
/*       HSX (0x20140000)                                                     */
/*       BDX (0x20140000)                                                     */
/* Register default value:              0x8086                                */
#define VID_QPIAGENT_LL_REG 0x06002000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
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
} VID_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* DID_QPIAGENT_LL_REG supported on:                                          */
/*       IVT_EP (0x20140002)                                                  */
/*       IVT_EX (0x20140002)                                                  */
/*       HSX (0x20140002)                                                     */
/*       BDX (0x20140002)                                                     */
/* Register default value on IVT_EP:    0x0E80                                */
/* Register default value on IVT_EX:    0x0E80                                */
/* Register default value on HSX:       0x2F80                                */
/* Register default value on BDX:       0x6F80                                */
#define DID_QPIAGENT_LL_REG 0x06002002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6F80 
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
} DID_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* PCICMD_QPIAGENT_LL_REG supported on:                                       */
/*       IVT_EP (0x20140004)                                                  */
/*       IVT_EX (0x20140004)                                                  */
/*       HSX (0x20140004)                                                     */
/*       BDX (0x20140004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_QPIAGENT_LL_REG 0x06002004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
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
} PCICMD_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* PCISTS_QPIAGENT_LL_REG supported on:                                       */
/*       IVT_EP (0x20140006)                                                  */
/*       IVT_EX (0x20140006)                                                  */
/*       HSX (0x20140006)                                                     */
/*       BDX (0x20140006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_QPIAGENT_LL_REG 0x06002006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.0.CFG.xml.
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
} PCISTS_QPIAGENT_LL_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_QPIAGENT_LL_REG supported on:                                          */
/*       IVT_EP (0x10140008)                                                  */
/*       IVT_EX (0x10140008)                                                  */
/*       HSX (0x10140008)                                                     */
/*       BDX (0x10140008)                                                     */
/* Register default value:              0x00                                  */
#define RID_QPIAGENT_LL_REG 0x06001008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
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
} RID_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_QPIAGENT_LL_REG supported on:                                       */
/*       IVT_EP (0x10140009)                                                  */
/*       IVT_EX (0x10140009)                                                  */
/*       HSX (0x10140009)                                                     */
/*       BDX (0x10140009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_QPIAGENT_LL_REG 0x06001009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.0.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_QPIAGENT_LL_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_QPIAGENT_LL_REG supported on:                                       */
/*       IVT_EP (0x2014000A)                                                  */
/*       IVT_EX (0x2014000A)                                                  */
/*       HSX (0x2014000A)                                                     */
/*       BDX (0x2014000A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_QPIAGENT_LL_REG 0x0600200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
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
} CCR_N1_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* CLSR_QPIAGENT_LL_REG supported on:                                         */
/*       IVT_EP (0x1014000C)                                                  */
/*       IVT_EX (0x1014000C)                                                  */
/*       HSX (0x1014000C)                                                     */
/*       BDX (0x1014000C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_QPIAGENT_LL_REG 0x0600100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
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
} CLSR_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* PLAT_QPIAGENT_LL_REG supported on:                                         */
/*       IVT_EP (0x1014000D)                                                  */
/*       IVT_EX (0x1014000D)                                                  */
/*       HSX (0x1014000D)                                                     */
/*       BDX (0x1014000D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_QPIAGENT_LL_REG 0x0600100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
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
} PLAT_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* HDR_QPIAGENT_LL_REG supported on:                                          */
/*       IVT_EP (0x1014000E)                                                  */
/*       IVT_EX (0x1014000E)                                                  */
/*       HSX (0x1014000E)                                                     */
/*       BDX (0x1014000E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_QPIAGENT_LL_REG 0x0600100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
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
} HDR_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* BIST_QPIAGENT_LL_REG supported on:                                         */
/*       IVT_EP (0x1014000F)                                                  */
/*       IVT_EX (0x1014000F)                                                  */
/*       HSX (0x1014000F)                                                     */
/*       BDX (0x1014000F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_QPIAGENT_LL_REG 0x0600100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
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
} BIST_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* SVID_F0_QPIAGENT_LL_REG supported on:                                      */
/*       HSX (0x2014002C)                                                     */
/*       BDX (0x2014002C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_F0_QPIAGENT_LL_REG 0x0600202C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
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
} SVID_F0_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SDID_F0_QPIAGENT_LL_REG supported on:                                      */
/*       HSX (0x2014002E)                                                     */
/*       BDX (0x2014002E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_F0_QPIAGENT_LL_REG 0x0600202E

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
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
} SDID_F0_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CAPPTR_QPIAGENT_LL_REG supported on:                                       */
/*       IVT_EP (0x10140034)                                                  */
/*       IVT_EX (0x10140034)                                                  */
/*       HSX (0x10140034)                                                     */
/*       BDX (0x10140034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_QPIAGENT_LL_REG 0x06001034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
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
} CAPPTR_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* INTL_QPIAGENT_LL_REG supported on:                                         */
/*       IVT_EP (0x1014003C)                                                  */
/*       IVT_EX (0x1014003C)                                                  */
/*       HSX (0x1014003C)                                                     */
/*       BDX (0x1014003C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_QPIAGENT_LL_REG 0x0600103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
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
} INTL_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* INTPIN_QPIAGENT_LL_REG supported on:                                       */
/*       IVT_EP (0x1014003D)                                                  */
/*       IVT_EX (0x1014003D)                                                  */
/*       HSX (0x1014003D)                                                     */
/*       BDX (0x1014003D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_QPIAGENT_LL_REG 0x0600103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
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
} INTPIN_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* MINGNT_QPIAGENT_LL_REG supported on:                                       */
/*       IVT_EP (0x1014003E)                                                  */
/*       IVT_EX (0x1014003E)                                                  */
/*       HSX (0x1014003E)                                                     */
/*       BDX (0x1014003E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_QPIAGENT_LL_REG 0x0600103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
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
} MINGNT_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_QPIAGENT_LL_REG supported on:                                       */
/*       IVT_EP (0x1014003F)                                                  */
/*       IVT_EX (0x1014003F)                                                  */
/*       HSX (0x1014003F)                                                     */
/*       BDX (0x1014003F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_QPIAGENT_LL_REG 0x0600103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
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
} MAXLAT_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPILCP_QPIAGENT_LL_REG supported on:                                       */
/*       IVT_EP (0x40140040)                                                  */
/*       IVT_EX (0x40140040)                                                  */
/*       HSX (0x40140040)                                                     */
/*       BDX (0x40140040)                                                     */
/* Register default value on IVT_EP:    0x2A960001                            */
/* Register default value on IVT_EX:    0x2A960001                            */
/* Register default value on HSX:       0x2A960801                            */
/* Register default value on BDX:       0x2A960801                            */
#define QPILCP_QPIAGENT_LL_REG 0x06004040


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.0.CFG.xml.
 * Intel QPI Link Capability Register.  
 *  Note VN1 is only supported on the EX processor.
 */
typedef union {
  struct {
    UINT32 qpi_version_number : 8;
    /* qpi_version_number - Bits[7:0], RO, default = 8'h1 
        
               Intel QPI Version number
       	0h - rev 1.0
       	1h - rev 1.1
       	>1h - Reserved
             
     */
    UINT32 flit_interleave : 2;
    /* flit_interleave - Bits[9:8], RO, default = 2'b0 
        
               Flit Interleave
       	00 - Idle flit only (default)
       	01 - Command insert interleave in data stream
       	1x -Reserved
             
     */
    UINT32 rsvd_10 : 1;
    /* rsvd_10 - Bits[10:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 crc_mode_supported : 1;
    /* crc_mode_supported - Bits[11:11], RO, default = 1'b1 
        
               CRC Mode supported
       	0 - 8b CRC 
       	1 - 8b CRC and 16b Rolling CRC (EX processor only)
       	Default value is '0' for parts that do not support 16b Rolling CRC.
             
     */
    UINT32 l0s_support : 1;
    /* l0s_support - Bits[12:12], RO_V, default = 1'b0 
        
               L0s Support
       	L0s is not supported.
             
     */
    UINT32 l1_slave_support : 1;
    /* l1_slave_support - Bits[13:13], RO_V, default = 1'b0 
        
               L1 Slave Support
       	Bit is set if L1 slave is supported.
             
     */
    UINT32 l1_master_support : 1;
    /* l1_master_support - Bits[14:14], RO_V, default = 1'b0 
        
               L1 Master Support
       	Bit is set if L1 master is supported.
             
     */
    UINT32 l0p_support : 1;
    /* l0p_support - Bits[15:15], RO_V, default = 1'b0 
        
               L0p Support
       	Bit is set if L0p is supported.
             
     */
    UINT32 vna_credits : 6;
    /* vna_credits - Bits[21:16], RO, default = 6'h16 
        
               VNA Credits
        Counted by 8s, rounded up. Max value for VNA is reflected in this register. 
       Actual value is set by a different register. 
             
     */
    UINT32 vn0_credits_per_supported_nondata_vc : 2;
    /* vn0_credits_per_supported_nondata_vc - Bits[23:22], RO, default = 2'h2 
        
                VN0 credits per supported non-data VC
       	 00 - 0 credits (unsupported)
       	 01 - 1 credit
       	 10 - 2 to 8 credits
       	 11 - 9+ credits
         Max value for VN0 is reflected in this register. Actual value is set by a 
       different register. 
             
     */
    UINT32 vn1_credits_per_supported_nondata_vc : 2;
    /* vn1_credits_per_supported_nondata_vc - Bits[25:24], RO, default = 2'h2 
        
               VN1 credits per supported non-data VC
       	00 - 0 Credits (unsupported)01 - 1 credit
       	10 - 2 to 8 credits
       	11 - 9+ credits
               Default value based on processor type.  
             
     */
    UINT32 vn0_credits_per_supported_data_vc : 2;
    /* vn0_credits_per_supported_data_vc - Bits[27:26], RO, default = 2'h2 
        
               VN0 credits per supported Data VC
       	00 - 0 credits (unsupported)01 - 1 credit
       	10 - 2 to 8 credits
       	11 - 9+ credits
        Max value for VN0 is reflected in this register. Actual value is set by a 
       different register. 
             
     */
    UINT32 vn1_credits_per_supported_data_vc : 2;
    /* vn1_credits_per_supported_data_vc - Bits[29:28], RO, default = 2'h2 
        
       	VN1 credits per supported Data VC
       	00 - 0 Credits (unsupported)01 - 1 credit
       	10 - 2 to 8 credits
       	11 - 9+ credits
               Default value based on processor type.  
             
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPILCP_QPIAGENT_LL_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPILCL_QPIAGENT_LL_REG supported on:                                       */
/*       IVT_EP (0x40140044)                                                  */
/*       IVT_EX (0x40140044)                                                  */
/*       HSX (0x40140044)                                                     */
/*       BDX (0x40140044)                                                     */
/* Register default value:              0x00000000                            */
#define QPILCL_QPIAGENT_LL_REG 0x06004044
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * Intel QPI Link Control Register.
 * Note VN1 is only supported on the EX processor.
 */
typedef union {
  struct {
    UINT32 link_layer_soft_reset : 1;
    /* link_layer_soft_reset - Bits[0:0], RW_V, default = 1'b0 
        
               Link Layer Soft Reset
       Re-initialize and clear all link layer state including most status registers. 
       Write 1 to reset link - this is a destructive reset - when reset asserts, 
       register clears to 0. 
       Warning: LL Reset is destructive and no traffic can be flowing to Intel QPI port 
       until parameter exchange is complete. If this is not followed packet loss and/or 
       error logs may occur. 
       
             
     */
    UINT32 link_layer_hard_reset : 1;
    /* link_layer_hard_reset - Bits[1:1], RW_V, default = 1'b0 
        
               Link Layer Hard Reset
       Re-initialize and clear all link layer state, including all most registers that 
       have special reset requirement. Write 1 to reset link - this is a destructive 
       reset - when reset asserts, register clears to 0. 
       Warning: LL Reset is destructive and no traffic can be flowing to Intel QPI port 
       until parameter exchange is complete. If this is not followed packet loss and/or 
       error logs may occur. 
             
     */
    UINT32 max_num_phy_reinit : 2;
    /* max_num_phy_reinit - Bits[3:2], RWS, default = 2'b0 
        
               MAX_NUM_PHY_REINIT
       	Consecutive Phy ReInits to Retry_ABORT
       	00 - up to 2
       	01 - up to 1
       	10 - up to 0
       	11 - Reserved
             
     */
    UINT32 max_num_retry : 2;
    /* max_num_retry - Bits[5:4], RWS, default = 2'b0 
        
               MAX_NUM_RETRY
       	Consecutive LLRs to Phy ReInit
       	00 - 16
       	01 - 8
       	10 - 4
       	11 - 0, disable LLR (If CRC error then error condition immediately)
             
     */
    UINT32 llr : 2;
    /* llr - Bits[7:6], RWS, default = 2'b0 
        
               Link Layer Retry
       	Timeout value in terms of flits received
       	00 - 4095 flits
       	01 - 1023 flits
       	10 - 255 flits
       	11 - 63 flits
        Note: This timeout value must be set higher than the round-trip delay between 
       this device and the remote device.  
             
     */
    UINT32 advertised_vna_credits : 2;
    /* advertised_vna_credits - Bits[9:8], RWS, default = 2'b0 
        
               Advertised VNA credits (on next initialization)
       	00 - Max
       	01 - 64 if less than Max
       	10 - 32 if less than Max
       	11 - 0 Disable VNA
             
     */
    UINT32 advertised_vn0_credits_per_supported_vc : 2;
    /* advertised_vn0_credits_per_supported_vc - Bits[11:10], RWS, default = 2'b0 
        
               Advertised VN0 credits per supported VC
       	(on next initialization)
       	00 - Max
       	01 - 2 if less than Max
       	10 - 1 if less than Max
       	11 - 0 Disabled VN0 (Can cause deadlock)
       	Max is value set per message class.
               Default value based on processor type.  
             
     */
    UINT32 advertised_vn1_credits_per_supported_vc : 2;
    /* advertised_vn1_credits_per_supported_vc - Bits[13:12], RWS, default = 2'b0 
        
               Advertised VN1 credits per supported VC
       	(on next initialization)
       	00 - Max
       	01 - 2 if less than Max
       	10 - 1 if less than Max
       	11 - 0 Disabled VN1 (Can cause deadlock)
       	Max is value set  per message class.
               Default value based on processor type.  
             
     */
    UINT32 crc_mode : 2;
    /* crc_mode - Bits[15:14], RWS, default = 2'b0 
        
               CRC mode
       	(on next initialization)
       	00 - 8b CRC
       	01 - 16b rolling CRC, only enabled if peer agent also supports in Parameter0
       	1X - Reserved
             
     */
    UINT32 link_layer_initialization_stall_at_ready_for_init : 1;
    /* link_layer_initialization_stall_at_ready_for_init - Bits[16:16], RWS, default = 1'b0 
        
               Link Layer
       	Initialization stall at Ready_For_Init
       	0 - disable
        1 - enable, stall initialization (on next initialization) till this bit is 
       cleared. 
       	Link Initialization Status field in the QPILS Register will show
        0001: Internal Stall Link Initialization, if this bit is set and the local Link 
       Layer logic is stalled waiting for this bit to be cleared 
        Note: this bit is set and cleared only by software (no hardware clearing is 
       supported). 
             
     */
    UINT32 link_layer_initialization_stall_at_ready_for_normal : 1;
    /* link_layer_initialization_stall_at_ready_for_normal - Bits[17:17], RWS, default = 1'b0 
        
               Link Layer
       	Initialization stall at Ready_For_Normal
       	0 - disable
        1 - enable, stall initialization (on next initialization) till this bit is 
       cleared. 
       	Link Initialization Status field in the QPILS Register will show
        1001: Param_Ex_Done Stalled, if this bit is set and the local Link Layer logic 
       is stalled waiting for this bit to be cleared 
        Note: this bit is set and cleared only by software (no hardware clearing is 
       supported). 
             
     */
    UINT32 l0s_enable_mode_at_transmitter : 1;
    /* l0s_enable_mode_at_transmitter - Bits[18:18], RWS_L, default = 1'b0 
        
               L0s enable mode at transmitter
               L0s is not supported on the processor
             
     */
    UINT32 l0p_enable_mode_at_transmitter : 1;
    /* l0p_enable_mode_at_transmitter - Bits[19:19], RWS_L, default = 1'b0 
        
               L0p enable mode at transmitter
        Bit is ANDed with the parameter exchanged value for L0p to determine if the 
       link may enter L0p. 
       	0 - disable
       	1 - enable
       	This is NOT a bit that determines the capability of a device.
             
     */
    UINT32 l1_enable : 1;
    /* l1_enable - Bits[20:20], RWS_L, default = 1'b0 
        
               L1 enable
        Bit is ANDed with the parameter exchanged value for L1 to determine if the link 
       may enter L1. 
       	0 - disable
       	1 - enable
             
     */
    UINT32 l1_master_slave : 1;
    /* l1_master_slave - Bits[21:21], RWS, default = 1'b0 
        
       	L1 Master/Slave
        If the link power state of L1 is enabled, then there is one master and one 
       slave per link. The master may only issue single L1 requests, while the slave 
       can only issue single L1_Ack or L1_NAck responses for the corresponding request. 
        0 - Slave - This link transmitter bit is an L1 power state slave and should 
       respond to L1 transitions with an ACK or NACK. 
        1 - Master - This link transmitter bit is an L1 power state master and can 
       initiate an L1power state transition. 
             
     */
    UINT32 rsvd : 10;
    /* rsvd - Bits[31:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPILCL_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPILS_QPIAGENT_LL_REG supported on:                                        */
/*       IVT_EP (0x40140048)                                                  */
/*       IVT_EX (0x40140048)                                                  */
/*       HSX (0x40140048)                                                     */
/*       BDX (0x40140048)                                                     */
/* Register default value:              0x00000000                            */
#define QPILS_QPIAGENT_LL_REG 0x06004048


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.0.CFG.xml.
 * Intel QPI Link Status
 * Note VN1 is only supported on the EX processor
 *     
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 vn1_ncb_credits_at_receiver : 1;
    /* vn1_ncb_credits_at_receiver - Bits[2:2], RO_V, default = 1'b0 
        
               VN1 NCB Credits at receiver
        VN1 available credits for remote device to use in transmission of packets to 
       processor Rx. 
       	0: 0 Credits
       	1: 1+Credits
             
     */
    UINT32 vn1_ncs_credits_at_receiver : 1;
    /* vn1_ncs_credits_at_receiver - Bits[3:3], RO_V, default = 1'b0 
        
               VN1 NCS Credits at receiver
        VN1 available credits for remote device to use in transmission of packets to 
       processor Rx. 
       	0: 0 Credits
       	1: 1+ Credits
             
     */
    UINT32 vn1_drs_credits_at_receiver : 1;
    /* vn1_drs_credits_at_receiver - Bits[4:4], RO_V, default = 1'b0 
        
               VN1 DRS Credits at receiver
        VN1 available credits for remote device to use in transmission of packets to 
       processor Rx. 
       	0: 0 Credits
       	1: 1+Credits
             
     */
    UINT32 vn1_ndr_credits_at_receiver : 1;
    /* vn1_ndr_credits_at_receiver - Bits[5:5], RO_V, default = 1'b0 
        
       	VN1 NDR Credits at receiver
        VN1 available credits for remote device to use in transmission of packets to 
       processor Rx. 
       	0: 0 Credits
       	1: 1+ Credits
             
     */
    UINT32 vn1_hom_credits_at_receiver : 1;
    /* vn1_hom_credits_at_receiver - Bits[6:6], RO_V, default = 1'b0 
        
               VN1 Hom Credits at receiver
        VN1 available credits for remote device to use in transmission of packets to 
       processor Rx 
       	0: 0 Credits
       	1: 1+ Credits
             
     */
    UINT32 vn1_snp_credits_at_receiver : 1;
    /* vn1_snp_credits_at_receiver - Bits[7:7], RO_V, default = 1'b0 
        
               VN1 Snp Credits at receiver
        VN1 available credits for remote device to use in transmission of packets to 
       processor Rx. 
       	0: 0 Credits
       	1: 1+ Credits
             
     */
    UINT32 rsvd_8 : 2;
    /* rsvd_8 - Bits[9:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 vn0_ncb_credits_at_receiver : 1;
    /* vn0_ncb_credits_at_receiver - Bits[10:10], RO_V, default = 1'b0 
        
               VN0 NCB Credits at receiver
        VNA available credits for remote device to use in transmission of packets to 
       processor Rx. 
       	0: 0 Credits
       	1: 1+Credits
             
     */
    UINT32 vn0_ncs_credits_at_receiver : 1;
    /* vn0_ncs_credits_at_receiver - Bits[11:11], RO_V, default = 1'b0 
        
               VN0 NCS Credits at receiver
        VNA available credits for remote device to use in transmission of packets to 
       processor Rx. 
       	0: 0 Credits
       	1: 1+ Credits
             
     */
    UINT32 vn0_drs_credits_at_receiver : 1;
    /* vn0_drs_credits_at_receiver - Bits[12:12], RO_V, default = 1'b0 
        
               VN0 DRS Credits at receiver
        VNA available credits for remote device to use in transmission of packets to 
       processor Rx. 
       	0: 0 Credits
       	1: 1+Credits
             
     */
    UINT32 vn0_ndr_credits_at_receiver : 1;
    /* vn0_ndr_credits_at_receiver - Bits[13:13], RO_V, default = 1'b0 
        
       	VN0 NDR Credits at receiver
        VNA available credits for remote device to use in transmission of packets to 
       processor Rx. 
       	0: 0 Credits
       	1: 1+ Credits
             
     */
    UINT32 vn0_hom_credits_at_receiver : 1;
    /* vn0_hom_credits_at_receiver - Bits[14:14], RO_V, default = 1'b0 
        
               VN0 Hom Credits at receiver
        VNA available credits for remote device to use in transmission of packets to 
       processor Rx. 
       	0: 0 Credits
       	1: 1+ Credits
             
     */
    UINT32 vn0_snp_credits_at_receiver : 1;
    /* vn0_snp_credits_at_receiver - Bits[15:15], RO_V, default = 1'b0 
        
               VN0 Snp Credits at receiver
        VNA available credits for remote device to use in transmission of packets to 
       processor Rx. 
       	0: 0 Credits
       	1: 1+ Credits
             
     */
    UINT32 vna_credits_at_receiver : 3;
    /* vna_credits_at_receiver - Bits[18:16], RO_V, default = 3'b0 
        
               VNA credits at receiver
        VNA available credits for remote device to use in transmission of packets to 
       processor 
       	Rx.
       	000: 0 credits
       	001: 1-7 credits
       	010: 8-11 credits
       	011: 12-15 credits
       	100: 16-31 credits
       	101: 32-63 credits
       	110: 64-127 credits
       	111: 128+ credits
             
     */
    UINT32 last_link_level_retry_count : 2;
    /* last_link_level_retry_count - Bits[20:19], RO_V, default = 2'b0 
        
               Last Link Level Retry NUM_RETRY
       	Saturates at 11Number of Retries since last Link Init or Phy Reinit 
       	00: 0
       	01: 1
       	10: 2-15
       	11: greater than 15
             
     */
    UINT32 last_link_level_phyreinit_count : 1;
    /* last_link_level_phyreinit_count - Bits[21:21], RO_V, default = 1'b0 
        
               Last Link Level Retry NUM_PHY_REINIT
       	Saturates at 1Number of Phy ReInits since last Link Init
       	0: 0
       	1: 1+
             
     */
    UINT32 link_initialization_failure_count : 2;
    /* link_initialization_failure_count - Bits[23:22], RO_V, default = 2'b0 
        
               Link Initialization Failure Count
        Saturates at 011All Link Init state machine arcs going into RDY_FOR_INIT 
       excluding the arcs from NOT_RDY_FOR_INIT and from NORMAL_OPERATION. 
       	00: 0
       	01: 1
       	10: 2-15
       	11: greater than 15
             
     */
    UINT32 link_initialization_status : 4;
    /* link_initialization_status - Bits[27:24], RO_V, default = 4'b0 
        
               Link Initialization status
       	0000: Waiting for Physical Layer Ready
       	0001: Internal Stall Link Initialization
       	0010: Sending ReadyForInit
       	0011: Parameter Exchange
       	0100: Sending ReadyForNormalOperation
       	0101: Reserved
       	0110: Normal Operation
       	0111: Link Level Retry (LRSM)
       	1000: Link Abort (LRSM)
       	1001 = Param_Ex_Done Stalled
       	1010 = Param_Ex_Done Proceeding
       	1011 = WaitForNormal
       	1100 = LocalLinkReset
       	1100-1111 = Reserved
             
     */
    UINT32 link_layer_retry_queue_consumed : 3;
    /* link_layer_retry_queue_consumed - Bits[30:28], RO_V, default = 3'b0 
        
               Link Layer Retry Queue Consumed
        Number of Retry Queue entries currently consumed while waiting for ACKFlits 
       allocated 
       	000: 0 to 7
       	001: 8 to 15
       	010: 16 to 31
       	011: 32 to 63
       	100: 64 to 95
       	101: 96 to 127
       	110: 128 to 191
       	111: 192 to 255
             
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPILS_QPIAGENT_LL_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPILP0_QPIAGENT_LL_REG supported on:                                       */
/*       IVT_EP (0x4014004C)                                                  */
/*       IVT_EX (0x4014004C)                                                  */
/*       HSX (0x4014004C)                                                     */
/*       BDX (0x4014004C)                                                     */
/* Register default value:              0x00000000                            */
#define QPILP0_QPIAGENT_LL_REG 0x0600404C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * Intel QPI Link Parameter 0
 */
typedef union {
  struct {
    UINT32 parameter_0_from_peer_agent : 32;
    /* parameter_0_from_peer_agent - Bits[31:0], RO_V, default = 32'b0 
        
               Parameter 0 from peer agent. Cleared on any LL initialization.
             
     */
  } Bits;
  UINT32 Data;
} QPILP0_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPILP1_QPIAGENT_LL_REG supported on:                                       */
/*       IVT_EP (0x40140050)                                                  */
/*       IVT_EX (0x40140050)                                                  */
/*       HSX (0x40140050)                                                     */
/*       BDX (0x40140050)                                                     */
/* Register default value:              0x00000000                            */
#define QPILP1_QPIAGENT_LL_REG 0x06004050
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * Intel QPI Link Parameter 1
 */
typedef union {
  struct {
    UINT32 parameter_1_from_peer_agent : 32;
    /* parameter_1_from_peer_agent - Bits[31:0], RO_V, default = 32'b0 
        
               Parameter 1 from peer agent. Cleared on any LL initialization.
             
     */
  } Bits;
  UINT32 Data;
} QPILP1_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPILP2_QPIAGENT_LL_REG supported on:                                       */
/*       IVT_EP (0x40140054)                                                  */
/*       IVT_EX (0x40140054)                                                  */
/*       HSX (0x40140054)                                                     */
/*       BDX (0x40140054)                                                     */
/* Register default value:              0x00000000                            */
#define QPILP2_QPIAGENT_LL_REG 0x06004054
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * Intel QPI Link Parameter 2
 */
typedef union {
  struct {
    UINT32 parameter_2_from_peer_agent : 32;
    /* parameter_2_from_peer_agent - Bits[31:0], RO_V, default = 32'b0 
        
               Parameter 2 from peer agent. Cleared on any LL initialization.
             
     */
  } Bits;
  UINT32 Data;
} QPILP2_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPILP3_QPIAGENT_LL_REG supported on:                                       */
/*       IVT_EP (0x40140058)                                                  */
/*       IVT_EX (0x40140058)                                                  */
/*       HSX (0x40140058)                                                     */
/*       BDX (0x40140058)                                                     */
/* Register default value:              0x00000000                            */
#define QPILP3_QPIAGENT_LL_REG 0x06004058
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * Intel QPI Link Parameter 3
 */
typedef union {
  struct {
    UINT32 parameter_3_from_peer_agent : 32;
    /* parameter_3_from_peer_agent - Bits[31:0], RO_V, default = 32'b0 
        
               Parameter 3 from peer agent. Cleared on any LL initialization.
             
     */
  } Bits;
  UINT32 Data;
} QPILP3_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPILPOC0_QPIAGENT_LL_REG supported on:                                     */
/*       IVT_EP (0x4014005C)                                                  */
/*       IVT_EX (0x4014005C)                                                  */
/*       HSX (0x4014005C)                                                     */
/*       BDX (0x4014005C)                                                     */
/* Register default value:              0x00000000                            */
#define QPILPOC0_QPIAGENT_LL_REG 0x0600405C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * Intel QPI Link POC0
 */
typedef union {
  struct {
    UINT32 poc_0_from_peer_agent : 32;
    /* poc_0_from_peer_agent - Bits[31:0], RO_V, default = 32'b0 
        
               POC 0 from peer agent. Cleared on any LL initialization.
             
     */
  } Bits;
  UINT32 Data;
} QPILPOC0_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPILCRDC_QPIAGENT_LL_REG supported on:                                     */
/*       IVT_EP (0x40140064)                                                  */
/*       IVT_EX (0x40140064)                                                  */
/*       HSX (0x40140064)                                                     */
/*       BDX (0x40140064)                                                     */
/* Register default value on IVT_EP:    0x11111164                            */
/* Register default value on IVT_EX:    0x11111164                            */
/* Register default value on HSX:       0x11111162                            */
/* Register default value on BDX:       0x11111162                            */
#define QPILCRDC_QPIAGENT_LL_REG 0x06004064
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * 
 * Intel QPI Link Credit Control Register.
 * Note VN1 is only supported on the EX processor.
 * Registers controls what credits are defined for each message class on VN0 and 
 * VNA. These credits are made visible on Intel QPI during the initialize phase of 
 * in the link layer. The values programmed here must exist within the size limits. 
 * Incorrect programming can result in overflow of the receive queue. When 
 * returning credits on Intel QPI this register is used in conjunction with the 
 * Intel QPI standard register Intel QPI Link Control (QPILCL) to determine how 
 * many credits are returned. In other words, the values specified here act as the 
 * Max in the field descriptions for QPI[11:10] and QPI[9:8]. This value is 
 * captured and used by the Link Layer when exiting the parameter exchange. This 
 * state is referred to as Begin Normal Operation in QuickPath Interconnect (Intel 
 * QPI) Version 1.1 Specification, Revision 1.8a. 
 *     
 */
typedef union {
  struct {
    UINT32 vna_credits : 7;
    /* vna_credits - Bits[6:0], RWS, default = 7'h62 
        
               VNA credits
       	0 - 127 credits
             
     */
    UINT32 vn1_credit_init : 1;
    /* vn1_credit_init - Bits[7:7], RWS, default = 1'b0 
        
               Program to 1 to enable VN1 Link Credit Init
             
     */
    UINT32 vn0_snp_credits : 3;
    /* vn0_snp_credits - Bits[10:8], RWS, default = 3'h1 
        
               VN0 - Snp credits
       	Allowed values:0-7 credits
             
     */
    UINT32 rsvd_11 : 1;
    /* rsvd_11 - Bits[11:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 vn0_drs_credits : 3;
    /* vn0_drs_credits - Bits[14:12], RWS, default = 3'h1 
        
               VN0 - DRS credits
       	Allowed values:0-7 credits
             
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 vn0_ndr_credits : 3;
    /* vn0_ndr_credits - Bits[18:16], RWS, default = 3'h1 
        
               VN0 - NDR credits
       	Allowed values:0-7 credits
             
     */
    UINT32 rsvd_19 : 1;
    /* rsvd_19 - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 vn0_ncs_credits : 3;
    /* vn0_ncs_credits - Bits[22:20], RWS, default = 3'h1 
        
       	VN0 - NCS credits
       	Allowed values:0-7 credits
             
     */
    UINT32 rsvd_23 : 1;
    /* rsvd_23 - Bits[23:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 vn0_ncb_credits : 3;
    /* vn0_ncb_credits - Bits[26:24], RWS, default = 3'h1 
        
               VN0 - NCB credits
       	Allowed values:0-7 credits
             
     */
    UINT32 rsvd_27 : 1;
    /* rsvd_27 - Bits[27:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 vn0_hom_credits : 3;
    /* vn0_hom_credits - Bits[30:28], RWS, default = 3'h1 
        
               VN0 - Hom credits
       	Allowed values:0-7 credits
             
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPILCRDC_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPICRCERRCNT_QPIAGENT_LL_REG supported on:                                 */
/*       HSX (0x4014006C)                                                     */
/*       BDX (0x4014006C)                                                     */
/* Register default value:              0x00000000                            */
#define QPICRCERRCNT_QPIAGENT_LL_REG 0x0600406C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * 
 * Intel QPI CRC Error Counter
 * Register used for counter CRC errors in Intel QPI. 
 *     
 */
typedef union {
  struct {
    UINT32 counter : 31;
    /* counter - Bits[30:0], RW1CS, default = 31'b0 
        
       	Counter increments every time the Link Layer detects a CRC error.
             
     */
    UINT32 overflow : 1;
    /* overflow - Bits[31:31], RW1CS, default = 1'b0 
        
               Overflow bit - set when the number of detected CRC errors exceeds the 
       counter width. 
             
     */
  } Bits;
  UINT32 Data;
} QPICRCERRCNT_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* QPITXPARAM1_LATE_QPIAGENT_LL_REG supported on:                             */
/*       IVT_EP (0x40140070)                                                  */
/*       IVT_EX (0x40140070)                                                  */
/*       HSX (0x40140070)                                                     */
/*       BDX (0x40140070)                                                     */
/* Register default value:              0x00000001                            */
#define QPITXPARAM1_LATE_QPIAGENT_LL_REG 0x06004070
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * Intel QPI Link Tx Parameter 1 Late Action
 */
typedef union {
  struct {
    UINT32 crc_mode_receive_modes_support : 2;
    /* crc_mode_receive_modes_support - Bits[1:0], RO_V, default = 2'h1 
        
               01 - 8b CRC mode
               10 - 8b CRC and 16b rolling CRC mode
             
     */
    UINT32 rsvd_2 : 4;
    /* rsvd_2 - Bits[5:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 command_insert_interleave : 1;
    /* command_insert_interleave - Bits[6:6], RO, default = 1'b0 
        
               Command Insert Interleave
             
     */
    UINT32 l0s_power_state_support : 1;
    /* l0s_power_state_support - Bits[7:7], RO_V, default = 1'b0 
        
               L0s Power State Support
             
     */
    UINT32 l0p_power_state_support : 1;
    /* l0p_power_state_support - Bits[8:8], RO_V, default = 1'b0 
        
               L0p Power State Support
             
     */
    UINT32 l1_power_state_support : 1;
    /* l1_power_state_support - Bits[9:9], RO_V, default = 1'b0 
        
               L1 Power State Support
             
     */
    UINT32 crc_inversion_supported : 1;
    /* crc_inversion_supported - Bits[10:10], RO, default = 1'b0 
        
               CRC Inversion Supported
             
     */
    UINT32 rsvd_11 : 21;
    /* rsvd_11 - Bits[31:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPITXPARAM1_LATE_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPIERRDIS_QPIAGENT_LL_REG supported on:                                    */
/*       IVT_EP (0x40140074)                                                  */
/*       IVT_EX (0x40140074)                                                  */
/*       HSX (0x40140074)                                                     */
/*       BDX (0x40140074)                                                     */
/* Register default value on IVT_EP:    0x00200080                            */
/* Register default value on IVT_EX:    0x00200080                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define QPIERRDIS_QPIAGENT_LL_REG 0x06004074


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.0.CFG.xml.
 * 
 * Intel QPI Error Disable CSR.
 * Allows BIOS to disable error report and logging for correctable and 
 * uncorrectable error cases. 
 *     
 */
typedef union {
  struct {
    UINT32 unc_mask : 11;
    /* unc_mask - Bits[10:0], RWS, default = 11'b0 
        
       Uncorrectable Disable Mask
       Disable uncorrectable errors. See error code description in the Intel QPI MCA 
       machine check bank register for error code description. 
       bit - Error code (hex)
       0 - 00 Legacy indicator for Physical layer control errors.  The processor does 
       not have uncorrectable PHY control errors. 
       1 - 02 Reserved  
       2 - 03 Intel QPI Physical layer detected latency buffer rollover
       3 - 10 R3QPI control error (Intel QPI Link layer reports on behalf of R3)
       4 - 11 Rx entered LLR abort state on CRC error
       5 - 12 Link layer received an unsupported or undefined packet
       6 - 13 Intel QPI Link layer control error
       7 - 14 Legacy indicator for L0r errors.  The processor no longer has 
       uncorrectable L0r errors. 
       8 - 15 Route-back table (RBT) used an un-initialized value.
       9 - 16 Reserved.
       10 - 1F Intel QPI received a config access crossing the 32-bit boundary.
             
     */
    UINT32 rsvd_11 : 5;
    /* rsvd_11 - Bits[15:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 cor_mask : 7;
    /* cor_mask - Bits[22:16], RWS, default = 7'b0 
        
               Correctable Error Disable Mask
       If bit is set it disable MCA error logging signal for the given error code. See 
       the Intel QPI MCA machine check bank register for error code description. 
       Bit - Error Code (Hex)
       0 - 20 Intel QPI Physical layer detected an Intel QPI inband reset but aborted 
       initialization 
       1 - 21 Link failover: data self-healing
       2 - 22 Phy detected inband reset but recovered without a width change
       3 - 32 Reserved 
       4 - 30 Rx detected a CRC error and successfully recovered via LLR after 
       escalating to a Phy re-initialization 
       5 - 31 Rx detected a CRC error and successfully recovered via LLR without 
       escalating to a Phy re-init 
       6 - 23 Link failover: clock failover
             
     */
    UINT32 rsvd_23 : 8;
    /* rsvd_23 - Bits[30:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 smi_en : 1;
    /* smi_en - Bits[31:31], RW_LB, default = 1'b0 
        
       	smi enable
       SMI enable for clock / data failover.
             
     */
  } Bits;
  UINT32 Data;
} QPIERRDIS_QPIAGENT_LL_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIERRCNT0_QPIAGENT_LL_REG supported on:                                   */
/*       IVT_EP (0x40140078)                                                  */
/*       IVT_EX (0x40140078)                                                  */
/*       HSX (0x40140078)                                                     */
/*       BDX (0x40140078)                                                     */
/* Register default value:              0x00000000                            */
#define QPIERRCNT0_QPIAGENT_LL_REG 0x06004078
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * 
 * Intel QPI Error Counter 0 CSR.
 * Register used for counter error cases in Intel QPI. Primary usage is to 
 * characterize correctable errors. 
 *     
 */
typedef union {
  struct {
    UINT32 counter : 16;
    /* counter - Bits[15:0], RWS_V, default = 16'b0 
        
        Counter increments based on error enabled in the Mask register. Bit 15 is an 
       overflow bit, which remains sticky until cleared by software. 
       	Counter is cleared by writing ones to all 16-bits.
             
     */
    UINT32 mask : 16;
    /* mask - Bits[31:16], RWS, default = 16'b0 
        
               Error Mask
        This mask is used to select which bits are counted as part of error count. When 
       bit is set that error type is included in the Count. See Intel QPI MCA machine 
       check bank register definition for details on each error code. 
       
             	 Bit - Error Code
             	 0   - 0x20
             	 1   - 0x21 or 0x23
             	 2   - 0x22
             	 3   - 0x32
             	 4   - 0x30
             	 5   - 0x31
             	 6   - 0x00
             	 7   - 0x02
             	 8   - 0x03
             	 9   - 0x10
             	 10  - 0x11
             	 11  - 0x12
             	 12  - 0x13
             	 13  - 0x14
             	 14  - 0x15
             	 15  - 0x16 or 0x1f
             
     */
  } Bits;
  UINT32 Data;
} QPIERRCNT0_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPIERRCNT1_QPIAGENT_LL_REG supported on:                                   */
/*       IVT_EP (0x4014007C)                                                  */
/*       IVT_EX (0x4014007C)                                                  */
/*       HSX (0x4014007C)                                                     */
/*       BDX (0x4014007C)                                                     */
/* Register default value:              0x00000000                            */
#define QPIERRCNT1_QPIAGENT_LL_REG 0x0600407C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * 
 * Intel QPI Error Counter 1 CSR.
 * Register used for counter error cases in Intel QPI. Primary usage is to 
 * characterize correctable errors. 
 *     
 */
typedef union {
  struct {
    UINT32 counter : 16;
    /* counter - Bits[15:0], RWS_V, default = 16'b0 
        
        Counter increments based on error enabled in the Mask register. Bit 15 is an 
       overflow bit, which remains sticky until cleared by software. 
       	Counter is cleared by writing ones to all 16-bits.
             
     */
    UINT32 mask : 16;
    /* mask - Bits[31:16], RWS, default = 16'b0 
        
               Error Mask
        This mask is used to select which bits are counted as part of error count. When 
       bit is set that error type is included in the Count. See Intel QPI MCA machine 
       check bank register definition for details on each error code. 
       
             	 Bit - Error Code
             	 0   - 0x20
             	 1   - 0x21 or 0x23
             	 2   - 0x22
             	 3   - 0x32
             	 4   - 0x30
             	 5   - 0x31
             	 6   - 0x00
             	 7   - 0x02
             	 8   - 0x03
             	 9   - 0x10
             	 10  - 0x11
             	 11  - 0x12
             	 12  - 0x13
             	 13  - 0x14
             	 14  - 0x15
             	 15  - 0x16 or 0x1f
             
     */
  } Bits;
  UINT32 Data;
} QPIERRCNT1_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPIAGCTRL_QPIAGENT_LL_REG supported on:                                    */
/*       IVT_EP (0x40140080)                                                  */
/*       IVT_EX (0x40140080)                                                  */
/*       HSX (0x40140080)                                                     */
/*       BDX (0x40140080)                                                     */
/* Register default value on IVT_EP:    0x00008220                            */
/* Register default value on IVT_EX:    0x00008220                            */
/* Register default value on HSX:       0x01200020                            */
/* Register default value on BDX:       0x01200020                            */
#define QPIAGCTRL_QPIAGENT_LL_REG 0x06004080


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.0.CFG.xml.
 * Intel QPI Agent Control CSR.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    UINT32 direct_to_core_disable : 1;
    /* direct_to_core_disable - Bits[1:1], RW, default = 1'b0 
        
               Direct-to-core disable
       	Disables the direct-to-core flow from Intel QPI agent. 
       	0 - Enable
       	1 - Disable
             
     */
    UINT32 rsvd_2 : 1;
    UINT32 enable_snoop_spawn : 1;
    /* enable_snoop_spawn - Bits[3:3], RW, default = 1'b0 
        
               Enable Snoop Spawn
        In this mode snoops will tried to be spawn for Home Requests Received from 
       Intel QPI. 
        This mode can only be enabled when in 2S early snoop mode is enabled. It can 
       also be disabled to reduce AD crediting issues. 
             
     */
    UINT32 direct_to_core_threshold : 4;
    /* direct_to_core_threshold - Bits[7:4], RW, default = 4'h2 
        
               Direct-to-core threshold. If DRS credits to ring-stop drop below the 
       threshold then do not set direct-to-core. 
        This will use an additional credits when arbitrating for DRS packets. Legal 
       values are 0-8. 
        Expected value of 1-3 to optimize DRS bandwidth. Value of zero can be used to 
       minimize latency to core using direct-to-core even under heavy load. Value of 8 
       will disable direct-to-core. 
             
     */
    UINT32 rsvd_8 : 8;
    /* rsvd_8 - Bits[15:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_16 : 1;
    UINT32 disable_pe : 1;
    /* disable_pe - Bits[17:17], RW, default = 1'b0 
        
               Disable Priority Encoding bits. Setting this bit disabled PE bit 
       encoding in Intel QPI headers. PE bits should be clear when in an Isoc enabled 
       platform. Any platform which does not use Isoc should set this bit to 1. This 
       mode bit is equivalent functionality to SW_Isoc mode bit Intel QPI v1.1 spec 
       (but polarity inverted). 
       	0 - Allows PE bits (overloaded on DNID[4:3])
       	1 - Disable PE bits
        When PE bits are disabled Intel QPI Agent will check that DNID[4:3] are zero. 
       There is not support for NodeID bits 4:3, so if they are set an unsupported 
       packet error will be signalled when in this mode. 
             
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 num_of_cbos : 5;
    /* num_of_cbos - Bits[24:20], RW, default = 5'h12 
        
               Number of active Cbos on the socket.  Used for determining the CboID for 
       Tx snoops (address hashing). 
             
     */
    UINT32 cod_mode : 1;
    /* cod_mode - Bits[25:25], RW, default = 1'b0 
        
               Enables Cluster-on-die mode. (EP processor select SKU only)
             
     */
    UINT32 rsvd_26 : 1;
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIAGCTRL_QPIAGENT_LL_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * Intel QPI Agent Control CSR.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    UINT32 direct_to_core_disable : 1;
    /* direct_to_core_disable - Bits[1:1], RW, default = 1'b0 
        
               Direct-to-core disable
       	Disables the direct-to-core flow from Intel QPI agent. 
       	0 - Enable
       	1 - Disable
             
     */
    UINT32 rsvd_2 : 1;
    UINT32 enable_snoop_spawn : 1;
    /* enable_snoop_spawn - Bits[3:3], RW, default = 1'b0 
        
               Enable Snoop Spawn
        In this mode snoops will tried to be spawn for Home Requests Received from 
       Intel QPI. 
        This mode can only be enabled when in 2S early snoop mode is enabled. It can 
       also be disabled to reduce AD crediting issues. 
             
     */
    UINT32 direct_to_core_threshold : 4;
    /* direct_to_core_threshold - Bits[7:4], RW, default = 4'h2 
        
               Direct-to-core threshold. If DRS credits to ring-stop drop below the 
       threshold then do not set direct-to-core. 
        This will use an additional credits when arbitrating for DRS packets. Legal 
       values are 0-8. 
        Expected value of 1-3 to optimize DRS bandwidth. Value of zero can be used to 
       minimize latency to core using direct-to-core even under heavy load. Value of 8 
       will disable direct-to-core. 
             
     */
    UINT32 rsvd_8 : 8;
    /* rsvd_8 - Bits[15:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_16 : 1;
    UINT32 disable_pe : 1;
    /* disable_pe - Bits[17:17], RW, default = 1'b0 
        
               Disable Priority Encoding bits. Setting this bit disabled PE bit 
       encoding in Intel QPI headers. PE bits should be clear when in an Isoc enabled 
       platform. Any platform which does not use Isoc should set this bit to 1. This 
       mode bit is equivalent functionality to SW_Isoc mode bit Intel QPI v1.1 spec 
       (but polarity inverted). 
       	0 - Allows PE bits (overloaded on DNID[4:3])
       	1 - Disable PE bits
        When PE bits are disabled Intel QPI Agent will check that DNID[4:3] are zero. 
       There is not support for NodeID bits 4:3, so if they are set an unsupported 
       packet error will be signalled when in this mode. 
             
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 num_of_cbos : 5;
    /* num_of_cbos - Bits[24:20], RW, default = 5'h12 
        
               Number of active Cbos on the socket.  Used for determining the CboID for 
       Tx snoops (address hashing). 
             
     */
    UINT32 cod_mode : 1;
    /* cod_mode - Bits[25:25], RW, default = 1'b0 
        
               Enables Cluster-on-die mode. (EP processor select SKU only)
             
     */
    UINT32 rsvd_26 : 1;
    UINT32 config_4s : 1;
    /* config_4s - Bits[27:27], RW, default = 1'b0 
        
               Identifies 4S configuration.
       	0 = Non-4S config
       	1 = 4S config
             
     */
    UINT32 iodc_mode : 1;
    /* iodc_mode - Bits[28:28], RW, default = 1'b0 
         
       	This allows PE[1] to pass through.  
       	If (iodc_mode == 0)  PE[1]'s fate is dedcided by disable_pe 
        If (iodc_mode == 1)  Allows PE[1] to pass through despite of the value of 
       disable_pe 
             
     */
    UINT32 rsvd_29 : 3;
    /* rsvd_29 - Bits[31:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIAGCTRL_QPIAGENT_LL_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* QPIRBTSO_QPIAGENT_LL_REG supported on:                                     */
/*       IVT_EP (0x40140088)                                                  */
/*       IVT_EX (0x40140088)                                                  */
/*       HSX (0x40140088)                                                     */
/*       BDX (0x40140088)                                                     */
/* Register default value:              0x00000000                            */
#define QPIRBTSO_QPIAGENT_LL_REG 0x06004088
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * 
 * Intel QPI RBT Offset CSR. 
 * This register is used to pre-load the RBT. This register is setup to control 
 * offset of values data access in QPIRBTSD. 
 *     
 */
typedef union {
  struct {
    UINT32 rbt_offset : 7;
    /* rbt_offset - Bits[6:0], RW_V, default = 7'b0 
        
               This value will determine RBT offset that is used when RBT data is 
       accessed. On a read this value increments and wraps back to zero after all 1s. 
       The depth of the table is 128, so all values between 0-127 are valid.  
             
     */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 we : 1;
    /* we - Bits[8:8], RW, default = 1'b0 
        
               Bit must be set on every write to this register. If bit is unset on a 
       write then offset will NOT be loaded internally.  
       	The bit is auto cleared on cycle after it is set. 
             
     */
    UINT32 rsvd_9 : 23;
    /* rsvd_9 - Bits[31:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIRBTSO_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPIRBTSD_QPIAGENT_LL_REG supported on:                                     */
/*       IVT_EP (0x4014008C)                                                  */
/*       IVT_EX (0x4014008C)                                                  */
/*       HSX (0x4014008C)                                                     */
/*       BDX (0x4014008C)                                                     */
/* Register default value:              0x00000000                            */
#define QPIRBTSD_QPIAGENT_LL_REG 0x0600408C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.0.CFG.xml.
 * Intel QPI Static RBT Data CSR.
 */
typedef union {
  struct {
    UINT32 cbo_ringid : 5;
    /* cbo_ringid - Bits[4:0], RW_V, default = 4'b0 
        
               Fill Cbo RingID in the RBT for entry defined by RBT Offset. Zeros are 
       written to all other fields in RBT entry.  
             
     */
    UINT32 rsvd_5 : 1;
    /* rsvd_5 - Bits[5:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_6 : 1;
    UINT32 read_successful : 1;
    /* read_successful - Bits[7:7], RO_V, default = 1'b0 
        
               Bit indicates if RBT read completed successfully. If Intel QPI traffic 
       is flowing while reading, then it is possible Successful would be '0' (FALSE), 
       but if reads occurs while no Intel QPI traffic is flowing it should always be 
       '1' (TRUE).  
             
     */
    UINT32 we : 1;
    /* we - Bits[8:8], RW, default = 1'b0 
        
               Bit must be set on any write to RBT. This bit is used to trigger 
       internal update of data into the RBT structure.  
       	Bit auto clears the cycle after it is set. 
             
     */
    UINT32 offset : 7;
    /* offset - Bits[15:9], RO_V, default = 7'b0 
        
               Indicates the offset which was used for most recent RBT read. Used 
       primarily for debug.  
             
     */
    UINT32 rsvd_16 : 16;
    /* rsvd_16 - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIRBTSD_QPIAGENT_LL_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIQRT_QPIAGENT_LL_REG supported on:                                       */
/*       IVT_EP (0x40140090)                                                  */
/*       IVT_EX (0x40140090)                                                  */
/*       HSX (0x40140090)                                                     */
/*       BDX (0x40140090)                                                     */
/* Register default value on IVT_EP:    0x00000000                            */
/* Register default value on IVT_EX:    0x00000000                            */
/* Register default value on HSX:       0x0000FF00                            */
/* Register default value on BDX:       0x00000000                            */
#define QPIQRT_QPIAGENT_LL_REG 0x06004090
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * 
 * Intel QPI Agent Intel QPI Routing Table CSR.
 * Control Routing from Intel QPI Local vs. Remote Determination. 
 *     
 */
typedef union {
  struct {
    UINT32 local_vs_remote : 8;
    /* local_vs_remote - Bits[7:0], RW_LB, default = 8'b0 
        
               The Intel QPI agent decodes DNID of incoming packet is targeting the 
       local socket or remote. Remote packets will be forwarded to the peer Intel QPI 
       link. Info is used in Intel QPI Agent to target NDR packets to either AK ring 
       for local or AD ring for remote.  
       	0 - local NodeID 
       	1 - Remote NodeID 
       	bit 0 for DNID==0 
       	... 
       	bit 7 for DNID==7 
             
     */
    UINT32 inhibit_write : 8;
    /* inhibit_write - Bits[15:8], RW, default = 8'h00 
        
               This vector is used to statically disable HAs from using Direct2Core.  
       Each bit represents the socket ID for HAs in the system. 
               A value of '1' means HAs on that socket are not allowed to use D2C. 
             
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIQRT_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPIA2RCRCTRL_QPIAGENT_LL_REG supported on:                                 */
/*       IVT_EP (0x40140094)                                                  */
/*       IVT_EX (0x40140094)                                                  */
/*       HSX (0x40140094)                                                     */
/*       BDX (0x40140094)                                                     */
/* Register default value on IVT_EP:    0x111C91CE                            */
/* Register default value on IVT_EX:    0x111C91CE                            */
/* Register default value on HSX:       0x116A81CE                            */
/* Register default value on BDX:       0x116A81CE                            */
#define QPIA2RCRCTRL_QPIAGENT_LL_REG 0x06004094
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * 
 * Intel QPI Agent to Ring Credit Control CSR.
 * This register is used to control credits flow from Intel QPI agent to R3QPI 
 * targeting the ring. The values in this register take affect on next Link Layer 
 * initialization, which should normally occur as part of CPU-only Reset or Warm 
 * Reset during the platform reset.  
 *       
 *     
 */
typedef union {
  struct {
    UINT32 home : 5;
    /* home - Bits[4:0], RWS, default = 5'hE 
        
               Credits given to the Intel QPI link layer to send HOM VN0 packets to the 
       R3QPI AD flow control queue.   
        If VN1 is enabled, LL additionally gets this number of credits to send HOM VN1 
       to R3 AD flow control queue. 
             
     */
    UINT32 snp : 5;
    /* snp - Bits[9:5], RWS, default = 5'hE 
        
        Credits given to the Intel QPI link layer to send SNP VN0 packets to the R3QPI 
       AD flow control queue.   
        If VN1 is enabled, LL additionally gets this number of credits to send SNP VN1 
       to R3 AD flow control queue.          
             
     */
    UINT32 bl_shr : 3;
    /* bl_shr - Bits[12:10], RWS, default = 3'h0 
        
        Credits given to the Intel QPI link layer to send packets from any VN and a BL 
       message class to the R3QPI BL egress.   
       	Link layer consumes these before using VN0/1 and BL MC-specific credits.
             
     */
    UINT32 ndr_ad : 4;
    /* ndr_ad - Bits[16:13], RWS, default = 4'h4 
        
               Credits given to the Intel QPI link layer to send NDR VN0 packets to the 
       R3QPI AD flow control queue.   
        If VN1 is enabled, LL additionally gets this number of credits to send NDR VN1 
       to R3 AD flow control queue. 
             
     */
    UINT32 ndr_ak : 3;
    /* ndr_ak - Bits[19:17], RWS, default = 3'h5 
        
               Credits given to the Intel QPI link layer to send NDR packets to the 
       R3QPI AK egress.   
       	If VN1 is enabled, this is shared among VN0 and VN1 messages.
             
     */
    UINT32 drs : 4;
    /* drs - Bits[23:20], RWS, default = 4'h6 
        
               Credits given to the Intel QPI link layer to send DRS VN0 packets to the 
       R3QPI BL egress.   
        If VN1 is enabled, LL additionally gets this number of credits to send DRS VN1 
       to R3 BL egress. 
             
     */
    UINT32 ncb : 4;
    /* ncb - Bits[27:24], RWS, default = 4'h1 
        
               Credits given to the Intel QPI link layer to send NCB VN0 packets to the 
       R3QPI BL egress.   
        If VN1 is enabled, LL additionally gets this number of credits to send NCB VN1 
       to R3 BL egress. 
             
     */
    UINT32 ncs : 4;
    /* ncs - Bits[31:28], RWS, default = 4'h1 
        
                Credits given to the Intel QPI link layer to send NCS VN0 packets to 
       the R3QPI BL egress.   
         If VN1 is enabled (EX processor only), LL additionally gets this number of 
       credits to send NCS VN1 to R3 BL egress. 
             
     */
  } Bits;
  UINT32 Data;
} QPIA2RCRCTRL_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPICTRTXPF_QPIAGENT_LL_REG supported on:                                   */
/*       IVT_EP (0x401400A0)                                                  */
/*       IVT_EX (0x401400A0)                                                  */
/*       HSX (0x401400A0)                                                     */
/*       BDX (0x401400A0)                                                     */
/* Register default value:              0x00000000                            */
#define QPICTRTXPF_QPIAGENT_LL_REG 0x060040A0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * 
 * Intel QPI Counter Tx Protocol Flits CSR.
 * Register used by P limit and power meter.
 *     
 */
typedef union {
  struct {
    UINT32 count_tx_protocol_flit : 32;
    /* count_tx_protocol_flit - Bits[31:0], RW_LV, default = 32'b0 
        
        Free running count of total Protocol flits in Tx. This count does not include 
       link layer only flits (NULL, Parameters, control, ect).  
             
     */
  } Bits;
  UINT32 Data;
} QPICTRTXPF_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPICTRRXPF_QPIAGENT_LL_REG supported on:                                   */
/*       IVT_EP (0x401400A4)                                                  */
/*       IVT_EX (0x401400A4)                                                  */
/*       HSX (0x401400A4)                                                     */
/*       BDX (0x401400A4)                                                     */
/* Register default value:              0x00000000                            */
#define QPICTRRXPF_QPIAGENT_LL_REG 0x060040A4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * 
 * Intel QPI Counter Rx Protocol Flits CSR.
 * Register used by P limit and power meter. 
 *     
 */
typedef union {
  struct {
    UINT32 count_rx_protocol_flit : 32;
    /* count_rx_protocol_flit - Bits[31:0], RW_LV, default = 32'b0 
        
               Free running count of total Protocol flits in Rx. This count does not 
       include link layer only flits (NULL, Parameters, control, ect).  
        Note: this counter only tracks based on the internal packet format that is sent 
       to R3QPI.   
                     All SNP/HOM/NDR packets are counted as 1 flit.
                     All DRS/NCB/NCS packets are counted as 9 flits.
             
     */
  } Bits;
  UINT32 Data;
} QPICTRRXPF_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPICTRCCLK_QPIAGENT_LL_REG supported on:                                   */
/*       HSX (0x401400A8)                                                     */
/*       BDX (0x401400A8)                                                     */
/* Register default value:              0x00000000                            */
#define QPICTRCCLK_QPIAGENT_LL_REG 0x060040A8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * 
 * Intel QPI Cclk Counter CSR.
 * Counts Cclk cycles when not gated for L1+Vret.
 *     
 */
typedef union {
  struct {
    UINT32 count_cclk : 32;
    /* count_cclk - Bits[31:0], RW_LV, default = 31'b0 
        
               Free running count of CClk. 
       	Lock is NOT(PwrMtrUnlock). 
             
     */
  } Bits;
  UINT32 Data;
} QPICTRCCLK_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* QPICTRL1_QPIAGENT_LL_REG supported on:                                     */
/*       IVT_EP (0x401400AC)                                                  */
/*       IVT_EX (0x401400AC)                                                  */
/*       HSX (0x401400AC)                                                     */
/*       BDX (0x401400AC)                                                     */
/* Register default value:              0x00000000                            */
#define QPICTRL1_QPIAGENT_LL_REG 0x060040AC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * 
 * Intel QPI Counter L1 CSR.
 * Register used by P limit and power meter. 
 *     
 */
typedef union {
  struct {
    UINT32 count_l1 : 23;
    /* count_l1 - Bits[22:0], RW_LV, default = 23'b0 
        
               Free running count of CSClk in L1 link state. 
             
     */
    UINT32 rsvd : 9;
    /* rsvd - Bits[31:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPICTRL1_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */






/* QPICTRRXL0P_QPIAGENT_LL_REG supported on:                                  */
/*       IVT_EP (0x401400B8)                                                  */
/*       IVT_EX (0x401400B8)                                                  */
/*       HSX (0x401400B8)                                                     */
/*       BDX (0x401400B8)                                                     */
/* Register default value:              0x00000000                            */
#define QPICTRRXL0P_QPIAGENT_LL_REG 0x060040B8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * 
 * Intel QPI Counter Rx L0p CSR.
 * Register used by P limit and power meter. 
 *     
 */
typedef union {
  struct {
    UINT32 count_rx_l0p : 23;
    /* count_rx_l0p - Bits[22:0], RW_LV, default = 23'b0 
        
               Free running count of CSClk in Rx side link in L0p state. 
             
     */
    UINT32 rsvd : 9;
    /* rsvd - Bits[31:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPICTRRXL0P_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPICTRTXL0P_QPIAGENT_LL_REG supported on:                                  */
/*       IVT_EP (0x401400BC)                                                  */
/*       IVT_EX (0x401400BC)                                                  */
/*       HSX (0x401400BC)                                                     */
/*       BDX (0x401400BC)                                                     */
/* Register default value:              0x00000000                            */
#define QPICTRTXL0P_QPIAGENT_LL_REG 0x060040BC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * 
 * Intel QPI Counter Tx L0p CSR.
 * Register used by P limit and power meter. 
 *     
 */
typedef union {
  struct {
    UINT32 count_tx_l0p : 23;
    /* count_tx_l0p - Bits[22:0], RW_LV, default = 23'b0 
        
               Free running count of CSClk in Tx side link in L0p state. 
             
     */
    UINT32 rsvd : 9;
    /* rsvd - Bits[31:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPICTRTXL0P_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPIPCSTS_QPIAGENT_LL_REG supported on:                                     */
/*       IVT_EP (0x401400C0)                                                  */
/*       IVT_EX (0x401400C0)                                                  */
/*       HSX (0x401400C0)                                                     */
/*       BDX (0x401400C0)                                                     */
/* Register default value:              0x00000000                            */
#define QPIPCSTS_QPIAGENT_LL_REG 0x060040C0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * 
 * Intel QPI Pcode Status CSR.
 * Regsister is used by PCode to store link training status.
 *     
 */
typedef union {
  struct {
    UINT32 ll_status : 4;
    /* ll_status - Bits[3:0], RW, default = 4'b0 
        
               Link Layer status registers that is written by Pcode. Encoding is the 
       same as QPILS.LinkStatus  
             
     */
    UINT32 rsvd_4 : 28;
  } Bits;
  UINT32 Data;
} QPIPCSTS_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */




/* QPIMISCSTAT_QPIAGENT_LL_REG supported on:                                  */
/*       IVT_EP (0x401400D4)                                                  */
/*       IVT_EX (0x401400D4)                                                  */
/*       HSX (0x401400D4)                                                     */
/*       BDX (0x401400D4)                                                     */
/* Register default value:              0x00000003                            */
#define QPIMISCSTAT_QPIAGENT_LL_REG 0x060040D4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * QPI Misc Status
 */
typedef union {
  struct {
    UINT32 qpi_rate : 3;
    /* qpi_rate - Bits[2:0], RO_V, default = 3'b011 
       
       	This reflects the current Intel QPI rate setting into the PLL.
       	010 - 5.6 GT/s
       	011 - 6.4 GT/s
       	100 - 7.2 GT/s
       	101 - 8 GT/s
       	110 - 8.8 GT/s
       	111 - 9.6 GT/s
       	other - Reserved
             
     */
    UINT32 rsvd : 29;
    /* rsvd - Bits[31:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIMISCSTAT_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */




/* QPIMISCCLKCTRL_QPIAGENT_LL_REG supported on:                               */
/*       IVT_EP (0x401400EC)                                                  */
/*       IVT_EX (0x401400EC)                                                  */
/*       HSX (0x401400EC)                                                     */
/*       BDX (0x401400EC)                                                     */
/* Register default value on IVT_EP:    0x00000038                            */
/* Register default value on IVT_EX:    0x00000038                            */
/* Register default value on HSX:       0x00000020                            */
/* Register default value on BDX:       0x00000020                            */
#define QPIMISCCLKCTRL_QPIAGENT_LL_REG 0x060040EC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.0.CFG.xml.
 * This CSR is not implemented in device 9, and for device 9 is shared with device 
 * 8. 
 */
typedef union {
  struct {
    UINT32 disable_agent_chain : 1;
    /* disable_agent_chain - Bits[0:0], RWS_LB, default = 1'b0 
       If set disables the 36-bit section of Agent Config Chain.
     */
    UINT32 rsvd_1 : 1;
    UINT32 rsvd_2 : 3;
    /* rsvd_2 - Bits[4:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 qpi_misc_csr_clkgat_en : 1;
    /* qpi_misc_csr_clkgat_en - Bits[5:5], RWS_LB, default = 1'b1 
       clock gate enable for QPI misc CSR and CSR chain
     */
    UINT32 rsvd_6 : 26;
    /* rsvd_6 - Bits[31:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIMISCCLKCTRL_QPIAGENT_LL_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIPOCREDTHR_QPIAGENT_LL_REG supported on:                                 */
/*       IVT_EP (0x201400F0)                                                  */
/*       IVT_EX (0x201400F0)                                                  */
/*       HSX (0x201400F0)                                                     */
/*       BDX (0x201400F0)                                                     */
/* Register default value:              0x0000                                */
#define QPIPOCREDTHR_QPIAGENT_LL_REG 0x060020F0


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.0.CFG.xml.
 * 
 *       Intel QPI Power Optimization Credit Threshold
 *       This register sets the number of return credits that need to accumulate 
 * before pulling the link out of L1. Also, when above the programmed threshold, 
 * the link will be prevented from entering L1.  L0s is not supported on the 
 * processor. 
 *     
 */
typedef union {
  struct {
    UINT16 not_used : 1;
    /* not_used - Bits[0:0], RO, default = 1'b0 
        
       	Reserved
             
     */
    UINT16 rsvd : 15;
    /* rsvd - Bits[15:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT16 Data;
} QPIPOCREDTHR_QPIAGENT_LL_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIPOACKTHR_QPIAGENT_LL_REG supported on:                                  */
/*       IVT_EP (0x101400F2)                                                  */
/*       IVT_EX (0x101400F2)                                                  */
/*       HSX (0x101400F2)                                                     */
/*       BDX (0x101400F2)                                                     */
/* Register default value:              0x00                                  */
#define QPIPOACKTHR_QPIAGENT_LL_REG 0x060010F2


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.0.CFG.xml.
 * 
 *       Intel QPI Power Optimization Acknowledge Threshold
 *       This register sets the number of acknowledges that need to accumulate 
 * before pulling the link out of L1. Also, when above the programmed threshold, 
 * the link will be prevented from entering L1.  L0s is not supported on the 
 * processor. 
 *     
 */
typedef union {
  struct {
    UINT8 not_used : 1;
    /* not_used - Bits[0:0], RO, default = 1'b0 
        
       	Reserved
             
     */
    UINT8 rsvd : 7;
    /* rsvd - Bits[7:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} QPIPOACKTHR_QPIAGENT_LL_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPITXQTHR_QPIAGENT_LL_REG supported on:                                    */
/*       IVT_EP (0x101400F3)                                                  */
/*       IVT_EX (0x101400F3)                                                  */
/*       HSX (0x101400F3)                                                     */
/*       BDX (0x101400F3)                                                     */
/* Register default value:              0x30                                  */
#define QPITXQTHR_QPIAGENT_LL_REG 0x060010F3
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * 
 * Intel QPI Tx Queue Threshold CSR.
 * Register defines tx Threshold used from R3QPI to Intel QPI Agent. The values in 
 * this register are used for error checking.  
 * No separate register is included here for L0p because corner cases around 
 * L0->L0p->L0 transitions make it difficult to exactly track the threshold value.  
 *     
 */
typedef union {
  struct {
    UINT8 l0_thr : 6;
    /* l0_thr - Bits[5:0], RW, default = 6'h30 
        
               Theshold value used when in L0 state. This register is used for error 
       checking of the Tx Data flow.  
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} QPITXQTHR_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */


/* QPI_DFX_LCK_CTL_CFG_QPIAGENT_LL_REG supported on:                          */
/*       IVT_EP (0x401400F4)                                                  */
/*       IVT_EX (0x401400F4)                                                  */
/*       HSX (0x401400F4)                                                     */
/*       BDX (0x401400F4)                                                     */
/* Register default value:              0x00000007                            */
#define QPI_DFX_LCK_CTL_CFG_QPIAGENT_LL_REG 0x060040F4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.0.CFG.xml.
 * 
 *       Intel QPI DFX lock control in cfg space
 *     
 */
typedef union {
  struct {
    UINT32 privdbg_phy_l0_sync_lck : 1;
    /* privdbg_phy_l0_sync_lck - Bits[0:0], RWS_LB, default = 1'b1 
        
               phy L0 Sync csr lock
             
     */
    UINT32 reutenglck : 1;
    /* reutenglck - Bits[1:1], RWS_LB, default = 1'b1 
        
               reut eng csr lock
             
     */
    UINT32 reutplck : 1;
    /* reutplck - Bits[2:2], RWS_LB, default = 1'b1 
        
               reut eng ia spce csr lock
             
     */
    UINT32 rsvd : 29;
    /* rsvd - Bits[31:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPI_DFX_LCK_CTL_CFG_QPIAGENT_LL_STRUCT;
#endif /* ASM_INC */



/* QPIPHYPWRCTRL_QPIAGENT_LL_REG supported on:                                */
/*       IVT_EP (0x101400FC)                                                  */
/*       IVT_EX (0x101400FC)                                                  */
/*       HSX (0x401400FC)                                                     */
/*       BDX (0x401400FC)                                                     */
/* Register default value on IVT_EP:    0x00                                  */
/* Register default value on IVT_EX:    0x00                                  */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define QPIPHYPWRCTRL_QPIAGENT_LL_REG 0x0600C000


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.0.CFG.xml.
 * QPI Phy power control CSR.
 */
typedef union {
  struct {
    UINT32 force_l1 : 1;
    /* force_l1 - Bits[0:0], RWS_LB, default = 1'b0 
        
        Used in Hot Add QPI port. Set by BIOS to force QPI port to report L1 to PMA 
       when waiting for add on device. 
             
     */
    UINT32 rsvd : 31;
    /* rsvd - Bits[31:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPIPHYPWRCTRL_QPIAGENT_LL_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* SVID_QPIAGENT_LL_REG supported on:                                         */
/*       IVT_EP (0x2014002C)                                                  */
/*       IVT_EX (0x2014002C)                                                  */
/* Register default value:              0x8086                                */
#define SVID_QPIAGENT_LL_REG 0x0600202C



/* SDID_QPIAGENT_LL_REG supported on:                                         */
/*       IVT_EP (0x2014002E)                                                  */
/*       IVT_EX (0x2014002E)                                                  */
/* Register default value:              0x0000                                */
#define SDID_QPIAGENT_LL_REG 0x0600202E



/* QPILCL_LATE_QPIAGENT_LL_REG supported on:                                  */
/*       IVT_EP (0x40140060)                                                  */
/*       IVT_EX (0x40140060)                                                  */
/* Register default value:              0x00000000                            */
#define QPILCL_LATE_QPIAGENT_LL_REG 0x06004060



/* QPILCRDC_LATE_QPIAGENT_LL_REG supported on:                                */
/*       IVT_EP (0x40140068)                                                  */
/*       IVT_EX (0x40140068)                                                  */
/* Register default value:              0x00000000                            */
#define QPILCRDC_LATE_QPIAGENT_LL_REG 0x06004068



/* QPIROTCTRL0_QPIAGENT_LL_REG supported on:                                  */
/*       IVT_EP (0x40140084)                                                  */
/*       IVT_EX (0x40140084)                                                  */
/* Register default value:              0x00000000                            */
#define QPIROTCTRL0_QPIAGENT_LL_REG 0x06004084



/* QPIA2RCRCTRL_LATE_QPIAGENT_LL_REG supported on:                            */
/*       IVT_EP (0x40140098)                                                  */
/*       IVT_EX (0x40140098)                                                  */
/* Register default value:              0x00000000                            */
#define QPIA2RCRCTRL_LATE_QPIAGENT_LL_REG 0x06004098



/* QPIROTLMT0_QPIAGENT_LL_REG supported on:                                   */
/*       IVT_EP (0x4014009C)                                                  */
/*       IVT_EX (0x4014009C)                                                  */
/* Register default value:              0x00000000                            */
#define QPIROTLMT0_QPIAGENT_LL_REG 0x0600409C



/* QPICTRCSCLK_QPIAGENT_LL_REG supported on:                                  */
/*       IVT_EP (0x401400A8)                                                  */
/*       IVT_EX (0x401400A8)                                                  */
/* Register default value:              0x00000000                            */
#define QPICTRCSCLK_QPIAGENT_LL_REG 0x060040A8



/* QPIROTCTRL1_QPIAGENT_LL_REG supported on:                                  */
/*       IVT_EP (0x401400C4)                                                  */
/*       IVT_EX (0x401400C4)                                                  */
/* Register default value:              0x00000000                            */
#define QPIROTCTRL1_QPIAGENT_LL_REG 0x060040C4



/* QPIROTCTRL2_QPIAGENT_LL_REG supported on:                                  */
/*       IVT_EP (0x401400C8)                                                  */
/*       IVT_EX (0x401400C8)                                                  */
/* Register default value:              0x00000000                            */
#define QPIROTCTRL2_QPIAGENT_LL_REG 0x060040C8



/* QPIROTCTRL3_QPIAGENT_LL_REG supported on:                                  */
/*       IVT_EP (0x401400CC)                                                  */
/*       IVT_EX (0x401400CC)                                                  */
/* Register default value:              0x00000000                            */
#define QPIROTCTRL3_QPIAGENT_LL_REG 0x060040CC



/* QPIVIRAL_QPIAGENT_LL_REG supported on:                                     */
/*       IVT_EP (0x401400D8)                                                  */
/*       IVT_EX (0x401400D8)                                                  */
/* Register default value:              0x00000000                            */
#define QPIVIRAL_QPIAGENT_LL_REG 0x060040D8



/* QPIROTLMT1_QPIAGENT_LL_REG supported on:                                   */
/*       IVT_EP (0x401400E0)                                                  */
/*       IVT_EX (0x401400E0)                                                  */
/* Register default value:              0x00000000                            */
#define QPIROTLMT1_QPIAGENT_LL_REG 0x060040E0



/* QPIROTLMT2_QPIAGENT_LL_REG supported on:                                   */
/*       IVT_EP (0x401400E4)                                                  */
/*       IVT_EX (0x401400E4)                                                  */
/* Register default value:              0x00000000                            */
#define QPIROTLMT2_QPIAGENT_LL_REG 0x060040E4



/* QPIROTLMT3_QPIAGENT_LL_REG supported on:                                   */
/*       IVT_EP (0x401400E8)                                                  */
/*       IVT_EX (0x401400E8)                                                  */
/* Register default value:              0x00000000                            */
#define QPIROTLMT3_QPIAGENT_LL_REG 0x060040E8



/* QPIPHYPWRCTRL_LATE_QPIAGENT_LL_REG supported on:                           */
/*       IVT_EP (0x101400FE)                                                  */
/*       IVT_EX (0x101400FE)                                                  */
/* Register default value:              0x00                                  */
#define QPIPHYPWRCTRL_LATE_QPIAGENT_LL_REG 0x060010FE



#endif /* QPIAGENT_LL_h */
