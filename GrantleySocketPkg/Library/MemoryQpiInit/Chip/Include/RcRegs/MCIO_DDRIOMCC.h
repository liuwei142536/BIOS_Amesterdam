/* Date Stamp: 8/23/2014 */

#ifndef MCIO_DDRIOMCC_h
#define MCIO_DDRIOMCC_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* MCIO_DDRIOMCC_IVT_DEV 17                                                   */
/* MCIO_DDRIOMCC_IVT_FUN 6                                                    */
/* For HSX_HOST:                                                              */
/* MCIO_DDRIOMCC_HSX_DEV 19                                                   */
/* MCIO_DDRIOMCC_HSX_FUN 6                                                    */
/* For BDX_HOST:                                                              */
/* MCIO_DDRIOMCC_BDX_DEV 19                                                   */
/* MCIO_DDRIOMCC_BDX_FUN 6                                                    */

/* VID_MCIO_DDRIOMCC_REG supported on:                                        */
/*       IVT_EP (0x2018E000)                                                  */
/*       HSX (0x2019E000)                                                     */
/*       BDX (0x2019E000)                                                     */
/* Register default value:              0x8086                                */
#define VID_MCIO_DDRIOMCC_REG 0x0D042000
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} VID_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */


/* DID_MCIO_DDRIOMCC_REG supported on:                                        */
/*       IVT_EP (0x2018E002)                                                  */
/*       HSX (0x2019E002)                                                     */
/*       BDX (0x2019E002)                                                     */
/* Register default value on IVT_EP:    0x0EBE                                */
/* Register default value on HSX:       0x2FAE                                */
/* Register default value on BDX:       0x6FAE                                */
#define DID_MCIO_DDRIOMCC_REG 0x0D042002
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h6FAE 
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
} DID_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */


/* PCICMD_MCIO_DDRIOMCC_REG supported on:                                     */
/*       IVT_EP (0x2018E004)                                                  */
/*       HSX (0x2019E004)                                                     */
/*       BDX (0x2019E004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_MCIO_DDRIOMCC_REG 0x0D042004
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} PCICMD_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */


/* PCISTS_MCIO_DDRIOMCC_REG supported on:                                     */
/*       IVT_EP (0x2018E006)                                                  */
/*       HSX (0x2019E006)                                                     */
/*       BDX (0x2019E006)                                                     */
/* Register default value on IVT_EP:    0x0010                                */
/* Register default value on HSX:       0x0000                                */
/* Register default value on BDX:       0x0000                                */
#define PCISTS_MCIO_DDRIOMCC_REG 0x0D042006

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} PCISTS_MCIO_DDRIOMCC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_MCIO_DDRIOMCC_REG supported on:                                        */
/*       IVT_EP (0x1018E008)                                                  */
/*       HSX (0x1019E008)                                                     */
/*       BDX (0x1019E008)                                                     */
/* Register default value:              0x00                                  */
#define RID_MCIO_DDRIOMCC_REG 0x0D041008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RID_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_MCIO_DDRIOMCC_REG supported on:                                     */
/*       IVT_EP (0x1018E009)                                                  */
/*       HSX (0x1019E009)                                                     */
/*       BDX (0x1019E009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_MCIO_DDRIOMCC_REG 0x0D041009

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_MCIO_DDRIOMCC_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_MCIO_DDRIOMCC_REG supported on:                                     */
/*       IVT_EP (0x2018E00A)                                                  */
/*       HSX (0x2019E00A)                                                     */
/*       BDX (0x2019E00A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_MCIO_DDRIOMCC_REG 0x0D04200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} CCR_N1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */


/* CLSR_MCIO_DDRIOMCC_REG supported on:                                       */
/*       IVT_EP (0x1018E00C)                                                  */
/*       HSX (0x1019E00C)                                                     */
/*       BDX (0x1019E00C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_MCIO_DDRIOMCC_REG 0x0D04100C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} CLSR_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */


/* PLAT_MCIO_DDRIOMCC_REG supported on:                                       */
/*       IVT_EP (0x1018E00D)                                                  */
/*       HSX (0x1019E00D)                                                     */
/*       BDX (0x1019E00D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_MCIO_DDRIOMCC_REG 0x0D04100D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} PLAT_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */


/* HDR_MCIO_DDRIOMCC_REG supported on:                                        */
/*       IVT_EP (0x1018E00E)                                                  */
/*       HSX (0x1019E00E)                                                     */
/*       BDX (0x1019E00E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_MCIO_DDRIOMCC_REG 0x0D04100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} HDR_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */


/* BIST_MCIO_DDRIOMCC_REG supported on:                                       */
/*       IVT_EP (0x1018E00F)                                                  */
/*       HSX (0x1019E00F)                                                     */
/*       BDX (0x1019E00F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_MCIO_DDRIOMCC_REG 0x0D04100F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} BIST_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_MCIO_DDRIOMCC_REG supported on:                                     */
/*       IVT_EP (0x1018E034)                                                  */
/*       HSX (0x1019E034)                                                     */
/*       BDX (0x1019E034)                                                     */
/* Register default value on IVT_EP:    0x40                                  */
/* Register default value on HSX:       0x00                                  */
/* Register default value on BDX:       0x00                                  */
#define CAPPTR_MCIO_DDRIOMCC_REG 0x0D041034
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} CAPPTR_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */


/* INTL_MCIO_DDRIOMCC_REG supported on:                                       */
/*       IVT_EP (0x1018E03C)                                                  */
/*       HSX (0x1019E03C)                                                     */
/*       BDX (0x1019E03C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_MCIO_DDRIOMCC_REG 0x0D04103C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} INTL_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */


/* INTPIN_MCIO_DDRIOMCC_REG supported on:                                     */
/*       IVT_EP (0x1018E03D)                                                  */
/*       HSX (0x1019E03D)                                                     */
/*       BDX (0x1019E03D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_MCIO_DDRIOMCC_REG 0x0D04103D
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} INTPIN_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */


/* MINGNT_MCIO_DDRIOMCC_REG supported on:                                     */
/*       IVT_EP (0x1018E03E)                                                  */
/*       HSX (0x1019E03E)                                                     */
/*       BDX (0x1019E03E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_MCIO_DDRIOMCC_REG 0x0D04103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} MINGNT_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_MCIO_DDRIOMCC_REG supported on:                                     */
/*       IVT_EP (0x1018E03F)                                                  */
/*       HSX (0x1019E03F)                                                     */
/*       BDX (0x1019E03F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_MCIO_DDRIOMCC_REG 0x0D04103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} MAXLAT_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */


/* RXGROUP0N0RANK0_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E200)                                                     */
/*       BDX (0x4019E200)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK0_CH0_MCIO_DDRIOMCC_REG 0x0D044200

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N0RANK0_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK1_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E204)                                                     */
/*       BDX (0x4019E204)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK1_CH0_MCIO_DDRIOMCC_REG 0x0D044204

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N0RANK1_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK2_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E208)                                                     */
/*       BDX (0x4019E208)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK2_CH0_MCIO_DDRIOMCC_REG 0x0D044208

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N0RANK2_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK3_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E20C)                                                     */
/*       BDX (0x4019E20C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK3_CH0_MCIO_DDRIOMCC_REG 0x0D04420C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N0RANK3_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK4_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E210)                                                     */
/*       BDX (0x4019E210)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK4_CH0_MCIO_DDRIOMCC_REG 0x0D044210

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N0RANK4_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK5_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E214)                                                     */
/*       BDX (0x4019E214)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK5_CH0_MCIO_DDRIOMCC_REG 0x0D044214

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N0RANK5_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK6_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E218)                                                     */
/*       BDX (0x4019E218)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK6_CH0_MCIO_DDRIOMCC_REG 0x0D044218

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N0RANK6_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK7_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E21C)                                                     */
/*       BDX (0x4019E21C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK7_CH0_MCIO_DDRIOMCC_REG 0x0D04421C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N0RANK7_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK0_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E220)                                                     */
/*       BDX (0x4019E220)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK0_CH0_MCIO_DDRIOMCC_REG 0x0D044220

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N0RANK0_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK1_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E224)                                                     */
/*       BDX (0x4019E224)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK1_CH0_MCIO_DDRIOMCC_REG 0x0D044224

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N0RANK1_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK2_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E228)                                                     */
/*       BDX (0x4019E228)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK2_CH0_MCIO_DDRIOMCC_REG 0x0D044228

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N0RANK2_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK3_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E22C)                                                     */
/*       BDX (0x4019E22C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK3_CH0_MCIO_DDRIOMCC_REG 0x0D04422C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N0RANK3_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK4_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E230)                                                     */
/*       BDX (0x4019E230)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK4_CH0_MCIO_DDRIOMCC_REG 0x0D044230

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N0RANK4_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK5_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E234)                                                     */
/*       BDX (0x4019E234)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK5_CH0_MCIO_DDRIOMCC_REG 0x0D044234

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N0RANK5_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK6_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E238)                                                     */
/*       BDX (0x4019E238)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK6_CH0_MCIO_DDRIOMCC_REG 0x0D044238

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N0RANK6_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK7_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E23C)                                                     */
/*       BDX (0x4019E23C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK7_CH0_MCIO_DDRIOMCC_REG 0x0D04423C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N0RANK7_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP0N0RANK0_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E240)                                                     */
/*       BDX (0x4019E240)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK0_CH0_MCIO_DDRIOMCC_REG 0x0D044240
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK0_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK0_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK1_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E244)                                                     */
/*       BDX (0x4019E244)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK1_CH0_MCIO_DDRIOMCC_REG 0x0D044244
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK1_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK1_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK2_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E248)                                                     */
/*       BDX (0x4019E248)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK2_CH0_MCIO_DDRIOMCC_REG 0x0D044248
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK2_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK2_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK3_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E24C)                                                     */
/*       BDX (0x4019E24C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK3_CH0_MCIO_DDRIOMCC_REG 0x0D04424C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK3_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK3_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK4_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E250)                                                     */
/*       BDX (0x4019E250)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK4_CH0_MCIO_DDRIOMCC_REG 0x0D044250
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK4_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK4_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK5_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E254)                                                     */
/*       BDX (0x4019E254)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK5_CH0_MCIO_DDRIOMCC_REG 0x0D044254
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK5_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK5_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK6_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E258)                                                     */
/*       BDX (0x4019E258)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK6_CH0_MCIO_DDRIOMCC_REG 0x0D044258
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK6_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK6_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK7_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E25C)                                                     */
/*       BDX (0x4019E25C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK7_CH0_MCIO_DDRIOMCC_REG 0x0D04425C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK7_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK7_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP1N0RANK0_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E260)                                                     */
/*       BDX (0x4019E260)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK0_CH0_MCIO_DDRIOMCC_REG 0x0D044260

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N0RANK0_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK1_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E264)                                                     */
/*       BDX (0x4019E264)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK1_CH0_MCIO_DDRIOMCC_REG 0x0D044264

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N0RANK1_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK2_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E268)                                                     */
/*       BDX (0x4019E268)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK2_CH0_MCIO_DDRIOMCC_REG 0x0D044268

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N0RANK2_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK3_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E26C)                                                     */
/*       BDX (0x4019E26C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK3_CH0_MCIO_DDRIOMCC_REG 0x0D04426C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N0RANK3_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK4_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E270)                                                     */
/*       BDX (0x4019E270)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK4_CH0_MCIO_DDRIOMCC_REG 0x0D044270

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N0RANK4_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK5_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E274)                                                     */
/*       BDX (0x4019E274)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK5_CH0_MCIO_DDRIOMCC_REG 0x0D044274

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N0RANK5_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK6_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E278)                                                     */
/*       BDX (0x4019E278)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK6_CH0_MCIO_DDRIOMCC_REG 0x0D044278

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N0RANK6_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK7_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E27C)                                                     */
/*       BDX (0x4019E27C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK7_CH0_MCIO_DDRIOMCC_REG 0x0D04427C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N0RANK7_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK0_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E280)                                                     */
/*       BDX (0x4019E280)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK0_CH0_MCIO_DDRIOMCC_REG 0x0D044280

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN0RANK0_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK1_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E284)                                                     */
/*       BDX (0x4019E284)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK1_CH0_MCIO_DDRIOMCC_REG 0x0D044284

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN0RANK1_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK2_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E288)                                                     */
/*       BDX (0x4019E288)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK2_CH0_MCIO_DDRIOMCC_REG 0x0D044288

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN0RANK2_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK3_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E28C)                                                     */
/*       BDX (0x4019E28C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK3_CH0_MCIO_DDRIOMCC_REG 0x0D04428C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN0RANK3_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK4_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E290)                                                     */
/*       BDX (0x4019E290)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK4_CH0_MCIO_DDRIOMCC_REG 0x0D044290

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN0RANK4_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK5_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E294)                                                     */
/*       BDX (0x4019E294)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK5_CH0_MCIO_DDRIOMCC_REG 0x0D044294

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN0RANK5_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK6_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E298)                                                     */
/*       BDX (0x4019E298)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK6_CH0_MCIO_DDRIOMCC_REG 0x0D044298

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN0RANK6_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK7_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E29C)                                                     */
/*       BDX (0x4019E29C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK7_CH0_MCIO_DDRIOMCC_REG 0x0D04429C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN0RANK7_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* RXVREFCTRLN0_CH0_MCIO_DDRIOMCC_REG supported on:                           */
/*       HSX (0x4019E2A4)                                                     */
/*       BDX (0x4019E2A4)                                                     */
/* Register default value:              0x00000000                            */
#define RXVREFCTRLN0_CH0_MCIO_DDRIOMCC_REG 0x0D0442A4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXVREFCTRLN0_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATATRAINFEEDBACKN0_CH0_MCIO_DDRIOMCC_REG supported on:                    */
/*       HSX (0x4019E2A8)                                                     */
/*       BDX (0x4019E2A8)                                                     */
/* Register default value:              0x00000000                            */
#define DATATRAINFEEDBACKN0_CH0_MCIO_DDRIOMCC_REG 0x0D0442A8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATATRAINFEEDBACKN0_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATACONTROL4_CH0_MCIO_DDRIOMCC_REG supported on:                           */
/*       HSX (0x4019E2AC)                                                     */
/*       BDX (0x4019E2AC)                                                     */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       <could not determine>                 */
#define DATACONTROL4_CH0_MCIO_DDRIOMCC_REG 0x0D0442AC
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} DATACONTROL4_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATACONTROL4_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* VSSHIORVREFCONTROL1_CH0_MCIO_DDRIOMCC_REG supported on:                    */
/*       BDX (0x4019E2CC)                                                     */
/* Register default value:              0x00000000                            */
#define VSSHIORVREFCONTROL1_CH0_MCIO_DDRIOMCC_REG 0x0D0442CC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} VSSHIORVREFCONTROL1_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* PXCATTACK1SEL_CH0_MCIO_DDRIOMCC_REG supported on:                          */
/*       BDX (0x4019E2D0)                                                     */
/* Register default value:              0x00FAC688                            */
#define PXCATTACK1SEL_CH0_MCIO_DDRIOMCC_REG 0x0D0442D0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} PXCATTACK1SEL_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */






/* DATAOFFSETTRAIN_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E2DC)                                                     */
/*       BDX (0x4019E2DC)                                                     */
/* Register default value:              0x00000000                            */
#define DATAOFFSETTRAIN_CH0_MCIO_DDRIOMCC_REG 0x0D0442DC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATAOFFSETTRAIN_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* PXCATTACK0SEL_CH0_MCIO_DDRIOMCC_REG supported on:                          */
/*       BDX (0x4019E2E4)                                                     */
/* Register default value:              0x00FAC688                            */
#define PXCATTACK0SEL_CH0_MCIO_DDRIOMCC_REG 0x0D0442E4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} PXCATTACK0SEL_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATACONTROL0_CH0_MCIO_DDRIOMCC_REG supported on:                           */
/*       HSX (0x4019E2E8)                                                     */
/*       BDX (0x4019E2E8)                                                     */
/* Register default value:              0x00000000                            */
#define DATACONTROL0_CH0_MCIO_DDRIOMCC_REG 0x0D0442E8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATACONTROL0_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATACONTROL1_CH0_MCIO_DDRIOMCC_REG supported on:                           */
/*       HSX (0x4019E2EC)                                                     */
/*       BDX (0x4019E2EC)                                                     */
/* Register default value:              0x00000000                            */
#define DATACONTROL1_CH0_MCIO_DDRIOMCC_REG 0x0D0442EC
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} DATACONTROL1_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATACONTROL1_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DATACONTROL2_CH0_MCIO_DDRIOMCC_REG supported on:                           */
/*       HSX (0x4019E2F0)                                                     */
/*       BDX (0x4019E2F0)                                                     */
/* Register default value:              0x00000000                            */
#define DATACONTROL2_CH0_MCIO_DDRIOMCC_REG 0x0D0442F0
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} DATACONTROL2_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATACONTROL2_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DATACONTROL3_CH0_MCIO_DDRIOMCC_REG supported on:                           */
/*       HSX (0x4019E2F4)                                                     */
/*       BDX (0x4019E2F4)                                                     */
/* Register default value:              0x8184924924F                         */
#define DATACONTROL3_CH0_MCIO_DDRIOMCC_REG 0x0D0442F4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATACONTROL3_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* VSSHIORVREFCONTROL_CH0_MCIO_DDRIOMCC_REG supported on:                     */
/*       HSX (0x4019E2F8)                                                     */
/*       BDX (0x4019E2F8)                                                     */
/* Register default value:              0x004D8238                            */
#define VSSHIORVREFCONTROL_CH0_MCIO_DDRIOMCC_REG 0x0D0442F8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} VSSHIORVREFCONTROL_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* RXGROUP0N1RANK0_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E300)                                                     */
/*       BDX (0x4019E300)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK0_CH0_MCIO_DDRIOMCC_REG 0x0D044300

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N1RANK0_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK1_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E304)                                                     */
/*       BDX (0x4019E304)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK1_CH0_MCIO_DDRIOMCC_REG 0x0D044304

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N1RANK1_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK2_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E308)                                                     */
/*       BDX (0x4019E308)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK2_CH0_MCIO_DDRIOMCC_REG 0x0D044308

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N1RANK2_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK3_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E30C)                                                     */
/*       BDX (0x4019E30C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK3_CH0_MCIO_DDRIOMCC_REG 0x0D04430C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N1RANK3_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK4_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E310)                                                     */
/*       BDX (0x4019E310)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK4_CH0_MCIO_DDRIOMCC_REG 0x0D044310

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N1RANK4_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK5_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E314)                                                     */
/*       BDX (0x4019E314)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK5_CH0_MCIO_DDRIOMCC_REG 0x0D044314

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N1RANK5_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK6_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E318)                                                     */
/*       BDX (0x4019E318)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK6_CH0_MCIO_DDRIOMCC_REG 0x0D044318

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N1RANK6_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK7_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E31C)                                                     */
/*       BDX (0x4019E31C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK7_CH0_MCIO_DDRIOMCC_REG 0x0D04431C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N1RANK7_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK0_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E320)                                                     */
/*       BDX (0x4019E320)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK0_CH0_MCIO_DDRIOMCC_REG 0x0D044320

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N1RANK0_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK1_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E324)                                                     */
/*       BDX (0x4019E324)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK1_CH0_MCIO_DDRIOMCC_REG 0x0D044324

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N1RANK1_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK2_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E328)                                                     */
/*       BDX (0x4019E328)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK2_CH0_MCIO_DDRIOMCC_REG 0x0D044328

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N1RANK2_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK3_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E32C)                                                     */
/*       BDX (0x4019E32C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK3_CH0_MCIO_DDRIOMCC_REG 0x0D04432C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N1RANK3_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK4_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E330)                                                     */
/*       BDX (0x4019E330)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK4_CH0_MCIO_DDRIOMCC_REG 0x0D044330

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N1RANK4_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK5_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E334)                                                     */
/*       BDX (0x4019E334)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK5_CH0_MCIO_DDRIOMCC_REG 0x0D044334

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N1RANK5_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK6_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E338)                                                     */
/*       BDX (0x4019E338)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK6_CH0_MCIO_DDRIOMCC_REG 0x0D044338

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N1RANK6_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK7_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E33C)                                                     */
/*       BDX (0x4019E33C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK7_CH0_MCIO_DDRIOMCC_REG 0x0D04433C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N1RANK7_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP0N1RANK0_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E340)                                                     */
/*       BDX (0x4019E340)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK0_CH0_MCIO_DDRIOMCC_REG 0x0D044340
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK0_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK0_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK1_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E344)                                                     */
/*       BDX (0x4019E344)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK1_CH0_MCIO_DDRIOMCC_REG 0x0D044344
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK1_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK1_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK2_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E348)                                                     */
/*       BDX (0x4019E348)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK2_CH0_MCIO_DDRIOMCC_REG 0x0D044348
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK2_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK2_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK3_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E34C)                                                     */
/*       BDX (0x4019E34C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK3_CH0_MCIO_DDRIOMCC_REG 0x0D04434C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK3_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK3_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK4_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E350)                                                     */
/*       BDX (0x4019E350)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK4_CH0_MCIO_DDRIOMCC_REG 0x0D044350
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK4_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK4_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK5_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E354)                                                     */
/*       BDX (0x4019E354)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK5_CH0_MCIO_DDRIOMCC_REG 0x0D044354
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK5_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK5_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK6_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E358)                                                     */
/*       BDX (0x4019E358)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK6_CH0_MCIO_DDRIOMCC_REG 0x0D044358
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK6_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK6_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK7_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E35C)                                                     */
/*       BDX (0x4019E35C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK7_CH0_MCIO_DDRIOMCC_REG 0x0D04435C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK7_CH0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK7_CH0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP1N1RANK0_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E360)                                                     */
/*       BDX (0x4019E360)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK0_CH0_MCIO_DDRIOMCC_REG 0x0D044360

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N1RANK0_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK1_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E364)                                                     */
/*       BDX (0x4019E364)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK1_CH0_MCIO_DDRIOMCC_REG 0x0D044364

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N1RANK1_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK2_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E368)                                                     */
/*       BDX (0x4019E368)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK2_CH0_MCIO_DDRIOMCC_REG 0x0D044368

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N1RANK2_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK3_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E36C)                                                     */
/*       BDX (0x4019E36C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK3_CH0_MCIO_DDRIOMCC_REG 0x0D04436C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N1RANK3_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK4_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E370)                                                     */
/*       BDX (0x4019E370)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK4_CH0_MCIO_DDRIOMCC_REG 0x0D044370

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N1RANK4_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK5_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E374)                                                     */
/*       BDX (0x4019E374)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK5_CH0_MCIO_DDRIOMCC_REG 0x0D044374

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N1RANK5_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK6_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E378)                                                     */
/*       BDX (0x4019E378)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK6_CH0_MCIO_DDRIOMCC_REG 0x0D044378

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N1RANK6_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK7_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E37C)                                                     */
/*       BDX (0x4019E37C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK7_CH0_MCIO_DDRIOMCC_REG 0x0D04437C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N1RANK7_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK0_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E380)                                                     */
/*       BDX (0x4019E380)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK0_CH0_MCIO_DDRIOMCC_REG 0x0D044380

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN1RANK0_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK1_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E384)                                                     */
/*       BDX (0x4019E384)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK1_CH0_MCIO_DDRIOMCC_REG 0x0D044384

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN1RANK1_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK2_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E388)                                                     */
/*       BDX (0x4019E388)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK2_CH0_MCIO_DDRIOMCC_REG 0x0D044388

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN1RANK2_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK3_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E38C)                                                     */
/*       BDX (0x4019E38C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK3_CH0_MCIO_DDRIOMCC_REG 0x0D04438C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN1RANK3_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK4_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E390)                                                     */
/*       BDX (0x4019E390)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK4_CH0_MCIO_DDRIOMCC_REG 0x0D044390

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN1RANK4_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK5_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E394)                                                     */
/*       BDX (0x4019E394)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK5_CH0_MCIO_DDRIOMCC_REG 0x0D044394

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN1RANK5_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK6_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E398)                                                     */
/*       BDX (0x4019E398)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK6_CH0_MCIO_DDRIOMCC_REG 0x0D044398

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN1RANK6_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK7_CH0_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019E39C)                                                     */
/*       BDX (0x4019E39C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK7_CH0_MCIO_DDRIOMCC_REG 0x0D04439C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN1RANK7_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* RXVREFCTRLN1_CH0_MCIO_DDRIOMCC_REG supported on:                           */
/*       HSX (0x4019E3A4)                                                     */
/*       BDX (0x4019E3A4)                                                     */
/* Register default value:              0x00000000                            */
#define RXVREFCTRLN1_CH0_MCIO_DDRIOMCC_REG 0x0D0443A4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXVREFCTRLN1_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATATRAINFEEDBACKN1_CH0_MCIO_DDRIOMCC_REG supported on:                    */
/*       HSX (0x4019E3A8)                                                     */
/*       BDX (0x4019E3A8)                                                     */
/* Register default value:              0x00000000                            */
#define DATATRAINFEEDBACKN1_CH0_MCIO_DDRIOMCC_REG 0x0D0443A8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATATRAINFEEDBACKN1_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATATCOCOMPOFFSET0_CH1_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3AC)                                                     */
/* Register default value:              0x00000000                            */
#define DATATCOCOMPOFFSET0_CH1_MCIO_DDRIOMCC_REG 0x0D0443AC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATATCOCOMPOFFSET0_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATATCOCOMPOFFSET0_CH0_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3AC)                                                     */
/* Register default value:              0x00000000                            */
#define DATATCOCOMPOFFSET0_CH0_MCIO_DDRIOMCC_REG 0x0D0443AC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATATCOCOMPOFFSET0_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATATCOCOMPOFFSET1_CH0_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3B0)                                                     */
/* Register default value:              0x00000000                            */
#define DATATCOCOMPOFFSET1_CH0_MCIO_DDRIOMCC_REG 0x0D0443B0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATATCOCOMPOFFSET1_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATATCOCOMPOFFSET1_CH1_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3B0)                                                     */
/* Register default value:              0x00000000                            */
#define DATATCOCOMPOFFSET1_CH1_MCIO_DDRIOMCC_REG 0x0D0443B0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATATCOCOMPOFFSET1_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET0_CH0_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3B4)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET0_CH0_MCIO_DDRIOMCC_REG 0x0D0443B4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATAODTCOMPOFFSET0_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET0_CH1_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3B4)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET0_CH1_MCIO_DDRIOMCC_REG 0x0D0443B4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATAODTCOMPOFFSET0_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET1_CH0_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3B8)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET1_CH0_MCIO_DDRIOMCC_REG 0x0D0443B8

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATAODTCOMPOFFSET1_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET1_CH1_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3B8)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET1_CH1_MCIO_DDRIOMCC_REG 0x0D0443B8

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATAODTCOMPOFFSET1_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET2_CH0_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3BC)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET2_CH0_MCIO_DDRIOMCC_REG 0x0D0443BC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATAODTCOMPOFFSET2_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET2_CH1_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3BC)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET2_CH1_MCIO_DDRIOMCC_REG 0x0D0443BC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATAODTCOMPOFFSET2_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET3_CH0_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3C0)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET3_CH0_MCIO_DDRIOMCC_REG 0x0D0443C0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATAODTCOMPOFFSET3_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATAODTCOMPOFFSET3_CH1_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3C0)                                                     */
/* Register default value:              0x00000000                            */
#define DATAODTCOMPOFFSET3_CH1_MCIO_DDRIOMCC_REG 0x0D0443C0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATAODTCOMPOFFSET3_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET0_CH1_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3C4)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET0_CH1_MCIO_DDRIOMCC_REG 0x0D0443C4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATARONCOMPOFFSET0_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET0_CH0_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3C4)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET0_CH0_MCIO_DDRIOMCC_REG 0x0D0443C4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATARONCOMPOFFSET0_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET1_CH0_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3C8)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET1_CH0_MCIO_DDRIOMCC_REG 0x0D0443C8

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATARONCOMPOFFSET1_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET1_CH1_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3C8)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET1_CH1_MCIO_DDRIOMCC_REG 0x0D0443C8

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATARONCOMPOFFSET1_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET2_CH0_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3CC)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET2_CH0_MCIO_DDRIOMCC_REG 0x0D0443CC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATARONCOMPOFFSET2_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET2_CH1_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3CC)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET2_CH1_MCIO_DDRIOMCC_REG 0x0D0443CC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATARONCOMPOFFSET2_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET3_CH0_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3D0)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET3_CH0_MCIO_DDRIOMCC_REG 0x0D0443D0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATARONCOMPOFFSET3_CH0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATARONCOMPOFFSET3_CH1_MCIO_DDRIOMCC_REG supported on:                     */
/*       BDX (0x4019E3D0)                                                     */
/* Register default value:              0x00000000                            */
#define DATARONCOMPOFFSET3_CH1_MCIO_DDRIOMCC_REG 0x0D0443D0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATARONCOMPOFFSET3_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */






