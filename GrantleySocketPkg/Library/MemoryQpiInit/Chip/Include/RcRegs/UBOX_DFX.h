/* Date Stamp: 8/23/2014 */

#ifndef UBOX_DFX_h
#define UBOX_DFX_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* UBOX_DFX_IVT_DEV 11                                                        */
/* UBOX_DFX_IVT_FUN 2                                                         */
/* For HSX_HOST:                                                              */
/* UBOX_DFX_HSX_DEV 16                                                        */
/* UBOX_DFX_HSX_FUN 6                                                         */
/* For BDX_HOST:                                                              */
/* UBOX_DFX_BDX_DEV 16                                                        */
/* UBOX_DFX_BDX_FUN 6                                                         */

/* VID_UBOX_DFX_REG supported on:                                             */
/*       IVT_EP (0x2015A000)                                                  */
/*       IVT_EX (0x2015A000)                                                  */
/*       HSX (0x20186000)                                                     */
/*       BDX (0x20186000)                                                     */
/* Register default value:              0x8086                                */
#define VID_UBOX_DFX_REG 0x08012000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
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
} VID_UBOX_DFX_STRUCT;
#endif /* ASM_INC */


/* DID_UBOX_DFX_REG supported on:                                             */
/*       IVT_EP (0x2015A002)                                                  */
/*       IVT_EX (0x2015A002)                                                  */
/*       HSX (0x20186002)                                                     */
/*       BDX (0x20186002)                                                     */
/* Register default value on IVT_EP:    0x0E7D                                */
/* Register default value on IVT_EX:    0x0E7D                                */
/* Register default value on HSX:       0x2F7D                                */
/* Register default value on BDX:       0x6F7D                                */
#define DID_UBOX_DFX_REG 0x08012002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6F7D 
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
} DID_UBOX_DFX_STRUCT;
#endif /* ASM_INC */


/* PCICMD_UBOX_DFX_REG supported on:                                          */
/*       IVT_EP (0x2015A004)                                                  */
/*       IVT_EX (0x2015A004)                                                  */
/*       HSX (0x20186004)                                                     */
/*       BDX (0x20186004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_UBOX_DFX_REG 0x08012004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
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
} PCICMD_UBOX_DFX_STRUCT;
#endif /* ASM_INC */


/* PCISTS_UBOX_DFX_REG supported on:                                          */
/*       IVT_EP (0x2015A006)                                                  */
/*       IVT_EX (0x2015A006)                                                  */
/*       HSX (0x20186006)                                                     */
/*       BDX (0x20186006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_UBOX_DFX_REG 0x08012006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.6.CFG.xml.
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
} PCISTS_UBOX_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_UBOX_DFX_REG supported on:                                             */
/*       IVT_EP (0x1015A008)                                                  */
/*       IVT_EX (0x1015A008)                                                  */
/*       HSX (0x10186008)                                                     */
/*       BDX (0x10186008)                                                     */
/* Register default value:              0x00                                  */
#define RID_UBOX_DFX_REG 0x08011008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
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
} RID_UBOX_DFX_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_UBOX_DFX_REG supported on:                                          */
/*       IVT_EP (0x1015A009)                                                  */
/*       IVT_EX (0x1015A009)                                                  */
/*       HSX (0x10186009)                                                     */
/*       BDX (0x10186009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_UBOX_DFX_REG 0x08011009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.6.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_UBOX_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_UBOX_DFX_REG supported on:                                          */
/*       IVT_EP (0x2015A00A)                                                  */
/*       IVT_EX (0x2015A00A)                                                  */
/*       HSX (0x2018600A)                                                     */
/*       BDX (0x2018600A)                                                     */
/* Register default value on IVT_EP:    0x0880                                */
/* Register default value on IVT_EX:    0x0880                                */
/* Register default value on HSX:       0x1101                                */
/* Register default value on BDX:       0x1101                                */
#define CCR_N1_UBOX_DFX_REG 0x0801200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
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
} CCR_N1_UBOX_DFX_STRUCT;
#endif /* ASM_INC */


/* CLSR_UBOX_DFX_REG supported on:                                            */
/*       IVT_EP (0x1015A00C)                                                  */
/*       IVT_EX (0x1015A00C)                                                  */
/*       HSX (0x1018600C)                                                     */
/*       BDX (0x1018600C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_UBOX_DFX_REG 0x0801100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
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
} CLSR_UBOX_DFX_STRUCT;
#endif /* ASM_INC */


