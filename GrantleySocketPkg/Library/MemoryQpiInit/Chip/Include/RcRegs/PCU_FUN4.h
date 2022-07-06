/* Date Stamp: 8/23/2014 */

#ifndef PCU_FUN4_h
#define PCU_FUN4_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* PCU_FUN4_IVT_DEV 10                                                        */
/* PCU_FUN4_IVT_FUN 4                                                         */
/* For HSX_HOST:                                                              */
/* PCU_FUN4_HSX_DEV 30                                                        */
/* PCU_FUN4_HSX_FUN 4                                                         */
/* For BDX_HOST:                                                              */
/* PCU_FUN4_BDX_DEV 30                                                        */
/* PCU_FUN4_BDX_FUN 4                                                         */

/* VID_PCU_FUN4_REG supported on:                                             */
/*       IVT_EP (0x20154000)                                                  */
/*       IVT_EX (0x20154000)                                                  */
/*       HSX (0x201F4000)                                                     */
/*       BDX (0x201F4000)                                                     */
/* Register default value:              0x8086                                */
#define VID_PCU_FUN4_REG 0x0A042000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
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
} VID_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* DID_PCU_FUN4_REG supported on:                                             */
/*       IVT_EP (0x20154002)                                                  */
/*       IVT_EX (0x20154002)                                                  */
/*       HSX (0x201F4002)                                                     */
/*       BDX (0x201F4002)                                                     */
/* Register default value on IVT_EP:    0x0EC4                                */
/* Register default value on IVT_EX:    0x0EC4                                */
/* Register default value on HSX:       0x2F9C                                */
/* Register default value on BDX:       0x6F9C                                */
#define DID_PCU_FUN4_REG 0x0A042002


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.4.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h2F9C (HSX), 16'h6F9C (BDX) 
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
} DID_PCU_FUN4_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PCICMD_PCU_FUN4_REG supported on:                                          */
/*       IVT_EP (0x20154004)                                                  */
/*       IVT_EX (0x20154004)                                                  */
/*       HSX (0x201F4004)                                                     */
/*       BDX (0x201F4004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_PCU_FUN4_REG 0x0A042004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
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
} PCICMD_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* PCISTS_PCU_FUN4_REG supported on:                                          */
/*       IVT_EP (0x20154006)                                                  */
/*       IVT_EX (0x20154006)                                                  */
/*       HSX (0x201F4006)                                                     */
/*       BDX (0x201F4006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_PCU_FUN4_REG 0x0A042006
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
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
} PCISTS_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* RID_PCU_FUN4_REG supported on:                                             */
/*       IVT_EP (0x10154008)                                                  */
/*       IVT_EX (0x10154008)                                                  */
/*       HSX (0x101F4008)                                                     */
/*       BDX (0x101F4008)                                                     */
/* Register default value:              0x00                                  */
#define RID_PCU_FUN4_REG 0x0A041008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
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
} RID_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_PCU_FUN4_REG supported on:                                          */
/*       IVT_EP (0x10154009)                                                  */
/*       IVT_EX (0x10154009)                                                  */
/*       HSX (0x101F4009)                                                     */
/*       BDX (0x101F4009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_PCU_FUN4_REG 0x0A041009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.4.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_PCU_FUN4_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_PCU_FUN4_REG supported on:                                          */
/*       IVT_EP (0x2015400A)                                                  */
/*       IVT_EX (0x2015400A)                                                  */
/*       HSX (0x201F400A)                                                     */
/*       BDX (0x201F400A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_PCU_FUN4_REG 0x0A04200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
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
} CCR_N1_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* CLSR_PCU_FUN4_REG supported on:                                            */
/*       IVT_EP (0x1015400C)                                                  */
/*       IVT_EX (0x1015400C)                                                  */
/*       HSX (0x101F400C)                                                     */
/*       BDX (0x101F400C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_PCU_FUN4_REG 0x0A04100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
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
} CLSR_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* PLAT_PCU_FUN4_REG supported on:                                            */
/*       IVT_EP (0x1015400D)                                                  */
/*       IVT_EX (0x1015400D)                                                  */
/*       HSX (0x101F400D)                                                     */
/*       BDX (0x101F400D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_PCU_FUN4_REG 0x0A04100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
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
} PLAT_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* HDR_PCU_FUN4_REG supported on:                                             */
/*       IVT_EP (0x1015400E)                                                  */
/*       IVT_EX (0x1015400E)                                                  */
/*       HSX (0x101F400E)                                                     */
/*       BDX (0x101F400E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_PCU_FUN4_REG 0x0A04100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
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
} HDR_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* BIST_PCU_FUN4_REG supported on:                                            */
/*       IVT_EP (0x1015400F)                                                  */
/*       IVT_EX (0x1015400F)                                                  */
/*       HSX (0x101F400F)                                                     */
/*       BDX (0x101F400F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_PCU_FUN4_REG 0x0A04100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
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
} BIST_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* SVID_PCU_FUN4_REG supported on:                                            */
/*       IVT_EP (0x2015402C)                                                  */
/*       IVT_EX (0x2015402C)                                                  */
/*       HSX (0x201F402C)                                                     */
/*       BDX (0x201F402C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_PCU_FUN4_REG 0x0A04202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
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
} SVID_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* SDID_PCU_FUN4_REG supported on:                                            */
/*       IVT_EP (0x2015402E)                                                  */
/*       IVT_EX (0x2015402E)                                                  */
/*       HSX (0x201F402E)                                                     */
/*       BDX (0x201F402E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_PCU_FUN4_REG 0x0A04202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
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
} SDID_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_PCU_FUN4_REG supported on:                                          */
/*       IVT_EP (0x10154034)                                                  */
/*       IVT_EX (0x10154034)                                                  */
/*       HSX (0x101F4034)                                                     */
/*       BDX (0x101F4034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_PCU_FUN4_REG 0x0A041034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
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
} CAPPTR_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* INTL_PCU_FUN4_REG supported on:                                            */
/*       IVT_EP (0x1015403C)                                                  */
/*       IVT_EX (0x1015403C)                                                  */
/*       HSX (0x101F403C)                                                     */
/*       BDX (0x101F403C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_PCU_FUN4_REG 0x0A04103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
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
} INTL_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* INTPIN_PCU_FUN4_REG supported on:                                          */
/*       IVT_EP (0x1015403D)                                                  */
/*       IVT_EX (0x1015403D)                                                  */
/*       HSX (0x101F403D)                                                     */
/*       BDX (0x101F403D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_PCU_FUN4_REG 0x0A04103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
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
} INTPIN_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* MINGNT_PCU_FUN4_REG supported on:                                          */
/*       IVT_EP (0x1015403E)                                                  */
/*       IVT_EX (0x1015403E)                                                  */
/*       HSX (0x101F403E)                                                     */
/*       BDX (0x101F403E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_PCU_FUN4_REG 0x0A04103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
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
} MINGNT_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_PCU_FUN4_REG supported on:                                          */
/*       IVT_EP (0x1015403F)                                                  */
/*       IVT_EX (0x1015403F)                                                  */
/*       HSX (0x101F403F)                                                     */
/*       BDX (0x101F403F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_PCU_FUN4_REG 0x0A04103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
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
} MAXLAT_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* MEM_TRML_TEMPERATURE_REPORT_1_PCU_FUN4_REG supported on:                   */
/*       IVT_EP (0x40154060)                                                  */
/*       IVT_EX (0x40154060)                                                  */
/*       HSX (0x401F4060)                                                     */
/*       BDX (0x401F4060)                                                     */
/* Register default value:              0x00000000                            */
#define MEM_TRML_TEMPERATURE_REPORT_1_PCU_FUN4_REG 0x0A044060
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
 * This register is used to report the thermal status of the memory. 
 *   The channel max temperature field is used to report the maximal temperature of 
 * all ranks. 
 */
