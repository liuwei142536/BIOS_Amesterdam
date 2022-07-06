/* Date Stamp: 8/23/2014 */

#ifndef MCIO_DDRIOEXT_h
#define MCIO_DDRIOEXT_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* MCIO_DDRIOEXT_IVT_EP_DEV 17                                                */
/* MCIO_DDRIOEXT_IVT_EP_FUN 5                                                 */
/* For IVT_HOST:                                                              */
/* MCIO_DDRIOEXT_IVT_EX_DEV 31                                                */
/* MCIO_DDRIOEXT_IVT_EX_FUN 5                                                 */
/* For HSX_HOST:                                                              */
/* MCIO_DDRIOEXT_HSX_DEV 20                                                   */
/* MCIO_DDRIOEXT_HSX_FUN 5                                                    */
/* For BDX_HOST:                                                              */
/* MCIO_DDRIOEXT_BDX_DEV 20                                                   */
/* MCIO_DDRIOEXT_BDX_FUN 5                                                    */

/* VID_MCIO_DDRIOEXT_REG supported on:                                        */
/*       IVT_EP (0x2018D000)                                                  */
/*       IVT_EX (0x201FD000)                                                  */
/*       HSX (0x201A5000)                                                     */
/*       BDX (0x201A5000)                                                     */
/* Register default value:              0x8086                                */
#define VID_MCIO_DDRIOEXT_REG 0x0D032000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
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
} VID_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */


/* DID_MCIO_DDRIOEXT_REG supported on:                                        */
/*       IVT_EP (0x2018D002)                                                  */
/*       IVT_EX (0x201FD002)                                                  */
/*       HSX (0x201A5002)                                                     */
/*       BDX (0x201A5002)                                                     */
/* Register default value on IVT_EP:    0x0EBD                                */
/* Register default value on IVT_EX:    0x0EDD                                */
/* Register default value on HSX:       0x2FBD                                */
/* Register default value on BDX:       0x6FBD                                */
#define DID_MCIO_DDRIOEXT_REG 0x0D032002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6FBD 
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
} DID_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */


/* PCICMD_MCIO_DDRIOEXT_REG supported on:                                     */
/*       IVT_EP (0x2018D004)                                                  */
/*       IVT_EX (0x201FD004)                                                  */
/*       HSX (0x201A5004)                                                     */
/*       BDX (0x201A5004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_MCIO_DDRIOEXT_REG 0x0D032004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
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
} PCICMD_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */


/* PCISTS_MCIO_DDRIOEXT_REG supported on:                                     */
/*       IVT_EP (0x2018D006)                                                  */
/*       IVT_EX (0x201FD006)                                                  */
/*       HSX (0x201A5006)                                                     */
/*       BDX (0x201A5006)                                                     */
/* Register default value on IVT_EP:    0x0010                                */
/* Register default value on IVT_EX:    0x0010                                */
/* Register default value on HSX:       0x0000                                */
/* Register default value on BDX:       0x0000                                */
#define PCISTS_MCIO_DDRIOEXT_REG 0x0D032006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
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
} PCISTS_MCIO_DDRIOEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_MCIO_DDRIOEXT_REG supported on:                                        */
/*       IVT_EP (0x1018D008)                                                  */
/*       IVT_EX (0x101FD008)                                                  */
/*       HSX (0x101A5008)                                                     */
/*       BDX (0x101A5008)                                                     */
/* Register default value:              0x00                                  */
#define RID_MCIO_DDRIOEXT_REG 0x0D031008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
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
} RID_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_MCIO_DDRIOEXT_REG supported on:                                     */
/*       IVT_EP (0x1018D009)                                                  */
/*       IVT_EX (0x101FD009)                                                  */
/*       HSX (0x101A5009)                                                     */
/*       BDX (0x101A5009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_MCIO_DDRIOEXT_REG 0x0D031009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_MCIO_DDRIOEXT_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_MCIO_DDRIOEXT_REG supported on:                                     */
/*       IVT_EP (0x2018D00A)                                                  */
/*       IVT_EX (0x201FD00A)                                                  */
/*       HSX (0x201A500A)                                                     */
/*       BDX (0x201A500A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_MCIO_DDRIOEXT_REG 0x0D03200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
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
} CCR_N1_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */


/* CLSR_MCIO_DDRIOEXT_REG supported on:                                       */
/*       IVT_EP (0x1018D00C)                                                  */
/*       IVT_EX (0x101FD00C)                                                  */
/*       HSX (0x101A500C)                                                     */
/*       BDX (0x101A500C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_MCIO_DDRIOEXT_REG 0x0D03100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
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
} CLSR_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */


/* PLAT_MCIO_DDRIOEXT_REG supported on:                                       */
/*       IVT_EP (0x1018D00D)                                                  */
/*       IVT_EX (0x101FD00D)                                                  */
/*       HSX (0x101A500D)                                                     */
/*       BDX (0x101A500D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_MCIO_DDRIOEXT_REG 0x0D03100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
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
} PLAT_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */


/* HDR_MCIO_DDRIOEXT_REG supported on:                                        */
/*       IVT_EP (0x1018D00E)                                                  */
/*       IVT_EX (0x101FD00E)                                                  */
/*       HSX (0x101A500E)                                                     */
/*       BDX (0x101A500E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_MCIO_DDRIOEXT_REG 0x0D03100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
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
} HDR_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */


/* BIST_MCIO_DDRIOEXT_REG supported on:                                       */
/*       IVT_EP (0x1018D00F)                                                  */
/*       IVT_EX (0x101FD00F)                                                  */
/*       HSX (0x101A500F)                                                     */
/*       BDX (0x101A500F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_MCIO_DDRIOEXT_REG 0x0D03100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
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
} BIST_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_MCIO_DDRIOEXT_REG supported on:                                     */
/*       IVT_EP (0x1018D034)                                                  */
/*       IVT_EX (0x101FD034)                                                  */
/*       HSX (0x101A5034)                                                     */
/*       BDX (0x101A5034)                                                     */
/* Register default value on IVT_EP:    0x40                                  */
/* Register default value on IVT_EX:    0x40                                  */
/* Register default value on HSX:       0x00                                  */
/* Register default value on BDX:       0x00                                  */
#define CAPPTR_MCIO_DDRIOEXT_REG 0x0D031034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
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
} CAPPTR_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */


/* INTL_MCIO_DDRIOEXT_REG supported on:                                       */
/*       IVT_EP (0x1018D03C)                                                  */
/*       IVT_EX (0x101FD03C)                                                  */
/*       HSX (0x101A503C)                                                     */
/*       BDX (0x101A503C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_MCIO_DDRIOEXT_REG 0x0D03103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
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
} INTL_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */


/* INTPIN_MCIO_DDRIOEXT_REG supported on:                                     */
/*       IVT_EP (0x1018D03D)                                                  */
/*       IVT_EX (0x101FD03D)                                                  */
/*       HSX (0x101A503D)                                                     */
/*       BDX (0x101A503D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_MCIO_DDRIOEXT_REG 0x0D03103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
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
} INTPIN_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */


/* MINGNT_MCIO_DDRIOEXT_REG supported on:                                     */
/*       IVT_EP (0x1018D03E)                                                  */
/*       IVT_EX (0x101FD03E)                                                  */
/*       HSX (0x101A503E)                                                     */
/*       BDX (0x101A503E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_MCIO_DDRIOEXT_REG 0x0D03103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
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
} MINGNT_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_MCIO_DDRIOEXT_REG supported on:                                     */
/*       IVT_EP (0x1018D03F)                                                  */
/*       IVT_EX (0x101FD03F)                                                  */
/*       HSX (0x101A503F)                                                     */
/*       BDX (0x101A503F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_MCIO_DDRIOEXT_REG 0x0D03103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
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
} MAXLAT_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */


/* RXGROUP0N0RANK0_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5200)                                                     */
/*       BDX (0x401A5200)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK0_7_MCIO_DDRIOEXT_REG 0x0D034200

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank0
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS (0: 1/16 UI 
       offset,  
                 1: 3/16 UI Offset, 2: 5/16 UI Offsets, 3: 7/16 UI Offsets, 4: 9/16 UI 
       Offsets, 5,6,7: 11/16 UI Offsets.) 
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK0_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK1_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5204)                                                     */
/*       BDX (0x401A5204)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK1_7_MCIO_DDRIOEXT_REG 0x0D034204

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank1
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK1_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK2_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5208)                                                     */
/*       BDX (0x401A5208)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK2_7_MCIO_DDRIOEXT_REG 0x0D034208

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank2
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK2_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK3_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A520C)                                                     */
/*       BDX (0x401A520C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK3_7_MCIO_DDRIOEXT_REG 0x0D03420C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank3
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK3_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK4_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5210)                                                     */
/*       BDX (0x401A5210)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK4_7_MCIO_DDRIOEXT_REG 0x0D034210

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank4
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK4_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK5_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5214)                                                     */
/*       BDX (0x401A5214)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK5_7_MCIO_DDRIOEXT_REG 0x0D034214

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank5
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK5_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK6_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5218)                                                     */
/*       BDX (0x401A5218)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK6_7_MCIO_DDRIOEXT_REG 0x0D034218

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank6
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK6_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK7_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A521C)                                                     */
/*       BDX (0x401A521C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK7_7_MCIO_DDRIOEXT_REG 0x0D03421C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank7
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK7_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK0_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5220)                                                     */
/*       BDX (0x401A5220)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK0_7_MCIO_DDRIOEXT_REG 0x0D034220

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK0_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK1_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5224)                                                     */
/*       BDX (0x401A5224)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK1_7_MCIO_DDRIOEXT_REG 0x0D034224

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK1_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK2_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5228)                                                     */
/*       BDX (0x401A5228)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK2_7_MCIO_DDRIOEXT_REG 0x0D034228

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK2_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK3_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A522C)                                                     */
/*       BDX (0x401A522C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK3_7_MCIO_DDRIOEXT_REG 0x0D03422C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK3_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK4_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5230)                                                     */
/*       BDX (0x401A5230)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK4_7_MCIO_DDRIOEXT_REG 0x0D034230

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK4_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK5_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5234)                                                     */
/*       BDX (0x401A5234)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK5_7_MCIO_DDRIOEXT_REG 0x0D034234

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK5_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK6_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5238)                                                     */
/*       BDX (0x401A5238)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK6_7_MCIO_DDRIOEXT_REG 0x0D034238

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK6_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK7_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A523C)                                                     */
/*       BDX (0x401A523C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK7_7_MCIO_DDRIOEXT_REG 0x0D03423C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK7_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP0N0RANK0_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5240)                                                     */
/*       BDX (0x401A5240)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK0_7_MCIO_DDRIOEXT_REG 0x0D034240
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK0_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK0_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK1_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5244)                                                     */
/*       BDX (0x401A5244)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK1_7_MCIO_DDRIOEXT_REG 0x0D034244
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK1_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK1_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK2_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5248)                                                     */
/*       BDX (0x401A5248)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK2_7_MCIO_DDRIOEXT_REG 0x0D034248
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on. 
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK2_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK2_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK3_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A524C)                                                     */
/*       BDX (0x401A524C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK3_7_MCIO_DDRIOEXT_REG 0x0D03424C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK3_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK3_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK4_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5250)                                                     */
/*       BDX (0x401A5250)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK4_7_MCIO_DDRIOEXT_REG 0x0D034250
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK4_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK4_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK5_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5254)                                                     */
/*       BDX (0x401A5254)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK5_7_MCIO_DDRIOEXT_REG 0x0D034254
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK5_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK5_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK6_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5258)                                                     */
/*       BDX (0x401A5258)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK6_7_MCIO_DDRIOEXT_REG 0x0D034258
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK6_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK6_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK7_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A525C)                                                     */
/*       BDX (0x401A525C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK7_7_MCIO_DDRIOEXT_REG 0x0D03425C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK7_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK7_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP1N0RANK0_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5260)                                                     */
/*       BDX (0x401A5260)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK0_7_MCIO_DDRIOEXT_REG 0x0D034260

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 6'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 6'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 6'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 6'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK0_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK1_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5264)                                                     */
/*       BDX (0x401A5264)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK1_7_MCIO_DDRIOEXT_REG 0x0D034264

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK1_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK2_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5268)                                                     */
/*       BDX (0x401A5268)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK2_7_MCIO_DDRIOEXT_REG 0x0D034268

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK2_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK3_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A526C)                                                     */
/*       BDX (0x401A526C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK3_7_MCIO_DDRIOEXT_REG 0x0D03426C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK3_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK4_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5270)                                                     */
/*       BDX (0x401A5270)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK4_7_MCIO_DDRIOEXT_REG 0x0D034270

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK4_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK5_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5274)                                                     */
/*       BDX (0x401A5274)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK5_7_MCIO_DDRIOEXT_REG 0x0D034274

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK5_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK6_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5278)                                                     */
/*       BDX (0x401A5278)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK6_7_MCIO_DDRIOEXT_REG 0x0D034278

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK6_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK7_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A527C)                                                     */
/*       BDX (0x401A527C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK7_7_MCIO_DDRIOEXT_REG 0x0D03427C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK7_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK0_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5280)                                                     */
/*       BDX (0x401A5280)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK0_7_MCIO_DDRIOEXT_REG 0x0D034280

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK0_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK1_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5284)                                                     */
/*       BDX (0x401A5284)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK1_7_MCIO_DDRIOEXT_REG 0x0D034284

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK1_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK2_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5288)                                                     */
/*       BDX (0x401A5288)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK2_7_MCIO_DDRIOEXT_REG 0x0D034288

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK2_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK3_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A528C)                                                     */
/*       BDX (0x401A528C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK3_7_MCIO_DDRIOEXT_REG 0x0D03428C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK3_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK4_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5290)                                                     */
/*       BDX (0x401A5290)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK4_7_MCIO_DDRIOEXT_REG 0x0D034290

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK4_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK5_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5294)                                                     */
/*       BDX (0x401A5294)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK5_7_MCIO_DDRIOEXT_REG 0x0D034294

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK5_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK6_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5298)                                                     */
/*       BDX (0x401A5298)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK6_7_MCIO_DDRIOEXT_REG 0x0D034298

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK6_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK7_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A529C)                                                     */
/*       BDX (0x401A529C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK7_7_MCIO_DDRIOEXT_REG 0x0D03429C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK7_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXXTALKN0_7_MCIO_DDRIOEXT_REG supported on:                                */
/*       HSX (0x401A52A0)                                                     */
/*       BDX (0x401A52A0)                                                     */
/* Register default value:              0x00000000                            */
#define TXXTALKN0_7_MCIO_DDRIOEXT_REG 0x0D0342A0

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * not used.
 */
typedef union {
  struct {
    UINT32 twototwologicdelayrank0 : 4;
    /* twototwologicdelayrank0 - Bits[3:0], RW_L, default = 4'b0000 
       This may need to be set for bits 3:0 in the nibble for Rank 0
     */
    UINT32 twototwologicdelayrank1 : 4;
    /* twototwologicdelayrank1 - Bits[7:4], RW_L, default = 4'b0000 
       This may need to be set for bits 3:0 in the nibble for Rank 1
     */
    UINT32 twototwologicdelayrank2 : 4;
    /* twototwologicdelayrank2 - Bits[11:8], RW_L, default = 4'b0000 
       This may need to be set for bits 3:0 in the nibble for Rank 2
     */
    UINT32 twototwologicdelayrank3 : 4;
    /* twototwologicdelayrank3 - Bits[15:12], RW_L, default = 4'b0000 
       This may need to be set for bits 3:0 in the nibble for Rank 3
     */
    UINT32 twototwologicdelayrank4 : 4;
    /* twototwologicdelayrank4 - Bits[19:16], RW_L, default = 4'b0000 
       This may need to be set for bits 3:0 in the nibble for Rank 4
     */
    UINT32 twototwologicdelayrank5 : 4;
    /* twototwologicdelayrank5 - Bits[23:20], RW_L, default = 4'b0000 
       This may need to be set for bits 3:0 in the nibble for rank 5
     */
    UINT32 twototwologicdelayrank6 : 4;
    /* twototwologicdelayrank6 - Bits[27:24], RW_L, default = 4'b0000 
       This may need to be set for bits 3:0 in the nibble for rank 6
     */
    UINT32 twototwologicdelayrank7 : 4;
    /* twototwologicdelayrank7 - Bits[31:28], RW_L, default = 4'b0000 
       This may need to be set for bits 3:0 in the nibble for rank 7
     */
  } Bits;
  UINT32 Data;
} TXXTALKN0_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXVREFCTRLN0_7_MCIO_DDRIOEXT_REG supported on:                             */
/*       HSX (0x401A52A4)                                                     */
/*       BDX (0x401A52A4)                                                     */
/* Register default value:              0x00000000                            */
#define RXVREFCTRLN0_7_MCIO_DDRIOEXT_REG 0x0D0342A4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * RxVrefCtrl
 */
typedef union {
  struct {
    UINT32 vrefperbit0 : 7;
    /* vrefperbit0 - Bits[6:0], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben0 : 1;
    /* halflsben0 - Bits[7:7], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit1 : 7;
    /* vrefperbit1 - Bits[14:8], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben1 : 1;
    /* halflsben1 - Bits[15:15], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit2 : 7;
    /* vrefperbit2 - Bits[22:16], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben2 : 1;
    /* halflsben2 - Bits[23:23], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit3 : 7;
    /* vrefperbit3 - Bits[30:24], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben3 : 1;
    /* halflsben3 - Bits[31:31], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
  } Bits;
  UINT32 Data;
} RXVREFCTRLN0_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATATRAINFEEDBACKN0_7_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A52A8)                                                     */
/*       BDX (0x401A52A8)                                                     */
/* Register default value:              0x00000000                            */
#define DATATRAINFEEDBACKN0_7_MCIO_DDRIOEXT_REG 0x0D0342A8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data Training Feedback Results
 */
typedef union {
  struct {
    UINT32 datatrainfeedback : 18;
    /* datatrainfeedback - Bits[17:0], RW_L, default = 1'b0 
       Data Training Feedback Storage
       
               DataTrainFeedback Description:
                Training Step    Bits    Nibble    Description
               --------------    ----    -------  
       -------------------------------------------------------------------------------------------------------------------------------------------------------- 
               Cross Over Cal     0      0        Tx Dqs Phase Detection
               Cross Over Cal     1      0        Tx Dq Phase Detection
               Cross Over Cal     2      0        Rec En Phase Detection
               RcvEn              8:0    0        Byte detection
               Rx Dq-Dqs          0      0        Pass/fail
               Wr Leveling        8:0    0        Byte detection
               Cross Over Cal     3      1        Tx Dqs Phase Detection
               Cross Over Cal     4      1        Tx Dq Phase Detection
               Cross Over Cal     5      1        Rec En Phase Detection
               RcvEn             17:9    1        Byte detection
               Rx Dq-Dqs          1      1        Pass/fail
               Wr Leveling       17:9    1        Byte detection
               
     */
    UINT32 rsvd_18 : 14;
  } Bits;
  UINT32 Data;
} DATATRAINFEEDBACKN0_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATACONTROL4_7_MCIO_DDRIOEXT_REG supported on:                             */
/*       HSX (0x401A52AC)                                                     */
/*       BDX (0x401A52AC)                                                     */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       <could not determine>                 */
#define DATACONTROL4_7_MCIO_DDRIOEXT_REG 0x0D0342AC
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * static configurations of tx and rx.
 */
typedef union {
  struct {
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[0:0], RW_L, default = None 
       enable grid clock divider in xover. used in native ddr mode for data rate 
       greater than 1867MT/s 
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[1:1], RW_L, default = None 
       enable PiRef clock divider in xover. used in vmse2to2mode, or whenever data rate 
       greater than 1867MT/s 
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[2:2], RW_L, default = None 
       enable extra pipestage in xover. POR setting = 1
     */
    UINT32 vmsehalfwidthmode : 1;
    /* vmsehalfwidthmode - Bits[3:3], RW_L, default = None 
       gate ClkPi when the nibble is disabled in VMSE half-width mode
     */
    UINT32 disablercvenextension : 1;
    /* disablercvenextension - Bits[4:4], RW_L, default = None 
       By default receive enable extension will be bypassed in VMSE2to1 mode. Setting 
       this to 1 in VMSE mode will use DDR to extend the rcven. VMSE ACIO LB use case 
       when only one scheduler is used. 
     */
    UINT32 ddrcrinvertpiclk : 2;
    /* ddrcrinvertpiclk - Bits[6:5], RW_L, default = 2'b00 
       invert ClkPiRef (per nibble) in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 
       phase. 
     */
    UINT32 dqvmselogicdelay : 8;
    /* dqvmselogicdelay - Bits[14:7], RW_L, default = 1'b0 
       DqVmseLogicDelay: If set add one UI delay to the Tx Fifo Read and DQ drivenable
     */
    UINT32 rcvenvmselogicdelay : 2;
    /* rcvenvmselogicdelay - Bits[16:15], RW_L, default = 1'b0 
       Receive enable VmseLogicDelay: If set add one UI delay to the receive enable
     */
    UINT32 dqsvmselogicdelay : 2;
    /* dqsvmselogicdelay - Bits[18:17], RW_L, default = 1'b0 
       DQS VMSE Logic Delay
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[19:19], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 ddrsroslmask : 1;
    /* ddrsroslmask - Bits[20:20], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 dqvmsenibblelogicdelay : 2;
    /* dqvmsenibblelogicdelay - Bits[22:21], RW_L, default = None 
       VMSE Logic delay that is shared for all bits in the nibble
     */
    UINT32 rsvd_23 : 9;
  } Bits;
  UINT32 Data;
} DATACONTROL4_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * static configurations of tx and rx.
 */
typedef union {
  struct {
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[0:0], RW_L, default = None 
       enable grid clock divider in xover. used in native ddr mode for data rate 
       greater than 1867MT/s 
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[1:1], RW_L, default = None 
       enable PiRef clock divider in xover. used in vmse2to2mode, or whenever data rate 
       greater than 1867MT/s 
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[2:2], RW_L, default = None 
       enable extra pipestage in xover. POR setting = 1
     */
    UINT32 vmsehalfwidthmode : 1;
    /* vmsehalfwidthmode - Bits[3:3], RW_L, default = None 
       gate ClkPi when the nibble is disabled in VMSE half-width mode
     */
    UINT32 disablercvenextension : 1;
    /* disablercvenextension - Bits[4:4], RW_L, default = None 
       By default receive enable extension will be bypassed in VMSE2to1 mode. Setting 
       this to 1 in VMSE mode will use DDR to extend the rcven. VMSE ACIO LB use case 
       when only one scheduler is used. 
     */
    UINT32 ddrcrinvertpiclk : 2;
    /* ddrcrinvertpiclk - Bits[6:5], RW_L, default = 2'b00 
       invert ClkPiRef (per nibble) in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 
       phase. 
     */
    UINT32 dqvmselogicdelay : 8;
    /* dqvmselogicdelay - Bits[14:7], RW_L, default = 1'b0 
       DqVmseLogicDelay: If set add one UI delay to the Tx Fifo Read and DQ drivenable
     */
    UINT32 rcvenvmselogicdelay : 2;
    /* rcvenvmselogicdelay - Bits[16:15], RW_L, default = 1'b0 
       Receive enable VmseLogicDelay: If set add one UI delay to the receive enable
     */
    UINT32 dqsvmselogicdelay : 2;
    /* dqsvmselogicdelay - Bits[18:17], RW_L, default = 1'b0 
       DQS VMSE Logic Delay
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[19:19], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 ddrsroslmask : 1;
    /* ddrsroslmask - Bits[20:20], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 dqvmsenibblelogicdelay : 2;
    /* dqvmsenibblelogicdelay - Bits[22:21], RW_L, default = None 
       VMSE Logic delay that is shared for all bits in the nibble
     */
    UINT32 writecrcenable : 1;
    /* writecrcenable - Bits[23:23], RW_L, default = None 
       Enable write crc mode
     */
    UINT32 bdx_sys : 1;
    /* bdx_sys - Bits[24:24], RW_L, default = '1 
       This bit helps to bypass the delaycell in transmitter
     */
    UINT32 rsvd_25 : 6;
    UINT32 bypassinitoptolatchenable : 1;
    /* bypassinitoptolatchenable - Bits[31:31], RW_L, default = None 
       This will ignore an InitOp when the RFFSM is active for a read operation
     */
  } Bits;
  UINT32 Data;
} DATACONTROL4_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* VSSHIORVREFCONTROL1_7_MCIO_DDRIOEXT_REG supported on:                      */
/*       BDX (0x401A52CC)                                                     */
/* Register default value:              0x00000000                            */
#define VSSHIORVREFCONTROL1_7_MCIO_DDRIOEXT_REG 0x0D0342CC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * VssHi Controls on Channel 0 only. DIMM Vref Controls on  Channel1, databyte 7, 
 * only. Extension for BDX. 
 */
typedef union {
  struct {
    UINT32 vsshiorvrefctl : 8;
    /* vsshiorvrefctl - Bits[7:0], RW_L, default = 8'b0 
        BDX-specific extended control signal. 
     */
    UINT32 spare : 24;
    /* spare - Bits[31:8], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} VSSHIORVREFCONTROL1_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* PXCATTACK1SEL_7_MCIO_DDRIOEXT_REG supported on:                            */
/*       BDX (0x401A52D0)                                                     */
/* Register default value:              0x00FAC688                            */
#define PXCATTACK1SEL_7_MCIO_DDRIOEXT_REG 0x0D0342D0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Select 2 aggressors out 3 neighbors in a nibble
 */
typedef union {
  struct {
    UINT32 attackr1d0 : 3;
    /* attackr1d0 - Bits[2:0], RW_L, default = 3'd0 
       xtalk aggressor1 select for dq bit[0] in a byte
     */
    UINT32 attackr1d1 : 3;
    /* attackr1d1 - Bits[5:3], RW_L, default = 3'd1 
       xtalk aggressor1 select for dq bit[1] in a byte
     */
    UINT32 attackr1d2 : 3;
    /* attackr1d2 - Bits[8:6], RW_L, default = 3'd2 
       xtalk aggressor1 select for dq bit[2] in a byte
     */
    UINT32 attackr1d3 : 3;
    /* attackr1d3 - Bits[11:9], RW_L, default = 3'd3 
       xtalk aggressor1 select for dq bit[3] in a byte
     */
    UINT32 attackr1d4 : 3;
    /* attackr1d4 - Bits[14:12], RW_L, default = 3'd4 
       xtalk aggressor1 select for dq bit[4] in a byte
     */
    UINT32 attackr1d5 : 3;
    /* attackr1d5 - Bits[17:15], RW_L, default = 3'd5 
       xtalk aggressor1 select for dq bit[5] in a byte
     */
    UINT32 attackr1d6 : 3;
    /* attackr1d6 - Bits[20:18], RW_L, default = 3'd6 
       xtalk aggressor1 select for dq bit[6] in a byte
     */
    UINT32 attackr1d7 : 3;
    /* attackr1d7 - Bits[23:21], RW_L, default = 3'd7 
       xtalk aggressor1 select for dq bit[7] in a byte
     */
    UINT32 spare : 8;
    /* spare - Bits[31:24], RW_L, default = 8'b0 
       spare bits.
     */
  } Bits;
  UINT32 Data;
} PXCATTACK1SEL_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */






/* DATAOFFSETTRAIN_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A52DC)                                                     */
/*       BDX (0x401A52DC)                                                     */
/* Register default value:              0x00000000                            */
#define DATAOFFSETTRAIN_7_MCIO_DDRIOEXT_REG 0x0D0342DC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rcvenoffset : 6;
    /* rcvenoffset - Bits[5:0], RW_L, default = 1'b0 
       Offset added to Trained RxRcvEn Value.
     */
    UINT32 rxdqsoffset : 7;
    /* rxdqsoffset - Bits[12:6], RW_L, default = 1'b0 
       Offset added to Trained Rx DQS Value.
     */
    UINT32 txdqoffset : 6;
    /* txdqoffset - Bits[18:13], RW_L, default = 2'b00 
       Offset added to Trained Tx DQ Value.
     */
    UINT32 txdqsoffset : 6;
    /* txdqsoffset - Bits[24:19], RW_L, default = 6'b000000 
       Offset added to Trained Tx DQS Value.
     */
    UINT32 vrefoffset : 7;
    /* vrefoffset - Bits[31:25], RW_L, default = 7'b0000000 
       unsiged offset added to Trained Vref Value. each step is Vdd/192.
     */
  } Bits;
  UINT32 Data;
} DATAOFFSETTRAIN_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* PXCATTACK0SEL_7_MCIO_DDRIOEXT_REG supported on:                            */
/*       BDX (0x401A52E4)                                                     */
/* Register default value:              0x00FAC688                            */
#define PXCATTACK0SEL_7_MCIO_DDRIOEXT_REG 0x0D0342E4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Select 2 aggressors out 3 neighbors in a nibble
 */
typedef union {
  struct {
    UINT32 attackr0d0 : 3;
    /* attackr0d0 - Bits[2:0], RW_L, default = 3'd0 
       xtalk aggressor0 select for dq bit[0] in a byte
     */
    UINT32 attackr0d1 : 3;
    /* attackr0d1 - Bits[5:3], RW_L, default = 3'd1 
       xtalk aggressor0 select for dq bit[1] in a byte
     */
    UINT32 attackr0d2 : 3;
    /* attackr0d2 - Bits[8:6], RW_L, default = 3'd2 
       xtalk aggressor0 select for dq bit[2] in a byte
     */
    UINT32 attackr0d3 : 3;
    /* attackr0d3 - Bits[11:9], RW_L, default = 3'd3 
       xtalk aggressor0 select for dq bit[3] in a byte
     */
    UINT32 attackr0d4 : 3;
    /* attackr0d4 - Bits[14:12], RW_L, default = 3'd4 
       xtalk aggressor0 select for dq bit[4] in a byte
     */
    UINT32 attackr0d5 : 3;
    /* attackr0d5 - Bits[17:15], RW_L, default = 3'd5 
       xtalk aggressor0 select for dq bit[5] in a byte
     */
    UINT32 attackr0d6 : 3;
    /* attackr0d6 - Bits[20:18], RW_L, default = 3'd6 
       xtalk aggressor0 select for dq bit[6] in a byte
     */
    UINT32 attackr0d7 : 3;
    /* attackr0d7 - Bits[23:21], RW_L, default = 3'd7 
       xtalk aggressor0 select for dq bit[7] in a byte
     */
    UINT32 spare : 8;
    /* spare - Bits[31:24], RW_L, default = 8'b0 
       spare bits.
     */
  } Bits;
  UINT32 Data;
} PXCATTACK0SEL_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATACONTROL0_7_MCIO_DDRIOEXT_REG supported on:                             */
/*       HSX (0x401A52E8)                                                     */
/*       BDX (0x401A52E8)                                                     */
/* Register default value:              0x00000000                            */
#define DATACONTROL0_7_MCIO_DDRIOEXT_REG 0x0D0342E8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rxtrainingmode : 1;
    /* rxtrainingmode - Bits[0:0], RW_L, default = 1'b0 
       Enables Read DqDqs Training Mode
     */
    UINT32 wltrainingmode : 1;
    /* wltrainingmode - Bits[1:1], RW_L, default = 1'b0 
       Enables Write Leveling Training Mode
     */
    UINT32 rltrainingmode : 1;
    /* rltrainingmode - Bits[2:2], RW_L, default = 1'b0 
       Enables Read Leveling Training Mode.
     */
    UINT32 senseamptrainingmode : 1;
    /* senseamptrainingmode - Bits[3:3], RW_L, default = 1'b0 
       Enables SenseAmp offset cancellation training mode.
     */
    UINT32 rsvd_4 : 1;
    UINT32 rfon : 1;
    /* rfon - Bits[5:5], RW_L, default = 1'b0 
       Power-down Disable:  Forces register file read, overriding the rank power down 
       logic. 
     */
    UINT32 rsvd_6 : 1;
    UINT32 txpion : 1;
    /* txpion - Bits[7:7], RW_L, default = 1'b0 
       Power-down Disable:  Forces TxDq and TxDqs PI clocks on.
     */
    UINT32 rsvd_8 : 1;
    UINT32 dcdetectmode : 1;
    /* dcdetectmode - Bits[9:9], RW_L, default = 1'b0 
       Enable VMSE DC detect mode.
     */
    UINT32 rsvd_10 : 2;
    UINT32 txlong : 1;
    /* txlong - Bits[12:12], RW_L, default = 1'b0 
       Sets the duration of the first transmitted DQ bit of the burst. 0=1UI, 1=2UI.
     */
    UINT32 rsvd_13 : 5;
    UINT32 driversegmentenable : 1;
    /* driversegmentenable - Bits[18:18], RW_L, default = 1'b0 
       Controls which segements of the driver are enabled:  {0: All, 1: Only ODT}.
     */
    UINT32 rsvd_19 : 1;
    UINT32 readrfrd : 1;
    /* readrfrd - Bits[20:20], RW_L, default = 1'b0 
       Self clearing command bit. When set, it will download the three 32 bit lines 
       from the RegFile associated with a read c 
       ommand to Rank = ReadRFRank
     */
    UINT32 readrfwr : 1;
    /* readrfwr - Bits[21:21], RW_L, default = 1'b0 
       Self clearing command bit. When set, it will download the three 32 bit lines 
       from the RegFile associated with a write  
       command to Rank = ReadRFRank
     */
    UINT32 readrfrank : 3;
    /* readrfrank - Bits[24:22], RW_L, default = 3'b000 
       Specifies the rank that ReadRFRd or ReadRFWr will manually download the CR 
       values from 
     */
    UINT32 forceodton : 1;
    /* forceodton - Bits[25:25], RW_L, default = 1'b0 
       ODT is forced-on. NOTE: Cannot be set if LongODTR2W is set or can create glitch 
       in transmitter.   NOTE: If forceodton = 1, odtduration should be set to 0 
     */
    UINT32 rsvd_26 : 3;
    UINT32 burstlength4 : 1;
    /* burstlength4 - Bits[29:29], RW_L, default = 1'b0 
       enable lockstep mode, burst length 4
     */
    UINT32 xovercal : 1;
    /* xovercal - Bits[30:30], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
    UINT32 longpreambleenable : 1;
    /* longpreambleenable - Bits[31:31], RW_L, default = 1'b0 
       Enables long preamble for DDR4 devices
     */
  } Bits;
  UINT32 Data;
} DATACONTROL0_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATACONTROL1_7_MCIO_DDRIOEXT_REG supported on:                             */
/*       HSX (0x401A52EC)                                                     */
/*       BDX (0x401A52EC)                                                     */
/* Register default value:              0x00000000                            */
#define DATACONTROL1_7_MCIO_DDRIOEXT_REG 0x0D0342EC
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 7;
    UINT32 sdlldisable : 2;
    /* sdlldisable - Bits[8:7], RW_L, default = 2'b00 
       Controls which slave DLL are disabled: used in VMSE half-width mode to turn off 
       the unused slave delay line. 
                 Set to 0 to have nothing disabled.
                 Set to 1 to have nibble0 disabled.
                 Set to 2 to have nibble1 disabled.
                 Set to 3 to have nibble0 and nibble 1 disabled.
               
     */
    UINT32 rsvd_9 : 1;
    UINT32 rxbiasctl : 3;
    /* rxbiasctl - Bits[12:10], RW_L, default = 1'b0 
       Controls the Current and BW of the Receiver: 
                 {0: 0.34Idll, 1: 0.66Idll, 2: 0.88Idll, 3: Idll, 4: 1.33Idll, 5: 
       1.66Idll, 6: 2.00Idll, 7: 2.33Idll} 
     */
    UINT32 odtdelay : 4;
    /* odtdelay - Bits[16:13], RW_L, default = 4'b0000 
       Controls ODT turn-on delay.   To turn on N tQCK before RcvEn, program to 
       RcvEn[8:6]  N + 4. 
     */
    UINT32 odtduration : 3;
    /* odtduration - Bits[19:17], RW_L, default = 4'b0000 
       Controls the ODT ON duration from (Code, Duration) = (0x0, 11 tQCK) to (0x7, 18 
       tQCK).  NOTE: If forceodton = 1, this should be set to 0 
     */
    UINT32 senseampdelay : 4;
    /* senseampdelay - Bits[23:20], RW_L, default = 4'b0000 
       Controls Senseamp turn-on delay.   To turn on N tQCK before RcvEn, program to 
       RcvEn[8:6]  N + 4. 
     */
    UINT32 senseampduration : 3;
    /* senseampduration - Bits[26:24], RW_L, default = 4'b0000 
       Controls the SenseAmp ON duration from (Code, Duration) = (0x0, 11 tQCK) to 
       (0x7, 18 tQCK) 
     */
    UINT32 burstendodtdelay : 3;
    /* burstendodtdelay - Bits[29:27], RW_L, default = 3'b000 
       Cycles after the burst, when the current pulse should turn on by enabling On-die 
       Terminators 
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} DATACONTROL1_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 7;
    UINT32 sdlldisable : 2;
    /* sdlldisable - Bits[8:7], RW_L, default = 2'b00 
       Controls which slave DLL are disabled: used in vmse half-width mode to turn off 
       the unused slave delay line. 
                 Set to 0 to have nothing disabled.
                 Set to 1 to have nibble0 disabled.
                 Set to 2 to have nibble1 disabled.
                 Set to 3 to have nibble0 and nibble 1 disabled.
               
     */
    UINT32 rsvd_9 : 1;
    UINT32 rxbiasctl : 3;
    /* rxbiasctl - Bits[12:10], RW_L, default = 1'b0 
       Controls the Current and BW of the Receiver: 
                 {0: 0.34Idll, 1: 0.66Idll, 2: 0.88Idll, 3: Idll, 4: 1.33Idll, 5: 
       1.66Idll, 6: 2.00Idll, 7: 2.33Idll} 
     */
    UINT32 odtdelay : 4;
    /* odtdelay - Bits[16:13], RW_L, default = 4'b0000 
       Controls ODT turn-on delay.   To turn on N tQCK before RcvEn, program to 
       RcvEn[8:6]  N + 4. 
     */
    UINT32 odtduration : 3;
    /* odtduration - Bits[19:17], RW_L, default = 4'b0000 
       Controls the ODT ON duration from (Code, Duration) = (0x0, 11 tQCK) to (0x7, 18 
       tQCK).  NOTE: If forceodton = 1, this should be set to 0 
     */
    UINT32 senseampdelay : 4;
    /* senseampdelay - Bits[23:20], RW_L, default = 4'b0000 
       Controls Senseamp turn-on delay.   To turn on N tQCK before RcvEn, program to 
       RcvEn[8:6]  N + 4. 
     */
    UINT32 senseampduration : 3;
    /* senseampduration - Bits[26:24], RW_L, default = 4'b0000 
       Controls the SenseAmp ON duration from (Code, Duration) = (0x0, 11 tQCK) to 
       (0x7, 18 tQCK) 
     */
    UINT32 burstendodtdelay : 3;
    /* burstendodtdelay - Bits[29:27], RW_L, default = 3'b000 
       Cycles after the burst, when the current pulse should turn on by enabling On-die 
       Terminators 
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} DATACONTROL1_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DATACONTROL2_7_MCIO_DDRIOEXT_REG supported on:                             */
/*       HSX (0x401A52F0)                                                     */
/*       BDX (0x401A52F0)                                                     */
/* Register default value:              0x00000000                            */
#define DATACONTROL2_7_MCIO_DDRIOEXT_REG 0x0D0342F0
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rxstaggerctl : 5;
    /* rxstaggerctl - Bits[4:0], RW_L, default = 5'b0 
       Delay DdrRxEnableQnnnH by RxStaggerCtl qclks (0-31) before turning on/off the 
       RxAmps  
               If RxStaggerCtl = 0, use the ODT Delay/Duration settings for turning 
       on/off the Rx amplifier  
     */
    UINT32 forcebiason : 1;
    /* forcebiason - Bits[5:5], RW_L, default = 1'b0 
       Force on the internal Vref and Rx bias circuit, regardless of any other power 
       downs 
     */
    UINT32 forcerxon : 1;
    /* forcerxon - Bits[6:6], RW_L, default = 1'b0 
       Force on the RxAmp only (as opposed to OdtSampOn, which turns on both ODT and 
       the amplifier). 
               Use for overclocking support where we may not be able to hit the fast 
       exit latency  
     */
    UINT32 rsvd_7 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[8:8], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/VMSE
     */
    UINT32 imodecfg : 1;
    /* imodecfg - Bits[9:9], RW_L, default = None 
       Imode Select, 0: Swing Boost Mode; 1: Xtalk Cancellation Mode
     */
    UINT32 imodeenable : 1;
    /* imodeenable - Bits[10:10], RW_L, default = None 
       Enable Imode EQ per byte lane. 
                 0: Imode completely off 
                 1: Imode enabled (Xtalk cancellation OR swing boost selected by 
       ImodeCfg. 
                 If enabled, the bit has to be turned on after ImodeBiasEnable turned 
       on. 
               
     */
    UINT32 imodeeqcode : 4;
    /* imodeeqcode - Bits[14:11], RW_L, default = None 
       Binary Imode Coefficient, sets Imode current strength. 
                 (used for either Xtalk cancellation or swing boost)
                 Code   Imode Current (approx)
                 ------------------
                 0000        0  
                 0001        1
                 0010        2 
                   *  
                   * 
                   *
                 1111        3
     */
    UINT32 imodebiasen : 1;
    /* imodebiasen - Bits[15:15], RW_L, default = None 
       Enables Imode Bias circuit. It must be enabled before ImodeEnable is turned on.
     */
    UINT32 imodescomp : 6;
    /* imodescomp - Bits[21:16], RW_L, default = None 
       Scomp code for Imode circuit
     */
    UINT32 imodescompovrd : 1;
    /* imodescompovrd - Bits[22:22], RW_L, default = None 
       Imode Scomp Override Enable
     */
    UINT32 rxdqssaoffset : 5;
    /* rxdqssaoffset - Bits[27:23], RW_L, default = None 
       strobe Rx Amp offset control
     */
    UINT32 rsvd_28 : 2;
    UINT32 xtalklowpwrenb : 1;
    /* xtalklowpwrenb - Bits[30:30], RW_L, default = 1'b0 
       disables xtalk toggle detect, also used to set narrow operation range for 
       internal  
                 Vref gen circuit. when narrow range operation is needed, it is 
       expected that bios will program 
                 this bit to '1' first and, when Vref is settled, bios program Vref 
       levelshifter hold cr, which 
                 latch the setting. Afterwards, this CR can be used to set xtalk 
       lowpower disable or not. 
               
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DATACONTROL2_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rxstaggerctl : 5;
    /* rxstaggerctl - Bits[4:0], RW_L, default = 5'b0 
       Delay DdrRxEnableQnnnH by RxStaggerCtl qclks (0-31) before turning on/off the 
       RxAmps  
               If RxStaggerCtl = 0, use the ODT Delay/Duration settings for turning 
       on/off the Rx amplifier  
     */
    UINT32 forcebiason : 1;
    /* forcebiason - Bits[5:5], RW_L, default = 1'b0 
       Force on the internal Vref and Rx bias circuit, regardless of any other power 
       downs 
     */
    UINT32 forcerxon : 1;
    /* forcerxon - Bits[6:6], RW_L, default = 1'b0 
       Force on the RxAmp only (as opposed to OdtSampOn, which turns on both ODT and 
       the amplifier). 
               Use for overclocking support where we may not be able to hit the fast 
       exit latency  
     */
    UINT32 rsvd_7 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[8:8], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/VMSE
     */
    UINT32 imodecfg : 1;
    /* imodecfg - Bits[9:9], RW_L, default = None 
       Imode Select, 0: Swing Boost Mode; 1: Xtalk Cancellation Mode
     */
    UINT32 imodeenable : 1;
    /* imodeenable - Bits[10:10], RW_L, default = None 
       Enable Imode EQ per byte lane. 
                 0: Imode completely off 
                 1: Imode enabled (Xtalk cancellation OR swing boost selected by 
       ImodeCfg. 
                 If enabled, the bit has to be turned on after ImodeBiasEnable turned 
       on. 
               
     */
    UINT32 imodeeqcode : 4;
    /* imodeeqcode - Bits[14:11], RW_L, default = None 
       Binary Imode Coefficient, sets Imode current strength. 
                 (used for either Xtalk cancellation or swing boost)
                 Code   Imode Current (approx)
                 ------------------
                 0000        0  
                 0001        1
                 0010        2 
                   *  
                   * 
                   *
                 1111        3
     */
    UINT32 imodebiasen : 1;
    /* imodebiasen - Bits[15:15], RW_L, default = None 
       Enables Imode Bias circuit. It must be enabled before ImodeEnable is turned on.
     */
    UINT32 imodescomp : 6;
    /* imodescomp - Bits[21:16], RW_L, default = None 
       Scomp code for Imode circuit
     */
    UINT32 imodescompovrd : 1;
    /* imodescompovrd - Bits[22:22], RW_L, default = None 
       Imode Scomp Override Enable
     */
    UINT32 rxdqssaoffset : 5;
    /* rxdqssaoffset - Bits[27:23], RW_L, default = None 
       strobe Rx Amp offset control
     */
    UINT32 rsvd_28 : 2;
    UINT32 xtalklowpwrenb : 1;
    /* xtalklowpwrenb - Bits[30:30], RW_L, default = 1'b0 
       disables xtalk toggle detect, also used to set narrow operation range for 
       internal  
                 Vref gen circuit. when narrow range operation is needed, it is 
       expected that bios will program 
                 this bit to '1' first and, when Vref is settled, bios program Vref 
       levelshifter hold cr, which 
                 latch the setting. Afterwards, this CR can be used to set xtalk 
       lowpower disable or not. 
               
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DATACONTROL2_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DATACONTROL3_7_MCIO_DDRIOEXT_REG supported on:                             */
/*       HSX (0x401A52F4)                                                     */
/*       BDX (0x401A52F4)                                                     */
/* Register default value:              0x8184924924F                         */
#define DATACONTROL3_7_MCIO_DDRIOEXT_REG 0x0D0342F4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * static configurations of tx and rx.
 */
typedef union {
  struct {
    UINT32 drvstaticlegcfg : 2;
    /* drvstaticlegcfg - Bits[1:0], RW_L, default = 2'b11 
       binary default setting for static legs in DRV mode
                  0    0       0 legs (EQ disabled)
                  0    1       3 legs (EQ max 3 legs)
                  1    0       6 legs (EQ max 6 legs)
                  1    1      12 legs (EQ max 12 legs)
                 In most cases, we would set this to be 11, but for some configurations 
       that require a higher  
                 Ron value, we need to reduce the number of static legs enabled so that 
       fast corner can hit  
                 the higher Ron target value.
     */
    UINT32 odtstaticlegcfg : 2;
    /* odtstaticlegcfg - Bits[3:2], RW_L, default = 2'b11 
       binary default setting for static legs in ODT mode
                  0    0       0 legs (EQ disabled)
                  0    1       3 legs (EQ max 3 legs)
                  1    0       6 legs (EQ max 6 legs)
                  1    1      12 legs (EQ max 12 legs)
                 In most cases, we would set this to be 11, but for some configurations 
       that require a higher  
                 Ron value, we need to reduce the number of static legs enabled so that 
       fast corner can hit  
                 the higher Ron target value.
     */
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[4:4], RW_L, default = 1'b0 
       level shift hold enable
                            Holds the HV control values and power down the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rsvd_5 : 1;
    UINT32 odtsegmentenable : 3;
    /* odtsegmentenable - Bits[8:6], RW_L, default = 3'b001 
       This will tell which  segments are enabled out of reset.  
                            After that, a shift register will determine the enabled 
       segments 
     */
    UINT32 datasegmentenable : 3;
    /* datasegmentenable - Bits[11:9], RW_L, default = 3'b001 
       This will tell which  segments are enabled out of reset. 
     */
    UINT32 longodtr2w : 1;
    /* longodtr2w - Bits[12:12], RW_L, default = 3'b001 
       When set, it will extends ODT by 1 qclk on read to write turnarounds. NOTE: 
       Cannot be set if ForceODTOn is set or can create glitch in transmitter. 
     */
    UINT32 imodebiashighcm : 1;
    /* imodebiashighcm - Bits[13:13], RW_L, default = 3'b001 
       imodebias control 
     */
    UINT32 imodebiasvrefen : 1;
    /* imodebiasvrefen - Bits[14:14], RW_L, default = 3'b001 
       imodebias control 
     */
    UINT32 imodebiasrxbiastrim : 3;
    /* imodebiasrxbiastrim - Bits[17:15], RW_L, default = 3'b001 
       imodebias control. Also used for rxbiasana 
     */
    UINT32 rsvd_18 : 3;
    UINT32 ddrcrctlecapen : 2;
    /* ddrcrctlecapen - Bits[22:21], RW_L, default = 2'b00 
       ctle capacitor setting 
     */
    UINT32 ddrcrctleresen : 2;
    /* ddrcrctleresen - Bits[24:23], RW_L, default = 2'b11 
       ctle resistor setting 
     */
    UINT32 pcasbiasclosedloopen : 1;
    /* pcasbiasclosedloopen - Bits[25:25], RW_L, default = 2'b00 
       when enabled, choose close loop for the pcas bias loop in Rxbias
     */
    UINT32 rxbiassel : 1;
    /* rxbiassel - Bits[26:26], RW_L, default = 2'b00 
       only used in CTL fub. when enabled, it turns on part of Rxbias for imodebias.
     */
    UINT32 rxbiasfoldedlegtrim : 2;
    /* rxbiasfoldedlegtrim - Bits[28:27], RW_L, default = 2'b00 
       adjust common mode in the first stage.
     */
    UINT32 rxbiasgcncomp : 2;
    /* rxbiasgcncomp - Bits[30:29], RW_L, default = 2'b01 
       used to compensate the GCN skew. The GCN skew information should be
                 extracted by reading and analyzing the final Rcomp code value. The 
       target spec 
                 is to categorize the GCN skew in 3 process condition (mainly slow, 
       fast, typical). 
                 (one example can be just reading the 2 MLS bit of Rcomp code and 
       determine if the 
                 GCN is slow, fast or typical). The gcncomp is used for CTLE.
                 11: gcn in slow corner
                 10: gcn in typical corner
                 01: gcn in fast corner
               
     */
    UINT32 postambleenable : 1;
    /* postambleenable - Bits[31:31], RW_L, default = None 
       VMSE: enables long postamble and a dummy toggle
     */
  } Bits;
  UINT32 Data;
} DATACONTROL3_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* VSSHIORVREFCONTROL_7_MCIO_DDRIOEXT_REG supported on:                       */
/*       HSX (0x401A52F8)                                                     */
/*       BDX (0x401A52F8)                                                     */
/* Register default value:              0x004D8238                            */
#define VSSHIORVREFCONTROL_7_MCIO_DDRIOEXT_REG 0x0D0342F8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * VssHi Controls on Channel 0 only. DIMM Vref Controls on  Channel1, databyte 7, 
 * only. 
 */
typedef union {
  struct {
    UINT32 vsshiorvrefctl : 24;
    /* vsshiorvrefctl - Bits[23:0], RW_L, default = 24'h4d8238 
         VssHi adjust setting.
                Field          Bits    Description
               -------         ----    
       ----------------------------------------------------------------------------- 
               SelCode          23     Selects the code to output:  {0: Code, 1: TCode}
               GainBoost        22     Enable gain boost is panic mode by updating 
       code/tcode    
               PanicVoltage  21:18  Unsigned integer controlling the voltage error on 
       VssHi that will trigger the panic driver.  Step size of VccDDQ/192 (0 to 117mV) 
               CloseLoop        17     Enables the VssHi close loop tracking 
               PanicEn          16     Enables the VssHi Panic Driver
               BWError          15:14  Limits the loop gain (ie: difference between 
       code and tcode) to +/- 2^(SlowBWError) 
               OpenLoop         13     Operates VssHi in open loop mode with the target 
       Vref code directly driving the output DAC with no feedback involved 
               SampleDivider    12:10  Controls the loop bandwidth by dividing down the 
       input clock: {0: Qclk, 1: Qclk/2, 2: Qclk/4, 3: Qclk/8, 4: Qclk/16, 5: Qclk/32, 
       6: Qclk/64, 7: Qclk/128} 
               LoBWDivider      9:8    For the slow integrator, selects the net number 
       of samples in a given direction to trip the filter.  {0: 4 samples, 1: 8 
       samples, 2: 16 samples, 3: 32 samples} 
               HiBWDivider      7:6    For the fast integrator, selects the net number 
       of samples in a given direction to trip the filter.  {0: 4 samples, 1: 8 
       samples, 2: 16 samples, 3: 32 samples} 
               Target           5:0    Unsigned integer controlling the target VssHi 
       voltage.  Step size of VccDDQ/192 and starting value of VccDDQ*20/192 
               
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} VSSHIORVREFCONTROL_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* RXGROUP0N1RANK0_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5300)                                                     */
/*       BDX (0x401A5300)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK0_7_MCIO_DDRIOEXT_REG 0x0D034300

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank0
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK0_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK1_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5304)                                                     */
/*       BDX (0x401A5304)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK1_7_MCIO_DDRIOEXT_REG 0x0D034304

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank1
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK1_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK2_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5308)                                                     */
/*       BDX (0x401A5308)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK2_7_MCIO_DDRIOEXT_REG 0x0D034308

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank2
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK2_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK3_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A530C)                                                     */
/*       BDX (0x401A530C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK3_7_MCIO_DDRIOEXT_REG 0x0D03430C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank3
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK3_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK4_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5310)                                                     */
/*       BDX (0x401A5310)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK4_7_MCIO_DDRIOEXT_REG 0x0D034310

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank4
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK4_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK5_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5314)                                                     */
/*       BDX (0x401A5314)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK5_7_MCIO_DDRIOEXT_REG 0x0D034314

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank5
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK5_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK6_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5318)                                                     */
/*       BDX (0x401A5318)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK6_7_MCIO_DDRIOEXT_REG 0x0D034318

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank6
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK6_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK7_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A531C)                                                     */
/*       BDX (0x401A531C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK7_7_MCIO_DDRIOEXT_REG 0x0D03431C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank7
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK7_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK0_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5320)                                                     */
/*       BDX (0x401A5320)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK0_7_MCIO_DDRIOEXT_REG 0x0D034320

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK0_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK1_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5324)                                                     */
/*       BDX (0x401A5324)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK1_7_MCIO_DDRIOEXT_REG 0x0D034324

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK1_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK2_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5328)                                                     */
/*       BDX (0x401A5328)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK2_7_MCIO_DDRIOEXT_REG 0x0D034328

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK2_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK3_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A532C)                                                     */
/*       BDX (0x401A532C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK3_7_MCIO_DDRIOEXT_REG 0x0D03432C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK3_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK4_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5330)                                                     */
/*       BDX (0x401A5330)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK4_7_MCIO_DDRIOEXT_REG 0x0D034330

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK4_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK5_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5334)                                                     */
/*       BDX (0x401A5334)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK5_7_MCIO_DDRIOEXT_REG 0x0D034334

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK5_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK6_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5338)                                                     */
/*       BDX (0x401A5338)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK6_7_MCIO_DDRIOEXT_REG 0x0D034338

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK6_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK7_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A533C)                                                     */
/*       BDX (0x401A533C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK7_7_MCIO_DDRIOEXT_REG 0x0D03433C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK7_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP0N1RANK0_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5340)                                                     */
/*       BDX (0x401A5340)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK0_7_MCIO_DDRIOEXT_REG 0x0D034340
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK0_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK0_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK1_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5344)                                                     */
/*       BDX (0x401A5344)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK1_7_MCIO_DDRIOEXT_REG 0x0D034344
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK1_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK1_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK2_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5348)                                                     */
/*       BDX (0x401A5348)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK2_7_MCIO_DDRIOEXT_REG 0x0D034348
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK2_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK2_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK3_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A534C)                                                     */
/*       BDX (0x401A534C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK3_7_MCIO_DDRIOEXT_REG 0x0D03434C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK3_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK3_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK4_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5350)                                                     */
/*       BDX (0x401A5350)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK4_7_MCIO_DDRIOEXT_REG 0x0D034350
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK4_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK4_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK5_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5354)                                                     */
/*       BDX (0x401A5354)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK5_7_MCIO_DDRIOEXT_REG 0x0D034354
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK5_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK5_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK6_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5358)                                                     */
/*       BDX (0x401A5358)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK6_7_MCIO_DDRIOEXT_REG 0x0D034358
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK6_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK6_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK7_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A535C)                                                     */
/*       BDX (0x401A535C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK7_7_MCIO_DDRIOEXT_REG 0x0D03435C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK7_7_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK7_7_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP1N1RANK0_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5360)                                                     */
/*       BDX (0x401A5360)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK0_7_MCIO_DDRIOEXT_REG 0x0D034360

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK0_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK1_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5364)                                                     */
/*       BDX (0x401A5364)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK1_7_MCIO_DDRIOEXT_REG 0x0D034364

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK1_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK2_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5368)                                                     */
/*       BDX (0x401A5368)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK2_7_MCIO_DDRIOEXT_REG 0x0D034368

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK2_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK3_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A536C)                                                     */
/*       BDX (0x401A536C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK3_7_MCIO_DDRIOEXT_REG 0x0D03436C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK3_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK4_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5370)                                                     */
/*       BDX (0x401A5370)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK4_7_MCIO_DDRIOEXT_REG 0x0D034370

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK4_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK5_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5374)                                                     */
/*       BDX (0x401A5374)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK5_7_MCIO_DDRIOEXT_REG 0x0D034374

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK5_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK6_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5378)                                                     */
/*       BDX (0x401A5378)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK6_7_MCIO_DDRIOEXT_REG 0x0D034378

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK6_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK7_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A537C)                                                     */
/*       BDX (0x401A537C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK7_7_MCIO_DDRIOEXT_REG 0x0D03437C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK7_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK0_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5380)                                                     */
/*       BDX (0x401A5380)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK0_7_MCIO_DDRIOEXT_REG 0x0D034380

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK0_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK1_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5384)                                                     */
/*       BDX (0x401A5384)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK1_7_MCIO_DDRIOEXT_REG 0x0D034384

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK1_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK2_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5388)                                                     */
/*       BDX (0x401A5388)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK2_7_MCIO_DDRIOEXT_REG 0x0D034388

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK2_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK3_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A538C)                                                     */
/*       BDX (0x401A538C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK3_7_MCIO_DDRIOEXT_REG 0x0D03438C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK3_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK4_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5390)                                                     */
/*       BDX (0x401A5390)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK4_7_MCIO_DDRIOEXT_REG 0x0D034390

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK4_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK5_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5394)                                                     */
/*       BDX (0x401A5394)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK5_7_MCIO_DDRIOEXT_REG 0x0D034394

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK5_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK6_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5398)                                                     */
/*       BDX (0x401A5398)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK6_7_MCIO_DDRIOEXT_REG 0x0D034398

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK6_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK7_7_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A539C)                                                     */
/*       BDX (0x401A539C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK7_7_MCIO_DDRIOEXT_REG 0x0D03439C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK7_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXXTALKN1_7_MCIO_DDRIOEXT_REG supported on:                                */
/*       HSX (0x401A53A0)                                                     */
/*       BDX (0x401A53A0)                                                     */
/* Register default value:              0x00000000                            */
#define TXXTALKN1_7_MCIO_DDRIOEXT_REG 0x0D0343A0

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * not used.
 */
typedef union {
  struct {
    UINT32 twototwologicdelayrank0 : 4;
    /* twototwologicdelayrank0 - Bits[3:0], RW_L, default = 4'b0000 
       This may need to be set for bits 3:0 in the nibble for Rank 0
     */
    UINT32 twototwologicdelayrank1 : 4;
    /* twototwologicdelayrank1 - Bits[7:4], RW_L, default = 4'b0000 
       This may need to be set for bits 3:0 in the nibble for Rank 1
     */
    UINT32 twototwologicdelayrank2 : 4;
    /* twototwologicdelayrank2 - Bits[11:8], RW_L, default = 4'b0000 
       This may need to be set for bits 3:0 in the nibble for Rank 2
     */
    UINT32 twototwologicdelayrank3 : 4;
    /* twototwologicdelayrank3 - Bits[15:12], RW_L, default = 4'b0000 
       This may need to be set for bits 3:0 in the nibble for Rank 3
     */
    UINT32 twototwologicdelayrank4 : 4;
    /* twototwologicdelayrank4 - Bits[19:16], RW_L, default = 4'b0000 
       This may need to be set for bits 3:0 in the nibble for Rank 4
     */
    UINT32 twototwologicdelayrank5 : 4;
    /* twototwologicdelayrank5 - Bits[23:20], RW_L, default = 4'b0000 
       This may need to be set for bits 3:0 in the nibble for rank 5
     */
    UINT32 twototwologicdelayrank6 : 4;
    /* twototwologicdelayrank6 - Bits[27:24], RW_L, default = 4'b0000 
       This may need to be set for bits 3:0 in the nibble for rank 6
     */
    UINT32 twototwologicdelayrank7 : 4;
    /* twototwologicdelayrank7 - Bits[31:28], RW_L, default = 4'b0000 
       This may need to be set for bits 3:0 in the nibble for rank 7
     */
  } Bits;
  UINT32 Data;
} TXXTALKN1_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXVREFCTRLN1_7_MCIO_DDRIOEXT_REG supported on:                             */
/*       HSX (0x401A53A4)                                                     */
/*       BDX (0x401A53A4)                                                     */
/* Register default value:              0x00000000                            */
#define RXVREFCTRLN1_7_MCIO_DDRIOEXT_REG 0x0D0343A4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * RxVrefCtrl
 */
typedef union {
  struct {
    UINT32 vrefperbit0 : 7;
    /* vrefperbit0 - Bits[6:0], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben0 : 1;
    /* halflsben0 - Bits[7:7], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit1 : 7;
    /* vrefperbit1 - Bits[14:8], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben1 : 1;
    /* halflsben1 - Bits[15:15], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit2 : 7;
    /* vrefperbit2 - Bits[22:16], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben2 : 1;
    /* halflsben2 - Bits[23:23], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit3 : 7;
    /* vrefperbit3 - Bits[30:24], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben3 : 1;
    /* halflsben3 - Bits[31:31], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
  } Bits;
  UINT32 Data;
} RXVREFCTRLN1_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATATRAINFEEDBACKN1_7_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A53A8)                                                     */
/*       BDX (0x401A53A8)                                                     */
/* Register default value:              0x00000000                            */
#define DATATRAINFEEDBACKN1_7_MCIO_DDRIOEXT_REG 0x0D0343A8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data Training Feedback Results
 */
typedef union {
  struct {
    UINT32 datatrainfeedback : 9;
    /* datatrainfeedback - Bits[8:0], RW_L, default = 1'b0 
       Data Training Feedback Storage
     */
    UINT32 rsvd_9 : 23;
  } Bits;
  UINT32 Data;
} DATATRAINFEEDBACKN1_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATATCOCOMPOFFSET0_7_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A53AC)                                                     */
/* Register default value:              0x00000000                            */
#define DATATCOCOMPOFFSET0_7_MCIO_DDRIOEXT_REG 0x0D0343AC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 dq0tcocompoffset : 5;
    /* dq0tcocompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment offset added to Tco Comp Value.
     */
    UINT32 dq1tcocompoffset : 5;
    /* dq1tcocompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment offset added to Tco Comp Value.
     */
    UINT32 dq2tcocompoffset : 5;
    /* dq2tcocompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment offset added to DQ2 Tco Comp Value.
     */
    UINT32 dq3tcocompoffset : 5;
    /* dq3tcocompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment offset added to DQ3 Tco Comp Value.
     */
    UINT32 dqs0tcocompoffset : 5;
    /* dqs0tcocompoffset - Bits[24:20], RW_L, default = 4'b0000 
       2s Compliment offset added to DQ4 Tco Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATATCOCOMPOFFSET0_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATATCOCOMPOFFSET1_7_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A53B0)                                                     */
/* Register default value:              0x00000000                            */
#define DATATCOCOMPOFFSET1_7_MCIO_DDRIOEXT_REG 0x0D0343B0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Per Bit Tco Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4tcocompoffset : 5;
    /* dq4tcocompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment offset added to DQ4 Tco Comp Value.
     */
    UINT32 dq5tcocompoffset : 5;
    /* dq5tcocompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment offset added to Dq5 Tco Comp Value.
     */
    UINT32 dq6tcocompoffset : 5;
    /* dq6tcocompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment offset added to Dq6 Tco Comp Value.
     */
    UINT32 dq7tcocompoffset : 5;
    /* dq7tcocompoffset - Bits[19:15], RW_L, default = 1'b0 
       2s Compliment offset added to Dq7 Tco Comp Value.
     */
    UINT32 dqs1tcocompoffset : 5;
    /* dqs1tcocompoffset - Bits[24:20], RW_L, default = 1'b0 
       2s Compliment offset added to Dqs1 Tco Comp Value.
     */
    UINT32 enperbittcocomp : 1;
    /* enperbittcocomp - Bits[25:25], RW_L, default = 1'b0 
       Enable per bit Tco Comp flow.
     */
    UINT32 gentcocompdone : 1;
    /* gentcocompdone - Bits[26:26], RW_L, default = 1'b0 
       Generate TCO Comp done.
     */
    UINT32 spare : 5;
    /* spare - Bits[31:27], RW_L, default = 1'b0 
       Generate TCO Comp done.
     */
  } Bits;
  UINT32 Data;
} DATATCOCOMPOFFSET1_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET0_7_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A53B4)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET0_7_MCIO_DDRIOEXT_REG 0x0D0343B4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Per BIT ODT Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq0odtpupcompoffset : 5;
    /* dq0odtpupcompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq0 Odt Comp Value.
     */
    UINT32 dq1odtpupcompoffset : 5;
    /* dq1odtpupcompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq1 Odt Comp Value.
     */
    UINT32 dq2odtpupcompoffset : 5;
    /* dq2odtpupcompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to DQ2 Odt Comp Value.
     */
    UINT32 dq3odtpupcompoffset : 5;
    /* dq3odtpupcompoffset - Bits[19:15], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQ3 Odt Comp Value.
     */
    UINT32 dqs0odtpupcompoffset : 5;
    /* dqs0odtpupcompoffset - Bits[24:20], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQs0 Odt Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATAODTCOMPOFFSET0_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET1_7_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A53B8)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET1_7_MCIO_DDRIOEXT_REG 0x0D0343B8

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Per Bit Odt Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq0odtpdncompoffset : 5;
    /* dq0odtpdncompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq0 Odt Comp Value.
     */
    UINT32 dq1odtpdncompoffset : 5;
    /* dq1odtpdncompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq1 Odt Comp Value.
     */
    UINT32 dq2odtpdncompoffset : 5;
    /* dq2odtpdncompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to DQ2 Odt Comp Value.
     */
    UINT32 dq3odtpdncompoffset : 5;
    /* dq3odtpdncompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment Pull-down offset added to DQ3 Odt Comp Value.
     */
    UINT32 dqs0odtpdncompoffset : 5;
    /* dqs0odtpdncompoffset - Bits[24:20], RW_L, default = 4'b0000 
       2s Compliment Pull-down offset added to DQS0 Odt Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATAODTCOMPOFFSET1_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET2_7_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A53BC)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET2_7_MCIO_DDRIOEXT_REG 0x0D0343BC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Per Bit Odt Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4odtpupcompoffset : 5;
    /* dq4odtpupcompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq4 Odt Comp Value.
     */
    UINT32 dq5odtpupcompoffset : 5;
    /* dq5odtpupcompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq5 Odt Comp Value.
     */
    UINT32 dq6odtpupcompoffset : 5;
    /* dq6odtpupcompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to DQ6 Odt Comp Value.
     */
    UINT32 dq7odtpupcompoffset : 5;
    /* dq7odtpupcompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment Pull-up offset added to DQ7 Odt Comp Value.
     */
    UINT32 dqs1odtpupcompoffset : 5;
    /* dqs1odtpupcompoffset - Bits[24:20], RW_L, default = 2'b00 
       2s Compliment Pull-up offset added to DQs1 Odt Comp Value.
     */
    UINT32 enperbitodt : 1;
    /* enperbitodt - Bits[25:25], RW_L, default = 1'b0 
       Enable per bit ODT comp.
     */
    UINT32 genodtcompdone : 1;
    /* genodtcompdone - Bits[26:26], RW_L, default = 1'b0 
       Generate ODT comp Done.
     */
    UINT32 spare : 5;
    /* spare - Bits[31:27], RW_L, default = 1'b0 
       spare
     */
  } Bits;
  UINT32 Data;
} DATAODTCOMPOFFSET2_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET3_7_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A53C0)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET3_7_MCIO_DDRIOEXT_REG 0x0D0343C0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Per BIT ODT Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4odtpdncompoffset : 5;
    /* dq4odtpdncompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq4 Odt Comp Value.
     */
    UINT32 dq5odtpdncompoffset : 5;
    /* dq5odtpdncompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq5 Odt Comp Value.
     */
    UINT32 dq6odtpdncompoffset : 5;
    /* dq6odtpdncompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to DQ6 Odt Comp Value.
     */
    UINT32 dq7odtpdncompoffset : 5;
    /* dq7odtpdncompoffset - Bits[19:15], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQ7 Odt Comp Value.
     */
    UINT32 dqs1odtpdncompoffset : 5;
    /* dqs1odtpdncompoffset - Bits[24:20], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQs0 Odt Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATAODTCOMPOFFSET3_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET0_7_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A53C4)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET0_7_MCIO_DDRIOEXT_REG 0x0D0343C4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Per BIT RON Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq0ronpupcompoffset : 5;
    /* dq0ronpupcompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq0 Ron Comp Value.
     */
    UINT32 dq1ronpupcompoffset : 5;
    /* dq1ronpupcompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq1 Ron Comp Value.
     */
    UINT32 dq2ronpupcompoffset : 5;
    /* dq2ronpupcompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to DQ2 Ron Comp Value.
     */
    UINT32 dq3ronpupcompoffset : 5;
    /* dq3ronpupcompoffset - Bits[19:15], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQ3 Ron Comp Value.
     */
    UINT32 dqs0ronpupcompoffset : 5;
    /* dqs0ronpupcompoffset - Bits[24:20], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQs0 Ron Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATARONCOMPOFFSET0_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET1_7_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A53C8)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET1_7_MCIO_DDRIOEXT_REG 0x0D0343C8

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Per Bit RON Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq0ronpdncompoffset : 5;
    /* dq0ronpdncompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq0 Ron Comp Value.
     */
    UINT32 dq1ronpdncompoffset : 5;
    /* dq1ronpdncompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq1 Ron Comp Value.
     */
    UINT32 dq2ronpdncompoffset : 5;
    /* dq2ronpdncompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to DQ2 Ron Comp Value.
     */
    UINT32 dq3ronpdncompoffset : 5;
    /* dq3ronpdncompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment Pull-down offset added to DQ3 Ron Comp Value.
     */
    UINT32 dqs0ronpdncompoffset : 5;
    /* dqs0ronpdncompoffset - Bits[24:20], RW_L, default = 4'b0000 
       2s Compliment Pull-down offset added to DQS0 Ron Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATARONCOMPOFFSET1_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET2_7_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A53CC)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET2_7_MCIO_DDRIOEXT_REG 0x0D0343CC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Per Bit Ron Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4ronpupcompoffset : 5;
    /* dq4ronpupcompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq4 Ron Comp Value.
     */
    UINT32 dq5ronpupcompoffset : 5;
    /* dq5ronpupcompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq5 Ron Comp Value.
     */
    UINT32 dq6ronpupcompoffset : 5;
    /* dq6ronpupcompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to DQ6 Ron Comp Value.
     */
    UINT32 dq7ronpupcompoffset : 5;
    /* dq7ronpupcompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment Pull-up offset added to DQ7 Ron Comp Value.
     */
    UINT32 dqs1ronpupcompoffset : 5;
    /* dqs1ronpupcompoffset - Bits[24:20], RW_L, default = 2'b00 
       2s Compliment Pull-up offset added to DQs1 Ron Comp Value.
     */
    UINT32 enperbitron : 1;
    /* enperbitron - Bits[25:25], RW_L, default = 1'b0 
       Enable per bit RON comp.
     */
    UINT32 spare : 6;
    /* spare - Bits[31:26], RW_L, default = 1'b0 
       spare
     */
  } Bits;
  UINT32 Data;
} DATARONCOMPOFFSET2_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET3_7_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A53D0)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET3_7_MCIO_DDRIOEXT_REG 0x0D0343D0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Per BIT RON Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4ronpdncompoffset : 5;
    /* dq4ronpdncompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq4 Ron Comp Value.
     */
    UINT32 dq5ronpdncompoffset : 5;
    /* dq5ronpdncompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq5 Ron Comp Value.
     */
    UINT32 dq6ronpdncompoffset : 5;
    /* dq6ronpdncompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to DQ6 Ron Comp Value.
     */
    UINT32 dq7ronpdncompoffset : 5;
    /* dq7ronpdncompoffset - Bits[19:15], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQ7 Ron Comp Value.
     */
    UINT32 dqs1ronpdncompoffset : 5;
    /* dqs1ronpdncompoffset - Bits[24:20], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQs1 Ron Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATARONCOMPOFFSET3_7_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* RXGROUP0N0RANK0_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5400)                                                     */
/*       BDX (0x401A5400)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK0_8_MCIO_DDRIOEXT_REG 0x0D034400

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank0
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS (0: 1/16 UI 
       offset,  
                 1: 3/16 UI Offset, 2: 5/16 UI Offsets, 3: 7/16 UI Offsets, 4: 9/16 UI 
       Offsets, 5,6,7: 11/16 UI Offsets.) 
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK0_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK1_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5404)                                                     */
/*       BDX (0x401A5404)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK1_8_MCIO_DDRIOEXT_REG 0x0D034404

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank1
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK1_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK2_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5408)                                                     */
/*       BDX (0x401A5408)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK2_8_MCIO_DDRIOEXT_REG 0x0D034408

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank2
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK2_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK3_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A540C)                                                     */
/*       BDX (0x401A540C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK3_8_MCIO_DDRIOEXT_REG 0x0D03440C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank3
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK3_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK4_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5410)                                                     */
/*       BDX (0x401A5410)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK4_8_MCIO_DDRIOEXT_REG 0x0D034410

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank4
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK4_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK5_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5414)                                                     */
/*       BDX (0x401A5414)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK5_8_MCIO_DDRIOEXT_REG 0x0D034414

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank5
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK5_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK6_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5418)                                                     */
/*       BDX (0x401A5418)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK6_8_MCIO_DDRIOEXT_REG 0x0D034418

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank6
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK6_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK7_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A541C)                                                     */
/*       BDX (0x401A541C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK7_8_MCIO_DDRIOEXT_REG 0x0D03441C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank7
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 9'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 7'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 3'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N0RANK7_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK0_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5420)                                                     */
/*       BDX (0x401A5420)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK0_8_MCIO_DDRIOEXT_REG 0x0D034420

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK0_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK1_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5424)                                                     */
/*       BDX (0x401A5424)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK1_8_MCIO_DDRIOEXT_REG 0x0D034424

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK1_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK2_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5428)                                                     */
/*       BDX (0x401A5428)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK2_8_MCIO_DDRIOEXT_REG 0x0D034428

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK2_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK3_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A542C)                                                     */
/*       BDX (0x401A542C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK3_8_MCIO_DDRIOEXT_REG 0x0D03442C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK3_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK4_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5430)                                                     */
/*       BDX (0x401A5430)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK4_8_MCIO_DDRIOEXT_REG 0x0D034430

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK4_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK5_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5434)                                                     */
/*       BDX (0x401A5434)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK5_8_MCIO_DDRIOEXT_REG 0x0D034434

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK5_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK6_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5438)                                                     */
/*       BDX (0x401A5438)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK6_8_MCIO_DDRIOEXT_REG 0x0D034438

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK6_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK7_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A543C)                                                     */
/*       BDX (0x401A543C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK7_8_MCIO_DDRIOEXT_REG 0x0D03443C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N0RANK7_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP0N0RANK0_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5440)                                                     */
/*       BDX (0x401A5440)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK0_8_MCIO_DDRIOEXT_REG 0x0D034440
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK0_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK0_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK1_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5444)                                                     */
/*       BDX (0x401A5444)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK1_8_MCIO_DDRIOEXT_REG 0x0D034444
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK1_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK1_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK2_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5448)                                                     */
/*       BDX (0x401A5448)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK2_8_MCIO_DDRIOEXT_REG 0x0D034448
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on. 
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK2_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK2_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK3_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A544C)                                                     */
/*       BDX (0x401A544C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK3_8_MCIO_DDRIOEXT_REG 0x0D03444C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK3_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK3_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK4_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5450)                                                     */
/*       BDX (0x401A5450)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK4_8_MCIO_DDRIOEXT_REG 0x0D034450
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK4_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK4_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK5_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5454)                                                     */
/*       BDX (0x401A5454)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK5_8_MCIO_DDRIOEXT_REG 0x0D034454
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK5_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK5_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK6_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5458)                                                     */
/*       BDX (0x401A5458)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK6_8_MCIO_DDRIOEXT_REG 0x0D034458
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK6_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK6_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK7_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A545C)                                                     */
/*       BDX (0x401A545C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK7_8_MCIO_DDRIOEXT_REG 0x0D03445C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK7_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N0RANK7_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP1N0RANK0_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5460)                                                     */
/*       BDX (0x401A5460)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK0_8_MCIO_DDRIOEXT_REG 0x0D034460

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 6'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 6'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 6'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 6'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK0_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK1_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5464)                                                     */
/*       BDX (0x401A5464)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK1_8_MCIO_DDRIOEXT_REG 0x0D034464

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK1_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK2_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5468)                                                     */
/*       BDX (0x401A5468)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK2_8_MCIO_DDRIOEXT_REG 0x0D034468

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK2_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK3_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A546C)                                                     */
/*       BDX (0x401A546C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK3_8_MCIO_DDRIOEXT_REG 0x0D03446C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK3_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK4_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5470)                                                     */
/*       BDX (0x401A5470)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK4_8_MCIO_DDRIOEXT_REG 0x0D034470

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK4_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK5_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5474)                                                     */
/*       BDX (0x401A5474)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK5_8_MCIO_DDRIOEXT_REG 0x0D034474

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK5_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK6_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5478)                                                     */
/*       BDX (0x401A5478)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK6_8_MCIO_DDRIOEXT_REG 0x0D034478

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK6_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK7_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A547C)                                                     */
/*       BDX (0x401A547C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK7_8_MCIO_DDRIOEXT_REG 0x0D03447C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[0] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[1] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[2] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[3] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N0RANK7_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK0_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5480)                                                     */
/*       BDX (0x401A5480)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK0_8_MCIO_DDRIOEXT_REG 0x0D034480

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK0_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK1_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5484)                                                     */
/*       BDX (0x401A5484)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK1_8_MCIO_DDRIOEXT_REG 0x0D034484

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK1_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK2_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5488)                                                     */
/*       BDX (0x401A5488)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK2_8_MCIO_DDRIOEXT_REG 0x0D034488

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK2_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK3_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A548C)                                                     */
/*       BDX (0x401A548C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK3_8_MCIO_DDRIOEXT_REG 0x0D03448C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK3_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK4_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5490)                                                     */
/*       BDX (0x401A5490)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK4_8_MCIO_DDRIOEXT_REG 0x0D034490

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK4_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK5_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5494)                                                     */
/*       BDX (0x401A5494)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK5_8_MCIO_DDRIOEXT_REG 0x0D034494

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK5_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK6_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5498)                                                     */
/*       BDX (0x401A5498)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK6_8_MCIO_DDRIOEXT_REG 0x0D034498

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK6_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK7_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A549C)                                                     */
/*       BDX (0x401A549C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK7_8_MCIO_DDRIOEXT_REG 0x0D03449C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       [0]=0 selects the raw output from the nibble DQS amplifier; [1] is not used.
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN0RANK7_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* RXVREFCTRLN0_8_MCIO_DDRIOEXT_REG supported on:                             */
/*       HSX (0x401A54A4)                                                     */
/*       BDX (0x401A54A4)                                                     */
/* Register default value:              0x00000000                            */
#define RXVREFCTRLN0_8_MCIO_DDRIOEXT_REG 0x0D0344A4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * RxVrefCtrl
 */
typedef union {
  struct {
    UINT32 vrefperbit0 : 7;
    /* vrefperbit0 - Bits[6:0], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben0 : 1;
    /* halflsben0 - Bits[7:7], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit1 : 7;
    /* vrefperbit1 - Bits[14:8], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben1 : 1;
    /* halflsben1 - Bits[15:15], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit2 : 7;
    /* vrefperbit2 - Bits[22:16], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben2 : 1;
    /* halflsben2 - Bits[23:23], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit3 : 7;
    /* vrefperbit3 - Bits[30:24], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben3 : 1;
    /* halflsben3 - Bits[31:31], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
  } Bits;
  UINT32 Data;
} RXVREFCTRLN0_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATATRAINFEEDBACKN0_8_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A54A8)                                                     */
/*       BDX (0x401A54A8)                                                     */
/* Register default value:              0x00000000                            */
#define DATATRAINFEEDBACKN0_8_MCIO_DDRIOEXT_REG 0x0D0344A8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data Training Feedback Results
 */
typedef union {
  struct {
    UINT32 datatrainfeedback : 18;
    /* datatrainfeedback - Bits[17:0], RW_L, default = 1'b0 
       Data Training Feedback Storage
       
               DataTrainFeedback Description:
                Training Step    Bits    Nibble    Description
               --------------    ----    -------  
       -------------------------------------------------------------------------------------------------------------------------------------------------------- 
               Cross Over Cal     0      0        Tx Dqs Phase Detection
               Cross Over Cal     1      0        Tx Dq Phase Detection
               Cross Over Cal     2      0        Rec En Phase Detection
               RcvEn              8:0    0        Byte detection
               Rx Dq-Dqs          0      0        Pass/fail
               Wr Leveling        8:0    0        Byte detection
               Cross Over Cal     3      1        Tx Dqs Phase Detection
               Cross Over Cal     4      1        Tx Dq Phase Detection
               Cross Over Cal     5      1        Rec En Phase Detection
               RcvEn             17:9    1        Byte detection
               Rx Dq-Dqs          1      1        Pass/fail
               Wr Leveling       17:9    1        Byte detection
               
     */
    UINT32 rsvd_18 : 14;
  } Bits;
  UINT32 Data;
} DATATRAINFEEDBACKN0_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATACONTROL4_8_MCIO_DDRIOEXT_REG supported on:                             */
/*       HSX (0x401A54AC)                                                     */
/*       BDX (0x401A54AC)                                                     */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       <could not determine>                 */
#define DATACONTROL4_8_MCIO_DDRIOEXT_REG 0x0D0344AC
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * static configurations of tx and rx.
 */
typedef union {
  struct {
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[0:0], RW_L, default = None 
       enable grid clock divider in xover. used in native ddr mode for data rate 
       greater than 1867MT/s 
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[1:1], RW_L, default = None 
       enable PiRef clock divider in xover. used in vmse2to2mode, or whenever data rate 
       greater than 1867MT/s 
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[2:2], RW_L, default = None 
       enable extra pipestage in xover. POR setting = 1
     */
    UINT32 vmsehalfwidthmode : 1;
    /* vmsehalfwidthmode - Bits[3:3], RW_L, default = None 
       gate ClkPi when the nibble is disabled in VMSE half-width mode
     */
    UINT32 disablercvenextension : 1;
    /* disablercvenextension - Bits[4:4], RW_L, default = None 
       By default receive enable extension will be bypassed in VMSE2to1 mode. Setting 
       this to 1 in VMSE mode will use DDR to extend the rcven. VMSE ACIO LB use case 
       when only one scheduler is used. 
     */
    UINT32 ddrcrinvertpiclk : 2;
    /* ddrcrinvertpiclk - Bits[6:5], RW_L, default = 2'b00 
       invert ClkPiRef (per nibble) in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 
       phase. 
     */
    UINT32 dqvmselogicdelay : 8;
    /* dqvmselogicdelay - Bits[14:7], RW_L, default = 1'b0 
       DqVmseLogicDelay: If set add one UI delay to the Tx Fifo Read and DQ drivenable
     */
    UINT32 rcvenvmselogicdelay : 2;
    /* rcvenvmselogicdelay - Bits[16:15], RW_L, default = 1'b0 
       Receive enable VmseLogicDelay: If set add one UI delay to the receive enable
     */
    UINT32 dqsvmselogicdelay : 2;
    /* dqsvmselogicdelay - Bits[18:17], RW_L, default = 1'b0 
       DQS VMSE Logic Delay
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[19:19], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 ddrsroslmask : 1;
    /* ddrsroslmask - Bits[20:20], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 dqvmsenibblelogicdelay : 2;
    /* dqvmsenibblelogicdelay - Bits[22:21], RW_L, default = None 
       VMSE Logic delay that is shared for all bits in the nibble
     */
    UINT32 rsvd_23 : 9;
  } Bits;
  UINT32 Data;
} DATACONTROL4_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * static configurations of tx and rx.
 */
typedef union {
  struct {
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[0:0], RW_L, default = None 
       enable grid clock divider in xover. used in native ddr mode for data rate 
       greater than 1867MT/s 
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[1:1], RW_L, default = None 
       enable PiRef clock divider in xover. used in vmse2to2mode, or whenever data rate 
       greater than 1867MT/s 
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[2:2], RW_L, default = None 
       enable extra pipestage in xover. POR setting = 1
     */
    UINT32 vmsehalfwidthmode : 1;
    /* vmsehalfwidthmode - Bits[3:3], RW_L, default = None 
       gate ClkPi when the nibble is disabled in VMSE half-width mode
     */
    UINT32 disablercvenextension : 1;
    /* disablercvenextension - Bits[4:4], RW_L, default = None 
       By default receive enable extension will be bypassed in VMSE2to1 mode. Setting 
       this to 1 in VMSE mode will use DDR to extend the rcven. VMSE ACIO LB use case 
       when only one scheduler is used. 
     */
    UINT32 ddrcrinvertpiclk : 2;
    /* ddrcrinvertpiclk - Bits[6:5], RW_L, default = 2'b00 
       invert ClkPiRef (per nibble) in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 
       phase. 
     */
    UINT32 dqvmselogicdelay : 8;
    /* dqvmselogicdelay - Bits[14:7], RW_L, default = 1'b0 
       DqVmseLogicDelay: If set add one UI delay to the Tx Fifo Read and DQ drivenable
     */
    UINT32 rcvenvmselogicdelay : 2;
    /* rcvenvmselogicdelay - Bits[16:15], RW_L, default = 1'b0 
       Receive enable VmseLogicDelay: If set add one UI delay to the receive enable
     */
    UINT32 dqsvmselogicdelay : 2;
    /* dqsvmselogicdelay - Bits[18:17], RW_L, default = 1'b0 
       DQS VMSE Logic Delay
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[19:19], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 ddrsroslmask : 1;
    /* ddrsroslmask - Bits[20:20], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 dqvmsenibblelogicdelay : 2;
    /* dqvmsenibblelogicdelay - Bits[22:21], RW_L, default = None 
       VMSE Logic delay that is shared for all bits in the nibble
     */
    UINT32 writecrcenable : 1;
    /* writecrcenable - Bits[23:23], RW_L, default = None 
       Enable write crc mode
     */
    UINT32 bdx_sys : 1;
    /* bdx_sys - Bits[24:24], RW_L, default = '1 
       This bit helps to bypass the delaycell in transmitter
     */
    UINT32 rsvd_25 : 6;
    UINT32 bypassinitoptolatchenable : 1;
    /* bypassinitoptolatchenable - Bits[31:31], RW_L, default = None 
       This will ignore an InitOp when the RFFSM is active for a read operation
     */
  } Bits;
  UINT32 Data;
} DATACONTROL4_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* VSSHIORVREFCONTROL1_8_MCIO_DDRIOEXT_REG supported on:                      */
/*       BDX (0x401A54CC)                                                     */
/* Register default value:              0x00000000                            */
#define VSSHIORVREFCONTROL1_8_MCIO_DDRIOEXT_REG 0x0D0344CC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * VssHi Controls on Channel 0 only. DIMM Vref Controls on  Channel1, databyte 7, 
 * only. Extension for BDX. 
 */
typedef union {
  struct {
    UINT32 vsshiorvrefctl : 8;
    /* vsshiorvrefctl - Bits[7:0], RW_L, default = 8'b0 
        BDX-specific extended control signal. 
     */
    UINT32 spare : 24;
    /* spare - Bits[31:8], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} VSSHIORVREFCONTROL1_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* PXCATTACK1SEL_8_MCIO_DDRIOEXT_REG supported on:                            */
/*       BDX (0x401A54D0)                                                     */
/* Register default value:              0x00FAC688                            */
#define PXCATTACK1SEL_8_MCIO_DDRIOEXT_REG 0x0D0344D0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Select 2 aggressors out 3 neighbors in a nibble
 */
typedef union {
  struct {
    UINT32 attackr1d0 : 3;
    /* attackr1d0 - Bits[2:0], RW_L, default = 3'd0 
       xtalk aggressor1 select for dq bit[0] in a byte
     */
    UINT32 attackr1d1 : 3;
    /* attackr1d1 - Bits[5:3], RW_L, default = 3'd1 
       xtalk aggressor1 select for dq bit[1] in a byte
     */
    UINT32 attackr1d2 : 3;
    /* attackr1d2 - Bits[8:6], RW_L, default = 3'd2 
       xtalk aggressor1 select for dq bit[2] in a byte
     */
    UINT32 attackr1d3 : 3;
    /* attackr1d3 - Bits[11:9], RW_L, default = 3'd3 
       xtalk aggressor1 select for dq bit[3] in a byte
     */
    UINT32 attackr1d4 : 3;
    /* attackr1d4 - Bits[14:12], RW_L, default = 3'd4 
       xtalk aggressor1 select for dq bit[4] in a byte
     */
    UINT32 attackr1d5 : 3;
    /* attackr1d5 - Bits[17:15], RW_L, default = 3'd5 
       xtalk aggressor1 select for dq bit[5] in a byte
     */
    UINT32 attackr1d6 : 3;
    /* attackr1d6 - Bits[20:18], RW_L, default = 3'd6 
       xtalk aggressor1 select for dq bit[6] in a byte
     */
    UINT32 attackr1d7 : 3;
    /* attackr1d7 - Bits[23:21], RW_L, default = 3'd7 
       xtalk aggressor1 select for dq bit[7] in a byte
     */
    UINT32 spare : 8;
    /* spare - Bits[31:24], RW_L, default = 8'b0 
       spare bits.
     */
  } Bits;
  UINT32 Data;
} PXCATTACK1SEL_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */






/* DATAOFFSETTRAIN_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A54DC)                                                     */
/*       BDX (0x401A54DC)                                                     */
/* Register default value:              0x00000000                            */
#define DATAOFFSETTRAIN_8_MCIO_DDRIOEXT_REG 0x0D0344DC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rcvenoffset : 6;
    /* rcvenoffset - Bits[5:0], RW_L, default = 1'b0 
       Offset added to Trained RxRcvEn Value.
     */
    UINT32 rxdqsoffset : 7;
    /* rxdqsoffset - Bits[12:6], RW_L, default = 1'b0 
       Offset added to Trained Rx DQS Value.
     */
    UINT32 txdqoffset : 6;
    /* txdqoffset - Bits[18:13], RW_L, default = 2'b00 
       Offset added to Trained Tx DQ Value.
     */
    UINT32 txdqsoffset : 6;
    /* txdqsoffset - Bits[24:19], RW_L, default = 6'b000000 
       Offset added to Trained Tx DQS Value.
     */
    UINT32 vrefoffset : 7;
    /* vrefoffset - Bits[31:25], RW_L, default = 7'b0000000 
       unsiged offset added to Trained Vref Value. each step is Vdd/192.
     */
  } Bits;
  UINT32 Data;
} DATAOFFSETTRAIN_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* PXCATTACK0SEL_8_MCIO_DDRIOEXT_REG supported on:                            */
/*       BDX (0x401A54E4)                                                     */
/* Register default value:              0x00FAC688                            */
#define PXCATTACK0SEL_8_MCIO_DDRIOEXT_REG 0x0D0344E4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Select 2 aggressors out 3 neighbors in a nibble
 */
typedef union {
  struct {
    UINT32 attackr0d0 : 3;
    /* attackr0d0 - Bits[2:0], RW_L, default = 3'd0 
       xtalk aggressor0 select for dq bit[0] in a byte
     */
    UINT32 attackr0d1 : 3;
    /* attackr0d1 - Bits[5:3], RW_L, default = 3'd1 
       xtalk aggressor0 select for dq bit[1] in a byte
     */
    UINT32 attackr0d2 : 3;
    /* attackr0d2 - Bits[8:6], RW_L, default = 3'd2 
       xtalk aggressor0 select for dq bit[2] in a byte
     */
    UINT32 attackr0d3 : 3;
    /* attackr0d3 - Bits[11:9], RW_L, default = 3'd3 
       xtalk aggressor0 select for dq bit[3] in a byte
     */
    UINT32 attackr0d4 : 3;
    /* attackr0d4 - Bits[14:12], RW_L, default = 3'd4 
       xtalk aggressor0 select for dq bit[4] in a byte
     */
    UINT32 attackr0d5 : 3;
    /* attackr0d5 - Bits[17:15], RW_L, default = 3'd5 
       xtalk aggressor0 select for dq bit[5] in a byte
     */
    UINT32 attackr0d6 : 3;
    /* attackr0d6 - Bits[20:18], RW_L, default = 3'd6 
       xtalk aggressor0 select for dq bit[6] in a byte
     */
    UINT32 attackr0d7 : 3;
    /* attackr0d7 - Bits[23:21], RW_L, default = 3'd7 
       xtalk aggressor0 select for dq bit[7] in a byte
     */
    UINT32 spare : 8;
    /* spare - Bits[31:24], RW_L, default = 8'b0 
       spare bits.
     */
  } Bits;
  UINT32 Data;
} PXCATTACK0SEL_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATACONTROL0_8_MCIO_DDRIOEXT_REG supported on:                             */
/*       HSX (0x401A54E8)                                                     */
/*       BDX (0x401A54E8)                                                     */
/* Register default value:              0x00000000                            */
#define DATACONTROL0_8_MCIO_DDRIOEXT_REG 0x0D0344E8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rxtrainingmode : 1;
    /* rxtrainingmode - Bits[0:0], RW_L, default = 1'b0 
       Enables Read DqDqs Training Mode
     */
    UINT32 wltrainingmode : 1;
    /* wltrainingmode - Bits[1:1], RW_L, default = 1'b0 
       Enables Write Leveling Training Mode
     */
    UINT32 rltrainingmode : 1;
    /* rltrainingmode - Bits[2:2], RW_L, default = 1'b0 
       Enables Read Leveling Training Mode.
     */
    UINT32 senseamptrainingmode : 1;
    /* senseamptrainingmode - Bits[3:3], RW_L, default = 1'b0 
       Enables SenseAmp offset cancellation training mode.
     */
    UINT32 rsvd_4 : 1;
    UINT32 rfon : 1;
    /* rfon - Bits[5:5], RW_L, default = 1'b0 
       Power-down Disable:  Forces register file read, overriding the rank power down 
       logic. 
     */
    UINT32 rsvd_6 : 1;
    UINT32 txpion : 1;
    /* txpion - Bits[7:7], RW_L, default = 1'b0 
       Power-down Disable:  Forces TxDq and TxDqs PI clocks on.
     */
    UINT32 rsvd_8 : 1;
    UINT32 dcdetectmode : 1;
    /* dcdetectmode - Bits[9:9], RW_L, default = 1'b0 
       Enable VMSE DC detect mode.
     */
    UINT32 rsvd_10 : 2;
    UINT32 txlong : 1;
    /* txlong - Bits[12:12], RW_L, default = 1'b0 
       Sets the duration of the first transmitted DQ bit of the burst. 0=1UI, 1=2UI.
     */
    UINT32 rsvd_13 : 5;
    UINT32 driversegmentenable : 1;
    /* driversegmentenable - Bits[18:18], RW_L, default = 1'b0 
       Controls which segements of the driver are enabled:  {0: All, 1: Only ODT}.
     */
    UINT32 rsvd_19 : 1;
    UINT32 readrfrd : 1;
    /* readrfrd - Bits[20:20], RW_L, default = 1'b0 
       Self clearing command bit. When set, it will download the three 32 bit lines 
       from the RegFile associated with a read c 
       ommand to Rank = ReadRFRank
     */
    UINT32 readrfwr : 1;
    /* readrfwr - Bits[21:21], RW_L, default = 1'b0 
       Self clearing command bit. When set, it will download the three 32 bit lines 
       from the RegFile associated with a write  
       command to Rank = ReadRFRank
     */
    UINT32 readrfrank : 3;
    /* readrfrank - Bits[24:22], RW_L, default = 3'b000 
       Specifies the rank that ReadRFRd or ReadRFWr will manually download the CR 
       values from 
     */
    UINT32 forceodton : 1;
    /* forceodton - Bits[25:25], RW_L, default = 1'b0 
       ODT is forced-on. NOTE: Cannot be set if LongODTR2W is set or can create glitch 
       in transmitter.   NOTE: If forceodton = 1, odtduration should be set to 0 
     */
    UINT32 rsvd_26 : 3;
    UINT32 burstlength4 : 1;
    /* burstlength4 - Bits[29:29], RW_L, default = 1'b0 
       enable lockstep mode, burst length 4
     */
    UINT32 xovercal : 1;
    /* xovercal - Bits[30:30], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
    UINT32 longpreambleenable : 1;
    /* longpreambleenable - Bits[31:31], RW_L, default = 1'b0 
       Enables long preamble for DDR4 devices
     */
  } Bits;
  UINT32 Data;
} DATACONTROL0_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATACONTROL1_8_MCIO_DDRIOEXT_REG supported on:                             */
/*       HSX (0x401A54EC)                                                     */
/*       BDX (0x401A54EC)                                                     */
/* Register default value:              0x00000000                            */
#define DATACONTROL1_8_MCIO_DDRIOEXT_REG 0x0D0344EC
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 7;
    UINT32 sdlldisable : 2;
    /* sdlldisable - Bits[8:7], RW_L, default = 2'b00 
       Controls which slave DLL are disabled: used in VMSE half-width mode to turn off 
       the unused slave delay line. 
                 Set to 0 to have nothing disabled.
                 Set to 1 to have nibble0 disabled.
                 Set to 2 to have nibble1 disabled.
                 Set to 3 to have nibble0 and nibble 1 disabled.
               
     */
    UINT32 rsvd_9 : 1;
    UINT32 rxbiasctl : 3;
    /* rxbiasctl - Bits[12:10], RW_L, default = 1'b0 
       Controls the Current and BW of the Receiver: 
                 {0: 0.34Idll, 1: 0.66Idll, 2: 0.88Idll, 3: Idll, 4: 1.33Idll, 5: 
       1.66Idll, 6: 2.00Idll, 7: 2.33Idll} 
     */
    UINT32 odtdelay : 4;
    /* odtdelay - Bits[16:13], RW_L, default = 4'b0000 
       Controls ODT turn-on delay.   To turn on N tQCK before RcvEn, program to 
       RcvEn[8:6]  N + 4. 
     */
    UINT32 odtduration : 3;
    /* odtduration - Bits[19:17], RW_L, default = 4'b0000 
       Controls the ODT ON duration from (Code, Duration) = (0x0, 11 tQCK) to (0x7, 18 
       tQCK).  NOTE: If forceodton = 1, this should be set to 0 
     */
    UINT32 senseampdelay : 4;
    /* senseampdelay - Bits[23:20], RW_L, default = 4'b0000 
       Controls Senseamp turn-on delay.   To turn on N tQCK before RcvEn, program to 
       RcvEn[8:6]  N + 4. 
     */
    UINT32 senseampduration : 3;
    /* senseampduration - Bits[26:24], RW_L, default = 4'b0000 
       Controls the SenseAmp ON duration from (Code, Duration) = (0x0, 11 tQCK) to 
       (0x7, 18 tQCK) 
     */
    UINT32 burstendodtdelay : 3;
    /* burstendodtdelay - Bits[29:27], RW_L, default = 3'b000 
       Cycles after the burst, when the current pulse should turn on by enabling On-die 
       Terminators 
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} DATACONTROL1_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 7;
    UINT32 sdlldisable : 2;
    /* sdlldisable - Bits[8:7], RW_L, default = 2'b00 
       Controls which slave DLL are disabled: used in vmse half-width mode to turn off 
       the unused slave delay line. 
                 Set to 0 to have nothing disabled.
                 Set to 1 to have nibble0 disabled.
                 Set to 2 to have nibble1 disabled.
                 Set to 3 to have nibble0 and nibble 1 disabled.
               
     */
    UINT32 rsvd_9 : 1;
    UINT32 rxbiasctl : 3;
    /* rxbiasctl - Bits[12:10], RW_L, default = 1'b0 
       Controls the Current and BW of the Receiver: 
                 {0: 0.34Idll, 1: 0.66Idll, 2: 0.88Idll, 3: Idll, 4: 1.33Idll, 5: 
       1.66Idll, 6: 2.00Idll, 7: 2.33Idll} 
     */
    UINT32 odtdelay : 4;
    /* odtdelay - Bits[16:13], RW_L, default = 4'b0000 
       Controls ODT turn-on delay.   To turn on N tQCK before RcvEn, program to 
       RcvEn[8:6]  N + 4. 
     */
    UINT32 odtduration : 3;
    /* odtduration - Bits[19:17], RW_L, default = 4'b0000 
       Controls the ODT ON duration from (Code, Duration) = (0x0, 11 tQCK) to (0x7, 18 
       tQCK).  NOTE: If forceodton = 1, this should be set to 0 
     */
    UINT32 senseampdelay : 4;
    /* senseampdelay - Bits[23:20], RW_L, default = 4'b0000 
       Controls Senseamp turn-on delay.   To turn on N tQCK before RcvEn, program to 
       RcvEn[8:6]  N + 4. 
     */
    UINT32 senseampduration : 3;
    /* senseampduration - Bits[26:24], RW_L, default = 4'b0000 
       Controls the SenseAmp ON duration from (Code, Duration) = (0x0, 11 tQCK) to 
       (0x7, 18 tQCK) 
     */
    UINT32 burstendodtdelay : 3;
    /* burstendodtdelay - Bits[29:27], RW_L, default = 3'b000 
       Cycles after the burst, when the current pulse should turn on by enabling On-die 
       Terminators 
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} DATACONTROL1_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DATACONTROL2_8_MCIO_DDRIOEXT_REG supported on:                             */
/*       HSX (0x401A54F0)                                                     */
/*       BDX (0x401A54F0)                                                     */
/* Register default value:              0x00000000                            */
#define DATACONTROL2_8_MCIO_DDRIOEXT_REG 0x0D0344F0
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rxstaggerctl : 5;
    /* rxstaggerctl - Bits[4:0], RW_L, default = 5'b0 
       Delay DdrRxEnableQnnnH by RxStaggerCtl qclks (0-31) before turning on/off the 
       RxAmps  
               If RxStaggerCtl = 0, use the ODT Delay/Duration settings for turning 
       on/off the Rx amplifier  
     */
    UINT32 forcebiason : 1;
    /* forcebiason - Bits[5:5], RW_L, default = 1'b0 
       Force on the internal Vref and Rx bias circuit, regardless of any other power 
       downs 
     */
    UINT32 forcerxon : 1;
    /* forcerxon - Bits[6:6], RW_L, default = 1'b0 
       Force on the RxAmp only (as opposed to OdtSampOn, which turns on both ODT and 
       the amplifier). 
               Use for overclocking support where we may not be able to hit the fast 
       exit latency  
     */
    UINT32 rsvd_7 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[8:8], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/VMSE
     */
    UINT32 imodecfg : 1;
    /* imodecfg - Bits[9:9], RW_L, default = None 
       Imode Select, 0: Swing Boost Mode; 1: Xtalk Cancellation Mode
     */
    UINT32 imodeenable : 1;
    /* imodeenable - Bits[10:10], RW_L, default = None 
       Enable Imode EQ per byte lane. 
                 0: Imode completely off 
                 1: Imode enabled (Xtalk cancellation OR swing boost selected by 
       ImodeCfg. 
                 If enabled, the bit has to be turned on after ImodeBiasEnable turned 
       on. 
               
     */
    UINT32 imodeeqcode : 4;
    /* imodeeqcode - Bits[14:11], RW_L, default = None 
       Binary Imode Coefficient, sets Imode current strength. 
                 (used for either Xtalk cancellation or swing boost)
                 Code   Imode Current (approx)
                 ------------------
                 0000        0  
                 0001        1
                 0010        2 
                   *  
                   * 
                   *
                 1111        3
     */
    UINT32 imodebiasen : 1;
    /* imodebiasen - Bits[15:15], RW_L, default = None 
       Enables Imode Bias circuit. It must be enabled before ImodeEnable is turned on.
     */
    UINT32 imodescomp : 6;
    /* imodescomp - Bits[21:16], RW_L, default = None 
       Scomp code for Imode circuit
     */
    UINT32 imodescompovrd : 1;
    /* imodescompovrd - Bits[22:22], RW_L, default = None 
       Imode Scomp Override Enable
     */
    UINT32 rxdqssaoffset : 5;
    /* rxdqssaoffset - Bits[27:23], RW_L, default = None 
       strobe Rx Amp offset control
     */
    UINT32 rsvd_28 : 2;
    UINT32 xtalklowpwrenb : 1;
    /* xtalklowpwrenb - Bits[30:30], RW_L, default = 1'b0 
       disables xtalk toggle detect, also used to set narrow operation range for 
       internal  
                 Vref gen circuit. when narrow range operation is needed, it is 
       expected that bios will program 
                 this bit to '1' first and, when Vref is settled, bios program Vref 
       levelshifter hold cr, which 
                 latch the setting. Afterwards, this CR can be used to set xtalk 
       lowpower disable or not. 
               
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DATACONTROL2_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 rxstaggerctl : 5;
    /* rxstaggerctl - Bits[4:0], RW_L, default = 5'b0 
       Delay DdrRxEnableQnnnH by RxStaggerCtl qclks (0-31) before turning on/off the 
       RxAmps  
               If RxStaggerCtl = 0, use the ODT Delay/Duration settings for turning 
       on/off the Rx amplifier  
     */
    UINT32 forcebiason : 1;
    /* forcebiason - Bits[5:5], RW_L, default = 1'b0 
       Force on the internal Vref and Rx bias circuit, regardless of any other power 
       downs 
     */
    UINT32 forcerxon : 1;
    /* forcerxon - Bits[6:6], RW_L, default = 1'b0 
       Force on the RxAmp only (as opposed to OdtSampOn, which turns on both ODT and 
       the amplifier). 
               Use for overclocking support where we may not be able to hit the fast 
       exit latency  
     */
    UINT32 rsvd_7 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[8:8], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/VMSE
     */
    UINT32 imodecfg : 1;
    /* imodecfg - Bits[9:9], RW_L, default = None 
       Imode Select, 0: Swing Boost Mode; 1: Xtalk Cancellation Mode
     */
    UINT32 imodeenable : 1;
    /* imodeenable - Bits[10:10], RW_L, default = None 
       Enable Imode EQ per byte lane. 
                 0: Imode completely off 
                 1: Imode enabled (Xtalk cancellation OR swing boost selected by 
       ImodeCfg. 
                 If enabled, the bit has to be turned on after ImodeBiasEnable turned 
       on. 
               
     */
    UINT32 imodeeqcode : 4;
    /* imodeeqcode - Bits[14:11], RW_L, default = None 
       Binary Imode Coefficient, sets Imode current strength. 
                 (used for either Xtalk cancellation or swing boost)
                 Code   Imode Current (approx)
                 ------------------
                 0000        0  
                 0001        1
                 0010        2 
                   *  
                   * 
                   *
                 1111        3
     */
    UINT32 imodebiasen : 1;
    /* imodebiasen - Bits[15:15], RW_L, default = None 
       Enables Imode Bias circuit. It must be enabled before ImodeEnable is turned on.
     */
    UINT32 imodescomp : 6;
    /* imodescomp - Bits[21:16], RW_L, default = None 
       Scomp code for Imode circuit
     */
    UINT32 imodescompovrd : 1;
    /* imodescompovrd - Bits[22:22], RW_L, default = None 
       Imode Scomp Override Enable
     */
    UINT32 rxdqssaoffset : 5;
    /* rxdqssaoffset - Bits[27:23], RW_L, default = None 
       strobe Rx Amp offset control
     */
    UINT32 rsvd_28 : 2;
    UINT32 xtalklowpwrenb : 1;
    /* xtalklowpwrenb - Bits[30:30], RW_L, default = 1'b0 
       disables xtalk toggle detect, also used to set narrow operation range for 
       internal  
                 Vref gen circuit. when narrow range operation is needed, it is 
       expected that bios will program 
                 this bit to '1' first and, when Vref is settled, bios program Vref 
       levelshifter hold cr, which 
                 latch the setting. Afterwards, this CR can be used to set xtalk 
       lowpower disable or not. 
               
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DATACONTROL2_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DATACONTROL3_8_MCIO_DDRIOEXT_REG supported on:                             */
/*       HSX (0x401A54F4)                                                     */
/*       BDX (0x401A54F4)                                                     */
/* Register default value:              0x8184924924F                         */
#define DATACONTROL3_8_MCIO_DDRIOEXT_REG 0x0D0344F4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * static configurations of tx and rx.
 */
typedef union {
  struct {
    UINT32 drvstaticlegcfg : 2;
    /* drvstaticlegcfg - Bits[1:0], RW_L, default = 2'b11 
       binary default setting for static legs in DRV mode
                  0    0       0 legs (EQ disabled)
                  0    1       3 legs (EQ max 3 legs)
                  1    0       6 legs (EQ max 6 legs)
                  1    1      12 legs (EQ max 12 legs)
                 In most cases, we would set this to be 11, but for some configurations 
       that require a higher  
                 Ron value, we need to reduce the number of static legs enabled so that 
       fast corner can hit  
                 the higher Ron target value.
     */
    UINT32 odtstaticlegcfg : 2;
    /* odtstaticlegcfg - Bits[3:2], RW_L, default = 2'b11 
       binary default setting for static legs in ODT mode
                  0    0       0 legs (EQ disabled)
                  0    1       3 legs (EQ max 3 legs)
                  1    0       6 legs (EQ max 6 legs)
                  1    1      12 legs (EQ max 12 legs)
                 In most cases, we would set this to be 11, but for some configurations 
       that require a higher  
                 Ron value, we need to reduce the number of static legs enabled so that 
       fast corner can hit  
                 the higher Ron target value.
     */
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[4:4], RW_L, default = 1'b0 
       level shift hold enable
                            Holds the HV control values and power down the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rsvd_5 : 1;
    UINT32 odtsegmentenable : 3;
    /* odtsegmentenable - Bits[8:6], RW_L, default = 3'b001 
       This will tell which  segments are enabled out of reset.  
                            After that, a shift register will determine the enabled 
       segments 
     */
    UINT32 datasegmentenable : 3;
    /* datasegmentenable - Bits[11:9], RW_L, default = 3'b001 
       This will tell which  segments are enabled out of reset. 
     */
    UINT32 longodtr2w : 1;
    /* longodtr2w - Bits[12:12], RW_L, default = 3'b001 
       When set, it will extends ODT by 1 qclk on read to write turnarounds. NOTE: 
       Cannot be set if ForceODTOn is set or can create glitch in transmitter. 
     */
    UINT32 imodebiashighcm : 1;
    /* imodebiashighcm - Bits[13:13], RW_L, default = 3'b001 
       imodebias control 
     */
    UINT32 imodebiasvrefen : 1;
    /* imodebiasvrefen - Bits[14:14], RW_L, default = 3'b001 
       imodebias control 
     */
    UINT32 imodebiasrxbiastrim : 3;
    /* imodebiasrxbiastrim - Bits[17:15], RW_L, default = 3'b001 
       imodebias control. Also used for rxbiasana 
     */
    UINT32 rsvd_18 : 3;
    UINT32 ddrcrctlecapen : 2;
    /* ddrcrctlecapen - Bits[22:21], RW_L, default = 2'b00 
       ctle capacitor setting 
     */
    UINT32 ddrcrctleresen : 2;
    /* ddrcrctleresen - Bits[24:23], RW_L, default = 2'b11 
       ctle resistor setting 
     */
    UINT32 pcasbiasclosedloopen : 1;
    /* pcasbiasclosedloopen - Bits[25:25], RW_L, default = 2'b00 
       when enabled, choose close loop for the pcas bias loop in Rxbias
     */
    UINT32 rxbiassel : 1;
    /* rxbiassel - Bits[26:26], RW_L, default = 2'b00 
       only used in CTL fub. when enabled, it turns on part of Rxbias for imodebias.
     */
    UINT32 rxbiasfoldedlegtrim : 2;
    /* rxbiasfoldedlegtrim - Bits[28:27], RW_L, default = 2'b00 
       adjust common mode in the first stage.
     */
    UINT32 rxbiasgcncomp : 2;
    /* rxbiasgcncomp - Bits[30:29], RW_L, default = 2'b01 
       used to compensate the GCN skew. The GCN skew information should be
                 extracted by reading and analyzing the final Rcomp code value. The 
       target spec 
                 is to categorize the GCN skew in 3 process condition (mainly slow, 
       fast, typical). 
                 (one example can be just reading the 2 MLS bit of Rcomp code and 
       determine if the 
                 GCN is slow, fast or typical). The gcncomp is used for CTLE.
                 11: gcn in slow corner
                 10: gcn in typical corner
                 01: gcn in fast corner
               
     */
    UINT32 postambleenable : 1;
    /* postambleenable - Bits[31:31], RW_L, default = None 
       VMSE: enables long postamble and a dummy toggle
     */
  } Bits;
  UINT32 Data;
} DATACONTROL3_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* VSSHIORVREFCONTROL_8_MCIO_DDRIOEXT_REG supported on:                       */
/*       HSX (0x401A54F8)                                                     */
/*       BDX (0x401A54F8)                                                     */
/* Register default value:              0x004D8238                            */
#define VSSHIORVREFCONTROL_8_MCIO_DDRIOEXT_REG 0x0D0344F8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * VssHi Controls on Channel 0 only. DIMM Vref Controls on  Channel1, databyte 7, 
 * only. 
 */
typedef union {
  struct {
    UINT32 vsshiorvrefctl : 24;
    /* vsshiorvrefctl - Bits[23:0], RW_L, default = 24'h4d8238 
         VssHi adjust setting.
                Field          Bits    Description
               -------         ----    
       ----------------------------------------------------------------------------- 
               SelCode          23     Selects the code to output:  {0: Code, 1: TCode}
               GainBoost        22     Enable gain boost is panic mode by updating 
       code/tcode    
               PanicVoltage  21:18  Unsigned integer controlling the voltage error on 
       VssHi that will trigger the panic driver.  Step size of VccDDQ/192 (0 to 117mV) 
               CloseLoop        17     Enables the VssHi close loop tracking 
               PanicEn          16     Enables the VssHi Panic Driver
               BWError          15:14  Limits the loop gain (ie: difference between 
       code and tcode) to +/- 2^(SlowBWError) 
               OpenLoop         13     Operates VssHi in open loop mode with the target 
       Vref code directly driving the output DAC with no feedback involved 
               SampleDivider    12:10  Controls the loop bandwidth by dividing down the 
       input clock: {0: Qclk, 1: Qclk/2, 2: Qclk/4, 3: Qclk/8, 4: Qclk/16, 5: Qclk/32, 
       6: Qclk/64, 7: Qclk/128} 
               LoBWDivider      9:8    For the slow integrator, selects the net number 
       of samples in a given direction to trip the filter.  {0: 4 samples, 1: 8 
       samples, 2: 16 samples, 3: 32 samples} 
               HiBWDivider      7:6    For the fast integrator, selects the net number 
       of samples in a given direction to trip the filter.  {0: 4 samples, 1: 8 
       samples, 2: 16 samples, 3: 32 samples} 
               Target           5:0    Unsigned integer controlling the target VssHi 
       voltage.  Step size of VccDDQ/192 and starting value of VccDDQ*20/192 
               
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} VSSHIORVREFCONTROL_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* RXGROUP0N1RANK0_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5500)                                                     */
/*       BDX (0x401A5500)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK0_8_MCIO_DDRIOEXT_REG 0x0D034500

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank0
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK0_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK1_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5504)                                                     */
/*       BDX (0x401A5504)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK1_8_MCIO_DDRIOEXT_REG 0x0D034504

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank1
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK1_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK2_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5508)                                                     */
/*       BDX (0x401A5508)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK2_8_MCIO_DDRIOEXT_REG 0x0D034508

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank2
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK2_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK3_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A550C)                                                     */
/*       BDX (0x401A550C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK3_8_MCIO_DDRIOEXT_REG 0x0D03450C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank3
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK3_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK4_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5510)                                                     */
/*       BDX (0x401A5510)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK4_8_MCIO_DDRIOEXT_REG 0x0D034510

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank4
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK4_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK5_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5514)                                                     */
/*       BDX (0x401A5514)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK5_8_MCIO_DDRIOEXT_REG 0x0D034514

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank5
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK5_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK6_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5518)                                                     */
/*       BDX (0x401A5518)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK6_8_MCIO_DDRIOEXT_REG 0x0D034518

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank6
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK6_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK7_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A551C)                                                     */
/*       BDX (0x401A551C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK7_8_MCIO_DDRIOEXT_REG 0x0D03451C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data RX training values for Rank7
 */
typedef union {
  struct {
    UINT32 rcvendelay : 9;
    /* rcvendelay - Bits[8:0], RW_L, default = 1'b0 
       RcvEn timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 rcvenxsel : 1;
    /* rcvenxsel - Bits[9:9], RW_L, default = 1'b0 
       mux select for crossover from ClkPiRef to CkPi4RcvEn, used in xover2to2 mode
     */
    UINT32 rxdqspdelay : 7;
    /* rxdqspdelay - Bits[16:10], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_P to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqsndelay : 7;
    /* rxdqsndelay - Bits[23:17], RW_L, default = 1'b0 
       per nibble READ timing control, delay DQS_N to match the earlest arrival DQ, 
       0~1.5UI with step size ClkPi/64 
     */
    UINT32 rxdqdelay : 3;
    /* rxdqdelay - Bits[26:24], RW_L, default = 1'b0 
       Rx DQ delay, per nibble, used in case if DQ is too early than DQS
     */
    UINT32 rxeq : 3;
    /* rxeq - Bits[29:27], RW_L, default = 3'b0 
       CTLE current steering tuning (0=no eq, 7=max eq). It is decoded to
                   CTLE resistor tuning: 00 - 3R (10.8Kohms), 01/10 -  R (3.6Kohms), 11 
       - R/3 (1.2Kohms) 
                   CTLE capacitor tuning: 00 - 0C (not used), 01 - 1C (200ff), 10 - 2C 
       (400ff), 11 - 3C (600ff) 
               
     */
    UINT32 rsvd_30 : 2;
  } Bits;
  UINT32 Data;
} RXGROUP0N1RANK7_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK0_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5520)                                                     */
/*       BDX (0x401A5520)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK0_8_MCIO_DDRIOEXT_REG 0x0D034520

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK0_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK1_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5524)                                                     */
/*       BDX (0x401A5524)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK1_8_MCIO_DDRIOEXT_REG 0x0D034524

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK1_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK2_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5528)                                                     */
/*       BDX (0x401A5528)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK2_8_MCIO_DDRIOEXT_REG 0x0D034528

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK2_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK3_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A552C)                                                     */
/*       BDX (0x401A552C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK3_8_MCIO_DDRIOEXT_REG 0x0D03452C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK3_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK4_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5530)                                                     */
/*       BDX (0x401A5530)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK4_8_MCIO_DDRIOEXT_REG 0x0D034530

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK4_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK5_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5534)                                                     */
/*       BDX (0x401A5534)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK5_8_MCIO_DDRIOEXT_REG 0x0D034534

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK5_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK6_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5538)                                                     */
/*       BDX (0x401A5538)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK6_8_MCIO_DDRIOEXT_REG 0x0D034538

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK6_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK7_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A553C)                                                     */
/*       BDX (0x401A553C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK7_8_MCIO_DDRIOEXT_REG 0x0D03453C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 rxdqspdelay0 : 4;
    /* rxdqspdelay0 - Bits[3:0], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay1 : 4;
    /* rxdqspdelay1 - Bits[7:4], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay2 : 4;
    /* rxdqspdelay2 - Bits[11:8], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqspdelay3 : 4;
    /* rxdqspdelay3 - Bits[15:12], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_P to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay0 : 4;
    /* rxdqsndelay0 - Bits[19:16], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay1 : 4;
    /* rxdqsndelay1 - Bits[23:20], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay2 : 4;
    /* rxdqsndelay2 - Bits[27:24], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
    UINT32 rxdqsndelay3 : 4;
    /* rxdqsndelay3 - Bits[31:28], RW_L, default = 1'b0 
       per bit READ timing control, 
                   delay DQS_N to match each DQ, 0~.25UI with step size ClkPi/64 added 
       to nibble delay RxDqsPDelay 
               
     */
  } Bits;
  UINT32 Data;
} RXGROUP1N1RANK7_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP0N1RANK0_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5540)                                                     */
/*       BDX (0x401A5540)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK0_8_MCIO_DDRIOEXT_REG 0x0D034540
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK0_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK0_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK1_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5544)                                                     */
/*       BDX (0x401A5544)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK1_8_MCIO_DDRIOEXT_REG 0x0D034544
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK1_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK1_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK2_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5548)                                                     */
/*       BDX (0x401A5548)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK2_8_MCIO_DDRIOEXT_REG 0x0D034548
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK2_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK2_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK3_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A554C)                                                     */
/*       BDX (0x401A554C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK3_8_MCIO_DDRIOEXT_REG 0x0D03454C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK3_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK3_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK4_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5550)                                                     */
/*       BDX (0x401A5550)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK4_8_MCIO_DDRIOEXT_REG 0x0D034550
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK4_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK4_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK5_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5554)                                                     */
/*       BDX (0x401A5554)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK5_8_MCIO_DDRIOEXT_REG 0x0D034554
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK5_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK5_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK6_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5558)                                                     */
/*       BDX (0x401A5558)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK6_8_MCIO_DDRIOEXT_REG 0x0D034558
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK6_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK6_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK7_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A555C)                                                     */
/*       BDX (0x401A555C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK7_8_MCIO_DDRIOEXT_REG 0x0D03455C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 5;
    /* txeq - Bits[27:23], RW_L, default = 1'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK7_8_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing and Voltage control for writing one data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqdelay : 9;
    /* txdqdelay - Bits[8:0], RW_L, default = 1'b0 
       Write DQ timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqsdelay : 9;
    /* txdqsdelay - Bits[17:9], RW_L, default = 1'b0 
       Write DQS timing control.  Unsigned number from 0 to 8 tQCK with step size of 
       tQCK/64 
     */
    UINT32 txdqxsel0 : 1;
    /* txdqxsel0 - Bits[18:18], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 0
     */
    UINT32 txdqxsel1 : 1;
    /* txdqxsel1 - Bits[19:19], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 1
     */
    UINT32 txdqxsel2 : 1;
    /* txdqxsel2 - Bits[20:20], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 2
     */
    UINT32 txdqxsel3 : 1;
    /* txdqxsel3 - Bits[21:21], RW_L, default = 1'b0 
       xelect for crossover from PiRef to PiN for bit 3
     */
    UINT32 txdqsxsel : 1;
    /* txdqsxsel - Bits[22:22], RW_L, default = 1'b0 
       xelect for crossover from ClkPiRef to CkPi4Dqs
     */
    UINT32 txeq : 6;
    /* txeq - Bits[28:23], RW_L, default = 6'b0 
       Write Equalization codes.  5 bit control, but only 24 of them are meaningful. 
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
                   BDX-change. Increased the width by 1bit for TXEQ tracking     
               
     */
    UINT32 rsvd_29 : 3;
  } Bits;
  UINT32 Data;
} TXGROUP0N1RANK7_8_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP1N1RANK0_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5560)                                                     */
/*       BDX (0x401A5560)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK0_8_MCIO_DDRIOEXT_REG 0x0D034560

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK0_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK1_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5564)                                                     */
/*       BDX (0x401A5564)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK1_8_MCIO_DDRIOEXT_REG 0x0D034564

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK1_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK2_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5568)                                                     */
/*       BDX (0x401A5568)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK2_8_MCIO_DDRIOEXT_REG 0x0D034568

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK2_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK3_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A556C)                                                     */
/*       BDX (0x401A556C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK3_8_MCIO_DDRIOEXT_REG 0x0D03456C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK3_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK4_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5570)                                                     */
/*       BDX (0x401A5570)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK4_8_MCIO_DDRIOEXT_REG 0x0D034570

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK4_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK5_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5574)                                                     */
/*       BDX (0x401A5574)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK5_8_MCIO_DDRIOEXT_REG 0x0D034574

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK5_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK6_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5578)                                                     */
/*       BDX (0x401A5578)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK6_8_MCIO_DDRIOEXT_REG 0x0D034578

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK6_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK7_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A557C)                                                     */
/*       BDX (0x401A557C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK7_8_MCIO_DDRIOEXT_REG 0x0D03457C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Timing control for each lane of the data byte on Rank
 */
typedef union {
  struct {
    UINT32 txdqpicode0 : 6;
    /* txdqpicode0 - Bits[5:0], RW_L, default = 1'b0 
       Write timing offset for bit[4] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode1 : 6;
    /* txdqpicode1 - Bits[11:6], RW_L, default = 1'b0 
       Write timing offset for bit[5] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode2 : 6;
    /* txdqpicode2 - Bits[17:12], RW_L, default = 1'b0 
       Write timing offset for bit[6] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 txdqpicode3 : 6;
    /* txdqpicode3 - Bits[23:18], RW_L, default = 1'b0 
       Write timing offset for bit[7] of the DQ byte.  0 to 1ClkPi with step size of 
       tClkPi/64 
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} TXGROUP1N1RANK7_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK0_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5580)                                                     */
/*       BDX (0x401A5580)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK0_8_MCIO_DDRIOEXT_REG 0x0D034580

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK0_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK1_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5584)                                                     */
/*       BDX (0x401A5584)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK1_8_MCIO_DDRIOEXT_REG 0x0D034584

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK1_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK2_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5588)                                                     */
/*       BDX (0x401A5588)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK2_8_MCIO_DDRIOEXT_REG 0x0D034588

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK2_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK3_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A558C)                                                     */
/*       BDX (0x401A558C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK3_8_MCIO_DDRIOEXT_REG 0x0D03458C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK3_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK4_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5590)                                                     */
/*       BDX (0x401A5590)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK4_8_MCIO_DDRIOEXT_REG 0x0D034590

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK4_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK5_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5594)                                                     */
/*       BDX (0x401A5594)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK5_8_MCIO_DDRIOEXT_REG 0x0D034594

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK5_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK6_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5598)                                                     */
/*       BDX (0x401A5598)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK6_8_MCIO_DDRIOEXT_REG 0x0D034598

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK6_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK7_8_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A559C)                                                     */
/*       BDX (0x401A559C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK7_8_MCIO_DDRIOEXT_REG 0x0D03459C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Read Per Lane Amplifier Voltage Offset
 */
typedef union {
  struct {
    UINT32 saoffset0 : 5;
    /* saoffset0 - Bits[4:0], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[0] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset1 : 5;
    /* saoffset1 - Bits[9:5], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[1] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset2 : 5;
    /* saoffset2 - Bits[14:10], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[2] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 saoffset3 : 5;
    /* saoffset3 - Bits[19:15], RW_L, default = 1'b0 
       Amplifier voltage offset for bit[3] of the DQ byte. {0: Most negative offset,... 
       8: 0 offset, ... 63: Most postive offset}  
     */
    UINT32 rxdqspsel : 2;
    /* rxdqspsel - Bits[21:20], RW_L, default = 1'b0 
       x4/x8 DQS mux select
                   [0]=1 selects the raw output from the nibble0 DQS amplifier in x8 
       mode;  
                   [0]=0 select nibble1 amplifier output in x4 mode; 
                   [1] is not used.
               
     */
    UINT32 rxdqsnsel : 2;
    /* rxdqsnsel - Bits[23:22], RW_L, default = 1'b0 
       select the dqs clock skewed with max delay to drive rxFIFO write
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} RXOFFSETN1RANK7_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* RXVREFCTRLN1_8_MCIO_DDRIOEXT_REG supported on:                             */
/*       HSX (0x401A55A4)                                                     */
/*       BDX (0x401A55A4)                                                     */
/* Register default value:              0x00000000                            */
#define RXVREFCTRLN1_8_MCIO_DDRIOEXT_REG 0x0D0345A4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * RxVrefCtrl
 */
typedef union {
  struct {
    UINT32 vrefperbit0 : 7;
    /* vrefperbit0 - Bits[6:0], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben0 : 1;
    /* halflsben0 - Bits[7:7], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit1 : 7;
    /* vrefperbit1 - Bits[14:8], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben1 : 1;
    /* halflsben1 - Bits[15:15], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit2 : 7;
    /* vrefperbit2 - Bits[22:16], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben2 : 1;
    /* halflsben2 - Bits[23:23], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 vrefperbit3 : 7;
    /* vrefperbit3 - Bits[30:24], RW_L, default = 1'b0 
       Per bit Rx Vref Control
     */
    UINT32 halflsben3 : 1;
    /* halflsben3 - Bits[31:31], RW_L, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
  } Bits;
  UINT32 Data;
} RXVREFCTRLN1_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATATRAINFEEDBACKN1_8_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A55A8)                                                     */
/*       BDX (0x401A55A8)                                                     */
/* Register default value:              0x00000000                            */
#define DATATRAINFEEDBACKN1_8_MCIO_DDRIOEXT_REG 0x0D0345A8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data Training Feedback Results
 */
typedef union {
  struct {
    UINT32 datatrainfeedback : 9;
    /* datatrainfeedback - Bits[8:0], RW_L, default = 1'b0 
       Data Training Feedback Storage
     */
    UINT32 rsvd_9 : 23;
  } Bits;
  UINT32 Data;
} DATATRAINFEEDBACKN1_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATATCOCOMPOFFSET0_8_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A55AC)                                                     */
/* Register default value:              0x00000000                            */
#define DATATCOCOMPOFFSET0_8_MCIO_DDRIOEXT_REG 0x0D0345AC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Data offsets to training values.
 */
typedef union {
  struct {
    UINT32 dq0tcocompoffset : 5;
    /* dq0tcocompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment offset added to Tco Comp Value.
     */
    UINT32 dq1tcocompoffset : 5;
    /* dq1tcocompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment offset added to Tco Comp Value.
     */
    UINT32 dq2tcocompoffset : 5;
    /* dq2tcocompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment offset added to DQ2 Tco Comp Value.
     */
    UINT32 dq3tcocompoffset : 5;
    /* dq3tcocompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment offset added to DQ3 Tco Comp Value.
     */
    UINT32 dqs0tcocompoffset : 5;
    /* dqs0tcocompoffset - Bits[24:20], RW_L, default = 4'b0000 
       2s Compliment offset added to DQ4 Tco Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATATCOCOMPOFFSET0_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATATCOCOMPOFFSET1_8_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A55B0)                                                     */
/* Register default value:              0x00000000                            */
#define DATATCOCOMPOFFSET1_8_MCIO_DDRIOEXT_REG 0x0D0345B0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Per Bit Tco Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4tcocompoffset : 5;
    /* dq4tcocompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment offset added to DQ4 Tco Comp Value.
     */
    UINT32 dq5tcocompoffset : 5;
    /* dq5tcocompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment offset added to Dq5 Tco Comp Value.
     */
    UINT32 dq6tcocompoffset : 5;
    /* dq6tcocompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment offset added to Dq6 Tco Comp Value.
     */
    UINT32 dq7tcocompoffset : 5;
    /* dq7tcocompoffset - Bits[19:15], RW_L, default = 1'b0 
       2s Compliment offset added to Dq7 Tco Comp Value.
     */
    UINT32 dqs1tcocompoffset : 5;
    /* dqs1tcocompoffset - Bits[24:20], RW_L, default = 1'b0 
       2s Compliment offset added to Dqs1 Tco Comp Value.
     */
    UINT32 enperbittcocomp : 1;
    /* enperbittcocomp - Bits[25:25], RW_L, default = 1'b0 
       Enable per bit Tco Comp flow.
     */
    UINT32 gentcocompdone : 1;
    /* gentcocompdone - Bits[26:26], RW_L, default = 1'b0 
       Generate TCO Comp done.
     */
    UINT32 spare : 5;
    /* spare - Bits[31:27], RW_L, default = 1'b0 
       Generate TCO Comp done.
     */
  } Bits;
  UINT32 Data;
} DATATCOCOMPOFFSET1_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET0_8_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A55B4)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET0_8_MCIO_DDRIOEXT_REG 0x0D0345B4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Per BIT ODT Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq0odtpupcompoffset : 5;
    /* dq0odtpupcompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq0 Odt Comp Value.
     */
    UINT32 dq1odtpupcompoffset : 5;
    /* dq1odtpupcompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq1 Odt Comp Value.
     */
    UINT32 dq2odtpupcompoffset : 5;
    /* dq2odtpupcompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to DQ2 Odt Comp Value.
     */
    UINT32 dq3odtpupcompoffset : 5;
    /* dq3odtpupcompoffset - Bits[19:15], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQ3 Odt Comp Value.
     */
    UINT32 dqs0odtpupcompoffset : 5;
    /* dqs0odtpupcompoffset - Bits[24:20], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQs0 Odt Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATAODTCOMPOFFSET0_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET1_8_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A55B8)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET1_8_MCIO_DDRIOEXT_REG 0x0D0345B8

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Per Bit Odt Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq0odtpdncompoffset : 5;
    /* dq0odtpdncompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq0 Odt Comp Value.
     */
    UINT32 dq1odtpdncompoffset : 5;
    /* dq1odtpdncompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq1 Odt Comp Value.
     */
    UINT32 dq2odtpdncompoffset : 5;
    /* dq2odtpdncompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to DQ2 Odt Comp Value.
     */
    UINT32 dq3odtpdncompoffset : 5;
    /* dq3odtpdncompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment Pull-down offset added to DQ3 Odt Comp Value.
     */
    UINT32 dqs0odtpdncompoffset : 5;
    /* dqs0odtpdncompoffset - Bits[24:20], RW_L, default = 4'b0000 
       2s Compliment Pull-down offset added to DQS0 Odt Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATAODTCOMPOFFSET1_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET2_8_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A55BC)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET2_8_MCIO_DDRIOEXT_REG 0x0D0345BC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Per Bit Odt Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4odtpupcompoffset : 5;
    /* dq4odtpupcompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq4 Odt Comp Value.
     */
    UINT32 dq5odtpupcompoffset : 5;
    /* dq5odtpupcompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq5 Odt Comp Value.
     */
    UINT32 dq6odtpupcompoffset : 5;
    /* dq6odtpupcompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to DQ6 Odt Comp Value.
     */
    UINT32 dq7odtpupcompoffset : 5;
    /* dq7odtpupcompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment Pull-up offset added to DQ7 Odt Comp Value.
     */
    UINT32 dqs1odtpupcompoffset : 5;
    /* dqs1odtpupcompoffset - Bits[24:20], RW_L, default = 2'b00 
       2s Compliment Pull-up offset added to DQs1 Odt Comp Value.
     */
    UINT32 enperbitodt : 1;
    /* enperbitodt - Bits[25:25], RW_L, default = 1'b0 
       Enable per bit ODT comp.
     */
    UINT32 genodtcompdone : 1;
    /* genodtcompdone - Bits[26:26], RW_L, default = 1'b0 
       Generate ODT comp Done.
     */
    UINT32 spare : 5;
    /* spare - Bits[31:27], RW_L, default = 1'b0 
       spare
     */
  } Bits;
  UINT32 Data;
} DATAODTCOMPOFFSET2_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET3_8_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A55C0)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET3_8_MCIO_DDRIOEXT_REG 0x0D0345C0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Per BIT ODT Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4odtpdncompoffset : 5;
    /* dq4odtpdncompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq4 Odt Comp Value.
     */
    UINT32 dq5odtpdncompoffset : 5;
    /* dq5odtpdncompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq5 Odt Comp Value.
     */
    UINT32 dq6odtpdncompoffset : 5;
    /* dq6odtpdncompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to DQ6 Odt Comp Value.
     */
    UINT32 dq7odtpdncompoffset : 5;
    /* dq7odtpdncompoffset - Bits[19:15], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQ7 Odt Comp Value.
     */
    UINT32 dqs1odtpdncompoffset : 5;
    /* dqs1odtpdncompoffset - Bits[24:20], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQs0 Odt Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATAODTCOMPOFFSET3_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET0_8_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A55C4)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET0_8_MCIO_DDRIOEXT_REG 0x0D0345C4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Per BIT RON Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq0ronpupcompoffset : 5;
    /* dq0ronpupcompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq0 Ron Comp Value.
     */
    UINT32 dq1ronpupcompoffset : 5;
    /* dq1ronpupcompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq1 Ron Comp Value.
     */
    UINT32 dq2ronpupcompoffset : 5;
    /* dq2ronpupcompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to DQ2 Ron Comp Value.
     */
    UINT32 dq3ronpupcompoffset : 5;
    /* dq3ronpupcompoffset - Bits[19:15], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQ3 Ron Comp Value.
     */
    UINT32 dqs0ronpupcompoffset : 5;
    /* dqs0ronpupcompoffset - Bits[24:20], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQs0 Ron Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATARONCOMPOFFSET0_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET1_8_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A55C8)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET1_8_MCIO_DDRIOEXT_REG 0x0D0345C8

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Per Bit RON Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq0ronpdncompoffset : 5;
    /* dq0ronpdncompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq0 Ron Comp Value.
     */
    UINT32 dq1ronpdncompoffset : 5;
    /* dq1ronpdncompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq1 Ron Comp Value.
     */
    UINT32 dq2ronpdncompoffset : 5;
    /* dq2ronpdncompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to DQ2 Ron Comp Value.
     */
    UINT32 dq3ronpdncompoffset : 5;
    /* dq3ronpdncompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment Pull-down offset added to DQ3 Ron Comp Value.
     */
    UINT32 dqs0ronpdncompoffset : 5;
    /* dqs0ronpdncompoffset - Bits[24:20], RW_L, default = 4'b0000 
       2s Compliment Pull-down offset added to DQS0 Ron Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATARONCOMPOFFSET1_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET2_8_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A55CC)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET2_8_MCIO_DDRIOEXT_REG 0x0D0345CC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Per Bit Ron Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4ronpupcompoffset : 5;
    /* dq4ronpupcompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq4 Ron Comp Value.
     */
    UINT32 dq5ronpupcompoffset : 5;
    /* dq5ronpupcompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to Dq5 Ron Comp Value.
     */
    UINT32 dq6ronpupcompoffset : 5;
    /* dq6ronpupcompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-up offset added to DQ6 Ron Comp Value.
     */
    UINT32 dq7ronpupcompoffset : 5;
    /* dq7ronpupcompoffset - Bits[19:15], RW_L, default = 2'b00 
       2s Compliment Pull-up offset added to DQ7 Ron Comp Value.
     */
    UINT32 dqs1ronpupcompoffset : 5;
    /* dqs1ronpupcompoffset - Bits[24:20], RW_L, default = 2'b00 
       2s Compliment Pull-up offset added to DQs1 Ron Comp Value.
     */
    UINT32 enperbitron : 1;
    /* enperbitron - Bits[25:25], RW_L, default = 1'b0 
       Enable per bit RON comp.
     */
    UINT32 spare : 6;
    /* spare - Bits[31:26], RW_L, default = 1'b0 
       spare
     */
  } Bits;
  UINT32 Data;
} DATARONCOMPOFFSET2_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET3_8_MCIO_DDRIOEXT_REG supported on:                       */
/*       BDX (0x401A55D0)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET3_8_MCIO_DDRIOEXT_REG 0x0D0345D0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Per BIT RON Comp offset values.
 */
typedef union {
  struct {
    UINT32 dq4ronpdncompoffset : 5;
    /* dq4ronpdncompoffset - Bits[4:0], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq4 Ron Comp Value.
     */
    UINT32 dq5ronpdncompoffset : 5;
    /* dq5ronpdncompoffset - Bits[9:5], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to Dq5 Ron Comp Value.
     */
    UINT32 dq6ronpdncompoffset : 5;
    /* dq6ronpdncompoffset - Bits[14:10], RW_L, default = 1'b0 
       2s Compliment Pull-down offset added to DQ6 Ron Comp Value.
     */
    UINT32 dq7ronpdncompoffset : 5;
    /* dq7ronpdncompoffset - Bits[19:15], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQ7 Ron Comp Value.
     */
    UINT32 dqs1ronpdncompoffset : 5;
    /* dqs1ronpdncompoffset - Bits[24:20], RW_L, default = 5'd0 
       2s Compliment Pull-up offset added to DQs1 Ron Comp Value.
     */
    UINT32 spare : 7;
    /* spare - Bits[31:25], RW_L, default = 1'b0 
       spare.
     */
  } Bits;
  UINT32 Data;
} DATARONCOMPOFFSET3_8_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DDRCRCMDCOMP_CMDN_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5600)                                                     */
/*       BDX (0x401A5600)                                                     */
/* Register default value on HSX:       0x00000040                            */
/* Register default value on BDX:       0x00000020                            */
#define DDRCRCMDCOMP_CMDN_MCIO_DDRIOEXT_REG 0x0D034600

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CMD Comp Values
 */
typedef union {
  struct {
    UINT32 scomp : 6;
    /* scomp - Bits[5:0], RW_L, default = 1'b0 
       Scomp Value. Normally updated by Comp FSM via CompBus. can be overriden via 
       message channel if periodical retrain is disabled. 
     */
    UINT32 tcocomp : 7;
    /* tcocomp - Bits[12:6], RW_L, default = 1'b0 
       Tco Comp Value. Normally updated by Comp FSM via CompBus. can be overriden via 
       message channel if periodical retrain is disabled. 
     */
    UINT32 rcompdrvup : 6;
    /* rcompdrvup - Bits[18:13], RW_L, default = 1'b0 
       Rcomp Drv Up Value. Normally updated by Comp FSM via CompBus. can be overriden 
       via message channel if periodical retrain is disabled. 
     */
    UINT32 rcompdrvdown : 6;
    /* rcompdrvdown - Bits[24:19], RW_L, default = 1'b0 
       Rcomp Drv Dn Value. Normally updated by Comp FSM via CompBus. can be overriden 
       via message channel if periodical retrain is disabled. 
     */
    UINT32 lscomp : 3;
    /* lscomp - Bits[27:25], RW_L, default = 3'b010 
       not used controlled by Dll fuse
     */
    UINT32 spare : 4;
    /* spare - Bits[31:28], RW_L, default = 1'b0 
       Spare
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCOMP_CMDN_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A5608)                                                     */
/*       BDX (0x401A5608)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXT_REG 0x0D034608

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CMD Pi Settings
 */
typedef union {
  struct {
    UINT32 cmdpicode0 : 6;
    /* cmdpicode0 - Bits[5:0], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay0 : 2;
    /* cmdpilogicdelay0 - Bits[7:6], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_8 : 1;
    UINT32 cmdpicode1 : 6;
    /* cmdpicode1 - Bits[14:9], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay1 : 2;
    /* cmdpilogicdelay1 - Bits[16:15], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_17 : 1;
    UINT32 cmdpicode2 : 6;
    /* cmdpicode2 - Bits[23:18], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay2 : 2;
    /* cmdpilogicdelay2 - Bits[25:24], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_26 : 6;
  } Bits;
  UINT32 Data;
} DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A560C)                                                     */
/*       BDX (0x401A560C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG 0x0D03460C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * CMD Controls
 */
typedef union {
  struct {
    UINT32 ddrphasextalkenableqnnnh : 1;
    /* ddrphasextalkenableqnnnh - Bits[0:0], RW_L, default = 1'b0 
       When Phased based xtalk cancellation is enabled, set this to delay cmd to 
       maintain constant Tcwl. 
     */
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider in xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider in xover 2to2 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       adding extra pipeline stage after xover to avoid timing failure at higher ClkPi 
       rate 
     */
    UINT32 rsvd_4 : 2;
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[6:6], RW_L, default = 1'b0 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 txon : 1;
    /* txon - Bits[7:7], RW_L, default = 1'b0 
       Forces the CMD transmitter on, overriding any power downs
     */
    UINT32 intclkon : 1;
    /* intclkon - Bits[8:8], RW_L, default = 1'b0 
       Forces on all PI enables and the LCB enables in the AFE fubs.  Does not force on 
       D0 PI enable or override weaklock mode. 
     */
    UINT32 repclkon : 1;
    /* repclkon - Bits[9:9], RW_L, default = 1'b0 
       not used.
     */
    UINT32 iolbctl : 2;
    /* iolbctl - Bits[11:10], RW_L, default = 1'b0 
       Controls IO Loopback.  Ctl[1] enables IOLB mode and Ctl[0] controls if this is a 
       Setup(0) or Hold(1) test 
     */
    UINT32 odtmode : 1;
    /* odtmode - Bits[12:12], RW_L, default = 1'b0 
       Forces the driver in ODT mode, where both the pullup and pulldown are enabled.  
       Used for Rstray testing 
     */
    UINT32 cmdtxeq : 2;
    /* cmdtxeq - Bits[14:13], RW_L, default = 1'b0 
       not used.
     */
    UINT32 earlyweakdrive : 2;
    /* earlyweakdrive - Bits[16:15], RW_L, default = 1'b0 
       Enables ODT pulse before/after command transmit. 
       	             Bit 0 (15) is used to enable ODT pulse before transmit. 
       	             Bit 1 (16) is used to enable ODT pulse after transmit. 
       	             NOTE: BROKEN - must be set to 2'b00.
     */
    UINT32 lvmode : 2;
    /* lvmode - Bits[18:17], RW_L, default = None 
       not used.
     */
    UINT32 lvmodevalid : 1;
    /* lvmodevalid - Bits[19:19], RW_L, default = None 
       not used.
     */
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[20:20], RW_L, default = None 
       level shift hold enable
                            Holds the HV control values and power down mode the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rxvref : 7;
    /* rxvref - Bits[27:21], RW_L, default = 6'b0 
       IOLB Vref Control.  2s compliment encoding from 1/3 Vdd to 2/3 Vdd with step 
       size of Vdd/192 (7.8mV) 
     */
    UINT32 vccddqhi : 1;
    /* vccddqhi - Bits[28:28], RW_L, default = 1'b0 
       not used.
     */
    UINT32 paritydisable : 1;
    /* paritydisable - Bits[29:29], RW_L, default = 1'b0 
        disable parity and par_err pins for certain dimm types.
     */
    UINT32 ddr4modeenable : 1;
    /* ddr4modeenable - Bits[30:30], RW_L, default = 1'b0 
       Ddr4 Mode Enable, set to 1 in CMD2 (Func:5.Addr:C00 and Func:7.Addr:C00) fub to 
       bypass extra delay on parity bit 
     */
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CMD Controls
 */
typedef union {
  struct {
    UINT32 ddrpxcenable : 1;
    /* ddrpxcenable - Bits[0:0], RW_L, default = 1'b0 
       When Phased based xtalk cancellation is enabled, set this to delay cmd to 
       maintain constant Tcwl. 
     */
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider in xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider in xover 2to2 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       adding extra pipeline stage after xover to avoid timing failure at higher ClkPi 
       rate 
     */
    UINT32 rsvd_4 : 2;
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[6:6], RW_L, default = 1'b0 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 txon : 1;
    /* txon - Bits[7:7], RW_L, default = 1'b0 
       Forces the CMD transmitter on, overriding any power downs
     */
    UINT32 intclkon : 1;
    /* intclkon - Bits[8:8], RW_L, default = 1'b0 
       Forces on all PI enables and the LCB enables in the AFE fubs.  Does not force on 
       D0 PI enable or override weaklock mode. 
     */
    UINT32 repclkon : 1;
    /* repclkon - Bits[9:9], RW_L, default = 1'b0 
       not used.
     */
    UINT32 iolbctl : 2;
    /* iolbctl - Bits[11:10], RW_L, default = 1'b0 
       Controls IO Loopback.  Ctl[1] enables IOLB mode and Ctl[0] controls if this is a 
       Setup(0) or Hold(1) test 
     */
    UINT32 odtmode : 1;
    /* odtmode - Bits[12:12], RW_L, default = 1'b0 
       Forces the driver in ODT mode, where both the pullup and pulldown are enabled.  
       Used for Rstray testing 
     */
    UINT32 cmdtxeq : 2;
    /* cmdtxeq - Bits[14:13], RW_L, default = 1'b0 
       not used.
     */
    UINT32 earlyweakdrive : 2;
    /* earlyweakdrive - Bits[16:15], RW_L, default = 1'b0 
       Enables ODT pulse before/after command transmit. 
       	             Bit 0 (15) is used to enable ODT pulse before transmit. 
       	             Bit 1 (16) is used to enable ODT pulse after transmit. 
       	             NOTE: BROKEN - must be set to 2'b00.
     */
    UINT32 lvmode : 2;
    /* lvmode - Bits[18:17], RW_L, default = None 
       not used.
     */
    UINT32 lvmodevalid : 1;
    /* lvmodevalid - Bits[19:19], RW_L, default = None 
       not used.
     */
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[20:20], RW_L, default = None 
       level shift hold enable
                            Holds the HV control values and power down mode the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rxvref : 7;
    /* rxvref - Bits[27:21], RW_L, default = 6'b0 
       IOLB Vref Control.  2s compliment encoding from 1/3 Vdd to 2/3 Vdd with step 
       size of Vdd/192 (7.8mV) 
     */
    UINT32 sr_gate_2to2 : 1;
    /* sr_gate_2to2 - Bits[28:28], RW_L, default = 1'b0 
       Used to ungate trainreset into crossover for 2:2 mode
     */
    UINT32 paritydisable : 1;
    /* paritydisable - Bits[29:29], RW_L, default = 1'b0 
        disable parity and par_err pins for certain dimm types. This must be set to 1 
       by BIOS for A0 to avoid Vref being enabled in non-ACIO loopback mode. 
     */
    UINT32 ddr4modeenable : 1;
    /* ddr4modeenable - Bits[30:30], RW_L, default = 1'b0 
       Ddr4 Mode Enable, set to 1 in CMD2 (Func:5.Addr:C00 and Func:7.Addr:C00) fub to 
       bypass extra delay on parity bit 
     */
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDCONTROLS2_CMDN_MCIO_DDRIOEXT_REG supported on:                     */
/*       HSX (0x401A5610)                                                     */
/*       BDX (0x401A5610)                                                     */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00001000                            */
#define DDRCRCMDCONTROLS2_CMDN_MCIO_DDRIOEXT_REG 0x0D034610
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * CMD Controls
 */
typedef union {
  struct {
    UINT32 ddrcrinvertpiclk : 12;
    /* ddrcrinvertpiclk - Bits[11:0], RW_L, default = 12'b0 
       Invert PiClk (used for 2:1, 2:2 when we need to invert the piclk.
     */
    UINT32 spare : 20;
    /* spare - Bits[31:12], RW_L, default = 20'b0 
       reserved.
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS2_CMDN_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CMD Controls
 */
typedef union {
  struct {
    UINT32 ddrcrinvertpiclk : 12;
    /* ddrcrinvertpiclk - Bits[11:0], RW_L, default = 12'b0 
       Invert PiClk (used for 2:1, 2:2 when we need to invert the piclk.
     */
    UINT32 bdx_sys : 1;
    /* bdx_sys - Bits[12:12], RW_L, default = 1'b1 
       BDX change : This bit helps to bypass the delaycell in transmitter
     */
    UINT32 rsvd_13 : 1;
    UINT32 spare : 18;
    /* spare - Bits[31:14], RW_L, default = 18'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS2_CMDN_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDPICODING2_CMDN_MCIO_DDRIOEXT_REG supported on:                     */
/*       HSX (0x401A5618)                                                     */
/*       BDX (0x401A5618)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDPICODING2_CMDN_MCIO_DDRIOEXT_REG 0x0D034618

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CMD Pi Settings
 */
typedef union {
  struct {
    UINT32 cmdpicode3 : 6;
    /* cmdpicode3 - Bits[5:0], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay3 : 2;
    /* cmdpilogicdelay3 - Bits[7:6], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_8 : 1;
    UINT32 cmdpicode4 : 6;
    /* cmdpicode4 - Bits[14:9], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay4 : 2;
    /* cmdpilogicdelay4 - Bits[16:15], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_17 : 1;
    UINT32 cmdpicode5 : 6;
    /* cmdpicode5 - Bits[23:18], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay5 : 2;
    /* cmdpilogicdelay5 - Bits[25:24], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_26 : 6;
  } Bits;
  UINT32 Data;
} DDRCRCMDPICODING2_CMDN_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






/* DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A562C)                                                     */
/*       BDX (0x401A562C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG 0x0D03462C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Cmd Training Register
 */
typedef union {
  struct {
    UINT32 trainingoffset : 6;
    /* trainingoffset - Bits[5:0], RW_L, default = 6'b0 
       Offset added to PiCode. Obtained via xover calibration such that ClkPi aligns 
       with grid clock when PiCode=0. 
     */
    UINT32 rsvd_6 : 2;
    UINT32 xoverphasedet : 14;
    /* xoverphasedet - Bits[21:8], RO_V, default = 14'b0 
       Xover Phase Detect. Xover calibration feedback bits. Actually, using one of 14 
       is sufficient. 
                   [13]   CtlOutEnPath Xover Phase Detect
                   [12]   CmdOutEnPath Xover Phase Detect
                   [11:0] CmdBuf Xover Phase Detects
               
     */
    UINT32 trainingoffset2 : 6;
    /* trainingoffset2 - Bits[27:22], RW_L, default = 6'b0 
       Offset added to PiCode. Obtained via xover calibration such that ClkPi aligns 
       with grid clock when PiCode=0. 
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCMDCONTROLS1_CMDN_MCIO_DDRIOEXT_REG supported on:                     */
/*       HSX (0x401A5630)                                                     */
/*       BDX (0x401A5630)                                                     */
/* Register default value on HSX:       0x00980000                            */
/* Register default value on BDX:       0x00180000                            */
#define DDRCRCMDCONTROLS1_CMDN_MCIO_DDRIOEXT_REG 0x0D034630
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * CMD Controls 1
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    UINT32 internalvrefen : 1;
    /* internalvrefen - Bits[2:2], RW_L, default = 1'b0 
       Enable internal Vref for parity error pin, ACIOLB, or DcDetect (only for CTL)
     */
    UINT32 ddrcrimodeen : 1;
    /* ddrcrimodeen - Bits[3:3], RW_L, default = 1'b0 
       Imode en
     */
    UINT32 ddrcrimodesel : 1;
    /* ddrcrimodesel - Bits[4:4], RW_L, default = 1'b0 
       Imode sel, 0: swing boost; 1: xtalk cancellation.
     */
    UINT32 ddrcrimodesrcntl : 6;
    /* ddrcrimodesrcntl - Bits[10:5], RW_L, default = 6'b0 
       Imode Scomp configuration
     */
    UINT32 ddrcrimodesrcntlovrd : 1;
    /* ddrcrimodesrcntlovrd - Bits[11:11], RW_L, default = 1'b0 
       Imode scomp override enable
     */
    UINT32 ddrcrimodecoef : 4;
    /* ddrcrimodecoef - Bits[15:12], RW_L, default = 1'b0 
       Imode coef, swing boost configure
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_20 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[21:21], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 dqstatdflt : 2;
    /* dqstatdflt - Bits[23:22], RW_L, default = 2'b11 
       Static leg enable
     */
    UINT32 odtstatdflt : 2;
    /* odtstatdflt - Bits[25:24], RW_L, default = None 
       Static leg enable
     */
    UINT32 rsvd_26 : 1;
    UINT32 txeq : 5;
    /* txeq - Bits[31:27], RW_L, default = 5'b0 
       When TxEQOverride = 0, bits [29:27] are used as selects for the RCOMP -> EQCODE 
       calculation: 
       
       	             TxEQ[29:27] = 000 : EQCode = 0
       	             TxEQ[29:27] = 001 : EQCode = -1/4 nominal value  
       	             TxEQ[29:27] = 010 : EQCode = -1/8 nominal value  
       	             TxEQ[29:27] = 101 : EQCode = +1/4 nominal value  
       	             TxEQ[29:27] = 110 : EQCode = +1/8 nominal value  
       
                     Bits [31:30] are used to select between a 5, 3, or 1 
       multiplication factor for use in VMSE/DDR4: 
       
       	             TxEQ[31:30] = 00 : 5 * (comp + 16) / 16
       	             TxEQ[31:30] = 11 : 3 * (comp + 16) / 16
       	             TxEQ[31:30] = 01 : 1 * (comp + 16) / 16
       	             TxEQ[31:30] = 10 : 1 * (comp + 16) / 16
       
       
                            When TxEQOverride = 1, bits [31:27] are used directly as 
       the EQ Code: 
       
                     Write Equalization codes.  5 bit control, but only 24 of them are 
       meaningful.  
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS1_CMDN_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CMD Controls 1
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    UINT32 internalvrefen : 1;
    /* internalvrefen - Bits[2:2], RW_L, default = 1'b0 
       Enable internal Vref for parity error pin, ACIOLB, or DcDetect (only for CTL)
     */
    UINT32 ddrcrimodeen : 1;
    /* ddrcrimodeen - Bits[3:3], RW_L, default = 1'b0 
       Imode en
     */
    UINT32 ddrcrimodesel : 1;
    /* ddrcrimodesel - Bits[4:4], RW_L, default = 1'b0 
       Imode sel, 0: swing boost; 1: xtalk cancellation.
     */
    UINT32 ddrcrimodesrcntl : 6;
    /* ddrcrimodesrcntl - Bits[10:5], RW_L, default = 6'b0 
       Imode Scomp configuration
     */
    UINT32 ddrcrimodesrcntlovrd : 1;
    /* ddrcrimodesrcntlovrd - Bits[11:11], RW_L, default = 1'b0 
       Imode scomp override enable
     */
    UINT32 ddrcrimodecoef : 4;
    /* ddrcrimodecoef - Bits[15:12], RW_L, default = 1'b0 
       Imode coef, swing boost configure
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_20 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[21:21], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 dqstatdflt : 2;
    /* dqstatdflt - Bits[23:22], RW_L, default = 2'b11 
       Static leg enable
     */
    UINT32 odtstatdflt : 2;
    /* odtstatdflt - Bits[25:24], RW_L, default = None 
       Static leg enable
     */
    UINT32 txeq : 6;
    /* txeq - Bits[31:26], RW_L, default = 6'b0 
        Modified in BDX!! When TxEQOverride = 0, bits [29:27] are used as selects for 
       the RCOMP -> EQCODE calculation: 
       
       	             TxEQ[29:27] = 000 : EQCode = 0
       	             TxEQ[29:27] = 001 : EQCode = -1/4 nominal value  
       	             TxEQ[29:27] = 010 : EQCode = -1/8 nominal value  
       	             TxEQ[29:27] = 101 : EQCode = +1/4 nominal value  
       	             TxEQ[29:27] = 110 : EQCode = +1/8 nominal value  
       
                     Bits [31:30] are used to select between a 5, 3, or 1 
       multiplication factor for use in VMSE/DDR4: 
       
       	             TxEQ[31:30] = 00 : 5 * (comp + 16) / 16
       	             TxEQ[31:30] = 11 : 3 * (comp + 16) / 16
       	             TxEQ[31:30] = 01 : 1 * (comp + 16) / 16
       	             TxEQ[31:30] = 10 : 1 * (comp + 16) / 16
       
       
                            When TxEQOverride = 1, bits [31:27] are used directly as 
       the EQ Code: 
       
                     Write Equalization codes.  5 bit control, but only 24 of them are 
       meaningful.  
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS1_CMDN_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCTLCOMP_CKE_MCIO_DDRIOEXT_REG supported on:                           */
/*       HSX (0x401A5810)                                                     */
/*       BDX (0x401A5810)                                                     */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000020                            */
#define DDRCRCTLCOMP_CKE_MCIO_DDRIOEXT_REG 0x0D034810

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CTL Comp Values
 */
typedef union {
  struct {
    UINT32 scomp : 5;
    /* scomp - Bits[4:0], RW_L, default = 1'b0 
       Scomp Value. Normally updated by Comp FSM via CompBus. can be overriden via 
       message channel if periodical retrain is disabled. 
     */
    UINT32 tcocomp : 7;
    /* tcocomp - Bits[11:5], RW_L, default = 1'b0 
       Tco Comp Value. Normally updated by Comp FSM via CompBus. can be overriden via 
       message channel if periodical retrain is disabled. 
     */
    UINT32 rcompdrvup : 6;
    /* rcompdrvup - Bits[17:12], RW_L, default = 1'b0 
       Rcomp Drv Up Value. Normally updated by Comp FSM via CompBus. can be overriden 
       via message channel if periodical retrain is disabled. 
     */
    UINT32 rcompdrvdown : 6;
    /* rcompdrvdown - Bits[23:18], RW_L, default = 1'b0 
       Rcomp Drv Dn Value. Normally updated by Comp FSM via CompBus. can be overriden 
       via message channel if periodical retrain is disabled. 
     */
    UINT32 lscomp : 3;
    /* lscomp - Bits[26:24], RW_L, default = 3'b010 
       used in Rx Amplifier. Need for Loopback operation only.
     */
    UINT32 spare : 4;
    /* spare - Bits[30:27], RW_L, default = 4'b0 
       Spare
     */
    UINT32 vmseallowcompupdatetxinject : 1;
    /* vmseallowcompupdatetxinject - Bits[31:31], RW_L, default = 1'b0 
       Chicken bit to allow injecting stable data into TX during VMSE CompUpdate. 
                     '1' allows injection of unscrambled NOOPS into TX, and should only 
       be needed when VCMD scrambling is enabled. 
                     '0' disables data injection, which is safe when VCMD scrambling is 
       not enabled 
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLCOMP_CKE_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* DDRCRCTLPICODING_CKE_MCIO_DDRIOEXT_REG supported on:                       */
/*       HSX (0x401A5818)                                                     */
/*       BDX (0x401A5818)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING_CKE_MCIO_DDRIOEXT_REG 0x0D034818

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CTL Pi Settings
 */
typedef union {
  struct {
    UINT32 ctlpicode0 : 6;
    /* ctlpicode0 - Bits[5:0], RW_L, default = 6'b0 
       Ctl Pi Code0
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay0 : 2;
    /* ctlpilogicdelay0 - Bits[7:6], RW_L, default = 2'b0 
       Ctl Pi Logic Delay0 - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode1 : 6;
    /* ctlpicode1 - Bits[13:8], RW_L, default = 6'b0 
       Ctl Pi Code1
     */
    UINT32 ctlpilogicdelay1 : 2;
    /* ctlpilogicdelay1 - Bits[15:14], RW_L, default = 2'b0 
       Ctl Pi Logic Delay1 - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode2 : 6;
    /* ctlpicode2 - Bits[21:16], RW_L, default = 6'b0 
       Ctl Pi Code2
     */
    UINT32 ctlpilogicdelay2 : 2;
    /* ctlpilogicdelay2 - Bits[23:22], RW_L, default = 2'b0 
       Ctl Pi Logic Delay2 - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode3 : 6;
    /* ctlpicode3 - Bits[29:24], RW_L, default = 6'b0 
       Ctl Pi Code3
     */
    UINT32 ctlpilogicdelay3 : 2;
    /* ctlpilogicdelay3 - Bits[31:30], RW_L, default = 2'b0 
       Ctl Pi Logic Delay3 - add 1~3 qclk cycles delay.
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLPICODING_CKE_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG supported on:                       */
/*       HSX (0x401A581C)                                                     */
/*       BDX (0x401A581C)                                                     */
/* Register default value:              0x10010000                            */
#define DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG 0x0D03481C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * CTL Controls
 */
typedef union {
  struct {
    UINT32 ddrphasextalkenableqnnnh : 1;
    /* ddrphasextalkenableqnnnh - Bits[0:0], RW_L, default = 1'b0 
       When Phased based xtalk cancellation is enabled, set this to delay cmd to 
       maintain constant Tcwl. 
     */
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider to enable xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider to enable xover 2to2 and 2to1 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       enable the extra flop stage in xover to help timing
     */
    UINT32 rsvd_4 : 17;
    UINT32 rxvref : 7;
    /* rxvref - Bits[27:21], RW_L, default = 7'b0 
       IOLB Vref Control. binary code from 1/3 Vdd to 2/3 Vdd with step size of 
       Vdd/192(??mV) 
     */
    UINT32 rsvd_28 : 1;
    UINT32 paritydisable : 1;
    /* paritydisable - Bits[29:29], RW_L, default = 1'b1 
       disable parity and par_err pins to save power when the populated dimm type does 
       not support parity 
     */
    UINT32 rsvd_30 : 1;
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CTL Controls
 */
typedef union {
  struct {
    UINT32 ddrpxcenable : 1;
    /* ddrpxcenable - Bits[0:0], RW_L, default = 1'b0 
       When Phased based xtalk cancellation is enabled, set this to delay cmd to 
       maintain constant Tcwl. 
     */
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider to enable xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider to enable xover 2to2 and 2to1 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       enable the extra flop stage in xover to help timing
     */
    UINT32 rsvd_4 : 13;
    UINT32 ctltxeq : 2;
    /* ctltxeq - Bits[18:17], RW_L, default = 2'b1 
       When LSB is set to 1, uses TxEQ register (DdrCrCmdControls1[31:27]) as the EQ 
       code rather than the  
       	             EQ code calculated from the CTL DRVUP comp code.
     */
    UINT32 rsvd_19 : 2;
    UINT32 rxvref : 7;
    /* rxvref - Bits[27:21], RW_L, default = 7'b0 
       IOLB Vref Control. binary code from 1/3 Vdd to 2/3 Vdd with step size of 
       Vdd/192(??mV) 
     */
    UINT32 sr_gate_2to2 : 1;
    /* sr_gate_2to2 - Bits[28:28], RW_L, default = 1'b0 
       Used to ungate trainreset into xover for 2:2 mode
     */
    UINT32 paritydisable : 1;
    /* paritydisable - Bits[29:29], RW_L, default = 1'b1 
       disable parity and par_err pins to save power when the populated dimm type does 
       not support parity 
     */
    UINT32 rsvd_30 : 1;
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A5820)                                                     */
/*       BDX (0x401A5820)                                                     */
/* Register default value on HSX:       0x00004FFF                            */
/* Register default value on BDX:       0xC0004FFF                            */
#define DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_REG 0x0D034820
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * CTL Ranks Used
 */
typedef union {
  struct {
    UINT32 ranken : 12;
    /* ranken - Bits[11:0], RW_L, default = 12'b111111111111 
       Enables output buffers for ctlbuf 0 to ctlbuf 11, PI clocks and output datapaths 
       for this rank 
     */
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[12:12], RW_L, default = 1'b0 
       level shift hold enable:
                            Holds the HV control values and power down the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rxbiasgcncomp : 2;
    /* rxbiasgcncomp - Bits[14:13], RW_L, default = 2'b10 
       used to compensate the GCN skew. The GCN skew information should be extracted by 
       reading and analyzing the final Rcomp code value. The target spec is to 
       categorize the GCN skew in 3 process condition (mainly slow, fast, typical). 
       (one example can be just reading the 2 MLS bit of Rcomp code and determine if 
       the GCN is slow, fast or typical). The gcncomp is used for CTLE. 
       00: gcn in typical corner
       11: gcn in slow corner
       10: gcn in typical corner
       01: gcn in fast corner
               
     */
    UINT32 rsvd_15 : 1;
    UINT32 imodebiashighcm : 1;
    /* imodebiashighcm - Bits[16:16], RW_L, default = 1'b0 
       Imode bias control
     */
    UINT32 rxbiassel : 1;
    /* rxbiassel - Bits[17:17], RW_L, default = 1'b0 
       Rx bias Sel
     */
    UINT32 ddrcrinvertpiclk : 12;
    /* ddrcrinvertpiclk - Bits[29:18], RW_L, default = 12'b0 
       Invert PiClk in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 phase
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[30:30], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CTL Ranks Used
 */
typedef union {
  struct {
    UINT32 ranken : 12;
    /* ranken - Bits[11:0], RW_L, default = 12'b111111111111 
       Enables output buffers for ctlbuf 0 to ctlbuf 11, PI clocks and output datapaths 
       for this rank 
     */
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[12:12], RW_L, default = 1'b0 
       level shift hold enable:
                            Holds the HV control values and power down the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rxbiasgcncomp : 2;
    /* rxbiasgcncomp - Bits[14:13], RW_L, default = 2'b10 
       used to compensate the GCN skew. The GCN skew information should be
                 extracted by reading and analyzing the final Rcomp code value. The 
       target spec 
                 is to categorize the GCN skew in 3 process condition (mainly slow, 
       fast, typical). 
                 (one example can be just reading the 2 MLS bit of Rcomp code and 
       determine if the 
                 GCN is slow, fast or typical). The gcncomp is used for CTLE.
                 11: gcn in slow corner
                 10: gcn in typical corner
                 01: gcn in fast corner
               
     */
    UINT32 rsvd_15 : 1;
    UINT32 imodebiashighcm : 1;
    /* imodebiashighcm - Bits[16:16], RW_L, default = 1'b0 
       Imode bias control
     */
    UINT32 rxbiassel : 1;
    /* rxbiassel - Bits[17:17], RW_L, default = 1'b0 
       Rx bias Sel
     */
    UINT32 ddrcrinvertpiclk : 12;
    /* ddrcrinvertpiclk - Bits[29:18], RW_L, default = 12'b0 
       Invert PiClk in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 phase
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[30:30], RW_L, default = 1'b1 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */







/* DDRCRCTLTRAINING_CKE_MCIO_DDRIOEXT_REG supported on:                       */
/*       HSX (0x401A582C)                                                     */
/*       BDX (0x401A582C)                                                     */
/* Register default value:              0x00007000                            */
#define DDRCRCTLTRAINING_CKE_MCIO_DDRIOEXT_REG 0x0D03482C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Ctl Training Register
 */
typedef union {
  struct {
    UINT32 trainingoffset : 6;
    /* trainingoffset - Bits[5:0], RW_L, default = 6'b0 
       Offset added to PiCode. Obtained via xover calibration such that ClkPi aligns 
       with grid clock when PiCode=0. 
     */
    UINT32 dcdetectmode : 1;
    /* dcdetectmode - Bits[6:6], RW_L, default = None 
       Enables VMSE init DcDetect mode
     */
    UINT32 rsvd_7 : 1;
    UINT32 xoverphasedet : 12;
    /* xoverphasedet - Bits[19:8], RO_V, default = 1'b0 
       Xover Phase Detect. Xover calibration feedback bits.
     */
    UINT32 ctldrvsegen : 3;
    /* ctldrvsegen - Bits[22:20], RW_L, default = 3'b000 
       drvsegdisable, or drvSegEn_b. 
     */
    UINT32 ckepddrvsegen : 3;
    /* ckepddrvsegen - Bits[25:23], RW_L, default = 3'b111 
        Controls the number of drive segments enabled during CKE powerdown 
     */
    UINT32 rsvd_26 : 6;
  } Bits;
  UINT32 Data;
} DDRCRCTLTRAINING_CKE_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Ctl Training Register
 */
typedef union {
  struct {
    UINT32 trainingoffset : 6;
    /* trainingoffset - Bits[5:0], RW_L, default = 6'b0 
       Offset added to PiCode. Obtained via xover calibration such that ClkPi aligns 
       with grid clock when PiCode=0. 
     */
    UINT32 dcdetectmode : 1;
    /* dcdetectmode - Bits[6:6], RW_L, default = None 
       Enables VMSE init DcDetect mode
     */
    UINT32 rsvd_7 : 1;
    UINT32 xoverphasedet : 12;
    /* xoverphasedet - Bits[19:8], RO_V, default = 1'b0 
       Xover Phase Detect. Xover calibration feedback bits.
     */
    UINT32 ctldrvsegen : 3;
    /* ctldrvsegen - Bits[22:20], RW_L, default = 3'b000 
       drvsegdisable, or drvSegEn_b. 
     */
    UINT32 ckepddrvsegen : 3;
    /* ckepddrvsegen - Bits[25:23], RW_L, default = 3'b111 
        Controls the number of drive segments enabled during CKE powerdown 
     */
    UINT32 rsvd_26 : 6;
  } Bits;
  UINT32 Data;
} DDRCRCTLTRAINING_CKE_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A5830)                                                     */
/*       BDX (0x401A5830)                                                     */
/* Register default value on HSX:       0x00980000                            */
/* Register default value on BDX:       0x00180000                            */
#define DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXT_REG 0x0D034830
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * CMD Controls 1
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    UINT32 internalvrefen : 1;
    /* internalvrefen - Bits[2:2], RW_L, default = 1'b0 
       Enable internal Vref for parity error pin, ACIOLB, or DcDetect (only for CTL)
     */
    UINT32 ddrcrimodeen : 1;
    /* ddrcrimodeen - Bits[3:3], RW_L, default = 1'b0 
       Imode en
     */
    UINT32 ddrcrimodesel : 1;
    /* ddrcrimodesel - Bits[4:4], RW_L, default = 1'b0 
       Imode sel, 0: swing boost; 1: xtalk cancellation.
     */
    UINT32 ddrcrimodesrcntl : 6;
    /* ddrcrimodesrcntl - Bits[10:5], RW_L, default = 6'b0 
       Imode Scomp configuration
     */
    UINT32 ddrcrimodesrcntlovrd : 1;
    /* ddrcrimodesrcntlovrd - Bits[11:11], RW_L, default = 1'b0 
       Imode scomp override enable
     */
    UINT32 ddrcrimodecoef : 4;
    /* ddrcrimodecoef - Bits[15:12], RW_L, default = 1'b0 
       Imode coef, swing boost configure
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_20 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[21:21], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 dqstatdflt : 2;
    /* dqstatdflt - Bits[23:22], RW_L, default = 2'b11 
       Static leg enable
     */
    UINT32 odtstatdflt : 2;
    /* odtstatdflt - Bits[25:24], RW_L, default = None 
       Static leg enable
     */
    UINT32 rsvd_26 : 1;
    UINT32 txeq : 5;
    /* txeq - Bits[31:27], RW_L, default = 5'b0 
       When TxEQOverride = 0, bits [29:27] are used as selects for the RCOMP -> EQCODE 
       calculation: 
       
       	             TxEQ[29:27] = 000 : EQCode = 0
       	             TxEQ[29:27] = 001 : EQCode = -1/4 nominal value  
       	             TxEQ[29:27] = 010 : EQCode = -1/8 nominal value  
       	             TxEQ[29:27] = 101 : EQCode = +1/4 nominal value  
       	             TxEQ[29:27] = 110 : EQCode = +1/8 nominal value  
       
                     Bits [31:30] are used to select between a 5, 3, or 1 
       multiplication factor for use in VMSE/DDR4: 
       
       	             TxEQ[31:30] = 00 : 5 * (comp + 16) / 16
       	             TxEQ[31:30] = 11 : 3 * (comp + 16) / 16
       	             TxEQ[31:30] = 01 : 1 * (comp + 16) / 16
       	             TxEQ[31:30] = 10 : 1 * (comp + 16) / 16
       
       
                            When TxEQOverride = 1, bits [31:27] are used directly as 
       the EQ Code: 
       
                     Write Equalization codes.  5 bit control, but only 24 of them are 
       meaningful.  
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CMD Controls 1
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    UINT32 internalvrefen : 1;
    /* internalvrefen - Bits[2:2], RW_L, default = 1'b0 
       Enable internal Vref for parity error pin, ACIOLB, or DcDetect (only for CTL)
     */
    UINT32 ddrcrimodeen : 1;
    /* ddrcrimodeen - Bits[3:3], RW_L, default = 1'b0 
       Imode en
     */
    UINT32 ddrcrimodesel : 1;
    /* ddrcrimodesel - Bits[4:4], RW_L, default = 1'b0 
       Imode sel, 0: swing boost; 1: xtalk cancellation.
     */
    UINT32 ddrcrimodesrcntl : 6;
    /* ddrcrimodesrcntl - Bits[10:5], RW_L, default = 6'b0 
       Imode Scomp configuration
     */
    UINT32 ddrcrimodesrcntlovrd : 1;
    /* ddrcrimodesrcntlovrd - Bits[11:11], RW_L, default = 1'b0 
       Imode scomp override enable
     */
    UINT32 ddrcrimodecoef : 4;
    /* ddrcrimodecoef - Bits[15:12], RW_L, default = 1'b0 
       Imode coef, swing boost configure
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_20 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[21:21], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 dqstatdflt : 2;
    /* dqstatdflt - Bits[23:22], RW_L, default = 2'b11 
       Static leg enable
     */
    UINT32 odtstatdflt : 2;
    /* odtstatdflt - Bits[25:24], RW_L, default = None 
       Static leg enable
     */
    UINT32 txeq : 6;
    /* txeq - Bits[31:26], RW_L, default = 6'b0 
        Modified in BDX!! When TxEQOverride = 0, bits [29:27] are used as selects for 
       the RCOMP -> EQCODE calculation: 
       
       	             TxEQ[29:27] = 000 : EQCode = 0
       	             TxEQ[29:27] = 001 : EQCode = -1/4 nominal value  
       	             TxEQ[29:27] = 010 : EQCode = -1/8 nominal value  
       	             TxEQ[29:27] = 101 : EQCode = +1/4 nominal value  
       	             TxEQ[29:27] = 110 : EQCode = +1/8 nominal value  
       
                     Bits [31:30] are used to select between a 5, 3, or 1 
       multiplication factor for use in VMSE/DDR4: 
       
       	             TxEQ[31:30] = 00 : 5 * (comp + 16) / 16
       	             TxEQ[31:30] = 11 : 3 * (comp + 16) / 16
       	             TxEQ[31:30] = 01 : 1 * (comp + 16) / 16
       	             TxEQ[31:30] = 10 : 1 * (comp + 16) / 16
       
       
                            When TxEQOverride = 1, bits [31:27] are used directly as 
       the EQ Code: 
       
                     Write Equalization codes.  5 bit control, but only 24 of them are 
       meaningful.  
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCTLPICODING2_CKE_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A5834)                                                     */
/*       BDX (0x401A5834)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING2_CKE_MCIO_DDRIOEXT_REG 0x0D034834

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CTL Pi Settings
 */
typedef union {
  struct {
    UINT32 ctlpicode4 : 6;
    /* ctlpicode4 - Bits[5:0], RW_L, default = 6'b0 
       Ctl Pi Code4
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay4 : 2;
    /* ctlpilogicdelay4 - Bits[7:6], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode5 : 6;
    /* ctlpicode5 - Bits[13:8], RW_L, default = 6'b0 
       Ctl Pi Code5
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay5 : 2;
    /* ctlpilogicdelay5 - Bits[15:14], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode6 : 6;
    /* ctlpicode6 - Bits[21:16], RW_L, default = 6'b0 
       Ctl Pi Code6
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay6 : 2;
    /* ctlpilogicdelay6 - Bits[23:22], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode7 : 6;
    /* ctlpicode7 - Bits[29:24], RW_L, default = 6'b0 
       Ctl Pi Code7
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay7 : 2;
    /* ctlpilogicdelay7 - Bits[31:30], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLPICODING2_CKE_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCTLPICODING3_CKE_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A5838)                                                     */
/*       BDX (0x401A5838)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING3_CKE_MCIO_DDRIOEXT_REG 0x0D034838

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CTL Pi Settings
 */
typedef union {
  struct {
    UINT32 ctlpicode8 : 6;
    /* ctlpicode8 - Bits[5:0], RW_L, default = 6'b0 
       Ctl Pi Code8
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay8 : 2;
    /* ctlpilogicdelay8 - Bits[7:6], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode9 : 6;
    /* ctlpicode9 - Bits[13:8], RW_L, default = 6'b0 
       Ctl Pi Code9
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay9 : 2;
    /* ctlpilogicdelay9 - Bits[15:14], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode10 : 6;
    /* ctlpicode10 - Bits[21:16], RW_L, default = 6'b0 
       Ctl Pi Code10
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay10 : 2;
    /* ctlpilogicdelay10 - Bits[23:22], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} DDRCRCTLPICODING3_CKE_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* DDRCRCTLPICODING4_CKE_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A5840)                                                     */
/*       BDX (0x401A5840)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING4_CKE_MCIO_DDRIOEXT_REG 0x0D034840

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CTL Pi Settings 4 
 */
typedef union {
  struct {
    UINT32 vmselogicdelay : 12;
    /* vmselogicdelay - Bits[11:0], RW_L, default = None 
       VmseLogicDelay - add 2x clock cycle delay per vcmd bit
     */
    UINT32 rsvd_12 : 2;
    UINT32 vmsephase1cmdlogicdelay : 12;
    /* vmsephase1cmdlogicdelay - Bits[25:14], RW_L, default = 12'h0 
       When this is set, add one Qclk delay to the Intel SMI 2 
                   phase1 VCMD in case of a polarity inversion in serializer.
                
     */
    UINT32 flipvmseserializerphase : 1;
    /* flipvmseserializerphase - Bits[26:26], RW_L, default = 1'b0 
       When this is set, flip the phase of the serializer for VMSE 1:1 mode.
                
     */
    UINT32 rsvd_27 : 5;
  } Bits;
  UINT32 Data;
} DDRCRCTLPICODING4_CKE_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5A00)                                                     */
/*       BDX (0x401A5A00)                                                     */
/* Register default value:              0x0000070F                            */
#define DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG 0x0D034A00

#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CLK Ranks Used
 */
typedef union {
  struct {
    UINT32 ranken : 4;
    /* ranken - Bits[3:0], RW_L, default = 4'b1111
       Enables output buffers, PI clocks and output datapaths for this rank
     */
    UINT32 ddrcrinvertpiclk : 4;
    /* ddrcrinvertpiclk - Bits[7:4], RW_L, default = 4'b0
       Invert PiClk in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 phase
     */
    UINT32 drvsegen : 3;
    /* drvsegen - Bits[10:8], RW_L, default = 3'b111
       Segments to enable for driving (inverse for ODT)
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[11:11], RW_L, default = None
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 rsvd_12 : 20;
  } Bits;
  UINT32 Data;
} DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CLK Ranks Used
 */
typedef union {
  struct {
    UINT32 ranken : 4;
    /* ranken - Bits[3:0], RW_L, default = 4'b1111 
       Enables output buffers, PI clocks and output datapaths for this rank
     */
    UINT32 ddrcrinvertpiclk : 4;
    /* ddrcrinvertpiclk - Bits[7:4], RW_L, default = 4'b0 
       Invert PiClk in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 phase
     */
    UINT32 drvsegen : 3;
    /* drvsegen - Bits[10:8], RW_L, default = 3'b111 
       Segments to enable for driving (inverse for ODT)
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[11:11], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 disablepc6fix : 1;
    /* disablepc6fix - Bits [12:12], RW_L, default = 1'b1
     BDX change. Chicken bit to disable pc6 fix added for ma0 stepping
     */
    UINT32 rsvd_13 : 19;
  } Bits;
  UINT32 Data;
} DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */

/* DDRCRCLKCOMP_CLK_MCIO_DDRIOEXT_REG supported on:                           */
/*       HSX (0x401A5A04)                                                     */
/*       BDX (0x401A5A04)                                                     */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x02000000                            */
#define DDRCRCLKCOMP_CLK_MCIO_DDRIOEXT_REG 0x0D034A04

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CLK Comp Values
 */
typedef union {
  struct {
    UINT32 scomp : 5;
    /* scomp - Bits[4:0], RW_L, default = 5'h0 
       Scomp Value. Normally updated by Comp FSM via CompBus. can be overriden via 
       message channel if periodical retrain is disabled.  
     */
    UINT32 tcocomp : 7;
    /* tcocomp - Bits[11:5], RW_L, default = 7'h0 
       Tco Comp Value
     */
    UINT32 rcompdrvup : 6;
    /* rcompdrvup - Bits[17:12], RW_L, default = 6'h0 
       Rcomp Drv Up Value. Normally updated by Comp FSM via CompBus. can be overriden 
       via message channel if periodical retrain is disabled.  
     */
    UINT32 rcompdrvdown : 6;
    /* rcompdrvdown - Bits[23:18], RW_L, default = 6'h0 
       Rcomp Drv Dn Value. Normally updated by Comp FSM via CompBus. can be overriden 
       via message channel if periodical retrain is disabled.  
     */
    UINT32 lscomp : 3;
    /* lscomp - Bits[26:24], RW_L, default = 3'b010 
       LevelShift Comp Value. Not unsed, controlled via MsgCh only.
     */
    UINT32 spare : 5;
    /* spare - Bits[31:27], RW_L, default = 5'h0 
       Spare
     */
  } Bits;
  UINT32 Data;
} DDRCRCLKCOMP_CLK_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* DDRCRCLKPICODE_MCIO_DDRIOEXT_REG supported on:                             */
/*       HSX (0x401A5A0C)                                                     */
/*       BDX (0x401A5A0C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCLKPICODE_MCIO_DDRIOEXT_REG 0x0D034A0C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CLK Pi Codes
 */
typedef union {
  struct {
    UINT32 pisettingrank0 : 6;
    /* pisettingrank0 - Bits[5:0], RW_L, default = 1'b0 
       Pi Setting, Rank 0
                            [5:0] - Pi setting for xxDDR_CLK_P/N[0]
     */
    UINT32 pilogicdelayrank0 : 1;
    /* pilogicdelayrank0 - Bits[6:6], RW_L, default = 1'b0 
       Pi Logic Delay, Rank 0, delays CLK by an extra qclk cycle
     */
    UINT32 rsvd_7 : 1;
    UINT32 pisettingrank1 : 6;
    /* pisettingrank1 - Bits[13:8], RW_L, default = 1'b0 
       Pi Setting, Rank 1
                            [5:0] - Pi setting for xxDDR_CLK_P/N[1]
     */
    UINT32 pilogicdelayrank1 : 1;
    /* pilogicdelayrank1 - Bits[14:14], RW_L, default = 1'b0 
       Pi Logic Delay, Rank 1, delays CLK by an extra qclk cycle
     */
    UINT32 rsvd_15 : 1;
    UINT32 pisettingrank2 : 6;
    /* pisettingrank2 - Bits[21:16], RW_L, default = 1'b0 
       Pi Setting, Rank 2
                            [5:0] - Pi setting for xxDDR_CLK_P/N[2]
     */
    UINT32 pilogicdelayrank2 : 1;
    /* pilogicdelayrank2 - Bits[22:22], RW_L, default = 1'b0 
       Pi Logic Delay, Rank 2, delays CLK by an extra qclk cycle
     */
    UINT32 rsvd_23 : 1;
    UINT32 pisettingrank3 : 6;
    /* pisettingrank3 - Bits[29:24], RW_L, default = 1'b0 
       Pi Setting, Rank 3
                            [5:0] - Pi setting for xxDDR_CLK_P/N[3]
     */
    UINT32 pilogicdelayrank3 : 1;
    /* pilogicdelayrank3 - Bits[30:30], RW_L, default = 1'b0 
       Pi Logic Delay, Rank 3
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DDRCRCLKPICODE_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG supported on:                           */
/*       HSX (0x401A5A10)                                                     */
/*       BDX (0x401A5A10)                                                     */
/* Register default value:              0x00030000                            */
#define DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG 0x0D034A10
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * CLK Controls
 */
typedef union {
  struct {
    UINT32 ddrphasextalkenableqnnnh : 1;
    /* ddrphasextalkenableqnnnh - Bits[0:0], RW_L, default = 1'b0 
       When Phased based xtalk cancellation is enabled, set this to delay cmd to 
       maintain constant Tcwl. 
     */
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider to enable xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider to enable xover 2to2 and 2to1 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       enable the extra flop stage in xover to help timing
     */
    UINT32 dllmask : 2;
    /* dllmask - Bits[5:4], RW_L, default = 1'b0 
       not used
     */
    UINT32 dllrsvd1 : 1;
    /* dllrsvd1 - Bits[6:6], RW_L, default = 1'b0 
       Reserved
     */
    UINT32 txon : 1;
    /* txon - Bits[7:7], RW_L, default = 1'b0 
       Forces the CLK transmitter on, overriding any power downs 
     */
    UINT32 intclkon : 1;
    /* intclkon - Bits[8:8], RW_L, default = 1'b0 
       Forces on all PI enables and the LCB enables in the AFE fubs.
     */
    UINT32 repclkon : 1;
    /* repclkon - Bits[9:9], RW_L, default = 1'b0 
       not used. 
     */
    UINT32 iolbctl : 2;
    /* iolbctl - Bits[11:10], RW_L, default = 2'b0 
       Controls IO Loopback. Ctl[1] enables ACIO loopback test. Ctl[0] not used.
     */
    UINT32 odtmode : 1;
    /* odtmode - Bits[12:12], RW_L, default = 1'b0 
       Forces the driver in ODT mode, where both the pullup and pulldown are enabled. 
       Used for Rstray and AcIoLb testings 
     */
    UINT32 lvmode : 2;
    /* lvmode - Bits[14:13], RW_L, default = None 
       not used
     */
    UINT32 lvmodevalid : 1;
    /* lvmodevalid - Bits[15:15], RW_L, default = None 
       not used
     */
    UINT32 is_rdimm : 1;
    /* is_rdimm - Bits[16:16], RW_L, default = None 
       Drives xxCLK to DC during P6 and S3. Should be set for RDIMMs and LRDIMMs in 
       DDR3 AND DDR4. 
     */
    UINT32 statlegen : 2;
    /* statlegen - Bits[18:17], RW_L, default = 2'b11 
       static leg enable
     */
    UINT32 rsvd_19 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[20:20], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 rxvref : 7;
    /* rxvref - Bits[27:21], RW_L, default = 1'b0 
       IOLB Vref Control. unsigned encoding from 1/3 Vdd to 2/3 Vdd with step size of 
       Vdd/192(??) 
     */
    UINT32 vccddqhi : 1;
    /* vccddqhi - Bits[28:28], RW_L, default = 1'b0 
       not used
     */
    UINT32 dllweaklock : 1;
    /* dllweaklock - Bits[29:29], RW_L, default = 1'b0 
       not used
     */
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[30:30], RW_L, default = 1'b0 
       level shift hold enable:
                            Holds the HV control values and power down the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training during MRC training
     */
  } Bits;
  UINT32 Data;
} DDRCRCLKCONTROLS_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CLK Controls
 */
typedef union {
  struct {
    UINT32 ddrpxcenable : 1;
    /* ddrpxcenable - Bits[0:0], RW_L, default = 1'b0 
       NOT USED - When Phased based xtalk cancellation is enabled, set this to delay 
       cmd to maintain constant Tcwl. 
     */
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider to enable xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider to enable xover 2to2 and 2to1 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       enable the extra flop stage in xover to help timing
     */
    UINT32 dllmask : 2;
    /* dllmask - Bits[5:4], RW_L, default = 1'b0 
       not used
     */
    UINT32 dllrsvd1 : 1;
    /* dllrsvd1 - Bits[6:6], RW_L, default = 1'b0 
       Reserved
     */
    UINT32 txon : 1;
    /* txon - Bits[7:7], RW_L, default = 1'b0 
       Forces the CLK transmitter on, overriding any power downs 
     */
    UINT32 intclkon : 1;
    /* intclkon - Bits[8:8], RW_L, default = 1'b0 
       Forces on all PI enables and the LCB enables in the AFE fubs.
     */
    UINT32 repclkon : 1;
    /* repclkon - Bits[9:9], RW_L, default = 1'b0 
       not used. 
     */
    UINT32 iolbctl : 2;
    /* iolbctl - Bits[11:10], RW_L, default = 2'b0 
       Controls IO Loopback. Ctl[1] enables ACIO loopback test. Ctl[0] not used.
     */
    UINT32 odtmode : 1;
    /* odtmode - Bits[12:12], RW_L, default = 1'b0 
       Forces the driver in ODT mode, where both the pullup and pulldown are enabled. 
       Used for Rstray and AcIoLb testings 
     */
    UINT32 lvmode : 2;
    /* lvmode - Bits[14:13], RW_L, default = None 
       not used
     */
    UINT32 lvmodevalid : 1;
    /* lvmodevalid - Bits[15:15], RW_L, default = None 
       not used
     */
    UINT32 is_rdimm : 1;
    /* is_rdimm - Bits[16:16], RW_L, default = None 
       Drives xxCLK to DC during P6 and S3. Should be set for RDIMMs and LRDIMMs in 
       DDR3 AND DDR4. 
     */
    UINT32 statlegen : 2;
    /* statlegen - Bits[18:17], RW_L, default = 2'b11 
       static leg enable
     */
    UINT32 rsvd_19 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[20:20], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 rxvref : 7;
    /* rxvref - Bits[27:21], RW_L, default = 1'b0 
       IOLB Vref Control. unsigned encoding from 1/3 Vdd to 2/3 Vdd with step size of 
       Vdd/192(??) 
     */
    UINT32 vccddqhi : 1;
    /* vccddqhi - Bits[28:28], RW_L, default = 1'b0 
       not used
     */
    UINT32 dllweaklock : 1;
    /* dllweaklock - Bits[29:29], RW_L, default = 1'b0 
       not used
     */
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[30:30], RW_L, default = 1'b0 
       level shift hold enable:
                            Holds the HV control values and power down the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training during MRC training
     */
  } Bits;
  UINT32 Data;
} DDRCRCLKCONTROLS_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */







/* DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG supported on:                           */
/*       HSX (0x401A5A1C)                                                     */
/*       BDX (0x401A5A1C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG 0x0D034A1C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Clk Training Register
 */
typedef union {
  struct {
    UINT32 trainingoffset : 6;
    /* trainingoffset - Bits[5:0], RW_L, default = 1'b0 
       Offset added to PiCode. Obtained via xover calibration such that ClkPi aligns 
       with grid clock when PiCode=0. 
     */
    UINT32 rsvd_6 : 2;
    UINT32 pclkxoverphasedet : 4;
    /* pclkxoverphasedet - Bits[11:8], RO_V, default = 1'b0 
       P Clock Buffer Xover Phase Detect. used for xover calibration.
     */
    UINT32 nclkxoverphasedet : 4;
    /* nclkxoverphasedet - Bits[15:12], RO_V, default = 1'b0 
       N Clock Buffer Xover Phase Detect.  used for xover calibration.
     */
    UINT32 rsvd_16 : 14;
    UINT32 vmselogicdelay : 1;
    /* vmselogicdelay - Bits[30:30], RW, default = 1'b0 
       VMSE Logic Delay, add extra qclk cycle delay to CLK output
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DDRCRCLKTRAINING_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* DDRCRCMDCOMP_CMDS_MCIO_DDRIOEXT_REG supported on:                          */
/*       HSX (0x401A5C00)                                                     */
/*       BDX (0x401A5C00)                                                     */
/* Register default value on HSX:       0x00000040                            */
/* Register default value on BDX:       0x00000020                            */
#define DDRCRCMDCOMP_CMDS_MCIO_DDRIOEXT_REG 0x0D034C00

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CMD Comp Values
 */
typedef union {
  struct {
    UINT32 scomp : 6;
    /* scomp - Bits[5:0], RW_L, default = 1'b0 
       Scomp Value. Normally updated by Comp FSM via CompBus. can be overriden via 
       message channel if periodical retrain is disabled. 
     */
    UINT32 tcocomp : 7;
    /* tcocomp - Bits[12:6], RW_L, default = 1'b0 
       Tco Comp Value. Normally updated by Comp FSM via CompBus. can be overriden via 
       message channel if periodical retrain is disabled. 
     */
    UINT32 rcompdrvup : 6;
    /* rcompdrvup - Bits[18:13], RW_L, default = 1'b0 
       Rcomp Drv Up Value. Normally updated by Comp FSM via CompBus. can be overriden 
       via message channel if periodical retrain is disabled. 
     */
    UINT32 rcompdrvdown : 6;
    /* rcompdrvdown - Bits[24:19], RW_L, default = 1'b0 
       Rcomp Drv Dn Value. Normally updated by Comp FSM via CompBus. can be overriden 
       via message channel if periodical retrain is disabled. 
     */
    UINT32 lscomp : 3;
    /* lscomp - Bits[27:25], RW_L, default = 3'b010 
       not used controlled by Dll fuse
     */
    UINT32 spare : 4;
    /* spare - Bits[31:28], RW_L, default = 1'b0 
       Spare
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCOMP_CMDS_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* DDRCRCMDPICODING_CMDS_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A5C08)                                                     */
/*       BDX (0x401A5C08)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDPICODING_CMDS_MCIO_DDRIOEXT_REG 0x0D034C08

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CMD Pi Settings
 */
typedef union {
  struct {
    UINT32 cmdpicode0 : 6;
    /* cmdpicode0 - Bits[5:0], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay0 : 2;
    /* cmdpilogicdelay0 - Bits[7:6], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_8 : 1;
    UINT32 cmdpicode1 : 6;
    /* cmdpicode1 - Bits[14:9], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay1 : 2;
    /* cmdpilogicdelay1 - Bits[16:15], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_17 : 1;
    UINT32 cmdpicode2 : 6;
    /* cmdpicode2 - Bits[23:18], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay2 : 2;
    /* cmdpilogicdelay2 - Bits[25:24], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_26 : 6;
  } Bits;
  UINT32 Data;
} DDRCRCMDPICODING_CMDS_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A5C0C)                                                     */
/*       BDX (0x401A5C0C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG 0x0D034C0C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * CMD Controls
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider in xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider in xover 2to2 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       adding extra pipeline stage after xover to avoid timing failure at higher ClkPi 
       rate 
     */
    UINT32 rsvd_4 : 2;
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[6:6], RW_L, default = 1'b0 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 txon : 1;
    /* txon - Bits[7:7], RW_L, default = 1'b0 
       Forces the CMD transmitter on, overriding any power downs
     */
    UINT32 intclkon : 1;
    /* intclkon - Bits[8:8], RW_L, default = 1'b0 
       Forces on all PI enables and the LCB enables in the AFE fubs.  Does not force on 
       D0 PI enable or override weaklock mode. 
     */
    UINT32 repclkon : 1;
    /* repclkon - Bits[9:9], RW_L, default = 1'b0 
       not used.
     */
    UINT32 iolbctl : 2;
    /* iolbctl - Bits[11:10], RW_L, default = 1'b0 
       Controls IO Loopback.  Ctl[1] enables IOLB mode and Ctl[0] controls if this is a 
       Setup(0) or Hold(1) test 
     */
    UINT32 odtmode : 1;
    /* odtmode - Bits[12:12], RW_L, default = 1'b0 
       Forces the driver in ODT mode, where both the pullup and pulldown are enabled.  
       Used for Rstray testing 
     */
    UINT32 cmdtxeq : 2;
    /* cmdtxeq - Bits[14:13], RW_L, default = 1'b0 
       not used.
     */
    UINT32 earlyweakdrive : 2;
    /* earlyweakdrive - Bits[16:15], RW_L, default = 1'b0 
       Enables ODT pulse before/after command transmit. 
       	             Bit 0 (15) is used to enable ODT pulse before transmit. 
       	             Bit 1 (16) is used to enable ODT pulse after transmit. 
       	             NOTE: BROKEN - must be set to 2'b00.
     */
    UINT32 lvmode : 2;
    /* lvmode - Bits[18:17], RW_L, default = None 
       not used.
     */
    UINT32 lvmodevalid : 1;
    /* lvmodevalid - Bits[19:19], RW_L, default = None 
       not used.
     */
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[20:20], RW_L, default = None 
       level shift hold enable
                            Holds the HV control values and power down mode the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rsvd_21 : 8;
    UINT32 paritydisable : 1;
    /* paritydisable - Bits[29:29], RW_L, default = 1'b0 
        disable parity and par_err pins for certain dimm types.
     */
    UINT32 ddr4modeenable : 1;
    /* ddr4modeenable - Bits[30:30], RW_L, default = 1'b0 
       Ddr4 Mode Enable, set to 1 in CMD2 (Func:5.Addr:C00 and Func:7.Addr:C00) fub to 
       bypass extra delay on parity bit 
     */
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CMD Controls
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider in xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider in xover 2to2 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       adding extra pipeline stage after xover to avoid timing failure at higher ClkPi 
       rate 
     */
    UINT32 rsvd_4 : 2;
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[6:6], RW_L, default = 1'b0 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 txon : 1;
    /* txon - Bits[7:7], RW_L, default = 1'b0 
       Forces the CMD transmitter on, overriding any power downs
     */
    UINT32 intclkon : 1;
    /* intclkon - Bits[8:8], RW_L, default = 1'b0 
       Forces on all PI enables and the LCB enables in the AFE fubs.  Does not force on 
       D0 PI enable or override weaklock mode. 
     */
    UINT32 repclkon : 1;
    /* repclkon - Bits[9:9], RW_L, default = 1'b0 
       not used.
     */
    UINT32 iolbctl : 2;
    /* iolbctl - Bits[11:10], RW_L, default = 1'b0 
       Controls IO Loopback.  Ctl[1] enables IOLB mode and Ctl[0] controls if this is a 
       Setup(0) or Hold(1) test 
     */
    UINT32 odtmode : 1;
    /* odtmode - Bits[12:12], RW_L, default = 1'b0 
       Forces the driver in ODT mode, where both the pullup and pulldown are enabled.  
       Used for Rstray testing 
     */
    UINT32 cmdtxeq : 2;
    /* cmdtxeq - Bits[14:13], RW_L, default = 1'b0 
       not used.
     */
    UINT32 earlyweakdrive : 2;
    /* earlyweakdrive - Bits[16:15], RW_L, default = 1'b0 
       Enables ODT pulse before/after command transmit. 
       	             Bit 0 (15) is used to enable ODT pulse before transmit. 
       	             Bit 1 (16) is used to enable ODT pulse after transmit. 
       	             NOTE: BROKEN - must be set to 2'b00.
     */
    UINT32 lvmode : 2;
    /* lvmode - Bits[18:17], RW_L, default = None 
       not used.
     */
    UINT32 lvmodevalid : 1;
    /* lvmodevalid - Bits[19:19], RW_L, default = None 
       not used.
     */
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[20:20], RW_L, default = None 
       level shift hold enable
                            Holds the HV control values and power down mode the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rsvd_21 : 7;
    UINT32 sr_gate_2to2 : 1;
    /* sr_gate_2to2 - Bits[28:28], RW_L, default = 1'b0 
       Used to ungate trainreset into crossover for 2:2 mode
     */
    UINT32 paritydisable : 1;
    /* paritydisable - Bits[29:29], RW_L, default = 1'b0 
        disable parity and par_err pins for certain dimm types. This must be set to 1 
       by BIOS for A0 to avoid Vref being enabled in non-ACIO loopback mode. 
     */
    UINT32 ddr4modeenable : 1;
    /* ddr4modeenable - Bits[30:30], RW_L, default = 1'b0 
       Ddr4 Mode Enable, set to 1 in CMD2 (Func:5.Addr:C00 and Func:7.Addr:C00) fub to 
       bypass extra delay on parity bit 
     */
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDCONTROLS2_CMDS_MCIO_DDRIOEXT_REG supported on:                     */
/*       HSX (0x401A5C10)                                                     */
/*       BDX (0x401A5C10)                                                     */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00001000                            */
#define DDRCRCMDCONTROLS2_CMDS_MCIO_DDRIOEXT_REG 0x0D034C10
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * CMD Controls
 */
typedef union {
  struct {
    UINT32 ddrcrinvertpiclk : 12;
    /* ddrcrinvertpiclk - Bits[11:0], RW_L, default = 12'b0 
       Invert PiClk (used for 2:1, 2:2 when we need to invert the piclk.
     */
    UINT32 spare : 20;
    /* spare - Bits[31:12], RW_L, default = 20'b0 
       reserved.
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS2_CMDS_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CMD Controls
 */
typedef union {
  struct {
    UINT32 ddrcrinvertpiclk : 12;
    /* ddrcrinvertpiclk - Bits[11:0], RW_L, default = 12'b0 
       Invert PiClk (used for 2:1, 2:2 when we need to invert the piclk.
     */
    UINT32 bdx_sys : 1;
    /* bdx_sys - Bits[12:12], RW_L, default = 1'b1 
       BDX change : This bit helps to bypass the delaycell in transmitter
     */
    UINT32 rsvd_13 : 1;
    UINT32 spare : 18;
    /* spare - Bits[31:14], RW_L, default = 18'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS2_CMDS_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDPICODING2_CMDS_MCIO_DDRIOEXT_REG supported on:                     */
/*       HSX (0x401A5C18)                                                     */
/*       BDX (0x401A5C18)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDPICODING2_CMDS_MCIO_DDRIOEXT_REG 0x0D034C18

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CMD Pi Settings
 */
typedef union {
  struct {
    UINT32 cmdpicode3 : 6;
    /* cmdpicode3 - Bits[5:0], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay3 : 2;
    /* cmdpilogicdelay3 - Bits[7:6], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_8 : 1;
    UINT32 cmdpicode4 : 6;
    /* cmdpicode4 - Bits[14:9], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay4 : 2;
    /* cmdpilogicdelay4 - Bits[16:15], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_17 : 1;
    UINT32 cmdpicode5 : 6;
    /* cmdpicode5 - Bits[23:18], RW_L, default = 6'b0 
       Cmd Pi Code
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 cmdpilogicdelay5 : 2;
    /* cmdpilogicdelay5 - Bits[25:24], RW_L, default = 2'b00 
       Cmd Pi Logic Delay - add 1 qclk cycle delay.
     */
    UINT32 rsvd_26 : 6;
  } Bits;
  UINT32 Data;
} DDRCRCMDPICODING2_CMDS_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






/* DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A5C2C)                                                     */
/*       BDX (0x401A5C2C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG 0x0D034C2C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Cmd Training Register
 */
typedef union {
  struct {
    UINT32 trainingoffset : 6;
    /* trainingoffset - Bits[5:0], RW_L, default = 6'b0 
       Offset added to PiCode. Obtained via xover calibration such that ClkPi aligns 
       with grid clock when PiCode=0. 
     */
    UINT32 rsvd_6 : 2;
    UINT32 xoverphasedet : 14;
    /* xoverphasedet - Bits[21:8], RO_V, default = 14'b0 
       Xover Phase Detect. Xover calibration feedback bits. Actually, using one of 14 
       is sufficient. 
                   [13]   CtlOutEnPath Xover Phase Detect
                   [12]   CmdOutEnPath Xover Phase Detect
                   [11:0] CmdBuf Xover Phase Detects
               
     */
    UINT32 trainingoffset2 : 6;
    /* trainingoffset2 - Bits[27:22], RW_L, default = 6'b0 
       Offset added to PiCode. Obtained via xover calibration such that ClkPi aligns 
       with grid clock when PiCode=0. 
     */
    UINT32 rsvd_28 : 4;
  } Bits;
  UINT32 Data;
} DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXT_REG supported on:                     */
/*       HSX (0x401A5C30)                                                     */
/*       BDX (0x401A5C30)                                                     */
/* Register default value on HSX:       0x00980000                            */
/* Register default value on BDX:       0x00180000                            */
#define DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXT_REG 0x0D034C30
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * CMD Controls 1
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    UINT32 internalvrefen : 1;
    /* internalvrefen - Bits[2:2], RW_L, default = 1'b0 
       Enable internal Vref for parity error pin, ACIOLB, or DcDetect (only for CTL)
     */
    UINT32 ddrcrimodeen : 1;
    /* ddrcrimodeen - Bits[3:3], RW_L, default = 1'b0 
       Imode en
     */
    UINT32 ddrcrimodesel : 1;
    /* ddrcrimodesel - Bits[4:4], RW_L, default = 1'b0 
       Imode sel, 0: swing boost; 1: xtalk cancellation.
     */
    UINT32 ddrcrimodesrcntl : 6;
    /* ddrcrimodesrcntl - Bits[10:5], RW_L, default = 6'b0 
       Imode Scomp configuration
     */
    UINT32 ddrcrimodesrcntlovrd : 1;
    /* ddrcrimodesrcntlovrd - Bits[11:11], RW_L, default = 1'b0 
       Imode scomp override enable
     */
    UINT32 ddrcrimodecoef : 4;
    /* ddrcrimodecoef - Bits[15:12], RW_L, default = 1'b0 
       Imode coef, swing boost configure
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_20 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[21:21], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 dqstatdflt : 2;
    /* dqstatdflt - Bits[23:22], RW_L, default = 2'b11 
       Static leg enable
     */
    UINT32 odtstatdflt : 2;
    /* odtstatdflt - Bits[25:24], RW_L, default = None 
       Static leg enable
     */
    UINT32 rsvd_26 : 1;
    UINT32 txeq : 5;
    /* txeq - Bits[31:27], RW_L, default = 5'b0 
       When TxEQOverride = 0, bits [29:27] are used as selects for the RCOMP -> EQCODE 
       calculation: 
       
       	             TxEQ[29:27] = 000 : EQCode = 0
       	             TxEQ[29:27] = 001 : EQCode = -1/4 nominal value  
       	             TxEQ[29:27] = 010 : EQCode = -1/8 nominal value  
       	             TxEQ[29:27] = 101 : EQCode = +1/4 nominal value  
       	             TxEQ[29:27] = 110 : EQCode = +1/8 nominal value  
       
                     Bits [31:30] are used to select between a 5, 3, or 1 
       multiplication factor for use in VMSE/DDR4: 
       
       	             TxEQ[31:30] = 00 : 5 * (comp + 16) / 16
       	             TxEQ[31:30] = 11 : 3 * (comp + 16) / 16
       	             TxEQ[31:30] = 01 : 1 * (comp + 16) / 16
       	             TxEQ[31:30] = 10 : 1 * (comp + 16) / 16
       
       
                            When TxEQOverride = 1, bits [31:27] are used directly as 
       the EQ Code: 
       
                     Write Equalization codes.  5 bit control, but only 24 of them are 
       meaningful.  
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CMD Controls 1
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    UINT32 internalvrefen : 1;
    /* internalvrefen - Bits[2:2], RW_L, default = 1'b0 
       Enable internal Vref for parity error pin, ACIOLB, or DcDetect (only for CTL)
     */
    UINT32 ddrcrimodeen : 1;
    /* ddrcrimodeen - Bits[3:3], RW_L, default = 1'b0 
       Imode en
     */
    UINT32 ddrcrimodesel : 1;
    /* ddrcrimodesel - Bits[4:4], RW_L, default = 1'b0 
       Imode sel, 0: swing boost; 1: xtalk cancellation.
     */
    UINT32 ddrcrimodesrcntl : 6;
    /* ddrcrimodesrcntl - Bits[10:5], RW_L, default = 6'b0 
       Imode Scomp configuration
     */
    UINT32 ddrcrimodesrcntlovrd : 1;
    /* ddrcrimodesrcntlovrd - Bits[11:11], RW_L, default = 1'b0 
       Imode scomp override enable
     */
    UINT32 ddrcrimodecoef : 4;
    /* ddrcrimodecoef - Bits[15:12], RW_L, default = 1'b0 
       Imode coef, swing boost configure
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_20 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[21:21], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 dqstatdflt : 2;
    /* dqstatdflt - Bits[23:22], RW_L, default = 2'b11 
       Static leg enable
     */
    UINT32 odtstatdflt : 2;
    /* odtstatdflt - Bits[25:24], RW_L, default = None 
       Static leg enable
     */
    UINT32 txeq : 6;
    /* txeq - Bits[31:26], RW_L, default = 6'b0 
        Modified in BDX!! When TxEQOverride = 0, bits [29:27] are used as selects for 
       the RCOMP -> EQCODE calculation: 
       
       	             TxEQ[29:27] = 000 : EQCode = 0
       	             TxEQ[29:27] = 001 : EQCode = -1/4 nominal value  
       	             TxEQ[29:27] = 010 : EQCode = -1/8 nominal value  
       	             TxEQ[29:27] = 101 : EQCode = +1/4 nominal value  
       	             TxEQ[29:27] = 110 : EQCode = +1/8 nominal value  
       
                     Bits [31:30] are used to select between a 5, 3, or 1 
       multiplication factor for use in VMSE/DDR4: 
       
       	             TxEQ[31:30] = 00 : 5 * (comp + 16) / 16
       	             TxEQ[31:30] = 11 : 3 * (comp + 16) / 16
       	             TxEQ[31:30] = 01 : 1 * (comp + 16) / 16
       	             TxEQ[31:30] = 10 : 1 * (comp + 16) / 16
       
       
                            When TxEQOverride = 1, bits [31:27] are used directly as 
       the EQ Code: 
       
                     Write Equalization codes.  5 bit control, but only 24 of them are 
       meaningful.  
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCTLCOMP_CTL_MCIO_DDRIOEXT_REG supported on:                           */
/*       HSX (0x401A5E10)                                                     */
/*       BDX (0x401A5E10)                                                     */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00000020                            */
#define DDRCRCTLCOMP_CTL_MCIO_DDRIOEXT_REG 0x0D034E10

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CTL Comp Values
 */
typedef union {
  struct {
    UINT32 scomp : 5;
    /* scomp - Bits[4:0], RW_L, default = 1'b0 
       Scomp Value. Normally updated by Comp FSM via CompBus. can be overriden via 
       message channel if periodical retrain is disabled. 
     */
    UINT32 tcocomp : 7;
    /* tcocomp - Bits[11:5], RW_L, default = 1'b0 
       Tco Comp Value. Normally updated by Comp FSM via CompBus. can be overriden via 
       message channel if periodical retrain is disabled. 
     */
    UINT32 rcompdrvup : 6;
    /* rcompdrvup - Bits[17:12], RW_L, default = 1'b0 
       Rcomp Drv Up Value. Normally updated by Comp FSM via CompBus. can be overriden 
       via message channel if periodical retrain is disabled. 
     */
    UINT32 rcompdrvdown : 6;
    /* rcompdrvdown - Bits[23:18], RW_L, default = 1'b0 
       Rcomp Drv Dn Value. Normally updated by Comp FSM via CompBus. can be overriden 
       via message channel if periodical retrain is disabled. 
     */
    UINT32 lscomp : 3;
    /* lscomp - Bits[26:24], RW_L, default = 3'b010 
       used in Rx Amplifier. Need for Loopback operation only.
     */
    UINT32 spare : 4;
    /* spare - Bits[30:27], RW_L, default = 4'b0 
       Spare
     */
    UINT32 vmseallowcompupdatetxinject : 1;
    /* vmseallowcompupdatetxinject - Bits[31:31], RW_L, default = 1'b0 
       Chicken bit to allow injecting stable data into TX during VMSE CompUpdate. 
                     '1' allows injection of unscrambled NOOPS into TX, and should only 
       be needed when VCMD scrambling is enabled. 
                     '0' disables data injection, which is safe when VCMD scrambling is 
       not enabled 
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLCOMP_CTL_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* DDRCRCTLPICODING_CTL_MCIO_DDRIOEXT_REG supported on:                       */
/*       HSX (0x401A5E18)                                                     */
/*       BDX (0x401A5E18)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING_CTL_MCIO_DDRIOEXT_REG 0x0D034E18

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CTL Pi Settings
 */
typedef union {
  struct {
    UINT32 ctlpicode0 : 6;
    /* ctlpicode0 - Bits[5:0], RW_L, default = 6'b0 
       Ctl Pi Code0
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay0 : 2;
    /* ctlpilogicdelay0 - Bits[7:6], RW_L, default = 2'b0 
       Ctl Pi Logic Delay0 - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode1 : 6;
    /* ctlpicode1 - Bits[13:8], RW_L, default = 6'b0 
       Ctl Pi Code1
     */
    UINT32 ctlpilogicdelay1 : 2;
    /* ctlpilogicdelay1 - Bits[15:14], RW_L, default = 2'b0 
       Ctl Pi Logic Delay1 - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode2 : 6;
    /* ctlpicode2 - Bits[21:16], RW_L, default = 6'b0 
       Ctl Pi Code2
     */
    UINT32 ctlpilogicdelay2 : 2;
    /* ctlpilogicdelay2 - Bits[23:22], RW_L, default = 2'b0 
       Ctl Pi Logic Delay2 - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode3 : 6;
    /* ctlpicode3 - Bits[29:24], RW_L, default = 6'b0 
       Ctl Pi Code3
     */
    UINT32 ctlpilogicdelay3 : 2;
    /* ctlpilogicdelay3 - Bits[31:30], RW_L, default = 2'b0 
       Ctl Pi Logic Delay3 - add 1~3 qclk cycles delay.
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLPICODING_CTL_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_REG supported on:                       */
/*       HSX (0x401A5E1C)                                                     */
/*       BDX (0x401A5E1C)                                                     */
/* Register default value:              0x10010000                            */
#define DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_REG 0x0D034E1C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * CTL Controls
 */
typedef union {
  struct {
    UINT32 ddrphasextalkenableqnnnh : 1;
    /* ddrphasextalkenableqnnnh - Bits[0:0], RW_L, default = 1'b0 
       When Phased based xtalk cancellation is enabled, set this to delay cmd to 
       maintain constant Tcwl. 
     */
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider to enable xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider to enable xover 2to2 and 2to1 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       enable the extra flop stage in xover to help timing
     */
    UINT32 lvmode : 2;
    /* lvmode - Bits[5:4], RW_L, default = 1'b0 
       not used.
     */
    UINT32 lvmodevalid : 1;
    /* lvmodevalid - Bits[6:6], RW_L, default = 1'b0 
       not used.
     */
    UINT32 txon : 1;
    /* txon - Bits[7:7], RW_L, default = 1'b0 
       Forces the CMD/CKE or CS/ODT transmitters on based on which one is programmed, 
       overriding any power downs 
     */
    UINT32 intclkon : 1;
    /* intclkon - Bits[8:8], RW_L, default = 1'b0 
       Forces on all PI enables and the LCB enables in the AFE fubs.  Does not force on 
       D0 PI enable or override weaklock mode. 
     */
    UINT32 repclkon : 1;
    /* repclkon - Bits[9:9], RW_L, default = 1'b0 
       not used.
     */
    UINT32 iolbctl : 2;
    /* iolbctl - Bits[11:10], RW_L, default = 2'b0 
       Controls IO Loopback.  Ctl[1] enables IOLB mode and Ctl[0] controls if this is a 
       Setup(0) or Hold(1) test 
     */
    UINT32 odtmode : 1;
    /* odtmode - Bits[12:12], RW_L, default = 1'b0 
       Forces the driver in ODT mode. Used for Rstray and AcIoLb testings
     */
    UINT32 cmdtxeq : 2;
    /* cmdtxeq - Bits[14:13], RW_L, default = 2'b0 
       not used.
     */
    UINT32 earlyweakdrive : 2;
    /* earlyweakdrive - Bits[16:15], RW_L, default = 2'b0 
       Unused in CTL
     */
    UINT32 ctltxeq : 2;
    /* ctltxeq - Bits[18:17], RW_L, default = 2'b1 
       When LSB is set to 1, uses TxEQ register (DdrCrCmdControls1[31:27]) as the EQ 
       code rather than the  
       	             EQ code calculated from the CTL DRVUP comp code.
     */
    UINT32 ctlsrdrv : 2;
    /* ctlsrdrv - Bits[20:19], RW_L, default = 2'b0 
       Not used in A0. Controls the number of CTL weak drive enabled during self 
       refresh: {0: All Enabled, 1: Enable Seg[4], 2: Enable Seg[4:3], 3: Enable 
       Seg[4:2]} 
     */
    UINT32 rxvref : 7;
    /* rxvref - Bits[27:21], RW_L, default = 7'b0 
       IOLB Vref Control. binary code from 1/3 Vdd to 2/3 Vdd with step size of 
       Vdd/192(??mV) 
     */
    UINT32 rsvd_28 : 1;
    UINT32 paritydisable : 1;
    /* paritydisable - Bits[29:29], RW_L, default = 1'b1 
       disable parity and par_err pins to save power when the populated dimm type does 
       not support parity 
     */
    UINT32 ddr3nren : 1;
    /* ddr3nren - Bits[30:30], RW_L, default = None 
       not used.
     */
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CTL Controls
 */
typedef union {
  struct {
    UINT32 ddrpxcenable : 1;
    /* ddrpxcenable - Bits[0:0], RW_L, default = 1'b0 
       When Phased based xtalk cancellation is enabled, set this to delay cmd to 
       maintain constant Tcwl. 
     */
    UINT32 gqclkdivenable : 1;
    /* gqclkdivenable - Bits[1:1], RW_L, default = None 
       enable grid clock divider to enable xover 2to2 mode
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW_L, default = None 
       enable PiRef clock divider to enable xover 2to2 and 2to1 mode
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW_L, default = None 
       enable the extra flop stage in xover to help timing
     */
    UINT32 rsvd_4 : 13;
    UINT32 ctltxeq : 2;
    /* ctltxeq - Bits[18:17], RW_L, default = 2'b1 
       When LSB is set to 1, uses TxEQ register (DdrCrCmdControls1[31:27]) as the EQ 
       code rather than the  
       	             EQ code calculated from the CTL DRVUP comp code.
     */
    UINT32 rsvd_19 : 2;
    UINT32 rxvref : 7;
    /* rxvref - Bits[27:21], RW_L, default = 7'b0 
       IOLB Vref Control. binary code from 1/3 Vdd to 2/3 Vdd with step size of 
       Vdd/192(??mV) 
     */
    UINT32 sr_gate_2to2 : 1;
    /* sr_gate_2to2 - Bits[28:28], RW_L, default = 1'b0 
       Used to ungate trainreset into xover for 2:2 mode
     */
    UINT32 paritydisable : 1;
    /* paritydisable - Bits[29:29], RW_L, default = 1'b1 
       disable parity and par_err pins to save power when the populated dimm type does 
       not support parity 
     */
    UINT32 rsvd_30 : 1;
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW_L, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A5E20)                                                     */
/*       BDX (0x401A5E20)                                                     */
/* Register default value on HSX:       0x00004FFF                            */
/* Register default value on BDX:       0xC0004FFF                            */
#define DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_REG 0x0D034E20
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * CTL Ranks Used
 */
typedef union {
  struct {
    UINT32 ranken : 12;
    /* ranken - Bits[11:0], RW_L, default = 12'b111111111111 
       Enables output buffers for ctlbuf 0 to ctlbuf 11, PI clocks and output datapaths 
       for this rank 
     */
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[12:12], RW_L, default = 1'b0 
       level shift hold enable:
                            Holds the HV control values and power down the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rxbiasgcncomp : 2;
    /* rxbiasgcncomp - Bits[14:13], RW_L, default = 2'b10 
       used to compensate the GCN skew. The GCN skew information should be extracted by 
       reading and analyzing the final Rcomp code value. The target spec is to 
       categorize the GCN skew in 3 process condition (mainly slow, fast, typical). 
       (one example can be just reading the 2 MLS bit of Rcomp code and determine if 
       the GCN is slow, fast or typical). The gcncomp is used for CTLE. 
       00: gcn in typical corner
       11: gcn in slow corner
       10: gcn in typical corner
       01: gcn in fast corner
               
     */
    UINT32 rsvd_15 : 1;
    UINT32 imodebiashighcm : 1;
    /* imodebiashighcm - Bits[16:16], RW_L, default = 1'b0 
       Imode bias control
     */
    UINT32 rxbiassel : 1;
    /* rxbiassel - Bits[17:17], RW_L, default = 1'b0 
       Rx bias Sel
     */
    UINT32 ddrcrinvertpiclk : 12;
    /* ddrcrinvertpiclk - Bits[29:18], RW_L, default = 12'b0 
       Invert PiClk in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 phase
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[30:30], RW_L, default = None 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CTL Ranks Used
 */
typedef union {
  struct {
    UINT32 ranken : 12;
    /* ranken - Bits[11:0], RW_L, default = 12'b111111111111 
       Enables output buffers for ctlbuf 0 to ctlbuf 11, PI clocks and output datapaths 
       for this rank 
     */
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[12:12], RW_L, default = 1'b0 
       level shift hold enable:
                            Holds the HV control values and power down the level 
       shifters. 
                            Apply "1" after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 rxbiasgcncomp : 2;
    /* rxbiasgcncomp - Bits[14:13], RW_L, default = 2'b10 
       used to compensate the GCN skew. The GCN skew information should be
                 extracted by reading and analyzing the final Rcomp code value. The 
       target spec 
                 is to categorize the GCN skew in 3 process condition (mainly slow, 
       fast, typical). 
                 (one example can be just reading the 2 MLS bit of Rcomp code and 
       determine if the 
                 GCN is slow, fast or typical). The gcncomp is used for CTLE.
                 11: gcn in slow corner
                 10: gcn in typical corner
                 01: gcn in fast corner
               
     */
    UINT32 rsvd_15 : 1;
    UINT32 imodebiashighcm : 1;
    /* imodebiashighcm - Bits[16:16], RW_L, default = 1'b0 
       Imode bias control
     */
    UINT32 rxbiassel : 1;
    /* rxbiassel - Bits[17:17], RW_L, default = 1'b0 
       Rx bias Sel
     */
    UINT32 ddrcrinvertpiclk : 12;
    /* ddrcrinvertpiclk - Bits[29:18], RW_L, default = 12'b0 
       Invert PiClk in 2:2 and 2:1 mode to extend the ClkPiRefDiv2 phase
     */
    UINT32 drvpupdis : 1;
    /* drvpupdis - Bits[30:30], RW_L, default = 1'b1 
       This will be connected to all tx, and must toggle 1->0 after a power state
     */
    UINT32 rsvd_31 : 1;
  } Bits;
  UINT32 Data;
} DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */







/* DDRCRCTLTRAINING_CTL_MCIO_DDRIOEXT_REG supported on:                       */
/*       HSX (0x401A5E2C)                                                     */
/*       BDX (0x401A5E2C)                                                     */
/* Register default value:              0x00007000                            */
#define DDRCRCTLTRAINING_CTL_MCIO_DDRIOEXT_REG 0x0D034E2C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * Ctl Training Register
 */
typedef union {
  struct {
    UINT32 trainingoffset : 6;
    /* trainingoffset - Bits[5:0], RW_L, default = 6'b0 
       Offset added to PiCode. Obtained via xover calibration such that ClkPi aligns 
       with grid clock when PiCode=0. 
     */
    UINT32 dcdetectmode : 1;
    /* dcdetectmode - Bits[6:6], RW_L, default = None 
       Enables VMSE init DcDetect mode
     */
    UINT32 rsvd_7 : 1;
    UINT32 xoverphasedet : 12;
    /* xoverphasedet - Bits[19:8], RO_V, default = 1'b0 
       Xover Phase Detect. Xover calibration feedback bits.
     */
    UINT32 ctldrvsegen : 3;
    /* ctldrvsegen - Bits[22:20], RW_L, default = 3'b000 
       drvsegdisable, or drvSegEn_b. 
     */
    UINT32 ckepddrvsegen : 3;
    /* ckepddrvsegen - Bits[25:23], RW_L, default = 3'b111 
        Controls the number of drive segments enabled during CKE powerdown 
     */
    UINT32 rsvd_26 : 6;
  } Bits;
  UINT32 Data;
} DDRCRCTLTRAINING_CTL_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * Ctl Training Register
 */
typedef union {
  struct {
    UINT32 trainingoffset : 6;
    /* trainingoffset - Bits[5:0], RW_L, default = 6'b0 
       Offset added to PiCode. Obtained via xover calibration such that ClkPi aligns 
       with grid clock when PiCode=0. 
     */
    UINT32 dcdetectmode : 1;
    /* dcdetectmode - Bits[6:6], RW_L, default = None 
       Enables VMSE init DcDetect mode
     */
    UINT32 rsvd_7 : 1;
    UINT32 xoverphasedet : 12;
    /* xoverphasedet - Bits[19:8], RO_V, default = 1'b0 
       Xover Phase Detect. Xover calibration feedback bits.
     */
    UINT32 ctldrvsegen : 3;
    /* ctldrvsegen - Bits[22:20], RW_L, default = 3'b000 
       drvsegdisable, or drvSegEn_b. 
     */
    UINT32 ckepddrvsegen : 3;
    /* ckepddrvsegen - Bits[25:23], RW_L, default = 3'b111 
        Controls the number of drive segments enabled during CKE powerdown 
     */
    UINT32 rsvd_26 : 6;
  } Bits;
  UINT32 Data;
} DDRCRCTLTRAINING_CTL_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A5E30)                                                     */
/*       BDX (0x401A5E30)                                                     */
/* Register default value on HSX:       0x00980000                            */
/* Register default value on BDX:       0x00180000                            */
#define DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXT_REG 0x0D034E30
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.20.5.CFG.xml.
 * CMD Controls 1
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    UINT32 internalvrefen : 1;
    /* internalvrefen - Bits[2:2], RW_L, default = 1'b0 
       Enable internal Vref for parity error pin, ACIOLB, or DcDetect (only for CTL)
     */
    UINT32 ddrcrimodeen : 1;
    /* ddrcrimodeen - Bits[3:3], RW_L, default = 1'b0 
       Imode en
     */
    UINT32 ddrcrimodesel : 1;
    /* ddrcrimodesel - Bits[4:4], RW_L, default = 1'b0 
       Imode sel, 0: swing boost; 1: xtalk cancellation.
     */
    UINT32 ddrcrimodesrcntl : 6;
    /* ddrcrimodesrcntl - Bits[10:5], RW_L, default = 6'b0 
       Imode Scomp configuration
     */
    UINT32 ddrcrimodesrcntlovrd : 1;
    /* ddrcrimodesrcntlovrd - Bits[11:11], RW_L, default = 1'b0 
       Imode scomp override enable
     */
    UINT32 ddrcrimodecoef : 4;
    /* ddrcrimodecoef - Bits[15:12], RW_L, default = 1'b0 
       Imode coef, swing boost configure
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_20 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[21:21], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 dqstatdflt : 2;
    /* dqstatdflt - Bits[23:22], RW_L, default = 2'b11 
       Static leg enable
     */
    UINT32 odtstatdflt : 2;
    /* odtstatdflt - Bits[25:24], RW_L, default = None 
       Static leg enable
     */
    UINT32 rsvd_26 : 1;
    UINT32 txeq : 5;
    /* txeq - Bits[31:27], RW_L, default = 5'b0 
       When TxEQOverride = 0, bits [29:27] are used as selects for the RCOMP -> EQCODE 
       calculation: 
       
       	             TxEQ[29:27] = 000 : EQCode = 0
       	             TxEQ[29:27] = 001 : EQCode = -1/4 nominal value  
       	             TxEQ[29:27] = 010 : EQCode = -1/8 nominal value  
       	             TxEQ[29:27] = 101 : EQCode = +1/4 nominal value  
       	             TxEQ[29:27] = 110 : EQCode = +1/8 nominal value  
       
                     Bits [31:30] are used to select between a 5, 3, or 1 
       multiplication factor for use in VMSE/DDR4: 
       
       	             TxEQ[31:30] = 00 : 5 * (comp + 16) / 16
       	             TxEQ[31:30] = 11 : 3 * (comp + 16) / 16
       	             TxEQ[31:30] = 01 : 1 * (comp + 16) / 16
       	             TxEQ[31:30] = 10 : 1 * (comp + 16) / 16
       
       
                            When TxEQOverride = 1, bits [31:27] are used directly as 
       the EQ Code: 
       
                     Write Equalization codes.  5 bit control, but only 24 of them are 
       meaningful.  
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXT_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CMD Controls 1
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 2;
    UINT32 internalvrefen : 1;
    /* internalvrefen - Bits[2:2], RW_L, default = 1'b0 
       Enable internal Vref for parity error pin, ACIOLB, or DcDetect (only for CTL)
     */
    UINT32 ddrcrimodeen : 1;
    /* ddrcrimodeen - Bits[3:3], RW_L, default = 1'b0 
       Imode en
     */
    UINT32 ddrcrimodesel : 1;
    /* ddrcrimodesel - Bits[4:4], RW_L, default = 1'b0 
       Imode sel, 0: swing boost; 1: xtalk cancellation.
     */
    UINT32 ddrcrimodesrcntl : 6;
    /* ddrcrimodesrcntl - Bits[10:5], RW_L, default = 6'b0 
       Imode Scomp configuration
     */
    UINT32 ddrcrimodesrcntlovrd : 1;
    /* ddrcrimodesrcntlovrd - Bits[11:11], RW_L, default = 1'b0 
       Imode scomp override enable
     */
    UINT32 ddrcrimodecoef : 4;
    /* ddrcrimodecoef - Bits[15:12], RW_L, default = 1'b0 
       Imode coef, swing boost configure
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[19:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_20 : 1;
    UINT32 cttermination : 1;
    /* cttermination - Bits[21:21], RW_L, default = None 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 dqstatdflt : 2;
    /* dqstatdflt - Bits[23:22], RW_L, default = 2'b11 
       Static leg enable
     */
    UINT32 odtstatdflt : 2;
    /* odtstatdflt - Bits[25:24], RW_L, default = None 
       Static leg enable
     */
    UINT32 txeq : 6;
    /* txeq - Bits[31:26], RW_L, default = 6'b0 
        Modified in BDX!! When TxEQOverride = 0, bits [29:27] are used as selects for 
       the RCOMP -> EQCODE calculation: 
       
       	             TxEQ[29:27] = 000 : EQCode = 0
       	             TxEQ[29:27] = 001 : EQCode = -1/4 nominal value  
       	             TxEQ[29:27] = 010 : EQCode = -1/8 nominal value  
       	             TxEQ[29:27] = 101 : EQCode = +1/4 nominal value  
       	             TxEQ[29:27] = 110 : EQCode = +1/8 nominal value  
       
                     Bits [31:30] are used to select between a 5, 3, or 1 
       multiplication factor for use in VMSE/DDR4: 
       
       	             TxEQ[31:30] = 00 : 5 * (comp + 16) / 16
       	             TxEQ[31:30] = 11 : 3 * (comp + 16) / 16
       	             TxEQ[31:30] = 01 : 1 * (comp + 16) / 16
       	             TxEQ[31:30] = 10 : 1 * (comp + 16) / 16
       
       
                            When TxEQOverride = 1, bits [31:27] are used directly as 
       the EQ Code: 
       
                     Write Equalization codes.  5 bit control, but only 24 of them are 
       meaningful.  
                   The decoding is 1-1 matched, means 0 is 0 legs enabled for 
       deemphasized, 1 is 1 leg enabled  
                   for deemphasized, and so on.
               
     */
  } Bits;
  UINT32 Data;
} DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXT_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCTLPICODING2_CTL_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A5E34)                                                     */
/*       BDX (0x401A5E34)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING2_CTL_MCIO_DDRIOEXT_REG 0x0D034E34

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CTL Pi Settings
 */
typedef union {
  struct {
    UINT32 ctlpicode4 : 6;
    /* ctlpicode4 - Bits[5:0], RW_L, default = 6'b0 
       Ctl Pi Code4
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay4 : 2;
    /* ctlpilogicdelay4 - Bits[7:6], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode5 : 6;
    /* ctlpicode5 - Bits[13:8], RW_L, default = 6'b0 
       Ctl Pi Code5
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay5 : 2;
    /* ctlpilogicdelay5 - Bits[15:14], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode6 : 6;
    /* ctlpicode6 - Bits[21:16], RW_L, default = 6'b0 
       Ctl Pi Code6
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay6 : 2;
    /* ctlpilogicdelay6 - Bits[23:22], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode7 : 6;
    /* ctlpicode7 - Bits[29:24], RW_L, default = 6'b0 
       Ctl Pi Code7
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay7 : 2;
    /* ctlpilogicdelay7 - Bits[31:30], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
  } Bits;
  UINT32 Data;
} DDRCRCTLPICODING2_CTL_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCTLPICODING3_CTL_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A5E38)                                                     */
/*       BDX (0x401A5E38)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING3_CTL_MCIO_DDRIOEXT_REG 0x0D034E38

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CTL Pi Settings
 */
typedef union {
  struct {
    UINT32 ctlpicode8 : 6;
    /* ctlpicode8 - Bits[5:0], RW_L, default = 6'b0 
       Ctl Pi Code8
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay8 : 2;
    /* ctlpilogicdelay8 - Bits[7:6], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode9 : 6;
    /* ctlpicode9 - Bits[13:8], RW_L, default = 6'b0 
       Ctl Pi Code9
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay9 : 2;
    /* ctlpilogicdelay9 - Bits[15:14], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 ctlpicode10 : 6;
    /* ctlpicode10 - Bits[21:16], RW_L, default = 6'b0 
       Ctl Pi Code10
                            [5:0] - Pi setting with [5] being the PiMSB sel
     */
    UINT32 ctlpilogicdelay10 : 2;
    /* ctlpilogicdelay10 - Bits[23:22], RW_L, default = 2'b0 
       Ctl Pi Logic Delay - add 1~3 qclk cycles delay.
     */
    UINT32 rsvd_24 : 8;
  } Bits;
  UINT32 Data;
} DDRCRCTLPICODING3_CTL_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* DDRCRCTLPICODING4_CTL_MCIO_DDRIOEXT_REG supported on:                      */
/*       HSX (0x401A5E40)                                                     */
/*       BDX (0x401A5E40)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING4_CTL_MCIO_DDRIOEXT_REG 0x0D034E40

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.20.5.CFG.xml.
 * CTL Pi Settings 4 
 */
typedef union {
  struct {
    UINT32 vmselogicdelay : 12;
    /* vmselogicdelay - Bits[11:0], RW_L, default = None 
       VmseLogicDelay - add 2x clock cycle delay per vcmd bit
     */
    UINT32 rsvd_12 : 2;
    UINT32 vmsephase1cmdlogicdelay : 12;
    /* vmsephase1cmdlogicdelay - Bits[25:14], RW_L, default = 12'h0 
       When this is set, add one Qclk delay to the Intel SMI 2 
                   phase1 VCMD in case of a polarity inversion in serializer.
                
     */
    UINT32 flipvmseserializerphase : 1;
    /* flipvmseserializerphase - Bits[26:26], RW_L, default = 1'b0 
       When this is set, flip the phase of the serializer for VMSE 1:1 mode.
                
     */
    UINT32 rsvd_27 : 5;
  } Bits;
  UINT32 Data;
} DDRCRCTLPICODING4_CTL_MCIO_DDRIOEXT_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SVID_MCIO_DDRIOEXT_REG supported on:                                       */
/*       IVT_EP (0x2018D02C)                                                  */
/*       IVT_EX (0x201FD02C)                                                  */
/* Register default value:              0x8086                                */
#define SVID_MCIO_DDRIOEXT_REG 0x0D03202C



/* SDID_MCIO_DDRIOEXT_REG supported on:                                       */
/*       IVT_EP (0x2018D02E)                                                  */
/*       IVT_EX (0x201FD02E)                                                  */
/* Register default value:              0x0000                                */
#define SDID_MCIO_DDRIOEXT_REG 0x0D03202E



/* PXPCAP_MCIO_DDRIOEXT_REG supported on:                                     */
/*       IVT_EP (0x4018D040)                                                  */
/*       IVT_EX (0x401FD040)                                                  */
/* Register default value:              0x00910010                            */
#define PXPCAP_MCIO_DDRIOEXT_REG 0x0D034040



/* GDCRRXOFFSETSKEWRANK4567A0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D104)                                                  */
/*       IVT_EX (0x201FD104)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567A0_MCIO_DDRIOEXT_REG 0x0D032104



/* GDCRRXOFFSETCOMPN0AA0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D108)                                                  */
/*       IVT_EX (0x201FD108)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AA0_MCIO_DDRIOEXT_REG 0x0D032108



/* GDCRRXOFFSETCOMPN0BA0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D10C)                                                  */
/*       IVT_EX (0x201FD10C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BA0_MCIO_DDRIOEXT_REG 0x0D03210C



/* GDCRRXOFFSETCOMPN1AA0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D110)                                                  */
/*       IVT_EX (0x201FD110)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AA0_MCIO_DDRIOEXT_REG 0x0D032110



/* GDCRRXOFFSETCOMPN1BA0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D114)                                                  */
/*       IVT_EX (0x201FD114)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BA0_MCIO_DDRIOEXT_REG 0x0D032114



/* GDCRRXOFFSETCOMPCFGA0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D118)                                                  */
/*       IVT_EX (0x201FD118)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGA0_MCIO_DDRIOEXT_REG 0x0D032118



/* GDCRRXOFFSETCOMPSTATUSA0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D11C)                                                  */
/*       IVT_EX (0x201FD11C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSA0_MCIO_DDRIOEXT_REG 0x0D03211C



/* GDCRTXDESKEWGROUP0LANE01A0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D120)                                                  */
/*       IVT_EX (0x201FD120)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01A0_MCIO_DDRIOEXT_REG 0x0D032120



/* GDCRTXDESKEWGROUP0LANE45A0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D124)                                                  */
/*       IVT_EX (0x201FD124)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45A0_MCIO_DDRIOEXT_REG 0x0D032124



/* GDCRRXDESKEWGROUP0LANE01A0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D128)                                                  */
/*       IVT_EX (0x201FD128)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01A0_MCIO_DDRIOEXT_REG 0x0D032128



/* GDCRRXDESKEWGROUP0LANE45A0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D12C)                                                  */
/*       IVT_EX (0x201FD12C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45A0_MCIO_DDRIOEXT_REG 0x0D03212C



/* GDCRTXDESKEWGROUP0LANE23A0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D130)                                                  */
/*       IVT_EX (0x201FD130)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23A0_MCIO_DDRIOEXT_REG 0x0D032130



/* GDCRTXDESKEWGROUP0LANE67A0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D134)                                                  */
/*       IVT_EX (0x201FD134)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67A0_MCIO_DDRIOEXT_REG 0x0D032134



/* GDCRRXDESKEWGROUP0LANE23A0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D138)                                                  */
/*       IVT_EX (0x201FD138)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23A0_MCIO_DDRIOEXT_REG 0x0D032138



/* GDCRRXDESKEWGROUP0LANE67A0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D13C)                                                  */
/*       IVT_EX (0x201FD13C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67A0_MCIO_DDRIOEXT_REG 0x0D03213C



/* GDCRTXDESKEWGROUP1NIB0A0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D140)                                                  */
/*       IVT_EX (0x201FD140)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0A0_MCIO_DDRIOEXT_REG 0x0D032140



/* GDCRTXDESKEWGROUP1NIB1A0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D144)                                                  */
/*       IVT_EX (0x201FD144)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1A0_MCIO_DDRIOEXT_REG 0x0D032144



/* GDCRRXDESKEWGROUP1NIB0A0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D148)                                                  */
/*       IVT_EX (0x201FD148)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0A0_MCIO_DDRIOEXT_REG 0x0D032148



/* GDCRRXDESKEWGROUP1NIB1A0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D14C)                                                  */
/*       IVT_EX (0x201FD14C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1A0_MCIO_DDRIOEXT_REG 0x0D03214C



/* GDCRTXDESKEWGROUP2NIB0A0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D150)                                                  */
/*       IVT_EX (0x201FD150)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0A0_MCIO_DDRIOEXT_REG 0x0D032150



/* GDCRTXDESKEWGROUP2NIB1A0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D154)                                                  */
/*       IVT_EX (0x201FD154)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1A0_MCIO_DDRIOEXT_REG 0x0D032154



/* GDCRRXDESKEWGROUP2NIB0A0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D158)                                                  */
/*       IVT_EX (0x201FD158)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0A0_MCIO_DDRIOEXT_REG 0x0D032158



/* GDCRRXDESKEWGROUP2NIB1A0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D15C)                                                  */
/*       IVT_EX (0x201FD15C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1A0_MCIO_DDRIOEXT_REG 0x0D03215C



/* GDCRTXDESKEWGROUP3NIB0A0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D160)                                                  */
/*       IVT_EX (0x201FD160)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0A0_MCIO_DDRIOEXT_REG 0x0D032160



/* GDCRTXDESKEWGROUP3NIB1A0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D164)                                                  */
/*       IVT_EX (0x201FD164)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1A0_MCIO_DDRIOEXT_REG 0x0D032164



/* GDCRRXDESKEWGROUP3NIB0A0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D168)                                                  */
/*       IVT_EX (0x201FD168)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0A0_MCIO_DDRIOEXT_REG 0x0D032168



/* GDCRRXDESKEWGROUP3NIB1A0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D16C)                                                  */
/*       IVT_EX (0x201FD16C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1A0_MCIO_DDRIOEXT_REG 0x0D03216C







/* GDCRXTALKEQATTKRA0_MCIO_DDRIOEXT_REG supported on:                         */
/*       IVT_EP (0x2018D178)                                                  */
/*       IVT_EX (0x201FD178)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRA0_MCIO_DDRIOEXT_REG 0x0D032178



/* GDCRDLLCFGA0_MCIO_DDRIOEXT_REG supported on:                               */
/*       IVT_EP (0x2018D17C)                                                  */
/*       IVT_EX (0x201FD17C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGA0_MCIO_DDRIOEXT_REG 0x0D03217C



/* GDCRRXOFFSETSKEWRANK4567B0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D184)                                                  */
/*       IVT_EX (0x201FD184)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567B0_MCIO_DDRIOEXT_REG 0x0D032184



/* GDCRRXOFFSETCOMPN0AB0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D188)                                                  */
/*       IVT_EX (0x201FD188)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AB0_MCIO_DDRIOEXT_REG 0x0D032188



/* GDCRRXOFFSETCOMPN0BB0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D18C)                                                  */
/*       IVT_EX (0x201FD18C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BB0_MCIO_DDRIOEXT_REG 0x0D03218C



/* GDCRRXOFFSETCOMPN1AB0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D190)                                                  */
/*       IVT_EX (0x201FD190)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AB0_MCIO_DDRIOEXT_REG 0x0D032190



/* GDCRRXOFFSETCOMPN1BB0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D194)                                                  */
/*       IVT_EX (0x201FD194)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BB0_MCIO_DDRIOEXT_REG 0x0D032194



/* GDCRRXOFFSETCOMPCFGB0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D198)                                                  */
/*       IVT_EX (0x201FD198)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGB0_MCIO_DDRIOEXT_REG 0x0D032198



/* GDCRRXOFFSETCOMPSTATUSB0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D19C)                                                  */
/*       IVT_EX (0x201FD19C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSB0_MCIO_DDRIOEXT_REG 0x0D03219C



/* GDCRTXDESKEWGROUP0LANE01B0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D1A0)                                                  */
/*       IVT_EX (0x201FD1A0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01B0_MCIO_DDRIOEXT_REG 0x0D0321A0



/* GDCRTXDESKEWGROUP0LANE45B0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D1A4)                                                  */
/*       IVT_EX (0x201FD1A4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45B0_MCIO_DDRIOEXT_REG 0x0D0321A4



/* GDCRRXDESKEWGROUP0LANE01B0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D1A8)                                                  */
/*       IVT_EX (0x201FD1A8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01B0_MCIO_DDRIOEXT_REG 0x0D0321A8



/* GDCRRXDESKEWGROUP0LANE45B0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D1AC)                                                  */
/*       IVT_EX (0x201FD1AC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45B0_MCIO_DDRIOEXT_REG 0x0D0321AC



/* GDCRTXDESKEWGROUP0LANE23B0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D1B0)                                                  */
/*       IVT_EX (0x201FD1B0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23B0_MCIO_DDRIOEXT_REG 0x0D0321B0



/* GDCRTXDESKEWGROUP0LANE67B0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D1B4)                                                  */
/*       IVT_EX (0x201FD1B4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67B0_MCIO_DDRIOEXT_REG 0x0D0321B4



/* GDCRRXDESKEWGROUP0LANE23B0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D1B8)                                                  */
/*       IVT_EX (0x201FD1B8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23B0_MCIO_DDRIOEXT_REG 0x0D0321B8



/* GDCRRXDESKEWGROUP0LANE67B0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D1BC)                                                  */
/*       IVT_EX (0x201FD1BC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67B0_MCIO_DDRIOEXT_REG 0x0D0321BC



/* GDCRTXDESKEWGROUP1NIB0B0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D1C0)                                                  */
/*       IVT_EX (0x201FD1C0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0B0_MCIO_DDRIOEXT_REG 0x0D0321C0



/* GDCRTXDESKEWGROUP1NIB1B0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D1C4)                                                  */
/*       IVT_EX (0x201FD1C4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1B0_MCIO_DDRIOEXT_REG 0x0D0321C4



/* GDCRRXDESKEWGROUP1NIB0B0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D1C8)                                                  */
/*       IVT_EX (0x201FD1C8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0B0_MCIO_DDRIOEXT_REG 0x0D0321C8



/* GDCRRXDESKEWGROUP1NIB1B0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D1CC)                                                  */
/*       IVT_EX (0x201FD1CC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1B0_MCIO_DDRIOEXT_REG 0x0D0321CC



/* GDCRTXDESKEWGROUP2NIB0B0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D1D0)                                                  */
/*       IVT_EX (0x201FD1D0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0B0_MCIO_DDRIOEXT_REG 0x0D0321D0



/* GDCRTXDESKEWGROUP2NIB1B0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D1D4)                                                  */
/*       IVT_EX (0x201FD1D4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1B0_MCIO_DDRIOEXT_REG 0x0D0321D4



/* GDCRRXDESKEWGROUP2NIB0B0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D1D8)                                                  */
/*       IVT_EX (0x201FD1D8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0B0_MCIO_DDRIOEXT_REG 0x0D0321D8



/* GDCRRXDESKEWGROUP2NIB1B0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D1DC)                                                  */
/*       IVT_EX (0x201FD1DC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1B0_MCIO_DDRIOEXT_REG 0x0D0321DC



/* GDCRTXDESKEWGROUP3NIB0B0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D1E0)                                                  */
/*       IVT_EX (0x201FD1E0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0B0_MCIO_DDRIOEXT_REG 0x0D0321E0



/* GDCRTXDESKEWGROUP3NIB1B0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D1E4)                                                  */
/*       IVT_EX (0x201FD1E4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1B0_MCIO_DDRIOEXT_REG 0x0D0321E4



/* GDCRRXDESKEWGROUP3NIB0B0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D1E8)                                                  */
/*       IVT_EX (0x201FD1E8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0B0_MCIO_DDRIOEXT_REG 0x0D0321E8



/* GDCRRXDESKEWGROUP3NIB1B0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D1EC)                                                  */
/*       IVT_EX (0x201FD1EC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1B0_MCIO_DDRIOEXT_REG 0x0D0321EC







/* GDCRXTALKEQATTKRB0_MCIO_DDRIOEXT_REG supported on:                         */
/*       IVT_EP (0x2018D1F8)                                                  */
/*       IVT_EX (0x201FD1F8)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRB0_MCIO_DDRIOEXT_REG 0x0D0321F8



/* GDCRDLLCFGB0_MCIO_DDRIOEXT_REG supported on:                               */
/*       IVT_EP (0x2018D1FC)                                                  */
/*       IVT_EX (0x201FD1FC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGB0_MCIO_DDRIOEXT_REG 0x0D0321FC



/* GDCRRXOFFSETSKEWRANK4567C0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D204)                                                  */
/*       IVT_EX (0x201FD204)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567C0_MCIO_DDRIOEXT_REG 0x0D032204



/* GDCRRXOFFSETCOMPN0AC0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D208)                                                  */
/*       IVT_EX (0x201FD208)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AC0_MCIO_DDRIOEXT_REG 0x0D032208



/* GDCRRXOFFSETCOMPN0BC0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D20C)                                                  */
/*       IVT_EX (0x201FD20C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BC0_MCIO_DDRIOEXT_REG 0x0D03220C



/* GDCRRXOFFSETCOMPN1AC0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D210)                                                  */
/*       IVT_EX (0x201FD210)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AC0_MCIO_DDRIOEXT_REG 0x0D032210



/* GDCRRXOFFSETCOMPN1BC0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D214)                                                  */
/*       IVT_EX (0x201FD214)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BC0_MCIO_DDRIOEXT_REG 0x0D032214



/* GDCRRXOFFSETCOMPCFGC0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D218)                                                  */
/*       IVT_EX (0x201FD218)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGC0_MCIO_DDRIOEXT_REG 0x0D032218



/* GDCRRXOFFSETCOMPSTATUSC0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D21C)                                                  */
/*       IVT_EX (0x201FD21C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSC0_MCIO_DDRIOEXT_REG 0x0D03221C



/* GDCRTXDESKEWGROUP0LANE01C0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D220)                                                  */
/*       IVT_EX (0x201FD220)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01C0_MCIO_DDRIOEXT_REG 0x0D032220



/* GDCRTXDESKEWGROUP0LANE45C0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D224)                                                  */
/*       IVT_EX (0x201FD224)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45C0_MCIO_DDRIOEXT_REG 0x0D032224



/* GDCRRXDESKEWGROUP0LANE01C0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D228)                                                  */
/*       IVT_EX (0x201FD228)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01C0_MCIO_DDRIOEXT_REG 0x0D032228



/* GDCRRXDESKEWGROUP0LANE45C0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D22C)                                                  */
/*       IVT_EX (0x201FD22C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45C0_MCIO_DDRIOEXT_REG 0x0D03222C



/* GDCRTXDESKEWGROUP0LANE23C0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D230)                                                  */
/*       IVT_EX (0x201FD230)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23C0_MCIO_DDRIOEXT_REG 0x0D032230



/* GDCRTXDESKEWGROUP0LANE67C0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D234)                                                  */
/*       IVT_EX (0x201FD234)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67C0_MCIO_DDRIOEXT_REG 0x0D032234



/* GDCRRXDESKEWGROUP0LANE23C0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D238)                                                  */
/*       IVT_EX (0x201FD238)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23C0_MCIO_DDRIOEXT_REG 0x0D032238



/* GDCRRXDESKEWGROUP0LANE67C0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D23C)                                                  */
/*       IVT_EX (0x201FD23C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67C0_MCIO_DDRIOEXT_REG 0x0D03223C



/* GDCRTXDESKEWGROUP1NIB0C0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D240)                                                  */
/*       IVT_EX (0x201FD240)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0C0_MCIO_DDRIOEXT_REG 0x0D032240



/* GDCRTXDESKEWGROUP1NIB1C0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D244)                                                  */
/*       IVT_EX (0x201FD244)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1C0_MCIO_DDRIOEXT_REG 0x0D032244



/* GDCRRXDESKEWGROUP1NIB0C0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D248)                                                  */
/*       IVT_EX (0x201FD248)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0C0_MCIO_DDRIOEXT_REG 0x0D032248



/* GDCRRXDESKEWGROUP1NIB1C0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D24C)                                                  */
/*       IVT_EX (0x201FD24C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1C0_MCIO_DDRIOEXT_REG 0x0D03224C



/* GDCRTXDESKEWGROUP2NIB0C0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D250)                                                  */
/*       IVT_EX (0x201FD250)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0C0_MCIO_DDRIOEXT_REG 0x0D032250



/* GDCRTXDESKEWGROUP2NIB1C0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D254)                                                  */
/*       IVT_EX (0x201FD254)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1C0_MCIO_DDRIOEXT_REG 0x0D032254



/* GDCRRXDESKEWGROUP2NIB0C0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D258)                                                  */
/*       IVT_EX (0x201FD258)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0C0_MCIO_DDRIOEXT_REG 0x0D032258



/* GDCRRXDESKEWGROUP2NIB1C0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D25C)                                                  */
/*       IVT_EX (0x201FD25C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1C0_MCIO_DDRIOEXT_REG 0x0D03225C



/* GDCRTXDESKEWGROUP3NIB0C0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D260)                                                  */
/*       IVT_EX (0x201FD260)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0C0_MCIO_DDRIOEXT_REG 0x0D032260



/* GDCRTXDESKEWGROUP3NIB1C0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D264)                                                  */
/*       IVT_EX (0x201FD264)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1C0_MCIO_DDRIOEXT_REG 0x0D032264



/* GDCRRXDESKEWGROUP3NIB0C0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D268)                                                  */
/*       IVT_EX (0x201FD268)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0C0_MCIO_DDRIOEXT_REG 0x0D032268



/* GDCRRXDESKEWGROUP3NIB1C0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D26C)                                                  */
/*       IVT_EX (0x201FD26C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1C0_MCIO_DDRIOEXT_REG 0x0D03226C







/* GDCRXTALKEQATTKRC0_MCIO_DDRIOEXT_REG supported on:                         */
/*       IVT_EP (0x2018D278)                                                  */
/*       IVT_EX (0x201FD278)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRC0_MCIO_DDRIOEXT_REG 0x0D032278



/* GDCRDLLCFGC0_MCIO_DDRIOEXT_REG supported on:                               */
/*       IVT_EP (0x2018D27C)                                                  */
/*       IVT_EX (0x201FD27C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGC0_MCIO_DDRIOEXT_REG 0x0D03227C



/* GDCRRXOFFSETSKEWRANK4567D0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D284)                                                  */
/*       IVT_EX (0x201FD284)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567D0_MCIO_DDRIOEXT_REG 0x0D032284



/* GDCRRXOFFSETCOMPN0AD0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D288)                                                  */
/*       IVT_EX (0x201FD288)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AD0_MCIO_DDRIOEXT_REG 0x0D032288



/* GDCRRXOFFSETCOMPN0BD0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D28C)                                                  */
/*       IVT_EX (0x201FD28C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BD0_MCIO_DDRIOEXT_REG 0x0D03228C



/* GDCRRXOFFSETCOMPN1AD0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D290)                                                  */
/*       IVT_EX (0x201FD290)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AD0_MCIO_DDRIOEXT_REG 0x0D032290



/* GDCRRXOFFSETCOMPN1BD0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D294)                                                  */
/*       IVT_EX (0x201FD294)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BD0_MCIO_DDRIOEXT_REG 0x0D032294



/* GDCRRXOFFSETCOMPCFGD0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D298)                                                  */
/*       IVT_EX (0x201FD298)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGD0_MCIO_DDRIOEXT_REG 0x0D032298



/* GDCRRXOFFSETCOMPSTATUSD0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D29C)                                                  */
/*       IVT_EX (0x201FD29C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSD0_MCIO_DDRIOEXT_REG 0x0D03229C



/* GDCRTXDESKEWGROUP0LANE01D0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D2A0)                                                  */
/*       IVT_EX (0x201FD2A0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01D0_MCIO_DDRIOEXT_REG 0x0D0322A0



/* GDCRTXDESKEWGROUP0LANE45D0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D2A4)                                                  */
/*       IVT_EX (0x201FD2A4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45D0_MCIO_DDRIOEXT_REG 0x0D0322A4



/* GDCRRXDESKEWGROUP0LANE01D0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D2A8)                                                  */
/*       IVT_EX (0x201FD2A8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01D0_MCIO_DDRIOEXT_REG 0x0D0322A8



/* GDCRRXDESKEWGROUP0LANE45D0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D2AC)                                                  */
/*       IVT_EX (0x201FD2AC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45D0_MCIO_DDRIOEXT_REG 0x0D0322AC



/* GDCRTXDESKEWGROUP0LANE23D0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D2B0)                                                  */
/*       IVT_EX (0x201FD2B0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23D0_MCIO_DDRIOEXT_REG 0x0D0322B0



/* GDCRTXDESKEWGROUP0LANE67D0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D2B4)                                                  */
/*       IVT_EX (0x201FD2B4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67D0_MCIO_DDRIOEXT_REG 0x0D0322B4



/* GDCRRXDESKEWGROUP0LANE23D0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D2B8)                                                  */
/*       IVT_EX (0x201FD2B8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23D0_MCIO_DDRIOEXT_REG 0x0D0322B8



/* GDCRRXDESKEWGROUP0LANE67D0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D2BC)                                                  */
/*       IVT_EX (0x201FD2BC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67D0_MCIO_DDRIOEXT_REG 0x0D0322BC



/* GDCRTXDESKEWGROUP1NIB0D0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D2C0)                                                  */
/*       IVT_EX (0x201FD2C0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0D0_MCIO_DDRIOEXT_REG 0x0D0322C0



/* GDCRTXDESKEWGROUP1NIB1D0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D2C4)                                                  */
/*       IVT_EX (0x201FD2C4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1D0_MCIO_DDRIOEXT_REG 0x0D0322C4



/* GDCRRXDESKEWGROUP1NIB0D0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D2C8)                                                  */
/*       IVT_EX (0x201FD2C8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0D0_MCIO_DDRIOEXT_REG 0x0D0322C8



/* GDCRRXDESKEWGROUP1NIB1D0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D2CC)                                                  */
/*       IVT_EX (0x201FD2CC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1D0_MCIO_DDRIOEXT_REG 0x0D0322CC



/* GDCRTXDESKEWGROUP2NIB0D0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D2D0)                                                  */
/*       IVT_EX (0x201FD2D0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0D0_MCIO_DDRIOEXT_REG 0x0D0322D0



/* GDCRTXDESKEWGROUP2NIB1D0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D2D4)                                                  */
/*       IVT_EX (0x201FD2D4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1D0_MCIO_DDRIOEXT_REG 0x0D0322D4



/* GDCRRXDESKEWGROUP2NIB0D0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D2D8)                                                  */
/*       IVT_EX (0x201FD2D8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0D0_MCIO_DDRIOEXT_REG 0x0D0322D8



/* GDCRRXDESKEWGROUP2NIB1D0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D2DC)                                                  */
/*       IVT_EX (0x201FD2DC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1D0_MCIO_DDRIOEXT_REG 0x0D0322DC



/* GDCRTXDESKEWGROUP3NIB0D0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D2E0)                                                  */
/*       IVT_EX (0x201FD2E0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0D0_MCIO_DDRIOEXT_REG 0x0D0322E0



/* GDCRTXDESKEWGROUP3NIB1D0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D2E4)                                                  */
/*       IVT_EX (0x201FD2E4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1D0_MCIO_DDRIOEXT_REG 0x0D0322E4



/* GDCRRXDESKEWGROUP3NIB0D0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D2E8)                                                  */
/*       IVT_EX (0x201FD2E8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0D0_MCIO_DDRIOEXT_REG 0x0D0322E8



/* GDCRRXDESKEWGROUP3NIB1D0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D2EC)                                                  */
/*       IVT_EX (0x201FD2EC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1D0_MCIO_DDRIOEXT_REG 0x0D0322EC







/* GDCRXTALKEQATTKRD0_MCIO_DDRIOEXT_REG supported on:                         */
/*       IVT_EP (0x2018D2F8)                                                  */
/*       IVT_EX (0x201FD2F8)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRD0_MCIO_DDRIOEXT_REG 0x0D0322F8



/* GDCRDLLCFGD0_MCIO_DDRIOEXT_REG supported on:                               */
/*       IVT_EP (0x2018D2FC)                                                  */
/*       IVT_EX (0x201FD2FC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGD0_MCIO_DDRIOEXT_REG 0x0D0322FC



/* GDCRRXOFFSETSKEWRANK4567E0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D504)                                                  */
/*       IVT_EX (0x201FD504)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567E0_MCIO_DDRIOEXT_REG 0x0D032504



/* GDCRRXOFFSETCOMPN0AE0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D508)                                                  */
/*       IVT_EX (0x201FD508)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AE0_MCIO_DDRIOEXT_REG 0x0D032508



/* GDCRRXOFFSETCOMPN0BE0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D50C)                                                  */
/*       IVT_EX (0x201FD50C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BE0_MCIO_DDRIOEXT_REG 0x0D03250C



/* GDCRRXOFFSETCOMPN1AE0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D510)                                                  */
/*       IVT_EX (0x201FD510)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AE0_MCIO_DDRIOEXT_REG 0x0D032510



/* GDCRRXOFFSETCOMPN1BE0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D514)                                                  */
/*       IVT_EX (0x201FD514)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BE0_MCIO_DDRIOEXT_REG 0x0D032514



/* GDCRRXOFFSETCOMPCFGE0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D518)                                                  */
/*       IVT_EX (0x201FD518)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGE0_MCIO_DDRIOEXT_REG 0x0D032518



/* GDCRRXOFFSETCOMPSTATUSE0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D51C)                                                  */
/*       IVT_EX (0x201FD51C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSE0_MCIO_DDRIOEXT_REG 0x0D03251C



/* GDCRTXDESKEWGROUP0LANE01E0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D520)                                                  */
/*       IVT_EX (0x201FD520)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01E0_MCIO_DDRIOEXT_REG 0x0D032520



/* GDCRTXDESKEWGROUP0LANE45E0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D524)                                                  */
/*       IVT_EX (0x201FD524)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45E0_MCIO_DDRIOEXT_REG 0x0D032524



/* GDCRRXDESKEWGROUP0LANE01E0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D528)                                                  */
/*       IVT_EX (0x201FD528)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01E0_MCIO_DDRIOEXT_REG 0x0D032528



/* GDCRRXDESKEWGROUP0LANE45E0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D52C)                                                  */
/*       IVT_EX (0x201FD52C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45E0_MCIO_DDRIOEXT_REG 0x0D03252C



/* GDCRTXDESKEWGROUP0LANE23E0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D530)                                                  */
/*       IVT_EX (0x201FD530)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23E0_MCIO_DDRIOEXT_REG 0x0D032530



/* GDCRTXDESKEWGROUP0LANE67E0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D534)                                                  */
/*       IVT_EX (0x201FD534)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67E0_MCIO_DDRIOEXT_REG 0x0D032534



/* GDCRRXDESKEWGROUP0LANE23E0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D538)                                                  */
/*       IVT_EX (0x201FD538)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23E0_MCIO_DDRIOEXT_REG 0x0D032538



/* GDCRRXDESKEWGROUP0LANE67E0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D53C)                                                  */
/*       IVT_EX (0x201FD53C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67E0_MCIO_DDRIOEXT_REG 0x0D03253C



/* GDCRTXDESKEWGROUP1NIB0E0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D540)                                                  */
/*       IVT_EX (0x201FD540)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0E0_MCIO_DDRIOEXT_REG 0x0D032540



/* GDCRTXDESKEWGROUP1NIB1E0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D544)                                                  */
/*       IVT_EX (0x201FD544)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1E0_MCIO_DDRIOEXT_REG 0x0D032544



/* GDCRRXDESKEWGROUP1NIB0E0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D548)                                                  */
/*       IVT_EX (0x201FD548)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0E0_MCIO_DDRIOEXT_REG 0x0D032548



/* GDCRRXDESKEWGROUP1NIB1E0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D54C)                                                  */
/*       IVT_EX (0x201FD54C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1E0_MCIO_DDRIOEXT_REG 0x0D03254C



/* GDCRTXDESKEWGROUP2NIB0E0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D550)                                                  */
/*       IVT_EX (0x201FD550)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0E0_MCIO_DDRIOEXT_REG 0x0D032550



/* GDCRTXDESKEWGROUP2NIB1E0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D554)                                                  */
/*       IVT_EX (0x201FD554)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1E0_MCIO_DDRIOEXT_REG 0x0D032554



/* GDCRRXDESKEWGROUP2NIB0E0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D558)                                                  */
/*       IVT_EX (0x201FD558)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0E0_MCIO_DDRIOEXT_REG 0x0D032558



/* GDCRRXDESKEWGROUP2NIB1E0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D55C)                                                  */
/*       IVT_EX (0x201FD55C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1E0_MCIO_DDRIOEXT_REG 0x0D03255C



/* GDCRTXDESKEWGROUP3NIB0E0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D560)                                                  */
/*       IVT_EX (0x201FD560)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0E0_MCIO_DDRIOEXT_REG 0x0D032560



/* GDCRTXDESKEWGROUP3NIB1E0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D564)                                                  */
/*       IVT_EX (0x201FD564)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1E0_MCIO_DDRIOEXT_REG 0x0D032564



/* GDCRRXDESKEWGROUP3NIB0E0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D568)                                                  */
/*       IVT_EX (0x201FD568)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0E0_MCIO_DDRIOEXT_REG 0x0D032568



/* GDCRRXDESKEWGROUP3NIB1E0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D56C)                                                  */
/*       IVT_EX (0x201FD56C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1E0_MCIO_DDRIOEXT_REG 0x0D03256C







/* GDCRXTALKEQATTKRE0_MCIO_DDRIOEXT_REG supported on:                         */
/*       IVT_EP (0x2018D578)                                                  */
/*       IVT_EX (0x201FD578)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRE0_MCIO_DDRIOEXT_REG 0x0D032578



/* GDCRDLLCFGE0_MCIO_DDRIOEXT_REG supported on:                               */
/*       IVT_EP (0x2018D57C)                                                  */
/*       IVT_EX (0x201FD57C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGE0_MCIO_DDRIOEXT_REG 0x0D03257C



/* GDCRRXOFFSETSKEWRANK4567F0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D604)                                                  */
/*       IVT_EX (0x201FD604)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567F0_MCIO_DDRIOEXT_REG 0x0D032604



/* GDCRRXOFFSETCOMPN0AF0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D608)                                                  */
/*       IVT_EX (0x201FD608)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AF0_MCIO_DDRIOEXT_REG 0x0D032608



/* GDCRRXOFFSETCOMPN0BF0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D60C)                                                  */
/*       IVT_EX (0x201FD60C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BF0_MCIO_DDRIOEXT_REG 0x0D03260C



/* GDCRRXOFFSETCOMPN1AF0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D610)                                                  */
/*       IVT_EX (0x201FD610)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AF0_MCIO_DDRIOEXT_REG 0x0D032610



/* GDCRRXOFFSETCOMPN1BF0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D614)                                                  */
/*       IVT_EX (0x201FD614)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BF0_MCIO_DDRIOEXT_REG 0x0D032614



/* GDCRRXOFFSETCOMPCFGF0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D618)                                                  */
/*       IVT_EX (0x201FD618)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGF0_MCIO_DDRIOEXT_REG 0x0D032618



/* GDCRRXOFFSETCOMPSTATUSF0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D61C)                                                  */
/*       IVT_EX (0x201FD61C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSF0_MCIO_DDRIOEXT_REG 0x0D03261C



/* GDCRTXDESKEWGROUP0LANE01F0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D620)                                                  */
/*       IVT_EX (0x201FD620)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01F0_MCIO_DDRIOEXT_REG 0x0D032620



/* GDCRTXDESKEWGROUP0LANE45F0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D624)                                                  */
/*       IVT_EX (0x201FD624)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45F0_MCIO_DDRIOEXT_REG 0x0D032624



/* GDCRRXDESKEWGROUP0LANE01F0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D628)                                                  */
/*       IVT_EX (0x201FD628)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01F0_MCIO_DDRIOEXT_REG 0x0D032628



/* GDCRRXDESKEWGROUP0LANE45F0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D62C)                                                  */
/*       IVT_EX (0x201FD62C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45F0_MCIO_DDRIOEXT_REG 0x0D03262C



/* GDCRTXDESKEWGROUP0LANE23F0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D630)                                                  */
/*       IVT_EX (0x201FD630)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23F0_MCIO_DDRIOEXT_REG 0x0D032630



/* GDCRTXDESKEWGROUP0LANE67F0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D634)                                                  */
/*       IVT_EX (0x201FD634)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67F0_MCIO_DDRIOEXT_REG 0x0D032634



/* GDCRRXDESKEWGROUP0LANE23F0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D638)                                                  */
/*       IVT_EX (0x201FD638)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23F0_MCIO_DDRIOEXT_REG 0x0D032638



/* GDCRRXDESKEWGROUP0LANE67F0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D63C)                                                  */
/*       IVT_EX (0x201FD63C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67F0_MCIO_DDRIOEXT_REG 0x0D03263C



/* GDCRTXDESKEWGROUP1NIB0F0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D640)                                                  */
/*       IVT_EX (0x201FD640)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0F0_MCIO_DDRIOEXT_REG 0x0D032640



/* GDCRTXDESKEWGROUP1NIB1F0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D644)                                                  */
/*       IVT_EX (0x201FD644)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1F0_MCIO_DDRIOEXT_REG 0x0D032644



/* GDCRRXDESKEWGROUP1NIB0F0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D648)                                                  */
/*       IVT_EX (0x201FD648)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0F0_MCIO_DDRIOEXT_REG 0x0D032648



/* GDCRRXDESKEWGROUP1NIB1F0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D64C)                                                  */
/*       IVT_EX (0x201FD64C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1F0_MCIO_DDRIOEXT_REG 0x0D03264C



/* GDCRTXDESKEWGROUP2NIB0F0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D650)                                                  */
/*       IVT_EX (0x201FD650)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0F0_MCIO_DDRIOEXT_REG 0x0D032650



/* GDCRTXDESKEWGROUP2NIB1F0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D654)                                                  */
/*       IVT_EX (0x201FD654)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1F0_MCIO_DDRIOEXT_REG 0x0D032654



/* GDCRRXDESKEWGROUP2NIB0F0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D658)                                                  */
/*       IVT_EX (0x201FD658)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0F0_MCIO_DDRIOEXT_REG 0x0D032658



/* GDCRRXDESKEWGROUP2NIB1F0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D65C)                                                  */
/*       IVT_EX (0x201FD65C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1F0_MCIO_DDRIOEXT_REG 0x0D03265C



/* GDCRTXDESKEWGROUP3NIB0F0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D660)                                                  */
/*       IVT_EX (0x201FD660)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0F0_MCIO_DDRIOEXT_REG 0x0D032660



/* GDCRTXDESKEWGROUP3NIB1F0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D664)                                                  */
/*       IVT_EX (0x201FD664)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1F0_MCIO_DDRIOEXT_REG 0x0D032664



/* GDCRRXDESKEWGROUP3NIB0F0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D668)                                                  */
/*       IVT_EX (0x201FD668)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0F0_MCIO_DDRIOEXT_REG 0x0D032668



/* GDCRRXDESKEWGROUP3NIB1F0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D66C)                                                  */
/*       IVT_EX (0x201FD66C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1F0_MCIO_DDRIOEXT_REG 0x0D03266C







/* GDCRXTALKEQATTKRF0_MCIO_DDRIOEXT_REG supported on:                         */
/*       IVT_EP (0x2018D678)                                                  */
/*       IVT_EX (0x201FD678)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRF0_MCIO_DDRIOEXT_REG 0x0D032678



/* GDCRDLLCFGF0_MCIO_DDRIOEXT_REG supported on:                               */
/*       IVT_EP (0x2018D67C)                                                  */
/*       IVT_EX (0x201FD67C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGF0_MCIO_DDRIOEXT_REG 0x0D03267C



/* GDCRRXOFFSETSKEWRANK4567G0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D684)                                                  */
/*       IVT_EX (0x201FD684)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567G0_MCIO_DDRIOEXT_REG 0x0D032684



/* GDCRRXOFFSETCOMPN0AG0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D688)                                                  */
/*       IVT_EX (0x201FD688)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AG0_MCIO_DDRIOEXT_REG 0x0D032688



/* GDCRRXOFFSETCOMPN0BG0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D68C)                                                  */
/*       IVT_EX (0x201FD68C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BG0_MCIO_DDRIOEXT_REG 0x0D03268C



/* GDCRRXOFFSETCOMPN1AG0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D690)                                                  */
/*       IVT_EX (0x201FD690)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AG0_MCIO_DDRIOEXT_REG 0x0D032690



/* GDCRRXOFFSETCOMPN1BG0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D694)                                                  */
/*       IVT_EX (0x201FD694)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BG0_MCIO_DDRIOEXT_REG 0x0D032694



/* GDCRRXOFFSETCOMPCFGG0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D698)                                                  */
/*       IVT_EX (0x201FD698)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGG0_MCIO_DDRIOEXT_REG 0x0D032698



/* GDCRRXOFFSETCOMPSTATUSG0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D69C)                                                  */
/*       IVT_EX (0x201FD69C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSG0_MCIO_DDRIOEXT_REG 0x0D03269C



/* GDCRTXDESKEWGROUP0LANE01G0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D6A0)                                                  */
/*       IVT_EX (0x201FD6A0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01G0_MCIO_DDRIOEXT_REG 0x0D0326A0



/* GDCRTXDESKEWGROUP0LANE45G0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D6A4)                                                  */
/*       IVT_EX (0x201FD6A4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45G0_MCIO_DDRIOEXT_REG 0x0D0326A4



/* GDCRRXDESKEWGROUP0LANE01G0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D6A8)                                                  */
/*       IVT_EX (0x201FD6A8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01G0_MCIO_DDRIOEXT_REG 0x0D0326A8



/* GDCRRXDESKEWGROUP0LANE45G0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D6AC)                                                  */
/*       IVT_EX (0x201FD6AC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45G0_MCIO_DDRIOEXT_REG 0x0D0326AC



/* GDCRTXDESKEWGROUP0LANE23G0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D6B0)                                                  */
/*       IVT_EX (0x201FD6B0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23G0_MCIO_DDRIOEXT_REG 0x0D0326B0



/* GDCRTXDESKEWGROUP0LANE67G0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D6B4)                                                  */
/*       IVT_EX (0x201FD6B4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67G0_MCIO_DDRIOEXT_REG 0x0D0326B4



/* GDCRRXDESKEWGROUP0LANE23G0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D6B8)                                                  */
/*       IVT_EX (0x201FD6B8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23G0_MCIO_DDRIOEXT_REG 0x0D0326B8



/* GDCRRXDESKEWGROUP0LANE67G0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D6BC)                                                  */
/*       IVT_EX (0x201FD6BC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67G0_MCIO_DDRIOEXT_REG 0x0D0326BC



/* GDCRTXDESKEWGROUP1NIB0G0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D6C0)                                                  */
/*       IVT_EX (0x201FD6C0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0G0_MCIO_DDRIOEXT_REG 0x0D0326C0



/* GDCRTXDESKEWGROUP1NIB1G0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D6C4)                                                  */
/*       IVT_EX (0x201FD6C4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1G0_MCIO_DDRIOEXT_REG 0x0D0326C4



/* GDCRRXDESKEWGROUP1NIB0G0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D6C8)                                                  */
/*       IVT_EX (0x201FD6C8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0G0_MCIO_DDRIOEXT_REG 0x0D0326C8



/* GDCRRXDESKEWGROUP1NIB1G0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D6CC)                                                  */
/*       IVT_EX (0x201FD6CC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1G0_MCIO_DDRIOEXT_REG 0x0D0326CC



/* GDCRTXDESKEWGROUP2NIB0G0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D6D0)                                                  */
/*       IVT_EX (0x201FD6D0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0G0_MCIO_DDRIOEXT_REG 0x0D0326D0



/* GDCRTXDESKEWGROUP2NIB1G0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D6D4)                                                  */
/*       IVT_EX (0x201FD6D4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1G0_MCIO_DDRIOEXT_REG 0x0D0326D4



/* GDCRRXDESKEWGROUP2NIB0G0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D6D8)                                                  */
/*       IVT_EX (0x201FD6D8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0G0_MCIO_DDRIOEXT_REG 0x0D0326D8



/* GDCRRXDESKEWGROUP2NIB1G0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D6DC)                                                  */
/*       IVT_EX (0x201FD6DC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1G0_MCIO_DDRIOEXT_REG 0x0D0326DC



/* GDCRTXDESKEWGROUP3NIB0G0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D6E0)                                                  */
/*       IVT_EX (0x201FD6E0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0G0_MCIO_DDRIOEXT_REG 0x0D0326E0



/* GDCRTXDESKEWGROUP3NIB1G0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D6E4)                                                  */
/*       IVT_EX (0x201FD6E4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1G0_MCIO_DDRIOEXT_REG 0x0D0326E4



/* GDCRRXDESKEWGROUP3NIB0G0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D6E8)                                                  */
/*       IVT_EX (0x201FD6E8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0G0_MCIO_DDRIOEXT_REG 0x0D0326E8



/* GDCRRXDESKEWGROUP3NIB1G0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D6EC)                                                  */
/*       IVT_EX (0x201FD6EC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1G0_MCIO_DDRIOEXT_REG 0x0D0326EC







/* GDCRXTALKEQATTKRG0_MCIO_DDRIOEXT_REG supported on:                         */
/*       IVT_EP (0x2018D6F8)                                                  */
/*       IVT_EX (0x201FD6F8)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRG0_MCIO_DDRIOEXT_REG 0x0D0326F8



/* GDCRDLLCFGG0_MCIO_DDRIOEXT_REG supported on:                               */
/*       IVT_EP (0x2018D6FC)                                                  */
/*       IVT_EX (0x201FD6FC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGG0_MCIO_DDRIOEXT_REG 0x0D0326FC



/* GDCRRXOFFSETSKEWRANK4567H0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D704)                                                  */
/*       IVT_EX (0x201FD704)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567H0_MCIO_DDRIOEXT_REG 0x0D032704



/* GDCRRXOFFSETCOMPN0AH0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D708)                                                  */
/*       IVT_EX (0x201FD708)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AH0_MCIO_DDRIOEXT_REG 0x0D032708



/* GDCRRXOFFSETCOMPN0BH0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D70C)                                                  */
/*       IVT_EX (0x201FD70C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BH0_MCIO_DDRIOEXT_REG 0x0D03270C



/* GDCRRXOFFSETCOMPN1AH0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D710)                                                  */
/*       IVT_EX (0x201FD710)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AH0_MCIO_DDRIOEXT_REG 0x0D032710



/* GDCRRXOFFSETCOMPN1BH0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D714)                                                  */
/*       IVT_EX (0x201FD714)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BH0_MCIO_DDRIOEXT_REG 0x0D032714



/* GDCRRXOFFSETCOMPCFGH0_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D718)                                                  */
/*       IVT_EX (0x201FD718)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGH0_MCIO_DDRIOEXT_REG 0x0D032718



/* GDCRRXOFFSETCOMPSTATUSH0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D71C)                                                  */
/*       IVT_EX (0x201FD71C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSH0_MCIO_DDRIOEXT_REG 0x0D03271C



/* GDCRTXDESKEWGROUP0LANE01H0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D720)                                                  */
/*       IVT_EX (0x201FD720)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01H0_MCIO_DDRIOEXT_REG 0x0D032720



/* GDCRTXDESKEWGROUP0LANE45H0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D724)                                                  */
/*       IVT_EX (0x201FD724)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45H0_MCIO_DDRIOEXT_REG 0x0D032724



/* GDCRRXDESKEWGROUP0LANE01H0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D728)                                                  */
/*       IVT_EX (0x201FD728)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01H0_MCIO_DDRIOEXT_REG 0x0D032728



/* GDCRRXDESKEWGROUP0LANE45H0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D72C)                                                  */
/*       IVT_EX (0x201FD72C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45H0_MCIO_DDRIOEXT_REG 0x0D03272C



/* GDCRTXDESKEWGROUP0LANE23H0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D730)                                                  */
/*       IVT_EX (0x201FD730)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23H0_MCIO_DDRIOEXT_REG 0x0D032730



/* GDCRTXDESKEWGROUP0LANE67H0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D734)                                                  */
/*       IVT_EX (0x201FD734)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67H0_MCIO_DDRIOEXT_REG 0x0D032734



/* GDCRRXDESKEWGROUP0LANE23H0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D738)                                                  */
/*       IVT_EX (0x201FD738)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23H0_MCIO_DDRIOEXT_REG 0x0D032738



/* GDCRRXDESKEWGROUP0LANE67H0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D73C)                                                  */
/*       IVT_EX (0x201FD73C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67H0_MCIO_DDRIOEXT_REG 0x0D03273C



/* GDCRTXDESKEWGROUP1NIB0H0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D740)                                                  */
/*       IVT_EX (0x201FD740)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0H0_MCIO_DDRIOEXT_REG 0x0D032740



/* GDCRTXDESKEWGROUP1NIB1H0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D744)                                                  */
/*       IVT_EX (0x201FD744)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1H0_MCIO_DDRIOEXT_REG 0x0D032744



/* GDCRRXDESKEWGROUP1NIB0H0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D748)                                                  */
/*       IVT_EX (0x201FD748)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0H0_MCIO_DDRIOEXT_REG 0x0D032748



/* GDCRRXDESKEWGROUP1NIB1H0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D74C)                                                  */
/*       IVT_EX (0x201FD74C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1H0_MCIO_DDRIOEXT_REG 0x0D03274C



/* GDCRTXDESKEWGROUP2NIB0H0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D750)                                                  */
/*       IVT_EX (0x201FD750)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0H0_MCIO_DDRIOEXT_REG 0x0D032750



/* GDCRTXDESKEWGROUP2NIB1H0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D754)                                                  */
/*       IVT_EX (0x201FD754)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1H0_MCIO_DDRIOEXT_REG 0x0D032754



/* GDCRRXDESKEWGROUP2NIB0H0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D758)                                                  */
/*       IVT_EX (0x201FD758)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0H0_MCIO_DDRIOEXT_REG 0x0D032758



/* GDCRRXDESKEWGROUP2NIB1H0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D75C)                                                  */
/*       IVT_EX (0x201FD75C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1H0_MCIO_DDRIOEXT_REG 0x0D03275C



/* GDCRTXDESKEWGROUP3NIB0H0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D760)                                                  */
/*       IVT_EX (0x201FD760)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0H0_MCIO_DDRIOEXT_REG 0x0D032760



/* GDCRTXDESKEWGROUP3NIB1H0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D764)                                                  */
/*       IVT_EX (0x201FD764)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1H0_MCIO_DDRIOEXT_REG 0x0D032764



/* GDCRRXDESKEWGROUP3NIB0H0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D768)                                                  */
/*       IVT_EX (0x201FD768)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0H0_MCIO_DDRIOEXT_REG 0x0D032768



/* GDCRRXDESKEWGROUP3NIB1H0_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D76C)                                                  */
/*       IVT_EX (0x201FD76C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1H0_MCIO_DDRIOEXT_REG 0x0D03276C







/* GDCRXTALKEQATTKRH0_MCIO_DDRIOEXT_REG supported on:                         */
/*       IVT_EP (0x2018D778)                                                  */
/*       IVT_EX (0x201FD778)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRH0_MCIO_DDRIOEXT_REG 0x0D032778



/* GDCRDLLCFGH0_MCIO_DDRIOEXT_REG supported on:                               */
/*       IVT_EP (0x2018D77C)                                                  */
/*       IVT_EX (0x201FD77C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGH0_MCIO_DDRIOEXT_REG 0x0D03277C



/* GDCRRXOFFSETSKEWRANK4567ECC0_MCIO_DDRIOEXT_REG supported on:               */
/*       IVT_EP (0x2018D784)                                                  */
/*       IVT_EX (0x201FD784)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567ECC0_MCIO_DDRIOEXT_REG 0x0D032784



/* GDCRRXOFFSETCOMPN0AECC0_MCIO_DDRIOEXT_REG supported on:                    */
/*       IVT_EP (0x2018D788)                                                  */
/*       IVT_EX (0x201FD788)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AECC0_MCIO_DDRIOEXT_REG 0x0D032788



/* GDCRRXOFFSETCOMPN0BECC0_MCIO_DDRIOEXT_REG supported on:                    */
/*       IVT_EP (0x2018D78C)                                                  */
/*       IVT_EX (0x201FD78C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BECC0_MCIO_DDRIOEXT_REG 0x0D03278C



/* GDCRRXOFFSETCOMPN1AECC0_MCIO_DDRIOEXT_REG supported on:                    */
/*       IVT_EP (0x2018D790)                                                  */
/*       IVT_EX (0x201FD790)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AECC0_MCIO_DDRIOEXT_REG 0x0D032790



/* GDCRRXOFFSETCOMPN1BECC0_MCIO_DDRIOEXT_REG supported on:                    */
/*       IVT_EP (0x2018D794)                                                  */
/*       IVT_EX (0x201FD794)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BECC0_MCIO_DDRIOEXT_REG 0x0D032794



/* GDCRRXOFFSETCOMPCFGECC0_MCIO_DDRIOEXT_REG supported on:                    */
/*       IVT_EP (0x2018D798)                                                  */
/*       IVT_EX (0x201FD798)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGECC0_MCIO_DDRIOEXT_REG 0x0D032798



/* GDCRRXOFFSETCOMPSTATUSECC0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D79C)                                                  */
/*       IVT_EX (0x201FD79C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSECC0_MCIO_DDRIOEXT_REG 0x0D03279C



/* GDCRTXDESKEWGROUP0LANE01ECC0_MCIO_DDRIOEXT_REG supported on:               */
/*       IVT_EP (0x2018D7A0)                                                  */
/*       IVT_EX (0x201FD7A0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01ECC0_MCIO_DDRIOEXT_REG 0x0D0327A0



/* GDCRTXDESKEWGROUP0LANE45ECC0_MCIO_DDRIOEXT_REG supported on:               */
/*       IVT_EP (0x2018D7A4)                                                  */
/*       IVT_EX (0x201FD7A4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45ECC0_MCIO_DDRIOEXT_REG 0x0D0327A4



/* GDCRRXDESKEWGROUP0LANE01ECC0_MCIO_DDRIOEXT_REG supported on:               */
/*       IVT_EP (0x2018D7A8)                                                  */
/*       IVT_EX (0x201FD7A8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01ECC0_MCIO_DDRIOEXT_REG 0x0D0327A8



/* GDCRRXDESKEWGROUP0LANE45ECC0_MCIO_DDRIOEXT_REG supported on:               */
/*       IVT_EP (0x2018D7AC)                                                  */
/*       IVT_EX (0x201FD7AC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45ECC0_MCIO_DDRIOEXT_REG 0x0D0327AC



/* GDCRTXDESKEWGROUP0LANE23ECC0_MCIO_DDRIOEXT_REG supported on:               */
/*       IVT_EP (0x2018D7B0)                                                  */
/*       IVT_EX (0x201FD7B0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23ECC0_MCIO_DDRIOEXT_REG 0x0D0327B0



/* GDCRTXDESKEWGROUP0LANE67ECC0_MCIO_DDRIOEXT_REG supported on:               */
/*       IVT_EP (0x2018D7B4)                                                  */
/*       IVT_EX (0x201FD7B4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67ECC0_MCIO_DDRIOEXT_REG 0x0D0327B4



/* GDCRRXDESKEWGROUP0LANE23ECC0_MCIO_DDRIOEXT_REG supported on:               */
/*       IVT_EP (0x2018D7B8)                                                  */
/*       IVT_EX (0x201FD7B8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23ECC0_MCIO_DDRIOEXT_REG 0x0D0327B8



/* GDCRRXDESKEWGROUP0LANE67ECC0_MCIO_DDRIOEXT_REG supported on:               */
/*       IVT_EP (0x2018D7BC)                                                  */
/*       IVT_EX (0x201FD7BC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67ECC0_MCIO_DDRIOEXT_REG 0x0D0327BC



/* GDCRTXDESKEWGROUP1NIB0ECC0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D7C0)                                                  */
/*       IVT_EX (0x201FD7C0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0ECC0_MCIO_DDRIOEXT_REG 0x0D0327C0



/* GDCRTXDESKEWGROUP1NIB1ECC0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D7C4)                                                  */
/*       IVT_EX (0x201FD7C4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1ECC0_MCIO_DDRIOEXT_REG 0x0D0327C4



/* GDCRRXDESKEWGROUP1NIB0ECC0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D7C8)                                                  */
/*       IVT_EX (0x201FD7C8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0ECC0_MCIO_DDRIOEXT_REG 0x0D0327C8



/* GDCRRXDESKEWGROUP1NIB1ECC0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D7CC)                                                  */
/*       IVT_EX (0x201FD7CC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1ECC0_MCIO_DDRIOEXT_REG 0x0D0327CC



/* GDCRTXDESKEWGROUP2NIB0ECC0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D7D0)                                                  */
/*       IVT_EX (0x201FD7D0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0ECC0_MCIO_DDRIOEXT_REG 0x0D0327D0



/* GDCRTXDESKEWGROUP2NIB1ECC0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D7D4)                                                  */
/*       IVT_EX (0x201FD7D4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1ECC0_MCIO_DDRIOEXT_REG 0x0D0327D4



/* GDCRRXDESKEWGROUP2NIB0ECC0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D7D8)                                                  */
/*       IVT_EX (0x201FD7D8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0ECC0_MCIO_DDRIOEXT_REG 0x0D0327D8



/* GDCRRXDESKEWGROUP2NIB1ECC0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D7DC)                                                  */
/*       IVT_EX (0x201FD7DC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1ECC0_MCIO_DDRIOEXT_REG 0x0D0327DC



/* GDCRTXDESKEWGROUP3NIB0ECC0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D7E0)                                                  */
/*       IVT_EX (0x201FD7E0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0ECC0_MCIO_DDRIOEXT_REG 0x0D0327E0



/* GDCRTXDESKEWGROUP3NIB1ECC0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D7E4)                                                  */
/*       IVT_EX (0x201FD7E4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1ECC0_MCIO_DDRIOEXT_REG 0x0D0327E4



/* GDCRRXDESKEWGROUP3NIB0ECC0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D7E8)                                                  */
/*       IVT_EX (0x201FD7E8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0ECC0_MCIO_DDRIOEXT_REG 0x0D0327E8



/* GDCRRXDESKEWGROUP3NIB1ECC0_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D7EC)                                                  */
/*       IVT_EX (0x201FD7EC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1ECC0_MCIO_DDRIOEXT_REG 0x0D0327EC







/* GDCRXTALKEQATTKRECC0_MCIO_DDRIOEXT_REG supported on:                       */
/*       IVT_EP (0x2018D7F8)                                                  */
/*       IVT_EX (0x201FD7F8)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRECC0_MCIO_DDRIOEXT_REG 0x0D0327F8



/* GDCRDLLCFGECC0_MCIO_DDRIOEXT_REG supported on:                             */
/*       IVT_EP (0x2018D7FC)                                                  */
/*       IVT_EX (0x201FD7FC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGECC0_MCIO_DDRIOEXT_REG 0x0D0327FC



/* GDCRRXOFFSETSKEWRANK4567A1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D904)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567A1_MCIO_DDRIOEXT_REG 0x0D032904



/* GDCRRXOFFSETCOMPN0AA1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D908)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AA1_MCIO_DDRIOEXT_REG 0x0D032908



/* GDCRRXOFFSETCOMPN0BA1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D90C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BA1_MCIO_DDRIOEXT_REG 0x0D03290C



/* GDCRRXOFFSETCOMPN1AA1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D910)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AA1_MCIO_DDRIOEXT_REG 0x0D032910



/* GDCRRXOFFSETCOMPN1BA1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D914)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BA1_MCIO_DDRIOEXT_REG 0x0D032914



/* GDCRRXOFFSETCOMPCFGA1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D918)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGA1_MCIO_DDRIOEXT_REG 0x0D032918



/* GDCRRXOFFSETCOMPSTATUSA1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D91C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSA1_MCIO_DDRIOEXT_REG 0x0D03291C



/* GDCRTXDESKEWGROUP0LANE01A1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D920)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01A1_MCIO_DDRIOEXT_REG 0x0D032920



/* GDCRTXDESKEWGROUP0LANE45A1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D924)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45A1_MCIO_DDRIOEXT_REG 0x0D032924



/* GDCRRXDESKEWGROUP0LANE01A1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D928)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01A1_MCIO_DDRIOEXT_REG 0x0D032928



/* GDCRRXDESKEWGROUP0LANE45A1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D92C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45A1_MCIO_DDRIOEXT_REG 0x0D03292C



/* GDCRTXDESKEWGROUP0LANE23A1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D930)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23A1_MCIO_DDRIOEXT_REG 0x0D032930



/* GDCRTXDESKEWGROUP0LANE67A1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D934)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67A1_MCIO_DDRIOEXT_REG 0x0D032934



/* GDCRRXDESKEWGROUP0LANE23A1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D938)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23A1_MCIO_DDRIOEXT_REG 0x0D032938



/* GDCRRXDESKEWGROUP0LANE67A1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D93C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67A1_MCIO_DDRIOEXT_REG 0x0D03293C



/* GDCRTXDESKEWGROUP1NIB0A1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D940)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0A1_MCIO_DDRIOEXT_REG 0x0D032940



/* GDCRTXDESKEWGROUP1NIB1A1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D944)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1A1_MCIO_DDRIOEXT_REG 0x0D032944



/* GDCRRXDESKEWGROUP1NIB0A1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D948)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0A1_MCIO_DDRIOEXT_REG 0x0D032948



/* GDCRRXDESKEWGROUP1NIB1A1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D94C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1A1_MCIO_DDRIOEXT_REG 0x0D03294C



/* GDCRTXDESKEWGROUP2NIB0A1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D950)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0A1_MCIO_DDRIOEXT_REG 0x0D032950



/* GDCRTXDESKEWGROUP2NIB1A1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D954)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1A1_MCIO_DDRIOEXT_REG 0x0D032954



/* GDCRRXDESKEWGROUP2NIB0A1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D958)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0A1_MCIO_DDRIOEXT_REG 0x0D032958



/* GDCRRXDESKEWGROUP2NIB1A1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D95C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1A1_MCIO_DDRIOEXT_REG 0x0D03295C



/* GDCRTXDESKEWGROUP3NIB0A1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D960)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0A1_MCIO_DDRIOEXT_REG 0x0D032960



/* GDCRTXDESKEWGROUP3NIB1A1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D964)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1A1_MCIO_DDRIOEXT_REG 0x0D032964



/* GDCRRXDESKEWGROUP3NIB0A1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D968)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0A1_MCIO_DDRIOEXT_REG 0x0D032968



/* GDCRRXDESKEWGROUP3NIB1A1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D96C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1A1_MCIO_DDRIOEXT_REG 0x0D03296C







/* GDCRXTALKEQATTKRA1_MCIO_DDRIOEXT_REG supported on:                         */
/*       IVT_EP (0x2018D978)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRA1_MCIO_DDRIOEXT_REG 0x0D032978



/* GDCRDLLCFGA1_MCIO_DDRIOEXT_REG supported on:                               */
/*       IVT_EP (0x2018D97C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGA1_MCIO_DDRIOEXT_REG 0x0D03297C



/* GDCRRXOFFSETSKEWRANK4567B1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D984)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567B1_MCIO_DDRIOEXT_REG 0x0D032984



/* GDCRRXOFFSETCOMPN0AB1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D988)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AB1_MCIO_DDRIOEXT_REG 0x0D032988



/* GDCRRXOFFSETCOMPN0BB1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D98C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BB1_MCIO_DDRIOEXT_REG 0x0D03298C



/* GDCRRXOFFSETCOMPN1AB1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D990)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AB1_MCIO_DDRIOEXT_REG 0x0D032990



/* GDCRRXOFFSETCOMPN1BB1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D994)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BB1_MCIO_DDRIOEXT_REG 0x0D032994



/* GDCRRXOFFSETCOMPCFGB1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018D998)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGB1_MCIO_DDRIOEXT_REG 0x0D032998



/* GDCRRXOFFSETCOMPSTATUSB1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D99C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSB1_MCIO_DDRIOEXT_REG 0x0D03299C



/* GDCRTXDESKEWGROUP0LANE01B1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D9A0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01B1_MCIO_DDRIOEXT_REG 0x0D0329A0



/* GDCRTXDESKEWGROUP0LANE45B1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D9A4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45B1_MCIO_DDRIOEXT_REG 0x0D0329A4



/* GDCRRXDESKEWGROUP0LANE01B1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D9A8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01B1_MCIO_DDRIOEXT_REG 0x0D0329A8



/* GDCRRXDESKEWGROUP0LANE45B1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D9AC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45B1_MCIO_DDRIOEXT_REG 0x0D0329AC



/* GDCRTXDESKEWGROUP0LANE23B1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D9B0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23B1_MCIO_DDRIOEXT_REG 0x0D0329B0



/* GDCRTXDESKEWGROUP0LANE67B1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D9B4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67B1_MCIO_DDRIOEXT_REG 0x0D0329B4



/* GDCRRXDESKEWGROUP0LANE23B1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D9B8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23B1_MCIO_DDRIOEXT_REG 0x0D0329B8



/* GDCRRXDESKEWGROUP0LANE67B1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018D9BC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67B1_MCIO_DDRIOEXT_REG 0x0D0329BC



/* GDCRTXDESKEWGROUP1NIB0B1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D9C0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0B1_MCIO_DDRIOEXT_REG 0x0D0329C0



/* GDCRTXDESKEWGROUP1NIB1B1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D9C4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1B1_MCIO_DDRIOEXT_REG 0x0D0329C4



/* GDCRRXDESKEWGROUP1NIB0B1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D9C8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0B1_MCIO_DDRIOEXT_REG 0x0D0329C8



/* GDCRRXDESKEWGROUP1NIB1B1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D9CC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1B1_MCIO_DDRIOEXT_REG 0x0D0329CC



/* GDCRTXDESKEWGROUP2NIB0B1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D9D0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0B1_MCIO_DDRIOEXT_REG 0x0D0329D0



/* GDCRTXDESKEWGROUP2NIB1B1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D9D4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1B1_MCIO_DDRIOEXT_REG 0x0D0329D4



/* GDCRRXDESKEWGROUP2NIB0B1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D9D8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0B1_MCIO_DDRIOEXT_REG 0x0D0329D8



/* GDCRRXDESKEWGROUP2NIB1B1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D9DC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1B1_MCIO_DDRIOEXT_REG 0x0D0329DC



/* GDCRTXDESKEWGROUP3NIB0B1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D9E0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0B1_MCIO_DDRIOEXT_REG 0x0D0329E0



/* GDCRTXDESKEWGROUP3NIB1B1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D9E4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1B1_MCIO_DDRIOEXT_REG 0x0D0329E4



/* GDCRRXDESKEWGROUP3NIB0B1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D9E8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0B1_MCIO_DDRIOEXT_REG 0x0D0329E8



/* GDCRRXDESKEWGROUP3NIB1B1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018D9EC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1B1_MCIO_DDRIOEXT_REG 0x0D0329EC







/* GDCRXTALKEQATTKRB1_MCIO_DDRIOEXT_REG supported on:                         */
/*       IVT_EP (0x2018D9F8)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRB1_MCIO_DDRIOEXT_REG 0x0D0329F8



/* GDCRDLLCFGB1_MCIO_DDRIOEXT_REG supported on:                               */
/*       IVT_EP (0x2018D9FC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGB1_MCIO_DDRIOEXT_REG 0x0D0329FC



/* GDCRRXOFFSETSKEWRANK4567C1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DA04)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567C1_MCIO_DDRIOEXT_REG 0x0D032A04



/* GDCRRXOFFSETCOMPN0AC1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DA08)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AC1_MCIO_DDRIOEXT_REG 0x0D032A08



/* GDCRRXOFFSETCOMPN0BC1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DA0C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BC1_MCIO_DDRIOEXT_REG 0x0D032A0C



/* GDCRRXOFFSETCOMPN1AC1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DA10)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AC1_MCIO_DDRIOEXT_REG 0x0D032A10



/* GDCRRXOFFSETCOMPN1BC1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DA14)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BC1_MCIO_DDRIOEXT_REG 0x0D032A14



/* GDCRRXOFFSETCOMPCFGC1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DA18)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGC1_MCIO_DDRIOEXT_REG 0x0D032A18



/* GDCRRXOFFSETCOMPSTATUSC1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DA1C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSC1_MCIO_DDRIOEXT_REG 0x0D032A1C



/* GDCRTXDESKEWGROUP0LANE01C1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DA20)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01C1_MCIO_DDRIOEXT_REG 0x0D032A20



/* GDCRTXDESKEWGROUP0LANE45C1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DA24)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45C1_MCIO_DDRIOEXT_REG 0x0D032A24



/* GDCRRXDESKEWGROUP0LANE01C1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DA28)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01C1_MCIO_DDRIOEXT_REG 0x0D032A28



/* GDCRRXDESKEWGROUP0LANE45C1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DA2C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45C1_MCIO_DDRIOEXT_REG 0x0D032A2C



/* GDCRTXDESKEWGROUP0LANE23C1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DA30)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23C1_MCIO_DDRIOEXT_REG 0x0D032A30



/* GDCRTXDESKEWGROUP0LANE67C1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DA34)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67C1_MCIO_DDRIOEXT_REG 0x0D032A34



/* GDCRRXDESKEWGROUP0LANE23C1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DA38)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23C1_MCIO_DDRIOEXT_REG 0x0D032A38



/* GDCRRXDESKEWGROUP0LANE67C1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DA3C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67C1_MCIO_DDRIOEXT_REG 0x0D032A3C



/* GDCRTXDESKEWGROUP1NIB0C1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DA40)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0C1_MCIO_DDRIOEXT_REG 0x0D032A40



/* GDCRTXDESKEWGROUP1NIB1C1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DA44)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1C1_MCIO_DDRIOEXT_REG 0x0D032A44



/* GDCRRXDESKEWGROUP1NIB0C1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DA48)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0C1_MCIO_DDRIOEXT_REG 0x0D032A48



/* GDCRRXDESKEWGROUP1NIB1C1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DA4C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1C1_MCIO_DDRIOEXT_REG 0x0D032A4C



/* GDCRTXDESKEWGROUP2NIB0C1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DA50)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0C1_MCIO_DDRIOEXT_REG 0x0D032A50



/* GDCRTXDESKEWGROUP2NIB1C1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DA54)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1C1_MCIO_DDRIOEXT_REG 0x0D032A54



/* GDCRRXDESKEWGROUP2NIB0C1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DA58)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0C1_MCIO_DDRIOEXT_REG 0x0D032A58



/* GDCRRXDESKEWGROUP2NIB1C1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DA5C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1C1_MCIO_DDRIOEXT_REG 0x0D032A5C



/* GDCRTXDESKEWGROUP3NIB0C1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DA60)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0C1_MCIO_DDRIOEXT_REG 0x0D032A60



/* GDCRTXDESKEWGROUP3NIB1C1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DA64)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1C1_MCIO_DDRIOEXT_REG 0x0D032A64



/* GDCRRXDESKEWGROUP3NIB0C1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DA68)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0C1_MCIO_DDRIOEXT_REG 0x0D032A68



/* GDCRRXDESKEWGROUP3NIB1C1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DA6C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1C1_MCIO_DDRIOEXT_REG 0x0D032A6C







/* GDCRXTALKEQATTKRC1_MCIO_DDRIOEXT_REG supported on:                         */
/*       IVT_EP (0x2018DA78)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRC1_MCIO_DDRIOEXT_REG 0x0D032A78



/* GDCRDLLCFGC1_MCIO_DDRIOEXT_REG supported on:                               */
/*       IVT_EP (0x2018DA7C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGC1_MCIO_DDRIOEXT_REG 0x0D032A7C



/* GDCRRXOFFSETSKEWRANK4567D1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DA84)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567D1_MCIO_DDRIOEXT_REG 0x0D032A84



/* GDCRRXOFFSETCOMPN0AD1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DA88)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AD1_MCIO_DDRIOEXT_REG 0x0D032A88



/* GDCRRXOFFSETCOMPN0BD1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DA8C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BD1_MCIO_DDRIOEXT_REG 0x0D032A8C



/* GDCRRXOFFSETCOMPN1AD1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DA90)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AD1_MCIO_DDRIOEXT_REG 0x0D032A90



/* GDCRRXOFFSETCOMPN1BD1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DA94)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BD1_MCIO_DDRIOEXT_REG 0x0D032A94



/* GDCRRXOFFSETCOMPCFGD1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DA98)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGD1_MCIO_DDRIOEXT_REG 0x0D032A98



/* GDCRRXOFFSETCOMPSTATUSD1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DA9C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSD1_MCIO_DDRIOEXT_REG 0x0D032A9C



/* GDCRTXDESKEWGROUP0LANE01D1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DAA0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01D1_MCIO_DDRIOEXT_REG 0x0D032AA0



/* GDCRTXDESKEWGROUP0LANE45D1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DAA4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45D1_MCIO_DDRIOEXT_REG 0x0D032AA4



/* GDCRRXDESKEWGROUP0LANE01D1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DAA8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01D1_MCIO_DDRIOEXT_REG 0x0D032AA8



/* GDCRRXDESKEWGROUP0LANE45D1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DAAC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45D1_MCIO_DDRIOEXT_REG 0x0D032AAC



/* GDCRTXDESKEWGROUP0LANE23D1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DAB0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23D1_MCIO_DDRIOEXT_REG 0x0D032AB0



/* GDCRTXDESKEWGROUP0LANE67D1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DAB4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67D1_MCIO_DDRIOEXT_REG 0x0D032AB4



/* GDCRRXDESKEWGROUP0LANE23D1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DAB8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23D1_MCIO_DDRIOEXT_REG 0x0D032AB8



/* GDCRRXDESKEWGROUP0LANE67D1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DABC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67D1_MCIO_DDRIOEXT_REG 0x0D032ABC



/* GDCRTXDESKEWGROUP1NIB0D1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DAC0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0D1_MCIO_DDRIOEXT_REG 0x0D032AC0



/* GDCRTXDESKEWGROUP1NIB1D1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DAC4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1D1_MCIO_DDRIOEXT_REG 0x0D032AC4



/* GDCRRXDESKEWGROUP1NIB0D1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DAC8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0D1_MCIO_DDRIOEXT_REG 0x0D032AC8



/* GDCRRXDESKEWGROUP1NIB1D1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DACC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1D1_MCIO_DDRIOEXT_REG 0x0D032ACC



/* GDCRTXDESKEWGROUP2NIB0D1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DAD0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0D1_MCIO_DDRIOEXT_REG 0x0D032AD0



/* GDCRTXDESKEWGROUP2NIB1D1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DAD4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1D1_MCIO_DDRIOEXT_REG 0x0D032AD4



/* GDCRRXDESKEWGROUP2NIB0D1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DAD8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0D1_MCIO_DDRIOEXT_REG 0x0D032AD8



/* GDCRRXDESKEWGROUP2NIB1D1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DADC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1D1_MCIO_DDRIOEXT_REG 0x0D032ADC



/* GDCRTXDESKEWGROUP3NIB0D1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DAE0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0D1_MCIO_DDRIOEXT_REG 0x0D032AE0



/* GDCRTXDESKEWGROUP3NIB1D1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DAE4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1D1_MCIO_DDRIOEXT_REG 0x0D032AE4



/* GDCRRXDESKEWGROUP3NIB0D1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DAE8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0D1_MCIO_DDRIOEXT_REG 0x0D032AE8



/* GDCRRXDESKEWGROUP3NIB1D1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DAEC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1D1_MCIO_DDRIOEXT_REG 0x0D032AEC







/* GDCRXTALKEQATTKRD1_MCIO_DDRIOEXT_REG supported on:                         */
/*       IVT_EP (0x2018DAF8)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRD1_MCIO_DDRIOEXT_REG 0x0D032AF8



/* GDCRDLLCFGD1_MCIO_DDRIOEXT_REG supported on:                               */
/*       IVT_EP (0x2018DAFC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGD1_MCIO_DDRIOEXT_REG 0x0D032AFC



/* GDCRRXOFFSETSKEWRANK4567E1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DD04)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567E1_MCIO_DDRIOEXT_REG 0x0D032D04



/* GDCRRXOFFSETCOMPN0AE1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DD08)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AE1_MCIO_DDRIOEXT_REG 0x0D032D08



/* GDCRRXOFFSETCOMPN0BE1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DD0C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BE1_MCIO_DDRIOEXT_REG 0x0D032D0C



/* GDCRRXOFFSETCOMPN1AE1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DD10)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AE1_MCIO_DDRIOEXT_REG 0x0D032D10



/* GDCRRXOFFSETCOMPN1BE1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DD14)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BE1_MCIO_DDRIOEXT_REG 0x0D032D14



/* GDCRRXOFFSETCOMPCFGE1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DD18)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGE1_MCIO_DDRIOEXT_REG 0x0D032D18



/* GDCRRXOFFSETCOMPSTATUSE1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DD1C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSE1_MCIO_DDRIOEXT_REG 0x0D032D1C



/* GDCRTXDESKEWGROUP0LANE01E1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DD20)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01E1_MCIO_DDRIOEXT_REG 0x0D032D20



/* GDCRTXDESKEWGROUP0LANE45E1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DD24)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45E1_MCIO_DDRIOEXT_REG 0x0D032D24



/* GDCRRXDESKEWGROUP0LANE01E1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DD28)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01E1_MCIO_DDRIOEXT_REG 0x0D032D28



/* GDCRRXDESKEWGROUP0LANE45E1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DD2C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45E1_MCIO_DDRIOEXT_REG 0x0D032D2C



/* GDCRTXDESKEWGROUP0LANE23E1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DD30)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23E1_MCIO_DDRIOEXT_REG 0x0D032D30



/* GDCRTXDESKEWGROUP0LANE67E1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DD34)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67E1_MCIO_DDRIOEXT_REG 0x0D032D34



/* GDCRRXDESKEWGROUP0LANE23E1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DD38)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23E1_MCIO_DDRIOEXT_REG 0x0D032D38



/* GDCRRXDESKEWGROUP0LANE67E1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DD3C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67E1_MCIO_DDRIOEXT_REG 0x0D032D3C



/* GDCRTXDESKEWGROUP1NIB0E1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DD40)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0E1_MCIO_DDRIOEXT_REG 0x0D032D40



/* GDCRTXDESKEWGROUP1NIB1E1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DD44)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1E1_MCIO_DDRIOEXT_REG 0x0D032D44



/* GDCRRXDESKEWGROUP1NIB0E1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DD48)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0E1_MCIO_DDRIOEXT_REG 0x0D032D48



/* GDCRRXDESKEWGROUP1NIB1E1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DD4C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1E1_MCIO_DDRIOEXT_REG 0x0D032D4C



/* GDCRTXDESKEWGROUP2NIB0E1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DD50)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0E1_MCIO_DDRIOEXT_REG 0x0D032D50



/* GDCRTXDESKEWGROUP2NIB1E1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DD54)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1E1_MCIO_DDRIOEXT_REG 0x0D032D54



/* GDCRRXDESKEWGROUP2NIB0E1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DD58)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0E1_MCIO_DDRIOEXT_REG 0x0D032D58



/* GDCRRXDESKEWGROUP2NIB1E1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DD5C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1E1_MCIO_DDRIOEXT_REG 0x0D032D5C



/* GDCRTXDESKEWGROUP3NIB0E1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DD60)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0E1_MCIO_DDRIOEXT_REG 0x0D032D60



/* GDCRTXDESKEWGROUP3NIB1E1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DD64)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1E1_MCIO_DDRIOEXT_REG 0x0D032D64



/* GDCRRXDESKEWGROUP3NIB0E1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DD68)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0E1_MCIO_DDRIOEXT_REG 0x0D032D68



/* GDCRRXDESKEWGROUP3NIB1E1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DD6C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1E1_MCIO_DDRIOEXT_REG 0x0D032D6C







/* GDCRXTALKEQATTKRE1_MCIO_DDRIOEXT_REG supported on:                         */
/*       IVT_EP (0x2018DD78)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRE1_MCIO_DDRIOEXT_REG 0x0D032D78



/* GDCRDLLCFGE1_MCIO_DDRIOEXT_REG supported on:                               */
/*       IVT_EP (0x2018DD7C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGE1_MCIO_DDRIOEXT_REG 0x0D032D7C



/* GDCRRXOFFSETSKEWRANK4567F1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DE04)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567F1_MCIO_DDRIOEXT_REG 0x0D032E04



/* GDCRRXOFFSETCOMPN0AF1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DE08)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AF1_MCIO_DDRIOEXT_REG 0x0D032E08



/* GDCRRXOFFSETCOMPN0BF1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DE0C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BF1_MCIO_DDRIOEXT_REG 0x0D032E0C



/* GDCRRXOFFSETCOMPN1AF1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DE10)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AF1_MCIO_DDRIOEXT_REG 0x0D032E10



/* GDCRRXOFFSETCOMPN1BF1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DE14)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BF1_MCIO_DDRIOEXT_REG 0x0D032E14



/* GDCRRXOFFSETCOMPCFGF1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DE18)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGF1_MCIO_DDRIOEXT_REG 0x0D032E18



/* GDCRRXOFFSETCOMPSTATUSF1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DE1C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSF1_MCIO_DDRIOEXT_REG 0x0D032E1C



/* GDCRTXDESKEWGROUP0LANE01F1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DE20)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01F1_MCIO_DDRIOEXT_REG 0x0D032E20



/* GDCRTXDESKEWGROUP0LANE45F1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DE24)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45F1_MCIO_DDRIOEXT_REG 0x0D032E24



/* GDCRRXDESKEWGROUP0LANE01F1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DE28)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01F1_MCIO_DDRIOEXT_REG 0x0D032E28



/* GDCRRXDESKEWGROUP0LANE45F1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DE2C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45F1_MCIO_DDRIOEXT_REG 0x0D032E2C



/* GDCRTXDESKEWGROUP0LANE23F1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DE30)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23F1_MCIO_DDRIOEXT_REG 0x0D032E30



/* GDCRTXDESKEWGROUP0LANE67F1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DE34)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67F1_MCIO_DDRIOEXT_REG 0x0D032E34



/* GDCRRXDESKEWGROUP0LANE23F1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DE38)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23F1_MCIO_DDRIOEXT_REG 0x0D032E38



/* GDCRRXDESKEWGROUP0LANE67F1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DE3C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67F1_MCIO_DDRIOEXT_REG 0x0D032E3C



/* GDCRTXDESKEWGROUP1NIB0F1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DE40)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0F1_MCIO_DDRIOEXT_REG 0x0D032E40



/* GDCRTXDESKEWGROUP1NIB1F1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DE44)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1F1_MCIO_DDRIOEXT_REG 0x0D032E44



/* GDCRRXDESKEWGROUP1NIB0F1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DE48)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0F1_MCIO_DDRIOEXT_REG 0x0D032E48



/* GDCRRXDESKEWGROUP1NIB1F1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DE4C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1F1_MCIO_DDRIOEXT_REG 0x0D032E4C



/* GDCRTXDESKEWGROUP2NIB0F1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DE50)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0F1_MCIO_DDRIOEXT_REG 0x0D032E50



/* GDCRTXDESKEWGROUP2NIB1F1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DE54)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1F1_MCIO_DDRIOEXT_REG 0x0D032E54



/* GDCRRXDESKEWGROUP2NIB0F1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DE58)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0F1_MCIO_DDRIOEXT_REG 0x0D032E58



/* GDCRRXDESKEWGROUP2NIB1F1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DE5C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1F1_MCIO_DDRIOEXT_REG 0x0D032E5C



/* GDCRTXDESKEWGROUP3NIB0F1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DE60)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0F1_MCIO_DDRIOEXT_REG 0x0D032E60



/* GDCRTXDESKEWGROUP3NIB1F1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DE64)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1F1_MCIO_DDRIOEXT_REG 0x0D032E64



/* GDCRRXDESKEWGROUP3NIB0F1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DE68)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0F1_MCIO_DDRIOEXT_REG 0x0D032E68



/* GDCRRXDESKEWGROUP3NIB1F1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DE6C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1F1_MCIO_DDRIOEXT_REG 0x0D032E6C







/* GDCRXTALKEQATTKRF1_MCIO_DDRIOEXT_REG supported on:                         */
/*       IVT_EP (0x2018DE78)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRF1_MCIO_DDRIOEXT_REG 0x0D032E78



/* GDCRDLLCFGF1_MCIO_DDRIOEXT_REG supported on:                               */
/*       IVT_EP (0x2018DE7C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGF1_MCIO_DDRIOEXT_REG 0x0D032E7C



/* GDCRRXOFFSETSKEWRANK4567G1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DE84)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567G1_MCIO_DDRIOEXT_REG 0x0D032E84



/* GDCRRXOFFSETCOMPN0AG1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DE88)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AG1_MCIO_DDRIOEXT_REG 0x0D032E88



/* GDCRRXOFFSETCOMPN0BG1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DE8C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BG1_MCIO_DDRIOEXT_REG 0x0D032E8C



/* GDCRRXOFFSETCOMPN1AG1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DE90)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AG1_MCIO_DDRIOEXT_REG 0x0D032E90



/* GDCRRXOFFSETCOMPN1BG1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DE94)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BG1_MCIO_DDRIOEXT_REG 0x0D032E94



/* GDCRRXOFFSETCOMPCFGG1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DE98)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGG1_MCIO_DDRIOEXT_REG 0x0D032E98



/* GDCRRXOFFSETCOMPSTATUSG1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DE9C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSG1_MCIO_DDRIOEXT_REG 0x0D032E9C



/* GDCRTXDESKEWGROUP0LANE01G1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DEA0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01G1_MCIO_DDRIOEXT_REG 0x0D032EA0



/* GDCRTXDESKEWGROUP0LANE45G1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DEA4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45G1_MCIO_DDRIOEXT_REG 0x0D032EA4



/* GDCRRXDESKEWGROUP0LANE01G1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DEA8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01G1_MCIO_DDRIOEXT_REG 0x0D032EA8



/* GDCRRXDESKEWGROUP0LANE45G1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DEAC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45G1_MCIO_DDRIOEXT_REG 0x0D032EAC



/* GDCRTXDESKEWGROUP0LANE23G1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DEB0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23G1_MCIO_DDRIOEXT_REG 0x0D032EB0



/* GDCRTXDESKEWGROUP0LANE67G1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DEB4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67G1_MCIO_DDRIOEXT_REG 0x0D032EB4



/* GDCRRXDESKEWGROUP0LANE23G1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DEB8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23G1_MCIO_DDRIOEXT_REG 0x0D032EB8



/* GDCRRXDESKEWGROUP0LANE67G1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DEBC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67G1_MCIO_DDRIOEXT_REG 0x0D032EBC



/* GDCRTXDESKEWGROUP1NIB0G1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DEC0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0G1_MCIO_DDRIOEXT_REG 0x0D032EC0



/* GDCRTXDESKEWGROUP1NIB1G1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DEC4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1G1_MCIO_DDRIOEXT_REG 0x0D032EC4



/* GDCRRXDESKEWGROUP1NIB0G1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DEC8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0G1_MCIO_DDRIOEXT_REG 0x0D032EC8



/* GDCRRXDESKEWGROUP1NIB1G1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DECC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1G1_MCIO_DDRIOEXT_REG 0x0D032ECC



/* GDCRTXDESKEWGROUP2NIB0G1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DED0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0G1_MCIO_DDRIOEXT_REG 0x0D032ED0



/* GDCRTXDESKEWGROUP2NIB1G1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DED4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1G1_MCIO_DDRIOEXT_REG 0x0D032ED4



/* GDCRRXDESKEWGROUP2NIB0G1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DED8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0G1_MCIO_DDRIOEXT_REG 0x0D032ED8



/* GDCRRXDESKEWGROUP2NIB1G1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DEDC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1G1_MCIO_DDRIOEXT_REG 0x0D032EDC



/* GDCRTXDESKEWGROUP3NIB0G1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DEE0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0G1_MCIO_DDRIOEXT_REG 0x0D032EE0



/* GDCRTXDESKEWGROUP3NIB1G1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DEE4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1G1_MCIO_DDRIOEXT_REG 0x0D032EE4



/* GDCRRXDESKEWGROUP3NIB0G1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DEE8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0G1_MCIO_DDRIOEXT_REG 0x0D032EE8



/* GDCRRXDESKEWGROUP3NIB1G1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DEEC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1G1_MCIO_DDRIOEXT_REG 0x0D032EEC







/* GDCRXTALKEQATTKRG1_MCIO_DDRIOEXT_REG supported on:                         */
/*       IVT_EP (0x2018DEF8)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRG1_MCIO_DDRIOEXT_REG 0x0D032EF8



/* GDCRDLLCFGG1_MCIO_DDRIOEXT_REG supported on:                               */
/*       IVT_EP (0x2018DEFC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGG1_MCIO_DDRIOEXT_REG 0x0D032EFC



/* GDCRRXOFFSETSKEWRANK4567H1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DF04)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567H1_MCIO_DDRIOEXT_REG 0x0D032F04



/* GDCRRXOFFSETCOMPN0AH1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DF08)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AH1_MCIO_DDRIOEXT_REG 0x0D032F08



/* GDCRRXOFFSETCOMPN0BH1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DF0C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BH1_MCIO_DDRIOEXT_REG 0x0D032F0C



/* GDCRRXOFFSETCOMPN1AH1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DF10)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AH1_MCIO_DDRIOEXT_REG 0x0D032F10



/* GDCRRXOFFSETCOMPN1BH1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DF14)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BH1_MCIO_DDRIOEXT_REG 0x0D032F14



/* GDCRRXOFFSETCOMPCFGH1_MCIO_DDRIOEXT_REG supported on:                      */
/*       IVT_EP (0x2018DF18)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGH1_MCIO_DDRIOEXT_REG 0x0D032F18



/* GDCRRXOFFSETCOMPSTATUSH1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DF1C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSH1_MCIO_DDRIOEXT_REG 0x0D032F1C



/* GDCRTXDESKEWGROUP0LANE01H1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DF20)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01H1_MCIO_DDRIOEXT_REG 0x0D032F20



/* GDCRTXDESKEWGROUP0LANE45H1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DF24)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45H1_MCIO_DDRIOEXT_REG 0x0D032F24



/* GDCRRXDESKEWGROUP0LANE01H1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DF28)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01H1_MCIO_DDRIOEXT_REG 0x0D032F28



/* GDCRRXDESKEWGROUP0LANE45H1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DF2C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45H1_MCIO_DDRIOEXT_REG 0x0D032F2C



/* GDCRTXDESKEWGROUP0LANE23H1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DF30)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23H1_MCIO_DDRIOEXT_REG 0x0D032F30



/* GDCRTXDESKEWGROUP0LANE67H1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DF34)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67H1_MCIO_DDRIOEXT_REG 0x0D032F34



/* GDCRRXDESKEWGROUP0LANE23H1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DF38)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23H1_MCIO_DDRIOEXT_REG 0x0D032F38



/* GDCRRXDESKEWGROUP0LANE67H1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DF3C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67H1_MCIO_DDRIOEXT_REG 0x0D032F3C



/* GDCRTXDESKEWGROUP1NIB0H1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DF40)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0H1_MCIO_DDRIOEXT_REG 0x0D032F40



/* GDCRTXDESKEWGROUP1NIB1H1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DF44)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1H1_MCIO_DDRIOEXT_REG 0x0D032F44



/* GDCRRXDESKEWGROUP1NIB0H1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DF48)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0H1_MCIO_DDRIOEXT_REG 0x0D032F48



/* GDCRRXDESKEWGROUP1NIB1H1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DF4C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1H1_MCIO_DDRIOEXT_REG 0x0D032F4C



/* GDCRTXDESKEWGROUP2NIB0H1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DF50)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0H1_MCIO_DDRIOEXT_REG 0x0D032F50



/* GDCRTXDESKEWGROUP2NIB1H1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DF54)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1H1_MCIO_DDRIOEXT_REG 0x0D032F54



/* GDCRRXDESKEWGROUP2NIB0H1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DF58)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0H1_MCIO_DDRIOEXT_REG 0x0D032F58



/* GDCRRXDESKEWGROUP2NIB1H1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DF5C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1H1_MCIO_DDRIOEXT_REG 0x0D032F5C



/* GDCRTXDESKEWGROUP3NIB0H1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DF60)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0H1_MCIO_DDRIOEXT_REG 0x0D032F60



/* GDCRTXDESKEWGROUP3NIB1H1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DF64)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1H1_MCIO_DDRIOEXT_REG 0x0D032F64



/* GDCRRXDESKEWGROUP3NIB0H1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DF68)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0H1_MCIO_DDRIOEXT_REG 0x0D032F68



/* GDCRRXDESKEWGROUP3NIB1H1_MCIO_DDRIOEXT_REG supported on:                   */
/*       IVT_EP (0x2018DF6C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1H1_MCIO_DDRIOEXT_REG 0x0D032F6C







/* GDCRXTALKEQATTKRH1_MCIO_DDRIOEXT_REG supported on:                         */
/*       IVT_EP (0x2018DF78)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRH1_MCIO_DDRIOEXT_REG 0x0D032F78



/* GDCRDLLCFGH1_MCIO_DDRIOEXT_REG supported on:                               */
/*       IVT_EP (0x2018DF7C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGH1_MCIO_DDRIOEXT_REG 0x0D032F7C



/* GDCRRXOFFSETSKEWRANK4567ECC1_MCIO_DDRIOEXT_REG supported on:               */
/*       IVT_EP (0x2018DF84)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETSKEWRANK4567ECC1_MCIO_DDRIOEXT_REG 0x0D032F84



/* GDCRRXOFFSETCOMPN0AECC1_MCIO_DDRIOEXT_REG supported on:                    */
/*       IVT_EP (0x2018DF88)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0AECC1_MCIO_DDRIOEXT_REG 0x0D032F88



/* GDCRRXOFFSETCOMPN0BECC1_MCIO_DDRIOEXT_REG supported on:                    */
/*       IVT_EP (0x2018DF8C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN0BECC1_MCIO_DDRIOEXT_REG 0x0D032F8C



/* GDCRRXOFFSETCOMPN1AECC1_MCIO_DDRIOEXT_REG supported on:                    */
/*       IVT_EP (0x2018DF90)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1AECC1_MCIO_DDRIOEXT_REG 0x0D032F90



/* GDCRRXOFFSETCOMPN1BECC1_MCIO_DDRIOEXT_REG supported on:                    */
/*       IVT_EP (0x2018DF94)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPN1BECC1_MCIO_DDRIOEXT_REG 0x0D032F94



/* GDCRRXOFFSETCOMPCFGECC1_MCIO_DDRIOEXT_REG supported on:                    */
/*       IVT_EP (0x2018DF98)                                                  */
/* Register default value:              0x0012                                */
#define GDCRRXOFFSETCOMPCFGECC1_MCIO_DDRIOEXT_REG 0x0D032F98



/* GDCRRXOFFSETCOMPSTATUSECC1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DF9C)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXOFFSETCOMPSTATUSECC1_MCIO_DDRIOEXT_REG 0x0D032F9C



/* GDCRTXDESKEWGROUP0LANE01ECC1_MCIO_DDRIOEXT_REG supported on:               */
/*       IVT_EP (0x2018DFA0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE01ECC1_MCIO_DDRIOEXT_REG 0x0D032FA0



/* GDCRTXDESKEWGROUP0LANE45ECC1_MCIO_DDRIOEXT_REG supported on:               */
/*       IVT_EP (0x2018DFA4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE45ECC1_MCIO_DDRIOEXT_REG 0x0D032FA4



/* GDCRRXDESKEWGROUP0LANE01ECC1_MCIO_DDRIOEXT_REG supported on:               */
/*       IVT_EP (0x2018DFA8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE01ECC1_MCIO_DDRIOEXT_REG 0x0D032FA8



/* GDCRRXDESKEWGROUP0LANE45ECC1_MCIO_DDRIOEXT_REG supported on:               */
/*       IVT_EP (0x2018DFAC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE45ECC1_MCIO_DDRIOEXT_REG 0x0D032FAC



/* GDCRTXDESKEWGROUP0LANE23ECC1_MCIO_DDRIOEXT_REG supported on:               */
/*       IVT_EP (0x2018DFB0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE23ECC1_MCIO_DDRIOEXT_REG 0x0D032FB0



/* GDCRTXDESKEWGROUP0LANE67ECC1_MCIO_DDRIOEXT_REG supported on:               */
/*       IVT_EP (0x2018DFB4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP0LANE67ECC1_MCIO_DDRIOEXT_REG 0x0D032FB4



/* GDCRRXDESKEWGROUP0LANE23ECC1_MCIO_DDRIOEXT_REG supported on:               */
/*       IVT_EP (0x2018DFB8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE23ECC1_MCIO_DDRIOEXT_REG 0x0D032FB8



/* GDCRRXDESKEWGROUP0LANE67ECC1_MCIO_DDRIOEXT_REG supported on:               */
/*       IVT_EP (0x2018DFBC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP0LANE67ECC1_MCIO_DDRIOEXT_REG 0x0D032FBC



/* GDCRTXDESKEWGROUP1NIB0ECC1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DFC0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB0ECC1_MCIO_DDRIOEXT_REG 0x0D032FC0



/* GDCRTXDESKEWGROUP1NIB1ECC1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DFC4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP1NIB1ECC1_MCIO_DDRIOEXT_REG 0x0D032FC4



/* GDCRRXDESKEWGROUP1NIB0ECC1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DFC8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB0ECC1_MCIO_DDRIOEXT_REG 0x0D032FC8



/* GDCRRXDESKEWGROUP1NIB1ECC1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DFCC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP1NIB1ECC1_MCIO_DDRIOEXT_REG 0x0D032FCC



/* GDCRTXDESKEWGROUP2NIB0ECC1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DFD0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB0ECC1_MCIO_DDRIOEXT_REG 0x0D032FD0



/* GDCRTXDESKEWGROUP2NIB1ECC1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DFD4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP2NIB1ECC1_MCIO_DDRIOEXT_REG 0x0D032FD4



/* GDCRRXDESKEWGROUP2NIB0ECC1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DFD8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB0ECC1_MCIO_DDRIOEXT_REG 0x0D032FD8



/* GDCRRXDESKEWGROUP2NIB1ECC1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DFDC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP2NIB1ECC1_MCIO_DDRIOEXT_REG 0x0D032FDC



/* GDCRTXDESKEWGROUP3NIB0ECC1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DFE0)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB0ECC1_MCIO_DDRIOEXT_REG 0x0D032FE0



/* GDCRTXDESKEWGROUP3NIB1ECC1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DFE4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRTXDESKEWGROUP3NIB1ECC1_MCIO_DDRIOEXT_REG 0x0D032FE4



/* GDCRRXDESKEWGROUP3NIB0ECC1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DFE8)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB0ECC1_MCIO_DDRIOEXT_REG 0x0D032FE8



/* GDCRRXDESKEWGROUP3NIB1ECC1_MCIO_DDRIOEXT_REG supported on:                 */
/*       IVT_EP (0x2018DFEC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRRXDESKEWGROUP3NIB1ECC1_MCIO_DDRIOEXT_REG 0x0D032FEC







/* GDCRXTALKEQATTKRECC1_MCIO_DDRIOEXT_REG supported on:                       */
/*       IVT_EP (0x2018DFF8)                                                  */
/* Register default value:              0xFFFF                                */
#define GDCRXTALKEQATTKRECC1_MCIO_DDRIOEXT_REG 0x0D032FF8



/* GDCRDLLCFGECC1_MCIO_DDRIOEXT_REG supported on:                             */
/*       IVT_EP (0x2018DFFC)                                                  */
/* Register default value:              0x0000                                */
#define GDCRDLLCFGECC1_MCIO_DDRIOEXT_REG 0x0D032FFC











#endif /* MCIO_DDRIOEXT_h */
