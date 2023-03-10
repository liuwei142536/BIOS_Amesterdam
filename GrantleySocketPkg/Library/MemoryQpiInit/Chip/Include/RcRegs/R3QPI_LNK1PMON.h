/* Date Stamp: 8/23/2014 */

#ifndef R3QPI_LNK1PMON_h
#define R3QPI_LNK1PMON_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* R3QPI_LNK1PMON_IVT_DEV 19                                                  */
/* R3QPI_LNK1PMON_IVT_FUN 6                                                   */
/* For HSX_HOST:                                                              */
/* R3QPI_LNK1PMON_HSX_DEV 11                                                  */
/* R3QPI_LNK1PMON_HSX_FUN 2                                                   */
/* For BDX_HOST:                                                              */
/* R3QPI_LNK1PMON_BDX_DEV 11                                                  */
/* R3QPI_LNK1PMON_BDX_FUN 2                                                   */

/* VID_R3QPI_LNK1PMON_REG supported on:                                       */
/*       IVT_EP (0x2019E000)                                                  */
/*       IVT_EX (0x2019E000)                                                  */
/*       HSX (0x2015A000)                                                     */
/*       BDX (0x2015A000)                                                     */
/* Register default value:              0x8086                                */
#define VID_R3QPI_LNK1PMON_REG 0x05022000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
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
} VID_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* DID_R3QPI_LNK1PMON_REG supported on:                                       */
/*       IVT_EP (0x2019E002)                                                  */
/*       IVT_EX (0x2019E002)                                                  */
/*       HSX (0x2015A002)                                                     */
/*       BDX (0x2015A002)                                                     */
/* Register default value on IVT_EP:    0x0E37                                */
/* Register default value on IVT_EX:    0x0E37                                */
/* Register default value on HSX:       0x2F37                                */
/* Register default value on BDX:       0x6F37                                */
#define DID_R3QPI_LNK1PMON_REG 0x05022002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6F37 
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
} DID_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* PCICMD_R3QPI_LNK1PMON_REG supported on:                                    */
/*       IVT_EP (0x2019E004)                                                  */
/*       IVT_EX (0x2019E004)                                                  */
/*       HSX (0x2015A004)                                                     */
/*       BDX (0x2015A004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_R3QPI_LNK1PMON_REG 0x05022004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
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
} PCICMD_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* PCISTS_R3QPI_LNK1PMON_REG supported on:                                    */
/*       IVT_EP (0x2019E006)                                                  */
/*       IVT_EX (0x2019E006)                                                  */
/*       HSX (0x2015A006)                                                     */
/*       BDX (0x2015A006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_R3QPI_LNK1PMON_REG 0x05022006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.2.CFG.xml.
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
} PCISTS_R3QPI_LNK1PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_R3QPI_LNK1PMON_REG supported on:                                       */
/*       IVT_EP (0x1019E008)                                                  */
/*       IVT_EX (0x1019E008)                                                  */
/*       HSX (0x1015A008)                                                     */
/*       BDX (0x1015A008)                                                     */
/* Register default value:              0x00                                  */
#define RID_R3QPI_LNK1PMON_REG 0x05021008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
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
} RID_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_R3QPI_LNK1PMON_REG supported on:                                    */
/*       IVT_EP (0x1019E009)                                                  */
/*       IVT_EX (0x1019E009)                                                  */
/*       HSX (0x1015A009)                                                     */
/*       BDX (0x1015A009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_R3QPI_LNK1PMON_REG 0x05021009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.2.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_R3QPI_LNK1PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_R3QPI_LNK1PMON_REG supported on:                                    */
/*       IVT_EP (0x2019E00A)                                                  */
/*       IVT_EX (0x2019E00A)                                                  */
/*       HSX (0x2015A00A)                                                     */
/*       BDX (0x2015A00A)                                                     */
/* Register default value:              0x1101                                */
#define CCR_N1_R3QPI_LNK1PMON_REG 0x0502200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT16 sub_class : 8;
    /* sub_class - Bits[7:0], RO_V, default = 8'b00000001 
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
    /* base_class - Bits[15:8], RO_V, default = 8'b00010001 
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
} CCR_N1_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* CLSR_R3QPI_LNK1PMON_REG supported on:                                      */
/*       IVT_EP (0x1019E00C)                                                  */
/*       IVT_EX (0x1019E00C)                                                  */
/*       HSX (0x1015A00C)                                                     */
/*       BDX (0x1015A00C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_R3QPI_LNK1PMON_REG 0x0502100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
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
} CLSR_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* PLAT_R3QPI_LNK1PMON_REG supported on:                                      */
/*       IVT_EP (0x1019E00D)                                                  */
/*       IVT_EX (0x1019E00D)                                                  */
/*       HSX (0x1015A00D)                                                     */
/*       BDX (0x1015A00D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_R3QPI_LNK1PMON_REG 0x0502100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
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
} PLAT_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* HDR_R3QPI_LNK1PMON_REG supported on:                                       */
/*       IVT_EP (0x1019E00E)                                                  */
/*       IVT_EX (0x1019E00E)                                                  */
/*       HSX (0x1015A00E)                                                     */
/*       BDX (0x1015A00E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_R3QPI_LNK1PMON_REG 0x0502100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
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
} HDR_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* BIST_R3QPI_LNK1PMON_REG supported on:                                      */
/*       IVT_EP (0x1019E00F)                                                  */
/*       IVT_EX (0x1019E00F)                                                  */
/*       HSX (0x1015A00F)                                                     */
/*       BDX (0x1015A00F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_R3QPI_LNK1PMON_REG 0x0502100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
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
} BIST_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* SVID_R3QPI_LNK1PMON_REG supported on:                                      */
/*       IVT_EP (0x2019E02C)                                                  */
/*       IVT_EX (0x2019E02C)                                                  */
/*       HSX (0x2015A02C)                                                     */
/*       BDX (0x2015A02C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_R3QPI_LNK1PMON_REG 0x0502202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
 * Subsystem Vendor Identification Number
 */
