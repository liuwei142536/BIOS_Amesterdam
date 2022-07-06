/* Date Stamp: 8/23/2014 */

#ifndef HA_CFG_h
#define HA_CFG_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* HA_CFG_IVT_DEV 14                                                          */
/* HA_CFG_IVT_FUN 0                                                           */
/* For HSX_HOST:                                                              */
/* HA_CFG_HSX_DEV 18                                                          */
/* HA_CFG_HSX_FUN 0                                                           */
/* For BDX_HOST:                                                              */
/* HA_CFG_BDX_DEV 18                                                          */
/* HA_CFG_BDX_FUN 0                                                           */

/* VID_HA_CFG_REG supported on:                                               */
/*       IVT_EP (0x20170000)                                                  */
/*       IVT_EX (0x20170000)                                                  */
/*       HSX (0x20190000)                                                     */
/*       BDX (0x20190000)                                                     */
/* Register default value:              0x8086                                */
#define VID_HA_CFG_REG 0x03002000


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.0.CFG.xml.
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
} VID_HA_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* DID_HA_CFG_REG supported on:                                               */
/*       IVT_EP (0x20170002)                                                  */
/*       IVT_EX (0x20170002)                                                  */
/*       HSX (0x20190002)                                                     */
/*       BDX (0x20190002)                                                     */
/* Register default value on IVT_EP:    0x0EA0                                */
/* Register default value on IVT_EX:    0x0EA0                                */
/* Register default value on HSX:       0x2FA0                                */
/* Register default value on BDX:       0x6FA0                                */
#define DID_HA_CFG_REG 0x03002002


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.0.CFG.xml.
 * PCI Device Identification Number
 */
typedef union {
  struct {
    UINT16 device_identification_number : 16;
    /* device_identification_number - Bits[15:0], RO, default = 16'h2FA0 (HSX), 16'h6FA0 (BDX) 
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
} DID_HA_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PCICMD_HA_CFG_REG supported on:                                            */
/*       IVT_EP (0x20170004)                                                  */
/*       IVT_EX (0x20170004)                                                  */
/*       HSX (0x20190004)                                                     */
/*       BDX (0x20190004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_HA_CFG_REG 0x03002004


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.0.CFG.xml.
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
} PCICMD_HA_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* PCISTS_HA_CFG_REG supported on:                                            */
/*       IVT_EP (0x20170006)                                                  */
/*       IVT_EX (0x20170006)                                                  */
/*       HSX (0x20190006)                                                     */
/*       BDX (0x20190006)                                                     */
/* Register default value:              0x0000                                */
#define PCISTS_HA_CFG_REG 0x03002006


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.0.CFG.xml.
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
} PCISTS_HA_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* RID_HA_CFG_REG supported on:                                               */
/*       IVT_EP (0x10170008)                                                  */
/*       IVT_EX (0x10170008)                                                  */
/*       HSX (0x10190008)                                                     */
/*       BDX (0x10190008)                                                     */
/* Register default value:              0x00                                  */
#define RID_HA_CFG_REG 0x03001008
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
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
} RID_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* CCR_N0_HA_CFG_REG supported on:                                            */
/*       IVT_EP (0x10170009)                                                  */
/*       IVT_EX (0x10170009)                                                  */
/*       HSX (0x10190009)                                                     */
/*       BDX (0x10190009)                                                     */
/* Register default value:              0x00                                  */
#define CCR_N0_HA_CFG_REG 0x03001009


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.0.CFG.xml.
 * PCIe header ClassCode register
 */
typedef union {
  struct {
    UINT8 register_level_programming_interface : 8;
    /* register_level_programming_interface - Bits[7:0], RO_V, default = 8'b00000000  */
  } Bits;
  UINT8 Data;
} CCR_N0_HA_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* CCR_N1_HA_CFG_REG supported on:                                            */
/*       IVT_EP (0x2017000A)                                                  */
/*       IVT_EX (0x2017000A)                                                  */
/*       HSX (0x2019000A)                                                     */
/*       BDX (0x2019000A)                                                     */
/* Register default value:              0x0880                                */
#define CCR_N1_HA_CFG_REG 0x0300200A
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
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
} CCR_N1_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* CLSR_HA_CFG_REG supported on:                                              */
/*       HSX (0x1019000C)                                                     */
/*       BDX (0x1019000C)                                                     */
/* Register default value:              0x00                                  */
#define CLSR_HA_CFG_REG 0x0300100C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
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
} CLSR_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* PLAT_HA_CFG_REG supported on:                                              */
/*       HSX (0x1019000D)                                                     */
/*       BDX (0x1019000D)                                                     */
/* Register default value:              0x00                                  */
#define PLAT_HA_CFG_REG 0x0300100D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
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
} PLAT_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* HDR_HA_CFG_REG supported on:                                               */
/*       IVT_EP (0x1017000E)                                                  */
/*       IVT_EX (0x1017000E)                                                  */
/*       HSX (0x1019000E)                                                     */
/*       BDX (0x1019000E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_HA_CFG_REG 0x0300100E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
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
} HDR_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* BIST_HA_CFG_REG supported on:                                              */
/*       IVT_EP (0x1017000F)                                                  */
/*       IVT_EX (0x1017000F)                                                  */
/*       HSX (0x1019000F)                                                     */
/*       BDX (0x1019000F)                                                     */
/* Register default value:              0x00                                  */
#define BIST_HA_CFG_REG 0x0300100F


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.0.CFG.xml.
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
} BIST_HA_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */








/* PCOMMITADDRHICH2_HA_CFG_REG supported on:                                  */
/*       BDX (0x40190014)                                                     */
/* Register default value:              0x00000000                            */
#define PCOMMITADDRHICH2_HA_CFG_REG 0x03004014

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 hiphyaddr : 14;
    /* hiphyaddr - Bits[13:0], RW, default = 14'b00000000000000 
       This contains 14 bits of physical address [45:32] of a cache line, corresponding 
       address match is an indication of pcommit wb   
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PCOMMITADDRHICH2_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* PCOMMITADDRCH0_HA_CFG_REG supported on:                                    */
/*       BDX (0x40190018)                                                     */
/* Register default value:              0x00000000                            */
#define PCOMMITADDRCH0_HA_CFG_REG 0x03004018

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rsvd : 6;
    /* rsvd - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 lophyaddr : 26;
    /* lophyaddr - Bits[31:6], RW, default = 26'b00000000000000000000000000 
       This contains 26 bits of physical address [31:6] of a cache line, corresponding 
       address match is an indication of pcommit wb   
     */
  } Bits;
  UINT32 Data;
} PCOMMITADDRCH0_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* PCOMMITADDRHICH0_HA_CFG_REG supported on:                                  */
/*       BDX (0x4019001C)                                                     */
/* Register default value:              0x00000000                            */
#define PCOMMITADDRHICH0_HA_CFG_REG 0x0300401C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 hiphyaddr : 14;
    /* hiphyaddr - Bits[13:0], RW, default = 14'b00000000000000 
       This contains 14 bits of physical address [45:32] of a cache line, corresponding 
       address match is an indication of pcommit wb   
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PCOMMITADDRHICH0_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* PCOMMITADDRCH1_HA_CFG_REG supported on:                                    */
/*       BDX (0x40190020)                                                     */
/* Register default value:              0x00000000                            */
#define PCOMMITADDRCH1_HA_CFG_REG 0x03004020

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rsvd : 6;
    /* rsvd - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 lophyaddr : 26;
    /* lophyaddr - Bits[31:6], RW, default = 26'b00000000000000000000000000 
       This contains 26 bits of physical address [31:6] of a cache line, corresponding 
       address match is an indication of pcommit wb   
     */
  } Bits;
  UINT32 Data;
} PCOMMITADDRCH1_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* PCOMMITADDRHICH1_HA_CFG_REG supported on:                                  */
/*       BDX (0x40190024)                                                     */
/* Register default value:              0x00000000                            */
#define PCOMMITADDRHICH1_HA_CFG_REG 0x03004024

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 hiphyaddr : 14;
    /* hiphyaddr - Bits[13:0], RW, default = 14'b00000000000000 
       This contains 14 bits of physical address [45:32] of a cache line, corresponding 
       address match is an indication of pcommit wb   
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PCOMMITADDRHICH1_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* PCOMMITADDRCH2_HA_CFG_REG supported on:                                    */
/*       BDX (0x40190028)                                                     */
/* Register default value:              0x00000000                            */
#define PCOMMITADDRCH2_HA_CFG_REG 0x03004028

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rsvd : 6;
    /* rsvd - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 lophyaddr : 26;
    /* lophyaddr - Bits[31:6], RW, default = 26'b00000000000000000000000000 
       This contains 26 bits of physical address [31:6] of a cache line, corresponding 
       address match is an indication of pcommit wb   
     */
  } Bits;
  UINT32 Data;
} PCOMMITADDRCH2_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* SVID_HA_CFG_REG supported on:                                              */
/*       IVT_EP (0x2017002C)                                                  */
/*       IVT_EX (0x2017002C)                                                  */
/*       HSX (0x2019002C)                                                     */
/*       BDX (0x2019002C)                                                     */
/* Register default value:              0x0000                                */
#define SVID_HA_CFG_REG 0x0300202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * This value is used to identify the vendor of the subsystem.
 */
typedef union {
  struct {
    UINT16 subvid : 16;
    /* subvid - Bits[15:0], RW_O, default = 16'b0000000000000000 
       This field should be programmed during boot-up to indicate the vendor of the 
       system board. After it has been written once, it becomes read only.PCI standard 
       identification for Intel. 
     */
  } Bits;
  UINT16 Data;
} SVID_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* SID_HA_CFG_REG supported on:                                               */
/*       IVT_EP (0x2017002E)                                                  */
/*       IVT_EX (0x2017002E)                                                  */
/*       HSX (0x2019002E)                                                     */
/*       BDX (0x2019002E)                                                     */
/* Register default value:              0x0000                                */
#define SID_HA_CFG_REG 0x0300202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * This value is used to identify a particular subsystem.
 */
typedef union {
  struct {
    UINT16 subid : 16;
    /* subid - Bits[15:0], RW_O, default = 16'b0000000000000000 
       This field should be programmed during BIOS initialization. After it has been 
       written once, it becomes read only. 
     */
  } Bits;
  UINT16 Data;
} SID_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* CAPPTR_HA_CFG_REG supported on:                                            */
/*       HSX (0x10190034)                                                     */
/*       BDX (0x10190034)                                                     */
/* Register default value:              0x00                                  */
#define CAPPTR_HA_CFG_REG 0x03001034

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
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
} CAPPTR_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTL_HA_CFG_REG supported on:                                              */
/*       HSX (0x1019003C)                                                     */
/*       BDX (0x1019003C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_HA_CFG_REG 0x0300103C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
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
} INTL_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* INTPIN_HA_CFG_REG supported on:                                            */
/*       HSX (0x1019003D)                                                     */
/*       BDX (0x1019003D)                                                     */
/* Register default value:              0x00                                  */
#define INTPIN_HA_CFG_REG 0x0300103D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
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
} INTPIN_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* MINGNT_HA_CFG_REG supported on:                                            */
/*       IVT_EP (0x1017003E)                                                  */
/*       IVT_EX (0x1017003E)                                                  */
/*       HSX (0x1019003E)                                                     */
/*       BDX (0x1019003E)                                                     */
/* Register default value:              0x00                                  */
#define MINGNT_HA_CFG_REG 0x0300103E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
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
} MINGNT_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* MAXLAT_HA_CFG_REG supported on:                                            */
/*       IVT_EP (0x1017003F)                                                  */
/*       IVT_EX (0x1017003F)                                                  */
/*       HSX (0x1019003F)                                                     */
/*       BDX (0x1019003F)                                                     */
/* Register default value:              0x00                                  */
#define MAXLAT_HA_CFG_REG 0x0300103F
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
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
} MAXLAT_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* TAD_0_HA_CFG_REG supported on:                                             */
/*       HSX (0x40190040)                                                     */
/*       BDX (0x40190040)                                                     */
/* Register default value:              0x00000000                            */
#define TAD_0_HA_CFG_REG 0x03004040

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Target Address Decode register
 */
typedef union {
  struct {
    UINT32 channel_0_id : 2;
    /* channel_0_id - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-channel interleaving).
     */
    UINT32 channel_1_id : 2;
    /* channel_1_id - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-channel interleaving).
     */
    UINT32 channel_2_id : 2;
    /* channel_2_id - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-channel interleaving).
     */
    UINT32 channel_3_id : 2;
    /* channel_3_id - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-channel interleaving).
     */
    UINT32 channelways : 2;
    /* channelways - Bits[9:8], RW_LB, default = 2'b00 
       This is a field that defines the number of memory channels interleave within a 
       home agent. 
        
        00: 1 channel way
        01: 2 channel ways
        10: 3 channel ways
        11: 4 channel ways
        Default value: 00b = 1 memory way
     */
    UINT32 socketways : 2;
    /* socketways - Bits[11:10], RW_LB, default = 2'b00 
       This is a filed that defines the number of home agent ways to interleave.
       
       00: 1 home agent (no interleave)
       01: 2 home agents interleave
       10: 4 home agents interleave
       11: 8 home agents interleave
        Default value: 00 = 1 home agent in the system
     */
    UINT32 limit : 20;
    /* limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       
       This is a field that defines the memory region limit. It contains the physical 
       address bit range [45:26]. 0 LessThanEqual Physicaladdress[45:26] LessThanEqual 
       TAD0.limit, when N = 0. TAD[N-1].limit + 1 LessThanEqual Physicaladdress[45:26] 
       LessThanEqual TAD[N].limit, when N = 1 to 11. 
        Note: "-LB Attr" means Ubox message ConfigRegWr can write to this register.
        However ConfigWrLtLock cannot write to this register 
     */
  } Bits;
  UINT32 Data;
} TAD_0_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TAD_1_HA_CFG_REG supported on:                                             */
/*       HSX (0x40190044)                                                     */
/*       BDX (0x40190044)                                                     */
/* Register default value:              0x00000000                            */
#define TAD_1_HA_CFG_REG 0x03004044

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Target Address Decode register
 */
typedef union {
  struct {
    UINT32 channel_0_id : 2;
    /* channel_0_id - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-channel interleaving).
     */
    UINT32 channel_1_id : 2;
    /* channel_1_id - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-channel interleaving).
     */
    UINT32 channel_2_id : 2;
    /* channel_2_id - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-channel interleaving).
     */
    UINT32 channel_3_id : 2;
    /* channel_3_id - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-channel interleaving).
     */
    UINT32 channelways : 2;
    /* channelways - Bits[9:8], RW_LB, default = 2'b00 
       This is a field that defines the number of memory channels interleave within a 
       home agent. 
        
        00: 1 channel way
        01: 2 channel ways
        10: 3 channel ways
        11: 4 channel ways
        Default value: 00b = 1 memory way
     */
    UINT32 socketways : 2;
    /* socketways - Bits[11:10], RW_LB, default = 2'b00 
       This is a filed that defines the number of home agent ways to interleave.
       
       00: 1 home agent (no interleave)
       01: 2 home agents interleave
       10: 4 home agents interleave
       11: 8 home agents interleave
        Default value: 00 = 1 home agent in the system
     */
    UINT32 limit : 20;
    /* limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       
       This is a field that defines the memory region limit. It contains the physical 
       address bit range [45:26]. 0 LessThanEqual Physicaladdress[45:26] LessThanEqual 
       TAD0.limit, when N = 0. TAD[N-1].limit + 1 LessThanEqual Physicaladdress[45:26] 
       LessThanEqual TAD[N].limit, when N = 1 to 11. 
        Note: "-LB Attr" means Ubox message ConfigRegWr can write to this register.
        However ConfigWrLtLock cannot write to this register 
     */
  } Bits;
  UINT32 Data;
} TAD_1_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TAD_2_HA_CFG_REG supported on:                                             */
/*       HSX (0x40190048)                                                     */
/*       BDX (0x40190048)                                                     */
/* Register default value:              0x00000000                            */
#define TAD_2_HA_CFG_REG 0x03004048

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Target Address Decode register
 */
