/* Date Stamp: 8/23/2014 */

#ifndef PCU_FUN0_h
#define PCU_FUN0_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* PCU_FUN0_IVT_DEV 10                                                        */
/* PCU_FUN0_IVT_FUN 0                                                         */
/* For HSX_HOST:                                                              */
/* PCU_FUN0_HSX_DEV 30                                                        */
/* PCU_FUN0_HSX_FUN 0                                                         */
/* For BDX_HOST:                                                              */
/* PCU_FUN0_BDX_DEV 30                                                        */
/* PCU_FUN0_BDX_FUN 0                                                         */

/* VID_PCU_FUN0_REG supported on:                                             */
/*       IVT_EP (0x20150000)                                                  */
/*       IVT_EX (0x20150000)                                                  */
/*       HSX (0x201F0000)                                                     */
/*       BDX (0x201F0000)                                                     */
/* Register default value:              0x8086                                */
#define VID_PCU_FUN0_REG 0x0A002000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} VID_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* DID_PCU_FUN0_REG supported on:                                             */
/*       IVT_EP (0x20150002)                                                  */
/*       IVT_EX (0x20150002)                                                  */
/*       HSX (0x201F0002)                                                     */
/*       BDX (0x201F0002)                                                     */
/* Register default value on IVT_EP:    0x0EC0                                */
/* Register default value on IVT_EX:    0x0EC0                                */
/* Register default value on HSX:       0x2F98                                */
/* Register default value on BDX:       0x6F98                                */
#define DID_PCU_FUN0_REG 0x0A002002


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.0.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h2F98 (HSX), 16'h6F98 (BDX) 
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
} DID_PCU_FUN0_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PCICMD_PCU_FUN0_REG supported on:                                          */
/*       IVT_EP (0x20150004)                                                  */
/*       IVT_EX (0x20150004)                                                  */
/*       HSX (0x201F0004)                                                     */
/*       BDX (0x201F0004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_PCU_FUN0_REG 0x0A002004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} PCICMD_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* PCISTS_PCU_FUN0_REG supported on:                                          */
/*       IVT_EP (0x20150006)                                                  */
/*       IVT_EX (0x20150006)                                                  */
/*       HSX (0x201F0006)                                                     */
/*       BDX (0x201F0006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_PCU_FUN0_REG 0x0A002006
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} PCISTS_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* RID_PCU_FUN0_REG supported on:                                             */
/*       IVT_EP (0x10150008)                                                  */
/*       IVT_EX (0x10150008)                                                  */
/*       HSX (0x101F0008)                                                     */
/*       BDX (0x101F0008)                                                     */
/* Register default value:              0x00                                  */
#define RID_PCU_FUN0_REG 0x0A001008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} RID_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_PCU_FUN0_REG supported on:                                          */
/*       IVT_EP (0x10150009)                                                  */
/*       IVT_EX (0x10150009)                                                  */
/*       HSX (0x101F0009)                                                     */
/*       BDX (0x101F0009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_PCU_FUN0_REG 0x0A001009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.0.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_PCU_FUN0_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_PCU_FUN0_REG supported on:                                          */
/*       IVT_EP (0x2015000A)                                                  */
/*       IVT_EX (0x2015000A)                                                  */
/*       HSX (0x201F000A)                                                     */
/*       BDX (0x201F000A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_PCU_FUN0_REG 0x0A00200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} CCR_N1_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* CLSR_PCU_FUN0_REG supported on:                                            */
/*       IVT_EP (0x1015000C)                                                  */
/*       IVT_EX (0x1015000C)                                                  */
/*       HSX (0x101F000C)                                                     */
/*       BDX (0x101F000C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_PCU_FUN0_REG 0x0A00100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} CLSR_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* PLAT_PCU_FUN0_REG supported on:                                            */
/*       IVT_EP (0x1015000D)                                                  */
/*       IVT_EX (0x1015000D)                                                  */
/*       HSX (0x101F000D)                                                     */
/*       BDX (0x101F000D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_PCU_FUN0_REG 0x0A00100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} PLAT_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* HDR_PCU_FUN0_REG supported on:                                             */
/*       IVT_EP (0x1015000E)                                                  */
/*       IVT_EX (0x1015000E)                                                  */
/*       HSX (0x101F000E)                                                     */
/*       BDX (0x101F000E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_PCU_FUN0_REG 0x0A00100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} HDR_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* BIST_PCU_FUN0_REG supported on:                                            */
/*       IVT_EP (0x1015000F)                                                  */
/*       IVT_EX (0x1015000F)                                                  */
/*       HSX (0x101F000F)                                                     */
/*       BDX (0x101F000F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_PCU_FUN0_REG 0x0A00100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} BIST_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* SVID_PCU_FUN0_REG supported on:                                            */
/*       IVT_EP (0x2015002C)                                                  */
/*       IVT_EX (0x2015002C)                                                  */
/*       HSX (0x201F002C)                                                     */
/*       BDX (0x201F002C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_PCU_FUN0_REG 0x0A00202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} SVID_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* SDID_PCU_FUN0_REG supported on:                                            */
/*       IVT_EP (0x2015002E)                                                  */
/*       IVT_EX (0x2015002E)                                                  */
/*       HSX (0x201F002E)                                                     */
/*       BDX (0x201F002E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_PCU_FUN0_REG 0x0A00202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} SDID_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_PCU_FUN0_REG supported on:                                          */
/*       IVT_EP (0x10150034)                                                  */
/*       IVT_EX (0x10150034)                                                  */
/*       HSX (0x101F0034)                                                     */
/*       BDX (0x101F0034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_PCU_FUN0_REG 0x0A001034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} CAPPTR_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* INTL_PCU_FUN0_REG supported on:                                            */
/*       IVT_EP (0x1015003C)                                                  */
/*       IVT_EX (0x1015003C)                                                  */
/*       HSX (0x101F003C)                                                     */
/*       BDX (0x101F003C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_PCU_FUN0_REG 0x0A00103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} INTL_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* INTPIN_PCU_FUN0_REG supported on:                                          */
/*       IVT_EP (0x1015003D)                                                  */
/*       IVT_EX (0x1015003D)                                                  */
/*       HSX (0x101F003D)                                                     */
/*       BDX (0x101F003D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_PCU_FUN0_REG 0x0A00103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} INTPIN_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* MINGNT_PCU_FUN0_REG supported on:                                          */
/*       IVT_EP (0x1015003E)                                                  */
/*       IVT_EX (0x1015003E)                                                  */
/*       HSX (0x101F003E)                                                     */
/*       BDX (0x101F003E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_PCU_FUN0_REG 0x0A00103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} MINGNT_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_PCU_FUN0_REG supported on:                                          */
/*       IVT_EP (0x1015003F)                                                  */
/*       IVT_EX (0x1015003F)                                                  */
/*       HSX (0x101F003F)                                                     */
/*       BDX (0x101F003F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_PCU_FUN0_REG 0x0A00103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} MAXLAT_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* MEM_TRML_ESTIMATION_CONFIG_PCU_FUN0_REG supported on:                      */
/*       IVT_EP (0x4015004C)                                                  */
/*       IVT_EX (0x4015004C)                                                  */
/*       HSX (0x401F004C)                                                     */
/*       BDX (0x401F004C)                                                     */
/* Register default value on IVT_EP:    0x007FF3C7                            */
/* Register default value on IVT_EX:    0x007FF3C7                            */
/* Register default value on HSX:       0x438C8320                            */
/* Register default value on BDX:       0x438C8320                            */
#define MEM_TRML_ESTIMATION_CONFIG_PCU_FUN0_REG 0x0A00404C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * This register contains configuration regarding DDR temperature calculations that 
 * are done by PCODE.  
 * For the BW estimation mode, the following formula is used: 
 * Temperature = T(n) + AMBIENT 
 * where 
 * T(n) = T(n-1) - (1 - Alpha) * T(n-1) + Theta * BW 
 * This register is read by PCODE only during Reset Phase 4. 
 */
