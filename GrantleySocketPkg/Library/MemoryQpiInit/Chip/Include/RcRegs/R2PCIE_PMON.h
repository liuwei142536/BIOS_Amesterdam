/* Date Stamp: 8/23/2014 */

#ifndef R2PCIE_PMON_h
#define R2PCIE_PMON_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* R2PCIE_PMON_IVT_DEV 19                                                     */
/* R2PCIE_PMON_IVT_FUN 1                                                      */
/* For HSX_HOST:                                                              */
/* R2PCIE_PMON_HSX_DEV 16                                                     */
/* R2PCIE_PMON_HSX_FUN 1                                                      */
/* For BDX_HOST:                                                              */
/* R2PCIE_PMON_BDX_DEV 16                                                     */
/* R2PCIE_PMON_BDX_FUN 1                                                      */

/* VID_R2PCIE_PMON_REG supported on:                                          */
/*       IVT_EP (0x20199000)                                                  */
/*       IVT_EX (0x20199000)                                                  */
/*       HSX (0x20181000)                                                     */
/*       BDX (0x20181000)                                                     */
/* Register default value:              0x8086                                */
#define VID_R2PCIE_PMON_REG 0x07012000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
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
} VID_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* DID_R2PCIE_PMON_REG supported on:                                          */
/*       IVT_EP (0x20199002)                                                  */
/*       IVT_EX (0x20199002)                                                  */
/*       HSX (0x20181002)                                                     */
/*       BDX (0x20181002)                                                     */
/* Register default value on IVT_EP:    0x0E34                                */
/* Register default value on IVT_EX:    0x0E34                                */
/* Register default value on HSX:       0x2F34                                */
/* Register default value on BDX:       0x6F34                                */
#define DID_R2PCIE_PMON_REG 0x07012002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6F34 
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
} DID_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* PCICMD_R2PCIE_PMON_REG supported on:                                       */
/*       IVT_EP (0x20199004)                                                  */
/*       IVT_EX (0x20199004)                                                  */
/*       HSX (0x20181004)                                                     */
/*       BDX (0x20181004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_R2PCIE_PMON_REG 0x07012004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
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
} PCICMD_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* PCISTS_R2PCIE_PMON_REG supported on:                                       */
/*       IVT_EP (0x20199006)                                                  */
/*       IVT_EX (0x20199006)                                                  */
/*       HSX (0x20181006)                                                     */
/*       BDX (0x20181006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_R2PCIE_PMON_REG 0x07012006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.1.CFG.xml.
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
} PCISTS_R2PCIE_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_R2PCIE_PMON_REG supported on:                                          */
/*       IVT_EP (0x10199008)                                                  */
/*       IVT_EX (0x10199008)                                                  */
/*       HSX (0x10181008)                                                     */
/*       BDX (0x10181008)                                                     */
/* Register default value:              0x00                                  */
#define RID_R2PCIE_PMON_REG 0x07011008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
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
} RID_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_R2PCIE_PMON_REG supported on:                                       */
/*       IVT_EP (0x10199009)                                                  */
/*       IVT_EX (0x10199009)                                                  */
/*       HSX (0x10181009)                                                     */
/*       BDX (0x10181009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_R2PCIE_PMON_REG 0x07011009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.1.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_R2PCIE_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_R2PCIE_PMON_REG supported on:                                       */
/*       IVT_EP (0x2019900A)                                                  */
/*       IVT_EX (0x2019900A)                                                  */
/*       HSX (0x2018100A)                                                     */
/*       BDX (0x2018100A)                                                     */
/* Register default value:              0x1101                                */
#define CCR_N1_R2PCIE_PMON_REG 0x0701200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
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
} CCR_N1_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* CLSR_R2PCIE_PMON_REG supported on:                                         */
/*       IVT_EP (0x1019900C)                                                  */
/*       IVT_EX (0x1019900C)                                                  */
/*       HSX (0x1018100C)                                                     */
/*       BDX (0x1018100C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_R2PCIE_PMON_REG 0x0701100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
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
} CLSR_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* PLAT_R2PCIE_PMON_REG supported on:                                         */
/*       IVT_EP (0x1019900D)                                                  */
/*       IVT_EX (0x1019900D)                                                  */
/*       HSX (0x1018100D)                                                     */
/*       BDX (0x1018100D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_R2PCIE_PMON_REG 0x0701100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
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
} PLAT_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* HDR_R2PCIE_PMON_REG supported on:                                          */
/*       IVT_EP (0x1019900E)                                                  */
/*       IVT_EX (0x1019900E)                                                  */
/*       HSX (0x1018100E)                                                     */
/*       BDX (0x1018100E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_R2PCIE_PMON_REG 0x0701100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
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
} HDR_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* BIST_R2PCIE_PMON_REG supported on:                                         */
/*       IVT_EP (0x1019900F)                                                  */
/*       IVT_EX (0x1019900F)                                                  */
/*       HSX (0x1018100F)                                                     */
/*       BDX (0x1018100F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_R2PCIE_PMON_REG 0x0701100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
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
} BIST_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* SVID_R2PCIE_PMON_REG supported on:                                         */
/*       IVT_EP (0x2019902C)                                                  */
/*       IVT_EX (0x2019902C)                                                  */
/*       HSX (0x4018102C)                                                     */
/*       BDX (0x4018102C)                                                     */
/* Register default value on IVT_EP:    0x8086                                */
/* Register default value on IVT_EX:    0x8086                                */
/* Register default value on HSX:       0x00008086                            */
/* Register default value on BDX:       0x00008086                            */
#define SVID_R2PCIE_PMON_REG 0x0701C000


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.1.CFG.xml.
 */