typedef union {
  struct {
    UINT32 channel_0_id : 2;
    /* channel_0_id - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-channel interleaving).
     */
    UINT32 channel_1_id : 2;
    /* channel_1_id - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-channel interleaving).
     */
    UINT32 channel_2_id : 2;
    /* channel_2_id - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-channel interleaving).
     */
    UINT32 channel_3_id : 2;
    /* channel_3_id - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-channel interleaving).
     */
    UINT32 channelways : 2;
    /* channelways - Bits[9:8], RW_LB, default = 2'b00 
       This is a field that defines the number of memory channels interleave within a 
       home agent. 
        
        00: 1 channel way
        01: 2 channel ways
        10: 3 channel ways
        11: 4 channel ways
        Default value: 00b = 1 memory way
     */
    UINT32 socketways : 2;
    /* socketways - Bits[11:10], RW_LB, default = 2'b00 
       This is a filed that defines the number of home agent ways to interleave.
       
       00: 1 home agent (no interleave)
       01: 2 home agents interleave
       10: 4 home agents interleave
       11: 8 home agents interleave
        Default value: 00 = 1 home agent in the system
     */
    UINT32 limit : 20;
    /* limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       
       This is a field that defines the memory region limit. It contains the physical 
       address bit range [45:26]. 0 LessThanEqual Physicaladdress[45:26] LessThanEqual 
       TAD0.limit, when N = 0. TAD[N-1].limit + 1 LessThanEqual Physicaladdress[45:26] 
       LessThanEqual TAD[N].limit, when N = 1 to 11. 
        Note: "-LB Attr" means Ubox message ConfigRegWr can write to this register.
        However ConfigWrLtLock cannot write to this register 
     */
  } Bits;
  UINT32 Data;
} TAD_2_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TAD_3_HA_CFG_REG supported on:                                             */
/*       HSX (0x4019004C)                                                     */
/*       BDX (0x4019004C)                                                     */
/* Register default value:              0x00000000                            */
#define TAD_3_HA_CFG_REG 0x0300404C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Target Address Decode register
 */
typedef union {
  struct {
    UINT32 channel_0_id : 2;
    /* channel_0_id - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-channel interleaving).
     */
    UINT32 channel_1_id : 2;
    /* channel_1_id - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-channel interleaving).
     */
    UINT32 channel_2_id : 2;
    /* channel_2_id - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-channel interleaving).
     */
    UINT32 channel_3_id : 2;
    /* channel_3_id - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-channel interleaving).
     */
    UINT32 channelways : 2;
    /* channelways - Bits[9:8], RW_LB, default = 2'b00 
       This is a field that defines the number of memory channels interleave within a 
       home agent. 
        
        00: 1 channel way
        01: 2 channel ways
        10: 3 channel ways
        11: 4 channel ways
        Default value: 00b = 1 memory way
     */
    UINT32 socketways : 2;
    /* socketways - Bits[11:10], RW_LB, default = 2'b00 
       This is a filed that defines the number of home agent ways to interleave.
       
       00: 1 home agent (no interleave)
       01: 2 home agents interleave
       10: 4 home agents interleave
       11: 8 home agents interleave
        Default value: 00 = 1 home agent in the system
     */
    UINT32 limit : 20;
    /* limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       
       This is a field that defines the memory region limit. It contains the physical 
       address bit range [45:26]. 0 LessThanEqual Physicaladdress[45:26] LessThanEqual 
       TAD0.limit, when N = 0. TAD[N-1].limit + 1 LessThanEqual Physicaladdress[45:26] 
       LessThanEqual TAD[N].limit, when N = 1 to 11. 
        Note: "-LB Attr" means Ubox message ConfigRegWr can write to this register.
        However ConfigWrLtLock cannot write to this register 
     */
  } Bits;
  UINT32 Data;
} TAD_3_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TAD_4_HA_CFG_REG supported on:                                             */
/*       HSX (0x40190050)                                                     */
/*       BDX (0x40190050)                                                     */
/* Register default value:              0x00000000                            */
#define TAD_4_HA_CFG_REG 0x03004050

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Target Address Decode register
 */
typedef union {
  struct {
    UINT32 channel_0_id : 2;
    /* channel_0_id - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-channel interleaving).
     */
    UINT32 channel_1_id : 2;
    /* channel_1_id - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-channel interleaving).
     */
    UINT32 channel_2_id : 2;
    /* channel_2_id - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-channel interleaving).
     */
    UINT32 channel_3_id : 2;
    /* channel_3_id - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-channel interleaving).
     */
    UINT32 channelways : 2;
    /* channelways - Bits[9:8], RW_LB, default = 2'b00 
       This is a field that defines the number of memory channels interleave within a 
       home agent. 
        
        00: 1 channel way
        01: 2 channel ways
        10: 3 channel ways
        11: 4 channel ways
        Default value: 00b = 1 memory way
     */
    UINT32 socketways : 2;
    /* socketways - Bits[11:10], RW_LB, default = 2'b00 
       This is a filed that defines the number of home agent ways to interleave.
       
       00: 1 home agent (no interleave)
       01: 2 home agents interleave
       10: 4 home agents interleave
       11: 8 home agents interleave
        Default value: 00 = 1 home agent in the system
     */
    UINT32 limit : 20;
    /* limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       
       This is a field that defines the memory region limit. It contains the physical 
       address bit range [45:26]. 0 LessThanEqual Physicaladdress[45:26] LessThanEqual 
       TAD0.limit, when N = 0. TAD[N-1].limit + 1 LessThanEqual Physicaladdress[45:26] 
       LessThanEqual TAD[N].limit, when N = 1 to 11. 
        Note: "-LB Attr" means Ubox message ConfigRegWr can write to this register.
        However ConfigWrLtLock cannot write to this register 
     */
  } Bits;
  UINT32 Data;
} TAD_4_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TAD_5_HA_CFG_REG supported on:                                             */
/*       HSX (0x40190054)                                                     */
/*       BDX (0x40190054)                                                     */
/* Register default value:              0x00000000                            */
#define TAD_5_HA_CFG_REG 0x03004054

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Target Address Decode register
 */
typedef union {
  struct {
    UINT32 channel_0_id : 2;
    /* channel_0_id - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-channel interleaving).
     */
    UINT32 channel_1_id : 2;
    /* channel_1_id - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-channel interleaving).
     */
    UINT32 channel_2_id : 2;
    /* channel_2_id - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-channel interleaving).
     */
    UINT32 channel_3_id : 2;
    /* channel_3_id - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-channel interleaving).
     */
    UINT32 channelways : 2;
    /* channelways - Bits[9:8], RW_LB, default = 2'b00 
       This is a field that defines the number of memory channels interleave within a 
       home agent. 
        
        00: 1 channel way
        01: 2 channel ways
        10: 3 channel ways
        11: 4 channel ways
        Default value: 00b = 1 memory way
     */
    UINT32 socketways : 2;
    /* socketways - Bits[11:10], RW_LB, default = 2'b00 
       This is a filed that defines the number of home agent ways to interleave.
       
       00: 1 home agent (no interleave)
       01: 2 home agents interleave
       10: 4 home agents interleave
       11: 8 home agents interleave
        Default value: 00 = 1 home agent in the system
     */
    UINT32 limit : 20;
    /* limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       
       This is a field that defines the memory region limit. It contains the physical 
       address bit range [45:26]. 0 LessThanEqual Physicaladdress[45:26] LessThanEqual 
       TAD0.limit, when N = 0. TAD[N-1].limit + 1 LessThanEqual Physicaladdress[45:26] 
       LessThanEqual TAD[N].limit, when N = 1 to 11. 
        Note: "-LB Attr" means Ubox message ConfigRegWr can write to this register.
        However ConfigWrLtLock cannot write to this register 
     */
  } Bits;
  UINT32 Data;
} TAD_5_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TAD_6_HA_CFG_REG supported on:                                             */
/*       HSX (0x40190058)                                                     */
/*       BDX (0x40190058)                                                     */
/* Register default value:              0x00000000                            */
#define TAD_6_HA_CFG_REG 0x03004058

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Target Address Decode register
 */
typedef union {
  struct {
    UINT32 channel_0_id : 2;
    /* channel_0_id - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-channel interleaving).
     */
    UINT32 channel_1_id : 2;
    /* channel_1_id - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-channel interleaving).
     */
    UINT32 channel_2_id : 2;
    /* channel_2_id - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-channel interleaving).
     */
    UINT32 channel_3_id : 2;
    /* channel_3_id - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-channel interleaving).
     */
    UINT32 channelways : 2;
    /* channelways - Bits[9:8], RW_LB, default = 2'b00 
       This is a field that defines the number of memory channels interleave within a 
       home agent. 
        
        00: 1 channel way
        01: 2 channel ways
        10: 3 channel ways
        11: 4 channel ways
        Default value: 00b = 1 memory way
     */
    UINT32 socketways : 2;
    /* socketways - Bits[11:10], RW_LB, default = 2'b00 
       This is a filed that defines the number of home agent ways to interleave.
       
       00: 1 home agent (no interleave)
       01: 2 home agents interleave
       10: 4 home agents interleave
       11: 8 home agents interleave
        Default value: 00 = 1 home agent in the system
     */
    UINT32 limit : 20;
    /* limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       
       This is a field that defines the memory region limit. It contains the physical 
       address bit range [45:26]. 0 LessThanEqual Physicaladdress[45:26] LessThanEqual 
       TAD0.limit, when N = 0. TAD[N-1].limit + 1 LessThanEqual Physicaladdress[45:26] 
       LessThanEqual TAD[N].limit, when N = 1 to 11. 
        Note: "-LB Attr" means Ubox message ConfigRegWr can write to this register.
        However ConfigWrLtLock cannot write to this register 
     */
  } Bits;
  UINT32 Data;
} TAD_6_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TAD_7_HA_CFG_REG supported on:                                             */
/*       HSX (0x4019005C)                                                     */
/*       BDX (0x4019005C)                                                     */
/* Register default value:              0x00000000                            */
#define TAD_7_HA_CFG_REG 0x0300405C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Target Address Decode register
 */
typedef union {
  struct {
    UINT32 channel_0_id : 2;
    /* channel_0_id - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-channel interleaving).
     */
    UINT32 channel_1_id : 2;
    /* channel_1_id - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-channel interleaving).
     */
    UINT32 channel_2_id : 2;
    /* channel_2_id - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-channel interleaving).
     */
    UINT32 channel_3_id : 2;
    /* channel_3_id - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-channel interleaving).
     */
    UINT32 channelways : 2;
    /* channelways - Bits[9:8], RW_LB, default = 2'b00 
       This is a field that defines the number of memory channels interleave within a 
       home agent. 
        
        00: 1 channel way
        01: 2 channel ways
        10: 3 channel ways
        11: 4 channel ways
        Default value: 00b = 1 memory way
     */
    UINT32 socketways : 2;
    /* socketways - Bits[11:10], RW_LB, default = 2'b00 
       This is a filed that defines the number of home agent ways to interleave.
       
       00: 1 home agent (no interleave)
       01: 2 home agents interleave
       10: 4 home agents interleave
       11: 8 home agents interleave
        Default value: 00 = 1 home agent in the system
     */
    UINT32 limit : 20;
    /* limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       
       This is a field that defines the memory region limit. It contains the physical 
       address bit range [45:26]. 0 LessThanEqual Physicaladdress[45:26] LessThanEqual 
       TAD0.limit, when N = 0. TAD[N-1].limit + 1 LessThanEqual Physicaladdress[45:26] 
       LessThanEqual TAD[N].limit, when N = 1 to 11. 
        Note: "-LB Attr" means Ubox message ConfigRegWr can write to this register.
        However ConfigWrLtLock cannot write to this register 
     */
  } Bits;
  UINT32 Data;
} TAD_7_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TAD_8_HA_CFG_REG supported on:                                             */
/*       HSX (0x40190060)                                                     */
/*       BDX (0x40190060)                                                     */
/* Register default value:              0x00000000                            */
#define TAD_8_HA_CFG_REG 0x03004060

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Target Address Decode register
 */
typedef union {
  struct {
    UINT32 channel_0_id : 2;
    /* channel_0_id - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-channel interleaving).
     */
    UINT32 channel_1_id : 2;
    /* channel_1_id - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-channel interleaving).
     */
    UINT32 channel_2_id : 2;
    /* channel_2_id - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-channel interleaving).
     */
    UINT32 channel_3_id : 2;
    /* channel_3_id - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-channel interleaving).
     */
    UINT32 channelways : 2;
    /* channelways - Bits[9:8], RW_LB, default = 2'b00 
       This is a field that defines the number of memory channels interleave within a 
       home agent. 
        
        00: 1 channel way
        01: 2 channel ways
        10: 3 channel ways
        11: 4 channel ways
        Default value: 00b = 1 memory way
     */
    UINT32 socketways : 2;
    /* socketways - Bits[11:10], RW_LB, default = 2'b00 
       This is a filed that defines the number of home agent ways to interleave.
       
       00: 1 home agent (no interleave)
       01: 2 home agents interleave
       10: 4 home agents interleave
       11: 8 home agents interleave
        Default value: 00 = 1 home agent in the system
     */
    UINT32 limit : 20;
    /* limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       
       This is a field that defines the memory region limit. It contains the physical 
       address bit range [45:26]. 0 LessThanEqual Physicaladdress[45:26] LessThanEqual 
       TAD0.limit, when N = 0. TAD[N-1].limit + 1 LessThanEqual Physicaladdress[45:26] 
       LessThanEqual TAD[N].limit, when N = 1 to 11. 
        Note: "-LB Attr" means Ubox message ConfigRegWr can write to this register.
        However ConfigWrLtLock cannot write to this register 
     */
  } Bits;
  UINT32 Data;
} TAD_8_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TAD_9_HA_CFG_REG supported on:                                             */
/*       HSX (0x40190064)                                                     */
/*       BDX (0x40190064)                                                     */
/* Register default value:              0x00000000                            */
#define TAD_9_HA_CFG_REG 0x03004064

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Target Address Decode register
 */
typedef union {
  struct {
    UINT32 channel_0_id : 2;
    /* channel_0_id - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-channel interleaving).
     */
    UINT32 channel_1_id : 2;
    /* channel_1_id - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-channel interleaving).
     */
    UINT32 channel_2_id : 2;
    /* channel_2_id - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-channel interleaving).
     */
    UINT32 channel_3_id : 2;
    /* channel_3_id - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-channel interleaving).
     */
    UINT32 channelways : 2;
    /* channelways - Bits[9:8], RW_LB, default = 2'b00 
       This is a field that defines the number of memory channels interleave within a 
       home agent. 
        
        00: 1 channel way
        01: 2 channel ways
        10: 3 channel ways
        11: 4 channel ways
        Default value: 00b = 1 memory way
     */
    UINT32 socketways : 2;
    /* socketways - Bits[11:10], RW_LB, default = 2'b00 
       This is a filed that defines the number of home agent ways to interleave.
       
       00: 1 home agent (no interleave)
       01: 2 home agents interleave
       10: 4 home agents interleave
       11: 8 home agents interleave
        Default value: 00 = 1 home agent in the system
     */
    UINT32 limit : 20;
    /* limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       
       This is a field that defines the memory region limit. It contains the physical 
       address bit range [45:26]. 0 LessThanEqual Physicaladdress[45:26] LessThanEqual 
       TAD0.limit, when N = 0. TAD[N-1].limit + 1 LessThanEqual Physicaladdress[45:26] 
       LessThanEqual TAD[N].limit, when N = 1 to 11. 
        Note: "-LB Attr" means Ubox message ConfigRegWr can write to this register.
        However ConfigWrLtLock cannot write to this register 
     */
  } Bits;
  UINT32 Data;
} TAD_9_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TAD_10_HA_CFG_REG supported on:                                            */
/*       HSX (0x40190068)                                                     */
/*       BDX (0x40190068)                                                     */
/* Register default value:              0x00000000                            */
#define TAD_10_HA_CFG_REG 0x03004068

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Target Address Decode register
 */
typedef union {
  struct {
    UINT32 channel_0_id : 2;
    /* channel_0_id - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-channel interleaving).
     */
    UINT32 channel_1_id : 2;
    /* channel_1_id - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-channel interleaving).
     */
    UINT32 channel_2_id : 2;
    /* channel_2_id - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-channel interleaving).
     */
    UINT32 channel_3_id : 2;
    /* channel_3_id - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-channel interleaving).
     */
    UINT32 channelways : 2;
    /* channelways - Bits[9:8], RW_LB, default = 2'b00 
       This is a field that defines the number of memory channels interleave within a 
       home agent. 
        
        00: 1 channel way
        01: 2 channel ways
        10: 3 channel ways
        11: 4 channel ways
        Default value: 00b = 1 memory way
     */
    UINT32 socketways : 2;
    /* socketways - Bits[11:10], RW_LB, default = 2'b00 
       This is a filed that defines the number of home agent ways to interleave.
       
       00: 1 home agent (no interleave)
       01: 2 home agents interleave
       10: 4 home agents interleave
       11: 8 home agents interleave
        Default value: 00 = 1 home agent in the system
     */
    UINT32 limit : 20;
    /* limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       
       This is a field that defines the memory region limit. It contains the physical 
       address bit range [45:26]. 0 LessThanEqual Physicaladdress[45:26] LessThanEqual 
       TAD0.limit, when N = 0. TAD[N-1].limit + 1 LessThanEqual Physicaladdress[45:26] 
       LessThanEqual TAD[N].limit, when N = 1 to 11. 
        Note: "-LB Attr" means Ubox message ConfigRegWr can write to this register.
        However ConfigWrLtLock cannot write to this register 
     */
  } Bits;
  UINT32 Data;
} TAD_10_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* TAD_11_HA_CFG_REG supported on:                                            */
/*       HSX (0x4019006C)                                                     */
/*       BDX (0x4019006C)                                                     */
/* Register default value:              0x00000000                            */
#define TAD_11_HA_CFG_REG 0x0300406C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Target Address Decode register
 */
