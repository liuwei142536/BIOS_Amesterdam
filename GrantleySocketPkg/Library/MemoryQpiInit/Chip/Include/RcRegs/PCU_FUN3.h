/* Date Stamp: 8/23/2014 */

#ifndef PCU_FUN3_h
#define PCU_FUN3_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* PCU_FUN3_IVT_DEV 10                                                        */
/* PCU_FUN3_IVT_FUN 3                                                         */
/* For HSX_HOST:                                                              */
/* PCU_FUN3_HSX_DEV 30                                                        */
/* PCU_FUN3_HSX_FUN 3                                                         */
/* For BDX_HOST:                                                              */
/* PCU_FUN3_BDX_DEV 30                                                        */
/* PCU_FUN3_BDX_FUN 3                                                         */

/* VID_PCU_FUN3_REG supported on:                                             */
/*       IVT_EP (0x20153000)                                                  */
/*       IVT_EX (0x20153000)                                                  */
/*       HSX (0x201F3000)                                                     */
/*       BDX (0x201F3000)                                                     */
/* Register default value:              0x8086                                */
#define VID_PCU_FUN3_REG 0x0A032000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
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
} VID_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* DID_PCU_FUN3_REG supported on:                                             */
/*       IVT_EP (0x20153002)                                                  */
/*       IVT_EX (0x20153002)                                                  */
/*       HSX (0x201F3002)                                                     */
/*       BDX (0x201F3002)                                                     */
/* Register default value on IVT_EP:    0x0EC3                                */
/* Register default value on IVT_EX:    0x0EC3                                */
/* Register default value on HSX:       0x2FC0                                */
/* Register default value on BDX:       0x6FC0                                */
#define DID_PCU_FUN3_REG 0x0A032002


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.3.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h2FC0 (HSX), 16'h6FC0 (BDX) 
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
} DID_PCU_FUN3_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PCICMD_PCU_FUN3_REG supported on:                                          */
/*       IVT_EP (0x20153004)                                                  */
/*       IVT_EX (0x20153004)                                                  */
/*       HSX (0x201F3004)                                                     */
/*       BDX (0x201F3004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_PCU_FUN3_REG 0x0A032004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
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
} PCICMD_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* PCISTS_PCU_FUN3_REG supported on:                                          */
/*       IVT_EP (0x20153006)                                                  */
/*       IVT_EX (0x20153006)                                                  */
/*       HSX (0x201F3006)                                                     */
/*       BDX (0x201F3006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_PCU_FUN3_REG 0x0A032006
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
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
} PCISTS_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* RID_PCU_FUN3_REG supported on:                                             */
/*       IVT_EP (0x10153008)                                                  */
/*       IVT_EX (0x10153008)                                                  */
/*       HSX (0x101F3008)                                                     */
/*       BDX (0x101F3008)                                                     */
/* Register default value:              0x00                                  */
#define RID_PCU_FUN3_REG 0x0A031008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
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
} RID_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_PCU_FUN3_REG supported on:                                          */
/*       IVT_EP (0x10153009)                                                  */
/*       IVT_EX (0x10153009)                                                  */
/*       HSX (0x101F3009)                                                     */
/*       BDX (0x101F3009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_PCU_FUN3_REG 0x0A031009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.3.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_PCU_FUN3_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_PCU_FUN3_REG supported on:                                          */
/*       IVT_EP (0x2015300A)                                                  */
/*       IVT_EX (0x2015300A)                                                  */
/*       HSX (0x201F300A)                                                     */
/*       BDX (0x201F300A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_PCU_FUN3_REG 0x0A03200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
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
} CCR_N1_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* CLSR_PCU_FUN3_REG supported on:                                            */
/*       IVT_EP (0x1015300C)                                                  */
/*       IVT_EX (0x1015300C)                                                  */
/*       HSX (0x101F300C)                                                     */
/*       BDX (0x101F300C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_PCU_FUN3_REG 0x0A03100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
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
} CLSR_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* PLAT_PCU_FUN3_REG supported on:                                            */
/*       IVT_EP (0x1015300D)                                                  */
/*       IVT_EX (0x1015300D)                                                  */
/*       HSX (0x101F300D)                                                     */
/*       BDX (0x101F300D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_PCU_FUN3_REG 0x0A03100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
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
} PLAT_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* HDR_PCU_FUN3_REG supported on:                                             */
/*       IVT_EP (0x1015300E)                                                  */
/*       IVT_EX (0x1015300E)                                                  */
/*       HSX (0x101F300E)                                                     */
/*       BDX (0x101F300E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_PCU_FUN3_REG 0x0A03100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
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
} HDR_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* BIST_PCU_FUN3_REG supported on:                                            */
/*       IVT_EP (0x1015300F)                                                  */
/*       IVT_EX (0x1015300F)                                                  */
/*       HSX (0x101F300F)                                                     */
/*       BDX (0x101F300F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_PCU_FUN3_REG 0x0A03100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
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
} BIST_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* CONFIG_TDP_NOMINAL_PCU_FUN3_REG supported on:                              */
/*       HSX (0x401F3010)                                                     */
/*       BDX (0x401F3010)                                                     */
/* Register default value:              0x00000000                            */
#define CONFIG_TDP_NOMINAL_PCU_FUN3_REG 0x0A034010

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 */
typedef union {
  struct {
    UINT32 tdp_ratio : 8;
    /* tdp_ratio - Bits[7:0], RO_V, default = 8'b00000000 
       Pcode set based on SKU and factors in SSKU/softbin and flex impact.
       This CSR is a mirror of MSR  (648h) CONFIG_TDP_NOMINAL.  Refer to this MSR for 
       field descriptions. 
     */
    UINT32 rsvd : 24;
    /* rsvd - Bits[31:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CONFIG_TDP_NOMINAL_PCU_FUN3_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SVID_PCU_FUN3_REG supported on:                                            */
/*       IVT_EP (0x2015302C)                                                  */
/*       IVT_EX (0x2015302C)                                                  */
/*       HSX (0x201F302C)                                                     */
/*       BDX (0x201F302C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_PCU_FUN3_REG 0x0A03202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * PCI Subsystem Vendor ID Register
 */
typedef union {
  struct {
    UINT16 subsystem_vendor_identification_number : 16;
    /* subsystem_vendor_identification_number - Bits[15:0], RWS_O, default = 16'h8086 
       The default value specifies Intel but can be set to any value once after reset.
     */
  } Bits;
  UINT16 Data;
} SVID_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* SDID_PCU_FUN3_REG supported on:                                            */
/*       IVT_EP (0x2015302E)                                                  */
/*       IVT_EX (0x2015302E)                                                  */
/*       HSX (0x201F302E)                                                     */
/*       BDX (0x201F302E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_PCU_FUN3_REG 0x0A03202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * PCI Subsystem device ID Register
 */
typedef union {
  struct {
    UINT16 subsystem_device_identification_number : 16;
    /* subsystem_device_identification_number - Bits[15:0], RWS_O, default = 16'b0000000000000000 
       Assigned by the subsystem vendor to uniquely identify the subsystem
     */
  } Bits;
  UINT16 Data;
} SDID_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_PCU_FUN3_REG supported on:                                          */
/*       IVT_EP (0x10153034)                                                  */
/*       IVT_EX (0x10153034)                                                  */
/*       HSX (0x101F3034)                                                     */
/*       BDX (0x101F3034)                                                     */
/* Register default value on IVT_EP:    0x80                                  */
/* Register default value on IVT_EX:    0x80                                  */
/* Register default value on HSX:       0x00                                  */
/* Register default value on BDX:       0x00                                  */
#define CAPPTR_PCU_FUN3_REG 0x0A031034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
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
} CAPPTR_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* INTL_PCU_FUN3_REG supported on:                                            */
/*       IVT_EP (0x1015303C)                                                  */
/*       IVT_EX (0x1015303C)                                                  */
/*       HSX (0x101F303C)                                                     */
/*       BDX (0x101F303C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_PCU_FUN3_REG 0x0A03103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
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
} INTL_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* INTPIN_PCU_FUN3_REG supported on:                                          */
/*       IVT_EP (0x1015303D)                                                  */
/*       IVT_EX (0x1015303D)                                                  */
/*       HSX (0x101F303D)                                                     */
/*       BDX (0x101F303D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_PCU_FUN3_REG 0x0A03103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
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
} INTPIN_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* MINGNT_PCU_FUN3_REG supported on:                                          */
/*       IVT_EP (0x1015303E)                                                  */
/*       IVT_EX (0x1015303E)                                                  */
/*       HSX (0x101F303E)                                                     */
/*       BDX (0x101F303E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_PCU_FUN3_REG 0x0A03103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
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
} MINGNT_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_PCU_FUN3_REG supported on:                                          */
/*       IVT_EP (0x1015303F)                                                  */
/*       IVT_EX (0x1015303F)                                                  */
/*       HSX (0x101F303F)                                                     */
/*       BDX (0x101F303F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_PCU_FUN3_REG 0x0A03103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
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
} MAXLAT_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* DEVHIDE0_PCU_FUN3_REG supported on:                                        */
/*       IVT_EP (0x40153040)                                                  */
/*       IVT_EX (0x40153040)                                                  */
/*       HSX (0x401F3040)                                                     */
/*       BDX (0x401F3040)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE0_PCU_FUN3_REG 0x0A034040
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * Device Hide Register in CSR space
 * The DEVHIDE[7-0] registers in device 16, 30, and 31 should be set the same.
 */
typedef union {
  struct {
    UINT32 dis_fn : 32;
    /* dis_fn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDE4, then it means that in device 
       5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE0_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* DEVHIDE1_PCU_FUN3_REG supported on:                                        */
/*       IVT_EP (0x40153044)                                                  */
/*       IVT_EX (0x40153044)                                                  */
/*       HSX (0x401F3044)                                                     */
/*       BDX (0x401F3044)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE1_PCU_FUN3_REG 0x0A034044
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 dis_fn : 32;
    /* dis_fn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDE4, then it means that in device 
       5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE1_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* DEVHIDE2_PCU_FUN3_REG supported on:                                        */
/*       IVT_EP (0x40153048)                                                  */
/*       IVT_EX (0x40153048)                                                  */
/*       HSX (0x401F3048)                                                     */
/*       BDX (0x401F3048)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE2_PCU_FUN3_REG 0x0A034048
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 dis_fn : 32;
    /* dis_fn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDE4, then it means that in device 
       5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE2_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* DEVHIDE3_PCU_FUN3_REG supported on:                                        */
/*       IVT_EP (0x4015304C)                                                  */
/*       IVT_EX (0x4015304C)                                                  */
/*       HSX (0x401F304C)                                                     */
/*       BDX (0x401F304C)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE3_PCU_FUN3_REG 0x0A03404C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 dis_fn : 32;
    /* dis_fn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDE4, then it means that in device 
       5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE3_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* DEVHIDE4_PCU_FUN3_REG supported on:                                        */
/*       IVT_EP (0x40153050)                                                  */
/*       IVT_EX (0x40153050)                                                  */
/*       HSX (0x401F3050)                                                     */
/*       BDX (0x401F3050)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE4_PCU_FUN3_REG 0x0A034050
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 dis_fn : 32;
    /* dis_fn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDE4, then it means that in device 
       5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE4_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* DEVHIDE5_PCU_FUN3_REG supported on:                                        */
/*       IVT_EP (0x40153054)                                                  */
/*       IVT_EX (0x40153054)                                                  */
/*       HSX (0x401F3054)                                                     */
/*       BDX (0x401F3054)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE5_PCU_FUN3_REG 0x0A034054
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 dis_fn : 32;
    /* dis_fn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDE4, then it means that in device 
       5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE5_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* DEVHIDE6_PCU_FUN3_REG supported on:                                        */
/*       IVT_EP (0x40153058)                                                  */
/*       IVT_EX (0x40153058)                                                  */
/*       HSX (0x401F3058)                                                     */
/*       BDX (0x401F3058)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE6_PCU_FUN3_REG 0x0A034058
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 dis_fn : 32;
    /* dis_fn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDE4, then it means that in device 
       5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE6_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* DEVHIDE7_PCU_FUN3_REG supported on:                                        */
/*       IVT_EP (0x4015305C)                                                  */
/*       IVT_EX (0x4015305C)                                                  */
/*       HSX (0x401F305C)                                                     */
/*       BDX (0x401F305C)                                                     */
/* Register default value:              0x00000000                            */
#define DEVHIDE7_PCU_FUN3_REG 0x0A03405C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * Device Hide Register in CSR space
 */
typedef union {
  struct {
    UINT32 dis_fn : 32;
    /* dis_fn - Bits[31:0], RW_LB, default = 32'b00000000000000000000000000000000 
       Disable Function(DisFn):
       A bit set in this register implies that the appropriate device function is not 
       enabled. For instance, if bit 5 is set in DEVHIDE4, then it means that in device 
       5, function 4 is disabled. 
     */
  } Bits;
  UINT32 Data;
} DEVHIDE7_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* CONFIG_TDP_CONTROL_PCU_FUN3_REG supported on:                              */
/*       HSX (0x401F3060)                                                     */
/*       BDX (0x401F3060)                                                     */
/* Register default value:              0x00000000                            */
#define CONFIG_TDP_CONTROL_PCU_FUN3_REG 0x0A034060

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * Rd/Wr register to allow platform SW to select TDP point and set lock via all 
 * three interfaces (MSR, MMIO and PECI/PCS). 
 * This CSR is a mirror of MSR  (64Bh) CONFIG_TDP_CONTROL.  Refer to this MSR for 
 * field descriptions. 
 */
typedef union {
  struct {
    UINT32 tdp_level : 2;
    /* tdp_level - Bits[1:0], RWS_L, default = 2'b00 
       Config TDP level selected
       0 = nominal TDP level (default)
       1 = Level from CONFIG_TDP_LEVEL_1
       2 = Level from CONFIG_TDP_LEVEL_2
       3 = reserved
     */
    UINT32 rsvd : 29;
    /* rsvd - Bits[30:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 config_tdp_lock : 1;
    /* config_tdp_lock - Bits[31:31], RWS_KL, default = 1'b0 
       Config TDP level select lock
       0 - unlocked.
       1 - locked till next reset.
       
     */
  } Bits;
  UINT32 Data;
} CONFIG_TDP_CONTROL_PCU_FUN3_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CONFIG_TDP_LEVEL1_N0_PCU_FUN3_REG supported on:                            */
/*       HSX (0x401F3070)                                                     */
/*       BDX (0x401F3070)                                                     */
/* Register default value:              0x00000000                            */
#define CONFIG_TDP_LEVEL1_N0_PCU_FUN3_REG 0x0A034070

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * Level 1 configurable TDP settings.
 * This CSR is a mirror of MSR  (649h) CONFIG_TDP_LEVEL1.  Refer to this MSR for 
 * field descriptions. 
 */
typedef union {
  struct {
    UINT32 pkg_tdp : 15;
    /* pkg_tdp - Bits[14:0], RO_V, default = 15'h0000 
       Power for this TDP level. Units defined in PACKAGE_POWER_SKU_MSR[PWR_UNIT]
       Similar to PACKAGE_POWER_SKU[PKG_TDP]
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tdp_ratio : 8;
    /* tdp_ratio - Bits[23:16], RO_V, default = 8'h00 
       TDP ratio for config tdp level 1.
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CONFIG_TDP_LEVEL1_N0_PCU_FUN3_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CONFIG_TDP_LEVEL1_N1_PCU_FUN3_REG supported on:                            */
/*       HSX (0x401F3074)                                                     */
/*       BDX (0x401F3074)                                                     */
/* Register default value:              0x00000000                            */
#define CONFIG_TDP_LEVEL1_N1_PCU_FUN3_REG 0x0A034074

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * Level 1 configurable TDP settings.
 * This CSR is a mirror of MSR  (649h) CONFIG_TDP_LEVEL1.  Refer to this MSR for 
 * field descriptions. 
 */
typedef union {
  struct {
    UINT32 pkg_max_pwr : 15;
    /* pkg_max_pwr - Bits[14:0], RO_V, default = 15'h0000 
       Max pkg power setting allowed for this config TDP level1. Higher values will be 
       clamped down to this value. 
       Units defined in PACKAGE_POWER_SKU_MSR[PWR_UNIT].
       Similar to PACKAGE_POWER_SKU[PKG_MAX_PWR].
     */
    UINT32 pkg_min_pwr : 16;
    /* pkg_min_pwr - Bits[30:15], RO_V, default = 16'h0000 
       Min pkg power setting allowed for this config TDP level. Lower values will be 
       clamped up to this value. 
       Units defined in PACKAGE_POWER_SKU_MSR[PWR_UNIT].
       Similar to PACKAGE_POWER_SKU[PKG_MIN_PWR].
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CONFIG_TDP_LEVEL1_N1_PCU_FUN3_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CONFIG_TDP_LEVEL2_N0_PCU_FUN3_REG supported on:                            */
/*       HSX (0x401F3078)                                                     */
/*       BDX (0x401F3078)                                                     */
/* Register default value:              0x00000000                            */
#define CONFIG_TDP_LEVEL2_N0_PCU_FUN3_REG 0x0A034078

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * Level 2 configurable TDP settings. On sku that does not support config tdp, 
 * these to registers will report out 0. 
 * This CSR is a mirror of MSR  (64Ah) CONFIG_TDP_LEVEL2.  Refer to this MSR for 
 * field descriptions. 
 *     
 */
typedef union {
  struct {
    UINT32 pkg_tdp : 15;
    /* pkg_tdp - Bits[14:0], RO_V, default = 15'h0000 
       Power for this TDP level. Units defined in PACKAGE_POWER_SKU_MSR[PWR_UNIT]
       Similar to PACKAGE_POWER_SKU[PKG_TDP].
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tdp_ratio : 8;
    /* tdp_ratio - Bits[23:16], RO_V, default = 8'h00 
       TDP ratio for level 2.
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CONFIG_TDP_LEVEL2_N0_PCU_FUN3_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CONFIG_TDP_LEVEL2_N1_PCU_FUN3_REG supported on:                            */
/*       HSX (0x401F307C)                                                     */
/*       BDX (0x401F307C)                                                     */
/* Register default value:              0x00000000                            */
#define CONFIG_TDP_LEVEL2_N1_PCU_FUN3_REG 0x0A03407C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * Level 2 configurable TDP settings. On sku that does not support config tdp, 
 * these to registers will report out 0. 
 * This CSR is a mirror of MSR  (64Ah) CONFIG_TDP_LEVEL2.  Refer to this MSR for 
 * field descriptions. 
 *     
 */
typedef union {
  struct {
    UINT32 pkg_max_pwr : 15;
    /* pkg_max_pwr - Bits[14:0], RO_V, default = 15'h0000 
       Max pkg power setting allowed for config TDP level 2. Higher values will be 
       clamped down to this value. 
       Units defined in PACKAGE_POWER_SKU_MSR[PWR_UNIT].
       Similar to PACKAGE_POWER_SKU[PKG_MAX_PWR].
     */
    UINT32 pkg_min_pwr : 16;
    /* pkg_min_pwr - Bits[30:15], RO_V, default = 16'h0000 
       Min pkg power setting allowed for this config TDP level 2. Lower values will be 
       clamped up to this value. 
       Units defined in PACKAGE_POWER_SKU_MSR[PWR_UNIT].
       Similar to PACKAGE_POWER_SKU[PKG_MIN_PWR].
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CONFIG_TDP_LEVEL2_N1_PCU_FUN3_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CAP_HDR_PCU_FUN3_REG supported on:                                         */
/*       IVT_EP (0x40153080)                                                  */
/*       IVT_EX (0x40153080)                                                  */
/*       HSX (0x401F3080)                                                     */
/*       BDX (0x401F3080)                                                     */
/* Register default value on IVT_EP:    0x01180009                            */
/* Register default value on IVT_EX:    0x01180009                            */
/* Register default value on HSX:       0x01200009                            */
/* Register default value on BDX:       0x01200009                            */
#define CAP_HDR_PCU_FUN3_REG 0x0A034080
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * This register is a Capability Header.It enumerates the CAPID registers 
 * available, and points to the next CAP_PTR. 
 */
typedef union {
  struct {
    UINT32 cap_id : 8;
    /* cap_id - Bits[7:0], RO_FW, default = 8'b00001001 
       This field has the value 1001b to identify the CAP_ID assigned by the PCI SIG 
       for vendor dependent capability pointers. 
     */
    UINT32 next_cap_ptr : 8;
    /* next_cap_ptr - Bits[15:8], RO_FW, default = 8'b00000000 
       This field is hardwired to 00h indicating the end of the capabilities linked 
       list. 
     */
    UINT32 capid_length : 8;
    /* capid_length - Bits[23:16], RO_FW, default = 8'b00100000 
       This field indicates the structure length including the header in Bytes.
     */
    UINT32 capid_version : 4;
    /* capid_version - Bits[27:24], RO_FW, default = 4'b0001 
       This field has the value 0001b to identify the first revision of the CAPID 
       register definition. 
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CAP_HDR_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* CAPID0_PCU_FUN3_REG supported on:                                          */
/*       IVT_EP (0x40153084)                                                  */
/*       IVT_EX (0x40153084)                                                  */
/*       HSX (0x401F3084)                                                     */
/*       BDX (0x401F3084)                                                     */
/* Register default value:              0x00000000                            */
#define CAPID0_PCU_FUN3_REG 0x0A034084


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.3.CFG.xml.
 * This register is a Capability Register used to expose feature support to BIOS 
 * for SKU differentiation. 
 */
typedef union {
  struct {
    UINT32 de_sktb2_up : 1;
    /* de_sktb2_up - Bits[0:0], RO_FW, default = 1'b0 
       SKU configuration indication to BIOS. Definition depends upon DE_SKTR1_EX bit 
       (EN\EP\EP 4S procssor vs EX processor) 
            EN,EP,EP 4S processor:      Indicates that device is a UP SKU, independent 
       of package 
            EX processor:   Indicates that this SKU is scalable, support determined by 
       CAPID[3:2] settings 
               
     */
    UINT32 de_sktb2_en : 1;
    /* de_sktb2_en - Bits[1:1], RO_FW, default = 1'b0 
        SKU configuration indication to BIOS. Definition depends upon DE_SKTR1_EX bit 
       (EN\EP\EP 4S procssor vs EX processor) 
       	    EN,EP,EP 4S processor:      B3 Package, EN 2S SKU
       	    EX processor:   Unused
       	
     */
    UINT32 de_sktr_ep2s : 1;
    /* de_sktr_ep2s - Bits[2:2], RO_FW, default = 1'b0 
       SKU configuration indication to BIOS. Definition depends upon DE_SKTR1_EX bit 
       (EN\EP\EP 4S procssor vs EX processor) 
       	    EN,EP, EP 4S processor:      SKT R, EP2S SKU
            EX processor:   When 0, indicates UP or 4S, depending upon CAPID0[3]; When 
       1, indicates 2S or 8S, depending upon CAPID0[3] 
               
     */
    UINT32 de_sktr_ep4s : 1;
    /* de_sktr_ep4s - Bits[3:3], RO_FW, default = 1'b0 
       SKU configuration indication to BIOS. Definition depends upon DE_SKTR1_EX bit 
       (EN\EP\EP 4S procssor vs EX processor) 
       	    EN,EP, EP 4S processor:      SKT R, EP2S SKU
            EX processor:   When 0, indicatest UP/2S, depending up CAPID0[2]; When 1, 
       indicates 4S/8S, depending upon CAPID0[2] 
               
     */
    UINT32 de_sktr1_ex : 1;
    /* de_sktr1_ex - Bits[4:4], RO_FW, default = 1'b0 
       Socket R1, EX processor
     */
    UINT32 rsvd_5 : 1;
    /* rsvd_5 - Bits[5:5], RO_FW, default = 1'b0 
       Reserved for future use
               
     */
    UINT32 rsvd_6 : 1;
    /* rsvd_6 - Bits[6:6], RO_FW, default = 1'b0 
       Reserved for future use
               
     */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], RO_FW, default = 1'b0 
       Reserved for future use
               
     */
    UINT32 prg_tdp_lim_en : 1;
    /* prg_tdp_lim_en - Bits[8:8], RO_FW, default = 1'b0 
       TURBO_POWER_LIMIT MSRs usage enabled 
     */
    UINT32 llc_way_en : 3;
    /* llc_way_en - Bits[11:9], RO_FW, default = 3'b000 
       Enable LLC ways in active Cbo slice
        Value    Cache size
        '000:          0.5 M (4 lower ways)
        '001:          1 M (8 lower ways)
        '010:          1.5 M (12 lower ways)
        '011:          2 M (16 lower ways)
        '100:          2.5M (20 lower ways)
     */
    UINT32 ht_dis : 1;
    /* ht_dis - Bits[12:12], RO_FW, default = 1'b0 
       Multi threading disabled 
     */
    UINT32 rsvd_13 : 1;
    /* rsvd_13 - Bits[13:13], RO_FW, default = 1'b0 
       Reserved for future use
               
     */
    UINT32 rsvd_14 : 1;
    /* rsvd_14 - Bits[14:14], RO_FW, default = 1'b0 
       Reserved for future use 
     */
    UINT32 vt_x3_en : 1;
    /* vt_x3_en - Bits[15:15], RO_FW, default = 1'b0 
       VT-x3 enabled
     */
    UINT32 rsvd_16 : 1;
    UINT32 vmx_dis : 1;
    /* vmx_dis - Bits[17:17], RO_FW, default = 1'b0 
       VMX disabled
     */
    UINT32 smx_dis : 1;
    /* smx_dis - Bits[18:18], RO_FW, default = 1'b0 
       SMX disabled
     */
    UINT32 lt_production : 1;
    /* lt_production - Bits[19:19], RO_FW, default = 1'b0 
       Intel Trusted Execution Technology support
        1. Intel TXT enable == SMX enable
        2. Intel TXT enable == FIT boot enable
        3. Intel TXT production
        Legal combination (assume 0/1 == disable/enable)
        SMX enable / FIT boot enable / Intel TXT Production -- Remark           
                0 / 0 / 0 -- Intel TXT is disabled 
                1 / 1 / 1 -- Intel TXT is enabled 
                other values reserved 
     */
    UINT32 lt_sx_en : 1;
    /* lt_sx_en - Bits[20:20], RO_FW, default = 1'b0 
       Intel TXT and FIT-boot Enable
     */
    UINT32 lt_smm_inhibit : 1;
    /* lt_smm_inhibit - Bits[21:21], RO_FW, default = 1'b0 
       LT for handling of SMI inhibit with opt-out SMM enabled
     */
    UINT32 tsc_deadline_dis : 1;
    /* tsc_deadline_dis - Bits[22:22], RO_FW, default = 1'b0 
       APIC timer last tick relative mode:
        Support for TSC Deadline disabled
     */
    UINT32 aes_dis : 1;
    /* aes_dis - Bits[23:23], RO_FW, default = 1'b0 
       AES disabled
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], RO_FW, default = 1'b0 
       Reserved for future use.
     */
    UINT32 xsaveopt_dis : 1;
    /* xsaveopt_dis - Bits[25:25], RO_FW, default = 1'b0 
       XSAVEOPT disabled.  XSAVEOPT will be disable if set.
     */
    UINT32 gsse256_dis : 1;
    /* gsse256_dis - Bits[26:26], RO_FW, default = 1'b0 
       All GSSE instructions and setting GSSE XFeatureEnabledMask[GSSE] disabled.  All 
       GSSE instructions will be disabled if set. 
     */
    UINT32 rsvd_27 : 1;
    /* rsvd_27 - Bits[27:27], RO_FW, default = 1'b0 
       Reserved for future use. 
     */
    UINT32 art_dis : 1;
    /* art_dis - Bits[28:28], RO_FW, default = 1'b0 
       SparDisable support for Always Running APIC Timer.
        ART (Always Running APIC Timer) function in the APIC (enable legacy timer) 
       disabled 
     */
    UINT32 peci_en : 1;
    /* peci_en - Bits[29:29], RO_FW, default = 1'b0 
       PECI to the processor 
     */
    UINT32 dcu_mode : 1;
    /* dcu_mode - Bits[30:30], RO_FW, default = 1'b0 
       DCU mode (as set by MSR (31h) DCU_MODE) enabled
        0:        normal
        1:        16K 1/2 size ECC mode
     */
    UINT32 pclmulq_dis : 1;
    /* pclmulq_dis - Bits[31:31], RO_FW, default = 1'b0 
       PCLMULQ instructions disabled
     */
  } Bits;
  UINT32 Data;
} CAPID0_PCU_FUN3_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * This register is a Capability Register used to expose feature support to BIOS 
 * for SKU differentiation. 
 */
typedef union {
  struct {
    UINT32 de_sktb2_up : 1;
    /* de_sktb2_up - Bits[0:0], RO_FW, default = 1'b0 
       Definition depends upon CAPID.[4] bit (EN, EP, EP 4S procssor vs EX processor).
            EN, EP, EP 4S processor:      Indicates that the device is a 1S SKU 
       (independent of package). 
            EX processor:   Indicates that the SKU is scalable, support determined by 
       CAPID[3:2] settings. 
               
     */
    UINT32 de_sktb2_en : 1;
    /* de_sktb2_en - Bits[1:1], RO_FW, default = 1'b0 
       Definition depends upon CAPID.[4] bit (EN, EP, EP 4S procssor vs EX processor).
       	EN, EP, EP 4S processor:      Socket B3, EN SKU
       	    EX processor:   Unused
       	
     */
    UINT32 de_sktr_ep2s : 1;
    /* de_sktr_ep2s - Bits[2:2], RO_FW, default = 1'b0 
       Definition depends upon CAPID.[4] bit (EN, EP, EP 4S procssor vs EX processor).
       	EN, EP, EP 4S processor:      Socket R3, EP SKU.
        EX processor:   When 0, indicates 1S or 4S, depending upon CAPID0.[3]; When 1, 
       indicates 2S or 8S, depending upon CAPID0.[3] 
               
     */
    UINT32 de_sktr_ep4s : 1;
    /* de_sktr_ep4s - Bits[3:3], RO_FW, default = 1'b0 
       Definition depends upon CAPID.[4] bit (EN, EP, EP 4S procssor vs EX processor).
       	EN, EP, EP 4S processor:      Socket R3, EP SKU.
        EX processor:   When 0, indicates 1S/2S, depending up CAPID0.[2]; When 1, 
       indicates 4S/8S, depending upon CAPID0.[2] 
               
     */
    UINT32 de_sktr1_ex : 1;
    /* de_sktr1_ex - Bits[4:4], RO_FW, default = 1'b0 
       "EX" processor if set, else it is either an "EN, EP, or EP4S".  Refer to 
       CAPID0.[3:0] for further details. 
     */
    UINT32 de_sktr_de1s : 1;
    /* de_sktr_de1s - Bits[5:5], RO_FW, default = 1'b0 
        BDX-DE platform
               
     */
    UINT32 rsvd_6 : 1;
    /* rsvd_6 - Bits[6:6], RO_FW, default = 1'b0 
       Reserved for future use
               
     */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], RO_FW, default = 1'b0 
       Reserved for future use
               
     */
    UINT32 prg_tdp_lim_en : 1;
    /* prg_tdp_lim_en - Bits[8:8], RO_FW, default = 1'b0 
       TURBO_POWER_LIMIT MSRs usage enabled 
     */
    UINT32 llc_way_en : 3;
    /* llc_way_en - Bits[11:9], RO_FW, default = 3'b000 
       Enable LLC ways in active Cbo slice
        Value    Cache size
        '000:          0.5 M (4 lower ways)
        '001:          1 M (8 lower ways)
        '010:          1.5 M (12 lower ways)
        '011:          2 M (16 lower ways)
        '100:          2.5M (20 lower ways)
     */
    UINT32 ht_dis : 1;
    /* ht_dis - Bits[12:12], RO_FW, default = 1'b0 
       Multi threading disabled 
     */
    UINT32 de_ns1s : 1;
    /* de_ns1s - Bits[13:13], RO_FW, default = 0'b0 
       To indicate if it is BDX-DE NS part
               
     */
    UINT32 rsvd_14 : 1;
    /* rsvd_14 - Bits[14:14], RO_FW, default = 1'b0 
       Reserved for future use 
     */
    UINT32 vt_x3_en : 1;
    /* vt_x3_en - Bits[15:15], RO_FW, default = 1'b0 
       VT-x3 enabled
     */
    UINT32 rsvd_16 : 1;
    UINT32 vmx_dis : 1;
    /* vmx_dis - Bits[17:17], RO_FW, default = 1'b0 
       VMX disabled
     */
    UINT32 smx_dis : 1;
    /* smx_dis - Bits[18:18], RO_FW, default = 1'b0 
       SMX disabled
     */
    UINT32 lt_production : 1;
    /* lt_production - Bits[19:19], RO_FW, default = 1'b0 
       Intel Trusted Execution Technology support
                0 - Intel TXT is disabled 
                1 - Intel TXT is enabled 
                
     */
    UINT32 lt_sx_en : 1;
    /* lt_sx_en - Bits[20:20], RO_FW, default = 1'b0 
       Intel TXT and FIT-boot Enable
     */
    UINT32 rsvd_21 : 1;
    /* rsvd_21 - Bits[21:21], RO_FW, default = 1'b0 
       BDW removed this LT_SMM_INHIBIT 
     */
    UINT32 rsvd_22 : 1;
    /* rsvd_22 - Bits[22:22], RO_FW, default = 1'b0 
       BDW removed this TSC_DEADLINE_DIS
     */
    UINT32 aes_dis : 1;
    /* aes_dis - Bits[23:23], RO_FW, default = 1'b0 
       AES disabled
     */
    UINT32 rsvd_24 : 1;
    /* rsvd_24 - Bits[24:24], RO_FW, default = 1'b0 
       Reserved for future use.
     */
    UINT32 rsvd_25 : 1;
    /* rsvd_25 - Bits[25:25], RO_FW, default = 1'b0 
       BDW removed XSAVEOPT_DIS .
     */
    UINT32 gsse256_dis : 1;
    /* gsse256_dis - Bits[26:26], RO_FW, default = 1'b0 
       All GSSE instructions and setting GSSE XFeatureEnabledMask[GSSE] disabled.  All 
       GSSE instructions will be disabled if set. 
     */
    UINT32 rsvd_27 : 1;
    /* rsvd_27 - Bits[27:27], RO_FW, default = 1'b0 
       Reserved for future use. 
     */
    UINT32 rsvd_28 : 1;
    /* rsvd_28 - Bits[28:28], RO_FW, default = 1'b0 
       BDW Removed the Fuse ART_DIS
     */
    UINT32 peci_en : 1;
    /* peci_en - Bits[29:29], RO_FW, default = 1'b0 
       PECI to the processor 
     */
    UINT32 dcu_mode : 1;
    /* dcu_mode - Bits[30:30], RO_FW, default = 1'b0 
       DCU mode 
        0:       32 KB
     */
    UINT32 pclmulq_dis : 1;
    /* pclmulq_dis - Bits[31:31], RO_FW, default = 1'b0 
       PCLMULQ instructions disabled
     */
  } Bits;
  UINT32 Data;
} CAPID0_PCU_FUN3_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* CAPID1_PCU_FUN3_REG supported on:                                          */
/*       IVT_EP (0x40153088)                                                  */
/*       IVT_EX (0x40153088)                                                  */
/*       HSX (0x401F3088)                                                     */
/*       BDX (0x401F3088)                                                     */
/* Register default value:              0x00000000                            */
#define CAPID1_PCU_FUN3_REG 0x0A034088


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.3.CFG.xml.
 * This register is a Capability Register used to expose feature support to BIOS 
 * for SKU differentiation. 
 */
typedef union {
  struct {
    UINT32 dca_en : 1;
    /* dca_en - Bits[0:0], RO_FW, default = 1'b0 
       DCA Enabled 
     */
    UINT32 core_ras_en : 1;
    /* core_ras_en - Bits[1:1], RO_FW, default = 1'b0 
       Data Poisoning, MCA recovery enabled
     */
    UINT32 pppe : 2;
    /* pppe - Bits[3:2], RO_FW, default = 2'b00 
       PPPE_ENABLE
               
     */
    UINT32 gv3_dis : 1;
    /* gv3_dis - Bits[4:4], RO_FW, default = 1'b0 
       GV3 disabled. Does not allow for the writing of the IA32_PERF_CONTROL register 
       in order to change ratios 
     */
    UINT32 pwrbits_dis : 1;
    /* pwrbits_dis - Bits[5:5], RO_FW, default = 1'b0 
       0b     Power features activated during reset
        1b     Power features (especially clock gating) are not activated
     */
    UINT32 cpu_hot_add_en : 1;
    /* cpu_hot_add_en - Bits[6:6], RO_FW, default = 1'b0 
       Intel TXT - CPU HOT ADD enabled
     */
    UINT32 x2apic_en : 1;
    /* x2apic_en - Bits[7:7], RO_FW, default = 1'b0 
       Enable Extended APIC support.
        When set enables the support of x2APIC (Extended APIC) in the core and uncore.  
        
     */
    UINT32 cdd : 1;
    /* cdd - Bits[8:8], RO_FW, default = 1'b0 
       0: Device enabled.  
        1: Device disabled.     
               
     */
    UINT32 qos_dis : 1;
    /* qos_dis - Bits[9:9], RO_FW, default = 1'b0 
       Disable Quality of Service
     */
    UINT32 rsvd_10 : 1;
    /* rsvd_10 - Bits[10:10], RO_FW, default = 1'b0 
       Reserved for future use.  
     */
    UINT32 rsvd_11 : 1;
    /* rsvd_11 - Bits[11:11], RO_FW, default = 1'b0 
       Reserved for future use.  
     */
    UINT32 rsvd_12 : 1;
    /* rsvd_12 - Bits[12:12], RO_FW, default = 1'b0 
       Reserved for future use.  
     */
    UINT32 rsvd_13 : 1;
    /* rsvd_13 - Bits[13:13], RO_FW, default = 1'b0 
       Reserved for future use.  
     */
    UINT32 rsvd_14 : 1;
    /* rsvd_14 - Bits[14:14], RO_FW, default = 1'b0 
       Reserved for future use.  
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], RO_FW, default = 1'b0 
       Reserved for future use.  
     */
    UINT32 storage_seg_sel : 1;
    /* storage_seg_sel - Bits[16:16], RO_FW, default = 0'b0 
       Indication about whether it is Storage Sku or Comms Sku in BDX-NS. 0 indicates Comms Sku and 1 indicates Storage Sku.  
     */
    UINT32 cpm_disabled : 1;
    /* cpm_disabled - Bits[17:17], RO_FW, default = 0'b0 
       Indication about whether CPM is disabled in BDX-NS sku.  
     */
    UINT32 gbe0_disabled : 1;
    /* gbe0_disabled - Bits[18:18], RO_FW, default = 0'b0 
       Indication about whether GBE0 is disabled in BDX-NS sku.  
     */
    UINT32 gbe1_disabled : 1;
    /* gbe1_disabled - Bits[19:19], RO_FW, default = 0'b0 
       Indication about whether GBE1 is disabled in BDX_NS sku.  
     */
    UINT32 rsvd_20 : 1;
    /* rsvd_20 - Bits[20:20], RO_FW, default = 1'b0 
       Reserved for future use.  
     */
    UINT32 rsvd_21 : 1;
    /* rsvd_21 - Bits[21:21], RO_FW, default = 1'b0 
       Reserved for future use.  
     */
    UINT32 cbdma_disabled : 1;
    /* cbdma_disabled - Bits[22:22], RO_FW, default = 0'b0 
       Indication about whether CBDMA is disabled in BDX-NS sku.  
     */
    UINT32 mem_pa_size : 3;
    /* mem_pa_size - Bits[25:23], RO_FW, default = 3'b000 
       Physical address size supported in the core low two bits (Assuming uncore is 44 
       by default) 
        000:     46 
        010:     44
        101:     36
        110:     40
        111:     39
        reserved 
               
     */
    UINT32 dmfc : 4;
    /* dmfc - Bits[29:26], RO_FW, default = 4'b0000 
       This field controls which values may be written to the Memory Frequency Select 
       field 6:4 of the Clocking Configuration registers (MCHBAR Offset C00h). Any 
       attempt to write an unsupported value will be ignored.   
       
       [3:3] - If set, over-clocking is supported and bits [2:0] are ignored. 
       [2:0] - Maximum allowed memory frequency.
                 3b111 - up to DDR-1066 (4 x 266)
                 3b110 - up to DDR-1333 (5 x 266)
                 3b101 - up to DDR-1600 (6 x 266)
                 3b100 - up to DDR-1866 (7 x 266)
                 3b011 - up to DDR-2133 (8 x 266)   
                 
     */
    UINT32 dis_mem_lt_support : 1;
    /* dis_mem_lt_support - Bits[30:30], RO_FW, default = 1'b0 
       Intel TXT support disabled
     */
    UINT32 dis_mem_mirror : 1;
    /* dis_mem_mirror - Bits[31:31], RO_FW, default = 1'b0 
       Disable memory channel mirroring mode. In the mirroring mode, the server 
       maintains two identical copies of all data in memory. The contents of branch 0 
       (ontaining channel 0/1) is duplicated in the DIMMs of branch 1 (containing 
       channel 2/3). In the event of an uncorrectable error in one of the copies, the 
       system can retrieve the mirrored copy of the data. The use of memory mirroring 
       means that only half of the installed memory is available to the operating 
       system. 
     */
  } Bits;
  UINT32 Data;
} CAPID1_PCU_FUN3_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CAPID2_PCU_FUN3_REG supported on:                                          */
/*       IVT_EP (0x4015308C)                                                  */
/*       IVT_EX (0x4015308C)                                                  */
/*       HSX (0x401F308C)                                                     */
/*       BDX (0x401F308C)                                                     */
/* Register default value:              0x00000000                            */
#define CAPID2_PCU_FUN3_REG 0x0A03408C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * This register is a Capability Register used to expose feature support to BIOS 
 * for SKU differentiation. 
 */
typedef union {
  struct {
    UINT32 pcie_disws : 1;
    /* pcie_disws - Bits[0:0], RO_FW, default = 1'b0 
       WS features such as graphics cards in PCIe gen 2 slots disabled.
     */
    UINT32 pcie_disx16 : 2;
    /* pcie_disx16 - Bits[2:1], RO_FW, default = 2'b00 
       PCIe x16 ports disabled (limit to x8's only)
     */
    UINT32 pcie_disxpdev : 10;
    /* pcie_disxpdev - Bits[12:3], RO_FW, default = 10'b0000000000 
       Specific PCIe port disabled (example: 2x20 (EP), 1x20(EN2), 2x20 (EN1) speed 
       supported here) 
     */
    UINT32 pcie_disdmi : 1;
    /* pcie_disdmi - Bits[13:13], RO_FW, default = 1'b0 
       DMI2 interface disabled
     */
    UINT32 pcie_disdma : 1;
    /* pcie_disdma - Bits[14:14], RO_FW, default = 1'b0 
       DMA engine and supporting functionality disabled
     */
    UINT32 pcie_dispcieg3 : 1;
    /* pcie_dispcieg3 - Bits[15:15], RO_FW, default = 1'b0 
       PCIe Gen 3 disabled
     */
    UINT32 pcie_dislt : 1;
    /* pcie_dislt - Bits[16:16], RO_FW, default = 1'b0 
       LT disabled
     */
    UINT32 pcie_disltsx : 1;
    /* pcie_disltsx - Bits[17:17], RO_FW, default = 1'b0 
       LTSX disabled
     */
    UINT32 pcie_disrol : 1;
    /* pcie_disrol - Bits[18:18], RO_FW, default = 1'b0 
       RAID-on-load disabled
     */
    UINT32 pcie_disntb : 1;
    /* pcie_disntb - Bits[19:19], RO_FW, default = 1'b0 
       NTB support disabled
     */
    UINT32 rsvd_20 : 3;
    UINT32 qpi_link0_dis : 1;
    /* qpi_link0_dis - Bits[23:23], RO_FW, default = 1'b0 
       When set Intel QPI link 0 will be disabled.
     */
    UINT32 qpi_link1_dis : 1;
    /* qpi_link1_dis - Bits[24:24], RO_FW, default = 1'b0 
       When set Intel QPI link 1 will be disabled.
     */
    UINT32 qpi_allowed_cfclk_ratio_dis : 5;
    /* qpi_allowed_cfclk_ratio_dis - Bits[29:25], RO_FW, default = 5'b00000 
       Allowed CFCLK ratio is 12,11,10,9, 8(default),7; One bit is allocated for each 
       supported ratio except 8 (the default ratio). Intel QPI transfer rate = 8 * 
       CFCLK. Bits are organized as r12_r11_r10_r9_r7 format. 0/1 --> ratio 
       supported/not_supported. Default ratio of 8 is always supported. 8 = 6.4GT/s, 9 
       = 7.2GT/s, 10 = 8.0GT/s 12 = 9.6GT/s.  
       Ex: 00000 ==> Supported ratio: 12,11,10,9,8(default),7; ratio not supported: 
       none  
       Ex: 00001 ==> Supported ratio: 12,11,10,9,8(default); ratio not supported: 7 
       Ex: 11111 ==> Supported ratio: 8(default); ratio not supported: 12,11,10,9,7
     */
    UINT32 qpi_link2_dis : 1;
    /* qpi_link2_dis - Bits[30:30], RO_FW, default = 1'b0 
       When set Intel QPI link 2 will be disabled.
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} CAPID2_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* CAPID3_PCU_FUN3_REG supported on:                                          */
/*       IVT_EP (0x40153090)                                                  */
/*       IVT_EX (0x40153090)                                                  */
/*       HSX (0x401F3090)                                                     */
/*       BDX (0x401F3090)                                                     */
/* Register default value:              0x00000000                            */
#define CAPID3_PCU_FUN3_REG 0x0A034090


#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.3.CFG.xml.
 * This register is a Capability Register used to expose feature support to BIOS
 * for SKU differentiation.
 */
typedef union {
  struct {
    UINT32 chn_disable : 4;
    /* chn_disable - Bits[3:0], RO_FW, default = 4'b0000
       Channel disabled. When set, the corresponding channel is disabled.
     */
    UINT32 disable_2_dpc : 1;
    /* disable_2_dpc - Bits[4:4], RO_FW, default = 1'b0
       2 DPC disabled.
     */
    UINT32 disable_3_dpc : 1;
    /* disable_3_dpc - Bits[5:5], RO_FW, default = 1'b0
       3 DPC disabled.
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[6:6], n/a, default = n/a
       Padding added by header generation tool.
     */
    UINT32 disable_8gbit_ddr3 : 1;
    /* disable_8gbit_ddr3 - Bits[7:7], RO_FW, default = 1'b0
       8Gb or higher disabled. When set, the address decode to the corresponding 8Gb or
       higher mapping is disabled. Note: LR-DIMM's logical device density is also
       limited to 8Gb when this bit is set.
     */
    UINT32 disable_4gbit_ddr3 : 1;
    /* disable_4gbit_ddr3 - Bits[8:8], RO_FW, default = 1'b0
       4Gb disabled. When set, the address decode to the corresponding 4Gb mapping is
       disabled. Note: LR-DIMM's logical device density is also limited to 4Gb when
       this bit is set.
     */
    UINT32 disable_qr_dimm : 1;
    /* disable_qr_dimm - Bits[9:9], RO_FW, default = 1'b0
       QR DIMM disabled. When set, CS signals for QR-DIMM in slot 0-1 is disabled.
       Note: some CS may have multiplexed with address signal to support extended
       addressing. The CS signal disabling is only applicable to CS not the being
       multiplexed with address.
     */
    UINT32 disable_ecc : 1;
    /* disable_ecc - Bits[10:10], RO_FW, default = 1'b0
       ECC disabled. When set, ECC is disabled.
     */
    UINT32 disable_dir : 1;
    /* disable_dir - Bits[11:11], RO_FW, default = 1'b0
       DIR disabled. When set, directory is disabled.
     */
    UINT32 disable_3n : 1;
    /* disable_3n - Bits[12:12], RO_FW, default = 1'b0
       3N disabled. When set, 3N mode under normal operation (excluding MRS) is
       disabled.
     */
    UINT32 disable_rdimm : 1;
    /* disable_rdimm - Bits[13:13], RO_FW, default = 1'b0
       RDIMM disabled. When set, RDIMM support is disabled.
     */
    UINT32 disable_udimm : 1;
    /* disable_udimm - Bits[14:14], RO_FW, default = 1'b0
       UDIMM disabled. When set, UDIMM support is disabled.
     */
    UINT32 disable_cltt : 1;
    /* disable_cltt - Bits[15:15], RO_FW, default = 1'b0
       CLTT disabled. When set, CLTT support is disabled.
     */
    UINT32 disable_lockstep : 1;
    /* disable_lockstep - Bits[16:16], RO_FW, default = 1'b0
       LOCKSTEP disabled. When set, channel lockstep operation is disabled.
     */
    UINT32 disable_sparing : 1;
    /* disable_sparing - Bits[17:17], RO_FW, default = 1'b0
       Sparing disabled. When set, rank sparing is disabled.
     */
    UINT32 disable_patrol_scrub : 1;
    /* disable_patrol_scrub - Bits[18:18], RO_FW, default = 1'b0
       Patrol scrub disabled. When set, rank patrol scrub is disabled.
     */
    UINT32 disable_extended_latency_dimm : 1;
    /* disable_extended_latency_dimm - Bits[19:19], RO_FW, default = 1'b0
       Extended latency DIMM disabled. When set, DIMM with extended latency is forced
       to CAS to be less than or equal to 14.
     */
    UINT32 disable_extended_addr_dimm : 1;
    /* disable_extended_addr_dimm - Bits[20:20], RO_FW, default = 1'b0
       Extended addressing DIMM disabled. When set, DIMM with extended addressing
       (MA[17/16] is forced to be zero when driving MA[17:16].
     */
    UINT32 disable_rol_or_adr : 1;
    /* disable_rol_or_adr - Bits[21:21], RO_FW, default = 1'b0
       RAID-On-LOAD disabled. When set, memory ignores ADR event.
     */
    UINT32 disable_smbus_wrt : 1;
    /* disable_smbus_wrt - Bits[22:22], RO_FW, default = 1'b0
       SMBUS write capability disabled. When set, SMBus write is disabled.
     */
    UINT32 disable_monroe : 1;
    /* disable_monroe - Bits[23:23], RO_FW, default = 1'b0
       Intel Dynamic Power Technology Disabled. When set, the MONROE_CHN_FORCE_SR
       register field in MCMTR and the channel MCMTR_SHDW becomes read-only.
     */
    UINT32 mc2gdfuse : 6;
    /* mc2gdfuse - Bits[29:24], RO_FW, default = 6'b000000
       Bit0:     1.35V DDR3L LVDDR disabled

     */
    UINT32 disable_mem_ddr4 : 1;
    /* disable_mem_ddr4 - Bits[30:30], RO_FW, default = 1'b0
       ddr4 disabled
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} CAPID3_PCU_FUN3_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.3.CFG.xml.
 * This register is a Capability Register used to expose feature support to BIOS 
 * for SKU differentiation. 
 */
typedef union {
  struct {
    UINT32 chn_disable : 4;
    /* chn_disable - Bits[3:0], RO_FW, default = 4'b0000 
       Channel disabled. When set, the corresponding channel is disabled. 
     */
    UINT32 disable_2_dpc : 1;
    /* disable_2_dpc - Bits[4:4], RO_FW, default = 1'b0 
       2 DPC disabled.
     */
    UINT32 disable_3_dpc : 1;
    /* disable_3_dpc - Bits[5:5], RO_FW, default = 1'b0 
       3 DPC disabled.
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[6:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 disable_8gbit_ddr3 : 1;
    /* disable_8gbit_ddr3 - Bits[7:7], RO_FW, default = 1'b0 
       8Gb or higher disabled. When set, the address decode to the corresponding 8Gb or 
       higher mapping is disabled. Note: LR-DIMM's logical device density is also 
       limited to 8Gb when this bit is set.  
     */
    UINT32 disable_4gbit_ddr3 : 1;
    /* disable_4gbit_ddr3 - Bits[8:8], RO_FW, default = 1'b0 
       4Gb disabled. When set, the address decode to the corresponding 4Gb mapping is 
       disabled. Note: LR-DIMM's logical device density is also limited to 4Gb when 
       this bit is set. 
     */
    UINT32 disable_qr_dimm : 1;
    /* disable_qr_dimm - Bits[9:9], RO_FW, default = 1'b0 
       QR DIMM disabled. When set, CS signals for QR-DIMM in slot 0-1 is disabled. 
       Note: some CS may have multiplexed with address signal to support extended 
       addressing. The CS signal disabling is only applicable to CS not the being 
       multiplexed with address.  
     */
    UINT32 disable_ecc : 1;
    /* disable_ecc - Bits[10:10], RO_FW, default = 1'b0 
       ECC disabled. When set, ECC is disabled. 
     */
    UINT32 disable_dir : 1;
    /* disable_dir - Bits[11:11], RO_FW, default = 1'b0 
       DIR disabled. When set, directory is disabled. 
     */
    UINT32 disable_3n : 1;
    /* disable_3n - Bits[12:12], RO_FW, default = 1'b0 
       3N disabled. When set, 3N mode under normal operation (excluding MRS) is 
       disabled.  
     */
    UINT32 disable_rdimm : 1;
    /* disable_rdimm - Bits[13:13], RO_FW, default = 1'b0 
       RDIMM disabled. When set, RDIMM support is disabled. 
     */
    UINT32 disable_udimm : 1;
    /* disable_udimm - Bits[14:14], RO_FW, default = 1'b0 
       UDIMM disabled. When set, UDIMM support is disabled. 
     */
    UINT32 disable_cltt : 1;
    /* disable_cltt - Bits[15:15], RO_FW, default = 1'b0 
       CLTT disabled. When set, CLTT support is disabled. 
     */
    UINT32 disable_lockstep : 1;
    /* disable_lockstep - Bits[16:16], RO_FW, default = 1'b0 
       LOCKSTEP disabled. When set, channel lockstep operation is disabled.
     */
    UINT32 disable_sparing : 1;
    /* disable_sparing - Bits[17:17], RO_FW, default = 1'b0 
       Sparing disabled. When set, rank sparing is disabled. 
     */
    UINT32 disable_patrol_scrub : 1;
    /* disable_patrol_scrub - Bits[18:18], RO_FW, default = 1'b0 
       Patrol scrub disabled. When set, rank patrol scrub is disabled.
     */
    UINT32 disable_extended_latency_dimm : 1;
    /* disable_extended_latency_dimm - Bits[19:19], RO_FW, default = 1'b0 
       Extended latency DIMM disabled. When set, DIMM with extended latency is forced 
       to CAS to be less than or equal to 14.  
     */
    UINT32 disable_extended_addr_dimm : 1;
    /* disable_extended_addr_dimm - Bits[20:20], RO_FW, default = 1'b0 
       Extended addressing DIMM disabled. When set, DIMM with extended addressing 
       (MA[17/16] is forced to be zero when driving MA[17:16].  
     */
    UINT32 disable_rol_or_adr : 1;
    /* disable_rol_or_adr - Bits[21:21], RO_FW, default = 1'b0 
       RAID-On-LOAD disabled. When set, memory ignores ADR event.
     */
    UINT32 disable_smbus_wrt : 1;
    /* disable_smbus_wrt - Bits[22:22], RO_FW, default = 1'b0 
       SMBUS write capability disabled. When set, SMBus write is disabled. 
     */
    UINT32 disable_monroe : 1;
    /* disable_monroe - Bits[23:23], RO_FW, default = 1'b0 
       Intel Dynamic Power Technology Disabled. When set, the MONROE_CHN_FORCE_SR 
       register field in MCMTR and the channel MCMTR_SHDW becomes read-only. 
     */
    UINT32 mc2gdfuse : 6;
    /* mc2gdfuse - Bits[29:24], RO_FW, default = 6'b000000 
       Bit0:     1.35V DDR3L LVDDR disabled
               
     */
    UINT32 disable_mem_ddr4 : 1;
    /* disable_mem_ddr4 - Bits[30:30], RO_FW, default = 1'b0 
       ddr4 disabled 
     */
    UINT32 uli_ulm_en : 1;
    /* uli_ulm_en - Bits[31:31], RO_FW, default = 1'b0  */
  } Bits;
  UINT32 Data;
} CAPID3_PCU_FUN3_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */


/* CAPID4_PCU_FUN3_REG supported on:                                          */
/*       IVT_EP (0x40153094)                                                  */
/*       IVT_EX (0x40153094)                                                  */
/*       HSX (0x401F3094)                                                     */
/*       BDX (0x401F3094)                                                     */
/* Register default value:              0x00000000                            */
#define CAPID4_PCU_FUN3_REG 0x0A034094


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.3.CFG.xml.
 * This register is a Capability Register used to expose feature support to BIOS 
 * for SKU differentiation. 
 */
typedef union {
  struct {
    UINT32 error_spoofing_dis : 1;
    /* error_spoofing_dis - Bits[0:0], RO_FW, default = 1'b0 
       Error spoofing disabled
     */
    UINT32 pfat_disable : 1;
    /* pfat_disable - Bits[1:1], RO_FW, default = 1'b0 
       PFAT disabled
     */
    UINT32 fit_boot_dis : 1;
    /* fit_boot_dis - Bits[2:2], RO_FW, default = 1'b0 
       FIT (Firmware Interface Table) boot disabled
     */
    UINT32 vmcs_shadowing_dis : 1;
    /* vmcs_shadowing_dis - Bits[3:3], RO_FW, default = 1'b0 
       VMCS shadowing disabled
     */
    UINT32 prod_type : 2;
    /* prod_type - Bits[5:4], RO_FW, default = 2'b00 
       Product type  
               00 = Haswell processor
     */
    UINT32 physical_chop : 2;
    /* physical_chop - Bits[7:6], RO_FW, default = 2'b00 
       10:HCC; 01:MCC; 00:LCC;
     */
    UINT32 ocpcie_ratio_dis : 1;
    /* ocpcie_ratio_dis - Bits[8:8], RO_FW, default = 1'b0 
       Changing PCIE ratio for BCLK overclocking enabled
     */
    UINT32 ocvolt_ovrd_dis : 1;
    /* ocvolt_ovrd_dis - Bits[9:9], RO_FW, default = 1'b0 
       Voltage override overclocking disabled
     */
    UINT32 llc_qos_mon_en : 1;
    /* llc_qos_mon_en - Bits[10:10], RO_FW, default = 1'b0 
       LLC QOS monitoring enabled
     */
    UINT32 llc_qos_en : 1;
    /* llc_qos_en - Bits[11:11], RO_FW, default = 1'b0 
       LLC QOS enforcement enabled
     */
    UINT32 rtm_dis : 1;
    /* rtm_dis - Bits[12:12], RO_FW, default = 1'b0 
       HLE+ disabled
     */
    UINT32 hle_dis : 1;
    /* hle_dis - Bits[13:13], RO_FW, default = 1'b0 
       HLE disabled
     */
    UINT32 fma_dis : 1;
    /* fma_dis - Bits[14:14], RO_FW, default = 1'b0 
       FMA (Floatingpoint Multiple Add) instructions disabled
     */
    UINT32 vmse_2xfc : 4;
    /* vmse_2xfc - Bits[18:15], RO_FW, default = 4'b0000 
       VMSE_2XFC enabled
     */
    UINT32 enhanced_mca_dis : 1;
    /* enhanced_mca_dis - Bits[19:19], RO_FW, default = 1'b0 
       Enhanced MCA disabled
     */
    UINT32 smm_code_chk_dis : 1;
    /* smm_code_chk_dis - Bits[20:20], RO_FW, default = 1'b0 
       Code access check disabled
     */
    UINT32 smm_cpu_svrstr_dis : 1;
    /* smm_cpu_svrstr_dis - Bits[21:21], RO_FW, default = 1'b0 
       SMM CPU save/restore disabled (part of enhanced SMM)
     */
    UINT32 targeted_smi_dis : 1;
    /* targeted_smi_dis - Bits[22:22], RO_FW, default = 1'b0 
       Targeted SMI disabled (part of enhanced SMM)
     */
    UINT32 sapm_dll_disable : 1;
    /* sapm_dll_disable - Bits[23:23], RO_FW, default = 1'b0  */
    UINT32 ufs_disable : 1;
    /* ufs_disable - Bits[24:24], RO_FW, default = 1'b0 
       UFS (Uncore Frequency Scaling) disabled.
     */
    UINT32 pcps_disable : 1;
    /* pcps_disable - Bits[25:25], RO_FW, default = 1'b0 
       Per-core P-state disabled
     */
    UINT32 eet_enable : 1;
    /* eet_enable - Bits[26:26], RO_FW, default = 1'b0 
       Energy efficient turbo enabled
     */
    UINT32 rsvd_27 : 1;
    UINT32 oc_enabled : 1;
    /* oc_enabled - Bits[28:28], RO_FW, default = 1'b0 
        
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], RO_FW, default = 1'b0 
       Reserved for future use
     */
    UINT32 dram_rapl_disable : 1;
    /* dram_rapl_disable - Bits[30:30], RO_FW, default = 1'b0 
       DRAM RAPL disabled 
     */
    UINT32 dram_power_meter_disable : 1;
    /* dram_power_meter_disable - Bits[31:31], RO_FW, default = 1'b0  */
  } Bits;
  UINT32 Data;
} CAPID4_PCU_FUN3_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * This register is a Capability Register used to expose feature support to BIOS 
 * for SKU differentiation. 
 */
typedef union {
  struct {
    UINT32 error_spoofing_dis : 1;
    /* error_spoofing_dis - Bits[0:0], RO_FW, default = 1'b0 
       Error spoofing disabled
     */
    UINT32 pfat_disable : 1;
    /* pfat_disable - Bits[1:1], RO_FW, default = 1'b0 
       PFAT disabled
     */
    UINT32 fit_boot_dis : 1;
    /* fit_boot_dis - Bits[2:2], RO_FW, default = 1'b0 
       FIT (Firmware Interface Table) boot disabled
     */
    UINT32 vmcs_shadowing_dis : 1;
    /* vmcs_shadowing_dis - Bits[3:3], RO_FW, default = 1'b0 
       VMCS shadowing disabled
     */
    UINT32 prod_type : 2;
    /* prod_type - Bits[5:4], RO_FW, default = 2'b00 
       Product type  
               00 = Haswell processor
     */
    UINT32 physical_chop : 3;
    /* physical_chop - Bits[8:6], RO_FW, default = 3'b00 
       10:HCC; 01:MCC; 00:LCC; 100:LCCDE; 101:HCCDE
     */
    UINT32 ocvolt_ovrd_dis : 1;
    /* ocvolt_ovrd_dis - Bits[9:9], RO_FW, default = 1'b0 
       Voltage override overclocking disabled
     */
    UINT32 llc_qos_mon_en : 1;
    /* llc_qos_mon_en - Bits[10:10], RO_FW, default = 1'b0 
       LLC QOS monitoring enabled
     */
    UINT32 llc_qos_en : 1;
    /* llc_qos_en - Bits[11:11], RO_FW, default = 1'b0 
       LLC QOS enforcement enabled
     */
    UINT32 rtm_dis : 1;
    /* rtm_dis - Bits[12:12], RO_FW, default = 1'b0 
       HLE+ disabled
     */
    UINT32 hle_dis : 1;
    /* hle_dis - Bits[13:13], RO_FW, default = 1'b0 
       HLE disabled
     */
    UINT32 fma_dis : 1;
    /* fma_dis - Bits[14:14], RO_FW, default = 1'b0 
       FMA (Floatingpoint Multiple Add) instructions disabled
     */
    UINT32 vmse_2xfc : 4;
    /* vmse_2xfc - Bits[18:15], RO_FW, default = 4'b0000 
       VMSE_2XFC enabled
     */
    UINT32 enhanced_mca_dis : 1;
    /* enhanced_mca_dis - Bits[19:19], RO_FW, default = 1'b0 
       Enhanced MCA disabled
     */
    UINT32 smm_code_chk_dis : 1;
    /* smm_code_chk_dis - Bits[20:20], RO_FW, default = 1'b0 
       Code access check disabled
     */
    UINT32 smm_cpu_svrstr_dis : 1;
    /* smm_cpu_svrstr_dis - Bits[21:21], RO_FW, default = 1'b0 
       SMM CPU save/restore disabled (part of enhanced SMM)
     */
    UINT32 targeted_smi_dis : 1;
    /* targeted_smi_dis - Bits[22:22], RO_FW, default = 1'b0 
       Targeted SMI disabled (part of enhanced SMM)
     */
    UINT32 sapm_dll_disable : 1;
    /* sapm_dll_disable - Bits[23:23], RO_FW, default = 1'b0  */
    UINT32 ufs_disable : 1;
    /* ufs_disable - Bits[24:24], RO_FW, default = 1'b0 
       UFS (Uncore Frequency Scaling) disabled.
     */
    UINT32 pcps_disable : 1;
    /* pcps_disable - Bits[25:25], RO_FW, default = 1'b0 
       Per-core P-state disabled
     */
    UINT32 eet_enable : 1;
    /* eet_enable - Bits[26:26], RO_FW, default = 1'b0 
       Energy efficient turbo enabled
     */
    UINT32 rsvd_27 : 1;
    UINT32 oc_enabled : 1;
    /* oc_enabled - Bits[28:28], RO_FW, default = 1'b0 
        
     */
    UINT32 ocpcie_ratio_dis : 1;
    /* ocpcie_ratio_dis - Bits[29:29], RO_FW, default = 1'b0 
       Changing PCIE ratio for BCLK overclocking enabled
     */
    UINT32 dram_rapl_disable : 1;
    /* dram_rapl_disable - Bits[30:30], RO_FW, default = 1'b0 
       DRAM RAPL disabled 
     */
    UINT32 dram_power_meter_disable : 1;
    /* dram_power_meter_disable - Bits[31:31], RO_FW, default = 1'b0  */
  } Bits;
  UINT32 Data;
} CAPID4_PCU_FUN3_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* CAPID5_PCU_FUN3_REG supported on:                                          */
/*       HSX (0x401F3098)                                                     */
/*       BDX (0x401F3098)                                                     */
/* Register default value:              0x00000000                            */
#define CAPID5_PCU_FUN3_REG 0x0A034098
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.3.CFG.xml.
 * This register is a Capability Register used to expose feature support to BIOS 
 * for SKU differentiation. 
 */
typedef union {
  struct {
    UINT32 llc_slice_en : 18;
    /* llc_slice_en - Bits[17:0], RO_FW, default = 18'b000000000000000000 
       Enabled Cbo slices (Cbo with enabled LLC slice).
     */
    UINT32 spare : 6;
    /* spare - Bits[23:18], RO_FW, default = 6'b000000 
       Reserved for future use
     */
    UINT32 hsw_ni_dis : 1;
    /* hsw_ni_dis - Bits[24:24], RO_FW, default = 1'b0 
       All the HSW new instructions except LZCNT, TZCNT, MOVBE disabled. 
     */
    UINT32 spare2 : 4;
    /* spare2 - Bits[28:25], RO_FW, default = 1'b0 
       Spare bits reserved for future use
     */
    UINT32 addr_based_mem_mirror : 1;
    /* addr_based_mem_mirror - Bits[29:29], RO_FW, default = 1'b0 
       Address based memory mirroring enabled
     */
    UINT32 hitme_enable : 1;
    /* hitme_enable - Bits[30:30], RO_FW, default = 1'b0 
       Directory Cache enabled.
     */
    UINT32 cod_enable : 1;
    /* cod_enable - Bits[31:31], RO_FW, default = 1'b0 
       COD support enabled.
     */
  } Bits;
  UINT32 Data;
} CAPID5_PCU_FUN3_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * This register is a Capability Register used to expose feature support to BIOS 
 * for SKU differentiation. 
 */
typedef union {
  struct {
    UINT32 llc_slice_en : 24;
    /* llc_slice_en - Bits[23:0], RO_FW, default = 24'b000000000000000000 
       Enabled Cbo slices (Cbo with enabled LLC slice).
     */
    UINT32 hsw_ni_dis : 1;
    /* hsw_ni_dis - Bits[24:24], RO_FW, default = 1'b0 
       All the HSW new instructions except LZCNT, TZCNT, MOVBE disabled. 
     */
    UINT32 spare2 : 4;
    /* spare2 - Bits[28:25], RO_FW, default = 1'b0 
       Spare bits reserved for future use. Bit 26:25 is taken to reflect fuse bits 
       [ACC_Enable, HWP_enable]. Bit27 is taken to reflect cbdma_disable  
     */
    UINT32 addr_based_mem_mirror : 1;
    /* addr_based_mem_mirror - Bits[29:29], RO_FW, default = 1'b0 
       Address based memory mirroring enabled
     */
    UINT32 hitme_enable : 1;
    /* hitme_enable - Bits[30:30], RO_FW, default = 1'b0 
       Directory Cache enabled.
     */
    UINT32 cod_enable : 1;
    /* cod_enable - Bits[31:31], RO_FW, default = 1'b0 
       COD support enabled.
     */
  } Bits;
  UINT32 Data;
} CAPID5_PCU_FUN3_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* CAPID6_PCU_FUN3_REG supported on:                                          */
/*       HSX (0x401F309C)                                                     */
/*       BDX (0x401F309C)                                                     */
/* Register default value:              0x00000000                            */
#define CAPID6_PCU_FUN3_REG 0x0A03409C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.3.CFG.xml.
 * This register is a Capability Register used to expose feature support to BIOS 
 * for SKU differentiation. 
 */
typedef union {
  struct {
    UINT32 llc_ia_core_en : 18;
    /* llc_ia_core_en - Bits[17:0], RO_FW, default = 18'b000000000000000000  */
    UINT32 spare : 11;
    /* spare - Bits[28:18], RO_FW, default = 11'b00000000000 
       Spare bits reserved for future use
     */
    UINT32 de_skt_secondha : 1;
    /* de_skt_secondha - Bits[29:29], RO_FW, default = 1'b0 
       Indicates when second Home Agent and Memory Controller is enabled
     */
    UINT32 iio_llcconfig_en : 1;
    /* iio_llcconfig_en - Bits[30:30], RO_FW, default = 1'b0 
       IIO to allocate in LLC enabled.
     */
    UINT32 timed_mwait_dis : 1;
    /* timed_mwait_dis - Bits[31:31], RO_FW, default = 1'b0  */
  } Bits;
  UINT32 Data;
} CAPID6_PCU_FUN3_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * This register is a Capability Register used to expose feature support to BIOS 
 * for SKU differentiation. 
 */
typedef union {
  struct {
    UINT32 llc_ia_core_en : 24;
    /* llc_ia_core_en - Bits[23:0], RO_FW, default = 24'b000000000000000000  */
    UINT32 rsvd : 5;
    /* rsvd - Bits[28:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 de_skt_secondha : 1;
    /* de_skt_secondha - Bits[29:29], RO_FW, default = 1'b0 
       Indicates when second Home Agent and Memory Controller is enabled
     */
    UINT32 iio_llcconfig_en : 1;
    /* iio_llcconfig_en - Bits[30:30], RO_FW, default = 1'b0 
       IIO to allocate in LLC enabled.
     */
    UINT32 timed_mwait_dis : 1;
    /* timed_mwait_dis - Bits[31:31], RO_FW, default = 1'b0  */
  } Bits;
  UINT32 Data;
} CAPID6_PCU_FUN3_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* FLEX_RATIO_N0_PCU_FUN3_REG supported on:                                   */
/*       IVT_EP (0x401530A0)                                                  */
/*       IVT_EX (0x401530A0)                                                  */
/*       HSX (0x401F30A0)                                                     */
/*       BDX (0x401F30A0)                                                     */
/* Register default value:              0x00000000                            */
#define FLEX_RATIO_N0_PCU_FUN3_REG 0x0A0340A0


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.3.CFG.xml.
 * The Flexible Boot register is written by BIOS in order to modify the maximum 
 * non-turbo ratio on the next reset. The DP/MP systems use this FLEX_RATIO in this 
 * register to configure the maximum common boot ratio for all physical processors 
 * in the system. The value in the FLEX_RATIO take effect on the next reset based 
 * on the value of FLEX_EN. 
 * This CSR is a mirror of MSR  (194h) FLEX_RATIO.  Refer to this MSR for field 
 * descriptions. 
 */
typedef union {
  struct {
    UINT32 oc_extra_voltage : 8;
    /* oc_extra_voltage - Bits[7:0], RWS, default = 8'b00000000 
       Extra voltage to be used for Over Clocking. The voltage is defined in units of 
       1/256 Volts. 
     */
    UINT32 flex_ratio : 8;
    /* flex_ratio - Bits[15:8], RWS, default = 8'b00000000 
       Desired Flex ratio. When enabled, the value in this field will set the maximum 
       non-turbo ratio available. If this value is greater than the maximum ratio set 
       by hardware, this field is ignored. 
     */
    UINT32 enable : 1;
    /* enable - Bits[16:16], RWS, default = 1'b0 
       Flex Ratio Enabled. When set to 1, indicates that the value programmed in 
       FLEX_RATIO field will be used to override the maximum non-turbo ratio on next 
       reboot. When this bit is cleared, all write to FLEX_RATIO field will be ignored. 
       However there will be no GP-fault# generated. 
     */
    UINT32 oc_bins : 3;
    /* oc_bins - Bits[19:17], RO_FW, default = 3'b000 
       This field indicates the maximum number of bins by which the part can be 
       overclocked 
     */
    UINT32 oc_lock : 1;
    /* oc_lock - Bits[20:20], RW1S, default = 1'b0 
       Once set to 1, the overclocking bitfields become locked from further writes are 
       ignored. A reset is required to clear the lock. 
       Note: System BIOS must set this bit to lock down overclocking limits.
     */
    UINT32 rsvd : 11;
    /* rsvd - Bits[31:21], n/a, default = n/a 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} FLEX_RATIO_N0_PCU_FUN3_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */






/* SMT_CONTROL_PCU_FUN3_REG supported on:                                     */
/*       HSX (0x401F30B0)                                                     */
/*       BDX (0x401F30B0)                                                     */
/* Register default value:              0x00000000                            */
#define SMT_CONTROL_PCU_FUN3_REG 0x0A0340B0

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 8;
    /* rsvd_0 - Bits[7:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 thread_mask : 2;
    /* thread_mask - Bits[9:8], ROS_V, default = 2'b00 
       Thread Mask indicates which threads are enabled in the core.  The LSB is the 
       enable bit for Thread 0, whereas the MSB is the enable bit for Thread 1. 
       This field is determined by FW based on CSR_DESIRED_CORES[SMT_DISABLE]. 
     */
    UINT32 rsvd_10 : 14;
    /* rsvd_10 - Bits[23:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fused_smt_capability : 1;
    /* fused_smt_capability - Bits[24:24], ROS_V, default = 1'b0 
       Enabled threads in the package.
       0b  1 thread
       1b  2 threads
       
     */
    UINT32 rsvd_25 : 7;
    /* rsvd_25 - Bits[31:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SMT_CONTROL_PCU_FUN3_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RESOLVED_CORES_PCU_FUN3_REG supported on:                                  */
/*       HSX (0x401F30B4)                                                     */
/*       BDX (0x401F30B4)                                                     */
/* Register default value:              0x00000000                            */
#define RESOLVED_CORES_PCU_FUN3_REG 0x0A0340B4
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.3.CFG.xml.
 */
typedef union {
  struct {
    UINT32 core_mask : 18;
    /* core_mask - Bits[17:0], ROS_V, default = 18'b000000000000000000 
       The resolved IA core mask contains the functional (enabled in SKU) and 
       non-defeatured IA cores. 
       The mask is indexed by logical ID.  It is normally contiguous, unless BIOS 
       defeature is activated on a particular core. 
       BSP and APIC IDs will be set by the processor based on this value.
       This field is determined by FW based on CSR_DESIRED_CORES[CORE_OFF_MASK]. 
     */
    UINT32 rsvd : 14;
    /* rsvd - Bits[31:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RESOLVED_CORES_PCU_FUN3_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 */
typedef union {
  struct {
    UINT32 core_mask : 24;
    /* core_mask - Bits[23:0], ROS_V, default = 18'b000000000000000000 
       The resolved IA core mask contains the functional (enabled in SKU) and 
       non-defeatured IA cores. 
       The mask is indexed by logical ID.  It is normally contiguous, unless BIOS 
       defeature is activated on a particular core. 
       BSP and APIC IDs will be set by the processor based on this value.
       This field is determined by FW based on CSR_DESIRED_CORES[CORE_OFF_MASK]. 
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RESOLVED_CORES_PCU_FUN3_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* FUSED_CORES_PCU_FUN3_REG supported on:                                     */
/*       HSX (0x401F30B8)                                                     */
/*       BDX (0x401F30B8)                                                     */
/* Register default value:              0x00000000                            */
#define FUSED_CORES_PCU_FUN3_REG 0x0A0340B8
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.3.CFG.xml.
 */
typedef union {
  struct {
    UINT32 fused_core_mask : 18;
    /* fused_core_mask - Bits[17:0], ROS_V, default = 18'b000000000000000000 
       Vector of SKU enabled IA cores in the package.
     */
    UINT32 rsvd : 14;
    /* rsvd - Bits[31:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} FUSED_CORES_PCU_FUN3_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 */
typedef union {
  struct {
    UINT32 fused_core_mask : 24;
    /* fused_core_mask - Bits[23:0], ROS_V, default = 18'b000000000000000000 
       Vector of SKU enabled IA cores in the package. This CSR contains the cores 
       enabled in the SKU, and CSR RESOLVED_CORES contains the active cores (takes into 
       account deconfigured cores).  
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} FUSED_CORES_PCU_FUN3_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */

























/* PCU_MC_CTL_PCU_FUN3_REG supported on:                                      */
/*       HSX (0x401F30EC)                                                     */
/*       BDX (0x401F30EC)                                                     */
/* Register default value:              0x00000021                            */
#define PCU_MC_CTL_PCU_FUN3_REG 0x0A0340EC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * This CSR is used to read a programmable counter value to interpret the incoming 
 * caterr pin as an IErr or UMCErr.  
 */
typedef union {
  struct {
    UINT32 caterrdetectcntr : 8;
    /* caterrdetectcntr - Bits[7:0], RW, default = 8'b00100001 
       If a socket counts the Caterr input pulse duration to be less than this value, 
       it will be classified as an UMCErr. Otherwise, if the duration is greater than 
       this threshold value, it will be classified as an Ierr. 
       Default Value = 0x33
     */
    UINT32 rsvd : 24;
    /* rsvd - Bits[31:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PCU_MC_CTL_PCU_FUN3_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PCU_LT_CTRL_PCU_FUN3_REG supported on:                                     */
/*       IVT_EP (0x401530F0)                                                  */
/*       IVT_EX (0x401530F0)                                                  */
/*       HSX (0x401F30F0)                                                     */
/*       BDX (0x401F30F0)                                                     */
/* Register default value:              0x00000000                            */
#define PCU_LT_CTRL_PCU_FUN3_REG 0x0A0340F0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * PCU Intel TXT Control Attributes
 */
typedef union {
  struct {
    UINT32 bios_spare : 31;
    /* bios_spare - Bits[30:0], RW, default = 31'b0000000000000000000000000000000 
       Reserved for BIOS
     */
    UINT32 pit : 1;
    /* pit - Bits[31:31], RW_O, default = 1'b0 
       PECI is Trusted bit. Allows BIOS to specify whether PECI can be trusted. 
       
       Pcode uses this bit to allow write access to Intel TXT protected registers 
       initiated from PECI. Pcode samples this bit when RST_CPL3 is set.  
       Pcode ignores this bit when in SSP boot mode. 
       Default value of 0 implies PECI is not Trusted.
       
       1 = PECI is Trusted
     */
  } Bits;
  UINT32 Data;
} PCU_LT_CTRL_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* PCU_BIOS_SPARE2_PCU_FUN3_REG supported on:                                 */
/*       IVT_EP (0x401530F4)                                                  */
/*       IVT_EX (0x401530F4)                                                  */
/*       HSX (0x401F30F4)                                                     */
/*       BDX (0x401F30F4)                                                     */
/* Register default value:              0x00000000                            */
#define PCU_BIOS_SPARE2_PCU_FUN3_REG 0x0A0340F4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 * Used by PCU.
 */
typedef union {
  struct {
    UINT32 bios_spare : 32;
    /* bios_spare - Bits[31:0], RW, default = 32'b00000000000000000000000000000000 
       [11:00] - Reserved
       [13:12] - 2 bit encoding of connected DIMM type:
                 2'b00 - UDIMM
                 2'b01 - RDIMM
                 2'b10 - LRDIMM
                 2'b11 - Reserved
       [15:14] - 2 bit encoding of the Vddrq voltage level
                 2'b00 - 1.25 V
                 2'b01 - 1.35 V
                 2'b10 - 1.50 V
                 2'b11 - Reserved
       [16:16] - Reserved.
       [17:17] - If set, enable GV block for memory sparing
       [18:18] - Used to select between Legacy ADR behavior and the new ADR flow; if 
       clear, use legacy behavior(default). 
       [31:19] - Unused
               
     */
  } Bits;
  UINT32 Data;
} PCU_BIOS_SPARE2_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* PWR_LIMIT_MISC_INFO_PCU_FUN3_REG supported on:                             */
/*       IVT_EP (0x401530F8)                                                  */
/*       IVT_EX (0x401530F8)                                                  */
/*       HSX (0x401F30F8)                                                     */
/*       BDX (0x401F30F8)                                                     */
/* Register default value:              0x00000000                            */
#define PWR_LIMIT_MISC_INFO_PCU_FUN3_REG 0x0A0340F8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.3.CFG.xml.
 */
typedef union {
  struct {
    UINT32 pn_power_of_sku : 15;
    /* pn_power_of_sku - Bits[14:0], RO_FW, default = 15'b000000000000000 
       Skt power at Pn
               
     */
    UINT32 pbm_minimal_tau_allowed : 7;
    /* pbm_minimal_tau_allowed - Bits[21:15], RO_FW, default = 7'b0000000 
       Minimal Time window 
     */
    UINT32 rsvd : 10;
    /* rsvd - Bits[31:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PWR_LIMIT_MISC_INFO_PCU_FUN3_STRUCT;
#endif /* ASM_INC */


/* RESOLVED_CORES_MASK_PCU_FUN3_REG supported on:                             */
/*       IVT_EP (0x401530B0)                                                  */
/*       IVT_EX (0x401530B0)                                                  */
/* Register default value:              0x00000000                            */
#define RESOLVED_CORES_MASK_PCU_FUN3_REG 0x0A0340B0



#endif /* PCU_FUN3_h */
