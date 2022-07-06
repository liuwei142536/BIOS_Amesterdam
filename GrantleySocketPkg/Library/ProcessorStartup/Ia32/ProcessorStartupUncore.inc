;
; This file contains an 'Intel Pre-EFI Module' and is licensed
; for Intel CPUs and Chipsets under the terms of your license 
; agreement with Intel or your vendor.  This file may be      
; modified by the user, subject to additional terms of the    
; license agreement                                           
;
;------------------------------------------------------------------------------
;
; Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>
; This software and associated documentation (if any) is furnished
; under a license and may only be used or copied in accordance
; with the terms of the license. Except as permitted by such
; license, no part of this software or documentation may be
; reproduced, stored in a retrieval system, or transmitted in any
; form or by any means without the express written consent of
; Intel Corporation.
;
; Module Name:
;
;  ProcessorStartupUncore.inc
;
; Abstract:
;
;
;------------------------------------------------------------------------------
;
; This is an assembly include file and is      
; licensed for Intel CPUs and chipsets under the terms of your
; license agreement with Intel or your vendor.  This file may 
; be modified by the user, subject to additional terms of the 
; license agreement                                           
;
;------------------------------------------------------------------------------

CPU_FAMILY_HSX     EQU     306Fh         ; bit[19:4] of CPUID_EAX
CPU_FAMILY_BDX_DE  EQU     5066h         ; bit[19:4] of CPUID_EAX - BDX-DE
CPU_FAMILY_BDX     EQU     406Fh         ; bit[19:4] of CPUID_EAX - BDX-EP
A0_REV_HSX         EQU     00h           ; bit[3:0] of CPUID_EAX, Stepping
STEPPING_ID_B0     EQU     01h           ; bit[3:0] of CPUID_EAX, Stepping
A0_REV_BDX         EQU     00h           ; bit[3:0] of CPUID_EAX, Stepping
STEPPING_ID_C0     EQU     02h           ; bit[3:0] of CPUID_EAX, Stepping

; These registers are defined in C header files  CBOBC_IOSAD.h,  UBOX_CFG.h,  UBOX_MISC.h, PCU_FUNC2.h

