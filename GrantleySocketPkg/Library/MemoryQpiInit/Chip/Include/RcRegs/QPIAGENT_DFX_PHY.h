/* Date Stamp: 8/23/2014 */

#ifndef QPIAGENT_DFX_PHY_h
#define QPIAGENT_DFX_PHY_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For all target CPUs:                                                       */
/* QPIAGENT_DFX_PHY_DEV 8                                                     */
/* QPIAGENT_DFX_PHY_FUN 5                                                     */

/* VID_QPIAGENT_DFX_PHY_REG supported on:                                     */
/*       IVT_EP (0x20145000)                                                  */
/*       IVT_EX (0x20145000)                                                  */
/*       HSX (0x20145000)                                                     */
/*       BDX (0x20145000)                                                     */
/* Register default value:              0x8086                                */
#define VID_QPIAGENT_DFX_PHY_REG 0x06042000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.5.CFG.xml.
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
} VID_QPIAGENT_DFX_PHY_STRUCT;
#endif /* ASM_INC */


/* DID_QPIAGENT_DFX_PHY_REG supported on:                                     */
/*       IVT_EP (0x20145002)                                                  */
/*       IVT_EX (0x20145002)                                                  */
/*       HSX (0x20145002)                                                     */
/*       BDX (0x20145002)                                                     */
/* Register default value on IVT_EP:    0x0E85                                */
/* Register default value on IVT_EX:    0x0E85                                */
/* Register default value on HSX:       0x2F85                                */
/* Register default value on BDX:       0x6F85                                */
#define DID_QPIAGENT_DFX_PHY_REG 0x06042002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.5.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6F85 
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
} DID_QPIAGENT_DFX_PHY_STRUCT;
#endif /* ASM_INC */


/* PCICMD_QPIAGENT_DFX_PHY_REG supported on:                                  */
/*       IVT_EP (0x20145004)                                                  */
/*       IVT_EX (0x20145004)                                                  */
/*       HSX (0x20145004)                                                     */
/*       BDX (0x20145004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_QPIAGENT_DFX_PHY_REG 0x06042004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.5.CFG.xml.
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
} PCICMD_QPIAGENT_DFX_PHY_STRUCT;
#endif /* ASM_INC */


/* PCISTS_QPIAGENT_DFX_PHY_REG supported on:                                  */
/*       IVT_EP (0x20145006)                                                  */
/*       IVT_EX (0x20145006)                                                  */
/*       HSX (0x20145006)                                                     */
/*       BDX (0x20145006)                                                     */
/* Register default value:              0x0010                                */
#define PCISTS_QPIAGENT_DFX_PHY_REG 0x06042006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.5.CFG.xml.
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
} PCISTS_QPIAGENT_DFX_PHY_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_QPIAGENT_DFX_PHY_REG supported on:                                     */
/*       IVT_EP (0x10145008)                                                  */
/*       IVT_EX (0x10145008)                                                  */
/*       HSX (0x10145008)                                                     */
/*       BDX (0x10145008)                                                     */
/* Register default value:              0x00                                  */
#define RID_QPIAGENT_DFX_PHY_REG 0x06041008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.5.CFG.xml.
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
} RID_QPIAGENT_DFX_PHY_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_QPIAGENT_DFX_PHY_REG supported on:                                  */
/*       IVT_EP (0x10145009)                                                  */
/*       IVT_EX (0x10145009)                                                  */
/*       HSX (0x10145009)                                                     */
/*       BDX (0x10145009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_QPIAGENT_DFX_PHY_REG 0x06041009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.5.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_QPIAGENT_DFX_PHY_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_QPIAGENT_DFX_PHY_REG supported on:                                  */
/*       IVT_EP (0x2014500A)                                                  */
/*       IVT_EX (0x2014500A)                                                  */
/*       HSX (0x2014500A)                                                     */
/*       BDX (0x2014500A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_QPIAGENT_DFX_PHY_REG 0x0604200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.5.CFG.xml.
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
} CCR_N1_QPIAGENT_DFX_PHY_STRUCT;
#endif /* ASM_INC */


/* CLSR_QPIAGENT_DFX_PHY_REG supported on:                                    */
/*       IVT_EP (0x1014500C)                                                  */
/*       IVT_EX (0x1014500C)                                                  */
/*       HSX (0x1014500C)                                                     */
/*       BDX (0x1014500C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_QPIAGENT_DFX_PHY_REG 0x0604100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.5.CFG.xml.
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
} CLSR_QPIAGENT_DFX_PHY_STRUCT;
#endif /* ASM_INC */


/* PLAT_QPIAGENT_DFX_PHY_REG supported on:                                    */
/*       IVT_EP (0x1014500D)                                                  */
/*       IVT_EX (0x1014500D)                                                  */
/*       HSX (0x1014500D)                                                     */
/*       BDX (0x1014500D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_QPIAGENT_DFX_PHY_REG 0x0604100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.5.CFG.xml.
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
} PLAT_QPIAGENT_DFX_PHY_STRUCT;
#endif /* ASM_INC */


/* HDR_QPIAGENT_DFX_PHY_REG supported on:                                     */
/*       IVT_EP (0x1014500E)                                                  */
/*       IVT_EX (0x1014500E)                                                  */
/*       HSX (0x1014500E)                                                     */
/*       BDX (0x1014500E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_QPIAGENT_DFX_PHY_REG 0x0604100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.5.CFG.xml.
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
} HDR_QPIAGENT_DFX_PHY_STRUCT;
#endif /* ASM_INC */


/* BIST_QPIAGENT_DFX_PHY_REG supported on:                                    */
/*       IVT_EP (0x1014500F)                                                  */
/*       IVT_EX (0x1014500F)                                                  */
/*       HSX (0x1014500F)                                                     */
/*       BDX (0x1014500F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_QPIAGENT_DFX_PHY_REG 0x0604100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.5.CFG.xml.
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
} BIST_QPIAGENT_DFX_PHY_STRUCT;
#endif /* ASM_INC */






/* CAPPTR_QPIAGENT_DFX_PHY_REG supported on:                                  */
/*       IVT_EP (0x10145034)                                                  */
/*       IVT_EX (0x10145034)                                                  */
/*       HSX (0x10145034)                                                     */
/*       BDX (0x10145034)                                                     */
/* Register default value on IVT_EP:    0x40                                  */
/* Register default value on IVT_EX:    0x40                                  */
/* Register default value on HSX:       0x00                                  */
/* Register default value on BDX:       0x40                                  */
#define CAPPTR_QPIAGENT_DFX_PHY_REG 0x06041034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.5.CFG.xml.
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
} CAPPTR_QPIAGENT_DFX_PHY_STRUCT;
#endif /* ASM_INC */