/* PLAT_UBOX_DFX_REG supported on:                                            */
/*       IVT_EP (0x1015A00D)                                                  */
/*       IVT_EX (0x1015A00D)                                                  */
/*       HSX (0x1018600D)                                                     */
/*       BDX (0x1018600D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_UBOX_DFX_REG 0x0801100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
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
} PLAT_UBOX_DFX_STRUCT;
#endif /* ASM_INC */


/* HDR_UBOX_DFX_REG supported on:                                             */
/*       IVT_EP (0x1015A00E)                                                  */
/*       IVT_EX (0x1015A00E)                                                  */
/*       HSX (0x1018600E)                                                     */
/*       BDX (0x1018600E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_UBOX_DFX_REG 0x0801100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
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
} HDR_UBOX_DFX_STRUCT;
#endif /* ASM_INC */


/* BIST_UBOX_DFX_REG supported on:                                            */
/*       IVT_EP (0x1015A00F)                                                  */
/*       IVT_EX (0x1015A00F)                                                  */
/*       HSX (0x1018600F)                                                     */
/*       BDX (0x1018600F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_UBOX_DFX_REG 0x0801100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
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
} BIST_UBOX_DFX_STRUCT;
#endif /* ASM_INC */


/* SVID_UBOX_DFX_REG supported on:                                            */
/*       IVT_EP (0x2015A02C)                                                  */
/*       IVT_EX (0x2015A02C)                                                  */
/*       HSX (0x2018602C)                                                     */
/*       BDX (0x2018602C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_UBOX_DFX_REG 0x0801202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
 */
typedef union {
  struct {
    UINT16 subsystem_vendor_identification_number : 16;
    /* subsystem_vendor_identification_number - Bits[15:0], RW_O, default = 16'b1000000010000110 
       The default value specifies Intel but can be set to any value once after reset.
     */
  } Bits;
  UINT16 Data;
} SVID_UBOX_DFX_STRUCT;
#endif /* ASM_INC */


/* SDID_UBOX_DFX_REG supported on:                                            */
/*       IVT_EP (0x2015A02E)                                                  */
/*       IVT_EX (0x2015A02E)                                                  */
/*       HSX (0x2018602E)                                                     */
/*       BDX (0x2018602E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_UBOX_DFX_REG 0x0801202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
 */
typedef union {
  struct {
    UINT16 subsystem_device_identification_number : 16;
    /* subsystem_device_identification_number - Bits[15:0], RW_O, default = 16'b0000000000000000 
       Assigned by the subsystem vendor to uniquely identify the subsystem
     */
  } Bits;
  UINT16 Data;
} SDID_UBOX_DFX_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_UBOX_DFX_REG supported on:                                          */
/*       IVT_EP (0x1015A034)                                                  */
/*       IVT_EX (0x1015A034)                                                  */
/*       HSX (0x10186034)                                                     */
/*       BDX (0x10186034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_UBOX_DFX_REG 0x08011034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
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
} CAPPTR_UBOX_DFX_STRUCT;
#endif /* ASM_INC */


/* INTL_UBOX_DFX_REG supported on:                                            */
/*       IVT_EP (0x1015A03C)                                                  */
/*       IVT_EX (0x1015A03C)                                                  */
/*       HSX (0x1018603C)                                                     */
/*       BDX (0x1018603C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_UBOX_DFX_REG 0x0801103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
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
} INTL_UBOX_DFX_STRUCT;
#endif /* ASM_INC */


/* INTPIN_UBOX_DFX_REG supported on:                                          */
/*       IVT_EP (0x1015A03D)                                                  */
/*       IVT_EX (0x1015A03D)                                                  */
/*       HSX (0x1018603D)                                                     */
/*       BDX (0x1018603D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_UBOX_DFX_REG 0x0801103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
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
} INTPIN_UBOX_DFX_STRUCT;
#endif /* ASM_INC */


