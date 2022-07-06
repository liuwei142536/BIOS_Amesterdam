/* Date Stamp: 8/23/2014 */

#ifndef MCIO_DDRIOEXTMCC_h
#define MCIO_DDRIOEXTMCC_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* MCIO_DDRIOEXTMCC_IVT_DEV 17                                                */
/* MCIO_DDRIOEXTMCC_IVT_FUN 7                                                 */
/* For HSX_HOST:                                                              */
/* MCIO_DDRIOEXTMCC_HSX_DEV 20                                                */
/* MCIO_DDRIOEXTMCC_HSX_FUN 7                                                 */
/* For BDX_HOST:                                                              */
/* MCIO_DDRIOEXTMCC_BDX_DEV 20                                                */
/* MCIO_DDRIOEXTMCC_BDX_FUN 7                                                 */

/* VID_MCIO_DDRIOEXTMCC_REG supported on:                                     */
/*       IVT_EP (0x2018F000)                                                  */
/*       HSX (0x201A7000)                                                     */
/*       BDX (0x201A7000)                                                     */
/* Register default value:              0x8086                                */
#define VID_MCIO_DDRIOEXTMCC_REG 0x0D052000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
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
} VID_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */


/* DID_MCIO_DDRIOEXTMCC_REG supported on:                                     */
/*       IVT_EP (0x2018F002)                                                  */
/*       HSX (0x201A7002)                                                     */
/*       BDX (0x201A7002)                                                     */
/* Register default value on IVT_EP:    0x0EBF                                */
/* Register default value on HSX:       0x2FBF                                */
/* Register default value on BDX:       0x6FBF                                */
#define DID_MCIO_DDRIOEXTMCC_REG 0x0D052002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6FBF 
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
} DID_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */


/* PCICMD_MCIO_DDRIOEXTMCC_REG supported on:                                  */
/*       IVT_EP (0x2018F004)                                                  */
/*       HSX (0x201A7004)                                                     */
/*       BDX (0x201A7004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_MCIO_DDRIOEXTMCC_REG 0x0D052004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
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
} PCICMD_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */


/* PCISTS_MCIO_DDRIOEXTMCC_REG supported on:                                  */
/*       IVT_EP (0x2018F006)                                                  */
/*       HSX (0x201A7006)                                                     */
/*       BDX (0x201A7006)                                                     */
/* Register default value on IVT_EP:    0x0010                                */
/* Register default value on HSX:       0x0000                                */
/* Register default value on BDX:       0x0000                                */
#define PCISTS_MCIO_DDRIOEXTMCC_REG 0x0D052006

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
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
} PCISTS_MCIO_DDRIOEXTMCC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_MCIO_DDRIOEXTMCC_REG supported on:                                     */
/*       IVT_EP (0x1018F008)                                                  */
/*       HSX (0x101A7008)                                                     */
/*       BDX (0x101A7008)                                                     */
/* Register default value:              0x00                                  */
#define RID_MCIO_DDRIOEXTMCC_REG 0x0D051008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
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
} RID_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_MCIO_DDRIOEXTMCC_REG supported on:                                  */
/*       IVT_EP (0x1018F009)                                                  */
/*       HSX (0x101A7009)                                                     */
/*       BDX (0x101A7009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_MCIO_DDRIOEXTMCC_REG 0x0D051009

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_MCIO_DDRIOEXTMCC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_MCIO_DDRIOEXTMCC_REG supported on:                                  */
/*       IVT_EP (0x2018F00A)                                                  */
/*       HSX (0x201A700A)                                                     */
/*       BDX (0x201A700A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_MCIO_DDRIOEXTMCC_REG 0x0D05200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
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
} CCR_N1_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */


/* CLSR_MCIO_DDRIOEXTMCC_REG supported on:                                    */
/*       IVT_EP (0x1018F00C)                                                  */
/*       HSX (0x101A700C)                                                     */
/*       BDX (0x101A700C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_MCIO_DDRIOEXTMCC_REG 0x0D05100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
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
} CLSR_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */


/* PLAT_MCIO_DDRIOEXTMCC_REG supported on:                                    */
/*       IVT_EP (0x1018F00D)                                                  */
/*       HSX (0x101A700D)                                                     */
/*       BDX (0x101A700D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_MCIO_DDRIOEXTMCC_REG 0x0D05100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
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
} PLAT_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */


/* HDR_MCIO_DDRIOEXTMCC_REG supported on:                                     */
/*       IVT_EP (0x1018F00E)                                                  */
/*       HSX (0x101A700E)                                                     */
/*       BDX (0x101A700E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_MCIO_DDRIOEXTMCC_REG 0x0D05100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
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
} HDR_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */


/* BIST_MCIO_DDRIOEXTMCC_REG supported on:                                    */
/*       IVT_EP (0x1018F00F)                                                  */
/*       HSX (0x101A700F)                                                     */
/*       BDX (0x101A700F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_MCIO_DDRIOEXTMCC_REG 0x0D05100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
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
} BIST_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_MCIO_DDRIOEXTMCC_REG supported on:                                  */
/*       IVT_EP (0x1018F034)                                                  */
/*       HSX (0x101A7034)                                                     */
/*       BDX (0x101A7034)                                                     */
/* Register default value on IVT_EP:    0x40                                  */
/* Register default value on HSX:       0x00                                  */
/* Register default value on BDX:       0x00                                  */
#define CAPPTR_MCIO_DDRIOEXTMCC_REG 0x0D051034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
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
} CAPPTR_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */


/* INTL_MCIO_DDRIOEXTMCC_REG supported on:                                    */
/*       IVT_EP (0x1018F03C)                                                  */
/*       HSX (0x101A703C)                                                     */
/*       BDX (0x101A703C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_MCIO_DDRIOEXTMCC_REG 0x0D05103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
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
} INTL_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */


/* INTPIN_MCIO_DDRIOEXTMCC_REG supported on:                                  */
/*       IVT_EP (0x1018F03D)                                                  */
/*       HSX (0x101A703D)                                                     */
/*       BDX (0x101A703D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_MCIO_DDRIOEXTMCC_REG 0x0D05103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
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
} INTPIN_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */


/* MINGNT_MCIO_DDRIOEXTMCC_REG supported on:                                  */
/*       IVT_EP (0x1018F03E)                                                  */
/*       HSX (0x101A703E)                                                     */
/*       BDX (0x101A703E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_MCIO_DDRIOEXTMCC_REG 0x0D05103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
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
} MINGNT_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_MCIO_DDRIOEXTMCC_REG supported on:                                  */
/*       IVT_EP (0x1018F03F)                                                  */
/*       HSX (0x101A703F)                                                     */
/*       BDX (0x101A703F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_MCIO_DDRIOEXTMCC_REG 0x0D05103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
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
} MAXLAT_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */




/* DDRCRDATACOMP1_MCIO_DDRIOEXTMCC_REG supported on:                          */
/*       HSX (0x401A7108)                                                     */
/*       BDX (0x401A7108)                                                     */
/* Register default value on HSX:       0x4C300000                            */
/* Register default value on BDX:       0x0C030000                            */
#define DDRCRDATACOMP1_MCIO_DDRIOEXTMCC_REG 0x0D054108
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * This CR holds the second (of two) 32-bit register set for Data fub Compensation
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 28;
    UINT32 levelshiftercomp : 3;
    /* levelshiftercomp - Bits[30:28], RW_L, default = 3'b100 
       Controls pulse width of level shifter to external Vref to DIMM. not comp. 
       software control only. 
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DDRCRDATACOMP1_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * This CR holds the second (of two) 32-bit register set for Data fub Compensation. 
 *  BDX changed 
 */
typedef union {
  struct {
    UINT32 rcompodtup : 6;
    /* rcompodtup - Bits[5:0], RW_L, default = None 
       Sets the resistive value of the pull-up block of on die termination 
     */
    UINT32 rcompodtdown : 6;
    /* rcompodtdown - Bits[11:6], RW_L, default = None 
       Sets the resistive value of the pull-dn block of on die termination
     */
    UINT32 panicdrvdn : 10;
    /* panicdrvdn - Bits[21:12], RW_L, default = 10'b0000110000 
       Sets the resistive value of the VssHi Panic driver pulldown to move VssHi by a 
       precise dV when it gets out of spec 
     */
    UINT32 panicdrvup : 10;
    /* panicdrvup - Bits[31:22], RW_L, default = 10'b0000110000 
       Sets the resistive value of the VssHi Panic driver pullup to move VssHi by a 
       precise dV when it gets out of spec. 
     */
  } Bits;
  UINT32 Data;
} DDRCRDATACOMP1_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDCOMP_MCIO_DDRIOEXTMCC_REG supported on:                            */
/*       HSX (0x401A710C)                                                     */
/*       BDX (0x401A710C)                                                     */
/* Register default value on HSX:       0x08000000                            */
/* Register default value on BDX:       0x04000000                            */
#define DDRCRCMDCOMP_MCIO_DDRIOEXTMCC_REG 0x0D05410C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * This CR holds the 32-bit register set for Cmd fub Compensation
 */