/* INTL_QPIAGENT_DFX_PHY_REG supported on:                                    */
/*       IVT_EP (0x1014503C)                                                  */
/*       IVT_EX (0x1014503C)                                                  */
/*       HSX (0x1014503C)                                                     */
/*       BDX (0x1014503C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_QPIAGENT_DFX_PHY_REG 0x0604103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.5.CFG.xml.
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
} INTL_QPIAGENT_DFX_PHY_STRUCT;
#endif /* ASM_INC */


/* INTPIN_QPIAGENT_DFX_PHY_REG supported on:                                  */
/*       IVT_EP (0x1014503D)                                                  */
/*       IVT_EX (0x1014503D)                                                  */
/*       HSX (0x1014503D)                                                     */
/*       BDX (0x1014503D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_QPIAGENT_DFX_PHY_REG 0x0604103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.5.CFG.xml.
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
} INTPIN_QPIAGENT_DFX_PHY_STRUCT;
#endif /* ASM_INC */


/* MINGNT_QPIAGENT_DFX_PHY_REG supported on:                                  */
/*       IVT_EP (0x1014503E)                                                  */
/*       IVT_EX (0x1014503E)                                                  */
/*       HSX (0x1014503E)                                                     */
/*       BDX (0x1014503E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_QPIAGENT_DFX_PHY_REG 0x0604103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.5.CFG.xml.
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
} MINGNT_QPIAGENT_DFX_PHY_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_QPIAGENT_DFX_PHY_REG supported on:                                  */
/*       IVT_EP (0x1014503F)                                                  */
/*       IVT_EX (0x1014503F)                                                  */
/*       HSX (0x1014503F)                                                     */
/*       BDX (0x1014503F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_QPIAGENT_DFX_PHY_REG 0x0604103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.5.CFG.xml.
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
} MAXLAT_QPIAGENT_DFX_PHY_STRUCT;
#endif /* ASM_INC */














/* PQ_CSR_IBCSR_QPIAGENT_DFX_PHY_REG supported on:                            */
/*       HSX (0x40145114)                                                     */
/*       BDX (0x40145114)                                                     */
/* Register default value:              0x00000000                            */
#define PQ_CSR_IBCSR_QPIAGENT_DFX_PHY_REG 0x06044114

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.5.CFG.xml.
 * Intel QPI IBIST Control and Status Register
 */
typedef union {
  struct {
    UINT32 rsvd : 29;
    /* rsvd - Bits[28:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_29 : 1;
    UINT32 lfsr1_reciprocal_en : 1;
    /* lfsr1_reciprocal_en - Bits[30:30], RWS_L, default = 1'b0 
       
       	0 - use ITU-T PRBS23 for LFSR1
       	1 - use reciprocal of ITU-T PRBS23 for LFSR1
             
     */
    UINT32 lfsr2_reciprocal_en : 1;
    /* lfsr2_reciprocal_en - Bits[31:31], RWS_L, default = 1'b0 
       
       	0 - use ITU-T PRBS23 for LFSR2
       	1 - use reciprocal of ITU-T PRBS23 for LFSR2
             
     */
  } Bits;
  UINT32 Data;
} PQ_CSR_IBCSR_QPIAGENT_DFX_PHY_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


















/* PQ_CSR_AFEIAR_QPIAGENT_DFX_PHY_REG supported on:                           */
/*       HSX (0x40145138)                                                     */
/*       BDX (0x40145138)                                                     */
/* Register default value:              0x00000000                            */
#define PQ_CSR_AFEIAR_QPIAGENT_DFX_PHY_REG 0x06044138

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.5.CFG.xml.
 * Intel QPI AFE Indirect Address Register
 */
typedef union {
  struct {
    UINT32 write_en : 1;
    /* write_en - Bits[0:0], RW1S_L, default = 1'b0 
       
       	1 - Write enable
       	Gets cleared once the write operation is complete
             
     */
    UINT32 hw_override_en : 1;
    /* hw_override_en - Bits[1:1], RW_L, default = 1'b0 
       
       	1 - Override enable
       	Hardware is locked out of IAR bus access until user clears this bit explicitly
             
     */
    UINT32 rsvd_2 : 2;
    UINT32 unit_id : 6;
    /* unit_id - Bits[9:4], RW_L, default = 6'b0 
       
       	Unit ID (example lane ID)
             
     */
    UINT32 register_id : 6;
    /* register_id - Bits[15:10], RW_L, default = 6'b0 
       
        Register ID within the unit addressed by the unit ID.  The following are the 
       supported values: 
       RX_CTLE0 = 0x4
       Note: for RX_CTLE0 wr_data bit 0 must be set to 1, ctle peak is written to bits 
       5:1, all other bits set to 0. 
       TXEQ_LVL0 = 0x16
       TXEQ_LVL1 = 0x17
       TXEQ_LVL2 = 0x18
       TXEQ_LVL3 = 0x19
       Note: For TXEQ_LVL[3-0] wr_data bits 5:0 hold the respective Transmit 
       Equalization coefficients, all other bits set to 0. 
       TXEQ_HALF_LVL0 = 0x27
       TXEQ_HALF_LVL1 = 0x28
       TXEQ_HALF_LVL2 = 0x29
       TXEQ_HALF_LVL3 = 0x2A
       Note: For TXEQ_HALF_LVL[3-0] wr_data bits 5:0 hold the respective Transmit 
       Equalization coefficients, all other bits set to 0. 
       TXEQ_LVL0_OVR = 0x23
       TXEQ_LVL1_OVR = 0x24
       TXEQ_LVL2_OVR = 0x25
       TXEQ_LVL3_OVR = 0x26
       Note: For TXEQ_LVL[3-0]_OVR wr_data bit 0 set to 1 to enable, override value is 
       written to bits 6:1, all other bits set to 0.  
             
     */
    UINT32 wr_data : 8;
    /* wr_data - Bits[23:16], RW_L, default = 8'b0 
       
       	Write data to be applied to the IAR addressed by unit and register IDs
             
     */
    UINT32 read_data : 8;
    /* read_data - Bits[31:24], RO_V, default = 8'b0 
       
       	Read data from the IAR addressed by unit and register IDs
             
     */
  } Bits;
  UINT32 Data;
} PQ_CSR_AFEIAR_QPIAGENT_DFX_PHY_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




















/* LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG supported on:                     */
/*       HSX (0x40145200)                                                     */
/*       BDX (0x40145200)                                                     */
/* Register default value:              0x00000000                            */
#define LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_REG 0x06044200

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.5.CFG.xml.
 * Load Bus control register
 */