/* DDRCRCMDPICODING_CMD0_MCIO_DDRIOMCC_REG supported on:                      */
/*       HSX (0x4019E608)                                                     */
/*       BDX (0x4019E608)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDPICODING_CMD0_MCIO_DDRIOMCC_REG 0x0D044608

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCMDPICODING_CMD0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCMDCONTROLS_CMD0_MCIO_DDRIOMCC_REG supported on:                      */
/*       HSX (0x4019E60C)                                                     */
/*       BDX (0x4019E60C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDCONTROLS_CMD0_MCIO_DDRIOMCC_REG 0x0D04460C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
    UINT32 rsvd_7 : 13;
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
} DDRCRCMDCONTROLS_CMD0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
    UINT32 rsvd_7 : 13;
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
} DDRCRCMDCONTROLS_CMD0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDCONTROLS2_CMD0_MCIO_DDRIOMCC_REG supported on:                     */
/*       HSX (0x4019E610)                                                     */
/*       BDX (0x4019E610)                                                     */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00001000                            */
#define DDRCRCMDCONTROLS2_CMD0_MCIO_DDRIOMCC_REG 0x0D044610
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} DDRCRCMDCONTROLS2_CMD0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCMDCONTROLS2_CMD0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDPICODING2_CMD0_MCIO_DDRIOMCC_REG supported on:                     */
/*       HSX (0x4019E618)                                                     */
/*       BDX (0x4019E618)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDPICODING2_CMD0_MCIO_DDRIOMCC_REG 0x0D044618

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCMDPICODING2_CMD0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






/* DDRCRCMDTRAINING_CMD0_MCIO_DDRIOMCC_REG supported on:                      */
/*       HSX (0x4019E62C)                                                     */
/*       BDX (0x4019E62C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDTRAINING_CMD0_MCIO_DDRIOMCC_REG 0x0D04462C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCMDTRAINING_CMD0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCMDCONTROLS1_CMD0_MCIO_DDRIOMCC_REG supported on:                     */
/*       HSX (0x4019E630)                                                     */
/*       BDX (0x4019E630)                                                     */
/* Register default value on HSX:       0x00980000                            */
/* Register default value on BDX:       0x00180000                            */
#define DDRCRCMDCONTROLS1_CMD0_MCIO_DDRIOMCC_REG 0x0D044630
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} DDRCRCMDCONTROLS1_CMD0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCMDCONTROLS1_CMD0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */







/* DDRCRCTLPICODING_CKE0_MCIO_DDRIOMCC_REG supported on:                      */
/*       HSX (0x4019E818)                                                     */
/*       BDX (0x4019E818)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING_CKE0_MCIO_DDRIOMCC_REG 0x0D044818

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCTLPICODING_CKE0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCTLCONTROLS_CKE0_MCIO_DDRIOMCC_REG supported on:                      */
/*       HSX (0x4019E81C)                                                     */
/*       BDX (0x4019E81C)                                                     */
/* Register default value:              0x10010000                            */
#define DDRCRCTLCONTROLS_CKE0_MCIO_DDRIOMCC_REG 0x0D04481C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} DDRCRCTLCONTROLS_CKE0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCTLCONTROLS_CKE0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCTLRANKSUSED_CKE0_MCIO_DDRIOMCC_REG supported on:                     */
/*       HSX (0x4019E820)                                                     */
/*       BDX (0x4019E820)                                                     */
/* Register default value on HSX:       0x00004FFF                            */
/* Register default value on BDX:       0xC0004FFF                            */
#define DDRCRCTLRANKSUSED_CKE0_MCIO_DDRIOMCC_REG 0x0D044820
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} DDRCRCTLRANKSUSED_CKE0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCTLRANKSUSED_CKE0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */







/* DDRCRCTLTRAINING_CKE0_MCIO_DDRIOMCC_REG supported on:                      */
/*       HSX (0x4019E82C)                                                     */
/*       BDX (0x4019E82C)                                                     */
/* Register default value:              0x00007000                            */
#define DDRCRCTLTRAINING_CKE0_MCIO_DDRIOMCC_REG 0x0D04482C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} DDRCRCTLTRAINING_CKE0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCTLTRAINING_CKE0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDCONTROLS1_CKE0_MCIO_DDRIOMCC_REG supported on:                     */
/*       HSX (0x4019E830)                                                     */
/*       BDX (0x4019E830)                                                     */
/* Register default value on HSX:       0x00980000                            */
/* Register default value on BDX:       0x00180000                            */
#define DDRCRCMDCONTROLS1_CKE0_MCIO_DDRIOMCC_REG 0x0D044830
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} DDRCRCMDCONTROLS1_CKE0_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCMDCONTROLS1_CKE0_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCTLPICODING2_CKE0_MCIO_DDRIOMCC_REG supported on:                     */
/*       HSX (0x4019E834)                                                     */
/*       BDX (0x4019E834)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING2_CKE0_MCIO_DDRIOMCC_REG 0x0D044834

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCTLPICODING2_CKE0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCTLPICODING3_CKE0_MCIO_DDRIOMCC_REG supported on:                     */
/*       HSX (0x4019E838)                                                     */
/*       BDX (0x4019E838)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING3_CKE0_MCIO_DDRIOMCC_REG 0x0D044838

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCTLPICODING3_CKE0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* DDRCRCTLPICODING4_CKE0_MCIO_DDRIOMCC_REG supported on:                     */
/*       HSX (0x4019E840)                                                     */
/*       BDX (0x4019E840)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING4_CKE0_MCIO_DDRIOMCC_REG 0x0D044840

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCTLPICODING4_CKE0_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK0_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA00)                                                     */
/*       BDX (0x4019EA00)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK0_CH1_MCIO_DDRIOMCC_REG 0x0D044A00

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N0RANK0_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK1_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA04)                                                     */
/*       BDX (0x4019EA04)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK1_CH1_MCIO_DDRIOMCC_REG 0x0D044A04

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N0RANK1_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK2_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA08)                                                     */
/*       BDX (0x4019EA08)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK2_CH1_MCIO_DDRIOMCC_REG 0x0D044A08

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N0RANK2_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK3_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA0C)                                                     */
/*       BDX (0x4019EA0C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK3_CH1_MCIO_DDRIOMCC_REG 0x0D044A0C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N0RANK3_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK4_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA10)                                                     */
/*       BDX (0x4019EA10)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK4_CH1_MCIO_DDRIOMCC_REG 0x0D044A10

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N0RANK4_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK5_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA14)                                                     */
/*       BDX (0x4019EA14)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK5_CH1_MCIO_DDRIOMCC_REG 0x0D044A14

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N0RANK5_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK6_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA18)                                                     */
/*       BDX (0x4019EA18)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK6_CH1_MCIO_DDRIOMCC_REG 0x0D044A18

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N0RANK6_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N0RANK7_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA1C)                                                     */
/*       BDX (0x4019EA1C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N0RANK7_CH1_MCIO_DDRIOMCC_REG 0x0D044A1C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N0RANK7_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK0_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA20)                                                     */
/*       BDX (0x4019EA20)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK0_CH1_MCIO_DDRIOMCC_REG 0x0D044A20

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N0RANK0_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK1_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA24)                                                     */
/*       BDX (0x4019EA24)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK1_CH1_MCIO_DDRIOMCC_REG 0x0D044A24

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N0RANK1_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK2_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA28)                                                     */
/*       BDX (0x4019EA28)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK2_CH1_MCIO_DDRIOMCC_REG 0x0D044A28

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N0RANK2_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK3_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA2C)                                                     */
/*       BDX (0x4019EA2C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK3_CH1_MCIO_DDRIOMCC_REG 0x0D044A2C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N0RANK3_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK4_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA30)                                                     */
/*       BDX (0x4019EA30)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK4_CH1_MCIO_DDRIOMCC_REG 0x0D044A30

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N0RANK4_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK5_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA34)                                                     */
/*       BDX (0x4019EA34)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK5_CH1_MCIO_DDRIOMCC_REG 0x0D044A34

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N0RANK5_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK6_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA38)                                                     */
/*       BDX (0x4019EA38)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK6_CH1_MCIO_DDRIOMCC_REG 0x0D044A38

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N0RANK6_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N0RANK7_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA3C)                                                     */
/*       BDX (0x4019EA3C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N0RANK7_CH1_MCIO_DDRIOMCC_REG 0x0D044A3C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N0RANK7_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP0N0RANK0_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA40)                                                     */
/*       BDX (0x4019EA40)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK0_CH1_MCIO_DDRIOMCC_REG 0x0D044A40
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK0_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK0_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK1_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA44)                                                     */
/*       BDX (0x4019EA44)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK1_CH1_MCIO_DDRIOMCC_REG 0x0D044A44
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK1_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK1_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK2_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA48)                                                     */
/*       BDX (0x4019EA48)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK2_CH1_MCIO_DDRIOMCC_REG 0x0D044A48
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK2_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK2_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK3_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA4C)                                                     */
/*       BDX (0x4019EA4C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK3_CH1_MCIO_DDRIOMCC_REG 0x0D044A4C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK3_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK3_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK4_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA50)                                                     */
/*       BDX (0x4019EA50)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK4_CH1_MCIO_DDRIOMCC_REG 0x0D044A50
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK4_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK4_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK5_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA54)                                                     */
/*       BDX (0x4019EA54)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK5_CH1_MCIO_DDRIOMCC_REG 0x0D044A54
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK5_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK5_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK6_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA58)                                                     */
/*       BDX (0x4019EA58)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK6_CH1_MCIO_DDRIOMCC_REG 0x0D044A58
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK6_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK6_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N0RANK7_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA5C)                                                     */
/*       BDX (0x4019EA5C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N0RANK7_CH1_MCIO_DDRIOMCC_REG 0x0D044A5C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK7_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N0RANK7_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP1N0RANK0_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA60)                                                     */
/*       BDX (0x4019EA60)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK0_CH1_MCIO_DDRIOMCC_REG 0x0D044A60

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N0RANK0_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK1_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA64)                                                     */
/*       BDX (0x4019EA64)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK1_CH1_MCIO_DDRIOMCC_REG 0x0D044A64

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N0RANK1_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK2_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA68)                                                     */
/*       BDX (0x4019EA68)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK2_CH1_MCIO_DDRIOMCC_REG 0x0D044A68

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N0RANK2_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK3_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA6C)                                                     */
/*       BDX (0x4019EA6C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK3_CH1_MCIO_DDRIOMCC_REG 0x0D044A6C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N0RANK3_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK4_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA70)                                                     */
/*       BDX (0x4019EA70)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK4_CH1_MCIO_DDRIOMCC_REG 0x0D044A70

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N0RANK4_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK5_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA74)                                                     */
/*       BDX (0x4019EA74)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK5_CH1_MCIO_DDRIOMCC_REG 0x0D044A74

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N0RANK5_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK6_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA78)                                                     */
/*       BDX (0x4019EA78)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK6_CH1_MCIO_DDRIOMCC_REG 0x0D044A78

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N0RANK6_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N0RANK7_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA7C)                                                     */
/*       BDX (0x4019EA7C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N0RANK7_CH1_MCIO_DDRIOMCC_REG 0x0D044A7C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N0RANK7_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK0_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA80)                                                     */
/*       BDX (0x4019EA80)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK0_CH1_MCIO_DDRIOMCC_REG 0x0D044A80

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN0RANK0_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK1_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA84)                                                     */
/*       BDX (0x4019EA84)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK1_CH1_MCIO_DDRIOMCC_REG 0x0D044A84

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN0RANK1_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK2_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA88)                                                     */
/*       BDX (0x4019EA88)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK2_CH1_MCIO_DDRIOMCC_REG 0x0D044A88

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN0RANK2_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK3_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA8C)                                                     */
/*       BDX (0x4019EA8C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK3_CH1_MCIO_DDRIOMCC_REG 0x0D044A8C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN0RANK3_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK4_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA90)                                                     */
/*       BDX (0x4019EA90)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK4_CH1_MCIO_DDRIOMCC_REG 0x0D044A90

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN0RANK4_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK5_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA94)                                                     */
/*       BDX (0x4019EA94)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK5_CH1_MCIO_DDRIOMCC_REG 0x0D044A94

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN0RANK5_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK6_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA98)                                                     */
/*       BDX (0x4019EA98)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK6_CH1_MCIO_DDRIOMCC_REG 0x0D044A98

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN0RANK6_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN0RANK7_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EA9C)                                                     */
/*       BDX (0x4019EA9C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN0RANK7_CH1_MCIO_DDRIOMCC_REG 0x0D044A9C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN0RANK7_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* RXVREFCTRLN0_CH1_MCIO_DDRIOMCC_REG supported on:                           */
/*       HSX (0x4019EAA4)                                                     */
/*       BDX (0x4019EAA4)                                                     */
/* Register default value:              0x00000000                            */
#define RXVREFCTRLN0_CH1_MCIO_DDRIOMCC_REG 0x0D044AA4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXVREFCTRLN0_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATATRAINFEEDBACKN0_CH1_MCIO_DDRIOMCC_REG supported on:                    */
/*       HSX (0x4019EAA8)                                                     */
/*       BDX (0x4019EAA8)                                                     */
/* Register default value:              0x00000000                            */
#define DATATRAINFEEDBACKN0_CH1_MCIO_DDRIOMCC_REG 0x0D044AA8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATATRAINFEEDBACKN0_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATACONTROL4_CH1_MCIO_DDRIOMCC_REG supported on:                           */
/*       HSX (0x4019EAAC)                                                     */
/*       BDX (0x4019EAAC)                                                     */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       <could not determine>                 */
#define DATACONTROL4_CH1_MCIO_DDRIOMCC_REG 0x0D044AAC
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} DATACONTROL4_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATACONTROL4_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* VSSHIORVREFCONTROL1_CH1_MCIO_DDRIOMCC_REG supported on:                    */
/*       BDX (0x4019EACC)                                                     */
/* Register default value:              0x00000000                            */
#define VSSHIORVREFCONTROL1_CH1_MCIO_DDRIOMCC_REG 0x0D044ACC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} VSSHIORVREFCONTROL1_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* PXCATTACK1SEL_CH1_MCIO_DDRIOMCC_REG supported on:                          */
/*       BDX (0x4019EAD0)                                                     */
/* Register default value:              0x00FAC688                            */
#define PXCATTACK1SEL_CH1_MCIO_DDRIOMCC_REG 0x0D044AD0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} PXCATTACK1SEL_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */






/* DATAOFFSETTRAIN_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EADC)                                                     */
/*       BDX (0x4019EADC)                                                     */
/* Register default value:              0x00000000                            */
#define DATAOFFSETTRAIN_CH1_MCIO_DDRIOMCC_REG 0x0D044ADC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATAOFFSETTRAIN_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* PXCATTACK0SEL_CH1_MCIO_DDRIOMCC_REG supported on:                          */
/*       BDX (0x4019EAE4)                                                     */
/* Register default value:              0x00FAC688                            */
#define PXCATTACK0SEL_CH1_MCIO_DDRIOMCC_REG 0x0D044AE4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} PXCATTACK0SEL_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DATACONTROL0_CH1_MCIO_DDRIOMCC_REG supported on:                           */
/*       HSX (0x4019EAE8)                                                     */
/*       BDX (0x4019EAE8)                                                     */
/* Register default value:              0x00000000                            */
#define DATACONTROL0_CH1_MCIO_DDRIOMCC_REG 0x0D044AE8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATACONTROL0_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATACONTROL1_CH1_MCIO_DDRIOMCC_REG supported on:                           */
/*       HSX (0x4019EAEC)                                                     */
/*       BDX (0x4019EAEC)                                                     */
/* Register default value:              0x00000000                            */
#define DATACONTROL1_CH1_MCIO_DDRIOMCC_REG 0x0D044AEC
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} DATACONTROL1_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATACONTROL1_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DATACONTROL2_CH1_MCIO_DDRIOMCC_REG supported on:                           */
/*       HSX (0x4019EAF0)                                                     */
/*       BDX (0x4019EAF0)                                                     */
/* Register default value:              0x00000000                            */
#define DATACONTROL2_CH1_MCIO_DDRIOMCC_REG 0x0D044AF0
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} DATACONTROL2_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATACONTROL2_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DATACONTROL3_CH1_MCIO_DDRIOMCC_REG supported on:                           */
/*       HSX (0x4019EAF4)                                                     */
/*       BDX (0x4019EAF4)                                                     */
/* Register default value:              0x8184924924F                         */
#define DATACONTROL3_CH1_MCIO_DDRIOMCC_REG 0x0D044AF4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATACONTROL3_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* VSSHIORVREFCONTROL_CH1_MCIO_DDRIOMCC_REG supported on:                     */
/*       HSX (0x4019EAF8)                                                     */
/*       BDX (0x4019EAF8)                                                     */
/* Register default value:              0x004D8238                            */
#define VSSHIORVREFCONTROL_CH1_MCIO_DDRIOMCC_REG 0x0D044AF8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} VSSHIORVREFCONTROL_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* RXGROUP0N1RANK0_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB00)                                                     */
/*       BDX (0x4019EB00)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK0_CH1_MCIO_DDRIOMCC_REG 0x0D044B00

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N1RANK0_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK1_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB04)                                                     */
/*       BDX (0x4019EB04)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK1_CH1_MCIO_DDRIOMCC_REG 0x0D044B04

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N1RANK1_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK2_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB08)                                                     */
/*       BDX (0x4019EB08)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK2_CH1_MCIO_DDRIOMCC_REG 0x0D044B08

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N1RANK2_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK3_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB0C)                                                     */
/*       BDX (0x4019EB0C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK3_CH1_MCIO_DDRIOMCC_REG 0x0D044B0C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N1RANK3_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK4_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB10)                                                     */
/*       BDX (0x4019EB10)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK4_CH1_MCIO_DDRIOMCC_REG 0x0D044B10

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N1RANK4_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK5_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB14)                                                     */
/*       BDX (0x4019EB14)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK5_CH1_MCIO_DDRIOMCC_REG 0x0D044B14

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N1RANK5_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK6_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB18)                                                     */
/*       BDX (0x4019EB18)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK6_CH1_MCIO_DDRIOMCC_REG 0x0D044B18

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N1RANK6_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP0N1RANK7_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB1C)                                                     */
/*       BDX (0x4019EB1C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP0N1RANK7_CH1_MCIO_DDRIOMCC_REG 0x0D044B1C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP0N1RANK7_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK0_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB20)                                                     */
/*       BDX (0x4019EB20)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK0_CH1_MCIO_DDRIOMCC_REG 0x0D044B20

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N1RANK0_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK1_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB24)                                                     */
/*       BDX (0x4019EB24)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK1_CH1_MCIO_DDRIOMCC_REG 0x0D044B24

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N1RANK1_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK2_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB28)                                                     */
/*       BDX (0x4019EB28)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK2_CH1_MCIO_DDRIOMCC_REG 0x0D044B28

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N1RANK2_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK3_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB2C)                                                     */
/*       BDX (0x4019EB2C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK3_CH1_MCIO_DDRIOMCC_REG 0x0D044B2C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N1RANK3_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK4_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB30)                                                     */
/*       BDX (0x4019EB30)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK4_CH1_MCIO_DDRIOMCC_REG 0x0D044B30

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N1RANK4_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK5_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB34)                                                     */
/*       BDX (0x4019EB34)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK5_CH1_MCIO_DDRIOMCC_REG 0x0D044B34

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N1RANK5_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK6_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB38)                                                     */
/*       BDX (0x4019EB38)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK6_CH1_MCIO_DDRIOMCC_REG 0x0D044B38

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N1RANK6_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXGROUP1N1RANK7_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB3C)                                                     */
/*       BDX (0x4019EB3C)                                                     */
/* Register default value:              0x00000000                            */
#define RXGROUP1N1RANK7_CH1_MCIO_DDRIOMCC_REG 0x0D044B3C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXGROUP1N1RANK7_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP0N1RANK0_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB40)                                                     */
/*       BDX (0x4019EB40)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK0_CH1_MCIO_DDRIOMCC_REG 0x0D044B40
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK0_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK0_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK1_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB44)                                                     */
/*       BDX (0x4019EB44)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK1_CH1_MCIO_DDRIOMCC_REG 0x0D044B44
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK1_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK1_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK2_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB48)                                                     */
/*       BDX (0x4019EB48)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK2_CH1_MCIO_DDRIOMCC_REG 0x0D044B48
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK2_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK2_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK3_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB4C)                                                     */
/*       BDX (0x4019EB4C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK3_CH1_MCIO_DDRIOMCC_REG 0x0D044B4C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK3_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK3_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK4_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB50)                                                     */
/*       BDX (0x4019EB50)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK4_CH1_MCIO_DDRIOMCC_REG 0x0D044B50
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK4_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK4_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK5_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB54)                                                     */
/*       BDX (0x4019EB54)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK5_CH1_MCIO_DDRIOMCC_REG 0x0D044B54
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK5_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK5_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK6_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB58)                                                     */
/*       BDX (0x4019EB58)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK6_CH1_MCIO_DDRIOMCC_REG 0x0D044B58
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK6_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK6_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP0N1RANK7_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB5C)                                                     */
/*       BDX (0x4019EB5C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP0N1RANK7_CH1_MCIO_DDRIOMCC_REG 0x0D044B5C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK7_CH1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP0N1RANK7_CH1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* TXGROUP1N1RANK0_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB60)                                                     */
/*       BDX (0x4019EB60)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK0_CH1_MCIO_DDRIOMCC_REG 0x0D044B60

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N1RANK0_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK1_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB64)                                                     */
/*       BDX (0x4019EB64)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK1_CH1_MCIO_DDRIOMCC_REG 0x0D044B64

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N1RANK1_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK2_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB68)                                                     */
/*       BDX (0x4019EB68)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK2_CH1_MCIO_DDRIOMCC_REG 0x0D044B68

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N1RANK2_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK3_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB6C)                                                     */
/*       BDX (0x4019EB6C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK3_CH1_MCIO_DDRIOMCC_REG 0x0D044B6C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N1RANK3_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK4_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB70)                                                     */
/*       BDX (0x4019EB70)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK4_CH1_MCIO_DDRIOMCC_REG 0x0D044B70

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N1RANK4_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK5_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB74)                                                     */
/*       BDX (0x4019EB74)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK5_CH1_MCIO_DDRIOMCC_REG 0x0D044B74

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N1RANK5_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK6_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB78)                                                     */
/*       BDX (0x4019EB78)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK6_CH1_MCIO_DDRIOMCC_REG 0x0D044B78

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N1RANK6_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TXGROUP1N1RANK7_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB7C)                                                     */
/*       BDX (0x4019EB7C)                                                     */
/* Register default value:              0x00000000                            */
#define TXGROUP1N1RANK7_CH1_MCIO_DDRIOMCC_REG 0x0D044B7C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} TXGROUP1N1RANK7_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK0_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB80)                                                     */
/*       BDX (0x4019EB80)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK0_CH1_MCIO_DDRIOMCC_REG 0x0D044B80

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN1RANK0_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK1_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB84)                                                     */
/*       BDX (0x4019EB84)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK1_CH1_MCIO_DDRIOMCC_REG 0x0D044B84

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN1RANK1_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK2_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB88)                                                     */
/*       BDX (0x4019EB88)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK2_CH1_MCIO_DDRIOMCC_REG 0x0D044B88

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN1RANK2_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK3_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB8C)                                                     */
/*       BDX (0x4019EB8C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK3_CH1_MCIO_DDRIOMCC_REG 0x0D044B8C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN1RANK3_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK4_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB90)                                                     */
/*       BDX (0x4019EB90)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK4_CH1_MCIO_DDRIOMCC_REG 0x0D044B90

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN1RANK4_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK5_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB94)                                                     */
/*       BDX (0x4019EB94)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK5_CH1_MCIO_DDRIOMCC_REG 0x0D044B94

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN1RANK5_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK6_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB98)                                                     */
/*       BDX (0x4019EB98)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK6_CH1_MCIO_DDRIOMCC_REG 0x0D044B98

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN1RANK6_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* RXOFFSETN1RANK7_CH1_MCIO_DDRIOMCC_REG supported on:                        */
/*       HSX (0x4019EB9C)                                                     */
/*       BDX (0x4019EB9C)                                                     */
/* Register default value:              0x00000000                            */
#define RXOFFSETN1RANK7_CH1_MCIO_DDRIOMCC_REG 0x0D044B9C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXOFFSETN1RANK7_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* RXVREFCTRLN1_CH1_MCIO_DDRIOMCC_REG supported on:                           */
/*       HSX (0x4019EBA4)                                                     */
/*       BDX (0x4019EBA4)                                                     */
/* Register default value:              0x00000000                            */
#define RXVREFCTRLN1_CH1_MCIO_DDRIOMCC_REG 0x0D044BA4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} RXVREFCTRLN1_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DATATRAINFEEDBACKN1_CH1_MCIO_DDRIOMCC_REG supported on:                    */
/*       HSX (0x4019EBA8)                                                     */
/*       BDX (0x4019EBA8)                                                     */
/* Register default value:              0x00000000                            */
#define DATATRAINFEEDBACKN1_CH1_MCIO_DDRIOMCC_REG 0x0D044BA8

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DATATRAINFEEDBACKN1_CH1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