typedef union {
  struct {
    UINT32 disable_peci_control : 1;
    /* disable_peci_control - Bits[0:0], RW, default = 1'b0 
       Disable PECI control
     */
    UINT32 disable_bw_estimation : 1;
    /* disable_bw_estimation - Bits[1:1], RW, default = 1'b0 
       BW estimation disable
     */
    UINT32 disable_imc : 1;
    /* disable_imc - Bits[2:2], RW, default = 1'b0 
       Disable IMC
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[3:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ambient : 8;
    /* ambient - Bits[11:4], RW, default = 8'b00110010 
       The Ambient temperature in units of 1 degree (C).  This is relevant for BW-based 
       temperature estimation mode only (option 4). 
       The default value of 32h corresponds to 50 degrees C.
     */
    UINT32 ddr_temp_decay_factor : 10;
    /* ddr_temp_decay_factor - Bits[21:12], RW, default = 10'b0011001000 
       This factor is relevant only for BW based temperature estimation. It is equal to 
       "1 minus alpha".  
       The value of the decay factor (1 - alpha) is determined by DDR_TEMP_DECAY_FACTOR 
       / power(2,25) per 1 mSec.  
       
       
       Temperature decay factor: defines the decay factor per 1 mSec for the BW 
       estimation modes (see FW temperature calculation). Relevant  for BW based 
       temperature estimation (options 4 and 5). The value is decay_factor/2^16 per 1 
       mSec. 
     */
    UINT32 ddr_thermal_resistance : 10;
    /* ddr_thermal_resistance - Bits[31:22], RW, default = 10'b0100001110 
       The thermal resistance serves as a multiplier for the translation of the memory 
       BW to temperature. The units are given in 1 / power(2,44).  
       Was power(2,48).
       
       
       Thermal resistance: defines the thermal resistance. The thermal resistance 
       serves as a multiplier for the translation of the memory BW to temperature. 
     */
  } Bits;
  UINT32 Data;
} MEM_TRML_ESTIMATION_CONFIG_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* MEM_TRML_ESTIMATION_CONFIG2_PCU_FUN0_REG supported on:                     */
/*       IVT_EP (0x40150050)                                                  */
/*       IVT_EX (0x40150050)                                                  */
/*       HSX (0x401F0050)                                                     */
/*       BDX (0x401F0050)                                                     */
/* Register default value:              0x00000000                            */
#define MEM_TRML_ESTIMATION_CONFIG2_PCU_FUN0_REG 0x0A004050
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * This register is used in addition to 
 * MEM_TRML_ESTIMATION_CONFIG and will be used to set the power constant of the 
 * DDR.
 * This register is read by PCODE only during Reset Phase 4.
 */
typedef union {
  struct {
    UINT32 ddr_rank_static_power : 10;
    /* ddr_rank_static_power - Bits[9:0], RW, default = 10'b0000000000 
       The static power of each rank.
         This is in format of 3.7 bits in units of W 
         (or in units of 1 W /2^7)
     */
    UINT32 rsvd : 22;
    /* rsvd - Bits[31:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MEM_TRML_ESTIMATION_CONFIG2_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* MEM_TRML_TEMPERATURE_REPORT_0_PCU_FUN0_REG supported on:                   */
/*       HSX (0x401F0060)                                                     */
/*       BDX (0x401F0060)                                                     */
/* Register default value:              0x00000000                            */
#define MEM_TRML_TEMPERATURE_REPORT_0_PCU_FUN0_REG 0x0A004060

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} MEM_TRML_TEMPERATURE_REPORT_0_PCU_FUN0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MEM_ACCUMULATED_BW_CH_0_PCU_FUN0_REG supported on:                         */
/*       IVT_EP (0x40150064)                                                  */
/*       IVT_EX (0x40150064)                                                  */
/*       HSX (0x401F0064)                                                     */
/*       BDX (0x401F0064)                                                     */
/* Register default value:              0x00000000                            */
#define MEM_ACCUMULATED_BW_CH_0_PCU_FUN0_REG 0x0A004064
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} MEM_ACCUMULATED_BW_CH_0_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* MEM_ACCUMULATED_BW_CH_1_PCU_FUN0_REG supported on:                         */
/*       IVT_EP (0x40150068)                                                  */
/*       IVT_EX (0x40150068)                                                  */
/*       HSX (0x401F0068)                                                     */
/*       BDX (0x401F0068)                                                     */
/* Register default value:              0x00000000                            */
#define MEM_ACCUMULATED_BW_CH_1_PCU_FUN0_REG 0x0A004068
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} MEM_ACCUMULATED_BW_CH_1_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* MEM_ACCUMULATED_BW_CH_2_PCU_FUN0_REG supported on:                         */
/*       IVT_EP (0x4015006C)                                                  */
/*       IVT_EX (0x4015006C)                                                  */
/*       HSX (0x401F006C)                                                     */
/*       BDX (0x401F006C)                                                     */
/* Register default value:              0x00000000                            */
#define MEM_ACCUMULATED_BW_CH_2_PCU_FUN0_REG 0x0A00406C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} MEM_ACCUMULATED_BW_CH_2_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* MEM_ACCUMULATED_BW_CH_3_PCU_FUN0_REG supported on:                         */
/*       IVT_EP (0x40150070)                                                  */
/*       IVT_EX (0x40150070)                                                  */
/*       HSX (0x401F0070)                                                     */
/*       BDX (0x401F0070)                                                     */
/* Register default value:              0x00000000                            */
#define MEM_ACCUMULATED_BW_CH_3_PCU_FUN0_REG 0x0A004070
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
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
} MEM_ACCUMULATED_BW_CH_3_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* PCH_TEMP_PCU_FUN0_REG supported on:                                        */
/*       BDX (0x401F0074)                                                     */
/* Register default value:              0x00000000                            */
#define PCH_TEMP_PCU_FUN0_REG 0x0A004074

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * This register provides bios control on the usage model of pch 
 * temperature/throttling for BDX-DE . 
 */