typedef union {
  struct {
    UINT32 channel_0_id : 2;
    /* channel_0_id - Bits[1:0], RW_LB, default = 2'b00 
       target channel for channel interleave 0 (used for 1/2/3/4-channel interleaving).
     */
    UINT32 channel_1_id : 2;
    /* channel_1_id - Bits[3:2], RW_LB, default = 2'b00 
       target channel for channel interleave 1 (used for 2/3/4-channel interleaving).
     */
    UINT32 channel_2_id : 2;
    /* channel_2_id - Bits[5:4], RW_LB, default = 2'b00 
       target channel for channel interleave 2 (used for 3/4-channel interleaving).
     */
    UINT32 channel_3_id : 2;
    /* channel_3_id - Bits[7:6], RW_LB, default = 2'b00 
       target channel for channel interleave 3 (used for 4-channel interleaving).
     */
    UINT32 channelways : 2;
    /* channelways - Bits[9:8], RW_LB, default = 2'b00 
       This is a field that defines the number of memory channels interleave within a 
       home agent. 
        
        00: 1 channel way
        01: 2 channel ways
        10: 3 channel ways
        11: 4 channel ways
        Default value: 00b = 1 memory way
     */
    UINT32 socketways : 2;
    /* socketways - Bits[11:10], RW_LB, default = 2'b00 
       This is a filed that defines the number of home agent ways to interleave.
       
       00: 1 home agent (no interleave)
       01: 2 home agents interleave
       10: 4 home agents interleave
       11: 8 home agents interleave
        Default value: 00 = 1 home agent in the system
     */
    UINT32 limit : 20;
    /* limit - Bits[31:12], RW_LB, default = 20'b00000000000000000000 
       
       This is a field that defines the memory region limit. It contains the physical 
       address bit range [45:26]. 0 LessThanEqual Physicaladdress[45:26] LessThanEqual 
       TAD0.limit, when N = 0. TAD[N-1].limit + 1 LessThanEqual Physicaladdress[45:26] 
       LessThanEqual TAD[N].limit, when N = 1 to 11. 
        Note: "-LB Attr" means Ubox message ConfigRegWr can write to this register.
        However ConfigWrLtLock cannot write to this register 
     */
  } Bits;
  UINT32 Data;
} TAD_11_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* HACRDTCNT_HA_CFG_REG supported on:                                         */
/*       IVT_EP (0x40170070)                                                  */
/*       IVT_EX (0x40170070)                                                  */
/*       HSX (0x40190070)                                                     */
/*       BDX (0x40190070)                                                     */
/* Register default value:              0x00000000                            */
#define HACRDTCNT_HA_CFG_REG 0x03004070
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * These registers are used for HA credit initialization and also for debug. They 
 * can be accessed by the BIOS and microcode. This is a special CSR register that 
 * requires the initialization process follows certain rules. 
 * This CSR sets the home tracker schedulers credit counters to specified values. 
 * There are two types of credit counters: private and shared. A private credit 
 * counter consists of two local counters, one for each of the two schedulers in 
 * the home tracker. A shared credit counter consists of three sub-counters: a 
 * global counter, and two local counters (each local counter is private to one of 
 * the two schedulers in the HT). A credit counter is assigned to different types 
 * of messages to ring agents as well as to read and write commands to the memory 
 * controller. This CSR also facilitates the querying of the various counters, and 
 * alters the way credits are shared between the global and local counters. 
 * HA Credit Counter Register Initialization Sequence:
 * Without specific programming, HA uses default values for shared credits, private 
 * credits, and MC credits. HA must be in the idle state (no active or outstanding 
 * transactions) throughout the credit programming sequence. To modify credit 
 * counts after initialization is complete, follow *all* of the steps below. To 
 * first initialize credit counts during HA initialization by system software, BIOS 
 * may begin with step 5 below. 
 * 
 *  1. Set SharCrdtRls to allow all credits to drain to GlblCnt
 *        HaCrdtCnt = 0x00030000;
 *  2. Wait for all credits to drain
 *        wait_ha_drain();
 *  3. Clear SharCrdtEn=SharCrdtEn-1can cause HA to hang... so HA must remain idle 
 * through process 
 *        HaCrdtCnt = 0x00010000;
 *  4. Check that credits are idle:
 *        if((local_cnt0 != 0) || (local_cnt1 !=0) || (glbl_cnt0 != INIT_CNT) || 
 * (glbl_cnt1 != INIT_CNT)) { 
 *          abort_not_idle(); 
 *        }
 *  5. Write credit counter (Shared Credits)
 *        HaCrdtCnt = 0x00050000 + (type << 8) + count;
 *     5a. for example, to program VNA (shared) credits:
 *        HaCrdtCnt = 0x00052800 + CSI0_BL_VNA_CNT;
 *        HaCrdtCnt = 0x00052900 + CSI1_BL_VNA_CNT;
 *        HaCrdtCnt = 0x00052a00 + CSI0_AD_VNA_CNT;
 *        HaCrdtCnt = 0x00052b00 + CSI1_AD_VNA_CNT;
 *        HaCrdtCnt = 0x00052d00 + CSI2_BL_VNA_CNT; (if 3 QPI ports)
 *        HaCrdtCnt = 0x00052e00 + CSI2_AD_VNA_CNT; (if 3 QPI ports)
 *     5b. for example, to program MC RPQ and WPQ (shared) credits:
 *        HaCrdtCnt = 0x00052000 + MC_Ch0_RPQ_CNT;
 *        HaCrdtCnt = 0x00052100 + MC_Ch1_RPQ_CNT;
 *        HaCrdtCnt = 0x00052200 + MC_Ch2_RPQ_CNT;
 *        HaCrdtCnt = 0x00052300 + MC_Ch3_RPQ_CNT;
 *        HaCrdtCnt = 0x00052400 + MC_Ch0_WPQ_CNT;
 *        HaCrdtCnt = 0x00052500 + MC_Ch1_WPQ_CNT;
 *        HaCrdtCnt = 0x00052600 + MC_Ch2_WPQ_CNT;
 *        HaCrdtCnt = 0x00052700 + MC_Ch3_WPQ_CNT;
 *     5c. for example, to program Sbo (shared) credits:
 *        HaCrdtCnt = 0x00052f00 + SBO1_BL_CNT; (if HCC or MCC)
 *        HaCrdtCnt = 0x00053000 + SBO1_AD_CNT; (if HCC or MCC)
 *        HaCrdtCnt = 0x00053100 + SBO2_BL_CNT; (if HCC, or HA0 in MCC)
 *        HaCrdtCnt = 0x00053200 + SBO2_AD_CNT; (if HCC, or HA0 in MCC)
 *  6. Enable shared credit pools and shared credit prefetching
 *        HaCrdtCnt = 0x00020000;
 *  7. Write credit counter (Private Credits)
 *        HaCrdtCnt = 0x00060000 + (type << 8) + count;
 *     7a. for example, to program the number of Cbo IPQ-credited egressing snoops:
 *        HaCrdtCnt = 0x00060d00 + CBO_IPQ_SNP_EGR_CNT;
 *  8. Enable shared credit pools and shared credit prefetching
 *        HaCrdtCnt = 0x00020000;
 *  9. End of period where HA must be idle
 *  
 * Notes for PLI programming:
 *   The read and write operations happen in non-zero time.  PLI macros should wait 
 * for the Sch0RdEn, Sch1RdEn, and WrEn bits to become clear before doing a 
 * subsequent access.  When doing MSG channel accesses, the 1-bit chain will 
 * naturally space accesses far enough apart that this will not be an issue. 
 */