/* DDRCRCMDPICODING_CMD1_MCIO_DDRIOMCC_REG supported on:                      */
/*       HSX (0x4019EC08)                                                     */
/*       BDX (0x4019EC08)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDPICODING_CMD1_MCIO_DDRIOMCC_REG 0x0D044C08

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCMDPICODING_CMD1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCMDCONTROLS_CMD1_MCIO_DDRIOMCC_REG supported on:                      */
/*       HSX (0x4019EC0C)                                                     */
/*       BDX (0x4019EC0C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDCONTROLS_CMD1_MCIO_DDRIOMCC_REG 0x0D044C0C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
    UINT32 rsvd_7 : 13;
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
} DDRCRCMDCONTROLS_CMD1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
    UINT32 rsvd_7 : 13;
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
} DDRCRCMDCONTROLS_CMD1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDCONTROLS2_CMD1_MCIO_DDRIOMCC_REG supported on:                     */
/*       HSX (0x4019EC10)                                                     */
/*       BDX (0x4019EC10)                                                     */
/* Register default value on HSX:       0x00000000                            */
/* Register default value on BDX:       0x00001000                            */
#define DDRCRCMDCONTROLS2_CMD1_MCIO_DDRIOMCC_REG 0x0D044C10
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} DDRCRCMDCONTROLS2_CMD1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCMDCONTROLS2_CMD1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDPICODING2_CMD1_MCIO_DDRIOMCC_REG supported on:                     */
/*       HSX (0x4019EC18)                                                     */
/*       BDX (0x4019EC18)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDPICODING2_CMD1_MCIO_DDRIOMCC_REG 0x0D044C18

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCMDPICODING2_CMD1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






/* DDRCRCMDTRAINING_CMD1_MCIO_DDRIOMCC_REG supported on:                      */
/*       HSX (0x4019EC2C)                                                     */
/*       BDX (0x4019EC2C)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCMDTRAINING_CMD1_MCIO_DDRIOMCC_REG 0x0D044C2C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCMDTRAINING_CMD1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCMDCONTROLS1_CMD1_MCIO_DDRIOMCC_REG supported on:                     */
/*       HSX (0x4019EC30)                                                     */
/*       BDX (0x4019EC30)                                                     */
/* Register default value on HSX:       0x00980000                            */
/* Register default value on BDX:       0x00180000                            */
#define DDRCRCMDCONTROLS1_CMD1_MCIO_DDRIOMCC_REG 0x0D044C30
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} DDRCRCMDCONTROLS1_CMD1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCMDCONTROLS1_CMD1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */







/* DDRCRCTLPICODING_CKE1_MCIO_DDRIOMCC_REG supported on:                      */
/*       HSX (0x4019EE18)                                                     */
/*       BDX (0x4019EE18)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING_CKE1_MCIO_DDRIOMCC_REG 0x0D044E18

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCTLPICODING_CKE1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCTLCONTROLS_CKE1_MCIO_DDRIOMCC_REG supported on:                      */
/*       HSX (0x4019EE1C)                                                     */
/*       BDX (0x4019EE1C)                                                     */
/* Register default value:              0x10010000                            */
#define DDRCRCTLCONTROLS_CKE1_MCIO_DDRIOMCC_REG 0x0D044E1C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} DDRCRCTLCONTROLS_CKE1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCTLCONTROLS_CKE1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCTLRANKSUSED_CKE1_MCIO_DDRIOMCC_REG supported on:                     */
/*       HSX (0x4019EE20)                                                     */
/*       BDX (0x4019EE20)                                                     */
/* Register default value on HSX:       0x00004FFF                            */
/* Register default value on BDX:       0xC0004FFF                            */
#define DDRCRCTLRANKSUSED_CKE1_MCIO_DDRIOMCC_REG 0x0D044E20
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} DDRCRCTLRANKSUSED_CKE1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCTLRANKSUSED_CKE1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */







/* DDRCRCTLTRAINING_CKE1_MCIO_DDRIOMCC_REG supported on:                      */
/*       HSX (0x4019EE2C)                                                     */
/*       BDX (0x4019EE2C)                                                     */
/* Register default value:              0x00007000                            */
#define DDRCRCTLTRAINING_CKE1_MCIO_DDRIOMCC_REG 0x0D044E2C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} DDRCRCTLTRAINING_CKE1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCTLTRAINING_CKE1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCMDCONTROLS1_CKE1_MCIO_DDRIOMCC_REG supported on:                     */
/*       HSX (0x4019EE30)                                                     */
/*       BDX (0x4019EE30)                                                     */
/* Register default value on HSX:       0x00980000                            */
/* Register default value on BDX:       0x00180000                            */
#define DDRCRCMDCONTROLS1_CKE1_MCIO_DDRIOMCC_REG 0x0D044E30
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.19.6.CFG.xml.
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
} DDRCRCMDCONTROLS1_CKE1_MCIO_DDRIOMCC_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCMDCONTROLS1_CKE1_MCIO_DDRIOMCC_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* DDRCRCTLPICODING2_CKE1_MCIO_DDRIOMCC_REG supported on:                     */
/*       HSX (0x4019EE34)                                                     */
/*       BDX (0x4019EE34)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING2_CKE1_MCIO_DDRIOMCC_REG 0x0D044E34

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCTLPICODING2_CKE1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* DDRCRCTLPICODING3_CKE1_MCIO_DDRIOMCC_REG supported on:                     */
/*       HSX (0x4019EE38)                                                     */
/*       BDX (0x4019EE38)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING3_CKE1_MCIO_DDRIOMCC_REG 0x0D044E38

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCTLPICODING3_CKE1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* DDRCRCTLPICODING4_CKE1_MCIO_DDRIOMCC_REG supported on:                     */
/*       HSX (0x4019EE40)                                                     */
/*       BDX (0x4019EE40)                                                     */
/* Register default value:              0x00000000                            */
#define DDRCRCTLPICODING4_CKE1_MCIO_DDRIOMCC_REG 0x0D044E40

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.19.6.CFG.xml.
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
} DDRCRCTLPICODING4_CKE1_MCIO_DDRIOMCC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* SVID_MCIO_DDRIOMCC_REG supported on:                                       */
/*       IVT_EP (0x2018E02C)                                                  */
/* Register default value:              0x8086                                */
#define SVID_MCIO_DDRIOMCC_REG 0x0D04202C



/* SDID_MCIO_DDRIOMCC_REG supported on:                                       */
/*       IVT_EP (0x2018E02E)                                                  */
/* Register default value:              0x0000                                */
#define SDID_MCIO_DDRIOMCC_REG 0x0D04202E



/* PXPCAP_MCIO_DDRIOMCC_REG supported on:                                     */
/*       IVT_EP (0x4018E040)                                                  */
/* Register default value:              0x00910010                            */
#define PXPCAP_MCIO_DDRIOMCC_REG 0x0D044040





/* GDCRTRAININGMODEA0_MCIO_DDRIOMCC_REG supported on:                         */
/*       IVT_EP (0x4018E108)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTRAININGMODEA0_MCIO_DDRIOMCC_REG 0x0D044108



/* GDCRTRAININGRESULT1A0_MCIO_DDRIOMCC_REG supported on:                      */
/*       IVT_EP (0x4018E10C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTRAININGRESULT1A0_MCIO_DDRIOMCC_REG 0x0D04410C



/* GDCRTRAININGRESULT2A0_MCIO_DDRIOMCC_REG supported on:                      */
/*       IVT_EP (0x4018E110)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTRAININGRESULT2A0_MCIO_DDRIOMCC_REG 0x0D044110



/* GDCRDATACOMPBA0_MCIO_DDRIOMCC_REG supported on:                            */
/*       IVT_EP (0x4018E114)                                                  */
/* Register default value:              0x24000000                            */
#define GDCRDATACOMPBA0_MCIO_DDRIOMCC_REG 0x0D044114



/* GDCRBUFFCFGA0_MCIO_DDRIOMCC_REG supported on:                              */
/*       IVT_EP (0x4018E118)                                                  */
/* Register default value:              0x03FF0001                            */
#define GDCRBUFFCFGA0_MCIO_DDRIOMCC_REG 0x0D044118



/* GDCRDATADEBUGMUXCFGA0_MCIO_DDRIOMCC_REG supported on:                      */
/*       IVT_EP (0x4018E11C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRDATADEBUGMUXCFGA0_MCIO_DDRIOMCC_REG 0x0D04411C



/* GDCRTXRXBOTRANK0A0_MCIO_DDRIOMCC_REG supported on:                         */
/*       IVT_EP (0x4018E120)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXRXBOTRANK0A0_MCIO_DDRIOMCC_REG 0x0D044120



/* GDCRTXRXBOTRANK1A0_MCIO_DDRIOMCC_REG supported on:                         */
/*       IVT_EP (0x4018E124)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXRXBOTRANK1A0_MCIO_DDRIOMCC_REG 0x0D044124



/* GDCRTXRXBOTRANK2A0_MCIO_DDRIOMCC_REG supported on:                         */
/*       IVT_EP (0x4018E128)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXRXBOTRANK2A0_MCIO_DDRIOMCC_REG 0x0D044128



/* GDCRTXRXBOTRANK3A0_MCIO_DDRIOMCC_REG supported on:                         */
/*       IVT_EP (0x4018E12C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXRXBOTRANK3A0_MCIO_DDRIOMCC_REG 0x0D04412C



/* GDCRTXRXBOTRANK4A0_MCIO_DDRIOMCC_REG supported on:                         */
/*       IVT_EP (0x4018E130)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXRXBOTRANK4A0_MCIO_DDRIOMCC_REG 0x0D044130



/* GDCRTXRXBOTRANK5A0_MCIO_DDRIOMCC_REG supported on:                         */
/*       IVT_EP (0x4018E134)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXRXBOTRANK5A0_MCIO_DDRIOMCC_REG 0x0D044134



/* GDCRTXRXBOTRANK6A0_MCIO_DDRIOMCC_REG supported on:                         */
/*       IVT_EP (0x4018E138)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXRXBOTRANK6A0_MCIO_DDRIOMCC_REG 0x0D044138



/* GDCRTXRXBOTRANK7A0_MCIO_DDRIOMCC_REG supported on:                         */
/*       IVT_EP (0x4018E13C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXRXBOTRANK7A0_MCIO_DDRIOMCC_REG 0x0D04413C



/* GDCRRXTOPRANK0A0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x4018E140)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRRXTOPRANK0A0_MCIO_DDRIOMCC_REG 0x0D044140



/* GDCRRXTOPRANK1A0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x4018E144)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRRXTOPRANK1A0_MCIO_DDRIOMCC_REG 0x0D044144



/* GDCRRXTOPRANK2A0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x4018E148)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRRXTOPRANK2A0_MCIO_DDRIOMCC_REG 0x0D044148



/* GDCRRXTOPRANK3A0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x4018E14C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRRXTOPRANK3A0_MCIO_DDRIOMCC_REG 0x0D04414C



/* GDCRRXTOPRANK4A0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x4018E150)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRRXTOPRANK4A0_MCIO_DDRIOMCC_REG 0x0D044150



/* GDCRRXTOPRANK5A0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x4018E154)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRRXTOPRANK5A0_MCIO_DDRIOMCC_REG 0x0D044154



/* GDCRRXTOPRANK6A0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x4018E158)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRRXTOPRANK6A0_MCIO_DDRIOMCC_REG 0x0D044158



/* GDCRRXTOPRANK7A0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x4018E15C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRRXTOPRANK7A0_MCIO_DDRIOMCC_REG 0x0D04415C



/* GDCRTXTOPRANK0A0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x4018E160)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXTOPRANK0A0_MCIO_DDRIOMCC_REG 0x0D044160



/* GDCRTXTOPRANK1A0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x4018E164)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXTOPRANK1A0_MCIO_DDRIOMCC_REG 0x0D044164



/* GDCRTXTOPRANK2A0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x4018E168)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXTOPRANK2A0_MCIO_DDRIOMCC_REG 0x0D044168



/* GDCRTXTOPRANK3A0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x4018E16C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXTOPRANK3A0_MCIO_DDRIOMCC_REG 0x0D04416C



/* GDCRTXTOPRANK4A0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x4018E170)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXTOPRANK4A0_MCIO_DDRIOMCC_REG 0x0D044170



/* GDCRTXTOPRANK5A0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x4018E174)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXTOPRANK5A0_MCIO_DDRIOMCC_REG 0x0D044174



/* GDCRTXTOPRANK6A0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x4018E178)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXTOPRANK6A0_MCIO_DDRIOMCC_REG 0x0D044178



/* GDCRTXTOPRANK7A0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x4018E17C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXTOPRANK7A0_MCIO_DDRIOMCC_REG 0x0D04417C







/* GDCRCMDVREFA0_MCIO_DDRIOMCC_REG supported on:                              */
/*       IVT_EP (0x4018E30C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCMDVREFA0_MCIO_DDRIOMCC_REG 0x0D04430C



/* GDCRCMDPICODEA0_MCIO_DDRIOMCC_REG supported on:                            */
/*       IVT_EP (0x4018E310)                                                  */
/* Register default value:              0x40404040                            */
#define GDCRCMDPICODEA0_MCIO_DDRIOMCC_REG 0x0D044310



