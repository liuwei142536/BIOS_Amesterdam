/* Date Stamp: 8/23/2014 */

#ifndef HA_DFX_h
#define HA_DFX_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* HA_DFX_IVT_DEV 14                                                          */
/* HA_DFX_IVT_FUN 2                                                           */
/* For HSX_HOST:                                                              */
/* HA_DFX_HSX_DEV 18                                                          */
/* HA_DFX_HSX_FUN 2                                                           */
/* For BDX_HOST:                                                              */
/* HA_DFX_BDX_DEV 18                                                          */
/* HA_DFX_BDX_FUN 2                                                           */

/* VID_HA_DFX_REG supported on:                                               */
/*       IVT_EP (0x20172000)                                                  */
/*       IVT_EX (0x20172000)                                                  */
/*       HSX (0x20192000)                                                     */
/*       BDX (0x20192000)                                                     */
/* Register default value:              0x8086                                */
#define VID_HA_DFX_REG 0x03022000


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.2.CFG.xml.
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
} VID_HA_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* DID_HA_DFX_REG supported on:                                               */
/*       IVT_EP (0x20172002)                                                  */
/*       IVT_EX (0x20172002)                                                  */
/*       HSX (0x20192002)                                                     */
/*       BDX (0x20192002)                                                     */
/* Register default value on IVT_EP:    0x0E70                                */
/* Register default value on IVT_EX:    0x0E70                                */
/* Register default value on HSX:       0x2F70                                */
/* Register default value on BDX:       0x6F70                                */
#define DID_HA_DFX_REG 0x03022002


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.2.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h2F70 (HSX), 16'h6F70 (BDX) 
       Device ID values vary from function to function. Bits 15:8 are equal to 0x2F. 
       The following list is a breakdown of the function groups. 
         0x2F00 - 0x2F1F : PCI Express and DMI2 ports
         0x2F20 - 0x2F3F : IO Features (Intel QuickData Technology, APIC, VT, RAS, 
       Intel TXT) 
         0x2F40 - 0x2F5F : Performance Monitors
         0x2F80 - 0x2F9F : Intel QPI
         0x2FA0 - 0x2FBF : Home Agent/Memory Controller
         0x2FC0 - 0x2FDF : Power Management
         0x2FE0 - 0x2FFF : Cbo/Ring
       Default value may vary based on bus, device, and function of this CSR location.
     */
  } Bits;
  UINT16 Data;
} DID_HA_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PCICMD_HA_DFX_REG supported on:                                            */
/*       IVT_EP (0x20172004)                                                  */
/*       IVT_EX (0x20172004)                                                  */
/*       HSX (0x20192004)                                                     */
/*       BDX (0x20192004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_HA_DFX_REG 0x03022004


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.2.CFG.xml.
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
} PCICMD_HA_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PCISTS_HA_DFX_REG supported on:                                            */
/*       IVT_EP (0x20172006)                                                  */
/*       IVT_EX (0x20172006)                                                  */
/*       HSX (0x20192006)                                                     */
/*       BDX (0x20192006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_HA_DFX_REG 0x03022006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.2.CFG.xml.
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
} PCISTS_HA_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_HA_DFX_REG supported on:                                               */
/*       IVT_EP (0x10172008)                                                  */
/*       IVT_EX (0x10172008)                                                  */
/*       HSX (0x10192008)                                                     */
/*       BDX (0x10192008)                                                     */
/* Register default value:              0x00                                  */
#define RID_HA_DFX_REG 0x03021008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.2.CFG.xml.
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
} RID_HA_DFX_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_HA_DFX_REG supported on:                                            */
/*       IVT_EP (0x10172009)                                                  */
/*       IVT_EX (0x10172009)                                                  */
/*       HSX (0x10192009)                                                     */
/*       BDX (0x10192009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_HA_DFX_REG 0x03021009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.2.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_HA_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_HA_DFX_REG supported on:                                            */
/*       IVT_EP (0x2017200A)                                                  */
/*       IVT_EX (0x2017200A)                                                  */
/*       HSX (0x2019200A)                                                     */
/*       BDX (0x2019200A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_HA_DFX_REG 0x0302200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.2.CFG.xml.
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
} CCR_N1_HA_DFX_STRUCT;
#endif /* ASM_INC */


/* CLSR_HA_DFX_REG supported on:                                              */
/*       HSX (0x1019200C)                                                     */
/*       BDX (0x1019200C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_HA_DFX_REG 0x0302100C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.2.CFG.xml.
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
} CLSR_HA_DFX_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PLAT_HA_DFX_REG supported on:                                              */
/*       HSX (0x1019200D)                                                     */
/*       BDX (0x1019200D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_HA_DFX_REG 0x0302100D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.2.CFG.xml.
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
} PLAT_HA_DFX_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* HDR_HA_DFX_REG supported on:                                               */
/*       IVT_EP (0x1017200E)                                                  */
/*       IVT_EX (0x1017200E)                                                  */
/*       HSX (0x1019200E)                                                     */
/*       BDX (0x1019200E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_HA_DFX_REG 0x0302100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.2.CFG.xml.
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
} HDR_HA_DFX_STRUCT;
#endif /* ASM_INC */


