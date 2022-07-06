/* Date Stamp: 8/23/2014 */

#ifndef PCU_FUN2_h
#define PCU_FUN2_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* PCU_FUN2_IVT_DEV 10                                                        */
/* PCU_FUN2_IVT_FUN 2                                                         */
/* For HSX_HOST:                                                              */
/* PCU_FUN2_HSX_DEV 30                                                        */
/* PCU_FUN2_HSX_FUN 2                                                         */
/* For BDX_HOST:                                                              */
/* PCU_FUN2_BDX_DEV 30                                                        */
/* PCU_FUN2_BDX_FUN 2                                                         */

/* VID_PCU_FUN2_REG supported on:                                             */
/*       IVT_EP (0x20152000)                                                  */
/*       IVT_EX (0x20152000)                                                  */
/*       HSX (0x201F2000)                                                     */
/*       BDX (0x201F2000)                                                     */
/* Register default value:              0x8086                                */
#define VID_PCU_FUN2_REG 0x0A022000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
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
} VID_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* DID_PCU_FUN2_REG supported on:                                             */
/*       IVT_EP (0x20152002)                                                  */
/*       IVT_EX (0x20152002)                                                  */
/*       HSX (0x201F2002)                                                     */
/*       BDX (0x201F2002)                                                     */
/* Register default value on IVT_EP:    0x0EC2                                */
/* Register default value on IVT_EX:    0x0EC2                                */
/* Register default value on HSX:       0x2F9A                                */
/* Register default value on BDX:       0x6F9A                                */
#define DID_PCU_FUN2_REG 0x0A022002


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.2.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h2F9A (HSX), 16'h6F9A (BDX) 
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
} DID_PCU_FUN2_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PCICMD_PCU_FUN2_REG supported on:                                          */
/*       IVT_EP (0x20152004)                                                  */
/*       IVT_EX (0x20152004)                                                  */
/*       HSX (0x201F2004)                                                     */
/*       BDX (0x201F2004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_PCU_FUN2_REG 0x0A022004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
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
} PCICMD_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* PCISTS_PCU_FUN2_REG supported on:                                          */
/*       IVT_EP (0x20152006)                                                  */
/*       IVT_EX (0x20152006)                                                  */
/*       HSX (0x201F2006)                                                     */
/*       BDX (0x201F2006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_PCU_FUN2_REG 0x0A022006
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
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
} PCISTS_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* RID_PCU_FUN2_REG supported on:                                             */
/*       IVT_EP (0x10152008)                                                  */
/*       IVT_EX (0x10152008)                                                  */
/*       HSX (0x101F2008)                                                     */
/*       BDX (0x101F2008)                                                     */
/* Register default value:              0x00                                  */
#define RID_PCU_FUN2_REG 0x0A021008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
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
} RID_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_PCU_FUN2_REG supported on:                                          */
/*       IVT_EP (0x10152009)                                                  */
/*       IVT_EX (0x10152009)                                                  */
/*       HSX (0x101F2009)                                                     */
/*       BDX (0x101F2009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_PCU_FUN2_REG 0x0A021009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.2.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_PCU_FUN2_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_PCU_FUN2_REG supported on:                                          */
/*       IVT_EP (0x2015200A)                                                  */
/*       IVT_EX (0x2015200A)                                                  */
/*       HSX (0x201F200A)                                                     */
/*       BDX (0x201F200A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_PCU_FUN2_REG 0x0A02200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
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
} CCR_N1_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* CLSR_PCU_FUN2_REG supported on:                                            */
/*       IVT_EP (0x1015200C)                                                  */
/*       IVT_EX (0x1015200C)                                                  */
/*       HSX (0x101F200C)                                                     */
/*       BDX (0x101F200C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_PCU_FUN2_REG 0x0A02100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
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
} CLSR_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* PLAT_PCU_FUN2_REG supported on:                                            */
/*       IVT_EP (0x1015200D)                                                  */
/*       IVT_EX (0x1015200D)                                                  */
/*       HSX (0x101F200D)                                                     */
/*       BDX (0x101F200D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_PCU_FUN2_REG 0x0A02100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
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
} PLAT_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* HDR_PCU_FUN2_REG supported on:                                             */
/*       IVT_EP (0x1015200E)                                                  */
/*       IVT_EX (0x1015200E)                                                  */
/*       HSX (0x101F200E)                                                     */
/*       BDX (0x101F200E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_PCU_FUN2_REG 0x0A02100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
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
} HDR_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* BIST_PCU_FUN2_REG supported on:                                            */
/*       IVT_EP (0x1015200F)                                                  */
/*       IVT_EX (0x1015200F)                                                  */
/*       HSX (0x101F200F)                                                     */
/*       BDX (0x101F200F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_PCU_FUN2_REG 0x0A02100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
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
} BIST_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* SVID_PCU_FUN2_REG supported on:                                            */
/*       IVT_EP (0x2015202C)                                                  */
/*       IVT_EX (0x2015202C)                                                  */
/*       HSX (0x201F202C)                                                     */
/*       BDX (0x201F202C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_PCU_FUN2_REG 0x0A02202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
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
} SVID_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* SDID_PCU_FUN2_REG supported on:                                            */
/*       IVT_EP (0x2015202E)                                                  */
/*       IVT_EX (0x2015202E)                                                  */
/*       HSX (0x201F202E)                                                     */
/*       BDX (0x201F202E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_PCU_FUN2_REG 0x0A02202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
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
} SDID_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_PCU_FUN2_REG supported on:                                          */
/*       IVT_EP (0x10152034)                                                  */
/*       IVT_EX (0x10152034)                                                  */
/*       HSX (0x101F2034)                                                     */
/*       BDX (0x101F2034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_PCU_FUN2_REG 0x0A021034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
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
} CAPPTR_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* INTL_PCU_FUN2_REG supported on:                                            */
/*       IVT_EP (0x1015203C)                                                  */
/*       IVT_EX (0x1015203C)                                                  */
/*       HSX (0x101F203C)                                                     */
/*       BDX (0x101F203C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_PCU_FUN2_REG 0x0A02103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
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
} INTL_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* INTPIN_PCU_FUN2_REG supported on:                                          */
/*       IVT_EP (0x1015203D)                                                  */
/*       IVT_EX (0x1015203D)                                                  */
/*       HSX (0x101F203D)                                                     */
/*       BDX (0x101F203D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_PCU_FUN2_REG 0x0A02103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
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
} INTPIN_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* MINGNT_PCU_FUN2_REG supported on:                                          */
/*       IVT_EP (0x1015203E)                                                  */
/*       IVT_EX (0x1015203E)                                                  */
/*       HSX (0x101F203E)                                                     */
/*       BDX (0x101F203E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_PCU_FUN2_REG 0x0A02103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
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
} MINGNT_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_PCU_FUN2_REG supported on:                                          */
/*       IVT_EP (0x1015203F)                                                  */
/*       IVT_EX (0x1015203F)                                                  */
/*       HSX (0x101F203F)                                                     */
/*       BDX (0x101F203F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_PCU_FUN2_REG 0x0A02103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
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
} MAXLAT_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* CPU_BUS_NUMBER_PCU_FUN2_REG supported on:                                  */
/*       IVT_EP (0x40152040)                                                  */
/*       IVT_EX (0x40152040)                                                  */
/*       HSX (0x401F2040)                                                     */
/*       BDX (0x401F2040)                                                     */
/* Register default value:              0x00000000                            */
#define CPU_BUS_NUMBER_PCU_FUN2_REG 0x0A024040
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * This register is used by BIOS to write the Bus number for 
 * the the socket. Pcode will use these values to determine whether PECI accesses 
 * are local or downstream. 
 * 
 */