typedef union {
  struct {
    UINT32 channel0_max_temperature : 8;
    /* channel0_max_temperature - Bits[7:0], RO_V, default = 8'b00000000 
       Temperature in Degrees (C).
     */
    UINT32 channel1_max_temperature : 8;
    /* channel1_max_temperature - Bits[15:8], RO_V, default = 8'b00000000 
       Temperature in Degrees (C).
     */
    UINT32 channel2_max_temperature : 8;
    /* channel2_max_temperature - Bits[23:16], RO_V, default = 8'b00000000 
       Temperature in Degrees (C).
     */
    UINT32 channel3_max_temperature : 8;
    /* channel3_max_temperature - Bits[31:24], RO_V, default = 8'b00000000 
       Temperature in Degrees (C).
     */
  } Bits;
  UINT32 Data;
} MEM_TRML_TEMPERATURE_REPORT_1_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* MEM_ACCUMULATED_BW_CH_4_PCU_FUN4_REG supported on:                         */
/*       IVT_EP (0x40154064)                                                  */
/*       IVT_EX (0x40154064)                                                  */
/*       HSX (0x401F4064)                                                     */
/*       BDX (0x401F4064)                                                     */
/* Register default value:              0x00000000                            */
#define MEM_ACCUMULATED_BW_CH_4_PCU_FUN4_REG 0x0A044064
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
 * This register contains a measurement proportional to the 
 * weighted DRAM BW for the channel (including all ranks).  The weights are 
 * configured in the memory controller channel register PM_CMD_PWR.
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       The weighted BW value is calculated by the memory 
       controller based on the following formula:
       
       Num_Precharge * PM_CMD_PWR[PWR_RAS_PRE] +
       Num_Reads * PM_CMD_PWR[PWR_CAS_R] + 
       Num_Writes * PM_CMD_PWR[PWR_CAS_W]
     */
  } Bits;
  UINT32 Data;
} MEM_ACCUMULATED_BW_CH_4_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* MEM_ACCUMULATED_BW_CH_5_PCU_FUN4_REG supported on:                         */
/*       IVT_EP (0x40154068)                                                  */
/*       IVT_EX (0x40154068)                                                  */
/*       HSX (0x401F4068)                                                     */
/*       BDX (0x401F4068)                                                     */
/* Register default value:              0x00000000                            */
#define MEM_ACCUMULATED_BW_CH_5_PCU_FUN4_REG 0x0A044068
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
 * This register contains a measurement proportional to the 
 * weighted DRAM BW for the channel (including all ranks).  The weights are 
 * configured in the memory controller channel register PM_CMD_PWR.
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       The weighted BW value is calculated by the memory 
       controller based on the following formula:
       
       Num_Precharge * PM_CMD_PWR[PWR_RAS_PRE] +
       Num_Reads * PM_CMD_PWR[PWR_CAS_R] + 
       Num_Writes * PM_CMD_PWR[PWR_CAS_W]
     */
  } Bits;
  UINT32 Data;
} MEM_ACCUMULATED_BW_CH_5_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* MEM_ACCUMULATED_BW_CH_6_PCU_FUN4_REG supported on:                         */
/*       IVT_EP (0x4015406C)                                                  */
/*       IVT_EX (0x4015406C)                                                  */
/*       HSX (0x401F406C)                                                     */
/*       BDX (0x401F406C)                                                     */
/* Register default value:              0x00000000                            */
#define MEM_ACCUMULATED_BW_CH_6_PCU_FUN4_REG 0x0A04406C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
 * This register contains a measurement proportional to the 
 * weighted DRAM BW for the channel (including all ranks).  The weights are 
 * configured in the memory controller channel register PM_CMD_PWR.
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       The weighted BW value is calculated by the memory 
       controller based on the following formula:
       
       Num_Precharge * PM_CMD_PWR[PWR_RAS_PRE] +
       Num_Reads * PM_CMD_PWR[PWR_CAS_R] + 
       Num_Writes * PM_CMD_PWR[PWR_CAS_W]
     */
  } Bits;
  UINT32 Data;
} MEM_ACCUMULATED_BW_CH_6_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* GLOBAL_NID_SOCKET_4_TO_7_MAP_PCU_FUN4_REG supported on:                    */
/*       HSX (0x401F4070)                                                     */
/*       BDX (0x401F4070)                                                     */
/* Register default value:              0x00000000                            */
#define GLOBAL_NID_SOCKET_4_TO_7_MAP_PCU_FUN4_REG 0x0A044070

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
 * This register is in the PCU CR space.  It contains NID information for sockets 
 * 4-7 in the platform.  Expectation is that BIOS will write this register during 
 * the Reset/Init flow. 
 */
