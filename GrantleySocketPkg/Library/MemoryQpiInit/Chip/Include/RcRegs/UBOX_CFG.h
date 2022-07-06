/* Date Stamp: 8/23/2014 */

#ifndef UBOX_CFG_h
#define UBOX_CFG_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* UBOX_CFG_IVT_DEV 11                                                        */
/* UBOX_CFG_IVT_FUN 0                                                         */
/* For HSX_HOST:                                                              */
/* UBOX_CFG_HSX_DEV 16                                                        */
/* UBOX_CFG_HSX_FUN 5                                                         */
/* For BDX_HOST:                                                              */
/* UBOX_CFG_BDX_DEV 16                                                        */
/* UBOX_CFG_BDX_FUN 5                                                         */

/* VID_UBOX_CFG_REG supported on:                                             */
/*       IVT_EP (0x20158000)                                                  */
/*       IVT_EX (0x20158000)                                                  */
/*       HSX (0x20185000)                                                     */
/*       BDX (0x20185000)                                                     */
/* Register default value:              0x8086                                */
#define VID_UBOX_CFG_REG 0x08002000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
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
} VID_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* DID_UBOX_CFG_REG supported on:                                             */
/*       IVT_EP (0x20158002)                                                  */
/*       IVT_EX (0x20158002)                                                  */
/*       HSX (0x20185002)                                                     */
/*       BDX (0x20185002)                                                     */
/* Register default value on IVT_EP:    0x0E1E                                */
/* Register default value on IVT_EX:    0x0E1E                                */
/* Register default value on HSX:       0x2F1E                                */
/* Register default value on BDX:       0x6F1E                                */
#define DID_UBOX_CFG_REG 0x08002002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6F1E 
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
} DID_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* PCICMD_UBOX_CFG_REG supported on:                                          */
/*       IVT_EP (0x20158004)                                                  */
/*       IVT_EX (0x20158004)                                                  */
/*       HSX (0x20185004)                                                     */
/*       BDX (0x20185004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_UBOX_CFG_REG 0x08002004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
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
} PCICMD_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* PCISTS_UBOX_CFG_REG supported on:                                          */
/*       IVT_EP (0x20158006)                                                  */
/*       IVT_EX (0x20158006)                                                  */
/*       HSX (0x20185006)                                                     */
/*       BDX (0x20185006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_UBOX_CFG_REG 0x08002006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.5.CFG.xml.
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
} PCISTS_UBOX_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_UBOX_CFG_REG supported on:                                             */
/*       IVT_EP (0x10158008)                                                  */
/*       IVT_EX (0x10158008)                                                  */
/*       HSX (0x10185008)                                                     */
/*       BDX (0x10185008)                                                     */
/* Register default value:              0x00                                  */
#define RID_UBOX_CFG_REG 0x08001008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
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
} RID_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_UBOX_CFG_REG supported on:                                          */
/*       IVT_EP (0x10158009)                                                  */
/*       IVT_EX (0x10158009)                                                  */
/*       HSX (0x10185009)                                                     */
/*       BDX (0x10185009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_UBOX_CFG_REG 0x08001009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.5.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_UBOX_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_UBOX_CFG_REG supported on:                                          */
/*       IVT_EP (0x2015800A)                                                  */
/*       IVT_EX (0x2015800A)                                                  */
/*       HSX (0x2018500A)                                                     */
/*       BDX (0x2018500A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_UBOX_CFG_REG 0x0800200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
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
} CCR_N1_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* CLSR_UBOX_CFG_REG supported on:                                            */
/*       IVT_EP (0x1015800C)                                                  */
/*       IVT_EX (0x1015800C)                                                  */
/*       HSX (0x1018500C)                                                     */
/*       BDX (0x1018500C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_UBOX_CFG_REG 0x0800100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
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
} CLSR_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* PLAT_UBOX_CFG_REG supported on:                                            */
/*       IVT_EP (0x1015800D)                                                  */
/*       IVT_EX (0x1015800D)                                                  */
/*       HSX (0x1018500D)                                                     */
/*       BDX (0x1018500D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_UBOX_CFG_REG 0x0800100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
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
} PLAT_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* HDR_UBOX_CFG_REG supported on:                                             */
/*       IVT_EP (0x1015800E)                                                  */
/*       IVT_EX (0x1015800E)                                                  */
/*       HSX (0x1018500E)                                                     */
/*       BDX (0x1018500E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_UBOX_CFG_REG 0x0800100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
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
} HDR_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* BIST_UBOX_CFG_REG supported on:                                            */
/*       IVT_EP (0x1015800F)                                                  */
/*       IVT_EX (0x1015800F)                                                  */
/*       HSX (0x1018500F)                                                     */
/*       BDX (0x1018500F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_UBOX_CFG_REG 0x0800100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
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
} BIST_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* SVID_UBOX_CFG_REG supported on:                                            */
/*       IVT_EP (0x2015802C)                                                  */
/*       IVT_EX (0x2015802C)                                                  */
/*       HSX (0x2018502C)                                                     */
/*       BDX (0x2018502C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_UBOX_CFG_REG 0x0800202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 */
typedef union {
  struct {
    UINT16 subsystem_vendor_identification_number : 16;
    /* subsystem_vendor_identification_number - Bits[15:0], RW_O, default = 16'b1000000010000110 
       The default value specifies Intel but can be set to any value once after reset.
     */
  } Bits;
  UINT16 Data;
} SVID_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* SDID_UBOX_CFG_REG supported on:                                            */
/*       IVT_EP (0x2015802E)                                                  */
/*       IVT_EX (0x2015802E)                                                  */
/*       HSX (0x2018502E)                                                     */
/*       BDX (0x2018502E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_UBOX_CFG_REG 0x0800202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 */
typedef union {
  struct {
    UINT16 subsystem_device_identification_number : 16;
    /* subsystem_device_identification_number - Bits[15:0], RW_O, default = 16'b0000000000000000 
       Assigned by the subsystem vendor to uniquely identify the subsystem
     */
  } Bits;
  UINT16 Data;
} SDID_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_UBOX_CFG_REG supported on:                                          */
/*       IVT_EP (0x10158034)                                                  */
/*       IVT_EX (0x10158034)                                                  */
/*       HSX (0x10185034)                                                     */
/*       BDX (0x10185034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_UBOX_CFG_REG 0x08001034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
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
} CAPPTR_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* INTL_UBOX_CFG_REG supported on:                                            */
/*       IVT_EP (0x1015803C)                                                  */
/*       IVT_EX (0x1015803C)                                                  */
/*       HSX (0x1018503C)                                                     */
/*       BDX (0x1018503C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_UBOX_CFG_REG 0x0800103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
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
} INTL_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* INTPIN_UBOX_CFG_REG supported on:                                          */
/*       IVT_EP (0x1015803D)                                                  */
/*       IVT_EX (0x1015803D)                                                  */
/*       HSX (0x1018503D)                                                     */
/*       BDX (0x1018503D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_UBOX_CFG_REG 0x0800103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
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
} INTPIN_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* MINGNT_UBOX_CFG_REG supported on:                                          */
/*       IVT_EP (0x1015803E)                                                  */
/*       IVT_EX (0x1015803E)                                                  */
/*       HSX (0x1018503E)                                                     */
/*       BDX (0x1018503E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_UBOX_CFG_REG 0x0800103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
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
} MINGNT_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_UBOX_CFG_REG supported on:                                          */
/*       IVT_EP (0x1015803F)                                                  */
/*       IVT_EX (0x1015803F)                                                  */
/*       HSX (0x1018503F)                                                     */
/*       BDX (0x1018503F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_UBOX_CFG_REG 0x0800103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
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
} MAXLAT_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* CPUNODEID_UBOX_CFG_REG supported on:                                       */
/*       IVT_EP (0x40158040)                                                  */
/*       IVT_EX (0x40158040)                                                  */
/*       HSX (0x40185040)                                                     */
/*       BDX (0x40185040)                                                     */
/* Register default value:              0x00000000                            */
#define CPUNODEID_UBOX_CFG_REG 0x08004040
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * Node ID Configuration Register
 */