/* MINGNT_UBOX_DFX_REG supported on:                                          */
/*       IVT_EP (0x1015A03E)                                                  */
/*       IVT_EX (0x1015A03E)                                                  */
/*       HSX (0x1018603E)                                                     */
/*       BDX (0x1018603E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_UBOX_DFX_REG 0x0801103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
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
} MINGNT_UBOX_DFX_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_UBOX_DFX_REG supported on:                                          */
/*       IVT_EP (0x1015A03F)                                                  */
/*       IVT_EX (0x1015A03F)                                                  */
/*       HSX (0x1018603F)                                                     */
/*       BDX (0x1018603F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_UBOX_DFX_REG 0x0801103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
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
} MAXLAT_UBOX_DFX_STRUCT;
#endif /* ASM_INC */


/* SMM_DELAYED0_UBOX_DFX_REG supported on:                                    */
/*       HSX (0x40186040)                                                     */
/*       BDX (0x40186040)                                                     */
/* Register default value:              0x00000000                            */
#define SMM_DELAYED0_UBOX_DFX_REG 0x08014040

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
 * This CSR contains bit per logical processor indicating
 *       whether the logical processor is in the middle of long flow and hence
 *       will delay servicing of SMI. SMI will be serviced after the long flow
 *       completes execution.
 * 
 *       16 cores will reside in instance 0 as such:
 *       Bit          function
 *       0            Core 0 Thread 0 is delayed
 *       1            Core 0 Thread 1 is delayed
 *       2            Core 1 Thread 0 is delayed
 *       3            ...
 *       31           Core 15 Thread 1 is delayed
 */
typedef union {
  struct {
    UINT32 log_proc : 32;
    /* log_proc - Bits[31:0], RO_V, default = 32'b00000000000000000000000000000000 
       
                 This bit will be set at the start of the flows listed below and
                 cleared at the end of the flows.
                    Patch load
                    C6 entry
                    C6 exit
                    WBINVD
                    Ratio Change/Throttle/S1
               
     */
  } Bits;
  UINT32 Data;
} SMM_DELAYED0_UBOX_DFX_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SMM_DELAYED1_UBOX_DFX_REG supported on:                                    */
/*       HSX (0x40186044)                                                     */
/*       BDX (0x40186044)                                                     */
/* Register default value:              0x00000000                            */
#define SMM_DELAYED1_UBOX_DFX_REG 0x08014044
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.6.CFG.xml.
 * This CSR contains bit per logical processor indicating
 *       whether the logical processor is in the middle of long flow and hence
 *       will delay servicing of SMI. SMI will be serviced after the long flow
 *       completes execution.
 * 
 *       2 cores will reside in instance 1 as such:
 *       Bit          function
 *       0            Core 16 Thread 0 is delayed
 *       1            Core 16 Thread 1 is delayed
 *       2            Core 17 Thread 0 is delayed
 *       3            Core 17 Thread 1 is delayed
 */
typedef union {
  struct {
    UINT32 log_proc : 4;
    /* log_proc - Bits[3:0], RO_V, default = 4'b0000 
       
                 This bit will be set at the start of the flows listed below and
                 cleared at the end of the flows.
                    Patch load
                    C6 entry
                    C6 exit
                    WBINVD
                    Ratio Change/Throttle/S1
               
     */
    UINT32 rsvd : 28;
    /* rsvd - Bits[31:4], RO, default = 28'b0000000000000000000000000000 
       
               Reserved
               
     */
  } Bits;
  UINT32 Data;
} SMM_DELAYED1_UBOX_DFX_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
 * This CSR contains bit per logical processor indicating
 *       whether the logical processor is in the middle of long flow and hence
 *       will delay servicing of SMI. SMI will be serviced after the long flow
 *       completes execution.
 * 
 *       2 cores will reside in instance 1 as such:
 *       Bit          function
 *       0            Core 16 Thread 0 is delayed
 *       1            Core 16 Thread 1 is delayed
 *       2            ---
 *       15           Core 23 Thread 1 is delayed
 */