typedef union {
  struct {
    UINT32 skt4_nid : 3;
    /* skt4_nid - Bits[2:0], RW_LB, default = 3'b000 
       Socket4 NID
     */
    UINT32 rsvd_3 : 1;
    UINT32 skt5_nid : 3;
    /* skt5_nid - Bits[6:4], RW_LB, default = 3'b000 
       Socket5 NID
     */
    UINT32 rsvd_7 : 1;
    UINT32 skt6_nid : 3;
    /* skt6_nid - Bits[10:8], RW_LB, default = 3'b000 
       Socket6 NID
     */
    UINT32 rsvd_11 : 1;
    UINT32 skt7_nid : 3;
    /* skt7_nid - Bits[14:12], RW_LB, default = 3'b000 
       Socket7 NID
     */
    UINT32 rsvd_15 : 13;
    UINT32 skt_valid : 4;
    /* skt_valid - Bits[31:28], RW_LB, default = 4'b0000 
       Valid bits indicating whether NID has been programmed by BIOS.  If bit is 0 
       after the CST/SST ready bit is set, then it implies that the socket is not 
       populated. 
     */
  } Bits;
  UINT32 Data;
} GLOBAL_NID_SOCKET_4_TO_7_MAP_PCU_FUN4_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* VIRAL_CONTROL_PCU_FUN4_REG supported on:                                   */
/*       IVT_EP (0x40154084)                                                  */
/*       IVT_EX (0x40154084)                                                  */
/*       HSX (0x401F4084)                                                     */
/*       BDX (0x401F4084)                                                     */
/* Register default value:              0x00000000                            */
#define VIRAL_CONTROL_PCU_FUN4_REG 0x0A044084


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.4.CFG.xml.
 * Controls how PCU responds to viral and eMCA signalling.
 *       Note: This register must be programmed for eMCA mode even if viral is not 
 * enabled. 
 */