/* BIST_HA_DFX_REG supported on:                                              */
/*       IVT_EP (0x1017200F)                                                  */
/*       IVT_EX (0x1017200F)                                                  */
/*       HSX (0x1019200F)                                                     */
/*       BDX (0x1019200F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_HA_DFX_REG 0x0302100F


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.2.CFG.xml.
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
} BIST_HA_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CAPPTR_HA_DFX_REG supported on:                                            */
/*       HSX (0x10192034)                                                     */
/*       BDX (0x10192034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_HA_DFX_REG 0x03021034

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.2.CFG.xml.
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
} CAPPTR_HA_DFX_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTL_HA_DFX_REG supported on:                                              */
/*       HSX (0x1019203C)                                                     */
/*       BDX (0x1019203C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_HA_DFX_REG 0x0302103C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.2.CFG.xml.
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
} INTL_HA_DFX_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTPIN_HA_DFX_REG supported on:                                            */
/*       HSX (0x1019203D)                                                     */
/*       BDX (0x1019203D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_HA_DFX_REG 0x0302103D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.2.CFG.xml.
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
} INTPIN_HA_DFX_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MINGNT_HA_DFX_REG supported on:                                            */
/*       IVT_EP (0x1017203E)                                                  */
/*       IVT_EX (0x1017203E)                                                  */
/*       HSX (0x1019203E)                                                     */
/*       BDX (0x1019203E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_HA_DFX_REG 0x0302103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.2.CFG.xml.
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
} MINGNT_HA_DFX_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_HA_DFX_REG supported on:                                            */
/*       IVT_EP (0x1017203F)                                                  */
/*       IVT_EX (0x1017203F)                                                  */
/*       HSX (0x1019203F)                                                     */
/*       BDX (0x1019203F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_HA_DFX_REG 0x0302103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.2.CFG.xml.
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
} MAXLAT_HA_DFX_STRUCT;
#endif /* ASM_INC */














/* HADUMPCTLR_HA_DFX_REG supported on:                                        */
/*       IVT_EP (0x40172060)                                                  */
/*       IVT_EX (0x40172060)                                                  */
/*       HSX (0x40192060)                                                     */
/*       BDX (0x40192060)                                                     */
/* Register default value:              0x00000000                            */
#define HADUMPCTLR_HA_DFX_REG 0x03024060


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.2.CFG.xml.
 * The home agent dump control register is used to select the content to be dumped 
 * into the dump registers. The default value is zero that means "not dump". 
 * //Reference to HaDFXGDXCLOCKCNTL Lock bit control.
 */