typedef union {
  struct {
    UINT32 lbc_start : 1;
    /* lbc_start - Bits[0:0], RW_LB, default = 1'b0 
       
        The SA sets this bit when a new request is sent out to the load bus controller. 
       The Ack from the load bus controller resets this bit once the operation is 
       completed. 
               BDX_DELTA: CSR attribute changed to RWS_LB as well as logic 
       fixed-b272503 
             
     */
    UINT32 lbc_req : 2;
    /* lbc_req - Bits[2:1], RW_LB, default = 2'b0 
       
       	00: rsv
       	01: read
       	10: write
       	11: read-modify-write
               BDX_DELTA: CSR attribute changed to RWS_LB as well as logic 
       fixed-b272503 
             
     */
    UINT32 lbc_ld_sel : 6;
    /* lbc_ld_sel - Bits[8:3], RW_LB, default = 6'b0 
       
       	Selects the EP on the load bus or monitor bus.
               BDX_DELTA: CSR attribute changed to RWS_LB as well as logic 
       fixed-b272503 
             
     */
    UINT32 lbc_ln_sel : 21;
    /* lbc_ln_sel - Bits[29:9], RW_LB, default = 21'b0 
       
        For writes, reads, and read-modify-writes this acts as the selection of which 
       lanes have to go through the respective transaction.  
       	Note that for read this has to be one hot.
       	19 to 0 for data lanes
       	20 for fwdclk lane
               BDX_DELTA: CSR attribute changed to RWS_LB as well as logic 
       fixed-b272503 
             
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} LBC_PER_IOU_CONTROL_QPIAGENT_DFX_PHY_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_REG supported on:                        */
/*       HSX (0x40145204)                                                     */
/*       BDX (0x40145204)                                                     */
/* Register default value:              0x00000000                            */
#define LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_REG 0x06044204

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.5.CFG.xml.
 * Load Bus data register
 */
typedef union {
  struct {
    UINT32 lbc_data_valid : 1;
    /* lbc_data_valid - Bits[0:0], RW_LB, default = 1'b0 
       
       	The LBC sets this bit in a read transaction when the data is ready to be read.
               BDX_DELTA: CSR attribute changed to RWS_LB as well as logic 
       fixed-b272503 
             
     */
    UINT32 lbc_data : 14;
    /* lbc_data - Bits[14:1], RW_LB, default = 14'b0 
       
        Reads: This is the data that is read out from the particular lane with the 
       monitor bus. 
        Writes: This is the data which needs to be written into the selected lanes 
       using the load bus. 
        Read-modify-Writes: This is the data that needs to be used as an offset for the 
       RMW on the lanes specified by the lane select. 
               BDX_DELTA: CSR attribute changed to RWS_LB as well as logic 
       fixed-b272503 
             
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[30:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 lbc_double_tmr : 1;
    /* lbc_double_tmr - Bits[31:31], RWS_LB, default = 1'b0 
       
       	Set to 1 to double the load bus controller wait timer.
               BDX_DELTA: CSR attribute changed to RWS_LB as well as logic 
       fixed-b272503 
             
     */
  } Bits;
  UINT32 Data;
} LBC_PER_IOU_DATA_QPIAGENT_DFX_PHY_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* QPIPHPRIVPTV_QPIAGENT_DFX_PHY_REG supported on:                            */
/*       IVT_EP (0x40145238)                                                  */
/*       IVT_EX (0x40145238)                                                  */
/*       HSX (0x40145238)                                                     */
/*       BDX (0x40145238)                                                     */
/* Register default value on IVT_EP:    0x05954595                            */
/* Register default value on IVT_EX:    0x05954595                            */
/* Register default value on HSX:       0x50050BCE                            */
/* Register default value on BDX:       0x50050BCE                            */
#define QPIPHPRIVPTV_QPIAGENT_DFX_PHY_REG 0x06044238


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.5.CFG.xml.
 * Private Primary Timeout value
 */
typedef union {
  struct {
    UINT32 etoc : 4;
    /* etoc - Bits[3:0], RWS_L, default = 4'hE 
       
        "offset calibration time inside Detect.Fwdclk. This time is added with etocs to 
       obtain the total offset calibration time.Require time is 256K UI (109.2us in 2.4 
       GHz). So the total time is 136.5us in 2.4GHz 
       	(2 ^ value) * TSL UI "
             
     */
    UINT32 etocs : 4;
    /* etocs - Bits[7:4], RWS_L, default = 4'hC 
       
        "offset calibration supplement time inside Detect.Fwdclk. This time is added 
       with etoc to obtain the total offset calibration time. Require time is 256K UI 
       (27.3us in 2.4 GHz). So the total time is 136.5us in 2.4GHz 
       	(2 ^ value) * TSL UI "
             
     */
    UINT32 etclk_icomp : 4;
    /* etclk_icomp - Bits[11:8], RWS_L, default = 4'hB 
       
        Clock per lane ICOMP inside Detect.Fwdclk. Require time is 128 UI (13.65us in 
       2.4 GHz) 
       	(2 exp value) * TSL UI 
             
     */
    UINT32 rsvd_12 : 4;
    /* rsvd_12 - Bits[15:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 etl1_idle : 4;
    /* etl1_idle - Bits[19:16], RWS_L, default = 4'h5 
       
       	L1 idle time? Require time is 2KUI (214ns in 2.4 GHz) 
       	(2 exp value) * TSL UI
             
     */
    UINT32 rsvd_20 : 7;
    /* rsvd_20 - Bits[26:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tfclk_icomp_rst : 5;
    /* tfclk_icomp_rst - Bits[31:27], RWS_L, default = 5'hA 
       
        Min time is 60ns based on uniphy data. Min program value should be 9 for 9.6GT. 
        
       	Timer value is Value * 16 cclk
        VAL NOTE: This timer must be less than Tx Forward clock lane ICOMP time in 
       etclk_icomp in this register and ICOMP logic has enough time to converge. 
             
     */
  } Bits;
  UINT32 Data;
} QPIPHPRIVPTV_QPIAGENT_DFX_PHY_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPIPHPRIVPTV2_QPIAGENT_DFX_PHY_REG supported on:                           */
/*       IVT_EP (0x4014523C)                                                  */
/*       IVT_EX (0x4014523C)                                                  */
/*       HSX (0x4014523C)                                                     */
/*       BDX (0x4014523C)                                                     */
/* Register default value on IVT_EP:    0xA0646606                            */
/* Register default value on IVT_EX:    0xA0646606                            */
/* Register default value on HSX:       0xB4850D06                            */
/* Register default value on BDX:       0xB4850D06                            */
#define QPIPHPRIVPTV2_QPIAGENT_DFX_PHY_REG 0x0604423C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.5.CFG.xml.
 * Private Primary Timeout value 2
 */
typedef union {
  struct {
    UINT32 etts0bw : 4;
    /* etts0bw - Bits[3:0], RWS_L, default = 4'h6 
       
        TS0B bitlock wait time before Deskew logic start looking for header in 
       Polling.BitlockB. Default is 4KUI. 
       	(2 exp value) * TSL UI 
             
     */
    UINT32 rsvd_4 : 4;
    /* rsvd_4 - Bits[7:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 etdcc : 4;
    /* etdcc - Bits[11:8], RWS_L, default = 4'hD 
       
       	DCC wait time in Detect.Fwdclk. Require time is 8K UI (54.6us in 2.4 GHz) 
       	(2 ^ value) * TSL UI 
             
     */
    UINT32 rsvd_12 : 4;
    /* rsvd_12 - Bits[15:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 etdll : 4;
    /* etdll - Bits[19:16], RWS_L, default = 4'h5 
       
       	DLL lock time in Detect.Fwdclk. Require time is 2KUI (214ns in 2.4 GHz) 
       	(2 ^ value) * TSL UI 
             
     */
    UINT32 rsvd_20 : 3;
    /* rsvd_20 - Bits[22:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 bypass_cdet2 : 1;
    /* bypass_cdet2 - Bits[23:23], RWS, default = 1'b1 
       
       	1 - Bypass etcdet2 timer when ICOMP is not perform in Detect.Fwdclk state.
             
     */
    UINT32 etcdet1 : 4;
    /* etcdet1 - Bits[27:24], RWS, default = 4'h4 
       
        Rx clock detect timer 1. Eanble Rx clock detect in Uniphy when both etcdet1 and 
       etcdet2 timers are expired in Deteect.Fwdclk and COMP state 
       	(2 ^ value) * TSL UI 
             
     */
    UINT32 etcdet2 : 4;
    /* etcdet2 - Bits[31:28], RWS, default = 4'hB 
       
        Rx clock detect timer 2. Eanble Rx clock detect in Uniphy when both etcdet1 and 
       etcdet2 timers are expired in Deteect.Fwdclk and COMP state 
       	(2 ^ value) * TSL UI 
             
     */
  } Bits;
  UINT32 Data;
} QPIPHPRIVPTV2_QPIAGENT_DFX_PHY_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */










/* QPIPHPRIVC10_QPIAGENT_DFX_PHY_REG supported on:                            */
/*       IVT_EP (0x4014525C)                                                  */
/*       IVT_EX (0x4014525C)                                                  */
/*       HSX (0x4014525C)                                                     */
/*       BDX (0x4014525C)                                                     */
/* Register default value:              0x00000000                            */
#define QPIPHPRIVC10_QPIAGENT_DFX_PHY_REG 0x0604425C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.5.CFG.xml.
 * QPI Phy Private Control 10
 */
typedef union {
  struct {
    UINT32 oc_done : 20;
    /* oc_done - Bits[19:0], ROS_V, default = 20'b0 
       
        per lane OC done based on QIO inputs. Output from csr is not used. Look like 
       status bits to me 
             
     */
    UINT32 fast_icomp_done : 1;
    /* fast_icomp_done - Bits[20:20], RW1C_L, default = 1'b0 
       
        Fast ICOMP done. It is set by HW when ICOMP timer is expired in operation speed 
       initialization. Besides RW1C by SW, this bit also clear by HW when link speed or 
       TX clock pointer is changed.  
       	0 : wait for ICOMP timer to advance AINIT state
       	1:  bypass ICOMP timer
             
     */
    UINT32 slow_icomp_done : 1;
    /* slow_icomp_done - Bits[21:21], RW1C_L, default = 1'b0 
       
        Slow ICOMP done. It is set by HW when ICOMP timer is expired in slow speed 
       initialization. Besides RW1C by SW, this bit also clear by HW when link speed or 
       TX clock pointer is changed.  
       	0 : wait for ICOMP timer to advance AINIT state
       	1:  bypass ICOMP timer
             
     */
    UINT32 fast_dcc_done : 1;
    /* fast_dcc_done - Bits[22:22], RW1C_L, default = 1'b0 
       
        Fast DCC done. It is set by HW when DCC timer is expired in operation speed 
       initialization. Besides RW1C by SW, this bit also clear by HW when link speed or 
       RX clock pointer is changed.  
       	0 : wait for DCC timer to advance AINIT state
       	1:  bypass DCC timer
             
     */
    UINT32 slow_dcc_done : 1;
    /* slow_dcc_done - Bits[23:23], RW1C_L, default = 1'b0 
       
        Slow DCC done. It is set by HW when DCC timer is expired in slow speed 
       initialization. Besides RW1C by SW, this bit also clear by HW when link speed or 
       RX clock pointer is changed.  
       	0 : wait for DCC timer to advance AINIT state
       	1:  bypass DCC timer
             
     */
    UINT32 fast_oc_done : 1;
    /* fast_oc_done - Bits[24:24], RW1C_L, default = 1'b0 
       
        Fast OC done. It is set by HW when OC timer is expired in operation speed 
       initialization. Besides RW1C by SW, this bit also clear by HW when link speed or 
       RX clock pointer is changed.  
       	0 : wait for OC timer to advance AINIT state
       	1:  bypass OCC timer
             
     */
    UINT32 slow_oc_done : 1;
    /* slow_oc_done - Bits[25:25], RW1C_L, default = 1'b0 
       
        Slow OC done. It is set by HW when OC timer is expired in slow speed 
       initialization. Besides RW1C by SW, this bit also clear by HW when link speed or 
       RX clock pointer is changed.  
       	0 : wait for OC timer to advance AINIT state
       	1:  bypass OC timer
             
     */
    UINT32 ignore_fast_icomp_done : 1;
    /* ignore_fast_icomp_done - Bits[26:26], RWS_L, default = 1'b0 
       
        when set, ignore fast ICOMP done bit. It is used to force to wait for ICOMP 
       timer if byass ICOMP is not set in privc4. 
             
     */
    UINT32 ignore_slow_icomp_done : 1;
    /* ignore_slow_icomp_done - Bits[27:27], RWS_L, default = 1'b0 
       
        when set, ignore slow ICOMP done bit. It is used to force to wait for ICOMP 
       timer if byass ICOMP is not set in privc4. 
             
     */
    UINT32 ignore_fast_dcc_done : 1;
    /* ignore_fast_dcc_done - Bits[28:28], RWS_L, default = 1'b0 
       
        when set, ignore fast DCC done bit. It is used to force to wait for DCC timer 
       if byass DCC is not set in privc4. POR value is 1, not expect SW to set this to 
       0. 
             
     */
    UINT32 ignore_slow_dcc_done : 1;
    /* ignore_slow_dcc_done - Bits[29:29], RWS_L, default = 1'b0 
       
        when set, ignore slow DCC done bit. It is used to force to wait for DCC timer 
       if byass DCC is not set in privc4. POR value is 1, not expect SW to set this to 
       0. 
             
     */
    UINT32 ignore_fast_oc_done : 1;
    /* ignore_fast_oc_done - Bits[30:30], RWS_L, default = 1'b0 
       
        when set, ignore fast OC done bit. It is used to force to wait for OC timer if 
       byass OC is not set in privc4. 
             
     */
    UINT32 ignore_slow_oc_done : 1;
    /* ignore_slow_oc_done - Bits[31:31], RWS_L, default = 1'b0 
       
        when set, ignore slow OC done bit. It is used to force to wait for OC timer if 
       byass OC is not set in privc4. 
             
     */
  } Bits;
  UINT32 Data;
} QPIPHPRIVC10_QPIAGENT_DFX_PHY_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */








/* QPI_RESP_TXINV_CTL_QPIAGENT_DFX_PHY_REG supported on:                      */
/*       IVT_EP (0x401452D4)                                                  */
/*       IVT_EX (0x401452D4)                                                  */
/*       HSX (0x401452D4)                                                     */
/*       BDX (0x401452D4)                                                     */
/* Register default value:              0xCA000000                            */
#define QPI_RESP_TXINV_CTL_QPIAGENT_DFX_PHY_REG 0x060442D4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.5.CFG.xml.
 * QPI Response TX Inversion Control
 */
typedef union {
  struct {
    UINT32 lnen : 20;
    /* lnen - Bits[19:0], RWS_L, default = 20'b0 
       
       	error inject lane select?
             
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[23:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 start_sel : 4;
    /* start_sel - Bits[27:24], RWS_L, default = 4'hA 
       
       	Start selection function. 
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
    UINT32 stop_sel : 4;
    /* stop_sel - Bits[31:28], RWS_L, default = 4'hC 
       
       	Stop selection response function. 
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
} QPI_RESP_TXINV_CTL_QPIAGENT_DFX_PHY_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPI_RESP_CLKSTP_CTL_QPIAGENT_DFX_PHY_REG supported on:                     */
/*       IVT_EP (0x401452DC)                                                  */
/*       IVT_EX (0x401452DC)                                                  */
/*       HSX (0x401452DC)                                                     */
/*       BDX (0x401452DC)                                                     */
/* Register default value:              0xCA000000                            */
#define QPI_RESP_CLKSTP_CTL_QPIAGENT_DFX_PHY_REG 0x060442DC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.5.CFG.xml.
 * QPI Response Clock Stop control
 */
typedef union {
  struct {
    UINT32 duration : 8;
    /* duration - Bits[7:0], RWS_L, default = 8'b0 
       
       	clock error injection duration
             
     */
    UINT32 pattern : 4;
    /* pattern - Bits[11:8], RWS_L, default = 4'b0 
       
       	clock error injection pattern
             
     */
    UINT32 rsvd_12 : 4;
    /* rsvd_12 - Bits[15:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 sel : 1;
    /* sel - Bits[16:16], RWS_L, default = 1'b0 
       
        another error injectionfor clock lane enable. Look like AND with the above 2 
       bits. 
             
     */
    UINT32 rsvd_17 : 7;
    /* rsvd_17 - Bits[23:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 start_sel : 4;
    /* start_sel - Bits[27:24], RWS_L, default = 4'hA 
       
       	Start selection function. 
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
    UINT32 stop_sel : 4;
    /* stop_sel - Bits[31:28], RWS_L, default = 4'hC 
       
       	Stop selection response function. 
       	0000: Micro Break Point 0 (MBP[0] 
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
} QPI_RESP_CLKSTP_CTL_QPIAGENT_DFX_PHY_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* SVID_QPIAGENT_DFX_PHY_REG supported on:                                    */
/*       IVT_EP (0x2014502C)                                                  */
/*       IVT_EX (0x2014502C)                                                  */
/* Register default value:              0x8086                                */
#define SVID_QPIAGENT_DFX_PHY_REG 0x0604202C



/* SDID_QPIAGENT_DFX_PHY_REG supported on:                                    */
/*       IVT_EP (0x2014502E)                                                  */
/*       IVT_EX (0x2014502E)                                                  */
/* Register default value:              0x0000                                */
#define SDID_QPIAGENT_DFX_PHY_REG 0x0604202E



/* PXPCAP_QPIAGENT_DFX_PHY_REG supported on:                                  */
/*       IVT_EP (0x40145040)                                                  */
/*       IVT_EX (0x40145040)                                                  */
/* Register default value:              0x00910010                            */
#define PXPCAP_QPIAGENT_DFX_PHY_REG 0x06044040



/* PXPENHCAP_QPIAGENT_DFX_PHY_REG supported on:                               */
/*       IVT_EP (0x40145100)                                                  */
/*       IVT_EX (0x40145100)                                                  */
/* Register default value:              0x00000000                            */
#define PXPENHCAP_QPIAGENT_DFX_PHY_REG 0x06044100



/* QPILTSSMST_QPIAGENT_DFX_PHY_REG supported on:                              */
/*       IVT_EP (0x40145104)                                                  */
/*       IVT_EX (0x40145104)                                                  */
/* Register default value:              0x00000000                            */
#define QPILTSSMST_QPIAGENT_DFX_PHY_REG 0x06044104



/* QPIRXINVST_QPIAGENT_DFX_PHY_REG supported on:                              */
/*       IVT_EP (0x40145108)                                                  */
/*       IVT_EX (0x40145108)                                                  */
/* Register default value:              0x00000000                            */
#define QPIRXINVST_QPIAGENT_DFX_PHY_REG 0x06044108



/* QPIRXMISCIN_QPIAGENT_DFX_PHY_REG supported on:                             */
/*       IVT_EP (0x4014510C)                                                  */
/*       IVT_EX (0x4014510C)                                                  */
/* Register default value:              0x00000000                            */
#define QPIRXMISCIN_QPIAGENT_DFX_PHY_REG 0x0604410C



/* QPI_DRIFT0_QPIAGENT_DFX_PHY_REG supported on:                              */
/*       IVT_EP (0x40145110)                                                  */
/*       IVT_EX (0x40145110)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_DRIFT0_QPIAGENT_DFX_PHY_REG 0x06044110



/* QPI_DRIFT1_QPIAGENT_DFX_PHY_REG supported on:                              */
/*       IVT_EP (0x40145114)                                                  */
/*       IVT_EX (0x40145114)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_DRIFT1_QPIAGENT_DFX_PHY_REG 0x06044114



/* QPI_DRIFT2_QPIAGENT_DFX_PHY_REG supported on:                              */
/*       IVT_EP (0x40145118)                                                  */
/*       IVT_EX (0x40145118)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_DRIFT2_QPIAGENT_DFX_PHY_REG 0x06044118



/* QPIRXST0_QPIAGENT_DFX_PHY_REG supported on:                                */
/*       IVT_EP (0x4014511C)                                                  */
/*       IVT_EX (0x4014511C)                                                  */
/* Register default value:              0x00000000                            */
#define QPIRXST0_QPIAGENT_DFX_PHY_REG 0x0604411C



/* QPIRXST1_QPIAGENT_DFX_PHY_REG supported on:                                */
/*       IVT_EP (0x40145120)                                                  */
/*       IVT_EX (0x40145120)                                                  */
/* Register default value:              0x00000000                            */
#define QPIRXST1_QPIAGENT_DFX_PHY_REG 0x06044120



/* QPIRXST2_QPIAGENT_DFX_PHY_REG supported on:                                */
/*       IVT_EP (0x40145124)                                                  */
/*       IVT_EX (0x40145124)                                                  */
/* Register default value:              0x00000000                            */
#define QPIRXST2_QPIAGENT_DFX_PHY_REG 0x06044124



/* QPI_TXLNDROP_CAUSE_QPIAGENT_DFX_PHY_REG supported on:                      */
/*       IVT_EP (0x40145128)                                                  */
/*       IVT_EX (0x40145128)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_TXLNDROP_CAUSE_QPIAGENT_DFX_PHY_REG 0x06044128



/* QPI_TXLNDROP_CAUSE1_QPIAGENT_DFX_PHY_REG supported on:                     */
/*       IVT_EP (0x4014512C)                                                  */
/*       IVT_EX (0x4014512C)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_TXLNDROP_CAUSE1_QPIAGENT_DFX_PHY_REG 0x0604412C



/* QPI_RXLNDROP_CAUSE_QPIAGENT_DFX_PHY_REG supported on:                      */
/*       IVT_EP (0x40145130)                                                  */
/*       IVT_EX (0x40145130)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_RXLNDROP_CAUSE_QPIAGENT_DFX_PHY_REG 0x06044130



/* QPI_RXLNDROP_CAUSE1_QPIAGENT_DFX_PHY_REG supported on:                     */
/*       IVT_EP (0x40145134)                                                  */
/*       IVT_EX (0x40145134)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_RXLNDROP_CAUSE1_QPIAGENT_DFX_PHY_REG 0x06044134



/* QPIPHDELS2_QPIAGENT_DFX_PHY_REG supported on:                              */
/*       IVT_EP (0x40145154)                                                  */
/*       IVT_EX (0x40145154)                                                  */
/* Register default value:              0x00000000                            */
#define QPIPHDELS2_QPIAGENT_DFX_PHY_REG 0x06044154



/* CSI13PHDELS_QPIAGENT_DFX_PHY_REG supported on:                             */
/*       IVT_EP (0x40145158)                                                  */
/*       IVT_EX (0x40145158)                                                  */
/* Register default value:              0x00000000                            */
#define CSI13PHDELS_QPIAGENT_DFX_PHY_REG 0x06044158



/* CSIPHORNGLNSEL_QPIAGENT_DFX_PHY_REG supported on:                          */
/*       IVT_EP (0x4014515C)                                                  */
/*       IVT_EX (0x4014515C)                                                  */
/* Register default value:              0x036DB6DB                            */
#define CSIPHORNGLNSEL_QPIAGENT_DFX_PHY_REG 0x0604415C



/* QPILSMERR_QPIAGENT_DFX_PHY_REG supported on:                               */
/*       IVT_EP (0x40145180)                                                  */
/*       IVT_EX (0x40145180)                                                  */
/* Register default value:              0x00000000                            */
#define QPILSMERR_QPIAGENT_DFX_PHY_REG 0x06044180



/* QPILSMERRMASK_QPIAGENT_DFX_PHY_REG supported on:                           */
/*       IVT_EP (0x40145184)                                                  */
/*       IVT_EX (0x40145184)                                                  */
/* Register default value:              0x00000000                            */
#define QPILSMERRMASK_QPIAGENT_DFX_PHY_REG 0x06044184



/* QPIPHDBAERR_QPIAGENT_DFX_PHY_REG supported on:                             */
/*       IVT_EP (0x40145188)                                                  */
/*       IVT_EX (0x40145188)                                                  */
/* Register default value:              0x00000000                            */
#define QPIPHDBAERR_QPIAGENT_DFX_PHY_REG 0x06044188



/* QPIPHYSCRATCH_QPIAGENT_DFX_PHY_REG supported on:                           */
/*       IVT_EP (0x1014518C)                                                  */
/*       IVT_EX (0x1014518C)                                                  */
/* Register default value:              0x00                                  */
#define QPIPHYSCRATCH_QPIAGENT_DFX_PHY_REG 0x0604118C



/* QPIRXOVRESET_QPIAGENT_DFX_PHY_REG supported on:                            */
/*       IVT_EP (0x40145200)                                                  */
/*       IVT_EX (0x40145200)                                                  */
/* Register default value:              0x00000000                            */
#define QPIRXOVRESET_QPIAGENT_DFX_PHY_REG 0x06044200



/* QPIRXOVINVEN_QPIAGENT_DFX_PHY_REG supported on:                            */
/*       IVT_EP (0x40145204)                                                  */
/*       IVT_EX (0x40145204)                                                  */
/* Register default value:              0x00000000                            */
#define QPIRXOVINVEN_QPIAGENT_DFX_PHY_REG 0x06044204



/* QPI_DRFTRSTOVR_QPIAGENT_DFX_PHY_REG supported on:                          */
/*       IVT_EP (0x40145208)                                                  */
/*       IVT_EX (0x40145208)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_DRFTRSTOVR_QPIAGENT_DFX_PHY_REG 0x06044208



/* QPI_RDPTRADJOVR0_QPIAGENT_DFX_PHY_REG supported on:                        */
/*       IVT_EP (0x4014520C)                                                  */
/*       IVT_EX (0x4014520C)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_RDPTRADJOVR0_QPIAGENT_DFX_PHY_REG 0x0604420C



/* QPI_RDPTRADJOVR1_QPIAGENT_DFX_PHY_REG supported on:                        */
/*       IVT_EP (0x40145210)                                                  */
/*       IVT_EX (0x40145210)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_RDPTRADJOVR1_QPIAGENT_DFX_PHY_REG 0x06044210



/* QPIRXOVDESKEW_QPIAGENT_DFX_PHY_REG supported on:                           */
/*       IVT_EP (0x40145214)                                                  */
/*       IVT_EX (0x40145214)                                                  */
/* Register default value:              0x00000000                            */
#define QPIRXOVDESKEW_QPIAGENT_DFX_PHY_REG 0x06044214



/* QPI_DSKWPTROVR0_QPIAGENT_DFX_PHY_REG supported on:                         */
/*       IVT_EP (0x40145218)                                                  */
/*       IVT_EX (0x40145218)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_DSKWPTROVR0_QPIAGENT_DFX_PHY_REG 0x06044218



/* QPI_DSKWPTROVR1_QPIAGENT_DFX_PHY_REG supported on:                         */
/*       IVT_EP (0x4014521C)                                                  */
/*       IVT_EX (0x4014521C)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_DSKWPTROVR1_QPIAGENT_DFX_PHY_REG 0x0604421C



/* QPI_DSKWPTROVR2_QPIAGENT_DFX_PHY_REG supported on:                         */
/*       IVT_EP (0x40145220)                                                  */
/*       IVT_EX (0x40145220)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_DSKWPTROVR2_QPIAGENT_DFX_PHY_REG 0x06044220



/* QPI_DSKWPTROVR3_QPIAGENT_DFX_PHY_REG supported on:                         */
/*       IVT_EP (0x40145224)                                                  */
/*       IVT_EX (0x40145224)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_DSKWPTROVR3_QPIAGENT_DFX_PHY_REG 0x06044224



/* QPIRXOVMISC1_QPIAGENT_DFX_PHY_REG supported on:                            */
/*       IVT_EP (0x40145228)                                                  */
/*       IVT_EX (0x40145228)                                                  */
/* Register default value:              0x00000000                            */
#define QPIRXOVMISC1_QPIAGENT_DFX_PHY_REG 0x06044228



/* QPIRXOVILO_QPIAGENT_DFX_PHY_REG supported on:                              */
/*       IVT_EP (0x4014522C)                                                  */
/*       IVT_EX (0x4014522C)                                                  */
/* Register default value:              0x00000000                            */
#define QPIRXOVILO_QPIAGENT_DFX_PHY_REG 0x0604422C



/* QPIRXDETOVR_QPIAGENT_DFX_PHY_REG supported on:                             */
/*       IVT_EP (0x40145230)                                                  */
/*       IVT_EX (0x40145230)                                                  */
/* Register default value:              0x00000000                            */
#define QPIRXDETOVR_QPIAGENT_DFX_PHY_REG 0x06044230























/* QPIPH_PRIVC15_QPIAGENT_DFX_PHY_REG supported on:                           */
/*       IVT_EP (0x40145278)                                                  */
/*       IVT_EX (0x40145278)                                                  */
/* Register default value:              0x000EF000                            */
#define QPIPH_PRIVC15_QPIAGENT_DFX_PHY_REG 0x06044278



/* PRBS23STARTVAL_QPIAGENT_DFX_PHY_REG supported on:                          */
/*       IVT_EP (0x40145280)                                                  */
/*       IVT_EX (0x40145280)                                                  */
/* Register default value:              0x007FFFFF                            */
#define PRBS23STARTVAL_QPIAGENT_DFX_PHY_REG 0x06044280



/* QPIPH_PRIVC16_QPIAGENT_DFX_PHY_REG supported on:                           */
/*       IVT_EP (0x40145284)                                                  */
/*       IVT_EX (0x40145284)                                                  */
/* Register default value:              0x00080044                            */
#define QPIPH_PRIVC16_QPIAGENT_DFX_PHY_REG 0x06044284





/* QPI_LTSSM_TXOVR0_QPIAGENT_DFX_PHY_REG supported on:                        */
/*       IVT_EP (0x40145290)                                                  */
/*       IVT_EX (0x40145290)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_LTSSM_TXOVR0_QPIAGENT_DFX_PHY_REG 0x06044290



/* QPI_LTSSM_TXOVR1_QPIAGENT_DFX_PHY_REG supported on:                        */
/*       IVT_EP (0x40145294)                                                  */
/*       IVT_EX (0x40145294)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_LTSSM_TXOVR1_QPIAGENT_DFX_PHY_REG 0x06044294



/* QPI_LTSSM_TXOVR2_QPIAGENT_DFX_PHY_REG supported on:                        */
/*       IVT_EP (0x40145298)                                                  */
/*       IVT_EX (0x40145298)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_LTSSM_TXOVR2_QPIAGENT_DFX_PHY_REG 0x06044298



/* QPI_LTSSM_TXOVR3_QPIAGENT_DFX_PHY_REG supported on:                        */
/*       IVT_EP (0x4014529C)                                                  */
/*       IVT_EX (0x4014529C)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_LTSSM_TXOVR3_QPIAGENT_DFX_PHY_REG 0x0604429C



/* QPI_LTSSM_RXOVR0_QPIAGENT_DFX_PHY_REG supported on:                        */
/*       IVT_EP (0x401452A0)                                                  */
/*       IVT_EX (0x401452A0)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_LTSSM_RXOVR0_QPIAGENT_DFX_PHY_REG 0x060442A0



/* QPI_LTSSM_RXOVR1_QPIAGENT_DFX_PHY_REG supported on:                        */
/*       IVT_EP (0x401452A4)                                                  */
/*       IVT_EX (0x401452A4)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_LTSSM_RXOVR1_QPIAGENT_DFX_PHY_REG 0x060442A4



/* QPI_LTSSM_RXOVR2_QPIAGENT_DFX_PHY_REG supported on:                        */
/*       IVT_EP (0x401452A8)                                                  */
/*       IVT_EX (0x401452A8)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_LTSSM_RXOVR2_QPIAGENT_DFX_PHY_REG 0x060442A8



/* QPI_LTSSM_T1OVR0_QPIAGENT_DFX_PHY_REG supported on:                        */
/*       IVT_EP (0x401452AC)                                                  */
/*       IVT_EX (0x401452AC)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_LTSSM_T1OVR0_QPIAGENT_DFX_PHY_REG 0x060442AC



/* QPI_LTSSM_T1OVR1_QPIAGENT_DFX_PHY_REG supported on:                        */
/*       IVT_EP (0x401452B0)                                                  */
/*       IVT_EX (0x401452B0)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_LTSSM_T1OVR1_QPIAGENT_DFX_PHY_REG 0x060442B0



/* QPI_LTSSM_T2OVR0_QPIAGENT_DFX_PHY_REG supported on:                        */
/*       IVT_EP (0x401452B4)                                                  */
/*       IVT_EX (0x401452B4)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_LTSSM_T2OVR0_QPIAGENT_DFX_PHY_REG 0x060442B4



/* QPI_LTSSM_T2OVR1_QPIAGENT_DFX_PHY_REG supported on:                        */
/*       IVT_EP (0x401452B8)                                                  */
/*       IVT_EX (0x401452B8)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_LTSSM_T2OVR1_QPIAGENT_DFX_PHY_REG 0x060442B8



/* QPI_LTSSM_T3OVR_QPIAGENT_DFX_PHY_REG supported on:                         */
/*       IVT_EP (0x401452BC)                                                  */
/*       IVT_EX (0x401452BC)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_LTSSM_T3OVR_QPIAGENT_DFX_PHY_REG 0x060442BC



/* QPI_LTSSM_T4OVR_QPIAGENT_DFX_PHY_REG supported on:                         */
/*       IVT_EP (0x401452C0)                                                  */
/*       IVT_EX (0x401452C0)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_LTSSM_T4OVR_QPIAGENT_DFX_PHY_REG 0x060442C0



/* QPI_LTSSM_T6OVR_QPIAGENT_DFX_PHY_REG supported on:                         */
/*       IVT_EP (0x401452C4)                                                  */
/*       IVT_EX (0x401452C4)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_LTSSM_T6OVR_QPIAGENT_DFX_PHY_REG 0x060442C4



/* QPI_OL_TXOVR_QPIAGENT_DFX_PHY_REG supported on:                            */
/*       IVT_EP (0x401452C8)                                                  */
/*       IVT_EX (0x401452C8)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_OL_TXOVR_QPIAGENT_DFX_PHY_REG 0x060442C8



/* QPI_OL_RXOVR_QPIAGENT_DFX_PHY_REG supported on:                            */
/*       IVT_EP (0x401452CC)                                                  */
/*       IVT_EX (0x401452CC)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_OL_RXOVR_QPIAGENT_DFX_PHY_REG 0x060442CC



/* QPI_RESP_TXINV_CTL_LA_QPIAGENT_DFX_PHY_REG supported on:                   */
/*       IVT_EP (0x401452D0)                                                  */
/*       IVT_EX (0x401452D0)                                                  */
/* Register default value:              0xCA000000                            */
#define QPI_RESP_TXINV_CTL_LA_QPIAGENT_DFX_PHY_REG 0x060442D0



/* QPI_RESP_TXINVBITSEL_QPIAGENT_DFX_PHY_REG supported on:                    */
/*       IVT_EP (0x401452D8)                                                  */
/*       IVT_EX (0x401452D8)                                                  */
/* Register default value:              0x00000000                            */
#define QPI_RESP_TXINVBITSEL_QPIAGENT_DFX_PHY_REG 0x060442D8



/* QPIPHCTR_CURR_QPIAGENT_DFX_PHY_REG supported on:                           */
/*       IVT_EP (0x40145300)                                                  */
/*       IVT_EX (0x40145300)                                                  */
/* Register default value:              0x00002000                            */
#define QPIPHCTR_CURR_QPIAGENT_DFX_PHY_REG 0x06044300



/* QPIPHTDC_CURR_QPIAGENT_DFX_PHY_REG supported on:                           */
/*       IVT_EP (0x40145304)                                                  */
/*       IVT_EX (0x40145304)                                                  */
/* Register default value:              0x00000000                            */
#define QPIPHTDC_CURR_QPIAGENT_DFX_PHY_REG 0x06044304



/* QPIPHRDC_CURR_QPIAGENT_DFX_PHY_REG supported on:                           */
/*       IVT_EP (0x40145308)                                                  */
/*       IVT_EX (0x40145308)                                                  */
/* Register default value:              0x00000000                            */
#define QPIPHRDC_CURR_QPIAGENT_DFX_PHY_REG 0x06044308



/* QPIPHWCI_CURR_QPIAGENT_DFX_PHY_REG supported on:                           */
/*       IVT_EP (0x4014530C)                                                  */
/*       IVT_EX (0x4014530C)                                                  */
/* Register default value:              0x00000043                            */
#define QPIPHWCI_CURR_QPIAGENT_DFX_PHY_REG 0x0604430C



/* QPIPHPTV_CURR_QPIAGENT_DFX_PHY_REG supported on:                           */
/*       IVT_EP (0x40145310)                                                  */
/*       IVT_EX (0x40145310)                                                  */
/* Register default value:              0x00000102                            */
#define QPIPHPTV_CURR_QPIAGENT_DFX_PHY_REG 0x06044310



/* QPIPHSTV_CURR_QPIAGENT_DFX_PHY_REG supported on:                           */
/*       IVT_EP (0x40145314)                                                  */
/*       IVT_EX (0x40145314)                                                  */
/* Register default value:              0x88880808                            */
#define QPIPHSTV_CURR_QPIAGENT_DFX_PHY_REG 0x06044314



/* QPIPHLDC_CURR_QPIAGENT_DFX_PHY_REG supported on:                           */
/*       IVT_EP (0x40145318)                                                  */
/*       IVT_EX (0x40145318)                                                  */
/* Register default value:              0x00000804                            */
#define QPIPHLDC_CURR_QPIAGENT_DFX_PHY_REG 0x06044318



/* QPIPHPRT_CURR_QPIAGENT_DFX_PHY_REG supported on:                           */
/*       IVT_EP (0x4014531C)                                                  */
/*       IVT_EX (0x4014531C)                                                  */
/* Register default value:              0x00322808                            */
#define QPIPHPRT_CURR_QPIAGENT_DFX_PHY_REG 0x0604431C



/* QPIREUT_PH_CTR1_CURR_QPIAGENT_DFX_PHY_REG supported on:                    */
/*       IVT_EP (0x40145320)                                                  */
/*       IVT_EX (0x40145320)                                                  */
/* Register default value:              0x01000012                            */
#define QPIREUT_PH_CTR1_CURR_QPIAGENT_DFX_PHY_REG 0x06044320



/* QPIREUT_PH_CTR2_CURR_QPIAGENT_DFX_PHY_REG supported on:                    */
/*       IVT_EP (0x40145324)                                                  */
/*       IVT_EX (0x40145324)                                                  */
/* Register default value:              0x00000000                            */
#define QPIREUT_PH_CTR2_CURR_QPIAGENT_DFX_PHY_REG 0x06044324



/* QPIREUT_PH_CTRADP_CURR_QPIAGENT_DFX_PHY_REG supported on:                  */
/*       IVT_EP (0x40145328)                                                  */
/*       IVT_EX (0x40145328)                                                  */
/* Register default value:              0x00000004                            */
#define QPIREUT_PH_CTRADP_CURR_QPIAGENT_DFX_PHY_REG 0x06044328



#endif /* QPIAGENT_DFX_PHY_h */