typedef union {
  struct {
    UINT32 lclnodeid : 3;
    /* lclnodeid - Bits[2:0], RW_LB, default = 3'b000 
       Node Id of the local Socket
     */
    UINT32 rsvd_3 : 2;
    /* rsvd_3 - Bits[4:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 locknodeid : 3;
    /* locknodeid - Bits[7:5], RW_LB, default = 3'b000 
       NodeId of the lock master
     */
    UINT32 rsvd_8 : 2;
    /* rsvd_8 - Bits[9:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 lgcnodeid : 3;
    /* lgcnodeid - Bits[12:10], RW_LB, default = 3'b000 
       NodeID of the legacy socket
     */
    UINT32 nodectrlid : 3;
    /* nodectrlid - Bits[15:13], RW_LB, default = 3'b000 
       Node ID of the Node Controller. Set by the BIOS. (EX processor only)
     */
    UINT32 rsvd_16 : 16;
    /* rsvd_16 - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CPUNODEID_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* CPUENABLE_UBOX_CFG_REG supported on:                                       */
/*       IVT_EP (0x40158044)                                                  */
/*       IVT_EX (0x40158044)                                                  */
/*       HSX (0x40185044)                                                     */
/*       BDX (0x40185044)                                                     */
/* Register default value:              0x00000000                            */
#define CPUENABLE_UBOX_CFG_REG 0x08004044
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * Indicates which CPU is enabled
 */
typedef union {
  struct {
    UINT32 nodeiden : 8;
    /* nodeiden - Bits[7:0], RW_LB, default = 8'b00000000 
       Bit mask to indicate which node_id is enabled.
     */
    UINT32 rsvd : 23;
    /* rsvd - Bits[30:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 valid : 1;
    /* valid - Bits[31:31], RW_LB, default = 1'b0 
       Valid bit to indicate that the register has been initialized.
     */
  } Bits;
  UINT32 Data;
} CPUENABLE_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* INTCONTROL_UBOX_CFG_REG supported on:                                      */
/*       IVT_EP (0x40158048)                                                  */
/*       IVT_EX (0x40158048)                                                  */
/*       HSX (0x40185048)                                                     */
/*       BDX (0x40185048)                                                     */
/* Register default value:              0x00000001                            */
#define INTCONTROL_UBOX_CFG_REG 0x08004048
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * Interrupt Configuration Register
 */
typedef union {
  struct {
    UINT32 xapicen : 1;
    /* xapicen - Bits[0:0], RW_LB, default = 1'b1 
       Set this bit if you would like extended XAPIC configuration to be used. This bit 
       can be written directly, and can also be updated using XTPR messages 
     */
    UINT32 forcex2apic : 1;
    /* forcex2apic - Bits[1:1], RW_LB, default = 1'b0 
       Write:
       1: Forces the system to move into X2APIC Mode.
       0: No affect
     */
    UINT32 rsvd_2 : 2;
    /* rsvd_2 - Bits[3:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rdrmodsel : 3;
    /* rdrmodsel - Bits[6:4], RW, default = 3'b000 
       Selects the redirection mode used for MSI interrupts with lowest-priority 
       delivery mode. The folowing schemes are used :  
       000 : Fixed Priority - select the first enabled APIC in the cluster. 
       001: Redirect last - last vector selected (applicable only in extended mode)
       010 : Hash Vector - select the first enabled APIC in round robin manner starting 
       form the hash of the vector number.  
       default: Fixed Priority
     */
    UINT32 rsvd_7 : 1;
    /* rsvd_7 - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 hashmodctr : 3;
    /* hashmodctr - Bits[10:8], RW, default = 3'b000 
       Indicates the hash mode control for the interrupt control.
       Select the hush function for the Vector based Hash Mode interrupt redirection 
       control :  
       000 select bits 7:4/5:4 for vector cluster/flat algorithm 
       001 select bits 6:3/4:3 
       010 select bits 4:1/2:1 
       011 select bits 3:0/1:0 
       other - reserved
     */
    UINT32 rsvd_11 : 5;
    /* rsvd_11 - Bits[15:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 clastchksmpmod : 1;
    /* clastchksmpmod - Bits[16:16], RW_LB, default = 1'b0 
       0: Disable checking for Logical_APICID[31:0] being non-zero when sampling flat/ 
       cluster mode bit in the IntPrioUpd message as part of setting bit 1 in this 
       register  
       1: Enable the above checking
     */
    UINT32 logfltclustmod : 1;
    /* logfltclustmod - Bits[17:17], RW_LBV, default = 1'b0 
       Set by bios to indicate if the OS is running logical flat or logical cluster 
       mode. This bit can also be updated by IntPrioUpd messages.  
       This bit reflects the setup of the filter at any given time. 
       0 - flat, 
       1 - cluster.
     */
    UINT32 logflatclustovren : 1;
    /* logflatclustovren - Bits[18:18], RW_LB, default = 1'b0 
       0 : IA32 Logical Flat or Cluster Mode bit is locked as Read only bit. 
       1 : IA32 Logical Flat or Cluster Mode bit may be written by SW, values written 
       by xTPR update are ignored.  
       For one time override of the IA32 Logical Flat or Cluster Mode value, return 
       this bit to it's default state after the bit is changed. Leaving this bit as '1' 
       will prevent automatic update of the filter. 
     */
    UINT32 rsvd_19 : 13;
    /* rsvd_19 - Bits[31:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} INTCONTROL_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* BCASTLIST_UBOX_CFG_REG supported on:                                       */
/*       IVT_EP (0x4015804C)                                                  */
/*       IVT_EX (0x4015804C)                                                  */
/*       HSX (0x4018504C)                                                     */
/*       BDX (0x4018504C)                                                     */
/* Register default value:              0x00000000                            */
#define BCASTLIST_UBOX_CFG_REG 0x0800404C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * Broadcast lists
 */
typedef union {
  struct {
    UINT32 intlist : 8;
    /* intlist - Bits[7:0], RW_LB, default = 8'b00000000 
       Broadcast list for interrupts and PMReq in an hot-add capable system
     */
    UINT32 stopstartlist : 8;
    /* stopstartlist - Bits[15:8], RW_LB, default = 8'b00000000 
       Broadcast list for StopReq/StartReq in a hot-add capable system
     */
    UINT32 misclist : 8;
    /* misclist - Bits[23:16], RW_LB, default = 8'b00000000 
       Broadcast list for WbInvdAck/InvdAck/EOI in a hot-add capable system
     */
    UINT32 intpriolist : 8;
    /* intpriolist - Bits[31:24], RW_LB, default = 8'b00000000 
       Broadcast list for IntPrioUpd in a hot-add capable system
     */
  } Bits;
  UINT32 Data;
} BCASTLIST_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* LOCKCONTROL_UBOX_CFG_REG supported on:                                     */
/*       IVT_EP (0x40158050)                                                  */
/*       IVT_EX (0x40158050)                                                  */
/*       HSX (0x40185050)                                                     */
/*       BDX (0x40185050)                                                     */
/* Register default value:              0x00000003                            */
#define LOCKCONTROL_UBOX_CFG_REG 0x08004050
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * Lock Control registers
 */
typedef union {
  struct {
    UINT32 lockdisable : 1;
    /* lockdisable - Bits[0:0], RW, default = 1'b1 
       Whether Locks are enabled in the system or not
     */
    UINT32 lockdelay : 3;
    /* lockdelay - Bits[3:1], RW, default = 3'b001 
       Delay between SysLock
       This may be used to prevent starvation on frequent Lock usage.
          000 - 0x0
          001 - 0x200 (1.2us)
          010 - 0x1000 (10us)
          011 - 0x2000 (20us)
          100 - 0x4000 (40us)
          101 - 0x8000 (80 us)
          110 - 0x10000 (160 us)
          111 - 0x20000 (320 us)
     */
    UINT32 compatibilitymode : 1;
    /* compatibilitymode - Bits[4:4], RW, default = 1'b0 
       Enable Compatibility Mode
     */
    UINT32 rsvd : 27;
    /* rsvd - Bits[31:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} LOCKCONTROL_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* GIDNIDMAP_UBOX_CFG_REG supported on:                                       */
/*       IVT_EP (0x40158054)                                                  */
/*       IVT_EX (0x40158054)                                                  */
/*       HSX (0x40185054)                                                     */
/*       BDX (0x40185054)                                                     */
/* Register default value:              0x00000000                            */
#define GIDNIDMAP_UBOX_CFG_REG 0x08004054
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * Mapping between group id and nodeid
 */
typedef union {
  struct {
    UINT32 nodeid0 : 3;
    /* nodeid0 - Bits[2:0], RW_LB, default = 3'b000 
       Node Id for group 0
     */
    UINT32 nodeid1 : 3;
    /* nodeid1 - Bits[5:3], RW_LB, default = 3'b000 
       Node Id for group Id 1
     */
    UINT32 nodeid2 : 3;
    /* nodeid2 - Bits[8:6], RW_LB, default = 3'b000 
       Node Id for group Id 2
     */
    UINT32 nodeid3 : 3;
    /* nodeid3 - Bits[11:9], RW_LB, default = 3'b000 
       Node Id for group 3
     */
    UINT32 nodeid4 : 3;
    /* nodeid4 - Bits[14:12], RW_LB, default = 3'b000 
       Node Id for group id 4
     */
    UINT32 nodeid5 : 3;
    /* nodeid5 - Bits[17:15], RW_LB, default = 3'b000 
       Node Id for group 5
     */
    UINT32 nodeid6 : 3;
    /* nodeid6 - Bits[20:18], RW_LB, default = 3'b000 
       Node Id for group 6
     */
    UINT32 nodeid7 : 3;
    /* nodeid7 - Bits[23:21], RW_LB, default = 3'b000 
       NodeId for group id 7
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} GIDNIDMAP_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* EMCA_EN_CORE_IERR_TO_MSMI_UBOX_CFG_REG supported on:                       */
/*       HSX (0x40185058)                                                     */
/*       BDX (0x40185058)                                                     */
/* Register default value:              0x00000000                            */
#define EMCA_EN_CORE_IERR_TO_MSMI_UBOX_CFG_REG 0x08004058

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * When this bit set the ubox will convert IERR's generated by the core to an 
 * MSMI's to allow firmware  
 *                    first model. In addition the Ubox will signal the PCU to 
 * assert the MSMI pin.   
 *        the MSMI socket pin
 */
typedef union {
  struct {
    UINT32 encoreierrtomsmi : 1;
    /* encoreierrtomsmi - Bits[0:0], RW_LB, default = 1'b0 
       setting this bit enables the Ubox to convert an IERR from the core to an MSMI. 
       Additionally the Ubox  
                            will send a NCU_PCU_MSG to the PCU with the IERR and MSMI 
       bit asserted.   
     */
    UINT32 rsvd : 31;
    /* rsvd - Bits[31:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} EMCA_EN_CORE_IERR_TO_MSMI_UBOX_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* CORECOUNT_UBOX_CFG_REG supported on:                                       */
/*       IVT_EP (0x40158060)                                                  */
/*       IVT_EX (0x40158060)                                                  */
/*       HSX (0x40185060)                                                     */
/*       BDX (0x40185060)                                                     */
/* Register default value:              0x00000000                            */
#define CORECOUNT_UBOX_CFG_REG 0x08004060
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * Reflection of the LTCount2 register
 */
typedef union {
  struct {
    UINT32 corecount : 5;
    /* corecount - Bits[4:0], RO_V, default = 5'b00000 
       Reflection of the LTCount2 uCR
     */
    UINT32 rsvd : 27;
    /* rsvd - Bits[31:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CORECOUNT_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* UBOXERRSTS_UBOX_CFG_REG supported on:                                      */
/*       IVT_EP (0x40158064)                                                  */
/*       IVT_EX (0x40158064)                                                  */
/*       HSX (0x40185064)                                                     */
/*       BDX (0x40185064)                                                     */
/* Register default value:              0x00000000                            */
#define UBOXERRSTS_UBOX_CFG_REG 0x08004064
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * This is error status register in the Ubox and covers most of the interrupt 
 * related errors 
 */
typedef union {
  struct {
    UINT32 smisrcumc : 1;
    /* smisrcumc - Bits[0:0], RWS_V, default = 1'b0 
       This is a bit that indicates that an SMI was caused due to a locally generated 
       UMC 
     */
    UINT32 smisrcimc : 1;
    /* smisrcimc - Bits[1:1], RWS_V, default = 1'b0 
       SMI is caused due to an imdication from the IMC
     */
    UINT32 poisonrsvd : 1;
    /* poisonrsvd - Bits[2:2], RWS_V, default = 1'b0 
       Ubox received a poisoned transaction
     */
    UINT32 unsupportedopcode : 1;
    /* unsupportedopcode - Bits[3:3], RWS_V, default = 1'b0 
       Unsupported opcode received by Ubox
     */
    UINT32 cfgrdaddrmisaligned : 1;
    /* cfgrdaddrmisaligned - Bits[4:4], RWS_V, default = 1'b0 
       MMCFG Read Address Misaligment received by Ubox
     */
    UINT32 cfgwraddrmisaligned : 1;
    /* cfgwraddrmisaligned - Bits[5:5], RWS_V, default = 1'b0 
       MMCFG Write Address Misaligment received by Ubox
     */
    UINT32 smitimeout : 1;
    /* smitimeout - Bits[6:6], RWS_V, default = 1'b0 
       SMI Timeout received by Ubox
     */
    UINT32 masterlocktimeout : 1;
    /* masterlocktimeout - Bits[7:7], RWS_V, default = 1'b0 
       Master Lock Timeout received by Ubox
     */
    UINT32 reserved : 8;
    /* reserved - Bits[15:8], RO_V, default = 8'h0 
       reserved
     */
    UINT32 smi_delivery_valid : 1;
    /* smi_delivery_valid - Bits[16:16], RW_V, default = 1'b0 
       SMI interrupt delivery status valid, write 1'b0 to clear valid status
     */
    UINT32 msg_ch_tkr_err : 1;
    /* msg_ch_tkr_err - Bits[17:17], RWS_V, default = 1'b0 
       Message Channel Tracker Error. This error occurs such case that illegal broad 
       cast port ID access to the message channel. The event is SV use and logging 
       only, not signaling as Ubox error. 
     */
    UINT32 msg_ch_tkr_timeout : 6;
    /* msg_ch_tkr_timeout - Bits[23:18], RWS_V, default = 6'b000000 
       Message Channel Tracker TimeOut. This error occurs when any NP request doesn't 
       receive response in 4K cycles. The event is SV use and logging only, not 
       signaling as Ubox error. 
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} UBOXERRSTS_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* UBOXERRCTL_UBOX_CFG_REG supported on:                                      */
/*       IVT_EP (0x40158068)                                                  */
/*       IVT_EX (0x40158068)                                                  */
/*       HSX (0x40185068)                                                     */
/*       BDX (0x40185068)                                                     */
/* Register default value:              0x81FF7FC0                            */
#define UBOXERRCTL_UBOX_CFG_REG 0x08004068
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * Ubox per-error enable mask. If masked, a given Ubox error will not be signaled 
 * via SMI and will not be logged.  
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    /* rsvd_0 - Bits[1:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 poisonmask : 1;
    /* poisonmask - Bits[2:2], RWS, default = 1'b0 
       Mask SMI generation on receiving poison in Ubox.
     */
    UINT32 unsupportedmask : 1;
    /* unsupportedmask - Bits[3:3], RWS, default = 1'b0 
       Mask SMI generation on receiving unsupported opcodes.
     */
    UINT32 cfgrdaddrmisalignedmask : 1;
    /* cfgrdaddrmisalignedmask - Bits[4:4], RWS, default = 1'b0 
       Mask SMI generation on receiving MMCFG read address misalignment.
     */
    UINT32 cfgwraddrmisalignedmask : 1;
    /* cfgwraddrmisalignedmask - Bits[5:5], RWS, default = 1'b0 
       Mask SMI generation on receiving MMCFG write address misalignment.
     */
    UINT32 smitimeoutmask : 1;
    /* smitimeoutmask - Bits[6:6], RWS, default = 1'b1 
       Mask SMI generation on receiving SMI Timeout. (Mask on default)
     */
    UINT32 smitimeoutbasesizeselect : 2;
    /* smitimeoutbasesizeselect - Bits[8:7], RWS, default = 2'h3 
       Selecting the size of the bit base counter 00->6 bits, 01->15 bits, 10->20 bits, 
       11->24 bits 
     */
    UINT32 smitimeoutcountmatch : 6;
    /* smitimeoutcountmatch - Bits[14:9], RWS, default = 6'h3F 
       The value to match the time out counter [9:4]
     */
    UINT32 masterlocktimeoutmask : 1;
    /* masterlocktimeoutmask - Bits[15:15], RWS, default = 1'b0 
       Mask SMI generation on receiving Master Lock Timeout.
     */
    UINT32 masterlocktimeoutbasesizeselect : 2;
    /* masterlocktimeoutbasesizeselect - Bits[17:16], RWS, default = 2'h3 
       Selecting the size of the bit base counter 00->6 bits, 01->15 bits, 10->20 bits, 
       11->24 bits 
     */
    UINT32 masterlocktimeoutcountmatch : 6;
    /* masterlocktimeoutcountmatch - Bits[23:18], RWS, default = 6'h3F 
       The value to match the time out counter [9:4]
     */
    UINT32 uboxerrortopcumask : 1;
    /* uboxerrortopcumask - Bits[24:24], RWS, default = 1'b1 
       Set 1 to mask direct MCA from Ubox local error to PCU
     */
    UINT32 rsvd_25 : 6;
    /* rsvd_25 - Bits[30:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 nmibiosenable : 1;
    /* nmibiosenable - Bits[31:31], RWS, default = 1'b1 
       Set to 1 to enable signalling of pin-based NMI to BIOS
     */
  } Bits;
  UINT32 Data;
} UBOXERRCTL_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* UBOXERRMISC_UBOX_CFG_REG supported on:                                     */
/*       IVT_EP (0x4015806C)                                                  */
/*       IVT_EX (0x4015806C)                                                  */
/*       HSX (0x4018506C)                                                     */
/*       BDX (0x4018506C)                                                     */
/* Register default value:              0x00000000                            */
#define UBOXERRMISC_UBOX_CFG_REG 0x0800406C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.5.CFG.xml.
 * Logging of additional error information. 
 */
typedef union {
  struct {
    UINT32 opcode : 4;
    /* opcode - Bits[3:0], RWS_V, default = 4'b0000 
       Opcode(inbound BL ring) logged at event of poison reception/unsupport 
       opcode/misaligned CFG access. 
     */
    UINT32 rtid : 11;
    /* rtid - Bits[14:4], RWS_V, default = 11'b00000000000 
       RTID(inbound BL ring) logged at event of poison reception/unsupport 
       opcode/misaligned CFG access. 
     */
    UINT32 rnid : 5;
    /* rnid - Bits[19:15], RWS_V, default = 5'b00000 
       RNID(inbound BL ring) logged at event of poison reception/unsupport 
       opcode/misaligned CFG access. 
     */
    UINT32 address_11_0 : 12;
    /* address_11_0 - Bits[31:20], RWS_V, default = 12'h000 
       address[11:0](inbound BL ring) logged at event of poison reception/unsupport 
       opcode/misaligned CFG access. 
     */
  } Bits;
  UINT32 Data;
} UBOXERRMISC_UBOX_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* UBOXERRMISC2_UBOX_CFG_REG supported on:                                    */
/*       IVT_EP (0x40158070)                                                  */
/*       IVT_EX (0x40158070)                                                  */
/*       HSX (0x40185070)                                                     */
/*       BDX (0x40185070)                                                     */
/* Register default value:              0x00000000                            */
#define UBOXERRMISC2_UBOX_CFG_REG 0x08004070


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.5.CFG.xml.
 * Logging of additional error information. 
 */
typedef union {
  struct {
    UINT32 address_43_12 : 32;
    /* address_43_12 - Bits[31:0], RWS_V, default = 32'h00000000 
       address[43:12](inbound BL ring) logged at event of poison reception/unsupport 
       opcode/misaligned CFG access. 
     */
  } Bits;
  UINT32 Data;
} UBOXERRMISC2_UBOX_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* BIOSSTICKYLOCKBYPASSSCRATCHPAD0_UBOX_CFG_REG supported on:                 */
/*       IVT_EP (0x40158074)                                                  */
/*       IVT_EX (0x40158074)                                                  */
/*       HSX (0x40185074)                                                     */
/*       BDX (0x40185074)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSSTICKYLOCKBYPASSSCRATCHPAD0_UBOX_CFG_REG 0x08004074
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * BIOS SMM handler modifies the register contents during CPU HotAdd_Sparing RAS 
 * events. 
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RWS_LB, default = 32'b00000000000000000000000000000000 
       Usage - checkpoints to see where in the process the new socket is at.
     */
  } Bits;
  UINT32 Data;
} BIOSSTICKYLOCKBYPASSSCRATCHPAD0_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* BIOSSTICKYLOCKBYPASSSCRATCHPAD1_UBOX_CFG_REG supported on:                 */
/*       IVT_EP (0x40158078)                                                  */
/*       IVT_EX (0x40158078)                                                  */
/*       HSX (0x40185078)                                                     */
/*       BDX (0x40185078)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSSTICKYLOCKBYPASSSCRATCHPAD1_UBOX_CFG_REG 0x08004078
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * BIOS SMM handler modifies the register contents during CPU HotAdd_Sparing RAS 
 * events. 
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RWS_LB, default = 32'b00000000000000000000000000000000 
       Usage - checkpoints to see where in the process the new socket is at.
     */
  } Bits;
  UINT32 Data;
} BIOSSTICKYLOCKBYPASSSCRATCHPAD1_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* BIOSSTICKYLOCKBYPASSSCRATCHPAD2_UBOX_CFG_REG supported on:                 */
/*       IVT_EP (0x4015807C)                                                  */
/*       IVT_EX (0x4015807C)                                                  */
/*       HSX (0x4018507C)                                                     */
/*       BDX (0x4018507C)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSSTICKYLOCKBYPASSSCRATCHPAD2_UBOX_CFG_REG 0x0800407C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * BIOS SMM handler modifies the register contents during CPU HotAdd_Sparing RAS 
 * events. 
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RWS_LB, default = 32'b00000000000000000000000000000000 
       Usage - checkpoints to see where in the process the new socket is at.
     */
  } Bits;
  UINT32 Data;
} BIOSSTICKYLOCKBYPASSSCRATCHPAD2_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* BIOSSTICKYLOCKBYPASSSCRATCHPAD3_UBOX_CFG_REG supported on:                 */
/*       IVT_EP (0x40158080)                                                  */
/*       IVT_EX (0x40158080)                                                  */
/*       HSX (0x40185080)                                                     */
/*       BDX (0x40185080)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSSTICKYLOCKBYPASSSCRATCHPAD3_UBOX_CFG_REG 0x08004080
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * BIOS SMM handler modifies the register contents during CPU HotAdd_Sparing RAS 
 * events. 
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RWS_LB, default = 32'b00000000000000000000000000000000 
       Usage - checkpoints to see where in the process the new socket is at.
     */
  } Bits;
  UINT32 Data;
} BIOSSTICKYLOCKBYPASSSCRATCHPAD3_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* BIOSSTICKYLOCKBYPASSSCRATCHPAD4_UBOX_CFG_REG supported on:                 */
/*       IVT_EP (0x40158084)                                                  */
/*       IVT_EX (0x40158084)                                                  */
/*       HSX (0x40185084)                                                     */
/*       BDX (0x40185084)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSSTICKYLOCKBYPASSSCRATCHPAD4_UBOX_CFG_REG 0x08004084
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * BIOS SMM handler modifies the register contents during CPU HotAdd_Sparing RAS 
 * events. 
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RWS_LB, default = 32'b00000000000000000000000000000000 
       Usage - checkpoints to see where in the process the new socket is at.
     */
  } Bits;
  UINT32 Data;
} BIOSSTICKYLOCKBYPASSSCRATCHPAD4_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* BIOSSTICKYLOCKBYPASSSCRATCHPAD5_UBOX_CFG_REG supported on:                 */
/*       IVT_EP (0x40158088)                                                  */
/*       IVT_EX (0x40158088)                                                  */
/*       HSX (0x40185088)                                                     */
/*       BDX (0x40185088)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSSTICKYLOCKBYPASSSCRATCHPAD5_UBOX_CFG_REG 0x08004088
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * BIOS SMM handler modifies the register contents during CPU HotAdd_Sparing RAS 
 * events. 
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RWS_LB, default = 32'b00000000000000000000000000000000 
       Usage - checkpoints to see where in the process the new socket is at.
     */
  } Bits;
  UINT32 Data;
} BIOSSTICKYLOCKBYPASSSCRATCHPAD5_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* BIOSSTICKYLOCKBYPASSSCRATCHPAD6_UBOX_CFG_REG supported on:                 */
/*       IVT_EP (0x4015808C)                                                  */
/*       IVT_EX (0x4015808C)                                                  */
/*       HSX (0x4018508C)                                                     */
/*       BDX (0x4018508C)                                                     */
/* Register default value:              0x00000000                            */
#define BIOSSTICKYLOCKBYPASSSCRATCHPAD6_UBOX_CFG_REG 0x0800408C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * BIOS SMM handler modifies the register contents during CPU HotAdd_Sparing RAS 
 * events. 
 */