;-------------------------------------------------------------------------------------------------------------
; CSR register address in legacy PCI format (used via IO port CF8/CFC access method)                                                                            Reg Name in Header File
;-------------------------------------------------------------------------------------------------------------  ------------------------------
CSR_LEGACY_MMIO_RULE3_CBO          EQU  BIT31 OR (1 SHL 16) OR (15 SHL 11) OR (5 SHL 8) OR 058h ;  1:15:5:0x58   MMIO_RULE_3_N0_CBOBC_IOSAD_REG        
CSR_LEGACY_MMIO_RULE7_CBO          EQU  BIT31 OR (1 SHL 16) OR (15 SHL 11) OR (5 SHL 8) OR 078h ;  1:15:5:0x78   MMIO_RULE_7_N0_CBOBC_IOSAD_REG        
CSR_LEGACY_IO_TARGET_LIST_CBO      EQU  BIT31 OR (1 SHL 16) OR (15 SHL 11) OR (5 SHL 8) OR 0E0h ;  1:15:5:0xe0   IOPORT_TARGET_LIST_CBOBC_IOSAD_REG
CSR_LEGACY_MMIO_TARGET_LIST_CBO    EQU  BIT31 OR (1 SHL 16) OR (15 SHL 11) OR (5 SHL 8) OR 0E8h ;  1:15:5:0xe8   MMIO_TARGET_LIST_0_CBOBC_IOSAD_REG   
CSR_LEGACY_MMIO_TARGET_LIST_HI_CBO EQU  BIT31 OR (1 SHL 16) OR (15 SHL 11) OR (5 SHL 8) OR 0F8h ;  1:15:5:0xf8   MMIO_TARGET_LIST_1_CBOBC_IOSAD_REG   (HSX)
CSR_LEGACY_MMCFG_RULE_CBO          EQU  BIT31 OR (1 SHL 16) OR (15 SHL 11) OR (5 SHL 8) OR 0C0h ;  1:15:5:0xc0   MMCFG_RULE_N0_CBOBC_IOSAD_REG         
CSR_LEGACY_MMCFG_RULE_HI_CBO       EQU  BIT31 OR (1 SHL 16) OR (15 SHL 11) OR (5 SHL 8) OR 0C4h ;  1:15:5:0xc0   MMCFG_RULE_N1_CBOBC_IOSAD_REG         
CSR_LEGACY_MMCFG_TARGET_LIST_CBO   EQU  BIT31 OR (1 SHL 16) OR (15 SHL 11) OR (5 SHL 8) OR 0E4h ;  1:15:5:0xe4   MMCFG_TARGET_LIST_CBOBC_IOSAD_REG
CSR_LEGACY_SAD_TARGET_CBO          EQU  BIT31 OR (1 SHL 16) OR (15 SHL 11) OR (5 SHL 8) OR 0F0h ;  1:15:5:0xf0    SAD_TARGET_CBOBC_IOSAD_REG         
CSR_LEGACY_SAD_CONTROL_CBO         EQU  BIT31 OR (1 SHL 16) OR (15 SHL 11) OR (5 SHL 8) OR 0F4h ;  1:15:5:0xf4    SAD_CONTROL_CBOBC_IOSAD_REG
CSR_LEGACY_LOCKCONTROL             EQU  BIT31 OR (1 SHL 16) OR (16 SHL 11) OR (5 SHL 8) OR 050h ;  1:16:5:0x50   LOCKCONTROL_UBOX_CFG_REG    
CSR_LEGACY_CPUBUSNO_UBOX           EQU  BIT31 OR (1 SHL 16) OR (16 SHL 11) OR (7 SHL 8) OR 0D0h ;  1:16:7:0xd0   CPUBUSNO_UBOX_MISC_REG         
CSR_LEGACY_MMCFG_RULE_UBOX         EQU  BIT31 OR (1 SHL 16) OR (16 SHL 11) OR (7 SHL 8) OR 0DCh ;  1:16:7:0xdc   MMCFG_RULE_UBOX_MISC_REG
CSR_LEGACY_MMCFG_IIO               EQU  BIT31 OR (0 SHL 16) OR ( 5 SHL 11) OR (0 SHL 8) OR 090h ;  0:5:0:0x90    IIO 
CSR_LEGACY_MMCFG_IIO_A0            EQU  BIT31 OR (0 SHL 16) OR ( 5 SHL 11) OR (0 SHL 8) OR 084h ;  0:5:0:0x84    IIO 

LEGACY_BIOSSCRATCHPAD3             EQU  LEGACY_BIOSSCRATCHPAD0 + 4*3    ; SSR3  
CSR_LEGACY_R3QCTRL_R3QPI           EQU  BIT31 OR (1 SHL 16) OR (11 SHL 11) OR (0 SHL 8) OR 060h ;  1:11:0:0x60   R3QCTRL_R3QPI    


LEGACY_BIOSSCRATCHPAD0             EQU  BIT31 OR (1 SHL 16) OR (16 SHL 11) OR (7 SHL 8) OR 40h  ; SSR0,  1:16:7:0x40   BIOSSCRATCHPAD1_UBOX_MISC_REG
LEGACY_BIOSSCRATCHPAD2             EQU  LEGACY_BIOSSCRATCHPAD0 + 4*2    ; SSR2,  1:16:7:0x48  
    ; [7:0]       - CPU package present bitmap (QPIRC updates, in all sockets)
    ; Other bits  - QPI Link Valid flags (Used by QPIRC)

SSR_LEGACY_BUS_CONFIG_INFO_CSR     EQU  LEGACY_BIOSSCRATCHPAD0 + 4*3    ;SSR03,  1:16:7:0x4c
         comment ^ 
         Scratch Pad Register 6 Format
         [2:0]   - Total number of buses  assigned to CPU0 in 32 Bus granularity; 00 - 32 buses, 1 - 64 buses ... 7 - 256 buses
         [5:3]   - CPU1 bus range  
         [8:6]   - CPU2 bus range
         [11:9]  - CPU3 bus range
         [14:12] - CPU4 bus range
         [17:15] - CPU5 bus range
         [20:18] - CPU6 bus range
         [23:21] - CPU7 bus range
         [29:24] - MMCFG Base, to store Address [31:26] of MMCFG BASE
         [31:30] - Reserved
         end comment ^