typedef union {
  struct {
    UINT32 subsystem_vendor_identification_number : 16;
    /* subsystem_vendor_identification_number - Bits[15:0], RW_O, default = 16'b1000000010000110  */
    UINT32 subsystem_device_identification_number : 16;
    /* subsystem_device_identification_number - Bits[31:16], RW_O, default = 16'b0000000000000000  */
  } Bits;
  UINT32 Data;
} SVID_R2PCIE_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CAPPTR_R2PCIE_PMON_REG supported on:                                       */
/*       IVT_EP (0x10199034)                                                  */
/*       IVT_EX (0x10199034)                                                  */
/*       HSX (0x10181034)                                                     */
/*       BDX (0x10181034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_R2PCIE_PMON_REG 0x07011034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
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
} CAPPTR_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* INTL_R2PCIE_PMON_REG supported on:                                         */
/*       IVT_EP (0x1019903C)                                                  */
/*       IVT_EX (0x1019903C)                                                  */
/*       HSX (0x1018103C)                                                     */
/*       BDX (0x1018103C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_R2PCIE_PMON_REG 0x0701103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
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
} INTL_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* INTPIN_R2PCIE_PMON_REG supported on:                                       */
/*       IVT_EP (0x1019903D)                                                  */
/*       IVT_EX (0x1019903D)                                                  */
/*       HSX (0x1018103D)                                                     */
/*       BDX (0x1018103D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_R2PCIE_PMON_REG 0x0701103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
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
} INTPIN_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* MINGNT_R2PCIE_PMON_REG supported on:                                       */
/*       IVT_EP (0x1019903E)                                                  */
/*       IVT_EX (0x1019903E)                                                  */
/*       HSX (0x1018103E)                                                     */
/*       BDX (0x1018103E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_R2PCIE_PMON_REG 0x0701103E


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.1.CFG.xml.
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
} MINGNT_R2PCIE_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* MAXLAT_R2PCIE_PMON_REG supported on:                                       */
/*       IVT_EP (0x1019903F)                                                  */
/*       IVT_EX (0x1019903F)                                                  */
/*       HSX (0x1018103F)                                                     */
/*       BDX (0x1018103F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_R2PCIE_PMON_REG 0x0701103F


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.1.CFG.xml.
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
} MAXLAT_R2PCIE_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PMONCNTRLOWER0_0_R2PCIE_PMON_REG supported on:                             */
/*       IVT_EP (0x401990A0)                                                  */
/*       IVT_EX (0x401990A0)                                                  */
/*       HSX (0x401810A0)                                                     */
/*       BDX (0x401810A0)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRLOWER0_0_R2PCIE_PMON_REG 0x070140A0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
 * This register is a perfmon counter. Software can both read it and write it.
 */
