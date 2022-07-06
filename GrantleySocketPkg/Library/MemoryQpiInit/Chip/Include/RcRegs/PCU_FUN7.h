/* Date Stamp: 8/23/2014 */

#ifndef PCU_FUN7_h
#define PCU_FUN7_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* PCU_FUN7_IVT_DEV 10                                                        */
/* PCU_FUN7_IVT_FUN 7                                                         */
/* For HSX_HOST:                                                              */
/* PCU_FUN7_HSX_DEV 30                                                        */
/* PCU_FUN7_HSX_FUN 7                                                         */
/* For BDX_HOST:                                                              */
/* PCU_FUN7_BDX_DEV 30                                                        */
/* PCU_FUN7_BDX_FUN 7                                                         */

/* VID_PCU_FUN7_REG supported on:                                             */
/*       IVT_EP (0x20157000)                                                  */
/*       IVT_EX (0x20157000)                                                  */
/*       BDX (0x201F7000)                                                     */
/* Register default value:              0x8086                                */
#define VID_PCU_FUN7_REG 0x0A072000

#if defined(IVT_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.7.CFG.xml.
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
} VID_PCU_FUN7_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(BDX_HOST) */


/* DID_PCU_FUN7_REG supported on:                                             */
/*       IVT_EP (0x20157002)                                                  */
/*       IVT_EX (0x20157002)                                                  */
/*       BDX (0x201F7002)                                                     */
/* Register default value on IVT_EP:    0x0EC7                                */
/* Register default value on IVT_EX:    0x0EC7                                */
/* Register default value on BDX:       0x6F9F                                */
#define DID_PCU_FUN7_REG 0x0A072002


#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.7.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6F9F 
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
} DID_PCU_FUN7_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* PCICMD_PCU_FUN7_REG supported on:                                          */
/*       IVT_EP (0x20157004)                                                  */
/*       IVT_EX (0x20157004)                                                  */
/*       BDX (0x201F7004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_PCU_FUN7_REG 0x0A072004

#if defined(IVT_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.7.CFG.xml.
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
} PCICMD_PCU_FUN7_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(BDX_HOST) */


/* PCISTS_PCU_FUN7_REG supported on:                                          */
/*       IVT_EP (0x20157006)                                                  */
/*       IVT_EX (0x20157006)                                                  */
/*       BDX (0x201F7006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_PCU_FUN7_REG 0x0A072006

#if defined(IVT_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.7.CFG.xml.
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
} PCISTS_PCU_FUN7_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(BDX_HOST) */


/* RID_PCU_FUN7_REG supported on:                                             */
/*       IVT_EP (0x10157008)                                                  */
/*       IVT_EX (0x10157008)                                                  */
/*       BDX (0x101F7008)                                                     */
/* Register default value:              0x00                                  */
#define RID_PCU_FUN7_REG 0x0A071008

#if defined(IVT_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.7.CFG.xml.
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
} RID_PCU_FUN7_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(BDX_HOST) */


/* CCR_N0_PCU_FUN7_REG supported on:                                          */
/*       IVT_EP (0x10157009)                                                  */
/*       IVT_EX (0x10157009)                                                  */
/*       BDX (0x101F7009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_PCU_FUN7_REG 0x0A071009


#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.7.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_PCU_FUN7_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* CCR_N1_PCU_FUN7_REG supported on:                                          */
/*       IVT_EP (0x2015700A)                                                  */
/*       IVT_EX (0x2015700A)                                                  */
/*       BDX (0x201F700A)                                                     */
/* Register default value on IVT_EP:    0x0880                                */
/* Register default value on IVT_EX:    0x0880                                */
/* Register default value on BDX:       0x0000                                */
#define CCR_N1_PCU_FUN7_REG 0x0A07200A

#if defined(IVT_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.7.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT16 sub_class : 8;
    /* sub_class - Bits[7:0], ROS_V, default = None 
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
    /* base_class - Bits[15:8], ROS_V, default = None 
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
} CCR_N1_PCU_FUN7_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(BDX_HOST) */


/* CLSR_PCU_FUN7_REG supported on:                                            */
/*       IVT_EP (0x1015700C)                                                  */
/*       IVT_EX (0x1015700C)                                                  */
/*       BDX (0x101F700C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_PCU_FUN7_REG 0x0A07100C

#if defined(IVT_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.7.CFG.xml.
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
} CLSR_PCU_FUN7_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(BDX_HOST) */


/* PLAT_PCU_FUN7_REG supported on:                                            */
/*       IVT_EP (0x1015700D)                                                  */
/*       IVT_EX (0x1015700D)                                                  */
/*       BDX (0x101F700D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_PCU_FUN7_REG 0x0A07100D

#if defined(IVT_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.7.CFG.xml.
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
} PLAT_PCU_FUN7_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(BDX_HOST) */


/* HDR_PCU_FUN7_REG supported on:                                             */
/*       IVT_EP (0x1015700E)                                                  */
/*       IVT_EX (0x1015700E)                                                  */
/*       BDX (0x101F700E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_PCU_FUN7_REG 0x0A07100E

#if defined(IVT_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.7.CFG.xml.
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
} HDR_PCU_FUN7_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(BDX_HOST) */


/* BIST_PCU_FUN7_REG supported on:                                            */
/*       IVT_EP (0x1015700F)                                                  */
/*       IVT_EX (0x1015700F)                                                  */
/*       BDX (0x101F700F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_PCU_FUN7_REG 0x0A07100F

#if defined(IVT_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.7.CFG.xml.
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
} BIST_PCU_FUN7_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(BDX_HOST) */


