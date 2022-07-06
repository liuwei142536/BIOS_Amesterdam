/* Date Stamp: 8/23/2014 */

#ifndef GbE_VF0_h
#define GbE_VF0_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For BDX_HOST:                                                              */
/* GbE_VF0_BDX_DEV 16                                                         */
/* GbE_VF0_BDX_FUN 0                                                          */

/* VENDOR_DEVICE_ID_GbE_VF0_REG supported on:                                 */
/*       BDX (0x40380000)                                                     */
/* Register default value:              0xFFFFFFFF                            */
#define VENDOR_DEVICE_ID_GbE_VF0_REG 0x1A004000

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 vendor_id : 16;
    /* vendor_id - Bits[15:0], ROV, default = 16'hFFFF 
       desc is missing
     */
    UINT32 device_id : 16;
    /* device_id - Bits[31:16], ROV, default = 16'hFFFF 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} VENDOR_DEVICE_ID_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CMD_STAT_REG_GbE_VF0_REG supported on:                                     */
/*       BDX (0x40380004)                                                     */
/* Register default value:              0x00100000                            */
#define CMD_STAT_REG_GbE_VF0_REG 0x1A004004

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 command_io_access_enable : 1;
    /* command_io_access_enable - Bits[0:0], RO, default = 1'h0 
       I/O Access Enable
     */
    UINT32 command_mem_access_enable : 1;
    /* command_mem_access_enable - Bits[1:1], RO, default = 1'h0 
       Memory Access Enable
     */
    UINT32 command_bus_access_enable : 1;
    /* command_bus_access_enable - Bits[2:2], RWS, default = 1'h0 
       Bus Master Enable (BME): 
       			memory or I/O requests. Note that as MSI/MSI-X interrupt messages are in-band
          memory writes, disabling the bus master enable bit disables MSI/MSI-X 
       interrupt 
       			messages as well.
       			Requests other than memory or I/O requests are not controlled by this bit.
          Note: The state of active transactions is not specified when this bit is 
       disabled after 
          being enabled. The device can choose how it behaves when this condition 
       occurs. 
          Software cannot count on the device retaining state and resuming without loss 
       of data 
       			when the bit is re-enabled.
          Transactions for a VF that has its Bus Master Enable set must not be blocked 
       by 
       			transactions for VFs that have their Bus Master Enable cleared.
       			
     */
    UINT32 command_cycle_mon : 1;
    /* command_cycle_mon - Bits[3:3], RO, default = 1'h0 
       Special Cycle Monitoring. Hardwired to 0.
     */
    UINT32 command_mwi_enable : 1;
    /* command_mwi_enable - Bits[4:4], RO, default = 1'h0 
       MWI Enable. Hardwired to 0.
     */
    UINT32 command_palette_snoop_enable : 1;
    /* command_palette_snoop_enable - Bits[5:5], RO, default = 1'h0 
       Palette Snoop Enable. Hardwired to 0
     */
    UINT32 command_parity_error_response : 1;
    /* command_parity_error_response - Bits[6:6], RO, default = 1'h0 
       Parity Error Response
     */
    UINT32 command_wait_cycle_enable : 1;
    /* command_wait_cycle_enable - Bits[7:7], RO, default = 1'h0 
       Wait Cycle Enable. Hardwired to 0
     */
    UINT32 command_serr_enable : 1;
    /* command_serr_enable - Bits[8:8], RO, default = 1'h0 
       SERR# Enable
     */
    UINT32 command_fast_b2b_enable : 1;
    /* command_fast_b2b_enable - Bits[9:9], RO, default = 1'h0 
       Fast Back-to-Back Enable. Hardwired to 0.
     */
    UINT32 command_interrupt_disable : 1;
    /* command_interrupt_disable - Bits[10:10], RO, default = 1'h0 
       Interrupt Disable: Controls the ability of a device to generate a legacy 
       interrupt message. When set, the device cannot generate legacy interrupt 
       messages. 
       			[br][b]Note:[/b] For dummy function this field is RO as 1.
       			
     */
    UINT32 command_reserved : 5;
    /* command_reserved - Bits[15:11], RSV, default = 5'h0 
       desc is missing
     */
    UINT32 status_reserved1 : 3;
    /* status_reserved1 - Bits[18:16], RSV, default = 3'h0 
       desc is missing
     */
    UINT32 status_interrupt_status : 1;
    /* status_interrupt_status - Bits[19:19], ROV, default = 1'h0 
       Interrupt Status. The Interrupt Status field is a RO field that indicates that 
       an interrupt message is pending internally to the device. 
     */
    UINT32 status_new_cap : 1;
    /* status_new_cap - Bits[20:20], RO, default = 1'h1 
       New Capabilities. Indicates that a device implements extended capabilities. The 
       LAN Controller sets this bit, and implements a capabilities list, to indicate 
       that it supports PCI Power Management, Message Signaled Interrupts (MSI), 
       Enhanced Message Signaled Interrupts (MSI-X), Vital Product Data (VPD), and the 
       PCIe extensions. 
     */
    UINT32 status_66mhz_capble : 1;
    /* status_66mhz_capble - Bits[21:21], RO, default = 1'h0 
       66MHz Capable. Hardwired to 0.
     */
    UINT32 status_reserved : 1;
    /* status_reserved - Bits[22:22], RSV, default = 1'h0 
       desc is missing
     */
    UINT32 status_fast_b2b_enable : 1;
    /* status_fast_b2b_enable - Bits[23:23], RO, default = 1'h0 
       Fast Back-to-Back Capable. Hardwired to 0.
     */
    UINT32 status_data_parity_reported : 1;
    /* status_data_parity_reported - Bits[24:24], RW1CS, default = 1'h0 
       Data Parity Reported. This bit is never set by the LAN device.
     */
    UINT32 status_devsel_timing : 2;
    /* status_devsel_timing - Bits[26:25], RO, default = 2'h0 
       DEVSEL Timing. Hardwired to 0.
     */
    UINT32 status_signaled_target_abort : 1;
    /* status_signaled_target_abort - Bits[27:27], RW1CS, default = 1'h0 
       Signaled Target Abort. This bit is never set by the LAN device.
     */
    UINT32 status_received_target_abort : 1;
    /* status_received_target_abort - Bits[28:28], RW1CS, default = 1'h0 
       Received Target Abort. This bit is never set by the LAN device.
     */
    UINT32 status_received_master_abort : 1;
    /* status_received_master_abort - Bits[29:29], RW1CS, default = 1'h0 
       Received Master Abort. This bit is never set by the LAN device.
     */
    UINT32 status_signaled_system_error : 1;
    /* status_signaled_system_error - Bits[30:30], RW1CS, default = 1'h0 
       Signaled System Error. Never expected to be asserted. This bit is never set by 
       the LAN device. 
     */
    UINT32 status_detected_parity_error : 1;
    /* status_detected_parity_error - Bits[31:31], RW1CS, default = 1'h0 
       Detected Parity Error. This bit is never set by the LAN device.&edsp;
     */
  } Bits;
  UINT32 Data;
} CMD_STAT_REG_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* REV_CLASS_CODE_GbE_VF0_REG supported on:                                   */
/*       BDX (0x40380008)                                                     */
/* Register default value:              0x02000001                            */
#define REV_CLASS_CODE_GbE_VF0_REG 0x1A004008

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 revision_id : 8;
    /* revision_id - Bits[7:0], ROV, default = 8'h01 
       
          The default revision ID is 0x01. The value of the rev ID is a logic XOR 
       between the default value and the value in EEPROM word 0x1E. Note that all LAN 
       functions have the same revision ID. 
          [br][b]Note:[/b] The default value is mirrored in the MREVID.Step_REV_ID 
       internal register. 
       			
     */
    UINT32 class_code : 24;
    /* class_code - Bits[31:8], RO, default = 24'h20000 
       
       			The class code is a RO hard-coded value that identifies its functionality.
          [br]LAN 0-3: 0x02_0000/0x01_0000 - Ethernet/SCSI Adapter. Selected according 
       to bit 11, 12, 13, or 14 in Device Rev ID EEPROM word for LAN0, LAN1, LAN2, or 
       LAN3, respectively. 
       			[br]Dummy Function: 0xFF_0000 - Other device
       			
     */
  } Bits;
  UINT32 Data;
} REV_CLASS_CODE_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CACHE_GbE_VF0_REG supported on:                                            */
/*       BDX (0x4038000C)                                                     */
/* Register default value:              0x00800020                            */
#define CACHE_GbE_VF0_REG 0x1A00400C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 cache_line_size : 8;
    /* cache_line_size - Bits[7:0], RO, default = 8'h20 
       
          This field is implemented by PCIe devices as a R/W field for legacy 
       compatibility purposes but has no impact on any PCIe device functionality. Field 
       is loaded from the PCIe Init Configuration 3 (Word 0x1A) EEPROM word and defines 
       cache line size in Dwords. All functions are initialized to the same value. In 
       EEPROM-less systems, the value is 0x20. 
       			
     */
    UINT32 latency_timer : 8;
    /* latency_timer - Bits[15:8], RO, default = 8'h0 
       desc is missing
     */
    UINT32 header_type : 8;
    /* header_type - Bits[23:16], RO, default = 8'h80 
       
          This indicates if a device is single-function or multifunction. If a single 
       LAN function is the only active one then this field has a vlaue of 0x00 to 
       indicate a single-function device. If other functions are enabled then this 
       field has a value of 0x80 to indicate a multi-function device.  
       			
     */
    UINT32 bist : 8;
    /* bist - Bits[31:24], RO, default = 8'h0 
       
       			BIST is not supported.
       			
     */
  } Bits;
  UINT32 Data;
} CACHE_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* BASE_ADDR_0_GbE_VF0_REG supported on:                                      */
/*       BDX (0x40380010)                                                     */
/* Register default value:              0x00000000                            */
#define BASE_ADDR_0_GbE_VF0_REG 0x1A004010

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * 
 *   The Base Address registers (BARs) are used to map register space of the 
 * various functions. It has a memory BAR, IO BAR, and MSI-X BAR. 64-bit addresses 
 * with 2 registers are used for each memory mapping window (BARCTRL.BAR32 bit is 
 * cleared; bit 10 in the Functions Control EEPROM word). 
 *   [br]The internal registers memories are accessed as direct memory mapped 
 * offsets from the Base Address register. Software can access a Dword or 64 bits.  
 * 		[br][b]Note:[/b] If this is a Dummy function the register is RO as 0.
 * 		
 */