typedef union {
  struct {
    UINT32 bus_no_0 : 8;
    /* bus_no_0 - Bits[7:0], RW_LB, default = 8'b00000000 
       Bus number for IIO on the local socket
     */
    UINT32 bus_no_1 : 8;
    /* bus_no_1 - Bits[15:8], RW_LB, default = 8'b00000000 
       Bus number for non-IIO entities on the local 
       socket
     */
    UINT32 rsvd : 15;
    /* rsvd - Bits[30:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 valid : 1;
    /* valid - Bits[31:31], RW_LB, default = 1'b0 
       Indicates whether the bus numbers have been initialized 
       or not
     */
  } Bits;
  UINT32 Data;
} CPU_BUS_NUMBER_PCU_FUN2_STRUCT;
#endif /* ASM_INC */








/* DYNAMIC_PERF_POWER_CTL_PCU_FUN2_REG supported on:                          */
/*       IVT_EP (0x40152064)                                                  */
/*       IVT_EX (0x40152064)                                                  */
/*       HSX (0x401F2064)                                                     */
/*       BDX (0x401F2064)                                                     */
/* Register default value:              0x00000000                            */
#define DYNAMIC_PERF_POWER_CTL_PCU_FUN2_REG 0x0A024064


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.2.CFG.xml.
 * This register effectively governs all major power saving 
 * engines and hueristics on the die.
 */
typedef union {
  struct {
    UINT32 qpi_apm_override_enable : 1;
    /* qpi_apm_override_enable - Bits[0:0], RW_V, default = 1'b0 
       0 disable over ride
       1 enable over ride
     */
    UINT32 rsvd_1 : 4;
    /* rsvd_1 - Bits[4:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 iom_apm_override_enable : 1;
    /* iom_apm_override_enable - Bits[5:5], RW_V, default = 1'b0 
       0 disable over ride
       1 enable over ride
     */
    UINT32 rsvd_6 : 4;
    /* rsvd_6 - Bits[9:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 imc_apm_override_enable : 1;
    /* imc_apm_override_enable - Bits[10:10], RW_V, default = 1'b0 
       0 disable over ride
       1 enable over ride
     */
    UINT32 rsvd_11 : 4;
    /* rsvd_11 - Bits[14:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 io_bw_plimit_override_enable : 1;
    /* io_bw_plimit_override_enable - Bits[15:15], RW_V, default = 1'b0 
       0 disable over ride
       1 enable over ride
     */
    UINT32 allow_peci_pcode_error_rsp : 1;
    /* allow_peci_pcode_error_rsp - Bits[16:16], RW_V, default = 1'b0 
       allow pcode to attempt to respond to peci requests after 
       a pcode MCA
     */
    UINT32 rsvd_17 : 3;
    UINT32 uncore_perf_plimit_override_enable : 1;
    /* uncore_perf_plimit_override_enable - Bits[20:20], RW_V, default = 1'b0 
       0 disable over ride
       1 enable over ride
     */
    UINT32 rsvd_21 : 1;
    UINT32 turbo_demotion_override_enable : 1;
    /* turbo_demotion_override_enable - Bits[22:22], RW_V, default = 1'b0 
       0 - Disable override
       1- Enable override
     */
    UINT32 cst_demotion_override_enable : 1;
    /* cst_demotion_override_enable - Bits[23:23], RW_V, default = 1'b0 
       0 - Disable override
       1- Enable override
     */
    UINT32 i_turbo_override_enable : 1;
    /* i_turbo_override_enable - Bits[24:24], RW_V, default = 1'b0 
       0 - Disable override
       1- Enable override
     */
    UINT32 eep_l_override_enable : 1;
    /* eep_l_override_enable - Bits[25:25], RW_V, default = 1'b0 
       0 disable over ride
       1 enable over ride
     */
    UINT32 eep_l_override : 4;
    /* eep_l_override - Bits[29:26], RW_V, default = 4'b0000 
       This indicates a EEP L override. Value 0-15
     */
    UINT32 sapm_osr_override : 1;
    /* sapm_osr_override - Bits[30:30], RW_V, default = 1'b0 
       Prevents SAPM control of self-refresh
     */
    UINT32 rsvd1 : 1;
    /* rsvd1 - Bits[31:31], RW_V, default = 1'b0 
       Reserved
     */
  } Bits;
  UINT32 Data;
} DYNAMIC_PERF_POWER_CTL_PCU_FUN2_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* GLOBAL_PKG_C_S_CONTROL_REGISTER_PCU_FUN2_REG supported on:                 */
/*       IVT_EP (0x4015206C)                                                  */
/*       IVT_EX (0x4015206C)                                                  */
/*       HSX (0x401F206C)                                                     */
/*       BDX (0x401F206C)                                                     */
/* Register default value:              0x00000000                            */
#define GLOBAL_PKG_C_S_CONTROL_REGISTER_PCU_FUN2_REG 0x0A02406C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * This CSR contains information on the master slave protocol.  Expectation is that 
 * BIOS will write this register during the initialization flow. 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    UINT32 am_i_master : 1;
    /* am_i_master - Bits[2:2], RWS, default = 1'b0 
       If set, socket is master.  Master socket will be the leady socket.  BIOS will 
       set this bit in the legacy socket. 
     */
    UINT32 rsvd_3 : 5;
    UINT32 my_nid : 3;
    /* my_nid - Bits[10:8], RWS, default = 3'b000 
       NID of this socket.
     */
    UINT32 rsvd_11 : 1;
    UINT32 master_nid : 3;
    /* master_nid - Bits[14:12], RWS, default = 3'b0 
       Master socket NID.  Can also be determined from the Socket0 entry in the NID MAP 
       register. 
     */
    UINT32 rsvd_15 : 17;
  } Bits;
  UINT32 Data;
} GLOBAL_PKG_C_S_CONTROL_REGISTER_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* GLOBAL_NID_SOCKET_0_TO_3_MAP_PCU_FUN2_REG supported on:                    */
/*       HSX (0x401F2070)                                                     */
/*       BDX (0x401F2070)                                                     */
/* Register default value:              0x00000000                            */
#define GLOBAL_NID_SOCKET_0_TO_3_MAP_PCU_FUN2_REG 0x0A024070

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * This register is in the PCU CR space.  It contains NID information for sockets 
 * 0-3 in the platform.  Expectation is that BIOS will write this register during 
 * the Reset/Init flow. 
 */