typedef union {
  struct {
    UINT32 data : 32;
    /* data - Bits[31:0], RWS_LB, default = 32'b00000000000000000000000000000000 
       Usage - checkpoints to see where in the process the new socket is at.
     */
  } Bits;
  UINT32 Data;
} BIOSSTICKYLOCKBYPASSSCRATCHPAD6_UBOX_CFG_STRUCT;
#endif /* ASM_INC */


/* UBOX_GL_ERR_CFG_UBOX_CFG_REG supported on:                                 */
/*       IVT_EP (0x40158090)                                                  */
/*       IVT_EX (0x40158090)                                                  */
/*       HSX (0x40185090)                                                     */
/*       BDX (0x40185090)                                                     */
/* Register default value on IVT_EP:    0x003FFFF0                            */
/* Register default value on IVT_EX:    0x003FFFF0                            */
/* Register default value on HSX:       0x03555570                            */
/* Register default value on BDX:       0x03555570                            */
#define UBOX_GL_ERR_CFG_UBOX_CFG_REG 0x08004090


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.5.CFG.xml.
 * This is the viral/global fatal error config/status register in the Ubox. The 
 * mask bits in UBOXErrCtl_CFG are set, the mask bits of this register are don't 
 * care and no viral/fatal error are signaled. 
 */
