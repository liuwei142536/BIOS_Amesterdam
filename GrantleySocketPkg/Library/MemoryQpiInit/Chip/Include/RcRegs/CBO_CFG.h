/* Date Stamp: 8/23/2014 */

#ifndef CBO_CFG_h
#define CBO_CFG_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For all target CPUs:                                                       */
/* CBO_CFG_DEV 12                                                             */
/* CBO_CFG_FUN 0                                                              */

/* VID_CBO_CFG_REG supported on:                                              */
/*       IVT_EP (0x20160000)                                                  */
/*       IVT_EX (0x20160000)                                                  */
/*       HSX (0x20160000)                                                     */
/*       BDX (0x20160000)                                                     */
/* Register default value:              0x8086                                */
#define VID_CBO_CFG_REG 0x00002000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
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
} VID_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* DID_CBO_CFG_REG supported on:                                              */
/*       IVT_EP (0x20160002)                                                  */
/*       IVT_EX (0x20160002)                                                  */
/*       HSX (0x20160002)                                                     */
/*       BDX (0x20160002)                                                     */
/* Register default value on IVT_EP:    0x0EE0                                */
/* Register default value on IVT_EX:    0x0EE0                                */
/* Register default value on HSX:       0x2FE0                                */
/* Register default value on BDX:       0x6FE0                                */
#define DID_CBO_CFG_REG 0x00002002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6FE0 
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
} DID_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* PCICMD_CBO_CFG_REG supported on:                                           */
/*       HSX (0x20160004)                                                     */
/*       BDX (0x20160004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_CBO_CFG_REG 0x00002004

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
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
} PCICMD_CBO_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PCISTS_CBO_CFG_REG supported on:                                           */
/*       HSX (0x20160006)                                                     */
/*       BDX (0x20160006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_CBO_CFG_REG 0x00002006

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
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
} PCISTS_CBO_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RID_CBO_CFG_REG supported on:                                              */
/*       IVT_EP (0x10160008)                                                  */
/*       IVT_EX (0x10160008)                                                  */
/*       HSX (0x10160008)                                                     */
/*       BDX (0x10160008)                                                     */
/* Register default value:              0x00                                  */
#define RID_CBO_CFG_REG 0x00001008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
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
} RID_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_CBO_CFG_REG supported on:                                           */
/*       IVT_EP (0x10160009)                                                  */
/*       IVT_EX (0x10160009)                                                  */
/*       HSX (0x10160009)                                                     */
/*       BDX (0x10160009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_CBO_CFG_REG 0x00001009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.12.0.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_CBO_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_CBO_CFG_REG supported on:                                           */
/*       IVT_EP (0x2016000A)                                                  */
/*       IVT_EX (0x2016000A)                                                  */
/*       HSX (0x2016000A)                                                     */
/*       BDX (0x2016000A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_CBO_CFG_REG 0x0000200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
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
} CCR_N1_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* CLSR_CBO_CFG_REG supported on:                                             */
/*       IVT_EP (0x1016000C)                                                  */
/*       IVT_EX (0x1016000C)                                                  */
/*       HSX (0x1016000C)                                                     */
/*       BDX (0x1016000C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_CBO_CFG_REG 0x0000100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
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
} CLSR_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* PLAT_CBO_CFG_REG supported on:                                             */
/*       HSX (0x1016000D)                                                     */
/*       BDX (0x1016000D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_CBO_CFG_REG 0x0000100D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
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
} PLAT_CBO_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* HDR_CBO_CFG_REG supported on:                                              */
/*       IVT_EP (0x1016000E)                                                  */
/*       IVT_EX (0x1016000E)                                                  */
/*       HSX (0x1016000E)                                                     */
/*       BDX (0x1016000E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_CBO_CFG_REG 0x0000100E


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.12.0.CFG.xml.
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
} HDR_CBO_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* BIST_CBO_CFG_REG supported on:                                             */
/*       HSX (0x1016000F)                                                     */
/*       BDX (0x1016000F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_CBO_CFG_REG 0x0000100F

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
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
} BIST_CBO_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SVID_CBO_CFG_REG supported on:                                             */
/*       IVT_EP (0x2016002C)                                                  */
/*       IVT_EX (0x2016002C)                                                  */
/*       HSX (0x2016002C)                                                     */
/*       BDX (0x2016002C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_CBO_CFG_REG 0x0000202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * PCI Subsystem Vendor ID Register
 */
typedef union {
  struct {
    UINT16 subsystem_vendor_identification_number : 16;
    /* subsystem_vendor_identification_number - Bits[15:0], RW_O, default = 16'b1000000010000110 
       The default value specifies Intel but can be set to any value once after reset.
     */
  } Bits;
  UINT16 Data;
} SVID_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* SDID_CBO_CFG_REG supported on:                                             */
/*       IVT_EP (0x2016002E)                                                  */
/*       IVT_EX (0x2016002E)                                                  */
/*       HSX (0x2016002E)                                                     */
/*       BDX (0x2016002E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_CBO_CFG_REG 0x0000202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * PCI Subsystem device ID Register
 */
typedef union {
  struct {
    UINT16 subsystem_device_identification_number : 16;
    /* subsystem_device_identification_number - Bits[15:0], RW_O, default = 16'b0000000000000000 
       Assigned by the subsystem vendor to uniquely identify the subsystem
     */
  } Bits;
  UINT16 Data;
} SDID_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_CBO_CFG_REG supported on:                                           */
/*       HSX (0x10160034)                                                     */
/*       BDX (0x10160034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_CBO_CFG_REG 0x00001034

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
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
} CAPPTR_CBO_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTL_CBO_CFG_REG supported on:                                             */
/*       HSX (0x1016003C)                                                     */
/*       BDX (0x1016003C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_CBO_CFG_REG 0x0000103C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
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
} INTL_CBO_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTPIN_CBO_CFG_REG supported on:                                           */
/*       HSX (0x1016003D)                                                     */
/*       BDX (0x1016003D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_CBO_CFG_REG 0x0000103D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
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
} INTPIN_CBO_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MINGNT_CBO_CFG_REG supported on:                                           */
/*       HSX (0x1016003E)                                                     */
/*       BDX (0x1016003E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_CBO_CFG_REG 0x0000103E

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
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
} MINGNT_CBO_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MAXLAT_CBO_CFG_REG supported on:                                           */
/*       HSX (0x1016003F)                                                     */
/*       BDX (0x1016003F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_CBO_CFG_REG 0x0000103F

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
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
} MAXLAT_CBO_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RTID_CONFIG_POOL01_SIZE_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x40160040)                                                  */
/*       IVT_EX (0x40160040)                                                  */
/*       HSX (0x40160040)                                                     */
/*       BDX (0x40160040)                                                     */
/* Register default value:              0x00800080                            */
#define RTID_CONFIG_POOL01_SIZE_CBO_CFG_REG 0x00004040
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool0_size : 4;
    /* pool0_size - Bits[9:6], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_10 : 12;
    /* rsvd_10 - Bits[21:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool1_size : 4;
    /* pool1_size - Bits[25:22], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOL01_SIZE_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOL23_SIZE_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x40160044)                                                  */
/*       IVT_EX (0x40160044)                                                  */
/*       HSX (0x40160044)                                                     */
/*       BDX (0x40160044)                                                     */
/* Register default value:              0x00800080                            */
#define RTID_CONFIG_POOL23_SIZE_CBO_CFG_REG 0x00004044
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool2_size : 4;
    /* pool2_size - Bits[9:6], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_10 : 12;
    /* rsvd_10 - Bits[21:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool3_size : 4;
    /* pool3_size - Bits[25:22], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOL23_SIZE_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOL45_SIZE_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x40160048)                                                  */
/*       IVT_EX (0x40160048)                                                  */
/*       HSX (0x40160048)                                                     */
/*       BDX (0x40160048)                                                     */
/* Register default value:              0x00800080                            */
#define RTID_CONFIG_POOL45_SIZE_CBO_CFG_REG 0x00004048
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool4_size : 4;
    /* pool4_size - Bits[9:6], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_10 : 12;
    /* rsvd_10 - Bits[21:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool5_size : 4;
    /* pool5_size - Bits[25:22], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOL45_SIZE_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOL67_SIZE_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x4016004C)                                                  */