typedef union {
  struct {
    UINT32 use_pch_hot : 1;
    /* use_pch_hot - Bits[0:0], RW, default = 1'b0 
       pcode is allowed to use PCH_HOT pin information for thermal 
     */
    UINT32 use_pch_temp_from_me : 1;
    /* use_pch_temp_from_me - Bits[1:1], RW, default = 1'b0 
       pcode is allowed to use PCH Temperature provided by ME 
     */
    UINT32 allow_pch_throttle : 1;
    /* allow_pch_throttle - Bits[2:2], RW, default = 1'b0 
       pcode is allowed to throttle PCH
     */
    UINT32 rsvd_3 : 5;
    /* rsvd_3 - Bits[7:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 t_control_offset : 9;
    /* t_control_offset - Bits[16:8], RW, default = 1'b0 
       T_CONTROL_OFFSET
     */
    UINT32 rsvd_17 : 15;
    /* rsvd_17 - Bits[31:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PCH_TEMP_PCU_FUN0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */




/* PACKAGE_POWER_SKU_N0_PCU_FUN0_REG supported on:                            */
/*       IVT_EP (0x40150084)                                                  */
/*       IVT_EX (0x40150084)                                                  */
/*       HSX (0x401F0084)                                                     */
/*       BDX (0x401F0084)                                                     */
/* Register default value on IVT_EP:    0x00780118                            */
/* Register default value on IVT_EX:    0x00780118                            */
/* Register default value on HSX:       0x00600118                            */
/* Register default value on BDX:       0x00600118                            */
#define PACKAGE_POWER_SKU_N0_PCU_FUN0_REG 0x0A004084
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * Defines allowed SKU power and timing parameters.
 * This CSR is a mirror of MSR (614h) PACKAGE_POWER_SKU.  Refer to this MSR for 
 * field descriptions. 
 */
typedef union {
  struct {
    UINT32 pkg_tdp : 15;
    /* pkg_tdp - Bits[14:0], RO_V, default = 15'b000000100011000 
       The TDP package power setting allowed for the SKU.  The TDP setting is typical 
       (not guaranteed). 
       The units for this value are defined in PACKAGE_POWER_SKU_MSR[PWR_UNIT].
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pkg_min_pwr : 15;
    /* pkg_min_pwr - Bits[30:16], ROS_V, default = 15'b000000001100000 
       The minimal package power setting allowed for this part.  Lower values will be 
       clamped to this value.  The minimum setting is typical (not guaranteed). 
       The units for this value are defined in PACKAGE_POWER_SKU_MSR[PWR_UNIT].
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PACKAGE_POWER_SKU_N0_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* PACKAGE_POWER_SKU_N1_PCU_FUN0_REG supported on:                            */
/*       IVT_EP (0x40150088)                                                  */
/*       IVT_EX (0x40150088)                                                  */
/*       HSX (0x401F0088)                                                     */
/*       BDX (0x401F0088)                                                     */
/* Register default value on IVT_EP:    0x00180258                            */
/* Register default value on IVT_EX:    0x00180258                            */
/* Register default value on HSX:       0x00120240                            */
/* Register default value on BDX:       0x00120240                            */
#define PACKAGE_POWER_SKU_N1_PCU_FUN0_REG 0x0A004088
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * Defines allowed SKU power and timing parameters.
 * This CSR is a mirror of MSR (614h) PACKAGE_POWER_SKU.  Refer to this MSR for 
 * field descriptions. 
 */
typedef union {
  struct {
    UINT32 pkg_max_pwr : 15;
    /* pkg_max_pwr - Bits[14:0], ROS_V, default = 15'b000001001000000 
       The maximal package power setting allowed for the SKU.  Higher values will be 
       clamped to this value.  The maximum setting is typical (not guaranteed). 
       The units for this value are defined in PACKAGE_POWER_SKU_MSR[PWR_UNIT].
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pkg_max_win : 7;
    /* pkg_max_win - Bits[22:16], ROS_V, default = 7'b0010010 
       The maximal time window allowed for the SKU.  Higher values will be clamped to 
       this value. 
       x = PKG_MAX_WIN[54:53]
       y = PKG_MAX_WIN[52:48]
       The timing interval window is Floating Point number given by 1.x * power(2,y).
       The unit of measurement is defined in PACKAGE_POWER_SKU_UNIT_MSR[TIME_UNIT].
     */
    UINT32 rsvd_23 : 9;
    /* rsvd_23 - Bits[31:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PACKAGE_POWER_SKU_N1_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* PACKAGE_POWER_SKU_UNIT_PCU_FUN0_REG supported on:                          */
/*       IVT_EP (0x4015008C)                                                  */
/*       IVT_EX (0x4015008C)                                                  */
/*       HSX (0x401F008C)                                                     */
/*       BDX (0x401F008C)                                                     */
/* Register default value on IVT_EP:    0x000A1003                            */
/* Register default value on IVT_EX:    0x000A1003                            */
/* Register default value on HSX:       0x000A0E03                            */
/* Register default value on BDX:       0x000A0E03                            */
#define PACKAGE_POWER_SKU_UNIT_PCU_FUN0_REG 0x0A00408C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * Defines units for calculating SKU power and timing parameters.
 * This CSR is a mirror of MSR (606h) PACKAGE_POWER_SKU_UNIT.  Refer to this MSR 
 * for field descriptions. 
 */
typedef union {
  struct {
    UINT32 pwr_unit : 4;
    /* pwr_unit - Bits[3:0], RO_V, default = 4'b0011 
       Power Units used for power control registers.
       The actual unit value is calculated by 1 W / Power(2,PWR_UNIT).
       The default value of 0011b corresponds to 1/8 W.
     */
    UINT32 rsvd_4 : 4;
    /* rsvd_4 - Bits[7:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 energy_unit : 5;
    /* energy_unit - Bits[12:8], RO_V, default = 5'h0e 
       Energy Units used for power control registers.
       The actual unit value is calculated by 1 J / Power(2,ENERGY_UNIT).
       The default value of 14 corresponds to Ux.14 number.
     */
    UINT32 rsvd_13 : 3;
    /* rsvd_13 - Bits[15:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 time_unit : 4;
    /* time_unit - Bits[19:16], RO_V, default = 4'b1010 
       Time Units used for power control registers.
       The actual unit value is calculated by 1 s / Power(2,TIME_UNIT).
       The default value of Ah corresponds to 976 usec.
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PACKAGE_POWER_SKU_UNIT_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* PACKAGE_ENERGY_STATUS_PCU_FUN0_REG supported on:                           */
/*       IVT_EP (0x40150090)                                                  */
/*       IVT_EX (0x40150090)                                                  */
/*       HSX (0x401F0090)                                                     */
/*       BDX (0x401F0090)                                                     */
/* Register default value:              0x00000000                            */
#define PACKAGE_ENERGY_STATUS_PCU_FUN0_REG 0x0A004090
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * Package energy consumed by the entire processor.  The counter will wrap around 
 * and continue counting when it reaches its limit. 
 * The energy status is reported in units which are defined in 
 * PACKAGE_POWER_SKU_UNIT_MSR[ENERGY_UNIT]. 
 * This CSR is a mirror of MSR (611h) PACKAGE_ENERGY_STATUS.  Refer to this MSR for 
 * field descriptions. 
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Energy Value
     */
  } Bits;
  UINT32 Data;
} PACKAGE_ENERGY_STATUS_PCU_FUN0_STRUCT;
#endif /* ASM_INC */




/* PLATFORM_ID_N1_PCU_FUN0_REG supported on:                                  */
/*       IVT_EP (0x401500A4)                                                  */
/*       IVT_EX (0x401500A4)                                                  */
/*       HSX (0x401F00A4)                                                     */
/*       BDX (0x401F00A4)                                                     */
/* Register default value:              0x00000000                            */
#define PLATFORM_ID_N1_PCU_FUN0_REG 0x0A0040A4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * Indicates the platform that the processor is intended for.  NOTE: For the 
 * processor family this MSR is used for microcode update loading purpose only. For 
 * all other device identification purposes it is recommended that System BIOS use 
 * PCI based Device Identification register. 
 * This CSR is a mirror of MSR (17h) IA32_PLATFORM_ID.  Refer to this MSR for field 
 * descriptions.  
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 18;
    /* rsvd_0 - Bits[17:0], n/a, default = n/a 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       Padding added by header generation tool.
     */
    UINT32 platformid : 3;
    /* platformid - Bits[20:18], ROS_V, default = 3'b000 
       The field gives information concerning the intended platform for the processor. 
       Bits 52, 51, 50 are concatenated to form the platform ID as shown below: 
     */
    UINT32 rsvd_21 : 11;
    /* rsvd_21 - Bits[31:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PLATFORM_ID_N1_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* PLATFORM_INFO_N0_PCU_FUN0_REG supported on:                                */
/*       IVT_EP (0x401500A8)                                                  */
/*       IVT_EX (0x401500A8)                                                  */
/*       HSX (0x401F00A8)                                                     */
/*       BDX (0x401F00A8)                                                     */
/* Register default value on IVT_EP:    0x70010000                            */
/* Register default value on IVT_EX:    0x70010000                            */
/* Register default value on HSX:       0xF0010000                            */
/* Register default value on BDX:       0xF0010000                            */
#define PLATFORM_INFO_N0_PCU_FUN0_REG 0x0A0040A8


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.0.CFG.xml.
 * Platform Info Register contains read-only package-level ratio information.
 * This CSR is a mirror of MSR (CEh) MSR_PLATFORM_INFO.  Refer to this MSR for 
 * field descriptions. 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 8;
    /* rsvd_0 - Bits[7:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 max_non_turbo_lim_ratio : 8;
    /* max_non_turbo_lim_ratio - Bits[15:8], ROS_V, default = 8'b00000000 
       Contains the max non-turbo ratio.
       Contains the max non-turbo ratio. This is the ratio of the frequency that 
       invariant TSC runs at. Frequency = ratio * 100 MHz. 
       Note: The Maximum Non-Turbo Ratio is adjusted to the flexible limit ratio (as 
       specified in the FLEX_RATIO MSR 194h FLEX_RATIO field bits [15:8]) if flexible 
       limit is enabled (by setting the FLEX_RATIO MSR 194h FLEX_EN field bit[16]=1. 
       Note: In case of Configurable TDP feature, the maximum of the available TDP 
       levels is reported in this field. 
     */
    UINT32 smm_save_cap : 1;
    /* smm_save_cap - Bits[16:16], RO_V, default = 1'b1 
       When set to '1' indicates this feature exists and is configured by 
       SMM_SAVE_CONTROL MSR 3Eh 
     */
    UINT32 tapunlock : 1;
    /* tapunlock - Bits[17:17], RO_V, default = 1'b0 
       Current Tap Unlock Status
     */
    UINT32 rsvd_18 : 5;
    /* rsvd_18 - Bits[22:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ppin_cap : 1;
    /* ppin_cap - Bits[23:23], RO_V, default = 1'b0 
       When set to 1, indicates that this part supports the Protected Processor 
       Inventory Number (PPIN) feature. 
     */
    UINT32 ocvolt_ovrd_avail : 1;
    /* ocvolt_ovrd_avail - Bits[24:24], RO_V, default = 1'b0 
       0b  Indicates that the part does not support Voltage override overclocking.
                            1b  Indicates that the part supports Voltage override 
       overclocking. 
                            
     */
    UINT32 rsvd_25 : 1;
    UINT32 dcu_16k_mode_avail : 1;
    /* dcu_16k_mode_avail - Bits[26:26], ROS_V, default = 1'b0 
       DCU 16K Mode Support
     */
    UINT32 sample_part : 1;
    /* sample_part - Bits[27:27], ROS_V, default = 1'b0 
       Indicates if this part is a sample
     */
    UINT32 prg_turbo_ratio_en : 1;
    /* prg_turbo_ratio_en - Bits[28:28], RO_V, default = 1'b1 
       Programmable Turbo Ratios per number of Active Cores
     */
    UINT32 prg_tdp_lim_en : 1;
    /* prg_tdp_lim_en - Bits[29:29], ROS_V, default = 1'b1 
       Programmable TDP Limits for Turbo Mode.
       When set to 1, indicates that TDP Limits for Turbo mode are programmable, and 
       when set to 0, indicates TDP Limits for Turbo mode are not programmable. When 
       this bit is 0, an attempt to write to PP0_POWER_LIMIT, PP1_POWER_LIMIT and 
       PACKAGE_POWER_LIMIT MSR will result in a GP fault. 
     */
    UINT32 prg_tj_offset_en : 1;
    /* prg_tj_offset_en - Bits[30:30], ROS_V, default = 1'b1 
       Programmable TJ Offset Enable.
       When set to 1, indicates that the TCC Activation Offset field in 
       IA32_TEMPERATURE_TARGET MSR is valid and programmable. When set to 0, indicates 
       it's not programmable. When this bit is 0, an attempt to write to 
       MSR_TEMPERATURE_TARGET bits [27:24] will result in a GP fault. 
     */
    UINT32 cpuid_faulting_en : 1;
    /* cpuid_faulting_en - Bits[31:31], ROS_V, default = 1'b1 
       When set to 1, indicates that the processor supports raising a #GP if CPUID is 
       executed when not in SMM and the CPL > 0. When this bit is set, it indicates 
       that (140h) MISC_FEATURE_ENABLES bit 0 can be written by a VMM. 
     */
  } Bits;
  UINT32 Data;
} PLATFORM_INFO_N0_PCU_FUN0_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PLATFORM_INFO_N1_PCU_FUN0_REG supported on:                                */
/*       IVT_EP (0x401500AC)                                                  */
/*       IVT_EX (0x401500AC)                                                  */
/*       HSX (0x401F00AC)                                                     */
/*       BDX (0x401F00AC)                                                     */
/* Register default value on IVT_EP:    0x00000000                            */
/* Register default value on IVT_EX:    0x00000000                            */
/* Register default value on HSX:       0x00080000                            */
/* Register default value on BDX:       0x00080000                            */
#define PLATFORM_INFO_N1_PCU_FUN0_REG 0x0A0040AC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.0.CFG.xml.
 * Platform Info Register contains read-only package-level ratio information.
 * This CSR is a mirror of MSR (CEh) MSR_PLATFORM_INFO.  Refer to this MSR for 
 * field descriptions. 
 */
typedef union {
  struct {
    UINT32 lpm_support : 1;
    /* lpm_support - Bits[0:0], ROS_V, default = 1'b0 
       When set to 1, indicates that BIOS may program IA32_PERF_CTL to levels below the 
       max efficiency ratio downto Minimum Ratio bits [55:48]. 
       0 - Low Power mode not supported
       1 - Low Power mode supported
     */
    UINT32 config_tdp_levels : 2;
    /* config_tdp_levels - Bits[2:1], ROS_V, default = 2'b00 
       Indicates the number of configurable TDP levels supported.
       00 - Config TDP not supported
       01 - One additional TDP level supported
       10 - Two additional TDP levels supported
     */
    UINT32 pfat_enable : 1;
    /* pfat_enable - Bits[3:3], ROS_V, default = 1'b0 
       0 indicates that the Platform Firmware Armoring Technology (PFAT) feature is not 
       available, 1 indicates that the PFAT feature is available.  
     */
    UINT32 rsvd_4 : 1;
    /* rsvd_4 - Bits[4:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 timed_mwait_enable : 1;
    /* timed_mwait_enable - Bits[5:5], ROS_V, default = 1'b0 
       0 indicates that Timed MWAIT feature is not available, 1 indicates that Timed 
       MWAIT feature is available. 
     */
    UINT32 rsvd_6 : 2;
    /* rsvd_6 - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 max_efficiency_ratio : 8;
    /* max_efficiency_ratio - Bits[15:8], ROS_V, default = 8'b00000000 
       Contains the maximum efficiency ratio in units of 100 MHz. System BIOS must use 
       this as the minimum ratio in the _PSS table. 
     */
    UINT32 min_operating_ratio : 8;
    /* min_operating_ratio - Bits[23:16], RO_V, default = 8'b00001000 
       Indicates the minimum ratio supported by the processor (in units of 100MHz)
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PLATFORM_INFO_N1_PCU_FUN0_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* TURBO_ACTIVATION_RATIO_PCU_FUN0_REG supported on:                          */
/*       HSX (0x401F00B0)                                                     */
/*       BDX (0x401F00B0)                                                     */
/* Register default value:              0x00000000                            */
#define TURBO_ACTIVATION_RATIO_PCU_FUN0_REG 0x0A0040B0

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * Read/write register to allow MSR/MMIO access to ACPI P-state notify (PCS 33).
 * This CSR is a mirror of MSR (64Ch) TURBO_ACTIVATION_RATIO.  Refer to this MSR 
 * for field descriptions. 
 */
typedef union {
  struct {
    UINT32 max_non_turbo_ratio : 8;
    /* max_non_turbo_ratio - Bits[7:0], RWS_L, default = 8'h00 
       CPU will treat any P-state request above this ratio as a request for max turbo
       0 is special encoding which disables the feature.
     */
    UINT32 rsvd : 23;
    /* rsvd - Bits[30:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 turbo_activation_ratio_lock : 1;
    /* turbo_activation_ratio_lock - Bits[31:31], RWS_KL, default = 1'b0 
       Lock this MSR  until next reset
       0 - unlocked
       1 - locked
     */
  } Bits;
  UINT32 Data;
} TURBO_ACTIVATION_RATIO_PCU_FUN0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PP0_ANY_THREAD_ACTIVITY_PCU_FUN0_REG supported on:                         */
/*       IVT_EP (0x401500B4)                                                  */
/*       IVT_EX (0x401500B4)                                                  */
/*       HSX (0x401F00B4)                                                     */
/*       BDX (0x401F00B4)                                                     */
/* Register default value:              0x00000000                            */
#define PP0_ANY_THREAD_ACTIVITY_PCU_FUN0_REG 0x0A0040B4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * This register will count the BCLK cycles in which at least one of the IA cores 
 * was active. 
 * This is a 32 bit accumulation done by PCU HW.  Values exceeding 32b will wrap 
 * around. 
 * This value can be used in conjunction with PP0_EFFICIENT_CYCLES and 
 * PP0_THREAD_ACTIVITY to generate statistics for SW. 
 *       
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Number of Cycles
     */
  } Bits;
  UINT32 Data;
} PP0_ANY_THREAD_ACTIVITY_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* PP0_EFFICIENT_CYCLES_PCU_FUN0_REG supported on:                            */
/*       IVT_EP (0x401500B8)                                                  */
/*       IVT_EX (0x401500B8)                                                  */
/*       HSX (0x401F00B8)                                                     */
/*       BDX (0x401F00B8)                                                     */
/* Register default value:              0x00000000                            */
#define PP0_EFFICIENT_CYCLES_PCU_FUN0_REG 0x0A0040B8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * This register will store a value equal to the product of the number of BCLK 
 * cycles in which at least one of the IA cores was active and the efficiency score 
 * calculated by the PCODE.  The efficiency score is a number between 0 and 1 that 
 * indicates the IA's efficiency. 
 * This is a 32 bit accumulation done by P-code to this register out of the 
 * PUSH-BUS.  Values exceeding 32b will wrap around. 
 *       
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Number of Cycles
     */
  } Bits;
  UINT32 Data;
} PP0_EFFICIENT_CYCLES_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* PP0_THREAD_ACTIVITY_PCU_FUN0_REG supported on:                             */
/*       IVT_EP (0x401500BC)                                                  */
/*       IVT_EX (0x401500BC)                                                  */
/*       HSX (0x401F00BC)                                                     */
/*       BDX (0x401F00BC)                                                     */
/* Register default value:              0x00000000                            */
#define PP0_THREAD_ACTIVITY_PCU_FUN0_REG 0x0A0040BC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * This register will store a value equal to the product of the number of BCLK 
 * cycles and the number of IA threads that are running.  This is a 32 bit 
 * accumulation done by PCU HW.  Values exceeding 32b will wrap around. 
 * 
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Number of Cycles
     */
  } Bits;
  UINT32 Data;
} PP0_THREAD_ACTIVITY_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* PKGC_LTR_LATENCY_MEASUREMENTS_PCU_FUN0_REG supported on:                   */
/*       HSX (0x401F00C4)                                                     */
/*       BDX (0x401F00C4)                                                     */
/* Register default value:              0x00000000                            */
#define PKGC_LTR_LATENCY_MEASUREMENTS_PCU_FUN0_REG 0x0A0040C4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * To measure memory transaction exit latencies, there are two hardware timers of 
 * 160 ns granularity. 
 *         These timers are in the unfiltered bclk domain, and one measures CLR 
 * exit latency and one DDR exit latency. 
 *       Pcode reads the timers, compares them to various thresholds, and if the 
 * timer value exceeds a threshold, pcode sets the corresponding bit in this IO 
 * register. 
 */