SSR_LEGACY_BOOT_CONFIG_INFO_CSR    EQU  LEGACY_BIOSSCRATCHPAD0 + 4*7    ;SSR07, 1:16:7:0x5c 
         comment ^ 
         Scratch Pad Register 7 Format
         [31] - Enable Protected Processor Inventory Number Feature
         [30] - QPI Cbo TOR Timeout Enable for w/a MSR
         [29] - Remote socket released in LT enabled system
         [28] - FRB error
         [25:27]Reserved
         [24] - DCU mode select
         [23:20] - Total Cbo in a cpu socket. 
         [19:16] - SBSP Sockeet ID
         [15-12] - w/a Ax: MSR_AX_STEPPING_R3QPI bit[3:0]
         [11] - w/a Ax: 0 = not program, 1 = program MSR_AX_STEPPING_R3QPI
         [10] - w/a Ax: MSR_AX_STEPPING_CBOBC bit[3]
         [9-8] - w/a Ax: MSR_AX_STEPPING_CBOBC bit[1:0]
         [7] - w/a Ax: 0 = to not program, 1 = to program MSR_AX_STEPPING_CBOBC
         [6] - hot plug Socket or not
         [5] - Completed Cold Reset Flow
         [4] - Entered Cold Reset Flow
         [3:0]  - Unused
         [0]    - ExtRTID Mode enabled or not
         end comment ^

LEGACY_BIOSNONSTICKYSCRATCHPAD0  EQU  BIT31 OR (1 SHL 16) OR (16 SHL 11) OR (7 SHL 8) OR 60h  ; Non-sticky SR0,  1:16:7:0x60    BIOSNONSTICKYSCRATCHPAD0_UBOX_MISC_REG
;SR_LEGACY_BOOT_HALTED_CSR      EQU  LEGACY_BIOSNONSTICKYSCRATCHPAD0       ; Non-sticky SR0
;SR_LEGACY_BOOT_GO_CSR          EQU  LEGACY_BIOSNONSTICKYSCRATCHPAD0 + 4*1 ; Non-sticky SR1
;SR_LEGACY_PBSP_CHECKIN_CSR     EQU  LEGACY_BIOSNONSTICKYSCRATCHPAD0 + 4*2 ; Non-sticky SR2