typedef union {
  struct {
    UINT16 subsystem_vendor_identification_number : 16;
    /* subsystem_vendor_identification_number - Bits[15:0], RW_O, default = 16'b1000000010000110 
       Subsystem Vendor Identification Number
     */
  } Bits;
  UINT16 Data;
} SVID_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* SDID_R3QPI_LNK1PMON_REG supported on:                                      */
/*       IVT_EP (0x2019E02E)                                                  */
/*       IVT_EX (0x2019E02E)                                                  */
/*       HSX (0x2015A02E)                                                     */
/*       BDX (0x2015A02E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_R3QPI_LNK1PMON_REG 0x0502202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
 * Subsystem Device ID
 */
typedef union {
  struct {
    UINT16 subsystem_device_identification_number : 16;
    /* subsystem_device_identification_number - Bits[15:0], RW_O, default = 16'b0000000000000000 
       Subsystem Device Identification Number
     */
  } Bits;
  UINT16 Data;
} SDID_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_R3QPI_LNK1PMON_REG supported on:                                    */
/*       IVT_EP (0x1019E034)                                                  */
/*       IVT_EX (0x1019E034)                                                  */
/*       HSX (0x1015A034)                                                     */
/*       BDX (0x1015A034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_R3QPI_LNK1PMON_REG 0x05021034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
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
} CAPPTR_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* INTL_R3QPI_LNK1PMON_REG supported on:                                      */
/*       IVT_EP (0x1019E03C)                                                  */
/*       IVT_EX (0x1019E03C)                                                  */
/*       HSX (0x1015A03C)                                                     */
/*       BDX (0x1015A03C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_R3QPI_LNK1PMON_REG 0x0502103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
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
} INTL_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* INTPIN_R3QPI_LNK1PMON_REG supported on:                                    */
/*       IVT_EP (0x1019E03D)                                                  */
/*       IVT_EX (0x1019E03D)                                                  */
/*       HSX (0x1015A03D)                                                     */
/*       BDX (0x1015A03D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_R3QPI_LNK1PMON_REG 0x0502103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
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
} INTPIN_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* MINGNT_R3QPI_LNK1PMON_REG supported on:                                    */
/*       IVT_EP (0x1019E03E)                                                  */
/*       IVT_EX (0x1019E03E)                                                  */
/*       HSX (0x1015A03E)                                                     */
/*       BDX (0x1015A03E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_R3QPI_LNK1PMON_REG 0x0502103E


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.2.CFG.xml.
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
} MINGNT_R3QPI_LNK1PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* MAXLAT_R3QPI_LNK1PMON_REG supported on:                                    */
/*       IVT_EP (0x1019E03F)                                                  */
/*       IVT_EX (0x1019E03F)                                                  */
/*       HSX (0x1015A03F)                                                     */
/*       BDX (0x1015A03F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_R3QPI_LNK1PMON_REG 0x0502103F


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.2.CFG.xml.
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
} MAXLAT_R3QPI_LNK1PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PMONCNTRLOWER1_0_R3QPI_LNK1PMON_REG supported on:                          */
/*       IVT_EP (0x4019E0A0)                                                  */
/*       IVT_EX (0x4019E0A0)                                                  */
/*       HSX (0x4015A0A0)                                                     */
/*       BDX (0x4015A0A0)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRLOWER1_0_R3QPI_LNK1PMON_REG 0x050240A0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
 * Performance Counter is a 48bit width counter. This is the Port1 lower portion of 
 * the counter0  
 */