typedef union {
  struct {
    UINT32 maincnt : 6;
    /* maincnt - Bits[5:0], RW_V, default = 6'b000000 
       Main credit counter. For shared credits, this is the global counter. For private 
       credits, this is the only credit counter. 
       On a write operation (WrEn = 1), the value in this field is written into the 
       counter of the specified credit counter. 
       On a read operation (Sch1RdEn = 1 | Sch0RdEn = 1), this field contains the 
       counter value of the specified credit counter. 
     */
    UINT32 prefcnt : 2;
    /* prefcnt - Bits[7:6], RW_V, default = 2'b00 
       This field is only valid when Sch1RdEn or Sch0RdEn is set, and CrdtCnt specifies 
       a shared credit counter. 
       Prefetch credit counter. It is not apply to private credtis. For shared credits, 
       this is the local counter. It only applies to the credit reads. 
       This field is not writeable, i.e. when WrEn=1, this field has no effect.
     */
    UINT32 crdttype : 6;
    /* crdttype - Bits[13:8], RW, default = 6'b000000 
       The HA has two schedulers. Each scheduler uses its own credit pool. The credit 
       type 0-31 decimal are private credit types; The credit type 32-63 decimal are 
       shared credit types. The minimum legal value for each shared credit type is 2, 
       which allows one credit per scheduler; 
       however, if isoch transactions are enabled, the minimum value becomes 4 per 
       shared credit type. 
       The exceptions are the
       shared Sbo BL credit types, whose minimum allowed values are 4, or 8 if isoch 
       transactions are enabled. 
       It is possible for the system to deadlock if a shared credit pool is set lower 
       than its minimum allowed value. 
       Private Credits
       0-3 (000000b-000011b): Write pull buffer credits per MC channel (default 6 each
       channel)
       4-7 (000100b-000111b): Partial write pull credits for MC channel (default 3 each
       channel)
       8 (001000b): BL egress credits (default 4 each direction)
       9 (001001b): AK egress credits (default 7 each direction ... may be increased to 
       8 only if BT is disabled) 
       10 (001010b): AD egress credits (default 6 each direction). This does not 
       include 
       dedicated SNP and NDR credits.
       11-12 (001011b-001100b): Reserved (RSVD)
       13 (001101b): Cbo IPQ-credited egressing snoops (default 4 ... may be increased 
       up to 7)(Set to 3 if Sbo AD credits (type 0x30 & 0x32) are set to 8. Set to 1 if 
       Sbo AD credits (type 0x30 & 0x32) are set to 4 (required for isoc enabled).) 
       14-31 (001110b-011111b): Reserved (RSVD)
       Shared Credits
       32-35 (100000b-100011b): Shared MC read credits per MC channel (default 48 each
       channel)
       36-39 (100100b-100111b): Shared MC write credits per MC channel (default 40
       each channel. When Patrol scrub is enabled, it must be reduced to 39)
       40 (101000b): Shared QPI0 BL VNA credits (default 3 and will be initialized by
       BIOS based on the credit table value)
       41 (101001b): Shared QPI1 BL VNA credits (default 3 and will be initialized by
       BIOS based on the credit table value)
       42 (101010b): Shared QPI0 AD VNA credits for NDR and SNP (default 4 and will be 
       initialized by BIOS based on the credit table value) 
       43 (101011b): Shared QPI1 AD VNA credits for NDR and SNP (default 4 and will be 
       initialized by BIOS based on the credit table value) 
       44 (101100b): Reserved (RSVD)
       45 (101101b): Shared QPI2 BL VNA credits (default 3 and will be initialized by 
       BIOS based on the credit table value) 
       46 (101110b): Shared QPI2 AD VNA credits for NDR and SNP (default 4 and will be 
       initialized by BIOS based on the credit table value) 
       47 (101111b): Shared Sbo1 (first Sbo CW on ring) BL credits (default 4 and will 
       be initialized by BIOS based on the credit table value) 
       48 (110000b): Shared Sbo1 (first Sbo CW on ring) AD credits (default 4 and will 
       be initialized by BIOS based on the credit table value) 
       49 (110001b): Shared Sbo2 (first Sbo CCW on ring) BL credits (default 4 and will 
       be initialized by BIOS based on the credit table value) 
       50 (110010b): Shared Sbo2 (first Sbo CCW on ring) AD credits (default 4 and will 
       be initialized by BIOS based on the credit table value) 
       51-63 (110011b-111111b): Reserved (RSVD)
     */
    UINT32 sch0rden : 1;
    /* sch0rden - Bits[14:14], RW_V, default = 1'b0 
       When set and CrdtCnt specifies a shared credit counter, read scheduler 0's local 
       counter of the specified credit counter and put local counter value into the 
       PrefCnt field. Read global counter of the specified credit counter and put 
       global counter value into the MainCnt field. Scheduler 0's local counter value 
       will be set to 0 and the credit count retuned to global counter. 
       When set and CrdtCnt specifies a private credit counter, read scheduler 0's 
       local counter of the specified credit counter and put local counter value into 
       the MainCnt field. Note that there is no change to the local counter value. 
       PrefCnt field is undefined. 
       0: do not read credit from the scheduler
       1: read credit counter value as explained above
     */
    UINT32 sch1rden : 1;
    /* sch1rden - Bits[15:15], RW_V, default = 1'b0 
       When set and CrdtCnt specifies a shared credit counter, read scheduler 1's local 
       counter of the specified credit counter and put local counter value into the 
       PrefCnt field. Read global counter of the specified credit counter and put 
       global counter value into the MainCnt field. Scheduler 1's local counter value 
       will be set to 0 and the credit count retuned to global counter. 
       When set and CrdtCnt specifies a private credit counter, read scheduler 1's 
       local counter of the specified credit counter and put local counter value into 
       the MainCnt field. Note that there is no change to the local counter value. 
       PrefCnt field is undefined. 
       0: do not read credit from the scheduler
       1: read credit counter value as explained above
     */
    UINT32 sharcrdtrls : 1;
    /* sharcrdtrls - Bits[16:16], RW, default = 1'b0 
       This bit applies to all shared credit counters at once.
       When set, prevents schedulers from speculatively allocating shared credits in 
       the local credit counter. This causes the idle state of the local credit counter 
       to be zero. All credits in the local counters are returned to their respective 
       global counter making the local counters zero. 
       When cleared, shared credits are pre-allocated to both schedulers' local 
       counters, allowing lower latency. 
       1: prevents the local counters of all shared credit counters from obtaining 
       credits from their respective global counters; 
       0: allows speculative pre-allocate of the local credit counters from global 
       credit counter to reduce the latency 
       To program shared credit counts, this ShrCrdtRls bit must be set first, then a 
       short delay (message channel + HA 1-bit chain latency should be sufficient), 
       then ShrCrdtEn must be cleared, then credit counts may be programmed. Of course, 
       HA must be idle during this time. 
       Or, credits may be programmed if ShrCrtdEn has never been set since the last 
       reset. 
     */
    UINT32 sharcrdten : 1;
    /* sharcrdten - Bits[17:17], RW, default = 1'b0 
       This bit affects all shared credit counters at once.
       When set, activate global counters of all shared credit counters. HA allows 
       scheduler to shared credits between the global credit counter to the local 
       credit counters. 
       1: allows to share credits between the global counter and local counter
       0: does not allow sharing of the credits between the global counter and local 
       counter. i.e. global counters do not send credit to their local counters. 
       When written, this bit applies to ALL shared credit counters. Note, during 
       credit counter programming, there are timing restrictions of when this bit can 
       be reset. See ShrCrdtRls. 
     */
    UINT32 wren : 1;
    /* wren - Bits[18:18], RW_V, default = 1'b0 
       When set, writes MainCnt field into shared or private credit counter as 
       specified by CrdtCtr. Software must ensure that credits are in idle state (all 
       credit returned) when writing the credit count. When writing to a private credit 
       counter, the MainCnt field is written into both credit counters (scheduler 0 and 
       scheduler 1's). For shared credits, only the global count is written. Software 
       must ensure that Local Credit counter is zero when doing the write by setting 
       sharCrdtRls prior to doing the write. 
       1: Write to schedulers by using MainCnt value
       0: Do not write to scheduler counts
       Only one of the bits: Sch1RdEn, Sch0RdEn, or WrEn can be set for any write to 
       this CSR. 
     */
    UINT32 rsvd : 13;
    /* rsvd - Bits[31:19], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HACRDTCNT_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* HTBASE_HA_CFG_REG supported on:                                            */
/*       IVT_EP (0x40170074)                                                  */
/*       IVT_EX (0x40170074)                                                  */
/*       HSX (0x40190074)                                                     */
/*       BDX (0x40190074)                                                     */
/* Register default value:              0x00000000                            */
#define HTBASE_HA_CFG_REG 0x03004074
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Each home agent has 7 bits mapping to the assigned HT entry starting address. 
 * The upper 4-bits are in the HtBase register and the lower 3-bits are in the 
 * HtBaseLow register. Previous processors only had the HtBase register since each 
 * home agent is assigned RTIDs that are multiples of 8. This restriction has been 
 * relaxed to allow arbitrary RTID allocation and hence the need to add HtBaseLow  
 * to carry the lower 3-bits of the starting HT entry. The RTID of an incoming 
 * transaction from a specific socket is added to the 7-bit HT entry starting 
 * address for that socket to identify the HTID for the incoming transaction. i.e. 
 * the HTID of a request is simply the concatenation {Nid*HtBase[3:0], 
 * Nid*HtBaseLow[2:0]} + requestRTID. Note that the HTSwizzle bit in HaSysDefeatuer 
 * could modify the HTID to distribute them evenly between the two HA schedulers. 
 * This register is used only when the BT is disabled. It is ignored when the BT is 
 * enabled. Note that NID in this register is the socket NID. When extended RTID 
 * mode is enabled, either a local socket or remote socket in a 2S configuration is 
 * assigned 2 NIDs. This is used in ISOC configurations. 
 * 
 * Even though we have HTBase fields up to Nid7, the only ones that would ever be 
 * need to be used are Nid0, Nid1, Nid4, and Nid5.  This is because only 1S and 2S 
 * (non-XNC) configs would ever have BT disabled, and the assumption is that there 
 * would be no randomization of the socketIDs in 1S/2S (i.e. we would not use 
 * local=6 and remote=7 in 2S config, instead we always use socketIDs 0 and 1).  
 * Nid4 and Nid5 would be used for legacy extended RTIDs, i.e. Nid4 == extended 
 * RTIDs for Nid0, and Nid5==extended RTIDs for Nid1. 
 * 
 * An RTID out of range error is signaled if the HTBASE + incoming_RTID for any NID 
 * is greather than 127 
 */
typedef union {
  struct {
    UINT32 nid0htbase : 4;
    /* nid0htbase - Bits[3:0], RW_LB, default = 4'b0000 
       NID0 HT Base (Nid0HtBase): This field defines the HTID mapping.
       The base identifies the first entry of HTID allocated for this Node ID. It is 
       calculated as (Nid0HtBase,Nid0HtBaseLow). The HTID entry address of node 0 is 
       statically allocated at NID0HtBase,NID0HtBaseLow. 
     */
    UINT32 nid1htbase : 4;
    /* nid1htbase - Bits[7:4], RW_LB, default = 4'b0000 
       NID1 HT Base (Nid1HtBase): This field defines the HTID mapping.
       The base identifies the first entry of HTID allocated for this Node ID. It is 
       calculated as (Nid1HtBase,Nid1HtBaseLow). The HTID entry address of node 1 is 
       statically allocated at NID1HtBase,NID1HtBaseLow. 
     */
    UINT32 nid2htbase : 4;
    /* nid2htbase - Bits[11:8], RW_LB, default = 4'b0000 
       NID2 HT Base (Nid2HtBase): This field defines the HTID mapping.
       The base identifies the first entry of HTID allocated for this Node ID. It is 
       calculated as (Nid2HtBase,Nid2HtBaseLow). The HTID entry address of node 2 is 
       statically allocated at NID2HtBase,NID2HtBaseLow. 
     */
    UINT32 nid3htbase : 4;
    /* nid3htbase - Bits[15:12], RW_LB, default = 4'b0000 
       NID3 HT Base (Nid3HtBase): This field defines the HTID mapping.
       The base identifies the first entry of HTID allocated for this Node ID. It is 
       calculated as (Nid3HtBase,Nid3HtBaseLow). The HTID entry address of node 3 is 
       statically allocated at NID3HtBase,NID3HtBaseLow. 
     */
    UINT32 nid4htbase : 4;
    /* nid4htbase - Bits[19:16], RW_LB, default = 4'b0000 
       NID4 HT Base (Nid4HtBase): This field defines the HTID mapping.
       The base identifies the first entry of HTID allocated for this Node ID. It is 
       calculated as (Nid4HtBase,Nid4HtBaseLow). The HTID entry address of node 4 is 
       statically allocated at NID4HtBase,NID4HtBaseLow. 
     */
    UINT32 nid5htbase : 4;
    /* nid5htbase - Bits[23:20], RW_LB, default = 4'b0000 
       NID5 HT Base (Nid5HtBase): This field defines the HTID mapping.
       The base identifies the first entry of HTID allocated for this Node ID. It is 
       calculated as (Nid5HtBase,Nid5HtBaseLow). The HTID entry address of node 5 is 
       statically allocated at NID5HtBase,NID5HtBaseLow. 
     */
    UINT32 nid6htbase : 4;
    /* nid6htbase - Bits[27:24], RW_LB, default = 4'b0000 
       NID6 HT Base (Nid6HtBase): This field defines the HTID mapping.
       The base identifies the first entry of HTID allocated for this Node ID. It is 
       calculated as (Nid6HtBase,Nid6HtBaseLow). The HTID entry address of node 6 is 
       statically allocated at NID6HtBase,NID6HtBaseLow. 
     */
    UINT32 nid7htbase : 4;
    /* nid7htbase - Bits[31:28], RW_LB, default = 4'b0000 
       NID7 HT Base (Nid7HtBase): This field defines the HTID mapping.
       The base identifies the first entry of HTID allocated for this Node ID. It is 
       calculated as (Nid7HtBase,Nid7HtBaseLow). The HTID entry address of node 7 is 
       statically allocated at NID7HtBase,NID7HtBaseLow. 
     */
  } Bits;
  UINT32 Data;
} HTBASE_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* MCCRDTTHRLD_HA_CFG_REG supported on:                                       */
/*       IVT_EP (0x40170078)                                                  */
/*       IVT_EX (0x40170078)                                                  */
/*       HSX (0x40190078)                                                     */
/*       BDX (0x40190078)                                                     */
/* Register default value:              0x44882121                            */
#define MCCRDTTHRLD_HA_CFG_REG 0x03004078
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * The McCrdtThld register is thresholds for ISOC transactions from home agent to 
 * memory controller. The critical priority threshold is the threshold of the 
 * number of credit reserved for the critical priority iosch transactions from the 
 * home agent to the memory controller. For example, when there are multiple 
 * outstanding memory read requests at the home agent, the critical priority read 
 * transaction will have the highest priority to use the memory credits. The home 
 * agent will issue the critical memory read transactions first. If the number of 
 * memory read credits larger than the critical read threshold, then the high 
 * priority read transaction can be issued by the home agent. A normal read 
 * transaction can be issued to memory controller when the number of the memory 
 * read credits is larger than the high priority credit threshold. 
 *  In general, we assume the iosch requester guarantees there is no very long 
 * period of isoc transactions to cause other transactions to be starved at home 
 * agent. The critical read threshold is smaller than the high priority read 
 * threshold; the critical write threshold is smaller than the high priority write 
 * threshold. The threshold at all memory channels are identical. The threshold 
 * number is programmable.  If ISOCH is disabled, the Crit and High priority 
 * thresholds should be programmed to 0 to allow for max number of credits to be 
 * given to normal requests. 
 */
typedef union {
  struct {
    UINT32 critwr : 4;
    /* critwr - Bits[3:0], RW, default = 4'b0001 
       The field defines the critical priority write transaction credit threshold.
     */
    UINT32 highwr : 4;
    /* highwr - Bits[7:4], RW, default = 4'b0010 
       The field defines the high priority write transaction credit threshold.
     */
    UINT32 critrd : 4;
    /* critrd - Bits[11:8], RW, default = 4'b0001 
       The field defines the critical priority read transaction credit threshold.
     */
    UINT32 highrd : 4;
    /* highrd - Bits[15:12], RW, default = 4'b0010 
       The field defines the high priority read transaction credit threshold.
     */
    UINT32 wrbgfthresh : 4;
    /* wrbgfthresh - Bits[19:16], RW, default = 4'b1000 
       The filed defines the write to BGF threshold to prevent over load the BGF.
     */
    UINT32 rsvd : 1;
    /* rsvd - Bits[20:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 updtbgfthreshold : 3;
    /* updtbgfthreshold - Bits[23:21], RW, default = 3'b100 
       It defines the cancel cmd bgf
       threshold to prevent over load to cancel cmd bgf. The threshold number depends
       on the bgf and target configuration.
       Note: The cmd and cancel go through the different bgfs. If the cancel cmd 
       reaches 
       the MC before MC receives the read/write cmd, the MC will send the no-cancel ack
       back to HA. If the cmd is already issued to DRAM and can not be cancelled, the 
       MC 
       will issue the no-cancel back to HA as well.
     */
    UINT32 cntlbgfthreshold : 4;
    /* cntlbgfthreshold - Bits[27:24], RW, default = 4'b0100 
       It defines the control bgf threshold to prevent over load to bgf. The threshold 
       number depends on the bgf and target configuration. 
     */
    UINT32 mcbypassbgfthreshold : 4;
    /* mcbypassbgfthreshold - Bits[31:28], RW, default = 4'b0100 
       The HA to MC BGF threshold is used for scheduling MC request in bypass 
       condition. 
     */
  } Bits;
  UINT32 Data;
} MCCRDTTHRLD_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* HAVNACRDT_HA_CFG_REG supported on:                                         */
/*       IVT_EP (0x4017007C)                                                  */
/*       IVT_EX (0x4017007C)                                                  */
/*       HSX (0x4019007C)                                                     */
/*       BDX (0x4019007C)                                                     */
/* Register default value:              0x00000002                            */
#define HAVNACRDT_HA_CFG_REG 0x0300407C


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.0.CFG.xml.
 * Register used to control credits flow from home agent to Intel QPI targeting the 
 * ring. The credit is going to update the counters in the Intel QPI Agent, so 
 * traffic must not be flowing from the HA Agent when HA does not have an Intel QPI 
 * credit.Initialization of VNA credit register: 
 * - The HA hardware provides 1 VNA credit for BL and 1 VNA credit for AD at reset 
 * time before BIOS initialization. This arrangement is to avoid a potential 
 * deadlock; 
 * - The HA hardware sets the VNA thresholds of BL and AD credits are zeros;
 * - VNA credit register should be initialized before BIOS initializing the memory 
 * configuration. This will effectively give BIOS more BW if BIOS accesses the 
 * remote socket memory for BIOS initialization; 
 * - The socket should only have configuration cycles (including MC configuration 
 * cycles), not memory transactions, until all these are done by BIOS. 
 * Due to CBO RITD usage (no remote socket activity at the beginning of 
 * initialization), it is desirable to let BIOS program the remote socket fist, and 
 * then the BIOS configures the lock socket. 
 * It is preferable to have the ring idle during the VNA credit initialization.
 * The Intel QPI protocol suggests the threshold for ISOC transactions DP 
 * configuration. 
 * Non-legacy socket:
 * - HA has 8 VNA per link after BIOS VNA credit register initialization.
 * - The IOSCH credit threshold is 4.
 * Legacy socket:
 * - HA has the total 4 VNA credits per link. 
 * - All isoc data response are return to local IIO. No isoch QPI credit is needed.
 */
typedef union {
  struct {
    UINT32 snpipqcreditcnt : 3;
    /* snpipqcreditcnt - Bits[2:0], RW, default = 3'b010 
       This field indicates how many IPQ credits per Cbo the HA has in order to send 
       local snoops. When local snoop bouncing is disabled, 
       HA must have an IPQ credit available in order to send a snoop to a particular 
       Cbo. If bouncing is enabled, HA will only use IPQ credit if the slot 
       it is injecting on is an ADS slot.
       By default, there are 2 IPQ credits per Cbo. Max of 4 credits per Cbo. Must be 
       careful not to over subscribe credits when bouncing is disabled. Since there is 
       no specific write enable to the credit counters, this value is written into the 
       credit counters on the arrival of the first AD egress message into HA AD egress 
       queues. 
       Note: ISOCH is only supported on the EN and EP processor in 1S or 2S 
       configurations. It is not supported on the EX processor. Not all bits in this 
       register are ISOCH register fields. 
     */
    UINT32 rsvd_3 : 5;
    /* rsvd_3 - Bits[7:3], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 advnahighthrshld : 4;
    /* advnahighthrshld - Bits[11:8], RW, default = 4'b0000 
       It is the IOSCH threshold. When the credit availability level is below than the 
       threshold, these credits are reserved for critical and high priority 
       transactions. The high priority credit threshold is great or equal to the 
       critical priority credit threshold level.Default 000b = zero high priority 
       credit threshold 
       If ISOCH is disabled, the Crit and High priority thresholds should be programmed 
       to 0 to allow for max number of credits to be given to normal requests. 
     */
    UINT32 advnacriticalthrshld : 4;
    /* advnacriticalthrshld - Bits[15:12], RW, default = 4'b0000 
       It is the IOSCH
       critical priority credit threshold. When the credit availability level is below 
       than the 
       threshold, these credits are reserved for critical priority transaction.
       Default 000b = zero critical priority credit threshold.
       If ISOCH is disabled, the Crit and High priority thresholds should be programmed 
       to 0 to allow for max number of credits to be given to normal requests. 
     */
    UINT32 rsvd_16 : 8;
    /* rsvd_16 - Bits[23:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 blvnacriticalthrshld : 4;
    /* blvnacriticalthrshld - Bits[27:24], RW, default = 4'b0000 
       It is the IOSCH
       critical priority credit threshold. When the credit availability level is below 
       than the 
       threshold, these credits are reserved for critical priority transaction.
       Default 000b = zero critical priority credit threshold.
       If ISOCH is disabled, the Crit and High priority thresholds should be programmed 
       to 0 to allow for max number of credits to be given to normal requests. 
     */
    UINT32 blvnahighthrshld : 4;
    /* blvnahighthrshld - Bits[31:28], RW, default = 4'b0000 
       It is the IOSCH threshold. When the credit availability level is below than the 
       threshold, these credits are reserved for critical and high priority 
       transactions. The high priority credit threshold is great or equal to the 
       critical priority credit threshold level. Default 000b = zero high priority 
       credit threshold 
       If ISOCH is disabled, the Crit and High priority thresholds should be programmed 
       to 0 to allow for max number of credits to be given to normal requests. 
     */
  } Bits;
  UINT32 Data;
} HAVNACRDT_HA_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* HASYSDEFEATURE_HA_CFG_REG supported on:                                    */
/*       IVT_EP (0x40170080)                                                  */
/*       IVT_EX (0x40170080)                                                  */
/*       HSX (0x40190080)                                                     */
/*       BDX (0x40190080)                                                     */
/* Register default value on IVT_EP:    0x01208202                            */
/* Register default value on IVT_EX:    0x01208202                            */
/* Register default value on HSX:       0x01208002                            */
/* Register default value on BDX:       0x01208002                            */
#define HASYSDEFEATURE_HA_CFG_REG 0x03004080
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Miscellaneous control and enable bits for various system features.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 1;
    UINT32 htswizzle : 1;
    /* htswizzle - Bits[1:1], RW, default = 1'b1 
       This filed defines the enabling mode of HT swizzle mode.
       1 means the HT Swizzle mode is enabled (default)
       0 means the mode is disabled.
       
       Note: Swizzle mode allows the HTID to be mapped so that the consecutive sections 
       are assigned to alternative scheduler in HA. 
     */
    UINT32 rsvd_2 : 2;
    UINT32 alwaysfrcack : 1;
    /* alwaysfrcack - Bits[4:4], RW, default = 1'b0 
       When it sets, HA supports Frc_ack transaction by always sending FrcAckCnflct 
       prior to Cmp or DataC_Cmp. 
       1: Enable
       0: Disable (default)
     */
    UINT32 mirrormod : 2;
    /* mirrormod - Bits[6:5], RW_L, default = 2'b00 
       Current IMC and HA assume that memory channel naming convention is channel 
       0,1,2,3 
       00: Disable Channel Mirroring mode (POR independent channel mode)
       01: Channel 0/1 and 2/3 mirroring (EP, EP 4S processor)
       10: Channel 0/2, 1/3 mirroring (EX processor)
       11: Reserved
       Mirroring not supported on the EN processor.
       Lockstep and mirroring cannot be enabled at the same time on the EP, EP 4S 
       processors. 
       Lockstep and mirroring can be enabled at the same time on the EX processor.
     */
    UINT32 lockstep_en : 1;
    /* lockstep_en - Bits[7:7], RW_L, default = 1'b0 
       0=disble lockstep mode
       1=Enable lockstep mode
     */
    UINT32 rsvd_8 : 1;
    UINT32 retrythreshold : 2;
    /* retrythreshold - Bits[10:9], RW, default = 2'b00 
       This sets the maximum retries HA should perform before giving up. The MC Intel 
       SMI retry flow on EX platforms relies on a minimum number of retries 
       (programmable in MC). Both fields need to be programmed with a similar value as 
       to be compatible. 
       The number of retries is determine by values of the fields RetryForever, 
       RetryThreshold: 
       0,0 = 2 retries after original read (POR behavior)......
       0,1 = 4 retries after original read (legacy).
       0,2 = 8 retries after original read.
       0,3 = The mirror mode and configuration are supported.
       14 retries after original read.
       1,0 = retry forever, even after Intel SMI (EX platforms) retry done (Legacy 
       error flow debug). 
       1,3 = retry forever till MC Intel SMI (EX platforms) retry kicks in (for error 
       flow debug). 
     */
    UINT32 enjkterrorflow : 1;
    /* enjkterrorflow - Bits[11:11], RW, default = 1'b0 
       The Legacy error flow would select an almost arbitrary htid amongst the htids 
       with errors. The default flow will retry the first error htid per channel first. 
       This is a requirement for Intel SMI 2 retry on EX processors. 
       0= Default error flow
       1= Legacy error flow. Incompatible with Intel SMI 2 on EX processors.
     */
    UINT32 hadbparitydis : 1;
    /* hadbparitydis - Bits[12:12], RW, default = 1'b0 
       0 = Enable HADB parity check (default);
       1 = Disable HADB parity check
     */
    UINT32 haretryrddis : 1;
    /* haretryrddis - Bits[13:13], RW, default = 1'b0 
       0=Enable HA retry memory read; 1= Disable HA retry memory read
       The current implementation does not either capture the retry address or assert a 
       separated error record. Therefore, this HA feature is redundant to iMC ECC error 
       disable. The post silicon team agreed to disable this feaure. It means we should 
       not set this bit (not touch this bit). 
     */
    UINT32 retryforever : 1;
    /* retryforever - Bits[14:14], RW, default = 1'b0 
       It allows the HA retry the memory channel forever when an memory read error 
       occurs. It is used for the testing and debugging.0: HA does not allow retry 
       forever (default) 
       1: HA allows to retry memory channel forever
     */
    UINT32 nonsnpsetdir : 1;
    /* nonsnpsetdir - Bits[15:15], RW, default = 1'b1 
       This bit is used for preserving the directory bit when the HA receives a 
       nonSnpWr. 
       0: The home directory bits are preserved at DRAM by read-modify-write operation. 
       (Not supported) 
       1: HA sets the directory bit and writes back to DRAM without snooping (default)
       
       Important HSX Note:
       Always keep this bit at the default value. Do not modify this bit since this 
       functionality is no longer supported. 
       The default option reduces the memory bandwidth and latency when HA receives a 
       nonsnpwr transaction. 
     */
    UINT32 dirupdate2sopten : 1;
    /* dirupdate2sopten - Bits[16:16], RW_LB, default = 1'b0 
       When set, 2S directory optimization is enabled, which eliminates directory 
       update for A->I transition, keeping the directory in A. 
       This is a performance optimization to reduce the number of memory directory 
       updates trading off Intel QPI snoop traffic. 
       0=2S Directory update optimization is not enabled (default);
       1=2S Directory update optimization is enabled (Not supported).
       Note:
       2S Directory update optimization should only be enabled in 2S configurations 
       with 2-bit directory enabled. 
       
       Important HSX Note:
       Always keep this bit at the default value. Do not modify this bit since this 
       functionality is no longer supported. 
     */
    UINT32 alwaysflocalen : 1;
    /* alwaysflocalen - Bits[17:17], RW_LB, default = 1'b0 
       When set, HA always sends DATA_C_F for local RdData/RdCode when directory state 
       indicates Shared. 
       This is a performance optimization in directory mode.
       1=sends DATA_C_F
       0=sends DATA_C_FS
       
       Important HSX Note:
       Always keep this bit at the default value. Do not modify this bit since this 
       functionality is no longer supported. 
     */
    UINT32 conddisablef : 1;
    /* conddisablef - Bits[18:18], RW_LB, default = 1'b0 
       When set, Cbo conditionally downgrades from F to S on remote non-ownership 
       snoop. 
       This is a performance optimization in directory mode. It will enable (when 
       CondDisableF=1) the EDR (Early Data Return) performance optimization for Intel 
       QPI RdCode/RdData when directory is shared. 
       1=enable Cbo conditional downgrade
       0=disable Cbo conditional downgrade
       Note:
       This bit should be set to the same value as the corresponding bit in the Cbo. It 
       is just an indicator of the Cbo CSR bit in the HA. 
       
       Important HSX Note:
       Always keep this bit at the default value. Do not modify this bit since this 
       functionality is no longer supported. 
     */
    UINT32 disablemcbyps : 1;
    /* disablemcbyps - Bits[19:19], RW, default = 1'b0 
       When it sets, HA to MC command cannot take the bypass pipeline at HA to the BGF.
       0=enable MC bypass (default);
       1=disable MC bypass;
     */
    UINT32 enablehd : 1;
    /* enablehd - Bits[20:20], RW_LB, default = 1'b0 
       This bit controls whether HA enables its Home Directory:
       0 = disable directory (for 2-socket isoch);
       1 = enable directory (all other configurations).
       Enabling the directory automatically disables critical-chunk data returns (see 
       CritChunkRetDisable in HaSysDefeature2), and always returns chunk 0 first. 
       This feature is not enabled on all processors.
     */
    UINT32 disablees : 1;
    /* disablees - Bits[21:21], RW_LB, default = 1'b1 
       This bit controls whether HA supports early snoop in a 2-socket isoch system:
       0: enable Early Snoop (for 2-socket isoch only);
       1: Disable Early Snoop (all other configurations).
       
       Note:
       This bit must be set for all system configurations greater than 2-socket.
     */
    UINT32 disabledatamigratory : 1;
    /* disabledatamigratory - Bits[22:22], RW, default = 1'b0 
       When clear, HA supports the data migratory request. Otherwise, HA treats data 
       migratory request as data request. 
       1=disable data migratory
       0=enable data migratory
       
       Note:
       RdDataMigratory can be generated by both xNC and processor Cbos. Processor Cbos 
       generate this request when MPrime is enabled. 
     */
    UINT32 singlebitdir : 1;
    /* singlebitdir - Bits[23:23], RW_LB, default = 1'b0 
       When set, HA supports 1bit directory scheme.
       Note that this mode is supported only on native DDR configs.
       1=enable 1bit directory scheme (Not supported on the EX processor)
       0=enable 2bit directory scheme
     */
    UINT32 active_node_mask_bit_7 : 8;
    /* active_node_mask_bit_7 - Bits[31:24], RW_LB, default = 8'b00000001 
       NodeMask[7:0]: This field defines an node ID for all the unique caching agents 
       in the system. It is used to control which node snoops are spawned to and 
       responses are expected from. 
       Node0 = 00000001b
       Node1 = 00000010b
       Node2 = 00000100b
       Node3 = 00001000b
       Node4 = 00010000b
       Node5 = 00100000b
       Node6 = 01000000b
       Node7 = 10000000b
       Default: 01h = only local socket node ID.
     */
  } Bits;
  UINT32 Data;
} HASYSDEFEATURE_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* HASYSDEFEATURE2_HA_CFG_REG supported on:                                   */
/*       IVT_EP (0x40170084)                                                  */
/*       IVT_EX (0x40170084)                                                  */
/*       HSX (0x40190084)                                                     */
/*       BDX (0x40190084)                                                     */
/* Register default value on IVT_EP:    0x21000B88                            */
/* Register default value on IVT_EX:    0x21000B88                            */
/* Register default value on HSX:       0x01200B88                            */
/* Register default value on BDX:       0x01200B88                            */
#define HASYSDEFEATURE2_HA_CFG_REG 0x03004084


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.0.CFG.xml.
 * Additional miscellaneous control and enable bits for various system features.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 3;
    UINT32 rspbypassdisable : 1;
    /* rspbypassdisable - Bits[3:3], RW, default = 1'b1 
       The HA has the latency optimization for response bypass control.
       This bit can be used to disable this latency optimization in case of debug.
       This optimization is disabled by default since thei directory mode bypass 
       optimization is used when directory is enabled. 
       Both RspByp and DirByp should not be enabled together.
       Response bypass latency optimization is expected to be enabled in the 2S ISOC
       configuration in which directory mode is disabled.
       0 = Enable response bypass latency reduction optimization
       1 = Disable response bypass latency optimization (default)
     */
    UINT32 rsvd_4 : 1;
    /* rsvd_4 - Bits[4:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_5 : 3;
    UINT32 mirrorsrb : 1;
    /* mirrorsrb - Bits[8:8], RW, default = 1'b1 
       HA writes the correct data, that is read from the secondary channel, back to the 
       primary channel when the primary channel read data can not be ECC corrected. The 
       HA will issue a read to the write data to check if this write scrub successful. 
       If it is, the HA error handler return the valid data and exits; if it is not 
       successful, the HA error handler disable the primary channel, returns the valid 
       data, and exit. It is only used in the mirroring channel configurations. 
       
       0: Mirror Scrubbing disable
       1: Mirror Scrubbing enable (default)
     */
    UINT32 demandsrb : 1;
    /* demandsrb - Bits[9:9], RW, default = 1'b1 
       HA writes the corrected data back to the primary channel (or independent 
       channel) based on MC demanding. 
       1: HA Error handler Demand Scrub function is enabled (default)
       0: HA Error Handler Demand Scrub function is disabled
     */
    UINT32 isochen : 1;
    /* isochen - Bits[10:10], RW, default = 1'b0 
       0: Disable Isoch (default)
       1: Enable Isoch
     */
    UINT32 rsvd_11 : 1;
    UINT32 locdirbypassdisable : 1;
    /* locdirbypassdisable - Bits[12:12], RW, default = 1'b0 
       The HA has a latency optimization for directory mode in cases where the 
       directory info returned from memory=I. This bit can be used to disable this 
       latency optimization for local transactions. 
       0 = Enable directory bypass latency reduction
       1 = Disable directory bypass latency optimization
     */
    UINT32 rmtdirbypassdisable : 1;
    /* rmtdirbypassdisable - Bits[13:13], RW, default = 1'b0 
       The HA has a latency optimization for directory mode in cases where the 
       directory info returned from memory=I. This bit is used to enable this latency 
       optimization for remote transactions. 
       0 = Enable directory bypass latency reduction optimization
       1 = Disable directory bypass latency optimization
     */
    UINT32 iodcen : 1;
    /* iodcen - Bits[14:14], RW, default = 1'b0 
       Enable IODC in the HA
       0: Disable Iodc (default)
       1: Enable Iodc
     */
    UINT32 forcenonsnpordering : 1;
    /* forcenonsnpordering - Bits[15:15], RW, default = 1'b0 
       0: The Nonsnp* transactions will not go through the ordering fifo for normal 
       priority (default). 
       1: The NonSnp* transactions (even to different addresses) are kept in strict 
       order w.r.t. the order they enter the home agent. 
       This bit only impact normal priority NonSnp* transactions. They will be forced 
       through an ordering fifo (for debug; legacy behavior). 
       Migration slave HA will need to set this bit to ensure memory consistency 
       between master and slave HA. 
     */
    UINT32 cboidsnoopintlv : 1;
    /* cboidsnoopintlv - Bits[16:16], RW_LB, default = 1'b0 
       In 2 Socket config w/ 2 Intel QPI links between the 2 sockets and w/ early snoop 
       disabled, 
       we use CboID[0] to pick the Intel QPI link to route snoops to remote agent when 
       there is only 1 HA. If there are 
       2 HAs, then the HAs can either route based on Intel QPI route vector or use 
       CboID[0] (HA0 interleaves on CboID[0], HA1 interleaves on inverse of CboID[0]). 
       This bit enables the use of CboID interleave. Must be set for 2S and Early Snoop 
       disabled in order 
       for Home snoop to work with Dynamic Routeback Table. Should not be set in any 
       config that is not 2S or 2S+XNC with 2 Intel QPI links between 2 sockets. 
       When CboIdSnoopIntlv = 1:
       CboID[0] = 0, HA0 route to QPI0
       CboID[0] = 1, HA0 route to QPI1
       CboID[0] = 0, HA1 route to QPI1
       CboID[0] = 1, HA1 route to QPI0
       Although the Intel QPI route vector does not indicate where to route snoops when 
       CboIdSnoopIntlv is enabled, the Intel QPI route vector MUST be programmed to 0x2 
       for the XNC socket behind Intel QPI link 2 in 2S 2link+XNC configs. This is 
       necessary in order for 
       the DNID of the snoop to be properly calculated when heading for the XNC. Under 
       this mode, only 1 snoop can be sent to the XNC, even if there are two nodes 
       behind it so snoop fanout should be enabled in that case. 
       
       When CboIdSnoopIntlv = 0:
       Route snoops based on Intel QPI route vector or Snoop Fanout table if snoop 
       fanout is enabled 
     */
    UINT32 cnfltlatoptdisable : 1;
    /* cnfltlatoptdisable - Bits[17:17], RW, default = 1'b0 
       0: Enable conflict flow latency optimization (default).
       Will result in snooping and speculative read to memory in parallel
       when transaction wakes up from conflict chain.
       1: Disable conflict flow latency optimization (for debug).
       When transaction in conflict chain wakes up, snoops are sent out, and
       read to memory is only started after receiving all snoop responses.
     */
    UINT32 gateiodcallocwithosb : 1;
    /* gateiodcallocwithosb - Bits[18:18], RW, default = 1'b0 
       Gate IODC allocation with OSB heuristic
       0: Disable gating of IODC allocation with OSB heuristic (default)
       1: Enable gating of IODC allocation with OSB heuristic
     */
    UINT32 adsrcthrottle : 2;
    /* adsrcthrottle - Bits[20:19], RW, default = 2'b00 
       Enables and defines the threshold for throttling back the AD snoop and remote 
       Cmp traffic from being sent onto the ring in HA. When this threshold is 
       non-zero, HA will monitor the global throttle wire, and when asserted, will 
       block AD egress traffic from being sent out onto the ring. The following are the 
       thresholds we allow the egress 
       to be blocked:
       00 = Source Throttle disabled
       01 = allow sending on every other ring slot on a given polarity/direction
       10 = allow sending on every 4 ring slots on given polarity/direction
       11 = allow sending on every 8 ring slots on a given polarity/direction
     */
    UINT32 mcchanhashen : 1;
    /* mcchanhashen - Bits[21:21], RW_LB, default = 1'b1 
       0=disable MC Channel hash mode;
       1=enable MC Channel hash mode.
       Note: Fixed address bits are chosen for the hash while generating the channel 
       select. 
       This is a performance optimization expected to distribute the accesses more 
       evenly between DDR channels. 
     */
    UINT32 mcchanshiftupenable : 1;
    /* mcchanshiftupenable - Bits[22:22], RW_LB, default = 1'b0 
       When it is set, address bit 6 will not be used for socket and channel select, 
       enabling 2 consecutive cachelines to target the same memory channel. 
       This is a performance optimization expected to be enabled on native DDR 
       configurations. Must be set if Directory Cache is enabled (if supported on SKU). 
       Note that there are parallel configuration bits in the Cbo and memory controller 
       that have to also be set. 
       0=disable MC Channel Shift Up mode;
       1=enable MC Channel Shift Up mode.
     */
    UINT32 rsvd_23 : 1;
    UINT32 snpearlyarbdisable : 1;
    /* snpearlyarbdisable - Bits[24:24], RW, default = 1'b1 
       The HA has an optimization for scheduling snoops under IDLE pipeline conditions, 
       in which a few cycles are saved by allowing an HT entry to arbitrate early to 
       send out a snoop. This is fine when only 1 snoop needs to be sent out, however, 
       if the config is > 2S, where an HT needs to send multiple snoops, subsequent 
       snoops after the first 
       one end up getting penalized and overall latency is actually increased for the 
       subsequent snoops. Use this to disable this optimization for > 2S configs. 
       0 = Enable optimization
       1 = Disable optimization
     */
    UINT32 switchqpi0vs1 : 1;
    /* switchqpi0vs1 - Bits[25:25], RW_LB, default = 1'b0 
       In 2 Socket config w/ early snoop disabled,
       we use CboID[0] to route snoops to remote agent when there is only 1 HA. If 
       there are 
       2 HAs, then the HAs can either route based on Intel QPI route vector or use 
       CboID[0] (HA0 interleaves on CboID[0], HA1 interleaves on inverse of CboID[0]). 
       When CboID interleave is enabled (CboIdSnoopIntlv = 1), this bit switches the 
       interleave between the two links: 
       CboID[0] = 0, HA0 route to QPI1
       CboID[0] = 1, HA0 route to QPI0
       CboID[0] = 0, HA1 route to QPI0
       CboID[0] = 1, HA1 route to QPI1
     */
    UINT32 enablehitme : 1;
    /* enablehitme - Bits[26:26], RW_LB, default = 1'b0 
       0=disable hitme cache;
       1=enable hitme cache.
     */
    UINT32 rsvd_27 : 1;
    UINT32 ptlmiren : 1;
    /* ptlmiren - Bits[28:28], RW_LB, default = 1'b0 
       0=disable partial mirroring mode
       1=enable partial mirroring mode,only the region mapped by TAD Entry0 is mirrored 
       (EX processor only) 
     */
    UINT32 rsvd_29 : 1;
    /* rsvd_29 - Bits[29:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 egcritalwaysfirstdisable : 1;
    /* egcritalwaysfirstdisable - Bits[30:30], RW, default = 1'b0 
       1 = Disable Critical chunk always first
     */
    UINT32 jktpoisonhandling : 1;
    /* jktpoisonhandling - Bits[31:31], RW, default = 1'b0 
       Don't invoke error flow on poison from MC. Only relevant when channel mirroring 
       is enabled. 
       1: Ignore poison from MC.
       0: Read from alternate mirror channel if poison from MC observed (default).
       This bit will be ignored after channel failed over or when not under channel 
       mirroring. 
     */
  } Bits;
  UINT32 Data;
} HASYSDEFEATURE2_HA_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* HAFRCCHFAIL_HA_CFG_REG supported on:                                       */
/*       IVT_EP (0x40170088)                                                  */
/*       IVT_EX (0x40170088)                                                  */
/*       HSX (0x40190088)                                                     */
/*       BDX (0x40190088)                                                     */
/* Register default value:              0x00000000                            */
#define HAFRCCHFAIL_HA_CFG_REG 0x03004088
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * This register is used for Dynamic Memory Migration (EX processor only)
 */