typedef union {
  struct {
    UINT32 ddr_access_time : 14;
    /* ddr_access_time - Bits[13:0], RO_V, default = 14'b0 
       DDR Access Time Value (in 160ns resolution)
     */
    UINT32 reserved : 1;
    /* reserved - Bits[14:14], RO_V, default = 1'b0 
       Reserved for future use.
     */
    UINT32 clr_access_time : 14;
    /* clr_access_time - Bits[28:15], RO_V, default = 14'b0 
       CLR Access Time Value (in 160ns resolution)
     */
    UINT32 non_snoop_threshold_resolution : 1;
    /* non_snoop_threshold_resolution - Bits[29:29], RO_V, default = 1'b0 
       Asserted if last wakeup time was greater than the non-snoop threshold for CLR 
       (when valid) and DDR. 
     */
    UINT32 slow_snoop_threshold_resolution : 1;
    /* slow_snoop_threshold_resolution - Bits[30:30], RO_V, default = 1'b0 
       Asserted if last wakeup time was greater than the slow snoop threshold for CLR 
       (when valid) and DDR. 
     */
    UINT32 fast_snoop_threshold_resolution : 1;
    /* fast_snoop_threshold_resolution - Bits[31:31], RO_V, default = 1'b0 
       Asserted if last wakeup time was greater than the fast snoop threshold for CLR 
       (when valid) and DDR. 
     */
  } Bits;
  UINT32 Data;
} PKGC_LTR_LATENCY_MEASUREMENTS_PCU_FUN0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PACKAGE_TEMPERATURE_PCU_FUN0_REG supported on:                             */
/*       IVT_EP (0x401500C8)                                                  */
/*       IVT_EX (0x401500C8)                                                  */
/*       HSX (0x401F00C8)                                                     */
/*       BDX (0x401F00C8)                                                     */
/* Register default value:              0x00000000                            */
#define PACKAGE_TEMPERATURE_PCU_FUN0_REG 0x0A0040C8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * Package temperature in degrees (C).   This field is updated by FW.
 */