typedef union {
  struct {
    UINT32 skt0_nid : 3;
    /* skt0_nid - Bits[2:0], RW_LB, default = 3'b000 
       Socket0 NID
     */
    UINT32 rsvd_3 : 1;
    UINT32 skt1_nid : 3;
    /* skt1_nid - Bits[6:4], RW_LB, default = 3'b000 
       Socket1 NID
     */
    UINT32 rsvd_7 : 1;
    UINT32 skt2_nid : 3;
    /* skt2_nid - Bits[10:8], RW_LB, default = 3'b000 
       Socket2 NID
     */
    UINT32 rsvd_11 : 1;
    UINT32 skt3_nid : 3;
    /* skt3_nid - Bits[14:12], RW_LB, default = 3'b000 
       Socket3 NID
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
} GLOBAL_NID_SOCKET_0_TO_3_MAP_PCU_FUN2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PKG_CST_ENTRY_CRITERIA_MASK_PCU_FUN2_REG supported on:                     */
/*       IVT_EP (0x4015207C)                                                  */
/*       IVT_EX (0x4015207C)                                                  */
/*       HSX (0x401F207C)                                                     */
/*       BDX (0x401F207C)                                                     */
/* Register default value on IVT_EP:    0x53000000                            */
/* Register default value on IVT_EX:    0x53000000                            */
/* Register default value on HSX:       0x0E000000                            */
/* Register default value on BDX:       0x0E000000                            */
#define PKG_CST_ENTRY_CRITERIA_MASK_PCU_FUN2_REG 0x0A02407C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.2.CFG.xml.
 * This register is used to configure which events will be used as a gate for PC3 
 * entry.  Expectation is that BIOS will write this register based on the system 
 * config and devices in the system. 
 * 
 * It is expected that disabled QPI/PCIe links must report L1.
 */
typedef union {
  struct {
    UINT32 pcie_in_l0s : 11;
    /* pcie_in_l0s - Bits[10:0], RWS, default = 11'b00000000000 
       MSB = PCIe_10.  LSB = PCIe_0.
     */
    UINT32 pcie_in_l1 : 11;
    /* pcie_in_l1 - Bits[21:11], RWS, default = 11'b00000000000 
       MSB = PCIe10.  LSB=PCIe0.
     */
    UINT32 rsvd_22 : 3;
    UINT32 qpi_0_in_l1 : 1;
    /* qpi_0_in_l1 - Bits[25:25], RWS, default = 1'b1 
       When set to 1, QPI_0 must be in L1
     */
    UINT32 qpi_1_in_l1 : 1;
    /* qpi_1_in_l1 - Bits[26:26], RWS, default = 1'b1 
       When set to 1, QPI_1 is required to be in L1.
     */
    UINT32 qpi_2_in_l1 : 1;
    /* qpi_2_in_l1 - Bits[27:27], RWS, default = 1'b1 
       When set to 1, QPI_2 is required to be in L1.
     */
    UINT32 dram_in_sr : 1;
    /* dram_in_sr - Bits[28:28], RWS, default = 1'b0 
       When set to 1, DRAM must be in SR.
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} PKG_CST_ENTRY_CRITERIA_MASK_PCU_FUN2_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CORE_FIVR_ERR_LOG_PCU_FUN2_REG supported on:                               */
/*       HSX (0x401F2080)                                                     */
/*       BDX (0x401F2080)                                                     */
/* Register default value:              0x00000000                            */
#define CORE_FIVR_ERR_LOG_PCU_FUN2_REG 0x0A024080

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * Reports Core FIVR Faults 
 */
typedef union {
  struct {
    UINT32 fault_vector : 32;
    /* fault_vector - Bits[31:0], ROS_V, default = 32'b000000 
       Fault vector - Bits 23:0 correspond to cores which have a core IVR fault. Cores 
       with a core IVR fault will not come out of reset. This field has the same field 
       mapping as the CSR RESOLVED_CORES and results are reported in CSR 
       RESOLVED_CORES.  
     */
  } Bits;
  UINT32 Data;
} CORE_FIVR_ERR_LOG_PCU_FUN2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* UNCORE_FIVR_ERR_LOG_PCU_FUN2_REG supported on:                             */
/*       HSX (0x401F2084)                                                     */
/*       BDX (0x401F2084)                                                     */
/* Register default value:              0x00000000                            */
#define UNCORE_FIVR_ERR_LOG_PCU_FUN2_REG 0x0A024084

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * Reports Uncore FIVR Faults
 */
typedef union {
  struct {
    UINT32 fault_vector : 32;
    /* fault_vector - Bits[31:0], ROS_V, default = 32'b000000 
       Fault vector
     */
  } Bits;
  UINT32 Data;
} UNCORE_FIVR_ERR_LOG_PCU_FUN2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PACKAGE_RAPL_PERF_STATUS_PCU_FUN2_REG supported on:                        */
/*       HSX (0x401F2088)                                                     */
/*       BDX (0x401F2088)                                                     */
/* Register default value:              0x00000000                            */
#define PACKAGE_RAPL_PERF_STATUS_PCU_FUN2_REG 0x0A024088

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * This register is used by Pcode to report Package Power limit violations in the 
 * Platform PBM. 
 * Provides information on the performance impact of the RAPL power limit. 
 * Provides the total time for which PACKAGE was throttled because of the RAPL 
 * power limit. Throttling here is defined as going below O.S requested P-state.  
 * Total time = Reg value * 1s *1 /2^(time unit)
 * Usage model: Enables the O.S/Driver to learn about PACKAGE throttling as a 
 * result of RAPL limit. 
 * Can be used by other S/W components that control the PACKAGE power.
 * This CSR is a mirror of MSR (611h) PACKAGE_ENERGY_STATUS.  Refer to this MSR for 
 * field descriptions. 
 */
typedef union {
  struct {
    UINT32 pwr_limit_throttle_ctr : 32;
    /* pwr_limit_throttle_ctr - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Reports the number of times the Power limiting algorithm 
       had to clip the power limit due to hitting the lowest power state available.
       
       Accumulated PACKAGE throttled time
     */
  } Bits;
  UINT32 Data;
} PACKAGE_RAPL_PERF_STATUS_PCU_FUN2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DRAM_POWER_INFO_N0_PCU_FUN2_REG supported on:                              */
/*       IVT_EP (0x40152090)                                                  */
/*       IVT_EX (0x40152090)                                                  */
/*       HSX (0x401F2090)                                                     */
/*       BDX (0x401F2090)                                                     */
/* Register default value:              0x00780118                            */
#define DRAM_POWER_INFO_N0_PCU_FUN2_REG 0x0A024090
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * This CSR is a mirror of MSR (61Ch) DRAM_POWER_INFO.  Refer to this MSR for field 
 * descriptions. 
 */
typedef union {
  struct {
    UINT32 dram_tdp : 15;
    /* dram_tdp - Bits[14:0], RW_L, default = 15'b000000100011000 
       The Spec power allowed for DRAM.  The TDP setting is 
       typical (not guaranteed).
       The units for this value are defined in PACKAGE_POWER_SKU_UNIT.[POWER_UNIT]. 
                
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 dram_min_pwr : 15;
    /* dram_min_pwr - Bits[30:16], RW_L, default = 15'b000000001111000 
       The minimal power setting allowed for DRAM.  Lower values 
       will be clamped to this value.  The minimum setting is typical (not 
       guaranteed).
       The units for this value are defined in PACKAGE_POWER_SKU_UNIT.[POWER_UNIT].
     */
    UINT32 rsvd_31 : 1;
    /* rsvd_31 - Bits[31:31], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DRAM_POWER_INFO_N0_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* DRAM_POWER_INFO_N1_PCU_FUN2_REG supported on:                              */
/*       IVT_EP (0x40152094)                                                  */
/*       IVT_EX (0x40152094)                                                  */
/*       HSX (0x401F2094)                                                     */
/*       BDX (0x401F2094)                                                     */
/* Register default value:              0x00280258                            */
#define DRAM_POWER_INFO_N1_PCU_FUN2_REG 0x0A024094
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * This CSR is a mirror of MSR (61Ch) DRAM_POWER_INFO.  Refer to this MSR for field 
 * descriptions. 
 */
typedef union {
  struct {
    UINT32 dram_max_pwr : 15;
    /* dram_max_pwr - Bits[14:0], RW_L, default = 15'b000001001011000 
       The maximal power setting allowed for DRAM.  Higher 
       values will be clamped to this value.  The maximum setting is typical (not 
       guaranteed).
       The units for this value are defined in PACKAGE_POWER_SKU_UNIT.[POWER_UNIT].
     */
    UINT32 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 dram_max_win : 7;
    /* dram_max_win - Bits[22:16], RW_L, default = 7'b0101000 
       The maximal time window allowed for the DRAM.  Higher 
       values will be clamped to this value.
         
         x = PKG_MAX_WIN[54:53]
         y = PKG_MAX_WIN[52:48]
         
         The timing interval window is Floating Point number given by 1.x * 
       power(2,y).
       The unit of measurement is defined in PACKAGE_POWER_SKU_UNIT.[TIME_UNIT]. 
       
     */
    UINT32 rsvd_23 : 8;
    /* rsvd_23 - Bits[30:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 lock : 1;
    /* lock - Bits[31:31], RW_KL, default = 1'b0 
       Lock bit to lock the Register
     */
  } Bits;
  UINT32 Data;
} DRAM_POWER_INFO_N1_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* DRAM_ENERGY_STATUS_PCU_FUN2_REG supported on:                              */
/*       HSX (0x401F20A0)                                                     */
/*       BDX (0x401F20A0)                                                     */
/* Register default value:              0x00000000                            */
#define DRAM_ENERGY_STATUS_PCU_FUN2_REG 0x0A0240A0

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * DRAM energy consumed by all the DIMMS in all the Channels.  The counter will 
 * wrap around and continue counting when it reaches its limit.   
 * ENERGY_UNIT for DRAM domain is 15.3uJ.
 * The data is updated by PCODE and is Read Only for all SW.  
 * This CSR is a mirror of MSR (619h) DDR_ENERGY_STATUS.  Refer to this MSR for 
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
} DRAM_ENERGY_STATUS_PCU_FUN2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DRAM_ENERGY_STATUS_CH0_PCU_FUN2_REG supported on:                          */
/*       HSX (0x401F20A8)                                                     */
/*       BDX (0x401F20A8)                                                     */
/* Register default value:              0x00000000                            */
#define DRAM_ENERGY_STATUS_CH0_PCU_FUN2_REG 0x0A0240A8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * DRAM energy consumed by all the DIMMS in Channel0.  The counter will wrap around 
 * and continue counting when it reaches its limit.   
 * ENERGY_UNIT for DRAM domain is 15.3uJ.
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
} DRAM_ENERGY_STATUS_CH0_PCU_FUN2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DRAM_ENERGY_STATUS_CH1_PCU_FUN2_REG supported on:                          */
/*       HSX (0x401F20B0)                                                     */
/*       BDX (0x401F20B0)                                                     */
/* Register default value:              0x00000000                            */
#define DRAM_ENERGY_STATUS_CH1_PCU_FUN2_REG 0x0A0240B0

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * DRAM energy consumed by all the DIMMS in Channel1.  The counter will wrap around 
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
} DRAM_ENERGY_STATUS_CH1_PCU_FUN2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DRAM_ENERGY_STATUS_CH2_PCU_FUN2_REG supported on:                          */
/*       HSX (0x401F20B8)                                                     */
/*       BDX (0x401F20B8)                                                     */
/* Register default value:              0x00000000                            */
#define DRAM_ENERGY_STATUS_CH2_PCU_FUN2_REG 0x0A0240B8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * DRAM energy consumed by all the DIMMS in Channel2.  The counter will wrap around 
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
} DRAM_ENERGY_STATUS_CH2_PCU_FUN2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DRAM_ENERGY_STATUS_CH3_PCU_FUN2_REG supported on:                          */
/*       HSX (0x401F20C0)                                                     */
/*       BDX (0x401F20C0)                                                     */
/* Register default value:              0x00000000                            */
#define DRAM_ENERGY_STATUS_CH3_PCU_FUN2_REG 0x0A0240C0

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * DRAM energy consumed by all the DIMMS in Channel3.  The counter will wrap around 
 * and continue counting when it reaches its limit.   
 * The ENERGY_UNIT for DRAM domain is 15.3uJ 
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
} DRAM_ENERGY_STATUS_CH3_PCU_FUN2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DRAM_PLANE_POWER_LIMIT_N0_PCU_FUN2_REG supported on:                       */
/*       IVT_EP (0x401520C8)                                                  */
/*       IVT_EX (0x401520C8)                                                  */
/*       HSX (0x401F20C8)                                                     */
/*       BDX (0x401F20C8)                                                     */
/* Register default value:              0x00000000                            */
#define DRAM_PLANE_POWER_LIMIT_N0_PCU_FUN2_REG 0x0A0240C8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * This register is used by BIOS/OS/Integrated Graphics Driver/CPM Driver to limit 
 * the power budget of DRAM Plane. 
 * The overall package turbo power limitation is controlled by 
 * DRAM_PLANE_POWER_LIMIT. 
 * This CSR is a mirror of MSR  (618h) DRAM_PLANE_POWER_LIMIT.  Refer to this MSR 
 * for field descriptions. 
 */
typedef union {
  struct {
    UINT32 dram_pp_pwr_lim : 15;
    /* dram_pp_pwr_lim - Bits[14:0], RW_L, default = 15'b000000000000000 
       This is the power limitation on the IA cores power plane.
        The unit of measurement is defined in PACKAGE_POWER_SKU_UNIT.[POWER_UNIT].
     */
    UINT32 pwr_lim_ctrl_en : 1;
    /* pwr_lim_ctrl_en - Bits[15:15], RW_L, default = 1'b0 
       This bit must be set in order to limit the power of the DRAM power plane.
         
         0b     DRAM power plane power limitation is disabled
         1b     DRAM power plane power limitation is enabled
     */
    UINT32 reserved : 1;
    /* reserved - Bits[16:16], RO, default = 1'b0 
       Reserved
     */
    UINT32 ctrl_time_win : 7;
    /* ctrl_time_win - Bits[23:17], RW_L, default = 7'b0000000 
       x = CTRL_TIME_WIN[23:22]
         y = CTRL_TIME_WIN[21:17]
         
         The timing interval window is Floating Point number given by 1.x * power(2,y).
         
         The unit of measurement is defined in PACKAGE_POWER_SKU_UNIT_MSR[TIME_UNIT].
         
         The maximal time window is bounded by PACKAGE_POWER_SKU_MSR[PKG_MAX_WIN].  The 
       minimum time window is 1 unit of measurement (as defined above). 
     */
    UINT32 rsvd : 7;
    /* rsvd - Bits[30:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pp_pwr_lim_lock : 1;
    /* pp_pwr_lim_lock - Bits[31:31], RW_KL, default = 1'b0 
       When set, all settings in this register are locked and are treated as Read Only.
     */
  } Bits;
  UINT32 Data;
} DRAM_PLANE_POWER_LIMIT_N0_PCU_FUN2_STRUCT;
#endif /* ASM_INC */




/* DRAM_RAPL_PERF_STATUS_PCU_FUN2_REG supported on:                           */
/*       HSX (0x401F20D8)                                                     */
/*       BDX (0x401F20D8)                                                     */
/* Register default value:              0x00000000                            */
#define DRAM_RAPL_PERF_STATUS_PCU_FUN2_REG 0x0A0240D8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * This register is used by Pcode to report DRAM Plane Power limit violations in 
 * the Platform PBM. 
 * Provides information on the performance impact of the RAPL power limit. 
 * Provides the total time for which DRAM was throttled because of the RAPL power 
 * limit. Total time = Reg Value *1 s * 1/ (2^time_unit). 
 * Usage model: Enables the OS/Driver to learn about DRAM throttling as a result of 
 * RAPL limit. 
 * Can be used by other S/W components that control the DRAM power.
 * This CSR is a mirror of MSR  (61Bh) DDR_RAPL_PERF_STATUS.  Refer to this MSR for 
 * field descriptions. 
 */
typedef union {
  struct {
    UINT32 pwr_limit_throttle_ctr : 32;
    /* pwr_limit_throttle_ctr - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       Reports the number of times the Power limiting algorithm 
       had to clip the power limit due to hitting the lowest power state available.
       
       Accumulated DRAM throttled time
     */
  } Bits;
  UINT32 Data;
} DRAM_RAPL_PERF_STATUS_PCU_FUN2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* PERF_P_LIMIT_CONTROL_PCU_FUN2_REG supported on:                            */
/*       IVT_EP (0x401520E4)                                                  */
/*       IVT_EX (0x401520E4)                                                  */
/*       HSX (0x401F20E4)                                                     */
/*       BDX (0x401F20E4)                                                     */
/* Register default value on IVT_EP:    0x7D000000                            */
/* Register default value on IVT_EX:    0x7D000000                            */
/* Register default value on HSX:       0x7D000000                            */
/* Register default value on BDX:       0x00000000                            */
#define PERF_P_LIMIT_CONTROL_PCU_FUN2_REG 0x0A0240E4


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.2.CFG.xml.
 * This register is BIOS configurable.  Dual mapping will prevent additional fast 
 * path events or polling needs from PCODE.  HW does not use the CSR input, it is 
 * primarily used by PCODE.  Note that PERF_P_LIMIT_CLIP must be nominally 
 * configured to guaranteed frequency + 1, if turbo related actions are needed in 
 * slave sockets. 
 */
typedef union {
  struct {
    UINT32 perf_plimit_enable : 1;
    /* perf_plimit_enable - Bits[0:0], RW_V, default = 1'b0 
       Enable Performance P-limit feature
     */
    UINT32 perf_plimit_threshold : 5;
    /* perf_plimit_threshold - Bits[5:1], RW_V, default = 5'b00000 
       Uncore frequency threshold above which this socket will trigger the feature and 
       start trying to raise frequency of other sockets. 
     */
    UINT32 rsvd_6 : 1;
    UINT32 perf_plimit_clip : 5;
    /* perf_plimit_clip - Bits[11:7], RW_V, default = 5'b00000 
       Maximum value the floor is allowed to be set to for perf P-limit.
     */
    UINT32 rsvd_12 : 3;
    UINT32 perf_plimit_differential : 3;
    /* perf_plimit_differential - Bits[17:15], RW_V, default = 3'b000 
       Parameter used to tune how far below local socket frequency remote socket 
       frequency is allowed to be. Also impacts rate at which frequency drops when 
       feature disengages. 
     */
    UINT32 rsvd_18 : 14;
  } Bits;
  UINT32 Data;
} PERF_P_LIMIT_CONTROL_PCU_FUN2_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * This register is BIOS configurable.  Dual mapping will prevent additional fast 
 * path events or polling needs from PCODE.  HW does not use the CSR input, it is 
 * primarily used by PCODE.  Note that PERF_P_LIMIT_CLIP must be nominally 
 * configured to guaranteed frequency + 1, if turbo related actions are needed in 
 * slave sockets. 
 */
typedef union {
  struct {
    UINT32 perf_plimit_enable : 1;
    /* perf_plimit_enable - Bits[0:0], RW_V, default = 1'b0 
       Enable Performance P-limit feature
     */
    UINT32 perf_plimit_threshold : 7;
    /* perf_plimit_threshold - Bits[7:1], RW_V, default = 7'b000000 
       Uncore frequency threshold above which this socket will trigger the feature and 
       start trying to raise frequency of other sockets. 
     */
    UINT32 perf_plimit_clip : 7;
    /* perf_plimit_clip - Bits[14:8], RW_V, default = 7'b000000 
       Maximum value the floor is allowed to be set to for perf P-limit.
     */
    UINT32 perf_plimit_differential : 3;
    /* perf_plimit_differential - Bits[17:15], RW_V, default = 3'b000 
       Parameter used to tune how far below local socket frequency remote socket 
       frequency is allowed to be. Also impacts rate at which frequency drops when 
       feature disengages. 
     */
    UINT32 reserved : 14;
    /* reserved - Bits[31:18], RW_V, default = 14'h0000 
       Reserved.
     */
  } Bits;
  UINT32 Data;
} PERF_P_LIMIT_CONTROL_PCU_FUN2_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* IO_BANDWIDTH_P_LIMIT_CONTROL_PCU_FUN2_REG supported on:                    */
/*       IVT_EP (0x401520E8)                                                  */
/*       IVT_EX (0x401520E8)                                                  */
/*       HSX (0x401F20E8)                                                     */
/*       BDX (0x401F20E8)                                                     */
/* Register default value:              0x00018AB4                            */
#define IO_BANDWIDTH_P_LIMIT_CONTROL_PCU_FUN2_REG 0x0A0240E8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * Various controls
 */
typedef union {
  struct {
    UINT32 io_floor_1 : 3;
    /* io_floor_1 - Bits[2:0], RWS_V, default = 3'b100 
       IO Floor 1
     */
    UINT32 io_thresh_1 : 3;
    /* io_thresh_1 - Bits[5:3], RWS_V, default = 3'b110 
       IO Threshold 1
     */
    UINT32 io_floor_2 : 3;
    /* io_floor_2 - Bits[8:6], RWS_V, default = 3'b010 
       IO Floor 2
     */
    UINT32 io_thresh_2 : 3;
    /* io_thresh_2 - Bits[11:9], RWS_V, default = 3'b101 
       IO threshold 2
     */
    UINT32 io_floor_3 : 3;
    /* io_floor_3 - Bits[14:12], RWS_V, default = 3'b000 
       IO floor 3
     */
    UINT32 io_thresh_3 : 3;
    /* io_thresh_3 - Bits[17:15], RWS_V, default = 3'b011 
       IO Threshold 3
     */
    UINT32 qpi_floor_1 : 3;
    /* qpi_floor_1 - Bits[20:18], RWS_V, default = 3'b000 
       QPI Floor 1
     */
    UINT32 qpi_thresh_1 : 3;
    /* qpi_thresh_1 - Bits[23:21], RWS_V, default = 3'b000 
       QPI Threshold 1
     */
    UINT32 qpi_floor_2 : 3;
    /* qpi_floor_2 - Bits[26:24], RWS_V, default = 3'b000 
       QPI Floor 2
     */
    UINT32 qpi_thresh_2 : 3;
    /* qpi_thresh_2 - Bits[29:27], RWS_V, default = 3'b000 
       QPI Threshold 2
     */
    UINT32 rsvd_30 : 1;
    UINT32 ovrd_enable : 1;
    /* ovrd_enable - Bits[31:31], RW_V, default = 1'b0 
       IO_BW_PLIMIT Override Bit:
       0 - Disable
       1 - Enable
     */
  } Bits;
  UINT32 Data;
} IO_BANDWIDTH_P_LIMIT_CONTROL_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* MCA_ERR_SRC_LOG_PCU_FUN2_REG supported on:                                 */
/*       IVT_EP (0x401520EC)                                                  */
/*       IVT_EX (0x401520EC)                                                  */
/*       HSX (0x401F20EC)                                                     */
/*       BDX (0x401F20EC)                                                     */
/* Register default value:              0x00000000                            */
#define MCA_ERR_SRC_LOG_PCU_FUN2_REG 0x0A0240EC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.30.2.CFG.xml.
 * MCSourceLog is used by the PCU to log the error sources. This register is 
 * initialized to zeroes during reset. The PCU will set the relevant bits when the 
 * condition they represent appears. The PCU never clears the registers-the UBox or 
 * off-die entities should clear them when they are consumed, unless their 
 * processing involves taking down the platform. 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 5;
    UINT32 rsvd_5 : 13;
    /* rsvd_5 - Bits[17:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 msmi_mcerr_internal : 1;
    /* msmi_mcerr_internal - Bits[18:18], RWS_V, default = 1'b0 
       Internal error: This socket asserted MSMI_MCERR.
     */
    UINT32 msmi_ierr_internal : 1;
    /* msmi_ierr_internal - Bits[19:19], RWS_V, default = 1'b0 
       Internal error: This socket asserted MSMI_IERR.
     */
    UINT32 msmi_internal : 1;
    /* msmi_internal - Bits[20:20], RWS_V, default = 1'b0 
       Internal error: This socket asserted a MSMI#. This is OR (bit 19,18).
     */
    UINT32 msmi_mcerr : 1;
    /* msmi_mcerr - Bits[21:21], RWS_V, default = 1'b0 
       External error: The package observed MSMI_MCERR.
     */
    UINT32 msmi_ierr : 1;
    /* msmi_ierr - Bits[22:22], RWS_V, default = 1'b0 
       External error: The package observed MSMI_IERR.
     */
    UINT32 msmi : 1;
    /* msmi - Bits[23:23], RWS_V, default = 1'b0 
       External error: The package observed MSMI# (for any reason). 
       It is or(bit 22, bit21); functions as a Valid bit for the other two package 
       conditions. It has no effect when a local core is associated with the error. 
     */
    UINT32 rsvd_24 : 2;
    /* rsvd_24 - Bits[25:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 mcerr_internal : 1;
    /* mcerr_internal - Bits[26:26], RWS_V, default = 1'b0 
       Internal error: This socket asserted MCERR.
     */
    UINT32 ierr_internal : 1;
    /* ierr_internal - Bits[27:27], RWS_V, default = 1'b0 
       Internal error: This socket asserted IERR.
     */
    UINT32 caterr_internal : 1;
    /* caterr_internal - Bits[28:28], RWS_V, default = 1'b0 
       Internal error: This socket asserted a CATERR#. This is OR (bit 27,26).
     */
    UINT32 mcerr : 1;
    /* mcerr - Bits[29:29], RWS_V, default = 1'b0 
       External error: The package observed MCERR.
     */
    UINT32 ierr : 1;
    /* ierr - Bits[30:30], RWS_V, default = 1'b0 
       External error: The package observed IERR.
     */
    UINT32 caterr : 1;
    /* caterr - Bits[31:31], RWS_V, default = 1'b0 
       External error: The package observed CATERR# (for any reason). 
       It is or(bit 30, bit29); functions as a Valid bit for the other two package 
       conditions. It has no effect when a local core is associated with the error. 
     */
  } Bits;
  UINT32 Data;
} MCA_ERR_SRC_LOG_PCU_FUN2_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* THERMTRIP_CONFIG_PCU_FUN2_REG supported on:                                */
/*       IVT_EP (0x401520F8)                                                  */
/*       IVT_EX (0x401520F8)                                                  */
/*       HSX (0x401F20F8)                                                     */
/*       BDX (0x401F20F8)                                                     */
/* Register default value:              0x00000000                            */
#define THERMTRIP_CONFIG_PCU_FUN2_REG 0x0A0240F8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * This register is used to configure whether the Thermtrip signal only carries the 
 * processor Trip information, or does it carry the Mem trip information as well. 
 * The register will be used by HW to enable ORing of the memtrip info into the 
 * thermtrip OR tree. 
 */