typedef union {
  struct {
    UINT32 frcfailch0 : 1;
    /* frcfailch0 - Bits[0:0], RW_LB, default = 1'b0 
       This bit is used by BIOS SMM to restore mirror channel fail status and force 
       fail mechanism 
       0: no force fail
       1: force fail
     */
    UINT32 frcfailch1 : 1;
    /* frcfailch1 - Bits[1:1], RW_LB, default = 1'b0 
       This bit is used by BIOS SMM to restore mirror channel fail status and force 
       fail mechanism 
       0: no force fail
       1: force fail
     */
    UINT32 frcfailch2 : 1;
    /* frcfailch2 - Bits[2:2], RW_LB, default = 1'b0 
       This bit is used by BIOS SMM to restore mirror channel fail status and force 
       fail mechanism 
       0: no force fail
       1: force fail
     */
    UINT32 frcfailch3 : 1;
    /* frcfailch3 - Bits[3:3], RW_LB, default = 1'b0 
       This bit is used by BIOS SMM to restore mirror channel fail status and force 
       fail mechanism 
       0: no force fail
       1: force fail
     */
    UINT32 rsvd_4 : 4;
    /* rsvd_4 - Bits[7:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_8 : 24;
  } Bits;
  UINT32 Data;
} HAFRCCHFAIL_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* HACHFAILSTS_HA_CFG_REG supported on:                                       */
/*       IVT_EP (0x4017008C)                                                  */
/*       IVT_EX (0x4017008C)                                                  */
/*       HSX (0x4019008C)                                                     */
/*       BDX (0x4019008C)                                                     */
/* Register default value:              0x00000000                            */
#define HACHFAILSTS_HA_CFG_REG 0x0300408C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * This register is used for Dynamic Memory Migration. (EX processor only)
 * This register is valid and should be read only if the MC5_MISC_Shadow.failover 
 * bit is set. 
 */
typedef union {
  struct {
    UINT32 failch0 : 1;
    /* failch0 - Bits[0:0], RO_V, default = 1'b0 
       The indication of the channel 0 fail
       0: no fail
       1: fail
     */
    UINT32 failch1 : 1;
    /* failch1 - Bits[1:1], RO_V, default = 1'b0 
       The indication of the channel 1 fail
       0: no fail
       1: fail
     */
    UINT32 failch2 : 1;
    /* failch2 - Bits[2:2], RO_V, default = 1'b0 
       The indication of the channel 2 fail
       0: no fail
       1: fail
     */
    UINT32 failch3 : 1;
    /* failch3 - Bits[3:3], RO_V, default = 1'b0 
       The indication of the channel 3 fail
       0: no fail
       1: fail
     */
    UINT32 rsvd_4 : 4;
    /* rsvd_4 - Bits[7:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 rsvd_8 : 24;
  } Bits;
  UINT32 Data;
} HACHFAILSTS_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* H2SLUT_HA_CFG_REG supported on:                                            */
/*       IVT_EP (0x40170090)                                                  */
/*       IVT_EX (0x40170090)                                                  */
/*       HSX (0x40190090)                                                     */
/*       BDX (0x40190090)                                                     */
/* Register default value:              0x00000000                            */
#define H2SLUT_HA_CFG_REG 0x03004090


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.0.CFG.xml.
 * Configures the protocol of how the HA communicates with the Intel QPI agents in 
 * the system. 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 16;
    /* rsvd_0 - Bits[15:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 dirnode : 8;
    /* dirnode - Bits[23:16], RWS_L, default = 8'b00000000 
       This is a field that defines the directory node value.
       Each bit represents a corresponding caching agent NodeID's directory ability.
       1 = means directory enable
       0 = means disable (no directory enable node in the system)
       One bit per (caching agent) node configuration for directory mode.
       Note: the active node identification is in the active node mask field of 
       register HA System Defeature Register (HaSysDefeature). 
     */
    UINT32 nodatacmpsplit : 1;
    /* nodatacmpsplit - Bits[24:24], RWS, default = 1'b0 
       This is a field that defines the return data to the requester does not split 
       with its completion on the RspiWB or other read messages which requires to 
       update the directory. 
       0: Allow data split and drive the data early (default)
       1: Do not split data and its completion
       NoDataCmpSplit returns the data with completion together. When it is enabled, it 
       can reduce the interconnect traffic at the cost of latency. 
       It forces the data to wait until all memory operations (memory write on RspWb or 
       directory update) are posted in MC before returning data. 
       For RspWb transaction, it can add a quite bit latency due to waiting.
       If the interconnect is very congested, NoDataCmpSplit can be used as for the 
       tradeoffs between interconnect traffic and transaction latency. 
     */
    UINT32 rsvd_25 : 4;
    /* rsvd_25 - Bits[28:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 socketid : 3;
    /* socketid - Bits[31:29], RWS_L, default = 3'b000 
       This is the identification of local socket ID. It is initialized by BIOS if the 
       socket is not the legacy socket. The legacy socket ID is from socket ID pins. 
       The pcode identifies the legacy socket from the packet socket_pin and set this 
       socket ID from pcode strip at the reset time. 
       Note that this is a 3-bit socket NID [3, 1:0].
       Range of NIDs in this register is [0...7]. Note that because of swizzling of 
       NID[2] with NID[3] before sending Intel QPI packets into the Intel QPI domain 
       and filing the Intel QPI NID[2] bit with either an RTID bit or a home agent 0/1 
       bit in some cases, the NIDs observed in the Intel QPI domain might fall outside 
       the [0...7] range. 
       
       When the HA is configured as a migration slave, the BIOS should program this 
       field to be the same as the Master HA SocketID. This will ensure that the Slave 
       HA and Master HA have the same BT mapping for all transactions. When the 
       migration mode is exited, it should be reprogramed to the original socket NID. 
     */
  } Bits;
  UINT32 Data;
} H2SLUT_HA_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* QPILUT_HA_CFG_REG supported on:                                            */
/*       IVT_EP (0x40170094)                                                  */
/*       IVT_EX (0x40170094)                                                  */
/*       HSX (0x40190094)                                                     */
/*       BDX (0x40190094)                                                     */
/* Register default value:              0x00010000                            */
#define QPILUT_HA_CFG_REG 0x03004094
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * This register configures the routing to the various Intel QPI agents in the 
 * system. 
 */
typedef union {
  struct {
    UINT32 qpiroute : 16;
    /* qpiroute - Bits[15:0], RWS_L, default = 16'b0000000000000000 
       This field defines the Intel QPI route vector. 2 bits per socket/CA identifies 
       which Intel QPI link route to that socket/CA. 00=>QPI0, 01=>QPI1, 1X=>QPI2 
       The local socket/CA is a don't care since there is no Intel QPI route needed.
       Socket/CA range from {7..0}. Note that NID2 is not used in the socket/CA NodeID.
       QpiRoute[1:0] will give the Intel QPI link route to socket/CA 0, QpiRoute[3:2] 
       will give the Intel QPI link route to socket/CA 1, etc. 
       Default: 00h = No Intel QPI route needed (it seems all socket/CAs go through 
       QPI0, actually it has no meaning for default UP configuration) 
       
       Example:
       0000000000000000b = route all socket/CA through QPI0 or do not care (based on 
       system configuration) 
       0000000000000001b = route socket/CA 1-7 through QPI0 or do not care (based on 
       system configuration), route socket 0 through QPI1; 
       0000000000000101b = route socket/CA 0 through QPI1, socket 1 through QPI1, 
       socket/CA 2-7 through QPI0 or do not care (based on the configuration) 
       
       Note:
       1S: This field is not useful
       2S with early snoop (not POR) and QpiSrcRoute: This field is not useful
       2S with early snoop disabled (POR) and QpiSrcRoute disabled: This field is used 
       to route home snoops 
       (if snoop fanout is disabled, and Cbo ID snoop interleave mode is disabled), 
       data, and completions. 
       2S with early snoop disabled (POR) and QpiSrcRoute enabled (POR): This field is 
       used to route home snoops 
       (if snoop fanout is disabled, and Cbo ID snoop interleave mode is disabled).
       2S+ configurations: This field is used to route snoops (if snoop fanout is 
       disabled, 
       and Cbo ID snoop interleave mode is disabled), data, and completions
     */
    UINT32 qpisrcroute : 1;
    /* qpisrcroute - Bits[16:16], RWS_L, default = 1'b1 
       Intel QPI Source Route (QpiSrcRoute):
       0: Route data and completions using the Intel QPI route vector;
       1: Route data and completions to Intel QPI link the request came from (default)
       Note:
       This bit must be cleared in 2S+ configurations.
       Snoops may be routed using the Intel QPI route vector, snoop fanout table or 
       based on Cbo ID depending upon configuration. 
     */
    UINT32 rsvd : 15;
    /* rsvd - Bits[31:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} QPILUT_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* HADIRECT2CORETHRESHOLDS_HA_CFG_REG supported on:                           */
/*       IVT_EP (0x40170098)                                                  */
/*       IVT_EX (0x40170098)                                                  */
/*       HSX (0x40190098)                                                     */
/*       BDX (0x40190098)                                                     */
/* Register default value:              0x00000008                            */
#define HADIRECT2CORETHRESHOLDS_HA_CFG_REG 0x03004098
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * The register is used to select direct2core control algorithm, calculation mode, 
 * and thresholds when the dynamic direct2core feature is enabled. However, there 
 * is a bit (bit 0) that can be used to enable the direct2core with static mode. If 
 * the system software selects this option, the dynamic direct2core thresholds and 
 * control algorithm will not be used at the HA. 
 */
typedef union {
  struct {
    UINT32 dynamicmodedis : 1;
    /* dynamicmodedis - Bits[0:0], RW, default = 1'b0 
       This bit can be used to disable dynamic direct2core mode at HA when the 
       direct2core bit sets on a received BL ring packet. This bit can be functional 
       only if Direct2core feature is enabled.The dynamic mode: HA uses the egress 
       credit threshold and algorithm to dynamically enable and disable the direct2core 
       feature at HA. The static mode is statically enable direct2core feature. 
       0 = Dynamic mode enable (default)
       1 = Static mode enable
     */
    UINT32 summode : 1;
    /* summode - Bits[1:1], RW, default = 1'b0 
       This bit is enable / disable the direct2core sum mode or individual mode. This 
       bit can be functional only if Direct2core feature is enabled.The sum mode is the 
       hardware logic based detection algorithm that looks at the sum of the credits 
       across the two internal home agent schedulers when deciding if HA to enable or 
       disable the direct2core feature. 
       The individual mode is the hardware logic based detection algorithm that looks 
       at the credits of two schedulers individually. The direct2core feature is 
       enabled only the both scheduler credits meet the threshold requirements. 
       0= sum mode is enabled (default)
       1= individual mode is enabled
     */
    UINT32 d2cthresholden : 3;
    /* d2cthresholden - Bits[4:2], RW, default = 3'b010 
       This field defines the egress credit threshold of the direct2core sum mode. When 
       the egress total credits of two schedulers is great than the threshold, the 
       direct2core mode should be dynamically enabled.This filed can be optimized by 
       the BIOS for the performance tuning. 
     */
    UINT32 d2cthresholddis : 3;
    /* d2cthresholddis - Bits[7:5], RW, default = 3'b000 
       This field defines the egress credit threshold of the direct2core sum mode. When 
       the egress total credits of two schedulers is equal or less than the threshold, 
       the direct2core mode should be dynamically disabled.This filed can be optimized 
       by the BIOS for the performance tuning. 
     */
    UINT32 rsvd : 24;
    /* rsvd - Bits[31:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HADIRECT2CORETHRESHOLDS_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* HASYSDEFEATURE3_HA_CFG_REG supported on:                                   */
/*       HSX (0x4019009C)                                                     */
/*       BDX (0x4019009C)                                                     */
/* Register default value:              0x00000000                            */
#define HASYSDEFEATURE3_HA_CFG_REG 0x0300409C
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.0.CFG.xml.
 * COD (Cluster on Die) is only supported on select SKUs of the EP processor.
 */
typedef union {
  struct {
    UINT32 enablecod : 1;
    /* enablecod - Bits[0:0], RW_LB, default = 1'b0 
       When set, COD mode is enabled. (Supported on select EP processors only)
     */
    UINT32 disablehmshrdstate : 1;
    /* disablehmshrdstate - Bits[1:1], RW_LB, default = 1'b0 
       When set, the hitme cache will *not* assume a line with multiple PV bits set is 
       in the shared state, and will issue snoops to those directory agents anyway. 
     */
    UINT32 rsvd : 30;
    /* rsvd - Bits[31:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HASYSDEFEATURE3_HA_CFG_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * COD (Cluster on Die) is only supported on select SKUs of the EP processor.
 */
typedef union {
  struct {
    UINT32 enablecod : 1;
    /* enablecod - Bits[0:0], RW_LB, default = 1'b0 
       When set, COD mode is enabled. (Supported on select EP processors only)
     */
    UINT32 disablehmshrdstate : 1;
    /* disablehmshrdstate - Bits[1:1], RW_LB, default = 1'b0 
       When set, the hitme cache will *not* assume a line with multiple PV bits set is 
       in the shared state, 
       and will issue snoops to those directory agents anyway.
     */
    UINT32 enablee2epar : 1;
    /* enablee2epar - Bits[2:2], RW_LB, default = 1'b0 
       When set, E2E par feature is enabled.
     */
    UINT32 endoublehadbwrbw : 1;
    /* endoublehadbwrbw - Bits[3:3], RW_LB, default = 1'b0 
       When set, hadb wr bw is doubled is enabled.
     */
    UINT32 endoublehadbrdbw : 1;
    /* endoublehadbrdbw - Bits[4:4], RW_LB, default = 1'b0 
       When set, hadb rd bw is doubled is enabled.
     */
    UINT32 enpcommitwr : 1;
    /* enpcommitwr - Bits[5:5], RW_LB, default = 1'b0 
       When set, the pcommit-write flow will be supported
       and will issue snoops to those directory agents anyway.
     */
    UINT32 rsvd : 26;
    /* rsvd - Bits[31:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HASYSDEFEATURE3_HA_CFG_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */





/* BT_CONFIG_HA_CFG_REG supported on:                                         */
/*       IVT_EP (0x401700A4)                                                  */
/*       IVT_EX (0x401700A4)                                                  */
/*       HSX (0x401900A4)                                                     */
/*       BDX (0x401900A4)                                                     */
/* Register default value:              0x00000000                            */
#define BT_CONFIG_HA_CFG_REG 0x030040A4


#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.18.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 btenable : 1;
    /* btenable - Bits[0:0], RW, default = 1'b0 
       Backup Tracker Enable 0: Disable BT, 1: Enable BT 
     */
    UINT32 btmode : 3;
    /* btmode - Bits[3:1], RW, default = 3'b0 
       Backup Tracker Modes: 
       0: Baseline 
       1: BT_Mode1 
       2: BT_Mode2 
       4: BT_Mode3
       Baseline BT_Mode0: This mode allows 64 RTIDs for each CA, supporting upto 8 CAs. 
       This mode does not allow extended RTIDs. 
       BT_Mode1: This mode allows 128 RTIDs for each CA, supporting upto 4 CAs. This 
       mode allows extended RTIDs on all CAs. Suggested mode for upto 4S configurations 
       with BT enabled. 
       BT_Mode2: This mode allows 128 RTIDs to the local CA (allowing xRTID mode), and 
       only 48 RTIDs to the remote CAs. This is the suggested mode for 8S 
       configuration. 
       BT_Mode3: This mode allows 128 RTIDs to the local CA (allowing xRTID mode), and 
       64 RTIDs to the remote CAs. This mode can support upto 7 CAs and is the 
       suggested mode for xNC configurations. 
     */
    UINT32 btreserve_drs : 1;
    /* btreserve_drs - Bits[4:4], RW, default = 1'b0 
       When enabled, reserve a number of HT entries for remote DRS transactions 
       (default is 0xC to keep 3 Intel QPI ports saturated). 
       There is no need to set this bit to obtain the default value of 0xC for the 
       number of reserved HT entries for remote DRS transactions. 
       When a different value is desired, set this bit and program numhtslotsrsvd_drs 
       to the value needed. 
     */
    UINT32 numhtslotsrsvd_drs : 4;
    /* numhtslotsrsvd_drs - Bits[8:5], RW, default = 4'b0 
       0->15 HT slots rsvd for remote DRS, can be divided in any manner amongst 3 Intel 
       QPI agents. Use value of 0 for mirror-slave or migration-slave. 
       Btreserve_drs should be set to 1 in order for the value programmed in this field 
       to take effect. 
     */
    UINT32 btreserve_ackcnflt : 1;
    /* btreserve_ackcnflt - Bits[9:9], RW, default = 1'b0 
       When enabled, reserve a number of HT entries for Ackcnflt/NonSnpRd(default is 
       0x1). 
       There is no need to set this bit to obtain the default value of 0x1 for the 
       number of reserved HT entries for Ackcnflt / NonSnpRd transactions. 
       When a different value is desired, set this bit and program 
       NumHTslotsRsvd_AckCnflt to the value needed. 
     */
    UINT32 numhtslotsrsvd_ackcnflt : 2;
    /* numhtslotsrsvd_ackcnflt - Bits[11:10], RW, default = 2'b0 
       0->3 HT slots rsvd for AckCnflt/NonSnpRd. Use value of 0 for mirror-slave or 
       migration-slave, which is the default internally reserved number of slots coming 
       out of reset, hence should not require programming for migration-slave. 
       Btreserve_ackcnflt should be set to 1 in order for the value programmed in this 
       field to take effect. 
     */
    UINT32 btreserve_localreq : 1;
    /* btreserve_localreq - Bits[12:12], RW, default = 1'b0 
       When enabled, reserve a number of HT entries for local requests (default is 
       0x1F) 
     */
    UINT32 numhtslotsrsvd_localreq : 6;
    /* numhtslotsrsvd_localreq - Bits[18:13], RW, default = 5'b0 
       0->35 HT slots rsvd for local requests. Use value of 0 for mirror-slave
     */
    UINT32 rsvd_19 : 1;
    UINT32 enablenosplit : 1;
    /* enablenosplit - Bits[20:20], RW, default = 1'b0 
       When set will wait for both critical and non-critical data chunks in the BL 
       egress before sending to ring. Will need to be enabled in any configuration in 
       which BT is enabled.   
     */
    UINT32 disablecmpfwdcode : 1;
    /* disablecmpfwdcode - Bits[21:21], RW, default = 1'b0 
       When set will disable CmpFwdCode generation during conflict flows and instead a 
       Cmp_FwdInvItoE will be generated. This is a debug defeature.   
     */
    UINT32 disablecmpfwdinvown : 1;
    /* disablecmpfwdinvown - Bits[22:22], RW, default = 1'b0 
       When set will disable Cmp_FwdInvOwn generation during conflict flows and instead 
       a Cmp_FwdInvItoE will be generated. This is a debug defeature. 
               
     */
    UINT32 rsvd_23 : 2;
    UINT32 defeaturegp : 1;
    /* defeaturegp - Bits[25:25], RW, default = 1'b0 
       Defeature GP to 2 entry/bank 
     */
    UINT32 rsvd_26 : 6;
  } Bits;
  UINT32 Data;
} BT_CONFIG_HA_CFG_HSX_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* (HSX_HOST) || defined(BDX_HOST) */




/* OSB_CONFIG_HA_CFG_REG supported on:                                        */
/*       IVT_EP (0x401700A8)                                                  */
/*       IVT_EX (0x401700A8)                                                  */
/*       HSX (0x401900A8)                                                     */
/*       BDX (0x401900A8)                                                     */
/* Register default value on IVT_EP:    0x00903405                            */
/* Register default value on IVT_EX:    0x00903405                            */
/* Register default value on HSX:       0x00803004                            */
/* Register default value on BDX:       0x00803004                            */
#define OSB_CONFIG_HA_CFG_REG 0x030040A8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Configures the opportunistic snoop broadcast feature.
 */
typedef union {
  struct {
    UINT32 osbloi2e : 1;
    /* osbloi2e - Bits[0:0], RW, default = 1'b0 
       Enable OSB for Local InvI2E
     */
    UINT32 osbloi2efrc : 1;
    /* osbloi2efrc - Bits[1:1], RW, default = 1'b0 
       Force OSB for these transactions all the time. So no longer opportunistic.
     */
    UINT32 loi2eadvnath : 4;
    /* loi2eadvnath - Bits[5:2], RW, default = 4'b0001 
       Local InvI2E AD VNA Credit threshold. OSB is cancelled for local InvItoE if AD 
       VNA credits available is below thredhold for atleast 1 Intel QPI port. 
       Maximum value of threshold depends on the maximum number of AD VNA credits 
       available per Intel QPI port. 
     */
    UINT32 loi2eadegrth : 4;
    /* loi2eadegrth - Bits[9:6], RW, default = 4'b0000 
       Local InvI2E AD Egress threshold. OSB is cancelled for local InvItoE if AD 
       egress credits available is below thredhold. 
       Maximum value of threshold depends on the maximum number of AD egress credits 
       available. 
     */
    UINT32 osblord : 1;
    /* osblord - Bits[10:10], RW, default = 1'b0 
       Enable OSB for Local Rd
       Important HSX Note:
       Always keep this bit at the default value. Do not modify this bit since this 
       functionality is no longer supported. 
     */
    UINT32 osblordfrc : 1;
    /* osblordfrc - Bits[11:11], RW, default = 1'b0 
       Force OSB for these transactions all the time. So no longer opportunistic.
       Important HSX Note:
       Always keep this bit at the default value. Do not modify this bit since this 
       functionality is no longer supported. 
     */
    UINT32 lrdadvnath : 4;
    /* lrdadvnath - Bits[15:12], RW, default = 4'b0011 
       Local Rd AD VNA Credit threshold. OSB is cancelled for local read if AD VNA 
       credits available is below thredhold for atleast 1 Intel QPI port. 
       Maximum value of threshold depends on the maximum number of AD VNA credits 
       available per Intel QPI port. 
       Important HSX Note:
       Always keep these bits at the default value. Do not modify these bits since this 
       functionality is no longer supported. 
     */
    UINT32 lrdadegrth : 4;
    /* lrdadegrth - Bits[19:16], RW, default = 4'b0000 
       Local Rd AD Egress threshold. OSB is cancelled for local read if AD egress 
       credits available is below thredhold. 
       Maximum value of threshold depends on the maximum number of AD egress credits 
       available. 
       Important HSX Note:
       Always keep these bits at the default value. Do not modify these bits since this 
       functionality is no longer supported. 
     */
    UINT32 osbrmt : 1;
    /* osbrmt - Bits[20:20], RW, default = 1'b0 
       Enable OSB for Remote Rd
       Important HSX Note:
       Always keep this bit at the default value. Do not modify this bit since this 
       functionality is no longer supported. 
     */
    UINT32 osbrmtfrc : 1;
    /* osbrmtfrc - Bits[21:21], RW, default = 1'b0 
       Force OSB for these transactions all the time. So no longer opportunistic.
       Important HSX Note:
       Always keep this bit at the default value. Do not modify this bit since this 
       functionality is no longer supported. 
     */
    UINT32 rmtadvnath : 4;
    /* rmtadvnath - Bits[25:22], RW, default = 4'b0010 
       Remote Rd AD VNA Credit threshold. OSB is cancelled for remote read/InvItoE if 
       AD VNA credits available is below thredhold for at least 1 Intel QPI port. 
       Maximum value of threshold depends on the maximum number of AD VNA credits 
       available per Intel QPI port. 
       Important HSX Note:
       Always keep these bits at the default value. Do not modify these bits since this 
       functionality is no longer supported. 
     */
    UINT32 rmtadegrth : 4;
    /* rmtadegrth - Bits[29:26], RW, default = 4'b0000 
       Remote Rd AD Egress threshold. OSB is cancelled for remote read/InvItoE if AD 
       egress credits available is below thredhold. 
       Maximum value of threshold depends on the maximum number of AD egress credits 
       available. 
       Important HSX Note:
       Always keep these bits at the default value. Do not modify these bits since this 
       functionality is no longer supported. 
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[31:30], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} OSB_CONFIG_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* EXRAS_CONFIG_HA_CFG_REG supported on:                                      */
/*       IVT_EP (0x401700AC)                                                  */
/*       IVT_EX (0x401700AC)                                                  */
/*       HSX (0x401900AC)                                                     */
/*       BDX (0x401900AC)                                                     */
/* Register default value:              0x00003000                            */
#define EXRAS_CONFIG_HA_CFG_REG 0x030040AC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Configures RAS mirroring and migration modes for EX processors.
 */
typedef union {
  struct {
    UINT32 isinterhamirrormaster : 1;
    /* isinterhamirrormaster - Bits[0:0], RW_LB, default = 1'b0 
       For migration: sets this home agent up as master.
       The only legal configs for bits [2:0] are:
       {IsInterHaMigrationUnnnH, IsInterHaMirrorSlave, IsInterHaMirrorMaster} =
       0,0,0: No migration.
       1,0,1: Migration master.
       1,1,0: Migration slave.
     */
    UINT32 isinterhamirrorslave : 1;
    /* isinterhamirrorslave - Bits[1:1], RW_LB, default = 1'b0 
       For migration: sets this home agent up as slave.
     */
    UINT32 isinterhamigration : 1;
    /* isinterhamigration - Bits[2:2], RW_LB, default = 1'b0 
       For migration: when set, indicates that this master/slave is in migration mode.
     */
    UINT32 mirtrgtnid : 4;
    /* mirtrgtnid - Bits[6:3], RW_LB, default = 4'b0000 
       For migration. This is the nodeID of the target,
       i.e. program with slaveNID at the master, and masterNID at the slave.
       Note that the Target NID should be programmed as follows:
       msb of Target NID should be HA identifier (HA0 vs HA1) of the targetHA,
       and the remaining 3 bits should be the socket NID[3, 1:0] of the targetHA.
     */
    UINT32 rsvd_7 : 1;
    UINT32 mirtrgtoncsi0 : 1;
    /* mirtrgtoncsi0 - Bits[8:8], RW_LB, default = 1'b0 
       Route through QPI0 to get to migration target.
     */
    UINT32 mirtrgtoncsi1 : 1;
    /* mirtrgtoncsi1 - Bits[9:9], RW_LB, default = 1'b0 
       Route through QPI1 to get to migration target.
     */
    UINT32 mirtrgtoncsi2 : 1;
    /* mirtrgtoncsi2 - Bits[10:10], RW_LB, default = 1'b0 
       Route through QPI2 to get to migration target.
     */
    UINT32 cfgmcacmcionmirrorcorr : 1;
    /* cfgmcacmcionmirrorcorr - Bits[11:11], RW_LB, default = 1'b0 
       Control error that causes an interrupt of CMCI
       immediately
       0: do not interrupt (default)
       1: interrupt
     */
    UINT32 cfgmcacmcionfailover : 1;
    /* cfgmcacmcionfailover - Bits[12:12], RW_LB, default = 1'b1 
       Control error that causes an interrupt of CMCI
       immediately
       0: do not interrupt
       1: interrupt (default)
       
       Note: BIOS will not use the upcode trap mechanism to initialize this register. 
       It does not need to be MSR register. It is HA responsibility to initialize this 
       register. Ucode and TAP can modify the initial value as needed. 
     */
    UINT32 cfgmcacmcioncorrcountthr : 1;
    /* cfgmcacmcioncorrcountthr - Bits[13:13], RW_LB, default = 1'b1 
       Control CMCI signaling when correctable error count reaches threshold.
       0: no CMCI
       1: allow CMCI (default)
     */
    UINT32 cfgmcasmioncorr : 1;
    /* cfgmcasmioncorr - Bits[14:14], RW_LB, default = 1'b0 
       Control SMI signaling for correctable error.
       0: no SMI (default).
       1: allow SMI.
       Max one bit should be set in {CfgMcaSmiOnCorr, CfgMcaCmciOnCorr, 
       CfgMcaMcerrOnCorr}. 
     */
    UINT32 cfgmcasmionmirrorcorr : 1;
    /* cfgmcasmionmirrorcorr - Bits[15:15], RW_LB, default = 1'b0 
       Control SMI signaling for mirror channel correctable error.
       0: no SMI (default).
       1: allow SMI.
       Max one bit should be set in {CfgMcaSmiOnMirrorCorr, CfgMcaCmciOnMirrorCorr, 
       CfgMcaMcerrOnMirrorCorr}. 
     */
    UINT32 cfgmcasmionfailover : 1;
    /* cfgmcasmionfailover - Bits[16:16], RW_LB, default = 1'b0 
       Control SMI signaling for mirror channel failover (one channel is bad).
       0: no SMI (default).
       1: allow SMI.
       Max one bit should be set in {CfgMcaSmiOnFailover, CfgMcaCmciOnFailover, 
       CfgMcaMcerrOnFailover}. 
     */
    UINT32 rsvd : 15;
    /* rsvd - Bits[31:17], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} EXRAS_CONFIG_HA_CFG_STRUCT;
#endif /* ASM_INC */




/* PCOMMITADDRHICH3_HA_CFG_REG supported on:                                  */
/*       BDX (0x401900B4)                                                     */
/* Register default value:              0x00000000                            */
#define PCOMMITADDRHICH3_HA_CFG_REG 0x030040B4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 hiphyaddr : 14;
    /* hiphyaddr - Bits[13:0], RW, default = 14'b00000000000000 
       This contains 14 bits of physical address [45:32] of a cache line, corresponding 
       address match is an indication of pcommit wb   
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} PCOMMITADDRHICH3_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* PCOMMITADDRCH3_HA_CFG_REG supported on:                                    */
/*       BDX (0x401900B8)                                                     */
/* Register default value:              0x00000000                            */
#define PCOMMITADDRCH3_HA_CFG_REG 0x030040B8

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rsvd : 6;
    /* rsvd - Bits[5:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 lophyaddr : 26;
    /* lophyaddr - Bits[31:6], RW, default = 26'b00000000000000000000000000 
       This contains 26 bits of physical address [31:6] of a cache line, corresponding 
       address match is an indication of pcommit wb   
     */
  } Bits;
  UINT32 Data;
} PCOMMITADDRCH3_HA_CFG_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* SNPFANOUT_CONFIG_HA_CFG_REG supported on:                                  */
/*       IVT_EP (0x401700BC)                                                  */
/*       IVT_EX (0x401700BC)                                                  */
/*       HSX (0x401900BC)                                                     */
/*       BDX (0x401900BC)                                                     */
/* Register default value on IVT_EP:    0x0000001C                            */
/* Register default value on IVT_EX:    0x0000001C                            */
/* Register default value on HSX:       0x00000004                            */
/* Register default value on BDX:       0x00000004                            */
#define SNPFANOUT_CONFIG_HA_CFG_REG 0x030040BC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Snoop Fanout register.  This mode reduces the number of snoops that the HA 
 * sends, i.e. snoops are fanned out to the different nodes by the Intel QPI agents 
 * rather than having HA send 1 directed snoop per node.  This register defines 
 * which Intel QPI links the HA needs to send a fanout snoop to. 
 */
typedef union {
  struct {
    UINT32 enablefanout : 1;
    /* enablefanout - Bits[0:0], RW, default = 1'b0 
       Enable snoop fanout. Under this mode, HA will send only 1 snoop per Intel QPI 
       link for a given NID, as programmed by the Send2QPIPorts fields. 
     */
    UINT32 fanout2srcdisable : 1;
    /* fanout2srcdisable - Bits[1:1], RW, default = 1'b0 
       Disable sending a fanout snoop to the source link for a remote request. If this 
       is set, a remote request will never send a snoop to the same link that it 
       arrived. For local requests, this does not apply as local request will never 
       send a snoop to itself. It is the responsibility of the Intel QPI agent when 
       fanning out to make sure that 
       a snoop does not go to the node with which a request originated, thus this field 
       is not functionally required. It can be used to reduce the # of snoops in a 
       fully connected topology. 
     */
    UINT32 activeqpiports : 3;
    /* activeqpiports - Bits[4:2], RW, default = 3'b001 
       Indicates which of the 3 Intel QPI links are active. Under fanout mode, HA will 
       send only 1 snoop per Intel QPI link for a given NID, as programmed by the 
       Send2QPIPorts fields. A snoop will only go out 
       to the link programmed by Send2QPIPorts if the link is active.
       All 3 are active by default.
     */
    UINT32 send2qpiports_n0 : 3;
    /* send2qpiports_n0 - Bits[7:5], RW, default = 3'b000 
       Which links to send snoops to for a request from Node0. These fields are indexed 
       directly using the RHNID of the request. 
     */
    UINT32 send2qpiports_n1 : 3;
    /* send2qpiports_n1 - Bits[10:8], RW, default = 3'b000 
       Which links to send snoops to for a request from Node1. These fields are indexed 
       directly using the RHNID of the request. 
     */
    UINT32 send2qpiports_n2 : 3;
    /* send2qpiports_n2 - Bits[13:11], RW, default = 3'b000 
       Which links to send snoops to for a request from Node2. These fields are indexed 
       directly using the RHNID of the request. 
     */
    UINT32 send2qpiports_n3 : 3;
    /* send2qpiports_n3 - Bits[16:14], RW, default = 3'b000 
       Which links to send snoops to for a request from Node3. These fields are indexed 
       directly using the RHNID of the request. 
     */
    UINT32 send2qpiports_n4 : 3;
    /* send2qpiports_n4 - Bits[19:17], RW, default = 3'b000 
       Which links to send snoops to for a request from Node4. These fields are indexed 
       directly using the RHNID of the request. 
     */
    UINT32 send2qpiports_n5 : 3;
    /* send2qpiports_n5 - Bits[22:20], RW, default = 3'b000 
       Which links to send snoops to for a request from Node5. These fields are indexed 
       directly using the RHNID of the request. 
     */
    UINT32 send2qpiports_n6 : 3;
    /* send2qpiports_n6 - Bits[25:23], RW, default = 3'b000 
       Which links to send snoops to for a request from Node6. These fields are indexed 
       directly using the RHNID of the request. 
     */
    UINT32 send2qpiports_n7 : 3;
    /* send2qpiports_n7 - Bits[28:26], RW, default = 3'b000 
       Which links to send snoops to for a request from Node7. These fields are indexed 
       directly using the RHNID of the request. 
     */
    UINT32 rsvd : 3;
    /* rsvd - Bits[31:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} SNPFANOUT_CONFIG_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* HTBASELOW_HA_CFG_REG supported on:                                         */
/*       IVT_EP (0x401700C4)                                                  */
/*       IVT_EX (0x401700C4)                                                  */
/*       HSX (0x401900C4)                                                     */
/*       BDX (0x401900C4)                                                     */
/* Register default value:              0x00000000                            */
#define HTBASELOW_HA_CFG_REG 0x030040C4
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Each home agent has 7 bits mapping to the assigned HT entry starting address. 
 * The upper 4-bits are in the HtBase register and the lower 3-bits are in the 
 * HtBaseLow register. Previous processors only had the HtBase register since each 
 * home agent is assigned RTIDs that are multiples of 8. This restriction has been 
 * relaxed to allow arbitrary RTID allocation and hence the need to add HtBaseLow 
 * to carry the lower 3-bits of the starting HT entry. The RTID of an incoming 
 * transaction from a specific socket is added to the 7-bit HT entry starting 
 * address for that socket to identify the HTID for the incoming transaction. i.e. 
 * the HTID of a request is simply the concatenation {Nid*HtBase[3:0], 
 * Nid*HtBaseLow[2:0]} + requestRTID. Note that the HTSwizzle bit in HaSysDefeatuer 
 * could modify the HTID to distribute them evenly between the two HA schedulers. 
 * This register is used only when the BT is disabled. It is ignored when the BT is 
 * enabled. Note that NID in this register is the socket NID. When extended RTID 
 * mode is enabled, either a local socket or remote socket in a 2S configuration is 
 * assigned 2 NIDs. This is used in ISOC configurations. 
 * 
 * Note that the HtBase is programmed in increasing order from low NID to high NID. 
 * i.e. NID0 is generally programmed to 0. Even though we have HTBase fields up to 
 * Nid7, the only ones that would ever be need to be used are Nid0, Nid1, Nid4, and 
 * Nid5.  This is because only 1S and 2S (non-XNC) configs would ever have BT 
 * disabled, and the assumption is that there would be no randomization of the 
 * socketIDs in 1S/2S (i.e. we would not use local=6 and remote=7 in 2S config, 
 * instead we always use socketIDs 0 and 1).  Nid4 and Nid5 would be used for 
 * legacy extended RTIDs, i.e. Nid4 == extended RTIDs for Nid0, and Nid5==extended 
 * RTIDs for Nid1. 
 * 
 * For purposes of detecting out of range RTIDs and being able to fire MCA when 
 * this happens, the HTBases must be programmed in ascending order such that Nid0 
 * <= Nid1, Nid1 <= Nid4 OR Nid4=0, and Nid4 <= Nid5 OR Nid5=0.  The number of 
 * entries each Nid is given is determined by the HTBase difference between the 
 * following HTBase of the following Nid. For Nid4/5, having a programming of 0 for 
 * the HTBases means no extended RTID for node0/1.  Alternatively, 
 * HTBaseNid4==HTBaseNid5 has 0 difference, so that also means no extended RTID for 
 * node0.  Similarily, if HTBaseNid5==HTBaseNid6, that would mean no extended RTID 
 * for node1. 
 */
typedef union {
  struct {
    UINT32 nid0htbaselow : 3;
    /* nid0htbaselow - Bits[2:0], RW_LB, default = 3'b000 
       NID0 HT BaseLow (Nid0HtBaseLow): This field defines the HTID mapping.
       Lower 3b of the HT Base for NID0. The upper 4b are stored in HTBASE
     */
    UINT32 nid1htbaselow : 3;
    /* nid1htbaselow - Bits[5:3], RW_LB, default = 3'b000 
       NID1 HT BaseLow (Nid1HtBaseLow): This field defines the HTID mapping.
       Lower 3b of the HT Base for NID1. The upper 4b are stored in HTBASE
     */
    UINT32 nid2htbaselow : 3;
    /* nid2htbaselow - Bits[8:6], RW_LB, default = 3'b000 
       NID2 HT BaseLow (Nid2HtBaseLow): This field defines the HTID mapping.
       Lower 3b of the HT Base for NID2. The upper 4b are stored in HTBASE
     */
    UINT32 nid3htbaselow : 3;
    /* nid3htbaselow - Bits[11:9], RW_LB, default = 3'b000 
       NID3 HT BaseLow (Nid3HtBaseLow): This field defines the HTID mapping.
       Lower 3b of the HT Base for NID3. The upper 4b are stored in HTBASE
     */
    UINT32 nid4htbaselow : 3;
    /* nid4htbaselow - Bits[14:12], RW_LB, default = 3'b000 
       NID4 HT BaseLow (Nid4HtBaseLow): This field defines the HTID mapping.
       Lower 3b of the HT Base for NID4. The upper 4b are stored in HTBASE
     */
    UINT32 nid5htbaselow : 3;
    /* nid5htbaselow - Bits[17:15], RW_LB, default = 3'b000 
       NID5 HT BaseLow (Nid5HtBaseLow): This field defines the HTID mapping.
       Lower 3b of the HT Base for NID5. The upper 4b are stored in HTBASE
     */
    UINT32 nid6htbaselow : 3;
    /* nid6htbaselow - Bits[20:18], RW_LB, default = 3'b000 
       NID6 HT BaseLow (Nid6HtBaseLow): This field defines the HTID mapping.
       Lower 3b of the HT Base for NID6. The upper 4b are stored in HTBASE
     */
    UINT32 nid7htbaselow : 3;
    /* nid7htbaselow - Bits[23:21], RW_LB, default = 3'b000 
       NID7 HT BaseLow (Nid7HtBaseLow): This field defines the HTID mapping.
       Lower 3b of the HT Base for NID7. The upper 4b are stored in HTBASE
     */
    UINT32 rsvd : 8;
    /* rsvd - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HTBASELOW_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* HAOSBPOWERHEUR_HA_CFG_REG supported on:                                    */
/*       IVT_EP (0x401700C8)                                                  */
/*       IVT_EX (0x401700C8)                                                  */
/*       HSX (0x401900C8)                                                     */
/*       BDX (0x401900C8)                                                     */
/* Register default value:              0x00002D2D                            */
#define HAOSBPOWERHEUR_HA_CFG_REG 0x030040C8
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * Configures opportunistic snoop broadcast power heuristics.
 */
typedef union {
  struct {
    UINT32 osbpoweren : 1;
    /* osbpoweren - Bits[0:0], RW, default = 1'b1 
       1 means the OSB Power heuristics is enabled
       0 means the OSB Power heuristics is disabled (default)
     */
    UINT32 osbtimermax : 5;
    /* osbtimermax - Bits[5:1], RW, default = 5'b10110 
       time window used to analyze application behavior (2^OSBTimerMax uncore cycles)
     */
    UINT32 osbtimerfraction : 5;
    /* osbtimerfraction - Bits[10:6], RW, default = 5'b10100 
       specified delay in time window before analyzing application behavior 
       (2^OSBTimerFraction uncore cycles) 
     */
    UINT32 osbprogshift : 5;
    /* osbprogshift - Bits[15:11], RW, default = 5'b00101 
       used to specify threshold of ratio of local read hitting non-I Directory
       The total local read count is shifted by OSBProgShit and compared to
       the count of local read hitting non-I directory.
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HAOSBPOWERHEUR_HA_CFG_STRUCT;
#endif /* ASM_INC */


/* HATIMEOUT_HA_CFG_REG supported on:                                         */
/*       IVT_EP (0x401700CC)                                                  */
/*       IVT_EX (0x401700CC)                                                  */
/*       HSX (0x401900CC)                                                     */
/*       BDX (0x401900CC)                                                     */
/* Register default value:              0x0000FFF8                            */
#define HATIMEOUT_HA_CFG_REG 0x030040CC
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.18.0.CFG.xml.
 * HA controls for initializing timeouts in the Backup Tracker.  The timeout 
 * thresholds are set here by BIOS, and if a timeout is detected they will be 
 * reported to MCBanks to the OS. 
 * Speculative timeouts are also initialized here, however, they are not reported 
 * to MCbanks as they should only be used in a debug situation.  Speculative 
 * timeouts are reported here. 
 */
typedef union {
  struct {
    UINT32 btspectoerror : 1;
    /* btspectoerror - Bits[0:0], RW1C, default = 1'b0 
       1= BT speculative timeout has occured
       0= No BT speculative timeout
     */
    UINT32 btspectodeten : 1;
    /* btspectodeten - Bits[1:1], RW, default = 1'b0 
       Enable BT Speculative TO detection. When enabled, this can be used to predict 
       whether a transaction will timeout and disable debug tracing to prevent 
       overwriting of 
       the useful trace information for the transaction that may or may not have 
       actually timed out. 
       0 = disable BT TO; 1 = enable BT TO
     */
    UINT32 bttimeoutlock : 1;
    /* bttimeoutlock - Bits[2:2], RW_LB, default = 1'b0 
       BIOS lock for the Time base, Time Mult, and SpecTOCount. 1 = BT Timeout values 
       locked; 
     */
    UINT32 bttotimebase : 2;
    /* bttotimebase - Bits[4:3], RW_L, default = 2'b11 
       Backup Tracker Timeout Base Unit:
       00 - Time base is 2**5=32 uncore cycles ~ 1088 uncore cycles. A tracker that 
       exceeds 32 cycles will timeout, but may not detect it until 1088 cycles later. 
       (max programmable time-out of 17K uncore cycles). 
       01 - Time base is 2**12=4K uncore cycles ~ 9K uncore cycles. A tracker that 
       exceeds 4K cycles will timeout, but may not detect it until 9K cycles later. 
       (max programmable time-out of 2M uncore cycles). 
       10 - Time base is 2**19=524K uncore cycles ~ 1.048M uncore cycles. A tracker 
       that exceeds 524K cycles will timeout, but may not detect it until 1.048M cycles 
       later. (max programmable time-out of 256M uncore cycles). 
       11 - Time base is 2**26=67M uncore cycles ~ 134M uncore cycles. A tracker that 
       exceeds 64M cycles will timeout, but may not detect it until 134M cycles later. 
       (max programmable time-out of 32G uncore cycles) 
     */
    UINT32 bttotimemult : 8;
    /* bttotimemult - Bits[12:5], RW_L, default = 8'b11111111 
       Backup Tracker Timeout Multiplier
     */
    UINT32 btspectocount : 3;
    /* btspectocount - Bits[15:13], RW_L, default = 3'b111 
       Backup Tracker Speculative Timeout Count
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} HATIMEOUT_HA_CFG_STRUCT;
#endif /* ASM_INC */




/* CLS_HA_CFG_REG supported on:                                               */
/*       IVT_EP (0x1017000C)                                                  */
/*       IVT_EX (0x1017000C)                                                  */
/* Register default value:              0x00                                  */
#define CLS_HA_CFG_REG 0x0300100C



/* MLT_HA_CFG_REG supported on:                                               */
/*       IVT_EP (0x1017000D)                                                  */
/*       IVT_EX (0x1017000D)                                                  */
/* Register default value:              0x00                                  */
#define MLT_HA_CFG_REG 0x0300100D



/* CAPPOINT_HA_CFG_REG supported on:                                          */
/*       IVT_EP (0x10170034)                                                  */
/*       IVT_EX (0x10170034)                                                  */
/* Register default value:              0x00                                  */
#define CAPPOINT_HA_CFG_REG 0x03001034



/* INTRLINE_HA_CFG_REG supported on:                                          */
/*       IVT_EP (0x1017003C)                                                  */
/*       IVT_EX (0x1017003C)                                                  */
/* Register default value:              0x00                                  */
#define INTRLINE_HA_CFG_REG 0x0300103C



/* INTRPIN_HA_CFG_REG supported on:                                           */
/*       IVT_EP (0x1017003D)                                                  */
/*       IVT_EX (0x1017003D)                                                  */
/* Register default value:              0x00                                  */
#define INTRPIN_HA_CFG_REG 0x0300103D



/* TAD0_HA_CFG_REG supported on:                                              */
/*       IVT_EP (0x40170040)                                                  */
/*       IVT_EX (0x40170040)                                                  */
/* Register default value:              0x00000000                            */
#define TAD0_HA_CFG_REG 0x03004040



/* TAD1_HA_CFG_REG supported on:                                              */
/*       IVT_EP (0x40170044)                                                  */
/*       IVT_EX (0x40170044)                                                  */
/* Register default value:              0x00000000                            */
#define TAD1_HA_CFG_REG 0x03004044



/* TAD2_HA_CFG_REG supported on:                                              */
/*       IVT_EP (0x40170048)                                                  */
/*       IVT_EX (0x40170048)                                                  */
/* Register default value:              0x00000000                            */
#define TAD2_HA_CFG_REG 0x03004048



/* TAD3_HA_CFG_REG supported on:                                              */
/*       IVT_EP (0x4017004C)                                                  */
/*       IVT_EX (0x4017004C)                                                  */
/* Register default value:              0x00000000                            */
#define TAD3_HA_CFG_REG 0x0300404C



/* TAD4_HA_CFG_REG supported on:                                              */
/*       IVT_EP (0x40170050)                                                  */
/*       IVT_EX (0x40170050)                                                  */
/* Register default value:              0x00000000                            */
#define TAD4_HA_CFG_REG 0x03004050



/* TAD5_HA_CFG_REG supported on:                                              */
/*       IVT_EP (0x40170054)                                                  */
/*       IVT_EX (0x40170054)                                                  */
/* Register default value:              0x00000000                            */
#define TAD5_HA_CFG_REG 0x03004054



/* TAD6_HA_CFG_REG supported on:                                              */
/*       IVT_EP (0x40170058)                                                  */
/*       IVT_EX (0x40170058)                                                  */
/* Register default value:              0x00000000                            */
#define TAD6_HA_CFG_REG 0x03004058



/* TAD7_HA_CFG_REG supported on:                                              */
/*       IVT_EP (0x4017005C)                                                  */
/*       IVT_EX (0x4017005C)                                                  */
/* Register default value:              0x00000000                            */
#define TAD7_HA_CFG_REG 0x0300405C



/* TAD8_HA_CFG_REG supported on:                                              */
/*       IVT_EP (0x40170060)                                                  */
/*       IVT_EX (0x40170060)                                                  */
/* Register default value:              0x00000000                            */
#define TAD8_HA_CFG_REG 0x03004060



/* TAD9_HA_CFG_REG supported on:                                              */
/*       IVT_EP (0x40170064)                                                  */
/*       IVT_EX (0x40170064)                                                  */
/* Register default value:              0x00000000                            */
#define TAD9_HA_CFG_REG 0x03004064



/* TAD10_HA_CFG_REG supported on:                                             */
/*       IVT_EP (0x40170068)                                                  */
/*       IVT_EX (0x40170068)                                                  */
/* Register default value:              0x00000000                            */
#define TAD10_HA_CFG_REG 0x03004068



/* TAD11_HA_CFG_REG supported on:                                             */
/*       IVT_EP (0x4017006C)                                                  */
/*       IVT_EX (0x4017006C)                                                  */
/* Register default value:              0x00000000                            */
#define TAD11_HA_CFG_REG 0x0300406C





#endif /* HA_CFG_h */