typedef union {
  struct {
    UINT32 log_proc : 16;
    /* log_proc - Bits[15:0], RO_V, default = 16'b0000000000000000 
       
                 This bit will be set at the start of the flows listed below and
                 cleared at the end of the flows.
                    Patch load
                    C6 entry
                    C6 exit
                    WBINVD
                    Ratio Change/Throttle/S1
               
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], RO, default = 16'b0000000000000000 
       
               Reserved
               
     */
  } Bits;
  UINT32 Data;
} SMM_DELAYED1_UBOX_DFX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* SMM_BLOCKED0_UBOX_DFX_REG supported on:                                    */
/*       HSX (0x40186048)                                                     */
/*       BDX (0x40186048)                                                     */
/* Register default value:              0xFFFFFFFF                            */
#define SMM_BLOCKED0_UBOX_DFX_REG 0x08014048

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
 * This CSR contains bit per logical processor indicating
 *       whether the logical processor is in state where SMIs are blocked and hence
 *       will not be able to service SMI. SMI can be serviced after the logical
 *       processor exits the state in which SMIs are blocked
 * 
 *       16 cores will reside in instance 0 as such:
 *       Bit          function
 *       0            Core 0 Thread 0 is blocked
 *       1            Core 0 Thread 1 is blocked
 *       2            Core 1 Thread 0 is blocked
 *       3            ...
 *       31           Core 15 Thread 1 is blocked
 */
typedef union {
  struct {
    UINT32 log_proc : 32;
    /* log_proc - Bits[31:0], RO_V, default = 32'b11111111111111111111111111111111 
       
                 This bit will be set when the logical processor is in the following
                 states
                    Wait For SIPI
                    SENTER Sleep
                    VMX Abort
                    Error Shutdown (Machine check in WFS state)
               
     */
  } Bits;
  UINT32 Data;
} SMM_BLOCKED0_UBOX_DFX_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SMM_BLOCKED1_UBOX_DFX_REG supported on:                                    */
/*       HSX (0x4018604C)                                                     */
/*       BDX (0x4018604C)                                                     */
/* Register default value on HSX:       0x0000000F                            */
/* Register default value on BDX:       0x0000FFFF                            */
#define SMM_BLOCKED1_UBOX_DFX_REG 0x0801404C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.6.CFG.xml.
 * This CSR contains bit per logical processor indicating
 *       whether the logical processor is in state where SMIs are blocked and hence
 *       will not be able to service SMI. SMI can be serviced after the logical
 *       processor exits the state in which SMIs are blocked
 * 
 *       2 cores will reside in instance 1 as such:
 *       Bit          function
 *       0            Core 16 Thread 0 is blocked
 *       1            Core 16 Thread 1 is blocked
 *       2            Core 17 Thread 0 is blocked
 *       3            Core 17 Thread 1 is blocked
 */
typedef union {
  struct {
    UINT32 log_proc : 4;
    /* log_proc - Bits[3:0], RO_V, default = 4'b1111 
       
                 This bit will be set when the logical processor is in the following
                 states
                    Wait For SIPI
                    SENTER Sleep
                    VMX Abort
                    Error Shutdown (Machine check in WFS state)
               
     */
    UINT32 rsvd : 28;
    /* rsvd - Bits[31:4], RO, default = 28'b0000000000000000000000000000 
       
               Reserved
               
     */
  } Bits;
  UINT32 Data;
} SMM_BLOCKED1_UBOX_DFX_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
 * This CSR contains bit per logical processor indicating
 *       whether the logical processor is in state where SMIs are blocked and hence
 *       will not be able to service SMI. SMI can be serviced after the logical
 *       processor exits the state in which SMIs are blocked
 * 
 *       2 cores will reside in instance 1 as such:
 *       Bit          function
 *       0            Core 16 Thread 0 is blocked
 *       1            Core 16 Thread 1 is blocked
 *       2            ---
 *      15            Core 23 Thread 1 is blocked
 */
typedef union {
  struct {
    UINT32 log_proc : 16;
    /* log_proc - Bits[15:0], RO_V, default = 16'b1111111111111111 
       
                 This bit will be set when the logical processor is in the following
                 states
                    Wait For SIPI
                    SENTER Sleep
                    VMX Abort
                    Error Shutdown (Machine check in WFS state)
               
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], RO, default = 16'b0000000000000000 
       
               Reserved
               
     */
  } Bits;
  UINT32 Data;
} SMM_BLOCKED1_UBOX_DFX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* SMM_ENABLE0_UBOX_DFX_REG supported on:                                     */
/*       HSX (0x40186050)                                                     */
/*       BDX (0x40186050)                                                     */
/* Register default value:              0xFFFFFFFF                            */
#define SMM_ENABLE0_UBOX_DFX_REG 0x08014050

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
 * This CSR has Targeted SMI controls and has SMM enable bits for
 *         each logical processor in a socket. This CSR can be written only from
 *         within SMM but can be read by SMM and non SMM code.
 * 
 *       16 cores will reside in instance 0 as such:
 *       Bit          function
 *       0            Core 0 Thread 0 is enabled
 *       1            Core 0 Thread 1 is enabled
 *       2            Core 1 Thread 0 is enabled
 *       3            ...
 *       31           Core 15 Thread 1 is enabled
 */