/* CAPPTR_PCU_FUN7_REG supported on:                                          */
/*       IVT_EP (0x10157034)                                                  */
/*       IVT_EX (0x10157034)                                                  */
/*       BDX (0x101F7034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_PCU_FUN7_REG 0x0A071034

#if defined(IVT_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.7.CFG.xml.
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
} CAPPTR_PCU_FUN7_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(BDX_HOST) */


/* INTL_PCU_FUN7_REG supported on:                                            */
/*       IVT_EP (0x1015703C)                                                  */
/*       IVT_EX (0x1015703C)                                                  */
/*       BDX (0x101F703C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_PCU_FUN7_REG 0x0A07103C

#if defined(IVT_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.7.CFG.xml.
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
} INTL_PCU_FUN7_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(BDX_HOST) */


/* INTPIN_PCU_FUN7_REG supported on:                                          */
/*       IVT_EP (0x1015703D)                                                  */
/*       IVT_EX (0x1015703D)                                                  */
/*       BDX (0x101F703D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_PCU_FUN7_REG 0x0A07103D

#if defined(IVT_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.7.CFG.xml.
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
} INTPIN_PCU_FUN7_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(BDX_HOST) */


/* MINGNT_PCU_FUN7_REG supported on:                                          */
/*       IVT_EP (0x1015703E)                                                  */
/*       IVT_EX (0x1015703E)                                                  */
/*       BDX (0x101F703E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_PCU_FUN7_REG 0x0A07103E

#if defined(IVT_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.7.CFG.xml.
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
} MINGNT_PCU_FUN7_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(BDX_HOST) */


/* MAXLAT_PCU_FUN7_REG supported on:                                          */
/*       IVT_EP (0x1015703F)                                                  */
/*       IVT_EX (0x1015703F)                                                  */
/*       BDX (0x101F703F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_PCU_FUN7_REG 0x0A07103F

#if defined(IVT_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.7.CFG.xml.
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
} MAXLAT_PCU_FUN7_STRUCT;
#endif /* ASM_INC */
#endif /* defined(IVT_HOST) || defined(BDX_HOST) */


/* SVID_PCU_FUN7_REG supported on:                                            */
/*       IVT_EP (0x2015702C)                                                  */
/*       IVT_EX (0x2015702C)                                                  */
/* Register default value:              0x8086                                */
#define SVID_PCU_FUN7_REG 0x0A07202C



/* SDID_PCU_FUN7_REG supported on:                                            */
/*       IVT_EP (0x2015702E)                                                  */
/*       IVT_EX (0x2015702E)                                                  */
/* Register default value:              0x0000                                */
#define SDID_PCU_FUN7_REG 0x0A07202E



/* LAN_PORT_SEL_PCU_FUN7_REG supported on:                                    */
/*       BDX (0x101F704C)                                                     */
/* Register default value:              0x00                                  */
#define LAN_PORT_SEL_PCU_FUN7_REG 0x0A07104C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1_30_7_CFG.xml.
 * Default values will come from Pin Straps. This is for LAN0
 */
typedef union {
  struct {
    UINT8 enable_lan_port0 : 1;
    /* enable_lan_port0 - Bits[0:0], RWS_V, default = 1'h0  */
    UINT8 enable_lan_port1 : 1;
    /* enable_lan_port1 - Bits[1:1], RWS_V, default = 1'h0  */
    UINT8 rsvd : 6;
    /* rsvd - Bits[7:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} LAN_PORT_SEL_PCU_FUN7_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* LAN1_PORT_SEL_PCU_FUN7_REG supported on:                                   */
/*       BDX (0x101F7050)                                                     */
/* Register default value:              0x00                                  */
#define LAN1_PORT_SEL_PCU_FUN7_REG 0x0A071050
#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1_30_7_CFG.xml.
 * Default values will come from Pin Straps. This is for bios configurability of 
 * new LAN1 ports in BDX-DE NS 
 */
typedef union {
  struct {
    UINT8 enable_lan_port0 : 1;
    /* enable_lan_port0 - Bits[0:0], RWS_V, default = 1'h0  */
    UINT8 enable_lan_port1 : 1;
    /* enable_lan_port1 - Bits[1:1], RWS_V, default = 1'h0  */
    UINT8 rsvd : 6;
    /* rsvd - Bits[7:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} LAN1_PORT_SEL_PCU_FUN7_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CPUBUSNO03_D2F3_PCU_FUN7_REG supported on:                                            */
/*      BDX (0x401870D5)                                                  */
/* Register default value:              0x0000                                */
#define CPUBUSNO03_D2F3_PCU_FUN7_REG 0x0A072054

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file IVX\1.10.7.CFG.xml.
 */
typedef union {
  struct {
    UINT16 bus_number : 8;
    /* bus_number- Bits[7:0], RWS, default = 8'b00000000 
       For bios to program bus number for device connected to D2F3 in BDX-DE NS. Others are in Ubox
     */
    UINT16 bus_valid : 1;
	/* bus_valid- Bits[8:8], RWS, default = 1'b1 
       1 indicates that bus number for D2F3 is valid.
     */
  } Bits;
  UINT16 Data;
} CPUBUSNO03_D2F3_PCU_FUN7_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */



#endif /* PCU_FUN7_h */