typedef union {
  struct {
    UINT32 pmonctrdata : 32;
    /* pmonctrdata - Bits[31:0], RW_V, default = 32'b00000000000000000000000000000000 
       This is the current value of the counter.
     */
  } Bits;
  UINT32 Data;
} PMONCNTRLOWER0_0_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* PMONCNTRUPPER0_0_R2PCIE_PMON_REG supported on:                             */
/*       IVT_EP (0x401990A4)                                                  */
/*       IVT_EX (0x401990A4)                                                  */
/*       HSX (0x401810A4)                                                     */
/*       BDX (0x401810A4)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRUPPER0_0_R2PCIE_PMON_REG 0x070140A4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.1.CFG.xml.
 * This register is a perfmon counter. Software can both read it and write it.
 */
typedef union {
  struct {
    UINT32 pmonctrdata : 16;
    /* pmonctrdata - Bits[15:0], RW_V, default = 16'b0000000000000000 
       This is the current value of the counter.
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PMONCNTRUPPER0_0_R2PCIE_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PMONCNTRLOWER0_1_R2PCIE_PMON_REG supported on:                             */
/*       IVT_EP (0x401990A8)                                                  */
/*       IVT_EX (0x401990A8)                                                  */
/*       HSX (0x401810A8)                                                     */
/*       BDX (0x401810A8)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRLOWER0_1_R2PCIE_PMON_REG 0x070140A8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
 * This register is a perfmon counter. Software can both read it and write it.
 */
typedef union {
  struct {
    UINT32 pmonctrdata : 32;
    /* pmonctrdata - Bits[31:0], RW_V, default = 32'b00000000000000000000000000000000 
       This is the current value of the counter.
     */
  } Bits;
  UINT32 Data;
} PMONCNTRLOWER0_1_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* PMONCNTRUPPER0_1_R2PCIE_PMON_REG supported on:                             */
/*       IVT_EP (0x401990AC)                                                  */
/*       IVT_EX (0x401990AC)                                                  */
/*       HSX (0x401810AC)                                                     */
/*       BDX (0x401810AC)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRUPPER0_1_R2PCIE_PMON_REG 0x070140AC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.1.CFG.xml.
 * This register is a perfmon counter. Software can both read it and write it.
 */
typedef union {
  struct {
    UINT32 pmonctrdata : 16;
    /* pmonctrdata - Bits[15:0], RW_V, default = 16'b0000000000000000 
       This is the current value of the counter.
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PMONCNTRUPPER0_1_R2PCIE_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PMONCNTRLOWER0_2_R2PCIE_PMON_REG supported on:                             */
/*       IVT_EP (0x401990B0)                                                  */
/*       IVT_EX (0x401990B0)                                                  */
/*       HSX (0x401810B0)                                                     */
/*       BDX (0x401810B0)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRLOWER0_2_R2PCIE_PMON_REG 0x070140B0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
 * This register is a perfmon counter. Software can both read it and write it.
 */
typedef union {
  struct {
    UINT32 pmonctrdata : 32;
    /* pmonctrdata - Bits[31:0], RW_V, default = 32'b00000000000000000000000000000000 
       This is the current value of the counter.
     */
  } Bits;
  UINT32 Data;
} PMONCNTRLOWER0_2_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* PMONCNTRUPPER0_2_R2PCIE_PMON_REG supported on:                             */
/*       IVT_EP (0x401990B4)                                                  */
/*       IVT_EX (0x401990B4)                                                  */
/*       HSX (0x401810B4)                                                     */
/*       BDX (0x401810B4)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRUPPER0_2_R2PCIE_PMON_REG 0x070140B4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.1.CFG.xml.
 * This register is a perfmon counter. Software can both read it and write it.
 */
typedef union {
  struct {
    UINT32 pmonctrdata : 16;
    /* pmonctrdata - Bits[15:0], RW_V, default = 16'b0000000000000000 
       This is the current value of the counter.
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PMONCNTRUPPER0_2_R2PCIE_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PMONCNTRLOWER0_3_R2PCIE_PMON_REG supported on:                             */
/*       IVT_EP (0x401990B8)                                                  */
/*       IVT_EX (0x401990B8)                                                  */
/*       HSX (0x401810B8)                                                     */
/*       BDX (0x401810B8)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRLOWER0_3_R2PCIE_PMON_REG 0x070140B8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
 * This register is a perfmon counter. Software can both read it and write it.
 */
typedef union {
  struct {
    UINT32 pmonctrdata : 32;
    /* pmonctrdata - Bits[31:0], RW_V, default = 32'b00000000000000000000000000000000 
       This is the current value of the counter.
     */
  } Bits;
  UINT32 Data;
} PMONCNTRLOWER0_3_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* PMONCNTRUPPER0_3_R2PCIE_PMON_REG supported on:                             */
/*       IVT_EP (0x401990BC)                                                  */
/*       IVT_EX (0x401990BC)                                                  */
/*       HSX (0x401810BC)                                                     */
/*       BDX (0x401810BC)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRUPPER0_3_R2PCIE_PMON_REG 0x070140BC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.1.CFG.xml.
 * This register is a perfmon counter. Software can both read it and write it.
 */
typedef union {
  struct {
    UINT32 pmonctrdata : 16;
    /* pmonctrdata - Bits[15:0], RW_V, default = 16'b0000000000000000 
       This is the current value of the counter.
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PMONCNTRUPPER0_3_R2PCIE_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PMONCNTRCFG0_0_R2PCIE_PMON_REG supported on:                               */
/*       IVT_EP (0x401990D8)                                                  */
/*       IVT_EX (0x401990D8)                                                  */
/*       HSX (0x401810D8)                                                     */
/*       BDX (0x401810D8)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRCFG0_0_R2PCIE_PMON_REG 0x070140D8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
 * Perfmon Counter Control Register
 */
typedef union {
  struct {
    UINT32 evslct : 8;
    /* evslct - Bits[7:0], RW_V, default = 8'b00000000 
       This field is used to decode the PerfMon event which is selected. The encodings 
       for each of the valid UnCore PerfMon events can be found in the respective 
       Perfmon documentation. 
     */
    UINT32 unitmask : 8;
    /* unitmask - Bits[15:8], RW_V, default = 8'b00000000 
       This mask selects the sub-events to be selected for creation of the event. The 
       selected sub-events are bitwise OR-ed together to create event. At least one 
       sub-event must be selected otherwise the PerfMon event signals will not ever get 
       asserted. Events with no sub-events listed effectively have only one sub-event 
       -- bit 8 must be set to 1 in this case. 
     */
    UINT32 rsvd_16 : 1;
    /* rsvd_16 - Bits[16:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 counterreset : 1;
    /* counterreset - Bits[17:17], WO, default = 1'b0 
       When this bit is set, the corresponding counter will be reset to 0. This allows 
       for a quick reset of the counter when changing event encodings. 
     */
    UINT32 edgedet : 1;
    /* edgedet - Bits[18:18], RW_V, default = 1'b0 
       Edge detect only works in conjunction with threshholding. This is true even for 
       events that can only increment by 1 in a given cycle (like the L1 example 
       above). In this case, one should set a threshhold of 1. One can also use Edge 
       Detect with queue occupancy events. For example, if one wanted to count the 
       number of times when the TOR occupancy was larger than 5, one would selet the 
       TOR occupancy event with a threshold of 5 and set the Edge Detect bit. 
     */
    UINT32 rsvd_19 : 1;
    /* rsvd_19 - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ovfenable : 1;
    /* ovfenable - Bits[20:20], RW_V, default = 1'b0 
       Setting this bit will enable the counter to send an overflow signal. If this bit 
       is not set, the counter will wrap around when it overflows without triggering 
       anything. If this bit is set and the Unit's configuration register has Overflow 
       enabled, then a signal will be transmitted to the Ubox. 
     */
    UINT32 rsvd_21 : 1;
    UINT32 counteren : 1;
    /* counteren - Bits[22:22], RW_V, default = 1'b0 
       This field is the local enable for the PerfMon Counter. This bit must be 
       asserted in order for the PerfMon counter to begin counting the events selected 
       by the event select, unit mask, and internal bits (see the fields below). There 
       is one bit per PerfMon Counter. Note that if this bit is set to 1 but the Unit 
       Control Registers have determined that counting is disabled, then the counter 
       will not count. 
     */
    UINT32 invert : 1;
    /* invert - Bits[23:23], RW_V, default = 1'b0 
       This bit indicates how the threshold field will be compared to the incoming 
       event. When 0, the comparison that will be done is threshold >= event. When set 
       to 1, the comparison that will be done is inverted from the case where this bit 
       is set to 0, i.e., threshold < event. The invert bit only works when Threshhold 
       != 0. So, if one would like to invert a non-occupancy event (like LLC Hit), one 
       needs to set the threshhold to 1. 
     */
    UINT32 threshold : 8;
    /* threshold - Bits[31:24], RW_V, default = 8'b00000000 
       This field is compared directly against an incoming event value for events that 
       can increment by 1 or more in a given cycle. Since the widest event from the 
       UnCore is 7bits (queue occupancy), bit 31 is unused. The result of the 
       comparison is effectively a 1 bit wide event, i.e., the counter will be 
       incremented by 1 when the comparison is true (the type of comparison depends on 
       the setting of the 'invert' bit - see bit 23 below) no matter how wide the 
       original event was. When this field is zero, threshold comparison is disabled 
       and the event is passed without modification. 
     */
  } Bits;
  UINT32 Data;
} PMONCNTRCFG0_0_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* PMONCNTRCFG0_1_R2PCIE_PMON_REG supported on:                               */
/*       IVT_EP (0x401990DC)                                                  */
/*       IVT_EX (0x401990DC)                                                  */
/*       HSX (0x401810DC)                                                     */
/*       BDX (0x401810DC)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRCFG0_1_R2PCIE_PMON_REG 0x070140DC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
 */
typedef union {
  struct {
    UINT32 evslct : 8;
    /* evslct - Bits[7:0], RW_V, default = 8'b00000000 
       This field is used to decode the PerfMon event which is selected. The encodings 
       for each of the valid UnCore PerfMon events can be found in the respective 
       Perfmon documentation. 
     */
    UINT32 unitmask : 8;
    /* unitmask - Bits[15:8], RW_V, default = 8'b00000000 
       This mask selects the sub-events to be selected for creation of the event. The 
       selected sub-events are bitwise OR-ed together to create event. At least one 
       sub-event must be selected otherwise the PerfMon event signals will not ever get 
       asserted. Events with no sub-events listed effectively have only one sub-event 
       -- bit 8 must be set to 1 in this case. 
     */
    UINT32 rsvd_16 : 1;
    /* rsvd_16 - Bits[16:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 counterreset : 1;
    /* counterreset - Bits[17:17], WO, default = 1'b0 
       When this bit is set, the corresponding counter will be reset to 0. This allows 
       for a quick reset of the counter when changing event encodings. 
     */
    UINT32 edgedet : 1;
    /* edgedet - Bits[18:18], RW_V, default = 1'b0 
       Edge detect only works in conjunction with threshholding. This is true even for 
       events that can only increment by 1 in a given cycle (like the L1 example 
       above). In this case, one should set a threshhold of 1. One can also use Edge 
       Detect with queue occupancy events. For example, if one wanted to count the 
       number of times when the TOR occupancy was larger than 5, one would selet the 
       TOR occupancy event with a threshold of 5 and set the Edge Detect bit. 
     */
    UINT32 rsvd_19 : 1;
    /* rsvd_19 - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ovfenable : 1;
    /* ovfenable - Bits[20:20], RW_V, default = 1'b0 
       Setting this bit will enable the counter to send an overflow signal. If this bit 
       is not set, the counter will wrap around when it overflows without triggering 
       anything. If this bit is set and the Unit's configuration register has Overflow 
       enabled, then a signal will be transmitted to the Ubox. 
     */
    UINT32 rsvd_21 : 1;
    UINT32 counteren : 1;
    /* counteren - Bits[22:22], RW_V, default = 1'b0 
       This field is the local enable for the PerfMon Counter. This bit must be 
       asserted in order for the PerfMon counter to begin counting the events selected 
       by the event select, unit mask, and internal bits (see the fields below). There 
       is one bit per PerfMon Counter. Note that if this bit is set to 1 but the Unit 
       Control Registers have determined that counting is disabled, then the counter 
       will not count. 
     */
    UINT32 invert : 1;
    /* invert - Bits[23:23], RW_V, default = 1'b0 
       This bit indicates how the threshold field will be compared to the incoming 
       event. When 0, the comparison that will be done is threshold >= event. When set 
       to 1, the comparison that will be done is inverted from the case where this bit 
       is set to 0, i.e., threshold < event. The invert bit only works when Threshhold 
       != 0. So, if one would like to invert a non-occupancy event (like LLC Hit), one 
       needs to set the threshhold to 1. 
     */
    UINT32 threshold : 8;
    /* threshold - Bits[31:24], RW_V, default = 8'b00000000 
       This field is compared directly against an incoming event value for events that 
       can increment by 1 or more in a given cycle. Since the widest event from the 
       UnCore is 7bits (queue occupancy), bit 31 is unused. The result of the 
       comparison is effectively a 1 bit wide event, i.e., the counter will be 
       incremented by 1 when the comparison is true (the type of comparison depends on 
       the setting of the 'invert' bit - see bit 23 below) no matter how wide the 
       original event was. When this field is zero, threshold comparison is disabled 
       and the event is passed without modification. 
     */
  } Bits;
  UINT32 Data;
} PMONCNTRCFG0_1_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* PMONCNTRCFG0_2_R2PCIE_PMON_REG supported on:                               */
/*       IVT_EP (0x401990E0)                                                  */
/*       IVT_EX (0x401990E0)                                                  */
/*       HSX (0x401810E0)                                                     */
/*       BDX (0x401810E0)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRCFG0_2_R2PCIE_PMON_REG 0x070140E0
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
 */
typedef union {
  struct {
    UINT32 evslct : 8;
    /* evslct - Bits[7:0], RW_V, default = 8'b00000000 
       This field is used to decode the PerfMon event which is selected. The encodings 
       for each of the valid UnCore PerfMon events can be found in the respective 
       Perfmon documentation. 
     */
    UINT32 unitmask : 8;
    /* unitmask - Bits[15:8], RW_V, default = 8'b00000000 
       This mask selects the sub-events to be selected for creation of the event. The 
       selected sub-events are bitwise OR-ed together to create event. At least one 
       sub-event must be selected otherwise the PerfMon event signals will not ever get 
       asserted. Events with no sub-events listed effectively have only one sub-event 
       -- bit 8 must be set to 1 in this case. 
     */
    UINT32 rsvd_16 : 1;
    /* rsvd_16 - Bits[16:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 counterreset : 1;
    /* counterreset - Bits[17:17], WO, default = 1'b0 
       When this bit is set, the corresponding counter will be reset to 0. This allows 
       for a quick reset of the counter when changing event encodings. 
     */
    UINT32 edgedet : 1;
    /* edgedet - Bits[18:18], RW_V, default = 1'b0 
       Edge detect only works in conjunction with threshholding. This is true even for 
       events that can only increment by 1 in a given cycle (like the L1 example 
       above). In this case, one should set a threshhold of 1. One can also use Edge 
       Detect with queue occupancy events. For example, if one wanted to count the 
       number of times when the TOR occupancy was larger than 5, one would selet the 
       TOR occupancy event with a threshold of 5 and set the Edge Detect bit. 
     */
    UINT32 rsvd_19 : 1;
    /* rsvd_19 - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ovfenable : 1;
    /* ovfenable - Bits[20:20], RW_V, default = 1'b0 
       Setting this bit will enable the counter to send an overflow signal. If this bit 
       is not set, the counter will wrap around when it overflows without triggering 
       anything. If this bit is set and the Unit's configuration register has Overflow 
       enabled, then a signal will be transmitted to the Ubox. 
     */
    UINT32 rsvd_21 : 1;
    UINT32 counteren : 1;
    /* counteren - Bits[22:22], RW_V, default = 1'b0 
       This field is the local enable for the PerfMon Counter. This bit must be 
       asserted in order for the PerfMon counter to begin counting the events selected 
       by the event select, unit mask, and internal bits (see the fields below). There 
       is one bit per PerfMon Counter. Note that if this bit is set to 1 but the Unit 
       Control Registers have determined that counting is disabled, then the counter 
       will not count. 
     */
    UINT32 invert : 1;
    /* invert - Bits[23:23], RW_V, default = 1'b0 
       This bit indicates how the threshold field will be compared to the incoming 
       event. When 0, the comparison that will be done is threshold >= event. When set 
       to 1, the comparison that will be done is inverted from the case where this bit 
       is set to 0, i.e., threshold < event. The invert bit only works when Threshhold 
       != 0. So, if one would like to invert a non-occupancy event (like LLC Hit), one 
       needs to set the threshhold to 1. 
     */
    UINT32 threshold : 8;
    /* threshold - Bits[31:24], RW_V, default = 8'b00000000 
       This field is compared directly against an incoming event value for events that 
       can increment by 1 or more in a given cycle. Since the widest event from the 
       UnCore is 7bits (queue occupancy), bit 31 is unused. The result of the 
       comparison is effectively a 1 bit wide event, i.e., the counter will be 
       incremented by 1 when the comparison is true (the type of comparison depends on 
       the setting of the 'invert' bit - see bit 23 below) no matter how wide the 
       original event was. When this field is zero, threshold comparison is disabled 
       and the event is passed without modification. 
     */
  } Bits;
  UINT32 Data;
} PMONCNTRCFG0_2_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* PMONCNTRCFG0_3_R2PCIE_PMON_REG supported on:                               */
/*       IVT_EP (0x401990E4)                                                  */
/*       IVT_EX (0x401990E4)                                                  */
/*       HSX (0x401810E4)                                                     */
/*       BDX (0x401810E4)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCNTRCFG0_3_R2PCIE_PMON_REG 0x070140E4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
 */
typedef union {
  struct {
    UINT32 evslct : 8;
    /* evslct - Bits[7:0], RW_V, default = 8'b00000000 
       This field is used to decode the PerfMon event which is selected. The encodings 
       for each of the valid UnCore PerfMon events can be found in the respective 
       Perfmon documentation. 
     */
    UINT32 unitmask : 8;
    /* unitmask - Bits[15:8], RW_V, default = 8'b00000000 
       This mask selects the sub-events to be selected for creation of the event. The 
       selected sub-events are bitwise OR-ed together to create event. At least one 
       sub-event must be selected otherwise the PerfMon event signals will not ever get 
       asserted. Events with no sub-events listed effectively have only one sub-event 
       -- bit 8 must be set to 1 in this case. 
     */
    UINT32 rsvd_16 : 1;
    /* rsvd_16 - Bits[16:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 counterreset : 1;
    /* counterreset - Bits[17:17], WO, default = 1'b0 
       When this bit is set, the corresponding counter will be reset to 0. This allows 
       for a quick reset of the counter when changing event encodings. 
     */
    UINT32 edgedet : 1;
    /* edgedet - Bits[18:18], RW_V, default = 1'b0 
       Edge detect only works in conjunction with threshholding. This is true even for 
       events that can only increment by 1 in a given cycle (like the L1 example 
       above). In this case, one should set a threshhold of 1. One can also use Edge 
       Detect with queue occupancy events. For example, if one wanted to count the 
       number of times when the TOR occupancy was larger than 5, one would selet the 
       TOR occupancy event with a threshold of 5 and set the Edge Detect bit. 
     */
    UINT32 rsvd_19 : 1;
    /* rsvd_19 - Bits[19:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ovfenable : 1;
    /* ovfenable - Bits[20:20], RW_V, default = 1'b0 
       Setting this bit will enable the counter to send an overflow signal. If this bit 
       is not set, the counter will wrap around when it overflows without triggering 
       anything. If this bit is set and the Unit's configuration register has Overflow 
       enabled, then a signal will be transmitted to the Ubox. 
     */
    UINT32 rsvd_21 : 1;
    UINT32 counteren : 1;
    /* counteren - Bits[22:22], RW_V, default = 1'b0 
       This field is the local enable for the PerfMon Counter. This bit must be 
       asserted in order for the PerfMon counter to begin counting the events selected 
       by the event select, unit mask, and internal bits (see the fields below). There 
       is one bit per PerfMon Counter. Note that if this bit is set to 1 but the Unit 
       Control Registers have determined that counting is disabled, then the counter 
       will not count. 
     */
    UINT32 invert : 1;
    /* invert - Bits[23:23], RW_V, default = 1'b0 
       This bit indicates how the threshold field will be compared to the incoming 
       event. When 0, the comparison that will be done is threshold >= event. When set 
       to 1, the comparison that will be done is inverted from the case where this bit 
       is set to 0, i.e., threshold < event. The invert bit only works when Threshhold 
       != 0. So, if one would like to invert a non-occupancy event (like LLC Hit), one 
       needs to set the threshhold to 1. 
     */
    UINT32 threshold : 8;
    /* threshold - Bits[31:24], RW_V, default = 8'b00000000 
       This field is compared directly against an incoming event value for events that 
       can increment by 1 or more in a given cycle. Since the widest event from the 
       UnCore is 7bits (queue occupancy), bit 31 is unused. The result of the 
       comparison is effectively a 1 bit wide event, i.e., the counter will be 
       incremented by 1 when the comparison is true (the type of comparison depends on 
       the setting of the 'invert' bit - see bit 23 below) no matter how wide the 
       original event was. When this field is zero, threshold comparison is disabled 
       and the event is passed without modification. 
     */
  } Bits;
  UINT32 Data;
} PMONCNTRCFG0_3_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* PMONUNITCTRL0_R2PCIE_PMON_REG supported on:                                */
/*       IVT_EP (0x401990F4)                                                  */
/*       IVT_EX (0x401990F4)                                                  */
/*       HSX (0x401810F4)                                                     */
/*       BDX (0x401810F4)                                                     */
/* Register default value on IVT_EP:    0x00000000                            */
/* Register default value on IVT_EX:    0x00000000                            */
/* Register default value on HSX:       0x00030000                            */
/* Register default value on BDX:       0x00030000                            */
#define PMONUNITCTRL0_R2PCIE_PMON_REG 0x070140F4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.1.CFG.xml.
 * Unit Control
 */
typedef union {
  struct {
    UINT32 resetcounterconfigs : 1;
    /* resetcounterconfigs - Bits[0:0], WO, default = 1'b0 
       When this bit is written to, the counter configuration registers will be reset. 
       This does not effect the values in the counters. 
     */
    UINT32 resetcounters : 1;
    /* resetcounters - Bits[1:1], WO, default = 1'b0 
       When this bit is written to, the counters data fields will be reset. The 
       configuration values will not be reset. 
     */
    UINT32 rsvd_2 : 6;
    /* rsvd_2 - Bits[7:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 freezecounters : 1;
    /* freezecounters - Bits[8:8], RW_V, default = 1'b0 
       When this bit is written to, the counters data fields will be reset. The 
       configuration values will not be reset. 
     */
    UINT32 rsvd_9 : 7;
    /* rsvd_9 - Bits[15:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 freezeenable : 1;
    /* freezeenable - Bits[16:16], RW, default = 1'b1 
       This bit controls what the counters in the unit will do when they receive a 
       freeze signal. When set, the counters will be allowed to freeze. When not set, 
       the counters will ignore the freeze signal. 
     */
    UINT32 overflowenable : 1;
    /* overflowenable - Bits[17:17], RW, default = 1'b1 
       This bit controls the behavior of counters when they overflow. When set, the 
       system will trigger the overflow handling process throughout the rest of the 
       uncore, potentially triggering a PMI and freezing counters. When it is not set, 
       the counters will simply wrap around and continue to count. 
     */
    UINT32 rsvd_18 : 14;
    /* rsvd_18 - Bits[31:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PMONUNITCTRL0_R2PCIE_PMON_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PMONCTRSTATUS0_R2PCIE_PMON_REG supported on:                               */
/*       IVT_EP (0x401990F8)                                                  */
/*       IVT_EX (0x401990F8)                                                  */
/*       HSX (0x401810F8)                                                     */
/*       BDX (0x401810F8)                                                     */
/* Register default value:              0x00000000                            */
#define PMONCTRSTATUS0_R2PCIE_PMON_REG 0x070140F8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.1.CFG.xml.
 * Pmon Counter Status
 */
typedef union {
  struct {
    UINT32 counter0ovf : 1;
    /* counter0ovf - Bits[0:0], RW1C, default = 1'b0 
       counter 0 overflowed
     */
    UINT32 counter1ovf : 1;
    /* counter1ovf - Bits[1:1], RW1C, default = 1'b0 
       counter 1 overflowed
     */
    UINT32 counter2ovf : 1;
    /* counter2ovf - Bits[2:2], RW1C, default = 1'b0 
       counter 2 overflowed
     */
    UINT32 counter3ovf : 1;
    /* counter3ovf - Bits[3:3], RW1C, default = 1'b0 
       counter 3 overflowed
     */
    UINT32 rsvd : 28;
    /* rsvd - Bits[31:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PMONCTRSTATUS0_R2PCIE_PMON_STRUCT;
#endif /* ASM_INC */


/* SDID_R2PCIE_PMON_REG supported on:                                         */
/*       IVT_EP (0x2019902E)                                                  */
/*       IVT_EX (0x2019902E)                                                  */
/* Register default value:              0x0000                                */
#define SDID_R2PCIE_PMON_REG 0x0701202E



#endif /* R2PCIE_PMON_h */