typedef union {
  struct {
    UINT32 log_proc_en : 32;
    /* log_proc_en - Bits[31:0], RW_LB, default = 32'b11111111111111111111111111111111 
       
                 Enable for each logical processor within the socket. If bit X is set,
                 then the corresponding logical processor will transfer control to SMM
                 handler in response to SMI (legacy). If bit X is clear, then the
                 corresponding logical processor will ignore/drop SMIs
               
     */
  } Bits;
  UINT32 Data;
} SMM_ENABLE0_UBOX_DFX_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SMM_ENABLE1_UBOX_DFX_REG supported on:                                     */
/*       HSX (0x40186054)                                                     */
/*       BDX (0x40186054)                                                     */
/* Register default value on HSX:       0x0000000F                            */
/* Register default value on BDX:       0x0000FFFF                            */
#define SMM_ENABLE1_UBOX_DFX_REG 0x08014054
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.6.CFG.xml.
 * This CSR has Targeted SMI controls and has SMM enable bits for
 *         each logical processor in a socket. This CSR can be written only from
 *         within SMM but can be read by SMM and non SMM code.
 * 
 *       2 cores will reside in instance 1 as such:
 *       Bit          function
 *       0            Core 16 Thread 0 is enabled
 *       1            Core 16 Thread 1 is enabled
 *       2            Core 17 Thread 0 is enabled
 *       3            Core 17 Thread 1 is enabled
 */