typedef union {
  struct {
    UINT32 statusuboxfatalerror : 1;
    /* statusuboxfatalerror - Bits[0:0], RO_V, default = 1'b0 
       Read only global fatal error status in UBox (UMC or IERR)
     */
    UINT32 statusuboxviral : 1;
    /* statusuboxviral - Bits[1:1], RO_V, default = 1'b0 
       Read only global viralr status in UBox
     */
    UINT32 resetuboxfatalerror : 1;
    /* resetuboxfatalerror - Bits[2:2], RW, default = 1'b0 
       Set 1 to force clear Ubox fatal error status.
     */
    UINT32 resetuboxviral : 1;
    /* resetuboxviral - Bits[3:3], RW, default = 1'b0 
       Set 1 to force clear Ubox viral status.
     */
    UINT32 maskuboxumc : 1;
    /* maskuboxumc - Bits[4:4], RW, default = 1'b1 
       Set 1 to block fatal error status updated by UMC message received in UBox.
     */
    UINT32 maskuboxierr : 1;
    /* maskuboxierr - Bits[5:5], RW, default = 1'b1 
       Set 1 to block fatal error status updated by IERR message received in UBox.
     */
    UINT32 maskunsupportopcode : 2;
    /* maskunsupportopcode - Bits[7:6], RW, default = 2'b01 
       Set 1 to block viral[0], fatal[1] status updated by unsupported opcode received 
       in UBox. 
     */
    UINT32 maskpoisonreception : 2;
    /* maskpoisonreception - Bits[9:8], RW, default = 2'b01 
       Set 1 to block viral[0], fatal[1] status updated by poison received in UBox.
     */
    UINT32 maskcfgrdaddrmisalign : 2;
    /* maskcfgrdaddrmisalign - Bits[11:10], RW, default = 2'b01 
       Set 1 to block viral[0], fatal[1] status updated by CFG read address 
       misalignment in UBox. 
     */
    UINT32 maskcfgwraddrmisalign : 2;
    /* maskcfgwraddrmisalign - Bits[13:12], RW, default = 2'b01 
       Set 1 to block viral[0], fatal[1] status updated by CFG write address 
       misalignment in UBox. 
     */
    UINT32 masksmitimeout : 2;
    /* masksmitimeout - Bits[15:14], RW, default = 2'b01 
       Set 1 to block viral[0], fatal[1] status updated by SMI timeout in UBox.
     */
    UINT32 maskmasterlocktimeout : 2;
    /* maskmasterlocktimeout - Bits[17:16], RW, default = 2'b01 
       Set 1 to block viral[0], fatal[1] status updated by master lock timeout in UBox.
     */
    UINT32 maskpcuproxyierr : 2;
    /* maskpcuproxyierr - Bits[19:18], RW, default = 2'b01 
       Set 1 to block viral[0], fatal[1] status updated by PCU proxy of IERR in UBox.
     */
    UINT32 maskpcuproxyumc : 2;
    /* maskpcuproxyumc - Bits[21:20], RW, default = 2'b01 
       Set 1 to block viral[0], fatal[1] status updated by PCU proxy of UMC in UBox.
     */
    UINT32 maskpcuproxymsmi : 2;
    /* maskpcuproxymsmi - Bits[23:22], RW, default = 2'b01 
       Set 1 to block viral[0], fatal[1] status updated by PCU proxy of UMC in UBox.
     */
    UINT32 maskextviral : 1;
    /* maskextviral - Bits[24:24], RW, default = 1'b1 
       Set 1 to block viral status updated by CATERR pin assertion
     */
    UINT32 maskuboxmsmi : 1;
    /* maskuboxmsmi - Bits[25:25], RW, default = 1'b1 
       Set 1 to block fatal error status updated by MSMI message received in UBox.
     */
    UINT32 rsvd : 6;
    /* rsvd - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} UBOX_GL_ERR_CFG_UBOX_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* EVENTS_DEBUG_UBOX_CFG_REG supported on:                                    */
/*       IVT_EP (0x401580A0)                                                  */
/*       IVT_EX (0x401580A0)                                                  */
/*       HSX (0x401850A0)                                                     */
/*       BDX (0x401850A0)                                                     */
/* Register default value:              0x00000000                            */
#define EVENTS_DEBUG_UBOX_CFG_REG 0x080040A0


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.5.CFG.xml.
 * Event bus control
 */
typedef union {
  struct {
    UINT32 uxevnts_debug_bus_enable : 1;
    /* uxevnts_debug_bus_enable - Bits[0:0], RWS_L, default = 1'b0 
       enable debug bus functionality in uxevents fub
     */
    UINT32 debug_uxevnts_sel_lo : 5;
    /* debug_uxevnts_sel_lo - Bits[5:1], RWS_L, default = 5'b00000 
       Selects source of low byte of debug bus
       0 = output 0's
       others per MAS
     */
    UINT32 debug_uxevnts_sel_hi : 5;
    /* debug_uxevnts_sel_hi - Bits[10:6], RWS_L, default = 5'b00000 
       Selects source of high byte of debug bus
       0 = output 0's
       others per MAS
     */
    UINT32 debug_nra_sel_lo : 5;
    /* debug_nra_sel_lo - Bits[15:11], RWS_L, default = 5'b00000 
       Selects source of low byte of nra debug bus
       0 = output 0's
       others per MAS
     */
    UINT32 debug_nra_sel_hi : 5;
    /* debug_nra_sel_hi - Bits[20:16], RWS_L, default = 5'b00000 
       Selects source of high byte of nra debug bus
       0 = output 0's
       others per MAS
     */
    UINT32 debug_nrspblcmd_sel_lo : 4;
    /* debug_nrspblcmd_sel_lo - Bits[24:21], RWS_L, default = 4'b0000 
       Selects source of low byte of nrspblcmd debug bus
       0 = output 0's
       others per MAS
     */
    UINT32 debug_nrspblcmd_sel_hi : 4;
    /* debug_nrspblcmd_sel_hi - Bits[28:25], RWS_L, default = 4'b0000 
       Selects source of high byte of nrspblcmd debug bus
       0 = output 0's
       others per MAS
     */
    UINT32 rsvd : 3;
    /* rsvd - Bits[31:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} EVENTS_DEBUG_UBOX_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* IERRLOGGINGREG_UBOX_CFG_REG supported on:                                  */
/*       HSX (0x401850A4)                                                     */
/*       BDX (0x401850A4)                                                     */
/* Register default value:              0x00000000                            */
#define IERRLOGGINGREG_UBOX_CFG_REG 0x080040A4
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.5.CFG.xml.
 *  IERR first/second logging error.
 */
typedef union {
  struct {
    UINT32 firstierrsrcid : 8;
    /* firstierrsrcid - Bits[7:0], RWS_V, default = 8'b00000000 
       
               msgCh portID of the end point with the first IERR.
        If FirstIerrSrcValid is 1, the block responsible for generating the first IERR 
       is decoded as follows: 
       	01000100 - PCU 
       	10000000 - Core 0
       	10000100 - Core 1
       	10001000 - Core 2
       	10001100 - Core 3
       	10010000 - Core 4
       	10010100 - Core 5
       	10011000 - Core 6
       	10011100 - Core 7
       	10100000 - Core 8
       	10100100 - Core 9
       	10101000 - Core 10
       	10101100 - Core 11
       	10110000 - Core 12
       	10110100 - Core 13
       	10111000 - Core 14
       	10111100 - Core 15
       	11000000 - Core 16
       	11000100 - Core 17
               
     */
    UINT32 firstierrsrcvalid : 1;
    /* firstierrsrcvalid - Bits[8:8], RWS_V, default = 1'b0 
        set to 1 if the FirstSrcID is valid
     */
    UINT32 firstierrsrcfromcbo : 1;
    /* firstierrsrcfromcbo - Bits[9:9], RWS_V, default = 1'b0 
       Set to 1 of the FirstIerrSrcID is from a Cbo or core.
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 secondierrsrcid : 8;
    /* secondierrsrcid - Bits[23:16], RWS_V, default = 8'b00000000 
       If SecondIerrSrcValid is 1, the block responsible for generating the second IERR 
       is logged here.  Refer to decode table in FirstIerrSrcID. 
     */
    UINT32 secondierrsrcvalid : 1;
    /* secondierrsrcvalid - Bits[24:24], RWS_V, default = 1'b0 
        set to 1 if the SecondIerrSrcID is valid
     */
    UINT32 secondierrsrcfromcbo : 1;
    /* secondierrsrcfromcbo - Bits[25:25], RWS_V, default = 1'b0 
       Set to 1 if the second SrcID is from a Cbo or core.
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} IERRLOGGINGREG_UBOX_CFG_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 *  IERR first/second logging error.
 */
typedef union {
  struct {
    UINT32 firstierrsrcid : 8;
    /* firstierrsrcid - Bits[7:0], RWS_V, default = 8'b00000000 
       
               msgCh portID of the end point with the first IERR.
        If FirstIerrSrcValid is 1, the block responsible for generating the first IERR 
       is decoded as follows: 
       	01000100 - PCU 
       	10000000 - Core 0
       	10000100 - Core 1
       	10001000 - Core 2
       	10001100 - Core 3
       	10010000 - Core 4
       	10010100 - Core 5
       	10011000 - Core 6
       	10011100 - Core 7
       	10100000 - Core 8
       	10100100 - Core 9
       	10101000 - Core 10
       	10101100 - Core 11
       	10110000 - Core 12
       	10110100 - Core 13
       	10111000 - Core 14
       	10111100 - Core 15
       	11000000 - Core 16
       	11000100 - Core 17
               
     */
    UINT32 firstierrsrcvalid : 1;
    /* firstierrsrcvalid - Bits[8:8], RWS_V, default = 1'b0 
        set to 1 if the FirstSrcID is valid
     */
    UINT32 firstierrsrcfromcbo : 1;
    /* firstierrsrcfromcbo - Bits[9:9], RWS_V, default = 1'b0 
       Set to 1 of the FirstIerrSrcID is from a Cbo or core.
     */
    UINT32 firstierrindex : 2;
    /* firstierrindex - Bits[11:10], RWS_V, default = 2'b0 
       Log 00 if it is the first  error happened across IERR and MCERR.
                            Log 01 if it is the second error happened across IERR and 
       MCERR.             
                            Log 10 if it is the third error happened across IERR and 
       MCERR.             
                            Log 11 if it is the fourth error happened across IERR and 
       MCERR.             
               
     */
    UINT32 rsvd_12 : 4;
    /* rsvd_12 - Bits[15:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 secondierrsrcid : 8;
    /* secondierrsrcid - Bits[23:16], RWS_V, default = 8'b00000000 
       If SecondIerrSrcValid is 1, the block responsible for generating the second IERR 
       is logged here.  Refer to decode table in FirstIerrSrcID. 
     */
    UINT32 secondierrsrcvalid : 1;
    /* secondierrsrcvalid - Bits[24:24], RWS_V, default = 1'b0 
        set to 1 if the SecondIerrSrcID is valid
     */
    UINT32 secondierrsrcfromcbo : 1;
    /* secondierrsrcfromcbo - Bits[25:25], RWS_V, default = 1'b0 
       Set to 1 if the second SrcID is from a Cbo or core.
     */
    UINT32 secondierrindex : 2;
    /* secondierrindex - Bits[27:26], RWS_V, default = 2'b0 
       Log 00 if it is the first  error happened across IERR and MCERR.
                            Log 01 if it is the second error happened across IERR and 
       MCERR.             
                            Log 10 if it is the third error happened across IERR and 
       MCERR.             
                            Log 11 if it is the fourth error happened across IERR and 
       MCERR.             
               
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} IERRLOGGINGREG_UBOX_CFG_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* MCERRLOGGINGREG_UBOX_CFG_REG supported on:                                 */
/*       HSX (0x401850A8)                                                     */
/*       BDX (0x401850A8)                                                     */
/* Register default value:              0x00000000                            */
#define MCERRLOGGINGREG_UBOX_CFG_REG 0x080040A8
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.5.CFG.xml.
 *  MCERR first/second logging error.
 */
typedef union {
  struct {
    UINT32 firstmcerrsrcid : 8;
    /* firstmcerrsrcid - Bits[7:0], RWS_V, default = 8'b00000000 
       msgCh portID of the EP with the first MCERR.
        If FirstMCerrSrcValid is 1, the block responsible for generating the first 
       MCERR is decoded as follows: 
       	00000001 - IMC 0 CH0  
       	00000101 - IMC 0 CH1
       	00000110 - IMC 0 both CH0\CH1
       	00001001 - IMC 0 CH2
       	00001101 - IMC 0 CH3
       	00001110 - IMC both CH2\CH3
       	00010001 - IMC 1 CH0
       	00010101 - IMC 1 CH1
       	00010110 - IMC 1 both CH0\CH1
       	00010111 - IMC 1 both CH2\CH3
       	00011001 - IMC 1 CH2
       	00011101 - IMC 1 CH3
       	00111100 - Home Agent 0
       	00111110 - Home Agent 1
       	01000100 - PCU
       	01110000 - Intel QPI 0
       	01110001 - Intel QPI 1
       	01110011 - Intel QPI 2
       	01111110 - IIO
       	10000000 - CORE OR Cbo 0 
       	10000100 - CORE OR Cbo 1 
       	10001000 - CORE OR Cbo 2 
       	10001100 - CORE OR Cbo 3 
       	10010000 - CORE OR Cbo 4 
       	10010100 - CORE OR Cbo 5 
       	10011000 - CORE OR Cbo 6 
       	10011100 - CORE OR Cbo 7 
       	10100000 - CORE OR Cbo 8 
       	10100100 - CORE OR Cbo 9 
       	10101000 - CORE OR Cbo 10 
       	10101100 - CORE OR Cbo 11 
       	10110000 - CORE OR Cbo 12 
       	10110100 - CORE OR Cbo 13 
       	10111000 - CORE OR Cbo 14 
       	10111100 - CORE OR Cbo 15 
       	11000000 - CORE OR Cbo 16 
       	11000100 - CORE OR Cbo 17
        	
     */
    UINT32 firstmcerrsrcvalid : 1;
    /* firstmcerrsrcvalid - Bits[8:8], RWS_V, default = 1'b0 
       Set to 1 if the FirstSrcID is valid.
     */
    UINT32 firstmcerrsrcfromcbo : 1;
    /* firstmcerrsrcfromcbo - Bits[9:9], RWS_V, default = 1'b0 
       Set to 1 of the FirstMCerrSrcID is from a Cbo or CORE. 
     */
    UINT32 rsvd_10 : 6;
    /* rsvd_10 - Bits[15:10], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 secondmcerrsrcid : 8;
    /* secondmcerrsrcid - Bits[23:16], RWS_V, default = 8'b00000000 
       If SecondMCerrSrcValid is 1, the block responsible for generating the second 
       MCERR is logged here. Refer to decode table in FirstMCerrSrcID. 
     */
    UINT32 secondmcerrsrcvalid : 1;
    /* secondmcerrsrcvalid - Bits[24:24], RWS_V, default = 1'b0 
       Set to 1 if the SecondMCerrSrcID is valid.
     */
    UINT32 secondmcerrsrcfromcbo : 1;
    /* secondmcerrsrcfromcbo - Bits[25:25], RWS_V, default = 1'b0 
       Set to 1 if the second SrcID is from a Cbo or CORE. 
     */
    UINT32 rsvd_26 : 6;
    /* rsvd_26 - Bits[31:26], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MCERRLOGGINGREG_UBOX_CFG_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 *  MCERR first/second logging error.
 */
typedef union {
  struct {
    UINT32 firstmcerrsrcid : 8;
    /* firstmcerrsrcid - Bits[7:0], RWS_V, default = 8'b00000000 
       msgCh portID of the EP with the first MCERR.
        If FirstMCerrSrcValid is 1, the block responsible for generating the first 
       MCERR is decoded as follows: 
       	00000001 - IMC 0 CH0  
       	00000101 - IMC 0 CH1
       	00000110 - IMC 0 both CH0\CH1
       	00001001 - IMC 0 CH2
       	00001101 - IMC 0 CH3
       	00001110 - IMC both CH2\CH3
       	00010001 - IMC 1 CH0
       	00010101 - IMC 1 CH1
       	00010110 - IMC 1 both CH0\CH1
       	00010111 - IMC 1 both CH2\CH3
       	00011001 - IMC 1 CH2
       	00011101 - IMC 1 CH3
       	00111100 - Home Agent 0
       	00111110 - Home Agent 1
       	01000100 - PCU
       	01110000 - Intel QPI 0
       	01110001 - Intel QPI 1
       	01110011 - Intel QPI 2
       	01111110 - IIO
       	10000000 - CORE OR Cbo 0 
       	10000100 - CORE OR Cbo 1 
       	10001000 - CORE OR Cbo 2 
       	10001100 - CORE OR Cbo 3 
       	10010000 - CORE OR Cbo 4 
       	10010100 - CORE OR Cbo 5 
       	10011000 - CORE OR Cbo 6 
       	10011100 - CORE OR Cbo 7 
       	10100000 - CORE OR Cbo 8 
       	10100100 - CORE OR Cbo 9 
       	10101000 - CORE OR Cbo 10 
       	10101100 - CORE OR Cbo 11 
       	10110000 - CORE OR Cbo 12 
       	10110100 - CORE OR Cbo 13 
       	10111000 - CORE OR Cbo 14 
       	10111100 - CORE OR Cbo 15 
       	11000000 - CORE OR Cbo 16 
       	11000100 - CORE OR Cbo 17
        	
     */
    UINT32 firstmcerrsrcvalid : 1;
    /* firstmcerrsrcvalid - Bits[8:8], RWS_V, default = 1'b0 
       Set to 1 if the FirstSrcID is valid.
     */
    UINT32 firstmcerrsrcfromcbo : 1;
    /* firstmcerrsrcfromcbo - Bits[9:9], RWS_V, default = 1'b0 
       Set to 1 of the FirstMCerrSrcID is from a Cbo or CORE. 
     */
    UINT32 firstmcerrindex : 2;
    /* firstmcerrindex - Bits[11:10], RWS_V, default = 2'b0 
       Log 00 if it is the first  error happened across IERR and MCERR.
                            Log 01 if it is the second error happened across IERR and 
       MCERR.             
                            Log 10 if it is the third error happened across IERR and 
       MCERR.             
                            Log 11 if it is the fourth error happened across IERR and 
       MCERR.             
               
     */
    UINT32 rsvd_12 : 4;
    /* rsvd_12 - Bits[15:12], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 secondmcerrsrcid : 8;
    /* secondmcerrsrcid - Bits[23:16], RWS_V, default = 8'b00000000 
       If SecondMCerrSrcValid is 1, the block responsible for generating the second 
       MCERR is logged here. Refer to decode table in FirstMCerrSrcID. 
     */
    UINT32 secondmcerrsrcvalid : 1;
    /* secondmcerrsrcvalid - Bits[24:24], RWS_V, default = 1'b0 
       Set to 1 if the SecondMCerrSrcID is valid.
     */
    UINT32 secondmcerrsrcfromcbo : 1;
    /* secondmcerrsrcfromcbo - Bits[25:25], RWS_V, default = 1'b0 
       Set to 1 if the second SrcID is from a Cbo or CORE. 
     */
    UINT32 secondmcerrindex : 2;
    /* secondmcerrindex - Bits[27:26], RWS_V, default = 2'b0 
       Log 00 if it is the first  error happened across IERR and MCERR.
                            Log 01 if it is the second error happened across IERR and 
       MCERR.             
                            Log 10 if it is the third error happened across IERR and 
       MCERR.             
                            Log 11 if it is the fourth error happened across IERR and 
       MCERR.             
               
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MCERRLOGGINGREG_UBOX_CFG_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */





/* EMCA_CORE_CSMI_LOG_UBOX_CFG_REG supported on:                              */
/*       HSX (0x401850B0)                                                     */
/*       BDX (0x401850B0)                                                     */
/* Register default value:              0x00000000                            */
#define EMCA_CORE_CSMI_LOG_UBOX_CFG_REG 0x080040B0
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.5.CFG.xml.
 * This is a log of which cores have signalled a CSMI to ubox via the MCLK message. 
 * Enabled on the B0 and later stepping of the processor. 
 */
typedef union {
  struct {
    UINT32 core0 : 1;
    /* core0 - Bits[0:0], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 0
               
     */
    UINT32 core1 : 1;
    /* core1 - Bits[1:1], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 1
               
     */
    UINT32 core2 : 1;
    /* core2 - Bits[2:2], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 2
               
     */
    UINT32 core3 : 1;
    /* core3 - Bits[3:3], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 3
               
     */
    UINT32 core4 : 1;
    /* core4 - Bits[4:4], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 4
               
     */
    UINT32 core5 : 1;
    /* core5 - Bits[5:5], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 5
               
     */
    UINT32 core6 : 1;
    /* core6 - Bits[6:6], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 6
               
     */
    UINT32 core7 : 1;
    /* core7 - Bits[7:7], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 7
               
     */
    UINT32 core8 : 1;
    /* core8 - Bits[8:8], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 8
               
     */
    UINT32 core9 : 1;
    /* core9 - Bits[9:9], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 9
               
     */
    UINT32 core10 : 1;
    /* core10 - Bits[10:10], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 10
               
     */
    UINT32 core11 : 1;
    /* core11 - Bits[11:11], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 11
               
     */
    UINT32 core12 : 1;
    /* core12 - Bits[12:12], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 12
               
     */
    UINT32 core13 : 1;
    /* core13 - Bits[13:13], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 13
               
     */
    UINT32 core14 : 1;
    /* core14 - Bits[14:14], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 14
               
     */
    UINT32 core15 : 1;
    /* core15 - Bits[15:15], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 15
               
     */
    UINT32 core16 : 1;
    /* core16 - Bits[16:16], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 16
               
     */
    UINT32 core17 : 1;
    /* core17 - Bits[17:17], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 17
               
     */
    UINT32 rsvd : 14;
    /* rsvd - Bits[31:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} EMCA_CORE_CSMI_LOG_UBOX_CFG_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * This is a log of which cores have signalled a CSMI to ubox via the MCLK message. 
 * Enabled on the B0 and later stepping of the processor. 
 */
typedef union {
  struct {
    UINT32 core0 : 1;
    /* core0 - Bits[0:0], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 0
               
     */
    UINT32 core1 : 1;
    /* core1 - Bits[1:1], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 1
               
     */
    UINT32 core2 : 1;
    /* core2 - Bits[2:2], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 2
               
     */
    UINT32 core3 : 1;
    /* core3 - Bits[3:3], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 3
               
     */
    UINT32 core4 : 1;
    /* core4 - Bits[4:4], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 4
               
     */
    UINT32 core5 : 1;
    /* core5 - Bits[5:5], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 5
               
     */
    UINT32 core6 : 1;
    /* core6 - Bits[6:6], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 6
               
     */
    UINT32 core7 : 1;
    /* core7 - Bits[7:7], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 7
               
     */
    UINT32 core8 : 1;
    /* core8 - Bits[8:8], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 8
               
     */
    UINT32 core9 : 1;
    /* core9 - Bits[9:9], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 9
               
     */
    UINT32 core10 : 1;
    /* core10 - Bits[10:10], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 10
               
     */
    UINT32 core11 : 1;
    /* core11 - Bits[11:11], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 11
               
     */
    UINT32 core12 : 1;
    /* core12 - Bits[12:12], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 12
               
     */
    UINT32 core13 : 1;
    /* core13 - Bits[13:13], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 13
               
     */
    UINT32 core14 : 1;
    /* core14 - Bits[14:14], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 14
               
     */
    UINT32 core15 : 1;
    /* core15 - Bits[15:15], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 15
               
     */
    UINT32 core16 : 1;
    /* core16 - Bits[16:16], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 16
               
     */
    UINT32 core17 : 1;
    /* core17 - Bits[17:17], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 17
               
     */
    UINT32 core18 : 1;
    /* core18 - Bits[18:18], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 18
               
     */
    UINT32 core19 : 1;
    /* core19 - Bits[19:19], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 19
               
     */
    UINT32 core20 : 1;
    /* core20 - Bits[20:20], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 20
               
     */
    UINT32 core21 : 1;
    /* core21 - Bits[21:21], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 21
               
     */
    UINT32 core22 : 1;
    /* core22 - Bits[22:22], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 22
               
     */
    UINT32 core23 : 1;
    /* core23 - Bits[23:23], RW1C, default = 1'b0 
       
                 CSMI received indicator for core 23
               
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} EMCA_CORE_CSMI_LOG_UBOX_CFG_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* EMCA_CORE_MSMI_LOG_UBOX_CFG_REG supported on:                              */
/*       HSX (0x401850B4)                                                     */
/*       BDX (0x401850B4)                                                     */
/* Register default value:              0x00000000                            */
#define EMCA_CORE_MSMI_LOG_UBOX_CFG_REG 0x080040B4
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.5.CFG.xml.
 * This is a log of which cores have signalled an MSMI to ubox via the MCLK 
 * message. Enabled on the B0 and later stepping of the processor. 
 */
typedef union {
  struct {
    UINT32 core0 : 1;
    /* core0 - Bits[0:0], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 0
               
     */
    UINT32 core1 : 1;
    /* core1 - Bits[1:1], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 1
               
     */
    UINT32 core2 : 1;
    /* core2 - Bits[2:2], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 2
               
     */
    UINT32 core3 : 1;
    /* core3 - Bits[3:3], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 3
               
     */
    UINT32 core4 : 1;
    /* core4 - Bits[4:4], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 4
               
     */
    UINT32 core5 : 1;
    /* core5 - Bits[5:5], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 5
               
     */
    UINT32 core6 : 1;
    /* core6 - Bits[6:6], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 6
               
     */
    UINT32 core7 : 1;
    /* core7 - Bits[7:7], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 7
               
     */
    UINT32 core8 : 1;
    /* core8 - Bits[8:8], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 8
               
     */
    UINT32 core9 : 1;
    /* core9 - Bits[9:9], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 9
               
     */
    UINT32 core10 : 1;
    /* core10 - Bits[10:10], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 10
               
     */
    UINT32 core11 : 1;
    /* core11 - Bits[11:11], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 11
               
     */
    UINT32 core12 : 1;
    /* core12 - Bits[12:12], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 12
               
     */
    UINT32 core13 : 1;
    /* core13 - Bits[13:13], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 13
               
     */
    UINT32 core14 : 1;
    /* core14 - Bits[14:14], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 14
               
     */
    UINT32 core15 : 1;
    /* core15 - Bits[15:15], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 15
               
     */
    UINT32 core16 : 1;
    /* core16 - Bits[16:16], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 16
               
     */
    UINT32 core17 : 1;
    /* core17 - Bits[17:17], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 17
               
     */
    UINT32 rsvd : 14;
    /* rsvd - Bits[31:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} EMCA_CORE_MSMI_LOG_UBOX_CFG_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.5.CFG.xml.
 * This is a log of which cores have signalled an MSMI to ubox via the MCLK 
 * message. Enabled on the B0 and later stepping of the processor. 
 */
typedef union {
  struct {
    UINT32 core0 : 1;
    /* core0 - Bits[0:0], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 0
               
     */
    UINT32 core1 : 1;
    /* core1 - Bits[1:1], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 1
               
     */
    UINT32 core2 : 1;
    /* core2 - Bits[2:2], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 2
               
     */
    UINT32 core3 : 1;
    /* core3 - Bits[3:3], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 3
               
     */
    UINT32 core4 : 1;
    /* core4 - Bits[4:4], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 4
               
     */
    UINT32 core5 : 1;
    /* core5 - Bits[5:5], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 5
               
     */
    UINT32 core6 : 1;
    /* core6 - Bits[6:6], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 6
               
     */
    UINT32 core7 : 1;
    /* core7 - Bits[7:7], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 7
               
     */
    UINT32 core8 : 1;
    /* core8 - Bits[8:8], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 8
               
     */
    UINT32 core9 : 1;
    /* core9 - Bits[9:9], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 9
               
     */
    UINT32 core10 : 1;
    /* core10 - Bits[10:10], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 10
               
     */
    UINT32 core11 : 1;
    /* core11 - Bits[11:11], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 11
               
     */
    UINT32 core12 : 1;
    /* core12 - Bits[12:12], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 12
               
     */
    UINT32 core13 : 1;
    /* core13 - Bits[13:13], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 13
               
     */
    UINT32 core14 : 1;
    /* core14 - Bits[14:14], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 14
               
     */
    UINT32 core15 : 1;
    /* core15 - Bits[15:15], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 15
               
     */
    UINT32 core16 : 1;
    /* core16 - Bits[16:16], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 16
               
     */
    UINT32 core17 : 1;
    /* core17 - Bits[17:17], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 17
               
     */
    UINT32 core18 : 1;
    /* core18 - Bits[18:18], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 18
               
     */
    UINT32 core19 : 1;
    /* core19 - Bits[19:19], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 19
               
     */
    UINT32 core20 : 1;
    /* core20 - Bits[20:20], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 20
               
     */
    UINT32 core21 : 1;
    /* core21 - Bits[21:21], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 21
               
     */
    UINT32 core22 : 1;
    /* core22 - Bits[22:22], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 22
               
     */
    UINT32 core23 : 1;
    /* core23 - Bits[23:23], RW1C, default = 1'b0 
       
                 MSMI received indicator for core 23
               
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} EMCA_CORE_MSMI_LOG_UBOX_CFG_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



#endif /* UBOX_CFG_h */