typedef union {
  struct {
    UINT32 viral_reset : 1;
    /* viral_reset - Bits[0:0], RW1S, default = 1'b0 
       Clears the CATERR#/MSMI# pin after a viral is signalled.
     */
    UINT32 viral_fast_pin_dis : 1;
    /* viral_fast_pin_dis - Bits[1:1], RW, default = 1'b0 
       When set, disables the signaling of IERR/MSMI based on the global fatal signal.  
       CATERR# assertion dependent on message (MCERR/IERR) received over message 
       channel. MSMI# assertion dependent on message received over message channel   
       
       Values:
               0 -> IERR signaling by global fatal is enabled. In viral condition, 
       CATERR#/MSMI# will assert always with IERR semantics. 
               1 -> IERR signaling by global fatal is disabled. CATERR#/MSMI# assertion 
       dependent on message (MCERR/IERR/MSMI) received over message channel. 
       
     */
    UINT32 emca_mode : 1;
    /* emca_mode - Bits[2:2], RW, default = 1'b0 
       When set to high, MSMI input is enabled, and viral will get propagated to MSMI# 
       pin, otherwise it will get propagated to CATERR# pin 
       
     */
    UINT32 viral_log_disable : 1;
    /* viral_log_disable - Bits[3:3], RW, default = 1'b0 
       This will disable immediate logging of errors while in viral mode.
       
     */
    UINT32 rsvd : 28;
    /* rsvd - Bits[31:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} VIRAL_CONTROL_PCU_FUN4_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* MEM_ACCUMULATED_BW_CH_7_PCU_FUN4_REG supported on:                         */
/*       IVT_EP (0x40154090)                                                  */
/*       IVT_EX (0x40154090)                                                  */
/*       HSX (0x401F4090)                                                     */
/*       BDX (0x401F4090)                                                     */
/* Register default value:              0x00000000                            */
#define MEM_ACCUMULATED_BW_CH_7_PCU_FUN4_REG 0x0A044090
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
 * This register contains a measurement proportional to the 
 * weighted DRAM BW for the channel (including all ranks).  The weights are 
 * configured in the memory controller channel register PM_CMD_PWR.
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       The weighted BW value is calculated by the memory 
       controller based on the following formula:
       
       Num_Precharge * PM_CMD_PWR[PWR_RAS_PRE] +
       Num_Reads * PM_CMD_PWR[PWR_CAS_R] + 
       Num_Writes * PM_CMD_PWR[PWR_CAS_W]
     */
  } Bits;
  UINT32 Data;
} MEM_ACCUMULATED_BW_CH_7_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* DRAM_ENERGY_STATUS_CH4_PCU_FUN4_REG supported on:                          */
/*       HSX (0x401F40A8)                                                     */
/*       BDX (0x401F40A8)                                                     */
/* Register default value:              0x00000000                            */
#define DRAM_ENERGY_STATUS_CH4_PCU_FUN4_REG 0x0A0440A8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
 * DRAM energy consumed by all the DIMMS in Channel4.  The counter will wrap around 
 * and continue counting when it reaches its limit.   
 * The ENERGY_UNIT for DRAM domain is 15.3uJ. 
 * The data is updated by PCODE and is Read Only for all SW.
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Energy Value
     */
  } Bits;
  UINT32 Data;
} DRAM_ENERGY_STATUS_CH4_PCU_FUN4_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DRAM_ENERGY_STATUS_CH5_PCU_FUN4_REG supported on:                          */
/*       HSX (0x401F40B0)                                                     */
/*       BDX (0x401F40B0)                                                     */
/* Register default value:              0x00000000                            */
#define DRAM_ENERGY_STATUS_CH5_PCU_FUN4_REG 0x0A0440B0

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
 * DRAM energy consumed by all the DIMMS in Channel5.  The counter will wrap around 
 * and continue counting when it reaches its limit.   
 * The ENERGY_UNIT for DRAM domain is 15.3uJ. 
 * The data is updated by PCODE and is Read Only for all SW.
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Energy Value
     */
  } Bits;
  UINT32 Data;
} DRAM_ENERGY_STATUS_CH5_PCU_FUN4_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DRAM_ENERGY_STATUS_CH6_PCU_FUN4_REG supported on:                          */
/*       HSX (0x401F40B8)                                                     */
/*       BDX (0x401F40B8)                                                     */
/* Register default value:              0x00000000                            */
#define DRAM_ENERGY_STATUS_CH6_PCU_FUN4_REG 0x0A0440B8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
 * DRAM energy consumed by all the DIMMS in Channel6.  The counter will wrap around 
 * and continue counting when it reaches its limit.   
 * The ENERGY_UNIT for DRAM domain is 15.3uJ. 
 * The data is updated by PCODE and is Read Only for all SW.
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Energy Value
     */
  } Bits;
  UINT32 Data;
} DRAM_ENERGY_STATUS_CH6_PCU_FUN4_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DRAM_ENERGY_STATUS_CH7_PCU_FUN4_REG supported on:                          */
/*       HSX (0x401F40C0)                                                     */
/*       BDX (0x401F40C0)                                                     */
/* Register default value:              0x00000000                            */
#define DRAM_ENERGY_STATUS_CH7_PCU_FUN4_REG 0x0A0440C0

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
 * DRAM energy consumed by all the DIMMS in Channel7.  The counter will wrap around 
 * and continue counting when it reaches its limit.   
 * The ENERGY_UNIT for DRAM domain is 15.3uJ.  
 * The data is updated by PCODE and is Read Only for all SW.
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Energy Value
     */
  } Bits;
  UINT32 Data;
} DRAM_ENERGY_STATUS_CH7_PCU_FUN4_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* UNC_TSC_SNAPSHOT_N0_PCU_FUN4_REG supported on:                             */
/*       IVT_EP (0x401540E0)                                                  */
/*       IVT_EX (0x401540E0)                                                  */
/*       HSX (0x401F40E0)                                                     */
/*       BDX (0x401F40E0)                                                     */
/* Register default value:              0x00000000                            */
#define UNC_TSC_SNAPSHOT_N0_PCU_FUN4_REG 0x0A0440E0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
 * This register will capture the value of Uncore TSC on TSC_SYNC assertion rising 
 * edge detect in PCU 
 */