typedef union {
  struct {
    UINT32 log_proc_en : 4;
    /* log_proc_en - Bits[3:0], RW_LB, default = 4'b1111 
       
                 Enable for each logical processor within the socket. If bit X is set,
                 then the corresponding logical processor will transfer control to SMM
                 handler in response to SMI (legacy). If bit X is clear, then the
                 corresponding logical processor will ignore/drop SMIs
               
     */
    UINT32 rsvd : 28;
    /* rsvd - Bits[31:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SMM_ENABLE1_UBOX_DFX_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
 * This CSR has Targeted SMI controls and has SMM enable bits for
 *         each logical processor in a socket. This CSR can be written only from
 *         within SMM but can be read by SMM and non SMM code.
 * 
 *       2 cores will reside in instance 1 as such:
 *       Bit          function
 *       0            Core 16 Thread 0 is enabled
 *       1            Core 16 Thread 1 is enabled
 *       2            ---
 *       3            Core 23 Thread 1 is enabled
 */
typedef union {
  struct {
    UINT32 log_proc_en : 16;
    /* log_proc_en - Bits[15:0], RW_LB, default = 16'b1111111111111111 
       
                 Enable for each logical processor within the socket. If bit X is set,
                 then the corresponding logical processor will transfer control to SMM
                 handler in response to SMI (legacy). If bit X is clear, then the
                 corresponding logical processor will ignore/drop SMIs
               
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SMM_ENABLE1_UBOX_DFX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* SMM_FEATURE_CONTROL_UBOX_DFX_REG supported on:                             */
/*       HSX (0x40186058)                                                     */
/*       BDX (0x40186058)                                                     */
/* Register default value:              0x00000000                            */
#define SMM_FEATURE_CONTROL_UBOX_DFX_REG 0x08014058

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
 * This CSR holds enable bits for on-chip storage of SMM and
 *         Code Access Check Violation features. This CSR can be written only from
 *         within SMM but can be read by SMM and non SMM code. This CSR is cleared
 *         only on cold reset.
 */
typedef union {
  struct {
    UINT32 lock : 1;
    /* lock - Bits[0:0], RW_LB, default = 1'b0 
       
                 If Lock = 1, CPU will drop writes to the CSR
                 If Lock = 0, CPU will allow update to b[2:0]
               
     */
    UINT32 smm_cpu_save_en : 1;
    /* smm_cpu_save_en - Bits[1:1], RW_LB, default = 1'b0 
       
                 If 0, SMI/RSM will save/restore state in SMRAM
                 If 1, SMI/RSM will save/restore state from SRAM
               
     */
    UINT32 smm_code_chk_en : 1;
    /* smm_code_chk_en - Bits[2:2], RW_LB, default = 1'b0 
       
                 If 1, generate unrecoverable MCHK on code fetch outside SMRR
                 If 0, do not report any errors for code fetch outside SMRR
               
     */
    UINT32 rsvd : 29;
    /* rsvd - Bits[31:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SMM_FEATURE_CONTROL_UBOX_DFX_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RABS_DEBUG_UBOX_DFX_REG supported on:                                      */
/*       IVT_EP (0x4015A0A0)                                                  */
/*       IVT_EX (0x4015A0A0)                                                  */
/*       HSX (0x401860A0)                                                     */
/*       BDX (0x401860A0)                                                     */
/* Register default value:              0x00000000                            */
#define RABS_DEBUG_UBOX_DFX_REG 0x080140A0


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.6.CFG.xml.
 * Debug Bus Control
 */
typedef union {
  struct {
    UINT32 ubox_debug_bus_enable : 1;
    /* ubox_debug_bus_enable - Bits[0:0], RWS_L, default = 1'b0 
       enable debug bus funtionality in UBOX.
        Access Controlled via UBOXDebugCtrl uCR
     */
    UINT32 rsvd_1 : 7;
    /* rsvd_1 - Bits[7:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 debug_uxrabs_sel_lo : 5;
    /* debug_uxrabs_sel_lo - Bits[12:8], RWS_L, default = 5'b00000 
       Selects source of low byte of debug bus from RABS fub
        0 = output 0's
        others per MAS
     */
    UINT32 debug_uxrabs_sel_hi : 5;
    /* debug_uxrabs_sel_hi - Bits[17:13], RWS_L, default = 5'b00000 
       Selects source of high byte of debug bus from the rabs block
        0 = output 0's
        others per MAS
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 debug_dfcu_sel_lo : 4;
    /* debug_dfcu_sel_lo - Bits[23:20], RWS_L, default = 4'b0000 
       Selects source of low byte of debug bus from DFCU fub
        0 = output 0's
        others per MAS
     */
    UINT32 debug_dfcu_sel_hi : 4;
    /* debug_dfcu_sel_hi - Bits[27:24], RWS_L, default = 4'b0000 
       Selects source of high byte of debug bus from the DFCU block
        0 = output 0's
        others per MAS
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} RABS_DEBUG_UBOX_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* DECS_DEBUG_UBOX_DFX_REG supported on:                                      */
/*       IVT_EP (0x4015A0B0)                                                  */
/*       IVT_EX (0x4015A0B0)                                                  */
/*       HSX (0x401860B0)                                                     */
/*       BDX (0x401860B0)                                                     */
/* Register default value:              0x00000000                            */
#define DECS_DEBUG_UBOX_DFX_REG 0x080140B0


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.6.CFG.xml.
 * DECS Debug Register Control
 */
typedef union {
  struct {
    UINT32 uxdecs_debug_bus_enable : 1;
    /* uxdecs_debug_bus_enable - Bits[0:0], RWS_L, default = 1'b0 
       enable debug bus functionality in uxdecs fub
     */
    UINT32 rsvd_1 : 7;
    /* rsvd_1 - Bits[7:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 debug_uxdecs_sel_lo : 5;
    /* debug_uxdecs_sel_lo - Bits[12:8], RWS_L, default = 5'b00000 
       Selects source of low byte of debug bus
        0 = output 0's
        others per MAS
     */
    UINT32 debug_uxdecs_sel_hi : 5;
    /* debug_uxdecs_sel_hi - Bits[17:13], RWS_L, default = 5'b00000 
       Selects source of high byte of debug bus
        0 = output 0's
        others per MAS
     */
    UINT32 rsvd_18 : 2;
    /* rsvd_18 - Bits[19:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 debug_mpdecs_sel_lo : 4;
    /* debug_mpdecs_sel_lo - Bits[23:20], RWS_L, default = 4'b0000 
       Selects source of low byte of debug bus
        0 = output 0's
        others per MAS
     */
    UINT32 debug_mpdecs_sel_hi : 4;
    /* debug_mpdecs_sel_hi - Bits[27:24], RWS_L, default = 4'b0000 
       Selects source of high byte of debug bus
        0 = output 0's
        others per MAS
     */
    UINT32 rsvd_28 : 4;
    /* rsvd_28 - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DECS_DEBUG_UBOX_DFX_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* BIST_RESULTS_UBOX_DFX_REG supported on:                                    */
/*       IVT_EP (0x4015A0B4)                                                  */
/*       IVT_EX (0x4015A0B4)                                                  */
/*       HSX (0x401860B4)                                                     */
/*       BDX (0x401860B4)                                                     */
/* Register default value:              0x00000000                            */
#define BIST_RESULTS_UBOX_DFX_REG 0x080140B4


#if defined(HSX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.16.6.CFG.xml.
 * Config read-only access to core BIST results 
 */
typedef union {
  struct {
    UINT32 results : 18;
    /* results - Bits[17:0], RO_V, default = 18'd0 
       0 - BIST failure, 1 - BIST pass
     */
    UINT32 rsvd : 14;
    /* rsvd - Bits[31:18], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} BIST_RESULTS_UBOX_DFX_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) */

#if defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.16.6.CFG.xml.
 * Config read-only access to core BIST results 
 */
typedef union {
  struct {
    UINT32 results : 24;
    /* results - Bits[23:0], RO_V, default = 24'd0 
       0 - BIST failure, 1 - BIST pass
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} BIST_RESULTS_UBOX_DFX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (BDX_HOST) */



/* DECS_UCLK_MONITORING_CONFIG_UBOX_DFX_REG supported on:                     */
/*       IVT_EP (0x4015A070)                                                  */
/*       IVT_EX (0x4015A070)                                                  */
/* Register default value:              0x00000000                            */
#define DECS_UCLK_MONITORING_CONFIG_UBOX_DFX_REG 0x08014070



/* DECS_UCLK_MSG_DEST_QUALIFIER_UBOX_DFX_REG supported on:                    */
/*       IVT_EP (0x4015A074)                                                  */
/*       IVT_EX (0x4015A074)                                                  */
/* Register default value:              0x00000000                            */
#define DECS_UCLK_MSG_DEST_QUALIFIER_UBOX_DFX_REG 0x08014074



/* DECS_UCLK_MSG_OPCODE_QUALIFIER_UBOX_DFX_REG supported on:                  */
/*       IVT_EP (0x4015A078)                                                  */
/*       IVT_EX (0x4015A078)                                                  */
/* Register default value:              0x00000000                            */
#define DECS_UCLK_MSG_OPCODE_QUALIFIER_UBOX_DFX_REG 0x08014078



/* DECS_UCLK_MSG_SOURCE_QUALIFIER_UBOX_DFX_REG supported on:                  */
/*       IVT_EP (0x4015A07C)                                                  */
/*       IVT_EX (0x4015A07C)                                                  */
/* Register default value:              0x00000000                            */
#define DECS_UCLK_MSG_SOURCE_QUALIFIER_UBOX_DFX_REG 0x0801407C



/* RACU_UCLK_MONITORING_CONFIG_UBOX_DFX_REG supported on:                     */
/*       IVT_EP (0x4015A080)                                                  */
/*       IVT_EX (0x4015A080)                                                  */
/* Register default value:              0x00000000                            */
#define RACU_UCLK_MONITORING_CONFIG_UBOX_DFX_REG 0x08014080



/* RACU_UCLK_MSG_DEST_QUALIFIER_UBOX_DFX_REG supported on:                    */
/*       IVT_EP (0x4015A084)                                                  */
/*       IVT_EX (0x4015A084)                                                  */
/* Register default value:              0x00000000                            */
#define RACU_UCLK_MSG_DEST_QUALIFIER_UBOX_DFX_REG 0x08014084



/* RACU_UCLK_MSG_OPCODE_QUALIFIER_UBOX_DFX_REG supported on:                  */
/*       IVT_EP (0x4015A088)                                                  */
/*       IVT_EX (0x4015A088)                                                  */
/* Register default value:              0x00000000                            */
#define RACU_UCLK_MSG_OPCODE_QUALIFIER_UBOX_DFX_REG 0x08014088



/* RACU_UCLK_MSG_SOURCE_QUALIFIER_UBOX_DFX_REG supported on:                  */
/*       IVT_EP (0x4015A08C)                                                  */
/*       IVT_EX (0x4015A08C)                                                  */
/* Register default value:              0x00000000                            */
#define RACU_UCLK_MSG_SOURCE_QUALIFIER_UBOX_DFX_REG 0x0801408C









#endif /* UBOX_DFX_h */
