/* Date Stamp: 8/23/2014 */

#ifndef R2PCIE_CFG_h
#define R2PCIE_CFG_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* R2PCIE_CFG_IVT_DEV 19                                                      */
/* R2PCIE_CFG_IVT_FUN 0                                                       */
/* For HSX_HOST:                                                              */
/* R2PCIE_CFG_HSX_DEV 16                                                      */
/* R2PCIE_CFG_HSX_FUN 0                                                       */
/* For BDX_HOST:                                                              */
/* R2PCIE_CFG_BDX_DEV 16                                                      */
/* R2PCIE_CFG_BDX_FUN 0                                                       */

/* VID_R2PCIE_CFG_REG supported on:                                           */
/*       IVT_EP (0x20198000)                                                  */
/*       IVT_EX (0x20198000)                                                  */
/*       HSX (0x20180000)                                                     */
/*       BDX (0x20180000)                                                     */
/* Register default value:              0x8086                                */
#define VID_R2PCIE_CFG_REG 0x07002000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
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
} VID_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */


/* DID_R2PCIE_CFG_REG supported on:                                           */
/*       IVT_EP (0x20198002)                                                  */
/*       IVT_EX (0x20198002)                                                  */
/*       HSX (0x20180002)                                                     */
/*       BDX (0x20180002)                                                     */
/* Register default value on IVT_EP:    0x0E1D                                */
/* Register default value on IVT_EX:    0x0E1D                                */
/* Register default value on HSX:       0x2F1D                                */
/* Register default value on BDX:       0x6F1D                                */
#define DID_R2PCIE_CFG_REG 0x07002002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6F1D 
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
} DID_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */


/* PCICMD_R2PCIE_CFG_REG supported on:                                        */
/*       IVT_EP (0x20198004)                                                  */
/*       IVT_EX (0x20198004)                                                  */
/*       HSX (0x20180004)                                                     */
/*       BDX (0x20180004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_R2PCIE_CFG_REG 0x07002004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
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
} PCICMD_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */


/* PCISTS_R2PCIE_CFG_REG supported on:                                        */
/*       IVT_EP (0x20198006)                                                  */
/*       IVT_EX (0x20198006)                                                  */
/*       HSX (0x20180006)                                                     */
/*       BDX (0x20180006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_R2PCIE_CFG_REG 0x07002006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.0.CFG.xml.
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
} PCISTS_R2PCIE_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_R2PCIE_CFG_REG supported on:                                           */
/*       IVT_EP (0x10198008)                                                  */
/*       IVT_EX (0x10198008)                                                  */
/*       HSX (0x10180008)                                                     */
/*       BDX (0x10180008)                                                     */
/* Register default value:              0x00                                  */
#define RID_R2PCIE_CFG_REG 0x07001008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
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
} RID_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_R2PCIE_CFG_REG supported on:                                        */
/*       IVT_EP (0x10198009)                                                  */
/*       IVT_EX (0x10198009)                                                  */
/*       HSX (0x10180009)                                                     */
/*       BDX (0x10180009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_R2PCIE_CFG_REG 0x07001009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.0.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_R2PCIE_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_R2PCIE_CFG_REG supported on:                                        */
/*       IVT_EP (0x2019800A)                                                  */
/*       IVT_EX (0x2019800A)                                                  */
/*       HSX (0x2018000A)                                                     */
/*       BDX (0x2018000A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_R2PCIE_CFG_REG 0x0700200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
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
} CCR_N1_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */


/* CLSR_R2PCIE_CFG_REG supported on:                                          */
/*       IVT_EP (0x1019800C)                                                  */
/*       IVT_EX (0x1019800C)                                                  */
/*       HSX (0x1018000C)                                                     */
/*       BDX (0x1018000C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_R2PCIE_CFG_REG 0x0700100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
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
} CLSR_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */


/* PLAT_R2PCIE_CFG_REG supported on:                                          */
/*       IVT_EP (0x1019800D)                                                  */
/*       IVT_EX (0x1019800D)                                                  */
/*       HSX (0x1018000D)                                                     */
/*       BDX (0x1018000D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_R2PCIE_CFG_REG 0x0700100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
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
} PLAT_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */


/* HDR_R2PCIE_CFG_REG supported on:                                           */
/*       IVT_EP (0x1019800E)                                                  */
/*       IVT_EX (0x1019800E)                                                  */
/*       HSX (0x1018000E)                                                     */
/*       BDX (0x1018000E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_R2PCIE_CFG_REG 0x0700100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
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
} HDR_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */


/* BIST_R2PCIE_CFG_REG supported on:                                          */
/*       IVT_EP (0x1019800F)                                                  */
/*       IVT_EX (0x1019800F)                                                  */
/*       HSX (0x1018000F)                                                     */
/*       BDX (0x1018000F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_R2PCIE_CFG_REG 0x0700100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
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
} BIST_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */


/* SVID_R2PCIE_CFG_REG supported on:                                          */
/*       IVT_EP (0x2019802C)                                                  */
/*       IVT_EX (0x2019802C)                                                  */
/*       HSX (0x4018002C)                                                     */
/*       BDX (0x4018002C)                                                     */
/* Register default value on IVT_EP:    0x8086                                */
/* Register default value on IVT_EX:    0x8086                                */
/* Register default value on HSX:       0x00008086                            */
/* Register default value on BDX:       0x00008086                            */
#define SVID_R2PCIE_CFG_REG 0x0700C000


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 subsystem_vendor_identification_number : 16;
    /* subsystem_vendor_identification_number - Bits[15:0], RW_O, default = 16'b1000000010000110  */
    UINT32 subsystem_device_identification_number : 16;
    /* subsystem_device_identification_number - Bits[31:16], RW_O, default = 16'b0000000000000000  */
  } Bits;
  UINT32 Data;
} SVID_R2PCIE_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CAPPTR_R2PCIE_CFG_REG supported on:                                        */
/*       IVT_EP (0x10198034)                                                  */
/*       IVT_EX (0x10198034)                                                  */
/*       HSX (0x10180034)                                                     */
/*       BDX (0x10180034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_R2PCIE_CFG_REG 0x07001034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
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
} CAPPTR_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */


/* INTL_R2PCIE_CFG_REG supported on:                                          */
/*       IVT_EP (0x1019803C)                                                  */
/*       IVT_EX (0x1019803C)                                                  */
/*       HSX (0x1018003C)                                                     */
/*       BDX (0x1018003C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_R2PCIE_CFG_REG 0x0700103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
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
} INTL_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */


/* INTPIN_R2PCIE_CFG_REG supported on:                                        */
/*       IVT_EP (0x1019803D)                                                  */
/*       IVT_EX (0x1019803D)                                                  */
/*       HSX (0x1018003D)                                                     */
/*       BDX (0x1018003D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_R2PCIE_CFG_REG 0x0700103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
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
} INTPIN_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */


/* MINGNT_R2PCIE_CFG_REG supported on:                                        */
/*       IVT_EP (0x1019803E)                                                  */
/*       IVT_EX (0x1019803E)                                                  */
/*       HSX (0x1018003E)                                                     */
/*       BDX (0x1018003E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_R2PCIE_CFG_REG 0x0700103E


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.0.CFG.xml.
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
} MINGNT_R2PCIE_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* MAXLAT_R2PCIE_CFG_REG supported on:                                        */
/*       IVT_EP (0x1019803F)                                                  */
/*       IVT_EX (0x1019803F)                                                  */
/*       HSX (0x1018003F)                                                     */
/*       BDX (0x1018003F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_R2PCIE_CFG_REG 0x0700103F


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.0.CFG.xml.
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
} MAXLAT_R2PCIE_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPI_RT_R2PCIE_CFG_REG supported on:                                        */
/*       IVT_EP (0x20198040)                                                  */
/*       IVT_EX (0x20198040)                                                  */
/*       HSX (0x20180040)                                                     */
/*       BDX (0x20180040)                                                     */
/* Register default value:              0x0000                                */
#define QPI_RT_R2PCIE_CFG_REG 0x07002040


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.0.CFG.xml.
 * Intel QPI Routing Table
 */