;-------------------------------------------------------------------------------------------------------------
; CSR register address used by ReadCpuCsr() and WriteCpuCsr()
;  Address format defined as follows
;     [31:28] = 0h
;     [27:20] = BusType #  ( 0: IIO device,  1: Uncore device)
;     [19:15] = Dev #
;     [14:12] = Func #
;     [11:0]  = Reg offset (dword aligned)
;-------------------------------------------------------------------------------------------------------------
CSR_EMULATION_FLAG_UBOX EQU  (1 SHL 20) OR (16 SHL 15) OR (5 SHL 12) OR 0B0h ;  1:16:5:0xB0    
CSR_CPUNODEID_UBOX      EQU  (1 SHL 20) OR (16 SHL 15) OR (5 SHL 12) OR 40h  ;  1:16:5:0x40   CPUNODEID_UBOX_CFG_REG 
CSR_CPU_BUS_NUMBER_PCU2 EQU  (1 SHL 20) OR (30 SHL 15) OR (2 SHL 12) OR 40h  ;  1:30:2:0x40   CPU_BUS_NUMBER_PCU_FUN2_REG
CSR_SYSTEMSEMAPHORE0    EQU  (1 SHL 20) OR (16 SHL 15) OR (7 SHL 12) OR 0A8h ;  1:16:7:0xa8   SYSTEMSEMAPHORE0_UBOX_MISC_REG 
CSR_GLOBAL_PKG_C_S_CONTROL_PCU EQU  (1 SHL 20) OR (30 SHL 15) OR (2 SHL 12) OR 6ch ; 1:30:2:0x6c  GLOBAL_PKG_C_S_CONTROL_REGISTER_PCU_FUN2_REG
CSR_VCU_MAILBOX_INTERFACE EQU  (1 SHL 20) OR (31 SHL 15) OR (0 SHL 12) OR 40h  ;1:31:0:0x40  VCU_CR_CSR_MAILBOX_INTERFACE_CFG
CSR_VCU_MAILBOX_DATA    EQU  (1 SHL 20) OR (31 SHL 15) OR (0 SHL 12) OR 44h  ; 1:31:0:0x44  VCU_CR_CSR_MAILBOX_DATA_CFG
CSR_CPUBUSNO_IIO        EQU  (0 SHL 20) OR ( 5 SHL 15) OR (0 SHL 12) OR 108h ; 0:5:0:0x108   IIO 
CSR_MMCFG_IIO           EQU  (0 SHL 20) OR ( 5 SHL 15) OR (0 SHL 12) OR 090h ; 0:5:0:0x90    IIO
CSR_MMCFG_IIO_A0        EQU  (0 SHL 20) OR ( 5 SHL 15) OR (0 SHL 12) OR 084h ; 0:5:0:0x84    IIO
CSR_MMCFG_IIO_BASE_LIMIT_GAP EQU 8
CSR_MMCFG_IIO_BASE_LIMIT_GAP_A0 EQU 4
CSR_SAD_TARGET_CBO      EQU  (1 SHL 20) OR (15 SHL 15) OR (5 SHL 12) OR 0F0h ; 1:15:5:0xf0   SAD_TARGET_CBOBC_IOSAD_REG  
CSR_STICKYSCRATCHPAD07  EQU  (1 SHL 20) OR (16 SHL 15) OR (7 SHL 12) OR 05ch ; SSR7,  1:16:7:0x5c   BIOSSCRATCHPAD7
CSR_CAPID4_HSX_BDX      EQU  (1 SHL 20) OR (30 SHL 15) OR (3 SHL 12) OR 094h ; 1:30:3:0x094  CAPID4 for HSX,BDX
CSR_CAPID1_HSX_BDX EQU           (1 SHL 20) OR (30 SHL 15) OR (3 SHL 12) OR 88h  ;uncore:30:3:0x88  

SR_PBSP_CHECKIN_CSR     EQU  (1 SHL 20) OR (16 SHL 15) OR (7 SHL 12) OR 068h ; Non-sticky SR2, 1:16:7:0x68    BIOSNONSTICKYSCRATCHPAD2  
                              ; [30:27] = Stepping of Package BSP
                              ; [23:8] = APIC ID of Package BSP
                              ; [0]    = Socket BSP check-in Flag
                              ; All other bits reserved
CSR_BIOSNONSTICKYSCRATCHPAD0_HSX      EQU (1 SHL 20) OR (16 SHL 15) OR (7 SHL 12) OR 060h   ; Non-sticky scratchpad SR0,  1:16:7:0x60  BIOSNONSTICKYSCRATCHPAD0 
CSR_BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG_HSX EQU  (1 SHL 20) OR (30 SHL 15) OR (1 SHL 12) OR 90h ; 1:30:1:0x90  BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG
CPU_OPTIONS_BIT_DCU_MODE_SEL        EQU  BIT0
CPU_OPTIONS_BIT_DEBUG_INTERFACE_EN  EQU  BIT1

CSR_BIOSNONSTICKYSCRATCHPAD0      EQU (1 SHL 20) OR (16 SHL 15) OR (7 SHL 12) OR 060h   ; Non-sticky scratchpad SR0,  1:16:7:0x60  BIOSNONSTICKYSCRATCHPAD0 
CSR_BIOSNONSTICKYSCRATCHPAD2      EQU (CSR_BIOSNONSTICKYSCRATCHPAD0 +  2*4)             ; Non-sticky scratchpad SR2
CSR_BIOSNONSTICKYSCRATCHPAD13     EQU (CSR_BIOSNONSTICKYSCRATCHPAD0 + 13*4)             ; Non-sticky scratchpad SR13
CSR_BIOSNONSTICKYSCRATCHPAD14     EQU (CSR_BIOSNONSTICKYSCRATCHPAD0 + 14*4)             ; Non-sticky scratchpad SR14
CSR_BIOSNONSTICKYSCRATCHPAD15     EQU (CSR_BIOSNONSTICKYSCRATCHPAD0 + 15*4)             ; Non-sticky scratchpad SR15

SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR    EQU CSR_BIOSNONSTICKYSCRATCHPAD13
SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR     EQU CSR_BIOSNONSTICKYSCRATCHPAD14
SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR        EQU CSR_BIOSNONSTICKYSCRATCHPAD15

PIPE_DISPATCH_SYNCH_PSYSHOST          EQU     4
PIPE_DISPATCH_COMMAND_TEAR_NEM        EQU    -1
PIPE_DISPATCH_COMMAND_PREPARE_RESET   EQU    -2
; pre-defined default values for mmcfg scheme used on cold boot path
DEFAULT_COLDBOOT_MMCFG_RULE          EQU PCIEX_BASE_ADDRESS ; mmcfg.base, with 256 buses

IF MAX_CPU_SOCKETS GT 4
DEFAULT_COLDBOOT_MMCFG_TARGET_LIST   EQU 00FAC688h         ; 0xFAC688 = [7,6,5,4,3,2,1,0], 0x006D2240 = [3,3,2,2,1,1,0,0], 0x00249000 = [1,1,1,1,0,0,0,0]
DEFAULT_COLDBOOT_IIO_BUS_NUMS        EQU 060402000h        ; bus# array = [0xc0, 0x80, 0x40, 0x00] for IIO
DEFAULT_COLDBOOT_UNCORE_BUS_NUMS     EQU 07f5f3f1fh        ; bus# array = [0xff, 0xbf, 0x7f, 0x3f] for uncore
DEFAULT_COLDBOOT_IIO_BUS_NUMS_4_7_S     EQU 0e0c0a080h        ; bus# array = [0xc0, 0x80, 0x40, 0x00] for IIO
DEFAULT_COLDBOOT_UNCORE_BUS_NUMS_4_7_S  EQU 0ffdfbf9fh        ; bus# array = [0xff, 0xbf, 0x7f, 0x3f] for uncore
ELSE
DEFAULT_COLDBOOT_MMCFG_TARGET_LIST   EQU 006D2240h         ; 0xFAC688 = [7,6,5,4,3,2,1,0], 0x006D2240 = [3,3,2,2,1,1,0,0], 0x00249000 = [1,1,1,1,0,0,0,0]
DEFAULT_COLDBOOT_IIO_BUS_NUMS        EQU 0c0804000h        ; bus# array = [0xc0, 0x80, 0x40, 0x00] for IIO
DEFAULT_COLDBOOT_UNCORE_BUS_NUMS     EQU 0ffbf7f3fh        ; bus# array = [0xff, 0xbf, 0x7f, 0x3f] for uncore
ENDIF

; VCU Mailbox API related equates
VCODE_API_OPCODE_SET_PARAMETER       EQU 06h
VCODE_API_SEQ_ID_IOT_LLC_SETUP       EQU 1000Ah
VCODE_API_OPCODE_OPEN_SEQUENCE       EQU 02h
VCODE_API_OPCODE_CLOSE_SEQUENCE      EQU 03h
VCODE_API_OPCODE_IOT_LLC_SETUP       EQU 100Fh
VCODE_API_DELAY_COUNT                EQU 1000h


; macro to read a PCI config dword via lagacy CF8/CFC method
READ_PCI_DWORD_LEGACY macro
; input: EAX = legacy PCI address format with bit31 = 1
; output: EBX = dword register data
;         DX is destroyed
   mov   dx, 0cf8h
   out   dx, eax
   xchg  eax, ebx
   add   dx, 4
   in    eax, dx
   xchg  eax, ebx
endm

; macro to write a PCI config dword via lagacy CF8/CFC method
; input:  EAX = legacy PCI address format with bit31 = 1
;         EBX = dword data to write
; output: DX is destroyed
WRITE_PCI_DWORD_LEGACY macro
   mov   dx, 0cf8h
   out   dx, eax
   xchg  eax, ebx
   add   dx, 4
   out   dx, eax
   xchg  eax, ebx
endm