typedef union {
  struct {
    UINT32 data : 8;
    /* data - Bits[7:0], RO_V, default = 8'b00000000 
       Package temperature in degrees (C).
     */
    UINT32 rsvd : 24;
    /* rsvd - Bits[31:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PACKAGE_TEMPERATURE_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* PP0_TEMPERATURE_PCU_FUN0_REG supported on:                                 */
/*       IVT_EP (0x401500CC)                                                  */
/*       IVT_EX (0x401500CC)                                                  */
/*       HSX (0x401F00CC)                                                     */
/*       BDX (0x401F00CC)                                                     */
/* Register default value:              0x00000000                            */
#define PP0_TEMPERATURE_PCU_FUN0_REG 0x0A0040CC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * PP0 temperature in degrees (C).   This field is updated by FW.
 */
typedef union {
  struct {
    UINT32 data : 8;
    /* data - Bits[7:0], RO_V, default = 8'b00000000 
       Temperature in degrees (C).
     */
    UINT32 rsvd : 24;
    /* rsvd - Bits[31:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PP0_TEMPERATURE_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* MRC_ODT_POWER_SAVING_PCU_FUN0_REG supported on:                            */
/*       HSX (0x401F00D0)                                                     */
/*       BDX (0x401F00D0)                                                     */
/* Register default value:              0x00000000                            */
#define MRC_ODT_POWER_SAVING_PCU_FUN0_REG 0x0A0040D0

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * Each field represents a percentage ODT power saving for the DDR command type. 
 * The range is 0 to ~100% with an increment of 1/256 ~= 0.391%. 
 */
typedef union {
  struct {
    UINT32 mrc_saving_rd : 8;
    /* mrc_saving_rd - Bits[7:0], RW_LB, default = 8'b0 
       Percentage ODT power saving for the DDR command. The range is 0 to ~100% with an 
       increment of 1/256 ~= 0.391%. 
     */
    UINT32 mrc_saving_wt : 8;
    /* mrc_saving_wt - Bits[15:8], RW_LB, default = 8'b0 
       Percentage ODT power saving for the DDR
               command. The range is 0 to ~100% with an increment of 1/256 ~= 0.391%.
     */
    UINT32 mrc_saving_cmd : 8;
    /* mrc_saving_cmd - Bits[23:16], RW_LB, default = 8'b0 
       Percentage ODT power saving for the DDR
               command. The range is 0 to ~100% with an increment of 1/256 ~=
               0.391%.
     */
    UINT32 reserved : 8;
    /* reserved - Bits[31:24], RW_LB, default = 8'b0  */
  } Bits;
  UINT32 Data;
} MRC_ODT_POWER_SAVING_PCU_FUN0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* P_STATE_LIMITS_PCU_FUN0_REG supported on:                                  */
/*       IVT_EP (0x401500D8)                                                  */
/*       IVT_EX (0x401500D8)                                                  */
/*       HSX (0x401F00D8)                                                     */
/*       BDX (0x401F00D8)                                                     */
/* Register default value:              0x000000FF                            */
#define P_STATE_LIMITS_PCU_FUN0_REG 0x0A0040D8


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.0.CFG.xml.
 * This register allows SW to limit the maximum frequency allowed during run-time.
 * PCODE will sample this register in slow loop.
 */
typedef union {
  struct {
    UINT32 pstt_lim : 8;
    /* pstt_lim - Bits[7:0], RW_L, default = 8'b11111111 
       This field indicates the maximum IA frequency limit allowed during run-time.
     */
    UINT32 rsvd_8 : 8;
    UINT32 rsvd : 15;
    /* rsvd - Bits[30:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 lock : 1;
    /* lock - Bits[31:31], RW_KL, default = 1'b0 
       This bit will lock all settings in this register.
     */
  } Bits;
  UINT32 Data;
} P_STATE_LIMITS_PCU_FUN0_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PACKAGE_THERM_MARGIN_PCU_FUN0_REG supported on:                            */
/*       IVT_EP (0x401500E0)                                                  */
/*       IVT_EX (0x401500E0)                                                  */
/*       HSX (0x401F00E0)                                                     */
/*       BDX (0x401F00E0)                                                     */
/* Register default value:              0x00000000                            */
#define PACKAGE_THERM_MARGIN_PCU_FUN0_REG 0x0A0040E0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * DTS2.0 Thermal Margin.
 * This CSR is a mirror of MSR (1A1h) PACKAGE_THERM_MARGIN.  Refer to this MSR for 
 * field descriptions. 
 */
typedef union {
  struct {
    UINT32 therm_margin : 16;
    /* therm_margin - Bits[15:0], RO_V, default = 16'b0000000000000000 
       Thermal margin in 8.8 format
     */
    UINT32 rsvd_16 : 16;
  } Bits;
  UINT32 Data;
} PACKAGE_THERM_MARGIN_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* TEMPERATURE_TARGET_PCU_FUN0_REG supported on:                              */
/*       IVT_EP (0x401500E4)                                                  */
/*       IVT_EX (0x401500E4)                                                  */
/*       HSX (0x401F00E4)                                                     */
/*       BDX (0x401F00E4)                                                     */
/* Register default value:              0x00000000                            */
#define TEMPERATURE_TARGET_PCU_FUN0_REG 0x0A0040E4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * Legacy register holding temperature related constants for Platform use.
 * This CSR is a mirror of MSR (1A2h) MSR_TEMPERATURE_TARGET.  Refer to this MSR 
 * for field descriptions. 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 8;
    /* rsvd_0 - Bits[7:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 fan_temp_target_ofst : 8;
    /* fan_temp_target_ofst - Bits[15:8], RO_V, default = 8'b00000000 
       Fan Temperature Target Offset (a.k.a. T-Control) indicates the relative offset 
       from the Thermal Monitor Trip Temperature at which fans should be engaged.  
     */
    UINT32 ref_temp : 8;
    /* ref_temp - Bits[23:16], RO_V, default = 8'b00000000 
       This field indicates the maximum junction temperature, also referred to as the 
       Throttle 
                 Temperature, TCC Activation Temperature or Prochot Temperature. This 
       is the temperature at which the Adaptive Thermal Monitor is activated.  
     */
    UINT32 tj_max_tcc_offset : 4;
    /* tj_max_tcc_offset - Bits[27:24], RO_V, default = 4'b0000 
       Temperature offset in degrees (C) from the TJ Max. Used for throttling 
       temperature. Will not impact 
                 temperature reading. If offset is allowed and set - the throttle will 
       occur and reported at lower then 
                 Tj_max.
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} TEMPERATURE_TARGET_PCU_FUN0_STRUCT;
#endif /* ASM_INC */


/* PACKAGE_RAPL_LIMIT_N0_PCU_FUN0_REG supported on:                           */
/*       HSX (0x401F00E8)                                                     */
/*       BDX (0x401F00E8)                                                     */
/* Register default value:              0x00000000                            */
#define PACKAGE_RAPL_LIMIT_N0_PCU_FUN0_REG 0x0A0040E8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * The Integrated Graphics driver, CPM driver, BIOS and OS can balance the power 
 * budget between the Primary Power Plane (IA) and the Secondary Power Plane (GT) 
 * via PRIMARY_PLANE_TURBO_POWER_LIMIT_MSR and 
 * SECONDARY_PLANE_TURBO_POWER_LIMIT_MSR. 
 */
typedef union {
  struct {
    UINT32 pkg_pwr_lim_1 : 15;
    /* pkg_pwr_lim_1 - Bits[14:0], RW_L, default = 15'b000000000000000 
       This field indicates the power limitation #1.
       The unit of measurement is defined in PACKAGE_POWER_SKU_UNIT_MSR[PWR_UNIT].
     */
    UINT32 pkg_pwr_lim_1_en : 1;
    /* pkg_pwr_lim_1_en - Bits[15:15], RW_L, default = 1'b0 
       This bit enables/disables PKG_PWR_LIM_1.
       0b  Package Power Limit 1 is Disabled
       1b  Package Power Limit 1 is Enabled
     */
    UINT32 pkg_clmp_lim_1 : 1;
    /* pkg_clmp_lim_1 - Bits[16:16], RW_L, default = 1'b0 
       Package Clamping limitation #1 - Allow going below P1.
       0b     PBM is limited between P1 and P0.
       1b     PBM can go below P1.
     */
    UINT32 pkg_pwr_lim_1_time : 7;
    /* pkg_pwr_lim_1_time - Bits[23:17], RW_L, default = 7'b0000000 
       x = PKG_PWR_LIM_1_TIME[23:22]
       y = PKG_PWR_LIM_1_TIME[21:17]
       The timing interval window is Floating Point number given by 1.x * power(2,y).
       The unit of measurement is defined in PACKAGE_POWER_SKU_UNIT_MSR[TIME_UNIT].
       The maximal time window is bounded by PACKAGE_POWER_SKU_MSR[PKG_MAX_WIN].  The 
       minimum time window is 1 unit of measurement (as defined above). 
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PACKAGE_RAPL_LIMIT_N0_PCU_FUN0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PACKAGE_RAPL_LIMIT_N1_PCU_FUN0_REG supported on:                           */
/*       HSX (0x401F00EC)                                                     */
/*       BDX (0x401F00EC)                                                     */
/* Register default value:              0x00000000                            */
#define PACKAGE_RAPL_LIMIT_N1_PCU_FUN0_REG 0x0A0040EC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * The Integrated Graphics driver, CPM driver, BIOS and OS can balance the power 
 * budget between the Primary Power Plane (IA) and the Secondary Power Plane (GT) 
 * via PRIMARY_PLANE_TURBO_POWER_LIMIT_MSR and 
 * SECONDARY_PLANE_TURBO_POWER_LIMIT_MSR. 
 */
typedef union {
  struct {
    UINT32 pkg_pwr_lim_2 : 15;
    /* pkg_pwr_lim_2 - Bits[14:0], RW_L, default = 15'b000000000000000 
       This field indicates the power limitation #2.
       The unit of measurement is defined in PACKAGE_POWER_SKU_UNIT_MSR[PWR_UNIT].
     */
    UINT32 pkg_pwr_lim_2_en : 1;
    /* pkg_pwr_lim_2_en - Bits[15:15], RW_L, default = 1'b0 
       This bit enables/disables PKG_PWR_LIM_2.
       0b  Package Power Limit 2 is Disabled
       1b  Package Power Limit 2 is Enabled
     */
    UINT32 pkg_clmp_lim_2 : 1;
    /* pkg_clmp_lim_2 - Bits[16:16], RW_L, default = 1'b0 
       Package Clamping limitation #2 - Allow going below P1.
       0b     PBM is limited between P1 and P0.
       1b     PBM can go below P1.
     */
    UINT32 pkg_pwr_lim_2_time : 7;
    /* pkg_pwr_lim_2_time - Bits[23:17], RW_L, default = 7'b0000000 
       x = PKG_PWR_LIM_2_TIME[55:54]
       y = PKG_PWR_LIM_2_TIME[53:49]
       The timing interval window is Floating Point number given by 1.x * power(2,y).
       The unit of measurement is defined in PACKAGE_POWER_SKU_UNIT_MSR[TIME_UNIT].
       The maximal time window is bounded by PACKAGE_POWER_SKU_MSR[PKG_MAX_WIN].
       The minimum time window is 1 unit of measurement (as defined above).
     */
    UINT32 rsvd : 7;
    /* rsvd - Bits[30:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pkg_pwr_lim_lock : 1;
    /* pkg_pwr_lim_lock - Bits[31:31], RW_KL, default = 1'b0 
       When set, all settings in this register are locked and are treated as Read Only.
       This bit will typically set by BIOS during boot time or resume from Sx.
     */
  } Bits;
  UINT32 Data;
} PACKAGE_RAPL_LIMIT_N1_PCU_FUN0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* VR_CURRENT_CONFIG_N0_PCU_FUN0_REG supported on:                            */
/*       HSX (0x401F00F8)                                                     */
/*       BDX (0x401F00F8)                                                     */
/* Register default value:              0x00000190                            */
#define VR_CURRENT_CONFIG_N0_PCU_FUN0_REG 0x0A0040F8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * Limitation on the maximum current consumption of the primary power plane.
 * This CSR is a mirror of MSR (601h) VR_CURRENT_CONFIG.  Refer to this MSR for 
 * field descriptions. 
 */
typedef union {
  struct {
    UINT32 current_limit : 13;
    /* current_limit - Bits[12:0], RO_V, default = 13'b0000110010000 
       Current limitation in 0.125 A increments.  This field is locked by 
       VR_CURRENT_CONFIG[LOCK].  When the LOCK bit is set to 1b, this field becomes 
       Read Only. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[30:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 lock : 1;
    /* lock - Bits[31:31], ROS_V, default = 1'b0 
       This bit will lock the CURRENT_LIMIT settings in this register and will also 
       lock this setting.  This means that once set to 1b, the CURRENT_LIMIT setting 
       and this bit become Read Only until the next Warm Reset. 
     */
  } Bits;
  UINT32 Data;
} VR_CURRENT_CONFIG_N0_PCU_FUN0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* VR_CURRENT_CONFIG_N1_PCU_FUN0_REG supported on:                            */
/*       HSX (0x401F00FC)                                                     */
/*       BDX (0x401F00FC)                                                     */
/* Register default value:              0x00000000                            */
#define VR_CURRENT_CONFIG_N1_PCU_FUN0_REG 0x0A0040FC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.0.CFG.xml.
 * Limitation on the maximum current consumption of the primary power plane.
 * This CSR is a mirror of MSR (601h) VR_CURRENT_CONFIG.  Refer to this MSR for 
 * field descriptions. 
 */
typedef union {
  struct {
    UINT32 psi1_threshold : 10;
    /* psi1_threshold - Bits[9:0], RO_V, default = 10'b0000000000 
       Maximum current in 1A units supported at external voltage regulator PS1
     */
    UINT32 psi2_threshold : 10;
    /* psi2_threshold - Bits[19:10], RO_V, default = 10'b0000000000 
       Maximum current in 1A units supported at external voltage regulator PS2
     */
    UINT32 psi3_threshold : 10;
    /* psi3_threshold - Bits[29:20], RO_V, default = 10'b0000000000 
       Maximum current in 1A units supported at external voltage regulator PS3
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} VR_CURRENT_CONFIG_N1_PCU_FUN0_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MEM_TRML_TEMPERATURE_REPORT_PCU_FUN0_REG supported on:                     */
/*       IVT_EP (0x40150060)                                                  */
/*       IVT_EX (0x40150060)                                                  */
/* Register default value:              0x00000000                            */
#define MEM_TRML_TEMPERATURE_REPORT_PCU_FUN0_REG 0x0A004060



/* PRIP_TURBO_PLCY_PCU_FUN0_REG supported on:                                 */
/*       IVT_EP (0x40150074)                                                  */
/*       IVT_EX (0x40150074)                                                  */
/* Register default value:              0x00000000                            */
#define PRIP_TURBO_PLCY_PCU_FUN0_REG 0x0A004074



/* TURBO_POWER_LIMIT_N0_PCU_FUN0_REG supported on:                            */
/*       IVT_EP (0x401500E8)                                                  */
/*       IVT_EX (0x401500E8)                                                  */
/* Register default value:              0x00000000                            */
#define TURBO_POWER_LIMIT_N0_PCU_FUN0_REG 0x0A0040E8



/* TURBO_POWER_LIMIT_N1_PCU_FUN0_REG supported on:                            */
/*       IVT_EP (0x401500EC)                                                  */
/*       IVT_EX (0x401500EC)                                                  */
/* Register default value:              0x00000000                            */
#define TURBO_POWER_LIMIT_N1_PCU_FUN0_REG 0x0A0040EC



/* PRIP_TURBO_PWR_LIM_PCU_FUN0_REG supported on:                              */
/*       IVT_EP (0x401500F0)                                                  */
/*       IVT_EX (0x401500F0)                                                  */
/* Register default value:              0x00000000                            */
#define PRIP_TURBO_PWR_LIM_PCU_FUN0_REG 0x0A0040F0



/* PRIMARY_PLANE_CURRENT_CONFIG_CONTROL_N0_PCU_FUN0_REG supported on:         */
/*       IVT_EP (0x401500F8)                                                  */
/*       IVT_EX (0x401500F8)                                                  */
/* Register default value:              0x00000438                            */
#define PRIMARY_PLANE_CURRENT_CONFIG_CONTROL_N0_PCU_FUN0_REG 0x0A0040F8



/* PRIMARY_PLANE_CURRENT_CONFIG_CONTROL_N1_PCU_FUN0_REG supported on:         */
/*       IVT_EP (0x401500FC)                                                  */
/*       IVT_EX (0x401500FC)                                                  */
/* Register default value:              0x18141494                            */
#define PRIMARY_PLANE_CURRENT_CONFIG_CONTROL_N1_PCU_FUN0_REG 0x0A0040FC



#endif /* PCU_FUN0_h */