typedef union {
  struct {
    UINT32 en_memtrip : 1;
    /* en_memtrip - Bits[0:0], RW, default = 1'b0 
       If set to 1, PCU will OR in the MEMtrip information into the ThermTrip OR Tree
       If set to 0, PCU will ignore the MEMtrip information and ThermTrip will just 
       have the processor indication.  
       
       Expect BIOS to Enable this in Phase4
     */
    UINT32 rsvd_1 : 3;
    UINT32 rsvd : 28;
    /* rsvd - Bits[31:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} THERMTRIP_CONFIG_PCU_FUN2_STRUCT;
#endif /* ASM_INC */


/* PMONPCODEFILTER_PCU_FUN2_REG supported on:                                 */
/*       HSX (0x401F20FC)                                                     */
/*       BDX (0x401F20FC)                                                     */
/* Register default value:              0xFFFFFFFF                            */
#define PMONPCODEFILTER_PCU_FUN2_REG 0x0A0240FC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.2.CFG.xml.
 * This register has three mappings depending on the type of Perfmon Events that 
 * are being counted.  
 * This register is read by Pcode and communicates the Masking information from the 
 * BIOS/SW to Pcode.  
 */
typedef union {
  struct {
    UINT32 filter : 32;
    /* filter - Bits[31:0], RW_V, default = 32'b11111111111111111111111111111111 
       Pcode makes the decision on how to interpret the 32-bit field
       Interpretation2:
       15:0 ThreadID
       Interpretation1:
       7:0 CoreID
       
       Interpretation0:
       31:24 Voltage/Frequency Range 3
       23:16 Voltage/Frequency Range 2
       15:8 Voltage/Frequency Range 1
       7:0 Voltage/Frequency Range 0
     */
  } Bits;
  UINT32 Data;
} PMONPCODEFILTER_PCU_FUN2_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SA_TEMPERATURE_PCU_FUN2_REG supported on:                                  */
/*       IVT_EP (0x40152044)                                                  */
/*       IVT_EX (0x40152044)                                                  */
/* Register default value:              0x00000000                            */
#define SA_TEMPERATURE_PCU_FUN2_REG 0x0A024044







/* BANDTIMERS2_PCU_FUN2_REG supported on:                                     */
/*       IVT_EP (0x40152050)                                                  */
/*       IVT_EX (0x40152050)                                                  */
/* Register default value:              0x00002FA8                            */
#define BANDTIMERS2_PCU_FUN2_REG 0x0A024050



/* GLOBAL_NID_MAP_REGISTER_0_PCU_FUN2_REG supported on:                       */
/*       IVT_EP (0x40152070)                                                  */
/*       IVT_EX (0x40152070)                                                  */
/* Register default value:              0x00000000                            */
#define GLOBAL_NID_MAP_REGISTER_0_PCU_FUN2_REG 0x0A024070



/* PRIMARY_PLANE_RAPL_PERF_STATUS_N0_PCU_FUN2_REG supported on:               */
/*       IVT_EP (0x40152080)                                                  */
/*       IVT_EX (0x40152080)                                                  */
/* Register default value:              0x00000000                            */
#define PRIMARY_PLANE_RAPL_PERF_STATUS_N0_PCU_FUN2_REG 0x0A024080





/* PACKAGE_RAPL_PERF_STATUS_N0_PCU_FUN2_REG supported on:                     */
/*       IVT_EP (0x40152088)                                                  */
/*       IVT_EX (0x40152088)                                                  */
/* Register default value:              0x00000000                            */
#define PACKAGE_RAPL_PERF_STATUS_N0_PCU_FUN2_REG 0x0A024088





/* DRAM_ENERGY_STATUS_N0_PCU_FUN2_REG supported on:                           */
/*       IVT_EP (0x401520A0)                                                  */
/*       IVT_EX (0x401520A0)                                                  */
/* Register default value:              0x00000000                            */
#define DRAM_ENERGY_STATUS_N0_PCU_FUN2_REG 0x0A0240A0





/* DRAM_ENERGY_STATUS_CH0_N0_PCU_FUN2_REG supported on:                       */
/*       IVT_EP (0x401520A8)                                                  */
/*       IVT_EX (0x401520A8)                                                  */
/* Register default value:              0x00000000                            */
#define DRAM_ENERGY_STATUS_CH0_N0_PCU_FUN2_REG 0x0A0240A8





/* DRAM_ENERGY_STATUS_CH1_N0_PCU_FUN2_REG supported on:                       */
/*       IVT_EP (0x401520B0)                                                  */
/*       IVT_EX (0x401520B0)                                                  */
/* Register default value:              0x00000000                            */
#define DRAM_ENERGY_STATUS_CH1_N0_PCU_FUN2_REG 0x0A0240B0





/* DRAM_ENERGY_STATUS_CH2_N0_PCU_FUN2_REG supported on:                       */
/*       IVT_EP (0x401520B8)                                                  */
/*       IVT_EX (0x401520B8)                                                  */
/* Register default value:              0x00000000                            */
#define DRAM_ENERGY_STATUS_CH2_N0_PCU_FUN2_REG 0x0A0240B8





/* DRAM_ENERGY_STATUS_CH3_N0_PCU_FUN2_REG supported on:                       */
/*       IVT_EP (0x401520C0)                                                  */
/*       IVT_EX (0x401520C0)                                                  */
/* Register default value:              0x00000000                            */
#define DRAM_ENERGY_STATUS_CH3_N0_PCU_FUN2_REG 0x0A0240C0





/* DRAM_RAPL_PERF_STATUS_N0_PCU_FUN2_REG supported on:                        */
/*       IVT_EP (0x401520D8)                                                  */
/*       IVT_EX (0x401520D8)                                                  */
/* Register default value:              0x00000000                            */
#define DRAM_RAPL_PERF_STATUS_N0_PCU_FUN2_REG 0x0A0240D8





/* SAPMTIMERS2_PCU_FUN2_REG supported on:                                     */
/*       IVT_EP (0x401520F0)                                                  */
/*       IVT_EX (0x401520F0)                                                  */
/* Register default value:              0x04001450                            */
#define SAPMTIMERS2_PCU_FUN2_REG 0x0A0240F0



/* SAPMTIMERS3_PCU_FUN2_REG supported on:                                     */
/*       IVT_EP (0x401520F4)                                                  */
/*       IVT_EX (0x401520F4)                                                  */
/* Register default value:              0x04001450                            */
#define SAPMTIMERS3_PCU_FUN2_REG 0x0A0240F4



/* PERFMON_PCODE_FILTER_PCU_FUN2_REG supported on:                            */
/*       IVT_EP (0x401520FC)                                                  */
/*       IVT_EX (0x401520FC)                                                  */
/* Register default value:              0xFFFFFFFF                            */
#define PERFMON_PCODE_FILTER_PCU_FUN2_REG 0x0A0240FC



#endif /* PCU_FUN2_h */