typedef union {
  struct {
    UINT32 htid : 7;
    /* htid - Bits[6:0], RW_LV, default = 7'b0000000 
       Updated by hardware when TriggerOnMca, HtidLoadOn
       Snooper, or HtidIncEn is set.
     */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 triggerrspfunc : 2;
    /* triggerrspfunc - Bits[9:8], RW_LV, default = 2'b00 
       trigger on
       response function assertion.
       00: disable
       01: trigger on response function 0
       10: trigger on response function 1
       11: trigger on response function 2
     */
    UINT32 rsvd_10 : 1;
    /* rsvd_10 - Bits[10:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 triggeronmca : 1;
    /* triggeronmca - Bits[11:11], RW_LV, default = 1'b0 
       Use HTID from MCA log
       1: enable function of triggerOnMca
       0: disable function of triggerOnMca
     */
    UINT32 triggernow : 1;
    /* triggernow - Bits[12:12], RW_LV, default = 1'b0 
       Use with TriggerRearm for repeated triggers
       in a tight loop
       1: enable function of triggerNow
       0: disable function of triggerNow
     */
    UINT32 triggerrearm : 1;
    /* triggerrearm - Bits[13:13], RW_LV, default = 1'b0 
       1: Do not clear trigger enables after trigggering. Allows repeated triggers 
       without 
       reprogramming.
       0: Clear all trigger enables upon triggering. register must be re-written to 
       capture 
       again
     */
    UINT32 htidincren : 1;
    /* htidincren - Bits[14:14], RW_L, default = 1'b0 
       1: enable function of incrementing HTID after lookup
       0: disable function of incrementing HTID after lookup
     */
    UINT32 clearen : 1;
    /* clearen - Bits[15:15], RW_L, default = 1'b0 
       Clear debug registers on the
       trigger instead of dumping state from trackers
       1: enable function of clear debug registers
       0: disable function of clear debug registers
     */
    UINT32 dbgdumpspacing : 4;
    /* dbgdumpspacing - Bits[19:16], RW_L, default = 4'b0000 
       It defines the minimum cycles between two
       dummping triggerings
       0000:
       0001:
       1111: TBD
       Note: Bits 19:0 provide the dump function. The can also be used with debug bus
       to get near real-time view of the state of a particular tracker entry.
     */
    UINT32 mcload0 : 1;
    /* mcload0 - Bits[20:20], RW_L, default = 1'b0 
       1: Update tracker ring Info 0/1 in tracker 0/1 whenever the scheduler 0 does MC
       lookup. If dbgSel1ring = 1, also update TrkrDbgInfo
       0: no update (default)
       Note: intended to enable real-time debug bus instrumentation of the loopup
       pipelines.
     */
    UINT32 mcload1 : 1;
    /* mcload1 - Bits[21:21], RW_L, default = 1'b0 
       1: Update tracker ring Info 0/1 in tracker 2/3 whenever the scheduler 1 does MC
       lookup. If dbgSel1ring = 1, also update TrkrDbgInfo
       0: no update (default)
       Note: intended to enable real-time debug bus instrumentation of the loopup
       pipelines.
     */
    UINT32 ringload0 : 1;
    /* ringload0 - Bits[22:22], RW_L, default = 1'b0 
       1: Update tracker ring Info 0/1 in tracker 0/1 whenever the scheduler 0 does 
       ring 
       lookup. If dbgSel1ring = 1, also update TrkrDbgInfo
       0: no update (default)
       Note: intended to enable real-time debug bus instrumentation of the loopup
       pipelines.
     */
    UINT32 ringload1 : 1;
    /* ringload1 - Bits[23:23], RW_L, default = 1'b0 
       1: Update tracker ring Info 0/1 in tracker 2/3 whenever the scheduler 1 does 
       ring 
       lookup. If dbgSel1ring = 1, also update TrkrDbgInfo
       0: no update (default)
       Note: intended to enable real-time debug bus instrumentation of the loopup
       pipelines.
     */
    UINT32 dbgselring : 1;
    /* dbgselring - Bits[24:24], RW_L, default = 1'b0 
       1: debug lookup done with ring lookup
       0: debug lookup done with mc lookup
       Note: intended to enable real-time debug bus instrumentation of the loopup
       pipelines.
     */
    UINT32 rsvd_25 : 7;
    /* rsvd_25 - Bits[31:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HADUMPCTLR_HA_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* HATRKRS2PRESEL_HA_DFX_REG supported on:                                    */
/*       IVT_EP (0x20172064)                                                  */
/*       IVT_EX (0x20172064)                                                  */
/*       HSX (0x40192064)                                                     */
/*       BDX (0x40192064)                                                     */
/* Register default value on IVT_EP:    0x0000                                */
/* Register default value on IVT_EX:    0x0000                                */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define HATRKRS2PRESEL_HA_DFX_REG 0x0302C000


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.2.CFG.xml.
 * This register controls what will be used to control the DFX selector from the 
 * HaTrks2 fub in  HA. 
 * 
 * Note:
 * 
 * Debug Capture and Dumpping
 * 
 * 27-bit debug dump:  Use in conjunction with HadumpCtl, dumps the contents of 
 * HaDump[2-7] after each debug capture operation completes.  All associated fubs 
 * should be enabled using DbgSel registers, and PreSel registers should be 
 * configured as indicated above.  The RTL pipelines are set up so there are no 
 * collisions on debug ring. 
 * 
 * Bits [26:24] of the debug ring can be use to detect which fub is driving (Bits 
 * 26:24 or RingInfo0 are always loaded to 3'b111 and trkrs will always drive 4 
 * consecutive cycles). 
 * 
 * 8-bit debug dump:   Similar idea as 27-bit dump, but captured data is driven out 
 * to a single lane one byte at a time.  Requires lower frequency of captures, but 
 * frees up rest of debug ring. For each capture, the data should come out in the 
 * following order: 
 * 
 * TrkrRingInfo0 byte 0
 * TrkrRingInfo0 byte 1
 * TrkrRingInfo0 byte 2
 * TrkrRingInfo0 byte 3
 * TrkrRingInfo1 byte 0
 * TrkrRingInfo1 byte 1
 * TrkrRingInfo1 byte 2
 * TrkrRingInfo1 byte 3
 * TrkrDbgInfo1 byte 0
 * TrkrDbgInfo1 byte 1
 * TrkrDbgInfo1 byte 2
 * TrkrDbgInfo1 byte 3
 * TrkrMcInfo1 byte 0
 * TrkrMcInfo1 byte 1
 * TrkrMcInfo1 byte 2
 * TrkrMcInfo1 byte 3
 * DctlInfo byte 0
 * DctlInfo byte 1
 * CtlInfo byte 0
 * CtlInfo byte 1
 * CtlInfo byte 2
 * FifoInfo byte 0
 * FifoInfo byte 1
 */
typedef union {
  struct {
    UINT32 lane0 : 3;
    /* lane0 - Bits[2:0], RWS_L, default = 3'b000 
       select for visa lane 0
     */
    UINT32 lane1 : 3;
    /* lane1 - Bits[5:3], RWS_L, default = 3'b000 
       select for visa lane 1
     */
    UINT32 lane2 : 3;
    /* lane2 - Bits[8:6], RWS_L, default = 3'b000 
       select for visa lane 2
     */
    UINT32 lane3 : 3;
    /* lane3 - Bits[11:9], RWS_L, default = 3'b000 
       select for visa lane 3
     */
    UINT32 muxa : 3;
    /* muxa - Bits[14:12], RWS_L, default = 3'b000 
       Selection for 31-bit MuxA signals are below:
       0:TrkrRingInfo0 [26:0]
       1: TrkrRingInfo1 [26:0]
       2: TrkrDbgInfo [26:0]
       3: TrkrMcInfo [26:0]
       4-5: Reserved
       6: Cycle up (TrkrRingInfo0 byte 0 to TrkrMcInfo byte 3) use for 27-bit trkr dump
       7: Cycle Down (TrkrMcInfo byte 3 to TrkrRingInfo byte 0) use for 27-bit trkr 
       dump 
     */
    UINT32 muxb : 5;
    /* muxb - Bits[19:15], RWS_L, default = 5'b00000 
       Selection for 8-bit MuxB signals are below:
       0: TrkrringInfo0 byte 0
       1: TrkrRingInfo0 byte 1
       2: TrkrRingInfo0 byte 2
       3: TrkrRingInfo0 byte 3
       4: TrkrRingInfo1 byte 0
       5: TrkrRingInfo1 byte 1
       6: TrkrRingInfo1 byte 2
       7: TrkrRingInfo1 byte 3
       8: TrkrDbgInfo byte 0
       9: TrkrDbgInfo byte 1
       10: TrkrDbgInfo byte 2
       11: TrkrDbgInfo byte 3
       12: TrkrMcInfo byte 0
       13: TrkrMcInfo byte 1
       14: TrkrMcInfo byte 2
       15: TrkrMcInfo byte 3
       16-29: Reseved
       30: cycle up (TrkrRingInfo0 byte 0 to TrkrMcInfo byte 3) uses for 8-bit trkr 
       dump 
       31: cycle down (TrkrMcInfo byte 3 to TrkrringInfo byte 0) uses for 8-bit trkr 
       dump 
     */
    UINT32 rsvd : 12;
    /* rsvd - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HATRKRS2PRESEL_HA_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* HASCHED0LOCALRSPCNTL_HA_DFX_REG supported on:                              */
/*       IVT_EP (0x40172068)                                                  */
/*       IVT_EX (0x40172068)                                                  */
/*       HSX (0x40192068)                                                     */
/*       BDX (0x40192068)                                                     */
/* Register default value:              0x00000000                            */
#define HASCHED0LOCALRSPCNTL_HA_DFX_REG 0x03024068


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.2.CFG.xml.
 * Controls which scheduler #0-related local responses are triggered when a global 
 * response is signaled. 
 */
typedef union {
  struct {
    UINT32 satrterrorsm : 2;
    /* satrterrorsm - Bits[1:0], RWS_L, default = 2'b00 
       It disables the local response at hascheds0 for triggering HA error handling 
       state machine. 
       00: Disable local response
       01: Enable local response on global response 0
       10: Enable local response on global response 1
       11: Enable local response on global response 2
     */
    UINT32 frccritcyc : 2;
    /* frccritcyc - Bits[3:2], RWS_L, default = 2'b00 
       It disables the local response at hasheds0 to insert a pipeline slot for 
       critical priority isoch. 
       00: Disable local response
       01: Enable local response on global response 0
       10: Enable local response on global response 1
       11: Enable local response on global response 2
     */
    UINT32 frchighcyc : 2;
    /* frchighcyc - Bits[5:4], RWS_L, default = 2'b00 
       It disables the local response at hasheds0 to insert a pipeline slot for high 
       priority isoch. 
       00: Disable local response
       01: Enable local response on global response 0
       10: Enable local response on global response 1
       11: Enable local response on global response 2
     */
    UINT32 sharcrdtrls : 2;
    /* sharcrdtrls - Bits[7:6], RWS_L, default = 2'b00 
       It disables the local response at hasheds0 to release local credits to be 
       returned to the global credit pool. 
       00: Disable local response
       01: Enable local response on global response 0
       10: Enable local response on global response 1
       11: Enable local response on global response 2
     */
    UINT32 blocksched : 2;
    /* blocksched - Bits[9:8], RWS_L, default = 2'b00 
       It disables the local response at hasheds0 to block new MC or Ring request from 
       being scheduled. 
       00: Disable local response
       01: Enable local response on global response 0
       10: Enable local response on global response 1
       11: Enable local response on global response 2
     */
    UINT32 advnacrdtblk : 2;
    /* advnacrdtblk - Bits[11:10], RWS_L, default = 2'b00 
       This local response at hasheds0 hides the AD VNA credits for all QPI agents from 
       the scheduler. 
       00: Disable local response
       01: Enable local response on global response 0
       10: Enable local response on global response 1
       11: Enable local response on global response 2
       Important HSX Note:
       Always keep this field at the default value of zero. Do not modify this field 
       since this functionality is not supported. 
     */
    UINT32 blvnacrdtblk : 2;
    /* blvnacrdtblk - Bits[13:12], RWS_L, default = 2'b00 
       This local response at hasheds0 hides the BL VNA credits for all QPI agents from 
       the scheduler. 
       00: Disable local response
       01: Enable local response on global response 0
       10: Enable local response on global response 1
       11: Enable local response on global response 2
       Important HSX Note:
       Always keep this field at the default value of zero. Do not modify this field 
       since this functionality is not supported. 
     */
    UINT32 mccrdtblk : 2;
    /* mccrdtblk - Bits[15:14], RWS_L, default = 2'b00 
       This local response at hasheds0 hides all the shared MC credits from the 
       scheduler. 
       00: Disable local response
       01: Enable local response on global response 0
       10: Enable local response on global response 1
       11: Enable local response on global response 2
       Important HSX Note:
       Always keep this field at the default value of zero. Do not modify this field 
       since this functionality is not supported. 
     */
    UINT32 injhitmeparerr : 2;
    /* injhitmeparerr - Bits[17:16], RWS_L, default = 2'b00 
       Inject parity error into hitme cache on the next access
       00: Disable local response
       01: Enable local response on global response 0
       10: Enable local response on global response 1
       11: Enable local response on global response 2
     */
    UINT32 rsvd : 14;
    /* rsvd - Bits[31:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HASCHED0LOCALRSPCNTL_HA_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* HATRKRS3PRESEL_HA_DFX_REG supported on:                                    */
/*       IVT_EP (0x2017206C)                                                  */
/*       IVT_EX (0x2017206C)                                                  */
/*       HSX (0x4019206C)                                                     */
/*       BDX (0x4019206C)                                                     */
/* Register default value on IVT_EP:    0x0000                                */
/* Register default value on IVT_EX:    0x0000                                */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define HATRKRS3PRESEL_HA_DFX_REG 0x0302C001


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.2.CFG.xml.
 * This register controls what will be used to control the DFX selector from the 
 * HaTrks3 fub in HA. 
 */
typedef union {
  struct {
    UINT32 lane0 : 3;
    /* lane0 - Bits[2:0], RWS_L, default = 3'b000 
       select for visa lane 0
     */
    UINT32 lane1 : 3;
    /* lane1 - Bits[5:3], RWS_L, default = 3'b000 
       select for visa lane 1
     */
    UINT32 lane2 : 3;
    /* lane2 - Bits[8:6], RWS_L, default = 3'b000 
       select for visa lane 2
     */
    UINT32 lane3 : 3;
    /* lane3 - Bits[11:9], RWS_L, default = 3'b000 
       select for visa lane 3
     */
    UINT32 muxa : 3;
    /* muxa - Bits[14:12], RWS_L, default = 3'b000 
       Selection for 31-bit MuxA signals are below:
       0:TrkrRingInfo0 [26:0]
       1: TrkrRingInfo1 [26:0]
       2: TrkrDbgInfo [26:0]
       3: TrkrMcInfo [26:0]
       4-5: Reserved
       6: Cycle up (TrkrRingInfo0 byte 0 to TrkrMcInfo byte 3) use for 27-bit trkr dump
       7: Cycle Down (TrkrMcInfo byte 3 to TrkrRingInfo byte 0) use for 27-bit trkr 
       dump 
     */
    UINT32 muxb : 5;
    /* muxb - Bits[19:15], RWS_L, default = 5'b00000 
       Selection for 8-bit MuxB signals are below:
       0: TrkrringInfo0 byte 0
       1: TrkrRingInfo0 byte 1
       2: TrkrRingInfo0 byte 2
       3: TrkrRingInfo0 byte 3
       4: TrkrRingInfo1 byte 0
       5: TrkrRingInfo1 byte 1
       6: TrkrRingInfo1 byte 2
       7: TrkrRingInfo1 byte 3
       8: TrkrDbgInfo byte 0
       9: TrkrDbgInfo byte 1
       10: TrkrDbgInfo byte 2
       11: TrkrDbgInfo byte 3
       12: TrkrMcInfo byte 0
       13: TrkrMcInfo byte 1
       14: TrkrMcInfo byte 2
       15: TrkrMcInfo byte 3
       16-29: Reseved
       30: cycle up (TrkrRingInfo0 byte 0 to TrkrMcInfo byte 3) uses for 8-bit trkr 
       dump 
       31: cycle down (TrkrMcInfo byte 3 to TrkrringInfo byte 0) uses for 8-bit trkr 
       dump 
     */
    UINT32 rsvd : 12;
    /* rsvd - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HATRKRS3PRESEL_HA_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* HASCHED1LOCALRSPCNTL_HA_DFX_REG supported on:                              */
/*       IVT_EP (0x40172070)                                                  */
/*       IVT_EX (0x40172070)                                                  */
/*       HSX (0x40192070)                                                     */
/*       BDX (0x40192070)                                                     */
/* Register default value:              0x00000000                            */
#define HASCHED1LOCALRSPCNTL_HA_DFX_REG 0x03024070


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.2.CFG.xml.
 * Controls which scheduler #1-related local responses are triggered when a global 
 * response is signaled. 
 */
typedef union {
  struct {
    UINT32 satrterrorsm : 2;
    /* satrterrorsm - Bits[1:0], RWS_L, default = 2'b00 
       It disables the local response at hascheds0 for triggering HA error handling 
       state machine. 
       00: Disable local response
       01: Enable local response on global response 0
       10: Enable local response on global response 1
       11: Enable local response on global response 2
     */
    UINT32 frccritcyc : 2;
    /* frccritcyc - Bits[3:2], RWS_L, default = 2'b00 
       It disables the local response at hasheds0 to insert a pipeline slot for 
       critical priority isoch. 
       00: Disable local response
       01: Enable local response on global response 0
       10: Enable local response on global response 1
       11: Enable local response on global response 2
     */
    UINT32 frchighcyc : 2;
    /* frchighcyc - Bits[5:4], RWS_L, default = 2'b00 
       It disables the local response at hasheds1 to insert a pipeline slot for 
       critical priority isoch. 
       00: Disable local response
       01: Enable local response on global response 0
       10: Enable local response on global response 1
       11: Enable local response on global response 2
     */
    UINT32 sharcrdtrls : 2;
    /* sharcrdtrls - Bits[7:6], RWS_L, default = 2'b00 
       It disables the local response at hasheds0 to release local credits to be 
       returned to the global credit pool. 
       00: Disable local response
       01: Enable local response on global response 0
       10: Enable local response on global response 1
       11: Enable local response on global response 2
     */
    UINT32 blocksched : 2;
    /* blocksched - Bits[9:8], RWS_L, default = 2'b00 
       It disables the local response at hasheds0 to block new MC or Ring request from 
       being scheduled. 
       00: Disable local response
       01: Enable local response on global response 0
       10: Enable local response on global response 1
       11: Enable local response on global response 2
     */
    UINT32 advnacrdtblk : 2;
    /* advnacrdtblk - Bits[11:10], RWS_L, default = 2'b00 
       This local response at hasheds0 hides the AD VNA credits for all QPI agents from 
       the scheduler. 
       00: Disable local response
       01: Enable local response on global response 0
       10: Enable local response on global response 1
       11: Enable local response on global response 2
       Important HSX Note:
       Always keep this field at the default value of zero. Do not modify this field 
       since this functionality is not supported. 
     */
    UINT32 blvnacrdtblk : 2;
    /* blvnacrdtblk - Bits[13:12], RWS_L, default = 2'b00 
       This local response at hasheds0 hides the BL VNA credits for all QPI agents from 
       the scheduler. 
       00: Disable local response
       01: Enable local response on global response 0
       10: Enable local response on global response 1
       11: Enable local response on global response 2
       Important HSX Note:
       Always keep this field at the default value of zero. Do not modify this field 
       since this functionality is not supported. 
     */
    UINT32 mccrdtblk : 2;
    /* mccrdtblk - Bits[15:14], RWS_L, default = 2'b00 
       This local response at hasheds0 hides all the shared MC credits from the 
       scheduler. 
       00: Disable local response
       01: Enable local response on global response 0
       10: Enable local response on global response 1
       11: Enable local response on global response 2
       Important HSX Note:
       Always keep this field at the default value of zero. Do not modify this field 
       since this functionality is not supported. 
     */
    UINT32 injhitmeparerr : 2;
    /* injhitmeparerr - Bits[17:16], RWS_L, default = 2'b00 
       Inject parity error into hitme cache on the next access
       00: Disable local response
       01: Enable local response on global response 0
       10: Enable local response on global response 1
       11: Enable local response on global response 2
     */
    UINT32 rsvd : 14;
    /* rsvd - Bits[31:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HASCHED1LOCALRSPCNTL_HA_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* HATRKRS1PRESEL_HA_DFX_REG supported on:                                    */
/*       IVT_EP (0x20172074)                                                  */
/*       IVT_EX (0x20172074)                                                  */
/*       HSX (0x40192074)                                                     */
/*       BDX (0x40192074)                                                     */
/* Register default value on IVT_EP:    0x0000                                */
/* Register default value on IVT_EX:    0x0000                                */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define HATRKRS1PRESEL_HA_DFX_REG 0x0302C002


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.2.CFG.xml.
 * This register controls what will be used to control the DFX selector from the 
 * HaTrk1 fub in HA. 
 */
typedef union {
  struct {
    UINT32 lane0 : 3;
    /* lane0 - Bits[2:0], RWS_L, default = 3'b000 
       select for visa lane 0
     */
    UINT32 lane1 : 3;
    /* lane1 - Bits[5:3], RWS_L, default = 3'b000 
       select for visa lane 1
     */
    UINT32 lane2 : 3;
    /* lane2 - Bits[8:6], RWS_L, default = 3'b000 
       select for visa lane 2
     */
    UINT32 lane3 : 3;
    /* lane3 - Bits[11:9], RWS_L, default = 3'b000 
       select for visa lane 3
     */
    UINT32 muxa : 3;
    /* muxa - Bits[14:12], RWS_L, default = 3'b000 
       Selection for 31-bit MuxA signals are below:
       0:TrkrRingInfo0 [26:0]
       1: TrkrRingInfo1 [26:0]
       2: TrkrDbgInfo [26:0]
       3: TrkrMcInfo [26:0]
       4-5: Reserved
       6: Cycle up (TrkrRingInfo0 byte 0 to TrkrMcInfo byte 3) use for 27-bit trkr dump
       7: Cycle Down (TrkrMcInfo byte 3 to TrkrRingInfo byte 0) use for 27-bit trkr 
       dump 
     */
    UINT32 muxb : 5;
    /* muxb - Bits[19:15], RWS_L, default = 5'b00000 
       Selection for 8-bit MuxB signals are below:
       0: TrkrringInfo0 byte 0
       1: TrkrRingInfo0 byte 1
       2: TrkrRingInfo0 byte 2
       3: TrkrRingInfo0 byte 3
       4: TrkrRingInfo1 byte 0
       5: TrkrRingInfo1 byte 1
       6: TrkrRingInfo1 byte 2
       7: TrkrRingInfo1 byte 3
       8: TrkrDbgInfo byte 0
       9: TrkrDbgInfo byte 1
       10: TrkrDbgInfo byte 2
       11: TrkrDbgInfo byte 3
       12: TrkrMcInfo byte 0
       13: TrkrMcInfo byte 1
       14: TrkrMcInfo byte 2
       15: TrkrMcInfo byte 3
       16-29: Reseved
       30: cycle up (TrkrRingInfo0 byte 0 to TrkrMcInfo byte 3) uses for 8-bit trkr 
       dump 
       31: cycle down (TrkrMcInfo byte 3 to TrkrringInfo byte 0) uses for 8-bit trkr 
       dump 
     */
    UINT32 rsvd : 12;
    /* rsvd - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HATRKRS1PRESEL_HA_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* HAFIFOSPRESEL_HA_DFX_REG supported on:                                     */
/*       IVT_EP (0x20172078)                                                  */
/*       IVT_EX (0x20172078)                                                  */
/*       HSX (0x40192078)                                                     */
/*       BDX (0x40192078)                                                     */
/* Register default value on IVT_EP:    0x0000                                */
/* Register default value on IVT_EX:    0x0000                                */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define HAFIFOSPRESEL_HA_DFX_REG 0x0302C003


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.2.CFG.xml.
 * This register controls what will be used to control the DFX selector from the 
 * haFifos fub in HA 
 */
typedef union {
  struct {
    UINT32 lane0 : 6;
    /* lane0 - Bits[5:0], RWS_L, default = 6'b000000 
       select for lane 0 of hactls debug node stop
     */
    UINT32 lane1 : 6;
    /* lane1 - Bits[11:6], RWS_L, default = 6'b000000 
       select for lane 1 of hactls debug node stop
     */
    UINT32 lane2 : 6;
    /* lane2 - Bits[17:12], RWS_L, default = 6'b000000 
       select for lane 2 of hactls debug node stop
     */
    UINT32 lane3 : 6;
    /* lane3 - Bits[23:18], RWS_L, default = 6'b000000 
       select for lane 3 of hactls debug node stop
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HAFIFOSPRESEL_HA_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */






/* HASCHEDS0PRESEL_HA_DFX_REG supported on:                                   */
/*       IVT_EP (0x20172066)                                                  */
/*       IVT_EX (0x20172066)                                                  */
/*       HSX (0x40192080)                                                     */
/*       BDX (0x40192080)                                                     */
/* Register default value on IVT_EP:    0x0000                                */
/* Register default value on IVT_EX:    0x0000                                */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define HASCHEDS0PRESEL_HA_DFX_REG 0x0302C004


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.2.CFG.xml.
 * This register controls what will be used to control the DFX selector from the 
 * hascheds0 fub in HA 
 * //Reference to HaDFXGDXCLOCKCNTL Lock bit control.
 */
typedef union {
  struct {
    UINT32 lane0 : 6;
    /* lane0 - Bits[5:0], RWS_L, default = 6'b000000 
       select for lane 0 of hactls debug node stop
     */
    UINT32 lane1 : 6;
    /* lane1 - Bits[11:6], RWS_L, default = 6'b000000 
       select for lane 1 of hactls debug node stop
     */
    UINT32 lane2 : 6;
    /* lane2 - Bits[17:12], RWS_L, default = 6'b000000 
       select for lane 2 of hactls debug node stop
     */
    UINT32 lane3 : 6;
    /* lane3 - Bits[23:18], RWS_L, default = 6'b000000 
       select for lane 3 of hactls debug node stop
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HASCHEDS0PRESEL_HA_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* HARNGCNTSPRESEL_HA_DFX_REG supported on:                                   */
/*       IVT_EP (0x2017207E)                                                  */
/*       IVT_EX (0x2017207E)                                                  */
/*       HSX (0x40192084)                                                     */
/*       BDX (0x40192084)                                                     */
/* Register default value on IVT_EP:    0x0000                                */
/* Register default value on IVT_EX:    0x0000                                */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define HARNGCNTSPRESEL_HA_DFX_REG 0x0302C005


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.2.CFG.xml.
 * This register controls what will be used to control the DFX selector from the 
 * haControl fub in HA 
 */
typedef union {
  struct {
    UINT32 lane0 : 7;
    /* lane0 - Bits[6:0], RWS_L, default = 7'b0000000 
       select for lane 0 of harngcntls debug node stop
     */
    UINT32 lane1 : 7;
    /* lane1 - Bits[13:7], RWS_L, default = 7'b0000000 
       select for lane 1 of harngcntls debug node stop
     */
    UINT32 lane2 : 7;
    /* lane2 - Bits[20:14], RWS_L, default = 7'b0000000 
       select for lane 2 of harngcntls debug node stop
     */
    UINT32 lane3 : 7;
    /* lane3 - Bits[27:21], RWS_L, default = 7'b0000000 
       select for lane 3 of harngcntls debug node stop
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HARNGCNTSPRESEL_HA_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* HASCHEDS1PRESEL_HA_DFX_REG supported on:                                   */
/*       IVT_EP (0x2017206E)                                                  */
/*       IVT_EX (0x2017206E)                                                  */
/*       HSX (0x40192088)                                                     */
/*       BDX (0x40192088)                                                     */
/* Register default value on IVT_EP:    0x0000                                */
/* Register default value on IVT_EX:    0x0000                                */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define HASCHEDS1PRESEL_HA_DFX_REG 0x0302C006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.2.CFG.xml.
 * This register controls what will be used to control the DFX selector from the 
 * hascheds1 fub in HA 
 */
typedef union {
  struct {
    UINT32 lane0 : 6;
    /* lane0 - Bits[5:0], RWS_L, default = 6'b000000 
       select for lane 0 of hactls debug node stop
     */
    UINT32 lane1 : 6;
    /* lane1 - Bits[11:6], RWS_L, default = 6'b000000 
       select for lane 1 of hactls debug node stop
     */
    UINT32 lane2 : 6;
    /* lane2 - Bits[17:12], RWS_L, default = 6'b000000 
       select for lane 2 of hactls debug node stop
     */
    UINT32 lane3 : 6;
    /* lane3 - Bits[23:18], RWS_L, default = 6'b000000 
       select for lane 3 of hactls debug node stop
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HASCHEDS1PRESEL_HA_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* HACTLSPRESEL_HA_DFX_REG supported on:                                      */
/*       IVT_EP (0x20172076)                                                  */
/*       IVT_EX (0x20172076)                                                  */
/*       HSX (0x4019208C)                                                     */
/*       BDX (0x4019208C)                                                     */
/* Register default value on IVT_EP:    0x0000                                */
/* Register default value on IVT_EX:    0x0000                                */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define HACTLSPRESEL_HA_DFX_REG 0x0302C007


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.2.CFG.xml.
 * This register controls what will be used to control the DFX selector from the 
 * haCtls fub in HA 
 */
typedef union {
  struct {
    UINT32 lane0 : 6;
    /* lane0 - Bits[5:0], RWS_L, default = 6'b000000 
       select for lane 0 of hactls debug node stop
     */
    UINT32 lane1 : 6;
    /* lane1 - Bits[11:6], RWS_L, default = 6'b000000 
       select for lane 1 of hactls debug node stop
     */
    UINT32 lane2 : 6;
    /* lane2 - Bits[17:12], RWS_L, default = 6'b000000 
       select for lane 2 of hactls debug node stop
     */
    UINT32 lane3 : 6;
    /* lane3 - Bits[23:18], RWS_L, default = 6'b000000 
       select for lane 3 of hactls debug node stop
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HACTLSPRESEL_HA_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* HADCTLPRESEL_HA_DFX_REG supported on:                                      */
/*       IVT_EP (0x2017207A)                                                  */
/*       IVT_EX (0x2017207A)                                                  */
/*       HSX (0x40192090)                                                     */
/*       BDX (0x40192090)                                                     */
/* Register default value on IVT_EP:    0x0000                                */
/* Register default value on IVT_EX:    0x0000                                */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000000                            */
#define HADCTLPRESEL_HA_DFX_REG 0x0302C008


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.2.CFG.xml.
 * This register controls what will be used to control the DFX selector from the 
 * haDcntl fub in HA 
 * //Reference to HaDFXGDXCLOCKCNTL Lock bit control.
 */
typedef union {
  struct {
    UINT32 lane0 : 8;
    /* lane0 - Bits[7:0], RWS_L, default = 8'b00000000 
       select for lane 0 of hactls debug node stop
     */
    UINT32 lane1 : 8;
    /* lane1 - Bits[15:8], RWS_L, default = 8'b00000000 
       select for lane 1 of hactls debug node stop
     */
    UINT32 lane2 : 8;
    /* lane2 - Bits[23:16], RWS_L, default = 8'b00000000 
       select for lane 2 of hactls debug node stop
     */
    UINT32 lane3 : 8;
    /* lane3 - Bits[31:24], RWS_L, default = 8'b00000000 
       select for lane 3 of hactls debug node stop
     */
  } Bits;
  UINT32 Data;
} HADCTLPRESEL_HA_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




















/* HAASC0LDVALLO_HA_DFX_REG supported on:                                     */
/*       HSX (0x401920AC)                                                     */
/*       BDX (0x401920AC)                                                     */
/* Register default value:              0x00000000                            */
#define HAASC0LDVALLO_HA_DFX_REG 0x030240AC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.2.CFG.xml.
 * Anti-starvation counter #0 low load value.
 */
typedef union {
  struct {
    UINT32 ldlowval : 32;
    /* ldlowval - Bits[31:0], RWS_L, default = 32'b00000000000000000000000000000000 
       This value will be loaded into the counter when the output state transitions to 
       0. 
     */
  } Bits;
  UINT32 Data;
} HAASC0LDVALLO_HA_DFX_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */








































/* CLS_HA_DFX_REG supported on:                                               */
/*       IVT_EP (0x1017200C)                                                  */
/*       IVT_EX (0x1017200C)                                                  */
/* Register default value:              0x00                                  */
#define CLS_HA_DFX_REG 0x0302100C



/* MLT_HA_DFX_REG supported on:                                               */
/*       IVT_EP (0x1017200D)                                                  */
/*       IVT_EX (0x1017200D)                                                  */
/* Register default value:              0x00                                  */
#define MLT_HA_DFX_REG 0x0302100D



/* TMBAR_N0_HA_DFX_REG supported on:                                          */
/*       IVT_EP (0x40172010)                                                  */
/*       IVT_EX (0x40172010)                                                  */
/* Register default value:              0x00000000                            */
#define TMBAR_N0_HA_DFX_REG 0x03024010



/* TMBAR_N1_HA_DFX_REG supported on:                                          */
/*       IVT_EP (0x40172014)                                                  */
/*       IVT_EX (0x40172014)                                                  */
/* Register default value:              0x00000000                            */
#define TMBAR_N1_HA_DFX_REG 0x03024014



/* SVID_HA_DFX_REG supported on:                                              */
/*       IVT_EP (0x2017202C)                                                  */
/*       IVT_EX (0x2017202C)                                                  */
/* Register default value:              0x0000                                */
#define SVID_HA_DFX_REG 0x0302202C



/* SID_HA_DFX_REG supported on:                                               */
/*       IVT_EP (0x2017202E)                                                  */
/*       IVT_EX (0x2017202E)                                                  */
/* Register default value:              0x0000                                */
#define SID_HA_DFX_REG 0x0302202E



/* CAPPOINT_HA_DFX_REG supported on:                                          */
/*       IVT_EP (0x10172034)                                                  */
/*       IVT_EX (0x10172034)                                                  */
/* Register default value:              0x00                                  */
#define CAPPOINT_HA_DFX_REG 0x03021034



/* INTRLINE_HA_DFX_REG supported on:                                          */
/*       IVT_EP (0x1017203C)                                                  */
/*       IVT_EX (0x1017203C)                                                  */
/* Register default value:              0x00                                  */
#define INTRLINE_HA_DFX_REG 0x0302103C



/* INTRPIN_HA_DFX_REG supported on:                                           */
/*       IVT_EP (0x1017203D)                                                  */
/*       IVT_EX (0x1017203D)                                                  */
/* Register default value:              0x00                                  */
#define INTRPIN_HA_DFX_REG 0x0302103D



/* HATRK0PRESEL_HA_DFX_REG supported on:                                      */
/*       IVT_EP (0x2017207C)                                                  */
/*       IVT_EX (0x2017207C)                                                  */
/* Register default value:              0x0000                                */
#define HATRK0PRESEL_HA_DFX_REG 0x0302207C



/* HATRKRS0DBGSEL_HA_DFX_REG supported on:                                    */
/*       IVT_EP (0x10172080)                                                  */
/*       IVT_EX (0x10172080)                                                  */
/* Register default value:              0x00                                  */
#define HATRKRS0DBGSEL_HA_DFX_REG 0x03021080



/* HATRKRS1DBGSEL_HA_DFX_REG supported on:                                    */
/*       IVT_EP (0x10172088)                                                  */
/*       IVT_EX (0x10172088)                                                  */
/* Register default value:              0x00                                  */
#define HATRKRS1DBGSEL_HA_DFX_REG 0x03021088



/* HAFIFOSDBGSEL_HA_DFX_REG supported on:                                     */
/*       IVT_EP (0x1017208A)                                                  */
/*       IVT_EX (0x1017208A)                                                  */
/* Register default value:              0x00                                  */
#define HAFIFOSDBGSEL_HA_DFX_REG 0x0302108A



/* HARNGCTLSDBGSEL_HA_DFX_REG supported on:                                   */
/*       IVT_EP (0x1017208C)                                                  */
/*       IVT_EX (0x1017208C)                                                  */
/* Register default value:              0x00                                  */
#define HARNGCTLSDBGSEL_HA_DFX_REG 0x0302108C



/* HATRKRS2DBGSEL_HA_DFX_REG supported on:                                    */
/*       IVT_EP (0x10172090)                                                  */
/*       IVT_EX (0x10172090)                                                  */
/* Register default value:              0x00                                  */
#define HATRKRS2DBGSEL_HA_DFX_REG 0x03021090



/* HATRKRS3DBGSEL_HA_DFX_REG supported on:                                    */
/*       IVT_EP (0x10172098)                                                  */
/*       IVT_EX (0x10172098)                                                  */
/* Register default value:              0x00                                  */
#define HATRKRS3DBGSEL_HA_DFX_REG 0x03021098



/* HADBGBUSMASK_HA_DFX_REG supported on:                                      */
/*       IVT_EP (0x4017209C)                                                  */
/*       IVT_EX (0x4017209C)                                                  */
/* Register default value:              0x00000000                            */
#define HADBGBUSMASK_HA_DFX_REG 0x0302409C



/* HADBGBUSSSREG_HA_DFX_REG supported on:                                     */
/*       IVT_EP (0x401720A8)                                                  */
/*       IVT_EX (0x401720A8)                                                  */
/* Register default value:              0x00000000                            */
#define HADBGBUSSSREG_HA_DFX_REG 0x030240A8



/* HADBGBUSMATCH_HA_DFX_REG supported on:                                     */
/*       IVT_EP (0x401720AC)                                                  */
/*       IVT_EX (0x401720AC)                                                  */
/* Register default value:              0x00000000                            */
#define HADBGBUSMATCH_HA_DFX_REG 0x030240AC



/* HAACSCCNTR0_HA_DFX_REG supported on:                                       */
/*       IVT_EP (0x201720B8)                                                  */
/*       IVT_EX (0x201720B8)                                                  */
/* Register default value:              0x0000                                */
#define HAACSCCNTR0_HA_DFX_REG 0x030220B8



/* HAACSCCNTR1_HA_DFX_REG supported on:                                       */
/*       IVT_EP (0x201720BA)                                                  */
/*       IVT_EX (0x201720BA)                                                  */
/* Register default value:              0x0000                                */
#define HAACSCCNTR1_HA_DFX_REG 0x030220BA



/* HAASC0LDVAL_HA_DFX_REG supported on:                                       */
/*       IVT_EP (0x401720BC)                                                  */
/*       IVT_EX (0x401720BC)                                                  */
/* Register default value:              0x00000000                            */
#define HAASC0LDVAL_HA_DFX_REG 0x030240BC



/* HAASC1LDVAL_HA_DFX_REG supported on:                                       */
/*       IVT_EP (0x401720C0)                                                  */
/*       IVT_EX (0x401720C0)                                                  */
/* Register default value:              0x00000000                            */
#define HAASC1LDVAL_HA_DFX_REG 0x030240C0



#endif /* HA_DFX_h */