typedef union {
  struct {
    UINT32 memory_io_space : 1;
    /* memory_io_space - Bits[0:0], RO, default = 1'h0 
       desc is missing
     */
    UINT32 memory_type : 2;
    /* memory_type - Bits[2:1], RO, default = 2'h0 
       
       			If this is a Dummy function the value is 00b; otherwise the value is 10b.
       			
     */
    UINT32 prefetchable_memory : 1;
    /* prefetchable_memory - Bits[3:3], RO, default = 1'h0 
       
          This is the 'low' address in conjunction with BASE_ADDR_1 of a 64-bit BAR to 
       Memory CSR. 
       			
     */
    UINT32 memory_size : 13;
    /* memory_size - Bits[16:4], RO, default = 13'h0 
       desc is missing
     */
    UINT32 base_addr : 15;
    /* base_addr - Bits[31:17], RO, default = 15'h0 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} BASE_ADDR_0_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* BASE_ADDR_1_GbE_VF0_REG supported on:                                      */
/*       BDX (0x40380014)                                                     */
/* Register default value:              0x00000000                            */
#define BASE_ADDR_1_GbE_VF0_REG 0x1A004014

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * 
 *   The Base Address registers (BARs) are used to map register space of the 
 * various functions. It has a memory BAR, IO BAR, and MSI-X BAR. 64-bit addresses 
 * with 2 registers are used for each memory mapping window (BARCTRL.BAR32 bit is 
 * cleared; bit 10 in the Functions Control EEPROM word). 
 *   [br]The internal registers memories are accessed as direct memory mapped 
 * offsets from the Base Address register. Software can access a Dword or 64 bits.  
 *   [br][b]Note:[/b] If this is a Dummy function the register is RO as 0; 
 * otherwise this register is R/W with a default of 0 and is the 'high' address in 
 * conjunction with BASE_ADDR_0 of a 64-bit BAR to Memory CSR. 
 * 		
 */
typedef union {
  struct {
    UINT32 base_addr : 32;
    /* base_addr - Bits[31:0], RO, default = 32'h0 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} BASE_ADDR_1_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* BASE_ADDR_2_GbE_VF0_REG supported on:                                      */
/*       BDX (0x40380018)                                                     */
/* Register default value:              0x00000000                            */
#define BASE_ADDR_2_GbE_VF0_REG 0x1A004018

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * 
 *   The Base Address registers (BARs) are used to map register space of the 
 * various functions. It has a memory BAR, IO BAR, and MSI-X BAR.  
 *   [br]All internal registers and memories can be accessed using I/O operations. 
 * There are two 4-byte registers in the IO mapping window: Addr Reg and Data Reg 
 * accessible as Dword entities. IO BAR support depends on the IO_Sup bit in the 
 * EEPROM PCIe Init Configuration 2 word. 
 * 		[br][b]Note:[/b] If this is a Dummy function the register is RO as 0.	
 * 		
 */
typedef union {
  struct {
    UINT32 memory_io_space : 1;
    /* memory_io_space - Bits[0:0], RO, default = 1'h0 
       desc is missing
     */
    UINT32 memory_type : 2;
    /* memory_type - Bits[2:1], RO, default = 2'h0 
       desc is missing
     */
    UINT32 prefetchable_memory : 1;
    /* prefetchable_memory - Bits[3:3], RO, default = 1'h0 
       desc is missing
     */
    UINT32 io_size : 1;
    /* io_size - Bits[4:4], RO, default = 1'h0 
       desc is missing
     */
    UINT32 base_addr : 27;
    /* base_addr - Bits[31:5], RO, default = 27'h0 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} BASE_ADDR_2_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* BASE_ADDR_3_GbE_VF0_REG supported on:                                      */
/*       BDX (0x4038001C)                                                     */
/* Register default value:              0x00000000                            */
#define BASE_ADDR_3_GbE_VF0_REG 0x1A00401C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * 
 * 		Reserved.	
 * 		
 */
typedef union {
  struct {
    UINT32 reserved : 32;
    /* reserved - Bits[31:0], RSV, default = 32'h0 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} BASE_ADDR_3_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* BASE_ADDR_4_GbE_VF0_REG supported on:                                      */
/*       BDX (0x40380020)                                                     */
/* Register default value:              0x00000004                            */
#define BASE_ADDR_4_GbE_VF0_REG 0x1A004020

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * 
 *   The Base Address registers (BARs) are used to map register space of the 
 * various functions. It has a memory BAR, IO BAR, and MSI-X BAR. 64-bit addresses 
 * with 2 registers are used for each memory mapping window (BARCTRL.BAR32 bit is 
 * cleared; bit 10 in the Functions Control EEPROM word). 
 *   [br]The MSI-X vectors and Pending Bit Array (PBA) structures are accessed as 
 * direct memory-mapped offsets from the MSI-X BAR. Software can access Dword 
 * entities. 
 *   [br][b]Note:[/b] If this is a Dummy function the register is RO as 0; 
 * otherwise this register is R/W with defaults as noted below and is the 'low' 
 * address in conjunction with BASE_ADDR_5 of a 64-bit MSI-X BAR.  
 * 		
 */
typedef union {
  struct {
    UINT32 memory_io_space : 1;
    /* memory_io_space - Bits[0:0], RO, default = 1'h0 
       desc is missing
     */
    UINT32 memory_type : 2;
    /* memory_type - Bits[2:1], RO, default = 2'b10 
       
          If BARCTRL.BAR32 = 0b and this is not a Dummy function the value is 10b; 
       otherwise the value is 00b. 
       			
     */
    UINT32 prefetchable_memory : 1;
    /* prefetchable_memory - Bits[3:3], RO, default = 1'h0 
       desc is missing
     */
    UINT32 memory_size : 10;
    /* memory_size - Bits[13:4], RO, default = 10'h0 
       desc is missing
     */
    UINT32 base_addr : 18;
    /* base_addr - Bits[31:14], RO, default = 18'h0 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} BASE_ADDR_4_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* BASE_ADDR_5_GbE_VF0_REG supported on:                                      */
/*       BDX (0x40380024)                                                     */
/* Register default value:              0x00000000                            */
#define BASE_ADDR_5_GbE_VF0_REG 0x1A004024

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * 
 *   The Base Address registers (BARs) are used to map register space of the 
 * various functions. It has a memory BAR, IO BAR, and MSI-X BAR. 64-bit addresses 
 * with 2 registers are used for each memory mapping window (BARCTRL.BAR32 bit is 
 * cleared; bit 10 in the Functions Control EEPROM word). 
 *   [br]The MSI-X vectors and Pending Bit Array (PBA) structures are accessed as 
 * direct memory-mapped offsets from the MSI-X BAR. Software can access Dword 
 * entities. 
 *   [br][b]Note:[/b] If this is a Dummy function the register is RO as 0; 
 * otherwise this register is R/W with a default of 0 and is the 'high' address in 
 * conjunction with BASE_ADDR_4 of a 64-bit MSI-X BAR.  
 * 		
 */
typedef union {
  struct {
    UINT32 base_addr : 32;
    /* base_addr - Bits[31:0], RO, default = 32'h0 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} BASE_ADDR_5_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CARDBUS_CIS_POINTER_GbE_VF0_REG supported on:                              */
/*       BDX (0x40380028)                                                     */
/* Register default value:              0x00000000                            */
#define CARDBUS_CIS_POINTER_GbE_VF0_REG 0x1A004028

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * 
 * 		Not used. Hardwired to 0.
 * 		
 */
typedef union {
  struct {
    UINT32 cardbus_cis_pointer : 32;
    /* cardbus_cis_pointer - Bits[31:0], RO, default = 32'h0 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} CARDBUS_CIS_POINTER_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* SUBSYSTEM_VENDOR_ID_GbE_VF0_REG supported on:                              */
/*       BDX (0x4038002C)                                                     */
/* Register default value:              0x00008086                            */
#define SUBSYSTEM_VENDOR_ID_GbE_VF0_REG 0x1A00402C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 subsystem_vendor_id : 16;
    /* subsystem_vendor_id - Bits[15:0], ROV, default = 16'h8086 
       
          This value can be loaded automatically from EEPROM address 0x0C at power-up 
       or reset. A value of 0x8086 is the default for this field at power-up if the 
       EEPROM does not respond or is not programmed. All functions are initialized to 
       the same value. 
       			
     */
    UINT32 subsystem_id : 16;
    /* subsystem_id - Bits[31:16], ROV, default = 16'h0 
       
          This value can be loaded automatically from EEPROM address 0x0B at power-up 
       with a default value of 0x0000. 
       			
     */
  } Bits;
  UINT32 Data;
} SUBSYSTEM_VENDOR_ID_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* ROM_BASE_ADDR_GbE_VF0_REG supported on:                                    */
/*       BDX (0x40380030)                                                     */
/* Register default value:              0x00000000                            */
#define ROM_BASE_ADDR_GbE_VF0_REG 0x1A004030

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * 
 *   This register is used to define the address and size information for boot-time 
 * access to the optional Flash memory. Expansion ROM is enabled by placing 0b in 
 * the LAN Boot Disable EEPROM bit for LAN0, LAN1, LAN2, and LAN3, respectively. 
 * This register returns a 0 value for functions without an expansion ROM window. 
 * 		[br][b]Note:[/b]For Dummy functions this register is RO with a value of 0.
 * 		
 */
typedef union {
  struct {
    UINT32 enable : 1;
    /* enable - Bits[0:0], RO, default = 1'h0 
       
       			0b - Disables expansion ROM access. 1b - Enables expansion ROM access.
       			
     */
    UINT32 reserved : 10;
    /* reserved - Bits[10:1], RO, default = 10'h0 
       desc is missing
     */
    UINT32 rom_base_addr : 21;
    /* rom_base_addr - Bits[31:11], RO, default = 21'h0 
       
          Read-write bits are hardwired to 0b and dependent on the memory mapping 
       window size. The LAN Expansion ROM spaces can be either 64KB or up to 8MB in 
       powers of 2. Mapping window size is set by the Flash Size EEPROM field. 
       			
     */
  } Bits;
  UINT32 Data;
} ROM_BASE_ADDR_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CAP_PTR_GbE_VF0_REG supported on:                                          */
/*       BDX (0x40380034)                                                     */
/* Register default value:              0x00000070                            */
#define CAP_PTR_GbE_VF0_REG 0x1A004034

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * 
 *   The Capabilities Pointer field is an 8-bit field that provides an offset in 
 * the device's PCI configuration space for the location of the first item in the 
 * Capabilities Linked List (CLL). Controller sets this bit and implements a 
 * capabilities list to indicate that it supports PCI power management, Message 
 * Signaled Interrupts (MSIs), and PCIe extended capabilities. Its value is 0x40, 
 * which is the address of the first entry: PCI power management. 
 * 		
 */
typedef union {
  struct {
    UINT32 cap_ptr : 8;
    /* cap_ptr - Bits[7:0], RO, default = 8'h70 
       desc is missing
     */
    UINT32 reserved : 24;
    /* reserved - Bits[31:8], RO, default = 24'h0 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} CAP_PTR_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* RSV_GbE_VF0_REG supported on:                                              */
/*       BDX (0x40380038)                                                     */
/* Register default value:              0x00000000                            */
#define RSV_GbE_VF0_REG 0x1A004038

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 rsv : 32;
    /* rsv - Bits[31:0], RSV, default = 32'h0 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} RSV_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* INTERRUPT_GbE_VF0_REG supported on:                                        */
/*       BDX (0x4038003C)                                                     */
/* Register default value:              0x00000100                            */
#define INTERRUPT_GbE_VF0_REG 0x1A00403C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 interrupt_line : 8;
    /* interrupt_line - Bits[7:0], RO, default = 8'h0 
       
          Read/write register programmed by software to indicate which of the system 
       interrupt request lines this controller's interrupt pin is bound to. See the 
       PCIe definition for more details. Each of the PCIe functions has its own 
       register. 
       			[br][b]Note:[/b]For Dummy functions this register is RO as zero.
       			
     */
    UINT32 interrupt_pin : 8;
    /* interrupt_pin - Bits[15:8], ROV, default = 8'h01 
       
          Read-only register. A value of 0x1, 0x2, 0x3, or 0x4 indicates that this 
       function implements legacy interrupt on INTA#, INTB#, INTC#, or INTD#, 
       respectively. Value is loaded from Initialization Control 3 (offset 0x24) EEPROM 
       words from relevant LAN0, LAN1, LAN2, LAN3 EEPROM sections. 
          [br][b]Note:[/b]If only a single port is enabled while the other ports are 
       disabled, the enabled port uses INTA, independent of the EEPROM setting. 
       			
     */
    UINT32 min_grant : 8;
    /* min_grant - Bits[23:16], RO, default = 8'h0 
       Not used. Hardwired to 0.
     */
    UINT32 max_latency : 8;
    /* max_latency - Bits[31:24], RO, default = 8'h0 
       Not used. Hardwired to 0.
     */
  } Bits;
  UINT32 Data;
} INTERRUPT_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MSIXC_GbE_VF0_REG supported on:                                            */
/*       BDX (0x40380070)                                                     */
/* Register default value:              0x0002A011                            */
#define MSIXC_GbE_VF0_REG 0x1A004070

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * This capability is not available for Dummy functions.
 */
typedef union {
  struct {
    UINT32 cap_id : 8;
    /* cap_id - Bits[7:0], RO, default = 8'h11 
       This field equals 0x11 indicating the linked list item as being the MSI-X 
       registers. 
     */
    UINT32 next_pointer : 8;
    /* next_pointer - Bits[15:8], RO, default = 8'hA0 
       This field provides an offset to the next capability item in the capability 
       list. Its value of 0xA0 points to the PCIe capability. 
     */
    UINT32 table_size : 11;
    /* table_size - Bits[26:16], RO, default = 11'h002 
       
          System software reads this field to determine the MSI-X Table Size N, which 
       is encoded as N-1. For example, a returned value of 0x00F indicates a table size 
       of 16. 
       			[br]Controller supports 10 MSI-X vectors.
          [br]This field is loaded from the MSI-X Configuration (offset 0x16) EEPROM 
       word. 
       			
     */
    UINT32 reserved : 3;
    /* reserved - Bits[29:27], RSV, default = 3'h0 
       desc is missing
     */
    UINT32 function_mask : 1;
    /* function_mask - Bits[30:30], RWS, default = 1'h0 
       
          If set to 1, all of the vectors associated with the function are masked, 
       regardless of their per-vector Mask bit states. 
          [br]If set to 0, each vector's Mask bit determines whether the vector is 
       masked or not. 
          [br]Setting or clearing the MSI-X Function Mask bit has no effect on the 
       state of the per-vector Mask bits. 
       			
     */
    UINT32 msix_en : 1;
    /* msix_en - Bits[31:31], RWS, default = 1'h0 
       
          If set to 1 and the MSI Enable bit in the MSI Message Control (MMC) register 
       is 0, the function is permitted to use MSI-X to request service and is 
       prohibited from using its INTx# pin. 
          [br]System configuration software sets this bit to enable MSI-X. A software 
       device driver is prohibited from writing this bit to mask a function's service 
       request. 
          [br]If set to 0, the function is prohibited from using MSI-X to request 
       service. 
       			
     */
  } Bits;
  UINT32 Data;
} MSIXC_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MSIXTOTB_GbE_VF0_REG supported on:                                         */
/*       BDX (0x40380074)                                                     */
/* Register default value:              0x00000003                            */
#define MSIXTOTB_GbE_VF0_REG 0x1A004074

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 tbir : 3;
    /* tbir - Bits[2:0], RO, default = 3'h3 
       
          Table BIR. Indicates which one of a function's BARs, located beginning at 
       0x10 in configuration space, is used to map the function's MSI-X table into 
       memory space. 
          [br]BIR values: 0..5 correspond to BARs 0x10..0x24 respectively. A BIR value 
       of 3 indicates that the table is mapped in BAR3 (address 0x1C). 
          [br]When BARCTRL.BAR32 = 0b (64-bit MMIO mapping) the table BIR equals 0x4. 
       When BARCTRL.BAR32 = 1b (32-bit MMIO mapping) the table BIR equals 0x3. 
       			
     */
    UINT32 toff : 29;
    /* toff - Bits[31:3], RO, default = 29'h0 
       
          Table Offset. Used as an offset from the address contained by one of the 
       function's BARs to point to the base of the MSI-X table. The lower three table 
       BIR bits are masked off (set to 0) by software to form a 32-bit Qword-aligned 
       offset. 
       			
     */
  } Bits;
  UINT32 Data;
} MSIXTOTB_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MSIXTPOPB_GbE_VF0_REG supported on:                                        */
/*       BDX (0x40380078)                                                     */
/* Register default value:              0x00002003                            */
#define MSIXTPOPB_GbE_VF0_REG 0x1A004078

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 pbir : 3;
    /* pbir - Bits[2:0], RO, default = 3'h3 
       
          PBA BIR. Indicates which one of a function's BARs, located beginning at 0x10 
       in configuration space, is used to map the function's MSI-X PBA into memory 
       space. 
          [br]BIR values: 0..5 correspond to BARs 0x10..0x24 respectively. A BIR value 
       of 3 indicates that the table is mapped in BAR3 (address 0x1C). 
          [br]When BARCTRL.BAR32 = 0b (64-bit MMIO mapping) the table BIR equals 0x4. 
       When BARCTRL.BAR32 = 1b (32-bit MMIO mapping) the table BIR equals 0x3. 
       			
     */
    UINT32 poff : 29;
    /* poff - Bits[31:3], RO, default = 29'h400 
       
          PBA Offset. Used as an offset from the address contained by one of the 
       function's BARs to point to the base of the MSI-X PBA. The lower three PBA BIR 
       bits are masked off (set to 0) by software to form a 32-bit Qword-aligned 
       offset. 
       			
     */
  } Bits;
  UINT32 Data;
} MSIXTPOPB_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CLIST_XCAP_GbE_VF0_REG supported on:                                       */
/*       BDX (0x403800A0)                                                     */
/* Register default value:              0x00920010                            */
#define CLIST_XCAP_GbE_VF0_REG 0x1A0040A0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * PCIe provides two mechanisms to support native features:
 *     [br]PCIe defines a PCI capability pointer indicating support for PCIe
 *     [br]PCIe extends the configuration space beyond the 256 bytes available for 
 * PCI to 4096 bytes 
 */
typedef union {
  struct {
    UINT32 cid : 8;
    /* cid - Bits[7:0], RO, default = 8'h10 
       Indicates this is a PCI Express capability.
     */
    UINT32 next : 8;
    /* next - Bits[15:8], RO, default = 8'h00 
       Offset to the next capability item in the capability list. Its value of 0xE0 
       points to the VPD structure. If VPD is disabled, operation is EEPROM-less, or 
       function is a Dummy function a value of 0x00 indicates that it is the last item 
       in the capability linked list. 
     */
    UINT32 cv : 4;
    /* cv - Bits[19:16], RO, default = 4'h2 
       Version 2.0 indicates devices compliant to the PCI Express 2.0 specification 
       which incorporates the Register Expansion ECN. 
     */
    UINT32 dt : 4;
    /* dt - Bits[23:20], RO, default = 4'b1001 
       Indicates the type of PCIe function. All functions are a root complex integrated 
       endpoint with a value of 1001b. 
     */
    UINT32 si : 1;
    /* si - Bits[24:24], RO, default = 1'b0 
       Controller does not implement slot options therefore this field is hardwired to 
       0. 
     */
    UINT32 imn : 5;
    /* imn - Bits[29:25], RO, default = 5'h00 
       Controller does not implement multiple MSI interrupts per function, therefore 
       this field is hardwired to 0. 
     */
    UINT32 rsvd : 2;
    /* rsvd - Bits[31:30], RSV, default = 2'b0 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} CLIST_XCAP_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DCAP_GbE_VF0_REG supported on:                                             */
/*       BDX (0x403800A4)                                                     */
/* Register default value:              0x10008CC2                            */
#define DCAP_GbE_VF0_REG 0x1A0040A4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * This register identifies the PCIe device specific capabilities. It is a 
 * read-only register with the same value for all functions. 
 */
typedef union {
  struct {
    UINT32 mps : 3;
    /* mps - Bits[2:0], RO, default = 3'b010 
       
        This field indicates the maximum payload that controller can support for TLPs. 
       It is loaded from the EEPROM's PCIe Init Configuration 3 word, 0x1A (with a 
       default value of 512 bytes). 
       	
     */
    UINT32 pfs : 2;
    /* pfs - Bits[4:3], RO, default = 2'b00 
       No phantom functions supported.
     */
    UINT32 etfs : 1;
    /* etfs - Bits[5:5], RO, default = 1'b0 
       Max supported size of the Tag field. Controller supports 5-bit Tag field for all 
       functions. 
     */
    UINT32 e0al : 3;
    /* e0al - Bits[8:6], RO, default = 3'b011 
       This field indicates the acceptable latency that controller can withstand due to 
       the transition from the L0s state to the L0 state. All functions share the same 
       value loaded from the EEPROM PCIe Init Configuration 1 word, 0x18. 
     */
    UINT32 e1al : 3;
    /* e1al - Bits[11:9], RO, default = 3'b110 
       This field indicates the acceptable latency that controller can withstand due to 
       the transition from the L1 state to the L0 state. All functions share the same 
       value loaded from the EEPROM PCIe L1 Exit latencies word, 0x14. 
     */
    UINT32 rsvd_4 : 1;
    /* rsvd_4 - Bits[12:12], RSV, default = 1'b0 
       On previous version of the specification this was Attention Button Present 
       (ABP). 
     */
    UINT32 rsvd_3 : 1;
    /* rsvd_3 - Bits[13:13], RSV, default = 1'b0 
       On previous version of the specification this was Attention Indicator Present 
       (AIP). 
     */
    UINT32 rsvd_2 : 1;
    /* rsvd_2 - Bits[14:14], RSV, default = 1'b0 
       On previous version of the specification this was Power Indicator Present (PIP).
     */
    UINT32 rber : 1;
    /* rber - Bits[15:15], RO, default = 1'b1 
       Indicates that this device implements the functionality defined in the Error 
       Reporting ECN as required by the PCI Express 1.1 spec. 
     */
    UINT32 rsvd_1 : 2;
    /* rsvd_1 - Bits[17:16], RSV, default = 2'b00 
       desc is missing
     */
    UINT32 cspv : 8;
    /* cspv - Bits[25:18], RO, default = 8'h00 
       Hardwired to 0 for all functions, as it consumes less than the 25W allowed for 
       its form factor. 
     */
    UINT32 csps : 2;
    /* csps - Bits[27:26], RO, default = 2'b00 
       Hardwired to 0 for all functions, as it consumes less than the 25W allowed for 
       its form factor. 
     */
    UINT32 flrc : 1;
    /* flrc - Bits[28:28], RO, default = 1'b1 
       A value of 1b indicates the function supports the optional FLR mechanism. Value 
       is loaded from EEPROM. 
     */
    UINT32 rsvd : 3;
    /* rsvd - Bits[31:29], RSV, default = 3'b000 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} DCAP_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DCTL_DSTS_GbE_VF0_REG supported on:                                        */
/*       BDX (0x403800A8)                                                     */
/* Register default value:              0x00000810                            */
#define DCTL_DSTS_GbE_VF0_REG 0x1A0040A8

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * This register contains the PCIe specific parameters. There is a dedicated 
 * register per each function. 
 */
typedef union {
  struct {
    UINT32 cee : 1;
    /* cee - Bits[0:0], RO, default = 1'b0 
       Enable report of correctable errors.
     */
    UINT32 nfe : 1;
    /* nfe - Bits[1:1], RO, default = 1'b0 
       Enable report of non-fatal errors.
     */
    UINT32 fee : 1;
    /* fee - Bits[2:2], RO, default = 1'b0 
       Enable report of fatal errors.
     */
    UINT32 ure : 1;
    /* ure - Bits[3:3], RO, default = 1'b0 
       Enable report of unsupported request errors.
     */
    UINT32 ero : 1;
    /* ero - Bits[4:4], RO, default = 1'b1 
       If this bit is set, controller is permitted to set the Relaxed Ordering bit in 
       the attribute field of write transactions that do not need strong ordering. For 
       more details, refer to the description about the RO_DIS bit in the CTRL_EXT 
       register bit. 
     */
    UINT32 mps : 3;
    /* mps - Bits[7:5], RO, default = 3'b000 
       
             This field sets maximum TLP payload size for controller functions. As a 
       receiver, controller must handle TLPs as large as the set value (default of 000b 
       = 128 bytes). As a transmitter, controller must not generate TLPs exceeding the 
       set value.  
             [br]The max payload size supported in controller Device capabilities 
       register indicates permissible values that can be programmed. 
             [br]When ARI support is exposed, the value set in function 0 (even when it 
       is a Dummy function) is the value used for all the functions' transactions. 
             
     */
    UINT32 etfe : 1;
    /* etfe - Bits[8:8], RO, default = 1'b0 
       Not supported.
     */
    UINT32 pfe : 1;
    /* pfe - Bits[9:9], RO, default = 1'b0 
       Not supported.
     */
    UINT32 apme : 1;
    /* apme - Bits[10:10], RO, default = 1'b0 
       When set, enables controller to draw AUX power independent of PME AUX power. 
       Controller is a multi-function device, therefore it is allowed to draw AUX power 
       if at least one of the functions has this bit set. 
     */
    UINT32 ens : 1;
    /* ens - Bits[11:11], RO, default = 1'b1 
       Snoop is gated by NONSNOOP bits in the GCR register in the CSR space.
     */
    UINT32 mrrs : 3;
    /* mrrs - Bits[14:12], RO, default = 3'b000 
       
       	This field sets maximum read request size for the Device as a requester.
       	[br]000b = 128 bytes
       	[br]001b = 256 bytes
       	[br]010b = 512 bytes (default value)
       	[br]011b = 1 KB
       	[br]100b = 2 KB
       	[br]101b = reserved
       	[br]110b = reserved
       	[br]111b = reserved
       	
     */
    UINT32 iflr : 1;
    /* iflr - Bits[15:15], RWS, default = 1'b0 
       A write of 1b initiates an FLR to the function. The value read by software from 
       this bit is always 0b. 
     */
    UINT32 ced : 1;
    /* ced - Bits[16:16], RW1CS, default = 1'b0 
       Indicates a correctable error was detected. Set when received an internal 
       correctable error from receiver errors / framing errors, TLP CRC error, dllp CRC 
       error, replay number rollover, or replay timeout. 
     */
    UINT32 nfed : 1;
    /* nfed - Bits[17:17], RW1CS, default = 1'b0 
       Indicates a non-fatal error was detected. Set when received a non-fatal error 
       occurred from a poisoned TLP, unexpected completions, unsupported requests, 
       completor abort, or completer timeout. 
     */
    UINT32 fed : 1;
    /* fed - Bits[18:18], RW1CS, default = 1'b0 
       Indicates a fatal error was detected. Set when a fatal error occurred from a 
       data link protocol error, buffer overflow, or malformed TLP. 
     */
    UINT32 urd : 1;
    /* urd - Bits[19:19], RW1CS, default = 1'b0 
       Indicates that controller received an unsupported request. This field is 
       identical in all functions. Controller cannot distinguish which function caused 
       an error. 
     */
    UINT32 apd : 1;
    /* apd - Bits[20:20], RO, default = 1'b0 
       If aux power is detected, this field is set to 1b. It is a strapping signal from 
       the periphery identical for all functions. Reset on LAN_PWR_GOOD and GIO Power 
       Good only. 
     */
    UINT32 tdp : 1;
    /* tdp - Bits[21:21], ROV, default = 1'b0 
       Indicates whether controller has any transaction pending. (Transactions include 
       completions for any outstanding non-posted request for all used traffic 
       classes.) 
     */
    UINT32 rsvd : 10;
    /* rsvd - Bits[31:22], RSV, default = 10'h000 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} DCTL_DSTS_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DCAP2_GbE_VF0_REG supported on:                                            */
/*       BDX (0x403800C4)                                                     */
/* Register default value:              0x0000081F                            */
#define DCAP2_GbE_VF0_REG 0x1A0040C4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * This register identifies PCIe device specific capabilities. It is a read only 
 * register with the same value for all functions. 
 */
typedef union {
  struct {
    UINT32 ctrs : 4;
    /* ctrs - Bits[3:0], RO, default = 4'hF 
       This field indicates device support for the optional Completion Timeout 
       programmability mechanism. This mechanism allows system software to modify the 
       Completion Timeout value. 
       [br]Four time value ranges are defined:
       [br]Range A: 50us to 10ms
       [br]Range B: 10ms to 250ms
       [br]Range C: 250ms to 4s
       [br]Range D: 4s to 64s
       [br]A value of 1111b indicates device supports ranges A, B, C, and D.
     */
    UINT32 ctds : 1;
    /* ctds - Bits[4:4], RO, default = 1'b1 
       A value of 1b indicates support for the Completion Timeout Disable mechanism. 
       For Dummy functions this field is RO as 0. 
     */
    UINT32 rsvd_2 : 6;
    /* rsvd_2 - Bits[10:5], RSV, default = 6'h00 
       desc is missing
     */
    UINT32 ltrms : 1;
    /* ltrms - Bits[11:11], RO, default = 1'b1 
       A value of 1b indicates support for the optional Latency Tolerance Reporting 
       (LTR) mechanism capability. 
       [br][b]Note:[/b]Value loaded from LTR_EN bit in Initialization Control Word 1 
       EEPROM word. 
       
     */
    UINT32 rsvd_1 : 6;
    /* rsvd_1 - Bits[17:12], RSV, default = 6'h00 
       desc is missing
     */
    UINT32 obffs : 2;
    /* obffs - Bits[19:18], RO, default = 2'b00 
       
       [br]00b - OBFF is not supported.
       [br]01b - OBFF is supported using Message signaling only.
       [br]10b - OBFF is supported using WAKE# signaling only.
       [br]11b - OBFF is supported using WAKE# and Message signaling.
       [br]Each function must report the same value for this field. Value is loaded 
       from EEPROM. 
     */
    UINT32 rsvd : 12;
    /* rsvd - Bits[31:20], RSV, default = 12'h000 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} DCAP2_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DCTL2_DSTS2_GbE_VF0_REG supported on:                                      */
/*       BDX (0x403800C8)                                                     */
/* Register default value:              0x00000000                            */
#define DCTL2_DSTS2_GbE_VF0_REG 0x1A0040C8

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 ctv : 4;
    /* ctv - Bits[3:0], RO, default = 4'h0 
       In Devices that support Completion Timeout programmability, this field allows 
       system software to modify the Completion Timeout value. This field is applicable 
       to Root Ports, Endpoints that issue requests on their own behalf, and PCI 
       Express to PCI/PCI-X Bridges that take ownership of requests issued on PCI 
       Express. For all other devices this field is reserved and must be hardwired to 
       0000b. 
       A Device that does not support this optional capability must hardwire this field 
       to 0000b and is required to implement a timeout value in the range 50us to 50ms. 
       Devices that support Completion Timeout programmability must support the values 
       given below corresponding to the programmability ranges indicated in the 
       Completion Timeout Values Supported field. 
       The PCH targeted configurable ranges are listed below, along with the range 
       allowed by the PCI Express 2.0 specification. 
       Defined encodings:
       0000b	Default range: 40-50ms (spec range 50us to 50ms)
       Values available if Range B (10ms to 250ms) programmability range is supported:
       0101b	40-50ms (spec range is 16ms to 55ms)
       0110b	160-170ms (spec range is 65ms to 210ms)
       Values available if Range C (250ms to 4s) programmability range is supported:
       1001b	400-500ms (spec range is 260ms to 900ms)
       1010b	1.6-1.7s (spec range is 1s to 3.5s)
       Values not defined above are Reserved.
       Software is permitted to change the value in this field at any time. For 
       requests already pending when the Completion Timeout Value is changed, hardware 
       is permitted to use either the new or the old value for the outstanding 
       requests, and is permitted to base the start time for each request either on 
       when this value was changed or on when each request was issued. 
     */
    UINT32 ctd : 1;
    /* ctd - Bits[4:4], RO, default = 1'b0 
       When set to 1b, this bit disables the Completion Timeout mechanism.
       This field is required for all devices that support the Completion Timeout 
       Disable Capability. 
       Software is permitted to set or clear this bit at any time. When set, the 
       Completion Timeout detection mechanism is disabled. 
       If there are outstanding requests when the bit is cleared, it is permitted but 
       not required for hardware to apply the completion timeout mechanism to the 
       outstanding requests. If this is done, it is permitted to base the start time 
       for each request on either the time this bit was cleared or the time each 
       request was issued. 
       Only the value from Port 1 (for ports 1-4) or Port 5 (for ports 5-8) is used.
     */
    UINT32 rsvd_3 : 5;
    /* rsvd_3 - Bits[9:5], RSV, default = 5'h00 
       desc is missing
     */
    UINT32 ltren : 1;
    /* ltren - Bits[10:10], RO, default = 1'b0 
       When Set to 1b, this bit enables the Latency Tolerance Reporting (LTR) 
       mechanism. 
       For Downstream Ports, this bit must be reset to the default value if the Port 
       goes to DL_Down status. 
       If DCAP2.LTRMS is clear, programming this field to any non-zero values will have 
       no effect. 
     */
    UINT32 rsvd_2 : 2;
    /* rsvd_2 - Bits[12:11], RSV, default = 2'b00 
       desc is missing
     */
    UINT32 obffen : 2;
    /* obffen - Bits[14:13], RO, default = 2'b00 
       00b - Disable OBFF mechanism.
       01b - Enable OBFF mechanism using Message signaling (Variation A).
       10b - Enable OBFF mechanism using Message signaling (Variation B).
       11b - Enable OBFF using WAKE# signaling.
       
       Note: Only encoding 00b and 11b are supported. The encoding of 01b or 10b would 
       be aliased to 00b. 
       If DCAP2.OBFFS is clear, programming this field to any non-zero values will have 
       no effect. 
     */
    UINT32 rsvd_1 : 1;
    /* rsvd_1 - Bits[15:15], RSV, default = 1'b0 
       desc is missing
     */
    UINT32 rsvd : 16;
    /* rsvd - Bits[31:16], RSV, default = 16'h0000 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} DCTL2_DSTS2_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* AECH_GbE_VF0_REG supported on:                                             */
/*       BDX (0x40380100)                                                     */
/* Register default value:              0x15010001                            */
#define AECH_GbE_VF0_REG 0x1A004100

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * The PCIe AER capability is an optional extended capability to support advanced 
 * error reporting. 
 */
typedef union {
  struct {
    UINT32 cid : 16;
    /* cid - Bits[15:0], RO, default = 16'h0001 
       PCIe extended capability ID indicating AER capability.
     */
    UINT32 cv : 4;
    /* cv - Bits[19:16], ROV, default = 4'h1 
       PCIe AER extended capability version number.
             [br][b]Note: [/b] Loaded from EEPROM.
     */
    UINT32 nco : 12;
    /* nco - Bits[31:20], ROV, default = 12'h150 
       Next PCIe extended capability pointer. A value of 0x140 points to the serial ID 
       capability. In EEPROM-less systems or when serial ID is disabled in the EEPROM, 
       the next pointer is 0x150 and points to the ARI capability structure. If ARI is 
       also disabled in the EEPROM the next pointer is 0x160 and points to the IOV 
       capability structure. If IOV is also disabled in the EEPROM this field points to 
       the TPH capability structure (0x1A0). 
             [br][b]Note: [/b]If function is a dummy function, depending on the 
       capability structures disabled in the EEPROM, this field can point to either 
       0x140 (Serial ID), 0x150 (ARI), 0x1C0 (LTR) or 0x1D0 (ACS). 
     */
  } Bits;
  UINT32 Data;
} AECH_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* UES_GbE_VF0_REG supported on:                                              */
/*       BDX (0x40380104)                                                     */
/* Register default value:              0x00000000                            */
#define UES_GbE_VF0_REG 0x1A004104

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * The Uncorrectable Error Status register reports error status of individual 
 * uncorrectable error sources on a PCIe device. An individual error status bit 
 * that is set to 1b indicates that a particular error occurred; software can clear 
 * an error status by writing a 1b to the respective bit. 
 */
typedef union {
  struct {
    UINT32 rsvd_2 : 4;
    /* rsvd_2 - Bits[3:0], RO, default = 4'b0000 
       desc is missing
     */
    UINT32 dlpe : 1;
    /* dlpe - Bits[4:4], RO, default = 1'b0 
       Indicates a data link protocol error occurred.
     */
    UINT32 sde : 1;
    /* sde - Bits[5:5], RO, default = 1'b0 
       Suprise Down is not supported.
     */
    UINT32 rsvd_1 : 6;
    /* rsvd_1 - Bits[11:6], RSV, default = 6'h00 
       desc is missing
     */
    UINT32 pt : 1;
    /* pt - Bits[12:12], RW1CS, default = 1'b0 
       Indicates a poisoned TLP was received.
     */
    UINT32 fcpe : 1;
    /* fcpe - Bits[13:13], RO, default = 1'b0 
       Indicates a flow control protocol error occurred.
     */
    UINT32 ct : 1;
    /* ct - Bits[14:14], RW1CS, default = 1'b0 
       Indicates a completion timed out. This is signaled if Completion Timeout is 
       enabled and a completion fails to return within the amount of time specified by 
       the Completion Timeout Value. 
     */
    UINT32 ca : 1;
    /* ca - Bits[15:15], RW1CS, default = 1'b0 
       Indicates a completor abort was received.
     */
    UINT32 uc : 1;
    /* uc - Bits[16:16], RW1CS, default = 1'b0 
       Indicates an unexpected completion was received.
     */
    UINT32 ro : 1;
    /* ro - Bits[17:17], RO, default = 1'b0 
       Indicates a receiver overflow occurred.
     */
    UINT32 mt : 1;
    /* mt - Bits[18:18], RO, default = 1'b0 
       Indicates a malformed TLP was received.
     */
    UINT32 ee : 1;
    /* ee - Bits[19:19], RO, default = 1'b0 
       Indicates an ECRC error occurred.
     */
    UINT32 ure : 1;
    /* ure - Bits[20:20], RW1CS, default = 1'b0 
       Indicates an unsupported request was received.
     */
    UINT32 avs : 1;
    /* avs - Bits[21:21], RO, default = 1'b0 
       Reserved. Access Control Services are not supported.
     */
    UINT32 rsvd : 10;
    /* rsvd - Bits[31:22], RO, default = 10'h000 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} UES_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* UEM_GbE_VF0_REG supported on:                                              */
/*       BDX (0x40380108)                                                     */
/* Register default value:              0x00000000                            */
#define UEM_GbE_VF0_REG 0x1A004108

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * The Uncorrectable Error Mask register controls reporting of individual 
 * uncorrectable errors by device to the host bridge via a PCIe error message. A 
 * masked error (respective bit set in mask register) is not reported to the host 
 * bridge by an individual device. There is a mask bit per bit in the Uncorrectable 
 * Error Status register. 
 */
typedef union {
  struct {
    UINT32 rsvd_2 : 4;
    /* rsvd_2 - Bits[3:0], RSV, default = 4'b0000 
       desc is missing
     */
    UINT32 dlpe : 1;
    /* dlpe - Bits[4:4], RWP, default = 1'b0 
       Mask for data link protocol errors.
     */
    UINT32 sde : 1;
    /* sde - Bits[5:5], RO, default = 1'b0 
       Suprise Down is not supported.
     */
    UINT32 rsvd_1 : 6;
    /* rsvd_1 - Bits[11:6], RSV, default = 6'h00 
       desc is missing
     */
    UINT32 pt : 1;
    /* pt - Bits[12:12], RWP, default = 1'b0 
       Mask for poisoned TLPs.
     */
    UINT32 fcpe : 1;
    /* fcpe - Bits[13:13], RWP, default = 1'b0 
       Mask for flow control protocol error.
     */
    UINT32 ct : 1;
    /* ct - Bits[14:14], RWP, default = 1'b0 
       Mask for completion timeouts.
     */
    UINT32 cm : 1;
    /* cm - Bits[15:15], RWP, default = 1'b0 
       Mask for completer abort.
     */
    UINT32 uc : 1;
    /* uc - Bits[16:16], RWP, default = 1'b0 
       Mask for unexpected completions.
     */
    UINT32 ro : 1;
    /* ro - Bits[17:17], RWP, default = 1'b0 
       Mask for receiver overflows.
     */
    UINT32 mt : 1;
    /* mt - Bits[18:18], RWP, default = 1'b0 
       Mask for malformed TLPs.
     */
    UINT32 ee : 1;
    /* ee - Bits[19:19], RWP, default = 1'b0 
       Mask for ECRC errors.
     */
    UINT32 ure : 1;
    /* ure - Bits[20:20], RWP, default = 1'b0 
       Mask for uncorrectable errors.
     */
    UINT32 avs : 1;
    /* avs - Bits[21:21], RO, default = 1'b0 
       Reserved. Access Control Services are not supported.
     */
    UINT32 rsvd : 10;
    /* rsvd - Bits[31:22], RSV, default = 10'h000 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} UEM_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* UEV_GbE_VF0_REG supported on:                                              */
/*       BDX (0x4038010C)                                                     */
/* Register default value:              0x00000001                            */
#define UEV_GbE_VF0_REG 0x1A00410C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * The Uncorrectable Error Severity register controls whether an individual 
 * uncorrectable error is reported as a fatal error. An uncorrectable error is 
 * reported as fatal when the corresponding error bit in the severity register is 
 * set. If the bit is cleared, the corresponding error is considered non-fatal. 
 */
typedef union {
  struct {
    UINT32 rsvd_2 : 4;
    /* rsvd_2 - Bits[3:0], RO, default = 4'b0001 
       desc is missing
     */
    UINT32 dlpe : 1;
    /* dlpe - Bits[4:4], RWP, default = 1'b0 
       Severity for data link protocol errors.
     */
    UINT32 sde : 1;
    /* sde - Bits[5:5], RO, default = 1'b0 
       Suprise Down is not supported.
     */
    UINT32 rsvd_1 : 6;
    /* rsvd_1 - Bits[11:6], RSV, default = 6'h00 
       desc is missing
     */
    UINT32 pt : 1;
    /* pt - Bits[12:12], RWP, default = 1'b0 
       Severity for poisoned TLP reception.
     */
    UINT32 fcpe : 1;
    /* fcpe - Bits[13:13], RWP, default = 1'b0 
       Severity for flow control protocol error.
     */
    UINT32 ct : 1;
    /* ct - Bits[14:14], RWP, default = 1'b0 
       Severity for completion timeout.
     */
    UINT32 ca : 1;
    /* ca - Bits[15:15], RWP, default = 1'b0 
       Severity for completor abort.
     */
    UINT32 uc : 1;
    /* uc - Bits[16:16], RWP, default = 1'b0 
       Severity for unexpected completion reception.
     */
    UINT32 ro : 1;
    /* ro - Bits[17:17], RWP, default = 1'b0 
       Severity for receiver overflow occurrences.
     */
    UINT32 mt : 1;
    /* mt - Bits[18:18], RWP, default = 1'b0 
       Severity for malformed TLP reception.
     */
    UINT32 ee : 1;
    /* ee - Bits[19:19], RWP, default = 1'b0 
       Severity for ECRC error.
     */
    UINT32 ure : 1;
    /* ure - Bits[20:20], RWP, default = 1'b0 
       Severity for unsupported request reception.
     */
    UINT32 avs : 1;
    /* avs - Bits[21:21], RWP, default = 1'b0 
       Severity for Access Control Services.
     */
    UINT32 rsvd : 10;
    /* rsvd - Bits[31:22], RO, default = 10'h000 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} UEV_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CES_GbE_VF0_REG supported on:                                              */
/*       BDX (0x40380110)                                                     */
/* Register default value:              0x00000000                            */
#define CES_GbE_VF0_REG 0x1A004110

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * The Correctable Error Status register reports error status of individual 
 * correctable error sources on a PCIe device. When an individual error status bit 
 * is set to 1b, it indicates that a particular error occurred; software can clear 
 * an error status by writing a 1b to the respective bit. 
 */
typedef union {
  struct {
    UINT32 re : 1;
    /* re - Bits[0:0], RW1CS, default = 1'b0 
       Indicates a receiver error occurred.
     */
    UINT32 rsvd_2 : 5;
    /* rsvd_2 - Bits[5:1], RO, default = 5'h00 
       desc is missing
     */
    UINT32 bt : 1;
    /* bt - Bits[6:6], RW1CS, default = 1'b0 
       Indicates a bad TLP was received.
     */
    UINT32 bd : 1;
    /* bd - Bits[7:7], RW1CS, default = 1'b0 
       Indicates a bad DLLP was received.
     */
    UINT32 rnr : 1;
    /* rnr - Bits[8:8], RW1CS, default = 1'b0 
       Indicates the replay number rolled over.
     */
    UINT32 rsvd_1 : 3;
    /* rsvd_1 - Bits[11:9], RO, default = 3'b000 
       desc is missing
     */
    UINT32 rtt : 1;
    /* rtt - Bits[12:12], RW1CS, default = 1'b0 
       Indicates the replay timer timed out.
     */
    UINT32 anfes : 1;
    /* anfes - Bits[13:13], RW1CS, default = 1'b0 
       When set, indicates that a Advisory Non-Fatal Error occured.
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], RO, default = 18'h00000 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} CES_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CEM_GbE_VF0_REG supported on:                                              */
/*       BDX (0x40380114)                                                     */
/* Register default value:              0x00002000                            */
#define CEM_GbE_VF0_REG 0x1A004114

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * The Correctable Error Mask register controls reporting of individual correctable 
 * errors by device to the host bridgge via a PCIe error message. A masked error 
 * (respective bit set in mask register) is not reported to the host bridge by an 
 * individual device. There is a mask bit per bit in the Correctable Error Status 
 * register. 
 */
typedef union {
  struct {
    UINT32 re : 1;
    /* re - Bits[0:0], RWP, default = 1'b0 
       Mask for receiver errors.
     */
    UINT32 rsvd_2 : 5;
    /* rsvd_2 - Bits[5:1], RO, default = 5'h00 
       desc is missing
     */
    UINT32 bt : 1;
    /* bt - Bits[6:6], RWP, default = 1'b0 
       Mask for bad TLP reception.
     */
    UINT32 bd : 1;
    /* bd - Bits[7:7], RWP, default = 1'b0 
       Mask for bad DLLP reception.
     */
    UINT32 rnr : 1;
    /* rnr - Bits[8:8], RWP, default = 1'b0 
       Mask for replay number rollover.
     */
    UINT32 rsvd_1 : 3;
    /* rsvd_1 - Bits[11:9], RO, default = 3'b000 
       desc is missing
     */
    UINT32 rtt : 1;
    /* rtt - Bits[12:12], RWP, default = 1'b0 
       Mask for replay timer timeout.
     */
    UINT32 anfem : 1;
    /* anfem - Bits[13:13], RWP, default = 1'b1 
       When set, masks Advisory Non-Fatal errors from (a) signaling ERR_COR to the 
       device control register and (b) updating the Uncorrectable Error Status 
       register. 
       This register is set by default to enable compatibility with software that does 
       not comprehend Role-Based Error Reporting. 
     */
    UINT32 rsvd : 18;
    /* rsvd - Bits[31:14], RO, default = 18'h00000 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} CEM_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* AECC_GbE_VF0_REG supported on:                                             */
/*       BDX (0x40380118)                                                     */
/* Register default value:              0x000000A0                            */
#define AECC_GbE_VF0_REG 0x1A004118

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * This register is only reset by a loss of core power.
 */
typedef union {
  struct {
    UINT32 fep : 5;
    /* fep - Bits[4:0], ROV, default = 5'b00000 
       The First Error Pointer is a field that identifies the bit position of the first 
       error reported in the Uncorrectable Error Status register. 
     */
    UINT32 egc : 1;
    /* egc - Bits[5:5], ROV, default = 1'b1 
       This bit indicates that the device is capable of generating ECRC. This bit is 
       loaded from EEPROM PCIe Control 2 word (Word 0x28). 
     */
    UINT32 ege : 1;
    /* ege - Bits[6:6], RWP, default = 1'b0 
       When set, enables ECRC generation.
     */
    UINT32 ecc : 1;
    /* ecc - Bits[7:7], ROV, default = 1'b1 
       If set, this bit indicates that the function is capable of checking ECRC. This 
       bit is loaded from EEPROM PCIe Control 2 word (Word 0x28). 
     */
    UINT32 ece : 1;
    /* ece - Bits[8:8], RWP, default = 1'b0 
       When set, enables ECRC checking.
     */
    UINT32 rsvd : 23;
    /* rsvd - Bits[31:9], RO, default = 23'h000000 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} AECC_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* HL_DW1_GbE_VF0_REG supported on:                                           */
/*       BDX (0x4038011C)                                                     */
/* Register default value:              0x00000000                            */
#define HL_DW1_GbE_VF0_REG 0x1A00411C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * These registers report the header for the TLP corresponding to a detected error. 
 * This register is only reset by a loss of core power. 
 */
typedef union {
  struct {
    UINT32 dw1 : 32;
    /* dw1 - Bits[31:0], ROV, default = 32'h00000000 
       Byte0 && Byte1 && Byte2 && Byte3
     */
  } Bits;
  UINT32 Data;
} HL_DW1_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* HL_DW2_GbE_VF0_REG supported on:                                           */
/*       BDX (0x40380120)                                                     */
/* Register default value:              0x00000000                            */
#define HL_DW2_GbE_VF0_REG 0x1A004120

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * These registers report the header for the TLP corresponding to a detected error. 
 * This register is only reset by a loss of core power. 
 */
typedef union {
  struct {
    UINT32 dw2 : 32;
    /* dw2 - Bits[31:0], ROV, default = 32'h00000000 
       Byte4 && Byte5 && Byte6 && Byte7
     */
  } Bits;
  UINT32 Data;
} HL_DW2_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* HL_DW3_GbE_VF0_REG supported on:                                           */
/*       BDX (0x40380124)                                                     */
/* Register default value:              0x00000000                            */
#define HL_DW3_GbE_VF0_REG 0x1A004124

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * These registers report the header for the TLP corresponding to a detected error. 
 * This register is only reset by a loss of core power. 
 */
typedef union {
  struct {
    UINT32 dw3 : 32;
    /* dw3 - Bits[31:0], ROV, default = 32'h00000000 
       Byte8 && Byte9 && Byte10 && Byte11
     */
  } Bits;
  UINT32 Data;
} HL_DW3_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* HL_DW4_GbE_VF0_REG supported on:                                           */
/*       BDX (0x40380128)                                                     */
/* Register default value:              0x00000000                            */
#define HL_DW4_GbE_VF0_REG 0x1A004128

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * These registers report the header for the TLP corresponding to a detected error. 
 * This register is only reset by a loss of core power. 
 */
typedef union {
  struct {
    UINT32 dw4 : 32;
    /* dw4 - Bits[31:0], ROV, default = 32'h00000000 
       Byte12 && Byte13 && Byte14 && Byte15
     */
  } Bits;
  UINT32 Data;
} HL_DW4_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DEVSERNUM_GbE_VF0_REG supported on:                                        */
/*       BDX (0x40380140)                                                     */
/* Register default value:              0x15010003                            */
#define DEVSERNUM_GbE_VF0_REG 0x1A004140

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * The extended capability ID for the device serial number capability is 0x0003.
 */
typedef union {
  struct {
    UINT32 cid : 16;
    /* cid - Bits[15:0], RO, default = 16'h0003 
       This field is a PCI-SIG defined ID number that indicates the nature and format 
       of the extended capability. The extended capability ID for the device serial 
       number capability is 0x0003. 
     */
    UINT32 cv : 4;
    /* cv - Bits[19:16], RO, default = 4'h1 
       This field is a PCI-SIG defined version number that indicates the version of the 
       current capability structure. 
     */
    UINT32 nco : 12;
    /* nco - Bits[31:20], RO, default = 12'h150 
       This field contains the offset to the next PCIe capability structure or 0x000 if 
       no other items exist in the linked list of capabilities. 
        [br]In a LAN function the value of this field is 0x150 to point to the ARI 
       capability structure. If ARI is disabled value of field is 0x160 that points to 
       the IOV capability structure. If IOV is also disabled in the EEPROM, then this 
       field is 0x1A0 that points to the TPH capabilities structure. 
        [br]In a Dummy function the value of this field is 0x150 to point to the ARI 
       capability structure. If ARI is disabled in the EEPROM, and LTR is enabled in 
       the EEPROM then the value of this field is 0x1C0 (LTR capability structure), 
       otherwise the value in this field is 0x1D0 (ACS capability structure). 
     */
  } Bits;
  UINT32 Data;
} DEVSERNUM_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* SERIALNUM_DW1_GbE_VF0_REG supported on:                                    */
/*       BDX (0x40380144)                                                     */
/* Register default value:              0xFFC9A000                            */
#define SERIALNUM_DW1_GbE_VF0_REG 0x1A004144

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * The Serial Number register is a 64-bit field that contains the IEEE defined 
 * 64-bit extended unique identifier (EUI-64). 
 */
typedef union {
  struct {
    UINT32 serialnum : 32;
    /* serialnum - Bits[31:0], ROV, default = 32'hFFC9A000 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} SERIALNUM_DW1_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* SERIALNUM_DW2_GbE_VF0_REG supported on:                                    */
/*       BDX (0x40380148)                                                     */
/* Register default value:              0x000000FF                            */
#define SERIALNUM_DW2_GbE_VF0_REG 0x1A004148

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * The Serial Number register is a 64-bit field that contains the IEEE defined 
 * 64-bit extended unique identifier (EUI-64). 
 */
typedef union {
  struct {
    UINT32 serialnum : 32;
    /* serialnum - Bits[31:0], ROV, default = 32'h000000FF 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} SERIALNUM_DW2_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* ARIHEADER_GbE_VF0_REG supported on:                                        */
/*       BDX (0x40380150)                                                     */
/* Register default value:              0x1A01000E                            */
#define ARIHEADER_GbE_VF0_REG 0x1A004150

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * In order to enable more than eight functions per endpoint without requesting an 
 * internal switch (typically needed in virtualization scenarios), the PCIsig 
 * defines a new capability that enables a different interpretation of the Bus, 
 * Device, and Function fields. The Alternate Requester ID Interpretation (ARI) 
 * capability structure follows. 
 */
typedef union {
  struct {
    UINT32 cid : 16;
    /* cid - Bits[15:0], RO, default = 16'h000E 
       PCIe extended capability ID for the ARI.
     */
    UINT32 cv : 4;
    /* cv - Bits[19:16], RO, default = 4'h1 
       This field is a PCI-SIG defined version number that indicates the version of the 
       current capability structure. Must be 0x1 for this version of the specification. 
     */
    UINT32 nco : 12;
    /* nco - Bits[31:20], RO, default = 12'h1A0 
       This field contains the offset to the next PCIe capability structure. The value 
       of 0x160 points to the IOV structure. If IOV is disabled the value of the field 
       is 0x1A0 that points to the TPH capability structure. 
        [br]For a dummy function, the next capability structure is LTR (value of 
       0x1C0), if LTR is disabled the next structure is ACS (value of 0x1D0). 
     */
  } Bits;
  UINT32 Data;
} ARIHEADER_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* ARICAPCTRL_GbE_VF0_REG supported on:                                       */
/*       BDX (0x40380154)                                                     */
/* Register default value:              <could not determine>                 */
#define ARICAPCTRL_GbE_VF0_REG 0x1A004154

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 mfvc : 1;
    /* mfvc - Bits[0:0], RO, default = 1'b0 
       Applicable only to function 0; must be 0b for all other functions. If 1b, 
       indicates that the ARI device supports function group level arbitration via its 
       Multi-Function Virtual Channel (MFVC) Capability structure. Not supported. 
     */
    UINT32 acs : 1;
    /* acs - Bits[1:1], RO, default = 1'b0 
       Applicable only to function 0; must be 0b for all other functions. If 1b, 
       indicates that the ARI device supports function group level granularity for ACS 
       P2P egress control via its ACS capability structure. Not supported. 
     */
    UINT32 reserved2 : 6;
    /* reserved2 - Bits[7:2], RSV, default = 6'h00 
       desc is missing
     */
    UINT32 nfp : 8;
    /* nfp - Bits[15:8], ROV, default = 0x0 
       This field contains the pointer to the next physical function configuration 
       space or 0x0000 if no other items exist in the linked list of functions. 
       Function 0 is the start of the link list of functions. 
       	[br]0x1 (function 0)
       	[br]0x2 (function 1)
       	[br]0x3 (function 2)
       	[br]0x0 (function 3)
        [br][b]Note: [/b]If port 0, port 1, port 2, or port 3 are switched or function 
       0 is a dummy function, this register should keep its attributes according to the 
       function number. If part of the LANs are disabled, then the value of this field 
       should create a valid link list between all the functions that are enabled. In 
       the last function this field should be zero. 
       	
     */
    UINT32 m_en : 1;
    /* m_en - Bits[16:16], RO, default = 1'b0 
       Applicable only for function 0; must be hardwired to 0b for all other functions. 
       When set, the ARI device must interpret entries in its function arbitration 
       table as function group numbers rather than function numbers. Not supported. 
     */
    UINT32 a_en : 1;
    /* a_en - Bits[17:17], RO, default = 1'b0 
       Applicable only for function 0; must be hardwired to 0b for all other functions. 
       When set, each function in the ARI device must associate bits within its egress 
       control vector with function group numbers rather than function numbers. Not 
       supported. 
     */
    UINT32 reserved18 : 2;
    /* reserved18 - Bits[19:18], RSV, default = 2'b00 
       desc is missing
     */
    UINT32 fgn : 3;
    /* fgn - Bits[22:20], RO, default = 3'b000 
       Not supported.
     */
    UINT32 reserved23 : 9;
    /* reserved23 - Bits[31:23], RSV, default = 9'h000 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} ARICAPCTRL_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* TLPHEADER_GbE_VF0_REG supported on:                                        */
/*       BDX (0x403801A0)                                                     */
/* Register default value:              0x1B010017                            */
#define TLPHEADER_GbE_VF0_REG 0x1A0041A0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 * The PCIe TLP Processing Hint (TPH) Requester Capability is an optional extended 
 * capability to support TLP Processing Hints. 
 */
typedef union {
  struct {
    UINT32 cid : 16;
    /* cid - Bits[15:0], RO, default = 16'h0017 
       PCIe extended capability ID indicating TPH capability.
     */
    UINT32 cv : 4;
    /* cv - Bits[19:16], RO, default = 4'h1 
       PCIe TPH extended capability version number.
     */
    UINT32 nco : 12;
    /* nco - Bits[31:20], RO, default = 12'h1B0 
       This field contains the offset to the next PCIe capability structure. If LTR is 
       enabled in EEPROM then the value of this field is 0x1C0 to point to the LTR 
       capability structure. If LTR is disabled then the value of this field is 0x1D0 
       to point to the ACS capability structure. 
        Depends on the EEPROM settings of the LTR_EN bit in Initialization Control Word 
       1 EEPROM word, that controls enabling of the LTR structures. 
     */
  } Bits;
  UINT32 Data;
} TLPHEADER_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* TLPCAP_GbE_VF0_REG supported on:                                           */
/*       BDX (0x403801A4)                                                     */
/* Register default value:              0x00000005                            */
#define TLPCAP_GbE_VF0_REG 0x1A0041A4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 nost : 1;
    /* nost - Bits[0:0], RO, default = 1'b1 
       When set indicates the function is capable of generating requests without using 
       ST. 
     */
    UINT32 ivm : 1;
    /* ivm - Bits[1:1], RO, default = 1'b0 
       Cleared to indicate that this device does not support Interrupt Vector Mode of 
       operation. 
     */
    UINT32 dsm : 1;
    /* dsm - Bits[2:2], RO, default = 1'b1 
       Set to indicate this device supports Device Specific Mode of operation.
     */
    UINT32 rsv3 : 5;
    /* rsv3 - Bits[7:3], RSV, default = 5'h00 
       desc is missing
     */
    UINT32 etph : 1;
    /* etph - Bits[8:8], RO, default = 1'b0 
       Cleared to indicate the the function is not capable of generating requests with 
       Extended TPH TLP Prefix. 
     */
    UINT32 stloc : 2;
    /* stloc - Bits[10:9], RO, default = 2'b0 
       Value indicates if and where the ST Table is located. Defined encodings are: 
       	[br]00b - ST Table is not present
       	[br]01b - ST Table is located in the TPH Requester Capability structure
       	[br]10b - ST Table is located in the MSI-X Table structure
       	[br]11b - reserved
        [br]Default value of 01b indicates that function supports ST table that is 
       located in the TPH Requester Capability structure. 
     */
    UINT32 rsv11 : 5;
    /* rsv11 - Bits[15:11], RSV, default = 5'h0 
       desc is missing
     */
    UINT32 size : 11;
    /* size - Bits[26:16], RO, default = 11'h0 
       System software reads this field to determine the ST_Table_Size N, which is 
       encoded as N-1. This device supports a table with 8 entries. 
     */
    UINT32 rsv27 : 5;
    /* rsv27 - Bits[31:27], RSV, default = 5'h0 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} TLPCAP_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* TPHCTRL_GbE_VF0_REG supported on:                                          */
/*       BDX (0x403801A8)                                                     */
/* Register default value:              0x00000000                            */
#define TPHCTRL_GbE_VF0_REG 0x1A0041A8

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\3.16.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 mode : 3;
    /* mode - Bits[2:0], RWS, default = 3'h0 
       Indicates the ST mode of operation selected. The ST mode encodings are as 
       defined below 
       	[br]000b - No Table Mode
       	[br]001b - Interrupt Vector Mode (not supported)
       	[br]010b - Device Specific Mode
       	[br]others - reserved
       	[br]The default value of 000 indicates No Table mode of operation.
     */
    UINT32 rsv3 : 5;
    /* rsv3 - Bits[7:3], RSV, default = 5'h00 
       desc is missing
     */
    UINT32 enable : 2;
    /* enable - Bits[9:8], RWS, default = 2'h0 
       Define encodings are:
        [br]00b - device is not permitted to issue transactions with TPH or Extended 
       TPH as Requester 
        [br]01b - device is permitted to issue transactions with TPH as Requester and 
       is not permitted to issue transactions with Extended TPH as Requester 
       	[br]10b - reserved
        [br]11b - device is permitted to issue transactions with TPH and Extended TPH 
       as Requester (this device does not issue transactions with Extended TPH) 
       	
     */
    UINT32 rsv10 : 22;
    /* rsv10 - Bits[31:10], RSV, default = 22'h000000 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} TPHCTRL_GbE_VF0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


#endif /* GbE_VF0_h */