typedef union {
  struct {
    UINT32 pmonctrdata : 32;
    /* pmonctrdata - Bits[31:0], RW_V, default = 32'b00000000000000000000000000000000 
       Counter0 Lower Data Field
     */
  } Bits;
  UINT32 Data;
} PMONCNTRLOWER1_0_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* PMONCNTRUPPER1_0_R3QPI_LNK1PMON_REG supported on:                          */
/*       IVT_EP (0x4019E0A4)                                                  */
/*       IVT_EX (0x4019E0A4)                                                  */
/*       HSX (0x4015A0A4)                                                     */
/*       BDX (0x4015A0A4)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRUPPER1_0_R3QPI_LNK1PMON_REG 0x050240A4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.2.CFG.xml.
 * This is the Port1 Upper portion of the Counter0
 */
typedef union {
  struct {
    UINT32 pmonctrdata : 16;
    /* pmonctrdata - Bits[15:0], RW_V, default = 16'b0000000000000000 
       Counter0 Upper Data Field
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PMONCNTRUPPER1_0_R3QPI_LNK1PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PMONCNTRLOWER1_1_R3QPI_LNK1PMON_REG supported on:                          */
/*       IVT_EP (0x4019E0A8)                                                  */
/*       IVT_EX (0x4019E0A8)                                                  */
/*       HSX (0x4015A0A8)                                                     */
/*       BDX (0x4015A0A8)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRLOWER1_1_R3QPI_LNK1PMON_REG 0x050240A8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
 * Performance Counter is a 48bit Width Counter. This is the Port1 lower portion of 
 * the Counter1 
 */
typedef union {
  struct {
    UINT32 pmonctrdata : 32;
    /* pmonctrdata - Bits[31:0], RW_V, default = 32'b00000000000000000000000000000000 
       This is the value of the counter lower portion
     */
  } Bits;
  UINT32 Data;
} PMONCNTRLOWER1_1_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* PMONCNTRUPPER1_1_R3QPI_LNK1PMON_REG supported on:                          */
/*       IVT_EP (0x4019E0AC)                                                  */
/*       IVT_EX (0x4019E0AC)                                                  */
/*       HSX (0x4015A0AC)                                                     */
/*       BDX (0x4015A0AC)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRUPPER1_1_R3QPI_LNK1PMON_REG 0x050240AC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.2.CFG.xml.
 * This is the Port1 upper portion of the Counter1 
 */
typedef union {
  struct {
    UINT32 pmonctrdata : 16;
    /* pmonctrdata - Bits[15:0], RW_V, default = 16'b0000000000000000 
       This is the value of the counter upper portion
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PMONCNTRUPPER1_1_R3QPI_LNK1PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PMONCNTRLOWER1_2_R3QPI_LNK1PMON_REG supported on:                          */
/*       IVT_EP (0x4019E0B0)                                                  */
/*       IVT_EX (0x4019E0B0)                                                  */
/*       HSX (0x4015A0B0)                                                     */
/*       BDX (0x4015A0B0)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRLOWER1_2_R3QPI_LNK1PMON_REG 0x050240B0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
 * Performance Counter is a 48bit Width Counter. This is the Port1 lower portion of 
 * the counter2 
 */
typedef union {
  struct {
    UINT32 pmonctrdata : 32;
    /* pmonctrdata - Bits[31:0], RW_V, default = 32'b00000000000000000000000000000000 
       This is the value of the counter lower portion
     */
  } Bits;
  UINT32 Data;
} PMONCNTRLOWER1_2_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* PMONCNTRUPPER1_2_R3QPI_LNK1PMON_REG supported on:                          */
/*       IVT_EP (0x4019E0B4)                                                  */
/*       IVT_EX (0x4019E0B4)                                                  */
/*       HSX (0x4015A0B4)                                                     */
/*       BDX (0x4015A0B4)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRUPPER1_2_R3QPI_LNK1PMON_REG 0x050240B4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.2.CFG.xml.
 * The is the Port1 upper portion of the counter2
 */
typedef union {
  struct {
    UINT32 pmonctrdata : 16;
    /* pmonctrdata - Bits[15:0], RW_V, default = 16'b0000000000000000 
       This is the value of the counter upper portion
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PMONCNTRUPPER1_2_R3QPI_LNK1PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PMONCNTRCFG1_0_R3QPI_LNK1PMON_REG supported on:                            */
/*       IVT_EP (0x4019E0D8)                                                  */
/*       IVT_EX (0x4019E0D8)                                                  */
/*       HSX (0x4015A0D8)                                                     */
/*       BDX (0x4015A0D8)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRCFG1_0_R3QPI_LNK1PMON_REG 0x050240D8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
 * Port1 Performance Counter Configuration Register0 
 */
typedef union {
  struct {
    UINT32 evslct : 8;
    /* evslct - Bits[7:0], RW_V, default = 8'b00000000 
       Pmon Events Selection. The encodings for each of the valid Uncore PerfMon events 
       can be found in the Perfmon HAS 
     */
    UINT32 unitmask : 8;
    /* unitmask - Bits[15:8], RW_V, default = 8'b00000000 
       Pmon Subevent Mask. This mask selects the sub-events for creating monitoring 
       events. At least one subevent must be selected for event signals to be asserted. 
       Events with no sub-events listed effectively have only one sub-event. 
     */
    UINT32 rsvd_16 : 1;
    /* rsvd_16 - Bits[16:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 counterreset : 1;
    /* counterreset - Bits[17:17], WO, default = 1'b0 
       Pmon Counter Reset. When this bit is set, the corresponding counter will be 
       reset to zero. This allows quick reset of the counter when changing event 
       encodings. 
     */
    UINT32 edgedet : 1;
    /* edgedet - Bits[18:18], RW_V, default = 1'b0 
       Pmon Counter Edge Detect. When this bit is asserted, the 0-to-1 transition edge 
       of a 1-bit event input will cause the corresponding Pmon counter to increment. 
       When this bit is 0, The Pmon counter will increment for whenever long the event 
       is asserted. This feature is only meaningful for 1 bit events. No edge detection 
       happens on events for which a zero threshold is set since edge detect on a 
       multi-bit events is not defined. A zero threshold disables edge detection.  
     */
    UINT32 rsvd_19 : 1;
    /* rsvd_19 - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ovfenable : 1;
    /* ovfenable - Bits[20:20], RW_V, default = 1'b0 
       Pmon Counter Overflow Enable. Setting this bit will enable the counter to send 
       an overflow signal. If this bit is not set, the counter will wrap around when it 
       overflows without triggering anything. If this bit is set and the configuration 
       register has overflow enabled, then a signal will be transmitted to the Ubox 
     */
    UINT32 internal : 1;
    /* internal - Bits[21:21], RW_V, default = 1'b0 
       This bit is unused. All Pmon CFGs are external
     */
    UINT32 counteren : 1;
    /* counteren - Bits[22:22], RW_V, default = 1'b0 
       Pmon Counter Enable. This field is the local enable for the Pmon counter. This 
       bit must be asserted in order for the Pmon counter to begin counting the events 
       which is selected by the event select and mask. Noted that if this bit is set 
       but the unit countrol register enable is not set. Then this counter will not 
       count. 
     */
    UINT32 invert : 1;
    /* invert - Bits[23:23], RW_V, default = 1'b0 
       This bit indicates how the threshold field will be compared to the incoming 
       event. When 0, the comparison that will be done on threshold larger than or 
       equal to event. When set to 1, the comparison that will be done on inverted from 
       the case where this bit is set to 0, i.e. threshold less than event. If the 
       threshold is set to 0 on an event that only has one bit, and the invert bit is 
       enabled, the counter will increment by 1 every cycle when the events is not 
       true.  
     */
    UINT32 threshold : 8;
    /* threshold - Bits[31:24], RW_V, default = 8'b00000000 
       This field is compared directly against an incoming event value for events that 
       can increment by 1 or more in a given cycle. Since the widest event from the 
       Uncore is 8bits, the result of the comparision is effectively a 1 bit wide 
       event, i.e., the counter will be incremented by 1 when the comparison is true, 
       no matter how wide the original event was. When this field is zero, threshold 
       comparision is disabled and the event is passed without modification. 
     */
  } Bits;
  UINT32 Data;
} PMONCNTRCFG1_0_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* PMONCNTRCFG1_1_R3QPI_LNK1PMON_REG supported on:                            */
/*       IVT_EP (0x4019E0DC)                                                  */
/*       IVT_EX (0x4019E0DC)                                                  */
/*       HSX (0x4015A0DC)                                                     */
/*       BDX (0x4015A0DC)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRCFG1_1_R3QPI_LNK1PMON_REG 0x050240DC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
 * Port1 Performance Counter Configuration Register1 
 */
typedef union {
  struct {
    UINT32 evslct : 8;
    /* evslct - Bits[7:0], RW_V, default = 8'b00000000 
       Pmon Events Selection. The encodings for each of the valid Uncore PerfMon events 
       can be found in the Perfmon HAS 
     */
    UINT32 unitmask : 8;
    /* unitmask - Bits[15:8], RW_V, default = 8'b00000000 
       Pmon Subevent Mask. This mask selects the sub-events for creating monitoring 
       events. At least one subevent must be selected for event signals to be asserted. 
       Events with no sub-events listed effectively have only one sub-event. 
     */
    UINT32 rsvd_16 : 1;
    /* rsvd_16 - Bits[16:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 counterreset : 1;
    /* counterreset - Bits[17:17], WO, default = 1'b0 
       Pmon Counter Reset. When this bit is set, the corresponding counter will be 
       reset to zero. This allows quick reset of the counter when changing event 
       encodings. 
     */
    UINT32 edgedet : 1;
    /* edgedet - Bits[18:18], RW_V, default = 1'b0 
       Pmon Counter Edge Detect. When this bit is asserted, the 0-to-1 transition edge 
       of a 1-bit event input will cause the corresponding Pmon counter to increment. 
       When this bit is 0, The Pmon counter will increment for whenever long the event 
       is asserted. This feature is only meaningful for 1 bit events. No edge detection 
       happens on events for which a zero threshold is set since edge detect on a 
       multi-bit events is not defined. A zero threshold disables edge detection.  
     */
    UINT32 rsvd_19 : 1;
    /* rsvd_19 - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ovfenable : 1;
    /* ovfenable - Bits[20:20], RW_V, default = 1'b0 
       Pmon Counter Overflow Enable. Setting this bit will enable the counter to send 
       an overflow signal. If this bit is not set, the counter will wrap around when it 
       overflows without triggering anything. If this bit is set and the configuration 
       register has overflow enabled, then a signal will be transmitted to the Ubox 
     */
    UINT32 internal : 1;
    /* internal - Bits[21:21], RW_V, default = 1'b0 
       This bit is unused. All Pmon CFGs are external
     */
    UINT32 counteren : 1;
    /* counteren - Bits[22:22], RW_V, default = 1'b0 
       Pmon Counter Enable. This field is the local enable for the Pmon counter. This 
       bit must be asserted in order for the Pmon counter to begin counting the events 
       which is selected by the event select and mask. Noted that if this bit is set 
       but the unit countrol register enable is not set. Then this counter will not 
       count. 
     */
    UINT32 invert : 1;
    /* invert - Bits[23:23], RW_V, default = 1'b0 
       This bit indicates how the threshold field will be compared to the incoming 
       event. When 0, the comparison that will be done on threshold larger than or 
       equal to event. When set to 1, the comparison that will be done on inverted from 
       the case where this bit is set to 0, i.e. threshold less than event. If the 
       threshold is set to 0 on an event that only has one bit, and the invert bit is 
       enabled, the counter will increment by 1 every cycle when the events is not 
       true.  
     */
    UINT32 threshold : 8;
    /* threshold - Bits[31:24], RW_V, default = 8'b00000000 
       This field is compared directly against an incoming event value for events that 
       can increment by 1 or more in a given cycle. Since the widest event from the 
       Uncore is 8bits, the result of the comparision is effectively a 1 bit wide 
       event, i.e., the counter will be incremented by 1 when the comparison is true, 
       no matter how wide the original event was. When this field is zero, threshold 
       comparision is disabled and the event is passed without modification. 
     */
  } Bits;
  UINT32 Data;
} PMONCNTRCFG1_1_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* PMONCNTRCFG1_2_R3QPI_LNK1PMON_REG supported on:                            */
/*       IVT_EP (0x4019E0E0)                                                  */
/*       IVT_EX (0x4019E0E0)                                                  */
/*       HSX (0x4015A0E0)                                                     */
/*       BDX (0x4015A0E0)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRCFG1_2_R3QPI_LNK1PMON_REG 0x050240E0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
 * Port1 Performance Counter Configuration Register2
 */
typedef union {
  struct {
    UINT32 evslct : 8;
    /* evslct - Bits[7:0], RW_V, default = 8'b00000000 
       Pmon Events Selection. The encodings for each of the valid Uncore PerfMon events 
       can be found in the Perfmon HAS 
     */
    UINT32 unitmask : 8;
    /* unitmask - Bits[15:8], RW_V, default = 8'b00000000 
       Pmon Subevent Mask. This mask selects the sub-events for creating monitoring 
       events. At least one subevent must be selected for event signals to be asserted. 
       Events with no sub-events listed effectively have only one sub-event. 
     */
    UINT32 rsvd_16 : 1;
    /* rsvd_16 - Bits[16:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 counterreset : 1;
    /* counterreset - Bits[17:17], WO, default = 1'b0 
       Pmon Counter Reset. When this bit is set, the corresponding counter will be 
       reset to zero. This allows quick reset of the counter when changing event 
       encodings. 
     */
    UINT32 edgedet : 1;
    /* edgedet - Bits[18:18], RW_V, default = 1'b0 
       Pmon Counter Edge Detect. When this bit is asserted, the 0-to-1 transition edge 
       of a 1-bit event input will cause the corresponding Pmon counter to increment. 
       When this bit is 0, The Pmon counter will increment for whenever long the event 
       is asserted. This feature is only meaningful for 1 bit events. No edge detection 
       happens on events for which a zero threshold is set since edge detect on a 
       multi-bit events is not defined. A zero threshold disables edge detection.  
     */
    UINT32 rsvd_19 : 1;
    /* rsvd_19 - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ovfenable : 1;
    /* ovfenable - Bits[20:20], RW_V, default = 1'b0 
       Pmon Counter Overflow Enable. Setting this bit will enable the counter to send 
       an overflow signal. If this bit is not set, the counter will wrap around when it 
       overflows without triggering anything. If this bit is set and the configuration 
       register has overflow enabled, then a signal will be transmitted to the Ubox 
     */
    UINT32 internal : 1;
    /* internal - Bits[21:21], RW_V, default = 1'b0 
       This bit is unused. All Pmon CFGs are external
     */
    UINT32 counteren : 1;
    /* counteren - Bits[22:22], RW_V, default = 1'b0 
       Pmon Counter Enable. This field is the local enable for the Pmon counter. This 
       bit must be asserted in order for the Pmon counter to begin counting the events 
       which is selected by the event select and mask. Noted that if this bit is set 
       but the unit countrol register enable is not set. Then this counter will not 
       count. 
     */
    UINT32 invert : 1;
    /* invert - Bits[23:23], RW_V, default = 1'b0 
       This bit indicates how the threshold field will be compared to the incoming 
       event. When 0, the comparison that will be done on threshold larger than or 
       equal to event. When set to 1, the comparison that will be done on inverted from 
       the case where this bit is set to 0, i.e. threshold less than event. If the 
       threshold is set to 0 on an event that only has one bit, and the invert bit is 
       enabled, the counter will increment by 1 every cycle when the events is not 
       true.  
     */
    UINT32 threshold : 8;
    /* threshold - Bits[31:24], RW_V, default = 8'b00000000 
       This field is compared directly against an incoming event value for events that 
       can increment by 1 or more in a given cycle. Since the widest event from the 
       Uncore is 8bits, the result of the comparision is effectively a 1 bit wide 
       event, i.e., the counter will be incremented by 1 when the comparison is true, 
       no matter how wide the original event was. When this field is zero, threshold 
       comparision is disabled and the event is passed without modification. 
     */
  } Bits;
  UINT32 Data;
} PMONCNTRCFG1_2_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


/* PMONUNITCTRL1_R3QPI_LNK1PMON_REG supported on:                             */
/*       IVT_EP (0x4019E0F4)                                                  */
/*       IVT_EX (0x4019E0F4)                                                  */
/*       HSX (0x4015A0F4)                                                     */
/*       BDX (0x4015A0F4)                                                     */
/* Register default value on IVT_EP:    0x00000000                            */
/* Register default value on IVT_EX:    0x00000000                            */
/* Register default value on HSX:       0x00030000                            */
/* Register default value on BDX:       0x00030000                            */
#define PMONUNITCTRL1_R3QPI_LNK1PMON_REG 0x050240F4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.11.2.CFG.xml.
 * Port1 Pmon Unit Control
 */
typedef union {
  struct {
    UINT32 resetcounterconfigs : 1;
    /* resetcounterconfigs - Bits[0:0], WO, default = 1'b0 
       When this bit is written, the counter configuration registers will be reset. 
       This does not affect the values in the counters  
     */
    UINT32 resetcounters : 1;
    /* resetcounters - Bits[1:1], WO, default = 1'b0 
       When this bit is written, the counters data fields will be reset. This 
       configuration values will not be reset. 
     */
    UINT32 rsvd_2 : 6;
    /* rsvd_2 - Bits[7:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 freezecounters : 1;
    /* freezecounters - Bits[8:8], RW_V, default = 1'b0 
       This bit is written when the counters should be frozen. If this bit is written 
       and freeze is enabled, The counters in the unit will stop counting. 
     */
    UINT32 rsvd_9 : 7;
    /* rsvd_9 - Bits[15:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 freezeenable : 1;
    /* freezeenable - Bits[16:16], RW, default = 1'b1 
       This bit controls what the counters in the unit will do when they receive a 
       freeze signal. When set, the counters will freeze. When not set, the counters 
       will ignore the freeze signal. 
     */
    UINT32 overflowenable : 1;
    /* overflowenable - Bits[17:17], RW, default = 1'b1 
       This bit controls the overflow behavior of the counters. When set, counter 
       overflow will trigger the overflow handling. potentially triggering a PMI and 
       freezing counters. When it is not set, the counters will simply wrap around and 
       continue to count. 
     */
    UINT32 rsvd_18 : 14;
    /* rsvd_18 - Bits[31:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PMONUNITCTRL1_R3QPI_LNK1PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PMONCTRSTATUS1_R3QPI_LNK1PMON_REG supported on:                            */
/*       IVT_EP (0x4019E0F8)                                                  */
/*       IVT_EX (0x4019E0F8)                                                  */
/*       HSX (0x4015A0F8)                                                     */
/*       BDX (0x4015A0F8)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCTRSTATUS1_R3QPI_LNK1PMON_REG 0x050240F8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.11.2.CFG.xml.
 * Port1 Performance Counter Status
 */
typedef union {
  struct {
    UINT32 counter0ovf : 1;
    /* counter0ovf - Bits[0:0], RW1C, default = 1'b0 
       Indicator for Counter0 Overflow
     */
    UINT32 counter1ovf : 1;
    /* counter1ovf - Bits[1:1], RW1C, default = 1'b0 
       Indicator for Counter1 Overflow
     */
    UINT32 counter2ovf : 1;
    /* counter2ovf - Bits[2:2], RW1C, default = 1'b0 
       Indicator for Counter2 Overflow
     */
    UINT32 rsvd : 29;
    /* rsvd - Bits[31:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PMONCTRSTATUS1_R3QPI_LNK1PMON_STRUCT;
#endif /* ASM_INC */


#endif /* R3QPI_LNK1PMON_h */