/* GDCRCMDLOGICPHASEDELAYA0_MCIO_DDRIOMCC_REG supported on:                   */
/*       IVT_EP (0x4018E314)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCMDLOGICPHASEDELAYA0_MCIO_DDRIOMCC_REG 0x0D044314



/* GDCRCMDDEBUGMUXDFTA0_MCIO_DDRIOMCC_REG supported on:                       */
/*       IVT_EP (0x4018E318)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCMDDEBUGMUXDFTA0_MCIO_DDRIOMCC_REG 0x0D044318





/* GDCRRANKCFGA0_MCIO_DDRIOMCC_REG supported on:                              */
/*       IVT_EP (0x2018E320)                                                  */
/* Register default value:              0xF3FF                                */
#define GDCRRANKCFGA0_MCIO_DDRIOMCC_REG 0x0D042320



/* GDCRCMDPICODE2A0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x4018E324)                                                  */
/* Register default value:              0x00004040                            */
#define GDCRCMDPICODE2A0_MCIO_DDRIOMCC_REG 0x0D044324





/* GDCRCKCOMP2A0_MCIO_DDRIOMCC_REG supported on:                              */
/*       IVT_EP (0x4018E388)                                                  */
/* Register default value:              0x00010491                            */
#define GDCRCKCOMP2A0_MCIO_DDRIOMCC_REG 0x0D044388



/* GDCRCKRANKUSEDA0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x2018E38C)                                                  */
/* Register default value:              0x0033                                */
#define GDCRCKRANKUSEDA0_MCIO_DDRIOMCC_REG 0x0D04238C



/* GDCRCKPICODE0A0_MCIO_DDRIOMCC_REG supported on:                            */
/*       IVT_EP (0x4018E390)                                                  */
/* Register default value:              0x03000000                            */
#define GDCRCKPICODE0A0_MCIO_DDRIOMCC_REG 0x0D044390



/* GDCRCKPICODE1A0_MCIO_DDRIOMCC_REG supported on:                            */
/*       IVT_EP (0x4018E394)                                                  */
/* Register default value:              0x03000000                            */
#define GDCRCKPICODE1A0_MCIO_DDRIOMCC_REG 0x0D044394



/* GDCRCKLOGICDELAYA0_MCIO_DDRIOMCC_REG supported on:                         */
/*       IVT_EP (0x2018E398)                                                  */
/* Register default value:              0x0000                                */
#define GDCRCKLOGICDELAYA0_MCIO_DDRIOMCC_REG 0x0D042398







/* GDCRCLKDLLDFTA0_MCIO_DDRIOMCC_REG supported on:                            */
/*       IVT_EP (0x2018E3A4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRCLKDLLDFTA0_MCIO_DDRIOMCC_REG 0x0D0423A4



/* GDCRCLKVSSHIA0_MCIO_DDRIOMCC_REG supported on:                             */
/*       IVT_EP (0x4018E3A8)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCLKVSSHIA0_MCIO_DDRIOMCC_REG 0x0D0443A8



/* GDCRCLKVMCFGA0_MCIO_DDRIOMCC_REG supported on:                             */
/*       IVT_EP (0x1018E3AC)                                                  */
/* Register default value:              0x00                                  */
#define GDCRCLKVMCFGA0_MCIO_DDRIOMCC_REG 0x0D0413AC









/* GDCRCOMPOVR2A0_MCIO_DDRIOMCC_REG supported on:                             */
/*       IVT_EP (0x4018E408)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCOMPOVR2A0_MCIO_DDRIOMCC_REG 0x0D044408



/* GDCRCOMPOVR3A0_MCIO_DDRIOMCC_REG supported on:                             */
/*       IVT_EP (0x4018E40C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCOMPOVR3A0_MCIO_DDRIOMCC_REG 0x0D04440C



/* GDCRCOMPOVR4A0_MCIO_DDRIOMCC_REG supported on:                             */
/*       IVT_EP (0x4018E410)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCOMPOVR4A0_MCIO_DDRIOMCC_REG 0x0D044410



/* GDCRCOMPOVR5A0_MCIO_DDRIOMCC_REG supported on:                             */
/*       IVT_EP (0x4018E414)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCOMPOVR5A0_MCIO_DDRIOMCC_REG 0x0D044414



/* GDCRCOMPOVR6A0_MCIO_DDRIOMCC_REG supported on:                             */
/*       IVT_EP (0x4018E418)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCOMPOVR6A0_MCIO_DDRIOMCC_REG 0x0D044418



/* GDCRCOMPOVROFST1A0_MCIO_DDRIOMCC_REG supported on:                         */
/*       IVT_EP (0x4018E41C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCOMPOVROFST1A0_MCIO_DDRIOMCC_REG 0x0D04441C



/* GDCRCOMPOVROFST2A0_MCIO_DDRIOMCC_REG supported on:                         */
/*       IVT_EP (0x4018E420)                                                  */
/* Register default value:              0x03F00000                            */
#define GDCRCOMPOVROFST2A0_MCIO_DDRIOMCC_REG 0x0D044420



/* GDCRCOMPOVROFST3A0_MCIO_DDRIOMCC_REG supported on:                         */
/*       IVT_EP (0x4018E424)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCOMPOVROFST3A0_MCIO_DDRIOMCC_REG 0x0D044424



/* GDCRCOMPOVROFST4A0_MCIO_DDRIOMCC_REG supported on:                         */
/*       IVT_EP (0x4018E428)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCOMPOVROFST4A0_MCIO_DDRIOMCC_REG 0x0D044428



/* GDCRCOMPCFGSPDA0_MCIO_DDRIOMCC_REG supported on:                           */
/*       IVT_EP (0x4018E42C)                                                  */
/* Register default value:              0x00010B5A                            */
#define GDCRCOMPCFGSPDA0_MCIO_DDRIOMCC_REG 0x0D04442C







/* GDCRMSCCTLA0_MCIO_DDRIOMCC_REG supported on:                               */
/*       IVT_EP (0x4018E43C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRMSCCTLA0_MCIO_DDRIOMCC_REG 0x0D04443C



/* GDCRCTLCOMPTXA0_MCIO_DDRIOMCC_REG supported on:                            */
/*       IVT_EP (0x4018E704)                                                  */
/* Register default value:              0x1F803820                            */
#define GDCRCTLCOMPTXA0_MCIO_DDRIOMCC_REG 0x0D044704





/* GDCRCOMPOFFSET_RXA0_MCIO_DDRIOMCC_REG supported on:                        */
/*       IVT_EP (0x4018E70C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCOMPOFFSET_RXA0_MCIO_DDRIOMCC_REG 0x0D04470C



/* GDCRCTLVREFRXBIASDCDETECTA0_MCIO_DDRIOMCC_REG supported on:                */
/*       IVT_EP (0x4018E710)                                                  */
/* Register default value:              0x00050000                            */
#define GDCRCTLVREFRXBIASDCDETECTA0_MCIO_DDRIOMCC_REG 0x0D044710



/* GDCRCTLPICODE03A0_MCIO_DDRIOMCC_REG supported on:                          */
/*       IVT_EP (0x4018E714)                                                  */
/* Register default value:              0x40404040                            */
#define GDCRCTLPICODE03A0_MCIO_DDRIOMCC_REG 0x0D044714



/* GDCRCTLPICODE47A0_MCIO_DDRIOMCC_REG supported on:                          */
/*       IVT_EP (0x4018E718)                                                  */
/* Register default value:              0x40404040                            */
#define GDCRCTLPICODE47A0_MCIO_DDRIOMCC_REG 0x0D044718



/* GDCRCTLPICODE810A0_MCIO_DDRIOMCC_REG supported on:                         */
/*       IVT_EP (0x4018E71C)                                                  */
/* Register default value:              0x00404040                            */
#define GDCRCTLPICODE810A0_MCIO_DDRIOMCC_REG 0x0D04471C





/* GDCRCTLDEBUGMUXDFTA0_MCIO_DDRIOMCC_REG supported on:                       */
/*       IVT_EP (0x4018E724)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCTLDEBUGMUXDFTA0_MCIO_DDRIOMCC_REG 0x0D044724



/* GDCRCTLRANKCNFGXTALK1A0_MCIO_DDRIOMCC_REG supported on:                    */
/*       IVT_EP (0x4018E728)                                                  */
/* Register default value:              0x000003FF                            */
#define GDCRCTLRANKCNFGXTALK1A0_MCIO_DDRIOMCC_REG 0x0D044728



/* GDCRCTLXTALK2ANDVMSEODTA0_MCIO_DDRIOMCC_REG supported on:                  */
/*       IVT_EP (0x4018E72C)                                                  */
/* Register default value:              0x10000000                            */
#define GDCRCTLXTALK2ANDVMSEODTA0_MCIO_DDRIOMCC_REG 0x0D04472C



/* GDCRCTLVSSHIA0_MCIO_DDRIOMCC_REG supported on:                             */
/*       IVT_EP (0x4018E730)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCTLVSSHIA0_MCIO_DDRIOMCC_REG 0x0D044730



/* GDCRCTLVSSHIDBGCNTLA0_MCIO_DDRIOMCC_REG supported on:                      */
/*       IVT_EP (0x4018E734)                                                  */
/* Register default value:              0x00000500                            */
#define GDCRCTLVSSHIDBGCNTLA0_MCIO_DDRIOMCC_REG 0x0D044734



/* GDCRCTLACIOLBA0_MCIO_DDRIOMCC_REG supported on:                            */
/*       IVT_EP (0x4018E738)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCTLACIOLBA0_MCIO_DDRIOMCC_REG 0x0D044738



/* GDCRVMSEERRORA0_MCIO_DDRIOMCC_REG supported on:                            */
/*       IVT_EP (0x4018E73C)                                                  */
/* Register default value:              0x03FFFF00                            */
#define GDCRVMSEERRORA0_MCIO_DDRIOMCC_REG 0x0D04473C



/* GDCRCTLLOGICDELAYSELECTA0_MCIO_DDRIOMCC_REG supported on:                  */
/*       IVT_EP (0x4018E740)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCTLLOGICDELAYSELECTA0_MCIO_DDRIOMCC_REG 0x0D044740













#endif /* MCIO_DDRIOMCC_h */