typedef union {
  struct {
    UINT16 qpiroutingtable : 16;
    /* qpiroutingtable - Bits[15:0], RW_LB, default = 16'b0000000000000000 
       Bit 0 corresponds to NodeID0 
                          Bit 1 corresponds to NodeID1 
                          ... 
                          Value of 0 means QPI0 
                          Value of 1 means QPI1 
             
     */
  } Bits;
  UINT16 Data;
} QPI_RT_R2PCIE_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* IIO_BW_COUNTER_R2PCIE_CFG_REG supported on:                                */
/*       IVT_EP (0x40198044)                                                  */
/*       IVT_EX (0x40198044)                                                  */
/*       HSX (0x40180044)                                                     */
/*       BDX (0x40180044)                                                     */
/* Register default value:              0x00000000                            */
#define IIO_BW_COUNTER_R2PCIE_CFG_REG 0x07004044
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
 * IIO Bandwidth Counter
 */
typedef union {
  struct {
    UINT32 iio_bandwidth_counter : 30;
    /* iio_bandwidth_counter - Bits[29:0], RW1C, default = 30'b000000000000000000000000000000 
       Free running counter that increments on each AD request sent to the ring.
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} IIO_BW_COUNTER_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */


/* R2PGNCTRL_R2PCIE_CFG_REG supported on:                                     */
/*       IVT_EP (0x40198048)                                                  */
/*       IVT_EX (0x40198048)                                                  */
/*       HSX (0x40180048)                                                     */
/*       BDX (0x40180048)                                                     */
/* Register default value on IVT_EP:    0x00000000                            */
/* Register default value on IVT_EX:    0x00000000                            */
/* Register default value on HSX:       0x00000004                            */
/* Register default value on BDX:       0x00000004                            */
#define R2PGNCTRL_R2PCIE_CFG_REG 0x07004048


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.0.CFG.xml.
 * R2PCIe General Control Register
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    /* rsvd_0 - Bits[0:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 qpi_rtselectforndrdrs : 1;
    /* qpi_rtselectforndrdrs - Bits[1:1], RW_L, default = 1'b0 
       When this bit is set, R2PCIe routes NDR and DRS packets destined to remote 
       sockets, using the QPI_RT. When this bit is clear, R2PCIe routes NDR and DRS 
       packets destined to remote sockets, using the Intel QPI Link_ID field sent by 
       IIO/Ubox along with these packet. 
     */
    UINT32 en_blvnacreditreturnperffix : 1;
    /* en_blvnacreditreturnperffix - Bits[2:2], RWS, default = 1'b1 
       Enables the return of multiple VNA credits. 
     */
    UINT32 en_2akcreditreturnperffix : 1;
    /* en_2akcreditreturnperffix - Bits[3:3], RWS, default = 1'b0 
       Enables the return of credits on AK egress using 2 AK egress entries, instead of 
       1 . 
     */
    UINT32 rsvd_4 : 28;
    /* rsvd_4 - Bits[31:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R2PGNCTRL_R2PCIE_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* R2PINGERRLOG0_R2PCIE_CFG_REG supported on:                                 */
/*       IVT_EP (0x4019804C)                                                  */
/*       IVT_EX (0x4019804C)                                                  */
/*       HSX (0x4018004C)                                                     */
/*       BDX (0x4018004C)                                                     */
/* Register default value:              0x00000000                            */
#define R2PINGERRLOG0_R2PCIE_CFG_REG 0x0700404C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.0.CFG.xml.
 * R2PCIe Ingress error log 0
 */
typedef union {
  struct {
    UINT32 ak0bgfcrdoverflow : 1;
    /* ak0bgfcrdoverflow - Bits[0:0], RW1CS, default = 1'b0 
       AK BGF 0 BGF credit overflow
     */
    UINT32 ak1bgfcrdoverflow : 1;
    /* ak1bgfcrdoverflow - Bits[1:1], RW1CS, default = 1'b0 
       AK BGF 1 BGF credit overflow
     */
    UINT32 blbgfcrdoverflow : 1;
    /* blbgfcrdoverflow - Bits[2:2], RW1CS, default = 1'b0 
       BL BGF credit overflow
     */
    UINT32 ubxcboncbcrdoverflow : 1;
    /* ubxcboncbcrdoverflow - Bits[3:3], RW1CS, default = 1'b0 
       Ubox Cbo NCB credit overflow
     */
    UINT32 ubxcboncscrdoverflow : 1;
    /* ubxcboncscrdoverflow - Bits[4:4], RW1CS, default = 1'b0 
       Ubox Cbo NCS credit overflow
     */
    UINT32 ubxqpincbcrdoverflow : 1;
    /* ubxqpincbcrdoverflow - Bits[5:5], RW1CS, default = 1'b0 
       Ubox QPI NCB credit overflow
     */
    UINT32 ubxqpincscrdoverflow : 1;
    /* ubxqpincscrdoverflow - Bits[6:6], RW1CS, default = 1'b0 
       Ubox QPI NCS credit overflow
     */
    UINT32 iioidicrdoverflow : 1;
    /* iioidicrdoverflow - Bits[7:7], RW1CS, default = 1'b0 
       IIO IDI credit overflow
     */
    UINT32 iioncbcrdoverflow : 1;
    /* iioncbcrdoverflow - Bits[8:8], RW1CS, default = 1'b0 
       IIO NCB credit overflow
     */
    UINT32 iioncscrdoverflow : 1;
    /* iioncscrdoverflow - Bits[9:9], RW1CS, default = 1'b0 
       IIO NCS credit overflow
     */
    UINT32 parerring0 : 1;
    /* parerring0 - Bits[10:10], RW1CS, default = 1'b0 
       Ingress BL data parity error - Agent 0
     */
    UINT32 parerring1 : 1;
    /* parerring1 - Bits[11:11], RW1CS, default = 1'b0 
       Ingress BL data parity error - Agent 1
     */
    UINT32 parerre2e0 : 1;
    /* parerre2e0 - Bits[12:12], RW1CS, default = 1'b0 
       Ingress End2End data parity error - Agent 0
     */
    UINT32 parerre2e1 : 1;
    /* parerre2e1 - Bits[13:13], RW1CS, default = 1'b0 
       Ingress E2E data parity error - Agent 1
     */
    UINT32 ivbgfcrdoverflow : 1;
    /* ivbgfcrdoverflow - Bits[14:14], RW1CS, default = 1'b0 
       IV BGF credit overflow
     */
    UINT32 ivsnpcrdoverflow : 1;
    /* ivsnpcrdoverflow - Bits[15:15], RW1CS, default = 1'b0 
       IV Snoop credit overflow
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R2PINGERRLOG0_R2PCIE_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* R2PINGERRMSK0_R2PCIE_CFG_REG supported on:                                 */
/*       IVT_EP (0x40198050)                                                  */
/*       IVT_EX (0x40198050)                                                  */
/*       HSX (0x40180050)                                                     */
/*       BDX (0x40180050)                                                     */
/* Register default value on IVT_EP:    0x00000000                            */
/* Register default value on IVT_EX:    0x00000000                            */
/* Register default value on HSX:       0x00003000                            */
/* Register default value on BDX:       0x00003000                            */
#define R2PINGERRMSK0_R2PCIE_CFG_REG 0x07004050


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.0.CFG.xml.
 * R2PCIe Ingress error mask register 0
 */
typedef union {
  struct {
    UINT32 ak0bgfcrderrmsk : 1;
    /* ak0bgfcrderrmsk - Bits[0:0], RWS, default = 1'b0 
       Mask AK 0 BGF credit overflow error
     */
    UINT32 ak1bgfcrderrmsk : 1;
    /* ak1bgfcrderrmsk - Bits[1:1], RWS, default = 1'b0 
       Mask AK 1 BGF credit overflow error
     */
    UINT32 blbgfcrderrmsk : 1;
    /* blbgfcrderrmsk - Bits[2:2], RWS, default = 1'b0 
       Mask BL BGF credit overflow error
     */
    UINT32 ubxcboncbcrderrmsk : 1;
    /* ubxcboncbcrderrmsk - Bits[3:3], RWS, default = 1'b0 
       Mask Ubox Cbo NCB credit overflow error
     */
    UINT32 ubxcboncscrderrmsk : 1;
    /* ubxcboncscrderrmsk - Bits[4:4], RWS, default = 1'b0 
       Mask Ubox Cbo NCS credit overflow error
     */
    UINT32 ubxqpincbcrderrmsk : 1;
    /* ubxqpincbcrderrmsk - Bits[5:5], RWS, default = 1'b0 
       Mask Ubox QPI NCB credit overflow error
     */
    UINT32 ubxqpincscrderrmsk : 1;
    /* ubxqpincscrderrmsk - Bits[6:6], RWS, default = 1'b0 
       Mask Ubox QPI NCS credit overflow error
     */
    UINT32 iioidicrderrmsk : 1;
    /* iioidicrderrmsk - Bits[7:7], RWS, default = 1'b0 
       Mask IIO IDI credit overflow error
     */
    UINT32 iioncbcrderrmsk : 1;
    /* iioncbcrderrmsk - Bits[8:8], RWS, default = 1'b0 
       Mask IIO NCB credit overflow error
     */
    UINT32 iioncscrderrmsk : 1;
    /* iioncscrderrmsk - Bits[9:9], RWS, default = 1'b0 
       Mask IIO NCS credit overflow error
     */
    UINT32 parerring0msk : 1;
    /* parerring0msk - Bits[10:10], RWS, default = 1'b0 
       Mask BL ingress data parity error - Agent 0
     */
    UINT32 parerring1msk : 1;
    /* parerring1msk - Bits[11:11], RWS, default = 1'b0 
       Mask BL ingress data parity error - Agent 1
     */
    UINT32 parerre2e0msk : 1;
    /* parerre2e0msk - Bits[12:12], RWS, default = 1'b1 
       Mask ingress End2End parity error - Agent 0
     */
    UINT32 parerre2e1msk : 1;
    /* parerre2e1msk - Bits[13:13], RWS, default = 1'b1 
       Mask ingress E2E data parity error - Agent 1
     */
    UINT32 ivbgfcrderrmsk : 1;
    /* ivbgfcrderrmsk - Bits[14:14], RWS, default = 1'b0 
       IV BGF credit overflow error
     */
    UINT32 ivsnpcrderrmsk : 1;
    /* ivsnpcrderrmsk - Bits[15:15], RWS, default = 1'b0 
       IV Snoop credit overflow error
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R2PINGERRMSK0_R2PCIE_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* R2PINGDBG_R2PCIE_CFG_REG supported on:                                     */
/*       IVT_EP (0x40198054)                                                  */
/*       IVT_EX (0x40198054)                                                  */
/*       HSX (0x40180054)                                                     */
/*       BDX (0x40180054)                                                     */
/* Register default value:              0x00000000                            */
#define R2PINGDBG_R2PCIE_CFG_REG 0x07004054


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.0.CFG.xml.
 * R2PCIe Ingress Debug
 */
typedef union {
  struct {
    UINT32 dbgbusen0 : 1;
    /* dbgbusen0 - Bits[0:0], RW_L, default = 1'b0  */
    UINT32 rsvd_1 : 1;
    /* rsvd_1 - Bits[1:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rspfuncsel0 : 1;
    /* rspfuncsel0 - Bits[2:2], RW_L, default = 1'b0  */
    UINT32 rsvd_3 : 2;
    /* rsvd_3 - Bits[4:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rspfuncthrotuboxncb : 1;
    /* rspfuncthrotuboxncb - Bits[5:5], RW_L, default = 1'b0  */
    UINT32 rsvd_6 : 1;
    /* rsvd_6 - Bits[6:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rspfuncthrotiiodrs : 1;
    /* rspfuncthrotiiodrs - Bits[7:7], RW_L, default = 1'b0  */
    UINT32 rspfuncthrotiioncbncs : 1;
    /* rspfuncthrotiioncbncs - Bits[8:8], RW_L, default = 1'b0  */
    UINT32 rsvd_9 : 2;
    /* rsvd_9 - Bits[10:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rspfuncthrotbrdcboncb : 1;
    /* rspfuncthrotbrdcboncb - Bits[11:11], RW_L, default = 1'b0  */
    UINT32 rspfuncthrotbrdcboncs : 1;
    /* rspfuncthrotbrdcboncs - Bits[12:12], RW_L, default = 1'b0  */
    UINT32 rspfuncthrotbrdqpincb : 1;
    /* rspfuncthrotbrdqpincb - Bits[13:13], RW_L, default = 1'b0  */
    UINT32 rspfuncthrotbrdqpincs : 1;
    /* rspfuncthrotbrdqpincs - Bits[14:14], RW_L, default = 1'b0  */
    UINT32 dbgbuspresel0 : 12;
    /* dbgbuspresel0 - Bits[26:15], RW_L, default = 12'b0000000000  */
    UINT32 rsvd_27 : 5;
    /* rsvd_27 - Bits[31:27], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R2PINGDBG_R2PCIE_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */



/* R2PEGRDBG_R2PCIE_CFG_REG supported on:                                     */
/*       IVT_EP (0x40198058)                                                  */
/*       IVT_EX (0x40198058)                                                  */
/*       HSX (0x40180058)                                                     */
/*       BDX (0x40180058)                                                     */
/* Register default value:              0x00000000                            */
#define R2PEGRDBG_R2PCIE_CFG_REG 0x07004058


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.0.CFG.xml.
 * TBD
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 4;
    /* rsvd_0 - Bits[3:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 dbgbusen : 1;
    /* dbgbusen - Bits[4:4], RW_L, default = 1'b0  */
    UINT32 rspfuncsel : 1;
    /* rspfuncsel - Bits[5:5], RW_L, default = 1'b0  */
    UINT32 rspfuncthrotad : 1;
    /* rspfuncthrotad - Bits[6:6], RW_L, default = 1'b0  */
    UINT32 rspfuncthrotbl : 1;
    /* rspfuncthrotbl - Bits[7:7], RW_L, default = 1'b0  */
    UINT32 rspfuncthrotak : 1;
    /* rspfuncthrotak - Bits[8:8], RW_L, default = 1'b0  */
    UINT32 rspfuncthrotcrd : 1;
    /* rspfuncthrotcrd - Bits[9:9], RW_L, default = 1'b0  */
    UINT32 rspfuncthrotiv : 1;
    /* rspfuncthrotiv - Bits[10:10], RW_L, default = 1'b0  */
    UINT32 rspfuncthrotdn : 1;
    /* rspfuncthrotdn - Bits[11:11], RW_L, default = 1'b0  */
    UINT32 rspfuncthrotup : 1;
    /* rspfuncthrotup - Bits[12:12], RW_L, default = 1'b0  */
    UINT32 rsvd_13 : 3;
    /* rsvd_13 - Bits[15:13], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 dbgbuspresel : 5;
    /* dbgbuspresel - Bits[20:16], RW_L, default = 5'b00000  */
    UINT32 rsvd_21 : 11;
    /* rsvd_21 - Bits[31:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R2PEGRDBG_R2PCIE_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* R2PDEBUG_R2PCIE_CFG_REG supported on:                                      */
/*       IVT_EP (0x4019805C)                                                  */
/*       IVT_EX (0x4019805C)                                                  */
/*       HSX (0x4018005C)                                                     */
/*       BDX (0x4018005C)                                                     */
/* Register default value:              0x00000000                            */
#define R2PDEBUG_R2PCIE_CFG_REG 0x0700405C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.0.CFG.xml.
 * R2PCIe Debug
 */
typedef union {
  struct {
    UINT32 psmiwipepwrdnovr : 1;
    /* psmiwipepwrdnovr - Bits[0:0], RW, default = 1'b0 
       Setting this bit will make all the gated clock to be free running during PSMI 
       Wipe.  
     */
    UINT32 rsvd : 31;
    /* rsvd - Bits[31:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R2PDEBUG_R2PCIE_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */








/* R2GLERRCFG_R2PCIE_CFG_REG supported on:                                    */
/*       HSX (0x401800A8)                                                     */
/*       BDX (0x401800A8)                                                     */
/* Register default value:              0x000066E0                            */
#define R2GLERRCFG_R2PCIE_CFG_REG 0x070040A8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
 * R2PCIe global viral/fatal error configuration
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 resetglobalfatalerror : 1;
    /* resetglobalfatalerror - Bits[3:3], RW, default = 1'b0 
       Set 1 to force clear global fatal error status.
     */
    UINT32 resetglobalviral : 1;
    /* resetglobalviral - Bits[4:4], RW, default = 1'b0 
       Set 1 to force clear global viral status.
     */
    UINT32 maskuboxviralin : 1;
    /* maskuboxviralin - Bits[5:5], RWS, default = 1'b1 
       Set 1 to block viral status of Ubox
     */
    UINT32 maskuboxfatalerrorin : 1;
    /* maskuboxfatalerrorin - Bits[6:6], RWS, default = 1'b1 
       Set 1 to block fatal error status of Ubox to global fatal status
     */
    UINT32 maskiiofatalerrorin : 1;
    /* maskiiofatalerrorin - Bits[7:7], RWS, default = 1'b1 
       Set 1 to block fatal error status from IIO to global fatal status
     */
    UINT32 rsvd_8 : 1;
    /* rsvd_8 - Bits[8:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 maskiioviralout : 1;
    /* maskiioviralout - Bits[9:9], RWS, default = 1'b1 
       Set 1 to block Vrial status from global viral status to IIO
     */
    UINT32 maskiioviralin : 1;
    /* maskiioviralin - Bits[10:10], RWS, default = 1'b1 
       Set 1 to block Vrial status from IIO to global viral status
     */
    UINT32 rsvd_11 : 2;
    /* rsvd_11 - Bits[12:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 maskr2fatalerror : 2;
    /* maskr2fatalerror - Bits[14:13], RWS, default = 3'b11 
       Set 1 to block Set 1 to block from R2PCIE Error to global fatal error status
     */
    UINT32 localerrorstatus : 1;
    /* localerrorstatus - Bits[15:15], RO, default = 1'b0 
       Read only R2PCIe error status
     */
    UINT32 viralstatustoiio : 1;
    /* viralstatustoiio - Bits[16:16], RO, default = 1'b0 
       Read only to IIO viral status
     */
    UINT32 viralstatusfromiio : 1;
    /* viralstatusfromiio - Bits[17:17], RO, default = 1'b0 
       Read only from IIO viral status
     */
    UINT32 fatalstatusfromiio : 1;
    /* fatalstatusfromiio - Bits[18:18], RO, default = 1'b0 
       Read only from IIO fatal status
     */
    UINT32 viralstatusfromubox : 1;
    /* viralstatusfromubox - Bits[19:19], RO, default = 1'b0 
       Read only from Ubox viral status
     */
    UINT32 fatalstatusfromubox : 1;
    /* fatalstatusfromubox - Bits[20:20], RO, default = 1'b0 
       Read only from Ubox fatal status
     */
    UINT32 rsvd_21 : 11;
    /* rsvd_21 - Bits[31:21], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R2GLERRCFG_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* R2EGRERRLOG3_R2PCIE_CFG_REG supported on:                                  */
/*       BDX (0x401800AC)                                                     */
/* Register default value:              0x00000000                            */
#define R2EGRERRLOG3_R2PCIE_CFG_REG 0x070040AC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
 * R2PCIe Egress Error Log 3
 */
typedef union {
  struct {
    UINT32 cbo18prqcrdoverflow : 1;
    /* cbo18prqcrdoverflow - Bits[0:0], RW1CS, default = 1'b0 
       Cbo 18 prq credit overflow
     */
    UINT32 cbo19prqcrdoverflow : 1;
    /* cbo19prqcrdoverflow - Bits[1:1], RW1CS, default = 1'b0 
       Cbo 19 prq credit overflow
     */
    UINT32 cbo20prqcrdoverflow : 1;
    /* cbo20prqcrdoverflow - Bits[2:2], RW1CS, default = 1'b0 
       Cbo 20 prq credit overflow
     */
    UINT32 cbo21prqcrdoverflow : 1;
    /* cbo21prqcrdoverflow - Bits[3:3], RW1CS, default = 1'b0 
       Cbo 21 prq credit overflow
     */
    UINT32 cbo22prqcrdoverflow : 1;
    /* cbo22prqcrdoverflow - Bits[4:4], RW1CS, default = 1'b0 
       Cbo 22 prq credit overflow
     */
    UINT32 cbo23prqcrdoverflow : 1;
    /* cbo23prqcrdoverflow - Bits[5:5], RW1CS, default = 1'b0 
       Cbo 23 prq credit overflow
     */
    UINT32 cbo18prqcrdoverflowmask : 1;
    /* cbo18prqcrdoverflowmask - Bits[6:6], RWS, default = 1'b0 
       Cbo 18 prq credit overflow mask
     */
    UINT32 cbo19prqcrdoverflowmask : 1;
    /* cbo19prqcrdoverflowmask - Bits[7:7], RWS, default = 1'b0 
       Cbo 19 prq credit overflow mask
     */
    UINT32 cbo20prqcrdoverflowmask : 1;
    /* cbo20prqcrdoverflowmask - Bits[8:8], RWS, default = 1'b0 
       Cbo 20 prq credit overflow mask
     */
    UINT32 cbo21prqcrdoverflowmask : 1;
    /* cbo21prqcrdoverflowmask - Bits[9:9], RWS, default = 1'b0 
       Cbo 21 prq credit overflow mask
     */
    UINT32 cbo22prqcrdoverflowmask : 1;
    /* cbo22prqcrdoverflowmask - Bits[10:10], RWS, default = 1'b0 
       Cbo 22 prq credit overflow mask
     */
    UINT32 cbo23prqcrdoverflowmask : 1;
    /* cbo23prqcrdoverflowmask - Bits[11:11], RWS, default = 1'b0 
       Cbo 23 prq credit overflow mask
     */
    UINT32 cbo18isochcrdoverflow : 1;
    /* cbo18isochcrdoverflow - Bits[12:12], RW1CS, default = 1'b0 
       Cbo 18 isochronous credit overflow
     */
    UINT32 cbo19isochcrdoverflow : 1;
    /* cbo19isochcrdoverflow - Bits[13:13], RW1CS, default = 1'b0 
       Cbo 19 isochronous credit overflow
     */
    UINT32 cbo20isochcrdoverflow : 1;
    /* cbo20isochcrdoverflow - Bits[14:14], RW1CS, default = 1'b0 
       Cbo 20 isochronous credit overflow
     */
    UINT32 cbo21isochcrdoverflow : 1;
    /* cbo21isochcrdoverflow - Bits[15:15], RW1CS, default = 1'b0 
       Cbo 21 isochronous credit overflow
     */
    UINT32 cbo22isochcrdoverflow : 1;
    /* cbo22isochcrdoverflow - Bits[16:16], RW1CS, default = 1'b0 
       Cbo 22 isochronous credit overflow
     */
    UINT32 cbo23isochcrdoverflow : 1;
    /* cbo23isochcrdoverflow - Bits[17:17], RW1CS, default = 1'b0 
       Cbo 23 isochronous credit overflow
     */
    UINT32 cbo18isochcrdoverflowmask : 1;
    /* cbo18isochcrdoverflowmask - Bits[18:18], RWS, default = 1'b0 
       Cbo 18 isochronous credit overflow mask
     */
    UINT32 cbo19isochcrdoverflowmask : 1;
    /* cbo19isochcrdoverflowmask - Bits[19:19], RWS, default = 1'b0 
       Cbo 19 isochronous credit overflow mask
     */
    UINT32 cbo20isochcrdoverflowmask : 1;
    /* cbo20isochcrdoverflowmask - Bits[20:20], RWS, default = 1'b0 
       Cbo 20 isochronous credit overflow mask
     */
    UINT32 cbo21isochcrdoverflowmask : 1;
    /* cbo21isochcrdoverflowmask - Bits[21:21], RWS, default = 1'b0 
       Cbo 21 isochronous credit overflowMask
     */
    UINT32 cbo22isochcrdoverflowmask : 1;
    /* cbo22isochcrdoverflowmask - Bits[22:22], RWS, default = 1'b0 
       Cbo 22 isochronous credit overflowMask
     */
    UINT32 cbo23isochcrdoverflowmask : 1;
    /* cbo23isochcrdoverflowmask - Bits[23:23], RWS, default = 1'b0 
       Cbo 23 isochronous credit overflowMask
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R2EGRERRLOG3_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* R2EGRERRLOG_R2PCIE_CFG_REG supported on:                                   */
/*       IVT_EP (0x401980B0)                                                  */
/*       IVT_EX (0x401980B0)                                                  */
/*       HSX (0x401800B0)                                                     */
/*       BDX (0x401800B0)                                                     */
/* Register default value:              0x00000000                            */
#define R2EGRERRLOG_R2PCIE_CFG_REG 0x070040B0


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.0.CFG.xml.
 * R2PCIe Egress error log
 */
typedef union {
  struct {
    UINT32 cbo0prqcrdoverflow : 1;
    /* cbo0prqcrdoverflow - Bits[0:0], RW1CS, default = 1'b0 
       Cbo 0 PRQ Credit Overflow
     */
    UINT32 cbo1prqcrdoverflow : 1;
    /* cbo1prqcrdoverflow - Bits[1:1], RW1CS, default = 1'b0 
       Cbo 1 PRQ Credit Overflow
     */
    UINT32 cbo2prqcrdoverflow : 1;
    /* cbo2prqcrdoverflow - Bits[2:2], RW1CS, default = 1'b0 
       Cbo 2 PRQ Credit Overflow
     */
    UINT32 cbo3prqcrdoverflow : 1;
    /* cbo3prqcrdoverflow - Bits[3:3], RW1CS, default = 1'b0 
       Cbo 3 PRQ Credit Overflow
     */
    UINT32 cbo4prqcrdoverflow : 1;
    /* cbo4prqcrdoverflow - Bits[4:4], RW1CS, default = 1'b0 
       Cbo 4 PRQ Credit Overflow
     */
    UINT32 cbo5prqcrdoverflow : 1;
    /* cbo5prqcrdoverflow - Bits[5:5], RW1CS, default = 1'b0 
       Cbo 5 PRQ Credit Overflow
     */
    UINT32 cbo6prqcrdoverflow : 1;
    /* cbo6prqcrdoverflow - Bits[6:6], RW1CS, default = 1'b0 
       Cbo 6 PRQ Credit Overflow
     */
    UINT32 cbo7prqcrdoverflow : 1;
    /* cbo7prqcrdoverflow - Bits[7:7], RW1CS, default = 1'b0 
       Cbo 7 PRQ Credit Overflow
     */
    UINT32 cbo8prqcrdoverflow : 1;
    /* cbo8prqcrdoverflow - Bits[8:8], RW1CS, default = 1'b0 
       Cbo 8 PRQ Credit Overflow
     */
    UINT32 cbo9prqcrdoverflow : 1;
    /* cbo9prqcrdoverflow - Bits[9:9], RW1CS, default = 1'b0 
       Cbo 9 PRQ Credit Overflow
     */
    UINT32 cbo10prqcrdoverflow : 1;
    /* cbo10prqcrdoverflow - Bits[10:10], RW1CS, default = 1'b0 
       Cbo 10 PRQ Credit Overflow
     */
    UINT32 cbo11prqcrdoverflow : 1;
    /* cbo11prqcrdoverflow - Bits[11:11], RW1CS, default = 1'b0 
       Cbo 11 PRQ Credit Overflow
     */
    UINT32 cbo12prqcrdoverflow : 1;
    /* cbo12prqcrdoverflow - Bits[12:12], RW1CS, default = 1'b0 
       Cbo 12 PRQ Credit Overflow
     */
    UINT32 cbo13prqcrdoverflow : 1;
    /* cbo13prqcrdoverflow - Bits[13:13], RW1CS, default = 1'b0 
       Cbo 13 PRQ Credit Overflow
     */
    UINT32 cbo14prqcrdoverflow : 1;
    /* cbo14prqcrdoverflow - Bits[14:14], RW1CS, default = 1'b0 
       Cbo 14 PRQ Credit Overflow
     */
    UINT32 cbo15prqcrdoverflow : 1;
    /* cbo15prqcrdoverflow - Bits[15:15], RW1CS, default = 1'b0 
       Cbo 15 PRQ Credit Overflow
     */
    UINT32 cbo16prqcrdoverflow : 1;
    /* cbo16prqcrdoverflow - Bits[16:16], RW1CS, default = 1'b0 
       Cbo 16 PRQ Credit Overflow
     */
    UINT32 cbo17prqcrdoverflow : 1;
    /* cbo17prqcrdoverflow - Bits[17:17], RW1CS, default = 1'b0 
       Cbo 17 PRQ Credit Overflow
     */
    UINT32 akegress0_write_to_valid_entry : 1;
    /* akegress0_write_to_valid_entry - Bits[18:18], RW1CS, default = 1'b0 
       AK Egress Agent 0 write to occupied entry
     */
    UINT32 akegress1_write_to_valid_entry : 1;
    /* akegress1_write_to_valid_entry - Bits[19:19], RW1CS, default = 1'b0 
       AK Egress Agent 1 write to occupied entry
     */
    UINT32 blegress0_write_to_valid_entry : 1;
    /* blegress0_write_to_valid_entry - Bits[20:20], RW1CS, default = 1'b0 
       BL Egress Agent 0 write to occupied entry
     */
    UINT32 blegress1_write_to_valid_entry : 1;
    /* blegress1_write_to_valid_entry - Bits[21:21], RW1CS, default = 1'b0 
       BL Egress Agent 1 write to occupied entry
     */
    UINT32 adegress0_write_to_valid_entry : 1;
    /* adegress0_write_to_valid_entry - Bits[22:22], RW1CS, default = 1'b0 
       AD Egress Agent 0 write to occupied entry
     */
    UINT32 adegress1_write_to_valid_entry : 1;
    /* adegress1_write_to_valid_entry - Bits[23:23], RW1CS, default = 1'b0 
       AD Egress Agent 1 write to occupied entry
     */
    UINT32 akegress0_overflow : 1;
    /* akegress0_overflow - Bits[24:24], RW1CS, default = 1'b0 
       AK Egress Agent 0 buffer overflow
     */
    UINT32 akegress1_overflow : 1;
    /* akegress1_overflow - Bits[25:25], RW1CS, default = 1'b0 
       AK Egress Agent 1 buffer overflow
     */
    UINT32 blegress0_overflow : 1;
    /* blegress0_overflow - Bits[26:26], RW1CS, default = 1'b0 
       BL Egress Agent 0 buffer overflow
     */
    UINT32 blegress1_overflow : 1;
    /* blegress1_overflow - Bits[27:27], RW1CS, default = 1'b0 
       BL Egress Agent 1 buffer overflow
     */
    UINT32 adegress0_overflow : 1;
    /* adegress0_overflow - Bits[28:28], RW1CS, default = 1'b0 
       AD Egress Agent 0 buffer overflow
     */
    UINT32 adegress1_overflow : 1;
    /* adegress1_overflow - Bits[29:29], RW1CS, default = 1'b0 
       AD Egress Agent 1 buffer overflow
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R2EGRERRLOG_R2PCIE_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* R2EGRERRLOG2_R2PCIE_CFG_REG supported on:                                  */
/*       IVT_EP (0x401980B4)                                                  */
/*       IVT_EX (0x401980B4)                                                  */
/*       HSX (0x401800B4)                                                     */
/*       BDX (0x401800B4)                                                     */
/* Register default value:              0x00000000                            */
#define R2EGRERRLOG2_R2PCIE_CFG_REG 0x070040B4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.0.CFG.xml.
 * R2PCIe Egress Error Log 2
 */
typedef union {
  struct {
    UINT32 cbo0isochcrdoverflow : 1;
    /* cbo0isochcrdoverflow - Bits[0:0], RW1CS, default = 1'b0 
       Cbo 0 isochronous credit overflow
     */
    UINT32 cbo1isochcrdoverflow : 1;
    /* cbo1isochcrdoverflow - Bits[1:1], RW1CS, default = 1'b0 
       Cbo 1 isochronous credit overflow
     */
    UINT32 cbo2isochcrdoverflow : 1;
    /* cbo2isochcrdoverflow - Bits[2:2], RW1CS, default = 1'b0 
       Cbo 2 isochronous credit overflow
     */
    UINT32 cbo3isochcrdoverflow : 1;
    /* cbo3isochcrdoverflow - Bits[3:3], RW1CS, default = 1'b0 
       Cbo 3 isochronous credit overflow
     */
    UINT32 cbo4isochcrdoverflow : 1;
    /* cbo4isochcrdoverflow - Bits[4:4], RW1CS, default = 1'b0 
       Cbo 4 isochronous credit overflow
     */
    UINT32 cbo5isochcrdoverflow : 1;
    /* cbo5isochcrdoverflow - Bits[5:5], RW1CS, default = 1'b0 
       Cbo 5 isochronous credit overflow
     */
    UINT32 cbo6isochcrdoverflow : 1;
    /* cbo6isochcrdoverflow - Bits[6:6], RW1CS, default = 1'b0 
       Cbo 6 isochronous credit overflow
     */
    UINT32 cbo7isochcrdoverflow : 1;
    /* cbo7isochcrdoverflow - Bits[7:7], RW1CS, default = 1'b0 
       Cbo 7 isochronous credit overflow
     */
    UINT32 cbo8isochcrdoverflow : 1;
    /* cbo8isochcrdoverflow - Bits[8:8], RW1CS, default = 1'b0 
       Cbo 8 isochronous credit overflow
     */
    UINT32 cbo9isochcrdoverflow : 1;
    /* cbo9isochcrdoverflow - Bits[9:9], RW1CS, default = 1'b0 
       Cbo 9 isochronous credit overflow
     */
    UINT32 cbo10isochcrdoverflow : 1;
    /* cbo10isochcrdoverflow - Bits[10:10], RW1CS, default = 1'b0 
       Cbo 10 isochronous credit overflow
     */
    UINT32 cbo11isochcrdoverflow : 1;
    /* cbo11isochcrdoverflow - Bits[11:11], RW1CS, default = 1'b0 
       Cbo 11 isochronous credit overflow
     */
    UINT32 cbo12isochcrdoverflow : 1;
    /* cbo12isochcrdoverflow - Bits[12:12], RW1CS, default = 1'b0 
       Cbo 12 isochronous credit overflow
     */
    UINT32 cbo13isochcrdoverflow : 1;
    /* cbo13isochcrdoverflow - Bits[13:13], RW1CS, default = 1'b0 
       Cbo 13 isochronous credit overflow
     */
    UINT32 cbo14isochcrdoverflow : 1;
    /* cbo14isochcrdoverflow - Bits[14:14], RW1CS, default = 1'b0 
       Cbo 14 isochronous credit overflow
     */
    UINT32 cbo15isochcrdoverflow : 1;
    /* cbo15isochcrdoverflow - Bits[15:15], RW1CS, default = 1'b0 
       Cbo 15 isochronous credit overflow
     */
    UINT32 cbo16isochcrdoverflow : 1;
    /* cbo16isochcrdoverflow - Bits[16:16], RW1CS, default = 1'b0 
       Cbo 16 isochronous credit overflow
     */
    UINT32 cbo17isochcrdoverflow : 1;
    /* cbo17isochcrdoverflow - Bits[17:17], RW1CS, default = 1'b0 
       Cbo 17 isochronous credit overflow
     */
    UINT32 parerregr0 : 2;
    /* parerregr0 - Bits[19:18], RW1CS, default = 2'b00 
       BL egress data parity error - Agent 0
     */
    UINT32 parerregr1 : 2;
    /* parerregr1 - Bits[21:20], RW1CS, default = 2'b00 
       BL egress data parity error - Agent 1
     */
    UINT32 rsvd_22 : 4;
    /* rsvd_22 - Bits[25:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 sbocreditoverflowad0 : 1;
    /* sbocreditoverflowad0 - Bits[26:26], RW1CS, default = 1'b0 
       AD0 Sbo Credit Overflow
     */
    UINT32 sbocreditoverflowad1 : 1;
    /* sbocreditoverflowad1 - Bits[27:27], RW1CS, default = 1'b0 
       AD1 Sbo Credit Overflow
     */
    UINT32 sbocreditoverflowbl0 : 1;
    /* sbocreditoverflowbl0 - Bits[28:28], RW1CS, default = 1'b0 
       BL0 Sbo Credit Overflow
     */
    UINT32 sbocreditoverflowbl1 : 1;
    /* sbocreditoverflowbl1 - Bits[29:29], RW1CS, default = 1'b0 
       BL1 Sbo Credit Overflow
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R2EGRERRLOG2_R2PCIE_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* R2EGRERRMSK_R2PCIE_CFG_REG supported on:                                   */
/*       IVT_EP (0x401980B8)                                                  */
/*       IVT_EX (0x401980B8)                                                  */
/*       HSX (0x401800B8)                                                     */
/*       BDX (0x401800B8)                                                     */
/* Register default value:              0x00000000                            */
#define R2EGRERRMSK_R2PCIE_CFG_REG 0x070040B8


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.0.CFG.xml.
 * R2PCIe Error Mask Register
 */
typedef union {
  struct {
    UINT32 cbo0prqcrdoverflowmask : 1;
    /* cbo0prqcrdoverflowmask - Bits[0:0], RWS, default = 1'b0 
       Mask Cbo 0 PRQ credit overflow error
     */
    UINT32 cbo1prqcrdoverflowmask : 1;
    /* cbo1prqcrdoverflowmask - Bits[1:1], RWS, default = 1'b0 
       Mask Cbo 1 PRQ credit overflow error
     */
    UINT32 cbo2prqcrdoverflowmask : 1;
    /* cbo2prqcrdoverflowmask - Bits[2:2], RWS, default = 1'b0 
       Mask Cbo 2 PRQ credit overflow error
     */
    UINT32 cbo3prqcrdoverflowmask : 1;
    /* cbo3prqcrdoverflowmask - Bits[3:3], RWS, default = 1'b0 
       Mask Cbo 3 PRQ credit overflow error
     */
    UINT32 cbo4prqcrdoverflowmask : 1;
    /* cbo4prqcrdoverflowmask - Bits[4:4], RWS, default = 1'b0 
       Mask Cbo 4 PRQ credit overflow error
     */
    UINT32 cbo5prqcrdoverflowmask : 1;
    /* cbo5prqcrdoverflowmask - Bits[5:5], RWS, default = 1'b0 
       Mask Cbo 5 PRQ credit overflow error
     */
    UINT32 cbo6prqcrdoverflowmask : 1;
    /* cbo6prqcrdoverflowmask - Bits[6:6], RWS, default = 1'b0 
       Mask Cbo 6 PRQ credit overflow error
     */
    UINT32 cbo7prqcrdoverflowmask : 1;
    /* cbo7prqcrdoverflowmask - Bits[7:7], RWS, default = 1'b0 
       Mask Cbo 7 PRQ credit overflow error
     */
    UINT32 cbo8prqcrdoverflowmask : 1;
    /* cbo8prqcrdoverflowmask - Bits[8:8], RWS, default = 1'b0 
       Mask Cbo 8 PRQ credit overflow error
     */
    UINT32 cbo9prqcrdoverflowmask : 1;
    /* cbo9prqcrdoverflowmask - Bits[9:9], RWS, default = 1'b0 
       Mask Cbo 9 PRQ credit overflow error
     */
    UINT32 cbo10prqcrdoverflowmask : 1;
    /* cbo10prqcrdoverflowmask - Bits[10:10], RWS, default = 1'b0 
       Mask Cbo 10 PRQ credit overflow error
     */
    UINT32 cbo11prqcrdoverflowmask : 1;
    /* cbo11prqcrdoverflowmask - Bits[11:11], RWS, default = 1'b0 
       Mask Cbo 11 PRQ credit overflow error
     */
    UINT32 cbo12prqcrdoverflowmask : 1;
    /* cbo12prqcrdoverflowmask - Bits[12:12], RWS, default = 1'b0 
       Mask Cbo 12 PRQ credit overflow error
     */
    UINT32 cbo13prqcrdoverflowmask : 1;
    /* cbo13prqcrdoverflowmask - Bits[13:13], RWS, default = 1'b0 
       Mask Cbo 13 PRQ credit overflow error
     */
    UINT32 cbo14prqcrdoverflowmask : 1;
    /* cbo14prqcrdoverflowmask - Bits[14:14], RWS, default = 1'b0 
       Mask Cbo 14 PRQ credit overflow error
     */
    UINT32 cbo15prqcrdoverflowmask : 1;
    /* cbo15prqcrdoverflowmask - Bits[15:15], RWS, default = 1'b0 
       Mask Cbo 15 PRQ credit overflow error
     */
    UINT32 cbo16prqcrdoverflowmask : 1;
    /* cbo16prqcrdoverflowmask - Bits[16:16], RWS, default = 1'b0 
       Mask Cbo 16 PRQ credit overflow error
     */
    UINT32 cbo17prqcrdoverflowmask : 1;
    /* cbo17prqcrdoverflowmask - Bits[17:17], RWS, default = 1'b0 
       Mask Cbo 17 PRQ credit overflow error
     */
    UINT32 akegress0_write_to_valid_entry : 1;
    /* akegress0_write_to_valid_entry - Bits[18:18], RWS, default = 1'b0 
       Mask AK Egress Agent 0 write to occupied entry error
     */
    UINT32 akegress1_write_to_valid_entry : 1;
    /* akegress1_write_to_valid_entry - Bits[19:19], RWS, default = 1'b0 
       Mask AK Egress Agent 1 write to occupied entry error
     */
    UINT32 blegress0_write_to_valid_entry : 1;
    /* blegress0_write_to_valid_entry - Bits[20:20], RWS, default = 1'b0 
       Mask BL Egress Agent 0 write to occupied entry error
     */
    UINT32 blegress1_write_to_valid_entry : 1;
    /* blegress1_write_to_valid_entry - Bits[21:21], RWS, default = 1'b0 
       Mask BL Egress Agent 1 write to occupied entry error
     */
    UINT32 adegress0_write_to_valid_entry : 1;
    /* adegress0_write_to_valid_entry - Bits[22:22], RWS, default = 1'b0 
       Mask AD Egress Agent 0 write to occupied entry error
     */
    UINT32 adegress1_write_to_valid_entry : 1;
    /* adegress1_write_to_valid_entry - Bits[23:23], RWS, default = 1'b0 
       Mask AD Egress Agent 0 write to occupied entry error
     */
    UINT32 akegress0_overflow : 1;
    /* akegress0_overflow - Bits[24:24], RWS, default = 1'b0 
       Mask AK Egress Agent 0 overflow error
     */
    UINT32 akegress1_overflow : 1;
    /* akegress1_overflow - Bits[25:25], RWS, default = 1'b0 
       Mask AK Egress Agent 1 overflow error
     */
    UINT32 blegress0_overflow : 1;
    /* blegress0_overflow - Bits[26:26], RWS, default = 1'b0 
       Mask BL Egress Agent 0 overflow error
     */
    UINT32 blegress1_overflow : 1;
    /* blegress1_overflow - Bits[27:27], RWS, default = 1'b0 
       Mask AK Egress Agent 1 overflow error
     */
    UINT32 adegress0_overflow : 1;
    /* adegress0_overflow - Bits[28:28], RWS, default = 1'b0 
       Mask AD Egress Agent 0 overflow error
     */
    UINT32 adegress1_overflow : 1;
    /* adegress1_overflow - Bits[29:29], RWS, default = 1'b0 
       Mask AD Egress Agent 1 overflow error
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R2EGRERRMSK_R2PCIE_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* R2EGRERRMSK2_R2PCIE_CFG_REG supported on:                                  */
/*       IVT_EP (0x401980BC)                                                  */
/*       IVT_EX (0x401980BC)                                                  */
/*       HSX (0x401800BC)                                                     */
/*       BDX (0x401800BC)                                                     */
/* Register default value:              0x00000000                            */
#define R2EGRERRMSK2_R2PCIE_CFG_REG 0x070040BC


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.0.CFG.xml.
 * R2PCIe Egress error mask 2
 */
typedef union {
  struct {
    UINT32 cbo0isochcrdoverflowmask : 1;
    /* cbo0isochcrdoverflowmask - Bits[0:0], RWS, default = 1'b0 
       Mask Cbo 0 isochronous overflow error
     */
    UINT32 cbo1isochcrdoverflowmask : 1;
    /* cbo1isochcrdoverflowmask - Bits[1:1], RWS, default = 1'b0 
       Mask Cbo 1 isochronous overflow error
     */
    UINT32 cbo2isochcrdoverflowmask : 1;
    /* cbo2isochcrdoverflowmask - Bits[2:2], RWS, default = 1'b0 
       Mask Cbo 2 isochronous overflow error
     */
    UINT32 cbo3isochcrdoverflowmask : 1;
    /* cbo3isochcrdoverflowmask - Bits[3:3], RWS, default = 1'b0 
       Mask Cbo 3 isochronous overflow error
     */
    UINT32 cbo4isochcrdoverflowmask : 1;
    /* cbo4isochcrdoverflowmask - Bits[4:4], RWS, default = 1'b0 
       Mask Cbo 4 isochronous overflow error
     */
    UINT32 cbo5isochcrdoverflowmask : 1;
    /* cbo5isochcrdoverflowmask - Bits[5:5], RWS, default = 1'b0 
       Mask Cbo 5 isochronous overflow error
     */
    UINT32 cbo6isochcrdoverflowmask : 1;
    /* cbo6isochcrdoverflowmask - Bits[6:6], RWS, default = 1'b0 
       Mask Cbo 6 isochronous overflow error
     */
    UINT32 cbo7isochcrdoverflowmask : 1;
    /* cbo7isochcrdoverflowmask - Bits[7:7], RWS, default = 1'b0 
       Mask Cbo 7 isochronous overflow error
     */
    UINT32 cbo8isochcrdoverflowmask : 1;
    /* cbo8isochcrdoverflowmask - Bits[8:8], RWS, default = 1'b0 
       Mask Cbo 8 isochronous overflow error
     */
    UINT32 cbo9isochcrdoverflowmask : 1;
    /* cbo9isochcrdoverflowmask - Bits[9:9], RWS, default = 1'b0 
       Mask Cbo 9 isochronous overflow error
     */
    UINT32 cbo10isochcrdoverflowmask : 1;
    /* cbo10isochcrdoverflowmask - Bits[10:10], RWS, default = 1'b0 
       Mask Cbo 10 isochronous overflow error
     */
    UINT32 cbo11isochcrdoverflowmask : 1;
    /* cbo11isochcrdoverflowmask - Bits[11:11], RWS, default = 1'b0 
       Mask Cbo 11 isochronous overflow error
     */
    UINT32 cbo12isochcrdoverflowmask : 1;
    /* cbo12isochcrdoverflowmask - Bits[12:12], RWS, default = 1'b0 
       Mask Cbo 12 isochronous overflow error
     */
    UINT32 cbo13isochcrdoverflowmask : 1;
    /* cbo13isochcrdoverflowmask - Bits[13:13], RWS, default = 1'b0 
       Mask Cbo 13 isochronous overflow error
     */
    UINT32 cbo14isochcrdoverflowmask : 1;
    /* cbo14isochcrdoverflowmask - Bits[14:14], RWS, default = 1'b0 
       Mask Cbo 14 isochronous overflow error
     */
    UINT32 cbo15isochcrdoverflowmask : 1;
    /* cbo15isochcrdoverflowmask - Bits[15:15], RWS, default = 1'b0 
       Mask Cbo 15 isochronous overflow error
     */
    UINT32 cbo16isochcrdoverflowmask : 1;
    /* cbo16isochcrdoverflowmask - Bits[16:16], RWS, default = 1'b0 
       Mask Cbo 16 isochronous overflow error
     */
    UINT32 cbo17isochcrdoverflowmask : 1;
    /* cbo17isochcrdoverflowmask - Bits[17:17], RWS, default = 1'b0 
       Mask Cbo 17 isochronous overflow error
     */
    UINT32 parerregr0mask : 2;
    /* parerregr0mask - Bits[19:18], RWS, default = 2'b00 
       Mask BL egress parity error - Agent 0
     */
    UINT32 parerregr1mask : 2;
    /* parerregr1mask - Bits[21:20], RWS, default = 2'b00 
       Mask BL egress parity error - Agent 1
     */
    UINT32 rsvd_22 : 4;
    /* rsvd_22 - Bits[25:22], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 sbocreditoverflowad0mask : 1;
    /* sbocreditoverflowad0mask - Bits[26:26], RWS, default = 1'b0 
       Mask AD0 Sbo Credit Overflow
     */
    UINT32 sbocreditoverflowad1mask : 1;
    /* sbocreditoverflowad1mask - Bits[27:27], RWS, default = 1'b0 
       Mask AD1 Sbo Credit Overflow
     */
    UINT32 sbocreditoverflowbl0mask : 1;
    /* sbocreditoverflowbl0mask - Bits[28:28], RWS, default = 1'b0 
       Mask BL0 Sbo Credit Overflow
     */
    UINT32 sbocreditoverflowbl1mask : 1;
    /* sbocreditoverflowbl1mask - Bits[29:29], RWS, default = 1'b0 
       Mask BL1 Sbo Credit Overflow
     */
    UINT32 rsvd_30 : 2;
    /* rsvd_30 - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} R2EGRERRMSK2_R2PCIE_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */






/* R2PCIE_ASC_LDVAL_LO_0_R2PCIE_CFG_REG supported on:                         */
/*       HSX (0x401800E8)                                                     */
/*       BDX (0x401800E8)                                                     */
/* Register default value:              0x00000000                            */
#define R2PCIE_ASC_LDVAL_LO_0_R2PCIE_CFG_REG 0x070040E8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
 * TBD
 */
typedef union {
  struct {
    UINT32 load_low_value : 32;
    /* load_low_value - Bits[31:0], RWS_L, default = 32'h0  */
  } Bits;
  UINT32 Data;
} R2PCIE_ASC_LDVAL_LO_0_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* R2PCIE_ASC_LDVAL_HI_0_R2PCIE_CFG_REG supported on:                         */
/*       HSX (0x401800EC)                                                     */
/*       BDX (0x401800EC)                                                     */
/* Register default value:              0x00000000                            */
#define R2PCIE_ASC_LDVAL_HI_0_R2PCIE_CFG_REG 0x070040EC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
 * TBD
 */
typedef union {
  struct {
    UINT32 load_high_value : 32;
    /* load_high_value - Bits[31:0], RWS_L, default = 32'h0  */
  } Bits;
  UINT32 Data;
} R2PCIE_ASC_LDVAL_HI_0_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* R2PCIE_ASC_CONTROL_0_R2PCIE_CFG_REG supported on:                          */
/*       HSX (0x201800F0)                                                     */
/*       BDX (0x201800F0)                                                     */
/* Register default value:              0x0000                                */
#define R2PCIE_ASC_CONTROL_0_R2PCIE_CFG_REG 0x070020F0

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
 * TBD
 */
typedef union {
  struct {
    UINT16 current_asc0_state_output : 1;
    /* current_asc0_state_output - Bits[0:0], RWS_LV, default = 1'b0  */
    UINT16 enable_asc0 : 1;
    /* enable_asc0 - Bits[1:1], RWS_L, default = 1'b0  */
    UINT16 rsvd_2 : 5;
    /* rsvd_2 - Bits[6:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 enable_mbp_qualification : 1;
    /* enable_mbp_qualification - Bits[7:7], RWS_L, default = 1'b0  */
    UINT16 mbp_selector : 3;
    /* mbp_selector - Bits[10:8], RWS_L, default = 3'b000  */
    UINT16 rsvd_11 : 5;
    /* rsvd_11 - Bits[15:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT16 Data;
} R2PCIE_ASC_CONTROL_0_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* R2PCIE_GLB_RSP_CNTRL_0_R2PCIE_CFG_REG supported on:                        */
/*       HSX (0x201800F2)                                                     */
/*       BDX (0x201800F2)                                                     */
/* Register default value:              0x0000                                */
#define R2PCIE_GLB_RSP_CNTRL_0_R2PCIE_CFG_REG 0x070020F2

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.0.CFG.xml.
 * TBD
 */
typedef union {
  struct {
    UINT16 start_trigger_selector_for_global_response_0 : 3;
    /* start_trigger_selector_for_global_response_0 - Bits[2:0], RWS_L, default = 3'b000  */
    UINT16 stop_trigger_selector_for_global_response_0 : 3;
    /* stop_trigger_selector_for_global_response_0 - Bits[5:3], RWS_L, default = 3'b000  */
    UINT16 rsvd_6 : 2;
    /* rsvd_6 - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 start_trigger_selector_for_global_response_1 : 3;
    /* start_trigger_selector_for_global_response_1 - Bits[10:8], RWS_L, default = 3'b000  */
    UINT16 stop_trigger_selector_for_global_response_1 : 3;
    /* stop_trigger_selector_for_global_response_1 - Bits[13:11], RWS_L, default = 3'b000  */
    UINT16 rsvd_14 : 2;
    /* rsvd_14 - Bits[15:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT16 Data;
} R2PCIE_GLB_RSP_CNTRL_0_R2PCIE_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* SDID_R2PCIE_CFG_REG supported on:                                          */
/*       IVT_EP (0x2019802E)                                                  */
/*       IVT_EX (0x2019802E)                                                  */
/* Register default value:              0x0000                                */
#define SDID_R2PCIE_CFG_REG 0x0700202E



/* R2_GL_ERR_CFG_R2PCIE_CFG_REG supported on:                                 */
/*       IVT_EP (0x20198080)                                                  */
/*       IVT_EX (0x20198080)                                                  */
/* Register default value:              0x7FC0                                */
#define R2_GL_ERR_CFG_R2PCIE_CFG_REG 0x07002080



/* R2PCIE_DBG_BUS_CONTROL_R2PCIE_CFG_REG supported on:                        */
/*       IVT_EP (0x101980E4)                                                  */
/*       IVT_EX (0x101980E4)                                                  */
/* Register default value:              0x08                                  */
#define R2PCIE_DBG_BUS_CONTROL_R2PCIE_CFG_REG 0x070010E4



/* R2PCIE_DBG_BUS_MATCH_N0_R2PCIE_CFG_REG supported on:                       */
/*       IVT_EP (0x101980E5)                                                  */
/*       IVT_EX (0x101980E5)                                                  */
/* Register default value:              0x00                                  */
#define R2PCIE_DBG_BUS_MATCH_N0_R2PCIE_CFG_REG 0x070010E5



/* R2PCIE_DBG_BUS_MATCH_N1_R2PCIE_CFG_REG supported on:                       */
/*       IVT_EP (0x101980E6)                                                  */
/*       IVT_EX (0x101980E6)                                                  */
/* Register default value:              0x00                                  */
#define R2PCIE_DBG_BUS_MATCH_N1_R2PCIE_CFG_REG 0x070010E6



/* R2PCIE_DBG_BUS_MASK_R2PCIE_CFG_REG supported on:                           */
/*       IVT_EP (0x201980E8)                                                  */
/*       IVT_EX (0x201980E8)                                                  */
/* Register default value:              0x0000                                */
#define R2PCIE_DBG_BUS_MASK_R2PCIE_CFG_REG 0x070020E8



/* R2PCIE_ASC_CNTR_R2PCIE_CFG_REG supported on:                               */
/*       IVT_EP (0x201980EA)                                                  */
/*       IVT_EX (0x201980EA)                                                  */
/* Register default value:              0x0000                                */
#define R2PCIE_ASC_CNTR_R2PCIE_CFG_REG 0x070020EA



/* R2PCIE_ASC_LDVAL_R2PCIE_CFG_REG supported on:                              */
/*       IVT_EP (0x401980EC)                                                  */
/*       IVT_EX (0x401980EC)                                                  */
/* Register default value:              0x00000000                            */
#define R2PCIE_ASC_LDVAL_R2PCIE_CFG_REG 0x070040EC



/* R2PCIE_ASC_CONTROL_R2PCIE_CFG_REG supported on:                            */
/*       IVT_EP (0x201980F0)                                                  */
/*       IVT_EX (0x201980F0)                                                  */
/* Register default value:              0x0000                                */
#define R2PCIE_ASC_CONTROL_R2PCIE_CFG_REG 0x070020F0



/* R2PCIE_GLB_RSP_CNTRL_R2PCIE_CFG_REG supported on:                          */
/*       IVT_EP (0x201980F2)                                                  */
/*       IVT_EX (0x201980F2)                                                  */
/* Register default value:              0x0000                                */
#define R2PCIE_GLB_RSP_CNTRL_R2PCIE_CFG_REG 0x070020F2



/* R2PCIE_LCL_RESP_CNTRL_R2PCIE_CFG_REG supported on:                         */
/*       IVT_EP (0x201980F4)                                                  */
/*       IVT_EX (0x201980F4)                                                  */
/* Register default value:              0x0000                                */
#define R2PCIE_LCL_RESP_CNTRL_R2PCIE_CFG_REG 0x070020F4



#endif /* R2PCIE_CFG_h */