typedef union {
  struct {
    UINT32 uncore_tsc_snapshot : 32;
    /* uncore_tsc_snapshot - Bits[31:0], RO_V, default = 61'b0000000000000000000000000000000000000000000000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       Value of the captured Uncore TSC on internal rising edge of TSC_SYNC
     */
  } Bits;
  UINT32 Data;
} UNC_TSC_SNAPSHOT_N0_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* UNC_TSC_SNAPSHOT_N1_PCU_FUN4_REG supported on:                             */
/*       IVT_EP (0x401540E4)                                                  */
/*       IVT_EX (0x401540E4)                                                  */
/*       HSX (0x401F40E4)                                                     */
/*       BDX (0x401F40E4)                                                     */
/* Register default value:              0x00000000                            */
#define UNC_TSC_SNAPSHOT_N1_PCU_FUN4_REG 0x0A0440E4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.4.CFG.xml.
 * This register will capture the value of Uncore TSC on TSC_SYNC assertion rising 
 * edge detect in PCU 
 */
typedef union {
  struct {
    UINT32 uncore_tsc_snapshot : 29;
    /* uncore_tsc_snapshot - Bits[28:0], RO_V, default = 61'b0000000000000000000000000000000000000000000000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       Value of the captured Uncore TSC on internal rising edge of TSC_SYNC
     */
    UINT32 rsvd : 3;
    /* rsvd - Bits[31:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} UNC_TSC_SNAPSHOT_N1_PCU_FUN4_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* TSC_HP_OFFSET_N0_PCU_FUN4_REG supported on:                                */
/*       IVT_EP (0x401540E8)                                                  */
/*       IVT_EX (0x401540E8)                                                  */
/*       HSX (0x401F40E8)                                                     */
/*       BDX (0x401F40E8)                                                     */
/* Register default value:              0x00000000                            */
#define TSC_HP_OFFSET_N0_PCU_FUN4_REG 0x0A0440E8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
 * BIOS may write this register to update the TSC in the hot plugged socket. This 
 * is a one time event.  
 */
typedef union {
  struct {
    UINT32 tsc_offset : 32;
    /* tsc_offset - Bits[31:0], RW_LB, default = 61'b0000000000000000000000000000000000000000000000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       BIOS programmed value to do a one-time update to the Uncore TSC to align it for 
       Hot Plug 
     */
  } Bits;
  UINT32 Data;
} TSC_HP_OFFSET_N0_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* TSC_HP_OFFSET_N1_PCU_FUN4_REG supported on:                                */
/*       IVT_EP (0x401540EC)                                                  */
/*       IVT_EX (0x401540EC)                                                  */
/*       HSX (0x401F40EC)                                                     */
/*       BDX (0x401F40EC)                                                     */
/* Register default value:              0x00000000                            */
#define TSC_HP_OFFSET_N1_PCU_FUN4_REG 0x0A0440EC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.4.CFG.xml.
 * BIOS may write this register to update the TSC in the hot plugged socket. This 
 * is a one time event.  
 */
typedef union {
  struct {
    UINT32 tsc_offset : 29;
    /* tsc_offset - Bits[28:0], RW_LB, default = 61'b0000000000000000000000000000000000000000000000000000000000000 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       BIOS programmed value to do a one-time update to the Uncore TSC to align it for 
       Hot Plug 
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[30:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 tsc_update : 1;
    /* tsc_update - Bits[31:31], RW_LB, default = 1'b0 
       When set, will result cause the TSC_OFFSET value to be added to the Uncore TSC 
       (one-shot) 
     */
  } Bits;
  UINT32 Data;
} TSC_HP_OFFSET_N1_PCU_FUN4_STRUCT;
#endif /* ASM_INC */


/* GLOBAL_NID_MAP_REGISTER_1_PCU_FUN4_REG supported on:                       */
/*       IVT_EP (0x40154070)                                                  */
/*       IVT_EX (0x40154070)                                                  */
/* Register default value:              0x00000000                            */
#define GLOBAL_NID_MAP_REGISTER_1_PCU_FUN4_REG 0x0A044070







/* RESOLVED_CORES_MASK2_PCU_FUN4_REG supported on:                            */
/*       IVT_EP (0x4015407C)                                                  */
/*       IVT_EX (0x4015407C)                                                  */
/* Register default value:              0x00000000                            */
#define RESOLVED_CORES_MASK2_PCU_FUN4_REG 0x0A04407C



/* M_COMP1_PCU_FUN4_REG supported on:                                         */
/*       IVT_EP (0x40154080)                                                  */
/*       IVT_EX (0x40154080)                                                  */
/* Register default value:              0x0000001A                            */
#define M_COMP1_PCU_FUN4_REG 0x0A044080



/* DRAM_ENERGY_STATUS_CH4_N0_PCU_FUN4_REG supported on:                       */
/*       IVT_EP (0x401540A8)                                                  */
/*       IVT_EX (0x401540A8)                                                  */
/* Register default value:              0x00000000                            */
#define DRAM_ENERGY_STATUS_CH4_N0_PCU_FUN4_REG 0x0A0440A8





/* DRAM_ENERGY_STATUS_CH5_N0_PCU_FUN4_REG supported on:                       */
/*       IVT_EP (0x401540B0)                                                  */
/*       IVT_EX (0x401540B0)                                                  */
/* Register default value:              0x00000000                            */
#define DRAM_ENERGY_STATUS_CH5_N0_PCU_FUN4_REG 0x0A0440B0





/* DRAM_ENERGY_STATUS_CH6_N0_PCU_FUN4_REG supported on:                       */
/*       IVT_EP (0x401540B8)                                                  */
/*       IVT_EX (0x401540B8)                                                  */
/* Register default value:              0x00000000                            */
#define DRAM_ENERGY_STATUS_CH6_N0_PCU_FUN4_REG 0x0A0440B8





/* DRAM_ENERGY_STATUS_CH7_N0_PCU_FUN4_REG supported on:                       */
/*       IVT_EP (0x401540C0)                                                  */
/*       IVT_EX (0x401540C0)                                                  */
/* Register default value:              0x00000000                            */
#define DRAM_ENERGY_STATUS_CH7_N0_PCU_FUN4_REG 0x0A0440C0





#endif /* PCU_FUN4_h */