/*       IVT_EX (0x4016004C)                                                  */
/*       HSX (0x4016004C)                                                     */
/*       BDX (0x4016004C)                                                     */
/* Register default value:              0x00800080                            */
#define RTID_CONFIG_POOL67_SIZE_CBO_CFG_REG 0x0000404C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool6_size : 4;
    /* pool6_size - Bits[9:6], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_10 : 12;
    /* rsvd_10 - Bits[21:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool7_size : 4;
    /* pool7_size - Bits[25:22], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOL67_SIZE_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOL89_SIZE_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x40160050)                                                  */
/*       IVT_EX (0x40160050)                                                  */
/*       HSX (0x40160050)                                                     */
/*       BDX (0x40160050)                                                     */
/* Register default value:              0x00800080                            */
#define RTID_CONFIG_POOL89_SIZE_CBO_CFG_REG 0x00004050
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool8_size : 4;
    /* pool8_size - Bits[9:6], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_10 : 12;
    /* rsvd_10 - Bits[21:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool9_size : 4;
    /* pool9_size - Bits[25:22], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOL89_SIZE_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOLAB_SIZE_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x40160054)                                                  */
/*       IVT_EX (0x40160054)                                                  */
/*       HSX (0x40160054)                                                     */
/*       BDX (0x40160054)                                                     */
/* Register default value:              0x00800080                            */
#define RTID_CONFIG_POOLAB_SIZE_CBO_CFG_REG 0x00004054
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poola_size : 4;
    /* poola_size - Bits[9:6], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_10 : 12;
    /* rsvd_10 - Bits[21:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolb_size : 4;
    /* poolb_size - Bits[25:22], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOLAB_SIZE_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOLCD_SIZE_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x40160058)                                                  */
/*       IVT_EX (0x40160058)                                                  */
/*       HSX (0x40160058)                                                     */
/*       BDX (0x40160058)                                                     */
/* Register default value:              0x00800080                            */
#define RTID_CONFIG_POOLCD_SIZE_CBO_CFG_REG 0x00004058
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolc_size : 4;
    /* poolc_size - Bits[9:6], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_10 : 12;
    /* rsvd_10 - Bits[21:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poold_size : 4;
    /* poold_size - Bits[25:22], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOLCD_SIZE_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOLEF_SIZE_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x4016005C)                                                  */
/*       IVT_EX (0x4016005C)                                                  */
/*       HSX (0x4016005C)                                                     */
/*       BDX (0x4016005C)                                                     */
/* Register default value:              0x00800080                            */
#define RTID_CONFIG_POOLEF_SIZE_CBO_CFG_REG 0x0000405C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poole_size : 4;
    /* poole_size - Bits[9:6], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_10 : 12;
    /* rsvd_10 - Bits[21:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolf_size : 4;
    /* poolf_size - Bits[25:22], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOLEF_SIZE_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOLGH_SIZE_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x40160060)                                                  */
/*       IVT_EX (0x40160060)                                                  */
/*       HSX (0x40160060)                                                     */
/*       BDX (0x40160060)                                                     */
/* Register default value:              0x00800080                            */
#define RTID_CONFIG_POOLGH_SIZE_CBO_CFG_REG 0x00004060
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 6;
    /* rsvd_0 - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolg_size : 4;
    /* poolg_size - Bits[9:6], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_10 : 12;
    /* rsvd_10 - Bits[21:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolh_size : 4;
    /* poolh_size - Bits[25:22], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOLGH_SIZE_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* QPI_ROUTING_TABLE_SHADOW_CBO_CFG_REG supported on:                         */
/*       IVT_EP (0x40160064)                                                  */
/*       IVT_EX (0x40160064)                                                  */
/*       HSX (0x40160064)                                                     */
/*       BDX (0x40160064)                                                     */
/* Register default value:              0x00000000                            */
#define QPI_ROUTING_TABLE_SHADOW_CBO_CFG_REG 0x00004064


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.12.0.CFG.xml.
 * QPI_Routing_Table is effectively divided into a table 16 2-bit fields.  Each 
 * 2-bit field specifies a QPI link (0,1,2) and the table is indexed by the NodeID. 
 */