typedef union {
  struct {
    UINT32 scomp : 6;
    /* scomp - Bits[5:0], RW_L, default = None 
       Set the delay between different segments of the output buffer to adjust slew 
       rate 
     */
    UINT32 tcocomp : 7;
    /* tcocomp - Bits[12:6], RW_L, default = None 
       Skews to incoming duty cycle to achieve 50% duty cycle at the transmitter output
     */
    UINT32 rcompdrvup : 6;
    /* rcompdrvup - Bits[18:13], RW_L, default = None 
       Sets the resistive value (aka driving strength) of the pull-up block of the 
       transmitter 
     */
    UINT32 rcompdrvdown : 6;
    /* rcompdrvdown - Bits[24:19], RW_L, default = None 
       Sets the resistive value (aka driving strength) of the pull-down block of the 
       transmitter 
     */
    UINT32 lscomp : 3;
    /* lscomp - Bits[27:25], RW_L, default = 3'b010 
       not used.
     */
    UINT32 spare : 4;
    /* spare - Bits[31:28], RW_L, default = None 
       Spare
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCOMP_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCTLCOMP_MCIO_DDRIOEXTMCC_REG supported on:                            */
/*       HSX (0x401A7110)                                                     */
/*       BDX (0x401A7110)                                                     */
/* Register default value on HSX:       0x04000000                            */
/* Register default value on BDX:       0x02000000                            */
#define DDRCRCTLCOMP_MCIO_DDRIOEXTMCC_REG 0x0D054110

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * This CR holds the 32-bit register set for Ctl fub Compensation
 */
typedef union {
  struct {
    UINT32 scomp : 5;
    /* scomp - Bits[4:0], RW_L, default = None 
       Set the delay between different segments of the output buffer to adjust slew 
       rate 
     */
    UINT32 tcocomp : 7;
    /* tcocomp - Bits[11:5], RW_L, default = None 
       Skews to incoming duty cycle to achieve 50% duty cycle at the transmitter output
     */
    UINT32 rcompdrvup : 6;
    /* rcompdrvup - Bits[17:12], RW_L, default = None 
       Sets the resistive value (aka driving strength) of the pull-up block of the 
       transmitter 
     */
    UINT32 rcompdrvdown : 6;
    /* rcompdrvdown - Bits[23:18], RW_L, default = None 
       Sets the resistive value (aka driving strength) of the pull-down block of the 
       transmitter 
     */
    UINT32 lscomp : 4;
    /* lscomp - Bits[27:24], RW_L, default = 3'b010 
       Sets the level shifter pulse width compensation value to optimize EOS vs. 
       Functionality.  
                 it is sent to CTL pins controling RxAmp for Loopback operation.
     */
    UINT32 spare : 4;
    /* spare - Bits[31:28], RW_L, default = None 
       Spare
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLCOMP_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCLKCOMP_CMP_MCIO_DDRIOEXTMCC_REG supported on:                        */
/*       HSX (0x401A7114)                                                     */
/*       BDX (0x401A7114)                                                     */
/* Register default value on HSX:       0x04000000                            */
/* Register default value on BDX:       0x02000000                            */
#define DDRCRCLKCOMP_CMP_MCIO_DDRIOEXTMCC_REG 0x0D054114

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * This CR holds the 32-bit register set for Clk fub Compensation
 */
typedef union {
  struct {
    UINT32 scomp : 5;
    /* scomp - Bits[4:0], RW_L, default = None 
       Set the delay between different segments of the output buffer to adjust slew 
       rate 
     */
    UINT32 tcocomp : 7;
    /* tcocomp - Bits[11:5], RW_L, default = None 
       Skews to incoming duty cycle to achieve 50% duty cycle at the transmitter output
     */
    UINT32 rcompdrvup : 6;
    /* rcompdrvup - Bits[17:12], RW_L, default = None 
       Sets the resistive value (aka driving strength) of the pull-up block of the 
       transmitter 
     */
    UINT32 rcompdrvdown : 6;
    /* rcompdrvdown - Bits[23:18], RW_L, default = None 
       Sets the resistive value (aka driving strength) of the pull-down block of the 
       transmitter 
     */
    UINT32 lscomp : 4;
    /* lscomp - Bits[27:24], RW_L, default = 3'b010 
       Sets the level shifter pulse width compensation value to optimize EOS vs. 
       Functionality.  
                 it is sent to CLK pins controling RxAmp for Loopback operation.
     */
    UINT32 spare : 4;
    /* spare - Bits[31:28], RW_L, default = None 
       Spare
     */
  } Bits;
  UINT32 Data;
} DDRCRCLKCOMP_CMP_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG supported on:                           */
/*       HSX (0x401A7118)                                                     */
/*       BDX (0x401A7118)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG 0x0D054118

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * This CR holds Compensation Controls like Vref Offset etc
 */
typedef union {
  struct {
    UINT32 lvmodevalid : 1;
    /* lvmodevalid - Bits[0:0], RW_L, default = None  */
    UINT32 lvmode : 2;
    /* lvmode - Bits[2:1], RW_L, default = 2'b0 
       low voltage mode (DDR3LV, DDR3U, DDR4, Intel SMI 2)
                   00: DDR3
                   01: DDR3LV or Intel SMI 2
                   10: DDR3U
                   11: DDR4
               DIMM support is processor type specific. 
               
     */
    UINT32 rsvd_3 : 8;
    UINT32 dqdrvpupvref : 7;
    /* dqdrvpupvref - Bits[17:11], RW_L, default = None 
       2's compliment analog offset for DqRcompDrvUp Vref with step size of Vccddq/64
     */
    UINT32 dqdrvpdnvref : 7;
    /* dqdrvpdnvref - Bits[24:18], RW_L, default = None 
       2's compliment analog offset for DqRcompDrvDown Vref with step size of Vccddq/96
     */
    UINT32 dqodtpupvref : 7;
    /* dqodtpupvref - Bits[31:25], RW_L, default = None 
       2's compliment analog offset for DqOdtcompDrvUp Vref with step size of Vccddq/96
     */
  } Bits;
  UINT32 Data;
} DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCOMPCTL1_MCIO_DDRIOEXTMCC_REG supported on:                           */
/*       HSX (0x401A711C)                                                     */
/*       BDX (0x401A711C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCOMPCTL1_MCIO_DDRIOEXTMCC_REG 0x0D05411C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * This CR holds Compensation Controls like Scomp Offset etc
 */
typedef union {
  struct {
    UINT32 dqscompcells : 4;
    /* dqscompcells - Bits[3:0], RW_L, default = None 
       # of delay cells in DqScomp delayline. (Min Setting > 3)
     */
    UINT32 dqscomppc : 1;
    /* dqscomppc - Bits[4:4], RW_L, default = None 
       When set (1'b1), SCOMP locks to a cycle, otherwise locks to a phase
     */
    UINT32 cmdscompcells : 4;
    /* cmdscompcells - Bits[8:5], RW_L, default = None 
       # of delay cells in CmdScomp delayline = CmdScompCells
     */
    UINT32 cmdscomppc : 1;
    /* cmdscomppc - Bits[9:9], RW_L, default = None 
       When set (1'b1), SCOMP locks to a cycle, otherwise locks to a phase
     */
    UINT32 ctlscompcells : 4;
    /* ctlscompcells - Bits[13:10], RW_L, default = None 
       # of delay cells in CtlScomp delayline = CtlScompCells
     */
    UINT32 ctlscomppc : 1;
    /* ctlscomppc - Bits[14:14], RW_L, default = None 
       When set (1'b1), SCOMP locks to a cycle, otherwise locks to a phase
     */
    UINT32 rsvd_15 : 6;
    UINT32 tcovref : 7;
    /* tcovref - Bits[27:21], RW_L, default = None 
       Vref control for TcoComp loop, can be used to offset FSM result
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} DDRCRCOMPCTL1_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCOMPCTL2_MCIO_DDRIOEXTMCC_REG supported on:                           */
/*       HSX (0x401A7120)                                                     */
/*       BDX (0x401A7120)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCOMPCTL2_MCIO_DDRIOEXTMCC_REG 0x0D054120

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * This CR holds Compensation Controls like ODT static leg, Rxbias etc
 */
typedef union {
  struct {
    UINT32 odtstatlegen : 2;
    /* odtstatlegen - Bits[1:0], RW_L, default = 2'b0 
       ODT Comp Loop Staic Leg Config. 00: 0-leg; 01: 3-leg; 10: 6-leg; 11: 12-leg
     */
    UINT32 txdqstatlegen : 2;
    /* txdqstatlegen - Bits[3:2], RW_L, default = 2'b0 
       DQ Comp Loop Staic Leg Config. 00: 0-leg; 01: 3-leg; 10: 6-leg; 11: 12-leg
     */
    UINT32 cmdstatlegen : 2;
    /* cmdstatlegen - Bits[5:4], RW_L, default = 2'b0 
       Cmd Comp Loop Staic Leg Config. 00: 0-leg; 01: 3-leg; 10: 6-leg; 11: 12-leg
     */
    UINT32 dqodtpdnvref : 7;
    /* dqodtpdnvref - Bits[12:6], RW_L, default = None 
       unsiged analog offset for DqOdtRcompDrvDown Vref with step size of 0.75mW
     */
    UINT32 clkdrvpupvref : 7;
    /* clkdrvpupvref - Bits[19:13], RW_L, default = None 
       unsiged analog offset for ClkRcompDrvUp Vref with step size of 0.75mV
     */
    UINT32 clkdrvpdnvref : 7;
    /* clkdrvpdnvref - Bits[26:20], RW_L, default = None 
       unsigned analog offset for ClkRcompDrvDown Vref with step size of 0.75mV
     */
    UINT32 rsvd_27 : 1;
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[28:28], RW_L, default = None 
       level shift hold enable
                            Holds the HV control values and power down the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling. 
     */
    UINT32 forceodton : 1;
    /* forceodton - Bits[29:29], RW_L, default = None 
       Force ODT on used in dq/clk/ctl rcomp AIP
     */
    UINT32 ddr3nren : 1;
    /* ddr3nren - Bits[30:30], RW_L, default = 2'b0 
       DDR3 Narrow Range
     */
    UINT32 highbwen : 1;
    /* highbwen - Bits[31:31], RW_L, default = 2'b0 
       highbwen for panicvtcomp
     */
  } Bits;
  UINT32 Data;
} DDRCRCOMPCTL2_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCOMPVSSHI_MCIO_DDRIOEXTMCC_REG supported on:                          */
/*       HSX (0x401A7124)                                                     */
/*       BDX (0x401A7124)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCOMPVSSHI_MCIO_DDRIOEXTMCC_REG 0x0D054124

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * This CR holds Compensation Controls for VssHi
 */
typedef union {
  struct {
    UINT32 panicdrvdnvref : 6;
    /* panicdrvdnvref - Bits[5:0], RW_L, default = None 
       Unsigned integer to select Vref for Panic DriverDn compensation. Step size of 
       VccIO/128 
     */
    UINT32 panicdrvupvref : 6;
    /* panicdrvupvref - Bits[11:6], RW_L, default = None 
       Unsigned integer to select Vref for Panic DriverUp compensation. Step size of 
       VccIO/128 
     */
    UINT32 vtoffset : 5;
    /* vtoffset - Bits[16:12], RW_L, default = None 
       Digital Offset to VtComp value.  Unsigned interger in the VccIO domain with a 
       step size of VccIO/128 
     */
    UINT32 vtslopea : 3;
    /* vtslopea - Bits[19:17], RW_L, default = None 
       Slope adjustment to the VtComp value.  {0: 0, 1: -1/4, 2: -1/8, 3: -1/16, 4: 
       +1/2, 5: +1/4, 6: +1/8, 7: +1/16} 
     */
    UINT32 vtslopeb : 3;
    /* vtslopeb - Bits[22:20], RW_L, default = None 
       Slope adjustment to the VtComp value.  {0: 0, 1: -1/4, 2: -1/8, 3: -1/16, 4: 
       +1/2, 5: +1/4, 6: +1/8, 7: +1/16} 
     */
    UINT32 clkdiv : 3;
    /* clkdiv - Bits[25:23], RW_L, default = None 
       divide by clk select for panicvt comp
     */
    UINT32 ddrdmvrfs3localpwrgoodoverride : 1;
    /* ddrdmvrfs3localpwrgoodoverride - Bits[26:26], RW_L, default = None 
       When this CSR bit is cleared,  S3LocalVcc in DIMM Vref is always on
     */
    UINT32 spare : 5;
    /* spare - Bits[31:27], RW_L, default = None 
       Spare
     */
  } Bits;
  UINT32 Data;
} DDRCRCOMPVSSHI_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCOMPOVR_MCIO_DDRIOEXTMCC_REG supported on:                            */
/*       HSX (0x401A7128)                                                     */
/*       BDX (0x401A7128)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCOMPOVR_MCIO_DDRIOEXTMCC_REG 0x0D054128
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * This CR holds bits for Override Enable for different Compensations
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 16;
    UINT32 dqtco : 1;
    /* dqtco - Bits[16:16], RW_L, default = None 
       Override Data Tco Value
     */
    UINT32 cmdtco : 1;
    /* cmdtco - Bits[17:17], RW_L, default = None 
       Override Cmd Tco Value
     */
    UINT32 ctltco : 1;
    /* ctltco - Bits[18:18], RW_L, default = None 
       Override Ctl Tco Value
     */
    UINT32 clktco : 1;
    /* clktco - Bits[19:19], RW_L, default = None 
       Override Clk Tco Value
     */
    UINT32 rsvd_20 : 12;
  } Bits;
  UINT32 Data;
} DDRCRCOMPOVR_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * This CR holds bits for Override Enable for different Compensations
 */
typedef union {
  struct {
    UINT32 dqdrvu : 1;
    /* dqdrvu - Bits[0:0], RW_L, default = None 
       Override Data Rcomp DriveUp Value
     */
    UINT32 dqdrvd : 1;
    /* dqdrvd - Bits[1:1], RW_L, default = None 
       Override Data Rcomp DriveDn Value
     */
    UINT32 dqodtu : 1;
    /* dqodtu - Bits[2:2], RW_L, default = None 
       Override Data Rcomp OdtUp Value
     */
    UINT32 dqodtd : 1;
    /* dqodtd - Bits[3:3], RW_L, default = None 
       Override Data Rcomp OdtDn Value
     */
    UINT32 cmddrvu : 1;
    /* cmddrvu - Bits[4:4], RW_L, default = None 
       Override Cmd Rcomp DriveUp Value
     */
    UINT32 cmddrvd : 1;
    /* cmddrvd - Bits[5:5], RW_L, default = None 
       Override Cmd Rcomp DriveDn Value
     */
    UINT32 ctldrvu : 1;
    /* ctldrvu - Bits[6:6], RW_L, default = None 
       Override Ctl Rcomp DriveUp Value
     */
    UINT32 ctldrvd : 1;
    /* ctldrvd - Bits[7:7], RW_L, default = None 
       Override Ctl Rcomp DriveDn Value
     */
    UINT32 clkdrvu : 1;
    /* clkdrvu - Bits[8:8], RW_L, default = None 
       Override Clk Rcomp DriveUp Value
     */
    UINT32 clkdrvd : 1;
    /* clkdrvd - Bits[9:9], RW_L, default = None 
       Override Clk Rcomp DriveDn Value
     */
    UINT32 dqsr : 1;
    /* dqsr - Bits[10:10], RW_L, default = None 
       Override Data Scomp Value
     */
    UINT32 cmdsr : 1;
    /* cmdsr - Bits[11:11], RW_L, default = None 
       Override Cmd Scomp Value
     */
    UINT32 ctlsr : 1;
    /* ctlsr - Bits[12:12], RW_L, default = None 
       Override Ctl Scomp Value
     */
    UINT32 clksr : 1;
    /* clksr - Bits[13:13], RW_L, default = None 
       Override Clk Scomp Value
     */
    UINT32 dqtcooff : 1;
    /* dqtcooff - Bits[14:14], RW_L, default = None 
       Override Data TcoOffset Value
     */
    UINT32 cmdtcooff : 1;
    /* cmdtcooff - Bits[15:15], RW_L, default = None 
       Override Cmd TcoOffset Value
     */
    UINT32 dqtco : 1;
    /* dqtco - Bits[16:16], RW_L, default = None 
       Override Data Tco Value
     */
    UINT32 cmdtco : 1;
    /* cmdtco - Bits[17:17], RW_L, default = None 
       Override Cmd Tco Value
     */
    UINT32 ctltco : 1;
    /* ctltco - Bits[18:18], RW_L, default = None 
       Override Ctl Tco Value
     */
    UINT32 clktco : 1;
    /* clktco - Bits[19:19], RW_L, default = None 
       Override Clk Tco Value
     */
    UINT32 ddr4extend : 1;
    /* ddr4extend - Bits[20:20], RW_L, default = None 
       Enable finer vref resolution in ddr4
     */
    UINT32 panicdrvup : 1;
    /* panicdrvup - Bits[21:21], RW_L, default = None 
       Override Panic DriveUp Value
     */
    UINT32 panicdrvdn : 1;
    /* panicdrvdn - Bits[22:22], RW_L, default = None 
       Override Panic DriveDn Value
     */
    UINT32 vtcomp : 1;
    /* vtcomp - Bits[23:23], RW_L, default = None 
       Override VT Comp Value
     */
    UINT32 genperbitupdate : 1;
    /* genperbitupdate - Bits[24:24], RW_L, default = 1'b0 
       Generate CSR based Comp Complete for per bit comp
     */
    UINT32 cttermination : 1;
    /* cttermination - Bits[25:25], RW_L, default = 1'b0 
        used to be VccddqHi in HSW, both are equivalent
                 1 - Nominal Vccddq voltage is 1.5v 
                 0 - Nominal Vccddq voltage is 1.35v or 1.2v
               
     */
    UINT32 spare2 : 4;
    /* spare2 - Bits[29:26], RW_L, default = None 
       Spare
     */
    UINT32 tcoaveragedisable : 1;
    /* tcoaveragedisable - Bits[30:30], RW_L, default = None 
       DFX bit to disable averaging tcocomp results - selects output of UP stage
     */
    UINT32 pdmuxselinvert : 1;
    /* pdmuxselinvert - Bits[31:31], RW_L, default = None 
       DFX feature that allows for a programmable invert on pdmuxsel that controls 
       tcocompana output mux. 
     */
  } Bits;
  UINT32 Data;
} DDRCRCOMPOVR_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCOMPVSSHICONTROL_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A712C)                                                     */
/*       BDX (0x401A712C)                                                     */
/* Register default value:              0x004D8238                            */
#define DDRCRCOMPVSSHICONTROL_MCIO_DDRIOEXTMCC_REG 0x0D05412C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * This CR holds bits to control the VssHi Generation
 */
typedef union {
  struct {
    UINT32 vsshiorvrefctl : 24;
    /* vsshiorvrefctl - Bits[23:0], RW_L, default = 24'h4d8238 
       VssHi control bits
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} DDRCRCOMPVSSHICONTROL_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A7130)                                                     */
/*       BDX (0x401A7130)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXTMCC_REG 0x0D054130
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * DimmVref Adjust Controls.
 */
typedef union {
  struct {
    UINT32 cavrefctl : 7;
    /* cavrefctl - Bits[6:0], RW_L, default = 7'b0 
        CA DIMM Vref Control. 
                             2s compliment encoding with 0 = Vdd/2, range of +/- Vdd/6 
       and step size of Vdd/384 (3.9mV) 
                             NOTE: LSB (bit 0) is a DFX feature (averaging between two 
       codes) - only use even codes so that LSB is '0'. 
               
     */
    UINT32 ch1vrefctl : 7;
    /* ch1vrefctl - Bits[13:7], RW_L, default = 7'b0 
        ch1 DQ DIMM Vref Control.
                             2s compliment encoding with 0 = Vdd/2, range of +/- Vdd/6 
       and step size of Vdd/384 (3.9mV) 
                             NOTE: LSB (bit 7) is a DFX feature (averaging between two 
       codes) - only use even codes so that LSB is '0'. 
               
     */
    UINT32 ch0vrefctl : 7;
    /* ch0vrefctl - Bits[20:14], RW_L, default = 7'b0 
        ch0 DQ DIMM Vref Control.
                             2s compliment encoding with 0 = Vdd/2, range of +/- Vdd/6 
       and step size of Vdd/384 (3.9mV) 
                             NOTE: LSB (bit 14) is a DFX feature - only use even codes 
       so that LSB is '0'. 
               
     */
    UINT32 endimmvrefca : 1;
    /* endimmvrefca - Bits[21:21], RW_L, default = 1'b0 
       Enables CA DIMM Vref
     */
    UINT32 endimmvrefch1 : 1;
    /* endimmvrefch1 - Bits[22:22], RW_L, default = 1'b0 
       Enables Channel 0 DIMM Vref
     */
    UINT32 endimmvrefch0 : 1;
    /* endimmvrefch0 - Bits[23:23], RW_L, default = 1'b0 
       Enables Channel 1 DIMM Vref
     */
    UINT32 hiztimerctrl : 2;
    /* hiztimerctrl - Bits[25:24], RW_L, default = 2'b0 
        For the HiZ Timer, selects the number of clocks for the timer to expire 
                             {0: 4 cycles, 1: 8 cycles, 2: 16 cycles, 3: 32 cycles}
               
     */
    UINT32 cttermination : 1;
    /* cttermination - Bits[26:26], RW_L, default = 1'b0 
       not used
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[28:27], RW_L, default = 1'b0 
       Reserved for future usage
     */
    UINT32 caslowbw : 1;
    /* caslowbw - Bits[29:29], RO_V, default = 1'b0 
       CA Vref controller has completed initial convergence and is in slow BW mode.  If 
       used as a lock detector, add 5uS after slow BW mode is achieved for full 
       convergence 
     */
    UINT32 ch0slowbw : 1;
    /* ch0slowbw - Bits[30:30], RO_V, default = 1'b0 
       Ch0 Vref controller has completed initial convergence and is in slow BW mode.  
       If used as a lock detector, add 5uS after slow BW mode is achieved for full 
       convergence 
     */
    UINT32 ch1slowbw : 1;
    /* ch1slowbw - Bits[31:31], RO_V, default = 1'b0 
       Ch1 Vref controller has completed initial convergence and is in slow BW mode.  
       If used as a lock detector, add 5uS after slow BW mode is achieved for full 
       convergence 
     */
  } Bits;
  UINT32 Data;
} DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * DimmVref Adjust Controls.
 */
typedef union {
  struct {
    UINT32 cavrefctl : 8;
    /* cavrefctl - Bits[7:0], RW_L, default = 8'b0 
        CA DIMM Vref Control. 
                             2s compliment encoding with 0 = Vdd/2, range of +/- Vdd/6 
       and step size of Vdd/384 (3.9mV) 
                             NOTE: LSB (bit 0) is a DFX feature (averaging between two 
       codes) - only use even codes so that LSB is '0'. 
               
     */
    UINT32 ch1vrefctl : 8;
    /* ch1vrefctl - Bits[15:8], RW_L, default = 8'b0 
        ch1 DQ DIMM Vref Control.
                             2s compliment encoding with 0 = Vdd/2, range of +/- Vdd/6 
       and step size of Vdd/384 (3.9mV) 
                             NOTE: LSB (bit 7) is a DFX feature (averaging between two 
       codes) - only use even codes so that LSB is '0'. 
               
     */
    UINT32 ch0vrefctl : 8;
    /* ch0vrefctl - Bits[23:16], RW_L, default = 8'b0 
        ch0 DQ DIMM Vref Control.
                             2s compliment encoding with 0 = Vdd/2, range of +/- Vdd/6 
       and step size of Vdd/384 (3.9mV) 
                             NOTE: LSB (bit 14) is a DFX feature - only use even codes 
       so that LSB is '0'. 
               
     */
    UINT32 endimmvrefca : 1;
    /* endimmvrefca - Bits[24:24], RW_L, default = 1'b0 
       Enables CA DIMM Vref
     */
    UINT32 endimmvrefch1 : 1;
    /* endimmvrefch1 - Bits[25:25], RW_L, default = 1'b0 
       Enables Channel 0 DIMM Vref
     */
    UINT32 endimmvrefch0 : 1;
    /* endimmvrefch0 - Bits[26:26], RW_L, default = 1'b0 
       Enables Channel 1 DIMM Vref
     */
    UINT32 hiztimerctrl : 2;
    /* hiztimerctrl - Bits[28:27], RW_L, default = 2'b0 
        For the HiZ Timer, selects the number of clocks for the timer to expire 
                             {0: 4 cycles, 1: 8 cycles, 2: 16 cycles, 3: 32 cycles}
               
     */
    UINT32 caslowbw : 1;
    /* caslowbw - Bits[29:29], RO_V, default = 1'b0 
       CA Vref controller has completed initial convergence and is in slow BW mode.  If 
       used as a lock detector, add 5uS after slow BW mode is achieved for full 
       convergence 
     */
    UINT32 ch0slowbw : 1;
    /* ch0slowbw - Bits[30:30], RO_V, default = 1'b0 
       Ch0 Vref controller has completed initial convergence and is in slow BW mode.  
       If used as a lock detector, add 5uS after slow BW mode is achieved for full 
       convergence 
     */
    UINT32 ch1slowbw : 1;
    /* ch1slowbw - Bits[31:31], RO_V, default = 1'b0 
       Ch1 Vref controller has completed initial convergence and is in slow BW mode.  
       If used as a lock detector, add 5uS after slow BW mode is achieved for full 
       convergence 
     */
  } Bits;
  UINT32 Data;
} DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRDIMMVREFCONTROL2_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A7134)                                                     */
/*       BDX (0x401A7134)                                                     */
/* Register default value:              0x000E4500                            */
#define DDRCRDIMMVREFCONTROL2_MCIO_DDRIOEXTMCC_REG 0x0D054134

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * VssHi Controls on Channel 0 only. DIMM Vref Controls on  Channel1, databyte 7, 
 * only. 
 */
typedef union {
  struct {
    UINT32 dimmvrefctl : 24;
    /* dimmvrefctl - Bits[23:0], RW_L, default = 24'h0E4500 
        DIMM VREF adjust settings.
                Field          Bits    Description
               -------         ----    
       --------------------------------------------------------------------------------------------------------------------------------------------------------------- 
               SelCode         23:21 Selects the code to output:  [0] = {0: Code, 1: 
       TCode}  [2:1] = {0: Select CA Code, 1: Select DQCh0 Code, 2: Select DQCh1 Code}  
        
        VtOffset 20:18 Offset for VtComp to convert from VccIO to VccDDQ.  Signed 
       integer in VccDDQ domain with a step size of VccDDQ/48 (32 mV) 
        VtSlopeA 17:15 Slope correction to convert between VtComp in VccIO to VccDDQ: 
       {0: 0, 1: -1/8, 2: -1/16, 3: -1/32. 4: +1/4, 5: +1/8, 6: +1/16. 7: +1/32} 
        VtSlopeB 14:12 Slope correction to convert between VtComp in VccIO to VccDDQ: 
       {0: 0, 1: -1/8, 2: -1/16, 3: -1/32. 4: +1/4, 5: +1/8, 6: +1/16. 7: +1/32} 
       	Reserved        11      Reserved for future usage
        HiBWEnable 10 Enables high bandwidth mode after a code change to allow fast 
       loop convergence 
        SlowBWError 9:8 In slow BW mode, limits the loop gain (ie: difference between 
       code and tcode) to +/- 2^(SlowBWError) 
        OpenLoop 7 Operates the dimm vref in open loop mode with the target Vref code 
       directly driving the output DAC with no feedback involved 
        SampleDivider 6:4 Controls the loop bandwidth by dividing down the input clock: 
       {0: Qclk, 1: Qclk/2, 2: Qclk/4, 3: Qclk/8, 4: Qclk/16, 5: Qclk/32, 6: Qclk/64, 
       7: Qclk/128} 
        LoBWDivider 3:2 For the slow integrator, selects the net number of samples in a 
       given direction to trip the filter.  {0: 4 samples, 1: 8 samples, 2: 16 samples, 
       3: 32 samples}  
        HiBWDivider 1:0 For the fast integrator, selects the net number of samples in a 
       given direction to trip the filter.  {0: 4 samples, 1: 8 samples, 2: 16 samples, 
       3: 32 samples}  
               
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} DDRCRDIMMVREFCONTROL2_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRSPDCFG1_MCIO_DDRIOEXTMCC_REG supported on:                            */
/*       HSX (0x401A7138)                                                     */
/*       BDX (0x401A7138)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRSPDCFG1_MCIO_DDRIOEXTMCC_REG 0x0D054138

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * SPD Comp Config
 */
typedef union {
  struct {
    UINT32 crspdcfg1 : 22;
    /* crspdcfg1 - Bits[21:0], RW_L, default = 1'b0 
         SPD config1
               Field Description: 
       
                Field             Bits    Description
               -------            ----    
       --------------------------------------------------------------------------------------------------------------------------------------------------------------- 
           
               spd_rcomp          21:15   view CComp
               spd_scomp_p        14:9   view SComp
               spd_scomp_n          8:3   view SComp
               spd_slowbuffer_ctl2    2   View Slow buffer control / Control 
       ddr_viewdig1  
               spd_slowbuffer_ctl1    1   View Slow buffer control / Control 
       ddr_viewdig0  
               spd_ddr_chdbg_sel      0   View Debug mux select in spd,  / 1'b1  - 
       select ch0  
               
     */
    UINT32 odtupsegen : 2;
    /* odtupsegen - Bits[23:22], RW_L, default = 2'b0 
       Odt up segment Enable
     */
    UINT32 odtdnsegen : 2;
    /* odtdnsegen - Bits[25:24], RW_L, default = 2'b0 
       Odt down segment Enable
     */
    UINT32 odtsegovrd : 1;
    /* odtsegovrd - Bits[26:26], RW_L, default = 1'b0 
       Dq segment Override
     */
    UINT32 rsvd : 5;
    /* rsvd - Bits[31:27], RW_L, default = 1'b0 
       Reserved for future usage
     */
  } Bits;
  UINT32 Data;
} DDRCRSPDCFG1_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRSPDCFG2_MCIO_DDRIOEXTMCC_REG supported on:                            */
/*       HSX (0x401A713C)                                                     */
/*       BDX (0x401A713C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRSPDCFG2_MCIO_DDRIOEXTMCC_REG 0x0D05413C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * SPD Comp Config
 */
typedef union {
  struct {
    UINT32 crspdcfg2 : 15;
    /* crspdcfg2 - Bits[14:0], RW_L, default = 1'b0 
         SPD config2
               Field Description: 
       
                Field             Bits    Description
               -------            ----    
       --------------------------------------------------------------------------------------------------------------------------------------------------------------- 
           
               spd_smbus1b           14   SPD/
               spd_hys_configp    13:12   SPD/Memhot/View
               spd_hys_confign_b  11:10   SPD/Memhot/View
               spd_rcompnctl        9:5   SPD/Memhot RComp
               spd_scompnctl        4:0   SPD/Memhot SComp
               
     */
    UINT32 dqupsegen : 2;
    /* dqupsegen - Bits[16:15], RW_L, default = 2'b0 
       Dq up segment Enable
     */
    UINT32 dqdnsegen : 2;
    /* dqdnsegen - Bits[18:17], RW_L, default = 2'b0 
       Dq down segment Enable
     */
    UINT32 dqsegovrd : 1;
    /* dqsegovrd - Bits[19:19], RW_L, default = 1'b0 
       Dq segment Override
     */
    UINT32 clkupsegen : 2;
    /* clkupsegen - Bits[21:20], RW_L, default = 2'b0 
       Clk up segment Enable
     */
    UINT32 clkdnsegen : 2;
    /* clkdnsegen - Bits[23:22], RW_L, default = 2'b0 
       Clk down segment Enable
     */
    UINT32 clksegovrd : 1;
    /* clksegovrd - Bits[24:24], RW_L, default = 1'b0 
       Clk segment Override
     */
    UINT32 ctlupsegen : 2;
    /* ctlupsegen - Bits[26:25], RW_L, default = 2'b0 
       Ctl up segment Enable
     */
    UINT32 ctldnsegen : 2;
    /* ctldnsegen - Bits[28:27], RW_L, default = 2'b0 
       Ctl down segment Enable
     */
    UINT32 ctlsegovrd : 1;
    /* ctlsegovrd - Bits[29:29], RW_L, default = 1'b0 
       Ctl segment Override
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[31:30], RW_L, default = 2'b0 
       Reserved for future usage
     */
  } Bits;
  UINT32 Data;
} DDRCRSPDCFG2_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCOMPCTL3_MCIO_DDRIOEXTMCC_REG supported on:                           */
/*       HSX (0x401A7140)                                                     */
/*       BDX (0x401A7140)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCOMPCTL3_MCIO_DDRIOEXTMCC_REG 0x0D054140

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * This CR holds Compensation Controls like cmd vref ctl, ctl vref ctl etc
 */
typedef union {
  struct {
    UINT32 cmddrvpupvref : 7;
    /* cmddrvpupvref - Bits[6:0], RW_L, default = None 
       2's compliment analog offset for CmdRcompDrvUp Vref with step size of Vccddq/96
     */
    UINT32 cmddrvpdnvref : 7;
    /* cmddrvpdnvref - Bits[13:7], RW_L, default = None 
       2's compliment analog offset for CmdRcompDrvDown Vref with step size of 
       Vccddq/96 
     */
    UINT32 ctldrvpupvref : 7;
    /* ctldrvpupvref - Bits[20:14], RW_L, default = None 
       2's compliment analog offset for CtlRcompDrvUp Vref with step size of Vccddq/96
     */
    UINT32 ctldrvpdnvref : 7;
    /* ctldrvpdnvref - Bits[27:21], RW_L, default = None 
       2's compliment analog offset for CtlRcompDrvDown Vref with step size of 
       Vccddq/96 
     */
    UINT32 swcapcmpclksel : 2;
    /* swcapcmpclksel - Bits[29:28], RW_L, default = None 
       Switch capacitor comparator clock select
     */
    UINT32 halflsbshiften : 1;
    /* halflsbshiften - Bits[30:30], RW_L, default = None 
       Half LSB Shift Enable
     */
    UINT32 dfxsel : 1;
    /* dfxsel - Bits[31:31], RW_L, default = 1'b0 
       dfxsel for cmd rcomp
     */
  } Bits;
  UINT32 Data;
} DDRCRCOMPCTL3_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCOMPVSSHICONTROL1_MCIO_DDRIOEXTMCC_REG supported on:                  */
/*       BDX (0x401A7148)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCOMPVSSHICONTROL1_MCIO_DDRIOEXTMCC_REG 0x0D054148

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * This CR holds bits to control the VssHi Generation
 */
typedef union {
  struct {
    UINT32 vsshiorvrefctl : 8;
    /* vsshiorvrefctl - Bits[7:0], RW_L, default = 8'h0 
       VssHi control bits
     */
    UINT32 spare : 24;
    /* spare - Bits[31:8], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DDRCRCOMPVSSHICONTROL1_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DDRCRCOMPVSSHI1_MCIO_DDRIOEXTMCC_REG supported on:                         */
/*       BDX (0x401A714C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCOMPVSSHI1_MCIO_DDRIOEXTMCC_REG 0x0D05414C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * This CR holds Compensation Controls for VssHi
 */
typedef union {
  struct {
    UINT32 spare0 : 18;
    /* spare0 - Bits[17:0], RW_L, default = None 
       Spare
     */
    UINT32 paniccompupmult : 7;
    /* paniccompupmult - Bits[24:18], RW_L, default = 7'b0 
       Multiplier to the Panic Comp Up code to expand to 10 bits
     */
    UINT32 paniccompdnmult : 7;
    /* paniccompdnmult - Bits[31:25], RW_L, default = 7'b0 
       Multiplier to the Panic Comp Dn code to expand to 10 bits
     */
  } Bits;
  UINT32 Data;
} DDRCRCOMPVSSHI1_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* RXGROUP0N0RANK0_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7200)                                                     */
/*       BDX (0x401A7200)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK0_7_MCIO_DDRIOEXTMCC_REG 0x0D054200

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank0
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS (0: 1/16 UI 
       offset,  
                 1: 3/16 UI Offset, 2: 5/16 UI Offsets, 3: 7/16 UI Offsets, 4: 9/16 UI 
       Offsets, 5,6,7: 11/16 UI Offsets.) 
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK0_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK1_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7204)                                                     */
/*       BDX (0x401A7204)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK1_7_MCIO_DDRIOEXTMCC_REG 0x0D054204

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank1
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK1_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK2_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7208)                                                     */
/*       BDX (0x401A7208)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK2_7_MCIO_DDRIOEXTMCC_REG 0x0D054208

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank2
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK2_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK3_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A720C)                                                     */
/*       BDX (0x401A720C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK3_7_MCIO_DDRIOEXTMCC_REG 0x0D05420C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank3
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK3_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK4_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7210)                                                     */
/*       BDX (0x401A7210)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK4_7_MCIO_DDRIOEXTMCC_REG 0x0D054210

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank4
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK4_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK5_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7214)                                                     */
/*       BDX (0x401A7214)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK5_7_MCIO_DDRIOEXTMCC_REG 0x0D054214

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank5
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK5_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK6_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7218)                                                     */
/*       BDX (0x401A7218)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK6_7_MCIO_DDRIOEXTMCC_REG 0x0D054218

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank6
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK6_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK7_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A721C)                                                     */
/*       BDX (0x401A721C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK7_7_MCIO_DDRIOEXTMCC_REG 0x0D05421C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank7
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK7_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK0_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7220)                                                     */
/*       BDX (0x401A7220)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK0_7_MCIO_DDRIOEXTMCC_REG 0x0D054220

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK0_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK1_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7224)                                                     */
/*       BDX (0x401A7224)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK1_7_MCIO_DDRIOEXTMCC_REG 0x0D054224

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK1_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK2_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7228)                                                     */
/*       BDX (0x401A7228)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK2_7_MCIO_DDRIOEXTMCC_REG 0x0D054228

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK2_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK3_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A722C)                                                     */
/*       BDX (0x401A722C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK3_7_MCIO_DDRIOEXTMCC_REG 0x0D05422C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK3_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK4_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7230)                                                     */
/*       BDX (0x401A7230)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK4_7_MCIO_DDRIOEXTMCC_REG 0x0D054230

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK4_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK5_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7234)                                                     */
/*       BDX (0x401A7234)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK5_7_MCIO_DDRIOEXTMCC_REG 0x0D054234

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK5_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK6_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7238)                                                     */
/*       BDX (0x401A7238)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK6_7_MCIO_DDRIOEXTMCC_REG 0x0D054238

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK6_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK7_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A723C)                                                     */
/*       BDX (0x401A723C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK7_7_MCIO_DDRIOEXTMCC_REG 0x0D05423C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK7_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP0N0RANK0_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7240)                                                     */
/*       BDX (0x401A7240)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK0_7_MCIO_DDRIOEXTMCC_REG 0x0D054240
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK0_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK0_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK1_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7244)                                                     */
/*       BDX (0x401A7244)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK1_7_MCIO_DDRIOEXTMCC_REG 0x0D054244
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK1_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK1_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK2_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7248)                                                     */
/*       BDX (0x401A7248)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK2_7_MCIO_DDRIOEXTMCC_REG 0x0D054248
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on. 
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK2_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK2_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK3_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A724C)                                                     */
/*       BDX (0x401A724C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK3_7_MCIO_DDRIOEXTMCC_REG 0x0D05424C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK3_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK3_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK4_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7250)                                                     */
/*       BDX (0x401A7250)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK4_7_MCIO_DDRIOEXTMCC_REG 0x0D054250
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK4_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK4_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK5_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7254)                                                     */
/*       BDX (0x401A7254)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK5_7_MCIO_DDRIOEXTMCC_REG 0x0D054254
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK5_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK5_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK6_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7258)                                                     */
/*       BDX (0x401A7258)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK6_7_MCIO_DDRIOEXTMCC_REG 0x0D054258
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK6_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK6_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK7_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A725C)                                                     */
/*       BDX (0x401A725C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK7_7_MCIO_DDRIOEXTMCC_REG 0x0D05425C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK7_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK7_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP1N0RANK0_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7260)                                                     */
/*       BDX (0x401A7260)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK0_7_MCIO_DDRIOEXTMCC_REG 0x0D054260

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 6'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 6'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 6'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 6'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK0_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK1_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7264)                                                     */
/*       BDX (0x401A7264)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK1_7_MCIO_DDRIOEXTMCC_REG 0x0D054264

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK1_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK2_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7268)                                                     */
/*       BDX (0x401A7268)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK2_7_MCIO_DDRIOEXTMCC_REG 0x0D054268

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK2_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK3_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A726C)                                                     */
/*       BDX (0x401A726C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK3_7_MCIO_DDRIOEXTMCC_REG 0x0D05426C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK3_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK4_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7270)                                                     */
/*       BDX (0x401A7270)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK4_7_MCIO_DDRIOEXTMCC_REG 0x0D054270

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK4_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK5_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7274)                                                     */
/*       BDX (0x401A7274)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK5_7_MCIO_DDRIOEXTMCC_REG 0x0D054274

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK5_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK6_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7278)                                                     */
/*       BDX (0x401A7278)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK6_7_MCIO_DDRIOEXTMCC_REG 0x0D054278

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK6_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK7_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A727C)                                                     */
/*       BDX (0x401A727C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK7_7_MCIO_DDRIOEXTMCC_REG 0x0D05427C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK7_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK0_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7280)                                                     */
/*       BDX (0x401A7280)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK0_7_MCIO_DDRIOEXTMCC_REG 0x0D054280

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK0_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK1_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7284)                                                     */
/*       BDX (0x401A7284)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK1_7_MCIO_DDRIOEXTMCC_REG 0x0D054284

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK1_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK2_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7288)                                                     */
/*       BDX (0x401A7288)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK2_7_MCIO_DDRIOEXTMCC_REG 0x0D054288

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK2_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK3_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A728C)                                                     */
/*       BDX (0x401A728C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK3_7_MCIO_DDRIOEXTMCC_REG 0x0D05428C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK3_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK4_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7290)                                                     */
/*       BDX (0x401A7290)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK4_7_MCIO_DDRIOEXTMCC_REG 0x0D054290

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK4_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK5_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7294)                                                     */
/*       BDX (0x401A7294)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK5_7_MCIO_DDRIOEXTMCC_REG 0x0D054294

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK5_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK6_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7298)                                                     */
/*       BDX (0x401A7298)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK6_7_MCIO_DDRIOEXTMCC_REG 0x0D054298

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK6_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK7_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A729C)                                                     */
/*       BDX (0x401A729C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK7_7_MCIO_DDRIOEXTMCC_REG 0x0D05429C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK7_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* RXVREFCTRLN0_7_MCIO_DDRIOEXTMCC_REG supported on:                          */
/*       HSX (0x401A72A4)                                                     */
/*       BDX (0x401A72A4)                                                     */
/* Register default value:              0x00000000                            */
#define RXVREFCTRLN0_7_MCIO_DDRIOEXTMCC_REG 0x0D0542A4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * RxVrefCtrl
 */
typedef union {
  struct {
    UINT32 vrefperbit0 : 7;
    /* vrefperbit0 - Bits[6:0], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben0 : 1;
    /* halflsben0 - Bits[7:7], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit1 : 7;
    /* vrefperbit1 - Bits[14:8], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben1 : 1;
    /* halflsben1 - Bits[15:15], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit2 : 7;
    /* vrefperbit2 - Bits[22:16], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben2 : 1;
    /* halflsben2 - Bits[23:23], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit3 : 7;
    /* vrefperbit3 - Bits[30:24], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben3 : 1;
    /* halflsben3 - Bits[31:31], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
  } Bits;
  UINT32 Data;
} RXVREFCTRLN0_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATATRAINFEEDBACKN0_7_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A72A8)                                                     */
/*       BDX (0x401A72A8)                                                     */
/* Register default value:              0x00000000                            */
#define DATATRAINFEEDBACKN0_7_MCIO_DDRIOEXTMCC_REG 0x0D0542A8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data Training Feedback Results
 */
typedef union {
  struct {
    UINT32 datatrainfeedback : 18;
    /* datatrainfeedback - Bits[17:0], RW_L, default = 1'b0 
       Data Training Feedback Storage
       
               DataTrainFeedback Description:
                Training Step    Bits    Nibble    Description
               --------------    ----    -------  
       -------------------------------------------------------------------------------------------------------------------------------------------------------- 
               Cross Over Cal     0      0        Tx Dqs Phase Detection
               Cross Over Cal     1      0        Tx Dq Phase Detection
               Cross Over Cal     2      0        Rec En Phase Detection
               RcvEn              8:0    0        Byte detection
               Rx Dq-Dqs          0      0        Pass/fail
               Wr Leveling        8:0    0        Byte detection
               Cross Over Cal     3      1        Tx Dqs Phase Detection
               Cross Over Cal     4      1        Tx Dq Phase Detection
               Cross Over Cal     5      1        Rec En Phase Detection
               RcvEn             17:9    1        Byte detection
               Rx Dq-Dqs          1      1        Pass/fail
               Wr Leveling       17:9    1        Byte detection
               
     */
    UINT32 rsvd_18 : 14;
  } Bits;
  UINT32 Data;
} DATATRAINFEEDBACKN0_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATACONTROL4_7_MCIO_DDRIOEXTMCC_REG supported on:                          */
/*       HSX (0x401A72AC)                                                     */
/*       BDX (0x401A72AC)                                                     */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       <could not determine>                 */
#define DATACONTROL4_7_MCIO_DDRIOEXTMCC_REG 0x0D0542AC
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * static configurations of tx and rx.
 */
typedef union {
  struct {
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[0:0], RW_L, default = None 
       enable grid clock divider in xover. used in native ddr mode for data rate 
       greater than 1867MT/s 
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[1:1], RW_L, default = None 
       enable PiRef clock divider in xover. used in vmse2to2mode, or whenever data rate 
       greater than 1867MT/s 
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[2:2], RW_L, default = None 
       enable extra pipestage in xover. POR setting = 1
     */
    UINT32 vmsehalfwidthmode : 1;
    /* vmsehalfwidthmode - Bits[3:3], RW_L, default = None 
       gate ClkPi when the nibble is disabled in VMSE half-width mode
     */
    UINT32 disablercvenextension : 1;
    /* disablercvenextension - Bits[4:4], RW_L, default = None 
       By default receive enable extension will be bypassed in VMSE2to1 mode. Setting 
       this to 1 in VMSE mode will use DDR to extend the rcven. VMSE ACIO LB use case 
       when only one scheduler is used. 
     */
    UINT32 ddrcrinvertpiclk : 2;
    /* ddrcrinvertpiclk - Bits[6:5], RW_L, default = 2'b00 
       invert ClkPiRef (per nibble) in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 
       phase. 
     */
    UINT32 dqvmselogicdelay : 8;
    /* dqvmselogicdelay - Bits[14:7], RW_L, default = 1'b0 
       DqVmseLogicDelay: If set add one UI delay to the Tx Fifo Read and DQ drivenable
     */
    UINT32 rcvenvmselogicdelay : 2;
    /* rcvenvmselogicdelay - Bits[16:15], RW_L, default = 1'b0 
       Receive enable VmseLogicDelay: If set add one UI delay to the receive enable
     */
    UINT32 dqsvmselogicdelay : 2;
    /* dqsvmselogicdelay - Bits[18:17], RW_L, default = 1'b0 
       DQS VMSE Logic Delay
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[19:19], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 ddrsroslmask : 1;
    /* ddrsroslmask - Bits[20:20], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 dqvmsenibblelogicdelay : 2;
    /* dqvmsenibblelogicdelay - Bits[22:21], RW_L, default = None 
       VMSE Logic delay that is shared for all bits in the nibble
     */
    UINT32 rsvd_23 : 9;
  } Bits;
  UINT32 Data;
} DATACONTROL4_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * static configurations of tx and rx.
 */
typedef union {
  struct {
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[0:0], RW_L, default = None 
       enable grid clock divider in xover. used in native ddr mode for data rate 
       greater than 1867MT/s 
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[1:1], RW_L, default = None 
       enable PiRef clock divider in xover. used in vmse2to2mode, or whenever data rate 
       greater than 1867MT/s 
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[2:2], RW_L, default = None 
       enable extra pipestage in xover. POR setting = 1
     */
    UINT32 vmsehalfwidthmode : 1;
    /* vmsehalfwidthmode - Bits[3:3], RW_L, default = None 
       gate ClkPi when the nibble is disabled in VMSE half-width mode
     */
    UINT32 disablercvenextension : 1;
    /* disablercvenextension - Bits[4:4], RW_L, default = None 
       By default receive enable extension will be bypassed in VMSE2to1 mode. Setting 
       this to 1 in VMSE mode will use DDR to extend the rcven. VMSE ACIO LB use case 
       when only one scheduler is used. 
     */
    UINT32 ddrcrinvertpiclk : 2;
    /* ddrcrinvertpiclk - Bits[6:5], RW_L, default = 2'b00 
       invert ClkPiRef (per nibble) in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 
       phase. 
     */
    UINT32 dqvmselogicdelay : 8;
    /* dqvmselogicdelay - Bits[14:7], RW_L, default = 1'b0 
       DqVmseLogicDelay: If set add one UI delay to the Tx Fifo Read and DQ drivenable
     */
    UINT32 rcvenvmselogicdelay : 2;
    /* rcvenvmselogicdelay - Bits[16:15], RW_L, default = 1'b0 
       Receive enable VmseLogicDelay: If set add one UI delay to the receive enable
     */
    UINT32 dqsvmselogicdelay : 2;
    /* dqsvmselogicdelay - Bits[18:17], RW_L, default = 1'b0 
       DQS VMSE Logic Delay
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[19:19], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 ddrsroslmask : 1;
    /* ddrsroslmask - Bits[20:20], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 dqvmsenibblelogicdelay : 2;
    /* dqvmsenibblelogicdelay - Bits[22:21], RW_L, default = None 
       VMSE Logic delay that is shared for all bits in the nibble
     */
    UINT32 writecrcenable : 1;
    /* writecrcenable - Bits[23:23], RW_L, default = None 
       Enable write crc mode
     */
    UINT32 bdx_sys : 1;
    /* bdx_sys - Bits[24:24], RW_L, default = '1 
       This bit helps to bypass the delaycell in transmitter
     */
    UINT32 rsvd_25 : 6;
    UINT32 bypassinitoptolatchenable : 1;
    /* bypassinitoptolatchenable - Bits[31:31], RW_L, default = None 
       This will ignore an InitOp when the RFFSM is active for a read operation
     */
  } Bits;
  UINT32 Data;
} DATACONTROL4_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* VSSHIORVREFCONTROL1_7_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       BDX (0x401A72CC)                                                     */
/* Register default value:              0x00000000                            */
#define VSSHIORVREFCONTROL1_7_MCIO_DDRIOEXTMCC_REG 0x0D0542CC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * VssHi Controls on Channel 0 only. DIMM Vref Controls on  Channel1, databyte 7, 
 * only. Extension for BDX. 
 */
typedef union {
  struct {
    UINT32 vsshiorvrefctl : 8;
    /* vsshiorvrefctl - Bits[7:0], RW_L, default = 8'b0 
        BDX-specific extended control signal. 
     */
    UINT32 spare : 24;
    /* spare - Bits[31:8], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} VSSHIORVREFCONTROL1_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* PXCATTACK1SEL_7_MCIO_DDRIOEXTMCC_REG supported on:                         */
/*       BDX (0x401A72D0)                                                     */
/* Register default value:              0x00FAC688                            */
#define PXCATTACK1SEL_7_MCIO_DDRIOEXTMCC_REG 0x0D0542D0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Select 2 aggressors out 3 neighbors in a nibble
 */
typedef union {
  struct {
    UINT32 attackr1d0 : 3;
    /* attackr1d0 - Bits[2:0], RW_L, default = 3'd0 
       xtalk aggressor1 select for dq bit[0] in a byte
     */
    UINT32 attackr1d1 : 3;
    /* attackr1d1 - Bits[5:3], RW_L, default = 3'd1 
       xtalk aggressor1 select for dq bit[1] in a byte
     */
    UINT32 attackr1d2 : 3;
    /* attackr1d2 - Bits[8:6], RW_L, default = 3'd2 
       xtalk aggressor1 select for dq bit[2] in a byte
     */
    UINT32 attackr1d3 : 3;
    /* attackr1d3 - Bits[11:9], RW_L, default = 3'd3 
       xtalk aggressor1 select for dq bit[3] in a byte
     */
    UINT32 attackr1d4 : 3;
    /* attackr1d4 - Bits[14:12], RW_L, default = 3'd4 
       xtalk aggressor1 select for dq bit[4] in a byte
     */
    UINT32 attackr1d5 : 3;
    /* attackr1d5 - Bits[17:15], RW_L, default = 3'd5 
       xtalk aggressor1 select for dq bit[5] in a byte
     */
    UINT32 attackr1d6 : 3;
    /* attackr1d6 - Bits[20:18], RW_L, default = 3'd6 
       xtalk aggressor1 select for dq bit[6] in a byte
     */
    UINT32 attackr1d7 : 3;
    /* attackr1d7 - Bits[23:21], RW_L, default = 3'd7 
       xtalk aggressor1 select for dq bit[7] in a byte
     */
    UINT32 spare : 8;
    /* spare - Bits[31:24], RW_L, default = 8'b0 
       spare bits.
     */
  } Bits;
  UINT32 Data;
} PXCATTACK1SEL_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */






/* DATAOFFSETTRAIN_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A72DC)                                                     */
/*       BDX (0x401A72DC)                                                     */
/* Register default value:              0x00000000                            */
#define DATAOFFSETTRAIN_7_MCIO_DDRIOEXTMCC_REG 0x0D0542DC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rcvenoffset : 6;
    /* rcvenoffset - Bits[5:0], RW_L, default = 1'b0 
       Offset added to Trained RxRcvEn Value.
     */
    UINT32 rxdqsoffset : 7;
    /* rxdqsoffset - Bits[12:6], RW_L, default = 1'b0 
       Offset added to Trained Rx DQS Value.
     */
    UINT32 txdqoffset : 6;
    /* txdqoffset - Bits[18:13], RW_L, default = 2'b00 
       Offset added to Trained Tx DQ Value.
     */
    UINT32 txdqsoffset : 6;
    /* txdqsoffset - Bits[24:19], RW_L, default = 6'b000000 
       Offset added to Trained Tx DQS Value.
     */
    UINT32 vrefoffset : 7;
    /* vrefoffset - Bits[31:25], RW_L, default = 7'b0000000 
       unsiged offset added to Trained Vref Value. each step is Vdd/192.
     */
  } Bits;
  UINT32 Data;
} DATAOFFSETTRAIN_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* PXCATTACK0SEL_7_MCIO_DDRIOEXTMCC_REG supported on:                         */
/*       BDX (0x401A72E4)                                                     */
/* Register default value:              0x00FAC688                            */
#define PXCATTACK0SEL_7_MCIO_DDRIOEXTMCC_REG 0x0D0542E4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Select 2 aggressors out 3 neighbors in a nibble
 */
typedef union {
  struct {
    UINT32 attackr0d0 : 3;
    /* attackr0d0 - Bits[2:0], RW_L, default = 3'd0 
       xtalk aggressor0 select for dq bit[0] in a byte
     */
    UINT32 attackr0d1 : 3;
    /* attackr0d1 - Bits[5:3], RW_L, default = 3'd1 
       xtalk aggressor0 select for dq bit[1] in a byte
     */
    UINT32 attackr0d2 : 3;
    /* attackr0d2 - Bits[8:6], RW_L, default = 3'd2 
       xtalk aggressor0 select for dq bit[2] in a byte
     */
    UINT32 attackr0d3 : 3;
    /* attackr0d3 - Bits[11:9], RW_L, default = 3'd3 
       xtalk aggressor0 select for dq bit[3] in a byte
     */
    UINT32 attackr0d4 : 3;
    /* attackr0d4 - Bits[14:12], RW_L, default = 3'd4 
       xtalk aggressor0 select for dq bit[4] in a byte
     */
    UINT32 attackr0d5 : 3;
    /* attackr0d5 - Bits[17:15], RW_L, default = 3'd5 
       xtalk aggressor0 select for dq bit[5] in a byte
     */
    UINT32 attackr0d6 : 3;
    /* attackr0d6 - Bits[20:18], RW_L, default = 3'd6 
       xtalk aggressor0 select for dq bit[6] in a byte
     */
    UINT32 attackr0d7 : 3;
    /* attackr0d7 - Bits[23:21], RW_L, default = 3'd7 
       xtalk aggressor0 select for dq bit[7] in a byte
     */
    UINT32 spare : 8;
    /* spare - Bits[31:24], RW_L, default = 8'b0 
       spare bits.
     */
  } Bits;
  UINT32 Data;
} PXCATTACK0SEL_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATACONTROL0_7_MCIO_DDRIOEXTMCC_REG supported on:                          */
/*       HSX (0x401A72E8)                                                     */
/*       BDX (0x401A72E8)                                                     */
/* Register default value:              0x00000000                            */
#define DATACONTROL0_7_MCIO_DDRIOEXTMCC_REG 0x0D0542E8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rxtrainingmode : 1;
    /* rxtrainingmode - Bits[0:0], RW_L, default = 1'b0 
       Enables Read DqDqs Training Mode
     */
    UINT32 wltrainingmode : 1;
    /* wltrainingmode - Bits[1:1], RW_L, default = 1'b0 
       Enables Write Leveling Training Mode
     */
    UINT32 rltrainingmode : 1;
    /* rltrainingmode - Bits[2:2], RW_L, default = 1'b0 
       Enables Read Leveling Training Mode.
     */
    UINT32 senseamptrainingmode : 1;
    /* senseamptrainingmode - Bits[3:3], RW_L, default = 1'b0 
       Enables SenseAmp offset cancellation training mode.
     */
    UINT32 rsvd_4 : 1;
    UINT32 rfon : 1;
    /* rfon - Bits[5:5], RW_L, default = 1'b0 
       Power-down Disable:  Forces register file read, overriding the rank power down 
       logic. 
     */
    UINT32 rsvd_6 : 1;
    UINT32 txpion : 1;
    /* txpion - Bits[7:7], RW_L, default = 1'b0 
       Power-down Disable:  Forces TxDq and TxDqs PI clocks on.
     */
    UINT32 rsvd_8 : 1;
    UINT32 dcdetectmode : 1;
    /* dcdetectmode - Bits[9:9], RW_L, default = 1'b0 
       Enable VMSE DC detect mode.
     */
    UINT32 rsvd_10 : 2;
    UINT32 txlong : 1;
    /* txlong - Bits[12:12], RW_L, default = 1'b0 
       Sets the duration of the first transmitted DQ bit of the burst. 0=1UI, 1=2UI.
     */
    UINT32 rsvd_13 : 5;
    UINT32 driversegmentenable : 1;
    /* driversegmentenable - Bits[18:18], RW_L, default = 1'b0 
       Controls which segements of the driver are enabled:  {0: All, 1: Only ODT}.
     */
    UINT32 rsvd_19 : 1;
    UINT32 readrfrd : 1;
    /* readrfrd - Bits[20:20], RW_L, default = 1'b0 
       Self clearing command bit. When set, it will download the three 32 bit lines 
       from the RegFile associated with a read c 
       ommand to Rank = ReadRFRank
     */
    UINT32 readrfwr : 1;
    /* readrfwr - Bits[21:21], RW_L, default = 1'b0 
       Self clearing command bit. When set, it will download the three 32 bit lines 
       from the RegFile associated with a write  
       command to Rank = ReadRFRank
     */
    UINT32 readrfrank : 3;
    /* readrfrank - Bits[24:22], RW_L, default = 3'b000 
       Specifies the rank that ReadRFRd or ReadRFWr will manually download the CR 
       values from 
     */
    UINT32 forceodton : 1;
    /* forceodton - Bits[25:25], RW_L, default = 1'b0 
       ODT is forced-on. NOTE: Cannot be set if LongODTR2W is set or can create glitch 
       in transmitter.   NOTE: If forceodton = 1, odtduration should be set to 0 
     */
    UINT32 rsvd_26 : 3;
    UINT32 burstlength4 : 1;
    /* burstlength4 - Bits[29:29], RW_L, default = 1'b0 
       enable lockstep mode, burst length 4
     */
    UINT32 xovercal : 1;
    /* xovercal - Bits[30:30], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
    UINT32 longpreambleenable : 1;
    /* longpreambleenable - Bits[31:31], RW_L, default = 1'b0 
       Enables long preamble for DDR4 devices
     */
  } Bits;
  UINT32 Data;
} DATACONTROL0_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATACONTROL1_7_MCIO_DDRIOEXTMCC_REG supported on:                          */
/*       HSX (0x401A72EC)                                                     */
/*       BDX (0x401A72EC)                                                     */
/* Register default value:              0x00000000                            */
#define DATACONTROL1_7_MCIO_DDRIOEXTMCC_REG 0x0D0542EC
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 7;
    UINT32 sdlldisable : 2;
    /* sdlldisable - Bits[8:7], RW_L, default = 2'b00 
       Controls which slave DLL are disabled: used in VMSE half-width mode to turn off 
       the unused slave delay line. 
                 Set to 0 to have nothing disabled.
                 Set to 1 to have nibble0 disabled.
                 Set to 2 to have nibble1 disabled.
                 Set to 3 to have nibble0 and nibble 1 disabled.
               
     */
    UINT32 rsvd_9 : 1;
    UINT32 rxbiasctl : 3;
    /* rxbiasctl - Bits[12:10], RW_L, default = 1'b0 
       Controls the Current and BW of the Receiver: 
                 {0: 0.34Idll, 1: 0.66Idll, 2: 0.88Idll, 3: Idll, 4: 1.33Idll, 5: 
       1.66Idll, 6: 2.00Idll, 7: 2.33Idll} 
     */
    UINT32 odtdelay : 4;
    /* odtdelay - Bits[16:13], RW_L, default = 4'b0000 
       Controls ODT turn-on delay.   To turn on N tQCK before RcvEn, program to 
       RcvEn[8:6]  N + 4. 
     */
    UINT32 odtduration : 3;
    /* odtduration - Bits[19:17], RW_L, default = 4'b0000 
       Controls the ODT ON duration from (Code, Duration) = (0x0, 11 tQCK) to (0x7, 18 
       tQCK).  NOTE: If forceodton = 1, this should be set to 0 
     */
    UINT32 senseampdelay : 4;
    /* senseampdelay - Bits[23:20], RW_L, default = 4'b0000 
       Controls Senseamp turn-on delay.   To turn on N tQCK before RcvEn, program to 
       RcvEn[8:6]  N + 4. 
     */
    UINT32 senseampduration : 3;
    /* senseampduration - Bits[26:24], RW_L, default = 4'b0000 
       Controls the SenseAmp ON duration from (Code, Duration) = (0x0, 11 tQCK) to 
       (0x7, 18 tQCK) 
     */
    UINT32 burstendodtdelay : 3;
    /* burstendodtdelay - Bits[29:27], RW_L, default = 3'b000 
       Cycles after the burst, when the current pulse should turn on by enabling On-die 
       Terminators 
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} DATACONTROL1_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 7;
    UINT32 sdlldisable : 2;
    /* sdlldisable - Bits[8:7], RW_L, default = 2'b00 
       Controls which slave DLL are disabled: used in vmse half-width mode to turn off 
       the unused slave delay line. 
                 Set to 0 to have nothing disabled.
                 Set to 1 to have nibble0 disabled.
                 Set to 2 to have nibble1 disabled.
                 Set to 3 to have nibble0 and nibble 1 disabled.
               
     */
    UINT32 rsvd_9 : 1;
    UINT32 rxbiasctl : 3;
    /* rxbiasctl - Bits[12:10], RW_L, default = 1'b0 
       Controls the Current and BW of the Receiver: 
                 {0: 0.34Idll, 1: 0.66Idll, 2: 0.88Idll, 3: Idll, 4: 1.33Idll, 5: 
       1.66Idll, 6: 2.00Idll, 7: 2.33Idll} 
     */
    UINT32 odtdelay : 4;
    /* odtdelay - Bits[16:13], RW_L, default = 4'b0000 
       Controls ODT turn-on delay.   To turn on N tQCK before RcvEn, program to 
       RcvEn[8:6]  N + 4. 
     */
    UINT32 odtduration : 3;
    /* odtduration - Bits[19:17], RW_L, default = 4'b0000 
       Controls the ODT ON duration from (Code, Duration) = (0x0, 11 tQCK) to (0x7, 18 
       tQCK).  NOTE: If forceodton = 1, this should be set to 0 
     */
    UINT32 senseampdelay : 4;
    /* senseampdelay - Bits[23:20], RW_L, default = 4'b0000 
       Controls Senseamp turn-on delay.   To turn on N tQCK before RcvEn, program to 
       RcvEn[8:6]  N + 4. 
     */
    UINT32 senseampduration : 3;
    /* senseampduration - Bits[26:24], RW_L, default = 4'b0000 
       Controls the SenseAmp ON duration from (Code, Duration) = (0x0, 11 tQCK) to 
       (0x7, 18 tQCK) 
     */
    UINT32 burstendodtdelay : 3;
    /* burstendodtdelay - Bits[29:27], RW_L, default = 3'b000 
       Cycles after the burst, when the current pulse should turn on by enabling On-die 
       Terminators 
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} DATACONTROL1_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DATACONTROL2_7_MCIO_DDRIOEXTMCC_REG supported on:                          */
/*       HSX (0x401A72F0)                                                     */
/*       BDX (0x401A72F0)                                                     */
/* Register default value:              0x00000000                            */
#define DATACONTROL2_7_MCIO_DDRIOEXTMCC_REG 0x0D0542F0
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rxstaggerctl : 5;
    /* rxstaggerctl - Bits[4:0], RW_L, default = 5'b0 
       Delay DdrRxEnableQnnnH by RxStaggerCtl qclks (0-31) before turning on/off the 
       RxAmps  
               If RxStaggerCtl = 0, use the ODT Delay/Duration settings for turning 
       on/off the Rx amplifier  
     */
    UINT32 forcebiason : 1;
    /* forcebiason - Bits[5:5], RW_L, default = 1'b0 
       Force on the internal Vref and Rx bias circuit, regardless of any other power 
       downs 
     */
    UINT32 forcerxon : 1;
    /* forcerxon - Bits[6:6], RW_L, default = 1'b0 
       Force on the RxAmp only (as opposed to OdtSampOn, which turns on both ODT and 
       the amplifier). 
               Use for overclocking support where we may not be able to hit the fast 
       exit latency  
     */
    UINT32 rsvd_7 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[8:8], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/VMSE
     */
    UINT32 imodecfg : 1;
    /* imodecfg - Bits[9:9], RW_L, default = None 
       Imode Select, 0: Swing Boost Mode; 1: Xtalk Cancellation Mode
     */
    UINT32 imodeenable : 1;
    /* imodeenable - Bits[10:10], RW_L, default = None 
       Enable Imode EQ per byte lane. 
                 0: Imode completely off 
                 1: Imode enabled (Xtalk cancellation OR swing boost selected by 
       ImodeCfg. 
                 If enabled, the bit has to be turned on after ImodeBiasEnable turned 
       on. 
               
     */
    UINT32 imodeeqcode : 4;
    /* imodeeqcode - Bits[14:11], RW_L, default = None 
       Binary Imode Coefficient, sets Imode current strength. 
                 (used for either Xtalk cancellation or swing boost)
                 Code   Imode Current (approx)
                 ------------------
                 0000        0  
                 0001        1
                 0010        2 
                   *  
                   * 
                   *
                 1111        3
     */
    UINT32 imodebiasen : 1;
    /* imodebiasen - Bits[15:15], RW_L, default = None 
       Enables Imode Bias circuit. It must be enabled before ImodeEnable is turned on.
     */
    UINT32 imodescomp : 6;
    /* imodescomp - Bits[21:16], RW_L, default = None 
       Scomp code for Imode circuit
     */
    UINT32 imodescompovrd : 1;
    /* imodescompovrd - Bits[22:22], RW_L, default = None 
       Imode Scomp Override Enable
     */
    UINT32 rxdqssaoffset : 5;
    /* rxdqssaoffset - Bits[27:23], RW_L, default = None 
       strobe Rx Amp offset control
     */
    UINT32 rsvd_28 : 2;
    UINT32 xtalklowpwrenb : 1;
    /* xtalklowpwrenb - Bits[30:30], RW_L, default = 1'b0 
       disables xtalk toggle detect, also used to set narrow operation range for 
       internal  
                 Vref gen circuit. when narrow range operation is needed, it is 
       expected that bios will program 
                 this bit to '1' first and, when Vref is settled, bios program Vref 
       levelshifter hold cr, which 
                 latch the setting. Afterwards, this CR can be used to set xtalk 
       lowpower disable or not. 
               
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DATACONTROL2_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rxstaggerctl : 5;
    /* rxstaggerctl - Bits[4:0], RW_L, default = 5'b0 
       Delay DdrRxEnableQnnnH by RxStaggerCtl qclks (0-31) before turning on/off the 
       RxAmps  
               If RxStaggerCtl = 0, use the ODT Delay/Duration settings for turning 
       on/off the Rx amplifier  
     */
    UINT32 forcebiason : 1;
    /* forcebiason - Bits[5:5], RW_L, default = 1'b0 
       Force on the internal Vref and Rx bias circuit, regardless of any other power 
       downs 
     */
    UINT32 forcerxon : 1;
    /* forcerxon - Bits[6:6], RW_L, default = 1'b0 
       Force on the RxAmp only (as opposed to OdtSampOn, which turns on both ODT and 
       the amplifier). 
               Use for overclocking support where we may not be able to hit the fast 
       exit latency  
     */
    UINT32 rsvd_7 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[8:8], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/VMSE
     */
    UINT32 imodecfg : 1;
    /* imodecfg - Bits[9:9], RW_L, default = None 
       Imode Select, 0: Swing Boost Mode; 1: Xtalk Cancellation Mode
     */
    UINT32 imodeenable : 1;
    /* imodeenable - Bits[10:10], RW_L, default = None 
       Enable Imode EQ per byte lane. 
                 0: Imode completely off 
                 1: Imode enabled (Xtalk cancellation OR swing boost selected by 
       ImodeCfg. 
                 If enabled, the bit has to be turned on after ImodeBiasEnable turned 
       on. 
               
     */
    UINT32 imodeeqcode : 4;
    /* imodeeqcode - Bits[14:11], RW_L, default = None 
       Binary Imode Coefficient, sets Imode current strength. 
                 (used for either Xtalk cancellation or swing boost)
                 Code   Imode Current (approx)
                 ------------------
                 0000        0  
                 0001        1
                 0010        2 
                   *  
                   * 
                   *
                 1111        3
     */
    UINT32 imodebiasen : 1;
    /* imodebiasen - Bits[15:15], RW_L, default = None 
       Enables Imode Bias circuit. It must be enabled before ImodeEnable is turned on.
     */
    UINT32 imodescomp : 6;
    /* imodescomp - Bits[21:16], RW_L, default = None 
       Scomp code for Imode circuit
     */
    UINT32 imodescompovrd : 1;
    /* imodescompovrd - Bits[22:22], RW_L, default = None 
       Imode Scomp Override Enable
     */
    UINT32 rxdqssaoffset : 5;
    /* rxdqssaoffset - Bits[27:23], RW_L, default = None 
       strobe Rx Amp offset control
     */
    UINT32 rsvd_28 : 2;
    UINT32 xtalklowpwrenb : 1;
    /* xtalklowpwrenb - Bits[30:30], RW_L, default = 1'b0 
       disables xtalk toggle detect, also used to set narrow operation range for 
       internal  
                 Vref gen circuit. when narrow range operation is needed, it is 
       expected that bios will program 
                 this bit to '1' first and, when Vref is settled, bios program Vref 
       levelshifter hold cr, which 
                 latch the setting. Afterwards, this CR can be used to set xtalk 
       lowpower disable or not. 
               
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DATACONTROL2_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DATACONTROL3_7_MCIO_DDRIOEXTMCC_REG supported on:                          */
/*       HSX (0x401A72F4)                                                     */
/*       BDX (0x401A72F4)                                                     */
/* Register default value:              0x8184924924F                         */
#define DATACONTROL3_7_MCIO_DDRIOEXTMCC_REG 0x0D0542F4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * static configurations of tx and rx.
 */
typedef union {
  struct {
    UINT32 drvstaticlegcfg : 2;
    /* drvstaticlegcfg - Bits[1:0], RW_L, default = 2'b11 
       binary default setting for static legs in DRV mode
                  0    0       0 legs (EQ disabled)
                  0    1       3 legs (EQ max 3 legs)
                  1    0       6 legs (EQ max 6 legs)
                  1    1      12 legs (EQ max 12 legs)
                 In most cases, we would set this to be 11, but for some configurations 
       that require a higher  
                 Ron value, we need to reduce the number of static legs enabled so that 
       fast corner can hit  
                 the higher Ron target value.
     */
    UINT32 odtstaticlegcfg : 2;
    /* odtstaticlegcfg - Bits[3:2], RW_L, default = 2'b11 
       binary default setting for static legs in ODT mode
                  0    0       0 legs (EQ disabled)
                  0    1       3 legs (EQ max 3 legs)
                  1    0       6 legs (EQ max 6 legs)
                  1    1      12 legs (EQ max 12 legs)
                 In most cases, we would set this to be 11, but for some configurations 
       that require a higher  
                 Ron value, we need to reduce the number of static legs enabled so that 
       fast corner can hit  
                 the higher Ron target value.
     */
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[4:4], RW_L, default = 1'b0 
       level shift hold enable
                            Holds the HV control values and power down the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rsvd_5 : 1;
    UINT32 odtsegmentenable : 3;
    /* odtsegmentenable - Bits[8:6], RW_L, default = 3'b001 
       This will tell which  segments are enabled out of reset.  
                            After that, a shift register will determine the enabled 
       segments 
     */
    UINT32 datasegmentenable : 3;
    /* datasegmentenable - Bits[11:9], RW_L, default = 3'b001 
       This will tell which  segments are enabled out of reset. 
     */
    UINT32 longodtr2w : 1;
    /* longodtr2w - Bits[12:12], RW_L, default = 3'b001 
       When set, it will extends ODT by 1 qclk on read to write turnarounds. NOTE: 
       Cannot be set if ForceODTOn is set or can create glitch in transmitter. 
     */
    UINT32 imodebiashighcm : 1;
    /* imodebiashighcm - Bits[13:13], RW_L, default = 3'b001 
       imodebias control 
     */
    UINT32 imodebiasvrefen : 1;
    /* imodebiasvrefen - Bits[14:14], RW_L, default = 3'b001 
       imodebias control 
     */
    UINT32 imodebiasrxbiastrim : 3;
    /* imodebiasrxbiastrim - Bits[17:15], RW_L, default = 3'b001 
       imodebias control. Also used for rxbiasana 
     */
    UINT32 rsvd_18 : 3;
    UINT32 ddrcrctlecapen : 2;
    /* ddrcrctlecapen - Bits[22:21], RW_L, default = 2'b00 
       ctle capacitor setting 
     */
    UINT32 ddrcrctleresen : 2;
    /* ddrcrctleresen - Bits[24:23], RW_L, default = 2'b11 
       ctle resistor setting 
     */
    UINT32 pcasbiasclosedloopen : 1;
    /* pcasbiasclosedloopen - Bits[25:25], RW_L, default = 2'b00 
       when enabled, choose close loop for the pcas bias loop in Rxbias
     */
    UINT32 rxbiassel : 1;
    /* rxbiassel - Bits[26:26], RW_L, default = 2'b00 
       only used in CTL fub. when enabled, it turns on part of Rxbias for imodebias.
     */
    UINT32 rxbiasfoldedlegtrim : 2;
    /* rxbiasfoldedlegtrim - Bits[28:27], RW_L, default = 2'b00 
       adjust common mode in the first stage.
     */
    UINT32 rxbiasgcncomp : 2;
    /* rxbiasgcncomp - Bits[30:29], RW_L, default = 2'b01 
       used to compensate the GCN skew. The GCN skew information should be
                 extracted by reading and analyzing the final Rcomp code value. The 
       target spec 
                 is to categorize the GCN skew in 3 process condition (mainly slow, 
       fast, typical). 
                 (one example can be just reading the 2 MLS bit of Rcomp code and 
       determine if the 
                 GCN is slow, fast or typical). The gcncomp is used for CTLE.
                 11: gcn in slow corner
                 10: gcn in typical corner
                 01: gcn in fast corner
               
     */
    UINT32 postambleenable : 1;
    /* postambleenable - Bits[31:31], RW_L, default = None 
       VMSE: enables long postamble and a dummy toggle
     */
  } Bits;
  UINT32 Data;
} DATACONTROL3_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* VSSHIORVREFCONTROL_7_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       HSX (0x401A72F8)                                                     */
/*       BDX (0x401A72F8)                                                     */
/* Register default value:              0x004D8238                            */
#define VSSHIORVREFCONTROL_7_MCIO_DDRIOEXTMCC_REG 0x0D0542F8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * VssHi Controls on Channel 0 only. DIMM Vref Controls on  Channel1, databyte 7, 
 * only. 
 */
typedef union {
  struct {
    UINT32 vsshiorvrefctl : 24;
    /* vsshiorvrefctl - Bits[23:0], RW_L, default = 24'h4d8238 
         VssHi adjust setting.
                Field          Bits    Description
               -------         ----    
       ----------------------------------------------------------------------------- 
               SelCode          23     Selects the code to output:  {0: Code, 1: TCode}
               GainBoost        22     Enable gain boost is panic mode by updating 
       code/tcode    
               PanicVoltage  21:18  Unsigned integer controlling the voltage error on 
       VssHi that will trigger the panic driver.  Step size of VccDDQ/192 (0 to 117mV) 
               CloseLoop        17     Enables the VssHi close loop tracking 
               PanicEn          16     Enables the VssHi Panic Driver
               BWError          15:14  Limits the loop gain (ie: difference between 
       code and tcode) to +/- 2^(SlowBWError) 
               OpenLoop         13     Operates VssHi in open loop mode with the target 
       Vref code directly driving the output DAC with no feedback involved 
               SampleDivider    12:10  Controls the loop bandwidth by dividing down the 
       input clock: {0: Qclk, 1: Qclk/2, 2: Qclk/4, 3: Qclk/8, 4: Qclk/16, 5: Qclk/32, 
       6: Qclk/64, 7: Qclk/128} 
               LoBWDivider      9:8    For the slow integrator, selects the net number 
       of samples in a given direction to trip the filter.  {0: 4 samples, 1: 8 
       samples, 2: 16 samples, 3: 32 samples} 
               HiBWDivider      7:6    For the fast integrator, selects the net number 
       of samples in a given direction to trip the filter.  {0: 4 samples, 1: 8 
       samples, 2: 16 samples, 3: 32 samples} 
               Target           5:0    Unsigned integer controlling the target VssHi 
       voltage.  Step size of VccDDQ/192 and starting value of VccDDQ*20/192 
               
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} VSSHIORVREFCONTROL_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* RXGROUP0N1RANK0_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7300)                                                     */
/*       BDX (0x401A7300)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK0_7_MCIO_DDRIOEXTMCC_REG 0x0D054300

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank0
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK0_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK1_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7304)                                                     */
/*       BDX (0x401A7304)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK1_7_MCIO_DDRIOEXTMCC_REG 0x0D054304

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank1
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK1_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK2_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7308)                                                     */
/*       BDX (0x401A7308)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK2_7_MCIO_DDRIOEXTMCC_REG 0x0D054308

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank2
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK2_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK3_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A730C)                                                     */
/*       BDX (0x401A730C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK3_7_MCIO_DDRIOEXTMCC_REG 0x0D05430C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank3
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK3_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK4_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7310)                                                     */
/*       BDX (0x401A7310)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK4_7_MCIO_DDRIOEXTMCC_REG 0x0D054310

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank4
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK4_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK5_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7314)                                                     */
/*       BDX (0x401A7314)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK5_7_MCIO_DDRIOEXTMCC_REG 0x0D054314

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank5
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK5_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK6_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7318)                                                     */
/*       BDX (0x401A7318)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK6_7_MCIO_DDRIOEXTMCC_REG 0x0D054318

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank6
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK6_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK7_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A731C)                                                     */
/*       BDX (0x401A731C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK7_7_MCIO_DDRIOEXTMCC_REG 0x0D05431C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank7
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK7_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK0_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7320)                                                     */
/*       BDX (0x401A7320)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK0_7_MCIO_DDRIOEXTMCC_REG 0x0D054320

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK0_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK1_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7324)                                                     */
/*       BDX (0x401A7324)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK1_7_MCIO_DDRIOEXTMCC_REG 0x0D054324

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK1_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK2_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7328)                                                     */
/*       BDX (0x401A7328)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK2_7_MCIO_DDRIOEXTMCC_REG 0x0D054328

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK2_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK3_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A732C)                                                     */
/*       BDX (0x401A732C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK3_7_MCIO_DDRIOEXTMCC_REG 0x0D05432C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK3_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK4_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7330)                                                     */
/*       BDX (0x401A7330)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK4_7_MCIO_DDRIOEXTMCC_REG 0x0D054330

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK4_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK5_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7334)                                                     */
/*       BDX (0x401A7334)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK5_7_MCIO_DDRIOEXTMCC_REG 0x0D054334

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK5_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK6_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7338)                                                     */
/*       BDX (0x401A7338)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK6_7_MCIO_DDRIOEXTMCC_REG 0x0D054338

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK6_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK7_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A733C)                                                     */
/*       BDX (0x401A733C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK7_7_MCIO_DDRIOEXTMCC_REG 0x0D05433C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK7_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP0N1RANK0_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7340)                                                     */
/*       BDX (0x401A7340)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK0_7_MCIO_DDRIOEXTMCC_REG 0x0D054340
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK0_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK0_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK1_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7344)                                                     */
/*       BDX (0x401A7344)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK1_7_MCIO_DDRIOEXTMCC_REG 0x0D054344
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK1_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK1_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK2_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7348)                                                     */
/*       BDX (0x401A7348)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK2_7_MCIO_DDRIOEXTMCC_REG 0x0D054348
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK2_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK2_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK3_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A734C)                                                     */
/*       BDX (0x401A734C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK3_7_MCIO_DDRIOEXTMCC_REG 0x0D05434C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK3_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK3_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK4_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7350)                                                     */
/*       BDX (0x401A7350)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK4_7_MCIO_DDRIOEXTMCC_REG 0x0D054350
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK4_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK4_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK5_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7354)                                                     */
/*       BDX (0x401A7354)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK5_7_MCIO_DDRIOEXTMCC_REG 0x0D054354
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK5_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK5_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK6_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7358)                                                     */
/*       BDX (0x401A7358)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK6_7_MCIO_DDRIOEXTMCC_REG 0x0D054358
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK6_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK6_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK7_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A735C)                                                     */
/*       BDX (0x401A735C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK7_7_MCIO_DDRIOEXTMCC_REG 0x0D05435C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK7_7_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK7_7_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP1N1RANK0_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7360)                                                     */
/*       BDX (0x401A7360)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK0_7_MCIO_DDRIOEXTMCC_REG 0x0D054360

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK0_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK1_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7364)                                                     */
/*       BDX (0x401A7364)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK1_7_MCIO_DDRIOEXTMCC_REG 0x0D054364

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK1_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK2_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7368)                                                     */
/*       BDX (0x401A7368)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK2_7_MCIO_DDRIOEXTMCC_REG 0x0D054368

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK2_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK3_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A736C)                                                     */
/*       BDX (0x401A736C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK3_7_MCIO_DDRIOEXTMCC_REG 0x0D05436C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK3_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK4_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7370)                                                     */
/*       BDX (0x401A7370)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK4_7_MCIO_DDRIOEXTMCC_REG 0x0D054370

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK4_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK5_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7374)                                                     */
/*       BDX (0x401A7374)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK5_7_MCIO_DDRIOEXTMCC_REG 0x0D054374

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK5_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK6_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7378)                                                     */
/*       BDX (0x401A7378)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK6_7_MCIO_DDRIOEXTMCC_REG 0x0D054378

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK6_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK7_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A737C)                                                     */
/*       BDX (0x401A737C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK7_7_MCIO_DDRIOEXTMCC_REG 0x0D05437C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK7_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK0_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7380)                                                     */
/*       BDX (0x401A7380)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK0_7_MCIO_DDRIOEXTMCC_REG 0x0D054380

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK0_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK1_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7384)                                                     */
/*       BDX (0x401A7384)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK1_7_MCIO_DDRIOEXTMCC_REG 0x0D054384

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK1_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK2_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7388)                                                     */
/*       BDX (0x401A7388)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK2_7_MCIO_DDRIOEXTMCC_REG 0x0D054388

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK2_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK3_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A738C)                                                     */
/*       BDX (0x401A738C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK3_7_MCIO_DDRIOEXTMCC_REG 0x0D05438C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK3_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK4_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7390)                                                     */
/*       BDX (0x401A7390)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK4_7_MCIO_DDRIOEXTMCC_REG 0x0D054390

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK4_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK5_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7394)                                                     */
/*       BDX (0x401A7394)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK5_7_MCIO_DDRIOEXTMCC_REG 0x0D054394

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK5_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK6_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7398)                                                     */
/*       BDX (0x401A7398)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK6_7_MCIO_DDRIOEXTMCC_REG 0x0D054398

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK6_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK7_7_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A739C)                                                     */
/*       BDX (0x401A739C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK7_7_MCIO_DDRIOEXTMCC_REG 0x0D05439C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK7_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* RXVREFCTRLN1_7_MCIO_DDRIOEXTMCC_REG supported on:                          */
/*       HSX (0x401A73A4)                                                     */
/*       BDX (0x401A73A4)                                                     */
/* Register default value:              0x00000000                            */
#define RXVREFCTRLN1_7_MCIO_DDRIOEXTMCC_REG 0x0D0543A4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * RxVrefCtrl
 */
typedef union {
  struct {
    UINT32 vrefperbit0 : 7;
    /* vrefperbit0 - Bits[6:0], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben0 : 1;
    /* halflsben0 - Bits[7:7], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit1 : 7;
    /* vrefperbit1 - Bits[14:8], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben1 : 1;
    /* halflsben1 - Bits[15:15], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit2 : 7;
    /* vrefperbit2 - Bits[22:16], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben2 : 1;
    /* halflsben2 - Bits[23:23], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit3 : 7;
    /* vrefperbit3 - Bits[30:24], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben3 : 1;
    /* halflsben3 - Bits[31:31], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
  } Bits;
  UINT32 Data;
} RXVREFCTRLN1_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATATRAINFEEDBACKN1_7_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A73A8)                                                     */
/*       BDX (0x401A73A8)                                                     */
/* Register default value:              0x00000000                            */
#define DATATRAINFEEDBACKN1_7_MCIO_DDRIOEXTMCC_REG 0x0D0543A8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data Training Feedback Results
 */
typedef union {
  struct {
    UINT32 datatrainfeedback : 9;
    /* datatrainfeedback - Bits[8:0], RW_L, default = 1'b0 
       Data Training Feedback Storage
     */
    UINT32 rsvd_9 : 23;
  } Bits;
  UINT32 Data;
} DATATRAINFEEDBACKN1_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATATCOCOMPOFFSET0_7_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A73AC)                                                     */
/* Register default value:              0x00000000                            */
#define DATATCOCOMPOFFSET0_7_MCIO_DDRIOEXTMCC_REG 0x0D0543AC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 dq0tcocompoffset : 5;
    /* dq0tcocompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment offset added to Tco Comp Value.
     */
    UINT32 dq1tcocompoffset : 5;
    /* dq1tcocompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment offset added to Tco Comp Value.
     */
    UINT32 dq2tcocompoffset : 5;
    /* dq2tcocompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment offset added to DQ2 Tco Comp Value.
     */
    UINT32 dq3tcocompoffset : 5;
    /* dq3tcocompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment offset added to DQ3 Tco Comp Value.
     */
    UINT32 dqs0tcocompoffset : 5;
    /* dqs0tcocompoffset - Bits[24:20], RW_L, default = 4'b0000 
       2s Compliment offset added to DQ4 Tco Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATATCOCOMPOFFSET0_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATATCOCOMPOFFSET1_7_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A73B0)                                                     */
/* Register default value:              0x00000000                            */
#define DATATCOCOMPOFFSET1_7_MCIO_DDRIOEXTMCC_REG 0x0D0543B0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Per Bit Tco Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4tcocompoffset : 5;
    /* dq4tcocompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment offset added to DQ4 Tco Comp Value.
     */
    UINT32 dq5tcocompoffset : 5;
    /* dq5tcocompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment offset added to Dq5 Tco Comp Value.
     */
    UINT32 dq6tcocompoffset : 5;
    /* dq6tcocompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment offset added to Dq6 Tco Comp Value.
     */
    UINT32 dq7tcocompoffset : 5;
    /* dq7tcocompoffset - Bits[19:15], RW_L, default = 1'b0 
       2s Compliment offset added to Dq7 Tco Comp Value.
     */
    UINT32 dqs1tcocompoffset : 5;
    /* dqs1tcocompoffset - Bits[24:20], RW_L, default = 1'b0 
       2s Compliment offset added to Dqs1 Tco Comp Value.
     */
    UINT32 enperbittcocomp : 1;
    /* enperbittcocomp - Bits[25:25], RW_L, default = 1'b0 
       Enable per bit Tco Comp flow.
     */
    UINT32 gentcocompdone : 1;
    /* gentcocompdone - Bits[26:26], RW_L, default = 1'b0 
       Generate TCO Comp done.
     */
    UINT32 spare : 5;
    /* spare - Bits[31:27], RW_L, default = 1'b0 
       Generate TCO Comp done.
     */
  } Bits;
  UINT32 Data;
} DATATCOCOMPOFFSET1_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET0_7_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A73B4)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET0_7_MCIO_DDRIOEXTMCC_REG 0x0D0543B4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Per BIT ODT Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq0odtpupcompoffset : 5;
    /* dq0odtpupcompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq0 Odt Comp Value.
     */
    UINT32 dq1odtpupcompoffset : 5;
    /* dq1odtpupcompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq1 Odt Comp Value.
     */
    UINT32 dq2odtpupcompoffset : 5;
    /* dq2odtpupcompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to DQ2 Odt Comp Value.
     */
    UINT32 dq3odtpupcompoffset : 5;
    /* dq3odtpupcompoffset - Bits[19:15], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQ3 Odt Comp Value.
     */
    UINT32 dqs0odtpupcompoffset : 5;
    /* dqs0odtpupcompoffset - Bits[24:20], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQs0 Odt Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATAODTCOMPOFFSET0_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET1_7_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A73B8)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET1_7_MCIO_DDRIOEXTMCC_REG 0x0D0543B8

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Per Bit Odt Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq0odtpdncompoffset : 5;
    /* dq0odtpdncompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq0 Odt Comp Value.
     */
    UINT32 dq1odtpdncompoffset : 5;
    /* dq1odtpdncompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq1 Odt Comp Value.
     */
    UINT32 dq2odtpdncompoffset : 5;
    /* dq2odtpdncompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to DQ2 Odt Comp Value.
     */
    UINT32 dq3odtpdncompoffset : 5;
    /* dq3odtpdncompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment Pull-down offset added to DQ3 Odt Comp Value.
     */
    UINT32 dqs0odtpdncompoffset : 5;
    /* dqs0odtpdncompoffset - Bits[24:20], RW_L, default = 4'b0000 
       2s Compliment Pull-down offset added to DQS0 Odt Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATAODTCOMPOFFSET1_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET2_7_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A73BC)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET2_7_MCIO_DDRIOEXTMCC_REG 0x0D0543BC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Per Bit Odt Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4odtpupcompoffset : 5;
    /* dq4odtpupcompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq4 Odt Comp Value.
     */
    UINT32 dq5odtpupcompoffset : 5;
    /* dq5odtpupcompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq5 Odt Comp Value.
     */
    UINT32 dq6odtpupcompoffset : 5;
    /* dq6odtpupcompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to DQ6 Odt Comp Value.
     */
    UINT32 dq7odtpupcompoffset : 5;
    /* dq7odtpupcompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment Pull-up offset added to DQ7 Odt Comp Value.
     */
    UINT32 dqs1odtpupcompoffset : 5;
    /* dqs1odtpupcompoffset - Bits[24:20], RW_L, default = 2'b00 
       2s Compliment Pull-up offset added to DQs1 Odt Comp Value.
     */
    UINT32 enperbitodt : 1;
    /* enperbitodt - Bits[25:25], RW_L, default = 1'b0 
       Enable per bit ODT comp.
     */
    UINT32 genodtcompdone : 1;
    /* genodtcompdone - Bits[26:26], RW_L, default = 1'b0 
       Generate ODT comp Done.
     */
    UINT32 spare : 5;
    /* spare - Bits[31:27], RW_L, default = 1'b0 
       spare
     */
  } Bits;
  UINT32 Data;
} DATAODTCOMPOFFSET2_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET3_7_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A73C0)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET3_7_MCIO_DDRIOEXTMCC_REG 0x0D0543C0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Per BIT ODT Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4odtpdncompoffset : 5;
    /* dq4odtpdncompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq4 Odt Comp Value.
     */
    UINT32 dq5odtpdncompoffset : 5;
    /* dq5odtpdncompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq5 Odt Comp Value.
     */
    UINT32 dq6odtpdncompoffset : 5;
    /* dq6odtpdncompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to DQ6 Odt Comp Value.
     */
    UINT32 dq7odtpdncompoffset : 5;
    /* dq7odtpdncompoffset - Bits[19:15], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQ7 Odt Comp Value.
     */
    UINT32 dqs1odtpdncompoffset : 5;
    /* dqs1odtpdncompoffset - Bits[24:20], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQs0 Odt Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATAODTCOMPOFFSET3_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET0_7_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A73C4)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET0_7_MCIO_DDRIOEXTMCC_REG 0x0D0543C4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Per BIT RON Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq0ronpupcompoffset : 5;
    /* dq0ronpupcompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq0 Ron Comp Value.
     */
    UINT32 dq1ronpupcompoffset : 5;
    /* dq1ronpupcompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq1 Ron Comp Value.
     */
    UINT32 dq2ronpupcompoffset : 5;
    /* dq2ronpupcompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to DQ2 Ron Comp Value.
     */
    UINT32 dq3ronpupcompoffset : 5;
    /* dq3ronpupcompoffset - Bits[19:15], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQ3 Ron Comp Value.
     */
    UINT32 dqs0ronpupcompoffset : 5;
    /* dqs0ronpupcompoffset - Bits[24:20], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQs0 Ron Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATARONCOMPOFFSET0_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET1_7_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A73C8)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET1_7_MCIO_DDRIOEXTMCC_REG 0x0D0543C8

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Per Bit RON Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq0ronpdncompoffset : 5;
    /* dq0ronpdncompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq0 Ron Comp Value.
     */
    UINT32 dq1ronpdncompoffset : 5;
    /* dq1ronpdncompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq1 Ron Comp Value.
     */
    UINT32 dq2ronpdncompoffset : 5;
    /* dq2ronpdncompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to DQ2 Ron Comp Value.
     */
    UINT32 dq3ronpdncompoffset : 5;
    /* dq3ronpdncompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment Pull-down offset added to DQ3 Ron Comp Value.
     */
    UINT32 dqs0ronpdncompoffset : 5;
    /* dqs0ronpdncompoffset - Bits[24:20], RW_L, default = 4'b0000 
       2s Compliment Pull-down offset added to DQS0 Ron Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATARONCOMPOFFSET1_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET2_7_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A73CC)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET2_7_MCIO_DDRIOEXTMCC_REG 0x0D0543CC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Per Bit Ron Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4ronpupcompoffset : 5;
    /* dq4ronpupcompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq4 Ron Comp Value.
     */
    UINT32 dq5ronpupcompoffset : 5;
    /* dq5ronpupcompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq5 Ron Comp Value.
     */
    UINT32 dq6ronpupcompoffset : 5;
    /* dq6ronpupcompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to DQ6 Ron Comp Value.
     */
    UINT32 dq7ronpupcompoffset : 5;
    /* dq7ronpupcompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment Pull-up offset added to DQ7 Ron Comp Value.
     */
    UINT32 dqs1ronpupcompoffset : 5;
    /* dqs1ronpupcompoffset - Bits[24:20], RW_L, default = 2'b00 
       2s Compliment Pull-up offset added to DQs1 Ron Comp Value.
     */
    UINT32 enperbitron : 1;
    /* enperbitron - Bits[25:25], RW_L, default = 1'b0 
       Enable per bit RON comp.
     */
    UINT32 spare : 6;
    /* spare - Bits[31:26], RW_L, default = 1'b0 
       spare
     */
  } Bits;
  UINT32 Data;
} DATARONCOMPOFFSET2_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET3_7_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A73D0)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET3_7_MCIO_DDRIOEXTMCC_REG 0x0D0543D0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Per BIT RON Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4ronpdncompoffset : 5;
    /* dq4ronpdncompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq4 Ron Comp Value.
     */
    UINT32 dq5ronpdncompoffset : 5;
    /* dq5ronpdncompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq5 Ron Comp Value.
     */
    UINT32 dq6ronpdncompoffset : 5;
    /* dq6ronpdncompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to DQ6 Ron Comp Value.
     */
    UINT32 dq7ronpdncompoffset : 5;
    /* dq7ronpdncompoffset - Bits[19:15], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQ7 Ron Comp Value.
     */
    UINT32 dqs1ronpdncompoffset : 5;
    /* dqs1ronpdncompoffset - Bits[24:20], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQs1 Ron Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATARONCOMPOFFSET3_7_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* RXGROUP0N0RANK0_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7400)                                                     */
/*       BDX (0x401A7400)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK0_8_MCIO_DDRIOEXTMCC_REG 0x0D054400

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank0
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS (0: 1/16 UI 
       offset,  
                 1: 3/16 UI Offset, 2: 5/16 UI Offsets, 3: 7/16 UI Offsets, 4: 9/16 UI 
       Offsets, 5,6,7: 11/16 UI Offsets.) 
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK0_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK1_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7404)                                                     */
/*       BDX (0x401A7404)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK1_8_MCIO_DDRIOEXTMCC_REG 0x0D054404

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank1
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK1_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK2_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7408)                                                     */
/*       BDX (0x401A7408)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK2_8_MCIO_DDRIOEXTMCC_REG 0x0D054408

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank2
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK2_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK3_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A740C)                                                     */
/*       BDX (0x401A740C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK3_8_MCIO_DDRIOEXTMCC_REG 0x0D05440C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank3
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK3_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK4_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7410)                                                     */
/*       BDX (0x401A7410)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK4_8_MCIO_DDRIOEXTMCC_REG 0x0D054410

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank4
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK4_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK5_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7414)                                                     */
/*       BDX (0x401A7414)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK5_8_MCIO_DDRIOEXTMCC_REG 0x0D054414

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank5
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK5_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK6_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7418)                                                     */
/*       BDX (0x401A7418)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK6_8_MCIO_DDRIOEXTMCC_REG 0x0D054418

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank6
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK6_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK7_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A741C)                                                     */
/*       BDX (0x401A741C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK7_8_MCIO_DDRIOEXTMCC_REG 0x0D05441C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank7
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK7_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK0_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7420)                                                     */
/*       BDX (0x401A7420)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK0_8_MCIO_DDRIOEXTMCC_REG 0x0D054420

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK0_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK1_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7424)                                                     */
/*       BDX (0x401A7424)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK1_8_MCIO_DDRIOEXTMCC_REG 0x0D054424

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK1_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK2_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7428)                                                     */
/*       BDX (0x401A7428)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK2_8_MCIO_DDRIOEXTMCC_REG 0x0D054428

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK2_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK3_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A742C)                                                     */
/*       BDX (0x401A742C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK3_8_MCIO_DDRIOEXTMCC_REG 0x0D05442C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK3_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK4_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7430)                                                     */
/*       BDX (0x401A7430)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK4_8_MCIO_DDRIOEXTMCC_REG 0x0D054430

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK4_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK5_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7434)                                                     */
/*       BDX (0x401A7434)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK5_8_MCIO_DDRIOEXTMCC_REG 0x0D054434

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK5_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK6_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7438)                                                     */
/*       BDX (0x401A7438)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK6_8_MCIO_DDRIOEXTMCC_REG 0x0D054438

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK6_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK7_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A743C)                                                     */
/*       BDX (0x401A743C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK7_8_MCIO_DDRIOEXTMCC_REG 0x0D05443C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK7_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP0N0RANK0_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7440)                                                     */
/*       BDX (0x401A7440)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK0_8_MCIO_DDRIOEXTMCC_REG 0x0D054440
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK0_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK0_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK1_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7444)                                                     */
/*       BDX (0x401A7444)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK1_8_MCIO_DDRIOEXTMCC_REG 0x0D054444
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK1_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK1_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK2_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7448)                                                     */
/*       BDX (0x401A7448)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK2_8_MCIO_DDRIOEXTMCC_REG 0x0D054448
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on. 
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK2_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK2_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK3_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A744C)                                                     */
/*       BDX (0x401A744C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK3_8_MCIO_DDRIOEXTMCC_REG 0x0D05444C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK3_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK3_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK4_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7450)                                                     */
/*       BDX (0x401A7450)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK4_8_MCIO_DDRIOEXTMCC_REG 0x0D054450
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK4_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK4_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK5_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7454)                                                     */
/*       BDX (0x401A7454)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK5_8_MCIO_DDRIOEXTMCC_REG 0x0D054454
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK5_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK5_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK6_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7458)                                                     */
/*       BDX (0x401A7458)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK6_8_MCIO_DDRIOEXTMCC_REG 0x0D054458
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK6_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK6_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK7_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A745C)                                                     */
/*       BDX (0x401A745C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK7_8_MCIO_DDRIOEXTMCC_REG 0x0D05445C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK7_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK7_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP1N0RANK0_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7460)                                                     */
/*       BDX (0x401A7460)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK0_8_MCIO_DDRIOEXTMCC_REG 0x0D054460

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 6'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 6'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 6'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 6'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK0_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK1_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7464)                                                     */
/*       BDX (0x401A7464)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK1_8_MCIO_DDRIOEXTMCC_REG 0x0D054464

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK1_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK2_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7468)                                                     */
/*       BDX (0x401A7468)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK2_8_MCIO_DDRIOEXTMCC_REG 0x0D054468

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK2_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK3_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A746C)                                                     */
/*       BDX (0x401A746C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK3_8_MCIO_DDRIOEXTMCC_REG 0x0D05446C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK3_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK4_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7470)                                                     */
/*       BDX (0x401A7470)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK4_8_MCIO_DDRIOEXTMCC_REG 0x0D054470

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK4_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK5_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7474)                                                     */
/*       BDX (0x401A7474)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK5_8_MCIO_DDRIOEXTMCC_REG 0x0D054474

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK5_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK6_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7478)                                                     */
/*       BDX (0x401A7478)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK6_8_MCIO_DDRIOEXTMCC_REG 0x0D054478

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK6_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK7_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A747C)                                                     */
/*       BDX (0x401A747C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK7_8_MCIO_DDRIOEXTMCC_REG 0x0D05447C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK7_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK0_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7480)                                                     */
/*       BDX (0x401A7480)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK0_8_MCIO_DDRIOEXTMCC_REG 0x0D054480

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK0_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK1_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7484)                                                     */
/*       BDX (0x401A7484)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK1_8_MCIO_DDRIOEXTMCC_REG 0x0D054484

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK1_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK2_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7488)                                                     */
/*       BDX (0x401A7488)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK2_8_MCIO_DDRIOEXTMCC_REG 0x0D054488

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK2_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK3_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A748C)                                                     */
/*       BDX (0x401A748C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK3_8_MCIO_DDRIOEXTMCC_REG 0x0D05448C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK3_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK4_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7490)                                                     */
/*       BDX (0x401A7490)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK4_8_MCIO_DDRIOEXTMCC_REG 0x0D054490

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK4_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK5_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7494)                                                     */
/*       BDX (0x401A7494)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK5_8_MCIO_DDRIOEXTMCC_REG 0x0D054494

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK5_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK6_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7498)                                                     */
/*       BDX (0x401A7498)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK6_8_MCIO_DDRIOEXTMCC_REG 0x0D054498

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK6_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK7_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A749C)                                                     */
/*       BDX (0x401A749C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK7_8_MCIO_DDRIOEXTMCC_REG 0x0D05449C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK7_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* RXVREFCTRLN0_8_MCIO_DDRIOEXTMCC_REG supported on:                          */
/*       HSX (0x401A74A4)                                                     */
/*       BDX (0x401A74A4)                                                     */
/* Register default value:              0x00000000                            */
#define RXVREFCTRLN0_8_MCIO_DDRIOEXTMCC_REG 0x0D0544A4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * RxVrefCtrl
 */
typedef union {
  struct {
    UINT32 vrefperbit0 : 7;
    /* vrefperbit0 - Bits[6:0], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben0 : 1;
    /* halflsben0 - Bits[7:7], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit1 : 7;
    /* vrefperbit1 - Bits[14:8], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben1 : 1;
    /* halflsben1 - Bits[15:15], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit2 : 7;
    /* vrefperbit2 - Bits[22:16], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben2 : 1;
    /* halflsben2 - Bits[23:23], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit3 : 7;
    /* vrefperbit3 - Bits[30:24], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben3 : 1;
    /* halflsben3 - Bits[31:31], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
  } Bits;
  UINT32 Data;
} RXVREFCTRLN0_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATATRAINFEEDBACKN0_8_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A74A8)                                                     */
/*       BDX (0x401A74A8)                                                     */
/* Register default value:              0x00000000                            */
#define DATATRAINFEEDBACKN0_8_MCIO_DDRIOEXTMCC_REG 0x0D0544A8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data Training Feedback Results
 */
typedef union {
  struct {
    UINT32 datatrainfeedback : 18;
    /* datatrainfeedback - Bits[17:0], RW_L, default = 1'b0 
       Data Training Feedback Storage
       
               DataTrainFeedback Description:
                Training Step    Bits    Nibble    Description
               --------------    ----    -------  
       -------------------------------------------------------------------------------------------------------------------------------------------------------- 
               Cross Over Cal     0      0        Tx Dqs Phase Detection
               Cross Over Cal     1      0        Tx Dq Phase Detection
               Cross Over Cal     2      0        Rec En Phase Detection
               RcvEn              8:0    0        Byte detection
               Rx Dq-Dqs          0      0        Pass/fail
               Wr Leveling        8:0    0        Byte detection
               Cross Over Cal     3      1        Tx Dqs Phase Detection
               Cross Over Cal     4      1        Tx Dq Phase Detection
               Cross Over Cal     5      1        Rec En Phase Detection
               RcvEn             17:9    1        Byte detection
               Rx Dq-Dqs          1      1        Pass/fail
               Wr Leveling       17:9    1        Byte detection
               
     */
    UINT32 rsvd_18 : 14;
  } Bits;
  UINT32 Data;
} DATATRAINFEEDBACKN0_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATACONTROL4_8_MCIO_DDRIOEXTMCC_REG supported on:                          */
/*       HSX (0x401A74AC)                                                     */
/*       BDX (0x401A74AC)                                                     */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       <could not determine>                 */
#define DATACONTROL4_8_MCIO_DDRIOEXTMCC_REG 0x0D0544AC
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * static configurations of tx and rx.
 */
typedef union {
  struct {
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[0:0], RW_L, default = None 
       enable grid clock divider in xover. used in native ddr mode for data rate 
       greater than 1867MT/s 
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[1:1], RW_L, default = None 
       enable PiRef clock divider in xover. used in vmse2to2mode, or whenever data rate 
       greater than 1867MT/s 
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[2:2], RW_L, default = None 
       enable extra pipestage in xover. POR setting = 1
     */
    UINT32 vmsehalfwidthmode : 1;
    /* vmsehalfwidthmode - Bits[3:3], RW_L, default = None 
       gate ClkPi when the nibble is disabled in VMSE half-width mode
     */
    UINT32 disablercvenextension : 1;
    /* disablercvenextension - Bits[4:4], RW_L, default = None 
       By default receive enable extension will be bypassed in VMSE2to1 mode. Setting 
       this to 1 in VMSE mode will use DDR to extend the rcven. VMSE ACIO LB use case 
       when only one scheduler is used. 
     */
    UINT32 ddrcrinvertpiclk : 2;
    /* ddrcrinvertpiclk - Bits[6:5], RW_L, default = 2'b00 
       invert ClkPiRef (per nibble) in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 
       phase. 
     */
    UINT32 dqvmselogicdelay : 8;
    /* dqvmselogicdelay - Bits[14:7], RW_L, default = 1'b0 
       DqVmseLogicDelay: If set add one UI delay to the Tx Fifo Read and DQ drivenable
     */
    UINT32 rcvenvmselogicdelay : 2;
    /* rcvenvmselogicdelay - Bits[16:15], RW_L, default = 1'b0 
       Receive enable VmseLogicDelay: If set add one UI delay to the receive enable
     */
    UINT32 dqsvmselogicdelay : 2;
    /* dqsvmselogicdelay - Bits[18:17], RW_L, default = 1'b0 
       DQS VMSE Logic Delay
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[19:19], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 ddrsroslmask : 1;
    /* ddrsroslmask - Bits[20:20], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 dqvmsenibblelogicdelay : 2;
    /* dqvmsenibblelogicdelay - Bits[22:21], RW_L, default = None 
       VMSE Logic delay that is shared for all bits in the nibble
     */
    UINT32 rsvd_23 : 9;
  } Bits;
  UINT32 Data;
} DATACONTROL4_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * static configurations of tx and rx.
 */
typedef union {
  struct {
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[0:0], RW_L, default = None 
       enable grid clock divider in xover. used in native ddr mode for data rate 
       greater than 1867MT/s 
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[1:1], RW_L, default = None 
       enable PiRef clock divider in xover. used in vmse2to2mode, or whenever data rate 
       greater than 1867MT/s 
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[2:2], RW_L, default = None 
       enable extra pipestage in xover. POR setting = 1
     */
    UINT32 vmsehalfwidthmode : 1;
    /* vmsehalfwidthmode - Bits[3:3], RW_L, default = None 
       gate ClkPi when the nibble is disabled in VMSE half-width mode
     */
    UINT32 disablercvenextension : 1;
    /* disablercvenextension - Bits[4:4], RW_L, default = None 
       By default receive enable extension will be bypassed in VMSE2to1 mode. Setting 
       this to 1 in VMSE mode will use DDR to extend the rcven. VMSE ACIO LB use case 
       when only one scheduler is used. 
     */
    UINT32 ddrcrinvertpiclk : 2;
    /* ddrcrinvertpiclk - Bits[6:5], RW_L, default = 2'b00 
       invert ClkPiRef (per nibble) in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 
       phase. 
     */
    UINT32 dqvmselogicdelay : 8;
    /* dqvmselogicdelay - Bits[14:7], RW_L, default = 1'b0 
       DqVmseLogicDelay: If set add one UI delay to the Tx Fifo Read and DQ drivenable
     */
    UINT32 rcvenvmselogicdelay : 2;
    /* rcvenvmselogicdelay - Bits[16:15], RW_L, default = 1'b0 
       Receive enable VmseLogicDelay: If set add one UI delay to the receive enable
     */
    UINT32 dqsvmselogicdelay : 2;
    /* dqsvmselogicdelay - Bits[18:17], RW_L, default = 1'b0 
       DQS VMSE Logic Delay
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[19:19], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 ddrsroslmask : 1;
    /* ddrsroslmask - Bits[20:20], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 dqvmsenibblelogicdelay : 2;
    /* dqvmsenibblelogicdelay - Bits[22:21], RW_L, default = None 
       VMSE Logic delay that is shared for all bits in the nibble
     */
    UINT32 writecrcenable : 1;
    /* writecrcenable - Bits[23:23], RW_L, default = None 
       Enable write crc mode
     */
    UINT32 bdx_sys : 1;
    /* bdx_sys - Bits[24:24], RW_L, default = '1 
       This bit helps to bypass the delaycell in transmitter
     */
    UINT32 rsvd_25 : 6;
    UINT32 bypassinitoptolatchenable : 1;
    /* bypassinitoptolatchenable - Bits[31:31], RW_L, default = None 
       This will ignore an InitOp when the RFFSM is active for a read operation
     */
  } Bits;
  UINT32 Data;
} DATACONTROL4_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* VSSHIORVREFCONTROL1_8_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       BDX (0x401A74CC)                                                     */
/* Register default value:              0x00000000                            */
#define VSSHIORVREFCONTROL1_8_MCIO_DDRIOEXTMCC_REG 0x0D0544CC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * VssHi Controls on Channel 0 only. DIMM Vref Controls on  Channel1, databyte 7, 
 * only. Extension for BDX. 
 */
typedef union {
  struct {
    UINT32 vsshiorvrefctl : 8;
    /* vsshiorvrefctl - Bits[7:0], RW_L, default = 8'b0 
        BDX-specific extended control signal. 
     */
    UINT32 spare : 24;
    /* spare - Bits[31:8], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} VSSHIORVREFCONTROL1_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* PXCATTACK1SEL_8_MCIO_DDRIOEXTMCC_REG supported on:                         */
/*       BDX (0x401A74D0)                                                     */
/* Register default value:              0x00FAC688                            */
#define PXCATTACK1SEL_8_MCIO_DDRIOEXTMCC_REG 0x0D0544D0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Select 2 aggressors out 3 neighbors in a nibble
 */
typedef union {
  struct {
    UINT32 attackr1d0 : 3;
    /* attackr1d0 - Bits[2:0], RW_L, default = 3'd0 
       xtalk aggressor1 select for dq bit[0] in a byte
     */
    UINT32 attackr1d1 : 3;
    /* attackr1d1 - Bits[5:3], RW_L, default = 3'd1 
       xtalk aggressor1 select for dq bit[1] in a byte
     */
    UINT32 attackr1d2 : 3;
    /* attackr1d2 - Bits[8:6], RW_L, default = 3'd2 
       xtalk aggressor1 select for dq bit[2] in a byte
     */
    UINT32 attackr1d3 : 3;
    /* attackr1d3 - Bits[11:9], RW_L, default = 3'd3 
       xtalk aggressor1 select for dq bit[3] in a byte
     */
    UINT32 attackr1d4 : 3;
    /* attackr1d4 - Bits[14:12], RW_L, default = 3'd4 
       xtalk aggressor1 select for dq bit[4] in a byte
     */
    UINT32 attackr1d5 : 3;
    /* attackr1d5 - Bits[17:15], RW_L, default = 3'd5 
       xtalk aggressor1 select for dq bit[5] in a byte
     */
    UINT32 attackr1d6 : 3;
    /* attackr1d6 - Bits[20:18], RW_L, default = 3'd6 
       xtalk aggressor1 select for dq bit[6] in a byte
     */
    UINT32 attackr1d7 : 3;
    /* attackr1d7 - Bits[23:21], RW_L, default = 3'd7 
       xtalk aggressor1 select for dq bit[7] in a byte
     */
    UINT32 spare : 8;
    /* spare - Bits[31:24], RW_L, default = 8'b0 
       spare bits.
     */
  } Bits;
  UINT32 Data;
} PXCATTACK1SEL_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */






/* DATAOFFSETTRAIN_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A74DC)                                                     */
/*       BDX (0x401A74DC)                                                     */
/* Register default value:              0x00000000                            */
#define DATAOFFSETTRAIN_8_MCIO_DDRIOEXTMCC_REG 0x0D0544DC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rcvenoffset : 6;
    /* rcvenoffset - Bits[5:0], RW_L, default = 1'b0 
       Offset added to Trained RxRcvEn Value.
     */
    UINT32 rxdqsoffset : 7;
    /* rxdqsoffset - Bits[12:6], RW_L, default = 1'b0 
       Offset added to Trained Rx DQS Value.
     */
    UINT32 txdqoffset : 6;
    /* txdqoffset - Bits[18:13], RW_L, default = 2'b00 
       Offset added to Trained Tx DQ Value.
     */
    UINT32 txdqsoffset : 6;
    /* txdqsoffset - Bits[24:19], RW_L, default = 6'b000000 
       Offset added to Trained Tx DQS Value.
     */
    UINT32 vrefoffset : 7;
    /* vrefoffset - Bits[31:25], RW_L, default = 7'b0000000 
       unsiged offset added to Trained Vref Value. each step is Vdd/192.
     */
  } Bits;
  UINT32 Data;
} DATAOFFSETTRAIN_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* PXCATTACK0SEL_8_MCIO_DDRIOEXTMCC_REG supported on:                         */
/*       BDX (0x401A74E4)                                                     */
/* Register default value:              0x00FAC688                            */
#define PXCATTACK0SEL_8_MCIO_DDRIOEXTMCC_REG 0x0D0544E4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Select 2 aggressors out 3 neighbors in a nibble
 */
typedef union {
  struct {
    UINT32 attackr0d0 : 3;
    /* attackr0d0 - Bits[2:0], RW_L, default = 3'd0 
       xtalk aggressor0 select for dq bit[0] in a byte
     */
    UINT32 attackr0d1 : 3;
    /* attackr0d1 - Bits[5:3], RW_L, default = 3'd1 
       xtalk aggressor0 select for dq bit[1] in a byte
     */
    UINT32 attackr0d2 : 3;
    /* attackr0d2 - Bits[8:6], RW_L, default = 3'd2 
       xtalk aggressor0 select for dq bit[2] in a byte
     */
    UINT32 attackr0d3 : 3;
    /* attackr0d3 - Bits[11:9], RW_L, default = 3'd3 
       xtalk aggressor0 select for dq bit[3] in a byte
     */
    UINT32 attackr0d4 : 3;
    /* attackr0d4 - Bits[14:12], RW_L, default = 3'd4 
       xtalk aggressor0 select for dq bit[4] in a byte
     */
    UINT32 attackr0d5 : 3;
    /* attackr0d5 - Bits[17:15], RW_L, default = 3'd5 
       xtalk aggressor0 select for dq bit[5] in a byte
     */
    UINT32 attackr0d6 : 3;
    /* attackr0d6 - Bits[20:18], RW_L, default = 3'd6 
       xtalk aggressor0 select for dq bit[6] in a byte
     */
    UINT32 attackr0d7 : 3;
    /* attackr0d7 - Bits[23:21], RW_L, default = 3'd7 
       xtalk aggressor0 select for dq bit[7] in a byte
     */
    UINT32 spare : 8;
    /* spare - Bits[31:24], RW_L, default = 8'b0 
       spare bits.
     */
  } Bits;
  UINT32 Data;
} PXCATTACK0SEL_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATACONTROL0_8_MCIO_DDRIOEXTMCC_REG supported on:                          */
/*       HSX (0x401A74E8)                                                     */
/*       BDX (0x401A74E8)                                                     */
/* Register default value:              0x00000000                            */
#define DATACONTROL0_8_MCIO_DDRIOEXTMCC_REG 0x0D0544E8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rxtrainingmode : 1;
    /* rxtrainingmode - Bits[0:0], RW_L, default = 1'b0 
       Enables Read DqDqs Training Mode
     */
    UINT32 wltrainingmode : 1;
    /* wltrainingmode - Bits[1:1], RW_L, default = 1'b0 
       Enables Write Leveling Training Mode
     */
    UINT32 rltrainingmode : 1;
    /* rltrainingmode - Bits[2:2], RW_L, default = 1'b0 
       Enables Read Leveling Training Mode.
     */
    UINT32 senseamptrainingmode : 1;
    /* senseamptrainingmode - Bits[3:3], RW_L, default = 1'b0 
       Enables SenseAmp offset cancellation training mode.
     */
    UINT32 rsvd_4 : 1;
    UINT32 rfon : 1;
    /* rfon - Bits[5:5], RW_L, default = 1'b0 
       Power-down Disable:  Forces register file read, overriding the rank power down 
       logic. 
     */
    UINT32 rsvd_6 : 1;
    UINT32 txpion : 1;
    /* txpion - Bits[7:7], RW_L, default = 1'b0 
       Power-down Disable:  Forces TxDq and TxDqs PI clocks on.
     */
    UINT32 rsvd_8 : 1;
    UINT32 dcdetectmode : 1;
    /* dcdetectmode - Bits[9:9], RW_L, default = 1'b0 
       Enable VMSE DC detect mode.
     */
    UINT32 rsvd_10 : 2;
    UINT32 txlong : 1;
    /* txlong - Bits[12:12], RW_L, default = 1'b0 
       Sets the duration of the first transmitted DQ bit of the burst. 0=1UI, 1=2UI.
     */
    UINT32 rsvd_13 : 5;
    UINT32 driversegmentenable : 1;
    /* driversegmentenable - Bits[18:18], RW_L, default = 1'b0 
       Controls which segements of the driver are enabled:  {0: All, 1: Only ODT}.
     */
    UINT32 rsvd_19 : 1;
    UINT32 readrfrd : 1;
    /* readrfrd - Bits[20:20], RW_L, default = 1'b0 
       Self clearing command bit. When set, it will download the three 32 bit lines 
       from the RegFile associated with a read c 
       ommand to Rank = ReadRFRank
     */
    UINT32 readrfwr : 1;
    /* readrfwr - Bits[21:21], RW_L, default = 1'b0 
       Self clearing command bit. When set, it will download the three 32 bit lines 
       from the RegFile associated with a write  
       command to Rank = ReadRFRank
     */
    UINT32 readrfrank : 3;
    /* readrfrank - Bits[24:22], RW_L, default = 3'b000 
       Specifies the rank that ReadRFRd or ReadRFWr will manually download the CR 
       values from 
     */
    UINT32 forceodton : 1;
    /* forceodton - Bits[25:25], RW_L, default = 1'b0 
       ODT is forced-on. NOTE: Cannot be set if LongODTR2W is set or can create glitch 
       in transmitter.   NOTE: If forceodton = 1, odtduration should be set to 0 
     */
    UINT32 rsvd_26 : 3;
    UINT32 burstlength4 : 1;
    /* burstlength4 - Bits[29:29], RW_L, default = 1'b0 
       enable lockstep mode, burst length 4
     */
    UINT32 xovercal : 1;
    /* xovercal - Bits[30:30], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
    UINT32 longpreambleenable : 1;
    /* longpreambleenable - Bits[31:31], RW_L, default = 1'b0 
       Enables long preamble for DDR4 devices
     */
  } Bits;
  UINT32 Data;
} DATACONTROL0_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATACONTROL1_8_MCIO_DDRIOEXTMCC_REG supported on:                          */
/*       HSX (0x401A74EC)                                                     */
/*       BDX (0x401A74EC)                                                     */
/* Register default value:              0x00000000                            */
#define DATACONTROL1_8_MCIO_DDRIOEXTMCC_REG 0x0D0544EC
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 7;
    UINT32 sdlldisable : 2;
    /* sdlldisable - Bits[8:7], RW_L, default = 2'b00 
       Controls which slave DLL are disabled: used in VMSE half-width mode to turn off 
       the unused slave delay line. 
                 Set to 0 to have nothing disabled.
                 Set to 1 to have nibble0 disabled.
                 Set to 2 to have nibble1 disabled.
                 Set to 3 to have nibble0 and nibble 1 disabled.
               
     */
    UINT32 rsvd_9 : 1;
    UINT32 rxbiasctl : 3;
    /* rxbiasctl - Bits[12:10], RW_L, default = 1'b0 
       Controls the Current and BW of the Receiver: 
                 {0: 0.34Idll, 1: 0.66Idll, 2: 0.88Idll, 3: Idll, 4: 1.33Idll, 5: 
       1.66Idll, 6: 2.00Idll, 7: 2.33Idll} 
     */
    UINT32 odtdelay : 4;
    /* odtdelay - Bits[16:13], RW_L, default = 4'b0000 
       Controls ODT turn-on delay.   To turn on N tQCK before RcvEn, program to 
       RcvEn[8:6]  N + 4. 
     */
    UINT32 odtduration : 3;
    /* odtduration - Bits[19:17], RW_L, default = 4'b0000 
       Controls the ODT ON duration from (Code, Duration) = (0x0, 11 tQCK) to (0x7, 18 
       tQCK).  NOTE: If forceodton = 1, this should be set to 0 
     */
    UINT32 senseampdelay : 4;
    /* senseampdelay - Bits[23:20], RW_L, default = 4'b0000 
       Controls Senseamp turn-on delay.   To turn on N tQCK before RcvEn, program to 
       RcvEn[8:6]  N + 4. 
     */
    UINT32 senseampduration : 3;
    /* senseampduration - Bits[26:24], RW_L, default = 4'b0000 
       Controls the SenseAmp ON duration from (Code, Duration) = (0x0, 11 tQCK) to 
       (0x7, 18 tQCK) 
     */
    UINT32 burstendodtdelay : 3;
    /* burstendodtdelay - Bits[29:27], RW_L, default = 3'b000 
       Cycles after the burst, when the current pulse should turn on by enabling On-die 
       Terminators 
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} DATACONTROL1_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 7;
    UINT32 sdlldisable : 2;
    /* sdlldisable - Bits[8:7], RW_L, default = 2'b00 
       Controls which slave DLL are disabled: used in vmse half-width mode to turn off 
       the unused slave delay line. 
                 Set to 0 to have nothing disabled.
                 Set to 1 to have nibble0 disabled.
                 Set to 2 to have nibble1 disabled.
                 Set to 3 to have nibble0 and nibble 1 disabled.
               
     */
    UINT32 rsvd_9 : 1;
    UINT32 rxbiasctl : 3;
    /* rxbiasctl - Bits[12:10], RW_L, default = 1'b0 
       Controls the Current and BW of the Receiver: 
                 {0: 0.34Idll, 1: 0.66Idll, 2: 0.88Idll, 3: Idll, 4: 1.33Idll, 5: 
       1.66Idll, 6: 2.00Idll, 7: 2.33Idll} 
     */
    UINT32 odtdelay : 4;
    /* odtdelay - Bits[16:13], RW_L, default = 4'b0000 
       Controls ODT turn-on delay.   To turn on N tQCK before RcvEn, program to 
       RcvEn[8:6]  N + 4. 
     */
    UINT32 odtduration : 3;
    /* odtduration - Bits[19:17], RW_L, default = 4'b0000 
       Controls the ODT ON duration from (Code, Duration) = (0x0, 11 tQCK) to (0x7, 18 
       tQCK).  NOTE: If forceodton = 1, this should be set to 0 
     */
    UINT32 senseampdelay : 4;
    /* senseampdelay - Bits[23:20], RW_L, default = 4'b0000 
       Controls Senseamp turn-on delay.   To turn on N tQCK before RcvEn, program to 
       RcvEn[8:6]  N + 4. 
     */
    UINT32 senseampduration : 3;
    /* senseampduration - Bits[26:24], RW_L, default = 4'b0000 
       Controls the SenseAmp ON duration from (Code, Duration) = (0x0, 11 tQCK) to 
       (0x7, 18 tQCK) 
     */
    UINT32 burstendodtdelay : 3;
    /* burstendodtdelay - Bits[29:27], RW_L, default = 3'b000 
       Cycles after the burst, when the current pulse should turn on by enabling On-die 
       Terminators 
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} DATACONTROL1_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DATACONTROL2_8_MCIO_DDRIOEXTMCC_REG supported on:                          */
/*       HSX (0x401A74F0)                                                     */
/*       BDX (0x401A74F0)                                                     */
/* Register default value:              0x00000000                            */
#define DATACONTROL2_8_MCIO_DDRIOEXTMCC_REG 0x0D0544F0
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rxstaggerctl : 5;
    /* rxstaggerctl - Bits[4:0], RW_L, default = 5'b0 
       Delay DdrRxEnableQnnnH by RxStaggerCtl qclks (0-31) before turning on/off the 
       RxAmps  
               If RxStaggerCtl = 0, use the ODT Delay/Duration settings for turning 
       on/off the Rx amplifier  
     */
    UINT32 forcebiason : 1;
    /* forcebiason - Bits[5:5], RW_L, default = 1'b0 
       Force on the internal Vref and Rx bias circuit, regardless of any other power 
       downs 
     */
    UINT32 forcerxon : 1;
    /* forcerxon - Bits[6:6], RW_L, default = 1'b0 
       Force on the RxAmp only (as opposed to OdtSampOn, which turns on both ODT and 
       the amplifier). 
               Use for overclocking support where we may not be able to hit the fast 
       exit latency  
     */
    UINT32 rsvd_7 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[8:8], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/VMSE
     */
    UINT32 imodecfg : 1;
    /* imodecfg - Bits[9:9], RW_L, default = None 
       Imode Select, 0: Swing Boost Mode; 1: Xtalk Cancellation Mode
     */
    UINT32 imodeenable : 1;
    /* imodeenable - Bits[10:10], RW_L, default = None 
       Enable Imode EQ per byte lane. 
                 0: Imode completely off 
                 1: Imode enabled (Xtalk cancellation OR swing boost selected by 
       ImodeCfg. 
                 If enabled, the bit has to be turned on after ImodeBiasEnable turned 
       on. 
               
     */
    UINT32 imodeeqcode : 4;
    /* imodeeqcode - Bits[14:11], RW_L, default = None 
       Binary Imode Coefficient, sets Imode current strength. 
                 (used for either Xtalk cancellation or swing boost)
                 Code   Imode Current (approx)
                 ------------------
                 0000        0  
                 0001        1
                 0010        2 
                   *  
                   * 
                   *
                 1111        3
     */
    UINT32 imodebiasen : 1;
    /* imodebiasen - Bits[15:15], RW_L, default = None 
       Enables Imode Bias circuit. It must be enabled before ImodeEnable is turned on.
     */
    UINT32 imodescomp : 6;
    /* imodescomp - Bits[21:16], RW_L, default = None 
       Scomp code for Imode circuit
     */
    UINT32 imodescompovrd : 1;
    /* imodescompovrd - Bits[22:22], RW_L, default = None 
       Imode Scomp Override Enable
     */
    UINT32 rxdqssaoffset : 5;
    /* rxdqssaoffset - Bits[27:23], RW_L, default = None 
       strobe Rx Amp offset control
     */
    UINT32 rsvd_28 : 2;
    UINT32 xtalklowpwrenb : 1;
    /* xtalklowpwrenb - Bits[30:30], RW_L, default = 1'b0 
       disables xtalk toggle detect, also used to set narrow operation range for 
       internal  
                 Vref gen circuit. when narrow range operation is needed, it is 
       expected that bios will program 
                 this bit to '1' first and, when Vref is settled, bios program Vref 
       levelshifter hold cr, which 
                 latch the setting. Afterwards, this CR can be used to set xtalk 
       lowpower disable or not. 
               
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DATACONTROL2_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rxstaggerctl : 5;
    /* rxstaggerctl - Bits[4:0], RW_L, default = 5'b0 
       Delay DdrRxEnableQnnnH by RxStaggerCtl qclks (0-31) before turning on/off the 
       RxAmps  
               If RxStaggerCtl = 0, use the ODT Delay/Duration settings for turning 
       on/off the Rx amplifier  
     */
    UINT32 forcebiason : 1;
    /* forcebiason - Bits[5:5], RW_L, default = 1'b0 
       Force on the internal Vref and Rx bias circuit, regardless of any other power 
       downs 
     */
    UINT32 forcerxon : 1;
    /* forcerxon - Bits[6:6], RW_L, default = 1'b0 
       Force on the RxAmp only (as opposed to OdtSampOn, which turns on both ODT and 
       the amplifier). 
               Use for overclocking support where we may not be able to hit the fast 
       exit latency  
     */
    UINT32 rsvd_7 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[8:8], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/VMSE
     */
    UINT32 imodecfg : 1;
    /* imodecfg - Bits[9:9], RW_L, default = None 
       Imode Select, 0: Swing Boost Mode; 1: Xtalk Cancellation Mode
     */
    UINT32 imodeenable : 1;
    /* imodeenable - Bits[10:10], RW_L, default = None 
       Enable Imode EQ per byte lane. 
                 0: Imode completely off 
                 1: Imode enabled (Xtalk cancellation OR swing boost selected by 
       ImodeCfg. 
                 If enabled, the bit has to be turned on after ImodeBiasEnable turned 
       on. 
               
     */
    UINT32 imodeeqcode : 4;
    /* imodeeqcode - Bits[14:11], RW_L, default = None 
       Binary Imode Coefficient, sets Imode current strength. 
                 (used for either Xtalk cancellation or swing boost)
                 Code   Imode Current (approx)
                 ------------------
                 0000        0  
                 0001        1
                 0010        2 
                   *  
                   * 
                   *
                 1111        3
     */
    UINT32 imodebiasen : 1;
    /* imodebiasen - Bits[15:15], RW_L, default = None 
       Enables Imode Bias circuit. It must be enabled before ImodeEnable is turned on.
     */
    UINT32 imodescomp : 6;
    /* imodescomp - Bits[21:16], RW_L, default = None 
       Scomp code for Imode circuit
     */
    UINT32 imodescompovrd : 1;
    /* imodescompovrd - Bits[22:22], RW_L, default = None 
       Imode Scomp Override Enable
     */
    UINT32 rxdqssaoffset : 5;
    /* rxdqssaoffset - Bits[27:23], RW_L, default = None 
       strobe Rx Amp offset control
     */
    UINT32 rsvd_28 : 2;
    UINT32 xtalklowpwrenb : 1;
    /* xtalklowpwrenb - Bits[30:30], RW_L, default = 1'b0 
       disables xtalk toggle detect, also used to set narrow operation range for 
       internal  
                 Vref gen circuit. when narrow range operation is needed, it is 
       expected that bios will program 
                 this bit to '1' first and, when Vref is settled, bios program Vref 
       levelshifter hold cr, which 
                 latch the setting. Afterwards, this CR can be used to set xtalk 
       lowpower disable or not. 
               
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DATACONTROL2_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DATACONTROL3_8_MCIO_DDRIOEXTMCC_REG supported on:                          */
/*       HSX (0x401A74F4)                                                     */
/*       BDX (0x401A74F4)                                                     */
/* Register default value:              0x8184924924F                         */
#define DATACONTROL3_8_MCIO_DDRIOEXTMCC_REG 0x0D0544F4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * static configurations of tx and rx.
 */
typedef union {
  struct {
    UINT32 drvstaticlegcfg : 2;
    /* drvstaticlegcfg - Bits[1:0], RW_L, default = 2'b11 
       binary default setting for static legs in DRV mode
                  0    0       0 legs (EQ disabled)
                  0    1       3 legs (EQ max 3 legs)
                  1    0       6 legs (EQ max 6 legs)
                  1    1      12 legs (EQ max 12 legs)
                 In most cases, we would set this to be 11, but for some configurations 
       that require a higher  
                 Ron value, we need to reduce the number of static legs enabled so that 
       fast corner can hit  
                 the higher Ron target value.
     */
    UINT32 odtstaticlegcfg : 2;
    /* odtstaticlegcfg - Bits[3:2], RW_L, default = 2'b11 
       binary default setting for static legs in ODT mode
                  0    0       0 legs (EQ disabled)
                  0    1       3 legs (EQ max 3 legs)
                  1    0       6 legs (EQ max 6 legs)
                  1    1      12 legs (EQ max 12 legs)
                 In most cases, we would set this to be 11, but for some configurations 
       that require a higher  
                 Ron value, we need to reduce the number of static legs enabled so that 
       fast corner can hit  
                 the higher Ron target value.
     */
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[4:4], RW_L, default = 1'b0 
       level shift hold enable
                            Holds the HV control values and power down the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rsvd_5 : 1;
    UINT32 odtsegmentenable : 3;
    /* odtsegmentenable - Bits[8:6], RW_L, default = 3'b001 
       This will tell which  segments are enabled out of reset.  
                            After that, a shift register will determine the enabled 
       segments 
     */
    UINT32 datasegmentenable : 3;
    /* datasegmentenable - Bits[11:9], RW_L, default = 3'b001 
       This will tell which  segments are enabled out of reset. 
     */
    UINT32 longodtr2w : 1;
    /* longodtr2w - Bits[12:12], RW_L, default = 3'b001 
       When set, it will extends ODT by 1 qclk on read to write turnarounds. NOTE: 
       Cannot be set if ForceODTOn is set or can create glitch in transmitter. 
     */
    UINT32 imodebiashighcm : 1;
    /* imodebiashighcm - Bits[13:13], RW_L, default = 3'b001 
       imodebias control 
     */
    UINT32 imodebiasvrefen : 1;
    /* imodebiasvrefen - Bits[14:14], RW_L, default = 3'b001 
       imodebias control 
     */
    UINT32 imodebiasrxbiastrim : 3;
    /* imodebiasrxbiastrim - Bits[17:15], RW_L, default = 3'b001 
       imodebias control. Also used for rxbiasana 
     */
    UINT32 rsvd_18 : 3;
    UINT32 ddrcrctlecapen : 2;
    /* ddrcrctlecapen - Bits[22:21], RW_L, default = 2'b00 
       ctle capacitor setting 
     */
    UINT32 ddrcrctleresen : 2;
    /* ddrcrctleresen - Bits[24:23], RW_L, default = 2'b11 
       ctle resistor setting 
     */
    UINT32 pcasbiasclosedloopen : 1;
    /* pcasbiasclosedloopen - Bits[25:25], RW_L, default = 2'b00 
       when enabled, choose close loop for the pcas bias loop in Rxbias
     */
    UINT32 rxbiassel : 1;
    /* rxbiassel - Bits[26:26], RW_L, default = 2'b00 
       only used in CTL fub. when enabled, it turns on part of Rxbias for imodebias.
     */
    UINT32 rxbiasfoldedlegtrim : 2;
    /* rxbiasfoldedlegtrim - Bits[28:27], RW_L, default = 2'b00 
       adjust common mode in the first stage.
     */
    UINT32 rxbiasgcncomp : 2;
    /* rxbiasgcncomp - Bits[30:29], RW_L, default = 2'b01 
       used to compensate the GCN skew. The GCN skew information should be
                 extracted by reading and analyzing the final Rcomp code value. The 
       target spec 
                 is to categorize the GCN skew in 3 process condition (mainly slow, 
       fast, typical). 
                 (one example can be just reading the 2 MLS bit of Rcomp code and 
       determine if the 
                 GCN is slow, fast or typical). The gcncomp is used for CTLE.
                 11: gcn in slow corner
                 10: gcn in typical corner
                 01: gcn in fast corner
               
     */
    UINT32 postambleenable : 1;
    /* postambleenable - Bits[31:31], RW_L, default = None 
       VMSE: enables long postamble and a dummy toggle
     */
  } Bits;
  UINT32 Data;
} DATACONTROL3_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* VSSHIORVREFCONTROL_8_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       HSX (0x401A74F8)                                                     */
/*       BDX (0x401A74F8)                                                     */
/* Register default value:              0x004D8238                            */
#define VSSHIORVREFCONTROL_8_MCIO_DDRIOEXTMCC_REG 0x0D0544F8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * VssHi Controls on Channel 0 only. DIMM Vref Controls on  Channel1, databyte 7, 
 * only. 
 */
typedef union {
  struct {
    UINT32 vsshiorvrefctl : 24;
    /* vsshiorvrefctl - Bits[23:0], RW_L, default = 24'h4d8238 
         VssHi adjust setting.
                Field          Bits    Description
               -------         ----    
       ----------------------------------------------------------------------------- 
               SelCode          23     Selects the code to output:  {0: Code, 1: TCode}
               GainBoost        22     Enable gain boost is panic mode by updating 
       code/tcode    
               PanicVoltage  21:18  Unsigned integer controlling the voltage error on 
       VssHi that will trigger the panic driver.  Step size of VccDDQ/192 (0 to 117mV) 
               CloseLoop        17     Enables the VssHi close loop tracking 
               PanicEn          16     Enables the VssHi Panic Driver
               BWError          15:14  Limits the loop gain (ie: difference between 
       code and tcode) to +/- 2^(SlowBWError) 
               OpenLoop         13     Operates VssHi in open loop mode with the target 
       Vref code directly driving the output DAC with no feedback involved 
               SampleDivider    12:10  Controls the loop bandwidth by dividing down the 
       input clock: {0: Qclk, 1: Qclk/2, 2: Qclk/4, 3: Qclk/8, 4: Qclk/16, 5: Qclk/32, 
       6: Qclk/64, 7: Qclk/128} 
               LoBWDivider      9:8    For the slow integrator, selects the net number 
       of samples in a given direction to trip the filter.  {0: 4 samples, 1: 8 
       samples, 2: 16 samples, 3: 32 samples} 
               HiBWDivider      7:6    For the fast integrator, selects the net number 
       of samples in a given direction to trip the filter.  {0: 4 samples, 1: 8 
       samples, 2: 16 samples, 3: 32 samples} 
               Target           5:0    Unsigned integer controlling the target VssHi 
       voltage.  Step size of VccDDQ/192 and starting value of VccDDQ*20/192 
               
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} VSSHIORVREFCONTROL_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* RXGROUP0N1RANK0_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7500)                                                     */
/*       BDX (0x401A7500)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK0_8_MCIO_DDRIOEXTMCC_REG 0x0D054500

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank0
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK0_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK1_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7504)                                                     */
/*       BDX (0x401A7504)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK1_8_MCIO_DDRIOEXTMCC_REG 0x0D054504

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank1
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK1_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK2_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7508)                                                     */
/*       BDX (0x401A7508)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK2_8_MCIO_DDRIOEXTMCC_REG 0x0D054508

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank2
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK2_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK3_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A750C)                                                     */
/*       BDX (0x401A750C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK3_8_MCIO_DDRIOEXTMCC_REG 0x0D05450C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank3
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK3_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK4_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7510)                                                     */
/*       BDX (0x401A7510)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK4_8_MCIO_DDRIOEXTMCC_REG 0x0D054510

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank4
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK4_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK5_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7514)                                                     */
/*       BDX (0x401A7514)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK5_8_MCIO_DDRIOEXTMCC_REG 0x0D054514

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank5
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK5_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK6_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7518)                                                     */
/*       BDX (0x401A7518)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK6_8_MCIO_DDRIOEXTMCC_REG 0x0D054518

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank6
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK6_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK7_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A751C)                                                     */
/*       BDX (0x401A751C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK7_8_MCIO_DDRIOEXTMCC_REG 0x0D05451C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data RX training values for Rank7
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK7_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK0_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7520)                                                     */
/*       BDX (0x401A7520)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK0_8_MCIO_DDRIOEXTMCC_REG 0x0D054520

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK0_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK1_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7524)                                                     */
/*       BDX (0x401A7524)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK1_8_MCIO_DDRIOEXTMCC_REG 0x0D054524

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK1_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK2_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7528)                                                     */
/*       BDX (0x401A7528)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK2_8_MCIO_DDRIOEXTMCC_REG 0x0D054528

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK2_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK3_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A752C)                                                     */
/*       BDX (0x401A752C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK3_8_MCIO_DDRIOEXTMCC_REG 0x0D05452C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK3_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK4_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7530)                                                     */
/*       BDX (0x401A7530)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK4_8_MCIO_DDRIOEXTMCC_REG 0x0D054530

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK4_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK5_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7534)                                                     */
/*       BDX (0x401A7534)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK5_8_MCIO_DDRIOEXTMCC_REG 0x0D054534

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK5_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK6_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7538)                                                     */
/*       BDX (0x401A7538)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK6_8_MCIO_DDRIOEXTMCC_REG 0x0D054538

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK6_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK7_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A753C)                                                     */
/*       BDX (0x401A753C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK7_8_MCIO_DDRIOEXTMCC_REG 0x0D05453C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK7_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP0N1RANK0_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7540)                                                     */
/*       BDX (0x401A7540)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK0_8_MCIO_DDRIOEXTMCC_REG 0x0D054540
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK0_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK0_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK1_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7544)                                                     */
/*       BDX (0x401A7544)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK1_8_MCIO_DDRIOEXTMCC_REG 0x0D054544
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK1_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK1_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK2_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7548)                                                     */
/*       BDX (0x401A7548)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK2_8_MCIO_DDRIOEXTMCC_REG 0x0D054548
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK2_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK2_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK3_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A754C)                                                     */
/*       BDX (0x401A754C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK3_8_MCIO_DDRIOEXTMCC_REG 0x0D05454C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK3_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK3_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK4_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7550)                                                     */
/*       BDX (0x401A7550)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK4_8_MCIO_DDRIOEXTMCC_REG 0x0D054550
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK4_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK4_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK5_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7554)                                                     */
/*       BDX (0x401A7554)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK5_8_MCIO_DDRIOEXTMCC_REG 0x0D054554
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK5_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK5_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK6_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7558)                                                     */
/*       BDX (0x401A7558)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK6_8_MCIO_DDRIOEXTMCC_REG 0x0D054558
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK6_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK6_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK7_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A755C)                                                     */
/*       BDX (0x401A755C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK7_8_MCIO_DDRIOEXTMCC_REG 0x0D05455C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK7_8_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK7_8_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP1N1RANK0_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7560)                                                     */
/*       BDX (0x401A7560)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK0_8_MCIO_DDRIOEXTMCC_REG 0x0D054560

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK0_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK1_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7564)                                                     */
/*       BDX (0x401A7564)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK1_8_MCIO_DDRIOEXTMCC_REG 0x0D054564

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK1_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK2_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7568)                                                     */
/*       BDX (0x401A7568)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK2_8_MCIO_DDRIOEXTMCC_REG 0x0D054568

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK2_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK3_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A756C)                                                     */
/*       BDX (0x401A756C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK3_8_MCIO_DDRIOEXTMCC_REG 0x0D05456C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK3_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK4_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7570)                                                     */
/*       BDX (0x401A7570)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK4_8_MCIO_DDRIOEXTMCC_REG 0x0D054570

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK4_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK5_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7574)                                                     */
/*       BDX (0x401A7574)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK5_8_MCIO_DDRIOEXTMCC_REG 0x0D054574

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK5_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK6_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7578)                                                     */
/*       BDX (0x401A7578)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK6_8_MCIO_DDRIOEXTMCC_REG 0x0D054578

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK6_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK7_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A757C)                                                     */
/*       BDX (0x401A757C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK7_8_MCIO_DDRIOEXTMCC_REG 0x0D05457C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK7_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK0_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7580)                                                     */
/*       BDX (0x401A7580)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK0_8_MCIO_DDRIOEXTMCC_REG 0x0D054580

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK0_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK1_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7584)                                                     */
/*       BDX (0x401A7584)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK1_8_MCIO_DDRIOEXTMCC_REG 0x0D054584

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK1_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK2_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7588)                                                     */
/*       BDX (0x401A7588)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK2_8_MCIO_DDRIOEXTMCC_REG 0x0D054588

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK2_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK3_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A758C)                                                     */
/*       BDX (0x401A758C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK3_8_MCIO_DDRIOEXTMCC_REG 0x0D05458C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK3_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK4_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7590)                                                     */
/*       BDX (0x401A7590)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK4_8_MCIO_DDRIOEXTMCC_REG 0x0D054590

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK4_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK5_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7594)                                                     */
/*       BDX (0x401A7594)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK5_8_MCIO_DDRIOEXTMCC_REG 0x0D054594

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK5_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK6_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7598)                                                     */
/*       BDX (0x401A7598)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK6_8_MCIO_DDRIOEXTMCC_REG 0x0D054598

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK6_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK7_8_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A759C)                                                     */
/*       BDX (0x401A759C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK7_8_MCIO_DDRIOEXTMCC_REG 0x0D05459C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK7_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* RXVREFCTRLN1_8_MCIO_DDRIOEXTMCC_REG supported on:                          */
/*       HSX (0x401A75A4)                                                     */
/*       BDX (0x401A75A4)                                                     */
/* Register default value:              0x00000000                            */
#define RXVREFCTRLN1_8_MCIO_DDRIOEXTMCC_REG 0x0D0545A4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * RxVrefCtrl
 */
typedef union {
  struct {
    UINT32 vrefperbit0 : 7;
    /* vrefperbit0 - Bits[6:0], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben0 : 1;
    /* halflsben0 - Bits[7:7], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit1 : 7;
    /* vrefperbit1 - Bits[14:8], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben1 : 1;
    /* halflsben1 - Bits[15:15], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit2 : 7;
    /* vrefperbit2 - Bits[22:16], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben2 : 1;
    /* halflsben2 - Bits[23:23], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit3 : 7;
    /* vrefperbit3 - Bits[30:24], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben3 : 1;
    /* halflsben3 - Bits[31:31], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
  } Bits;
  UINT32 Data;
} RXVREFCTRLN1_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATATRAINFEEDBACKN1_8_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A75A8)                                                     */
/*       BDX (0x401A75A8)                                                     */
/* Register default value:              0x00000000                            */
#define DATATRAINFEEDBACKN1_8_MCIO_DDRIOEXTMCC_REG 0x0D0545A8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data Training Feedback Results
 */
typedef union {
  struct {
    UINT32 datatrainfeedback : 9;
    /* datatrainfeedback - Bits[8:0], RW_L, default = 1'b0 
       Data Training Feedback Storage
     */
    UINT32 rsvd_9 : 23;
  } Bits;
  UINT32 Data;
} DATATRAINFEEDBACKN1_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATATCOCOMPOFFSET0_8_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A75AC)                                                     */
/* Register default value:              0x00000000                            */
#define DATATCOCOMPOFFSET0_8_MCIO_DDRIOEXTMCC_REG 0x0D0545AC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 dq0tcocompoffset : 5;
    /* dq0tcocompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment offset added to Tco Comp Value.
     */
    UINT32 dq1tcocompoffset : 5;
    /* dq1tcocompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment offset added to Tco Comp Value.
     */
    UINT32 dq2tcocompoffset : 5;
    /* dq2tcocompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment offset added to DQ2 Tco Comp Value.
     */
    UINT32 dq3tcocompoffset : 5;
    /* dq3tcocompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment offset added to DQ3 Tco Comp Value.
     */
    UINT32 dqs0tcocompoffset : 5;
    /* dqs0tcocompoffset - Bits[24:20], RW_L, default = 4'b0000 
       2s Compliment offset added to DQ4 Tco Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATATCOCOMPOFFSET0_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATATCOCOMPOFFSET1_8_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A75B0)                                                     */
/* Register default value:              0x00000000                            */
#define DATATCOCOMPOFFSET1_8_MCIO_DDRIOEXTMCC_REG 0x0D0545B0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Per Bit Tco Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4tcocompoffset : 5;
    /* dq4tcocompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment offset added to DQ4 Tco Comp Value.
     */
    UINT32 dq5tcocompoffset : 5;
    /* dq5tcocompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment offset added to Dq5 Tco Comp Value.
     */
    UINT32 dq6tcocompoffset : 5;
    /* dq6tcocompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment offset added to Dq6 Tco Comp Value.
     */
    UINT32 dq7tcocompoffset : 5;
    /* dq7tcocompoffset - Bits[19:15], RW_L, default = 1'b0 
       2s Compliment offset added to Dq7 Tco Comp Value.
     */
    UINT32 dqs1tcocompoffset : 5;
    /* dqs1tcocompoffset - Bits[24:20], RW_L, default = 1'b0 
       2s Compliment offset added to Dqs1 Tco Comp Value.
     */
    UINT32 enperbittcocomp : 1;
    /* enperbittcocomp - Bits[25:25], RW_L, default = 1'b0 
       Enable per bit Tco Comp flow.
     */
    UINT32 gentcocompdone : 1;
    /* gentcocompdone - Bits[26:26], RW_L, default = 1'b0 
       Generate TCO Comp done.
     */
    UINT32 spare : 5;
    /* spare - Bits[31:27], RW_L, default = 1'b0 
       Generate TCO Comp done.
     */
  } Bits;
  UINT32 Data;
} DATATCOCOMPOFFSET1_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET0_8_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A75B4)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET0_8_MCIO_DDRIOEXTMCC_REG 0x0D0545B4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Per BIT ODT Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq0odtpupcompoffset : 5;
    /* dq0odtpupcompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq0 Odt Comp Value.
     */
    UINT32 dq1odtpupcompoffset : 5;
    /* dq1odtpupcompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq1 Odt Comp Value.
     */
    UINT32 dq2odtpupcompoffset : 5;
    /* dq2odtpupcompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to DQ2 Odt Comp Value.
     */
    UINT32 dq3odtpupcompoffset : 5;
    /* dq3odtpupcompoffset - Bits[19:15], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQ3 Odt Comp Value.
     */
    UINT32 dqs0odtpupcompoffset : 5;
    /* dqs0odtpupcompoffset - Bits[24:20], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQs0 Odt Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATAODTCOMPOFFSET0_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET1_8_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A75B8)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET1_8_MCIO_DDRIOEXTMCC_REG 0x0D0545B8

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Per Bit Odt Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq0odtpdncompoffset : 5;
    /* dq0odtpdncompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq0 Odt Comp Value.
     */
    UINT32 dq1odtpdncompoffset : 5;
    /* dq1odtpdncompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq1 Odt Comp Value.
     */
    UINT32 dq2odtpdncompoffset : 5;
    /* dq2odtpdncompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to DQ2 Odt Comp Value.
     */
    UINT32 dq3odtpdncompoffset : 5;
    /* dq3odtpdncompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment Pull-down offset added to DQ3 Odt Comp Value.
     */
    UINT32 dqs0odtpdncompoffset : 5;
    /* dqs0odtpdncompoffset - Bits[24:20], RW_L, default = 4'b0000 
       2s Compliment Pull-down offset added to DQS0 Odt Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATAODTCOMPOFFSET1_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET2_8_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A75BC)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET2_8_MCIO_DDRIOEXTMCC_REG 0x0D0545BC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Per Bit Odt Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4odtpupcompoffset : 5;
    /* dq4odtpupcompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq4 Odt Comp Value.
     */
    UINT32 dq5odtpupcompoffset : 5;
    /* dq5odtpupcompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq5 Odt Comp Value.
     */
    UINT32 dq6odtpupcompoffset : 5;
    /* dq6odtpupcompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to DQ6 Odt Comp Value.
     */
    UINT32 dq7odtpupcompoffset : 5;
    /* dq7odtpupcompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment Pull-up offset added to DQ7 Odt Comp Value.
     */
    UINT32 dqs1odtpupcompoffset : 5;
    /* dqs1odtpupcompoffset - Bits[24:20], RW_L, default = 2'b00 
       2s Compliment Pull-up offset added to DQs1 Odt Comp Value.
     */
    UINT32 enperbitodt : 1;
    /* enperbitodt - Bits[25:25], RW_L, default = 1'b0 
       Enable per bit ODT comp.
     */
    UINT32 genodtcompdone : 1;
    /* genodtcompdone - Bits[26:26], RW_L, default = 1'b0 
       Generate ODT comp Done.
     */
    UINT32 spare : 5;
    /* spare - Bits[31:27], RW_L, default = 1'b0 
       spare
     */
  } Bits;
  UINT32 Data;
} DATAODTCOMPOFFSET2_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET3_8_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A75C0)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET3_8_MCIO_DDRIOEXTMCC_REG 0x0D0545C0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Per BIT ODT Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4odtpdncompoffset : 5;
    /* dq4odtpdncompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq4 Odt Comp Value.
     */
    UINT32 dq5odtpdncompoffset : 5;
    /* dq5odtpdncompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq5 Odt Comp Value.
     */
    UINT32 dq6odtpdncompoffset : 5;
    /* dq6odtpdncompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to DQ6 Odt Comp Value.
     */
    UINT32 dq7odtpdncompoffset : 5;
    /* dq7odtpdncompoffset - Bits[19:15], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQ7 Odt Comp Value.
     */
    UINT32 dqs1odtpdncompoffset : 5;
    /* dqs1odtpdncompoffset - Bits[24:20], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQs0 Odt Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATAODTCOMPOFFSET3_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET0_8_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A75C4)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET0_8_MCIO_DDRIOEXTMCC_REG 0x0D0545C4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Per BIT RON Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq0ronpupcompoffset : 5;
    /* dq0ronpupcompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq0 Ron Comp Value.
     */
    UINT32 dq1ronpupcompoffset : 5;
    /* dq1ronpupcompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq1 Ron Comp Value.
     */
    UINT32 dq2ronpupcompoffset : 5;
    /* dq2ronpupcompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to DQ2 Ron Comp Value.
     */
    UINT32 dq3ronpupcompoffset : 5;
    /* dq3ronpupcompoffset - Bits[19:15], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQ3 Ron Comp Value.
     */
    UINT32 dqs0ronpupcompoffset : 5;
    /* dqs0ronpupcompoffset - Bits[24:20], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQs0 Ron Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATARONCOMPOFFSET0_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET1_8_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A75C8)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET1_8_MCIO_DDRIOEXTMCC_REG 0x0D0545C8

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Per Bit RON Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq0ronpdncompoffset : 5;
    /* dq0ronpdncompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq0 Ron Comp Value.
     */
    UINT32 dq1ronpdncompoffset : 5;
    /* dq1ronpdncompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq1 Ron Comp Value.
     */
    UINT32 dq2ronpdncompoffset : 5;
    /* dq2ronpdncompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to DQ2 Ron Comp Value.
     */
    UINT32 dq3ronpdncompoffset : 5;
    /* dq3ronpdncompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment Pull-down offset added to DQ3 Ron Comp Value.
     */
    UINT32 dqs0ronpdncompoffset : 5;
    /* dqs0ronpdncompoffset - Bits[24:20], RW_L, default = 4'b0000 
       2s Compliment Pull-down offset added to DQS0 Ron Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATARONCOMPOFFSET1_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET2_8_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A75CC)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET2_8_MCIO_DDRIOEXTMCC_REG 0x0D0545CC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Per Bit Ron Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4ronpupcompoffset : 5;
    /* dq4ronpupcompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq4 Ron Comp Value.
     */
    UINT32 dq5ronpupcompoffset : 5;
    /* dq5ronpupcompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq5 Ron Comp Value.
     */
    UINT32 dq6ronpupcompoffset : 5;
    /* dq6ronpupcompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to DQ6 Ron Comp Value.
     */
    UINT32 dq7ronpupcompoffset : 5;
    /* dq7ronpupcompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment Pull-up offset added to DQ7 Ron Comp Value.
     */
    UINT32 dqs1ronpupcompoffset : 5;
    /* dqs1ronpupcompoffset - Bits[24:20], RW_L, default = 2'b00 
       2s Compliment Pull-up offset added to DQs1 Ron Comp Value.
     */
    UINT32 enperbitron : 1;
    /* enperbitron - Bits[25:25], RW_L, default = 1'b0 
       Enable per bit RON comp.
     */
    UINT32 spare : 6;
    /* spare - Bits[31:26], RW_L, default = 1'b0 
       spare
     */
  } Bits;
  UINT32 Data;
} DATARONCOMPOFFSET2_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET3_8_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       BDX (0x401A75D0)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET3_8_MCIO_DDRIOEXTMCC_REG 0x0D0545D0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Per BIT RON Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4ronpdncompoffset : 5;
    /* dq4ronpdncompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq4 Ron Comp Value.
     */
    UINT32 dq5ronpdncompoffset : 5;
    /* dq5ronpdncompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq5 Ron Comp Value.
     */
    UINT32 dq6ronpdncompoffset : 5;
    /* dq6ronpdncompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to DQ6 Ron Comp Value.
     */
    UINT32 dq7ronpdncompoffset : 5;
    /* dq7ronpdncompoffset - Bits[19:15], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQ7 Ron Comp Value.
     */
    UINT32 dqs1ronpdncompoffset : 5;
    /* dqs1ronpdncompoffset - Bits[24:20], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQs1 Ron Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATARONCOMPOFFSET3_8_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */






/* DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A7608)                                                     */
/*       BDX (0x401A7608)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXTMCC_REG 0x0D054608

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CMD Pi Settings
 */
typedef union {
  struct {
    UINT32 cmdpicode0 : 6;
    /* cmdpicode0 - Bits[5:0], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay0 : 2;
    /* cmdpilogicdelay0 - Bits[7:6], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_8 : 1;
    UINT32 cmdpicode1 : 6;
    /* cmdpicode1 - Bits[14:9], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay1 : 2;
    /* cmdpilogicdelay1 - Bits[16:15], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_17 : 1;
    UINT32 cmdpicode2 : 6;
    /* cmdpicode2 - Bits[23:18], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay2 : 2;
    /* cmdpilogicdelay2 - Bits[25:24], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_26 : 6;
  } Bits;
  UINT32 Data;
} DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A760C)                                                     */
/*       BDX (0x401A760C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXTMCC_REG 0x0D05460C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * CMD Controls
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider in xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider in xover 2to2 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       adding extra pipeline stage after xover to avoid timing failure at higher ClkPi 
       rate 
     */
    UINT32 rsvd_4 : 2;
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[6:6], RW_L, default = 1'b0 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 rsvd_7 : 13;
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[20:20], RW_L, default = None 
       level shift hold enable
                            Holds the HV control values and power down mode the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rsvd_21 : 8;
    UINT32 paritydisable : 1;
    /* paritydisable - Bits[29:29], RW_L, default = 1'b0 
        disable parity and par_err pins for certain dimm types.
     */
    UINT32 ddr4modeenable : 1;
    /* ddr4modeenable - Bits[30:30], RW_L, default = 1'b0 
       Ddr4 Mode Enable, set to 1 in CMD2 (Func:5.Addr:C00 and Func:7.Addr:C00) fub to 
       bypass extra delay on parity bit 
     */
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CMD Controls
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider in xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider in xover 2to2 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       adding extra pipeline stage after xover to avoid timing failure at higher ClkPi 
       rate 
     */
    UINT32 rsvd_4 : 2;
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[6:6], RW_L, default = 1'b0 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 rsvd_7 : 13;
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[20:20], RW_L, default = None 
       level shift hold enable
                            Holds the HV control values and power down mode the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rsvd_21 : 7;
    UINT32 sr_gate_2to2 : 1;
    /* sr_gate_2to2 - Bits[28:28], RW_L, default = 1'b0 
       Used to ungate trainreset into crossover for 2:2 mode
     */
    UINT32 paritydisable : 1;
    /* paritydisable - Bits[29:29], RW_L, default = 1'b0 
        disable parity and par_err pins for certain dimm types. This must be set to 1 
       by BIOS for A0 to avoid Vref being enabled in non-ACIO loopback mode. 
     */
    UINT32 ddr4modeenable : 1;
    /* ddr4modeenable - Bits[30:30], RW_L, default = 1'b0 
       Ddr4 Mode Enable, set to 1 in CMD2 (Func:5.Addr:C00 and Func:7.Addr:C00) fub to 
       bypass extra delay on parity bit 
     */
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDCONTROLS2_CMDN_MCIO_DDRIOEXTMCC_REG supported on:                  */
/*       HSX (0x401A7610)                                                     */
/*       BDX (0x401A7610)                                                     */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00001000                            */
#define DDRCRCMDCONTROLS2_CMDN_MCIO_DDRIOEXTMCC_REG 0x0D054610
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * CMD Controls
 */
typedef union {
  struct {
    UINT32 ddrcrinvertpiclk : 12;
    /* ddrcrinvertpiclk - Bits[11:0], RW_L, default = 12'b0 
       Invert PiClk (used for 2:1, 2:2 when we need to invert the piclk.
     */
    UINT32 spare : 20;
    /* spare - Bits[31:12], RW_L, default = 20'b0 
       reserved.
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS2_CMDN_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CMD Controls
 */
typedef union {
  struct {
    UINT32 ddrcrinvertpiclk : 12;
    /* ddrcrinvertpiclk - Bits[11:0], RW_L, default = 12'b0 
       Invert PiClk (used for 2:1, 2:2 when we need to invert the piclk.
     */
    UINT32 bdx_sys : 1;
    /* bdx_sys - Bits[12:12], RW_L, default = 1'b1 
       BDX change : This bit helps to bypass the delaycell in transmitter
     */
    UINT32 rsvd_13 : 1;
    UINT32 spare : 18;
    /* spare - Bits[31:14], RW_L, default = 18'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS2_CMDN_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDPICODING2_CMDN_MCIO_DDRIOEXTMCC_REG supported on:                  */
/*       HSX (0x401A7618)                                                     */
/*       BDX (0x401A7618)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDPICODING2_CMDN_MCIO_DDRIOEXTMCC_REG 0x0D054618

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CMD Pi Settings
 */
typedef union {
  struct {
    UINT32 cmdpicode3 : 6;
    /* cmdpicode3 - Bits[5:0], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay3 : 2;
    /* cmdpilogicdelay3 - Bits[7:6], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_8 : 1;
    UINT32 cmdpicode4 : 6;
    /* cmdpicode4 - Bits[14:9], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay4 : 2;
    /* cmdpilogicdelay4 - Bits[16:15], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_17 : 1;
    UINT32 cmdpicode5 : 6;
    /* cmdpicode5 - Bits[23:18], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay5 : 2;
    /* cmdpilogicdelay5 - Bits[25:24], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_26 : 6;
  } Bits;
  UINT32 Data;
} DDRCRCMDPICODING2_CMDN_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






/* DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A762C)                                                     */
/*       BDX (0x401A762C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXTMCC_REG 0x0D05462C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Cmd Training Register
 */
typedef union {
  struct {
    UINT32 trainingoffset : 6;
    /* trainingoffset - Bits[5:0], RW_L, default = 6'b0 
       Offset added to PiCode. Obtained via xover calibration such that ClkPi aligns 
       with grid clock when PiCode=0. 
     */
    UINT32 rsvd_6 : 2;
    UINT32 xoverphasedet : 14;
    /* xoverphasedet - Bits[21:8], RO_V, default = 14'b0 
       Xover Phase Detect. Xover calibration feedback bits. Actually, using one of 14 
       is sufficient. 
                   [13]   CtlOutEnPath Xover Phase Detect
                   [12]   CmdOutEnPath Xover Phase Detect
                   [11:0] CmdBuf Xover Phase Detects
               
     */
    UINT32 trainingoffset2 : 6;
    /* trainingoffset2 - Bits[27:22], RW_L, default = 6'b0 
       Offset added to PiCode. Obtained via xover calibration such that ClkPi aligns 
       with grid clock when PiCode=0. 
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCMDCONTROLS1_CMDN_MCIO_DDRIOEXTMCC_REG supported on:                  */
/*       HSX (0x401A7630)                                                     */
/*       BDX (0x401A7630)                                                     */
/* Register default value on HSX:       0x00980000                            */
/* Register default value on BDX:       0x00180000                            */
#define DDRCRCMDCONTROLS1_CMDN_MCIO_DDRIOEXTMCC_REG 0x0D054630
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * CMD Controls 1
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    UINT32 internalvrefen : 1;
    /* internalvrefen - Bits[2:2], RW_L, default = 1'b0 
       Enable internal Vref for parity error pin, ACIOLB, or DcDetect (only for CTL)
     */
    UINT32 ddrcrimodeen : 1;
    /* ddrcrimodeen - Bits[3:3], RW_L, default = 1'b0 
       Imode en
     */
    UINT32 ddrcrimodesel : 1;
    /* ddrcrimodesel - Bits[4:4], RW_L, default = 1'b0 
       Imode sel, 0: swing boost; 1: xtalk cancellation.
     */
    UINT32 ddrcrimodesrcntl : 6;
    /* ddrcrimodesrcntl - Bits[10:5], RW_L, default = 6'b0 
       Imode Scomp configuration
     */
    UINT32 ddrcrimodesrcntlovrd : 1;
    /* ddrcrimodesrcntlovrd - Bits[11:11], RW_L, default = 1'b0 
       Imode scomp override enable
     */
    UINT32 ddrcrimodecoef : 4;
    /* ddrcrimodecoef - Bits[15:12], RW_L, default = 1'b0 
       Imode coef, swing boost configure
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_20 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[21:21], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 dqstatdflt : 2;
    /* dqstatdflt - Bits[23:22], RW_L, default = 2'b11 
       Static leg enable
     */
    UINT32 odtstatdflt : 2;
    /* odtstatdflt - Bits[25:24], RW_L, default = None 
       Static leg enable
     */
    UINT32 rsvd_26 : 1;
    UINT32 txeq : 5;
    /* txeq - Bits[31:27], RW_L, default = 5'b0 
       When TxEQOverride = 0, bits [29:27] are used as selects for the RCOMP -> EQCODE 
       calculation: 
       
       	             TxEQ[29:27] = 000 : EQCode = 0
       	             TxEQ[29:27] = 001 : EQCode = -1/4 nominal value  
       	             TxEQ[29:27] = 010 : EQCode = -1/8 nominal value  
       	             TxEQ[29:27] = 101 : EQCode = +1/4 nominal value  
       	             TxEQ[29:27] = 110 : EQCode = +1/8 nominal value  
       
                     Bits [31:30] are used to select between a 5, 3, or 1 
       multiplication factor for use in VMSE/DDR4: 
       
       	             TxEQ[31:30] = 00 : 5 * (comp + 16) / 16
       	             TxEQ[31:30] = 11 : 3 * (comp + 16) / 16
       	             TxEQ[31:30] = 01 : 1 * (comp + 16) / 16
       	             TxEQ[31:30] = 10 : 1 * (comp + 16) / 16
       
       
                            When TxEQOverride = 1, bits [31:27] are used directly as 
       the EQ Code: 
       
                     Write Equalization codes.  5 bit control, but only 24 of them are 
       meaningful.  
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS1_CMDN_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CMD Controls 1
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    UINT32 internalvrefen : 1;
    /* internalvrefen - Bits[2:2], RW_L, default = 1'b0 
       Enable internal Vref for parity error pin, ACIOLB, or DcDetect (only for CTL)
     */
    UINT32 ddrcrimodeen : 1;
    /* ddrcrimodeen - Bits[3:3], RW_L, default = 1'b0 
       Imode en
     */
    UINT32 ddrcrimodesel : 1;
    /* ddrcrimodesel - Bits[4:4], RW_L, default = 1'b0 
       Imode sel, 0: swing boost; 1: xtalk cancellation.
     */
    UINT32 ddrcrimodesrcntl : 6;
    /* ddrcrimodesrcntl - Bits[10:5], RW_L, default = 6'b0 
       Imode Scomp configuration
     */
    UINT32 ddrcrimodesrcntlovrd : 1;
    /* ddrcrimodesrcntlovrd - Bits[11:11], RW_L, default = 1'b0 
       Imode scomp override enable
     */
    UINT32 ddrcrimodecoef : 4;
    /* ddrcrimodecoef - Bits[15:12], RW_L, default = 1'b0 
       Imode coef, swing boost configure
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_20 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[21:21], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 dqstatdflt : 2;
    /* dqstatdflt - Bits[23:22], RW_L, default = 2'b11 
       Static leg enable
     */
    UINT32 odtstatdflt : 2;
    /* odtstatdflt - Bits[25:24], RW_L, default = None 
       Static leg enable
     */
    UINT32 txeq : 6;
    /* txeq - Bits[31:26], RW_L, default = 6'b0 
        Modified in BDX!! When TxEQOverride = 0, bits [29:27] are used as selects for 
       the RCOMP -> EQCODE calculation: 
       
       	             TxEQ[29:27] = 000 : EQCode = 0
       	             TxEQ[29:27] = 001 : EQCode = -1/4 nominal value  
       	             TxEQ[29:27] = 010 : EQCode = -1/8 nominal value  
       	             TxEQ[29:27] = 101 : EQCode = +1/4 nominal value  
       	             TxEQ[29:27] = 110 : EQCode = +1/8 nominal value  
       
                     Bits [31:30] are used to select between a 5, 3, or 1 
       multiplication factor for use in VMSE/DDR4: 
       
       	             TxEQ[31:30] = 00 : 5 * (comp + 16) / 16
       	             TxEQ[31:30] = 11 : 3 * (comp + 16) / 16
       	             TxEQ[31:30] = 01 : 1 * (comp + 16) / 16
       	             TxEQ[31:30] = 10 : 1 * (comp + 16) / 16
       
       
                            When TxEQOverride = 1, bits [31:27] are used directly as 
       the EQ Code: 
       
                     Write Equalization codes.  5 bit control, but only 24 of them are 
       meaningful.  
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS1_CMDN_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */







/* DDRCRCTLPICODING_CKE_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       HSX (0x401A7818)                                                     */
/*       BDX (0x401A7818)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING_CKE_MCIO_DDRIOEXTMCC_REG 0x0D054818

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CTL Pi Settings
 */
typedef union {
  struct {
    UINT32 ctlpicode0 : 6;
    /* ctlpicode0 - Bits[5:0], RW_L, default = 6'b0 
       Ctl Pi Code0
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay0 : 2;
    /* ctlpilogicdelay0 - Bits[7:6], RW_L, default = 2'b0 
       Ctl Pi Logic Delay0 - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode1 : 6;
    /* ctlpicode1 - Bits[13:8], RW_L, default = 6'b0 
       Ctl Pi Code1
     */
    UINT32 ctlpilogicdelay1 : 2;
    /* ctlpilogicdelay1 - Bits[15:14], RW_L, default = 2'b0 
       Ctl Pi Logic Delay1 - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode2 : 6;
    /* ctlpicode2 - Bits[21:16], RW_L, default = 6'b0 
       Ctl Pi Code2
     */
    UINT32 ctlpilogicdelay2 : 2;
    /* ctlpilogicdelay2 - Bits[23:22], RW_L, default = 2'b0 
       Ctl Pi Logic Delay2 - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode3 : 6;
    /* ctlpicode3 - Bits[29:24], RW_L, default = 6'b0 
       Ctl Pi Code3
     */
    UINT32 ctlpilogicdelay3 : 2;
    /* ctlpilogicdelay3 - Bits[31:30], RW_L, default = 2'b0 
       Ctl Pi Logic Delay3 - add 1~3 qclk cycles delay.
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLPICODING_CKE_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       HSX (0x401A781C)                                                     */
/*       BDX (0x401A781C)                                                     */
/* Register default value:              0x10010000                            */
#define DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXTMCC_REG 0x0D05481C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * CTL Controls
 */
typedef union {
  struct {
    UINT32 ddrphasextalkenableqnnnh : 1;
    /* ddrphasextalkenableqnnnh - Bits[0:0], RW_L, default = 1'b0 
       When Phased based xtalk cancellation is enabled, set this to delay cmd to 
       maintain constant Tcwl. 
     */
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider to enable xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider to enable xover 2to2 and 2to1 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       enable the extra flop stage in xover to help timing
     */
    UINT32 rsvd_4 : 17;
    UINT32 rxvref : 7;
    /* rxvref - Bits[27:21], RW_L, default = 7'b0 
       IOLB Vref Control. binary code from 1/3 Vdd to 2/3 Vdd with step size of 
       Vdd/192(??mV) 
     */
    UINT32 rsvd_28 : 1;
    UINT32 paritydisable : 1;
    /* paritydisable - Bits[29:29], RW_L, default = 1'b1 
       disable parity and par_err pins to save power when the populated dimm type does 
       not support parity 
     */
    UINT32 rsvd_30 : 1;
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CTL Controls
 */
typedef union {
  struct {
    UINT32 ddrpxcenable : 1;
    /* ddrpxcenable - Bits[0:0], RW_L, default = 1'b0 
       When Phased based xtalk cancellation is enabled, set this to delay cmd to 
       maintain constant Tcwl. 
     */
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider to enable xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider to enable xover 2to2 and 2to1 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       enable the extra flop stage in xover to help timing
     */
    UINT32 rsvd_4 : 13;
    UINT32 ctltxeq : 2;
    /* ctltxeq - Bits[18:17], RW_L, default = 2'b1 
       When LSB is set to 1, uses TxEQ register (DdrCrCmdControls1[31:27]) as the EQ 
       code rather than the  
       	             EQ code calculated from the CTL DRVUP comp code.
     */
    UINT32 rsvd_19 : 2;
    UINT32 rxvref : 7;
    /* rxvref - Bits[27:21], RW_L, default = 7'b0 
       IOLB Vref Control. binary code from 1/3 Vdd to 2/3 Vdd with step size of 
       Vdd/192(??mV) 
     */
    UINT32 sr_gate_2to2 : 1;
    /* sr_gate_2to2 - Bits[28:28], RW_L, default = 1'b0 
       Used to ungate trainreset into xover for 2:2 mode
     */
    UINT32 paritydisable : 1;
    /* paritydisable - Bits[29:29], RW_L, default = 1'b1 
       disable parity and par_err pins to save power when the populated dimm type does 
       not support parity 
     */
    UINT32 rsvd_30 : 1;
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A7820)                                                     */
/*       BDX (0x401A7820)                                                     */
/* Register default value on HSX:       0x00004FFF                            */
/* Register default value on BDX:       0xC0004FFF                            */
#define DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXTMCC_REG 0x0D054820
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * CTL Ranks Used
 */
typedef union {
  struct {
    UINT32 ranken : 12;
    /* ranken - Bits[11:0], RW_L, default = 12'b111111111111 
       Enables output buffers for ctlbuf 0 to ctlbuf 11, PI clocks and output datapaths 
       for this rank 
     */
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[12:12], RW_L, default = 1'b0 
       level shift hold enable:
                            Holds the HV control values and power down the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rxbiasgcncomp : 2;
    /* rxbiasgcncomp - Bits[14:13], RW_L, default = 2'b10 
       used to compensate the GCN skew. The GCN skew information should be extracted by 
       reading and analyzing the final Rcomp code value. The target spec is to 
       categorize the GCN skew in 3 process condition (mainly slow, fast, typical). 
       (one example can be just reading the 2 MLS bit of Rcomp code and determine if 
       the GCN is slow, fast or typical). The gcncomp is used for CTLE. 
       00: gcn in typical corner
       11: gcn in slow corner
       10: gcn in typical corner
       01: gcn in fast corner
               
     */
    UINT32 rsvd_15 : 1;
    UINT32 imodebiashighcm : 1;
    /* imodebiashighcm - Bits[16:16], RW_L, default = 1'b0 
       Imode bias control
     */
    UINT32 rxbiassel : 1;
    /* rxbiassel - Bits[17:17], RW_L, default = 1'b0 
       Rx bias Sel
     */
    UINT32 ddrcrinvertpiclk : 12;
    /* ddrcrinvertpiclk - Bits[29:18], RW_L, default = 12'b0 
       Invert PiClk in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 phase
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[30:30], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CTL Ranks Used
 */
typedef union {
  struct {
    UINT32 ranken : 12;
    /* ranken - Bits[11:0], RW_L, default = 12'b111111111111 
       Enables output buffers for ctlbuf 0 to ctlbuf 11, PI clocks and output datapaths 
       for this rank 
     */
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[12:12], RW_L, default = 1'b0 
       level shift hold enable:
                            Holds the HV control values and power down the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rxbiasgcncomp : 2;
    /* rxbiasgcncomp - Bits[14:13], RW_L, default = 2'b10 
       used to compensate the GCN skew. The GCN skew information should be
                 extracted by reading and analyzing the final Rcomp code value. The 
       target spec 
                 is to categorize the GCN skew in 3 process condition (mainly slow, 
       fast, typical). 
                 (one example can be just reading the 2 MLS bit of Rcomp code and 
       determine if the 
                 GCN is slow, fast or typical). The gcncomp is used for CTLE.
                 11: gcn in slow corner
                 10: gcn in typical corner
                 01: gcn in fast corner
               
     */
    UINT32 rsvd_15 : 1;
    UINT32 imodebiashighcm : 1;
    /* imodebiashighcm - Bits[16:16], RW_L, default = 1'b0 
       Imode bias control
     */
    UINT32 rxbiassel : 1;
    /* rxbiassel - Bits[17:17], RW_L, default = 1'b0 
       Rx bias Sel
     */
    UINT32 ddrcrinvertpiclk : 12;
    /* ddrcrinvertpiclk - Bits[29:18], RW_L, default = 12'b0 
       Invert PiClk in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 phase
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[30:30], RW_L, default = 1'b1 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */







/* DDRCRCTLTRAINING_CKE_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       HSX (0x401A782C)                                                     */
/*       BDX (0x401A782C)                                                     */
/* Register default value:              0x00007000                            */
#define DDRCRCTLTRAINING_CKE_MCIO_DDRIOEXTMCC_REG 0x0D05482C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Ctl Training Register
 */
typedef union {
  struct {
    UINT32 trainingoffset : 6;
    /* trainingoffset - Bits[5:0], RW_L, default = 6'b0 
       Offset added to PiCode. Obtained via xover calibration such that ClkPi aligns 
       with grid clock when PiCode=0. 
     */
    UINT32 dcdetectmode : 1;
    /* dcdetectmode - Bits[6:6], RW_L, default = None 
       Enables VMSE init DcDetect mode
     */
    UINT32 rsvd_7 : 1;
    UINT32 xoverphasedet : 12;
    /* xoverphasedet - Bits[19:8], RO_V, default = 1'b0 
       Xover Phase Detect. Xover calibration feedback bits.
     */
    UINT32 ctldrvsegen : 3;
    /* ctldrvsegen - Bits[22:20], RW_L, default = 3'b000 
       drvsegdisable, or drvSegEn_b. 
     */
    UINT32 ckepddrvsegen : 3;
    /* ckepddrvsegen - Bits[25:23], RW_L, default = 3'b111 
        Controls the number of drive segments enabled during CKE powerdown 
     */
    UINT32 rsvd_26 : 6;
  } Bits;
  UINT32 Data;
} DDRCRCTLTRAINING_CKE_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Ctl Training Register
 */
typedef union {
  struct {
    UINT32 trainingoffset : 6;
    /* trainingoffset - Bits[5:0], RW_L, default = 6'b0 
       Offset added to PiCode. Obtained via xover calibration such that ClkPi aligns 
       with grid clock when PiCode=0. 
     */
    UINT32 dcdetectmode : 1;
    /* dcdetectmode - Bits[6:6], RW_L, default = None 
       Enables VMSE init DcDetect mode
     */
    UINT32 rsvd_7 : 1;
    UINT32 xoverphasedet : 12;
    /* xoverphasedet - Bits[19:8], RO_V, default = 1'b0 
       Xover Phase Detect. Xover calibration feedback bits.
     */
    UINT32 ctldrvsegen : 3;
    /* ctldrvsegen - Bits[22:20], RW_L, default = 3'b000 
       drvsegdisable, or drvSegEn_b. 
     */
    UINT32 ckepddrvsegen : 3;
    /* ckepddrvsegen - Bits[25:23], RW_L, default = 3'b111 
        Controls the number of drive segments enabled during CKE powerdown 
     */
    UINT32 rsvd_26 : 6;
  } Bits;
  UINT32 Data;
} DDRCRCTLTRAINING_CKE_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A7830)                                                     */
/*       BDX (0x401A7830)                                                     */
/* Register default value on HSX:       0x00980000                            */
/* Register default value on BDX:       0x00180000                            */
#define DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXTMCC_REG 0x0D054830
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * CMD Controls 1
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    UINT32 internalvrefen : 1;
    /* internalvrefen - Bits[2:2], RW_L, default = 1'b0 
       Enable internal Vref for parity error pin, ACIOLB, or DcDetect (only for CTL)
     */
    UINT32 ddrcrimodeen : 1;
    /* ddrcrimodeen - Bits[3:3], RW_L, default = 1'b0 
       Imode en
     */
    UINT32 ddrcrimodesel : 1;
    /* ddrcrimodesel - Bits[4:4], RW_L, default = 1'b0 
       Imode sel, 0: swing boost; 1: xtalk cancellation.
     */
    UINT32 ddrcrimodesrcntl : 6;
    /* ddrcrimodesrcntl - Bits[10:5], RW_L, default = 6'b0 
       Imode Scomp configuration
     */
    UINT32 ddrcrimodesrcntlovrd : 1;
    /* ddrcrimodesrcntlovrd - Bits[11:11], RW_L, default = 1'b0 
       Imode scomp override enable
     */
    UINT32 ddrcrimodecoef : 4;
    /* ddrcrimodecoef - Bits[15:12], RW_L, default = 1'b0 
       Imode coef, swing boost configure
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_20 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[21:21], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 dqstatdflt : 2;
    /* dqstatdflt - Bits[23:22], RW_L, default = 2'b11 
       Static leg enable
     */
    UINT32 odtstatdflt : 2;
    /* odtstatdflt - Bits[25:24], RW_L, default = None 
       Static leg enable
     */
    UINT32 rsvd_26 : 1;
    UINT32 txeq : 5;
    /* txeq - Bits[31:27], RW_L, default = 5'b0 
       When TxEQOverride = 0, bits [29:27] are used as selects for the RCOMP -> EQCODE 
       calculation: 
       
       	             TxEQ[29:27] = 000 : EQCode = 0
       	             TxEQ[29:27] = 001 : EQCode = -1/4 nominal value  
       	             TxEQ[29:27] = 010 : EQCode = -1/8 nominal value  
       	             TxEQ[29:27] = 101 : EQCode = +1/4 nominal value  
       	             TxEQ[29:27] = 110 : EQCode = +1/8 nominal value  
       
                     Bits [31:30] are used to select between a 5, 3, or 1 
       multiplication factor for use in VMSE/DDR4: 
       
       	             TxEQ[31:30] = 00 : 5 * (comp + 16) / 16
       	             TxEQ[31:30] = 11 : 3 * (comp + 16) / 16
       	             TxEQ[31:30] = 01 : 1 * (comp + 16) / 16
       	             TxEQ[31:30] = 10 : 1 * (comp + 16) / 16
       
       
                            When TxEQOverride = 1, bits [31:27] are used directly as 
       the EQ Code: 
       
                     Write Equalization codes.  5 bit control, but only 24 of them are 
       meaningful.  
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CMD Controls 1
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    UINT32 internalvrefen : 1;
    /* internalvrefen - Bits[2:2], RW_L, default = 1'b0 
       Enable internal Vref for parity error pin, ACIOLB, or DcDetect (only for CTL)
     */
    UINT32 ddrcrimodeen : 1;
    /* ddrcrimodeen - Bits[3:3], RW_L, default = 1'b0 
       Imode en
     */
    UINT32 ddrcrimodesel : 1;
    /* ddrcrimodesel - Bits[4:4], RW_L, default = 1'b0 
       Imode sel, 0: swing boost; 1: xtalk cancellation.
     */
    UINT32 ddrcrimodesrcntl : 6;
    /* ddrcrimodesrcntl - Bits[10:5], RW_L, default = 6'b0 
       Imode Scomp configuration
     */
    UINT32 ddrcrimodesrcntlovrd : 1;
    /* ddrcrimodesrcntlovrd - Bits[11:11], RW_L, default = 1'b0 
       Imode scomp override enable
     */
    UINT32 ddrcrimodecoef : 4;
    /* ddrcrimodecoef - Bits[15:12], RW_L, default = 1'b0 
       Imode coef, swing boost configure
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_20 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[21:21], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 dqstatdflt : 2;
    /* dqstatdflt - Bits[23:22], RW_L, default = 2'b11 
       Static leg enable
     */
    UINT32 odtstatdflt : 2;
    /* odtstatdflt - Bits[25:24], RW_L, default = None 
       Static leg enable
     */
    UINT32 txeq : 6;
    /* txeq - Bits[31:26], RW_L, default = 6'b0 
        Modified in BDX!! When TxEQOverride = 0, bits [29:27] are used as selects for 
       the RCOMP -> EQCODE calculation: 
       
       	             TxEQ[29:27] = 000 : EQCode = 0
       	             TxEQ[29:27] = 001 : EQCode = -1/4 nominal value  
       	             TxEQ[29:27] = 010 : EQCode = -1/8 nominal value  
       	             TxEQ[29:27] = 101 : EQCode = +1/4 nominal value  
       	             TxEQ[29:27] = 110 : EQCode = +1/8 nominal value  
       
                     Bits [31:30] are used to select between a 5, 3, or 1 
       multiplication factor for use in VMSE/DDR4: 
       
       	             TxEQ[31:30] = 00 : 5 * (comp + 16) / 16
       	             TxEQ[31:30] = 11 : 3 * (comp + 16) / 16
       	             TxEQ[31:30] = 01 : 1 * (comp + 16) / 16
       	             TxEQ[31:30] = 10 : 1 * (comp + 16) / 16
       
       
                            When TxEQOverride = 1, bits [31:27] are used directly as 
       the EQ Code: 
       
                     Write Equalization codes.  5 bit control, but only 24 of them are 
       meaningful.  
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCTLPICODING2_CKE_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A7834)                                                     */
/*       BDX (0x401A7834)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING2_CKE_MCIO_DDRIOEXTMCC_REG 0x0D054834

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CTL Pi Settings
 */
typedef union {
  struct {
    UINT32 ctlpicode4 : 6;
    /* ctlpicode4 - Bits[5:0], RW_L, default = 6'b0 
       Ctl Pi Code4
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay4 : 2;
    /* ctlpilogicdelay4 - Bits[7:6], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode5 : 6;
    /* ctlpicode5 - Bits[13:8], RW_L, default = 6'b0 
       Ctl Pi Code5
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay5 : 2;
    /* ctlpilogicdelay5 - Bits[15:14], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode6 : 6;
    /* ctlpicode6 - Bits[21:16], RW_L, default = 6'b0 
       Ctl Pi Code6
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay6 : 2;
    /* ctlpilogicdelay6 - Bits[23:22], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode7 : 6;
    /* ctlpicode7 - Bits[29:24], RW_L, default = 6'b0 
       Ctl Pi Code7
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay7 : 2;
    /* ctlpilogicdelay7 - Bits[31:30], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLPICODING2_CKE_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCTLPICODING3_CKE_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A7838)                                                     */
/*       BDX (0x401A7838)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING3_CKE_MCIO_DDRIOEXTMCC_REG 0x0D054838

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CTL Pi Settings
 */
typedef union {
  struct {
    UINT32 ctlpicode8 : 6;
    /* ctlpicode8 - Bits[5:0], RW_L, default = 6'b0 
       Ctl Pi Code8
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay8 : 2;
    /* ctlpilogicdelay8 - Bits[7:6], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode9 : 6;
    /* ctlpicode9 - Bits[13:8], RW_L, default = 6'b0 
       Ctl Pi Code9
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay9 : 2;
    /* ctlpilogicdelay9 - Bits[15:14], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode10 : 6;
    /* ctlpicode10 - Bits[21:16], RW_L, default = 6'b0 
       Ctl Pi Code10
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay10 : 2;
    /* ctlpilogicdelay10 - Bits[23:22], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} DDRCRCTLPICODING3_CKE_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* DDRCRCTLPICODING4_CKE_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A7840)                                                     */
/*       BDX (0x401A7840)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING4_CKE_MCIO_DDRIOEXTMCC_REG 0x0D054840

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CTL Pi Settings 4 
 */
typedef union {
  struct {
    UINT32 vmselogicdelay : 12;
    /* vmselogicdelay - Bits[11:0], RW_L, default = None 
       VmseLogicDelay - add 2x clock cycle delay per vcmd bit
     */
    UINT32 rsvd_12 : 2;
    UINT32 vmsephase1cmdlogicdelay : 12;
    /* vmsephase1cmdlogicdelay - Bits[25:14], RW_L, default = 12'h0 
       When this is set, add one Qclk delay to the Intel SMI 2 
                   phase1 VCMD in case of a polarity inversion in serializer.
                
     */
    UINT32 flipvmseserializerphase : 1;
    /* flipvmseserializerphase - Bits[26:26], RW_L, default = 1'b0 
       When this is set, flip the phase of the serializer for VMSE 1:1 mode.
                
     */
    UINT32 rsvd_27 : 5;
  } Bits;
  UINT32 Data;
} DDRCRCTLPICODING4_CKE_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCLKRANKSUSED_MCIO_DDRIOEXTMCC_REG supported on:                       */
/*       HSX (0x401A7A00)                                                     */
/*       BDX (0x401A7A00)                                                     */
/* Register default value:              0x0000070F                            */
#define DDRCRCLKRANKSUSED_MCIO_DDRIOEXTMCC_REG 0x0D054A00

#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CLK Ranks Used
 */
typedef union {
  struct {
    UINT32 ranken : 4;
    /* ranken - Bits[3:0], RW_L, default = 4'b1111 
       Enables output buffers, PI clocks and output datapaths for this rank
     */
    UINT32 ddrcrinvertpiclk : 4;
    /* ddrcrinvertpiclk - Bits[7:4], RW_L, default = 4'b0 
       Invert PiClk in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 phase
     */
    UINT32 drvsegen : 3;
    /* drvsegen - Bits[10:8], RW_L, default = 3'b111 
       Segments to enable for driving (inverse for ODT)
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[11:11], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 rsvd_12 : 20;
  } Bits;
  UINT32 Data;
} DDRCRCLKRANKSUSED_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CLK Ranks Used
 */
typedef union {
  struct {
    UINT32 ranken : 4;
    /* ranken - Bits[3:0], RW_L, default = 4'b1111
       Enables output buffers, PI clocks and output datapaths for this rank
     */
    UINT32 ddrcrinvertpiclk : 4;
    /* ddrcrinvertpiclk - Bits[7:4], RW_L, default = 4'b0
       Invert PiClk in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 phase
     */
    UINT32 drvsegen : 3;
    /* drvsegen - Bits[10:8], RW_L, default = 3'b111
       Segments to enable for driving (inverse for ODT)
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[11:11], RW_L, default = None
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 rsvd_12 : 1;
    UINT32 rsvd_13 : 19;
  } Bits;
  UINT32 Data;
} DDRCRCLKRANKSUSED_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */






/* DDRCRCLKPICODE_MCIO_DDRIOEXTMCC_REG supported on:                          */
/*       HSX (0x401A7A0C)                                                     */
/*       BDX (0x401A7A0C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCLKPICODE_MCIO_DDRIOEXTMCC_REG 0x0D054A0C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CLK Pi Codes
 */
typedef union {
  struct {
    UINT32 pisettingrank0 : 6;
    /* pisettingrank0 - Bits[5:0], RW_L, default = 1'b0 
       Pi Setting, Rank 0
                            [5:0] - Pi setting for xxDDR_CLK_P/N[0]
     */
    UINT32 pilogicdelayrank0 : 1;
    /* pilogicdelayrank0 - Bits[6:6], RW_L, default = 1'b0 
       Pi Logic Delay, Rank 0, delays CLK by an extra qclk cycle
     */
    UINT32 rsvd_7 : 1;
    UINT32 pisettingrank1 : 6;
    /* pisettingrank1 - Bits[13:8], RW_L, default = 1'b0 
       Pi Setting, Rank 1
                            [5:0] - Pi setting for xxDDR_CLK_P/N[1]
     */
    UINT32 pilogicdelayrank1 : 1;
    /* pilogicdelayrank1 - Bits[14:14], RW_L, default = 1'b0 
       Pi Logic Delay, Rank 1, delays CLK by an extra qclk cycle
     */
    UINT32 rsvd_15 : 1;
    UINT32 pisettingrank2 : 6;
    /* pisettingrank2 - Bits[21:16], RW_L, default = 1'b0 
       Pi Setting, Rank 2
                            [5:0] - Pi setting for xxDDR_CLK_P/N[2]
     */
    UINT32 pilogicdelayrank2 : 1;
    /* pilogicdelayrank2 - Bits[22:22], RW_L, default = 1'b0 
       Pi Logic Delay, Rank 2, delays CLK by an extra qclk cycle
     */
    UINT32 rsvd_23 : 1;
    UINT32 pisettingrank3 : 6;
    /* pisettingrank3 - Bits[29:24], RW_L, default = 1'b0 
       Pi Setting, Rank 3
                            [5:0] - Pi setting for xxDDR_CLK_P/N[3]
     */
    UINT32 pilogicdelayrank3 : 1;
    /* pilogicdelayrank3 - Bits[30:30], RW_L, default = 1'b0 
       Pi Logic Delay, Rank 3
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DDRCRCLKPICODE_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCLKCONTROLS_MCIO_DDRIOEXTMCC_REG supported on:                        */
/*       HSX (0x401A7A10)                                                     */
/*       BDX (0x401A7A10)                                                     */
/* Register default value:              0x00030000                            */
#define DDRCRCLKCONTROLS_MCIO_DDRIOEXTMCC_REG 0x0D054A10
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * CLK Controls
 */
typedef union {
  struct {
    UINT32 ddrphasextalkenableqnnnh : 1;
    /* ddrphasextalkenableqnnnh - Bits[0:0], RW_L, default = 1'b0 
       When Phased based xtalk cancellation is enabled, set this to delay cmd to 
       maintain constant Tcwl. 
     */
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider to enable xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider to enable xover 2to2 and 2to1 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       enable the extra flop stage in xover to help timing
     */
    UINT32 rsvd_4 : 12;
    UINT32 is_rdimm : 1;
    /* is_rdimm - Bits[16:16], RW_L, default = None 
       Drives xxCLK to DC during P6 and S3. Should be set for RDIMMs and LRDIMMs in 
       DDR3 AND DDR4. 
     */
    UINT32 statlegen : 2;
    /* statlegen - Bits[18:17], RW_L, default = 2'b11 
       static leg enable
     */
    UINT32 rsvd_19 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[20:20], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 rsvd_21 : 9;
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[30:30], RW_L, default = 1'b0 
       level shift hold enable:
                            Holds the HV control values and power down the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training during MRC training
     */
  } Bits;
  UINT32 Data;
} DDRCRCLKCONTROLS_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CLK Controls
 */
typedef union {
  struct {
    UINT32 ddrpxcenable : 1;
    /* ddrpxcenable - Bits[0:0], RW_L, default = 1'b0 
       NOT USED - When Phased based xtalk cancellation is enabled, set this to delay 
       cmd to maintain constant Tcwl. 
     */
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider to enable xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider to enable xover 2to2 and 2to1 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       enable the extra flop stage in xover to help timing
     */
    UINT32 rsvd_4 : 12;
    UINT32 is_rdimm : 1;
    /* is_rdimm - Bits[16:16], RW_L, default = None 
       Drives xxCLK to DC during P6 and S3. Should be set for RDIMMs and LRDIMMs in 
       DDR3 AND DDR4. 
     */
    UINT32 statlegen : 2;
    /* statlegen - Bits[18:17], RW_L, default = 2'b11 
       static leg enable
     */
    UINT32 rsvd_19 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[20:20], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 rsvd_21 : 9;
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[30:30], RW_L, default = 1'b0 
       level shift hold enable:
                            Holds the HV control values and power down the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training during MRC training
     */
  } Bits;
  UINT32 Data;
} DDRCRCLKCONTROLS_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */







/* DDRCRCLKTRAINING_MCIO_DDRIOEXTMCC_REG supported on:                        */
/*       HSX (0x401A7A1C)                                                     */
/*       BDX (0x401A7A1C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCLKTRAINING_MCIO_DDRIOEXTMCC_REG 0x0D054A1C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Clk Training Register
 */
typedef union {
  struct {
    UINT32 trainingoffset : 6;
    /* trainingoffset - Bits[5:0], RW_L, default = 1'b0 
       Offset added to PiCode. Obtained via xover calibration such that ClkPi aligns 
       with grid clock when PiCode=0. 
     */
    UINT32 rsvd_6 : 2;
    UINT32 pclkxoverphasedet : 4;
    /* pclkxoverphasedet - Bits[11:8], RO_V, default = 1'b0 
       P Clock Buffer Xover Phase Detect. used for xover calibration.
     */
    UINT32 nclkxoverphasedet : 4;
    /* nclkxoverphasedet - Bits[15:12], RO_V, default = 1'b0 
       N Clock Buffer Xover Phase Detect.  used for xover calibration.
     */
    UINT32 rsvd_16 : 14;
    UINT32 vmselogicdelay : 1;
    /* vmselogicdelay - Bits[30:30], RW, default = 1'b0 
       VMSE Logic Delay, add extra qclk cycle delay to CLK output
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DDRCRCLKTRAINING_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */








/* DDRCRCMDPICODING_CMDS_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A7C08)                                                     */
/*       BDX (0x401A7C08)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDPICODING_CMDS_MCIO_DDRIOEXTMCC_REG 0x0D054C08

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CMD Pi Settings
 */
typedef union {
  struct {
    UINT32 cmdpicode0 : 6;
    /* cmdpicode0 - Bits[5:0], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay0 : 2;
    /* cmdpilogicdelay0 - Bits[7:6], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_8 : 1;
    UINT32 cmdpicode1 : 6;
    /* cmdpicode1 - Bits[14:9], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay1 : 2;
    /* cmdpilogicdelay1 - Bits[16:15], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_17 : 1;
    UINT32 cmdpicode2 : 6;
    /* cmdpicode2 - Bits[23:18], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay2 : 2;
    /* cmdpilogicdelay2 - Bits[25:24], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_26 : 6;
  } Bits;
  UINT32 Data;
} DDRCRCMDPICODING_CMDS_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A7C0C)                                                     */
/*       BDX (0x401A7C0C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXTMCC_REG 0x0D054C0C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * CMD Controls
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider in xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider in xover 2to2 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       adding extra pipeline stage after xover to avoid timing failure at higher ClkPi 
       rate 
     */
    UINT32 rsvd_4 : 2;
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[6:6], RW_L, default = 1'b0 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 rsvd_7 : 13;
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[20:20], RW_L, default = None 
       level shift hold enable
                            Holds the HV control values and power down mode the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rsvd_21 : 8;
    UINT32 paritydisable : 1;
    /* paritydisable - Bits[29:29], RW_L, default = 1'b0 
        disable parity and par_err pins for certain dimm types.
     */
    UINT32 ddr4modeenable : 1;
    /* ddr4modeenable - Bits[30:30], RW_L, default = 1'b0 
       Ddr4 Mode Enable, set to 1 in CMD2 (Func:5.Addr:C00 and Func:7.Addr:C00) fub to 
       bypass extra delay on parity bit 
     */
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CMD Controls
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider in xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider in xover 2to2 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       adding extra pipeline stage after xover to avoid timing failure at higher ClkPi 
       rate 
     */
    UINT32 rsvd_4 : 2;
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[6:6], RW_L, default = 1'b0 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 rsvd_7 : 13;
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[20:20], RW_L, default = None 
       level shift hold enable
                            Holds the HV control values and power down mode the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rsvd_21 : 7;
    UINT32 sr_gate_2to2 : 1;
    /* sr_gate_2to2 - Bits[28:28], RW_L, default = 1'b0 
       Used to ungate trainreset into crossover for 2:2 mode
     */
    UINT32 paritydisable : 1;
    /* paritydisable - Bits[29:29], RW_L, default = 1'b0 
        disable parity and par_err pins for certain dimm types. This must be set to 1 
       by BIOS for A0 to avoid Vref being enabled in non-ACIO loopback mode. 
     */
    UINT32 ddr4modeenable : 1;
    /* ddr4modeenable - Bits[30:30], RW_L, default = 1'b0 
       Ddr4 Mode Enable, set to 1 in CMD2 (Func:5.Addr:C00 and Func:7.Addr:C00) fub to 
       bypass extra delay on parity bit 
     */
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDCONTROLS2_CMDS_MCIO_DDRIOEXTMCC_REG supported on:                  */
/*       HSX (0x401A7C10)                                                     */
/*       BDX (0x401A7C10)                                                     */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00001000                            */
#define DDRCRCMDCONTROLS2_CMDS_MCIO_DDRIOEXTMCC_REG 0x0D054C10
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * CMD Controls
 */
typedef union {
  struct {
    UINT32 ddrcrinvertpiclk : 12;
    /* ddrcrinvertpiclk - Bits[11:0], RW_L, default = 12'b0 
       Invert PiClk (used for 2:1, 2:2 when we need to invert the piclk.
     */
    UINT32 spare : 20;
    /* spare - Bits[31:12], RW_L, default = 20'b0 
       reserved.
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS2_CMDS_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CMD Controls
 */
typedef union {
  struct {
    UINT32 ddrcrinvertpiclk : 12;
    /* ddrcrinvertpiclk - Bits[11:0], RW_L, default = 12'b0 
       Invert PiClk (used for 2:1, 2:2 when we need to invert the piclk.
     */
    UINT32 bdx_sys : 1;
    /* bdx_sys - Bits[12:12], RW_L, default = 1'b1 
       BDX change : This bit helps to bypass the delaycell in transmitter
     */
    UINT32 rsvd_13 : 1;
    UINT32 spare : 18;
    /* spare - Bits[31:14], RW_L, default = 18'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS2_CMDS_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDPICODING2_CMDS_MCIO_DDRIOEXTMCC_REG supported on:                  */
/*       HSX (0x401A7C18)                                                     */
/*       BDX (0x401A7C18)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDPICODING2_CMDS_MCIO_DDRIOEXTMCC_REG 0x0D054C18

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CMD Pi Settings
 */
typedef union {
  struct {
    UINT32 cmdpicode3 : 6;
    /* cmdpicode3 - Bits[5:0], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay3 : 2;
    /* cmdpilogicdelay3 - Bits[7:6], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_8 : 1;
    UINT32 cmdpicode4 : 6;
    /* cmdpicode4 - Bits[14:9], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay4 : 2;
    /* cmdpilogicdelay4 - Bits[16:15], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_17 : 1;
    UINT32 cmdpicode5 : 6;
    /* cmdpicode5 - Bits[23:18], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay5 : 2;
    /* cmdpilogicdelay5 - Bits[25:24], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_26 : 6;
  } Bits;
  UINT32 Data;
} DDRCRCMDPICODING2_CMDS_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






/* DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A7C2C)                                                     */
/*       BDX (0x401A7C2C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXTMCC_REG 0x0D054C2C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Cmd Training Register
 */
typedef union {
  struct {
    UINT32 trainingoffset : 6;
    /* trainingoffset - Bits[5:0], RW_L, default = 6'b0 
       Offset added to PiCode. Obtained via xover calibration such that ClkPi aligns 
       with grid clock when PiCode=0. 
     */
    UINT32 rsvd_6 : 2;
    UINT32 xoverphasedet : 14;
    /* xoverphasedet - Bits[21:8], RO_V, default = 14'b0 
       Xover Phase Detect. Xover calibration feedback bits. Actually, using one of 14 
       is sufficient. 
                   [13]   CtlOutEnPath Xover Phase Detect
                   [12]   CmdOutEnPath Xover Phase Detect
                   [11:0] CmdBuf Xover Phase Detects
               
     */
    UINT32 trainingoffset2 : 6;
    /* trainingoffset2 - Bits[27:22], RW_L, default = 6'b0 
       Offset added to PiCode. Obtained via xover calibration such that ClkPi aligns 
       with grid clock when PiCode=0. 
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXTMCC_REG supported on:                  */
/*       HSX (0x401A7C30)                                                     */
/*       BDX (0x401A7C30)                                                     */
/* Register default value on HSX:       0x00980000                            */
/* Register default value on BDX:       0x00180000                            */
#define DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXTMCC_REG 0x0D054C30
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * CMD Controls 1
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    UINT32 internalvrefen : 1;
    /* internalvrefen - Bits[2:2], RW_L, default = 1'b0 
       Enable internal Vref for parity error pin, ACIOLB, or DcDetect (only for CTL)
     */
    UINT32 ddrcrimodeen : 1;
    /* ddrcrimodeen - Bits[3:3], RW_L, default = 1'b0 
       Imode en
     */
    UINT32 ddrcrimodesel : 1;
    /* ddrcrimodesel - Bits[4:4], RW_L, default = 1'b0 
       Imode sel, 0: swing boost; 1: xtalk cancellation.
     */
    UINT32 ddrcrimodesrcntl : 6;
    /* ddrcrimodesrcntl - Bits[10:5], RW_L, default = 6'b0 
       Imode Scomp configuration
     */
    UINT32 ddrcrimodesrcntlovrd : 1;
    /* ddrcrimodesrcntlovrd - Bits[11:11], RW_L, default = 1'b0 
       Imode scomp override enable
     */
    UINT32 ddrcrimodecoef : 4;
    /* ddrcrimodecoef - Bits[15:12], RW_L, default = 1'b0 
       Imode coef, swing boost configure
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_20 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[21:21], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 dqstatdflt : 2;
    /* dqstatdflt - Bits[23:22], RW_L, default = 2'b11 
       Static leg enable
     */
    UINT32 odtstatdflt : 2;
    /* odtstatdflt - Bits[25:24], RW_L, default = None 
       Static leg enable
     */
    UINT32 rsvd_26 : 1;
    UINT32 txeq : 5;
    /* txeq - Bits[31:27], RW_L, default = 5'b0 
       When TxEQOverride = 0, bits [29:27] are used as selects for the RCOMP -> EQCODE 
       calculation: 
       
       	             TxEQ[29:27] = 000 : EQCode = 0
       	             TxEQ[29:27] = 001 : EQCode = -1/4 nominal value  
       	             TxEQ[29:27] = 010 : EQCode = -1/8 nominal value  
       	             TxEQ[29:27] = 101 : EQCode = +1/4 nominal value  
       	             TxEQ[29:27] = 110 : EQCode = +1/8 nominal value  
       
                     Bits [31:30] are used to select between a 5, 3, or 1 
       multiplication factor for use in VMSE/DDR4: 
       
       	             TxEQ[31:30] = 00 : 5 * (comp + 16) / 16
       	             TxEQ[31:30] = 11 : 3 * (comp + 16) / 16
       	             TxEQ[31:30] = 01 : 1 * (comp + 16) / 16
       	             TxEQ[31:30] = 10 : 1 * (comp + 16) / 16
       
       
                            When TxEQOverride = 1, bits [31:27] are used directly as 
       the EQ Code: 
       
                     Write Equalization codes.  5 bit control, but only 24 of them are 
       meaningful.  
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CMD Controls 1
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    UINT32 internalvrefen : 1;
    /* internalvrefen - Bits[2:2], RW_L, default = 1'b0 
       Enable internal Vref for parity error pin, ACIOLB, or DcDetect (only for CTL)
     */
    UINT32 ddrcrimodeen : 1;
    /* ddrcrimodeen - Bits[3:3], RW_L, default = 1'b0 
       Imode en
     */
    UINT32 ddrcrimodesel : 1;
    /* ddrcrimodesel - Bits[4:4], RW_L, default = 1'b0 
       Imode sel, 0: swing boost; 1: xtalk cancellation.
     */
    UINT32 ddrcrimodesrcntl : 6;
    /* ddrcrimodesrcntl - Bits[10:5], RW_L, default = 6'b0 
       Imode Scomp configuration
     */
    UINT32 ddrcrimodesrcntlovrd : 1;
    /* ddrcrimodesrcntlovrd - Bits[11:11], RW_L, default = 1'b0 
       Imode scomp override enable
     */
    UINT32 ddrcrimodecoef : 4;
    /* ddrcrimodecoef - Bits[15:12], RW_L, default = 1'b0 
       Imode coef, swing boost configure
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_20 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[21:21], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 dqstatdflt : 2;
    /* dqstatdflt - Bits[23:22], RW_L, default = 2'b11 
       Static leg enable
     */
    UINT32 odtstatdflt : 2;
    /* odtstatdflt - Bits[25:24], RW_L, default = None 
       Static leg enable
     */
    UINT32 txeq : 6;
    /* txeq - Bits[31:26], RW_L, default = 6'b0 
        Modified in BDX!! When TxEQOverride = 0, bits [29:27] are used as selects for 
       the RCOMP -> EQCODE calculation: 
       
       	             TxEQ[29:27] = 000 : EQCode = 0
       	             TxEQ[29:27] = 001 : EQCode = -1/4 nominal value  
       	             TxEQ[29:27] = 010 : EQCode = -1/8 nominal value  
       	             TxEQ[29:27] = 101 : EQCode = +1/4 nominal value  
       	             TxEQ[29:27] = 110 : EQCode = +1/8 nominal value  
       
                     Bits [31:30] are used to select between a 5, 3, or 1 
       multiplication factor for use in VMSE/DDR4: 
       
       	             TxEQ[31:30] = 00 : 5 * (comp + 16) / 16
       	             TxEQ[31:30] = 11 : 3 * (comp + 16) / 16
       	             TxEQ[31:30] = 01 : 1 * (comp + 16) / 16
       	             TxEQ[31:30] = 10 : 1 * (comp + 16) / 16
       
       
                            When TxEQOverride = 1, bits [31:27] are used directly as 
       the EQ Code: 
       
                     Write Equalization codes.  5 bit control, but only 24 of them are 
       meaningful.  
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */







/* DDRCRCTLPICODING_CTL_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       HSX (0x401A7E18)                                                     */
/*       BDX (0x401A7E18)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING_CTL_MCIO_DDRIOEXTMCC_REG 0x0D054E18

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CTL Pi Settings
 */
typedef union {
  struct {
    UINT32 ctlpicode0 : 6;
    /* ctlpicode0 - Bits[5:0], RW_L, default = 6'b0 
       Ctl Pi Code0
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay0 : 2;
    /* ctlpilogicdelay0 - Bits[7:6], RW_L, default = 2'b0 
       Ctl Pi Logic Delay0 - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode1 : 6;
    /* ctlpicode1 - Bits[13:8], RW_L, default = 6'b0 
       Ctl Pi Code1
     */
    UINT32 ctlpilogicdelay1 : 2;
    /* ctlpilogicdelay1 - Bits[15:14], RW_L, default = 2'b0 
       Ctl Pi Logic Delay1 - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode2 : 6;
    /* ctlpicode2 - Bits[21:16], RW_L, default = 6'b0 
       Ctl Pi Code2
     */
    UINT32 ctlpilogicdelay2 : 2;
    /* ctlpilogicdelay2 - Bits[23:22], RW_L, default = 2'b0 
       Ctl Pi Logic Delay2 - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode3 : 6;
    /* ctlpicode3 - Bits[29:24], RW_L, default = 6'b0 
       Ctl Pi Code3
     */
    UINT32 ctlpilogicdelay3 : 2;
    /* ctlpilogicdelay3 - Bits[31:30], RW_L, default = 2'b0 
       Ctl Pi Logic Delay3 - add 1~3 qclk cycles delay.
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLPICODING_CTL_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       HSX (0x401A7E1C)                                                     */
/*       BDX (0x401A7E1C)                                                     */
/* Register default value:              0x10010000                            */
#define DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXTMCC_REG 0x0D054E1C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * CTL Controls
 */
typedef union {
  struct {
    UINT32 ddrphasextalkenableqnnnh : 1;
    /* ddrphasextalkenableqnnnh - Bits[0:0], RW_L, default = 1'b0 
       When Phased based xtalk cancellation is enabled, set this to delay cmd to 
       maintain constant Tcwl. 
     */
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider to enable xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider to enable xover 2to2 and 2to1 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       enable the extra flop stage in xover to help timing
     */
    UINT32 rsvd_4 : 17;
    UINT32 rxvref : 7;
    /* rxvref - Bits[27:21], RW_L, default = 7'b0 
       IOLB Vref Control. binary code from 1/3 Vdd to 2/3 Vdd with step size of 
       Vdd/192(??mV) 
     */
    UINT32 rsvd_28 : 1;
    UINT32 paritydisable : 1;
    /* paritydisable - Bits[29:29], RW_L, default = 1'b1 
       disable parity and par_err pins to save power when the populated dimm type does 
       not support parity 
     */
    UINT32 rsvd_30 : 1;
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CTL Controls
 */
typedef union {
  struct {
    UINT32 ddrpxcenable : 1;
    /* ddrpxcenable - Bits[0:0], RW_L, default = 1'b0 
       When Phased based xtalk cancellation is enabled, set this to delay cmd to 
       maintain constant Tcwl. 
     */
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider to enable xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider to enable xover 2to2 and 2to1 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       enable the extra flop stage in xover to help timing
     */
    UINT32 rsvd_4 : 13;
    UINT32 ctltxeq : 2;
    /* ctltxeq - Bits[18:17], RW_L, default = 2'b1 
       When LSB is set to 1, uses TxEQ register (DdrCrCmdControls1[31:27]) as the EQ 
       code rather than the  
       	             EQ code calculated from the CTL DRVUP comp code.
     */
    UINT32 rsvd_19 : 2;
    UINT32 rxvref : 7;
    /* rxvref - Bits[27:21], RW_L, default = 7'b0 
       IOLB Vref Control. binary code from 1/3 Vdd to 2/3 Vdd with step size of 
       Vdd/192(??mV) 
     */
    UINT32 sr_gate_2to2 : 1;
    /* sr_gate_2to2 - Bits[28:28], RW_L, default = 1'b0 
       Used to ungate trainreset into xover for 2:2 mode
     */
    UINT32 paritydisable : 1;
    /* paritydisable - Bits[29:29], RW_L, default = 1'b1 
       disable parity and par_err pins to save power when the populated dimm type does 
       not support parity 
     */
    UINT32 rsvd_30 : 1;
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A7E20)                                                     */
/*       BDX (0x401A7E20)                                                     */
/* Register default value on HSX:       0x00004FFF                            */
/* Register default value on BDX:       0xC0004FFF                            */
#define DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXTMCC_REG 0x0D054E20
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * CTL Ranks Used
 */
typedef union {
  struct {
    UINT32 ranken : 12;
    /* ranken - Bits[11:0], RW_L, default = 12'b111111111111 
       Enables output buffers for ctlbuf 0 to ctlbuf 11, PI clocks and output datapaths 
       for this rank 
     */
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[12:12], RW_L, default = 1'b0 
       level shift hold enable:
                            Holds the HV control values and power down the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rxbiasgcncomp : 2;
    /* rxbiasgcncomp - Bits[14:13], RW_L, default = 2'b10 
       used to compensate the GCN skew. The GCN skew information should be extracted by 
       reading and analyzing the final Rcomp code value. The target spec is to 
       categorize the GCN skew in 3 process condition (mainly slow, fast, typical). 
       (one example can be just reading the 2 MLS bit of Rcomp code and determine if 
       the GCN is slow, fast or typical). The gcncomp is used for CTLE. 
       00: gcn in typical corner
       11: gcn in slow corner
       10: gcn in typical corner
       01: gcn in fast corner
               
     */
    UINT32 rsvd_15 : 1;
    UINT32 imodebiashighcm : 1;
    /* imodebiashighcm - Bits[16:16], RW_L, default = 1'b0 
       Imode bias control
     */
    UINT32 rxbiassel : 1;
    /* rxbiassel - Bits[17:17], RW_L, default = 1'b0 
       Rx bias Sel
     */
    UINT32 ddrcrinvertpiclk : 12;
    /* ddrcrinvertpiclk - Bits[29:18], RW_L, default = 12'b0 
       Invert PiClk in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 phase
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[30:30], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CTL Ranks Used
 */
typedef union {
  struct {
    UINT32 ranken : 12;
    /* ranken - Bits[11:0], RW_L, default = 12'b111111111111 
       Enables output buffers for ctlbuf 0 to ctlbuf 11, PI clocks and output datapaths 
       for this rank 
     */
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[12:12], RW_L, default = 1'b0 
       level shift hold enable:
                            Holds the HV control values and power down the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rxbiasgcncomp : 2;
    /* rxbiasgcncomp - Bits[14:13], RW_L, default = 2'b10 
       used to compensate the GCN skew. The GCN skew information should be
                 extracted by reading and analyzing the final Rcomp code value. The 
       target spec 
                 is to categorize the GCN skew in 3 process condition (mainly slow, 
       fast, typical). 
                 (one example can be just reading the 2 MLS bit of Rcomp code and 
       determine if the 
                 GCN is slow, fast or typical). The gcncomp is used for CTLE.
                 11: gcn in slow corner
                 10: gcn in typical corner
                 01: gcn in fast corner
               
     */
    UINT32 rsvd_15 : 1;
    UINT32 imodebiashighcm : 1;
    /* imodebiashighcm - Bits[16:16], RW_L, default = 1'b0 
       Imode bias control
     */
    UINT32 rxbiassel : 1;
    /* rxbiassel - Bits[17:17], RW_L, default = 1'b0 
       Rx bias Sel
     */
    UINT32 ddrcrinvertpiclk : 12;
    /* ddrcrinvertpiclk - Bits[29:18], RW_L, default = 12'b0 
       Invert PiClk in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 phase
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[30:30], RW_L, default = 1'b1 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */







/* DDRCRCTLTRAINING_CTL_MCIO_DDRIOEXTMCC_REG supported on:                    */
/*       HSX (0x401A7E2C)                                                     */
/*       BDX (0x401A7E2C)                                                     */
/* Register default value:              0x00007000                            */
#define DDRCRCTLTRAINING_CTL_MCIO_DDRIOEXTMCC_REG 0x0D054E2C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * Ctl Training Register
 */
typedef union {
  struct {
    UINT32 trainingoffset : 6;
    /* trainingoffset - Bits[5:0], RW_L, default = 6'b0 
       Offset added to PiCode. Obtained via xover calibration such that ClkPi aligns 
       with grid clock when PiCode=0. 
     */
    UINT32 dcdetectmode : 1;
    /* dcdetectmode - Bits[6:6], RW_L, default = None 
       Enables VMSE init DcDetect mode
     */
    UINT32 rsvd_7 : 1;
    UINT32 xoverphasedet : 12;
    /* xoverphasedet - Bits[19:8], RO_V, default = 1'b0 
       Xover Phase Detect. Xover calibration feedback bits.
     */
    UINT32 ctldrvsegen : 3;
    /* ctldrvsegen - Bits[22:20], RW_L, default = 3'b000 
       drvsegdisable, or drvSegEn_b. 
     */
    UINT32 ckepddrvsegen : 3;
    /* ckepddrvsegen - Bits[25:23], RW_L, default = 3'b111 
        Controls the number of drive segments enabled during CKE powerdown 
     */
    UINT32 rsvd_26 : 6;
  } Bits;
  UINT32 Data;
} DDRCRCTLTRAINING_CTL_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * Ctl Training Register
 */
typedef union {
  struct {
    UINT32 trainingoffset : 6;
    /* trainingoffset - Bits[5:0], RW_L, default = 6'b0 
       Offset added to PiCode. Obtained via xover calibration such that ClkPi aligns 
       with grid clock when PiCode=0. 
     */
    UINT32 dcdetectmode : 1;
    /* dcdetectmode - Bits[6:6], RW_L, default = None 
       Enables VMSE init DcDetect mode
     */
    UINT32 rsvd_7 : 1;
    UINT32 xoverphasedet : 12;
    /* xoverphasedet - Bits[19:8], RO_V, default = 1'b0 
       Xover Phase Detect. Xover calibration feedback bits.
     */
    UINT32 ctldrvsegen : 3;
    /* ctldrvsegen - Bits[22:20], RW_L, default = 3'b000 
       drvsegdisable, or drvSegEn_b. 
     */
    UINT32 ckepddrvsegen : 3;
    /* ckepddrvsegen - Bits[25:23], RW_L, default = 3'b111 
        Controls the number of drive segments enabled during CKE powerdown 
     */
    UINT32 rsvd_26 : 6;
  } Bits;
  UINT32 Data;
} DDRCRCTLTRAINING_CTL_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A7E30)                                                     */
/*       BDX (0x401A7E30)                                                     */
/* Register default value on HSX:       0x00980000                            */
/* Register default value on BDX:       0x00180000                            */
#define DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXTMCC_REG 0x0D054E30
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.7.CFG.xml.
 * CMD Controls 1
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    UINT32 internalvrefen : 1;
    /* internalvrefen - Bits[2:2], RW_L, default = 1'b0 
       Enable internal Vref for parity error pin, ACIOLB, or DcDetect (only for CTL)
     */
    UINT32 ddrcrimodeen : 1;
    /* ddrcrimodeen - Bits[3:3], RW_L, default = 1'b0 
       Imode en
     */
    UINT32 ddrcrimodesel : 1;
    /* ddrcrimodesel - Bits[4:4], RW_L, default = 1'b0 
       Imode sel, 0: swing boost; 1: xtalk cancellation.
     */
    UINT32 ddrcrimodesrcntl : 6;
    /* ddrcrimodesrcntl - Bits[10:5], RW_L, default = 6'b0 
       Imode Scomp configuration
     */
    UINT32 ddrcrimodesrcntlovrd : 1;
    /* ddrcrimodesrcntlovrd - Bits[11:11], RW_L, default = 1'b0 
       Imode scomp override enable
     */
    UINT32 ddrcrimodecoef : 4;
    /* ddrcrimodecoef - Bits[15:12], RW_L, default = 1'b0 
       Imode coef, swing boost configure
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_20 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[21:21], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 dqstatdflt : 2;
    /* dqstatdflt - Bits[23:22], RW_L, default = 2'b11 
       Static leg enable
     */
    UINT32 odtstatdflt : 2;
    /* odtstatdflt - Bits[25:24], RW_L, default = None 
       Static leg enable
     */
    UINT32 rsvd_26 : 1;
    UINT32 txeq : 5;
    /* txeq - Bits[31:27], RW_L, default = 5'b0 
       When TxEQOverride = 0, bits [29:27] are used as selects for the RCOMP -> EQCODE 
       calculation: 
       
       	             TxEQ[29:27] = 000 : EQCode = 0
       	             TxEQ[29:27] = 001 : EQCode = -1/4 nominal value  
       	             TxEQ[29:27] = 010 : EQCode = -1/8 nominal value  
       	             TxEQ[29:27] = 101 : EQCode = +1/4 nominal value  
       	             TxEQ[29:27] = 110 : EQCode = +1/8 nominal value  
       
                     Bits [31:30] are used to select between a 5, 3, or 1 
       multiplication factor for use in VMSE/DDR4: 
       
       	             TxEQ[31:30] = 00 : 5 * (comp + 16) / 16
       	             TxEQ[31:30] = 11 : 3 * (comp + 16) / 16
       	             TxEQ[31:30] = 01 : 1 * (comp + 16) / 16
       	             TxEQ[31:30] = 10 : 1 * (comp + 16) / 16
       
       
                            When TxEQOverride = 1, bits [31:27] are used directly as 
       the EQ Code: 
       
                     Write Equalization codes.  5 bit control, but only 24 of them are 
       meaningful.  
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXTMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CMD Controls 1
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    UINT32 internalvrefen : 1;
    /* internalvrefen - Bits[2:2], RW_L, default = 1'b0 
       Enable internal Vref for parity error pin, ACIOLB, or DcDetect (only for CTL)
     */
    UINT32 ddrcrimodeen : 1;
    /* ddrcrimodeen - Bits[3:3], RW_L, default = 1'b0 
       Imode en
     */
    UINT32 ddrcrimodesel : 1;
    /* ddrcrimodesel - Bits[4:4], RW_L, default = 1'b0 
       Imode sel, 0: swing boost; 1: xtalk cancellation.
     */
    UINT32 ddrcrimodesrcntl : 6;
    /* ddrcrimodesrcntl - Bits[10:5], RW_L, default = 6'b0 
       Imode Scomp configuration
     */
    UINT32 ddrcrimodesrcntlovrd : 1;
    /* ddrcrimodesrcntlovrd - Bits[11:11], RW_L, default = 1'b0 
       Imode scomp override enable
     */
    UINT32 ddrcrimodecoef : 4;
    /* ddrcrimodecoef - Bits[15:12], RW_L, default = 1'b0 
       Imode coef, swing boost configure
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_20 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[21:21], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 dqstatdflt : 2;
    /* dqstatdflt - Bits[23:22], RW_L, default = 2'b11 
       Static leg enable
     */
    UINT32 odtstatdflt : 2;
    /* odtstatdflt - Bits[25:24], RW_L, default = None 
       Static leg enable
     */
    UINT32 txeq : 6;
    /* txeq - Bits[31:26], RW_L, default = 6'b0 
        Modified in BDX!! When TxEQOverride = 0, bits [29:27] are used as selects for 
       the RCOMP -> EQCODE calculation: 
       
       	             TxEQ[29:27] = 000 : EQCode = 0
       	             TxEQ[29:27] = 001 : EQCode = -1/4 nominal value  
       	             TxEQ[29:27] = 010 : EQCode = -1/8 nominal value  
       	             TxEQ[29:27] = 101 : EQCode = +1/4 nominal value  
       	             TxEQ[29:27] = 110 : EQCode = +1/8 nominal value  
       
                     Bits [31:30] are used to select between a 5, 3, or 1 
       multiplication factor for use in VMSE/DDR4: 
       
       	             TxEQ[31:30] = 00 : 5 * (comp + 16) / 16
       	             TxEQ[31:30] = 11 : 3 * (comp + 16) / 16
       	             TxEQ[31:30] = 01 : 1 * (comp + 16) / 16
       	             TxEQ[31:30] = 10 : 1 * (comp + 16) / 16
       
       
                            When TxEQOverride = 1, bits [31:27] are used directly as 
       the EQ Code: 
       
                     Write Equalization codes.  5 bit control, but only 24 of them are 
       meaningful.  
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXTMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCTLPICODING2_CTL_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A7E34)                                                     */
/*       BDX (0x401A7E34)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING2_CTL_MCIO_DDRIOEXTMCC_REG 0x0D054E34

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CTL Pi Settings
 */
typedef union {
  struct {
    UINT32 ctlpicode4 : 6;
    /* ctlpicode4 - Bits[5:0], RW_L, default = 6'b0 
       Ctl Pi Code4
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay4 : 2;
    /* ctlpilogicdelay4 - Bits[7:6], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode5 : 6;
    /* ctlpicode5 - Bits[13:8], RW_L, default = 6'b0 
       Ctl Pi Code5
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay5 : 2;
    /* ctlpilogicdelay5 - Bits[15:14], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode6 : 6;
    /* ctlpicode6 - Bits[21:16], RW_L, default = 6'b0 
       Ctl Pi Code6
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay6 : 2;
    /* ctlpilogicdelay6 - Bits[23:22], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode7 : 6;
    /* ctlpicode7 - Bits[29:24], RW_L, default = 6'b0 
       Ctl Pi Code7
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay7 : 2;
    /* ctlpilogicdelay7 - Bits[31:30], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLPICODING2_CTL_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCTLPICODING3_CTL_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A7E38)                                                     */
/*       BDX (0x401A7E38)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING3_CTL_MCIO_DDRIOEXTMCC_REG 0x0D054E38

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CTL Pi Settings
 */
typedef union {
  struct {
    UINT32 ctlpicode8 : 6;
    /* ctlpicode8 - Bits[5:0], RW_L, default = 6'b0 
       Ctl Pi Code8
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay8 : 2;
    /* ctlpilogicdelay8 - Bits[7:6], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode9 : 6;
    /* ctlpicode9 - Bits[13:8], RW_L, default = 6'b0 
       Ctl Pi Code9
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay9 : 2;
    /* ctlpilogicdelay9 - Bits[15:14], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode10 : 6;
    /* ctlpicode10 - Bits[21:16], RW_L, default = 6'b0 
       Ctl Pi Code10
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay10 : 2;
    /* ctlpilogicdelay10 - Bits[23:22], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} DDRCRCTLPICODING3_CTL_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* DDRCRCTLPICODING4_CTL_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       HSX (0x401A7E40)                                                     */
/*       BDX (0x401A7E40)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING4_CTL_MCIO_DDRIOEXTMCC_REG 0x0D054E40

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.7.CFG.xml.
 * CTL Pi Settings 4 
 */
typedef union {
  struct {
    UINT32 vmselogicdelay : 12;
    /* vmselogicdelay - Bits[11:0], RW_L, default = None 
       VmseLogicDelay - add 2x clock cycle delay per vcmd bit
     */
    UINT32 rsvd_12 : 2;
    UINT32 vmsephase1cmdlogicdelay : 12;
    /* vmsephase1cmdlogicdelay - Bits[25:14], RW_L, default = 12'h0 
       When this is set, add one Qclk delay to the Intel SMI 2 
                   phase1 VCMD in case of a polarity inversion in serializer.
                
     */
    UINT32 flipvmseserializerphase : 1;
    /* flipvmseserializerphase - Bits[26:26], RW_L, default = 1'b0 
       When this is set, flip the phase of the serializer for VMSE 1:1 mode.
                
     */
    UINT32 rsvd_27 : 5;
  } Bits;
  UINT32 Data;
} DDRCRCTLPICODING4_CTL_MCIO_DDRIOEXTMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SVID_MCIO_DDRIOEXTMCC_REG supported on:                                    */
/*       IVT_EP (0x2018F02C)                                                  */
/* Register default value:              0x8086                                */
#define SVID_MCIO_DDRIOEXTMCC_REG 0x0D05202C



/* SDID_MCIO_DDRIOEXTMCC_REG supported on:                                    */
/*       IVT_EP (0x2018F02E)                                                  */
/* Register default value:              0x0000                                */
#define SDID_MCIO_DDRIOEXTMCC_REG 0x0D05202E



/* PXPCAP_MCIO_DDRIOEXTMCC_REG supported on:                                  */
/*       IVT_EP (0x4018F040)                                                  */
/* Register default value:              0x00910010                            */
#define PXPCAP_MCIO_DDRIOEXTMCC_REG 0x0D054040



/* GDCRRXOFFSETSKEWRANK4567A0_MCIO_DDRIOEXTMCC_REG supported on:              */
/*       IVT_EP (0x2018F104)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567A0_MCIO_DDRIOEXTMCC_REG 0x0D052104



/* GDCRRXOFFSETCOMPN0AA0_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       IVT_EP (0x2018F108)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AA0_MCIO_DDRIOEXTMCC_REG 0x0D052108



/* GDCRRXOFFSETCOMPN0BA0_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       IVT_EP (0x2018F10C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BA0_MCIO_DDRIOEXTMCC_REG 0x0D05210C



/* GDCRRXOFFSETCOMPN1AA0_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       IVT_EP (0x2018F110)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AA0_MCIO_DDRIOEXTMCC_REG 0x0D052110



/* GDCRRXOFFSETCOMPN1BA0_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       IVT_EP (0x2018F114)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BA0_MCIO_DDRIOEXTMCC_REG 0x0D052114



/* GDCRRXOFFSETCOMPCFGA0_MCIO_DDRIOEXTMCC_REG supported on:                   */
/*       IVT_EP (0x2018F118)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGA0_MCIO_DDRIOEXTMCC_REG 0x0D052118



/* GDCRRXOFFSETCOMPSTATUSA0_MCIO_DDRIOEXTMCC_REG supported on:                */
/*       IVT_EP (0x2018F11C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSA0_MCIO_DDRIOEXTMCC_REG 0x0D05211C



/* GDCRTXDESKEWGROUP0LANE01A0_MCIO_DDRIOEXTMCC_REG supported on:              */
/*       IVT_EP (0x2018F120)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01A0_MCIO_DDRIOEXTMCC_REG 0x0D052120



/* GDCRTXDESKEWGROUP0LANE45A0_MCIO_DDRIOEXTMCC_REG supported on:              */
/*       IVT_EP (0x2018F124)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45A0_MCIO_DDRIOEXTMCC_REG 0x0D052124



/* GDCRRXDESKEWGROUP0LANE01A0_MCIO_DDRIOEXTMCC_REG supported on:              */
/*       IVT_EP (0x2018F128)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01A0_MCIO_DDRIOEXTMCC_REG 0x0D052128



/* GDCRRXDESKEWGROUP0LANE45A0_MCIO_DDRIOEXTMCC_REG supported on:              */
/*       IVT_EP (0x2018F12C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45A0_MCIO_DDRIOEXTMCC_REG 0x0D05212C



/* GDCRTXDESKEWGROUP0LANE23A0_MCIO_DDRIOEXTMCC_REG supported on:              */
/*       IVT_EP (0x2018F130)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23A0_MCIO_DDRIOEXTMCC_REG 0x0D052130



/* GDCRTXDESKEWGROUP0LANE67A0_MCIO_DDRIOEXTMCC_REG supported on:              */
/*       IVT_EP (0x2018F134)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67A0_MCIO_DDRIOEXTMCC_REG 0x0D052134



/* GDCRRXDESKEWGROUP0LANE23A0_MCIO_DDRIOEXTMCC_REG supported on:              */
/*       IVT_EP (0x2018F138)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23A0_MCIO_DDRIOEXTMCC_REG 0x0D052138



/* GDCRRXDESKEWGROUP0LANE67A0_MCIO_DDRIOEXTMCC_REG supported on:              */
/*       IVT_EP (0x2018F13C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67A0_MCIO_DDRIOEXTMCC_REG 0x0D05213C



/* GDCRTXDESKEWGROUP1NIB0A0_MCIO_DDRIOEXTMCC_REG supported on:                */
/*       IVT_EP (0x2018F140)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0A0_MCIO_DDRIOEXTMCC_REG 0x0D052140



/* GDCRTXDESKEWGROUP1NIB1A0_MCIO_DDRIOEXTMCC_REG supported on:                */
/*       IVT_EP (0x2018F144)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1A0_MCIO_DDRIOEXTMCC_REG 0x0D052144



/* GDCRRXDESKEWGROUP1NIB0A0_MCIO_DDRIOEXTMCC_REG supported on:                */
/*       IVT_EP (0x2018F148)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0A0_MCIO_DDRIOEXTMCC_REG 0x0D052148



/* GDCRRXDESKEWGROUP1NIB1A0_MCIO_DDRIOEXTMCC_REG supported on:                */
/*       IVT_EP (0x2018F14C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1A0_MCIO_DDRIOEXTMCC_REG 0x0D05214C



/* GDCRTXDESKEWGROUP2NIB0A0_MCIO_DDRIOEXTMCC_REG supported on:                */
/*       IVT_EP (0x2018F150)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0A0_MCIO_DDRIOEXTMCC_REG 0x0D052150



/* GDCRTXDESKEWGROUP2NIB1A0_MCIO_DDRIOEXTMCC_REG supported on:                */
/*       IVT_EP (0x2018F154)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1A0_MCIO_DDRIOEXTMCC_REG 0x0D052154



/* GDCRRXDESKEWGROUP2NIB0A0_MCIO_DDRIOEXTMCC_REG supported on:                */
/*       IVT_EP (0x2018F158)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0A0_MCIO_DDRIOEXTMCC_REG 0x0D052158



/* GDCRRXDESKEWGROUP2NIB1A0_MCIO_DDRIOEXTMCC_REG supported on:                */
/*       IVT_EP (0x2018F15C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1A0_MCIO_DDRIOEXTMCC_REG 0x0D05215C



/* GDCRTXDESKEWGROUP3NIB0A0_MCIO_DDRIOEXTMCC_REG supported on:                */
/*       IVT_EP (0x2018F160)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0A0_MCIO_DDRIOEXTMCC_REG 0x0D052160



/* GDCRTXDESKEWGROUP3NIB1A0_MCIO_DDRIOEXTMCC_REG supported on:                */
/*       IVT_EP (0x2018F164)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1A0_MCIO_DDRIOEXTMCC_REG 0x0D052164



/* GDCRRXDESKEWGROUP3NIB0A0_MCIO_DDRIOEXTMCC_REG supported on:                */
/*       IVT_EP (0x2018F168)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0A0_MCIO_DDRIOEXTMCC_REG 0x0D052168



/* GDCRRXDESKEWGROUP3NIB1A0_MCIO_DDRIOEXTMCC_REG supported on:                */
/*       IVT_EP (0x2018F16C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1A0_MCIO_DDRIOEXTMCC_REG 0x0D05216C







/* GDCRXTALKEQATTKRA0_MCIO_DDRIOEXTMCC_REG supported on:                      */
/*       IVT_EP (0x2018F178)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRA0_MCIO_DDRIOEXTMCC_REG 0x0D052178



/* GDCRDLLCFGA0_MCIO_DDRIOEXTMCC_REG supported on:                            */
/*       IVT_EP (0x2018F17C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGA0_MCIO_DDRIOEXTMCC_REG 0x0D05217C











#endif /* MCIO_DDRIOEXTMCC_h */