typedef union {
  struct {
    UINT32 package0 : 2;
    /* package0 - Bits[1:0], RWS_LB, default = 2'b00 
       value 00 = R3QPI0, value 01 = R3QPI1, value 10 = R3QPI2 
     */
    UINT32 rsvd_2 : 2;
    /* rsvd_2 - Bits[3:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 package1 : 2;
    /* package1 - Bits[5:4], RWS_LB, default = 2'b00 
       value 00 = R3QPI0, value 01 = R3QPI1, value 10 = R3QPI2 
     */
    UINT32 rsvd_6 : 2;
    /* rsvd_6 - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 package2 : 2;
    /* package2 - Bits[9:8], RWS_LB, default = 2'b00 
       value 00 = R3QPI0, value 01 = R3QPI1, value 10 = R3QPI2 
     */
    UINT32 rsvd_10 : 2;
    /* rsvd_10 - Bits[11:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 package3 : 2;
    /* package3 - Bits[13:12], RWS_LB, default = 2'b00 
       value 00 = R3QPI0, value 01 = R3QPI1, value 10 = R3QPI2 
     */
    UINT32 rsvd_14 : 2;
    /* rsvd_14 - Bits[15:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 package4 : 2;
    /* package4 - Bits[17:16], RWS_LB, default = 2'b00 
       value 00 = R3QPI0, value 01 = R3QPI1, value 10 = R3QPI2 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 package5 : 2;
    /* package5 - Bits[21:20], RWS_LB, default = 2'b00 
       value 00 = R3QPI0, value 01 = R3QPI1, value 10 = R3QPI2 
     */
    UINT32 rsvd_22 : 2;
    /* rsvd_22 - Bits[23:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 package6 : 2;
    /* package6 - Bits[25:24], RWS_LB, default = 2'b00 
       value 00 = R3QPI0, value 01 = R3QPI1, value 10 = R3QPI2 
     */
    UINT32 rsvd_26 : 2;
    /* rsvd_26 - Bits[27:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 package7 : 2;
    /* package7 - Bits[29:28], RWS_LB, default = 2'b00 
       value 00 = R3QPI0, value 01 = R3QPI1, value 10 = R3QPI2 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPI_ROUTING_TABLE_SHADOW_CBO_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPI_ROUTING_TABLE_CBO_CFG_REG supported on:                                */
/*       IVT_EP (0x40160068)                                                  */
/*       IVT_EX (0x40160068)                                                  */
/*       HSX (0x40160068)                                                     */
/*       BDX (0x40160068)                                                     */
/* Register default value:              0x00000000                            */
#define QPI_ROUTING_TABLE_CBO_CFG_REG 0x00004068


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.12.0.CFG.xml.
 * Not all processor types support all Intel QPI links. The field mapping in this 
 * CSR has changed over the previous generation of processor. 
 */
typedef union {
  struct {
    UINT32 package0 : 2;
    /* package0 - Bits[1:0], RW_LBV, default = 2'b00 
       value 00 = R3QPI0, value 01 = R3QPI1, value 10 = R3QPI2 
     */
    UINT32 rsvd_2 : 2;
    /* rsvd_2 - Bits[3:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 package1 : 2;
    /* package1 - Bits[5:4], RW_LBV, default = 2'b00 
       value 00 = R3QPI0, value 01 = R3QPI1, value 10 = R3QPI2 
     */
    UINT32 rsvd_6 : 2;
    /* rsvd_6 - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 package2 : 2;
    /* package2 - Bits[9:8], RW_LBV, default = 2'b00 
       value 00 = R3QPI0, value 01 = R3QPI1, value 10 = R3QPI2 
     */
    UINT32 rsvd_10 : 2;
    /* rsvd_10 - Bits[11:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 package3 : 2;
    /* package3 - Bits[13:12], RW_LBV, default = 2'b00 
       value 00 = R3QPI0, value 01 = R3QPI1, value 10 = R3QPI2 
     */
    UINT32 rsvd_14 : 2;
    /* rsvd_14 - Bits[15:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 package4 : 2;
    /* package4 - Bits[17:16], RW_LBV, default = 2'b00 
       value 00 = R3QPI0, value 01 = R3QPI1, value 10 = R3QPI2 
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 package5 : 2;
    /* package5 - Bits[21:20], RW_LBV, default = 2'b00 
       value 00 = R3QPI0, value 01 = R3QPI1, value 10 = R3QPI2 
     */
    UINT32 rsvd_22 : 2;
    /* rsvd_22 - Bits[23:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 package6 : 2;
    /* package6 - Bits[25:24], RW_LBV, default = 2'b00 
       value 00 = R3QPI0, value 01 = R3QPI1, value 10 = R3QPI2 
     */
    UINT32 rsvd_26 : 2;
    /* rsvd_26 - Bits[27:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 package7 : 2;
    /* package7 - Bits[29:28], RW_LBV, default = 2'b00 
       value 00 = R3QPI0, value 01 = R3QPI1, value 10 = R3QPI2 
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPI_ROUTING_TABLE_CBO_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* VNA_CREDIT_CONFIG_CBO_CFG_REG supported on:                                */
/*       IVT_EP (0x4016006C)                                                  */
/*       IVT_EX (0x4016006C)                                                  */
/*       HSX (0x4016006C)                                                     */
/*       BDX (0x4016006C)                                                     */
/* Register default value on IVT_EP:    0x0148A249                            */
/* Register default value on IVT_EX:    0x0148A249                            */
/* Register default value on HSX:       0x0144A249                            */
/* Register default value on BDX:       0x0144A249                            */
#define VNA_CREDIT_CONFIG_CBO_CFG_REG 0x0000406C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * Register related to VNA Credit Configuration. Not all processor types support 
 * all Intel QPI links. 
 */
typedef union {
  struct {
    UINT32 ad_vna_r3qpi0 : 3;
    /* ad_vna_r3qpi0 - Bits[2:0], RWS, default = 3'b001 
       AD VNA credit count for R3QPI0
     */
    UINT32 ad_vna_r3qpi1 : 3;
    /* ad_vna_r3qpi1 - Bits[5:3], RWS, default = 3'b001 
       AD VNA credit count for R3QPI1
     */
    UINT32 bl_vna_r3qpi0 : 3;
    /* bl_vna_r3qpi0 - Bits[8:6], RWS, default = 3'b001 
       BL VNA credit count for R3QPI0
     */
    UINT32 bl_vna_r3qpi1 : 3;
    /* bl_vna_r3qpi1 - Bits[11:9], RWS, default = 3'b001 
       BL VNA credit count for R3QPI1
     */
    UINT32 bl_vna_r2pcie : 3;
    /* bl_vna_r2pcie - Bits[14:12], RWS, default = 3'b010 
       BL VNA credit count for R2PCIE (The VNA credit count toward R2PCIE can't exceed 
       3, so the maximum value should be 3 or less) 
     */
    UINT32 ad_vna_r3qpi2 : 3;
    /* ad_vna_r3qpi2 - Bits[17:15], RWS, default = 3'b001 
       AD VNA credit count for R3QPI2
     */
    UINT32 bl_vna_r3qpi2 : 3;
    /* bl_vna_r3qpi2 - Bits[20:18], RWS, default = 3'b001 
       BL VNA credit count for R3QPI2
     */
    UINT32 numhtcred_ha0 : 2;
    /* numhtcred_ha0 - Bits[22:21], RWS, default = 2'b10 
       Number of local home tracker credits for HA0
     */
    UINT32 numhtcred_ha1 : 2;
    /* numhtcred_ha1 - Bits[24:23], RWS, default = 2'b10 
       Number of local home tracker credits for HA1
     */
    UINT32 rsvd : 5;
    /* rsvd - Bits[29:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 vnacreditchange : 1;
    /* vnacreditchange - Bits[30:30], RWS, default = 1'b0 
       Setting of this bit is not supported in the processor.
     */
    UINT32 disisocvnres : 1;
    /* disisocvnres - Bits[31:31], RWS, default = 1'b0 
       Disable ISOC VN credit reservation
     */
  } Bits;
  UINT32 Data;
} VNA_CREDIT_CONFIG_CBO_CFG_STRUCT;
#endif /* ASM_INC */




/* QPI_XNC_MASK_CBO_CFG_REG supported on:                                     */
/*       IVT_EP (0x40160074)                                                  */
/*       IVT_EX (0x40160074)                                                  */
/*       HSX (0x40160074)                                                     */
/*       BDX (0x40160074)                                                     */
/* Register default value:              0x00000000                            */
#define QPI_XNC_MASK_CBO_CFG_REG 0x00004074
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * Contains info about the XNCs and Peer NodeID in the system.
 */
typedef union {
  struct {
    UINT32 xncnodemask : 16;
    /* xncnodemask - Bits[15:0], RWS, default = 16'h0000 
       Denote which nodes in a system are XNCs. xNCNodeMask[node_id]=1 if the node is 
       an XNC, and 0 otherwise. (EX processor only) 
     */
    UINT32 evctclnen : 1;
    /* evctclnen - Bits[16:16], RWS, default = 1'b0 
       Send EvctCln message when victimizing an E-state line from the cache, unless no 
       RTID is available or the line is being monitored (EX processor only) 
     */
    UINT32 rsvd : 11;
    /* rsvd - Bits[27:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 peer_nodeid : 4;
    /* peer_nodeid - Bits[31:28], RWS, default = 4'b0000 
       NodeID of the Peer Node. Use for DP configuration where source snoop 
       optimization is enabled. 
     */
  } Bits;
  UINT32 Data;
} QPI_XNC_MASK_CBO_CFG_STRUCT;
#endif /* ASM_INC */




/* PIPERSPFUNC_CBO_CFG_REG supported on:                                      */
/*       IVT_EP (0x4016007C)                                                  */
/*       IVT_EX (0x4016007C)                                                  */
/*       HSX (0x4016007C)                                                     */
/*       BDX (0x4016007C)                                                     */
/* Register default value:              0x00000000                            */
#define PIPERSPFUNC_CBO_CFG_REG 0x0000407C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.12.0.CFG.xml.
 * Pipe Response Function
 */
typedef union {
  struct {
    UINT32 errinjcven : 1;
    /* errinjcven - Bits[0:0], RWS_L, default = 1'b0 
       When set, enables write side error injection into LLC CV ECC.  The bits in the 
       LLC CV ECC that will be inverted is determined by the ErrInjMask field. 
     */
    UINT32 errinjstateen : 1;
    /* errinjstateen - Bits[1:1], RWS_L, default = 1'b0 
       When set, enables write side error injection into LLC state ECC.  The bits in 
       the LLC state ECC that will be inverted is determined by the ErrInjMask field. 
     */
    UINT32 errinjmask : 10;
    /* errinjmask - Bits[11:2], RWS_L, default = 10'b0000000000 
       Used for write side LLC state or CV error injection.  Contains a 1 in each bit 
       position corresponding to a ECC bit that should be inverted. 
     */
    UINT32 forcereject : 1;
    /* forcereject - Bits[12:12], RWS_L, default = 1'b0 
       When set, the cbo pipeline will reject a transaction coming down the pipeline 
       whenever there is a trigger from the cbo response logic. 
     */
    UINT32 rsvd_13 : 4;
    /* rsvd_13 - Bits[16:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 injwrdataerr : 2;
    /* injwrdataerr - Bits[18:17], RWS_L, default = 2'b00 
       Inject 1/2/3b errors into the write data.00:No error; 01:Inj 1b err; 10:Inj 2b 
       err; 11:Inj 3b error. 
     */
    UINT32 rsvd_19 : 13;
    /* rsvd_19 - Bits[31:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PIPERSPFUNC_CBO_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */


















/* SADDBGMM0_CBO_CFG_REG supported on:                                        */
/*       IVT_EP (0x401600A0)                                                  */
/*       IVT_EX (0x401600A0)                                                  */
/*       HSX (0x401600A0)                                                     */
/*       BDX (0x401600A0)                                                     */
/* Register default value:              0x00000000                            */
#define SADDBGMM0_CBO_CFG_REG 0x000040A0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 mask_37_6 : 32;
    /* mask_37_6 - Bits[31:0], RWS_LB, default = 32'b00000000000000000000000000000000 
       Mask field for addres[37:6]
     */
  } Bits;
  UINT32 Data;
} SADDBGMM0_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* SADDBGMM1_CBO_CFG_REG supported on:                                        */
/*       IVT_EP (0x401600A4)                                                  */
/*       IVT_EX (0x401600A4)                                                  */
/*       HSX (0x401600A4)                                                     */
/*       BDX (0x401600A4)                                                     */
/* Register default value:              0x00000000                            */
#define SADDBGMM1_CBO_CFG_REG 0x000040A4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 mask_45_38 : 8;
    /* mask_45_38 - Bits[7:0], RWS_LB, default = 8'b00000000 
       Mask field for address[45:38].  Mask bits that are 1 correspond to address bits 
       that are not matched. 
     */
    UINT32 match_29_6 : 24;
    /* match_29_6 - Bits[31:8], RWS_LB, default = 24'b000000000000000000000000 
       Match field for address[29:6]. If bit i in the mask is 1, the corresponding bit 
       in the match field should be 0.  
     */
  } Bits;
  UINT32 Data;
} SADDBGMM1_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* SADDBGMM2_CBO_CFG_REG supported on:                                        */
/*       IVT_EP (0x401600A8)                                                  */
/*       IVT_EX (0x401600A8)                                                  */
/*       HSX (0x401600A8)                                                     */
/*       BDX (0x401600A8)                                                     */
/* Register default value:              0x00000000                            */
#define SADDBGMM2_CBO_CFG_REG 0x000040A8


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.12.0.CFG.xml.
 * Controls debug functions in the sad. Software must always perform a read modify 
 * write of the full 32 bits of this register. Partial writes to this register are 
 * not supported.  
 */
typedef union {
  struct {
    UINT32 match_45_30 : 16;
    /* match_45_30 - Bits[15:0], RWS_LB, default = 16'b0000000000000000 
       Match field for address[45:30]. If bit i in the mask is 1, the corresponding bit 
       in the match field should be 0. 
     */
    UINT32 rsvd_16 : 5;
    UINT32 errinjaddrvalid : 1;
    /* errinjaddrvalid - Bits[21:21], RWS_LB, default = 1'b0 
       Set to 1 for PA address match on read-side LLC tag/state/cv/data error 
       injection. 
     */
    UINT32 errinjopcodevalid : 1;
    /* errinjopcodevalid - Bits[22:22], RWS_LB, default = 1'b0 
       Set to 1 for CLFLUSH opcode match along with PA Addr match on read side 3 bit 
       data error Injection. 
     */
    UINT32 rsvd_23 : 9;
  } Bits;
  UINT32 Data;
} SADDBGMM2_CBO_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RTID_POOL_CONFIG_CBO_CFG_REG supported on:                                 */
/*       IVT_EP (0x401600AC)                                                  */
/*       IVT_EX (0x401600AC)                                                  */
/*       HSX (0x401600AC)                                                     */
/*       BDX (0x401600AC)                                                     */
/* Register default value on IVT_EP:    0x00800000                            */
/* Register default value on IVT_EX:    0x00800000                            */
/* Register default value on HSX:       0x008028A0                            */
/* Register default value on BDX:       0x008028A0                            */
#define RTID_POOL_CONFIG_CBO_CFG_REG 0x000040AC


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo. 
 * RTID_Pool_Config_CFG is not writable via transactions that use Ubox as the 
 * driver of Message channel (ring->Ubox).  
 *             Updating  RTID_Pool_Config_CFG  via Ubox is accomplished by writing 
 * to RTID_Pool_Config_Shadow_CFG. Other Message channel active endpoints (such as 
 * PCU/VCU/PBIST and TAP)   
 *             are able to directly write to RTID_Pool_Config_CFG.  Reading 
 * RTID_Pool_Config_CFG from all active endpoints (including Ubox) is supported.    
 *       
 *       
 */
typedef union {
  struct {
    UINT32 rtidpoolsel : 3;
    /* rtidpoolsel - Bits[2:0], RW_V, default = 3'b000 
       Controls the mapping of Target NID to RTID Pool(s)
       000 - Use NodeID[3:0] SinglePool001 - Use NodeID [3,1:0], DoublePool10
       010 - Use NodeID[3:1], DoublePool21
       011 - Use NodeID[3:2], NodeID0 DoublePool20
       10X - 8S single pool mode, use node ID[3:0] and use RTIDPool16 and 17 for local 
       HA   
       11X - processor double pool mode, use node ID[2:0]
     */
    UINT32 rsvd_3 : 1;
    UINT32 frcismqrtid : 1;
    /* frcismqrtid - Bits[4:4], RW_V, default = 1'b0 
       Force all WBs to use only the shared RTID for Eviction.  This cannot be enabled 
       when EvctCln is also enabled. 
     */
    UINT32 isocha0rtidbase : 6;
    /* isocha0rtidbase - Bits[10:5], RW_V, default = 6'b000101 
       RTID Base for ISOC HA0 RTID pool 
     */
    UINT32 isocha1rtidbase : 6;
    /* isocha1rtidbase - Bits[16:11], RW_V, default = 6'b000101 
       RTID Base for ISOC HA1 RTID pool
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 isocrtidpoolsel : 2;
    /* isocrtidpoolsel - Bits[21:20], RW_V, default = 2'b00 
       When this field is 0, base node ID[0] for ISOC RTID pool selction (i.e. 2 
       socket). If this field is 1 then base node ID[2] (i.e. 1 socket).  If this field 
       is 2, pool0 is always used (single pool mode).  A value of 3 is reserved 
     */
    UINT32 rsvd_22 : 1;
    /* rsvd_22 - Bits[22:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 swaprtid : 1;
    /* swaprtid - Bits[23:23], RW_V, default = 1'b1 
       Performance change: Swap lower RTID with higher RTID during RTID selection in 
       RemoteHome pools. This helps reduce overlap between local and remote home RTIDs 
       at RBT 
     */
    UINT32 extendedrtiden : 1;
    /* extendedrtiden - Bits[24:24], RW_V, default = 1'b0 
       Enable Extended RTID Mode
     */
    UINT32 enivtrtidpoolsel : 1;
    /* enivtrtidpoolsel - Bits[25:25], RW_V, default = 1'b0 
       In double pool mode, we want to pick the RTID from pool0 first. When the pool0 
       has no RTID then pick from the pool1. Legacy behavior is ping-pong between two 
       RTID pools 
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_POOL_CONFIG_CBO_CFG_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo. 
 * RTID_Pool_Config_CFG is not writable via transactions that use Ubox as the 
 * driver of Message channel (ring->Ubox).  
 *             Updating  RTID_Pool_Config_CFG  via Ubox is accomplished by writing 
 * to RTID_Pool_Config_Shadow_CFG. Other Message channel active endpoints (such as 
 * PCU/VCU/PBIST and TAP)   
 *             are able to directly write to RTID_Pool_Config_CFG.  Reading 
 * RTID_Pool_Config_CFG from all active endpoints (including Ubox) is supported.    
 *       
 *       
 */
typedef union {
  struct {
    UINT32 rtidpoolsel : 3;
    /* rtidpoolsel - Bits[2:0], RW_V, default = 3'b000 
       Controls the mapping of Target NID to RTID Pool(s)
     */
    UINT32 rsvd_3 : 1;
    UINT32 frcismqrtid : 1;
    /* frcismqrtid - Bits[4:4], RW_V, default = 1'b0 
       Force all WBs to use only the shared RTID for Eviction.  This cannot be enabled 
       when EvctCln is also enabled. 
     */
    UINT32 isocha0rtidbase : 6;
    /* isocha0rtidbase - Bits[10:5], RW_V, default = 6'b000101 
       RTID Base for ISOC HA0 RTID pool 
     */
    UINT32 isocha1rtidbase : 6;
    /* isocha1rtidbase - Bits[16:11], RW_V, default = 6'b000101 
       RTID Base for ISOC HA1 RTID pool
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 isocrtidpoolsel : 2;
    /* isocrtidpoolsel - Bits[21:20], RW_V, default = 2'b00 
       When this field is 0, base node ID[0] for ISOC RTID pool selction (i.e. 2 
       socket). If this field is 1 then base node ID[2] (i.e. 1 socket).  If this field 
       is 2, pool0 is always used (single pool mode).  A value of 3 is reserved 
     */
    UINT32 rsvd_22 : 1;
    /* rsvd_22 - Bits[22:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 swaprtid : 1;
    /* swaprtid - Bits[23:23], RW_V, default = 1'b1 
       Performance change: Swap lower RTID with higher RTID during RTID selection in 
       RemoteHome pools. This helps reduce overlap between local and remote home RTIDs 
       at RBT 
     */
    UINT32 extendedrtiden : 1;
    /* extendedrtiden - Bits[24:24], RW_V, default = 1'b0 
       Enable Extended RTID Mode
     */
    UINT32 enivtrtidpoolsel : 1;
    /* enivtrtidpoolsel - Bits[25:25], RW_V, default = 1'b0 
       In double pool mode, we want to pick the RTID from pool0 first. When the pool0 
       has no RTID then pick from the pool1. Legacy behavior is ping-pong between two 
       RTID pools 
     */
    UINT32 enbdxbt2rtidpool : 1;
    /* enbdxbt2rtidpool - Bits[26:26], RW_V, default = 1'b0 
       In conjuction with IVT8SSinglePool mode, allows use of 2 more pools for local 
       home in BTMode2. Thus in this mode there are 3 rtid pools per local home, 1 RTID 
       pool per remote home. Used only for 8S, 4S COD 
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_POOL_CONFIG_CBO_CFG_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RTID_POOL_CONFIG_SHADOW_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x401600B0)                                                  */
/*       IVT_EX (0x401600B0)                                                  */
/*       HSX (0x401600B0)                                                     */
/*       BDX (0x401600B0)                                                     */
/* Register default value on IVT_EP:    0x00800000                            */
/* Register default value on IVT_EX:    0x00800000                            */
/* Register default value on HSX:       0x008028A0                            */
/* Register default value on BDX:       0x008028A0                            */
#define RTID_POOL_CONFIG_SHADOW_CBO_CFG_REG 0x000040B0


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.
 */
typedef union {
  struct {
    UINT32 rtidpoolsel : 3;
    /* rtidpoolsel - Bits[2:0], RWS, default = 3'b000 
       Controls the mapping of Target NID to RTID Pool(s)
     */
    UINT32 rsvd_3 : 1;
    UINT32 frcismqrtid : 1;
    /* frcismqrtid - Bits[4:4], RWS, default = 1'b0 
       Force all WBs to use only the shared RTID for Eviction
     */
    UINT32 isocha0rtidbase : 6;
    /* isocha0rtidbase - Bits[10:5], RWS, default = 6'b000101 
       RTID Base for ISOC HA0 RTID pool
     */
    UINT32 isocha1rtidbase : 6;
    /* isocha1rtidbase - Bits[16:11], RWS, default = 6'b000101 
       RTID Base for ISOC HA1 RTID pool
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 isocrtidpoolsel : 2;
    /* isocrtidpoolsel - Bits[21:20], RWS_V, default = 2'b00 
       When this field is 0, base node ID[0] for ISOC RTID pool selction (i.e. 2 
       socket). If this field is 1 then base node ID[2] (i.e. 1 socket).  If this field 
       is 2, pool0 is always used (single pool mode).  A value of 3 is reserved 
     */
    UINT32 rsvd_22 : 1;
    /* rsvd_22 - Bits[22:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 swaprtid : 1;
    /* swaprtid - Bits[23:23], RWS, default = 1'b1 
       Performance change: Swap lower RTID with higher RTID during RTID selection in 
       RemoteHome pools. This helps reduce overlap between local and remote home RTIDs 
       at RBT 
     */
    UINT32 extendedrtiden : 1;
    /* extendedrtiden - Bits[24:24], RWS, default = 1'b0 
       Enable Extended RTID Mode
     */
    UINT32 enivtrtidpoolsel : 1;
    /* enivtrtidpoolsel - Bits[25:25], RWS, default = 1'b0 
       In double pool mode, we want to pick the RTID from pool0 first. When the pool0 
       has no RTID then pick from the pool1. Legacy behavior is ping-pong between two 
       RTID pools 
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_POOL_CONFIG_SHADOW_CBO_CFG_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.
 */
typedef union {
  struct {
    UINT32 rtidpoolsel : 3;
    /* rtidpoolsel - Bits[2:0], RWS, default = 3'b000 
       Controls the mapping of Target NID to RTID Pool(s)
     */
    UINT32 rsvd_3 : 1;
    UINT32 frcismqrtid : 1;
    /* frcismqrtid - Bits[4:4], RWS, default = 1'b0 
       Force all WBs to use only the shared RTID for Eviction
     */
    UINT32 isocha0rtidbase : 6;
    /* isocha0rtidbase - Bits[10:5], RWS, default = 6'b000101 
       RTID Base for ISOC HA0 RTID pool
     */
    UINT32 isocha1rtidbase : 6;
    /* isocha1rtidbase - Bits[16:11], RWS, default = 6'b000101 
       RTID Base for ISOC HA1 RTID pool
     */
    UINT32 rsvd_17 : 3;
    /* rsvd_17 - Bits[19:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 isocrtidpoolsel : 2;
    /* isocrtidpoolsel - Bits[21:20], RWS_V, default = 2'b00 
       When this field is 0, base node ID[0] for ISOC RTID pool selction (i.e. 2 
       socket). If this field is 1 then base node ID[2] (i.e. 1 socket).  If this field 
       is 2, pool0 is always used (single pool mode).  A value of 3 is reserved 
     */
    UINT32 rsvd_22 : 1;
    /* rsvd_22 - Bits[22:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 swaprtid : 1;
    /* swaprtid - Bits[23:23], RWS, default = 1'b1 
       Performance change: Swap lower RTID with higher RTID during RTID selection in 
       RemoteHome pools. This helps reduce overlap between local and remote home RTIDs 
       at RBT 
     */
    UINT32 extendedrtiden : 1;
    /* extendedrtiden - Bits[24:24], RWS, default = 1'b0 
       Enable Extended RTID Mode
     */
    UINT32 enivtrtidpoolsel : 1;
    /* enivtrtidpoolsel - Bits[25:25], RWS, default = 1'b0 
       In double pool mode, we want to pick the RTID from pool0 first. When the pool0 
       has no RTID then pick from the pool1. Legacy behavior is ping-pong between two 
       RTID pools 
     */
    UINT32 enbdxbt2rtidpool : 1;
    /* enbdxbt2rtidpool - Bits[26:26], RW_V, default = 1'b0 
       In conjuction with IVT8SSinglePool mode, allows use of 2 more pools for local 
       home in BTMode2. Thus in this mode there are 3 rtid pools per local home, 1 RTID 
       pool per remote home. Used only for 8S, 4S COD 
     */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_POOL_CONFIG_SHADOW_CBO_CFG_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RTID_CONFIG_POOL01_BASE_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x401600B4)                                                  */
/*       IVT_EX (0x401600B4)                                                  */
/*       HSX (0x401600B4)                                                     */
/*       BDX (0x401600B4)                                                     */
/* Register default value:              0x00000000                            */
#define RTID_CONFIG_POOL01_BASE_CBO_CFG_REG 0x000040B4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.  The contents 
 * of this register are written with the contents of the PoolXX_Base_Shadow_CFG 
 * register on Logic reset or Reconfiguration. 
 */
typedef union {
  struct {
    UINT32 pool0_base_rtid : 6;
    /* pool0_base_rtid - Bits[5:0], RW_V, default = 6'b000000 
       Starting RTID number for Pool0
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool0_extendedmode : 1;
    /* pool0_extendedmode - Bits[9:9], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool1_base_rtid : 6;
    /* pool1_base_rtid - Bits[21:16], RW_V, default = 6'b000000 
       Starting RTID number for Pool1
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool1_extendedmode : 1;
    /* pool1_extendedmode - Bits[25:25], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOL01_BASE_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOL23_BASE_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x401600B8)                                                  */
/*       IVT_EX (0x401600B8)                                                  */
/*       HSX (0x401600B8)                                                     */
/*       BDX (0x401600B8)                                                     */
/* Register default value:              0x00000000                            */
#define RTID_CONFIG_POOL23_BASE_CBO_CFG_REG 0x000040B8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.  The contents 
 * of this register are written with the contents of the PoolXX_Base_Shadow_CFG 
 * register on Logic reset or Reconfiguration. 
 */
typedef union {
  struct {
    UINT32 pool2_base_rtid : 6;
    /* pool2_base_rtid - Bits[5:0], RW_V, default = 6'b000000 
       Starting RTID number for Pool2
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool2_extendedmode : 1;
    /* pool2_extendedmode - Bits[9:9], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool3_base_rtid : 6;
    /* pool3_base_rtid - Bits[21:16], RW_V, default = 6'b000000 
       Starting RTID number for Pool3
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool3_extendedmode : 1;
    /* pool3_extendedmode - Bits[25:25], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOL23_BASE_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOL45_BASE_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x401600BC)                                                  */
/*       IVT_EX (0x401600BC)                                                  */
/*       HSX (0x401600BC)                                                     */
/*       BDX (0x401600BC)                                                     */
/* Register default value:              0x00000000                            */
#define RTID_CONFIG_POOL45_BASE_CBO_CFG_REG 0x000040BC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.  The contents 
 * of this register are written with the contents of the PoolXX_Base_Shadow_CFG 
 * register on Logic reset or Reconfiguration. 
 */
typedef union {
  struct {
    UINT32 pool4_base_rtid : 6;
    /* pool4_base_rtid - Bits[5:0], RW_V, default = 6'b000000 
       Starting RTID number for Pool4
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool4_extendedmode : 1;
    /* pool4_extendedmode - Bits[9:9], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool5_base_rtid : 6;
    /* pool5_base_rtid - Bits[21:16], RW_V, default = 6'b000000 
       Starting RTID number for Pool5
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool5_extendedmode : 1;
    /* pool5_extendedmode - Bits[25:25], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOL45_BASE_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOL67_BASE_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x401600C0)                                                  */
/*       IVT_EX (0x401600C0)                                                  */
/*       HSX (0x401600C0)                                                     */
/*       BDX (0x401600C0)                                                     */
/* Register default value:              0x00000000                            */
#define RTID_CONFIG_POOL67_BASE_CBO_CFG_REG 0x000040C0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.
 */
typedef union {
  struct {
    UINT32 pool6_base_rtid : 6;
    /* pool6_base_rtid - Bits[5:0], RW_V, default = 6'b000000 
       Starting RTID number for Pool6
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool6_extendedmode : 1;
    /* pool6_extendedmode - Bits[9:9], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool7_base_rtid : 6;
    /* pool7_base_rtid - Bits[21:16], RW_V, default = 6'b000000 
       Starting RTID number for Pool7
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool7_extendedmode : 1;
    /* pool7_extendedmode - Bits[25:25], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOL67_BASE_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOL89_BASE_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x401600C4)                                                  */
/*       IVT_EX (0x401600C4)                                                  */
/*       HSX (0x401600C4)                                                     */
/*       BDX (0x401600C4)                                                     */
/* Register default value:              0x00000000                            */
#define RTID_CONFIG_POOL89_BASE_CBO_CFG_REG 0x000040C4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.  The contents 
 * of this register are written with the contents of the PoolXX_Base_Shadow_CFG 
 * register on Logic reset or Reconfiguration. 
 */
typedef union {
  struct {
    UINT32 pool8_base_rtid : 6;
    /* pool8_base_rtid - Bits[5:0], RW_V, default = 6'b000000 
       Starting RTID number for Pool8
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool8_extendedmode : 1;
    /* pool8_extendedmode - Bits[9:9], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool9_base_rtid : 6;
    /* pool9_base_rtid - Bits[21:16], RW_V, default = 6'b000000 
       Starting RTID number for Pool9
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool9_extendedmode : 1;
    /* pool9_extendedmode - Bits[25:25], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOL89_BASE_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOLAB_BASE_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x401600C8)                                                  */
/*       IVT_EX (0x401600C8)                                                  */
/*       HSX (0x401600C8)                                                     */
/*       BDX (0x401600C8)                                                     */
/* Register default value:              0x00000000                            */
#define RTID_CONFIG_POOLAB_BASE_CBO_CFG_REG 0x000040C8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.  The contents 
 * of this register are written with the contents of the PoolXX_Base_Shadow_CFG 
 * register on Logic reset or Reconfiguration. 
 */
typedef union {
  struct {
    UINT32 poola_base_rtid : 6;
    /* poola_base_rtid - Bits[5:0], RW_V, default = 6'b000000 
       Starting RTID number for PoolA
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poola_extendedmode : 1;
    /* poola_extendedmode - Bits[9:9], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolb_base_rtid : 6;
    /* poolb_base_rtid - Bits[21:16], RW_V, default = 6'b000000 
       Starting RTID number for PoolB
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolb_extendedmode : 1;
    /* poolb_extendedmode - Bits[25:25], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOLAB_BASE_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOLCD_BASE_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x401600CC)                                                  */
/*       IVT_EX (0x401600CC)                                                  */
/*       HSX (0x401600CC)                                                     */
/*       BDX (0x401600CC)                                                     */
/* Register default value:              0x00000000                            */
#define RTID_CONFIG_POOLCD_BASE_CBO_CFG_REG 0x000040CC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.  The contents 
 * of this register are written with the contents of the PoolXX_Base_Shadow_CFG 
 * register on Logic reset or Reconfiguration. 
 */
typedef union {
  struct {
    UINT32 poolc_base_rtid : 6;
    /* poolc_base_rtid - Bits[5:0], RW_V, default = 6'b000000 
       Starting RTID number for PoolC
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolc_extendedmode : 1;
    /* poolc_extendedmode - Bits[9:9], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poold_base_rtid : 6;
    /* poold_base_rtid - Bits[21:16], RW_V, default = 6'b000000 
       Starting RTID number for PoolD
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poold_extendedmode : 1;
    /* poold_extendedmode - Bits[25:25], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOLCD_BASE_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOLEF_BASE_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x401600D0)                                                  */
/*       IVT_EX (0x401600D0)                                                  */
/*       HSX (0x401600D0)                                                     */
/*       BDX (0x401600D0)                                                     */
/* Register default value:              0x00000000                            */
#define RTID_CONFIG_POOLEF_BASE_CBO_CFG_REG 0x000040D0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.  The contents 
 * of this register are written with the contents of the PoolXX_Base_Shadow_CFG 
 * register on Logic reset or Reconfiguration. 
 */
typedef union {
  struct {
    UINT32 poole_base_rtid : 6;
    /* poole_base_rtid - Bits[5:0], RW_V, default = 6'b000000 
       Starting RTID number for PoolE
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poole_extendedmode : 1;
    /* poole_extendedmode - Bits[9:9], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolf_base_rtid : 6;
    /* poolf_base_rtid - Bits[21:16], RW_V, default = 6'b000000 
       Starting RTID number for PoolF
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolf_extendedmode : 1;
    /* poolf_extendedmode - Bits[25:25], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOLEF_BASE_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOLGH_BASE_CBO_CFG_REG supported on:                          */
/*       IVT_EP (0x401600D4)                                                  */
/*       IVT_EX (0x401600D4)                                                  */
/*       HSX (0x401600D4)                                                     */
/*       BDX (0x401600D4)                                                     */
/* Register default value:              0x00000000                            */
#define RTID_CONFIG_POOLGH_BASE_CBO_CFG_REG 0x000040D4
#if defined(IVT_HOST) || defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file IVX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.
 */
typedef union {
  struct {
    UINT32 poolg_base_rtid : 6;
    /* poolg_base_rtid - Bits[5:0], RW_V, default = 6'b000000 
       Starting RTID number for PoolG
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolg_extendedmode : 1;
    /* poolg_extendedmode - Bits[9:9], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolh_base_rtid : 6;
    /* poolh_base_rtid - Bits[21:16], RW_V, default = 6'b000000 
       Starting RTID number for Pool0
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolh_extendedmode : 1;
    /* poolh_extendedmode - Bits[25:25], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOLGH_BASE_CBO_CFG_IVT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(HSX_HOST) */



#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.  The contents 
 * of this register are written with the contents of the PoolXX_Base_Shadow_CFG 
 * register on Logic reset or Reconfiguration. 
 */
typedef union {
  struct {
    UINT32 poolg_base_rtid : 6;
    /* poolg_base_rtid - Bits[5:0], RW_V, default = 6'b000000 
       Starting RTID number for PoolG
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolg_extendedmode : 1;
    /* poolg_extendedmode - Bits[9:9], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 pooli_base_rtid : 6;
    /* pooli_base_rtid - Bits[15:10], RW_V, default = 6'b000000 
       Starting RTID number for PoolI
     */
    UINT32 poolh_base_rtid : 6;
    /* poolh_base_rtid - Bits[21:16], RW_V, default = 6'b000000 
       Starting RTID number for PoolH
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolh_extendedmode : 1;
    /* poolh_extendedmode - Bits[25:25], RW_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 poolj_base_rtid : 6;
    /* poolj_base_rtid - Bits[31:26], RW_V, default = 6'b000000 
       Starting RTID number for PoolJ
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOLGH_BASE_CBO_CFG_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RTID_CONFIG_POOL01_BASE_SHADOW_CBO_CFG_REG supported on:                   */
/*       IVT_EP (0x401600D8)                                                  */
/*       IVT_EX (0x401600D8)                                                  */
/*       HSX (0x401600D8)                                                     */
/*       BDX (0x401600D8)                                                     */
/* Register default value:              0x00000000                            */
#define RTID_CONFIG_POOL01_BASE_SHADOW_CBO_CFG_REG 0x000040D8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.  The base RTID 
 * fields of this register are written with 4 + (2*CBO_ID) at the assertion of 
 * PwrGood. CBO_ID is the logical slice number of a Cbo where the LLC is comprised 
 * of N slices, each CBO_ID is a slice in the range (0..N-1). 
 */
typedef union {
  struct {
    UINT32 pool0_base_rtid : 6;
    /* pool0_base_rtid - Bits[5:0], RWS_V, default = 6'b000000 
       Starting RTID number for Pool0
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool0_extendedmode : 1;
    /* pool0_extendedmode - Bits[9:9], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool1_base_rtid : 6;
    /* pool1_base_rtid - Bits[21:16], RWS_V, default = 6'b000000 
       Starting RTID number for Pool1
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool1_extendedmode : 1;
    /* pool1_extendedmode - Bits[25:25], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOL01_BASE_SHADOW_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOL23_BASE_SHADOW_CBO_CFG_REG supported on:                   */
/*       IVT_EP (0x401600DC)                                                  */
/*       IVT_EX (0x401600DC)                                                  */
/*       HSX (0x401600DC)                                                     */
/*       BDX (0x401600DC)                                                     */
/* Register default value:              0x00000000                            */
#define RTID_CONFIG_POOL23_BASE_SHADOW_CBO_CFG_REG 0x000040DC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.  The base RTID 
 * fields of this register are written with 4 + (2*CBO_ID) at the assertion of 
 * PwrGood. 
 */
typedef union {
  struct {
    UINT32 pool2_base_rtid : 6;
    /* pool2_base_rtid - Bits[5:0], RWS_V, default = 6'b000000 
       Starting RTID number for Pool2
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool2_extendedmode : 1;
    /* pool2_extendedmode - Bits[9:9], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool3_base_rtid : 6;
    /* pool3_base_rtid - Bits[21:16], RWS_V, default = 6'b000000 
       Starting RTID number for Pool3
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool3_extendedmode : 1;
    /* pool3_extendedmode - Bits[25:25], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOL23_BASE_SHADOW_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOL45_BASE_SHADOW_CBO_CFG_REG supported on:                   */
/*       IVT_EP (0x401600E0)                                                  */
/*       IVT_EX (0x401600E0)                                                  */
/*       HSX (0x401600E0)                                                     */
/*       BDX (0x401600E0)                                                     */
/* Register default value:              0x00000000                            */
#define RTID_CONFIG_POOL45_BASE_SHADOW_CBO_CFG_REG 0x000040E0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.  The base RTID 
 * fields of this register are written with 4 + (2*CBO_ID) at the assertion of 
 * PwrGood. 
 */
typedef union {
  struct {
    UINT32 pool4_base_rtid : 6;
    /* pool4_base_rtid - Bits[5:0], RWS_V, default = 6'b000000 
       Starting RTID number for Pool4
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool4_extendedmode : 1;
    /* pool4_extendedmode - Bits[9:9], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool5_base_rtid : 6;
    /* pool5_base_rtid - Bits[21:16], RWS_V, default = 6'b000000 
       Starting RTID number for Pool5
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool5_extendedmode : 1;
    /* pool5_extendedmode - Bits[25:25], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOL45_BASE_SHADOW_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOL67_BASE_SHADOW_CBO_CFG_REG supported on:                   */
/*       IVT_EP (0x401600E4)                                                  */
/*       IVT_EX (0x401600E4)                                                  */
/*       HSX (0x401600E4)                                                     */
/*       BDX (0x401600E4)                                                     */
/* Register default value:              0x00000000                            */
#define RTID_CONFIG_POOL67_BASE_SHADOW_CBO_CFG_REG 0x000040E4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.  The base RTID 
 * fields of this register are written with 4 + (2*CBO_ID) at the assertion of 
 * PwrGood. 
 */
typedef union {
  struct {
    UINT32 pool6_base_rtid : 6;
    /* pool6_base_rtid - Bits[5:0], RWS_V, default = 6'b000000 
       Starting RTID number for Pool6
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool6_extendedmode : 1;
    /* pool6_extendedmode - Bits[9:9], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool7_base_rtid : 6;
    /* pool7_base_rtid - Bits[21:16], RWS_V, default = 6'b000000 
       Starting RTID number for Pool7
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool7_extendedmode : 1;
    /* pool7_extendedmode - Bits[25:25], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOL67_BASE_SHADOW_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOL89_BASE_SHADOW_CBO_CFG_REG supported on:                   */
/*       IVT_EP (0x401600E8)                                                  */
/*       IVT_EX (0x401600E8)                                                  */
/*       HSX (0x401600E8)                                                     */
/*       BDX (0x401600E8)                                                     */
/* Register default value:              0x00000000                            */
#define RTID_CONFIG_POOL89_BASE_SHADOW_CBO_CFG_REG 0x000040E8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.  The base RTID 
 * fields of this register are written with 4 + (2*CBO_ID) at the assertion of 
 * PwrGood. 
 */
typedef union {
  struct {
    UINT32 pool8_base_rtid : 6;
    /* pool8_base_rtid - Bits[5:0], RWS_V, default = 6'b000000 
       Starting RTID number for Pool8
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool8_extendedmode : 1;
    /* pool8_extendedmode - Bits[9:9], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool9_base_rtid : 6;
    /* pool9_base_rtid - Bits[21:16], RWS_V, default = 6'b000000 
       Starting RTID number for Pool9
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pool9_extendedmode : 1;
    /* pool9_extendedmode - Bits[25:25], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOL89_BASE_SHADOW_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOLAB_BASE_SHADOW_CBO_CFG_REG supported on:                   */
/*       IVT_EP (0x401600EC)                                                  */
/*       IVT_EX (0x401600EC)                                                  */
/*       HSX (0x401600EC)                                                     */
/*       BDX (0x401600EC)                                                     */
/* Register default value:              0x00000000                            */
#define RTID_CONFIG_POOLAB_BASE_SHADOW_CBO_CFG_REG 0x000040EC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.  The base RTID 
 * fields of this register are written with 4 + (2*CBO_ID) at the assertion of 
 * PwrGood. 
 */
typedef union {
  struct {
    UINT32 poola_base_rtid : 6;
    /* poola_base_rtid - Bits[5:0], RWS_V, default = 6'b000000 
       Starting RTID number for PoolA
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poola_extendedmode : 1;
    /* poola_extendedmode - Bits[9:9], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolb_base_rtid : 6;
    /* poolb_base_rtid - Bits[21:16], RWS_V, default = 6'b000000 
       Starting RTID number for PoolB
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolb_extendedmode : 1;
    /* poolb_extendedmode - Bits[25:25], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOLAB_BASE_SHADOW_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOLCD_BASE_SHADOW_CBO_CFG_REG supported on:                   */
/*       IVT_EP (0x401600F0)                                                  */
/*       IVT_EX (0x401600F0)                                                  */
/*       HSX (0x401600F0)                                                     */
/*       BDX (0x401600F0)                                                     */
/* Register default value:              0x00000000                            */
#define RTID_CONFIG_POOLCD_BASE_SHADOW_CBO_CFG_REG 0x000040F0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.  The base RTID 
 * fields of this register are written with 4 + (2*CBO_ID) at the assertion of 
 * PwrGood. 
 */
typedef union {
  struct {
    UINT32 poolc_base_rtid : 6;
    /* poolc_base_rtid - Bits[5:0], RWS_V, default = 6'b000000 
       Starting RTID number for PoolC
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolc_extendedmode : 1;
    /* poolc_extendedmode - Bits[9:9], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poold_base_rtid : 6;
    /* poold_base_rtid - Bits[21:16], RWS_V, default = 6'b000000 
       Starting RTID number for PoolD
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poold_extendedmode : 1;
    /* poold_extendedmode - Bits[25:25], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOLCD_BASE_SHADOW_CBO_CFG_STRUCT;
#endif /* ASM_INC */


/* RTID_CONFIG_POOLEF_BASE_SHADOW_CBO_CFG_REG supported on:                   */
/*       IVT_EP (0x401600F4)                                                  */
/*       IVT_EX (0x401600F4)                                                  */
/*       HSX (0x401600F4)                                                     */
/*       BDX (0x401600F4)                                                     */
/* Register default value on IVT_EP:    0x00000000                            */
/* Register default value on IVT_EX:    0x00000000                            */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x20001000                            */
#define RTID_CONFIG_POOLEF_BASE_SHADOW_CBO_CFG_REG 0x000040F4
#if defined(IVT_HOST) || defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file IVX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.
 */
typedef union {
  struct {
    UINT32 poole_base_rtid : 6;
    /* poole_base_rtid - Bits[5:0], RWS_V, default = 6'b000000 
       Starting RTID number for Pool0
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poole_extendedmode : 1;
    /* poole_extendedmode - Bits[9:9], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolf_base_rtid : 6;
    /* poolf_base_rtid - Bits[21:16], RWS_V, default = 6'b000000 
       Starting RTID number for Pool0
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolf_extendedmode : 1;
    /* poolf_extendedmode - Bits[25:25], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOLEF_BASE_SHADOW_CBO_CFG_IVT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(HSX_HOST) */



#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.  The base RTID 
 * fields of this register are written with 4 + (2*CBO_ID) at the assertion of 
 * PwrGood. 
 */
typedef union {
  struct {
    UINT32 poole_base_rtid : 6;
    /* poole_base_rtid - Bits[5:0], RWS_V, default = 6'b000000 
       Starting RTID number for PoolE
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poole_extendedmode : 1;
    /* poole_extendedmode - Bits[9:9], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 1;
    /* rsvd_10 - Bits[10:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pooli_size : 4;
    /* pooli_size - Bits[14:11], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolf_base_rtid : 6;
    /* poolf_base_rtid - Bits[21:16], RWS_V, default = 6'b000000 
       Starting RTID number for PoolF
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolf_extendedmode : 1;
    /* poolf_extendedmode - Bits[25:25], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 2;
    /* rsvd_26 - Bits[27:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolj_size : 4;
    /* poolj_size - Bits[31:28], RWS, default = 4'b0010 
       Total number of enabled RTIDs in the Pool (of 8)
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOLEF_BASE_SHADOW_CBO_CFG_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* RTID_CONFIG_POOLGH_BASE_SHADOW_CBO_CFG_REG supported on:                   */
/*       IVT_EP (0x401600F8)                                                  */
/*       IVT_EX (0x401600F8)                                                  */
/*       HSX (0x401600F8)                                                     */
/*       BDX (0x401600F8)                                                     */
/* Register default value:              0x00000000                            */
#define RTID_CONFIG_POOLGH_BASE_SHADOW_CBO_CFG_REG 0x000040F8
#if defined(IVT_HOST) || defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file IVX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.
 */
typedef union {
  struct {
    UINT32 poolg_base_rtid : 6;
    /* poolg_base_rtid - Bits[5:0], RWS_V, default = 6'b000000 
       Starting RTID number for PoolG
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolg_extendedmode : 1;
    /* poolg_extendedmode - Bits[9:9], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolh_base_rtid : 6;
    /* poolh_base_rtid - Bits[21:16], RWS_V, default = 6'b000000 
       Starting RTID number for Pool0
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolh_extendedmode : 1;
    /* poolh_extendedmode - Bits[25:25], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOLGH_BASE_SHADOW_CBO_CFG_IVT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(HSX_HOST) */



#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This control register contain the RTID pool information for Cbo.  The base RTID 
 * fields of this register are written with 4 + (2*CBO_ID) at the assertion of 
 * PwrGood. 
 */
typedef union {
  struct {
    UINT32 poolg_base_rtid : 6;
    /* poolg_base_rtid - Bits[5:0], RWS_V, default = 6'b000000 
       Starting RTID number for PoolG
     */
    UINT32 rsvd_6 : 3;
    /* rsvd_6 - Bits[8:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolg_extendedmode : 1;
    /* poolg_extendedmode - Bits[9:9], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 pooli_base_rtid : 6;
    /* pooli_base_rtid - Bits[15:10], RW_V, default = 6'b000000 
       Starting RTID number for PoolI
     */
    UINT32 poolh_base_rtid : 6;
    /* poolh_base_rtid - Bits[21:16], RWS_V, default = 6'b000000 
       Starting RTID number for PoolH
     */
    UINT32 rsvd_22 : 3;
    /* rsvd_22 - Bits[24:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poolh_extendedmode : 1;
    /* poolh_extendedmode - Bits[25:25], RWS_V, default = 1'b0 
       To indicate that this pool is in use for RTID extension feature.
     */
    UINT32 poolj_base_rtid : 6;
    /* poolj_base_rtid - Bits[31:26], RW_V, default = 6'b000000 
       Starting RTID number for PoolJ
     */
  } Bits;
  UINT32 Data;
} RTID_CONFIG_POOLGH_BASE_SHADOW_CBO_CFG_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* LLCERRINJ_CBO_CFG_REG supported on:                                        */
/*       IVT_EP (0x401600FC)                                                  */
/*       IVT_EX (0x401600FC)                                                  */
/*       HSX (0x401600FC)                                                     */
/*       BDX (0x401600FC)                                                     */
/* Register default value:              0x00000000                            */
#define LLCERRINJ_CBO_CFG_REG 0x000040FC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.12.0.CFG.xml.
 * This CSR is for read-side DATA Array Error Injection. Software must always 
 * perform a read modify write of the full 32 bits of this register. Partial writes 
 * to this register are not supported. 
 */
typedef union {
  struct {
    UINT32 inj3bdataerr : 1;
    /* inj3bdataerr - Bits[0:0], RWS_LBV, default = 1'b0 
       This is used to flip the triple bit data error indication.  Inj3bDataErr is the 
       arm bit for 3 bit data error injection.  It should be set to enable 3 bit data 
       error injection.  In once-only mode, Inj3bDataErr will be cleared by the cbo 
       after the first 3 bit data error has been injected.  In continuous mode, 
       Inj3bDataErr has to be explicitly cleared using a CR write to stop 3 bit data 
       error injection. 
     */
    UINT32 errinjmode : 1;
    /* errinjmode - Bits[1:1], RWS_LB, default = 1'b0 
       If set to 1, read side LLC 3 bit data error will be injected once-only.  If set 
       to 0, read side LLC 3 bit data error will be injected until the Inj3bDataErr is 
       cleared.  ErrInjMode does not apply to 1/2 bit data error injection for which 
       only continuous mode is supported. 
     */
    UINT32 rsvd : 30;
    /* rsvd - Bits[31:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} LLCERRINJ_CBO_CFG_STRUCT;
#endif /* ASM_INC */


















#endif /* CBO_CFG_h */
